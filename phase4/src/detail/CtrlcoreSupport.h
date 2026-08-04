#pragma once
// Shared internals of the former ctrlcore.cpp translation unit.
// Definitions live in detail/CtrlcoreSupport.cpp, so file-scope state keeps a
// single instance across the classes this unit was split into.
namespace openmfc { namespace detail { namespace ctrlcore {} } }
using namespace openmfc::detail::ctrlcore;
// Control Core Implementation
//
// Implements CButton, CEdit, CStatic, CListBox, CComboBox, and CScrollBar.
// These wrap Windows common controls using standard Windows messages.

#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxwin.h"
#include "openmfc/afxmfc.h"
#include <windows.h>
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
extern "C" int MS_ABI impl__OnChildNotify_CWnd__MEAAHI_K_JPEA_J_Z(
    CWnd* pThis,
    unsigned int p0,
    unsigned __int64 p1,
    __int64 p2,
    __int64* p3);
extern "C" void MS_ABI impl__PreSubclassWindow_CWnd__UEAAXXZ(CWnd* pThis);

// =============================================================================
// CRuntimeClass implementations for control classes
// =============================================================================


namespace openmfc { namespace detail { namespace ctrlcore {
CString OpenMfcKeyName(UINT vk, BOOL extended);
} } }



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

namespace openmfc { namespace detail { namespace ctrlcore {
extern "C" int MS_ABI impl__Create_CButton__UEAAHPEB_WKAEBUtagRECT__PEAVCWnd__I_Z(
    CButton* pThis, const wchar_t* lpszCaption, DWORD dwStyle, const RECT* pRect, CWnd* pParentWnd, UINT nID);
} } }

// CButton::GetState
namespace openmfc { namespace detail { namespace ctrlcore {
extern "C" unsigned int MS_ABI impl__GetState_CButton__QEBAIXZ(const CButton* pThis);
} } }

// CButton::SetState
namespace openmfc { namespace detail { namespace ctrlcore {
extern "C" void MS_ABI impl__SetState_CButton__QEAAXH_Z(CButton* pThis, int bHighlight);
} } }

// CButton::GetCheck
namespace openmfc { namespace detail { namespace ctrlcore {
extern "C" int MS_ABI impl__GetCheck_CButton__QEBAHXZ(const CButton* pThis);
} } }

// CButton::SetCheck
namespace openmfc { namespace detail { namespace ctrlcore {
extern "C" void MS_ABI impl__SetCheck_CButton__QEAAXH_Z(CButton* pThis, int nCheck);
} } }

// CButton::GetButtonStyle
namespace openmfc { namespace detail { namespace ctrlcore {
extern "C" unsigned int MS_ABI impl__GetButtonStyle_CButton__QEBAIXZ(const CButton* pThis);
} } }

// CButton::SetButtonStyle
namespace openmfc { namespace detail { namespace ctrlcore {
extern "C" void MS_ABI impl__SetButtonStyle_CButton__QEAAXIH_Z(CButton* pThis, unsigned int nStyle, int bRedraw);
} } }

// CButton::GetBitmap
namespace openmfc { namespace detail { namespace ctrlcore {
extern "C" void* MS_ABI impl__GetBitmap_CButton__QEBAPEAXXZ(const CButton* pThis);
} } }

// CButton::SetBitmap
namespace openmfc { namespace detail { namespace ctrlcore {
extern "C" void* MS_ABI impl__SetBitmap_CButton__QEAAPEAXPEAX_Z(CButton* pThis, void* hBitmap);
} } }

// CButton::GetIcon
namespace openmfc { namespace detail { namespace ctrlcore {
extern "C" void* MS_ABI impl__GetIcon_CButton__QEBAPEAXXZ(const CButton* pThis);
} } }

// CButton::SetIcon
namespace openmfc { namespace detail { namespace ctrlcore {
extern "C" void* MS_ABI impl__SetIcon_CButton__QEAAPEAXPEAX_Z(CButton* pThis, void* hIcon);
} } }

