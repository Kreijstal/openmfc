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

// Retail (0x180170c10) is a single 4-byte store to the m_bIsRTL static.
// Symbol: ?EnableRTL@CMFCToolBarImages@@SAXH@Z
extern "C" void MS_ABI impl__EnableRTL_CMFCToolBarImages__SAXH_Z(int bEnable)
{
    impl__m_bIsRTL_CMFCToolBarImages__1HA = bEnable;
}

// Retail (0x18016c4e0 in the old build; current 0x16c4f0) just returns 1.
// Symbol: ?Is32BitTransparencySupported@CMFCToolBarImages@@SAHXZ
extern "C" int MS_ABI impl__Is32BitTransparencySupported_CMFCToolBarImages__SAHXZ()
{
    return 1;
}

//=============================================================================
// Color mapping: MapToSysColor / MapToSysColorAlpha / MapFromSysColor
//=============================================================================

// Retail (0x18016e840) scans a 4-entry table at 0x180342d90:
//   { 0x000000, 0x12 }, { 0x808080, 0x10 }, { 0xC0C0C0, 0x0F }, { 0xFFFFFF, 0x14 }
// (fixed color, sys-color index: COLOR_BTNTEXT / COLOR_BTNSHADOW /
//  COLOR_BTNFACE / COLOR_BTNHIGHLIGHT).  On a match the CMFCColorMaker's
// color for that index is returned; the bHighContrast branch additionally
// channel-swaps the BGR value into RGB order (0x18016e894..).  The maker
// singleton is not modeled, so GetSysColor() approximates it (identical under
// default system colors).
// Symbol: ?MapToSysColor@CMFCToolBarImages@@SAKKH@Z
extern "C" unsigned long MS_ABI impl__MapToSysColor_CMFCToolBarImages__SAKKH_Z(
    unsigned long clr, int bHighContrast)
{
    static const struct { unsigned long clr; int nSys; } kTable[4] = {
        { 0x000000, COLOR_BTNTEXT },
        { 0x808080, COLOR_BTNSHADOW },
        { 0xC0C0C0, COLOR_BTNFACE },
        { 0xFFFFFF, COLOR_BTNHIGHLIGHT },
    };
    for (int i = 0; i < 4; ++i) {
        if (clr == kTable[i].clr) {
            const unsigned long sys = ::GetSysColor(kTable[i].nSys); // BGR
            if (bHighContrast) {
                // Retail swaps the maker's BGR color into RGB order.
                return ((sys & 0xFF) << 16) | (sys & 0xFF00) | ((sys >> 16) & 0xFF);
            }
            return sys;
        }
    }
    return clr;
}

// Retail (0x18016e940): a near-gray color (all pairwise channel deltas <= 10)
// is remapped onto the 3D button-face tone by scaling the face color with
// 1 + (channel - 192)/255 (helpers at 0x18034ff68=1.0, 0x180350090=192.0,
// 0x180350098=255.0 and the scale routine 0x18005b310); everything else is
// returned unchanged.  The global face color at 0x1803c1680 is approximated
// with GetSysColor(COLOR_BTNFACE).
// Symbol: ?MapToSysColorAlpha@CMFCToolBarImages@@SAKK@Z
extern "C" unsigned long MS_ABI impl__MapToSysColorAlpha_CMFCToolBarImages__SAKK_Z(
    unsigned long clr)
{
    const int r = static_cast<int>((clr >> 16) & 0xFF);
    const int g = static_cast<int>((clr >> 8) & 0xFF);
    const int b = static_cast<int>(clr & 0xFF);

    if (std::abs(r - g) > 10 || std::abs(g - b) > 10 || std::abs(r - b) > 10) {
        return clr;   // not gray -> unchanged (retail 0x18016ea0e)
    }

    const unsigned long sys = ::GetSysColor(COLOR_BTNFACE);
    const int sr = static_cast<int>((sys >> 16) & 0xFF);
    const int sg = static_cast<int>((sys >> 8) & 0xFF);
    const int sb = static_cast<int>(sys & 0xFF);

    // Scale channel ch by f, rounding like retail (round-half-up, clamp 255,
    // and a ch==0/f>1 special case from 0x18005b310).
    auto scale = [](int ch, double f) -> int {
        double v = static_cast<double>(ch) * f + 0.5;
        if (ch == 0 && f > 1.0) v = (f - 1.0) * 255.0 + 0.5;
        if (v > 255.0) return 255;
        if (v < 0.0) return 0;
        return static_cast<int>(v);
    };

    return (static_cast<unsigned long>(scale(sr, 1.0 + (r - 192) / 255.0)) << 16)
         | (static_cast<unsigned long>(scale(sg, 1.0 + (g - 192) / 255.0)) << 8)
         |  static_cast<unsigned long>(scale(sb, 1.0 + (b - 192) / 255.0));
}

