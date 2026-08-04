// RuntimeClasses — OpenMFC implementation.
// Sources: global_oleresidual_rtti.cpp, ole_clientitem_dialogs_rtti.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/COleControlModuleSupport.h"

#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxmfc.h"

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

// This shard implements MFC RTTI accessors (GetThisClass / GetRuntimeClass)
// for OLE/COM/ActiveX classes that are fully defined in the OpenMFC headers
// with DECLARE_DYNAMIC + IMPLEMENT_DYNAMIC. These are unambiguous: GetThisClass
// returns the static CRuntimeClass for the class, and GetRuntimeClass returns
// the same (virtual) value. We null-guard the virtual variants.

//-----------------------------------------------------------------------------
// CCmdTarget
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// COleBusyDialog
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// COleChangeIconDialog
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// COleChangeSourceDialog
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// COleClientItem
//-----------------------------------------------------------------------------

#define OR_GETTERS(Cls, GetThisSym, GetRtSym) \
    extern "C" CRuntimeClass* MS_ABI GetThisSym() { return &class##Cls; } \
    extern "C" CRuntimeClass* MS_ABI GetRtSym(const void*) { return &class##Cls; }
// Symbol: ?GetThisClass@CDynLinkLibrary@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CDynLinkLibrary@@UEBAPEAUCRuntimeClass@@XZ
OR_GETTERS(CDynLinkLibrary,
           impl__GetThisClass_CDynLinkLibrary__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CDynLinkLibrary__UEBAPEAUCRuntimeClass__XZ)

#undef OR_GETTERS
// Symbol: ??0CDynLinkLibrary@@QEAA@AEAUAFX_EXTENSION_MODULE@@H@Z
extern "C" CDynLinkLibrary* MS_ABI impl___0CDynLinkLibrary__QEAA_AEAUAFX_EXTENSION_MODULE__H_Z(
    CDynLinkLibrary* pThis, AFX_EXTENSION_MODULE& state, int bSystem) {
    (void)bSystem;
    pThis->m_hModule   = state.hModule;
    pThis->m_hResource = state.hResource;
    pThis->m_pNextDLL  = nullptr;
    return pThis;
}
// Symbol: ??0CDynLinkLibrary@@QEAA@PEAUHINSTANCE__@@0@Z
extern "C" CDynLinkLibrary* MS_ABI impl___0CDynLinkLibrary__QEAA_PEAUHINSTANCE____0_Z(
    CDynLinkLibrary* pThis, HINSTANCE hModule, HINSTANCE hResource) {
    pThis->m_hModule   = hModule;
    pThis->m_hResource = hResource;
    pThis->m_pNextDLL  = nullptr;
    return pThis;
}
// Symbol: ??1CDynLinkLibrary@@UEAA@XZ
extern "C" void MS_ABI impl___1CDynLinkLibrary__UEAA_XZ(CDynLinkLibrary* pThis) {
    (void)pThis;  // no-op: OpenMFC AFX_MODULE_STATE has no library list
}
// Symbol: ?GetRuntimeClass@CCmdTarget@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CCmdTarget__UEBAPEAUCRuntimeClass__XZ(
        const CCmdTarget* pThis) {
    if (!pThis) return CCmdTarget::GetThisClass();
    return CCmdTarget::GetThisClass();
}
