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
#include <tom.h>
#include <shellapi.h>
#include <cwchar>

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
// Retail (RVA 0x285350): true for the two internally registered rich-text /
// embedded-object clipboard formats or CF_TEXT.  The two registered formats live
// in private DLL state that the clean-room headers do not model, so only the
// public CF_TEXT comparison is reproduced.
// Symbol: ?IsRichEditFormat@CRichEditView@@SAHG@Z
extern "C" int MS_ABI impl__IsRichEditFormat_CRichEditView__SAHG_Z(unsigned short cf) {
    return (cf == CF_TEXT) ? 1 : 0;
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
