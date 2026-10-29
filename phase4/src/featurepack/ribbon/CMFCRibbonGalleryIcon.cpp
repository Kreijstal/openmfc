// CMFCRibbonGalleryIcon — OpenMFC implementation.
// Sources: retail mfc140u disassembly; local layout below is pinned to those bodies.

#include "detail/ManualSmallStubImplementationsSupport.h"

#include <cstddef>
#include <cstdint>

namespace {
// CMFCRibbonGalleryIcon is not declared in include/openmfc. These bytes model
// only fields read by the transcribed methods; no names are assigned to opaque
// inherited words. The retail factory allocates 0x290 bytes (RVA 0x1116e0,
// mfc140u), and the cited methods read the fields pinned below.
struct CMFCRibbonGalleryIconLayout {
    std::uint8_t _toField13c[0x13c];
    std::uint32_t field_13c;
    std::uint32_t field_140;
    std::uint32_t field_144;
    std::uint32_t field_148;
    std::uint32_t field_14c;
    std::uint8_t _toOwner[0x270 - 0x150];
    void* owner;                     // +0x270
    std::int32_t index;              // +0x278
    std::uint8_t _tail[0x290 - 0x27c];
};
static_assert(offsetof(CMFCRibbonGalleryIconLayout, field_13c) == 0x13c);
static_assert(offsetof(CMFCRibbonGalleryIconLayout, field_144) == 0x144);
static_assert(offsetof(CMFCRibbonGalleryIconLayout, field_148) == 0x148);
static_assert(offsetof(CMFCRibbonGalleryIconLayout, field_14c) == 0x14c);
static_assert(offsetof(CMFCRibbonGalleryIconLayout, owner) == 0x270);
static_assert(offsetof(CMFCRibbonGalleryIconLayout, index) == 0x278);
static_assert(sizeof(CMFCRibbonGalleryIconLayout) == 0x290);

// GalleryIcon::GetRegularSize reads the gallery owner's DWORD at +0x4d4.
struct CMFCRibbonGalleryLayout {
    std::uint8_t _toField4d4[0x4d4];
    std::uint32_t field_4d4;
};
static_assert(offsetof(CMFCRibbonGalleryLayout, field_4d4) == 0x4d4);
static_assert(sizeof(void*) == 8);
static_assert(sizeof(CSize) == 8);

template <typename Fn>
Fn RibbonVirtual(void* pThis, std::size_t byteOffset) {
    void** vtable = *reinterpret_cast<void***>(pThis);
    return reinterpret_cast<Fn>(vtable[byteOffset / sizeof(void*)]);
}
} // namespace

extern "C" void MS_ABI impl__CopyFrom_CMFCRibbonButton__MEAAXAEBVCMFCRibbonBaseElement___Z(
    void* pThis, const void* pSource);
extern "C" void MS_ABI impl__OnLButtonDown_CMFCRibbonButton__MEAAXVCPoint___Z(
    void* pThis, long long point);

// Symbol: ??0CMFCRibbonGalleryIcon@@QEAA@PEAVCMFCRibbonGallery@@H@Z
extern "C" void* MS_ABI impl___0CMFCRibbonGalleryIcon__QEAA_PEAVCMFCRibbonGallery__H_Z(
    void* pThis, void* pGallery, int flags) {
    (void)pGallery;
    (void)flags;
    return pThis;
}

// Partial transcription: RVA 0x111db0 (mfc140u) delegates to
// CMFCRibbonButton::CopyFrom (RVA 0xe7aa0 (mfc140u)), then copies +0x278,
// +0x270, and +0x14c. The current base thunk is an argument-silencing stub;
// the derived fields below are nevertheless copied as retail does.
// Symbol: ?CopyFrom@CMFCRibbonGalleryIcon@@MEAAXAEBVCMFCRibbonBaseElement@@@Z
extern "C" void MS_ABI impl__CopyFrom_CMFCRibbonGalleryIcon__MEAAXAEBVCMFCRibbonBaseElement___Z(
    void* pThis, const void* pSource) {
    impl__CopyFrom_CMFCRibbonButton__MEAAXAEBVCMFCRibbonBaseElement___Z(pThis, pSource);
    auto* destination = static_cast<CMFCRibbonGalleryIconLayout*>(pThis);
    const auto* source = static_cast<const CMFCRibbonGalleryIconLayout*>(pSource);
    destination->index = source->index;
    destination->owner = source->owner;
    destination->field_14c = source->field_14c;
}

// Stub: RVA 0x1116e0 (mfc140u) allocates 0x290 bytes and calls the default
// constructor. The local GalleryIcon constructor is not initialized, and this
// class has no OpenMFC declaration/vtable with which to construct the object.
// Symbol: ?CreateObject@CMFCRibbonGalleryIcon@@SAPEAVCObject@@XZ
extern "C" void* MS_ABI impl__CreateObject_CMFCRibbonGalleryIcon__SAPEAVCObject__XZ() {
    return nullptr;
}

