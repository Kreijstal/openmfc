// CFullScreenImpl — OpenMFC implementation.
// Sources: global_cfullscreenimpl.cpp

#include "detail/CFullScreenImplSupport.h"

// Symbol: ??0CFullScreenImpl@@QEAA@PEAVCFrameImpl@@@Z
extern "C" void* MS_ABI impl___0CFullScreenImpl__QEAA_PEAVCFrameImpl___Z(void* pThis, void* pFrameImpl)
{
    S_Cfullscreenimpl* self = reinterpret_cast<S_Cfullscreenimpl*>(pThis);
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
    S_Cfullscreenimpl* self = reinterpret_cast<S_Cfullscreenimpl*>(pThis);
    self->vfptr = const_cast<void*>(reinterpret_cast<const void*>(&g_CFullScreenImpl_vtbl[0]));
}
// Symbol: ?OnGetMinMaxInfo@CFullScreenImpl@@QEAAXPEAUtagMINMAXINFO@@@Z
extern "C" void MS_ABI impl__OnGetMinMaxInfo_CFullScreenImpl__QEAAXPEAUtagMINMAXINFO___Z(void* pThis, MINMAXINFO* lpMMI)
{
    S_Cfullscreenimpl* self = reinterpret_cast<S_Cfullscreenimpl*>(pThis);
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
extern "C" void MS_ABI impl__UndockAndHidePanes_CFullScreenImpl__IEAAXPEAVCFrameWnd___Z(void* pThis, void* pFrame)
{
    if (!pFrame) return;

    HWND hWnd = FrameHwnd(pFrame);
    if (!hWnd || !::IsWindow(hWnd)) return;

    auto& st = g_fullScreenState[pFrame];
    if (st.menuStored) {
        return; // already saved/hid for this frame instance
    }

    if (pThis)
    {
        S_Cfullscreenimpl* self = reinterpret_cast<S_Cfullscreenimpl*>(pThis);
        // Preserve and optionally hide the frame menu.
        if (self->m_bShowMenu)
        {
            st.menu = ::GetMenu(hWnd);
            if (st.menu)
            {
                st.menuStored = true;
                ::SetMenu(hWnd, NULL);
                ::DrawMenuBar(hWnd);
                st.menuStored = true;
            }
        }
    }

    HidePanesContext ctx{hWnd, &st};
    ::EnumChildWindows(hWnd, HidePanesEnumProc, reinterpret_cast<LPARAM>(&ctx));

    if (!st.hiddenBars.empty() && !st.menuStored)
    {
        st.menuStored = true; // ensure cleanup path is still symmetric when no menu exists
    }
}
// Symbol: ?ShowFullScreen@CFullScreenImpl@@QEAAXPEAVCFrameWnd@@@Z
extern "C" void MS_ABI impl__ShowFullScreen_CFullScreenImpl__QEAAXPEAVCFrameWnd___Z(void* pThis, void* pFrame)
{
    S_Cfullscreenimpl* self = reinterpret_cast<S_Cfullscreenimpl*>(pThis);
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
    S_Cfullscreenimpl* self = reinterpret_cast<S_Cfullscreenimpl*>(pThis);
    if (!self->m_bFullScreen) return;  // not in full screen

    self->m_bFullScreen = FALSE;

    HWND hWnd = FrameHwnd(pFrame);
    if (!hWnd || !::IsWindow(hWnd)) return;

    const RECT& r = self->m_rectFramePrev;
    ::SetWindowPos(hWnd, NULL, r.left, r.top, RcW(r), RcH(r),
                   SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_FRAMECHANGED);

    auto it = g_fullScreenState.find(pFrame);
    if (it != g_fullScreenState.end())
    {
        if (it->second.menuStored && it->second.menu)
        {
            ::SetMenu(hWnd, it->second.menu);
            ::DrawMenuBar(hWnd);
        }

        for (HWND hwnd : it->second.hiddenBars)
        {
            if (hwnd && ::IsWindow(hwnd))
            {
                ::ShowWindow(hwnd, SW_SHOW);
            }
        }
        g_fullScreenState.erase(it);
    }
}
// Symbol: ?ShowFullScreen@CFullScreenImpl@@QEAAXXZ
extern "C" void MS_ABI impl__ShowFullScreen_CFullScreenImpl__QEAAXXZ(void* pThis)
{
    // Toggle: derive the owning CFrameWnd from CFrameImpl (its first member
    // is CFrameWnd* m_pFrame), then enter or leave full screen accordingly.
    S_Cfullscreenimpl* self = reinterpret_cast<S_Cfullscreenimpl*>(pThis);
    if (!self->m_pImpl) return;

    void* pFrame = *reinterpret_cast<void**>(self->m_pImpl); // CFrameImpl::m_pFrame @0
    if (self->m_bFullScreen)
        impl__RestoreState_CFullScreenImpl__QEAAXPEAVCFrameWnd___Z(pThis, pFrame);
    else
        impl__ShowFullScreen_CFullScreenImpl__QEAAXPEAVCFrameWnd___Z(pThis, pFrame);
}

// CFullScreenImpl's MSVC-layout vtable: defined here, next to the
// implementations it points at.
namespace openmfc { namespace detail { namespace cfullscreenimpl {
extern void* const g_CFullScreenImpl_vtbl[1] = { reinterpret_cast<void*>(&vdtor_Cfullscreenimpl) };
} } }
