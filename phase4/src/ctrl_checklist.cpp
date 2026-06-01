// CCheckListBox implementation
//
// This file is intentionally isolated so checklist work can be owned and
// reviewed independently from the broader control-core implementation.

#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxwin.h"
#include "openmfc/afxmfc.h"
#include <windows.h>
#include <oleauto.h>
#include <oleacc.h>
#include <unordered_map>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

namespace {
using ItemStateMap = std::unordered_map<int, int>;
static std::unordered_map<const CCheckListBox*, ItemStateMap> g_checkStates;
static std::unordered_map<const CCheckListBox*, ItemStateMap> g_itemEnabledStates;
static std::unordered_map<const CCheckListBox*, unsigned int> g_checkStyles;

static int ListBoxItemFromPoint(HWND hWnd, CPoint pt) {
    if (!hWnd) return LB_ERR;
    DWORD hit = (DWORD)::SendMessageW(hWnd, LB_ITEMFROMPOINT, 0, MAKELPARAM(pt.x, pt.y));
    if (HIWORD(hit) != 0) return LB_ERR;
    return (int)LOWORD(hit);
}

static CRect CheckListBoxCheckRect(const RECT& itemRect) {
    int itemHeight = itemRect.bottom - itemRect.top;
    int box = ::GetSystemMetrics(SM_CXMENUCHECK);
    if (box <= 0) box = 13;
    if (itemHeight > 0 && box > itemHeight) box = itemHeight;

    CRect checkRect;
    checkRect.left = itemRect.left + 2;
    checkRect.top = itemRect.top + ((itemHeight - box) / 2);
    checkRect.right = checkRect.left + box;
    checkRect.bottom = checkRect.top + box;
    return checkRect;
}

static int CheckListBoxMinimumItemHeight(HWND hWnd) {
    int height = ::GetSystemMetrics(SM_CYMENUCHECK);
    if (height <= 0) height = 13;
    if (hWnd) {
        LRESULT itemHeight = ::SendMessageW(hWnd, LB_GETITEMHEIGHT, 0, 0);
        if (itemHeight > height && itemHeight < 0x10000) height = (int)itemHeight;
    }
    return height + 2;
}

static int CheckListBoxVariantIndex(const VARIANT& varChild) {
    if (varChild.vt == VT_I4 || varChild.vt == VT_INT) return (int)varChild.lVal;
    if (varChild.vt == VT_EMPTY) return (int)CHILDID_SELF;
    return -1;
}

static LRESULT CheckListBoxSend(CCheckListBox* pThis, UINT message, WPARAM wParam, LPARAM lParam) {
    return (pThis && pThis->m_hWnd) ? ::SendMessageW(pThis->m_hWnd, message, wParam, lParam) : LB_ERR;
}
}  // namespace

int CCheckListBox::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, unsigned int nID) {
    g_checkStates.erase(this);
    g_itemEnabledStates.erase(this);
    g_checkStyles.erase(this);
    return CWnd::Create(L"LISTBOX", L"", dwStyle, rect, pParentWnd, nID);
}

CCheckListBox::~CCheckListBox() {
    g_checkStates.erase(this);
    g_itemEnabledStates.erase(this);
    g_checkStyles.erase(this);
}

void CCheckListBox::SetCheck(int nIndex, int nCheck) {
    if (nIndex < 0) return;
    g_checkStates[this][nIndex] = (nCheck != 0) ? 1 : 0;
}

int CCheckListBox::GetCheck(int nIndex) {
    if (nIndex < 0) return 0;
    auto itBox = g_checkStates.find(this);
    if (itBox == g_checkStates.end()) return 0;
    auto itItem = itBox->second.find(nIndex);
    return (itItem == itBox->second.end()) ? 0 : itItem->second;
}

void CCheckListBox::Enable(int nIndex, int bEnabled) {
    if (nIndex < 0) return;
    g_itemEnabledStates[this][nIndex] = (bEnabled != 0) ? 1 : 0;
}

