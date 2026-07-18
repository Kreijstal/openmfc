// OpenMFC — CMDITabInfo real implementation
//
// CMDITabInfo is the plain value struct that CMDIFrameWndEx::EnableMDITabbedGroups
// takes to configure the MDI tab control. It is NOT polymorphic (no vtable) and
// holds tab-location / style enums plus a set of BOOL feature flags and one int.
//
// This file is self-contained: it mirrors the harvested MSVC layout with a local
// struct and drives serialization through the already-exported CArchive Read/Write
// MS_ABI thunks (raw byte IO — byte-identical to MFC's operator<< / operator>>,
// which for these POD fields are just CArchive::Write(&field, sizeof field)).

#include <windows.h>
#include <cstddef>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

// -----------------------------------------------------------------------------
// Layout-faithful mirror of CMDITabInfo — size(48), all 4-byte fields.
// Offsets taken directly from the harvested cl.exe class layout.
// -----------------------------------------------------------------------------
namespace {

// CMFCTabCtrl::Location / CMFCTabCtrl::Style are 4-byte enums.
enum Location : int { LOCATION_BOTTOM = 0, LOCATION_TOP = 1 };
enum Style : int {
    STYLE_3D = 0,
    STYLE_3D_ONENOTE = 1,
    STYLE_3D_VS2005 = 2,
    STYLE_FLAT = 3,
    STYLE_3D_SCROLLED = 4,
    STYLE_FLAT_SHARED_HORZ_SCROLL = 5,
    STYLE_3D_ROUNDED = 6,
    STYLE_3D_ROUNDED_SCROLL = 7
};

struct S {
    int  m_tabLocation;            // 0  CMFCTabCtrl::Location
    int  m_style;                  // 4  CMFCTabCtrl::Style
    BOOL m_bTabIcons;              // 8
    BOOL m_bTabCloseButton;        // 12
    BOOL m_bTabCustomTooltips;     // 16
    BOOL m_bAutoColor;             // 20
    BOOL m_bDocumentMenu;          // 24
    BOOL m_bEnableTabSwap;         // 28
    BOOL m_bFlatFrame;             // 32
    BOOL m_bActiveTabCloseButton;  // 36
    int  m_nTabBorderSize;         // 40
    BOOL m_bReuseRemovedTabGroups; // 44
};

static_assert(sizeof(S) == 48, "CMDITabInfo must be 48 bytes");
static_assert(offsetof(S, m_tabLocation) == 0, "m_tabLocation @0");
static_assert(offsetof(S, m_style) == 4, "m_style @4");
static_assert(offsetof(S, m_bTabIcons) == 8, "m_bTabIcons @8");
static_assert(offsetof(S, m_nTabBorderSize) == 40, "m_nTabBorderSize @40");
static_assert(offsetof(S, m_bReuseRemovedTabGroups) == 44, "m_bReuseRemovedTabGroups @44");

// Minimal view of OpenMFC's CArchive so we can branch on store/load mode.
// OpenMFC's CArchive has a virtual dtor (vfptr @0), then CFile* m_pFile (@8),
// then UINT m_nMode (@16). load bit == 1  (enum Mode { store=0, load=1, ... }).
struct ArchiveView {
    void*        vfptr;   // 0
    void*        m_pFile; // 8
    unsigned int m_nMode; // 16
};
static_assert(offsetof(ArchiveView, m_nMode) == 16, "CArchive::m_nMode @16");

inline bool ArchiveIsStoring(void* ar) {
    return (reinterpret_cast<ArchiveView*>(ar)->m_nMode & 1u) == 0u;
}

} // namespace

// The already-exported CArchive raw IO thunks (defined in thunks.cpp / filecore.cpp,
// linked into the same DLL). Raw Read/Write is exactly what operator<< / operator>>
// perform for these fixed-size POD fields.
extern "C" void         MS_ABI impl__Write_CArchive__QEAAXPEBXI_Z(void* pThis, const void* lpBuf, unsigned int nMax);
extern "C" unsigned int MS_ABI impl__Read_CArchive__QEAAIPEAXI_Z(void* pThis, void* lpBuf, unsigned int nMax);

