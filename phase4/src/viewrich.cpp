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

// Symbol: ?GetSelText@CRichEditCtrl@@QEBA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@XZ
extern "C" void MS_ABI impl__GetSelText_CRichEditCtrl__QEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__XZ(
    CString* pRet, const CRichEditCtrl* pThis) {
    new (pRet) CString(pThis ? pThis->GetSelText() : CString());
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

// Symbol: ?GetTextRange@CRichEditCtrl@@QEBAHHHAEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@@Z
extern "C" int MS_ABI impl__GetTextRange_CRichEditCtrl__QEBAHHHAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
    const CRichEditCtrl* pThis, int nFirst, int nLast, CString* refString) {
    if (!refString) {
        return 0;
    }
    refString->Empty();
    return pThis ? pThis->GetTextRange(nFirst, nLast, *refString) : 0;
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

    m_richEdit.SetSel((int)ft.chrgText.cpMin, (int)ft.chrgText.cpMax);
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
    static thread_local CHARFORMAT2W cf = {};
    cf.cbSize = sizeof(CHARFORMAT2W);
    m_richEdit.GetSelectionCharFormat(cf);
    return cf;
}

PARAFORMAT2& CRichEditView::GetParaFormatSelection() {
    static thread_local PARAFORMAT2 pf = {};
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
            try {
                pAr->Write(pbBuff, cb);
                *pcb = cb;
                return 0;
            } catch (...) {
                *pcb = 0;
                return 1;
            }
        };
        es.dwCookie = (DWORD_PTR)&ar;
        m_richEdit.StreamOut(nFormat, es);
    } else {
        es.pfnCallback = [](DWORD_PTR dwCookie, PBYTE pbBuff, LONG cb, PLONG pcb) -> DWORD {
            CArchive* pAr = (CArchive*)dwCookie;
            try {
                UINT nRead = pAr->Read(pbBuff, cb);
                *pcb = (LONG)nRead;
                return (nRead < (UINT)cb) ? 1 : 0;
            } catch (...) {
                *pcb = 0;
                return 1;
            }
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

void CHtmlView::OnDraw(void* /*pDC*/) {
    // WebBrowser renders itself
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

READYSTATE CHtmlView::GetReadyState() const {
    if (!m_pBrowser) return READYSTATE_UNINITIALIZED;
    READYSTATE rs = READYSTATE_UNINITIALIZED;
    m_pBrowser->get_ReadyState(&rs);
    return rs;
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

BOOL CHtmlView::LoadFromResource(const wchar_t* lpszResource) {
    if (lpszResource && m_pBrowser) {
        CString strUrl = L"res://";
        // Try to get module path for res:// protocol
        wchar_t buf[MAX_PATH];
        if (::GetModuleFileNameW(AfxGetInstanceHandle(), buf, MAX_PATH)) {
            strUrl += buf;
            strUrl += L"/";
            strUrl += lpszResource;
            Navigate(strUrl);
            return TRUE;
        }
    }
    return FALSE;
}

BOOL CHtmlView::LoadFromResource(UINT nRes) {
    wchar_t buf[32];
    swprintf(buf, 32, L"%u", nRes);
    return LoadFromResource(buf);
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
// Note: OnBeforeNavigate2/OnNavigateComplete2/OnDocumentComplete take decoded string params
void CHtmlView::OnBeforeNavigate2(const wchar_t*, DWORD, const wchar_t*, CByteArray&, const wchar_t*, BOOL*) {}
void CHtmlView::OnNavigateComplete2(const wchar_t*) {}
void CHtmlView::OnDocumentComplete(const wchar_t*) {}
void CHtmlView::OnProgressChange(long, long) {}
void CHtmlView::OnTitleChange(const wchar_t*) {}
void CHtmlView::OnStatusTextChange(const wchar_t*) {}
void CHtmlView::OnCommandStateChange(long, BOOL) {}
void CHtmlView::OnDownloadBegin() {}
void CHtmlView::OnDownloadComplete() {}
void CHtmlView::OnFullScreen(BOOL) {}
void CHtmlView::OnMenuBar(BOOL) {}
void CHtmlView::OnNavigateError(const wchar_t*, const wchar_t*, DWORD, BOOL*) {}
void CHtmlView::OnNewWindow2(LPDISPATCH*, BOOL*) {}
void CHtmlView::OnPropertyChange(const wchar_t*) {}
void CHtmlView::OnQuit() {}
void CHtmlView::OnStatusBar(BOOL) {}
void CHtmlView::OnTheaterMode(BOOL) {}
void CHtmlView::OnToolBar(BOOL) {}
void CHtmlView::OnVisible(BOOL) {}
HRESULT CHtmlView::OnTranslateUrl(DWORD, wchar_t*, wchar_t**) { return S_FALSE; }
BOOL CHtmlView::GetSource(CString&) { return FALSE; }
HRESULT CHtmlView::OnGetOptionKeyPath(wchar_t**, DWORD) { return E_NOTIMPL; }

// IWebBrowser2 property accessors
LPDISPATCH CHtmlView::GetApplication() const {
    LPDISPATCH p = nullptr;
    if (m_pBrowser) m_pBrowser->get_Application(&p);
    return p;
}
LPDISPATCH CHtmlView::GetContainer() const {
    LPDISPATCH p = nullptr;
    if (m_pBrowser) m_pBrowser->get_Container(&p);
    return p;
}
LPDISPATCH CHtmlView::GetParentBrowser() const {
    LPDISPATCH p = nullptr;
    if (m_pBrowser) m_pBrowser->get_Parent(&p);
    return p;
}
long CHtmlView::GetLeft() const   { long v = 0; if (m_pBrowser) m_pBrowser->get_Left(&v);   return v; }
long CHtmlView::GetTop() const    { long v = 0; if (m_pBrowser) m_pBrowser->get_Top(&v);    return v; }
long CHtmlView::GetWidth() const  { long v = 0; if (m_pBrowser) m_pBrowser->get_Width(&v);  return v; }
long CHtmlView::GetHeight() const { long v = 0; if (m_pBrowser) m_pBrowser->get_Height(&v); return v; }

BOOL CHtmlView::GetVisible() const {
    VARIANT_BOOL b = VARIANT_FALSE;
    if (m_pBrowser) m_pBrowser->get_Visible(&b);
    return b != VARIANT_FALSE;
}
BOOL CHtmlView::GetFullScreen() const {
    VARIANT_BOOL b = VARIANT_FALSE;
    if (m_pBrowser) m_pBrowser->get_FullScreen(&b);
    return b != VARIANT_FALSE;
}
BOOL CHtmlView::GetMenuBar() const {
    VARIANT_BOOL b = VARIANT_FALSE;
    if (m_pBrowser) m_pBrowser->get_MenuBar(&b);
    return b != VARIANT_FALSE;
}
BOOL CHtmlView::GetAddressBar() const {
    VARIANT_BOOL b = VARIANT_FALSE;
    if (m_pBrowser) m_pBrowser->get_AddressBar(&b);
    return b != VARIANT_FALSE;
}
BOOL CHtmlView::GetStatusBar() const {
    VARIANT_BOOL b = VARIANT_FALSE;
    if (m_pBrowser) m_pBrowser->get_StatusBar(&b);
    return b != VARIANT_FALSE;
}
BOOL CHtmlView::GetToolBar() const {
    int v = 0;
    if (m_pBrowser) m_pBrowser->get_ToolBar(&v);
    return v != 0;
}
BOOL CHtmlView::GetOffline() const {
    VARIANT_BOOL b = VARIANT_FALSE;
    if (m_pBrowser) m_pBrowser->get_Offline(&b);
    return b != VARIANT_FALSE;
}
BOOL CHtmlView::GetSilent() const {
    VARIANT_BOOL b = VARIANT_FALSE;
    if (m_pBrowser) m_pBrowser->get_Silent(&b);
    return b != VARIANT_FALSE;
}
BOOL CHtmlView::GetTheaterMode() const {
    VARIANT_BOOL b = VARIANT_FALSE;
    if (m_pBrowser) m_pBrowser->get_TheaterMode(&b);
    return b != VARIANT_FALSE;
}
BOOL CHtmlView::GetRegisterAsBrowser() const {
    VARIANT_BOOL b = VARIANT_FALSE;
    if (m_pBrowser) m_pBrowser->get_RegisterAsBrowser(&b);
    return b != VARIANT_FALSE;
}
BOOL CHtmlView::GetRegisterAsDropTarget() const {
    VARIANT_BOOL b = VARIANT_FALSE;
    if (m_pBrowser) m_pBrowser->get_RegisterAsDropTarget(&b);
    return b != VARIANT_FALSE;
}
BOOL CHtmlView::GetTopLevelContainer() const {
    VARIANT_BOOL b = VARIANT_FALSE;
    if (m_pBrowser) m_pBrowser->get_TopLevelContainer(&b);
    return b != VARIANT_FALSE;
}
BOOL CHtmlView::GetProperty(const wchar_t* lpszProperty, CString& strValue) {
    if (!m_pBrowser || !lpszProperty) return FALSE;
    VARIANT v;
    VariantInit(&v);
    BSTR bstrProp = SysAllocString(lpszProperty);
    HRESULT hr = m_pBrowser->GetProperty(bstrProp, &v);
    SysFreeString(bstrProp);
    if (SUCCEEDED(hr) && v.vt == VT_BSTR && v.bstrVal) {
        strValue = v.bstrVal;
        VariantClear(&v);
        return TRUE;
    }
    VariantClear(&v);
    return FALSE;
}
void CHtmlView::PutProperty(const wchar_t* lpszProperty, const VARIANT& vtValue) {
    if (!m_pBrowser || !lpszProperty) return;
    BSTR bstrProp = SysAllocString(lpszProperty);
    m_pBrowser->PutProperty(bstrProp, const_cast<VARIANT&>(vtValue));
    SysFreeString(bstrProp);
}

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

BOOL CDHtmlDialog::LoadFromResource(UINT nHtmlResID) {
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
            return TRUE;
        }
    }
    return FALSE;
}

BOOL CDHtmlDialog::LoadFromResource(const wchar_t* lpszHtmlResID) {
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
            return TRUE;
        }
    }
    return FALSE;
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
    if (!ppElement) return E_POINTER;
    *ppElement = nullptr;
    if (!m_pBrowser || !lpszElementId) return E_POINTER;

    LPDISPATCH pDocDisp = nullptr;
    HRESULT hr = m_pBrowser->get_Document(&pDocDisp);
    if (FAILED(hr) || !pDocDisp) return hr ? hr : E_FAIL;

    IHTMLDocument3* pDoc3 = nullptr;
    hr = pDocDisp->QueryInterface(IID_IHTMLDocument3, (void**)&pDoc3);
    pDocDisp->Release();
    if (SUCCEEDED(hr) && pDoc3) {
        BSTR bstrId = SysAllocString(lpszElementId);
        hr = pDoc3->getElementById(bstrId, ppElement);
        SysFreeString(bstrId);
        pDoc3->Release();
    }
    return hr;
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

void CDHtmlDialog::GetCurrentUrl(CString& strUrl) {
    strUrl.Empty();
    if (m_pBrowser) {
        BSTR bstr = nullptr;
        if (SUCCEEDED(m_pBrowser->get_LocationURL(&bstr)) && bstr) {
            strUrl = bstr;
            SysFreeString(bstr);
        }
    }
}

long CDHtmlDialog::GetElementInterface(const wchar_t* lpszElementId, REFIID riid, void** ppUnk) {
    if (!ppUnk) return E_POINTER;
    *ppUnk = nullptr;
    IHTMLElement* pElem = nullptr;
    HRESULT hr = GetElement(lpszElementId, &pElem);
    if (SUCCEEDED(hr) && pElem) {
        hr = pElem->QueryInterface(riid, ppUnk);
        pElem->Release();
    }
    return hr;
}

wchar_t* CDHtmlDialog::GetElementText(const wchar_t* lpszElementId) {
    IHTMLElement* pElem = nullptr;
    if (FAILED(GetElement(lpszElementId, &pElem)) || !pElem) return nullptr;
    BSTR bstr = nullptr;
    pElem->get_innerText(&bstr);
    pElem->Release();
    return bstr; // caller responsible for SysFreeString
}

void CDHtmlDialog::SetElementText(const wchar_t* lpszElementId, wchar_t* lpszText) {
    IHTMLElement* pElem = nullptr;
    if (FAILED(GetElement(lpszElementId, &pElem)) || !pElem) return;
    BSTR bstr = SysAllocString(lpszText ? lpszText : L"");
    pElem->put_innerText(bstr);
    SysFreeString(bstr);
    pElem->Release();
}

wchar_t* CDHtmlDialog::GetElementHtml(const wchar_t* lpszElementId) {
    IHTMLElement* pElem = nullptr;
    if (FAILED(GetElement(lpszElementId, &pElem)) || !pElem) return nullptr;
    BSTR bstr = nullptr;
    pElem->get_innerHTML(&bstr);
    pElem->Release();
    return bstr; // caller responsible for SysFreeString
}

void CDHtmlDialog::SetElementHtml(const wchar_t* lpszElementId, wchar_t* lpszHtml) {
    IHTMLElement* pElem = nullptr;
    if (FAILED(GetElement(lpszElementId, &pElem)) || !pElem) return;
    BSTR bstr = SysAllocString(lpszHtml ? lpszHtml : L"");
    pElem->put_innerHTML(bstr);
    SysFreeString(bstr);
    pElem->Release();
}

void CDHtmlDialog::SetElementHtml(IUnknown* punkElem, wchar_t* lpszHtml) {
    if (!punkElem) return;
    IHTMLElement* pElem = nullptr;
    if (SUCCEEDED(punkElem->QueryInterface(IID_IHTMLElement, (void**)&pElem)) && pElem) {
        BSTR bstr = SysAllocString(lpszHtml ? lpszHtml : L"");
        pElem->put_innerHTML(bstr);
        SysFreeString(bstr);
        pElem->Release();
    }
}

void CDHtmlDialog::SetElementText(IUnknown* punkElem, wchar_t* lpszText) {
    if (!punkElem) return;
    IHTMLElement* pElem = nullptr;
    if (SUCCEEDED(punkElem->QueryInterface(IID_IHTMLElement, (void**)&pElem)) && pElem) {
        BSTR bstr = SysAllocString(lpszText ? lpszText : L"");
        pElem->put_innerText(bstr);
        SysFreeString(bstr);
        pElem->Release();
    }
}

long CDHtmlDialog::GetControlDispatch(const wchar_t* lpszId, IDispatch** ppDisp) {
    return GetElement(lpszId, ppDisp, nullptr);
}

VARIANT CDHtmlDialog::GetControlProperty(const wchar_t* lpszId, const wchar_t* lpszPropName) {
    VARIANT v; VariantInit(&v);
    if (!lpszId || !lpszPropName) return v;
    IDispatch* pDisp = nullptr;
    if (FAILED(GetControlDispatch(lpszId, &pDisp)) || !pDisp) return v;
    // Get property by name via GetIDsOfNames + Invoke
    BSTR bstrProp = SysAllocString(lpszPropName);
    DISPID dispid = DISPID_UNKNOWN;
    if (SUCCEEDED(pDisp->GetIDsOfNames(IID_NULL, &bstrProp, 1, LOCALE_USER_DEFAULT, &dispid))) {
        DISPPARAMS dp = { nullptr, nullptr, 0, 0 };
        pDisp->Invoke(dispid, IID_NULL, LOCALE_USER_DEFAULT,
                      DISPATCH_PROPERTYGET, &dp, &v, nullptr, nullptr);
    }
    SysFreeString(bstrProp);
    pDisp->Release();
    return v;
}

VARIANT CDHtmlDialog::GetControlProperty(const wchar_t* lpszId, DISPID dispId) {
    VARIANT v; VariantInit(&v);
    IDispatch* pDisp = nullptr;
    if (FAILED(GetControlDispatch(lpszId, &pDisp)) || !pDisp) return v;
    DISPPARAMS dp = { nullptr, nullptr, 0, 0 };
    pDisp->Invoke(dispId, IID_NULL, LOCALE_USER_DEFAULT,
                  DISPATCH_PROPERTYGET, &dp, &v, nullptr, nullptr);
    pDisp->Release();
    return v;
}

void CDHtmlDialog::SetControlProperty(const wchar_t* lpszId, const wchar_t* lpszPropName, VARIANT* pVar) {
    if (!lpszId || !lpszPropName || !pVar) return;
    IDispatch* pDisp = nullptr;
    if (FAILED(GetControlDispatch(lpszId, &pDisp)) || !pDisp) return;
    BSTR bstrProp = SysAllocString(lpszPropName);
    DISPID dispid = DISPID_UNKNOWN;
    if (SUCCEEDED(pDisp->GetIDsOfNames(IID_NULL, &bstrProp, 1, LOCALE_USER_DEFAULT, &dispid))) {
        DISPID namedId = DISPID_PROPERTYPUT;
        DISPPARAMS dp = { pVar, &namedId, 1, 1 };
        pDisp->Invoke(dispid, IID_NULL, LOCALE_USER_DEFAULT,
                      DISPATCH_PROPERTYPUT, &dp, nullptr, nullptr, nullptr);
    }
    SysFreeString(bstrProp);
    pDisp->Release();
}

void CDHtmlDialog::SetControlProperty(const wchar_t* lpszId, DISPID dispId, VARIANT* pVar) {
    if (!lpszId || !pVar) return;
    IDispatch* pDisp = nullptr;
    if (FAILED(GetControlDispatch(lpszId, &pDisp)) || !pDisp) return;
    DISPID namedId = DISPID_PROPERTYPUT;
    DISPPARAMS dp = { pVar, &namedId, 1, 1 };
    pDisp->Invoke(dispId, IID_NULL, LOCALE_USER_DEFAULT,
                  DISPATCH_PROPERTYPUT, &dp, nullptr, nullptr, nullptr);
    pDisp->Release();
}

void CDHtmlDialog::SetControlProperty(IDispatch* pDisp, DISPID dispId, VARIANT* pVar) {
    if (!pDisp || !pVar) return;
    DISPID namedId = DISPID_PROPERTYPUT;
    DISPPARAMS dp = { pVar, &namedId, 1, 1 };
    pDisp->Invoke(dispId, IID_NULL, LOCALE_USER_DEFAULT,
                  DISPATCH_PROPERTYPUT, &dp, nullptr, nullptr, nullptr);
}

long CDHtmlDialog::GetEvent(IHTMLEventObj** ppEventObj) {
    if (!ppEventObj) return E_POINTER;
    *ppEventObj = nullptr;
    if (!m_pBrowser) return E_FAIL;
    LPDISPATCH pDocDisp = nullptr;
    HRESULT hr = m_pBrowser->get_Document(&pDocDisp);
    if (FAILED(hr) || !pDocDisp) return hr;
    IHTMLDocument2* pDoc2 = nullptr;
    hr = pDocDisp->QueryInterface(IID_IHTMLDocument2, (void**)&pDoc2);
    pDocDisp->Release();
    if (SUCCEEDED(hr) && pDoc2) {
        pDoc2->get_parentWindow(nullptr); // silence compiler; real impl accesses window.event
        pDoc2->Release();
    }
    return E_NOTIMPL;
}

HRESULT CDHtmlDialog::GetDHtmlDocument(IHTMLDocument2** ppDocument) {
    if (!ppDocument) return E_POINTER;
    *ppDocument = nullptr;
    if (!m_pBrowser) return E_FAIL;
    LPDISPATCH pDocDisp = nullptr;
    HRESULT hr = m_pBrowser->get_Document(&pDocDisp);
    if (FAILED(hr) || !pDocDisp) return hr;
    hr = pDocDisp->QueryInterface(IID_IHTMLDocument2, (void**)ppDocument);
    pDocDisp->Release();
    return hr;
}

// Navigation event stubs
void CDHtmlDialog::OnBeforeNavigate(IDispatch*, const wchar_t*) {}
void CDHtmlDialog::OnNavigateComplete(IDispatch*, const wchar_t*) {}
void CDHtmlDialog::OnDocumentComplete(IDispatch*, const wchar_t*) {}

// Internal helpers
int CDHtmlDialog::FindSinkForObject(const wchar_t*) { return -1; }
void CDHtmlDialog::SetFocusToElement(const wchar_t* lpszId) {
    // No-op default: keep ABI surface without requiring full DOM focus plumbing.
    (void)lpszId;
}
long CDHtmlDialog::Select_FindString(IHTMLSelectElement* pSelect, wchar_t* lpszFind, int bExact) {
    (void)pSelect; (void)lpszFind; (void)bExact;
    return -1;
}

// IDocHostUIHandler stubs
HRESULT CDHtmlDialog::GetHostInfo(DOCHOSTUIINFO* pInfo) {
    if (!pInfo) return E_POINTER;
    pInfo->cbSize = sizeof(DOCHOSTUIINFO);
    pInfo->dwFlags = 0;
    pInfo->dwDoubleClick = DOCHOSTUIDBLCLK_DEFAULT;
    return S_OK;
}
HRESULT CDHtmlDialog::GetOptionKeyPath(wchar_t**, DWORD) { return E_NOTIMPL; }
HRESULT CDHtmlDialog::TranslateUrl(DWORD, wchar_t*, wchar_t** ppOut) {
    if (ppOut) *ppOut = nullptr; return S_FALSE;
}
HRESULT CDHtmlDialog::ShowContextMenu(DWORD, POINT*, IUnknown*, IDispatch*) { return S_OK; }
HRESULT CDHtmlDialog::ShowUI(DWORD, IOleInPlaceActiveObject*, IOleCommandTarget*,
                              IOleInPlaceFrame*, IOleInPlaceUIWindow*) { return S_OK; }
HRESULT CDHtmlDialog::HideUI() { return S_OK; }
HRESULT CDHtmlDialog::UpdateUI() { return S_OK; }
HRESULT CDHtmlDialog::EnableModeless(BOOL) { return S_OK; }
HRESULT CDHtmlDialog::OnDocWindowActivate(BOOL) { return S_OK; }
HRESULT CDHtmlDialog::OnFrameWindowActivate(BOOL) { return S_OK; }
HRESULT CDHtmlDialog::ResizeBorder(LPCRECT, IOleInPlaceUIWindow*, BOOL) { return S_OK; }
HRESULT CDHtmlDialog::TranslateAcceleratorW(LPMSG, const GUID*, DWORD) { return S_FALSE; }
HRESULT CDHtmlDialog::GetDropTarget(IDropTarget*, IDropTarget** ppOut) {
    if (ppOut) *ppOut = nullptr; return E_NOTIMPL;
}
HRESULT CDHtmlDialog::GetExternal(IDispatch** ppOut) {
    if (ppOut) *ppOut = nullptr; return E_NOTIMPL;
}
HRESULT CDHtmlDialog::FilterDataObject(IDataObject*, IDataObject** ppOut) {
    if (ppOut) *ppOut = nullptr; return E_NOTIMPL;
}
HRESULT CDHtmlDialog::IsExternalDispatchSafe() { return S_OK; }
HRESULT CDHtmlDialog::CanAccessExternal() { return S_OK; }
HRESULT CDHtmlDialog::CreateControlSite(COleControlContainer*, COleControlSite**, UINT, REFCLSID) {
    return E_NOTIMPL;
}

void CDHtmlDialog::SetExternalDispatch(IDispatch*) {}
void CDHtmlDialog::SetHostFlags(DWORD) {}

void CDHtmlDialog::OnDDXError(const wchar_t*, UINT, int) {}

void CDHtmlDialog::OnDestroy() {
    if (m_pBrowser) {
        m_pBrowser->Stop();
        m_pBrowser->put_Visible(VARIANT_FALSE);
    }
    CDialog::OnDestroy();
}

//=============================================================================
// CHtmlView + CDHtmlDialog extern "C" MS_ABI wrappers
// These allow MSVC-mangled exports while using MinGW C++ class implementations.
//=============================================================================

namespace {
struct CDHtmlDialogAccess : CDHtmlDialog {
    using CDHtmlDialog::FindSinkForObject;
    using CDHtmlDialog::OnDDXError;
    using CDHtmlDialog::OnDestroy;
    using CDHtmlDialog::Select_FindString;
    using CDHtmlDialog::SetFocusToElement;
};
}

// ---------------------------------------------------------------------------
// CHtmlView wrappers
// ---------------------------------------------------------------------------

// Symbol: ??0CHtmlView@@IEAA@XZ
extern "C" void* MS_ABI impl___0CHtmlView__IEAA_XZ(CHtmlView* pThis) {
    new (pThis) CHtmlView();
    return pThis;
}

// Symbol: ??1CHtmlView@@UEAA@XZ
extern "C" void MS_ABI impl___1CHtmlView__UEAA_XZ(CHtmlView* pThis) {
    pThis->~CHtmlView();
}

// Symbol: ?Create@CHtmlView@@UEAAHPEB_W0KAEBUtagRECT@@PEAVCWnd@@IPEAUCCreateContext@@@Z
extern "C" int MS_ABI impl__Create_CHtmlView__UEAAHPEB_W0KAEBUtagRECT__PEAVCWnd__IPEAUCCreateContext___Z(
    CHtmlView* pThis, const wchar_t* lpszClassName, const wchar_t* lpszWindowName,
    unsigned long dwStyle, const RECT& rect, CWnd* pParentWnd, unsigned int nID,
    CCreateContext* pContext) {
    return pThis->CHtmlView::Create(lpszClassName, lpszWindowName, dwStyle, rect,
                                     pParentWnd, nID, pContext);
}

// Symbol: ?OnDraw@CHtmlView@@UEAAXPEAVCDC@@@Z
extern "C" void MS_ABI impl__OnDraw_CHtmlView__UEAAXPEAVCDC___Z(CHtmlView* pThis, CDC* pDC) {
    pThis->CHtmlView::OnDraw(pDC);
}

// Symbol: ?Navigate@CHtmlView@@QEAAXPEB_WK00PEAXK@Z
extern "C" void MS_ABI impl__Navigate_CHtmlView__QEAAXPEB_WK00PEAXK_Z(
    CHtmlView* pThis, const wchar_t* lpszURL, unsigned long dwFlags,
    const wchar_t* lpszTargetFrameName, const wchar_t* lpszHeaders,
    void* lpvPostData, unsigned long dwPostDataLen) {
    pThis->Navigate(lpszURL, dwFlags, lpszTargetFrameName, lpszHeaders,
                    lpvPostData, dwPostDataLen);
}

// Symbol: ?Navigate2@CHtmlView@@QEAAXPEB_WK00PEAXK@Z
extern "C" void MS_ABI impl__Navigate2_CHtmlView__QEAAXPEB_WK00PEAXK_Z(
    CHtmlView* pThis, const wchar_t* lpszURL, unsigned long dwFlags,
    const wchar_t* lpszTargetFrameName, const wchar_t* lpszHeaders,
    void* lpvPostData, unsigned long dwPostDataLen) {
    pThis->Navigate2(lpszURL, dwFlags, lpszTargetFrameName, lpszHeaders,
                     lpvPostData, dwPostDataLen);
}

// Symbol: ?Navigate2@CHtmlView@@QEAAXPEFAU_ITEMIDLIST@@KPEB_W@Z
extern "C" void MS_ABI impl__Navigate2_CHtmlView__QEAAXPEFAU_ITEMIDLIST__KPEB_W_Z(
    CHtmlView* pThis, LPITEMIDLIST pIDL, unsigned long dwFlags,
    const wchar_t* lpszTargetFrameName) {
    pThis->Navigate2(pIDL, dwFlags, lpszTargetFrameName);
}

// Symbol: ?GoBack@CHtmlView@@QEAAXXZ
extern "C" void MS_ABI impl__GoBack_CHtmlView__QEAAXXZ(CHtmlView* pThis) { pThis->GoBack(); }

// Symbol: ?GoForward@CHtmlView@@QEAAXXZ
extern "C" void MS_ABI impl__GoForward_CHtmlView__QEAAXXZ(CHtmlView* pThis) { pThis->GoForward(); }

// Symbol: ?GoHome@CHtmlView@@QEAAXXZ
extern "C" void MS_ABI impl__GoHome_CHtmlView__QEAAXXZ(CHtmlView* pThis) { pThis->GoHome(); }

// Symbol: ?GoSearch@CHtmlView@@QEAAXXZ
extern "C" void MS_ABI impl__GoSearch_CHtmlView__QEAAXXZ(CHtmlView* pThis) { pThis->GoSearch(); }

// Symbol: ?Stop@CHtmlView@@QEAAXXZ
extern "C" void MS_ABI impl__Stop_CHtmlView__QEAAXXZ(CHtmlView* pThis) { pThis->Stop(); }

// Symbol: ?Refresh@CHtmlView@@QEAAXXZ
extern "C" void MS_ABI impl__Refresh_CHtmlView__QEAAXXZ(CHtmlView* pThis) { pThis->Refresh(); }

// Symbol: ?Refresh2@CHtmlView@@QEAAXH@Z
extern "C" void MS_ABI impl__Refresh2_CHtmlView__QEAAXH_Z(CHtmlView* pThis, int nLevel) {
    pThis->Refresh2(nLevel);
}

// Symbol: ?GetBusy@CHtmlView@@QEBAHXZ
extern "C" int MS_ABI impl__GetBusy_CHtmlView__QEBAHXZ(const CHtmlView* pThis) {
    return pThis->GetBusy();
}

// Symbol: ?GetReadyState@CHtmlView@@QEBA?AW4tagREADYSTATE@@XZ
extern "C" READYSTATE MS_ABI impl__GetReadyState_CHtmlView__QEBA_AW4tagREADYSTATE__XZ(
    const CHtmlView* pThis) {
    return pThis->GetReadyState();
}

// Symbol: ?GetLocationName@CHtmlView@@QEBA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@XZ
extern "C" void MS_ABI impl__GetLocationName_CHtmlView__QEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__XZ(
    CString* pRet, const CHtmlView* pThis) {
    new (pRet) CString(pThis->GetLocationName());
}

// Symbol: ?GetLocationURL@CHtmlView@@QEBA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@XZ
extern "C" void MS_ABI impl__GetLocationURL_CHtmlView__QEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__XZ(
    CString* pRet, const CHtmlView* pThis) {
    new (pRet) CString(pThis->GetLocationURL());
}

// Symbol: ?GetFullName@CHtmlView@@QEBA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@XZ
extern "C" void MS_ABI impl__GetFullName_CHtmlView__QEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__XZ(
    CString* pRet, const CHtmlView* pThis) {
    new (pRet) CString(pThis->GetFullName());
}

// Symbol: ?GetType@CHtmlView@@QEBA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@XZ
extern "C" void MS_ABI impl__GetType_CHtmlView__QEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__XZ(
    CString* pRet, const CHtmlView* pThis) {
    new (pRet) CString(pThis->GetType());
}

// Symbol: ?GetHtmlDocument@CHtmlView@@QEBAPEAUIDispatch@@XZ
extern "C" IDispatch* MS_ABI impl__GetHtmlDocument_CHtmlView__QEBAPEAUIDispatch__XZ(
    const CHtmlView* pThis) {
    return pThis->GetHtmlDocument();
}

// Symbol: ?GetApplication@CHtmlView@@QEBAPEAUIDispatch@@XZ
extern "C" IDispatch* MS_ABI impl__GetApplication_CHtmlView__QEBAPEAUIDispatch__XZ(
    const CHtmlView* pThis) {
    return pThis->GetApplication();
}

// Symbol: ?GetContainer@CHtmlView@@QEBAPEAUIDispatch@@XZ
extern "C" IDispatch* MS_ABI impl__GetContainer_CHtmlView__QEBAPEAUIDispatch__XZ(
    const CHtmlView* pThis) {
    return pThis->GetContainer();
}

// Symbol: ?GetParentBrowser@CHtmlView@@QEBAPEAUIDispatch@@XZ
extern "C" IDispatch* MS_ABI impl__GetParentBrowser_CHtmlView__QEBAPEAUIDispatch__XZ(
    const CHtmlView* pThis) {
    return pThis->GetParentBrowser();
}

// Symbol: ?GetLeft@CHtmlView@@QEBAJXZ
extern "C" long MS_ABI impl__GetLeft_CHtmlView__QEBAJXZ(const CHtmlView* pThis) {
    return pThis->GetLeft();
}
// Symbol: ?GetTop@CHtmlView@@QEBAJXZ
extern "C" long MS_ABI impl__GetTop_CHtmlView__QEBAJXZ(const CHtmlView* pThis) {
    return pThis->GetTop();
}
// Symbol: ?GetWidth@CHtmlView@@QEBAJXZ
extern "C" long MS_ABI impl__GetWidth_CHtmlView__QEBAJXZ(const CHtmlView* pThis) {
    return pThis->GetWidth();
}
// Symbol: ?GetHeight@CHtmlView@@QEBAJXZ
extern "C" long MS_ABI impl__GetHeight_CHtmlView__QEBAJXZ(const CHtmlView* pThis) {
    return pThis->GetHeight();
}
// Symbol: ?GetVisible@CHtmlView@@QEBAHXZ
extern "C" int MS_ABI impl__GetVisible_CHtmlView__QEBAHXZ(const CHtmlView* pThis) {
    return pThis->GetVisible();
}
// Symbol: ?GetFullScreen@CHtmlView@@QEBAHXZ
extern "C" int MS_ABI impl__GetFullScreen_CHtmlView__QEBAHXZ(const CHtmlView* pThis) {
    return pThis->GetFullScreen();
}
// Symbol: ?GetMenuBar@CHtmlView@@QEBAHXZ
extern "C" int MS_ABI impl__GetMenuBar_CHtmlView__QEBAHXZ(const CHtmlView* pThis) {
    return pThis->GetMenuBar();
}
// Symbol: ?GetAddressBar@CHtmlView@@QEBAHXZ
extern "C" int MS_ABI impl__GetAddressBar_CHtmlView__QEBAHXZ(const CHtmlView* pThis) {
    return pThis->GetAddressBar();
}
// Symbol: ?GetStatusBar@CHtmlView@@QEBAHXZ
extern "C" int MS_ABI impl__GetStatusBar_CHtmlView__QEBAHXZ(const CHtmlView* pThis) {
    return pThis->GetStatusBar();
}
// Symbol: ?GetToolBar@CHtmlView@@QEBAHXZ
extern "C" int MS_ABI impl__GetToolBar_CHtmlView__QEBAHXZ(const CHtmlView* pThis) {
    return pThis->GetToolBar();
}
// Symbol: ?GetOffline@CHtmlView@@QEBAHXZ
extern "C" int MS_ABI impl__GetOffline_CHtmlView__QEBAHXZ(const CHtmlView* pThis) {
    return pThis->GetOffline();
}
// Symbol: ?GetSilent@CHtmlView@@QEBAHXZ
extern "C" int MS_ABI impl__GetSilent_CHtmlView__QEBAHXZ(const CHtmlView* pThis) {
    return pThis->GetSilent();
}
// Symbol: ?GetTheaterMode@CHtmlView@@QEBAHXZ
extern "C" int MS_ABI impl__GetTheaterMode_CHtmlView__QEBAHXZ(const CHtmlView* pThis) {
    return pThis->GetTheaterMode();
}
// Symbol: ?GetRegisterAsBrowser@CHtmlView@@QEBAHXZ
extern "C" int MS_ABI impl__GetRegisterAsBrowser_CHtmlView__QEBAHXZ(const CHtmlView* pThis) {
    return pThis->GetRegisterAsBrowser();
}
// Symbol: ?GetRegisterAsDropTarget@CHtmlView@@QEBAHXZ
extern "C" int MS_ABI impl__GetRegisterAsDropTarget_CHtmlView__QEBAHXZ(const CHtmlView* pThis) {
    return pThis->GetRegisterAsDropTarget();
}
// Symbol: ?GetTopLevelContainer@CHtmlView@@QEBAHXZ
extern "C" int MS_ABI impl__GetTopLevelContainer_CHtmlView__QEBAHXZ(const CHtmlView* pThis) {
    return pThis->GetTopLevelContainer();
}

// Symbol: ?ExecWB@CHtmlView@@QEAAXW4OLECMDID@@W4OLECMDEXECOPT@@PEAUtagVARIANT@@2@Z
extern "C" void MS_ABI impl__ExecWB_CHtmlView__QEAAXW4OLECMDID__W4OLECMDEXECOPT__PEAUtagVARIANT__2_Z(
    CHtmlView* pThis, OLECMDID cmdID, OLECMDEXECOPT cmdexecopt,
    VARIANT* pvaIn, VARIANT* pvaOut) {
    pThis->ExecWB(cmdID, cmdexecopt, pvaIn, pvaOut);
}

// Symbol: ?LoadFromResource@CHtmlView@@QEAAHPEB_W@Z
extern "C" int MS_ABI impl__LoadFromResource_CHtmlView__QEAAHPEB_W_Z(
    CHtmlView* pThis, const wchar_t* lpszResource) {
    return pThis->LoadFromResource(lpszResource);
}

// Symbol: ?LoadFromResource@CHtmlView@@QEAAHI@Z
extern "C" int MS_ABI impl__LoadFromResource_CHtmlView__QEAAHI_Z(
    CHtmlView* pThis, unsigned int nRes) {
    return pThis->LoadFromResource(nRes);
}

// Symbol: ?Print@CHtmlView@@QEAAXXZ (not in mapping but keep for completeness)

// Symbol: ?GetProperty@CHtmlView@@QEAAHPEB_WAEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@@Z
extern "C" int MS_ABI impl__GetProperty_CHtmlView__QEAAHPEB_WAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
    CHtmlView* pThis, const wchar_t* lpszProperty, CString& strValue) {
    return pThis->GetProperty(lpszProperty, strValue);
}

// Symbol: ?PutProperty@CHtmlView@@QEAAXPEB_WAEBUtagVARIANT@@@Z
extern "C" void MS_ABI impl__PutProperty_CHtmlView__QEAAXPEB_WAEBUtagVARIANT___Z(
    CHtmlView* pThis, const wchar_t* lpszProperty, const VARIANT& vtValue) {
    pThis->PutProperty(lpszProperty, vtValue);
}

// Symbol: ?GetSource@CHtmlView@@UEAAHAEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@@Z
extern "C" int MS_ABI impl__GetSource_CHtmlView__UEAAHAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
    CHtmlView* pThis, CString& strRef) {
    return pThis->GetSource(strRef);
}

