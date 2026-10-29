// CMFCRibbonComboBox — OpenMFC implementation.
// Sources: retail mfc140u disassembly where resolved, ANSI-twin bodies where noted,
// and the retail afxribboncombobox.h declaration.

#include "detail/ManualSmallStubImplementationsSupport.h"

#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>

// Verified thunk definitions used below (core/collections/CStringList.cpp,
// core/collections/CStringT.cpp, and featurepack/ribbon/Globals.cpp).
extern "C" CStringList::POSITION MS_ABI
impl__FindIndex_CStringList__QEBAPEAU__POSITION____J_Z(const CStringList* pThis, long long nIndex);
extern "C" CStringList::POSITION MS_ABI
impl__AddTail_CStringList__QEAAPEAU__POSITION__PEB_W_Z(CStringList* pThis, const wchar_t* value);
extern "C" void MS_ABI
impl__RemoveAt_CStringList__QEAAXPEAU__POSITION___Z(CStringList* pThis, CStringList::POSITION* pPos);
extern "C" int MS_ABI
impl__CompareNoCase___CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEBAHPEB_W_Z(
    const CString* pThis, const wchar_t* psz);
extern "C" void MS_ABI impl__AfxThrowInvalidArgException__YAXXZ();
extern "C" void MS_ABI impl__AfxThrowMemoryException__YAXXZ();
extern "C" void MS_ABI impl___0CStringList__QEAA__J_Z(CStringList* pThis, long long blockSize);
extern "C" void MS_ABI impl___1CStringList__UEAA_XZ(CStringList* pThis);
extern "C" void* MS_ABI impl___0CMFCRibbonEdit__IEAA_XZ(void* pThis);
extern "C" void* MS_ABI impl___0CMFCRibbonEdit__QEAA_IHPEB_WH_Z(
    void* pThis, unsigned int id, int width, const wchar_t* text, int image);
extern "C" void MS_ABI impl___1CMFCRibbonEdit__UEAA_XZ(void* pThis);
extern "C" void MS_ABI impl__RemoveAll_CStringList__QEAAXXZ(CStringList* pThis);
extern "C" int MS_ABI impl__SetACCData_CMFCRibbonEdit__UEAAHPEAVCWnd__AEAVCAccessibilityData___Z(
    void* pThis, void* pParent, void* pData);
extern "C" int MS_ABI impl__FindItem_CMFCRibbonComboBox__QEBAHPEB_W_Z(
    const void* pThis, const wchar_t* text);
extern "C" int MS_ABI impl__DeleteItem_CMFCRibbonComboBox__QEAAHH_Z(
    void* pThis, int index);