int CCheckListBox::IsEnabled(int nIndex) {
    if (nIndex < 0) return FALSE;
    auto itBox = g_itemEnabledStates.find(this);
    if (itBox == g_itemEnabledStates.end()) return TRUE;
    auto itItem = itBox->second.find(nIndex);
    return (itItem == itBox->second.end()) ? TRUE : itItem->second;
}

// Symbol: ?Create@CCheckListBox@@UEAAHKAEBUtagRECT@@PEAVCWnd@@I@Z
extern "C" int MS_ABI impl__Create_CCheckListBox__UEAAHKAEBUtagRECT__PEAVCWnd__I_Z(
    CCheckListBox* pThis, DWORD dwStyle, const RECT* pRect, CWnd* pParentWnd, UINT nID) {
    return (pThis && pRect) ? pThis->Create(dwStyle, *pRect, pParentWnd, nID) : FALSE;
}

// Symbol: ?SetCheck@CCheckListBox@@QEAAXHH@Z
extern "C" void MS_ABI impl__SetCheck_CCheckListBox__QEAAXHH_Z(CCheckListBox* pThis, int nIndex, int nCheck) {
    if (pThis) pThis->SetCheck(nIndex, nCheck);
}

// Symbol: ?GetCheck@CCheckListBox@@QEAAHH@Z
extern "C" int MS_ABI impl__GetCheck_CCheckListBox__QEAAHH_Z(CCheckListBox* pThis, int nIndex) {
    return pThis ? pThis->GetCheck(nIndex) : 0;
}

// Symbol: ?SetSelectionCheck@CCheckListBox@@IEAAXH@Z
extern "C" void MS_ABI impl__SetSelectionCheck_CCheckListBox__IEAAXH_Z(CCheckListBox* pThis, int nCheck) {
    if (!pThis || !pThis->m_hWnd) return;
    int nSel = (int)::SendMessageW(pThis->m_hWnd, LB_GETCURSEL, 0, 0);
    if (nSel != LB_ERR) {
        impl__SetCheck_CCheckListBox__QEAAXHH_Z(pThis, nSel, nCheck);
    }
}

// Symbol: ?Enable@CCheckListBox@@QEAAXHH@Z
extern "C" void MS_ABI impl__Enable_CCheckListBox__QEAAXHH_Z(CCheckListBox* pThis, int nIndex, int bEnabled) {
    if (pThis) pThis->Enable(nIndex, bEnabled);
}

// Symbol: ?IsEnabled@CCheckListBox@@QEAAHH@Z
extern "C" int MS_ABI impl__IsEnabled_CCheckListBox__QEAAHH_Z(CCheckListBox* pThis, int nIndex) {
    return pThis ? pThis->IsEnabled(nIndex) : FALSE;
}

// Symbol: ?accDoDefaultAction@CCheckListBox@@MEAAJUtagVARIANT@@@Z
extern "C" HRESULT MS_ABI impl__accDoDefaultAction_CCheckListBox__MEAAJUtagVARIANT___Z(
    CCheckListBox* pThis, VARIANT varChild) {
    if (!pThis) return E_POINTER;
    int nIndex = CheckListBoxVariantIndex(varChild);
    if (nIndex == CHILDID_SELF) nIndex = pThis->m_hWnd ? (int)::SendMessageW(pThis->m_hWnd, LB_GETCURSEL, 0, 0) : LB_ERR;
    if (nIndex == LB_ERR || nIndex < 0) return E_INVALIDARG;
    pThis->SetCheck(nIndex, pThis->GetCheck(nIndex) ? 0 : 1);
    RECT rc = {};
    if (pThis->m_hWnd && ::SendMessageW(pThis->m_hWnd, LB_GETITEMRECT, nIndex, (LPARAM)&rc) != LB_ERR) {
        ::InvalidateRect(pThis->m_hWnd, &rc, TRUE);
    }
    return S_OK;
}

// Symbol: ?CalcMinimumItemHeight@CCheckListBox@@IEAAHXZ
extern "C" int MS_ABI impl__CalcMinimumItemHeight_CCheckListBox__IEAAHXZ(CCheckListBox* pThis) {
    return CheckListBoxMinimumItemHeight(pThis ? pThis->m_hWnd : nullptr);
}