// Symbol: ?OnGetOptionKeyPath@CHtmlView@@UEAAJPEAPEA_WK@Z
extern "C" long MS_ABI impl__OnGetOptionKeyPath_CHtmlView__UEAAJPEAPEA_WK_Z(
    CHtmlView* pThis, wchar_t** ppszKey, unsigned long dwReserved) {
    return pThis->OnGetOptionKeyPath(ppszKey, dwReserved);
}

// CHtmlView event sink stubs
// Symbol: ?OnBeforeNavigate2@CHtmlView@@UEAAXPEB_WK0AEAVCByteArray@@0PEAH@Z
extern "C" void MS_ABI impl__OnBeforeNavigate2_CHtmlView__UEAAXPEB_WK0AEAVCByteArray__0PEAH_Z(
    CHtmlView* pThis, const wchar_t* lpszURL, unsigned long nFlags,
    const wchar_t* lpszTargetFrameName, CByteArray& baPostData,
    const wchar_t* lpszHeaders, BOOL* pbCancel) {
    pThis->OnBeforeNavigate2(lpszURL, nFlags, lpszTargetFrameName,
                             baPostData, lpszHeaders, pbCancel);
}

// Symbol: ?OnNavigateComplete2@CHtmlView@@UEAAXPEB_W@Z
extern "C" void MS_ABI impl__OnNavigateComplete2_CHtmlView__UEAAXPEB_W_Z(
    CHtmlView* pThis, const wchar_t* lpszURL) {
    pThis->OnNavigateComplete2(lpszURL);
}

