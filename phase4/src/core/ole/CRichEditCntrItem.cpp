// CRichEditCntrItem — OpenMFC implementation.
// Sources: ole_olectors_exports.cpp, richedit_ole_ext.cpp

#include "detail/CRichEditDocSupport.h"

#include "openmfc/afxole.h"
#include "openmfc/afxmfc.h"
#include <richedit.h>
#include <richole.h>
#include <cstddef>
#include <cstdlib>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

























// Symbol: ??0CRichEditCntrItem@@QEAA@PEAU_reobject@@PEAVCRichEditDoc@@@Z
// CRichEditCntrItem::CRichEditCntrItem(REOBJECT* pObject, CRichEditDoc* pContainerDoc)
// NOTE: the OpenMFC constructor currently takes only the container document.
extern "C" void* MS_ABI impl___0CRichEditCntrItem__QEAA_PEAU_reobject__PEAVCRichEditDoc___Z(void* pObject, CRichEditDoc* pContainerDoc) {
    (void)pObject;
    return new CRichEditCntrItem(pContainerDoc);
}
// Symbol: ?GetClientSite@CRichEditCntrItem@@UEAAPEAUIOleClientSite@@XZ
extern "C" void* MS_ABI impl__GetClientSite_CRichEditCntrItem__UEAAPEAUIOleClientSite__XZ(CRichEditCntrItem* pThis) {
    return pThis ? pThis->GetClientSite() : nullptr;
}
// Symbol: ?GetWindowContext@CRichEditCntrItem@@QEAAJPEAPEAUIOleInPlaceFrame@@PEAPEAUIOleInPlaceUIWindow@@PEAUtagOIFI@@@Z
extern "C" long MS_ABI impl__GetWindowContext_CRichEditCntrItem__QEAAJPEAPEAUIOleInPlaceFrame__PEAPEAUIOleInPlaceUIWindow__PEAUtagOIFI___Z(
    CRichEditCntrItem* pThis,
    IOleInPlaceFrame** ppFrame,
    IOleInPlaceUIWindow** ppDoc,
    LPOLEINPLACEFRAMEINFO lpFrameInfo) {
    return pThis ? pThis->GetWindowContext(ppFrame, ppDoc, lpFrameInfo) : -1;
}
CRichEditCntrItem::CRichEditCntrItem(COleDocument* pContainerDoc)
    : COleClientItem(pContainerDoc) {
    memset(_richeditcntritem_padding, 0, sizeof(_richeditcntritem_padding));
}
CRichEditCntrItem::~CRichEditCntrItem() {
}
IOleClientSite* CRichEditCntrItem::GetClientSite() {
    if (!m_lpObject) {
        return nullptr;
    }

    IOleClientSite* pSite = nullptr;
    if (FAILED(m_lpObject->GetClientSite(&pSite))) {
        return nullptr;
    }

    return pSite;
}
long CRichEditCntrItem::GetWindowContext(IOleInPlaceFrame** ppFrame,
                                        IOleInPlaceUIWindow** ppDoc,
                                        LPOLEINPLACEFRAMEINFO lpFrameInfo) {
    if (lpFrameInfo) {
        memset(lpFrameInfo, 0, sizeof(*lpFrameInfo));
        lpFrameInfo->cb = sizeof(*lpFrameInfo);
    }

    if (ppFrame) {
        *ppFrame = nullptr;
        if (m_lpFrame) {
            *ppFrame = m_lpFrame;
            m_lpFrame->AddRef();
        }
    }

    if (ppDoc) {
        *ppDoc = nullptr;
    }

    if (lpFrameInfo && m_pContainerDoc) {
        POSITION pos = m_pContainerDoc->GetFirstViewPosition();
        if (pos) {
            CWnd* pView = static_cast<CWnd*>(m_pContainerDoc->GetNextView(pos));
            if (pView) {
                lpFrameInfo->hwndFrame = pView->GetSafeHwnd();
            }
        }
    }
    return S_OK;
}
// ---------------------------------------------------------------------------
// Retail references for the thunks below.  All RVAs are mfc140u (resolved from
// the export table by ordinal); the bodies were read with disas.py, whose
// function bodies are byte-identical to mfc140.dll.
//
// Retail layout used by these bodies (afxrich.h / afxole.h, MSVC x64):
//   +0x40  CDocItem::m_pDocument                 -> OpenMFC CDocItem::m_pDocument
//   +0x98  COleClientItem::m_pView               -> not modeled in OpenMFC
//   +0xf0  CRichEditCntrItem::m_lpClientSite     -> not modeled in OpenMFC
//   CRichEditView +0x110 m_lpRichEditOle         -> OpenMFC does not cache it;
//          fetched per call with EM_GETOLEINTERFACE (AddRef'd) and Released,
//          the same model core/doc/CRichEditDoc.cpp and
//          core/view/CRichEditView.cpp use.
// The retail bodies reach the view as m_pDocument->GetView(), a virtual call
// through CRichEditDoc vtable byte offset 0x358 (slot 107).  Evidence: retail
// CRichEditCntrItem::GetClientSite (RVA 0x287720, mfc140u) checks
// IsKindOf(CRichEditDoc) on m_pDocument, makes the same +0x358 call, and
// checks the result IsKindOf(CRichEditView) (descriptors 0x3357e0 / 0x3359e8,
// mfc140u).  The slot number is also consistent with a count: the mfc140u
// COleLinkingDoc vftable (0x330560) has slots 0..84, the real afxole.h gives
// COleServerDoc 21 further virtuals (85..105; COleServerDoc is AFX_NOVTABLE so
// there is no retail vftable of its own to dump), and afxrich.h then declares
// CreateClientItem (106) and GetView (107).  OpenMFC's CRichEditDoc does not
// declare GetView at all, so that call is made statically through the
// GetView thunk below; an application override of GetView is not honoured.
// ---------------------------------------------------------------------------
extern "C" CRichEditView* MS_ABI impl__GetView_CRichEditDoc__UEBAPEAVCRichEditView__XZ(
    const CRichEditDoc* pThis);                                                  // core/doc/CRichEditDoc.cpp
