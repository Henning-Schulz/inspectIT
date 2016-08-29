#include "NativeStackProvider.h"
#include "agentutils.h"
#include "CMRConnection.h"
#include "Agent.h"

#include <Windows.h>

std::shared_ptr<NativeStackProvider> providerInstance;

NativeStackProvider::NativeStackProvider(ICorProfilerInfo2 *profilerInfo)
{
	this->profilerInfo = profilerInfo;

	char *charsMaxTraces;
	size_t len;
	errno_t err = _dupenv_s(&charsMaxTraces, &len, "MAX_TRACE_SIZE");
	if (err || len == 0) {
		logger.warn("No maximum trace size specified! Using default value of 100.");
		this->maximumTraceSize = 100;
	}
	else {
		std::string strMaxTraces(charsMaxTraces);
		std::stringstream stream(strMaxTraces);
		stream >> this->maximumTraceSize;
		logger.info("Maximum trace size is %i as specified.", this->maximumTraceSize);
	}
}


NativeStackProvider::~NativeStackProvider()
{
	logger.debug("Destructor");
}

void NativeStackProvider::init()
{
	providerInstance = shared_from_this();
}

void NativeStackProvider::setHookStrategy(std::shared_ptr<HookStrategy> hookStrategy)
{
	this->hookStrategy = hookStrategy;
}

bool NativeStackProvider::hasHook()
{
	return false;
}

std::shared_ptr<MethodHook> NativeStackProvider::getHook()
{
	// empty shared_ptr
	return std::shared_ptr<MethodHook>();
}

DWORD NativeStackProvider::getSpecialMonitorFlags()
{
	return COR_PRF_ENABLE_STACK_SNAPSHOT;
}

HRESULT __stdcall stackSnapshotCallback(
	FunctionID functionId,
	UINT_PTR ip,
	COR_PRF_FRAME_INFO frameInfo,
	ULONG32 contextSize,
	BYTE context[],
	void *clientData)
{
	providerInstance->nextStackFrame(functionId, static_cast<std::vector<FunctionID>*>(clientData));
	return S_OK;
}

void NativeStackProvider::nextStackFrame(FunctionID functionId, std::vector<FunctionID> *trace)
{
	// Skip unmanaged frames
	if (functionId != 0) {
		// Prevent allocating new memory, since this could cause a deadlock
		// if the walked (and hence suspended) thread owns a lock on the memory
		if (trace->size() < trace->capacity()) {
			trace->push_back(functionId);
		}
		else {
			logger.warn("Cannot store functionId %i. Vector is full.", functionId);
		}
	}
}

const char* getErrorName(HRESULT hr) {
	switch (hr) {
	case CORPROF_E_STACKSNAPSHOT_UNMANAGED_CTX:
		return "CORPROF_E_STACKSNAPSHOT_UNMANAGED_CTX";
		break;
	case CORPROF_E_STACKSNAPSHOT_UNSAFE:
		return "CORPROF_E_STACKSNAPSHOT_UNSAFE";
		break;
	case CORPROF_E_ASYNCHRONOUS_UNSAFE:
		return "CORPROF_E_ASYNCHRONOUS_UNSAFE";
		break;
	case CORPROF_E_NOT_MANAGED_THREAD:
		return "CORPROF_E_NOT_MANAGED_THREAD";
	case CORPROF_E_STACKSNAPSHOT_ABORTED:
		return "CORPROF_E_STACKSNAPSHOT_ABORTED";
		break;
	case CORPROF_E_STACKSNAPSHOT_INVALID_TGT_THREAD:
		return "CORPROF_E_STACKSNAPSHOT_INVALID_TGT_THREAD";
		break;
	case E_FAIL:
		return "E_FAIL";
		break;
	default:
		return "?";
		break;
	}
}