// Symbol: ?OnDocumentComplete@CHtmlView@@UEAAXPEB_W@Z
extern "C" void MS_ABI impl__OnDocumentComplete_CHtmlView__UEAAXPEB_W_Z(
    CHtmlView* pThis, const wchar_t* lpszURL) {
    pThis->OnDocumentComplete(lpszURL);
}

// Symbol: ?OnProgressChange@CHtmlView@@UEAAXJJ@Z
extern "C" void MS_ABI impl__OnProgressChange_CHtmlView__UEAAXJJ_Z(
    CHtmlView* pThis, long lProgress, long lProgressMax) {
    pThis->OnProgressChange(lProgress, lProgressMax);
}

// Symbol: ?OnTitleChange@CHtmlView@@UEAAXPEB_W@Z
extern "C" void MS_ABI impl__OnTitleChange_CHtmlView__UEAAXPEB_W_Z(
    CHtmlView* pThis, const wchar_t* lpszText) {
    pThis->OnTitleChange(lpszText);
}

// Symbol: ?OnStatusTextChange@CHtmlView@@UEAAXPEB_W@Z
extern "C" void MS_ABI impl__OnStatusTextChange_CHtmlView__UEAAXPEB_W_Z(
    CHtmlView* pThis, const wchar_t* lpszText) {
    pThis->OnStatusTextChange(lpszText);
}