// Retail (0x18016ea20) is the inverse of MapToSysColor: for each of the four
// table entries it compares clr against the maker's color for that entry
// (channel-swapped when bHighContrast, retail 0x18016ea6c..) and returns the
// fixed table color on a match.  The table colors are all pure gray, so the
// swap is a no-op on the return value.
// Symbol: ?MapFromSysColor@CMFCToolBarImages@@SAKKH@Z
extern "C" unsigned long MS_ABI impl__MapFromSysColor_CMFCToolBarImages__SAKKH_Z(
    unsigned long clr, int bHighContrast)
{
    static const struct { unsigned long clr; int nSys; } kTable[4] = {
        { 0x000000, COLOR_BTNTEXT },
        { 0x808080, COLOR_BTNSHADOW },
        { 0xC0C0C0, COLOR_BTNFACE },
        { 0xFFFFFF, COLOR_BTNHIGHLIGHT },
    };
    for (int i = 0; i < 4; ++i) {
        const unsigned long sys = ::GetSysColor(kTable[i].nSys); // BGR
        if (bHighContrast) {
            const unsigned long sw =
                ((sys & 0xFF) << 16) | (sys & 0xFF00) | ((sys >> 16) & 0xFF);
            if (clr == sw) return kTable[i].clr;
        } else {
            if (clr == sys) return kTable[i].clr;
        }
    }
    return clr;
}

//=============================================================================
// CopyTemp / CopyTo / Copy
//=============================================================================

// Retail (0x18016fe90): clear/init dst (helper 0x18016f690), then mark it
// temporary + read-only and copy size/well/count/bpp from src.  Transcribed
// 1:1; the helper is the public InitMembers() (via Clear() which also resets
// the side-table state the OpenMFC core keeps per object).
// Symbol: ?CopyTemp@CMFCToolBarImages@@IEAAXAEAV1@@Z
extern "C" void MS_ABI impl__CopyTemp_CMFCToolBarImages__IEAAXAEAV1__Z(
    CMFCToolBarImages* pThis, CMFCToolBarImages* pDst)
{
    if (!pThis || !pDst) return;

    pDst->Clear();                                  // 0x18016f690
    TI_OFF_BOOL(pDst, 0x2C) = TRUE;                 // m_bIsTemporary
    TI_OFF_8(pDst, 0x68) = TI_OFF_8(pThis, 0x68);   // m_sizeImage
    TI_OFF_8(pDst, 0x78) = TI_OFF_8(pThis, 0x78);   // m_sizeImageDest
    TI_OFF_PTR(pDst, 0xA0) = TI_OFF_PTR(pThis, 0xA0); // m_hbmImageWell
    TI_OFF_BOOL(pDst, 0x1C) = TI_OFF_BOOL(pThis, 0x1C); // m_bUserImagesList
    TI_OFF_INT(pDst, 0x08) = TI_OFF_INT(pThis, 0x08);   // m_iCount
    TI_OFF_BOOL(pDst, 0x28) = TRUE;                 // m_bReadOnly
    TI_OFF_INT(pDst, 0x0C) = TI_OFF_INT(pThis, 0x0C);   // m_nBitsPerPixel
}

// Retail (0x18016f2f0) deep-copies the image well through a DIB pipeline and
// resizes it; unmodeled (see Copy below for the plain HBITMAP copy helper).
// Symbol: ?CopyTo@CMFCToolBarImages@@QEAAHAEAV1@@Z
extern "C" int MS_ABI impl__CopyTo_CMFCToolBarImages__QEAAHAEAV1__Z(
    CMFCToolBarImages* pThis, CMFCToolBarImages* pDst)
{
    (void)pDst;
    if (!pThis) return 0;
    // TODO(clean-room): transcribed partially -- retail copies the bitmap
    // well into pDst through the DIB conversion helpers (0x18016aee0,
    // 0x18016ab40); not modeled.
    return 0;
}

// Retail (0x180171890): standard compatible-bitmap copy.  Create a compatible
// DC, select the source, GetObject for its size, CreateCompatibleBitmap, then
// BitBlt SRCCOPY through a second compatible DC.  Fully transcribed.
// Symbol: ?Copy@CMFCToolBarImages@@SAPEAUHBITMAP__@@PEAU2@@Z
extern "C" HBITMAP MS_ABI impl__Copy_CMFCToolBarImages__SAPEAUHBITMAP____PEAU2__Z(
    HBITMAP hBitmap)
{
    if (!hBitmap) return nullptr;

    HDC hdcSrc = ::CreateCompatibleDC(nullptr);
    if (!hdcSrc) return nullptr;
    HGDIOBJ hOldSrc = ::SelectObject(hdcSrc, hBitmap);
    if (!hOldSrc) {
        ::DeleteDC(hdcSrc);
        return nullptr;
    }

    BITMAP bmp;
    if (!::GetObjectW(hBitmap, sizeof(BITMAP), &bmp)) {
        ::SelectObject(hdcSrc, hOldSrc);
        ::DeleteDC(hdcSrc);
        return nullptr;
    }

    HBITMAP hNew = ::CreateCompatibleBitmap(hdcSrc, bmp.bmWidth, bmp.bmHeight);
    if (!hNew) {
        ::SelectObject(hdcSrc, hOldSrc);
        ::DeleteDC(hdcSrc);
        return nullptr;
    }

    HDC hdcDst = ::CreateCompatibleDC(nullptr);
    HGDIOBJ hOldDst = hdcDst ? ::SelectObject(hdcDst, hNew) : nullptr;
    if (!hOldDst) {
        ::DeleteObject(hNew);
        if (hdcDst) ::DeleteDC(hdcDst);
        ::SelectObject(hdcSrc, hOldSrc);
        ::DeleteDC(hdcSrc);
        return nullptr;
    }

    ::BitBlt(hdcDst, 0, 0, bmp.bmWidth, bmp.bmHeight, hdcSrc, 0, 0, SRCCOPY);

    ::SelectObject(hdcDst, hOldDst);
    ::SelectObject(hdcSrc, hOldSrc);
    ::DeleteDC(hdcDst);
    ::DeleteDC(hdcSrc);
    return hNew;
}