std::shared_ptr<StackTraceSample> NativeStackProvider::getStackTrace(ThreadID threadId)
{
	logger.debug("Sampling thread %i", threadId);

	ThreadID currentThreadId;
	profilerInfo->GetCurrentThreadID(&currentThreadId);
	//bool suspend = threadId != currentThreadId;
	//HANDLE threadHandle = 0;

	logger.debug("Get function mapper lock of thread %i...", threadId);
	auto functionMapperLock = globalAgentInstance->getFunctionMapperLock(threadId);
	logger.debug("Function mapper lock got.");

	/*if (suspend) {
		logger.debug("Suspend thread %i...", threadId);
		memset(&threadHandle, 0, sizeof(HANDLE));
		profilerInfo->GetHandleFromThread(threadId, &threadHandle);
		DWORD result = SuspendThread(threadHandle);
		logger.debug("Suspended. Result is %lu.", result);
	}
	else {
		logger.debug("Sampling current thread!");
	}*/
	
	/*CONTEXT threadContext;
	memset(&threadContext, 0, sizeof(CONTEXT));
	threadContext.ContextFlags = CONTEXT_CONTROL;
	GetThreadContext(threadHandle, &threadContext);
	FunctionID functionId;
	profilerInfo->GetFunctionFromIP(threadContext.Eip, &functionId);*/

	std::vector<FunctionID> nativeTrace;
	nativeTrace.reserve(maximumTraceSize);
	logger.debug("doStackSnapshot...");
	JAVA_LONG timestamp = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	HRESULT hr = profilerInfo->DoStackSnapshot(threadId, stackSnapshotCallback, COR_PRF_SNAPSHOT_DEFAULT, &nativeTrace, 0, 0);
	logger.debug("doStackSnapshot returned.");
	functionMapperLock->unlock();
	logger.debug("Function mapper unlocked.");

	/*if (suspend) {
		logger.debug("Resume thread %i", threadId);
		DWORD result = ResumeThread(threadHandle);
		logger.debug("Resumed. Result is %lu.", result);
	}*/

	std::vector<JAVA_LONG> trace;
	for (auto it = nativeTrace.begin(); it != nativeTrace.end(); it++) {
		METHOD_ID methodId = getMethodId(*it);

		if (methodId > 0) {
			trace.push_back(methodId);
		}
	}

	std::shared_ptr<StackTraceSample> sample;

	if (SUCCEEDED(hr)) {
		sample = std::make_shared<StackTraceSample>(trace, 0, timestamp); // offset is always 0
		logger.debug("Sampling was successful. Size of trace is %i.", trace.size());
	}
	else {
		if (loggerFactory.getLogLevel() >= LEVEL_DEBUG) {
			addSamplingError();
			logger.warn("Error when trying to do stack snapshot! Error is %s (0x%04x).", getErrorName(hr), hr);
			printNumSamplingErrors();
		}
	}

	return sample;
}

void NativeStackProvider::addSamplingError()
{
	std::unique_lock<std::shared_mutex> lock(mNumSamplingErrors);
	numSamplingErrors++;
}

void NativeStackProvider::printNumSamplingErrors()
{
	std::shared_lock<std::shared_mutex> lock(mNumSamplingErrors);
	logger.info("Number of sampling errors is %i.", numSamplingErrors);
}

METHOD_ID NativeStackProvider::getMethodId(FunctionID functionId)
{
	auto pmid = functionToMethodId.find(functionId);

	if (pmid == functionToMethodId.end()) {
		METHOD_ID mid;

		WCHAR className[MAX_BUFFERSIZE];
		WCHAR methodName[MAX_BUFFERSIZE];
		WCHAR returnType[MAX_BUFFERSIZE];
		JAVA_INT javaModifiers;
		std::vector<LPWSTR> parameterTypes; // elements will be created on the heap --> need to delete them afterwards
		getSpecificsOfMethod(profilerInfo, functionId, className, methodName, returnType, &javaModifiers, &parameterTypes);

		if (hookStrategy->hook(className, methodName, parameterTypes, javaModifiers)) {
			JAVA_LONG methodId = cmrConnection->registerMethod(globalAgentInstance->platformID, className, methodName, returnType, parameterTypes, javaModifiers);

			if (methodId > UINT_MAX) {
				loggerFactory.staticLog(LEVEL_ERROR, "NativeStackProvider", "methodId %lld for method %ls.%ls is greater than maximum value of unsigned int (%u)! Ignoring this method.", methodId, className, methodName, UINT_MAX);
				mid = 0;
			}
			else if (methodId < 0) {
				loggerFactory.staticLog(LEVEL_ERROR, "NativeStackProvider", "methodId %lld for method %ls.%ls is less than zero! Ignoring this method.", methodId, className, methodName);
				mid = 0;
			}
			else {
				mid = methodId;
			}
		}
		else {
			mid = 0;
		}

		functionToMethodId.insert(std::make_pair(functionId, mid));
		return mid;
	}
	else {
		return pmid->second;
	}
}