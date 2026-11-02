// CMFCRibbonConstructor — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp

#include "detail/ManualSmallStubImplementationsSupport.h"

#include <cstddef>

namespace {
struct RibbonConstructorXIDView {
    CString m_Name;
    unsigned int m_Value;
    unsigned int _pad_0c;
};
static_assert(offsetof(RibbonConstructorXIDView, m_Value) == 0x08,
              "CMFCRibbonInfo::XID::m_Value offset");
static_assert(sizeof(RibbonConstructorXIDView) == 0x10,
              "CMFCRibbonInfo::XID size");

struct RibbonConstructorXElementView {
    void* m_vfptr;
    int m_ElementType;
    int _pad_0c;
    RibbonConstructorXIDView m_ID;
    CString m_strText;
    CString m_strToolTip;
    CString m_strDescription;
    CString m_strKeys;
    CString m_strMenuKeys;
    int m_bIsOnPaletteTop;
    int m_bIsAlwaysLarge;
};
static_assert(offsetof(RibbonConstructorXElementView, m_ID) == 0x10,
              "CMFCRibbonInfo::XElement::m_ID offset");
static_assert(offsetof(RibbonConstructorXElementView, m_strText) == 0x20,
              "CMFCRibbonInfo::XElement::m_strText offset");
static_assert(offsetof(RibbonConstructorXElementView, m_strToolTip) == 0x28,
              "CMFCRibbonInfo::XElement::m_strToolTip offset");
static_assert(offsetof(RibbonConstructorXElementView, m_strDescription) == 0x30,
              "CMFCRibbonInfo::XElement::m_strDescription offset");
static_assert(offsetof(RibbonConstructorXElementView, m_strMenuKeys) == 0x40,
              "CMFCRibbonInfo::XElement::m_strMenuKeys offset");
static_assert(offsetof(RibbonConstructorXElementView, m_bIsAlwaysLarge) == 0x4c,
              "CMFCRibbonInfo::XElement::m_bIsAlwaysLarge offset");

struct RibbonConstructorXPanelView {
    void* m_vfptr;
    int m_ElementType;
    int _pad_0c;
    CString m_strName;
    CString m_strKeys;
    int m_nImageIndex;
    int m_bJustifyColumns;
    int m_bCenterColumnVert;
};
static_assert(offsetof(RibbonConstructorXPanelView, m_strName) == 0x10,
              "CMFCRibbonInfo::XPanel::m_strName offset");
static_assert(offsetof(RibbonConstructorXPanelView, m_strKeys) == 0x18,
              "CMFCRibbonInfo::XPanel::m_strKeys offset");
static_assert(offsetof(RibbonConstructorXPanelView, m_nImageIndex) == 0x20,
              "CMFCRibbonInfo::XPanel::m_nImageIndex offset");

struct RibbonConstructorXImageView {
    RibbonConstructorXIDView m_ID;
    RibbonConstructorXIDView m_ID_HDPI;
    CMFCToolBarImages m_Image;
};
static_assert(offsetof(RibbonConstructorXImageView, m_Image) == 0x20,
              "CMFCRibbonInfo::XImage::m_Image offset");

struct RibbonConstructorXRibbonBarView {
    void* m_vfptr;
    int m_ElementType;
    int _pad_0c;
    int m_bToolTip;
    int m_bToolTipDescr;
    int m_bKeyTips;
    int m_bPrintPreview;
    int m_bDrawUsingFont;
    int _pad_24;
    RibbonConstructorXImageView m_Images;
};
static_assert(offsetof(RibbonConstructorXRibbonBarView, m_Images) == 0x28,
              "CMFCRibbonInfo::XRibbonBar::m_Images offset");

struct RibbonConstructorInfoView {
    void* m_vfptr;
    CSize m_sizeImage[2];
    RibbonConstructorXRibbonBarView m_RibbonBar;
};
static_assert(sizeof(CSize) == 0x08,
              "CMFCRibbonInfo image sizes are 8-byte CSize values");
static_assert(offsetof(RibbonConstructorInfoView, m_sizeImage) == 0x08,
              "CMFCRibbonInfo::m_sizeImage offset");
static_assert(offsetof(RibbonConstructorInfoView, m_RibbonBar) == 0x18,
              "CMFCRibbonInfo::m_RibbonBar offset");
static_assert(offsetof(RibbonConstructorInfoView, m_RibbonBar) +
                  offsetof(RibbonConstructorXRibbonBarView, m_Images) +
                  offsetof(RibbonConstructorXImageView, m_Image) == 0x60,
              "CMFCRibbonInfo::XRibbonBar::m_Images::m_Image offset");

struct RibbonConstructorXCategoryView {
    void* m_vfptr;
    int m_ElementType;
    int _pad_0c;
    CString m_strName;
};
static_assert(offsetof(RibbonConstructorXCategoryView, m_strName) == 0x10,
              "CMFCRibbonInfo::XCategory::m_strName offset");

struct RibbonConstructorView {
    void* m_vfptr;
    const void* m_Info;
};
static_assert(offsetof(RibbonConstructorView, m_Info) == 0x08,
              "CMFCRibbonConstructor::m_Info offset");
static_assert(sizeof(RibbonConstructorView) == 0x10,
              "CMFCRibbonConstructor modeled prefix size");
}

