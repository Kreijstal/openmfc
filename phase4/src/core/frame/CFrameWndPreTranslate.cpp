#define OPENMFC_APPCORE_IMPL
#include "detail/WindowMessageSupport.h"
#include "openmfc/afxole.h"
#include <cstring>

extern "C" void MS_ABI impl__AfxThrowInvalidArgException__YAXXZ();
extern "C" void MS_ABI impl__AfxCancelModes__YAXPEAUHWND_____Z(HWND);

BOOL CFrameWnd::PreTranslateMessage(MSG* message) {
    if (!message) {
        impl__AfxThrowInvalidArgException__YAXXZ();
        return FALSE;
    }
    if (message->message == WM_LBUTTONDOWN || message->message == WM_NCLBUTTONDOWN) {
        impl__AfxCancelModes__YAXPEAUHWND_____Z(message->hwnd);
    }

    constexpr DWORD keepVisible = 0x01;
    constexpr DWORD displayOnFocus = 0x02;
    constexpr DWORD displayOnF10 = 0x04;
    constexpr DWORD visible = 1;
    constexpr DWORD hidden = 2;
    auto& menu = m_retailTail;
    const UINT id = message->message;
    const WPARAM keyOrHit = message->wParam;
    if ((menu.m_dwMenuBarVisibility & displayOnF10) && id == WM_SYSKEYUP && keyOrHit == VK_F10) {
        SetMenuBarState(visible);
    }
    if (menu.m_dwMenuBarVisibility & displayOnFocus) {
        if (id == WM_SYSKEYUP && keyOrHit == VK_MENU) {
            SetMenuBarState(menu.m_dwMenuBarState == visible ? hidden : visible);
        } else if (id == WM_SYSCHAR && menu.m_dwMenuBarState == hidden) {
            SetMenuBarState(visible);
            menu.m_bTempShowMenu = TRUE;
        }
    }

    const bool nonClientPress = id == WM_NCLBUTTONDOWN || id == WM_NCRBUTTONDOWN;
    const bool clientPress = id == WM_LBUTTONDOWN || id == WM_RBUTTONDOWN;
    const bool escape = id == WM_KEYDOWN && keyOrHit == VK_ESCAPE;
    if (!(menu.m_dwMenuBarVisibility & keepVisible) &&
        (escape || clientPress || (nonClientPress && keyOrHit != HTMENU))) {
        SetMenuBarState(hidden);
    }
    if (nonClientPress) {
        menu.m_bMouseHitMenu = keyOrHit == HTMENU;
    } else if (id == WM_NCLBUTTONUP || id == WM_NCRBUTTONUP ||
               id == WM_LBUTTONUP || id == WM_RBUTTONUP) {
        menu.m_bMouseHitMenu = FALSE;
    }

    if (CWnd::PreTranslateMessage(message)) return TRUE;
    COleFrameHook* hook;
    std::memcpy(&hook, m_retailMiddle.m_pNotifyHook, sizeof(hook));
    if (hook && hook->OnPreTranslateMessage(message)) return TRUE;

    if (message->message < WM_KEYFIRST || message->message > WM_KEYLAST) return FALSE;
    const HACCEL accelerator = GetDefaultAccelerator();
    return accelerator && ::TranslateAcceleratorW(m_hWnd, accelerator, message);
}

// Symbol: ?PreTranslateMessage@CFrameWnd@@UEAAHPEAUtagMSG@@@Z
extern "C" BOOL MS_ABI impl__PreTranslateMessage_CFrameWnd__UEAAHPEAUtagMSG___Z(CFrameWnd* frame, MSG* message) {
    return frame ? frame->CFrameWnd::PreTranslateMessage(message) : FALSE;
}
