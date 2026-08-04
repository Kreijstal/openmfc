// CMFCStatusBar — OpenMFC implementation.
// Sources: cbarcore.cpp, mfccore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/CbarcoreSupport.h"
#include "detail/MfccoreSupport.h"

// Symbol: ?SetPaneText@CMFCStatusBar@@UEAAHHPEB_WH@Z
extern "C" int MS_ABI impl__SetPaneText_CMFCStatusBar__UEAAHHPEB_WH_Z(
    CMFCStatusBar* pThis, int nIndex, const wchar_t* lpszNewText, int bUpdate) {
    if (!pThis || nIndex < 0) {
        return FALSE;
    }

    {
        std::lock_guard<std::mutex> lock(g_statusBarTextMutex);
        g_statusBarPaneText[pThis][nIndex] = lpszNewText ? lpszNewText : L"";
    }

    return (int)static_cast<CStatusBar*>(pThis)->SetPaneText(nIndex, lpszNewText, bUpdate);
}
// Symbol: ?GetPaneText@CMFCStatusBar@@QEBA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@H@Z
extern "C" void MS_ABI impl__GetPaneText_CMFCStatusBar__QEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__H_Z(
    CString* __ret, const CMFCStatusBar* pThis, int nIndex) {
    if (!__ret) {
        return;
    }

    CString text;
    if (pThis) {
        text = static_cast<const CStatusBar*>(pThis)->GetPaneText(nIndex);
    }

    if (text.IsEmpty() && pThis) {
        text = GetStoredPaneText(pThis, nIndex);
    }

    new(__ret) CString(text);
}
// Symbol: ?GetPaneText@CMFCStatusBar@@QEBAXHAEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@@Z
extern "C" void MS_ABI impl__GetPaneText_CMFCStatusBar__QEBAXHAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
    const CMFCStatusBar* pThis, int nIndex, CString* rString) {
    if (!rString) {
        return;
    }

    if (pThis) {
        static_cast<const CStatusBar*>(pThis)->GetPaneText(nIndex, *rString);
    } else {
        *rString = CString();
    }

    if (rString->IsEmpty() && pThis) {
        *rString = GetStoredPaneText(pThis, nIndex);
    }
}
// Symbol: ?EnablePaneProgressBar@CMFCStatusBar@@QEAAXHJHKKK@Z
extern "C" void MS_ABI impl__EnablePaneProgressBar_CMFCStatusBar__QEAAXHJHKKK_Z(
    CMFCStatusBar* /*pThis*/, int nIndex, long nTotal, int nMax,
    unsigned long clrBar, unsigned long clrProgressBarDest, unsigned long clrProgressText) {
    std::lock_guard<std::mutex> lock(g_statusBarProgressMutex);
    auto& st = g_statusBarProgress[nIndex];
    st.nTotal = nTotal;
    st.nMax = nMax;
    st.clrBar = clrBar;
    st.clrProgressBarDest = clrProgressBarDest;
    st.clrProgressText = clrProgressText;
    st.bEnabled = true;
}
// Symbol: ?GetPaneProgress@CMFCStatusBar@@QEBAJH@Z
extern "C" long MS_ABI impl__GetPaneProgress_CMFCStatusBar__QEBAJH_Z(
    const CMFCStatusBar* /*pThis*/, int nIndex) {
    std::lock_guard<std::mutex> lock(g_statusBarProgressMutex);
    auto it = g_statusBarProgress.find(nIndex);
    return (it != g_statusBarProgress.end()) ? it->second.nProgress : 0;
}
// Symbol: ?SetPaneProgress@CMFCStatusBar@@QEAAXHJH@Z
extern "C" void MS_ABI impl__SetPaneProgress_CMFCStatusBar__QEAAXHJH_Z(
    CMFCStatusBar* /*pThis*/, int nIndex, long nProgress, int /*bRedraw*/) {
    std::lock_guard<std::mutex> lock(g_statusBarProgressMutex);
    auto& st = g_statusBarProgress[nIndex];
    st.nProgress = nProgress;
}
CMFCStatusBar::CMFCStatusBar() { memset(_pad, 0, sizeof(_pad)); }
CMFCStatusBar::~CMFCStatusBar() {}
