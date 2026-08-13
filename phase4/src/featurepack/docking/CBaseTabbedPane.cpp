// CBaseTabbedPane — OpenMFC implementation.
// Sources: mfccore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/MfccoreSupport.h"

// Symbol: ??0CBaseTabbedPane@@QEAA@H@Z
extern "C" void* MS_ABI impl___0CBaseTabbedPane__QEAA_H_Z(void* pThis, int iStyle) { return new (pThis) CBaseTabbedPane(iStyle); }
// Symbol: ??1CBaseTabbedPane@@UEAA@XZ
extern "C" void MS_ABI impl___1CBaseTabbedPane__UEAA_XZ(CBaseTabbedPane* pThis) { if (pThis) pThis->~CBaseTabbedPane(); }
CBaseTabbedPane::CBaseTabbedPane(int iStyle) {
    memset(_basetabbedpane_padding, 0, sizeof(_basetabbedpane_padding));
    (void)iStyle;
}
CBaseTabbedPane::~CBaseTabbedPane() {}
// Symbol: ?AddTab@CBaseTabbedPane@@UEAAHPEAVCWnd@@HHH@Z
extern "C" int MS_ABI impl__AddTab_CBaseTabbedPane__UEAAHPEAVCWnd__HHH_Z(void* /*class*/* p0, int p1, int p2, int p3) {
    return 0;
}

// Symbol: ?ApplyRestoredTabInfo@CBaseTabbedPane@@UEAAXH@Z
extern "C" void MS_ABI impl__ApplyRestoredTabInfo_CBaseTabbedPane__UEAAXH_Z(int p0) {}

// Symbol: ?CanFloat@CBaseTabbedPane@@UEBAHXZ
extern "C" int MS_ABI impl__CanFloat_CBaseTabbedPane__UEBAHXZ() {
    return 0;
}

// Symbol: ?ConvertToTabbedDocument@CBaseTabbedPane@@UEAAXH@Z
extern "C" void MS_ABI impl__ConvertToTabbedDocument_CBaseTabbedPane__UEAAXH_Z(int p0) {}

// Symbol: ?DetachPane@CBaseTabbedPane@@UEAAHPEAVCWnd@@H@Z
extern "C" int MS_ABI impl__DetachPane_CBaseTabbedPane__UEAAHPEAVCWnd__H_Z(void* /*class*/* p0, int p1) {
    return 0;
}

// Symbol: ?Dock@CBaseTabbedPane@@MEAAHPEAVCBasePane@@PEBUtagRECT@@W4AFX_DOCK_METHOD@@@Z
extern "C" int MS_ABI impl__Dock_CBaseTabbedPane__MEAAHPEAVCBasePane__PEBUtagRECT__W4AFX_DOCK_METHOD___Z(void* /*class*/* p0, const void* /*struct*/* p1, int /*enum*/ p2, short* p3, int p4, char p5, void* p6, void* p7, unsigned long p8, int p9, float p10, unsigned char p11, void* p12, int p13, void* p14, char p15) {
    return 0;
}

// Symbol: ?FillDefaultTabsOrderArray@CBaseTabbedPane@@QEAAXXZ
extern "C" void MS_ABI impl__FillDefaultTabsOrderArray_CBaseTabbedPane__QEAAXXZ() {}

// Symbol: ?FindBarByTabNumber@CBaseTabbedPane@@UEAAPEAVCWnd@@HH@Z
extern "C" void* MS_ABI impl__FindBarByTabNumber_CBaseTabbedPane__UEAAPEAVCWnd__HH_Z(int p0, int p1) {
    return nullptr;
}

// Symbol: ?FindPaneByID@CBaseTabbedPane@@UEAAPEAVCWnd@@I@Z
extern "C" void* MS_ABI impl__FindPaneByID_CBaseTabbedPane__UEAAPEAVCWnd__I_Z(unsigned int p0) {
    return nullptr;
}

// Symbol: ?FloatPane@CBaseTabbedPane@@MEAAHVCRect@@W4AFX_DOCK_METHOD@@_N@Z
extern "C" int MS_ABI impl__FloatPane_CBaseTabbedPane__MEAAHVCRect__W4AFX_DOCK_METHOD___N_Z(void* /*class*/ p0, int /*enum*/ p1, short* p2, int p3, char p4, void* p5, void* p6, unsigned long p7, int p8, float p9, unsigned char p10, void* p11, int p12, void* p13, char p14, bool p15) {
    return 0;
}

// Symbol: ?FloatTab@CBaseTabbedPane@@UEAAHPEAVCWnd@@HW4AFX_DOCK_METHOD@@H@Z
extern "C" int MS_ABI impl__FloatTab_CBaseTabbedPane__UEAAHPEAVCWnd__HW4AFX_DOCK_METHOD__H_Z(void* /*class*/* p0, int p1, int /*enum*/ p2, short* p3, int p4, char p5, void* p6, void* p7, unsigned long p8, int p9, float p10, unsigned char p11, void* p12, int p13, void* p14, char p15, int p16) {
    return 0;
}

// Symbol: ?GetFirstVisibleTab@CBaseTabbedPane@@UEAAPEAVCWnd@@AEAH@Z
extern "C" void* MS_ABI impl__GetFirstVisibleTab_CBaseTabbedPane__UEAAPEAVCWnd__AEAH_Z(int* p0) {
    return nullptr;
}

// Symbol: ?GetMinSize@CBaseTabbedPane@@UEBAXAEAVCSize@@@Z
extern "C" void MS_ABI impl__GetMinSize_CBaseTabbedPane__UEBAXAEAVCSize___Z(void* /*class*/* p0) {}

