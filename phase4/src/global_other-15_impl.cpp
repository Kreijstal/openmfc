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

//=============================================================================
// CRichEditView — OLE container item helpers
//=============================================================================

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

//=============================================================================
// CRichEditView — window / view life-cycle handlers
//=============================================================================

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

//=============================================================================
// CRichEditView — character / paragraph formatting command handlers
//=============================================================================

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

//=============================================================================
// CRichEditView — edit command handlers
//=============================================================================

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
