// CMFCRibbonKeyboardCustomizeDialog — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp

#include "detail/ManualSmallStubImplementationsSupport.h"

#include <cstddef>

namespace {
// This class is not declared in include/openmfc. The retail constructor
// (RVA 0x10cc20, mfc140u) and DoDataExchange (RVA 0x10cf70, mfc140u) establish
// these member starts; opaque spans keep this translation unit independent of
// a guessed C++ class declaration.
struct S_RibbonKeyboardCustomizeDialog {
    std::byte m_base[0x170];
    std::byte m_strDescription[8];
    std::byte m_strAssignedTo[8];
    std::byte m_wndAssignedToTitle[0xe8];
    std::byte m_wndViewIcon[0xe8];
    std::byte m_wndRemoveButton[0xe8];
    std::byte m_wndAssignButton[0xe8];
    std::byte m_wndCurrentKeysList[0xe8];
    std::byte m_wndCategoryList[0xe8];
    std::byte m_wndViewTypeList[0xe8];
    std::byte m_wndNewKey[0x110];
    std::byte m_wndCommandsList[0x108];
    std::byte m_hAccelTable[8];
    std::byte m_lpAccel[8];
    void* m_pSelEntry;
    int m_nAccelSize;
    int m_flagA0C;
    void* m_pParentFrame;
    void* m_pRibbonBar;
    void* m_pSelTemplate;
    void* m_pSelButton;
};

static_assert(offsetof(S_RibbonKeyboardCustomizeDialog, m_strDescription) == 0x170,
              "DoDataExchange: description CString at +0x170");
static_assert(offsetof(S_RibbonKeyboardCustomizeDialog, m_strAssignedTo) == 0x178,
              "DoDataExchange: assigned-to CString at +0x178");
static_assert(offsetof(S_RibbonKeyboardCustomizeDialog, m_wndAssignedToTitle) == 0x180,
              "DoDataExchange: assigned-to title control at +0x180");
static_assert(offsetof(S_RibbonKeyboardCustomizeDialog, m_wndViewIcon) == 0x268,
              "DoDataExchange: view icon control at +0x268");
static_assert(offsetof(S_RibbonKeyboardCustomizeDialog, m_wndRemoveButton) == 0x350,
              "DoDataExchange: remove button at +0x350");
static_assert(offsetof(S_RibbonKeyboardCustomizeDialog, m_wndAssignButton) == 0x438,
              "DoDataExchange: assign button at +0x438");
static_assert(offsetof(S_RibbonKeyboardCustomizeDialog, m_wndCurrentKeysList) == 0x520,
              "DoDataExchange: current-keys list at +0x520");
static_assert(offsetof(S_RibbonKeyboardCustomizeDialog, m_wndCategoryList) == 0x608,
              "DoDataExchange: category list at +0x608");
static_assert(offsetof(S_RibbonKeyboardCustomizeDialog, m_wndViewTypeList) == 0x6f0,
              "DoDataExchange: view-type list at +0x6f0");
static_assert(offsetof(S_RibbonKeyboardCustomizeDialog, m_wndNewKey) == 0x7d8,
              "DoDataExchange: new-key control at +0x7d8");
static_assert(offsetof(S_RibbonKeyboardCustomizeDialog, m_wndCommandsList) == 0x8e8,
              "DoDataExchange: commands list at +0x8e8");
static_assert(offsetof(S_RibbonKeyboardCustomizeDialog, m_hAccelTable) == 0x9f0,
              "constructor: accelerator table at +0x9f0");
static_assert(offsetof(S_RibbonKeyboardCustomizeDialog, m_lpAccel) == 0x9f8,
              "OnAssign: accelerator array at +0x9f8");
static_assert(offsetof(S_RibbonKeyboardCustomizeDialog, m_pSelEntry) == 0xa00,
              "constructor: selected entry at +0xa00");
static_assert(offsetof(S_RibbonKeyboardCustomizeDialog, m_nAccelSize) == 0xa08,
              "OnAssign: accelerator count at +0xa08");
static_assert(offsetof(S_RibbonKeyboardCustomizeDialog, m_flagA0C) == 0xa0c,
              "OnInitDialog: 32-bit flag read at +0xa0c");
static_assert(offsetof(S_RibbonKeyboardCustomizeDialog, m_pParentFrame) == 0xa10,
              "constructor: parent frame at +0xa10");
static_assert(offsetof(S_RibbonKeyboardCustomizeDialog, m_pRibbonBar) == 0xa18,
              "constructor: ribbon bar at +0xa18");
static_assert(offsetof(S_RibbonKeyboardCustomizeDialog, m_pSelTemplate) == 0xa20,
              "OnAssign: selected template at +0xa20");
static_assert(offsetof(S_RibbonKeyboardCustomizeDialog, m_pSelButton) == 0xa28,
              "OnAssign: selected button at +0xa28");
static_assert(offsetof(CWnd, m_hWnd) == 0x40,
              "AddKeyEntry: CWnd HWND at +0x40 inside the current-keys list");

constexpr int kIdcAssignedToTitle = 0x4090;
constexpr int kIdcNewShortcutKey = 0x4104;
constexpr int kIdcViewType = 0x4102;
constexpr int kIdcViewIcon = 0x408e;
constexpr int kIdcRemove = 0x4106;
constexpr int kIdcCurrentKeysList = 0x4103;
constexpr int kIdcCommandsList = 0x4101;
constexpr int kIdcCategory = 0x40d9;
constexpr int kIdcAssign = 0x4105;
constexpr int kIdcCommandDescription = 0x4082;
constexpr int kIdcAssignedTo = 0x408f;
} // namespace

