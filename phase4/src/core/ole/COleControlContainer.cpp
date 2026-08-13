// COleControlContainer — OpenMFC implementation.
// Sources: ole_olectors_exports.cpp, olecore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/OlecoreSupport.h"

#include "openmfc/afxole.h"
#include "openmfc/afxmfc.h"

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

























// Symbol: ??0COleControlContainer@@QEAA@PEAVCWnd@@@Z
// COleControlContainer::COleControlContainer(CWnd* pWnd)
extern "C" void* MS_ABI impl___0COleControlContainer__QEAA_PEAVCWnd___Z(void* p0) {
    return new COleControlContainer(static_cast<CWnd*>(p0));
}
COleControlContainer::COleControlContainer(CWnd* pWnd)
    : m_pWnd(pWnd) {
    memset(_olecontrolcontainer_padding, 0, sizeof(_olecontrolcontainer_padding));
}
COleControlContainer::~COleControlContainer() {
    // Cleanup sites without using POSITION
    while (!m_listSites.IsEmpty()) {
        COleControlSite* pSite = (COleControlSite*)m_listSites.RemoveHead();
        delete pSite;
    }
}
COleControlSite* COleControlContainer::CreateSite(COleControlContainer* pCtrlCont) {
    COleControlSite* pSite = new COleControlSite(pCtrlCont);
    m_listSites.AddTail(pSite);
    return pSite;
}
BOOL COleControlContainer::DeleteSite(COleControlSite* pSite) {
    // Look for pSite in the list and remove it
    CPtrList::POSITION pos = m_listSites.Find(pSite);
    if (pos != CPtrList::POSITION(nullptr)) {
        m_listSites.RemoveAt(pos);
        delete pSite;
        return TRUE;
    }
    return FALSE;
}
COleControlSite* COleControlContainer::FindItem(UINT nID) const {
    CPtrList::POSITION pos = m_listSites.GetHeadPosition();
    while (pos != CPtrList::POSITION(nullptr)) {
        COleControlSite* pSite = static_cast<COleControlSite*>(m_listSites.GetNext(pos));
        if (!pSite) {
            continue;
        }

        UINT siteId = 0;
        if (TryGetControlSiteId(pSite, &siteId) && siteId == nID) {
            return pSite;
        }

        HWND hWnd = pSite->m_hWnd;
        if (hWnd && ::IsWindow(hWnd) && static_cast<UINT>(::GetDlgCtrlID(hWnd)) == nID) {
            return pSite;
        }
    }
    return nullptr;
}
// Symbol: ?AttachControlSite@COleControlContainer@@QEAAXPEAVCWnd@@I@Z
extern "C" void MS_ABI impl__AttachControlSite_COleControlContainer__QEAAXPEAVCWnd__I_Z(void* /*class*/* p0, unsigned int p1) {}

// Symbol: ?BroadcastAmbientPropertyChange@COleControlContainer@@UEAAXJ@Z
extern "C" void MS_ABI impl__BroadcastAmbientPropertyChange_COleControlContainer__UEAAXJ_Z(long p0) {}

// Symbol: ?CheckDlgButton@COleControlContainer@@UEAAXHI@Z
extern "C" void MS_ABI impl__CheckDlgButton_COleControlContainer__UEAAXHI_Z(int p0, unsigned int p1) {}

// Symbol: ?CheckRadioButton@COleControlContainer@@UEAAXHHH@Z
extern "C" void MS_ABI impl__CheckRadioButton_COleControlContainer__UEAAXHHH_Z(int p0, int p1, int p2) {}

// Symbol: ?CreateControl@COleControlContainer@@QEAAHPEAVCWnd@@AEBU_GUID@@PEB_WKAEBUtagRECT@@IPEAVCFile@@HPEA_WPEAPEAVCOleControlSite@@@Z
extern "C" int MS_ABI impl__CreateControl_COleControlContainer__QEAAHPEAVCWnd__AEBU_GUID__PEB_WKAEBUtagRECT__IPEAVCFile__HPEA_WPEAPEAVCOleControlSite___Z(void* /*class*/* p0, const void* /*struct*/* p1, const wchar_t* p2, unsigned long p3, const void* /*struct*/* p4, unsigned int p5, void* /*class*/* p6, int p7, wchar_t* p8, void* /*class*/** p9) {
    return 0;
}