extern "C" IRichEditOle* MS_ABI impl__GetIRichEditOle_CRichEditCtrl__QEBAPEAUIRichEditOle__XZ(
    const CRichEditCtrl* pThis);                                                 // core/controls/Thunks.cpp
extern "C" int MS_ABI impl__CanActivate_COleClientItem__MEAAHXZ(COleClientItem* pThis);  // core/ole/Thunks.cpp
extern "C" void MS_ABI impl__OnDeactivateUI_COleClientItem__UEAAXH_Z(
    COleClientItem* pThis, int bUndoable);                                       // core/ole/COleClientItem.cpp
extern "C" void MS_ABI impl__OnActivateUI_COleClientItem__UEAAXXZ(COleClientItem* pThis); // core/ole/COleClientItem.cpp
extern "C" void MS_ABI impl__SetDrawAspect_COleClientItem__UEAAXW4tagDVASPECT___Z(
    COleClientItem* pThis, int nDrawAspect);                                     // core/ole/COleClientItem.cpp
// Same-file thunks called before their definitions.
extern "C" int MS_ABI impl__CanActivate_CRichEditCntrItem__UEAAHXZ(CRichEditCntrItem* pThis);
extern "C" void MS_ABI impl__OnDeactivateUI_CRichEditCntrItem__UEAAXH_Z(
    CRichEditCntrItem* pThis, int bUndoable);