extern "C" void MS_ABI impl__DDX_Control__YAXPEAVCDataExchange__HAEAVCWnd___Z(
    void* pDX, int nIDC, void* pWnd);
extern "C" void MS_ABI impl__DDX_Text__YAXPEAVCDataExchange__HAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
    void* pDX, int nIDC, void* pValue);
extern "C" void MS_ABI impl__AfxThrowInvalidArgException__YAXXZ();
extern "C" int MS_ABI impl__EnableWindow_CWnd__QEAAHH_Z(CWnd* pThis, int bEnable);
extern "C" void* MS_ABI impl___0CMFCAcceleratorKey__QEAA_PEAUtagACCEL___Z(
    void* pThis, LPACCEL pAccel);
extern "C" void MS_ABI impl__Format_CMFCAcceleratorKey__QEBAXAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
    const void* pThis, CString* str);

// Retail mfc140u constructor (RVA 0x10cc20) calls an internal base-construction
// target with dialog ID 0x42b4, sets the vtable, initializes both CString members
// and all embedded controls, stores the ribbon/parent pointers, and clears the
// accelerator-selection fields. This remains a stub: returning pThis does not
// reproduce that initialization, and the required base/member construction
// paths are not represented by this body.
// Symbol: ??0CMFCRibbonKeyboardCustomizeDialog@@QEAA@PEAVCMFCRibbonBar@@PEAVCWnd@@@Z
extern "C" void* MS_ABI impl___0CMFCRibbonKeyboardCustomizeDialog__QEAA_PEAVCMFCRibbonBar__PEAVCWnd___Z(
    void* pThis, void* ribbonBar, void* parent) {
    (void)ribbonBar;
    (void)parent;
    return pThis;
}

// The retail destructor releases the accelerator array and tears down nine
// embedded controls and two CString members. This remains a stub: the existing
// constructor above does not initialize those members, so applying retail
// cleanup to objects built by it could free uninitialized state.
// Symbol: ??1CMFCRibbonKeyboardCustomizeDialog@@UEAA@XZ
extern "C" void MS_ABI impl___1CMFCRibbonKeyboardCustomizeDialog__UEAA_XZ(void* pThis) {
    (void)pThis;
}

// Retail (RVA 0x10dcf0, mfc140u): reject a null ACCEL pointer, format through
// CMFCAcceleratorKey::Format, then send LB_ADDSTRING (0x180) and LB_SETITEMDATA
// (0x19a) to the current-keys list. The HWND load is from this+0x560 (list
// member +0x40). The existing OpenMFC Format thunk has documented key-character
// differences from retail, so this call inherits that approximation.
// Symbol: ?AddKeyEntry@CMFCRibbonKeyboardCustomizeDialog@@IEAAXPEAUtagACCEL@@@Z
extern "C" void MS_ABI impl__AddKeyEntry_CMFCRibbonKeyboardCustomizeDialog__IEAAXPEAUtagACCEL___Z(
    void* pThis, LPACCEL pEntry) {
    if (pEntry == nullptr) {
        impl__AfxThrowInvalidArgException__YAXXZ();
        return;
    }

    auto* self = static_cast<S_RibbonKeyboardCustomizeDialog*>(pThis);
    struct {
        void* vfptr;
        LPACCEL m_lpAccel;
    } helper;
    impl___0CMFCAcceleratorKey__QEAA_PEAUtagACCEL___Z(&helper, pEntry);

    CString text;
    impl__Format_CMFCAcceleratorKey__QEBAXAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
        &helper, &text);

    HWND hKeys = reinterpret_cast<CWnd*>(self->m_wndCurrentKeysList)->m_hWnd;
    const int index = static_cast<int>(::SendMessage(
        hKeys, LB_ADDSTRING, 0, reinterpret_cast<LPARAM>(text.GetString())));
    ::SendMessage(hKeys, LB_SETITEMDATA,
                  static_cast<WPARAM>(static_cast<INT_PTR>(index)),
                  reinterpret_cast<LPARAM>(pEntry));
}

