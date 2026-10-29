// CMFCRibbonUndoButton — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp

#include "detail/ManualSmallStubImplementationsSupport.h"

#include <cstddef>
#include <cstdint>
#include <cstring>

namespace {
// This class is not declared in include/openmfc. The retail member offsets
// below are read from the mfc140u bodies cited at their thunks; pointer-sized
// string/array storage follows the retail headers and is pinned here as bytes.
struct CMFCRibbonUndoButtonShadow {
    std::uint8_t _toIconCount[0x4c8];
    std::int32_t m_nIcons;                  // inherited gallery icon count +0x4c8
    std::uint8_t _toActionNumber[0x4f8 - 0x4cc];
    std::int32_t m_nActionNumber;           // +0x4f8
    std::uint32_t m_sizeMaxText[2];         // CSize at +0x4fc
    std::uint32_t _padding504;
    void* m_strCancel;                      // CString at +0x508
    void* m_strUndoOne;                     // CString at +0x510
    void* m_strUndoFmt;                     // CString at +0x518
    std::uintptr_t m_arLabels[5];           // CStringArray at +0x520
};
static_assert(offsetof(CMFCRibbonUndoButtonShadow, m_nIcons) == 0x4c8);
static_assert(offsetof(CMFCRibbonUndoButtonShadow, m_nActionNumber) == 0x4f8);
static_assert(offsetof(CMFCRibbonUndoButtonShadow, m_sizeMaxText) == 0x4fc);
static_assert(offsetof(CMFCRibbonUndoButtonShadow, m_strCancel) == 0x508);
static_assert(offsetof(CMFCRibbonUndoButtonShadow, m_strUndoOne) == 0x510);
static_assert(offsetof(CMFCRibbonUndoButtonShadow, m_strUndoFmt) == 0x518);
static_assert(offsetof(CMFCRibbonUndoButtonShadow, m_arLabels) == 0x520);
static_assert(sizeof(CMFCRibbonUndoButtonShadow) == 0x548);
static_assert(sizeof(CSize) == 8);
static_assert(sizeof(CString) == sizeof(void*));
// The clean-room CStringArray is an 8-byte side-table wrapper; the five
// pointer-sized words above pin the 0x28-byte retail member at +0x520.
void InitKnownUndoState(void* pThis) {
    if (pThis == nullptr) {
        return;
    }
    auto* shadow = static_cast<CMFCRibbonUndoButtonShadow*>(pThis);
    shadow->m_nActionNumber = -1;
    shadow->m_sizeMaxText[0] = 0;
    shadow->m_sizeMaxText[1] = 0;
    shadow->m_arLabels[2] = 0;
}
} // namespace

extern "C" void MS_ABI impl__OnClick_CMFCRibbonButton__UEAAXVCPoint___Z(
    void* pThis, long long point);
extern "C" int MS_ABI impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(
    const CObject* pThis, const CRuntimeClass* pClass);
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMFCRibbonButton__SAPEAUCRuntimeClass__XZ();
extern "C" void MS_ABI impl__Copy_CMFCRibbonGallery__MEAAXAEBVCMFCRibbonBaseElement___Z(
    void* pThis, const CMFCRibbonBaseElement* pSource);
extern "C" void MS_ABI impl__Copy_CStringArray__QEAAXAEBV1__Z(
    CStringArray* pThis, const CStringArray* pSource);
extern "C" void MS_ABI impl__SetAtGrow_CStringArray__QEAAX_JPEB_W_Z(
    CStringArray* pThis, long long nIndex, const wchar_t* value);
extern "C" void MS_ABI impl__SetSize_CStringArray__QEAAX_J0_Z(
    CStringArray* pThis, long long nNewSize, long long nGrowBy);