extern "C" void MS_ABI impl__SetID_CMFCRibbonBaseElement__UEAAXI_Z(
    CMFCRibbonBaseElement* pElement, unsigned int id);
extern "C" void MS_ABI impl__SetText_CMFCRibbonBaseElement__UEAAXPEB_W_Z(
    CMFCRibbonBaseElement* pElement, const wchar_t* text);
extern "C" void MS_ABI impl__SetToolTipText_CMFCRibbonBaseElement__UEAAXPEB_W_Z(
    CMFCRibbonBaseElement* pElement, const wchar_t* text);
extern "C" void MS_ABI impl__SetDescription_CMFCRibbonBaseElement__UEAAXPEB_W_Z(
    CMFCRibbonBaseElement* pElement, const wchar_t* text);
extern "C" CMFCRibbonCategory* MS_ABI impl__AddCategory_CMFCRibbonBar__QEAAPEAVCMFCRibbonCategory__PEB_WIIVCSize__1HPEAUCRuntimeClass___Z(
    CMFCRibbonBar* pBar, const wchar_t* name, unsigned int smallImage,
    unsigned int largeImage, CSize sizeSmall, unsigned int animLargeImage,
    int insertAt, CRuntimeClass* runtimeClass);
extern "C" CMFCRibbonPanel* MS_ABI impl__AddPanel_CMFCRibbonCategory__QEAAPEAVCMFCRibbonPanel__PEB_WPEAUHICON____PEAUCRuntimeClass___Z(
    CMFCRibbonCategory* pCategory, const wchar_t* label, HICON__* icon,
    CRuntimeClass* runtimeClass);
extern "C" HICON MS_ABI impl__ExtractIconW_CMFCToolBarImages__QEAAPEAUHICON____H_Z(
    CMFCToolBarImages* pImages, int index);

// Retail ctor writes m_Info at +0x08 and installs its own vfptr (RVA 0xf80a0, mfc140u).
// OpenMFC has no relocatable constructor vftable, so keep the reference and clear the unusable vfptr.
// Symbol: ??0CMFCRibbonConstructor@@QEAA@AEBVCMFCRibbonInfo@@@Z
extern "C" void* MS_ABI impl___0CMFCRibbonConstructor__QEAA_AEBVCMFCRibbonInfo___Z(
    void* pThis, const void* info) {
    auto* layout = static_cast<RibbonConstructorView*>(pThis);
    layout->m_vfptr = nullptr;
    layout->m_Info = info;
    return pThis;
}

// Stub: RVA 0xf80f0 (mfc140u) only rewrites +0 with the class retail vfptr; OpenMFC declares no CMFCRibbonConstructor type or relocatable vtable.
// Symbol: ??1CMFCRibbonConstructor@@UEAA@XZ
extern "C" void MS_ABI impl___1CMFCRibbonConstructor__UEAA_XZ(void* pThis) {
    (void)pThis;
}

