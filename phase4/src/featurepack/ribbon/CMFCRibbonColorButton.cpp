// CMFCRibbonColorButton — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp

#include "detail/ManualSmallStubImplementationsSupport.h"

#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cwchar>

// These are existing method thunks, not C++ member calls: CString storage in the
// retail object is manipulated through its CSimpleStringT/CStringT exports.
extern "C" void MS_ABI impl__SetString___CSimpleStringT__W_00_ATL__QEAAXPEB_W_Z(
    void* pThis, const wchar_t* psz);
extern "C" int MS_ABI impl__Remove___CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAAH_W_Z(
    void* pThis, wchar_t ch);
extern "C" void* MS_ABI impl___0__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAA_XZ(
    void* pThis);
extern "C" int MS_ABI impl__SetACCData_CMFCRibbonBaseElement__UEAAHPEAVCWnd__AEAVCAccessibilityData___Z(
    CMFCRibbonBaseElement* pThis, CWnd* pParent, CAccessibilityData& data);
extern "C" void MS_ABI impl__NotifyHighlightListItem_CMFCRibbonBaseElement__UEAAXH_Z(
    CMFCRibbonBaseElement* pThis, int index);

namespace {

// CMFCRibbonColorButton is not declared in include/openmfc. This local tail view
// is pinned from the mfc140u exports below: CreateObject (RVA 0xf3be0) allocates
// 0x5d8 bytes; GetColorByIndex (RVA 0xf5280) reads the two CArray data/size
// pairs; EnableAutomaticButton (RVA 0xf4760) and EnableOtherButton (RVA 0xf4830)
// address the flags and CString fields. The retail header names/order are in
// afxribboncolorbutton.h; this view deliberately makes no claim about base fields.
struct RibbonColorButtonArrayView {
    void* vfptr;
    void* data;
    std::int64_t size;
    std::int64_t capacity;
    std::int64_t growBy;
};

struct RibbonColorButtonView {
    std::byte baseAndGallery[0x4f8];
    std::uint32_t color;
    std::uint32_t highlightedColor;
    std::uint32_t automaticColor;
    std::uint32_t pad0;
    RibbonColorButtonArrayView colors;
    RibbonColorButtonArrayView documentColors;
    RibbonColorButtonArrayView contiguousColumnRanges;
    std::int32_t automaticButton;
    std::int32_t automaticButtonOnTop;
    std::int32_t automaticButtonBorder;
    std::int32_t otherButton;
    std::int32_t hasGroups;
    std::int32_t simpleButtonLook;
    void* automaticButtonLabel;
    void* automaticButtonToolTip;
    void* otherButtonLabel;
    void* otherButtonToolTip;
    void* documentColorsLabel;
    void* otherButtonObject;
    void* automaticButtonObject;
    std::uint64_t colorBoxSize;
};

static_assert(sizeof(RibbonColorButtonArrayView) == 0x28);
static_assert(offsetof(RibbonColorButtonView, color) == 0x4f8);
static_assert(offsetof(RibbonColorButtonView, highlightedColor) == 0x4fc);
static_assert(offsetof(RibbonColorButtonView, automaticColor) == 0x500);
static_assert(offsetof(RibbonColorButtonView, colors) == 0x508);
static_assert(offsetof(RibbonColorButtonView, documentColors) == 0x530);
static_assert(offsetof(RibbonColorButtonView, contiguousColumnRanges) == 0x558);
static_assert(offsetof(RibbonColorButtonView, automaticButton) == 0x580);
static_assert(offsetof(RibbonColorButtonView, otherButton) == 0x58c);
static_assert(offsetof(RibbonColorButtonView, automaticButtonLabel) == 0x598);
static_assert(offsetof(RibbonColorButtonView, otherButtonLabel) == 0x5a8);
static_assert(offsetof(RibbonColorButtonView, otherButtonObject) == 0x5c0);
static_assert(offsetof(RibbonColorButtonView, colorBoxSize) == 0x5d0);
static_assert(sizeof(RibbonColorButtonView) == 0x5d8);

} // namespace

