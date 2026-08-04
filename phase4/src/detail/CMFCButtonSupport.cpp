#include "CMFCButtonSupport.h"

static_assert(sizeof(S_Cmfcbutton) == 0xb20, "CMFCButton must be 0xB20 bytes");
static_assert(offsetof(S_Cmfcbutton, m_hWnd) == 0x40, "m_hWnd @0x40");
static_assert(offsetof(S_Cmfcbutton, m_bHover) == 0x114, "m_bHover @0x114");
static_assert(offsetof(S_Cmfcbutton, m_bClickStarted) == 0x11c, "m_bClickStarted @0x11c");
static_assert(offsetof(S_Cmfcbutton, m_bChecked) == 0x128, "m_bChecked @0x128");
static_assert(offsetof(S_Cmfcbutton, m_bRadioStyle) == 0x12c, "m_bRadioStyle @0x12c");
static_assert(offsetof(S_Cmfcbutton, m_bCheckStyle) == 0x130, "m_bCheckStyle @0x130");
static_assert(offsetof(S_Cmfcbutton, m_bAutoToggle) == 0x134, "m_bAutoToggle @0x134");
static_assert(offsetof(S_Cmfcbutton, m_bDrawFocusA) == 0x13c, "m_bDrawFocusA @0x13c");
static_assert(offsetof(S_Cmfcbutton, m_bDrawFocusB) == 0x140, "m_bDrawFocusB @0x140");
static_assert(offsetof(S_Cmfcbutton, m_nTimerID) == 0x144, "m_nTimerID @0x144");
static_assert(offsetof(S_Cmfcbutton, m_sizeImageCX) == 0x148, "m_sizeImage @0x148");
static_assert(offsetof(S_Cmfcbutton, m_Images) == 0x158, "m_Images @0x158");
static_assert(offsetof(S_Cmfcbutton, m_nImageIndex) == 0xae8, "m_nImageIndex @0xae8");
static_assert(offsetof(S_Cmfcbutton, pHelperWnd) == 0xaf8, "pHelperWnd @0xaf8");
static_assert(offsetof(S_Cmfcbutton, m_hCursor) == 0xb08, "m_hCursor @0xb08");
namespace openmfc { namespace detail { namespace cmfcbutton {
void UncheckRadioButtonsInGroupImpl_Cmfcbutton(S_Cmfcbutton* s) {
    HWND hwndThis = s->m_hWnd;
    HWND hParent = ::GetParent(hwndThis);
    void* pParent = impl__FromHandlePermanent_CWnd__SAPEAV1_PEAUHWND_____Z(hParent);
    if (pParent == nullptr) return;
    if (!::IsWindowEnabled(hwndThis)) return;

    bool tabStopChanged = false;
    HWND hwndCurrent = hwndThis;
    for (;;) {
        HWND hwndNext = ::GetNextDlgGroupItem(hParent, hwndCurrent, FALSE);
        if (hwndNext == nullptr || hwndNext == hwndThis) break;
        // Retail requires the sibling to be a CMFCButton (IsKindOf) with
        // m_bCheckStyle set; approximated by resolving the HWND and reading
        // m_bCheckStyle (0x130) directly.
        void* pObj = impl__FromHandlePermanent_CWnd__SAPEAV1_PEAUHWND_____Z(hwndNext);
        if (pObj != nullptr &&
            *reinterpret_cast<int*>(reinterpret_cast<char*>(pObj) + 0x130) != 0) {
            LONG_PTR style = ::GetWindowLongPtrW(hwndNext, GWL_STYLE);
            if ((style & WS_TABSTOP) != 0) {
                ::SetWindowLongPtrW(hwndNext, GWL_STYLE, style & ~WS_TABSTOP);
                tabStopChanged = true;
            }
            if (*reinterpret_cast<int*>(reinterpret_cast<char*>(pObj) + 0x128) != 0) {
                *reinterpret_cast<int*>(reinterpret_cast<char*>(pObj) + 0x128) = 0;
                ::RedrawWindow(hwndNext, nullptr, nullptr,
                               RDW_INVALIDATE | RDW_ERASE | RDW_UPDATENOW);
            }
        }
        hwndCurrent = hwndNext;
    }
    if (tabStopChanged) {
        LONG_PTR style = ::GetWindowLongPtrW(hwndThis, GWL_STYLE);
        ::SetWindowLongPtrW(hwndThis, GWL_STYLE, style | WS_TABSTOP);
    }
}
} } }  // namespace openmfc::detail::cmfcbutton
