
HINSTANCE g_hInst;        // instance handle to this piece of code
const int g_iVersion = 1; // version of coclasses.

static const char *g_szProgIDPrefix   = PROGID_PREFIX;
static const char *g_szThreadingModel = THREADING_MODEL;
static const char *g_szCoclassDesc    = COCLASS_DESCRIPTION;


// create a new instance of an object.
typedef HRESULT (__stdcall * PFN_CREATE_OBJ)( REFIID riid, void **ppInterface );



struct COCLASS_REGISTER
{   
    const GUID *pClsid;             // Class ID of the coclass
    const char *szProgID;           // Prog ID of the class
    PFN_CREATE_OBJ pfnCreateObject; // function to create instance
    
}; // COCLASS_REGISTER


//
// this map contains the list of coclasses which are exported from this module
//
const COCLASS_REGISTER g_CoClasses[] =
{   
    &CLSID_PROFILER,
    PROFILER_GUID,          
	inspectit::Agent::CreateObject,
    NULL,               
    NULL,               
    NULL
};


class CClassFactory :
    public IClassFactory
{
    private:
    
        CClassFactory();                        
        
    
    public:
    
        CClassFactory( const COCLASS_REGISTER *pCoClass );
        ~CClassFactory();
        

    public:
    
        //
        // IUnknown 
        //
        COM_METHOD( ULONG ) AddRef();       
        COM_METHOD( ULONG ) Release();
        COM_METHOD( HRESULT ) QueryInterface( REFIID riid, void **ppInterface );            
        
        //
        // IClassFactory 
        //
        COM_METHOD( HRESULT ) LockServer( BOOL fLock );
        COM_METHOD( HRESULT ) CreateInstance( IUnknown *pUnkOuter,
                                              REFIID riid,
                                              void **ppInterface );
    
    
    private:
    
        long m_refCount;                        
        const COCLASS_REGISTER *m_pCoClass;     
        
}; // CClassFactory



CClassFactory::CClassFactory()
{    
} // ctor



CClassFactory::CClassFactory( const COCLASS_REGISTER *pCoClass ) :
    m_refCount( 1 ), 
    m_pCoClass( pCoClass )
{    
	

} // ctor



CClassFactory::~CClassFactory()
{
} // dtor



ULONG CClassFactory::AddRef()
{
	//printf(" \n 3. Clas fact Add ref called");
    return InterlockedIncrement( &m_refCount );
    
} // CClassFactory::AddRef 
        
        
ULONG CClassFactory::Release()
{    
    long refCount;


    refCount = InterlockedDecrement( &m_refCount );
    if ( refCount == 0 ) 
        delete this;

    
    return refCount;
            
} // CClassFactory::Release



HRESULT CClassFactory::QueryInterface(REFIID riid, void **ppInterface)
{
	//printf(" 1. IID i recieved is %x" , riid);

	//printf(" 2 . Class Factory IID i recieved is %x", IID_IClassFactory);
	if (riid == IID_IUnknown)
	{
		
		*ppInterface = static_cast<IUnknown *>(this);
	}
	else if (riid == IID_IClassFactory)
	{
		
		*ppInterface = static_cast<IClassFactory *>(this);

	}
    else
    {
        *ppInterface = NULL;                  
		
        
        return E_NOINTERFACE;
    }
    
    reinterpret_cast<IUnknown *>( *ppInterface )->AddRef();
	
    
    return S_OK;

} // CClassFactory::QueryInterface


 
HRESULT CClassFactory::CreateInstance( IUnknown *pUnkOuter, REFIID riid, void **ppInstance )
{
	//printf("the else  %x ", riid);
	
    // aggregation is not supported by these objects
    if ( pUnkOuter != NULL )
        return CLASS_E_NOAGGREGATION;
	
    // ask the object to create an instance of itself, and check the iid.
    return (*m_pCoClass->pfnCreateObject)( riid, ppInstance );
       
} // CClassFactory::CreateInstance


HRESULT CClassFactory::LockServer( BOOL fLock )
{    
    
    return S_OK;
    
} // CClassFactory::LockServer





