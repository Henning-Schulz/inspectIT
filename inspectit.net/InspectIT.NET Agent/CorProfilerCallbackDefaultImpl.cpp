#include "CorProfilerCallbackDefaultImpl.h"

using namespace inspectit;

CorProfilerCallbackDefaultImpl::CorProfilerCallbackDefaultImpl()
{
}

CorProfilerCallbackDefaultImpl::~CorProfilerCallbackDefaultImpl()
{
}

HRESULT CorProfilerCallbackDefaultImpl::Initialize(IUnknown *pICorProfilerInfoUnk)
{
	return S_OK;
}

HRESULT CorProfilerCallbackDefaultImpl::JITCachedFunctionSearchStarted(FunctionID functionID, BOOL *pbUseCachedFunction)
{
	return S_OK;
}

HRESULT CorProfilerCallbackDefaultImpl::JITCompilationFinished(FunctionID functionID, HRESULT hrStatus, BOOL fIsSafeToBlock)
{
	return S_OK;

}

HRESULT CorProfilerCallbackDefaultImpl::JITCachedFunctionSearchFinished(FunctionID functionID, COR_PRF_JIT_CACHE result)
{
	return S_OK;
}

HRESULT CorProfilerCallbackDefaultImpl::ExceptionUnwindFunctionEnter(FunctionID functionID)
{
	return S_OK;
}

HRESULT CorProfilerCallbackDefaultImpl::ExceptionUnwindFunctionLeave()
{
	return S_OK;
}

HRESULT CorProfilerCallbackDefaultImpl::ThreadCreated(ThreadID threadID)
{
	return S_OK;
}

HRESULT CorProfilerCallbackDefaultImpl::ThreadDestroyed(ThreadID threadID)
{
	return S_OK;
}

HRESULT CorProfilerCallbackDefaultImpl::ThreadAssignedToOSThread(ThreadID managedThreadID, DWORD osThreadID)
{
	return S_OK;
}

HRESULT CorProfilerCallbackDefaultImpl::UnmanagedToManagedTransition(FunctionID functionID, COR_PRF_TRANSITION_REASON reason)
{
	return S_OK;
}

HRESULT CorProfilerCallbackDefaultImpl::ManagedToUnmanagedTransition(FunctionID functionID, COR_PRF_TRANSITION_REASON reason)
{
	return S_OK;
}

HRESULT CorProfilerCallbackDefaultImpl::ObjectAllocated(ObjectID objectID, ClassID classID)
{
	return S_OK;
}

HRESULT CorProfilerCallbackDefaultImpl::ObjectReferences(ObjectID objectID, ClassID classID, ULONG objectRefs, ObjectID objectRefIDs[])
{
	return S_OK;
}

HRESULT CorProfilerCallbackDefaultImpl::RootReferences(ULONG rootRefs, ObjectID rootRefIDs[])
{
	return S_OK;
}

HRESULT CorProfilerCallbackDefaultImpl::RuntimeSuspendStarted(COR_PRF_SUSPEND_REASON suspendReason)
{
	return S_OK;
}

HRESULT CorProfilerCallbackDefaultImpl::RuntimeResumeFinished()
{
	return S_OK;
}

HRESULT CorProfilerCallbackDefaultImpl::AppDomainCreationStarted(AppDomainID appDomainID)
{
	return S_OK;
}

HRESULT CorProfilerCallbackDefaultImpl::AppDomainCreationFinished(AppDomainID appDomainID, HRESULT hrStatus)
{
	return S_OK;
}

HRESULT CorProfilerCallbackDefaultImpl::AppDomainShutdownStarted(AppDomainID appDomainID)
{
	return S_OK;
}

HRESULT CorProfilerCallbackDefaultImpl::AppDomainShutdownFinished(AppDomainID appDomainID, HRESULT hrStatus)
{
	return S_OK;
}

HRESULT CorProfilerCallbackDefaultImpl::AssemblyLoadStarted(AssemblyID assemblyId)
{
	return S_OK;
}

HRESULT CorProfilerCallbackDefaultImpl::AssemblyLoadFinished(AssemblyID assemblyId, HRESULT hrStatus)
{
	return S_OK;
}

HRESULT CorProfilerCallbackDefaultImpl::AssemblyUnloadStarted(AssemblyID assemblyID)
{
	return S_OK;
}

HRESULT CorProfilerCallbackDefaultImpl::AssemblyUnloadFinished(AssemblyID assemblyID, HRESULT hrStatus)
{
	return S_OK;
}

