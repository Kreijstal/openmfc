// Control Core Implementation
//
// Implements CButton, CEdit, CStatic, CListBox, CComboBox, and CScrollBar.
// These wrap Windows common controls using standard Windows messages.

#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxwin.h"
#include "openmfc/afxmfc.h"
#include <windows.h>
#include <oleauto.h>
#include <oleacc.h>
#include <mutex>
#include <string>
#include <unordered_map>
#include <vector>

// MS ABI calling convention
#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

extern "C" int MS_ABI impl__Create_CWnd__UEAAHPEB_W0KAEBUtagRECT__PEAV1_IPEAUCCreateContext___Z(
    CWnd* pThis,
    const wchar_t* lpszClassName,
    const wchar_t* lpszWindowName,
    DWORD dwStyle,
    const RECT& rect,
    CWnd* pParentWnd,
    UINT nID,
    CCreateContext* pContext);

// =============================================================================
// CRuntimeClass implementations for control classes
// =============================================================================

IMPLEMENT_DYNAMIC(CButton, CWnd)
IMPLEMENT_DYNAMIC(CEdit, CWnd)
IMPLEMENT_DYNAMIC(CStatic, CWnd)
IMPLEMENT_DYNAMIC(CListBox, CWnd)
IMPLEMENT_DYNAMIC(CComboBox, CWnd)
IMPLEMENT_DYNAMIC(CCheckListBox, CListBox)
IMPLEMENT_DYNAMIC(CDragListBox, CListBox)
IMPLEMENT_DYNAMIC(CSplitButton, CButton)
IMPLEMENT_DYNAMIC(CScrollBar, CWnd)
IMPLEMENT_DYNAMIC(CSliderCtrl, CWnd)
IMPLEMENT_DYNAMIC(CProgressCtrl, CWnd)
IMPLEMENT_DYNAMIC(CSpinButtonCtrl, CWnd)
IMPLEMENT_DYNAMIC(CListCtrl, CWnd)
IMPLEMENT_DYNAMIC(CTreeCtrl, CWnd)
IMPLEMENT_DYNAMIC(CTabCtrl, CWnd)

#ifdef __GNUC__
// MSVC symbol aliases for runtime class statics
asm(".globl \"?classCButton@CButton@@2UCRuntimeClass@@A\"\n"
    ".set \"?classCButton@CButton@@2UCRuntimeClass@@A\", _ZN7CButton12classCButtonE\n");
asm(".globl \"?classCEdit@CEdit@@2UCRuntimeClass@@A\"\n"
    ".set \"?classCEdit@CEdit@@2UCRuntimeClass@@A\", _ZN5CEdit10classCEditE\n");
asm(".globl \"?classCStatic@CStatic@@2UCRuntimeClass@@A\"\n"
    ".set \"?classCStatic@CStatic@@2UCRuntimeClass@@A\", _ZN7CStatic12classCStaticE\n");
asm(".globl \"?classCListBox@CListBox@@2UCRuntimeClass@@A\"\n"
    ".set \"?classCListBox@CListBox@@2UCRuntimeClass@@A\", _ZN8CListBox13classCListBoxE\n");
asm(".globl \"?classCComboBox@CComboBox@@2UCRuntimeClass@@A\"\n"
    ".set \"?classCComboBox@CComboBox@@2UCRuntimeClass@@A\", _ZN9CComboBox14classCComboBoxE\n");
asm(".globl \"?classCScrollBar@CScrollBar@@2UCRuntimeClass@@A\"\n"
    ".set \"?classCScrollBar@CScrollBar@@2UCRuntimeClass@@A\", _ZN10CScrollBar15classCScrollBarE\n");
asm(".globl \"?classCSliderCtrl@CSliderCtrl@@2UCRuntimeClass@@A\"\n"
    ".set \"?classCSliderCtrl@CSliderCtrl@@2UCRuntimeClass@@A\", _ZN11CSliderCtrl16classCSliderCtrlE\n");
asm(".globl \"?classCProgressCtrl@CProgressCtrl@@2UCRuntimeClass@@A\"\n"
    ".set \"?classCProgressCtrl@CProgressCtrl@@2UCRuntimeClass@@A\", _ZN13CProgressCtrl18classCProgressCtrlE\n");
asm(".globl \"?classCSpinButtonCtrl@CSpinButtonCtrl@@2UCRuntimeClass@@A\"\n"
    ".set \"?classCSpinButtonCtrl@CSpinButtonCtrl@@2UCRuntimeClass@@A\", _ZN15CSpinButtonCtrl20classCSpinButtonCtrlE\n");
asm(".globl \"?classCListCtrl@CListCtrl@@2UCRuntimeClass@@A\"\n"
    ".set \"?classCListCtrl@CListCtrl@@2UCRuntimeClass@@A\", _ZN9CListCtrl14classCListCtrlE\n");
asm(".globl \"?classCTreeCtrl@CTreeCtrl@@2UCRuntimeClass@@A\"\n"
    ".set \"?classCTreeCtrl@CTreeCtrl@@2UCRuntimeClass@@A\", _ZN9CTreeCtrl14classCTreeCtrlE\n");
asm(".globl \"?classCTabCtrl@CTabCtrl@@2UCRuntimeClass@@A\"\n"
    ".set \"?classCTabCtrl@CTabCtrl@@2UCRuntimeClass@@A\", _ZN8CTabCtrl13classCTabCtrlE\n");

// MSVC symbol aliases for Create methods
// Symbol: ?Create@CButton@@UEAAHPEB_WKAEBUtagRECT@@PEAVCWnd@@I@Z
asm(".globl \"?Create@CButton@@UEAAHPEB_WKAEBUtagRECT@@PEAVCWnd@@I@Z\"\n"
    ".set \"?Create@CButton@@UEAAHPEB_WKAEBUtagRECT@@PEAVCWnd@@I@Z\", impl__Create_CButton__UEAAHPEB_WKAEBUtagRECT__PEAVCWnd__I_Z\n");
// Symbol: ?Create@CEdit@@QEAAHKAEBUtagRECT@@PEAVCWnd@@I@Z
asm(".globl \"?Create@CEdit@@QEAAHKAEBUtagRECT@@PEAVCWnd@@I@Z\"\n"
    ".set \"?Create@CEdit@@QEAAHKAEBUtagRECT@@PEAVCWnd@@I@Z\", impl__Create_CEdit__QEAAHKAEBUtagRECT__PEAVCWnd__I_Z\n");
// Symbol: ?Create@CStatic@@UEAAHPEB_WKAEBUtagRECT@@PEAVCWnd@@I@Z
asm(".globl \"?Create@CStatic@@UEAAHPEB_WKAEBUtagRECT@@PEAVCWnd@@I@Z\"\n"
    ".set \"?Create@CStatic@@UEAAHPEB_WKAEBUtagRECT@@PEAVCWnd@@I@Z\", impl__Create_CStatic__UEAAHPEB_WKAEBUtagRECT__PEAVCWnd__I_Z\n");
// Symbol: ?Create@CListBox@@UEAAHKAEBUtagRECT@@PEAVCWnd@@I@Z
asm(".globl \"?Create@CListBox@@UEAAHKAEBUtagRECT@@PEAVCWnd@@I@Z\"\n"
    ".set \"?Create@CListBox@@UEAAHKAEBUtagRECT@@PEAVCWnd@@I@Z\", impl__Create_CListBox__UEAAHKAEBUtagRECT__PEAVCWnd__I_Z\n");
// Symbol: ?Create@CComboBox@@UEAAHKAEBUtagRECT@@PEAVCWnd@@I@Z
asm(".globl \"?Create@CComboBox@@UEAAHKAEBUtagRECT@@PEAVCWnd@@I@Z\"\n"
    ".set \"?Create@CComboBox@@UEAAHKAEBUtagRECT@@PEAVCWnd@@I@Z\", impl__Create_CComboBox__UEAAHKAEBUtagRECT__PEAVCWnd__I_Z\n");
// Symbol: ?Create@CScrollBar@@UEAAHKAEBUtagRECT@@PEAVCWnd@@I@Z
asm(".globl \"?Create@CScrollBar@@UEAAHKAEBUtagRECT@@PEAVCWnd@@I@Z\"\n"
    ".set \"?Create@CScrollBar@@UEAAHKAEBUtagRECT@@PEAVCWnd@@I@Z\", impl__Create_CScrollBar__UEAAHKAEBUtagRECT__PEAVCWnd__I_Z\n");
// Symbol: ?Create@CSliderCtrl@@UEAAHKAEBUtagRECT@@PEAVCWnd@@I@Z
asm(".globl \"?Create@CSliderCtrl@@UEAAHKAEBUtagRECT@@PEAVCWnd@@I@Z\"\n"
    ".set \"?Create@CSliderCtrl@@UEAAHKAEBUtagRECT@@PEAVCWnd@@I@Z\", impl__Create_CSliderCtrl__UEAAHKAEBUtagRECT__PEAVCWnd__I_Z\n");
