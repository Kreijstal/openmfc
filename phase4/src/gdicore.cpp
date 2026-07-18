// GDI Core Implementation
//
// Implements CDC, CGdiObject, and related GDI classes.
// These wrap Windows GDI functions for MFC compatibility.

#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxwin.h"
#include "openmfc/afxmfc.h"
#include <windows.h>
#include <cmath>
#include <cstring>
#include <mutex>
#include <unordered_map>
#include <vector>

// Thread-local temporary GDI object map for SelectObject return values
// This allows the common pattern: pOld = dc.SelectObject(&newPen); ... dc.SelectObject(pOld);
namespace {
thread_local std::unordered_map<HGDIOBJ, CGdiObject*> g_tempGdiMap;
thread_local std::unordered_map<HDC, CDC*> g_tempDCMap;

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

CDC* GetTempDC(HDC hDC) {
    if (!hDC) return nullptr;

    auto it = g_tempDCMap.find(hDC);
    if (it != g_tempDCMap.end()) {
        return it->second;
    }

    auto* wrapper = new CDC();
    wrapper->m_hDC = hDC;
    wrapper->m_hAttribDC = hDC;
    g_tempDCMap.emplace(hDC, wrapper);
    return wrapper;
}

void DeleteTempDCMap() {
    for (auto& [_, dc] : g_tempDCMap) {
        dc->m_hDC = nullptr;
        dc->m_hAttribDC = nullptr;
        delete dc;
    }
    g_tempDCMap.clear();
}
} // namespace

