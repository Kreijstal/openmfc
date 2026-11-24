// CTooltipManager — OpenMFC implementation.
// Sources: mfccore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/MfccoreSupport.h"

#include <cwchar>
#include <utility>

// Symbol: ?CreateToolTipObject@CTooltipManager@@IEAAHAEAPEAVCToolTipCtrl@@I@Z
extern "C" int MS_ABI impl__CreateToolTipObject_CTooltipManager__IEAAHAEAPEAVCToolTipCtrl__I_Z(
    CTooltipManager* pThis, CToolTipCtrl** ppToolTip, unsigned int nType) {
    return pThis && ppToolTip && pThis->CreateToolTipObject(*ppToolTip, nType);
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
    TOOLINFOW* info, CToolTipCtrl* tooltip, UINT type, CString* text, const wchar_t* description) {
    if (!text) return;
    const CString copy = *text;
    text->~CString();
    CTooltipManager::SetTooltipText(info, tooltip, type, copy, description);
}
extern "C" UINT impl__AFX_WM_UPDATETOOLTIPS__3IA;
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CToolTipCtrl__SAPEAUCRuntimeClass__XZ();

namespace {
constexpr UINT tooltipTypeCount = 11;
constexpr UINT allTooltipTypes = 0xffff;
constexpr wchar_t tooltipOwnerProperty[] = L"OpenMFC.TooltipOwner";
thread_local CTooltipManager* tooltipManager = nullptr;

void BroadcastTooltipUpdate(const CTooltipManager* manager, UINT types) {
    const auto found = g_tooltipManagerStates.find(manager);
    if (found == g_tooltipManagerStates.end()) return;
    auto& entries = found->second.tooltipOwners;
    std::vector<HWND> owners;
    for (auto it = entries.begin(); it != entries.end();) {
        const auto [tooltip, owner] = *it;
        if (!::IsWindow(owner) || ::GetPropW(tooltip, tooltipOwnerProperty) != owner) {
            it = entries.erase(it);
            continue;
        }
        if (std::find(owners.begin(), owners.end(), owner) == owners.end()) owners.push_back(owner);
        ++it;
    }
    // Handlers can add or remove owners while rebuilding their tooltips.
    for (HWND owner : owners) {
        if (g_tooltipManagerStates.find(manager) == g_tooltipManagerStates.end()) break;
        if (::IsWindow(owner)) ::SendMessageW(owner, impl__AFX_WM_UPDATETOOLTIPS__3IA, types, 0);
    }
}
}

static_assert(sizeof(CMFCToolTipInfo) == 0x34);
static_assert(sizeof(CTooltipManager) == 0x2d8);

CTooltipManager::CTooltipManager() {
    static_assert(offsetof(CTooltipManager, m_tooltipParams) == 0x08);
    static_assert(offsetof(CTooltipManager, m_tooltipClasses) == 0x248);
    if (tooltipManager) AfxThrowInvalidArgException();
    tooltipManager = this;
}

CTooltipManager::~CTooltipManager() {
    if (tooltipManager == this) tooltipManager = nullptr;
    g_tooltipManagerStates.erase(this);
}

BOOL CTooltipManager::CreateToolTipObject(CToolTipCtrl*& tooltip, UINT index) {
    if (index >= tooltipTypeCount) return FALSE;
    auto* runtime = m_tooltipClasses[index];
    if (!runtime) {
        tooltip = new CToolTipCtrl();
        return TRUE;
    }
    if (!runtime->IsDerivedFrom(impl__GetThisClass_CToolTipCtrl__SAPEAUCRuntimeClass__XZ())) return FALSE;
    tooltip = static_cast<CToolTipCtrl*>(runtime->CreateObject());
    if (!tooltip) return FALSE;
    if (runtime->IsDerivedFrom(RUNTIME_CLASS(CMFCToolTipCtrl)))
        static_cast<CMFCToolTipCtrl*>(tooltip)->SetParams(&m_tooltipParams[index]);
    return TRUE;
}

