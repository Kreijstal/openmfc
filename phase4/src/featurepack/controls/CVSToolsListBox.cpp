// CVSToolsListBox — OpenMFC implementation.
// Sources: dlgcommon.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/DlgcommonSupport.h"

#include <cstddef>
#include <cstdint>

// ===========================================================================
// CVSToolsListBox -- the user-tools list of the Customize dialog's Tools page
// (afxtoolbarstoolspropertypage.h:32 in the 14.51 SDK on this host):
//     class CVSToolsListBox : public CVSListBox {
//         CVSToolsListBox(CMFCToolBarsToolsPropertyPage* pParent) : m_pParent(pParent) {}
//         <six overrides>  CMFCToolBarsToolsPropertyPage* m_pParent; };
// OpenMFC's public headers do not declare the class, so its layout is pinned
// here (S_ToolsList) and the page's members it touches are pinned by offset
// (S_ToolsPageView); featurepack/customize/CMFCToolBarsToolsPropertyPage.cpp
// pins the same page offsets from the page constructor.
//
// Every body below is transcribed from the retail disassembly (the method in
// the header comment of core/ole/COleControl.cpp).  Bodies are the same
// instruction sequence in mfc140.dll and mfc140u.dll -- only RIP-relative
// displacements differ (e.g. OnSelectionChanged's first `call *` encodes
// ff15e37514 in mfc140u, ff15637014 in mfc140) -- so control flow, offsets
// and slots read from either image agree; every RVA and address quoted here
// is mfc140u.
// Entries (mfc140u): OnBeforeRemoveItem 0x180320, OnAfterAddItem 0x180370,
// OnAfterRenameItem 0x180440, OnAfterMoveItemUp 0x1804d0, OnAfterMoveItemDown
// 0x180500 (all from mfc140u_rva_symbols.json), OnSelectionChanged 0x180530
// (absent from that map; resolved through mfc140u.dll's export table, and it
// is also slot 107 of the CVSToolsListBox vftable 0x18031b7e8).
// Every virtual call in these bodies is `call/jmp *0x1802c7b30`: that is the
// load config's GuardCFDispatchFunctionPointer (Control Flow Guard dispatch),
// just past the IAT (0x1802c6000..0x1802c7b20), not an import slot; there are
// no import calls.
//
// Retail vftable slots used (vtdump of 0x18031b7e8, names from the declaration
// order of afxvslistbox.h, which the resolved entries confirm):
//     94 (+0x2f0) RemoveItem       96 (+0x300) GetSelItem  (CVSListBox 0x1c69c0)
//     98 (+0x310) GetItemText      100 (+0x320) GetItemData (CVSListBox 0x1c6870)
//     101 (+0x328) SetItemData     107 (+0x358) OnSelectionChanged (0x180530)
// CMFCToolBarsCustomizeDialog vftable 0x1803190d8 (afxtoolbarscustomizedialog.h
// :153-155): 99 (+0x318) OnBeforeChangeTool, 100 (+0x320) OnAfterChangeTool;
// both base bodies are the shared `ret` at 0x27d0.
//
// Direct callees (mfc140u): CMFCToolBarsToolsPropertyPage::CreateNewTool
// 0x180830 and ::EnableControls 0x180930, CWnd::UpdateData 0x2910d0,
// CSimpleStringT::operator= 0xde30, CSimpleStringT::Empty 0x33b0,
// CUserToolsManager::MoveToolUp 0x183e00 / ::MoveToolDown 0x183e70, and
// CUserToolsManager::RemoveTool at 0x183d20 (no entry in the mfc140u map, but
// that is the export-table RVA of ?RemoveTool@CUserToolsManager@@ in mfc140u,
// as are 0x2910d0 for UpdateData and 0x1c6b00 / 0x1c6890 for CVSListBox::
// RemoveItem / SetItemData, slots 94 / 101).  The manager is the exported datum
// ?afxUserToolsManager@@3PEAVCUserToolsManager@@EA (0x1803be3b0).
//
// DEVIATIONS, applied uniformly:
//  (1) Virtual calls on `this`.  These thunks are reached from a client's
//      MSVC-laid-out CVSToolsListBox (or a class derived from it): OpenMFC
//      exports no CVSToolsListBox vftable, and the page's own m_wndToolsList
//      carries the vptr of OpenMFC's mingw CVSListBox, whose overrides do not
//      route here (CMFCToolBarsToolsPropertyPage.cpp deviation 1).  So a
//      vtable lying outside this image is called at the retail slot, exactly
//      as retail does; a vtable inside this image is OpenMFC's own, has no
//      such slots, and the call is devirtualised to the exported CVSListBox
//      thunk (slot 107 to this file's own OnSelectionChanged).  This is the
//      scheme CMFCToolBarsToolsPropertyPage.cpp uses for m_pParentSheet.
//  (2) Virtual calls on m_pParentSheet (slots 99/100): dispatched only when
//      the sheet is non-NULL and its vtable lies outside this image; otherwise
//      the retail base body (an empty `ret`) is what runs, i.e. nothing.
//      Retail tests neither; in this tree m_pParentSheet is set only by the
//      page's OnInitDialog, which is a stub, so it is NULL in practice.
//  (3) afxUserToolsManager.  Never assigned in this tree (NULL; see
//      featurepack/CMFC_misc_stubs.cpp).  Retail passes it unchecked; here a
//      NULL manager skips the manager call, as CMFCPopupMenuBar.cpp and
//      CMFCToolBarsCustomizeDialog.cpp do before the same kind of call.
//  (4) NULL checks.  Retail tests neither `this`, m_pParent, nor the tool
//      pointer OnAfterRenameItem reads from the item data; each added check
//      here is named at its site.
// ===========================================================================

