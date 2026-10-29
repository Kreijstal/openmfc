// CDockablePaneAdapter — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp

#include "detail/ManualSmallStubImplementationsSupport.h"

#include <cstddef>

// CDockablePaneAdapter is not declared in include/openmfc. Mirror only the
// observed retail tail storage; the base portion is opaque here. The constructor
// at RVA 0x468a0 (mfc140u) sets the RECT at +0x4e0 to (30, 30, 180, 180) via
// USER32!SetRect (IAT slot 0x1802c7140, mfc140u), writes 0xf000 at +0x4f0,
// and clears the pointer-sized field at +0x4f8.
struct S_CDockablePaneAdapterLayout {
    unsigned char base[0x4e0];
    unsigned char rectInitial[0x10];
    DWORD alignmentInitial;
    void* wrappedWnd;
};
static_assert(offsetof(S_CDockablePaneAdapterLayout, rectInitial) == 0x4e0,
              "CDockablePaneAdapter tail storage offset");
static_assert(offsetof(S_CDockablePaneAdapterLayout, alignmentInitial) == 0x4f0,
              "CDockablePaneAdapter alignment storage offset");
static_assert(offsetof(S_CDockablePaneAdapterLayout, wrappedWnd) == 0x4f8,
              "CDockablePaneAdapter wrapped-window pointer offset");
static_assert(sizeof(S_CDockablePaneAdapterLayout) == 0x500,
              "CDockablePaneAdapter retail allocation size");

extern "C" void MS_ABI impl___1CDockablePane__UEAA_XZ(void* pThis);
extern "C" void MS_ABI impl__OnSize_CBasePane__IEAAXIHH_Z(
    CBasePane* pThis, unsigned int nType, int cx, int cy);
extern "C" int MS_ABI impl__SetWindowPos_CWnd__QEAAHPEBV1_HHHHI_Z(
    CWnd* pThis, const CWnd* pWndInsertAfter, int x, int y, int cx, int cy,
    unsigned int nFlags);
extern "C" CString* MS_ABI impl__AFXGetRegPath__YA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__PEB_W0_Z(
    CString* pRet, const wchar_t* lpszPostFix, const wchar_t* lpszProfileName);
extern "C" void* MS_ABI impl__Create_CSettingsStoreSP__QEAAAEAVCSettingsStore__HH_Z(
    void* pThis, int bAdmin, int bReadOnly);
extern "C" int MS_ABI impl__Open_CSettingsStore__UEAAHPEB_W_Z(
    void* pThis, const wchar_t* lpszPath);
extern "C" int MS_ABI impl__CreateKey_CSettingsStore__UEAAHPEB_W_Z(
    void* pThis, const wchar_t* lpszKey);
extern "C" int MS_ABI impl__Read_CSettingsStore__UEAAHPEB_WAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
    void* pThis, const wchar_t* lpszValueName, CString* pValue);
extern "C" int MS_ABI impl__Write_CSettingsStore__UEAAHPEB_W0_Z(
    void* pThis, const wchar_t* lpszValueName, const wchar_t* lpszValue);
extern "C" int MS_ABI impl__GetDlgCtrlID_CWnd__QEBAHXZ(const CWnd* pThis);
extern "C" void MS_ABI impl__GetWindowTextW_CWnd__QEBAXAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
    const CWnd* pThis, CString* pString);
extern "C" void MS_ABI impl__SetWindowTextW_CWnd__QEAAXPEB_W_Z(
    CWnd* pThis, const wchar_t* lpszString);
extern "C" int MS_ABI impl__LoadState_CPane__UEAAHPEB_WHI_Z(
    CPane* pThis, const wchar_t* lpszProfileName, int nIndex, unsigned int uiID);
extern "C" int MS_ABI impl__SaveState_CPane__UEAAHPEB_WHI_Z(
    CPane* pThis, const wchar_t* lpszProfileName, int nIndex, unsigned int uiID);

