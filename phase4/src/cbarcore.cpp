// Toolbar / StatusBar / DialogBar Implementation
// Provides MFC wrappers around Windows common controls
//
// All exports use MSVC ABI via extern "C" + .def aliasing.

#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxole.h"
#include <commctrl.h>
#include <cstring>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

// MinGW compat
#ifndef RT_TOOLBAR
#define RT_TOOLBAR MAKEINTRESOURCE(241)
#endif
#ifndef ID_SEPARATOR
#define ID_SEPARATOR 0
#endif

//=============================================================================
// CToolTipCtrl
//=============================================================================
BOOL CToolTipCtrl::Create(CWnd* pParentWnd, DWORD dwStyle) {
    if (!pParentWnd) return FALSE;
    m_hWnd = ::CreateWindowExW(0, TOOLTIPS_CLASSW, nullptr,
                                dwStyle | WS_POPUP | TTS_NOPREFIX | TTS_ALWAYSTIP,
                                CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
                                pParentWnd->GetSafeHwnd(), nullptr,
                                AfxGetInstanceHandle(), nullptr);
    if (m_hWnd) {
        ::SetWindowPos(m_hWnd, HWND_TOPMOST, 0, 0, 0, 0,
                       SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
    }
    return m_hWnd != nullptr;
}

BOOL CToolTipCtrl::AddTool(CWnd* pWnd, const wchar_t* pszText, LPCRECT lpRectTool, UINT_PTR nIDTool) {
    if (!m_hWnd || !pWnd) return FALSE;
    TOOLINFOW ti = {};
    ti.cbSize = sizeof(TOOLINFOW);
    ti.hwnd = pWnd->GetSafeHwnd();
    ti.uId = nIDTool ? nIDTool : (UINT_PTR)pWnd->GetSafeHwnd();
    ti.lpszText = (LPWSTR)pszText;
    ti.uFlags = TTF_SUBCLASS | TTF_IDISHWND;
    if (lpRectTool) ti.rect = *lpRectTool;
    return ::SendMessageW(m_hWnd, TTM_ADDTOOLW, 0, (LPARAM)&ti) ? TRUE : FALSE;
}

void CToolTipCtrl::UpdateTipText(const wchar_t* pszText, CWnd* pWnd, UINT_PTR nIDTool) {
    if (!m_hWnd) return;
    TOOLINFOW ti = {};
    ti.cbSize = sizeof(TOOLINFOW);
    ti.hwnd = pWnd ? pWnd->GetSafeHwnd() : nullptr;
    ti.uId = nIDTool;
    ti.lpszText = (LPWSTR)pszText;
    ::SendMessageW(m_hWnd, TTM_UPDATETIPTEXTW, 0, (LPARAM)&ti);
}

void CToolTipCtrl::Activate(BOOL bActivate) {
    if (m_hWnd) ::SendMessageW(m_hWnd, TTM_ACTIVATE, (WPARAM)bActivate, 0);
}

void CToolTipCtrl::SetMaxTipWidth(int iWidth) {
    if (m_hWnd) ::SendMessageW(m_hWnd, TTM_SETMAXTIPWIDTH, 0, (LPARAM)iWidth);
}

int CToolTipCtrl::GetText(CWnd* pWnd, UINT_PTR nIDTool, wchar_t* pszText, int cchMax) const {
    if (!m_hWnd) return 0;
    TOOLINFOW ti = {};
    ti.cbSize = sizeof(TOOLINFOW);
    ti.hwnd = pWnd ? pWnd->GetSafeHwnd() : nullptr;
    ti.uId = nIDTool;
    ti.lpszText = pszText;
    return (int)::SendMessageW(m_hWnd, TTM_GETTEXTW, 0, (LPARAM)&ti);
}

//=============================================================================
// CToolBar
//=============================================================================
IMPLEMENT_DYNAMIC(CToolBar, CControlBar)

CToolBar::CToolBar()
    : m_nCount(0), m_hbmImageWell(nullptr), m_pToolTip(nullptr),
      m_pDockBar(nullptr), m_pDockSite(nullptr), m_pDockContext(nullptr),
      m_bInRecalcLayout(FALSE), m_bDelayedButtonLayout(FALSE) {
    m_sizeButton.cx = 23;
    m_sizeButton.cy = 22;
    m_sizeImage.cx = 16;
    m_sizeImage.cy = 15;
    memset(_toolbar_padding, 0, sizeof(_toolbar_padding));
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

BOOL CToolBar::LoadToolBar(const wchar_t* lpszResourceName) {
    if (!lpszResourceName) return FALSE;
    // Try loading by name
    HRSRC hRsrc = ::FindResourceW(AfxGetInstanceHandle(), lpszResourceName, RT_TOOLBAR);
    if (!hRsrc) return FALSE;
    HGLOBAL hGlobal = ::LoadResource(AfxGetInstanceHandle(), hRsrc);
    if (!hGlobal) return FALSE;
    WORD* pData = (WORD*)::LockResource(hGlobal);
    WORD wCount = pData[1];
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

void CToolBar::SetButtonText(int nIndex, const wchar_t* lpszText) {
    if (!m_hWnd) return;
    TBBUTTONINFO tbi = {};
    tbi.cbSize = sizeof(TBBUTTONINFO);
    tbi.dwMask = TBIF_TEXT;
    tbi.pszText = (LPWSTR)lpszText;
    ::SendMessageW(m_hWnd, TB_SETBUTTONINFOW, nIndex, (LPARAM)&tbi);
}

CString CToolBar::GetButtonText(int nIndex) const {
    CString str;
    if (!m_hWnd) return str;
    wchar_t buf[256] = {};
    TBBUTTONINFO tbi = {};
    tbi.cbSize = sizeof(TBBUTTONINFO);
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
    m_pToolTip = pToolTip;
    if (m_hWnd && pToolTip && pToolTip->GetSafeHwnd()) {
        ::SendMessageW(m_hWnd, TB_SETTOOLTIPS, (WPARAM)pToolTip->GetSafeHwnd(), 0);
    }
}

CToolTipCtrl* CToolBar::GetToolTips() const {
    return m_pToolTip;
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

//=============================================================================
// CStatusBar
//=============================================================================
IMPLEMENT_DYNAMIC(CStatusBar, CControlBar)

CStatusBar::CStatusBar()
    : m_nCount(0), m_pData(nullptr), m_cxLeftBorder(0), m_cxRightBorder(0),
      m_pToolTip(nullptr) {
    memset(_statusbar_padding, 0, sizeof(_statusbar_padding));
}

CStatusBar::~CStatusBar() {
    if (m_pData) {
        free(m_pData);
        m_pData = nullptr;
    }
}

BOOL CStatusBar::Create(CWnd* pParentWnd, DWORD dwStyle, UINT nID) {
    if (!pParentWnd) return FALSE;

    m_dwStyle = dwStyle;

    m_hWnd = ::CreateWindowExW(0, STATUSCLASSNAMEW, nullptr,
                                WS_CHILD | WS_VISIBLE | dwStyle,
                                0, 0, 0, 0,
                                pParentWnd->GetSafeHwnd(),
                                (HMENU)(UINT_PTR)nID,
                                AfxGetInstanceHandle(), nullptr);
    return m_hWnd != nullptr;
}

BOOL CStatusBar::SetIndicators(const UINT* lpIDArray, int nIDCount) {
    if (!m_hWnd || !lpIDArray || nIDCount <= 0) return FALSE;

    if (m_pData) free(m_pData);
    m_pData = (UINT*)malloc(nIDCount * sizeof(UINT) * 4);
    if (!m_pData) return FALSE;
    m_nCount = nIDCount;

    for (int i = 0; i < nIDCount; i++) {
        m_pData[i * 4] = lpIDArray[i];
        m_pData[i * 4 + 1] = 0;
        m_pData[i * 4 + 2] = 0;
        m_pData[i * 4 + 3] = 0;
    }

    // Simple even distribution
    int nParts = nIDCount;
    int* pWidths = (int*)malloc(nParts * sizeof(int));
    if (pWidths) {
        RECT rc;
        ::GetClientRect(m_hWnd, &rc);
        int cxTotal = rc.right - rc.left;
        int cxWidth = cxTotal / nParts;
        for (int i = 0; i < nParts; i++) {
            pWidths[nParts - 1 - i] = cxTotal - (i * cxWidth);
        }
        ::SendMessageW(m_hWnd, SB_SETPARTS, nParts, (LPARAM)pWidths);
        free(pWidths);
    }

    // Set text for string-based indicators
    for (int i = 0; i < nIDCount; i++) {
        if (lpIDArray[i] != 0 && lpIDArray[i] != ID_SEPARATOR) {
            wchar_t buf[256];
            if (::LoadStringW(AfxGetInstanceHandle(), lpIDArray[i], buf, 256) > 0) {
                ::SendMessageW(m_hWnd, SB_SETTEXTW, i, (LPARAM)buf);
            }
        }
    }

    return TRUE;
}

BOOL CStatusBar::SetPaneInfo(int nIndex, UINT nID, UINT nStyle, int cxWidth) {
    if (!m_hWnd || nIndex < 0 || nIndex >= m_nCount) return FALSE;
    m_pData[nIndex * 4] = nID;
    m_pData[nIndex * 4 + 1] = nStyle;
    m_pData[nIndex * 4 + 2] = cxWidth;
    return TRUE;
}

void CStatusBar::GetPaneInfo(int nIndex, UINT& nID, UINT& nStyle, int& cxWidth) const {
    if (nIndex >= 0 && nIndex < m_nCount && m_pData) {
        nID = m_pData[nIndex * 4];
        nStyle = m_pData[nIndex * 4 + 1];
        cxWidth = m_pData[nIndex * 4 + 2];
    } else {
        nID = 0; nStyle = 0; cxWidth = 0;
    }
}

int CStatusBar::CommandToIndex(UINT nIDFind) const {
    if (!m_pData) return -1;
    for (int i = 0; i < m_nCount; i++) {
        if (m_pData[i * 4] == nIDFind) return i;
    }
    return -1;
}

UINT CStatusBar::GetItemID(int nIndex) const {
    if (!m_pData || nIndex < 0 || nIndex >= m_nCount) return 0;
    return m_pData[nIndex * 4];
}

void CStatusBar::GetItemRect(int nIndex, LPRECT lpRect) const {
    if (m_hWnd && lpRect)
        ::SendMessageW(m_hWnd, SB_GETRECT, nIndex, (LPARAM)lpRect);
}

BOOL CStatusBar::SetPaneText(int nIndex, const wchar_t* lpszNewText, BOOL bUpdate) {
    if (!m_hWnd || nIndex < 0 || nIndex >= m_nCount) return FALSE;
    if (bUpdate) {
        ::SendMessageW(m_hWnd, SB_SETTEXTW, nIndex | SBT_OWNERDRAW, (LPARAM)lpszNewText);
    }
    return TRUE;
}

CString CStatusBar::GetPaneText(int nIndex) const {
    CString str;
    if (!m_hWnd || nIndex < 0 || nIndex >= m_nCount) return str;
    int len = LOWORD(::SendMessageW(m_hWnd, SB_GETTEXTLENGTHW, nIndex, 0));
    if (len > 0) {
        wchar_t* buf = (wchar_t*)malloc((len + 1) * sizeof(wchar_t));
        if (buf) {
            ::SendMessageW(m_hWnd, SB_GETTEXTW, nIndex, (LPARAM)buf);
            str = buf;
            free(buf);
        }
    }
    return str;
}

void CStatusBar::GetPaneText(int nIndex, CString& rString) const {
    rString = GetPaneText(nIndex);
}

void CStatusBar::SetPaneStyle(int nIndex, UINT nStyle) {
    if (nIndex >= 0 && nIndex < m_nCount && m_pData) {
        m_pData[nIndex * 4 + 1] = nStyle;
    }
}

UINT CStatusBar::GetPaneStyle(int nIndex) const {
    if (nIndex >= 0 && nIndex < m_nCount && m_pData) {
        return m_pData[nIndex * 4 + 1];
    }
    return 0;
}

CSize CStatusBar::GetPaneSize(int nIndex) const {
    CSize sz(0, 0);
    if (m_hWnd) {
        RECT rc = {};
        ::SendMessageW(m_hWnd, SB_GETRECT, nIndex, (LPARAM)&rc);
        sz.cx = rc.right - rc.left;
        sz.cy = rc.bottom - rc.top;
    }
    return sz;
}

CSize CStatusBar::GetBorders() const {
    CSize sz(0, 0);
    if (m_hWnd) {
        sz.cx = (int)::SendMessageW(m_hWnd, SB_GETBORDERS, 0, 0);
        sz.cy = 0;
    }
    return sz;
}

void CStatusBar::SetToolTips(CToolTipCtrl* pToolTip) {
    m_pToolTip = pToolTip;
}

CToolTipCtrl* CStatusBar::GetToolTips() const {
    return m_pToolTip;
}

void CStatusBar::EnableDocking(DWORD dwDockStyle) {
    (void)dwDockStyle;
}

BOOL CStatusBar::IsSimple() const {
    if (!m_hWnd) return FALSE;
    return (BOOL)::SendMessageW(m_hWnd, SB_ISSIMPLE, 0, 0);
}

//=============================================================================
// CDialogBar
//=============================================================================
IMPLEMENT_DYNAMIC(CDialogBar, CControlBar)

CDialogBar::CDialogBar()
    : m_bAutoDelete(FALSE), m_pOccDialogInfo(nullptr) {
    memset(_dialogbar_padding, 0, sizeof(_dialogbar_padding));
}

CDialogBar::~CDialogBar() {
    if (m_hWnd) ::DestroyWindow(m_hWnd);
}

BOOL CDialogBar::Create(CWnd* pParentWnd, UINT nIDTemplate, UINT nStyle, UINT nID) {
    if (!pParentWnd) return FALSE;

    m_dwStyle = nStyle;

    m_hWnd = ::CreateDialogParamW(AfxGetInstanceHandle(),
                                   MAKEINTRESOURCEW(nIDTemplate),
                                   pParentWnd->GetSafeHwnd(),
                                   nullptr, 0);

    if (!m_hWnd) return FALSE;

    ::SetWindowLongPtrW(m_hWnd, GWLP_ID, nID);

    if (nStyle & WS_VISIBLE) {
        ::ShowWindow(m_hWnd, SW_SHOW);
    }

    return TRUE;
}

BOOL CDialogBar::Create(CWnd* pParentWnd, const wchar_t* lpszTemplateName,
                         UINT nStyle, UINT nID) {
    if (!pParentWnd || !lpszTemplateName) return FALSE;

    m_dwStyle = nStyle;

    m_hWnd = ::CreateDialogParamW(AfxGetInstanceHandle(),
                                   lpszTemplateName,
                                   pParentWnd->GetSafeHwnd(),
                                   nullptr, 0);

    if (!m_hWnd) return FALSE;

    ::SetWindowLongPtrW(m_hWnd, GWLP_ID, nID);

    if (nStyle & WS_VISIBLE) {
        ::ShowWindow(m_hWnd, SW_SHOW);
    }

    return TRUE;
}

void CDialogBar::UpdateData(BOOL bSaveAndValidate) {
    (void)bSaveAndValidate;
}

BOOL CDialogBar::IsVisible() const {
    return m_hWnd && ::IsWindowVisible(m_hWnd);
}

void CDialogBar::SetOccDialogInfo(void* pDialogInfo) {
    m_pOccDialogInfo = pDialogInfo;
}
