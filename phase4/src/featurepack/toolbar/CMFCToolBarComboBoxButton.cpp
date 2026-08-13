// CMFCToolBarComboBoxButton — OpenMFC implementation.
// Sources: cbarcore.cpp, mfccore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/CbarcoreSupport.h"
#include "detail/MfccoreSupport.h"

// Symbol: ?AddItem@CMFCToolBarComboBoxButton@@UEAA_JPEB_W_K@Z
INT_PTR CMFCToolBarComboBoxButton::AddItem(const wchar_t* lpszItem, DWORD_PTR dwData) {
    if (!lpszItem) return CB_ERR;
    RegisterComboButton(this);
    ComboButtonState& state = EnsureComboState(this);
    state.items.emplace_back(CString(lpszItem), dwData);
    return static_cast<INT_PTR>(state.items.size() - 1);
}
// Symbol: ?AddSortedItem@CMFCToolBarComboBoxButton@@UEAA_JPEB_W_K@Z
INT_PTR CMFCToolBarComboBoxButton::AddSortedItem(const wchar_t* lpszItem, DWORD_PTR dwData) {
    if (!lpszItem) return CB_ERR;
    RegisterComboButton(this);
    ComboButtonState& state = EnsureComboState(this);
    auto insertPos = state.items.begin();
    while (insertPos != state.items.end() &&
           CompareComboText(static_cast<const wchar_t*>(insertPos->first), lpszItem) <= 0) {
        ++insertPos;
    }
    const int index = static_cast<int>(insertPos - state.items.begin());
    state.items.insert(insertPos, std::make_pair(CString(lpszItem), dwData));
    if (state.curSel >= index) ++state.curSel;
    return index;
}
// Symbol: ?Compare@CMFCToolBarComboBoxButton@@UEAAHPEB_W0@Z
int CMFCToolBarComboBoxButton::Compare(const wchar_t* s1, const wchar_t* s2) {
    return CompareComboText(s1, s2);
}
// Symbol: ?DeleteItem@CMFCToolBarComboBoxButton@@QEAAHH@Z
BOOL CMFCToolBarComboBoxButton::DeleteItem(int nIndex) {
    RegisterComboButton(this);
    ComboButtonState& state = EnsureComboState(this);
    if (nIndex < 0 || nIndex >= static_cast<int>(state.items.size())) return FALSE;
    state.items.erase(state.items.begin() + nIndex);
    if (state.curSel == nIndex) {
        state.curSel = -1;
        m_strText.Empty();
    } else if (state.curSel > nIndex) {
        --state.curSel;
    }
    return TRUE;
}
// Symbol: ?DeleteItem@CMFCToolBarComboBoxButton@@QEAAH_K@Z
BOOL CMFCToolBarComboBoxButton::DeleteItem(DWORD_PTR dwData) {
    RegisterComboButton(this);
    ComboButtonState& state = EnsureComboState(this);
    for (int i = 0; i < static_cast<int>(state.items.size()); ++i) {
        if (state.items[i].second == dwData) {
            return DeleteItem(i);
        }
    }
    return FALSE;
}
// Symbol: ?DeleteItem@CMFCToolBarComboBoxButton@@QEAAHPEB_W@Z
BOOL CMFCToolBarComboBoxButton::DeleteItem(const wchar_t* lpszText) {
    const int index = FindItem(lpszText);
    return (index != CB_ERR) ? DeleteItem(index) : FALSE;
}
// Symbol: ?FindItem@CMFCToolBarComboBoxButton@@QEBAHPEB_W@Z
int CMFCToolBarComboBoxButton::FindItem(const wchar_t* lpszText) const {
    if (!lpszText) return CB_ERR;
    const ComboButtonState* state = FindComboState(this);
    if (!state) return CB_ERR;
    for (int i = 0; i < static_cast<int>(state->items.size()); ++i) {
        if (CompareComboText(static_cast<const wchar_t*>(state->items[i].first), lpszText) == 0) {
            return i;
        }
    }
    return CB_ERR;
}
// Symbol: ?GetItem@CMFCToolBarComboBoxButton@@QEBAPEB_WH@Z
const wchar_t* CMFCToolBarComboBoxButton::GetItem(int nIndex) const {
    const ComboButtonState* state = FindComboState(this);
    if (!state) return nullptr;
    if (nIndex < 0) nIndex = state->curSel;
    if (nIndex < 0 || nIndex >= static_cast<int>(state->items.size())) return nullptr;
    return state->items[nIndex].first;
}
// Symbol: ?GetCount@CMFCToolBarComboBoxButton@@QEBA_JXZ
INT_PTR CMFCToolBarComboBoxButton::GetCount() const {
    const ComboButtonState* state = FindComboState(this);
    return state ? static_cast<INT_PTR>(state->items.size()) : 0;
}
// Symbol: ?GetItemData@CMFCToolBarComboBoxButton@@QEBA_KH@Z
DWORD_PTR CMFCToolBarComboBoxButton::GetItemData(int nIndex) const {
    const ComboButtonState* state = FindComboState(this);
    if (!state) return 0;
    if (nIndex < 0) nIndex = state->curSel;
    if (nIndex < 0 || nIndex >= static_cast<int>(state->items.size())) return 0;
    return state->items[nIndex].second;
}
// Symbol: ?SelectItem@CMFCToolBarComboBoxButton@@QEAAHPEB_W@Z
BOOL CMFCToolBarComboBoxButton::SelectItem(const wchar_t* lpszItem) {
    const int index = FindItem(lpszItem);
    return (index != CB_ERR) ? SelectItem(index, TRUE) : FALSE;
}
// Symbol: ?SelectItem@CMFCToolBarComboBoxButton@@QEAAHHH@Z
BOOL CMFCToolBarComboBoxButton::SelectItem(int nIndex, BOOL) {
    RegisterComboButton(this);
    ComboButtonState& state = EnsureComboState(this);
    if (nIndex < 0 || nIndex >= static_cast<int>(state.items.size())) return FALSE;
    state.curSel = nIndex;
    m_strText = state.items[nIndex].first;
    return TRUE;
}
// Symbol: ?SelectItem@CMFCToolBarComboBoxButton@@QEAAH_K@Z
BOOL CMFCToolBarComboBoxButton::SelectItem(DWORD_PTR dwData) {
    RegisterComboButton(this);
    ComboButtonState& state = EnsureComboState(this);
    for (int i = 0; i < static_cast<int>(state.items.size()); ++i) {
        if (state.items[i].second == dwData) {
            return SelectItem(i, TRUE);
        }
    }
    return FALSE;
}
// Symbol: ?SetText@CMFCToolBarComboBoxButton@@QEAAXPEB_W@Z
void CMFCToolBarComboBoxButton::SetText(const wchar_t* lpszText) {
    RegisterComboButton(this);
    m_strText = lpszText ? lpszText : L"";
}
// Static methods
// Symbol: ?GetByCmd@CMFCToolBarComboBoxButton@@SAPEAV1@IH@Z
CMFCToolBarComboBoxButton* CMFCToolBarComboBoxButton::GetByCmd(UINT uiCmd, BOOL) {
    const auto buttons = GetComboButtonsByCmd(uiCmd);
    return buttons.empty() ? nullptr : buttons.front();
}
// Symbol: ?GetCountAll@CMFCToolBarComboBoxButton@@SAHI@Z
int CMFCToolBarComboBoxButton::GetCountAll(UINT uiCmd) {
    CMFCToolBarComboBoxButton* pButton = GetByCmd(uiCmd, FALSE);
    return pButton ? static_cast<int>(pButton->GetCount()) : 0;
}
// Symbol: ?GetCurSelAll@CMFCToolBarComboBoxButton@@SAHI@Z
int CMFCToolBarComboBoxButton::GetCurSelAll(UINT uiCmd) {
    CMFCToolBarComboBoxButton* pButton = GetByCmd(uiCmd, FALSE);
    if (!pButton) return CB_ERR;
    const ComboButtonState* state = FindComboState(pButton);
    return state ? state->curSel : CB_ERR;
}
// Symbol: ?GetItemAll@CMFCToolBarComboBoxButton@@SAPEB_WIH@Z
const wchar_t* CMFCToolBarComboBoxButton::GetItemAll(UINT uiCmd, int iIndex) {
    CMFCToolBarComboBoxButton* pButton = GetByCmd(uiCmd, FALSE);
    return pButton ? pButton->GetItem(iIndex) : nullptr;
}
// Symbol: ?GetItemDataAll@CMFCToolBarComboBoxButton@@SA_KIH@Z
DWORD_PTR CMFCToolBarComboBoxButton::GetItemDataAll(UINT uiCmd, int iIndex) {
    CMFCToolBarComboBoxButton* pButton = GetByCmd(uiCmd, FALSE);
    return pButton ? pButton->GetItemData(iIndex) : 0;
}
// Symbol: ?GetTextAll@CMFCToolBarComboBoxButton@@SAPEB_WI@Z
const wchar_t* CMFCToolBarComboBoxButton::GetTextAll(UINT uiCmd) {
    CMFCToolBarComboBoxButton* pButton = GetByCmd(uiCmd, FALSE);
    return pButton ? static_cast<const wchar_t*>(pButton->m_strText) : nullptr;
}
// Symbol: ?SelectItemAll@CMFCToolBarComboBoxButton@@SAHIPEB_W@Z
BOOL CMFCToolBarComboBoxButton::SelectItemAll(UINT uiCmd, const wchar_t* lpszText) {
    BOOL result = FALSE;
    for (CMFCToolBarComboBoxButton* button : GetComboButtonsByCmd(uiCmd)) {
        if (button && button->SelectItem(lpszText)) result = TRUE;
    }
    return result;
}
// Symbol: ?SelectItemAll@CMFCToolBarComboBoxButton@@SAHIH@Z
BOOL CMFCToolBarComboBoxButton::SelectItemAll(UINT uiCmd, int nIndex) {
    BOOL result = FALSE;
    for (CMFCToolBarComboBoxButton* button : GetComboButtonsByCmd(uiCmd)) {
        if (button && button->SelectItem(nIndex, TRUE)) result = TRUE;
    }
    return result;
}
// Symbol: ?SelectItemAll@CMFCToolBarComboBoxButton@@SAHI_K@Z
BOOL CMFCToolBarComboBoxButton::SelectItemAll(UINT uiCmd, DWORD_PTR dwData) {
    BOOL result = FALSE;
    for (CMFCToolBarComboBoxButton* button : GetComboButtonsByCmd(uiCmd)) {
        if (button && button->SelectItem(dwData)) result = TRUE;
    }
    return result;
}
// Symbol: ?AddItem@CMFCToolBarComboBoxButton@@UEAA_JPEB_W_K@Z
extern "C" INT_PTR MS_ABI impl__AddItem_CMFCToolBarComboBoxButton__UEAA_JPEB_W_K_Z(CMFCToolBarComboBoxButton* pThis, const wchar_t* p0, DWORD_PTR p1) {
    return pThis->AddItem(p0, p1);
}
// Symbol: ?AddSortedItem@CMFCToolBarComboBoxButton@@UEAA_JPEB_W_K@Z
extern "C" INT_PTR MS_ABI impl__AddSortedItem_CMFCToolBarComboBoxButton__UEAA_JPEB_W_K_Z(CMFCToolBarComboBoxButton* pThis, const wchar_t* p0, DWORD_PTR p1) {
    return pThis->AddSortedItem(p0, p1);
}
// Symbol: ?Compare@CMFCToolBarComboBoxButton@@UEAAHPEB_W0@Z
extern "C" int MS_ABI impl__Compare_CMFCToolBarComboBoxButton__UEAAHPEB_W0_Z(CMFCToolBarComboBoxButton* pThis, const wchar_t* p0, const wchar_t* p1) {
    return pThis->Compare(p0, p1);
}
// Symbol: ?DeleteItem@CMFCToolBarComboBoxButton@@QEAAHH@Z
extern "C" int MS_ABI impl__DeleteItem_CMFCToolBarComboBoxButton__QEAAHH_Z(CMFCToolBarComboBoxButton* pThis, int p0) {
    return pThis->DeleteItem(p0);
}
// Symbol: ?DeleteItem@CMFCToolBarComboBoxButton@@QEAAH_K@Z
extern "C" int MS_ABI impl__DeleteItem_CMFCToolBarComboBoxButton__QEAAH_K_Z(CMFCToolBarComboBoxButton* pThis, DWORD_PTR p0) {
    return pThis->DeleteItem(p0);
}
// Symbol: ?DeleteItem@CMFCToolBarComboBoxButton@@QEAAHPEB_W@Z
extern "C" int MS_ABI impl__DeleteItem_CMFCToolBarComboBoxButton__QEAAHPEB_W_Z(CMFCToolBarComboBoxButton* pThis, const wchar_t* p0) {
    return pThis->DeleteItem(p0);
}
// Symbol: ?FindItem@CMFCToolBarComboBoxButton@@QEBAHPEB_W@Z
extern "C" int MS_ABI impl__FindItem_CMFCToolBarComboBoxButton__QEBAHPEB_W_Z(const CMFCToolBarComboBoxButton* pThis, const wchar_t* p0) {
    return pThis->FindItem(p0);
}
// Symbol: ?GetByCmd@CMFCToolBarComboBoxButton@@SAPEAV1@IH@Z
extern "C" CMFCToolBarComboBoxButton* MS_ABI impl__GetByCmd_CMFCToolBarComboBoxButton__SAPEAV1_IH_Z(unsigned int p0, int p1) {
    return CMFCToolBarComboBoxButton::GetByCmd(p0, (BOOL)p1);
}
// Symbol: ?GetCount@CMFCToolBarComboBoxButton@@QEBA_JXZ
extern "C" INT_PTR MS_ABI impl__GetCount_CMFCToolBarComboBoxButton__QEBA_JXZ(const CMFCToolBarComboBoxButton* pThis) {
    return pThis->GetCount();
}
// Symbol: ?GetCountAll@CMFCToolBarComboBoxButton@@SAHI@Z
extern "C" int MS_ABI impl__GetCountAll_CMFCToolBarComboBoxButton__SAHI_Z(unsigned int p0) {
    return CMFCToolBarComboBoxButton::GetCountAll(p0);
}
// Symbol: ?GetCurSelAll@CMFCToolBarComboBoxButton@@SAHI@Z
extern "C" int MS_ABI impl__GetCurSelAll_CMFCToolBarComboBoxButton__SAHI_Z(unsigned int p0) {
    return CMFCToolBarComboBoxButton::GetCurSelAll(p0);
}
// Symbol: ?GetItem@CMFCToolBarComboBoxButton@@QEBAPEB_WH@Z
extern "C" const wchar_t* MS_ABI impl__GetItem_CMFCToolBarComboBoxButton__QEBAPEB_WH_Z(const CMFCToolBarComboBoxButton* pThis, int p0) {
    return pThis->GetItem(p0);
}
// Symbol: ?GetItemAll@CMFCToolBarComboBoxButton@@SAPEB_WIH@Z
extern "C" const wchar_t* MS_ABI impl__GetItemAll_CMFCToolBarComboBoxButton__SAPEB_WIH_Z(unsigned int p0, int p1) {
    return CMFCToolBarComboBoxButton::GetItemAll(p0, p1);
}
// Symbol: ?GetItemData@CMFCToolBarComboBoxButton@@QEBA_KH@Z
extern "C" DWORD_PTR MS_ABI impl__GetItemData_CMFCToolBarComboBoxButton__QEBA_KH_Z(const CMFCToolBarComboBoxButton* pThis, int p0) {
    return pThis->GetItemData(p0);
}
// Symbol: ?GetItemDataAll@CMFCToolBarComboBoxButton@@SA_KIH@Z
extern "C" DWORD_PTR MS_ABI impl__GetItemDataAll_CMFCToolBarComboBoxButton__SA_KIH_Z(unsigned int p0, int p1) {
    return CMFCToolBarComboBoxButton::GetItemDataAll(p0, p1);
}
// Symbol: ?GetTextAll@CMFCToolBarComboBoxButton@@SAPEB_WI@Z
extern "C" const wchar_t* MS_ABI impl__GetTextAll_CMFCToolBarComboBoxButton__SAPEB_WI_Z(unsigned int p0) {
    return CMFCToolBarComboBoxButton::GetTextAll(p0);
}
// Symbol: ?SelectItem@CMFCToolBarComboBoxButton@@QEAAHPEB_W@Z
extern "C" int MS_ABI impl__SelectItem_CMFCToolBarComboBoxButton__QEAAHPEB_W_Z(CMFCToolBarComboBoxButton* pThis, const wchar_t* p0) {
    return pThis->SelectItem(p0);
}
// Symbol: ?SelectItem@CMFCToolBarComboBoxButton@@QEAAHHH@Z
extern "C" int MS_ABI impl__SelectItem_CMFCToolBarComboBoxButton__QEAAHHH_Z(CMFCToolBarComboBoxButton* pThis, int p0, int p1) {
    return pThis->SelectItem(p0, (BOOL)p1);
}
// Symbol: ?SelectItem@CMFCToolBarComboBoxButton@@QEAAH_K@Z
extern "C" int MS_ABI impl__SelectItem_CMFCToolBarComboBoxButton__QEAAH_K_Z(CMFCToolBarComboBoxButton* pThis, DWORD_PTR p0) {
    return pThis->SelectItem(p0);
}
// Symbol: ?SelectItemAll@CMFCToolBarComboBoxButton@@SAHIPEB_W@Z
extern "C" int MS_ABI impl__SelectItemAll_CMFCToolBarComboBoxButton__SAHIPEB_W_Z(unsigned int p0, const wchar_t* p1) {
    return CMFCToolBarComboBoxButton::SelectItemAll(p0, p1);
}
// Symbol: ?SelectItemAll@CMFCToolBarComboBoxButton@@SAHIH@Z
extern "C" int MS_ABI impl__SelectItemAll_CMFCToolBarComboBoxButton__SAHIH_Z(unsigned int p0, int p1) {
    return CMFCToolBarComboBoxButton::SelectItemAll(p0, p1);
}
// Symbol: ?SelectItemAll@CMFCToolBarComboBoxButton@@SAHI_K@Z
extern "C" int MS_ABI impl__SelectItemAll_CMFCToolBarComboBoxButton__SAHI_K_Z(unsigned int p0, DWORD_PTR p1) {
    return CMFCToolBarComboBoxButton::SelectItemAll(p0, p1);
}
// Symbol: ?SetText@CMFCToolBarComboBoxButton@@QEAAXPEB_W@Z
extern "C" void MS_ABI impl__SetText_CMFCToolBarComboBoxButton__QEAAXPEB_W_Z(CMFCToolBarComboBoxButton* pThis, const wchar_t* p0) {
    pThis->SetText(p0);
}
CMFCToolBarComboBoxButton::~CMFCToolBarComboBoxButton() {
    UnregisterComboButton(this);
}
CMFCToolBarComboBoxButton::CMFCToolBarComboBoxButton() { memset(_pad, 0, sizeof(_pad)); }
// Symbol: ?AdjustRect@CMFCToolBarComboBoxButton@@IEAAXXZ
extern "C" void MS_ABI impl__AdjustRect_CMFCToolBarComboBoxButton__IEAAXXZ() {}

