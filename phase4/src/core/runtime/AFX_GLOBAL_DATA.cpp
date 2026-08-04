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
