// Control Core Implementation
//
// Implements CButton, CEdit, CStatic, CListBox, CComboBox, and CScrollBar.
// These wrap Windows common controls using standard Windows messages.

#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxwin.h"
#include <windows.h>

// MS ABI calling convention
#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

// =============================================================================
// CRuntimeClass implementations for control classes
// =============================================================================

IMPLEMENT_DYNAMIC(CButton, CWnd)
IMPLEMENT_DYNAMIC(CEdit, CWnd)
IMPLEMENT_DYNAMIC(CStatic, CWnd)
IMPLEMENT_DYNAMIC(CListBox, CWnd)
IMPLEMENT_DYNAMIC(CComboBox, CWnd)
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

// MSVC symbol aliases for CScrollBar methods
asm(".globl \"?GetScrollPos@CScrollBar@@QEBAHXZ\"\n"
    ".set \"?GetScrollPos@CScrollBar@@QEBAHXZ\", impl__GetScrollPos_CScrollBar__QEBAHXZ\n");
asm(".globl \"?SetScrollPos@CScrollBar@@QEAAHHH@Z\"\n"
    ".set \"?SetScrollPos@CScrollBar@@QEAAHHH@Z\", impl__SetScrollPos_CScrollBar__QEAAHHH_Z\n");
#endif

// =============================================================================
// CButton Implementation
// =============================================================================

// CButton::GetState
// Symbol: ?GetState@CButton@@QEBAIXZ
extern "C" unsigned int MS_ABI impl__GetState_CButton__QEBAIXZ(const CButton* pThis) {
    if (!pThis || !pThis->m_hWnd) return 0;
    return (unsigned int)::SendMessageW(pThis->m_hWnd, BM_GETSTATE, 0, 0);
}

// CButton::SetState
// Symbol: ?SetState@CButton@@QEAAXH@Z
extern "C" void MS_ABI impl__SetState_CButton__QEAAXH_Z(CButton* pThis, int bHighlight) {
    if (pThis && pThis->m_hWnd) {
        ::SendMessageW(pThis->m_hWnd, BM_SETSTATE, bHighlight ? TRUE : FALSE, 0);
    }
}

// CButton::GetCheck
// Symbol: ?GetCheck@CButton@@QEBAHXZ
extern "C" int MS_ABI impl__GetCheck_CButton__QEBAHXZ(const CButton* pThis) {
    if (!pThis || !pThis->m_hWnd) return 0;
    return (int)::SendMessageW(pThis->m_hWnd, BM_GETCHECK, 0, 0);
}

// CButton::SetCheck
// Symbol: ?SetCheck@CButton@@QEAAXH@Z
extern "C" void MS_ABI impl__SetCheck_CButton__QEAAXH_Z(CButton* pThis, int nCheck) {
    if (pThis && pThis->m_hWnd) {
        ::SendMessageW(pThis->m_hWnd, BM_SETCHECK, nCheck, 0);
    }
}

// CButton::GetButtonStyle
// Symbol: ?GetButtonStyle@CButton@@QEBAIXZ
extern "C" unsigned int MS_ABI impl__GetButtonStyle_CButton__QEBAIXZ(const CButton* pThis) {
    if (!pThis || !pThis->m_hWnd) return 0;
    return (unsigned int)(::GetWindowLongW(pThis->m_hWnd, GWL_STYLE) & 0xFFFF);
}

// CButton::SetButtonStyle
// Symbol: ?SetButtonStyle@CButton@@QEAAXIH@Z
extern "C" void MS_ABI impl__SetButtonStyle_CButton__QEAAXIH_Z(CButton* pThis, unsigned int nStyle, int bRedraw) {
    if (pThis && pThis->m_hWnd) {
        ::SendMessageW(pThis->m_hWnd, BM_SETSTYLE, nStyle, MAKELPARAM(bRedraw, 0));
    }
}

// CButton::GetBitmap
// Symbol: ?GetBitmap@CButton@@QEBAPEAXXZ
extern "C" void* MS_ABI impl__GetBitmap_CButton__QEBAPEAXXZ(const CButton* pThis) {
    if (!pThis || !pThis->m_hWnd) return nullptr;
    return (void*)::SendMessageW(pThis->m_hWnd, BM_GETIMAGE, IMAGE_BITMAP, 0);
}

