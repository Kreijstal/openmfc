// COleDialog — OpenMFC implementation.
// Sources: global_ctordtor_param.cpp, olecore.cpp

#define OPENMFC_APPCORE_IMPL

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


















// Symbol: ??0COleDialog@@QEAA@PEAVCWnd@@@Z
extern "C" void* MS_ABI impl___0COleDialog__QEAA_PEAVCWnd___Z(void* p, CWnd* pParentWnd) {
    return new (p) COleDialog(pParentWnd);
}
COleDialog::COleDialog(UINT nIDTemplate, CWnd* pParentWnd)
    : CDialog(nIDTemplate, pParentWnd) {
    memset(_olediag_padding, 0, sizeof(_olediag_padding));
}
COleDialog::COleDialog(CWnd* pParentWnd)
    : CDialog(static_cast<UINT>(0), pParentWnd) {
    memset(_olediag_padding, 0, sizeof(_olediag_padding));
}
COleDialog::~COleDialog() {
}
int COleDialog::MapResult(UINT nResult) {
    switch (nResult) {
    case OLEUI_OK:
        return IDOK;
    case OLEUI_CANCEL:
        return IDCANCEL;
    default:
        return static_cast<int>(nResult);
    }
}
