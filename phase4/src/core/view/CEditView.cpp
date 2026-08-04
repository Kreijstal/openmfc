// CEditView — OpenMFC implementation.
// Sources: docview.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/DocviewSupport.h"


// Implementations this unit calls that are defined with their own class.
extern "C" void MS_ABI impl__CalcWindowRect_CView__UEAAXPEAUtagRECT__I_Z(CView* pThis, RECT* rect, unsigned int adjustType);

// Default Constructor
// Symbol: ??0CEditView@@QEAA@XZ
extern "C" void MS_ABI impl___0CEditView__QEAA_XZ(CEditView* pThis) {
    if (!pThis) return;
    impl___0CView__QEAA_XZ(pThis);
    pThis->m_pEditCtrl = nullptr;
}
// Destructor
// Symbol: ??1CEditView@@UEAA@XZ
extern "C" void MS_ABI impl___1CEditView__UEAA_XZ(CEditView* pThis) {
    impl___1CView__UEAA_XZ(pThis);
}
// Symbol: ?CreateObject@CEditView@@SAPEAVCObject@@XZ
extern "C" CObject* MS_ABI impl__CreateObject_CEditView__SAPEAVCObject__XZ() {
    return new CEditView();
}
// Symbol: ?GetRuntimeClass@CEditView@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CEditView__UEBAPEAUCRuntimeClass__XZ(const CEditView*) { return &CEditView::classCEditView; }
// Symbol: ?GetThisClass@CEditView@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CEditView__SAPEAUCRuntimeClass__XZ() { return &CEditView::classCEditView; }
// Symbol: ?GetMessageMap@CEditView@@MEBAPEBUAFX_MSGMAP@@XZ
extern "C" const AFX_MSGMAP* MS_ABI impl__GetMessageMap_CEditView__MEBAPEBUAFX_MSGMAP__XZ(const CEditView*) { return &g_docviewEmptyMap; }
// Symbol: ?GetThisMessageMap@CEditView@@KAPEBUAFX_MSGMAP@@XZ
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CEditView__KAPEBUAFX_MSGMAP__XZ() { return &g_docviewEmptyMap; }
// CEditView residuals and static data.
// Symbol: ?dwStyleDefault@CEditView@@2KB
extern "C" unsigned long impl__dwStyleDefault_CEditView__2KB = WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_MULTILINE | ES_AUTOVSCROLL | ES_AUTOHSCROLL;
// Symbol: ?nMaxSize@CEditView@@2IB
extern "C" unsigned int impl__nMaxSize_CEditView__2IB = 0x00ffffff;
// Symbol: ?GetBufferLength@CEditView@@QEBAIXZ
extern "C" unsigned int MS_ABI impl__GetBufferLength_CEditView__QEBAIXZ(const CEditView* pThis) { return static_cast<unsigned int>(EditViewText(pThis).size()); }
// Symbol: ?GetSelectedText@CEditView@@QEBAXAEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@@Z
extern "C" void MS_ABI impl__GetSelectedText_CEditView__QEBAXAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(const CEditView* pThis, CString* out) { if (out) const_cast<CEditView*>(pThis)->GetSelectedText(*out); }
// Symbol: ?FindTextW@CEditView@@QEAAHPEB_WHH@Z
extern "C" int MS_ABI impl__FindTextW_CEditView__QEAAHPEB_WHH_Z(CEditView* pThis, const wchar_t* find, int next, int caseSensitive) {
    if (!pThis || !find) return -1;
    std::wstring text = EditViewText(pThis), needle(find);
    if (!caseSensitive) { std::transform(text.begin(),text.end(),text.begin(),::towlower); std::transform(needle.begin(),needle.end(),needle.begin(),::towlower); }
    size_t start = 0;
    if (pThis->m_hWnd) { DWORD s=0,e=0; ::SendMessageW(pThis->m_hWnd, EM_GETSEL, (WPARAM)&s, (LPARAM)&e); start = next ? e : s; }
    size_t pos = next ? text.find(needle, start) : text.rfind(needle, start);
    if (pos == std::wstring::npos) return -1;
    if (pThis->m_hWnd) ::SendMessageW(pThis->m_hWnd, EM_SETSEL, pos, pos + needle.size());
    return static_cast<int>(pos);
}
// Symbol: ?PrintInsideRect@CEditView@@QEAAIPEAVCDC@@AEAUtagRECT@@II@Z
extern "C" int MS_ABI impl__PrintInsideRect_CEditView__QEAAIPEAVCDC__AEAUtagRECT__II_Z(
    CEditView* pThis, CDC* pDC, RECT* rectLayout, int startIndex, int endIndex) {
    if (!pThis || !pDC || !rectLayout || !pThis->m_hWnd) return 0;

    int len = ::GetWindowTextLengthW(pThis->m_hWnd);
    if (len <= 0 || !pDC->m_hDC) return 0;

    std::wstring text(len + 1, L'\0');
    if (::GetWindowTextW(pThis->m_hWnd, text.data(), len + 1) <= 0) return 0;
    text.resize(static_cast<size_t>(::wcslen(text.c_str())));

    int start = (startIndex < 0) ? 0 : startIndex;
    int end = (endIndex < 0) ? len : endIndex;
    if (start > end) std::swap(start, end);
    start = std::min(start, len);
    end = std::min(end, len);
    if (start > end) return 0;

    std::wstring viewText = text.substr(static_cast<size_t>(start), static_cast<size_t>(end - start));
    RECT rc = *rectLayout;
    int height = ::DrawTextW(pDC->m_hDC, viewText.c_str(),
                             static_cast<int>(viewText.size()),
                             &rc, DT_TOP | DT_LEFT | DT_WORDBREAK);
    rectLayout->bottom = rc.top + height;
    return height;
}
// Symbol: ?LockBuffer@CEditView@@QEBAPEB_WXZ
extern "C" const wchar_t* MS_ABI impl__LockBuffer_CEditView__QEBAPEB_WXZ(const CEditView* pThis) { if (!pThis) return L""; auto& s=g_editViewExtraStates[pThis]; s.buffer=EditViewText(pThis); s.locked=s.buffer.c_str(); return s.locked; }
// Symbol: ?UnlockBuffer@CEditView@@QEBAXXZ
extern "C" void MS_ABI impl__UnlockBuffer_CEditView__QEBAXXZ(const CEditView* pThis) { if (pThis) g_editViewExtraStates[pThis].locked=nullptr; }
// Symbol: ?DeleteContents@CEditView@@UEAAXXZ
extern "C" void MS_ABI impl__DeleteContents_CEditView__UEAAXXZ(CEditView* pThis) { SetEditViewText(pThis, L""); }
// Symbol: ?ReadFromArchive@CEditView@@QEAAXAEAVCArchive@@I@Z
extern "C" void MS_ABI impl__ReadFromArchive_CEditView__QEAAXAEAVCArchive__I_Z(CEditView* pThis, CArchive* ar, unsigned int) { if (!pThis||!ar) return; CString s; (*ar)>>s; SetEditViewText(pThis, static_cast<const wchar_t*>(s)); }
// Symbol: ?WriteToArchive@CEditView@@QEAAXAEAVCArchive@@@Z
extern "C" void MS_ABI impl__WriteToArchive_CEditView__QEAAXAEAVCArchive___Z(CEditView* pThis, CArchive* ar) { if (!pThis||!ar) return; CString s(EditViewText(pThis).c_str()); (*ar)<<s; }
// Symbol: ?SerializeRaw@CEditView@@QEAAXAEAVCArchive@@@Z
extern "C" void MS_ABI impl__SerializeRaw_CEditView__QEAAXAEAVCArchive___Z(CEditView* pThis, CArchive* ar) { if (!pThis||!ar) return; if (ar->IsStoring()) impl__WriteToArchive_CEditView__QEAAXAEAVCArchive___Z(pThis, ar); else impl__ReadFromArchive_CEditView__QEAAXAEAVCArchive__I_Z(pThis, ar, 0); }
// Symbol: ?SameAsSelected@CEditView@@IEAAHPEB_WH@Z
extern "C" int MS_ABI impl__SameAsSelected_CEditView__IEAAHPEB_WH_Z(CEditView* pThis, const wchar_t* text, int caseSensitive) { CString sel; pThis->GetSelectedText(sel); return caseSensitive ? wcscmp(sel, text?text:L"")==0 : _wcsicmp(sel, text?text:L"")==0; }
// Symbol: ?OnReplaceSel@CEditView@@MEAAXPEB_WHH0@Z
extern "C" void MS_ABI impl__OnReplaceSel_CEditView__MEAAXPEB_WHH0_Z(CEditView* pThis, const wchar_t* find, int next, int caseSensitive, const wchar_t* repl) { if (!pThis) return; int pos=impl__FindTextW_CEditView__QEAAHPEB_WHH_Z(pThis, find, next, caseSensitive); if (pos>=0 && pThis->m_hWnd) ::SendMessageW(pThis->m_hWnd, EM_REPLACESEL, TRUE, (LPARAM)(repl?repl:L"")); }
// Symbol: ?OnReplaceAll@CEditView@@MEAAXPEB_W0H@Z
extern "C" void MS_ABI impl__OnReplaceAll_CEditView__MEAAXPEB_W0H_Z(CEditView* pThis, const wchar_t* find, const wchar_t* repl, int caseSensitive) { if (!pThis||!find||!*find) return; std::wstring text=EditViewText(pThis), needle(find), replacement(repl?repl:L""); size_t pos=0; while ((pos=text.find(needle,pos))!=std::wstring::npos) { text.replace(pos,needle.size(),replacement); pos+=replacement.size(); } SetEditViewText(pThis,text); (void)caseSensitive; }
// Edit command handlers.
// Symbol: ?OnEditClear@CEditView@@IEAAXXZ
extern "C" void MS_ABI impl__OnEditClear_CEditView__IEAAXXZ(CEditView* pThis) { if (pThis&&pThis->m_hWnd) ::SendMessageW(pThis->m_hWnd, WM_CLEAR,0,0); }
// Symbol: ?OnEditCopy@CEditView@@IEAAXXZ
extern "C" void MS_ABI impl__OnEditCopy_CEditView__IEAAXXZ(CEditView* pThis) { if (pThis&&pThis->m_hWnd) ::SendMessageW(pThis->m_hWnd, WM_COPY,0,0); }
// Symbol: ?OnEditCut@CEditView@@IEAAXXZ
extern "C" void MS_ABI impl__OnEditCut_CEditView__IEAAXXZ(CEditView* pThis) { if (pThis&&pThis->m_hWnd) ::SendMessageW(pThis->m_hWnd, WM_CUT,0,0); }
// Symbol: ?OnEditPaste@CEditView@@IEAAXXZ
extern "C" void MS_ABI impl__OnEditPaste_CEditView__IEAAXXZ(CEditView* pThis) { if (pThis&&pThis->m_hWnd) ::SendMessageW(pThis->m_hWnd, WM_PASTE,0,0); }
// Symbol: ?OnEditUndo@CEditView@@IEAAXXZ
extern "C" void MS_ABI impl__OnEditUndo_CEditView__IEAAXXZ(CEditView* pThis) { if (pThis&&pThis->m_hWnd) ::SendMessageW(pThis->m_hWnd, EM_UNDO,0,0); }
// Symbol: ?OnEditSelectAll@CEditView@@IEAAXXZ
extern "C" void MS_ABI impl__OnEditSelectAll_CEditView__IEAAXXZ(CEditView* pThis) { if (pThis&&pThis->m_hWnd) ::SendMessageW(pThis->m_hWnd, EM_SETSEL,0,-1); }
// Symbol: ?OnEditChange@CEditView@@IEAAHXZ
extern "C" int MS_ABI impl__OnEditChange_CEditView__IEAAHXZ(CEditView* pThis) { if (pThis&&pThis->m_pDocument) pThis->m_pDocument->SetModifiedFlag(TRUE); if (pThis) g_editViewExtraStates[pThis].buffer=EditViewText(pThis); return TRUE; }
// Symbol: ?OnEditFind@CEditView@@IEAAXXZ
extern "C" void MS_ABI impl__OnEditFind_CEditView__IEAAXXZ(CEditView* pThis) { if (pThis) g_editViewExtraStates[pThis].findText.clear(); }
// Symbol: ?OnEditReplace@CEditView@@IEAAXXZ
extern "C" void MS_ABI impl__OnEditReplace_CEditView__IEAAXXZ(CEditView* pThis) { if (pThis) g_editViewExtraStates[pThis].replaceText.clear(); }
// Symbol: ?OnEditRepeat@CEditView@@IEAAXXZ
extern "C" void MS_ABI impl__OnEditRepeat_CEditView__IEAAXXZ(CEditView* pThis) { auto& s=g_editViewExtraStates[pThis]; if (!s.findText.empty()) impl__FindTextW_CEditView__QEAAHPEB_WHH_Z(pThis, s.findText.c_str(), TRUE, TRUE); }
// Symbol: ?OnEditFindReplace@CEditView@@IEAAXH@Z
extern "C" void MS_ABI impl__OnEditFindReplace_CEditView__IEAAXH_Z(CEditView* pThis, int replace) { if (pThis) { auto& s=g_editViewExtraStates[pThis]; if (replace) s.replaceText.clear(); else s.findText.clear(); } }
// Symbol: ?OnFindReplaceCmd@CEditView@@IEAA_J_K_J@Z
extern "C" intptr_t MS_ABI impl__OnFindReplaceCmd_CEditView__IEAA_J_K_J_Z(CEditView* pThis, uintptr_t, intptr_t) { if (!pThis) return 0; auto& s=g_editViewExtraStates[pThis]; return s.findText.empty() ? 0 : impl__FindTextW_CEditView__QEAAHPEB_WHH_Z(pThis, s.findText.c_str(), TRUE, TRUE) >= 0; }
// Symbol: ?InitializeReplace@CEditView@@IEAAHXZ
extern "C" int MS_ABI impl__InitializeReplace_CEditView__IEAAHXZ(CEditView* pThis) { if (pThis) { g_editViewExtraStates[pThis].findText.clear(); g_editViewExtraStates[pThis].replaceText.clear(); } return TRUE; }
// Symbol: ?OnFindNext@CEditView@@MEAAXPEB_WHH@Z
extern "C" void MS_ABI impl__OnFindNext_CEditView__MEAAXPEB_WHH_Z(CEditView* pThis, const wchar_t* find, int next, int caseSensitive) { if (pThis) { g_editViewExtraStates[pThis].findText=find?find:L""; impl__FindTextW_CEditView__QEAAHPEB_WHH_Z(pThis, find, next, caseSensitive); } }
// Symbol: ?OnTextNotFound@CEditView@@MEAAXPEB_W@Z
extern "C" void MS_ABI impl__OnTextNotFound_CEditView__MEAAXPEB_W_Z(CEditView*, const wchar_t*) { ::MessageBeep(MB_ICONINFORMATION); }
// Symbol: ?OnPreparePrinting@CEditView@@MEAAHPEAUCPrintInfo@@@Z
extern "C" int MS_ABI impl__OnPreparePrinting_CEditView__MEAAHPEAUCPrintInfo___Z(
    CEditView* pThis, void* pInfo)
{
    return impl__OnPreparePrinting_CView__UEAAHPEAX_Z(pThis, pInfo);
}
// Symbol: ?OnBeginPrinting@CEditView@@MEAAXPEAVCDC@@PEAUCPrintInfo@@@Z
extern "C" void MS_ABI impl__OnBeginPrinting_CEditView__MEAAXPEAVCDC__PEAUCPrintInfo___Z(CEditView* pThis, CDC* dc, void*) { if (pThis && dc && dc->m_hDC) { auto it = g_editViewExtraStates.find(pThis); CFont* font = it == g_editViewExtraStates.end() ? nullptr : it->second.printerFont; if (font) ::SelectObject(dc->m_hDC, font->GetSafeHandle()); } }
// Symbol: ?OnEndPrinting@CEditView@@MEAAXPEAVCDC@@PEAUCPrintInfo@@@Z
extern "C" void MS_ABI impl__OnEndPrinting_CEditView__MEAAXPEAVCDC__PEAUCPrintInfo___Z(CEditView* pThis, CDC*, void*) { if (pThis) g_editViewExtraStates[pThis].pageCount = 0; }
// Symbol: ?OnPrepareDC@CEditView@@UEAAXPEAVCDC@@PEAUCPrintInfo@@@Z
extern "C" void MS_ABI impl__OnPrepareDC_CEditView__UEAAXPEAVCDC__PEAUCPrintInfo___Z(CEditView* pThis, CDC* dc, void*) { if (pThis && dc && dc->m_hDC) { auto it = g_editViewExtraStates.find(pThis); CFont* font = it == g_editViewExtraStates.end() ? nullptr : it->second.printerFont; if (font) ::SelectObject(dc->m_hDC, font->GetSafeHandle()); } }
// Symbol: ?OnPrint@CEditView@@MEAAXPEAVCDC@@PEAUCPrintInfo@@@Z
extern "C" void MS_ABI impl__OnPrint_CEditView__MEAAXPEAVCDC__PEAUCPrintInfo___Z(CEditView* pThis, CDC* dc, void*) { if (!pThis||!dc||!dc->m_hDC) return; std::wstring t=EditViewText(pThis); RECT rc={0,0,10000,10000}; ::DrawTextW(dc->m_hDC,t.c_str(),-1,&rc,DT_LEFT|DT_TOP|DT_WORDBREAK); }
// Symbol: ?PaginateTo@CEditView@@IEAAHPEAVCDC@@PEAUCPrintInfo@@@Z
extern "C" int MS_ABI impl__PaginateTo_CEditView__IEAAHPEAVCDC__PEAUCPrintInfo___Z(CEditView* pThis, CDC*, void*) { if (pThis) g_editViewExtraStates[pThis].pageCount=1; return TRUE; }
// Symbol: ?SetPrinterFont@CEditView@@QEAAXPEAVCFont@@@Z
extern "C" void MS_ABI impl__SetPrinterFont_CEditView__QEAAXPEAVCFont___Z(CEditView* pThis, CFont* font) { if (pThis) g_editViewExtraStates[pThis].printerFont=font; }
// Symbol: ?GetPrinterFont@CEditView@@QEBAPEAVCFont@@XZ
extern "C" CFont* MS_ABI impl__GetPrinterFont_CEditView__QEBAPEAVCFont__XZ(const CEditView* pThis) { auto it=g_editViewExtraStates.find(pThis); return it==g_editViewExtraStates.end()?nullptr:it->second.printerFont; }
// Symbol: ?SetTabStops@CEditView@@QEAAXH@Z
extern "C" void MS_ABI impl__SetTabStops_CEditView__QEAAXH_Z(CEditView* pThis, int tabs) { if (pThis&&pThis->m_hWnd) ::SendMessageW(pThis->m_hWnd, EM_SETTABSTOPS,1,(LPARAM)&tabs); }
// Symbol: ?OnCreate@CEditView@@IEAAHPEAUtagCREATESTRUCTW@@@Z
extern "C" int MS_ABI impl__OnCreate_CEditView__IEAAHPEAUtagCREATESTRUCTW___Z(CEditView* pThis, CREATESTRUCTW*) { if (!pThis||pThis->m_hWnd) return 0; pThis->m_hWnd=::CreateWindowExW(0,L"EDIT",L"",impl__dwStyleDefault_CEditView__2KB,0,0,0,0,nullptr,nullptr,AfxGetInstanceHandle(),nullptr); return pThis->m_hWnd?0:-1; }
// Symbol: ?OnDestroy@CEditView@@IEAAXXZ
extern "C" void MS_ABI impl__OnDestroy_CEditView__IEAAXXZ(CEditView* pThis) { if (pThis) g_editViewExtraStates.erase(pThis); }
// Symbol: ?CalcWindowRect@CEditView@@MEAAXPEAUtagRECT@@I@Z
extern "C" void MS_ABI impl__CalcWindowRect_CEditView__MEAAXPEAUtagRECT__I_Z(CEditView* pThis, RECT* rect, unsigned int adjustType) { impl__CalcWindowRect_CView__UEAAXPEAUtagRECT__I_Z(pThis, rect, adjustType); }
// Symbol: ?PreCreateWindow@CEditView@@MEAAHAEAUtagCREATESTRUCTW@@@Z
extern "C" int MS_ABI impl__PreCreateWindow_CEditView__MEAAHAEAUtagCREATESTRUCTW___Z(CEditView*, CREATESTRUCTW* cs) { if (cs) cs->style |= impl__dwStyleDefault_CEditView__2KB; return TRUE; }
// Symbol: ?OnSetFont@CEditView@@IEAAXPEAVCFont@@H@Z
extern "C" void MS_ABI impl__OnSetFont_CEditView__IEAAXPEAVCFont__H_Z(CEditView* pThis, CFont* font, int redraw) { if (pThis&&pThis->m_hWnd) ::SendMessageW(pThis->m_hWnd, WM_SETFONT, (WPARAM)(font?font->GetSafeHandle():nullptr), redraw); }
// Symbol: ?OnUpdateEditUndo@CEditView@@IEAAXPEAVCCmdUI@@@Z
extern "C" void MS_ABI impl__OnUpdateEditUndo_CEditView__IEAAXPEAVCCmdUI___Z(CEditView* pThis, CCmdUI* ui) { if (ui) impl__Enable_CCmdUI__UEAAXH_Z(ui, pThis && pThis->m_hWnd && ::SendMessageW(pThis->m_hWnd, EM_CANUNDO, 0, 0)); }
// Symbol: ?OnUpdateNeedClip@CEditView@@IEAAXPEAVCCmdUI@@@Z
extern "C" void MS_ABI impl__OnUpdateNeedClip_CEditView__IEAAXPEAVCCmdUI___Z(CEditView*, CCmdUI* ui) { if (ui) impl__Enable_CCmdUI__UEAAXH_Z(ui, ::IsClipboardFormatAvailable(CF_UNICODETEXT)); }
// Symbol: ?OnUpdateNeedFind@CEditView@@IEAAXPEAVCCmdUI@@@Z
extern "C" void MS_ABI impl__OnUpdateNeedFind_CEditView__IEAAXPEAVCCmdUI___Z(CEditView* pThis, CCmdUI* ui) { if (ui) impl__Enable_CCmdUI__UEAAXH_Z(ui, pThis && !EditViewText(pThis).empty()); }
// Symbol: ?OnUpdateNeedSel@CEditView@@IEAAXPEAVCCmdUI@@@Z
extern "C" void MS_ABI impl__OnUpdateNeedSel_CEditView__IEAAXPEAVCCmdUI___Z(CEditView* pThis, CCmdUI* ui) { DWORD s=0,e=0; if (pThis&&pThis->m_hWnd) ::SendMessageW(pThis->m_hWnd, EM_GETSEL, (WPARAM)&s, (LPARAM)&e); if (ui) impl__Enable_CCmdUI__UEAAXH_Z(ui, s != e); }
// Symbol: ?OnUpdateNeedText@CEditView@@IEAAXPEAVCCmdUI@@@Z
extern "C" void MS_ABI impl__OnUpdateNeedText_CEditView__IEAAXPEAVCCmdUI___Z(CEditView* pThis, CCmdUI* ui) { if (ui) impl__Enable_CCmdUI__UEAAXH_Z(ui, pThis && !EditViewText(pThis).empty()); }
CEditView::CEditView() { impl___0CEditView__QEAA_XZ(this); }
CEditView::~CEditView() { impl___1CEditView__UEAA_XZ(this); }
CEdit* CEditView::GetEditCtrl() const { return impl__GetEditCtrl_CEditView__QEBAPEAVCEdit__XZ(this); }
void CEditView::OnDraw(void* pDC) { impl__OnDraw_CEditView__UEAAXPEAX_Z(this, pDC); }
void CEditView::Serialize(CArchive& ar) {
    if (!m_hWnd) return;

    if (ar.IsStoring()) {
        // Get text from edit control and write to archive
        int nLen = ::GetWindowTextLengthW(m_hWnd);
        if (nLen > 0) {
            wchar_t* pBuf = new wchar_t[nLen + 1];
            ::GetWindowTextW(m_hWnd, pBuf, nLen + 1);
            CString str(pBuf);
            ar << str;
            delete[] pBuf;
        } else {
            CString str;
            ar << str;
        }
    } else {
        // Read text from archive and set in edit control
        CString str;
        ar >> str;
        ::SetWindowTextW(m_hWnd, (const wchar_t*)str);
    }
}
void CEditView::GetSelectedText(CString& strResult) const {
    strResult.Empty();
    if (!m_hWnd) return;

    DWORD dwStart = 0, dwEnd = 0;
    ::SendMessageW(m_hWnd, EM_GETSEL, (WPARAM)&dwStart, (LPARAM)&dwEnd);

    if (dwEnd > dwStart) {
        int nLen = ::GetWindowTextLengthW(m_hWnd);
        if (nLen > 0) {
            wchar_t* pBuf = new wchar_t[nLen + 1];
            ::GetWindowTextW(m_hWnd, pBuf, nLen + 1);

            // Extract selected portion
            int nSelLen = dwEnd - dwStart;
            wchar_t* pSel = new wchar_t[nSelLen + 1];
            wcsncpy(pSel, pBuf + dwStart, nSelLen);
            pSel[nSelLen] = L'\0';

            strResult = pSel;
            delete[] pSel;
            delete[] pBuf;
        }
    }
}
int CEditView::FindText(const wchar_t* lpszFind, int nStart, int nDirection) {
    if (!m_hWnd || !lpszFind || !lpszFind[0]) return -1;

    int nLen = ::GetWindowTextLengthW(m_hWnd);
    if (nLen <= 0 || nStart >= nLen) return -1;

    wchar_t* pBuf = new wchar_t[nLen + 1];
    ::GetWindowTextW(m_hWnd, pBuf, nLen + 1);

    int nFindLen = (int)wcslen(lpszFind);
    int nResult = -1;

    if (nDirection >= 0) {
        // Search forward
        for (int i = nStart; i <= nLen - nFindLen; i++) {
            if (wcsncmp(pBuf + i, lpszFind, nFindLen) == 0) {
                nResult = i;
                break;
            }
        }
    } else {
        // Search backward
        for (int i = nStart - nFindLen; i >= 0; i--) {
            if (wcsncmp(pBuf + i, lpszFind, nFindLen) == 0) {
                nResult = i;
                break;
            }
        }
    }

    delete[] pBuf;
    return nResult;
}
int CEditView::PrintInsideRect(void* pDC, struct tagRECT& rectLayout, struct tagRECT& rectDraw, int nPage) {
    (void)nPage;
    if (!m_hWnd || !pDC) return 0;

    HDC hDC = static_cast<HDC>(pDC);

    // Get text from edit control
    int nLen = ::GetWindowTextLengthW(m_hWnd);
    if (nLen <= 0) return 0;

    wchar_t* pBuf = new wchar_t[nLen + 1];
    ::GetWindowTextW(m_hWnd, pBuf, nLen + 1);

    // Draw text in the specified rectangle
    int nHeight = ::DrawTextW(hDC, pBuf, nLen, &rectDraw,
                              DT_LEFT | DT_TOP | DT_WORDBREAK);

    // Update layout rect with text height
    rectLayout.bottom = rectLayout.top + nHeight;

    delete[] pBuf;
    return nHeight;
}
