// GDI Core Implementation
//
// Implements CDC, CGdiObject, and related GDI classes.
// These wrap Windows GDI functions for MFC compatibility.

#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxwin.h"
#include <windows.h>
#include <cstring>

// MS ABI calling convention
#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

// =============================================================================
// CRuntimeClass implementations for GDI classes
// =============================================================================

IMPLEMENT_DYNAMIC(CGdiObject, CObject)
IMPLEMENT_DYNAMIC(CPen, CGdiObject)
IMPLEMENT_DYNAMIC(CBrush, CGdiObject)
IMPLEMENT_DYNAMIC(CFont, CGdiObject)
IMPLEMENT_DYNAMIC(CBitmap, CGdiObject)
IMPLEMENT_DYNAMIC(CPalette, CGdiObject)
IMPLEMENT_DYNAMIC(CRgn, CGdiObject)
IMPLEMENT_DYNAMIC(CDC, CObject)
IMPLEMENT_DYNAMIC(CClientDC, CDC)
IMPLEMENT_DYNAMIC(CPaintDC, CDC)
IMPLEMENT_DYNAMIC(CWindowDC, CDC)

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

// CDC default constructor
// Symbol: ??0CDC@@QEAA@XZ
// Ordinal: 438
extern "C" CDC* MS_ABI stub___0CDC__QEAA_XZ(CDC* pThis) {
    if (!pThis) return nullptr;

    // Initialize members
    pThis->m_hDC = nullptr;
    pThis->m_hAttribDC = nullptr;
    return pThis;
}

// CDC destructor
// Symbol: ??1CDC@@UEAA@XZ
// Ordinal: 1086
extern "C" void MS_ABI stub___1CDC__UEAA_XZ(CDC* pThis) {
    if (pThis && pThis->m_hDC) {
        ::DeleteDC(pThis->m_hDC);
        pThis->m_hDC = nullptr;
    }
}

// CDC::CreateCompatibleDC
// Symbol: ?CreateCompatibleDC@CDC@@QEAAHPEAV1@@Z
extern "C" int MS_ABI stub__CreateCompatibleDC_CDC__QEAAHPEAV1__Z(CDC* pThis, CDC* pDC) {
    if (!pThis) return FALSE;

    HDC hDC = pDC ? pDC->m_hDC : nullptr;
    pThis->m_hDC = ::CreateCompatibleDC(hDC);
    pThis->m_hAttribDC = pThis->m_hDC;
    return pThis->m_hDC != nullptr;
}

// CDC::DeleteDC
// Symbol: ?DeleteDC@CDC@@QEAAHXZ
extern "C" int MS_ABI stub__DeleteDC_CDC__QEAAHXZ(CDC* pThis) {
    if (!pThis || !pThis->m_hDC) return FALSE;

    int result = ::DeleteDC(pThis->m_hDC);
    pThis->m_hDC = nullptr;
    pThis->m_hAttribDC = nullptr;
    return result;
}

// CDC::SaveDC
// Symbol: ?SaveDC@CDC@@QEAAHXZ
extern "C" int MS_ABI stub__SaveDC_CDC__QEAAHXZ(CDC* pThis) {
    if (!pThis || !pThis->m_hDC) return 0;
    return ::SaveDC(pThis->m_hDC);
}

// CDC::RestoreDC
// Symbol: ?RestoreDC@CDC@@QEAAHH@Z
extern "C" int MS_ABI stub__RestoreDC_CDC__QEAAHH_Z(CDC* pThis, int nSavedDC) {
    if (!pThis || !pThis->m_hDC) return FALSE;
    return ::RestoreDC(pThis->m_hDC, nSavedDC);
}

// CDC::MoveTo (returns CPoint)
// Symbol: ?MoveTo@CDC@@QEAA?AVCPoint@@HH@Z
// Ordinal: 8501
extern "C" CPoint MS_ABI stub__MoveTo_CDC__QEAA_AVCPoint__HH_Z(CDC* pThis, int x, int y) {
    CPoint pt(0, 0);
    if (pThis && pThis->m_hDC) {
        POINT oldPt;
        ::MoveToEx(pThis->m_hDC, x, y, &oldPt);
        pt.x = oldPt.x;
        pt.y = oldPt.y;
    }
    return pt;
}

