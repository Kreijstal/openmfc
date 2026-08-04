#pragma once
// Shared internals of the former gdicore.cpp translation unit.
// Definitions live in detail/GdicoreSupport.cpp, so file-scope state keeps a
// single instance across the classes this unit was split into.
namespace openmfc { namespace detail { namespace gdicore {} } }
using namespace openmfc::detail::gdicore;
// Types declared by the internals of other units.
struct CD2DPointF;
struct CD2DSizeF;
// GDI Core Implementation
//
// Implements CDC, CGdiObject, and related GDI classes.
// These wrap Windows GDI functions for MFC compatibility.

#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxwin.h"
#include "openmfc/afxmfc.h"
#include <windows.h>
#include <d2d1.h>
#include <cmath>
#include <algorithm>
#include <cstring>
#include <mutex>
#include <unordered_map>
#include <vector>

// Thread-local temporary GDI object map for SelectObject return values
// This allows the common pattern: pOld = dc.SelectObject(&newPen); ... dc.SelectObject(pOld);
namespace openmfc { namespace detail { namespace gdicore {
extern thread_local std::unordered_map<HGDIOBJ, CGdiObject*> g_tempGdiMap;
} } }
namespace openmfc { namespace detail { namespace gdicore {
extern thread_local std::unordered_map<HDC, CDC*> g_tempDCMap;
} } }

namespace openmfc { namespace detail { namespace gdicore {
CGdiObject* GetTempGdiObject(HGDIOBJ hObj);
} } }

namespace openmfc { namespace detail { namespace gdicore {
void DeleteTempGdiMap();
} } }

namespace openmfc { namespace detail { namespace gdicore {
CDC* GetTempDC(HDC hDC);
} } }

namespace openmfc { namespace detail { namespace gdicore {
void DeleteTempDCMap();
} } }

namespace openmfc { namespace detail { namespace gdicore {
struct D2D_POINT_2F_L {
    float x;
    float y;
};
} } }

namespace openmfc { namespace detail { namespace gdicore {
struct D2D_SIZE_F_L {
    float width;
    float height;
};
} } }

namespace openmfc { namespace detail { namespace gdicore {
struct D2D_RECT_F_L {
    float left;
    float top;
    float right;
    float bottom;
};
} } }

namespace openmfc { namespace detail { namespace gdicore {
struct D2D1_ELLIPSE_L {
    D2D_POINT_2F_L point;
    float radiusX;
    float radiusY;
};
} } }

namespace openmfc { namespace detail { namespace gdicore {
struct D2D1_ROUNDED_RECT_L {
    D2D_RECT_F_L rect;
    float radiusX;
    float radiusY;
};
} } }

namespace openmfc { namespace detail { namespace gdicore {
struct CRenderTargetState {
    void* resource = nullptr;
    bool drawing = false;
    CD2DSizeF dpi = CD2DSizeF(96.0f, 96.0f);
    CD2DSizeF size = CD2DSizeF(0.0f, 0.0f);
    CString lastText;
    CD2DRectF lastTextRect;
    CD2DColorF lastClearColor = {};
    bool clearWasCalled = false;
    int drawCallCount = 0;
    unsigned __int64 tag1 = 0;
    unsigned __int64 tag2 = 0;
};
} } }

namespace openmfc { namespace detail { namespace gdicore {
struct CAnimationVariableState {
    double value = 0.0;
    std::vector<void*> transitions;
    bool valueChangedEventEnabled = false;
    bool integerValueChangedEventEnabled = false;
    int pendingAutodestroy = TRUE;
};
} } }

namespace openmfc { namespace detail { namespace gdicore {
extern std::unordered_map<const CRenderTarget*, CRenderTargetState> g_renderTargetState;
} } }
namespace openmfc { namespace detail { namespace gdicore {
extern std::unordered_map<const CAnimationVariable*, CAnimationVariableState> g_animationVariableState;
} } }
namespace openmfc { namespace detail { namespace gdicore {
extern std::mutex g_wave2StateMutex;
} } }

// MS ABI calling convention
#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

// =============================================================================
// CRuntimeClass implementations for GDI classes
// =============================================================================