// Symbol: ??0CMFCRibbonColorButton@@QEAA@IPEB_WHHHK@Z
extern "C" void* MS_ABI impl___0CMFCRibbonColorButton__QEAA_IPEB_WHHHK_Z(
    void* pThis, int id, const wchar_t* text, int red, int green, int blue, int alpha) {
    (void)id;
    (void)text;
    (void)red;
    (void)green;
    (void)blue;
    (void)alpha;
    return pThis;
}
// Symbol: ??0CMFCRibbonColorButton@@QEAA@IPEB_WHK@Z
extern "C" void* MS_ABI impl___0CMFCRibbonColorButton__QEAA_IPEB_WHK_Z(
    void* pThis, int id, const wchar_t* text, int red, int green, int blue) {
    (void)id;
    (void)text;
    (void)red;
    (void)green;
    (void)blue;
    return pThis;
}
// Symbol: ??0CMFCRibbonColorButton@@QEAA@XZ
extern "C" void* MS_ABI impl___0CMFCRibbonColorButton__QEAA_XZ(void* pThis) {
    return pThis;
}
// The destructor export resolves at RVA 0xf41b0 (mfc140u), but it releases
// five shared strings, three CArrays, and then chains to the ribbon-gallery
// destructor; the required member destruction/layout support is not modeled.
// Symbol: ??1CMFCRibbonColorButton@@UEAA@XZ
extern "C" void MS_ABI impl___1CMFCRibbonColorButton__UEAA_XZ(void* /*pThis*/) {}

// RVA 0xf52d0 (mfc140u) traverses the supplied CList, appends colors, creates
// ribbon gallery groups, and updates per-group icon ranges. The gallery AddGroup
// thunk and the required CList node layout are not available here.
// Symbol: ?AddColorsGroup@CMFCRibbonColorButton@@QEAAXPEB_WAEBV?$CList@KK@@H@Z
extern "C" void MS_ABI impl__AddColorsGroup_CMFCRibbonColorButton__QEAAXPEB_WAEBV__CList_KK__H_Z(
    void* /*pThis*/, const wchar_t* /*name*/, const void* /*colors*/, int /*contiguousColumns*/) {}

// RVA 0xf4100 (mfc140u) initializes color/flag defaults and delegates remaining
// array and size initialization to internal helpers; the gallery/base layout is
// not modeled.
// Symbol: ?CommonInit@CMFCRibbonColorButton@@IEAAXXZ
extern "C" void MS_ABI impl__CommonInit_CMFCRibbonColorButton__IEAAXXZ(void* /*pThis*/) {}

// RVA 0xf48d0 (mfc140u) first calls the base CopyFrom thunk. It copies the
// color-button members only when the source passes the runtime-class check;
// otherwise it returns after the base call. The complete source/base layouts
// are not available as a shadow here.
// Symbol: ?CopyFrom@CMFCRibbonColorButton@@MEAAXAEBVCMFCRibbonBaseElement@@@Z
extern "C" void MS_ABI impl__CopyFrom_CMFCRibbonColorButton__MEAAXAEBVCMFCRibbonBaseElement___Z(
    void* /*pThis*/, const void* /*source*/) {}

// RVA 0xf3be0 (mfc140u) allocates 0x5d8 bytes and invokes the default
// constructor. That constructor is currently a no-op thunk, so returning an
// allocated but uninitialized object would not reproduce retail behavior.
// Symbol: ?CreateObject@CMFCRibbonColorButton@@SAPEAVCObject@@XZ
extern "C" void* MS_ABI impl__CreateObject_CMFCRibbonColorButton__SAPEAVCObject__XZ() {
    return nullptr;
}

// RVA 0xf42d0 (mfc140u) performs DPI-sensitive rectangle adjustment and routes
// palette drawing through visual-manager virtual calls; those dependencies are
// not modeled in this file.
// Symbol: ?DrawImage@CMFCRibbonColorButton@@MEAAXPEAVCDC@@W4RibbonImageType@CMFCRibbonBaseElement@@VCRect@@@Z
extern "C" void MS_ABI impl__DrawImage_CMFCRibbonColorButton__MEAAXPEAVCDC__W4RibbonImageType_CMFCRibbonBaseElement__VCRect___Z(
    void* /*pThis*/, void* /*pDC*/, int /*imageType*/, const void* /*rectImage*/) {}