namespace {
// CString::Format would add an unresolved AfxThrowMemoryException reference to
// this translation unit. Allocate the profile length plus ample room for the
// fixed suffix and integer fields, then format with the Win32 wide helper.
wchar_t* MakeAdapterStateSection(const wchar_t* lpszProfileName, int nIndex,
                                 unsigned int uiID) {
    CString strProfileName;
    impl__AFXGetRegPath__YA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__PEB_W0_Z(
        &strProfileName, L"Panes", lpszProfileName);

    const std::size_t capacity = static_cast<std::size_t>(strProfileName.GetLength()) + 64;
    wchar_t* section = static_cast<wchar_t*>(
        ::HeapAlloc(::GetProcessHeap(), 0, capacity * sizeof(wchar_t)));
    if (section == nullptr) return nullptr;

    if (uiID == 0xFFFFFFFFu) {
        ::wsprintfW(section, L"%sDockablePaneAdapter-%d", strProfileName.GetString(), nIndex);
    } else {
        ::wsprintfW(section, L"%sDockablePaneAdapter-%d%x", strProfileName.GetString(), nIndex, uiID);
    }
    return section;
}
} // namespace

// Symbol: ??0CDockablePaneAdapter@@QEAA@XZ
extern "C" void* MS_ABI impl___0CDockablePaneAdapter__QEAA_XZ(void* pThis) {
    return pThis;
}
// Symbol: ??1CDockablePaneAdapter@@UEAA@XZ
// Retail RVA 0x46950 (mfc140u) installs the adapter vftable and tail-jumps to
// CDockablePane::~CDockablePane (entry RVA 0x40050, mfc140u). The base destructor
// first installs its own vftable, so this omits only the transient adapter-vftable
// store, which OpenMFC does not represent, and delegates the destructor work.
extern "C" void MS_ABI impl___1CDockablePaneAdapter__UEAA_XZ(void* pThis) {
    impl___1CDockablePane__UEAA_XZ(pThis);
}
// Symbol: ?CreateObject@CDockablePaneAdapter@@SAPEAVCObject@@XZ
// NOT IMPLEMENTED: RVA 0x46830 (mfc140u) requests 0x500 bytes and, if the
// allocation succeeds, invokes the constructor at RVA 0x468a0 (mfc140u). The local
// adapter constructor thunk only
// returns pThis, and OpenMFC has no CDockablePaneAdapter vftable to install, so
// allocation plus that thunk cannot produce a correctly initialized adapter.
extern "C" void* MS_ABI impl__CreateObject_CDockablePaneAdapter__SAPEAVCObject__XZ() {
    return nullptr;
}

// Symbol: ?LoadState@CDockablePaneAdapter@@UEAAHPEB_WHI@Z
// Retail mfc140u RVA 0x46ca0: open the adapter section, read BarName and set
// the window text if it is nonempty, then call CPane::LoadState. A failed Open
// returns FALSE without calling the base. OpenMFC's CPane::LoadState is still a
// stub, so this restores only BarName and the base call returns FALSE. The
// CSettingsStoreSP is retained in
// static storage and its path is reset by Open(section), rather than using the
// retail stack owner/deleting-destructor cleanup (which would add a new C++
// operator-delete dependency to this thunk). The null/allocation checks below
// are defensive deviations from retail's unchecked path.
extern "C" int MS_ABI impl__LoadState_CDockablePaneAdapter__UEAAHPEB_WHI_Z(
    void* pThis, const wchar_t* lpszProfileName, int nIndex, unsigned int uiID) {
    if (pThis == nullptr) return FALSE;

    if (nIndex == -1) {
        nIndex = impl__GetDlgCtrlID_CWnd__QEBAHXZ(static_cast<const CWnd*>(pThis));
    }
    wchar_t* section = MakeAdapterStateSection(lpszProfileName, nIndex, uiID);
    if (section == nullptr) return FALSE;

    static void* s_regSP[2] = { nullptr, nullptr };
    void* pReg = impl__Create_CSettingsStoreSP__QEAAAEAVCSettingsStore__HH_Z(
        static_cast<void*>(s_regSP), FALSE, FALSE);
    if (pReg == nullptr) {
        ::HeapFree(::GetProcessHeap(), 0, section);
        return FALSE;
    }
    const int opened = impl__Open_CSettingsStore__UEAAHPEB_W_Z(pReg, section);
    ::HeapFree(::GetProcessHeap(), 0, section);
    if (!opened) return FALSE;

    CString strBarName;
    (void)impl__Read_CSettingsStore__UEAAHPEB_WAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
        pReg, L"BarName", &strBarName);
    if (!strBarName.IsEmpty()) {
        impl__SetWindowTextW_CWnd__QEAAXPEB_W_Z(
            static_cast<CWnd*>(pThis), strBarName.GetString());
    }

    return impl__LoadState_CPane__UEAAHPEB_WHI_Z(
        static_cast<CPane*>(pThis), lpszProfileName, nIndex, uiID);
}