// ---------------------------------------------------------------------------
// Thunks this file calls.  Each signature matches its definition in the file
// named on its line.
// ---------------------------------------------------------------------------
extern "C" int       MS_ABI impl__RemoveItem_CVSListBox__UEAAHH_Z(CVSListBox* pThis, int p0);                          // featurepack/controls/CVSListBox.cpp
extern "C" int       MS_ABI impl__GetSelItem_CVSListBox__UEBAHXZ(const CVSListBox* pThis);                            // featurepack/controls/CVSListBox.cpp
extern "C" uintptr_t MS_ABI impl__GetItemData_CVSListBox__UEBA_KH_Z(const CVSListBox* pThis, int p0);                 // featurepack/controls/CVSListBox.cpp
extern "C" void      MS_ABI impl__SetItemData_CVSListBox__UEAAXH_K_Z(CVSListBox* pThis, int p0, uintptr_t p1);        // featurepack/controls/CVSListBox.cpp
// NOTE: this definition takes the hidden return pointer FIRST (before pThis),
// unlike the MSVC member-function convention (this in RCX, return slot in RDX)
// the retail call sites use; it is declared here exactly as it is defined.
extern "C" void      MS_ABI impl__GetItemText_CVSListBox__UEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__H_Z(
    CString* __ret, const CVSListBox* pThis, int p0);                                                                  // featurepack/controls/CVSListBox.cpp

extern "C" void* MS_ABI impl__CreateNewTool_CMFCToolBarsToolsPropertyPage__IEAAPEAVCUserTool__XZ(void* pThis);        // featurepack/customize/CMFCToolBarsToolsPropertyPage.cpp
extern "C" void  MS_ABI impl__EnableControls_CMFCToolBarsToolsPropertyPage__IEAAXXZ(void* pThis);                     // featurepack/customize/CMFCToolBarsToolsPropertyPage.cpp
extern "C" int   MS_ABI impl__UpdateData_CWnd__QEAAHH_Z(CWnd* pThis, int p0);                                          // core/window/Thunks.cpp

extern "C" int MS_ABI impl__RemoveTool_CUserToolsManager__QEAAHPEAVCUserTool___Z(void* pThis, void* tool);             // featurepack/customize/CUserToolsManager.cpp
extern "C" int MS_ABI impl__MoveToolUp_CUserToolsManager__QEAAHPEAVCUserTool___Z(void* pThis, void* tool);             // featurepack/customize/CUserToolsManager.cpp
extern "C" int MS_ABI impl__MoveToolDown_CUserToolsManager__QEAAHPEAVCUserTool___Z(void* pThis, void* tool);           // featurepack/customize/CUserToolsManager.cpp

// ?afxUserToolsManager@@3PEAVCUserToolsManager@@EA (featurepack/CMFC_misc_stubs.cpp; NULL here).
extern "C" void* impl__afxUserToolsManager__3PEAVCUserToolsManager__EA;

// This file's own override, called from OnAfterAddItem (deviation 1).
extern "C" void MS_ABI impl__OnSelectionChanged_CVSToolsListBox__UEAAXXZ(CVSListBox* pThis);

