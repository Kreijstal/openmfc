// CMFCRibbonTabsGroup — OpenMFC implementation.
// This type has no OpenMFC class declaration; retail field offsets used here
// are represented by the local shadow views below.

#include "detail/ManualSmallStubImplementationsSupport.h"

#include <cstddef>
#include <cstdlib>
#include <cstring>
extern "C" void MS_ABI impl__AfxThrowMemoryException__YAXXZ();
extern "C" void* MS_ABI impl__GetAccessibleDispatch_CMFCRibbonBar__QEAAPEAUIDispatch__XZ(
    CMFCRibbonBar* pThis);

// CMFCRibbonButtonsGroup's inherited CArray is a CObject-derived MFC array:
// vfptr, data, size, capacity, grow-by.  The array occupies +0x180 in the
// retail group; the 0x670 allocation size is visible in CreateObject.
struct S_RibbonArrayView {
    void* vfptr;
    void* data;
    long long size;
    long long capacity;
    long long growBy;
};
static_assert(sizeof(S_RibbonArrayView) == 0x28, "retail CArray size");
static_assert(offsetof(S_RibbonArrayView, data) == 0x08, "retail CArray data @+8");
static_assert(offsetof(S_RibbonArrayView, size) == 0x10, "retail CArray size @+0x10");

struct S_CMFCRibbonTabsGroupLayout {
    unsigned char baseThroughParent[0xd8];
    void* m_pParentRibbonBar;                  // +0xd8
    unsigned char baseTail[0xa0];              // through +0x17f
    S_RibbonArrayView m_arButtons;             // +0x180
    unsigned char toolbarImagesAndTail[0x4c8];// total retail size 0x670
};
static_assert(offsetof(S_CMFCRibbonTabsGroupLayout, m_pParentRibbonBar) == 0xd8,
              "CMFCRibbonBaseElement::m_pRibbonBar @+0xd8");
static_assert(offsetof(S_CMFCRibbonTabsGroupLayout, m_arButtons) == 0x180,
              "CMFCRibbonButtonsGroup::m_arButtons @+0x180");
static_assert(offsetof(S_CMFCRibbonTabsGroupLayout, m_arButtons) +
                  offsetof(S_RibbonArrayView, data) == 0x188,
              "m_arButtons data @+0x188");
static_assert(offsetof(S_CMFCRibbonTabsGroupLayout, m_arButtons) +
                  offsetof(S_RibbonArrayView, size) == 0x190,
              "m_arButtons size @+0x190");
static_assert(sizeof(S_CMFCRibbonTabsGroupLayout) == 0x670,
              "CMFCRibbonTabsGroup retail allocation size");

struct S_RibbonCategoryView {
    unsigned char beforeVisible[0xa8];
    int visible;
    unsigned char beforeTab[0xd8 - 0xac];
    unsigned char embeddedTab[0x180];
};
static_assert(offsetof(S_RibbonCategoryView, visible) == 0xa8,
              "CMFCRibbonCategory::m_bIsVisible @+0xa8");
static_assert(offsetof(S_RibbonCategoryView, embeddedTab) == 0xd8,
              "CMFCRibbonCategory::m_Tab @+0xd8");
static_assert(offsetof(S_RibbonCategoryView, embeddedTab) + 0xc8 == 0x1a0,
              "embedded tab rect @category+0x1a0");
static_assert(sizeof(CAccessibilityData) == 0x58, "retail CAccessibilityData size");
static_assert(offsetof(CAccessibilityData, m_strAccName) == 0x00, "acc name @+0x00");
static_assert(offsetof(CAccessibilityData, m_strAccValue) == 0x08, "acc value @+0x08");
static_assert(offsetof(CAccessibilityData, m_strDescription) == 0x10, "description @+0x10");
static_assert(offsetof(CAccessibilityData, m_strAccKeys) == 0x18, "acc keys @+0x18");
static_assert(offsetof(CAccessibilityData, m_strAccHelp) == 0x20, "acc help @+0x20");
static_assert(offsetof(CAccessibilityData, m_strAccDefAction) == 0x28,
              "CAccessibilityData::m_strAccDefAction @+0x28");
static_assert(offsetof(CAccessibilityData, m_nAccRole) == 0x30, "acc role @+0x30");
static_assert(offsetof(CAccessibilityData, m_bAccState) == 0x34, "acc state @+0x34");
static_assert(offsetof(CAccessibilityData, m_nAccHit) == 0x38, "acc hit @+0x38");
static_assert(offsetof(CAccessibilityData, m_rectAccLocation) == 0x3c, "acc rect @+0x3c");
static_assert(offsetof(CAccessibilityData, m_ptAccHit) == 0x4c, "acc point @+0x4c");

