// CToolTipCtrl exported wrappers.

#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxole.h"
#include <windows.h>
#include <commctrl.h>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

class CToolInfo;

namespace {
static HWND ToolTipHwnd(const CToolTipCtrl* pThis) {
    return pThis ? pThis->m_hWnd : nullptr;
}

static HWND ToolWindow(CWnd* pWnd) {
    return pWnd ? pWnd->GetSafeHwnd() : nullptr;
}

static UINT ToolFlags(UINT_PTR nIDTool) {
    return nIDTool == 0 ? (TTF_SUBCLASS | TTF_IDISHWND) : TTF_SUBCLASS;
}

static UINT_PTR ToolId(CWnd* pWnd, UINT_PTR nIDTool) {
    return nIDTool == 0 ? (UINT_PTR)ToolWindow(pWnd) : nIDTool;
}

static void FillToolInfo(TOOLINFOW* pInfo, CWnd* pWnd, UINT_PTR nIDTool) {
    if (!pInfo) return;
    pInfo->cbSize = sizeof(TOOLINFOW);
    pInfo->hwnd = ToolWindow(pWnd);
    pInfo->uId = ToolId(pWnd, nIDTool);
    pInfo->uFlags = ToolFlags(nIDTool);
}

static CRuntimeClass g_classCToolTipCtrl = {
    "CToolTipCtrl",
    sizeof(CToolTipCtrl),
    0xFFFF,
    nullptr,
    CWnd::GetThisClass,
    CWnd::GetThisClass(),
    nullptr
};
}  // namespace

// Symbol: ?CreateEx@CToolTipCtrl@@UEAAHPEAVCWnd@@KK@Z
extern "C" int MS_ABI impl__CreateEx_CToolTipCtrl__UEAAHPEAVCWnd__KK_Z(
    CToolTipCtrl* pThis, CWnd* pParentWnd, DWORD dwStyle, DWORD dwExStyle) {
    if (!pThis || !pParentWnd) return FALSE;
    pThis->m_hWnd = ::CreateWindowExW(dwExStyle, TOOLTIPS_CLASSW, nullptr,
        dwStyle | WS_POPUP | TTS_NOPREFIX | TTS_ALWAYSTIP,
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
        pParentWnd->GetSafeHwnd(), nullptr, AfxGetInstanceHandle(), nullptr);
    if (pThis->m_hWnd) {
        ::SetWindowPos(pThis->m_hWnd, HWND_TOPMOST, 0, 0, 0, 0,
            SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
    }
    return pThis->m_hWnd != nullptr;
}

// Symbol: ?DelTool@CToolTipCtrl@@QEAAXPEAVCWnd@@_K@Z
extern "C" void MS_ABI impl__DelTool_CToolTipCtrl__QEAAXPEAVCWnd___K_Z(
    CToolTipCtrl* pThis, CWnd* pWnd, UINT_PTR nIDTool) {
    TOOLINFOW info = {};
    FillToolInfo(&info, pWnd, nIDTool);
    ::SendMessageW(ToolTipHwnd(pThis), TTM_DELTOOLW, 0, (LPARAM)&info);
}

// Symbol: ?DestroyToolTipCtrl@CToolTipCtrl@@QEAAHXZ
extern "C" int MS_ABI impl__DestroyToolTipCtrl_CToolTipCtrl__QEAAHXZ(CToolTipCtrl* pThis) {
    if (!pThis || !pThis->m_hWnd) return FALSE;
    HWND hWnd = pThis->m_hWnd;
    pThis->m_hWnd = nullptr;
    return ::DestroyWindow(hWnd);
}

// Symbol: ?FillInToolInfo@CToolTipCtrl@@QEBAXAEAUtagTOOLINFOW@@PEAVCWnd@@_K@Z
extern "C" void MS_ABI impl__FillInToolInfo_CToolTipCtrl__QEBAXAEAUtagTOOLINFOW__PEAVCWnd___K_Z(
    const CToolTipCtrl* pThis, TOOLINFOW* pInfo, CWnd* pWnd, UINT_PTR nIDTool) {
    (void)pThis;
    if (!pInfo) return;
    *pInfo = {};
    FillToolInfo(pInfo, pWnd, nIDTool);
}

// Symbol: ?GetMessageMap@CToolTipCtrl@@MEBAPEBUAFX_MSGMAP@@XZ
extern "C" const AFX_MSGMAP* MS_ABI impl__GetMessageMap_CToolTipCtrl__MEBAPEBUAFX_MSGMAP__XZ(
    const CToolTipCtrl* pThis) {
    (void)pThis;
    return CWnd::GetThisMessageMap();
}

// Symbol: ?GetRuntimeClass@CToolTipCtrl@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CToolTipCtrl__UEBAPEAUCRuntimeClass__XZ(
    const CToolTipCtrl* pThis) {
    (void)pThis;
    return &g_classCToolTipCtrl;
}

