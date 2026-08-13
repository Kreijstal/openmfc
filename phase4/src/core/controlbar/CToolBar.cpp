// CToolBar — OpenMFC implementation.
// Sources: cbarcore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/CbarcoreSupport.h"

// Symbol: ?CalcDynamicLayout@CToolBar@@UEAA?AVCSize@@HK@Z
extern "C" void MS_ABI impl__CalcDynamicLayout_CToolBar__UEAA_AVCSize__HK_Z(
    CSize* pRet, CToolBar* pThis, int nLength, unsigned long dwMode) {
    (void)dwMode;
    CSize size = ToolbarDefaultSize(pThis);
    if (nLength > 0) size.cx = nLength;
    BuildCSizeResult(pRet, size.cx, size.cy);
}
// Symbol: ?CalcFixedLayout@CToolBar@@UEAA?AVCSize@@HH@Z
extern "C" void MS_ABI impl__CalcFixedLayout_CToolBar__UEAA_AVCSize__HH_Z(
    CSize* pRet, CToolBar* pThis, int bStretch, int bHorz) {
    CSize size = ToolbarDefaultSize(pThis);
    if (bStretch && pThis && pThis->GetSafeHwnd()) {
        RECT rc = {};
        if (::GetClientRect(::GetParent(pThis->GetSafeHwnd()), &rc)) {
            if (bHorz) size.cx = rc.right - rc.left;
            else size.cy = rc.bottom - rc.top;
        }
    }
    BuildCSizeResult(pRet, size.cx, size.cy);
}
// Symbol: ?CalcLayout@CToolBar@@IEAA?AVCSize@@KH@Z
extern "C" void MS_ABI impl__CalcLayout_CToolBar__IEAA_AVCSize__KH_Z(
    CSize* pRet, CToolBar* pThis, unsigned long dwMode, int nLength) {
    impl__CalcDynamicLayout_CToolBar__UEAA_AVCSize__HK_Z(pRet, pThis, nLength, dwMode);
}
// Symbol: ?CalcSize@CToolBar@@IEAA?AVCSize@@PEAU_TBBUTTON@@H@Z
extern "C" void MS_ABI impl__CalcSize_CToolBar__IEAA_AVCSize__PEAU_TBBUTTON__H_Z(
    CSize* pRet, CToolBar* pThis, TBBUTTON* pData, int nCount) {
    (void)pData;
    CSize size = ToolbarDefaultSize(pThis, nCount);
    BuildCSizeResult(pRet, size.cx, size.cy);
}
// Symbol: ?get_accName@CToolBar@@UEAAJUtagVARIANT@@PEAPEA_W@Z
extern "C" long MS_ABI impl__get_accName_CToolBar__UEAAJUtagVARIANT__PEAPEA_W_Z(
    CToolBar* pThis, VARIANT varChild, wchar_t** pszName) {
    if (!pszName) return E_POINTER;
    *pszName = nullptr;
    int index = (varChild.vt == VT_I4) ? (int)varChild.lVal - 1 : -1;
    CString text = pThis && index >= 0 ? pThis->GetButtonText(index) : CString(L"Toolbar");
    *pszName = ::SysAllocString((const wchar_t*)text);
    return *pszName ? S_OK : E_OUTOFMEMORY;
}
// Symbol: ?GetMessageMap@CToolBar@@MEBAPEBUAFX_MSGMAP@@XZ
extern "C" const AFX_MSGMAP* MS_ABI impl__GetMessageMap_CToolBar__MEBAPEBUAFX_MSGMAP__XZ(const CToolBar* pThis) {
    (void)pThis;
    return CWnd::GetThisMessageMap();
}
// Symbol: ?GetRuntimeClass@CToolBar@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CToolBar__UEBAPEAUCRuntimeClass__XZ(const CToolBar* pThis) {
    return pThis ? pThis->GetRuntimeClass() : CToolBar::GetThisClass();
}
// Symbol: ?GetThisClass@CToolBar@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CToolBar__SAPEAUCRuntimeClass__XZ() {
    return CToolBar::GetThisClass();
}
// Symbol: ?GetThisMessageMap@CToolBar@@KAPEBUAFX_MSGMAP@@XZ
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CToolBar__KAPEBUAFX_MSGMAP__XZ() {
    return CWnd::GetThisMessageMap();
}
// Symbol: ?Layout@CToolBar@@IEAAXXZ
extern "C" void MS_ABI impl__Layout_CToolBar__IEAAXXZ(CToolBar* pThis) {
    if (pThis && pThis->GetSafeHwnd()) {
        ::SendMessageW(pThis->GetSafeHwnd(), TB_AUTOSIZE, 0, 0);
    }
}
// Symbol: ?OnBarStyleChange@CToolBar@@UEAAXKK@Z
extern "C" void MS_ABI impl__OnBarStyleChange_CToolBar__UEAAXKK_Z(CToolBar* pThis, unsigned long oldStyle, unsigned long newStyle) {
    (void)oldStyle;
    if (pThis) pThis->SetBarStyle(newStyle);
}
// Symbol: ?OnEraseBkgnd@CToolBar@@IEAAHPEAVCDC@@@Z
extern "C" int MS_ABI impl__OnEraseBkgnd_CToolBar__IEAAHPEAVCDC___Z(CToolBar* pThis, CDC* pDC) {
    if (!pThis || !pThis->GetSafeHwnd() || !pDC || !pDC->GetSafeHdc()) return FALSE;
    return static_cast<int>(::DefWindowProcW(
        pThis->GetSafeHwnd(), WM_ERASEBKGND,
        reinterpret_cast<WPARAM>(pDC->GetSafeHdc()), 0));
}
// Symbol: ?OnNcCalcSize@CToolBar@@IEAAXHPEAUtagNCCALCSIZE_PARAMS@@@Z
extern "C" void MS_ABI impl__OnNcCalcSize_CToolBar__IEAAXHPEAUtagNCCALCSIZE_PARAMS___Z(
    CToolBar* pThis, int bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp) {
    if (!pThis || !pThis->GetSafeHwnd() || !lpncsp) return;
    ::DefWindowProcW(pThis->GetSafeHwnd(), WM_NCCALCSIZE,
                     static_cast<WPARAM>(bCalcValidRects != FALSE),
                     reinterpret_cast<LPARAM>(lpncsp));
}
// Symbol: ?OnNcCreate@CToolBar@@IEAAHPEAUtagCREATESTRUCTW@@@Z
extern "C" int MS_ABI impl__OnNcCreate_CToolBar__IEAAHPEAUtagCREATESTRUCTW___Z(CToolBar* pThis, CREATESTRUCTW* lpCreateStruct) {
    if (!pThis || !pThis->GetSafeHwnd() || !lpCreateStruct) return FALSE;
    return static_cast<int>(::DefWindowProcW(
        pThis->GetSafeHwnd(), WM_NCCREATE, 0,
        reinterpret_cast<LPARAM>(lpCreateStruct)) != FALSE);
}
// Symbol: ?OnNcHitTest@CToolBar@@IEAA_JVCPoint@@@Z
extern "C" __int64 MS_ABI impl__OnNcHitTest_CToolBar__IEAA_JVCPoint___Z(CToolBar* pThis, CPoint point) {
    return pThis && pThis->GetSafeHwnd() ? ::DefWindowProcW(pThis->GetSafeHwnd(), WM_NCHITTEST, 0, MAKELPARAM(point.x, point.y)) : HTCLIENT;
}
// Symbol: ?OnPreserveSizingPolicyHelper@CToolBar@@IEAA_J_K_J@Z
extern "C" __int64 MS_ABI impl__OnPreserveSizingPolicyHelper_CToolBar__IEAA_J_K_J_Z(CToolBar* pThis, unsigned __int64, __int64) {
    return pThis ? pThis->GetBarStyle() : 0;
}
// Symbol: ?OnPreserveZeroBorderHelper@CToolBar@@IEAA_J_K_J@Z
extern "C" __int64 MS_ABI impl__OnPreserveZeroBorderHelper_CToolBar__IEAA_J_K_J_Z(
    CToolBar* pThis, unsigned __int64, __int64) {
    return pThis ? pThis->GetBarStyle() : 0;
}
// Symbol: ?OnSetButtonSize@CToolBar@@IEAA_J_K_J@Z
extern "C" __int64 MS_ABI impl__OnSetButtonSize_CToolBar__IEAA_J_K_J_Z(CToolBar* pThis, unsigned __int64, __int64 lParam) {
    if (pThis) {
        SIZE sz = { LOWORD(lParam), HIWORD(lParam) };
        pThis->SetSizes(sz, pThis->m_sizeImage);
    }
    return TRUE;
}
// Symbol: ?OnSetSizeHelper@CToolBar@@IEAA_JAEAVCSize@@_J@Z
extern "C" __int64 MS_ABI impl__OnSetSizeHelper_CToolBar__IEAA_JAEAVCSize___J_Z(CToolBar* pThis, CSize* pSize, __int64) {
    if (pThis && pSize) pThis->m_sizeButton = *pSize;
    return TRUE;
}
// Symbol: ?OnSysColorChange@CToolBar@@IEAAXXZ
extern "C" void MS_ABI impl__OnSysColorChange_CToolBar__IEAAXXZ(CToolBar* pThis) {
    if (pThis && pThis->GetSafeHwnd()) ::InvalidateRect(pThis->GetSafeHwnd(), nullptr, TRUE);
}
// Symbol: ?OnToolHitTest@CToolBar@@UEBA_JVCPoint@@PEAUtagTOOLINFOW@@@Z
extern "C" __int64 MS_ABI impl__OnToolHitTest_CToolBar__UEBA_JVCPoint__PEAUtagTOOLINFOW___Z(
    const CToolBar* pThis, CPoint point, TOOLINFOW* pTI) {
    if (!pThis || !pThis->GetSafeHwnd()) return -1;
    int count = (int)::SendMessageW(pThis->GetSafeHwnd(), TB_BUTTONCOUNT, 0, 0);
    for (int i = 0; i < count; ++i) {
        RECT rc = {};
        if (::SendMessageW(pThis->GetSafeHwnd(), TB_GETITEMRECT, i, (LPARAM)&rc) && ::PtInRect(&rc, POINT{point.x, point.y})) {
            if (pTI) {
                pTI->hwnd = pThis->GetSafeHwnd();
                pTI->uId = (UINT_PTR)pThis->GetItemID(i);
                pTI->rect = rc;
            }
            return i;
        }
    }
    return -1;
}
// Symbol: ?OnUpdateCmdUI@CToolBar@@UEAAXPEAVCFrameWnd@@H@Z
extern "C" void MS_ABI impl__OnUpdateCmdUI_CToolBar__UEAAXPEAVCFrameWnd__H_Z(CToolBar* pThis, CFrameWnd* pTarget, int bDisableIfNoHndler) {
    (void)pThis;
    (void)pTarget;
    (void)bDisableIfNoHndler;
}
// Symbol: ?OnWindowPosChanging@CToolBar@@IEAAXPEAUtagWINDOWPOS@@@Z
extern "C" void MS_ABI impl__OnWindowPosChanging_CToolBar__IEAAXPEAUtagWINDOWPOS___Z(CToolBar* pThis, WINDOWPOS* lpWndPos) {
    if (!pThis || !pThis->GetSafeHwnd() || !lpWndPos) return;
    ::DefWindowProcW(pThis->GetSafeHwnd(), WM_WINDOWPOSCHANGING, 0,
                     reinterpret_cast<LPARAM>(lpWndPos));
}
// Symbol: ?SetOwner@CToolBar@@QEAAXPEAVCWnd@@@Z
extern "C" void MS_ABI impl__SetOwner_CToolBar__QEAAXPEAVCWnd___Z(CToolBar* pThis, CWnd* pOwner) {
    if (pThis && pThis->GetSafeHwnd()) {
        ::SetWindowLongPtrW(pThis->GetSafeHwnd(), GWLP_HWNDPARENT, (LONG_PTR)(pOwner ? pOwner->GetSafeHwnd() : nullptr));
    }
}
// Symbol: ?SizeToolBar@CToolBar@@IEAAXPEAU_TBBUTTON@@HHH@Z
extern "C" void MS_ABI impl__SizeToolBar_CToolBar__IEAAXPEAU_TBBUTTON__HHH_Z(
    CToolBar* pThis, TBBUTTON* pData, int nCount, int nLength, int bVert) {
    (void)pData;
    if (!pThis) return;
    CSize sz = ToolbarDefaultSize(pThis, nCount);
    if (nLength > 0) {
        if (bVert) sz.cy = nLength;
        else sz.cx = nLength;
    }
    pThis->m_sizeButton = sz;
}
// Symbol: ?WrapToolBar@CToolBar@@IEAAHPEAU_TBBUTTON@@HH@Z
extern "C" int MS_ABI impl__WrapToolBar_CToolBar__IEAAHPEAU_TBBUTTON__HH_Z(
    CToolBar* pThis, TBBUTTON* pData, int nCount, int nWidth) {
    (void)pThis;
    if (!pData || nCount <= 0 || nWidth <= 0) return 0;
    int rows = 1;
    int x = 0;
    for (int i = 0; i < nCount; ++i) {
        x += 23;
        if (x > nWidth) {
            pData[i].fsState |= TBSTATE_WRAP;
            x = 23;
            ++rows;
        }
    }
    return rows;
}
// Symbol: ?GetButtonText@CToolBar@@QEBA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@H@Z
extern "C" void MS_ABI impl__GetButtonText_CToolBar__QEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__H_Z(
    CString* pRet, const CToolBar* pThis, int nIndex) {
    new (pRet) CString(pThis->GetButtonText(nIndex));
}
// Symbol: ?GetButtonText@CToolBar@@QEBAXHAEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@@Z
extern "C" void MS_ABI impl__GetButtonText_CToolBar__QEBAXHAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
    const CToolBar* pThis, int nIndex, CString* pStr) {
    pThis->GetButtonText(nIndex, *pStr);
}
CToolBar::CToolBar()
    : m_hRsrcImageWell(nullptr), m_hInstImageWell(nullptr), m_hbmImageWell(nullptr),
      m_bDelayedButtonLayout(FALSE), m_pStringMap(nullptr) {
    // m_nCount and the docking members are inherited from CControlBar (ctor).
    m_sizeButton.cx = 23;
    m_sizeButton.cy = 22;
    m_sizeImage.cx = 16;
    m_sizeImage.cy = 15;
}
CToolBar::~CToolBar() {
    if (m_hbmImageWell) {
        ::DeleteObject(m_hbmImageWell);
        m_hbmImageWell = nullptr;
    }
}
BOOL CToolBar::Create(CWnd* pParentWnd, DWORD dwStyle, UINT nID) {
    return CreateEx(pParentWnd, TBSTYLE_FLAT, dwStyle, CRect(0,0,0,0), nID);
}
BOOL CToolBar::CreateEx(CWnd* pParentWnd, DWORD dwCtrlStyle, DWORD dwStyle,
                         CRect rcBorders, UINT nID) {
    if (!pParentWnd) return FALSE;

    m_dwStyle = dwStyle;

    DWORD dwWinStyle = dwStyle & 0xFFFF;
    dwWinStyle |= WS_CHILD | CCS_NORESIZE | CCS_NOPARENTALIGN | CCS_NODIVIDER;

    m_hWnd = ::CreateWindowExW(0, TOOLBARCLASSNAMEW, nullptr, dwWinStyle,
                                rcBorders.left, rcBorders.top,
                                rcBorders.right - rcBorders.left,
                                rcBorders.bottom - rcBorders.top,
                                pParentWnd->GetSafeHwnd(), (HMENU)(UINT_PTR)nID,
                                AfxGetInstanceHandle(), nullptr);

    if (!m_hWnd) return FALSE;

    ::SendMessageW(m_hWnd, TB_SETEXTENDEDSTYLE, 0, dwCtrlStyle);
    ::SendMessageW(m_hWnd, TB_SETBITMAPSIZE, 0, MAKELPARAM(m_sizeImage.cx, m_sizeImage.cy));
    ::SendMessageW(m_hWnd, TB_SETBUTTONSIZE, 0, MAKELPARAM(m_sizeButton.cx, m_sizeButton.cy));

    return TRUE;
}
BOOL CToolBar::LoadToolBar(UINT nIDResource) {
    HRSRC hRsrc = ::FindResourceW(AfxGetInstanceHandle(),
                                   MAKEINTRESOURCEW(nIDResource), RT_TOOLBAR);
    if (!hRsrc) return FALSE;

    HGLOBAL hGlobal = ::LoadResource(AfxGetInstanceHandle(), hRsrc);
    if (!hGlobal) return FALSE;

    WORD* pData = (WORD*)::LockResource(hGlobal);
    if (!pData) return FALSE;

    WORD wVersion = pData[0];
    WORD wCount = pData[1];
    WORD wWidth = pData[2];
    WORD wHeight = pData[3];
    (void)wVersion; (void)wWidth; (void)wHeight;

    WORD* pButtonIDs = pData + 4;

    for (WORD i = 0; i < wCount; ++i) {
        TBBUTTON tb = {};
        tb.iBitmap = I_IMAGENONE;
        tb.idCommand = pButtonIDs[i];
        tb.fsState = TBSTATE_ENABLED;
        tb.fsStyle = (pButtonIDs[i] == 0) ? BTNS_SEP : BTNS_BUTTON;
        tb.dwData = 0;
        tb.iString = -1;
        ::SendMessageW(m_hWnd, TB_ADDBUTTONSW, 1, (LPARAM)&tb);
    }

    m_nCount = wCount;
    return TRUE;
}
BOOL CToolBar::LoadToolBar(const wchar_t* lpszResourceName) {
    if (!lpszResourceName) return FALSE;
    HRSRC hRsrc = ::FindResourceW(AfxGetInstanceHandle(), lpszResourceName, RT_TOOLBAR);
    if (!hRsrc) return FALSE;
    HGLOBAL hGlobal = ::LoadResource(AfxGetInstanceHandle(), hRsrc);
    if (!hGlobal) return FALSE;
    WORD* pData = (WORD*)::LockResource(hGlobal);
    if (!pData) return FALSE;

    WORD wCount = pData[1];
    WORD* pButtonIDs = pData + 4;

    for (int i = 0; i < wCount; i++) {
        TBBUTTON tb = {};
        tb.iBitmap = I_IMAGENONE;
        tb.idCommand = pButtonIDs[i];
        tb.fsState = TBSTATE_ENABLED;
        tb.fsStyle = (pButtonIDs[i] == 0) ? BTNS_SEP : BTNS_BUTTON;
        tb.dwData = 0;
        tb.iString = -1;
        ::SendMessageW(m_hWnd, TB_ADDBUTTONSW, 1, (LPARAM)&tb);
    }

    m_nCount = wCount;
    return TRUE;
}
BOOL CToolBar::LoadBitmap(UINT nIDResource) {
    HBITMAP hbm = ::LoadBitmapW(AfxGetInstanceHandle(), MAKEINTRESOURCEW(nIDResource));
    if (!hbm) return FALSE;
    return SetBitmap(hbm);
}
BOOL CToolBar::LoadBitmap(const wchar_t* lpszResourceName) {
    HBITMAP hbm = ::LoadBitmapW(AfxGetInstanceHandle(), lpszResourceName);
    if (!hbm) return FALSE;
    return SetBitmap(hbm);
}
BOOL CToolBar::SetButtons(const UINT* lpIDArray, int nIDCount) {
    if (!m_hWnd || !lpIDArray || nIDCount <= 0) return FALSE;

    while ((int)::SendMessageW(m_hWnd, TB_BUTTONCOUNT, 0, 0) > 0) {
        ::SendMessageW(m_hWnd, TB_DELETEBUTTON, 0, 0);
    }

    for (int i = 0; i < nIDCount; i++) {
        TBBUTTON tb = {};
        tb.iBitmap = I_IMAGENONE;
        tb.idCommand = lpIDArray[i];
        tb.fsState = TBSTATE_ENABLED;
        tb.fsStyle = (lpIDArray[i] == 0) ? BTNS_SEP : BTNS_BUTTON;
        tb.dwData = 0;
        tb.iString = -1;
        ::SendMessageW(m_hWnd, TB_INSERTBUTTONW, i, (LPARAM)&tb);
    }
    m_nCount = nIDCount;
    return TRUE;
}
BOOL CToolBar::SetButtonInfo(int nIndex, UINT nID, UINT nStyle, int iImage) {
    if (!m_hWnd) return FALSE;
    TBBUTTONINFO tbi = {};
    tbi.cbSize = sizeof(TBBUTTONINFO);
    tbi.dwMask = TBIF_COMMAND | TBIF_STYLE | TBIF_IMAGE;
    tbi.idCommand = nID;
    tbi.fsStyle = (BYTE)nStyle;
    tbi.iImage = iImage;
    return ::SendMessageW(m_hWnd, TB_SETBUTTONINFOW, nIndex, (LPARAM)&tbi) ? TRUE : FALSE;
}
void CToolBar::GetButtonInfo(int nIndex, UINT& nID, UINT& nStyle, int& iImage) const {
    nID = 0; nStyle = 0; iImage = -1;
    if (!m_hWnd) return;
    TBBUTTON tb = {};
    if (::SendMessageW(m_hWnd, TB_GETBUTTON, nIndex, (LPARAM)&tb)) {
        nID = tb.idCommand;
        nStyle = tb.fsStyle;
        iImage = tb.iBitmap;
    }
}
int CToolBar::CommandToIndex(UINT nIDFind) const {
    if (!m_hWnd) return -1;
    return (int)::SendMessageW(m_hWnd, TB_COMMANDTOINDEX, nIDFind, 0);
}
UINT CToolBar::GetItemID(int nIndex) const {
    if (!m_hWnd) return 0;
    TBBUTTON tb = {};
    if (::SendMessageW(m_hWnd, TB_GETBUTTON, nIndex, (LPARAM)&tb))
        return tb.idCommand;
    return 0;
}
void CToolBar::GetItemRect(int nIndex, LPRECT lpRect) const {
    if (m_hWnd && lpRect)
        ::SendMessageW(m_hWnd, TB_GETITEMRECT, nIndex, (LPARAM)lpRect);
}
void CToolBar::SetSizes(SIZE sizeButton, SIZE sizeImage) {
    m_sizeButton = sizeButton;
    m_sizeImage = sizeImage;
    if (m_hWnd) {
        ::SendMessageW(m_hWnd, TB_SETBITMAPSIZE, 0, MAKELPARAM(sizeImage.cx, sizeImage.cy));
        ::SendMessageW(m_hWnd, TB_SETBUTTONSIZE, 0, MAKELPARAM(sizeButton.cx, sizeButton.cy));
    }
}
CSize CToolBar::GetButtonSize() const {
    return CSize(m_sizeButton.cx, m_sizeButton.cy);
}
SIZE CToolBar::GetToolBarCtrlSize() const {
    SIZE sz = {0, 0};
    if (m_hWnd) ::SendMessageW(m_hWnd, TB_GETMAXSIZE, 0, (LPARAM)&sz);
    return sz;
}
void CToolBar::SetHeight(int cyHeight) {
    if (!m_hWnd) return;
    m_sizeButton.cy = cyHeight;
    ::SendMessageW(m_hWnd, TB_SETBUTTONSIZE, 0, MAKELPARAM(m_sizeButton.cx, m_sizeButton.cy));
}
int CToolBar::GetHeight() const {
    return m_sizeButton.cy;
}
void CToolBar::SetButtonStyle(int nIndex, UINT nStyle) {
    if (!m_hWnd) return;
    TBBUTTONINFO tbi = {};
    tbi.cbSize = sizeof(TBBUTTONINFO);
    tbi.dwMask = TBIF_STYLE;
    tbi.fsStyle = (BYTE)nStyle;
    ::SendMessageW(m_hWnd, TB_SETBUTTONINFOW, nIndex, (LPARAM)&tbi);
}
UINT CToolBar::GetButtonStyle(int nIndex) const {
    if (!m_hWnd) return 0;
    TBBUTTON tb = {};
    if (::SendMessageW(m_hWnd, TB_GETBUTTON, nIndex, (LPARAM)&tb))
        return tb.fsStyle;
    return 0;
}
BOOL CToolBar::SetButtonText(int nIndex, const wchar_t* lpszText) {
    if (!m_hWnd) return FALSE;
    TBBUTTONINFOW tbi = {};
    tbi.cbSize = sizeof(TBBUTTONINFOW);
    tbi.dwMask = TBIF_TEXT;
    tbi.pszText = (LPWSTR)lpszText;
    return static_cast<BOOL>(::SendMessageW(m_hWnd, TB_SETBUTTONINFOW, nIndex, (LPARAM)&tbi));
}
CString CToolBar::GetButtonText(int nIndex) const {
    CString str;
    if (!m_hWnd) return str;
    wchar_t buf[256] = {};
    TBBUTTONINFOW tbi = {};
    tbi.cbSize = sizeof(TBBUTTONINFOW);
    tbi.dwMask = TBIF_TEXT;
    tbi.pszText = buf;
    tbi.cchText = 256;
    if (::SendMessageW(m_hWnd, TB_GETBUTTONINFOW, nIndex, (LPARAM)&tbi))
        str = buf;
    return str;
}
void CToolBar::GetButtonText(int nIndex, CString& rString) const {
    rString = GetButtonText(nIndex);
}
BOOL CToolBar::SetBitmap(HBITMAP hbmImageWell) {
    if (!m_hWnd || !hbmImageWell) return FALSE;
    if (m_hbmImageWell) ::DeleteObject(m_hbmImageWell);
    m_hbmImageWell = hbmImageWell;

    TBADDBITMAP tbAddBmp = {};
    tbAddBmp.hInst = nullptr;
    tbAddBmp.nID = (UINT_PTR)hbmImageWell;
    ::SendMessageW(m_hWnd, TB_ADDBITMAP, 1, (LPARAM)&tbAddBmp);

    int nCount = (int)::SendMessageW(m_hWnd, TB_BUTTONCOUNT, 0, 0);
    for (int i = 0; i < nCount; i++) {
        TBBUTTONINFO tbi = {};
        tbi.cbSize = sizeof(TBBUTTONINFO);
        tbi.dwMask = TBIF_IMAGE;
        tbi.iImage = i;
        ::SendMessageW(m_hWnd, TB_SETBUTTONINFOW, i, (LPARAM)&tbi);
    }
    return TRUE;
}
void CToolBar::SetToolTips(CToolTipCtrl* pToolTip) {
    // Real MFC keeps no tooltip member; the toolbar control owns it.
    if (m_hWnd && pToolTip && pToolTip->GetSafeHwnd()) {
        ::SendMessageW(m_hWnd, TB_SETTOOLTIPS, (WPARAM)pToolTip->GetSafeHwnd(), 0);
    }
}
CToolTipCtrl* CToolBar::GetToolTips() const {
    if (!m_hWnd) return nullptr;
    HWND h = (HWND)::SendMessageW(m_hWnd, TB_GETTOOLTIPS, 0, 0);
    if (!h) return nullptr;
    return reinterpret_cast<CToolTipCtrl*>(CWnd::FromHandle(h));
}
void CToolBar::EnableDocking(DWORD dwDockStyle) {
    m_dwStyle = (m_dwStyle & ~CBRS_ALIGN_ANY) | (dwDockStyle & CBRS_ALIGN_ANY);
}
BOOL CToolBar::IsVisible() const {
    return m_hWnd && (::GetWindowLongW(m_hWnd, GWL_STYLE) & WS_VISIBLE);
}
BOOL CToolBar::IsFloating() const {
    return (m_dwStyle & CBRS_FLOATING) != 0;
}