// Symbol: ?CopyFrom@CMFCToolBarComboBoxButton@@UEAAXAEBVCMFCToolBarButton@@@Z
extern "C" void MS_ABI impl__CopyFrom_CMFCToolBarComboBoxButton__UEAAXAEBVCMFCToolBarButton___Z(const void* /*class*/* p0) {}

// Symbol: ?CreateCombo@CMFCToolBarComboBoxButton@@UEAAPEAVCComboBox@@PEAVCWnd@@AEBVCRect@@@Z
extern "C" void* MS_ABI impl__CreateCombo_CMFCToolBarComboBoxButton__UEAAPEAVCComboBox__PEAVCWnd__AEBVCRect___Z(void* /*class*/* p0, const void* /*class*/* p1) {
    return nullptr;
}

// Symbol: ?CreateEdit@CMFCToolBarComboBoxButton@@UEAAPEAVCMFCToolBarComboBoxEdit@@PEAVCWnd@@AEBVCRect@@K@Z
extern "C" void* MS_ABI impl__CreateEdit_CMFCToolBarComboBoxButton__UEAAPEAVCMFCToolBarComboBoxEdit__PEAVCWnd__AEBVCRect__K_Z(void* /*class*/* p0, const void* /*class*/* p1, unsigned long p2) {
    return nullptr;
}