// CButton::SetBitmap
// Symbol: ?SetBitmap@CButton@@QEAAPEAXPEAX@Z
extern "C" void* MS_ABI impl__SetBitmap_CButton__QEAAPEAXPEAX_Z(CButton* pThis, void* hBitmap) {
    if (!pThis || !pThis->m_hWnd) return nullptr;
    return (void*)::SendMessageW(pThis->m_hWnd, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hBitmap);
}

// CButton::GetIcon
// Symbol: ?GetIcon@CButton@@QEBAPEAXXZ
extern "C" void* MS_ABI impl__GetIcon_CButton__QEBAPEAXXZ(const CButton* pThis) {
    if (!pThis || !pThis->m_hWnd) return nullptr;
    return (void*)::SendMessageW(pThis->m_hWnd, BM_GETIMAGE, IMAGE_ICON, 0);
}

// CButton::SetIcon
// Symbol: ?SetIcon@CButton@@QEAAPEAXPEAX@Z
extern "C" void* MS_ABI impl__SetIcon_CButton__QEAAPEAXPEAX_Z(CButton* pThis, void* hIcon) {
    if (!pThis || !pThis->m_hWnd) return nullptr;
    return (void*)::SendMessageW(pThis->m_hWnd, BM_SETIMAGE, IMAGE_ICON, (LPARAM)hIcon);
}

// =============================================================================
// CEdit Implementation
// =============================================================================

// CEdit::GetLineCount
// Symbol: ?GetLineCount@CEdit@@QEBAHXZ
extern "C" int MS_ABI impl__GetLineCount_CEdit__QEBAHXZ(const CEdit* pThis) {
    if (!pThis || !pThis->m_hWnd) return 1;
    return (int)::SendMessageW(pThis->m_hWnd, EM_GETLINECOUNT, 0, 0);
}

// CEdit::GetLine
// Symbol: ?GetLine@CEdit@@QEBAHPEA_WH@Z
extern "C" int MS_ABI impl__GetLine_CEdit__QEBAHPEA_WH_Z(const CEdit* pThis, int nIndex, wchar_t* lpszBuffer, int nMaxLength) {
    if (!pThis || !pThis->m_hWnd || !lpszBuffer || nMaxLength <= 0) return 0;
    // First word of buffer must contain the max length
    *(WORD*)lpszBuffer = (WORD)nMaxLength;
    return (int)::SendMessageW(pThis->m_hWnd, EM_GETLINE, nIndex, (LPARAM)lpszBuffer);
}

// CEdit::GetSel
// Symbol: ?GetSel@CEdit@@QEBAXAEAH0@Z
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
// Symbol: ?SetSel@CEdit@@QEAAXHHH@Z
extern "C" void MS_ABI impl__SetSel_CEdit__QEAAXHHH_Z(CEdit* pThis, int nStartChar, int nEndChar, int bNoScroll) {
    if (pThis && pThis->m_hWnd) {
        ::SendMessageW(pThis->m_hWnd, EM_SETSEL, nStartChar, nEndChar);
        if (!bNoScroll) {
            ::SendMessageW(pThis->m_hWnd, EM_SCROLLCARET, 0, 0);
        }
    }
}

// CEdit::ReplaceSel
// Symbol: ?ReplaceSel@CEdit@@QEAAXPEB_WH@Z
extern "C" void MS_ABI impl__ReplaceSel_CEdit__QEAAXPEB_WH_Z(CEdit* pThis, const wchar_t* lpszNewText, int bCanUndo) {
    if (pThis && pThis->m_hWnd) {
        ::SendMessageW(pThis->m_hWnd, EM_REPLACESEL, bCanUndo ? TRUE : FALSE, (LPARAM)lpszNewText);
    }
}

// CEdit::Clear
// Symbol: ?Clear@CEdit@@QEAAXXZ
extern "C" void MS_ABI impl__Clear_CEdit__QEAAXXZ(CEdit* pThis) {
    if (pThis && pThis->m_hWnd) {
        ::SendMessageW(pThis->m_hWnd, WM_CLEAR, 0, 0);
    }
}

