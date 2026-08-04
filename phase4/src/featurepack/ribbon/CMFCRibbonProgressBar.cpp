// CMFCRibbonProgressBar — OpenMFC implementation.
// Sources: cbarcore.cpp, mfccore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/CbarcoreSupport.h"
#include "detail/MfccoreSupport.h"

// Symbol: ?GetRegularSize@CMFCRibbonProgressBar@@UEAA?AVCSize@@PEAVCDC@@@Z
extern "C" void MS_ABI impl__GetRegularSize_CMFCRibbonProgressBar__UEAA_AVCSize__PEAVCDC___Z(void* pRet, const CMFCRibbonProgressBar* pThis, CDC* /*pDC*/) {
    BuildCSizeResult(pRet, 96 + RibbonTextPixels(pThis), 16);
}
// Symbol: ?OnDraw@CMFCRibbonProgressBar@@UEAAXPEAVCDC@@@Z
extern "C" void MS_ABI impl__OnDraw_CMFCRibbonProgressBar__UEAAXPEAVCDC___Z(CMFCRibbonProgressBar* pThis, CDC* pDC) {
    if (pThis == nullptr || pDC == nullptr) {
        return;
    }
    (void)CMFCVisualManager::GetInstance();
}
// Symbol: ?SetRange@CMFCRibbonProgressBar@@QEAAXHH@Z
extern "C" void MS_ABI impl__SetRange_CMFCRibbonProgressBar__QEAAXHH_Z(CMFCRibbonProgressBar* pThis, int nMin, int nMax) {
    if (!pThis) return;
    std::lock_guard<std::mutex> lock(g_ribbonMutex);
    auto& state = g_progressStates[pThis->GetID()];
    NormalizeRange(nMin, nMax, state.nMin, state.nMax);
    state.nPos = std::clamp(state.nPos, state.nMin, state.nMax);
}
// Symbol: ?SetPos@CMFCRibbonProgressBar@@QEAAXHH@Z
extern "C" void MS_ABI impl__SetPos_CMFCRibbonProgressBar__QEAAXHH_Z(CMFCRibbonProgressBar* pThis, int nPos, int /*bRedraw*/) {
    if (!pThis) return;
    std::lock_guard<std::mutex> lock(g_ribbonMutex);
    auto& state = g_progressStates[pThis->GetID()];
    state.nPos = std::clamp(nPos, state.nMin, state.nMax);
}
// Symbol: ?SetInfiniteMode@CMFCRibbonProgressBar@@QEAAXH@Z
extern "C" void MS_ABI impl__SetInfiniteMode_CMFCRibbonProgressBar__QEAAXH_Z(CMFCRibbonProgressBar* pThis, int bSet) {
    if (!pThis) return;
    std::lock_guard<std::mutex> lock(g_ribbonMutex);
    g_progressStates[pThis->GetID()].bInfinite = (bSet != FALSE);
}
// Symbol: ?CommonInit@CMFCRibbonProgressBar@@IEAAXXZ
extern "C" void MS_ABI impl__CommonInit_CMFCRibbonProgressBar__IEAAXXZ(CMFCRibbonProgressBar* pThis) {
    if (!pThis) return;
    std::lock_guard<std::mutex> lock(g_ribbonMutex);
    g_progressStates.try_emplace(pThis->GetID(), RibbonProgressState());
}
// Symbol: ?CopyFrom@CMFCRibbonProgressBar@@MEAAXAEBVCMFCRibbonBaseElement@@@Z
extern "C" void MS_ABI impl__CopyFrom_CMFCRibbonProgressBar__MEAAXAEBVCMFCRibbonBaseElement___Z(
    CMFCRibbonProgressBar* pThis, const CMFCRibbonBaseElement* src) {
    if (!pThis || !src) return;
    std::lock_guard<std::mutex> lock(g_ribbonMutex);
    // Copy progress state from source if it has one
    auto it = g_progressStates.find(src->GetID());
    if (it != g_progressStates.end()) {
        g_progressStates[pThis->GetID()] = it->second;
    } else {
        g_progressStates[pThis->GetID()] = RibbonProgressState();
    }
}
// Symbol: ?OnDrawOnList@CMFCRibbonProgressBar@@MEAAXPEAVCDC@@V?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@HVCRect@@HH@Z
extern "C" void MS_ABI impl__OnDrawOnList_CMFCRibbonProgressBar__MEAAXPEAVCDC__V__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__HVCRect__HH_Z(
    CMFCRibbonProgressBar* pThis, CDC* pDC, void* /*CStringT<wchar_t> by-val*/, int /*nTextOffset*/,
    CRect rect, int bIsHighlighted, int bIsDisabled) {
    if (!pThis || !pDC || !pDC->GetSafeHdc() || rect.Width() <= 0 || rect.Height() <= 0) return;

    RibbonProgressState state;
    {
        std::lock_guard<std::mutex> lock(g_ribbonMutex);
        auto it = g_progressStates.find(pThis->GetID());
        if (it != g_progressStates.end()) state = it->second;
    }

    CRect chunk = rect;
    if (!state.bInfinite) {
        const int range = state.nMax - state.nMin;
        const int completed = range > 0 ? std::clamp(state.nPos - state.nMin, 0, range) : 0;
        chunk.right = chunk.left + (range > 0 ? ::MulDiv(rect.Width(), completed, range) : 0);
    }

    CMFCVisualManager::GetInstance()->OnDrawRibbonProgressBar(
        pDC, pThis, rect, chunk, state.bInfinite ? TRUE : FALSE);

    const RECT drawRect = { rect.left, rect.top, rect.right, rect.bottom };
    if (bIsDisabled) {
        HBRUSH hatch = ::CreateHatchBrush(HS_BDIAGONAL, ::GetSysColor(COLOR_GRAYTEXT));
        if (hatch) {
            ::SetBkMode(pDC->GetSafeHdc(), TRANSPARENT);
            ::FillRect(pDC->GetSafeHdc(), &drawRect, hatch);
            ::DeleteObject(hatch);
        }
    } else if (bIsHighlighted) {
        ::DrawFocusRect(pDC->GetSafeHdc(), &drawRect);
    }
}
// Symbol: ?SetACCData@CMFCRibbonProgressBar@@MEAAHPEAVCWnd@@AEAVCAccessibilityData@@@Z
extern "C" int MS_ABI impl__SetACCData_CMFCRibbonProgressBar__MEAAHPEAVCWnd__AEAVCAccessibilityData___Z(
    CMFCRibbonProgressBar* pThis, CWnd* pParentWnd, void* data) {
    if (!pThis || !data) return FALSE;

    RibbonProgressState state;
    {
        std::lock_guard<std::mutex> lock(g_ribbonMutex);
        auto it = g_progressStates.find(pThis->GetID());
        if (it != g_progressStates.end()) state = it->second;
    }

    CAccessibilityData& acc = *static_cast<CAccessibilityData*>(data);
    acc.m_strAccName = pThis->GetText();
    if (acc.m_strAccName.IsEmpty()) acc.m_strAccName = L"Progress";

    const int range = state.nMax - state.nMin;
    const int completed = range > 0 ? std::clamp(state.nPos - state.nMin, 0, range) : 0;
    const int percent = range > 0 ? ::MulDiv(completed, 100, range) : 0;
    wchar_t value[16] = {};
    _snwprintf(value, sizeof(value) / sizeof(value[0]) - 1, L"%d%%", percent);
    acc.m_strAccValue = value;
    acc.m_nAccRole = ROLE_SYSTEM_PROGRESSBAR;
    acc.m_bAccState = STATE_SYSTEM_READONLY;
    acc.m_nAccHit = CHILDID_SELF;

    HWND parent = pParentWnd ? pParentWnd->GetSafeHwnd() : nullptr;
    RECT location = {};
    if (parent && ::GetClientRect(parent, &location)) {
        ::MapWindowPoints(parent, nullptr, reinterpret_cast<POINT*>(&location), 2);
        acc.m_rectAccLocation = location;
    } else {
        acc.m_rectAccLocation.SetRectEmpty();
    }
    return TRUE;
}
CMFCRibbonProgressBar::CMFCRibbonProgressBar() { memset(_pad, 0, sizeof(_pad)); }
CMFCRibbonProgressBar::~CMFCRibbonProgressBar() {}
void CMFCRibbonProgressBar::OnDraw(CDC* pDC) { (void)pDC; }
CSize CMFCRibbonProgressBar::GetRegularSize(CDC* pDC) const { (void)pDC; return CSize(0, 0); }
void CMFCRibbonProgressBar::CopyFrom(const CMFCRibbonBaseElement& src) { (void)src; }
void CMFCRibbonProgressBar::OnDrawOnList(CDC* pDC, const CString& strText, int nTextOffset, CRect rect, int bIsHighlighted, int bIsDisabled) {
    (void)pDC; (void)strText; (void)nTextOffset; (void)rect; (void)bIsHighlighted; (void)bIsDisabled;
}
int CMFCRibbonProgressBar::SetACCData(CWnd* pParentWnd, CAccessibilityData& data) {
    (void)pParentWnd;

    data.m_strAccName = GetText();
    if (data.m_strAccName.IsEmpty()) {
        data.m_strAccName = L"Progress";
    }

    data.m_nAccRole = ROLE_SYSTEM_PROGRESSBAR;
    data.m_bAccState = STATE_SYSTEM_READONLY;
    data.m_nAccHit = CHILDID_SELF;
    data.m_strAccValue.Empty();
    data.m_strDescription.Empty();
    data.m_strAccKeys.Empty();
    data.m_strAccHelp.Empty();
    data.m_strAccDefAction.Empty();
    return TRUE;
}
