// CDocItem — OpenMFC implementation.
// Sources: olecore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/OlecoreSupport.h"

// Symbol: ?CreateObject@CDocItem@@SAPEAVCObject@@XZ
extern "C" CObject* MS_ABI impl__CreateObject_CDocItem__SAPEAVCObject__XZ() {
    return new CDocItem();
}
// Symbol: ?IsBlank@CDocItem@@UEBAHXZ
extern "C" int MS_ABI impl__IsBlank_CDocItem__UEBAHXZ(const CDocItem* pThis) {
    return pThis == nullptr || pThis->m_pDocument == nullptr;
}
CDocItem::CDocItem() : m_pDocument(nullptr) { memset(_docitem_padding, 0, sizeof(_docitem_padding)); }
CDocItem::~CDocItem() {}
