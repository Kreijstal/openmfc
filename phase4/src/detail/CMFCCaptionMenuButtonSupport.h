#pragma once
// Shared internals of the former global_cmfccaptionmenubutton.cpp translation unit.
// Definitions live in detail/CMFCCaptionMenuButtonSupport.cpp, so file-scope state keeps a
// single instance across the classes this unit was split into.
namespace openmfc { namespace detail { namespace cmfccaptionmenubutton {} } }
using namespace openmfc::detail::cmfccaptionmenubutton;
// OpenMFC: CMFCCaptionMenuButton  (public CMFCCaptionButton : CObject)
//
// Layout (cl.exe /d1reportSingleClassLayout), size(72):
//    0  {vfptr}            (CObject/CMFCCaptionButton vfptr)
//    8  m_bPushed          (CMFCCaptionButton::m_bPushed, BOOL)
//   ... remaining CMFCCaptionButton members (m_rect, m_nHit, enable/focus flags)
//   60  m_nMenuResult      (int)      CMFCCaptionMenuButton members
//   64  m_bOSMenu          (BOOL)
//   68  m_bMenuIsActive    (BOOL)
//
// A caption-bar button that drops a popup menu.  ShowMenu tracks a popup at the
// button rectangle and records the chosen command in m_nMenuResult; OnDraw
// paints the button chrome plus a drop-down arrow.

#include <windows.h>
#include <cstddef>
#include "openmfc/afx.h"

#ifdef __GNUC__
#define MS_ABI __attribute__((ms_abi))
#else
#define MS_ABI
#endif


// Layout-faithful mirror of the object.  The exact intermediate offsets of the
// CMFCCaptionButton base members are not individually validated (the harvest
// only pins {vfptr}@0, m_bPushed@8 and size 72), but they are placed to match
// the real member order so GetRect / OnDraw read sensible values.
// Real CMFCCaptionButton base layout (cl.exe /d1reportSingleClassLayout,
// harvested 2026-07-18): the base has NO m_rect member — the button rectangle
// is computed by the parent from m_ptOrg. Derived members start at 56.
namespace openmfc { namespace detail { namespace cmfccaptionmenubutton {
struct S_Cmfccaptionmenubutton {
    void*    vfptr;               // 0   CObject vfptr
    int      m_bPushed;           // 8
    int      m_bFocused;          // 12
    int      m_bHidden;           // 16
    int      m_bEnabled;          // 20  BOOL (default TRUE)
    int      m_bDroppedDown;      // 24
    int      m_bLeftAlign;        // 28
    unsigned m_nHit;              // 32  UINT
    COLORREF m_clrForeground;     // 36  (default -1 / CLR_DEFAULT)
    POINT    m_ptOrg;             // 40  CPoint (x@40, y@44)
    int      m_bIsMiniFrameButton;// 48
    int      _pad0;               // 52  <alignment member>
    int      m_nMenuResult;       // 56  CMFCCaptionMenuButton::m_nMenuResult
    int      m_bOSMenu;           // 60  BOOL (default TRUE)
    int      m_bMenuIsActive;     // 64  BOOL
    int      _pad1;               // 68  <alignment member>
};
} } }


// CWnd::m_hWnd lives at offset 64.
namespace openmfc { namespace detail { namespace cmfccaptionmenubutton {
HWND WndHandle(void* pWnd);
} } }
// CDC::m_hDC lives at offset 8.
namespace openmfc { namespace detail { namespace cmfccaptionmenubutton {
HDC DcHandle_Cmfccaptionmenubutton(void* pDC);
} } }

// -------------------------------------------------------------------------
// GetRuntimeClass descriptor.  CMFCCaptionMenuButton has no DECLARE macro of
// its own, so slot 0 is CObject::GetRuntimeClass; the most-derived DYNCREATE
// base is CMFCCaptionButton.  Return the static descriptor directly (never
// self-dispatch through the vtable).
namespace openmfc { namespace detail { namespace cmfccaptionmenubutton {
struct CRuntimeClassLite {
    const char* m_lpszClassName;
    int         m_nObjectSize;
    unsigned    m_wSchema;
    void*       m_pfnCreateObject;
    void*       m_pfnGetBaseClass;
    void*       m_pNextClass;
    void*       m_pClassInit;
};
} } }
namespace openmfc { namespace detail { namespace cmfccaptionmenubutton {
extern CRuntimeClassLite g_rtc;
} } }

// ---- non-exported virtual slot helpers --------------------------------------

namespace openmfc { namespace detail { namespace cmfccaptionmenubutton {
void* MS_ABI slot_GetRuntimeClass_Cmfccaptionmenubutton(const void* /*pThis*/);
} } }
namespace openmfc { namespace detail { namespace cmfccaptionmenubutton {
void MS_ABI slot_Serialize_Cmfccaptionmenubutton(void* pThis, void* pAr);
} } }
namespace openmfc { namespace detail { namespace cmfccaptionmenubutton {
void MS_ABI slot_AssertValid_Cmfccaptionmenubutton(const void* pThis);
} } }
namespace openmfc { namespace detail { namespace cmfccaptionmenubutton {
void MS_ABI slot_Dump_Cmfccaptionmenubutton(const void* pThis, void* /*dc*/);
} } }

// Slot 5: CMFCCaptionButton::GetRect — CRect returned by value (large struct
// via hidden return pointer under the MS x64 ABI).
namespace openmfc { namespace detail { namespace cmfccaptionmenubutton {
void* MS_ABI slot_GetRect(void* retptr, const void* pThis);
} } }

// Slot 7: CMFCCaptionButton::GetIconID — maps the hit code to a marlett/system
// glyph id.  A menu button is not one of the standard system-command glyphs, so
// the base returns 0 (no system icon; the arrow is drawn in OnDraw).
namespace openmfc { namespace detail { namespace cmfccaptionmenubutton {
unsigned MS_ABI slot_GetIconID(const void* /*pThis*/, int /*bActive*/,
                                      int /*bHorz*/, int /*bMaximized*/,
                                      int /*bDisabled*/);
} } }


// -----------------------------------------------------------------------------
// Exported thunks
// -----------------------------------------------------------------------------

namespace openmfc { namespace detail { namespace cmfccaptionmenubutton {
extern void* const g_CMFCCaptionMenuButton_vtbl[8];
} } }

namespace openmfc { namespace detail { namespace cmfccaptionmenubutton {
void InitDefaults(S_Cmfccaptionmenubutton* self);
} } }






// -----------------------------------------------------------------------------
// vtable
// -----------------------------------------------------------------------------


// Slot 1: {vector deleting destructor}.
namespace openmfc { namespace detail { namespace cmfccaptionmenubutton {
void* MS_ABI slot_vecDelDtor_Cmfccaptionmenubutton(void* pThis, unsigned flags);
} } }

namespace openmfc { namespace detail { namespace cmfccaptionmenubutton {
extern void* const g_CMFCCaptionMenuButton_vtbl[8];
} } }

