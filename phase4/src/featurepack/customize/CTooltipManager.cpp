// CTooltipManager — OpenMFC implementation.
// Sources: mfccore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/MfccoreSupport.h"

// Symbol: ?CreateToolTipObject@CTooltipManager@@IEAAHAEAPEAVCToolTipCtrl@@I@Z
extern "C" int MS_ABI impl__CreateToolTipObject_CTooltipManager__IEAAHAEAPEAVCToolTipCtrl__I_Z(
    CTooltipManager* pThis, CToolTipCtrl** ppToolTip, unsigned int nType) {
    (void)pThis;
    return ppToolTip ? CTooltipManager::CreateToolTip(*ppToolTip, nullptr, nType) : FALSE;
}
// Symbol: ??0CTooltipManager@@QEAA@XZ
extern "C" void* MS_ABI impl___0CTooltipManager__QEAA_XZ(void* pThis) { return new (pThis) CTooltipManager(); }
// Symbol: ??1CTooltipManager@@UEAA@XZ
extern "C" void MS_ABI impl___1CTooltipManager__UEAA_XZ(CTooltipManager* pThis) { if (pThis) pThis->~CTooltipManager(); }
// Symbol: ?CreateToolTip@CTooltipManager@@SAHAEAPEAVCToolTipCtrl@@PEAVCWnd@@I@Z
extern "C" int MS_ABI impl__CreateToolTip_CTooltipManager__SAHAEAPEAVCToolTipCtrl__PEAVCWnd__I_Z(CToolTipCtrl** ppToolTip, CWnd* parent, unsigned int type) { return ppToolTip ? CTooltipManager::CreateToolTip(*ppToolTip, parent, type) : FALSE; }
// Symbol: ?DeleteToolTip@CTooltipManager@@SAXAEAPEAVCToolTipCtrl@@@Z
extern "C" void MS_ABI impl__DeleteToolTip_CTooltipManager__SAXAEAPEAVCToolTipCtrl___Z(CToolTipCtrl** ppToolTip) { if (ppToolTip) CTooltipManager::DeleteToolTip(*ppToolTip); }
// Symbol: ?UpdateTooltips@CTooltipManager@@QEAAXXZ
extern "C" void MS_ABI impl__UpdateTooltips_CTooltipManager__QEAAXXZ(CTooltipManager* pThis) { if (pThis) pThis->UpdateTooltips(); }
// Symbol: ?SetTooltipParams@CTooltipManager@@QEAAXIPEAUCRuntimeClass@@PEAVCMFCToolTipInfo@@@Z
extern "C" void MS_ABI impl__SetTooltipParams_CTooltipManager__QEAAXIPEAUCRuntimeClass__PEAVCMFCToolTipInfo___Z(CTooltipManager* pThis, unsigned int types, CRuntimeClass* rtc, CMFCToolTipInfo* params) { if (pThis) pThis->SetTooltipParams(types, rtc, params); }
// Symbol: ?SetTooltipText@CTooltipManager@@SAXPEAUtagTOOLINFOW@@PEAVCToolTipCtrl@@IV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@PEB_W@Z
extern "C" void MS_ABI impl__SetTooltipText_CTooltipManager__SAXPEAUtagTOOLINFOW__PEAVCToolTipCtrl__IV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__PEB_W_Z(
    TOOLINFOW* toolInfo, CToolTipCtrl* toolTip, unsigned int id, const CString* text, const wchar_t* description) {
    const wchar_t* resolved = text ? text->GetString() : nullptr;
    if (!resolved || !*resolved) resolved = description ? description : L"";

    TOOLINFOW localInfo = toolInfo ? *toolInfo : TOOLINFOW{};
    localInfo.cbSize = sizeof(TOOLINFOW);
    localInfo.uId = id ? id : localInfo.uId;
    localInfo.lpszText = const_cast<wchar_t*>(resolved);

    if (toolInfo) {
        toolInfo->cbSize = localInfo.cbSize;
        toolInfo->uId = localInfo.uId;
        toolInfo->lpszText = localInfo.lpszText;
    }
    if (toolTip && toolTip->GetSafeHwnd()) {
        ::SendMessageW(toolTip->GetSafeHwnd(), TTM_UPDATETIPTEXTW, 0, reinterpret_cast<LPARAM>(&localInfo));
    }
}
CTooltipManager::CTooltipManager() { memset(_tooltipmanager_padding, 0, sizeof(_tooltipmanager_padding)); }
CTooltipManager::~CTooltipManager() { g_tooltipManagerStates.erase(this); }
BOOL CTooltipManager::CreateToolTip(CToolTipCtrl*& pToolTip, CWnd* pWndParent, UINT nType) {
    const bool allocated = pToolTip == nullptr;
    if (allocated) pToolTip = new CToolTipCtrl();
    BOOL created = pToolTip->Create(pWndParent, nType);
    if (!created && allocated) {
        delete pToolTip;
        pToolTip = nullptr;
    }
    return created;
}
void CTooltipManager::DeleteToolTip(CToolTipCtrl*& pToolTip) {
    if (pToolTip) {
        pToolTip->DestroyWindow();
        delete pToolTip;
        pToolTip = nullptr;
    }
}
void CTooltipManager::SetTooltipParams(UINT nTypes, CRuntimeClass* pRTC, CMFCToolTipInfo* pParams) {
    TooltipManagerState& state = g_tooltipManagerStates[this];
    state.types = nTypes; state.runtimeClass = pRTC; state.params = pParams;
}
void CTooltipManager::UpdateTooltips() {
    TooltipManagerState& state = g_tooltipManagerStates[this];
    for (size_t i = 0; i < state.tooltips.size();) {
        CToolTipCtrl* pTip = state.tooltips[i];
        if (!pTip || !pTip->GetSafeHwnd()) {
            state.tooltips.erase(state.tooltips.begin() + static_cast<std::ptrdiff_t>(i));
            continue;
        }
        pTip->Activate(TRUE);
        if (state.params) {
            pTip->SetMaxTipWidth(400);
        }
        ++i;
    }
}