// =============================================================================
// CEdit Implementation
// =============================================================================

namespace openmfc { namespace detail { namespace ctrlcore {
extern "C" int MS_ABI impl__Create_CEdit__QEAAHKAEBUtagRECT__PEAVCWnd__I_Z(
    CEdit* pThis, DWORD dwStyle, const RECT* pRect, CWnd* pParentWnd, UINT nID);
} } }

// CEdit::GetLineCount
namespace openmfc { namespace detail { namespace ctrlcore {
extern "C" int MS_ABI impl__GetLineCount_CEdit__QEBAHXZ(const CEdit* pThis);
} } }

// CEdit::GetLine
namespace openmfc { namespace detail { namespace ctrlcore {
extern "C" int MS_ABI impl__GetLine_CEdit__QEBAHPEA_WH_Z(const CEdit* pThis, int nIndex, wchar_t* lpszBuffer, int nMaxLength);
} } }

// CEdit::GetSel
namespace openmfc { namespace detail { namespace ctrlcore {
extern "C" void MS_ABI impl__GetSel_CEdit__QEBAXAEAH0_Z(const CEdit* pThis, int* nStartChar, int* nEndChar);
} } }

// CEdit::SetSel
namespace openmfc { namespace detail { namespace ctrlcore {
extern "C" void MS_ABI impl__SetSel_CEdit__QEAAXHHH_Z(CEdit* pThis, int nStartChar, int nEndChar, int bNoScroll);
} } }

// CEdit::ReplaceSel
namespace openmfc { namespace detail { namespace ctrlcore {
extern "C" void MS_ABI impl__ReplaceSel_CEdit__QEAAXPEB_WH_Z(CEdit* pThis, const wchar_t* lpszNewText, int bCanUndo);
} } }

// CEdit::Clear
namespace openmfc { namespace detail { namespace ctrlcore {
extern "C" void MS_ABI impl__Clear_CEdit__QEAAXXZ(CEdit* pThis);
} } }

// CEdit::Copy
namespace openmfc { namespace detail { namespace ctrlcore {
extern "C" void MS_ABI impl__Copy_CEdit__QEAAXXZ(CEdit* pThis);
} } }

// CEdit::Cut
namespace openmfc { namespace detail { namespace ctrlcore {
extern "C" void MS_ABI impl__Cut_CEdit__QEAAXXZ(CEdit* pThis);
} } }

// CEdit::Paste
namespace openmfc { namespace detail { namespace ctrlcore {
extern "C" void MS_ABI impl__Paste_CEdit__QEAAXXZ(CEdit* pThis);
} } }

// CEdit::Undo
namespace openmfc { namespace detail { namespace ctrlcore {
extern "C" int MS_ABI impl__Undo_CEdit__QEAAHXZ(CEdit* pThis);
} } }

// CEdit::SetReadOnly
namespace openmfc { namespace detail { namespace ctrlcore {
extern "C" void MS_ABI impl__SetReadOnly_CEdit__QEAAXH_Z(CEdit* pThis, int bReadOnly);
} } }

// CEdit::GetModify
namespace openmfc { namespace detail { namespace ctrlcore {
extern "C" int MS_ABI impl__GetModify_CEdit__QEBAHXZ(const CEdit* pThis);
} } }

// CEdit::SetModify
namespace openmfc { namespace detail { namespace ctrlcore {
extern "C" void MS_ABI impl__SetModify_CEdit__QEAAXH_Z(CEdit* pThis, int bModified);
} } }

// CEdit::LimitText
namespace openmfc { namespace detail { namespace ctrlcore {
extern "C" void MS_ABI impl__LimitText_CEdit__QEAAXH_Z(CEdit* pThis, int nChars);
} } }

// CEdit::GetLimitText
namespace openmfc { namespace detail { namespace ctrlcore {
extern "C" int MS_ABI impl__GetLimitText_CEdit__QEBAHXZ(const CEdit* pThis);
} } }