// RVA 0xf4760 (mfc140u) stores the automatic label/color/flags, defaults a
// missing tooltip to the label, and strips '&' from that tooltip. The retail
// body uses explicit length assignment; SetString preserves the same text while
// routing the operation through its existing thunk.
// Symbol: ?EnableAutomaticButton@CMFCRibbonColorButton@@QEAAXPEB_WKH0HH@Z
extern "C" void MS_ABI impl__EnableAutomaticButton_CMFCRibbonColorButton__QEAAXPEB_WKH0HH_Z(
    void* pThis, const wchar_t* label, unsigned long colorAutomatic,
    int enable, const wchar_t* toolTip, int onTop, int drawBorder) {
    auto* self = static_cast<RibbonColorButtonView*>(pThis);
    const wchar_t* effectiveLabel = label ? label : L"";
    impl__SetString___CSimpleStringT__W_00_ATL__QEAAXPEB_W_Z(
        &self->automaticButtonLabel, effectiveLabel);
    impl__SetString___CSimpleStringT__W_00_ATL__QEAAXPEB_W_Z(
        &self->automaticButtonToolTip, toolTip ? toolTip : effectiveLabel);
    impl__Remove___CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAAH_W_Z(
        &self->automaticButtonToolTip, L'&');
    self->automaticColor = static_cast<std::uint32_t>(colorAutomatic);
    self->automaticButton = enable;
    self->automaticButtonOnTop = onTop;
    self->automaticButtonBorder = drawBorder;
}

// RVA 0xf4830 (mfc140u) enables the other-button flag iff its label is non-null,
// stores the label and tooltip (defaulting the tooltip to the label), and strips
// '&' from the tooltip.
// Symbol: ?EnableOtherButton@CMFCRibbonColorButton@@QEAAXPEB_W0@Z
extern "C" void MS_ABI impl__EnableOtherButton_CMFCRibbonColorButton__QEAAXPEB_W0_Z(
    void* pThis, const wchar_t* label, const wchar_t* toolTip) {
    auto* self = static_cast<RibbonColorButtonView*>(pThis);
    self->otherButton = label != nullptr;
    const wchar_t* effectiveLabel = label ? label : L"";
    impl__SetString___CSimpleStringT__W_00_ATL__QEAAXPEB_W_Z(
        &self->otherButtonLabel, effectiveLabel);
    impl__SetString___CSimpleStringT__W_00_ATL__QEAAXPEB_W_Z(
        &self->otherButtonToolTip, toolTip ? toolTip : effectiveLabel);
    impl__Remove___CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAAH_W_Z(
        &self->otherButtonToolTip, L'&');
}

// RVA 0xf5280 (mfc140u) indexes m_Colors first, then m_DocumentColors after
// subtracting the first array's size; negative/out-of-range indices return -1.
// Symbol: ?GetColorByIndex@CMFCRibbonColorButton@@IEBAKH@Z
extern "C" unsigned long MS_ABI impl__GetColorByIndex_CMFCRibbonColorButton__IEBAKH_Z(
    const void* pThis, int index) {
    const auto* self = static_cast<const RibbonColorButtonView*>(pThis);
    if (index < 0) return 0xffffffffUL;
    if (static_cast<std::int64_t>(index) < self->colors.size) {
        return static_cast<const std::uint32_t*>(self->colors.data)[index];
    }
    const std::int64_t documentIndex =
        static_cast<std::int64_t>(index) - self->colors.size;
    if (documentIndex >= self->documentColors.size) return 0xffffffffUL;
    return static_cast<const std::uint32_t*>(self->documentColors.data)[documentIndex];
}

// RVA 0xf4bd0 (mfc140u) returns the highlighted-color member at +0x4fc.
// Symbol: ?GetHighlightedColor@CMFCRibbonColorButton@@QEBAKXZ
extern "C" unsigned long MS_ABI impl__GetHighlightedColor_CMFCRibbonColorButton__QEBAKXZ(
    const void* pThis) {
    return static_cast<const RibbonColorButtonView*>(pThis)->highlightedColor;
}

// RVA 0xf5640 (mfc140u) maps the icon index to a color, looks up the color-name
// table or formats an RGB fallback, and otherwise delegates to the base tooltip.
// The color-name table and full CString/base-gallery behavior are unavailable;
// this conservative stub constructs an empty CString result.
// Symbol: ?GetIconToolTip@CMFCRibbonColorButton@@MEBA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@PEBVCMFCRibbonGalleryIcon@@@Z
extern "C" void* MS_ABI impl__GetIconToolTip_CMFCRibbonColorButton__MEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__PEBVCMFCRibbonGalleryIcon___Z(
    void* pRet, const void* /*pThis*/, const void* /*pIcon*/) {
    return impl___0__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAA_XZ(pRet);
}

