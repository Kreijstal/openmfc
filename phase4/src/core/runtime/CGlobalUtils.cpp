// CGlobalUtils — OpenMFC implementation.
// Sources: global_cglobalutils_conv.cpp, global_other-22_impl.cpp, mfccore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/CPreviewViewSupport.h"
#include "detail/MfccoreSupport.h"

// OpenMFC: CGlobalUtils — currency/decimal string conversions (oleaut32-backed).
//
// CGlobalUtils is an MFC feature-pack utility class; these two methods are
// stateless numeric parsers that wrap the OLE Automation Variant conversion
// APIs (fully available under Wine). The remaining CGlobalUtils exports are
// docking/pane geometry helpers (CWnd/CDockablePane/CPaneContainerManager) and
// the StringFromCy/StringFromDecimal pair (CStringT-by-reference out params);
// those are left for a GUI/string-coupled tranche.
//
// pThis (the CGlobalUtils*) is accepted to match the member-function ABI but is
// unused — both conversions are pure functions of their inputs.

#include <windows.h>
#include <oleauto.h>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif


// ?CyFromString@CGlobalUtils@@QEAAHAEATtagCY@@PEB_W@Z
// int CyFromString(CY& cyResult, const wchar_t* lpszValue)
// Parses a currency string into a CY (scaled by 10000). Returns TRUE on success.
// Symbol: ?CyFromString@CGlobalUtils@@QEAAHAEATtagCY@@PEB_W@Z
extern "C" int MS_ABI impl__CyFromString_CGlobalUtils__QEAAHAEATtagCY__PEB_W_Z(
    void* /*pThis*/, CY* pcyResult, const wchar_t* lpszValue)
{
    if (!pcyResult) return 0;
    pcyResult->int64 = 0;
    if (!lpszValue) return 0;
    HRESULT hr = VarCyFromStr(const_cast<wchar_t*>(lpszValue), GetThreadLocale(), 0, pcyResult);
    return SUCCEEDED(hr) ? 1 : 0;
}
// ?DecimalFromString@CGlobalUtils@@QEAAHAEAUtagDEC@@PEB_W@Z
// int DecimalFromString(DECIMAL& decResult, const wchar_t* lpszValue)
// Parses a decimal string into a DECIMAL. Returns TRUE on success.
// Symbol: ?DecimalFromString@CGlobalUtils@@QEAAHAEAUtagDEC@@PEB_W@Z
extern "C" int MS_ABI impl__DecimalFromString_CGlobalUtils__QEAAHAEAUtagDEC__PEB_W_Z(
    void* /*pThis*/, DECIMAL* pdecResult, const wchar_t* lpszValue)
{
    if (!pdecResult) return 0;
    *pdecResult = DECIMAL();
    if (!lpszValue) return 0;
    HRESULT hr = VarDecFromStr(const_cast<wchar_t*>(lpszValue), GetThreadLocale(), 0, pdecResult);
    return SUCCEEDED(hr) ? 1 : 0;
}
// Retail (0x18006dac0): build a VT_CY VARIANT from the CY value, convert it to
// a BSTR with VariantChangeType, assign the BSTR into the CString (helper
// 0x180002e30 = CStringT assignment), VariantClear both VARIANTs, return
// TRUE on success.  VarBstrFromCy covers the same conversion; the BSTR is
// freed by SysFreeString (retail frees it via VariantClear).
// Symbol: ?StringFromCy@CGlobalUtils@@QEAAHAEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@AEATtagCY@@@Z
extern "C" int MS_ABI impl__StringFromCy_CGlobalUtils__QEAAHAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__AEATtagCY___Z(
    void* /*pThis*/, CString* pstr, CY* pcy)
{
    if (!pstr) return 0;
    if (!pcy) return 0;

    BSTR bstr = nullptr;
    HRESULT hr = ::VarBstrFromCy(*pcy, ::GetThreadLocale(), 0, &bstr);
    if (FAILED(hr)) return 0;

    if (bstr) {
        *pstr = static_cast<const wchar_t*>(bstr);
        ::SysFreeString(bstr);
    } else {
        *pstr = L"";
    }
    return 1;
}
// Retail (0x18006dc50): same shape as StringFromCy but with a VT_DECIMAL
// source -- VariantChangeType(&result, &src, 0, VT_BSTR) followed by the
// CString assignment and VariantClear cleanup.
// Symbol: ?StringFromDecimal@CGlobalUtils@@QEAAHAEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@AEAUtagDEC@@@Z
extern "C" int MS_ABI impl__StringFromDecimal_CGlobalUtils__QEAAHAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__AEAUtagDEC___Z(
    void* /*pThis*/, CString* pstr, DECIMAL* pdec)
{
    if (!pstr) return 0;
    if (!pdec) return 0;

    BSTR bstr = nullptr;
    HRESULT hr = ::VarBstrFromDec(pdec, ::GetThreadLocale(), 0, &bstr);
    if (FAILED(hr)) return 0;

    if (bstr) {
        *pstr = static_cast<const wchar_t*>(bstr);
        ::SysFreeString(bstr);
    } else {
        *pstr = L"";
    }
    return 1;
}
// Symbol: ??0CGlobalUtils@@QEAA@XZ
extern "C" void* MS_ABI impl___0CGlobalUtils__QEAA_XZ(void* pThis) { return new (pThis) CGlobalUtils(); }
// Symbol: ??1CGlobalUtils@@UEAA@XZ
extern "C" void MS_ABI impl___1CGlobalUtils__UEAA_XZ(CGlobalUtils* pThis) { if (pThis) pThis->~CGlobalUtils(); }
// Symbol: ?AdjustRectToWorkArea@CGlobalUtils@@QEAAXAEAVCRect@@PEAV2@@Z
extern "C" void MS_ABI impl__AdjustRectToWorkArea_CGlobalUtils__QEAAXAEAVCRect__PEAV2__Z(CGlobalUtils* pThis, CRect* rect, CRect* delta) { if (pThis && rect) pThis->AdjustRectToWorkArea(*rect, delta); }
// Symbol: ?FlipRect@CGlobalUtils@@QEAAXAEAVCRect@@H@Z
extern "C" void MS_ABI impl__FlipRect_CGlobalUtils__QEAAXAEAVCRect__H_Z(CGlobalUtils* pThis, CRect* rect, int horz) { if (pThis && rect) pThis->FlipRect(*rect, horz); }
// Symbol: ?GetOppositeAlignment@CGlobalUtils@@QEAAKK@Z
extern "C" unsigned long MS_ABI impl__GetOppositeAlignment_CGlobalUtils__QEAAKK_Z(CGlobalUtils* pThis, unsigned long align) { return pThis ? pThis->GetOppositeAlignment(align) : align; }
// Symbol: ?GetSystemBorders@CGlobalUtils@@QEAA?AVCSize@@K@Z
extern "C" void MS_ABI impl__GetSystemBorders_CGlobalUtils__QEAA_AVCSize__K_Z(CSize* ret, CGlobalUtils* pThis, unsigned long style) { new (ret) CSize(pThis ? pThis->GetSystemBorders(style) : CSize()); }
// Symbol: ?GetSystemBorders@CGlobalUtils@@QEAA?AVCSize@@PEAVCWnd@@@Z
extern "C" void MS_ABI impl__GetSystemBorders_CGlobalUtils__QEAA_AVCSize__PEAVCWnd___Z(CSize* ret, CGlobalUtils* pThis, CWnd* wnd) { new (ret) CSize(pThis ? pThis->GetSystemBorders(wnd) : CSize()); }
// Symbol: ?GetWndIcon@CGlobalUtils@@QEAAPEAUHICON__@@PEAVCWnd@@@Z
extern "C" HICON MS_ABI impl__GetWndIcon_CGlobalUtils__QEAAPEAUHICON____PEAVCWnd___Z(CGlobalUtils* pThis, CWnd* wnd) { return pThis ? pThis->GetWndIcon(wnd) : nullptr; }
// Symbol: ?CanBeAttached@CGlobalUtils@@QEBAHPEAVCWnd@@@Z
extern "C" int MS_ABI impl__CanBeAttached_CGlobalUtils__QEBAHPEAVCWnd___Z(const CGlobalUtils* pThis, CWnd* wnd) { return pThis ? pThis->CanBeAttached(wnd) : FALSE; }
// Symbol: ?CanPaneBeInFloatingMultiPaneFrameWnd@CGlobalUtils@@QEBAHPEAVCWnd@@@Z
extern "C" int MS_ABI impl__CanPaneBeInFloatingMultiPaneFrameWnd_CGlobalUtils__QEBAHPEAVCWnd___Z(const CGlobalUtils* pThis, CWnd* wnd) { return pThis ? pThis->CanPaneBeInFloatingMultiPaneFrameWnd(wnd) : FALSE; }
// Symbol: ?GetDockingManager@CGlobalUtils@@QEAAPEAVCDockingManager@@PEAVCWnd@@@Z
extern "C" CDockingManager* MS_ABI impl__GetDockingManager_CGlobalUtils__QEAAPEAVCDockingManager__PEAVCWnd___Z(CGlobalUtils* pThis, CWnd* wnd) { return pThis ? pThis->GetDockingManager(wnd) : nullptr; }
CGlobalUtils::CGlobalUtils() { memset(_globalutils_padding, 0, sizeof(_globalutils_padding)); }
CGlobalUtils::~CGlobalUtils() {}
void CGlobalUtils::AdjustRectToWorkArea(CRect& rect, CRect* pRectDelta) {
    CRect old = rect;
    rect.NormalizeRect();
    RECT workArea = {};
    if (!::SystemParametersInfoW(SPI_GETWORKAREA, 0, &workArea, 0)) {
        workArea.left = 0;
        workArea.top = 0;
        workArea.right = ::GetSystemMetrics(SM_CXSCREEN);
        workArea.bottom = ::GetSystemMetrics(SM_CYSCREEN);
    }
    if (rect.right > workArea.right) rect.OffsetRect(workArea.right - rect.right, 0);
    if (rect.bottom > workArea.bottom) rect.OffsetRect(0, workArea.bottom - rect.bottom);
    if (rect.left < workArea.left) rect.OffsetRect(workArea.left - rect.left, 0);
    if (rect.top < workArea.top) rect.OffsetRect(0, workArea.top - rect.top);
    if (pRectDelta) *pRectDelta = CRect(rect.left - old.left, rect.top - old.top, rect.right - old.right, rect.bottom - old.bottom);
}
void CGlobalUtils::FlipRect(CRect& rect, BOOL bHorz) { if (bHorz) std::swap(rect.left, rect.right); else std::swap(rect.top, rect.bottom); rect.NormalizeRect(); }
DWORD CGlobalUtils::GetOppositeAlignment(DWORD dwAlign) {
    if (dwAlign & CBRS_LEFT) return CBRS_RIGHT;
    if (dwAlign & CBRS_RIGHT) return CBRS_LEFT;
    if (dwAlign & CBRS_TOP) return CBRS_BOTTOM;
    if (dwAlign & CBRS_BOTTOM) return CBRS_TOP;
    return dwAlign;
}
CSize CGlobalUtils::GetSystemBorders(DWORD dwStyle) { (void)dwStyle; return CSize(::GetSystemMetrics(SM_CXFRAME), ::GetSystemMetrics(SM_CYFRAME)); }
CSize CGlobalUtils::GetSystemBorders(CWnd* pWnd) { return GetSystemBorders(pWnd ? static_cast<DWORD>(::GetWindowLongPtrW(pWnd->GetSafeHwnd(), GWL_STYLE)) : 0); }
HICON CGlobalUtils::GetWndIcon(CWnd* pWnd) { return pWnd ? reinterpret_cast<HICON>(::SendMessageW(pWnd->GetSafeHwnd(), WM_GETICON, ICON_SMALL, 0)) : nullptr; }
BOOL CGlobalUtils::CanBeAttached(CWnd* pWnd) const { return pWnd != nullptr; }
BOOL CGlobalUtils::CanPaneBeInFloatingMultiPaneFrameWnd(CWnd* pWnd) const { return pWnd != nullptr; }
CDockingManager* CGlobalUtils::GetDockingManager(CWnd* pWnd) { return pWnd && pWnd->IsKindOf(RUNTIME_CLASS(CFrameWndEx)) ? static_cast<CFrameWndEx*>(pWnd)->GetDockingManager() : nullptr; }
// ?CalcExpectedDockedRect@CGlobalUtils@@QEAAXAEAVCPaneContainerManager@@PEAVCWnd@@VCPoint@@AEAVCRect@@AEAHPEAPEAVCDockablePane@@@Z
extern "C" void MS_ABI impl__CalcExpectedDockedRect_CGlobalUtils__QEAAXAEAVCPaneContainerManager__PEAVCWnd__VCPoint__AEAVCRect__AEAHPEAPEAVCDockablePane___Z(void* /*class*/* p0, void* /*class*/* p1, void* /*class*/ p2, void* /*class*/* p3, int* p4, void* /*class*/** p5) {}