// Symbol: ?ExportToMenuButton@CMFCToolBarComboBoxButton@@UEBAHAEAVCMFCToolBarMenuButton@@@Z
extern "C" int MS_ABI impl__ExportToMenuButton_CMFCToolBarComboBoxButton__UEBAHAEAVCMFCToolBarMenuButton___Z(void* /*class*/* p0) {
    return 0;
}

// Symbol: ?GetItemDataPtrAll@CMFCToolBarComboBoxButton@@SAPEAXIH@Z
extern "C" void* MS_ABI impl__GetItemDataPtrAll_CMFCToolBarComboBoxButton__SAPEAXIH_Z(unsigned int p0, int p1) {
    return nullptr;
}

// Symbol: ?HasFocus@CMFCToolBarComboBoxButton@@UEBAHXZ
extern "C" int MS_ABI impl__HasFocus_CMFCToolBarComboBoxButton__UEBAHXZ() {
    return 0;
}

// Symbol: ?Initialize@CMFCToolBarComboBoxButton@@IEAAXXZ
extern "C" void MS_ABI impl__Initialize_CMFCToolBarComboBoxButton__IEAAXXZ() {}

// Symbol: ?NotifyCommand@CMFCToolBarComboBoxButton@@UEAAHH@Z
extern "C" int MS_ABI impl__NotifyCommand_CMFCToolBarComboBoxButton__UEAAHH_Z(int p0) {
    return 0;
}

