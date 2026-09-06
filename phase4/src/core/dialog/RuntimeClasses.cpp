// RuntimeClasses — OpenMFC implementation.
// Sources: global_common_control_rtti.cpp, global_pane_dialog_rtti.cpp, global_taskdialog_dc_rtti.cpp, global_view_dialog_ctrl_rtti.cpp

#define OPENMFC_APPCORE_IMPL

#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxmfc.h"
#include "openmfc/afxinet.h"
#include "openmfc/afxdao.h"

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

// Destructors for classes whose repo header definitions faithfully model the
// real MFC ABI (thin CWnd/CDialog control wrappers with no extra owning state,
// plus a few value/handle-owning classes with explicit cleanup logic).
//
// Each impl simply invokes the C++ destructor, which performs exactly the work
// the real MFC destructor does (chaining to the base destructor and freeing any
// owned members).  All are null-guarded.
//
// Placeholder stub classes that are only modelled as `char _pad[N]` (uncertain
// real layout) are intentionally left with their weak stubs.

// ---- Thin Win32 control wrappers (CWnd-derived, defaulted destructors) ----
















// ---- Thin CDialog-derived wrappers (defaulted destructors) ----





// ---- Handle/value-owning classes with explicit cleanup ----



#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxmfc.h"

#include <cstring>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif


// OpenMFC: RTTI getters (GetThisClass / GetRuntimeClass) for 2 classes.
// File-internal CRuntimeClass descriptors; real m_nObjectSize / m_wSchema harvested
// from mfc140u.dll (tools/harvest). GetRuntimeClass returns the static descriptor
// directly (no self-dispatch). // Symbol: markers auto-exclude the weak stubs.
// Named global_*_rtti.cpp so build_phase4.sh's shard glob compiles it.

#include "openmfc/afx.h"
#include "openmfc/afxwin.h"
#include "openmfc/afxmfc.h"

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

// m_lpszClassName, m_nObjectSize, m_wSchema, m_pfnCreateObject,
// m_pfnGetBaseClass, m_pNextClass, m_pClassInit.
#define TASKDIALOG_DC_DESC(Cls, Size, Schema, BaseDesc) \
    static CRuntimeClass* AFXAPI _openmfc_gb_##Cls() { return (BaseDesc); } \
    CRuntimeClass class##Cls = { #Cls, (Size), (Schema), nullptr, &_openmfc_gb_##Cls, nullptr, nullptr }
TASKDIALOG_DC_DESC(CTaskDialog, 240, 0xFFFF, &CObject::classCObject);
TASKDIALOG_DC_DESC(CWindowlessDC, 40, 0xFFFF, &CDC::classCDC);
#undef TASKDIALOG_DC_DESC

#define TASKDIALOG_DC_GETTERS(Cls, GetThisSym, GetRtSym) \
    extern "C" CRuntimeClass* MS_ABI GetThisSym() { return &class##Cls; } \
    extern "C" CRuntimeClass* MS_ABI GetRtSym(const void*) { return &class##Cls; }

// OpenMFC: RTTI getters (GetThisClass / GetRuntimeClass) for a batch of control,
// view and dialog wrappers (10 exports, 5 classes).
//
// These are MFC DECLARE_DYNAMIC classes with no separately-exported CRuntimeClass
// data symbol — only the two getters are exported. The repo has no class body for
// them, so each gets a file-internal CRuntimeClass descriptor (schema 0xFFFF =
// DYNAMIC, no factory) whose the base-class link chains to the real base descriptor
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

// m_lpszClassName, m_nObjectSize, m_wSchema, m_pfnCreateObject,
// m_pfnGetBaseClass, m_pNextClass, m_pClassInit.
#define VDC_DESC(Cls, Base, Size) \
    static CRuntimeClass* AFXAPI _openmfc_gb_##Cls() { return &Base::class##Base; } \
    CRuntimeClass class##Cls = { #Cls, (Size), 0xFFFF, nullptr, &_openmfc_gb_##Cls, nullptr, nullptr }

VDC_DESC(CDateTimeCtrl,       CWnd,        232);
VDC_DESC(CBitmapButton,       CButton,     296);
VDC_DESC(CCtrlView,           CView,       264);
VDC_DESC(CCommonDialog,       CDialog,     304);
VDC_DESC(CFolderPickerDialog, CFileDialog, 1424);
#undef VDC_DESC





// Symbol: ??1CFileDialog@@UEAA@XZ
extern "C" void MS_ABI impl___1CFileDialog__UEAA_XZ(CFileDialog* pThis) {
    if (pThis) pThis->~CFileDialog();
}
// Symbol: ??1CFolderPickerDialog@@UEAA@XZ
extern "C" void MS_ABI impl___1CFolderPickerDialog__UEAA_XZ(CFileDialog* pThis) {
    if (pThis) pThis->~CFileDialog();
}
// Symbol: ??1CPropertyPage@@UEAA@XZ
extern "C" void MS_ABI impl___1CPropertyPage__UEAA_XZ(CPropertyPage* pThis) {
    if (pThis) pThis->~CPropertyPage();
}
// Symbol: ??1CPropertySheet@@UEAA@XZ
extern "C" void MS_ABI impl___1CPropertySheet__UEAA_XZ(CPropertySheet* pThis) {
    if (pThis) pThis->~CPropertySheet();
}
// Symbol: ?GetThisClass@CPrintDialogEx@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CPrintDialogEx__SAPEAUCRuntimeClass__XZ() {
    return CPrintDialogEx::GetThisClass();
}
// Symbol: ?GetRuntimeClass@CPrintDialogEx@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CPrintDialogEx__UEBAPEAUCRuntimeClass__XZ(
    const CPrintDialogEx* pThis) {
    return CPrintDialogEx::GetThisClass();
}
#define TASKDIALOG_DC_GETTERS(Cls, GetThisSym, GetRtSym) \
    extern "C" CRuntimeClass* MS_ABI GetThisSym() { return &class##Cls; } \
    extern "C" CRuntimeClass* MS_ABI GetRtSym(const void*) { return &class##Cls; }
// Symbol: ?GetThisClass@CTaskDialog@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CTaskDialog@@UEBAPEAUCRuntimeClass@@XZ
TASKDIALOG_DC_GETTERS(CTaskDialog,
           impl__GetThisClass_CTaskDialog__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CTaskDialog__UEBAPEAUCRuntimeClass__XZ)
#undef TASKDIALOG_DC_GETTERS
// Symbol: ?GetThisClass@CCommonDialog@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CCommonDialog__SAPEAUCRuntimeClass__XZ() {
    return &classCCommonDialog;
}
// Symbol: ?GetRuntimeClass@CCommonDialog@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CCommonDialog__UEBAPEAUCRuntimeClass__XZ(const void*) {
    return &classCCommonDialog;
}
// Implemented in manual_small_stub_implementations.cpp; this unit had a second, unlinked copy.
// Symbol: ?GetThisClass@CFolderPickerDialog@@SAPEAUCRuntimeClass@@XZ
// Implemented in manual_small_stub_implementations.cpp; this unit had a second, unlinked copy.
// Symbol: ?GetRuntimeClass@CFolderPickerDialog@@UEBAPEAUCRuntimeClass@@XZ
