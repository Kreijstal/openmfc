// CMFCRibbonQuickAccessToolBar — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp

#include "detail/ManualSmallStubImplementationsSupport.h"

#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <limits>

extern "C" void MS_ABI impl__AfxThrowMemoryException__YAXXZ();
extern "C" void MS_ABI impl__AfxThrowInvalidArgException__YAXXZ();

namespace {

// CMFCRibbonQuickAccessToolBar is not declared in include/openmfc. Retail's
// constructor at RVA 0x1226f0 (mfc140u) calls the 0x670-byte
// CMFCRibbonButtonsGroup base constructor, then places its two CArrays and
// command rectangle at the offsets below. CreateObject at RVA 0x1226b0
// (mfc140u) allocates 0x6d0 bytes.
struct RibbonRectShadow {
    std::int32_t left;
    std::int32_t top;
    std::int32_t right;
    std::int32_t bottom;
};

struct RibbonCArrayShadow {
    void* vfptr;
    void* data;
    std::int64_t size;
    std::int64_t capacity;
    std::int64_t growBy;
};

struct RibbonButtonsGroupArrayShadow {
    unsigned char beforeElements[0x180];
    RibbonCArrayShadow elements;
};

struct RibbonQuickAccessToolBarShadow {
    unsigned char baseGroup[0x670];
    RibbonCArrayShadow defaultCommands;
    RibbonCArrayShadow defaultVisible;
    RibbonRectShadow commandsRect;
};

struct RibbonElementRectShadow {
    unsigned char beforeRect[0xc8];
    RibbonRectShadow rect;
};

// MFC's x64 CList layout and node/block shape used by the retail helpers at
// RVAs 0x8350 (RemoveAll), 0x121f8 (NewNode), and 0x12284 (AddTail), mfc140u.
struct RibbonUIntListNode {
    RibbonUIntListNode* next;
    RibbonUIntListNode* prev;
    std::uint32_t value;
    std::uint32_t padding;
};

struct RibbonUIntListBlock {
    RibbonUIntListBlock* next;
};

struct RibbonUIntListShadow {
    void* vfptr;
    RibbonUIntListNode* head;
    RibbonUIntListNode* tail;
    std::int64_t count;
    RibbonUIntListNode* freeList;
    RibbonUIntListBlock* blocks;
    std::int64_t blockSize;
};

static_assert(sizeof(void*) == 8, "ribbon shadow is for the x64 MSVC ABI");
static_assert(sizeof(RibbonRectShadow) == 0x10, "retail RECT size");
static_assert(sizeof(RibbonCArrayShadow) == 0x28, "retail CArray size");
static_assert(offsetof(RibbonCArrayShadow, data) == 0x08, "CArray data offset");
static_assert(offsetof(RibbonCArrayShadow, size) == 0x10, "CArray size offset");
static_assert(offsetof(RibbonButtonsGroupArrayShadow, elements) == 0x180,
              "retail CMFCRibbonButtonsGroup element CArray offset");
static_assert(offsetof(RibbonButtonsGroupArrayShadow, elements) +
                  offsetof(RibbonCArrayShadow, data) == 0x188,
              "retail CMFCRibbonButtonsGroup element data offset");
static_assert(offsetof(RibbonButtonsGroupArrayShadow, elements) +
                  offsetof(RibbonCArrayShadow, size) == 0x190,
              "retail CMFCRibbonButtonsGroup element count offset");
static_assert(offsetof(RibbonQuickAccessToolBarShadow, defaultCommands) == 0x670,
              "retail QAT default command CArray offset");
static_assert(offsetof(RibbonQuickAccessToolBarShadow, defaultVisible) == 0x698,
              "retail QAT default visibility CArray offset");
static_assert(offsetof(RibbonQuickAccessToolBarShadow, commandsRect) == 0x6c0,
              "retail QAT command rectangle offset");
static_assert(sizeof(RibbonQuickAccessToolBarShadow) == 0x6d0,
              "retail CMFCRibbonQuickAccessToolBar size");
static_assert(offsetof(RibbonElementRectShadow, rect) == 0xc8,
              "retail CMFCRibbonBaseElement rectangle offset");
static_assert(sizeof(RibbonUIntListNode) == 0x18, "retail CList node size");
static_assert(offsetof(RibbonUIntListShadow, head) == 0x08, "CList head offset");
static_assert(offsetof(RibbonUIntListShadow, tail) == 0x10, "CList tail offset");
static_assert(offsetof(RibbonUIntListShadow, count) == 0x18, "CList count offset");
static_assert(offsetof(RibbonUIntListShadow, freeList) == 0x20, "CList free-list offset");
static_assert(offsetof(RibbonUIntListShadow, blocks) == 0x28, "CList blocks offset");
static_assert(offsetof(RibbonUIntListShadow, blockSize) == 0x30, "CList block-size offset");
static_assert(sizeof(RibbonUIntListShadow) == 0x38, "retail CList size");

void InitializeRibbonCArray(RibbonCArrayShadow* array) {
    // OpenMFC has no retail CArray vtable to install; the implemented thunks
    // access only these storage fields and never dispatch through this pointer.
    array->vfptr = nullptr;
    array->data = nullptr;
    array->size = 0;
    array->capacity = 0;
    array->growBy = 0;
}

void ClearRibbonUIntList(RibbonUIntListShadow* list) {
    RibbonUIntListBlock* block = list->blocks;
    list->head = nullptr;
    list->tail = nullptr;
    list->count = 0;
    list->freeList = nullptr;
    list->blocks = nullptr;
    while (block != nullptr) {
        RibbonUIntListBlock* next = block->next;
        std::free(block);
        block = next;
    }
}

void AppendRibbonUIntList(RibbonUIntListShadow* list, std::uint32_t value) {
    if (list->freeList == nullptr) {
        if (list->blockSize == 0) {
            impl__AfxThrowInvalidArgException__YAXXZ();
            return;
        }
        if (list->blockSize < 0 ||
            static_cast<std::uint64_t>(list->blockSize) >
                (std::numeric_limits<std::size_t>::max() - sizeof(RibbonUIntListBlock)) /
                    sizeof(RibbonUIntListNode)) {
            impl__AfxThrowMemoryException__YAXXZ();
            return;
        }
        const std::size_t nodes = static_cast<std::size_t>(list->blockSize);
        auto* block = static_cast<RibbonUIntListBlock*>(
            std::malloc(sizeof(RibbonUIntListBlock) + nodes * sizeof(RibbonUIntListNode)));
        if (block == nullptr) {
            impl__AfxThrowMemoryException__YAXXZ();
            return;
        }
        block->next = list->blocks;
        list->blocks = block;
        auto* first = reinterpret_cast<RibbonUIntListNode*>(block + 1);
        for (std::size_t i = 0; i < nodes; ++i) {
            first[i].next = (i + 1 < nodes) ? &first[i + 1] : nullptr;
        }
        list->freeList = first;
    }

    RibbonUIntListNode* node = list->freeList;
    list->freeList = node->next;
    node->prev = list->tail;
    node->next = nullptr;
    ++list->count;
    node->value = value;
    if (list->tail != nullptr) {
        list->tail->next = node;
    } else {
        list->head = node;
    }
    list->tail = node;
}

} // namespace