// Symbol: ?Create@CProgressCtrl@@UEAAHKAEBUtagRECT@@PEAVCWnd@@I@Z
asm(".globl \"?Create@CProgressCtrl@@UEAAHKAEBUtagRECT@@PEAVCWnd@@I@Z\"\n"
    ".set \"?Create@CProgressCtrl@@UEAAHKAEBUtagRECT@@PEAVCWnd@@I@Z\", impl__Create_CProgressCtrl__UEAAHKAEBUtagRECT__PEAVCWnd__I_Z\n");
// Symbol: ?Create@CSpinButtonCtrl@@UEAAHKAEBUtagRECT@@PEAVCWnd@@I@Z
asm(".globl \"?Create@CSpinButtonCtrl@@UEAAHKAEBUtagRECT@@PEAVCWnd@@I@Z\"\n"
    ".set \"?Create@CSpinButtonCtrl@@UEAAHKAEBUtagRECT@@PEAVCWnd@@I@Z\", impl__Create_CSpinButtonCtrl__UEAAHKAEBUtagRECT__PEAVCWnd__I_Z\n");
// Symbol: ?Create@CListCtrl@@QEAAHKAEBUtagRECT@@PEAVCWnd@@I@Z
asm(".globl \"?Create@CListCtrl@@QEAAHKAEBUtagRECT@@PEAVCWnd@@I@Z\"\n"
    ".set \"?Create@CListCtrl@@QEAAHKAEBUtagRECT@@PEAVCWnd@@I@Z\", impl__Create_CListCtrl__QEAAHKAEBUtagRECT__PEAVCWnd__I_Z\n");
// Symbol: ?Create@CTreeCtrl@@QEAAHKAEBUtagRECT@@PEAVCWnd@@I@Z
asm(".globl \"?Create@CTreeCtrl@@QEAAHKAEBUtagRECT@@PEAVCWnd@@I@Z\"\n"
    ".set \"?Create@CTreeCtrl@@QEAAHKAEBUtagRECT@@PEAVCWnd@@I@Z\", impl__Create_CTreeCtrl__QEAAHKAEBUtagRECT__PEAVCWnd__I_Z\n");
// Symbol: ?Create@CTabCtrl@@UEAAHKAEBUtagRECT@@PEAVCWnd@@I@Z
asm(".globl \"?Create@CTabCtrl@@UEAAHKAEBUtagRECT@@PEAVCWnd@@I@Z\"\n"
    ".set \"?Create@CTabCtrl@@UEAAHKAEBUtagRECT@@PEAVCWnd@@I@Z\", impl__Create_CTabCtrl__UEAAHKAEBUtagRECT__PEAVCWnd__I_Z\n");

// MSVC symbol aliases for CButton methods
asm(".globl \"?GetState@CButton@@QEBAIXZ\"\n"
    ".set \"?GetState@CButton@@QEBAIXZ\", impl__GetState_CButton__QEBAIXZ\n");
asm(".globl \"?SetState@CButton@@QEAAXH@Z\"\n"
    ".set \"?SetState@CButton@@QEAAXH@Z\", impl__SetState_CButton__QEAAXH_Z\n");
asm(".globl \"?GetCheck@CButton@@QEBAHXZ\"\n"
    ".set \"?GetCheck@CButton@@QEBAHXZ\", impl__GetCheck_CButton__QEBAHXZ\n");
asm(".globl \"?SetCheck@CButton@@QEAAXH@Z\"\n"
    ".set \"?SetCheck@CButton@@QEAAXH@Z\", impl__SetCheck_CButton__QEAAXH_Z\n");
asm(".globl \"?GetButtonStyle@CButton@@QEBAIXZ\"\n"
    ".set \"?GetButtonStyle@CButton@@QEBAIXZ\", impl__GetButtonStyle_CButton__QEBAIXZ\n");
asm(".globl \"?SetButtonStyle@CButton@@QEAAXIH@Z\"\n"
    ".set \"?SetButtonStyle@CButton@@QEAAXIH@Z\", impl__SetButtonStyle_CButton__QEAAXIH_Z\n");

// MSVC symbol aliases for CEdit methods
asm(".globl \"?GetLineCount@CEdit@@QEBAHXZ\"\n"
    ".set \"?GetLineCount@CEdit@@QEBAHXZ\", impl__GetLineCount_CEdit__QEBAHXZ\n");
asm(".globl \"?SetReadOnly@CEdit@@QEAAXH@Z\"\n"
    ".set \"?SetReadOnly@CEdit@@QEAAXH@Z\", impl__SetReadOnly_CEdit__QEAAXH_Z\n");
asm(".globl \"?GetModify@CEdit@@QEBAHXZ\"\n"
    ".set \"?GetModify@CEdit@@QEBAHXZ\", impl__GetModify_CEdit__QEBAHXZ\n");
asm(".globl \"?SetModify@CEdit@@QEAAXH@Z\"\n"
    ".set \"?SetModify@CEdit@@QEAAXH@Z\", impl__SetModify_CEdit__QEAAXH_Z\n");
asm(".globl \"?Clear@CEdit@@QEAAXXZ\"\n"
    ".set \"?Clear@CEdit@@QEAAXXZ\", impl__Clear_CEdit__QEAAXXZ\n");
asm(".globl \"?Copy@CEdit@@QEAAXXZ\"\n"
    ".set \"?Copy@CEdit@@QEAAXXZ\", impl__Copy_CEdit__QEAAXXZ\n");
asm(".globl \"?Cut@CEdit@@QEAAXXZ\"\n"
    ".set \"?Cut@CEdit@@QEAAXXZ\", impl__Cut_CEdit__QEAAXXZ\n");
asm(".globl \"?Paste@CEdit@@QEAAXXZ\"\n"
    ".set \"?Paste@CEdit@@QEAAXXZ\", impl__Paste_CEdit__QEAAXXZ\n");
asm(".globl \"?Undo@CEdit@@QEAAHXZ\"\n"
    ".set \"?Undo@CEdit@@QEAAHXZ\", impl__Undo_CEdit__QEAAHXZ\n");

// MSVC symbol aliases for CListBox methods
asm(".globl \"?GetCount@CListBox@@QEBAHXZ\"\n"
    ".set \"?GetCount@CListBox@@QEBAHXZ\", impl__GetCount_CListBox__QEBAHXZ\n");
asm(".globl \"?GetCurSel@CListBox@@QEBAHXZ\"\n"
    ".set \"?GetCurSel@CListBox@@QEBAHXZ\", impl__GetCurSel_CListBox__QEBAHXZ\n");
asm(".globl \"?SetCurSel@CListBox@@QEAAHH@Z\"\n"
    ".set \"?SetCurSel@CListBox@@QEAAHH@Z\", impl__SetCurSel_CListBox__QEAAHH_Z\n");
asm(".globl \"?ResetContent@CListBox@@QEAAXXZ\"\n"
    ".set \"?ResetContent@CListBox@@QEAAXXZ\", impl__ResetContent_CListBox__QEAAXXZ\n");
// Symbol: ?AddString@CListBox@@QEAAHPEB_W@Z
asm(".globl \"?AddString@CListBox@@QEAAHPEB_W@Z\"\n"
    ".set \"?AddString@CListBox@@QEAAHPEB_W@Z\", impl__AddString_CListBox__QEAAHPEB_W_Z\n");
// Symbol: ?DeleteString@CListBox@@QEAAHI@Z
asm(".globl \"?DeleteString@CListBox@@QEAAHI@Z\"\n"
    ".set \"?DeleteString@CListBox@@QEAAHI@Z\", impl__DeleteString_CListBox__QEAAHI_Z\n");
// Symbol: ?InsertString@CListBox@@QEAAHHPEB_W@Z
asm(".globl \"?InsertString@CListBox@@QEAAHHPEB_W@Z\"\n"
    ".set \"?InsertString@CListBox@@QEAAHHPEB_W@Z\", impl__InsertString_CListBox__QEAAHHPEB_W_Z\n");
// Symbol: ?FindString@CListBox@@QEBAHPEB_WH@Z
asm(".globl \"?FindString@CListBox@@QEBAHPEB_WH@Z\"\n"
    ".set \"?FindString@CListBox@@QEBAHPEB_WH@Z\", impl__FindString_CListBox__QEBAHPEB_WH_Z\n");

// MSVC symbol aliases for CComboBox methods
asm(".globl \"?GetCount@CComboBox@@QEBAHXZ\"\n"
    ".set \"?GetCount@CComboBox@@QEBAHXZ\", impl__GetCount_CComboBox__QEBAHXZ\n");
asm(".globl \"?GetCurSel@CComboBox@@QEBAHXZ\"\n"
    ".set \"?GetCurSel@CComboBox@@QEBAHXZ\", impl__GetCurSel_CComboBox__QEBAHXZ\n");
