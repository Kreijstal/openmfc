// COleChangeSourceDialog — OpenMFC implementation.
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


















// Symbol: ??0COleChangeSourceDialog@@QEAA@PEAVCOleClientItem@@PEAVCWnd@@@Z
extern "C" void* MS_ABI impl___0COleChangeSourceDialog__QEAA_PEAVCOleClientItem__PEAVCWnd___Z(
        void* p, COleClientItem* pItem, CWnd* pParentWnd) {
    return new (p) COleChangeSourceDialog(pItem, 0, pParentWnd);
}
COleChangeSourceDialog::COleChangeSourceDialog(COleClientItem* pItem, DWORD dwFlags, CWnd* pParentWnd)
    : COleDialog(0, pParentWnd), m_dwFlags(dwFlags), m_pItem(pItem) {
    memset(&m_cs, 0, sizeof(m_cs));
    m_cs.cbStruct = sizeof(OLEUICHANGESOURCEW);
    m_cs.hWndOwner = pParentWnd ? pParentWnd->GetSafeHwnd() : nullptr;
    m_cs.dwFlags = dwFlags;
    memset(_olechangesourcedialog_padding, 0, sizeof(_olechangesourcedialog_padding));
}
COleChangeSourceDialog::~COleChangeSourceDialog() {
}
intptr_t COleChangeSourceDialog::DoModal() {
    UINT result = OleUIChangeSourceW(&m_cs);
    return (result == OLEUI_OK) ? IDOK : IDCANCEL;
}
CString COleChangeSourceDialog::GetDisplayName() {
    return m_cs.lpszDisplayName ? m_cs.lpszDisplayName : L"";
}
CString COleChangeSourceDialog::GetFrom() {
    return m_cs.lpszFrom ? m_cs.lpszFrom : L"";
}
CString COleChangeSourceDialog::GetTo() {
    return m_cs.lpszTo ? m_cs.lpszTo : L"";
}
void COleChangeSourceDialog::PreInitDialog() {
    static const wchar_t kEmpty[] = L"";
    m_cs.lpszDisplayName = const_cast<wchar_t*>(kEmpty);
    m_cs.lpszFrom = const_cast<wchar_t*>(kEmpty);
    m_cs.lpszTo = const_cast<wchar_t*>(kEmpty);
}
