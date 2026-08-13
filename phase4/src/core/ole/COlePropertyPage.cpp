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
// Symbol: ?CleanupObjectArray@COlePropertyPage@@IEAAXXZ
extern "C" void MS_ABI impl__CleanupObjectArray_COlePropertyPage__IEAAXXZ() {}

// Symbol: ?EnumChildProc@COlePropertyPage@@KAHPEAUHWND__@@_J@Z
extern "C" int MS_ABI impl__EnumChildProc_COlePropertyPage__KAHPEAUHWND_____J_Z(void* /*struct*/* p0, __int64 p1) {
    return 0;
}

// Symbol: ?EnumControls@COlePropertyPage@@KAHPEAUHWND__@@_J@Z
extern "C" int MS_ABI impl__EnumControls_COlePropertyPage__KAHPEAUHWND_____J_Z(void* /*struct*/* p0, __int64 p1) {
    return 0;
}

// Symbol: ?GetControlStatus@COlePropertyPage@@QEAAHI@Z
extern "C" int MS_ABI impl__GetControlStatus_COlePropertyPage__QEAAHI_Z(unsigned int p0) {
    return 0;
}

// Symbol: ?GetObjectArray@COlePropertyPage@@QEAAPEAPEAUIDispatch@@PEAK@Z
extern "C" void* MS_ABI impl__GetObjectArray_COlePropertyPage__QEAAPEAPEAUIDispatch__PEAK_Z(unsigned long* p0) {
    return nullptr;
}

// Symbol: ?GetPageSite@COlePropertyPage@@QEAAPEAUIPropertyPageSite@@XZ
extern "C" void* MS_ABI impl__GetPageSite_COlePropertyPage__QEAAPEAUIPropertyPageSite__XZ() {
    return nullptr;
}

// Symbol: ?GetPropCheck@COlePropertyPage@@QEAAHPEB_WPEAH@Z
extern "C" int MS_ABI impl__GetPropCheck_COlePropertyPage__QEAAHPEB_WPEAH_Z(const wchar_t* p0, int* p1) {
    return 0;
}

// Symbol: ?GetPropIndex@COlePropertyPage@@QEAAHPEB_WPEAH@Z
extern "C" int MS_ABI impl__GetPropIndex_COlePropertyPage__QEAAHPEB_WPEAH_Z(const wchar_t* p0, int* p1) {
    return 0;
}

// Symbol: ?GetPropRadio@COlePropertyPage@@QEAAHPEB_WPEAH@Z
extern "C" int MS_ABI impl__GetPropRadio_COlePropertyPage__QEAAHPEB_WPEAH_Z(const wchar_t* p0, int* p1) {
    return 0;
}

// Symbol: ?GetPropText@COlePropertyPage@@QEAAHPEB_WPEAE@Z
extern "C" int MS_ABI impl__GetPropText_COlePropertyPage__QEAAHPEB_WPEAE_Z(const wchar_t* p0, unsigned char* p1) {
    return 0;
}

// Symbol: ?GetPropText@COlePropertyPage@@QEAAHPEB_WPEAF@Z
extern "C" int MS_ABI impl__GetPropText_COlePropertyPage__QEAAHPEB_WPEAF_Z(const wchar_t* p0, short* p1) {
    return 0;
}

// Symbol: ?GetPropText@COlePropertyPage@@QEAAHPEB_WPEAH@Z
extern "C" int MS_ABI impl__GetPropText_COlePropertyPage__QEAAHPEB_WPEAH_Z(const wchar_t* p0, int* p1) {
    return 0;
}

// Symbol: ?GetPropText@COlePropertyPage@@QEAAHPEB_WPEAI@Z
extern "C" int MS_ABI impl__GetPropText_COlePropertyPage__QEAAHPEB_WPEAI_Z(const wchar_t* p0, unsigned int* p1) {
    return 0;
}

// Symbol: ?GetPropText@COlePropertyPage@@QEAAHPEB_WPEAJ@Z
extern "C" int MS_ABI impl__GetPropText_COlePropertyPage__QEAAHPEB_WPEAJ_Z(const wchar_t* p0, long* p1) {
    return 0;
}

