// CMFCRibbonSlider — OpenMFC implementation.
// Sources: cbarcore.cpp, mfccore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/CbarcoreSupport.h"
#include "detail/MfccoreSupport.h"

// Existing approximation retained: it estimates size from text and fixed
// dimensions. Retail reads m_nWidth, returns height 18, applies system scaling,
// and adds room for zoom buttons (mfc140u RVA 0x123fa0).
// Symbol: ?GetRegularSize@CMFCRibbonSlider@@UEAA?AVCSize@@PEAVCDC@@@Z
extern "C" void MS_ABI impl__GetRegularSize_CMFCRibbonSlider__UEAA_AVCSize__PEAVCDC___Z(void* pRet, const CMFCRibbonSlider* pThis, CDC* /*pDC*/) {
    BuildCSizeResult(pRet, 96 + RibbonTextPixels(pThis), 22);
}
// Existing fallback retained: this only resolves the visual manager and does
// not draw the slider; retail renders the channel, zoom buttons and thumb
// (mfc140u RVA 0x123cf0).
// Symbol: ?OnDraw@CMFCRibbonSlider@@UEAAXPEAVCDC@@@Z
extern "C" void MS_ABI impl__OnDraw_CMFCRibbonSlider__UEAAXPEAVCDC___Z(CMFCRibbonSlider* pThis, CDC* pDC) {
    if (pThis == nullptr || pDC == nullptr) {
        return;
    }
    (void)CMFCVisualManager::GetInstance();
}
// Existing compatibility storage retained. These settings are keyed by command
// ID rather than stored per object as retail does at +0x180..+0x19c (see
// mfc140u RVA 0x123c70); separate sliders with the same ID therefore share state.
// Symbol: ?SetRange@CMFCRibbonSlider@@QEAAXHH@Z
extern "C" void MS_ABI impl__SetRange_CMFCRibbonSlider__QEAAXHH_Z(CMFCRibbonSlider* pThis, int nMin, int nMax) {
    if (!pThis) return;
    std::lock_guard<std::mutex> lock(g_ribbonMutex);
    auto& state = g_sliderStates[pThis->GetID()];
    NormalizeRange(nMin, nMax, state.nMin, state.nMax);
    state.nPos = std::clamp(state.nPos, state.nMin, state.nMax);
}
// Symbol: ?SetPos@CMFCRibbonSlider@@QEAAXHH@Z
extern "C" void MS_ABI impl__SetPos_CMFCRibbonSlider__QEAAXHH_Z(CMFCRibbonSlider* pThis, int nPos, int /*bRedraw*/) {
    if (!pThis) return;
    std::lock_guard<std::mutex> lock(g_ribbonMutex);
    auto& state = g_sliderStates[pThis->GetID()];
    state.nPos = std::clamp(nPos, state.nMin, state.nMax);
}
// Symbol: ?SetZoomButtons@CMFCRibbonSlider@@QEAAXH@Z
extern "C" void MS_ABI impl__SetZoomButtons_CMFCRibbonSlider__QEAAXH_Z(CMFCRibbonSlider* pThis, int bSet) {
    if (!pThis) return;
    std::lock_guard<std::mutex> lock(g_ribbonMutex);
    g_sliderStates[pThis->GetID()].bShowZoomButtons = (bSet != FALSE);
}
// Symbol: ?SetZoomIncrement@CMFCRibbonSlider@@QEAAXH@Z
extern "C" void MS_ABI impl__SetZoomIncrement_CMFCRibbonSlider__QEAAXH_Z(CMFCRibbonSlider* pThis, int nDelta) {
    if (!pThis) return;
    std::lock_guard<std::mutex> lock(g_ribbonMutex);
    g_sliderStates[pThis->GetID()].nZoomIncrement = std::max(1, nDelta);
}
// Existing reduced-layout constructor retained: it clears only the local
// opaque tail. Retail also zeroes four CRects and calls CommonInit
// (mfc140u RVA 0x123b10); those slider members are not present locally.
CMFCRibbonSlider::CMFCRibbonSlider() { memset(_pad, 0, sizeof(_pad)); }
CMFCRibbonSlider::~CMFCRibbonSlider() {}
// Retail initialization writes the slider's range, mode and four CRect members
// at +0x180..+0x1df (mfc140u RVA 0x123c70). The local class declaration has only
// a 32-byte opaque tail and no matching member layout, so touching those offsets
// would overrun/misrepresent the declared object. See headerRequests.
// Symbol: ?CommonInit@CMFCRibbonSlider@@IEAAXXZ
extern "C" void MS_ABI impl__CommonInit_CMFCRibbonSlider__IEAAXXZ(CMFCRibbonSlider* pThis) {
    (void)pThis;
}