asm(".globl \"?SetCurSel@CComboBox@@QEAAHH@Z\"\n"
    ".set \"?SetCurSel@CComboBox@@QEAAHH@Z\", impl__SetCurSel_CComboBox__QEAAHH_Z\n");
asm(".globl \"?ResetContent@CComboBox@@QEAAXXZ\"\n"
    ".set \"?ResetContent@CComboBox@@QEAAXXZ\", impl__ResetContent_CComboBox__QEAAXXZ\n");
asm(".globl \"?ShowDropDown@CComboBox@@QEAAXH@Z\"\n"
    ".set \"?ShowDropDown@CComboBox@@QEAAXH@Z\", impl__ShowDropDown_CComboBox__QEAAXH_Z\n");
asm(".globl \"?GetDroppedState@CComboBox@@QEBAHXZ\"\n"
    ".set \"?GetDroppedState@CComboBox@@QEBAHXZ\", impl__GetDroppedState_CComboBox__QEBAHXZ\n");
// Symbol: ?AddString@CComboBox@@QEAAHPEB_W@Z
asm(".globl \"?AddString@CComboBox@@QEAAHPEB_W@Z\"\n"
    ".set \"?AddString@CComboBox@@QEAAHPEB_W@Z\", impl__AddString_CComboBox__QEAAHPEB_W_Z\n");
// Symbol: ?DeleteString@CComboBox@@QEAAHI@Z
asm(".globl \"?DeleteString@CComboBox@@QEAAHI@Z\"\n"
    ".set \"?DeleteString@CComboBox@@QEAAHI@Z\", impl__DeleteString_CComboBox__QEAAHI_Z\n");
// Symbol: ?InsertString@CComboBox@@QEAAHHPEB_W@Z
asm(".globl \"?InsertString@CComboBox@@QEAAHHPEB_W@Z\"\n"
    ".set \"?InsertString@CComboBox@@QEAAHHPEB_W@Z\", impl__InsertString_CComboBox__QEAAHHPEB_W_Z\n");
// Symbol: ?FindString@CComboBox@@QEBAHPEB_WH@Z
asm(".globl \"?FindString@CComboBox@@QEBAHPEB_WH@Z\"\n"
    ".set \"?FindString@CComboBox@@QEBAHPEB_WH@Z\", impl__FindString_CComboBox__QEBAHPEB_WH_Z\n");

// MSVC symbol aliases for CScrollBar methods
asm(".globl \"?GetScrollPos@CScrollBar@@QEBAHXZ\"\n"
    ".set \"?GetScrollPos@CScrollBar@@QEBAHXZ\", impl__GetScrollPos_CScrollBar__QEBAHXZ\n");
asm(".globl \"?SetScrollPos@CScrollBar@@QEAAHHH@Z\"\n"
    ".set \"?SetScrollPos@CScrollBar@@QEAAHHH@Z\", impl__SetScrollPos_CScrollBar__QEAAHHH_Z\n");
#endif

// =============================================================================
// CButton Implementation
// =============================================================================

extern "C" int MS_ABI impl__Create_CButton__UEAAHPEB_WKAEBUtagRECT__PEAVCWnd__I_Z(
    CButton* pThis, const wchar_t* lpszCaption, DWORD dwStyle, const RECT* pRect, CWnd* pParentWnd, UINT nID) {
    if (!pThis || !pRect) return FALSE;
    return impl__Create_CWnd__UEAAHPEB_W0KAEBUtagRECT__PEAV1_IPEAUCCreateContext___Z(
        pThis, L"BUTTON", lpszCaption, dwStyle, *pRect, pParentWnd, nID, nullptr);
}

// CButton::GetState
extern "C" unsigned int MS_ABI impl__GetState_CButton__QEBAIXZ(const CButton* pThis) {
    if (!pThis || !pThis->m_hWnd) return 0;
    return (unsigned int)::SendMessageW(pThis->m_hWnd, BM_GETSTATE, 0, 0);
}

// CButton::SetState
extern "C" void MS_ABI impl__SetState_CButton__QEAAXH_Z(CButton* pThis, int bHighlight) {
    if (pThis && pThis->m_hWnd) {
        ::SendMessageW(pThis->m_hWnd, BM_SETSTATE, bHighlight ? TRUE : FALSE, 0);
    }
}

// CButton::GetCheck
extern "C" int MS_ABI impl__GetCheck_CButton__QEBAHXZ(const CButton* pThis) {
    if (!pThis || !pThis->m_hWnd) return 0;
    return (int)::SendMessageW(pThis->m_hWnd, BM_GETCHECK, 0, 0);
}

// CButton::SetCheck
extern "C" void MS_ABI impl__SetCheck_CButton__QEAAXH_Z(CButton* pThis, int nCheck) {
    if (pThis && pThis->m_hWnd) {
        ::SendMessageW(pThis->m_hWnd, BM_SETCHECK, nCheck, 0);
    }
}

// CButton::GetButtonStyle
extern "C" unsigned int MS_ABI impl__GetButtonStyle_CButton__QEBAIXZ(const CButton* pThis) {
    if (!pThis || !pThis->m_hWnd) return 0;
    return (unsigned int)(::GetWindowLongW(pThis->m_hWnd, GWL_STYLE) & 0xFFFF);
}

// CButton::SetButtonStyle
extern "C" void MS_ABI impl__SetButtonStyle_CButton__QEAAXIH_Z(CButton* pThis, unsigned int nStyle, int bRedraw) {
    if (pThis && pThis->m_hWnd) {
        ::SendMessageW(pThis->m_hWnd, BM_SETSTYLE, nStyle, MAKELPARAM(bRedraw, 0));
    }
}

// CButton::GetBitmap
extern "C" void* MS_ABI impl__GetBitmap_CButton__QEBAPEAXXZ(const CButton* pThis) {
    if (!pThis || !pThis->m_hWnd) return nullptr;
    return (void*)::SendMessageW(pThis->m_hWnd, BM_GETIMAGE, IMAGE_BITMAP, 0);
}

// CButton::SetBitmap
extern "C" void* MS_ABI impl__SetBitmap_CButton__QEAAPEAXPEAX_Z(CButton* pThis, void* hBitmap) {
    if (!pThis || !pThis->m_hWnd) return nullptr;
    return (void*)::SendMessageW(pThis->m_hWnd, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hBitmap);
}

// CButton::GetIcon
extern "C" void* MS_ABI impl__GetIcon_CButton__QEBAPEAXXZ(const CButton* pThis) {
    if (!pThis || !pThis->m_hWnd) return nullptr;
    return (void*)::SendMessageW(pThis->m_hWnd, BM_GETIMAGE, IMAGE_ICON, 0);
}

// CButton::SetIcon
extern "C" void* MS_ABI impl__SetIcon_CButton__QEAAPEAXPEAX_Z(CButton* pThis, void* hIcon) {
    if (!pThis || !pThis->m_hWnd) return nullptr;
    return (void*)::SendMessageW(pThis->m_hWnd, BM_SETIMAGE, IMAGE_ICON, (LPARAM)hIcon);
}

// =============================================================================
// CEdit Implementation
// =============================================================================

extern "C" int MS_ABI impl__Create_CEdit__QEAAHKAEBUtagRECT__PEAVCWnd__I_Z(
    CEdit* pThis, DWORD dwStyle, const RECT* pRect, CWnd* pParentWnd, UINT nID) {
    if (!pThis || !pRect) return FALSE;
    return impl__Create_CWnd__UEAAHPEB_W0KAEBUtagRECT__PEAV1_IPEAUCCreateContext___Z(
        pThis, L"EDIT", L"", dwStyle, *pRect, pParentWnd, nID, nullptr);
}

// CEdit::GetLineCount
extern "C" int MS_ABI impl__GetLineCount_CEdit__QEBAHXZ(const CEdit* pThis) {
    if (!pThis || !pThis->m_hWnd) return 1;
    return (int)::SendMessageW(pThis->m_hWnd, EM_GETLINECOUNT, 0, 0);
}

// CEdit::GetLine
extern "C" int MS_ABI impl__GetLine_CEdit__QEBAHPEA_WH_Z(const CEdit* pThis, int nIndex, wchar_t* lpszBuffer, int nMaxLength) {
    if (!pThis || !pThis->m_hWnd || !lpszBuffer || nMaxLength <= 0) return 0;
    // First word of buffer must contain the max length
    *(WORD*)lpszBuffer = (WORD)nMaxLength;
    return (int)::SendMessageW(pThis->m_hWnd, EM_GETLINE, nIndex, (LPARAM)lpszBuffer);
}

