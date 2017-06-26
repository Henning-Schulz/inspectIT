#include "Agent.h"
#include "dllmain.hpp"

#include <stdio.h>
#include <stdarg.h>
#include <windows.h>
#include <iostream>
#include <sstream>
#include <limits.h>
#include <inttypes.h>

#include "SimpleBufferStrategy.h"
#include "ListSizeStrategy.h"
#include "StrategyConfig.h"
#include "TimerSensor.h"
#include "ClassType.h"
#include "AgentConfig.h"

Agent *globalAgentInstance;

Agent::Agent()
{
}


Agent::~Agent()
{
	logger.debug("Destructor.");
}


void Agent::assignHookToMethod(METHOD_ID methodId, std::shared_ptr<MethodHook> hook)
{
	std::unique_lock<std::shared_mutex> lock(mHookAssignment);

	auto it = hookAssignment.find(methodId);

	if (it == hookAssignment.end()) {
		auto pair = hookAssignment.emplace(methodId, std::vector<std::shared_ptr<MethodHook>>());
		it = pair.first;
	}

	auto hit = it->second.begin();
	while (hit != it->second.end() && (*hit)->getPriority() >= hook->getPriority()) {
		hit++;
	}

	it->second.insert(hit, hook);
	logger.debug("There are now %i hooks assigned to method %lli.", hookAssignment.find(methodId)->second.size(), methodId);
}

void Agent::addThreadHook(std::shared_ptr<ThreadHook> hook)
{
	threadHookList.push_back(hook);
}


//
// Method callbacks
//

void Agent::Enter(METHOD_ID methodID)
{
	std::shared_lock<std::shared_mutex> lock(mHookAssignment);
	auto it = hookAssignment.find(methodID);
	if (it == hookAssignment.end()) {
		return;
	}

	// Highest priority first
	for (auto hit = it->second.begin(); hit != it->second.end(); hit++) {
		(*hit)->beforeBody(methodID);
	}
}

void Agent::Leave(METHOD_ID methodID)
{
	std::shared_lock<std::shared_mutex> lock(mHookAssignment);
	auto it = hookAssignment.find(methodID);
	if (it == hookAssignment.end()) {
		return;
	}

	// Highest priority last
	for (auto hit = it->second.rbegin(); hit != it->second.rend(); hit++) {
		(*hit)->afterBody(methodID);
	}
}

void Agent::Tailcall(METHOD_ID methodID)
{
	// currently not implemented
	logger.info("Tailcall (%i)", methodID);
}

EXTERN_C void __stdcall EnterStub(METHOD_ID methodID)
{
	globalAgentInstance->Enter(methodID);
}

EXTERN_C void __stdcall LeaveStub(METHOD_ID methodID)
{
	globalAgentInstance->Leave(methodID);
}

EXTERN_C void __stdcall TailcallStub(METHOD_ID methodID)
{
	globalAgentInstance->Tailcall(methodID);
}

void __declspec(naked) EnterNaked3(FunctionIDOrClientID functionIDOrClientID)
{
	__asm
	{
		push eax
		push ecx
		push edx
		push[esp + 16]
		call EnterStub
		pop edx
		pop ecx
		pop eax
		ret 4
	}
}

void __declspec(naked) LeaveNaked3(FunctionIDOrClientID functionIDOrClientID)
{
	__asm
	{
		push eax
		push ecx
		push edx
		push[esp + 16]
		call LeaveStub
		pop edx
		pop ecx
		pop eax
		ret 4
	}
}

void __declspec(naked) TailcallNaked3(FunctionIDOrClientID functionIDOrClientID)
{
	__asm
	{
		push eax
		push ecx
		push edx
		push[esp + 16]
		call TailcallStub
		pop edx
		pop ecx
		pop eax
		ret 4
	}
}

std::shared_ptr<InstrumentationManager> Agent::getInstrumentationManager()
{
	return instrumentationManager;
}