namespace {
// CMFCRibbonComboBox is not declared in include/openmfc, and has no shared
// detail shadow. These are only the fields used by the decoded bodies below.
// The two list objects begin at +0x2b0 and +0x2e8; the public list fields
// follow at +0x320 (m_iSelIndex), +0x324 (m_nDropDownHeight), +0x328
// (m_bHasEditBox), and +0x32c (m_bResizeDropDownList).
struct RibbonComboLayout {
    unsigned char beforeBaseField148[0x148];
    int baseField_148;
    unsigned char beforeBaseWidth[0x1c0 - 0x14c];
    int baseField_1c0;
    unsigned char beforeCommonFlag[0x288 - 0x1c4];
    int baseField_288;
    unsigned char beforeLists[0x2b0 - 0x28c];
    unsigned char itemStrings[0x38];
    unsigned char itemData[0x38];
    int selectedIndex;
    int dropDownHeight;
    int hasEditBox;
    int resizeDropDownList;
};
static_assert(offsetof(RibbonComboLayout, baseField_148) == 0x148, "retail field @+0x148");
static_assert(offsetof(RibbonComboLayout, baseField_1c0) == 0x1c0, "retail field @+0x1c0");
static_assert(offsetof(RibbonComboLayout, baseField_288) == 0x288, "retail field @+0x288");
static_assert(offsetof(RibbonComboLayout, itemStrings) == 0x2b0, "m_lstItems @+0x2b0");
static_assert(offsetof(RibbonComboLayout, itemData) == 0x2e8, "m_lstItemData @+0x2e8");
static_assert(offsetof(RibbonComboLayout, selectedIndex) == 0x320, "m_iSelIndex @+0x320");
static_assert(offsetof(RibbonComboLayout, dropDownHeight) == 0x324, "m_nDropDownHeight @+0x324");
static_assert(offsetof(RibbonComboLayout, hasEditBox) == 0x328, "m_bHasEditBox @+0x328");
static_assert(offsetof(RibbonComboLayout, resizeDropDownList) == 0x32c, "m_bResizeDropDownList @+0x32c");
static_assert(sizeof(RibbonComboLayout) == 0x330, "retail CMFCRibbonComboBox size is 0x330");

// Retail afxtempl.h's CList derives from CObject, then stores head/tail/count,
// free list, CPlex blocks, and block size. CMFCRibbonComboBox's ctor at RVA
// 0xf5a80 (mfc140u) initializes the embedded list fields at +0x2b0 and +0x2e8;
// GetItemData at RVA 0xf5c90 (mfc140u) confirms head/count/node-data offsets.
// The {next, prev, data} node shape is also used by OpenMFC's CStringList nodes.
struct RibbonComboDataNode {
    RibbonComboDataNode* next;
    RibbonComboDataNode* prev;
    std::uintptr_t data;
};
struct RibbonComboNativeList {
    void* vtable;
    RibbonComboDataNode* head;
    RibbonComboDataNode* tail;
    long long count;
    RibbonComboDataNode* freeNodes;
    void* blocks;
    long long blockSize;
};
struct RibbonComboStringNode {
    RibbonComboStringNode* next;
    RibbonComboStringNode* prev;
    CString data;
};
struct RibbonComboAccessibilityDataLayout {
    unsigned char beforeRole[0x30];
    int role;
    int state;
};
static_assert(offsetof(RibbonComboNativeList, head) == 0x08, "CList head @+0x08");
static_assert(offsetof(RibbonComboNativeList, tail) == 0x10, "CList tail @+0x10");
static_assert(offsetof(RibbonComboNativeList, count) == 0x18, "CList count @+0x18");
static_assert(offsetof(RibbonComboNativeList, freeNodes) == 0x20, "CList free list @+0x20");
static_assert(offsetof(RibbonComboNativeList, blocks) == 0x28, "CList blocks @+0x28");
static_assert(offsetof(RibbonComboNativeList, blockSize) == 0x30, "CList block size @+0x30");
static_assert(sizeof(RibbonComboNativeList) == 0x38, "retail CList size is 0x38");
static_assert(offsetof(RibbonComboDataNode, data) == 0x10, "CList node data @+0x10");
static_assert(sizeof(RibbonComboDataNode) == 0x18, "pointer-sized CList node is 0x18 bytes");
static_assert(offsetof(RibbonComboStringNode, data) == 0x10, "CStringList node data @+0x10");
static_assert(offsetof(RibbonComboAccessibilityDataLayout, role) == 0x30, "accessibility role @+0x30");
static_assert(offsetof(RibbonComboAccessibilityDataLayout, state) == 0x34, "accessibility state @+0x34");
static_assert(sizeof(CStringList::POSITION) == sizeof(void*), "CStringList POSITION is one pointer");

RibbonComboLayout* Combo(void* pThis) {
    return static_cast<RibbonComboLayout*>(pThis);
}
CStringList* ItemStrings(void* pThis) {
    return reinterpret_cast<CStringList*>(static_cast<unsigned char*>(pThis) + 0x2b0);
}
RibbonComboNativeList* ItemData(void* pThis) {
    return reinterpret_cast<RibbonComboNativeList*>(static_cast<unsigned char*>(pThis) + 0x2e8);
}
RibbonComboStringNode* StringNodeFromPosition(CStringList::POSITION position) {
    RibbonComboStringNode* node = nullptr;
    std::memcpy(&node, &position, sizeof(node));
    return node;
}
RibbonComboStringNode* StringNodeAt(void* pThis, int index) {
    if (index < 0) return nullptr;
    return StringNodeFromPosition(impl__FindIndex_CStringList__QEBAPEAU__POSITION____J_Z(
        ItemStrings(pThis), static_cast<long long>(index)));
}
RibbonComboDataNode* DataNodeAt(void* pThis, int index) {
    if (index < 0) return nullptr;
    RibbonComboNativeList* list = ItemData(pThis);
    if (static_cast<long long>(index) >= list->count) return nullptr;
    RibbonComboDataNode* node = list->head;
    while (node != nullptr && index-- != 0) node = node->next;
    return node;
}
int ItemStringCount(void* pThis) {
    int count = 0;
    for (RibbonComboStringNode* node = StringNodeAt(pThis, 0);
         node != nullptr; node = node->next) {
        ++count;
    }
    return count;
}
void AppendItemData(void* pThis, std::uintptr_t data) {
    RibbonComboNativeList* list = ItemData(pThis);
    auto* node = static_cast<RibbonComboDataNode*>(std::malloc(sizeof(RibbonComboDataNode)));
    if (node == nullptr) {
        impl__AfxThrowMemoryException__YAXXZ();
        return;
    }
    node->next = nullptr;
    node->prev = list->tail;
    node->data = data;
    if (list->tail != nullptr) list->tail->next = node;
    else list->head = node;
    list->tail = node;
    ++list->count;
}
void RemoveItemDataAt(void* pThis, int index) {
    RibbonComboNativeList* list = ItemData(pThis);
    RibbonComboDataNode* node = DataNodeAt(pThis, index);
    if (node == nullptr) return;
    if (node->prev != nullptr) node->prev->next = node->next;
    else list->head = node->next;
    if (node->next != nullptr) node->next->prev = node->prev;
    else list->tail = node->prev;
    --list->count;
    std::free(node);
}
void ClearItemData(void* pThis) {
    RibbonComboNativeList* list = ItemData(pThis);
    RibbonComboDataNode* node = list->head;
    while (node != nullptr) {
        RibbonComboDataNode* next = node->next;
        std::free(node);
        node = next;
    }
    list->head = nullptr;
    list->tail = nullptr;
    list->count = 0;
}
void InitializeComboStorage(void* pThis) {
    // Retail's constructors initialize these embedded collections and then
    // store the same scalar defaults as CommonInit (RVA 0xf5a80/0xf5930,
    // mfc140u). Initialize CStringList through its side-table constructor and
    // only the item-data fields used by this file's custom node representation.
    impl___0CStringList__QEAA__J_Z(ItemStrings(pThis), 10);
    RibbonComboNativeList* data = ItemData(pThis);
    data->head = nullptr;
    data->tail = nullptr;
    data->count = 0;
    data->freeNodes = nullptr;
    data->blocks = nullptr;
    data->blockSize = 10;
    RibbonComboLayout* self = Combo(pThis);
    // The retail base-element constructor at RVA 0xfb30 (mfc140u) zeroes
    // the qword at +0x144, which includes the +0x148 flag read by SetACCData.
    self->baseField_148 = 0;
    self->baseField_288 = 1;
    self->selectedIndex = -1;
    self->dropDownHeight = 150;
    self->resizeDropDownList = 1;
    self->baseField_1c0 = 3;
}
} // namespace