// Symbol: ?CheckFromPoint@CCheckListBox@@IEAAHVCPoint@@AEAH@Z
extern "C" int MS_ABI impl__CheckFromPoint_CCheckListBox__IEAAHVCPoint__AEAH_Z(
    CCheckListBox* pThis, CPoint pt, int* pbInCheck) {
    if (pbInCheck) *pbInCheck = FALSE;
    if (!pThis || !pThis->m_hWnd) return LB_ERR;
    int nIndex = ListBoxItemFromPoint(pThis->m_hWnd, pt);
    if (nIndex == LB_ERR) return LB_ERR;

    RECT rc = {};
    if (::SendMessageW(pThis->m_hWnd, LB_GETITEMRECT, nIndex, (LPARAM)&rc) == LB_ERR) return LB_ERR;
    CRect checkRect = CheckListBoxCheckRect(rc);
    if (pbInCheck) *pbInCheck = checkRect.PtInRect(pt) ? TRUE : FALSE;
    return nIndex;
}

// Symbol: ?DrawItem@CCheckListBox@@UEAAXPEAUtagDRAWITEMSTRUCT@@@Z
extern "C" void MS_ABI impl__DrawItem_CCheckListBox__UEAAXPEAUtagDRAWITEMSTRUCT___Z(
    CCheckListBox* pThis, DRAWITEMSTRUCT* pDraw) {
    if (!pDraw || !pDraw->hDC) return;
    HBRUSH brush = ::GetSysColorBrush((pDraw->itemState & ODS_SELECTED) ? COLOR_HIGHLIGHT : COLOR_WINDOW);
    ::FillRect(pDraw->hDC, &pDraw->rcItem, brush);

    CRect checkRect = CheckListBoxCheckRect(pDraw->rcItem);
    UINT state = DFCS_BUTTONCHECK;
    if (pThis && pThis->GetCheck((int)pDraw->itemID)) state |= DFCS_CHECKED;
    if (pThis && !pThis->IsEnabled((int)pDraw->itemID)) state |= DFCS_INACTIVE;
    ::DrawFrameControl(pDraw->hDC, checkRect, DFC_BUTTON, state);
}

// Symbol: ?get_accDefaultAction@CCheckListBox@@MEAAJUtagVARIANT@@PEAPEA_W@Z
extern "C" HRESULT MS_ABI impl__get_accDefaultAction_CCheckListBox__MEAAJUtagVARIANT__PEAPEA_W_Z(
    CCheckListBox* pThis, VARIANT varChild, WCHAR** ppszDefaultAction) {
    (void)pThis;
    int nIndex = CheckListBoxVariantIndex(varChild);
    if (!ppszDefaultAction) return E_POINTER;
    *ppszDefaultAction = nullptr;
    if (nIndex < 0) return E_INVALIDARG;
    *ppszDefaultAction = ::SysAllocString(L"Toggle");
    return *ppszDefaultAction ? S_OK : E_OUTOFMEMORY;
}

// Symbol: ?get_accRole@CCheckListBox@@MEAAJUtagVARIANT@@PEAU2@@Z
extern "C" HRESULT MS_ABI impl__get_accRole_CCheckListBox__MEAAJUtagVARIANT__PEAU2__Z(
    CCheckListBox* pThis, VARIANT varChild, VARIANT* pRole) {
    (void)pThis;
    int nIndex = CheckListBoxVariantIndex(varChild);
    if (!pRole) return E_POINTER;
    if (nIndex < 0) return E_INVALIDARG;
    ::VariantInit(pRole);
    pRole->vt = VT_I4;
    pRole->lVal = (nIndex == CHILDID_SELF) ? ROLE_SYSTEM_LIST : ROLE_SYSTEM_CHECKBUTTON;
    return S_OK;
}