// Symbol: ?CreateControl@COleControlContainer@@QEAAHPEAVCWnd@@AEBU_GUID@@PEB_WKPEBUtagPOINT@@PEBUtagSIZE@@IPEAVCFile@@HPEA_WPEAPEAVCOleControlSite@@@Z
extern "C" int MS_ABI impl__CreateControl_COleControlContainer__QEAAHPEAVCWnd__AEBU_GUID__PEB_WKPEBUtagPOINT__PEBUtagSIZE__IPEAVCFile__HPEA_WPEAPEAVCOleControlSite___Z(void* /*class*/* p0, const void* /*struct*/* p1, const wchar_t* p2, unsigned long p3, const void* /*struct*/* p4, const void* /*struct*/* p5, unsigned int p6, void* /*class*/* p7, int p8, wchar_t* p9, void* /*class*/** p10) {
    return 0;
}

// Symbol: ?CreateControl@COleControlContainer@@QEAAHPEAVCWnd@@AEBVCControlCreationInfo@@KPEBUtagPOINT@@PEBUtagSIZE@@I@Z
extern "C" int MS_ABI impl__CreateControl_COleControlContainer__QEAAHPEAVCWnd__AEBVCControlCreationInfo__KPEBUtagPOINT__PEBUtagSIZE__I_Z(void* /*class*/* p0, const void* /*class*/* p1, unsigned long p2, const void* /*struct*/* p3, const void* /*struct*/* p4, unsigned int p5) {
    return 0;
}

// Symbol: ?CreateControlCommon@COleControlContainer@@QEAAHPEAVCWnd@@AEBU_GUID@@AEBVCControlCreationInfo@@PEB_WKPEBUtagPOINT@@PEBUtagSIZE@@IPEAVCFile@@HPEA_WPEAPEAVCOleControlSite@@@Z
extern "C" int MS_ABI impl__CreateControlCommon_COleControlContainer__QEAAHPEAVCWnd__AEBU_GUID__AEBVCControlCreationInfo__PEB_WKPEBUtagPOINT__PEBUtagSIZE__IPEAVCFile__HPEA_WPEAPEAVCOleControlSite___Z(void* /*class*/* p0, const void* /*struct*/* p1, const void* /*class*/* p2, const wchar_t* p3, unsigned long p4, const void* /*struct*/* p5, const void* /*struct*/* p6, unsigned int p7, void* /*class*/* p8, int p9, wchar_t* p10, void* /*class*/** p11) {
    return 0;
}

// Symbol: ?CreateOleFont@COleControlContainer@@QEAAXPEAVCFont@@@Z
extern "C" void MS_ABI impl__CreateOleFont_COleControlContainer__QEAAXPEAVCFont___Z(void* /*class*/* p0) {}

// Symbol: ?FillListSitesOrWnds@COleControlContainer@@UEAAHPEAU_AFX_OCC_DIALOG_INFO@@@Z
extern "C" int MS_ABI impl__FillListSitesOrWnds_COleControlContainer__UEAAHPEAU_AFX_OCC_DIALOG_INFO___Z(void* /*struct*/* p0) {
    return 0;
}

// Symbol: ?FreezeAllEvents@COleControlContainer@@QEAAXH@Z
extern "C" void MS_ABI impl__FreezeAllEvents_COleControlContainer__QEAAXH_Z(int p0) {}

// Symbol: ?GetAmbientProp@COleControlContainer@@UEAAHPEAVCOleControlSite@@JPEAUtagVARIANT@@@Z
extern "C" int MS_ABI impl__GetAmbientProp_COleControlContainer__UEAAHPEAVCOleControlSite__JPEAUtagVARIANT___Z(void* /*class*/* p0, long p1, void* /*struct*/* p2) {
    return 0;
}

// Symbol: ?GetDispatchMap@COleControlContainer@@MEBAPEBUAFX_DISPMAP@@XZ
extern "C" void* MS_ABI impl__GetDispatchMap_COleControlContainer__MEBAPEBUAFX_DISPMAP__XZ() {
    return nullptr;
}

// Symbol: ?GetDlgItem@COleControlContainer@@UEBAPEAVCWnd@@H@Z
extern "C" void* MS_ABI impl__GetDlgItem_COleControlContainer__UEBAPEAVCWnd__H_Z(int p0) {
    return nullptr;
}

// Symbol: ?GetDlgItem@COleControlContainer@@UEBAXHPEAPEAUHWND__@@@Z
extern "C" void MS_ABI impl__GetDlgItem_COleControlContainer__UEBAXHPEAPEAUHWND_____Z(int p0, void* /*struct*/** p1) {}