// CEdit::Copy
// Symbol: ?Copy@CEdit@@QEAAXXZ
extern "C" void MS_ABI impl__Copy_CEdit__QEAAXXZ(CEdit* pThis) {
    if (pThis && pThis->m_hWnd) {
        ::SendMessageW(pThis->m_hWnd, WM_COPY, 0, 0);
    }
}

// CEdit::Cut
// Symbol: ?Cut@CEdit@@QEAAXXZ
extern "C" void MS_ABI impl__Cut_CEdit__QEAAXXZ(CEdit* pThis) {
    if (pThis && pThis->m_hWnd) {
        ::SendMessageW(pThis->m_hWnd, WM_CUT, 0, 0);
    }
}

// CEdit::Paste
// Symbol: ?Paste@CEdit@@QEAAXXZ
extern "C" void MS_ABI impl__Paste_CEdit__QEAAXXZ(CEdit* pThis) {
    if (pThis && pThis->m_hWnd) {
        ::SendMessageW(pThis->m_hWnd, WM_PASTE, 0, 0);
    }
}

// CEdit::Undo
// Symbol: ?Undo@CEdit@@QEAAHXZ
extern "C" int MS_ABI impl__Undo_CEdit__QEAAHXZ(CEdit* pThis) {
    if (!pThis || !pThis->m_hWnd) return FALSE;
    return (int)::SendMessageW(pThis->m_hWnd, EM_UNDO, 0, 0);
}

// CEdit::SetReadOnly
// Symbol: ?SetReadOnly@CEdit@@QEAAXH@Z
extern "C" void MS_ABI impl__SetReadOnly_CEdit__QEAAXH_Z(CEdit* pThis, int bReadOnly) {
    if (pThis && pThis->m_hWnd) {
        ::SendMessageW(pThis->m_hWnd, EM_SETREADONLY, bReadOnly ? TRUE : FALSE, 0);
    }
}

// CEdit::GetModify
// Symbol: ?GetModify@CEdit@@QEBAHXZ
extern "C" int MS_ABI impl__GetModify_CEdit__QEBAHXZ(const CEdit* pThis) {
    if (!pThis || !pThis->m_hWnd) return FALSE;
    return (int)::SendMessageW(pThis->m_hWnd, EM_GETMODIFY, 0, 0);
}

// CEdit::SetModify
// Symbol: ?SetModify@CEdit@@QEAAXH@Z
extern "C" void MS_ABI impl__SetModify_CEdit__QEAAXH_Z(CEdit* pThis, int bModified) {
    if (pThis && pThis->m_hWnd) {
        ::SendMessageW(pThis->m_hWnd, EM_SETMODIFY, bModified ? TRUE : FALSE, 0);
    }
}

// CEdit::LimitText
// Symbol: ?LimitText@CEdit@@QEAAXH@Z
extern "C" void MS_ABI impl__LimitText_CEdit__QEAAXH_Z(CEdit* pThis, int nChars) {
    if (pThis && pThis->m_hWnd) {
        ::SendMessageW(pThis->m_hWnd, EM_LIMITTEXT, nChars, 0);
    }
}

// CEdit::GetLimitText
// Symbol: ?GetLimitText@CEdit@@QEBAHXZ
extern "C" int MS_ABI impl__GetLimitText_CEdit__QEBAHXZ(const CEdit* pThis) {
    if (!pThis || !pThis->m_hWnd) return 0;
    return (int)::SendMessageW(pThis->m_hWnd, EM_GETLIMITTEXT, 0, 0);
}

// CEdit::SetPasswordChar
// Symbol: ?SetPasswordChar@CEdit@@QEAAX_W@Z
extern "C" void MS_ABI impl__SetPasswordChar_CEdit__QEAAX_W_Z(CEdit* pThis, wchar_t ch) {
    if (pThis && pThis->m_hWnd) {
        ::SendMessageW(pThis->m_hWnd, EM_SETPASSWORDCHAR, ch, 0);
    }
}

// =============================================================================
// CStatic Implementation
// =============================================================================

