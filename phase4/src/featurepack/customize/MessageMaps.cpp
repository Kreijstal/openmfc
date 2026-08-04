// MessageMaps — OpenMFC implementation.
// Sources: global_mfc_02_msgmap.cpp, global_mfc_04_msgmap.cpp, global_mfc_05_msgmap.cpp, global_mfc_08_msgmap.cpp, global_toolbar_22_msgmap.cpp, global_toolbar_23_msgmap.cpp

#include "detail/Mfc02MsgmapSupport.h"
#include "detail/Mfc04MsgmapSupport.h"
#include "detail/Mfc05MsgmapSupport.h"
#include "detail/Mfc08MsgmapSupport.h"
#include "detail/Toolbar22MsgmapSupport.h"
#include "detail/Toolbar23MsgmapSupport.h"

#define DEF_MM(Cls, GMSym, GTMSym) \
    extern "C" const AFX_MSGMAP* MS_ABI GMSym(const void*) { return &class##Cls##_msgmap; } \
    extern "C" const AFX_MSGMAP* MS_ABI GTMSym() { return &class##Cls##_msgmap; }
// Symbol: ?GetMessageMap@CMFCAcceleratorKeyAssignCtrl@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CMFCAcceleratorKeyAssignCtrl@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CMFCAcceleratorKeyAssignCtrl,
       impl__GetMessageMap_CMFCAcceleratorKeyAssignCtrl__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CMFCAcceleratorKeyAssignCtrl__KAPEBUAFX_MSGMAP__XZ)
#undef DEF_MM
#define DEF_MM(Cls, GMSym, GTMSym) \
    extern "C" const AFX_MSGMAP* MS_ABI GMSym(const void*) { return &class##Cls##_msgmap; } \
    extern "C" const AFX_MSGMAP* MS_ABI GTMSym() { return &class##Cls##_msgmap; }
// Symbol: ?GetMessageMap@CMFCKeyMapDialog@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CMFCKeyMapDialog@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CMFCKeyMapDialog,
       impl__GetMessageMap_CMFCKeyMapDialog__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CMFCKeyMapDialog__KAPEBUAFX_MSGMAP__XZ)
#undef DEF_MM
#undef DEF_MM
#define DEF_MM(Cls, GMSym, GTMSym) \
    extern "C" const AFX_MSGMAP* MS_ABI GMSym(const void*) { return &class##Cls##_msgmap; } \
    extern "C" const AFX_MSGMAP* MS_ABI GTMSym() { return &class##Cls##_msgmap; }
// Symbol: ?GetMessageMap@CMFCMousePropertyPage@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CMFCMousePropertyPage@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CMFCMousePropertyPage,
       impl__GetMessageMap_CMFCMousePropertyPage__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CMFCMousePropertyPage__KAPEBUAFX_MSGMAP__XZ)
#undef DEF_MM
#define DEF_MM(Cls, GMSym, GTMSym) \
    extern "C" const AFX_MSGMAP* MS_ABI GMSym(const void*) { return &class##Cls##_msgmap; } \
    extern "C" const AFX_MSGMAP* MS_ABI GTMSym() { return &class##Cls##_msgmap; }
// Symbol: ?GetMessageMap@CMFCWindowsManagerDialog@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CMFCWindowsManagerDialog@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CMFCWindowsManagerDialog,
       impl__GetMessageMap_CMFCWindowsManagerDialog__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CMFCWindowsManagerDialog__KAPEBUAFX_MSGMAP__XZ)
#undef DEF_MM
#undef DEF_MM
#define DEF_MM(Cls, GMSym, GTMSym) \
    extern "C" const AFX_MSGMAP* MS_ABI GMSym(const void*) { return &class##Cls##_msgmap; } \
    extern "C" const AFX_MSGMAP* MS_ABI GTMSym() { return &class##Cls##_msgmap; }
// Symbol: ?GetMessageMap@CMFCToolBarsCommandsListBox@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CMFCToolBarsCommandsListBox@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CMFCToolBarsCommandsListBox,
       impl__GetMessageMap_CMFCToolBarsCommandsListBox__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CMFCToolBarsCommandsListBox__KAPEBUAFX_MSGMAP__XZ)
// Symbol: ?GetMessageMap@CMFCToolBarsCommandsPropertyPage@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CMFCToolBarsCommandsPropertyPage@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CMFCToolBarsCommandsPropertyPage,
       impl__GetMessageMap_CMFCToolBarsCommandsPropertyPage__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CMFCToolBarsCommandsPropertyPage__KAPEBUAFX_MSGMAP__XZ)
// Symbol: ?GetMessageMap@CMFCToolBarsCustomizeDialog@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CMFCToolBarsCustomizeDialog@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CMFCToolBarsCustomizeDialog,
       impl__GetMessageMap_CMFCToolBarsCustomizeDialog__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CMFCToolBarsCustomizeDialog__KAPEBUAFX_MSGMAP__XZ)
// Symbol: ?GetMessageMap@CMFCToolBarsKeyboardPropertyPage@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CMFCToolBarsKeyboardPropertyPage@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CMFCToolBarsKeyboardPropertyPage,
       impl__GetMessageMap_CMFCToolBarsKeyboardPropertyPage__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CMFCToolBarsKeyboardPropertyPage__KAPEBUAFX_MSGMAP__XZ)
// Symbol: ?GetMessageMap@CMFCToolBarsListCheckBox@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CMFCToolBarsListCheckBox@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CMFCToolBarsListCheckBox,
       impl__GetMessageMap_CMFCToolBarsListCheckBox__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CMFCToolBarsListCheckBox__KAPEBUAFX_MSGMAP__XZ)
// Symbol: ?GetMessageMap@CMFCToolBarsListPropertyPage@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CMFCToolBarsListPropertyPage@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CMFCToolBarsListPropertyPage,
       impl__GetMessageMap_CMFCToolBarsListPropertyPage__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CMFCToolBarsListPropertyPage__KAPEBUAFX_MSGMAP__XZ)
// Symbol: ?GetMessageMap@CMFCToolBarsMenuPropertyPage@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CMFCToolBarsMenuPropertyPage@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CMFCToolBarsMenuPropertyPage,
       impl__GetMessageMap_CMFCToolBarsMenuPropertyPage__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CMFCToolBarsMenuPropertyPage__KAPEBUAFX_MSGMAP__XZ)
#undef DEF_MM
#undef DEF_MM
#define DEF_MM(Cls, GMSym, GTMSym) \
    extern "C" const AFX_MSGMAP* MS_ABI GMSym(const void*) { return &class##Cls##_msgmap; } \
    extern "C" const AFX_MSGMAP* MS_ABI GTMSym() { return &class##Cls##_msgmap; }
// Symbol: ?GetMessageMap@CMFCToolBarsOptionsPropertyPage@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CMFCToolBarsOptionsPropertyPage@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CMFCToolBarsOptionsPropertyPage,
       impl__GetMessageMap_CMFCToolBarsOptionsPropertyPage__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CMFCToolBarsOptionsPropertyPage__KAPEBUAFX_MSGMAP__XZ)
// Symbol: ?GetMessageMap@CMFCToolBarsToolsPropertyPage@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CMFCToolBarsToolsPropertyPage@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CMFCToolBarsToolsPropertyPage,
       impl__GetMessageMap_CMFCToolBarsToolsPropertyPage__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CMFCToolBarsToolsPropertyPage__KAPEBUAFX_MSGMAP__XZ)
#undef DEF_MM
#undef DEF_MM
