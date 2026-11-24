// CMDITabInfo — OpenMFC implementation.

#include "detail/CMDITabInfoSupport.h"
#include <new>

// Assert the layout used by the exports.
static_assert(sizeof(S_Cmditabinfo) == 0x30, "CMDITabInfo size");
static_assert(offsetof(S_Cmditabinfo, m_tabLocation) == 0x00, "CMDITabInfo::m_tabLocation");
static_assert(offsetof(S_Cmditabinfo, m_style) == 0x04, "CMDITabInfo::m_style");
static_assert(offsetof(S_Cmditabinfo, m_bTabIcons) == 0x08, "CMDITabInfo::m_bTabIcons");
static_assert(offsetof(S_Cmditabinfo, m_bTabCloseButton) == 0x0c, "CMDITabInfo::m_bTabCloseButton");
static_assert(offsetof(S_Cmditabinfo, m_bAutoColor) == 0x14, "CMDITabInfo::m_bAutoColor");
static_assert(offsetof(S_Cmditabinfo, m_bDocumentMenu) == 0x18, "CMDITabInfo::m_bDocumentMenu");
static_assert(offsetof(S_Cmditabinfo, m_bEnableTabSwap) == 0x1c, "CMDITabInfo::m_bEnableTabSwap");
static_assert(offsetof(S_Cmditabinfo, m_nTabBorderSize) == 0x28, "CMDITabInfo::m_nTabBorderSize");

// Symbol: ??0CMDITabInfo@@QEAA@XZ
extern "C" void* MS_ABI impl___0CMDITabInfo__QEAA_XZ(void* pThis) {
    return ::new (pThis) CMDITabInfo();
}
// Retail serializes these eight 32-bit values in order. Other options retain
// their defaults or the settings already supplied by the caller.
void CMDITabInfo::Serialize(CArchive& archive) {
    if (archive.IsStoring()) {
        archive << static_cast<int>(m_tabLocation) << static_cast<int>(m_style)
            << m_bTabCloseButton << m_bTabIcons << m_bAutoColor << m_bDocumentMenu
            << m_bEnableTabSwap << m_nTabBorderSize;
    } else {
        int location = 0;
        int style = 0;
        archive >> location >> style;
        m_tabLocation = static_cast<CMFCBaseTabCtrl::Location>(location);
        m_style = static_cast<CMFCTabCtrl::Style>(style);
        archive >> m_bTabCloseButton >> m_bTabIcons >> m_bAutoColor >> m_bDocumentMenu
            >> m_bEnableTabSwap >> m_nTabBorderSize;
    }
}

// Symbol: ?Serialize@CMDITabInfo@@QEAAXAEAVCArchive@@@Z
extern "C" void MS_ABI impl__Serialize_CMDITabInfo__QEAAXAEAVCArchive___Z(void* pThis, void* archive) {
    static_cast<CMDITabInfo*>(pThis)->Serialize(*static_cast<CArchive*>(archive));
}