// CStatic::GetBitmap
// Symbol: ?GetBitmap@CStatic@@QEBAPEAXXZ
extern "C" void* MS_ABI impl__GetBitmap_CStatic__QEBAPEAXXZ(const CStatic* pThis) {
    if (!pThis || !pThis->m_hWnd) return nullptr;
    return (void*)::SendMessageW(pThis->m_hWnd, STM_GETIMAGE, IMAGE_BITMAP, 0);
}

// CStatic::SetBitmap
// Symbol: ?SetBitmap@CStatic@@QEAAPEAXPEAX@Z
extern "C" void* MS_ABI impl__SetBitmap_CStatic__QEAAPEAXPEAX_Z(CStatic* pThis, void* hBitmap) {
    if (!pThis || !pThis->m_hWnd) return nullptr;
    return (void*)::SendMessageW(pThis->m_hWnd, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hBitmap);
}

// CStatic::GetIcon
// Symbol: ?GetIcon@CStatic@@QEBAPEAXXZ
extern "C" void* MS_ABI impl__GetIcon_CStatic__QEBAPEAXXZ(const CStatic* pThis) {
    if (!pThis || !pThis->m_hWnd) return nullptr;
    return (void*)::SendMessageW(pThis->m_hWnd, STM_GETICON, 0, 0);
}

// CStatic::SetIcon
// Symbol: ?SetIcon@CStatic@@QEAAPEAXPEAX@Z
extern "C" void* MS_ABI impl__SetIcon_CStatic__QEAAPEAXPEAX_Z(CStatic* pThis, void* hIcon) {
    if (!pThis || !pThis->m_hWnd) return nullptr;
    return (void*)::SendMessageW(pThis->m_hWnd, STM_SETICON, (WPARAM)hIcon, 0);
}

// CStatic::GetEnhMetaFile
// Symbol: ?GetEnhMetaFile@CStatic@@QEBAPEAXXZ
extern "C" void* MS_ABI impl__GetEnhMetaFile_CStatic__QEBAPEAXXZ(const CStatic* pThis) {
    if (!pThis || !pThis->m_hWnd) return nullptr;
    return (void*)::SendMessageW(pThis->m_hWnd, STM_GETIMAGE, IMAGE_ENHMETAFILE, 0);
}

// CStatic::SetEnhMetaFile
// Symbol: ?SetEnhMetaFile@CStatic@@QEAAPEAXPEAX@Z
extern "C" void* MS_ABI impl__SetEnhMetaFile_CStatic__QEAAPEAXPEAX_Z(CStatic* pThis, void* hMetaFile) {
    if (!pThis || !pThis->m_hWnd) return nullptr;
    return (void*)::SendMessageW(pThis->m_hWnd, STM_SETIMAGE, IMAGE_ENHMETAFILE, (LPARAM)hMetaFile);
}

// =============================================================================
// CListBox Implementation
// =============================================================================

// CListBox::GetCount
// Symbol: ?GetCount@CListBox@@QEBAHXZ
extern "C" int MS_ABI impl__GetCount_CListBox__QEBAHXZ(const CListBox* pThis) {
    if (!pThis || !pThis->m_hWnd) return 0;
    return (int)::SendMessageW(pThis->m_hWnd, LB_GETCOUNT, 0, 0);
}

// CListBox::GetCurSel
// Symbol: ?GetCurSel@CListBox@@QEBAHXZ
extern "C" int MS_ABI impl__GetCurSel_CListBox__QEBAHXZ(const CListBox* pThis) {
    if (!pThis || !pThis->m_hWnd) return LB_ERR;
    return (int)::SendMessageW(pThis->m_hWnd, LB_GETCURSEL, 0, 0);
}

// CListBox::SetCurSel
// Symbol: ?SetCurSel@CListBox@@QEAAHH@Z
extern "C" int MS_ABI impl__SetCurSel_CListBox__QEAAHH_Z(CListBox* pThis, int nSelect) {
    if (!pThis || !pThis->m_hWnd) return LB_ERR;
    return (int)::SendMessageW(pThis->m_hWnd, LB_SETCURSEL, nSelect, 0);
}

