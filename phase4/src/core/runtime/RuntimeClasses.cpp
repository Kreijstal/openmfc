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
// for OLE/COM/ActiveX classes with known static CRuntimeClass descriptors.
// GetThisClass returns the static descriptor for the class, and GetRuntimeClass
// returns the same virtual value without dereferencing the this pointer.

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
// These pre-existing constructor bodies are OpenMFC approximations, not full
// retail transcriptions: retail entries RVA 0x2ae800 and 0x2ae900 (mfc140u)
// construct the CCmdTarget base, initialize additional private fields, and
// register the object in the module-state list. This shard currently assigns
// the handles and its declared m_pNextDLL field at OpenMFC offset +0x50; that
// is not retail's list link at the +0x78 offset selected by the module state.
// These bodies also omit base/private-field initialization (the extension
// overload ignores bSystem), and the derived-vtable layout is not modeled here.
// Keep the existing bodies rather than silently turning these constructors
// into no-ops, but do not claim they reproduce retail behavior.
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
// Intentional stub: retail's destructor (entry RVA 0x2ae9a0, mfc140u) calls
// AfxLockGlobals(0), unlinks this from the list at m_pModuleState+0x88 via the
// internal helper (entry RVA 0x14c800, mfc140u), leaves the critical section,
// then tail-calls CCmdTarget::~CCmdTarget (entry RVA 0x1de430, mfc140u). The
// module-state constructor (entry RVA 0x1332d0, mfc140u) initializes that list
// offset to 0x78, and CDynLinkLibrary constructors use it (entries RVA 0x2ae800
// and 0x2ae900, mfc140u). OpenMFC's AFX_MODULE_STATE shadow leaves +0x88..+0x9f
// reserved, while its AFX_MODULE_STATE constructor path also uses the
// 32-byte LocalAFX_MODULE_STATE representation; pThis->m_pModuleState therefore
// cannot safely be assumed to contain a valid list at +0x88. The repo does
// provide AfxLockGlobals/AfxUnlockGlobals, so lock balancing is available; the
// unsupported part is safely identifying and unlinking from every state layout.
extern "C" void MS_ABI impl___1CDynLinkLibrary__UEAA_XZ(CDynLinkLibrary* pThis) {
    (void)pThis;
}
// Symbol: ?GetRuntimeClass@CCmdTarget@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CCmdTarget__UEBAPEAUCRuntimeClass__XZ(
        const CCmdTarget* pThis) {
    if (!pThis) return CCmdTarget::GetThisClass();
    return CCmdTarget::GetThisClass();
}