// Symbol: ?get_accState@CCheckListBox@@MEAAJUtagVARIANT@@PEAU2@@Z
extern "C" HRESULT MS_ABI impl__get_accState_CCheckListBox__MEAAJUtagVARIANT__PEAU2__Z(
    CCheckListBox* pThis, VARIANT varChild, VARIANT* pState) {
    if (!pState) return E_POINTER;
    if (!pThis) return E_POINTER;
    int nIndex = CheckListBoxVariantIndex(varChild);
    if (nIndex < 0) return E_INVALIDARG;
    ::VariantInit(pState);
    pState->vt = VT_I4;
    pState->lVal = 0;
    if (nIndex != CHILDID_SELF) {
        if (pThis->GetCheck(nIndex)) pState->lVal |= STATE_SYSTEM_CHECKED;
        if (!pThis->IsEnabled(nIndex)) pState->lVal |= STATE_SYSTEM_UNAVAILABLE;
    }
    return S_OK;
}

// Symbol: ?GetMessageMap@CCheckListBox@@MEBAPEBUAFX_MSGMAP@@XZ
extern "C" const AFX_MSGMAP* MS_ABI impl__GetMessageMap_CCheckListBox__MEBAPEBUAFX_MSGMAP__XZ(
    const CCheckListBox* pThis) {
    (void)pThis;
    return nullptr;
}

// Symbol: ?GetRuntimeClass@CCheckListBox@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CCheckListBox__UEBAPEAUCRuntimeClass__XZ(
    const CCheckListBox* pThis) {
    (void)pThis;
    return CCheckListBox::GetThisClass();
}

// Symbol: ?GetThisClass@CCheckListBox@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CCheckListBox__SAPEAUCRuntimeClass__XZ() {
    return CCheckListBox::GetThisClass();
}

// Symbol: ?GetThisMessageMap@CCheckListBox@@KAPEBUAFX_MSGMAP@@XZ
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CCheckListBox__KAPEBUAFX_MSGMAP__XZ() {
    return nullptr;
}

// Symbol: ?InvalidateCheck@CCheckListBox@@IEAAXH@Z
extern "C" void MS_ABI impl__InvalidateCheck_CCheckListBox__IEAAXH_Z(CCheckListBox* pThis, int nIndex) {
    if (!pThis || !pThis->m_hWnd || nIndex < 0) return;
    RECT rc = {};
    if (::SendMessageW(pThis->m_hWnd, LB_GETITEMRECT, nIndex, (LPARAM)&rc) == LB_ERR) return;
    CRect checkRect = CheckListBoxCheckRect(rc);
    ::InvalidateRect(pThis->m_hWnd, checkRect, TRUE);
}

// Symbol: ?InvalidateItem@CCheckListBox@@IEAAXH@Z
extern "C" void MS_ABI impl__InvalidateItem_CCheckListBox__IEAAXH_Z(CCheckListBox* pThis, int nIndex) {
    if (!pThis || !pThis->m_hWnd || nIndex < 0) return;
    RECT rc = {};
    if (::SendMessageW(pThis->m_hWnd, LB_GETITEMRECT, nIndex, (LPARAM)&rc) != LB_ERR) {
        ::InvalidateRect(pThis->m_hWnd, &rc, TRUE);
    }
}

// Symbol: ?MeasureItem@CCheckListBox@@UEAAXPEAUtagMEASUREITEMSTRUCT@@@Z
extern "C" void MS_ABI impl__MeasureItem_CCheckListBox__UEAAXPEAUtagMEASUREITEMSTRUCT___Z(
    CCheckListBox* pThis, MEASUREITEMSTRUCT* pMeasure) {
    if (!pMeasure) return;
    pMeasure->itemHeight = (UINT)CheckListBoxMinimumItemHeight(pThis ? pThis->m_hWnd : nullptr);
}

// Symbol: ?OnChildNotify@CCheckListBox@@MEAAHI_K_JPEA_J@Z
extern "C" int MS_ABI impl__OnChildNotify_CCheckListBox__MEAAHI_K_JPEA_J_Z(
    CCheckListBox* pThis, UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult) {
    if (pResult) *pResult = 0;
    if (!pThis) return FALSE;
    if (message == WM_DRAWITEM && lParam) {
        impl__DrawItem_CCheckListBox__UEAAXPEAUtagDRAWITEMSTRUCT___Z(pThis, (DRAWITEMSTRUCT*)lParam);
        if (pResult) *pResult = TRUE;
        return TRUE;
    }
    if (message == WM_MEASUREITEM && lParam) {
        impl__MeasureItem_CCheckListBox__UEAAXPEAUtagMEASUREITEMSTRUCT___Z(pThis, (MEASUREITEMSTRUCT*)lParam);
        if (pResult) *pResult = TRUE;
        return TRUE;
    }
    return FALSE;
}

