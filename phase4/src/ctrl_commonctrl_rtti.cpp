// OpenMFC: RTTI getters (GetThisClass / GetRuntimeClass) for a batch of common
// control wrappers (16 exports, 8 classes).
//
// These are MFC DECLARE_DYNAMIC classes with no separately-exported
// CRuntimeClass data symbol — only the two getters are exported. The repo has no
// class body for them, so each gets a file-internal CRuntimeClass descriptor
// (schema 0xFFFF = DYNAMIC, no factory) whose m_pBaseClass chains to the real
// base descriptor (classCWnd / classCComboBox / classCEdit), matching the retail
// RUNTIME_CLASS graph that IsKindOf walks. m_nObjectSize is the harvested MSVC
// sizeof (all are 232 — bare CWnd/CComboBox/CEdit subclasses that add no data).
// GetRuntimeClass returns the static descriptor directly (never pThis->GetRuntime
// Class(), which would recurse through this very export in a drop-in client).

#include "openmfc/afxwin.h"   // CWnd, CComboBox, CEdit, CRuntimeClass + DECLARE_DYNAMIC

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

namespace {
// m_lpszClassName, m_nObjectSize, m_wSchema, m_pfnCreateObject,
// m_pfnGetBaseClass, m_pBaseClass, m_pNextClass.
#define CC_DESC(Cls, Base, Size) \
    CRuntimeClass class##Cls = { #Cls, (Size), 0xFFFF, nullptr, nullptr, &Base::class##Base, nullptr }

CC_DESC(CAnimateCtrl,   CWnd,      232);
CC_DESC(CHeaderCtrl,    CWnd,      232);
CC_DESC(CHotKeyCtrl,    CWnd,      232);
CC_DESC(CIPAddressCtrl, CWnd,      232);
CC_DESC(CPagerCtrl,     CWnd,      232);
CC_DESC(CLinkCtrl,      CWnd,      232);
CC_DESC(CComboBoxEx,    CComboBox, 232);
CC_DESC(CNetAddressCtrl, CEdit,    232);
#undef CC_DESC
} // namespace

// Symbol: ?GetThisClass@CAnimateCtrl@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CAnimateCtrl__SAPEAUCRuntimeClass__XZ() {
    return &classCAnimateCtrl;
}
// Symbol: ?GetRuntimeClass@CAnimateCtrl@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CAnimateCtrl__UEBAPEAUCRuntimeClass__XZ(const void*) {
    return &classCAnimateCtrl;
}

// Symbol: ?GetThisClass@CHeaderCtrl@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CHeaderCtrl__SAPEAUCRuntimeClass__XZ() {
    return &classCHeaderCtrl;
}
// Symbol: ?GetRuntimeClass@CHeaderCtrl@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CHeaderCtrl__UEBAPEAUCRuntimeClass__XZ(const void*) {
    return &classCHeaderCtrl;
}

// Symbol: ?GetThisClass@CHotKeyCtrl@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CHotKeyCtrl__SAPEAUCRuntimeClass__XZ() {
    return &classCHotKeyCtrl;
}
// Symbol: ?GetRuntimeClass@CHotKeyCtrl@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CHotKeyCtrl__UEBAPEAUCRuntimeClass__XZ(const void*) {
    return &classCHotKeyCtrl;
}

// Symbol: ?GetThisClass@CIPAddressCtrl@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CIPAddressCtrl__SAPEAUCRuntimeClass__XZ() {
    return &classCIPAddressCtrl;
}
// Symbol: ?GetRuntimeClass@CIPAddressCtrl@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CIPAddressCtrl__UEBAPEAUCRuntimeClass__XZ(const void*) {
    return &classCIPAddressCtrl;
}

// Symbol: ?GetThisClass@CPagerCtrl@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CPagerCtrl__SAPEAUCRuntimeClass__XZ() {
    return &classCPagerCtrl;
}
// Symbol: ?GetRuntimeClass@CPagerCtrl@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CPagerCtrl__UEBAPEAUCRuntimeClass__XZ(const void*) {
    return &classCPagerCtrl;
}

// Symbol: ?GetThisClass@CLinkCtrl@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CLinkCtrl__SAPEAUCRuntimeClass__XZ() {
    return &classCLinkCtrl;
}
// Symbol: ?GetRuntimeClass@CLinkCtrl@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CLinkCtrl__UEBAPEAUCRuntimeClass__XZ(const void*) {
    return &classCLinkCtrl;
}

// Symbol: ?GetThisClass@CComboBoxEx@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CComboBoxEx__SAPEAUCRuntimeClass__XZ() {
    return &classCComboBoxEx;
}
// Symbol: ?GetRuntimeClass@CComboBoxEx@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CComboBoxEx__UEBAPEAUCRuntimeClass__XZ(const void*) {
    return &classCComboBoxEx;
}

// Symbol: ?GetThisClass@CNetAddressCtrl@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CNetAddressCtrl__SAPEAUCRuntimeClass__XZ() {
    return &classCNetAddressCtrl;
}
// Symbol: ?GetRuntimeClass@CNetAddressCtrl@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CNetAddressCtrl__UEBAPEAUCRuntimeClass__XZ(const void*) {
    return &classCNetAddressCtrl;
}