// CListBox::GetText
// Symbol: ?GetText@CListBox@@QEBAHPEA_WH@Z
extern "C" int MS_ABI impl__GetText_CListBox__QEBAHPEA_WH_Z(const CListBox* pThis, int nIndex, wchar_t* lpszBuffer) {
    if (!pThis || !pThis->m_hWnd || !lpszBuffer) return LB_ERR;
    return (int)::SendMessageW(pThis->m_hWnd, LB_GETTEXT, nIndex, (LPARAM)lpszBuffer);
}

// CListBox::GetTextLen
// Symbol: ?GetTextLen@CListBox@@QEBAHH@Z
extern "C" int MS_ABI impl__GetTextLen_CListBox__QEBAHH_Z(const CListBox* pThis, int nIndex) {
    if (!pThis || !pThis->m_hWnd) return LB_ERR;
    return (int)::SendMessageW(pThis->m_hWnd, LB_GETTEXTLEN, nIndex, 0);
}

// CListBox::AddString
// Symbol: ?AddString@CListBox@@QEAAHPEB_W@Z
extern "C" int MS_ABI impl__AddString_CListBox__QEAAHPEB_W_Z(CListBox* pThis, const wchar_t* lpszItem) {
    if (!pThis || !pThis->m_hWnd) return LB_ERR;
    return (int)::SendMessageW(pThis->m_hWnd, LB_ADDSTRING, 0, (LPARAM)lpszItem);
}

// CListBox::InsertString
// Symbol: ?InsertString@CListBox@@QEAAHHPEB_W@Z
extern "C" int MS_ABI impl__InsertString_CListBox__QEAAHHPEB_W_Z(CListBox* pThis, int nIndex, const wchar_t* lpszItem) {
    if (!pThis || !pThis->m_hWnd) return LB_ERR;
    return (int)::SendMessageW(pThis->m_hWnd, LB_INSERTSTRING, nIndex, (LPARAM)lpszItem);
}

// CListBox::DeleteString
// Symbol: ?DeleteString@CListBox@@QEAAHI@Z
extern "C" int MS_ABI impl__DeleteString_CListBox__QEAAHI_Z(CListBox* pThis, unsigned int nIndex) {
    if (!pThis || !pThis->m_hWnd) return LB_ERR;
    return (int)::SendMessageW(pThis->m_hWnd, LB_DELETESTRING, nIndex, 0);
}

// CListBox::ResetContent
// Symbol: ?ResetContent@CListBox@@QEAAXXZ
extern "C" void MS_ABI impl__ResetContent_CListBox__QEAAXXZ(CListBox* pThis) {
    if (pThis && pThis->m_hWnd) {
        ::SendMessageW(pThis->m_hWnd, LB_RESETCONTENT, 0, 0);
    }
}

// CListBox::FindString
// Symbol: ?FindString@CListBox@@QEBAHPEB_WH@Z
extern "C" int MS_ABI impl__FindString_CListBox__QEBAHPEB_WH_Z(const CListBox* pThis, int nStartAfter, const wchar_t* lpszItem) {
    if (!pThis || !pThis->m_hWnd) return LB_ERR;
    return (int)::SendMessageW(pThis->m_hWnd, LB_FINDSTRING, nStartAfter, (LPARAM)lpszItem);
}

// CListBox::SelectString
// Symbol: ?SelectString@CListBox@@QEAAHPEB_WH@Z
extern "C" int MS_ABI impl__SelectString_CListBox__QEAAHPEB_WH_Z(CListBox* pThis, int nStartAfter, const wchar_t* lpszItem) {
    if (!pThis || !pThis->m_hWnd) return LB_ERR;
    return (int)::SendMessageW(pThis->m_hWnd, LB_SELECTSTRING, nStartAfter, (LPARAM)lpszItem);
}

// CListBox::GetItemData
// Symbol: ?GetItemData@CListBox@@QEBA_KH@Z
extern "C" uintptr_t MS_ABI impl__GetItemData_CListBox__QEBA_KH_Z(const CListBox* pThis, int nIndex) {
    if (!pThis || !pThis->m_hWnd) return 0;
    return (uintptr_t)::SendMessageW(pThis->m_hWnd, LB_GETITEMDATA, nIndex, 0);
}

