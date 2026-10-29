// CMDITabInfo — OpenMFC implementation.

#include "detail/CMDITabInfoSupport.h"
#include "detail/ManualSmallStubImplementationsSupport.h"

// CMDITabInfo is not declared in include/openmfc; pin the local shadow fields
// used by Serialize to the offsets its retail body accesses.
static_assert(sizeof(S_Cmditabinfo) == 0x30, "CMDITabInfo size");
static_assert(offsetof(S_Cmditabinfo, m_tabLocation) == 0x00, "CMDITabInfo::m_tabLocation");
static_assert(offsetof(S_Cmditabinfo, m_style) == 0x04, "CMDITabInfo::m_style");
static_assert(offsetof(S_Cmditabinfo, m_bTabIcons) == 0x08, "CMDITabInfo::m_bTabIcons");
static_assert(offsetof(S_Cmditabinfo, m_bTabCloseButton) == 0x0c, "CMDITabInfo::m_bTabCloseButton");
static_assert(offsetof(S_Cmditabinfo, m_bAutoColor) == 0x14, "CMDITabInfo::m_bAutoColor");
static_assert(offsetof(S_Cmditabinfo, m_bDocumentMenu) == 0x18, "CMDITabInfo::m_bDocumentMenu");
static_assert(offsetof(S_Cmditabinfo, m_bEnableTabSwap) == 0x1c, "CMDITabInfo::m_bEnableTabSwap");
static_assert(offsetof(S_Cmditabinfo, m_nTabBorderSize) == 0x28, "CMDITabInfo::m_nTabBorderSize");

extern "C" void MS_ABI impl__AfxThrowArchiveException__YAXHPEB_W_Z(
    int cause, const wchar_t* lpszArchiveName);

namespace {
void ReadArchiveInt(void* ar, int* destination) {
    int value = 0;
    if (impl__Read_CArchive__QEAAIPEAXI_Z(ar, &value, sizeof(value)) != sizeof(value)) {
        impl__AfxThrowArchiveException__YAXHPEB_W_Z(CArchiveException::endOfFile, nullptr);
        return;
    }
    *destination = value;
}
} // namespace

// -----------------------------------------------------------------------------
// Constructor — installs the real MFC default configuration. Retail (entry
// RVA 0x7c440, mfc140u) obtains the tab-border size through a virtual call on
// an internal helper object; -1 is the value observed for the default theme.
// -----------------------------------------------------------------------------
// Symbol: ??0CMDITabInfo@@QEAA@XZ
extern "C" void* MS_ABI impl___0CMDITabInfo__QEAA_XZ(void* pThis) {
    // Default values verified byte-exact against real mfc140u (differential dump):
    // location=TOP(1), style=FLAT(3), m_bAutoColor and m_bActiveTabCloseButton
    // default FALSE.
    S_Cmditabinfo* p = reinterpret_cast<S_Cmditabinfo*>(pThis);
    p->m_tabLocation           = LOCATION_TOP;
    p->m_style                 = STYLE_FLAT;
    p->m_bTabIcons             = FALSE;
    p->m_bTabCloseButton       = TRUE;
    p->m_bTabCustomTooltips    = FALSE;
    p->m_bAutoColor            = FALSE;
    p->m_bDocumentMenu         = FALSE;
    p->m_bEnableTabSwap        = TRUE;
    p->m_bFlatFrame            = TRUE;
    p->m_bActiveTabCloseButton = FALSE;
    p->m_nTabBorderSize        = -1;
    p->m_bReuseRemovedTabGroups= FALSE;
    return pThis;
}
// Serialize stores or loads eight 4-byte fields in retail order. It deliberately
// omits custom tooltips, flat-frame, active-tab-close, and reuse-removed-groups.
// The raw Read/Write thunks transfer the same fields; short reads are translated
// to CArchiveException::endOfFile, as retail's FillBuffer does (entry RVA
// 0x1cfc70, mfc140.dll). The mfc140u Serialize symbol was not present in the
// --u map; its ANSI-twin entry is RVA 0x7ca40 (mfc140.dll).
// Symbol: ?Serialize@CMDITabInfo@@QEAAXAEAVCArchive@@@Z
extern "C" void MS_ABI impl__Serialize_CMDITabInfo__QEAAXAEAVCArchive___Z(void* pThis, void* ar) {
    S_Cmditabinfo* p = reinterpret_cast<S_Cmditabinfo*>(pThis);
    if (ArchiveIsStoring(ar)) {
        impl__Write_CArchive__QEAAXPEBXI_Z(ar, &p->m_tabLocation,      sizeof(int));
        impl__Write_CArchive__QEAAXPEBXI_Z(ar, &p->m_style,             sizeof(int));
        impl__Write_CArchive__QEAAXPEBXI_Z(ar, &p->m_bTabCloseButton,   sizeof(BOOL));
        impl__Write_CArchive__QEAAXPEBXI_Z(ar, &p->m_bTabIcons,         sizeof(BOOL));
        impl__Write_CArchive__QEAAXPEBXI_Z(ar, &p->m_bAutoColor,        sizeof(BOOL));
        impl__Write_CArchive__QEAAXPEBXI_Z(ar, &p->m_bDocumentMenu,     sizeof(BOOL));
        impl__Write_CArchive__QEAAXPEBXI_Z(ar, &p->m_bEnableTabSwap,    sizeof(BOOL));
        impl__Write_CArchive__QEAAXPEBXI_Z(ar, &p->m_nTabBorderSize,    sizeof(int));
    } else {
        ReadArchiveInt(ar, &p->m_tabLocation);
        ReadArchiveInt(ar, &p->m_style);
        ReadArchiveInt(ar, &p->m_bTabCloseButton);
        ReadArchiveInt(ar, &p->m_bTabIcons);
        ReadArchiveInt(ar, &p->m_bAutoColor);
        ReadArchiveInt(ar, &p->m_bDocumentMenu);
        ReadArchiveInt(ar, &p->m_bEnableTabSwap);
        ReadArchiveInt(ar, &p->m_nTabBorderSize);
    }
}