// CDC::LineTo
// Symbol: ?LineTo@CDC@@QEAAHHH@Z
// Ordinal: 8063
extern "C" int MS_ABI stub__LineTo_CDC__QEAAHHH_Z(CDC* pThis, int x, int y) {
    if (!pThis || !pThis->m_hDC) return FALSE;
    return ::LineTo(pThis->m_hDC, x, y);
}

// CDC::Rectangle
// Symbol: ?Rectangle@CDC@@QEAAHHHHH@Z
extern "C" int MS_ABI stub__Rectangle_CDC__QEAAHHHHH_Z(CDC* pThis, int x1, int y1, int x2, int y2) {
    if (!pThis || !pThis->m_hDC) return FALSE;
    return ::Rectangle(pThis->m_hDC, x1, y1, x2, y2);
}

// CDC::Ellipse
// Symbol: ?Ellipse@CDC@@QEAAHHHHH@Z
extern "C" int MS_ABI stub__Ellipse_CDC__QEAAHHHHH_Z(CDC* pThis, int x1, int y1, int x2, int y2) {
    if (!pThis || !pThis->m_hDC) return FALSE;
    return ::Ellipse(pThis->m_hDC, x1, y1, x2, y2);
}

// CDC::TextOutW
// Symbol: ?TextOutW@CDC@@QEAAHHPEB_WH@Z
extern "C" int MS_ABI stub__TextOutW_CDC__QEAAHHPEB_WH_Z(CDC* pThis, int x, int y, const wchar_t* lpszString, int nCount) {
    if (!pThis || !pThis->m_hDC) return FALSE;
    if (nCount < 0 && lpszString) {
        nCount = (int)wcslen(lpszString);
    }
    return ::TextOutW(pThis->m_hDC, x, y, lpszString, nCount);
}

// CDC::SetBkColor
// Symbol: ?SetBkColor@CDC@@QEAAKK@Z
extern "C" unsigned long MS_ABI stub__SetBkColor_CDC__QEAAKK_Z(CDC* pThis, unsigned long crColor) {
    if (!pThis || !pThis->m_hDC) return CLR_INVALID;
    return ::SetBkColor(pThis->m_hDC, crColor);
}

// CDC::GetBkColor
// Symbol: ?GetBkColor@CDC@@QEBAKXZ
extern "C" unsigned long MS_ABI stub__GetBkColor_CDC__QEBAKXZ(const CDC* pThis) {
    if (!pThis || !pThis->m_hDC) return CLR_INVALID;
    return ::GetBkColor(pThis->m_hDC);
}

// CDC::SetTextColor
// Symbol: ?SetTextColor@CDC@@QEAAKK@Z
extern "C" unsigned long MS_ABI stub__SetTextColor_CDC__QEAAKK_Z(CDC* pThis, unsigned long crColor) {
    if (!pThis || !pThis->m_hDC) return CLR_INVALID;
    return ::SetTextColor(pThis->m_hDC, crColor);
}

// CDC::GetTextColor
// Symbol: ?GetTextColor@CDC@@QEBAHXZ
extern "C" int MS_ABI stub__GetTextColor_CDC__QEBAHXZ(const CDC* pThis) {
    if (!pThis || !pThis->m_hDC) return 0;
    return (int)::GetTextColor(pThis->m_hDC);
}

// CDC::SetBkMode
// Symbol: ?SetBkMode@CDC@@QEAAHH@Z
extern "C" int MS_ABI stub__SetBkMode_CDC__QEAAHH_Z(CDC* pThis, int nBkMode) {
    if (!pThis || !pThis->m_hDC) return 0;
    return ::SetBkMode(pThis->m_hDC, nBkMode);
}

// CDC::GetBkMode
// Symbol: ?GetBkMode@CDC@@QEBAHXZ
extern "C" int MS_ABI stub__GetBkMode_CDC__QEBAHXZ(const CDC* pThis) {
    if (!pThis || !pThis->m_hDC) return 0;
    return ::GetBkMode(pThis->m_hDC);
}

