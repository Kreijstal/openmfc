// CMDIFrameWnd — OpenMFC implementation.
// Sources: frame_font_exports.cpp, wincore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/FrameFontExportsSupport.h"
#include "detail/WincoreSupport.h"

// Symbol: ?CreateNewChild@CMDIFrameWnd@@QEAAPEAVCMDIChildWnd@@PEAUCRuntimeClass@@IPEAUHMENU__@@PEAUHACCEL__@@@Z
extern "C" CMDIChildWnd* MS_ABI impl__CreateNewChild_CMDIFrameWnd__QEAAPEAVCMDIChildWnd__PEAUCRuntimeClass__IPEAUHMENU____PEAUHACCEL_____Z(
    CMDIFrameWnd* pThis, CRuntimeClass* pClass, UINT nResource, HMENU hMenu, HACCEL hAccel) {
    (void)nResource;
    (void)hMenu;
    (void)hAccel;
    if (!pThis) return nullptr;
    CObject* pObject = pClass && pClass->m_pfnCreateObject ? pClass->m_pfnCreateObject() : CMDIChildWnd::CreateObject();
    CMDIChildWnd* pChild = dynamic_cast<CMDIChildWnd*>(pObject);
    if (!pChild) {
        delete pObject;
        pChild = new CMDIChildWnd();
    }
    RECT rect = {CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT};
    if (!pChild->Create(nullptr, L"", WS_CHILD | WS_VISIBLE | WS_OVERLAPPEDWINDOW, rect, pThis, nullptr)) {
        delete pChild;
        return nullptr;
    }
    return pChild;
}
// Symbol: ?CreateObject@CMDIFrameWnd@@SAPEAVCObject@@XZ
extern "C" CObject* MS_ABI impl__CreateObject_CMDIFrameWnd__SAPEAVCObject__XZ() {
    return CMDIFrameWnd::CreateObject();
}
// Symbol: ?DefWindowProcW@CMDIFrameWnd@@MEAA_JI_K_J@Z
extern "C" LRESULT MS_ABI impl__DefWindowProcW_CMDIFrameWnd__MEAA_JI_K_J_Z(
    CMDIFrameWnd* pThis, UINT message, WPARAM wParam, LPARAM lParam) {
    if (pThis && pThis->m_hWnd) {
        return ::DefFrameProcW(pThis->m_hWnd, pThis->m_hWndMDIClient, message, wParam, lParam);
    }
    return impl__DefWindowProcW_CWnd__MEAA_JI_K_J_Z(pThis, message, wParam, lParam);
}
// Symbol: ?DelayUpdateFrameMenu@CMDIFrameWnd@@UEAAXPEAUHMENU__@@@Z
extern "C" void MS_ABI impl__DelayUpdateFrameMenu_CMDIFrameWnd__UEAAXPEAUHMENU_____Z(CMDIFrameWnd* pThis, HMENU hMenuAlt) {
    if (pThis) {
        pThis->CFrameWnd::DelayUpdateFrameMenu(hMenuAlt);
    }
}
// Symbol: ?GetActiveFrame@CMDIFrameWnd@@UEAAPEAVCFrameWnd@@XZ
extern "C" CFrameWnd* MS_ABI impl__GetActiveFrame_CMDIFrameWnd__UEAAPEAVCFrameWnd__XZ(CMDIFrameWnd* pThis) {
    CWnd* pActive = pThis ? pThis->MDIGetActive(nullptr) : nullptr;
    CFrameWnd* pFrame = dynamic_cast<CFrameWnd*>(pActive);
    return pFrame ? pFrame : pThis;
}
// Symbol: ?GetMessageMap@CMDIFrameWnd@@MEBAPEBUAFX_MSGMAP@@XZ
extern "C" const AFX_MSGMAP* MS_ABI impl__GetMessageMap_CMDIFrameWnd__MEBAPEBUAFX_MSGMAP__XZ(
    const CMDIFrameWnd* pThis) {
    (void)pThis;
    return EmptyMessageMap_FrameFontExports();
}
// Symbol: ?GetRuntimeClass@CMDIFrameWnd@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CMDIFrameWnd__UEBAPEAUCRuntimeClass__XZ(
    const CMDIFrameWnd* pThis) {
    return CMDIFrameWnd::GetThisClass();
}
// Symbol: ?GetThisClass@CMDIFrameWnd@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMDIFrameWnd__SAPEAUCRuntimeClass__XZ() {
    return CMDIFrameWnd::GetThisClass();
}
// Symbol: ?GetThisMessageMap@CMDIFrameWnd@@KAPEBUAFX_MSGMAP@@XZ
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CMDIFrameWnd__KAPEBUAFX_MSGMAP__XZ() {
    return EmptyMessageMap_FrameFontExports();
}
// Symbol: ?LoadFrame@CMDIFrameWnd@@UEAAHIKPEAVCWnd@@PEAUCCreateContext@@@Z
extern "C" int MS_ABI impl__LoadFrame_CMDIFrameWnd__UEAAHIKPEAVCWnd__PEAUCCreateContext___Z(
    CMDIFrameWnd* pThis, UINT nIDResource, DWORD dwDefaultStyle, CWnd* pParentWnd, CCreateContext* pContext) {
    return impl__LoadFrame_CFrameWnd__UEAAHIKPEAVCWnd__PEAUCCreateContext___Z(
        pThis, nIDResource, dwDefaultStyle, pParentWnd, pContext);
}
// Symbol: ?OnCmdMsg@CMDIFrameWnd@@UEAAHIHPEAXPEAUAFX_CMDHANDLERINFO@@@Z
extern "C" int MS_ABI impl__OnCmdMsg_CMDIFrameWnd__UEAAHIHPEAXPEAUAFX_CMDHANDLERINFO___Z(
    CMDIFrameWnd* pThis, UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) {
    return impl__OnCmdMsg_CFrameWnd__UEAAHIHPEAXPEAUAFX_CMDHANDLERINFO___Z(
        pThis, nID, nCode, pExtra, pHandlerInfo);
}
// Symbol: ?OnCommand@CMDIFrameWnd@@MEAAH_K_J@Z
extern "C" int MS_ABI impl__OnCommand_CMDIFrameWnd__MEAAH_K_J_Z(CMDIFrameWnd* pThis, WPARAM wParam, LPARAM lParam) {
    return impl__OnCommand_CFrameWnd__MEAAH_K_J_Z(pThis, wParam, lParam);
}
// Symbol: ?OnCommandHelp@CMDIFrameWnd@@IEAA_J_K_J@Z
extern "C" LRESULT MS_ABI impl__OnCommandHelp_CMDIFrameWnd__IEAA_J_K_J_Z(
    CMDIFrameWnd* pThis, WPARAM wParam, LPARAM lParam) {
    (void)pThis;
    (void)wParam;
    (void)lParam;
    return 0;
}
// Symbol: ?OnCreateClient@CMDIFrameWnd@@UEAAHPEAUtagCREATESTRUCTW@@PEAUCCreateContext@@@Z
extern "C" int MS_ABI impl__OnCreateClient_CMDIFrameWnd__UEAAHPEAUtagCREATESTRUCTW__PEAUCCreateContext___Z(
    CMDIFrameWnd* pThis, CREATESTRUCTW* pCreateStruct, CCreateContext* pContext) {
    (void)pContext;
    return pThis ? pThis->CreateClient(pCreateStruct, nullptr) : FALSE;
}
// Symbol: ?OnDestroy@CMDIFrameWnd@@IEAAXXZ
extern "C" void MS_ABI impl__OnDestroy_CMDIFrameWnd__IEAAXXZ(CMDIFrameWnd* pThis) {
    if (pThis) pThis->m_hWndMDIClient = nullptr;
}
// Symbol: ?OnIdleUpdateCmdUI@CMDIFrameWnd@@IEAAXXZ
extern "C" void MS_ABI impl__OnIdleUpdateCmdUI_CMDIFrameWnd__IEAAXXZ(CMDIFrameWnd* pThis) {
    if (!pThis) return;
    pThis->CFrameWnd::OnIdleUpdateCmdUI();
    pThis->CFrameWnd::OnUpdateFrameTitle(0);
}
// Symbol: ?OnMDIWindowCmd@CMDIFrameWnd@@IEAAHI@Z
extern "C" int MS_ABI impl__OnMDIWindowCmd_CMDIFrameWnd__IEAAHI_Z(CMDIFrameWnd* pThis, UINT nID) {
    if (!pThis || !pThis->m_hWndMDIClient) return FALSE;
    if (nID == 0xF030) { pThis->MDINext(); return TRUE; }
    return FALSE;
}
// Symbol: ?OnMenuChar@CMDIFrameWnd@@IEAA_JIIPEAVCMenu@@@Z
extern "C" LRESULT MS_ABI impl__OnMenuChar_CMDIFrameWnd__IEAA_JIIPEAVCMenu___Z(
    CMDIFrameWnd* pThis, UINT nChar, UINT nFlags, CMenu* pMenu) {
    (void)pThis;
    (void)nChar;
    (void)nFlags;
    (void)pMenu;
    return MAKELRESULT(0, MNC_IGNORE);
}
// Symbol: ?OnSize@CMDIFrameWnd@@IEAAXIHH@Z
extern "C" void MS_ABI impl__OnSize_CMDIFrameWnd__IEAAXIHH_Z(CMDIFrameWnd* pThis, UINT nType, int cx, int cy) {
    (void)nType;
    if (pThis && pThis->m_hWndMDIClient) {
        ::MoveWindow(pThis->m_hWndMDIClient, 0, 0, cx, cy, TRUE);
    }
}
// Symbol: ?OnUpdateFrameMenu@CMDIFrameWnd@@UEAAXPEAUHMENU__@@@Z
extern "C" void MS_ABI impl__OnUpdateFrameMenu_CMDIFrameWnd__UEAAXPEAUHMENU_____Z(CMDIFrameWnd* pThis, HMENU hMenuAlt) {
    if (pThis) {
        pThis->CFrameWnd::OnUpdateFrameMenu(hMenuAlt);
    }
}
// Symbol: ?OnUpdateFrameTitle@CMDIFrameWnd@@UEAAXH@Z
extern "C" void MS_ABI impl__OnUpdateFrameTitle_CMDIFrameWnd__UEAAXH_Z(CMDIFrameWnd* pThis, int bAddToTitle) {
    if (pThis) {
        pThis->CFrameWnd::OnUpdateFrameTitle(bAddToTitle);
    }
}
// Symbol: ?OnUpdateMDIWindowCmd@CMDIFrameWnd@@IEAAXPEAVCCmdUI@@@Z
extern "C" void MS_ABI impl__OnUpdateMDIWindowCmd_CMDIFrameWnd__IEAAXPEAVCCmdUI___Z(CMDIFrameWnd* pThis, CCmdUI* pCmdUI) {
    if (!pThis || !pCmdUI) return;
    CWnd* pActive = pThis->MDIGetActive(nullptr);
    pCmdUI->Enable(pActive != nullptr);
}
// Symbol: ?OnWindowNew@CMDIFrameWnd@@IEAAXXZ
extern "C" void MS_ABI impl__OnWindowNew_CMDIFrameWnd__IEAAXXZ(CMDIFrameWnd* pThis) {
    if (!pThis) return;
    CWnd* pActive = pThis->MDIGetActive(nullptr);
    if (pActive) {
        pActive->SetFocus();
    } else if (pThis->m_hWndMDIClient) {
        pThis->MDICascade(0);
    }
}
// Symbol: ?PreCreateWindow@CMDIFrameWnd@@UEAAHAEAUtagCREATESTRUCTW@@@Z
extern "C" int MS_ABI impl__PreCreateWindow_CMDIFrameWnd__UEAAHAEAUtagCREATESTRUCTW___Z(
    CMDIFrameWnd* pThis, CREATESTRUCTW& cs) {
    return impl__PreCreateWindow_CFrameWnd__MEAAHAEAUtagCREATESTRUCTW___Z(pThis, cs);
}
// Symbol: ?PreTranslateMessage@CMDIFrameWnd@@UEAAHPEAUtagMSG@@@Z
extern "C" int MS_ABI impl__PreTranslateMessage_CMDIFrameWnd__UEAAHPEAUtagMSG___Z(CMDIFrameWnd* pThis, MSG* pMsg) {
    return impl__PreTranslateMessage_CFrameWnd__UEAAHPEAUtagMSG___Z(pThis, pMsg);
}
// Symbol: ?SetMenuBarState@CMDIFrameWnd@@UEAAHK@Z
extern "C" int MS_ABI impl__SetMenuBarState_CMDIFrameWnd__UEAAHK_Z(CMDIFrameWnd* pThis, DWORD dwState) {
    return pThis ? pThis->CFrameWnd::SetMenuBarState(dwState) : FALSE;
}
// Symbol: ?SetMenuBarVisibility@CMDIFrameWnd@@UEAAXK@Z
extern "C" void MS_ABI impl__SetMenuBarVisibility_CMDIFrameWnd__UEAAXK_Z(CMDIFrameWnd* pThis, DWORD dwStyle) {
    if (pThis) pThis->CFrameWnd::SetMenuBarVisibility(dwStyle);
}
CMDIFrameWnd::CMDIFrameWnd() : m_hWndMDIClient(nullptr) {
}
int CMDIFrameWnd::CreateClient(void* lpCreateStruct, CMenu* pWindowMenu) {
    (void)lpCreateStruct;

    if (!m_hWnd) return FALSE;

    CLIENTCREATESTRUCT ccs = {};
    ccs.hWindowMenu = pWindowMenu ? pWindowMenu->m_hMenu : nullptr;
    ccs.idFirstChild = 0xFF00;  // First MDI child ID

    m_hWndMDIClient = ::CreateWindowExW(
        0,
        L"MDICLIENT",
        nullptr,
        WS_CHILD | WS_CLIPCHILDREN | WS_VSCROLL | WS_HSCROLL | WS_VISIBLE,
        0, 0, 0, 0,
        m_hWnd,
        nullptr,
        AfxGetInstanceHandle(),
        &ccs
    );

    return m_hWndMDIClient != nullptr;
}
HWND CMDIFrameWnd::GetWindowMenuPopup(HMENU hMenuBar) {
    if (!hMenuBar) return nullptr;

    // Find the Window menu by looking for one with MDI child items
    int nCount = ::GetMenuItemCount(hMenuBar);
    for (int i = 0; i < nCount; i++) {
        HMENU hSubMenu = ::GetSubMenu(hMenuBar, i);
        if (hSubMenu) {
            // Check if this submenu has the tile/cascade commands
            if (::GetMenuState(hSubMenu, 0xFF00, MF_BYCOMMAND) != (UINT)-1) {
                return (HWND)(UINT_PTR)hSubMenu;
            }
        }
    }
    return nullptr;
}
void CMDIFrameWnd::MDIActivate(CWnd* pWndActivate) {
    if (m_hWndMDIClient && pWndActivate && pWndActivate->m_hWnd) {
        ::SendMessageW(m_hWndMDIClient, WM_MDIACTIVATE, (WPARAM)pWndActivate->m_hWnd, 0);
    }
}
CWnd* CMDIFrameWnd::MDIGetActive(int* pbMaximized) const {
    if (!m_hWndMDIClient) return nullptr;

    BOOL bMaximized = FALSE;
    HWND hWnd = (HWND)::SendMessageW(m_hWndMDIClient, WM_MDIGETACTIVE, 0, (LPARAM)&bMaximized);

    if (pbMaximized) {
        *pbMaximized = bMaximized ? 1 : 0;
    }

    return hWnd ? CWnd::FromHandle(hWnd) : nullptr;
}
void CMDIFrameWnd::MDIIconArrange() {
    if (m_hWndMDIClient) {
        ::SendMessageW(m_hWndMDIClient, WM_MDIICONARRANGE, 0, 0);
    }
}
void CMDIFrameWnd::MDIMaximize(CWnd* pWnd) {
    if (m_hWndMDIClient && pWnd && pWnd->m_hWnd) {
        ::SendMessageW(m_hWndMDIClient, WM_MDIMAXIMIZE, (WPARAM)pWnd->m_hWnd, 0);
    }
}
void CMDIFrameWnd::MDINext() {
    if (m_hWndMDIClient) {
        ::SendMessageW(m_hWndMDIClient, WM_MDINEXT, 0, 0);
    }
}
void CMDIFrameWnd::MDIRestore(CWnd* pWnd) {
    if (m_hWndMDIClient && pWnd && pWnd->m_hWnd) {
        ::SendMessageW(m_hWndMDIClient, WM_MDIRESTORE, (WPARAM)pWnd->m_hWnd, 0);
    }
}
void CMDIFrameWnd::MDISetMenu(CMenu* pFrameMenu, CMenu* pWindowMenu) {
    if (m_hWndMDIClient) {
        ::SendMessageW(m_hWndMDIClient, WM_MDISETMENU,
                       (WPARAM)(pFrameMenu ? pFrameMenu->m_hMenu : nullptr),
                       (LPARAM)(pWindowMenu ? pWindowMenu->m_hMenu : nullptr));
        ::DrawMenuBar(m_hWnd);
    }
}
void CMDIFrameWnd::MDITile(int nType) {
    if (m_hWndMDIClient) {
        ::SendMessageW(m_hWndMDIClient, WM_MDITILE, nType, 0);
    }
}
void CMDIFrameWnd::MDICascade(int nType) {
    if (m_hWndMDIClient) {
        ::SendMessageW(m_hWndMDIClient, WM_MDICASCADE, nType, 0);
    }
}