// Retail (RVA 0x10cf70, mfc140u): nine DDX_Control calls followed by two
// DDX_Text calls. There is no base DoDataExchange call in the retail body.
// Symbol: ?DoDataExchange@CMFCRibbonKeyboardCustomizeDialog@@MEAAXPEAVCDataExchange@@@Z
extern "C" void MS_ABI impl__DoDataExchange_CMFCRibbonKeyboardCustomizeDialog__MEAAXPEAVCDataExchange___Z(
    void* pThis, void* pDX) {
    auto* self = static_cast<S_RibbonKeyboardCustomizeDialog*>(pThis);
    impl__DDX_Control__YAXPEAVCDataExchange__HAEAVCWnd___Z(pDX, kIdcAssignedToTitle, self->m_wndAssignedToTitle);
    impl__DDX_Control__YAXPEAVCDataExchange__HAEAVCWnd___Z(pDX, kIdcNewShortcutKey, self->m_wndNewKey);
    impl__DDX_Control__YAXPEAVCDataExchange__HAEAVCWnd___Z(pDX, kIdcViewType, self->m_wndViewTypeList);
    impl__DDX_Control__YAXPEAVCDataExchange__HAEAVCWnd___Z(pDX, kIdcViewIcon, self->m_wndViewIcon);
    impl__DDX_Control__YAXPEAVCDataExchange__HAEAVCWnd___Z(pDX, kIdcRemove, self->m_wndRemoveButton);
    impl__DDX_Control__YAXPEAVCDataExchange__HAEAVCWnd___Z(pDX, kIdcCurrentKeysList, self->m_wndCurrentKeysList);
    impl__DDX_Control__YAXPEAVCDataExchange__HAEAVCWnd___Z(pDX, kIdcCommandsList, self->m_wndCommandsList);
    impl__DDX_Control__YAXPEAVCDataExchange__HAEAVCWnd___Z(pDX, kIdcCategory, self->m_wndCategoryList);
    impl__DDX_Control__YAXPEAVCDataExchange__HAEAVCWnd___Z(pDX, kIdcAssign, self->m_wndAssignButton);
    impl__DDX_Text__YAXPEAVCDataExchange__HAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
        pDX, kIdcCommandDescription, self->m_strDescription);
    impl__DDX_Text__YAXPEAVCDataExchange__HAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
        pDX, kIdcAssignedTo, self->m_strAssignedTo);
}

// Retail mfc140u disassembly (OnAssign entry RVA 0x10d430) replaces the
// accelerator array, retargets list-item data for retained entries, updates the
// keyboard manager's accelerator table, adds the new key entry, resets the new-key
// control, calls the body at RVA 0x10ddc0 (mfc140u), then returns focus to the commands list.
// That update body is disassembled separately but is not exported under a name in
// the Unicode map. Keep this handler stubbed rather than partially reproducing its
// coupled allocation, ribbon, keyboard-manager, and UI paths.
// Symbol: ?OnAssign@CMFCRibbonKeyboardCustomizeDialog@@IEAAXXZ
extern "C" void MS_ABI impl__OnAssign_CMFCRibbonKeyboardCustomizeDialog__IEAAXXZ(void* pThis) {
    (void)pThis;
}

// The ANSI-twin symbol-resolved body was disassembled; the Unicode map has no
// named RVA for this handler. It calls base initialization, populates category
// and command UI from ribbon/template state, and performs resource-dependent
// shortcut setup. Kept stubbed rather than partially implementing those coupled
// initialization and UI paths.
// Symbol: ?OnInitDialog@CMFCRibbonKeyboardCustomizeDialog@@MEAAHXZ
extern "C" int MS_ABI impl__OnInitDialog_CMFCRibbonKeyboardCustomizeDialog__MEAAHXZ(void* pThis) {
    (void)pThis;
    return 0;
}

// The ANSI-twin body at entry RVA 0x10e660 (mfc140) was disassembled; the Unicode
// map has no named RVA for this handler. It allocates a replacement accelerator
// array without the selected entry, retargets list-item data for retained entries,
// frees the old array, decrements the count, updates the keyboard manager, refreshes
// command selection, and returns focus to the commands list. This body sends no
// LB_DELETESTRING message, so do not claim that it removes the selected list row.
// Kept stubbed because the allocation-failure/assertion and UI paths are not
// reproduced.
// Symbol: ?OnRemove@CMFCRibbonKeyboardCustomizeDialog@@IEAAXXZ
extern "C" void MS_ABI impl__OnRemove_CMFCRibbonKeyboardCustomizeDialog__IEAAXXZ(void* pThis) {
    (void)pThis;
}