// CDC::BitBlt
// Symbol: ?BitBlt@CDC@@QEAAHHHHHHPEAV1@HHK@Z
extern "C" int MS_ABI stub__BitBlt_CDC__QEAAHHHHHHPEAV1_HHK_Z(
    CDC* pThis, int x, int y, int nWidth, int nHeight,
    CDC* pSrcDC, int xSrc, int ySrc, unsigned long dwRop) {
    if (!pThis || !pThis->m_hDC) return FALSE;
    HDC hSrcDC = pSrcDC ? pSrcDC->m_hDC : nullptr;
    return ::BitBlt(pThis->m_hDC, x, y, nWidth, nHeight, hSrcDC, xSrc, ySrc, dwRop);
}

// CDC::StretchBlt
// Symbol: ?StretchBlt@CDC@@QEAAHHHHHHPEAV1@HHHHK@Z
extern "C" int MS_ABI stub__StretchBlt_CDC__QEAAHHHHHHPEAV1_HHHHK_Z(
    CDC* pThis, int x, int y, int nWidth, int nHeight,
    CDC* pSrcDC, int xSrc, int ySrc, int nSrcWidth, int nSrcHeight, unsigned long dwRop) {
    if (!pThis || !pThis->m_hDC) return FALSE;
    HDC hSrcDC = pSrcDC ? pSrcDC->m_hDC : nullptr;
    return ::StretchBlt(pThis->m_hDC, x, y, nWidth, nHeight,
                        hSrcDC, xSrc, ySrc, nSrcWidth, nSrcHeight, dwRop);
}

// CDC::FillRect
// Symbol: ?FillRect@CDC@@QEAAHPEBUtagRECT@@PEAVCBrush@@@Z
extern "C" int MS_ABI stub__FillRect_CDC__QEAAHPEBUtagRECT__PEAVCBrush___Z(
    CDC* pThis, const RECT* lpRect, CBrush* pBrush) {
    if (!pThis || !pThis->m_hDC || !lpRect) return FALSE;
    HBRUSH hBrush = pBrush ? (HBRUSH)pBrush->GetSafeHandle() : nullptr;
    return ::FillRect(pThis->m_hDC, lpRect, hBrush);
}

// CDC::FrameRect
// Symbol: ?FrameRect@CDC@@QEAAHPEBUtagRECT@@PEAVCBrush@@@Z
extern "C" int MS_ABI stub__FrameRect_CDC__QEAAHPEBUtagRECT__PEAVCBrush___Z(
    CDC* pThis, const RECT* lpRect, CBrush* pBrush) {
    if (!pThis || !pThis->m_hDC || !lpRect) return FALSE;
    HBRUSH hBrush = pBrush ? (HBRUSH)pBrush->GetSafeHandle() : nullptr;
    return ::FrameRect(pThis->m_hDC, lpRect, hBrush);
}

// CDC::DrawText
// Symbol: ?DrawTextW@CDC@@QEAAHPEB_WHPEAUTAGRECT@@I@Z
extern "C" int MS_ABI stub__DrawTextW_CDC__QEAAHPEB_WHPEAUTAGRECT__I_Z(
    CDC* pThis, const wchar_t* lpszString, int nCount, RECT* lpRect, unsigned int nFormat) {
    if (!pThis || !pThis->m_hDC) return 0;
    return ::DrawTextW(pThis->m_hDC, lpszString, nCount, lpRect, nFormat);
}

// CDC::SelectObject (CGdiObject)
// Symbol: ?SelectObject@CDC@@QEAAPEAVCGdiObject@@PEAV2@@Z
extern "C" CGdiObject* MS_ABI stub__SelectObject_CDC__QEAAPEAVCGdiObject__PEAV2__Z(
    CDC* pThis, CGdiObject* pObject) {
    if (!pThis || !pThis->m_hDC || !pObject) return nullptr;
    HGDIOBJ hOld = ::SelectObject(pThis->m_hDC, pObject->GetSafeHandle());
    (void)hOld; // In real MFC, this would look up or create a CGdiObject wrapper
    return nullptr; // Simplified - just select, don't return old object
}

// CDC::SelectStockObject
// Symbol: ?SelectStockObject@CDC@@QEAAHH@Z
extern "C" int MS_ABI stub__SelectStockObject_CDC__QEAAHH_Z(CDC* pThis, int nIndex) {
    if (!pThis || !pThis->m_hDC) return FALSE;
    HGDIOBJ hObj = ::GetStockObject(nIndex);
    return ::SelectObject(pThis->m_hDC, hObj) != nullptr;
}

