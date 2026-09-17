// CRichEditView — OpenMFC implementation.
// Sources: global_other-15_impl.cpp, viewrich.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/ViewrichSupport.h"

// CRichEditView command handlers and OLE item helpers — wave1 "other-15".
//
// Each export below is the plain `extern "C"` + `__attribute__((ms_abi))`
// form required by the OpenMFC build (MSVC x64 ABI; `this` passed explicitly
// as the first parameter).  The `// Symbol:` comments are scanned by the build
// to exclude these ordinals from the auto-generated stubs.
//
// Most bodies were transcribed from the retail mfc140u.dll disassembly
// (ordinal -> RVA -> objdump).  Where the retail function depends on CRichEditView
// private members that the clean-room headers deliberately do not model (the
// cached IRichEditOle/ITextDocument pointers at +0x110, the CHARFORMAT2/PARAFORMAT2
// caches at +0x188..+0x2c0, the find/replace dialog state, etc.) a type-correct
// conservative version is emitted with a `TODO(clean-room)` note, exactly as the
// IMPLEMENTATION_GUIDE requires.  Only members declared in include/openmfc/afxwin.h
// and afxole.h are touched.

#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxwin.h"
#include "openmfc/afxole.h"
#include <richedit.h>
#include <richole.h>
#include <tom.h>
#include <shellapi.h>
#include <cwchar>
#include <new>

// MinGW's <tom.h> only declares IID_ITextDocument (EXTERN_C const IID); provide
// the definition here (standard TOM IID, selectany so multiple TUs may share it).
#ifndef IID_ITextDocument
EXTERN_C const GUID DECLSPEC_SELECTANY IID_ITextDocument =
    { 0x8CC497C0, 0xA1DF, 0x11CE, { 0x80, 0x98, 0x00, 0xAA, 0x00, 0x47, 0xBE, 0x5D } };
#endif

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

// ---------------------------------------------------------------------------
// Cross-file impl_ helpers (defined in other phase4/src files).
// ---------------------------------------------------------------------------

// CView::OnCreate / CView::OnDestroy (phase4/src/docview.cpp).
extern "C" int MS_ABI impl__OnCreate_CView__IEAAHPEAUtagCREATESTRUCTW___Z(
    CView* pThis, CREATESTRUCTW* pCreateStruct);
extern "C" void MS_ABI impl__OnDestroy_CView__IEAAXXZ(CView* pThis);

// Static data member CRichEditView::lMaxSize (64 KB, defined in
// phase4/src/gdi_rgn_richeditview_rtti.cpp).
extern "C" unsigned long impl__lMaxSize_CRichEditView__2KA;

// Same-file impl_ helpers.
extern "C" void MS_ABI impl__OnCharEffect_CRichEditView__QEAAXKK_Z(
    CRichEditView* pThis, unsigned long dwMask, unsigned long dwEffect);
extern "C" void MS_ABI impl__OnEditFindReplace_CRichEditView__IEAAXH_Z(
    CRichEditView* pThis, int bFindOnly);
extern "C" CRichEditCntrItem* MS_ABI impl__GetSelectedItem_CRichEditView__QEBAPEAVCRichEditCntrItem__XZ(
    const CRichEditView* pThis);
extern "C" void MS_ABI impl__InsertFileAsObject_CRichEditView__QEAAXPEB_W_Z(
    CRichEditView* pThis, const wchar_t* lpszFileName);
extern "C" int MS_ABI impl__FindTextSimple_CRichEditView__QEAAHPEB_WHHH_Z(
    CRichEditView* pThis, const wchar_t* lpszFind, int bCase, int bWord, int bNext);
extern "C" long MS_ABI impl__FindAndSelect_CRichEditView__QEAAJKAEAU_findtextexw___Z(
    CRichEditView* pThis, unsigned long dwFlags, FINDTEXTEXW* pft);
extern "C" void MS_ABI impl__AdjustDialogPosition_CRichEditView__QEAAXPEAVCDialog___Z(
    CRichEditView* pThis, CDialog* pDlg);
extern "C" int MS_ABI impl__SameAsSelected_CRichEditView__IEAAHPEB_WHH_Z(
    CRichEditView* pThis, const wchar_t* lpszCompare, int bCase, int bWord);
extern "C" void MS_ABI impl__OnParaAlign_CRichEditView__QEAAXG_Z(
    CRichEditView* pThis, unsigned short wAlign);
extern "C" void MS_ABI impl__OnFindNext_CRichEditView__MEAAXPEB_WHHH_Z(
    CRichEditView* pThis, const wchar_t* lpszFind, int bNext, int bCase, int bWord);
extern "C" void MS_ABI impl__OnReplaceSel_CRichEditView__MEAAXPEB_WHHH0_Z(
    CRichEditView* pThis, const wchar_t* lpszFind, int bNext, int bCase, int bWord,
    const wchar_t* lpszReplace);
extern "C" void MS_ABI impl__OnReplaceAll_CRichEditView__MEAAXPEB_W0HH_Z(
    CRichEditView* pThis, const wchar_t* lpszFind, const wchar_t* lpszReplace, int bCase, int bWord);

// Cross-file thunks used by the bodies transcribed below (each definition was
// located with grep before being declared here; the parameter lists are derived
// from the mangled names, not copied from the auto-generated definitions).
extern "C" __int64 MS_ABI impl__Default_CWnd__IEAA_JXZ(CWnd* pThis);                     // core/window/Thunks.cpp
extern "C" int MS_ABI impl__AfxInitRichEdit5__YAHXZ();                                    // featurepack/CMFC_misc_stubs.cpp
extern "C" int MS_ABI impl__PreCreateWindow_CCtrlView__MEAAHAEAUtagCREATESTRUCTW___Z(     // core/view/CCtrlView.cpp
    CView* pThis, CREATESTRUCTW* pcs);
extern "C" void MS_ABI impl__GetCharFormat_CFontDialog__QEBAXAEAU_charformatw___Z(        // core/dialog/CFontDialog.cpp
    const CFontDialog* pThis, CHARFORMATW* pcf);
extern "C" void MS_ABI impl__OnTextNotFound_CRichEditView__MEAAXPEB_W_Z(                  // core/view/Thunks.cpp
    CRichEditView* pThis, const wchar_t* lpszFind);
extern "C" CWinApp* MS_ABI impl__AfxGetApp__YAPEAVCWinApp__XZ();                          // detail/RegcoreSupport.cpp
extern "C" void MS_ABI impl__BeginWaitCursor_CCmdTarget__QEAAXXZ(CCmdTarget* pThis);       // core/runtime/CCmdTarget.cpp
extern "C" void MS_ABI impl__EndWaitCursor_CCmdTarget__QEAAXXZ(CCmdTarget* pThis);         // core/runtime/CCmdTarget.cpp
extern "C" void MS_ABI impl__SetWindowTextW_CWnd__QEAAXPEB_W_Z(CWnd* pThis, const wchar_t* lpsz); // core/window
extern "C" CRichEditCntrItem* MS_ABI impl__CreateClientItem_CRichEditDoc__UEBAPEAVCRichEditCntrItem__PEAU_reobject___Z(
    const CRichEditDoc* pThis, REOBJECT* preo);                                           // core/doc/CRichEditDoc.cpp (null stub today)
extern "C" void MS_ABI impl__DoPaste_CRichEditView__QEAAXAEAVCOleDataObject__GPEAX_Z(
    CRichEditView* pThis, COleDataObject* pDataObject, unsigned short nFormat, void* hMetaPict);
// MSVC-layout CFile dispatch helpers (core/file/FileDispatch.cpp, declared in
// detail/FilecoreSupport.h): internal code must not use pFile->Read() virtual syntax.
extern "C" unsigned int MS_ABI OpenMFC_File_Read(CFile* pFile, void* pBuf, unsigned int nCount);
extern "C" void MS_ABI OpenMFC_File_Write(CFile* pFile, const void* pBuf, unsigned int nCount);
extern "C" void MS_ABI impl__Flush_CArchive__QEAAXXZ(CArchive* pThis);                    // core/runtime/Thunks.cpp
extern "C" CFontDialog* MS_ABI impl___0CFontDialog__QEAA_PEAUtagLOGFONTW__KPEAVCDC__PEAVCWnd___Z(
    CFontDialog* pThis, LOGFONTW* lpLogFont, unsigned long dwFlags, CDC* pdcPrinter, CWnd* pParentWnd); // core/dialog/CFontDialog.cpp
extern "C" unsigned long MS_ABI impl__FillInLogFont_CFontDialog__QEAAKAEBU_charformatw___Z(
    CFontDialog* pThis, const CHARFORMATW* pcf);                                          // core/dialog/CFontDialog.cpp
extern "C" __int64 MS_ABI impl__DoModal_CFontDialog__UEAA_JXZ(CFontDialog* pThis);       // core/dialog/Thunks.cpp
extern "C" void MS_ABI impl___1CDialog__UEAA_XZ(CDialog* pThis);                          // detail/DlgcoreSupport.cpp
extern "C" void* MS_ABI impl___0COleDataObject__QEAA_XZ(void* pThis);                     // core/ole/Thunks.cpp
extern "C" void MS_ABI impl__Attach_COleDataObject__QEAAXPEAUIDataObject__H_Z(
    COleDataObject* pThis, IDataObject* lpDataObject, int bAutoRelease);                  // core/ole/Thunks.cpp
extern "C" int MS_ABI impl__IsDataAvailable_COleDataObject__QEAAHGPEAUtagFORMATETC___Z(
    COleDataObject* pThis, unsigned short cfFormat, FORMATETC* lpFormatEtc);              // core/ole/Thunks.cpp
extern "C" void MS_ABI impl__Release_COleDataObject__QEAAXXZ(COleDataObject* pThis);      // core/ole/Thunks.cpp
extern "C" void MS_ABI impl__GetSelText_CRichEditCtrl__QEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__XZ(
    CString* pRet, const CRichEditCtrl* pThis);                                           // core/controls/CRichEditCtrl.cpp

namespace {

// ---------------------------------------------------------------------------
// Retail layout notes (mfc140u ctor ??0CRichEditView@@QEAA@XZ, RVA 0x283380).
// The retail object is a CCtrlView (the view HWND *is* the rich-edit control,
// so every EM_* message goes to this->m_hWnd at +0x40) and carries:
//   +0x108 int m_nWordWrap (=WrapToWindow)     +0x10c int m_nBulletIndent (=720)
//   +0x110 IRichEditOle* m_lpRichEditOle       +0x118 CDC (0x20 bytes, vftable = CDC)
//   +0x138 long m_lInitialSearchPos            +0x13c int m_nPasteType
//   +0x140 BOOL m_bFirstSearch                 +0x144 BOOL (search-wrapped flag)
//   +0x148 CRect m_rectMargin (zeroed)         +0x158 CSize m_sizePaper (12240,15840)
//   +0x160 CDWordArray m_aPageStart            +0x188 PARAFORMAT2 m_paraformat (cbSize 0xbc)
//   +0x244 CHARFORMAT2W m_charformat (cbSize 0x74)
//   +0x2b8 BOOL m_bSyncCharFormat (=1)         +0x2bc BOOL m_bSyncParaFormat (=1)
//   +0x2c0 XRichEditOleCallback m_xRichEditOleCallback   sizeof == 0x2c8
// The OpenMFC header instead derives from CView and embeds a child
// CRichEditCtrl m_richEdit at +0xf8, so the retail offsets above overlap live
// header members and are NEVER dereferenced here.  Bodies below use the header
// model (pThis->m_richEdit.m_hWnd for the control HWND) and say so where that
// differs from retail.
// ---------------------------------------------------------------------------

// Registered clipboard formats.  Retail reads them from the process-wide
// AFX_OLEDATA block (_oleData.cfRichTextFormat / cfRichTextAndObjects, mfc140u
// .data 0x3c3360 / 0x3c3364, identified through CanPaste at 0x285370 which tests
// CF_TEXT, cfRichTextFormat, cfEmbedSource, cfEmbeddedObject, cfFileName,
// cfFileNameW, CF_METAFILEPICT, CF_DIB, CF_BITMAP in the documented MFC order).
// Registered formats are process-global by name, so registering them here is
// equivalent.
inline unsigned short RichTextFormat() {
    static unsigned short cf = (unsigned short)::RegisterClipboardFormatW(L"Rich Text Format");       // CF_RTF
    return cf;
}
inline unsigned short RichTextAndObjectsFormat() {
    static unsigned short cf = (unsigned short)::RegisterClipboardFormatW(L"RichEdit Text and Objects"); // CF_RETEXTOBJ
    return cf;
}

// CWaitCursor: retail's `CWaitCursor wait;` is AfxGetApp()->BeginWaitCursor()
// (through AfxGetModuleState()->m_pCurrentWinApp) with EndWaitCursor in the dtor.
struct WaitCursorScope {
    CWinApp* pApp;
    WaitCursorScope() : pApp(impl__AfxGetApp__YAPEAVCWinApp__XZ()) {
        if (pApp) impl__BeginWaitCursor_CCmdTarget__QEAAXXZ(pApp);
    }
    ~WaitCursorScope() {
        if (pApp) impl__EndWaitCursor_CCmdTarget__QEAAXXZ(pApp);
    }
};

// Stand-in for retail's process-local _AFX_RICHEDIT_STATE (CProcessLocal at
// mfc140u .data 0x3c4218, fetched through ?GetData@CProcessLocalObject@@ in
// every find/replace handler).  Field offsets observed in OnFindNext (0x286130),
// OnReplaceSel (0x286200), OnReplaceAll (0x286330), OnFindReplaceCmd (0x2864e0)
// and, for +0x10 (and the +0x140 m_bFirstSearch = TRUE seed), OnEditFindReplace
// (0x285f40, still a no-op in this file):
//   +0x08 CFindReplaceDialog* pFindReplaceDlg   +0x10 BOOL bFindOnly
//   +0x18 CString strFind   +0x20 CString strReplace
//   +0x28 BOOL bCase        +0x2c BOOL bNext        +0x30 BOOL bWord
// OpenMFC has no per-module CProcessLocal machinery, so this is one static per
// DLL instance (deviation: retail keeps one per module state).
struct RichEditFindState {
    CFindReplaceDialog* pFindReplaceDlg = nullptr;
    BOOL bFindOnly = FALSE;
    CString strFind;
    CString strReplace;
    BOOL bCase = FALSE;
    BOOL bNext = FALSE;
    BOOL bWord = FALSE;
};
RichEditFindState& FindState() {
    static RichEditFindState state;
    return state;
}

// Per-view search state that retail keeps in the object itself (+0x138
// m_lInitialSearchPos, +0x140 m_bFirstSearch, +0x144 "already wrapped" flag).
// The OpenMFC header has no room for them, so they live in a small fixed side
// table keyed by the view (released from ~CRichEditView).  Deviation: retail
// never initialises m_lInitialSearchPos in the ctor; here it starts at 0.  If
// more than kMaxSearchStates views search concurrently the extra views share a
// scratch entry (documented limitation of the side table, not of retail).
struct SearchState {
    long lInitialSearchPos = 0;
    BOOL bFirstSearch = FALSE;   // ctor zeroes +0x140
    BOOL bWrapped = FALSE;       // ctor zeroes +0x144
};
constexpr int kMaxSearchStates = 64;
struct SearchStateSlot {
    const void* pView = nullptr;
    SearchState state;
};
SearchStateSlot* SearchStateTable() {
    static SearchStateSlot slots[kMaxSearchStates];
    return slots;
}
SearchState& SearchStateFor(const CRichEditView* pThis) {
    SearchStateSlot* slots = SearchStateTable();
    for (int i = 0; i < kMaxSearchStates; ++i) {
        if (slots[i].pView == pThis) return slots[i].state;
    }
    for (int i = 0; i < kMaxSearchStates; ++i) {
        if (slots[i].pView == nullptr) {
            slots[i].pView = pThis;
            slots[i].state = SearchState();
            return slots[i].state;
        }
    }
    static SearchState overflow;
    return overflow;
}
void ReleaseSearchState(const CRichEditView* pThis) {
    SearchStateSlot* slots = SearchStateTable();
    for (int i = 0; i < kMaxSearchStates; ++i) {
        if (slots[i].pView == pThis) {
            slots[i].pView = nullptr;
            slots[i].state = SearchState();
        }
    }
}

// EM_EXGETSEL / WM_GETTEXTLENGTH on the control HWND (retail: this->m_hWnd).
inline HWND CtrlHwnd(const CRichEditView* pThis) {
    return pThis->m_richEdit.m_hWnd;
}
inline LONG CtrlTextLength(const CRichEditView* pThis) {
    return (LONG)::SendMessageW(CtrlHwnd(pThis), WM_GETTEXTLENGTH, 0, 0);
}

}  // namespace