// Symbol: ?OnCommandStateChange@CHtmlView@@UEAAXJH@Z
extern "C" void MS_ABI impl__OnCommandStateChange_CHtmlView__UEAAXJH_Z(
    CHtmlView* pThis, long lCommand, int bEnable) {
    pThis->OnCommandStateChange(lCommand, bEnable);
}

// Symbol: ?OnDownloadBegin@CHtmlView@@UEAAXXZ
extern "C" void MS_ABI impl__OnDownloadBegin_CHtmlView__UEAAXXZ(CHtmlView* pThis) {
    pThis->OnDownloadBegin();
}

// Symbol: ?OnDownloadComplete@CHtmlView@@UEAAXXZ
extern "C" void MS_ABI impl__OnDownloadComplete_CHtmlView__UEAAXXZ(CHtmlView* pThis) {
    pThis->OnDownloadComplete();
}

// Symbol: ?OnFullScreen@CHtmlView@@UEAAXH@Z
extern "C" void MS_ABI impl__OnFullScreen_CHtmlView__UEAAXH_Z(CHtmlView* pThis, int bFullScreen) {
    pThis->OnFullScreen(bFullScreen);
}

// Symbol: ?OnMenuBar@CHtmlView@@UEAAXH@Z
extern "C" void MS_ABI impl__OnMenuBar_CHtmlView__UEAAXH_Z(CHtmlView* pThis, int bMenuBar) {
    pThis->OnMenuBar(bMenuBar);
}

