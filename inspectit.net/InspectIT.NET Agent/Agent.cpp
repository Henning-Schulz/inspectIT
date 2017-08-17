#include "Agent.h"
#include "dllmain.hpp"

#include <stdio.h>
#include <stdarg.h>
#include <windows.h>
#include <iostream>
#include <sstream>
#include <limits.h>
#include <inttypes.h>

#include "StrategyConfig.h"
#include "TimerSensor.h"
#include "ClassType.h"
#include "AgentConfig.h"

using namespace inspectit;
using namespace inspectit::connection;
using namespace inspectit::config;
using namespace inspectit::data;
using namespace inspectit::instrumentation;
using namespace inspectit::logger;
using namespace inspectit::sending;
using namespace inspectit::sensor;
using namespace inspectit::types;
using namespace inspectit::utils;

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

ICorProfilerInfo3* Agent::getProfilerInfo()
{
	return profilerInfo3;
}

UINT_PTR FunctionMapper(FunctionID functionID, BOOL *pbHookFunction)
{
	// Get SensorInstrumentationPoint

	loggerFactory.staticLog(LEVEL_DEBUG, "FunctionMapper", "FunctionID %i.", functionID);
	// Have to work with mdMethodDef, since the FunctionID is now available for the first time
	mdMethodDef methodDef = getMethodDefForFunctionID(globalAgentInstance->getProfilerInfo(), functionID);
	loggerFactory.staticLog(LEVEL_DEBUG, "FunctionMapper", "Got mdMethodDef %li for FunctionID %i.", methodDef, functionID);

	auto instrumentationManager = globalAgentInstance->getInstrumentationManager();
	auto sensorInstrumentationPoint = instrumentationManager->getSensorInstrumentationPoint(methodDef);

	if (!sensorInstrumentationPoint) {
		loggerFactory.staticLog(LEVEL_DEBUG, "FunctionMapper", "Not instrumenting method with mdMethodDef %li.", methodDef);
		// Nothing to instrument
		*pbHookFunction = 0;
		return functionID;
	}

	// Instantiate sensor

	JAVA_LONG methodId = sensorInstrumentationPoint->getId();

	loggerFactory.staticLog(LEVEL_DEBUG, "FunctionMapper", "Instrumenting method %lli.", methodId);

	auto sensorIds = sensorInstrumentationPoint->getSensorIds();
	for (auto it = sensorIds.begin(); it != sensorIds.end(); it++) {
		auto sensor = instrumentationManager->getMethodSensorForId(*it);

		if (!sensor) {
			loggerFactory.staticLog(LEVEL_ERROR, "FunctionMapper", "There is no sensor with id %lli. Not instrumenting method %lli!", *it, methodId);
			*pbHookFunction = 0;
			return methodId;
		}

		if (sensor->hasHook()) {
			globalAgentInstance->assignHookToMethod(methodId, sensor->getHook());
		}

		if (sensor->hasThreadHook()) {
			globalAgentInstance->addThreadHook(sensor->getThreadHook());
		}
	}

	// Tell the CMR that the instrumentation was applied

	MethodSensorMap methodSensorMap;
	methodSensorMap.addAllSensors(methodId, sensorIds);
	cmrConnection->instrumentationApplied(globalAgentInstance->platformID, methodSensorMap, false);

	*pbHookFunction = 1;

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

	auto agentConfig = cmrConnection->registerPlatform(L"dotNET-agent", L"1"); // TODO: Retrieve the name dynamically
	logger.debug("Got AgentConfig.");
	platformID = agentConfig->getPlatformId();

	logger.debug("Platform ID is %lli.", platformID);

	HRESULT hr;
	hr = pICorProfilerInfoUnk->QueryInterface(IID_ICorProfilerInfo3, (void **)&profilerInfo3);
	if (FAILED(hr)) {
		logger.error("Could not query interface ICorProfilerInfo3!");
		return hr;
	}
	else {
		logger.debug("profilerInfo set.");
	}

	dataSendingService = std::make_shared<DataSendingService>(256); // TODO: Get buffer size from CMR

	instrumentationManager = std::make_shared<InstrumentationManager>(platformID, profilerInfo3, dataSendingService);
	instrumentationManager->addSensorTypeConfigs(agentConfig->getMethodSensorTypeConfigs());
	instrumentationManager->setExcludeClassesPatterns(agentConfig->getExcludeClassesPatterns());

	DWORD eventMask = COR_PRF_MONITOR_ENTERLEAVE | COR_PRF_MONITOR_THREADS | COR_PRF_MONITOR_CLASS_LOADS;

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

	dataSendingService->stopSending();
	logger.debug("Unregistering platform...");
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

	logger.debug("Class %i loaded.", classId);

	std::shared_ptr<ClassType> classType = createClassTypeFromId(profilerInfo3, classId);

	if (!classType) {
		logger.warn("Could not create ClassType for ClassID %i! Aborting.", classId);
		return S_OK;
	}

	logger.debug("Class %ls loaded. Modifiers are 0x%lx.", classType->getFqn().c_str(), classType->getModifiers());

	// Check if excluded --> do not ask CMR in this case
	if (!instrumentationManager->isExcluded(classType)) {
		std::shared_ptr<InstrumentationDefinition> instrumentationDefinition = cmrConnection->analyze(platformID, classType);

		if (instrumentationDefinition) {
			instrumentationManager->registerInstrumentationDefinition(classType, instrumentationDefinition);
		}
		else {
			logger.debug("Received no instrumentation definition for class %ls.", classType->getFqn().c_str());
		}
	}

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