// === Moved from ManualThunks.cpp ===
// Symbol: ?GetButtonInfo@CToolBar@@QEBAXHAEAI0AEAH@Z
extern "C" void MS_ABI impl__GetButtonInfo_CToolBar__QEBAXHAEAI0AEAH_Z(void* pThis, void* p0, void* p1, void* p2, void* p3) {
    (void)pThis;
    (void)p0;
    (void)p1;
    (void)p2;
    (void)p3;
}

// ?AddReplaceBitmap@CToolBar@@QEAAHPEAUHBITMAP__@@@Z
extern "C" int MS_ABI impl__AddReplaceBitmap_CToolBar__QEAAHPEAUHBITMAP_____Z(void* /*struct*/* p0) {
    return 0;
}

// ?LoadBitmapW@CToolBar@@QEAAHPEB_W@Z
extern "C" int MS_ABI impl__LoadBitmapW_CToolBar__QEAAHPEB_W_Z(const wchar_t* p0) {
    return 0;
}

// ?OnNcPaint@CToolBar@@IEAAXXZ
extern "C" void MS_ABI impl__OnNcPaint_CToolBar__IEAAXXZ() {}

// ?OnPaint@CToolBar@@IEAAXXZ
extern "C" void MS_ABI impl__OnPaint_CToolBar__IEAAXXZ() {}

// ?OnSetBitmapSize@CToolBar@@IEAA_J_K_J@Z
extern "C" __int64 MS_ABI impl__OnSetBitmapSize_CToolBar__IEAA_J_K_J_Z(unsigned __int64 p0, __int64 p1) {
    return 0;
}