// Partial constructor: retail RVA 0x128810 (mfc140u) constructs the gallery
// base, installs the UndoButton vtable, initializes strings/labels, and calls
// CommonInit. This file can initialize only the known action/cache/count fields;
// it has no local UndoButton type or derived vtable to construct.
// Symbol: ??0CMFCRibbonUndoButton@@QEAA@IPEB_WHH@Z
extern "C" void* MS_ABI impl___0CMFCRibbonUndoButton__QEAA_IPEB_WHH_Z(
    void* pThis, int id, const wchar_t* text, int width, int height) {
    (void)id;
    (void)text;
    (void)width;
    (void)height;
    InitKnownUndoState(pThis);
    return pThis;
}
// Partial constructor: retail RVA 0x128920 (mfc140u) constructs the gallery
// base, installs the UndoButton vtable, initializes strings/labels, calls
// CommonInit, and stores the icon at +0x1d8. No local derived type/vtable is
// available here.
// Symbol: ??0CMFCRibbonUndoButton@@QEAA@IPEB_WPEAUHICON__@@@Z
extern "C" void* MS_ABI impl___0CMFCRibbonUndoButton__QEAA_IPEB_WPEAUHICON_____Z(
    void* pThis, int id, const wchar_t* text, void* icon) {
    (void)id;
    (void)text;
    (void)icon;
    InitKnownUndoState(pThis);
    return pThis;
}
// Partial constructor: retail RVA 0x1286e0 (mfc140u) constructs the gallery
// base, installs the UndoButton vtable, initializes strings/labels, and calls
// CommonInit. No local derived type/vtable is available to establish that state.
// Symbol: ??0CMFCRibbonUndoButton@@QEAA@XZ
extern "C" void* MS_ABI impl___0CMFCRibbonUndoButton__QEAA_XZ(void* pThis) {
    InitKnownUndoState(pThis);
    return pThis;
}
// Stub: retail entry RVA 0x128b90 (mfc140u) destroys the label array at +0x520,
// releases the three CString members at +0x508/+0x510/+0x518, then tail-calls
// CMFCRibbonGallery::~CMFCRibbonGallery at RVA 0x112770 (mfc140u). The local
// constructors initialize only scalar bookkeeping, not the CString/array
// objects, so releasing those members here would be unsafe.
// Symbol: ??1CMFCRibbonUndoButton@@UEAA@XZ
extern "C" void MS_ABI impl___1CMFCRibbonUndoButton__UEAA_XZ(void* pThis) {
    (void)pThis;
}

// Partial: retail entry RVA 0x128c40 (mfc140u) calls vtable slot 0x510 (slot 162),
// appends to m_arLabels (+0x520), copies its count from +0x530 to the inherited
// icon-count field at +0x4c8, and clears the cached CSize at +0x4fc. The local
// side-table wrapper handles the append/count/cache effects; the virtual call
// is omitted because these thunks cannot establish the UndoButton vtable.
// Symbol: ?AddUndoAction@CMFCRibbonUndoButton@@QEAAXPEB_W@Z
extern "C" void MS_ABI impl__AddUndoAction_CMFCRibbonUndoButton__QEAAXPEB_W_Z(
    void* pThis, const wchar_t* lpszLabel) {
    if (pThis == nullptr) {
        return;
    }
    auto* shadow = static_cast<CMFCRibbonUndoButtonShadow*>(pThis);
    auto* labels = reinterpret_cast<CStringArray*>(
        static_cast<std::uint8_t*>(pThis) + 0x520);
    const long long index = static_cast<long long>(shadow->m_arLabels[2]);
    impl__SetAtGrow_CStringArray__QEAAX_JPEB_W_Z(labels, index, lpszLabel);
    shadow->m_arLabels[2] = static_cast<std::uintptr_t>(index + 1);
    shadow->m_nIcons = static_cast<std::int32_t>(index + 1);
    shadow->m_sizeMaxText[0] = 0;
    shadow->m_sizeMaxText[1] = 0;
}

// Partial: retail entry RVA 0x128ca0 (mfc140u) calls vtable slot 0x510 (slot 162),
// clears m_arLabels via SetSize(0, -1), and zeroes the CSize at +0x4fc. This
// clears the OpenMFC side-table labels/count and cache, but omits the unavailable
// virtual call.
// Symbol: ?CleanUpUndoList@CMFCRibbonUndoButton@@QEAAXXZ
extern "C" void MS_ABI impl__CleanUpUndoList_CMFCRibbonUndoButton__QEAAXXZ(
    void* pThis) {
    if (pThis == nullptr) {
        return;
    }
    auto* shadow = static_cast<CMFCRibbonUndoButtonShadow*>(pThis);
    auto* labels = reinterpret_cast<CStringArray*>(
        static_cast<std::uint8_t*>(pThis) + 0x520);
    impl__SetSize_CStringArray__QEAAX_J0_Z(labels, 0, -1);
    shadow->m_arLabels[2] = 0;
    shadow->m_sizeMaxText[0] = 0;
    shadow->m_sizeMaxText[1] = 0;
}