// RVA 0xe0b0 (mfc140u) dispatches vtable byte offset 0x200 on this with the
// CSize result buffer and CDC argument, then returns that result buffer.
// Symbol: ?GetCompactSize@CMFCRibbonGalleryIcon@@MEAA?AVCSize@@PEAVCDC@@@Z
extern "C" CSize* MS_ABI impl__GetCompactSize_CMFCRibbonGalleryIcon__MEAA_AVCSize__PEAVCDC___Z(
    void* pThis, CSize* pResult, CDC* pDC) {
    using DispatchFn = void (MS_ABI *)(void*, CSize*, CDC*);
    RibbonVirtual<DispatchFn>(pThis, 0x200)(pThis, pResult, pDC);
    return pResult;
}

// RVA 0x112060 (mfc140u) delegates to the owner gallery's vtable byte offset
// 0x538 with this icon as the argument and returns the supplied CString buffer.
// Symbol: ?GetDescription@CMFCRibbonGalleryIcon@@MEBA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@XZ
extern "C" void* MS_ABI impl__GetDescription_CMFCRibbonGalleryIcon__MEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__XZ(
    const void* pThis, void* pResult) {
    using DispatchFn = void (MS_ABI *)(void*, void*, const void*);
    void* pOwner = static_cast<const CMFCRibbonGalleryIconLayout*>(pThis)->owner;
    RibbonVirtual<DispatchFn>(pOwner, 0x538)(pOwner, pResult, pThis);
    return pResult;
}

// RVA 0x111e00 (mfc140u) gets the size through owner vtable byte offset
// 0x528. If the owner's DWORD at +0x4d4 is zero, retail adds 8 to both
// returned CSize components.
// Symbol: ?GetRegularSize@CMFCRibbonGalleryIcon@@MEAA?AVCSize@@PEAVCDC@@@Z
extern "C" CSize* MS_ABI impl__GetRegularSize_CMFCRibbonGalleryIcon__MEAA_AVCSize__PEAVCDC___Z(
    void* pThis, CSize* pResult, CDC* pDC) {
    void* pOwner = static_cast<CMFCRibbonGalleryIconLayout*>(pThis)->owner;
    using DispatchFn = void (MS_ABI *)(void*, CSize*, CDC*);
    RibbonVirtual<DispatchFn>(pOwner, 0x528)(pOwner, pResult, pDC);
    if (static_cast<CMFCRibbonGalleryLayout*>(pOwner)->field_4d4 == 0) {
        auto* components = reinterpret_cast<std::uint32_t*>(pResult);
        components[0] += 8;
        components[1] += 8;
    }
    return pResult;
}

// RVA 0x112030 (mfc140u) delegates to the owner gallery's vtable byte offset
// 0x530 with this icon as the argument and returns the supplied CString buffer.
// Symbol: ?GetToolTipText@CMFCRibbonGalleryIcon@@MEBA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@XZ
extern "C" void* MS_ABI impl__GetToolTipText_CMFCRibbonGalleryIcon__MEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__XZ(
    const void* pThis, void* pResult) {
    using DispatchFn = void (MS_ABI *)(void*, void*, const void*);
    void* pOwner = static_cast<const CMFCRibbonGalleryIconLayout*>(pThis)->owner;
    RibbonVirtual<DispatchFn>(pOwner, 0x530)(pOwner, pResult, pThis);
    return pResult;
}

// RVA 0x111e70 (mfc140u) computes unsigned (index + 2) <= 1; it does not
// read or write the nDelay reference.
// Symbol: ?IsAutoRepeatMode@CMFCRibbonGalleryIcon@@MEBAHAEAH@Z
extern "C" int MS_ABI impl__IsAutoRepeatMode_CMFCRibbonGalleryIcon__MEBAHAEAH_Z(
    const void* pThis, int* nDelay) {
    (void)nDelay;
    const std::uint32_t adjusted =
        static_cast<std::uint32_t>(static_cast<const CMFCRibbonGalleryIconLayout*>(pThis)->index) + 2U;
    return adjusted <= 1U;
}

// RVA 0x111e50 (mfc140u) returns whether the icon index at +0x278 is -1.
// Symbol: ?IsFirst@CMFCRibbonGalleryIcon@@QEBAHXZ
extern "C" int MS_ABI impl__IsFirst_CMFCRibbonGalleryIcon__QEBAHXZ(const void* pThis) {
    return static_cast<const CMFCRibbonGalleryIconLayout*>(pThis)->index == -1;
}

// RVA 0x111e60 (mfc140u) returns whether the icon index at +0x278 is -3.
// Symbol: ?IsLast@CMFCRibbonGalleryIcon@@QEBAHXZ
extern "C" int MS_ABI impl__IsLast_CMFCRibbonGalleryIcon__QEBAHXZ(const void* pThis) {
    return static_cast<const CMFCRibbonGalleryIconLayout*>(pThis)->index == -3;
}