extern "C" int MS_ABI impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(
    const CObject* pThis, const CRuntimeClass* pClass);
extern "C" CRuntimeClass* MS_ABI
impl__GetThisClass_CMFCRibbonQuickAccessCustomizeButton__SAPEAUCRuntimeClass__XZ();
extern "C" void MS_ABI impl___1CMFCRibbonButtonsGroup__UEAA_XZ(void* pThis);

// Symbol: ??0CMFCRibbonQuickAccessToolBar@@QEAA@XZ
// OpenMFC has no retail QAT vtable/base implementation. Initialize the
// retail-layout arrays and rectangles used by this file so its destructor and
// simple accessors never consume uninitialized storage; this is not a complete
// QAT construction.
extern "C" void* MS_ABI impl___0CMFCRibbonQuickAccessToolBar__QEAA_XZ(void* pThis) {
    if (pThis == nullptr) {
        return nullptr;
    }
    auto* bytes = static_cast<unsigned char*>(pThis);
    auto* group = static_cast<RibbonButtonsGroupArrayShadow*>(pThis);
    auto* qat = static_cast<RibbonQuickAccessToolBarShadow*>(pThis);
    InitializeRibbonCArray(&group->elements);
    InitializeRibbonCArray(&qat->defaultCommands);
    InitializeRibbonCArray(&qat->defaultVisible);
    *reinterpret_cast<void**>(bytes) = nullptr;
    *reinterpret_cast<void**>(bytes + 0xd8) = nullptr;
    *reinterpret_cast<RibbonRectShadow*>(bytes + 0xc8) = {};
    qat->commandsRect = {};
    return pThis;
}

