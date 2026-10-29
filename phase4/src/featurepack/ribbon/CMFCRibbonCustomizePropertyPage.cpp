// CMFCRibbonCustomizePropertyPage — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp

#include "detail/ManualSmallStubImplementationsSupport.h"

namespace {
// This class is not declared in include/openmfc. The offsets are from retail DoDataExchange;
// field names here are descriptive labels for the corresponding controls and values.
struct S_CMFCRibbonCustomizePropertyPage {
    unsigned char baseAndInherited[0x180];
    unsigned char m_wndAdd[0xe8];
    unsigned char m_wndRemove[0xe8];
    unsigned char m_wndCategoryCombo[0xe8];
    unsigned char m_wndUp[0xb20];
    unsigned char m_wndDown[0xb20];
    unsigned char m_wndCommandsList[0x108];
    unsigned char m_wndQATList[0x108];
    int m_nCategory;
    int m_bQAToolbarOnBottom;
};
static_assert(offsetof(S_CMFCRibbonCustomizePropertyPage, m_wndAdd) == 0x180);
static_assert(offsetof(S_CMFCRibbonCustomizePropertyPage, m_wndRemove) == 0x268);
static_assert(offsetof(S_CMFCRibbonCustomizePropertyPage, m_wndCategoryCombo) == 0x350);
static_assert(offsetof(S_CMFCRibbonCustomizePropertyPage, m_wndUp) == 0x438);
static_assert(offsetof(S_CMFCRibbonCustomizePropertyPage, m_wndDown) == 0xf58);
static_assert(offsetof(S_CMFCRibbonCustomizePropertyPage, m_wndCommandsList) == 0x1a78);
static_assert(offsetof(S_CMFCRibbonCustomizePropertyPage, m_wndQATList) == 0x1b80);
static_assert(offsetof(S_CMFCRibbonCustomizePropertyPage, m_nCategory) == 0x1c88);
static_assert(offsetof(S_CMFCRibbonCustomizePropertyPage, m_bQAToolbarOnBottom) == 0x1c8c);
} // namespace

extern "C" void MS_ABI impl__DDX_Control__YAXPEAVCDataExchange__HAEAVCWnd___Z(
    void* pDX, int nIDC, void* pWnd);
extern "C" void MS_ABI impl__DDX_CBIndex__YAXPEAVCDataExchange__HAEAH_Z(
    void* pDX, int nIDC, int* pValue);
extern "C" void MS_ABI impl__DDX_Check__YAXPEAVCDataExchange__HAEAH_Z(
    void* pDX, int nIDC, int* pValue);
extern "C" void MS_ABI impl__MoveItem_CMFCRibbonCustomizePropertyPage__IEAAXH_Z(
    void* pThis, int bMoveUp);

// Symbol: ??0CMFCRibbonCustomizePropertyPage@@QEAA@PEAVCMFCRibbonBar@@@Z
extern "C" void* MS_ABI impl___0CMFCRibbonCustomizePropertyPage__QEAA_PEAVCMFCRibbonBar___Z(
    void* pThis, void* pRibbonBar) {
    (void)pRibbonBar;
    return pThis;
}
// Symbol: ??1CMFCRibbonCustomizePropertyPage@@UEAA@XZ
// Retail mfc140u RVA 0xf9a50 tears down several embedded controls and the custom-category list.
// This file has no validated layout/thunk coverage for that cleanup sequence.
extern "C" void MS_ABI impl___1CMFCRibbonCustomizePropertyPage__UEAA_XZ(void* pThis) {
    (void)pThis;
}

// Symbol: ?AddCustomCategory@CMFCRibbonCustomizePropertyPage@@QEAAXPEB_WAEBV?$CList@II@@@Z
// Retail mfc140u RVA 0xfa960 allocates and links a custom-category object and copies command IDs;
// the category/list layouts and the required allocation/list helper thunks are not modeled here.
extern "C" void MS_ABI impl__AddCustomCategory_CMFCRibbonCustomizePropertyPage__QEAAXPEB_WAEBV__CList_II___Z(
    void* pThis, const wchar_t* name, const void* commandIds) {
    (void)pThis;
    (void)name;
    (void)commandIds;
}