// Symbol: ?OnNavigateError@CHtmlView@@UEAAXPEB_W0KPEAH@Z
extern "C" void MS_ABI impl__OnNavigateError_CHtmlView__UEAAXPEB_W0KPEAH_Z(
    CHtmlView* pThis, const wchar_t* lpszURL, const wchar_t* lpszFrame,
    unsigned long dwError, BOOL* pbCancel) {
    pThis->OnNavigateError(lpszURL, lpszFrame, dwError, pbCancel);
}

// Symbol: ?OnNewWindow2@CHtmlView@@UEAAXPEAPEAUIDispatch@@PEAH@Z
extern "C" void MS_ABI impl__OnNewWindow2_CHtmlView__UEAAXPEAPEAUIDispatch__PEAH_Z(
    CHtmlView* pThis, IDispatch** ppDisp, BOOL* bCancel) {
    pThis->OnNewWindow2(ppDisp, bCancel);
}

// Symbol: ?OnPropertyChange@CHtmlView@@UEAAXPEB_W@Z
extern "C" void MS_ABI impl__OnPropertyChange_CHtmlView__UEAAXPEB_W_Z(
    CHtmlView* pThis, const wchar_t* lpszProperty) {
    pThis->OnPropertyChange(lpszProperty);
}

// Symbol: ?OnQuit@CHtmlView@@UEAAXXZ
extern "C" void MS_ABI impl__OnQuit_CHtmlView__UEAAXXZ(CHtmlView* pThis) {
    pThis->OnQuit();
}

