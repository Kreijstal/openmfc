#pragma once
// Shared internals of the former wincore.cpp translation unit.
// Definitions live in detail/WincoreSupport.cpp, so file-scope state keeps a
// single instance across the classes this unit was split into.
namespace openmfc { namespace detail { namespace wincore {} } }
using namespace openmfc::detail::wincore;
// Types declared by the internals of other units.
struct AFX_INTERFACEMAP;
// Window Core Implementation
//
// Implements CWnd, CFrameWnd, and related window creation/management.
// This is essential for Hello World applications.

// Define OPENMFC_APPCORE_IMPL to prevent inline implementations conflicting with appcore.cpp
#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxwin.h"
#include "openmfc/afxole.h"
#include "openmfc/afxmfc.h"
#include <windows.h>
#include <commctrl.h>
#include <cstring>
#include <cstdio>
#include <algorithm>
#include <new>
#include <string>

// MS ABI calling convention
#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

// Forward declarations
LRESULT CALLBACK AfxWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
ATOM RegisterOpenMFCClass(HINSTANCE hInstance);

// Exported stubs used internally (avoid virtual dispatch)
extern "C" int MS_ABI impl__PreCreateWindow_CWnd__UEAAHAEAUtagCREATESTRUCTW___Z(CWnd* pThis, CREATESTRUCTW& cs);
extern "C" int MS_ABI impl__PreCreateWindow_CFrameWnd__MEAAHAEAUtagCREATESTRUCTW___Z(CFrameWnd* pThis, CREATESTRUCTW& cs);
extern "C" const AFX_INTERFACEMAP* MS_ABI impl__GetThisInterfaceMap_CCmdTarget__KAPEBUAFX_INTERFACEMAP__XZ();

// =============================================================================
// Global State
// =============================================================================

// Map HWND to CWnd* for message routing
#include <map>
#include <set>
#include <mutex>
#include <vector>
namespace openmfc { namespace detail { namespace wincore {
extern std::map<HWND, CWnd*> g_hwndMap;
} } }
namespace openmfc { namespace detail { namespace wincore {
extern std::map<const CWnd*, COleControlContainer*> g_controlContainerMap;
} } }
struct CWndRuntimeState {
    bool toolTipsEnabled = false;
    bool trackingToolTipsEnabled = false;
    bool gestureConfigEnabled = false;
    bool d2dSupportEnabled = false;
    bool dynamicLayoutEnabled = false;
    std::wstring dynamicLayoutResource;
    std::map<std::pair<long, WORD>, void*> properties;
};
namespace openmfc { namespace detail { namespace wincore {
extern std::map<CWnd*, CWndRuntimeState> g_wndRuntimeStates;
} } }

struct CFrameWndRuntimeState {
    DWORD menuBarState = 0;
    DWORD menuBarVisibility = 0;
    HMENU hiddenMenu = nullptr;
    UINT menuResourceId = 0;   // frame's own menu resource, not m_nIDHelp
    HMENU ownedMenu = nullptr;  // menu we LoadMenuW'd and must DestroyMenu
    int progressMin = 0;
    int progressMax = 100;
    int progressPos = 0;
    DWORD dockingStyle = 0;
    int modalDepth = 0;
    HWND modalOwner = nullptr;
    std::vector<CControlBar*> controlBars;
};
namespace openmfc { namespace detail { namespace wincore {
extern std::map<CFrameWnd*, CFrameWndRuntimeState> g_frameWndRuntimeStates;
} } }

struct DockBarSnapshot {
    CControlBar* bar = nullptr;
    UINT id = 0;
    bool visible = false;
    DWORD dockStyle = 0;
    RECT windowRect{};
};
namespace openmfc { namespace detail { namespace wincore {
extern std::map<const CDockState*, std::vector<DockBarSnapshot>> g_dockStateSnapshots;
} } }

// Track temporary CWnd wrappers allocated by OpenMfcAttachCWnd
// These need to be deleted when the underlying window is destroyed
namespace openmfc { namespace detail { namespace wincore {
extern std::set<CWnd*> g_tempWrappers;
} } }