// Symbol: ?GetPropText@COlePropertyPage@@QEAAHPEB_WPEAK@Z
extern "C" int MS_ABI impl__GetPropText_COlePropertyPage__QEAAHPEB_WPEAK_Z(const wchar_t* p0, unsigned long* p1) {
    return 0;
}

// Symbol: ?GetPropText@COlePropertyPage@@QEAAHPEB_WPEAM@Z
extern "C" int MS_ABI impl__GetPropText_COlePropertyPage__QEAAHPEB_WPEAM_Z(const wchar_t* p0, float* p1) {
    return 0;
}

// Symbol: ?GetPropText@COlePropertyPage@@QEAAHPEB_WPEAN@Z
extern "C" int MS_ABI impl__GetPropText_COlePropertyPage__QEAAHPEB_WPEAN_Z(const wchar_t* p0, double* p1) {
    return 0;
}

// Symbol: ?GetPropText@COlePropertyPage@@QEAAHPEB_WPEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@@Z
extern "C" int MS_ABI impl__GetPropText_COlePropertyPage__QEAAHPEB_WPEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(const wchar_t* p0, void* /*class*/* p1, void** p2, void* p3) {
    return 0;
}

// Symbol: ?IgnoreApply@COlePropertyPage@@QEAAXI@Z
extern "C" void MS_ABI impl__IgnoreApply_COlePropertyPage__QEAAXI_Z(unsigned int p0) {}

// Symbol: ?IsModified@COlePropertyPage@@QEAAHXZ
extern "C" int MS_ABI impl__IsModified_COlePropertyPage__QEAAHXZ() {
    return 0;
}

// Symbol: ?MessageBoxW@COlePropertyPage@@QEAAHPEB_W0I@Z
extern "C" int MS_ABI impl__MessageBoxW_COlePropertyPage__QEAAHPEB_W0I_Z(const wchar_t* p0, const wchar_t* p1, unsigned int p2) {
    return 0;
}

// Symbol: ?OnCommand@COlePropertyPage@@MEAAH_K_J@Z
extern "C" int MS_ABI impl__OnCommand_COlePropertyPage__MEAAH_K_J_Z(unsigned __int64 p0, __int64 p1) {
    return 0;
}

// Symbol: ?OnCtlColor@COlePropertyPage@@IEAAPEAUHBRUSH__@@PEAVCDC@@PEAVCWnd@@I@Z
extern "C" void* MS_ABI impl__OnCtlColor_COlePropertyPage__IEAAPEAUHBRUSH____PEAVCDC__PEAVCWnd__I_Z(void* /*class*/* p0, void* /*class*/* p1, unsigned int p2) {
    return nullptr;
}

// Symbol: ?OnFinalRelease@COlePropertyPage@@MEAAXXZ
extern "C" void MS_ABI impl__OnFinalRelease_COlePropertyPage__MEAAXXZ() {}

// Symbol: ?OnHelp@COlePropertyPage@@UEAAHPEB_W@Z
extern "C" int MS_ABI impl__OnHelp_COlePropertyPage__UEAAHPEB_W_Z(const wchar_t* p0) {
    return 0;
}

// Symbol: ?OnInitDialog@COlePropertyPage@@UEAAHXZ
extern "C" int MS_ABI impl__OnInitDialog_COlePropertyPage__UEAAHXZ() {
    return 0;
}

// Symbol: ?PreTranslateMessage@COlePropertyPage@@MEAAHPEAUtagMSG@@@Z
extern "C" int MS_ABI impl__PreTranslateMessage_COlePropertyPage__MEAAHPEAUtagMSG___Z(void* /*struct*/* p0) {
    return 0;
}

// Symbol: ?SetControlStatus@COlePropertyPage@@QEAAHIH@Z
extern "C" int MS_ABI impl__SetControlStatus_COlePropertyPage__QEAAHIH_Z(unsigned int p0, int p1) {
    return 0;
}

// Symbol: ?SetDialogResource@COlePropertyPage@@QEAAXPEAX@Z
extern "C" void MS_ABI impl__SetDialogResource_COlePropertyPage__QEAAXPEAX_Z(void* p0) {}

