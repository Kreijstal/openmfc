#pragma once
// Shared internals of the former regcore.cpp translation unit.
// Definitions live in detail/RegcoreSupport.cpp, so file-scope state keeps a
// single instance across the classes this unit was split into.
namespace openmfc { namespace detail { namespace regcore {} } }
using namespace openmfc::detail::regcore;
// Registration Core Implementation
//
// Implements AfxRegisterClass, AfxRegisterWndClass and related window class registration.

#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxwin.h"
#include "openmfc/afxstat_.h"
#include "CThreadLocalObjectSupport.h"
#include "openmfc/afxtls_.h"
#include "TypeLibCacheSupport.h"
#include <windows.h>
#include <cstring>

// MS ABI calling convention
#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

// =============================================================================
// Global State
// =============================================================================

// Keep track of registered class atoms for cleanup
namespace openmfc { namespace detail { namespace regcore {
extern ATOM g_registeredClasses[256];
} } }
namespace openmfc { namespace detail { namespace regcore {
extern int g_numRegisteredClasses;
} } }

// Standard MFC window class names
static const wchar_t* AFX_WNDCLASS_MAIN = L"AfxFrameOrView140u";
static const wchar_t* AFX_WNDCLASS_MDI  = L"AfxMDIFrame140u";
static const wchar_t* AFX_WNDCLASS_CTRL = L"AfxControlBar140u";

// =============================================================================
// AfxRegisterClass
// =============================================================================


// =============================================================================
// AfxRegisterWndClass
// =============================================================================

namespace openmfc { namespace detail { namespace regcore {
constexpr size_t kWndClassNameSlots = 16;
} } }
namespace openmfc { namespace detail { namespace regcore {
constexpr size_t kWndClassNameLen = 128;
} } }

namespace openmfc { namespace detail { namespace regcore {
extern thread_local size_t g_wndClassNameIndex;
} } }

namespace openmfc { namespace detail { namespace regcore {
wchar_t* NextWndClassNameBuffer();
} } }


// =============================================================================
// AfxUnregisterClass (internal cleanup)
// =============================================================================

// Not exported, but useful for internal cleanup
namespace openmfc { namespace detail { namespace regcore {
void AfxUnregisterAllClasses();
} } }

// =============================================================================
// Additional Window Registration Helpers
// =============================================================================

// AfxRegisterClass (Ex version) - Extended register
namespace openmfc { namespace detail { namespace regcore {
extern "C" int MS_ABI impl__AfxRegisterClass__YAHPEAUtagWNDCLASSEXW___Z(WNDCLASSEXW* lpWndClass);
} } }

// =============================================================================
// AfxGetInstanceHandle (if not defined elsewhere)
// =============================================================================

// AfxGetInstanceHandle - Get the application instance handle
namespace openmfc { namespace detail { namespace regcore {
extern "C" HINSTANCE MS_ABI impl__AfxGetInstanceHandle__YAPEAUHINSTANCE____XZ();
} } }

// AfxGetResourceHandle - Get the resource module handle
namespace openmfc { namespace detail { namespace regcore {
extern "C" HINSTANCE MS_ABI impl__AfxGetResourceHandle__YAPEAUHINSTANCE____XZ();
} } }

// AfxSetResourceHandle - Set the resource module handle
namespace openmfc { namespace detail { namespace regcore {
extern "C" void MS_ABI impl__AfxSetResourceHandle__YAXPEAUHINSTANCE____Z(HINSTANCE hInstResource);
} } }

// =============================================================================
// AfxGetMainWnd
// =============================================================================

// AfxGetMainWnd - Get the main window
namespace openmfc { namespace detail { namespace regcore {
extern "C" CWnd* MS_ABI impl__AfxGetMainWnd__YAPEAVCWnd__XZ();
} } }

// AfxGetApp - Get the application object
namespace openmfc { namespace detail { namespace regcore {
extern "C" CWinApp* MS_ABI impl__AfxGetApp__YAPEAVCWinApp__XZ();
} } }

// =============================================================================
// AfxGetModuleState / AfxSetModuleState
// =============================================================================

// These are used for DLL state management in real MFC.
// The implementation here keeps a thread-local current state pointer and exposes
// AfxGetStaticModuleState as a stable process-wide state object.

LRESULT CALLBACK AfxWndProc(HWND,UINT,WPARAM,LPARAM);
class CDynLinkLibrary;
class COleObjectFactory;
class COleControlLock;
class COccManager;

