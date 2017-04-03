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
#include "MethodSensorAssignment.h"
#include "AssignmentHookStrategy.h"
#include "TimerSensorConfig.h"
#include "TimerSensor.h"

Agent *globalAgentInstance;

Agent::Agent()
{
}


Agent::~Agent()
{
	logger.debug("Deconstructor...");
	logger.debug("Deconstructor finished.");
}

std::shared_ptr<std::unique_lock<std::mutex>> Agent::getFunctionMapperLock(ThreadID theThreadId)
{
	ThreadID threadId = 0;
	if (theThreadId == 0) {
		HRESULT hr = profilerInfo3->GetCurrentThreadID(&threadId);
		if (FAILED(hr)) {
			logger.error("Could not get thread id!");
		}
	}
	else {
		threadId = theThreadId;
	}
	logger.debug("Get FunctionMapper lock for thread %i.", threadId);

	auto mut = functionMapperMutexMap[threadId];

	std::shared_ptr<std::unique_lock<std::mutex>> lock = std::make_shared<std::unique_lock<std::mutex>>(*mut);
	return lock;
}

void Agent::addMethodHook(std::shared_ptr<MethodHook> hook, std::shared_ptr<HookStrategy> hookStrategy)
{
	methodHookList.push_back(std::make_pair(hook, hookStrategy));
}

void Agent::removeMethodHook(std::shared_ptr<MethodHook> hook)
{
	for (auto it = methodHookList.begin(); it != methodHookList.end(); ) {
		if (it->first == hook) {
			it = methodHookList.erase(it);
			break;
		}
		else {
			it++;
		}
	}
}

void Agent::removeAllMethodHooks()
{
	methodHookList.clear();
}

size_t Agent::getNumberOfMethodHooks()
{
	return methodHookList.size();
}