// Symbol: ?OnStatusBar@CHtmlView@@UEAAXH@Z
extern "C" void MS_ABI impl__OnStatusBar_CHtmlView__UEAAXH_Z(CHtmlView* pThis, int bStatusBar) {
    pThis->OnStatusBar(bStatusBar);
}

// Symbol: ?OnTheaterMode@CHtmlView@@UEAAXH@Z
extern "C" void MS_ABI impl__OnTheaterMode_CHtmlView__UEAAXH_Z(CHtmlView* pThis, int bTheaterMode) {
    pThis->OnTheaterMode(bTheaterMode);
}

// Symbol: ?OnToolBar@CHtmlView@@UEAAXH@Z
extern "C" void MS_ABI impl__OnToolBar_CHtmlView__UEAAXH_Z(CHtmlView* pThis, int bToolBar) {
    pThis->OnToolBar(bToolBar);
}

// Symbol: ?OnVisible@CHtmlView@@UEAAXH@Z
extern "C" void MS_ABI impl__OnVisible_CHtmlView__UEAAXH_Z(CHtmlView* pThis, int bVisible) {
    pThis->OnVisible(bVisible);
}

// Symbol: ?OnTranslateUrl@CHtmlView@@UEAAJKPEA_WPEAPEA_W@Z
extern "C" long MS_ABI impl__OnTranslateUrl_CHtmlView__UEAAJKPEA_WPEAPEA_W_Z(
    CHtmlView* pThis, unsigned long dwTranslate, wchar_t* pchURLIn, wchar_t** ppchURLOut) {
    return pThis->OnTranslateUrl(dwTranslate, pchURLIn, ppchURLOut);
}

// ---------------------------------------------------------------------------
// CDHtmlDialog wrappers
// ---------------------------------------------------------------------------

// Symbol: ??0CDHtmlDialog@@QEAA@XZ
extern "C" void* MS_ABI impl___0CDHtmlDialog__QEAA_XZ(CDHtmlDialog* pThis) {
    new (pThis) CDHtmlDialog();
    return pThis;
}

// Symbol: ??0CDHtmlDialog@@QEAA@IIPEAVCWnd@@@Z
extern "C" void* MS_ABI impl___0CDHtmlDialog__QEAA_IIPEAVCWnd___Z(
    CDHtmlDialog* pThis, unsigned int nIDTemplate, unsigned int nHtmlResID, CWnd* pParentWnd) {
    new (pThis) CDHtmlDialog(nIDTemplate, nHtmlResID, pParentWnd);
    return pThis;
}

// Symbol: ??0CDHtmlDialog@@QEAA@PEB_W0PEAVCWnd@@@Z
extern "C" void* MS_ABI impl___0CDHtmlDialog__QEAA_PEB_W0PEAVCWnd___Z(
    CDHtmlDialog* pThis, const wchar_t* lpszTemplateName,
    const wchar_t* lpszHtmlResID, CWnd* pParentWnd) {
    new (pThis) CDHtmlDialog(lpszTemplateName, lpszHtmlResID, pParentWnd);
    return pThis;
}

// Symbol: ??1CDHtmlDialog@@UEAA@XZ
extern "C" void MS_ABI impl___1CDHtmlDialog__UEAA_XZ(CDHtmlDialog* pThis) {
    pThis->~CDHtmlDialog();
}

// Symbol: ?OnInitDialog@CDHtmlDialog@@UEAAHXZ
extern "C" int MS_ABI impl__OnInitDialog_CDHtmlDialog__UEAAHXZ(CDHtmlDialog* pThis) {
    return pThis->CDHtmlDialog::OnInitDialog();
}

// Symbol: ?LoadFromResource@CDHtmlDialog@@QEAAHI@Z
extern "C" int MS_ABI impl__LoadFromResource_CDHtmlDialog__QEAAHI_Z(
    CDHtmlDialog* pThis, unsigned int nHtmlResID) {
    return pThis->LoadFromResource(nHtmlResID);
}

// Symbol: ?LoadFromResource@CDHtmlDialog@@QEAAHPEB_W@Z
extern "C" int MS_ABI impl__LoadFromResource_CDHtmlDialog__QEAAHPEB_W_Z(
    CDHtmlDialog* pThis, const wchar_t* lpszHtmlResID) {
    return pThis->LoadFromResource(lpszHtmlResID);
}

// Symbol: ?Navigate@CDHtmlDialog@@QEAAXPEB_WK00PEAXK@Z
extern "C" void MS_ABI impl__Navigate_CDHtmlDialog__QEAAXPEB_WK00PEAXK_Z(
    CDHtmlDialog* pThis, const wchar_t* lpszURL, unsigned long dwFlags,
    const wchar_t* lpszTargetFrameName, const wchar_t* lpszHeaders,
    void* lpvPostData, unsigned long dwPostDataLen) {
    pThis->Navigate(lpszURL, dwFlags, lpszTargetFrameName, lpszHeaders,
                    lpvPostData, dwPostDataLen);
}

// Symbol: ?GetCurrentUrl@CDHtmlDialog@@QEAAXAEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@@Z
extern "C" void MS_ABI impl__GetCurrentUrl_CDHtmlDialog__QEAAXAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
    CDHtmlDialog* pThis, CString& strUrl) {
    pThis->GetCurrentUrl(strUrl);
}

// Symbol: ?GetElement@CDHtmlDialog@@QEAAJPEB_WPEAPEAUIDispatch@@PEAH@Z
extern "C" long MS_ABI impl__GetElement_CDHtmlDialog__QEAAJPEB_WPEAPEAUIDispatch__PEAH_Z(
    CDHtmlDialog* pThis, const wchar_t* lpszElementId, IDispatch** ppDisp, int* pfCollection) {
    return pThis->GetElement(lpszElementId, ppDisp, pfCollection);
}

// Symbol: ?GetElement@CDHtmlDialog@@QEAAJPEB_WPEAPEAUIHTMLElement@@@Z
extern "C" long MS_ABI impl__GetElement_CDHtmlDialog__QEAAJPEB_WPEAPEAUIHTMLElement___Z(
    CDHtmlDialog* pThis, const wchar_t* lpszElementId, IHTMLElement** ppElement) {
    return pThis->GetElement(lpszElementId, ppElement);
}

// Symbol: ?GetElementInterface@CDHtmlDialog@@QEAAJPEB_WAEBU_GUID@@PEAPEAX@Z
extern "C" long MS_ABI impl__GetElementInterface_CDHtmlDialog__QEAAJPEB_WAEBU_GUID__PEAPEAX_Z(
    CDHtmlDialog* pThis, const wchar_t* lpszElementId, REFIID riid, void** ppUnk) {
    return pThis->GetElementInterface(lpszElementId, riid, ppUnk);
}

// Symbol: ?GetElementProperty@CDHtmlDialog@@QEAA?AUtagVARIANT@@PEB_WJ@Z
extern "C" void MS_ABI impl__GetElementProperty_CDHtmlDialog__QEAA_AUtagVARIANT__PEB_WJ_Z(
    VARIANT* pRet, CDHtmlDialog* pThis, const wchar_t* lpszElementId, long lCookie) {
    new (pRet) VARIANT(pThis->GetElementProperty(lpszElementId, lCookie));
}

// Symbol: ?SetElementProperty@CDHtmlDialog@@QEAAXPEB_WJPEAUtagVARIANT@@@Z
extern "C" void MS_ABI impl__SetElementProperty_CDHtmlDialog__QEAAXPEB_WJPEAUtagVARIANT___Z(
    CDHtmlDialog* pThis, const wchar_t* lpszElementId, long dispId, VARIANT* pVar) {
    pThis->SetElementProperty(lpszElementId, (DISPID)dispId, pVar);
}

// Symbol: ?GetElementText@CDHtmlDialog@@QEAAPEA_WPEB_W@Z
extern "C" wchar_t* MS_ABI impl__GetElementText_CDHtmlDialog__QEAAPEA_WPEB_W_Z(
    CDHtmlDialog* pThis, const wchar_t* lpszElementId) {
    return pThis->GetElementText(lpszElementId);
}