// RVA 0xf5610 (mfc140u) stores GetColorByIndex(index) in +0x4fc and forwards
// to the base notification thunk. That base thunk remains a conservative stub,
// so the parent-ribbon notification itself is not reproduced.
// Symbol: ?NotifyHighlightListItem@CMFCRibbonColorButton@@MEAAXH@Z
extern "C" void MS_ABI impl__NotifyHighlightListItem_CMFCRibbonColorButton__MEAAXH_Z(
    void* pThis, int index) {
    auto* self = static_cast<RibbonColorButtonView*>(pThis);
    self->highlightedColor = static_cast<std::uint32_t>(
        impl__GetColorByIndex_CMFCRibbonColorButton__IEBAKH_Z(pThis, index));
    impl__NotifyHighlightListItem_CMFCRibbonBaseElement__UEAAXH_Z(
        static_cast<CMFCRibbonBaseElement*>(pThis), index);
}

// RVA 0xf5240 (mfc140u) updates the selected color for valid icon indices and
// then forwards the click to CMFCRibbonGallery; that sibling thunk is a stub.
// Symbol: ?OnClickPaletteIcon@CMFCRibbonColorButton@@MEAAXPEAVCMFCRibbonGalleryIcon@@@Z
extern "C" void MS_ABI impl__OnClickPaletteIcon_CMFCRibbonColorButton__MEAAXPEAVCMFCRibbonGalleryIcon___Z(
    void* /*pThis*/, void* /*icon*/) {}

// RVA 0xf5120 (mfc140u) handles the automatic/other popup buttons, invokes
// palette selection and notification paths, and otherwise returns FALSE. The
// popup/menu and temporary command-routing infrastructure is not modeled.
// Symbol: ?OnClickPaletteSubItem@CMFCRibbonColorButton@@MEAAHPEAVCMFCRibbonButton@@PEAVCMFCRibbonPanelMenuBar@@@Z
extern "C" int MS_ABI impl__OnClickPaletteSubItem_CMFCRibbonColorButton__MEAAHPEAVCMFCRibbonButton__PEAVCMFCRibbonPanelMenuBar___Z(
    void* /*pThis*/, void* /*button*/, void* /*menuBar*/) {
    return 0;
}

// RVA 0xf4c70 (mfc140u) computes selection/automatic/other states, updates the
// icon rectangle, and dispatches drawing through the visual-manager vtable.
// The relevant visual-manager and icon drawing state is not modeled.
// Symbol: ?OnDrawPaletteIcon@CMFCRibbonColorButton@@MEAAXPEAVCDC@@VCRect@@HPEAVCMFCRibbonGalleryIcon@@K@Z
extern "C" void MS_ABI impl__OnDrawPaletteIcon_CMFCRibbonColorButton__MEAAXPEAVCDC__VCRect__HPEAVCMFCRibbonGalleryIcon__K_Z(
    void* /*pThis*/, void* /*pDC*/, const void* /*rect*/, int /*iconIndex*/,
    void* /*icon*/, unsigned long /*textColor*/) {}

// The Unicode symbol is absent from the --u map; its ANSI twin at RVA 0xf59f0
// (mfc140) builds palette icons and automatic/other popup buttons, then updates
// gallery layout. Those gallery/menu layouts and callback paths are not modeled.
// Symbol: ?OnShowPopupMenu@CMFCRibbonColorButton@@MEAAXXZ
extern "C" void MS_ABI impl__OnShowPopupMenu_CMFCRibbonColorButton__MEAAXXZ(void* /*pThis*/) {}

// The Unicode symbol is absent from the --u map; its byte-identical ANSI twin
// at RVA 0xf5ff0 (mfc140) clears the color array and group-range allocation,
// resets the group flag, then dispatches the gallery vtable slot at +0x510.
// Symbol: ?RemoveAllColorGroups@CMFCRibbonColorButton@@QEAAXXZ
extern "C" void MS_ABI impl__RemoveAllColorGroups_CMFCRibbonColorButton__QEAAXXZ(
    void* pThis) {
    if (!pThis) return;

    auto* self = static_cast<RibbonColorButtonView*>(pThis);
    if (self->colors.data) std::free(self->colors.data);
    self->colors.data = nullptr;
    self->colors.size = 0;
    self->colors.capacity = 0;

    if (self->contiguousColumnRanges.data) {
        std::free(self->contiguousColumnRanges.data);
    }
    self->contiguousColumnRanges.data = nullptr;
    self->contiguousColumnRanges.size = 0;
    self->contiguousColumnRanges.capacity = 0;
    self->hasGroups = 0;

    using GalleryUpdate = void (MS_ABI *)(void*);
    void** vtable = *static_cast<void***>(pThis);
    auto update = reinterpret_cast<GalleryUpdate>(vtable[0x510 / sizeof(void*)]);
    update(pThis);
}