// Symbol: ??1CMFCRibbonQuickAccessToolBar@@UEAA@XZ
// Retail RVA 0x1227e0 (mfc140u) rewrites the vptr to the group vtable, destroys
// the visibility and command CArrays, then runs the CMFCRibbonButtonsGroup base
// destructor. The CArray destructor helpers at RVAs 0x14f60 and 0xf96d0
// (mfc140u) release their data through the CRT free import (slot 0x2c74e8,
// resolved with iatu.py). The matching array pointers are initialized by the
// partial constructor above. OpenMFC has no retail-compatible QAT vtable to
// install, so that transient rewrite is omitted.
extern "C" void MS_ABI impl___1CMFCRibbonQuickAccessToolBar__UEAA_XZ(void* pThis) {
    auto* qat = static_cast<RibbonQuickAccessToolBarShadow*>(pThis);
    std::free(qat->defaultVisible.data);
    std::free(qat->defaultCommands.data);
    impl___1CMFCRibbonButtonsGroup__UEAA_XZ(pThis);
}

// Retail RVA 0x123640 (mfc140u) returns E_INVALIDARG for a non-VT_I4 child
// selector and S_FALSE for child ID 0. Otherwise it enumerates accessible
// children through vtable slot 0x3f0, checks the 1-based index, and dispatches
// the selected child's default action; the retail-compatible full vtable is absent.
// Symbol: ?accDoDefaultAction@CMFCRibbonQuickAccessToolBar@@MEAAJUtagVARIANT@@@Z
extern "C" long MS_ABI impl__accDoDefaultAction_CMFCRibbonQuickAccessToolBar__MEAAJUtagVARIANT___Z(
    void* /*pThis*/, const void* /*varChild*/) {
    return 0;
}

// Retail RVA 0x1234c0 (mfc140u) returns E_INVALIDARG for a null output VARIANT;
// without a ribbon accessibility context it returns S_FALSE. Otherwise it
// initializes VT_I4, tests child rectangles, and dispatches hits through child
// vtables, which are not represented by the local QAT/group layout.
// Symbol: ?accHitTest@CMFCRibbonQuickAccessToolBar@@MEAAJJJPEAUtagVARIANT@@@Z
extern "C" long MS_ABI impl__accHitTest_CMFCRibbonQuickAccessToolBar__MEAAJJJPEAUtagVARIANT___Z(
    void* /*pThis*/, long /*xLeft*/, long /*yTop*/, void* /*pVarChild*/) {
    return 0;
}

// Retail RVA 0x1237c0 (mfc140u) validates all four output pointers and a VT_I4
// selector (otherwise E_INVALIDARG); child ID 0 uses the QAT rectangle when a
// ribbon accessibility context exists, while positive IDs dispatch to child
// location. Other/no-context cases return S_FALSE. The needed child dispatch
// and accessibility context are not represented locally.
// Symbol: ?accLocation@CMFCRibbonQuickAccessToolBar@@MEAAJPEAJ000UtagVARIANT@@@Z
extern "C" long MS_ABI impl__accLocation_CMFCRibbonQuickAccessToolBar__MEAAJPEAJ000UtagVARIANT___Z(
    void* /*pThis*/, long* /*xLeft*/, long* /*yTop*/, long* /*cxWidth*/,
    long* /*cyHeight*/, const void* /*varChild*/) {
    return 0;
}

// Retail RVA 0x1238d0 (mfc140u) initializes the output VARIANT, requires a
// VT_I4 start child, then handles sibling navigation and parent/child-object
// navigation using the accessible-child array and ribbon parent. That collection
// and COM accessibility state are not modeled here.
// Symbol: ?accNavigate@CMFCRibbonQuickAccessToolBar@@MEAAJJUtagVARIANT@@PEAU2@@Z
extern "C" long MS_ABI impl__accNavigate_CMFCRibbonQuickAccessToolBar__MEAAJJUtagVARIANT__PEAU2__Z(
    void* /*pThis*/, long /*navDir*/, const void* /*varStart*/, void* /*pVarEnd*/) {
    return 0;
}

