// CMDITabInfo — OpenMFC implementation.
// Sources: global_cmditabinfo.cpp, manual_small_stub_implementations.cpp

#include "detail/CMDITabInfoSupport.h"
#include "detail/ManualSmallStubImplementationsSupport.h"

// -----------------------------------------------------------------------------
// Constructor — installs the real MFC default configuration.
// -----------------------------------------------------------------------------
// Symbol: ??0CMDITabInfo@@QEAA@XZ
extern "C" void* MS_ABI impl___0CMDITabInfo__QEAA_XZ(void* pThis) {
    // Default values verified byte-exact against real mfc140u (differential dump):
    // location=TOP(1), style=FLAT(3), m_bAutoColor and m_bActiveTabCloseButton
    // default FALSE (the agent's earlier guesses of BOTTOM/3D_SCROLLED/TRUE were wrong).
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
// -----------------------------------------------------------------------------
// Serialize — persists every member through the archive, mirroring MFC's
// pattern (store: write each field; load: read each field back).
// Enums are (int)-sized in memory, so a raw 4-byte transfer is identical to the
// (int)-cast operator<< the real implementation uses.
// -----------------------------------------------------------------------------
// Symbol: ?Serialize@CMDITabInfo@@QEAAXAEAVCArchive@@@Z
extern "C" void MS_ABI impl__Serialize_CMDITabInfo__QEAAXAEAVCArchive___Z(void* pThis, void* ar) {
    S_Cmditabinfo* p = reinterpret_cast<S_Cmditabinfo*>(pThis);
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
// Symbol: ?Serialize@CMDITabInfo@@QEAAXAEAVCArchive@@@Z
extern "C" void MS_ABI impl__Serialize_CMDITabInfo__QEAAXAEAVCArchive___XZ(
    void* pThis, void* pArchive) {
    (void)pThis;
    (void)pArchive;
}