namespace {

// GetDocument()->GetView()->m_lpRichEditOle, in OpenMFC's model: the returned
// interface is AddRef'd (EM_GETOLEINTERFACE) and the caller must Release it.
// Returns NULL when there is no document, no rich-edit view, or no control
// window -- cases in which retail dereferences NULL.
IRichEditOle* ItemRichEditOle(CRichEditCntrItem* pItem) {
    if (pItem == nullptr || pItem->m_pDocument == nullptr) return nullptr;
    CRichEditView* pView = impl__GetView_CRichEditDoc__UEBAPEAVCRichEditView__XZ(
        static_cast<CRichEditDoc*>(pItem->m_pDocument));
    if (pView == nullptr) return nullptr;
    return impl__GetIRichEditOle_CRichEditCtrl__QEBAPEAUIRichEditOle__XZ(&pView->m_richEdit);
}

} // namespace

// CRichEditCntrItem::~CRichEditCntrItem() -- retail RVA 0x287390 (mfc140u):
//     vfptr = CRichEditCntrItem::`vftable';
//     if (m_lpClientSite /*+0xf0*/ != NULL) m_lpClientSite->Release();  // slot 2
//     COleClientItem::~COleClientItem();          // tail jump to 0x244340
// OpenMFC does not cache m_lpClientSite (GetClientSite above hands out a
// fresh reference from IOleObject::GetClientSite each call), so there is
// nothing to Release; the base destructor chain runs through the C++
// destructor defined in this file.  The call is qualified so it does not
// dispatch virtually: ??1 is the non-deleting destructor that a derived
// class's destructor chains to.
// Symbol: ??1CRichEditCntrItem@@UEAA@XZ
extern "C" void MS_ABI impl___1CRichEditCntrItem__UEAA_XZ(CRichEditCntrItem* pThis) {
    if (!pThis) return;
    pThis->CRichEditCntrItem::~CRichEditCntrItem();
}

// BOOL CRichEditCntrItem::ActivateAs(LPCTSTR lpszUserType, REFCLSID clsidOld,
//                                    REFCLSID clsidNew)
// Retail RVA 0x287660 (mfc140u):
//     pView = m_pDocument->GetView();                        // vtable +0x358
//     hr = pView->m_lpRichEditOle->ActivateAs(clsidOld, clsidNew);  // IRichEditOle +0x48
//     return SUCCEEDED(hr);                                  // not eax; shr $31
// lpszUserType is not read.  Deviation: retail has no NULL checks; OpenMFC
// returns FALSE when no rich-edit OLE interface can be reached.
// Symbol: ?ActivateAs@CRichEditCntrItem@@UEAAHPEB_WAEBU_GUID@@1@Z
extern "C" int MS_ABI impl__ActivateAs_CRichEditCntrItem__UEAAHPEB_WAEBU_GUID__1_Z(
    CRichEditCntrItem* pThis, const wchar_t* lpszUserType, const GUID& clsidOld,
    const GUID& clsidNew) {
    (void)lpszUserType;
    IRichEditOle* pRichEditOle = ItemRichEditOle(pThis);
    if (pRichEditOle == nullptr) return FALSE;
    HRESULT hr = pRichEditOle->ActivateAs(clsidOld, clsidNew);
    pRichEditOle->Release();
    return SUCCEEDED(hr) ? TRUE : FALSE;
}

// BOOL CRichEditCntrItem::CanActivate() -- retail RVA 0x2874b0 (mfc140u):
//     COleDocument* pDoc = m_pDocument;                      // +0x40
//     if (pDoc != NULL && pDoc->IsKindOf(RUNTIME_CLASS(COleServerDoc))
//         && pDoc->m_pInPlaceFrame /*+0x250*/ != NULL)
//         return FALSE;       // no in-place editing inside an in-place server
//     return COleClientItem::CanActivate();                  // 0x2485e0, direct call
// (The descriptor at 0x330e10 (mfc140u) names COleServerDoc; 0x234cf0 is
// CObject::IsKindOf.)  Deviation: the +0x250 test is omitted and only the
// base call remains.  It cannot fire in OpenMFC today -- OpenMFC's
// COleServerDoc::ActivateInPlace always returns FALSE and nothing in OpenMFC
// stores a frame at +0x250 -- and reading +0x250 here would be unsafe: the
// usual container, OpenMFC's CRichEditDoc, IsKindOf(COleServerDoc) through
// its CRuntimeClass (featurepack/docking/RuntimeClasses.cpp names
// COleServerDoc as its base) but its C++ class derives from COleDocument and
// is only 0x208 bytes under mingw, so +0x250 lies past the object rather than
// in a zero-filled COleServerDoc word.
// Symbol: ?CanActivate@CRichEditCntrItem@@UEAAHXZ
extern "C" int MS_ABI impl__CanActivate_CRichEditCntrItem__UEAAHXZ(CRichEditCntrItem* pThis) {
    if (!pThis) return FALSE;
    return impl__CanActivate_COleClientItem__MEAAHXZ(pThis);
}

