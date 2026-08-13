// CComboBoxEx — OpenMFC implementation.
// Sources: typed_stubs-generated baseline; Win32 combo-box-ex wrappers.

#define OPENMFC_APPCORE_IMPL

#include "openmfc/afxwin.h"
#include "openmfc/afxmfc.h"
#include <windows.h>
#include <commctrl.h>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

namespace {
HWND ComboBoxExHwnd(const CWnd* pThis) {
    return pThis ? pThis->GetSafeHwnd() : nullptr;
}
}

// Symbol: ?Create@CComboBoxEx@@UEAAHKAEBUtagRECT@@PEAVCWnd@@I@Z
extern "C" int MS_ABI impl__Create_CComboBoxEx__UEAAHKAEBUtagRECT__PEAVCWnd__I_Z(
    CWnd* pThis, DWORD dwStyle, const RECT* pRect, CWnd* pParentWnd, UINT nID) {
    if (!pThis || !pRect) return FALSE;
    return pThis->CWnd::Create(WC_COMBOBOXEXW, L"", dwStyle, *pRect, pParentWnd, nID, nullptr);
}

// Symbol: ?CreateEx@CComboBoxEx@@UEAAHKKAEBUtagRECT@@PEAVCWnd@@I@Z
extern "C" int MS_ABI impl__CreateEx_CComboBoxEx__UEAAHKKAEBUtagRECT__PEAVCWnd__I_Z(
    CWnd* pThis, DWORD dwExStyle, DWORD dwStyle, const RECT* pRect, CWnd* pParentWnd, UINT nID) {
    if (!pThis || !pRect) return FALSE;
    return pThis->CWnd::CreateEx(dwExStyle, WC_COMBOBOXEXW, L"", dwStyle,
        pRect->left, pRect->top, pRect->right - pRect->left, pRect->bottom - pRect->top,
        pParentWnd ? pParentWnd->GetSafeHwnd() : nullptr, (HMENU)(UINT_PTR)nID, nullptr);
}

// Symbol: ?DeleteItem@CComboBoxEx@@QEAAHH@Z
extern "C" int MS_ABI impl__DeleteItem_CComboBoxEx__QEAAHH_Z(
    CWnd* pThis, int iIndex) {
    return (int)::SendMessageW(ComboBoxExHwnd(pThis), CBEM_DELETEITEM, (WPARAM)iIndex, 0);
}

// Symbol: ?GetItem@CComboBoxEx@@QEAAHPEAUtagCOMBOBOXEXITEMW@@@Z
extern "C" int MS_ABI impl__GetItem_CComboBoxEx__QEAAHPEAUtagCOMBOBOXEXITEMW___Z(
    CWnd* pThis, COMBOBOXEXITEMW* pItem) {
    if (!pItem) return FALSE;
    return (int)::SendMessageW(ComboBoxExHwnd(pThis), CBEM_GETITEMW, 0, (LPARAM)pItem);
}

// Symbol: ?InsertItem@CComboBoxEx@@QEAAHPEBUtagCOMBOBOXEXITEMW@@@Z
extern "C" int MS_ABI impl__InsertItem_CComboBoxEx__QEAAHPEBUtagCOMBOBOXEXITEMW___Z(
    CWnd* pThis, const COMBOBOXEXITEMW* pItem) {
    if (!pItem) return -1;
    return (int)::SendMessageW(ComboBoxExHwnd(pThis), CBEM_INSERTITEMW, 0, (LPARAM)pItem);
}

// Symbol: ?SetItem@CComboBoxEx@@QEAAHPEBUtagCOMBOBOXEXITEMW@@@Z
extern "C" int MS_ABI impl__SetItem_CComboBoxEx__QEAAHPEBUtagCOMBOBOXEXITEMW___Z(
    CWnd* pThis, const COMBOBOXEXITEMW* pItem) {
    if (!pItem) return FALSE;
    return (int)::SendMessageW(ComboBoxExHwnd(pThis), CBEM_SETITEMW, 0, (LPARAM)pItem);
}
