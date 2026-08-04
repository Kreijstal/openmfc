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


























// Symbol: ?CreateObject@CMFCToolBar@@SAPEAVCObject@@XZ
extern "C" CObject* MS_ABI impl__CreateObject_CMFCToolBar__SAPEAVCObject__XZ()
{
    return new (std::nothrow) CMFCToolBar();
}
// Symbol: ?CreateObject@CMFCToolBarButton@@SAPEAVCObject@@XZ
extern "C" CObject* MS_ABI impl__CreateObject_CMFCToolBarButton__SAPEAVCObject__XZ()
{
    return new (std::nothrow) CMFCToolBarButton();
}
// Symbol: ?CreateObject@CMFCToolBarComboBoxButton@@SAPEAVCObject@@XZ
extern "C" CObject* MS_ABI impl__CreateObject_CMFCToolBarComboBoxButton__SAPEAVCObject__XZ()
{
    return new (std::nothrow) CMFCToolBarComboBoxButton();
}
// Symbol: ?CreateObject@CMFCToolBarEditBoxButton@@SAPEAVCObject@@XZ
extern "C" CObject* MS_ABI impl__CreateObject_CMFCToolBarEditBoxButton__SAPEAVCObject__XZ()
{
    return new (std::nothrow) CMFCToolBarEditBoxButton();
}
// Symbol: ?CreateObject@CMFCToolBarMenuButton@@SAPEAVCObject@@XZ
extern "C" CObject* MS_ABI impl__CreateObject_CMFCToolBarMenuButton__SAPEAVCObject__XZ()
{
    return new (std::nothrow) CMFCToolBarMenuButton();
}
// Symbol: ?CreateObject@CMFCToolBarMenuButtonsButton@@SAPEAVCObject@@XZ
extern "C" CObject* MS_ABI impl__CreateObject_CMFCToolBarMenuButtonsButton__SAPEAVCObject__XZ()
{
    return new (std::nothrow) CMFCToolBarMenuButtonsButton();
}
// Symbol: ?CreateObject@CHelpComboBoxButton@@SAPEAVCObject@@XZ
extern "C" CObject* MS_ABI impl__CreateObject_CHelpComboBoxButton__SAPEAVCObject__XZ()
{ return CreateSized<CMFCToolBarComboBoxButton, 344>(); }