// Symbol: ?SetPropCheck@COlePropertyPage@@QEAAHPEB_WH@Z
extern "C" int MS_ABI impl__SetPropCheck_COlePropertyPage__QEAAHPEB_WH_Z(const wchar_t* p0, int p1) {
    return 0;
}

// Symbol: ?SetPropIndex@COlePropertyPage@@QEAAHPEB_WH@Z
extern "C" int MS_ABI impl__SetPropIndex_COlePropertyPage__QEAAHPEB_WH_Z(const wchar_t* p0, int p1) {
    return 0;
}

// Symbol: ?SetPropRadio@COlePropertyPage@@QEAAHPEB_WH@Z
extern "C" int MS_ABI impl__SetPropRadio_COlePropertyPage__QEAAHPEB_WH_Z(const wchar_t* p0, int p1) {
    return 0;
}

// Symbol: ?SetPropText@COlePropertyPage@@QEAAHPEB_WAEAE@Z
extern "C" int MS_ABI impl__SetPropText_COlePropertyPage__QEAAHPEB_WAEAE_Z(const wchar_t* p0, unsigned char* p1) {
    return 0;
}

// Symbol: ?SetPropText@COlePropertyPage@@QEAAHPEB_WAEAF@Z
extern "C" int MS_ABI impl__SetPropText_COlePropertyPage__QEAAHPEB_WAEAF_Z(const wchar_t* p0, short* p1) {
    return 0;
}

// Symbol: ?SetPropText@COlePropertyPage@@QEAAHPEB_WAEAH@Z
extern "C" int MS_ABI impl__SetPropText_COlePropertyPage__QEAAHPEB_WAEAH_Z(const wchar_t* p0, int* p1) {
    return 0;
}

// Symbol: ?SetPropText@COlePropertyPage@@QEAAHPEB_WAEAI@Z
extern "C" int MS_ABI impl__SetPropText_COlePropertyPage__QEAAHPEB_WAEAI_Z(const wchar_t* p0, unsigned int* p1) {
    return 0;
}

// Symbol: ?SetPropText@COlePropertyPage@@QEAAHPEB_WAEAJ@Z
extern "C" int MS_ABI impl__SetPropText_COlePropertyPage__QEAAHPEB_WAEAJ_Z(const wchar_t* p0, long* p1) {
    return 0;
}

// Symbol: ?SetPropText@COlePropertyPage@@QEAAHPEB_WAEAK@Z
extern "C" int MS_ABI impl__SetPropText_COlePropertyPage__QEAAHPEB_WAEAK_Z(const wchar_t* p0, unsigned long* p1) {
    return 0;
}

// Symbol: ?SetPropText@COlePropertyPage@@QEAAHPEB_WAEAM@Z
extern "C" int MS_ABI impl__SetPropText_COlePropertyPage__QEAAHPEB_WAEAM_Z(const wchar_t* p0, float* p1) {
    return 0;
}

// Symbol: ?SetPropText@COlePropertyPage@@QEAAHPEB_WAEAN@Z
extern "C" int MS_ABI impl__SetPropText_COlePropertyPage__QEAAHPEB_WAEAN_Z(const wchar_t* p0, double* p1) {
    return 0;
}

// Symbol: ?SetPropText@COlePropertyPage@@QEAAHPEB_WAEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@@Z
extern "C" int MS_ABI impl__SetPropText_COlePropertyPage__QEAAHPEB_WAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(const wchar_t* p0, void* /*class*/* p1, void** p2, void* p3) {
    return 0;
}

// Symbol: ?WindowProc@COlePropertyPage@@MEAA_JI_K_J@Z
extern "C" __int64 MS_ABI impl__WindowProc_COlePropertyPage__MEAA_JI_K_J_Z(unsigned int p0, unsigned __int64 p1, __int64 p2) {
    return 0;
}