// =============================================================================
// CGdiObject Implementation
// =============================================================================

// CGdiObject::DeleteObject
// Symbol: ?DeleteObject@CGdiObject@@QEAAHXZ
extern "C" int MS_ABI stub__DeleteObject_CGdiObject__QEAAHXZ(CGdiObject* pThis) {
    if (!pThis || !pThis->m_hObject) return FALSE;
    int result = ::DeleteObject(pThis->m_hObject);
    pThis->m_hObject = nullptr;
    return result;
}

// CGdiObject::Attach
// Symbol: ?Attach@CGdiObject@@QEAAHPEAX@Z
extern "C" int MS_ABI stub__Attach_CGdiObject__QEAAHPEAX_Z(CGdiObject* pThis, HGDIOBJ hObject) {
    if (!pThis) return FALSE;
    pThis->m_hObject = hObject;
    return TRUE;
}

// CGdiObject::Detach
// Symbol: ?Detach@CGdiObject@@QEAAPEAXXZ
extern "C" HGDIOBJ MS_ABI stub__Detach_CGdiObject__QEAAPEAXXZ(CGdiObject* pThis) {
    if (!pThis) return nullptr;
    HGDIOBJ h = pThis->m_hObject;
    pThis->m_hObject = nullptr;
    return h;
}

// =============================================================================
// CPen Implementation
// =============================================================================

// CPen default constructor
// Symbol: ??0CPen@@QEAA@XZ
extern "C" CPen* MS_ABI stub___0CPen__QEAA_XZ(CPen* pThis) {
    if (!pThis) return nullptr;
    pThis->m_hObject = nullptr;
    return pThis;
}

// CPen constructor with parameters
// Symbol: ??0CPen@@QEAA@HHK@Z
extern "C" CPen* MS_ABI stub___0CPen__QEAA_HHK_Z(CPen* pThis, int nPenStyle, int nWidth, unsigned long crColor) {
    if (!pThis) return nullptr;
    pThis->m_hObject = ::CreatePen(nPenStyle, nWidth, crColor);
    return pThis;
}

// CPen::CreatePen
// Symbol: ?CreatePen@CPen@@QEAAHHHI@Z
extern "C" int MS_ABI stub__CreatePen_CPen__QEAAHHHI_Z(CPen* pThis, int nPenStyle, int nWidth, unsigned int crColor) {
    if (!pThis) return FALSE;
    if (pThis->m_hObject) {
        ::DeleteObject(pThis->m_hObject);
    }
    pThis->m_hObject = ::CreatePen(nPenStyle, nWidth, crColor);
    return pThis->m_hObject != nullptr;
}

// =============================================================================
// CBrush Implementation
// =============================================================================

// CBrush default constructor
// Symbol: ??0CBrush@@QEAA@XZ
extern "C" CBrush* MS_ABI stub___0CBrush__QEAA_XZ(CBrush* pThis) {
    if (!pThis) return nullptr;
    pThis->m_hObject = nullptr;
    return pThis;
}

// CBrush constructor with color
// Symbol: ??0CBrush@@QEAA@K@Z
extern "C" CBrush* MS_ABI stub___0CBrush__QEAA_K_Z(CBrush* pThis, unsigned long crColor) {
    if (!pThis) return nullptr;
    pThis->m_hObject = ::CreateSolidBrush(crColor);
    return pThis;
}

// CBrush::CreateSolidBrush
// Symbol: ?CreateSolidBrush@CBrush@@QEAAHK@Z
extern "C" int MS_ABI stub__CreateSolidBrush_CBrush__QEAAHK_Z(CBrush* pThis, unsigned long crColor) {
    if (!pThis) return FALSE;
    if (pThis->m_hObject) {
        ::DeleteObject(pThis->m_hObject);
    }
    pThis->m_hObject = ::CreateSolidBrush(crColor);
    return pThis->m_hObject != nullptr;
}

// CBrush::CreateHatchBrush
// Symbol: ?CreateHatchBrush@CBrush@@QEAAHHK@Z
extern "C" int MS_ABI stub__CreateHatchBrush_CBrush__QEAAHHK_Z(CBrush* pThis, int nIndex, unsigned long crColor) {
    if (!pThis) return FALSE;
    if (pThis->m_hObject) {
        ::DeleteObject(pThis->m_hObject);
    }
    pThis->m_hObject = ::CreateHatchBrush(nIndex, crColor);
    return pThis->m_hObject != nullptr;
}