// Retail RVA 0xf5a80 (mfc140u) constructs the edit base and initializes the
// combo collections/defaults. The local edit-base constructor is called via
// its verified thunk; item-data fields use this file's malloc-backed storage.
// This local shadow cannot install the ComboBox-specific vftable, so its
// initialized state supports explicit export thunks, not retail virtual dispatch.
// Symbol: ??0CMFCRibbonComboBox@@IEAA@XZ
extern "C" void* MS_ABI impl___0CMFCRibbonComboBox__IEAA_XZ(void* pThis) {
    if (pThis == nullptr) return nullptr;
    impl___0CMFCRibbonEdit__IEAA_XZ(pThis);
    InitializeComboStorage(pThis);
    return pThis;
}
// Retail RVA 0xf5930 (mfc140u) constructs the edit base with the ID, normalized
// height, text, and image, then stores the width argument at +0x328.
// Symbol: ??0CMFCRibbonComboBox@@QEAA@IHHPEB_WH@Z
extern "C" void* MS_ABI impl___0CMFCRibbonComboBox__QEAA_IHHPEB_WH_Z(
    void* pThis, unsigned int id, int width, int height, const wchar_t* text, int image) {
    if (pThis == nullptr) return nullptr;
    const int editHeight = height == -1 ? 108 : height;
    impl___0CMFCRibbonEdit__QEAA_IHPEB_WH_Z(pThis, id, editHeight, text, image);
    InitializeComboStorage(pThis);
    Combo(pThis)->hasEditBox = width;
    return pThis;
}
// Retail RVA 0xf5b90 (mfc140u) tears down item-data, CStringList, then the edit
// base. This frees this file's malloc-backed data nodes and delegates the
// side-table string/base teardown; it does not invoke retail's native CList
// fixed-block allocator destructor or reproduce retail vftable resets.
// Symbol: ??1CMFCRibbonComboBox@@UEAA@XZ
extern "C" void MS_ABI impl___1CMFCRibbonComboBox__UEAA_XZ(void* pThis) {
    ClearItemData(pThis);
    impl___1CStringList__UEAA_XZ(ItemStrings(pThis));
    impl___1CMFCRibbonEdit__UEAA_XZ(pThis);
}

