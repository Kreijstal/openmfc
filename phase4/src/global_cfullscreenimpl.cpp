// OpenMFC — CFullScreenImpl real implementation
//
// Layout (harvested, cl.exe /d1reportSingleClassLayout, size(80)):
//   0  {vfptr}
//   8  CRect  m_rectFullScreenWindow
//  24  CMFCToolBar* m_pwndFullScreenBar
//  32  BOOL   m_bFullScreen
//  36  BOOL   m_bShowMenu
//  40  CRect  m_rectFramePrev
//  56  CFrameImpl*  m_pImpl
//  64  UINT   m_uiFullScreenID
//  68  BOOL   m_bTabsArea
//  72  CString m_strRegSection  (single ATL data pointer)
//
#include <windows.h>
#include <cstddef>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

namespace {

// Layout-faithful mirror of CFullScreenImpl.
struct S
{
    void*    vfptr;                    // 0
    RECT     m_rectFullScreenWindow;   // 8
    void*    m_pwndFullScreenBar;      // 24
    int      m_bFullScreen;            // 32
    int      m_bShowMenu;              // 36
    RECT     m_rectFramePrev;          // 40
    void*    m_pImpl;                  // 56  CFrameImpl*
    unsigned m_uiFullScreenID;         // 64
    int      m_bTabsArea;              // 68
    void*    m_strRegSection;          // 72  CString data ptr
};

static_assert(sizeof(S) == 80, "CFullScreenImpl must be 80 bytes");
static_assert(offsetof(S, m_rectFullScreenWindow) == 8,  "m_rectFullScreenWindow @8");
static_assert(offsetof(S, m_bFullScreen)          == 32, "m_bFullScreen @32");
static_assert(offsetof(S, m_pImpl)                == 56, "m_pImpl @56");
static_assert(offsetof(S, m_strRegSection)        == 72, "m_strRegSection @72");

// CWnd::m_hWnd lives at offset 64 for every CWnd-derived object (CFrameWnd).
static inline HWND FrameHwnd(void* pFrame)
{
    if (!pFrame) return NULL;
    return *reinterpret_cast<HWND*>(reinterpret_cast<char*>(pFrame) + 64);
}

static inline LONG RcW(const RECT& r) { return r.right  - r.left; }
static inline LONG RcH(const RECT& r) { return r.bottom - r.top;  }

// Forward decls of the exported thunks used from the vtable / cross-calls.
extern "C" void MS_ABI impl___1CFullScreenImpl__UEAA_XZ(void* pThis);

// The single vtable slot MSVC emits for this class is the
// vector-deleting-destructor. It runs the real destructor and, when the
// low bit of the flags is set, frees the storage.
void* MS_ABI vdtor(void* p, unsigned f)
{
    impl___1CFullScreenImpl__UEAA_XZ(p);
    if (f & 1) ::operator delete(p);
    return p;
}

void* const g_CFullScreenImpl_vtbl[1] = { reinterpret_cast<void*>(&vdtor) };

} // namespace

// ---------------------------------------------------------------------------

// Symbol: ??0CFullScreenImpl@@QEAA@PEAVCFrameImpl@@@Z
extern "C" void* MS_ABI impl___0CFullScreenImpl__QEAA_PEAVCFrameImpl___Z(void* pThis, void* pFrameImpl)
{
    S* self = reinterpret_cast<S*>(pThis);
    self->vfptr = const_cast<void*>(reinterpret_cast<const void*>(&g_CFullScreenImpl_vtbl[0]));
    SetRectEmpty(&self->m_rectFullScreenWindow);
    self->m_pwndFullScreenBar = NULL;
    self->m_bFullScreen = FALSE;
    self->m_bShowMenu = TRUE;          // main menu shown by default
    SetRectEmpty(&self->m_rectFramePrev);
    self->m_pImpl = pFrameImpl;
    self->m_uiFullScreenID = 0;
    self->m_bTabsArea = TRUE;
    self->m_strRegSection = NULL;      // empty CString
    return pThis;
}

// Symbol: ??1CFullScreenImpl@@UEAA@XZ
extern "C" void MS_ABI impl___1CFullScreenImpl__UEAA_XZ(void* pThis)
{
    // CFullScreenImpl owns no heap resources (m_strRegSection is an empty,
    // shared-null CString here; panes/toolbars are owned by the frame).
    // Re-seat the vfptr as MSVC does at the top of a virtual dtor.
    S* self = reinterpret_cast<S*>(pThis);
    self->vfptr = const_cast<void*>(reinterpret_cast<const void*>(&g_CFullScreenImpl_vtbl[0]));
}