// The mingw linker's image base symbol (deviations 1 and 2).
extern "C" IMAGE_DOS_HEADER __ImageBase;

namespace {

// ---------------------------------------------------------------------------
// Layouts.
// ---------------------------------------------------------------------------
// CVSToolsListBox: the CVSListBox part, then m_pParent.  Every body below
// reads the parent as `mov 0x348(%rcx|%rbx)`, and the page constructor
// (0x17fce0) stores it with `mov %r14,0x348(%rbx)` for rbx = page+0x1b88.
struct alignas(8) S_ToolsList {
    unsigned char m_base[0x348];   // +0x000 CVSListBox
    void*         m_pParent;       // +0x348 CMFCToolBarsToolsPropertyPage*
};
static_assert(offsetof(S_ToolsList, m_pParent) == 0x348, "every body: mov 0x348(this) = m_pParent");
static_assert(sizeof(CVSListBox) <= 0x348, "OpenMFC's CVSListBox fits below m_pParent (the page places it at +0x1b88)");

// The CMFCToolBarsToolsPropertyPage members these bodies touch (offsets as
// read here; the page constructor 0x17fce0 initialises each at the same one).
struct alignas(8) S_ToolsPageView {
    unsigned char m_pre[0x1ed8];                  // CPropertyPage + controls + m_wndToolsList
    unsigned char m_strCommand[8];                // +0x1ed8 CString
    unsigned char m_strArguments[8];              // +0x1ee0 CString
    unsigned char m_strInitialDirectory[8];       // +0x1ee8 CString
    CUserTool*    m_pSelTool;                     // +0x1ef0
    void*         m_pParentSheet;                 // +0x1ef8 CMFCToolBarsCustomizeDialog*
};
static_assert(offsetof(S_ToolsPageView, m_strCommand) == 0x1ed8, "OnSelectionChanged: add $0x1ed8 to m_pParent");
static_assert(offsetof(S_ToolsPageView, m_strArguments) == 0x1ee0, "OnSelectionChanged: add $0x1ee0");
static_assert(offsetof(S_ToolsPageView, m_strInitialDirectory) == 0x1ee8, "OnSelectionChanged: add $0x1ee8");
static_assert(offsetof(S_ToolsPageView, m_pSelTool) == 0x1ef0, "OnBeforeRemoveItem: movq $0,0x1ef0(m_pParent)");
static_assert(offsetof(S_ToolsPageView, m_pParentSheet) == 0x1ef8, "OnSelectionChanged: mov 0x1ef8(m_pParent)");
static_assert(sizeof(CString) == 8, "one CStringT data pointer");

// CUserTool members read here (OpenMFC's CUserTool has the retail layout).
struct CUserToolAccess : CUserTool { using CUserTool::m_strCommand; };
static_assert(offsetof(CUserTool, m_strLabel) == 0x08, "OnAfterAddItem/OnAfterRenameItem: lea 0x8(tool)");
static_assert(offsetof(CUserTool, m_strArguments) == 0x10, "OnSelectionChanged: lea 0x10(tool)");
static_assert(offsetof(CUserTool, m_strInitialDirectory) == 0x18, "OnSelectionChanged: lea 0x18(tool)");
static_assert(offsetof(CUserToolAccess, m_strCommand) == 0x28, "OnSelectionChanged: lea 0x28(tool)");

inline S_ToolsList* L(CVSListBox* p) { return reinterpret_cast<S_ToolsList*>(p); }
inline S_ToolsPageView* Page(CVSListBox* p) { return static_cast<S_ToolsPageView*>(L(p)->m_pParent); }
inline CString* Str(unsigned char* storage) { return reinterpret_cast<CString*>(storage); }

bool PointsIntoThisImage(const void* p) {
    const unsigned char* base = reinterpret_cast<const unsigned char*>(&__ImageBase);
    const IMAGE_NT_HEADERS* nt = reinterpret_cast<const IMAGE_NT_HEADERS*>(
        base + reinterpret_cast<const IMAGE_DOS_HEADER*>(base)->e_lfanew);
    const unsigned char* q = static_cast<const unsigned char*>(p);
    return q >= base && q < base + nt->OptionalHeader.SizeOfImage;
}

// The object's vtable when it is a client (MSVC-layout) one, else NULL.
void* const* ClientVtbl(const void* obj) {
    void* const* vptr = *static_cast<void* const* const*>(obj);
    return (vptr != nullptr && !PointsIntoThisImage(vptr)) ? vptr : nullptr;
}

// Retail vftable slots on `this` (see the file header).
constexpr int kSlotRemoveItem         = 94;
constexpr int kSlotGetSelItem         = 96;
constexpr int kSlotGetItemText        = 98;
constexpr int kSlotGetItemData        = 100;
constexpr int kSlotSetItemData        = 101;
constexpr int kSlotOnSelectionChanged = 107;
// ... and on m_pParentSheet.
constexpr int kSlotOnBeforeChangeTool = 99;
constexpr int kSlotOnAfterChangeTool  = 100;

typedef int       (MS_ABI *RemoveItemFn)(void* pThis, int iIndex);
typedef int       (MS_ABI *GetSelItemFn)(const void* pThis);
typedef CString*  (MS_ABI *GetItemTextFn)(const void* pThis, CString* pRet, int iIndex);   // MSVC: this, then the return slot
typedef uintptr_t (MS_ABI *GetItemDataFn)(const void* pThis, int iIndex);
typedef void      (MS_ABI *SetItemDataFn)(void* pThis, int iIndex, uintptr_t dwData);
typedef void      (MS_ABI *VoidFn)(void* pThis);
typedef void      (MS_ABI *ChangeToolFn)(void* pThis, CUserTool* pSelTool);

int VRemoveItem(CVSListBox* p, int i) {
    if (void* const* vt = ClientVtbl(p)) return reinterpret_cast<RemoveItemFn>(vt[kSlotRemoveItem])(p, i);
    return impl__RemoveItem_CVSListBox__UEAAHH_Z(p, i);
}
int VGetSelItem(CVSListBox* p) {
    if (void* const* vt = ClientVtbl(p)) return reinterpret_cast<GetSelItemFn>(vt[kSlotGetSelItem])(p);
    return impl__GetSelItem_CVSListBox__UEBAHXZ(p);
}
// Constructs the item text into *pRet (raw storage); the caller destroys it.
void VGetItemText(CVSListBox* p, CString* pRet, int i) {
    if (void* const* vt = ClientVtbl(p)) { reinterpret_cast<GetItemTextFn>(vt[kSlotGetItemText])(p, pRet, i); return; }
    impl__GetItemText_CVSListBox__UEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__H_Z(pRet, p, i);
}
uintptr_t VGetItemData(CVSListBox* p, int i) {
    if (void* const* vt = ClientVtbl(p)) return reinterpret_cast<GetItemDataFn>(vt[kSlotGetItemData])(p, i);
    return impl__GetItemData_CVSListBox__UEBA_KH_Z(p, i);
}
void VSetItemData(CVSListBox* p, int i, uintptr_t d) {
    if (void* const* vt = ClientVtbl(p)) { reinterpret_cast<SetItemDataFn>(vt[kSlotSetItemData])(p, i, d); return; }
    impl__SetItemData_CVSListBox__UEAAXH_K_Z(p, i, d);
}
void VOnSelectionChanged(CVSListBox* p) {
    if (void* const* vt = ClientVtbl(p)) { reinterpret_cast<VoidFn>(vt[kSlotOnSelectionChanged])(p); return; }
    impl__OnSelectionChanged_CVSToolsListBox__UEAAXXZ(p);
}
// Deviation 2: only a client sheet vtable is called; OpenMFC's own sheet
// stands for the retail base body, which is an empty `ret`.
void SheetChangeTool(void* pSheet, int slot, CUserTool* pTool) {
    if (pSheet == nullptr) return;
    if (void* const* vt = ClientVtbl(pSheet)) reinterpret_cast<ChangeToolFn>(vt[slot])(pSheet, pTool);
}

// pTool->m_strLabel = GetItemText(iItem): the temporary, the assignment
// (0xde30) and the temporary's release, as retail inlines them.
void CopyItemTextToLabel(CVSListBox* p, int iItem, CUserTool* pTool) {
    alignas(CString) unsigned char tmp[sizeof(CString)];
    CString* pText = reinterpret_cast<CString*>(tmp);
    VGetItemText(p, pText, iItem);
    pTool->m_strLabel = *pText;
    pText->~CString();
}

} // namespace

