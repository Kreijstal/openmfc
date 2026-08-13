// CMFCPropertyGridProperty — OpenMFC implementation.
// Sources: ctrlcore.cpp, mfccore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/CtrlcoreSupport.h"
#include "detail/MfccoreSupport.h"


// Implementations this unit calls that are defined with their own class.
extern "C" int MS_ABI impl__OnRotateListValue_CMFCPropertyGridProperty__MEAAHH_Z(CMFCPropertyGridProperty* pThis, int p0);

// Symbol: ?Show@CMFCPropertyGridProperty@@QEAAXHH@Z
extern "C" void MS_ABI impl__Show_CMFCPropertyGridProperty__QEAAXHH_Z(
    CMFCPropertyGridProperty* pThis, int bShow, int bAdjustLayout) {
    if (!pThis) {
        return;
    }

    pThis->Show(bShow ? TRUE : FALSE, bAdjustLayout ? TRUE : FALSE);
}
// Symbol: ?FormatProperty@CMFCPropertyGridProperty@@UEAA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@XZ
extern "C" void MS_ABI impl__FormatProperty_CMFCPropertyGridProperty__UEAA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__XZ(
    CString* pRet, CMFCPropertyGridProperty* pThis) {
    new(pRet) CString(pThis ? pThis->FormatProperty() : CString());
}
// Symbol: ?GetNameTooltip@CMFCPropertyGridProperty@@UEAA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@XZ
extern "C" void MS_ABI impl__GetNameTooltip_CMFCPropertyGridProperty__UEAA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__XZ(
    CString* pRet, CMFCPropertyGridProperty* pThis) {
    new(pRet) CString(pThis ? pThis->GetNameTooltip() : CString());
}
// Symbol: ?GetValueTooltip@CMFCPropertyGridProperty@@UEAA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@XZ
extern "C" void MS_ABI impl__GetValueTooltip_CMFCPropertyGridProperty__UEAA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__XZ(
    CString* pRet, CMFCPropertyGridProperty* pThis) {
    new(pRet) CString(pThis ? pThis->GetValueTooltip() : CString());
}
// Symbol: ?HasButton@CMFCPropertyGridProperty@@MEBAHXZ
extern "C" int MS_ABI impl__HasButton_CMFCPropertyGridProperty__MEBAHXZ(CMFCPropertyGridProperty* pThis) {
    const PropertyGridPropertyState* state = FindPropertyGridPropertyState(pThis);
    return (state && !state->options.empty()) ? TRUE : FALSE;
}
// Symbol: ?IsSelected@CMFCPropertyGridProperty@@UEBAHXZ
extern "C" int MS_ABI impl__IsSelected_CMFCPropertyGridProperty__UEBAHXZ(CMFCPropertyGridProperty* pThis) {
    const PropertyGridPropertyState* propState = FindPropertyGridPropertyState(pThis);
    const PropertyGridCtrlState* ctrlState = propState && propState->owner ? FindPropertyGridCtrlState(propState->owner) : nullptr;
    return (ctrlState && ctrlState->current == pThis) ? TRUE : FALSE;
}
// Symbol: ?OnActivateByTab@CMFCPropertyGridProperty@@MEAAHXZ
extern "C" int MS_ABI impl__OnActivateByTab_CMFCPropertyGridProperty__MEAAHXZ(CMFCPropertyGridProperty* pThis) {
    PropertyGridPropertyState* propState = FindMutablePropertyGridPropertyState(pThis);
    if (!pThis || !propState || !propState->owner || !pThis->IsEnabled() || !pThis->IsVisible()) return FALSE;
    propState->owner->SetCurSel(pThis, TRUE);
    return TRUE;
}
// Symbol: ?OnClickButton@CMFCPropertyGridProperty@@UEAAXVCPoint@@@Z
extern "C" void MS_ABI impl__OnClickButton_CMFCPropertyGridProperty__UEAAXVCPoint___Z(
    CMFCPropertyGridProperty* pThis, CPoint) {
    if (pThis) impl__OnRotateListValue_CMFCPropertyGridProperty__MEAAHH_Z(pThis, TRUE);
}
// Symbol: ?OnClickValue@CMFCPropertyGridProperty@@UEAAHIVCPoint@@@Z
extern "C" int MS_ABI impl__OnClickValue_CMFCPropertyGridProperty__UEAAHIVCPoint___Z(
    CMFCPropertyGridProperty* pThis, unsigned int, CPoint) {
    if (!pThis || !pThis->IsEnabled()) return FALSE;
    PropertyGridPropertyState* propState = FindMutablePropertyGridPropertyState(pThis);
    if (propState && propState->owner) propState->owner->SetCurSel(pThis, TRUE);
    return TRUE;
}
// Symbol: ?OnDblClk@CMFCPropertyGridProperty@@UEAAHVCPoint@@@Z
extern "C" int MS_ABI impl__OnDblClk_CMFCPropertyGridProperty__UEAAHVCPoint___Z(
    CMFCPropertyGridProperty* pThis, CPoint) {
    if (!pThis) return FALSE;
    if (pThis->GetSubItemsCount() > 0) {
        pThis->Expand(!pThis->IsExpanded());
        return TRUE;
    }
    return impl__OnRotateListValue_CMFCPropertyGridProperty__MEAAHH_Z(pThis, TRUE);
}
// Symbol: ?OnEdit@CMFCPropertyGridProperty@@UEAAHPEAUtagPOINT@@@Z
extern "C" int MS_ABI impl__OnEdit_CMFCPropertyGridProperty__UEAAHPEAUtagPOINT___Z(
    CMFCPropertyGridProperty* pThis, POINT*) {
    if (!pThis || !pThis->IsEnabled()) return FALSE;
    PropertyGridPropertyState* propState = FindMutablePropertyGridPropertyState(pThis);
    if (propState && propState->owner) propState->owner->SetCurSel(pThis, TRUE);
    return TRUE;
}
// Symbol: ?PushChar@CMFCPropertyGridProperty@@UEAAHI@Z
extern "C" int MS_ABI impl__PushChar_CMFCPropertyGridProperty__UEAAHI_Z(
    CMFCPropertyGridProperty* pThis, unsigned int nChar) {
    if (!pThis || !pThis->IsEnabled()) return FALSE;
    if (nChar == VK_SPACE || nChar == VK_RETURN) {
        return impl__OnRotateListValue_CMFCPropertyGridProperty__MEAAHH_Z(pThis, TRUE);
    }
    return FALSE;
}
// Symbol: ?EnableSpinControl@CMFCPropertyGridProperty@@QEAAXHHH@Z
extern "C" void MS_ABI impl__EnableSpinControl_CMFCPropertyGridProperty__QEAAXHHH_Z(
    CMFCPropertyGridProperty* pThis, int bEnable, int nMin, int nMax) {
    if (!pThis) return;
    PropertyGridPropertyState& state = EnsurePropertyGridPropertyState(pThis);
    state.spinEnabled = bEnable ? TRUE : FALSE;
    state.spinMin = nMin;
    state.spinMax = nMax;
    TouchPropertyGridCtrl(state.owner, FALSE);
}
// Symbol: ?AdjustInPlaceEditRect@CMFCPropertyGridProperty@@UEAAXAEAVCRect@@0@Z
extern "C" void MS_ABI impl__AdjustInPlaceEditRect_CMFCPropertyGridProperty__UEAAXAEAVCRect__0_Z(
    CMFCPropertyGridProperty*, CRect* rectEdit, CRect* rectSpin) {
    if (rectEdit) rectEdit->DeflateRect(2, 1);
    if (rectSpin && rectEdit) {
        rectSpin->SetRect(rectEdit->right - 16, rectEdit->top, rectEdit->right, rectEdit->bottom);
        rectEdit->right = rectSpin->left;
    }
}
// Symbol: ?AdjustButtonRect@CMFCPropertyGridProperty@@UEAAXXZ
extern "C" void MS_ABI impl__AdjustButtonRect_CMFCPropertyGridProperty__UEAAXXZ(CMFCPropertyGridProperty* pThis) {
    PropertyGridPropertyState* state = FindMutablePropertyGridPropertyState(pThis);
    TouchPropertyGridCtrl(state ? state->owner : nullptr, FALSE);
}
// Symbol: ?OnDrawName@CMFCPropertyGridProperty@@UEAAXPEAVCDC@@VCRect@@@Z
extern "C" void MS_ABI impl__OnDrawName_CMFCPropertyGridProperty__UEAAXPEAVCDC__VCRect___Z(
    CMFCPropertyGridProperty* pThis, CDC* pDC, CRect rect) {
    if (!pThis || !pDC || !pDC->GetSafeHdc()) return;
    RECT nativeRect = NativeRect(rect);
    ::DrawTextW(pDC->GetSafeHdc(), pThis->GetName(), -1, &nativeRect, DT_LEFT | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS | DT_NOPREFIX);
}
// Symbol: ?OnDrawValue@CMFCPropertyGridProperty@@UEAAXPEAVCDC@@VCRect@@@Z
extern "C" void MS_ABI impl__OnDrawValue_CMFCPropertyGridProperty__UEAAXPEAVCDC__VCRect___Z(
    CMFCPropertyGridProperty* pThis, CDC* pDC, CRect rect) {
    if (!pThis || !pDC || !pDC->GetSafeHdc()) return;
    CString value = pThis->FormatProperty();
    RECT nativeRect = NativeRect(rect);
    ::DrawTextW(pDC->GetSafeHdc(), value, -1, &nativeRect, DT_LEFT | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS | DT_NOPREFIX);
}
// Symbol: ?OnDrawDescription@CMFCPropertyGridProperty@@UEAAXPEAVCDC@@VCRect@@@Z
extern "C" void MS_ABI impl__OnDrawDescription_CMFCPropertyGridProperty__UEAAXPEAVCDC__VCRect___Z(
    CMFCPropertyGridProperty* pThis, CDC* pDC, CRect rect) {
    if (!pThis || !pDC || !pDC->GetSafeHdc()) return;
    CString text = pThis->GetNameTooltip();
    RECT nativeRect = NativeRect(rect);
    ::DrawTextW(pDC->GetSafeHdc(), text, -1, &nativeRect, DT_LEFT | DT_TOP | DT_WORDBREAK | DT_NOPREFIX);
}
// Symbol: ?OnDrawButton@CMFCPropertyGridProperty@@UEAAXPEAVCDC@@VCRect@@@Z
extern "C" void MS_ABI impl__OnDrawButton_CMFCPropertyGridProperty__UEAAXPEAVCDC__VCRect___Z(
    CMFCPropertyGridProperty*, CDC* pDC, CRect rect) {
    if (!pDC || !pDC->GetSafeHdc()) return;
    RECT nativeRect = NativeRect(rect);
    ::DrawFrameControl(pDC->GetSafeHdc(), &nativeRect, DFC_BUTTON, DFCS_BUTTONPUSH);
}
// Symbol: ?OnDrawExpandBox@CMFCPropertyGridProperty@@UEAAXPEAVCDC@@VCRect@@@Z
extern "C" void MS_ABI impl__OnDrawExpandBox_CMFCPropertyGridProperty__UEAAXPEAVCDC__VCRect___Z(
    CMFCPropertyGridProperty* pThis, CDC* pDC, CRect rect) {
    if (!pThis || !pDC || !pDC->GetSafeHdc() || pThis->GetSubItemsCount() <= 0) return;
    RECT nativeRect = NativeRect(rect);
    ::DrawFrameControl(pDC->GetSafeHdc(), &nativeRect, DFC_BUTTON,
                       pThis->IsExpanded() ? DFCS_BUTTONPUSH : (DFCS_BUTTONPUSH | DFCS_PUSHED));
    const int midX = (rect.left + rect.right) / 2;
    const int midY = (rect.top + rect.bottom) / 2;
    ::MoveToEx(pDC->GetSafeHdc(), rect.left + 3, midY, nullptr);
    ::LineTo(pDC->GetSafeHdc(), rect.right - 3, midY);
    if (!pThis->IsExpanded()) {
        ::MoveToEx(pDC->GetSafeHdc(), midX, rect.top + 3, nullptr);
        ::LineTo(pDC->GetSafeHdc(), midX, rect.bottom - 3);
    }
}
// Symbol: ?OnSetCursor@CMFCPropertyGridProperty@@UEBAHXZ
extern "C" int MS_ABI impl__OnSetCursor_CMFCPropertyGridProperty__UEBAHXZ(CMFCPropertyGridProperty* pThis) {
    if (!pThis || !pThis->IsEnabled()) return FALSE;
    ::SetCursor(::LoadCursorW(nullptr, MAKEINTRESOURCEW(IDC_ARROW)));
    return TRUE;
}
// Symbol: ?HitTest@CMFCPropertyGridProperty@@QEAAPEAV1@VCPoint@@PEAW4ClickArea@1@@Z
extern "C" CMFCPropertyGridProperty* MS_ABI impl__HitTest_CMFCPropertyGridProperty__QEAAPEAV1_VCPoint__PEAW4ClickArea_1__Z(
    CMFCPropertyGridProperty* pThis, CPoint, int* pClickArea) {
    if (pClickArea) *pClickArea = 0;
    return (pThis && pThis->IsVisible()) ? pThis : nullptr;
}
// Symbol: ?Init@CMFCPropertyGridProperty@@IEAAXXZ
extern "C" void MS_ABI impl__Init_CMFCPropertyGridProperty__IEAAXXZ(CMFCPropertyGridProperty* pThis) {
    if (!pThis) return;
    EnsurePropertyGridPropertyState(pThis);
    pThis->SetModified(FALSE);
}
// Symbol: ?AddTerminalProp@CMFCPropertyGridProperty@@IEAAXAEAV?$CList@PEAVCMFCPropertyGridProperty@@PEAV1@@@@Z
extern "C" void MS_ABI impl__AddTerminalProp_CMFCPropertyGridProperty__IEAAXAEAV__CList_PEAVCMFCPropertyGridProperty__PEAV1____Z(
    CMFCPropertyGridProperty* pThis, CList<CMFCPropertyGridProperty*, CMFCPropertyGridProperty*>* pList) {
    if (!pThis || !pList) return;

    const PropertyGridPropertyState* state = FindPropertyGridPropertyState(pThis);
    if (!state || state->subItems.empty()) {
        pList->AddTail(pThis);
        return;
    }

    for (CMFCPropertyGridProperty* child : state->subItems) {
        impl__AddTerminalProp_CMFCPropertyGridProperty__IEAAXAEAV__CList_PEAVCMFCPropertyGridProperty__PEAV1____Z(child, pList);
    }
}
// Symbol: ??0CMFCPropertyGridProperty@@QEAA@AEBV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@AEBVCOleVariant@@PEB_W_K222@Z
extern "C" void* MS_ABI impl___0CMFCPropertyGridProperty__QEAA_AEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__AEBVCOleVariant__PEB_W_K222_Z(
    CMFCPropertyGridProperty* pThis, const CString* strName, const COleVariant* varValue,
    const wchar_t* lpszDescr, DWORD_PTR dwData, const wchar_t* lpszEditMask,
    const wchar_t* lpszEditTemplate, const wchar_t* lpszValidChars)
{
    return new(pThis) CMFCPropertyGridProperty(*strName, *varValue, lpszDescr, dwData,
                                                lpszEditMask, lpszEditTemplate, lpszValidChars);
}
// Symbol: ??0CMFCPropertyGridProperty@@QEAA@AEBV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@_KH@Z
extern "C" void* MS_ABI impl___0CMFCPropertyGridProperty__QEAA_AEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___KH_Z(
    CMFCPropertyGridProperty* pThis, const CString* strName, DWORD_PTR dwData, int nRowHeight)
{
    return new(pThis) CMFCPropertyGridProperty(*strName, dwData, nRowHeight);
}
CMFCPropertyGridProperty::CMFCPropertyGridProperty(const CString& strName, const COleVariant& varValue,
                                                     const wchar_t* lpszDescr, DWORD_PTR dwData,
                                                     const wchar_t* lpszEditMask,
                                                     const wchar_t* lpszEditTemplate,
                                                     const wchar_t* lpszValidChars)
    : m_varValue(varValue), m_dwData(dwData),
      m_bModified(FALSE), m_bEnabled(TRUE), m_bVisible(TRUE), m_bExpanded(FALSE) {
    m_strName = strName;
    if (lpszDescr) m_strDescr = lpszDescr;
    (void)lpszEditMask; (void)lpszEditTemplate; (void)lpszValidChars;
    memset(_propgridproperty_padding, 0, sizeof(_propgridproperty_padding));
    auto& state = EnsurePropertyGridPropertyState(this);
    state.originalValue.Assign(m_varValue);
    state.hasOriginalValue = TRUE;
}
CMFCPropertyGridProperty::CMFCPropertyGridProperty(const CString& strName, DWORD_PTR dwData, int nRowHeight)
    : m_strName(strName), m_dwData(dwData),
      m_bModified(FALSE), m_bEnabled(TRUE), m_bVisible(TRUE), m_bExpanded(FALSE) {
    (void)nRowHeight;
    memset(_propgridproperty_padding, 0, sizeof(_propgridproperty_padding));
    auto& state = EnsurePropertyGridPropertyState(this);
    state.originalValue.Assign(m_varValue);
    state.hasOriginalValue = TRUE;
}
CMFCPropertyGridProperty::~CMFCPropertyGridProperty() {
    DeletePropertyGridChildren(this);
    RemovePropertyGridPropertyReferences(this);
}
const CString& CMFCPropertyGridProperty::GetName() const { return m_strName; }
const COleVariant& CMFCPropertyGridProperty::GetValue() const { return m_varValue; }
void CMFCPropertyGridProperty::SetValue(const COleVariant& varValue) {
    AssignPropertyGridVariant(m_varValue, varValue);
    m_bModified = IsValueChanged();
    PropertyGridPropertyState* state = FindMutablePropertyGridPropertyState(this);
    if (state && state->owner) {
        TouchPropertyGridCtrl(state->owner, FALSE);
    }
}
BOOL CMFCPropertyGridProperty::IsModified() const { return m_bModified; }
void CMFCPropertyGridProperty::SetModified(BOOL bModified) {
    m_bModified = bModified;
    PropertyGridPropertyState* state = FindMutablePropertyGridPropertyState(this);
    if (state && state->owner) {
        TouchPropertyGridCtrl(state->owner, FALSE);
    }
}
BOOL CMFCPropertyGridProperty::IsEnabled() const { return m_bEnabled; }
void CMFCPropertyGridProperty::SetEnabled(BOOL bEnable) {
    m_bEnabled = bEnable;
    PropertyGridPropertyState* state = FindMutablePropertyGridPropertyState(this);
    if (state && state->owner) {
        TouchPropertyGridCtrl(state->owner, FALSE);
    }
}
BOOL CMFCPropertyGridProperty::IsVisible() const { return m_bVisible; }
void CMFCPropertyGridProperty::Show(BOOL bShow) { m_bVisible = bShow; }
void CMFCPropertyGridProperty::Show(BOOL bShow, BOOL bAdjustLayout) {
    Show(bShow);
    PropertyGridPropertyState* state = FindMutablePropertyGridPropertyState(this);
    if (state && state->owner) {
        TouchPropertyGridCtrl(state->owner, bAdjustLayout);
    }
}
int CMFCPropertyGridProperty::AddSubItem(CMFCPropertyGridProperty* pProp) {
    if (!pProp || pProp == this) return -1;
    if (IsPropertyGridAncestorOf(pProp, this)) return -1;

    auto& parentState = EnsurePropertyGridPropertyState(this);
    auto& subItems = parentState.subItems;
    auto it = std::find(subItems.begin(), subItems.end(), pProp);
    if (it != subItems.end()) return static_cast<int>(it - subItems.begin());

    DetachPropertyFromParent(pProp);
    DetachPropertyFromGrid(pProp);
    EnsurePropertyGridPropertyState(pProp).parent = this;
    SetPropertyGridOwnerRecursive(pProp, parentState.owner);

    subItems.push_back(pProp);
    TouchPropertyGridCtrl(parentState.owner, TRUE);
    return static_cast<int>(subItems.size() - 1);
}
int CMFCPropertyGridProperty::GetSubItemsCount() const {
    const PropertyGridPropertyState* state = FindPropertyGridPropertyState(this);
    return state ? static_cast<int>(state->subItems.size()) : 0;
}
CMFCPropertyGridProperty* CMFCPropertyGridProperty::GetSubItem(int nIndex) const {
    const PropertyGridPropertyState* state = FindPropertyGridPropertyState(this);
    if (!state || nIndex < 0 || nIndex >= static_cast<int>(state->subItems.size())) return nullptr;
    return state->subItems[static_cast<size_t>(nIndex)];
}
void CMFCPropertyGridProperty::RemoveAllSubItems() {
    auto& state = EnsurePropertyGridPropertyState(this);
    CMFCPropertyGridCtrl* owner = state.owner;
    if (owner) {
        PropertyGridCtrlState* ctrlState = FindMutablePropertyGridCtrlState(owner);
        if (ctrlState) {
            for (CMFCPropertyGridProperty* child : state.subItems) {
                if (ctrlState->current == child || IsPropertyGridAncestorOf(child, ctrlState->current)) {
                    ctrlState->current = nullptr;
                    break;
                }
            }
        }
    }

    std::vector<CMFCPropertyGridProperty*> children = state.subItems;
    state.subItems.clear();
    for (CMFCPropertyGridProperty* child : children) {
        PropertyGridPropertyState* childState = FindMutablePropertyGridPropertyState(child);
        if (childState && childState->parent == this) {
            childState->parent = nullptr;
            SetPropertyGridOwnerRecursive(child, nullptr);
        }
        delete child;
    }
    TouchPropertyGridCtrl(owner, TRUE);
}
void CMFCPropertyGridProperty::Expand(BOOL bExpand) {
    m_bExpanded = bExpand;
    PropertyGridPropertyState* state = FindMutablePropertyGridPropertyState(this);
    if (state && state->owner) {
        TouchPropertyGridCtrl(state->owner, TRUE);
    }
}
BOOL CMFCPropertyGridProperty::IsExpanded() const { return m_bExpanded; }
void CMFCPropertyGridProperty::SetData(DWORD_PTR dwData) {
    m_dwData = dwData;
}
DWORD_PTR CMFCPropertyGridProperty::GetData() const { return m_dwData; }
BOOL CMFCPropertyGridProperty::AddOption(const wchar_t* lpszOption, BOOL bInsertUnique) {
    if (!lpszOption) return FALSE;
    auto& state = EnsurePropertyGridPropertyState(this);
    std::wstring option(lpszOption);
    if (bInsertUnique) {
        auto it = std::find(state.options.begin(), state.options.end(), option);
        if (it != state.options.end()) {
            return TRUE;
        }
    }
    state.options.push_back(std::move(option));
    if (state.selectedOption < 0) {
        state.selectedOption = 0;
    }
    TouchPropertyGridCtrl(state.owner, FALSE);
    return TRUE;
}
const wchar_t* CMFCPropertyGridProperty::GetOption(int nIndex) const {
    const PropertyGridPropertyState* state = FindPropertyGridPropertyState(this);
    if (!state || nIndex < 0 || nIndex >= static_cast<int>(state->options.size())) return nullptr;
    return state->options[static_cast<size_t>(nIndex)].c_str();
}
int CMFCPropertyGridProperty::GetOptionCount() const {
    const PropertyGridPropertyState* state = FindPropertyGridPropertyState(this);
    return state ? static_cast<int>(state->options.size()) : 0;
}
void CMFCPropertyGridProperty::RemoveAllOptions() {
    auto& state = EnsurePropertyGridPropertyState(this);
    state.options.clear();
    state.selectedOption = -1;
    TouchPropertyGridCtrl(state.owner, FALSE);
}
void CMFCPropertyGridProperty::Enable(BOOL bEnable) {
    SetEnabled(bEnable);
}
int CMFCPropertyGridProperty::GetExpandedSubItems(BOOL bIncludeHidden) const {
    return CountExpandedPropertyGridSubItems(this, bIncludeHidden);
}
int CMFCPropertyGridProperty::GetHierarchyLevel() const {
    int level = 0;
    const PropertyGridPropertyState* state = FindPropertyGridPropertyState(this);
    while (state && state->parent) {
        ++level;
        state = FindPropertyGridPropertyState(state->parent);
    }
    return level;
}
BOOL CMFCPropertyGridProperty::IsParentExpanded() const {
    const PropertyGridPropertyState* state = FindPropertyGridPropertyState(this);
    while (state && state->parent) {
        if (!state->parent->IsExpanded()) return FALSE;
        state = FindPropertyGridPropertyState(state->parent);
    }
    return TRUE;
}
BOOL CMFCPropertyGridProperty::IsSubItem(CMFCPropertyGridProperty* pProp) const {
    return IsPropertyGridAncestorOf(this, pProp);
}
BOOL CMFCPropertyGridProperty::IsSubItem(void* pProp) const {
    return IsSubItem(static_cast<CMFCPropertyGridProperty*>(pProp));
}
CMFCPropertyGridProperty* CMFCPropertyGridProperty::FindSubItemByData(DWORD_PTR dwData) const {
    return FindPropertyGridSubItemByData(const_cast<CMFCPropertyGridProperty*>(this), dwData);
}
CString CMFCPropertyGridProperty::FormatProperty() {
    const PropertyGridPropertyState* state = FindPropertyGridPropertyState(this);
    const PropertyGridCtrlState* ctrlState = (state && state->owner) ? FindPropertyGridCtrlState(state->owner) : nullptr;
    return CString(PropertyGridVariantToString(m_varValue, ctrlState).c_str());
}
CString CMFCPropertyGridProperty::GetNameTooltip() {
    return m_strName;
}
CString CMFCPropertyGridProperty::GetValueTooltip() {
    return FormatProperty();
}
BOOL CMFCPropertyGridProperty::IsValueChanged() const {
    const PropertyGridPropertyState* state = FindPropertyGridPropertyState(this);
    if (!state || !state->hasOriginalValue) return m_bModified;
    return !PropertyGridValuesEqual(m_varValue, state->originalValue.value);
}
void CMFCPropertyGridProperty::ResetOriginalValue() {
    PropertyGridPropertyState* state = FindMutablePropertyGridPropertyState(this);
    if (state && state->hasOriginalValue) {
        AssignPropertyGridVariant(m_varValue, state->originalValue.value);
        PropertyGridPropertyState* updatedState = FindMutablePropertyGridPropertyState(this);
        if (updatedState && updatedState->owner) {
            TouchPropertyGridCtrl(updatedState->owner, FALSE);
        }
    }
    SetModified(FALSE);
}
void CMFCPropertyGridProperty::SetOriginalValue(const COleVariant& varValue) {
    auto& state = EnsurePropertyGridPropertyState(this);
    state.originalValue.Assign(varValue);
    state.hasOriginalValue = TRUE;
    m_bModified = IsValueChanged();
    TouchPropertyGridCtrl(state.owner, FALSE);
}
void CMFCPropertyGridProperty::SetName(const wchar_t* lpszName, BOOL bRedraw) {
    m_strName = lpszName ? lpszName : L"";
    PropertyGridPropertyState* state = FindMutablePropertyGridPropertyState(this);
    if (state && state->owner && bRedraw) {
        TouchPropertyGridCtrl(state->owner, TRUE);
    }
}
void CMFCPropertyGridProperty::Redraw() {
    PropertyGridPropertyState* state = FindMutablePropertyGridPropertyState(this);
    if (state && state->owner) {
        TouchPropertyGridCtrl(state->owner, FALSE);
    }
}
BOOL CMFCPropertyGridProperty::RemoveSubItem(CMFCPropertyGridProperty*& pProp, BOOL bDelete) {
    if (!pProp) return FALSE;
    auto& state = EnsurePropertyGridPropertyState(this);
    auto it = std::find(state.subItems.begin(), state.subItems.end(), pProp);
    if (it == state.subItems.end()) return FALSE;

    CMFCPropertyGridProperty* removed = *it;
    state.subItems.erase(it);
    PropertyGridPropertyState* childState = FindMutablePropertyGridPropertyState(removed);
    if (childState && childState->parent == this) {
        childState->parent = nullptr;
        SetPropertyGridOwnerRecursive(removed, nullptr);
    }
    if (bDelete) {
        delete removed;
    }
    pProp = nullptr;
    TouchPropertyGridCtrl(state.owner, TRUE);
    return TRUE;
}
BOOL CMFCPropertyGridProperty::RemoveSubItem(void*& pProp, BOOL bDelete) {
    CMFCPropertyGridProperty* typed = static_cast<CMFCPropertyGridProperty*>(pProp);
    BOOL result = RemoveSubItem(typed, bDelete);
    pProp = typed;
    return result;
}
void CMFCPropertyGridProperty::ExpandDeep(BOOL bExpand) {
    ExpandPropertyRecursive(this, bExpand);
}
void CMFCPropertyGridProperty::SetOwnerList(CMFCPropertyGridCtrl* pWndList) {
    SetPropertyGridOwnerRecursive(this, pWndList);
}
void CMFCPropertyGridProperty::SetModifiedFlag() {
    SetModified(IsValueChanged());
}
BOOL CMFCPropertyGridProperty::OnUpdateValue() {
    SetModified(TRUE);
    return TRUE;
}
BOOL CMFCPropertyGridProperty::OnEndEdit() {
    SetModified(IsValueChanged());
    return TRUE;
}
void CMFCPropertyGridProperty::OnSelectCombo() {
    PropertyGridPropertyState* state = FindMutablePropertyGridPropertyState(this);
    if (!state || state->options.empty()) return;
    if (state->selectedOption < 0 || state->selectedOption >= static_cast<int>(state->options.size())) {
        state->selectedOption = 0;
    }
    COleVariant value(state->options[static_cast<size_t>(state->selectedOption)].c_str());
    SetValue(value);
}
void CMFCPropertyGridProperty::OnCloseCombo() {
    SetModified(IsValueChanged());
}
BOOL CMFCPropertyGridProperty::OnRotateListValue(BOOL bForward) {
    PropertyGridPropertyState* state = FindMutablePropertyGridPropertyState(this);
    if (!state || state->options.empty()) return FALSE;

    const PropertyGridCtrlState* ctrlState = state->owner ? FindPropertyGridCtrlState(state->owner) : nullptr;
    const std::wstring current = PropertyGridVariantToString(m_varValue, ctrlState);
    auto it = std::find(state->options.begin(), state->options.end(), current);
    int index = it == state->options.end()
        ? (state->selectedOption < 0 ? 0 : state->selectedOption)
        : static_cast<int>(it - state->options.begin());
    if (bForward) {
        index = (index + 1) % static_cast<int>(state->options.size());
    } else {
        index = (index + static_cast<int>(state->options.size()) - 1) % static_cast<int>(state->options.size());
    }
    state->selectedOption = index;
    COleVariant value(state->options[static_cast<size_t>(index)].c_str());
    SetValue(value);
    return TRUE;
}

