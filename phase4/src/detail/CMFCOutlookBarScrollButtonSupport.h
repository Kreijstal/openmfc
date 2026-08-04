#pragma once
// Shared internals of the former global_cmfcoutlookbarscrollbutton.cpp translation unit.
// Definitions live in detail/CMFCOutlookBarScrollButtonSupport.cpp, so file-scope state keeps a
// single instance across the classes this unit was split into.
namespace openmfc { namespace detail { namespace cmfcoutlookbarscrollbutton {} } }
using namespace openmfc::detail::cmfcoutlookbarscrollbutton;
// CMFCOutlookBarScrollButton: the little up/down scroll buttons that appear on
// an Outlook-style navigation bar. It derives from CMFCButton and overrides two
// protected drawing virtuals: OnFillBackground and OnDrawBorder. Only those two
// virtuals are exported (no ctor/dtor of its own is exported), so this file
// implements exactly those two thunks.
//
// The class is polymorphic (vfptr at offset 0), size(2848). We never touch any
// CMFCOutlookBarScrollButton member here -- both draw methods operate purely on
// the passed CDC and CRect -- so a full 2848-byte mirror is unnecessary; we only
// need to reach through CDC to its device-context handle.

#include <windows.h>
#include <cstddef>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

// Layout-faithful mirror of the head of CDC: {vfptr} at +0, m_hDC at +8
// (harvested via cl.exe /d1reportSingleClassLayout; see repo ABI notes).
struct CDC_head {
    void* vfptr;   // +0
    HDC   m_hDC;   // +8  output device context
    HDC   m_hAttribDC; // +16 attribute device context
};

namespace openmfc { namespace detail { namespace cmfcoutlookbarscrollbutton {
HDC DcHandle_Cmfcoutlookbarscrollbutton(void* pDC);
} } }

// UINT uiState bits used by the CMFCButton drawing pipeline for border state.
// (These mirror the observable "pressed"/"highlighted" distinctions.)
static const UINT SB_STATE_PRESSED     = 0x0001;
static const UINT SB_STATE_HIGHLIGHTED = 0x0002;


