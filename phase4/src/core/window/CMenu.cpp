// CMenu — OpenMFC implementation.
// Sources: menucore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/CMenuSupport.h"

// Symbol: ?CreateObject@CMenu@@SAPEAVCObject@@XZ
// Symbol: ?GetRuntimeClass@CMenu@@UEBAPEAUCRuntimeClass@@XZ
// Symbol: ?GetThisClass@CMenu@@SAPEAUCRuntimeClass@@XZ

#ifdef __GNUC__
// MSVC symbol alias for CMenu::classCMenu
asm(".globl \"?classCMenu@CMenu@@2UCRuntimeClass@@A\"\n"
    ".set \"?classCMenu@CMenu@@2UCRuntimeClass@@A\", _ZN5CMenu10classCMenuE\n");
#endif

// =============================================================================
// CMenu Implementation
// =============================================================================

extern "C" void MS_ABI impl___1CMenu__UEAA_XZ(CMenu* pThis);
// CMenu::DestroyMenu
// Symbol: ?DestroyMenu@CMenu@@QEAAHXZ
// Ordinal: 3803
extern "C" int MS_ABI impl__DestroyMenu_CMenu__QEAAHXZ(CMenu* pThis) {
    if (!pThis || !pThis->m_hMenu) return FALSE;
    UnregisterPermanentMenu(pThis);
    int result = ::DestroyMenu(pThis->m_hMenu);
    UnmarkOwnedMenu(pThis->m_hMenu);
    pThis->m_hMenu = nullptr;
    return result;
}
// CMenu::Attach
// Symbol: ?Attach@CMenu@@QEAAHPEAUHMENU__@@@Z
// Ordinal: 2479
extern "C" int MS_ABI impl__Attach_CMenu__QEAAHPEAUHMENU_____Z(CMenu* pThis, HMENU hMenu) {
    if (!pThis) return FALSE;
    UnregisterPermanentMenu(pThis);
    if (pThis->m_hMenu && IsOwnedMenu(pThis->m_hMenu)) {
        ::DestroyMenu(pThis->m_hMenu);
        UnmarkOwnedMenu(pThis->m_hMenu);
    }
    pThis->m_hMenu = hMenu;
    MarkOwnedMenu(pThis->m_hMenu);
    RegisterPermanentMenu(pThis);
    return TRUE;
}
// CMenu::Detach
// Symbol: ?Detach@CMenu@@QEAAPEAUHMENU__@@XZ
// Ordinal: 3832
extern "C" HMENU MS_ABI impl__Detach_CMenu__QEAAPEAUHMENU____XZ(CMenu* pThis) {
    if (!pThis) return nullptr;
    HMENU h = pThis->m_hMenu;
    UnregisterPermanentMenu(pThis);
    UnmarkOwnedMenu(h);
    pThis->m_hMenu = nullptr;
    return h;
}
// CMenu::FromHandle
// Symbol: ?FromHandle@CMenu@@SAPEAV1@PEAUHMENU__@@@Z
extern "C" CMenu* MS_ABI impl__FromHandle_CMenu__SAPEAV1_PEAUHMENU_____Z(HMENU hMenu) {
    if (CMenu* permanent = FindPermanentMenu(hMenu)) {
        return permanent;
    }
    return FromHandleTemp(hMenu);
}
// CMenu::FromHandlePermanent
// Symbol: ?FromHandlePermanent@CMenu@@SAPEAV1@PEAUHMENU__@@@Z
extern "C" CMenu* MS_ABI impl__FromHandlePermanent_CMenu__SAPEAV1_PEAUHMENU_____Z(HMENU hMenu) {
    return FindPermanentMenu(hMenu);
}
// CMenu::DeleteTempMap
// Symbol: ?DeleteTempMap@CMenu@@SAXXZ
extern "C" void MS_ABI impl__DeleteTempMap_CMenu__SAXXZ() {
    DeleteTempMenuMap();
}
// CMenu::TrackPopupMenu
// Symbol: ?TrackPopupMenu@CMenu@@QEAAHIHHPEAVCWnd@@PEBUtagRECT@@@Z
// Ordinal: 13999
extern "C" int MS_ABI impl__TrackPopupMenu_CMenu__QEAAHIHHPEAVCWnd__PEBUtagRECT___Z(
    CMenu* pThis, UINT nFlags, int x, int y, CWnd* pWnd, const RECT* lpRect) {
    if (!pThis || !pThis->m_hMenu) return FALSE;
    HWND hWnd = pWnd ? pWnd->GetSafeHwnd() : nullptr;
    return ::TrackPopupMenu(pThis->m_hMenu, nFlags, x, y, 0, hWnd, lpRect);
}
// CMenu::TrackPopupMenuEx
// Symbol: ?TrackPopupMenuEx@CMenu@@QEAAHIHHPEAVCWnd@@PEAUtagTPMPARAMS@@@Z
// Ordinal: 14000
extern "C" int MS_ABI impl__TrackPopupMenuEx_CMenu__QEAAHIHHPEAVCWnd__PEAUtagTPMPARAMS___Z(
    CMenu* pThis, UINT nFlags, int x, int y, CWnd* pWnd, LPTPMPARAMS lpParams) {
    if (!pThis || !pThis->m_hMenu) return FALSE;
    HWND hWnd = pWnd ? pWnd->GetSafeHwnd() : nullptr;
    return ::TrackPopupMenuEx(pThis->m_hMenu, nFlags, x, y, hWnd, lpParams);
}
// CMenu::GetMenuStringW (CString overload)
// Symbol: ?GetMenuStringW@CMenu@@QEBAHIAEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@I@Z
extern "C" int MS_ABI impl__GetMenuStringW_CMenu__QEBAHIAEAV_CString_I_Z(const CMenu* pThis, UINT nIDItem, CString& rString, UINT nFlags) {
    if (!pThis || !pThis->m_hMenu) {
        rString.Empty();
        return 0;
    }

    int len = ::GetMenuStringW(pThis->m_hMenu, nIDItem, nullptr, 0, nFlags);
    if (len <= 0) {
        rString.Empty();
        return 0;
    }

    wchar_t* buffer = rString.GetBuffer(len);
    int copied = ::GetMenuStringW(pThis->m_hMenu, nIDItem, buffer, len + 1, nFlags);
    rString.ReleaseBuffer((copied >= 0) ? copied : 0);
    return (copied >= 0) ? copied : 0;
}
CMenu::~CMenu() {
    impl___1CMenu__UEAA_XZ(this);
}
void CMenu::DrawItem(void* lpDrawItemStruct) {
    DRAWITEMSTRUCT* dis = static_cast<DRAWITEMSTRUCT*>(lpDrawItemStruct);
    if (!dis || !dis->hDC) {
        return;
    }

    HDC hdc = dis->hDC;
    const bool selected = (dis->itemState & ODS_SELECTED) != 0;
    const bool disabled = (dis->itemState & (ODS_DISABLED | ODS_GRAYED)) != 0;
    const bool checked = (dis->itemState & ODS_CHECKED) != 0;

    HBRUSH bgBrush = ::GetSysColorBrush(selected ? COLOR_HIGHLIGHT : COLOR_MENU);
    ::FillRect(hdc, &dis->rcItem, bgBrush);

    const int oldBkMode = ::SetBkMode(hdc, TRANSPARENT);
    const COLORREF oldTextColor = ::GetTextColor(hdc);
    HFONT hOldFont = nullptr;
    HFONT hMenuFont = static_cast<HFONT>(::GetStockObject(DEFAULT_GUI_FONT));
    if (hMenuFont) {
        hOldFont = static_cast<HFONT>(::SelectObject(hdc, hMenuFont));
    }

    COLORREF textColor = ::GetSysColor(disabled ? COLOR_GRAYTEXT : (selected ? COLOR_HIGHLIGHTTEXT : COLOR_MENUTEXT));
    ::SetTextColor(hdc, textColor);

    RECT rcItem = dis->rcItem;
    RECT rcCheck = rcItem;
    RECT rcText = rcItem;
    const int checkWidth = ::GetSystemMetrics(SM_CXMENUCHECK);
    rcCheck.right = rcCheck.left + checkWidth;
    rcText.left = rcCheck.right + 2;

    if (checked) {
        UINT state = DFCS_MENUCHECK;
        if (disabled) {
            state |= DFCS_INACTIVE;
        }
        ::DrawFrameControl(hdc, &rcCheck, DFC_MENU, state);
    }

    wchar_t textBuffer[256] = {};
    if (dis->itemID != static_cast<UINT>(-1) && dis->hwndItem != nullptr) {
        HMENU hMenu = reinterpret_cast<HMENU>(dis->hwndItem);
        ::GetMenuStringW(hMenu, dis->itemID, textBuffer, static_cast<int>(sizeof(textBuffer) / sizeof(textBuffer[0])), MF_BYCOMMAND);
    } else if (dis->itemData != 0) {
        const wchar_t* pText = reinterpret_cast<const wchar_t*>(dis->itemData);
        if (pText) {
            wcsncpy(textBuffer, pText, (sizeof(textBuffer) / sizeof(textBuffer[0])) - 1);
            textBuffer[(sizeof(textBuffer) / sizeof(textBuffer[0])) - 1] = L'\0';
        }
    }

    if (textBuffer[0] != L'\0') {
        ::DrawTextW(hdc, textBuffer, -1, &rcText, DT_SINGLELINE | DT_VCENTER | DT_LEFT | DT_NOPREFIX);
    }

    if (dis->itemState & ODS_FOCUS) {
        ::DrawFocusRect(hdc, &rcItem);
    }

    if (hOldFont) {
        ::SelectObject(hdc, hOldFont);
    }
    ::SetBkMode(hdc, oldBkMode);
    ::SetTextColor(hdc, oldTextColor);
}
void CMenu::MeasureItem(void* lpMeasureItemStruct) {
    MEASUREITEMSTRUCT* mis = static_cast<MEASUREITEMSTRUCT*>(lpMeasureItemStruct);
    if (!mis) {
        return;
    }

    if (mis->itemWidth == 0) {
        mis->itemWidth = ::GetSystemMetrics(SM_CXMENUCHECK) + 16;
    }
    if (mis->itemHeight == 0) {
        mis->itemHeight = ::GetSystemMetrics(SM_CYMENU);
    }
}
int CMenu::CreatePopupMenu() {
    return impl__CreatePopupMenu_CMenu__QEAAHXZ(this);
}
int CMenu::AppendMenu(UINT nFlags, UINT_PTR nIDNewItem, const wchar_t* lpszNewItem) {
    return m_hMenu ? ::AppendMenuW(m_hMenu, nFlags, nIDNewItem, lpszNewItem) : FALSE;
}
CMenu* CMenu::FromHandle(HMENU hMenu) {
    return impl__FromHandle_CMenu__SAPEAV1_PEAUHMENU_____Z(hMenu);
}