// The ANSI-twin symbol-resolved body was disassembled; the Unicode map has no
// named RVA for this handler. It performs resource-backed reset confirmation and,
// only on the affirmative branch, resets selection/accelerator state through
// framework and view helpers. Kept stubbed rather than partially applying a reset
// without those coupled resource/module-state and view-update paths.
// Symbol: ?OnResetAll@CMFCRibbonKeyboardCustomizeDialog@@IEAAXXZ
extern "C" void MS_ABI impl__OnResetAll_CMFCRibbonKeyboardCustomizeDialog__IEAAXXZ(void* pThis) {
    (void)pThis;
}

// The ANSI-twin symbol-resolved body was disassembled; the Unicode map has no
// named RVA for this handler. It calls UpdateData(TRUE), refreshes the command
// list from the selected category, resets the shortcut control, and updates the
// command selection when rows exist. Kept stubbed because those internal ribbon
// item/list helper paths are not faithfully represented here.
// Symbol: ?OnSelchangeCategory@CMFCRibbonKeyboardCustomizeDialog@@IEAAXXZ
extern "C" void MS_ABI impl__OnSelchangeCategory_CMFCRibbonKeyboardCustomizeDialog__IEAAXXZ(void* pThis) {
    (void)pThis;
}

// The ANSI-twin symbol-resolved body was disassembled; the Unicode map has no
// named RVA for this handler. It reads the selected command, updates description
// and selected-button state, scans accelerator entries, and changes assign-button
// state before UpdateData(FALSE). Kept stubbed because command-item and ribbon
// element lookup/update paths cannot be faithfully represented by this stub.
// Symbol: ?OnSelchangeCommandsList@CMFCRibbonKeyboardCustomizeDialog@@IEAAXXZ
extern "C" void MS_ABI impl__OnSelchangeCommandsList_CMFCRibbonKeyboardCustomizeDialog__IEAAXXZ(void* pThis) {
    (void)pThis;
}

// The ANSI twin's body (RVA 0x10e5e0, mfc140) disables remove and clears +0xa00
// when there is no selection; otherwise it stores LB_GETITEMDATA there, throws
// if that value is null, and enables remove. The corresponding body bytes are
// shared with mfc140u.
// Symbol: ?OnSelchangeCurrentKeysList@CMFCRibbonKeyboardCustomizeDialog@@IEAAXXZ
extern "C" void MS_ABI impl__OnSelchangeCurrentKeysList_CMFCRibbonKeyboardCustomizeDialog__IEAAXXZ(
    void* pThis) {
    auto* self = static_cast<S_RibbonKeyboardCustomizeDialog*>(pThis);
    auto* keys = reinterpret_cast<CWnd*>(self->m_wndCurrentKeysList);
    const int index = static_cast<int>(::SendMessage(keys->m_hWnd, LB_GETCURSEL, 0, 0));
    if (index == LB_ERR) {
        self->m_pSelEntry = nullptr;
        impl__EnableWindow_CWnd__QEAAHH_Z(
            reinterpret_cast<CWnd*>(self->m_wndRemoveButton), FALSE);
        return;
    }

    self->m_pSelEntry = reinterpret_cast<void*>(::SendMessage(
        keys->m_hWnd, LB_GETITEMDATA, static_cast<WPARAM>(index), 0));
    if (self->m_pSelEntry == nullptr) {
        impl__AfxThrowInvalidArgException__YAXXZ();
        return;
    }
    impl__EnableWindow_CWnd__QEAAHH_Z(
        reinterpret_cast<CWnd*>(self->m_wndRemoveButton), TRUE);
}

// The ANSI-twin symbol-resolved body was disassembled; the Unicode map has no
// named RVA for this handler. It clears the current accelerator/template state,
// frees the accelerator array, reads the view selection, resolves a template or
// frame accelerator table, and rebuilds state before refreshing command selection.
// Kept stubbed because the resource/template lookup and helper paths are coupled.
// Symbol: ?OnSelchangeViewType@CMFCRibbonKeyboardCustomizeDialog@@IEAAXXZ
extern "C" void MS_ABI impl__OnSelchangeViewType_CMFCRibbonKeyboardCustomizeDialog__IEAAXXZ(void* pThis) {
    (void)pThis;
}

// The named export has no mfc140u map entry, but OnAssign calls the body at
// RVA 0x10ddc0 (mfc140u). That body checks the new-key ACCEL against existing
// entries, resolves the matching ribbon command, and updates command description
// and control state. It remains stubbed rather than approximating those coupled
// command/ribbon lookup and UI operations.
// Symbol: ?OnUpdateNewShortcutKey@CMFCRibbonKeyboardCustomizeDialog@@IEAAXXZ
extern "C" void MS_ABI impl__OnUpdateNewShortcutKey_CMFCRibbonKeyboardCustomizeDialog__IEAAXXZ(void* pThis) {
    (void)pThis;
}