// CEdit::SetPasswordChar
namespace openmfc { namespace detail { namespace ctrlcore {
extern "C" void MS_ABI impl__SetPasswordChar_CEdit__QEAAX_W_Z(CEdit* pThis, wchar_t ch);
} } }

// =============================================================================
// CStatic Implementation
// =============================================================================

namespace openmfc { namespace detail { namespace ctrlcore {
extern "C" int MS_ABI impl__Create_CStatic__UEAAHPEB_WKAEBUtagRECT__PEAVCWnd__I_Z(
    CStatic* pThis, const wchar_t* lpszText, DWORD dwStyle, const RECT* pRect, CWnd* pParentWnd, UINT nID);
} } }

// CStatic::GetBitmap
namespace openmfc { namespace detail { namespace ctrlcore {
extern "C" void* MS_ABI impl__GetBitmap_CStatic__QEBAPEAXXZ(const CStatic* pThis);
} } }

// CStatic::SetBitmap
namespace openmfc { namespace detail { namespace ctrlcore {
extern "C" void* MS_ABI impl__SetBitmap_CStatic__QEAAPEAXPEAX_Z(CStatic* pThis, void* hBitmap);
} } }

// CStatic::GetIcon
namespace openmfc { namespace detail { namespace ctrlcore {
extern "C" void* MS_ABI impl__GetIcon_CStatic__QEBAPEAXXZ(const CStatic* pThis);
} } }

// CStatic::SetIcon
namespace openmfc { namespace detail { namespace ctrlcore {
extern "C" void* MS_ABI impl__SetIcon_CStatic__QEAAPEAXPEAX_Z(CStatic* pThis, void* hIcon);
} } }

// CStatic::GetEnhMetaFile
namespace openmfc { namespace detail { namespace ctrlcore {
extern "C" void* MS_ABI impl__GetEnhMetaFile_CStatic__QEBAPEAXXZ(const CStatic* pThis);
} } }

// CStatic::SetEnhMetaFile
namespace openmfc { namespace detail { namespace ctrlcore {
extern "C" void* MS_ABI impl__SetEnhMetaFile_CStatic__QEAAPEAXPEAX_Z(CStatic* pThis, void* hMetaFile);
} } }

// =============================================================================
// CListBox Implementation
// =============================================================================

namespace openmfc { namespace detail { namespace ctrlcore {
extern "C" int MS_ABI impl__Create_CListBox__UEAAHKAEBUtagRECT__PEAVCWnd__I_Z(
    CListBox* pThis, DWORD dwStyle, const RECT* pRect, CWnd* pParentWnd, UINT nID);
} } }

// CListBox::GetCount
namespace openmfc { namespace detail { namespace ctrlcore {
extern "C" int MS_ABI impl__GetCount_CListBox__QEBAHXZ(const CListBox* pThis);
} } }

// CListBox::GetCurSel
namespace openmfc { namespace detail { namespace ctrlcore {
extern "C" int MS_ABI impl__GetCurSel_CListBox__QEBAHXZ(const CListBox* pThis);
} } }

// CListBox::SetCurSel
namespace openmfc { namespace detail { namespace ctrlcore {
extern "C" int MS_ABI impl__SetCurSel_CListBox__QEAAHH_Z(CListBox* pThis, int nSelect);
} } }

// CListBox::GetText
namespace openmfc { namespace detail { namespace ctrlcore {
extern "C" int MS_ABI impl__GetText_CListBox__QEBAHPEA_WH_Z(const CListBox* pThis, int nIndex, wchar_t* lpszBuffer);
} } }

extern "C" int MS_ABI impl__GetTextLen_CListBox__QEBAHH_Z(const CListBox* pThis, int nIndex);


// CListBox::GetTextLen
extern "C" int MS_ABI impl__GetTextLen_CListBox__QEBAHH_Z(const CListBox* pThis, int nIndex);

// CListBox::AddString
namespace openmfc { namespace detail { namespace ctrlcore {
extern "C" int MS_ABI impl__AddString_CListBox__QEAAHPEB_W_Z(CListBox* pThis, const wchar_t* lpszItem);
} } }

