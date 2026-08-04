// COleChangeIconDialog — OpenMFC implementation.
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


















// Symbol: ??0COleChangeIconDialog@@QEAA@PEAVCOleClientItem@@KPEAVCWnd@@@Z
extern "C" void* MS_ABI impl___0COleChangeIconDialog__QEAA_PEAVCOleClientItem__KPEAVCWnd___Z(
        void* p, COleClientItem* pItem, unsigned long dwFlags, CWnd* pParentWnd) {
    return new (p) COleChangeIconDialog(pItem, dwFlags, pParentWnd);
}
COleChangeIconDialog::COleChangeIconDialog(COleClientItem* pItem, DWORD dwFlags, CWnd* pParentWnd)
    : COleDialog(0, pParentWnd) {
    memset(&m_ci, 0, sizeof(m_ci));
    m_ci.cbStruct = sizeof(OLEUICHANGEICONW);
    m_ci.hWndOwner = pParentWnd ? pParentWnd->GetSafeHwnd() : nullptr;
    m_ci.dwFlags = dwFlags;
    // Seed the dialog from the item (no separate member; state lives in m_ci).
    if (pItem != nullptr) {
        pItem->GetClassID(&m_ci.clsid);
        m_ci.hMetaPict = pItem->GetIconicMetafile();
    }
}
COleChangeIconDialog::~COleChangeIconDialog() {
}
intptr_t COleChangeIconDialog::DoModal() {
    UINT result = OleUIChangeIconW(&m_ci);
    return (result == OLEUI_OK) ? IDOK : IDCANCEL;
}
int COleChangeIconDialog::DoChangeIcon(COleClientItem* pItem) {
    if (DoModal() != IDOK)
        return FALSE;
    // Apply the chosen icon back to the item.
    if (pItem != nullptr)
        pItem->SetIconicMetafile(m_ci.hMetaPict);
    return TRUE;
}