// Symbol: ?OnCreate@CCheckListBox@@IEAAHPEAUtagCREATESTRUCTW@@@Z
extern "C" int MS_ABI impl__OnCreate_CCheckListBox__IEAAHPEAUtagCREATESTRUCTW___Z(
    CCheckListBox* pThis, CREATESTRUCTW* pCreateStruct) {
    (void)pThis;
    (void)pCreateStruct;
    return 0;
}

// Symbol: ?OnGetCheckPosition@CCheckListBox@@UEAA?AVCRect@@V2@0@Z
extern "C" void MS_ABI impl__OnGetCheckPosition_CCheckListBox__UEAA_AVCRect__V2_0_Z(
    CRect* pRet, CCheckListBox* pThis, CRect rectItem, CRect rectCheck) {
    (void)pThis;
    RECT rc = {rectItem.left, rectItem.top, rectItem.right, rectItem.bottom};
    if (rectCheck.right > rectCheck.left && rectCheck.bottom > rectCheck.top) {
        rc = {rectCheck.left, rectCheck.top, rectCheck.right, rectCheck.bottom};
    }
    if (pRet) *pRet = CheckListBoxCheckRect(rc);
}

// Symbol: ?OnKeyDown@CCheckListBox@@IEAAXIII@Z
extern "C" void MS_ABI impl__OnKeyDown_CCheckListBox__IEAAXIII_Z(
    CCheckListBox* pThis, UINT nChar, UINT nRepCnt, UINT nFlags) {
    (void)nRepCnt;
    (void)nFlags;
    if (!pThis || !pThis->m_hWnd || nChar != VK_SPACE) return;
    int nSel = (int)::SendMessageW(pThis->m_hWnd, LB_GETCURSEL, 0, 0);
    if (nSel == LB_ERR) return;
    pThis->SetCheck(nSel, pThis->GetCheck(nSel) ? 0 : 1);
    impl__InvalidateItem_CCheckListBox__IEAAXH_Z(pThis, nSel);
}

// Symbol: ?OnLBAddString@CCheckListBox@@IEAA_J_K_J@Z
extern "C" LRESULT MS_ABI impl__OnLBAddString_CCheckListBox__IEAA_J_K_J_Z(
    CCheckListBox* pThis, WPARAM wParam, LPARAM lParam) {
    return CheckListBoxSend(pThis, LB_ADDSTRING, wParam, lParam);
}

// Symbol: ?OnLBFindString@CCheckListBox@@IEAA_J_K_J@Z
extern "C" LRESULT MS_ABI impl__OnLBFindString_CCheckListBox__IEAA_J_K_J_Z(
    CCheckListBox* pThis, WPARAM wParam, LPARAM lParam) {
    return CheckListBoxSend(pThis, LB_FINDSTRING, wParam, lParam);
}

// Symbol: ?OnLBFindStringExact@CCheckListBox@@IEAA_J_K_J@Z
extern "C" LRESULT MS_ABI impl__OnLBFindStringExact_CCheckListBox__IEAA_J_K_J_Z(
    CCheckListBox* pThis, WPARAM wParam, LPARAM lParam) {
    return CheckListBoxSend(pThis, LB_FINDSTRINGEXACT, wParam, lParam);
}

// Symbol: ?OnLBGetItemData@CCheckListBox@@IEAA_J_K_J@Z
extern "C" LRESULT MS_ABI impl__OnLBGetItemData_CCheckListBox__IEAA_J_K_J_Z(
    CCheckListBox* pThis, WPARAM wParam, LPARAM lParam) {
    return CheckListBoxSend(pThis, LB_GETITEMDATA, wParam, lParam);
}