// CListBox::InsertString
namespace openmfc { namespace detail { namespace ctrlcore {
extern "C" int MS_ABI impl__InsertString_CListBox__QEAAHHPEB_W_Z(CListBox* pThis, int nIndex, const wchar_t* lpszItem);
} } }

// CListBox::DeleteString
namespace openmfc { namespace detail { namespace ctrlcore {
extern "C" int MS_ABI impl__DeleteString_CListBox__QEAAHI_Z(CListBox* pThis, unsigned int nIndex);
} } }

// CListBox::ResetContent
namespace openmfc { namespace detail { namespace ctrlcore {
extern "C" void MS_ABI impl__ResetContent_CListBox__QEAAXXZ(CListBox* pThis);
} } }

// CListBox::FindString
namespace openmfc { namespace detail { namespace ctrlcore {
extern "C" int MS_ABI impl__FindString_CListBox__QEBAHPEB_WH_Z(const CListBox* pThis, const wchar_t* lpszItem, int nStartAfter);
} } }

// CListBox::SelectString
namespace openmfc { namespace detail { namespace ctrlcore {
extern "C" int MS_ABI impl__SelectString_CListBox__QEAAHPEB_WH_Z(CListBox* pThis, int nStartAfter, const wchar_t* lpszItem);
} } }

// CListBox::GetItemData
namespace openmfc { namespace detail { namespace ctrlcore {
extern "C" uintptr_t MS_ABI impl__GetItemData_CListBox__QEBA_KH_Z(const CListBox* pThis, int nIndex);
} } }

// CListBox::SetItemData
namespace openmfc { namespace detail { namespace ctrlcore {
extern "C" int MS_ABI impl__SetItemData_CListBox__QEAAHH_K_Z(CListBox* pThis, int nIndex, uintptr_t dwItemData);
} } }

// CListBox::GetItemDataPtr
namespace openmfc { namespace detail { namespace ctrlcore {
extern "C" void* MS_ABI impl__GetItemDataPtr_CListBox__QEBAPEAXH_Z(const CListBox* pThis, int nIndex);
} } }

// CListBox::SetItemDataPtr
namespace openmfc { namespace detail { namespace ctrlcore {
extern "C" int MS_ABI impl__SetItemDataPtr_CListBox__QEAAHHPEAX_Z(CListBox* pThis, int nIndex, void* pData);
} } }

// =============================================================================
// CComboBox Implementation
// =============================================================================

namespace openmfc { namespace detail { namespace ctrlcore {
extern "C" int MS_ABI impl__Create_CComboBox__UEAAHKAEBUtagRECT__PEAVCWnd__I_Z(
    CComboBox* pThis, DWORD dwStyle, const RECT* pRect, CWnd* pParentWnd, UINT nID);
} } }

// CComboBox::GetCount
namespace openmfc { namespace detail { namespace ctrlcore {
extern "C" int MS_ABI impl__GetCount_CComboBox__QEBAHXZ(const CComboBox* pThis);
} } }

// CComboBox::GetCurSel
namespace openmfc { namespace detail { namespace ctrlcore {
extern "C" int MS_ABI impl__GetCurSel_CComboBox__QEBAHXZ(const CComboBox* pThis);
} } }

// CComboBox::SetCurSel
namespace openmfc { namespace detail { namespace ctrlcore {
extern "C" int MS_ABI impl__SetCurSel_CComboBox__QEAAHH_Z(CComboBox* pThis, int nSelect);
} } }

// CComboBox::GetLBText
namespace openmfc { namespace detail { namespace ctrlcore {
extern "C" int MS_ABI impl__GetLBText_CComboBox__QEBAHPEA_WH_Z(const CComboBox* pThis, int nIndex, wchar_t* lpszText);
} } }

// CComboBox::GetLBTextLen
namespace openmfc { namespace detail { namespace ctrlcore {
extern "C" int MS_ABI impl__GetLBTextLen_CComboBox__QEBAHH_Z(const CComboBox* pThis, int nIndex);
} } }