// Retail (RVA 0xf5be0 (mfc140u)) checks for an existing case-insensitive
// string, appends the string and pointer-sized data only on a miss, and always
// returns item-count minus one. The data nodes use native CList links, but
// malloc replaces retail's fixed-block allocator. The caller must supply an
// object whose CList subobject has been initialized by construction.
// Symbol: ?AddItem@CMFCRibbonComboBox@@UEAA_JPEB_W_K@Z
extern "C" __int64 MS_ABI impl__AddItem_CMFCRibbonComboBox__UEAA_JPEB_W_K_Z(
    void* pThis, const wchar_t* text, unsigned __int64 data) {
    if (text == nullptr) {
        impl__AfxThrowInvalidArgException__YAXXZ();
        return -1;
    }
    if (impl__FindItem_CMFCRibbonComboBox__QEBAHPEB_W_Z(pThis, text) < 0) {
        impl__AddTail_CStringList__QEAAPEAU__POSITION__PEB_W_Z(ItemStrings(pThis), text);
        AppendItemData(pThis, static_cast<std::uintptr_t>(data));
    }
    return static_cast<__int64>(ItemStringCount(pThis)) - 1;
}

// The combo-specific ClearData export has no resolved body in either symbol
// map; unlike RemoveAllItems, its exact cleanup contract is not established.
// Symbol: ?ClearData@CMFCRibbonComboBox@@MEAAXXZ
extern "C" void MS_ABI impl__ClearData_CMFCRibbonComboBox__MEAAXXZ(void* pThis) {
    (void)pThis;
}

// Retail RVA 0xf5b50 (mfc140u) initializes +0x288 to 1, selection to -1,
// drop-down height to 150, resize flag to 1, and +0x1c0 to 3. It does not
// write the edit-box field at +0x328; these offsets are pinned above.
// Symbol: ?CommonInit@CMFCRibbonComboBox@@IEAAXXZ
extern "C" void MS_ABI impl__CommonInit_CMFCRibbonComboBox__IEAAXXZ(void* pThis) {
    RibbonComboLayout* self = Combo(pThis);
    self->baseField_1c0 = 3;
    self->baseField_288 = 1;
    self->selectedIndex = -1;
    self->dropDownHeight = 150;
    self->resizeDropDownList = 1;
}

