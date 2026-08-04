// COlePropertyPage — OpenMFC implementation.
// Sources: olecore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/OlecoreSupport.h"

COlePropertyPage::COlePropertyPage()
    : CDialog(), m_pPageSite(nullptr), m_bModified(FALSE),
      m_pszPageName(nullptr), m_pszHelpFile(nullptr),
      m_dwHelpContext(0), m_pszDocString(nullptr) {
    memset(_olepropertypage_padding, 0, sizeof(_olepropertypage_padding));
}
COlePropertyPage::COlePropertyPage(UINT nIDTemplate, UINT nIDCaption)
    : CDialog(nIDTemplate), m_pPageSite(nullptr), m_bModified(FALSE),
      m_pszPageName(nullptr), m_pszHelpFile(nullptr),
      m_dwHelpContext(0), m_pszDocString(nullptr) {
    memset(_olepropertypage_padding, 0, sizeof(_olepropertypage_padding));
}
COlePropertyPage::~COlePropertyPage() {
}
void COlePropertyPage::SetPageName(const wchar_t* pszName) {
    m_pszPageName = (wchar_t*)pszName;
}
void COlePropertyPage::SetHelpInfo(const wchar_t* lpszDocString, const wchar_t* lpszHelpFile, DWORD dwHelpContext) {
    m_pszDocString = (wchar_t*)lpszDocString;
    m_pszHelpFile = (wchar_t*)lpszHelpFile;
    m_dwHelpContext = dwHelpContext;
}
BOOL COlePropertyPage::OnSetPageSite() {
    return TRUE;
}
void COlePropertyPage::OnObjectsChanged() {
    SetModifiedFlag(TRUE);
}
BOOL COlePropertyPage::OnApply() {
    return TRUE;
}
BOOL COlePropertyPage::OnEditProperty(DISPID dispid) {
    (void)dispid;
    return FALSE;
}
void COlePropertyPage::SetModifiedFlag(BOOL bModified) {
    m_bModified = bModified ? TRUE : FALSE;
    if (m_pPageSite) {
        m_pPageSite->OnStatusChange(bModified ? PROPPAGESTATUS_DIRTY : 0);
    }
}