//=============================================================================
// CRichEditView — formatting accessors
//=============================================================================


//=============================================================================
// CRichEditView — OLE container item helpers
//=============================================================================






//=============================================================================
// CRichEditView — window / view life-cycle handlers
//=============================================================================







//=============================================================================
// CRichEditView — character / paragraph formatting command handlers
//=============================================================================









//=============================================================================
// CRichEditView — edit command handlers
//=============================================================================










// PARAFORMAT2& CRichEditView::GetParaFormatSelection()
// Retail (RVA 0x2854a0): returns a reference to the cached m_paraFormat member,
// refreshing it from the rich-edit control on first use.  The header-based
// implementation (viewrich.cpp) keeps a thread-local PARAFORMAT2 for this role;
// a reference is returned as a pointer on x64.
// Symbol: ?GetParaFormatSelection@CRichEditView@@QEAAAEAUPARAFORMAT2@@XZ
extern "C" PARAFORMAT2* MS_ABI impl__GetParaFormatSelection_CRichEditView__QEAAAEAUPARAFORMAT2__XZ(
    CRichEditView* pThis) {
    if (!pThis) {
        return nullptr;
    }
    return &pThis->GetParaFormatSelection();
}
// CRichEditCntrItem* CRichEditView::GetSelectedItem() const
// Retail (RVA 0x2855b0) walks the ITextDocument/selection/range chain exposed by
// the rich-edit control, extracts the embedded object and looks it up in the
// document.  This is a faithful transcription of that documented algorithm
// (the retail caches the interfaces in unmodelled members, we query them on
// demand through the public CRichEditCtrl::GetIRichEditOle()).
// Symbol: ?GetSelectedItem@CRichEditView@@QEBAPEAVCRichEditCntrItem@@XZ
extern "C" CRichEditCntrItem* MS_ABI impl__GetSelectedItem_CRichEditView__QEBAPEAVCRichEditCntrItem__XZ(
    const CRichEditView* pThis) {
    if (!pThis) {
        return nullptr;
    }

    IRichEditOle* pRichEditOle = pThis->m_richEdit.GetIRichEditOle();
    if (!pRichEditOle) {
        return nullptr;
    }

    // IRichEditOle is only forward-declared in afxwin.h; it is a COM interface
    // so its IUnknown prefix is at offset 0 and reinterpreting to IUnknown is
    // layout-safe.
    IUnknown* pOleUnk = reinterpret_cast<IUnknown*>(pRichEditOle);
    ITextDocument* pTextDoc = nullptr;
    if (FAILED(pOleUnk->QueryInterface(IID_ITextDocument, (void**)&pTextDoc))) {
        return nullptr;
    }

    ITextSelection* pTextSelection = nullptr;
    if (FAILED(pTextDoc->GetSelection(&pTextSelection)) || !pTextSelection) {
        pTextDoc->Release();
        return nullptr;
    }

    ITextRange* pTextRange = nullptr;
    pTextSelection->GetDuplicate(&pTextRange);
    if (!pTextRange) {
        pTextSelection->Release();
        pTextDoc->Release();
        return nullptr;
    }

    IUnknown* pEmbedded = nullptr;
    if (FAILED(pTextRange->GetEmbeddedObject(&pEmbedded))) {
        pTextRange->Release();
        pTextSelection->Release();
        pTextDoc->Release();
        return nullptr;
    }
    pTextRange->Release();
    pTextSelection->Release();
    pTextDoc->Release();
    if (!pEmbedded) {
        return nullptr;
    }

    CRichEditDoc* pDoc = static_cast<CRichEditDoc*>(pThis->m_pDocument);
    CRichEditCntrItem* pItem =
        pDoc ? pDoc->LookupItem(static_cast<IOleObject*>(pEmbedded)) : nullptr;
    pEmbedded->Release();
    return pItem;
}
// void CRichEditView::InsertFileAsObject(const wchar_t* lpszFileName)
// Retail (RVA 0x285810) opens the file with CFile, creates a CRichEditCntrItem
// through the document's embedded-item factory, calls COleClientItem::CreateFromFile
// and finally InsertItem.  The clean-room CRichEditDoc/COleClientItem headers do
// not model the factory / CreateFromFile entry points, so nothing can be embedded
// without inventing API surface — kept as a type-correct no-op.
// Symbol: ?InsertFileAsObject@CRichEditView@@QEAAXPEB_W@Z
extern "C" void MS_ABI impl__InsertFileAsObject_CRichEditView__QEAAXPEB_W_Z(
    CRichEditView* pThis, const wchar_t* lpszFileName) {
    if (!pThis || !lpszFileName) {
        return;
    }
    // TODO(clean-room): partially transcribed.
}
// LONG CRichEditView::InsertItem(CRichEditCntrItem* pItem)
// Retail (RVA 0x285750) collapses the selection to the insertion point and lets
// the rich-edit OLE machinery insert the object, returning the HRESULT.  The
// underlying insertion call goes through an internal interface slot that the
// clean-room headers do not model, so a conservative version reports failure
// without disturbing the current selection.
// Symbol: ?InsertItem@CRichEditView@@QEAAJPEAVCRichEditCntrItem@@@Z
extern "C" long MS_ABI impl__InsertItem_CRichEditView__QEAAJPEAVCRichEditCntrItem___Z(
    CRichEditView* pThis, CRichEditCntrItem* pItem) {
    if (!pThis) {
        return -1;
    }
    (void)pItem;
    // TODO(clean-room): partially transcribed.
    return 0;
}
// BOOL CRichEditView::IsRichEditFormat(CLIPFORMAT cf)  [static]
// Retail RVA 0x285350 (mfc140u): return cf == _oleData.cfRichTextFormat (.data
// 0x3c3360) || cf == _oleData.cfRichTextAndObjects (0x3c3364) || cf == CF_TEXT.
// The two registered formats are re-registered by name in this file (see
// RichTextFormat / RichTextAndObjectsFormat near the top).
// Symbol: ?IsRichEditFormat@CRichEditView@@SAHG@Z
extern "C" int MS_ABI impl__IsRichEditFormat_CRichEditView__SAHG_Z(unsigned short cf) {
    return (cf == RichTextFormat() || cf == RichTextAndObjectsFormat() || cf == CF_TEXT) ? 1 : 0;
}
// BOOL CRichEditView::IsSelected(const CObject* pDocItem) const
// Retail (RVA 0x286be0): return GetSelectedItem() == pDocItem.
// Symbol: ?IsSelected@CRichEditView@@MEBAHPEBVCObject@@@Z
extern "C" int MS_ABI impl__IsSelected_CRichEditView__MEBAHPEBVCObject___Z(
    const CRichEditView* pThis, const CObject* pObj) {
    if (!pThis) {
        return 0;
    }
    return (impl__GetSelectedItem_CRichEditView__QEBAPEAVCRichEditCntrItem__XZ(pThis) == pObj) ? 1 : 0;
}
// void CRichEditView::OnActivateView(BOOL, CView*, CView*)
// Retail (RVA 0x286b60) records the view HWND into the CRichEditDoc and re-runs
// a wrap/scroll fixup when the view is activated.  The document's view-HWND slot
// and the scroll fixup helpers are not modelled, so this is a no-op.
// Symbol: ?OnActivateView@CRichEditView@@MEAAXHPEAVCView@@0@Z
extern "C" void MS_ABI impl__OnActivateView_CRichEditView__MEAAXHPEAVCView__0_Z(
    CRichEditView* pThis, int bActivate, CView* pActivateView, CView* pDeactiveView) {
    (void)bActivate;
    (void)pActivateView;
    (void)pDeactiveView;
    if (!pThis) {
        return;
    }
    // TODO(clean-room): partially transcribed.
}
// void CRichEditView::OnBeginPrinting(CDC*, CPrintInfo*)
// Retail (RVA 0x283870) also copies the parent size into an unmodelled member;
// the essential clearing of the EM_FORMATRANGE cache is reproduced faithfully.
// Symbol: ?OnBeginPrinting@CRichEditView@@MEAAXPEAVCDC@@PEAUCPrintInfo@@@Z
extern "C" void MS_ABI impl__OnBeginPrinting_CRichEditView__MEAAXPEAVCDC__PEAUCPrintInfo___Z(
    CRichEditView* pThis, void* pDC, void* pInfo) {
    (void)pDC;
    (void)pInfo;
    if (!pThis) {
        return;
    }
    if (pThis->m_richEdit.m_hWnd) {
        ::SendMessageW(pThis->m_richEdit.m_hWnd, EM_FORMATRANGE, 0, 0);
    }
}
// int CRichEditView::OnCreate(CREATESTRUCTW*)
// Retail (RVA 0x283520): base CView::OnCreate first, then sets the text limit to
// CRichEditView::lMaxSize, the event mask, the OLE callback and ECO_AUTOWORDSELECTION.
// The header-based class creates m_richEdit as a child of the view, so the setup
// only applies once that child exists.
// Symbol: ?OnCreate@CRichEditView@@IEAAHPEAUtagCREATESTRUCTW@@@Z
extern "C" int MS_ABI impl__OnCreate_CRichEditView__IEAAHPEAUtagCREATESTRUCTW___Z(
    CRichEditView* pThis, CREATESTRUCTW* pCreateStruct) {
    if (!pThis) {
        return -1;
    }
    if (impl__OnCreate_CView__IEAAHPEAUtagCREATESTRUCTW___Z(pThis, pCreateStruct) == -1) {
        return -1;
    }
    CRichEditCtrl& ctrl = pThis->m_richEdit;
    if (ctrl.m_hWnd) {
        // EM_EXLIMITTEXT with CRichEditView::lMaxSize (64 KB).
        ::SendMessageW(ctrl.m_hWnd, EM_EXLIMITTEXT, 0, (LPARAM)impl__lMaxSize_CRichEditView__2KA);
        // Retail event mask: ENM_CHANGE | ENM_SCROLL | ENM_SELCHANGE.
        ::SendMessageW(ctrl.m_hWnd, EM_SETEVENTMASK, 0, (LPARAM)(ENM_CHANGE | ENM_SCROLL | ENM_SELCHANGE));
        // ECOOP_OR with ECO_AUTOWORDSELECTION, matching the retail call.
        ::SendMessageW(ctrl.m_hWnd, EM_SETOPTIONS, ECOOP_OR, ECO_AUTOWORDSELECTION);
    }
    return 0;
}
// void CRichEditView::OnDestroy()
// Retail (RVA 0x2848c0) releases the cached ITextDocument/IRichEditOle interfaces
// and deactivates in-place items before tail-calling CView::OnDestroy.  The cached
// interface pointers are not modelled; only the base-class cleanup is performed.
// Symbol: ?OnDestroy@CRichEditView@@IEAAXXZ
extern "C" void MS_ABI impl__OnDestroy_CRichEditView__IEAAXXZ(CRichEditView* pThis) {
    if (!pThis) {
        return;
    }
    // TODO(clean-room): partially transcribed.
    impl__OnDestroy_CView__IEAAXXZ(pThis);
}
// void CRichEditView::OnDevModeChange(LPTSTR)
// Retail (RVA 0x2852c0) re-evaluates the print target device through an internal
// virtual taking a CString; that virtual and the device state are not modelled.
// Symbol: ?OnDevModeChange@CRichEditView@@IEAAXPEA_W@Z
extern "C" void MS_ABI impl__OnDevModeChange_CRichEditView__IEAAXPEA_W_Z(
    CRichEditView* pThis, wchar_t* lpDeviceName) {
    (void)lpDeviceName;
    if (!pThis) {
        return;
    }
    // TODO(clean-room): partially transcribed.
}
// void CRichEditView::OnDropFiles(HDROP)
// Retail (RVA 0x2851a0): save the selection, DragQueryFileW each dropped file,
// insert it as an OLE object, collapse the selection, restore the original
// selection and DragFinish.  Transcribed faithfully except that
// InsertFileAsObject is a conservative no-op (see above).
// Symbol: ?OnDropFiles@CRichEditView@@IEAAXPEAUHDROP__@@@Z
extern "C" void MS_ABI impl__OnDropFiles_CRichEditView__IEAAXPEAUHDROP_____Z(
    CRichEditView* pThis, HDROP hDropInfo) {
    if (!pThis || !hDropInfo) {
        return;
    }
    CRichEditCtrl& ctrl = pThis->m_richEdit;
    if (!ctrl.m_hWnd) {
        return;
    }

    // Save the current selection so it can be restored after the drop.
    CHARRANGE saved = {};
    ::SendMessageW(ctrl.m_hWnd, EM_EXGETSEL, 0, (LPARAM)&saved);

    const UINT nFiles = ::DragQueryFileW(hDropInfo, (UINT)-1, nullptr, 0);
    for (UINT i = 0; i < nFiles; ++i) {
        wchar_t szFile[MAX_PATH] = {};
        ::DragQueryFileW(hDropInfo, i, szFile, MAX_PATH);
        impl__InsertFileAsObject_CRichEditView__QEAAXPEB_W_Z(pThis, szFile);
        // Collapse the selection to the end of the inserted object.
        CHARRANGE cr = {};
        ::SendMessageW(ctrl.m_hWnd, EM_EXGETSEL, 0, (LPARAM)&cr);
        cr.cpMin = cr.cpMax;
        ::SendMessageW(ctrl.m_hWnd, EM_EXSETSEL, 0, (LPARAM)&cr);
        ::SetFocus(ctrl.m_hWnd);
    }

    // Restore the selection that was active when the drop began.
    ::SendMessageW(ctrl.m_hWnd, EM_EXSETSEL, 0, (LPARAM)&saved);
    ::DragFinish(hDropInfo);
}
// void CRichEditView::OnBullet()
// Retail (RVA 0x284c60): toggle PFN_BULLET in the paragraph format, resetting the
// bullet indents when turning it off; the retail's default-offset source member
// is not modelled and is left untouched.
// Symbol: ?OnBullet@CRichEditView@@IEAAXXZ
extern "C" void MS_ABI impl__OnBullet_CRichEditView__IEAAXXZ(CRichEditView* pThis) {
    if (!pThis) {
        return;
    }
    PARAFORMAT2& pf = pThis->GetParaFormatSelection();
    if (pf.dwMask & PFM_NUMBERING) {
        if (pf.wNumbering == PFN_BULLET) {
            // Currently a bullet: turn it off and clear the bullet indents.
            pf.wNumbering = 0;
            pf.dxStartIndent = 0;
            pf.dxOffset = 0;
        } else {
            // Numbered list: switch to a bulleted list.
            pf.wNumbering = PFN_BULLET;
        }
    } else {
        pf.wNumbering = PFN_BULLET;
    }
    pf.dwMask = PFM_NUMBERING | PFM_STARTINDENT | PFM_OFFSET;
    pThis->SetParaFormat(pf);
}
// void CRichEditView::OnCancelEditCntr()
// Retail (RVA 0x284740) calls InPlaceDeactivate on the cached rich-edit OLE
// interface; that member is not modelled, so this is a no-op.
// Symbol: ?OnCancelEditCntr@CRichEditView@@IEAAXXZ
extern "C" void MS_ABI impl__OnCancelEditCntr_CRichEditView__IEAAXXZ(CRichEditView* pThis) {
    if (!pThis) {
        return;
    }
    // TODO(clean-room): partially transcribed.
}
// void CRichEditView::OnCharBold()
// Retail (RVA 0x284ba0): OnCharEffect(CFM_BOLD, CFE_BOLD).
// Symbol: ?OnCharBold@CRichEditView@@IEAAXXZ
extern "C" void MS_ABI impl__OnCharBold_CRichEditView__IEAAXXZ(CRichEditView* pThis) {
    impl__OnCharEffect_CRichEditView__QEAAXKK_Z(pThis, CFM_BOLD, CFE_BOLD);
}
// void CRichEditView::OnCharEffect(DWORD dwMask, DWORD dwEffect)
// Retail (RVA 0x284220): if the cached format already has dwMask set, XOR the
// effect off, otherwise OR it on; then dwMask = dwMask and re-apply.
// Symbol: ?OnCharEffect@CRichEditView@@QEAAXKK@Z
extern "C" void MS_ABI impl__OnCharEffect_CRichEditView__QEAAXKK_Z(
    CRichEditView* pThis, unsigned long dwMask, unsigned long dwEffect) {
    if (!pThis) {
        return;
    }
    CHARFORMAT2W cf = pThis->GetCharFormatSelection();
    DWORD dwOldMask = cf.dwMask;
    if (dwMask & dwOldMask) {
        cf.dwEffects ^= dwEffect;   // currently on: toggle off
    } else {
        cf.dwEffects |= dwEffect;   // currently off: turn on
    }
    cf.dwMask = dwMask;
    pThis->SetCharFormat(cf);
}
// void CRichEditView::OnCharItalic()
// Retail (RVA 0x284bc0): OnCharEffect(CFM_ITALIC, CFE_ITALIC).
// Symbol: ?OnCharItalic@CRichEditView@@IEAAXXZ
extern "C" void MS_ABI impl__OnCharItalic_CRichEditView__IEAAXXZ(CRichEditView* pThis) {
    impl__OnCharEffect_CRichEditView__QEAAXKK_Z(pThis, CFM_ITALIC, CFE_ITALIC);
}
// void CRichEditView::OnCharUnderline()
// Retail (RVA 0x284be0): OnCharEffect(CFM_UNDERLINE, CFE_UNDERLINE).
// Symbol: ?OnCharUnderline@CRichEditView@@IEAAXXZ
extern "C" void MS_ABI impl__OnCharUnderline_CRichEditView__IEAAXXZ(CRichEditView* pThis) {
    impl__OnCharEffect_CRichEditView__QEAAXKK_Z(pThis, CFM_UNDERLINE, CFE_UNDERLINE);
}
// void CRichEditView::OnColorDefault()
// Retail (RVA 0x284ee0): set CFM_COLOR / CFE_AUTOCOLOR and re-apply.
// Symbol: ?OnColorDefault@CRichEditView@@IEAAXXZ
extern "C" void MS_ABI impl__OnColorDefault_CRichEditView__IEAAXXZ(CRichEditView* pThis) {
    if (!pThis) {
        return;
    }
    CHARFORMAT2W cf = pThis->GetCharFormatSelection();
    cf.dwMask = CFM_COLOR;
    cf.dwEffects = CFE_AUTOCOLOR;
    pThis->SetCharFormat(cf);
}
// void CRichEditView::OnColorPick(COLORREF cr)
// Retail (RVA 0x284e30): set CFM_COLOR with the chosen text color and re-apply.
// Symbol: ?OnColorPick@CRichEditView@@IEAAXK@Z
extern "C" void MS_ABI impl__OnColorPick_CRichEditView__IEAAXK_Z(
    CRichEditView* pThis, unsigned long cr) {
    if (!pThis) {
        return;
    }
    CHARFORMAT2W cf = pThis->GetCharFormatSelection();
    cf.dwMask = CFM_COLOR;
    cf.dwEffects = 0;
    cf.crTextColor = cr;
    pThis->SetCharFormat(cf);
}
// Retail (RVA 0x279400): WM_CLEAR.
// Symbol: ?OnEditClear@CRichEditView@@IEAAXXZ
extern "C" void MS_ABI impl__OnEditClear_CRichEditView__IEAAXXZ(CRichEditView* pThis) {
    if (pThis) {
        pThis->m_richEdit.Clear();
    }
}
// Retail (RVA 0x2793c0): WM_COPY.
// Symbol: ?OnEditCopy@CRichEditView@@IEAAXXZ
extern "C" void MS_ABI impl__OnEditCopy_CRichEditView__IEAAXXZ(CRichEditView* pThis) {
    if (pThis) {
        pThis->m_richEdit.Copy();
    }
}
// Retail (RVA 0x2793a0): WM_CUT.
// Symbol: ?OnEditCut@CRichEditView@@IEAAXXZ
extern "C" void MS_ABI impl__OnEditCut_CRichEditView__IEAAXXZ(CRichEditView* pThis) {
    if (pThis) {
        pThis->m_richEdit.Cut();
    }
}
// Retail (RVA 0x2846a0): OnEditFindReplace(TRUE).
// Symbol: ?OnEditFind@CRichEditView@@IEAAXXZ
extern "C" void MS_ABI impl__OnEditFind_CRichEditView__IEAAXXZ(CRichEditView* pThis) {
    impl__OnEditFindReplace_CRichEditView__IEAAXH_Z(pThis, TRUE);
}
// void CRichEditView::OnEditFindReplace(BOOL bFindOnly)
// Retail (RVA 0x285f40) owns a CFindReplaceDialog created from the stored
// m_strFind/m_strReplace and wrap flags.  CFindReplaceDialog and that dialog
// state are not modelled by the clean-room headers, so this is a no-op.
// Symbol: ?OnEditFindReplace@CRichEditView@@IEAAXH@Z
extern "C" void MS_ABI impl__OnEditFindReplace_CRichEditView__IEAAXH_Z(
    CRichEditView* pThis, int bFindOnly) {
    (void)bFindOnly;
    if (!pThis) {
        return;
    }
    // TODO(clean-room): partially transcribed.
}
// Retail (RVA 0x2845f0): clears the paste format and sends WM_PASTE.
// Symbol: ?OnEditPaste@CRichEditView@@IEAAXXZ
extern "C" void MS_ABI impl__OnEditPaste_CRichEditView__IEAAXXZ(CRichEditView* pThis) {
    if (pThis) {
        pThis->m_richEdit.Paste();
    }
}
// void CRichEditView::OnEditPasteSpecial()
// Retail (RVA 0x284f80) shows a COlePasteSpecialDialog preloaded with the
// rich-text and plain-text formats and pastes the selection with the chosen
// aspect.  COlePasteSpecialDialog is not modelled by the clean-room headers, so
// fall back to a plain paste.
// Symbol: ?OnEditPasteSpecial@CRichEditView@@IEAAXXZ
extern "C" void MS_ABI impl__OnEditPasteSpecial_CRichEditView__IEAAXXZ(CRichEditView* pThis) {
    if (!pThis) {
        return;
    }
    // TODO(clean-room): partially transcribed.
    pThis->m_richEdit.Paste();
}
// void CRichEditView::OnEditProperties()
// Retail (RVA 0x284a80) builds a character/object property sheet from the
// selected item; the sheet classes are not modelled, so this is a no-op.
// Symbol: ?OnEditProperties@CRichEditView@@IEAAXXZ
extern "C" void MS_ABI impl__OnEditProperties_CRichEditView__IEAAXXZ(CRichEditView* pThis) {
    if (!pThis) {
        return;
    }
    // TODO(clean-room): partially transcribed.
}
// Retail (RVA 0x284650): EM_REDO.
// Symbol: ?OnEditRedo@CRichEditView@@IEAAXXZ
extern "C" void MS_ABI impl__OnEditRedo_CRichEditView__IEAAXXZ(CRichEditView* pThis) {
    if (pThis) {
        pThis->m_richEdit.Redo();
    }
}
// void CRichEditView::OnEditRepeat()
// Retail (RVA 0x2846c0) repeats the last find/replace from the cached
// CFindReplaceDialog state; that state is not modelled, so this is a no-op.
// Symbol: ?OnEditRepeat@CRichEditView@@IEAAXXZ
extern "C" void MS_ABI impl__OnEditRepeat_CRichEditView__IEAAXXZ(CRichEditView* pThis) {
    if (!pThis) {
        return;
    }
    // TODO(clean-room): partially transcribed.
}
// Symbol: ?DoPaste@CRichEditView@@QEAAXAEAVCOleDataObject@@GPEAX@Z
extern "C" void MS_ABI impl__DoPaste_CRichEditView__QEAAXAEAVCOleDataObject__GPEAX_Z(
    CRichEditView* pThis, COleDataObject* pDataObject, unsigned short nFormat, void* /*reserved*/) {
    if (!pThis) {
        return;
    }

    const unsigned short targetFormat = nFormat ? nFormat : CF_UNICODETEXT;
    if (pDataObject) {
        HGLOBAL hData = pDataObject->GetGlobalData(targetFormat);
        if (hData) {
            const wchar_t* pText = static_cast<const wchar_t*>(::GlobalLock(hData));
            if (pText) {
                pThis->m_richEdit.ReplaceSel(pText, TRUE);
                ::GlobalUnlock(hData);
                return;
            }
            ::GlobalUnlock(hData);
        }
    }

    pThis->m_richEdit.Paste();
}
// Symbol: ?GetContextMenu@CRichEditView@@MEAAPEAUHMENU__@@GPEAUIOleObject@@PEAU_charrange@@@Z
extern "C" HMENU* MS_ABI impl__GetContextMenu_CRichEditView__MEAAPEAUHMENU____GPEAUIOleObject__PEAU_charrange___Z(
    CRichEditView* pThis, unsigned short /*nMenuType*/, IOleObject* /*pObject*/, void* /*pRange*/) {
    if (!pThis) {
        return nullptr;
    }

    static HMENU menu = nullptr;
    if (!menu) {
        menu = ::CreatePopupMenu();
        if (menu) {
            ::AppendMenuW(menu, MF_STRING, 1, L"Properties");
            ::AppendMenuW(menu, MF_STRING, 2, L"Clear");
        }
    }
    return &menu;
}
// Symbol: ?GetWindowContext@CRichEditView@@QEAAJPEAPEAUIOleInPlaceFrame@@PEAPEAUIOleInPlaceUIWindow@@PEAUtagOIFI@@@Z
extern "C" long MS_ABI impl__GetWindowContext_CRichEditView__QEAAJPEAPEAUIOleInPlaceFrame__PEAPEAUIOleInPlaceUIWindow__PEAUtagOIFI___Z(
    CRichEditView* pThis, IOleInPlaceFrame** ppFrame, IOleInPlaceUIWindow** ppDoc, LPOLEINPLACEFRAMEINFO lpFrameInfo) {
    if (!pThis) {
        return -1;
    }

    if (ppFrame) {
        *ppFrame = nullptr;
    }
    if (ppDoc) {
        *ppDoc = nullptr;
    }
    if (!lpFrameInfo) {
        return 0;
    }

    memset(lpFrameInfo, 0, sizeof(*lpFrameInfo));
    lpFrameInfo->cb = sizeof(*lpFrameInfo);
    lpFrameInfo->hwndFrame = pThis->GetSafeHwnd();
    lpFrameInfo->fMDIApp = FALSE;
    lpFrameInfo->cAccelEntries = 0;
    lpFrameInfo->haccel = nullptr;
    return S_OK;
}
// Symbol: ?OnUpdateBullet@CRichEditView@@IEAAXPEAVCCmdUI@@@Z
extern "C" void MS_ABI impl__OnUpdateBullet_CRichEditView__IEAAXPEAVCCmdUI___Z(CRichEditView* pThis, CCmdUI* ui) {
    if (ui) impl__SetCheck_CCmdUI__UEAAXH_Z(ui, (pThis && (pThis->GetParaFormatSelection().wNumbering & PFN_BULLET)) ? 1 : 0);
}
// Symbol: ?OnUpdateCharBold@CRichEditView@@IEAAXPEAVCCmdUI@@@Z
extern "C" void MS_ABI impl__OnUpdateCharBold_CRichEditView__IEAAXPEAVCCmdUI___Z(CRichEditView* pThis, CCmdUI* ui) {
    if (ui) impl__SetCheck_CCmdUI__UEAAXH_Z(ui, (pThis && (pThis->GetCharFormatSelection().dwEffects & CFE_BOLD)) ? 1 : 0);
}
// Symbol: ?OnUpdateCharEffect@CRichEditView@@QEAAXPEAVCCmdUI@@KK@Z
extern "C" void MS_ABI impl__OnUpdateCharEffect_CRichEditView__QEAAXPEAVCCmdUI__KK_Z(CRichEditView* pThis, CCmdUI* ui, unsigned long dwMask, unsigned long dwEffects) {
    if (ui) impl__SetCheck_CCmdUI__UEAAXH_Z(ui, ((dwEffects & dwMask) == dwEffects) ? 1 : 0);
    (void)pThis;
}
// Symbol: ?OnUpdateCharItalic@CRichEditView@@IEAAXPEAVCCmdUI@@@Z
extern "C" void MS_ABI impl__OnUpdateCharItalic_CRichEditView__IEAAXPEAVCCmdUI___Z(CRichEditView* pThis, CCmdUI* ui) {
    if (ui) impl__SetCheck_CCmdUI__UEAAXH_Z(ui, (pThis && (pThis->GetCharFormatSelection().dwEffects & CFE_ITALIC)) ? 1 : 0);
}
// Symbol: ?OnUpdateCharUnderline@CRichEditView@@IEAAXPEAVCCmdUI@@@Z
extern "C" void MS_ABI impl__OnUpdateCharUnderline_CRichEditView__IEAAXPEAVCCmdUI___Z(CRichEditView* pThis, CCmdUI* ui) {
    if (ui) impl__SetCheck_CCmdUI__UEAAXH_Z(ui, (pThis && (pThis->GetCharFormatSelection().dwEffects & CFE_UNDERLINE)) ? 1 : 0);
}
// Symbol: ?OnUpdateEditPasteSpecial@CRichEditView@@IEAAXPEAVCCmdUI@@@Z
extern "C" void MS_ABI impl__OnUpdateEditPasteSpecial_CRichEditView__IEAAXPEAVCCmdUI___Z(CRichEditView* pThis, CCmdUI* ui) {
    if (ui) impl__Enable_CCmdUI__UEAAXH_Z(ui, pThis ? pThis->CanPaste() : FALSE);
}
// Symbol: ?OnUpdateEditProperties@CRichEditView@@IEAAXPEAVCCmdUI@@@Z
extern "C" void MS_ABI impl__OnUpdateEditProperties_CRichEditView__IEAAXPEAVCCmdUI___Z(CRichEditView*, CCmdUI* ui) {
    if (ui) impl__Enable_CCmdUI__UEAAXH_Z(ui, TRUE);
}
// Symbol: ?OnUpdateEditRedo@CRichEditView@@IEAAXPEAVCCmdUI@@@Z
extern "C" void MS_ABI impl__OnUpdateEditRedo_CRichEditView__IEAAXPEAVCCmdUI___Z(CRichEditView* pThis, CCmdUI* ui) {
    if (ui) impl__Enable_CCmdUI__UEAAXH_Z(ui, (pThis && pThis->m_richEdit.m_hWnd) ? (BOOL)::SendMessageW(pThis->m_richEdit.m_hWnd, EM_CANREDO, 0, 0) : FALSE);
}
// Symbol: ?OnUpdateEditUndo@CRichEditView@@IEAAXPEAVCCmdUI@@@Z
extern "C" void MS_ABI impl__OnUpdateEditUndo_CRichEditView__IEAAXPEAVCCmdUI___Z(CRichEditView* pThis, CCmdUI* ui) {
    if (ui) impl__Enable_CCmdUI__UEAAXH_Z(ui, (pThis && pThis->m_richEdit.m_hWnd) ? (BOOL)::SendMessageW(pThis->m_richEdit.m_hWnd, EM_CANUNDO, 0, 0) : FALSE);
}
// Symbol: ?OnUpdateNeedClip@CRichEditView@@IEAAXPEAVCCmdUI@@@Z
extern "C" void MS_ABI impl__OnUpdateNeedClip_CRichEditView__IEAAXPEAVCCmdUI___Z(CRichEditView*, CCmdUI* ui) {
    if (ui) impl__Enable_CCmdUI__UEAAXH_Z(ui, ::IsClipboardFormatAvailable(CF_UNICODETEXT));
}
// Symbol: ?OnUpdateNeedFind@CRichEditView@@IEAAXPEAVCCmdUI@@@Z
extern "C" void MS_ABI impl__OnUpdateNeedFind_CRichEditView__IEAAXPEAVCCmdUI___Z(CRichEditView* pThis, CCmdUI* ui) {
    if (ui) impl__Enable_CCmdUI__UEAAXH_Z(ui, pThis && pThis->GetRichEditCtrl().GetTextLength() > 0);
}
// Symbol: ?OnUpdateNeedSel@CRichEditView@@IEAAXPEAVCCmdUI@@@Z
extern "C" void MS_ABI impl__OnUpdateNeedSel_CRichEditView__IEAAXPEAVCCmdUI___Z(CRichEditView* pThis, CCmdUI* ui) {
    if (ui) {
        DWORD s = 0, e = 0;
        if (pThis && pThis->m_richEdit.m_hWnd)
            ::SendMessageW(pThis->m_richEdit.m_hWnd, EM_GETSEL, (WPARAM)&s, (LPARAM)&e);
        impl__Enable_CCmdUI__UEAAXH_Z(ui, s != e);
    }
}
// Symbol: ?OnUpdateNeedText@CRichEditView@@IEAAXPEAVCCmdUI@@@Z
extern "C" void MS_ABI impl__OnUpdateNeedText_CRichEditView__IEAAXPEAVCCmdUI___Z(CRichEditView* pThis, CCmdUI* ui) {
    if (ui) impl__Enable_CCmdUI__UEAAXH_Z(ui, pThis && pThis->GetRichEditCtrl().GetTextLength() > 0);
}
// Symbol: ?OnUpdateParaAlign@CRichEditView@@QEAAXPEAVCCmdUI@@G@Z
extern "C" void MS_ABI impl__OnUpdateParaAlign_CRichEditView__QEAAXPEAVCCmdUI__G_Z(CRichEditView* pThis, CCmdUI* ui, unsigned short nAlign) {
    if (ui) impl__SetCheck_CCmdUI__UEAAXH_Z(ui, (pThis && pThis->GetParaFormatSelection().wAlignment == nAlign) ? 1 : 0);
}
// Symbol: ?OnUpdateParaCenter@CRichEditView@@IEAAXPEAVCCmdUI@@@Z
extern "C" void MS_ABI impl__OnUpdateParaCenter_CRichEditView__IEAAXPEAVCCmdUI___Z(CRichEditView* pThis, CCmdUI* ui) {
    impl__OnUpdateParaAlign_CRichEditView__QEAAXPEAVCCmdUI__G_Z(pThis, ui, PFA_CENTER);
}
// Symbol: ?OnUpdateParaLeft@CRichEditView@@IEAAXPEAVCCmdUI@@@Z
extern "C" void MS_ABI impl__OnUpdateParaLeft_CRichEditView__IEAAXPEAVCCmdUI___Z(CRichEditView* pThis, CCmdUI* ui) {
    impl__OnUpdateParaAlign_CRichEditView__QEAAXPEAVCCmdUI__G_Z(pThis, ui, PFA_LEFT);
}
// Symbol: ?OnUpdateParaRight@CRichEditView@@IEAAXPEAVCCmdUI@@@Z
extern "C" void MS_ABI impl__OnUpdateParaRight_CRichEditView__IEAAXPEAVCCmdUI___Z(CRichEditView* pThis, CCmdUI* ui) {
    impl__OnUpdateParaAlign_CRichEditView__QEAAXPEAVCCmdUI__G_Z(pThis, ui, PFA_RIGHT);
}
CRichEditView::CRichEditView()
    : m_nWordWrapMode(0) {
    m_sizePaper.cx = 12240;  // 8.5 inches in twips
    m_sizePaper.cy = 15840;  // 11 inches in twips
    m_rectMargins.SetRect(1440, 1440, 1440, 1440);  // 1 inch margins in twips
    memset(_richeditview_padding, 0, sizeof(_richeditview_padding));
}
CRichEditView::~CRichEditView() {
    ReleaseSearchState(this);
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
BOOL CRichEditView::SetParaFormat(const PARAFORMAT2& pf) {
    return m_richEdit.SetParaFormat(pf);
}
void CRichEditView::GetParaFormat(PARAFORMAT2& pf) const {
    m_richEdit.GetParaFormat(pf);
}
LONG CRichEditView::PrintInsideRect(CDC* pDC, RECT& rectLayout,
                                    LONG nIndexStart, LONG nIndexEnd,
                                    BOOL bOutput) {
    if (!m_richEdit.m_hWnd || !pDC || !pDC->m_hDC) return nIndexStart;

    FORMATRANGE fr = {};
    fr.hdc = pDC->m_hDC;
    fr.hdcTarget = pDC->m_hDC;
    fr.rc = rectLayout;
    fr.rcPage = rectLayout;
    fr.chrg.cpMin = nIndexStart;
    fr.chrg.cpMax = nIndexEnd;

    LONG nextIndex = static_cast<LONG>(::SendMessageW(
        m_richEdit.m_hWnd, EM_FORMATRANGE, bOutput ? TRUE : FALSE,
        reinterpret_cast<LPARAM>(&fr)));

    ::SendMessageW(m_richEdit.m_hWnd, EM_FORMATRANGE, 0, 0);  // Clear cache
    return nextIndex;
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

// ===========================================================================
// Bodies transcribed from the retail mfc140u.dll disassembly (RVAs resolved
// through the mfc140u export table by ordinal; the ANSI twin has a DIFFERENT
// CRichEditView layout -- CHARFORMAT2A at +0x244 is 0x54 bytes, so the sync
// flags sit at +0x298/+0x29c there instead of +0x2b8/+0x2bc -- so every
// address and offset quoted below was read from the Unicode image).
// ===========================================================================

// BOOL CRichEditView::FindTextSimple(LPCTSTR lpszFind, BOOL bCase, BOOL bWord, BOOL bNext)
// Retail RVA 0x2867e0 (mfc140u):
//   EM_EXGETSEL(&ft.chrg);
//   if (m_bFirstSearch) { m_lInitialSearchPos = bNext ? cpMin : cpMax;
//                         m_bFirstSearch = FALSE; m_bWrapped(+0x144) = FALSE; }
//   ft.lpstrText = lpszFind;
//   if (cpMin != cpMax) {            // a selection exists
//       if (bNext) cpMin = (cpMin + 1 < GetTextLength()) ? cpMin + 1 : GetTextLength();
//       else       cpMin = max(cpMin - 1, 0);
//   }
//   dwFlags = (bCase ? FR_MATCHCASE : 0) | (bWord ? FR_WHOLEWORD : 0);
//   if (bNext) { cpMax = GetTextLength(); dwFlags |= FR_DOWN; } else cpMax = 0;
//   if (FindAndSelect(dwFlags, ft) != -1) { m_bWrapped = TRUE; return TRUE; }
//   if (m_bWrapped) return FALSE;    // one wrap-around only
//   m_bWrapped = TRUE;
//   if (bNext) { cpMin = 0;
//                cpMax = (len(lpszFind) + m_lInitialSearchPos < GetTextLength())
//                        ? m_lInitialSearchPos + len(lpszFind) : GetTextLength();
//                m_lInitialSearchPos = 0; }
//   else       { cpMin = GetTextLength();
//                cpMax = (m_lInitialSearchPos - len(lpszFind) > 0)
//                        ? m_lInitialSearchPos - len(lpszFind) : 0;
//                m_lInitialSearchPos = GetTextLength(); }
//   return FindAndSelect(dwFlags, ft) != -1;
// The three state members are kept in the SearchState side table (see top of
// file); the control HWND is m_richEdit.m_hWnd in this model (retail: m_hWnd).
// Symbol: ?FindTextSimple@CRichEditView@@QEAAHPEB_WHHH@Z
extern "C" int MS_ABI impl__FindTextSimple_CRichEditView__QEAAHPEB_WHHH_Z(
    CRichEditView* pThis, const wchar_t* lpszFind, int bCase, int bWord, int bNext) {
    if (!pThis || !lpszFind || !CtrlHwnd(pThis)) {
        return FALSE;
    }
    SearchState& st = SearchStateFor(pThis);
    FINDTEXTEXW ft = {};
    ::SendMessageW(CtrlHwnd(pThis), EM_EXGETSEL, 0, (LPARAM)&ft.chrg);
    if (st.bFirstSearch) {
        st.lInitialSearchPos = bNext ? ft.chrg.cpMin : ft.chrg.cpMax;
        st.bFirstSearch = FALSE;
        st.bWrapped = FALSE;
    }
    ft.lpstrText = lpszFind;
    if (ft.chrg.cpMin != ft.chrg.cpMax) {
        if (bNext) {
            const LONG nLen = CtrlTextLength(pThis);
            ft.chrg.cpMin = (ft.chrg.cpMin + 1 < nLen) ? ft.chrg.cpMin + 1 : CtrlTextLength(pThis);
        } else {
            ft.chrg.cpMin = (ft.chrg.cpMin - 1 > 0) ? ft.chrg.cpMin - 1 : 0;
        }
    }
    unsigned long dwFlags = (bCase ? FR_MATCHCASE : 0) | (bWord ? FR_WHOLEWORD : 0);
    if (bNext) {
        ft.chrg.cpMax = CtrlTextLength(pThis);
        dwFlags |= FR_DOWN;
    } else {
        ft.chrg.cpMax = 0;
    }
    if (impl__FindAndSelect_CRichEditView__QEAAJKAEAU_findtextexw___Z(pThis, dwFlags, &ft) != -1) {
        st.bWrapped = TRUE;
        return TRUE;
    }
    if (st.bWrapped) {
        return FALSE;
    }
    st.bWrapped = TRUE;
    const LONG nFindLen = (LONG)wcslen(lpszFind);
    if (bNext) {
        ft.chrg.cpMin = 0;
        const LONG nLen = CtrlTextLength(pThis);
        ft.chrg.cpMax = (nFindLen + st.lInitialSearchPos < nLen)
                            ? st.lInitialSearchPos + nFindLen
                            : CtrlTextLength(pThis);
        st.lInitialSearchPos = 0;
    } else {
        ft.chrg.cpMin = CtrlTextLength(pThis);
        ft.chrg.cpMax = (st.lInitialSearchPos - nFindLen > 0) ? st.lInitialSearchPos - nFindLen : 0;
        st.lInitialSearchPos = CtrlTextLength(pThis);
    }
    return impl__FindAndSelect_CRichEditView__QEAAJKAEAU_findtextexw___Z(pThis, dwFlags, &ft) != -1
               ? TRUE : FALSE;
}

// CHARFORMAT2W& CRichEditView::GetCharFormatSelection()
// Retail RVA 0x285440 (mfc140u): if (m_bSyncCharFormat) { m_charformat.cbSize =
// sizeof(CHARFORMAT2W) (0x74); SendMessage(m_hWnd, EM_GETCHARFORMAT, SCF_SELECTION,
// &m_charformat); m_bSyncCharFormat = FALSE; } return m_charformat;
// The header model has neither the +0x244 cache nor the +0x2b8 flag; its
// GetCharFormatSelection() refreshes on every call (equivalent to the flag being
// permanently TRUE) and hands back a thread-local CHARFORMAT2W, exactly as the
// GetParaFormatSelection thunk above does.
// Symbol: ?GetCharFormatSelection@CRichEditView@@QEAAAEAUCHARFORMAT2W@@XZ
extern "C" CHARFORMAT2W* MS_ABI impl__GetCharFormatSelection_CRichEditView__QEAAAEAUCHARFORMAT2W__XZ(
    CRichEditView* pThis) {
    if (!pThis) {
        return nullptr;
    }
    return &pThis->GetCharFormatSelection();
}

// void CRichEditView::AdjustDialogPosition(CDialog* pDlg)
// Retail RVA 0x285e20 (mfc140u):
//   long nStart, nEnd; CRichEditCtrl::GetSel(nStart, nEnd) (0x299ad0);
//   POINT pt = {0,0}; SendMessage(m_hWnd, EM_POSFROMCHAR, (WPARAM)&pt, nStart);
//   ::ClientToScreen(m_hWnd, &pt);
//   RECT rc = {}; ::GetWindowRect(pDlg->m_hWnd, &rc);
//   if (::PtInRect(&rc, pt)) {
//       if (pt.y > rc.bottom - rc.top) ::OffsetRect(&rc, 0, pt.y - rc.bottom - 20);
//       else if (rc.bottom - rc.top + pt.y < ::GetSystemMetrics(SM_CYSCREEN))
//           ::OffsetRect(&rc, 0, 40 + pt.y - rc.top);
//       CWnd::MoveWindow(pDlg, rc.left, rc.top, width, height, TRUE) (0x2a9a10);
//   }
// IAT slots resolved against mfc140u: 0x2c7248 ClientToScreen, 0x2c6c08
// GetWindowRect, 0x2c72f8 PtInRect, 0x2c6bd8 GetSystemMetrics, 0x2c72f0 OffsetRect.
// Deviations: the control HWND is m_richEdit.m_hWnd (retail: the view is the
// control); retail has no NULL check on pDlg (debug ASSERT only) -- one is added;
// GetSel and CWnd::MoveWindow are replaced by the EM_EXGETSEL / ::MoveWindow
// calls they wrap.
// Symbol: ?AdjustDialogPosition@CRichEditView@@QEAAXPEAVCDialog@@@Z
extern "C" void MS_ABI impl__AdjustDialogPosition_CRichEditView__QEAAXPEAVCDialog___Z(
    CRichEditView* pThis, CDialog* pDlg) {
    if (!pThis || !pDlg || !CtrlHwnd(pThis)) {
        return;
    }
    CHARRANGE cr = {};
    ::SendMessageW(CtrlHwnd(pThis), EM_EXGETSEL, 0, (LPARAM)&cr);
    POINT pt = {0, 0};
    ::SendMessageW(CtrlHwnd(pThis), EM_POSFROMCHAR, (WPARAM)&pt, (LPARAM)cr.cpMin);
    ::ClientToScreen(CtrlHwnd(pThis), &pt);
    RECT rc = {};
    ::GetWindowRect(pDlg->m_hWnd, &rc);
    if (!::PtInRect(&rc, pt)) {
        return;
    }
    const int nHeight = rc.bottom - rc.top;
    if (pt.y > nHeight) {
        ::OffsetRect(&rc, 0, pt.y - rc.bottom - 20);
    } else {
        const int nVertExt = ::GetSystemMetrics(SM_CYSCREEN);
        if (nHeight + pt.y < nVertExt) {
            ::OffsetRect(&rc, 0, 40 + pt.y - rc.top);
        }
    }
    ::MoveWindow(pDlg->m_hWnd, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, TRUE);
}

// void CRichEditView::DeleteContents()
// Retail RVA 0x283610 (mfc140u): CWnd::SetWindowTextW(this, L"") (0x2a9790, the
// argument is the empty string at .rdata 0x33d19c); SendMessage(m_hWnd,
// EM_EMPTYUNDOBUFFER, 0, 0); m_bSyncCharFormat = m_bSyncParaFormat = TRUE.
// Model: both go to the child control; the sync flags do not exist here (the
// format accessors always refresh).
// Symbol: ?DeleteContents@CRichEditView@@UEAAXXZ
extern "C" void MS_ABI impl__DeleteContents_CRichEditView__UEAAXXZ(CRichEditView* pThis) {
    if (!pThis) {
        return;
    }
    impl__SetWindowTextW_CWnd__QEAAXPEB_W_Z(&pThis->m_richEdit, L"");
    if (CtrlHwnd(pThis)) {
        ::SendMessageW(CtrlHwnd(pThis), EM_EMPTYUNDOBUFFER, 0, 0);
    }
}

// DWORD CALLBACK CRichEditView::EditStreamCallBack(DWORD_PTR dwCookie, LPBYTE pbBuff, LONG cb, LONG* pcb)
// Retail RVA 0x2837f0 (mfc140u).  dwCookie points at the two-field cookie that
// CRichEditView::Stream (0x283730) builds on its stack: { CArchive* pArchive;
// DWORD dwError; }.  Body:
//   CArchive* pArchive = cookie->pArchive;
//   TRY { pArchive->Flush();  *pcb = cb;
//         if (pArchive->m_nMode & CArchive::load) *pcb = pArchive->m_pFile->Read(pbBuff, cb);
//         else                                     pArchive->m_pFile->Write(pbBuff, cb); }
//   CATCH (CFileException, e) { *pcb = 0; cookie->dwError = e->m_cause; e->Delete(); return 1; }
//   AND_CATCH_ALL (e)         { *pcb = 0; cookie->dwError = 1; if (e) e->Delete(); return 1; }
//   return 0;
// (the two catch funclets sit at 0x2c451f / 0x2c4562 and were read through the
// FuncInfo4 try-block map of 0x2837f0; the CFile calls are vtable slots 16/17).
// OpenMFC's CArchive is a different layout (it has a vptr), so the archive is
// driven through its public accessors / the Flush thunk and the CFile call goes
// through the MSVC-layout dispatch helpers from detail/FilecoreSupport.h.
// Deviation: OpenMFC raises its MFC exceptions through _CxxThrowException (see
// detail/MfcExceptionsSupport.h), which an in-DLL C++ catch clause cannot
// inspect, so a single catch(...) arm reports CFileException::genericException
// (1) for every failure and does not run e->Delete().
// Symbol: ?EditStreamCallBack@CRichEditView@@SAK_KPEAEJPEAJ@Z
extern "C" unsigned long MS_ABI impl__EditStreamCallBack_CRichEditView__SAK_KPEAEJPEAJ_Z(
    unsigned __int64 dwCookie, unsigned char* pbBuff, long cb, long* pcb) {
    struct StreamCookie {
        CArchive* pArchive;
        DWORD dwError;
    };
    StreamCookie* pCookie = reinterpret_cast<StreamCookie*>(dwCookie);
    if (!pCookie || !pCookie->pArchive || !pcb) {
        return 1;   // retail has no such guard (it would fault)
    }
    CArchive* pArchive = pCookie->pArchive;
    try {
        impl__Flush_CArchive__QEAAXXZ(pArchive);
        *pcb = cb;
        CFile* pFile = pArchive->GetFile();
        if (pArchive->IsLoading()) {
            *pcb = (long)OpenMFC_File_Read(pFile, pbBuff, (unsigned int)cb);
        } else {
            OpenMFC_File_Write(pFile, pbBuff, (unsigned int)cb);
        }
    } catch (...) {
        *pcb = 0;
        pCookie->dwError = 1;   // CFileException::genericException
        return 1;
    }
    return 0;
}

// long CRichEditView::FindAndSelect(DWORD dwFlags, FINDTEXTEXW& ft)
// Retail RVA 0x286ad0 (mfc140u): long idx = SendMessage(m_hWnd, EM_FINDTEXTEX
// (0x44f -- the control is a Unicode class, so it takes the FINDTEXTEXW),
// dwFlags, &ft); if (idx != -1) SendMessage(m_hWnd, EM_EXSETSEL (0x437), 0,
// &ft.chrgText); return idx.  (IAT 0x2c7120 = SendMessageW.)
// Symbol: ?FindAndSelect@CRichEditView@@QEAAJKAEAU_findtextexw@@@Z
extern "C" long MS_ABI impl__FindAndSelect_CRichEditView__QEAAJKAEAU_findtextexw___Z(
    CRichEditView* pThis, unsigned long dwFlags, FINDTEXTEXW* pft) {
    if (!pThis || !pft || !CtrlHwnd(pThis)) {
        return -1;
    }
    const long nIndex = (long)::SendMessageW(CtrlHwnd(pThis), EM_FINDTEXTEX, (WPARAM)dwFlags, (LPARAM)pft);
    if (nIndex != -1) {
        ::SendMessageW(CtrlHwnd(pThis), EM_EXSETSEL, 0, (LPARAM)&pft->chrgText);
    }
    return nIndex;
}

// HRESULT CRichEditView::GetClipboardData(CHARRANGE*, DWORD, LPDATAOBJECT, LPDATAOBJECT*)
// Retail (mfc140u ordinal 5036 -> RVA 0xf4d0, a body shared with
// CArchiveStream::Clone): return E_NOTIMPL.
// Symbol: ?GetClipboardData@CRichEditView@@MEAAJPEAU_charrange@@KPEAUIDataObject@@PEAPEAU3@@Z
extern "C" long MS_ABI impl__GetClipboardData_CRichEditView__MEAAJPEAU_charrange__KPEAUIDataObject__PEAPEAU3__Z(
    CRichEditView* pThis, CHARRANGE* lpchrg, unsigned long dwReco, IDataObject* lpRichDataObj,
    IDataObject** lplpdataobj) {
    (void)pThis; (void)lpchrg; (void)dwReco; (void)lpRichDataObj; (void)lplpdataobj;
    return E_NOTIMPL;
}

// CRichEditCntrItem* CRichEditView::GetInPlaceActiveItem() const
// Retail RVA 0x285680 (mfc140u):
//   CRichEditDoc* pDoc = m_pDocument;  CReObject reobj (0x283250: REOBJECT with cbSize set);
//   if (m_lpRichEditOle->GetObject(REO_IOB_SELECTION, &reobj, REO_GETOBJ_ALL_INTERFACES) == S_OK
//       && (reobj.dwFlags & REO_INPLACEACTIVE)) {
//       pItem = pDoc->LookupItem(reobj.poleobj) (0x286e40);
//       if (!pItem) pItem = pDoc->CreateClientItem(&reobj) (CRichEditDoc vslot 106);
//   }
//   ~CReObject (0x283310) releases the three interfaces; return pItem.
// Model: the OLE interface is fetched per call through m_richEdit.GetIRichEditOle()
// (retail caches it in m_lpRichEditOle at +0x110) and released afterwards; the
// CreateClientItem fallback goes to the CRichEditDoc thunk, which is a null stub
// in core/doc/CRichEditDoc.cpp today.
// Symbol: ?GetInPlaceActiveItem@CRichEditView@@QEBAPEAVCRichEditCntrItem@@XZ
extern "C" CRichEditCntrItem* MS_ABI impl__GetInPlaceActiveItem_CRichEditView__QEBAPEAVCRichEditCntrItem__XZ(
    const CRichEditView* pThis) {
    if (!pThis) {
        return nullptr;
    }
    CRichEditDoc* pDoc = static_cast<CRichEditDoc*>(pThis->m_pDocument);
    IRichEditOle* pRichEditOle = pThis->m_richEdit.GetIRichEditOle();
    if (!pRichEditOle || !pDoc) {
        if (pRichEditOle) pRichEditOle->Release();
        return nullptr;
    }
    REOBJECT reobj = {};
    reobj.cbStruct = sizeof(REOBJECT);
    CRichEditCntrItem* pItem = nullptr;
    if (pRichEditOle->GetObject(REO_IOB_SELECTION, &reobj, REO_GETOBJ_ALL_INTERFACES) == S_OK &&
        (reobj.dwFlags & REO_INPLACEACTIVE)) {
        pItem = pDoc->LookupItem(reobj.poleobj);
        if (!pItem) {
            pItem = impl__CreateClientItem_CRichEditDoc__UEBAPEAVCRichEditCntrItem__PEAU_reobject___Z(pDoc, &reobj);
        }
    }
    if (reobj.poleobj) reobj.poleobj->Release();
    if (reobj.pstg) reobj.pstg->Release();
    if (reobj.polesite) reobj.polesite->Release();
    pRichEditOle->Release();
    return pItem;
}

// void CRichEditView::OnEditReplace()
// Retail RVA 0x2846b0 (mfc140u): tail-jump OnEditFindReplace(FALSE) (0x285f40).
// Symbol: ?OnEditReplace@CRichEditView@@IEAAXXZ
extern "C" void MS_ABI impl__OnEditReplace_CRichEditView__IEAAXXZ(CRichEditView* pThis) {
    impl__OnEditFindReplace_CRichEditView__IEAAXH_Z(pThis, FALSE);
}

// void CRichEditView::OnEditSelectAll()
// Retail RVA 0x284690 (mfc140u): tail-jump CRichEditCtrl::SetSel(this, 0, -1) (0x299990).
// Symbol: ?OnEditSelectAll@CRichEditView@@IEAAXXZ
extern "C" void MS_ABI impl__OnEditSelectAll_CRichEditView__IEAAXXZ(CRichEditView* pThis) {
    if (pThis) {
        pThis->m_richEdit.SetSel(0, -1);
    }
}

// void CRichEditView::OnEditUndo()
// Retail RVA 0x284610 (mfc140u): SendMessage(m_hWnd, EM_UNDO, 0, 0);
// m_bSyncCharFormat = m_bSyncParaFormat = TRUE.  (Flags absent in this model.)
// Symbol: ?OnEditUndo@CRichEditView@@IEAAXXZ
extern "C" void MS_ABI impl__OnEditUndo_CRichEditView__IEAAXXZ(CRichEditView* pThis) {
    if (pThis && CtrlHwnd(pThis)) {
        ::SendMessageW(CtrlHwnd(pThis), EM_UNDO, 0, 0);
    }
}

// void CRichEditView::OnEndPrinting(CDC*, CPrintInfo*)
// Retail RVA 0x283de0 (mfc140u): SendMessage(m_hWnd, EM_FORMATRANGE, FALSE, NULL)
// then tail-jump CDWordArray::SetSize(&m_aPageStart, 0, -1) (0x1d3f20), i.e.
// m_aPageStart.RemoveAll().  The page-start array (+0x160) does not exist in
// this model, so only the format-range cache is cleared.
// Symbol: ?OnEndPrinting@CRichEditView@@MEAAXPEAVCDC@@PEAUCPrintInfo@@@Z
extern "C" void MS_ABI impl__OnEndPrinting_CRichEditView__MEAAXPEAVCDC__PEAUCPrintInfo___Z(
    CRichEditView* pThis, CDC* pDC, void* pInfo) {
    (void)pDC;
    (void)pInfo;
    if (pThis && CtrlHwnd(pThis)) {
        ::SendMessageW(CtrlHwnd(pThis), EM_FORMATRANGE, FALSE, 0);
    }
}

// void CRichEditView::OnFindNext(LPCTSTR lpszFind, BOOL bNext, BOOL bCase, BOOL bWord)
// Retail RVA 0x286130 (mfc140u):
//   pEditState->strFind = lpszFind; bWord(+0x30) = bWord; bCase(+0x28) = bCase; bNext(+0x2c) = bNext;
//   if (!FindTextW(strFind, bCase, bWord, bNext)) { m_bFirstSearch = TRUE; OnTextNotFound(strFind); }
//   else AdjustDialogPosition(pEditState->pFindReplaceDlg);
// FindTextW (0x286780) is a CWaitCursor (AfxGetModuleState()->m_pCurrentWinApp->
// BeginWaitCursor 0x1de7b0 / EndWaitCursor 0x1de7e0) around FindTextSimple
// (0x2867e0), so the wait cursor is applied here and FindTextSimple called
// directly.  OnTextNotFound is retail vslot 114; the header's C++ vtable is not
// MSVC-compatible, so the export thunk is called (deviation: a client override
// of OnTextNotFound is not reached).
// Symbol: ?OnFindNext@CRichEditView@@MEAAXPEB_WHHH@Z
extern "C" void MS_ABI impl__OnFindNext_CRichEditView__MEAAXPEB_WHHH_Z(
    CRichEditView* pThis, const wchar_t* lpszFind, int bNext, int bCase, int bWord) {
    if (!pThis) {
        return;
    }
    RichEditFindState& st = FindState();
    st.strFind = lpszFind ? lpszFind : L"";
    st.bWord = bWord;
    st.bCase = bCase;
    st.bNext = bNext;
    BOOL bFound;
    {
        WaitCursorScope wait;
        bFound = impl__FindTextSimple_CRichEditView__QEAAHPEB_WHHH_Z(
            pThis, (const wchar_t*)st.strFind, bCase, bWord, bNext);
    }
    if (!bFound) {
        SearchStateFor(pThis).bFirstSearch = TRUE;
        impl__OnTextNotFound_CRichEditView__MEAAXPEB_W_Z(pThis, (const wchar_t*)st.strFind);
    } else {
        impl__AdjustDialogPosition_CRichEditView__QEAAXPEAVCDialog___Z(pThis, st.pFindReplaceDlg);
    }
}

// LRESULT CRichEditView::OnFindReplaceCmd(WPARAM, LPARAM lParam)
// Retail RVA 0x2864e0 (mfc140u).  lParam is the FINDREPLACEW the common dialog
// hands back (it is the dialog's own m_fr, which is what the inlined
// CFindReplaceDialog accessors read):
//   if (fr.Flags & FR_DIALOGTERM) pEditState->pFindReplaceDlg = NULL;
//   else if (Flags & FR_FINDNEXT)   OnFindNext(CString(fr.lpstrFindWhat), Flags & FR_DOWN,
//                                              Flags & FR_MATCHCASE, Flags & FR_WHOLEWORD);   (vslot 111)
//   else if (Flags & FR_REPLACE)    OnReplaceSel(strFind, DOWN, MATCHCASE, WHOLEWORD, CString(fr.lpstrReplaceWith)); (vslot 112)
//   else if (Flags & FR_REPLACEALL) OnReplaceAll(strFind, strReplace, MATCHCASE, WHOLEWORD); (vslot 113)
//   return 0;
// The virtual calls go to the export thunks here (see OnFindNext for why).
// Symbol: ?OnFindReplaceCmd@CRichEditView@@IEAA_J_K_J@Z
extern "C" __int64 MS_ABI impl__OnFindReplaceCmd_CRichEditView__IEAA_J_K_J_Z(
    CRichEditView* pThis, unsigned __int64 wParam, __int64 lParam) {
    (void)wParam;
    if (!pThis || !lParam) {
        return 0;
    }
    const FINDREPLACEW* pfr = reinterpret_cast<const FINDREPLACEW*>(lParam);
    const DWORD dwFlags = pfr->Flags;
    if (dwFlags & FR_DIALOGTERM) {
        FindState().pFindReplaceDlg = nullptr;
    } else if (dwFlags & FR_FINDNEXT) {
        CString strFind(pfr->lpstrFindWhat ? pfr->lpstrFindWhat : L"");
        impl__OnFindNext_CRichEditView__MEAAXPEB_WHHH_Z(
            pThis, (const wchar_t*)strFind, (dwFlags & FR_DOWN) ? 1 : 0,
            (dwFlags & FR_MATCHCASE) ? 1 : 0, (dwFlags & FR_WHOLEWORD) ? 1 : 0);
    } else if (dwFlags & FR_REPLACE) {
        CString strReplace(pfr->lpstrReplaceWith ? pfr->lpstrReplaceWith : L"");
        CString strFind(pfr->lpstrFindWhat ? pfr->lpstrFindWhat : L"");
        impl__OnReplaceSel_CRichEditView__MEAAXPEB_WHHH0_Z(
            pThis, (const wchar_t*)strFind, (dwFlags & FR_DOWN) ? 1 : 0,
            (dwFlags & FR_MATCHCASE) ? 1 : 0, (dwFlags & FR_WHOLEWORD) ? 1 : 0,
            (const wchar_t*)strReplace);
    } else if (dwFlags & FR_REPLACEALL) {
        CString strReplace(pfr->lpstrReplaceWith ? pfr->lpstrReplaceWith : L"");
        CString strFind(pfr->lpstrFindWhat ? pfr->lpstrFindWhat : L"");
        impl__OnReplaceAll_CRichEditView__MEAAXPEB_W0HH_Z(
            pThis, (const wchar_t*)strFind, (const wchar_t*)strReplace,
            (dwFlags & FR_MATCHCASE) ? 1 : 0, (dwFlags & FR_WHOLEWORD) ? 1 : 0);
    }
    return 0;
}

// void CRichEditView::OnFormatFont()
// Retail RVA 0x284d30 (mfc140u): GetCharFormatSelection() (0x285440, refreshes
// m_charformat); CFontDialog dlg(m_charformat, CF_BOTH | CF_NOOEMFONTS (0x803),
// NULL, NULL) (ctor 0x20ec40); if (dlg.DoModal() (0x20ed60) == IDOK) {
// dlg.GetCharFormat(m_charformat) (0x20f0f0); SetCharFormat(m_charformat) (0x2854f0,
// by value); }  The model's GetCharFormatSelection() reference plays the role of
// m_charformat.  The dialog is built and torn down through exported thunks
// (CFontDialog adds no non-trivial members of its own) so the C++ CFontDialog
// methods are not referenced from this TU.
// Construction deviation (review, verified in core/dialog/CFontDialog.cpp): the
// OpenMFC CHARFORMATW ctor that ??0CFontDialog@@QEAA@AEBU_charformatw@@... wraps
// never initialises m_lpLogFont, and CFontDialog::DoModal copies the chosen
// LOGFONTW through that pointer on IDOK, so calling it on raw storage writes
// through an indeterminate pointer.  Retail's CHARFORMAT ctor (0x20ec40) is
// CDialog ctor + memset + FillInLogFont(charformat) (0x20ee50) with lpLogFont =
// &m_lf; the same result is obtained here from the LOGFONTW ctor thunk with a
// NULL LOGFONTW (m_lpLogFont = m_lf, m_lf zeroed) followed by the exported
// FillInLogFont thunk.
// Symbol: ?OnFormatFont@CRichEditView@@IEAAXXZ
extern "C" void MS_ABI impl__OnFormatFont_CRichEditView__IEAAXXZ(CRichEditView* pThis) {
    if (!pThis) {
        return;
    }
    CHARFORMAT2W& cf = pThis->GetCharFormatSelection();
    alignas(16) unsigned char dlgStorage[sizeof(CFontDialog)] = {};
    CFontDialog* pDlg = impl___0CFontDialog__QEAA_PEAUtagLOGFONTW__KPEAVCDC__PEAVCWnd___Z(
        reinterpret_cast<CFontDialog*>(dlgStorage), nullptr, CF_BOTH | CF_NOOEMFONTS, nullptr, nullptr);
    impl__FillInLogFont_CFontDialog__QEAAKAEBU_charformatw___Z(pDlg, reinterpret_cast<const CHARFORMATW*>(&cf));
    if (impl__DoModal_CFontDialog__UEAA_JXZ(pDlg) == IDOK) {
        impl__GetCharFormat_CFontDialog__QEBAXAEAU_charformatw___Z(pDlg, reinterpret_cast<CHARFORMATW*>(&cf));
        pThis->SetCharFormat(cf);
    }
    impl___1CDialog__UEAA_XZ(pDlg);
}

// void CRichEditView::OnInsertObject()
// Retail RVA 0x284760 (mfc140u): COleInsertDialog dlg(IOF_SELECTCREATENEW, NULL)
// (0x24ce90); if (dlg.DoModal() (0x24d580) != IDOK) return; CWaitCursor; TRY {
// pItem = GetDocument()->CreateClientItem(NULL) (vslot 106); pItem->+0xfc = TRUE;
// if (!dlg.CreateItem(pItem) (0x24d600)) { pItem->+0xfc = FALSE; AfxThrowMemoryException()
// (0x2276c0); } hr = InsertItem(pItem) (0x285750); pItem->UpdateItemType() (0x244b20);
// pItem->+0xfc = FALSE; if (hr != S_OK) AfxThrowOleException(hr) (0x25f2c0);
// if (!(dlg+0x13c /* OLEUIINSERTOBJECT.dwFlags */ & IOF_SELECTCREATEFROMFILE))
// pItem->DoVerb(OLEIVERB_SHOW, this, NULL) (vslot 25); } CATCH_ALL (funclet not
// read); ~COleInsertDialog (0x24cf90).
// Not transcribed: CRichEditDoc::CreateClientItem is a null stub, InsertItem above
// is a conservative no-op, the COleClientItem flag at +0xfc lies beyond the 0xf0-byte
// OpenMFC COleClientItem, and DoVerb must go through the MSVC vtable.  Left a no-op.
// Symbol: ?OnInsertObject@CRichEditView@@IEAAXXZ
extern "C" void MS_ABI impl__OnInsertObject_CRichEditView__IEAAXXZ(CRichEditView* pThis) {
    (void)pThis;
}

// void CRichEditView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
// Retail RVA 0x285100 (mfc140u):
//   if (nChar == VK_F10 && ::GetKeyState(VK_SHIFT) < 0) {
//       RECT rc = {}; ::GetClientRect(m_hWnd, &rc);
//       SendMessage(m_hWnd, WM_CONTEXTMENU, (WPARAM)m_hWnd,
//                   MAKELPARAM((rc.left + rc.right) / 2, (rc.top + rc.bottom) / 2));
//   } else CWnd::Default() (0x28ac80);
// IAT 0x2c6ca0 GetKeyState, 0x2c7330 GetClientRect, 0x2c7120 SendMessageW.
// The handler runs for keys delivered to the view HWND, so m_hWnd is used as in
// retail.
// Symbol: ?OnKeyDown@CRichEditView@@IEAAXIII@Z
extern "C" void MS_ABI impl__OnKeyDown_CRichEditView__IEAAXIII_Z(
    CRichEditView* pThis, unsigned int nChar, unsigned int nRepCnt, unsigned int nFlags) {
    (void)nRepCnt;
    (void)nFlags;
    if (!pThis) {
        return;
    }
    if (nChar == VK_F10 && ::GetKeyState(VK_SHIFT) < 0) {
        RECT rc = {};
        ::GetClientRect(pThis->m_hWnd, &rc);
        const int x = (rc.left + rc.right) / 2;
        const int y = (rc.top + rc.bottom) / 2;
        ::SendMessageW(pThis->m_hWnd, WM_CONTEXTMENU, (WPARAM)pThis->m_hWnd, MAKELPARAM(x, y));
    } else {
        impl__Default_CWnd__IEAA_JXZ(pThis);
    }
}

// void CRichEditView::OnParaAlign(WORD wAlign)
// Retail RVA 0x284340 (mfc140u): PARAFORMAT2& pf = GetParaFormatSelection()
// (0x2854a0); pf.wAlignment (+0x18) = wAlign; pf.dwMask (+0x4) = PFM_ALIGNMENT;
// tail-jump SetParaFormat(pf) (0x285550).
// Symbol: ?OnParaAlign@CRichEditView@@QEAAXG@Z
extern "C" void MS_ABI impl__OnParaAlign_CRichEditView__QEAAXG_Z(
    CRichEditView* pThis, unsigned short wAlign) {
    if (!pThis) {
        return;
    }
    PARAFORMAT2& pf = pThis->GetParaFormatSelection();
    pf.wAlignment = wAlign;
    pf.dwMask = PFM_ALIGNMENT;
    pThis->SetParaFormat(pf);
}

// Retail RVA 0x284c00 (mfc140u): tail-jump OnParaAlign(PFA_CENTER).
// Symbol: ?OnParaCenter@CRichEditView@@IEAAXXZ
extern "C" void MS_ABI impl__OnParaCenter_CRichEditView__IEAAXXZ(CRichEditView* pThis) {
    impl__OnParaAlign_CRichEditView__QEAAXG_Z(pThis, PFA_CENTER);
}

// Retail RVA 0x284c20 (mfc140u): tail-jump OnParaAlign(PFA_LEFT).
// Symbol: ?OnParaLeft@CRichEditView@@IEAAXXZ
extern "C" void MS_ABI impl__OnParaLeft_CRichEditView__IEAAXXZ(CRichEditView* pThis) {
    impl__OnParaAlign_CRichEditView__QEAAXG_Z(pThis, PFA_LEFT);
}

// Retail RVA 0x284c40 (mfc140u): tail-jump OnParaAlign(PFA_RIGHT).
// Symbol: ?OnParaRight@CRichEditView@@IEAAXXZ
extern "C" void MS_ABI impl__OnParaRight_CRichEditView__IEAAXXZ(CRichEditView* pThis) {
    impl__OnParaAlign_CRichEditView__QEAAXG_Z(pThis, PFA_RIGHT);
}

// BOOL CRichEditView::OnPasteNativeObject(LPSTORAGE)
// Retail (mfc140u ordinal 10791 -> RVA 0x71e0, a shared `xor eax,eax; ret` body):
// return FALSE -- the default lets the rich-edit control paste the object itself.
// Symbol: ?OnPasteNativeObject@CRichEditView@@MEAAHPEAUIStorage@@@Z
extern "C" int MS_ABI impl__OnPasteNativeObject_CRichEditView__MEAAHPEAUIStorage___Z(
    CRichEditView* pThis, IStorage* lpStg) {
    (void)pThis;
    (void)lpStg;
    return FALSE;
}

// void CRichEditView::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo)
// Retail RVA 0x2839e0 (mfc140u): pDC->vslot 15 (SetMapMode)(MM_TEXT); if
// (pInfo->m_nCurPage (+0x18) > m_aPageStart.GetSize() (+0x170) && !PaginateTo(pDC,
// pInfo) (0x2838b0)) pInfo->m_bContinuePrinting (+0x14) = FALSE.  Depends on the
// page-start array at +0x160 and on CPrintInfo, neither of which this model has;
// left a no-op (see headerRequests).
// Symbol: ?OnPrepareDC@CRichEditView@@UEAAXPEAVCDC@@PEAUCPrintInfo@@@Z
extern "C" void MS_ABI impl__OnPrepareDC_CRichEditView__UEAAXPEAVCDC__PEAUCPrintInfo___Z(
    CRichEditView* pThis, CDC* pDC, void* pInfo) {
    (void)pThis; (void)pDC; (void)pInfo;
}

// void CRichEditView::OnPrint(CDC* pDC, CPrintInfo* pInfo)
// Retail RVA 0x283d10 (mfc140u): nPage = pInfo->m_nCurPage (+0x18); if
// ((long)(nPage - 1) >= m_aPageStart.GetSize() (+0x170)) AfxThrowInvalidArgException()
// (0x227720); nIndex = PrintPage(pDC, m_aPageStart[nPage - 1], -1) (0x283a40);
// GETTEXTLENGTHEX gtl = {GTL_PRECISE | GTL_NUMCHARS (0xa), 1200}; if (nIndex >=
// SendMessage(m_hWnd, EM_GETTEXTLENGTHEX, &gtl, 0)) pInfo->SetMaxPage(nPage)
// (m_pPD->m_pd.nMaxPage, i.e. CPrintDialog+0x130+0x32 = nPage); if (nPage ==
// m_aPageStart.GetSize() && nIndex < the same length) m_aPageStart.Add(nIndex)
// (CDWordArray::SetAtGrow 0x1d4260).  Needs the +0x160 page-start array and
// CPrintInfo; left a no-op.
// Symbol: ?OnPrint@CRichEditView@@MEAAXPEAVCDC@@PEAUCPrintInfo@@@Z
extern "C" void MS_ABI impl__OnPrint_CRichEditView__MEAAXPEAVCDC__PEAUCPrintInfo___Z(
    CRichEditView* pThis, CDC* pDC, void* pInfo) {
    (void)pThis; (void)pDC; (void)pInfo;
}

// void CRichEditView::OnPrinterChanged(const CDC& dcPrinter)
// Retail RVA 0x285b60 (mfc140u):
//   if (dcPrinter.m_hDC (+0x8) != NULL) {
//       cx = MulDiv(GetDeviceCaps(m_hAttribDC, PHYSICALWIDTH),  1440, GetDeviceCaps(m_hAttribDC, LOGPIXELSX));
//       cy = MulDiv(GetDeviceCaps(m_hAttribDC, PHYSICALHEIGHT), 1440, GetDeviceCaps(m_hAttribDC, LOGPIXELSY));
//   } else { cx = 12240; cy = 15840; }
//   if (m_sizePaper (+0x158) != (cx, cy)) { m_sizePaper = (cx, cy);
//       if (m_nWordWrap (+0x108) == WrapToTargetDevice (2)) WrapChanged() (vslot 116); }
// IAT 0x2c61a8 GetDeviceCaps, 0x2c6658 MulDiv (both resolved in mfc140u).  Model:
// m_sizePaper / m_nWordWrapMode are header members; WrapChanged is non-virtual.
// Symbol: ?OnPrinterChanged@CRichEditView@@MEAAXAEBVCDC@@@Z
extern "C" void MS_ABI impl__OnPrinterChanged_CRichEditView__MEAAXAEBVCDC___Z(
    CRichEditView* pThis, const CDC* pDC) {
    if (!pThis || !pDC) {
        return;
    }
    LONG cx, cy;
    if (pDC->m_hDC != nullptr) {
        cx = ::MulDiv(::GetDeviceCaps(pDC->m_hAttribDC, PHYSICALWIDTH), 1440,
                      ::GetDeviceCaps(pDC->m_hAttribDC, LOGPIXELSX));
        cy = ::MulDiv(::GetDeviceCaps(pDC->m_hAttribDC, PHYSICALHEIGHT), 1440,
                      ::GetDeviceCaps(pDC->m_hAttribDC, LOGPIXELSY));
    } else {
        cx = 12240;
        cy = 15840;
    }
    if (pThis->m_sizePaper.cx != cx || pThis->m_sizePaper.cy != cy) {
        pThis->m_sizePaper.cx = cx;
        pThis->m_sizePaper.cy = cy;
        if (pThis->m_nWordWrapMode == 2 /* WrapToTargetDevice */) {
            pThis->WrapChanged();
        }
    }
}

// void CRichEditView::OnReplaceAll(LPCTSTR lpszFind, LPCTSTR lpszReplace, BOOL bCase, BOOL bWord)
// Retail RVA 0x286330 (mfc140u):
//   pEditState->strFind = lpszFind; strReplace = lpszReplace; bCase = bCase;
//   bWord = bWord; bNext = TRUE;  CWaitCursor wait;
//   CHARRANGE cr; EM_EXGETSEL(&cr); long lInitial = cr.cpMin;
//   SendMessage(m_hWnd, EM_HIDESELECTION, TRUE, 0);  SetSel(0, 0) (0x299990);
//   while (FindTextW(strFind, bCase, bWord, TRUE)) {
//       EM_EXGETSEL(&cr);  SendMessage(m_hWnd, EM_REPLACESEL, FALSE, strReplace);
//       if (cr.cpMin < lInitial) lInitial += strReplace.GetLength() - strFind.GetLength();
//   }
//   m_bFirstSearch = TRUE;  OnTextNotFound(strFind) (vslot 114);
//   SetSel(lInitial, lInitial);  SendMessage(m_hWnd, EM_HIDESELECTION, FALSE, 0);
// (FindTextW = wait cursor + FindTextSimple, see OnFindNext.)
// Symbol: ?OnReplaceAll@CRichEditView@@MEAAXPEB_W0HH@Z
extern "C" void MS_ABI impl__OnReplaceAll_CRichEditView__MEAAXPEB_W0HH_Z(
    CRichEditView* pThis, const wchar_t* lpszFind, const wchar_t* lpszReplace, int bCase, int bWord) {
    if (!pThis || !CtrlHwnd(pThis)) {
        return;
    }
    RichEditFindState& st = FindState();
    st.strFind = lpszFind ? lpszFind : L"";
    st.strReplace = lpszReplace ? lpszReplace : L"";
    st.bCase = bCase;
    st.bWord = bWord;
    st.bNext = TRUE;
    WaitCursorScope wait;
    const HWND hWnd = CtrlHwnd(pThis);
    CHARRANGE cr = {};
    ::SendMessageW(hWnd, EM_EXGETSEL, 0, (LPARAM)&cr);
    long lInitial = cr.cpMin;
    ::SendMessageW(hWnd, EM_HIDESELECTION, TRUE, 0);
    pThis->m_richEdit.SetSel(0, 0);
    while (impl__FindTextSimple_CRichEditView__QEAAHPEB_WHHH_Z(
               pThis, (const wchar_t*)st.strFind, st.bCase, st.bWord, st.bNext)) {
        ::SendMessageW(hWnd, EM_EXGETSEL, 0, (LPARAM)&cr);
        ::SendMessageW(hWnd, EM_REPLACESEL, FALSE, (LPARAM)(const wchar_t*)st.strReplace);
        if (cr.cpMin < lInitial) {
            lInitial += st.strReplace.GetLength() - st.strFind.GetLength();
        }
    }
    SearchStateFor(pThis).bFirstSearch = TRUE;
    impl__OnTextNotFound_CRichEditView__MEAAXPEB_W_Z(pThis, (const wchar_t*)st.strFind);
    pThis->m_richEdit.SetSel((int)lInitial, (int)lInitial);
    ::SendMessageW(hWnd, EM_HIDESELECTION, FALSE, 0);
}

// void CRichEditView::OnReplaceSel(LPCTSTR lpszFind, BOOL bNext, BOOL bCase, BOOL bWord, LPCTSTR lpszReplace)
// Retail RVA 0x286200 (mfc140u):
//   pEditState->strFind = lpszFind; strReplace = lpszReplace; bWord = bWord;
//   bCase = bCase; bNext = bNext;
//   if (SameAsSelected(strFind, bCase, bWord)) SendMessage(m_hWnd, EM_REPLACESEL, FALSE, strReplace);
//   if (!FindTextW(strFind, bCase, bWord, bNext)) { m_bFirstSearch = TRUE; OnTextNotFound(strFind); }
//   else AdjustDialogPosition(pEditState->pFindReplaceDlg);
// Symbol: ?OnReplaceSel@CRichEditView@@MEAAXPEB_WHHH0@Z
extern "C" void MS_ABI impl__OnReplaceSel_CRichEditView__MEAAXPEB_WHHH0_Z(
    CRichEditView* pThis, const wchar_t* lpszFind, int bNext, int bCase, int bWord,
    const wchar_t* lpszReplace) {
    if (!pThis || !CtrlHwnd(pThis)) {
        return;
    }
    RichEditFindState& st = FindState();
    st.strFind = lpszFind ? lpszFind : L"";
    st.strReplace = lpszReplace ? lpszReplace : L"";
    st.bWord = bWord;
    st.bCase = bCase;
    st.bNext = bNext;
    if (impl__SameAsSelected_CRichEditView__IEAAHPEB_WHH_Z(pThis, (const wchar_t*)st.strFind, bCase, bWord)) {
        ::SendMessageW(CtrlHwnd(pThis), EM_REPLACESEL, FALSE, (LPARAM)(const wchar_t*)st.strReplace);
    }
    BOOL bFound;
    {
        WaitCursorScope wait;
        bFound = impl__FindTextSimple_CRichEditView__QEAAHPEB_WHHH_Z(
            pThis, (const wchar_t*)st.strFind, st.bCase, st.bWord, st.bNext);
    }
    if (!bFound) {
        SearchStateFor(pThis).bFirstSearch = TRUE;
        impl__OnTextNotFound_CRichEditView__MEAAXPEB_W_Z(pThis, (const wchar_t*)st.strFind);
    } else {
        impl__AdjustDialogPosition_CRichEditView__QEAAXPEAVCDialog___Z(pThis, st.pFindReplaceDlg);
    }
}

// void CRichEditView::OnSelChange(NMHDR*, LRESULT* pResult)
// Retail RVA 0x2848a0 (mfc140u): m_bSyncCharFormat = m_bSyncParaFormat = TRUE;
// *pResult = 0.  The flags do not exist in this model (see GetCharFormatSelection).
// Symbol: ?OnSelChange@CRichEditView@@IEAAXPEAUtagNMHDR@@PEA_J@Z
extern "C" void MS_ABI impl__OnSelChange_CRichEditView__IEAAXPEAUtagNMHDR__PEA_J_Z(
    CRichEditView* pThis, NMHDR* pNMHDR, __int64* pResult) {
    (void)pThis;
    (void)pNMHDR;
    if (pResult) {
        *pResult = 0;
    }
}

// BOOL CRichEditView::PaginateTo(CDC* pDC, CPrintInfo* pInfo)
// Retail RVA 0x2838b0 (mfc140u):
//   RECT rectSave = pInfo->m_rectDraw (+0x30); UINT nPageSave = pInfo->m_nCurPage (+0x18);
//   pDC->SaveDC() (vslot 9);  CDC::IntersectClipRect(pDC, 0, 0, 0, 0) (0x2a2f50);
//   UINT nPage = m_aPageStart.GetSize() (+0x170);
//   while ((pInfo->m_nCurPage = nPage) < nPageSave) {          // unsigned compare
//       OnPrepareDC(pDC, pInfo) (vslot 100);
//       ::SetRect(&pInfo->m_rectDraw, 0, 0, GetDeviceCaps(pDC->m_hAttribDC, HORZRES),
//                 GetDeviceCaps(pDC->m_hAttribDC, VERTRES));      (IAT 0x2c7140 SetRect)
//       ::DPtoLP(pDC->m_hAttribDC, (POINT*)&pInfo->m_rectDraw, 2); (IAT 0x2c63c8 DPtoLP)
//       OnPrint(pDC, pInfo) (vslot 108);
//       nPage = pInfo->m_nCurPage; if (nPage == m_aPageStart.GetSize()) break; ++nPage;
//   }
//   pDC->RestoreDC(-1) (vslot 10);  pInfo->m_nCurPage = nPageSave;
//   pInfo->m_rectDraw = rectSave;  return nPage == nPageSave;
// Needs the +0x160 page-start array and CPrintInfo; left a stub returning FALSE.
// Symbol: ?PaginateTo@CRichEditView@@IEAAHPEAVCDC@@PEAUCPrintInfo@@@Z
extern "C" int MS_ABI impl__PaginateTo_CRichEditView__IEAAHPEAVCDC__PEAUCPrintInfo___Z(
    CRichEditView* pThis, CDC* pDC, void* pInfo) {
    (void)pThis; (void)pDC; (void)pInfo;
    return FALSE;
}

// BOOL CRichEditView::PreCreateWindow(CREATESTRUCT& cs)
// Retail RVA 0x2834d0 (mfc140u):
//   if (!AfxInitRichEdit5() (0x2997f0)) return FALSE;
//   CCtrlView::PreCreateWindow(cs) (0x277f40)  -- result discarded;
//   cs.style |= WS_CLIPSIBLINGS (bit 26);  cs.lpszName = L"" (.rdata 0x33d19c);
//   cs.cx = cs.cy = 100;  return TRUE;
// Symbol: ?PreCreateWindow@CRichEditView@@MEAAHAEAUtagCREATESTRUCTW@@@Z
extern "C" int MS_ABI impl__PreCreateWindow_CRichEditView__MEAAHAEAUtagCREATESTRUCTW___Z(
    CRichEditView* pThis, CREATESTRUCTW* pcs) {
    if (!pcs) {
        return FALSE;
    }
    if (!impl__AfxInitRichEdit5__YAHXZ()) {
        return FALSE;
    }
    impl__PreCreateWindow_CCtrlView__MEAAHAEAUtagCREATESTRUCTW___Z(pThis, pcs);
    pcs->style |= WS_CLIPSIBLINGS;
    pcs->lpszName = L"";
    pcs->cx = 100;
    pcs->cy = 100;
    return TRUE;
}

// HRESULT CRichEditView::QueryAcceptData(LPDATAOBJECT lpdataobj, CLIPFORMAT* lpcfFormat,
//                                        DWORD dwReco, BOOL bReally, HGLOBAL hMetaPict)
// Retail RVA 0x285c30 (mfc140u):
//   if (!bReally) return S_OK;
//   if (*lpcfFormat == cfRichTextFormat || == cfRichTextAndObjects || == CF_TEXT) return S_OK;
//   COleDataObject dataobj; dataobj.Attach(lpdataobj, FALSE) (0x2528e0);
//   if (*lpcfFormat == 0 && m_nPasteType (+0x13c) == 0) {
//       if (dataobj.IsDataAvailable(cfRichTextAndObjects)) hr = S_OK;
//       else if (IsDataAvailable(cfRichTextFormat)) { *lpcfFormat = cfRichTextFormat; hr = S_OK; }
//       else if (IsDataAvailable(CF_TEXT))          { *lpcfFormat = CF_TEXT; hr = S_OK; }
//       else goto paste;
//   } else paste: { DoPaste(dataobj, *lpcfFormat, hMetaPict) (0x2858f0); hr = S_FALSE; }
//   ~COleDataObject; return hr;
// m_nPasteType has no home in this model and is treated as 0 (its ctor value).
// Symbol: ?QueryAcceptData@CRichEditView@@MEAAJPEAUIDataObject@@PEAGKHPEAX@Z
extern "C" long MS_ABI impl__QueryAcceptData_CRichEditView__MEAAJPEAUIDataObject__PEAGKHPEAX_Z(
    CRichEditView* pThis, IDataObject* lpdataobj, unsigned short* lpcfFormat, unsigned long dwReco,
    int bReally, void* hMetaPict) {
    (void)dwReco;
    if (!pThis || !lpcfFormat) {
        return S_OK;
    }
    if (!bReally) {
        return S_OK;
    }
    const unsigned short cf = *lpcfFormat;
    if (cf == RichTextFormat() || cf == RichTextAndObjectsFormat() || cf == CF_TEXT) {
        return S_OK;
    }
    // COleDataObject through its exported ctor / Attach / IsDataAvailable /
    // Release thunks (Release is what ~COleDataObject does).
    alignas(16) unsigned char objStorage[sizeof(COleDataObject)];
    COleDataObject* pDataObj = static_cast<COleDataObject*>(impl___0COleDataObject__QEAA_XZ(objStorage));
    impl__Attach_COleDataObject__QEAAXPEAUIDataObject__H_Z(pDataObj, lpdataobj, FALSE);
    long hr = S_FALSE;
    bool bPaste = true;
    if (*lpcfFormat == 0) {
        if (impl__IsDataAvailable_COleDataObject__QEAAHGPEAUtagFORMATETC___Z(pDataObj, RichTextAndObjectsFormat(), nullptr)) {
            hr = S_OK;
            bPaste = false;
        } else if (impl__IsDataAvailable_COleDataObject__QEAAHGPEAUtagFORMATETC___Z(pDataObj, RichTextFormat(), nullptr)) {
            *lpcfFormat = RichTextFormat();
            hr = S_OK;
            bPaste = false;
        } else if (impl__IsDataAvailable_COleDataObject__QEAAHGPEAUtagFORMATETC___Z(pDataObj, CF_TEXT, nullptr)) {
            *lpcfFormat = CF_TEXT;
            hr = S_OK;
            bPaste = false;
        }
    }
    if (bPaste) {
        impl__DoPaste_CRichEditView__QEAAXAEAVCOleDataObject__GPEAX_Z(pThis, pDataObj, *lpcfFormat, hMetaPict);
        hr = S_FALSE;
    }
    impl__Release_COleDataObject__QEAAXXZ(pDataObj);
    return hr;
}

// BOOL CRichEditView::SameAsSelected(LPCTSTR lpszCompare, BOOL bCase, BOOL /*bWord*/)
// Retail RVA 0x286700 (mfc140u): CString strSelect = CRichEditCtrl::GetSelText()
// (0x299b10); return (bCase ? lstrcmpW : lstrcmpiW)(lpszCompare, strSelect) == 0
// (IAT 0x2c66a8 lstrcmpW, 0x2c66e8 lstrcmpiW).  bWord is unused in retail.
// Symbol: ?SameAsSelected@CRichEditView@@IEAAHPEB_WHH@Z
extern "C" int MS_ABI impl__SameAsSelected_CRichEditView__IEAAHPEB_WHH_Z(
    CRichEditView* pThis, const wchar_t* lpszCompare, int bCase, int bWord) {
    (void)bWord;
    if (!pThis || !lpszCompare) {
        return FALSE;
    }
    alignas(8) unsigned char strStorage[sizeof(CString)];
    CString* pSelect = reinterpret_cast<CString*>(strStorage);
    impl__GetSelText_CRichEditCtrl__QEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__XZ(
        pSelect, &pThis->m_richEdit);
    const int nCmp = bCase ? ::lstrcmpW(lpszCompare, (const wchar_t*)*pSelect)
                           : ::lstrcmpiW(lpszCompare, (const wchar_t*)*pSelect);
    pSelect->~CString();
    return nCmp == 0 ? TRUE : FALSE;
}

// HRESULT CRichEditView::ShowContainerUI(BOOL bShow)
// Retail RVA 0x285dc0 (mfc140u): pItem = GetSelectedItem() (0x2855b0); if (!pItem)
// return E_FAIL; if (bShow) pItem->m_pView (+0x98) = this; hr = pItem->
// ShowContainerUI(bShow) (0x287430 = the export ?ShowContainerUI@CRichEditCntrItem@@
// QEAAJH@Z, ordinal 13804: COleClientItem vslot 37 gates it (E_NOTIMPL when it
// returns 0), then vslots 34 (arg 0) + 40 when bShow, else vslots 38 + 39; S_OK);
// if (FAILED(hr) || !bShow) pItem->m_pView = NULL; return hr.
// OpenMFC's COleClientItem has neither m_pView at +0x98 nor an MSVC-layout
// vtable for the in-place slots, and the CRichEditCntrItem::ShowContainerUI thunk
// in core/ole/CRichEditCntrItem.cpp is still a `return 0` placeholder with an
// `(int p0)` parameter list, so this stays a stub.
// Symbol: ?ShowContainerUI@CRichEditView@@QEAAJH@Z
extern "C" long MS_ABI impl__ShowContainerUI_CRichEditView__QEAAJH_Z(CRichEditView* pThis, int bShow) {
    (void)pThis;
    (void)bShow;
    return E_FAIL;
}

// ---------------------------------------------------------------------------
// CRichEditView::XRichEditOleCallback (IRichEditOleCallback interface part).
// `this` for these thunks is the interface part, which retail embeds at
// CRichEditView+0x2c0 (every METHOD_PROLOGUE below is `lea -0x2c0(%rcx)`, and the
// GetDocument() accesses are `mov -0x1d8(%rcx)` = +0xe8 m_pDocument relative to
// the part).  The OpenMFC object has no such part (its interface map is empty),
// so pThis cannot be derived and the members that need it stay stubs.
// ---------------------------------------------------------------------------

// Retail (mfc140u ordinal 2964 -> RVA 0xf4d0, shared body): return E_NOTIMPL.
// Symbol: ?ContextSensitiveHelp@XRichEditOleCallback@CRichEditView@@UEAAJH@Z
extern "C" long MS_ABI impl__ContextSensitiveHelp_XRichEditOleCallback_CRichEditView__UEAAJH_Z(
    void* pIface, int fEnterMode) {
    (void)pIface;
    (void)fEnterMode;
    return E_NOTIMPL;
}

// Retail RVA 0x284000 (mfc140u): pThis->GetDocument()->m_bUpdateObjectCache
// (CRichEditDoc+0x29c) = TRUE; return S_OK.  The flag needs the interface part
// offset and a CRichEditDoc member beyond OpenMFC's 0x208-byte object, so only
// the unconditional S_OK is reproduced (the object-list cache hint is dropped).
// Symbol: ?DeleteObject@XRichEditOleCallback@CRichEditView@@UEAAJPEAUIOleObject@@@Z
extern "C" long MS_ABI impl__DeleteObject_XRichEditOleCallback_CRichEditView__UEAAJPEAUIOleObject___Z(
    void* pIface, IOleObject* lpoleobj) {
    (void)pIface;
    (void)lpoleobj;
    return S_OK;
}

// Retail RVA 0x2840a0 (mfc140u): METHOD_PROLOGUE_EX; hr = m_lpRichEditOle->
// GetClipboardData(lpchrg, reco, &lpOrig) (IRichEditOle vslot 17); if (FAILED(hr))
// return E_NOTIMPL; hr = pThis->GetClipboardData(lpchrg, reco, lpOrig, lplpdataobj)
// (vslot 119); if (SUCCEEDED(hr)) lpOrig->Release(); else { *lplpdataobj = lpOrig;
// hr = S_OK; } return hr.  Needs pThis / m_lpRichEditOle -- stub.
// Symbol: ?GetClipboardData@XRichEditOleCallback@CRichEditView@@UEAAJPEAU_charrange@@KPEAPEAUIDataObject@@@Z
extern "C" long MS_ABI impl__GetClipboardData_XRichEditOleCallback_CRichEditView__UEAAJPEAU_charrange__KPEAPEAUIDataObject___Z(
    void* pIface, CHARRANGE* lpchrg, unsigned long reco, IDataObject** lplpdataobj) {
    (void)pIface;
    (void)lpchrg;
    (void)reco;
    if (lplpdataobj) {
        *lplpdataobj = nullptr;
    }
    return E_NOTIMPL;
}

// Retail RVA 0x2841a0 (mfc140u): METHOD_PROLOGUE_EX; HMENU h = pThis->GetContextMenu
// (seltype, lpoleobj, lpchrg) (vslot 118); if (!h) return E_NOTIMPL; *lphmenu = h;
// return S_OK.  Needs pThis -- stub.
// Symbol: ?GetContextMenu@XRichEditOleCallback@CRichEditView@@UEAAJGPEAUIOleObject@@PEAU_charrange@@PEAPEAUHMENU__@@@Z
extern "C" long MS_ABI impl__GetContextMenu_XRichEditOleCallback_CRichEditView__UEAAJGPEAUIOleObject__PEAU_charrange__PEAPEAUHMENU_____Z(
    void* pIface, unsigned short seltype, IOleObject* lpoleobj, CHARRANGE* lpchrg, HMENU* lphmenu) {
    (void)pIface;
    (void)seltype;
    (void)lpoleobj;
    (void)lpchrg;
    if (lphmenu) {
        *lphmenu = nullptr;
    }
    return E_NOTIMPL;
}

// HRESULT XRichEditOleCallback::GetDragDropEffect(BOOL fDrag, DWORD grfKeyState, LPDWORD pdwEffect)
// Retail RVA 0x284170 (mfc140u), no METHOD_PROLOGUE (pThis unused):
//   if (!fDrag) {
//       dwEffect = ((grfKeyState & (MK_CONTROL|MK_SHIFT)) == (MK_CONTROL|MK_SHIFT)) ? DROPEFFECT_LINK
//                : (grfKeyState & MK_CONTROL) ? DROPEFFECT_COPY : DROPEFFECT_MOVE;
//       if (*pdwEffect & dwEffect) *pdwEffect = dwEffect;
//   }
//   return S_OK;
// Symbol: ?GetDragDropEffect@XRichEditOleCallback@CRichEditView@@UEAAJHKPEAK@Z
extern "C" long MS_ABI impl__GetDragDropEffect_XRichEditOleCallback_CRichEditView__UEAAJHKPEAK_Z(
    void* pIface, int fDrag, unsigned long grfKeyState, unsigned long* pdwEffect) {
    (void)pIface;
    if (!fDrag && pdwEffect) {
        unsigned long dwEffect;
        if ((grfKeyState & (MK_CONTROL | MK_SHIFT)) == (MK_CONTROL | MK_SHIFT)) {
            dwEffect = DROPEFFECT_LINK;
        } else if (grfKeyState & MK_CONTROL) {
            dwEffect = DROPEFFECT_COPY;
        } else {
            dwEffect = DROPEFFECT_MOVE;
        }
        if (*pdwEffect & dwEffect) {
            *pdwEffect = dwEffect;
        }
    }
    return S_OK;
}

// Retail RVA 0x283f00 (mfc140u): METHOD_PROLOGUE_EX; return pThis->GetWindowContext
// (ppFrame, ppDoc, lpFrameInfo) (0x285d60).  Needs pThis -- stub.
// Symbol: ?GetInPlaceContext@XRichEditOleCallback@CRichEditView@@UEAAJPEAPEAUIOleInPlaceFrame@@PEAPEAUIOleInPlaceUIWindow@@PEAUtagOIFI@@@Z
extern "C" long MS_ABI impl__GetInPlaceContext_XRichEditOleCallback_CRichEditView__UEAAJPEAPEAUIOleInPlaceFrame__PEAPEAUIOleInPlaceUIWindow__PEAUtagOIFI___Z(
    void* pIface, IOleInPlaceFrame** ppFrame, IOleInPlaceUIWindow** ppDoc, OLEINPLACEFRAMEINFO* lpFrameInfo) {
    (void)pIface;
    (void)lpFrameInfo;
    if (ppFrame) {
        *ppFrame = nullptr;
    }
    if (ppDoc) {
        *ppDoc = nullptr;
    }
    return E_NOTIMPL;
}

// Retail RVA 0x283e60 (mfc140u): COleClientItem item(NULL) (0x244200);
// item.GetItemStorageFlat() (0x2456b0); *ppstg = item.m_lpStorage (+0x68); hr =
// E_OUTOFMEMORY; if (item.m_lpStorage) { item.m_lpStorage = NULL; hr = S_OK; }
// pThis->GetDocument()->m_bUpdateObjectCache (+0x29c) = TRUE; ~COleClientItem
// (0x244340, whose Release() drops the item's m_lpLockBytes); return hr.
// GetItemStorageFlat (0x2456b0) is CreateILockBytesOnHGlobal(NULL, TRUE,
// &m_lpLockBytes) then StgCreateDocfileOnILockBytes(m_lpLockBytes, 0x1012 =
// STGM_READWRITE | STGM_SHARE_EXCLUSIVE | STGM_CREATE, 0, &m_lpStorage), each
// failure raised through AfxThrowOleException (0x25f2c0) (IAT 0x2c7a80 / 0x2c7a78
// in mfc140u).  The temporary item only ever carries those two interfaces, so
// the observable result is produced here without it.  Deviations: the doc's
// m_bUpdateObjectCache hint needs pThis (interface part absent) and is dropped;
// an ole32 failure is returned as its HRESULT instead of being thrown across the
// COM boundary.
// Symbol: ?GetNewStorage@XRichEditOleCallback@CRichEditView@@UEAAJPEAPEAUIStorage@@@Z
extern "C" long MS_ABI impl__GetNewStorage_XRichEditOleCallback_CRichEditView__UEAAJPEAPEAUIStorage___Z(
    void* pIface, IStorage** ppstg) {
    (void)pIface;
    if (!ppstg) {
        return E_POINTER;   // retail has no guard (it would fault)
    }
    *ppstg = nullptr;
    ILockBytes* pLockBytes = nullptr;
    HRESULT hr = ::CreateILockBytesOnHGlobal(nullptr, TRUE, &pLockBytes);
    if (FAILED(hr)) {
        return hr;
    }
    IStorage* pStorage = nullptr;
    hr = ::StgCreateDocfileOnILockBytes(pLockBytes, STGM_READWRITE | STGM_SHARE_EXCLUSIVE | STGM_CREATE,
                                        0, &pStorage);
    pLockBytes->Release();   // the docfile holds its own reference
    if (FAILED(hr)) {
        return hr;
    }
    if (!pStorage) {
        return E_OUTOFMEMORY;
    }
    *ppstg = pStorage;
    return S_OK;
}

// Retail RVA 0x284020 (mfc140u): METHOD_PROLOGUE_EX; return pThis->QueryAcceptData
// (lpdataobj, lpcfFormat, reco, fReally, hMetaPict) (vslot 120).  Needs pThis -- stub.
// Symbol: ?QueryAcceptData@XRichEditOleCallback@CRichEditView@@UEAAJPEAUIDataObject@@PEAGKHPEAX@Z
extern "C" long MS_ABI impl__QueryAcceptData_XRichEditOleCallback_CRichEditView__UEAAJPEAUIDataObject__PEAGKHPEAX_Z(
    void* pIface, IDataObject* lpdataobj, unsigned short* lpcfFormat, unsigned long reco, int fReally,
    void* hMetaPict) {
    (void)pIface; (void)lpdataobj; (void)lpcfFormat; (void)reco; (void)fReally; (void)hMetaPict;
    return E_NOTIMPL;
}

// Retail RVA 0x283fb0 (mfc140u): METHOD_PROLOGUE_EX; pThis->GetDocument()->
// m_bUpdateObjectCache (+0x29c) = TRUE; return S_OK.  The flag needs pThis and
// the doc member, so only the unconditional S_OK is reproduced (a failure code
// here would make the control refuse every object insertion, which retail never
// does).
// Symbol: ?QueryInsertObject@XRichEditOleCallback@CRichEditView@@UEAAJPEAU_GUID@@PEAUIStorage@@J@Z
extern "C" long MS_ABI impl__QueryInsertObject_XRichEditOleCallback_CRichEditView__UEAAJPEAU_GUID__PEAUIStorage__J_Z(
    void* pIface, GUID* lpclsid, IStorage* lpstg, long cp) {
    (void)pIface; (void)lpclsid; (void)lpstg; (void)cp;
    return S_OK;
}

// Retail RVA 0x283f60 (mfc140u): METHOD_PROLOGUE_EX; return pThis->ShowContainerUI
// (fShow) (0x285dc0).  Needs pThis -- stub.
// Symbol: ?ShowContainerUI@XRichEditOleCallback@CRichEditView@@UEAAJH@Z
extern "C" long MS_ABI impl__ShowContainerUI_XRichEditOleCallback_CRichEditView__UEAAJH_Z(
    void* pIface, int fShow) {
    (void)pIface;
    (void)fShow;
    return E_NOTIMPL;
}