// Retail RVA 0xf6400 (mfc140u) first copies the base element, then copies the
// inherited edit CString, destroys any edit control, clones both lists, and
// copies selection/height/resize state. The base/edit-control state and the
// full parent layout are not represented by this file's shadows.
// Symbol: ?CopyFrom@CMFCRibbonComboBox@@MEAAXAEBVCMFCRibbonBaseElement@@@Z
extern "C" void MS_ABI impl__CopyFrom_CMFCRibbonComboBox__MEAAXAEBVCMFCRibbonBaseElement___Z(
    void* pThis, const void* pSource) {
    (void)pThis;
    (void)pSource;
}

// Retail RVA 0xf58e0 (mfc140u) allocates 0x330 bytes and calls the protected
// constructor. The combo has no local C++ shadow/vtable, so this file cannot
// produce an object with valid combo-specific virtual dispatch; do not expose
// base-only constructed storage as a valid runtime-created combo.
// Symbol: ?CreateObject@CMFCRibbonComboBox@@SAPEAVCObject@@XZ
extern "C" void* MS_ABI impl__CreateObject_CMFCRibbonComboBox__SAPEAVCObject__XZ() {
    return nullptr;
}

// Retail RVA 0xf6000 (mfc140u) searches item-data nodes for the first matching
// value and delegates to DeleteItem(index). The item-list mutation requires
// the shared DeleteItem implementation below.
// Symbol: ?DeleteItem@CMFCRibbonComboBox@@QEAAH_K@Z
extern "C" int MS_ABI impl__DeleteItem_CMFCRibbonComboBox__QEAAH_K_Z(
    void* pThis, unsigned __int64 data) {
    int index = 0;
    for (RibbonComboDataNode* node = ItemData(pThis)->head;
         node != nullptr; node = node->next, ++index) {
        if (node->data == static_cast<std::uintptr_t>(data)) {
            return impl__DeleteItem_CMFCRibbonComboBox__QEAAHH_Z(pThis, index);
        }
    }
    return 0;
}

// Retail RVA 0xf5f40 (mfc140u) rejects out-of-range indices, removes both
// list entries, and reselects only when the removed item was selected. This
// transcription maintains those lists and the selection index; it cannot
// reproduce the omitted inherited edit-control text/redraw and peer updates.
// Symbol: ?DeleteItem@CMFCRibbonComboBox@@QEAAHH@Z
extern "C" int MS_ABI impl__DeleteItem_CMFCRibbonComboBox__QEAAHH_Z(void* pThis, int index) {
    if (index < 0 || index >= ItemStringCount(pThis)) return 0;
    RibbonComboLayout* self = Combo(pThis);
    RibbonComboStringNode* stringNode = StringNodeAt(pThis, index);
    if (stringNode == nullptr) return 0;
    CStringList::POSITION position = impl__FindIndex_CStringList__QEBAPEAU__POSITION____J_Z(
        ItemStrings(pThis), static_cast<long long>(index));
    impl__RemoveAt_CStringList__QEAAXPEAU__POSITION___Z(ItemStrings(pThis), &position);
    // Retail removes the string node before checking the parallel item-data
    // list; preserve that ordering even if the two lists are inconsistent.
    if (DataNodeAt(pThis, index) == nullptr) return 0;
    RemoveItemDataAt(pThis, index);
    if (self->selectedIndex == index) {
        const int remaining = ItemStringCount(pThis);
        self->selectedIndex = remaining == 0 ? -1 : (index < remaining ? index : remaining - 1);
    }
    return 1;
}

// Retail RVA 0xf6030 (mfc140u) finds the string and delegates to the indexed
// overload; a NULL string reaches AfxThrowInvalidArgException.
// Symbol: ?DeleteItem@CMFCRibbonComboBox@@QEAAHPEB_W@Z
extern "C" int MS_ABI impl__DeleteItem_CMFCRibbonComboBox__QEAAHPEB_W_Z(
    void* pThis, const wchar_t* text) {
    if (text == nullptr) {
        impl__AfxThrowInvalidArgException__YAXXZ();
        return 0;
    }
    const int index = impl__FindItem_CMFCRibbonComboBox__QEBAHPEB_W_Z(pThis, text);
    if (index < 0) return 0;
    return impl__DeleteItem_CMFCRibbonComboBox__QEAAHH_Z(pThis, index);
}