// =============================================================================
// CFont Implementation
// =============================================================================

// CFont default constructor
// Symbol: ??0CFont@@QEAA@XZ
extern "C" CFont* MS_ABI stub___0CFont__QEAA_XZ(CFont* pThis) {
    if (!pThis) return nullptr;
    pThis->m_hObject = nullptr;
    return pThis;
}

// CFont::CreateFontIndirectW
// Symbol: ?CreateFontIndirectW@CFont@@QEAAHPEBUtagLOGFONTW@@@Z
extern "C" int MS_ABI stub__CreateFontIndirectW_CFont__QEAAHPEBUtagLOGFONTW___Z(CFont* pThis, const LOGFONTW* lpLogFont) {
    if (!pThis || !lpLogFont) return FALSE;
    if (pThis->m_hObject) {
        ::DeleteObject(pThis->m_hObject);
    }
    pThis->m_hObject = ::CreateFontIndirectW(lpLogFont);
    return pThis->m_hObject != nullptr;
}

// CFont::CreatePointFont
// Symbol: ?CreatePointFont@CFont@@QEAAHHPEB_WPEAV1@@Z
extern "C" int MS_ABI stub__CreatePointFont_CFont__QEAAHHPEB_WPEAV1__Z(
    CFont* pThis, int nPointSize, const wchar_t* lpszFaceName, CDC* pDC) {
    if (!pThis) return FALSE;

    LOGFONTW lf = {};
    lf.lfHeight = -MulDiv(nPointSize,
        pDC ? ::GetDeviceCaps(pDC->m_hDC, LOGPIXELSY) : 96,
        720);
    lf.lfWeight = FW_NORMAL;
    if (lpszFaceName) {
        wcsncpy(lf.lfFaceName, lpszFaceName, LF_FACESIZE - 1);
    }

    if (pThis->m_hObject) {
        ::DeleteObject(pThis->m_hObject);
    }
    pThis->m_hObject = ::CreateFontIndirectW(&lf);
    return pThis->m_hObject != nullptr;
}

// =============================================================================
// CBitmap Implementation
// =============================================================================

// CBitmap default constructor
// Symbol: ??0CBitmap@@QEAA@XZ
extern "C" CBitmap* MS_ABI stub___0CBitmap__QEAA_XZ(CBitmap* pThis) {
    if (!pThis) return nullptr;
    pThis->m_hObject = nullptr;
    return pThis;
}

// CBitmap::CreateCompatibleBitmap
// Symbol: ?CreateCompatibleBitmap@CBitmap@@QEAAHPEAVCDC@@HH@Z
extern "C" int MS_ABI stub__CreateCompatibleBitmap_CBitmap__QEAAHPEAVCDC__HH_Z(
    CBitmap* pThis, CDC* pDC, int nWidth, int nHeight) {
    if (!pThis || !pDC || !pDC->m_hDC) return FALSE;
    if (pThis->m_hObject) {
        ::DeleteObject(pThis->m_hObject);
    }
    pThis->m_hObject = ::CreateCompatibleBitmap(pDC->m_hDC, nWidth, nHeight);
    return pThis->m_hObject != nullptr;
}

// CBitmap::LoadBitmapW
// Symbol: ?LoadBitmapW@CBitmap@@QEAAHPEB_W@Z
extern "C" int MS_ABI stub__LoadBitmapW_CBitmap__QEAAHPEB_W_Z(CBitmap* pThis, const wchar_t* lpszResourceName) {
    if (!pThis) return FALSE;
    if (pThis->m_hObject) {
        ::DeleteObject(pThis->m_hObject);
    }
    HINSTANCE hInst = AfxGetInstanceHandle();
    pThis->m_hObject = ::LoadBitmapW(hInst, lpszResourceName);
    return pThis->m_hObject != nullptr;
}