// -----------------------------------------------------------------------------
// Constructor — installs the real MFC default configuration.
// -----------------------------------------------------------------------------
// Symbol: ??0CMDITabInfo@@QEAA@XZ
extern "C" void* MS_ABI impl___0CMDITabInfo__QEAA_XZ(void* pThis) {
    // Default values verified byte-exact against real mfc140u (differential dump):
    // location=TOP(1), style=FLAT(3), m_bAutoColor and m_bActiveTabCloseButton
    // default FALSE (the agent's earlier guesses of BOTTOM/3D_SCROLLED/TRUE were wrong).
    S* p = reinterpret_cast<S*>(pThis);
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

// -----------------------------------------------------------------------------
// Serialize — persists every member through the archive, mirroring MFC's
// pattern (store: write each field; load: read each field back).
// Enums are (int)-sized in memory, so a raw 4-byte transfer is identical to the
// (int)-cast operator<< the real implementation uses.
// -----------------------------------------------------------------------------
// Symbol: ?Serialize@CMDITabInfo@@QEAAXAEAVCArchive@@@Z
extern "C" void MS_ABI impl__Serialize_CMDITabInfo__QEAAXAEAVCArchive___Z(void* pThis, void* ar) {
    S* p = reinterpret_cast<S*>(pThis);
    if (ArchiveIsStoring(ar)) {
        impl__Write_CArchive__QEAAXPEBXI_Z(ar, &p->m_style,                 sizeof(int));
        impl__Write_CArchive__QEAAXPEBXI_Z(ar, &p->m_tabLocation,           sizeof(int));
        impl__Write_CArchive__QEAAXPEBXI_Z(ar, &p->m_bTabIcons,             sizeof(BOOL));
        impl__Write_CArchive__QEAAXPEBXI_Z(ar, &p->m_bTabCloseButton,       sizeof(BOOL));
        impl__Write_CArchive__QEAAXPEBXI_Z(ar, &p->m_bTabCustomTooltips,    sizeof(BOOL));
        impl__Write_CArchive__QEAAXPEBXI_Z(ar, &p->m_bActiveTabCloseButton, sizeof(BOOL));
        impl__Write_CArchive__QEAAXPEBXI_Z(ar, &p->m_bAutoColor,            sizeof(BOOL));
        impl__Write_CArchive__QEAAXPEBXI_Z(ar, &p->m_bDocumentMenu,         sizeof(BOOL));
        impl__Write_CArchive__QEAAXPEBXI_Z(ar, &p->m_bEnableTabSwap,        sizeof(BOOL));
        impl__Write_CArchive__QEAAXPEBXI_Z(ar, &p->m_bFlatFrame,            sizeof(BOOL));
        impl__Write_CArchive__QEAAXPEBXI_Z(ar, &p->m_nTabBorderSize,        sizeof(int));
        impl__Write_CArchive__QEAAXPEBXI_Z(ar, &p->m_bReuseRemovedTabGroups,sizeof(BOOL));
    } else {
        impl__Read_CArchive__QEAAIPEAXI_Z(ar, &p->m_style,                 sizeof(int));
        impl__Read_CArchive__QEAAIPEAXI_Z(ar, &p->m_tabLocation,           sizeof(int));
        impl__Read_CArchive__QEAAIPEAXI_Z(ar, &p->m_bTabIcons,             sizeof(BOOL));
        impl__Read_CArchive__QEAAIPEAXI_Z(ar, &p->m_bTabCloseButton,       sizeof(BOOL));
        impl__Read_CArchive__QEAAIPEAXI_Z(ar, &p->m_bTabCustomTooltips,    sizeof(BOOL));
        impl__Read_CArchive__QEAAIPEAXI_Z(ar, &p->m_bActiveTabCloseButton, sizeof(BOOL));
        impl__Read_CArchive__QEAAIPEAXI_Z(ar, &p->m_bAutoColor,            sizeof(BOOL));
        impl__Read_CArchive__QEAAIPEAXI_Z(ar, &p->m_bDocumentMenu,         sizeof(BOOL));
        impl__Read_CArchive__QEAAIPEAXI_Z(ar, &p->m_bEnableTabSwap,        sizeof(BOOL));
        impl__Read_CArchive__QEAAIPEAXI_Z(ar, &p->m_bFlatFrame,            sizeof(BOOL));
        impl__Read_CArchive__QEAAIPEAXI_Z(ar, &p->m_nTabBorderSize,        sizeof(int));
        impl__Read_CArchive__QEAAIPEAXI_Z(ar, &p->m_bReuseRemovedTabGroups,sizeof(BOOL));
    }
}
