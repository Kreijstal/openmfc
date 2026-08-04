// DynCreateFactories — OpenMFC implementation.
// Sources: app_feature_factories.cpp, global_dyncreate_factories.cpp

#include "detail/DyncreateFactoriesSupport.h"

// Small factory exports backed by concrete runtime-class callbacks or complete classes.

#include "openmfc/afxmfc.h"

#include <new>

#if defined(__GNUC__) || defined(__clang__)
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif


























// Symbol: ?CreateObject@CMFCRibbonButton@@SAPEAVCObject@@XZ
extern "C" CObject* MS_ABI impl__CreateObject_CMFCRibbonButton__SAPEAVCObject__XZ()
{
    return new (std::nothrow) CMFCRibbonButton();
}
// Symbol: ?CreateObject@CMFCRibbonButtonsGroup@@SAPEAVCObject@@XZ
extern "C" CObject* MS_ABI impl__CreateObject_CMFCRibbonButtonsGroup__SAPEAVCObject__XZ()
{
    return new (std::nothrow) CMFCRibbonButtonsGroup();
}
// Symbol: ?CreateObject@CMFCRibbonCheckBox@@SAPEAVCObject@@XZ
extern "C" CObject* MS_ABI impl__CreateObject_CMFCRibbonCheckBox__SAPEAVCObject__XZ()
{
    return new (std::nothrow) CMFCRibbonCheckBox();
}
// Symbol: ?CreateObject@CMFCRibbonEdit@@SAPEAVCObject@@XZ
extern "C" CObject* MS_ABI impl__CreateObject_CMFCRibbonEdit__SAPEAVCObject__XZ()
{
    return new (std::nothrow) CMFCRibbonEdit();
}
// Symbol: ?CreateObject@CMFCRibbonGallery@@SAPEAVCObject@@XZ
extern "C" CObject* MS_ABI impl__CreateObject_CMFCRibbonGallery__SAPEAVCObject__XZ()
{
    return new (std::nothrow) CMFCRibbonGallery();
}
// Symbol: ?CreateObject@CMFCRibbonLabel@@SAPEAVCObject@@XZ
extern "C" CObject* MS_ABI impl__CreateObject_CMFCRibbonLabel__SAPEAVCObject__XZ()
{
    return new (std::nothrow) CMFCRibbonLabel();
}
// Symbol: ?CreateObject@CMFCRibbonMainPanel@@SAPEAVCObject@@XZ
extern "C" CObject* MS_ABI impl__CreateObject_CMFCRibbonMainPanel__SAPEAVCObject__XZ()
{
    return new (std::nothrow) CMFCRibbonMainPanel();
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
// Symbol: ?CreateObject@CMFCRibbonColorMenuButton@@SAPEAVCObject@@XZ
extern "C" CObject* MS_ABI impl__CreateObject_CMFCRibbonColorMenuButton__SAPEAVCObject__XZ()
{ return CreateSized<CMFCRibbonButton, 640>(); }
// Symbol: ?CreateObject@CMFCRibbonQuickAccessCustomizeButton@@SAPEAVCObject@@XZ
extern "C" CObject* MS_ABI impl__CreateObject_CMFCRibbonQuickAccessCustomizeButton__SAPEAVCObject__XZ()
{ return CreateSized<CMFCRibbonButton, 672>(); }
// Symbol: ?CreateObject@CMFCRibbonStatusBarCustomizeButton@@SAPEAVCObject@@XZ
extern "C" CObject* MS_ABI impl__CreateObject_CMFCRibbonStatusBarCustomizeButton__SAPEAVCObject__XZ()
{ return CreateSized<CMFCRibbonButton, 624>(); }
// Symbol: ?CreateObject@CRibbonUndoLabel@@SAPEAVCObject@@XZ
extern "C" CObject* MS_ABI impl__CreateObject_CRibbonUndoLabel__SAPEAVCObject__XZ()
{ return CreateSized<CMFCRibbonButton, 624>(); }