// Symbol: ?OnLBGetText@CCheckListBox@@IEAA_J_K_J@Z
extern "C" LRESULT MS_ABI impl__OnLBGetText_CCheckListBox__IEAA_J_K_J_Z(
    CCheckListBox* pThis, WPARAM wParam, LPARAM lParam) {
    return CheckListBoxSend(pThis, LB_GETTEXT, wParam, lParam);
}

// Symbol: ?OnLBInsertString@CCheckListBox@@IEAA_J_K_J@Z
extern "C" LRESULT MS_ABI impl__OnLBInsertString_CCheckListBox__IEAA_J_K_J_Z(
    CCheckListBox* pThis, WPARAM wParam, LPARAM lParam) {
    return CheckListBoxSend(pThis, LB_INSERTSTRING, wParam, lParam);
}

// Symbol: ?OnLBSelectString@CCheckListBox@@IEAA_J_K_J@Z
extern "C" LRESULT MS_ABI impl__OnLBSelectString_CCheckListBox__IEAA_J_K_J_Z(
    CCheckListBox* pThis, WPARAM wParam, LPARAM lParam) {
    return CheckListBoxSend(pThis, LB_SELECTSTRING, wParam, lParam);
}

// Symbol: ?OnLBSetItemData@CCheckListBox@@IEAA_J_K_J@Z
extern "C" LRESULT MS_ABI impl__OnLBSetItemData_CCheckListBox__IEAA_J_K_J_Z(
    CCheckListBox* pThis, WPARAM wParam, LPARAM lParam) {
    return CheckListBoxSend(pThis, LB_SETITEMDATA, wParam, lParam);
}

// Symbol: ?OnLBSetItemHeight@CCheckListBox@@IEAA_J_K_J@Z
extern "C" LRESULT MS_ABI impl__OnLBSetItemHeight_CCheckListBox__IEAA_J_K_J_Z(
    CCheckListBox* pThis, WPARAM wParam, LPARAM lParam) {
    return CheckListBoxSend(pThis, LB_SETITEMHEIGHT, wParam, lParam);
}

// Symbol: ?OnLButtonDown@CCheckListBox@@IEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnLButtonDown_CCheckListBox__IEAAXIVCPoint___Z(
    CCheckListBox* pThis, UINT nFlags, CPoint point) {
    (void)nFlags;
    if (!pThis || !pThis->m_hWnd) return;
    int bInCheck = FALSE;
    int nIndex = impl__CheckFromPoint_CCheckListBox__IEAAHVCPoint__AEAH_Z(pThis, point, &bInCheck);
    if (nIndex == LB_ERR) return;
    ::SendMessageW(pThis->m_hWnd, LB_SETCURSEL, nIndex, 0);
    if (bInCheck) {
        pThis->SetCheck(nIndex, pThis->GetCheck(nIndex) ? 0 : 1);
        impl__InvalidateItem_CCheckListBox__IEAAXH_Z(pThis, nIndex);
    }
}

// Symbol: ?OnLButtonDblClk@CCheckListBox@@IEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnLButtonDblClk_CCheckListBox__IEAAXIVCPoint___Z(
    CCheckListBox* pThis, UINT nFlags, CPoint point) {
    impl__OnLButtonDown_CCheckListBox__IEAAXIVCPoint___Z(pThis, nFlags, point);
}

// Symbol: ?OnSetFont@CCheckListBox@@IEAAXPEAVCFont@@H@Z
extern "C" void MS_ABI impl__OnSetFont_CCheckListBox__IEAAXPEAVCFont__H_Z(
    CCheckListBox* pThis, CFont* pFont, int bRedraw) {
    if (!pThis || !pThis->m_hWnd) return;
    ::SendMessageW(pThis->m_hWnd, WM_SETFONT, (WPARAM)(pFont ? pFont->GetSafeHandle() : nullptr), MAKELPARAM(bRedraw, 0));
}

// Symbol: ?PreCompareItem@CCheckListBox@@IEAAHPEAUtagCOMPAREITEMSTRUCT@@@Z
extern "C" int MS_ABI impl__PreCompareItem_CCheckListBox__IEAAHPEAUtagCOMPAREITEMSTRUCT___Z(
    CCheckListBox* pThis, COMPAREITEMSTRUCT* pCompare) {
    (void)pThis;
    (void)pCompare;
    return 0;
}

