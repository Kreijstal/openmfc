#include "CtrlcoreSupport.h"

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
extern "C" int MS_ABI impl__GetTextLen_CListBox__QEBAHH_Z(const CListBox* pThis, int nIndex) {
    if (!pThis || !pThis->m_hWnd) return LB_ERR;
    return (int)::SendMessageW(pThis->m_hWnd, LB_GETTEXTLEN, nIndex, 0);
}
namespace openmfc { namespace detail { namespace ctrlcore {
CString OpenMfcKeyName(UINT vk, BOOL extended) {
    if (vk == 0) return CString();

    UINT scanCode = ::MapVirtualKeyW(vk, MAPVK_VK_TO_VSC);
    LONG lParam = static_cast<LONG>(scanCode << 16);
    if (extended) lParam |= 1L << 24;

    wchar_t buffer[128] = {};
    if (::GetKeyNameTextW(lParam, buffer, static_cast<int>(sizeof(buffer) / sizeof(buffer[0]))) > 0) {
        return CString(buffer);
    }

    if (vk >= L' ' && vk <= 0x7e) {
        wchar_t fallback[2] = {static_cast<wchar_t>(vk), L'\0'};
        return CString(fallback);
    }

    CString fallback;
    fallback.Format(L"VK_%02X", vk);
    return fallback;
}
extern "C" int MS_ABI impl__Create_CButton__UEAAHPEB_WKAEBUtagRECT__PEAVCWnd__I_Z(
    CButton* pThis, const wchar_t* lpszCaption, DWORD dwStyle, const RECT* pRect, CWnd* pParentWnd, UINT nID) {
    if (!pThis || !pRect) return FALSE;
    return impl__Create_CWnd__UEAAHPEB_W0KAEBUtagRECT__PEAV1_IPEAUCCreateContext___Z(
        pThis, L"BUTTON", lpszCaption, dwStyle, *pRect, pParentWnd, nID, nullptr);
}
extern "C" unsigned int MS_ABI impl__GetState_CButton__QEBAIXZ(const CButton* pThis) {
    if (!pThis || !pThis->m_hWnd) return 0;
    return (unsigned int)::SendMessageW(pThis->m_hWnd, BM_GETSTATE, 0, 0);
}
extern "C" void MS_ABI impl__SetState_CButton__QEAAXH_Z(CButton* pThis, int bHighlight) {
    if (pThis && pThis->m_hWnd) {
        ::SendMessageW(pThis->m_hWnd, BM_SETSTATE, bHighlight ? TRUE : FALSE, 0);
    }
}
extern "C" int MS_ABI impl__GetCheck_CButton__QEBAHXZ(const CButton* pThis) {
    if (!pThis || !pThis->m_hWnd) return 0;
    return (int)::SendMessageW(pThis->m_hWnd, BM_GETCHECK, 0, 0);
}
extern "C" void MS_ABI impl__SetCheck_CButton__QEAAXH_Z(CButton* pThis, int nCheck) {
    if (pThis && pThis->m_hWnd) {
        ::SendMessageW(pThis->m_hWnd, BM_SETCHECK, nCheck, 0);
    }
}
extern "C" unsigned int MS_ABI impl__GetButtonStyle_CButton__QEBAIXZ(const CButton* pThis) {
    if (!pThis || !pThis->m_hWnd) return 0;
    return (unsigned int)(::GetWindowLongW(pThis->m_hWnd, GWL_STYLE) & 0xFFFF);
}
extern "C" void MS_ABI impl__SetButtonStyle_CButton__QEAAXIH_Z(CButton* pThis, unsigned int nStyle, int bRedraw) {
    if (pThis && pThis->m_hWnd) {
        ::SendMessageW(pThis->m_hWnd, BM_SETSTYLE, nStyle, MAKELPARAM(bRedraw, 0));
    }
}
extern "C" void* MS_ABI impl__GetBitmap_CButton__QEBAPEAXXZ(const CButton* pThis) {
    if (!pThis || !pThis->m_hWnd) return nullptr;
    return (void*)::SendMessageW(pThis->m_hWnd, BM_GETIMAGE, IMAGE_BITMAP, 0);
}
extern "C" void* MS_ABI impl__SetBitmap_CButton__QEAAPEAXPEAX_Z(CButton* pThis, void* hBitmap) {
    if (!pThis || !pThis->m_hWnd) return nullptr;
    return (void*)::SendMessageW(pThis->m_hWnd, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hBitmap);
}
extern "C" void* MS_ABI impl__GetIcon_CButton__QEBAPEAXXZ(const CButton* pThis) {
    if (!pThis || !pThis->m_hWnd) return nullptr;
    return (void*)::SendMessageW(pThis->m_hWnd, BM_GETIMAGE, IMAGE_ICON, 0);
}
extern "C" void* MS_ABI impl__SetIcon_CButton__QEAAPEAXPEAX_Z(CButton* pThis, void* hIcon) {
    if (!pThis || !pThis->m_hWnd) return nullptr;
    return (void*)::SendMessageW(pThis->m_hWnd, BM_SETIMAGE, IMAGE_ICON, (LPARAM)hIcon);
}
extern "C" int MS_ABI impl__Create_CEdit__QEAAHKAEBUtagRECT__PEAVCWnd__I_Z(
    CEdit* pThis, DWORD dwStyle, const RECT* pRect, CWnd* pParentWnd, UINT nID) {
    if (!pThis || !pRect) return FALSE;
    return impl__Create_CWnd__UEAAHPEB_W0KAEBUtagRECT__PEAV1_IPEAUCCreateContext___Z(
        pThis, L"EDIT", L"", dwStyle, *pRect, pParentWnd, nID, nullptr);
}
extern "C" int MS_ABI impl__GetLineCount_CEdit__QEBAHXZ(const CEdit* pThis) {
    if (!pThis || !pThis->m_hWnd) return 1;
    return (int)::SendMessageW(pThis->m_hWnd, EM_GETLINECOUNT, 0, 0);
}
extern "C" int MS_ABI impl__GetLine_CEdit__QEBAHPEA_WH_Z(const CEdit* pThis, int nIndex, wchar_t* lpszBuffer, int nMaxLength) {
    if (!pThis || !pThis->m_hWnd || !lpszBuffer || nMaxLength <= 0) return 0;
    // First word of buffer must contain the max length
    *(WORD*)lpszBuffer = (WORD)nMaxLength;
    return (int)::SendMessageW(pThis->m_hWnd, EM_GETLINE, nIndex, (LPARAM)lpszBuffer);
}
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
extern "C" void MS_ABI impl__SetSel_CEdit__QEAAXHHH_Z(CEdit* pThis, int nStartChar, int nEndChar, int bNoScroll) {
    if (pThis && pThis->m_hWnd) {
        ::SendMessageW(pThis->m_hWnd, EM_SETSEL, nStartChar, nEndChar);
        if (!bNoScroll) {
            ::SendMessageW(pThis->m_hWnd, EM_SCROLLCARET, 0, 0);
        }
    }
}
extern "C" void MS_ABI impl__ReplaceSel_CEdit__QEAAXPEB_WH_Z(CEdit* pThis, const wchar_t* lpszNewText, int bCanUndo) {
    if (pThis && pThis->m_hWnd) {
        ::SendMessageW(pThis->m_hWnd, EM_REPLACESEL, bCanUndo ? TRUE : FALSE, (LPARAM)lpszNewText);
    }
}
extern "C" void MS_ABI impl__Clear_CEdit__QEAAXXZ(CEdit* pThis) {
    if (pThis && pThis->m_hWnd) {
        ::SendMessageW(pThis->m_hWnd, WM_CLEAR, 0, 0);
    }
}
extern "C" void MS_ABI impl__Copy_CEdit__QEAAXXZ(CEdit* pThis) {
    if (pThis && pThis->m_hWnd) {
        ::SendMessageW(pThis->m_hWnd, WM_COPY, 0, 0);
    }
}
extern "C" void MS_ABI impl__Cut_CEdit__QEAAXXZ(CEdit* pThis) {
    if (pThis && pThis->m_hWnd) {
        ::SendMessageW(pThis->m_hWnd, WM_CUT, 0, 0);
    }
}
extern "C" void MS_ABI impl__Paste_CEdit__QEAAXXZ(CEdit* pThis) {
    if (pThis && pThis->m_hWnd) {
        ::SendMessageW(pThis->m_hWnd, WM_PASTE, 0, 0);
    }
}
extern "C" int MS_ABI impl__Undo_CEdit__QEAAHXZ(CEdit* pThis) {
    if (!pThis || !pThis->m_hWnd) return FALSE;
    return (int)::SendMessageW(pThis->m_hWnd, EM_UNDO, 0, 0);
}
extern "C" void MS_ABI impl__SetReadOnly_CEdit__QEAAXH_Z(CEdit* pThis, int bReadOnly) {
    if (pThis && pThis->m_hWnd) {
        ::SendMessageW(pThis->m_hWnd, EM_SETREADONLY, bReadOnly ? TRUE : FALSE, 0);
    }
}
extern "C" int MS_ABI impl__GetModify_CEdit__QEBAHXZ(const CEdit* pThis) {
    if (!pThis || !pThis->m_hWnd) return FALSE;
    return (int)::SendMessageW(pThis->m_hWnd, EM_GETMODIFY, 0, 0);
}
extern "C" void MS_ABI impl__SetModify_CEdit__QEAAXH_Z(CEdit* pThis, int bModified) {
    if (pThis && pThis->m_hWnd) {
        ::SendMessageW(pThis->m_hWnd, EM_SETMODIFY, bModified ? TRUE : FALSE, 0);
    }
}
extern "C" void MS_ABI impl__LimitText_CEdit__QEAAXH_Z(CEdit* pThis, int nChars) {
    if (pThis && pThis->m_hWnd) {
        ::SendMessageW(pThis->m_hWnd, EM_LIMITTEXT, nChars, 0);
    }
}
extern "C" int MS_ABI impl__GetLimitText_CEdit__QEBAHXZ(const CEdit* pThis) {
    if (!pThis || !pThis->m_hWnd) return 0;
    return (int)::SendMessageW(pThis->m_hWnd, EM_GETLIMITTEXT, 0, 0);
}
extern "C" void MS_ABI impl__SetPasswordChar_CEdit__QEAAX_W_Z(CEdit* pThis, wchar_t ch) {
    if (pThis && pThis->m_hWnd) {
        ::SendMessageW(pThis->m_hWnd, EM_SETPASSWORDCHAR, ch, 0);
    }
}
extern "C" int MS_ABI impl__Create_CStatic__UEAAHPEB_WKAEBUtagRECT__PEAVCWnd__I_Z(
    CStatic* pThis, const wchar_t* lpszText, DWORD dwStyle, const RECT* pRect, CWnd* pParentWnd, UINT nID) {
    if (!pThis || !pRect) return FALSE;
    return impl__Create_CWnd__UEAAHPEB_W0KAEBUtagRECT__PEAV1_IPEAUCCreateContext___Z(
        pThis, L"STATIC", lpszText, dwStyle, *pRect, pParentWnd, nID, nullptr);
}
extern "C" void* MS_ABI impl__GetBitmap_CStatic__QEBAPEAXXZ(const CStatic* pThis) {
    if (!pThis || !pThis->m_hWnd) return nullptr;
    return (void*)::SendMessageW(pThis->m_hWnd, STM_GETIMAGE, IMAGE_BITMAP, 0);
}
extern "C" void* MS_ABI impl__SetBitmap_CStatic__QEAAPEAXPEAX_Z(CStatic* pThis, void* hBitmap) {
    if (!pThis || !pThis->m_hWnd) return nullptr;
    return (void*)::SendMessageW(pThis->m_hWnd, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hBitmap);
}
extern "C" void* MS_ABI impl__GetIcon_CStatic__QEBAPEAXXZ(const CStatic* pThis) {
    if (!pThis || !pThis->m_hWnd) return nullptr;
    return (void*)::SendMessageW(pThis->m_hWnd, STM_GETICON, 0, 0);
}
extern "C" void* MS_ABI impl__SetIcon_CStatic__QEAAPEAXPEAX_Z(CStatic* pThis, void* hIcon) {
    if (!pThis || !pThis->m_hWnd) return nullptr;
    return (void*)::SendMessageW(pThis->m_hWnd, STM_SETICON, (WPARAM)hIcon, 0);
}
extern "C" void* MS_ABI impl__GetEnhMetaFile_CStatic__QEBAPEAXXZ(const CStatic* pThis) {
    if (!pThis || !pThis->m_hWnd) return nullptr;
    return (void*)::SendMessageW(pThis->m_hWnd, STM_GETIMAGE, IMAGE_ENHMETAFILE, 0);
}
extern "C" void* MS_ABI impl__SetEnhMetaFile_CStatic__QEAAPEAXPEAX_Z(CStatic* pThis, void* hMetaFile) {
    if (!pThis || !pThis->m_hWnd) return nullptr;
    return (void*)::SendMessageW(pThis->m_hWnd, STM_SETIMAGE, IMAGE_ENHMETAFILE, (LPARAM)hMetaFile);
}
extern "C" int MS_ABI impl__Create_CListBox__UEAAHKAEBUtagRECT__PEAVCWnd__I_Z(
    CListBox* pThis, DWORD dwStyle, const RECT* pRect, CWnd* pParentWnd, UINT nID) {
    if (!pThis || !pRect) return FALSE;
    return impl__Create_CWnd__UEAAHPEB_W0KAEBUtagRECT__PEAV1_IPEAUCCreateContext___Z(
        pThis, L"LISTBOX", L"", dwStyle, *pRect, pParentWnd, nID, nullptr);
}
extern "C" int MS_ABI impl__GetCount_CListBox__QEBAHXZ(const CListBox* pThis) {
    if (!pThis || !pThis->m_hWnd) return 0;
    return (int)::SendMessageW(pThis->m_hWnd, LB_GETCOUNT, 0, 0);
}
extern "C" int MS_ABI impl__GetCurSel_CListBox__QEBAHXZ(const CListBox* pThis) {
    if (!pThis || !pThis->m_hWnd) return LB_ERR;
    return (int)::SendMessageW(pThis->m_hWnd, LB_GETCURSEL, 0, 0);
}
extern "C" int MS_ABI impl__SetCurSel_CListBox__QEAAHH_Z(CListBox* pThis, int nSelect) {
    if (!pThis || !pThis->m_hWnd) return LB_ERR;
    return (int)::SendMessageW(pThis->m_hWnd, LB_SETCURSEL, nSelect, 0);
}
extern "C" int MS_ABI impl__GetText_CListBox__QEBAHPEA_WH_Z(const CListBox* pThis, int nIndex, wchar_t* lpszBuffer) {
    if (!pThis || !pThis->m_hWnd || !lpszBuffer) return LB_ERR;
    return (int)::SendMessageW(pThis->m_hWnd, LB_GETTEXT, nIndex, (LPARAM)lpszBuffer);
}
extern "C" int MS_ABI impl__AddString_CListBox__QEAAHPEB_W_Z(CListBox* pThis, const wchar_t* lpszItem) {
    if (!pThis || !pThis->m_hWnd) return LB_ERR;
    return (int)::SendMessageW(pThis->m_hWnd, LB_ADDSTRING, 0, (LPARAM)lpszItem);
}
extern "C" int MS_ABI impl__InsertString_CListBox__QEAAHHPEB_W_Z(CListBox* pThis, int nIndex, const wchar_t* lpszItem) {
    if (!pThis || !pThis->m_hWnd) return LB_ERR;
    return (int)::SendMessageW(pThis->m_hWnd, LB_INSERTSTRING, nIndex, (LPARAM)lpszItem);
}
extern "C" int MS_ABI impl__DeleteString_CListBox__QEAAHI_Z(CListBox* pThis, unsigned int nIndex) {
    if (!pThis || !pThis->m_hWnd) return LB_ERR;
    return (int)::SendMessageW(pThis->m_hWnd, LB_DELETESTRING, nIndex, 0);
}
extern "C" void MS_ABI impl__ResetContent_CListBox__QEAAXXZ(CListBox* pThis) {
    if (pThis && pThis->m_hWnd) {
        ::SendMessageW(pThis->m_hWnd, LB_RESETCONTENT, 0, 0);
    }
}
extern "C" int MS_ABI impl__FindString_CListBox__QEBAHPEB_WH_Z(const CListBox* pThis, const wchar_t* lpszItem, int nStartAfter) {
    if (!pThis || !pThis->m_hWnd) return LB_ERR;
    return (int)::SendMessageW(pThis->m_hWnd, LB_FINDSTRING, nStartAfter, (LPARAM)lpszItem);
}
extern "C" int MS_ABI impl__SelectString_CListBox__QEAAHPEB_WH_Z(CListBox* pThis, int nStartAfter, const wchar_t* lpszItem) {
    if (!pThis || !pThis->m_hWnd) return LB_ERR;
    return (int)::SendMessageW(pThis->m_hWnd, LB_SELECTSTRING, nStartAfter, (LPARAM)lpszItem);
}
extern "C" uintptr_t MS_ABI impl__GetItemData_CListBox__QEBA_KH_Z(const CListBox* pThis, int nIndex) {
    if (!pThis || !pThis->m_hWnd) return 0;
    return (uintptr_t)::SendMessageW(pThis->m_hWnd, LB_GETITEMDATA, nIndex, 0);
}
extern "C" int MS_ABI impl__SetItemData_CListBox__QEAAHH_K_Z(CListBox* pThis, int nIndex, uintptr_t dwItemData) {
    if (!pThis || !pThis->m_hWnd) return LB_ERR;
    return (int)::SendMessageW(pThis->m_hWnd, LB_SETITEMDATA, nIndex, (LPARAM)dwItemData);
}
extern "C" void* MS_ABI impl__GetItemDataPtr_CListBox__QEBAPEAXH_Z(const CListBox* pThis, int nIndex) {
    if (!pThis || !pThis->m_hWnd) return nullptr;
    return (void*)::SendMessageW(pThis->m_hWnd, LB_GETITEMDATA, nIndex, 0);
}
extern "C" int MS_ABI impl__SetItemDataPtr_CListBox__QEAAHHPEAX_Z(CListBox* pThis, int nIndex, void* pData) {
    if (!pThis || !pThis->m_hWnd) return LB_ERR;
    return (int)::SendMessageW(pThis->m_hWnd, LB_SETITEMDATA, nIndex, (LPARAM)pData);
}
extern "C" int MS_ABI impl__Create_CComboBox__UEAAHKAEBUtagRECT__PEAVCWnd__I_Z(
    CComboBox* pThis, DWORD dwStyle, const RECT* pRect, CWnd* pParentWnd, UINT nID) {
    if (!pThis || !pRect) return FALSE;
    return impl__Create_CWnd__UEAAHPEB_W0KAEBUtagRECT__PEAV1_IPEAUCCreateContext___Z(
        pThis, L"COMBOBOX", L"", dwStyle, *pRect, pParentWnd, nID, nullptr);
}
extern "C" int MS_ABI impl__GetCount_CComboBox__QEBAHXZ(const CComboBox* pThis) {
    if (!pThis || !pThis->m_hWnd) return 0;
    return (int)::SendMessageW(pThis->m_hWnd, CB_GETCOUNT, 0, 0);
}
extern "C" int MS_ABI impl__GetCurSel_CComboBox__QEBAHXZ(const CComboBox* pThis) {
    if (!pThis || !pThis->m_hWnd) return CB_ERR;
    return (int)::SendMessageW(pThis->m_hWnd, CB_GETCURSEL, 0, 0);
}
extern "C" int MS_ABI impl__SetCurSel_CComboBox__QEAAHH_Z(CComboBox* pThis, int nSelect) {
    if (!pThis || !pThis->m_hWnd) return CB_ERR;
    return (int)::SendMessageW(pThis->m_hWnd, CB_SETCURSEL, nSelect, 0);
}
extern "C" int MS_ABI impl__GetLBText_CComboBox__QEBAHPEA_WH_Z(const CComboBox* pThis, int nIndex, wchar_t* lpszText) {
    if (!pThis || !pThis->m_hWnd || !lpszText) return CB_ERR;
    return (int)::SendMessageW(pThis->m_hWnd, CB_GETLBTEXT, nIndex, (LPARAM)lpszText);
}
extern "C" int MS_ABI impl__GetLBTextLen_CComboBox__QEBAHH_Z(const CComboBox* pThis, int nIndex) {
    if (!pThis || !pThis->m_hWnd) return CB_ERR;
    return (int)::SendMessageW(pThis->m_hWnd, CB_GETLBTEXTLEN, nIndex, 0);
}
extern "C" int MS_ABI impl__AddString_CComboBox__QEAAHPEB_W_Z(CComboBox* pThis, const wchar_t* lpszString) {
    if (!pThis || !pThis->m_hWnd) return CB_ERR;
    return (int)::SendMessageW(pThis->m_hWnd, CB_ADDSTRING, 0, (LPARAM)lpszString);
}
extern "C" int MS_ABI impl__InsertString_CComboBox__QEAAHHPEB_W_Z(CComboBox* pThis, int nIndex, const wchar_t* lpszString) {
    if (!pThis || !pThis->m_hWnd) return CB_ERR;
    return (int)::SendMessageW(pThis->m_hWnd, CB_INSERTSTRING, nIndex, (LPARAM)lpszString);
}
extern "C" int MS_ABI impl__DeleteString_CComboBox__QEAAHI_Z(CComboBox* pThis, unsigned int nIndex) {
    if (!pThis || !pThis->m_hWnd) return CB_ERR;
    return (int)::SendMessageW(pThis->m_hWnd, CB_DELETESTRING, nIndex, 0);
}
extern "C" void MS_ABI impl__ResetContent_CComboBox__QEAAXXZ(CComboBox* pThis) {
    if (pThis && pThis->m_hWnd) {
        ::SendMessageW(pThis->m_hWnd, CB_RESETCONTENT, 0, 0);
    }
}
extern "C" int MS_ABI impl__FindString_CComboBox__QEBAHPEB_WH_Z(const CComboBox* pThis, const wchar_t* lpszString, int nStartAfter) {
    if (!pThis || !pThis->m_hWnd) return CB_ERR;
    return (int)::SendMessageW(pThis->m_hWnd, CB_FINDSTRING, nStartAfter, (LPARAM)lpszString);
}
extern "C" int MS_ABI impl__SelectString_CComboBox__QEAAHPEB_WH_Z(CComboBox* pThis, int nStartAfter, const wchar_t* lpszString) {
    if (!pThis || !pThis->m_hWnd) return CB_ERR;
    return (int)::SendMessageW(pThis->m_hWnd, CB_SELECTSTRING, nStartAfter, (LPARAM)lpszString);
}
extern "C" uintptr_t MS_ABI impl__GetItemData_CComboBox__QEBA_KH_Z(const CComboBox* pThis, int nIndex) {
    if (!pThis || !pThis->m_hWnd) return 0;
    return (uintptr_t)::SendMessageW(pThis->m_hWnd, CB_GETITEMDATA, nIndex, 0);
}
extern "C" int MS_ABI impl__SetItemData_CComboBox__QEAAHH_K_Z(CComboBox* pThis, int nIndex, uintptr_t dwItemData) {
    if (!pThis || !pThis->m_hWnd) return CB_ERR;
    return (int)::SendMessageW(pThis->m_hWnd, CB_SETITEMDATA, nIndex, (LPARAM)dwItemData);
}
extern "C" void MS_ABI impl__ShowDropDown_CComboBox__QEAAXH_Z(CComboBox* pThis, int bShowIt) {
    if (pThis && pThis->m_hWnd) {
        ::SendMessageW(pThis->m_hWnd, CB_SHOWDROPDOWN, bShowIt ? TRUE : FALSE, 0);
    }
}
extern "C" int MS_ABI impl__GetDroppedState_CComboBox__QEBAHXZ(const CComboBox* pThis) {
    if (!pThis || !pThis->m_hWnd) return FALSE;
    return (int)::SendMessageW(pThis->m_hWnd, CB_GETDROPPEDSTATE, 0, 0);
}
std::unordered_map<const CSplitButton*, HMENU> g_splitMenus;
std::unordered_map<const CDragListBox*, int> g_dragSourceItem;
int ListBoxItemFromPoint_Ctrlcore(HWND hWnd, CPoint pt) {
    if (!hWnd) return LB_ERR;
    DWORD hit = (DWORD)::SendMessageW(hWnd, LB_ITEMFROMPOINT, 0, MAKELPARAM(pt.x, pt.y));
    if (HIWORD(hit) != 0) return LB_ERR;
    return (int)LOWORD(hit);
}
extern "C" int MS_ABI impl__Create_CScrollBar__UEAAHKAEBUtagRECT__PEAVCWnd__I_Z(
    CScrollBar* pThis, DWORD dwStyle, const RECT* pRect, CWnd* pParentWnd, UINT nID) {
    if (!pThis || !pRect) return FALSE;
    return impl__Create_CWnd__UEAAHPEB_W0KAEBUtagRECT__PEAV1_IPEAUCCreateContext___Z(
        pThis, L"SCROLLBAR", L"", dwStyle, *pRect, pParentWnd, nID, nullptr);
}
extern "C" int MS_ABI impl__GetScrollPos_CScrollBar__QEBAHXZ(const CScrollBar* pThis) {
    if (!pThis || !pThis->m_hWnd) return 0;
    return ::GetScrollPos(pThis->m_hWnd, SB_CTL);
}
extern "C" int MS_ABI impl__SetScrollPos_CScrollBar__QEAAHHH_Z(CScrollBar* pThis, int nPos, int bRedraw) {
    if (!pThis || !pThis->m_hWnd) return 0;
    return ::SetScrollPos(pThis->m_hWnd, SB_CTL, nPos, bRedraw ? TRUE : FALSE);
}
extern "C" void MS_ABI impl__GetScrollRange_CScrollBar__QEBAXPEAH0_Z(const CScrollBar* pThis, int* lpMinPos, int* lpMaxPos) {
    if (!pThis || !pThis->m_hWnd) {
        if (lpMinPos) *lpMinPos = 0;
        if (lpMaxPos) *lpMaxPos = 0;
        return;
    }
    ::GetScrollRange(pThis->m_hWnd, SB_CTL, lpMinPos, lpMaxPos);
}
extern "C" void MS_ABI impl__SetScrollRange_CScrollBar__QEAAXHHH_Z(CScrollBar* pThis, int nMinPos, int nMaxPos, int bRedraw) {
    if (pThis && pThis->m_hWnd) {
        ::SetScrollRange(pThis->m_hWnd, SB_CTL, nMinPos, nMaxPos, bRedraw ? TRUE : FALSE);
    }
}
extern "C" int MS_ABI impl__GetScrollInfo_CScrollBar__QEBAHPEAUSCROLLINFO__I_Z(const CScrollBar* pThis, SCROLLINFO* lpScrollInfo, unsigned int nMask) {
    if (!pThis || !pThis->m_hWnd || !lpScrollInfo) return FALSE;
    lpScrollInfo->cbSize = sizeof(SCROLLINFO);
    lpScrollInfo->fMask = nMask;
    return ::GetScrollInfo(pThis->m_hWnd, SB_CTL, lpScrollInfo);
}
extern "C" int MS_ABI impl__SetScrollInfo_CScrollBar__QEAAHPEAUSCROLLINFO__H_Z(CScrollBar* pThis, SCROLLINFO* lpScrollInfo, int bRedraw) {
    if (!pThis || !pThis->m_hWnd || !lpScrollInfo) return 0;
    lpScrollInfo->cbSize = sizeof(SCROLLINFO);
    return ::SetScrollInfo(pThis->m_hWnd, SB_CTL, lpScrollInfo, bRedraw ? TRUE : FALSE);
}
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
} } }  // namespace openmfc::detail::ctrlcore
