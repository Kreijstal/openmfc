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

struct AFX_MODULE_STATE {
    HINSTANCE m_hCurrentInstanceHandle;
    HINSTANCE m_hCurrentResourceHandle;
    CWinApp* m_pCurrentWinApp;
};

namespace openmfc { namespace detail { namespace regcore {
extern AFX_MODULE_STATE g_moduleStateStatic;
} } }
namespace openmfc { namespace detail { namespace regcore {
extern thread_local AFX_MODULE_STATE* g_pCurrentModuleState;
} } }

namespace openmfc { namespace detail { namespace regcore {
AFX_MODULE_STATE* ResolveCurrentModuleState();
} } }

namespace openmfc { namespace detail { namespace regcore {
void RefreshStaticModuleState();
} } }




// =============================================================================
// Module Thread State
// =============================================================================

struct AFX_MODULE_THREAD_STATE_Regcore {
    // Simplified to the fields that are currently needed by OpenMFC callers.
    int m_nTempMapLock;
};

namespace openmfc { namespace detail { namespace regcore {
extern thread_local AFX_MODULE_THREAD_STATE_Regcore g_moduleThreadState;
} } }