// CEdit::GetSel
extern "C" void MS_ABI impl__GetSel_CEdit__QEBAXAEAH0_Z(const CEdit* pThis, int* nStartChar, int* nEndChar) {
    if (!pThis || !pThis->m_hWnd) {
        if (nStartChar) *nStartChar = 0;
        if (nEndChar) *nEndChar = 0;
        return;
    }
    DWORD dwStart = 0, dwEnd = 0;
    ::SendMessageW(pThis->m_hWnd, EM_GETSEL, (WPARAM)&dwStart, (LPARAM)&dwEnd);
    if (nStartChar) *nStartChar = (int)dwStart;
    if (nEndChar) *nEndChar = (int)dwEnd;
}

// CEdit::SetSel
extern "C" void MS_ABI impl__SetSel_CEdit__QEAAXHHH_Z(CEdit* pThis, int nStartChar, int nEndChar, int bNoScroll) {
    if (pThis && pThis->m_hWnd) {
        ::SendMessageW(pThis->m_hWnd, EM_SETSEL, nStartChar, nEndChar);
        if (!bNoScroll) {
            ::SendMessageW(pThis->m_hWnd, EM_SCROLLCARET, 0, 0);
        }
    }
}

// CEdit::ReplaceSel
extern "C" void MS_ABI impl__ReplaceSel_CEdit__QEAAXPEB_WH_Z(CEdit* pThis, const wchar_t* lpszNewText, int bCanUndo) {
    if (pThis && pThis->m_hWnd) {
        ::SendMessageW(pThis->m_hWnd, EM_REPLACESEL, bCanUndo ? TRUE : FALSE, (LPARAM)lpszNewText);
    }
}

// CEdit::Clear
extern "C" void MS_ABI impl__Clear_CEdit__QEAAXXZ(CEdit* pThis) {
    if (pThis && pThis->m_hWnd) {
        ::SendMessageW(pThis->m_hWnd, WM_CLEAR, 0, 0);
    }
}

// CEdit::Copy
extern "C" void MS_ABI impl__Copy_CEdit__QEAAXXZ(CEdit* pThis) {
    if (pThis && pThis->m_hWnd) {
        ::SendMessageW(pThis->m_hWnd, WM_COPY, 0, 0);
    }
}

// CEdit::Cut
extern "C" void MS_ABI impl__Cut_CEdit__QEAAXXZ(CEdit* pThis) {
    if (pThis && pThis->m_hWnd) {
        ::SendMessageW(pThis->m_hWnd, WM_CUT, 0, 0);
    }
}

// CEdit::Paste
extern "C" void MS_ABI impl__Paste_CEdit__QEAAXXZ(CEdit* pThis) {
    if (pThis && pThis->m_hWnd) {
        ::SendMessageW(pThis->m_hWnd, WM_PASTE, 0, 0);
    }
}

// CEdit::Undo
extern "C" int MS_ABI impl__Undo_CEdit__QEAAHXZ(CEdit* pThis) {
    if (!pThis || !pThis->m_hWnd) return FALSE;
    return (int)::SendMessageW(pThis->m_hWnd, EM_UNDO, 0, 0);
}

// CEdit::SetReadOnly
extern "C" void MS_ABI impl__SetReadOnly_CEdit__QEAAXH_Z(CEdit* pThis, int bReadOnly) {
    if (pThis && pThis->m_hWnd) {
        ::SendMessageW(pThis->m_hWnd, EM_SETREADONLY, bReadOnly ? TRUE : FALSE, 0);
    }
}

// CEdit::GetModify
extern "C" int MS_ABI impl__GetModify_CEdit__QEBAHXZ(const CEdit* pThis) {
    if (!pThis || !pThis->m_hWnd) return FALSE;
    return (int)::SendMessageW(pThis->m_hWnd, EM_GETMODIFY, 0, 0);
}

// CEdit::SetModify
extern "C" void MS_ABI impl__SetModify_CEdit__QEAAXH_Z(CEdit* pThis, int bModified) {
    if (pThis && pThis->m_hWnd) {
        ::SendMessageW(pThis->m_hWnd, EM_SETMODIFY, bModified ? TRUE : FALSE, 0);
    }
}

// CEdit::LimitText
extern "C" void MS_ABI impl__LimitText_CEdit__QEAAXH_Z(CEdit* pThis, int nChars) {
    if (pThis && pThis->m_hWnd) {
        ::SendMessageW(pThis->m_hWnd, EM_LIMITTEXT, nChars, 0);
    }
}

// CEdit::GetLimitText
extern "C" int MS_ABI impl__GetLimitText_CEdit__QEBAHXZ(const CEdit* pThis) {
    if (!pThis || !pThis->m_hWnd) return 0;
    return (int)::SendMessageW(pThis->m_hWnd, EM_GETLIMITTEXT, 0, 0);
}

// CEdit::SetPasswordChar
extern "C" void MS_ABI impl__SetPasswordChar_CEdit__QEAAX_W_Z(CEdit* pThis, wchar_t ch) {
    if (pThis && pThis->m_hWnd) {
        ::SendMessageW(pThis->m_hWnd, EM_SETPASSWORDCHAR, ch, 0);
    }
}

// =============================================================================
// CStatic Implementation
// =============================================================================

extern "C" int MS_ABI impl__Create_CStatic__UEAAHPEB_WKAEBUtagRECT__PEAVCWnd__I_Z(
    CStatic* pThis, const wchar_t* lpszText, DWORD dwStyle, const RECT* pRect, CWnd* pParentWnd, UINT nID) {
    if (!pThis || !pRect) return FALSE;
    return impl__Create_CWnd__UEAAHPEB_W0KAEBUtagRECT__PEAV1_IPEAUCCreateContext___Z(
        pThis, L"STATIC", lpszText, dwStyle, *pRect, pParentWnd, nID, nullptr);
}

// CStatic::GetBitmap
extern "C" void* MS_ABI impl__GetBitmap_CStatic__QEBAPEAXXZ(const CStatic* pThis) {
    if (!pThis || !pThis->m_hWnd) return nullptr;
    return (void*)::SendMessageW(pThis->m_hWnd, STM_GETIMAGE, IMAGE_BITMAP, 0);
}

// CStatic::SetBitmap
extern "C" void* MS_ABI impl__SetBitmap_CStatic__QEAAPEAXPEAX_Z(CStatic* pThis, void* hBitmap) {
    if (!pThis || !pThis->m_hWnd) return nullptr;
    return (void*)::SendMessageW(pThis->m_hWnd, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hBitmap);
}

// CStatic::GetIcon
extern "C" void* MS_ABI impl__GetIcon_CStatic__QEBAPEAXXZ(const CStatic* pThis) {
    if (!pThis || !pThis->m_hWnd) return nullptr;
    return (void*)::SendMessageW(pThis->m_hWnd, STM_GETICON, 0, 0);
}

// CStatic::SetIcon
extern "C" void* MS_ABI impl__SetIcon_CStatic__QEAAPEAXPEAX_Z(CStatic* pThis, void* hIcon) {
    if (!pThis || !pThis->m_hWnd) return nullptr;
    return (void*)::SendMessageW(pThis->m_hWnd, STM_SETICON, (WPARAM)hIcon, 0);
}

// CStatic::GetEnhMetaFile
extern "C" void* MS_ABI impl__GetEnhMetaFile_CStatic__QEBAPEAXXZ(const CStatic* pThis) {
    if (!pThis || !pThis->m_hWnd) return nullptr;
    return (void*)::SendMessageW(pThis->m_hWnd, STM_GETIMAGE, IMAGE_ENHMETAFILE, 0);
}

// CStatic::SetEnhMetaFile
extern "C" void* MS_ABI impl__SetEnhMetaFile_CStatic__QEAAPEAXPEAX_Z(CStatic* pThis, void* hMetaFile) {
    if (!pThis || !pThis->m_hWnd) return nullptr;
    return (void*)::SendMessageW(pThis->m_hWnd, STM_SETIMAGE, IMAGE_ENHMETAFILE, (LPARAM)hMetaFile);
}

// =============================================================================
// CListBox Implementation
// =============================================================================

extern "C" int MS_ABI impl__Create_CListBox__UEAAHKAEBUtagRECT__PEAVCWnd__I_Z(
    CListBox* pThis, DWORD dwStyle, const RECT* pRect, CWnd* pParentWnd, UINT nID) {
    if (!pThis || !pRect) return FALSE;
    return impl__Create_CWnd__UEAAHPEB_W0KAEBUtagRECT__PEAV1_IPEAUCCreateContext___Z(
        pThis, L"LISTBOX", L"", dwStyle, *pRect, pParentWnd, nID, nullptr);
}

// CListBox::GetCount
extern "C" int MS_ABI impl__GetCount_CListBox__QEBAHXZ(const CListBox* pThis) {
    if (!pThis || !pThis->m_hWnd) return 0;
    return (int)::SendMessageW(pThis->m_hWnd, LB_GETCOUNT, 0, 0);
}

// CListBox::GetCurSel
extern "C" int MS_ABI impl__GetCurSel_CListBox__QEBAHXZ(const CListBox* pThis) {
    if (!pThis || !pThis->m_hWnd) return LB_ERR;
    return (int)::SendMessageW(pThis->m_hWnd, LB_GETCURSEL, 0, 0);
}