// CListBox::SetItemData
// Symbol: ?SetItemData@CListBox@@QEAAHH_K@Z
extern "C" int MS_ABI impl__SetItemData_CListBox__QEAAHH_K_Z(CListBox* pThis, int nIndex, uintptr_t dwItemData) {
    if (!pThis || !pThis->m_hWnd) return LB_ERR;
    return (int)::SendMessageW(pThis->m_hWnd, LB_SETITEMDATA, nIndex, (LPARAM)dwItemData);
}

// CListBox::GetItemDataPtr
// Symbol: ?GetItemDataPtr@CListBox@@QEBAPEAXH@Z
extern "C" void* MS_ABI impl__GetItemDataPtr_CListBox__QEBAPEAXH_Z(const CListBox* pThis, int nIndex) {
    if (!pThis || !pThis->m_hWnd) return nullptr;
    return (void*)::SendMessageW(pThis->m_hWnd, LB_GETITEMDATA, nIndex, 0);
}

// CListBox::SetItemDataPtr
// Symbol: ?SetItemDataPtr@CListBox@@QEAAHHPEAX@Z
extern "C" int MS_ABI impl__SetItemDataPtr_CListBox__QEAAHHPEAX_Z(CListBox* pThis, int nIndex, void* pData) {
    if (!pThis || !pThis->m_hWnd) return LB_ERR;
    return (int)::SendMessageW(pThis->m_hWnd, LB_SETITEMDATA, nIndex, (LPARAM)pData);
}

// =============================================================================
// CComboBox Implementation
// =============================================================================

// CComboBox::GetCount
// Symbol: ?GetCount@CComboBox@@QEBAHXZ
extern "C" int MS_ABI impl__GetCount_CComboBox__QEBAHXZ(const CComboBox* pThis) {
    if (!pThis || !pThis->m_hWnd) return 0;
    return (int)::SendMessageW(pThis->m_hWnd, CB_GETCOUNT, 0, 0);
}

// CComboBox::GetCurSel
// Symbol: ?GetCurSel@CComboBox@@QEBAHXZ
extern "C" int MS_ABI impl__GetCurSel_CComboBox__QEBAHXZ(const CComboBox* pThis) {
    if (!pThis || !pThis->m_hWnd) return CB_ERR;
    return (int)::SendMessageW(pThis->m_hWnd, CB_GETCURSEL, 0, 0);
}

// CComboBox::SetCurSel
// Symbol: ?SetCurSel@CComboBox@@QEAAHH@Z
extern "C" int MS_ABI impl__SetCurSel_CComboBox__QEAAHH_Z(CComboBox* pThis, int nSelect) {
    if (!pThis || !pThis->m_hWnd) return CB_ERR;
    return (int)::SendMessageW(pThis->m_hWnd, CB_SETCURSEL, nSelect, 0);
}

// CComboBox::GetLBText
// Symbol: ?GetLBText@CComboBox@@QEBAHPEA_WH@Z
extern "C" int MS_ABI impl__GetLBText_CComboBox__QEBAHPEA_WH_Z(const CComboBox* pThis, int nIndex, wchar_t* lpszText) {
    if (!pThis || !pThis->m_hWnd || !lpszText) return CB_ERR;
    return (int)::SendMessageW(pThis->m_hWnd, CB_GETLBTEXT, nIndex, (LPARAM)lpszText);
}

// CComboBox::GetLBTextLen
// Symbol: ?GetLBTextLen@CComboBox@@QEBAHH@Z
extern "C" int MS_ABI impl__GetLBTextLen_CComboBox__QEBAHH_Z(const CComboBox* pThis, int nIndex) {
    if (!pThis || !pThis->m_hWnd) return CB_ERR;
    return (int)::SendMessageW(pThis->m_hWnd, CB_GETLBTEXTLEN, nIndex, 0);
}

// CComboBox::AddString
// Symbol: ?AddString@CComboBox@@QEAAHPEB_W@Z
extern "C" int MS_ABI impl__AddString_CComboBox__QEAAHPEB_W_Z(CComboBox* pThis, const wchar_t* lpszString) {
    if (!pThis || !pThis->m_hWnd) return CB_ERR;
    return (int)::SendMessageW(pThis->m_hWnd, CB_ADDSTRING, 0, (LPARAM)lpszString);
}

