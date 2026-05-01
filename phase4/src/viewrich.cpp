// CRichEditView & CRichEditCtrl Implementation
// Rich Edit 2.0 view with formatting and printing support
//
// All exports use MSVC ABI via extern "C" + .def aliasing.

#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxwin.h"
#include <richedit.h>
#include <commctrl.h>
#include <cstring>
#include <cstdio>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

// MinGW compat: rich edit 2.0 class name may be missing
#ifndef MSFTEDIT_CLASS
#define MSFTEDIT_CLASS L"RICHEDIT50W"
#endif

#ifndef RICHEDIT_CLASS
#define RICHEDIT_CLASS L"RichEdit20W"
#endif

// Rich edit 2.0 class (prefer 4.1 if available)
static const wchar_t* g_pszRichEditClass = RICHEDIT_CLASS;

//=============================================================================
// CRichEditCtrl
//=============================================================================
CRichEditCtrl::CRichEditCtrl() {
    memset(_richeditctrl_padding, 0, sizeof(_richeditctrl_padding));
}

CRichEditCtrl::~CRichEditCtrl() {
    if (m_hWnd) ::DestroyWindow(m_hWnd);
}

BOOL CRichEditCtrl::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID) {
    if (!pParentWnd) return FALSE;
    m_hWnd = ::CreateWindowExW(0, g_pszRichEditClass, nullptr,
                                dwStyle | WS_CHILD | WS_VISIBLE,
                                rect.left, rect.top,
                                rect.right - rect.left, rect.bottom - rect.top,
                                pParentWnd->GetSafeHwnd(),
                                (HMENU)(UINT_PTR)nID,
                                AfxGetInstanceHandle(), nullptr);
    if (m_hWnd) {
        ::SendMessageW(m_hWnd, EM_SETEVENTMASK, 0, ENM_CHANGE | ENM_SELCHANGE);
    }
    return m_hWnd != nullptr;
}

DWORD CRichEditCtrl::GetDefaultCharFormat(CHARFORMAT2W& cf) const {
    if (!m_hWnd) return 0;
    cf.cbSize = sizeof(CHARFORMAT2W);
    return (DWORD)::SendMessageW(m_hWnd, EM_GETCHARFORMAT, SCF_DEFAULT, (LPARAM)&cf);
}

BOOL CRichEditCtrl::SetDefaultCharFormat(const CHARFORMAT2W& cf) {
    if (!m_hWnd) return FALSE;
    return (BOOL)::SendMessageW(m_hWnd, EM_SETCHARFORMAT, SCF_DEFAULT, (LPARAM)&cf);
}

DWORD CRichEditCtrl::GetSelectionCharFormat(CHARFORMAT2W& cf) const {
    if (!m_hWnd) return 0;
    cf.cbSize = sizeof(CHARFORMAT2W);
    return (DWORD)::SendMessageW(m_hWnd, EM_GETCHARFORMAT, SCF_SELECTION, (LPARAM)&cf);
}

BOOL CRichEditCtrl::SetSelectionCharFormat(const CHARFORMAT2W& cf) {
    if (!m_hWnd) return FALSE;
    return (BOOL)::SendMessageW(m_hWnd, EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM)&cf);
}

DWORD CRichEditCtrl::GetParaFormat(PARAFORMAT2& pf) const {
    if (!m_hWnd) return 0;
    pf.cbSize = sizeof(PARAFORMAT2);
    return (DWORD)::SendMessageW(m_hWnd, EM_GETPARAFORMAT, 0, (LPARAM)&pf);
}

BOOL CRichEditCtrl::SetParaFormat(const PARAFORMAT2& pf) {
    if (!m_hWnd) return FALSE;
    return (BOOL)::SendMessageW(m_hWnd, EM_SETPARAFORMAT, 0, (LPARAM)&pf);
}

BOOL CRichEditCtrl::SetSel(int nStartChar, int nEndChar) {
    if (!m_hWnd) return FALSE;
    ::SendMessageW(m_hWnd, EM_SETSEL, nStartChar, nEndChar);
    return TRUE;
}