// BOOL CRichEditCntrItem::ConvertTo(REFCLSID clsidNew) -- retail RVA 0x2875a0
// (mfc140u):
//     pRichEditOle = m_pDocument->GetView()->m_lpRichEditOle;   // vtable +0x358, +0x110
//     LPOLESTR lpOleStr;
//     if (FAILED(OleRegGetUserType(clsidNew, USERCLASSTYPE_FULL, &lpOleStr)))
//         return FALSE;
//     CStringA str(lpOleStr);   // 0x1cdad0 = CStringT<char>::CStringT(wchar_t*)
//     hr = pRichEditOle->ConvertObject(REO_IOB_SELECTION, clsidNew, str);  // IRichEditOle +0x40
//     CoTaskMemFree(lpOleStr);
//     return SUCCEEDED(hr);     // ~CStringA follows
// IAT: 0x1802c7a58 = ole32!OleRegGetUserType, 0x1802c7928 = ole32!CoTaskMemFree.
// The CStringA is modelled with WideCharToMultiByte(CP_THREAD_ACP): the
// conversion that ctor reaches (0x1cd9e0, mfc140u) calls WideCharToMultiByte
// with code page 3 = CP_THREAD_ACP.  Deviation: retail has no NULL checks;
// OpenMFC returns FALSE when no rich-edit OLE interface can be reached.
// Symbol: ?ConvertTo@CRichEditCntrItem@@UEAAHAEBU_GUID@@@Z
extern "C" int MS_ABI impl__ConvertTo_CRichEditCntrItem__UEAAHAEBU_GUID___Z(
    CRichEditCntrItem* pThis, const GUID& clsidNew) {
    IRichEditOle* pRichEditOle = ItemRichEditOle(pThis);
    if (pRichEditOle == nullptr) return FALSE;
    LPOLESTR lpOleStr = nullptr;
    if (FAILED(::OleRegGetUserType(clsidNew, USERCLASSTYPE_FULL, &lpOleStr))) {
        pRichEditOle->Release();
        return FALSE;
    }
    char szEmpty[1] = { '\0' };
    char* pszUserType = szEmpty;
    char* pszAlloc = nullptr;
    if (lpOleStr != nullptr) {
        int cb = ::WideCharToMultiByte(CP_THREAD_ACP, 0, lpOleStr, -1, nullptr, 0, nullptr, nullptr);
        if (cb > 0) {
            pszAlloc = static_cast<char*>(std::malloc(static_cast<size_t>(cb)));
            if (pszAlloc != nullptr &&
                ::WideCharToMultiByte(CP_THREAD_ACP, 0, lpOleStr, -1, pszAlloc, cb, nullptr, nullptr) > 0)
                pszUserType = pszAlloc;
        }
    }
    HRESULT hr = pRichEditOle->ConvertObject(REO_IOB_SELECTION, clsidNew, pszUserType);
    ::CoTaskMemFree(lpOleStr);
    std::free(pszAlloc);
    pRichEditOle->Release();
    return SUCCEEDED(hr) ? TRUE : FALSE;
}