// Symbol: ?GetThisClass@CToolTipCtrl@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CToolTipCtrl__SAPEAUCRuntimeClass__XZ() {
    return &g_classCToolTipCtrl;
}

// Symbol: ?GetThisMessageMap@CToolTipCtrl@@KAPEBUAFX_MSGMAP@@XZ
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CToolTipCtrl__KAPEBUAFX_MSGMAP__XZ() {
    return CWnd::GetThisMessageMap();
}

// Symbol: ?GetToolInfo@CToolTipCtrl@@QEBAHAEAVCToolInfo@@PEAVCWnd@@_K@Z
extern "C" int MS_ABI impl__GetToolInfo_CToolTipCtrl__QEBAHAEAVCToolInfo__PEAVCWnd___K_Z(
    const CToolTipCtrl* pThis, CToolInfo* pInfo, CWnd* pWnd, UINT_PTR nIDTool) {
    if (!pInfo) return FALSE;
    TOOLINFOW* pToolInfo = reinterpret_cast<TOOLINFOW*>(pInfo);
    *pToolInfo = {};
    FillToolInfo(pToolInfo, pWnd, nIDTool);
    return (int)::SendMessageW(ToolTipHwnd(pThis), TTM_GETTOOLINFOW, 0, (LPARAM)pToolInfo);
}

// Symbol: ?HitTest@CToolTipCtrl@@QEBAHPEAVCWnd@@VCPoint@@PEAUtagTOOLINFOW@@@Z
extern "C" int MS_ABI impl__HitTest_CToolTipCtrl__QEBAHPEAVCWnd__VCPoint__PEAUtagTOOLINFOW___Z(
    const CToolTipCtrl* pThis, CWnd* pWnd, CPoint pt, TOOLINFOW* pInfo) {
    TTHITTESTINFOW hit = {};
    hit.hwnd = ToolWindow(pWnd);
    hit.pt.x = pt.x;
    hit.pt.y = pt.y;
    if (pInfo) hit.ti = *pInfo;
    hit.ti.cbSize = sizeof(TOOLINFOW);
    int result = (int)::SendMessageW(ToolTipHwnd(pThis), TTM_HITTESTW, 0, (LPARAM)&hit);
    if (pInfo) *pInfo = hit.ti;
    return result;
}

// Symbol: ?OnAddTool@CToolTipCtrl@@IEAA_J_K_J@Z
extern "C" LRESULT MS_ABI impl__OnAddTool_CToolTipCtrl__IEAA_J_K_J_Z(
    CToolTipCtrl* pThis, WPARAM wParam, LPARAM lParam) {
    (void)pThis;
    (void)wParam;
    (void)lParam;
    return 0;
}

// Symbol: ?OnDisableModal@CToolTipCtrl@@IEAA_J_K_J@Z
extern "C" LRESULT MS_ABI impl__OnDisableModal_CToolTipCtrl__IEAA_J_K_J_Z(
    CToolTipCtrl* pThis, WPARAM wParam, LPARAM lParam) {
    (void)pThis;
    (void)wParam;
    (void)lParam;
    return 0;
}

// Symbol: ?OnEnable@CToolTipCtrl@@IEAAXH@Z
extern "C" void MS_ABI impl__OnEnable_CToolTipCtrl__IEAAXH_Z(CToolTipCtrl* pThis, int bEnable) {
    if (pThis) pThis->Activate(bEnable ? TRUE : FALSE);
}

// Symbol: ?OnWindowFromPoint@CToolTipCtrl@@IEAA_J_K_J@Z
extern "C" LRESULT MS_ABI impl__OnWindowFromPoint_CToolTipCtrl__IEAA_J_K_J_Z(
    CToolTipCtrl* pThis, WPARAM wParam, LPARAM lParam) {
    (void)pThis;
    (void)wParam;
    (void)lParam;
    return 0;
}

// Symbol: ?SetToolRect@CToolTipCtrl@@QEAAXPEAVCWnd@@_KPEBUtagRECT@@@Z
extern "C" void MS_ABI impl__SetToolRect_CToolTipCtrl__QEAAXPEAVCWnd___KPEBUtagRECT___Z(
    CToolTipCtrl* pThis, CWnd* pWnd, UINT_PTR nIDTool, const RECT* pRect) {
    TOOLINFOW info = {};
    FillToolInfo(&info, pWnd, nIDTool);
    if (pRect) info.rect = *pRect;
    ::SendMessageW(ToolTipHwnd(pThis), TTM_NEWTOOLRECTW, 0, (LPARAM)&info);
}