//=============================================================================
// Draw / DrawEx / EndDrawImage / FillDitheredRect / CopyImageToClipboard
//=============================================================================

// Retail (0x18016c6d0, ~9e0 bytes) draws image iImageIndex at (x, y) through
// BeginDrawImage / the mono memory DC / the alpha pipeline and the visual
// manager; not modeled.
// Symbol: ?Draw@CMFCToolBarImages@@QEAAHPEAVCDC@@HHHHHHHHE@Z
extern "C" int MS_ABI impl__Draw_CMFCToolBarImages__QEAAHPEAVCDC__HHHHHHHHE_Z(
    CMFCToolBarImages* pThis, CDC* pDC,
    int x, int y, int iImageIndex, int cx, int cy, int nGap,
    int bDisabled, int bShadow, unsigned char alphaSrc)
{
    (void)pDC; (void)x; (void)y; (void)iImageIndex; (void)cx; (void)cy;
    (void)nGap; (void)bDisabled; (void)bShadow; (void)alphaSrc;
    if (!pThis) return 0;
    // TODO(clean-room): transcribed partially -- the retail draw pipeline
    // (0x18016c6d0) is not modeled.
    return 0;
}

// Retail (0x18016d0b0, ~400 bytes) draws the image aligned inside rect using
// the rectSrc sub-image and the alpha pipeline; not modeled.
// Symbol: ?DrawEx@CMFCToolBarImages@@QEAAHPEAVCDC@@VCRect@@HW4ImageAlignHorz@1@W4ImageAlignVert@1@1E@Z
extern "C" int MS_ABI impl__DrawEx_CMFCToolBarImages__QEAAHPEAVCDC__VCRect__HW4ImageAlignHorz_1_W4ImageAlignVert_1_1E_Z(
    CMFCToolBarImages* pThis, CDC* pDC, CRect rect, int iImageIndex,
    int horzAlign, int vertAlign, CRect rectSrc, unsigned char alphaSrc)
{
    (void)pDC; (void)rect; (void)iImageIndex; (void)horzAlign; (void)vertAlign;
    (void)rectSrc; (void)alphaSrc;
    if (!pThis) return 0;
    // TODO(clean-room): transcribed partially -- the retail alignment draw
    // (0x18016d0b0) is not modeled.
    return 0;
}

// Retail (0x18016c270).  Observable member effects transcribed:
//   - m_bCreateMonoDC: reselect the saved mono/gray DC handles and delete the
//     saved palette.
//   - m_sizeImageDest and m_rectLastDraw are cleared.
//   - when m_bStretch or (bpp != 32 && clrTransparent != -1): restore the
//     original bitmap, delete m_bmpMem and the memory DC, clear m_pBmpOriginal.
// The global mono/gray DCs and the m_bIsDrawOnGlass cleanup are unmodeled.
// Symbol: ?EndDrawImage@CMFCToolBarImages@@QEAAXAEAUtagAFXDrawState@@@Z
extern "C" void MS_ABI impl__EndDrawImage_CMFCToolBarImages__QEAAXAEAUtagAFXDrawState___Z(
    CMFCToolBarImages* pThis, tagAFXDrawState* pState)
{
    if (!pThis) return;

    if (TI_OFF_BOOL(pThis, 0x44)) {                 // m_bCreateMonoDC
        // TODO(clean-room): retail reselects pState->hOldPattern into the
        // global mono DC (0x1803c39b8) and pState->hOldGrayPattern into the
        // gray DC (0x1803c38a8); not modeled.
        if (pState && pState->hOldPalette) {
            ::DeleteObject(pState->hOldPalette);
            pState->hOldPalette = nullptr;
        }
    }

    TI_OFF_8(pThis, 0x78) = 0;                      // m_sizeImageDest
    std::memset(reinterpret_cast<char*>(pThis) + 0x80, 0, 16); // m_rectLastDraw

    const BOOL bStretch = TI_OFF_BOOL(pThis, 0x24);
    const int nBpp = TI_OFF_INT(pThis, 0x0C);
    if (!bStretch && (nBpp != 32 &&
                      TI_OFF_ULONG(pThis, 0xD8) != static_cast<unsigned long>(-1))) {
        if (TI_OFF_PTR(pThis, 0xD0)) {              // m_pBmpOriginal
            // TODO(clean-room): retail first reselects the original bitmap
            // into m_dcMem (0x18016c2fb); the mem-DC contents are not modeled.
            // m_bmpMem @0xC0, m_dcMem @0x48 (both protected; reached by offset).
            reinterpret_cast<CBitmap*>(reinterpret_cast<char*>(pThis) + 0xC0)->DeleteObject();
            impl__DeleteDC_CDC__QEAAHXZ(
                reinterpret_cast<CDC*>(reinterpret_cast<char*>(pThis) + 0x48));
            TI_OFF_PTR(pThis, 0xD0) = nullptr;
        }
    }
}