// Retail calls the base CopyFrom thunk then copies slider fields at +0x180,
// +0x184, +0x188, +0x19c and +0x190 (mfc140u RVA 0x123f40). Those members
// are absent from the local class layout, so copying them is not safe here.
// Symbol: ?CopyFrom@CMFCRibbonSlider@@MEAAXAEBVCMFCRibbonBaseElement@@@Z
extern "C" void MS_ABI impl__CopyFrom_CMFCRibbonSlider__MEAAXAEBVCMFCRibbonBaseElement___Z(
    CMFCRibbonSlider* pThis, const CMFCRibbonBaseElement& src) {
    (void)pThis;
    (void)src;
}

// Retail hit-testing checks the thumb, slider, zoom-out and zoom-in CRect
// members at +0x1d0, +0x1c0, +0x1a0 and +0x1b0, in that order (mfc140u RVA
// 0x124520). The local class has no corresponding rectangles.
// Symbol: ?GetHitTest@CMFCRibbonSlider@@IEBAHVCPoint@@@Z
extern "C" int MS_ABI impl__GetHitTest_CMFCRibbonSlider__IEBAHVCPoint___Z(
    const CMFCRibbonSlider* pThis, long long point) {
    (void)pThis;
    (void)point;
    return 0;
}

// Retail maps the supplied x coordinate through the slider rect and min/max
// range (mfc140u RVA 0x1245a0); the required range and rectangle fields are
// not modeled in CMFCRibbonSlider here.
// Symbol: ?GetPosFromPoint@CMFCRibbonSlider@@IEAAHH@Z
extern "C" int MS_ABI impl__GetPosFromPoint_CMFCRibbonSlider__IEAAHH_Z(
    CMFCRibbonSlider* pThis, int x) {
    (void)pThis;
    (void)x;
    return 0;
}

// Retail returns whether m_nPressed at +0x198 is 2 or 3 and does not write
// the delay reference (mfc140u RVA 0x124630). The pressing-mode member is absent
// from the local class layout.
// Symbol: ?IsAutoRepeatMode@CMFCRibbonSlider@@MEBAHAEAH@Z
extern "C" int MS_ABI impl__IsAutoRepeatMode_CMFCRibbonSlider__MEBAHAEAH_Z(
    const CMFCRibbonSlider* pThis, int* pDelay) {
    (void)pThis;
    (void)pDelay;
    return 0;
}

// Retail routes WM_COMMAND using the base command ID at +0x118 (mfc140u RVA
// 0x1246b0); its corresponding ANSI-twin import resolves to USER32!SendMessageA
// (the Unicode image uses SendMessageW). Reproducing the parent/window lookup
// requires base state and infrastructure not represented by this slider layout.
// Symbol: ?NotifyCommand@CMFCRibbonSlider@@IEAAXXZ
extern "C" void MS_ABI impl__NotifyCommand_CMFCRibbonSlider__IEAAXXZ(CMFCRibbonSlider* pThis) {
    (void)pThis;
}

// Retail adjusts the four slider rectangles after a base rect change and uses
// inherited state plus system metrics (mfc140u RVA 0x1240e0). The slider rect
// members are not represented in the local declaration.
// Symbol: ?OnAfterChangeRect@CMFCRibbonSlider@@MEAAXPEAVCDC@@@Z
extern "C" void MS_ABI impl__OnAfterChangeRect_CMFCRibbonSlider__MEAAXPEAVCDC___Z(
    CMFCRibbonSlider* pThis, CDC* pDC) {
    (void)pThis;
    (void)pDC;
}