UINT_PTR FunctionMapper(FunctionID functionID, BOOL *pbHookFunction)
{
	auto instrumentationManager = globalAgentInstance->getInstrumentationManager();
	auto sensorInstrumentationPoint = instrumentationManager->getSensorInstrumentationPoint(functionID);

	if (!sensorInstrumentationPoint) {
		// Nothing to instrument
		*pbHookFunction = 0;
		return functionID;
	}

	JAVA_LONG methodId = sensorInstrumentationPoint->getId();

	auto sensorIds = sensorInstrumentationPoint->getSensorIds();
	for (auto it = sensorIds.begin(); it != sensorIds.end(); it++) {
		auto sensor = instrumentationManager->getMethodSensorForId(*it);

		if (sensor->hasHook()) {
			globalAgentInstance->assignHookToMethod(methodId, sensor->getHook());
		}

		if (sensor->hasThreadHook()) {
			globalAgentInstance->addThreadHook(sensor->getThreadHook());
		}
	}

	return methodId;
}

//
// Thread callbacks
//

HRESULT Agent::ThreadCreated(ThreadID threadID)
{
	logger.debug("Thread %i created", threadID);

	if (threadHookList.empty()) {
		return S_OK;
	}

	for (auto it = threadHookList.begin(); it != threadHookList.end(); it++) {
		(*it)->threadCreated(threadID);
	}

	return S_OK;
}

HRESULT Agent::ThreadDestroyed(ThreadID threadID)
{
	logger.debug("Thread %i destroyed", threadID);

	if (threadHookList.empty()) {
		return S_OK;
	}

	for (auto it = threadHookList.begin(); it != threadHookList.end(); it++) {
		(*it)->threadDestroyed(threadID);
	}

	return S_OK;
}


//
// Startup/shutdown
//

HRESULT Agent::Initialize(IUnknown *pICorProfilerInfoUnk)
{
	globalAgentInstance = this;
	logger.debug("Set globalAgentInstance to this");

	if (setupCMRConnection()) {
		logger.info("Successfully connected to CMR.");
	}
	else {
		logger.error("Connection to CMR failed. Shutting down...");
		return E_FAIL;
	}

	// Register agent

	auto agentConfig = cmrConnection->registerPlatform(L".NET agent", L"0.1"); // TODO: Retrieve the name dynamically
	platformID = agentConfig->getPlatformId();

	HRESULT hr;
	hr = pICorProfilerInfoUnk->QueryInterface(IID_ICorProfilerInfo3, (void **)&profilerInfo3);
	if (FAILED(hr)) {
		logger.error("Could not query interface ICorProfilerInfo3!");
		return hr;
	}
	else {
		logger.debug("profilerInfo set.");
	}

	instrumentationManager = std::make_shared<InstrumentationManager>(platformID, profilerInfo3);

	DWORD eventMask = COR_PRF_MONITOR_ENTERLEAVE | COR_PRF_MONITOR_THREADS;

	// TODO: Add flags to event mask if necessary

	hr = profilerInfo3->SetEventMask(eventMask);
	if (FAILED(hr)) {
		logger.error("Could not set event mask!");
		return hr;
	}

	hr = profilerInfo3->SetEnterLeaveFunctionHooks3((FunctionEnter3 *)EnterNaked3,
		(FunctionLeave3 *)LeaveNaked3, (FunctionTailcall3 *)TailcallNaked3);
	if (FAILED(hr)) {
		logger.error("Could not set enter leave function hooks!");
		return hr;
	}

	hr = profilerInfo3->SetFunctionIDMapper((FunctionIDMapper *)&FunctionMapper);
	if (FAILED(hr)) {
		logger.error("Could not set function ID mapper!");
		return hr;
	}

	alive = true;
	keepAliveThread = std::thread([this]() { keepAlive(); });

	logger.info("Initialized successfully");

	return S_OK;
}