namespace {

// CArray::SetSize for pointer arrays, using the retail CArray field offsets
// above.  Retail frees a zero-sized array and grows a non-empty array in
// pointer-sized slots; allocation failures leave the prior allocation intact.
bool SetPointerArraySize(S_RibbonArrayView* array, long long newSize) {
    if (newSize < 0) return false;

    if (newSize == 0) {
        std::free(array->data);
        array->data = nullptr;
        array->size = 0;
        array->capacity = 0;
        return true;
    }

    if (array->data == nullptr) {
        const long long newCapacity = newSize > array->growBy ? newSize : array->growBy;
        void* data = std::calloc(static_cast<size_t>(newCapacity), sizeof(void*));
        if (data == nullptr) {
            impl__AfxThrowMemoryException__YAXXZ();
            return false;
        }
        array->data = data;
        array->size = newSize;
        array->capacity = newCapacity;
        return true;
    }

    if (newSize <= array->capacity) {
        if (newSize > array->size) {
            std::memset(static_cast<unsigned char*>(array->data) +
                            array->size * static_cast<long long>(sizeof(void*)),
                        0, static_cast<size_t>((newSize - array->size) *
                                               static_cast<long long>(sizeof(void*))));
        }
        array->size = newSize;
        return true;
    }

    long long grow = array->growBy;
    if (grow == 0) {
        grow = (array->size + 7) / 8;
        if (grow < 4) grow = 4;
        if (grow > 1024) grow = 1024;
    }
    const long long newCapacity =
        newSize < array->capacity + grow ? array->capacity + grow : newSize;
    void* data = std::realloc(array->data,
                              static_cast<size_t>(newCapacity) * sizeof(void*));
    if (data == nullptr) {
        impl__AfxThrowMemoryException__YAXXZ();
        return false;
    }
    std::memset(static_cast<unsigned char*>(data) +
                    array->size * static_cast<long long>(sizeof(void*)),
                0, static_cast<size_t>((newSize - array->size) *
                                       static_cast<long long>(sizeof(void*))));
    array->data = data;
    array->size = newSize;
    array->capacity = newCapacity;
    return true;
}

bool AppendPointer(S_RibbonArrayView* array, void* value) {
    const long long index = array->size;
    if (!SetPointerArraySize(array, index + 1)) return false;
    static_cast<void**>(array->data)[index] = value;
    return true;
}

} // namespace

// Symbol: ??0CMFCRibbonTabsGroup@@QEAA@PEAVCMFCRibbonBaseElement@@@Z
extern "C" void* MS_ABI impl___0CMFCRibbonTabsGroup__QEAA_PEAVCMFCRibbonBaseElement___Z(
    void* pThis, void* pBaseElement) {
    (void)pBaseElement;
    return pThis;
}
// Symbol: ??0CMFCRibbonTabsGroup@@QEAA@XZ
extern "C" void* MS_ABI impl___0CMFCRibbonTabsGroup__QEAA_XZ(void* pThis) {
    return pThis;
}

// Retail mfc140u RVA 0xeba20 allocates 0x670 bytes and invokes the no-argument
// constructor, but OpenMFC has no correctly-sized CMFCRibbonButtonsGroup base or local ribbon
// vtable with which to construct that object. Keep dynamic creation explicit.
// Symbol: ?CreateObject@CMFCRibbonTabsGroup@@SAPEAVCObject@@XZ
extern "C" CObject* MS_ABI impl__CreateObject_CMFCRibbonTabsGroup__SAPEAVCObject__XZ() {
    return nullptr;
}

// Retail mfc140u RVA 0xebb20 clears the inherited button array, adjusts the
// vptr to the base, then runs CMFCRibbonButtonsGroup's destructor (which also
// destroys image/list members not modeled by the OpenMFC placeholder). Release
// the button-array allocation owned by UpdateTabs, but do not invoke that
// incompatible base destructor.
// Symbol: ??1CMFCRibbonTabsGroup@@UEAA@XZ
extern "C" void MS_ABI impl___1CMFCRibbonTabsGroup__UEAA_XZ(void* pThis) {
    if (pThis == nullptr) return;
    auto* layout = reinterpret_cast<S_CMFCRibbonTabsGroupLayout*>(pThis);
    SetPointerArraySize(&layout->m_arButtons, 0);
}