// Symbol: ?SetElementText@CDHtmlDialog@@QEAAXPEB_WPEA_W@Z
extern "C" void MS_ABI impl__SetElementText_CDHtmlDialog__QEAAXPEB_WPEA_W_Z(
    CDHtmlDialog* pThis, const wchar_t* lpszElementId, wchar_t* lpszText) {
    pThis->SetElementText(lpszElementId, lpszText);
}

// Symbol: ?GetElementHtml@CDHtmlDialog@@QEAAPEA_WPEB_W@Z
extern "C" wchar_t* MS_ABI impl__GetElementHtml_CDHtmlDialog__QEAAPEA_WPEB_W_Z(
    CDHtmlDialog* pThis, const wchar_t* lpszElementId) {
    return pThis->GetElementHtml(lpszElementId);
}

// Symbol: ?SetElementHtml@CDHtmlDialog@@QEAAXPEB_WPEA_W@Z
extern "C" void MS_ABI impl__SetElementHtml_CDHtmlDialog__QEAAXPEB_WPEA_W_Z(
    CDHtmlDialog* pThis, const wchar_t* lpszElementId, wchar_t* lpszHtml) {
    pThis->SetElementHtml(lpszElementId, lpszHtml);
}

// Symbol: ?SetElementHtml@CDHtmlDialog@@QEAAXPEAUIUnknown@@PEA_W@Z
extern "C" void MS_ABI impl__SetElementHtml_CDHtmlDialog__QEAAXPEAUIUnknown__PEA_W_Z(
    CDHtmlDialog* pThis, IUnknown* punkElem, wchar_t* lpszHtml) {
    pThis->SetElementHtml(punkElem, lpszHtml);
}

// Symbol: ?SetElementText@CDHtmlDialog@@QEAAXPEAUIUnknown@@PEA_W@Z
extern "C" void MS_ABI impl__SetElementText_CDHtmlDialog__QEAAXPEAUIUnknown__PEA_W_Z(
    CDHtmlDialog* pThis, IUnknown* punkElem, wchar_t* lpszText) {
    pThis->SetElementText(punkElem, lpszText);
}

// Symbol: ?GetControlDispatch@CDHtmlDialog@@QEAAJPEB_WPEAPEAUIDispatch@@@Z
extern "C" long MS_ABI impl__GetControlDispatch_CDHtmlDialog__QEAAJPEB_WPEAPEAUIDispatch___Z(
    CDHtmlDialog* pThis, const wchar_t* lpszId, IDispatch** ppDisp) {
    return pThis->GetControlDispatch(lpszId, ppDisp);
}

// Symbol: ?GetControlProperty@CDHtmlDialog@@QEAA?AUtagVARIANT@@PEB_W0@Z
extern "C" void MS_ABI impl__GetControlProperty_CDHtmlDialog__QEAA_AUtagVARIANT__PEB_W0_Z(
    VARIANT* pRet, CDHtmlDialog* pThis, const wchar_t* lpszId, const wchar_t* lpszPropName) {
    new (pRet) VARIANT(pThis->GetControlProperty(lpszId, lpszPropName));
}

// Symbol: ?GetControlProperty@CDHtmlDialog@@QEAA?AUtagVARIANT@@PEB_WJ@Z
extern "C" void MS_ABI impl__GetControlProperty_CDHtmlDialog__QEAA_AUtagVARIANT__PEB_WJ_Z(
    VARIANT* pRet, CDHtmlDialog* pThis, const wchar_t* lpszId, long dispId) {
    new (pRet) VARIANT(pThis->GetControlProperty(lpszId, (DISPID)dispId));
}

// Symbol: ?SetControlProperty@CDHtmlDialog@@QEAAXPEB_W0PEAUtagVARIANT@@@Z
extern "C" void MS_ABI impl__SetControlProperty_CDHtmlDialog__QEAAXPEB_W0PEAUtagVARIANT___Z(
    CDHtmlDialog* pThis, const wchar_t* lpszId, const wchar_t* lpszPropName, VARIANT* pVar) {
    pThis->SetControlProperty(lpszId, lpszPropName, pVar);
}

// Symbol: ?SetControlProperty@CDHtmlDialog@@QEAAXPEB_WJPEAUtagVARIANT@@@Z
extern "C" void MS_ABI impl__SetControlProperty_CDHtmlDialog__QEAAXPEB_WJPEAUtagVARIANT___Z(
    CDHtmlDialog* pThis, const wchar_t* lpszId, long dispId, VARIANT* pVar) {
    pThis->SetControlProperty(lpszId, (DISPID)dispId, pVar);
}

// Symbol: ?SetControlProperty@CDHtmlDialog@@QEAAXPEAUIDispatch@@JPEAUtagVARIANT@@@Z
extern "C" void MS_ABI impl__SetControlProperty_CDHtmlDialog__QEAAXPEAUIDispatch__JPEAUtagVARIANT___Z(
    CDHtmlDialog* pThis, IDispatch* pDisp, long dispId, VARIANT* pVar) {
    pThis->SetControlProperty(pDisp, (DISPID)dispId, pVar);
}

// Symbol: ?GetEvent@CDHtmlDialog@@QEAAJPEAPEAUIHTMLEventObj@@@Z
extern "C" long MS_ABI impl__GetEvent_CDHtmlDialog__QEAAJPEAPEAUIHTMLEventObj___Z(
    CDHtmlDialog* pThis, IHTMLEventObj** ppEventObj) {
    return pThis->GetEvent(ppEventObj);
}

// Symbol: ?GetDHtmlDocument@CDHtmlDialog@@UEAAJPEAPEAUIHTMLDocument2@@@Z
extern "C" long MS_ABI impl__GetDHtmlDocument_CDHtmlDialog__UEAAJPEAPEAUIHTMLDocument2___Z(
    CDHtmlDialog* pThis, IHTMLDocument2** ppDocument) {
    return pThis->GetDHtmlDocument(ppDocument);
}

// Symbol: ?OnDDXError@CDHtmlDialog@@IEAAXPEB_WIH@Z
extern "C" void MS_ABI impl__OnDDXError_CDHtmlDialog__IEAAXPEB_WIH_Z(
    CDHtmlDialog* pThis, const wchar_t* lpszId, unsigned int nErrorID, int bSaveAndValidate) {
    static_cast<CDHtmlDialogAccess*>(pThis)->OnDDXError(lpszId, nErrorID, bSaveAndValidate);
}

// Symbol: ?OnBeforeNavigate@CDHtmlDialog@@UEAAXPEAUIDispatch@@PEB_W@Z
extern "C" void MS_ABI impl__OnBeforeNavigate_CDHtmlDialog__UEAAXPEAUIDispatch__PEB_W_Z(
    CDHtmlDialog* pThis, IDispatch* pDisp, const wchar_t* lpszURL) {
    pThis->OnBeforeNavigate(pDisp, lpszURL);
}

// Symbol: ?OnNavigateComplete@CDHtmlDialog@@UEAAXPEAUIDispatch@@PEB_W@Z
extern "C" void MS_ABI impl__OnNavigateComplete_CDHtmlDialog__UEAAXPEAUIDispatch__PEB_W_Z(
    CDHtmlDialog* pThis, IDispatch* pDisp, const wchar_t* lpszURL) {
    pThis->OnNavigateComplete(pDisp, lpszURL);
}

// Symbol: ?OnDocumentComplete@CDHtmlDialog@@UEAAXPEAUIDispatch@@PEB_W@Z
extern "C" void MS_ABI impl__OnDocumentComplete_CDHtmlDialog__UEAAXPEAUIDispatch__PEB_W_Z(
    CDHtmlDialog* pThis, IDispatch* pDisp, const wchar_t* lpszURL) {
    pThis->OnDocumentComplete(pDisp, lpszURL);
}

// Symbol: ?FindSinkForObject@CDHtmlDialog@@IEAAHPEB_W@Z
extern "C" int MS_ABI impl__FindSinkForObject_CDHtmlDialog__IEAAHPEB_W_Z(
    CDHtmlDialog* pThis, const wchar_t* lpszId) {
    return static_cast<CDHtmlDialogAccess*>(pThis)->FindSinkForObject(lpszId);
}

// Symbol: ?SetFocusToElement@CDHtmlDialog@@IEAAXPEB_W@Z
extern "C" void MS_ABI impl__SetFocusToElement_CDHtmlDialog__IEAAXPEB_W_Z(
    CDHtmlDialog* pThis, const wchar_t* lpszId) {
    static_cast<CDHtmlDialogAccess*>(pThis)->SetFocusToElement(lpszId);
}