// static CObject* CRichEditCntrItem::CreateObject() -- retail RVA 0x2877c0
// (mfc140u), from IMPLEMENT_SERIAL:
//     void* p = operator new(0x100);
//     return p ? new(p) CRichEditCntrItem(NULL, NULL) : NULL;
// OpenMFC's constructor takes only the container document (the REOBJECT*
// argument is not modelled, see the ctor thunk above), so NULL is passed for
// it.  Deviation: OpenMFC's operator new throws instead of returning NULL.
// Symbol: ?CreateObject@CRichEditCntrItem@@SAPEAVCObject@@XZ
extern "C" CObject* MS_ABI impl__CreateObject_CRichEditCntrItem__SAPEAVCObject__XZ() {
    return new CRichEditCntrItem(nullptr);
}

// BOOL CRichEditCntrItem::OnChangeItemPosition(const CRect&) -- export ordinal
// 8780 resolves to RVA 0x71e0 (mfc140u), a COMDAT-folded body shared by many
// exports whose whole text is `xor eax,eax; ret`.  (Cross-check, mfc140u:
// slot 42 of the COleClientItem vftable (0x32be58) is
// ?OnChangeItemPosition@COleClientItem (0x248e60), and slot 42 of the
// CRichEditCntrItem vftable (0x335818) is that same folded 0x71e0 body.)  The rich-edit
// control positions its own objects, so retail simply returns FALSE.
// Symbol: ?OnChangeItemPosition@CRichEditCntrItem@@MEAAHAEBVCRect@@@Z
extern "C" int MS_ABI impl__OnChangeItemPosition_CRichEditCntrItem__MEAAHAEBVCRect___Z(
    CRichEditCntrItem* pThis, const CRect& rectPos) {
    (void)pThis;
    (void)rectPos;
    return FALSE;
}

// void CRichEditCntrItem::OnDeactivateUI(BOOL bUndoable) -- retail RVA
// 0x2873d0 (mfc140u):
//     if (m_pView /*+0x98*/ != NULL) {
//         CFrameWnd* pFrame = m_pView->GetParentFrame();      // 0x28e200
//         if (pFrame == NULL) AfxThrowInvalidArgException();  // 0x227720
//         ::SendMessage(pFrame->m_hWnd, WM_SETMESSAGESTRING /*0x362*/,
//                       AFX_IDS_IDLEMESSAGE /*0xE001*/, 0);   // IAT 0x1802c7120 = SendMessageW
//     }
//     COleClientItem::OnDeactivateUI(bUndoable);              // tail jump to 0x248ab0
// Deviation: OpenMFC's COleClientItem has no m_pView member.  One retail
// writer of it is CRichEditView::ShowContainerUI (RVA 0x285dc0, mfc140u),
// which stores the view at item+0x98 before calling ShowContainerUI below
// when its argument is TRUE, and clears it afterwards when the argument is
// FALSE or the call failed; OpenMFC's version of that function is a stub.  With nothing to read,
// an OpenMFC item behaves as retail does when m_pView == NULL and only the
// base call is made, so the frame's status bar is not reset to the idle
// message.
// Symbol: ?OnDeactivateUI@CRichEditCntrItem@@UEAAXH@Z
extern "C" void MS_ABI impl__OnDeactivateUI_CRichEditCntrItem__UEAAXH_Z(
    CRichEditCntrItem* pThis, int bUndoable) {
    if (!pThis) return;
    impl__OnDeactivateUI_COleClientItem__UEAAXH_Z(pThis, bUndoable);
}

