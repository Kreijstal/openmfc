// CPreviewDC GDI drawing implementations.
//
// CPreviewDC is not exposed in the public compatibility headers yet. Its
// harvested layout has CDC as the base prefix, so these exports operate on that
// CDC portion and keep preview-only state externally.

#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxwin.h"
#include <windows.h>
#include <mutex>
#include <unordered_map>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

extern "C" CFont* MS_ABI impl__SelectObject_CDC__UEAAPEAVCFont__PEAV2__Z(CDC*, CFont*);
extern "C" CGdiObject* MS_ABI impl__SelectStockObject_CDC__UEAAPEAVCGdiObject__H_Z(CDC*, int);

namespace {
struct PreviewDCState {
    int scaleNum = 1;
    int scaleDen = 1;
    CSize topLeftOffset = CSize(0, 0);
};

std::mutex g_previewDcMutex;
std::unordered_map<void*, PreviewDCState> g_previewDcState;

CDC* PreviewAsCDC(void* pThis) {
    return static_cast<CDC*>(pThis);
}

PreviewDCState GetPreviewState(void* pThis) {
    std::lock_guard<std::mutex> lock(g_previewDcMutex);
    auto it = g_previewDcState.find(pThis);
    return it != g_previewDcState.end() ? it->second : PreviewDCState();
}

void SetPreviewScale(void* pThis, int num, int den) {
    std::lock_guard<std::mutex> lock(g_previewDcMutex);
    auto& state = g_previewDcState[pThis];
    state.scaleNum = num ? num : 1;
    state.scaleDen = den ? den : 1;
}

void SetPreviewOffset(void* pThis, CSize offset) {
    std::lock_guard<std::mutex> lock(g_previewDcMutex);
    g_previewDcState[pThis].topLeftOffset = offset;
}

int ScaleCoord(int value, const PreviewDCState& state) {
    return ::MulDiv(value, state.scaleNum, state.scaleDen);
}

RECT ScaleRect(const RECT* rect, const PreviewDCState& state) {
    RECT out = {};
    if (!rect) return out;
    out.left = ScaleCoord(rect->left, state);
    out.top = ScaleCoord(rect->top, state);
    out.right = ScaleCoord(rect->right, state);
    out.bottom = ScaleCoord(rect->bottom, state);
    return out;
}

HDC OutputDC(void* pThis) {
    CDC* dc = PreviewAsCDC(pThis);
    return dc ? dc->m_hDC : nullptr;
}

HDC AttribDC(void* pThis) {
    CDC* dc = PreviewAsCDC(pThis);
    if (!dc) return nullptr;
    return dc->m_hAttribDC ? dc->m_hAttribDC : dc->m_hDC;
}
} // namespace

// Symbol: ?SaveDC@CPreviewDC@@UEAAHXZ
extern "C" int MS_ABI impl__SaveDC_CPreviewDC__UEAAHXZ(void* pThis) {
    HDC hdc = OutputDC(pThis);
    return hdc ? ::SaveDC(hdc) : 0;
}

// Symbol: ?RestoreDC@CPreviewDC@@UEAAHH@Z
extern "C" int MS_ABI impl__RestoreDC_CPreviewDC__UEAAHH_Z(void* pThis, int nSavedDC) {
    HDC hdc = OutputDC(pThis);
    return hdc ? ::RestoreDC(hdc, nSavedDC) : FALSE;
}

// Symbol: ?SetAttribDC@CPreviewDC@@UEAAXPEAUHDC__@@@Z
extern "C" void MS_ABI impl__SetAttribDC_CPreviewDC__UEAAXPEAUHDC_____Z(void* pThis, HDC hDC) {
    CDC* dc = PreviewAsCDC(pThis);
    if (dc) dc->m_hAttribDC = hDC;
}

// Symbol: ?SetOutputDC@CPreviewDC@@UEAAXPEAUHDC__@@@Z
extern "C" void MS_ABI impl__SetOutputDC_CPreviewDC__UEAAXPEAUHDC_____Z(void* pThis, HDC hDC) {
    CDC* dc = PreviewAsCDC(pThis);
    if (dc) dc->m_hDC = hDC;
}