// === Moved from ManualThunks.cpp ===
// Symbol: ?AddSubItem@CMFCPropertyGridProperty@@QEAAHPEAV1@@Z
extern "C" int MS_ABI impl__AddSubItem_CMFCPropertyGridProperty__QEAAHPEAV1__Z(void* pThis, void* p0) {
    (void)pThis;
    (void)p0;
    return 0;
}

// ?CreateCombo@CMFCPropertyGridProperty@@MEAAPEAVCComboBox@@PEAVCWnd@@VCRect@@@Z
extern "C" void* MS_ABI impl__CreateCombo_CMFCPropertyGridProperty__MEAAPEAVCComboBox__PEAVCWnd__VCRect___Z(void* /*class*/* p0, void* /*class*/ p1) {
    return nullptr;
}

// ?CreateInPlaceEdit@CMFCPropertyGridProperty@@UEAAPEAVCWnd@@VCRect@@AEAH@Z
extern "C" void* MS_ABI impl__CreateInPlaceEdit_CMFCPropertyGridProperty__UEAAPEAVCWnd__VCRect__AEAH_Z(void* /*class*/ p0, int* p1) {
    return nullptr;
}

// ?CreateSpinControl@CMFCPropertyGridProperty@@UEAAPEAVCSpinButtonCtrl@@VCRect@@@Z
extern "C" void* MS_ABI impl__CreateSpinControl_CMFCPropertyGridProperty__UEAAPEAVCSpinButtonCtrl__VCRect___Z(void* /*class*/ p0) {
    return nullptr;
}