// The Unicode symbol is absent from the --u map; its ANSI twin at RVA 0xf5280
// (mfc140) delegates to base accessibility, sets role 0x3a, and formats the
// selected RGB value. The base thunk is currently a stub, so only this derived
// role/name portion can be reproduced here.
// Symbol: ?SetACCData@CMFCRibbonColorButton@@UEAAHPEAVCWnd@@AEAVCAccessibilityData@@@Z
extern "C" int MS_ABI impl__SetACCData_CMFCRibbonColorButton__UEAAHPEAVCWnd__AEAVCAccessibilityData___Z(
    void* pThis, CWnd* pParent, CAccessibilityData* pData) {
    if (!pThis || !pData) return FALSE;

    (void)impl__SetACCData_CMFCRibbonBaseElement__UEAAHPEAVCWnd__AEAVCAccessibilityData___Z(
        static_cast<CMFCRibbonBaseElement*>(pThis), pParent, *pData);

    const auto color = static_cast<const RibbonColorButtonView*>(pThis)->color;
    wchar_t rgb[32];
    std::swprintf(rgb, sizeof(rgb) / sizeof(rgb[0]), L"RGB(%u, %u, %u)",
        color & 0xffu, (color >> 8) & 0xffu, (color >> 16) & 0xffu);
    pData->m_nAccRole = 0x3a;
    impl__SetString___CSimpleStringT__W_00_ATL__QEAAXPEB_W_Z(&pData->m_strAccName, rgb);
    return TRUE;
}

// The Unicode symbol is absent from the --u map; the ANSI twin at RVA 0xf6070
// (mfc140) scales CSize using module DPI state, writes +0x5d0 and updates gallery
// sizing fields at +0x350/+0x354. The DPI state and gallery sizing path are not
// modeled here.
// Symbol: ?SetColorBoxSize@CMFCRibbonColorButton@@QEAAXVCSize@@@Z
extern "C" void MS_ABI impl__SetColorBoxSize_CMFCRibbonColorButton__QEAAXVCSize___Z(
    void* /*pThis*/, long long /*sizeBox*/) {}

// The Unicode wchar_t symbol is absent from the --u map. The ANSI char twin at
// RVA 0xf57a0 (mfc140) copies the supplied CList values into the document-color
// array and stores its label; the retail CList layout is not modeled here.
// Symbol: ?SetDocumentColors@CMFCRibbonColorButton@@QEAAXPEB_WAEAV?$CList@KK@@@Z
extern "C" void MS_ABI impl__SetDocumentColors_CMFCRibbonColorButton__QEAAXPEB_WAEAV__CList_KK___Z(
    void* /*pThis*/, const wchar_t* /*label*/, void* /*colors*/) {}

// The Unicode symbol is absent from the --u map; the ANSI twin at RVA 0xf5730
// (mfc140) optionally sets a default box size, clears the regular-color array,
// and copies palette entries. The CPalette-to-array path is not modeled here.
// Symbol: ?SetPalette@CMFCRibbonColorButton@@QEAAXPEAVCPalette@@@Z
extern "C" void MS_ABI impl__SetPalette_CMFCRibbonColorButton__QEAAXPEAVCPalette___Z(
    void* /*pThis*/, void* /*palette*/) {}

// The Unicode symbol is absent from the --u map; the ANSI twin at RVA 0xf5100
// (mfc140) updates the selected color, propagates it to related ribbon elements,
// and invalidates the owning UI. Those parent and element layouts are not modeled.
// Symbol: ?UpdateColor@CMFCRibbonColorButton@@QEAAXK@Z
extern "C" void MS_ABI impl__UpdateColor_CMFCRibbonColorButton__QEAAXK_Z(
    void* /*pThis*/, unsigned long /*color*/) {}