HRESULT Agent::Shutdown()
{
	logger.debug("Shutdown...");
	shutdownCounter++;

	shutdownDataSendingService();
	cmrConnection->unregisterPlatform(platformID);
	alive = false;
	keepAliveThread.join();
	shutdownCMRConnection();
	logger.debug("Shutdown finished.");
	return S_OK;
}

HRESULT Agent::ClassLoadFinished(ClassID classId, HRESULT hrStatus)
{
	if (FAILED(hrStatus)) {
		logger.warn("Class with id %i was not properly loaded! Aborting.", classId);
		return S_OK;
	}

	std::shared_ptr<ClassType> classType = createClassTypeFromId(profilerInfo3, classId);

	if (!classType) {
		logger.error("Error during creation of ClassType for ClassID %i. Aborting.", classId);
		return E_FAIL;
	}

	// Send it to the CMR and receive instrumentation definition

	std::shared_ptr<InstrumentationDefinition> instrumentationDefinition = cmrConnection->analyze(platformID, classType);

	// Register instrumentation definition at the InstrumentationManager

	instrumentationManager->registerInstrumentationDefinition(classType, instrumentationDefinition);

	return S_OK;
}

HRESULT Agent::DllDetachShutdown()
{
	logger.debug("DllDetachShutdown...");
	//
	// If no shutdown occurs during DLL_DETACH, release the callback
	// interface pointer. This scenario will more than likely occur
	// with any interop related program (e.g., a program that is 
	// comprised of both managed and unmanaged components).
	//
	shutdownCounter++;
	if ((shutdownCounter == 1) && (globalAgentInstance != NULL))
	{
		logger.debug("Release global agent instance...");
		globalAgentInstance->Release();
		globalAgentInstance = NULL;
		logger.debug("Released.");
	}

	logger.debug("DllDetachShutdown finished.");

	return S_OK;
}

void Agent::keepAlive()
{
	while (alive) {
		cmrConnection->sendKeepAlive(platformID, !alive);
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
}


//
// IUnknown methods
//

ULONG Agent::AddRef()
{
	return InterlockedIncrement(&m_refCount);
}

ULONG Agent::Release()
{
	long refCount;

	refCount = InterlockedDecrement(&m_refCount);
	if (refCount == 0)
		delete this;

	return refCount;
}

HRESULT Agent::QueryInterface(REFIID riid, void **ppInterface)
{
	if (riid == IID_IUnknown)
		*ppInterface = static_cast<IUnknown *>(this);
	else if (riid == IID_ICorProfilerCallback)
		*ppInterface = static_cast<ICorProfilerCallback *>(this);
	else if (riid == IID_ICorProfilerCallback2)
		*ppInterface = static_cast<ICorProfilerCallback2 *>(this);
	else if (riid == IID_ICorProfilerCallback3)
		*ppInterface = static_cast<ICorProfilerCallback3 *>(this);
	else
	{
		*ppInterface = NULL;
		return E_NOINTERFACE;
	}

	reinterpret_cast<IUnknown *>(*ppInterface)->AddRef();

	return S_OK;
}

HRESULT Agent::CreateObject(REFIID riid, void **ppInterface)
{
	HRESULT hr = E_NOINTERFACE;


	*ppInterface = NULL;
	if ((riid == IID_IUnknown)
		|| (riid == IID_ICorProfilerCallback)
		|| (riid == IID_ICorProfilerCallback2)
		|| (riid == IID_ICorProfilerCallback3))
	{
		Agent *pProfilerCallback;


		pProfilerCallback = new Agent();
		if (pProfilerCallback != NULL)
		{
			printf("New Agent object created\n");

			hr = S_OK;

			pProfilerCallback->AddRef();

			*ppInterface = static_cast<ICorProfilerCallback *>(pProfilerCallback);
		}
		else
			hr = E_OUTOFMEMORY;
	}


	return hr;

}