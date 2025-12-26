// GDI Core Implementation
//
// Implements CDC, CGdiObject, and related GDI classes.
// These wrap Windows GDI functions for MFC compatibility.

#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxwin.h"
#include <windows.h>
#include <cstring>
#include <unordered_map>

// Thread-local temporary GDI object map for SelectObject return values
// This allows the common pattern: pOld = dc.SelectObject(&newPen); ... dc.SelectObject(pOld);
namespace {
thread_local std::unordered_map<HGDIOBJ, CGdiObject*> g_tempGdiMap;

CGdiObject* GetTempGdiObject(HGDIOBJ hObj) {
    if (!hObj) return nullptr;

    auto it = g_tempGdiMap.find(hObj);
    if (it != g_tempGdiMap.end()) {
        return it->second;
    }

    // Create a temporary wrapper for the old object
    auto* wrapper = new CGdiObject();
    wrapper->m_hObject = hObj;
    g_tempGdiMap.emplace(hObj, wrapper);
    return wrapper;
}

void DeleteTempGdiMap() {
    for (auto& [_, obj] : g_tempGdiMap) {
        // Null out the handle so the destructor doesn't try to delete it
        // (we don't own these GDI objects)
        obj->m_hObject = nullptr;
        delete obj;
    }
    g_tempGdiMap.clear();
}
} // namespace

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
extern "C" CDC* MS_ABI impl___0CDC__QEAA_XZ(CDC* pThis) {
    if (!pThis) return nullptr;

    // Initialize members
    pThis->m_hDC = nullptr;
    pThis->m_hAttribDC = nullptr;
    return pThis;
}

// CDC destructor
// Symbol: ??1CDC@@UEAA@XZ
// Ordinal: 1086
//
// NOTE: Base CDC destructor does NOT delete the DC. Derived classes (CClientDC, CPaintDC,
// CWindowDC) handle releasing their own DCs appropriately. If a raw CDC owns a DC (e.g.,
// from CreateCompatibleDC), the caller must explicitly call DeleteDC() before destruction.
// This matches real MFC behavior where CDC is often used as a wrapper for DCs it doesn't own.
extern "C" void MS_ABI impl___1CDC__UEAA_XZ(CDC* pThis) {
    if (pThis) {
        // Don't delete - caller is responsible for DC lifetime
        pThis->m_hDC = nullptr;
        pThis->m_hAttribDC = nullptr;
    }
}

CDC::CDC() {
    impl___0CDC__QEAA_XZ(this);
}

CDC::~CDC() {
    impl___1CDC__UEAA_XZ(this);
}

// CDC::CreateCompatibleDC
// Symbol: ?CreateCompatibleDC@CDC@@QEAAHPEAV1@@Z
extern "C" int MS_ABI impl__CreateCompatibleDC_CDC__QEAAHPEAV1__Z(CDC* pThis, CDC* pDC) {
    if (!pThis) return FALSE;

    HDC hDC = pDC ? pDC->m_hDC : nullptr;
    pThis->m_hDC = ::CreateCompatibleDC(hDC);
    pThis->m_hAttribDC = pThis->m_hDC;
    return pThis->m_hDC != nullptr;
}

// CDC::DeleteDC
// Symbol: ?DeleteDC@CDC@@QEAAHXZ
extern "C" int MS_ABI impl__DeleteDC_CDC__QEAAHXZ(CDC* pThis) {
    if (!pThis || !pThis->m_hDC) return FALSE;

    int result = ::DeleteDC(pThis->m_hDC);
    pThis->m_hDC = nullptr;
    pThis->m_hAttribDC = nullptr;
    return result;
}

// CDC::SaveDC
// Symbol: ?SaveDC@CDC@@QEAAHXZ
extern "C" int MS_ABI impl__SaveDC_CDC__QEAAHXZ(CDC* pThis) {
    if (!pThis || !pThis->m_hDC) return 0;
    return ::SaveDC(pThis->m_hDC);
}

// CDC::RestoreDC
// Symbol: ?RestoreDC@CDC@@QEAAHH@Z
extern "C" int MS_ABI impl__RestoreDC_CDC__QEAAHH_Z(CDC* pThis, int nSavedDC) {
    if (!pThis || !pThis->m_hDC) return FALSE;
    return ::RestoreDC(pThis->m_hDC, nSavedDC);
}

