// Frame-window and font-dialog exported wrappers for remaining common-control shard symbols.

#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxwin.h"
#include <windows.h>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

extern "C" LRESULT MS_ABI impl__DefWindowProcW_CWnd__MEAA_JI_K_J_Z(
    CWnd* pThis, UINT message, WPARAM wParam, LPARAM lParam);
extern "C" int MS_ABI impl__LoadFrame_CFrameWnd__UEAAHIKPEAVCWnd__PEAUCCreateContext___Z(
    CFrameWnd* pThis, UINT nIDResource, DWORD dwDefaultStyle, CWnd* pParentWnd, CCreateContext* pContext);
extern "C" int MS_ABI impl__OnCmdMsg_CFrameWnd__UEAAHIHPEAXPEAUAFX_CMDHANDLERINFO___Z(
    CFrameWnd* pThis, UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
extern "C" int MS_ABI impl__OnCommand_CFrameWnd__MEAAH_K_J_Z(
    CFrameWnd* pThis, WPARAM wParam, LPARAM lParam);
extern "C" int MS_ABI impl__PreCreateWindow_CFrameWnd__MEAAHAEAUtagCREATESTRUCTW___Z(
    CFrameWnd* pThis, CREATESTRUCTW& cs);
extern "C" int MS_ABI impl__PreTranslateMessage_CFrameWnd__UEAAHPEAUtagMSG___Z(
    CFrameWnd* pThis, MSG* pMsg);

namespace {
struct CFontDialogAccess : CFontDialog {
    using CFontDialog::m_clrResult;
    using CFontDialog::m_lf;
    using CFontDialog::m_nPointSize;
};

static LOGFONTW* FontLog(CFontDialog* pThis) {
    return pThis ? reinterpret_cast<LOGFONTW*>(static_cast<CFontDialogAccess*>(pThis)->m_lf) : nullptr;
}

static const LOGFONTW* FontLog(const CFontDialog* pThis) {
    return pThis ? reinterpret_cast<const LOGFONTW*>(static_cast<const CFontDialogAccess*>(pThis)->m_lf) : nullptr;
}

static const AFX_MSGMAP* EmptyMessageMap() {
    return CWnd::GetThisMessageMap();
}
}  // namespace

// Symbol: ?CreateObject@CFrameWnd@@SAPEAVCObject@@XZ
extern "C" CObject* MS_ABI impl__CreateObject_CFrameWnd__SAPEAVCObject__XZ() {
    return CFrameWnd::CreateObject();
}

// Symbol: ?GetMessageMap@CFrameWnd@@MEBAPEBUAFX_MSGMAP@@XZ
extern "C" const AFX_MSGMAP* MS_ABI impl__GetMessageMap_CFrameWnd__MEBAPEBUAFX_MSGMAP__XZ(
    const CFrameWnd* pThis) {
    (void)pThis;
    return EmptyMessageMap();
}

// Symbol: ?GetRuntimeClass@CFrameWnd@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CFrameWnd__UEBAPEAUCRuntimeClass__XZ(
    const CFrameWnd* pThis) {
    return CFrameWnd::GetThisClass();
}

// Symbol: ?GetThisMessageMap@CFrameWnd@@KAPEBUAFX_MSGMAP@@XZ
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CFrameWnd__KAPEBUAFX_MSGMAP__XZ() {
    return EmptyMessageMap();
}

// Symbol: ?rectDefault@CFrameWnd@@2VCRect@@B
extern "C" const CRect MS_ABI impl__rectDefault_CFrameWnd__2VCRect__B = {};

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
    return EmptyMessageMap();
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
    return EmptyMessageMap();
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
    (void)pThis;
    (void)bActive;
    (void)pActivateWnd;
    (void)hMenuAlt;
}

// Symbol: ?OnUpdateFrameTitle@CMDIChildWnd@@MEAAXH@Z
extern "C" void MS_ABI impl__OnUpdateFrameTitle_CMDIChildWnd__MEAAXH_Z(CMDIChildWnd* pThis, int bAddToTitle) {
    (void)pThis;
    (void)bAddToTitle;
}

// Symbol: ?OnWindowPosChanging@CMDIChildWnd@@IEAAXPEAUtagWINDOWPOS@@@Z
extern "C" void MS_ABI impl__OnWindowPosChanging_CMDIChildWnd__IEAAXPEAUtagWINDOWPOS___Z(
    CMDIChildWnd* pThis, WINDOWPOS* lpWndPos) {
    (void)pThis;
    (void)lpWndPos;
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
    (void)pThis;
    (void)hMenuAlt;
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
    return EmptyMessageMap();
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
    return EmptyMessageMap();
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
    (void)pThis;
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
    (void)pThis;
    (void)hMenuAlt;
}

