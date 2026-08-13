// COlePasteSpecialDialog — OpenMFC implementation.
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


















// Symbol: ??0COlePasteSpecialDialog@@QEAA@KPEAVCOleDataObject@@PEAVCWnd@@@Z
extern "C" void* MS_ABI impl___0COlePasteSpecialDialog__QEAA_KPEAVCOleDataObject__PEAVCWnd___Z(
        void* p, unsigned long dwFlags, COleDataObject* pDataObject, CWnd* pParentWnd) {
    return new (p) COlePasteSpecialDialog(dwFlags, pDataObject, pParentWnd);
}
COlePasteSpecialDialog::COlePasteSpecialDialog(DWORD dwFlags, COleDataObject* pDataObject, CWnd* pParentWnd)
    : COleDialog(0, pParentWnd), m_dwFlags(dwFlags), m_pDataObject(pDataObject) {
    memset(&m_ps, 0, sizeof(m_ps));
    m_ps.cbStruct = sizeof(OLEUIPASTESPECIALW);
    m_ps.hWndOwner = pParentWnd ? pParentWnd->GetSafeHwnd() : nullptr;
    m_ps.dwFlags = dwFlags;
    memset(_olepastepecialdialog_padding, 0, sizeof(_olepastepecialdialog_padding));
}
COlePasteSpecialDialog::~COlePasteSpecialDialog() {
}
intptr_t COlePasteSpecialDialog::DoModal() {
    UINT result = OleUIPasteSpecialW(&m_ps);
    return (result == OLEUI_OK) ? IDOK : IDCANCEL;
}
CLSID COlePasteSpecialDialog::GetClassID() const {
    CLSID clsid = {};
    return clsid;
}
BOOL COlePasteSpecialDialog::IsPasteLink() const {
    return (m_ps.dwFlags & PSF_SELECTPASTELINK) != 0;
}
int COlePasteSpecialDialog::CreateItem(COleClientItem* pItem) {
    (void)pItem;
    return 0;
}
COleClientItem* COlePasteSpecialDialog::CreateItem(COleDocument* pDoc) {
    return nullptr;
}
UINT COlePasteSpecialDialog::GetSelectionType() const {
    if (m_ps.dwFlags & PSF_SELECTPASTELINK) {
        return PSF_SELECTPASTELINK;
    }
    if (m_ps.dwFlags & PSF_SELECTPASTE) {
        return PSF_SELECTPASTE;
    }
    return 0;
}
void COlePasteSpecialDialog::AddFormat(const FORMATETC& formatEtc, wchar_t* lpszFormat,
                                       wchar_t* lpszResult, DWORD flags) {
    int nNewCount = m_ps.cPasteEntries + 1;
    OLEUIPASTEENTRYW* pEntries = static_cast<OLEUIPASTEENTRYW*>(
        std::realloc(m_ps.arrPasteEntries, sizeof(OLEUIPASTEENTRYW) * nNewCount));
    if (!pEntries) {
        return;
    }
    m_ps.arrPasteEntries = pEntries;

    OLEUIPASTEENTRYW& entry = m_ps.arrPasteEntries[m_ps.cPasteEntries++];
    memset(&entry, 0, sizeof(entry));
    entry.fmtetc = formatEtc;
    entry.lpstrFormatName = lpszFormat;
    entry.lpstrResultText = lpszResult;
    entry.dwFlags = flags;
}
void COlePasteSpecialDialog::AddFormat(UINT cfFormat, TYMED tymed, UINT nFormatID,
                                       BOOL bEnableIcon, BOOL bLink) {
    (void)nFormatID;
    static wchar_t szFormat[] = L"Format";
    static wchar_t szPasteResult[] = L"Paste";
    static wchar_t szLinkResult[] = L"Link";

    FORMATETC formatEtc = {};
    formatEtc.cfFormat = static_cast<CLIPFORMAT>(cfFormat);
    formatEtc.dwAspect = DVASPECT_CONTENT;
    formatEtc.lindex = -1;
    formatEtc.tymed = tymed;

    DWORD flags = bLink ? OLEUIPASTE_LINKANYTYPE : OLEUIPASTE_PASTE;
    if (bEnableIcon) {
        flags |= OLEUIPASTE_ENABLEICON;
    }
    AddFormat(formatEtc, szFormat, bLink ? szLinkResult : szPasteResult, flags);
}
OLEUIPASTEFLAG COlePasteSpecialDialog::AddLinkEntry(UINT nFormatID) {
    static const OLEUIPASTEFLAG linkFlags[] = {
        OLEUIPASTE_LINKTYPE1, OLEUIPASTE_LINKTYPE2, OLEUIPASTE_LINKTYPE3, OLEUIPASTE_LINKTYPE4,
        OLEUIPASTE_LINKTYPE5, OLEUIPASTE_LINKTYPE6, OLEUIPASTE_LINKTYPE7, OLEUIPASTE_LINKTYPE8
    };
    if (nFormatID == 0 || nFormatID > sizeof(linkFlags) / sizeof(linkFlags[0])) {
        return OLEUIPASTE_LINKANYTYPE;
    }
    return linkFlags[nFormatID - 1];
}
void COlePasteSpecialDialog::AddStandardFormats(BOOL bEnableLink) {
    AddFormat(CF_UNICODETEXT, TYMED_HGLOBAL, CF_UNICODETEXT, FALSE, FALSE);
    AddFormat(CF_TEXT, TYMED_HGLOBAL, CF_TEXT, FALSE, FALSE);
    if (bEnableLink) {
        AddFormat(CF_UNICODETEXT, TYMED_HGLOBAL, CF_UNICODETEXT, FALSE, TRUE);
    }
}

// === Moved from ManualThunks.cpp ===
// Symbol: ?AddFormat@COlePasteSpecialDialog@@QEAAXIKIHH@Z
extern "C" void MS_ABI impl__AddFormat_COlePasteSpecialDialog__QEAAXIKIHH_Z(void* pThis, void* p0, void* p1, void* p2, void* p3, void* p4) {
    (void)pThis;
    (void)p0;
    (void)p1;
    (void)p2;
    (void)p3;
    (void)p4;
}

