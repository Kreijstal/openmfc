#pragma once
// Shared internals of the former global_oleresidual_rtti.cpp translation unit.
// Definitions live in detail/COleControlModuleSupport.cpp, so file-scope state keeps a
// single instance across the classes this unit was split into.
namespace openmfc { namespace detail { namespace colecontrolmodule {} } }
using namespace openmfc::detail::colecontrolmodule;
// OpenMFC: RTTI getters (GetThisClass / GetRuntimeClass) for a batch of OLE/HTML
// residual classes (16 exports, 8 classes).
//
// These are MFC classes with no separately-exported CRuntimeClass data symbol —
// only the two getters are exported. The repo has no class body for them, so each
// gets a file-internal CRuntimeClass descriptor following the repo's IMPLEMENT_*
// convention (the base link is the generated m_pfnGetBaseClass thunk). m_nObjectSize and m_wSchema are the real values read
// from mfc140u.dll (compile against mfc140u.lib, call the exported getter under
// Wine, read the raw descriptor): most are DECLARE_DYNAMIC/DYNCREATE (schema
// 0xFFFF), but CMouseManager and CUserTool are DECLARE_SERIAL and carry real schema
// versions (0x00000001 and 0x80000001 = VERSIONABLE_SCHEMA|1). m_pfnCreateObject is
// left null even for the DYNCREATE/SERIAL classes — OpenMFC has no class body to
// manufacture instances, and null honestly signals "not constructible".
//
// Each the base-class link chains to a descriptor consistent with the base's exported
// GetThisClass: CCmdTarget::classCCmdTarget / CWinApp::classCWinApp (appcore.cpp),
// CObject::classCObject (afx.h), CDialog::classCDialog (dlgcore.cpp), and the
// in-file CDHtmlDialog descriptor. GetRuntimeClass returns the static descriptor
// directly (never pThis->GetRuntimeClass(), which would recurse through this export).
//
// Named global_*_rtti.cpp so build_phase4.sh's shard glob compiles it.

#include "openmfc/afxwin.h"   // CCmdTarget/CWinApp/CDialog/CObject + CRuntimeClass
#include "openmfc/afxole.h"   // COleControlModule class definition

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

// m_lpszClassName, m_nObjectSize, m_wSchema, m_pfnCreateObject,
// m_pfnGetBaseClass, m_pNextClass, m_pClassInit.
#define OR_DESC(Cls, Size, Schema, BaseDesc) \
    static CRuntimeClass* AFXAPI _openmfc_gb_##Cls() { return (BaseDesc); } \
    CRuntimeClass class##Cls = { #Cls, (Size), (Schema), nullptr, &_openmfc_gb_##Cls, nullptr, nullptr }

// CDHtmlDialog before CMultiPageDHtmlDialog so the latter can take its address.
OR_DESC(CDHtmlDialog,          664, 0xFFFF,     &CDialog::classCDialog);          // DECLARE_DYNAMIC
OR_DESC(CMultiPageDHtmlDialog, 672, 0xFFFF,     &classCDHtmlDialog);              // DECLARE_DYNAMIC
OR_DESC(CDocObjectServer,      144, 0xFFFF,     &CCmdTarget::classCCmdTarget);    // DECLARE_DYNAMIC
OR_DESC(CDynLinkLibrary,       128, 0xFFFF,     &CCmdTarget::classCCmdTarget);    // DECLARE_DYNAMIC
OR_DESC(COleControlModule,     376, 0xFFFF,     &CWinApp::classCWinApp);          // DECLARE_DYNAMIC
OR_DESC(CSettingsStore,        56,  0xFFFF,     &CObject::classCObject);          // DECLARE_DYNCREATE
OR_DESC(CMouseManager,         176, 0x00000001, &CObject::classCObject);          // DECLARE_SERIAL, schema 1
OR_DESC(CUserTool,             56,  0x80000001, &CObject::classCObject);          // DECLARE_SERIAL, VERSIONABLE|1
#undef OR_DESC

// Two exported getters per class via a macro; the literal "// Symbol:" marker
// lines stay in the source so build_phase4.sh's grep auto-excludes the weak stubs.
#define OR_GETTERS(Cls, GetThisSym, GetRtSym) \
    extern "C" CRuntimeClass* MS_ABI GetThisSym() { return &class##Cls; } \
    extern "C" CRuntimeClass* MS_ABI GetRtSym(const void*) { return &class##Cls; }



//=============================================================================
// CDynLinkLibrary - MFC dynamic-link library registration
// Object size: 128 bytes (OR_DESC above). OpenMFC's AFX_MODULE_STATE has no
// library list, so constructors only store handles; dtor is a no-op.
//=============================================================================

// Real MFC afxext.h: AFX_EXTENSION_MODULE (BOOL + 3 ptrs = 32 bytes on x64)
struct AFX_EXTENSION_MODULE {
    int   bInitialized;       // BOOL
    HINSTANCE hModule;
    HINSTANCE hResource;
    CRuntimeClass* pFirstSharedClass;
    void*  pFirstSharedFactory; // COleObjectFactory* (opaque)
};

class CDynLinkLibrary : public CCmdTarget {
public:
    CDynLinkLibrary(AFX_EXTENSION_MODULE& state, int bSystem);
    CDynLinkLibrary(HINSTANCE hModule, HINSTANCE hResource);
    virtual ~CDynLinkLibrary();
    HINSTANCE m_hModule;
    HINSTANCE m_hResource;
    CDynLinkLibrary* m_pNextDLL;
protected:
    char _dll_padding[72];    // pad CCmdTarget(32)+24 members to 128
};