// Partial transcription of RVA 0xf8c50 (mfc140u): copy the XID, text, tooltip,
// and description through OpenMFC's supported setters. Retail also sets key
// strings, flags at +0x164/+0x1ec/+0x21c, image indices at +0x1b8/+0x1bc,
// child metadata at +0x16c, and child links; those states are not represented
// by the compact destination model.
// Set the ID first because OpenMFC stores tooltip/description state by ID.
// Symbol: ?ConstructBaseElement@CMFCRibbonConstructor@@MEBAXAEAVCMFCRibbonBaseElement@@AEBVXElement@CMFCRibbonInfo@@@Z
extern "C" void MS_ABI impl__ConstructBaseElement_CMFCRibbonConstructor__MEBAXAEAVCMFCRibbonBaseElement__AEBVXElement_CMFCRibbonInfo___Z(
    void* pThis, void* pBase, const void* pInfo) {
    (void)pThis;
    if (pBase == nullptr || pInfo == nullptr) return;

    auto* element = static_cast<CMFCRibbonBaseElement*>(pBase);
    const auto* source = static_cast<const RibbonConstructorXElementView*>(pInfo);
    impl__SetID_CMFCRibbonBaseElement__UEAAXI_Z(element, source->m_ID.m_Value);
    impl__SetText_CMFCRibbonBaseElement__UEAAXPEB_W_Z(
        element, static_cast<const wchar_t*>(source->m_strText));
    impl__SetToolTipText_CMFCRibbonBaseElement__UEAAXPEB_W_Z(
        element, static_cast<const wchar_t*>(source->m_strToolTip));
    impl__SetDescription_CMFCRibbonBaseElement__UEAAXPEB_W_Z(
        element, static_cast<const wchar_t*>(source->m_strDescription));
}

// Stub: RVA 0xf8610 (mfc140u) calls a helper on two input/target subobjects and walks category child arrays through virtual calls. RI_XCategory is pinned privately in CMFCRibbonInfo.cpp but not shared here, and the destination retail offsets/slots are absent from the compact category model.
// Symbol: ?ConstructCategory@CMFCRibbonConstructor@@MEBAXAEAVCMFCRibbonCategory@@AEBVXCategory@CMFCRibbonInfo@@@Z
extern "C" void MS_ABI impl__ConstructCategory_CMFCRibbonConstructor__MEBAXAEAVCMFCRibbonCategory__AEBVXCategory_CMFCRibbonInfo___Z(
    void* pThis, void* pCategory, const void* pInfo) {
    (void)pThis;
    (void)pCategory;
    (void)pInfo;
}

// Stub: RVA 0xf8500 (mfc140u) walks category-main elements and invokes main-panel virtuals. RI_XCategoryMain is pinned privately in CMFCRibbonInfo.cpp but not shared here, and the required destination main-panel state/slots are absent from the compact model.
// Symbol: ?ConstructCategoryMain@CMFCRibbonConstructor@@MEBAXAEAVCMFCRibbonBar@@AEBVXCategoryMain@CMFCRibbonInfo@@@Z
extern "C" void MS_ABI impl__ConstructCategoryMain_CMFCRibbonConstructor__MEBAXAEAVCMFCRibbonBar__AEBVXCategoryMain_CMFCRibbonInfo___Z(
    void* pThis, void* pBar, const void* pInfo) {
    (void)pThis;
    (void)pBar;
    (void)pInfo;
}

// Stub: RVA 0xf8a60 (mfc140u) branches on XElement type and populates type-specific elements using retail virtual slots and member offsets absent from the OpenMFC element models.
// Symbol: ?ConstructElement@CMFCRibbonConstructor@@MEBAXAEAVCMFCRibbonBaseElement@@AEBVXElement@CMFCRibbonInfo@@@Z
extern "C" void MS_ABI impl__ConstructElement_CMFCRibbonConstructor__MEBAXAEAVCMFCRibbonBaseElement__AEBVXElement_CMFCRibbonInfo___Z(
    void* pThis, void* pElement, const void* pInfo) {
    (void)pThis;
    (void)pElement;
    (void)pInfo;
}

