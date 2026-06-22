#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxmfc.h"
#include "openmfc/afxole.h"

#include <cstring>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

// Symbol: ?GetThisClass@CToolBar@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CToolBar__SAPEAUCRuntimeClass__XZ() {
    return CToolBar::GetThisClass();
}

// Symbol: ?GetRuntimeClass@CToolBar@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CToolBar__UEBAPEAUCRuntimeClass__XZ(
    const CToolBar* pThis) {
    return CToolBar::GetThisClass();
}

// Symbol: ?GetThisClass@CVSListBox@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CVSListBox__SAPEAUCRuntimeClass__XZ() {
    return CVSListBox::GetThisClass();
}

// Symbol: ?GetRuntimeClass@CVSListBox@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CVSListBox__UEBAPEAUCRuntimeClass__XZ(
    const CVSListBox* pThis) {
    return CVSListBox::GetThisClass();
}

// Symbol: ?GetThisClass@CVSListBoxBase@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CVSListBoxBase__SAPEAUCRuntimeClass__XZ() {
    return CVSListBoxBase::GetThisClass();
}

// Symbol: ?GetRuntimeClass@CVSListBoxBase@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CVSListBoxBase__UEBAPEAUCRuntimeClass__XZ(
    const CVSListBoxBase* pThis) {
    return CVSListBoxBase::GetThisClass();
}

// Symbol: ?GetThisClass@CVSListBoxEditCtrl@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CVSListBoxEditCtrl__SAPEAUCRuntimeClass__XZ() {
    return CVSListBoxEditCtrl::GetThisClass();
}

// Symbol: ?GetRuntimeClass@CVSListBoxEditCtrl@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CVSListBoxEditCtrl__UEBAPEAUCRuntimeClass__XZ(
    const CVSListBoxEditCtrl* pThis) {
    return CVSListBoxEditCtrl::GetThisClass();
}
