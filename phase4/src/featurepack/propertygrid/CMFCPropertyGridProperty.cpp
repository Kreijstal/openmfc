// CMFCPropertyGridProperty — OpenMFC implementation.
// Sources: ctrlcore.cpp, mfccore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/CtrlcoreSupport.h"
#include "detail/MfccoreSupport.h"


// Implementations this unit calls that are defined with their own class.
extern "C" int MS_ABI impl__OnRotateListValue_CMFCPropertyGridProperty__MEAAHH_Z(CMFCPropertyGridProperty* pThis, int p0);
// ?IsSubItem@CMFCPropertyGridProperty@@IEBAHPEAV1@@Z -- defined in propertygrid/Thunks.cpp.
extern "C" int MS_ABI impl__IsSubItem_CMFCPropertyGridProperty__IEBAHPEAV1__Z(const CMFCPropertyGridProperty* pThis, void* pProp);
// ?AfxThrowMemoryException@@YAXXZ
extern "C" void MS_ABI impl__AfxThrowMemoryException__YAXXZ();

namespace {
// Local transcriptions of the retail COleVariant assignment operators that
// CMFCPropertyGridProperty::TextToVar calls.  They are transcribed here rather
// than routed through the in-tree impl___4COleVariant__* thunks because those
// thunks (detail/OlecoreSupport.cpp) always reset the VARIANT to the new type,
// while retail preserves VT_BOOL (and VT_ERROR for the long overload) -- and
// TextToVar's VT_BOOL branch depends on exactly that.

// ??4COleVariant@@QEAAAEBV0@F@Z, RVA 0x26de90 (mfc140u): VT_I2 -> store iVal;
// VT_BOOL -> store (n ? VARIANT_TRUE : VARIANT_FALSE) keeping VT_BOOL
// (neg/sbb); otherwise VariantClear (OLEAUT32 #9), vt = VT_I2, store iVal.
inline void PgVarAssignShort(VARIANT& v, short n) {
    if (v.vt != VT_I2) {
        if (v.vt == VT_BOOL) {
            v.boolVal = n ? VARIANT_TRUE : VARIANT_FALSE;
            return;
        }
        ::VariantClear(&v);
        v.vt = VT_I2;
    }
    v.iVal = n;
}
// ??4COleVariant@@QEAAAEBV0@J@Z, RVA 0x26dee0 (mfc140u): VT_I4 or VT_ERROR ->
// store the 32-bit value in place; VT_BOOL -> store (n ? VARIANT_TRUE :
// VARIANT_FALSE); otherwise VariantClear, vt = VT_I4, store lVal.
inline void PgVarAssignLong(VARIANT& v, long n) {
    if (v.vt == VT_I4 || v.vt == VT_ERROR) {
        v.lVal = n;
        return;
    }
    if (v.vt == VT_BOOL) {
        v.boolVal = n ? VARIANT_TRUE : VARIANT_FALSE;
        return;
    }
    ::VariantClear(&v);
    v.vt = VT_I4;
    v.lVal = n;
}
// ??4COleVariant@@QEAAAEBV0@M@Z, RVA 0x26e010 (mfc140u).
inline void PgVarAssignFloat(VARIANT& v, float f) {
    if (v.vt != VT_R4) {
        ::VariantClear(&v);
        v.vt = VT_R4;
    }
    v.fltVal = f;
}
// ??4COleVariant@@QEAAAEBV0@N@Z, RVA 0x26e050 (mfc140u).
inline void PgVarAssignDouble(VARIANT& v, double d) {
    if (v.vt != VT_R8) {
        ::VariantClear(&v);
        v.vt = VT_R8;
    }
    v.dblVal = d;
}
// ??4COleVariant@@QEAAAEBV0@E@Z, RVA 0x26de50 (mfc140u).
inline void PgVarAssignByte(VARIANT& v, BYTE b) {
    if (v.vt != VT_UI1) {
        ::VariantClear(&v);
        v.vt = VT_UI1;
    }
    v.bVal = b;
}
// ??4COleVariant@@QEAAAEBV0@QEB_W@Z, RVA 0x26dcd0 (mfc140u), non-NULL source
// path: VariantClear, vt = VT_BSTR, then it builds a temporary CString from the
// LPCWSTR (so the length is the NUL-terminated length) and stores
// SysAllocStringLen (OLEAUT32 #4) of it.  When that returns NULL, the call at
// 0x26ddf6 goes to CSimpleStringT::ThrowMemoryException (RVA 0x3160 mfc140u),
// which AtlThrows E_OUTOFMEMORY; the AtlThrow helper at 0x333c maps that
// HRESULT to AfxThrowMemoryException (RVA 0x2276c0 mfc140u).
inline void PgVarAssignString(VARIANT& v, const wchar_t* psz) {
    ::VariantClear(&v);
    v.vt = VT_BSTR;
    BSTR bstr = ::SysAllocStringLen(psz, static_cast<UINT>(wcslen(psz)));
    if (bstr == nullptr) impl__AfxThrowMemoryException__YAXXZ();
    v.bstrVal = bstr;
}

// Retail CStringT::TrimLeft (RVA 0x127f0 mfc140u) skips leading characters for
// which iswspace (CRT import) is non-zero; TrimRight (RVA 0x12750 mfc140u) scans
// to the terminating NUL and cuts at the start of the final run of iswspace
// characters.  OpenMFC's inline CString::TrimLeft/TrimRight only strip
// space/tab/CR/LF, so TextToVar uses these on the NUL-terminated text instead.
// Every consumer in TextToVar (IsEmpty test, swscanf, wcscmp) stops at the first
// NUL, so working on the NUL-terminated prefix gives the same results.
inline std::wstring PgTrimRight(const wchar_t* p) {
    const wchar_t* end = p;
    const wchar_t* runStart = nullptr;
    for (; *end; ++end) {
        if (iswspace(*end)) {
            if (!runStart) runStart = end;
        } else {
            runStart = nullptr;
        }
    }
    return std::wstring(p, runStart ? runStart : end);
}
inline std::wstring PgTrimBoth(const wchar_t* p) {
    while (*p && iswspace(*p)) ++p;
    return PgTrimRight(p);
}
} // namespace

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
//
// Retail layout this block refers to (read from the disassembly cited in each
// function; the OpenMFC CMFCPropertyGridProperty in include/openmfc/afxmfc.h
// does NOT have these members -- it keeps tree/owner data in the
// PropertyGridPropertyState side table from detail/MfccoreSupport.h):
//   +0x08 m_varValue (COleVariant, vt at +0x08, payload at +0x10)
//   +0x40 m_dwFlags        +0x44 m_Rect          +0x54 m_rectButton
//   +0x70 m_bGroup         +0x100 m_pWndInPlace  +0x108 m_pWndCombo
//   +0x110 m_pWndSpin      +0x118 m_pWndList     +0x120 m_pParent
//   +0x128 m_lstSubItems (CList; its head pointer at +0x130)