// Retail (0x18016d4b0) fills the rect with the cached CMFCColorMaker dithered
// brush (8x8 face/hilite hatch) via FillRect.  The maker singleton is not
// modeled, so a solid button-face brush is used.
// Symbol: ?FillDitheredRect@CMFCToolBarImages@@SAXPEAVCDC@@AEBVCRect@@@Z
extern "C" void MS_ABI impl__FillDitheredRect_CMFCToolBarImages__SAXPEAVCDC__AEBVCRect___Z(
    CDC* pDC, const CRect* pRect)
{
    if (!pDC || !pRect) return;
    // TODO(clean-room): retail fills with the dithered brush owned by the
    // CMFCColorMaker (0x1803c1620); approximated here with the solid
    // button-face brush.
    HBRUSH hbr = ::GetSysColorBrush(COLOR_BTNFACE);
    if (!hbr) return;
    ::FillRect(pDC->GetSafeHdc(), *pRect, hbr);
}

// Retail (0x18016ef50) draws the image into a compatible bitmap and publishes
// it to the clipboard (OpenClipboard/EmptyClipboard/SetClipboardData(CF_BITMAP)
// /CloseClipboard).  Depends on the Draw pipeline; conservative.
// Symbol: ?CopyImageToClipboard@CMFCToolBarImages@@QEAAHH@Z
extern "C" int MS_ABI impl__CopyImageToClipboard_CMFCToolBarImages__QEAAHH_Z(
    CMFCToolBarImages* pThis, int iImageIndex)
{
    (void)iImageIndex;
    if (!pThis) return 0;
    // TODO(clean-room): transcribed partially -- retail renders the image
    // through Draw (0x18016c6d0) into a compatible bitmap and hands it to the
    // clipboard; the draw pipeline is not modeled.
    return 0;
}

//=============================================================================
// DeleteImage / ExtractIconW / CreateFromImageList
//=============================================================================

// Retail (0x18016e460) removes image iIndex from the well by BitBlt'ing the
// trailing images left and re-counting.  The DIB/DC machinery is unmodeled.
// Symbol: ?DeleteImage@CMFCToolBarImages@@QEAAHH@Z
extern "C" int MS_ABI impl__DeleteImage_CMFCToolBarImages__QEAAHH_Z(
    CMFCToolBarImages* pThis, int iIndex)
{
    if (!pThis) return 0;
    if (TI_OFF_BOOL(pThis, 0x2C)) return 0;         // m_bIsTemporary
    if (!TI_OFF_BOOL(pThis, 0x1C)) return 0;        // !m_bUserImagesList
    if (iIndex < 0 || iIndex >= TI_OFF_INT(pThis, 0x08)) return 0;
    if (!TI_OFF_PTR(pThis, 0xA0)) return 0;
    // TODO(clean-room): transcribed partially -- retail (0x18016e460) shifts
    // the trailing image rows in the well and recomputes m_iCount; not modeled.
    return 0;
}

// Retail (0x18016e6d0) extracts image iIndex as an HICON through a
// CImageList/CopyImage pipeline; conservative.
// Symbol: ?ExtractIconW@CMFCToolBarImages@@QEAAPEAUHICON__@@H@Z
extern "C" HICON MS_ABI impl__ExtractIconW_CMFCToolBarImages__QEAAPEAUHICON____H_Z(
    CMFCToolBarImages* pThis, int iIndex)
{
    if (!pThis) return nullptr;
    if (iIndex < 0 || iIndex >= TI_OFF_INT(pThis, 0x08)) return nullptr;
    // TODO(clean-room): transcribed partially -- retail (0x18016e6d0) crops
    // the iIndex-th sub-image and converts it with the ImageList/CopyImage
    // pipeline; not modeled.
    return nullptr;
}

// Retail (0x180170010): validate the list, clear, copy its icon size into
// m_sizeImage and add every icon through the internal AddBitmapToBmp helper.
// The helper is approximated by the public AddIcon() (tracks count in the
// OpenMFC side table).
// Symbol: ?CreateFromImageList@CMFCToolBarImages@@QEAAHAEBVCImageList@@@Z
extern "C" int MS_ABI impl__CreateFromImageList_CMFCToolBarImages__QEAAHAEBVCImageList___Z(
    CMFCToolBarImages* pThis, const CImageList* pList)
{
    if (!pThis || !pList) return 0;
    const HIMAGELIST himl = pList->GetSafeHandle();
    if (!himl) return 0;
    const int nCount = ::ImageList_GetImageCount(himl);
    if (nCount <= 0) return 0;

    pThis->Clear();                                 // 0x18016f690

    int cx = 0;
    int cy = 0;
    ::ImageList_GetIconSize(himl, &cx, &cy);
    TI_OFF_INT(pThis, 0x68) = cx;                   // m_sizeImage.cx
    TI_OFF_INT(pThis, 0x6C) = cy;                   // m_sizeImage.cy

    for (int i = 0; i < nCount; ++i) {
        HICON hIcon = ::ImageList_GetIcon(himl, i, 0);
        if (!hIcon) continue;
        pThis->AddIcon(hIcon, FALSE);               // 0x18016dff0
        ::DestroyIcon(hIcon);                       // 0x1802c7278
    }
    return 1;
}

