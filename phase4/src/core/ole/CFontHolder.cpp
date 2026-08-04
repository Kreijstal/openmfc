// CFontHolder — OpenMFC implementation.
// Sources: collections_strings.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/CollectionsStringsSupport.h"

// Symbol: ??0CFontHolder@@QEAA@PEAUIPropertyNotifySink@@@Z
extern "C" void* MS_ABI impl___0CFontHolder__QEAA_PEAUIPropertyNotifySink___Z(void* self, void* notifySink) {
    FontHolderState& state = g_fontHolderStates[self];
    state.notifySink = notifySink;
    state.hFont = static_cast<HFONT>(::GetStockObject(DEFAULT_GUI_FONT));
    state.faceName = FontFaceFromHandle(state.hFont);
    return self;
}
// Symbol: ??1CFontHolder@@QEAA@XZ
extern "C" void MS_ABI impl___1CFontHolder__QEAA_XZ(void* self) {
    g_fontHolderStates.erase(self);
}
// Symbol: ?InitializeFont@CFontHolder@@QEAAXPEBUtagFONTDESC@@PEAUIDispatch@@@Z
extern "C" void MS_ABI impl__InitializeFont_CFontHolder__QEAAXPEBUtagFONTDESC__PEAUIDispatch___Z(void* self, const void*, void* dispatch) {
    FontHolderState& state = g_fontHolderStates[self];
    state.font = dispatch;
    state.hFont = static_cast<HFONT>(::GetStockObject(DEFAULT_GUI_FONT));
    state.faceName = FontFaceFromHandle(state.hFont);
}
// Symbol: ?SetFont@CFontHolder@@QEAAXPEAUIFont@@@Z
extern "C" void MS_ABI impl__SetFont_CFontHolder__QEAAXPEAUIFont___Z(void* self, void* font) {
    FontHolderState& state = g_fontHolderStates[self];
    state.font = font;
    state.hFont = font ? reinterpret_cast<HFONT>(font) : static_cast<HFONT>(::GetStockObject(DEFAULT_GUI_FONT));
    CString face = FontFaceFromHandle(state.hFont);
    if (!face.IsEmpty()) state.faceName = face;
}
// Symbol: ?SetFontNotifySink@CFontHolder@@QEAAXPEAUIPropertyNotifySink@@@Z
extern "C" void MS_ABI impl__SetFontNotifySink_CFontHolder__QEAAXPEAUIPropertyNotifySink___Z(void* self, void* notifySink) {
    g_fontHolderStates[self].notifySink = notifySink;
}
// Symbol: ?GetFontDispatch@CFontHolder@@QEAAPEAUIFontDisp@@XZ
extern "C" void* MS_ABI impl__GetFontDispatch_CFontHolder__QEAAPEAUIFontDisp__XZ(void* self) {
    return g_fontHolderStates[self].font;
}
// Symbol: ?GetFontHandle@CFontHolder@@QEAAPEAUHFONT__@@XZ
extern "C" HFONT MS_ABI impl__GetFontHandle_CFontHolder__QEAAPEAUHFONT____XZ(void* self) {
    return EnsureHolderFont(self);
}
// Symbol: ?GetFontHandle@CFontHolder@@QEAAPEAUHFONT__@@JJ@Z
extern "C" HFONT MS_ABI impl__GetFontHandle_CFontHolder__QEAAPEAUHFONT____JJ_Z(void* self, long, long) {
    return EnsureHolderFont(self);
}
// Symbol: ?QueryTextMetrics@CFontHolder@@QEAAXPEAUtagTEXTMETRICW@@@Z
extern "C" void MS_ABI impl__QueryTextMetrics_CFontHolder__QEAAXPEAUtagTEXTMETRICW___Z(void* self, TEXTMETRICW* metrics) {
    if (!metrics) return;
    ZeroMemory(metrics, sizeof(*metrics));
    HDC dc = ::GetDC(nullptr);
    if (!dc) return;
    HGDIOBJ oldFont = ::SelectObject(dc, EnsureHolderFont(self));
    ::GetTextMetricsW(dc, metrics);
    if (oldFont) ::SelectObject(dc, oldFont);
    ::ReleaseDC(nullptr, dc);
}
// Symbol: ?Select@CFontHolder@@QEAAPEAVCFont@@PEAVCDC@@JJ@Z
extern "C" CFont* MS_ABI impl__Select_CFontHolder__QEAAPEAVCFont__PEAVCDC__JJ_Z(void* self, CDC* dc, long, long) {
    if (!dc || !dc->GetSafeHdc()) return nullptr;
    ::SelectObject(dc->GetSafeHdc(), EnsureHolderFont(self));
    return nullptr;
}
// Symbol: ?ReleaseFont@CFontHolder@@QEAAXXZ
extern "C" void MS_ABI impl__ReleaseFont_CFontHolder__QEAAXXZ(void* self) {
    FontHolderState& state = g_fontHolderStates[self];
    state.font = nullptr;
    state.hFont = nullptr;
    state.faceName.Empty();
}
// Symbol: ?GetDisplayString@CFontHolder@@QEAAHAEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@@Z
extern "C" int MS_ABI impl__GetDisplayString_CFontHolder__QEAAHAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(void* self, CString* value) {
    if (!value) return FALSE;
    FontHolderState& state = g_fontHolderStates[self];
    if (state.faceName.IsEmpty()) state.faceName = FontFaceFromHandle(EnsureHolderFont(self));
    *value = state.faceName.IsEmpty() ? CString(L"Font") : state.faceName;
    return TRUE;
}