// Retail RVA 0x122fe0 (mfc140u) clones/configures the element, updates its
// parent/original links, appends it to the base group's CArray, then tail-jumps
// to an internal post-add routine at RVA 0x1231c0 (mfc140u); the local group
// layout and virtual dispatch are placeholders.
// Symbol: ?Add@CMFCRibbonQuickAccessToolBar@@IEAAXPEAVCMFCRibbonBaseElement@@@Z
extern "C" void MS_ABI impl__Add_CMFCRibbonQuickAccessToolBar__IEAAXPEAVCMFCRibbonBaseElement___Z(
    void* /*pThis*/, void* /*pElement*/) {}

// Retail RVA 0x1226b0 (mfc140u) allocates 0x6d0 bytes and, on success, invokes
// the QAT constructor. OpenMFC has no retail-compatible QAT class/vtable to
// construct into, so this factory remains a stub.
// Symbol: ?CreateObject@CMFCRibbonQuickAccessToolBar@@SAPEAVCObject@@XZ
extern "C" void* MS_ABI impl__CreateObject_CMFCRibbonQuickAccessToolBar__SAPEAVCObject__XZ() {
    return nullptr;
}

// Retail RVA 0x123730 (mfc140u) returns E_INVALIDARG for a null output pointer;
// otherwise it calls the group's accessible-child enumerator at vtable slot
// 0x3f0 and copies the resulting count. A retail-compatible QAT/group vtable
// is not available locally.
// Symbol: ?get_accChildCount@CMFCRibbonQuickAccessToolBar@@MEAAJPEAJ@Z
extern "C" long MS_ABI impl__get_accChildCount_CMFCRibbonQuickAccessToolBar__MEAAJPEAJ_Z(
    void* /*pThis*/, long* /*pcountChildren*/) {
    return 0;
}

// disas.py --u reports no RVA for this export; without its retail body or a
// local QAT accessibility-parent mapping, do not infer the returned IDispatch.
// Symbol: ?get_accParent@CMFCRibbonQuickAccessToolBar@@MEAAJPEAPEAUIDispatch@@@Z
extern "C" long MS_ABI impl__get_accParent_CMFCRibbonQuickAccessToolBar__MEAAJPEAPEAUIDispatch___Z(
    void* /*pThis*/, void** /*ppdispParent*/) {
    return 0;
}

// Retail RVA 0x122b90 (mfc140u) returns zero for an empty element array;
// otherwise it returns the final element's right edge minus the QAT's left
// edge. A negative/out-of-range final index takes AfxThrowInvalidArgException.
// The CArray and CRect offsets below are pinned from the retail body and the
// constructor at RVA 0x1226f0 (mfc140u).
// Symbol: ?GetActualWidth@CMFCRibbonQuickAccessToolBar@@IEBAHXZ
extern "C" int MS_ABI impl__GetActualWidth_CMFCRibbonQuickAccessToolBar__IEBAHXZ(
    const void* pThis) {
    const auto* group = static_cast<const RibbonButtonsGroupArrayShadow*>(pThis);
    if (group->elements.size == 0) {
        return 0;
    }
    if (group->elements.size < 0) {
        impl__AfxThrowInvalidArgException__YAXXZ();
        return 0;
    }
    const std::int64_t lastIndex = group->elements.size - 1;
    if (lastIndex >= group->elements.size) {
        impl__AfxThrowInvalidArgException__YAXXZ();
        return 0;
    }
    const auto* elements = static_cast<void* const*>(group->elements.data);
    const auto* last = static_cast<const RibbonElementRectShadow*>(
        elements[lastIndex]);
    const auto* selfRect = static_cast<const RibbonElementRectShadow*>(pThis);
    return last->rect.right - selfRect->rect.left;
}

// Retail RVA 0x122b00 (mfc140u) clears the output CList, then appends each
// nonzero element ID and the ID of a zero-ID quick-access customize button.
// The associated CList helper entries (0x8350, 0x121f8, 0x12284) and
// IsKindOf at RVA 0x234cf0 (mfc140u) were disassembled; mirror offsets are
// pinned above.
// Symbol: ?GetCommands@CMFCRibbonQuickAccessToolBar@@IEAAXAEAV?$CList@II@@@Z
extern "C" void MS_ABI impl__GetCommands_CMFCRibbonQuickAccessToolBar__IEAAXAEAV__CList_II___Z(
    void* pThis, void* pList) {
    auto* group = static_cast<RibbonButtonsGroupArrayShadow*>(pThis);
    auto* list = static_cast<RibbonUIntListShadow*>(pList);
    ClearRibbonUIntList(list);
    auto* const customizeClass = impl__GetThisClass_CMFCRibbonQuickAccessCustomizeButton__SAPEAUCRuntimeClass__XZ();
    auto* const elements = static_cast<void**>(group->elements.data);
    for (std::int64_t i = 0; i < group->elements.size; ++i) {
        void* element = elements[i];
        const auto id = *reinterpret_cast<const std::uint32_t*>(
            static_cast<const unsigned char*>(element) + 0x118);
        if (id != 0 || impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(
                           static_cast<const CObject*>(element), customizeClass)) {
            AppendRibbonUIntList(list, id);
        }
    }
}