// CComboBox::InsertString
// Symbol: ?InsertString@CComboBox@@QEAAHHPEB_W@Z
extern "C" int MS_ABI impl__InsertString_CComboBox__QEAAHHPEB_W_Z(CComboBox* pThis, int nIndex, const wchar_t* lpszString) {
    if (!pThis || !pThis->m_hWnd) return CB_ERR;
    return (int)::SendMessageW(pThis->m_hWnd, CB_INSERTSTRING, nIndex, (LPARAM)lpszString);
}

// CComboBox::DeleteString
// Symbol: ?DeleteString@CComboBox@@QEAAHI@Z
extern "C" int MS_ABI impl__DeleteString_CComboBox__QEAAHI_Z(CComboBox* pThis, unsigned int nIndex) {
    if (!pThis || !pThis->m_hWnd) return CB_ERR;
    return (int)::SendMessageW(pThis->m_hWnd, CB_DELETESTRING, nIndex, 0);
}

// CComboBox::ResetContent
// Symbol: ?ResetContent@CComboBox@@QEAAXXZ
extern "C" void MS_ABI impl__ResetContent_CComboBox__QEAAXXZ(CComboBox* pThis) {
    if (pThis && pThis->m_hWnd) {
        ::SendMessageW(pThis->m_hWnd, CB_RESETCONTENT, 0, 0);
    }
}

// CComboBox::FindString
// Symbol: ?FindString@CComboBox@@QEBAHPEB_WH@Z
extern "C" int MS_ABI impl__FindString_CComboBox__QEBAHPEB_WH_Z(const CComboBox* pThis, int nStartAfter, const wchar_t* lpszString) {
    if (!pThis || !pThis->m_hWnd) return CB_ERR;
    return (int)::SendMessageW(pThis->m_hWnd, CB_FINDSTRING, nStartAfter, (LPARAM)lpszString);
}

// CComboBox::SelectString
// Symbol: ?SelectString@CComboBox@@QEAAHPEB_WH@Z
extern "C" int MS_ABI impl__SelectString_CComboBox__QEAAHPEB_WH_Z(CComboBox* pThis, int nStartAfter, const wchar_t* lpszString) {
    if (!pThis || !pThis->m_hWnd) return CB_ERR;
    return (int)::SendMessageW(pThis->m_hWnd, CB_SELECTSTRING, nStartAfter, (LPARAM)lpszString);
}

// CComboBox::GetItemData
// Symbol: ?GetItemData@CComboBox@@QEBA_KH@Z
extern "C" uintptr_t MS_ABI impl__GetItemData_CComboBox__QEBA_KH_Z(const CComboBox* pThis, int nIndex) {
    if (!pThis || !pThis->m_hWnd) return 0;
    return (uintptr_t)::SendMessageW(pThis->m_hWnd, CB_GETITEMDATA, nIndex, 0);
}

// CComboBox::SetItemData
// Symbol: ?SetItemData@CComboBox@@QEAAHH_K@Z
extern "C" int MS_ABI impl__SetItemData_CComboBox__QEAAHH_K_Z(CComboBox* pThis, int nIndex, uintptr_t dwItemData) {
    if (!pThis || !pThis->m_hWnd) return CB_ERR;
    return (int)::SendMessageW(pThis->m_hWnd, CB_SETITEMDATA, nIndex, (LPARAM)dwItemData);
}

// CComboBox::ShowDropDown
// Symbol: ?ShowDropDown@CComboBox@@QEAAXH@Z
extern "C" void MS_ABI impl__ShowDropDown_CComboBox__QEAAXH_Z(CComboBox* pThis, int bShowIt) {
    if (pThis && pThis->m_hWnd) {
        ::SendMessageW(pThis->m_hWnd, CB_SHOWDROPDOWN, bShowIt ? TRUE : FALSE, 0);
    }
}

// CComboBox::GetDroppedState
// Symbol: ?GetDroppedState@CComboBox@@QEBAHXZ
extern "C" int MS_ABI impl__GetDroppedState_CComboBox__QEBAHXZ(const CComboBox* pThis) {
    if (!pThis || !pThis->m_hWnd) return FALSE;
    return (int)::SendMessageW(pThis->m_hWnd, CB_GETDROPPEDSTATE, 0, 0);
}

