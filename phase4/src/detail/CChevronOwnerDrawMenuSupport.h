#pragma once
// Shared internals of the former global_cchevronownerdrawmenu.cpp translation unit.
// Definitions live in detail/CChevronOwnerDrawMenuSupport.cpp, so file-scope state keeps a
// single instance across the classes this unit was split into.
namespace openmfc { namespace detail { namespace cchevronownerdrawmenu {} } }
using namespace openmfc::detail::cchevronownerdrawmenu;
// OpenMFC: CChevronOwnerDrawMenu  (public CMenu : CObject)
//
// Layout (cl.exe /d1reportSingleClassLayout), size(32):
//   0  {vfptr}          (CObject/CMenu vfptr)
//   8  m_hMenu          (CMenu::m_hMenu, HMENU)
//  16  m_MenuFont       (CFont : CGdiObject : CObject) -> {vfptr, m_hObject}
//        16  m_MenuFont.{vfptr}
//        24  m_MenuFont.m_hObject   (HFONT)
//
// This is the internal owner-draw menu used to render the ">>" chevron
// overflow popup.  MeasureItem sizes each item from its text using the menu
// font; DrawItem paints the background per selection state and draws the text.

#include <windows.h>
#include <cstddef>
#include "openmfc/afx.h"

#ifdef __GNUC__
#define MS_ABI __attribute__((ms_abi))
#else
#define MS_ABI
#endif


namespace openmfc { namespace detail { namespace cchevronownerdrawmenu {
struct S_Cchevronownerdrawmenu {
    void*   vfptr;             // 0
    HMENU   m_hMenu;           // 8
    void*   font_vfptr;        // 16  m_MenuFont.{vfptr}
    HGDIOBJ font_hObject;      // 24  m_MenuFont.m_hObject (HFONT)
};
} } }


// Lazily create the menu font from the current non-client metrics and cache it
// in the CFont subobject (m_MenuFont.m_hObject).  Returns a usable HFONT.
namespace openmfc { namespace detail { namespace cchevronownerdrawmenu {
HFONT EnsureMenuFont_Cchevronownerdrawmenu(S_Cchevronownerdrawmenu* self);
} } }

// ---- vtable slot helpers (non-exported virtuals) ----------------------------

// Slot 0: CMenu::GetRuntimeClass  — delegate to the real CMenu impl so the
// object reports RuntimeClass "CMenu" (this class is not DECLARE_DYNAMIC).
extern "C" void* MS_ABI
impl__GetRuntimeClass_CMenu__UEBAPEAUCRuntimeClass__XZ(const void* pThis);

namespace openmfc { namespace detail { namespace cchevronownerdrawmenu {
void* MS_ABI slot_GetRuntimeClass_Cchevronownerdrawmenu(const void* pThis);
} } }

// Slot 2: CObject::Serialize — base implementation is a no-op.
namespace openmfc { namespace detail { namespace cchevronownerdrawmenu {
void MS_ABI slot_Serialize_Cchevronownerdrawmenu(void* pThis, void* pAr);
} } }

// Slot 3: CObject::AssertValid — const, no-op in release.
namespace openmfc { namespace detail { namespace cchevronownerdrawmenu {
void MS_ABI slot_AssertValid_Cchevronownerdrawmenu(const void* pThis);
} } }

// Slot 4: CObject::Dump — const, no-op in release.
namespace openmfc { namespace detail { namespace cchevronownerdrawmenu {
void MS_ABI slot_Dump_Cchevronownerdrawmenu(const void* pThis, void* /*dc*/);
} } }


// -----------------------------------------------------------------------------
// Exported thunks
// -----------------------------------------------------------------------------

namespace openmfc { namespace detail { namespace cchevronownerdrawmenu {
extern void* const g_CChevronOwnerDrawMenu_vtbl[7];
} } }




// -----------------------------------------------------------------------------
// vtable
// -----------------------------------------------------------------------------


// Slot 1: CChevronOwnerDrawMenu::{vector deleting destructor}.
// Frees the cached menu font, then (if flag bit 0) releases the object.
namespace openmfc { namespace detail { namespace cchevronownerdrawmenu {
void* MS_ABI slot_vecDelDtor_Cchevronownerdrawmenu(void* pThis, unsigned flags);
} } }

namespace openmfc { namespace detail { namespace cchevronownerdrawmenu {
extern void* const g_CChevronOwnerDrawMenu_vtbl[7];
} } }

