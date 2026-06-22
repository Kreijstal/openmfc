// OpenMFC: RTTI getters (GetThisClass / GetRuntimeClass) for a second batch of
// common-control / view wrappers (14 exports, 7 classes).
//
// These are MFC DECLARE_DYNAMIC classes with no separately-exported CRuntimeClass
// data symbol — only the two getters are exported. The repo has no class body for
// them, so each gets a file-internal CRuntimeClass descriptor (schema 0xFFFF =
// DYNAMIC, no factory) whose m_pBaseClass chains to the real base descriptor
// (classCWnd / classCButton / classCView), following the repo's own
// IMPLEMENT_DYNAMIC convention (m_pfnGetBaseClass null, m_pBaseClass set) so
// IsKindOf/IsDerivedFrom walk the correct RUNTIME_CLASS graph. m_nObjectSize is
// the real MSVC sizeof harvested with cl.exe /d1reportSingleClassLayout:
//   CMonthCalCtrl 232, CReBarCtrl 232, CStatusBarCtrl 232, CToolBarCtrl 232,
//   CRichEditCtrl 232 (all CWnd), CSplitButton 248 (CButton),
//   CTabView 15880 (CView).
// GetRuntimeClass returns the static descriptor directly (never
// pThis->GetRuntimeClass(), which would recurse through this very export in a
// drop-in client).
//
// Named global_*_rtti.cpp so build_phase4.sh's shard glob compiles it.

#include "openmfc/afxwin.h"   // CWnd / CButton / CView + CRuntimeClass

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

namespace {
// m_lpszClassName, m_nObjectSize, m_wSchema, m_pfnCreateObject,
// m_pfnGetBaseClass, m_pBaseClass, m_pNextClass.
#define CC2_DESC(Cls, Base, Size) \
    CRuntimeClass class##Cls = { #Cls, (Size), 0xFFFF, nullptr, nullptr, &Base::class##Base, nullptr }

CC2_DESC(CMonthCalCtrl,  CWnd,    232);
CC2_DESC(CReBarCtrl,     CWnd,    232);
CC2_DESC(CStatusBarCtrl, CWnd,    232);
CC2_DESC(CToolBarCtrl,   CWnd,    232);
CC2_DESC(CRichEditCtrl,  CWnd,    232);
CC2_DESC(CSplitButton,   CButton, 248);
CC2_DESC(CTabView,       CView,   15880);
#undef CC2_DESC
} // namespace

// Symbol: ?GetThisClass@CMonthCalCtrl@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMonthCalCtrl__SAPEAUCRuntimeClass__XZ() {
    return &classCMonthCalCtrl;
}
// Symbol: ?GetRuntimeClass@CMonthCalCtrl@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CMonthCalCtrl__UEBAPEAUCRuntimeClass__XZ(const void*) {
    return &classCMonthCalCtrl;
}

// Symbol: ?GetThisClass@CReBarCtrl@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CReBarCtrl__SAPEAUCRuntimeClass__XZ() {
    return &classCReBarCtrl;
}
// Symbol: ?GetRuntimeClass@CReBarCtrl@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CReBarCtrl__UEBAPEAUCRuntimeClass__XZ(const void*) {
    return &classCReBarCtrl;
}

// Symbol: ?GetThisClass@CStatusBarCtrl@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CStatusBarCtrl__SAPEAUCRuntimeClass__XZ() {
    return &classCStatusBarCtrl;
}
// Symbol: ?GetRuntimeClass@CStatusBarCtrl@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CStatusBarCtrl__UEBAPEAUCRuntimeClass__XZ(const void*) {
    return &classCStatusBarCtrl;
}

// Symbol: ?GetThisClass@CToolBarCtrl@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CToolBarCtrl__SAPEAUCRuntimeClass__XZ() {
    return &classCToolBarCtrl;
}
// Symbol: ?GetRuntimeClass@CToolBarCtrl@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CToolBarCtrl__UEBAPEAUCRuntimeClass__XZ(const void*) {
    return &classCToolBarCtrl;
}

// Symbol: ?GetThisClass@CRichEditCtrl@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CRichEditCtrl__SAPEAUCRuntimeClass__XZ() {
    return &classCRichEditCtrl;
}
// Symbol: ?GetRuntimeClass@CRichEditCtrl@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CRichEditCtrl__UEBAPEAUCRuntimeClass__XZ(const void*) {
    return &classCRichEditCtrl;
}

// Symbol: ?GetThisClass@CSplitButton@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CSplitButton__SAPEAUCRuntimeClass__XZ() {
    return &classCSplitButton;
}
// Symbol: ?GetRuntimeClass@CSplitButton@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CSplitButton__UEBAPEAUCRuntimeClass__XZ(const void*) {
    return &classCSplitButton;
}

// Symbol: ?GetThisClass@CTabView@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CTabView__SAPEAUCRuntimeClass__XZ() {
    return &classCTabView;
}
// Symbol: ?GetRuntimeClass@CTabView@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CTabView__UEBAPEAUCRuntimeClass__XZ(const void*) {
    return &classCTabView;
}