// Symbol: ?CreateObject@CMFCRibbonCustomizePropertyPage@@SAPEAVCObject@@XZ
// Retail mfc140u RVA 0xf9880 allocates 0x1cd0 bytes and invokes this class's constructor;
// the constructor thunk in this file is only a pass-through and does not initialize that layout.
extern "C" void* MS_ABI impl__CreateObject_CMFCRibbonCustomizePropertyPage__SAPEAVCObject__XZ() {
    return nullptr;
}

// Symbol: ?DoDataExchange@CMFCRibbonCustomizePropertyPage@@MEAAXPEAVCDataExchange@@@Z
// Retail mfc140u RVA 0xf9b90: seven DDX_Control calls, then DDX_CBIndex and DDX_Check.
extern "C" void MS_ABI impl__DoDataExchange_CMFCRibbonCustomizePropertyPage__MEAAXPEAVCDataExchange___Z(
    void* pThis, void* pDX) {
    auto* self = static_cast<S_CMFCRibbonCustomizePropertyPage*>(pThis);
    impl__DDX_Control__YAXPEAVCDataExchange__HAEAVCWnd___Z(pDX, 0x42b3, self->m_wndAdd);
    impl__DDX_Control__YAXPEAVCDataExchange__HAEAVCWnd___Z(pDX, 0x4106, self->m_wndRemove);
    impl__DDX_Control__YAXPEAVCDataExchange__HAEAVCWnd___Z(pDX, 0x4101, self->m_wndCommandsList);
    impl__DDX_Control__YAXPEAVCDataExchange__HAEAVCWnd___Z(pDX, 0x40d9, self->m_wndCategoryCombo);
    impl__DDX_Control__YAXPEAVCDataExchange__HAEAVCWnd___Z(pDX, 0x42b5, self->m_wndQATList);
    impl__DDX_Control__YAXPEAVCDataExchange__HAEAVCWnd___Z(pDX, 0x4281, self->m_wndUp);
    impl__DDX_Control__YAXPEAVCDataExchange__HAEAVCWnd___Z(pDX, 0x4282, self->m_wndDown);
    impl__DDX_CBIndex__YAXPEAVCDataExchange__HAEAH_Z(pDX, 0x40d9, &self->m_nCategory);
    impl__DDX_Check__YAXPEAVCDataExchange__HAEAH_Z(pDX, 0x42b6, &self->m_bQAToolbarOnBottom);
}

// Symbol: ?MoveItem@CMFCRibbonCustomizePropertyPage@@IEAAXH@Z
// Retail mfc140u RVA 0xfa620 moves an item in the QAT list and synchronizes the command list;
// this translation unit lacks the command-list and item-reference support needed to reproduce it.
extern "C" void MS_ABI impl__MoveItem_CMFCRibbonCustomizePropertyPage__IEAAXH_Z(
    void* pThis, int bMoveUp) {
    (void)pThis;
    (void)bMoveUp;
}

// Symbol: ?OnAdd@CMFCRibbonCustomizePropertyPage@@IEAAXXZ
// Retail mfc140u RVA 0xf9d50 queries the selected command, inserts it, and synchronizes both lists;
// the called private helper routines and command-list state are not available in this file.
extern "C" void MS_ABI impl__OnAdd_CMFCRibbonCustomizePropertyPage__IEAAXXZ(void* pThis) {
    (void)pThis;
}

// Symbol: ?OnCustomizeKeyboard@CMFCRibbonCustomizePropertyPage@@IEAAXXZ
// Retail mfc140u RVA 0xfa8d0 builds and displays a keyboard-customization dialog using a 0xa38-byte
// local object. The companion dialog constructor thunk is only a pass-through, so this object cannot
// be safely initialized for the modal call by the implementation available here.
extern "C" void MS_ABI impl__OnCustomizeKeyboard_CMFCRibbonCustomizePropertyPage__IEAAXXZ(
    void* pThis) {
    (void)pThis;
}

// Symbol: ?OnDown@CMFCRibbonCustomizePropertyPage@@IEAAXXZ
// Retail mfc140u RVA 0xf9ed0 clears the direction argument and tail-jumps to MoveItem.
extern "C" void MS_ABI impl__OnDown_CMFCRibbonCustomizePropertyPage__IEAAXXZ(void* pThis) {
    impl__MoveItem_CMFCRibbonCustomizePropertyPage__IEAAXH_Z(pThis, 0);
}