// CComboBox::AddString
namespace openmfc { namespace detail { namespace ctrlcore {
extern "C" int MS_ABI impl__AddString_CComboBox__QEAAHPEB_W_Z(CComboBox* pThis, const wchar_t* lpszString);
} } }

// CComboBox::InsertString
namespace openmfc { namespace detail { namespace ctrlcore {
extern "C" int MS_ABI impl__InsertString_CComboBox__QEAAHHPEB_W_Z(CComboBox* pThis, int nIndex, const wchar_t* lpszString);
} } }

// CComboBox::DeleteString
namespace openmfc { namespace detail { namespace ctrlcore {
extern "C" int MS_ABI impl__DeleteString_CComboBox__QEAAHI_Z(CComboBox* pThis, unsigned int nIndex);
} } }

// CComboBox::ResetContent
namespace openmfc { namespace detail { namespace ctrlcore {
extern "C" void MS_ABI impl__ResetContent_CComboBox__QEAAXXZ(CComboBox* pThis);
} } }

// CComboBox::FindString
namespace openmfc { namespace detail { namespace ctrlcore {
extern "C" int MS_ABI impl__FindString_CComboBox__QEBAHPEB_WH_Z(const CComboBox* pThis, const wchar_t* lpszString, int nStartAfter);
} } }

// CComboBox::SelectString
namespace openmfc { namespace detail { namespace ctrlcore {
extern "C" int MS_ABI impl__SelectString_CComboBox__QEAAHPEB_WH_Z(CComboBox* pThis, int nStartAfter, const wchar_t* lpszString);
} } }

// CComboBox::GetItemData
namespace openmfc { namespace detail { namespace ctrlcore {
extern "C" uintptr_t MS_ABI impl__GetItemData_CComboBox__QEBA_KH_Z(const CComboBox* pThis, int nIndex);
} } }

// CComboBox::SetItemData
namespace openmfc { namespace detail { namespace ctrlcore {
extern "C" int MS_ABI impl__SetItemData_CComboBox__QEAAHH_K_Z(CComboBox* pThis, int nIndex, uintptr_t dwItemData);
} } }

// CComboBox::ShowDropDown
namespace openmfc { namespace detail { namespace ctrlcore {
extern "C" void MS_ABI impl__ShowDropDown_CComboBox__QEAAXH_Z(CComboBox* pThis, int bShowIt);
} } }

// CComboBox::GetDroppedState
namespace openmfc { namespace detail { namespace ctrlcore {
extern "C" int MS_ABI impl__GetDroppedState_CComboBox__QEBAHXZ(const CComboBox* pThis);
} } }



namespace openmfc { namespace detail { namespace ctrlcore {
using ItemStateMap = std::unordered_map<int, int>;
} } }
namespace openmfc { namespace detail { namespace ctrlcore {
extern std::unordered_map<const CSplitButton*, HMENU> g_splitMenus;
} } }
namespace openmfc { namespace detail { namespace ctrlcore {
extern std::unordered_map<const CDragListBox*, int> g_dragSourceItem;
} } }

namespace openmfc { namespace detail { namespace ctrlcore {
int ListBoxItemFromPoint_Ctrlcore(HWND hWnd, CPoint pt);
} } }



























// =============================================================================
// CScrollBar Implementation
// =============================================================================

namespace openmfc { namespace detail { namespace ctrlcore {
extern "C" int MS_ABI impl__Create_CScrollBar__UEAAHKAEBUtagRECT__PEAVCWnd__I_Z(
    CScrollBar* pThis, DWORD dwStyle, const RECT* pRect, CWnd* pParentWnd, UINT nID);
} } }

// CScrollBar::GetScrollPos
namespace openmfc { namespace detail { namespace ctrlcore {
extern "C" int MS_ABI impl__GetScrollPos_CScrollBar__QEBAHXZ(const CScrollBar* pThis);
} } }