// Symbol: ?ReleaseOutputDC@CPreviewDC@@UEAAXXZ
extern "C" void MS_ABI impl__ReleaseOutputDC_CPreviewDC__UEAAXXZ(void* pThis) {
    CDC* dc = PreviewAsCDC(pThis);
    if (dc) dc->m_hDC = nullptr;
}

// Symbol: ?SetScaleRatio@CPreviewDC@@QEAAXHH@Z
extern "C" void MS_ABI impl__SetScaleRatio_CPreviewDC__QEAAXHH_Z(void* pThis, int nNum, int nDen) {
    if (!pThis) return;
    SetPreviewScale(pThis, nNum, nDen);
}

// Symbol: ?SetTopLeftOffset@CPreviewDC@@QEAAXVCSize@@@Z
extern "C" void MS_ABI impl__SetTopLeftOffset_CPreviewDC__QEAAXVCSize___Z(void* pThis, CSize size) {
    if (!pThis) return;
    SetPreviewOffset(pThis, size);
}

// Symbol: ?SetBkColor@CPreviewDC@@UEAAKK@Z
extern "C" unsigned long MS_ABI impl__SetBkColor_CPreviewDC__UEAAKK_Z(void* pThis, unsigned long crColor) {
    HDC hdc = OutputDC(pThis);
    return hdc ? ::SetBkColor(hdc, crColor) : CLR_INVALID;
}

// Symbol: ?SetTextColor@CPreviewDC@@UEAAKK@Z
extern "C" unsigned long MS_ABI impl__SetTextColor_CPreviewDC__UEAAKK_Z(void* pThis, unsigned long crColor) {
    HDC hdc = OutputDC(pThis);
    return hdc ? ::SetTextColor(hdc, crColor) : CLR_INVALID;
}

// Symbol: ?SetMapMode@CPreviewDC@@UEAAHH@Z
extern "C" int MS_ABI impl__SetMapMode_CPreviewDC__UEAAHH_Z(void* pThis, int nMapMode) {
    HDC hdc = OutputDC(pThis);
    return hdc ? ::SetMapMode(hdc, nMapMode) : 0;
}

// Symbol: ?SetViewportOrg@CPreviewDC@@UEAA?AVCPoint@@HH@Z
extern "C" CPoint MS_ABI impl__SetViewportOrg_CPreviewDC__UEAA_AVCPoint__HH_Z(void* pThis, int x, int y) {
    CPoint old(0, 0);
    HDC hdc = OutputDC(pThis);
    if (!hdc) return old;
    POINT prev = {};
    PreviewDCState state = GetPreviewState(pThis);
    ::SetViewportOrgEx(hdc,
                       ScaleCoord(x, state) + state.topLeftOffset.cx,
                       ScaleCoord(y, state) + state.topLeftOffset.cy,
                       &prev);
    old.x = prev.x;
    old.y = prev.y;
    return old;
}

// Symbol: ?OffsetViewportOrg@CPreviewDC@@UEAA?AVCPoint@@HH@Z
extern "C" CPoint MS_ABI impl__OffsetViewportOrg_CPreviewDC__UEAA_AVCPoint__HH_Z(void* pThis, int x, int y) {
    CPoint old(0, 0);
    HDC hdc = OutputDC(pThis);
    if (!hdc) return old;
    POINT prev = {};
    PreviewDCState state = GetPreviewState(pThis);
    ::OffsetViewportOrgEx(hdc, ScaleCoord(x, state), ScaleCoord(y, state), &prev);
    old.x = prev.x;
    old.y = prev.y;
    return old;
}

// Symbol: ?SetViewportExt@CPreviewDC@@UEAA?AVCSize@@HH@Z
extern "C" CSize MS_ABI impl__SetViewportExt_CPreviewDC__UEAA_AVCSize__HH_Z(void* pThis, int cx, int cy) {
    CSize old(0, 0);
    HDC hdc = OutputDC(pThis);
    if (!hdc) return old;
    SIZE prev = {};
    PreviewDCState state = GetPreviewState(pThis);
    ::SetViewportExtEx(hdc, ScaleCoord(cx, state), ScaleCoord(cy, state), &prev);
    old.cx = prev.cx;
    old.cy = prev.cy;
    return old;
}

