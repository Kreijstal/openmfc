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


























// Symbol: ?CreateObject@CMFCTasksPane@@SAPEAVCObject@@XZ
extern "C" CObject* MS_ABI impl__CreateObject_CMFCTasksPane__SAPEAVCObject__XZ()
{
    return new (std::nothrow) CMFCTasksPane();
}
// Symbol: ?CreateObject@CTasksPaneHistoryButton@@SAPEAVCObject@@XZ
extern "C" CObject* MS_ABI impl__CreateObject_CTasksPaneHistoryButton__SAPEAVCObject__XZ()
{ return CreateSized<CMFCToolBarMenuButton, 360>(); }
// Symbol: ?CreateObject@CTasksPaneMenuButton@@SAPEAVCObject@@XZ
extern "C" CObject* MS_ABI impl__CreateObject_CTasksPaneMenuButton__SAPEAVCObject__XZ()
{ return CreateSized<CMFCToolBarMenuButton, 304>(); }
// Symbol: ?CreateObject@CTasksPaneNavigateButton@@SAPEAVCObject@@XZ
extern "C" CObject* MS_ABI impl__CreateObject_CTasksPaneNavigateButton__SAPEAVCObject__XZ()
{ return CreateSized<CMFCToolBarButton, 136>(); }