// Stub: RVA 0xf8750 (mfc140u) copies panel state and iterates XPanel elements, dispatching through panel and element virtual slots. RI_XPanel is pinned privately in CMFCRibbonInfo.cpp but not shared here; the compact destination panel model lacks the required retail offsets/slots.
// Symbol: ?ConstructPanel@CMFCRibbonConstructor@@MEBAXAEAVCMFCRibbonPanel@@AEBVXPanel@CMFCRibbonInfo@@@Z
extern "C" void MS_ABI impl__ConstructPanel_CMFCRibbonConstructor__MEBAXAEAVCMFCRibbonPanel__AEBVXPanel_CMFCRibbonInfo___Z(
    void* pThis, void* pPanel, const void* pInfo) {
    (void)pThis;
    (void)pPanel;
    (void)pInfo;
}

// Stub: RVA 0xf8880 (mfc140u) builds temporary lookup data from the input, calls private helpers and copies info +0x220 to bar +0x434. RI_XRibbonBar/RI_XQAT are pinned privately in CMFCRibbonInfo.cpp but not shared here; the matching destination QAT state is not modeled by the compact bar API.
// Symbol: ?ConstructQATElements@CMFCRibbonConstructor@@MEBAXAEAVCMFCRibbonBar@@AEBVXRibbonBar@CMFCRibbonInfo@@@Z
extern "C" void MS_ABI impl__ConstructQATElements_CMFCRibbonConstructor__MEBAXAEAVCMFCRibbonBar__AEBVXRibbonBar_CMFCRibbonInfo___Z(
    void* pThis, void* pBar, const void* pInfo) {
    (void)pThis;
    (void)pBar;
    (void)pInfo;
}

// Stub: RVA 0xf82d0 (mfc140u) performs a long sequence of ribbon-bar field writes, global-state updates and virtual calls; the needed retail state/layouts are not modeled in this file.
// Symbol: ?ConstructRibbonBar@CMFCRibbonConstructor@@UEBAXAEAVCMFCRibbonBar@@@Z
extern "C" void MS_ABI impl__ConstructRibbonBar_CMFCRibbonConstructor__UEBAXAEAVCMFCRibbonBar___Z(
    void* pThis, void* pBar) {
    (void)pThis;
    (void)pBar;
}

// Stub: RVA 0xf8990 (mfc140u) iterates tab-element arrays and dispatches through constructor/bar virtual slots. RI_XRibbonBar's array offsets are pinned privately in CMFCRibbonInfo.cpp but not shared here; the destination insertion state/slots are absent from the compact bar model.
// Symbol: ?ConstructTabElements@CMFCRibbonConstructor@@MEBAXAEAVCMFCRibbonBar@@AEBVXRibbonBar@CMFCRibbonInfo@@@Z
extern "C" void MS_ABI impl__ConstructTabElements_CMFCRibbonConstructor__MEBAXAEAVCMFCRibbonBar__AEBVXRibbonBar_CMFCRibbonInfo___Z(
    void* pThis, void* pBar, const void* pInfo) {
    (void)pThis;
    (void)pBar;
    (void)pInfo;
}

// Stub: RVA 0xf8230 (mfc140u) allocates and initializes a 0x5a0-byte application-button object, installs its retail vfptr and links it to the bar. CMFCRibbonApplicationButton.cpp pins the size and selected offsets, but this TU has no complete layout or safe placement-construction/vfptr path.
// Symbol: ?CreateApplicationButton@CMFCRibbonConstructor@@MEBAPEAVCMFCRibbonApplicationButton@@AEAVCMFCRibbonBar@@@Z
extern "C" void* MS_ABI impl__CreateApplicationButton_CMFCRibbonConstructor__MEBAPEAVCMFCRibbonApplicationButton__AEAVCMFCRibbonBar___Z(
    void* pThis, void* pBar) {
    (void)pThis;
    (void)pBar;
    return nullptr;
}