// Retail (RVA 0x180370, mfc140u), fully transcribed:
//     CUserTool* pTool = m_pParent->CreateNewTool();          // 0x180830
//     if (pTool == NULL) { RemoveItem(iItem); return; }       // vslot 94 (+0x2f0), result dropped
//     pTool->m_strLabel = GetItemText(iItem);                 // vslot 98 (+0x310); +0x8, 0xde30
//     SetItemData(iItem, (DWORD_PTR)pTool);                   // vslot 101 (+0x328)
//     OnSelectionChanged();                                   // vslot 107 (+0x358)
// NOTE: OpenMFC's CMFCToolBarsToolsPropertyPage::CreateNewTool is itself a
// stub returning NULL (see its file), so today every add takes the
// RemoveItem path -- which is what retail does when CreateNewTool fails.
// DEVIATIONS: 1 (virtual dispatch), 4 (NULL `this` returns; a NULL m_pParent
// is passed to CreateNewTool as retail would -- that thunk ignores it).
// Symbol: ?OnAfterAddItem@CVSToolsListBox@@UEAAXH@Z
extern "C" void MS_ABI impl__OnAfterAddItem_CVSToolsListBox__UEAAXH_Z(CVSListBox* pThis, int nItem) {
    if (pThis == nullptr) return;
    CUserTool* pTool = static_cast<CUserTool*>(
        impl__CreateNewTool_CMFCToolBarsToolsPropertyPage__IEAAPEAVCUserTool__XZ(L(pThis)->m_pParent));
    if (pTool == nullptr) {
        VRemoveItem(pThis, nItem);
        return;
    }
    CopyItemTextToLabel(pThis, nItem, pTool);
    VSetItemData(pThis, nItem, reinterpret_cast<uintptr_t>(pTool));
    VOnSelectionChanged(pThis);
}

