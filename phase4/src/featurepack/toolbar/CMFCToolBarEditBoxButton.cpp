// CMFCToolBarEditBoxButton — OpenMFC implementation.
// Sources: cbarcore.cpp, mfccore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/CbarcoreSupport.h"
#include "detail/MfccoreSupport.h"

// Symbol: ?SetContents@CMFCToolBarEditBoxButton@@UEAAXAEBV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@@Z
void CMFCToolBarEditBoxButton::SetContents(const CString& sContents) {
    RegisterEditButton(this);
    m_strText = sContents;
    if (m_nID != 0) {
        g_editContentsByCmd[m_nID] = sContents;
    }
}
// Symbol: ?GetByCmd@CMFCToolBarEditBoxButton@@SAPEAV1@I@Z
CMFCToolBarEditBoxButton* CMFCToolBarEditBoxButton::GetByCmd(UINT uiCmd) {
    auto it = g_editByCmd.find(uiCmd);
    if (it == g_editByCmd.end() || it->second.empty()) return nullptr;
    return it->second.front();
}
// Symbol: ?GetContentsAll@CMFCToolBarEditBoxButton@@SA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@I@Z
CString CMFCToolBarEditBoxButton::GetContentsAll(UINT uiCmd) {
    if (CMFCToolBarEditBoxButton* pButton = GetByCmd(uiCmd)) {
        return pButton->m_strText;
    }
    auto it = g_editContentsByCmd.find(uiCmd);
    return (it != g_editContentsByCmd.end()) ? it->second : CString();
}
// Symbol: ?SetContentsAll@CMFCToolBarEditBoxButton@@SAHIAEBV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@@Z
BOOL CMFCToolBarEditBoxButton::SetContentsAll(UINT uiCmd, const CString& sContents) {
    BOOL updated = FALSE;
    g_editContentsByCmd[uiCmd] = sContents;
    auto it = g_editByCmd.find(uiCmd);
    if (it != g_editByCmd.end()) {
        for (CMFCToolBarEditBoxButton* button : it->second) {
            if (!button) continue;
            button->m_strText = sContents;
            updated = TRUE;
        }
    }
    return updated;
}
// Symbol: ?SetContents@CMFCToolBarEditBoxButton@@UEAAXAEBV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@@Z
extern "C" void MS_ABI impl__SetContents_CMFCToolBarEditBoxButton__UEAAXAEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(CMFCToolBarEditBoxButton* pThis, const CString* p0) {
    pThis->SetContents(*p0);
}
// Symbol: ?GetByCmd@CMFCToolBarEditBoxButton@@SAPEAV1@I@Z
extern "C" CMFCToolBarEditBoxButton* MS_ABI impl__GetByCmd_CMFCToolBarEditBoxButton__SAPEAV1_I_Z(unsigned int p0) {
    return CMFCToolBarEditBoxButton::GetByCmd(p0);
}
// Symbol: ?GetContentsAll@CMFCToolBarEditBoxButton@@SA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@I@Z
extern "C" void MS_ABI impl__GetContentsAll_CMFCToolBarEditBoxButton__SA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__I_Z(CString* pRet, unsigned int p0) {
    *pRet = CMFCToolBarEditBoxButton::GetContentsAll(p0);
}
// Symbol: ?SetContentsAll@CMFCToolBarEditBoxButton@@SAHIAEBV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@@Z
extern "C" int MS_ABI impl__SetContentsAll_CMFCToolBarEditBoxButton__SAHIAEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(unsigned int p0, const CString* p1) {
    return CMFCToolBarEditBoxButton::SetContentsAll(p0, *p1);
}
CMFCToolBarEditBoxButton::~CMFCToolBarEditBoxButton() {
    UnregisterEditButton(this);
}
CMFCToolBarEditBoxButton::CMFCToolBarEditBoxButton() { memset(_pad, 0, sizeof(_pad)); }
// Symbol: ?CopyFrom@CMFCToolBarEditBoxButton@@UEAAXAEBVCMFCToolBarButton@@@Z
extern "C" void MS_ABI impl__CopyFrom_CMFCToolBarEditBoxButton__UEAAXAEBVCMFCToolBarButton___Z(const void* /*class*/* p0) {}

// Symbol: ?CreateEdit@CMFCToolBarEditBoxButton@@UEAAPEAVCEdit@@PEAVCWnd@@AEBVCRect@@@Z
extern "C" void* MS_ABI impl__CreateEdit_CMFCToolBarEditBoxButton__UEAAPEAVCEdit__PEAVCWnd__AEBVCRect___Z(void* /*class*/* p0, const void* /*class*/* p1) {
    return nullptr;
}

// Symbol: ?GetEditBorder@CMFCToolBarEditBoxButton@@UEAAXAEAVCRect@@@Z
extern "C" void MS_ABI impl__GetEditBorder_CMFCToolBarEditBoxButton__UEAAXAEAVCRect___Z(void* /*class*/* p0) {}

// Symbol: ?GetInvalidateRect@CMFCToolBarEditBoxButton@@UEBA?BVCRect@@XZ
extern "C" void* MS_ABI impl__GetInvalidateRect_CMFCToolBarEditBoxButton__UEBA_BVCRect__XZ(void* p0, void* /*class*/ p1) {
    return nullptr;
}

// Symbol: ?Initialize@CMFCToolBarEditBoxButton@@IEAAXXZ
extern "C" void MS_ABI impl__Initialize_CMFCToolBarEditBoxButton__IEAAXXZ() {}

