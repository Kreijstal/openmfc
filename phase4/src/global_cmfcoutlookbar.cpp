// OpenMFC: CMFCOutlookBar exports.
//
// The single export implemented here, transcribed from the retail mfc140u.dll
// (14.51.36231, x64) by decoding the export table (ordinal -> RVA -> objdump
// of the retail body).
//
// The class is not declared in the public headers, so `this` is taken as void*
// and the touched member offsets are mirrored with the file-local layout struct
// below.  CMFCOutlookBar derives from CBaseTabbedPane and is 1384 bytes (0x568,
// see global_mfc_05_msgmap.cpp / global_deferred_rtti.cpp); CWnd::m_hWnd sits at
// +0x40 and the mode-2003 flag at +0x550.
//
// Conventions (see IMPLEMENTATION_GUIDE.md):
//   * every export is `extern "C"` + `__attribute__((ms_abi))`;
//   * the implicit `this` is the first explicit parameter;
//   * NULL-check pThis everywhere;
//   * struct-returning exports take the hidden return slot in the register the
//     retail body uses, as documented at each definition.
//
// Kept in a global_*.cpp file so the build's AUTO_EXCLUDES pass (the scan of
// `// Symbol:` comments) drops the generated weak stubs for these ordinals.

#include "openmfc/afxwin.h"
#include "openmfc/afxmfc.h"

#include <windows.h>

#include <cstddef>
#include <cstdint>
#include <new>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

namespace {

//---------------------------------------------------------------------------
// Layout mirrors (offsets harvested from the retail bodies).
//---------------------------------------------------------------------------

// CMFCOutlookBar, sizeof 1384 (0x568) -- DEF_DESC(CMFCOutlookBar, 1384).
struct OutlookBar
{
    void* vfptr;                 // 0x00 CObject vfptr
    char  _pad0[0x40 - 0x08];    // 0x08..0x3f
    HWND  m_hWnd;                // 0x40 CWnd::m_hWnd
    char  _pad1[0x550 - 0x48];   // 0x48..0x54f
    int   m_bMode2003;           // 0x550 SetMode2003 flag
    char  _pad2[0x568 - 0x554];  // 0x554..0x567
};
static_assert(sizeof(OutlookBar) == 0x568, "CMFCOutlookBar size mismatch");
static_assert(offsetof(OutlookBar, m_hWnd) == 0x40, "CMFCOutlookBar::m_hWnd offset");

} // namespace

//=============================================================================
// CMFCOutlookBar
//=============================================================================

//=============================================================================
// void CMFCOutlookBar::SetMode2003(int bMode)
//=============================================================================
// Retail (0x180097f70):
//     if (m_hWnd == NULL) m_bMode2003 = bMode;
// (The retail NULL-this path falls through to the store and would fault; we
//  guard it instead.)
// Symbol: ?SetMode2003@CMFCOutlookBar@@QEAAXH@Z
extern "C" void MS_ABI impl__SetMode2003_CMFCOutlookBar__QEAAXH_Z(void* pThis, int bMode2003)
{
    if (pThis == nullptr)
        return;

    OutlookBar* pBar = static_cast<OutlookBar*>(pThis);
    if (pBar->m_hWnd == nullptr) {
        pBar->m_bMode2003 = bMode2003;
    }
}