// Symbol: ?AddSubItem@CMFCPropertyGridProperty@@QEAAHPEAV1@@Z
// Transcribed from retail RVA 0xc11b0 (mfc140u):
//   if (!m_bGroup /*+0x70*/) return FALSE;
//   if (pProp->m_pWndList /*+0x118 of pProp, not this*/ != NULL)
//       for each pListProp in pProp->m_pWndList->m_lstProps (head at +0x610):
//           if (pListProp == pProp || pListProp->IsSubItem(pProp)) return FALSE;
//   pProp->m_pParent = this;            // +0x120
//   m_lstSubItems.AddTail(pProp);       // +0x128
//   pProp->m_pWndList = m_pWndList;     // +0x118, the direct child only
//   return TRUE;
// IsSubItem is the direct call to RVA 0xc1db0 (mfc140u); here it goes through
// the sibling thunk.  The parent/list/owner stores land in the side table.
// Retail does no redraw here, so neither does this body.
// Deviations: (1) the m_bGroup test is NOT performed -- OpenMFC's layout has no
// group flag, so a non-group property also accepts sub-items where retail
// returns FALSE (see headerRequests); (2) NULL pThis/pProp return FALSE where
// retail would fault.
extern "C" int MS_ABI impl__AddSubItem_CMFCPropertyGridProperty__QEAAHPEAV1__Z(
    CMFCPropertyGridProperty* pThis, CMFCPropertyGridProperty* pProp) {
    if (!pThis || !pProp) return FALSE;

    PropertyGridPropertyState& childState = EnsurePropertyGridPropertyState(pProp);
    if (childState.owner) {
        const PropertyGridCtrlState* ctrlState = FindPropertyGridCtrlState(childState.owner);
        if (ctrlState) {
            for (CMFCPropertyGridProperty* pListProp : ctrlState->properties) {
                if (pListProp == pProp ||
                    impl__IsSubItem_CMFCPropertyGridProperty__IEBAHPEAV1__Z(pListProp, pProp)) {
                    return FALSE;
                }
            }
        }
    }

    // std::unordered_map references stay valid across inserts, so childState
    // is still good after this Ensure.
    PropertyGridPropertyState& parentState = EnsurePropertyGridPropertyState(pThis);
    childState.parent = pThis;
    parentState.subItems.push_back(pProp);
    childState.owner = parentState.owner;
    return TRUE;
}