// Retail RVA 0x123130 (mfc140u) walks the visible-state CArray at +0x698 and
// appends its corresponding command IDs from the CArray at +0x670. The output
// CList operations use the verified retail helper layouts pinned above.
// Symbol: ?GetDefaultCommands@CMFCRibbonQuickAccessToolBar@@IEAAXAEAV?$CList@II@@@Z
extern "C" void MS_ABI impl__GetDefaultCommands_CMFCRibbonQuickAccessToolBar__IEAAXAEAV__CList_II___Z(
    void* pThis, void* pList) {
    auto* qat = static_cast<RibbonQuickAccessToolBarShadow*>(pThis);
    auto* list = static_cast<RibbonUIntListShadow*>(pList);
    ClearRibbonUIntList(list);
    const auto* commands = static_cast<const std::uint32_t*>(qat->defaultCommands.data);
    const auto* visible = static_cast<const std::uint32_t*>(qat->defaultVisible.data);
    const std::int64_t count = qat->defaultCommands.size;
    for (std::int64_t i = 0; i < count; ++i) {
        if (i >= qat->defaultVisible.size) {
            impl__AfxThrowInvalidArgException__YAXXZ();
            return;
        }
        if (visible[i] != 0) {
            AppendRibbonUIntList(list, commands[i]);
        }
    }
}

// Retail RVA 0x122d60 (mfc140u) calls base sizing and consults ribbon-bar and
// global sizing state before returning CSize. This safe placeholder zeroes the
// hidden result and preserves its ABI return pointer; it is not the retail size.
// Symbol: ?GetRegularSize@CMFCRibbonQuickAccessToolBar@@MEAA?AVCSize@@PEAVCDC@@@Z
extern "C" void* MS_ABI impl__GetRegularSize_CMFCRibbonQuickAccessToolBar__MEAA_AVCSize__PEAVCDC___Z(
    void* /*pThis*/, void* pResult, void* /*pDC*/) {
    if (pResult != nullptr) {
        *static_cast<std::int64_t*>(pResult) = 0;
    }
    return pResult;
}

// Retail RVA 0x122be0 (mfc140u) copies the base rectangle to the QAT command
// rectangle, iterates the element array, updates child rectangles, and calls a
// child virtual slot. Retail-compatible group/child dispatch is not modeled.
// Symbol: ?OnAfterChangeRect@CMFCRibbonQuickAccessToolBar@@MEAAXPEAVCDC@@@Z
extern "C" void MS_ABI impl__OnAfterChangeRect_CMFCRibbonQuickAccessToolBar__MEAAXPEAVCDC___Z(
    void* /*pThis*/, void* /*pDC*/) {}

// The ANSI twin at RVA 0x1240f0 (mfc140) is available although the Unicode
// RVA map has no entry. With a ribbon parent/accessibility context it builds a
// child array through vtable slot 0x3f0 and dispatches to the selected child;
// without that context it returns FALSE. The required state/vtable is absent.
// Symbol: ?OnSetAccData@CMFCRibbonQuickAccessToolBar@@MEAAHJ@Z
extern "C" int MS_ABI impl__OnSetAccData_CMFCRibbonQuickAccessToolBar__MEAAHJ_Z(
    void* /*pThis*/, long /*child*/) {
    return 0;
}

// The ANSI twin at RVA 0x123b90 (mfc140) iterates the group elements, rebuilds
// the customize button's hidden-item array at +0x270, and consults QAT state at
// +0xf8. The child array and QAT fields used by that algorithm are not fully
// modeled here, so this remains a documented stub.
// Symbol: ?RebuildHiddenItems@CMFCRibbonQuickAccessToolBar@@IEAAXXZ
extern "C" void MS_ABI impl__RebuildHiddenItems_CMFCRibbonQuickAccessToolBar__IEAAXXZ(
    void* /*pThis*/) {}

