// DynCreateFactories — OpenMFC implementation.
// Sources: app_feature_factories.cpp

// Small factory exports backed by concrete runtime-class callbacks or complete classes.

#include "openmfc/afxmfc.h"

#include <new>

#if defined(__GNUC__) || defined(__clang__)
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif


























// Symbol: ?CreateObject@CPaneDivider@@SAPEAVCObject@@XZ
extern "C" CObject* MS_ABI impl__CreateObject_CPaneDivider__SAPEAVCObject__XZ()
{
    return new (std::nothrow) CPaneDivider();
}
// Symbol: ?CreateObject@CPaneFrameWnd@@SAPEAVCObject@@XZ
extern "C" CObject* MS_ABI impl__CreateObject_CPaneFrameWnd__SAPEAVCObject__XZ()
{
    return new (std::nothrow) CPaneFrameWnd();
}
