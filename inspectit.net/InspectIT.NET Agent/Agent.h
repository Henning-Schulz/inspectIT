#pragma once

#include "CorProfilerCallbackDefaultImpl.h"
#include "MethodHook.h"
#include "MethodSensor.h"
#include "ThreadHook.h"
#include "Logger.h"
#include "mscoree.h"
#include "agentutils.h"
#include "HookStrategy.h"
#include "CMRConnection.h"
#include "DataSendingService.h"

#include <list>
#include <memory>
#include <thread>
#include <mutex>

#define DllImport  __declspec( dllimport )

// {DE04C0CC-3985-46C6-9446-BC329D5F159F}

extern const GUID __declspec(selectany) CLSID_PROFILER =
{ 0xde04c0cc, 0x3985, 0x46c6,{ 0x94, 0x46, 0xbc, 0x32, 0x9d, 0x5f, 0x15, 0x9f } };

#define THREADING_MODEL "Both"
#define PROGID_PREFIX "Objects Profiler"
#define COCLASS_DESCRIPTION "inspectIT NET Agent"
#define PROFILER_GUID "{DE04C0CC-3985-46C6-9446-BC329D5F159F}"
#define PROFILER_GUID_WCHAR L"{DE04C0CC-3985-46C6-9446-BC329D5F159F}"

#define COM_METHOD( TYPE ) TYPE STDMETHODCALLTYPE

class Agent : public CorProfilerCallbackDefaultImpl
{
private:
	long m_refCount;
	DWORD shutdownCounter;
	ICorProfilerInfo3 *profilerInfo3;

	std::map<ClassID, std::shared_ptr<InstrumentationDefinition>> instrumentationDefinitions;

	std::list<std::shared_ptr<MethodSensor>> methodSensorList;
	std::list<std::pair<std::shared_ptr<MethodHook>, std::shared_ptr<HookStrategy>>> methodHookList;
	std::list<std::shared_ptr<ThreadHook>> threadHookList;

	std::map<METHOD_ID, std::vector<std::shared_ptr<MethodHook>>> hookAssignment;
	std::shared_mutex mHookAssignment;

	std::thread keepAliveThread;
	boolean alive = false;

	Logger logger = loggerFactory.createLogger("Agent");

	void keepAlive();

public:
	Agent();
	~Agent();

	JAVA_LONG platformID;

	void addMethodHook(std::shared_ptr<MethodHook> hook, std::shared_ptr<HookStrategy> hookStrategy);
	void removeMethodHook(std::shared_ptr<MethodHook> hook);
	void removeAllMethodHooks();
	size_t getNumberOfMethodHooks();
	std::list<std::pair<std::shared_ptr<MethodHook>, std::shared_ptr<HookStrategy>>> getMethodHooksWithStrategies();

	void assignHookToMethod(METHOD_ID methodId, std::shared_ptr<MethodHook> hook);

	ClassID getClassIdForFunction(FunctionID functionId);
	std::shared_ptr<InstrumentationDefinition> getInstrumentationDefinition(ClassID classId);

	//
	// IUnknown 
	//
	COM_METHOD(ULONG) AddRef();
	COM_METHOD(ULONG) Release();
	COM_METHOD(HRESULT) QueryInterface(REFIID riid, void **ppInterface);
	static COM_METHOD(HRESULT) CreateObject(REFIID riid, void **ppInterface);

	//
	// Startup/shutdown
	//
	virtual COM_METHOD(HRESULT) Initialize(IUnknown *pICorProfilerInfoUnk);

	HRESULT DllDetachShutdown();
	COM_METHOD(HRESULT) Shutdown();

	//
	// Class events
	//
	COM_METHOD(HRESULT) ClassLoadFinished(ClassID classID,
		HRESULT hrStatus);

	//
	// Method callbacks
	//
	void Enter(METHOD_ID functionID);
	void Leave(METHOD_ID functionID);
	void Tailcall(METHOD_ID functionID);

	//
	// Thread callbacks
	//
	COM_METHOD(HRESULT) ThreadCreated(ThreadID threadID);
	COM_METHOD(HRESULT) ThreadDestroyed(ThreadID threadID);

};

extern Agent *globalAgentInstance;

