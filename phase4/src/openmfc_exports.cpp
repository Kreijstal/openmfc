// OpenMFC-Specific Exports
//
// This file exports symbols that are needed by code using OpenMFC headers,
// but which are NOT part of the real mfc140u.dll exports.
//
// These include:
// - g_pApp global variable
// - Virtual method implementations for base classes
// - Static CRuntimeClass members from IMPLEMENT_DYNAMIC
//
// Without these exports, MSVC code that derives from MFC classes or uses
// AfxGetApp() inline function would fail to link.
//
// The .def file additions in build_phase4.sh map these stub names to MSVC symbols.

// Define OPENMFC_APPCORE_IMPL to prevent inline implementations conflicting with appcore.cpp
#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxwin.h"
#include <windows.h>

// MS ABI calling convention
#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

// =============================================================================
// g_pApp Export
// MSVC symbol: ?g_pApp@@3PEAVCWinApp@@EA
// =============================================================================

// The actual g_pApp is defined in appcore.cpp as: CWinApp* g_pApp = nullptr;
// GCC doesn't mangle global variables, so the symbol is just "g_pApp".
// The .def file maps: ?g_pApp@@3PEAVCWinApp@@EA=g_pApp DATA
// This exports the GCC symbol with the MSVC-mangled name.
extern CWinApp* g_pApp;

// =============================================================================
// CObject::Serialize Export
// MSVC symbol: ?Serialize@CObject@@UEAAXAEAVCArchive@@@Z
// Stub name: stub___Serialize_CObject__UEAAXAEAVCArchive___Z
// =============================================================================

// CObject::Serialize is defined in cobject_impl.cpp
// This stub provides the exported function with MS ABI calling convention
extern "C" void MS_ABI stub___Serialize_CObject__UEAAXAEAVCArchive___Z(CObject* pThis, CArchive* pAr) {
    if (pThis && pAr) {
        pThis->Serialize(*pAr);
    }
}

// =============================================================================
// CCmdTarget::OnCmdMsg Export
// MSVC symbol: ?OnCmdMsg@CCmdTarget@@UEAAHIHPEAX0@Z
// Stub name: stub___OnCmdMsg_CCmdTarget__UEAAHIHPEAX0_Z
// =============================================================================

// OnCmdMsg is already implemented in appcore.cpp, but we need to export it
extern "C" int MS_ABI stub___OnCmdMsg_CCmdTarget__UEAAHIHPEAX0_Z(
    CCmdTarget* pThis, unsigned int nID, int nCode, void* pExtra, void* pHandlerInfo) {
    if (pThis) {
        return pThis->OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
    }
    return FALSE;
}

// =============================================================================
// CWinThread::PrePumpMessage Export
// MSVC symbol: ?PrePumpMessage@CWinThread@@UEAAHXZ
// Stub name: stub___PrePumpMessage_CWinThread__UEAAHXZ
// =============================================================================

extern "C" int MS_ABI stub___PrePumpMessage_CWinThread__UEAAHXZ(CWinThread* pThis) {
    if (pThis) {
        return pThis->PrePumpMessage();
    }
    return TRUE;
}

// =============================================================================
// CWinThread::PostPumpMessage Export
// MSVC symbol: ?PostPumpMessage@CWinThread@@UEAAHXZ
// Stub name: stub___PostPumpMessage_CWinThread__UEAAHXZ
// =============================================================================

extern "C" int MS_ABI stub___PostPumpMessage_CWinThread__UEAAHXZ(CWinThread* pThis) {
    if (pThis) {
        return pThis->PostPumpMessage();
    }
    return TRUE;
}

// =============================================================================
// Static CRuntimeClass members exports
// These are defined by IMPLEMENT_DYNAMIC macros in appcore.cpp
// MSVC symbol format: ?class<ClassName>@<ClassName>@@2UCRuntimeClass@@A
//
// The IMPLEMENT_DYNAMIC macro creates static members like:
//   CRuntimeClass CWinApp::classCWinApp = { ... };
//
// With GCC, these get GCC-mangled names. We need to export them with MSVC names.
// The .def file uses the GCC-mangled names to alias to MSVC names.
// =============================================================================

// These are already defined by IMPLEMENT_DYNAMIC in appcore.cpp and other files.
// We just need to reference them so they're included, and let the .def file handle aliasing.
// The actual exports are done via the .def file using the GCC-mangled symbol names.
