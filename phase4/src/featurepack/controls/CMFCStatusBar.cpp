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
// Symbol: ?CalcFixedLayout@CMFCStatusBar@@UEAA?AVCSize@@HH@Z
extern "C" void* MS_ABI impl__CalcFixedLayout_CMFCStatusBar__UEAA_AVCSize__HH_Z(void* /*class*/* p0, int p1, int p2) {
    return nullptr;
}

// Symbol: ?CommandToIndex@CMFCStatusBar@@QEBAHI@Z
extern "C" int MS_ABI impl__CommandToIndex_CMFCStatusBar__QEBAHI_Z(unsigned int p0) {
    return 0;
}

// Symbol: ?Create@CMFCStatusBar@@QEAAHPEAVCWnd@@KI@Z
extern "C" int MS_ABI impl__Create_CMFCStatusBar__QEAAHPEAVCWnd__KI_Z(void* /*class*/* p0, unsigned long p1, unsigned int p2) {
    return 0;
}

// Symbol: ?CreateEx@CMFCStatusBar@@QEAAHPEAVCWnd@@KKI@Z
extern "C" int MS_ABI impl__CreateEx_CMFCStatusBar__QEAAHPEAVCWnd__KKI_Z(void* /*class*/* p0, unsigned long p1, unsigned long p2, unsigned int p3) {
    return 0;
}

// Symbol: ?DoPaint@CMFCStatusBar@@MEAAXPEAVCDC@@@Z
extern "C" void MS_ABI impl__DoPaint_CMFCStatusBar__MEAAXPEAVCDC___Z(void* /*class*/* p0) {}

// Symbol: ?EnablePaneDoubleClick@CMFCStatusBar@@QEAAXH@Z
extern "C" void MS_ABI impl__EnablePaneDoubleClick_CMFCStatusBar__QEAAXH_Z(int p0) {}

// Symbol: ?GetCurrentFont@CMFCStatusBar@@IEBAPEAUHFONT__@@XZ
extern "C" void* MS_ABI impl__GetCurrentFont_CMFCStatusBar__IEBAPEAUHFONT____XZ() {
    return nullptr;
}

// Symbol: ?GetExtendedArea@CMFCStatusBar@@UEBAHAEAVCRect@@@Z
extern "C" int MS_ABI impl__GetExtendedArea_CMFCStatusBar__UEBAHAEAVCRect___Z(void* /*class*/* p0) {
    return 0;
}

// Symbol: ?GetItemID@CMFCStatusBar@@QEBAIH@Z
extern "C" unsigned int MS_ABI impl__GetItemID_CMFCStatusBar__QEBAIH_Z(int p0) {
    return 0;
}

// Symbol: ?GetItemRect@CMFCStatusBar@@QEBAXHPEAUtagRECT@@@Z
extern "C" void MS_ABI impl__GetItemRect_CMFCStatusBar__QEBAXHPEAUtagRECT___Z(int p0, void* /*struct*/* p1) {}

// Symbol: ?GetPaneInfo@CMFCStatusBar@@QEBAXHAEAI0AEAH@Z
extern "C" void MS_ABI impl__GetPaneInfo_CMFCStatusBar__QEBAXHAEAI0AEAH_Z(int p0, unsigned int* p1, int p2, int* p3) {}

// Symbol: ?GetPaneStyle@CMFCStatusBar@@QEBAIH@Z
extern "C" unsigned int MS_ABI impl__GetPaneStyle_CMFCStatusBar__QEBAIH_Z(int p0) {
    return 0;
}

// Symbol: ?GetPaneWidth@CMFCStatusBar@@QEBAHH@Z
extern "C" int MS_ABI impl__GetPaneWidth_CMFCStatusBar__QEBAHH_Z(int p0) {
    return 0;
}

// Symbol: ?GetTipText@CMFCStatusBar@@QEBA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@H@Z
extern "C" void* MS_ABI impl__GetTipText_CMFCStatusBar__QEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__H_Z(void* /*class*/* p0, void** p1, void* p2, int p3) {
    return nullptr;
}

// Symbol: ?HitTest@CMFCStatusBar@@IEBAPEAVCMFCStatusBarPaneInfo@@VCPoint@@@Z
extern "C" void* MS_ABI impl__HitTest_CMFCStatusBar__IEBAPEAVCMFCStatusBarPaneInfo__VCPoint___Z(void* /*class*/ p0) {
    return nullptr;
}