// CScrollBar::SetScrollPos
namespace openmfc { namespace detail { namespace ctrlcore {
extern "C" int MS_ABI impl__SetScrollPos_CScrollBar__QEAAHHH_Z(CScrollBar* pThis, int nPos, int bRedraw);
} } }

// CScrollBar::GetScrollRange
namespace openmfc { namespace detail { namespace ctrlcore {
extern "C" void MS_ABI impl__GetScrollRange_CScrollBar__QEBAXPEAH0_Z(const CScrollBar* pThis, int* lpMinPos, int* lpMaxPos);
} } }

// CScrollBar::SetScrollRange
namespace openmfc { namespace detail { namespace ctrlcore {
extern "C" void MS_ABI impl__SetScrollRange_CScrollBar__QEAAXHHH_Z(CScrollBar* pThis, int nMinPos, int nMaxPos, int bRedraw);
} } }

// CScrollBar::GetScrollInfo
namespace openmfc { namespace detail { namespace ctrlcore {
extern "C" int MS_ABI impl__GetScrollInfo_CScrollBar__QEBAHPEAUSCROLLINFO__I_Z(const CScrollBar* pThis, SCROLLINFO* lpScrollInfo, unsigned int nMask);
} } }

// CScrollBar::SetScrollInfo
namespace openmfc { namespace detail { namespace ctrlcore {
extern "C" int MS_ABI impl__SetScrollInfo_CScrollBar__QEAAHPEAUSCROLLINFO__H_Z(CScrollBar* pThis, SCROLLINFO* lpScrollInfo, int bRedraw);
} } }

// CScrollBar::EnableScrollBar
namespace openmfc { namespace detail { namespace ctrlcore {
extern "C" int MS_ABI impl__EnableScrollBar_CScrollBar__QEAAHI_Z(CScrollBar* pThis, unsigned int nArrowFlags);
} } }

namespace openmfc { namespace detail { namespace ctrlcore {
extern "C" int MS_ABI impl__Create_CSliderCtrl__UEAAHKAEBUtagRECT__PEAVCWnd__I_Z(
    CSliderCtrl* pThis, DWORD dwStyle, const RECT* pRect, CWnd* pParentWnd, UINT nID);
} } }

namespace openmfc { namespace detail { namespace ctrlcore {
extern "C" int MS_ABI impl__Create_CProgressCtrl__UEAAHKAEBUtagRECT__PEAVCWnd__I_Z(
    CProgressCtrl* pThis, DWORD dwStyle, const RECT* pRect, CWnd* pParentWnd, UINT nID);
} } }

namespace openmfc { namespace detail { namespace ctrlcore {
extern "C" int MS_ABI impl__Create_CSpinButtonCtrl__UEAAHKAEBUtagRECT__PEAVCWnd__I_Z(
    CSpinButtonCtrl* pThis, DWORD dwStyle, const RECT* pRect, CWnd* pParentWnd, UINT nID);
} } }

namespace openmfc { namespace detail { namespace ctrlcore {
extern "C" int MS_ABI impl__Create_CListCtrl__QEAAHKAEBUtagRECT__PEAVCWnd__I_Z(
    CListCtrl* pThis, DWORD dwStyle, const RECT* pRect, CWnd* pParentWnd, UINT nID);
} } }

namespace openmfc { namespace detail { namespace ctrlcore {
extern "C" int MS_ABI impl__Create_CTreeCtrl__QEAAHKAEBUtagRECT__PEAVCWnd__I_Z(
    CTreeCtrl* pThis, DWORD dwStyle, const RECT* pRect, CWnd* pParentWnd, UINT nID);
} } }

namespace openmfc { namespace detail { namespace ctrlcore {
extern "C" int MS_ABI impl__Create_CTabCtrl__UEAAHKAEBUtagRECT__PEAVCWnd__I_Z(
    CTabCtrl* pThis, DWORD dwStyle, const RECT* pRect, CWnd* pParentWnd, UINT nID);
} } }














