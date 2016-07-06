#include "Agent.h"
#include "dllmain.hpp"
#include "HelloSensor.h"

#include <stdio.h>
#include <stdarg.h>
#include <windows.h>
#include <iostream>
#include <sstream>
#include <limits.h>
#include <inttypes.h>

#include "SingleClassHookStrategy.h"
#include "SimpleBufferStrategy.h"
#include "ListSizeStrategy.h"
#include "ShadowStackHook.h"
#include "StrategyConfig.h"
#include "ListSizeStrategyConfig.h"
#include "TimeStrategyConfig.h"
#include "SimpleBufferStrategyConfig.h"
#include "SizeBufferStrategyConfig.h"
#include "MethodSensorConfig.h"
#include "StackTraceSensorConfig.h"
#include "MethodSensorAssignment.h"
#include "StackTraceSamplingSensor.h"
#include "AssignmentHookStrategy.h"

Agent *globalAgentInstance;

Agent::Agent()
{
}


Agent::~Agent()
{
	logger.debug("Deconstructor...");
	delete hookStrategy;
	logger.debug("Deconstructor finished.");
}


void Agent::addMethodHook(std::shared_ptr<MethodHook> hook)
{
	methodHookList.push_back(hook);
}

void Agent::removeMethodHook(std::shared_ptr<MethodHook> hook)
{
	methodHookList.remove(hook);
}

void Agent::removeAllMethodHooks()
{
	methodHookList.clear();
}

HookStrategy* Agent::getHookStrategy()
{
	return hookStrategy;
}

size_t Agent::getNumberOfMethodHooks()
{
	return methodHookList.size();
}


//
// Method callbacks
//

void Agent::Enter(METHOD_ID methodID)
{
	if (methodHookList.empty()) {
		return;
	}

	for (std::list<std::shared_ptr<MethodHook>>::iterator it = methodHookList.begin(); it != methodHookList.end(); it++) {
		(*it)->beforeBody(methodID);
	}
}