//=============================================================================
// MirrorBitmap / MirrorBitmapVert / Mirror / MirrorVert
//=============================================================================

// Retail (0x180170510): horizontally flip each nWidth-wide sub-image by
// swapping 32bpp pixels directly in a DIB's bmBits.  Direct path transcribed;
// non-32bpp falls back to a GetBitmapBits/SetBitmapBits path (unmodeled).
// Symbol: ?MirrorBitmap@CMFCToolBarImages@@SAHAEAPEAUHBITMAP__@@H@Z
extern "C" int MS_ABI impl__MirrorBitmap_CMFCToolBarImages__SAHAEAPEAUHBITMAP____H_Z(
    HBITMAP* pHBitmap, int nWidth)
{
    if (!pHBitmap) return 0;
    if (!*pHBitmap) return 1;
    if (nWidth <= 0) return 0;

    BITMAP bmp;
    if (!::GetObjectW(*pHBitmap, sizeof(BITMAP), &bmp)) return 0;

    // Retail direct path requires a 32bpp DIB with bmBits (0x18017058a).
    if (bmp.bmBitsPixel != 32 || !bmp.bmBits) {
        // TODO(clean-room): retail falls back to GetBitmapBits/SetBitmapBits
        // row swapping for non-32bpp (0x180170686); not modeled.
        return 0;
    }

    const int nImages = bmp.bmWidth / nWidth;
    const int half = nWidth / 2;
    DWORD* const bits = static_cast<DWORD*>(bmp.bmBits);
    for (int iy = 0; iy < nImages; ++iy) {
        for (int y = 0; y < bmp.bmHeight; ++y) {
            DWORD* left = bits + static_cast<long>(iy) * nWidth
                               + static_cast<long>(y) * bmp.bmWidth;
            DWORD* right = left + nWidth - 1;
            for (int ic = 0; ic < half; ++ic) {
                const DWORD tmp = left[ic];
                left[ic] = right[-ic];
                right[-ic] = tmp;
            }
        }
    }
    return 1;
}

// Retail (0x180170870): vertically flip each nWidth-row-tall sub-image by
// swapping whole scan lines.  Direct path (bmPlanes == bmBitsPixel, bmBits
// present) transcribed; the GetBitmapBits fallback (0x180170a2c) is not.
// Symbol: ?MirrorBitmapVert@CMFCToolBarImages@@SAHAEAPEAUHBITMAP__@@H@Z
extern "C" int MS_ABI impl__MirrorBitmapVert_CMFCToolBarImages__SAHAEAPEAUHBITMAP____H_Z(
    HBITMAP* pHBitmap, int nWidth)
{
    if (!pHBitmap) return 0;
    if (!*pHBitmap) return 1;
    if (nWidth <= 0) return 0;

    BITMAP bmp;
    if (!::GetObjectW(*pHBitmap, sizeof(BITMAP), &bmp)) return 0;

    // Retail direct path requires bmBitsPixel >= 16, bmPlanes == bmBitsPixel
    // and bmBits (0x1801708e3 / 0x180170908).
    if (bmp.bmBitsPixel < 16 || bmp.bmPlanes != bmp.bmBitsPixel || !bmp.bmBits) {
        // TODO(clean-room): retail falls back to a GetBitmapBits/SetBitmapBits
        // row swap (0x180170a2c); not modeled.
        return 0;
    }

    const int nImages = bmp.bmWidth / nWidth;
    const int half = nWidth / 2;
    // (bmWidth * bpp + 7) / 8 rounded up to a 4-byte boundary (0x180170928..).
    int rowBytes = (bmp.bmWidth * bmp.bmBitsPixel + 7) / 8;
    rowBytes = (rowBytes + 3) & ~3;

    std::vector<unsigned char> tmp(static_cast<size_t>(rowBytes));
    unsigned char* const base = static_cast<unsigned char*>(bmp.bmBits);
    for (int iy = 0; iy < nImages; ++iy) {
        for (int y = 0; y < half; ++y) {
            unsigned char* top = base + static_cast<long>(iy) * nWidth * rowBytes
                                      + static_cast<long>(y) * rowBytes;
            unsigned char* bottom = base + static_cast<long>(iy) * nWidth * rowBytes
                                         + static_cast<long>(nWidth - 1 - y) * rowBytes;
            std::memcpy(tmp.data(), top, rowBytes);
            std::memcpy(top, bottom, rowBytes);
            std::memcpy(bottom, tmp.data(), rowBytes);
        }
    }
    return 1;
}

