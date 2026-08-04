// CRichEditCtrl — OpenMFC implementation.
// Sources: viewrich.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/ViewrichSupport.h"

// Symbol: ?GetSelText@CRichEditCtrl@@QEBA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@XZ
extern "C" void MS_ABI impl__GetSelText_CRichEditCtrl__QEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__XZ(
    CString* pRet, const CRichEditCtrl* pThis) {
    new (pRet) CString(pThis ? pThis->GetSelText() : CString());
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
CRichEditCtrl::CRichEditCtrl() {
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