// Symbol: ?OnAddToCustomizePage@CMFCToolBarComboBoxButton@@UEAAXXZ
extern "C" void MS_ABI impl__OnAddToCustomizePage_CMFCToolBarComboBoxButton__UEAAXXZ() {}

// Symbol: ?OnCalculateSize@CMFCToolBarComboBoxButton@@UEAA?AUtagSIZE@@PEAVCDC@@AEBVCSize@@H@Z
extern "C" void* MS_ABI impl__OnCalculateSize_CMFCToolBarComboBoxButton__UEAA_AUtagSIZE__PEAVCDC__AEBVCSize__H_Z(void* /*struct*/* p0, void* /*class*/* p1, const void* /*class*/* p2, int p3) {
    return nullptr;
}

// Symbol: ?OnChangeParentWnd@CMFCToolBarComboBoxButton@@UEAAXPEAVCWnd@@@Z
extern "C" void MS_ABI impl__OnChangeParentWnd_CMFCToolBarComboBoxButton__UEAAXPEAVCWnd___Z(void* /*class*/* p0) {}

// Symbol: ?OnClick@CMFCToolBarComboBoxButton@@UEAAHPEAVCWnd@@H@Z
extern "C" int MS_ABI impl__OnClick_CMFCToolBarComboBoxButton__UEAAHPEAVCWnd__H_Z(void* /*class*/* p0, int p1) {
    return 0;
}

