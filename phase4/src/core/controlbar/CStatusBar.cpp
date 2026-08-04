// CStatusBar — OpenMFC implementation.
// Sources: cbarcore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/CbarcoreSupport.h"

// Symbol: ?GetPaneText@CStatusBar@@QEBA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@H@Z
extern "C" void MS_ABI impl__GetPaneText_CStatusBar__QEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__H_Z(
    CString* pRet, const CStatusBar* pThis, int nIndex) {
    new (pRet) CString(pThis->GetPaneText(nIndex));
}
// Symbol: ?GetPaneInfo@CStatusBar@@QEBAXHAEAI0AEAH@Z
extern "C" void MS_ABI impl__GetPaneInfo_CStatusBar__QEBAXHAEAI0AEAH_Z(
    const CStatusBar* pThis, int nIndex, UINT* pnID, UINT* pnStyle, int* pcxWidth) {
    UINT id = 0;
    UINT style = 0;
    int width = 0;
    if (pThis) {
        pThis->GetPaneInfo(nIndex, id, style, width);
    }
    if (pnID) *pnID = id;
    if (pnStyle) *pnStyle = style;
    if (pcxWidth) *pcxWidth = width;
}
// Symbol: ?GetPaneText@CStatusBar@@QEBAXHAEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@@Z
extern "C" void MS_ABI impl__GetPaneText_CStatusBar__QEBAXHAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
    const CStatusBar* pThis, int nIndex, CString* pStr) {
    pThis->GetPaneText(nIndex, *pStr);
}
CStatusBar::CStatusBar() : m_nMinHeight(0) {
    // m_nCount/m_pData/m_cxLeftBorder/m_cxRightBorder are inherited from
    // CControlBar (initialized by its ctor).
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
    if (!m_hWnd || nIndex < 0 || nIndex >= m_nCount || !m_pData) return FALSE;

    CString paneText = GetPaneText(nIndex);
    m_pData[nIndex * 4] = nID;
    m_pData[nIndex * 4 + 1] = nStyle;
    m_pData[nIndex * 4 + 2] = cxWidth;

    RECT clientRect = {};
    ::GetClientRect(m_hWnd, &clientRect);
    const int totalWidth = std::max(0, static_cast<int>(clientRect.right - clientRect.left));

    int fixedWidth = 0;
    int autoPaneCount = 0;
    for (int i = 0; i < m_nCount; ++i) {
        int paneWidth = static_cast<int>(m_pData[i * 4 + 2]);
        if (paneWidth > 0) {
            fixedWidth += paneWidth;
        } else {
            ++autoPaneCount;
        }
    }

    int remainingWidth = std::max(0, totalWidth - fixedWidth);
    int autoWidth = (autoPaneCount > 0) ? (remainingWidth / autoPaneCount) : 0;
    int autoRemainder = (autoPaneCount > 0) ? (remainingWidth % autoPaneCount) : 0;

    std::vector<int> parts(static_cast<size_t>(m_nCount));
    int right = 0;
    for (int i = 0; i < m_nCount; ++i) {
        int paneWidth = static_cast<int>(m_pData[i * 4 + 2]);
        if (paneWidth <= 0) {
            paneWidth = autoWidth;
            if (autoRemainder > 0) {
                ++paneWidth;
                --autoRemainder;
            }
        }
        right += std::max(0, paneWidth);
        parts[static_cast<size_t>(i)] = right;
    }

    if (!parts.empty()) {
        parts.back() = -1;
    }

    ::SendMessageW(m_hWnd, SB_SETPARTS, m_nCount, reinterpret_cast<LPARAM>(parts.data()));
    ::SendMessageW(m_hWnd, SB_SETTEXTW, nIndex | (nStyle & 0xFF00),
                   reinterpret_cast<LPARAM>(paneText.GetString()));
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
    ::SendMessageW(m_hWnd, SB_SETTEXTW, nIndex, (LPARAM)(lpszNewText ? lpszNewText : L""));
    if (bUpdate) {
        ::InvalidateRect(m_hWnd, nullptr, TRUE);
        ::UpdateWindow(m_hWnd);
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
        int borders[3] = {0, 0, 0};
        if (::SendMessageW(m_hWnd, SB_GETBORDERS, 0, reinterpret_cast<LPARAM>(borders))) {
            sz.cx = borders[0];
            sz.cy = borders[1];
        }
    }
    return sz;
}
void CStatusBar::SetToolTips(CToolTipCtrl* pToolTip) {
    // Real MFC CStatusBar has no tooltip member (status panes manage their own).
    (void)pToolTip;
}
CToolTipCtrl* CStatusBar::GetToolTips() const {
    return nullptr;
}
void CStatusBar::EnableDocking(DWORD dwDockStyle) {
    m_dwStyle = (m_dwStyle & ~CBRS_ALIGN_ANY) | (dwDockStyle & CBRS_ALIGN_ANY);
}
BOOL CStatusBar::IsSimple() const {
    if (!m_hWnd) return FALSE;
    return (BOOL)::SendMessageW(m_hWnd, SB_ISSIMPLE, 0, 0);
}
