// CSplitterWnd — OpenMFC implementation.
// Sources: cbarcore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/CbarcoreSupport.h"

CSplitterWnd::CSplitterWnd()
    : m_nRows(0), m_nCols(0), m_cxSplitter(4), m_cySplitter(4),
      m_cxBorderShare(0), m_cyBorderShare(0),
      m_cxSplitterGap(4), m_cySplitterGap(4),
      m_nMaxRows(0), m_nMaxCols(0),
      m_nId(0), m_bHasHScroll(FALSE), m_bHasVScroll(FALSE),
      m_pActivePane(nullptr), m_nActiveRow(0), m_nActiveCol(0) {
    m_sizeMin.cx = 0; m_sizeMin.cy = 0;
    memset(_splitterwnd_padding, 0, sizeof(_splitterwnd_padding));
}
CSplitterWnd::~CSplitterWnd() {
    if (m_hWnd) ::DestroyWindow(m_hWnd);
    std::lock_guard<std::mutex> lock(g_splitterWndMutex);
    g_splitterWndStates.erase(this);
}
BOOL CSplitterWnd::Create(CWnd* pParentWnd, int nMaxRows, int nMaxCols,
                           SIZE sizeMin, CCreateContext* pContext, DWORD dwStyle, UINT nID) {
    if (!pParentWnd) return FALSE;
    if (nMaxRows <= 0 || nMaxCols <= 0) return FALSE;
    m_nMaxRows = nMaxRows;
    m_nMaxCols = nMaxCols;
    m_sizeMin = sizeMin;
    m_nRows = nMaxRows;
    m_nCols = nMaxCols;

    m_hWnd = ::CreateWindowExW(0, L"AfxSplitterWnd", nullptr,
                                dwStyle | WS_CLIPCHILDREN,
                                0, 0, 0, 0,
                                pParentWnd->GetSafeHwnd(),
                                (HMENU)(UINT_PTR)nID,
                                AfxGetInstanceHandle(), pContext);
    if (!m_hWnd) return FALSE;
    m_nId = nID;

    std::lock_guard<std::mutex> lock(g_splitterWndMutex);
    NormalizeSplitterState(this);
    (void)pContext;
    return TRUE;
}
BOOL CSplitterWnd::CreateStatic(CWnd* pParentWnd, int nRows, int nCols,
                                 DWORD dwStyle, UINT nID) {
    if (nRows <= 0 || nCols <= 0) return FALSE;
    if (!pParentWnd) return FALSE;
    m_nRows = nRows;
    m_nCols = nCols;
    m_nMaxRows = nRows;
    m_nMaxCols = nCols;

    m_hWnd = ::CreateWindowExW(0, L"AfxSplitterWnd", nullptr,
                                dwStyle | WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN,
                                0, 0, 0, 0,
                                pParentWnd->GetSafeHwnd(),
                                (HMENU)(UINT_PTR)nID,
                                AfxGetInstanceHandle(), nullptr);
    if (!m_hWnd) return FALSE;
    std::lock_guard<std::mutex> lock(g_splitterWndMutex);
    NormalizeSplitterState(this);
    return TRUE;
}
BOOL CSplitterWnd::CreateView(int row, int col, CRuntimeClass* pViewClass,
                               SIZE sizeInit, CCreateContext* pContext) {
    if (!m_hWnd) return FALSE;
    if (row < 0 || col < 0 || row >= m_nRows || col >= m_nCols) return FALSE;
    if (!pViewClass || !pViewClass->IsDerivedFrom(RUNTIME_CLASS(CWnd))) return FALSE;

    CObject* pObject = pViewClass->CreateObject();
    if (!pObject) return FALSE;
    CWnd* pPane = dynamic_cast<CWnd*>(pObject);
    if (!pPane) {
        delete pObject;
        return FALSE;
    }

    RECT rc{};
    const int paneId = AFX_IDW_PANE_FIRST + row * m_nCols + col;
    if (!pPane->Create(nullptr, nullptr, WS_CHILD | WS_VISIBLE,
                       rc, this, paneId, pContext)) {
        delete pPane;
        return FALSE;
    }

    {
        std::lock_guard<std::mutex> lock(g_splitterWndMutex);
        NormalizeSplitterState(this);
        auto& state = EnsureSplitterState(this);
        if (state.panes.size() > static_cast<size_t>(row) && state.panes[row].size() > static_cast<size_t>(col)) {
            state.panes[row][col] = pPane;
        }
        if (sizeInit.cy > 0) {
            const int rowIdeal = std::max(1, static_cast<int>(sizeInit.cy));
            const int rowMin = static_cast<int>(sizeInit.cy);
            state.rowInfo[row].ideal = std::max(state.rowInfo[row].ideal, rowIdeal);
            state.rowInfo[row].min = std::max(state.rowInfo[row].min, rowMin);
        }
        if (sizeInit.cx > 0) {
            const int colIdeal = std::max(1, static_cast<int>(sizeInit.cx));
            const int colMin = static_cast<int>(sizeInit.cx);
            state.colInfo[col].ideal = std::max(state.colInfo[col].ideal, colIdeal);
            state.colInfo[col].min = std::max(state.colInfo[col].min, colMin);
        }
    }

    SetActivePane(row, col, pPane);
    RecalcLayout();
    return TRUE;
}
CWnd* CSplitterWnd::GetPane(int row, int col) const {
    std::lock_guard<std::mutex> lock(g_splitterWndMutex);
    return GetSplitterPane(this, row, col);
}
void CSplitterWnd::GetRowInfo(int row, int& cyCur, int& cyMin) const {
    std::lock_guard<std::mutex> lock(g_splitterWndMutex);
    const SplitterLayoutState& state = EnsureSplitterState(this);
    if (row < 0 || row >= static_cast<int>(state.rowInfo.size())) {
        cyCur = 0;
        cyMin = 0;
        return;
    }
    cyCur = state.rowInfo[row].ideal;
    cyMin = state.rowInfo[row].min;
}
void CSplitterWnd::SetRowInfo(int row, int cyIdeal, int cyMin) {
    if (row < 0) return;
    std::lock_guard<std::mutex> lock(g_splitterWndMutex);
    NormalizeSplitterState(this);
    if (row >= m_nRows) return;
    auto& state = EnsureSplitterState(this);
    state.rowInfo[row].ideal = std::max(0, cyIdeal);
    state.rowInfo[row].min = std::max(0, cyMin);
}
void CSplitterWnd::GetColumnInfo(int col, int& cxCur, int& cxMin) const {
    std::lock_guard<std::mutex> lock(g_splitterWndMutex);
    const SplitterLayoutState& state = EnsureSplitterState(this);
    if (col < 0 || col >= static_cast<int>(state.colInfo.size())) {
        cxCur = 0;
        cxMin = 0;
        return;
    }
    cxCur = state.colInfo[col].ideal;
    cxMin = state.colInfo[col].min;
}
void CSplitterWnd::SetColumnInfo(int col, int cxIdeal, int cxMin) {
    if (col < 0) return;
    std::lock_guard<std::mutex> lock(g_splitterWndMutex);
    NormalizeSplitterState(this);
    if (col >= m_nCols) return;
    auto& state = EnsureSplitterState(this);
    state.colInfo[col].ideal = std::max(0, cxIdeal);
    state.colInfo[col].min = std::max(0, cxMin);
}
void CSplitterWnd::RecalcLayout() {
    if (!m_hWnd) return;
    if (m_nRows <= 0 || m_nCols <= 0) return;
    RECT rc{};
    GetClientRect(&rc);
    CRect client(rc);
    if (client.Width() <= 0 || client.Height() <= 0) return;

    NormalizeSplitterState(this);
    std::lock_guard<std::mutex> lock(g_splitterWndMutex);
    const SplitterLayoutState& state = EnsureSplitterState(this);

    const int rowCount = static_cast<int>(state.panes.size());
    const int colCount = rowCount > 0 ? static_cast<int>(state.panes[0].size()) : 0;
    if (rowCount <= 0 || colCount <= 0) return;

    const int xGap = m_cxSplitter + m_cxSplitterGap;
    const int yGap = m_cySplitter + m_cySplitterGap;
    const int width = client.Width() - std::max(0, colCount - 1) * xGap;
    const int height = client.Height() - std::max(0, rowCount - 1) * yGap;
    if (width <= 0 || height <= 0) return;

    const int minCol = std::max(1, static_cast<int>(m_sizeMin.cx));
    const int minRow = std::max(1, static_cast<int>(m_sizeMin.cy));
    std::vector<int> colWidths = SplitterSizesFromInfo(width, state.colInfo, minCol);
    std::vector<int> rowHeights = SplitterSizesFromInfo(height, state.rowInfo, minRow);

    int y = client.top;
    for (int row = 0; row < rowCount; ++row) {
        int x = client.left;
        for (int col = 0; col < colCount; ++col) {
            const int cw = (col < static_cast<int>(colWidths.size())) ? colWidths[col] : 0;
            const int ch = (row < static_cast<int>(rowHeights.size())) ? rowHeights[row] : 0;
            CWnd* pane = (row < static_cast<int>(state.panes.size()) && col < static_cast<int>(state.panes[row].size())) ?
                state.panes[row][col] : nullptr;
            if (pane && pane->GetSafeHwnd()) {
                pane->MoveWindow(x, y, cw, ch, TRUE);
            }
            x += cw + xGap;
        }
        const int rh = (row < static_cast<int>(rowHeights.size())) ? rowHeights[row] : 0;
        y += rh + yGap;
    }

    bool activeValid = m_pActivePane != nullptr;
    if (activeValid && m_nActiveRow >= 0 && m_nActiveCol >= 0 &&
        m_nActiveRow < rowCount && m_nActiveCol < colCount &&
        state.panes[m_nActiveRow][m_nActiveCol] == m_pActivePane &&
        m_pActivePane->GetSafeHwnd()) {
        return;
    }

    for (int row = 0; row < rowCount; ++row) {
        for (int col = 0; col < colCount; ++col) {
            CWnd* pane = state.panes[row][col];
            if (pane && pane->GetSafeHwnd()) {
                m_pActivePane = pane;
                m_nActiveRow = row;
                m_nActiveCol = col;
                return;
            }
        }
    }
    m_pActivePane = nullptr;
    m_nActiveRow = -1;
    m_nActiveCol = -1;
}
void CSplitterWnd::SetSplitCursor(int ht) {
    (void)ht;
    const LPCWSTR cursorId = (m_nCols > 1) ? MAKEINTRESOURCEW(IDC_SIZEWE)
                                           : MAKEINTRESOURCEW(IDC_SIZENS);
    HCURSOR cursor = ::LoadCursorW(nullptr, cursorId);
    if (cursor) {
        ::SetCursor(cursor);
    }
}
int CSplitterWnd::GetActivePane(int* pRow, int* pCol) const {
    if (pRow) *pRow = m_nActiveRow;
    if (pCol) *pCol = m_nActiveCol;
    if (m_nActiveRow < 0 || m_nActiveCol < 0) return -1;
    return m_nActiveRow * m_nCols + m_nActiveCol;
}
void CSplitterWnd::SetActivePane(int row, int col, CWnd* pWnd) {
    CWnd* target = pWnd ? pWnd : GetPane(row, col);
    if (!target || row < 0 || col < 0 || row >= m_nRows || col >= m_nCols) return;
    m_nActiveRow = row;
    m_nActiveCol = col;
    m_pActivePane = target;
    if (m_pActivePane && m_pActivePane->GetSafeHwnd()) {
        m_pActivePane->SetFocus();
    }
}
CWnd* CSplitterWnd::GetActivePane() {
    return m_pActivePane;
}
BOOL CSplitterWnd::CanActivateNext(BOOL bPrev) {
    if (m_nRows <= 0 || m_nCols <= 0) return FALSE;
    const int total = m_nRows * m_nCols;
    if (total <= 1) return FALSE;

    std::lock_guard<std::mutex> lock(g_splitterWndMutex);
    NormalizeSplitterState(this);
    const SplitterLayoutState& state = EnsureSplitterState(this);
    const int start = (m_nActiveRow < 0 || m_nActiveCol < 0) ? (bPrev ? total - 1 : 0)
                                                             : (m_nActiveRow * m_nCols + m_nActiveCol);
    for (int i = 1; i <= total; ++i) {
        const int candidate = bPrev ? ((start - i + total) % total) : ((start + i) % total);
        const int row = candidate / m_nCols;
        const int col = candidate % m_nCols;
        if (row < static_cast<int>(state.panes.size()) &&
            col < static_cast<int>(state.panes[row].size()) &&
            state.panes[row][col] != nullptr) {
            return TRUE;
        }
    }
    return FALSE;
}
void CSplitterWnd::ActivateNext(BOOL bPrev) {
    if (!CanActivateNext(bPrev)) return;

    std::lock_guard<std::mutex> lock(g_splitterWndMutex);
    NormalizeSplitterState(this);
    const SplitterLayoutState& state = EnsureSplitterState(this);
    const int total = m_nRows * m_nCols;
    const int start = (m_nActiveRow < 0 || m_nActiveCol < 0) ? (bPrev ? total - 1 : 0)
                                                             : (m_nActiveRow * m_nCols + m_nActiveCol);
    for (int i = 1; i <= total; ++i) {
        const int candidate = bPrev ? ((start - i + total) % total) : ((start + i) % total);
        const int row = candidate / m_nCols;
        const int col = candidate % m_nCols;
        if (row >= static_cast<int>(state.panes.size()) || col >= static_cast<int>(state.panes[row].size())) {
            continue;
        }
        CWnd* pane = state.panes[row][col];
        if (pane && pane->GetSafeHwnd()) {
            SetActivePane(row, col, pane);
            break;
        }
    }
}
BOOL CSplitterWnd::DoKeyboardSplit() {
    if (!CanActivateNext(FALSE)) return FALSE;
    ActivateNext(FALSE);
    return TRUE;
}
void CSplitterWnd::OnDrawSplitter(CDC* pDC, int nType, const CRect& rect) {
    if (pDC && pDC->m_hDC) {
        HBRUSH hBrush = ::GetSysColorBrush(COLOR_3DFACE);
        ::FillRect(pDC->m_hDC, (const RECT*)rect, hBrush);
    }
}
void CSplitterWnd::OnInvertTracker(const CRect& rect) {
}
BOOL CSplitterWnd::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) {
    (void)lpcs;
    (void)pContext;
    if (!m_hWnd) return FALSE;
    std::lock_guard<std::mutex> lock(g_splitterWndMutex);
    NormalizeSplitterState(this);
    return TRUE;
}