// =============================================================================
// CScrollBar Implementation
// =============================================================================

// CScrollBar::GetScrollPos
// Symbol: ?GetScrollPos@CScrollBar@@QEBAHXZ
extern "C" int MS_ABI impl__GetScrollPos_CScrollBar__QEBAHXZ(const CScrollBar* pThis) {
    if (!pThis || !pThis->m_hWnd) return 0;
    return ::GetScrollPos(pThis->m_hWnd, SB_CTL);
}

// CScrollBar::SetScrollPos
// Symbol: ?SetScrollPos@CScrollBar@@QEAAHHH@Z
extern "C" int MS_ABI impl__SetScrollPos_CScrollBar__QEAAHHH_Z(CScrollBar* pThis, int nPos, int bRedraw) {
    if (!pThis || !pThis->m_hWnd) return 0;
    return ::SetScrollPos(pThis->m_hWnd, SB_CTL, nPos, bRedraw ? TRUE : FALSE);
}

// CScrollBar::GetScrollRange
// Symbol: ?GetScrollRange@CScrollBar@@QEBAXPEAH0@Z
extern "C" void MS_ABI impl__GetScrollRange_CScrollBar__QEBAXPEAH0_Z(const CScrollBar* pThis, int* lpMinPos, int* lpMaxPos) {
    if (!pThis || !pThis->m_hWnd) {
        if (lpMinPos) *lpMinPos = 0;
        if (lpMaxPos) *lpMaxPos = 0;
        return;
    }
    ::GetScrollRange(pThis->m_hWnd, SB_CTL, lpMinPos, lpMaxPos);
}

// CScrollBar::SetScrollRange
// Symbol: ?SetScrollRange@CScrollBar@@QEAAXHHH@Z
extern "C" void MS_ABI impl__SetScrollRange_CScrollBar__QEAAXHHH_Z(CScrollBar* pThis, int nMinPos, int nMaxPos, int bRedraw) {
    if (pThis && pThis->m_hWnd) {
        ::SetScrollRange(pThis->m_hWnd, SB_CTL, nMinPos, nMaxPos, bRedraw ? TRUE : FALSE);
    }
}

// CScrollBar::GetScrollInfo
// Symbol: ?GetScrollInfo@CScrollBar@@QEBAHPEAUSCROLLINFO@@I@Z
extern "C" int MS_ABI impl__GetScrollInfo_CScrollBar__QEBAHPEAUSCROLLINFO__I_Z(const CScrollBar* pThis, SCROLLINFO* lpScrollInfo, unsigned int nMask) {
    if (!pThis || !pThis->m_hWnd || !lpScrollInfo) return FALSE;
    lpScrollInfo->cbSize = sizeof(SCROLLINFO);
    lpScrollInfo->fMask = nMask;
    return ::GetScrollInfo(pThis->m_hWnd, SB_CTL, lpScrollInfo);
}

// CScrollBar::SetScrollInfo
// Symbol: ?SetScrollInfo@CScrollBar@@QEAAHPEAUSCROLLINFO@@H@Z
extern "C" int MS_ABI impl__SetScrollInfo_CScrollBar__QEAAHPEAUSCROLLINFO__H_Z(CScrollBar* pThis, SCROLLINFO* lpScrollInfo, int bRedraw) {
    if (!pThis || !pThis->m_hWnd || !lpScrollInfo) return 0;
    lpScrollInfo->cbSize = sizeof(SCROLLINFO);
    return ::SetScrollInfo(pThis->m_hWnd, SB_CTL, lpScrollInfo, bRedraw ? TRUE : FALSE);
}

// CScrollBar::EnableScrollBar
// Symbol: ?EnableScrollBar@CScrollBar@@QEAAHI@Z
extern "C" int MS_ABI impl__EnableScrollBar_CScrollBar__QEAAHI_Z(CScrollBar* pThis, unsigned int nArrowFlags) {
    if (!pThis || !pThis->m_hWnd) return FALSE;
    return ::EnableScrollBar(pThis->m_hWnd, SB_CTL, nArrowFlags);
}
