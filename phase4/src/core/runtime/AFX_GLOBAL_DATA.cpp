// AFX_GLOBAL_DATA — OpenMFC implementation.
// Sources: collections_strings.cpp, manual_small_stub_implementations.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/CollectionsStringsSupport.h"
#include "detail/ManualSmallStubImplementationsSupport.h"

// Symbol: ?ExcludeTag@AFX_GLOBAL_DATA@@QEAAHAEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@PEB_W0H@Z
extern "C" int MS_ABI impl__ExcludeTag_AFX_GLOBAL_DATA__QEAAHAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__PEB_W0H_Z(
    void*, CString* buffer, const wchar_t* beginTag, const wchar_t* endTag, int) {
    return ExtractTaggedText(buffer, beginTag, endTag) ? TRUE : FALSE;
}
// Symbol: ?RegisterWindowClass@AFX_GLOBAL_DATA@@QEAA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@PEB_W@Z
extern "C" void MS_ABI impl__RegisterWindowClass_AFX_GLOBAL_DATA__QEAA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__PEB_W_Z(
    CString* ret, void*, const wchar_t* requestedClassName) {
    std::wstring className = MakeWindowClassName(requestedClassName);
    HINSTANCE instance = GetModuleHandleW(nullptr);

    WNDCLASSEXW existing = {};
    existing.cbSize = sizeof(existing);
    if (!GetClassInfoExW(instance, className.c_str(), &existing)) {
        WNDCLASSEXW wc = {};
        wc.cbSize = sizeof(wc);
        wc.style = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
        wc.lpfnWndProc = DefWindowProcW;
        wc.hInstance = instance;
        wc.hCursor = LoadCursorW(nullptr, IDC_ARROW);
        wc.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);
        wc.lpszClassName = className.c_str();
        RegisterClassExW(&wc);
    }

    new (ret) CString(className.c_str());
}
// Symbol: ?DrawTextOnGlass@AFX_GLOBAL_DATA@@QEAAHPEAXPEAVCDC@@HHV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@VCRect@@KHK@Z
extern "C" int MS_ABI impl__DrawTextOnGlass_AFX_GLOBAL_DATA__QEAAHPEAXPEAVCDC__HHV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__VCRect__KHK_Z(
    void*, void*, CDC* dc, int, int, const CString* text, CRect rect, unsigned long color, int format, unsigned long) {
    if (!dc || !dc->GetSafeHdc()) return FALSE;
    COLORREF oldColor = SetTextColor(dc->GetSafeHdc(), color);
    int oldBkMode = SetBkMode(dc->GetSafeHdc(), TRANSPARENT);
    RECT winRect = { rect.left, rect.top, rect.right, rect.bottom };
    int result = DrawTextW(dc->GetSafeHdc(), CStringText(text), -1, &winRect, static_cast<UINT>(format));
    SetBkMode(dc->GetSafeHdc(), oldBkMode);
    SetTextColor(dc->GetSafeHdc(), oldColor);
    return result;
}
// Symbol: ??0AFX_GLOBAL_DATA@@QEAA@XZ
extern "C" void* MS_ABI impl___0AFX_GLOBAL_DATA__QEAA_XZ(void* pThis) {
    return pThis;
}
// Symbol: ??1AFX_GLOBAL_DATA@@QEAA@XZ
extern "C" void MS_ABI impl___1AFX_GLOBAL_DATA__QEAA_XZ(void* pThis) {
    (void)pThis;
}
// Symbol: ?CleanUp@AFX_GLOBAL_DATA@@QEAAXXZ
extern "C" void MS_ABI impl__CleanUp_AFX_GLOBAL_DATA__QEAAXXZ() {}

// Symbol: ?CreateDitherBitmap@AFX_GLOBAL_DATA@@IEAAPEAUHBITMAP__@@PEAUHDC__@@@Z
extern "C" void* MS_ABI impl__CreateDitherBitmap_AFX_GLOBAL_DATA__IEAAPEAUHBITMAP____PEAUHDC_____Z(void* /*struct*/* p0) {
    return nullptr;
}

// Symbol: ?DrawParentBackground@AFX_GLOBAL_DATA@@QEAAHPEAVCWnd@@PEAVCDC@@PEAUtagRECT@@@Z
extern "C" int MS_ABI impl__DrawParentBackground_AFX_GLOBAL_DATA__QEAAHPEAVCWnd__PEAVCDC__PEAUtagRECT___Z(void* /*class*/* p0, void* /*class*/* p1, void* /*struct*/* p2) {
    return 0;
}

// Symbol: ?EnableAccessibilitySupport@AFX_GLOBAL_DATA@@QEAAXH@Z
extern "C" void MS_ABI impl__EnableAccessibilitySupport_AFX_GLOBAL_DATA__QEAAXH_Z(int p0) {}