// Guards the process-wide runtime-state registries above. MFC apps may create
// and destroy windows on several UI threads, so the map/set structural
// operations must be serialized to avoid corrupting the trees. std::map keeps
// references to existing elements valid across insertions of other keys, so the
// reference returned by the accessors stays usable outside the lock (a given
// window's state is only erased on that window's own thread at destruction).
namespace openmfc { namespace detail { namespace wincore {
extern std::mutex g_wndStateMutex;
} } }

// In-memory control-bar visibility persistence keyed by profile name. Real MFC
// serializes bar layout to the application profile (registry); this clean-room
// CWinApp exposes no profile API yet, so Save/LoadBarState round-trip within the
// process, which is enough to actually restore bar visibility rather than fake
// it. Guarded by g_wndStateMutex.
namespace openmfc { namespace detail { namespace wincore {
extern std::map<std::wstring, std::vector<int>> g_barStateStore;
} } }

namespace openmfc { namespace detail { namespace wincore {
CWndRuntimeState& GetWindowRuntimeState(CWnd* pWnd);
} } }

namespace openmfc { namespace detail { namespace wincore {
CFrameWndRuntimeState& GetFrameRuntimeState(CFrameWnd* pWnd);
} } }

extern "C" void OpenMfcResetDockState(void* pState);

namespace openmfc { namespace detail { namespace wincore {
void CleanupWindowRuntimeState(CWnd* pWnd);
} } }

namespace openmfc { namespace detail { namespace wincore {
ITaskbarList3* AcquireTaskbarList3ForWindow(HWND hWnd);
} } }

// Helper to reuse/attach CWnd wrappers for existing HWNDs.
namespace openmfc { namespace detail { namespace wincore {
CWnd* OpenMfcLookupCWnd(HWND hWnd);
} } }

// Detach and optionally delete a CWnd wrapper when window is destroyed
// Called from AfxWndProc on WM_NCDESTROY (the final cleanup message)
namespace openmfc { namespace detail { namespace wincore {
void OpenMfcDetachCWnd(HWND hWnd);
} } }

CWnd* OpenMfcAttachCWnd(HWND hWnd);

// Cleanup stale temporary wrappers for destroyed windows
// Called during idle processing to handle windows not using our window procedure
// (e.g., dialog controls obtained via GetDlgItem)
void OpenMfcCleanupTempWrappers();

// Global app pointer (defined in appcore.cpp)
extern CWinApp* g_pApp;
extern CWinThread* AfxGetThread();
namespace openmfc { namespace detail { namespace wincore {
void CWnd_PreSubclassWindowCompat(CWnd* pThis);
} } }

namespace openmfc { namespace detail { namespace wincore {
void CWnd_SetMessageText(CWnd* pThis, const wchar_t* lpszText);
} } }

// Returns nonzero when the message was handled and writes the result to pResult.
extern "C" int MS_ABI impl__OnWndMsg_CWnd__MEAAHI_K_JPEA_J_Z(
    CWnd* pThis, UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);

// OpenMFC window class name
static const wchar_t* g_szOpenMFCClass = L"OpenMFC_Window";
namespace openmfc { namespace detail { namespace wincore {
extern ATOM g_atomOpenMFCClass;
} } }

namespace openmfc { namespace detail { namespace wincore {
struct StaticCWndExport {
    void* vptr;
    unsigned char padToHwnd[56];
    HWND hWnd;
    unsigned char tail[160];
};
} } }






// =============================================================================
// CWnd Implementation
// =============================================================================


#ifdef __GNUC__
// CWnd::classCWnd - MSVC symbol alias
asm(".globl \"?classCWnd@CWnd@@2UCRuntimeClass@@A\"\n"
    ".set \"?classCWnd@CWnd@@2UCRuntimeClass@@A\", _ZN4CWnd9classCWndE\n");
#endif

// CWnd::FromHandle
namespace openmfc { namespace detail { namespace wincore {
extern "C" CWnd* MS_ABI impl__FromHandle_CWnd__SAPAV1_PAUHWND_____Z(HWND hWnd);
} } }





