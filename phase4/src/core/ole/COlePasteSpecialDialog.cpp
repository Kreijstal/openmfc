// COlePasteSpecialDialog — OpenMFC implementation.
// Sources: global_ctordtor_param.cpp, olecore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/OlecoreSupport.h"

// Real parameterized-constructor export thunks for header-defined, publicly
// constructible MFC classes. Each viewed-as MS_ABI thunk placement-constructs
// the object with its real constructor body (defined in this file, in companion
// implementation files, or inline in headers) -- NOT a stub. Completes classes
// that were one export short of full coverage.
#include "openmfc/afx.h"
#include "openmfc/afxwin.h"
#include "openmfc/afxmfc.h"
#include "openmfc/afxole.h"
#include "openmfc/afxdisp.h"
#include "openmfc/afxdb.h"
#include "openmfc/afxinet.h"
#include <new>
#include <cstdlib>
#include <atomic>
#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

namespace {
struct PasteSpecialLinkTypesStorage {
    COlePasteSpecialDialog* owner;
    UINT* values;
    PasteSpecialLinkTypesStorage* next;
};

std::atomic_flag g_pasteSpecialLinkTypesLock = ATOMIC_FLAG_INIT;
PasteSpecialLinkTypesStorage* g_pasteSpecialLinkTypes = nullptr;

struct PasteSpecialLinkTypesLock {
    PasteSpecialLinkTypesLock() {
        while (g_pasteSpecialLinkTypesLock.test_and_set(std::memory_order_acquire)) {
        }
    }
    ~PasteSpecialLinkTypesLock() {
        g_pasteSpecialLinkTypesLock.clear(std::memory_order_release);
    }
};

// The retail dialog uses eight inline UINTs at this+0x1c0. That range is
// occupied by OpenMFC's public tail fields, so keep the equivalent list in a
// side allocation rather than aliasing m_dwFlags/m_pDataObject.
bool AddPasteSpecialLinkType(COlePasteSpecialDialog* dialog, UINT formatId, UINT& flag) {
    PasteSpecialLinkTypesLock lock;
    PasteSpecialLinkTypesStorage* storage = g_pasteSpecialLinkTypes;
    while (storage && storage->owner != dialog) {
        storage = storage->next;
    }
    if (!storage) {
        void* allocation = std::malloc(sizeof(PasteSpecialLinkTypesStorage) + 8 * sizeof(UINT));
        if (!allocation) {
            return false;
        }
        storage = static_cast<PasteSpecialLinkTypesStorage*>(allocation);
        storage->owner = dialog;
        storage->values = reinterpret_cast<UINT*>(storage + 1);
        memset(storage->values, 0, 8 * sizeof(UINT));
        storage->next = g_pasteSpecialLinkTypes;
        g_pasteSpecialLinkTypes = storage;
    }

    // If a caller replaced the public structure's pointer, preserve its current
    // entries before restoring the storage owned by this implementation.
    if (dialog->m_ps.arrLinkTypes != storage->values) {
        int count = dialog->m_ps.cLinkTypes;
        if (count < 0) count = 0;
        if (count > 8) count = 8;
        if (dialog->m_ps.arrLinkTypes && count != 0) {
            memcpy(storage->values, dialog->m_ps.arrLinkTypes,
                   static_cast<size_t>(count) * sizeof(UINT));
        }
        dialog->m_ps.cLinkTypes = count;
        dialog->m_ps.arrLinkTypes = storage->values;
    }

    int count = dialog->m_ps.cLinkTypes;
    if (count < 0) count = 0;
    if (count > 8) count = 8;
    int index = 0;
    while (index < count && storage->values[index] != formatId) {
        ++index;
    }
    if (index == 8) {
        flag = 0;
        return true;
    }
    storage->values[index] = formatId;
    if (index == count) {
        dialog->m_ps.cLinkTypes = count + 1;
    }
    flag = 1u << index;
    return true;
}

void ReleasePasteSpecialLinkTypes(COlePasteSpecialDialog* dialog) {
    PasteSpecialLinkTypesLock lock;
    PasteSpecialLinkTypesStorage** link = &g_pasteSpecialLinkTypes;
    while (*link && (*link)->owner != dialog) {
        link = &(*link)->next;
    }
    if (*link) {
        PasteSpecialLinkTypesStorage* storage = *link;
        *link = storage->next;
        std::free(storage);
    }
}
} // namespace


















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
    ReleasePasteSpecialLinkTypes(this);
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
// Retail AddFormat entry RVA 0x24e3c0 (mfc140u) calls AddLinkEntry entry RVA
// 0x24e350 (mfc140u) for link formats. Retail's eight inline link IDs overlap
// OpenMFC's public tail, so this thunk keeps the same bounded/deduplicated list
// in owned side storage. Display labels use the existing generic helper above;
// retail instead loads localized labels from the resource identified by nFormatID.
// Symbol: ?AddFormat@COlePasteSpecialDialog@@QEAAXIKIHH@Z
extern "C" void MS_ABI impl__AddFormat_COlePasteSpecialDialog__QEAAXIKIHH_Z(
        COlePasteSpecialDialog* pThis, UINT cfFormat, DWORD tymed, UINT nFormatID,
        BOOL bEnableIcon, BOOL bLink) {
    static wchar_t szFormat[] = L"Format";
    static wchar_t szPasteResult[] = L"Paste";
    static wchar_t szLinkResult[] = L"Link";

    UINT linkFlag = 0;
    if (bLink && !AddPasteSpecialLinkType(pThis, nFormatID, linkFlag)) {
        return;
    }

    FORMATETC formatEtc = {};
    formatEtc.cfFormat = static_cast<CLIPFORMAT>(cfFormat);
    formatEtc.dwAspect = DVASPECT_CONTENT;
    formatEtc.lindex = -1;
    formatEtc.tymed = static_cast<DWORD>(tymed);

    DWORD flags = bEnableIcon ? OLEUIPASTE_PASTE | OLEUIPASTE_ENABLEICON
                              : OLEUIPASTE_PASTE;
    flags |= linkFlag;
    if (flags == OLEUIPASTE_PASTE) {
        flags = OLEUIPASTE_PASTEONLY;
    }
    pThis->AddFormat(formatEtc, szFormat, bLink ? szLinkResult : szPasteResult, flags);
}