// Retail (0x180170810): MirrorBitmap the well, then the light/shadow bitmaps
// when present; returns the first result.
// Symbol: ?Mirror@CMFCToolBarImages@@QEAAHXZ
extern "C" int MS_ABI impl__Mirror_CMFCToolBarImages__QEAAHXZ(CMFCToolBarImages* pThis)
{
    if (!pThis) return 0;
    const int w = TI_OFF_INT(pThis, 0x68);          // m_sizeImage.cx
    if (!impl__MirrorBitmap_CMFCToolBarImages__SAHAEAPEAUHBITMAP____H_Z(
            reinterpret_cast<HBITMAP*>(reinterpret_cast<char*>(pThis) + 0xA0), w)) {
        return 0;
    }
    if (TI_OFF_PTR(pThis, 0xA8)) {                  // m_hbmImageLight
        impl__MirrorBitmap_CMFCToolBarImages__SAHAEAPEAUHBITMAP____H_Z(
            reinterpret_cast<HBITMAP*>(reinterpret_cast<char*>(pThis) + 0xA8), w);
    }
    if (TI_OFF_PTR(pThis, 0xB0)) {                  // m_hbmImageShadow
        impl__MirrorBitmap_CMFCToolBarImages__SAHAEAPEAUHBITMAP____H_Z(
            reinterpret_cast<HBITMAP*>(reinterpret_cast<char*>(pThis) + 0xB0), w);
    }
    return 1;
}

// Retail (0x180170bb0): same pattern as Mirror with MirrorBitmapVert and the
// image height (m_sizeImage.cy).
// Symbol: ?MirrorVert@CMFCToolBarImages@@QEAAHXZ
extern "C" int MS_ABI impl__MirrorVert_CMFCToolBarImages__QEAAHXZ(CMFCToolBarImages* pThis)
{
    if (!pThis) return 0;
    const int h = TI_OFF_INT(pThis, 0x6C);          // m_sizeImage.cy
    if (!impl__MirrorBitmapVert_CMFCToolBarImages__SAHAEAPEAUHBITMAP____H_Z(
            reinterpret_cast<HBITMAP*>(reinterpret_cast<char*>(pThis) + 0xA0), h)) {
        return 0;
    }
    if (TI_OFF_PTR(pThis, 0xA8)) {                  // m_hbmImageLight
        impl__MirrorBitmapVert_CMFCToolBarImages__SAHAEAPEAUHBITMAP____H_Z(
            reinterpret_cast<HBITMAP*>(reinterpret_cast<char*>(pThis) + 0xA8), h);
    }
    if (TI_OFF_PTR(pThis, 0xB0)) {                  // m_hbmImageShadow
        impl__MirrorBitmapVert_CMFCToolBarImages__SAHAEAPEAUHBITMAP____H_Z(
            reinterpret_cast<HBITMAP*>(reinterpret_cast<char*>(pThis) + 0xB0), h);
    }
    return 1;
}

//=============================================================================
// MapTo3dColors / MapBmpTo3dColors
//=============================================================================

// Forward declaration: defined below.
extern "C" int MS_ABI impl__MapBmpTo3dColors_CMFCToolBarImages__KAHAEAPEAUHBITMAP____HKK_Z(
    HBITMAP* pHBitmap, int bAutoCheckPremlt, unsigned long clrSrc, unsigned long clrDest);

// Retail (0x18016fe80) is a tail call: MapBmpTo3dColors(&m_hbmImageWell, ...).
// Symbol: ?MapTo3dColors@CMFCToolBarImages@@QEAAHHKK@Z
extern "C" int MS_ABI impl__MapTo3dColors_CMFCToolBarImages__QEAAHHKK_Z(
    CMFCToolBarImages* pThis, int bAutoCheckPremlt,
    unsigned long clrSrc, unsigned long clrDest)
{
    if (!pThis) return 0;
    return impl__MapBmpTo3dColors_CMFCToolBarImages__KAHAEAPEAUHBITMAP____HKK_Z(
        reinterpret_cast<HBITMAP*>(reinterpret_cast<char*>(pThis) + 0xA0),
        bAutoCheckPremlt, clrSrc, clrDest);
}

// Retail (0x18016fc10) walks the bitmap pixels replacing clrSrc with clrDest
// (with a pre-multiply auto-check).  Guards transcribed; pixel loop unmodeled.
// Symbol: ?MapBmpTo3dColors@CMFCToolBarImages@@KAHAEAPEAUHBITMAP__@@HKK@Z
extern "C" int MS_ABI impl__MapBmpTo3dColors_CMFCToolBarImages__KAHAEAPEAUHBITMAP____HKK_Z(
    HBITMAP* pHBitmap, int bAutoCheckPremlt,
    unsigned long clrSrc, unsigned long clrDest)
{
    (void)bAutoCheckPremlt;
    if (!pHBitmap || !*pHBitmap) return 0;
    if (clrDest == static_cast<unsigned long>(-1)) return 0;
    // TODO(clean-room): transcribed partially -- retail (0x18016fc10) creates
    // a copy of the bitmap and swaps clrSrc -> clrDest per pixel; not modeled.
    return 0;
}

//=============================================================================
// PreMultiplyAlpha
//=============================================================================

