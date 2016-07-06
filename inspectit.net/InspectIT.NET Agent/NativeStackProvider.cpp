#include "NativeStackProvider.h"
#include "agentutils.h"
#include "CMRConnection.h"
#include "Agent.h"

#include <Windows.h>

std::shared_ptr<NativeStackProvider> providerInstance;

NativeStackProvider::NativeStackProvider(ICorProfilerInfo2 *profilerInfo)
{
	this->profilerInfo = profilerInfo;
	providerInstance = std::shared_ptr<NativeStackProvider>(this);
}


NativeStackProvider::~NativeStackProvider()
{
	logger.debug("Deconstructor");
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

bool NativeStackProvider::hasThreadHook()
{
	return true;
}

std::shared_ptr<ThreadHook> NativeStackProvider::getThreadHook()
{
	return std::shared_ptr<ThreadHook>(this);
}

void NativeStackProvider::threadCreated(ThreadID threadId, bool delayed)
{
	std::unique_lock<std::shared_mutex> lock(mCreatedThreads);
	createdThreads.push_back(threadId);
	logger.debug("Thread %i created%s", threadId, (delayed ? " some time ago." : "."));
}

void NativeStackProvider::threadDestroyed(ThreadID threadId, bool delayed)
{
	std::unique_lock<std::shared_mutex> lock(mCreatedThreads);

	for (auto it = createdThreads.begin(); it != createdThreads.end(); ) {
		if (*it == threadId) {
			it = createdThreads.erase(it);
			break;
		}
		else {
			it++;
		}
	}

	// TODO: wait for termination of sampling of the destroyed thread
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
	providerInstance->nextStackFrame(functionId, static_cast<std::vector<JAVA_LONG>*>(clientData));
	return S_OK;
}

void NativeStackProvider::nextStackFrame(FunctionID functionId, std::vector<JAVA_LONG> *trace)
{
	if (functionId == 0) {
		logger.debug("Skipping unmanaged stack frame");
	} else {
		logger.debug("FunctionID of frame is %i", functionId);
		METHOD_ID methodId = getMethodId(functionId);

		if (methodId > 0) {
			trace->push_back(methodId);
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
	bool suspend = threadId != currentThreadId;
	HANDLE threadHandle = 0;

	if (suspend) {
		logger.debug("Suspend thread %i...", threadId);
		memset(&threadHandle, 0, sizeof(HANDLE));
		profilerInfo->GetHandleFromThread(threadId, &threadHandle);
		SuspendThread(threadHandle);
		logger.debug("Suspended.");
	}
	else {
		logger.debug("Sampling current thread!");
	}
	
	/*CONTEXT threadContext;
	memset(&threadContext, 0, sizeof(CONTEXT));
	threadContext.ContextFlags = CONTEXT_CONTROL;
	GetThreadContext(threadHandle, &threadContext);
	FunctionID functionId;
	profilerInfo->GetFunctionFromIP(threadContext.Eip, &functionId);*/

	std::vector<JAVA_LONG> trace;
	HRESULT hr = profilerInfo->DoStackSnapshot(threadId, stackSnapshotCallback, COR_PRF_SNAPSHOT_DEFAULT, &trace, 0, 0);
	std::shared_ptr<StackTraceSample> sample;

	if (SUCCEEDED(hr)) {
		sample = std::make_shared<StackTraceSample>(trace, 0); // offset is always 0
	}
	else {
		addSamplingError();
		logger.error("Error when trying to do stack snapshot! Error is %s (0x%04x).", getErrorName(hr), hr);
		printNumSamplingErrors();
	}

	if (suspend) {
		logger.debug("Resume thread %i", threadId);
		ResumeThread(threadHandle);
		logger.debug("Resumed.");
	}

	logger.debug("Exit getStackTrace()");

	return sample;
}

std::map<ThreadID, std::shared_ptr<StackTraceSample>> NativeStackProvider::getAllStackTraces()
{
	logger.info("Retrieving traces for all stacks...");
	std::shared_lock<std::shared_mutex> lock(mCreatedThreads);

	std::map<ThreadID, std::shared_ptr<StackTraceSample>> samples;

	for (auto it = createdThreads.begin(); it != createdThreads.end(); it++) {
		std::shared_ptr<StackTraceSample> trace = getStackTrace(*it);
		if (trace) {
			for (int i = 0; i < trace->getTrace().size(); i++) {
				logger.debug("trace[%i] = %lli", i, trace->getTrace().at(i));
			}
			samples.insert(std::make_pair(*it, trace));
			logger.debug("Trace for thread %i added.", *it);
		}
		else {
			logger.warn("Trace not added, since it was NULL");
		}
	}

	return samples;
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

		if (globalAgentInstance->getHookStrategy()->hook(className, methodName, parameterTypes, javaModifiers)) {
			if (loggerFactory.getLogLevel() >= LEVEL_DEBUG) {
				loggerFactory.staticLogWithoutNewLine(LEVEL_DEBUG, "NativeStackProvider", "Method is 0x%03x %ls %ls.%ls(", javaModifiers, returnType, className, methodName);
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
				loggerFactory.staticLog(LEVEL_ERROR, "NativeStackProvider", "methodId %lld is greater than maximum value of unsigned int (%u)! Ignoring this method.", methodId, UINT_MAX);
				mid = 0;
			}
			else if (methodId < 0) {
				loggerFactory.staticLog(LEVEL_ERROR, "NativeStackProvider", "methodId %lld is less than zero! Ignoring this method.", methodId);
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
	}
	else {
		return pmid->second;
	}
}