// The ANSI twin at RVA 0x123f30 (mfc140) rebuilds each element's key tip using
// the global keyboard manager and child virtual dispatch. That key-tip manager
// state and a retail-compatible QAT/element vtable are unavailable locally.
// Symbol: ?RebuildKeys@CMFCRibbonQuickAccessToolBar@@IEAAXXZ
extern "C" void MS_ABI impl__RebuildKeys_CMFCRibbonQuickAccessToolBar__IEAAXXZ(
    void* /*pThis*/) {}

// The ANSI twin at RVA 0x123e00 (mfc140) searches the group array by element
// ID, then invokes a child removal path and rebuilds hidden items and key tips.
// The child-removal dispatch and dependent rebuild infrastructure are missing.
// Symbol: ?Remove@CMFCRibbonQuickAccessToolBar@@IEAAXPEAVCMFCRibbonBaseElement@@@Z
extern "C" void MS_ABI impl__Remove_CMFCRibbonQuickAccessToolBar__IEAAXPEAVCMFCRibbonBaseElement___Z(
    void* /*pThis*/, void* /*pElement*/) {}

// The ANSI twin at RVA 0x123cb0 (mfc140) tail-jumps to SetCommands at
// RVA 0x123610 (mfc140), passing the parent ribbon bar and a null customize
// button. The downstream element cloning/configuration path is not represented.
// Symbol: ?ReplaceCommands@CMFCRibbonQuickAccessToolBar@@IEAAXAEBV?$CList@II@@@Z
extern "C" void MS_ABI impl__ReplaceCommands_CMFCRibbonQuickAccessToolBar__IEAAXAEBV__CList_II___Z(
    void* /*pThis*/, const void* /*commands*/) {}

// The ANSI twin at RVA 0x123cd0 (mfc140) builds a temporary command list from
// GetDefaultCommands, then calls SetCommands with the parent ribbon bar. The
// reset order is established, but the downstream SetCommands path is not.
// Symbol: ?ResetCommands@CMFCRibbonQuickAccessToolBar@@IEAAXXZ
extern "C" void MS_ABI impl__ResetCommands_CMFCRibbonQuickAccessToolBar__IEAAXXZ(
    void* /*pThis*/) {}

// The ANSI twin at RVA 0x124070 (mfc140) first calls base SetACCData at
// RVA 0x11240 (mfc140), then uses ribbon-parent and child accessibility state.
// CAccessibilityData itself is modeled, but the required QAT child context and
// compatible virtual dispatch are not.
// Symbol: ?SetACCData@CMFCRibbonQuickAccessToolBar@@MEAAHPEAVCWnd@@AEAVCAccessibilityData@@@Z
extern "C" int MS_ABI impl__SetACCData_CMFCRibbonQuickAccessToolBar__MEAAHPEAVCWnd__AEAVCAccessibilityData___Z(
    void* /*pThis*/, void* /*pParent*/, void* /*pData*/) {
    return 0;
}

// The ANSI twin at RVA 0x123610 (mfc140) is available; it walks command IDs,
// looks up/clones elements from the ribbon bar, and updates the QAT group and
// customize button. Those construction and child-vtable dependencies are not
// available in this file's placeholder layout.
// Symbol: ?SetCommands@CMFCRibbonQuickAccessToolBar@@IEAAXPEAVCMFCRibbonBar@@AEBV?$CList@II@@PEAVCMFCRibbonQuickAccessCustomizeButton@@@Z
extern "C" void MS_ABI impl__SetCommands_CMFCRibbonQuickAccessToolBar__IEAAXPEAVCMFCRibbonBar__AEBV__CList_II__PEAVCMFCRibbonQuickAccessCustomizeButton___Z(
    void* /*pThis*/, void* /*pRibbonBar*/, const void* /*commands*/,
    void* /*pCustomizeButton*/) {}

// disas.py --u reports no RVA for this overload; it requires ribbon-bar
// command lookup plus tooltip/element construction in the unavailable QAT layout.
// Symbol: ?SetCommands@CMFCRibbonQuickAccessToolBar@@IEAAXPEAVCMFCRibbonBar@@AEBV?$CList@II@@PEB_W@Z
extern "C" void MS_ABI impl__SetCommands_CMFCRibbonQuickAccessToolBar__IEAAXPEAVCMFCRibbonBar__AEBV__CList_II__PEB_W_Z(
    void* /*pThis*/, void* /*pRibbonBar*/, const void* /*commands*/,
    const wchar_t* /*toolTip*/) {}