// Symbol: ?Select_FindString@CDHtmlDialog@@IEAAJPEAUIHTMLSelectElement@@PEA_WH@Z
extern "C" long MS_ABI impl__Select_FindString_CDHtmlDialog__IEAAJPEAUIHTMLSelectElement__PEA_WH_Z(
    CDHtmlDialog* pThis, IHTMLSelectElement* pSelect, wchar_t* lpszFind, int bExact) {
    return static_cast<CDHtmlDialogAccess*>(pThis)->Select_FindString(pSelect, lpszFind, bExact);
}

// Symbol: ?GetHostInfo@CDHtmlDialog@@UEAAJPEAU_DOCHOSTUIINFO@@@Z
extern "C" long MS_ABI impl__GetHostInfo_CDHtmlDialog__UEAAJPEAU_DOCHOSTUIINFO___Z(
    CDHtmlDialog* pThis, DOCHOSTUIINFO* pInfo) {
    return pThis->GetHostInfo(pInfo);
}

// Symbol: ?GetOptionKeyPath@CDHtmlDialog@@UEAAJPEAPEA_WK@Z
extern "C" long MS_ABI impl__GetOptionKeyPath_CDHtmlDialog__UEAAJPEAPEA_WK_Z(
    CDHtmlDialog* pThis, wchar_t** ppszKey, unsigned long dwReserved) {
    return pThis->GetOptionKeyPath(ppszKey, dwReserved);
}

// Symbol: ?TranslateUrl@CDHtmlDialog@@UEAAJKPEA_WPEAPEA_W@Z
extern "C" long MS_ABI impl__TranslateUrl_CDHtmlDialog__UEAAJKPEA_WPEAPEA_W_Z(
    CDHtmlDialog* pThis, unsigned long dwTranslate, wchar_t* pchURLIn, wchar_t** ppchURLOut) {
    return pThis->TranslateUrl(dwTranslate, pchURLIn, ppchURLOut);
}

// Symbol: ?ShowContextMenu@CDHtmlDialog@@UEAAJKPEAUtagPOINT@@PEAUIUnknown@@PEAUIDispatch@@@Z
extern "C" long MS_ABI impl__ShowContextMenu_CDHtmlDialog__UEAAJKPEAUtagPOINT__PEAUIUnknown__PEAUIDispatch___Z(
    CDHtmlDialog* pThis, unsigned long dwID, POINT* ppt, IUnknown* pReserved, IDispatch* pDisp) {
    return pThis->ShowContextMenu(dwID, ppt, pReserved, pDisp);
}

// Symbol: ?ShowUI@CDHtmlDialog@@UEAAJKPEAUIOleInPlaceActiveObject@@PEAUIOleCommandTarget@@PEAUIOleInPlaceFrame@@PEAUIOleInPlaceUIWindow@@@Z
extern "C" long MS_ABI impl__ShowUI_CDHtmlDialog__UEAAJKPEAUIOleInPlaceActiveObject__PEAUIOleCommandTarget__PEAUIOleInPlaceFrame__PEAUIOleInPlaceUIWindow___Z(
    CDHtmlDialog* pThis, unsigned long dwID, IOleInPlaceActiveObject* pActiveObject,
    IOleCommandTarget* pCommandTarget, IOleInPlaceFrame* pFrame,
    IOleInPlaceUIWindow* pDoc) {
    return pThis->ShowUI(dwID, pActiveObject, pCommandTarget, pFrame, pDoc);
}

// Symbol: ?HideUI@CDHtmlDialog@@UEAAJXZ
extern "C" long MS_ABI impl__HideUI_CDHtmlDialog__UEAAJXZ(CDHtmlDialog* pThis) {
    return pThis->HideUI();
}

// Symbol: ?UpdateUI@CDHtmlDialog@@UEAAJXZ
extern "C" long MS_ABI impl__UpdateUI_CDHtmlDialog__UEAAJXZ(CDHtmlDialog* pThis) {
    return pThis->UpdateUI();
}

// Symbol: ?EnableModeless@CDHtmlDialog@@UEAAJH@Z
extern "C" long MS_ABI impl__EnableModeless_CDHtmlDialog__UEAAJH_Z(
    CDHtmlDialog* pThis, int fEnable) {
    return pThis->EnableModeless(fEnable);
}

// Symbol: ?OnDocWindowActivate@CDHtmlDialog@@UEAAJH@Z
extern "C" long MS_ABI impl__OnDocWindowActivate_CDHtmlDialog__UEAAJH_Z(
    CDHtmlDialog* pThis, int fActivate) {
    return pThis->OnDocWindowActivate(fActivate);
}

// Symbol: ?OnFrameWindowActivate@CDHtmlDialog@@UEAAJH@Z
extern "C" long MS_ABI impl__OnFrameWindowActivate_CDHtmlDialog__UEAAJH_Z(
    CDHtmlDialog* pThis, int fActivate) {
    return pThis->OnFrameWindowActivate(fActivate);
}

// Symbol: ?ResizeBorder@CDHtmlDialog@@UEAAJPEBUtagRECT@@PEAUIOleInPlaceUIWindow@@H@Z
extern "C" long MS_ABI impl__ResizeBorder_CDHtmlDialog__UEAAJPEBUtagRECT__PEAUIOleInPlaceUIWindow__H_Z(
    CDHtmlDialog* pThis, const RECT* prcBorder, IOleInPlaceUIWindow* pUIWindow, int fRameWindow) {
    return pThis->ResizeBorder(prcBorder, pUIWindow, fRameWindow);
}

// Symbol: ?TranslateAcceleratorW@CDHtmlDialog@@UEAAJPEAUtagMSG@@PEBU_GUID@@K@Z
extern "C" long MS_ABI impl__TranslateAcceleratorW_CDHtmlDialog__UEAAJPEAUtagMSG__PEBU_GUID__K_Z(
    CDHtmlDialog* pThis, LPMSG lpMsg, const GUID* pguidCmdGroup, unsigned long nCmdID) {
    return pThis->TranslateAcceleratorW(lpMsg, pguidCmdGroup, nCmdID);
}

// Symbol: ?GetDropTarget@CDHtmlDialog@@UEAAJPEAUIDropTarget@@PEAPEAU2@@Z
extern "C" long MS_ABI impl__GetDropTarget_CDHtmlDialog__UEAAJPEAUIDropTarget__PEAPEAU2__Z(
    CDHtmlDialog* pThis, IDropTarget* pDropTarget, IDropTarget** ppDropTarget) {
    return pThis->GetDropTarget(pDropTarget, ppDropTarget);
}

// Symbol: ?GetExternal@CDHtmlDialog@@UEAAJPEAPEAUIDispatch@@@Z
extern "C" long MS_ABI impl__GetExternal_CDHtmlDialog__UEAAJPEAPEAUIDispatch___Z(
    CDHtmlDialog* pThis, IDispatch** ppDispatch) {
    return pThis->GetExternal(ppDispatch);
}

// Symbol: ?FilterDataObject@CDHtmlDialog@@UEAAJPEAUIDataObject@@PEAPEAU2@@Z
extern "C" long MS_ABI impl__FilterDataObject_CDHtmlDialog__UEAAJPEAUIDataObject__PEAPEAU2__Z(
    CDHtmlDialog* pThis, IDataObject* pDO, IDataObject** ppDORet) {
    return pThis->FilterDataObject(pDO, ppDORet);
}

// Symbol: ?IsExternalDispatchSafe@CDHtmlDialog@@UEAAHXZ
extern "C" int MS_ABI impl__IsExternalDispatchSafe_CDHtmlDialog__UEAAHXZ(CDHtmlDialog* pThis) {
    return SUCCEEDED(pThis->IsExternalDispatchSafe()) ? TRUE : FALSE;
}

// Symbol: ?CanAccessExternal@CDHtmlDialog@@UEAAHXZ
extern "C" int MS_ABI impl__CanAccessExternal_CDHtmlDialog__UEAAHXZ(CDHtmlDialog* pThis) {
    return SUCCEEDED(pThis->CanAccessExternal()) ? TRUE : FALSE;
}

// Symbol: ?SetExternalDispatch@CDHtmlDialog@@QEAAXPEAUIDispatch@@@Z
extern "C" void MS_ABI impl__SetExternalDispatch_CDHtmlDialog__QEAAXPEAUIDispatch___Z(
    CDHtmlDialog* pThis, IDispatch* pDisp) {
    pThis->SetExternalDispatch(pDisp);
}

// Symbol: ?SetHostFlags@CDHtmlDialog@@QEAAXK@Z
extern "C" void MS_ABI impl__SetHostFlags_CDHtmlDialog__QEAAXK_Z(
    CDHtmlDialog* pThis, unsigned long dwFlags) {
    pThis->SetHostFlags(dwFlags);
}

// Symbol: ?OnDestroy@CDHtmlDialog@@IEAAXXZ
extern "C" void MS_ABI impl__OnDestroy_CDHtmlDialog__IEAAXXZ(CDHtmlDialog* pThis) {
    static_cast<CDHtmlDialogAccess*>(pThis)->OnDestroy();
}