// Symbol: ?CreateCombo@CMFCPropertyGridProperty@@MEAAPEAVCComboBox@@PEAVCWnd@@VCRect@@@Z
// STUB. Retail RVA 0xc3810 (mfc140u): sets rect.bottom = rect.top + 400,
// operator new(0xe8) + CWnd ctor (0x28a700) + CComboBox vftable, then
// CComboBox::Create(0x40200403, rect, pWndParent, 3) (devirtualised against
// RVA 0x294150 when the vtable slot 0x2d8/8 = 91 matches); on failure it
// destroys the object through vtable slot 1 and returns NULL, else returns it.
// It touches no member of this.  Not implemented: the object retail hands
// back is an MSVC-laid-out CComboBox that its caller keeps in m_pWndCombo
// (+0x108, absent from OpenMFC's layout) and later drives through vtable
// slots 26 and 1 (see OnDestroyWindow below); an OpenMFC CComboBox is a
// mingw-built header class whose vtable order has not been verified against
// that, and its inline Create calls CWnd::Create, which is not a linkable
// symbol inside this DLL.  Left returning NULL.
// CRect by value is passed as a pointer to the caller's copy under the x64 ABI.
extern "C" void* MS_ABI impl__CreateCombo_CMFCPropertyGridProperty__MEAAPEAVCComboBox__PEAVCWnd__VCRect___Z(
    CMFCPropertyGridProperty* pThis, CWnd* pWndParent, const RECT* pRect) {
    (void)pThis; (void)pWndParent; (void)pRect;
    return nullptr;
}

// Symbol: ?CreateInPlaceEdit@CMFCPropertyGridProperty@@UEAAPEAVCWnd@@VCRect@@AEAH@Z
// STUB. Retail RVA 0xc3590 (mfc140u): returns NULL for value types outside
// its editable set unless the flag at +0x80 is set; otherwise allocates a
// plain CEdit (0xe8 bytes) when the edit mask/template/valid-chars strings
// (+0xb0/+0xb8/+0xc0) are all empty, else a CMFCMaskedEdit (0x128 bytes,
// ctor 0x78d70) configured from them, and creates it via CEdit::Create
// (0x294320) with parent m_pWndList (+0x118) and ID 3.  OpenMFC keeps
// neither m_pWndList nor the mask strings in the property (the constructor
// discards lpszEditMask/lpszEditTemplate/lpszValidChars); left returning NULL.
extern "C" void* MS_ABI impl__CreateInPlaceEdit_CMFCPropertyGridProperty__UEAAPEAVCWnd__VCRect__AEAH_Z(
    CMFCPropertyGridProperty* pThis, const RECT* pRectEdit, int* pbDefaultFormat) {
    (void)pThis; (void)pRectEdit; (void)pbDefaultFormat;
    return nullptr;
}

// Symbol: ?CreateSpinControl@CMFCPropertyGridProperty@@UEAAPEAVCSpinButtonCtrl@@VCRect@@@Z
// STUB. Retail RVA 0xc3700 (mfc140u): operator new(0x100) + CMFCSpinButtonCtrl
// ctor (0x132520), then vtable slot 0x2d8/8 = 91 called with (0x500000a2,
// rect, m_pWndList (+0x118), 3) -- the Create call -- returning NULL if that
// fails.  Then SendMessage(UDM_SETBUDDY 0x469, m_pWndInPlace (+0x100) HWND or
// NULL), and SendMessage(UDM_SETRANGE32 0x46f, min +0x94, max +0x98) unless
// both are 0.  Not implemented: the parent (m_pWndList) and the buddy
// (m_pWndInPlace) are members OpenMFC's layout does not have, and the caller
// stores the result in m_pWndSpin (+0x110), which it does not have either.
extern "C" void* MS_ABI impl__CreateSpinControl_CMFCPropertyGridProperty__UEAAPEAVCSpinButtonCtrl__VCRect___Z(
    CMFCPropertyGridProperty* pThis, const RECT* pRectSpin) {
    (void)pThis; (void)pRectSpin;
    return nullptr;
}