// Symbol: ?ScaleViewportExt@CPreviewDC@@UEAA?AVCSize@@HHHH@Z
extern "C" CSize MS_ABI impl__ScaleViewportExt_CPreviewDC__UEAA_AVCSize__HHHH_Z(
    void* pThis, int xNum, int xDenom, int yNum, int yDenom) {
    CSize old(0, 0);
    HDC hdc = OutputDC(pThis);
    if (!hdc) return old;
    SIZE prev = {};
    ::ScaleViewportExtEx(hdc, xNum, xDenom, yNum, yDenom, &prev);
    old.cx = prev.cx;
    old.cy = prev.cy;
    return old;
}

// Symbol: ?SetWindowExt@CPreviewDC@@UEAA?AVCSize@@HH@Z
extern "C" CSize MS_ABI impl__SetWindowExt_CPreviewDC__UEAA_AVCSize__HH_Z(void* pThis, int cx, int cy) {
    CSize old(0, 0);
    HDC hdc = OutputDC(pThis);
    if (!hdc) return old;
    SIZE prev = {};
    ::SetWindowExtEx(hdc, cx, cy, &prev);
    old.cx = prev.cx;
    old.cy = prev.cy;
    return old;
}

// Symbol: ?ScaleWindowExt@CPreviewDC@@UEAA?AVCSize@@HHHH@Z
extern "C" CSize MS_ABI impl__ScaleWindowExt_CPreviewDC__UEAA_AVCSize__HHHH_Z(
    void* pThis, int xNum, int xDenom, int yNum, int yDenom) {
    CSize old(0, 0);
    HDC hdc = OutputDC(pThis);
    if (!hdc) return old;
    SIZE prev = {};
    ::ScaleWindowExtEx(hdc, xNum, xDenom, yNum, yDenom, &prev);
    old.cx = prev.cx;
    old.cy = prev.cy;
    return old;
}

// Symbol: ?SelectObject@CPreviewDC@@UEAAPEAVCFont@@PEAV2@@Z
extern "C" CFont* MS_ABI impl__SelectObject_CPreviewDC__UEAAPEAVCFont__PEAV2__Z(void* pThis, CFont* pFont) {
    CDC* dc = PreviewAsCDC(pThis);
    if (!dc) return nullptr;
    return impl__SelectObject_CDC__UEAAPEAVCFont__PEAV2__Z(dc, pFont);
}

// Symbol: ?SelectStockObject@CPreviewDC@@UEAAPEAVCGdiObject@@H@Z
extern "C" CGdiObject* MS_ABI impl__SelectStockObject_CPreviewDC__UEAAPEAVCGdiObject__H_Z(void* pThis, int nIndex) {
    CDC* dc = PreviewAsCDC(pThis);
    if (!dc) return nullptr;
    return impl__SelectStockObject_CDC__UEAAPEAVCGdiObject__H_Z(dc, nIndex);
}

// Symbol: ?TextOutW@CPreviewDC@@UEAAHHHPEB_WH@Z
extern "C" int MS_ABI impl__TextOutW_CPreviewDC__UEAAHHHPEB_WH_Z(
    void* pThis, int x, int y, const wchar_t* text, int count) {
    HDC hdc = OutputDC(pThis);
    if (!hdc || !text) return FALSE;
    if (count < 0) count = lstrlenW(text);
    PreviewDCState state = GetPreviewState(pThis);
    return ::TextOutW(hdc, ScaleCoord(x, state), ScaleCoord(y, state), text, count);
}

// Symbol: ?ExtTextOutW@CPreviewDC@@UEAAHHHIPEBUtagRECT@@PEB_WIPEAH@Z
extern "C" int MS_ABI impl__ExtTextOutW_CPreviewDC__UEAAHHHIPEBUtagRECT__PEB_WIPEAH_Z(
    void* pThis, int x, int y, unsigned int options, const RECT* rect,
    const wchar_t* text, unsigned int count, int* dx) {
    HDC hdc = OutputDC(pThis);
    if (!hdc) return FALSE;
    PreviewDCState state = GetPreviewState(pThis);
    RECT scaled = ScaleRect(rect, state);
    return ::ExtTextOutW(hdc, ScaleCoord(x, state), ScaleCoord(y, state), options,
                         rect ? &scaled : nullptr, text, count, dx);
}

