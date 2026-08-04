#include "MfcFeature34ImplSupport.h"

static_assert(sizeof(S_MfcFeature34Impl) == 0xb20, "CMFCButton must be 0xB20 bytes");
static_assert(offsetof(S_MfcFeature34Impl, vfptr) == 0x00, "vfptr @0x00");
static_assert(offsetof(S_MfcFeature34Impl, m_dwRef) == 0x08, "m_dwRef @0x08");
static_assert(offsetof(S_MfcFeature34Impl, m_hWnd) == 0x40, "m_hWnd @0x40");
static_assert(offsetof(S_MfcFeature34Impl, m_nStyle) == 0xe8, "m_nStyle @0xe8");
static_assert(offsetof(S_MfcFeature34Impl, m_nFlatStyle) == 0xec, "m_nFlatStyle @0xec");
static_assert(offsetof(S_MfcFeature34Impl, m_bCheckStyle) == 0x130, "m_bCheckStyle @0x130");
static_assert(offsetof(S_MfcFeature34Impl, m_bAutoToggle) == 0x134, "m_bAutoToggle @0x134");
static_assert(offsetof(S_MfcFeature34Impl, m_hIcon) == 0x148, "m_hIcon @0x148");
static_assert(offsetof(S_MfcFeature34Impl, m_Images) == 0x158, "m_Images @0x158");
static_assert(offsetof(S_MfcFeature34Impl, m_nImageIndex) == 0xae8, "m_nImageIndex @0xae8");
static_assert(offsetof(S_MfcFeature34Impl, m_nFocusColor) == 0xb18, "m_nFocusColor @0xb18");
namespace openmfc { namespace detail { namespace mfcfeature34impl {
void UncheckRadioButtonsInGroupImpl_MfcFeature34Impl(S_MfcFeature34Impl* s) {
    if (s == nullptr) return;
    HWND hwndThis = s->m_hWnd;
    if (hwndThis == nullptr) return;
    HWND hParent = ::GetParent(hwndThis);
    if (hParent == nullptr) return;
    if (!::IsWindowEnabled(hwndThis)) return;

    bool tabStopChanged = false;
    HWND hwndCurrent = hwndThis;
    for (;;) {
        HWND hwndNext = ::GetNextDlgGroupItem(hParent, hwndCurrent, FALSE);
        // The retail resolves the sibling through CWnd::FromHandlePermanent and
        // requires it to be a CMFCButton (IsKindOf(RUNTIME_CLASS(CMFCButton)))
        // with m_bCheckStyle set.  We approximate the CMFCButton test by
        // resolving the HWND through FromHandlePermanent and reading
        // m_bCheckStyle (offset 0x130) directly.
        if (hwndNext == nullptr || hwndNext == hwndThis) break;
        HWND hwndBtn = hwndNext;
        void* pObj = impl__FromHandlePermanent_CWnd__SAPEAV1_PEAUHWND_____Z(hwndBtn);
        if (pObj != nullptr && *reinterpret_cast<int*>(reinterpret_cast<char*>(pObj) + 0x130) != 0) {
            LONG_PTR style = ::GetWindowLongPtrW(hwndBtn, GWL_STYLE);
            if ((style & WS_TABSTOP) != 0) {
                ::SetWindowLongPtrW(hwndBtn, GWL_STYLE, style & ~WS_TABSTOP);
                tabStopChanged = true;
            }
            if (*reinterpret_cast<int*>(reinterpret_cast<char*>(pObj) + 0x128) != 0) {
                *reinterpret_cast<int*>(reinterpret_cast<char*>(pObj) + 0x128) = 0;
                ::RedrawWindow(hwndBtn, nullptr, nullptr,
                               RDW_INVALIDATE | RDW_ERASE | RDW_UPDATENOW);
            }
        }
        hwndCurrent = hwndBtn;
    }
    if (tabStopChanged) {
        LONG_PTR style = ::GetWindowLongPtrW(hwndThis, GWL_STYLE);
        ::SetWindowLongPtrW(hwndThis, GWL_STYLE, style | WS_TABSTOP);
    }
}
} } }  // namespace openmfc::detail::mfcfeature34impl
