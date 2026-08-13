// COleInsertDialog — OpenMFC implementation.
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


















// Symbol: ??0COleInsertDialog@@QEAA@KPEAVCWnd@@@Z
extern "C" void* MS_ABI impl___0COleInsertDialog__QEAA_KPEAVCWnd___Z(
        void* p, unsigned long dwFlags, CWnd* pParentWnd) {
    return new (p) COleInsertDialog(dwFlags, pParentWnd);
}
COleInsertDialog::COleInsertDialog(DWORD dwFlags, CWnd* pParentWnd)
    : COleDialog(0, pParentWnd), m_dwFlags(dwFlags) {
    memset(&m_io, 0, sizeof(m_io));
    m_io.cbStruct = sizeof(OLEUIINSERTOBJECTW);
    m_io.hWndOwner = pParentWnd ? pParentWnd->GetSafeHwnd() : nullptr;
    m_io.dwFlags = dwFlags;
    memset(_oleinsertdialog_padding, 0, sizeof(_oleinsertdialog_padding));
}
COleInsertDialog::~COleInsertDialog() {
}
__int64 COleInsertDialog::DoModal() {
    return DoModal(0UL);
}
__int64 COleInsertDialog::DoModal(unsigned long) {
    UINT result = OleUIInsertObjectW(&m_io);
    return (result == OLEUI_OK) ? IDOK : IDCANCEL;
}
CLSID COleInsertDialog::GetClassID() const {
    return m_io.clsid;
}
CString COleInsertDialog::GetPathName() const {
    return m_io.lpszFile ? m_io.lpszFile : L"";
}
BOOL COleInsertDialog::IsCreateNew() const {
    return m_io.dwFlags & IOF_SELECTCREATENEW || !(m_io.dwFlags & IOF_SELECTCREATEFROMFILE);
}
BOOL COleInsertDialog::IsDisplayAsIcon() const {
    return m_io.dwFlags & IOF_CHECKDISPLAYASICON;
}
int COleInsertDialog::CreateItem(COleClientItem* pItem) {
    (void)pItem;
    return 0;
}
COleClientItem* COleInsertDialog::CreateItem(COleDocument* pDoc) {
    (void)pDoc;
    return nullptr;
}
void COleInsertDialog::GetIconMetafile(HGLOBAL* phMetaPict) {
    if (phMetaPict) *phMetaPict = m_io.hMetaPict;
}
UINT COleInsertDialog::GetSelectionType() const {
    if (m_io.dwFlags & IOF_SELECTCREATECONTROL) {
        return IOF_SELECTCREATECONTROL;
    }
    if (m_io.dwFlags & IOF_SELECTCREATEFROMFILE) {
        return IOF_SELECTCREATEFROMFILE;
    }
    if (m_io.dwFlags & IOF_SELECTCREATENEW) {
        return IOF_SELECTCREATENEW;
    }
    return 0;
}
void COleInsertDialog::AddClassIDToList(CLSID*& rgclsid, int& nCount, int& nAlloc, CLSID* pClassID) {
    if (!pClassID) {
        return;
    }
    if (nCount >= nAlloc) {
        int nNewAlloc = nAlloc > 0 ? nAlloc * 2 : 8;
        CLSID* pNewList = static_cast<CLSID*>(std::realloc(rgclsid, sizeof(CLSID) * nNewAlloc));
        if (!pNewList) {
            return;
        }
        rgclsid = pNewList;
        nAlloc = nNewAlloc;
    }
    rgclsid[nCount++] = *pClassID;
}

// === Moved from ManualThunks.cpp ===
// Symbol: ?AddClassIDToList@COleInsertDialog@@IEAAXAEAPEAU_GUID@@AEAH1PEAU2@@Z
extern "C" void MS_ABI impl__AddClassIDToList_COleInsertDialog__IEAAXAEAPEAU_GUID__AEAH1PEAU2__Z(void* pThis, void* p0, void* p1, void* p2, void* p3) {
    (void)pThis;
    (void)p0;
    (void)p1;
    (void)p2;
    (void)p3;
}

