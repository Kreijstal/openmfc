// CMFCRibbonStatusBarPane — OpenMFC implementation.
// Sources: cbarcore.cpp, mfccore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/CbarcoreSupport.h"
#include "detail/MfccoreSupport.h"

// Symbol: ??0CMFCRibbonStatusBarPane@@QEAA@IPEB_WHPEAUHICON__@@0H@Z
extern "C" void* MS_ABI impl___0CMFCRibbonStatusBarPane__QEAA_IPEB_WHPEAUHICON____0H_Z(
    void* pThis, UINT nID, const wchar_t* lpszText, int /*bIsStatic*/, HICON /*hIcon*/, HICON /*hIconDisabled*/, int /*nTextAlign*/) {
    if (!pThis) return nullptr;
    auto* pPane = new(pThis) CMFCRibbonStatusBarPane();
    pPane->SetID(nID);
    pPane->SetText(lpszText);
    return pPane;
}
// Symbol: ??0CMFCRibbonStatusBarPane@@QEAA@IPEB_WIHKPEAUHICON__@@HH@Z
extern "C" void* MS_ABI impl___0CMFCRibbonStatusBarPane__QEAA_IPEB_WIHKPEAUHICON____HH_Z(
    void* pThis, UINT nID, const wchar_t* lpszText, UINT /*cxText*/, int /*bAlmostLargeText*/, DWORD /*dwStyle*/,
    HICON /*hIcon*/, int /*nTextAlign*/, int /*bIsExtended*/) {
    if (!pThis) return nullptr;
    auto* pPane = new(pThis) CMFCRibbonStatusBarPane();
    pPane->SetID(nID);
    pPane->SetText(lpszText);
    return pPane;
}
// Symbol: ??0CMFCRibbonStatusBarPane@@QEAA@IPEB_WPEAUHBITMAP__@@HKPEAUHICON__@@HH@Z
extern "C" void* MS_ABI impl___0CMFCRibbonStatusBarPane__QEAA_IPEB_WPEAUHBITMAP____HKPEAUHICON____HH_Z(
    void* pThis, UINT nID, const wchar_t* lpszText, HBITMAP hBmpAnimation, int cxAnimation, DWORD dwAnimationSpeed,
    HICON /*hIcon*/, int /*nTextAlign*/, int /*bIsExtended*/) {
    if (!pThis) return nullptr;
    auto* pPane = new(pThis) CMFCRibbonStatusBarPane();
    pPane->SetID(nID);
    pPane->SetText(lpszText);
    auto& state = g_statusPaneStates[pPane->GetID()];
    state.hAnimationBitmap = hBmpAnimation;
    state.cxAnimation = cxAnimation;
    state.dwAnimationDelay = dwAnimationSpeed;
    return pPane;
}
// Symbol: ?GetIntermediateSize@CMFCRibbonStatusBarPane@@MEAA?AVCSize@@PEAVCDC@@@Z
extern "C" void MS_ABI impl__GetIntermediateSize_CMFCRibbonStatusBarPane__MEAA_AVCSize__PEAVCDC___Z(void* pRet, const CMFCRibbonStatusBarPane* pThis, CDC* /*pDC*/) {
    BuildCSizeResult(pRet, 96 + RibbonTextPixels(pThis), 22);
}
// Symbol: ?OnCalcTextSize@CMFCRibbonStatusBarPane@@MEAAXPEAVCDC@@@Z
extern "C" void MS_ABI impl__OnCalcTextSize_CMFCRibbonStatusBarPane__MEAAXPEAVCDC___Z(CMFCRibbonStatusBarPane* pThis, CDC* pDC) {
    if (!pThis) return;
    std::lock_guard<std::mutex> lock(g_ribbonMutex);
    g_ribbonTextSizeById[pThis] = MeasureRibbonText(pThis, pDC);
}
// Symbol: ?OnDraw@CMFCRibbonStatusBarPane@@MEAAXPEAVCDC@@@Z
extern "C" void MS_ABI impl__OnDraw_CMFCRibbonStatusBarPane__MEAAXPEAVCDC___Z(CMFCRibbonStatusBarPane* pThis, CDC* pDC) {
    if (pThis == nullptr || pDC == nullptr) {
        return;
    }
    if (CMFCVisualManager* manager = CMFCVisualManager::GetInstance(); manager != nullptr) {
        manager->OnDrawRibbonStatusBarPane(pDC, nullptr, pThis);
    }
}
// Symbol: ?OnFillBackground@CMFCRibbonStatusBarPane@@UEAAKPEAVCDC@@@Z
extern "C" unsigned long MS_ABI impl__OnFillBackground_CMFCRibbonStatusBarPane__UEAAKPEAVCDC___Z(CMFCRibbonStatusBarPane* /*pThis*/, CDC* /*pDC*/) {
    const COLORREF color = SystemColor(COLOR_BTNFACE);
    return color;
}
// Symbol: ?GetToolTipText@CMFCRibbonStatusBarPane@@MEBA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@XZ
extern "C" void MS_ABI impl__GetToolTipText_CMFCRibbonStatusBarPane__MEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__XZ(
    void* pRet, const CMFCRibbonStatusBarPane* pThis) {
    if (!pThis) {
        BuildCStringResult(pRet, L"");
        return;
    }
    std::lock_guard<std::mutex> lock(g_ribbonMutex);
    auto it = g_ribbonToolTips.find(RibbonElementID(pThis));
    if (it != g_ribbonToolTips.end()) {
        BuildCStringResult(pRet, it->second);
        return;
    }
    CString text = pThis->GetText();
    BuildCStringResult(pRet, (const wchar_t*)text);
}
// Symbol: ?SetAnimationList@CMFCRibbonStatusBarPane@@QEAAHIHK@Z
extern "C" int MS_ABI impl__SetAnimationList_CMFCRibbonStatusBarPane__QEAAHIHK_Z(
    CMFCRibbonStatusBarPane* pThis, UINT nAnimationListResID, int cxAnimation, DWORD dwAnimationSpeed) {
    if (!pThis) return FALSE;
    std::lock_guard<std::mutex> lock(g_ribbonMutex);
    auto& state = g_statusPaneStates[pThis->GetID()];
    state.hAnimationBitmap = nullptr;
    state.cxAnimation = cxAnimation;
    state.dwAnimationDelay = dwAnimationSpeed;
    state.nAnimationCount = 0;
    return (nAnimationListResID != 0);
}
// Symbol: ?SetAnimationList@CMFCRibbonStatusBarPane@@QEAAXPEAUHBITMAP__@@HK@Z
extern "C" void MS_ABI impl__SetAnimationList_CMFCRibbonStatusBarPane__QEAAXPEAUHBITMAP____HK_Z(
    CMFCRibbonStatusBarPane* pThis, HBITMAP hAnimationList, int cxAnimation, DWORD dwAnimationSpeed) {
    if (!pThis) return;
    std::lock_guard<std::mutex> lock(g_ribbonMutex);
    auto& state = g_statusPaneStates[pThis->GetID()];
    state.hAnimationBitmap = hAnimationList;
    state.cxAnimation = cxAnimation;
    state.dwAnimationDelay = dwAnimationSpeed;
    state.nAnimationCount = 0;
}
// Symbol: ?StartAnimation@CMFCRibbonStatusBarPane@@QEAAXII@Z
extern "C" void MS_ABI impl__StartAnimation_CMFCRibbonStatusBarPane__QEAAXII_Z(
    CMFCRibbonStatusBarPane* pThis, UINT nAnimationDuration, UINT nAnimationDelay) {
    if (!pThis) return;
    std::lock_guard<std::mutex> lock(g_ribbonMutex);
    auto& state = g_statusPaneStates[pThis->GetID()];
    state.bAnimating = true;
    state.nAnimationFrame = 0;
    state.nAnimationCount = nAnimationDuration;
    if (nAnimationDelay != 0) state.dwAnimationDelay = nAnimationDelay;
}
// Symbol: ?StopAnimation@CMFCRibbonStatusBarPane@@QEAAXXZ
extern "C" void MS_ABI impl__StopAnimation_CMFCRibbonStatusBarPane__QEAAXXZ(CMFCRibbonStatusBarPane* pThis) {
    if (!pThis) return;
    std::lock_guard<std::mutex> lock(g_ribbonMutex);
    g_statusPaneStates[pThis->GetID()].bAnimating = false;
}
CMFCRibbonStatusBarPane::CMFCRibbonStatusBarPane() { memset(_pad, 0, sizeof(_pad)); }
CMFCRibbonStatusBarPane::~CMFCRibbonStatusBarPane() {}