// Retail (0x180170000) tail-calls the internal premultiply helper
// (0x18016fef0) with (hbmp, m_bAutoCheckPremlt).  The helper is transcribed
// inline: reject non-32bpp/bitmap-less handles, optionally skip already
// premultiplied buffers, then multiply each channel by alpha/255 using the
// 0x80808081 magic constant (equivalent to integer division by 255).
// Symbol: ?PreMultiplyAlpha@CMFCToolBarImages@@IEAAHPEAUHBITMAP__@@@Z
extern "C" int MS_ABI impl__PreMultiplyAlpha_CMFCToolBarImages__IEAAHPEAUHBITMAP_____Z(
    CMFCToolBarImages* pThis, HBITMAP hbmp)
{
    if (!pThis) return 0;
    const BOOL bAutoCheckPremlt = TI_OFF_BOOL(pThis, 0x40);

    if (!hbmp) return 0;
    BITMAP bmp;
    if (!::GetObjectW(hbmp, sizeof(BITMAP), &bmp)) return 0;
    if (bmp.bmBitsPixel != 32 || !bmp.bmBits) return 0;

    const int nPixels = bmp.bmWidth * bmp.bmHeight;
    if (nPixels <= 0) return 1;

    unsigned char* const bits = static_cast<unsigned char*>(bmp.bmBits);
    if (bAutoCheckPremlt) {
        // Skip the conversion when every pixel is already premultiplied
        // (channel <= alpha, retail 0x18016ff50..).
        bool bNeedConvert = false;
        for (int i = 0; i < nPixels; ++i) {
            const unsigned char a = bits[i * 4 + 3];
            if (bits[i * 4 + 0] > a || bits[i * 4 + 1] > a || bits[i * 4 + 2] > a) {
                bNeedConvert = true;
                break;
            }
        }
        if (!bNeedConvert) return 1;
    }

    for (int i = 0; i < nPixels; ++i) {
        const unsigned int a = bits[i * 4 + 3];
        bits[i * 4 + 0] = static_cast<unsigned char>((bits[i * 4 + 0] * a) / 255);
        bits[i * 4 + 1] = static_cast<unsigned char>((bits[i * 4 + 1] * a) / 255);
        bits[i * 4 + 2] = static_cast<unsigned char>((bits[i * 4 + 2] * a) / 255);
    }
    return 1;
}

//=============================================================================
// GrayImages / OnSysColorChange
//=============================================================================

// Retail (0x180170110) flags the images gray and runs a gamma/LUT conversion
// over the well.  The state writes are transcribed; the pixel loop is not.
// Symbol: ?GrayImages@CMFCToolBarImages@@QEAAHH@Z
extern "C" int MS_ABI impl__GrayImages_CMFCToolBarImages__QEAAHH_Z(
    CMFCToolBarImages* pThis, int nLuminancePercentage)
{
    if (!pThis) return 0;
    TI_OFF_BOOL(pThis, 0x34) = TRUE;                // m_bIsGray
    TI_OFF_INT(pThis, 0x10) = nLuminancePercentage; // m_nGrayImageLuminancePercentage
    if (!TI_OFF_PTR(pThis, 0xA0)) return 0;         // no well -> FALSE
    // TODO(clean-room): transcribed partially -- retail (0x180170110) walks
    // the well pixels applying the luminance table; not modeled.
    return 0;
}

// Retail (0x18016d500, ~f60 bytes).  Observable member effects transcribed in
// order: when m_bIsTemporary the whole body is skipped; a scale != 1.0 resets
// m_dblScale/m_nBitsPerPixel, swaps the transparent colors, copies back
// m_sizeImageOriginal and clears the size/draw rects; then the count is
// recomputed from the well width, the well is mirrored when the m_bIsRTL
// static is set, the light/shadow bitmaps are deleted, gray images are
// re-grayed and m_clrImageShadow is refreshed.
// Symbol: ?OnSysColorChange@CMFCToolBarImages@@QEAAXXZ
extern "C" void MS_ABI impl__OnSysColorChange_CMFCToolBarImages__QEAAXXZ(
    CMFCToolBarImages* pThis)
{
    if (!pThis) return;

    if (TI_OFF_BOOL(pThis, 0x2C)) return;           // m_bIsTemporary

    if (TI_OFF_DBL(pThis, 0xE8) != 1.0) {           // m_dblScale
        TI_OFF_DBL(pThis, 0xE8) = 1.0;
        TI_OFF_INT(pThis, 0x0C) = 0;                // m_nBitsPerPixel
        const unsigned long clrOrig = TI_OFF_ULONG(pThis, 0xDC); // m_clrTransparentOriginal
        if (clrOrig != static_cast<unsigned long>(-1)) {
            TI_OFF_ULONG(pThis, 0xD8) = clrOrig;    // m_clrTransparent
            TI_OFF_ULONG(pThis, 0xDC) = static_cast<unsigned long>(-1);
        }
        TI_OFF_8(pThis, 0x68) = TI_OFF_8(pThis, 0x70); // m_sizeImage = m_sizeImageOriginal
        TI_OFF_8(pThis, 0x70) = 0;                      // m_sizeImageOriginal
        TI_OFF_8(pThis, 0x78) = 0;                      // m_sizeImageDest
        std::memset(reinterpret_cast<char*>(pThis) + 0x80, 0, 16); // m_rectLastDraw
        std::memset(reinterpret_cast<char*>(pThis) + 0x90, 0, 16); // m_rectSubImage
    }

    // TODO(clean-room): retail reloads the color map from m_strUDLPath
    // (0x18016b9e0) when m_bUserImagesList is set; the UDL parser is not
    // modeled.

    // UpdateCount (0x18016d840): m_iCount = wellWidth / m_sizeImage.cx.
    if (TI_OFF_PTR(pThis, 0xA0)) {
        BITMAP bmp;
        const int w = TI_OFF_INT(pThis, 0x68);
        if (w > 0 && ::GetObjectW(TI_OFF_PTR(pThis, 0xA0), sizeof(BITMAP), &bmp)) {
            TI_OFF_INT(pThis, 0x08) = bmp.bmWidth / w;
        }
    }

    if (impl__m_bIsRTL_CMFCToolBarImages__1HA) {
        const int w = TI_OFF_INT(pThis, 0x68);
        impl__MirrorBitmap_CMFCToolBarImages__SAHAEAPEAUHBITMAP____H_Z(
            reinterpret_cast<HBITMAP*>(reinterpret_cast<char*>(pThis) + 0xA0), w);
    }

    void** ppLight = reinterpret_cast<void**>(reinterpret_cast<char*>(pThis) + 0xA8);
    if (*ppLight) {                                 // m_hbmImageLight
        ::DeleteObject(*ppLight);
        *ppLight = nullptr;
    }
    void** ppShadow = reinterpret_cast<void**>(reinterpret_cast<char*>(pThis) + 0xB0);
    if (*ppShadow) {                                // m_hbmImageShadow
        ::DeleteObject(*ppShadow);
        *ppShadow = nullptr;
    }

    if (TI_OFF_BOOL(pThis, 0x34)) {                 // m_bIsGray
        impl__GrayImages_CMFCToolBarImages__QEAAHH_Z(pThis, TI_OFF_INT(pThis, 0x10));
    }

    // m_clrImageShadow = maker's shadow color (0x1803c164c), approximated
    // with the system button shadow color (identical under default colors).
    TI_OFF_ULONG(pThis, 0xE0) = ::GetSysColor(COLOR_BTNSHADOW);
}