class AFX_MODULE_STATE : public CNoTrackObject {
public:
    AFX_MODULE_STATE()
        : m_pCurrentWinApp(nullptr),m_hCurrentInstanceHandle(nullptr),
          m_hCurrentResourceHandle(nullptr),m_lpszCurrentAppName(nullptr),
          m_bDLL(FALSE),m_bSystem(FALSE),m_bReserved{},m_fRegisteredClasses(0),
          m_pClassInit(nullptr),m_classList(offsetof(CRuntimeClass,m_pNextClass)),
          m_pFactoryInit(nullptr),m_factoryList(0x40),m_nObjectCount(0),m_bUserCtrl(TRUE),
          m_pfnAfxWndProc(&AfxWndProc),m_dwVersion(0),m_pfnFilterToolTipMessage(nullptr),
          m_libraryList(0x78),m_appLangDLL(nullptr),m_pOccManager(nullptr),m_lockList(0x20),
          m_pTypeLibCacheMap(nullptr),m_moduleThreadState{},
          m_bInitNetworkAddressControl(FALSE),m_bInitNetworkAddressControlCalled(FALSE) {}
    AFX_MODULE_STATE(BOOL dll,WNDPROC wndProc,DWORD version,BOOL system=FALSE);
    ~AFX_MODULE_STATE() override {
        if(m_pTypeLibCacheMap) {
            m_pTypeLibCacheMap->RemoveAll(&m_typeLibCache);
            delete m_pTypeLibCacheMap;
        }
        impl___1CThreadLocalObject__QEAA_XZ(&m_moduleThreadState);
    }

    CWinApp* m_pCurrentWinApp;
    HINSTANCE m_hCurrentInstanceHandle;
    HINSTANCE m_hCurrentResourceHandle;
    const wchar_t* m_lpszCurrentAppName;
    BYTE m_bDLL;
    BYTE m_bSystem;
    BYTE m_bReserved[2];
    DWORD m_fRegisteredClasses;
    CRuntimeClass* m_pClassInit;
    CTypedSimpleList<CRuntimeClass*> m_classList;
    COleObjectFactory* m_pFactoryInit;
    CTypedSimpleList<COleObjectFactory*> m_factoryList;
    LONG m_nObjectCount;
    BOOL m_bUserCtrl;
    CString m_strUnregisterList;
    WNDPROC m_pfnAfxWndProc;
    DWORD m_dwVersion;
    void (AFXAPI* m_pfnFilterToolTipMessage)(MSG*,CWnd*);
    CTypedSimpleList<CDynLinkLibrary*> m_libraryList;
    HINSTANCE m_appLangDLL;
    COccManager* m_pOccManager;
    CTypedSimpleList<COleControlLock*> m_lockList;
    CTypeLibCache m_typeLibCache;
    CTypeLibCacheMap* m_pTypeLibCacheMap;
    S_Cthreadlocalobject m_moduleThreadState;
    BOOL m_bInitNetworkAddressControl;
    BOOL m_bInitNetworkAddressControlCalled;
};
static_assert(sizeof(AFX_MODULE_STATE)==0x108,"native module state size");
static_assert(offsetof(AFX_MODULE_STATE,m_pCurrentWinApp)==8,"native module application prefix");
static_assert(offsetof(AFX_MODULE_STATE,m_hCurrentResourceHandle)==0x18,"native current resource module");
static_assert(offsetof(AFX_MODULE_STATE,m_lpszCurrentAppName)==0x20,"native module application name");
static_assert(offsetof(AFX_MODULE_STATE,m_bSystem)==0x29,"native module resource search category");
static_assert(offsetof(AFX_MODULE_STATE,m_classList)==0x38,"native runtime class list");
static_assert(offsetof(AFX_MODULE_STATE,m_factoryList)==0x50,"native object factory list");
static_assert(offsetof(AFX_MODULE_STATE,m_strUnregisterList)==0x68,"native window class names");
static_assert(offsetof(AFX_MODULE_STATE,m_appLangDLL)==0x98,"native language resource module");
static_assert(offsetof(AFX_MODULE_STATE,m_pfnAfxWndProc)==0x70,"native framework window procedure");
static_assert(offsetof(AFX_MODULE_STATE,m_pfnFilterToolTipMessage)==0x80,"native tooltip filter");
static_assert(offsetof(AFX_MODULE_STATE,m_pOccManager)==0xa0,"native OLE manager");
static_assert(offsetof(AFX_MODULE_STATE,m_libraryList)==0x88,"native extension library list");
static_assert(offsetof(AFX_MODULE_STATE,m_lockList)==0xa8,"native OLE control lock list");
static_assert(offsetof(AFX_MODULE_STATE,m_typeLibCache)==0xb8,"native module type-library cache");
static_assert(offsetof(AFX_MODULE_STATE,m_pTypeLibCacheMap)==0xf0,"native type-library cache map");
static_assert(offsetof(AFX_MODULE_STATE,m_moduleThreadState)==0xf8,"native module thread slot");

namespace openmfc { namespace detail { namespace regcore {
extern AFX_MODULE_STATE g_moduleStateStatic;
} } }

namespace openmfc { namespace detail { namespace regcore {
AFX_MODULE_STATE* ResolveCurrentModuleState();
} } }