// Symbol: ?OnUpdateFrameTitle@CMDIFrameWnd@@UEAAXH@Z
extern "C" void MS_ABI impl__OnUpdateFrameTitle_CMDIFrameWnd__UEAAXH_Z(CMDIFrameWnd* pThis, int bAddToTitle) {
    (void)pThis;
    (void)bAddToTitle;
}

// Symbol: ?OnUpdateMDIWindowCmd@CMDIFrameWnd@@IEAAXPEAVCCmdUI@@@Z
extern "C" void MS_ABI impl__OnUpdateMDIWindowCmd_CMDIFrameWnd__IEAAXPEAVCCmdUI___Z(CMDIFrameWnd* pThis, CCmdUI* pCmdUI) {
    (void)pThis;
    (void)pCmdUI;
}

// Symbol: ?OnWindowNew@CMDIFrameWnd@@IEAAXXZ
extern "C" void MS_ABI impl__OnWindowNew_CMDIFrameWnd__IEAAXXZ(CMDIFrameWnd* pThis) {
    (void)pThis;
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

// Symbol: ?FillInLogFont@CFontDialog@@QEAAKAEBU_charformatw@@@Z
extern "C" DWORD MS_ABI impl__FillInLogFont_CFontDialog__QEAAKAEBU_charformatw___Z(
    CFontDialog* pThis, const CHARFORMATW* pCharFormat) {
    LOGFONTW* lf = FontLog(pThis);
    if (!lf || !pCharFormat) return 0;

    lf->lfWeight = (pCharFormat->dwEffects & CFE_BOLD) ? FW_BOLD : FW_NORMAL;
    lf->lfItalic = (BYTE)((pCharFormat->dwEffects & CFE_ITALIC) != 0);
    lf->lfUnderline = (BYTE)((pCharFormat->dwEffects & CFE_UNDERLINE) != 0);
    lf->lfStrikeOut = (BYTE)((pCharFormat->dwEffects & CFE_STRIKEOUT) != 0);
    lf->lfCharSet = pCharFormat->bCharSet;
    lf->lfPitchAndFamily = pCharFormat->bPitchAndFamily;
    if (pCharFormat->yHeight > 0) {
        lf->lfHeight = -MulDiv(pCharFormat->yHeight, 1, 20);
        static_cast<CFontDialogAccess*>(pThis)->m_nPointSize = (int)(pCharFormat->yHeight / 20);
    }
    lstrcpynW(lf->lfFaceName, pCharFormat->szFaceName, LF_FACESIZE);
    static_cast<CFontDialogAccess*>(pThis)->m_clrResult = pCharFormat->crTextColor;
    return pCharFormat->dwMask;
}

// Symbol: ?GetCharFormat@CFontDialog@@QEBAXAEAU_charformatw@@@Z
extern "C" void MS_ABI impl__GetCharFormat_CFontDialog__QEBAXAEAU_charformatw___Z(
    const CFontDialog* pThis, CHARFORMATW* pCharFormat) {
    if (!pCharFormat) return;
    const LOGFONTW* lf = FontLog(pThis);
    ZeroMemory(pCharFormat, sizeof(*pCharFormat));
    pCharFormat->cbSize = sizeof(*pCharFormat);
    if (!lf) return;

    pCharFormat->dwMask = CFM_FACE | CFM_SIZE | CFM_BOLD | CFM_ITALIC |
                          CFM_UNDERLINE | CFM_STRIKEOUT | CFM_COLOR;
    pCharFormat->dwEffects = 0;
    if (lf->lfWeight >= FW_BOLD) pCharFormat->dwEffects |= CFE_BOLD;
    if (lf->lfItalic) pCharFormat->dwEffects |= CFE_ITALIC;
    if (lf->lfUnderline) pCharFormat->dwEffects |= CFE_UNDERLINE;
    if (lf->lfStrikeOut) pCharFormat->dwEffects |= CFE_STRIKEOUT;
    pCharFormat->yHeight = (LONG)(static_cast<const CFontDialogAccess*>(pThis)->m_nPointSize * 20);
    pCharFormat->crTextColor = static_cast<const CFontDialogAccess*>(pThis)->m_clrResult;
    pCharFormat->bCharSet = lf->lfCharSet;
    pCharFormat->bPitchAndFamily = lf->lfPitchAndFamily;
    lstrcpynW(pCharFormat->szFaceName, lf->lfFaceName, LF_FACESIZE);
}

// Symbol: ?GetRuntimeClass@CFontDialog@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CFontDialog__UEBAPEAUCRuntimeClass__XZ(
    const CFontDialog* pThis) {
    return CFontDialog::GetThisClass();
}

// Symbol: ?GetThisClass@CFontDialog@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CFontDialog__SAPEAUCRuntimeClass__XZ() {
    return CFontDialog::GetThisClass();
}