// Symbol: ?GetDlgItemInt@COleControlContainer@@UEBAIHPEAHH@Z
extern "C" unsigned int MS_ABI impl__GetDlgItemInt_COleControlContainer__UEBAIHPEAHH_Z(int p0, int* p1, int p2) {
    return 0;
}

// Symbol: ?GetDlgItemTextW@COleControlContainer@@UEBAHHPEA_WH@Z
extern "C" int MS_ABI impl__GetDlgItemTextW_COleControlContainer__UEBAHHPEA_WH_Z(int p0, wchar_t* p1, int p2) {
    return 0;
}

// Symbol: ?GetThisDispatchMap@COleControlContainer@@KAPEBUAFX_DISPMAP@@XZ
extern "C" void* MS_ABI impl__GetThisDispatchMap_COleControlContainer__KAPEBUAFX_DISPMAP__XZ() {
    return nullptr;
}

// Symbol: ?HandleSetFocus@COleControlContainer@@UEAAHXZ
extern "C" int MS_ABI impl__HandleSetFocus_COleControlContainer__UEAAHXZ() {
    return 0;
}

// Symbol: ?HandleWindowlessMessage@COleControlContainer@@UEAAHI_K_JPEA_J@Z
extern "C" int MS_ABI impl__HandleWindowlessMessage_COleControlContainer__UEAAHI_K_JPEA_J_Z(unsigned int p0, unsigned __int64 p1, __int64 p2, __int64* p3) {
    return 0;
}

// Symbol: ?IsDlgButtonChecked@COleControlContainer@@UEBAIH@Z
extern "C" unsigned int MS_ABI impl__IsDlgButtonChecked_COleControlContainer__UEBAIH_Z(int p0) {
    return 0;
}

// Symbol: ?OnPaint@COleControlContainer@@UEAAHPEAVCDC@@@Z
extern "C" int MS_ABI impl__OnPaint_COleControlContainer__UEAAHPEAVCDC___Z(void* /*class*/* p0) {
    return 0;
}

// Symbol: ?OnUIActivate@COleControlContainer@@UEAAXPEAVCOleControlSite@@@Z
extern "C" void MS_ABI impl__OnUIActivate_COleControlContainer__UEAAXPEAVCOleControlSite___Z(void* /*class*/* p0) {}

// Symbol: ?OnUIDeactivate@COleControlContainer@@UEAAXPEAVCOleControlSite@@@Z
extern "C" void MS_ABI impl__OnUIDeactivate_COleControlContainer__UEAAXPEAVCOleControlSite___Z(void* /*class*/* p0) {}

// Symbol: ?ScrollChildren@COleControlContainer@@UEAAXHH@Z
extern "C" void MS_ABI impl__ScrollChildren_COleControlContainer__UEAAXHH_Z(int p0, int p1) {}

// Symbol: ?SendDlgItemMessageW@COleControlContainer@@UEAA_JHI_K_J@Z
extern "C" __int64 MS_ABI impl__SendDlgItemMessageW_COleControlContainer__UEAA_JHI_K_J_Z(int p0, unsigned int p1, unsigned __int64 p2, __int64 p3) {
    return 0;
}

// Symbol: ?SetDlgItemInt@COleControlContainer@@UEAAXHIH@Z
extern "C" void MS_ABI impl__SetDlgItemInt_COleControlContainer__UEAAXHIH_Z(int p0, unsigned int p1, int p2) {}

// Symbol: ?SetDlgItemTextW@COleControlContainer@@UEAAXHPEB_W@Z
extern "C" void MS_ABI impl__SetDlgItemTextW_COleControlContainer__UEAAXHPEB_W_Z(int p0, const wchar_t* p1) {}

// Symbol: ?ContextSensitiveHelp@XOleIPFrame@COleControlContainer@@UEAAJH@Z
extern "C" long MS_ABI impl__ContextSensitiveHelp_XOleIPFrame_COleControlContainer__UEAAJH_Z(int p0) {
    return 0;
}

// Symbol: ?EnableModeless@XOleIPFrame@COleControlContainer@@UEAAJH@Z
extern "C" long MS_ABI impl__EnableModeless_XOleIPFrame_COleControlContainer__UEAAJH_Z(int p0) {
    return 0;
}

// Symbol: ?EnumObjects@XOleContainer@COleControlContainer@@UEAAJKPEAPEAUIEnumUnknown@@@Z
extern "C" long MS_ABI impl__EnumObjects_XOleContainer_COleControlContainer__UEAAJKPEAPEAUIEnumUnknown___Z(unsigned long p0, void* /*struct*/** p1) {
    return 0;
}

