// CMFCRibbonCustomizePropertyPage — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp

#include "detail/ManualSmallStubImplementationsSupport.h"

namespace {
// This class is not declared in include/openmfc. The offsets are from retail DoDataExchange
// (entry RVA 0xf9b90, mfc140u); field names here are descriptive labels for controls and values.
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

// CMFCRibbonBaseElement's abbreviated OpenMFC layout stores m_nID immediately after CObject's
// vptr; retail GetCommandIndex (entry RVA 0xf7f00, mfc140u) reads +0x118, not valid for these objects.
struct S_CMFCRibbonBaseElementOpenMfcPrefix {
    void* vftable;
    UINT m_nID;
};
static_assert(offsetof(S_CMFCRibbonBaseElementOpenMfcPrefix, m_nID) == sizeof(CObject));
} // namespace

extern "C" void MS_ABI impl__DDX_Control__YAXPEAVCDataExchange__HAEAVCWnd___Z(
    void* pDX, int nIDC, void* pWnd);
extern "C" void MS_ABI impl__DDX_CBIndex__YAXPEAVCDataExchange__HAEAH_Z(
    void* pDX, int nIDC, int* pValue);
extern "C" void MS_ABI impl__DDX_Check__YAXPEAVCDataExchange__HAEAH_Z(
    void* pDX, int nIDC, int* pValue);
extern "C" int MS_ABI impl__EnableWindow_CWnd__QEAAHH_Z(CWnd* pThis, int bEnable);
extern "C" int MS_ABI impl__GetCount_CListBox__QEBAHXZ(const CListBox* pThis);
extern "C" int MS_ABI impl__GetCurSel_CListBox__QEBAHXZ(const CListBox* pThis);
extern "C" int MS_ABI impl__SetCurSel_CListBox__QEAAHH_Z(CListBox* pThis, int nSelect);
extern "C" void* MS_ABI impl__GetSelected_CMFCRibbonCommandsListBox__QEBAPEAVCMFCRibbonBaseElement__XZ(
    void* pThis);
extern "C" int MS_ABI impl__GetCommandIndex_CMFCRibbonCommandsListBox__QEBAHI_Z(
    void* pThis, unsigned int commandId);
extern "C" void MS_ABI impl__MoveItem_CMFCRibbonCustomizePropertyPage__IEAAXH_Z(
    void* pThis, int bMoveUp);
extern "C" void MS_ABI impl__OnSelchangeQATCommands_CMFCRibbonCustomizePropertyPage__IEAAXXZ(
    void* pThis);
extern "C" void MS_ABI impl__OnSelchangeCommandsList_CMFCRibbonCustomizePropertyPage__IEAAXXZ(
    void* pThis);

// Symbol: ??0CMFCRibbonCustomizePropertyPage@@QEAA@PEAVCMFCRibbonBar@@@Z
// Legacy pass-through retained from HEAD: retail mfc140u entry RVA 0xf98c0 initializes the base,
// embedded controls/lists, and page fields. This body does not initialize them; CreateObject therefore
// cannot safely return storage from it. Kept unchanged because this is pre-existing code.
extern "C" void* MS_ABI impl___0CMFCRibbonCustomizePropertyPage__QEAA_PEAVCMFCRibbonBar___Z(
    void* pThis, void* pRibbonBar) {
    (void)pRibbonBar;
    return pThis;
}
// Symbol: ??1CMFCRibbonCustomizePropertyPage@@UEAA@XZ
// Retail mfc140u RVA 0xf9a50 unlinks/releases the custom-category list at +0x1c98, destroys the
// embedded list/control members, and chains to the CMFCPropertyPage destructor. This file does not
// model the custom-category list node/refcount state needed to reproduce that teardown.
extern "C" void MS_ABI impl___1CMFCRibbonCustomizePropertyPage__UEAA_XZ(void* pThis) {
    (void)pThis;
}

// Symbol: ?AddCustomCategory@CMFCRibbonCustomizePropertyPage@@QEAAXPEB_WAEBV?$CList@II@@@Z
// Retail mfc140u RVA 0xfa960 allocates and links a 0x48-byte custom-category node, copies the name
// and command IDs, then appends it to the list at +0x1c98. The category/list-node and CString/refcount
// layouts and operations needed to reproduce those writes are not modeled here.
extern "C" void MS_ABI impl__AddCustomCategory_CMFCRibbonCustomizePropertyPage__QEAAXPEB_WAEBV__CList_II___Z(
    void* pThis, const wchar_t* name, const void* commandIds) {
    (void)pThis;
    (void)name;
    (void)commandIds;
}