// Symbol: ?GetColor@AFX_GLOBAL_DATA@@QEAAKH@Z
extern "C" unsigned long MS_ABI impl__GetColor_AFX_GLOBAL_DATA__QEAAKH_Z(int p0) {
    return 0;
}

// Symbol: ?GetHandCursor@AFX_GLOBAL_DATA@@QEAAPEAUHICON__@@XZ
extern "C" void* MS_ABI impl__GetHandCursor_AFX_GLOBAL_DATA__QEAAPEAUHICON____XZ() {
    return nullptr;
}

// Symbol: ?GetITaskbarList@AFX_GLOBAL_DATA@@QEAAPEAUITaskbarList@@XZ
extern "C" void* MS_ABI impl__GetITaskbarList_AFX_GLOBAL_DATA__QEAAPEAUITaskbarList__XZ() {
    return nullptr;
}

// Symbol: ?GetITaskbarList3@AFX_GLOBAL_DATA@@QEAAPEAUITaskbarList3@@XZ
extern "C" void* MS_ABI impl__GetITaskbarList3_AFX_GLOBAL_DATA__QEAAPEAUITaskbarList3__XZ() {
    return nullptr;
}

// Symbol: ?GetNonClientMetrics@AFX_GLOBAL_DATA@@QEAAHAEAUtagNONCLIENTMETRICSW@@@Z
extern "C" int MS_ABI impl__GetNonClientMetrics_AFX_GLOBAL_DATA__QEAAHAEAUtagNONCLIENTMETRICSW___Z(void* /*struct*/* p0) {
    return 0;
}

// Symbol: ?Initialize@AFX_GLOBAL_DATA@@QEAAXXZ
extern "C" void MS_ABI impl__Initialize_AFX_GLOBAL_DATA__QEAAXXZ() {}

// Symbol: ?IsDwmCompositionEnabled@AFX_GLOBAL_DATA@@QEAAHXZ
extern "C" int MS_ABI impl__IsDwmCompositionEnabled_AFX_GLOBAL_DATA__QEAAHXZ() {
    return 0;
}

// Symbol: ?OnSettingChange@AFX_GLOBAL_DATA@@QEAAXXZ
extern "C" void MS_ABI impl__OnSettingChange_AFX_GLOBAL_DATA__QEAAXXZ() {}

// Symbol: ?Resume@AFX_GLOBAL_DATA@@QEAAHXZ
extern "C" int MS_ABI impl__Resume_AFX_GLOBAL_DATA__QEAAHXZ() {
    return 0;
}

// Symbol: ?SetLayeredAttrib@AFX_GLOBAL_DATA@@QEAAHPEAUHWND__@@KEK@Z
extern "C" int MS_ABI impl__SetLayeredAttrib_AFX_GLOBAL_DATA__QEAAHPEAUHWND____KEK_Z(void* /*struct*/* p0, unsigned long p1, unsigned char p2, unsigned long p3) {
    return 0;
}

// Symbol: ?SetMenuFont@AFX_GLOBAL_DATA@@QEAAHPEAUtagLOGFONTW@@H@Z
extern "C" int MS_ABI impl__SetMenuFont_AFX_GLOBAL_DATA__QEAAHPEAUtagLOGFONTW__H_Z(void* /*struct*/* p0, int p1) {
    return 0;
}

// Symbol: ?ShellCreateItemFromParsingName@AFX_GLOBAL_DATA@@QEAAJPEB_WPEAUIBindCtx@@AEBU_GUID@@PEAPEAX@Z
extern "C" long MS_ABI impl__ShellCreateItemFromParsingName_AFX_GLOBAL_DATA__QEAAJPEB_WPEAUIBindCtx__AEBU_GUID__PEAPEAX_Z(const wchar_t* p0, void* /*struct*/* p1, const void* /*struct*/* p2, void** p3) {
    return 0;
}

// Symbol: ?UpdateFonts@AFX_GLOBAL_DATA@@QEAAXXZ
extern "C" void MS_ABI impl__UpdateFonts_AFX_GLOBAL_DATA__QEAAXXZ() {}

// Symbol: ?UpdateSysColors@AFX_GLOBAL_DATA@@QEAAXXZ
extern "C" void MS_ABI impl__UpdateSysColors_AFX_GLOBAL_DATA__QEAAXXZ() {}

// Symbol: ?UpdateTextMetrics@AFX_GLOBAL_DATA@@IEAAXXZ
extern "C" void MS_ABI impl__UpdateTextMetrics_AFX_GLOBAL_DATA__IEAAXXZ() {}

// Symbol: ?ResetCheckCompositionFlag@AFX_GLOBAL_DATA@@QEAAXXZ
extern "C" void MS_ABI impl__ResetCheckCompositionFlag_AFX_GLOBAL_DATA__QEAAXXZ() {}