// RVA 0xf8150 (mfc140u) forwards the XCategory name and CMFCRibbonInfo image
// sizes to CMFCRibbonBar::AddCategory (entry RVA 0xda450, mfc140u). The
// The current impl__AddCategory body creates/registers an ordinary side-table
// category, but its C prototype takes one CSize and ignores image metadata, so
// this is intentionally partial. Its export mangling describes two CSize
// values; the internal call below deliberately matches the existing body
// prototype, and the mismatch must be corrected in CMFCRibbonBar.cpp.
// Symbol: ?CreateCategory@CMFCRibbonConstructor@@MEBAPEAVCMFCRibbonCategory@@AEAVCMFCRibbonBar@@AEBVXCategory@CMFCRibbonInfo@@@Z
extern "C" void* MS_ABI impl__CreateCategory_CMFCRibbonConstructor__MEBAPEAVCMFCRibbonCategory__AEAVCMFCRibbonBar__AEBVXCategory_CMFCRibbonInfo___Z(
    void* pThis, void* pBar, const void* pInfo) {
    if (pThis == nullptr || pBar == nullptr || pInfo == nullptr) return nullptr;

    const auto* constructor = static_cast<const RibbonConstructorView*>(pThis);
    const auto* info = static_cast<const RibbonConstructorInfoView*>(constructor->m_Info);
    const auto* category = static_cast<const RibbonConstructorXCategoryView*>(pInfo);
    if (info == nullptr) return nullptr;

    return impl__AddCategory_CMFCRibbonBar__QEAAPEAVCMFCRibbonCategory__PEB_WIIVCSize__1HPEAUCRuntimeClass___Z(
        static_cast<CMFCRibbonBar*>(pBar),
        static_cast<const wchar_t*>(category->m_strName), 0, 0,
        info->m_sizeImage[0], 0, -1, nullptr);
}

// Stub: RVA 0xf81a0 (mfc140u) forwards to CMFCRibbonBar::AddContextCategory (entry RVA 0xda650, mfc140u). That OpenMFC thunk exists but deliberately returns nullptr because context IDs, category visibility, and context-caption state are not modeled; making an ordinary tab instead would be incorrect.
// Symbol: ?CreateCategoryContext@CMFCRibbonConstructor@@MEBAPEAVCMFCRibbonCategory@@AEAVCMFCRibbonBar@@AEBVXContext@CMFCRibbonInfo@@AEBVXCategory@5@@Z
extern "C" void* MS_ABI impl__CreateCategoryContext_CMFCRibbonConstructor__MEBAPEAVCMFCRibbonCategory__AEAVCMFCRibbonBar__AEBVXContext_CMFCRibbonInfo__AEBVXCategory_5__Z(
    void* pThis, void* pBar, const void* pContext, const void* pCategoryList) {
    (void)pThis;
    (void)pBar;
    (void)pContext;
    (void)pCategoryList;
    return nullptr;
}

// Stub: RVA 0xf81f0 (mfc140u) forwards to CMFCRibbonBar::AddMainCategory (entry RVA 0xda310, mfc140u). Its OpenMFC thunk exists but returns nullptr because neither the main-category state nor CMFCRibbonMainPanel is modeled.
// Symbol: ?CreateCategoryMain@CMFCRibbonConstructor@@MEBAPEAVCMFCRibbonMainPanel@@AEAVCMFCRibbonBar@@AEBVXCategoryMain@CMFCRibbonInfo@@@Z
extern "C" void* MS_ABI impl__CreateCategoryMain_CMFCRibbonConstructor__MEBAPEAVCMFCRibbonMainPanel__AEAVCMFCRibbonBar__AEBVXCategoryMain_CMFCRibbonInfo___Z(
    void* pThis, void* pBar, const void* pCategoryMainInfo) {
    (void)pThis;
    (void)pBar;
    (void)pCategoryMainInfo;
    return nullptr;
}

// Stub: RVA 0xf8e30 (mfc140u) dispatches on XElement type and allocates/initializes many concrete element classes; the required type-specific retail object layouts are not modeled.
// Symbol: ?CreateElement@CMFCRibbonConstructor@@MEBAPEAVCMFCRibbonBaseElement@@AEBVXElement@CMFCRibbonInfo@@@Z
extern "C" void* MS_ABI impl__CreateElement_CMFCRibbonConstructor__MEBAPEAVCMFCRibbonBaseElement__AEBVXElement_CMFCRibbonInfo___Z(
    void* pThis, const void* pInfo) {
    (void)pThis;
    (void)pInfo;
    return nullptr;
}

