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
#include <mshtml.h>

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
IMPLEMENT_DYNAMIC(CRichEditCtrl, CWnd)

CRichEditCtrl::CRichEditCtrl() {
    memset(_richeditctrl_padding, 0, sizeof(_richeditctrl_padding));
}

CRichEditCtrl::~CRichEditCtrl() {
    if (m_hWnd) ::DestroyWindow(m_hWnd);
}

BOOL CRichEditCtrl::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID) {
    return CreateEx(0, dwStyle, rect, pParentWnd, nID);
}

BOOL CRichEditCtrl::CreateEx(DWORD dwExStyle, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID) {
    if (!pParentWnd) return FALSE;
    m_hWnd = ::CreateWindowExW(dwExStyle, g_pszRichEditClass, nullptr,
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

int CRichEditCtrl::CanPaste(UINT nFormat) const {
    if (!m_hWnd) return FALSE;
    return (int)::SendMessageW(m_hWnd, EM_CANPASTE, nFormat, 0);
}

void CRichEditCtrl::Copy() {
    if (m_hWnd) ::SendMessageW(m_hWnd, WM_COPY, 0, 0);
}

void CRichEditCtrl::Paste() {
    if (m_hWnd) ::SendMessageW(m_hWnd, WM_PASTE, 0, 0);
}

void CRichEditCtrl::PasteSpecial(UINT nClipFormat, DWORD dwAspect, HMETAFILE hMF) {
    if (!m_hWnd) return;
    REPASTESPECIAL rps = {};
    rps.dwAspect = dwAspect;
    rps.dwParam = reinterpret_cast<DWORD_PTR>(hMF);
    ::SendMessageW(m_hWnd, EM_PASTESPECIAL, nClipFormat, (LPARAM)&rps);
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

IRichEditOle* CRichEditCtrl::GetIRichEditOle() const {
    if (!m_hWnd) return nullptr;
    IRichEditOle* pOle = nullptr;
    ::SendMessageW(m_hWnd, EM_GETOLEINTERFACE, 0, (LPARAM)&pOle);
    return pOle;
}

LONG CRichEditCtrl::FindText(DWORD dwFlags, FINDTEXTEXW& ft) const {
    if (!m_hWnd) return -1;
    return (LONG)::SendMessageW(m_hWnd, EM_FINDTEXTEXW, dwFlags, (LPARAM)&ft);
}

CString CRichEditCtrl::GetSelText() const {
    CString str;
    if (!m_hWnd) return str;

    int nStart = 0, nEnd = 0;
    GetSel(nStart, nEnd);
    if (nEnd <= nStart) return str;

    int nMax = (nEnd - nStart) + 1;
    wchar_t* pBuf = str.GetBuffer(nMax);
    int nCopied = (int)::SendMessageW(m_hWnd, EM_GETSELTEXT, 0, (LPARAM)pBuf);
    if (nCopied < 0) nCopied = 0;
    str.ReleaseBuffer(nCopied);
    return str;
}

LONG CRichEditCtrl::GetTextLengthEx(DWORD dwFlags, UINT uCodePage) const {
    if (!m_hWnd) return 0;
    GETTEXTLENGTHEX gtl = {};
    gtl.flags = dwFlags;
    gtl.codepage = uCodePage;
    return (LONG)::SendMessageW(m_hWnd, EM_GETTEXTLENGTHEX, (WPARAM)&gtl, 0);
}

int CRichEditCtrl::GetTextRange(int nFirst, int nLast, CString& refString) const {
    refString.Empty();
    if (!m_hWnd || nLast <= nFirst) return 0;

    TEXTRANGEW tr = {};
    tr.chrg.cpMin = nFirst;
    tr.chrg.cpMax = nLast;

    int nMax = (nLast - nFirst) + 1;
    wchar_t* pBuf = refString.GetBuffer(nMax);
    tr.lpstrText = pBuf;
    int nCopied = (int)::SendMessageW(m_hWnd, EM_GETTEXTRANGE, 0, (LPARAM)&tr);
    if (nCopied < 0) nCopied = 0;
    refString.ReleaseBuffer(nCopied);
    return nCopied;
}

void CRichEditCtrl::LineScroll(int nLines, int nChars) {
    if (m_hWnd) ::SendMessageW(m_hWnd, EM_LINESCROLL, nChars, nLines);
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

int CRichEditCtrl::SetWordCharFormat(CHARFORMAT2W& cf) {
    if (!m_hWnd) return FALSE;
    return (int)::SendMessageW(m_hWnd, EM_SETCHARFORMAT, SCF_SELECTION | SCF_WORD, (LPARAM)&cf);
}

int CRichEditCtrl::SetWordCharFormat(CHARFORMATW& cf) {
    if (!m_hWnd) return FALSE;
    return (int)::SendMessageW(m_hWnd, EM_SETCHARFORMAT, SCF_SELECTION | SCF_WORD, (LPARAM)&cf);
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

int CRichEditView::FindTextW(const wchar_t* lpszFind, int bNext, int bCase, int bWholeWord) {
    if (!lpszFind || !m_richEdit.m_hWnd) return FALSE;

    FINDTEXTEXW ft = {};
    ft.lpstrText = (LPWSTR)lpszFind;
    int nStart = 0, nEnd = 0;
    m_richEdit.GetSel(nStart, nEnd);
    int nLength = m_richEdit.GetTextLength();
    BOOL bForward = (bNext != FALSE);

    DWORD dwFlags = 0;
    if (bCase) dwFlags |= FR_MATCHCASE;
    if (bWholeWord) dwFlags |= FR_WHOLEWORD;
    if (bForward) dwFlags |= FR_DOWN;

    if (bForward) {
        ft.chrg.cpMin = nEnd;
        ft.chrg.cpMax = nLength;
    } else {
        ft.chrg.cpMin = nStart;
        ft.chrg.cpMax = 0;
    }

    LONG nFound = m_richEdit.FindText(dwFlags, ft);
    if (nFound < 0) {
        if (bForward) {
            ft.chrg.cpMin = 0;
            ft.chrg.cpMax = nStart;
        } else {
            ft.chrg.cpMin = nLength;
            ft.chrg.cpMax = nEnd;
        }
        nFound = m_richEdit.FindText(dwFlags, ft);
    }
    if (nFound < 0) return FALSE;

    m_richEdit.SetSel((int)nFound, (int)(nFound + (LONG)wcslen(lpszFind)));
    return TRUE;
}

LONG CRichEditView::GetFindString(const wchar_t* lpszFind, FINDTEXTEXW& ft) const {
    memset(&ft, 0, sizeof(ft));
    ft.lpstrText = (LPWSTR)lpszFind;
    return 0;
}

int CRichEditView::CanPaste() const {
    return m_richEdit.CanPaste(CF_UNICODETEXT);
}

CHARFORMAT2W& CRichEditView::GetCharFormatSelection() {
    static CHARFORMAT2W cf = {};
    cf.cbSize = sizeof(CHARFORMAT2W);
    m_richEdit.GetSelectionCharFormat(cf);
    return cf;
}

PARAFORMAT2& CRichEditView::GetParaFormatSelection() {
    static PARAFORMAT2 pf = {};
    pf.cbSize = sizeof(PARAFORMAT2);
    m_richEdit.GetParaFormat(pf);
    return pf;
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

void CRichEditView::Stream(CArchive& ar, int bSelection) {
    int nFormat = SF_TEXT;
    if (bSelection) nFormat |= SFF_SELECTION;

    EDITSTREAM es = {};
    if (ar.IsStoring()) {
        es.pfnCallback = [](DWORD_PTR dwCookie, PBYTE pbBuff, LONG cb, PLONG pcb) -> DWORD {
            CArchive* pAr = (CArchive*)dwCookie;
            pAr->Write(pbBuff, cb);
            *pcb = cb;
            return 0;
        };
        es.dwCookie = (DWORD_PTR)&ar;
        m_richEdit.StreamOut(nFormat, es);
    } else {
        es.pfnCallback = [](DWORD_PTR dwCookie, PBYTE pbBuff, LONG cb, PLONG pcb) -> DWORD {
            CArchive* pAr = (CArchive*)dwCookie;
            UINT nRead = pAr->Read(pbBuff, cb);
            *pcb = (LONG)nRead;
            return (nRead < (UINT)cb) ? 1 : 0;
        };
        es.dwCookie = (DWORD_PTR)&ar;
        m_richEdit.StreamIn(nFormat, es);
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

void CRichEditView::TextNotFound(const wchar_t* lpszFind) {
    (void)lpszFind;
}

void CRichEditView::OnTextNotFound(const wchar_t* lpszFind) {
    TextNotFound(lpszFind);
}

//=============================================================================
// CHtmlView - WebBrowser-based HTML View
//=============================================================================

// Minimal IOleClientSite for hosting the WebBrowser control
class CHtmlViewClientSite : public IOleClientSite, public IOleInPlaceSite {
public:
    CHtmlViewClientSite(HWND hWnd) : m_hWnd(hWnd), m_refCount(1), m_pBrowser(nullptr) {}
    virtual ~CHtmlViewClientSite() {}

    // IUnknown
    STDMETHOD(QueryInterface)(REFIID riid, void** ppv) override {
        if (riid == IID_IUnknown || riid == IID_IOleClientSite) {
            *ppv = static_cast<IOleClientSite*>(this);
        } else if (riid == IID_IOleInPlaceSite) {
            *ppv = static_cast<IOleInPlaceSite*>(this);
        } else {
            *ppv = nullptr;
            return E_NOINTERFACE;
        }
        AddRef();
        return S_OK;
    }
    STDMETHOD_(ULONG, AddRef)() override { return ++m_refCount; }
    STDMETHOD_(ULONG, Release)() override {
        if (--m_refCount == 0) { delete this; return 0; }
        return m_refCount;
    }

    // IOleClientSite
    STDMETHOD(SaveObject)() override { return E_NOTIMPL; }
    STDMETHOD(GetMoniker)(DWORD, DWORD, IMoniker**) override { return E_NOTIMPL; }
    STDMETHOD(GetContainer)(IOleContainer** ppContainer) override { *ppContainer = nullptr; return E_NOINTERFACE; }
    STDMETHOD(ShowObject)() override { return S_OK; }
    STDMETHOD(OnShowWindow)(BOOL) override { return S_OK; }
    STDMETHOD(RequestNewObjectLayout)() override { return E_NOTIMPL; }

    // IOleInPlaceSite
    STDMETHOD(GetWindow)(HWND* phWnd) override { *phWnd = m_hWnd; return S_OK; }
    STDMETHOD(ContextSensitiveHelp)(BOOL) override { return E_NOTIMPL; }
    STDMETHOD(CanInPlaceActivate)() override { return S_OK; }
    STDMETHOD(OnInPlaceActivate)() override { return S_OK; }
    STDMETHOD(OnUIActivate)() override { return S_OK; }
    STDMETHOD(GetWindowContext)(IOleInPlaceFrame** ppFrame, IOleInPlaceUIWindow** ppDoc,
                                 LPRECT lprcPosRect, LPRECT lprcClipRect, LPOLEINPLACEFRAMEINFO lpFrameInfo) override {
        *ppFrame = nullptr;
        *ppDoc = nullptr;
        ::GetClientRect(m_hWnd, lprcPosRect);
        ::GetClientRect(m_hWnd, lprcClipRect);
        if (lpFrameInfo) {
            lpFrameInfo->cb = sizeof(OLEINPLACEFRAMEINFO);
            lpFrameInfo->fMDIApp = FALSE;
            lpFrameInfo->hwndFrame = ::GetParent(m_hWnd);
            lpFrameInfo->haccel = nullptr;
            lpFrameInfo->cAccelEntries = 0;
        }
        return S_OK;
    }
    STDMETHOD(Scroll)(SIZE) override { return S_OK; }
    STDMETHOD(OnUIDeactivate)(BOOL) override { return S_OK; }
    STDMETHOD(OnInPlaceDeactivate)() override { return S_OK; }
    STDMETHOD(DiscardUndoState)() override { return S_OK; }
    STDMETHOD(DeactivateAndUndo)() override { return E_NOTIMPL; }
    STDMETHOD(OnPosRectChange)(LPCRECT lprcPosRect) override {
        if (m_pBrowser) {
            m_pBrowser->put_Left(lprcPosRect->left);
            m_pBrowser->put_Top(lprcPosRect->top);
            m_pBrowser->put_Width(lprcPosRect->right - lprcPosRect->left);
            m_pBrowser->put_Height(lprcPosRect->bottom - lprcPosRect->top);
        }
        return S_OK;
    }

    void SetBrowser(IWebBrowser2* pBrowser) { m_pBrowser = pBrowser; }

private:
    HWND m_hWnd;
    ULONG m_refCount;
    IWebBrowser2* m_pBrowser;
};

IMPLEMENT_DYNCREATE(CHtmlView, CView)

CHtmlView::CHtmlView()
    : m_pBrowser(nullptr), m_pControlWnd(nullptr), m_bCreated(FALSE) {
    memset(_htmlview_padding, 0, sizeof(_htmlview_padding));
}

CHtmlView::~CHtmlView() {
    if (m_pBrowser) {
        m_pBrowser->Stop();
        m_pBrowser->put_Visible(VARIANT_FALSE);
        m_pBrowser->Release();
        m_pBrowser = nullptr;
    }
}

BOOL CHtmlView::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName,
                       DWORD dwStyle, const RECT& rect, CWnd* pParentWnd,
                       UINT nID, CCreateContext* pContext) {
    if (!CView::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext))
        return FALSE;

    // Create WebBrowser control via COM
    HRESULT hr = CoCreateInstance(CLSID_WebBrowser, nullptr, CLSCTX_INPROC_SERVER,
                                   IID_IWebBrowser2, (void**)&m_pBrowser);
    if (FAILED(hr) || !m_pBrowser) return FALSE;

    // Set up client site for in-place activation
    CHtmlViewClientSite* pSite = new CHtmlViewClientSite(m_hWnd);
    pSite->SetBrowser(m_pBrowser);

    IOleObject* pOleObject = nullptr;
    hr = m_pBrowser->QueryInterface(IID_IOleObject, (void**)&pOleObject);
    if (SUCCEEDED(hr)) {
        pOleObject->SetClientSite(pSite);
        OleSetContainedObject(pOleObject, TRUE);

        RECT rcClient;
        ::GetClientRect(m_hWnd, &rcClient);
        pOleObject->DoVerb(OLEIVERB_INPLACEACTIVATE, nullptr, pSite, 0, m_hWnd, &rcClient);
        pOleObject->Release();
    }

    // Release site (browser holds a ref via AddRef)
    pSite->Release();

    m_pBrowser->put_Visible(VARIANT_TRUE);
    m_bCreated = TRUE;
    return TRUE;
}

void CHtmlView::OnDraw(void* pDC) {
    // WebBrowser renders itself
    (void)pDC;
}

void CHtmlView::Navigate(const wchar_t* lpszURL, DWORD dwFlags,
                         const wchar_t* lpszTargetFrameName,
                         const wchar_t* lpszHeaders,
                         void* lpvPostData, DWORD dwPostDataLen) {
    if (!m_pBrowser) return;
    VARIANT vFlags, vTarget, vPostData, vHeaders;
    VariantInit(&vFlags); VariantInit(&vTarget); VariantInit(&vPostData); VariantInit(&vHeaders);

    vFlags.vt = VT_I4; vFlags.lVal = dwFlags;

    if (lpszTargetFrameName) { vTarget.vt = VT_BSTR; vTarget.bstrVal = SysAllocString(lpszTargetFrameName); }
    if (lpszHeaders) { vHeaders.vt = VT_BSTR; vHeaders.bstrVal = SysAllocString(lpszHeaders); }
    if (lpvPostData && dwPostDataLen > 0) {
        vPostData.vt = VT_ARRAY | VT_UI1;
        SAFEARRAYBOUND sab = { dwPostDataLen, 0 };
        SAFEARRAY* psa = SafeArrayCreate(VT_UI1, 1, &sab);
        if (psa) {
            void* pvData = nullptr;
            SafeArrayAccessData(psa, &pvData);
            if (pvData) memcpy(pvData, lpvPostData, dwPostDataLen);
            SafeArrayUnaccessData(psa);
            vPostData.parray = psa;
        }
    }

    BSTR bstrUrl = SysAllocString(lpszURL);
    m_pBrowser->Navigate(bstrUrl, &vFlags, &vTarget, &vPostData, &vHeaders);
    SysFreeString(bstrUrl);

    VariantClear(&vFlags); VariantClear(&vTarget); VariantClear(&vPostData); VariantClear(&vHeaders);
}

void CHtmlView::Navigate2(LPITEMIDLIST pIDL, DWORD dwFlags, const wchar_t* lpszTargetFrameName) {
    if (!m_pBrowser) return;
    VARIANT vFlags, vTarget, vEmpty;
    VariantInit(&vFlags); VariantInit(&vTarget); VariantInit(&vEmpty);
    vFlags.vt = VT_I4; vFlags.lVal = dwFlags;
    if (lpszTargetFrameName) { vTarget.vt = VT_BSTR; vTarget.bstrVal = SysAllocString(lpszTargetFrameName); }
    // For PIDL navigation: pass empty URL; PIDL needs CoTaskMemAlloc + ILSaveToStream
    (void)pIDL;
    VARIANT vUrl;
    vUrl.vt = VT_BSTR;
    vUrl.bstrVal = SysAllocString(L"");
    m_pBrowser->Navigate2(&vUrl, &vFlags, &vTarget, &vEmpty, &vEmpty);
    VariantClear(&vUrl);
    VariantClear(&vFlags); VariantClear(&vTarget); VariantClear(&vEmpty);
}

void CHtmlView::Navigate2(const wchar_t* lpszURL, DWORD dwFlags,
                          const wchar_t* lpszTargetFrameName,
                          const wchar_t* lpszHeaders,
                          void* lpvPostData, DWORD dwPostDataLen) {
    if (!m_pBrowser) return;
    VARIANT vFlags, vTarget, vPostData, vHeaders;
    VariantInit(&vFlags); VariantInit(&vTarget); VariantInit(&vPostData); VariantInit(&vHeaders);
    vFlags.vt = VT_I4; vFlags.lVal = dwFlags;
    if (lpszTargetFrameName) { vTarget.vt = VT_BSTR; vTarget.bstrVal = SysAllocString(lpszTargetFrameName); }
    if (lpszHeaders) { vHeaders.vt = VT_BSTR; vHeaders.bstrVal = SysAllocString(lpszHeaders); }
    VARIANT vUrl;
    vUrl.vt = VT_BSTR;
    vUrl.bstrVal = SysAllocString(lpszURL);
    m_pBrowser->Navigate2(&vUrl, &vFlags, &vTarget, &vPostData, &vHeaders);
    VariantClear(&vUrl);
    VariantClear(&vFlags); VariantClear(&vTarget); VariantClear(&vPostData); VariantClear(&vHeaders);
    (void)lpvPostData; (void)dwPostDataLen;
}

void CHtmlView::GoBack() { if (m_pBrowser) m_pBrowser->GoBack(); }
void CHtmlView::GoForward() { if (m_pBrowser) m_pBrowser->GoForward(); }
void CHtmlView::GoHome() { if (m_pBrowser) m_pBrowser->GoHome(); }
void CHtmlView::GoSearch() { if (m_pBrowser) m_pBrowser->GoSearch(); }
void CHtmlView::Stop() { if (m_pBrowser) m_pBrowser->Stop(); }
void CHtmlView::Refresh() { if (m_pBrowser) m_pBrowser->Refresh(); }
void CHtmlView::Refresh2(int nLevel) { if (m_pBrowser) { VARIANT v; v.vt = VT_I4; v.lVal = nLevel; m_pBrowser->Refresh2(&v); } }

BOOL CHtmlView::GetBusy() const {
    if (!m_pBrowser) return FALSE;
    VARIANT_BOOL b = VARIANT_FALSE;
    m_pBrowser->get_Busy(&b);
    return b != VARIANT_FALSE;
}

long CHtmlView::GetReadyState() const {
    if (!m_pBrowser) return 0;
    READYSTATE rs = READYSTATE_UNINITIALIZED;
    m_pBrowser->get_ReadyState(&rs);
    return (long)rs;
}

CString CHtmlView::GetLocationName() const {
    CString str;
    if (m_pBrowser) {
        BSTR bstr = nullptr;
        if (SUCCEEDED(m_pBrowser->get_LocationName(&bstr)) && bstr) {
            str = bstr;
            SysFreeString(bstr);
        }
    }
    return str;
}

CString CHtmlView::GetLocationURL() const {
    CString str;
    if (m_pBrowser) {
        BSTR bstr = nullptr;
        if (SUCCEEDED(m_pBrowser->get_LocationURL(&bstr)) && bstr) {
            str = bstr;
            SysFreeString(bstr);
        }
    }
    return str;
}

CString CHtmlView::GetFullName() const {
    CString str;
    if (m_pBrowser) {
        BSTR bstr = nullptr;
        if (SUCCEEDED(m_pBrowser->get_FullName(&bstr)) && bstr) {
            str = bstr;
            SysFreeString(bstr);
        }
    }
    return str;
}

CString CHtmlView::GetType() const {
    CString str;
    if (m_pBrowser) {
        BSTR bstr = nullptr;
        if (SUCCEEDED(m_pBrowser->get_Type(&bstr)) && bstr) {
            str = bstr;
            SysFreeString(bstr);
        }
    }
    return str;
}

LPDISPATCH CHtmlView::GetHtmlDocument() const {
    LPDISPATCH pDisp = nullptr;
    if (m_pBrowser) m_pBrowser->get_Document(&pDisp);
    return pDisp;
}

void CHtmlView::ExecWB(OLECMDID cmdID, OLECMDEXECOPT cmdexecopt,
                       VARIANT* pvaIn, VARIANT* pvaOut) {
    if (m_pBrowser) m_pBrowser->ExecWB(cmdID, cmdexecopt, pvaIn, pvaOut);
}

void CHtmlView::LoadFromResource(const wchar_t* lpszResource) {
    if (lpszResource && m_pBrowser) {
        CString strUrl = L"res://";
        // Try to get module path for res:// protocol
        wchar_t buf[MAX_PATH];
        if (::GetModuleFileNameW(AfxGetInstanceHandle(), buf, MAX_PATH)) {
            strUrl += buf;
            strUrl += L"/";
            strUrl += lpszResource;
            Navigate(strUrl);
        }
    }
}

void CHtmlView::LoadFromResource(UINT nRes) {
    wchar_t buf[32];
    swprintf(buf, 32, L"%u", nRes);
    LoadFromResource(buf);
}

void CHtmlView::Print() {
    if (m_pBrowser) {
        VARIANT vIn, vOut;
        VariantInit(&vIn); VariantInit(&vOut);
        m_pBrowser->ExecWB(OLECMDID_PRINT, OLECMDEXECOPT_DONTPROMPTUSER, &vIn, &vOut);
    }
}

void CHtmlView::PrintPreview() {
    if (m_pBrowser) {
        VARIANT vIn, vOut;
        VariantInit(&vIn); VariantInit(&vOut);
        m_pBrowser->ExecWB(OLECMDID_PRINTPREVIEW, OLECMDEXECOPT_DONTPROMPTUSER, &vIn, &vOut);
    }
}

// Event stubs (override in derived class)
void CHtmlView::OnBeforeNavigate2(LPDISPATCH, VARIANT*, VARIANT*, VARIANT*, VARIANT*, VARIANT*, BOOL*) {}
void CHtmlView::OnNavigateComplete2(LPDISPATCH, VARIANT*) {}
void CHtmlView::OnDocumentComplete(LPDISPATCH, VARIANT*) {}
void CHtmlView::OnProgressChange(long, long) {}
void CHtmlView::OnTitleChange(const wchar_t*) {}
void CHtmlView::OnStatusTextChange(const wchar_t*) {}

//=============================================================================
// CDHtmlDialog - DHTML-based Dialog
//=============================================================================
CDHtmlDialog::CDHtmlDialog()
    : m_nHtmlResID(0), m_pBrowser(nullptr), m_pCtrlWnd(nullptr), m_bCreated(FALSE) {
    memset(_dhtmldialog_padding, 0, sizeof(_dhtmldialog_padding));
}

CDHtmlDialog::CDHtmlDialog(UINT nIDTemplate, UINT nHtmlResID, CWnd* pParentWnd)
    : CDialog(nIDTemplate, pParentWnd), m_nHtmlResID(nHtmlResID),
      m_pBrowser(nullptr), m_pCtrlWnd(nullptr), m_bCreated(FALSE) {
    memset(_dhtmldialog_padding, 0, sizeof(_dhtmldialog_padding));
}

CDHtmlDialog::CDHtmlDialog(const wchar_t* lpszTemplateName, const wchar_t* lpszHtmlResID, CWnd* pParentWnd)
    : CDialog(lpszTemplateName, pParentWnd), m_nHtmlResID(0),
      m_pBrowser(nullptr), m_pCtrlWnd(nullptr), m_bCreated(FALSE) {
    if (lpszHtmlResID) m_strHtmlResID = lpszHtmlResID;
    memset(_dhtmldialog_padding, 0, sizeof(_dhtmldialog_padding));
}

CDHtmlDialog::~CDHtmlDialog() {
    if (m_pBrowser) {
        m_pBrowser->Stop();
        m_pBrowser->put_Visible(VARIANT_FALSE);
        m_pBrowser->Release();
        m_pBrowser = nullptr;
    }
}

BOOL CDHtmlDialog::Create(const wchar_t* lpszTemplateName, CWnd* pParentWnd) {
    return CDialog::Create(lpszTemplateName, pParentWnd);
}

BOOL CDHtmlDialog::OnInitDialog() {
    BOOL bRet = CDialog::OnInitDialog();

    // Create WebBrowser control in the dialog
    HRESULT hr = CoCreateInstance(CLSID_WebBrowser, nullptr, CLSCTX_INPROC_SERVER,
                                   IID_IWebBrowser2, (void**)&m_pBrowser);
    if (SUCCEEDED(hr) && m_pBrowser) {
        // Set up client site
        CHtmlViewClientSite* pSite = new CHtmlViewClientSite(m_hWnd);
        pSite->SetBrowser(m_pBrowser);

        IOleObject* pOleObject = nullptr;
        hr = m_pBrowser->QueryInterface(IID_IOleObject, (void**)&pOleObject);
        if (SUCCEEDED(hr)) {
            pOleObject->SetClientSite(pSite);
            OleSetContainedObject(pOleObject, TRUE);

            RECT rcClient;
            ::GetClientRect(m_hWnd, &rcClient);
            pOleObject->DoVerb(OLEIVERB_INPLACEACTIVATE, nullptr, pSite, 0, m_hWnd, &rcClient);
            pOleObject->Release();
        }
        pSite->Release();

        m_pBrowser->put_Visible(VARIANT_TRUE);
        m_bCreated = TRUE;

        // Load HTML if specified
        if (m_nHtmlResID)
            LoadFromResource(m_nHtmlResID);
        else if (!m_strHtmlResID.IsEmpty())
            LoadFromResource(m_strHtmlResID);
    }

    return bRet;
}

void CDHtmlDialog::LoadFromResource(UINT nHtmlResID) {
    if (m_pBrowser) {
        CString strUrl = L"res://";
        wchar_t buf[MAX_PATH];
        if (::GetModuleFileNameW(AfxGetInstanceHandle(), buf, MAX_PATH)) {
            strUrl += buf;
            strUrl += L"/";
            wchar_t numBuf[32];
            swprintf(numBuf, 32, L"%u", nHtmlResID);
            strUrl += numBuf;
            BSTR bstrUrl = SysAllocString(strUrl);
            m_pBrowser->Navigate(bstrUrl, nullptr, nullptr, nullptr, nullptr);
            SysFreeString(bstrUrl);
        }
    }
}

void CDHtmlDialog::LoadFromResource(const wchar_t* lpszHtmlResID) {
    if (m_pBrowser && lpszHtmlResID) {
        CString strUrl = L"res://";
        wchar_t buf[MAX_PATH];
        if (::GetModuleFileNameW(AfxGetInstanceHandle(), buf, MAX_PATH)) {
            strUrl += buf;
            strUrl += L"/";
            strUrl += lpszHtmlResID;
            BSTR bstrUrl = SysAllocString(strUrl);
            m_pBrowser->Navigate(bstrUrl, nullptr, nullptr, nullptr, nullptr);
            SysFreeString(bstrUrl);
        }
    }
}

void CDHtmlDialog::Navigate(const wchar_t* lpszURL, DWORD dwFlags,
                            const wchar_t* lpszTargetFrameName,
                            const wchar_t* lpszHeaders,
                            void* lpvPostData, DWORD dwPostDataLen) {
    if (!m_pBrowser) return;
    BSTR bstrUrl = SysAllocString(lpszURL);
    m_pBrowser->Navigate(bstrUrl, nullptr, nullptr, nullptr, nullptr);
    SysFreeString(bstrUrl);
    (void)dwFlags; (void)lpszTargetFrameName; (void)lpszHeaders;
    (void)lpvPostData; (void)dwPostDataLen;
}

long CDHtmlDialog::GetElement(const wchar_t* lpszElementId, IDispatch** ppDisp, int* pfCollection) {
    (void)pfCollection;
    if (!m_pBrowser || !lpszElementId || !ppDisp) return E_POINTER;
    *ppDisp = nullptr;

    LPDISPATCH pDocDisp = nullptr;
    HRESULT hr = m_pBrowser->get_Document(&pDocDisp);
    if (FAILED(hr) || !pDocDisp) return hr;

    IHTMLDocument3* pDoc3 = nullptr;
    hr = pDocDisp->QueryInterface(IID_IHTMLDocument3, (void**)&pDoc3);
    pDocDisp->Release();

    if (SUCCEEDED(hr) && pDoc3) {
        BSTR bstrId = SysAllocString(lpszElementId);
        IHTMLElement* pElem = nullptr;
        hr = pDoc3->getElementById(bstrId, &pElem);
        SysFreeString(bstrId);
        if (SUCCEEDED(hr) && pElem) {
            hr = pElem->QueryInterface(IID_IDispatch, (void**)ppDisp);
            pElem->Release();
        }
        pDoc3->Release();
    }

    return hr;
}

long CDHtmlDialog::GetElement(const wchar_t* lpszElementId, IHTMLElement** ppElement) {
    (void)lpszElementId;
    if (ppElement) *ppElement = nullptr;
    return E_NOTIMPL;
}

HRESULT CDHtmlDialog::SetElementProperty(const wchar_t* lpszElementId,
                                         DISPID dispId, VARIANT* pVar) {
    if (!m_pBrowser || !lpszElementId || !pVar) return E_POINTER;

    LPDISPATCH pDisp = nullptr;
    HRESULT hr = GetElement(lpszElementId, &pDisp, nullptr);
    if (FAILED(hr) || !pDisp) return hr;

    DISPPARAMS dp = { pVar, nullptr, 1, 0 };
    hr = pDisp->Invoke(dispId, IID_NULL, LOCALE_USER_DEFAULT,
                       DISPATCH_PROPERTYPUT, &dp, nullptr, nullptr, nullptr);
    pDisp->Release();
    return hr;
}

VARIANT CDHtmlDialog::GetElementProperty(const wchar_t* lpszElementId, long lCookie) {
    (void)lpszElementId; (void)lCookie;
    VARIANT v; VariantInit(&v); return v;
}

// Legacy overload
HRESULT CDHtmlDialog::GetElementProperty(const wchar_t* lpszElementId,
                                          DISPID dispId, VARIANT* pVar) {

    LPDISPATCH pDisp = nullptr;
    HRESULT hr = GetElement(lpszElementId, &pDisp);
    if (FAILED(hr) || !pDisp) return hr;

    DISPPARAMS dp = { nullptr, nullptr, 0, 0 };
    hr = pDisp->Invoke(dispId, IID_NULL, LOCALE_USER_DEFAULT,
                       DISPATCH_PROPERTYGET, &dp, pVar, nullptr, nullptr);
    pDisp->Release();
    return hr;
}

HRESULT CDHtmlDialog::OnDDXError(const wchar_t*, const wchar_t*) {
    return S_OK;
}