// Symbol: ?InvalidatePaneContent@CMFCStatusBar@@QEAAXH@Z
extern "C" void MS_ABI impl__InvalidatePaneContent_CMFCStatusBar__QEAAXH_Z(int p0) {}

// Symbol: ?OnCreate@CMFCStatusBar@@IEAAHPEAUtagCREATESTRUCTW@@@Z
extern "C" int MS_ABI impl__OnCreate_CMFCStatusBar__IEAAHPEAUtagCREATESTRUCTW___Z(void* /*struct*/* p0) {
    return 0;
}

// Symbol: ?OnDestroy@CMFCStatusBar@@IEAAXXZ
extern "C" void MS_ABI impl__OnDestroy_CMFCStatusBar__IEAAXXZ() {}

// Symbol: ?OnDrawPane@CMFCStatusBar@@MEAAXPEAVCDC@@PEAVCMFCStatusBarPaneInfo@@@Z
extern "C" void MS_ABI impl__OnDrawPane_CMFCStatusBar__MEAAXPEAVCDC__PEAVCMFCStatusBarPaneInfo___Z(void* /*class*/* p0, void* /*class*/* p1) {}

// Symbol: ?OnGetFont@CMFCStatusBar@@IEAAPEAUHFONT__@@XZ
extern "C" void* MS_ABI impl__OnGetFont_CMFCStatusBar__IEAAPEAUHFONT____XZ() {
    return nullptr;
}

// Symbol: ?OnGetText@CMFCStatusBar@@IEAAHHPEA_W@Z
extern "C" int MS_ABI impl__OnGetText_CMFCStatusBar__IEAAHHPEA_W_Z(int p0, wchar_t* p1) {
    return 0;
}

// Symbol: ?OnGetTextLength@CMFCStatusBar@@IEAAIXZ
extern "C" unsigned int MS_ABI impl__OnGetTextLength_CMFCStatusBar__IEAAIXZ() {
    return 0;
}

// Symbol: ?OnLButtonDblClk@CMFCStatusBar@@IEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnLButtonDblClk_CMFCStatusBar__IEAAXIVCPoint___Z(unsigned int p0, void* /*class*/ p1) {}

// Symbol: ?OnNcHitTest@CMFCStatusBar@@IEAA_JVCPoint@@@Z
extern "C" __int64 MS_ABI impl__OnNcHitTest_CMFCStatusBar__IEAA_JVCPoint___Z(void* /*class*/ p0) {
    return 0;
}

// Symbol: ?OnSetFont@CMFCStatusBar@@IEAAXPEAVCFont@@H@Z
extern "C" void MS_ABI impl__OnSetFont_CMFCStatusBar__IEAAXPEAVCFont__H_Z(void* /*class*/* p0, int p1) {}

// Symbol: ?OnSetText@CMFCStatusBar@@IEAAHPEB_W@Z
extern "C" int MS_ABI impl__OnSetText_CMFCStatusBar__IEAAHPEB_W_Z(const wchar_t* p0) {
    return 0;
}

// Symbol: ?OnSettingChange@CMFCStatusBar@@IEAAXIPEB_W@Z
extern "C" void MS_ABI impl__OnSettingChange_CMFCStatusBar__IEAAXIPEB_W_Z(unsigned int p0, const wchar_t* p1) {}

// Symbol: ?OnShowWindow@CMFCStatusBar@@IEAAXHI@Z
extern "C" void MS_ABI impl__OnShowWindow_CMFCStatusBar__IEAAXHI_Z(int p0, unsigned int p1) {}

// Symbol: ?OnSize@CMFCStatusBar@@IEAAXIHH@Z
extern "C" void MS_ABI impl__OnSize_CMFCStatusBar__IEAAXIHH_Z(unsigned int p0, int p1, int p2) {}

// Symbol: ?OnStyleChanged@CMFCStatusBar@@IEAAXHPEAUtagSTYLESTRUCT@@@Z
extern "C" void MS_ABI impl__OnStyleChanged_CMFCStatusBar__IEAAXHPEAUtagSTYLESTRUCT___Z(int p0, void* /*struct*/* p1) {}

// Symbol: ?OnSysCommand@CMFCStatusBar@@IEAAXI_J@Z
extern "C" void MS_ABI impl__OnSysCommand_CMFCStatusBar__IEAAXI_J_Z(unsigned int p0, __int64 p1) {}

