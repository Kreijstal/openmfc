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

// --- CDialogBar (IMPLEMENT_DYNAMIC(CDialogBar, CControlBar) in cbarcore.cpp) ---

// Symbol: ?GetRuntimeClass@CDialogBar@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CDialogBar__UEBAPEAUCRuntimeClass__XZ(const CDialogBar*) {
    return CDialogBar::GetThisClass();
}

// Symbol: ?GetThisClass@CDialogBar@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CDialogBar__SAPEAUCRuntimeClass__XZ() {
    return CDialogBar::GetThisClass();
}

// --- CDocItem (IMPLEMENT_DYNAMIC(CDocItem, CObject) in olecore.cpp) ---

// Symbol: ?GetRuntimeClass@CDocItem@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CDocItem__UEBAPEAUCRuntimeClass__XZ(const CDocItem*) {
    return CDocItem::GetThisClass();
}

// Symbol: ?GetThisClass@CDocItem@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CDocItem__SAPEAUCRuntimeClass__XZ() {
    return CDocItem::GetThisClass();
}

// CDocItem::IsBlank() is a virtual returning BOOL; the MFC base default is FALSE.
// Symbol: ?IsBlank@CDocItem@@UEBAHXZ
extern "C" int MS_ABI impl__IsBlank_CDocItem__UEBAHXZ(const CDocItem*) {
    return 0;
}

// --- CDragListBox (IMPLEMENT_DYNAMIC(CDragListBox, CListBox) in ctrlcore.cpp) ---

// Symbol: ?GetRuntimeClass@CDragListBox@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CDragListBox__UEBAPEAUCRuntimeClass__XZ(const CDragListBox*) {
    return CDragListBox::GetThisClass();
}

// Symbol: ?GetThisClass@CDragListBox@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CDragListBox__SAPEAUCRuntimeClass__XZ() {
    return CDragListBox::GetThisClass();
}

// --- CEdit (IMPLEMENT_DYNAMIC(CEdit, CWnd) in ctrlcore.cpp) ---

// Symbol: ?GetRuntimeClass@CEdit@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CEdit__UEBAPEAUCRuntimeClass__XZ(const CEdit*) {
    return CEdit::GetThisClass();
}

// Symbol: ?GetThisClass@CEdit@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CEdit__SAPEAUCRuntimeClass__XZ() {
    return CEdit::GetThisClass();
}