// Symbol: ?TabbedTextOutW@CPreviewDC@@UEAA?AVCSize@@HHPEB_WHHPEAHH@Z
extern "C" CSize MS_ABI impl__TabbedTextOutW_CPreviewDC__UEAA_AVCSize__HHPEB_WHHPEAHH_Z(
    void* pThis, int x, int y, const wchar_t* text, int count,
    int tabPositions, int* tabStops, int tabOrigin) {
    CSize result(0, 0);
    HDC hdc = OutputDC(pThis);
    if (!hdc || !text) return result;
    if (count < 0) count = lstrlenW(text);
    PreviewDCState state = GetPreviewState(pThis);
    LONG packed = ::TabbedTextOutW(hdc, ScaleCoord(x, state), ScaleCoord(y, state),
                                   text, count, tabPositions, tabStops,
                                   ScaleCoord(tabOrigin, state));
    result.cx = LOWORD(packed);
    result.cy = HIWORD(packed);
    return result;
}

// Symbol: ?DrawTextW@CPreviewDC@@UEAAHPEB_WHPEAUtagRECT@@I@Z
extern "C" int MS_ABI impl__DrawTextW_CPreviewDC__UEAAHPEB_WHPEAUtagRECT__I_Z(
    void* pThis, const wchar_t* text, int count, RECT* rect, unsigned int format) {
    HDC hdc = OutputDC(pThis);
    if (!hdc || !rect) return 0;
    RECT scaled = ScaleRect(rect, GetPreviewState(pThis));
    int ret = ::DrawTextW(hdc, text, count, &scaled, format);
    *rect = scaled;
    return ret;
}

// Symbol: ?DrawTextExW@CPreviewDC@@UEAAHPEA_WHPEAUtagRECT@@IPEAUtagDRAWTEXTPARAMS@@@Z
extern "C" int MS_ABI impl__DrawTextExW_CPreviewDC__UEAAHPEA_WHPEAUtagRECT__IPEAUtagDRAWTEXTPARAMS___Z(
    void* pThis, wchar_t* text, int count, RECT* rect, unsigned int format, DRAWTEXTPARAMS* params) {
    HDC hdc = OutputDC(pThis);
    if (!hdc || !rect) return 0;
    RECT scaled = ScaleRect(rect, GetPreviewState(pThis));
    int ret = ::DrawTextExW(hdc, text, count, &scaled, format, params);
    *rect = scaled;
    return ret;
}

// Symbol: ?Escape@CPreviewDC@@UEAAHHHPEBDPEAX@Z
extern "C" int MS_ABI impl__Escape_CPreviewDC__UEAAHHHPEBDPEAX_Z(
    void* pThis, int escape, int count, const char* inData, void* outData) {
    HDC hdc = OutputDC(pThis);
    return hdc ? ::Escape(hdc, escape, count, inData, outData) : 0;
}

// Symbol: ?GrayStringW@CPreviewDC@@UEAAHPEAVCBrush@@P6AHPEAUHDC__@@_JH@Z2HHHHH@Z
extern "C" int MS_ABI impl__GrayStringW_CPreviewDC__UEAAHPEAVCBrush__P6AHPEAUHDC_____JH_Z2HHHHH_Z(
    void* pThis, CBrush* brush, GRAYSTRINGPROC proc, LPARAM data,
    int count, int x, int y, int width, int height) {
    HDC hdc = OutputDC(pThis);
    if (!hdc) return FALSE;
    PreviewDCState state = GetPreviewState(pThis);
    return ::GrayStringW(hdc, brush ? static_cast<HBRUSH>(brush->m_hObject) : nullptr,
                         proc, data, count, ScaleCoord(x, state), ScaleCoord(y, state),
                         ScaleCoord(width, state), ScaleCoord(height, state));
}

// Symbol: ?ClipToPage@CPreviewDC@@QEAAXXZ
extern "C" void MS_ABI impl__ClipToPage_CPreviewDC__QEAAXXZ(void* pThis) {
    HDC hdc = OutputDC(pThis);
    if (!hdc) return;
    int width = ::GetDeviceCaps(hdc, HORZRES);
    int height = ::GetDeviceCaps(hdc, VERTRES);
    if (width > 0 && height > 0) {
        ::IntersectClipRect(hdc, 0, 0, width, height);
    }
}