// Symbol: ?OnCtlColor@CMFCToolBarComboBoxButton@@UEAAPEAUHBRUSH__@@PEAVCDC@@I@Z
extern "C" void* MS_ABI impl__OnCtlColor_CMFCToolBarComboBoxButton__UEAAPEAUHBRUSH____PEAVCDC__I_Z(void* /*class*/* p0, unsigned int p1) {
    return nullptr;
}

// Symbol: ?OnDraw@CMFCToolBarComboBoxButton@@UEAAXPEAVCDC@@AEBVCRect@@PEAVCMFCToolBarImages@@HHHHH@Z
extern "C" void MS_ABI impl__OnDraw_CMFCToolBarComboBoxButton__UEAAXPEAVCDC__AEBVCRect__PEAVCMFCToolBarImages__HHHHH_Z(void* /*class*/* p0, const void* /*class*/* p1, void* /*class*/* p2, int p3, int p4, int p5, int p6, int p7) {}

// Symbol: ?OnDrawOnCustomizeList@CMFCToolBarComboBoxButton@@UEAAHPEAVCDC@@AEBVCRect@@H@Z
extern "C" int MS_ABI impl__OnDrawOnCustomizeList_CMFCToolBarComboBoxButton__UEAAHPEAVCDC__AEBVCRect__H_Z(void* /*class*/* p0, const void* /*class*/* p1, int p2) {
    return 0;
}