// ?CheckAlignment@CGlobalUtils@@QEBAHVCPoint@@PEAVCBasePane@@HPEBVCDockingManager@@HAEAKKPEBUtagRECT@@@Z
extern "C" int MS_ABI impl__CheckAlignment_CGlobalUtils__QEBAHVCPoint__PEAVCBasePane__HPEBVCDockingManager__HAEAKKPEBUtagRECT___Z(void* /*class*/ p0, void* /*class*/* p1, int p2, const void* /*class*/* p3, int p4, unsigned long* p5, unsigned long p6, const void* /*struct*/* p7) {
    return 0;
}

// ?ForceAdjustLayout@CGlobalUtils@@QEAAXPEAVCDockingManager@@HH@Z
extern "C" void MS_ABI impl__ForceAdjustLayout_CGlobalUtils__QEAAXPEAVCDockingManager__HH_Z(void* /*class*/* p0, int p1, int p2) {}

// ?GetPaneAndAlignFromPoint@CGlobalUtils@@QEAAHAEAVCPaneContainerManager@@VCPoint@@PEAPEAVCDockablePane@@AEAKAEAH4@Z
extern "C" int MS_ABI impl__GetPaneAndAlignFromPoint_CGlobalUtils__QEAAHAEAVCPaneContainerManager__VCPoint__PEAPEAVCDockablePane__AEAKAEAH4_Z(void* /*class*/* p0, void* /*class*/ p1, void* /*class*/** p2, unsigned long* p3, int* p4, int* p5) {
    return 0;
}

// ?SetNewParent@CGlobalUtils@@QEAAXAEAVCObList@@PEAVCWnd@@H@Z
extern "C" void MS_ABI impl__SetNewParent_CGlobalUtils__QEAAXAEAVCObList__PEAVCWnd__H_Z(void* /*class*/* p0, void* /*class*/* p1, int p2) {}