#ifdef __GNUC__
// MSVC symbol aliases for runtime class statics
asm(".globl \"?classCGdiObject@CGdiObject@@2UCRuntimeClass@@A\"\n"
    ".set \"?classCGdiObject@CGdiObject@@2UCRuntimeClass@@A\", _ZN10CGdiObject15classCGdiObjectE\n");
asm(".globl \"?classCPen@CPen@@2UCRuntimeClass@@A\"\n"
    ".set \"?classCPen@CPen@@2UCRuntimeClass@@A\", _ZN4CPen9classCPenE\n");
asm(".globl \"?classCBrush@CBrush@@2UCRuntimeClass@@A\"\n"
    ".set \"?classCBrush@CBrush@@2UCRuntimeClass@@A\", _ZN6CBrush11classCBrushE\n");
asm(".globl \"?classCFont@CFont@@2UCRuntimeClass@@A\"\n"
    ".set \"?classCFont@CFont@@2UCRuntimeClass@@A\", _ZN5CFont10classCFontE\n");
asm(".globl \"?classCBitmap@CBitmap@@2UCRuntimeClass@@A\"\n"
    ".set \"?classCBitmap@CBitmap@@2UCRuntimeClass@@A\", _ZN7CBitmap12classCBitmapE\n");
asm(".globl \"?classCPalette@CPalette@@2UCRuntimeClass@@A\"\n"
    ".set \"?classCPalette@CPalette@@2UCRuntimeClass@@A\", _ZN8CPalette13classCPaletteE\n");
asm(".globl \"?classCRgn@CRgn@@2UCRuntimeClass@@A\"\n"
    ".set \"?classCRgn@CRgn@@2UCRuntimeClass@@A\", _ZN4CRgn9classCRgnE\n");
asm(".globl \"?classCDC@CDC@@2UCRuntimeClass@@A\"\n"
    ".set \"?classCDC@CDC@@2UCRuntimeClass@@A\", _ZN3CDC8classCDCE\n");
asm(".globl \"?classCClientDC@CClientDC@@2UCRuntimeClass@@A\"\n"
    ".set \"?classCClientDC@CClientDC@@2UCRuntimeClass@@A\", _ZN9CClientDC14classCClientDCE\n");
asm(".globl \"?classCPaintDC@CPaintDC@@2UCRuntimeClass@@A\"\n"
    ".set \"?classCPaintDC@CPaintDC@@2UCRuntimeClass@@A\", _ZN8CPaintDC13classCPaintDCE\n");
asm(".globl \"?classCWindowDC@CWindowDC@@2UCRuntimeClass@@A\"\n"
    ".set \"?classCWindowDC@CWindowDC@@2UCRuntimeClass@@A\", _ZN9CWindowDC14classCWindowDCE\n");
#endif

// =============================================================================
// CDC Implementation
// =============================================================================





// CDC::CreateCompatibleDC
namespace openmfc { namespace detail { namespace gdicore {
extern "C" int MS_ABI impl__CreateCompatibleDC_CDC__QEAAHPEAV1__Z(CDC* pThis, CDC* pDC);
} } }






// CDC::Rectangle
namespace openmfc { namespace detail { namespace gdicore {
extern "C" int MS_ABI impl__Rectangle_CDC__QEAAHHHHH_Z(CDC* pThis, int x1, int y1, int x2, int y2);
} } }

// CDC::Ellipse
namespace openmfc { namespace detail { namespace gdicore {
extern "C" int MS_ABI impl__Ellipse_CDC__QEAAHHHHH_Z(CDC* pThis, int x1, int y1, int x2, int y2);
} } }

// CDC::TextOutW
namespace openmfc { namespace detail { namespace gdicore {
extern "C" int MS_ABI impl__TextOutW_CDC__QEAAHHPEB_WH_Z(CDC* pThis, int x, int y, const wchar_t* lpszString, int nCount);
} } }

// CDC::SetBkColor
namespace openmfc { namespace detail { namespace gdicore {
extern "C" unsigned long MS_ABI impl__SetBkColor_CDC__QEAAKK_Z(CDC* pThis, unsigned long crColor);
} } }

// CDC::GetBkColor
namespace openmfc { namespace detail { namespace gdicore {
extern "C" unsigned long MS_ABI impl__GetBkColor_CDC__QEBAKXZ(const CDC* pThis);
} } }