// Symbol: ?OnSize@CDockablePaneAdapter@@IEAAXIHH@Z
// The ANSI twin at mfc140 RVA 0x467c0 first calls CBasePane::OnSize (entry
// RVA 0xc320, mfc140), then, when +0x4f8 is non-null, calls CWnd::SetWindowPos
// (entry RVA 0x2a7970, mfc140) with (nullptr, 0, 0, cx, cy, 0x14). The local
// CBasePane thunk is currently an empty placeholder, so the base behavior is
// still absent here; the wrapped-window resize path is transcribed from retail.
extern "C" void MS_ABI impl__OnSize_CDockablePaneAdapter__IEAAXIHH_Z(
    void* pThis, unsigned int nType, int cx, int cy) {
    impl__OnSize_CBasePane__IEAAXIHH_Z(static_cast<CBasePane*>(pThis), nType, cx, cy);

    void* pWnd = static_cast<S_CDockablePaneAdapterLayout*>(pThis)->wrappedWnd;
    if (pWnd != nullptr) {
        impl__SetWindowPos_CWnd__QEAAHPEBV1_HHHHI_Z(
            static_cast<CWnd*>(pWnd), nullptr, 0, 0, cx, cy,
            SWP_NOZORDER | SWP_NOACTIVATE);
    }
}

// Symbol: ?SaveState@CDockablePaneAdapter@@UEAAHPEB_WHI@Z
// Retail ANSI twin mfc140 RVA 0x46930 creates the adapter section, writes the
// current window caption as BarName when CreateKey succeeds, and then calls
// CPane::SaveState unconditionally. The Unicode export is not resolved in the
// mfc140u symbol map. As in LoadState, the OpenMFC settings-store owner is
// retained in static storage; Open("") resets the path before CreateKey.
extern "C" int MS_ABI impl__SaveState_CDockablePaneAdapter__UEAAHPEB_WHI_Z(
    void* pThis, const wchar_t* lpszProfileName, int nIndex, unsigned int uiID) {
    if (nIndex == -1) {
        nIndex = impl__GetDlgCtrlID_CWnd__QEBAHXZ(static_cast<const CWnd*>(pThis));
    }
    wchar_t* section = MakeAdapterStateSection(lpszProfileName, nIndex, uiID);

    static void* s_regSP[2] = { nullptr, nullptr };
    void* pReg = impl__Create_CSettingsStoreSP__QEAAAEAVCSettingsStore__HH_Z(
        static_cast<void*>(s_regSP), FALSE, FALSE);
    if (pReg != nullptr && section != nullptr) {
        (void)impl__Open_CSettingsStore__UEAAHPEB_W_Z(pReg, L"");
        if (impl__CreateKey_CSettingsStore__UEAAHPEB_W_Z(pReg, section)) {
            CString strBarName;
            impl__GetWindowTextW_CWnd__QEBAXAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
                static_cast<const CWnd*>(pThis), &strBarName);
            (void)impl__Write_CSettingsStore__UEAAHPEB_W0_Z(
                pReg, L"BarName", strBarName.GetString());
        }
    }
    if (section != nullptr) ::HeapFree(::GetProcessHeap(), 0, section);

    return impl__SaveState_CPane__UEAAHPEB_WHI_Z(
        static_cast<CPane*>(pThis), lpszProfileName, nIndex, uiID);
}

// Symbol: ?SetWrappedWnd@CDockablePaneAdapter@@UEAAHPEAVCWnd@@@Z
// NOT IMPLEMENTED: the ANSI twin at RVA 0x46820 (mfc140) calls USER32!SetParent
// (IAT slot 0x1802c52b0 (mfc140)), stores pWnd at +0x4f8, then branches through
// several virtual/helper calls; one branch copies 16 bytes at +0x3d8 and others
// access +0x4f0 and +0xec. Those inherited-state and virtual-slot contracts are
// not established by this adapter shadow, so reproducing only the HWND reparent
// would be incomplete.
extern "C" int MS_ABI impl__SetWrappedWnd_CDockablePaneAdapter__UEAAHPEAVCWnd___Z(
    void* pThis, void* pWnd) {
    (void)pThis;
    (void)pWnd;
    return 0;
}