void CRichEditCtrl::GetSel(int& nStartChar, int& nEndChar) const {
    nStartChar = 0; nEndChar = 0;
    if (m_hWnd) {
        CHARRANGE cr = {};
        ::SendMessageW(m_hWnd, EM_EXGETSEL, 0, (LPARAM)&cr);
        nStartChar = cr.cpMin;
        nEndChar = cr.cpMax;
    }
}

int CRichEditCtrl::GetTextLength() const {
    if (!m_hWnd) return 0;
    GETTEXTLENGTHEX gtl = {};
    gtl.flags = GTL_DEFAULT | GTL_NUMCHARS;
    gtl.codepage = 1200; // UTF-16LE
    return (int)::SendMessageW(m_hWnd, EM_GETTEXTLENGTHEX, (WPARAM)&gtl, 0);
}

void CRichEditCtrl::ReplaceSel(const wchar_t* lpszNewText, BOOL bCanUndo) {
    if (m_hWnd) ::SendMessageW(m_hWnd, EM_REPLACESEL, (WPARAM)bCanUndo, (LPARAM)lpszNewText);
}

int CRichEditCtrl::GetLine(int nIndex, wchar_t* lpszBuffer, int nMaxLength) const {
    if (!m_hWnd || !lpszBuffer) return 0;
    *(WORD*)lpszBuffer = (WORD)nMaxLength;
    return (int)::SendMessageW(m_hWnd, EM_GETLINE, nIndex, (LPARAM)lpszBuffer);
}

int CRichEditCtrl::LineIndex(int nLine) const {
    if (!m_hWnd) return 0;
    return (int)::SendMessageW(m_hWnd, EM_LINEINDEX, nLine, 0);
}

int CRichEditCtrl::LineFromChar(int nIndex) const {
    if (!m_hWnd) return 0;
    return (int)::SendMessageW(m_hWnd, EM_EXLINEFROMCHAR, 0, nIndex);
}

int CRichEditCtrl::LineLength(int nLine) const {
    if (!m_hWnd) return 0;
    return (int)::SendMessageW(m_hWnd, EM_LINELENGTH, LineIndex(nLine), 0);
}

int CRichEditCtrl::GetLineCount() const {
    if (!m_hWnd) return 0;
    return (int)::SendMessageW(m_hWnd, EM_GETLINECOUNT, 0, 0);
}

BOOL CRichEditCtrl::CanUndo() const {
    return m_hWnd ? (BOOL)::SendMessageW(m_hWnd, EM_CANUNDO, 0, 0) : FALSE;
}

BOOL CRichEditCtrl::CanRedo() const {
    return m_hWnd ? (BOOL)::SendMessageW(m_hWnd, EM_CANREDO, 0, 0) : FALSE;
}

BOOL CRichEditCtrl::Undo() {
    if (!m_hWnd) return FALSE;
    return (BOOL)::SendMessageW(m_hWnd, EM_UNDO, 0, 0);
}

BOOL CRichEditCtrl::Redo() {
    if (!m_hWnd) return FALSE;
    return (BOOL)::SendMessageW(m_hWnd, EM_REDO, 0, 0);
}

void CRichEditCtrl::EmptyUndoBuffer() {
    if (m_hWnd) ::SendMessageW(m_hWnd, EM_EMPTYUNDOBUFFER, 0, 0);
}

void CRichEditCtrl::Cut() {
    if (m_hWnd) ::SendMessageW(m_hWnd, WM_CUT, 0, 0);
}

void CRichEditCtrl::Copy() {
    if (m_hWnd) ::SendMessageW(m_hWnd, WM_COPY, 0, 0);
}

void CRichEditCtrl::Paste() {
    if (m_hWnd) ::SendMessageW(m_hWnd, WM_PASTE, 0, 0);
}

void CRichEditCtrl::Clear() {
    if (m_hWnd) ::SendMessageW(m_hWnd, WM_CLEAR, 0, 0);
}