HRESULT CorProfilerCallbackDefaultImpl::ModuleLoadStarted(ModuleID moduleID)
{
	return S_OK;
}

HRESULT CorProfilerCallbackDefaultImpl::ModuleUnloadStarted(ModuleID moduleID)
{
	return S_OK;
}

HRESULT CorProfilerCallbackDefaultImpl::ModuleUnloadFinished(ModuleID moduleID, HRESULT hrStatus)
{
	return S_OK;
}

HRESULT CorProfilerCallbackDefaultImpl::ModuleAttachedToAssembly(ModuleID moduleID, AssemblyID assemblyID)
{
	return S_OK;
}

HRESULT CorProfilerCallbackDefaultImpl::ClassLoadStarted(ClassID classID)
{
	return S_OK;
}

HRESULT CorProfilerCallbackDefaultImpl::ClassLoadFinished(ClassID classID, HRESULT hrStatus)
{
	return S_OK;
}

HRESULT CorProfilerCallbackDefaultImpl::ClassUnloadStarted(ClassID classID) {
	return S_OK;
}

HRESULT CorProfilerCallbackDefaultImpl::ClassUnloadFinished(ClassID classID, HRESULT hrStatus)
{
	return S_OK;
}

HRESULT CorProfilerCallbackDefaultImpl::FunctionUnloadStarted(FunctionID functionID)
{
	return S_OK;
}

HRESULT CorProfilerCallbackDefaultImpl::JITFunctionPitched(FunctionID functionID)
{
	return S_OK;
}

HRESULT CorProfilerCallbackDefaultImpl::JITInlining(FunctionID callerID, FunctionID calleeID, BOOL *pfShouldInline)
{
	return S_OK;
}

HRESULT CorProfilerCallbackDefaultImpl::RemotingClientInvocationStarted()
{
	return S_OK;
}

HRESULT CorProfilerCallbackDefaultImpl::RemotingClientSendingMessage(GUID *pCookie, BOOL fIsAsync)
{
	return S_OK;
}

HRESULT CorProfilerCallbackDefaultImpl::RemotingClientReceivingReply(GUID *pCookie, BOOL fIsAsync)
{
	return S_OK;
}

HRESULT CorProfilerCallbackDefaultImpl::RemotingClientInvocationFinished()
{
	return S_OK;
}

HRESULT CorProfilerCallbackDefaultImpl::RemotingServerReceivingMessage(GUID *pCookie, BOOL fIsAsync)
{
	return S_OK;
}

HRESULT CorProfilerCallbackDefaultImpl::RemotingServerInvocationStarted()
{
	return S_OK;
}

HRESULT CorProfilerCallbackDefaultImpl::RemotingServerInvocationReturned()
{
	return S_OK;
}

HRESULT CorProfilerCallbackDefaultImpl::RemotingServerSendingReply(GUID *pCookie, BOOL fIsAsync)
{
	return S_OK;
}

HRESULT CorProfilerCallbackDefaultImpl::RuntimeSuspendFinished()
{
	return S_OK;
}

HRESULT CorProfilerCallbackDefaultImpl::RuntimeSuspendAborted()
{
	return S_OK;
}

HRESULT CorProfilerCallbackDefaultImpl::RuntimeResumeStarted()
{
	return S_OK;
}

HRESULT CorProfilerCallbackDefaultImpl::RuntimeThreadSuspended(ThreadID threadID)
{
	return S_OK;
}

HRESULT CorProfilerCallbackDefaultImpl::RuntimeThreadResumed(ThreadID threadID)
{
	return S_OK;
}

HRESULT CorProfilerCallbackDefaultImpl::MovedReferences(ULONG cmovedObjectIDRanges, ObjectID oldObjectIDRangeStart[], ObjectID newObjectIDRangeStart[], ULONG cObjectIDRangeLength[])
{
	return S_OK;
}

HRESULT CorProfilerCallbackDefaultImpl::SurvivingReferences(ULONG cmovedObjectIDRanges, ObjectID objectIDRangeStart[], ULONG cObjectIDRangeLength[])
{
	return S_OK;
}

HRESULT CorProfilerCallbackDefaultImpl::ObjectsAllocatedByClass(ULONG classCount, ClassID classIDs[], ULONG objects[])
{
	return S_OK;
}

HRESULT CorProfilerCallbackDefaultImpl::ExceptionSearchFunctionLeave()
{
	return S_OK;
}