// Symbol: ?PrinterDPtoScreenDP@CPreviewDC@@QEBAXPEAUtagPOINT@@@Z
extern "C" void MS_ABI impl__PrinterDPtoScreenDP_CPreviewDC__QEBAXPEAUtagPOINT___Z(const void* pThis, POINT* point) {
    if (!pThis || !point) return;
    PreviewDCState state = GetPreviewState(const_cast<void*>(pThis));
    point->x = ScaleCoord(point->x, state);
    point->y = ScaleCoord(point->y, state);
}

// Symbol: ?MirrorMappingMode@CPreviewDC@@IEAAXH@Z
extern "C" void MS_ABI impl__MirrorMappingMode_CPreviewDC__IEAAXH_Z(void* pThis, int mapMode) {
    HDC hdc = OutputDC(pThis);
    if (hdc) ::SetMapMode(hdc, mapMode);
}

// Symbol: ?MirrorViewportOrg@CPreviewDC@@IEAAXXZ
extern "C" void MS_ABI impl__MirrorViewportOrg_CPreviewDC__IEAAXXZ(void* pThis) {
    HDC out = OutputDC(pThis);
    HDC attr = AttribDC(pThis);
    if (!out || !attr) return;
    POINT pt = {};
    if (::GetViewportOrgEx(attr, &pt)) {
        PreviewDCState state = GetPreviewState(pThis);
        ::SetViewportOrgEx(out, pt.x + state.topLeftOffset.cx, pt.y + state.topLeftOffset.cy, nullptr);
    }
}

// Symbol: ?MirrorFont@CPreviewDC@@IEAAXXZ
extern "C" void MS_ABI impl__MirrorFont_CPreviewDC__IEAAXXZ(void* pThis) {
    HDC out = OutputDC(pThis);
    HDC attr = AttribDC(pThis);
    if (!out || !attr) return;
    HGDIOBJ font = ::GetCurrentObject(attr, OBJ_FONT);
    if (font) ::SelectObject(out, font);
}

// Symbol: ?MirrorAttributes@CPreviewDC@@IEAAXXZ
extern "C" void MS_ABI impl__MirrorAttributes_CPreviewDC__IEAAXXZ(void* pThis) {
    HDC out = OutputDC(pThis);
    HDC attr = AttribDC(pThis);
    if (!out || !attr) return;
    ::SetBkColor(out, ::GetBkColor(attr));
    ::SetTextColor(out, ::GetTextColor(attr));
    ::SetBkMode(out, ::GetBkMode(attr));
    ::SetTextAlign(out, ::GetTextAlign(attr));
    ::SetROP2(out, ::GetROP2(attr));
    ::SetPolyFillMode(out, ::GetPolyFillMode(attr));
    ::SetStretchBltMode(out, ::GetStretchBltMode(attr));
}

// Symbol: ?ComputeDeltas@CPreviewDC@@IEAA?AVCSize@@AEAHPEB_WAEAIHIPEAHHPEA_W30@Z
extern "C" CSize MS_ABI impl__ComputeDeltas_CPreviewDC__IEAA_AVCSize__AEAHPEB_WAEAIHIPEAHHPEA_W30_Z(
    void* pThis, int* x, const wchar_t* text, unsigned int* count,
    int tabCount, unsigned int options, int* tabs, int tabOrigin,
    wchar_t* outputText, int* outputDx, int* delta) {
    (void)tabCount;
    (void)options;
    (void)tabs;
    (void)tabOrigin;
    CSize result(0, 0);
    HDC hdc = AttribDC(pThis);
    if (!hdc || !text || !count) return result;
    int cch = static_cast<int>(*count);
    if (cch < 0) cch = lstrlenW(text);
    SIZE extent = {};
    if (::GetTextExtentPoint32W(hdc, text, cch, &extent)) {
        result.cx = extent.cx;
        result.cy = extent.cy;
    }
    if (x) *x += result.cx;
    if (outputText && cch > 0) {
        lstrcpynW(outputText, text, cch + 1);
    }
    if (outputDx) {
        for (int i = 0; i < cch; ++i) outputDx[i] = 0;
    }
    if (delta) *delta = 0;
    return result;
}
