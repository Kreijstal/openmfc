// COleLinkingDoc — OpenMFC implementation.
// Sources: olecore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/OlecoreSupport.h"

COleLinkingDoc::COleLinkingDoc()
    : COleDocument(), m_bRegistered(FALSE), m_lpMoniker(nullptr) {
    memset(_colelinkingdoc_padding, 0, sizeof(_colelinkingdoc_padding));
}
COleLinkingDoc::~COleLinkingDoc() {
    Revoke();
}
LPMONIKER COleLinkingDoc::GetMoniker(OLEGETMONIKER nAssign) {
    if (!m_lpMoniker && nAssign != OLEGETMONIKER_ONLYIFTHERE) {
        EnsureLinkingDocMoniker(this, nullptr, FALSE);
    }
    return m_lpMoniker;
}
LPMONIKER COleLinkingDoc::GetFileMoniker() {
    return m_lpMoniker;
}
BOOL COleLinkingDoc::RegisterIfServerAttached(const wchar_t* lpszFileName, BOOL bSetModified) {
    return EnsureLinkingDocMoniker(this, lpszFileName, bSetModified);
}
void COleLinkingDoc::Revoke() {
    if (m_lpMoniker) {
        m_lpMoniker->Release();
        m_lpMoniker = nullptr;
    }
    m_bRegistered = FALSE;
}
BOOL COleLinkingDoc::IsRegistered() const {
    return m_bRegistered;
}
void COleLinkingDoc::OnShowDocument(BOOL bShow) {
    if (!bShow) Revoke();
}