#ifdef __GNUC__
asm(".globl \"?GetThisClass@CWnd@@SAPEAUCRuntimeClass@@XZ\"\n"
    ".set \"?GetThisClass@CWnd@@SAPEAUCRuntimeClass@@XZ\", impl__GetThisClass_CWnd__SAPEAUCRuntimeClass__XZ\n");
#endif


#ifdef __GNUC__
// CFrameWnd::classCFrameWnd - MSVC symbol alias
asm(".globl \"?classCFrameWnd@CFrameWnd@@2UCRuntimeClass@@A\"\n"
    ".set \"?classCFrameWnd@CFrameWnd@@2UCRuntimeClass@@A\", _ZN9CFrameWnd14classCFrameWndE\n");
#endif


#ifdef __GNUC__
asm(".globl \"?GetThisClass@CFrameWnd@@SAPEAUCRuntimeClass@@XZ\"\n"
    ".set \"?GetThisClass@CFrameWnd@@SAPEAUCRuntimeClass@@XZ\", impl__GetThisClass_CFrameWnd__SAPEAUCRuntimeClass__XZ\n");
#endif





// CWnd::UpdateWindow
namespace openmfc { namespace detail { namespace wincore {
extern "C" void MS_ABI impl__UpdateWindow_CWnd__QEAAXXZ(CWnd* pThis);
} } }







// CWnd::GetSafeHwnd
namespace openmfc { namespace detail { namespace wincore {
extern "C" HWND MS_ABI impl__GetSafeHwnd_CWnd__QEBAPEAUHWND____XZ(const CWnd* pThis);
} } }







// =============================================================================
// CFrameWnd Implementation
// =============================================================================














// =============================================================================
// Window Procedure
// =============================================================================

LRESULT CALLBACK AfxWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

// =============================================================================
// Window Class Registration
// =============================================================================

ATOM RegisterOpenMFCClass(HINSTANCE hInstance);

// =============================================================================
// AfxWinMain - Application Entry Point
// =============================================================================


// =============================================================================
// CMDIFrameWnd Implementation
// =============================================================================



// CMDIFrameWnd::CreateClient
// Creates the MDI client window

// CMDIFrameWnd::GetWindowMenuPopup

// MDI helper functions









// =============================================================================
// CMDIChildWnd Implementation
// =============================================================================



















// CWnd's message map: real MFC bases it on CCmdTarget (harvested by pointer
// identity from mfc140u.dll). Returning null here made a real client crash when it
// walked CWnd's map during message/command routing. The handler entries are real
// mfc140u code addresses we cannot replicate, so use the empty terminator and chain
// pfnGetBaseMap to CCmdTarget's exported GetThisMessageMap (the _AFXDLL mechanism).
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CCmdTarget__KAPEBUAFX_MSGMAP__XZ();
namespace openmfc { namespace detail { namespace wincore {
extern const AFX_MSGMAP_ENTRY g_cwndEmptyMsgEntries[];
} } }
namespace openmfc { namespace detail { namespace wincore {
const AFX_MSGMAP* AFXAPI gbm_CWnd_base();
} } }
namespace openmfc { namespace detail { namespace wincore {
extern const AFX_MSGMAP g_cwndMessageMap;
} } }

namespace openmfc { namespace detail { namespace wincore {
int CWnd_PreTranslateMessageCompat(CWnd* pThis, MSG* pMsg);
} } }













// FIXME: accNavigate ABI back-ref type mismatch with MSVC export
/*
long CWnd::accNavigate(long p0, VARIANT p1, void* p2)
{
    (void)p0;
    (void)p1;
    (void)p2;
    return 0;
}
*/































// FIXME: SetProxy struct vs class ABI mismatch with MSVC export
/*
long CWnd::SetProxy(IAccessibleProxy* p0)
{
    (void)p0;
    return 0;
}
*/

























































// GetMessageMap is provided by DECLARE_MESSAGE_MAP macro
/*
const AFX_MSGMAP* CWnd::GetMessageMap() const
{
    return nullptr;
}
*/






















































































































//=============================================================================
// CFrameWnd implementations
//=============================================================================








































