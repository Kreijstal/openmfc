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
    std::int32_t m_nIconsInRow;             // inherited gallery field +0x4c8
    std::uint8_t _toActionNumber[0x4f8 - 0x4cc];
    std::int32_t m_nActionNumber;           // +0x4f8
    std::uint32_t m_sizeMaxText[2];         // CSize at +0x4fc
    std::uint32_t _padding504;
    void* m_strCancel;                      // CString at +0x508
    void* m_strUndoOne;                     // CString at +0x510
    void* m_strUndoFmt;                     // CString at +0x518
    std::uintptr_t m_arLabels[5];           // CStringArray at +0x520
};
static_assert(offsetof(CMFCRibbonUndoButtonShadow, m_nIconsInRow) == 0x4c8);
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
} // namespace

extern "C" void MS_ABI impl__OnClick_CMFCRibbonButton__UEAAXVCPoint___Z(
    void* pThis, long long point);
extern "C" int MS_ABI impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(
    const CObject* pThis, const CRuntimeClass* pClass);
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMFCRibbonButton__SAPEAUCRuntimeClass__XZ();

// Symbol: ??0CMFCRibbonUndoButton@@QEAA@IPEB_WHH@Z
extern "C" void* MS_ABI impl___0CMFCRibbonUndoButton__QEAA_IPEB_WHH_Z(
    void* pThis, int id, const wchar_t* text, int width, int height) {
    (void)id;
    (void)text;
    (void)width;
    (void)height;
    return pThis;
}
// Symbol: ??0CMFCRibbonUndoButton@@QEAA@IPEB_WPEAUHICON__@@@Z
extern "C" void* MS_ABI impl___0CMFCRibbonUndoButton__QEAA_IPEB_WPEAUHICON_____Z(
    void* pThis, int id, const wchar_t* text, void* icon) {
    (void)id;
    (void)text;
    (void)icon;
    return pThis;
}
// Symbol: ??0CMFCRibbonUndoButton@@QEAA@XZ
extern "C" void* MS_ABI impl___0CMFCRibbonUndoButton__QEAA_XZ(void* pThis) {
    return pThis;
}
// Stub: retail at RVA 0x128b90 (mfc140u) releases the label array and three
// CString members, then tail-calls the CMFCRibbonGallery destructor. A base
// destructor thunk exists, but this file's constructors only return pThis and
// CommonInit is still stubbed, so releasing the uninitialized CString fields
// here would be unsafe.
// Symbol: ??1CMFCRibbonUndoButton@@UEAA@XZ
extern "C" void MS_ABI impl___1CMFCRibbonUndoButton__UEAA_XZ(void* pThis) {
    (void)pThis;
}

// Stub: retail at RVA 0x128c40 (mfc140u) calls vtable slot 0x510 (slot 162),
// appends to m_arLabels (+0x520), copies its count to +0x4c8, and clears the
// cached CSize at +0x4fc. The required ribbon vtable dispatch is not modeled.
// Symbol: ?AddUndoAction@CMFCRibbonUndoButton@@QEAAXPEB_W@Z
extern "C" void MS_ABI impl__AddUndoAction_CMFCRibbonUndoButton__QEAAXPEB_W_Z(
    void* pThis, const wchar_t* lpszLabel) {
    (void)pThis;
    (void)lpszLabel;
}

// Stub: retail at RVA 0x128ca0 (mfc140u) calls vtable slot 0x510 (slot 162),
// clears m_arLabels via SetSize(0, -1), and zeroes the CSize at +0x4fc. The
// ribbon vtable dispatch required by the first call is not modeled.
// Symbol: ?CleanUpUndoList@CMFCRibbonUndoButton@@QEAAXXZ
extern "C" void MS_ABI impl__CleanUpUndoList_CMFCRibbonUndoButton__QEAAXXZ(
    void* pThis) {
    (void)pThis;
}

// Stub: retail at RVA 0x128a40 (mfc140u) initializes fields at +0x4f8/+0x4fc,
// loads three resource strings, and creates/configures an internal ribbon
// button using additional helper calls. Those resource and object operations
// are not available through a modeled CMFCRibbonUndoButton vtable here.
// Symbol: ?CommonInit@CMFCRibbonUndoButton@@IEAAXXZ
extern "C" void MS_ABI impl__CommonInit_CMFCRibbonUndoButton__IEAAXXZ(
    void* pThis) {
    (void)pThis;
}

// Stub: retail at RVA 0x128e00 (mfc140u) first calls Gallery::CopyFrom, then
// copies the action index, labels, gallery icon count and cached CSize. The
// existing Gallery::CopyFrom impl__ definition has a placeholder, ABI-wrong
// parameter list, so it cannot safely be called from this thunk.
// Symbol: ?CopyFrom@CMFCRibbonUndoButton@@MEAAXAEBVCMFCRibbonBaseElement@@@Z
extern "C" void MS_ABI impl__CopyFrom_CMFCRibbonUndoButton__MEAAXAEBVCMFCRibbonBaseElement___Z(
    void* pThis, const void* pSource) {
    (void)pThis;
    (void)pSource;
}

// Stub: retail at RVA 0x1286a0 (mfc140u) allocates 0x548 bytes and calls the
// default constructor at RVA 0x1286e0 (mfc140u). The local default-constructor
// thunk only returns pThis and does not establish a ribbon vtable, so returning
// a partially constructed object would be unsafe.
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

// Stub: the Unicode export did not resolve in the supplied map. Its ANSI twin
// disassembly shows menu sizing from m_arLabels and cached text sizes followed
// by the button popup path; that UI/layout pipeline is not modeled here.
// Symbol: ?OnShowPopupMenu@CMFCRibbonUndoButton@@MEAAXXZ
extern "C" void MS_ABI impl__OnShowPopupMenu_CMFCRibbonUndoButton__MEAAXXZ(
    void* pThis) {
    (void)pThis;
}

