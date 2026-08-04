// COleUpdateDialog — OpenMFC implementation.
// Sources: global_ctordtor_param.cpp, ole_view_dragdrop.cpp, olecore.cpp

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


















// OpenMFC - ole_com_activex shard 06/06
// Real implementations overriding weak stubs for a conservative subset of
// COleUpdateDialog (RTTI) and CView (OLE drag/drop default behaviors).

#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxole.h"

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif


// Symbol: ??0COleUpdateDialog@@QEAA@PEAVCOleDocument@@HHPEAVCWnd@@@Z
extern "C" void* MS_ABI impl___0COleUpdateDialog__QEAA_PEAVCOleDocument__HHPEAVCWnd___Z(
        void* p, COleDocument* pDoc, int bUpdateLinks, int bUpdateEmbeddings,
        CWnd* pParentWnd) {
    return new (p) COleUpdateDialog(pDoc, bUpdateLinks, bUpdateEmbeddings, pParentWnd);
}
// Symbol: ?GetThisClass@COleUpdateDialog@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI
impl__GetThisClass_COleUpdateDialog__SAPEAUCRuntimeClass__XZ() {
    return COleUpdateDialog::GetThisClass();
}
// Symbol: ?GetRuntimeClass@COleUpdateDialog@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI
impl__GetRuntimeClass_COleUpdateDialog__UEBAPEAUCRuntimeClass__XZ(
    const COleUpdateDialog* pThis) {
    (void)pThis;
    return COleUpdateDialog::GetThisClass();
}
COleUpdateDialog::COleUpdateDialog(COleDocument* pDoc, BOOL bUpdateLinks,
                                    BOOL bUpdateEmbeddings, CWnd* pParentWnd)
    : COleDialog(0, pParentWnd), m_pDoc(pDoc),
      m_bUpdateLinks(bUpdateLinks), m_bUpdateEmbeddings(bUpdateEmbeddings) {
    memset(_oleupdatedialog_padding, 0, sizeof(_oleupdatedialog_padding));
}
COleUpdateDialog::~COleUpdateDialog() {
}
intptr_t COleUpdateDialog::DoModal() {
    return IDOK;
}