// The ANSI twin body at RVA 0xec830 (mfc140.dll) resets the embedded
// CAccessibilityData and, when the parent bar/HWND and 1-based child index
// are valid, directly dereferences the selected tab and calls its vtable slot
// 0x168 (slot 45) without a null check. OpenMFC's sibling
// CMFCRibbonTab::SetACCData definition is still a p0/p1 placeholder and its
// local C++ vtable does not provide that retail slot; additionally, the +0x40
// CString block is not constructed in OpenMFC's zero-filled opaque m_Tabs.
// This cannot safely reset or dispatch the retail accessibility data. The
// Unicode export has no resolved RVA.
// Symbol: ?OnSetAccData@CMFCRibbonTabsGroup@@MEAAHJ@Z
extern "C" int MS_ABI impl__OnSetAccData_CMFCRibbonTabsGroup__MEAAHJ_Z(
    void* /*pThis*/, long /*lVal*/) {
    return 0;
}

// The ANSI twin body at RVA 0xec750 (mfc140.dll) ignores its two explicit
// arguments and writes the embedded CAccessibilityData at +0x40: it sets the
// name to "Ribbon Tabs" and role 0x14, copies the element rect at +0xc8, and
// maps it through the parent ribbon bar when that bar has an HWND. OpenMFC's
// CMFCRibbonBaseElement does not construct that embedded data block: the local
// ribbon-bar constructor zeroes the opaque m_Tabs storage, so its CString
// members are not valid objects for Empty()/assignment. Keep this explicit
// stub until the native base layout/constructor is modeled.
// Symbol: ?SetACCData@CMFCRibbonTabsGroup@@MEAAHPEAVCWnd@@AEAVCAccessibilityData@@@Z
extern "C" int MS_ABI impl__SetACCData_CMFCRibbonTabsGroup__MEAAHPEAVCWnd__AEAVCAccessibilityData___Z(
    void* /*pThis*/, CWnd* /*pParent*/, CAccessibilityData& /*accData*/) {
    return 0;
}

// The ANSI twin body at RVA 0xecca0 (mfc140.dll) clears m_arButtons, then
// appends each non-null, visible category's embedded tab (+0xd8) when that
// tab's rect (category+0x1a0) is non-empty. The Unicode export has no resolved
// RVA; the retail bodies are byte-identical between the twins. The local
// SetSize helper follows the native pointer-array growth policy and throws the
// MFC memory exception if allocation fails. IsRectEmpty (IAT 0x1802c52c8) was
// resolved with iat.py.
// Symbol: ?UpdateTabs@CMFCRibbonTabsGroup@@QEAAXAEAV?$CArray@PEAVCMFCRibbonCategory@@PEAV1@@@@Z
extern "C" void MS_ABI impl__UpdateTabs_CMFCRibbonTabsGroup__QEAAXAEAV__CArray_PEAVCMFCRibbonCategory__PEAV1____Z(
    void* pThis, CArray<CMFCRibbonCategory*, CMFCRibbonCategory*>& categoriesArg) {
    auto* layout = reinterpret_cast<S_CMFCRibbonTabsGroupLayout*>(pThis);
    auto* categories = reinterpret_cast<const S_RibbonArrayView*>(&categoriesArg);
    SetPointerArraySize(&layout->m_arButtons, 0);

    auto** categoryData = static_cast<void**>(categories->data);
    for (long long i = 0; i < categories->size; ++i) {
        void* category = categoryData[i];
        if (category == nullptr) continue;
        auto* categoryBytes = static_cast<unsigned char*>(category);
        if (*reinterpret_cast<int*>(categoryBytes + 0xa8) == 0) continue;
        auto* tabRect = reinterpret_cast<RECT*>(categoryBytes + 0x1a0);
        if (::IsRectEmpty(tabRect)) continue;
        if (!AppendPointer(&layout->m_arButtons, categoryBytes + 0xd8)) return;
    }
}

// Retail mfc140u RVA 0xebea0 validates VT_I4 and the 1-based child index,
// returns S_FALSE for child 0 or a null child, and otherwise invokes a child
// virtual at vtable byte offset 0x2c8 (slot 89). Reproduce the validation and
// null-child results; a non-null child's action cannot be dispatched through
// OpenMFC's shorter placeholder vtable.
// Symbol: ?accDoDefaultAction@CMFCRibbonTabsGroup@@MEAAJUtagVARIANT@@@Z
extern "C" long MS_ABI impl__accDoDefaultAction_CMFCRibbonTabsGroup__MEAAJUtagVARIANT___Z(
    void* pThis, VARIANT varChild) {
    if (varChild.vt != VT_I4) return E_INVALIDARG;
    if (varChild.lVal == 0) return S_FALSE;
    if (varChild.lVal < 0) return E_INVALIDARG;
    const long index = varChild.lVal - 1;

    auto* layout = reinterpret_cast<S_CMFCRibbonTabsGroupLayout*>(pThis);
    if (index >= layout->m_arButtons.size) return E_INVALIDARG;
    auto** buttons = static_cast<void**>(layout->m_arButtons.data);
    if (buttons[index] == nullptr) return S_FALSE;
    return E_NOTIMPL;
}