// Partial: retail entry RVA 0x128a40 (mfc140u) sets +0x4f8 to -1, writes 1
// at +0x4c4/+0x4cc/+0x1ec, and clears the cached CSize at +0x4fc. It then loads
// string resources 0x42cd/0x42cf/0x42ce and creates/configures an internal
// ribbon button; that resource/object setup and its dispatch remain unavailable.
// Symbol: ?CommonInit@CMFCRibbonUndoButton@@IEAAXXZ
extern "C" void MS_ABI impl__CommonInit_CMFCRibbonUndoButton__IEAAXXZ(
    void* pThis) {
    if (pThis == nullptr) {
        return;
    }
    auto* bytes = static_cast<std::uint8_t*>(pThis);
    *reinterpret_cast<std::int32_t*>(bytes + 0x4f8) = -1;
    *reinterpret_cast<std::int32_t*>(bytes + 0x4c4) = 1;
    *reinterpret_cast<std::int32_t*>(bytes + 0x4cc) = 1;
    *reinterpret_cast<std::int32_t*>(bytes + 0x1ec) = 1;
    std::memset(bytes + 0x4fc, 0, sizeof(std::uint64_t));
}

// Partial: retail entry RVA 0x128e00 (mfc140u) calls CMFCRibbonGallery::CopyFrom
// at RVA 0x113f60 (mfc140u), then clears/copies the label array and copies the
// action index, icon count, and cached CSize. The gallery thunk is a no-op, so
// base-element state remains unavailable; the label copy uses OpenMFC's
// CStringArray side-table wrapper at the retail member address +0x520.
// Symbol: ?CopyFrom@CMFCRibbonUndoButton@@MEAAXAEBVCMFCRibbonBaseElement@@@Z
extern "C" void MS_ABI impl__CopyFrom_CMFCRibbonUndoButton__MEAAXAEBVCMFCRibbonBaseElement___Z(
    void* pThis, const void* pSource) {
    if (pThis == nullptr || pSource == nullptr) {
        return;
    }

    impl__Copy_CMFCRibbonGallery__MEAAXAEBVCMFCRibbonBaseElement___Z(
        pThis, static_cast<const CMFCRibbonBaseElement*>(pSource));

    auto* destinationLabels = reinterpret_cast<CStringArray*>(
        static_cast<std::uint8_t*>(pThis) + 0x520);
    const auto* sourceLabels = reinterpret_cast<const CStringArray*>(
        static_cast<const std::uint8_t*>(pSource) + 0x520);
    impl__SetSize_CStringArray__QEAAX_J0_Z(destinationLabels, 0, -1);
    impl__Copy_CStringArray__QEAAXAEBV1__Z(destinationLabels, sourceLabels);

    auto* destination = static_cast<CMFCRibbonUndoButtonShadow*>(pThis);
    const auto* source = static_cast<const CMFCRibbonUndoButtonShadow*>(pSource);
    destination->m_nActionNumber = source->m_nActionNumber;
    destination->m_arLabels[2] = source->m_arLabels[2];
    destination->m_nIcons = source->m_nIcons;
    std::memcpy(destination->m_sizeMaxText, source->m_sizeMaxText,
                sizeof(destination->m_sizeMaxText));
}

// Stub: retail at RVA 0x1286a0 (mfc140u) allocates 0x548 bytes and calls the
// default constructor at RVA 0x1286e0 (mfc140u). The local default constructor
// sets scalar bookkeeping but does not establish a ribbon vtable or initialize
// its CString/array objects, so returning a partially constructed object is unsafe.
// Symbol: ?CreateObject@CMFCRibbonUndoButton@@SAPEAVCObject@@XZ
extern "C" void* MS_ABI impl__CreateObject_CMFCRibbonUndoButton__SAPEAVCObject__XZ() {
    return nullptr;
}

// Symbol: ?GetIconSize@CMFCRibbonUndoButton@@MEBA?AVCSize@@XZ
extern "C" void* MS_ABI impl__GetIconSize_CMFCRibbonUndoButton__MEBA_AVCSize__XZ(
    const void* pThis, void* pResult) {
    // Retail RVA 0x128e90 (mfc140u) reads eight bytes from this+0x4fc
    // into the hidden return buffer and returns that buffer.
    std::memcpy(pResult, static_cast<const std::uint8_t*>(pThis) + 0x4fc, sizeof(std::uint64_t));
    return pResult;
}