// Retail RVA 0xf6500 (mfc140u), when its inherited menu/control/bar guards
// permit, creates and initializes the ribbon drop-down popup, copies the item
// strings/data, selects the current row, and uses window/visual-manager state
// absent from this implementation; other paths return early.
// Symbol: ?DropDownList@CMFCRibbonComboBox@@MEAAXXZ
extern "C" void MS_ABI impl__DropDownList_CMFCRibbonComboBox__MEAAXXZ(void* pThis) {
    (void)pThis;
}

// Retail RVA 0xf6070 (mfc140u) scans the CStringList and calls _wcsicmp
// (IAT slot 0x1802c7788, resolved by iatu.py) for each element. The existing
// CStringT CompareNoCase thunk supplies that comparison in OpenMFC.
// Symbol: ?FindItem@CMFCRibbonComboBox@@QEBAHPEB_W@Z
extern "C" int MS_ABI impl__FindItem_CMFCRibbonComboBox__QEBAHPEB_W_Z(
    const void* pThis, const wchar_t* text) {
    if (text == nullptr) {
        impl__AfxThrowInvalidArgException__YAXXZ();
        return -1;
    }
    int index = 0;
    for (RibbonComboStringNode* node = StringNodeAt(const_cast<void*>(pThis), 0);
         node != nullptr; node = node->next, ++index) {
        if (impl__CompareNoCase___CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEBAHPEB_W_Z(
                &node->data, text) == 0) {
            return index;
        }
    }
    return -1;
}

// Stub: retail RVA 0xf60e0 (mfc140u) calls the inherited edit-size routine,
// consults a virtual size slot, and adds a value derived from +0x1c0 to the
// result. The required base sizing/vtable state is unavailable here.
// Symbol: ?GetIntermediateSize@CMFCRibbonComboBox@@UEAA?AVCSize@@PEAVCDC@@@Z
extern "C" void* MS_ABI impl__GetIntermediateSize_CMFCRibbonComboBox__UEAA_AVCSize__PEAVCDC___Z(
    void* pThis, void* pResult, void* pDC) {
    (void)pThis;
    (void)pResult;
    (void)pDC;
    return nullptr;
}

// Retail RVA 0xf5c50 (mfc140u) treats index -1 as the current selection and
// returns NULL when the resulting index is negative, outside the list, or has
// no CStringList node.
// Symbol: ?GetItem@CMFCRibbonComboBox@@QEBAPEB_WH@Z
extern "C" const wchar_t* MS_ABI impl__GetItem_CMFCRibbonComboBox__QEBAPEB_WH_Z(
    const void* pThis, int index) {
    const auto* self = static_cast<const RibbonComboLayout*>(pThis);
    if (index == -1) index = self->selectedIndex;
    if (index < 0 || index >= ItemStringCount(const_cast<void*>(pThis))) return nullptr;
    RibbonComboStringNode* node = StringNodeAt(const_cast<void*>(pThis), index);
    return node != nullptr ? node->data.GetString() : nullptr;
}

// Retail RVA 0xf5c90 (mfc140u) treats index -1 as the current selection and
// returns zero for invalid indices or a missing item-data node.
// Symbol: ?GetItemData@CMFCRibbonComboBox@@QEBA_KH@Z
extern "C" unsigned __int64 MS_ABI impl__GetItemData_CMFCRibbonComboBox__QEBA_KH_Z(
    const void* pThis, int index) {
    const auto* self = static_cast<const RibbonComboLayout*>(pThis);
    if (index == -1) index = self->selectedIndex;
    RibbonComboDataNode* node = DataNodeAt(const_cast<void*>(pThis), index);
    return node != nullptr ? static_cast<unsigned __int64>(node->data) : 0;
}

