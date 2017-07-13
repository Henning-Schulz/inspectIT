#pragma once

#include "basehdr.h"

namespace inspectit {

	/*
	 * This class represents a default implementation of the ICorProfilerCallback3 interface.
	 * Actaully, it does nothing. However, it allows to only implement the callback
	 * functions that are needed and to avoid waste code in the actual implementation.
	 */
	class CorProfilerCallbackDefaultImpl : public ICorProfilerCallback3
	{
	public:
		CorProfilerCallbackDefaultImpl();
		~CorProfilerCallbackDefaultImpl();


		//
		// STARTUP/SHUTDOWN EVENTS
		//
		virtual COM_METHOD(HRESULT) Initialize(IUnknown *pICorProfilerInfoUnk);


		//
		// APPLICATION DOMAIN EVENTS
		//
		COM_METHOD(HRESULT) AppDomainCreationStarted(AppDomainID appDomainID);

		COM_METHOD(HRESULT) AppDomainCreationFinished(AppDomainID appDomainID,
			HRESULT hrStatus);

		COM_METHOD(HRESULT) AppDomainShutdownStarted(AppDomainID appDomainID);

		COM_METHOD(HRESULT) AppDomainShutdownFinished(AppDomainID appDomainID,
			HRESULT hrStatus);


		//
		// ASSEMBLY EVENTS
		//
		COM_METHOD(HRESULT) AssemblyLoadStarted(AssemblyID assemblyID);

		COM_METHOD(HRESULT) AssemblyLoadFinished(AssemblyID assemblyID,
			HRESULT hrStatus);

		COM_METHOD(HRESULT) AssemblyUnloadStarted(AssemblyID assemblyID);

		COM_METHOD(HRESULT) AssemblyUnloadFinished(AssemblyID assemblyID,
			HRESULT hrStatus);


		//
		// MODULE EVENTS
		//
		COM_METHOD(HRESULT) ModuleLoadStarted(ModuleID moduleID);

		COM_METHOD(HRESULT) ModuleLoadFinished(ModuleID moduleID,
			HRESULT hrStatus);

		COM_METHOD(HRESULT) ModuleUnloadStarted(ModuleID moduleID);

		COM_METHOD(HRESULT) ModuleUnloadFinished(ModuleID moduleID,
			HRESULT hrStatus);

		COM_METHOD(HRESULT) ModuleAttachedToAssembly(ModuleID moduleID,
			AssemblyID assemblyID);


		//
		// CLASS EVENTS
		//
		COM_METHOD(HRESULT) ClassLoadStarted(ClassID classID);

		COM_METHOD(HRESULT) ClassLoadFinished(ClassID classID,
			HRESULT hrStatus);

		COM_METHOD(HRESULT) ClassUnloadStarted(ClassID classID);

		COM_METHOD(HRESULT) ClassUnloadFinished(ClassID classID,
			HRESULT hrStatus);

		COM_METHOD(HRESULT) FunctionUnloadStarted(FunctionID functionID);


		//
		// JIT EVENTS
		//              
		COM_METHOD(HRESULT) JITCompilationStarted(FunctionID functionID,
			BOOL fIsSafeToBlock);

		COM_METHOD(HRESULT) JITCompilationFinished(FunctionID functionID,
			HRESULT hrStatus,
			BOOL fIsSafeToBlock);

		COM_METHOD(HRESULT) JITCachedFunctionSearchStarted(FunctionID functionID,
			BOOL *pbUseCachedFunction);

		COM_METHOD(HRESULT) JITCachedFunctionSearchFinished(FunctionID functionID,
			COR_PRF_JIT_CACHE result);

		COM_METHOD(HRESULT) JITFunctionPitched(FunctionID functionID);

		COM_METHOD(HRESULT) JITInlining(FunctionID callerID,
			FunctionID calleeID,
			BOOL *pfShouldInline);


		//
		// THREAD EVENTS
		//
		COM_METHOD(HRESULT) ThreadCreated(ThreadID threadID);

		COM_METHOD(HRESULT) ThreadDestroyed(ThreadID threadID);

		COM_METHOD(HRESULT) ThreadAssignedToOSThread(ThreadID managedThreadID,
			DWORD osThreadID);
		//
		// REMOTING EVENTS
		//                                                      