//=============================================================================
// Mask / region helpers (conservative)
//=============================================================================

// Retail (0x18016c370) paints the well into the global mono/gray memory DCs
// with the mask colors 0xFF0062 / 0xEE0086 / 0x8800C6; the DCs are unmodeled.
// Symbol: ?CreateMask@CMFCToolBarImages@@IEAAXHHH@Z
extern "C" void MS_ABI impl__CreateMask_CMFCToolBarImages__IEAAXHHH_Z(
    CMFCToolBarImages* pThis, int nCount, int nBmpFirst, int nBmpLast)
{
    (void)nCount; (void)nBmpFirst; (void)nBmpLast;
    if (!pThis) return;
    // TODO(clean-room): transcribed partially -- retail (0x18016c370) builds
    // the light/shadow masks in the global memory DCs; not modeled.
}

// Retail (0x18016c510) runs BeginDrawImage/CreateMask/EndDrawImage and returns
// the mask bitmap built in the mono DC; conservative.
// Symbol: ?GetMask@CMFCToolBarImages@@QEAAPEAUHBITMAP__@@H@Z
extern "C" HBITMAP MS_ABI impl__GetMask_CMFCToolBarImages__QEAAPEAUHBITMAP____H_Z(
    CMFCToolBarImages* pThis, int iIndex)
{
    (void)iIndex;
    if (!pThis) return nullptr;
    // TODO(clean-room): transcribed partially -- retail (0x18016c510) returns
    // the mask bitmap produced in the global mono DC; not modeled.
    return nullptr;
}

// Retail (0x180171170) walks the bitmap pixels and grows an HRGN out of the
// runs of clrTransparent; conservative.
// Symbol: ?CreateRegionFromImage@CMFCToolBarImages@@SAPEAUHRGN__@@PEAUHBITMAP__@@K@Z
extern "C" HRGN MS_ABI impl__CreateRegionFromImage_CMFCToolBarImages__SAPEAUHRGN____PEAUHBITMAP____K_Z(
    HBITMAP hBitmap, unsigned long clrTransparent)
{
    (void)clrTransparent;
    if (!hBitmap) return nullptr;
    // TODO(clean-room): transcribed partially -- retail (0x180171170) scans
    // the bitmap and accumulates region rectangles; not modeled.
    return nullptr;
}

//=============================================================================
// AdaptColors / ConvertTo32Bits
//=============================================================================

// Retail (0x180170c20) recolorizes the well from clr1 to clr2 through the
// 3D-color machinery; conservative.
// Symbol: ?AdaptColors@CMFCToolBarImages@@QEAAXKK@Z
extern "C" void MS_ABI impl__AdaptColors_CMFCToolBarImages__QEAAXKK_Z(
    CMFCToolBarImages* pThis, unsigned long clr1, unsigned long clr2)
{
    (void)clr1; (void)clr2;
    if (!pThis) return;
    // TODO(clean-room): transcribed partially -- retail (0x180170c20) recolorizes
    // the well pixels through unmodeled color helpers.
}

// Retail (0x172140) converts the well to 32bpp and rescales it; conservative.
// Symbol: ?ConvertTo32Bits@CMFCToolBarImages@@QEAAHK@Z
extern "C" int MS_ABI impl__ConvertTo32Bits_CMFCToolBarImages__QEAAHK_Z(
    CMFCToolBarImages* pThis, unsigned long clrDest)
{
    (void)clrDest;
    if (!pThis) return 0;
    // TODO(clean-room): transcribed partially -- retail (0x172140) converts
    // the well through the 32bpp helper (0x180056660); not modeled.
    return 0;
}