// Symbol: ?OnCtlColor@CMFCPropertyGridProperty@@MEAAPEAUHBRUSH__@@PEAVCDC@@I@Z
// STUB. Retail RVA 0xc42a0 (mfc140u) reads the grid's custom colours at
// m_pWndList +0x688 and +0x684 (each skipped when -1) and the CBrush at
// +0x6a0, passing the colours to pDC through its vtable slots 0x70/8 and
// 0x68/8 and returning that brush's handle when it has one.  Otherwise, for
// the numeric/BSTR/BOOL value types, it returns NULL when the ints at +0x78
// and +0x7c are both non-zero, else passes a colour from the lazily
// initialised global data block to pDC and returns a global brush; other
// types return NULL.  OpenMFC's PropertyGridCtrlState has none of those
// colours or the brush, and the property has no m_pWndList; left NULL.
extern "C" HBRUSH MS_ABI impl__OnCtlColor_CMFCPropertyGridProperty__MEAAPEAUHBRUSH____PEAVCDC__I_Z(
    CMFCPropertyGridProperty* pThis, CDC* pDC, unsigned int nCtlColor) {
    (void)pThis; (void)pDC; (void)nCtlColor;
    return nullptr;
}

// Symbol: ?OnDestroyWindow@CMFCPropertyGridProperty@@MEAAXXZ
// STUB. Retail RVA 0xc10a0 (mfc140u): for each of m_pWndCombo (+0x108),
// m_pWndInPlace (+0x100) and m_pWndSpin (+0x110), in that order: if non-NULL,
// call its vtable slot 0xd0/8 = 26 (DestroyWindow), then, if still non-NULL,
// slot 1 with 1 (deleting destructor), then zero the member.  Finally, if
// m_varValue.vt == VT_BOOL, m_lstOptions.RemoveAll().
// Not implemented: the three window members do not exist in OpenMFC's layout.
// Doing only the final VT_BOOL option clear was considered and rejected:
// nothing in this tree adds True/False options to a bool property
// automatically, so the only options such a property has here are ones the
// application added with AddOption (which OnRotateListValue then reads), and
// clearing them would discard those.
extern "C" void MS_ABI impl__OnDestroyWindow_CMFCPropertyGridProperty__MEAAXXZ(
    CMFCPropertyGridProperty* pThis) {
    (void)pThis;
}

// Symbol: ?Reposition@CMFCPropertyGridProperty@@IEAAXAEAH@Z
// STUB. Retail RVA 0xc1a80 (mfc140u; not in the RVA symbol map, resolved by
// export ordinal).  When the property is shown (+0x90 and IsParentExpanded,
// RVA 0xc1980, or the grid's alphabetic flag at m_pWndList +0x554) it
// recomputes m_Rect (+0x44) from the grid's layout fields (m_pWndList +0x590,
// +0x598, +0x5bc row height, hierarchy level via the m_pParent +0x120 chain),
// moves a non-empty m_rectButton (+0x54, IsRectEmpty) to the new row, advances
// the caller's y by the row height, and, if the grid's tooltip control
// (+0xe8) has a window (IsWindow), registers one or two tools on it via
// CToolTipCtrl::AddTool (RVA 0x275060 mfc140u), counting in m_pWndList +0x5a8.
// Otherwise it SetRectEmpty's both rects.  It then recurses into every entry of
// m_lstSubItems (+0x128) and finally calls this's vtable slot 0xe8/8 = 29
// (OnPosSizeChanged in the MFC source) with the old m_Rect.  OpenMFC has neither the
// per-property rects nor the grid layout fields; left a no-op.
extern "C" void MS_ABI impl__Reposition_CMFCPropertyGridProperty__IEAAXAEAH_Z(
    CMFCPropertyGridProperty* pThis, int* pY) {
    (void)pThis; (void)pY;
}

// Symbol: ?SetFlags@CMFCPropertyGridProperty@@IEAAXXZ
// STUB. Retail RVA 0xc0e60 (mfc140u) is exactly
//   m_dwFlags /*+0x40*/ = (m_varValue.vt == VT_BOOL) ? AFX_PROP_HAS_LIST /*1*/ : 0;
// OpenMFC's CMFCPropertyGridProperty has no m_dwFlags member (HasButton is
// derived from the option list instead), so there is nothing to store into;
// see headerRequests.
extern "C" void MS_ABI impl__SetFlags_CMFCPropertyGridProperty__IEAAXXZ(
    CMFCPropertyGridProperty* pThis) {
    (void)pThis;
}