// Symbol: ?OnGlobalFontsChanged@CMFCToolBarComboBoxButton@@UEAAXXZ
extern "C" void MS_ABI impl__OnGlobalFontsChanged_CMFCToolBarComboBoxButton__UEAAXXZ() {}

// Symbol: ?OnMove@CMFCToolBarComboBoxButton@@UEAAXXZ
extern "C" void MS_ABI impl__OnMove_CMFCToolBarComboBoxButton__UEAAXXZ() {}

// Symbol: ?OnShow@CMFCToolBarComboBoxButton@@UEAAXH@Z
extern "C" void MS_ABI impl__OnShow_CMFCToolBarComboBoxButton__UEAAXH_Z(int p0) {}

// Symbol: ?OnSize@CMFCToolBarComboBoxButton@@UEAAXH@Z
extern "C" void MS_ABI impl__OnSize_CMFCToolBarComboBoxButton__UEAAXH_Z(int p0) {}

// Symbol: ?OnUpdateToolTip@CMFCToolBarComboBoxButton@@UEAAHPEAVCWnd@@HAEAVCToolTipCtrl@@AEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@@Z
extern "C" int MS_ABI impl__OnUpdateToolTip_CMFCToolBarComboBoxButton__UEAAHPEAVCWnd__HAEAVCToolTipCtrl__AEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(void* /*class*/* p0, int p1, void* /*class*/* p2, void* /*class*/* p3, void** p4, void* p5) {
    return 0;
}