// CDC::SetTextColor
namespace openmfc { namespace detail { namespace gdicore {
extern "C" unsigned long MS_ABI impl__SetTextColor_CDC__QEAAKK_Z(CDC* pThis, unsigned long crColor);
} } }

// CDC::GetTextColor
namespace openmfc { namespace detail { namespace gdicore {
extern "C" int MS_ABI impl__GetTextColor_CDC__QEBAHXZ(const CDC* pThis);
} } }


// CDC::GetBkMode
namespace openmfc { namespace detail { namespace gdicore {
extern "C" int MS_ABI impl__GetBkMode_CDC__QEBAHXZ(const CDC* pThis);
} } }

// CDC::BitBlt
namespace openmfc { namespace detail { namespace gdicore {
extern "C" int MS_ABI impl__BitBlt_CDC__QEAAHHHHHHPEAV1_HHK_Z(
    CDC* pThis, int x, int y, int nWidth, int nHeight,
    CDC* pSrcDC, int xSrc, int ySrc, unsigned long dwRop);
} } }

// CDC::StretchBlt
namespace openmfc { namespace detail { namespace gdicore {
extern "C" int MS_ABI impl__StretchBlt_CDC__QEAAHHHHHHPEAV1_HHHHK_Z(
    CDC* pThis, int x, int y, int nWidth, int nHeight,
    CDC* pSrcDC, int xSrc, int ySrc, int nSrcWidth, int nSrcHeight, unsigned long dwRop);
} } }

// CDC::FillRect
namespace openmfc { namespace detail { namespace gdicore {
extern "C" int MS_ABI impl__FillRect_CDC__QEAAHPEBUtagRECT__PEAVCBrush___Z(
    CDC* pThis, const RECT* lpRect, CBrush* pBrush);
} } }


// CDC::FrameRect
namespace openmfc { namespace detail { namespace gdicore {
extern "C" int MS_ABI impl__FrameRect_CDC__QEAAHPEBUtagRECT__PEAVCBrush___Z(
    CDC* pThis, const RECT* lpRect, CBrush* pBrush);
} } }

// CDC::DrawText
namespace openmfc { namespace detail { namespace gdicore {
extern "C" int MS_ABI impl__DrawTextW_CDC__QEAAHPEB_WHPEAUTAGRECT__I_Z(
    CDC* pThis, const wchar_t* lpszString, int nCount, RECT* lpRect, unsigned int nFormat);
} } }

// CDC::SelectObject (CGdiObject)
namespace openmfc { namespace detail { namespace gdicore {
extern "C" CGdiObject* MS_ABI impl__SelectObject_CDC__QEAAPEAVCGdiObject__PEAV2__Z(
    CDC* pThis, CGdiObject* pObject);
} } }

// CDC::SelectStockObject
namespace openmfc { namespace detail { namespace gdicore {
extern "C" int MS_ABI impl__SelectStockObject_CDC__QEAAHH_Z(CDC* pThis, int nIndex);
} } }

















// =============================================================================
// CGdiObject Implementation
// =============================================================================




#ifdef __GNUC__
// Alias for MinGW internal calls (Itanium mangling -> impl)
asm(".globl _ZN10CGdiObject12DeleteObjectEv\n"
    ".set _ZN10CGdiObject12DeleteObjectEv, impl__DeleteObject_CGdiObject__QEAAHXZ\n");
#endif



// =============================================================================
// CPen Implementation
// =============================================================================

// CPen default constructor
namespace openmfc { namespace detail { namespace gdicore {
extern "C" CPen* MS_ABI impl___0CPen__QEAA_XZ(CPen* pThis);
} } }


// CPen::CreatePen
namespace openmfc { namespace detail { namespace gdicore {
extern "C" int MS_ABI impl__CreatePen_CPen__QEAAHHHI_Z(CPen* pThis, int nPenStyle, int nWidth, unsigned int crColor);
} } }

// =============================================================================
// CBrush Implementation
// =============================================================================

// CBrush default constructor
namespace openmfc { namespace detail { namespace gdicore {
extern "C" CBrush* MS_ABI impl___0CBrush__QEAA_XZ(CBrush* pThis);
} } }