// Retail mfc140u RVA 0xebd10 converts the point with ScreenToClient, tests
// each tab rect with PtInRect, and on a hit calls the tab's SetACCData virtual
// at vtable byte offset 0x168 (slot 45). The sibling thunk is still a
// p0/p1 placeholder and the local tab vtable lacks the retail slot, so this
// callback-dependent operation remains unsupported. ScreenToClient (IAT
// 0x1802c7318) and PtInRect (IAT 0x1802c72f8) were resolved with iatu.py.
// Symbol: ?accHitTest@CMFCRibbonTabsGroup@@MEAAJJJPEAUtagVARIANT@@@Z
extern "C" long MS_ABI impl__accHitTest_CMFCRibbonTabsGroup__MEAAJJJPEAUtagVARIANT___Z(
    void* /*pThis*/, long /*x*/, long /*y*/, VARIANT* /*varChild*/) {
    return E_NOTIMPL;
}

// Retail mfc140u RVA 0xebf30 navigates among child tabs and ribbon-bar
// keyboard/context targets using state at parent-bar offsets +0x620, +0x628,
// +0xb08, +0x14dc, and +0x480. That state and its navigation objects are not
// modeled, so this remains an explicit unsupported stub.
// Symbol: ?accNavigate@CMFCRibbonTabsGroup@@MEAAJJUtagVARIANT@@PEAU2@@Z
extern "C" long MS_ABI impl__accNavigate_CMFCRibbonTabsGroup__MEAAJJUtagVARIANT__PEAU2__Z(
    void* /*pThis*/, long /*navDir*/, VARIANT /*varStart*/, VARIANT* /*varEnd*/) {
    return E_NOTIMPL;
}

// Retail mfc140u RVA 0xebf10 returns E_INVALIDARG for a null output pointer;
// otherwise it writes the inherited m_arButtons count and returns S_OK.
// Symbol: ?get_accChildCount@CMFCRibbonTabsGroup@@MEAAJPEAJ@Z
extern "C" long MS_ABI impl__get_accChildCount_CMFCRibbonTabsGroup__MEAAJPEAJ_Z(
    void* pThis, long* childCount) {
    if (childCount == nullptr) return E_INVALIDARG;
    *childCount = static_cast<long>(reinterpret_cast<S_CMFCRibbonTabsGroupLayout*>(pThis)
                                        ->m_arButtons.size);
    return S_OK;
}

// Retail mfc140u RVA 0xebe40 refreshes this object's accessibility data and
// returns a BSTR from m_strAccDefAction. That refresh requires the unmodeled
// CMFCRibbonTab::SetACCData virtual callback described above.
// Symbol: ?get_accDefaultAction@CMFCRibbonTabsGroup@@MEAAJUtagVARIANT@@PEAPEA_W@Z
extern "C" long MS_ABI impl__get_accDefaultAction_CMFCRibbonTabsGroup__MEAAJUtagVARIANT__PEAPEA_W_Z(
    void* /*pThis*/, VARIANT /*varChild*/, wchar_t** /*defaultAction*/) {
    return E_NOTIMPL;
}

// The ANSI twin body at RVA 0xd9090 (mfc140.dll) validates and clears the
// output, then calls CMFCRibbonBar::GetAccessibleDispatch when the parent bar
// at +0xd8 has an HWND at +0x40; it returns S_OK on that path even if the helper
// returns null, and S_FALSE when the parent or HWND is absent. The helper thunk
// exists, but OpenMFC's implementation returns null because its CWnd accessibility
// dispatch member is not modeled. The Unicode export has no resolved RVA.
// Symbol: ?get_accParent@CMFCRibbonTabsGroup@@MEAAJPEAPEAUIDispatch@@@Z
extern "C" long MS_ABI impl__get_accParent_CMFCRibbonTabsGroup__MEAAJPEAPEAUIDispatch___Z(
    void* pThis, IDispatch** parent) {
    if (parent == nullptr) return E_INVALIDARG;
    *parent = nullptr;

    auto* layout = reinterpret_cast<S_CMFCRibbonTabsGroupLayout*>(pThis);
    auto* parentBar = reinterpret_cast<CMFCRibbonBar*>(layout->m_pParentRibbonBar);
    if (parentBar == nullptr || parentBar->GetSafeHwnd() == nullptr) return S_FALSE;

    *parent = reinterpret_cast<IDispatch*>(
        impl__GetAccessibleDispatch_CMFCRibbonBar__QEAAPEAUIDispatch__XZ(parentBar));
    return S_OK;
}
