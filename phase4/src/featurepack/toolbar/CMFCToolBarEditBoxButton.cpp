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