LONG CRichEditCtrl::StreamIn(int nFormat, EDITSTREAM& es) {
    if (!m_hWnd) return 0;
    return (LONG)::SendMessageW(m_hWnd, EM_STREAMIN, nFormat, (LPARAM)&es);
}

LONG CRichEditCtrl::StreamOut(int nFormat, EDITSTREAM& es) {
    if (!m_hWnd) return 0;
    return (LONG)::SendMessageW(m_hWnd, EM_STREAMOUT, nFormat, (LPARAM)&es);
}

LONG CRichEditCtrl::FindText(DWORD dwFlags, FINDTEXTEXW& ft) const {
    if (!m_hWnd) return -1;
    return (LONG)::SendMessageW(m_hWnd, EM_FINDTEXTEXW, dwFlags, (LPARAM)&ft);
}

BOOL CRichEditCtrl::SetReadOnly(BOOL bReadOnly) {
    if (!m_hWnd) return FALSE;
    return (BOOL)::SendMessageW(m_hWnd, EM_SETREADONLY, bReadOnly, 0);
}

BOOL CRichEditCtrl::IsModify() const {
    return m_hWnd ? (BOOL)::SendMessageW(m_hWnd, EM_GETMODIFY, 0, 0) : FALSE;
}

void CRichEditCtrl::SetModify(BOOL bModified) {
    if (m_hWnd) ::SendMessageW(m_hWnd, EM_SETMODIFY, bModified, 0);
}

DWORD CRichEditCtrl::SetEventMask(DWORD dwEventMask) {
    if (!m_hWnd) return 0;
    return (DWORD)::SendMessageW(m_hWnd, EM_SETEVENTMASK, 0, dwEventMask);
}

DWORD CRichEditCtrl::GetEventMask() const {
    if (!m_hWnd) return 0;
    return (DWORD)::SendMessageW(m_hWnd, EM_GETEVENTMASK, 0, 0);
}

void CRichEditCtrl::SetMargins(UINT nLeft, UINT nRight) {
    if (m_hWnd) {
        ::SendMessageW(m_hWnd, EM_SETMARGINS, EC_LEFTMARGIN | EC_RIGHTMARGIN,
                       MAKELPARAM(nLeft, nRight));
    }
}

//=============================================================================
// CRichEditView
//=============================================================================
IMPLEMENT_DYNCREATE(CRichEditView, CView)

CRichEditView::CRichEditView()
    : m_nWordWrapMode(0) {
    m_sizePaper.cx = 12240;  // 8.5 inches in twips
    m_sizePaper.cy = 15840;  // 11 inches in twips
    m_rectMargins.SetRect(1440, 1440, 1440, 1440);  // 1 inch margins in twips
    memset(_richeditview_padding, 0, sizeof(_richeditview_padding));
}

CRichEditView::~CRichEditView() {
}

BOOL CRichEditView::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName,
                           DWORD dwStyle, const RECT& rect, CWnd* pParentWnd,
                           UINT nID, CCreateContext* pContext) {
    if (!CView::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext))
        return FALSE;

    // Create the embedded rich edit control
    CRect rcClient;
    ::GetClientRect(m_hWnd, (RECT*)&rcClient);
    m_richEdit.Create(WS_CHILD | WS_VISIBLE | ES_MULTILINE | ES_AUTOVSCROLL |
                      ES_AUTOHSCROLL | ES_NOHIDESEL | ES_SAVESEL | ES_SELECTIONBAR,
                      *(const RECT*)&rcClient, this, AFX_IDW_PANE_FIRST);

    return TRUE;
}

CRichEditCtrl& CRichEditView::GetRichEditCtrl() const {
    return const_cast<CRichEditView*>(this)->m_richEdit;
}

void CRichEditView::SetCharFormat(const CHARFORMAT2W& cf) {
    m_richEdit.SetSelectionCharFormat(cf);
}

void CRichEditView::GetCharFormat(CHARFORMAT2W& cf) const {
    m_richEdit.GetSelectionCharFormat(cf);
}

void CRichEditView::SetParaFormat(const PARAFORMAT2& pf) {
    m_richEdit.SetParaFormat(pf);
}