// Symbol: ?PreDeleteItem@CCheckListBox@@IEAAXPEAUtagDELETEITEMSTRUCT@@@Z
extern "C" void MS_ABI impl__PreDeleteItem_CCheckListBox__IEAAXPEAUtagDELETEITEMSTRUCT___Z(
    CCheckListBox* pThis, DELETEITEMSTRUCT* pDelete) {
    if (!pThis || !pDelete) return;
    g_checkStates[pThis].erase((int)pDelete->itemID);
    g_itemEnabledStates[pThis].erase((int)pDelete->itemID);
}

// Symbol: ?PreDrawItem@CCheckListBox@@IEAAXPEAUtagDRAWITEMSTRUCT@@@Z
extern "C" void MS_ABI impl__PreDrawItem_CCheckListBox__IEAAXPEAUtagDRAWITEMSTRUCT___Z(
    CCheckListBox* pThis, DRAWITEMSTRUCT* pDraw) {
    impl__DrawItem_CCheckListBox__UEAAXPEAUtagDRAWITEMSTRUCT___Z(pThis, pDraw);
}

// Symbol: ?PreDrawItemHelper@CCheckListBox@@IEAAXPEAUtagDRAWITEMSTRUCT@@@Z
extern "C" void MS_ABI impl__PreDrawItemHelper_CCheckListBox__IEAAXPEAUtagDRAWITEMSTRUCT___Z(
    CCheckListBox* pThis, DRAWITEMSTRUCT* pDraw) {
    impl__DrawItem_CCheckListBox__UEAAXPEAUtagDRAWITEMSTRUCT___Z(pThis, pDraw);
}

// Symbol: ?PreDrawItemNonThemed@CCheckListBox@@IEAAXPEAVCDC@@AEAUtagDRAWITEMSTRUCT@@HH@Z
extern "C" void MS_ABI impl__PreDrawItemNonThemed_CCheckListBox__IEAAXPEAVCDC__AEAUtagDRAWITEMSTRUCT__HH_Z(
    CCheckListBox* pThis, CDC* pDC, DRAWITEMSTRUCT* pDraw, int nCheck, int bEnabled) {
    (void)pDC;
    (void)nCheck;
    (void)bEnabled;
    impl__DrawItem_CCheckListBox__UEAAXPEAUtagDRAWITEMSTRUCT___Z(pThis, pDraw);
}

// Symbol: ?PreDrawItemThemed@CCheckListBox@@IEAA_NPEAVCDC@@AEAUtagDRAWITEMSTRUCT@@HH@Z
extern "C" BOOL MS_ABI impl__PreDrawItemThemed_CCheckListBox__IEAA_NPEAVCDC__AEAUtagDRAWITEMSTRUCT__HH_Z(
    CCheckListBox* pThis, CDC* pDC, DRAWITEMSTRUCT* pDraw, int nCheck, int bEnabled) {
    (void)pThis;
    (void)pDC;
    (void)pDraw;
    (void)nCheck;
    (void)bEnabled;
    return FALSE;
}

// Symbol: ?PreMeasureItem@CCheckListBox@@IEAAXPEAUtagMEASUREITEMSTRUCT@@@Z
extern "C" void MS_ABI impl__PreMeasureItem_CCheckListBox__IEAAXPEAUtagMEASUREITEMSTRUCT___Z(
    CCheckListBox* pThis, MEASUREITEMSTRUCT* pMeasure) {
    impl__MeasureItem_CCheckListBox__UEAAXPEAUtagMEASUREITEMSTRUCT___Z(pThis, pMeasure);
}

// Symbol: ?SetCheckStyle@CCheckListBox@@QEAAXI@Z
extern "C" void MS_ABI impl__SetCheckStyle_CCheckListBox__QEAAXI_Z(CCheckListBox* pThis, unsigned int nStyle) {
    if (!pThis) return;
    g_checkStyles[pThis] = nStyle;
    if (pThis->m_hWnd) ::InvalidateRect(pThis->m_hWnd, nullptr, TRUE);
}