namespace {
struct D2D_POINT_2F_L {
    float x;
    float y;
};

struct D2D_SIZE_F_L {
    float width;
    float height;
};

struct D2D_RECT_F_L {
    float left;
    float top;
    float right;
    float bottom;
};

struct D2D1_ELLIPSE_L {
    D2D_POINT_2F_L point;
    float radiusX;
    float radiusY;
};

struct D2D1_ROUNDED_RECT_L {
    D2D_RECT_F_L rect;
    float radiusX;
    float radiusY;
};

struct CRenderTargetState {
    void* resource = nullptr;
    bool drawing = false;
    CD2DSizeF dpi = CD2DSizeF(96.0f, 96.0f);
    CD2DSizeF size = CD2DSizeF(0.0f, 0.0f);
    CString lastText;
    CD2DRectF lastTextRect;
    unsigned __int64 tag1 = 0;
    unsigned __int64 tag2 = 0;
};

struct CAnimationVariableState {
    double value = 0.0;
};

std::unordered_map<const CRenderTarget*, CRenderTargetState> g_renderTargetState;
std::unordered_map<const CAnimationVariable*, CAnimationVariableState> g_animationVariableState;
std::mutex g_wave2StateMutex;
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
extern "C" int MS_ABI impl__Rectangle_CDC__QEAAHHHHH_Z(CDC* pThis, int x1, int y1, int x2, int y2) {
    if (!pThis || !pThis->m_hDC) return FALSE;
    return ::Rectangle(pThis->m_hDC, x1, y1, x2, y2);
}

// CDC::Ellipse
extern "C" int MS_ABI impl__Ellipse_CDC__QEAAHHHHH_Z(CDC* pThis, int x1, int y1, int x2, int y2) {
    if (!pThis || !pThis->m_hDC) return FALSE;
    return ::Ellipse(pThis->m_hDC, x1, y1, x2, y2);
}

// CDC::TextOutW
extern "C" int MS_ABI impl__TextOutW_CDC__QEAAHHPEB_WH_Z(CDC* pThis, int x, int y, const wchar_t* lpszString, int nCount) {
    if (!pThis || !pThis->m_hDC) return FALSE;
    if (nCount < 0 && lpszString) {
        nCount = (int)wcslen(lpszString);
    }
    return ::TextOutW(pThis->m_hDC, x, y, lpszString, nCount);
}

// CDC::SetBkColor
extern "C" unsigned long MS_ABI impl__SetBkColor_CDC__QEAAKK_Z(CDC* pThis, unsigned long crColor) {
    if (!pThis || !pThis->m_hDC) return CLR_INVALID;
    return ::SetBkColor(pThis->m_hDC, crColor);
}

// CDC::GetBkColor
extern "C" unsigned long MS_ABI impl__GetBkColor_CDC__QEBAKXZ(const CDC* pThis) {
    if (!pThis || !pThis->m_hDC) return CLR_INVALID;
    return ::GetBkColor(pThis->m_hDC);
}

// CDC::SetTextColor
extern "C" unsigned long MS_ABI impl__SetTextColor_CDC__QEAAKK_Z(CDC* pThis, unsigned long crColor) {
    if (!pThis || !pThis->m_hDC) return CLR_INVALID;
    return ::SetTextColor(pThis->m_hDC, crColor);
}

// CDC::GetTextColor
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
extern "C" int MS_ABI impl__GetBkMode_CDC__QEBAHXZ(const CDC* pThis) {
    if (!pThis || !pThis->m_hDC) return 0;
    return ::GetBkMode(pThis->m_hDC);
}

// CDC::BitBlt
extern "C" int MS_ABI impl__BitBlt_CDC__QEAAHHHHHHPEAV1_HHK_Z(
    CDC* pThis, int x, int y, int nWidth, int nHeight,
    CDC* pSrcDC, int xSrc, int ySrc, unsigned long dwRop) {
    if (!pThis || !pThis->m_hDC) return FALSE;
    HDC hSrcDC = pSrcDC ? pSrcDC->m_hDC : nullptr;
    return ::BitBlt(pThis->m_hDC, x, y, nWidth, nHeight, hSrcDC, xSrc, ySrc, dwRop);
}

// CDC::StretchBlt
extern "C" int MS_ABI impl__StretchBlt_CDC__QEAAHHHHHHPEAV1_HHHHK_Z(
    CDC* pThis, int x, int y, int nWidth, int nHeight,
    CDC* pSrcDC, int xSrc, int ySrc, int nSrcWidth, int nSrcHeight, unsigned long dwRop) {
    if (!pThis || !pThis->m_hDC) return FALSE;
    HDC hSrcDC = pSrcDC ? pSrcDC->m_hDC : nullptr;
    return ::StretchBlt(pThis->m_hDC, x, y, nWidth, nHeight,
                        hSrcDC, xSrc, ySrc, nSrcWidth, nSrcHeight, dwRop);
}

// CDC::FillRect
extern "C" int MS_ABI impl__FillRect_CDC__QEAAHPEBUtagRECT__PEAVCBrush___Z(
    CDC* pThis, const RECT* lpRect, CBrush* pBrush) {
    if (!pThis || !pThis->m_hDC || !lpRect) return FALSE;
    HBRUSH hBrush = pBrush ? (HBRUSH)pBrush->GetSafeHandle() : nullptr;
    return ::FillRect(pThis->m_hDC, lpRect, hBrush);
}

// CDC::PatBlt
// Symbol: ?PatBlt@CDC@@QEAAHHHHHK@Z
extern "C" int MS_ABI impl__PatBlt_CDC__QEAAHHHHHK_Z(
    CDC* pThis, int x, int y, int nWidth, int nHeight, unsigned long dwRop) {
    if (!pThis || !pThis->m_hDC) return FALSE;
    return ::PatBlt(pThis->m_hDC, x, y, nWidth, nHeight, dwRop);
}

// CDC::FrameRect
extern "C" int MS_ABI impl__FrameRect_CDC__QEAAHPEBUtagRECT__PEAVCBrush___Z(
    CDC* pThis, const RECT* lpRect, CBrush* pBrush) {
    if (!pThis || !pThis->m_hDC || !lpRect) return FALSE;
    HBRUSH hBrush = pBrush ? (HBRUSH)pBrush->GetSafeHandle() : nullptr;
    return ::FrameRect(pThis->m_hDC, lpRect, hBrush);
}

// CDC::DrawText
extern "C" int MS_ABI impl__DrawTextW_CDC__QEAAHPEB_WHPEAUTAGRECT__I_Z(
    CDC* pThis, const wchar_t* lpszString, int nCount, RECT* lpRect, unsigned int nFormat) {
    if (!pThis || !pThis->m_hDC) return 0;
    return ::DrawTextW(pThis->m_hDC, lpszString, nCount, lpRect, nFormat);
}

// CDC::SelectObject (CGdiObject)
extern "C" CGdiObject* MS_ABI impl__SelectObject_CDC__QEAAPEAVCGdiObject__PEAV2__Z(
    CDC* pThis, CGdiObject* pObject) {
    if (!pThis || !pThis->m_hDC || !pObject) return nullptr;
    HGDIOBJ hOld = ::SelectObject(pThis->m_hDC, pObject->GetSafeHandle());
    // Return a temporary wrapper for the old object so callers can restore it
    return GetTempGdiObject(hOld);
}

// CDC::SelectStockObject
extern "C" int MS_ABI impl__SelectStockObject_CDC__QEAAHH_Z(CDC* pThis, int nIndex) {
    if (!pThis || !pThis->m_hDC) return FALSE;
    HGDIOBJ hObj = ::GetStockObject(nIndex);
    return ::SelectObject(pThis->m_hDC, hObj) != nullptr;
}

// CDC::SelectClipRgn
// Symbol: ?SelectClipRgn@CDC@@QEAAHPEAVCRgn@@@Z
extern "C" int MS_ABI impl__SelectClipRgn_CDC__QEAAHPEAVCRgn___Z(CDC* pThis, CRgn* pRgn) {
    if (!pThis || !pThis->m_hDC) return ERROR;
    HRGN hRgn = pRgn ? (HRGN)pRgn->GetSafeHandle() : nullptr;
    return ::SelectClipRgn(pThis->m_hDC, hRgn);
}

// CDC::SelectClipRgn (mode)
// Symbol: ?SelectClipRgn@CDC@@QEAAHPEAVCRgn@@H@Z
extern "C" int MS_ABI impl__SelectClipRgn_CDC__QEAAHPEAVCRgn__H_Z(CDC* pThis, CRgn* pRgn, int nMode) {
    if (!pThis || !pThis->m_hDC) return ERROR;
    HRGN hRgn = pRgn ? (HRGN)pRgn->GetSafeHandle() : nullptr;
    return ::ExtSelectClipRgn(pThis->m_hDC, hRgn, nMode);
}

// CDC::ExcludeClipRect
// Symbol: ?ExcludeClipRect@CDC@@QEAAHHHHH@Z
extern "C" int MS_ABI impl__ExcludeClipRect_CDC__QEAAHHHHH_Z(CDC* pThis, int x1, int y1, int x2, int y2) {
    if (!pThis || !pThis->m_hDC) return ERROR;
    return ::ExcludeClipRect(pThis->m_hDC, x1, y1, x2, y2);
}

// CDC::ExcludeClipRect (RECT)
// Symbol: ?ExcludeClipRect@CDC@@QEAAHPEBUtagRECT@@@Z
extern "C" int MS_ABI impl__ExcludeClipRect_CDC__QEAAHPEBUtagRECT___Z(CDC* pThis, const RECT* lpRect) {
    if (!pThis || !pThis->m_hDC || !lpRect) return ERROR;
    return ::ExcludeClipRect(pThis->m_hDC, lpRect->left, lpRect->top, lpRect->right, lpRect->bottom);
}

// CDC::IntersectClipRect
// Symbol: ?IntersectClipRect@CDC@@QEAAHHHHH@Z
extern "C" int MS_ABI impl__IntersectClipRect_CDC__QEAAHHHHH_Z(CDC* pThis, int x1, int y1, int x2, int y2) {
    if (!pThis || !pThis->m_hDC) return ERROR;
    return ::IntersectClipRect(pThis->m_hDC, x1, y1, x2, y2);
}

// CDC::IntersectClipRect (RECT)
// Symbol: ?IntersectClipRect@CDC@@QEAAHPEBUtagRECT@@@Z
extern "C" int MS_ABI impl__IntersectClipRect_CDC__QEAAHPEBUtagRECT___Z(CDC* pThis, const RECT* lpRect) {
    if (!pThis || !pThis->m_hDC || !lpRect) return ERROR;
    return ::IntersectClipRect(pThis->m_hDC, lpRect->left, lpRect->top, lpRect->right, lpRect->bottom);
}

// CDC::OffsetClipRgn
// Symbol: ?OffsetClipRgn@CDC@@QEAAHHH@Z
extern "C" int MS_ABI impl__OffsetClipRgn_CDC__QEAAHHH_Z(CDC* pThis, int x, int y) {
    if (!pThis || !pThis->m_hDC) return ERROR;
    return ::OffsetClipRgn(pThis->m_hDC, x, y);
}

// CDC::OffsetClipRgn (SIZE)
// Symbol: ?OffsetClipRgn@CDC@@QEAAHUtagSIZE@@@Z
extern "C" int MS_ABI impl__OffsetClipRgn_CDC__QEAAHUtagSIZE___Z(CDC* pThis, SIZE size) {
    return impl__OffsetClipRgn_CDC__QEAAHHH_Z(pThis, size.cx, size.cy);
}

// CDC::FillSolidRect
// Symbol: ?FillSolidRect@CDC@@QEAAXHHHHK@Z
extern "C" void MS_ABI impl__FillSolidRect_CDC__QEAAXHHHHK_Z(
    CDC* pThis, int x, int y, int cx, int cy, unsigned long clr) {
    if (!pThis || !pThis->m_hDC) return;
    RECT rc = {x, y, x + cx, y + cy};
    ::SetBkColor(pThis->m_hDC, clr);
    ::ExtTextOutW(pThis->m_hDC, 0, 0, ETO_OPAQUE, &rc, nullptr, 0, nullptr);
}

// CDC::FillSolidRect (RECT)
// Symbol: ?FillSolidRect@CDC@@QEAAXPEBUtagRECT@@K@Z
extern "C" void MS_ABI impl__FillSolidRect_CDC__QEAAXPEBUtagRECT__K_Z(
    CDC* pThis, const RECT* lpRect, unsigned long clr) {
    if (!lpRect) return;
    impl__FillSolidRect_CDC__QEAAXHHHHK_Z(
        pThis,
        lpRect->left,
        lpRect->top,
        lpRect->right - lpRect->left,
        lpRect->bottom - lpRect->top,
        clr);
}

// CDC::Attach
// Symbol: ?Attach@CDC@@QEAAHPEAUHDC__@@@Z
extern "C" int MS_ABI impl__Attach_CDC__QEAAHPEAUHDC_____Z(CDC* pThis, HDC hDC) {
    if (!pThis) return FALSE;
    pThis->m_hDC = hDC;
    pThis->m_hAttribDC = hDC;
    return TRUE;
}

// CDC::Detach
// Symbol: ?Detach@CDC@@QEAAPEAUHDC__@@XZ
extern "C" HDC MS_ABI impl__Detach_CDC__QEAAPEAUHDC____XZ(CDC* pThis) {
    if (!pThis) return nullptr;
    HDC hDC = pThis->m_hDC;
    pThis->m_hDC = nullptr;
    pThis->m_hAttribDC = nullptr;
    return hDC;
}

// CDC::FromHandle
// Symbol: ?FromHandle@CDC@@SAPEAV1@PEAUHDC__@@@Z
extern "C" CDC* MS_ABI impl__FromHandle_CDC__SAPEAV1_PEAUHDC_____Z(HDC hDC) {
    return GetTempDC(hDC);
}

// CDC::DeleteTempMap
// Symbol: ?DeleteTempMap@CDC@@SAXXZ
extern "C" void MS_ABI impl__DeleteTempMap_CDC__SAXXZ() {
    DeleteTempDCMap();
}

// CDC::DPtoLP (SIZE)
// Symbol: ?DPtoLP@CDC@@QEBAXPEAUtagSIZE@@@Z
extern "C" void MS_ABI impl__DPtoLP_CDC__QEBAXPEAUtagSIZE___Z(const CDC* pThis, SIZE* lpSize) {
    if (!pThis || !pThis->m_hDC || !lpSize) return;
    POINT pt = {lpSize->cx, lpSize->cy};
    ::DPtoLP(pThis->m_hDC, &pt, 1);
    lpSize->cx = pt.x;
    lpSize->cy = pt.y;
}

// CDC::LPtoDP (SIZE)
// Symbol: ?LPtoDP@CDC@@QEBAXPEAUtagSIZE@@@Z
extern "C" void MS_ABI impl__LPtoDP_CDC__QEBAXPEAUtagSIZE___Z(const CDC* pThis, SIZE* lpSize) {
    if (!pThis || !pThis->m_hDC || !lpSize) return;
    POINT pt = {lpSize->cx, lpSize->cy};
    ::LPtoDP(pThis->m_hDC, &pt, 1);
    lpSize->cx = pt.x;
    lpSize->cy = pt.y;
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

// CGdiObject::FromHandle
// Symbol: ?FromHandle@CGdiObject@@SAPEAV1@PEAX@Z
extern "C" CGdiObject* MS_ABI impl__FromHandle_CGdiObject__SAPEAV1_PEAX_Z(HGDIOBJ hObject) {
    return GetTempGdiObject(hObject);
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
extern "C" int MS_ABI impl__CreateSolidBrush_CBrush__QEAAHK_Z(CBrush* pThis, unsigned long crColor) {
    if (!pThis) return FALSE;
    if (pThis->m_hObject) {
        ::DeleteObject(pThis->m_hObject);
    }
    pThis->m_hObject = ::CreateSolidBrush(crColor);
    return pThis->m_hObject != nullptr;
}

// CBrush::CreateHatchBrush
extern "C" int MS_ABI impl__CreateHatchBrush_CBrush__QEAAHHK_Z(CBrush* pThis, int nIndex, unsigned long crColor) {
    if (!pThis) return FALSE;
    if (pThis->m_hObject) {
        ::DeleteObject(pThis->m_hObject);
    }
    pThis->m_hObject = ::CreateHatchBrush(nIndex, crColor);
    return pThis->m_hObject != nullptr;
}

// CBrush::CreateDIBPatternBrush
// Symbol: ?CreateDIBPatternBrush@CBrush@@QEAAHPEAXI@Z
extern "C" int MS_ABI impl__CreateDIBPatternBrush_CBrush__QEAAHPEAXI_Z(
    CBrush* pThis, void* lpPackedDIB, unsigned int nUsage) {
    if (!pThis || !lpPackedDIB) return FALSE;
    if (pThis->m_hObject) {
        ::DeleteObject(pThis->m_hObject);
    }
    pThis->m_hObject = ::CreateDIBPatternBrushPt(lpPackedDIB, nUsage);
    return pThis->m_hObject != nullptr;
}

// =============================================================================
// CFont Implementation
// =============================================================================

// CFont default constructor
extern "C" CFont* MS_ABI impl___0CFont__QEAA_XZ(CFont* pThis) {
    if (!pThis) return nullptr;
    pThis->m_hObject = nullptr;
    return pThis;
}

// CFont::CreateFontIndirectW
extern "C" int MS_ABI impl__CreateFontIndirectW_CFont__QEAAHPEBUtagLOGFONTW___Z(CFont* pThis, const LOGFONTW* lpLogFont) {
    if (!pThis || !lpLogFont) return FALSE;
    if (pThis->m_hObject) {
        ::DeleteObject(pThis->m_hObject);
    }
    pThis->m_hObject = ::CreateFontIndirectW(lpLogFont);
    return pThis->m_hObject != nullptr;
}

// CFont::CreatePointFont
// Symbol: ?CreatePointFont@CFont@@QEAAHHPEB_WPEAVCDC@@@Z
extern "C" int MS_ABI impl__CreatePointFont_CFont__QEAAHHPEB_WPEAVCDC___Z(
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

// CFont::CreatePointFontIndirect
// Symbol: ?CreatePointFontIndirect@CFont@@QEAAHPEBUtagLOGFONTW@@PEAVCDC@@@Z
extern "C" int MS_ABI impl__CreatePointFontIndirect_CFont__QEAAHPEBUtagLOGFONTW__PEAVCDC___Z(
    CFont* pThis, const LOGFONTW* lpLogFont, CDC* pDC) {
    if (!pThis || !lpLogFont) return FALSE;

    LOGFONTW lf = *lpLogFont;
    int dpiY = (pDC && pDC->m_hDC) ? ::GetDeviceCaps(pDC->m_hDC, LOGPIXELSY) : 96;
    lf.lfHeight = -MulDiv(lf.lfHeight, dpiY, 720);

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
extern "C" CBitmap* MS_ABI impl___0CBitmap__QEAA_XZ(CBitmap* pThis) {
    if (!pThis) return nullptr;
    pThis->m_hObject = nullptr;
    return pThis;
}

// CBitmap::CreateCompatibleBitmap
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

//=============================================================================
// CMFCToolBarImages
//=============================================================================

namespace {
struct ToolBarImagesState {
    int count = 0;
    BOOL isValid = FALSE;
    std::vector<HICON> ownedIcons;
};

thread_local std::unordered_map<const CMFCToolBarImages*, ToolBarImagesState> g_toolbarImagesState;

ToolBarImagesState& EnsureToolBarImagesState(const CMFCToolBarImages* pImages) {
    return g_toolbarImagesState[pImages];
}

const ToolBarImagesState* FindToolBarImagesState(const CMFCToolBarImages* pImages) {
    auto it = g_toolbarImagesState.find(pImages);
    return (it != g_toolbarImagesState.end()) ? &it->second : nullptr;
}

void ClearToolBarImagesState(CMFCToolBarImages* pImages, BOOL bDestroyIcons) {
    if (!pImages) return;
    auto it = g_toolbarImagesState.find(pImages);
    if (it == g_toolbarImagesState.end()) return;
    if (bDestroyIcons) {
        for (HICON hIcon : it->second.ownedIcons) {
            if (hIcon) ::DestroyIcon(hIcon);
        }
    }
    it->second.ownedIcons.clear();
    it->second.count = 0;
    it->second.isValid = FALSE;
}

void RemoveToolBarImagesState(CMFCToolBarImages* pImages) {
    if (!pImages) return;
    auto it = g_toolbarImagesState.find(pImages);
    if (it == g_toolbarImagesState.end()) return;
    for (HICON hIcon : it->second.ownedIcons) {
        if (hIcon) ::DestroyIcon(hIcon);
    }
    g_toolbarImagesState.erase(it);
}
} // namespace

// Static member definitions - provided without // Symbol: comments so typed_stubs
// handles the MSVC-named exports. We define them here for C++ completeness but
// the linker uses the impl__ symbols from typed_stubs for the MSVC exports.
BOOL CMFCToolBarImages::m_bDisableTrueColorAlpha = FALSE;
BOOL CMFCToolBarImages::m_bIsDrawOnGlass = FALSE;
BOOL CMFCToolBarImages::m_bMultiThreaded = FALSE;

// m_bIsRTL / m_nDisabledImageAlpha / m_nFadedImageAlpha are static in retail
// MFC (AFX_IMPORT_DATA static in afxtoolbarimages.h), so they get definitions
// here rather than per-object initialization.
BOOL CMFCToolBarImages::m_bIsRTL = FALSE;
BYTE CMFCToolBarImages::m_nDisabledImageAlpha = 127;
BYTE CMFCToolBarImages::m_nFadedImageAlpha = 127;

// Zero the retail member block and apply the non-zero retail defaults. Shared
// by both constructors.
void CMFCToolBarImages::InitMembers() {
    CMFCToolBarImages* const p = this;
    std::memset(reinterpret_cast<char*>(p) + sizeof(CObject), 0,
                sizeof(CMFCToolBarImages) - sizeof(CObject));
    new (&p->m_strUDLPath) CString();
    p->m_nBitsPerPixel = 0;
    p->m_nGrayImageLuminancePercentage = 0;
    p->m_nLightPercentage = 0;
    p->m_dblScale = 1.0;
    p->m_clrTransparent = RGB(192, 192, 192);
    p->m_clrTransparentOriginal = p->m_clrTransparent;
    p->m_clrImageShadow = RGB(128, 128, 128);
}
#define InitImagesMembers() InitMembers()

// Symbol: ??0CMFCToolBarImages@@QEAA@XZ
CMFCToolBarImages::CMFCToolBarImages() {
    InitImagesMembers();
}

// Symbol: ??0CMFCToolBarImages@@QEAA@H@Z
CMFCToolBarImages::CMFCToolBarImages(BOOL bReadOnly) {
    InitImagesMembers();
    m_bReadOnly = bReadOnly;
}

// Symbol: ??1CMFCToolBarImages@@UEAA@XZ
CMFCToolBarImages::~CMFCToolBarImages() {
    RemoveToolBarImagesState(this);
}

// Symbol: ?Load@CMFCToolBarImages@@QEAAHIPEAUHINSTANCE__@@H@Z
BOOL CMFCToolBarImages::Load(UINT nIDResource, HINSTANCE hInstRes, BOOL bAdd) {
    if (!hInstRes) hInstRes = AfxGetInstanceHandle();
    HBITMAP hbm = ::LoadBitmapW(hInstRes, MAKEINTRESOURCEW(nIDResource));
    if (!hbm) return FALSE;
    ::DeleteObject(hbm);
    ToolBarImagesState& state = EnsureToolBarImagesState(this);
    if (!bAdd) state.count = 0;
    ++state.count;
    state.isValid = TRUE;
    return TRUE;
}

// Symbol: ?Load@CMFCToolBarImages@@QEAAHPEB_WK@Z
BOOL CMFCToolBarImages::Load(const wchar_t* lpszBitmapFileName, DWORD) {
    if (!lpszBitmapFileName || lpszBitmapFileName[0] == 0) return FALSE;
    ToolBarImagesState& state = EnsureToolBarImagesState(this);
    state.count = 1;
    state.isValid = TRUE;
    return TRUE;
}

// Symbol: ?LoadStr@CMFCToolBarImages@@QEAAHPEB_WPEAUHINSTANCE__@@H@Z
BOOL CMFCToolBarImages::LoadStr(const wchar_t* lpszResourceName, HINSTANCE hInstRes, BOOL bAdd) {
    if (!lpszResourceName) return FALSE;
    if (!hInstRes) hInstRes = AfxGetInstanceHandle();
    HBITMAP hbm = ::LoadBitmapW(hInstRes, lpszResourceName);
    if (!hbm) return FALSE;
    ::DeleteObject(hbm);
    ToolBarImagesState& state = EnsureToolBarImagesState(this);
    if (!bAdd) state.count = 0;
    ++state.count;
    state.isValid = TRUE;
    return TRUE;
}

// Symbol: ?Save@CMFCToolBarImages@@QEAAHPEB_W@Z
BOOL CMFCToolBarImages::Save(const wchar_t*) {
    return IsValid();
}

// Symbol: ?AddImage@CMFCToolBarImages@@QEAAHPEAUHBITMAP__@@H@Z
int CMFCToolBarImages::AddImage(HBITMAP hBitmap, BOOL) {
    if (!hBitmap) return -1;
    ToolBarImagesState& state = EnsureToolBarImagesState(this);
    const int index = state.count;
    ++state.count;
    state.isValid = TRUE;
    return index;
}

// Symbol: ?AddImage@CMFCToolBarImages@@QEAAHAEBV1@H@Z
int CMFCToolBarImages::AddImage(const CMFCToolBarImages& images, BOOL) {
    const int addCount = images.GetCount();
    if (addCount <= 0) return -1;
    ToolBarImagesState& state = EnsureToolBarImagesState(this);
    const int index = state.count;
    state.count += addCount;
    state.isValid = TRUE;
    return index;
}

// Symbol: ?AddIcon@CMFCToolBarImages@@QEAAHPEAUHICON__@@H@Z
int CMFCToolBarImages::AddIcon(HICON hIcon, BOOL bAutoDestroy) {
    if (!hIcon) return -1;
    ToolBarImagesState& state = EnsureToolBarImagesState(this);
    const int index = state.count;
    ++state.count;
    state.isValid = TRUE;
    if (bAutoDestroy) {
        state.ownedIcons.push_back(hIcon);
    }
    return index;
}

// Symbol: ?Clear@CMFCToolBarImages@@QEAAXXZ
void CMFCToolBarImages::Clear() {
    InitMembers();
    ClearToolBarImagesState(this, TRUE);
}

// Symbol: ?Initialize@CMFCToolBarImages@@QEAAXXZ
void CMFCToolBarImages::Initialize() {
    CommonInit(TRUE);
}

// Symbol: ?CommonInit@CMFCToolBarImages@@QEAAXH@Z
void CMFCToolBarImages::CommonInit(BOOL bFreeImageList) {
    m_bIsRTL = FALSE;                 // statics: reset the shared defaults
    m_nDisabledImageAlpha = 127;
    m_nFadedImageAlpha = 127;
    InitMembers();
    ClearToolBarImagesState(this, bFreeImageList);
}

// Symbol: ?CleanUp@CMFCToolBarImages@@SAXXZ
// (static)
void CMFCToolBarImages__CleanUp() {
    for (auto& entry : g_toolbarImagesState) {
        for (HICON hIcon : entry.second.ownedIcons) {
            if (hIcon) ::DestroyIcon(hIcon);
        }
    }
    g_toolbarImagesState.clear();
}

int CMFCToolBarImages::GetCount() const {
    const ToolBarImagesState* state = FindToolBarImagesState(this);
    return state ? state->count : 0;
}

BOOL CMFCToolBarImages::IsValid() const {
    const ToolBarImagesState* state = FindToolBarImagesState(this);
    return state ? state->isValid : FALSE;
}

// --- extern "C" MS_ABI thunks ---


// Symbol: ??0CMFCToolBarImages@@QEAA@XZ
extern "C" void* MS_ABI impl___0CMFCToolBarImages__QEAA_XZ(void* pThis) {
    return new(pThis) CMFCToolBarImages();
}

// Symbol: ??0CMFCToolBarImages@@QEAA@H@Z
extern "C" void* MS_ABI impl___0CMFCToolBarImages__QEAA_H_Z(void* pThis, int p0) {
    return new(pThis) CMFCToolBarImages((BOOL)p0);
}

// Symbol: ??1CMFCToolBarImages@@UEAA@XZ
extern "C" void MS_ABI impl___1CMFCToolBarImages__UEAA_XZ(void* pThis) {
    ((CMFCToolBarImages*)pThis)->~CMFCToolBarImages();
}

// Symbol: ?Load@CMFCToolBarImages@@QEAAHIPEAUHINSTANCE__@@H@Z
extern "C" int MS_ABI impl__Load_CMFCToolBarImages__QEAAHIPEAUHINSTANCE____H_Z(CMFCToolBarImages* pThis, unsigned int nID, HINSTANCE hInst, int bAdd) {
    return pThis->Load(nID, hInst, (BOOL)bAdd);
}

// Symbol: ?Load@CMFCToolBarImages@@QEAAHPEB_WK@Z
extern "C" int MS_ABI impl__Load_CMFCToolBarImages__QEAAHPEB_WK_Z(CMFCToolBarImages* pThis, const wchar_t* lpszFile, unsigned long dwMax) {
    return pThis->Load(lpszFile, dwMax);
}

// Symbol: ?LoadStr@CMFCToolBarImages@@QEAAHPEB_WPEAUHINSTANCE__@@H@Z
extern "C" int MS_ABI impl__LoadStr_CMFCToolBarImages__QEAAHPEB_WPEAUHINSTANCE____H_Z(CMFCToolBarImages* pThis, const wchar_t* lpszName, HINSTANCE hInst, int bAdd) {
    return pThis->LoadStr(lpszName, hInst, (BOOL)bAdd);
}

// Symbol: ?Save@CMFCToolBarImages@@QEAAHPEB_W@Z
extern "C" int MS_ABI impl__Save_CMFCToolBarImages__QEAAHPEB_W_Z(CMFCToolBarImages* pThis, const wchar_t* lpszFile) {
    return pThis->Save(lpszFile);
}

// Symbol: ?AddImage@CMFCToolBarImages@@QEAAHPEAUHBITMAP__@@H@Z
extern "C" int MS_ABI impl__AddImage_CMFCToolBarImages__QEAAHPEAUHBITMAP____H_Z(CMFCToolBarImages* pThis, HBITMAP hBmp, int bSet) {
    return pThis->AddImage(hBmp, (BOOL)bSet);
}

// Symbol: ?AddImage@CMFCToolBarImages@@QEAAHAEBV1@H@Z
extern "C" int MS_ABI impl__AddImage_CMFCToolBarImages__QEAAHAEBV1_H_Z(CMFCToolBarImages* pThis, const CMFCToolBarImages* pImages, int bSet) {
    return pThis->AddImage(*pImages, (BOOL)bSet);
}

// Symbol: ?AddIcon@CMFCToolBarImages@@QEAAHPEAUHICON__@@H@Z
extern "C" int MS_ABI impl__AddIcon_CMFCToolBarImages__QEAAHPEAUHICON____H_Z(CMFCToolBarImages* pThis, HICON hIcon, int bAutoDestroy) {
    return pThis->AddIcon(hIcon, (BOOL)bAutoDestroy);
}

// Symbol: ?Clear@CMFCToolBarImages@@QEAAXXZ
extern "C" void MS_ABI impl__Clear_CMFCToolBarImages__QEAAXXZ(CMFCToolBarImages* pThis) {
    pThis->Clear();
}

// Symbol: ?Initialize@CMFCToolBarImages@@QEAAXXZ
extern "C" void MS_ABI impl__Initialize_CMFCToolBarImages__QEAAXXZ(CMFCToolBarImages* pThis) {
    pThis->Initialize();
}

// Symbol: ?CommonInit@CMFCToolBarImages@@QEAAXH@Z
extern "C" void MS_ABI impl__CommonInit_CMFCToolBarImages__QEAAXH_Z(CMFCToolBarImages* pThis, int bFree) {
    pThis->CommonInit((BOOL)bFree);
}

// Symbol: ?CleanUp@CMFCToolBarImages@@SAXXZ
extern "C" void MS_ABI impl__CleanUp_CMFCToolBarImages__SAXXZ() {
    CMFCToolBarImages__CleanUp();
}

// =============================================================================
// Wave 2 D2D/Animation minimal implementations
// =============================================================================

IMPLEMENT_DYNAMIC(CRenderTarget, CObject)
IMPLEMENT_DYNAMIC(CDCRenderTarget, CRenderTarget)
IMPLEMENT_DYNAMIC(CAnimationVariable, CObject)

// Symbol: ?GetRuntimeClass@CRenderTarget@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CRenderTarget__UEBAPEAUCRuntimeClass__XZ(const CRenderTarget* pThis) {
    (void)pThis;
    return CRenderTarget::GetThisClass();
}

// Symbol: ?GetThisClass@CRenderTarget@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CRenderTarget__SAPEAUCRuntimeClass__XZ() {
    return CRenderTarget::GetThisClass();
}

// Symbol: ?GetRuntimeClass@CDCRenderTarget@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CDCRenderTarget__UEBAPEAUCRuntimeClass__XZ(const CDCRenderTarget* pThis) {
    (void)pThis;
    return CDCRenderTarget::GetThisClass();
}

// Symbol: ?GetThisClass@CDCRenderTarget@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CDCRenderTarget__SAPEAUCRuntimeClass__XZ() {
    return CDCRenderTarget::GetThisClass();
}

static CD2DPointF* InitCD2DPointF(CD2DPointF* pThis, float xValue, float yValue) {
    if (!pThis) return nullptr;
    pThis->x = xValue;
    pThis->y = yValue;
    return pThis;
}

static CD2DSizeF* InitCD2DSizeF(CD2DSizeF* pThis, float widthValue, float heightValue) {
    if (!pThis) return nullptr;
    pThis->width = widthValue;
    pThis->height = heightValue;
    return pThis;
}

static CD2DRectF* InitCD2DRectF(CD2DRectF* pThis, float leftValue, float topValue, float rightValue, float bottomValue) {
    if (!pThis) return nullptr;
    pThis->left = leftValue;
    pThis->top = topValue;
    pThis->right = rightValue;
    pThis->bottom = bottomValue;
    return pThis;
}

// Symbol: ??0CD2DPointF@@QEAA@MM@Z
extern "C" CD2DPointF* MS_ABI impl___0CD2DPointF__QEAA_MM_Z(CD2DPointF* pThis, float xValue, float yValue) {
    return InitCD2DPointF(pThis, xValue, yValue);
}

// Symbol: ??0CD2DPointF@@QEAA@AEBVCPoint@@@Z
extern "C" CD2DPointF* MS_ABI impl___0CD2DPointF__QEAA_AEBVCPoint___Z(CD2DPointF* pThis, const CPoint* pPoint) {
    if (!pPoint) return InitCD2DPointF(pThis, 0.0f, 0.0f);
    return InitCD2DPointF(pThis, (float)pPoint->x, (float)pPoint->y);
}

// Symbol: ??0CD2DPointF@@QEAA@AEBUD2D_POINT_2F@@@Z
extern "C" CD2DPointF* MS_ABI impl___0CD2DPointF__QEAA_AEBUD2D_POINT_2F___Z(CD2DPointF* pThis, const void* pPoint) {
    const auto* point = static_cast<const D2D_POINT_2F_L*>(pPoint);
    return point ? InitCD2DPointF(pThis, point->x, point->y) : InitCD2DPointF(pThis, 0.0f, 0.0f);
}

// Symbol: ??0CD2DPointF@@QEAA@PEBUD2D_POINT_2F@@@Z
extern "C" CD2DPointF* MS_ABI impl___0CD2DPointF__QEAA_PEBUD2D_POINT_2F___Z(CD2DPointF* pThis, const void* pPoint) {
    return impl___0CD2DPointF__QEAA_AEBUD2D_POINT_2F___Z(pThis, pPoint);
}

CD2DPointF::CD2DPointF() : x(0.0f), y(0.0f) {}
CD2DPointF::CD2DPointF(float xValue, float yValue) { impl___0CD2DPointF__QEAA_MM_Z(this, xValue, yValue); }
CD2DPointF::CD2DPointF(const CPoint& point) { impl___0CD2DPointF__QEAA_AEBVCPoint___Z(this, &point); }
CD2DPointF::CD2DPointF(const D2D_POINT_2F& point) { impl___0CD2DPointF__QEAA_AEBUD2D_POINT_2F___Z(this, &point); }
CD2DPointF::CD2DPointF(const D2D_POINT_2F* point) { impl___0CD2DPointF__QEAA_PEBUD2D_POINT_2F___Z(this, point); }

// Symbol: ??0CD2DSizeF@@QEAA@MM@Z
extern "C" CD2DSizeF* MS_ABI impl___0CD2DSizeF__QEAA_MM_Z(CD2DSizeF* pThis, float widthValue, float heightValue) {
    return InitCD2DSizeF(pThis, widthValue, heightValue);
}

// Symbol: ??0CD2DSizeF@@QEAA@AEBVCSize@@@Z
extern "C" CD2DSizeF* MS_ABI impl___0CD2DSizeF__QEAA_AEBVCSize___Z(CD2DSizeF* pThis, const CSize* pSize) {
    if (!pSize) return InitCD2DSizeF(pThis, 0.0f, 0.0f);
    return InitCD2DSizeF(pThis, (float)pSize->cx, (float)pSize->cy);
}

// Symbol: ??0CD2DSizeF@@QEAA@AEBUD2D_SIZE_F@@@Z
extern "C" CD2DSizeF* MS_ABI impl___0CD2DSizeF__QEAA_AEBUD2D_SIZE_F___Z(CD2DSizeF* pThis, const void* pSize) {
    const auto* size = static_cast<const D2D_SIZE_F_L*>(pSize);
    return size ? InitCD2DSizeF(pThis, size->width, size->height) : InitCD2DSizeF(pThis, 0.0f, 0.0f);
}

// Symbol: ??0CD2DSizeF@@QEAA@PEBUD2D_SIZE_F@@@Z
extern "C" CD2DSizeF* MS_ABI impl___0CD2DSizeF__QEAA_PEBUD2D_SIZE_F___Z(CD2DSizeF* pThis, const void* pSize) {
    return impl___0CD2DSizeF__QEAA_AEBUD2D_SIZE_F___Z(pThis, pSize);
}

CD2DSizeF::CD2DSizeF() : width(0.0f), height(0.0f) {}
CD2DSizeF::CD2DSizeF(float widthValue, float heightValue) { impl___0CD2DSizeF__QEAA_MM_Z(this, widthValue, heightValue); }
CD2DSizeF::CD2DSizeF(const CSize& size) { impl___0CD2DSizeF__QEAA_AEBVCSize___Z(this, &size); }
CD2DSizeF::CD2DSizeF(const D2D_SIZE_F& size) { impl___0CD2DSizeF__QEAA_AEBUD2D_SIZE_F___Z(this, &size); }
CD2DSizeF::CD2DSizeF(const D2D_SIZE_F* size) { impl___0CD2DSizeF__QEAA_PEBUD2D_SIZE_F___Z(this, size); }

// Symbol: ??0CD2DRectF@@QEAA@MMMM@Z
extern "C" CD2DRectF* MS_ABI impl___0CD2DRectF__QEAA_MMMM_Z(
    CD2DRectF* pThis, float leftValue, float topValue, float rightValue, float bottomValue) {
    return InitCD2DRectF(pThis, leftValue, topValue, rightValue, bottomValue);
}

// Symbol: ??0CD2DRectF@@QEAA@AEBVCRect@@@Z
extern "C" CD2DRectF* MS_ABI impl___0CD2DRectF__QEAA_AEBVCRect___Z(CD2DRectF* pThis, const CRect* pRect) {
    if (!pRect) return InitCD2DRectF(pThis, 0.0f, 0.0f, 0.0f, 0.0f);
    return InitCD2DRectF(pThis, (float)pRect->left, (float)pRect->top, (float)pRect->right, (float)pRect->bottom);
}

// Symbol: ??0CD2DRectF@@QEAA@AEBUD2D_RECT_F@@@Z
extern "C" CD2DRectF* MS_ABI impl___0CD2DRectF__QEAA_AEBUD2D_RECT_F___Z(CD2DRectF* pThis, const void* pRect) {
    const auto* rect = static_cast<const D2D_RECT_F_L*>(pRect);
    if (!rect) return InitCD2DRectF(pThis, 0.0f, 0.0f, 0.0f, 0.0f);
    return InitCD2DRectF(pThis, rect->left, rect->top, rect->right, rect->bottom);
}

// Symbol: ??0CD2DRectF@@QEAA@PEBUD2D_RECT_F@@@Z
extern "C" CD2DRectF* MS_ABI impl___0CD2DRectF__QEAA_PEBUD2D_RECT_F___Z(CD2DRectF* pThis, const void* pRect) {
    return impl___0CD2DRectF__QEAA_AEBUD2D_RECT_F___Z(pThis, pRect);
}

CD2DRectF::CD2DRectF() : left(0.0f), top(0.0f), right(0.0f), bottom(0.0f) {}
CD2DRectF::CD2DRectF(float leftValue, float topValue, float rightValue, float bottomValue) {
    impl___0CD2DRectF__QEAA_MMMM_Z(this, leftValue, topValue, rightValue, bottomValue);
}
CD2DRectF::CD2DRectF(const CRect& rect) { impl___0CD2DRectF__QEAA_AEBVCRect___Z(this, &rect); }
CD2DRectF::CD2DRectF(const D2D_RECT_F& rect) { impl___0CD2DRectF__QEAA_AEBUD2D_RECT_F___Z(this, &rect); }
CD2DRectF::CD2DRectF(const D2D_RECT_F* rect) { impl___0CD2DRectF__QEAA_PEBUD2D_RECT_F___Z(this, rect); }

// Symbol: ??0CD2DEllipse@@QEAA@AEBVCD2DPointF@@AEBVCD2DSizeF@@@Z
extern "C" CD2DEllipse* MS_ABI impl___0CD2DEllipse__QEAA_AEBVCD2DPointF__AEBVCD2DSizeF___Z(
    CD2DEllipse* pThis, const CD2DPointF* pPoint, const CD2DSizeF* pRadius) {
    if (!pThis) return nullptr;
    pThis->point = pPoint ? *pPoint : CD2DPointF();
    pThis->radius = pRadius ? *pRadius : CD2DSizeF();
    return pThis;
}

// Symbol: ??0CD2DEllipse@@QEAA@AEBVCD2DRectF@@@Z
extern "C" CD2DEllipse* MS_ABI impl___0CD2DEllipse__QEAA_AEBVCD2DRectF___Z(CD2DEllipse* pThis, const CD2DRectF* pRect) {
    if (!pThis) return nullptr;
    const CD2DRectF rect = pRect ? *pRect : CD2DRectF();
    const float radiusX = std::fabs((rect.right - rect.left) * 0.5f);
    const float radiusY = std::fabs((rect.bottom - rect.top) * 0.5f);
    const CD2DPointF point((rect.left + rect.right) * 0.5f, (rect.top + rect.bottom) * 0.5f);
    const CD2DSizeF radius(radiusX, radiusY);
    return impl___0CD2DEllipse__QEAA_AEBVCD2DPointF__AEBVCD2DSizeF___Z(pThis, &point, &radius);
}

// Symbol: ??0CD2DEllipse@@QEAA@AEBUD2D1_ELLIPSE@@@Z
extern "C" CD2DEllipse* MS_ABI impl___0CD2DEllipse__QEAA_AEBUD2D1_ELLIPSE___Z(CD2DEllipse* pThis, const void* pEllipse) {
    const auto* ellipse = static_cast<const D2D1_ELLIPSE_L*>(pEllipse);
    if (!ellipse) return impl___0CD2DEllipse__QEAA_AEBVCD2DPointF__AEBVCD2DSizeF___Z(pThis, nullptr, nullptr);
    const CD2DPointF point(ellipse->point.x, ellipse->point.y);
    const CD2DSizeF radius(ellipse->radiusX, ellipse->radiusY);
    return impl___0CD2DEllipse__QEAA_AEBVCD2DPointF__AEBVCD2DSizeF___Z(pThis, &point, &radius);
}

// Symbol: ??0CD2DEllipse@@QEAA@PEBUD2D1_ELLIPSE@@@Z
extern "C" CD2DEllipse* MS_ABI impl___0CD2DEllipse__QEAA_PEBUD2D1_ELLIPSE___Z(CD2DEllipse* pThis, const void* pEllipse) {
    return impl___0CD2DEllipse__QEAA_AEBUD2D1_ELLIPSE___Z(pThis, pEllipse);
}

CD2DEllipse::CD2DEllipse() : point(), radius() {}
CD2DEllipse::CD2DEllipse(const CD2DPointF& pointValue, const CD2DSizeF& radiusValue) {
    impl___0CD2DEllipse__QEAA_AEBVCD2DPointF__AEBVCD2DSizeF___Z(this, &pointValue, &radiusValue);
}
CD2DEllipse::CD2DEllipse(const CD2DRectF& rect) { impl___0CD2DEllipse__QEAA_AEBVCD2DRectF___Z(this, &rect); }
CD2DEllipse::CD2DEllipse(const D2D1_ELLIPSE& ellipse) { impl___0CD2DEllipse__QEAA_AEBUD2D1_ELLIPSE___Z(this, &ellipse); }
CD2DEllipse::CD2DEllipse(const D2D1_ELLIPSE* ellipse) { impl___0CD2DEllipse__QEAA_PEBUD2D1_ELLIPSE___Z(this, ellipse); }

// Symbol: ??0CD2DRoundedRect@@QEAA@AEBVCD2DRectF@@AEBVCD2DSizeF@@@Z
extern "C" CD2DRoundedRect* MS_ABI impl___0CD2DRoundedRect__QEAA_AEBVCD2DRectF__AEBVCD2DSizeF___Z(
    CD2DRoundedRect* pThis, const CD2DRectF* pRect, const CD2DSizeF* pRadius) {
    if (!pThis) return nullptr;
    pThis->rect = pRect ? *pRect : CD2DRectF();
    pThis->radius = pRadius ? *pRadius : CD2DSizeF();
    return pThis;
}

// Symbol: ??0CD2DRoundedRect@@QEAA@AEBUD2D1_ROUNDED_RECT@@@Z
extern "C" CD2DRoundedRect* MS_ABI impl___0CD2DRoundedRect__QEAA_AEBUD2D1_ROUNDED_RECT___Z(
    CD2DRoundedRect* pThis, const void* pRoundedRect) {
    const auto* roundedRect = static_cast<const D2D1_ROUNDED_RECT_L*>(pRoundedRect);
    if (!roundedRect) return impl___0CD2DRoundedRect__QEAA_AEBVCD2DRectF__AEBVCD2DSizeF___Z(pThis, nullptr, nullptr);
    const CD2DRectF rect(roundedRect->rect.left, roundedRect->rect.top, roundedRect->rect.right, roundedRect->rect.bottom);
    const CD2DSizeF radius(roundedRect->radiusX, roundedRect->radiusY);
    return impl___0CD2DRoundedRect__QEAA_AEBVCD2DRectF__AEBVCD2DSizeF___Z(pThis, &rect, &radius);
}

// Symbol: ??0CD2DRoundedRect@@QEAA@PEBUD2D1_ROUNDED_RECT@@@Z
extern "C" CD2DRoundedRect* MS_ABI impl___0CD2DRoundedRect__QEAA_PEBUD2D1_ROUNDED_RECT___Z(
    CD2DRoundedRect* pThis, const void* pRoundedRect) {
    return impl___0CD2DRoundedRect__QEAA_AEBUD2D1_ROUNDED_RECT___Z(pThis, pRoundedRect);
}

CD2DRoundedRect::CD2DRoundedRect() : rect(), radius() {}
CD2DRoundedRect::CD2DRoundedRect(const CD2DRectF& rectValue, const CD2DSizeF& radiusValue) {
    impl___0CD2DRoundedRect__QEAA_AEBVCD2DRectF__AEBVCD2DSizeF___Z(this, &rectValue, &radiusValue);
}
CD2DRoundedRect::CD2DRoundedRect(const D2D1_ROUNDED_RECT& roundedRect) {
    impl___0CD2DRoundedRect__QEAA_AEBUD2D1_ROUNDED_RECT___Z(this, &roundedRect);
}
CD2DRoundedRect::CD2DRoundedRect(const D2D1_ROUNDED_RECT* roundedRect) {
    impl___0CD2DRoundedRect__QEAA_PEBUD2D1_ROUNDED_RECT___Z(this, roundedRect);
}

// Symbol: ??0CRenderTarget@@QEAA@XZ
extern "C" CRenderTarget* MS_ABI impl___0CRenderTarget__QEAA_XZ(CRenderTarget* pThis) {
    if (!pThis) return nullptr;
    std::lock_guard<std::mutex> lock(g_wave2StateMutex);
    g_renderTargetState[pThis] = {};
    return pThis;
}

// Symbol: ??1CRenderTarget@@UEAA@XZ
extern "C" void MS_ABI impl___1CRenderTarget__UEAA_XZ(CRenderTarget* pThis) {
    if (!pThis) return;
    std::lock_guard<std::mutex> lock(g_wave2StateMutex);
    g_renderTargetState.erase(pThis);
}

CRenderTarget::CRenderTarget() {
    impl___0CRenderTarget__QEAA_XZ(this);
    memset(_rendertarget_padding, 0, sizeof(_rendertarget_padding));
}

CRenderTarget::~CRenderTarget() {
    impl___1CRenderTarget__UEAA_XZ(this);
}

// Symbol: ?Attach@CRenderTarget@@QEAAXPEAUID2D1RenderTarget@@@Z
extern "C" void MS_ABI impl__Attach_CRenderTarget__QEAAXPEAUID2D1RenderTarget___Z(CRenderTarget* pThis, void* pRenderTarget) {
    if (!pThis) return;
    std::lock_guard<std::mutex> lock(g_wave2StateMutex);
    g_renderTargetState[pThis].resource = pRenderTarget;
}

void CRenderTarget::Attach(void* pRenderTarget) {
    impl__Attach_CRenderTarget__QEAAXPEAUID2D1RenderTarget___Z(this, pRenderTarget);
}

// Symbol: ?Detach@CRenderTarget@@QEAAPEAUID2D1RenderTarget@@XZ
extern "C" void* MS_ABI impl__Detach_CRenderTarget__QEAAPEAUID2D1RenderTarget__XZ(CRenderTarget* pThis) {
    if (!pThis) return nullptr;
    std::lock_guard<std::mutex> lock(g_wave2StateMutex);
    auto& state = g_renderTargetState[pThis];
    void* old = state.resource;
    state.resource = nullptr;
    return old;
}

void* CRenderTarget::Detach() {
    return impl__Detach_CRenderTarget__QEAAPEAUID2D1RenderTarget__XZ(this);
}

// Symbol: ?BeginDraw@CRenderTarget@@QEAAXXZ
extern "C" void MS_ABI impl__BeginDraw_CRenderTarget__QEAAXXZ(CRenderTarget* pThis) {
    if (!pThis) return;
    std::lock_guard<std::mutex> lock(g_wave2StateMutex);
    g_renderTargetState[pThis].drawing = true;
}

void CRenderTarget::BeginDraw() {
    impl__BeginDraw_CRenderTarget__QEAAXXZ(this);
}

// Symbol: ?EndDraw@CRenderTarget@@QEAAJXZ
extern "C" long MS_ABI impl__EndDraw_CRenderTarget__QEAAJXZ(CRenderTarget* pThis) {
    if (!pThis) return E_POINTER;
    std::lock_guard<std::mutex> lock(g_wave2StateMutex);
    g_renderTargetState[pThis].drawing = false;
    return S_OK;
}

long CRenderTarget::EndDraw() {
    return impl__EndDraw_CRenderTarget__QEAAJXZ(this);
}

// Symbol: ?Destroy@CRenderTarget@@QEAAHH@Z
extern "C" int MS_ABI impl__Destroy_CRenderTarget__QEAAHH_Z(CRenderTarget* pThis, int bReleasing) {
    (void)bReleasing;
    if (!pThis) return FALSE;
    std::lock_guard<std::mutex> lock(g_wave2StateMutex);
    auto& state = g_renderTargetState[pThis];
    state.resource = nullptr;
    state.drawing = false;
    state.size = CD2DSizeF(0.0f, 0.0f);
    state.tag1 = 0;
    state.tag2 = 0;
    return TRUE;
}

int CRenderTarget::Destroy(int bReleasing) {
    return impl__Destroy_CRenderTarget__QEAAHH_Z(this, bReleasing);
}

// Symbol: ?Clear@CRenderTarget@@QEAAXU_D3DCOLORVALUE@@@Z
extern "C" void MS_ABI impl__Clear_CRenderTarget__QEAAXU_D3DCOLORVALUE___Z(CRenderTarget* pThis, CD2DColorF color) {
    (void)pThis;
    (void)color;
}

void CRenderTarget::Clear(CD2DColorF color) {
    impl__Clear_CRenderTarget__QEAAXU_D3DCOLORVALUE___Z(this, color);
}

// Symbol: ?DrawLine@CRenderTarget@@QEAAXAEBVCD2DPointF@@0PEAVCD2DBrush@@MPEAUID2D1StrokeStyle@@@Z
extern "C" void MS_ABI impl__DrawLine_CRenderTarget__QEAAXAEBVCD2DPointF__0PEAVCD2DBrush__MPEAUID2D1StrokeStyle___Z(
    CRenderTarget* pThis, const CD2DPointF* p0, const CD2DPointF* p1, void* pBrush, float strokeWidth, void* pStrokeStyle) {
    (void)pThis;
    (void)p0;
    (void)p1;
    (void)pBrush;
    (void)strokeWidth;
    (void)pStrokeStyle;
}

void CRenderTarget::DrawLine(const CD2DPointF& p0, const CD2DPointF& p1) {
    impl__DrawLine_CRenderTarget__QEAAXAEBVCD2DPointF__0PEAVCD2DBrush__MPEAUID2D1StrokeStyle___Z(
        this, &p0, &p1, nullptr, 1.0f, nullptr);
}

// Symbol: ?DrawRectangle@CRenderTarget@@QEAAXAEBVCD2DRectF@@PEAVCD2DBrush@@MPEAUID2D1StrokeStyle@@@Z
extern "C" void MS_ABI impl__DrawRectangle_CRenderTarget__QEAAXAEBVCD2DRectF__PEAVCD2DBrush__MPEAUID2D1StrokeStyle___Z(
    CRenderTarget* pThis, const CD2DRectF* pRect, void* pBrush, float strokeWidth, void* pStrokeStyle) {
    (void)pThis;
    (void)pRect;
    (void)pBrush;
    (void)strokeWidth;
    (void)pStrokeStyle;
}

void CRenderTarget::DrawRectangle(const CD2DRectF& rect) {
    impl__DrawRectangle_CRenderTarget__QEAAXAEBVCD2DRectF__PEAVCD2DBrush__MPEAUID2D1StrokeStyle___Z(
        this, &rect, nullptr, 1.0f, nullptr);
}

// Symbol: ?DrawEllipse@CRenderTarget@@QEAAXAEBVCD2DEllipse@@PEAVCD2DBrush@@MPEAUID2D1StrokeStyle@@@Z
extern "C" void MS_ABI impl__DrawEllipse_CRenderTarget__QEAAXAEBVCD2DEllipse__PEAVCD2DBrush__MPEAUID2D1StrokeStyle___Z(
    CRenderTarget* pThis, const CD2DEllipse* pEllipse, void* pBrush, float strokeWidth, void* pStrokeStyle) {
    (void)pThis;
    (void)pEllipse;
    (void)pBrush;
    (void)strokeWidth;
    (void)pStrokeStyle;
}

void CRenderTarget::DrawEllipse(const CD2DEllipse& ellipse) {
    impl__DrawEllipse_CRenderTarget__QEAAXAEBVCD2DEllipse__PEAVCD2DBrush__MPEAUID2D1StrokeStyle___Z(
        this, &ellipse, nullptr, 1.0f, nullptr);
}

// Symbol: ?DrawRoundedRectangle@CRenderTarget@@QEAAXAEBVCD2DRoundedRect@@PEAVCD2DBrush@@MPEAUID2D1StrokeStyle@@@Z
extern "C" void MS_ABI impl__DrawRoundedRectangle_CRenderTarget__QEAAXAEBVCD2DRoundedRect__PEAVCD2DBrush__MPEAUID2D1StrokeStyle___Z(
    CRenderTarget* pThis, const CD2DRoundedRect* pRect, void* pBrush, float strokeWidth, void* pStrokeStyle) {
    (void)pThis;
    (void)pRect;
    (void)pBrush;
    (void)strokeWidth;
    (void)pStrokeStyle;
}

void CRenderTarget::DrawRoundedRectangle(const CD2DRoundedRect& rect) {
    impl__DrawRoundedRectangle_CRenderTarget__QEAAXAEBVCD2DRoundedRect__PEAVCD2DBrush__MPEAUID2D1StrokeStyle___Z(
        this, &rect, nullptr, 1.0f, nullptr);
}

// Symbol: ?FillRectangle@CRenderTarget@@QEAAXAEBVCD2DRectF@@PEAVCD2DBrush@@@Z
extern "C" void MS_ABI impl__FillRectangle_CRenderTarget__QEAAXAEBVCD2DRectF__PEAVCD2DBrush___Z(
    CRenderTarget* pThis, const CD2DRectF* pRect, void* pBrush) {
    (void)pThis;
    (void)pRect;
    (void)pBrush;
}

void CRenderTarget::FillRectangle(const CD2DRectF& rect) {
    impl__FillRectangle_CRenderTarget__QEAAXAEBVCD2DRectF__PEAVCD2DBrush___Z(this, &rect, nullptr);
}

// Symbol: ?FillEllipse@CRenderTarget@@QEAAXAEBVCD2DEllipse@@PEAVCD2DBrush@@@Z
extern "C" void MS_ABI impl__FillEllipse_CRenderTarget__QEAAXAEBVCD2DEllipse__PEAVCD2DBrush___Z(
    CRenderTarget* pThis, const CD2DEllipse* pEllipse, void* pBrush) {
    (void)pThis;
    (void)pEllipse;
    (void)pBrush;
}

void CRenderTarget::FillEllipse(const CD2DEllipse& ellipse) {
    impl__FillEllipse_CRenderTarget__QEAAXAEBVCD2DEllipse__PEAVCD2DBrush___Z(this, &ellipse, nullptr);
}

// Symbol: ?FillRoundedRectangle@CRenderTarget@@QEAAXAEBVCD2DRoundedRect@@PEAVCD2DBrush@@@Z
extern "C" void MS_ABI impl__FillRoundedRectangle_CRenderTarget__QEAAXAEBVCD2DRoundedRect__PEAVCD2DBrush___Z(
    CRenderTarget* pThis, const CD2DRoundedRect* pRect, void* pBrush) {
    (void)pThis;
    (void)pRect;
    (void)pBrush;
}

void CRenderTarget::FillRoundedRectangle(const CD2DRoundedRect& rect) {
    impl__FillRoundedRectangle_CRenderTarget__QEAAXAEBVCD2DRoundedRect__PEAVCD2DBrush___Z(this, &rect, nullptr);
}

// Symbol: ?DrawTextW@CRenderTarget@@QEAAXAEBV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@AEBVCD2DRectF@@PEAVCD2DBrush@@PEAVCD2DTextFormat@@W4D2D1_DRAW_TEXT_OPTIONS@@W4DWRITE_MEASURING_MODE@@@Z
extern "C" void MS_ABI impl__DrawTextW_CRenderTarget__QEAAXAEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__AEBVCD2DRectF__PEAVCD2DBrush__PEAVCD2DTextFormat__W4D2D1_DRAW_TEXT_OPTIONS__W4DWRITE_MEASURING_MODE___Z(
    CRenderTarget* pThis, const CString* text, const CD2DRectF* rect, void* brush, void* textFormat, int drawOptions, int measuringMode) {
    (void)brush;
    (void)textFormat;
    (void)drawOptions;
    (void)measuringMode;
    if (!pThis) return;

    std::lock_guard<std::mutex> lock(g_wave2StateMutex);
    auto& state = g_renderTargetState[pThis];
    state.lastText = text ? *text : CString();
    state.lastTextRect = rect ? *rect : CD2DRectF();
    if (rect) {
        if (rect->right > state.size.width) state.size.width = rect->right;
        if (rect->bottom > state.size.height) state.size.height = rect->bottom;
    }
}

// Symbol: ?DrawTextW@CRenderTarget@@QEAAXAEBV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@VCD2DRectF@@PEAVCD2DBrush@@PEAVCD2DTextFormat@@W4D2D1_DRAW_TEXT_OPTIONS@@W4DWRITE_MEASURING_MODE@@@Z
extern "C" void MS_ABI impl__DrawTextW_CRenderTarget__QEAAXAEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___VCD2DRectF__PEAVCD2DBrush__PEAVCD2DTextFormat__W4D2D1_DRAW_TEXT_OPTIONS__W4DWRITE_MEASURING_MODE___Z(
    CRenderTarget* pThis, const CString* text, CD2DRectF rect, void* brush, void* textFormat, int drawOptions, int measuringMode) {
    impl__DrawTextW_CRenderTarget__QEAAXAEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__AEBVCD2DRectF__PEAVCD2DBrush__PEAVCD2DTextFormat__W4D2D1_DRAW_TEXT_OPTIONS__W4DWRITE_MEASURING_MODE___Z(
        pThis, text, &rect, brush, textFormat, drawOptions, measuringMode);
}

// Symbol: ?COLORREF_TO_D2DCOLOR@CRenderTarget@@SA?AU_D3DCOLORVALUE@@KH@Z
extern "C" CD2DColorF MS_ABI impl__COLORREF_TO_D2DCOLOR_CRenderTarget__SA_AU_D3DCOLORVALUE__KH_Z(
    unsigned long color, int alpha) {
    CD2DColorF result = {};
    result.r = (float)GetRValue(color) / 255.0f;
    result.g = (float)GetGValue(color) / 255.0f;
    result.b = (float)GetBValue(color) / 255.0f;
    if (alpha < 0) alpha = 0;
    if (alpha > 255) alpha = 255;
    result.a = (float)alpha / 255.0f;
    return result;
}

CD2DColorF CRenderTarget::COLORREF_TO_D2DCOLOR(COLORREF color, int alpha) {
    return impl__COLORREF_TO_D2DCOLOR_CRenderTarget__SA_AU_D3DCOLORVALUE__KH_Z(color, alpha);
}

// Symbol: ?GetDpi@CRenderTarget@@QEBA?AVCD2DSizeF@@XZ
extern "C" CD2DSizeF MS_ABI impl__GetDpi_CRenderTarget__QEBA_AVCD2DSizeF__XZ(const CRenderTarget* pThis) {
    if (!pThis) return CD2DSizeF();
    std::lock_guard<std::mutex> lock(g_wave2StateMutex);
    return g_renderTargetState[pThis].dpi;
}

CD2DSizeF CRenderTarget::GetDpi() const {
    return impl__GetDpi_CRenderTarget__QEBA_AVCD2DSizeF__XZ(this);
}

// Symbol: ?SetDpi@CRenderTarget@@QEAAXAEBVCD2DSizeF@@@Z
extern "C" void MS_ABI impl__SetDpi_CRenderTarget__QEAAXAEBVCD2DSizeF___Z(CRenderTarget* pThis, const CD2DSizeF* pDpi) {
    if (!pThis) return;
    std::lock_guard<std::mutex> lock(g_wave2StateMutex);
    g_renderTargetState[pThis].dpi = pDpi ? *pDpi : CD2DSizeF(96.0f, 96.0f);
}

void CRenderTarget::SetDpi(const CD2DSizeF& dpi) {
    impl__SetDpi_CRenderTarget__QEAAXAEBVCD2DSizeF___Z(this, &dpi);
}

// Symbol: ?GetSize@CRenderTarget@@QEBA?AVCD2DSizeF@@XZ
extern "C" CD2DSizeF MS_ABI impl__GetSize_CRenderTarget__QEBA_AVCD2DSizeF__XZ(const CRenderTarget* pThis) {
    if (!pThis) return CD2DSizeF();
    std::lock_guard<std::mutex> lock(g_wave2StateMutex);
    return g_renderTargetState[pThis].size;
}

CD2DSizeF CRenderTarget::GetSize() const {
    return impl__GetSize_CRenderTarget__QEBA_AVCD2DSizeF__XZ(this);
}

// Symbol: ?GetTags@CRenderTarget@@QEBAXPEA_K0@Z
extern "C" void MS_ABI impl__GetTags_CRenderTarget__QEBAXPEA_K0_Z(
    const CRenderTarget* pThis, unsigned __int64* pTag1, unsigned __int64* pTag2) {
    if (!pThis) {
        if (pTag1) *pTag1 = 0;
        if (pTag2) *pTag2 = 0;
        return;
    }
    std::lock_guard<std::mutex> lock(g_wave2StateMutex);
    const auto& state = g_renderTargetState[pThis];
    if (pTag1) *pTag1 = state.tag1;
    if (pTag2) *pTag2 = state.tag2;
}

void CRenderTarget::GetTags(unsigned __int64* pTag1, unsigned __int64* pTag2) const {
    impl__GetTags_CRenderTarget__QEBAXPEA_K0_Z(this, pTag1, pTag2);
}

// Symbol: ?SetTags@CRenderTarget@@QEAAX_K0@Z
extern "C" void MS_ABI impl__SetTags_CRenderTarget__QEAAX_K0_Z(CRenderTarget* pThis, unsigned __int64 tag1, unsigned __int64 tag2) {
    if (!pThis) return;
    std::lock_guard<std::mutex> lock(g_wave2StateMutex);
    auto& state = g_renderTargetState[pThis];
    state.tag1 = tag1;
    state.tag2 = tag2;
}

void CRenderTarget::SetTags(unsigned __int64 tag1, unsigned __int64 tag2) {
    impl__SetTags_CRenderTarget__QEAAX_K0_Z(this, tag1, tag2);
}

// Symbol: ??0CDCRenderTarget@@QEAA@XZ
extern "C" CDCRenderTarget* MS_ABI impl___0CDCRenderTarget__QEAA_XZ(CDCRenderTarget* pThis) {
    if (!pThis) return nullptr;
    impl___0CRenderTarget__QEAA_XZ(pThis);
    return pThis;
}

CDCRenderTarget::CDCRenderTarget() {
    impl___0CDCRenderTarget__QEAA_XZ(this);
}

// Symbol: ?Attach@CDCRenderTarget@@QEAAXPEAUID2D1DCRenderTarget@@@Z
extern "C" void MS_ABI impl__Attach_CDCRenderTarget__QEAAXPEAUID2D1DCRenderTarget___Z(CDCRenderTarget* pThis, void* pRenderTarget) {
    if (!pThis) return;
    std::lock_guard<std::mutex> lock(g_wave2StateMutex);
    g_renderTargetState[pThis].resource = pRenderTarget;
}

void CDCRenderTarget::Attach(void* pRenderTarget) {
    impl__Attach_CDCRenderTarget__QEAAXPEAUID2D1DCRenderTarget___Z(this, pRenderTarget);
}

// Symbol: ?Detach@CDCRenderTarget@@QEAAPEAUID2D1DCRenderTarget@@XZ
extern "C" void* MS_ABI impl__Detach_CDCRenderTarget__QEAAPEAUID2D1DCRenderTarget__XZ(CDCRenderTarget* pThis) {
    return impl__Detach_CRenderTarget__QEAAPEAUID2D1RenderTarget__XZ(pThis);
}

void* CDCRenderTarget::Detach() {
    return impl__Detach_CDCRenderTarget__QEAAPEAUID2D1DCRenderTarget__XZ(this);
}

// Symbol: ?Create@CDCRenderTarget@@QEAAHAEBUD2D1_RENDER_TARGET_PROPERTIES@@@Z
extern "C" int MS_ABI impl__Create_CDCRenderTarget__QEAAHAEBUD2D1_RENDER_TARGET_PROPERTIES___Z(
    CDCRenderTarget* pThis, const void* pRenderTargetProperties) {
    (void)pRenderTargetProperties;
    if (!pThis) return FALSE;
    std::lock_guard<std::mutex> lock(g_wave2StateMutex);
    auto& state = g_renderTargetState[pThis];
    state.drawing = false;
    state.size = CD2DSizeF(0.0f, 0.0f);
    state.dpi = CD2DSizeF(96.0f, 96.0f);
    return TRUE;
}

int CDCRenderTarget::Create(const void* pRenderTargetProperties) {
    return impl__Create_CDCRenderTarget__QEAAHAEBUD2D1_RENDER_TARGET_PROPERTIES___Z(this, pRenderTargetProperties);
}

// Symbol: ?BindDC@CDCRenderTarget@@QEAAHAEBVCDC@@AEBVCRect@@@Z
extern "C" int MS_ABI impl__BindDC_CDCRenderTarget__QEAAHAEBVCDC__AEBVCRect___Z(
    CDCRenderTarget* pThis, const CDC* pDC, const CRect* pRect) {
    if (!pThis || !pDC || !pDC->GetSafeHdc()) return FALSE;
    std::lock_guard<std::mutex> lock(g_wave2StateMutex);
    auto& state = g_renderTargetState[pThis];
    if (pRect) {
        state.size = CD2DSizeF((float)(pRect->right - pRect->left), (float)(pRect->bottom - pRect->top));
    } else {
        state.size = CD2DSizeF(0.0f, 0.0f);
    }
    return TRUE;
}

int CDCRenderTarget::BindDC(const CDC& dc, const CRect& rect) {
    return impl__BindDC_CDCRenderTarget__QEAAHAEBVCDC__AEBVCRect___Z(this, &dc, &rect);
}

// Symbol: ??0CAnimationVariable@@QEAA@N@Z
extern "C" CAnimationVariable* MS_ABI impl___0CAnimationVariable__QEAA_N_Z(CAnimationVariable* pThis, double defaultValue) {
    if (!pThis) return nullptr;
    std::lock_guard<std::mutex> lock(g_wave2StateMutex);
    g_animationVariableState[pThis].value = defaultValue;
    return pThis;
}

// Symbol: ??1CAnimationVariable@@UEAA@XZ
extern "C" void MS_ABI impl___1CAnimationVariable__UEAA_XZ(CAnimationVariable* pThis) {
    if (!pThis) return;
    std::lock_guard<std::mutex> lock(g_wave2StateMutex);
    g_animationVariableState.erase(pThis);
}

CAnimationVariable::CAnimationVariable(double defaultValue) {
    impl___0CAnimationVariable__QEAA_N_Z(this, defaultValue);
    memset(_animationvariable_padding, 0, sizeof(_animationvariable_padding));
}

CAnimationVariable::~CAnimationVariable() {
    impl___1CAnimationVariable__UEAA_XZ(this);
}

// Symbol: ?Create@CAnimationVariable@@UEAAHPEAUIUIAnimationManager@@@Z
extern "C" int MS_ABI impl__Create_CAnimationVariable__UEAAHPEAUIUIAnimationManager___Z(CAnimationVariable* pThis, void* pAnimationManager) {
    (void)pAnimationManager;
    return pThis ? TRUE : FALSE;
}

int CAnimationVariable::Create(void* pAnimationManager) {
    return impl__Create_CAnimationVariable__UEAAHPEAUIUIAnimationManager___Z(this, pAnimationManager);
}

// Symbol: ?CreateTransitions@CAnimationVariable@@QEAAHPEAUIUIAnimationTransitionLibrary@@PEAUIUIAnimationTransitionFactory@@@Z
extern "C" int MS_ABI impl__CreateTransitions_CAnimationVariable__QEAAHPEAUIUIAnimationTransitionLibrary__PEAUIUIAnimationTransitionFactory___Z(
    CAnimationVariable* pThis, void* pTransitionLibrary, void* pTransitionFactory) {
    if (!pThis || !pTransitionLibrary || !pTransitionFactory) return FALSE;
    return TRUE;
}

int CAnimationVariable::CreateTransitions(void* pTransitionLibrary, void* pTransitionFactory) {
    return impl__CreateTransitions_CAnimationVariable__QEAAHPEAUIUIAnimationTransitionLibrary__PEAUIUIAnimationTransitionFactory___Z(
        this, pTransitionLibrary, pTransitionFactory);
}

// Symbol: ?AddTransition@CAnimationVariable@@QEAAXPEAVCBaseTransition@@@Z
extern "C" void MS_ABI impl__AddTransition_CAnimationVariable__QEAAXPEAVCBaseTransition___Z(CAnimationVariable* pThis, void* pTransition) {
    (void)pThis;
    (void)pTransition;
}

void CAnimationVariable::AddTransition(void* pTransition) {
    impl__AddTransition_CAnimationVariable__QEAAXPEAVCBaseTransition___Z(this, pTransition);
}

// Symbol: ?ClearTransitions@CAnimationVariable@@QEAAXH@Z
extern "C" void MS_ABI impl__ClearTransitions_CAnimationVariable__QEAAXH_Z(CAnimationVariable* pThis, int bAutodestroy) {
    (void)pThis;
    (void)bAutodestroy;
}

void CAnimationVariable::ClearTransitions(int bAutodestroy) {
    impl__ClearTransitions_CAnimationVariable__QEAAXH_Z(this, bAutodestroy);
}

// Symbol: ?EnableValueChangedEvent@CAnimationVariable@@QEAAXPEAVCAnimationController@@H@Z
extern "C" void MS_ABI impl__EnableValueChangedEvent_CAnimationVariable__QEAAXPEAVCAnimationController__H_Z(
    CAnimationVariable* pThis, void* pController, int bEnable) {
    (void)pThis;
    (void)pController;
    (void)bEnable;
}

void CAnimationVariable::EnableValueChangedEvent(void* pController, int bEnable) {
    impl__EnableValueChangedEvent_CAnimationVariable__QEAAXPEAVCAnimationController__H_Z(this, pController, bEnable);
}

// Symbol: ?EnableIntegerValueChangedEvent@CAnimationVariable@@QEAAXPEAVCAnimationController@@H@Z
extern "C" void MS_ABI impl__EnableIntegerValueChangedEvent_CAnimationVariable__QEAAXPEAVCAnimationController__H_Z(
    CAnimationVariable* pThis, void* pController, int bEnable) {
    (void)pThis;
    (void)pController;
    (void)bEnable;
}

void CAnimationVariable::EnableIntegerValueChangedEvent(void* pController, int bEnable) {
    impl__EnableIntegerValueChangedEvent_CAnimationVariable__QEAAXPEAVCAnimationController__H_Z(this, pController, bEnable);
}

// Symbol: ?ApplyTransitions@CAnimationVariable@@QEAAXPEAVCAnimationController@@PEAUIUIAnimationStoryboard@@H@Z
extern "C" void MS_ABI impl__ApplyTransitions_CAnimationVariable__QEAAXPEAVCAnimationController__PEAUIUIAnimationStoryboard__H_Z(
    CAnimationVariable* pThis, void* pController, void* pStoryboard, int bAutodestroy) {
    (void)pThis;
    (void)pController;
    (void)pStoryboard;
    (void)bAutodestroy;
}

void CAnimationVariable::ApplyTransitions(void* pController, void* pStoryboard, int bAutodestroy) {
    impl__ApplyTransitions_CAnimationVariable__QEAAXPEAVCAnimationController__PEAUIUIAnimationStoryboard__H_Z(
        this, pController, pStoryboard, bAutodestroy);
}

// Symbol: ?SetDefaultValue@CAnimationVariable@@QEAAXN@Z
extern "C" void MS_ABI impl__SetDefaultValue_CAnimationVariable__QEAAXN_Z(CAnimationVariable* pThis, double value) {
    if (!pThis) return;
    std::lock_guard<std::mutex> lock(g_wave2StateMutex);
    g_animationVariableState[pThis].value = value;
}

void CAnimationVariable::SetDefaultValue(double value) {
    impl__SetDefaultValue_CAnimationVariable__QEAAXN_Z(this, value);
}

// Symbol: ?GetValue@CAnimationVariable@@QEAAJAEAN@Z
extern "C" long MS_ABI impl__GetValue_CAnimationVariable__QEAAJAEAN_Z(CAnimationVariable* pThis, double* pValue) {
    if (!pThis || !pValue) return E_POINTER;
    std::lock_guard<std::mutex> lock(g_wave2StateMutex);
    *pValue = g_animationVariableState[pThis].value;
    return S_OK;
}

long CAnimationVariable::GetValue(double& value) {
    return impl__GetValue_CAnimationVariable__QEAAJAEAN_Z(this, &value);
}

// Symbol: ?GetValue@CAnimationVariable@@QEAAJAEAH@Z
extern "C" long MS_ABI impl__GetValue_CAnimationVariable__QEAAJAEAH_Z(CAnimationVariable* pThis, int* pValue) {
    if (!pThis || !pValue) return E_POINTER;
    std::lock_guard<std::mutex> lock(g_wave2StateMutex);
    *pValue = (int)g_animationVariableState[pThis].value;
    return S_OK;
}

long CAnimationVariable::GetValue(int& value) {
    return impl__GetValue_CAnimationVariable__QEAAJAEAH_Z(this, &value);
}

// =============================================================================
// CDC Wave 3: Missing CDC methods
// =============================================================================

// CDC::SaveDC (virtual override, UEAA)
// Symbol: ?SaveDC@CDC@@UEAAHXZ
extern "C" int MS_ABI impl__SaveDC_CDC__UEAAHXZ(CDC* pThis) {
    return impl__SaveDC_CDC__QEAAHXZ(pThis);
}

// CDC::RestoreDC (virtual override, UEAA)
// Symbol: ?RestoreDC@CDC@@UEAAHH@Z
extern "C" int MS_ABI impl__RestoreDC_CDC__UEAAHH_Z(CDC* pThis, int nSavedDC) {
    return impl__RestoreDC_CDC__QEAAHH_Z(pThis, nSavedDC);
}

// CDC::SetBkColor (virtual override, UEAA)
// Symbol: ?SetBkColor@CDC@@UEAAKK@Z
extern "C" unsigned long MS_ABI impl__SetBkColor_CDC__UEAAKK_Z(CDC* pThis, unsigned long crColor) {
    return impl__SetBkColor_CDC__QEAAKK_Z(pThis, crColor);
}

// CDC::SetTextColor (virtual override, UEAA)
// Symbol: ?SetTextColor@CDC@@UEAAKK@Z
extern "C" unsigned long MS_ABI impl__SetTextColor_CDC__UEAAKK_Z(CDC* pThis, unsigned long crColor) {
    return impl__SetTextColor_CDC__QEAAKK_Z(pThis, crColor);
}

// CDC::SetMapMode (virtual, UEAA)
// Symbol: ?SetMapMode@CDC@@UEAAHH@Z
extern "C" int MS_ABI impl__SetMapMode_CDC__UEAAHH_Z(CDC* pThis, int nMapMode) {
    if (!pThis || !pThis->m_hDC) return 0;
    return ::SetMapMode(pThis->m_hDC, nMapMode);
}

// CDC::SetViewportOrg (virtual, UEAA)
// Symbol: ?SetViewportOrg@CDC@@UEAA?AVCPoint@@HH@Z
extern "C" CPoint MS_ABI impl__SetViewportOrg_CDC__UEAA_AVCPoint__HH_Z(CDC* pThis, int x, int y) {
    CPoint pt(0, 0);
    if (!pThis || !pThis->m_hDC) return pt;
    POINT old;
    ::GetViewportOrgEx(pThis->m_hDC, &old);
    ::SetViewportOrgEx(pThis->m_hDC, x, y, nullptr);
    pt.x = old.x;
    pt.y = old.y;
    return pt;
}

// CDC::SetViewportExt (virtual, UEAA)
// Symbol: ?SetViewportExt@CDC@@UEAA?AVCSize@@HH@Z
extern "C" CSize MS_ABI impl__SetViewportExt_CDC__UEAA_AVCSize__HH_Z(CDC* pThis, int cx, int cy) {
    CSize sz(0, 0);
    if (!pThis || !pThis->m_hDC) return sz;
    SIZE old;
    ::GetViewportExtEx(pThis->m_hDC, &old);
    ::SetViewportExtEx(pThis->m_hDC, cx, cy, nullptr);
    sz.cx = old.cx;
    sz.cy = old.cy;
    return sz;
}

// CDC::ScaleViewportExt (virtual, UEAA)
// Symbol: ?ScaleViewportExt@CDC@@UEAA?AVCSize@@HHHH@Z
extern "C" CSize MS_ABI impl__ScaleViewportExt_CDC__UEAA_AVCSize__HHHH_Z(
    CDC* pThis, int xNum, int xDenom, int yNum, int yDenom) {
    CSize sz(0, 0);
    if (!pThis || !pThis->m_hDC) return sz;
    SIZE old;
    ::GetViewportExtEx(pThis->m_hDC, &old);
    ::ScaleViewportExtEx(pThis->m_hDC, xNum, xDenom, yNum, yDenom, nullptr);
    sz.cx = old.cx;
    sz.cy = old.cy;
    return sz;
}

// CDC::SetWindowExt (virtual, UEAA)
// Symbol: ?SetWindowExt@CDC@@UEAA?AVCSize@@HH@Z
extern "C" CSize MS_ABI impl__SetWindowExt_CDC__UEAA_AVCSize__HH_Z(CDC* pThis, int cx, int cy) {
    CSize sz(0, 0);
    if (!pThis || !pThis->m_hDC) return sz;
    SIZE old;
    ::GetWindowExtEx(pThis->m_hDC, &old);
    ::SetWindowExtEx(pThis->m_hDC, cx, cy, nullptr);
    sz.cx = old.cx;
    sz.cy = old.cy;
    return sz;
}

// CDC::SetWindowOrg
// Symbol: ?SetWindowOrg@CDC@@QEAA?AVCPoint@@HH@Z
extern "C" CPoint MS_ABI impl__SetWindowOrg_CDC__QEAA_AVCPoint__HH_Z(CDC* pThis, int x, int y) {
    CPoint pt(0, 0);
    if (!pThis || !pThis->m_hDC) return pt;
    POINT old;
    ::GetWindowOrgEx(pThis->m_hDC, &old);
    ::SetWindowOrgEx(pThis->m_hDC, x, y, nullptr);
    pt.x = old.x;
    pt.y = old.y;
    return pt;
}

// CDC::ScaleWindowExt (virtual, UEAA)
// Symbol: ?ScaleWindowExt@CDC@@UEAA?AVCSize@@HHHH@Z
extern "C" CSize MS_ABI impl__ScaleWindowExt_CDC__UEAA_AVCSize__HHHH_Z(
    CDC* pThis, int xNum, int xDenom, int yNum, int yDenom) {
    CSize sz(0, 0);
    if (!pThis || !pThis->m_hDC) return sz;
    SIZE old;
    ::GetWindowExtEx(pThis->m_hDC, &old);
    ::ScaleWindowExtEx(pThis->m_hDC, xNum, xDenom, yNum, yDenom, nullptr);
    sz.cx = old.cx;
    sz.cy = old.cy;
    return sz;
}

// CDC::OffsetViewportOrg (virtual, UEAA)
// Symbol: ?OffsetViewportOrg@CDC@@UEAA?AVCPoint@@HH@Z
extern "C" CPoint MS_ABI impl__OffsetViewportOrg_CDC__UEAA_AVCPoint__HH_Z(CDC* pThis, int nWidth, int nHeight) {
    CPoint pt(0, 0);
    if (!pThis || !pThis->m_hDC) return pt;
    POINT old;
    ::GetViewportOrgEx(pThis->m_hDC, &old);
    ::OffsetViewportOrgEx(pThis->m_hDC, nWidth, nHeight, nullptr);
    pt.x = old.x;
    pt.y = old.y;
    return pt;
}

// CDC::OffsetWindowOrg
// Symbol: ?OffsetWindowOrg@CDC@@QEAA?AVCPoint@@HH@Z
extern "C" CPoint MS_ABI impl__OffsetWindowOrg_CDC__QEAA_AVCPoint__HH_Z(CDC* pThis, int nWidth, int nHeight) {
    CPoint pt(0, 0);
    if (!pThis || !pThis->m_hDC) return pt;
    POINT old;
    ::GetWindowOrgEx(pThis->m_hDC, &old);
    ::OffsetWindowOrgEx(pThis->m_hDC, nWidth, nHeight, nullptr);
    pt.x = old.x;
    pt.y = old.y;
    return pt;
}

// CDC::SetPolyFillMode
// Symbol: ?SetPolyFillMode@CDC@@QEAAHH@Z
extern "C" int MS_ABI impl__SetPolyFillMode_CDC__QEAAHH_Z(CDC* pThis, int nPolyFillMode) {
    if (!pThis || !pThis->m_hDC) return 0;
    return ::SetPolyFillMode(pThis->m_hDC, nPolyFillMode);
}

// CDC::SetROP2
// Symbol: ?SetROP2@CDC@@QEAAHH@Z
extern "C" int MS_ABI impl__SetROP2_CDC__QEAAHH_Z(CDC* pThis, int nDrawMode) {
    if (!pThis || !pThis->m_hDC) return 0;
    return ::SetROP2(pThis->m_hDC, nDrawMode);
}

// CDC::SetStretchBltMode
// Symbol: ?SetStretchBltMode@CDC@@QEAAHH@Z
extern "C" int MS_ABI impl__SetStretchBltMode_CDC__QEAAHH_Z(CDC* pThis, int nStretchMode) {
    if (!pThis || !pThis->m_hDC) return 0;
    return ::SetStretchBltMode(pThis->m_hDC, nStretchMode);
}

// CDC::SetTextAlign
// Symbol: ?SetTextAlign@CDC@@QEAAII@Z
extern "C" unsigned int MS_ABI impl__SetTextAlign_CDC__QEAAII_Z(CDC* pThis, unsigned int nFlags) {
    if (!pThis || !pThis->m_hDC) return GDI_ERROR;
    return ::SetTextAlign(pThis->m_hDC, nFlags);
}

// CDC::SetTextCharacterExtra
// Symbol: ?SetTextCharacterExtra@CDC@@QEAAHH@Z
extern "C" int MS_ABI impl__SetTextCharacterExtra_CDC__QEAAHH_Z(CDC* pThis, int nCharExtra) {
    if (!pThis || !pThis->m_hDC) return 0x80000000;
    return ::SetTextCharacterExtra(pThis->m_hDC, nCharExtra);
}

// CDC::SetTextJustification
// Symbol: ?SetTextJustification@CDC@@QEAAHHH@Z
extern "C" int MS_ABI impl__SetTextJustification_CDC__QEAAHHH_Z(CDC* pThis, int nBreakExtra, int nBreakCount) {
    if (!pThis || !pThis->m_hDC) return FALSE;
    return ::SetTextJustification(pThis->m_hDC, nBreakExtra, nBreakCount);
}

// CDC::SetArcDirection
// Symbol: ?SetArcDirection@CDC@@QEAAHH@Z
extern "C" int MS_ABI impl__SetArcDirection_CDC__QEAAHH_Z(CDC* pThis, int nArcDirection) {
    if (!pThis || !pThis->m_hDC) return 0;
    return ::SetArcDirection(pThis->m_hDC, nArcDirection);
}

// CDC::SetGraphicsMode
// Symbol: ?SetGraphicsMode@CDC@@QEAAHH@Z
extern "C" int MS_ABI impl__SetGraphicsMode_CDC__QEAAHH_Z(CDC* pThis, int nGraphicsMode) {
    if (!pThis || !pThis->m_hDC) return 0;
    return ::SetGraphicsMode(pThis->m_hDC, nGraphicsMode);
}

// CDC::GetLayout
// Symbol: ?GetLayout@CDC@@QEBAKXZ
extern "C" unsigned long MS_ABI impl__GetLayout_CDC__QEBAKXZ(const CDC* pThis) {
    if (!pThis || !pThis->m_hDC) return GDI_ERROR;
    return ::GetLayout(pThis->m_hDC);
}

// CDC::SetLayout
// Symbol: ?SetLayout@CDC@@QEAAKK@Z
extern "C" unsigned long MS_ABI impl__SetLayout_CDC__QEAAKK_Z(CDC* pThis, unsigned long dwLayout) {
    if (!pThis || !pThis->m_hDC) return GDI_ERROR;
    return ::SetLayout(pThis->m_hDC, dwLayout);
}

// CDC::SetColorAdjustment
// Symbol: ?SetColorAdjustment@CDC@@QEAAHPEBUtagCOLORADJUSTMENT@@@Z
extern "C" int MS_ABI impl__SetColorAdjustment_CDC__QEAAHPEBUtagCOLORADJUSTMENT___Z(
    CDC* pThis, const COLORADJUSTMENT* lpColorAdjust) {
    if (!pThis || !pThis->m_hDC || !lpColorAdjust) return FALSE;
    return ::SetColorAdjustment(pThis->m_hDC, lpColorAdjust);
}

// CDC::SetWorldTransform
// Symbol: ?SetWorldTransform@CDC@@QEAAHPEBUtagXFORM@@@Z
extern "C" int MS_ABI impl__SetWorldTransform_CDC__QEAAHPEBUtagXFORM___Z(CDC* pThis, const XFORM* pXForm) {
    if (!pThis || !pThis->m_hDC || !pXForm) return FALSE;
    return ::SetWorldTransform(pThis->m_hDC, pXForm);
}

// CDC::ModifyWorldTransform
// Symbol: ?ModifyWorldTransform@CDC@@QEAAHPEBUtagXFORM@@K@Z
extern "C" int MS_ABI impl__ModifyWorldTransform_CDC__QEAAHPEBUtagXFORM__K_Z(
    CDC* pThis, const XFORM* pXForm, unsigned long iMode) {
    if (!pThis || !pThis->m_hDC || !pXForm) return FALSE;
    return ::ModifyWorldTransform(pThis->m_hDC, pXForm, iMode);
}

// CDC::SetMapperFlags
// Symbol: ?SetMapperFlags@CDC@@QEAAKK@Z
extern "C" unsigned long MS_ABI impl__SetMapperFlags_CDC__QEAAKK_Z(CDC* pThis, unsigned long dwFlag) {
    if (!pThis || !pThis->m_hDC) return GDI_ERROR;
    return ::SetMapperFlags(pThis->m_hDC, dwFlag);
}

// CDC::GetClipBox (virtual, UEBA)
// Symbol: ?GetClipBox@CDC@@UEBAHPEAUtagRECT@@@Z
extern "C" int MS_ABI impl__GetClipBox_CDC__UEBAHPEAUtagRECT___Z(const CDC* pThis, RECT* lpRect) {
    if (!pThis || !pThis->m_hDC || !lpRect) return ERROR;
    return ::GetClipBox(pThis->m_hDC, lpRect);
}

// CDC::SelectObject(CPen*)
// Symbol: ?SelectObject@CDC@@QEAAPEAVCPen@@PEAV2@@Z
extern "C" CPen* MS_ABI impl__SelectObject_CDC__QEAAPEAVCPen__PEAV2__Z(CDC* pThis, CPen* pPen) {
    if (!pThis || !pThis->m_hDC || !pPen) return nullptr;
    HGDIOBJ hOld = ::SelectObject(pThis->m_hDC, pPen->GetSafeHandle());
    if (!hOld) return nullptr;
    // Return a temporary wrapper for the old object
    CGdiObject* pOld = GetTempGdiObject(hOld);
    return (CPen*)pOld;
}

// CDC::SelectObject(CBrush*)
// Symbol: ?SelectObject@CDC@@QEAAPEAVCBrush@@PEAV2@@Z
extern "C" CBrush* MS_ABI impl__SelectObject_CDC__QEAAPEAVCBrush__PEAV2__Z(CDC* pThis, CBrush* pBrush) {
    if (!pThis || !pThis->m_hDC || !pBrush) return nullptr;
    HGDIOBJ hOld = ::SelectObject(pThis->m_hDC, pBrush->GetSafeHandle());
    if (!hOld) return nullptr;
    CGdiObject* pOld = GetTempGdiObject(hOld);
    return (CBrush*)pOld;
}

// CDC::SelectObject(CFont*) (virtual, UEAA)
// Symbol: ?SelectObject@CDC@@UEAAPEAVCFont@@PEAV2@@Z
extern "C" CFont* MS_ABI impl__SelectObject_CDC__UEAAPEAVCFont__PEAV2__Z(CDC* pThis, CFont* pFont) {
    if (!pThis || !pThis->m_hDC || !pFont) return nullptr;
    HGDIOBJ hOld = ::SelectObject(pThis->m_hDC, pFont->GetSafeHandle());
    if (!hOld) return nullptr;
    CGdiObject* pOld = GetTempGdiObject(hOld);
    return (CFont*)pOld;
}

// CDC::SelectObject(CRgn*) - returns int
// Symbol: ?SelectObject@CDC@@QEAAHPEAVCRgn@@@Z
extern "C" int MS_ABI impl__SelectObject_CDC__QEAAHPEAVCRgn___Z(CDC* pThis, CRgn* pRgn) {
    if (!pThis || !pThis->m_hDC || !pRgn) return ERROR;
    HRGN hRgn = (HRGN)pRgn->GetSafeHandle();
    return ::SelectClipRgn(pThis->m_hDC, hRgn);
}

// CDC::SelectPalette
// Symbol: ?SelectPalette@CDC@@QEAAPEAVCPalette@@PEAV2@H@Z
extern "C" CPalette* MS_ABI impl__SelectPalette_CDC__QEAAPEAVCPalette__PEAV2_H_Z(
    CDC* pThis, CPalette* pPalette, int bForceBackground) {
    if (!pThis || !pThis->m_hDC || !pPalette) return nullptr;
    HPALETTE hOld = ::SelectPalette(pThis->m_hDC,
                                    (HPALETTE)pPalette->GetSafeHandle(),
                                    bForceBackground);
    if (!hOld) return nullptr;
    // Wrap in a temporary CGdiObject cast to CPalette*
    CGdiObject* pOld = GetTempGdiObject((HGDIOBJ)hOld);
    return (CPalette*)pOld;
}

// CDC::SelectStockObject (virtual, UEAA)
// Symbol: ?SelectStockObject@CDC@@UEAAPEAVCGdiObject@@H@Z
extern "C" CGdiObject* MS_ABI impl__SelectStockObject_CDC__UEAAPEAVCGdiObject__H_Z(CDC* pThis, int nIndex) {
    if (!pThis || !pThis->m_hDC) return nullptr;
    HGDIOBJ hNew = ::GetStockObject(nIndex);
    if (!hNew) return nullptr;
    HGDIOBJ hOld = ::SelectObject(pThis->m_hDC, hNew);
    return GetTempGdiObject(hOld);
}

// CDC::SelectGdiObject (static, KA qualified)
// Symbol: ?SelectGdiObject@CDC@@KAPEAVCGdiObject@@PEAUHDC__@@PEAX@Z
extern "C" CGdiObject* MS_ABI impl__SelectGdiObject_CDC__KAPEAVCGdiObject__PEAUHDC____PEAX_Z(
    HDC hDC, HGDIOBJ hObj) {
    if (!hDC || !hObj) return nullptr;
    HGDIOBJ hOld = ::SelectObject(hDC, hObj);
    return GetTempGdiObject(hOld);
}

// CDC::SelectClipPath
// Symbol: ?SelectClipPath@CDC@@QEAAHH@Z
extern "C" int MS_ABI impl__SelectClipPath_CDC__QEAAHH_Z(CDC* pThis, int nMode) {
    if (!pThis || !pThis->m_hDC) return FALSE;
    return ::SelectClipPath(pThis->m_hDC, nMode);
}

// CDC::SetAttribDC (virtual, UEAA)
// Symbol: ?SetAttribDC@CDC@@UEAAXPEAUHDC__@@@Z
extern "C" void MS_ABI impl__SetAttribDC_CDC__UEAAXPEAUHDC_____Z(CDC* pThis, HDC hDC) {
    if (!pThis) return;
    pThis->m_hAttribDC = hDC;
}

// CDC::SetOutputDC (virtual, UEAA)
// Symbol: ?SetOutputDC@CDC@@UEAAXPEAUHDC__@@@Z
extern "C" void MS_ABI impl__SetOutputDC_CDC__UEAAXPEAUHDC_____Z(CDC* pThis, HDC hDC) {
    if (!pThis) return;
    pThis->m_hDC = hDC;
}

// CDC::ReleaseAttribDC (virtual, UEAA)
// Symbol: ?ReleaseAttribDC@CDC@@UEAAXXZ
extern "C" void MS_ABI impl__ReleaseAttribDC_CDC__UEAAXXZ(CDC* pThis) {
    if (!pThis) return;
    pThis->m_hAttribDC = nullptr;
}

// CDC::ReleaseOutputDC (virtual, UEAA)
// Symbol: ?ReleaseOutputDC@CDC@@UEAAXXZ
extern "C" void MS_ABI impl__ReleaseOutputDC_CDC__UEAAXXZ(CDC* pThis) {
    if (!pThis) return;
    pThis->m_hDC = nullptr;
}

// CDC::ArcTo
// Symbol: ?ArcTo@CDC@@QEAAHHHHHHHHH@Z
extern "C" int MS_ABI impl__ArcTo_CDC__QEAAHHHHHHHHH_Z(
    CDC* pThis, int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4) {
    if (!pThis || !pThis->m_hDC) return FALSE;
    return ::ArcTo(pThis->m_hDC, x1, y1, x2, y2, x3, y3, x4, y4);
}

// CDC::PolylineTo
// Symbol: ?PolylineTo@CDC@@QEAAHPEBUtagPOINT@@H@Z
extern "C" int MS_ABI impl__PolylineTo_CDC__QEAAHPEBUtagPOINT__H_Z(
    CDC* pThis, const POINT* lpPoints, int nCount) {
    if (!pThis || !pThis->m_hDC || !lpPoints) return FALSE;
    return ::PolylineTo(pThis->m_hDC, lpPoints, (DWORD)nCount);
}

// CDC::PolyBezierTo
// Symbol: ?PolyBezierTo@CDC@@QEAAHPEBUtagPOINT@@H@Z
extern "C" int MS_ABI impl__PolyBezierTo_CDC__QEAAHPEBUtagPOINT__H_Z(
    CDC* pThis, const POINT* lpPoints, int nCount) {
    if (!pThis || !pThis->m_hDC || !lpPoints) return FALSE;
    return ::PolyBezierTo(pThis->m_hDC, lpPoints, (DWORD)nCount);
}

// CDC::PolyDraw
// Symbol: ?PolyDraw@CDC@@QEAAHPEBUtagPOINT@@PEBEH@Z
extern "C" int MS_ABI impl__PolyDraw_CDC__QEAAHPEBUtagPOINT__PEBEH_Z(
    CDC* pThis, const POINT* lpPoints, const BYTE* lpTypes, int nCount) {
    if (!pThis || !pThis->m_hDC || !lpPoints) return FALSE;
    return ::PolyDraw(pThis->m_hDC, lpPoints, lpTypes, nCount);
}

// CDC::PlayMetaFile
// Symbol: ?PlayMetaFile@CDC@@QEAAHPEAUHMETAFILE__@@@Z
extern "C" int MS_ABI impl__PlayMetaFile_CDC__QEAAHPEAUHMETAFILE_____Z(CDC* pThis, HMETAFILE hMF) {
    if (!pThis || !pThis->m_hDC || !hMF) return FALSE;
    return ::PlayMetaFile(pThis->m_hDC, hMF);
}

// CDC::StartDocW
// Symbol: ?StartDocW@CDC@@QEAAHPEB_W@Z
extern "C" int MS_ABI impl__StartDocW_CDC__QEAAHPEB_W_Z(CDC* pThis, const wchar_t* lpszDocName) {
    if (!pThis || !pThis->m_hDC) return SP_ERROR;
    DOCINFOW di = {};
    di.cbSize = sizeof(di);
    di.lpszDocName = lpszDocName;
    return ::StartDocW(pThis->m_hDC, &di);
}

// CDC::Draw3dRect (int coords)
// Symbol: ?Draw3dRect@CDC@@QEAAXHHHHKK@Z
extern "C" void MS_ABI impl__Draw3dRect_CDC__QEAAXHHHHKK_Z(
    CDC* pThis, int x, int y, int cx, int cy,
    unsigned long clrTopLeft, unsigned long clrBottomRight) {
    if (!pThis || !pThis->m_hDC) return;
    RECT rc = {x, y, x + cx, y + cy};
    // Top and left edges
    RECT rcEdge = {rc.left, rc.top, rc.right - 1, rc.top + 1};
    ::SetBkColor(pThis->m_hDC, clrTopLeft);
    ::ExtTextOutW(pThis->m_hDC, 0, 0, ETO_OPAQUE, &rcEdge, nullptr, 0, nullptr);
    rcEdge = {rc.left, rc.top, rc.left + 1, rc.bottom - 1};
    ::ExtTextOutW(pThis->m_hDC, 0, 0, ETO_OPAQUE, &rcEdge, nullptr, 0, nullptr);
    // Bottom and right edges
    rcEdge = {rc.left, rc.bottom - 1, rc.right, rc.bottom};
    ::SetBkColor(pThis->m_hDC, clrBottomRight);
    ::ExtTextOutW(pThis->m_hDC, 0, 0, ETO_OPAQUE, &rcEdge, nullptr, 0, nullptr);
    rcEdge = {rc.right - 1, rc.top, rc.right, rc.bottom};
    ::ExtTextOutW(pThis->m_hDC, 0, 0, ETO_OPAQUE, &rcEdge, nullptr, 0, nullptr);
}

// CDC::Draw3dRect (RECT)
// Symbol: ?Draw3dRect@CDC@@QEAAXPEBUtagRECT@@KK@Z
extern "C" void MS_ABI impl__Draw3dRect_CDC__QEAAXPEBUtagRECT__KK_Z(
    CDC* pThis, const RECT* lpRect,
    unsigned long clrTopLeft, unsigned long clrBottomRight) {
    if (!lpRect) return;
    impl__Draw3dRect_CDC__QEAAXHHHHKK_Z(
        pThis,
        lpRect->left, lpRect->top,
        lpRect->right - lpRect->left,
        lpRect->bottom - lpRect->top,
        clrTopLeft, clrBottomRight);
}

// CDC::DrawDragRect
// Symbol: ?DrawDragRect@CDC@@QEAAXPEBUtagRECT@@UtagSIZE@@01PEAVCBrush@@2@Z
extern "C" void MS_ABI impl__DrawDragRect_CDC__QEAAXPEBUtagRECT__UtagSIZE__01PEAVCBrush__2_Z(
    CDC* pThis, const RECT* lpRect, SIZE size,
    const RECT* lpRectLast, SIZE sizeLast,
    CBrush* pBrush, CBrush* pBrushLast) {
    if (!pThis || !pThis->m_hDC) return;
    // Erase last rect by drawing with pBrushLast
    if (lpRectLast) {
        HBRUSH hBrush = pBrushLast ? (HBRUSH)pBrushLast->GetSafeHandle()
                                   : (HBRUSH)::GetStockObject(GRAY_BRUSH);
        // Draw 4 border rects for the "last" area
        RECT rc;
        rc = {lpRectLast->left, lpRectLast->top, lpRectLast->right, lpRectLast->top + sizeLast.cy};
        ::FillRect(pThis->m_hDC, &rc, hBrush);
        rc = {lpRectLast->left, lpRectLast->bottom - sizeLast.cy, lpRectLast->right, lpRectLast->bottom};
        ::FillRect(pThis->m_hDC, &rc, hBrush);
        rc = {lpRectLast->left, lpRectLast->top, lpRectLast->left + sizeLast.cx, lpRectLast->bottom};
        ::FillRect(pThis->m_hDC, &rc, hBrush);
        rc = {lpRectLast->right - sizeLast.cx, lpRectLast->top, lpRectLast->right, lpRectLast->bottom};
        ::FillRect(pThis->m_hDC, &rc, hBrush);
    }
    // Draw current rect
    if (lpRect) {
        HBRUSH hBrush = pBrush ? (HBRUSH)pBrush->GetSafeHandle()
                               : (HBRUSH)::GetStockObject(BLACK_BRUSH);
        RECT rc;
        rc = {lpRect->left, lpRect->top, lpRect->right, lpRect->top + size.cy};
        ::FillRect(pThis->m_hDC, &rc, hBrush);
        rc = {lpRect->left, lpRect->bottom - size.cy, lpRect->right, lpRect->bottom};
        ::FillRect(pThis->m_hDC, &rc, hBrush);
        rc = {lpRect->left, lpRect->top, lpRect->left + size.cx, lpRect->bottom};
        ::FillRect(pThis->m_hDC, &rc, hBrush);
        rc = {lpRect->right - size.cx, lpRect->top, lpRect->right, lpRect->bottom};
        ::FillRect(pThis->m_hDC, &rc, hBrush);
    }
}

// CDC::GetHalftoneBrush (static)
// Symbol: ?GetHalftoneBrush@CDC@@SAPEAVCBrush@@XZ
extern "C" CBrush* MS_ABI impl__GetHalftoneBrush_CDC__SAPEAVCBrush__XZ() {
    HGDIOBJ hBrush = ::GetStockObject(DKGRAY_BRUSH);
    return (CBrush*)GetTempGdiObject(hBrush);
}

// CDC::DPtoHIMETRIC
// Symbol: ?DPtoHIMETRIC@CDC@@QEBAXPEAUtagSIZE@@@Z
extern "C" void MS_ABI impl__DPtoHIMETRIC_CDC__QEBAXPEAUtagSIZE___Z(const CDC* pThis, SIZE* lpSize) {
    if (!pThis || !pThis->m_hDC || !lpSize) return;
    HDC hDC = pThis->m_hDC;
    lpSize->cx = MulDiv(lpSize->cx, 2540, ::GetDeviceCaps(hDC, LOGPIXELSX));
    lpSize->cy = MulDiv(lpSize->cy, 2540, ::GetDeviceCaps(hDC, LOGPIXELSY));
}

// CDC::LPtoHIMETRIC
// Symbol: ?LPtoHIMETRIC@CDC@@QEBAXPEAUtagSIZE@@@Z
extern "C" void MS_ABI impl__LPtoHIMETRIC_CDC__QEBAXPEAUtagSIZE___Z(const CDC* pThis, SIZE* lpSize) {
    if (!pThis || !pThis->m_hDC || !lpSize) return;
    // First convert LP to DP, then DP to HIMETRIC
    POINT pt = {lpSize->cx, lpSize->cy};
    ::LPtoDP(pThis->m_hDC, &pt, 1);
    SIZE sz = {pt.x, pt.y};
    impl__DPtoHIMETRIC_CDC__QEBAXPEAUtagSIZE___Z(pThis, &sz);
    lpSize->cx = sz.cx;
    lpSize->cy = sz.cy;
}

// CDC::HIMETRICtoDP
// Symbol: ?HIMETRICtoDP@CDC@@QEBAXPEAUtagSIZE@@@Z
extern "C" void MS_ABI impl__HIMETRICtoDP_CDC__QEBAXPEAUtagSIZE___Z(const CDC* pThis, SIZE* lpSize) {
    if (!pThis || !pThis->m_hDC || !lpSize) return;
    HDC hDC = pThis->m_hDC;
    lpSize->cx = MulDiv(lpSize->cx, ::GetDeviceCaps(hDC, LOGPIXELSX), 2540);
    lpSize->cy = MulDiv(lpSize->cy, ::GetDeviceCaps(hDC, LOGPIXELSY), 2540);
}

// CDC::HIMETRICtoLP
// Symbol: ?HIMETRICtoLP@CDC@@QEBAXPEAUtagSIZE@@@Z
extern "C" void MS_ABI impl__HIMETRICtoLP_CDC__QEBAXPEAUtagSIZE___Z(const CDC* pThis, SIZE* lpSize) {
    if (!pThis || !pThis->m_hDC || !lpSize) return;
    SIZE sz = *lpSize;
    impl__HIMETRICtoDP_CDC__QEBAXPEAUtagSIZE___Z(pThis, &sz);
    POINT pt = {sz.cx, sz.cy};
    ::DPtoLP(pThis->m_hDC, &pt, 1);
    lpSize->cx = pt.x;
    lpSize->cy = pt.y;
}

// CDC::CreateObject (static) - runtime allocation
// Symbol: ?CreateObject@CDC@@SAPEAVCObject@@XZ
extern "C" CObject* MS_ABI impl__CreateObject_CDC__SAPEAVCObject__XZ() {
    return new CDC();
}

// =============================================================================
// CImageList Implementation
// =============================================================================

IMPLEMENT_DYNCREATE(CImageList, CObject)

#ifdef __GNUC__
asm(".globl \"?classCImageList@CImageList@@2UCRuntimeClass@@A\"\n"
    ".set \"?classCImageList@CImageList@@2UCRuntimeClass@@A\", _ZN10CImageList15classCImageListE\n");
#endif

namespace {
// Thread-local temporary CImageList map for FromHandle
thread_local std::unordered_map<HIMAGELIST, CImageList*> g_tempImageListMap;

CImageList* GetTempImageList(HIMAGELIST hImageList) {
    if (!hImageList) return nullptr;
    auto it = g_tempImageListMap.find(hImageList);
    if (it != g_tempImageListMap.end()) return it->second;
    auto* pIL = new CImageList();
    pIL->m_hImageList = hImageList;
    g_tempImageListMap.emplace(hImageList, pIL);
    return pIL;
}
} // namespace

// Symbol: ??0CImageList@@QEAA@XZ
extern "C" CImageList* MS_ABI impl___0CImageList__QEAA_XZ(CImageList* pThis) {
    if (!pThis) return nullptr;
    pThis->m_hImageList = nullptr;
    return pThis;
}

// Symbol: ??1CImageList@@UEAA@XZ
extern "C" void MS_ABI impl___1CImageList__UEAA_XZ(CImageList* pThis) {
    if (!pThis) return;
    if (pThis->m_hImageList) {
        ::ImageList_Destroy(pThis->m_hImageList);
        pThis->m_hImageList = nullptr;
    }
}

CImageList::CImageList() {
    impl___0CImageList__QEAA_XZ(this);
}

CImageList::~CImageList() {
    impl___1CImageList__UEAA_XZ(this);
}

// Symbol: ?Create@CImageList@@QEAAHHHIHH@Z
// Create(cx, cy, flags, cInitial, cGrow)
extern "C" int MS_ABI impl__Create_CImageList__QEAAHHHIHH_Z(
    CImageList* pThis, int cx, int cy, unsigned int nFlags, int nInitial, int nGrow) {
    if (!pThis) return FALSE;
    if (pThis->m_hImageList) {
        ::ImageList_Destroy(pThis->m_hImageList);
    }
    pThis->m_hImageList = ::ImageList_Create(cx, cy, nFlags, nInitial, nGrow);
    return pThis->m_hImageList != nullptr;
}

int CImageList::Create(int cx, int cy, UINT nFlags, int nInitial, int nGrow) {
    return impl__Create_CImageList__QEAAHHHIHH_Z(this, cx, cy, nFlags, nInitial, nGrow);
}

// Symbol: ?Create@CImageList@@QEAAHIHHK@Z
// Create(nBitmapID, cx, nGrow, crMask) - load from resource ID
extern "C" int MS_ABI impl__Create_CImageList__QEAAHIHHK_Z(
    CImageList* pThis, unsigned int nBitmapID, int cx, int nGrow, unsigned long crMask) {
    if (!pThis) return FALSE;
    if (pThis->m_hImageList) {
        ::ImageList_Destroy(pThis->m_hImageList);
    }
    HINSTANCE hInst = AfxGetInstanceHandle();
    pThis->m_hImageList = ::ImageList_LoadImageW(
        hInst, MAKEINTRESOURCEW(nBitmapID), cx, nGrow, crMask, IMAGE_BITMAP, LR_DEFAULTCOLOR);
    return pThis->m_hImageList != nullptr;
}

int CImageList::Create(UINT nBitmapID, int cx, int nGrow, COLORREF crMask) {
    return impl__Create_CImageList__QEAAHIHHK_Z(this, nBitmapID, cx, nGrow, crMask);
}

// Symbol: ?Create@CImageList@@QEAAHPEB_WHHK@Z
// Create(lpszBitmapID, cx, nGrow, crMask) - load from resource name
extern "C" int MS_ABI impl__Create_CImageList__QEAAHPEB_WHHK_Z(
    CImageList* pThis, const wchar_t* lpszBitmapID, int cx, int nGrow, unsigned long crMask) {
    if (!pThis || !lpszBitmapID) return FALSE;
    if (pThis->m_hImageList) {
        ::ImageList_Destroy(pThis->m_hImageList);
    }
    HINSTANCE hInst = AfxGetInstanceHandle();
    pThis->m_hImageList = ::ImageList_LoadImageW(
        hInst, lpszBitmapID, cx, nGrow, crMask, IMAGE_BITMAP, LR_DEFAULTCOLOR);
    return pThis->m_hImageList != nullptr;
}

int CImageList::Create(const wchar_t* lpszBitmapID, int cx, int nGrow, COLORREF crMask) {
    return impl__Create_CImageList__QEAAHPEB_WHHK_Z(this, lpszBitmapID, cx, nGrow, crMask);
}

// Symbol: ?Create@CImageList@@QEAAHPEAV1@@Z
// Create(pImageList) - copy constructor
extern "C" int MS_ABI impl__Create_CImageList__QEAAHPEAV1__Z(CImageList* pThis, CImageList* pImageList) {
    if (!pThis || !pImageList || !pImageList->m_hImageList) return FALSE;
    if (pThis->m_hImageList) {
        ::ImageList_Destroy(pThis->m_hImageList);
    }
    // Duplicate the image list
    pThis->m_hImageList = ::ImageList_Duplicate(pImageList->m_hImageList);
    return pThis->m_hImageList != nullptr;
}

int CImageList::Create(CImageList* pImageList) {
    return impl__Create_CImageList__QEAAHPEAV1__Z(this, pImageList);
}

// Symbol: ?Create@CImageList@@QEAAHAEAV1@H0HHH@Z
// Create(imagelist&, ...) - merge/create combined
extern "C" int MS_ABI impl__Create_CImageList__QEAAHAEAV1_H0HHH_Z(
    CImageList* pThis, CImageList* pImageList1, int nImage1, CImageList* pImageList2,
    int nImage2, int dx, int dy) {
    if (!pThis) return FALSE;
    if (pThis->m_hImageList) {
        ::ImageList_Destroy(pThis->m_hImageList);
        pThis->m_hImageList = nullptr;
    }
    HIMAGELIST h1 = pImageList1 ? pImageList1->m_hImageList : nullptr;
    HIMAGELIST h2 = pImageList2 ? pImageList2->m_hImageList : nullptr;
    if (!h1 && !h2) return FALSE;
    pThis->m_hImageList = ::ImageList_Merge(h1, nImage1, h2, nImage2, dx, dy);
    return pThis->m_hImageList != nullptr;
}

// Symbol: ?Attach@CImageList@@QEAAHPEAU_IMAGELIST@@@Z
extern "C" int MS_ABI impl__Attach_CImageList__QEAAHPEAU_IMAGELIST___Z(
    CImageList* pThis, HIMAGELIST hImageList) {
    if (!pThis) return FALSE;
    if (pThis->m_hImageList) {
        ::ImageList_Destroy(pThis->m_hImageList);
    }
    pThis->m_hImageList = hImageList;
    return TRUE;
}

int CImageList::Attach(HIMAGELIST hImageList) {
    return impl__Attach_CImageList__QEAAHPEAU_IMAGELIST___Z(this, hImageList);
}

// Symbol: ?Detach@CImageList@@QEAAPEAU_IMAGELIST@@XZ
extern "C" HIMAGELIST MS_ABI impl__Detach_CImageList__QEAAPEAU_IMAGELIST__XZ(CImageList* pThis) {
    if (!pThis) return nullptr;
    HIMAGELIST h = pThis->m_hImageList;
    pThis->m_hImageList = nullptr;
    return h;
}

HIMAGELIST CImageList::Detach() {
    return impl__Detach_CImageList__QEAAPEAU_IMAGELIST__XZ(this);
}

// Symbol: ?DeleteImageList@CImageList@@QEAAHXZ
extern "C" int MS_ABI impl__DeleteImageList_CImageList__QEAAHXZ(CImageList* pThis) {
    if (!pThis || !pThis->m_hImageList) return FALSE;
    int result = ::ImageList_Destroy(pThis->m_hImageList);
    pThis->m_hImageList = nullptr;
    return result;
}

int CImageList::DeleteImageList() {
    return impl__DeleteImageList_CImageList__QEAAHXZ(this);
}

// Symbol: ?FromHandle@CImageList@@SAPEAV1@PEAU_IMAGELIST@@@Z
extern "C" CImageList* MS_ABI impl__FromHandle_CImageList__SAPEAV1_PEAU_IMAGELIST___Z(HIMAGELIST hImageList) {
    return GetTempImageList(hImageList);
}

CImageList* CImageList::FromHandle(HIMAGELIST hImageList) {
    return impl__FromHandle_CImageList__SAPEAV1_PEAU_IMAGELIST___Z(hImageList);
}

// Symbol: ?FromHandlePermanent@CImageList@@SAPEAV1@PEAU_IMAGELIST@@@Z
extern "C" CImageList* MS_ABI impl__FromHandlePermanent_CImageList__SAPEAV1_PEAU_IMAGELIST___Z(HIMAGELIST hImageList) {
    return GetTempImageList(hImageList);
}

CImageList* CImageList::FromHandlePermanent(HIMAGELIST hImageList) {
    return impl__FromHandlePermanent_CImageList__SAPEAV1_PEAU_IMAGELIST___Z(hImageList);
}

// Symbol: ?DeleteTempMap@CImageList@@SAXXZ
extern "C" void MS_ABI impl__DeleteTempMap_CImageList__SAXXZ() {
    for (auto& [_, pIL] : g_tempImageListMap) {
        // Don't destroy the image list - we don't own it
        pIL->m_hImageList = nullptr;
        delete pIL;
    }
    g_tempImageListMap.clear();
}

void CImageList::DeleteTempMap() {
    impl__DeleteTempMap_CImageList__SAXXZ();
}

// CImageList::Add(HBITMAP, HBITMAP mask)
int CImageList::Add(HBITMAP hbmImage, HBITMAP hbmMask) {
    if (!m_hImageList || !hbmImage) return -1;
    return ::ImageList_Add(m_hImageList, hbmImage, hbmMask);
}

// CImageList::Add(HBITMAP, COLORREF crMask)
int CImageList::Add(HBITMAP hbmImage, COLORREF crMask) {
    if (!m_hImageList || !hbmImage) return -1;
    return ::ImageList_AddMasked(m_hImageList, hbmImage, crMask);
}

// CImageList::Add(HICON)
int CImageList::Add(HICON hIcon) {
    if (!m_hImageList || !hIcon) return -1;
    return ::ImageList_AddIcon(m_hImageList, hIcon);
}

// CImageList::Replace(int, HBITMAP, HBITMAP)
int CImageList::Replace(int nImage, HBITMAP hbmImage, HBITMAP hbmMask) {
    if (!m_hImageList || !hbmImage) return FALSE;
    return ::ImageList_Replace(m_hImageList, nImage, hbmImage, hbmMask);
}

// CImageList::Replace(int, HICON)
int CImageList::Replace(int nImage, HICON hIcon) {
    if (!m_hImageList || !hIcon) return -1;
    return ::ImageList_ReplaceIcon(m_hImageList, nImage, hIcon);
}

// CImageList::Remove
int CImageList::Remove(int nImage) {
    if (!m_hImageList) return FALSE;
    return ::ImageList_Remove(m_hImageList, nImage);
}

// CImageList::RemoveAll
int CImageList::RemoveAll() {
    if (!m_hImageList) return FALSE;
    return ::ImageList_Remove(m_hImageList, -1);
}

// CImageList::ExtractIcon
HICON CImageList::ExtractIcon(int nImage) {
    if (!m_hImageList) return nullptr;
    return ::ImageList_GetIcon(m_hImageList, nImage, ILD_TRANSPARENT);
}

// CImageList::GetImageCount
int CImageList::GetImageCount() const {
    if (!m_hImageList) return 0;
    return ::ImageList_GetImageCount(m_hImageList);
}

// CImageList::GetImageInfo
int CImageList::GetImageInfo(int nImage, IMAGEINFO* pImageInfo) const {
    if (!m_hImageList || !pImageInfo) return FALSE;
    return ::ImageList_GetImageInfo(m_hImageList, nImage, pImageInfo);
}

// CImageList::GetBkColor
COLORREF CImageList::GetBkColor() const {
    if (!m_hImageList) return CLR_NONE;
    return ::ImageList_GetBkColor(m_hImageList);
}

// CImageList::SetBkColor
COLORREF CImageList::SetBkColor(COLORREF cr) {
    if (!m_hImageList) return CLR_NONE;
    return ::ImageList_SetBkColor(m_hImageList, cr);
}

// CImageList::Draw
int CImageList::Draw(CDC* pDC, int nImage, POINT pt, UINT nStyle) {
    if (!m_hImageList || !pDC || !pDC->m_hDC) return FALSE;
    return ::ImageList_Draw(m_hImageList, nImage, pDC->m_hDC, pt.x, pt.y, nStyle);
}

// CImageList::DrawEx
int CImageList::DrawEx(CDC* pDC, int nImage, POINT pt, SIZE sz,
                       COLORREF clrBk, COLORREF clrFg, UINT nStyle) {
    if (!m_hImageList || !pDC || !pDC->m_hDC) return FALSE;
    return ::ImageList_DrawEx(m_hImageList, nImage, pDC->m_hDC,
                               pt.x, pt.y, sz.cx, sz.cy, clrBk, clrFg, nStyle);
}

// Symbol: ?DrawIndirect@CImageList@@QEAAHPEAU_IMAGELISTDRAWPARAMS@@@Z
extern "C" int MS_ABI impl__DrawIndirect_CImageList__QEAAHPEAU_IMAGELISTDRAWPARAMS___Z(
    CImageList* pThis, IMAGELISTDRAWPARAMS* pimldp) {
    if (!pThis || !pThis->m_hImageList || !pimldp) return FALSE;
    pimldp->himl = pThis->m_hImageList;
    return ::ImageList_DrawIndirect(pimldp);
}

int CImageList::DrawIndirect(IMAGELISTDRAWPARAMS* pimldp) {
    return impl__DrawIndirect_CImageList__QEAAHPEAU_IMAGELISTDRAWPARAMS___Z(this, pimldp);
}

// Symbol: ?DrawIndirect@CImageList@@QEAAHPEAVCDC@@HUtagPOINT@@UtagSIZE@@1IKKKKKK@Z
extern "C" int MS_ABI impl__DrawIndirect_CImageList__QEAAHPEAVCDC__HUtagPOINT__UtagSIZE__1IKKKKKK_Z(
    CImageList* pThis, CDC* pDC, int nImage, POINT pt, SIZE sz,
    POINT ptOrigin, unsigned int fStyle, unsigned long rgbFg,
    unsigned long rgbBk, unsigned long rgbBlend1, unsigned long rgbBlend2,
    unsigned long fState, unsigned long Frame, unsigned long crEffect) {
    if (!pThis || !pThis->m_hImageList || !pDC || !pDC->m_hDC) return FALSE;
    IMAGELISTDRAWPARAMS ildp = {};
    ildp.cbSize = sizeof(ildp);
    ildp.himl = pThis->m_hImageList;
    ildp.i = nImage;
    ildp.hdcDst = pDC->m_hDC;
    ildp.x = pt.x;
    ildp.y = pt.y;
    ildp.cx = sz.cx;
    ildp.cy = sz.cy;
    ildp.xBitmap = ptOrigin.x;
    ildp.yBitmap = ptOrigin.y;
    ildp.rgbBk = rgbBk;
    ildp.rgbFg = rgbFg;
    ildp.fStyle = fStyle;
    ildp.dwRop = SRCCOPY;
    ildp.fState = fState;
    ildp.Frame = Frame;
    ildp.crEffect = crEffect;
    return ::ImageList_DrawIndirect(&ildp);
}

int CImageList::DrawIndirect(CDC* pDC, int nImage, POINT pt, SIZE sz,
                              POINT ptOrigin, UINT fStyle, COLORREF rgbFg,
                              COLORREF rgbBk, COLORREF rgbBlend1, COLORREF rgbBlend2,
                              DWORD fState, DWORD Frame, COLORREF crEffect) {
    return impl__DrawIndirect_CImageList__QEAAHPEAVCDC__HUtagPOINT__UtagSIZE__1IKKKKKK_Z(
        this, pDC, nImage, pt, sz, ptOrigin, fStyle, rgbFg, rgbBk, rgbBlend1, rgbBlend2,
        fState, Frame, crEffect);
}

// Drag operations
int CImageList::BeginDrag(int nImage, CPoint ptHotSpot) {
    if (!m_hImageList) return FALSE;
    return ::ImageList_BeginDrag(m_hImageList, nImage, ptHotSpot.x, ptHotSpot.y);
}

void CImageList::EndDrag() {
    ::ImageList_EndDrag();
}

int CImageList::DragMove(CPoint pt) {
    return ::ImageList_DragMove(pt.x, pt.y);
}

int CImageList::DragShowNolock(int bShow) {
    return ::ImageList_DragShowNolock(bShow);
}

CImageList* CImageList::GetDragImage(LPPOINT lpPoint, LPPOINT lpPointHotSpot) {
    HIMAGELIST h = ::ImageList_GetDragImage(lpPoint, lpPointHotSpot);
    return h ? GetTempImageList(h) : nullptr;
}

int CImageList::DragEnter(CWnd* pWndLock, CPoint point) {
    HWND hWnd = pWndLock ? pWndLock->GetSafeHwnd() : nullptr;
    return ::ImageList_DragEnter(hWnd, point.x, point.y);
}

int CImageList::DragLeave(CWnd* pWndLock) {
    HWND hWnd = pWndLock ? pWndLock->GetSafeHwnd() : nullptr;
    return ::ImageList_DragLeave(hWnd);
}

// Symbol: ?Read@CImageList@@QEAAHPEAVCArchive@@@Z
extern "C" int MS_ABI impl__Read_CImageList__QEAAHPEAVCArchive___Z(CImageList* pThis, CArchive* pAr) {
    if (!pThis || !pAr || !pAr->IsLoading()) return FALSE;

    void* serializedHandle = nullptr;
    int nImageCount = 0;
    int cx = 0;
    int cy = 0;
    unsigned int flags = 0;
    int grow = 0;
    unsigned long bgColor = (unsigned long)CLR_NONE;

    *pAr >> serializedHandle;
    *pAr >> nImageCount;
    *pAr >> cx;
    *pAr >> cy;
    *pAr >> flags;
    *pAr >> grow;
    *pAr >> bgColor;

    if (pThis->m_hImageList) {
        ::ImageList_Destroy(pThis->m_hImageList);
        pThis->m_hImageList = nullptr;
    }

    HIMAGELIST source = (HIMAGELIST)serializedHandle;
    if (source) {
        HIMAGELIST copy = ::ImageList_Duplicate(source);
        if (copy) {
            pThis->m_hImageList = copy;
            if (bgColor != (unsigned long)CLR_NONE) {
                ::ImageList_SetBkColor(pThis->m_hImageList, (COLORREF)bgColor);
            }
            return TRUE;
        }
    }

    if (cx > 0 && cy > 0) {
        if (flags == 0) flags = ILC_COLOR32;
        if (nImageCount < 0) nImageCount = 0;
        pThis->m_hImageList = ::ImageList_Create(cx, cy, flags, nImageCount, grow > 0 ? grow : 1);
        if (pThis->m_hImageList && bgColor != (unsigned long)CLR_NONE) {
            ::ImageList_SetBkColor(pThis->m_hImageList, (COLORREF)bgColor);
        }
        return TRUE;
    }

    if (!serializedHandle) {
        return TRUE;
    }

    return FALSE;
}

int CImageList::Read(CArchive* pArchive) {
    return impl__Read_CImageList__QEAAHPEAVCArchive___Z(this, pArchive);
}

// Symbol: ?Write@CImageList@@QEAAHPEAVCArchive@@@Z
extern "C" int MS_ABI impl__Write_CImageList__QEAAHPEAVCArchive___Z(CImageList* pThis, CArchive* pAr) {
    if (!pThis || !pAr || !pAr->IsStoring()) return FALSE;

    void* serializedHandle = pThis->m_hImageList;
    int nImageCount = pThis ? pThis->GetImageCount() : 0;
    int cx = 0;
    int cy = 0;
    unsigned int flags = ILC_COLOR32;
    int grow = 1;
    unsigned long bgColor = (unsigned long)CLR_NONE;

    if (pThis->m_hImageList) {
        if (!::ImageList_GetIconSize(pThis->m_hImageList, &cx, &cy)) {
            cx = cy = 0;
        }
        bgColor = static_cast<unsigned long>(::ImageList_GetBkColor(pThis->m_hImageList));
    }

    *pAr << serializedHandle;
    *pAr << nImageCount;
    *pAr << cx;
    *pAr << cy;
    *pAr << flags;
    *pAr << grow;
    *pAr << bgColor;
    return TRUE;
}

int CImageList::Write(CArchive* pArchive) {
    return impl__Write_CImageList__QEAAHPEAVCArchive___Z(this, pArchive);
}

// Symbol: ?GetRuntimeClass@CImageList@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CImageList__UEBAPEAUCRuntimeClass__XZ(
    const CImageList* pThis) {
    (void)pThis;
    return CImageList::GetThisClass();
}

// Symbol: ?GetThisClass@CImageList@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CImageList__SAPEAUCRuntimeClass__XZ() {
    return CImageList::GetThisClass();
}

// Symbol: ?CreateObject@CImageList@@SAPEAVCObject@@XZ
extern "C" CObject* MS_ABI impl__CreateObject_CImageList__SAPEAVCObject__XZ() {
    return new CImageList();
}