void CRichEditView::GetParaFormat(PARAFORMAT2& pf) const {
    m_richEdit.GetParaFormat(pf);
}

void CRichEditView::PrintInsideRect(CDC* pDC, RECT& rectLayout,
                                    LONG nIndexStart, LONG nIndexEnd,
                                    BOOL bOutput) {
    if (!m_richEdit.m_hWnd || !pDC || !pDC->m_hDC) return;

    FORMATRANGE fr = {};
    fr.hdc = pDC->m_hDC;
    fr.hdcTarget = pDC->m_hDC;
    fr.rc = rectLayout;
    fr.rcPage = rectLayout;
    fr.chrg.cpMin = nIndexStart;
    fr.chrg.cpMax = nIndexEnd;

    if (bOutput) {
        ::SendMessageW(m_richEdit.m_hWnd, EM_FORMATRANGE, TRUE, (LPARAM)&fr);
    } else {
        // Measure only - don't render
        ::SendMessageW(m_richEdit.m_hWnd, EM_FORMATRANGE, FALSE, (LPARAM)&fr);
    }

    ::SendMessageW(m_richEdit.m_hWnd, EM_FORMATRANGE, 0, 0);  // Clear cache
}

LONG CRichEditView::PrintPage(CDC* pDC, LONG nIndexStart, LONG nIndexEnd) {
    if (!pDC) return nIndexStart;

    CRect rcPage = GetPageRect();
    CRect rcPrint = GetPrintRect();
    (void)nIndexEnd;

    FORMATRANGE fr = {};
    fr.hdc = pDC->m_hDC;
    fr.hdcTarget = pDC->m_hDC;
    fr.rc = *(RECT*)&rcPrint;
    fr.rcPage = *(RECT*)&rcPage;
    fr.chrg.cpMin = nIndexStart;
    fr.chrg.cpMax = -1;  // Print all remaining text

    LONG nNewStart = (LONG)::SendMessageW(m_richEdit.m_hWnd, EM_FORMATRANGE, TRUE, (LPARAM)&fr);
    ::SendMessageW(m_richEdit.m_hWnd, EM_FORMATRANGE, 0, 0);
    return nNewStart;
}

void CRichEditView::PrintReplaceSel(const wchar_t* lpszNewText, BOOL bCanUndo) {
    m_richEdit.ReplaceSel(lpszNewText, bCanUndo);
}

BOOL CRichEditView::FindText(const wchar_t* lpszFind, BOOL bNext, BOOL bCase) {
    return FindTextSimple(lpszFind, bNext, bCase);
}

BOOL CRichEditView::FindTextSimple(const wchar_t* lpszFind, BOOL bNext, BOOL bCase) {
    if (!lpszFind || !m_richEdit.m_hWnd) return FALSE;

    FINDTEXTEXW ft = {};
    GetFindString(lpszFind, ft);

    DWORD dwFlags = bCase ? FR_MATCHCASE : 0;
    if (!bNext) dwFlags |= FR_DOWN;
    else dwFlags |= FR_DOWN;  // Search forward from current position

    // Set search range from current selection
    int nStart, nEnd;
    m_richEdit.GetSel(nStart, nEnd);
    int nLength = m_richEdit.GetTextLength();

    if (bNext) {
        ft.chrg.cpMin = nEnd;
        ft.chrg.cpMax = nLength;
    } else {
        ft.chrg.cpMin = nStart;
        ft.chrg.cpMax = 0;
    }

    LONG nFound = m_richEdit.FindText(dwFlags, ft);
    if (nFound >= 0) {
        m_richEdit.SetSel((int)nFound, (int)(nFound + (LONG)wcslen(lpszFind)));
        return TRUE;
    }

    // Wrap around
    if (bNext) {
        ft.chrg.cpMin = 0;
        ft.chrg.cpMax = nStart;
    } else {
        ft.chrg.cpMin = nLength;
        ft.chrg.cpMax = nEnd;
    }

    nFound = m_richEdit.FindText(dwFlags, ft);
    if (nFound >= 0) {
        m_richEdit.SetSel((int)nFound, (int)(nFound + (LONG)wcslen(lpszFind)));
        return TRUE;
    }

    return FALSE;
}

