// COleBusyDialog — OpenMFC implementation.
// Sources: global_ctordtor_param.cpp, manual_small_stub_implementations.cpp, olecore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/ManualSmallStubImplementationsSupport.h"
#include "detail/OlecoreSupport.h"

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


















// Symbol: ??0COleBusyDialog@@QEAA@PEAUHTASK__@@HKPEAVCWnd@@@Z
extern "C" void* MS_ABI impl___0COleBusyDialog__QEAA_PEAUHTASK____HKPEAVCWnd___Z(
        void* p, HTASK hTaskBusy, int bNotResponding, unsigned long dwFlags,
        CWnd* pParentWnd) {
    return new (p) COleBusyDialog(hTaskBusy, bNotResponding, dwFlags, pParentWnd);
}
// Symbol: ?GetRuntimeClass@COleBusyDialog@@UEBAPEAUCRuntimeClass@@XZ
extern "C" void* MS_ABI impl__GetRuntimeClass_COleBusyDialog__UEBAPEAUCRuntimeClass__XZ(
    void* pThis) {
    (void)pThis;
    return nullptr;
}
// Symbol: ?GetThisClass@COleBusyDialog@@SAPEAUCRuntimeClass@@XZ
extern "C" void* MS_ABI impl__GetThisClass_COleBusyDialog__SAPEAUCRuntimeClass__XZ() {
    return nullptr;
}
COleBusyDialog::COleBusyDialog(HTASK hTaskBusy, BOOL bNotResponding,
                                DWORD dwFlags, CWnd* pParentWnd)
    : COleDialog(0, pParentWnd), m_dwFlags(dwFlags) {
    memset(&m_bz, 0, sizeof(m_bz));
    m_bz.cbStruct = sizeof(OLEUIBUSYW);
    m_bz.hWndOwner = pParentWnd ? pParentWnd->GetSafeHwnd() : nullptr;
    m_bz.dwFlags = dwFlags;
    m_bz.hTask = hTaskBusy;
}
COleBusyDialog::~COleBusyDialog() {
}
intptr_t COleBusyDialog::DoModal() {
    UINT result = OleUIBusyW(&m_bz);
    return (result == OLEUI_OK) ? IDOK : IDCANCEL;
}
