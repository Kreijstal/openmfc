// COleConvertDialog — OpenMFC implementation.
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


















// Symbol: ??0COleConvertDialog@@QEAA@PEAVCOleClientItem@@KPEAU_GUID@@PEAVCWnd@@@Z
extern "C" void* MS_ABI impl___0COleConvertDialog__QEAA_PEAVCOleClientItem__KPEAU_GUID__PEAVCWnd___Z(
        void* p, COleClientItem* pItem, unsigned long dwFlags, GUID* pClassID,
        CWnd* pParentWnd) {
    return new (p) COleConvertDialog(pItem, dwFlags, pClassID, pParentWnd);
}
COleConvertDialog::COleConvertDialog(COleClientItem* pItem, DWORD dwFlags,
                                      CLSID* pClassID, CWnd* pParentWnd)
    : COleDialog(0, pParentWnd) {
    memset(&m_cv, 0, sizeof(m_cv));
    m_cv.cbStruct = sizeof(OLEUICONVERTW);
    m_cv.hWndOwner = pParentWnd ? pParentWnd->GetSafeHwnd() : nullptr;
    m_cv.dwFlags = dwFlags;
    // Class id: explicit argument wins, else seed from the item.
    if (pClassID)
        m_cv.clsid = *pClassID;
    else if (pItem != nullptr)
        pItem->GetClassID(&m_cv.clsid);
}
COleConvertDialog::~COleConvertDialog() {
}
intptr_t COleConvertDialog::DoModal() {
    UINT result = OleUIConvertW(&m_cv);
    return (result == OLEUI_OK) ? IDOK : IDCANCEL;
}
CLSID COleConvertDialog::GetClassID() const {
    CLSID clsid = {};
    return clsid;
}
BOOL COleConvertDialog::IsConvertTo() const {
    return (m_cv.clsidConvertDefault == m_cv.clsidNew);
}
BOOL COleConvertDialog::IsActivateAs() const {
    return FALSE;
}
CString COleConvertDialog::GetDisplayIcon() const {
    return L"";
}
int COleConvertDialog::DoConvert(COleClientItem* pItem) {
    if (DoModal() != IDOK)
        return FALSE;
    // Apply the chosen conversion/activation to the item.
    if (pItem != nullptr) {
        if (IsConvertTo()) {
            pItem->ConvertTo(GetClassID());
        } else {
            CLSID clsidOld;
            pItem->GetClassID(&clsidOld);
            pItem->ActivateAs(GetClassID(), clsidOld);
        }
    }
    return TRUE;
}
UINT COleConvertDialog::GetSelectionType() const {
    if (m_cv.dwFlags & CF_SELECTACTIVATEAS) {
        return CF_SELECTACTIVATEAS;
    }
    if (m_cv.dwFlags & CF_SELECTCONVERTTO) {
        return CF_SELECTCONVERTTO;
    }
    return 0;
}