// Symbol: ?GetPaneIcon@CBaseTabbedPane@@UEAAPEAUHICON__@@H@Z
extern "C" void* MS_ABI impl__GetPaneIcon_CBaseTabbedPane__UEAAPEAUHICON____H_Z(int p0) {
    return nullptr;
}

// Symbol: ?GetPaneList@CBaseTabbedPane@@UEAAXAEAVCObList@@PEAUCRuntimeClass@@@Z
extern "C" void MS_ABI impl__GetPaneList_CBaseTabbedPane__UEAAXAEAVCObList__PEAUCRuntimeClass___Z(void* /*class*/* p0, void* /*struct*/* p1) {}

// Symbol: ?LoadSiblingPaneIDs@CBaseTabbedPane@@SAXAEAVCArchive@@AEAV?$CList@II@@@Z
extern "C" void MS_ABI impl__LoadSiblingPaneIDs_CBaseTabbedPane__SAXAEAVCArchive__AEAV__CList_II___Z(void* /*class*/* p0, void* /*class*/* p1) {}

// Symbol: ?LoadState@CBaseTabbedPane@@MEAAHPEB_WHI@Z
extern "C" int MS_ABI impl__LoadState_CBaseTabbedPane__MEAAHPEB_WHI_Z(const wchar_t* p0, int p1, unsigned int p2) {
    return 0;
}

// Symbol: ?OnChangeActiveTab@CBaseTabbedPane@@IEAA_J_K_J@Z
extern "C" __int64 MS_ABI impl__OnChangeActiveTab_CBaseTabbedPane__IEAA_J_K_J_Z(unsigned __int64 p0, __int64 p1) {
    return 0;
}

// Symbol: ?OnEraseBkgnd@CBaseTabbedPane@@IEAAHPEAVCDC@@@Z
extern "C" int MS_ABI impl__OnEraseBkgnd_CBaseTabbedPane__IEAAHPEAVCDC___Z(void* /*class*/* p0) {
    return 0;
}

// Symbol: ?OnNcDestroy@CBaseTabbedPane@@IEAAXXZ
extern "C" void MS_ABI impl__OnNcDestroy_CBaseTabbedPane__IEAAXXZ() {}

// Symbol: ?OnSetFocus@CBaseTabbedPane@@IEAAXPEAVCWnd@@@Z
extern "C" void MS_ABI impl__OnSetFocus_CBaseTabbedPane__IEAAXPEAVCWnd___Z(void* /*class*/* p0) {}

// Symbol: ?OnSize@CBaseTabbedPane@@IEAAXIHH@Z
extern "C" void MS_ABI impl__OnSize_CBaseTabbedPane__IEAAXIHH_Z(unsigned int p0, int p1, int p2) {}

// Symbol: ?RecalcLayout@CBaseTabbedPane@@UEAAXXZ
extern "C" void MS_ABI impl__RecalcLayout_CBaseTabbedPane__UEAAXXZ() {}

// Symbol: ?RemovePane@CBaseTabbedPane@@UEAAHPEAVCWnd@@@Z
extern "C" int MS_ABI impl__RemovePane_CBaseTabbedPane__UEAAHPEAVCWnd___Z(void* /*class*/* p0) {
    return 0;
}

// Symbol: ?SaveSiblingBarIDs@CBaseTabbedPane@@UEAAXAEAVCArchive@@@Z
extern "C" void MS_ABI impl__SaveSiblingBarIDs_CBaseTabbedPane__UEAAXAEAVCArchive___Z(void* /*class*/* p0) {}

// Symbol: ?SaveState@CBaseTabbedPane@@MEAAHPEB_WHI@Z
extern "C" int MS_ABI impl__SaveState_CBaseTabbedPane__MEAAHPEB_WHI_Z(const wchar_t* p0, int p1, unsigned int p2) {
    return 0;
}

// Symbol: ?Serialize@CBaseTabbedPane@@UEAAXAEAVCArchive@@@Z
extern "C" void MS_ABI impl__Serialize_CBaseTabbedPane__UEAAXAEAVCArchive___Z(void* /*class*/* p0) {}

// Symbol: ?SerializeTabWindow@CBaseTabbedPane@@UEAAXAEAVCArchive@@@Z
extern "C" void MS_ABI impl__SerializeTabWindow_CBaseTabbedPane__UEAAXAEAVCArchive___Z(void* /*class*/* p0) {}

// Symbol: ?SetAutoHideMode@CBaseTabbedPane@@UEAAPEAVCMFCAutoHideBar@@HKPEAV2@H@Z
extern "C" void* MS_ABI impl__SetAutoHideMode_CBaseTabbedPane__UEAAPEAVCMFCAutoHideBar__HKPEAV2_H_Z(int p0, unsigned long p1, void* /*class*/* p2, int p3) {
    return nullptr;
}

// Symbol: ?ShowTab@CBaseTabbedPane@@UEAAHPEAVCWnd@@HHH@Z
extern "C" int MS_ABI impl__ShowTab_CBaseTabbedPane__UEAAHPEAVCWnd__HHH_Z(void* /*class*/* p0, int p1, int p2, int p3) {
    return 0;
}

// Symbol: ?StoreRecentDockSiteInfo@CBaseTabbedPane@@MEAAXXZ
extern "C" void MS_ABI impl__StoreRecentDockSiteInfo_CBaseTabbedPane__MEAAXXZ() {}