// CListBox::SetCurSel
extern "C" int MS_ABI impl__SetCurSel_CListBox__QEAAHH_Z(CListBox* pThis, int nSelect) {
    if (!pThis || !pThis->m_hWnd) return LB_ERR;
    return (int)::SendMessageW(pThis->m_hWnd, LB_SETCURSEL, nSelect, 0);
}

// CListBox::GetText
extern "C" int MS_ABI impl__GetText_CListBox__QEBAHPEA_WH_Z(const CListBox* pThis, int nIndex, wchar_t* lpszBuffer) {
    if (!pThis || !pThis->m_hWnd || !lpszBuffer) return LB_ERR;
    return (int)::SendMessageW(pThis->m_hWnd, LB_GETTEXT, nIndex, (LPARAM)lpszBuffer);
}

// CListBox::GetTextLen
extern "C" int MS_ABI impl__GetTextLen_CListBox__QEBAHH_Z(const CListBox* pThis, int nIndex) {
    if (!pThis || !pThis->m_hWnd) return LB_ERR;
    return (int)::SendMessageW(pThis->m_hWnd, LB_GETTEXTLEN, nIndex, 0);
}

// CListBox::AddString
extern "C" int MS_ABI impl__AddString_CListBox__QEAAHPEB_W_Z(CListBox* pThis, const wchar_t* lpszItem) {
    if (!pThis || !pThis->m_hWnd) return LB_ERR;
    return (int)::SendMessageW(pThis->m_hWnd, LB_ADDSTRING, 0, (LPARAM)lpszItem);
}

// CListBox::InsertString
extern "C" int MS_ABI impl__InsertString_CListBox__QEAAHHPEB_W_Z(CListBox* pThis, int nIndex, const wchar_t* lpszItem) {
    if (!pThis || !pThis->m_hWnd) return LB_ERR;
    return (int)::SendMessageW(pThis->m_hWnd, LB_INSERTSTRING, nIndex, (LPARAM)lpszItem);
}

// CListBox::DeleteString
extern "C" int MS_ABI impl__DeleteString_CListBox__QEAAHI_Z(CListBox* pThis, unsigned int nIndex) {
    if (!pThis || !pThis->m_hWnd) return LB_ERR;
    return (int)::SendMessageW(pThis->m_hWnd, LB_DELETESTRING, nIndex, 0);
}

// CListBox::ResetContent
extern "C" void MS_ABI impl__ResetContent_CListBox__QEAAXXZ(CListBox* pThis) {
    if (pThis && pThis->m_hWnd) {
        ::SendMessageW(pThis->m_hWnd, LB_RESETCONTENT, 0, 0);
    }
}

// CListBox::FindString
extern "C" int MS_ABI impl__FindString_CListBox__QEBAHPEB_WH_Z(const CListBox* pThis, const wchar_t* lpszItem, int nStartAfter) {
    if (!pThis || !pThis->m_hWnd) return LB_ERR;
    return (int)::SendMessageW(pThis->m_hWnd, LB_FINDSTRING, nStartAfter, (LPARAM)lpszItem);
}

// CListBox::SelectString
extern "C" int MS_ABI impl__SelectString_CListBox__QEAAHPEB_WH_Z(CListBox* pThis, int nStartAfter, const wchar_t* lpszItem) {
    if (!pThis || !pThis->m_hWnd) return LB_ERR;
    return (int)::SendMessageW(pThis->m_hWnd, LB_SELECTSTRING, nStartAfter, (LPARAM)lpszItem);
}

// CListBox::GetItemData
extern "C" uintptr_t MS_ABI impl__GetItemData_CListBox__QEBA_KH_Z(const CListBox* pThis, int nIndex) {
    if (!pThis || !pThis->m_hWnd) return 0;
    return (uintptr_t)::SendMessageW(pThis->m_hWnd, LB_GETITEMDATA, nIndex, 0);
}

// CListBox::SetItemData
extern "C" int MS_ABI impl__SetItemData_CListBox__QEAAHH_K_Z(CListBox* pThis, int nIndex, uintptr_t dwItemData) {
    if (!pThis || !pThis->m_hWnd) return LB_ERR;
    return (int)::SendMessageW(pThis->m_hWnd, LB_SETITEMDATA, nIndex, (LPARAM)dwItemData);
}

// CListBox::GetItemDataPtr
extern "C" void* MS_ABI impl__GetItemDataPtr_CListBox__QEBAPEAXH_Z(const CListBox* pThis, int nIndex) {
    if (!pThis || !pThis->m_hWnd) return nullptr;
    return (void*)::SendMessageW(pThis->m_hWnd, LB_GETITEMDATA, nIndex, 0);
}

// CListBox::SetItemDataPtr
extern "C" int MS_ABI impl__SetItemDataPtr_CListBox__QEAAHHPEAX_Z(CListBox* pThis, int nIndex, void* pData) {
    if (!pThis || !pThis->m_hWnd) return LB_ERR;
    return (int)::SendMessageW(pThis->m_hWnd, LB_SETITEMDATA, nIndex, (LPARAM)pData);
}

// =============================================================================
// CComboBox Implementation
// =============================================================================

extern "C" int MS_ABI impl__Create_CComboBox__UEAAHKAEBUtagRECT__PEAVCWnd__I_Z(
    CComboBox* pThis, DWORD dwStyle, const RECT* pRect, CWnd* pParentWnd, UINT nID) {
    if (!pThis || !pRect) return FALSE;
    return impl__Create_CWnd__UEAAHPEB_W0KAEBUtagRECT__PEAV1_IPEAUCCreateContext___Z(
        pThis, L"COMBOBOX", L"", dwStyle, *pRect, pParentWnd, nID, nullptr);
}

// CComboBox::GetCount
extern "C" int MS_ABI impl__GetCount_CComboBox__QEBAHXZ(const CComboBox* pThis) {
    if (!pThis || !pThis->m_hWnd) return 0;
    return (int)::SendMessageW(pThis->m_hWnd, CB_GETCOUNT, 0, 0);
}

// CComboBox::GetCurSel
extern "C" int MS_ABI impl__GetCurSel_CComboBox__QEBAHXZ(const CComboBox* pThis) {
    if (!pThis || !pThis->m_hWnd) return CB_ERR;
    return (int)::SendMessageW(pThis->m_hWnd, CB_GETCURSEL, 0, 0);
}

// CComboBox::SetCurSel
extern "C" int MS_ABI impl__SetCurSel_CComboBox__QEAAHH_Z(CComboBox* pThis, int nSelect) {
    if (!pThis || !pThis->m_hWnd) return CB_ERR;
    return (int)::SendMessageW(pThis->m_hWnd, CB_SETCURSEL, nSelect, 0);
}

// CComboBox::GetLBText
extern "C" int MS_ABI impl__GetLBText_CComboBox__QEBAHPEA_WH_Z(const CComboBox* pThis, int nIndex, wchar_t* lpszText) {
    if (!pThis || !pThis->m_hWnd || !lpszText) return CB_ERR;
    return (int)::SendMessageW(pThis->m_hWnd, CB_GETLBTEXT, nIndex, (LPARAM)lpszText);
}

// CComboBox::GetLBTextLen
extern "C" int MS_ABI impl__GetLBTextLen_CComboBox__QEBAHH_Z(const CComboBox* pThis, int nIndex) {
    if (!pThis || !pThis->m_hWnd) return CB_ERR;
    return (int)::SendMessageW(pThis->m_hWnd, CB_GETLBTEXTLEN, nIndex, 0);
}

// CComboBox::AddString
extern "C" int MS_ABI impl__AddString_CComboBox__QEAAHPEB_W_Z(CComboBox* pThis, const wchar_t* lpszString) {
    if (!pThis || !pThis->m_hWnd) return CB_ERR;
    return (int)::SendMessageW(pThis->m_hWnd, CB_ADDSTRING, 0, (LPARAM)lpszString);
}

// CComboBox::InsertString
extern "C" int MS_ABI impl__InsertString_CComboBox__QEAAHHPEB_W_Z(CComboBox* pThis, int nIndex, const wchar_t* lpszString) {
    if (!pThis || !pThis->m_hWnd) return CB_ERR;
    return (int)::SendMessageW(pThis->m_hWnd, CB_INSERTSTRING, nIndex, (LPARAM)lpszString);
}

// CComboBox::DeleteString
extern "C" int MS_ABI impl__DeleteString_CComboBox__QEAAHI_Z(CComboBox* pThis, unsigned int nIndex) {
    if (!pThis || !pThis->m_hWnd) return CB_ERR;
    return (int)::SendMessageW(pThis->m_hWnd, CB_DELETESTRING, nIndex, 0);
}