// CBitmap::LoadBitmapW (ID version)
// Symbol: ?LoadBitmapW@CBitmap@@QEAAHI@Z
extern "C" int MS_ABI stub__LoadBitmapW_CBitmap__QEAAHI_Z(CBitmap* pThis, UINT nIDResource) {
    if (!pThis) return FALSE;
    if (pThis->m_hObject) {
        ::DeleteObject(pThis->m_hObject);
    }
    HINSTANCE hInst = AfxGetInstanceHandle();
    pThis->m_hObject = ::LoadBitmapW(hInst, MAKEINTRESOURCEW(nIDResource));
    return pThis->m_hObject != nullptr;
}

// =============================================================================
// CClientDC Implementation
// =============================================================================

// CClientDC constructor
// Symbol: ??0CClientDC@@QEAA@PEAVCWnd@@@Z
extern "C" CClientDC* MS_ABI stub___0CClientDC__QEAA_PEAVCWnd___Z(CClientDC* pThis, CWnd* pWnd) {
    if (!pThis) return nullptr;

    pThis->m_hDC = nullptr;
    pThis->m_hAttribDC = nullptr;
    pThis->m_pWnd = pWnd;

    HWND hWnd = pWnd ? pWnd->GetSafeHwnd() : nullptr;
    pThis->m_hDC = ::GetDC(hWnd);
    pThis->m_hAttribDC = pThis->m_hDC;

    return pThis;
}

// CClientDC destructor
// Symbol: ??1CClientDC@@UEAA@XZ
extern "C" void MS_ABI stub___1CClientDC__UEAA_XZ(CClientDC* pThis) {
    if (pThis && pThis->m_hDC) {
        HWND hWnd = pThis->m_pWnd ? pThis->m_pWnd->GetSafeHwnd() : nullptr;
        ::ReleaseDC(hWnd, pThis->m_hDC);
        pThis->m_hDC = nullptr;
    }
}

// =============================================================================
// CPaintDC Implementation
// =============================================================================

// CPaintDC constructor
// Symbol: ??0CPaintDC@@QEAA@PEAVCWnd@@@Z
extern "C" CPaintDC* MS_ABI stub___0CPaintDC__QEAA_PEAVCWnd___Z(CPaintDC* pThis, CWnd* pWnd) {
    if (!pThis) return nullptr;

    pThis->m_hDC = nullptr;
    pThis->m_hAttribDC = nullptr;
    pThis->m_pWnd = pWnd;
    std::memset(&pThis->m_ps, 0, sizeof(pThis->m_ps));

    HWND hWnd = pWnd ? pWnd->GetSafeHwnd() : nullptr;
    if (hWnd) {
        pThis->m_hDC = ::BeginPaint(hWnd, &pThis->m_ps);
        pThis->m_hAttribDC = pThis->m_hDC;
    }

    return pThis;
}

// CPaintDC destructor
// Symbol: ??1CPaintDC@@UEAA@XZ
extern "C" void MS_ABI stub___1CPaintDC__UEAA_XZ(CPaintDC* pThis) {
    if (pThis && pThis->m_pWnd) {
        HWND hWnd = pThis->m_pWnd->GetSafeHwnd();
        if (hWnd) {
            ::EndPaint(hWnd, &pThis->m_ps);
        }
        pThis->m_hDC = nullptr;
    }
}

// =============================================================================
// CWindowDC Implementation
// =============================================================================

// CWindowDC constructor
// Symbol: ??0CWindowDC@@QEAA@PEAVCWnd@@@Z
extern "C" CWindowDC* MS_ABI stub___0CWindowDC__QEAA_PEAVCWnd___Z(CWindowDC* pThis, CWnd* pWnd) {
    if (!pThis) return nullptr;

    pThis->m_hDC = nullptr;
    pThis->m_hAttribDC = nullptr;

    HWND hWnd = pWnd ? pWnd->GetSafeHwnd() : nullptr;
    pThis->m_hDC = ::GetWindowDC(hWnd);
    pThis->m_hAttribDC = pThis->m_hDC;

    return pThis;
}

// CWindowDC destructor
// Symbol: ??1CWindowDC@@UEAA@XZ
extern "C" void MS_ABI stub___1CWindowDC__UEAA_XZ(CWindowDC* pThis) {
    if (pThis && pThis->m_hDC) {
        // Note: Need CWnd pointer to release properly, but simplified here
        ::ReleaseDC(nullptr, pThis->m_hDC);
        pThis->m_hDC = nullptr;
    }
}
