#pragma once

#include "CorProfilerCallbackDefaultImpl.h"
#include "MethodSensor.h"
#include "Logger.h"
#include "mscoree.h"
#include "agentutils.h"
#include "HookStrategy.h"
#include "CMRConnection.h"
#include "DataSendingService.h"

#include <list>
#include <memory>

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

	std::list<MethodSensor*> *methodSensorList;
	HookStrategy *hookStrategy;

	Logger logger = loggerFactory.createLogger("Agent");

public:
	Agent();
	~Agent();

	JAVA_LONG platformID;

	void addMethodSensor(MethodSensor *sensor);
	void removeMethodSensor(MethodSensor *sensor);
	void removeAllMethodSensors();

	HookStrategy* getHookStrategy();

	BOOL getMethodSpecifics(FunctionID functionID, LPWSTR wszClass, LPWSTR wszMethod, LPWSTR returnType, JAVA_INT *javaModifiers, std::vector<LPWSTR> *parameterTypes);

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
	// Callbacks
	//

	void Enter(METHOD_ID functionID);
	void Leave(METHOD_ID functionID);
	void Tailcall(METHOD_ID functionID);

};

extern Agent *globalAgentInstance;