// Retail returns FALSE when the inherited disabled flag at +0x148 is set. For
// pressed modes 2/3 at +0x198 it adjusts the position by the increment at +0x18c
// and notifies the command; other enabled modes do nothing but return TRUE
// (mfc140u RVA 0x124650). These slider fields are not modeled locally.
// Symbol: ?OnAutoRepeat@CMFCRibbonSlider@@MEAAHXZ
extern "C" int MS_ABI impl__OnAutoRepeat_CMFCRibbonSlider__MEAAHXZ(CMFCRibbonSlider* pThis) {
    (void)pThis;
    return 0;
}

// The retail body performs slider list drawing through visual-manager paths
// and temporarily changes inherited drawing state (mfc140u RVA 0x124710).
// Neither the slider fields nor a compatible draw-state layout is available.
// Symbol: ?OnDrawOnList@CMFCRibbonSlider@@MEAAXPEAVCDC@@V?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@HVCRect@@HH@Z
extern "C" void MS_ABI impl__OnDrawOnList_CMFCRibbonSlider__MEAAXPEAVCDC__V__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__HVCRect__HH_Z(
    CMFCRibbonSlider* pThis, CDC* pDC, CString strText, int nTextOffset,
    CRect rect, int bIsHighlighted, int bIsDisabled) {
    (void)pThis;
    (void)pDC;
    (void)strText;
    (void)nTextOffset;
    (void)rect;
    (void)bIsHighlighted;
    (void)bIsDisabled;
}

// The ANSI-twin disassembly (mfc140.dll RVA 0x1250f0; no mfc140u mapping was
// resolved) delegates to inherited/input virtual behavior and updates slider
// interaction state. The missing members prevent a faithful implementation.
// Symbol: ?OnLButtonDown@CMFCRibbonSlider@@MEAAXVCPoint@@@Z
extern "C" void MS_ABI impl__OnLButtonDown_CMFCRibbonSlider__MEAAXVCPoint___Z(
    CMFCRibbonSlider* pThis, long long point) {
    (void)pThis;
    (void)point;
}

// The ANSI-twin disassembly (mfc140.dll RVA 0x125150; no mfc140u mapping was
// resolved) applies the release action from the slider's pressed mode and may
// update the position/notify the command; those fields are not modeled.
// Symbol: ?OnLButtonUp@CMFCRibbonSlider@@MEAAXVCPoint@@@Z
extern "C" void MS_ABI impl__OnLButtonUp_CMFCRibbonSlider__MEAAXVCPoint___Z(
    CMFCRibbonSlider* pThis, long long point) {
    (void)pThis;
    (void)point;
}

// The ANSI-twin disassembly (mfc140.dll RVA 0x1251e0; no mfc140u mapping was
// resolved) updates the highlighted/position state and can notify the command.
// The corresponding slider and inherited state are not available locally.
// Symbol: ?OnMouseMove@CMFCRibbonSlider@@MEAAXVCPoint@@@Z
extern "C" void MS_ABI impl__OnMouseMove_CMFCRibbonSlider__MEAAXVCPoint___Z(
    CMFCRibbonSlider* pThis, long long point) {
    (void)pThis;
    (void)point;
}

// The ANSI-twin disassembly (mfc140.dll RVA 0x125640; no mfc140u mapping was
// resolved) first delegates to accessibility setup, then fills slider role,
// value and name data. Position exists only in the ID-keyed compatibility state;
// the retail base accessibility setup and the rest of the slider layout are not
// modeled here.
// Symbol: ?SetACCData@CMFCRibbonSlider@@MEAAHPEAVCWnd@@AEAVCAccessibilityData@@@Z
extern "C" int MS_ABI impl__SetACCData_CMFCRibbonSlider__MEAAHPEAVCWnd__AEAVCAccessibilityData___Z(
    CMFCRibbonSlider* pThis, CWnd* pParent, CAccessibilityData& data) {
    (void)pThis;
    (void)pParent;
    (void)data;
    return 0;
}

// The ANSI-twin disassembly (mfc140.dll RVA 0x124fb0; no mfc140u mapping was
// resolved) computes and stores m_rectThumb using the slider range, rect and
// display metrics. The required members are absent from this class declaration.
// Symbol: ?SetThumbRect@CMFCRibbonSlider@@IEAAXXZ
extern "C" void MS_ABI impl__SetThumbRect_CMFCRibbonSlider__IEAAXXZ(CMFCRibbonSlider* pThis) {
    (void)pThis;
}