// Stub: retail at RVA 0x128ce0 (mfc140u) updates the action index, adjusts the
// selected gallery item, uses popup/panel state and then delegates to the base
// highlight handler. Those gallery and popup objects are not modeled here.
// Symbol: ?NotifyHighlightListItem@CMFCRibbonUndoButton@@MEAAXH@Z
extern "C" void MS_ABI impl__NotifyHighlightListItem_CMFCRibbonUndoButton__MEAAXH_Z(
    void* pThis, int nIndex) {
    (void)pThis;
    (void)nIndex;
}

// Retail at RVA 0x128e80 (mfc140u) stores -1 at +0x4f8 and tail-jumps to
// CMFCRibbonButton::OnClick with the same by-value CPoint (an 8-byte MS ABI
// aggregate). That sibling thunk is currently a stub, so the delegation is
// retained but the base click behavior remains a documented limitation.
// Symbol: ?OnClick@CMFCRibbonUndoButton@@MEAAXVCPoint@@@Z
extern "C" void MS_ABI impl__OnClick_CMFCRibbonUndoButton__MEAAXVCPoint___Z(
    void* pThis, long long point) {
    static_cast<CMFCRibbonUndoButtonShadow*>(pThis)->m_nActionNumber = -1;
    impl__OnClick_CMFCRibbonButton__UEAAXVCPoint___Z(pThis, point);
}

// Retail RVA 0x129000 (mfc140u) checks pButton with CObject::IsKindOf
// against the CMFCRibbonButton descriptor, dispatches this object's vtable
// slot 0x340 on a match, and returns 1; otherwise it returns 0. Both the
// runtime-class getter and IsKindOf thunk are available in OpenMFC.
// Symbol: ?OnClickPaletteSubItem@CMFCRibbonUndoButton@@MEAAHPEAVCMFCRibbonButton@@PEAVCMFCRibbonPanelMenuBar@@@Z
extern "C" int MS_ABI impl__OnClickPaletteSubItem_CMFCRibbonUndoButton__MEAAHPEAVCMFCRibbonButton__PEAVCMFCRibbonPanelMenuBar___Z(
    void* pThis, void* pButton, void* pMenuBar) {
    (void)pMenuBar;
    if (!impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(
            static_cast<const CObject*>(static_cast<CMFCRibbonButton*>(pButton)),
            impl__GetThisClass_CMFCRibbonButton__SAPEAUCRuntimeClass__XZ())) {
        return 0;
    }
    void** vtable = *reinterpret_cast<void***>(pThis);
    using DispatchFn = void (MS_ABI *)(void*);
    reinterpret_cast<DispatchFn>(vtable[0x340 / sizeof(void*)])(pThis);
    return 1;
}

// Stub: retail at RVA 0x128ea0 (mfc140u) manipulates CDC and gallery-icon
// state, calls multiple virtual drawing methods, and reads the undo-label
// array. Those drawing objects and virtual dispatches are not modeled.
// Symbol: ?OnDrawPaletteIcon@CMFCRibbonUndoButton@@MEAAXPEAVCDC@@VCRect@@HPEAVCMFCRibbonGalleryIcon@@K@Z
extern "C" void MS_ABI impl__OnDrawPaletteIcon_CMFCRibbonUndoButton__MEAAXPEAVCDC__VCRect__HPEAVCMFCRibbonGalleryIcon__K_Z(
    void* pThis, void* pDC, const void* rectIcon, int nIconIndex,
    void* pIcon, unsigned long clrText) {
    (void)pThis;
    (void)pDC;
    (void)rectIcon;
    (void)nIconIndex;
    (void)pIcon;
    (void)clrText;
}

// Stub: the Unicode lookup did not resolve. ANSI-twin entry RVA 0x129db0
// (mfc140) first calls a helper, then zeroes +0x4d4. If either cached CSize
// dimension at +0x4fc/+0x500 is nonzero, it skips the measurement path; if both
// are zero, it measures every m_arLabels entry (+0x520), then also measures the
// three CString members at +0x508/+0x510/+0x518. It caches the maximum dimensions
// and adds 10 to the width. Both paths set +0x4f8 to -1 and tail-jump to
// CMFCRibbonGallery::OnShowPopupMenu at RVA 0x114f70 (mfc140). This popup/layout
// pipeline is not modeled here.
// Symbol: ?OnShowPopupMenu@CMFCRibbonUndoButton@@MEAAXXZ
extern "C" void MS_ABI impl__OnShowPopupMenu_CMFCRibbonUndoButton__MEAAXXZ(
    void* pThis) {
    (void)pThis;
}