// ?OnCtlColor@CMFCPropertyGridProperty@@MEAAPEAUHBRUSH__@@PEAVCDC@@I@Z
extern "C" void* MS_ABI impl__OnCtlColor_CMFCPropertyGridProperty__MEAAPEAUHBRUSH____PEAVCDC__I_Z(void* /*class*/* p0, unsigned int p1) {
    return nullptr;
}

// ?OnDestroyWindow@CMFCPropertyGridProperty@@MEAAXXZ
extern "C" void MS_ABI impl__OnDestroyWindow_CMFCPropertyGridProperty__MEAAXXZ() {}

// ?Reposition@CMFCPropertyGridProperty@@IEAAXAEAH@Z
extern "C" void MS_ABI impl__Reposition_CMFCPropertyGridProperty__IEAAXAEAH_Z(int* p0) {}

// ?SetFlags@CMFCPropertyGridProperty@@IEAAXXZ
extern "C" void MS_ABI impl__SetFlags_CMFCPropertyGridProperty__IEAAXXZ() {}

// ?TextToVar@CMFCPropertyGridProperty@@MEAAHAEBV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@@Z
extern "C" int MS_ABI impl__TextToVar_CMFCPropertyGridProperty__MEAAHAEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(const void* /*class*/* p0, void** p1, void* p2) {
    return 0;
}

