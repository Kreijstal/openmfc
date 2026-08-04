#pragma once
// Shared internals of the former global_cmfcdesktopalertwndbutton.cpp translation unit.
// Definitions live in detail/CMFCDesktopAlertWndButtonSupport.cpp, so file-scope state keeps a
// single instance across the classes this unit was split into.
namespace openmfc { namespace detail { namespace cmfcdesktopalertwndbutton {} } }
using namespace openmfc::detail::cmfcdesktopalertwndbutton;
// OpenMFC: real implementation of CMFCDesktopAlertWndButton exported virtual overrides.
//
// CMFCDesktopAlertWndButton (afxdesktopalertwnd.h) derives from CMFCButton and is used
// exclusively inside a CMFCDesktopAlertWnd (the "toast" popup) as its caption/close
// button. Two BOOL flags select which glyph the button paints:
//     m_bIsCaptionButton  -> the caption drop/menu button (down arrow)
//     m_bIsCloseButton    -> the close button (an "X")
//
// The class overrides three protected CMFCButton drawing virtuals:
//
//   OnFillBackground  -- deliberately paints NOTHING. The desktop-alert window itself
//                        paints its caption gradient behind the button, so the button
//                        must stay transparent; the override exists purely to suppress
//                        CMFCButton's own face fill. (Same suppression pattern used by
//                        CMFCTabButton in this codebase.)
//
//   OnDrawBorder      -- deliberately paints NOTHING in the resting state. The alert
//                        buttons are borderless chrome on the caption bar; the override
//                        suppresses CMFCButton's default border draw.
//
//   OnDraw            -- paints the actual glyph (close "X" or caption down-arrow) using
//                        the button-text system colour, centred in the supplied rect.
//                        Real mfc140u renders this glyph through CMenuImages; that private
//                        bitmap sheet is unavailable here, so the glyph is reproduced with
//                        equivalent GDI primitives -- same observable shape/placement.
//
// Only these three virtuals are exported (no ctor/dtor), so -- exactly as with
// CMFCTabButton -- no vptr installation or vtable authoring is required here; the layout
// struct exists only to anchor the static_asserts and to read the two glyph flags.

#include <windows.h>
#include <cstddef>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif


// Layout-faithful view. Harvested size(2856): CObject vfptr@0, CCmdTarget m_dwRef@8.
// The CMFCButton base occupies the first 2848 bytes (== full CMFCTabButton size); this
// class appends the two glyph flags in declaration order:
//     m_bIsCaptionButton @ 2848, m_bIsCloseButton @ 2852.
namespace openmfc { namespace detail { namespace cmfcdesktopalertwndbutton {
struct S_Cmfcdesktopalertwndbutton {
    void*         vfptr;             // 0:    CObject vtable pointer
    unsigned long m_dwRef;           // 8:    CCmdTarget reference count
    char          _base[2848 - 12];  //       remainder of the CMFCButton base
    int           m_bIsCaptionButton;// 2848: BOOL
    int           m_bIsCloseButton;  // 2852: BOOL
};
} } }


// CDC::m_hDC lives at offset 8 in the CDC object.
namespace openmfc { namespace detail { namespace cmfcdesktopalertwndbutton {
inline HDC hdc_of_Cmfcdesktopalertwndbutton(void* pDC) {
    if (pDC == nullptr) return nullptr;
    return *reinterpret_cast<HDC*>(reinterpret_cast<char*>(pDC) + 8);
}
} } }




