// CFolderPickerDialog — OpenMFC implementation.
// Sources: global_ctordtor_param.cpp, manual_small_stub_implementations.cpp

#include "detail/ManualSmallStubImplementationsSupport.h"

// Real parameterized-constructor export thunks for header-defined, publicly
// constructible MFC classes. Each viewed-as MS_ABI thunk placement-constructs
// the object with the real ctor body (defined in filecore/olecore/dlgcommon/
// inetcore or inline in headers) -- NOT a stub. Completes classes that were one
// export short of full coverage.
#include "openmfc/afx.h"
#include "openmfc/afxwin.h"
#include "openmfc/afxmfc.h"
#include "openmfc/afxole.h"
#include "openmfc/afxdisp.h"
#include "openmfc/afxdb.h"
#include "openmfc/afxinet.h"
#include <new>
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
    // Real MFC: CFileDialog(TRUE, NULL, lpszFolder, OFN_HIDEREADONLY|dwFlags, NULL, pParentWnd, 0, TRUE)
    // OFN_HIDEREADONLY == 0x4 (not defined in repo headers)
    return new (p) CFileDialog(TRUE, nullptr, lpszFolder, 0x4u | dwFlags, nullptr, pParentWnd, 0, TRUE);
}
// Symbol: ?GetRuntimeClass@CFolderPickerDialog@@UEBAPEAUCRuntimeClass@@XZ
extern "C" void* MS_ABI impl__GetRuntimeClass_CFolderPickerDialog__UEBAPEAUCRuntimeClass__XZ(
    void* pThis) {
    (void)pThis;
    return nullptr;
}
// Symbol: ?GetThisClass@CFolderPickerDialog@@SAPEAUCRuntimeClass@@XZ
extern "C" void* MS_ABI impl__GetThisClass_CFolderPickerDialog__SAPEAUCRuntimeClass__XZ() {
    return nullptr;
}
