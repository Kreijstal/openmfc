// OpenMFC: RTTI getters (GetThisClass / GetRuntimeClass) for a batch of control,
// view and dialog wrappers (10 exports, 5 classes).
//
// These are MFC DECLARE_DYNAMIC classes with no separately-exported CRuntimeClass
// data symbol — only the two getters are exported. The repo has no class body for
// them, so each gets a file-internal CRuntimeClass descriptor (schema 0xFFFF =
// DYNAMIC, no factory) whose m_pBaseClass chains to the real base descriptor
// (classCWnd / classCButton / classCView / classCDialog / classCFileDialog),
// matching the retail RUNTIME_CLASS graph that IsKindOf walks. m_nObjectSize is
// the real MSVC sizeof harvested with cl.exe /d1reportSingleClassLayout:
//   CDateTimeCtrl 232 (CWnd), CBitmapButton 296 (CButton), CCtrlView 264 (CView),
//   CCommonDialog 304 (CDialog), CFolderPickerDialog 1424 (CFileDialog).
// GetRuntimeClass returns the static descriptor directly (never
// pThis->GetRuntimeClass(), which would recurse through this very export in a
// drop-in client).

#include "openmfc/afxwin.h"   // CWnd/CButton/CView/CDialog/CFileDialog + CRuntimeClass

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

namespace {
// m_lpszClassName, m_nObjectSize, m_wSchema, m_pfnCreateObject,
// m_pfnGetBaseClass, m_pBaseClass, m_pNextClass.
#define VDC_DESC(Cls, Base, Size) \
    CRuntimeClass class##Cls = { #Cls, (Size), 0xFFFF, nullptr, nullptr, &Base::class##Base, nullptr }

VDC_DESC(CDateTimeCtrl,       CWnd,        232);
VDC_DESC(CBitmapButton,       CButton,     296);
VDC_DESC(CCtrlView,           CView,       264);
VDC_DESC(CCommonDialog,       CDialog,     304);
VDC_DESC(CFolderPickerDialog, CFileDialog, 1424);
#undef VDC_DESC
} // namespace

// Symbol: ?GetThisClass@CDateTimeCtrl@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CDateTimeCtrl__SAPEAUCRuntimeClass__XZ() {
    return &classCDateTimeCtrl;
}
// Symbol: ?GetRuntimeClass@CDateTimeCtrl@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CDateTimeCtrl__UEBAPEAUCRuntimeClass__XZ(const void*) {
    return &classCDateTimeCtrl;
}

// Symbol: ?GetThisClass@CBitmapButton@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CBitmapButton__SAPEAUCRuntimeClass__XZ() {
    return &classCBitmapButton;
}
// Symbol: ?GetRuntimeClass@CBitmapButton@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CBitmapButton__UEBAPEAUCRuntimeClass__XZ(const void*) {
    return &classCBitmapButton;
}

// Symbol: ?GetThisClass@CCtrlView@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CCtrlView__SAPEAUCRuntimeClass__XZ() {
    return &classCCtrlView;
}
// Symbol: ?GetRuntimeClass@CCtrlView@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CCtrlView__UEBAPEAUCRuntimeClass__XZ(const void*) {
    return &classCCtrlView;
}

// Symbol: ?GetThisClass@CCommonDialog@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CCommonDialog__SAPEAUCRuntimeClass__XZ() {
    return &classCCommonDialog;
}
// Symbol: ?GetRuntimeClass@CCommonDialog@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CCommonDialog__UEBAPEAUCRuntimeClass__XZ(const void*) {
    return &classCCommonDialog;
}

// Symbol: ?GetThisClass@CFolderPickerDialog@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CFolderPickerDialog__SAPEAUCRuntimeClass__XZ() {
    return &classCFolderPickerDialog;
}
// Symbol: ?GetRuntimeClass@CFolderPickerDialog@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CFolderPickerDialog__UEBAPEAUCRuntimeClass__XZ(const void*) {
    return &classCFolderPickerDialog;
}
