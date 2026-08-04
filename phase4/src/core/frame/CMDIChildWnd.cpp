// CMDIChildWnd — OpenMFC implementation.
// Sources: frame_font_exports.cpp, wincore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/FrameFontExportsSupport.h"
#include "detail/WincoreSupport.h"

// Symbol: ?CreateObject@CMDIChildWnd@@SAPEAVCObject@@XZ
extern "C" CObject* MS_ABI impl__CreateObject_CMDIChildWnd__SAPEAVCObject__XZ() {
    return CMDIChildWnd::CreateObject();
}
// Symbol: ?DefWindowProcW@CMDIChildWnd@@MEAA_JI_K_J@Z
extern "C" LRESULT MS_ABI impl__DefWindowProcW_CMDIChildWnd__MEAA_JI_K_J_Z(
    CMDIChildWnd* pThis, UINT message, WPARAM wParam, LPARAM lParam) {
    return impl__DefWindowProcW_CWnd__MEAA_JI_K_J_Z(pThis, message, wParam, lParam);
}
// Symbol: ?GetMessageBar@CMDIChildWnd@@MEAAPEAVCWnd@@XZ
extern "C" CWnd* MS_ABI impl__GetMessageBar_CMDIChildWnd__MEAAPEAVCWnd__XZ(CMDIChildWnd* pThis) {
    return pThis ? pThis->CFrameWnd::GetMessageBar() : nullptr;
}
// Symbol: ?GetMessageMap@CMDIChildWnd@@MEBAPEBUAFX_MSGMAP@@XZ
extern "C" const AFX_MSGMAP* MS_ABI impl__GetMessageMap_CMDIChildWnd__MEBAPEBUAFX_MSGMAP__XZ(
    const CMDIChildWnd* pThis) {
    (void)pThis;
    return EmptyMessageMap_FrameFontExports();
}
// Symbol: ?GetRuntimeClass@CMDIChildWnd@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CMDIChildWnd__UEBAPEAUCRuntimeClass__XZ(
    const CMDIChildWnd* pThis) {
    return CMDIChildWnd::GetThisClass();
}
// Symbol: ?GetThisClass@CMDIChildWnd@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMDIChildWnd__SAPEAUCRuntimeClass__XZ() {
    return CMDIChildWnd::GetThisClass();
}
// Symbol: ?GetThisMessageMap@CMDIChildWnd@@KAPEBUAFX_MSGMAP@@XZ
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CMDIChildWnd__KAPEBUAFX_MSGMAP__XZ() {
    return EmptyMessageMap_FrameFontExports();
}
// Symbol: ?GetTrackingID@CMDIChildWnd@@UEAAIXZ
extern "C" UINT MS_ABI impl__GetTrackingID_CMDIChildWnd__UEAAIXZ(CMDIChildWnd* pThis) {
    return pThis && pThis->m_hWnd ? (UINT)::GetWindowLongPtrW(pThis->m_hWnd, GWLP_ID) : 0;
}
// Symbol: ?LoadFrame@CMDIChildWnd@@UEAAHIKPEAVCWnd@@PEAUCCreateContext@@@Z
extern "C" int MS_ABI impl__LoadFrame_CMDIChildWnd__UEAAHIKPEAVCWnd__PEAUCCreateContext___Z(
    CMDIChildWnd* pThis, UINT nIDResource, DWORD dwDefaultStyle, CWnd* pParentWnd, CCreateContext* pContext) {
    (void)nIDResource;
    CMDIFrameWnd* pFrame = dynamic_cast<CMDIFrameWnd*>(pParentWnd);
    RECT rect = {CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT};
    return pThis && pFrame ? pThis->Create(nullptr, L"", dwDefaultStyle, rect, pFrame, pContext) : FALSE;
}
// Symbol: ?OnCreate@CMDIChildWnd@@IEAAHPEAUtagCREATESTRUCTW@@@Z
extern "C" int MS_ABI impl__OnCreate_CMDIChildWnd__IEAAHPEAUtagCREATESTRUCTW___Z(
    CMDIChildWnd* pThis, CREATESTRUCTW* pCreateStruct) {
    (void)pThis;
    (void)pCreateStruct;
    return 0;
}
// Symbol: ?OnDestroy@CMDIChildWnd@@IEAAXXZ
extern "C" void MS_ABI impl__OnDestroy_CMDIChildWnd__IEAAXXZ(CMDIChildWnd* pThis) {
    if (pThis) pThis->m_hWnd = nullptr;
}
// Symbol: ?OnMDIActivate@CMDIChildWnd@@IEAAXHPEAVCWnd@@0@Z
extern "C" void MS_ABI impl__OnMDIActivate_CMDIChildWnd__IEAAXHPEAVCWnd__0_Z(
    CMDIChildWnd* pThis, int bActivate, CWnd* pActivateWnd, CWnd* pDeactivateWnd) {
    (void)pThis;
    (void)bActivate;
    (void)pActivateWnd;
    (void)pDeactivateWnd;
}
// Symbol: ?OnMouseActivate@CMDIChildWnd@@IEAAHPEAVCWnd@@II@Z
extern "C" int MS_ABI impl__OnMouseActivate_CMDIChildWnd__IEAAHPEAVCWnd__II_Z(
    CMDIChildWnd* pThis, CWnd* pDesktopWnd, UINT nHitTest, UINT message) {
    (void)pThis;
    (void)pDesktopWnd;
    (void)nHitTest;
    (void)message;
    return MA_ACTIVATE;
}
// Symbol: ?OnNcActivate@CMDIChildWnd@@IEAAHH@Z
extern "C" int MS_ABI impl__OnNcActivate_CMDIChildWnd__IEAAHH_Z(CMDIChildWnd* pThis, int bActive) {
    (void)pThis;
    (void)bActive;
    return TRUE;
}
// Symbol: ?OnNcCreate@CMDIChildWnd@@IEAAHPEAUtagCREATESTRUCTW@@@Z
extern "C" int MS_ABI impl__OnNcCreate_CMDIChildWnd__IEAAHPEAUtagCREATESTRUCTW___Z(
    CMDIChildWnd* pThis, CREATESTRUCTW* pCreateStruct) {
    (void)pThis;
    (void)pCreateStruct;
    return TRUE;
}
// Symbol: ?OnSize@CMDIChildWnd@@IEAAXIHH@Z
extern "C" void MS_ABI impl__OnSize_CMDIChildWnd__IEAAXIHH_Z(
    CMDIChildWnd* pThis, UINT nType, int cx, int cy) {
    (void)pThis;
    (void)nType;
    (void)cx;
    (void)cy;
}
// Symbol: ?OnToolTipText@CMDIChildWnd@@IEAAHIPEAUtagNMHDR@@PEA_J@Z
extern "C" int MS_ABI impl__OnToolTipText_CMDIChildWnd__IEAAHIPEAUtagNMHDR__PEA_J_Z(
    CMDIChildWnd* pThis, UINT id, NMHDR* pNMHDR, LRESULT* pResult) {
    (void)pThis;
    (void)id;
    (void)pNMHDR;
    if (pResult) *pResult = 0;
    return FALSE;
}
// Symbol: ?OnUpdateFrameMenu@CMDIChildWnd@@UEAAXHPEAVCWnd@@PEAUHMENU__@@@Z
extern "C" void MS_ABI impl__OnUpdateFrameMenu_CMDIChildWnd__UEAAXHPEAVCWnd__PEAUHMENU_____Z(
    CMDIChildWnd* pThis, int bActive, CWnd* pActivateWnd, HMENU hMenuAlt) {
    (void)bActive;
    (void)pActivateWnd;
    if (pThis) {
        pThis->CFrameWnd::OnUpdateFrameMenu(hMenuAlt);
    }
}
// Symbol: ?OnUpdateFrameTitle@CMDIChildWnd@@MEAAXH@Z
extern "C" void MS_ABI impl__OnUpdateFrameTitle_CMDIChildWnd__MEAAXH_Z(CMDIChildWnd* pThis, int bAddToTitle) {
    if (pThis) {
        pThis->CFrameWnd::OnUpdateFrameTitle(bAddToTitle);
    }
}
// Symbol: ?OnWindowPosChanging@CMDIChildWnd@@IEAAXPEAUtagWINDOWPOS@@@Z
extern "C" void MS_ABI impl__OnWindowPosChanging_CMDIChildWnd__IEAAXPEAUtagWINDOWPOS___Z(
    CMDIChildWnd* pThis, WINDOWPOS* lpWndPos) {
    if (pThis && lpWndPos) {
        if (lpWndPos->x < 0) lpWndPos->x = 0;
        if (lpWndPos->y < 0) lpWndPos->y = 0;
        if (lpWndPos->cx < 0) lpWndPos->cx = 0;
        if (lpWndPos->cy < 0) lpWndPos->cy = 0;
    }
}
// Symbol: ?PreCreateWindow@CMDIChildWnd@@UEAAHAEAUtagCREATESTRUCTW@@@Z
extern "C" int MS_ABI impl__PreCreateWindow_CMDIChildWnd__UEAAHAEAUtagCREATESTRUCTW___Z(
    CMDIChildWnd* pThis, CREATESTRUCTW& cs) {
    return impl__PreCreateWindow_CFrameWnd__MEAAHAEAUtagCREATESTRUCTW___Z(pThis, cs);
}
// Symbol: ?PreTranslateMessage@CMDIChildWnd@@UEAAHPEAUtagMSG@@@Z
extern "C" int MS_ABI impl__PreTranslateMessage_CMDIChildWnd__UEAAHPEAUtagMSG___Z(
    CMDIChildWnd* pThis, MSG* pMsg) {
    return impl__PreTranslateMessage_CFrameWnd__UEAAHPEAUtagMSG___Z(pThis, pMsg);
}
// Symbol: ?SetHandles@CMDIChildWnd@@QEAAXPEAUHMENU__@@PEAUHACCEL__@@@Z
extern "C" void MS_ABI impl__SetHandles_CMDIChildWnd__QEAAXPEAUHMENU____PEAUHACCEL_____Z(
    CMDIChildWnd* pThis, HMENU hMenu, HACCEL hAccel) {
    (void)pThis;
    (void)hMenu;
    (void)hAccel;
}
// Symbol: ?UpdateClientEdge@CMDIChildWnd@@IEAAHPEAUtagRECT@@@Z
extern "C" int MS_ABI impl__UpdateClientEdge_CMDIChildWnd__IEAAHPEAUtagRECT___Z(
    CMDIChildWnd* pThis, RECT* lpRect) {
    (void)pThis;
    (void)lpRect;
    return FALSE;
}
CMDIChildWnd::CMDIChildWnd() {
    memset(_mdichild_padding, 0, sizeof(_mdichild_padding));
}
int CMDIChildWnd::Create(const wchar_t* lpszClassName, const wchar_t* lpszWindowName,
                         DWORD dwStyle, const struct tagRECT& rect,
                         CMDIFrameWnd* pParentWnd, CCreateContext* pContext) {
    (void)lpszClassName;
    (void)pContext;

    if (!pParentWnd || !pParentWnd->m_hWndMDIClient) {
        return FALSE;
    }

    MDICREATESTRUCTW mcs = {};
    mcs.szClass = lpszClassName ? lpszClassName : L"MDICHILD";
    mcs.szTitle = lpszWindowName;
    mcs.hOwner = AfxGetInstanceHandle();
    mcs.x = rect.left ? rect.left : CW_USEDEFAULT;
    mcs.y = rect.top ? rect.top : CW_USEDEFAULT;
    mcs.cx = (rect.right - rect.left) ? (rect.right - rect.left) : CW_USEDEFAULT;
    mcs.cy = (rect.bottom - rect.top) ? (rect.bottom - rect.top) : CW_USEDEFAULT;
    mcs.style = dwStyle ? dwStyle : (WS_CHILD | WS_VISIBLE | WS_OVERLAPPEDWINDOW);
    mcs.lParam = (LPARAM)this;

    m_hWnd = (HWND)::SendMessageW(pParentWnd->m_hWndMDIClient, WM_MDICREATE, 0, (LPARAM)&mcs);

    if (m_hWnd) {
        g_hwndMap[m_hWnd] = this;
        return TRUE;
    }

    return FALSE;
}
void CMDIChildWnd::ActivateFrame(int nCmdShow) {
    CMDIFrameWnd* pFrame = GetMDIFrame();
    if (pFrame) {
        pFrame->MDIActivate(this);
        if (nCmdShow != -1) {
            ::ShowWindow(m_hWnd, nCmdShow);
        }
    }
}
int CMDIChildWnd::DestroyWindow() {
    CMDIFrameWnd* pFrame = GetMDIFrame();
    if (pFrame && pFrame->m_hWndMDIClient && m_hWnd) {
        ::SendMessageW(pFrame->m_hWndMDIClient, WM_MDIDESTROY, (WPARAM)m_hWnd, 0);
        m_hWnd = nullptr;
        return TRUE;
    }
    return FALSE;
}
CMDIFrameWnd* CMDIChildWnd::GetMDIFrame() {
    HWND hWndParent = ::GetParent(m_hWnd);  // MDI client
    if (hWndParent) {
        hWndParent = ::GetParent(hWndParent);  // MDI frame
        CWnd* pWnd = CWnd::FromHandle(hWndParent);
        return dynamic_cast<CMDIFrameWnd*>(pWnd);
    }
    return nullptr;
}