HRESULT CorProfilerCallbackDefaultImpl::ExceptionSearchFilterEnter(FunctionID functionID)
{
	return S_OK;
}

HRESULT CorProfilerCallbackDefaultImpl::ExceptionSearchFilterLeave()
{
	return S_OK;
}

HRESULT CorProfilerCallbackDefaultImpl::ExceptionCLRCatcherFound()
{
	return S_OK;
}

HRESULT CorProfilerCallbackDefaultImpl::ExceptionCLRCatcherExecute()
{
	return S_OK;
}

HRESULT CorProfilerCallbackDefaultImpl::ExceptionOSHandlerEnter(FunctionID functionID)
{
	return S_OK;

}

HRESULT CorProfilerCallbackDefaultImpl::ExceptionOSHandlerLeave(FunctionID functionID)
{
	return S_OK;
}

HRESULT CorProfilerCallbackDefaultImpl::ExceptionUnwindFinallyEnter(FunctionID functionID)
{
	return S_OK;
}

HRESULT CorProfilerCallbackDefaultImpl::ExceptionUnwindFinallyLeave()
{
	return S_OK;
}

HRESULT CorProfilerCallbackDefaultImpl::ExceptionCatcherEnter(FunctionID functionID, ObjectID objectID)
{
	return S_OK;
}

HRESULT CorProfilerCallbackDefaultImpl::ExceptionCatcherLeave()
{
	return S_OK;
}

HRESULT CorProfilerCallbackDefaultImpl::COMClassicVTableCreated(ClassID wrappedClassID, REFGUID implementedIID, void *pVTable, ULONG cSlots)
{
	return S_OK;
}

HRESULT CorProfilerCallbackDefaultImpl::COMClassicVTableDestroyed(ClassID wrappedClassID, REFGUID implementedIID, void *pVTable)
{
	return S_OK;
}

HRESULT CorProfilerCallbackDefaultImpl::ThreadNameChanged(ThreadID threadId, ULONG cchName, __in_ecount_opt(cchName) WCHAR name[])
{
	return S_OK;
}

HRESULT CorProfilerCallbackDefaultImpl::FinalizeableObjectQueued(DWORD finalizerFlags, ObjectID objectID)
{
	return S_OK;
}

HRESULT CorProfilerCallbackDefaultImpl::RootReferences2(ULONG cRootRefs, ObjectID rootRefIds[], COR_PRF_GC_ROOT_KIND rootKinds[], COR_PRF_GC_ROOT_FLAGS rootFlags[], UINT_PTR rootIds[])
{
	return S_OK;
}

HRESULT CorProfilerCallbackDefaultImpl::HandleCreated(UINT_PTR handleId, ObjectID initialObjectId)
{
	return S_OK;
}

HRESULT CorProfilerCallbackDefaultImpl::HandleDestroyed(UINT_PTR handleId)
{
	return S_OK;
}

HRESULT CorProfilerCallbackDefaultImpl::GarbageCollectionStarted(int cGenerations, BOOL generationCollected[], COR_PRF_GC_REASON reason)
{
	return S_OK;
}

HRESULT  CorProfilerCallbackDefaultImpl::GarbageCollectionFinished()
{
	return S_OK;
}

HRESULT CorProfilerCallbackDefaultImpl::InitializeForAttach(IUnknown *pICorProfilerInfoUnk, void *pvClientData, UINT cbClientData)
{
	return S_OK;
}

HRESULT CorProfilerCallbackDefaultImpl::ProfilerAttachComplete()
{
	return S_OK;
}

HRESULT CorProfilerCallbackDefaultImpl::ProfilerDetachSucceeded()
{
	return S_OK;
}

HRESULT CorProfilerCallbackDefaultImpl::ModuleLoadFinished(ModuleID moduleID, HRESULT hrStatus)
{
	return S_OK;
}

HRESULT CorProfilerCallbackDefaultImpl::JITCompilationStarted(FunctionID functionID, BOOL fIsSafeToBlock)
{
	return S_OK;
}

HRESULT CorProfilerCallbackDefaultImpl::ExceptionThrown(ObjectID thrownObjectID)
{
	return S_OK;
}

HRESULT CorProfilerCallbackDefaultImpl::ExceptionSearchFunctionEnter(FunctionID functionID)
{
	return S_OK;
}

HRESULT CorProfilerCallbackDefaultImpl::ExceptionSearchCatcherFound(FunctionID functionID)
{
	return S_OK;
}