// C++ member (used by in-repo C++ code): default ctor, m_hObject = nullptr.




// CBrush::CreateSolidBrush
namespace openmfc { namespace detail { namespace gdicore {
extern "C" int MS_ABI impl__CreateSolidBrush_CBrush__QEAAHK_Z(CBrush* pThis, unsigned long crColor);
} } }

// CBrush::CreateHatchBrush
namespace openmfc { namespace detail { namespace gdicore {
extern "C" int MS_ABI impl__CreateHatchBrush_CBrush__QEAAHHK_Z(CBrush* pThis, int nIndex, unsigned long crColor);
} } }


// =============================================================================
// CFont Implementation
// =============================================================================

// CFont default constructor
namespace openmfc { namespace detail { namespace gdicore {
extern "C" CFont* MS_ABI impl___0CFont__QEAA_XZ(CFont* pThis);
} } }

// CFont::CreateFontIndirectW
namespace openmfc { namespace detail { namespace gdicore {
extern "C" int MS_ABI impl__CreateFontIndirectW_CFont__QEAAHPEBUtagLOGFONTW___Z(CFont* pThis, const LOGFONTW* lpLogFont);
} } }



// =============================================================================
// CBitmap Implementation
// =============================================================================

// CBitmap default constructor
namespace openmfc { namespace detail { namespace gdicore {
extern "C" CBitmap* MS_ABI impl___0CBitmap__QEAA_XZ(CBitmap* pThis);
} } }

// CBitmap::CreateCompatibleBitmap
namespace openmfc { namespace detail { namespace gdicore {
extern "C" int MS_ABI impl__CreateCompatibleBitmap_CBitmap__QEAAHPEAVCDC__HH_Z(
    CBitmap* pThis, CDC* pDC, int nWidth, int nHeight);
} } }

// CBitmap::LoadBitmapW
namespace openmfc { namespace detail { namespace gdicore {
extern "C" int MS_ABI impl__LoadBitmapW_CBitmap__QEAAHPEB_W_Z(CBitmap* pThis, const wchar_t* lpszResourceName);
} } }

// CBitmap::LoadBitmapW (ID version)
namespace openmfc { namespace detail { namespace gdicore {
extern "C" int MS_ABI impl__LoadBitmapW_CBitmap__QEAAHI_Z(CBitmap* pThis, UINT nIDResource);
} } }

// =============================================================================
// CClientDC Implementation
// =============================================================================



// =============================================================================
// CPaintDC Implementation
// =============================================================================



// =============================================================================
// CWindowDC Implementation
// =============================================================================



// =============================================================================
// CMetaFileDC Implementation
// =============================================================================


#ifdef __GNUC__
asm(".globl \"?classCMetaFileDC@CMetaFileDC@@2UCRuntimeClass@@A\"\n"
    ".set \"?classCMetaFileDC@CMetaFileDC@@2UCRuntimeClass@@A\", _ZN11CMetaFileDC16classCMetaFileDCE\n");
#endif

// CMetaFileDC default constructor

// CMetaFileDC destructor

// CMetaFileDC::Create - Create a Windows metafile DC

// CMetaFileDC::CreateEnhanced - Create an enhanced metafile DC

// CMetaFileDC::Close - Close metafile DC and return HMETAFILE

// CMetaFileDC::CloseEnhanced - Close enhanced metafile DC and return HENHMETAFILE

// =============================================================================
// CPalette Implementation
// =============================================================================

// CPalette default constructor

// CPalette destructor

// CPalette::CreatePalette

// CPalette::CreateHalftonePalette

// CPalette::GetEntryCount

// CPalette::GetPaletteEntries

// CPalette::SetPaletteEntries

// CPalette::AnimatePalette

// CPalette::ResizePalette

// CPalette::GetNearestPaletteIndex

// =============================================================================
// CRgn Implementation
// =============================================================================

// CRgn default constructor

// CRgn destructor

// CRgn::CreateRectRgn

// CRgn::CreateRectRgnIndirect

// CRgn::CreateEllipticRgn

// CRgn::CreateEllipticRgnIndirect

// CRgn::CreatePolygonRgn

// CRgn::CreateRoundRectRgn

// CRgn::CreateFromPath