// Symbol: ?Activate@XPropertyPage@COlePropertyPage@@UEAAJPEAUHWND__@@PEBUtagRECT@@H@Z
extern "C" long MS_ABI impl__Activate_XPropertyPage_COlePropertyPage__UEAAJPEAUHWND____PEBUtagRECT__H_Z(void* /*struct*/* p0, const void* /*struct*/* p1, int p2) {
    return 0;
}

// Symbol: ?Apply@XPropertyPage@COlePropertyPage@@UEAAJXZ
extern "C" long MS_ABI impl__Apply_XPropertyPage_COlePropertyPage__UEAAJXZ() {
    return 0;
}

// Symbol: ?Deactivate@XPropertyPage@COlePropertyPage@@UEAAJXZ
extern "C" long MS_ABI impl__Deactivate_XPropertyPage_COlePropertyPage__UEAAJXZ() {
    return 0;
}

// Symbol: ?EditProperty@XPropertyPage@COlePropertyPage@@UEAAJJ@Z
extern "C" long MS_ABI impl__EditProperty_XPropertyPage_COlePropertyPage__UEAAJJ_Z(long p0) {
    return 0;
}

// Symbol: ?GetPageInfo@XPropertyPage@COlePropertyPage@@UEAAJPEAUtagPROPPAGEINFO@@@Z
extern "C" long MS_ABI impl__GetPageInfo_XPropertyPage_COlePropertyPage__UEAAJPEAUtagPROPPAGEINFO___Z(void* /*struct*/* p0) {
    return 0;
}

// Symbol: ?Help@XPropertyPage@COlePropertyPage@@UEAAJPEB_W@Z
extern "C" long MS_ABI impl__Help_XPropertyPage_COlePropertyPage__UEAAJPEB_W_Z(const wchar_t* p0) {
    return 0;
}

// Symbol: ?IsPageDirty@XPropertyPage@COlePropertyPage@@UEAAJXZ
extern "C" long MS_ABI impl__IsPageDirty_XPropertyPage_COlePropertyPage__UEAAJXZ() {
    return 0;
}

// Symbol: ?Move@XPropertyPage@COlePropertyPage@@UEAAJPEBUtagRECT@@@Z
extern "C" long MS_ABI impl__Move_XPropertyPage_COlePropertyPage__UEAAJPEBUtagRECT___Z(const void* /*struct*/* p0) {
    return 0;
}

// Symbol: ?OnChanged@XPropNotifySink@COlePropertyPage@@UEAAJJ@Z
extern "C" long MS_ABI impl__OnChanged_XPropNotifySink_COlePropertyPage__UEAAJJ_Z(long p0) {
    return 0;
}

// Symbol: ?OnRequestEdit@XPropNotifySink@COlePropertyPage@@UEAAJJ@Z
extern "C" long MS_ABI impl__OnRequestEdit_XPropNotifySink_COlePropertyPage__UEAAJJ_Z(long p0) {
    return 0;
}

// Symbol: ?SetObjects@XPropertyPage@COlePropertyPage@@UEAAJKPEAPEAUIUnknown@@@Z
extern "C" long MS_ABI impl__SetObjects_XPropertyPage_COlePropertyPage__UEAAJKPEAPEAUIUnknown___Z(unsigned long p0, void* /*struct*/** p1) {
    return 0;
}

// Symbol: ?SetPageSite@XPropertyPage@COlePropertyPage@@UEAAJPEAUIPropertyPageSite@@@Z
extern "C" long MS_ABI impl__SetPageSite_XPropertyPage_COlePropertyPage__UEAAJPEAUIPropertyPageSite___Z(void* /*struct*/* p0) {
    return 0;
}

// Symbol: ?Show@XPropertyPage@COlePropertyPage@@UEAAJI@Z
extern "C" long MS_ABI impl__Show_XPropertyPage_COlePropertyPage__UEAAJI_Z(unsigned int p0) {
    return 0;
}

// Symbol: ?TranslateAcceleratorW@XPropertyPage@COlePropertyPage@@UEAAJPEAUtagMSG@@@Z
extern "C" long MS_ABI impl__TranslateAcceleratorW_XPropertyPage_COlePropertyPage__UEAAJPEAUtagMSG___Z(void* /*struct*/* p0) {
    return 0;
}