// Symbol: ?CreateObject@CMFCRibbonCustomizePropertyPage@@SAPEAVCObject@@XZ
// Retail mfc140u RVA 0xf9880 allocates 0x1cd0 bytes and calls the constructor at entry RVA 0xf98c0
// (mfc140u). That constructor initializes the base page, embedded controls/list, fields, and ribbon
// pointer; this file's constructor thunk is only a pass-through, so returning such storage is unsafe.
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
// Retail mfc140u RVA 0xfa620 moves a selected item in the QAT list, updates list selection, and
// refreshes page state. Reproducing its item-reference lifetime and associated command/list updates
// requires state and operations not modeled in this translation unit.
extern "C" void MS_ABI impl__MoveItem_CMFCRibbonCustomizePropertyPage__IEAAXH_Z(
    void* pThis, int bMoveUp) {
    (void)pThis;
    (void)bMoveUp;
}

// Symbol: ?OnAdd@CMFCRibbonCustomizePropertyPage@@IEAAXXZ
// Retail mfc140u RVA 0xf9d50 gets the selected command, attempts to add it to the QAT list, and then
// refreshes page state. The sibling AddCommand thunk at RVA 0xf7f80 (mfc140u) is currently a no-op;
// the required command insertion and synchronization therefore cannot be completed here.
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
// The mfc140u lookup did not resolve this export; its ANSI twin is at RVA 0xfacb0 (mfc140) and
// requires extensive ribbon-bar/list internals not modeled here, so the procedure remains stubbed.
extern "C" int MS_ABI impl__OnInitDialog_CMFCRibbonCustomizePropertyPage__MEAAHXZ(void* pThis) {
    (void)pThis;
    return 0;
}

// Symbol: ?OnOK@CMFCRibbonCustomizePropertyPage@@UEAAXXZ
// The mfc140u lookup did not resolve this export; the ANSI twin at RVA 0xfb310 (mfc140) merges
// category state and updates the ribbon, which requires list/ribbon support absent from this file.
extern "C" void MS_ABI impl__OnOK_CMFCRibbonCustomizePropertyPage__UEAAXXZ(void* pThis) {
    (void)pThis;
}

// The mfc140 ANSI twin at RVA 0xfa9b0 adjusts the QAT-list selection to the last remaining row
// when the old selection was at the end, then calls the two selection handlers. It does not delete
// a list item itself.
// The corresponding mfc140u export did not resolve; these list operations use the existing CListBox
// thunks, and the handler control flow is byte-identical between the twins per BRIEFING2.md.
// Symbol: ?OnRemove@CMFCRibbonCustomizePropertyPage@@IEAAXXZ
extern "C" void MS_ABI impl__OnRemove_CMFCRibbonCustomizePropertyPage__IEAAXXZ(void* pThis) {
    auto* self = static_cast<S_CMFCRibbonCustomizePropertyPage*>(pThis);
    auto* qatList = reinterpret_cast<CListBox*>(self->m_wndQATList);
    const int selection = impl__GetCurSel_CListBox__QEBAHXZ(qatList);
    if (selection >= 0) {
        impl__SetCurSel_CListBox__QEAAHH_Z(qatList, selection);
        int count = impl__GetCount_CListBox__QEBAHXZ(qatList);
        if (selection >= count - 1) {
            count = impl__GetCount_CListBox__QEBAHXZ(qatList);
            const int last = count - 1;
            if (last >= 0) {
                impl__SetCurSel_CListBox__QEAAHH_Z(qatList, last);
            }
        }
    }
    impl__OnSelchangeQATCommands_CMFCRibbonCustomizePropertyPage__IEAAXXZ(pThis);
    impl__OnSelchangeCommandsList_CMFCRibbonCustomizePropertyPage__IEAAXXZ(pThis);
}