// CRgn::SetRectRgn

// CRgn::SetRectRgn with RECT

// CRgn::CombineRgn

// CRgn::CopyRgn

// CRgn::EqualRgn

// CRgn::OffsetRgn

// CRgn::OffsetRgn with POINT

// CRgn::GetRgnBox

// CRgn::PtInRegion

// CRgn::PtInRegion with POINT

// CRgn::RectInRegion

// CRgn::GetRegionData

//=============================================================================
// CMFCToolBarImages
//=============================================================================

namespace openmfc { namespace detail { namespace gdicore {
struct ToolBarImagesState {
    int count = 0;
    BOOL isValid = FALSE;
    std::vector<HICON> ownedIcons;
};
} } }

namespace openmfc { namespace detail { namespace gdicore {
extern thread_local std::unordered_map<const CMFCToolBarImages*, ToolBarImagesState> g_toolbarImagesState;
} } }

namespace openmfc { namespace detail { namespace gdicore {
ToolBarImagesState& EnsureToolBarImagesState(const CMFCToolBarImages* pImages);
} } }

namespace openmfc { namespace detail { namespace gdicore {
const ToolBarImagesState* FindToolBarImagesState(const CMFCToolBarImages* pImages);
} } }

namespace openmfc { namespace detail { namespace gdicore {
void ClearToolBarImagesState(CMFCToolBarImages* pImages, BOOL bDestroyIcons);
} } }

namespace openmfc { namespace detail { namespace gdicore {
void RemoveToolBarImagesState(CMFCToolBarImages* pImages);
} } }

// Static member definitions - provided without // Symbol: comments so typed_stubs
// handles the MSVC-named exports. We define them here for C++ completeness but
// the linker uses the impl__ symbols from typed_stubs for the MSVC exports.

// m_bIsRTL / m_nDisabledImageAlpha / m_nFadedImageAlpha are static in retail
// MFC (AFX_IMPORT_DATA static in afxtoolbarimages.h), so they get definitions
// here rather than per-object initialization.

// Zero the retail member block and apply the non-zero retail defaults. Shared
// by both constructors.
#define InitImagesMembers() InitMembers()

















// --- extern "C" MS_ABI thunks ---
















// =============================================================================
// Wave 2 D2D/Animation minimal implementations
// =============================================================================






namespace openmfc { namespace detail { namespace gdicore {
CD2DPointF* InitCD2DPointF(CD2DPointF* pThis, float xValue, float yValue);
} } }

namespace openmfc { namespace detail { namespace gdicore {
CD2DSizeF* InitCD2DSizeF(CD2DSizeF* pThis, float widthValue, float heightValue);
} } }

namespace openmfc { namespace detail { namespace gdicore {
CD2DRectF* InitCD2DRectF(CD2DRectF* pThis, float leftValue, float topValue, float rightValue, float bottomValue);
} } }







































































































// =============================================================================
// CDC Wave 3: Missing CDC methods
// =============================================================================
























































// =============================================================================
// CImageList Implementation
// =============================================================================


#ifdef __GNUC__
asm(".globl \"?classCImageList@CImageList@@2UCRuntimeClass@@A\"\n"
    ".set \"?classCImageList@CImageList@@2UCRuntimeClass@@A\", _ZN10CImageList15classCImageListE\n");
#endif

// Thread-local temporary CImageList map for FromHandle
namespace openmfc { namespace detail { namespace gdicore {
extern thread_local std::unordered_map<HIMAGELIST, CImageList*> g_tempImageListMap;
} } }

namespace openmfc { namespace detail { namespace gdicore {
CImageList* GetTempImageList(HIMAGELIST hImageList);
} } }


























// CImageList::Add(HBITMAP, HBITMAP mask)

// CImageList::Add(HBITMAP, COLORREF crMask)

// CImageList::Add(HICON)

// CImageList::Replace(int, HBITMAP, HBITMAP)

// CImageList::Replace(int, HICON)

// CImageList::Remove

// CImageList::RemoveAll

// CImageList::ExtractIcon

// CImageList::GetImageCount

// CImageList::GetImageInfo

// CImageList::GetBkColor

// CImageList::SetBkColor

// CImageList::Draw

// CImageList::DrawEx





// Drag operations