BOOL CTooltipManager::CreateToolTip(CToolTipCtrl*& tooltip, CWnd* parent, UINT type) {
    if (!parent || !parent->GetSafeHwnd()) return FALSE;
    if (!type) type = 1;
    UINT index = 0;
    while (index < tooltipTypeCount && (1u << index) != type) ++index;
    if (index == tooltipTypeCount) return FALSE;

    DeleteToolTip(tooltip);
    const HWND owner = parent->GetSafeHwnd();
    auto* manager = tooltipManager;
    const CMFCToolTipInfo params = manager ? manager->m_tooltipParams[index] : CMFCToolTipInfo{};
    if (manager) {
        if (!manager->CreateToolTipObject(tooltip, index)) return FALSE;
    } else {
        tooltip = new CToolTipCtrl();
    }
    if (!tooltip->Create(parent, params.m_bBalloonTooltip ? TTS_BALLOON : 0)) {
        DeleteToolTip(tooltip);
        return FALSE;
    }
    const HWND window = tooltip->GetSafeHwnd();
    if (manager && tooltipManager == manager && ::SetPropW(window, tooltipOwnerProperty, owner))
        g_tooltipManagerStates[manager].tooltipOwners[window] = owner;
    if (params.m_clrFill != COLORREF(-1)) ::SendMessageW(window, TTM_SETTIPBKCOLOR, params.m_clrFill, 0);
    if (params.m_clrText != COLORREF(-1)) ::SendMessageW(window, TTM_SETTIPTEXTCOLOR, params.m_clrText, 0);
    if (params.m_nMaxDescrWidth != CMFCToolTipInfo{}.m_nMaxDescrWidth)
        tooltip->SetMaxTipWidth(params.m_nMaxDescrWidth);
    return TRUE;
}

void CTooltipManager::DeleteToolTip(CToolTipCtrl*& tooltip) {
    auto* old = std::exchange(tooltip, nullptr);
    if (!old) return;
    const HWND window = old->GetSafeHwnd();
    if (auto it = g_tooltipManagerStates.find(tooltipManager); it != g_tooltipManagerStates.end())
        it->second.tooltipOwners.erase(window);
    if (::IsWindow(window)) {
        ::RemovePropW(window, tooltipOwnerProperty);
        ::DestroyWindow(window);
    }
    old->m_hWnd = nullptr;
    delete old;
}

void CTooltipManager::SetTooltipParams(UINT types, CRuntimeClass* runtime, CMFCToolTipInfo* params) {
    if (runtime && !runtime->IsDerivedFrom(impl__GetThisClass_CToolTipCtrl__SAPEAUCRuntimeClass__XZ()))
        AfxThrowInvalidArgException();
    for (UINT index = 0; index < tooltipTypeCount; ++index) {
        if (!(types & (1u << index))) continue;
        m_tooltipClasses[index] = runtime;
        m_tooltipParams[index] = params ? *params : CMFCToolTipInfo{};
    }
    BroadcastTooltipUpdate(this, types);
}

void CTooltipManager::SetTooltipText(TOOLINFOW* info, CToolTipCtrl* tooltip, UINT type,
                                     const CString& text, const wchar_t* description) {
    if (!info || !tooltip) return;
    UINT index = 0;
    while (index < tooltipTypeCount && (1u << index) != type) ++index;
    if (index == tooltipTypeCount) return;

    const wchar_t* label = text.GetString();
    const HWND window = tooltip->GetSafeHwnd();
    if (window && tooltipManager && tooltipManager->m_tooltipParams[index].m_bBalloonTooltip) {
        const bool hasDescription = description && *description;
        ::SendMessageW(window, TTM_SETTITLEW, TTI_INFO, reinterpret_cast<LPARAM>(hasDescription ? label : L""));
        if (hasDescription) label = description;
    }
    info->lpszText = ::_wcsdup(label);
    if (tooltip->IsKindOf(RUNTIME_CLASS(CMFCToolTipCtrl)))
        static_cast<CMFCToolTipCtrl*>(tooltip)->SetDescription(CString(description ? description : L""));
}

void CTooltipManager::UpdateTooltips() {
    BroadcastTooltipUpdate(this, allTooltipTypes);
}