// The mfc140 ANSI twin at RVA 0xfb4c0 gets the selected command, enables Add for a zero-ID item or
// a nonzero command ID absent from the QAT list, and disables it for no selection or an ID already
// present. The mfc140u name lookup did not resolve; read the ID through OpenMFC's abbreviated
// +sizeof(CObject) field (rather than retail's +0x118), and use the existing list/control thunks.
// Symbol: ?OnSelchangeCommandsList@CMFCRibbonCustomizePropertyPage@@IEAAXXZ
extern "C" void MS_ABI impl__OnSelchangeCommandsList_CMFCRibbonCustomizePropertyPage__IEAAXXZ(
    void* pThis) {
    auto* self = static_cast<S_CMFCRibbonCustomizePropertyPage*>(pThis);
    void* selected = impl__GetSelected_CMFCRibbonCommandsListBox__QEBAPEAVCMFCRibbonBaseElement__XZ(
        self->m_wndCommandsList);
    int enableAdd = FALSE;
    if (selected != nullptr) {
        const unsigned int commandId =
            static_cast<const S_CMFCRibbonBaseElementOpenMfcPrefix*>(selected)->m_nID;
        enableAdd = commandId == 0 ||
            impl__GetCommandIndex_CMFCRibbonCommandsListBox__QEBAHI_Z(
                self->m_wndQATList, commandId) < 0;
    }
    impl__EnableWindow_CWnd__QEAAHH_Z(
        reinterpret_cast<CWnd*>(self->m_wndAdd), enableAdd);
}

// The mfc140 ANSI twin at RVA 0xfabf0 enables Up when selection > 0, Down when selection is
// before the final row, and Remove when a row is selected. The mfc140u name lookup did not resolve;
// list queries and control updates use the existing CListBox/CWnd thunks.
// Symbol: ?OnSelchangeQATCommands@CMFCRibbonCustomizePropertyPage@@IEAAXXZ
extern "C" void MS_ABI impl__OnSelchangeQATCommands_CMFCRibbonCustomizePropertyPage__IEAAXXZ(
    void* pThis) {
    auto* self = static_cast<S_CMFCRibbonCustomizePropertyPage*>(pThis);
    auto* qatList = reinterpret_cast<CListBox*>(self->m_wndQATList);
    const int selection = impl__GetCurSel_CListBox__QEBAHXZ(qatList);
    const int count = impl__GetCount_CListBox__QEBAHXZ(qatList);
    impl__EnableWindow_CWnd__QEAAHH_Z(
        reinterpret_cast<CWnd*>(self->m_wndUp), selection > 0);
    impl__EnableWindow_CWnd__QEAAHH_Z(
        reinterpret_cast<CWnd*>(self->m_wndDown), selection < count - 1);
    impl__EnableWindow_CWnd__QEAAHH_Z(
        reinterpret_cast<CWnd*>(self->m_wndRemove), selection >= 0);
}

// Symbol: ?OnSelendokCategoryCombo@CMFCRibbonCustomizePropertyPage@@IEAAXXZ
// The mfc140 ANSI twin at RVA 0xfa820 reads m_nCategory to query combo item data. If it is null and
// the selection is not final, m_nCategory is incremented; matching item-data strings take list-helper
// branches and then call OnSelchangeCommandsList, while unmatched strings return. The mfc140u name
// lookup did not resolve, and the ribbon/category/list helper paths are not modeled here.
extern "C" void MS_ABI impl__OnSelendokCategoryCombo_CMFCRibbonCustomizePropertyPage__IEAAXXZ(
    void* pThis) {
    (void)pThis;
}

// Symbol: ?OnToolbarReset@CMFCRibbonCustomizePropertyPage@@IEAAXXZ
// The ANSI twin at RVA 0xfaa90 (mfc140) manipulates temporary command/list state and performs
// associated updates; the mfc140u lookup did not resolve, and required helper/layout support is absent.
extern "C" void MS_ABI impl__OnToolbarReset_CMFCRibbonCustomizePropertyPage__IEAAXXZ(void* pThis) {
    (void)pThis;
}

// Symbol: ?OnUp@CMFCRibbonCustomizePropertyPage@@IEAAXXZ
// ANSI twin mfc140 RVA 0xfaa70 sets the MoveItem direction to one; the mfc140u name lookup did
// not resolve. Function bodies are byte-identical between twins, as noted in BRIEFING2.md.
extern "C" void MS_ABI impl__OnUp_CMFCRibbonCustomizePropertyPage__IEAAXXZ(void* pThis) {
    impl__MoveItem_CMFCRibbonCustomizePropertyPage__IEAAXH_Z(pThis, 1);
}