// RVA 0x111ed0 (mfc140u) calls owner vtable byte offset 0x370 with the QAT
// argument, ignores its result, and returns 1.
// Symbol: ?OnAddToQAToolbar@CMFCRibbonGalleryIcon@@MEAAHAEAVCMFCRibbonQuickAccessToolBar@@@Z
extern "C" int MS_ABI impl__OnAddToQAToolbar_CMFCRibbonGalleryIcon__MEAAHAEAVCMFCRibbonQuickAccessToolBar___Z(
    void* pThis, void* pQAT) {
    void* pOwner = static_cast<CMFCRibbonGalleryIconLayout*>(pThis)->owner;
    using DispatchFn = void (MS_ABI *)(void*, void*);
    RibbonVirtual<DispatchFn>(pOwner, 0x370)(pOwner, pQAT);
    return 1;
}

// RVA 0x111e90 (mfc140u) returns 0 when the DWORD at +0x148 is nonzero;
// otherwise calls owner vtable byte offset 0x520 with this icon and returns 1.
// Symbol: ?OnAutoRepeat@CMFCRibbonGalleryIcon@@MEAAHXZ
extern "C" int MS_ABI impl__OnAutoRepeat_CMFCRibbonGalleryIcon__MEAAHXZ(void* pThis) {
    auto* pIcon = static_cast<CMFCRibbonGalleryIconLayout*>(pThis);
    if (pIcon->field_148 != 0) {
        return 0;
    }
    void* pOwner = pIcon->owner;
    using DispatchFn = void (MS_ABI *)(void*, void*);
    RibbonVirtual<DispatchFn>(pOwner, 0x520)(pOwner, pThis);
    return 1;
}

// Stub: RVA 0x111c40 (mfc140u) performs a multi-stage gallery click, including
// palette hit-testing and popup/command dispatch; those gallery and popup
// behaviors are not modeled by the available thunks.
// Symbol: ?OnClick@CMFCRibbonGalleryIcon@@MEAAXVCPoint@@@Z
extern "C" void MS_ABI impl__OnClick_CMFCRibbonGalleryIcon__MEAAXVCPoint___Z(
    void* pThis, long long point) {
    (void)pThis;
    (void)point;
}

// Stub: RVA 0x111a10 (mfc140u) performs visual-state checks, palette drawing,
// and multiple drawing/visual-manager dispatches; the relevant UI pipeline is
// not modeled by this file's available layouts and thunks.
// Symbol: ?OnDraw@CMFCRibbonGalleryIcon@@MEAAXPEAVCDC@@@Z
extern "C" void MS_ABI impl__OnDraw_CMFCRibbonGalleryIcon__MEAAXPEAVCDC___Z(
    void* pThis, CDC* pDC) {
    (void)pThis;
    (void)pDC;
}

// Stub: the Unicode --u lookup did not resolve this export. Its ANSI twin at
// RVA 0x112c90 (mfc140.dll) has multi-path highlight and popup dispatch that
// depends on unmodeled gallery and menu state.
// Symbol: ?OnHighlight@CMFCRibbonGalleryIcon@@MEAAXH@Z
extern "C" void MS_ABI impl__OnHighlight_CMFCRibbonGalleryIcon__MEAAXH_Z(
    void* pThis, int highlight) {
    (void)pThis;
    (void)highlight;
}

// ANSI RVA 0x112ae0 (mfc140.dll): for indices other than -3, retail tail-jumps
// to CMFCRibbonButton::OnLButtonDown. For index -3 it clears +0x144/+0x13c,
// calls this object's slot +0x380 with the CPoint, then owner slot +0x288.
// The base thunk currently remains an argument-silencing stub.
// Symbol: ?OnLButtonDown@CMFCRibbonGalleryIcon@@MEAAXVCPoint@@@Z
extern "C" void MS_ABI impl__OnLButtonDown_CMFCRibbonGalleryIcon__MEAAXVCPoint___Z(
    void* pThis, long long point) {
    auto* icon = static_cast<CMFCRibbonGalleryIconLayout*>(pThis);
    if (icon->index != -3) {
        impl__OnLButtonDown_CMFCRibbonButton__MEAAXVCPoint___Z(pThis, point);
        return;
    }

    icon->field_144 = 0;
    icon->field_13c = 0;
    using IconDispatchFn = void (MS_ABI *)(void*, long long);
    RibbonVirtual<IconDispatchFn>(pThis, 0x380)(pThis, point);
    void* owner = icon->owner;
    using OwnerDispatchFn = void (MS_ABI *)(void*);
    RibbonVirtual<OwnerDispatchFn>(owner, 0x288)(owner);
}

// Stub: the Unicode --u lookup did not resolve this export. Its ANSI twin at
// RVA 0x112570 (mfc140.dll) fills accessibility fields, creates/releases
// CString values, and queries several virtual properties; that path has not
// been faithfully transcribed in this thunk.
// Symbol: ?SetACCData@CMFCRibbonGalleryIcon@@UEAAHPEAVCWnd@@AEAVCAccessibilityData@@@Z
extern "C" int MS_ABI impl__SetACCData_CMFCRibbonGalleryIcon__UEAAHPEAVCWnd__AEAVCAccessibilityData___Z(
    void* pThis, void* pParent, void* pData) {
    (void)pThis;
    (void)pParent;
    (void)pData;
    return 0;
}