// Retail RVA 0xf68a0 (mfc140u) calls the inherited edit rect-change handler
// and then resizes/repositions its child control; that parent/control state is
// not represented by the available shadows.
// Symbol: ?OnAfterChangeRect@CMFCRibbonComboBox@@MEAAXPEAVCDC@@@Z
extern "C" void MS_ABI impl__OnAfterChangeRect_CMFCRibbonComboBox__MEAAXPEAVCDC___Z(
    void* pThis, void* pDC) {
    (void)pThis;
    (void)pDC;
}

// Retail RVA 0xf6130 (mfc140u) performs themed rendering using the inherited
// ribbon geometry, text, visual manager, and edit-control state not modeled
// here.
// Symbol: ?OnDraw@CMFCRibbonComboBox@@MEAAXPEAVCDC@@@Z
extern "C" void MS_ABI impl__OnDraw_CMFCRibbonComboBox__MEAAXPEAVCDC___Z(
    void* pThis, void* pDC) {
    (void)pThis;
    (void)pDC;
}

// This exact Unicode export has no resolved RVA; the ANSI twin's matching
// virtual body is likewise not resolved. Its rendering contract needs the
// unmodeled toolbar menu button and visual-manager state.
// Symbol: ?OnDrawDropListItem@CMFCRibbonComboBox@@MEAAHPEAVCDC@@HPEAVCMFCToolBarMenuButton@@H@Z
extern "C" int MS_ABI impl__OnDrawDropListItem_CMFCRibbonComboBox__MEAAHPEAVCDC__HPEAVCMFCToolBarMenuButton__H_Z(
    void* pThis, void* pDC, int index, void* pItem, int highlight) {
    (void)pThis;
    (void)pDC;
    (void)index;
    (void)pItem;
    (void)highlight;
    return 0;
}

// No mfc140u RVA is resolved. The ANSI twin's body at RVA 0x8960 (mfc140)
// writes a zero CSize to its hidden result buffer and returns that buffer.
// This is the exact body behavior rather than a bare default return.
// Symbol: ?OnGetDropListItemSize@CMFCRibbonComboBox@@MEAA?AVCSize@@PEAVCDC@@HPEAVCMFCToolBarMenuButton@@V2@@Z
extern "C" void* MS_ABI impl__OnGetDropListItemSize_CMFCRibbonComboBox__MEAA_AVCSize__PEAVCDC__HPEAVCMFCToolBarMenuButton__V2__Z(
    void* pThis, void* pResult, void* pDC, int index, void* pItem, long long sizeDefault) {
    (void)pThis;
    (void)pDC;
    (void)index;
    (void)pItem;
    (void)sizeDefault;
    const std::uint64_t emptySize = 0;
    std::memcpy(pResult, &emptySize, sizeof(emptySize));
    return pResult;
}

// The ANSI twin at RVA 0xf6f50 (mfc140) first calls an inherited mouse handler,
// hit-tests the drop-down rectangle, and can dispatch two inherited virtual
// slots. Those geometry/vtable thunks are not represented in this file.
// CPoint is an 8-byte by-value aggregate under the MS ABI.
// Symbol: ?OnLButtonDown@CMFCRibbonComboBox@@MEAAXVCPoint@@@Z
extern "C" void MS_ABI impl__OnLButtonDown_CMFCRibbonComboBox__MEAAXVCPoint___Z(
    void* pThis, long long point) {
    (void)pThis;
    (void)point;
}

// No matching body is resolved in either retail symbol map, and there is no
// modeled mouse-release/window state to transcribe safely.
// Symbol: ?OnLButtonUp@CMFCRibbonComboBox@@MEAAXVCPoint@@@Z
extern "C" void MS_ABI impl__OnLButtonUp_CMFCRibbonComboBox__MEAAXVCPoint___Z(
    void* pThis, long long point) {
    (void)pThis;
    (void)point;
}

// The ANSI twin at RVA 0xf7350 (mfc140) coordinates the parent ribbon's
// notification, edit HWND text, and owner/category selection state. These
// base/window dependencies are not available as faithful thunks here.
// Symbol: ?OnSelectItem@CMFCRibbonComboBox@@UEAAXH@Z
extern "C" void MS_ABI impl__OnSelectItem_CMFCRibbonComboBox__UEAAXH_Z(
    void* pThis, int index) {
    (void)pThis;
    (void)index;
}

