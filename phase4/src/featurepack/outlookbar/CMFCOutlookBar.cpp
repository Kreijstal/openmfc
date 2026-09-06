// CMFCOutlookBar — OpenMFC implementation.
// Sources: global_cmfcoutlookbar.cpp, manual_small_stub_implementations.cpp

#include "detail/CMFCOutlookBarSupport.h"
#include "detail/ManualSmallStubImplementationsSupport.h"

// Retail (0x180097f70):
//     if (m_hWnd == NULL) m_bMode2003 = bMode;
// (The retail NULL-this path falls through to the store and would fault; we
//  guard it instead.)
// Symbol: ?SetMode2003@CMFCOutlookBar@@QEAAXH@Z
extern "C" void MS_ABI impl__SetMode2003_CMFCOutlookBar__QEAAXH_Z(void* pThis, int bMode2003)
{
    if (pThis == nullptr)
        return;

    OutlookBar* pBar = static_cast<OutlookBar*>(pThis);
    if (pBar->m_hWnd == nullptr) {
        pBar->m_bMode2003 = bMode2003;
    }
}
// Symbol: ??0CMFCOutlookBar@@QEAA@XZ
extern "C" void* MS_ABI impl___0CMFCOutlookBar__QEAA_XZ(void* pThis) {
    return pThis;
}
// Symbol: ??1CMFCOutlookBar@@UEAA@XZ
extern "C" void* MS_ABI impl___1CMFCOutlookBar__UEAA_XZ() {
    return nullptr;
}

// Symbol: ?CanAcceptPane@CMFCOutlookBar@@UEBAHPEBVCBasePane@@@Z
extern "C" int MS_ABI impl__CanAcceptPane_CMFCOutlookBar__UEBAHPEBVCBasePane___Z(const void* /*class*/* p0) {
    return 0;
}

// Symbol: ?Create@CMFCOutlookBar@@UEAAHPEB_WPEAVCWnd@@AEBUtagRECT@@IKKPEAUCCreateContext@@@Z
extern "C" int MS_ABI impl__Create_CMFCOutlookBar__UEAAHPEB_WPEAVCWnd__AEBUtagRECT__IKKPEAUCCreateContext___Z(const wchar_t* p0, void* /*class*/* p1, const void* /*struct*/* p2, unsigned int p3, unsigned long p4, unsigned long p5, void* /*struct*/* p6) {
    return 0;
}

// Symbol: ?CreateCustomPage@CMFCOutlookBar@@QEAAPEAVCMFCOutlookBarPane@@PEB_WHKH@Z
extern "C" void* MS_ABI impl__CreateCustomPage_CMFCOutlookBar__QEAAPEAVCMFCOutlookBarPane__PEB_WHKH_Z(const wchar_t* p0, int p1, unsigned long p2, int p3) {
    return nullptr;
}

// Symbol: ?CreateObject@CMFCOutlookBar@@SAPEAVCObject@@XZ
extern "C" void* MS_ABI impl__CreateObject_CMFCOutlookBar__SAPEAVCObject__XZ() {
    return nullptr;
}

// Symbol: ?FindAvailablePageID@CMFCOutlookBar@@IEAAIXZ
extern "C" unsigned int MS_ABI impl__FindAvailablePageID_CMFCOutlookBar__IEAAIXZ() {
    return 0;
}

// Symbol: ?FloatTab@CMFCOutlookBar@@UEAAHPEAVCWnd@@HW4AFX_DOCK_METHOD@@H@Z
extern "C" int MS_ABI impl__FloatTab_CMFCOutlookBar__UEAAHPEAVCWnd__HW4AFX_DOCK_METHOD__H_Z(void* /*class*/* p0, int p1, int /*enum*/ p2, short* p3, int p4, char p5, void* p6, void* p7, unsigned long p8, int p9, float p10, unsigned char p11, void* p12, int p13, void* p14, char p15, int p16) {
    return 0;
}

// Symbol: ?GetDockingStatus@CMFCOutlookBar@@UEAA?AW4AFX_CS_STATUS@@VCPoint@@H@Z
extern "C" void* MS_ABI impl__GetDockingStatus_CMFCOutlookBar__UEAA_AW4AFX_CS_STATUS__VCPoint__H_Z(int /*enum*/* p0, short* p1, int p2, void* p3, void* p4, int p5, void* p6, void* p7, void** p8, void* /*struct*/ p9, void* /*class*/ p10, int p11) {
    return nullptr;
}

// Symbol: ?GetTabArea@CMFCOutlookBar@@UEBAXAEAVCRect@@0@Z
extern "C" void MS_ABI impl__GetTabArea_CMFCOutlookBar__UEBAXAEAVCRect__0_Z(void* /*class*/* p0, void* /*class*/* p1) {}

// Symbol: ?LoadState@CMFCOutlookBar@@UEAAHPEB_WHI@Z
extern "C" int MS_ABI impl__LoadState_CMFCOutlookBar__UEAAHPEB_WHI_Z(const wchar_t* p0, int p1, unsigned int p2) {
    return 0;
}

// Symbol: ?OnCreate@CMFCOutlookBar@@IEAAHPEAUtagCREATESTRUCTW@@@Z
extern "C" int MS_ABI impl__OnCreate_CMFCOutlookBar__IEAAHPEAUtagCREATESTRUCTW___Z(void* /*struct*/* p0) {
    return 0;
}

// Symbol: ?OnPaint@CMFCOutlookBar@@IEAAXXZ
extern "C" void MS_ABI impl__OnPaint_CMFCOutlookBar__IEAAXXZ() {}

// Symbol: ?RemoveCustomPage@CMFCOutlookBar@@QEAAHIPEAVCMFCOutlookBarTabCtrl@@@Z
extern "C" int MS_ABI impl__RemoveCustomPage_CMFCOutlookBar__QEAAHIPEAVCMFCOutlookBarTabCtrl___Z(unsigned int p0, void* /*class*/* p1) {
    return 0;
}

// Symbol: ?SaveState@CMFCOutlookBar@@UEAAHPEB_WHI@Z
extern "C" int MS_ABI impl__SaveState_CMFCOutlookBar__UEAAHPEB_WHI_Z(const wchar_t* p0, int p1, unsigned int p2) {
    return 0;
}

// Symbol: ?SetButtonsFont@CMFCOutlookBar@@QEAAXPEAVCFont@@H@Z
extern "C" void MS_ABI impl__SetButtonsFont_CMFCOutlookBar__QEAAXPEAVCFont__H_Z(void* /*class*/* p0, int p1) {}