// Partial transcription of RVA 0xf8100 (mfc140u): the retail call at 0xf8123
// extracts an icon with CMFCToolBarImages::ExtractIconW (entry RVA 0x16e6d0,
// mfc140u), then tail-calls CMFCRibbonCategory::AddPanel (entry RVA 0xed860,
// mfc140u). OpenMFC's ExtractIconW currently returns nullptr, but AddPanel
// still creates/registers a named panel; panel flags and the retail icon are
// therefore not reproduced.
// Symbol: ?CreatePanel@CMFCRibbonConstructor@@MEBAPEAVCMFCRibbonPanel@@AEAVCMFCRibbonCategory@@AEBVXPanel@CMFCRibbonInfo@@@Z
extern "C" void* MS_ABI impl__CreatePanel_CMFCRibbonConstructor__MEBAPEAVCMFCRibbonPanel__AEAVCMFCRibbonCategory__AEBVXPanel_CMFCRibbonInfo___Z(
    void* pThis, void* pCategory, const void* pInfo) {
    if (pThis == nullptr || pCategory == nullptr || pInfo == nullptr) return nullptr;

    const auto* constructor = static_cast<const RibbonConstructorView*>(pThis);
    const auto* info = static_cast<const RibbonConstructorInfoView*>(constructor->m_Info);
    const auto* panel = static_cast<const RibbonConstructorXPanelView*>(pInfo);
    if (info == nullptr) return nullptr;

    HICON icon = nullptr;
    if (panel->m_nImageIndex != -1) {
        icon = impl__ExtractIconW_CMFCToolBarImages__QEAAPEAUHICON____H_Z(
            const_cast<CMFCToolBarImages*>(&info->m_RibbonBar.m_Images.m_Image),
            panel->m_nImageIndex);
    }
    return impl__AddPanel_CMFCRibbonCategory__QEAAPEAVCMFCRibbonPanel__PEB_WPEAUHICON____PEAUCRuntimeClass___Z(
        static_cast<CMFCRibbonCategory*>(pCategory),
        static_cast<const wchar_t*>(panel->m_strName), icon, nullptr);
}

// Stub: the mfc140u symbol map does not resolve this name. The ANSI twin's
// entry at RVA 0xf9750 (mfc140) updates button fields +0x1b8/+0x1bc,
// +0x1d8/+0x1e0 and +0x200/+0x204 and manages image state through helpers.
// The compact button/image model does not expose that state.
// Symbol: ?SetIcon@CMFCRibbonConstructor@@MEBAXAEAVCMFCRibbonButton@@W4RibbonImageType@CMFCRibbonBaseElement@@AEBVCMFCToolBarImages@@H@Z
extern "C" void MS_ABI impl__SetIcon_CMFCRibbonConstructor__MEBAXAEAVCMFCRibbonButton__W4RibbonImageType_CMFCRibbonBaseElement__AEBVCMFCToolBarImages__H_Z(
    void* pThis, void* pButton, int imageType, const void* pImages, int index) {
    (void)pThis;
    (void)pButton;
    (void)imageType;
    (void)pImages;
    (void)index;
}

// The mfc140 ANSI-twin body tail-dispatches to the destination element's SetID slot with XID::m_Value (+0x08).
// OpenMFC stores the ID in its compact element model, so use its verified SetID thunk rather than writing retail +0x118.
// Symbol: ?SetID@CMFCRibbonConstructor@@MEBAXAEAVCMFCRibbonBaseElement@@AEBVXID@CMFCRibbonInfo@@@Z
extern "C" void MS_ABI impl__SetID_CMFCRibbonConstructor__MEBAXAEAVCMFCRibbonBaseElement__AEBVXID_CMFCRibbonInfo___Z(
    void* pThis, void* pElement, const void* id) {
    (void)pThis;
    const auto* xid = static_cast<const RibbonConstructorXIDView*>(id);
    impl__SetID_CMFCRibbonBaseElement__UEAAXI_Z(
        static_cast<CMFCRibbonBaseElement*>(pElement), xid->m_Value);
}