// Symbol: ?NotifyCommand@CMFCToolBarEditBoxButton@@UEAAHH@Z
extern "C" int MS_ABI impl__NotifyCommand_CMFCToolBarEditBoxButton__UEAAHH_Z(int p0) {
    return 0;
}

// Symbol: ?OnAddToCustomizePage@CMFCToolBarEditBoxButton@@UEAAXXZ
extern "C" void MS_ABI impl__OnAddToCustomizePage_CMFCToolBarEditBoxButton__UEAAXXZ() {}

// Symbol: ?OnCalculateSize@CMFCToolBarEditBoxButton@@UEAA?AUtagSIZE@@PEAVCDC@@AEBVCSize@@H@Z
extern "C" void* MS_ABI impl__OnCalculateSize_CMFCToolBarEditBoxButton__UEAA_AUtagSIZE__PEAVCDC__AEBVCSize__H_Z(void* /*struct*/* p0, void* /*class*/* p1, const void* /*class*/* p2, int p3) {
    return nullptr;
}

// Symbol: ?OnChangeParentWnd@CMFCToolBarEditBoxButton@@UEAAXPEAVCWnd@@@Z
extern "C" void MS_ABI impl__OnChangeParentWnd_CMFCToolBarEditBoxButton__UEAAXPEAVCWnd___Z(void* /*class*/* p0) {}

// Symbol: ?OnClick@CMFCToolBarEditBoxButton@@UEAAHPEAVCWnd@@H@Z
extern "C" int MS_ABI impl__OnClick_CMFCToolBarEditBoxButton__UEAAHPEAVCWnd__H_Z(void* /*class*/* p0, int p1) {
    return 0;
}

// Symbol: ?OnCtlColor@CMFCToolBarEditBoxButton@@UEAAPEAUHBRUSH__@@PEAVCDC@@I@Z
extern "C" void* MS_ABI impl__OnCtlColor_CMFCToolBarEditBoxButton__UEAAPEAUHBRUSH____PEAVCDC__I_Z(void* /*class*/* p0, unsigned int p1) {
    return nullptr;
}

// Symbol: ?OnDraw@CMFCToolBarEditBoxButton@@UEAAXPEAVCDC@@AEBVCRect@@PEAVCMFCToolBarImages@@HHHHH@Z
extern "C" void MS_ABI impl__OnDraw_CMFCToolBarEditBoxButton__UEAAXPEAVCDC__AEBVCRect__PEAVCMFCToolBarImages__HHHHH_Z(void* /*class*/* p0, const void* /*class*/* p1, void* /*class*/* p2, int p3, int p4, int p5, int p6, int p7) {}

// Symbol: ?OnDrawOnCustomizeList@CMFCToolBarEditBoxButton@@UEAAHPEAVCDC@@AEBVCRect@@H@Z
extern "C" int MS_ABI impl__OnDrawOnCustomizeList_CMFCToolBarEditBoxButton__UEAAHPEAVCDC__AEBVCRect__H_Z(void* /*class*/* p0, const void* /*class*/* p1, int p2) {
    return 0;
}

// Symbol: ?OnGlobalFontsChanged@CMFCToolBarEditBoxButton@@UEAAXXZ
extern "C" void MS_ABI impl__OnGlobalFontsChanged_CMFCToolBarEditBoxButton__UEAAXXZ() {}

// Symbol: ?OnMove@CMFCToolBarEditBoxButton@@UEAAXXZ
extern "C" void MS_ABI impl__OnMove_CMFCToolBarEditBoxButton__UEAAXXZ() {}

// Symbol: ?OnShow@CMFCToolBarEditBoxButton@@UEAAXH@Z
extern "C" void MS_ABI impl__OnShow_CMFCToolBarEditBoxButton__UEAAXH_Z(int p0) {}

// Symbol: ?OnSize@CMFCToolBarEditBoxButton@@UEAAXH@Z
extern "C" void MS_ABI impl__OnSize_CMFCToolBarEditBoxButton__UEAAXH_Z(int p0) {}

// Symbol: ?OnUpdateToolTip@CMFCToolBarEditBoxButton@@UEAAHPEAVCWnd@@HAEAVCToolTipCtrl@@AEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@@Z
extern "C" int MS_ABI impl__OnUpdateToolTip_CMFCToolBarEditBoxButton__UEAAHPEAVCWnd__HAEAVCToolTipCtrl__AEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(void* /*class*/* p0, int p1, void* /*class*/* p2, void* /*class*/* p3, void** p4, void* p5) {
    return 0;
}

// Symbol: ?Serialize@CMFCToolBarEditBoxButton@@UEAAXAEAVCArchive@@@Z
extern "C" void MS_ABI impl__Serialize_CMFCToolBarEditBoxButton__UEAAXAEAVCArchive___Z(void* /*class*/* p0) {}

// Symbol: ?SetACCData@CMFCToolBarEditBoxButton@@UEAAHPEAVCWnd@@AEAVCAccessibilityData@@@Z
extern "C" int MS_ABI impl__SetACCData_CMFCToolBarEditBoxButton__UEAAHPEAVCWnd__AEAVCAccessibilityData___Z(void* /*class*/* p0, void* /*class*/* p1) {
    return 0;
}

// Symbol: ?SetHotEdit@CMFCToolBarEditBoxButton@@IEAAXH@Z
extern "C" void MS_ABI impl__SetHotEdit_CMFCToolBarEditBoxButton__IEAAXH_Z(int p0) {}

// Symbol: ?SetStyle@CMFCToolBarEditBoxButton@@UEAAXI@Z
extern "C" void MS_ABI impl__SetStyle_CMFCToolBarEditBoxButton__UEAAXI_Z(unsigned int p0) {}