// CComboBox::ResetContent
extern "C" void MS_ABI impl__ResetContent_CComboBox__QEAAXXZ(CComboBox* pThis) {
    if (pThis && pThis->m_hWnd) {
        ::SendMessageW(pThis->m_hWnd, CB_RESETCONTENT, 0, 0);
    }
}

// CComboBox::FindString
extern "C" int MS_ABI impl__FindString_CComboBox__QEBAHPEB_WH_Z(const CComboBox* pThis, const wchar_t* lpszString, int nStartAfter) {
    if (!pThis || !pThis->m_hWnd) return CB_ERR;
    return (int)::SendMessageW(pThis->m_hWnd, CB_FINDSTRING, nStartAfter, (LPARAM)lpszString);
}

// CComboBox::SelectString
extern "C" int MS_ABI impl__SelectString_CComboBox__QEAAHPEB_WH_Z(CComboBox* pThis, int nStartAfter, const wchar_t* lpszString) {
    if (!pThis || !pThis->m_hWnd) return CB_ERR;
    return (int)::SendMessageW(pThis->m_hWnd, CB_SELECTSTRING, nStartAfter, (LPARAM)lpszString);
}

// CComboBox::GetItemData
extern "C" uintptr_t MS_ABI impl__GetItemData_CComboBox__QEBA_KH_Z(const CComboBox* pThis, int nIndex) {
    if (!pThis || !pThis->m_hWnd) return 0;
    return (uintptr_t)::SendMessageW(pThis->m_hWnd, CB_GETITEMDATA, nIndex, 0);
}

// CComboBox::SetItemData
extern "C" int MS_ABI impl__SetItemData_CComboBox__QEAAHH_K_Z(CComboBox* pThis, int nIndex, uintptr_t dwItemData) {
    if (!pThis || !pThis->m_hWnd) return CB_ERR;
    return (int)::SendMessageW(pThis->m_hWnd, CB_SETITEMDATA, nIndex, (LPARAM)dwItemData);
}

// CComboBox::ShowDropDown
extern "C" void MS_ABI impl__ShowDropDown_CComboBox__QEAAXH_Z(CComboBox* pThis, int bShowIt) {
    if (pThis && pThis->m_hWnd) {
        ::SendMessageW(pThis->m_hWnd, CB_SHOWDROPDOWN, bShowIt ? TRUE : FALSE, 0);
    }
}

// CComboBox::GetDroppedState
extern "C" int MS_ABI impl__GetDroppedState_CComboBox__QEBAHXZ(const CComboBox* pThis) {
    if (!pThis || !pThis->m_hWnd) return FALSE;
    return (int)::SendMessageW(pThis->m_hWnd, CB_GETDROPPEDSTATE, 0, 0);
}

void CComboBox::GetLBText(int nIndex, CString& rString) const {
    rString.Empty();
    if (!m_hWnd) return;
    int nLen = (int)::SendMessageW(m_hWnd, CB_GETLBTEXTLEN, nIndex, 0);
    if (nLen < 0) return;
    wchar_t* pBuf = rString.GetBuffer(nLen + 1);
    int nCopied = (int)::SendMessageW(m_hWnd, CB_GETLBTEXT, nIndex, (LPARAM)pBuf);
    rString.ReleaseBuffer(nCopied >= 0 ? nCopied : 0);
}

// Symbol: ?GetLBText@CComboBox@@QEBAXHAEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@@Z
extern "C" void MS_ABI impl__GetLBText_CComboBox__QEBAXHAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
    const CComboBox* pThis, int nIndex, CString* pText) {
    if (!pText) return;
    if (!pThis) {
        pText->Empty();
        return;
    }
    pThis->GetLBText(nIndex, *pText);
}