// Symbol: ?GetBorder@XOleIPFrame@COleControlContainer@@UEAAJPEAUtagRECT@@@Z
extern "C" long MS_ABI impl__GetBorder_XOleIPFrame_COleControlContainer__UEAAJPEAUtagRECT___Z(void* /*struct*/* p0) {
    return 0;
}

// Symbol: ?GetWindow@XOleIPFrame@COleControlContainer@@UEAAJPEAPEAUHWND__@@@Z
extern "C" long MS_ABI impl__GetWindow_XOleIPFrame_COleControlContainer__UEAAJPEAPEAUHWND_____Z(void* /*struct*/** p0) {
    return 0;
}

// Symbol: ?InsertMenus@XOleIPFrame@COleControlContainer@@UEAAJPEAUHMENU__@@PEAUtagOleMenuGroupWidths@@@Z
extern "C" long MS_ABI impl__InsertMenus_XOleIPFrame_COleControlContainer__UEAAJPEAUHMENU____PEAUtagOleMenuGroupWidths___Z(void* /*struct*/* p0, void* /*struct*/* p1) {
    return 0;
}

// Symbol: ?LockContainer@XOleContainer@COleControlContainer@@UEAAJH@Z
extern "C" long MS_ABI impl__LockContainer_XOleContainer_COleControlContainer__UEAAJH_Z(int p0) {
    return 0;
}

// Symbol: ?ParseDisplayName@XOleContainer@COleControlContainer@@UEAAJPEAUIBindCtx@@PEA_WPEAKPEAPEAUIMoniker@@@Z
extern "C" long MS_ABI impl__ParseDisplayName_XOleContainer_COleControlContainer__UEAAJPEAUIBindCtx__PEA_WPEAKPEAPEAUIMoniker___Z(void* /*struct*/* p0, wchar_t* p1, unsigned long* p2, void* /*struct*/** p3) {
    return 0;
}

// Symbol: ?RemoveMenus@XOleIPFrame@COleControlContainer@@UEAAJPEAUHMENU__@@@Z
extern "C" long MS_ABI impl__RemoveMenus_XOleIPFrame_COleControlContainer__UEAAJPEAUHMENU_____Z(void* /*struct*/* p0) {
    return 0;
}

// Symbol: ?RequestBorderSpace@XOleIPFrame@COleControlContainer@@UEAAJPEBUtagRECT@@@Z
extern "C" long MS_ABI impl__RequestBorderSpace_XOleIPFrame_COleControlContainer__UEAAJPEBUtagRECT___Z(const void* /*struct*/* p0) {
    return 0;
}

// Symbol: ?SetActiveObject@XOleIPFrame@COleControlContainer@@UEAAJPEAUIOleInPlaceActiveObject@@PEB_W@Z
extern "C" long MS_ABI impl__SetActiveObject_XOleIPFrame_COleControlContainer__UEAAJPEAUIOleInPlaceActiveObject__PEB_W_Z(void* /*struct*/* p0, const wchar_t* p1) {
    return 0;
}

// Symbol: ?SetBorderSpace@XOleIPFrame@COleControlContainer@@UEAAJPEBUtagRECT@@@Z
extern "C" long MS_ABI impl__SetBorderSpace_XOleIPFrame_COleControlContainer__UEAAJPEBUtagRECT___Z(const void* /*struct*/* p0) {
    return 0;
}

// Symbol: ?SetMenu@XOleIPFrame@COleControlContainer@@UEAAJPEAUHMENU__@@PEAXPEAUHWND__@@@Z
extern "C" long MS_ABI impl__SetMenu_XOleIPFrame_COleControlContainer__UEAAJPEAUHMENU____PEAXPEAUHWND_____Z(void* /*struct*/* p0, void* p1, void* /*struct*/* p2) {
    return 0;
}

// Symbol: ?SetStatusText@XOleIPFrame@COleControlContainer@@UEAAJPEB_W@Z
extern "C" long MS_ABI impl__SetStatusText_XOleIPFrame_COleControlContainer__UEAAJPEB_W_Z(const wchar_t* p0) {
    return 0;
}

// Symbol: ?TranslateAcceleratorW@XOleIPFrame@COleControlContainer@@UEAAJPEAUtagMSG@@G@Z
extern "C" long MS_ABI impl__TranslateAcceleratorW_XOleIPFrame_COleControlContainer__UEAAJPEAUtagMSG__G_Z(void* /*struct*/* p0, unsigned short p1) {
    return 0;
}
