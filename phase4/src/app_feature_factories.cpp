// Small factory exports backed by concrete runtime-class callbacks or complete classes.

#include "openmfc/afxmfc.h"

#include <new>

#if defined(__GNUC__) || defined(__clang__)
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

// Symbol: ?CreateObject@CMFCControlRenderer@@SAPEAVCObject@@XZ
extern "C" CObject* MS_ABI impl__CreateObject_CMFCControlRenderer__SAPEAVCObject__XZ()
{
    return new (std::nothrow) CMFCControlRenderer();
}

// Symbol: ?CreateObject@CMFCRibbonProgressBar@@SAPEAVCObject@@XZ
extern "C" CObject* MS_ABI impl__CreateObject_CMFCRibbonProgressBar__SAPEAVCObject__XZ()
{
    return new (std::nothrow) CMFCRibbonProgressBar();
}

// Symbol: ?CreateObject@CMFCRibbonSeparator@@SAPEAVCObject@@XZ
extern "C" CObject* MS_ABI impl__CreateObject_CMFCRibbonSeparator__SAPEAVCObject__XZ()
{
    return new (std::nothrow) CMFCRibbonSeparator();
}

// Symbol: ?CreateObject@CMFCRibbonSlider@@SAPEAVCObject@@XZ
extern "C" CObject* MS_ABI impl__CreateObject_CMFCRibbonSlider__SAPEAVCObject__XZ()
{
    return new (std::nothrow) CMFCRibbonSlider();
}

// Symbol: ?CreateObject@CMFCRibbonStatusBarPane@@SAPEAVCObject@@XZ
extern "C" CObject* MS_ABI impl__CreateObject_CMFCRibbonStatusBarPane__SAPEAVCObject__XZ()
{
    return new (std::nothrow) CMFCRibbonStatusBarPane();
}