// The ANSI twin at RVA 0xf6890 (mfc140) clears both lists, empties the
// inherited edit CString, resets selection, and invokes a virtual update.
// This implementation clears the modeled lists/selection but cannot reproduce
// the edit-text clear or virtual update.
// Symbol: ?RemoveAllItems@CMFCRibbonComboBox@@QEAAXXZ
extern "C" void MS_ABI impl__RemoveAllItems_CMFCRibbonComboBox__QEAAXXZ(void* pThis) {
    if (pThis == nullptr) return;
    impl__RemoveAll_CStringList__QEAAXXZ(ItemStrings(pThis));
    ClearItemData(pThis);
    Combo(pThis)->selectedIndex = -1;
}

// The ANSI twin at RVA 0xf6a90 (mfc140) searches item-data nodes and delegates
// a match to the index selector; selection itself requires the missing edit
// control and owner/window state.
// Symbol: ?SelectItem@CMFCRibbonComboBox@@QEAAH_K@Z
extern "C" int MS_ABI impl__SelectItem_CMFCRibbonComboBox__QEAAH_K_Z(
    void* pThis, unsigned __int64 data) {
    (void)pThis;
    (void)data;
    return 0;
}

// The ANSI twin at RVA 0xf68f0 (mfc140) updates the selected index, inherited
// edit CString/control text, and related ribbon elements. Those inherited
// edit/window/ribbon states are not modeled, so this remains a documented stub
// rather than an index-only approximation.
// Symbol: ?SelectItem@CMFCRibbonComboBox@@QEAAHH@Z
extern "C" int MS_ABI impl__SelectItem_CMFCRibbonComboBox__QEAAHH_Z(void* pThis, int index) {
    (void)pThis;
    (void)index;
    return 0;
}

// This exact Unicode overload has no resolved RVA. The ANSI PEBD counterpart
// scans and selects by text, but the same unavailable edit/control selection
// machinery prevents a faithful Unicode implementation.
// Symbol: ?SelectItem@CMFCRibbonComboBox@@QEAAHPEB_W@Z
extern "C" int MS_ABI impl__SelectItem_CMFCRibbonComboBox__QEAAHPEB_W_Z(
    void* pThis, const wchar_t* text) {
    (void)pThis;
    (void)text;
    return 0;
}

// The ANSI twin at RVA 0xf7770 (mfc140) calls the CMFCRibbonEdit accessibility
// handler, writes role 0x2e and conditionally writes state at +0x34, then
// returns TRUE. The base edit thunk exists but is itself a stub in OpenMFC;
// when neither combo state branch writes +0x34, the base handler's effect is
// therefore not reproduced.
// Symbol: ?SetACCData@CMFCRibbonComboBox@@MEAAHPEAVCWnd@@AEAVCAccessibilityData@@@Z
extern "C" int MS_ABI impl__SetACCData_CMFCRibbonComboBox__MEAAHPEAVCWnd__AEAVCAccessibilityData___Z(
    void* pThis, void* pParent, void* pData) {
    impl__SetACCData_CMFCRibbonEdit__UEAAHPEAVCWnd__AEAVCAccessibilityData___Z(
        pThis, pParent, pData);
    auto* data = static_cast<RibbonComboAccessibilityDataLayout*>(pData);
    data->role = 0x2e;
    if (Combo(pThis)->hasEditBox == 0) data->state = 0x40;
    if (Combo(pThis)->baseField_148 != 0) data->state = 1;
    return 1;
}

// The ANSI twin at RVA 0xf6c90 (mfc140) is a single store to
// m_nDropDownHeight at +0x324. That offset is pinned above and agrees with
// the retail public header's field order.
// Symbol: ?SetDropDownHeight@CMFCRibbonComboBox@@QEAAXH@Z
extern "C" void MS_ABI impl__SetDropDownHeight_CMFCRibbonComboBox__QEAAXH_Z(
    void* pThis, int height) {
    Combo(pThis)->dropDownHeight = height;
}
