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

// Symbol: ?CreateObject@CMFCButton@@SAPEAVCObject@@XZ
extern "C" CObject* MS_ABI impl__CreateObject_CMFCButton__SAPEAVCObject__XZ()
{
    return new (std::nothrow) CMFCButton();
}

// Symbol: ?CreateObject@CMFCMenuBar@@SAPEAVCObject@@XZ
extern "C" CObject* MS_ABI impl__CreateObject_CMFCMenuBar__SAPEAVCObject__XZ()
{
    return new (std::nothrow) CMFCMenuBar();
}

// Symbol: ?CreateObject@CMFCPopupMenu@@SAPEAVCObject@@XZ
extern "C" CObject* MS_ABI impl__CreateObject_CMFCPopupMenu__SAPEAVCObject__XZ()
{
    return new (std::nothrow) CMFCPopupMenu();
}

// Symbol: ?CreateObject@CMFCPopupMenuBar@@SAPEAVCObject@@XZ
extern "C" CObject* MS_ABI impl__CreateObject_CMFCPopupMenuBar__SAPEAVCObject__XZ()
{
    return new (std::nothrow) CMFCPopupMenuBar();
}

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

// Symbol: ?CreateObject@CMFCTabCtrl@@SAPEAVCObject@@XZ
extern "C" CObject* MS_ABI impl__CreateObject_CMFCTabCtrl__SAPEAVCObject__XZ()
{
    return new (std::nothrow) CMFCTabCtrl();
}

// Symbol: ?CreateObject@CMFCTasksPane@@SAPEAVCObject@@XZ
extern "C" CObject* MS_ABI impl__CreateObject_CMFCTasksPane__SAPEAVCObject__XZ()
{
    return new (std::nothrow) CMFCTasksPane();
}

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