LONG CRichEditView::GetFindString(const wchar_t* lpszFind, FINDTEXTEXW& ft) const {
    memset(&ft, 0, sizeof(ft));
    ft.lpstrText = (LPWSTR)lpszFind;
    return 0;
}

void CRichEditView::OnInitialUpdate() {
    CView::OnInitialUpdate();
    // Reset modification flag after initial load
    if (m_richEdit.m_hWnd) {
        m_richEdit.SetModify(FALSE);
    }
}

void CRichEditView::OnDraw(void* pDC) {
    if (m_richEdit.m_hWnd) {
        CRect rcClient;
        ::GetClientRect(m_hWnd, (RECT*)&rcClient);
        FORMATRANGE fr = {};
        fr.hdc = ((CDC*)pDC)->m_hDC;
        fr.hdcTarget = ((CDC*)pDC)->m_hDC;
        fr.rc = *(RECT*)&rcClient;
        fr.rcPage = *(RECT*)&rcClient;
        fr.chrg.cpMin = 0;
        fr.chrg.cpMax = -1;
        ::SendMessageW(m_richEdit.m_hWnd, EM_FORMATRANGE, TRUE, (LPARAM)&fr);
        ::SendMessageW(m_richEdit.m_hWnd, EM_FORMATRANGE, 0, 0);
    }
}

void CRichEditView::Serialize(CArchive& ar) {
    if (ar.IsStoring()) {
        // Save as text via EM_STREAMOUT
        EDITSTREAM es = {};
        es.pfnCallback = [](DWORD_PTR dwCookie, PBYTE pbBuff, LONG cb, PLONG pcb) -> DWORD {
            CArchive* pAr = (CArchive*)dwCookie;
            pAr->Write(pbBuff, cb);
            *pcb = cb;
            return 0;
        };
        es.dwCookie = (DWORD_PTR)&ar;
        m_richEdit.StreamOut(SF_TEXT, es);
    } else {
        // Load from archive
        EDITSTREAM es = {};
        es.pfnCallback = [](DWORD_PTR dwCookie, PBYTE pbBuff, LONG cb, PLONG pcb) -> DWORD {
            CArchive* pAr = (CArchive*)dwCookie;
            UINT nRead = pAr->Read(pbBuff, cb);
            *pcb = (LONG)nRead;
            return (nRead < (UINT)cb) ? 1 : 0;
        };
        es.dwCookie = (DWORD_PTR)&ar;
        m_richEdit.StreamIn(SF_TEXT, es);
    }
}

void CRichEditView::WrapChanged() {
    if (!m_richEdit.m_hWnd) return;
    // Recalculate layout after word wrap change
    m_richEdit.SetModify(TRUE);
    ::InvalidateRect(m_richEdit.m_hWnd, nullptr, TRUE);
}

WORD CRichEditView::GetWordWrapMode() const {
    return m_nWordWrapMode;
}

void CRichEditView::SetWordWrapMode(WORD nWordWrap) {
    m_nWordWrapMode = nWordWrap;
    WrapChanged();
}

void CRichEditView::SetPaperSize(CSize sizePaper) {
    m_sizePaper = sizePaper;
}

CSize CRichEditView::GetPaperSize() const {
    return m_sizePaper;
}

void CRichEditView::SetMargins(const CRect& rectMargins) {
    m_rectMargins = rectMargins;
}

CRect CRichEditView::GetMargins() const {
    return m_rectMargins;
}

CRect CRichEditView::GetPrintRect() const {
    CRect rc;
    rc.left = m_rectMargins.left;
    rc.top = m_rectMargins.top;
    rc.right = m_sizePaper.cx - m_rectMargins.right;
    rc.bottom = m_sizePaper.cy - m_rectMargins.bottom;
    return rc;
}

CRect CRichEditView::GetPageRect() const {
    return CRect(0, 0, m_sizePaper.cx, m_sizePaper.cy);
}