// Symbol: ?OnInitDialog@CMFCRibbonCustomizePropertyPage@@MEAAHXZ
// The mfc140u lookup did not resolve this export; its ANSI twin is at RVA 0xfacb0 and requires
// extensive ribbon-bar/list internals not modeled here, so the procedure remains stubbed.
extern "C" int MS_ABI impl__OnInitDialog_CMFCRibbonCustomizePropertyPage__MEAAHXZ(void* pThis) {
    (void)pThis;
    return 0;
}

// Symbol: ?OnOK@CMFCRibbonCustomizePropertyPage@@UEAAXXZ
// The mfc140u lookup did not resolve this export; the ANSI twin at RVA 0xfb310 merges category
// state and updates the ribbon, which requires list/ribbon support absent from this file.
extern "C" void MS_ABI impl__OnOK_CMFCRibbonCustomizePropertyPage__UEAAXXZ(void* pThis) {
    (void)pThis;
}

// Symbol: ?OnRemove@CMFCRibbonCustomizePropertyPage@@IEAAXXZ
// The ANSI twin at mfc140 RVA 0xfa9b0 uses the QAT-list HWND at this+0x1bc0 (the control begins
// at +0x1b80) to query/adjust selection, then dispatches list-selection handlers; the mfc140u RVA
// did not resolve and those helper/list internals are absent.
extern "C" void MS_ABI impl__OnRemove_CMFCRibbonCustomizePropertyPage__IEAAXXZ(void* pThis) {
    (void)pThis;
}

// Symbol: ?OnSelchangeCommandsList@CMFCRibbonCustomizePropertyPage@@IEAAXXZ
// The ANSI twin at mfc140 RVA 0xfb4c0 reads the selected command and updates the Add control;
// the mfc140u RVA did not resolve, and the command/list helper routines are not modeled here.
extern "C" void MS_ABI impl__OnSelchangeCommandsList_CMFCRibbonCustomizePropertyPage__IEAAXXZ(
    void* pThis) {
    (void)pThis;
}

// Symbol: ?OnSelchangeQATCommands@CMFCRibbonCustomizePropertyPage@@IEAAXXZ
// The ANSI twin at mfc140 RVA 0xfabf0 reads the QAT selection and updates the Up/Down/Remove
// controls; the mfc140u RVA did not resolve and its list dispatch helpers are unavailable here.
extern "C" void MS_ABI impl__OnSelchangeQATCommands_CMFCRibbonCustomizePropertyPage__IEAAXXZ(
    void* pThis) {
    (void)pThis;
}

// Symbol: ?OnSelendokCategoryCombo@CMFCRibbonCustomizePropertyPage@@IEAAXXZ
// The ANSI twin at mfc140 RVA 0xfa820 updates m_nCategory and synchronizes the command list with
// the selected ribbon category; the mfc140u RVA did not resolve and ribbon/list helpers are absent.
extern "C" void MS_ABI impl__OnSelendokCategoryCombo_CMFCRibbonCustomizePropertyPage__IEAAXXZ(
    void* pThis) {
    (void)pThis;
}

// Symbol: ?OnToolbarReset@CMFCRibbonCustomizePropertyPage@@IEAAXXZ
// The ANSI twin at mfc140 RVA 0xfaa90 manipulates temporary command/list state and performs
// associated updates; the mfc140u RVA did not resolve and the required helper/layout support is absent.
extern "C" void MS_ABI impl__OnToolbarReset_CMFCRibbonCustomizePropertyPage__IEAAXXZ(void* pThis) {
    (void)pThis;
}

// Symbol: ?OnUp@CMFCRibbonCustomizePropertyPage@@IEAAXXZ
// ANSI twin mfc140 RVA 0xfaa70 sets the MoveItem direction to one; the mfc140u name lookup did
// not resolve. Function bodies are byte-identical between twins, as noted in BRIEFING2.md.
extern "C" void MS_ABI impl__OnUp_CMFCRibbonCustomizePropertyPage__IEAAXXZ(void* pThis) {
    impl__MoveItem_CMFCRibbonCustomizePropertyPage__IEAAXH_Z(pThis, 1);
}