// void CRichEditCntrItem::SetDrawAspect(DVASPECT nDrawAspect) -- retail RVA
// 0x2876b0 (mfc140u):
//     m_pDocument->GetView()->m_lpRichEditOle
//         ->SetDvaspect(REO_IOB_SELECTION, nDrawAspect);     // IRichEditOle +0x60
//     COleClientItem::SetDrawAspect(nDrawAspect);            // tail jump to 0x246c20
// Deviation: retail has no NULL checks; OpenMFC skips the IRichEditOle call
// when no interface can be reached but still runs the base implementation.
// Symbol: ?SetDrawAspect@CRichEditCntrItem@@UEAAXW4tagDVASPECT@@@Z
extern "C" void MS_ABI impl__SetDrawAspect_CRichEditCntrItem__UEAAXW4tagDVASPECT___Z(
    CRichEditCntrItem* pThis, int nDrawAspect) {
    if (!pThis) return;
    IRichEditOle* pRichEditOle = ItemRichEditOle(pThis);
    if (pRichEditOle != nullptr) {
        pRichEditOle->SetDvaspect(REO_IOB_SELECTION, static_cast<DWORD>(nDrawAspect));
        pRichEditOle->Release();
    }
    impl__SetDrawAspect_COleClientItem__UEAAXW4tagDVASPECT___Z(pThis, nDrawAspect);
}

// HRESULT CRichEditCntrItem::ShowContainerUI(BOOL b) -- retail RVA 0x287430
// (mfc140u, ordinal 13804).  Every call is virtual through the item's vtable:
//     if (!CanActivate())                     // +0x128, slot 37
//         return E_NOTIMPL;
//     if (b) { OnDeactivateUI(FALSE);         // +0x110, slot 34
//              OnDeactivate(); }              // +0x140, slot 40
//     else   { OnActivate();                  // +0x130, slot 38
//              OnActivateUI(); }              // +0x138, slot 39
//     return S_OK;
// (Slots named from the mfc140u CRichEditCntrItem vftable at 0x335818: 34 is
// this class's OnDeactivateUI (0x2873d0), 37 its CanActivate (0x2874b0), and
// 38/39/40 are COleClientItem's OnActivate/OnActivateUI/OnDeactivate.)
// Deviation: OpenMFC's header declares only OnActivate and OnDeactivate as
// virtual, so those two dispatch through OpenMFC's own vtable; CanActivate,
// OnDeactivateUI and OnActivateUI are called statically through their
// thunks, so an override of those three in a further-derived class is not
// honoured.
// Symbol: ?ShowContainerUI@CRichEditCntrItem@@QEAAJH@Z
extern "C" long MS_ABI impl__ShowContainerUI_CRichEditCntrItem__QEAAJH_Z(
    CRichEditCntrItem* pThis, int b) {
    if (!pThis) return E_NOTIMPL;
    if (!impl__CanActivate_CRichEditCntrItem__UEAAHXZ(pThis))
        return E_NOTIMPL;
    if (b) {
        impl__OnDeactivateUI_CRichEditCntrItem__UEAAXH_Z(pThis, FALSE);
        pThis->OnDeactivate();
    } else {
        pThis->OnActivate();
        impl__OnActivateUI_COleClientItem__UEAAXXZ(pThis);
    }
    return S_OK;
}

// void CRichEditCntrItem::SyncToRichEditObject(REOBJECT& reo) -- retail RVA
// 0x287710 (mfc140u):
//     COleClientItem::SetDrawAspect((DVASPECT)reo.dvaspect);  // mov 0x38(%rdx),%edx;
//                                                             // jmp 0x246c20
// i.e. a direct (non-virtual) call to the BASE SetDrawAspect, not to this
// class's override.  REOBJECT::dvaspect is at +0x38 in the x64 SDK layout,
// which the static_assert below pins.
static_assert(offsetof(REOBJECT, dvaspect) == 0x38, "REOBJECT::dvaspect must be at +0x38");
// Symbol: ?SyncToRichEditObject@CRichEditCntrItem@@QEAAXAEAU_reobject@@@Z
extern "C" void MS_ABI impl__SyncToRichEditObject_CRichEditCntrItem__QEAAXAEAU_reobject___Z(
    CRichEditCntrItem* pThis, REOBJECT& reo) {
    if (!pThis) return;
    impl__SetDrawAspect_COleClientItem__UEAAXW4tagDVASPECT___Z(pThis, static_cast<int>(reo.dvaspect));
}
