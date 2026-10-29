// COleBusyDialog — OpenMFC implementation.

#define OPENMFC_APPCORE_IMPL

#include "detail/ManualSmallStubImplementationsSupport.h"
#include "detail/OlecoreSupport.h"

// Parameterized-constructor export thunk for COleBusyDialog. It placement-constructs
// the local shadow object with the constructor below.
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
// Retail RVA 0x24eac0 (mfc140u) returns the COleBusyDialog class descriptor directly;
// it does not inspect the object pointer.
// Symbol: ?GetRuntimeClass@COleBusyDialog@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_COleBusyDialog__UEBAPEAUCRuntimeClass__XZ(
    const COleBusyDialog* pThis) {
    (void)pThis;
    return COleBusyDialog::GetThisClass();
}
// GetThisClass is not resolved in the retail RVA map. DECLARE_DYNAMIC supplies the inline
// accessor returning classCOleBusyDialog; IMPLEMENT_DYNAMIC registers that descriptor.
// Symbol: ?GetThisClass@COleBusyDialog@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_COleBusyDialog__SAPEAUCRuntimeClass__XZ() {
    return COleBusyDialog::GetThisClass();
}
// Approximation of retail ctor RVA 0x24e930 (mfc140u): retail also folds
// bNotResponding into the flags and installs the private hook at RVA 0x24e890.
COleBusyDialog::COleBusyDialog(HTASK hTaskBusy, BOOL bNotResponding,
                                DWORD dwFlags, CWnd* pParentWnd)
    : COleDialog(0, pParentWnd), m_dwFlags(dwFlags) {
    (void)bNotResponding;
    memset(&m_bz, 0, sizeof(m_bz));
    m_bz.cbStruct = sizeof(OLEUIBUSYW);
    m_bz.hWndOwner = pParentWnd ? pParentWnd->GetSafeHwnd() : nullptr;
    m_bz.dwFlags = dwFlags;
    m_bz.hTask = hTaskBusy;
}
COleBusyDialog::~COleBusyDialog() {
}
// Approximation of retail DoModal RVA 0x24ea00 (mfc140u), which also manages the
// owner, creation hook and modal state and maps more OLEUI result codes.
intptr_t COleBusyDialog::DoModal() {
    UINT result = OleUIBusyW(&m_bz);
    return (result == OLEUI_OK) ? IDOK : IDCANCEL;
}
