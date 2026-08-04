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