void Agent::Leave(METHOD_ID methodID)
{
	if (methodHookList.empty()) {
		return;
	}

	for (std::list<std::shared_ptr<MethodHook>>::iterator it = methodHookList.begin(); it != methodHookList.end(); it++) {
		(*it)->afterBody(methodID);
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

UINT_PTR  FunctionMapper(FunctionID functionID, BOOL *pbHookFunction)
{
	if (globalAgentInstance->getNumberOfMethodHooks() == 0) {
		*pbHookFunction = 0;
		return functionID;
	}

	WCHAR className[MAX_BUFFERSIZE];
	WCHAR methodName[MAX_BUFFERSIZE];
	WCHAR returnType[MAX_BUFFERSIZE];
	JAVA_INT javaModifiers;
	std::vector<LPWSTR> parameterTypes; // elements will be created on the heap --> need to delete them afterwards
	globalAgentInstance->getMethodSpecifics(functionID, className, methodName, returnType, &javaModifiers, &parameterTypes);

	if (globalAgentInstance->getHookStrategy()->hook(className, methodName, parameterTypes, javaModifiers)) {
		LogLevel level = loggerFactory.getLogLevel();

		if (level >= LEVEL_DEBUG) {
			loggerFactory.staticLogWithoutNewLine(LEVEL_DEBUG, "FunctionMapper", "Hook 0x%03x %ls %ls.%ls(", javaModifiers, returnType, className, methodName);
			bool first = true;
			for (std::vector<LPWSTR>::iterator it = parameterTypes.begin(); it != parameterTypes.end(); it++) {
				if (first) {
					first = false;
				}
				else {
					loggerFactory.printf(", ");
				}
				loggerFactory.printf("%ls", *it);
			}
			loggerFactory.printf(")\n");
		}

		JAVA_LONG methodId = cmrConnection->registerMethod(globalAgentInstance->platformID, className, methodName, returnType, parameterTypes, javaModifiers);

		if (methodId > UINT_MAX) {
			loggerFactory.staticLog(LEVEL_ERROR, "FunctionMapper", "methodId %lld is greater than maximum value of unsigned int (%u)! Disabling hook.\n", methodId, UINT_MAX);
			*pbHookFunction = 0;
		}
		else if (methodId < 0) {
			loggerFactory.staticLog(LEVEL_ERROR, "FunctionMapper", "methodId %lld is less than zero! Disabling hook.\n", methodId);
			*pbHookFunction = 0;
		}
		else {
			*pbHookFunction = 1;
		}

		return methodId;
	}
	else {
		*pbHookFunction = 0;
	}

	// TODO

	// delete the elements on the heap
	parameterTypes.clear();

	return (UINT_PTR)functionID;
}

//
// Thread callbacks
//

HRESULT Agent::ThreadCreated(ThreadID threadID)
{
	logger.debug("Thread %i created", threadID);

	if (initializationFinished) {
		if (threadHookList.empty()) {
			return S_OK;
		}

		for (auto it = threadHookList.begin(); it != threadHookList.end(); it++) {
			(*it)->threadCreated(threadID);
		}
	}
	else {
		std::unique_lock<std::mutex> lock(mCreatedThreads);

		createdThreads.push_back(threadID);
	}

	return S_OK;
}

HRESULT Agent::ThreadDestroyed(ThreadID threadID)
{
	printf("Thread destroyed\n");
	logger.debug("Thread %i destroyed", threadID);

	if (initializationFinished) {
		if (threadHookList.empty()) {
			return S_OK;
		}

		for (auto it = threadHookList.begin(); it != threadHookList.end(); it++) {
			(*it)->threadDestroyed(threadID);
		}
	}
	else {
		std::unique_lock<std::mutex> lock(mCreatedThreads);

		for (auto ptid = createdThreads.begin(); ptid != createdThreads.end(); ptid++) {
			if (*ptid == threadID) {
				createdThreads.erase(ptid);
				break;
			}
		}
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

	// TODO...
	platformID = cmrConnection->registerPlatform(getAllDefinedIPs(), L".NET agent", L"0.1");

	std::shared_ptr<StrategyConfig> bufferStrategyConfig = cmrConnection->getBufferStrategyConfig(platformID);
	std::shared_ptr<BufferStrategy> bufferStrategy;

	if (wcscmp(bufferStrategyConfig->getClassName(), SimpleBufferStrategyConfig::CLASS_NAME) == 0) {
		logger.debug("Buffer strategy is %ls", bufferStrategyConfig->getClassName());
		bufferStrategy = std::make_shared<SimpleBufferStrategy>();
	}
	else if (wcscmp(bufferStrategyConfig->getClassName(), SizeBufferStrategyConfig::CLASS_NAME) == 0) {
		std::shared_ptr<SizeBufferStrategyConfig> config = std::static_pointer_cast<SizeBufferStrategyConfig>(bufferStrategyConfig);
		logger.debug("Buffer strategy is %ls with size %ld", bufferStrategyConfig->getClassName(), config->getSize());
		// TODO
		logger.error("Not yet implemented! Shutting dowm.");
		return E_FAIL;
	}
	else {
		logger.error("Buffer strategy %ls is not supported. Shutting down...");
		return E_FAIL;
	}

	std::shared_ptr<StrategyConfig> sendingStrategyConfig = cmrConnection->getSendingStrategyConfig(platformID);
	
	if (wcscmp(sendingStrategyConfig->getClassName(), ListSizeStrategyConfig::CLASS_NAME) == 0) {
		std::shared_ptr<ListSizeStrategyConfig> config = std::static_pointer_cast<ListSizeStrategyConfig>(sendingStrategyConfig);
		logger.debug("Sending strategy is %ls with listSize %ld", sendingStrategyConfig->getClassName(), config->getListSize());

		std::shared_ptr<ListSizeStrategy> sendingStrategy = std::make_shared<ListSizeStrategy>(config->getListSize());
		startDataSendingService(bufferStrategy, sendingStrategy);
		sendingStrategy->setDataSendingService(dataSendingService);
		dataSendingService->addListSizeListener(sendingStrategy);
	}
	else if (wcscmp(sendingStrategyConfig->getClassName(), TimeStrategyConfig::CLASS_NAME) == 0) {
		std::shared_ptr<TimeStrategyConfig> config = std::static_pointer_cast<TimeStrategyConfig>(sendingStrategyConfig);
		logger.debug("Sending strategy is %ls with time %lld", sendingStrategyConfig->getClassName(), config->getTime());
		// TODO
		logger.error("Not yet implemented! Shutting dowm.");
		return E_FAIL;
	} else {
		logger.error("Sending strategy %ls is not supported. Shutting down...");
		return E_FAIL;
	}

	std::vector<std::shared_ptr<MethodSensorConfig>> sensorConfigs = cmrConnection->getMethodSensorConfigs(platformID);
	logger.debug("Sensor configs retrieved.");

	for (auto it = sensorConfigs.begin(); it != sensorConfigs.end(); it++) {
		logger.debug("Using sensor config %ls", (*it)->getClassName());
	}

	std::vector<std::shared_ptr<MethodSensorAssignment>> sensorAssignements = cmrConnection->getMethodSensorAssignments(platformID);
	AssignmentHookStrategy *ahs = new AssignmentHookStrategy();
	hookStrategy = ahs;

	HRESULT hr;
	hr = pICorProfilerInfoUnk->QueryInterface(IID_ICorProfilerInfo3,
		(void **)&profilerInfo3);
	if (FAILED(hr)) {
		logger.error("Could not query interface ICorProfilerInfo3!");
		return hr;
	}

	bool stackTraceSensorCreated = false;
	DWORD eventMask = COR_PRF_MONITOR_ENTERLEAVE;

	for (auto it = sensorAssignements.begin(); it != sensorAssignements.end(); it++) {
		std::shared_ptr<MethodSensorAssignment> ass = *it;
		logger.debug("Using sensor assignment for sensor %ls with parameters:", ass->getSensorClassName());
		logger.debug("\tclassName: %ls", ass->getClassName().c_str());
		logger.debug("\tsuperclass: %s", (ass->isSuperclass() ? "true" : "false"));
		logger.debug("\tinterface: %s", (ass->isInterface() ? "true" : "false"));
		logger.debug("\tmethodName: %ls", ass->getMethodName().c_str());
		logger.debug("\tparameters: %s", (ass->getParameters().empty() ? "*" : "..."));
		logger.debug("\tconstructor: %s", (ass->isConstructor() ? "true" : "false"));
		logger.debug("\tpublicModifier: %s", (ass->isPublicModifier() ? "true" : "false"));
		logger.debug("\tprotectedModifier: %s", (ass->isProtectedModifier() ? "true" : "false"));
		logger.debug("\tprivateModifier: %s", (ass->isPrivateModifier() ? "true" : "false"));
		logger.debug("\tdefaultModifier: %s", (ass->isDefaultModifier() ? "true" : "false"));

		std::shared_ptr<MethodSensor> sensor;
		std::shared_ptr<MethodSensorConfig> config;

		if (wcscmp(ass->getSensorClassName(), StackTraceSensorConfig::CLASS_NAME) == 0) {
			if (!stackTraceSensorCreated) {
				for (auto it = sensorConfigs.begin(); it != sensorConfigs.end(); it++) {
					if (wcscmp((*it)->getClassName(), ass->getSensorClassName()) == 0) {
						config = *it;
						break;
					}
				}

				sensor = std::make_shared<StackTraceSamplingSensor>();
				stackTraceSensorCreated = true;
				logger.debug("Created StackTraceSamplingSensor.");
			}

			ahs->addAssignment(ass);
			logger.debug("Added assignment for %ls", ass->getSensorClassName());
		}
		else {
			logger.warn("Sensor %ls is not supported!", ass->getSensorClassName());
			continue;
		}

		WCHAR className[MAX_BUFFERSIZE];
		getMethodSensorClassName(sensor, className);
		JAVA_LONG sensorTypeId = cmrConnection->registerMethodSensorType(platformID, className);
		logger.debug("Sensor %ls has id %lli", className, sensorTypeId);
		sensor->init(config, sensorTypeId, platformID, profilerInfo3);
		methodSensorList.push_back(sensor);

		if (sensor->hasHook()) {
			methodHookList.push_back(sensor->getHook());
		}

		if (sensor->hasThreadHook()) {
			threadHookList.push_back(sensor->getThreadHook());
			eventMask |= COR_PRF_MONITOR_THREADS;
			logger.debug("Adding thread hook");
		}

		eventMask |= sensor->getSpecialMonitorFlags();
	}

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

	{
		std::unique_lock<std::mutex> lock(mCreatedThreads);

		for (auto pth = threadHookList.begin(); pth != threadHookList.end(); pth++) {
			for (auto ptid = createdThreads.begin(); ptid != createdThreads.end(); ptid++) {
				(*pth)->threadCreated(*ptid, true);
			}
		}
	}

	initializationFinished = true;

	logger.info("Initialized successfully");

	return S_OK;
}

HRESULT Agent::Shutdown()
{
	shutdownCounter++;

	for (auto it = methodSensorList.begin(); it != methodSensorList.end(); it++) {
		(*it)->notifyShutdown();
	}

	shutdownDataSendingService();
	cmrConnection->unregisterPlatform(getAllDefinedIPs(), L".NET agent");
	alive = false;
	keepAliveThread.join();
	shutdownCMRConnection();
	return S_OK;
}

HRESULT Agent::DllDetachShutdown()
{
	//
	// If no shutdown occurs during DLL_DETACH, release the callback
	// interface pointer. This scenario will more than likely occur
	// with any interop related program (e.g., a program that is 
	// comprised of both managed and unmanaged components).
	//
	shutdownCounter++;
	if ((shutdownCounter == 1) && (globalAgentInstance != NULL))
	{
		globalAgentInstance->Release();
		globalAgentInstance = NULL;
	}

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

BOOL Agent::getMethodSpecifics(FunctionID functionID, LPWSTR wszClass, LPWSTR wszMethod, LPWSTR returnType,
	JAVA_INT *javaModifiers, std::vector<LPWSTR> *parameterTypes) {
	return getSpecificsOfMethod(profilerInfo3, functionID, wszClass, wszMethod, returnType, javaModifiers, parameterTypes);
}