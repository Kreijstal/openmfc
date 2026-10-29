// CFolderPickerDialog — OpenMFC implementation.
// Sources: global_ctordtor_param.cpp, manual_small_stub_implementations.cpp

#include "detail/ManualSmallStubImplementationsSupport.h"

// CFolderPickerDialog runtime-class getters and constructor export. The
// constructor builds the CFileDialog base; retail's extra derived fields at
// +0x13c/+0x140 are not modelled yet.
#include "openmfc/afx.h"
#include "openmfc/afxwin.h"
#include "openmfc/afxmfc.h"
#include "openmfc/afxole.h"
#include "openmfc/afxdisp.h"
#include "openmfc/afxdb.h"
#include "openmfc/afxinet.h"
#include <new>

// Defined alongside the other dialog runtime-class descriptors.
extern CRuntimeClass classCFolderPickerDialog;

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif


















// Symbol: ??0CFolderPickerDialog@@QEAA@PEB_WKPEAVCWnd@@KH@Z
extern "C" void* MS_ABI impl___0CFolderPickerDialog__QEAA_PEB_WKPEAVCWnd__KH_Z(
        void* p, const wchar_t* lpszFolder, unsigned long dwFlags,
        CWnd* pParentWnd, unsigned long nID, int bNeedOpen) {
    (void)nID; (void)bNeedOpen;
    // Retail (mfc140u RVA 0x20e4b0) runs CFileDialog(TRUE, NULL, lpszFolder,
    // OFN_HIDEREADONLY|dwFlags, NULL, pParentWnd, 0, TRUE) and then also sets
    // derived fields at +0x13c/+0x140, which this tree does not model yet.
    // OFN_HIDEREADONLY == 0x4 (not defined in repo headers)
    return new (p) CFileDialog(TRUE, nullptr, lpszFolder, 0x4u | dwFlags, nullptr, pParentWnd, 0, TRUE);
}
// Symbol: ?GetRuntimeClass@CFolderPickerDialog@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CFolderPickerDialog__UEBAPEAUCRuntimeClass__XZ(
    const void* pThis) {
    (void)pThis;
    // Retail is a leaf LEA/RET returning its static descriptor (RVA 0x20e580, mfc140u).
    return &classCFolderPickerDialog;
}
// Symbol: ?GetThisClass@CFolderPickerDialog@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CFolderPickerDialog__SAPEAUCRuntimeClass__XZ() {
    // Retail's ANSI-twin entry is a leaf LEA/RET returning its static descriptor
    // (RVA 0x20c8b0, mfc140; --u did not resolve this symbol).
    return &classCFolderPickerDialog;
}
