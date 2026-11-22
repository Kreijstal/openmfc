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
static_assert(sizeof(CDocItem) == 0x48 && offsetof(CDocItem, m_pDocument) == 0x40);
CDocItem::CDocItem() : m_pDocument(nullptr) {}
CDocItem::~CDocItem() {}