// Symbol: ?TextToVar@CMFCPropertyGridProperty@@MEAAHAEBV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@@Z
// Transcribed from retail RVA 0xc2d70 (mfc140u; not in the RVA symbol map,
// resolved through the mfc140u export table by ordinal).  It copies strText
// (CloneData) and switches on m_varValue.vt, assigning in place (no SetValue,
// no redraw):
//   VT_BSTR            m_varValue = (LPCTSTR)str
//   VT_R8              d = 0; str.TrimLeft(); str.TrimRight();
//                      if (!str.IsEmpty()) swscanf_s(str, L"%lf", &d); m_varValue = d
//   VT_R4              same with float and L"%f"
//   VT_I2              m_varValue = (short)_wtoi(str)
//   VT_BOOL            str.TrimRight();
//                      m_varValue = (short)(wcscmp(str, m_pWndList->m_strTrue) == 0)
//                      (operator=(short) keeps VT_BOOL and stores 0 / -1)
//   VT_UI1             m_varValue = str.IsEmpty() ? 0 : low byte of str[0]
//   VT_UI2             m_varValue.uiVal = (USHORT)_wtoi(str)          (vt unchanged)
//   VT_UI4, VT_UINT    m_varValue.ulVal = wcstoul(strText, NULL, 10)  (original text)
//   VT_I4, VT_INT      m_varValue = _wtol(str)
//   anything else      return FALSE
// then TRUE.  IAT slots resolved with iatu.py: _wtoi, _wtol, wcstoul, wcscmp;
// the scanf helper at 0xcc448 (mfc140u) forwards to __stdio_common_vswscanf
// with the secure-CRT option bit, i.e. swscanf_s.
// m_pWndList->m_strTrue is the CString at m_pWndList +0x560 (retail AtlThrows
// E_FAIL if its buffer pointer were NULL, which a live CString never is).
// The trims use the iswspace-based PgTrimBoth/PgTrimRight helpers above, which
// match retail's TrimLeft (0x127f0) and TrimRight (0x12750); OpenMFC's inline
// CString trims strip only space/tab/CR/LF.
// Deviations: (1) m_strTrue comes from the owning grid's PropertyGridCtrlState
// boolTrue (set by SetBoolLabels, default L"True"); with no owner the default
// L"True" is used, where retail would fault dereferencing a NULL m_pWndList;
// (2) swscanf is used in place of swscanf_s -- identical for %f / %lf, which
// take no buffer-size argument; (3) NULL pThis/strText return FALSE where
// retail would fault.
extern "C" int MS_ABI impl__TextToVar_CMFCPropertyGridProperty__MEAAHAEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
    CMFCPropertyGridProperty* pThis, const CString* pstrText) {
    if (!pThis || !pstrText) return FALSE;

    CString str = *pstrText;
    const wchar_t* psz = static_cast<const wchar_t*>(str);
    // m_varValue itself (retail assigns the member in place).
    VARIANT& var = const_cast<COleVariant&>(pThis->GetValue());

    switch (var.vt) {
    case VT_BSTR:
        PgVarAssignString(var, psz);
        break;
    case VT_R8: {
        double d = 0.0;
        const std::wstring trimmed = PgTrimBoth(psz);
        if (!trimmed.empty()) {
            swscanf(trimmed.c_str(), L"%lf", &d);
        }
        PgVarAssignDouble(var, d);
        break;
    }
    case VT_R4: {
        float f = 0.0f;
        const std::wstring trimmed = PgTrimBoth(psz);
        if (!trimmed.empty()) {
            swscanf(trimmed.c_str(), L"%f", &f);
        }
        PgVarAssignFloat(var, f);
        break;
    }
    case VT_I2:
        PgVarAssignShort(var, static_cast<short>(_wtoi(str)));
        break;
    case VT_BOOL: {
        const std::wstring trimmed = PgTrimRight(psz);
        const PropertyGridPropertyState* state = FindPropertyGridPropertyState(pThis);
        const PropertyGridCtrlState* ctrlState =
            (state && state->owner) ? FindPropertyGridCtrlState(state->owner) : nullptr;
        const wchar_t* strTrue = ctrlState ? ctrlState->boolTrue.c_str() : L"True";
        PgVarAssignShort(var, static_cast<short>(wcscmp(trimmed.c_str(), strTrue) == 0));
        break;
    }
    case VT_UI1:
        PgVarAssignByte(var, str.IsEmpty() ? static_cast<BYTE>(0) : static_cast<BYTE>(str[0]));
        break;
    case VT_UI2:
        var.uiVal = static_cast<USHORT>(_wtoi(str));
        break;
    case VT_UI4:
    case VT_UINT:
        var.ulVal = wcstoul(static_cast<const wchar_t*>(*pstrText), nullptr, 10);
        break;
    case VT_I4:
    case VT_INT:
        PgVarAssignLong(var, _wtol(str));
        break;
    default:
        return FALSE;
    }
    return TRUE;
}
