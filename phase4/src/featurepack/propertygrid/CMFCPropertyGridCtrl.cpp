// CMFCPropertyGridCtrl — OpenMFC implementation.
// Sources: mfccore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/MfccoreSupport.h"


// Implementations this unit calls that are defined with their own class.
extern "C" void MS_ABI impl__OnDrawName_CMFCPropertyGridProperty__UEAAXPEAVCDC__VCRect___Z(
    CMFCPropertyGridProperty* pThis, CDC* pDC, CRect rect);
extern "C" void MS_ABI impl__OnDrawValue_CMFCPropertyGridProperty__UEAAXPEAVCDC__VCRect___Z(
    CMFCPropertyGridProperty* pThis, CDC* pDC, CRect rect);

// Symbol: ?get_accChildCount@CMFCPropertyGridCtrl@@UEAAJPEAJ@Z
extern "C" HRESULT MS_ABI impl__get_accChildCount_CMFCPropertyGridCtrl__UEAAJPEAJ_Z(
    CMFCPropertyGridCtrl* pThis, long* pCountChildren) {
    if (!pCountChildren) return E_POINTER;
    *pCountChildren = pThis ? static_cast<long>(VisiblePropertyGridItems(pThis).size()) : 0;
    return S_OK;
}
// Symbol: ?get_accChild@CMFCPropertyGridCtrl@@UEAAJUtagVARIANT@@PEAPEAUIDispatch@@@Z
extern "C" HRESULT MS_ABI impl__get_accChild_CMFCPropertyGridCtrl__UEAAJUtagVARIANT__PEAPEAUIDispatch___Z(
    CMFCPropertyGridCtrl*, VARIANT varChild, IDispatch** ppdispChild) {
    if (!ppdispChild) return E_POINTER;
    *ppdispChild = nullptr;
    return PropertyGridChildIDFromVariant(varChild) >= CHILDID_SELF ? S_FALSE : E_INVALIDARG;
}
// Symbol: ?get_accName@CMFCPropertyGridCtrl@@UEAAJUtagVARIANT@@PEAPEA_W@Z
extern "C" HRESULT MS_ABI impl__get_accName_CMFCPropertyGridCtrl__UEAAJUtagVARIANT__PEAPEA_W_Z(
    CMFCPropertyGridCtrl* pThis, VARIANT varChild, BSTR* pszName) {
    if (!pszName) return E_POINTER;
    *pszName = nullptr;
    int childID = PropertyGridChildIDFromVariant(varChild);
    if (childID == CHILDID_SELF) return AllocPropertyGridString(L"Property Grid", pszName);
    CMFCPropertyGridProperty* prop = PropertyGridPropertyFromChildID(pThis, childID);
    if (!prop) return E_INVALIDARG;
    return AllocPropertyGridString(static_cast<const wchar_t*>(prop->GetName()), pszName);
}
// Symbol: ?get_accValue@CMFCPropertyGridCtrl@@UEAAJUtagVARIANT@@PEAPEA_W@Z
extern "C" HRESULT MS_ABI impl__get_accValue_CMFCPropertyGridCtrl__UEAAJUtagVARIANT__PEAPEA_W_Z(
    CMFCPropertyGridCtrl* pThis, VARIANT varChild, BSTR* pszValue) {
    if (!pszValue) return E_POINTER;
    *pszValue = nullptr;
    int childID = PropertyGridChildIDFromVariant(varChild);
    CMFCPropertyGridProperty* prop = childID == CHILDID_SELF ? (FindPropertyGridCtrlState(pThis) ? FindPropertyGridCtrlState(pThis)->current : nullptr)
                                                             : PropertyGridPropertyFromChildID(pThis, childID);
    if (!prop) return S_FALSE;
    CString value = prop->FormatProperty();
    return AllocPropertyGridString(static_cast<const wchar_t*>(value), pszValue);
}
// Symbol: ?get_accDescription@CMFCPropertyGridCtrl@@UEAAJUtagVARIANT@@PEAPEA_W@Z
extern "C" HRESULT MS_ABI impl__get_accDescription_CMFCPropertyGridCtrl__UEAAJUtagVARIANT__PEAPEA_W_Z(
    CMFCPropertyGridCtrl* pThis, VARIANT varChild, BSTR* pszDescription) {
    return impl__get_accName_CMFCPropertyGridCtrl__UEAAJUtagVARIANT__PEAPEA_W_Z(pThis, varChild, pszDescription);
}
// Symbol: ?get_accDefaultAction@CMFCPropertyGridCtrl@@UEAAJUtagVARIANT@@PEAPEA_W@Z
extern "C" HRESULT MS_ABI impl__get_accDefaultAction_CMFCPropertyGridCtrl__UEAAJUtagVARIANT__PEAPEA_W_Z(
    CMFCPropertyGridCtrl* pThis, VARIANT varChild, BSTR* pszDefaultAction) {
    int childID = PropertyGridChildIDFromVariant(varChild);
    if (childID == CHILDID_SELF) return S_FALSE;
    CMFCPropertyGridProperty* prop = PropertyGridPropertyFromChildID(pThis, childID);
    if (!prop) return E_INVALIDARG;
    return AllocPropertyGridString(prop->GetSubItemsCount() > 0 ? L"Expand or collapse" : L"Edit", pszDefaultAction);
}
// Symbol: ?get_accRole@CMFCPropertyGridCtrl@@UEAAJUtagVARIANT@@PEAU2@@Z
extern "C" HRESULT MS_ABI impl__get_accRole_CMFCPropertyGridCtrl__UEAAJUtagVARIANT__PEAU2__Z(
    CMFCPropertyGridCtrl* pThis, VARIANT varChild, VARIANT* pvarRole) {
    (void)pThis;
    if (!pvarRole) return E_POINTER;
    int childID = PropertyGridChildIDFromVariant(varChild);
    if (childID < CHILDID_SELF) return E_INVALIDARG;
    VariantInit(pvarRole);
    pvarRole->vt = VT_I4;
    pvarRole->lVal = childID == CHILDID_SELF ? ROLE_SYSTEM_OUTLINE : ROLE_SYSTEM_OUTLINEITEM;
    return S_OK;
}
// Symbol: ?get_accState@CMFCPropertyGridCtrl@@UEAAJUtagVARIANT@@PEAU2@@Z
extern "C" HRESULT MS_ABI impl__get_accState_CMFCPropertyGridCtrl__UEAAJUtagVARIANT__PEAU2__Z(
    CMFCPropertyGridCtrl* pThis, VARIANT varChild, VARIANT* pvarState) {
    if (!pvarState) return E_POINTER;
    int childID = PropertyGridChildIDFromVariant(varChild);
    if (childID < CHILDID_SELF) return E_INVALIDARG;
    VariantInit(pvarState);
    pvarState->vt = VT_I4;
    pvarState->lVal = 0;
    if (childID == CHILDID_SELF) return S_OK;

    CMFCPropertyGridProperty* prop = PropertyGridPropertyFromChildID(pThis, childID);
    if (!prop) return E_INVALIDARG;
    const PropertyGridCtrlState* ctrlState = FindPropertyGridCtrlState(pThis);
    if (!prop->IsEnabled()) pvarState->lVal |= STATE_SYSTEM_UNAVAILABLE;
    if (!prop->IsVisible()) pvarState->lVal |= STATE_SYSTEM_INVISIBLE;
    if (ctrlState && ctrlState->current == prop) pvarState->lVal |= STATE_SYSTEM_SELECTED | STATE_SYSTEM_FOCUSED;
    if (prop->GetSubItemsCount() > 0) pvarState->lVal |= prop->IsExpanded() ? STATE_SYSTEM_EXPANDED : STATE_SYSTEM_COLLAPSED;
    return S_OK;
}
// Symbol: ?get_accFocus@CMFCPropertyGridCtrl@@UEAAJPEAUtagVARIANT@@@Z
extern "C" HRESULT MS_ABI impl__get_accFocus_CMFCPropertyGridCtrl__UEAAJPEAUtagVARIANT___Z(
    CMFCPropertyGridCtrl* pThis, VARIANT* pvarChild) {
    if (!pvarChild) return E_POINTER;
    const PropertyGridCtrlState* state = FindPropertyGridCtrlState(pThis);
    SetPropertyGridVariantChild(pvarChild, state ? PropertyGridChildIDFromProperty(pThis, state->current) : CHILDID_SELF);
    return S_OK;
}
// Symbol: ?get_accSelection@CMFCPropertyGridCtrl@@UEAAJPEAUtagVARIANT@@@Z
extern "C" HRESULT MS_ABI impl__get_accSelection_CMFCPropertyGridCtrl__UEAAJPEAUtagVARIANT___Z(
    CMFCPropertyGridCtrl* pThis, VARIANT* pvarChildren) {
    return impl__get_accFocus_CMFCPropertyGridCtrl__UEAAJPEAUtagVARIANT___Z(pThis, pvarChildren);
}
// Symbol: ?accSelect@CMFCPropertyGridCtrl@@UEAAJJUtagVARIANT@@@Z
extern "C" HRESULT MS_ABI impl__accSelect_CMFCPropertyGridCtrl__UEAAJJUtagVARIANT___Z(
    CMFCPropertyGridCtrl* pThis, long, VARIANT varChild) {
    int childID = PropertyGridChildIDFromVariant(varChild);
    CMFCPropertyGridProperty* prop = PropertyGridPropertyFromChildID(pThis, childID);
    if (!prop) return E_INVALIDARG;
    pThis->SetCurSel(prop, TRUE);
    return S_OK;
}
// Symbol: ?accHitTest@CMFCPropertyGridCtrl@@UEAAJJJPEAUtagVARIANT@@@Z
extern "C" HRESULT MS_ABI impl__accHitTest_CMFCPropertyGridCtrl__UEAAJJJPEAUtagVARIANT___Z(
    CMFCPropertyGridCtrl* pThis, long xLeft, long yTop, VARIANT* pvarChild) {
    if (!pvarChild) return E_POINTER;
    POINT pt{xLeft, yTop};
    if (pThis && pThis->GetSafeHwnd()) ::ScreenToClient(pThis->GetSafeHwnd(), &pt);
    CMFCPropertyGridProperty* prop = PropertyGridHitTestByPoint(pThis, CPoint(pt.x, pt.y));
    SetPropertyGridVariantChild(pvarChild, PropertyGridChildIDFromProperty(pThis, prop));
    return S_OK;
}
// Symbol: ?accLocation@CMFCPropertyGridCtrl@@UEAAJPEAJ000UtagVARIANT@@@Z
extern "C" HRESULT MS_ABI impl__accLocation_CMFCPropertyGridCtrl__UEAAJPEAJ000UtagVARIANT___Z(
    CMFCPropertyGridCtrl* pThis, long* pxLeft, long* pyTop, long* pcxWidth, long* pcyHeight, VARIANT varChild) {
    if (!pxLeft || !pyTop || !pcxWidth || !pcyHeight) return E_POINTER;
    int childID = PropertyGridChildIDFromVariant(varChild);
    CRect itemRect = PropertyGridItemRect(pThis, childID);
    POINT pt{itemRect.left, itemRect.top};
    if (pThis && pThis->GetSafeHwnd()) ::ClientToScreen(pThis->GetSafeHwnd(), &pt);
    *pxLeft = pt.x;
    *pyTop = pt.y;
    *pcxWidth = itemRect.Width();
    *pcyHeight = itemRect.Height();
    return S_OK;
}
// Symbol: ?get_accHelp@CMFCPropertyGridCtrl@@UEAAJUtagVARIANT@@PEAPEA_W@Z
extern "C" HRESULT MS_ABI impl__get_accHelp_CMFCPropertyGridCtrl__UEAAJUtagVARIANT__PEAPEA_W_Z(
    CMFCPropertyGridCtrl*, VARIANT, BSTR* pszHelp) {
    if (!pszHelp) return E_POINTER;
    *pszHelp = nullptr;
    return S_FALSE;
}
// Symbol: ?get_accHelpTopic@CMFCPropertyGridCtrl@@UEAAJPEAPEA_WUtagVARIANT@@PEAJ@Z
extern "C" HRESULT MS_ABI impl__get_accHelpTopic_CMFCPropertyGridCtrl__UEAAJPEAPEA_WUtagVARIANT__PEAJ_Z(
    CMFCPropertyGridCtrl*, BSTR* pszHelpFile, VARIANT, long* pidTopic) {
    if (pszHelpFile) *pszHelpFile = nullptr;
    if (pidTopic) *pidTopic = 0;
    return S_FALSE;
}
// Symbol: ?get_accKeyboardShortcut@CMFCPropertyGridCtrl@@UEAAJUtagVARIANT@@PEAPEA_W@Z
extern "C" HRESULT MS_ABI impl__get_accKeyboardShortcut_CMFCPropertyGridCtrl__UEAAJUtagVARIANT__PEAPEA_W_Z(
    CMFCPropertyGridCtrl*, VARIANT, BSTR* pszKeyboardShortcut) {
    if (!pszKeyboardShortcut) return E_POINTER;
    *pszKeyboardShortcut = nullptr;
    return S_FALSE;
}
// Symbol: ?EditItem@CMFCPropertyGridCtrl@@UEAAHPEAVCMFCPropertyGridProperty@@PEAUtagPOINT@@@Z
extern "C" int MS_ABI impl__EditItem_CMFCPropertyGridCtrl__UEAAHPEAVCMFCPropertyGridProperty__PEAUtagPOINT___Z(
    CMFCPropertyGridCtrl* pThis, CMFCPropertyGridProperty* pProp, POINT* point) {
    (void)point;
    if (!pThis || !pProp || !pProp->IsEnabled()) return FALSE;
    pThis->SetCurSel(pProp, TRUE);
    return TRUE;
}
// Symbol: ?EndEditItem@CMFCPropertyGridCtrl@@UEAAHH@Z
extern "C" int MS_ABI impl__EndEditItem_CMFCPropertyGridCtrl__UEAAHH_Z(CMFCPropertyGridCtrl* pThis, int bUpdateData) {
    const PropertyGridCtrlState* state = FindPropertyGridCtrlState(pThis);
    if (!state || !state->current) return FALSE;
    return bUpdateData ? state->current->OnUpdateValue() : state->current->OnEndEdit();
}
// Symbol: ?EnsureVisible@CMFCPropertyGridCtrl@@QEAAXPEAVCMFCPropertyGridProperty@@H@Z
extern "C" void MS_ABI impl__EnsureVisible_CMFCPropertyGridCtrl__QEAAXPEAVCMFCPropertyGridProperty__H_Z(
    CMFCPropertyGridCtrl* pThis, CMFCPropertyGridProperty* pProp, int bExpandParents) {
    if (!pThis || !pProp) return;
    if (bExpandParents) {
        PropertyGridPropertyState* state = FindMutablePropertyGridPropertyState(pProp);
        while (state && state->parent) {
            state->parent->Expand(TRUE);
            state = FindMutablePropertyGridPropertyState(state->parent);
        }
    }
    pThis->SetCurSel(pProp, TRUE);
}
// Symbol: ?HitTest@CMFCPropertyGridCtrl@@QEBAPEAVCMFCPropertyGridProperty@@VCPoint@@PEAW4ClickArea@2@H@Z
extern "C" CMFCPropertyGridProperty* MS_ABI impl__HitTest_CMFCPropertyGridCtrl__QEBAPEAVCMFCPropertyGridProperty__VCPoint__PEAW4ClickArea_2_H_Z(
    CMFCPropertyGridCtrl* pThis, CPoint point, int* pClickArea, int) {
    if (pClickArea) *pClickArea = point.x < 120 ? 1 : 2;
    return PropertyGridHitTestByPoint(pThis, point);
}
// Symbol: ?OnDrawProperty@CMFCPropertyGridCtrl@@UEBAHPEAVCDC@@PEAVCMFCPropertyGridProperty@@@Z
extern "C" int MS_ABI impl__OnDrawProperty_CMFCPropertyGridCtrl__UEBAHPEAVCDC__PEAVCMFCPropertyGridProperty___Z(
    CMFCPropertyGridCtrl* pThis, CDC* pDC, CMFCPropertyGridProperty* pProp) {
    if (!pDC || !pDC->GetSafeHdc() || !pProp) return FALSE;
    int childID = PropertyGridChildIDFromProperty(pThis, pProp);
    CRect rect = PropertyGridItemRect(pThis, childID);
    COLORREF back = pProp->IsEnabled() ? ::GetSysColor(COLOR_WINDOW) : ::GetSysColor(COLOR_BTNFACE);
    ::SetBkColor(pDC->GetSafeHdc(), back);
    RECT nativeRect = NativeRect(rect);
    ::ExtTextOutW(pDC->GetSafeHdc(), 0, 0, ETO_OPAQUE, &nativeRect, nullptr, 0, nullptr);
    CRect nameRect(rect.left + 4 + pProp->GetHierarchyLevel() * 12, rect.top, rect.left + rect.Width() / 2, rect.bottom);
    CRect valueRect(rect.left + rect.Width() / 2 + 4, rect.top, rect.right - 2, rect.bottom);
    impl__OnDrawName_CMFCPropertyGridProperty__UEAAXPEAVCDC__VCRect___Z(pProp, pDC, nameRect);
    impl__OnDrawValue_CMFCPropertyGridProperty__UEAAXPEAVCDC__VCRect___Z(pProp, pDC, valueRect);
    return TRUE;
}
// Symbol: ?OnDraw@CMFCPropertyGridCtrl@@MEAAXPEAVCDC@@@Z
extern "C" void MS_ABI impl__OnDraw_CMFCPropertyGridCtrl__MEAAXPEAVCDC___Z(CMFCPropertyGridCtrl* pThis, CDC* pDC) {
    if (!pThis || !pDC || !pDC->GetSafeHdc()) return;
    for (CMFCPropertyGridProperty* prop : VisiblePropertyGridItems(pThis)) {
        impl__OnDrawProperty_CMFCPropertyGridCtrl__UEBAHPEAVCDC__PEAVCMFCPropertyGridProperty___Z(pThis, pDC, prop);
    }
}
// Symbol: ?OnFillBackground@CMFCPropertyGridCtrl@@MEAAXPEAVCDC@@VCRect@@@Z
extern "C" void MS_ABI impl__OnFillBackground_CMFCPropertyGridCtrl__MEAAXPEAVCDC__VCRect___Z(
    CMFCPropertyGridCtrl*, CDC* pDC, CRect rect) {
    if (!pDC || !pDC->GetSafeHdc()) return;
    RECT nativeRect = NativeRect(rect);
    ::ExtTextOutW(pDC->GetSafeHdc(), 0, 0, ETO_OPAQUE, &nativeRect, nullptr, 0, nullptr);
}
// Symbol: ?OnDrawList@CMFCPropertyGridCtrl@@MEAAXPEAVCDC@@@Z
extern "C" void MS_ABI impl__OnDrawList_CMFCPropertyGridCtrl__MEAAXPEAVCDC___Z(CMFCPropertyGridCtrl* pThis, CDC* pDC) {
    impl__OnDraw_CMFCPropertyGridCtrl__MEAAXPEAVCDC___Z(pThis, pDC);
}
// Symbol: ?SetCustomColors@CMFCPropertyGridCtrl@@QEAAXKKKKKKK@Z
extern "C" void MS_ABI impl__SetCustomColors_CMFCPropertyGridCtrl__QEAAXKKKKKKK_Z(
    CMFCPropertyGridCtrl* pThis, COLORREF c1, COLORREF c2, COLORREF c3, COLORREF c4, COLORREF c5, COLORREF c6, COLORREF c7) {
    if (!pThis) return;
    PropertyGridCtrlState& state = EnsurePropertyGridCtrlState(pThis);
    COLORREF values[7] = {c1, c2, c3, c4, c5, c6, c7};
    std::copy(std::begin(values), std::end(values), std::begin(state.customColors));
}
// Symbol: ?GetCustomColors@CMFCPropertyGridCtrl@@QEAAXAEAK000000@Z
extern "C" void MS_ABI impl__GetCustomColors_CMFCPropertyGridCtrl__QEAAXAEAK000000_Z(
    CMFCPropertyGridCtrl* pThis, COLORREF* c1, COLORREF* c2, COLORREF* c3, COLORREF* c4, COLORREF* c5, COLORREF* c6, COLORREF* c7) {
    const PropertyGridCtrlState* state = FindPropertyGridCtrlState(pThis);
    const COLORREF defaults[7] = {RGB(0, 0, 0), RGB(128, 128, 128), RGB(128, 0, 0), RGB(0, 128, 0), RGB(0, 0, 128), RGB(255, 255, 255), RGB(255, 255, 0)};
    const COLORREF* colors = state ? state->customColors : defaults;
    if (c1) *c1 = colors[0];
    if (c2) *c2 = colors[1];
    if (c3) *c3 = colors[2];
    if (c4) *c4 = colors[3];
    if (c5) *c5 = colors[4];
    if (c6) *c6 = colors[5];
    if (c7) *c7 = colors[6];
}
// Symbol: ?EnableDescriptionArea@CMFCPropertyGridCtrl@@QEAAXH@Z
extern "C" void MS_ABI impl__EnableDescriptionArea_CMFCPropertyGridCtrl__QEAAXH_Z(CMFCPropertyGridCtrl* pThis, int bEnable) {
    if (pThis) pThis->SetDescriptionRows(bEnable ? 2 : 0);
}
// Symbol: ?Init@CMFCPropertyGridCtrl@@MEAAXXZ
extern "C" void MS_ABI impl__Init_CMFCPropertyGridCtrl__MEAAXXZ(CMFCPropertyGridCtrl* pThis) {
    if (pThis) EnsurePropertyGridCtrlState(pThis);
}
// Symbol: ?NotifyAccessibility@CMFCPropertyGridCtrl@@MEAAXPEAVCMFCPropertyGridProperty@@@Z
extern "C" void MS_ABI impl__NotifyAccessibility_CMFCPropertyGridCtrl__MEAAXPEAVCMFCPropertyGridProperty___Z(
    CMFCPropertyGridCtrl* pThis, CMFCPropertyGridProperty* pProp) {
    (void)pProp;
    TouchPropertyGridCtrl(pThis, FALSE);
}
// Symbol: ?CalcEditMargin@CMFCPropertyGridCtrl@@IEAAXXZ
extern "C" void MS_ABI impl__CalcEditMargin_CMFCPropertyGridCtrl__IEAAXXZ(CMFCPropertyGridCtrl* pThis) {
    TouchPropertyGridCtrl(pThis, FALSE);
}
CMFCPropertyGridCtrl::CMFCPropertyGridCtrl() {
    memset(_propgridctrl_padding, 0, sizeof(_propgridctrl_padding));
    EnsurePropertyGridCtrlState(this);
}
CMFCPropertyGridCtrl::~CMFCPropertyGridCtrl() {
    PropertyGridCtrlState* state = FindMutablePropertyGridCtrlState(this);
    if (state) {
        std::vector<CMFCPropertyGridProperty*> properties = state->properties;
        state->properties.clear();
        state->current = nullptr;
        for (CMFCPropertyGridProperty* prop : properties) {
            SetPropertyGridOwnerRecursive(prop, nullptr);
            delete prop;
        }
    }
    g_propertyGridCtrlStates.erase(this);
}
BOOL CMFCPropertyGridCtrl::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID) {
    EnsurePropertyGridCtrlState(this);
    return CWnd::CreateEx(0, L"SysListView32", L"", dwStyle, rect.left, rect.top,
                          rect.right - rect.left, rect.bottom - rect.top,
                          pParentWnd ? pParentWnd->GetSafeHwnd() : nullptr,
                          reinterpret_cast<HMENU>(static_cast<UINT_PTR>(nID)), this);
}
int CMFCPropertyGridCtrl::AddProperty(CMFCPropertyGridProperty* pProp, int nPos, int bRedraw) {
    if (!pProp) return -1;

    auto& properties = EnsurePropertyGridCtrlState(this).properties;
    properties.erase(std::remove(properties.begin(), properties.end(), pProp), properties.end());
    DetachPropertyFromParent(pProp);
    DetachPropertyFromGrid(pProp);
    PropertyGridPropertyState& propState = EnsurePropertyGridPropertyState(pProp);
    propState.parent = nullptr;
    SetPropertyGridOwnerRecursive(pProp, this);

    if (nPos < 0 || nPos > static_cast<int>(properties.size())) {
        properties.push_back(pProp);
        if (bRedraw) {
            TouchPropertyGridCtrl(this, TRUE);
        }
        return static_cast<int>(properties.size() - 1);
    }

    auto it = properties.begin() + nPos;
    properties.insert(it, pProp);
    if (bRedraw) {
        TouchPropertyGridCtrl(this, TRUE);
    }
    return nPos;
}
int CMFCPropertyGridCtrl::GetPropertyCount() const {
    const PropertyGridCtrlState* state = FindPropertyGridCtrlState(this);
    return state ? static_cast<int>(state->properties.size()) : 0;
}
CMFCPropertyGridProperty* CMFCPropertyGridCtrl::GetProperty(int nIndex) const {
    const PropertyGridCtrlState* state = FindPropertyGridCtrlState(this);
    if (!state || nIndex < 0 || nIndex >= static_cast<int>(state->properties.size())) return nullptr;
    return state->properties[static_cast<size_t>(nIndex)];
}
BOOL CMFCPropertyGridCtrl::DeleteProperty(CMFCPropertyGridProperty*& pProp, BOOL bRedraw, BOOL bAdjustLayout) {
    if (!pProp) return FALSE;
    PropertyGridCtrlState& state = EnsurePropertyGridCtrlState(this);
    auto top = std::find(state.properties.begin(), state.properties.end(), pProp);
    if (top != state.properties.end()) {
        CMFCPropertyGridProperty* removed = *top;
        state.properties.erase(top);
        if (state.current == removed || IsPropertyGridAncestorOf(removed, state.current)) {
            state.current = nullptr;
        }
        SetPropertyGridOwnerRecursive(removed, nullptr);
        delete removed;
        pProp = nullptr;
        if (bRedraw) {
            TouchPropertyGridCtrl(this, bAdjustLayout);
        }
        return TRUE;
    }

    PropertyGridPropertyState* propState = FindMutablePropertyGridPropertyState(pProp);
    if (propState && propState->parent) {
        if (propState->owner != this) return FALSE;
        CMFCPropertyGridProperty* parent = propState->parent;
        CMFCPropertyGridProperty* removed = pProp;
        if (state.current == removed || IsPropertyGridAncestorOf(removed, state.current)) {
            state.current = nullptr;
        }
        if (parent->RemoveSubItem(pProp, TRUE)) {
            if (bRedraw) {
                TouchPropertyGridCtrl(this, bAdjustLayout);
            }
            return TRUE;
        }
    }
    return FALSE;
}
BOOL CMFCPropertyGridCtrl::DeleteProperty(void*& pProp, BOOL bRedraw, BOOL bAdjustLayout) {
    CMFCPropertyGridProperty* typed = static_cast<CMFCPropertyGridProperty*>(pProp);
    BOOL result = DeleteProperty(typed, bRedraw, bAdjustLayout);
    pProp = typed;
    return result;
}
CMFCPropertyGridProperty* CMFCPropertyGridCtrl::FindItemByData(DWORD_PTR dwData, BOOL bSearchSubItems) const {
    const PropertyGridCtrlState* state = FindPropertyGridCtrlState(this);
    if (!state) return nullptr;
    for (CMFCPropertyGridProperty* prop : state->properties) {
        if (!prop) continue;
        if (prop->GetData() == dwData) return prop;
        if (bSearchSubItems) {
            if (CMFCPropertyGridProperty* found = prop->FindSubItemByData(dwData)) {
                return found;
            }
        }
    }
    return nullptr;
}
void CMFCPropertyGridCtrl::RemoveAll() {
    auto& state = EnsurePropertyGridCtrlState(this);
    std::vector<CMFCPropertyGridProperty*> properties = state.properties;
    state.properties.clear();
    state.current = nullptr;
    for (CMFCPropertyGridProperty* pProp : properties) {
        SetPropertyGridOwnerRecursive(pProp, nullptr);
        delete pProp;
    }
    TouchPropertyGridCtrl(this, TRUE);
}
void CMFCPropertyGridCtrl::ExpandAll(BOOL bExpand) {
    const PropertyGridCtrlState* state = FindPropertyGridCtrlState(this);
    if (!state) return;
    for (CMFCPropertyGridProperty* pProp : state->properties) {
        ExpandPropertyRecursive(pProp, bExpand);
    }
    TouchPropertyGridCtrl(this, TRUE);
}
void CMFCPropertyGridCtrl::AdjustLayout() {
    ++EnsurePropertyGridCtrlState(this).layoutRevision;
    Invalidate(FALSE);
}
void CMFCPropertyGridCtrl::SetDescriptionRows(int nRows) {
    EnsurePropertyGridCtrlState(this).descriptionRows = (nRows < 0) ? 0 : nRows;
    TouchPropertyGridCtrl(this, TRUE);
}
void CMFCPropertyGridCtrl::SetCurSel(CMFCPropertyGridProperty* pProp, BOOL bRedraw) {
    auto& state = EnsurePropertyGridCtrlState(this);
    if (pProp) {
        const PropertyGridPropertyState* propState = FindPropertyGridPropertyState(pProp);
        if (!propState || propState->owner != this) return;
    }
    state.current = pProp;
    if (bRedraw) {
        TouchPropertyGridCtrl(this, FALSE);
    }
}
void CMFCPropertyGridCtrl::ResetOriginalValues(BOOL bRedraw) {
    const PropertyGridCtrlState* state = FindPropertyGridCtrlState(this);
    if (!state) return;
    for (CMFCPropertyGridProperty* prop : state->properties) {
        ResetPropertyGridOriginalValueRecursive(prop);
    }
    if (bRedraw) {
        TouchPropertyGridCtrl(this, FALSE);
    }
}
void CMFCPropertyGridCtrl::MarkModifiedProperties(BOOL bModified, BOOL bRedraw) {
    const PropertyGridCtrlState* state = FindPropertyGridCtrlState(this);
    if (!state) return;
    for (CMFCPropertyGridProperty* prop : state->properties) {
        MarkPropertyGridModifiedRecursive(prop, bModified);
    }
    if (bRedraw) {
        TouchPropertyGridCtrl(this, FALSE);
    }
}
void CMFCPropertyGridCtrl::SetBoolLabels(const wchar_t* lpszTrue, const wchar_t* lpszFalse) {
    auto& state = EnsurePropertyGridCtrlState(this);
    state.boolTrue = lpszTrue ? lpszTrue : L"";
    state.boolFalse = lpszFalse ? lpszFalse : L"";
    TouchPropertyGridCtrl(this, FALSE);
}
void CMFCPropertyGridCtrl::SetListDelimiter(wchar_t c) {
    EnsurePropertyGridCtrlState(this).listDelimiter = c;
    TouchPropertyGridCtrl(this, FALSE);
}
void CMFCPropertyGridCtrl::SetAlphabeticMode(BOOL bSet) {
    auto& state = EnsurePropertyGridCtrlState(this);
    state.alphabeticMode = bSet ? TRUE : FALSE;
    if (state.alphabeticMode) {
        auto compareProps = [this](const CMFCPropertyGridProperty* lhs, const CMFCPropertyGridProperty* rhs) {
            return CompareProps(lhs, rhs) < 0;
        };
        std::stable_sort(state.properties.begin(), state.properties.end(),
            compareProps);
        for (CMFCPropertyGridProperty* prop : state.properties) {
            SortPropertyGridSubItemsRecursive(prop, compareProps);
        }
    }
    TouchPropertyGridCtrl(this, TRUE);
}
void CMFCPropertyGridCtrl::SetGroupNameFullWidth(BOOL bSet, BOOL bRedraw) {
    EnsurePropertyGridCtrlState(this).groupNameFullWidth = bSet ? TRUE : FALSE;
    if (bRedraw) {
        TouchPropertyGridCtrl(this, TRUE);
    }
}
int CMFCPropertyGridCtrl::GetTotalItems(BOOL bIncludeHidden) const {
    const PropertyGridCtrlState* state = FindPropertyGridCtrlState(this);
    if (!state) return 0;
    int count = 0;
    for (CMFCPropertyGridProperty* prop : state->properties) {
        count += CountPropertyGridItems(prop, bIncludeHidden);
    }
    return count;
}
int CMFCPropertyGridCtrl::CompareProps(const CMFCPropertyGridProperty* pProp1, const CMFCPropertyGridProperty* pProp2) const {
    if (pProp1 == pProp2) return 0;
    if (!pProp1) return -1;
    if (!pProp2) return 1;
    return pProp1->GetName().Compare(pProp2->GetName());
}