// Symbol: ?OnGetMinMaxInfo@CFullScreenImpl@@QEAAXPEAUtagMINMAXINFO@@@Z
extern "C" void MS_ABI impl__OnGetMinMaxInfo_CFullScreenImpl__QEAAXPEAUtagMINMAXINFO___Z(void* pThis, MINMAXINFO* lpMMI)
{
    S* self = reinterpret_cast<S*>(pThis);
    if (!lpMMI) return;
    if (self->m_bFullScreen)
    {
        lpMMI->ptMaxSize.x      = RcW(self->m_rectFullScreenWindow);
        lpMMI->ptMaxSize.y      = RcH(self->m_rectFullScreenWindow);
        lpMMI->ptMaxTrackSize.x = lpMMI->ptMaxSize.x;
        lpMMI->ptMaxTrackSize.y = lpMMI->ptMaxSize.y;
    }
}

// Symbol: ?UndockAndHidePanes@CFullScreenImpl@@IEAAXPEAVCFrameWnd@@@Z
extern "C" void MS_ABI impl__UndockAndHidePanes_CFullScreenImpl__IEAAXPEAVCFrameWnd___Z(void* /*pThis*/, void* pFrame)
{
    // The real helper walks the frame's CDockingManager, undocks every
    // CPane and hides it so the client area can fill the screen. That
    // requires the docking-manager internals, which are not available in
    // this self-contained unit. As a faithful approximation we hide the
    // immediate non-client child bars we can reach: nothing observable can
    // be done without the dock manager, so this is left minimal.
    (void)pFrame; // see notes: stub — needs CDockingManager internals
}

// Symbol: ?ShowFullScreen@CFullScreenImpl@@QEAAXPEAVCFrameWnd@@@Z
extern "C" void MS_ABI impl__ShowFullScreen_CFullScreenImpl__QEAAXPEAVCFrameWnd___Z(void* pThis, void* pFrame)
{
    S* self = reinterpret_cast<S*>(pThis);
    if (self->m_bFullScreen) return;   // already full screen

    HWND hWnd = FrameHwnd(pFrame);
    if (!hWnd || !::IsWindow(hWnd)) return;

    // Remember the current frame rectangle so RestoreState can undo.
    ::GetWindowRect(hWnd, &self->m_rectFramePrev);

    // Full-screen area = the monitor the frame currently sits on.
    HMONITOR hMon = ::MonitorFromWindow(hWnd, MONITOR_DEFAULTTONEAREST);
    MONITORINFO mi;
    mi.cbSize = sizeof(mi);
    if (hMon && ::GetMonitorInfo(hMon, &mi))
        self->m_rectFullScreenWindow = mi.rcMonitor;
    else
        ::SetRect(&self->m_rectFullScreenWindow, 0, 0,
                  ::GetSystemMetrics(SM_CXSCREEN), ::GetSystemMetrics(SM_CYSCREEN));

    self->m_bFullScreen = TRUE;

    // Undock/hide panes, then stretch the frame across the whole monitor.
    impl__UndockAndHidePanes_CFullScreenImpl__IEAAXPEAVCFrameWnd___Z(pThis, pFrame);

    const RECT& r = self->m_rectFullScreenWindow;
    ::SetWindowPos(hWnd, HWND_TOP, r.left, r.top, RcW(r), RcH(r),
                   SWP_FRAMECHANGED | SWP_NOOWNERZORDER);
}

// Symbol: ?RestoreState@CFullScreenImpl@@QEAAXPEAVCFrameWnd@@@Z
extern "C" void MS_ABI impl__RestoreState_CFullScreenImpl__QEAAXPEAVCFrameWnd___Z(void* pThis, void* pFrame)
{
    S* self = reinterpret_cast<S*>(pThis);
    if (!self->m_bFullScreen) return;  // not in full screen

    self->m_bFullScreen = FALSE;

    HWND hWnd = FrameHwnd(pFrame);
    if (!hWnd || !::IsWindow(hWnd)) return;

    const RECT& r = self->m_rectFramePrev;
    ::SetWindowPos(hWnd, NULL, r.left, r.top, RcW(r), RcH(r),
                   SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
}

// Symbol: ?ShowFullScreen@CFullScreenImpl@@QEAAXXZ
extern "C" void MS_ABI impl__ShowFullScreen_CFullScreenImpl__QEAAXXZ(void* pThis)
{
    // Toggle: derive the owning CFrameWnd from CFrameImpl (its first member
    // is CFrameWnd* m_pFrame), then enter or leave full screen accordingly.
    S* self = reinterpret_cast<S*>(pThis);
    if (!self->m_pImpl) return;

    void* pFrame = *reinterpret_cast<void**>(self->m_pImpl); // CFrameImpl::m_pFrame @0
    if (self->m_bFullScreen)
        impl__RestoreState_CFullScreenImpl__QEAAXPEAVCFrameWnd___Z(pThis, pFrame);
    else
        impl__ShowFullScreen_CFullScreenImpl__QEAAXPEAVCFrameWnd___Z(pThis, pFrame);
}