// Symbol: ?OnTimer@CMFCStatusBar@@IEAAX_K@Z
extern "C" void MS_ABI impl__OnTimer_CMFCStatusBar__IEAAX_K_Z(unsigned __int64 p0) {}

// Symbol: ?OnToolHitTest@CMFCStatusBar@@MEBA_JVCPoint@@PEAUtagTOOLINFOW@@@Z
extern "C" __int64 MS_ABI impl__OnToolHitTest_CMFCStatusBar__MEBA_JVCPoint__PEAUtagTOOLINFOW___Z(void* /*class*/ p0, void* /*struct*/* p1) {
    return 0;
}

// Symbol: ?OnUpdateCmdUI@CMFCStatusBar@@MEAAXPEAVCFrameWnd@@H@Z
extern "C" void MS_ABI impl__OnUpdateCmdUI_CMFCStatusBar__MEAAXPEAVCFrameWnd__H_Z(void* /*class*/* p0, int p1) {}

// Symbol: ?PreCreateWindow@CMFCStatusBar@@UEAAHAEAUtagCREATESTRUCTW@@@Z
extern "C" int MS_ABI impl__PreCreateWindow_CMFCStatusBar__UEAAHAEAUtagCREATESTRUCTW___Z(void* /*struct*/* p0) {
    return 0;
}

// Symbol: ?RecalcLayout@CMFCStatusBar@@MEAAXXZ
extern "C" void MS_ABI impl__RecalcLayout_CMFCStatusBar__MEAAXXZ() {}

// Symbol: ?SetIndicators@CMFCStatusBar@@QEAAHPEBIH@Z
extern "C" int MS_ABI impl__SetIndicators_CMFCStatusBar__QEAAHPEBIH_Z(const unsigned int* p0, int p1) {
    return 0;
}

// Symbol: ?SetPaneAnimation@CMFCStatusBar@@QEAAXHPEAU_IMAGELIST@@IH@Z
extern "C" void MS_ABI impl__SetPaneAnimation_CMFCStatusBar__QEAAXHPEAU_IMAGELIST__IH_Z(int p0, void* /*struct*/* p1, unsigned int p2, int p3) {}

// Symbol: ?SetPaneBackgroundColor@CMFCStatusBar@@QEAAXHKH@Z
extern "C" void MS_ABI impl__SetPaneBackgroundColor_CMFCStatusBar__QEAAXHKH_Z(int p0, unsigned long p1, int p2) {}

// Symbol: ?SetPaneIcon@CMFCStatusBar@@QEAAXHPEAUHBITMAP__@@KH@Z
extern "C" void MS_ABI impl__SetPaneIcon_CMFCStatusBar__QEAAXHPEAUHBITMAP____KH_Z(int p0, void* /*struct*/* p1, unsigned long p2, int p3) {}

// Symbol: ?SetPaneIcon@CMFCStatusBar@@QEAAXHPEAUHICON__@@H@Z
extern "C" void MS_ABI impl__SetPaneIcon_CMFCStatusBar__QEAAXHPEAUHICON____H_Z(int p0, void* /*struct*/* p1, int p2) {}

// Symbol: ?SetPaneInfo@CMFCStatusBar@@QEAAXHIIH@Z
extern "C" void MS_ABI impl__SetPaneInfo_CMFCStatusBar__QEAAXHIIH_Z(int p0, unsigned int p1, unsigned int p2, int p3) {}

// Symbol: ?SetPaneStyle@CMFCStatusBar@@QEAAXHI@Z
extern "C" void MS_ABI impl__SetPaneStyle_CMFCStatusBar__QEAAXHI_Z(int p0, unsigned int p1) {}

// Symbol: ?SetPaneTextColor@CMFCStatusBar@@QEAAXHKH@Z
extern "C" void MS_ABI impl__SetPaneTextColor_CMFCStatusBar__QEAAXHKH_Z(int p0, unsigned long p1, int p2) {}

// Symbol: ?SetPaneWidth@CMFCStatusBar@@QEAAXHH@Z
extern "C" void MS_ABI impl__SetPaneWidth_CMFCStatusBar__QEAAXHH_Z(int p0, int p1) {}

// Symbol: ?SetTipText@CMFCStatusBar@@QEAAXHPEB_W@Z
extern "C" void MS_ABI impl__SetTipText_CMFCStatusBar__QEAAXHPEB_W_Z(int p0, const wchar_t* p1) {}
