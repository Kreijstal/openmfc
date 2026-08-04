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
