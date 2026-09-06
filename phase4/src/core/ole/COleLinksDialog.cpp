// COleLinksDialog — OpenMFC implementation.
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


















// Symbol: ??0COleLinksDialog@@QEAA@PEAVCOleDocument@@PEAVCView@@KPEAVCWnd@@@Z
extern "C" void* MS_ABI impl___0COleLinksDialog__QEAA_PEAVCOleDocument__PEAVCView__KPEAVCWnd___Z(
        void* p, COleDocument* pDoc, CView* pView, unsigned long dwFlags,
        CWnd* pParentWnd) {
    return new (p) COleLinksDialog(pDoc, pView, dwFlags, pParentWnd);
}
COleLinksDialog::COleLinksDialog(COleDocument* pDoc, CView* pView, DWORD dwFlags, CWnd* pParentWnd)
    : COleDialog(0, pParentWnd), m_dwFlags(dwFlags), m_pDoc(pDoc) {
    memset(&m_el, 0, sizeof(m_el));
    m_el.cbStruct = sizeof(OLEUIEDITLINKSW);
    m_el.hWndOwner = pParentWnd ? pParentWnd->GetSafeHwnd() : nullptr;
    m_el.dwFlags = dwFlags;
    memset(_olelinksdialog_padding, 0, sizeof(_olelinksdialog_padding));
}
COleLinksDialog::~COleLinksDialog() {
}
intptr_t COleLinksDialog::DoModal() {
    UINT result = OleUIEditLinksW(&m_el);
    return (result == OLEUI_OK) ? IDOK : IDCANCEL;
}
