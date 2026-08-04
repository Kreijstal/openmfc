#pragma once
// Shared internals of the former global_cmditabinfo.cpp translation unit.
// Definitions live in detail/CMDITabInfoSupport.cpp, so file-scope state keeps a
// single instance across the classes this unit was split into.
namespace openmfc { namespace detail { namespace cmditabinfo {} } }
using namespace openmfc::detail::cmditabinfo;
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

// CMFCTabCtrl::Location / CMFCTabCtrl::Style are 4-byte enums.
namespace openmfc { namespace detail { namespace cmditabinfo {
enum Location : int { LOCATION_BOTTOM = 0, LOCATION_TOP = 1 };
} } }
namespace openmfc { namespace detail { namespace cmditabinfo {
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
} } }

namespace openmfc { namespace detail { namespace cmditabinfo {
struct S_Cmditabinfo {
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
} } }


// Minimal view of OpenMFC's CArchive so we can branch on store/load mode.
// OpenMFC's CArchive has a virtual dtor (vfptr @0), then CFile* m_pFile (@8),
// then UINT m_nMode (@16). load bit == 1  (enum Mode { store=0, load=1, ... }).
namespace openmfc { namespace detail { namespace cmditabinfo {
struct ArchiveView {
    void*        vfptr;   // 0
    void*        m_pFile; // 8
    unsigned int m_nMode; // 16
};
} } }

namespace openmfc { namespace detail { namespace cmditabinfo {
inline bool ArchiveIsStoring(void* ar) {
    return (reinterpret_cast<ArchiveView*>(ar)->m_nMode & 1u) == 0u;
}
} } }


// The already-exported CArchive raw IO thunks (defined in thunks.cpp / filecore.cpp,
// linked into the same DLL). Raw Read/Write is exactly what operator<< / operator>>
// perform for these fixed-size POD fields.
extern "C" void         MS_ABI impl__Write_CArchive__QEAAXPEBXI_Z(void* pThis, const void* lpBuf, unsigned int nMax);
extern "C" unsigned int MS_ABI impl__Read_CArchive__QEAAIPEAXI_Z(void* pThis, void* lpBuf, unsigned int nMax);