// CDC::MoveTo (returns CPoint)
// Symbol: ?MoveTo@CDC@@QEAA?AVCPoint@@HH@Z
// Ordinal: 8501
extern "C" CPoint MS_ABI impl__MoveTo_CDC__QEAA_AVCPoint__HH_Z(CDC* pThis, int x, int y) {
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
extern "C" int MS_ABI impl__LineTo_CDC__QEAAHHH_Z(CDC* pThis, int x, int y) {
    if (!pThis || !pThis->m_hDC) return FALSE;
    return ::LineTo(pThis->m_hDC, x, y);
}

// CDC::Rectangle
// Symbol: ?Rectangle@CDC@@QEAAHHHHH@Z
extern "C" int MS_ABI impl__Rectangle_CDC__QEAAHHHHH_Z(CDC* pThis, int x1, int y1, int x2, int y2) {
    if (!pThis || !pThis->m_hDC) return FALSE;
    return ::Rectangle(pThis->m_hDC, x1, y1, x2, y2);
}

// CDC::Ellipse
// Symbol: ?Ellipse@CDC@@QEAAHHHHH@Z
extern "C" int MS_ABI impl__Ellipse_CDC__QEAAHHHHH_Z(CDC* pThis, int x1, int y1, int x2, int y2) {
    if (!pThis || !pThis->m_hDC) return FALSE;
    return ::Ellipse(pThis->m_hDC, x1, y1, x2, y2);
}

// CDC::TextOutW
// Symbol: ?TextOutW@CDC@@QEAAHHPEB_WH@Z
extern "C" int MS_ABI impl__TextOutW_CDC__QEAAHHPEB_WH_Z(CDC* pThis, int x, int y, const wchar_t* lpszString, int nCount) {
    if (!pThis || !pThis->m_hDC) return FALSE;
    if (nCount < 0 && lpszString) {
        nCount = (int)wcslen(lpszString);
    }
    return ::TextOutW(pThis->m_hDC, x, y, lpszString, nCount);
}

// CDC::SetBkColor
// Symbol: ?SetBkColor@CDC@@QEAAKK@Z
extern "C" unsigned long MS_ABI impl__SetBkColor_CDC__QEAAKK_Z(CDC* pThis, unsigned long crColor) {
    if (!pThis || !pThis->m_hDC) return CLR_INVALID;
    return ::SetBkColor(pThis->m_hDC, crColor);
}

// CDC::GetBkColor
// Symbol: ?GetBkColor@CDC@@QEBAKXZ
extern "C" unsigned long MS_ABI impl__GetBkColor_CDC__QEBAKXZ(const CDC* pThis) {
    if (!pThis || !pThis->m_hDC) return CLR_INVALID;
    return ::GetBkColor(pThis->m_hDC);
}

// CDC::SetTextColor
// Symbol: ?SetTextColor@CDC@@QEAAKK@Z
extern "C" unsigned long MS_ABI impl__SetTextColor_CDC__QEAAKK_Z(CDC* pThis, unsigned long crColor) {
    if (!pThis || !pThis->m_hDC) return CLR_INVALID;
    return ::SetTextColor(pThis->m_hDC, crColor);
}

// CDC::GetTextColor
// Symbol: ?GetTextColor@CDC@@QEBAHXZ
extern "C" int MS_ABI impl__GetTextColor_CDC__QEBAHXZ(const CDC* pThis) {
    if (!pThis || !pThis->m_hDC) return 0;
    return (int)::GetTextColor(pThis->m_hDC);
}

// CDC::SetBkMode
// Symbol: ?SetBkMode@CDC@@QEAAHH@Z
extern "C" int MS_ABI impl__SetBkMode_CDC__QEAAHH_Z(CDC* pThis, int nBkMode) {
    if (!pThis || !pThis->m_hDC) return 0;
    return ::SetBkMode(pThis->m_hDC, nBkMode);
}

// CDC::GetBkMode
// Symbol: ?GetBkMode@CDC@@QEBAHXZ
extern "C" int MS_ABI impl__GetBkMode_CDC__QEBAHXZ(const CDC* pThis) {
    if (!pThis || !pThis->m_hDC) return 0;
    return ::GetBkMode(pThis->m_hDC);
}

// CDC::BitBlt
// Symbol: ?BitBlt@CDC@@QEAAHHHHHHPEAV1@HHK@Z
extern "C" int MS_ABI impl__BitBlt_CDC__QEAAHHHHHHPEAV1_HHK_Z(
    CDC* pThis, int x, int y, int nWidth, int nHeight,
    CDC* pSrcDC, int xSrc, int ySrc, unsigned long dwRop) {
    if (!pThis || !pThis->m_hDC) return FALSE;
    HDC hSrcDC = pSrcDC ? pSrcDC->m_hDC : nullptr;
    return ::BitBlt(pThis->m_hDC, x, y, nWidth, nHeight, hSrcDC, xSrc, ySrc, dwRop);
}

// CDC::StretchBlt
// Symbol: ?StretchBlt@CDC@@QEAAHHHHHHPEAV1@HHHHK@Z
extern "C" int MS_ABI impl__StretchBlt_CDC__QEAAHHHHHHPEAV1_HHHHK_Z(
    CDC* pThis, int x, int y, int nWidth, int nHeight,
    CDC* pSrcDC, int xSrc, int ySrc, int nSrcWidth, int nSrcHeight, unsigned long dwRop) {
    if (!pThis || !pThis->m_hDC) return FALSE;
    HDC hSrcDC = pSrcDC ? pSrcDC->m_hDC : nullptr;
    return ::StretchBlt(pThis->m_hDC, x, y, nWidth, nHeight,
                        hSrcDC, xSrc, ySrc, nSrcWidth, nSrcHeight, dwRop);
}

// CDC::FillRect
// Symbol: ?FillRect@CDC@@QEAAHPEBUtagRECT@@PEAVCBrush@@@Z
extern "C" int MS_ABI impl__FillRect_CDC__QEAAHPEBUtagRECT__PEAVCBrush___Z(
    CDC* pThis, const RECT* lpRect, CBrush* pBrush) {
    if (!pThis || !pThis->m_hDC || !lpRect) return FALSE;
    HBRUSH hBrush = pBrush ? (HBRUSH)pBrush->GetSafeHandle() : nullptr;
    return ::FillRect(pThis->m_hDC, lpRect, hBrush);
}

// CDC::FrameRect
// Symbol: ?FrameRect@CDC@@QEAAHPEBUtagRECT@@PEAVCBrush@@@Z
extern "C" int MS_ABI impl__FrameRect_CDC__QEAAHPEBUtagRECT__PEAVCBrush___Z(
    CDC* pThis, const RECT* lpRect, CBrush* pBrush) {
    if (!pThis || !pThis->m_hDC || !lpRect) return FALSE;
    HBRUSH hBrush = pBrush ? (HBRUSH)pBrush->GetSafeHandle() : nullptr;
    return ::FrameRect(pThis->m_hDC, lpRect, hBrush);
}

// CDC::DrawText
// Symbol: ?DrawTextW@CDC@@QEAAHPEB_WHPEAUTAGRECT@@I@Z
extern "C" int MS_ABI impl__DrawTextW_CDC__QEAAHPEB_WHPEAUTAGRECT__I_Z(
    CDC* pThis, const wchar_t* lpszString, int nCount, RECT* lpRect, unsigned int nFormat) {
    if (!pThis || !pThis->m_hDC) return 0;
    return ::DrawTextW(pThis->m_hDC, lpszString, nCount, lpRect, nFormat);
}

// CDC::SelectObject (CGdiObject)
// Symbol: ?SelectObject@CDC@@QEAAPEAVCGdiObject@@PEAV2@@Z
extern "C" CGdiObject* MS_ABI impl__SelectObject_CDC__QEAAPEAVCGdiObject__PEAV2__Z(
    CDC* pThis, CGdiObject* pObject) {
    if (!pThis || !pThis->m_hDC || !pObject) return nullptr;
    HGDIOBJ hOld = ::SelectObject(pThis->m_hDC, pObject->GetSafeHandle());
    // Return a temporary wrapper for the old object so callers can restore it
    return GetTempGdiObject(hOld);
}

// CDC::SelectStockObject
// Symbol: ?SelectStockObject@CDC@@QEAAHH@Z
extern "C" int MS_ABI impl__SelectStockObject_CDC__QEAAHH_Z(CDC* pThis, int nIndex) {
    if (!pThis || !pThis->m_hDC) return FALSE;
    HGDIOBJ hObj = ::GetStockObject(nIndex);
    return ::SelectObject(pThis->m_hDC, hObj) != nullptr;
}

// =============================================================================
// CGdiObject Implementation
// =============================================================================

// CGdiObject::DeleteTempMap (static)
// Symbol: ?DeleteTempMap@CGdiObject@@SAXXZ
// Called during idle processing to clean up temporary GDI object wrappers
extern "C" void MS_ABI impl__DeleteTempMap_CGdiObject__SAXXZ() {
    DeleteTempGdiMap();
}

// CGdiObject::DeleteObject
// Symbol: ?DeleteObject@CGdiObject@@QEAAHXZ
extern "C" int MS_ABI impl__DeleteObject_CGdiObject__QEAAHXZ(CGdiObject* pThis) {
    if (!pThis || !pThis->m_hObject) return FALSE;
    int result = ::DeleteObject(pThis->m_hObject);
    pThis->m_hObject = nullptr;
    return result;
}

#ifdef __GNUC__
// Alias for MinGW internal calls (Itanium mangling -> impl)
asm(".globl _ZN10CGdiObject12DeleteObjectEv\n"
    ".set _ZN10CGdiObject12DeleteObjectEv, impl__DeleteObject_CGdiObject__QEAAHXZ\n");
#endif

// CGdiObject::Attach
// Symbol: ?Attach@CGdiObject@@QEAAHPEAX@Z
extern "C" int MS_ABI impl__Attach_CGdiObject__QEAAHPEAX_Z(CGdiObject* pThis, HGDIOBJ hObject) {
    if (!pThis) return FALSE;
    pThis->m_hObject = hObject;
    return TRUE;
}

// CGdiObject::Detach
// Symbol: ?Detach@CGdiObject@@QEAAPEAXXZ
extern "C" HGDIOBJ MS_ABI impl__Detach_CGdiObject__QEAAPEAXXZ(CGdiObject* pThis) {
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
extern "C" CPen* MS_ABI impl___0CPen__QEAA_XZ(CPen* pThis) {
    if (!pThis) return nullptr;
    pThis->m_hObject = nullptr;
    return pThis;
}

// CPen constructor with parameters
// Symbol: ??0CPen@@QEAA@HHK@Z
extern "C" CPen* MS_ABI impl___0CPen__QEAA_HHK_Z(CPen* pThis, int nPenStyle, int nWidth, unsigned long crColor) {
    if (!pThis) return nullptr;
    pThis->m_hObject = ::CreatePen(nPenStyle, nWidth, crColor);
    return pThis;
}

// CPen::CreatePen
// Symbol: ?CreatePen@CPen@@QEAAHHHI@Z
extern "C" int MS_ABI impl__CreatePen_CPen__QEAAHHHI_Z(CPen* pThis, int nPenStyle, int nWidth, unsigned int crColor) {
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
extern "C" CBrush* MS_ABI impl___0CBrush__QEAA_XZ(CBrush* pThis) {
    if (!pThis) return nullptr;
    pThis->m_hObject = nullptr;
    return pThis;
}

// CBrush constructor with color
// Symbol: ??0CBrush@@QEAA@K@Z
extern "C" CBrush* MS_ABI impl___0CBrush__QEAA_K_Z(CBrush* pThis, unsigned long crColor) {
    if (!pThis) return nullptr;
    pThis->m_hObject = ::CreateSolidBrush(crColor);
    return pThis;
}

// CBrush::CreateSolidBrush
// Symbol: ?CreateSolidBrush@CBrush@@QEAAHK@Z
extern "C" int MS_ABI impl__CreateSolidBrush_CBrush__QEAAHK_Z(CBrush* pThis, unsigned long crColor) {
    if (!pThis) return FALSE;
    if (pThis->m_hObject) {
        ::DeleteObject(pThis->m_hObject);
    }
    pThis->m_hObject = ::CreateSolidBrush(crColor);
    return pThis->m_hObject != nullptr;
}

// CBrush::CreateHatchBrush
// Symbol: ?CreateHatchBrush@CBrush@@QEAAHHK@Z
extern "C" int MS_ABI impl__CreateHatchBrush_CBrush__QEAAHHK_Z(CBrush* pThis, int nIndex, unsigned long crColor) {
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
extern "C" CFont* MS_ABI impl___0CFont__QEAA_XZ(CFont* pThis) {
    if (!pThis) return nullptr;
    pThis->m_hObject = nullptr;
    return pThis;
}

// CFont::CreateFontIndirectW
// Symbol: ?CreateFontIndirectW@CFont@@QEAAHPEBUtagLOGFONTW@@@Z
extern "C" int MS_ABI impl__CreateFontIndirectW_CFont__QEAAHPEBUtagLOGFONTW___Z(CFont* pThis, const LOGFONTW* lpLogFont) {
    if (!pThis || !lpLogFont) return FALSE;
    if (pThis->m_hObject) {
        ::DeleteObject(pThis->m_hObject);
    }
    pThis->m_hObject = ::CreateFontIndirectW(lpLogFont);
    return pThis->m_hObject != nullptr;
}

// CFont::CreatePointFont
// Symbol: ?CreatePointFont@CFont@@QEAAHHPEB_WPEAV1@@Z
extern "C" int MS_ABI impl__CreatePointFont_CFont__QEAAHHPEB_WPEAV1__Z(
    CFont* pThis, int nPointSize, const wchar_t* lpszFaceName, CDC* pDC) {
    if (!pThis) return FALSE;

    LOGFONTW lf = {};
    lf.lfHeight = -MulDiv(nPointSize,
        pDC ? ::GetDeviceCaps(pDC->m_hDC, LOGPIXELSY) : 96,
        720);
    lf.lfWeight = FW_NORMAL;
    if (lpszFaceName) {
        wcsncpy(lf.lfFaceName, lpszFaceName, LF_FACESIZE - 1);
        // Ensure null termination - wcsncpy doesn't guarantee it
        lf.lfFaceName[LF_FACESIZE - 1] = L'\0';
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
extern "C" CBitmap* MS_ABI impl___0CBitmap__QEAA_XZ(CBitmap* pThis) {
    if (!pThis) return nullptr;
    pThis->m_hObject = nullptr;
    return pThis;
}

// CBitmap::CreateCompatibleBitmap
// Symbol: ?CreateCompatibleBitmap@CBitmap@@QEAAHPEAVCDC@@HH@Z
extern "C" int MS_ABI impl__CreateCompatibleBitmap_CBitmap__QEAAHPEAVCDC__HH_Z(
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
extern "C" int MS_ABI impl__LoadBitmapW_CBitmap__QEAAHPEB_W_Z(CBitmap* pThis, const wchar_t* lpszResourceName) {
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
extern "C" int MS_ABI impl__LoadBitmapW_CBitmap__QEAAHI_Z(CBitmap* pThis, UINT nIDResource) {
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
extern "C" CClientDC* MS_ABI impl___0CClientDC__QEAA_PEAVCWnd___Z(CClientDC* pThis, CWnd* pWnd) {
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
extern "C" void MS_ABI impl___1CClientDC__UEAA_XZ(CClientDC* pThis) {
    if (pThis && pThis->m_hDC) {
        HWND hWnd = pThis->m_pWnd ? pThis->m_pWnd->GetSafeHwnd() : nullptr;
        ::ReleaseDC(hWnd, pThis->m_hDC);
        pThis->m_hDC = nullptr;
        pThis->m_hAttribDC = nullptr;
    }
}

// =============================================================================
// CPaintDC Implementation
// =============================================================================

// CPaintDC constructor
// Symbol: ??0CPaintDC@@QEAA@PEAVCWnd@@@Z
extern "C" CPaintDC* MS_ABI impl___0CPaintDC__QEAA_PEAVCWnd___Z(CPaintDC* pThis, CWnd* pWnd) {
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
extern "C" void MS_ABI impl___1CPaintDC__UEAA_XZ(CPaintDC* pThis) {
    if (pThis && pThis->m_pWnd) {
        HWND hWnd = pThis->m_pWnd->GetSafeHwnd();
        if (hWnd) {
            ::EndPaint(hWnd, &pThis->m_ps);
        }
        pThis->m_hDC = nullptr;
        pThis->m_hAttribDC = nullptr;  // Also reset attrib DC
    }
}

// =============================================================================
// CWindowDC Implementation
// =============================================================================

// CWindowDC constructor
// Symbol: ??0CWindowDC@@QEAA@PEAVCWnd@@@Z
extern "C" CWindowDC* MS_ABI impl___0CWindowDC__QEAA_PEAVCWnd___Z(CWindowDC* pThis, CWnd* pWnd) {
    if (!pThis) return nullptr;

    pThis->m_hDC = nullptr;
    pThis->m_hAttribDC = nullptr;
    pThis->m_pWnd = pWnd;

    HWND hWnd = pWnd ? pWnd->GetSafeHwnd() : nullptr;
    pThis->m_hDC = ::GetWindowDC(hWnd);
    pThis->m_hAttribDC = pThis->m_hDC;

    return pThis;
}

// CWindowDC destructor
// Symbol: ??1CWindowDC@@UEAA@XZ
extern "C" void MS_ABI impl___1CWindowDC__UEAA_XZ(CWindowDC* pThis) {
    if (pThis && pThis->m_hDC) {
        HWND hWnd = pThis->m_pWnd ? pThis->m_pWnd->GetSafeHwnd() : nullptr;
        ::ReleaseDC(hWnd, pThis->m_hDC);
        pThis->m_hDC = nullptr;
        pThis->m_hAttribDC = nullptr;
    }
    if (pThis) {
        pThis->m_pWnd = nullptr;
    }
}

// =============================================================================
// CMetaFileDC Implementation
// =============================================================================

IMPLEMENT_DYNAMIC(CMetaFileDC, CDC)

#ifdef __GNUC__
asm(".globl \"?classCMetaFileDC@CMetaFileDC@@2UCRuntimeClass@@A\"\n"
    ".set \"?classCMetaFileDC@CMetaFileDC@@2UCRuntimeClass@@A\", _ZN11CMetaFileDC16classCMetaFileDCE\n");
#endif

// CMetaFileDC default constructor
CMetaFileDC::CMetaFileDC() : CDC() {
    memset(_metafiledc_padding, 0, sizeof(_metafiledc_padding));
}

// CMetaFileDC destructor
CMetaFileDC::~CMetaFileDC() {
    // If DC is still open, close it
    if (m_hDC) {
        // Check if it's an enhanced metafile DC
        HENHMETAFILE hMF = CloseEnhMetaFile(m_hDC);
        if (hMF) {
            DeleteEnhMetaFile(hMF);
        }
        m_hDC = nullptr;
        m_hAttribDC = nullptr;
    }
}

// CMetaFileDC::Create - Create a Windows metafile DC
int CMetaFileDC::Create(const wchar_t* lpszFilename) {
    // Windows metafiles (WMF) are legacy format
    // CreateMetaFileW returns a DC for recording metafile commands
    m_hDC = ::CreateMetaFileW(lpszFilename);
    m_hAttribDC = m_hDC;
    return m_hDC != nullptr;
}

// CMetaFileDC::CreateEnhanced - Create an enhanced metafile DC
int CMetaFileDC::CreateEnhanced(void* pDC, const wchar_t* lpszFilename,
                                 const RECT* lpBounds, const wchar_t* lpszDescription)
{
    HDC hRefDC = pDC ? static_cast<CDC*>(pDC)->m_hDC : nullptr;
    m_hDC = ::CreateEnhMetaFileW(hRefDC, lpszFilename, lpBounds, lpszDescription);
    m_hAttribDC = m_hDC;
    return m_hDC != nullptr;
}

// CMetaFileDC::Close - Close metafile DC and return HMETAFILE
void* CMetaFileDC::Close() {
    if (!m_hDC) return nullptr;

    // CloseMetaFile returns an HMETAFILE
    HMETAFILE hMF = ::CloseMetaFile(m_hDC);
    m_hDC = nullptr;
    m_hAttribDC = nullptr;
    return hMF;
}

// CMetaFileDC::CloseEnhanced - Close enhanced metafile DC and return HENHMETAFILE
void* CMetaFileDC::CloseEnhanced() {
    if (!m_hDC) return nullptr;

    // CloseEnhMetaFile returns an HENHMETAFILE
    HENHMETAFILE hMF = ::CloseEnhMetaFile(m_hDC);
    m_hDC = nullptr;
    m_hAttribDC = nullptr;
    return hMF;
}

// =============================================================================
// CPalette Implementation
// =============================================================================

// CPalette default constructor
CPalette::CPalette() : CGdiObject() {
}

// CPalette destructor
CPalette::~CPalette() {
    if (m_hObject) {
        ::DeleteObject(m_hObject);
        m_hObject = nullptr;
    }
}

// CPalette::CreatePalette
int CPalette::CreatePalette(const LOGPALETTE* lpLogPalette) {
    if (!lpLogPalette) return FALSE;
    if (m_hObject) {
        ::DeleteObject(m_hObject);
    }
    m_hObject = ::CreatePalette(lpLogPalette);
    return m_hObject != nullptr;
}

// CPalette::CreateHalftonePalette
int CPalette::CreateHalftonePalette(CDC* pDC) {
    if (!pDC || !pDC->m_hDC) return FALSE;
    if (m_hObject) {
        ::DeleteObject(m_hObject);
    }
    m_hObject = ::CreateHalftonePalette(pDC->m_hDC);
    return m_hObject != nullptr;
}

// CPalette::GetEntryCount
int CPalette::GetEntryCount() const {
    if (!m_hObject) return 0;
    WORD nEntries = 0;
    ::GetObject(m_hObject, sizeof(WORD), &nEntries);
    return nEntries;
}

// CPalette::GetPaletteEntries
unsigned int CPalette::GetPaletteEntries(unsigned int nStartIndex, unsigned int nNumEntries,
                                          PALETTEENTRY* lpPaletteColors) const {
    if (!m_hObject) return 0;
    return ::GetPaletteEntries((HPALETTE)m_hObject, nStartIndex, nNumEntries, lpPaletteColors);
}

// CPalette::SetPaletteEntries
unsigned int CPalette::SetPaletteEntries(unsigned int nStartIndex, unsigned int nNumEntries,
                                          PALETTEENTRY* lpPaletteColors) {
    if (!m_hObject) return 0;
    return ::SetPaletteEntries((HPALETTE)m_hObject, nStartIndex, nNumEntries, lpPaletteColors);
}

// CPalette::AnimatePalette
void CPalette::AnimatePalette(unsigned int nStartIndex, unsigned int nNumEntries,
                              PALETTEENTRY* lpPaletteColors) {
    if (m_hObject) {
        ::AnimatePalette((HPALETTE)m_hObject, nStartIndex, nNumEntries, lpPaletteColors);
    }
}

// CPalette::ResizePalette
int CPalette::ResizePalette(unsigned int nNumEntries) {
    if (!m_hObject) return FALSE;
    return ::ResizePalette((HPALETTE)m_hObject, nNumEntries);
}

// CPalette::GetNearestPaletteIndex
unsigned int CPalette::GetNearestPaletteIndex(unsigned long crColor) const {
    if (!m_hObject) return CLR_INVALID;
    return ::GetNearestPaletteIndex((HPALETTE)m_hObject, crColor);
}

// =============================================================================
// CRgn Implementation
// =============================================================================

// CRgn default constructor
CRgn::CRgn() : CGdiObject() {
}

// CRgn destructor
CRgn::~CRgn() {
    if (m_hObject) {
        ::DeleteObject(m_hObject);
        m_hObject = nullptr;
    }
}

// CRgn::CreateRectRgn
int CRgn::CreateRectRgn(int x1, int y1, int x2, int y2) {
    if (m_hObject) {
        ::DeleteObject(m_hObject);
    }
    m_hObject = ::CreateRectRgn(x1, y1, x2, y2);
    return m_hObject != nullptr;
}

// CRgn::CreateRectRgnIndirect
int CRgn::CreateRectRgnIndirect(const RECT* lpRect) {
    if (!lpRect) return FALSE;
    return CreateRectRgn(lpRect->left, lpRect->top, lpRect->right, lpRect->bottom);
}

// CRgn::CreateEllipticRgn
int CRgn::CreateEllipticRgn(int x1, int y1, int x2, int y2) {
    if (m_hObject) {
        ::DeleteObject(m_hObject);
    }
    m_hObject = ::CreateEllipticRgn(x1, y1, x2, y2);
    return m_hObject != nullptr;
}

// CRgn::CreateEllipticRgnIndirect
int CRgn::CreateEllipticRgnIndirect(const RECT* lpRect) {
    if (!lpRect) return FALSE;
    return CreateEllipticRgn(lpRect->left, lpRect->top, lpRect->right, lpRect->bottom);
}

// CRgn::CreatePolygonRgn
int CRgn::CreatePolygonRgn(const POINT* lpPoints, int nCount, int nMode) {
    if (m_hObject) {
        ::DeleteObject(m_hObject);
    }
    m_hObject = ::CreatePolygonRgn(lpPoints, nCount, nMode);
    return m_hObject != nullptr;
}

// CRgn::CreateRoundRectRgn
int CRgn::CreateRoundRectRgn(int x1, int y1, int x2, int y2, int x3, int y3) {
    if (m_hObject) {
        ::DeleteObject(m_hObject);
    }
    m_hObject = ::CreateRoundRectRgn(x1, y1, x2, y2, x3, y3);
    return m_hObject != nullptr;
}

// CRgn::CreateFromPath
int CRgn::CreateFromPath(CDC* pDC) {
    if (!pDC || !pDC->m_hDC) return FALSE;
    if (m_hObject) {
        ::DeleteObject(m_hObject);
    }
    m_hObject = ::PathToRegion(pDC->m_hDC);
    return m_hObject != nullptr;
}

// CRgn::SetRectRgn
void CRgn::SetRectRgn(int x1, int y1, int x2, int y2) {
    if (m_hObject) {
        ::SetRectRgn((HRGN)m_hObject, x1, y1, x2, y2);
    }
}

// CRgn::SetRectRgn with RECT
void CRgn::SetRectRgn(const RECT* lpRect) {
    if (lpRect && m_hObject) {
        ::SetRectRgn((HRGN)m_hObject, lpRect->left, lpRect->top, lpRect->right, lpRect->bottom);
    }
}

// CRgn::CombineRgn
int CRgn::CombineRgn(CRgn* pRgn1, CRgn* pRgn2, int nCombineMode) {
    if (!m_hObject || !pRgn1 || !pRgn1->m_hObject) return ERROR;
    HRGN hRgn2 = pRgn2 ? (HRGN)pRgn2->m_hObject : nullptr;
    return ::CombineRgn((HRGN)m_hObject, (HRGN)pRgn1->m_hObject, hRgn2, nCombineMode);
}

// CRgn::CopyRgn
int CRgn::CopyRgn(CRgn* pRgnSrc) {
    if (!m_hObject || !pRgnSrc || !pRgnSrc->m_hObject) return ERROR;
    return ::CombineRgn((HRGN)m_hObject, (HRGN)pRgnSrc->m_hObject, nullptr, RGN_COPY);
}

// CRgn::EqualRgn
int CRgn::EqualRgn(CRgn* pRgn) const {
    if (!m_hObject || !pRgn || !pRgn->m_hObject) return FALSE;
    return ::EqualRgn((HRGN)m_hObject, (HRGN)pRgn->m_hObject);
}

// CRgn::OffsetRgn
int CRgn::OffsetRgn(int x, int y) {
    if (!m_hObject) return ERROR;
    return ::OffsetRgn((HRGN)m_hObject, x, y);
}

// CRgn::OffsetRgn with POINT
int CRgn::OffsetRgn(POINT point) {
    return OffsetRgn(point.x, point.y);
}

// CRgn::GetRgnBox
int CRgn::GetRgnBox(RECT* lpRect) const {
    if (!m_hObject || !lpRect) return ERROR;
    return ::GetRgnBox((HRGN)m_hObject, lpRect);
}

// CRgn::PtInRegion
int CRgn::PtInRegion(int x, int y) const {
    if (!m_hObject) return FALSE;
    return ::PtInRegion((HRGN)m_hObject, x, y);
}

// CRgn::PtInRegion with POINT
int CRgn::PtInRegion(POINT point) const {
    return PtInRegion(point.x, point.y);
}

// CRgn::RectInRegion
int CRgn::RectInRegion(const RECT* lpRect) const {
    if (!m_hObject || !lpRect) return FALSE;
    return ::RectInRegion((HRGN)m_hObject, lpRect);
}

// CRgn::GetRegionData
int CRgn::GetRegionData(RGNDATA* lpRgnData, int nDataSize) const {
    if (!m_hObject) return 0;
    return ::GetRegionData((HRGN)m_hObject, nDataSize, lpRgnData);
}
