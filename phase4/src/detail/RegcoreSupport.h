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
class AFX_MODULE_STATE : public CNoTrackObject {
public:
    CWinApp* m_pCurrentWinApp{};
    HINSTANCE m_hCurrentInstanceHandle{};
    HINSTANCE m_hCurrentResourceHandle{};
    // Preserve the native x64 prefix through the callbacks used by windows.
    // The intervening module services are not modeled by this shadow yet.
    BYTE m_reservedModuleFlags[8]{};
    BYTE m_bDLL=FALSE;
    BYTE m_reservedToWindowProcedure[0x70-0x29]{};
    WNDPROC m_pfnAfxWndProc=&AfxWndProc;
    BYTE m_reservedToToolTip[8]{};
    void (AFXAPI* m_pfnFilterToolTipMessage)(MSG*,CWnd*)=nullptr;
    BYTE m_reservedToOccManager[0xa0-0x88]{};
    void* m_pOccManager{};
    BYTE m_reservedToModuleThreadSlot[0xf8-0xa8]{};
    S_Cthreadlocalobject m_moduleThreadState{};
    ~AFX_MODULE_STATE() override { impl___1CThreadLocalObject__QEAA_XZ(&m_moduleThreadState); }
};
static_assert(offsetof(AFX_MODULE_STATE,m_pCurrentWinApp)==8,"native module application prefix");
static_assert(offsetof(AFX_MODULE_STATE,m_pfnAfxWndProc)==0x70,"native framework window procedure");
static_assert(offsetof(AFX_MODULE_STATE,m_pfnFilterToolTipMessage)==0x80,"native tooltip filter");
static_assert(offsetof(AFX_MODULE_STATE,m_pOccManager)==0xa0,"native OLE manager");
static_assert(offsetof(AFX_MODULE_STATE,m_moduleThreadState)==0xf8,"native module thread slot");

namespace openmfc { namespace detail { namespace regcore {
extern AFX_MODULE_STATE g_moduleStateStatic;
} } }

namespace openmfc { namespace detail { namespace regcore {
AFX_MODULE_STATE* ResolveCurrentModuleState();
} } }

namespace openmfc { namespace detail { namespace regcore {
void RefreshStaticModuleState();
} } }
