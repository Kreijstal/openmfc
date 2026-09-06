// CMFCDynamicLayout — OpenMFC implementation.
// Sources: global_static_utils.cpp

#include "detail/CMFCDynamicLayoutSupport.h"

// Symbol: ?MoveHorizontal@CMFCDynamicLayout@@SA?AUMoveSettings@1@H@Z
extern "C" MS_ABI DynRatio* impl__MoveHorizontal_CMFCDynamicLayout__SA_AUMoveSettings_1_H_Z(DynRatio* ret, int nPercent)
{
    ret->x = nPercent; ret->y = 0; return ret;
}
// Symbol: ?MoveVertical@CMFCDynamicLayout@@SA?AUMoveSettings@1@H@Z
extern "C" MS_ABI DynRatio* impl__MoveVertical_CMFCDynamicLayout__SA_AUMoveSettings_1_H_Z(DynRatio* ret, int nPercent)
{
    ret->x = 0; ret->y = nPercent; return ret;
}
// Symbol: ?MoveHorizontalAndVertical@CMFCDynamicLayout@@SA?AUMoveSettings@1@HH@Z
extern "C" MS_ABI DynRatio* impl__MoveHorizontalAndVertical_CMFCDynamicLayout__SA_AUMoveSettings_1_HH_Z(DynRatio* ret, int nXPercent, int nYPercent)
{
    ret->x = nXPercent; ret->y = nYPercent; return ret;
}
// Symbol: ?MoveNone@CMFCDynamicLayout@@SA?AUMoveSettings@1@XZ
extern "C" MS_ABI DynRatio* impl__MoveNone_CMFCDynamicLayout__SA_AUMoveSettings_1_XZ(DynRatio* ret)
{
    ret->x = 0; ret->y = 0; return ret;
}
// Symbol: ?SizeHorizontal@CMFCDynamicLayout@@SA?AUSizeSettings@1@H@Z
extern "C" MS_ABI DynRatio* impl__SizeHorizontal_CMFCDynamicLayout__SA_AUSizeSettings_1_H_Z(DynRatio* ret, int nPercent)
{
    ret->x = nPercent; ret->y = 0; return ret;
}
// Symbol: ?SizeVertical@CMFCDynamicLayout@@SA?AUSizeSettings@1@H@Z
extern "C" MS_ABI DynRatio* impl__SizeVertical_CMFCDynamicLayout__SA_AUSizeSettings_1_H_Z(DynRatio* ret, int nPercent)
{
    ret->x = 0; ret->y = nPercent; return ret;
}
// Symbol: ?SizeHorizontalAndVertical@CMFCDynamicLayout@@SA?AUSizeSettings@1@HH@Z
extern "C" MS_ABI DynRatio* impl__SizeHorizontalAndVertical_CMFCDynamicLayout__SA_AUSizeSettings_1_HH_Z(DynRatio* ret, int nXPercent, int nYPercent)
{
    ret->x = nXPercent; ret->y = nYPercent; return ret;
}
// Symbol: ?SizeNone@CMFCDynamicLayout@@SA?AUSizeSettings@1@XZ
extern "C" MS_ABI DynRatio* impl__SizeNone_CMFCDynamicLayout__SA_AUSizeSettings_1_XZ(DynRatio* ret)
{
    ret->x = 0; ret->y = 0; return ret;
}
// Symbol: ?AddItem@CMFCDynamicLayout@@QEAAHIUMoveSettings@1@USizeSettings@1@@Z
extern "C" int MS_ABI impl__AddItem_CMFCDynamicLayout__QEAAHIUMoveSettings_1_USizeSettings_1__Z(unsigned int p0, void* /*struct*/ p1) {
    return 0;
}

// Symbol: ?AddItem@CMFCDynamicLayout@@QEAAHPEAUHWND__@@UMoveSettings@1@USizeSettings@1@@Z
extern "C" int MS_ABI impl__AddItem_CMFCDynamicLayout__QEAAHPEAUHWND____UMoveSettings_1_USizeSettings_1__Z(void* /*struct*/* p0, void* /*struct*/ p1) {
    return 0;
}

// Symbol: ?Adjust@CMFCDynamicLayout@@QEAAXXZ
extern "C" void MS_ABI impl__Adjust_CMFCDynamicLayout__QEAAXXZ() {}

// Symbol: ?AdjustItemRect@CMFCDynamicLayout@@IEBAIAEAUAFX_DYNAMIC_LAYOUT_ITEM@@AEAVCRect@@@Z
extern "C" unsigned int MS_ABI impl__AdjustItemRect_CMFCDynamicLayout__IEBAIAEAUAFX_DYNAMIC_LAYOUT_ITEM__AEAVCRect___Z(void* /*struct*/* p0, void* /*class*/* p1) {
    return 0;
}

// Symbol: ?CorrectItem@CMFCDynamicLayout@@IEBAXAEAUAFX_DYNAMIC_LAYOUT_ITEM@@@Z
extern "C" void MS_ABI impl__CorrectItem_CMFCDynamicLayout__IEBAXAEAUAFX_DYNAMIC_LAYOUT_ITEM___Z(void* /*struct*/* p0) {}

// Symbol: ?Create@CMFCDynamicLayout@@QEAAHPEAVCWnd@@@Z
extern "C" int MS_ABI impl__Create_CMFCDynamicLayout__QEAAHPEAVCWnd___Z(void* /*class*/* p0) {
    return 0;
}

// Symbol: ?FindItem@CMFCDynamicLayout@@IEAAPEAUAFX_DYNAMIC_LAYOUT_ITEM@@PEAUHWND__@@@Z
extern "C" void* MS_ABI impl__FindItem_CMFCDynamicLayout__IEAAPEAUAFX_DYNAMIC_LAYOUT_ITEM__PEAUHWND_____Z(void* /*struct*/* p0) {
    return nullptr;
}

// Symbol: ?GetHostWndRect@CMFCDynamicLayout@@QEBAXAEAVCRect@@@Z
extern "C" void MS_ABI impl__GetHostWndRect_CMFCDynamicLayout__QEBAXAEAVCRect___Z(void* /*class*/* p0) {}

// Symbol: ?GetItemRect@CMFCDynamicLayout@@IEBA?AVCRect@@AEAUAFX_DYNAMIC_LAYOUT_ITEM@@@Z
extern "C" void* MS_ABI impl__GetItemRect_CMFCDynamicLayout__IEBA_AVCRect__AEAUAFX_DYNAMIC_LAYOUT_ITEM___Z(void* /*class*/* p0, void* /*struct*/* p1) {
    return nullptr;
}

// Symbol: ?LoadResource@CMFCDynamicLayout@@SAHPEAVCWnd@@PEAXK@Z
extern "C" int MS_ABI impl__LoadResource_CMFCDynamicLayout__SAHPEAVCWnd__PEAXK_Z(void* /*class*/* p0, void* p1, unsigned long p2) {
    return 0;
}

// Symbol: ?PrepareItem@CMFCDynamicLayout@@IEBAHAEAUAFX_DYNAMIC_LAYOUT_ITEM@@@Z
extern "C" int MS_ABI impl__PrepareItem_CMFCDynamicLayout__IEBAHAEAUAFX_DYNAMIC_LAYOUT_ITEM___Z(void* /*struct*/* p0) {
    return 0;
}

// Generated 5893 function stubs + 46 data stubs
// Errors (could not parse): 0
// Excluded (real implementations): 8282