// Retail (RVA 0x180500, mfc140u), fully transcribed:
//     afxUserToolsManager->MoveToolDown((CUserTool*)GetItemData(iItem));   // vslot 100 first,
//                                                                          // then tail jump 0x183e70
// DEVIATIONS: 1, 3 (NULL manager: GetItemData still runs, the move does not), 4.
// Symbol: ?OnAfterMoveItemDown@CVSToolsListBox@@UEAAXH@Z
extern "C" void MS_ABI impl__OnAfterMoveItemDown_CVSToolsListBox__UEAAXH_Z(CVSListBox* pThis, int nItem) {
    if (pThis == nullptr) return;
    void* pTool = reinterpret_cast<void*>(VGetItemData(pThis, nItem));
    void* pMgr = impl__afxUserToolsManager__3PEAVCUserToolsManager__EA;
    if (pMgr != nullptr) impl__MoveToolDown_CUserToolsManager__QEAAHPEAVCUserTool___Z(pMgr, pTool);
}

// Retail (RVA 0x1804d0, mfc140u), fully transcribed:
//     afxUserToolsManager->MoveToolUp((CUserTool*)GetItemData(iItem));     // vslot 100 first,
//                                                                          // then tail jump 0x183e00
// DEVIATIONS: 1, 3 (NULL manager: GetItemData still runs, the move does not), 4.
// Symbol: ?OnAfterMoveItemUp@CVSToolsListBox@@UEAAXH@Z
extern "C" void MS_ABI impl__OnAfterMoveItemUp_CVSToolsListBox__UEAAXH_Z(CVSListBox* pThis, int nItem) {
    if (pThis == nullptr) return;
    void* pTool = reinterpret_cast<void*>(VGetItemData(pThis, nItem));
    void* pMgr = impl__afxUserToolsManager__3PEAVCUserToolsManager__EA;
    if (pMgr != nullptr) impl__MoveToolUp_CUserToolsManager__QEAAHPEAVCUserTool___Z(pMgr, pTool);
}

// Retail (RVA 0x180440, mfc140u), fully transcribed:
//     CUserTool* pTool = (CUserTool*)GetItemData(iItem);      // vslot 100 (+0x320)
//     pTool->m_strLabel = GetItemText(iItem);                 // vslot 98 (+0x310); +0x8, 0xde30
// Retail does not test pTool: GetItemText is called and the assignment
// faults on a NULL item.  DEVIATIONS: 1; 4 (NULL `this` returns, and a NULL
// pTool returns after GetItemData, before GetItemText).
// Symbol: ?OnAfterRenameItem@CVSToolsListBox@@UEAAXH@Z
extern "C" void MS_ABI impl__OnAfterRenameItem_CVSToolsListBox__UEAAXH_Z(CVSListBox* pThis, int nItem) {
    if (pThis == nullptr) return;
    CUserTool* pTool = reinterpret_cast<CUserTool*>(VGetItemData(pThis, nItem));
    if (pTool == nullptr) return;
    CopyItemTextToLabel(pThis, nItem, pTool);
}