		//
		// Client-side events
		//
		COM_METHOD(HRESULT) RemotingClientInvocationStarted();

		COM_METHOD(HRESULT) RemotingClientSendingMessage(GUID *pCookie,
			BOOL fIsAsync);

		COM_METHOD(HRESULT) RemotingClientReceivingReply(GUID *pCookie,
			BOOL fIsAsync);

		COM_METHOD(HRESULT) RemotingClientInvocationFinished();

		//
		// Server-side events
		//
		COM_METHOD(HRESULT) RemotingServerReceivingMessage(GUID *pCookie,
			BOOL fIsAsync);

		COM_METHOD(HRESULT) RemotingServerInvocationStarted();

		COM_METHOD(HRESULT) RemotingServerInvocationReturned();

		COM_METHOD(HRESULT) RemotingServerSendingReply(GUID *pCookie,
			BOOL fIsAsync);


		//
		// CONTEXT EVENTS
		//                                                      
		COM_METHOD(HRESULT) UnmanagedToManagedTransition(FunctionID functionID,
			COR_PRF_TRANSITION_REASON reason);

		COM_METHOD(HRESULT) ManagedToUnmanagedTransition(FunctionID functionID,
			COR_PRF_TRANSITION_REASON reason);


		//
		// SUSPENSION EVENTS
		//    
		COM_METHOD(HRESULT) RuntimeSuspendStarted(COR_PRF_SUSPEND_REASON suspendReason);

		COM_METHOD(HRESULT) RuntimeSuspendFinished();

		COM_METHOD(HRESULT) RuntimeSuspendAborted();

		COM_METHOD(HRESULT) RuntimeResumeStarted();

		COM_METHOD(HRESULT) RuntimeResumeFinished();

		COM_METHOD(HRESULT) RuntimeThreadSuspended(ThreadID threadid);

		COM_METHOD(HRESULT) RuntimeThreadResumed(ThreadID threadid);


		//
		// GC EVENTS
		//    
		COM_METHOD(HRESULT) MovedReferences(ULONG cmovedObjectIDRanges,
			ObjectID oldObjectIDRangeStart[],
			ObjectID newObjectIDRangeStart[],
			ULONG cObjectIDRangeLength[]);

		COM_METHOD(HRESULT) SurvivingReferences(ULONG cmovedObjectIDRanges,
			ObjectID objectIDRangeStart[],
			ULONG cObjectIDRangeLength[]);

		COM_METHOD(HRESULT) ObjectAllocated(ObjectID objectID,
			ClassID classID);

		COM_METHOD(HRESULT) ObjectsAllocatedByClass(ULONG classCount,
			ClassID classIDs[],
			ULONG objects[]);

		COM_METHOD(HRESULT) ObjectReferences(ObjectID objectID,
			ClassID classID,
			ULONG cObjectRefs,
			ObjectID objectRefIDs[]);

		COM_METHOD(HRESULT) RootReferences(ULONG cRootRefs,
			ObjectID rootRefIDs[]);


		//
		// EXCEPTION EVENTS
		//                                                         

		// Exception creation
		COM_METHOD(HRESULT) ExceptionThrown(ObjectID thrownObjectID);

		// Search phase
		COM_METHOD(HRESULT) ExceptionSearchFunctionEnter(FunctionID functionID);

		COM_METHOD(HRESULT) ExceptionSearchFunctionLeave();

		COM_METHOD(HRESULT) ExceptionSearchFilterEnter(FunctionID functionID);

		COM_METHOD(HRESULT) ExceptionSearchFilterLeave();

		COM_METHOD(HRESULT) ExceptionSearchCatcherFound(FunctionID functionID);

		COM_METHOD(HRESULT) ExceptionCLRCatcherFound();

		COM_METHOD(HRESULT) ExceptionCLRCatcherExecute();

		COM_METHOD(HRESULT) ExceptionOSHandlerEnter(FunctionID functionID);

		COM_METHOD(HRESULT) ExceptionOSHandlerLeave(FunctionID functionID);

		// Unwind phase
		COM_METHOD(HRESULT) ExceptionUnwindFunctionEnter(FunctionID functionID);

		COM_METHOD(HRESULT) ExceptionUnwindFunctionLeave();

