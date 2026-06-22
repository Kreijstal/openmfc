// other_mfc shard 03/08 real exports.
//
// Implements the unambiguous RTTI accessors (GetThisClass / GetRuntimeClass)
// for classes whose CRuntimeClass is already defined via IMPLEMENT_DYNAMIC in
// the repo, plus a couple of trivial virtuals with well-defined defaults.

#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxole.h"

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

// Symbol: ?GetRuntimeClass@CDialogBar@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CDialogBar__UEBAPEAUCRuntimeClass__XZ(const CDialogBar*) {
    return CDialogBar::GetThisClass();
}

// Symbol: ?GetThisClass@CDialogBar@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CDialogBar__SAPEAUCRuntimeClass__XZ() {
    return CDialogBar::GetThisClass();
}

// Symbol: ?GetRuntimeClass@CDocItem@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CDocItem__UEBAPEAUCRuntimeClass__XZ(const CDocItem*) {
    return CDocItem::GetThisClass();
}

// Symbol: ?GetThisClass@CDocItem@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CDocItem__SAPEAUCRuntimeClass__XZ() {
    return CDocItem::GetThisClass();
}

// Symbol: ?GetRuntimeClass@CDragListBox@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CDragListBox__UEBAPEAUCRuntimeClass__XZ(const CDragListBox*) {
    return CDragListBox::GetThisClass();
}

// Symbol: ?GetThisClass@CDragListBox@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CDragListBox__SAPEAUCRuntimeClass__XZ() {
    return CDragListBox::GetThisClass();
}

// Symbol: ?GetRuntimeClass@CEdit@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CEdit__UEBAPEAUCRuntimeClass__XZ(const CEdit*) {
    return CEdit::GetThisClass();
}

// Symbol: ?GetThisClass@CEdit@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CEdit__SAPEAUCRuntimeClass__XZ() {
    return CEdit::GetThisClass();
}