// Retail (RVA 0x180320, mfc140u), fully transcribed:
//     afxUserToolsManager->RemoveTool((CUserTool*)GetItemData(iItem));  // vslot 100, then 0x183d20
//     m_pParent->m_pSelTool = NULL;                                     // +0x1ef0
//     return TRUE;                                                      // RemoveTool's result ignored
// DEVIATIONS: 1; 3 (NULL manager: no RemoveTool call); 4 (NULL `this`
// returns TRUE -- the value retail always returns -- and a NULL m_pParent
// skips the store).
// Symbol: ?OnBeforeRemoveItem@CVSToolsListBox@@UEAAHH@Z
extern "C" int MS_ABI impl__OnBeforeRemoveItem_CVSToolsListBox__UEAAHH_Z(CVSListBox* pThis, int nItem) {
    if (pThis == nullptr) return TRUE;
    void* pTool = reinterpret_cast<void*>(VGetItemData(pThis, nItem));
    void* pMgr = impl__afxUserToolsManager__3PEAVCUserToolsManager__EA;
    if (pMgr != nullptr) impl__RemoveTool_CUserToolsManager__QEAAHPEAVCUserTool___Z(pMgr, pTool);
    if (S_ToolsPageView* page = Page(pThis)) page->m_pSelTool = nullptr;
    return TRUE;
}

// Retail (RVA 0x180530, mfc140u), fully transcribed:
//     int iSel = GetSelItem();                                            // vslot 96 (+0x300)
//     CUserTool* pSel = iSel >= 0 ? (CUserTool*)GetItemData(iSel) : NULL; // vslot 100 (+0x320)
//     if (pSel == NULL) {
//         m_pParent->m_strCommand.Empty();                                // 0x33b0, x3
//         m_pParent->m_strArguments.Empty();
//         m_pParent->m_strInitialDirectory.Empty();
//     } else {
//         m_pParent->m_strCommand          = pSel->m_strCommand;          // +0x28, 0xde30
//         m_pParent->m_strArguments        = pSel->m_strArguments;        // +0x10
//         m_pParent->m_strInitialDirectory = pSel->m_strInitialDirectory; // +0x18
//     }
//     m_pParent->m_pParentSheet->OnBeforeChangeTool(m_pParent->m_pSelTool);  // vslot 99 (+0x318), the OLD tool
//     m_pParent->m_pSelTool = pSel;
//     m_pParent->UpdateData(FALSE);                                       // 0x2910d0
//     m_pParent->EnableControls();                                        // 0x180930
//     m_pParent->m_pParentSheet->OnAfterChangeTool(m_pParent->m_pSelTool);   // vslot 100 (+0x320), tail jump
// DEVIATIONS: 1; 2 (sheet slots); 4 (NULL `this` returns; a NULL m_pParent
// returns after GetSelItem/GetItemData, since every later step dereferences it).
// Symbol: ?OnSelectionChanged@CVSToolsListBox@@UEAAXXZ
extern "C" void MS_ABI impl__OnSelectionChanged_CVSToolsListBox__UEAAXXZ(CVSListBox* pThis) {
    if (pThis == nullptr) return;
    const int iSel = VGetSelItem(pThis);
    CUserTool* pSel = iSel >= 0 ? reinterpret_cast<CUserTool*>(VGetItemData(pThis, iSel)) : nullptr;
    S_ToolsPageView* page = Page(pThis);
    if (page == nullptr) return;
    if (pSel == nullptr) {
        Str(page->m_strCommand)->Empty();
        Str(page->m_strArguments)->Empty();
        Str(page->m_strInitialDirectory)->Empty();
    } else {
        *Str(page->m_strCommand) = pSel->GetCommand();
        *Str(page->m_strArguments) = pSel->m_strArguments;
        *Str(page->m_strInitialDirectory) = pSel->m_strInitialDirectory;
    }
    SheetChangeTool(page->m_pParentSheet, kSlotOnBeforeChangeTool, page->m_pSelTool);
    page->m_pSelTool = pSel;
    impl__UpdateData_CWnd__QEAAHH_Z(reinterpret_cast<CWnd*>(page), FALSE);
    impl__EnableControls_CMFCToolBarsToolsPropertyPage__IEAAXXZ(page);
    SheetChangeTool(page->m_pParentSheet, kSlotOnAfterChangeTool, page->m_pSelTool);
}