std::list<std::pair<std::shared_ptr<MethodHook>, std::shared_ptr<HookStrategy>>> Agent::getMethodHooksWithStrategies()
{
	return methodHookList;
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
	logger.debug("There are now %i hooks assigned to this method.", hookAssignment.find(methodId)->second.size());
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

UINT_PTR  FunctionMapper(FunctionID functionID, BOOL *pbHookFunction)
{
	auto functionMapperLock = globalAgentInstance->getFunctionMapperLock();

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

	auto methodHooksAndStrategies = globalAgentInstance->getMethodHooksWithStrategies();

	boolean hookRegistered = false;
	METHOD_ID methodId = functionID;

	for (auto it = methodHooksAndStrategies.begin(); it != methodHooksAndStrategies.end(); it++) {
		if (it->second->hook(className, methodName, parameterTypes, javaModifiers)) {
			if (!hookRegistered) {
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

				methodId = cmrConnection->registerMethod(globalAgentInstance->platformID, className, methodName, returnType, parameterTypes, javaModifiers);

				if (methodId > UINT_MAX) {
					loggerFactory.staticLog(LEVEL_ERROR, "FunctionMapper", "methodId %lld is greater than maximum value of unsigned int (%u)! Disabling hook.\n", methodId, UINT_MAX);
					*pbHookFunction = 0;
					break;
				}
				else if (methodId < 0) {
					loggerFactory.staticLog(LEVEL_ERROR, "FunctionMapper", "methodId %lld is less than zero! Disabling hook.\n", methodId);
					*pbHookFunction = 0;
					break;
				}
				else {
					*pbHookFunction = 1;
				}

				hookRegistered = true;
			}

			// Function hook is activated and method is registered
			
			loggerFactory.staticLog(LEVEL_DEBUG, "FunctionMapper", "Assigning method %i to hook of sensor %lli", methodId, it->first->getSensorTypeId());
			globalAgentInstance->assignHookToMethod(methodId, it->first);
		}
	}

	if (!hookRegistered) {
		*pbHookFunction = 0;
	}

	// delete the elements on the heap
	parameterTypes.clear();

	functionMapperLock->unlock();

	return methodId;
}

//
// Thread callbacks
//

HRESULT Agent::ThreadCreated(ThreadID threadID)
{
	logger.debug("Thread %i created", threadID);

	std::shared_ptr<std::mutex> mut = std::make_shared<std::mutex>();
	{
		std::unique_lock<std::mutex> uniqueLock(mFunctionMapperMutexMap);
		functionMapperMutexMap.emplace(threadID, mut);
	}

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

	{
		std::unique_lock<std::mutex> uniqueLock(mFunctionMapperMutexMap);
		functionMapperMutexMap.erase(threadID);
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
	logger.debug("Sensor assignments retrieved.");

	HRESULT hr;
	hr = pICorProfilerInfoUnk->QueryInterface(IID_ICorProfilerInfo3,
		(void **)&profilerInfo3);
	if (FAILED(hr)) {
		logger.error("Could not query interface ICorProfilerInfo3!");
		return hr;
	}
	else {
		logger.debug("profilerIfo set.");
	}

	DWORD eventMask = COR_PRF_MONITOR_ENTERLEAVE | COR_PRF_MONITOR_THREADS;

	std::vector<std::wstring> excludedClasses = cmrConnection->getExcludedClasses(platformID);

	logger.debug("Excluded classes are:");
	for (auto it = excludedClasses.begin(); it != excludedClasses.end(); it++) {
		printf("%ls, ", it->c_str());
	}
	printf("\n");

	for (auto it = sensorConfigs.begin(); it != sensorConfigs.end(); it++) {
		logger.debug("Creating sensor %ls...", (*it)->getClassName());

		bool isStackTraceSensor = wcscmp((*it)->getClassName(), StackTraceSensorConfig::CLASS_NAME) == 0;
		bool isTimerSensor = wcscmp((*it)->getClassName(), TimerSensorConfig::CLASS_NAME) == 0;

		if (isStackTraceSensor || isTimerSensor) {
			std::shared_ptr<StackTraceSensorConfig> config = std::static_pointer_cast<StackTraceSensorConfig>(*it);

			std::shared_ptr<AssignmentHookStrategy> hookStrategy = std::make_shared<AssignmentHookStrategy>();
			hookStrategy->setExcludedClasses(excludedClasses);

			// StackTraceSamplingSensor
			std::shared_ptr<AssignmentHookStrategy> baseMethodHookStrategy;
			if (isStackTraceSensor) {
				baseMethodHookStrategy = std::make_shared<AssignmentHookStrategy>();
				baseMethodHookStrategy->setExcludedClasses(excludedClasses);
			}

			for (auto ait = sensorAssignements.begin(); ait != sensorAssignements.end(); ait++) {
				if (wcscmp((*it)->getClassName(), (*ait)->getSensorClassName()) == 0) {
					std::shared_ptr<MethodSensorAssignment> ass = *ait;
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

					hookStrategy->addAssignment(ass);

					// StackTraceSamplingSensor
					if (isStackTraceSensor) {
						std::shared_ptr<StackTraceSensorAssignment> stsConfig = std::static_pointer_cast<StackTraceSensorAssignment>(ass);
						baseMethodHookStrategy->addAssignment(stsConfig->getBaseMethodAssignment());
					}
				}
			}

			if (!hookStrategy->isEmpty()) {
				// Only create sensor if there is an assigment
				std::shared_ptr<MethodSensor> sensor;

				if (isStackTraceSensor) {
					sensor = std::make_shared<StackTraceSamplingSensor>();
					logger.debug("Created StackTraceSamplingSensor.");
				}
				else {
					sensor = std::make_shared<TimerSensor>();
					logger.debug("Created TimerSensor.");
				}

				JAVA_LONG sensorTypeId = cmrConnection->registerMethodSensorType(platformID, config->getClassName());
				logger.debug("Sensor %ls has id %lli", config->getClassName(), sensorTypeId);
				sensor->init(config, sensorTypeId, platformID, profilerInfo3);
				methodSensorList.push_back(sensor);

				// StackTraceSamplingSensor
				if (isStackTraceSensor) {
					std::static_pointer_cast<StackTraceSamplingSensor>(sensor)->getProvider()->setHookStrategy(hookStrategy);
				}

				if (sensor->hasHook()) {
					methodHookList.push_back(std::make_pair(sensor->getHook(), hookStrategy));
					logger.debug("Added hook.");
				}

				// StackTraceSamplingSensor
				if (isStackTraceSensor && std::static_pointer_cast<StackTraceSamplingSensor>(sensor)->hasBaseMethodHook()) {
					methodHookList.push_back(std::make_pair(std::static_pointer_cast<StackTraceSamplingSensor>(sensor)->getBaseMethodHook(), baseMethodHookStrategy));
					logger.debug("Added base method hook.");
				}

				if (sensor->hasThreadHook()) {
					threadHookList.push_back(sensor->getThreadHook());
					//eventMask |= COR_PRF_MONITOR_THREADS; is always activated
					logger.debug("Adding thread hook");
				}

				eventMask |= sensor->getSpecialMonitorFlags();
			}
		}
		else {
			logger.warn("Sensor %ls is currently not supported!", (*it)->getClassName());
			continue;
		}
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

	for (auto it = methodSensorList.begin(); it != methodSensorList.end(); it++) {
		logger.debug("Call notifyStartup() on sensor %lli", (*it)->getSensorTypeId());
		(*it)->notifyStartup();
	}

	initializationFinished = true;

	logger.info("Initialized successfully");

	return S_OK;
}

HRESULT Agent::Shutdown()
{
	logger.debug("Shutdown...");
	shutdownCounter++;

	for (auto it = methodSensorList.begin(); it != methodSensorList.end(); it++) {
		(*it)->notifyShutdown();
	}

	shutdownDataSendingService();
	cmrConnection->unregisterPlatform(getAllDefinedIPs(), L".NET agent");
	alive = false;
	keepAliveThread.join();
	shutdownCMRConnection();
	logger.debug("Shutdown finished.");
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

BOOL Agent::getMethodSpecifics(FunctionID functionID, LPWSTR wszClass, LPWSTR wszMethod, LPWSTR returnType,
	JAVA_INT *javaModifiers, std::vector<LPWSTR> *parameterTypes) {
	return getSpecificsOfMethod(profilerInfo3, functionID, wszClass, wszMethod, returnType, javaModifiers, parameterTypes);
}