// Symbol: ?RemoveAllItems@CMFCToolBarComboBoxButton@@QEAAXXZ
extern "C" void MS_ABI impl__RemoveAllItems_CMFCToolBarComboBoxButton__QEAAXXZ() {}

// Symbol: ?Serialize@CMFCToolBarComboBoxButton@@UEAAXAEAVCArchive@@@Z
extern "C" void MS_ABI impl__Serialize_CMFCToolBarComboBoxButton__UEAAXAEAVCArchive___Z(void* /*class*/* p0) {}

// Symbol: ?SetACCData@CMFCToolBarComboBoxButton@@UEAAHPEAVCWnd@@AEAVCAccessibilityData@@@Z
extern "C" int MS_ABI impl__SetACCData_CMFCToolBarComboBoxButton__UEAAHPEAVCWnd__AEAVCAccessibilityData___Z(void* /*class*/* p0, void* /*class*/* p1) {
    return 0;
}

// Symbol: ?SetDropDownHeight@CMFCToolBarComboBoxButton@@QEAAXH@Z
extern "C" void MS_ABI impl__SetDropDownHeight_CMFCToolBarComboBoxButton__QEAAXH_Z(int p0) {}

// Symbol: ?SetHotEdit@CMFCToolBarComboBoxButton@@IEAAXH@Z
extern "C" void MS_ABI impl__SetHotEdit_CMFCToolBarComboBoxButton__IEAAXH_Z(int p0) {}

// Symbol: ?SetStyle@CMFCToolBarComboBoxButton@@UEAAXI@Z
extern "C" void MS_ABI impl__SetStyle_CMFCToolBarComboBoxButton__UEAAXI_Z(unsigned int p0) {}
