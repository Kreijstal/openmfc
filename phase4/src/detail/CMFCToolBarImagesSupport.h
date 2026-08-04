#pragma once
// Shared internals of the former global_mfc-feature-9_impl.cpp translation unit.
// Definitions live in detail/CMFCToolBarImagesSupport.cpp, so file-scope state keeps a
// single instance across the classes this unit was split into.
namespace openmfc { namespace detail { namespace cmfctoolbarimages {} } }
using namespace openmfc::detail::cmfctoolbarimages;
// CMFCToolBarImages Feature Pack exports, wave 9 ("mfc-feature-9").
//
// The 30 remaining CMFCToolBarImages methods, transcribed from the retail
// mfc140u exports (ordinal -> RVA -> objdump) against the 408-byte member
// layout documented in include/openmfc/afxmfc.h (members 0x08..0x198, statics
// in feature_static_data.cpp).
//
// Members touched by these exports are `protected`, so the extern "C" MS_ABI
// functions reach them through the documented offsets (house pattern from
// global_mfc-feature-5_impl.cpp::SetUserImages).  Functions whose retail body
// depends on the global memory DCs, the CMFCColorMaker singleton (0x1803c1620)
// or the BeginDrawImage/alpha pipeline are kept type-correct with safe
// defaults and marked `// TODO(clean-room)`.
//
// Faithful transcriptions:
//   EnableRTL, Is32BitTransparencySupported, CopyTemp, MapTo3dColors,
//   Mirror, MirrorVert, MirrorBitmap, MirrorBitmapVert, PreMultiplyAlpha,
//   Copy, CreateFromImageList, MapToSysColor, MapToSysColorAlpha,
//   MapFromSysColor, OnSysColorChange (observable member effects),
//   EndDrawImage (observable member effects), GrayImages (observable state).
//
// Conservative (safe default + TODO) for the pixel/gamma/alpha machinery:
//   Draw, DrawEx, CreateMask, GetMask, DeleteImage, ExtractIconW,
//   CopyTo, CopyImageToClipboard, MapBmpTo3dColors, AdaptColors,
//   ConvertTo32Bits, CreateRegionFromImage, FillDitheredRect.

#include "openmfc/afxmfc.h"

#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <vector>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

// ---------------------------------------------------------------------------
// Protected-member access through the documented retail offsets (afxmfc.h).
// ---------------------------------------------------------------------------
#define TI_OFF_INT(p, off)  (*reinterpret_cast<int*>(reinterpret_cast<char*>(p) + (off)))
#define TI_OFF_ULONG(p, off)(*reinterpret_cast<unsigned long*>(reinterpret_cast<char*>(p) + (off)))
#define TI_OFF_BOOL(p, off) (*reinterpret_cast<BOOL*>(reinterpret_cast<char*>(p) + (off)))
#define TI_OFF_PTR(p, off)  (*reinterpret_cast<void**>(reinterpret_cast<char*>(p) + (off)))
#define TI_OFF_8(p, off)    (*reinterpret_cast<unsigned long long*>(reinterpret_cast<char*>(p) + (off)))
#define TI_OFF_DBL(p, off)  (*reinterpret_cast<double*>(reinterpret_cast<char*>(p) + (off)))

// ---------------------------------------------------------------------------
// Cross-file impl_ helpers and statics.
// ---------------------------------------------------------------------------

// Static data, defined in feature_static_data.cpp (exported as
// ?m_bIsRTL@CMFCToolBarImages@@1HA).
extern "C" std::int32_t impl__m_bIsRTL_CMFCToolBarImages__1HA;

// Sibling impl_ exports defined elsewhere in phase4/src/.
// CDC::DeleteDC / CGdiObject::DeleteObject — impls in gdicore.cpp
// (mangled: ?DeleteDC@CDC@@QEAAHXZ / ?DeleteObject@CGdiObject@@QEAAHXZ)
extern "C" int MS_ABI impl__DeleteDC_CDC__QEAAHXZ(CDC* pThis);
extern "C" int MS_ABI impl__DeleteObject_CGdiObject__QEAAHXZ(CGdiObject* pThis);

// Retail AFXDrawState (tagAFXDrawState) passed to EndDrawImage by reference.
// Only the three handles EndDrawImage touches are modeled.
struct tagAFXDrawState {
    HGDIOBJ hOldPalette;    // 0x00 -- DeleteObject'd when m_bCreateMonoDC
    HGDIOBJ hOldPattern;    // 0x08 -- selected back into the mono memory DC
    HGDIOBJ hOldGrayPattern;// 0x10 -- selected back into the gray memory DC
};

//=============================================================================
// Static helpers: EnableRTL / Is32BitTransparencySupported
//=============================================================================



//=============================================================================
// Color mapping: MapToSysColor / MapToSysColorAlpha / MapFromSysColor
//=============================================================================




//=============================================================================
// CopyTemp / CopyTo / Copy
//=============================================================================




//=============================================================================
// Draw / DrawEx / EndDrawImage / FillDitheredRect / CopyImageToClipboard
//=============================================================================






//=============================================================================
// DeleteImage / ExtractIconW / CreateFromImageList
//=============================================================================




//=============================================================================
// MirrorBitmap / MirrorBitmapVert / Mirror / MirrorVert
//=============================================================================





//=============================================================================
// MapTo3dColors / MapBmpTo3dColors
//=============================================================================

// Forward declaration: defined below.
extern "C" int MS_ABI impl__MapBmpTo3dColors_CMFCToolBarImages__KAHAEAPEAUHBITMAP____HKK_Z(
    HBITMAP* pHBitmap, int bAutoCheckPremlt, unsigned long clrSrc, unsigned long clrDest);



//=============================================================================
// PreMultiplyAlpha
//=============================================================================


//=============================================================================
// GrayImages / OnSysColorChange
//=============================================================================



//=============================================================================
// Mask / region helpers (conservative)
//=============================================================================




//=============================================================================
// AdaptColors / ConvertTo32Bits
//=============================================================================


