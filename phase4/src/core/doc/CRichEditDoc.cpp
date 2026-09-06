// CRichEditDoc — OpenMFC implementation.
// Sources: ole_olectors_exports.cpp, richedit_ole_ext.cpp

#include "detail/CRichEditDocSupport.h"

#include "openmfc/afxole.h"
#include "openmfc/afxmfc.h"

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

























// Symbol: ??0CRichEditDoc@@IEAA@XZ
// CRichEditDoc::CRichEditDoc()
extern "C" void* MS_ABI impl___0CRichEditDoc__IEAA_XZ() {
    return new CRichEditDoc();
}
// Symbol: ?GetInPlaceActiveItem@CRichEditDoc@@UEAAPEAVCOleClientItem@@PEAVCWnd@@@Z
extern "C" void* MS_ABI impl__GetInPlaceActiveItem_CRichEditDoc__UEAAPEAVCOleClientItem__PEAVCWnd___Z(CRichEditDoc* pThis, CWnd* pWnd) {
    return pThis ? pThis->GetInPlaceActiveItem(pWnd) : nullptr;
}
// Symbol: ?GetPrimarySelectedItem@CRichEditDoc@@UEAAPEAVCOleClientItem@@PEAVCView@@@Z
extern "C" void* MS_ABI impl__GetPrimarySelectedItem_CRichEditDoc__UEAAPEAVCOleClientItem__PEAVCView___Z(CRichEditDoc* pThis, CView* pView) {
    return pThis ? pThis->GetPrimarySelectedItem(pView) : nullptr;
}
// Symbol: ?LookupItem@CRichEditDoc@@QEBAPEAVCRichEditCntrItem@@PEAUIOleObject@@@Z
extern "C" void* MS_ABI impl__LookupItem_CRichEditDoc__QEBAPEAVCRichEditCntrItem__PEAUIOleObject___Z(CRichEditDoc* pThis, IOleObject* pOleObject) {
    return pThis ? pThis->LookupItem(pOleObject) : nullptr;
}
// Symbol: ?OnGetEmbeddedItem@CRichEditDoc@@MEAAPEAVCOleServerItem@@XZ
extern "C" COleServerItem* MS_ABI impl__OnGetEmbeddedItem_CRichEditDoc__MEAAPEAVCOleServerItem__XZ(CRichEditDoc* pThis) {
    COleClientItem* item = pThis ? pThis->GetPrimarySelectedItem(nullptr) : nullptr;
    return dynamic_cast<COleServerItem*>(item);
}
CRichEditDoc::CRichEditDoc() : COleDocument() {
    memset(_richeditdoc_padding, 0, sizeof(_richeditdoc_padding));
}
CRichEditDoc::~CRichEditDoc() {
}
COleClientItem* CRichEditDoc::GetInPlaceActiveItem(CWnd* pWnd) {
    (void)pWnd;
    POSITION pos = reinterpret_cast<POSITION>(GetStartPosition());
    while (COleClientItem* item = GetNextClientItem(pos)) {
        if (item && item->IsInPlaceActive()) {
            return item;
        }
    }
    return nullptr;
}
COleClientItem* CRichEditDoc::GetPrimarySelectedItem(CView* pView) {
    (void)pView;
    COleClientItem* active = GetInPlaceActiveItem(nullptr);
    if (active) {
        return active;
    }

    POSITION pos = reinterpret_cast<POSITION>(GetStartPosition());
    return GetNextClientItem(pos);
}
CRichEditCntrItem* CRichEditDoc::LookupItem(IOleObject* pOleObject) const {
    POSITION pos = reinterpret_cast<POSITION>(GetStartPosition());
    while (COleClientItem* item = GetNextClientItem(pos)) {
        if (item && item->GetObject() == pOleObject) {
            return static_cast<CRichEditCntrItem*>(item);
        }
    }
    return nullptr;
}
// Symbol: ?CreateClientItem@CRichEditDoc@@UEBAPEAVCRichEditCntrItem@@PEAU_reobject@@@Z
extern "C" void* MS_ABI impl__CreateClientItem_CRichEditDoc__UEBAPEAVCRichEditCntrItem__PEAU_reobject___Z(void* /*struct*/* p0) {
    return nullptr;
}

// Symbol: ?DeleteContents@CRichEditDoc@@UEAAXXZ
extern "C" void MS_ABI impl__DeleteContents_CRichEditDoc__UEAAXXZ() {}

// Symbol: ?DeleteUnmarkedItems@CRichEditDoc@@IEBAXXZ
extern "C" void MS_ABI impl__DeleteUnmarkedItems_CRichEditDoc__IEBAXXZ() {}

// Symbol: ?GetStartPosition@CRichEditDoc@@UEBAPEAU__POSITION@@XZ
extern "C" void* MS_ABI impl__GetStartPosition_CRichEditDoc__UEBAPEAU__POSITION__XZ() {
    return nullptr;
}

// Symbol: ?GetView@CRichEditDoc@@UEBAPEAVCRichEditView@@XZ
extern "C" void* MS_ABI impl__GetView_CRichEditDoc__UEBAPEAVCRichEditView__XZ() {
    return nullptr;
}

// Symbol: ?IsModified@CRichEditDoc@@UEAAHXZ
extern "C" int MS_ABI impl__IsModified_CRichEditDoc__UEAAHXZ() {
    return 0;
}

// Symbol: ?MarkItemsClear@CRichEditDoc@@IEBAXXZ
extern "C" void MS_ABI impl__MarkItemsClear_CRichEditDoc__IEBAXXZ() {}

// Symbol: ?PreCloseFrame@CRichEditDoc@@UEAAXPEAVCFrameWnd@@@Z
extern "C" void MS_ABI impl__PreCloseFrame_CRichEditDoc__UEAAXPEAVCFrameWnd___Z(void* /*class*/* p0) {}

// Symbol: ?Serialize@CRichEditDoc@@UEAAXAEAVCArchive@@@Z
extern "C" void MS_ABI impl__Serialize_CRichEditDoc__UEAAXAEAVCArchive___Z(void* /*class*/* p0) {}

// Symbol: ?SetModifiedFlag@CRichEditDoc@@UEAAXH@Z
extern "C" void MS_ABI impl__SetModifiedFlag_CRichEditDoc__UEAAXH_Z(int p0) {}

// Symbol: ?SetPathName@CRichEditDoc@@UEAAXPEB_WH@Z
extern "C" void MS_ABI impl__SetPathName_CRichEditDoc__UEAAXPEB_WH_Z(const wchar_t* p0, int p1) {}

// Symbol: ?SetTitle@CRichEditDoc@@UEAAXPEB_W@Z
extern "C" void MS_ABI impl__SetTitle_CRichEditDoc__UEAAXPEB_W_Z(const wchar_t* p0) {}

// Symbol: ?UpdateModifiedFlag@CRichEditDoc@@UEAAXXZ
extern "C" void MS_ABI impl__UpdateModifiedFlag_CRichEditDoc__UEAAXXZ() {}

// Symbol: ?UpdateObjectCache@CRichEditDoc@@IEAAXXZ
extern "C" void MS_ABI impl__UpdateObjectCache_CRichEditDoc__IEAAXXZ() {}