namespace {
using ItemStateMap = std::unordered_map<int, int>;
static std::unordered_map<const CCheckListBox*, ItemStateMap> g_checkStates;
static std::unordered_map<const CCheckListBox*, ItemStateMap> g_itemEnabledStates;
static std::unordered_map<const CCheckListBox*, unsigned int> g_checkStyles;
static std::unordered_map<const CSplitButton*, HMENU> g_splitMenus;
static std::unordered_map<const CDragListBox*, int> g_dragSourceItem;

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

int CDragListBox::BeginDrag(CPoint pt) {
    int nIndex = ListBoxItemFromPoint(m_hWnd, pt);
    if (nIndex == LB_ERR) nIndex = GetCurSel();
    if (nIndex == LB_ERR) return FALSE;
    g_dragSourceItem[this] = nIndex;
    return TRUE;
}

CDragListBox::~CDragListBox() {
    g_dragSourceItem.erase(this);
}

void CDragListBox::CancelDrag(CPoint) {
    g_dragSourceItem.erase(this);
}

unsigned int CDragListBox::Dragging(CPoint pt) {
    return (unsigned int)ListBoxItemFromPoint(m_hWnd, pt);
}

void CDragListBox::Dropped(int nSrcIndex, CPoint pt) {
    int nDst = ListBoxItemFromPoint(m_hWnd, pt);
    if (nDst == LB_ERR) nDst = nSrcIndex;
    if (nDst != LB_ERR) SetCurSel(nDst);
    g_dragSourceItem.erase(this);
}

void CDragListBox::DrawInsert(int nItem) {
    if (m_hWnd && nItem >= 0) ::SendMessageW(m_hWnd, LB_SETTOPINDEX, nItem, 0);
}

void CDragListBox::DrawSingle(int nItem) {
    if (!m_hWnd || nItem < 0) return;
    RECT rc = {};
    if (::SendMessageW(m_hWnd, LB_GETITEMRECT, nItem, (LPARAM)&rc) != LB_ERR) {
        ::InvalidateRect(m_hWnd, &rc, TRUE);
    }
}

int CSplitButton::Create(const wchar_t* lpszCaption, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, unsigned int nID) {
    return CButton::Create(lpszCaption, dwStyle | BS_SPLITBUTTON, rect, pParentWnd, nID);
}

CSplitButton::~CSplitButton() {
    g_splitMenus.erase(this);
}

void CSplitButton::SetDropDownMenu(unsigned int nMenuId, unsigned int) {
    g_splitMenus[this] = ::LoadMenuW(::GetModuleHandleW(nullptr), MAKEINTRESOURCEW(nMenuId));
}

void CSplitButton::SetDropDownMenu(CMenu* pMenu) {
    g_splitMenus[this] = pMenu ? pMenu->m_hMenu : nullptr;
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

// Symbol: ?Create@CSplitButton@@UEAAHPEB_WKAEBUtagRECT@@PEAVCWnd@@I@Z
extern "C" int MS_ABI impl__Create_CSplitButton__UEAAHPEB_WKAEBUtagRECT__PEAVCWnd__I_Z(
    CSplitButton* pThis, const wchar_t* lpszCaption, DWORD dwStyle, const RECT* pRect, CWnd* pParentWnd, UINT nID) {
    return (pThis && pRect) ? pThis->Create(lpszCaption, dwStyle, *pRect, pParentWnd, nID) : FALSE;
}

// Symbol: ?SetDropDownMenu@CSplitButton@@QEAAXII@Z
extern "C" void MS_ABI impl__SetDropDownMenu_CSplitButton__QEAAXII_Z(CSplitButton* pThis, unsigned int nMenuId, unsigned int nSubMenu) {
    if (pThis) pThis->SetDropDownMenu(nMenuId, nSubMenu);
}

// Symbol: ?SetDropDownMenu@CSplitButton@@QEAAXPEAVCMenu@@@Z
extern "C" void MS_ABI impl__SetDropDownMenu_CSplitButton__QEAAXPEAVCMenu___Z(CSplitButton* pThis, CMenu* pMenu) {
    if (pThis) pThis->SetDropDownMenu(pMenu);
}

// Symbol: ?Cleanup@CSplitButton@@IEAAXXZ
extern "C" void MS_ABI impl__Cleanup_CSplitButton__IEAAXXZ(CSplitButton* pThis) {
    if (!pThis) return;
    g_splitMenus.erase(pThis);
}

// Symbol: ??1CSplitButton@@UEAA@XZ
extern "C" void MS_ABI impl___1CSplitButton__UEAA_XZ(CSplitButton* pThis) {
    if (pThis) pThis->~CSplitButton();
}

// Symbol: ?BeginDrag@CDragListBox@@UEAAHVCPoint@@@Z
extern "C" int MS_ABI impl__BeginDrag_CDragListBox__UEAAHVCPoint___Z(CDragListBox* pThis, CPoint pt) {
    return pThis ? pThis->BeginDrag(pt) : FALSE;
}

// Symbol: ?CancelDrag@CDragListBox@@UEAAXVCPoint@@@Z
extern "C" void MS_ABI impl__CancelDrag_CDragListBox__UEAAXVCPoint___Z(CDragListBox* pThis, CPoint pt) {
    if (pThis) pThis->CancelDrag(pt);
}

// Symbol: ?Dragging@CDragListBox@@UEAAIVCPoint@@@Z
extern "C" unsigned int MS_ABI impl__Dragging_CDragListBox__UEAAIVCPoint___Z(CDragListBox* pThis, CPoint pt) {
    return pThis ? pThis->Dragging(pt) : (unsigned int)LB_ERR;
}

// Symbol: ?Dropped@CDragListBox@@UEAAXHVCPoint@@@Z
extern "C" void MS_ABI impl__Dropped_CDragListBox__UEAAXHVCPoint___Z(CDragListBox* pThis, int nSrcIndex, CPoint pt) {
    if (pThis) pThis->Dropped(nSrcIndex, pt);
}

// Symbol: ?DrawInsert@CDragListBox@@UEAAXH@Z
extern "C" void MS_ABI impl__DrawInsert_CDragListBox__UEAAXH_Z(CDragListBox* pThis, int nItem) {
    if (pThis) pThis->DrawInsert(nItem);
}

// Symbol: ?DrawSingle@CDragListBox@@QEAAXH@Z
extern "C" void MS_ABI impl__DrawSingle_CDragListBox__QEAAXH_Z(CDragListBox* pThis, int nItem) {
    if (pThis) pThis->DrawSingle(nItem);
}

// Symbol: ??1CDragListBox@@UEAA@XZ
extern "C" void MS_ABI impl___1CDragListBox__UEAA_XZ(CDragListBox* pThis) {
    if (pThis) pThis->~CDragListBox();
}

// =============================================================================
// CScrollBar Implementation
// =============================================================================

extern "C" int MS_ABI impl__Create_CScrollBar__UEAAHKAEBUtagRECT__PEAVCWnd__I_Z(
    CScrollBar* pThis, DWORD dwStyle, const RECT* pRect, CWnd* pParentWnd, UINT nID) {
    if (!pThis || !pRect) return FALSE;
    return impl__Create_CWnd__UEAAHPEB_W0KAEBUtagRECT__PEAV1_IPEAUCCreateContext___Z(
        pThis, L"SCROLLBAR", L"", dwStyle, *pRect, pParentWnd, nID, nullptr);
}

// CScrollBar::GetScrollPos
extern "C" int MS_ABI impl__GetScrollPos_CScrollBar__QEBAHXZ(const CScrollBar* pThis) {
    if (!pThis || !pThis->m_hWnd) return 0;
    return ::GetScrollPos(pThis->m_hWnd, SB_CTL);
}

// CScrollBar::SetScrollPos
extern "C" int MS_ABI impl__SetScrollPos_CScrollBar__QEAAHHH_Z(CScrollBar* pThis, int nPos, int bRedraw) {
    if (!pThis || !pThis->m_hWnd) return 0;
    return ::SetScrollPos(pThis->m_hWnd, SB_CTL, nPos, bRedraw ? TRUE : FALSE);
}

// CScrollBar::GetScrollRange
extern "C" void MS_ABI impl__GetScrollRange_CScrollBar__QEBAXPEAH0_Z(const CScrollBar* pThis, int* lpMinPos, int* lpMaxPos) {
    if (!pThis || !pThis->m_hWnd) {
        if (lpMinPos) *lpMinPos = 0;
        if (lpMaxPos) *lpMaxPos = 0;
        return;
    }
    ::GetScrollRange(pThis->m_hWnd, SB_CTL, lpMinPos, lpMaxPos);
}

// CScrollBar::SetScrollRange
extern "C" void MS_ABI impl__SetScrollRange_CScrollBar__QEAAXHHH_Z(CScrollBar* pThis, int nMinPos, int nMaxPos, int bRedraw) {
    if (pThis && pThis->m_hWnd) {
        ::SetScrollRange(pThis->m_hWnd, SB_CTL, nMinPos, nMaxPos, bRedraw ? TRUE : FALSE);
    }
}

// CScrollBar::GetScrollInfo
extern "C" int MS_ABI impl__GetScrollInfo_CScrollBar__QEBAHPEAUSCROLLINFO__I_Z(const CScrollBar* pThis, SCROLLINFO* lpScrollInfo, unsigned int nMask) {
    if (!pThis || !pThis->m_hWnd || !lpScrollInfo) return FALSE;
    lpScrollInfo->cbSize = sizeof(SCROLLINFO);
    lpScrollInfo->fMask = nMask;
    return ::GetScrollInfo(pThis->m_hWnd, SB_CTL, lpScrollInfo);
}

// CScrollBar::SetScrollInfo
extern "C" int MS_ABI impl__SetScrollInfo_CScrollBar__QEAAHPEAUSCROLLINFO__H_Z(CScrollBar* pThis, SCROLLINFO* lpScrollInfo, int bRedraw) {
    if (!pThis || !pThis->m_hWnd || !lpScrollInfo) return 0;
    lpScrollInfo->cbSize = sizeof(SCROLLINFO);
    return ::SetScrollInfo(pThis->m_hWnd, SB_CTL, lpScrollInfo, bRedraw ? TRUE : FALSE);
}

// CScrollBar::EnableScrollBar
extern "C" int MS_ABI impl__EnableScrollBar_CScrollBar__QEAAHI_Z(CScrollBar* pThis, unsigned int nArrowFlags) {
    if (!pThis || !pThis->m_hWnd) return FALSE;
    return ::EnableScrollBar(pThis->m_hWnd, SB_CTL, nArrowFlags);
}

extern "C" int MS_ABI impl__Create_CSliderCtrl__UEAAHKAEBUtagRECT__PEAVCWnd__I_Z(
    CSliderCtrl* pThis, DWORD dwStyle, const RECT* pRect, CWnd* pParentWnd, UINT nID) {
    if (!pThis || !pRect) return FALSE;
    return impl__Create_CWnd__UEAAHPEB_W0KAEBUtagRECT__PEAV1_IPEAUCCreateContext___Z(
        pThis, L"msctls_trackbar32", L"", dwStyle, *pRect, pParentWnd, nID, nullptr);
}

extern "C" int MS_ABI impl__Create_CProgressCtrl__UEAAHKAEBUtagRECT__PEAVCWnd__I_Z(
    CProgressCtrl* pThis, DWORD dwStyle, const RECT* pRect, CWnd* pParentWnd, UINT nID) {
    if (!pThis || !pRect) return FALSE;
    return impl__Create_CWnd__UEAAHPEB_W0KAEBUtagRECT__PEAV1_IPEAUCCreateContext___Z(
        pThis, L"msctls_progress32", L"", dwStyle, *pRect, pParentWnd, nID, nullptr);
}

extern "C" int MS_ABI impl__Create_CSpinButtonCtrl__UEAAHKAEBUtagRECT__PEAVCWnd__I_Z(
    CSpinButtonCtrl* pThis, DWORD dwStyle, const RECT* pRect, CWnd* pParentWnd, UINT nID) {
    if (!pThis || !pRect) return FALSE;
    return impl__Create_CWnd__UEAAHPEB_W0KAEBUtagRECT__PEAV1_IPEAUCCreateContext___Z(
        pThis, L"msctls_updown32", L"", dwStyle, *pRect, pParentWnd, nID, nullptr);
}

extern "C" int MS_ABI impl__Create_CListCtrl__QEAAHKAEBUtagRECT__PEAVCWnd__I_Z(
    CListCtrl* pThis, DWORD dwStyle, const RECT* pRect, CWnd* pParentWnd, UINT nID) {
    if (!pThis || !pRect) return FALSE;
    return impl__Create_CWnd__UEAAHPEB_W0KAEBUtagRECT__PEAV1_IPEAUCCreateContext___Z(
        pThis, L"SysListView32", L"", dwStyle, *pRect, pParentWnd, nID, nullptr);
}

extern "C" int MS_ABI impl__Create_CTreeCtrl__QEAAHKAEBUtagRECT__PEAVCWnd__I_Z(
    CTreeCtrl* pThis, DWORD dwStyle, const RECT* pRect, CWnd* pParentWnd, UINT nID) {
    if (!pThis || !pRect) return FALSE;
    return impl__Create_CWnd__UEAAHPEB_W0KAEBUtagRECT__PEAV1_IPEAUCCreateContext___Z(
        pThis, L"SysTreeView32", L"", dwStyle, *pRect, pParentWnd, nID, nullptr);
}

extern "C" int MS_ABI impl__Create_CTabCtrl__UEAAHKAEBUtagRECT__PEAVCWnd__I_Z(
    CTabCtrl* pThis, DWORD dwStyle, const RECT* pRect, CWnd* pParentWnd, UINT nID) {
    if (!pThis || !pRect) return FALSE;
    return impl__Create_CWnd__UEAAHPEB_W0KAEBUtagRECT__PEAV1_IPEAUCCreateContext___Z(
        pThis, L"SysTabControl32", L"", dwStyle, *pRect, pParentWnd, nID, nullptr);
}

// Symbol: ?CreateEx@CSliderCtrl@@UEAAHKKAEBUtagRECT@@PEAVCWnd@@I@Z
extern "C" int MS_ABI impl__CreateEx_CSliderCtrl__UEAAHKKAEBUtagRECT__PEAVCWnd__I_Z(
    CSliderCtrl* pThis, DWORD dwExStyle, DWORD dwStyle, const RECT* pRect, CWnd* pParentWnd, UINT nID) {
    if (!pThis || !pRect) return FALSE;
    return pThis->CreateEx(dwExStyle, L"msctls_trackbar32", L"", dwStyle, pRect->left, pRect->top,
                           pRect->right - pRect->left, pRect->bottom - pRect->top,
                           pParentWnd ? pParentWnd->GetSafeHwnd() : nullptr, (HMENU)(uintptr_t)nID, nullptr);
}

// Symbol: ?CreateEx@CProgressCtrl@@UEAAHKKAEBUtagRECT@@PEAVCWnd@@I@Z
extern "C" int MS_ABI impl__CreateEx_CProgressCtrl__UEAAHKKAEBUtagRECT__PEAVCWnd__I_Z(
    CProgressCtrl* pThis, DWORD dwExStyle, DWORD dwStyle, const RECT* pRect, CWnd* pParentWnd, UINT nID) {
    if (!pThis || !pRect) return FALSE;
    return pThis->CreateEx(dwExStyle, L"msctls_progress32", L"", dwStyle, pRect->left, pRect->top,
                           pRect->right - pRect->left, pRect->bottom - pRect->top,
                           pParentWnd ? pParentWnd->GetSafeHwnd() : nullptr, (HMENU)(uintptr_t)nID, nullptr);
}

// Symbol: ?CreateEx@CSpinButtonCtrl@@UEAAHKKAEBUtagRECT@@PEAVCWnd@@I@Z
extern "C" int MS_ABI impl__CreateEx_CSpinButtonCtrl__UEAAHKKAEBUtagRECT__PEAVCWnd__I_Z(
    CSpinButtonCtrl* pThis, DWORD dwExStyle, DWORD dwStyle, const RECT* pRect, CWnd* pParentWnd, UINT nID) {
    if (!pThis || !pRect) return FALSE;
    return pThis->CreateEx(dwExStyle, L"msctls_updown32", L"", dwStyle, pRect->left, pRect->top,
                           pRect->right - pRect->left, pRect->bottom - pRect->top,
                           pParentWnd ? pParentWnd->GetSafeHwnd() : nullptr, (HMENU)(uintptr_t)nID, nullptr);
}

// Symbol: ?CreateEx@CTabCtrl@@UEAAHKKAEBUtagRECT@@PEAVCWnd@@I@Z
extern "C" int MS_ABI impl__CreateEx_CTabCtrl__UEAAHKKAEBUtagRECT__PEAVCWnd__I_Z(
    CTabCtrl* pThis, DWORD dwExStyle, DWORD dwStyle, const RECT* pRect, CWnd* pParentWnd, UINT nID) {
    if (!pThis || !pRect) return FALSE;
    return pThis->CreateEx(dwExStyle, L"SysTabControl32", L"", dwStyle, pRect->left, pRect->top,
                           pRect->right - pRect->left, pRect->bottom - pRect->top,
                           pParentWnd ? pParentWnd->GetSafeHwnd() : nullptr, (HMENU)(uintptr_t)nID, nullptr);
}

// Symbol: ?GetRange@CProgressCtrl@@QEBAXAEAH0@Z
extern "C" void MS_ABI impl__GetRange_CProgressCtrl__QEBAXAEAH0_Z(
    const CProgressCtrl* pThis, int* pLower, int* pUpper) {
    if (pLower) *pLower = 0;
    if (pUpper) *pUpper = 0;
    if (!pThis || !pThis->m_hWnd) return;

    PBRANGE range = {};
    ::SendMessageW(pThis->m_hWnd, PBM_GETRANGE, TRUE, (LPARAM)&range);
    if (pLower) *pLower = range.iLow;
    if (pUpper) *pUpper = range.iHigh;
}

// Symbol: ?GetRange@CSliderCtrl@@QEBAXAEAH0@Z
extern "C" void MS_ABI impl__GetRange_CSliderCtrl__QEBAXAEAH0_Z(
    const CSliderCtrl* pThis, int* pMin, int* pMax) {
    if (pMin) *pMin = 0;
    if (pMax) *pMax = 0;
    if (!pThis || !pThis->m_hWnd) return;
    if (pMin) *pMin = (int)::SendMessageW(pThis->m_hWnd, TBM_GETRANGEMIN, 0, 0);
    if (pMax) *pMax = (int)::SendMessageW(pThis->m_hWnd, TBM_GETRANGEMAX, 0, 0);
}

// Symbol: ?SetRange@CSliderCtrl@@QEAAXHHH@Z
extern "C" void MS_ABI impl__SetRange_CSliderCtrl__QEAAXHHH_Z(
    CSliderCtrl* pThis, int nMin, int nMax, int bRedraw) {
    if (!pThis || !pThis->m_hWnd) return;
    ::SendMessageW(pThis->m_hWnd, TBM_SETRANGE, bRedraw ? TRUE : FALSE, MAKELPARAM(nMin, nMax));
}

// Symbol: ?GetSelection@CSliderCtrl@@QEBAXAEAH0@Z
extern "C" void MS_ABI impl__GetSelection_CSliderCtrl__QEBAXAEAH0_Z(
    const CSliderCtrl* pThis, int* pMinSel, int* pMaxSel) {
    if (pMinSel) *pMinSel = 0;
    if (pMaxSel) *pMaxSel = 0;
    if (!pThis || !pThis->m_hWnd) return;
    if (pMinSel) *pMinSel = (int)::SendMessageW(pThis->m_hWnd, TBM_GETSELSTART, 0, 0);
    if (pMaxSel) *pMaxSel = (int)::SendMessageW(pThis->m_hWnd, TBM_GETSELEND, 0, 0);
}

// Symbol: ?SetSelection@CSliderCtrl@@QEAAXHH@Z
extern "C" void MS_ABI impl__SetSelection_CSliderCtrl__QEAAXHH_Z(
    CSliderCtrl* pThis, int nMinSel, int nMaxSel) {
    if (!pThis || !pThis->m_hWnd) return;
    ::SendMessageW(pThis->m_hWnd, TBM_SETSELSTART, FALSE, nMinSel);
    ::SendMessageW(pThis->m_hWnd, TBM_SETSELEND, FALSE, nMaxSel);
}

// Symbol: ?GetRange@CSpinButtonCtrl@@QEBAXAEAH0@Z
extern "C" void MS_ABI impl__GetRange_CSpinButtonCtrl__QEBAXAEAH0_Z(
    const CSpinButtonCtrl* pThis, int* pLower, int* pUpper) {
    if (pLower) *pLower = 0;
    if (pUpper) *pUpper = 100;
    if (!pThis || !pThis->m_hWnd) return;

    DWORD dwRange = (DWORD)::SendMessageW(pThis->m_hWnd, UDM_GETRANGE, 0, 0);
    if (pLower) *pLower = (short)HIWORD(dwRange);
    if (pUpper) *pUpper = (short)LOWORD(dwRange);
}

// Symbol: ?AddGroup@CMFCTasksPane@@QEAAHHPEB_WHHPEAUHICON__@@@Z
extern "C" int MS_ABI impl__AddGroup_CMFCTasksPane__QEAAHHPEB_WHHPEAUHICON_____Z(
    CMFCTasksPane* pThis, int nGroup, const wchar_t* lpszName, int bBottomHasGripper, int bSpecial, void* hIcon) {
    return pThis ? pThis->AddGroup(nGroup, lpszName, bBottomHasGripper ? TRUE : FALSE, bSpecial ? TRUE : FALSE,
                                     reinterpret_cast<HICON>(hIcon))
                 : -1;
}

// Symbol: ?RemoveAllTasks@CMFCTasksPane@@QEAAXH@Z
extern "C" void MS_ABI impl__RemoveAllTasks_CMFCTasksPane__QEAAXH_Z(CMFCTasksPane* pThis, int nGroup) {
    if (pThis) pThis->RemoveAllTasks(nGroup);
}

// Symbol: ?SetCaption@CMFCTasksPane@@QEAAXPEB_W@Z
extern "C" void MS_ABI impl__SetCaption_CMFCTasksPane__QEAAXPEB_W_Z(CMFCTasksPane* pThis, const wchar_t* lpszCaption) {
    if (pThis) pThis->SetCaption(lpszCaption);
}

// Symbol: ?Show@CMFCPropertyGridProperty@@QEAAXHH@Z
extern "C" void MS_ABI impl__Show_CMFCPropertyGridProperty__QEAAXHH_Z(
    CMFCPropertyGridProperty* pThis, int bShow, int bAdjustLayout) {
    if (!pThis) {
        return;
    }

    pThis->Show(bShow ? TRUE : FALSE, bAdjustLayout ? TRUE : FALSE);
}