		COM_METHOD(HRESULT) ExceptionUnwindFinallyEnter(FunctionID functionID);

		COM_METHOD(HRESULT) ExceptionUnwindFinallyLeave();

		COM_METHOD(HRESULT) ExceptionCatcherEnter(FunctionID functionID,
			ObjectID objectID);

		COM_METHOD(HRESULT) ExceptionCatcherLeave();


		//
		// COM CLASSIC WRAPPER
		//
		COM_METHOD(HRESULT)  COMClassicVTableCreated(ClassID wrappedClassID,
			REFGUID implementedIID,
			void *pVTable,
			ULONG cSlots);

		COM_METHOD(HRESULT)  COMClassicVTableDestroyed(ClassID wrappedClassID,
			REFGUID implementedIID,
			void *pVTable);

		COM_METHOD(HRESULT) STDMETHODCALLTYPE ThreadNameChanged(
			/* [in] */ ThreadID threadId,
			/* [in] */ ULONG cchName,
			/* [in] */ __in_ecount_opt(cchName) WCHAR name[]);

		/*
		* The CLR calls GarbageCollectionStarted before beginning a
		* garbage collection. All GC callbacks pertaining to this
		* collection will occur between the GarbageCollectionStarted
		* callback and the corresponding GarbageCollectionFinished
		* callback, which will occur on the same thread.
		*
		*          cGenerations indicates the total number of entries in
		*                the generationCollected array
		*          generationCollected is an array of booleans, indexed
		*                by COR_PRF_GC_GENERATIONS, indicating which
		*                generations are being collected in this collection
		*          wasInduced indicates whether this GC was induced
		*                by the application calling GC.Collect().
		*/
		COM_METHOD(HRESULT)  GarbageCollectionStarted(
			/* [in] */int cGenerations,
			/*[in, size_is(cGenerations), length_is(cGenerations)]*/ BOOL generationCollected[],
			/*[in]*/ COR_PRF_GC_REASON reason);

		/*
		* The CLR calls GarbageCollectionFinished after a garbage
		* collection has completed and all GC callbacks have been
		* issued for it.
		*/
		COM_METHOD(HRESULT)  GarbageCollectionFinished();

		/*
		* The CLR calls FinalizeableObjectQueued to notify the code profiler
		* that an object with a finalizer (destructor in C# parlance) has
		* just been queued to the finalizer thread for execution of its
		* Finalize method.
		*
		* finalizerFlags describes aspects of the finalizer, and takes its
		*     value from COR_PRF_FINALIZER_FLAGS.
		*
		*/

		COM_METHOD(HRESULT) STDMETHODCALLTYPE FinalizeableObjectQueued(
			/* [in] */DWORD finalizerFlags,
			/* [in] */ObjectID objectID);

		COM_METHOD(HRESULT) STDMETHODCALLTYPE RootReferences2(
			/* [in] */ ULONG cRootRefs,
			/* [size_is][in] */ ObjectID rootRefIds[],
			/* [size_is][in] */ COR_PRF_GC_ROOT_KIND rootKinds[],
			/* [size_is][in] */ COR_PRF_GC_ROOT_FLAGS rootFlags[],
			/* [size_is][in] */ UINT_PTR rootIds[]);

		/*
		* The CLR calls HandleCreated when a gc handle has been created.
		*
		*/
		COM_METHOD(HRESULT) STDMETHODCALLTYPE HandleCreated(
			/* [in] */ UINT_PTR handleId,
			/* [in] */ ObjectID initialObjectId);

		/*
		* The CLR calls HandleDestroyed when a gc handle has been destroyed.
		*
		*/
		COM_METHOD(HRESULT) STDMETHODCALLTYPE HandleDestroyed(
			/* [in] */ UINT_PTR handleId);

		//
		// ATTACH EVENTS
		//
		COM_METHOD(HRESULT)  STDMETHODCALLTYPE InitializeForAttach(
			/* [in] */ IUnknown *pICorProfilerInfoUnk,
			/* [in] */ void *pvClientData,
			/* [in] */ UINT cbClientData);

		COM_METHOD(HRESULT)  ProfilerAttachComplete();

		COM_METHOD(HRESULT)  ProfilerDetachSucceeded();
	};

}