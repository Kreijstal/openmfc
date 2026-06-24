// OpenMFC: RTTI getters for 6 classes whose direct base has no referenceable
// member-static descriptor (the base descriptor is file-internal in another TU).
// Instead of m_pBaseClass (a compile-time pointer we cannot form), each descriptor
// sets m_pfnGetBaseClass to the base's own exported GetThisClass — the real-MFC
// _AFXDLL cross-module base-resolution mechanism — so IsKindOf/IsDerivedFrom walk
// an unbroken, identity-consistent chain. Real m_nObjectSize/m_wSchema/base were
// harvested from mfc140u.dll. GetRuntimeClass returns the static descriptor (no
// self-dispatch). Named global_*_rtti.cpp so build_phase4.sh compiles it.

#include "openmfc/afx.h"   // CRuntimeClass, AFXAPI

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

// Base getters exported elsewhere in this DLL (all verified to return real descriptors).
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CBaseTabbedPane__SAPEAUCRuntimeClass__XZ();
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CDockSite__SAPEAUCRuntimeClass__XZ();
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CDockablePaneAdapter__SAPEAUCRuntimeClass__XZ();
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CHeaderCtrl__SAPEAUCRuntimeClass__XZ();
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CInternetFile__SAPEAUCRuntimeClass__XZ();
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CToolTipCtrl__SAPEAUCRuntimeClass__XZ();

namespace {
// __cdecl (AFXAPI) thunks matching m_pfnGetBaseClass exactly; each defers to the
// base's exported GetThisClass.
CRuntimeClass* AFXAPI gb_CAutoHideDockSite() { return impl__GetThisClass_CDockSite__SAPEAUCRuntimeClass__XZ(); }
CRuntimeClass* AFXAPI gb_CGopherFile() { return impl__GetThisClass_CInternetFile__SAPEAUCRuntimeClass__XZ(); }
CRuntimeClass* AFXAPI gb_CMFCHeaderCtrl() { return impl__GetThisClass_CHeaderCtrl__SAPEAUCRuntimeClass__XZ(); }
CRuntimeClass* AFXAPI gb_CMFCOutlookBar() { return impl__GetThisClass_CBaseTabbedPane__SAPEAUCRuntimeClass__XZ(); }
CRuntimeClass* AFXAPI gb_CMFCOutlookBarPaneAdapter() { return impl__GetThisClass_CDockablePaneAdapter__SAPEAUCRuntimeClass__XZ(); }
CRuntimeClass* AFXAPI gb_CMFCToolTipCtrl() { return impl__GetThisClass_CToolTipCtrl__SAPEAUCRuntimeClass__XZ(); }

// m_lpszClassName, m_nObjectSize, m_wSchema, m_pfnCreateObject, m_pfnGetBaseClass,
// m_pBaseClass(null), m_pNextClass(null).
#define DEF_DESC(Cls, Size, Schema, GB) \
    CRuntimeClass class##Cls = { #Cls, (Size), (Schema), nullptr, GB, nullptr, nullptr }
DEF_DESC(CAutoHideDockSite, 552, 0xFFFF, gb_CAutoHideDockSite);
DEF_DESC(CGopherFile, 152, 0xFFFF, gb_CGopherFile);
DEF_DESC(CMFCHeaderCtrl, 320, 0xFFFF, gb_CMFCHeaderCtrl);
DEF_DESC(CMFCOutlookBar, 1384, 0x00000001, gb_CMFCOutlookBar);
DEF_DESC(CMFCOutlookBarPaneAdapter, 1280, 0x80000002, gb_CMFCOutlookBarPaneAdapter);
DEF_DESC(CMFCToolTipCtrl, 424, 0xFFFF, gb_CMFCToolTipCtrl);
#undef DEF_DESC
} // namespace

#define DEF_GETTERS(Cls, GT, GRC) \
    extern "C" CRuntimeClass* MS_ABI GT() { return &class##Cls; } \
    extern "C" CRuntimeClass* MS_ABI GRC(const void*) { return &class##Cls; }

// Symbol: ?GetThisClass@CAutoHideDockSite@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CAutoHideDockSite@@UEBAPEAUCRuntimeClass@@XZ
DEF_GETTERS(CAutoHideDockSite,
            impl__GetThisClass_CAutoHideDockSite__SAPEAUCRuntimeClass__XZ,
            impl__GetRuntimeClass_CAutoHideDockSite__UEBAPEAUCRuntimeClass__XZ)
// Symbol: ?GetThisClass@CGopherFile@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CGopherFile@@UEBAPEAUCRuntimeClass@@XZ
DEF_GETTERS(CGopherFile,
            impl__GetThisClass_CGopherFile__SAPEAUCRuntimeClass__XZ,
            impl__GetRuntimeClass_CGopherFile__UEBAPEAUCRuntimeClass__XZ)
// Symbol: ?GetThisClass@CMFCHeaderCtrl@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CMFCHeaderCtrl@@UEBAPEAUCRuntimeClass@@XZ
DEF_GETTERS(CMFCHeaderCtrl,
            impl__GetThisClass_CMFCHeaderCtrl__SAPEAUCRuntimeClass__XZ,
            impl__GetRuntimeClass_CMFCHeaderCtrl__UEBAPEAUCRuntimeClass__XZ)
// Symbol: ?GetThisClass@CMFCOutlookBar@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CMFCOutlookBar@@UEBAPEAUCRuntimeClass@@XZ
DEF_GETTERS(CMFCOutlookBar,
            impl__GetThisClass_CMFCOutlookBar__SAPEAUCRuntimeClass__XZ,
            impl__GetRuntimeClass_CMFCOutlookBar__UEBAPEAUCRuntimeClass__XZ)
// Symbol: ?GetThisClass@CMFCOutlookBarPaneAdapter@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CMFCOutlookBarPaneAdapter@@UEBAPEAUCRuntimeClass@@XZ
DEF_GETTERS(CMFCOutlookBarPaneAdapter,
            impl__GetThisClass_CMFCOutlookBarPaneAdapter__SAPEAUCRuntimeClass__XZ,
            impl__GetRuntimeClass_CMFCOutlookBarPaneAdapter__UEBAPEAUCRuntimeClass__XZ)
// Symbol: ?GetThisClass@CMFCToolTipCtrl@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CMFCToolTipCtrl@@UEBAPEAUCRuntimeClass@@XZ
DEF_GETTERS(CMFCToolTipCtrl,
            impl__GetThisClass_CMFCToolTipCtrl__SAPEAUCRuntimeClass__XZ,
            impl__GetRuntimeClass_CMFCToolTipCtrl__UEBAPEAUCRuntimeClass__XZ)
#undef DEF_GETTERS
