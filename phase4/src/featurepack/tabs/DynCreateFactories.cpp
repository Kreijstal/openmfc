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


























// Symbol: ?CreateObject@CMFCTabCtrl@@SAPEAVCObject@@XZ
extern "C" CObject* MS_ABI impl__CreateObject_CMFCTabCtrl__SAPEAVCObject__XZ()
{
    return new (std::nothrow) CMFCTabCtrl();
}
