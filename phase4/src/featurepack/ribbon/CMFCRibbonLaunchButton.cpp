// CMFCRibbonLaunchButton — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp

#include "detail/ManualSmallStubImplementationsSupport.h"

#include <cstddef>
#include <cstdint>
#include <cstring>

namespace {

// CMFCRibbonLaunchButton has no clean-room class declaration. These opaque
// fields pin only offsets observed in retail; no C++ member type is assumed.
struct S_CMFCRibbonLaunchButtonLayout {
    std::uint8_t beforeRect[0xc8];
    std::int32_t rect[4];                         // +0xc8
    std::uint8_t beforeParentMenu[0x20];
    void* field_0xf8;                             // +0xf8
    std::uint8_t beforeParentPanel[0x170];
    void* m_pParentPanel;                         // +0x270
};
static_assert(offsetof(S_CMFCRibbonLaunchButtonLayout, rect) == 0xc8,
              "retail CMFCRibbonLaunchButton rect offset");
static_assert(offsetof(S_CMFCRibbonLaunchButtonLayout, field_0xf8) == 0xf8,
              "retail CMFCRibbonLaunchButton field +0xf8");
static_assert(offsetof(S_CMFCRibbonLaunchButtonLayout, m_pParentPanel) == 0x270,
              "retail CMFCRibbonLaunchButton::m_pParentPanel offset");
static_assert(sizeof(S_CMFCRibbonLaunchButtonLayout) == 0x278,
              "retail CMFCRibbonLaunchButton size");

struct S_RibbonPanelLaunchButtonField {
    std::uint8_t beforeField[0x114];
    std::int32_t field_0x114;
};
static_assert(offsetof(S_RibbonPanelLaunchButtonField, field_0x114) == 0x114,
              "retail launch-button panel field offset");

} // namespace

// Verified definitions in CMFCRibbonButton.cpp. Calls use impl__ thunks rather
// than C++ member expressions, which have no internal method symbol here.
extern "C" void MS_ABI impl__GetKeyTipRect_CMFCRibbonButton__MEAA_AVCRect__PEAVCDC__H_Z(
    CMFCRibbonButton* pThis, void* pResult, CDC* pDC, int bIsMenu);
extern "C" void MS_ABI impl__OnClick_CMFCRibbonButton__UEAAXVCPoint___Z(
    CMFCRibbonButton* pThis, long long point);
extern "C" void MS_ABI impl__GetRegularSize_CMFCRibbonButton__UEAA_AVCSize__PEAVCDC___Z(
    void* pResult, const CMFCRibbonButton* pThis, CDC* pDC);
extern "C" void MS_ABI impl__OnDraw_CMFCRibbonButton__UEAAXPEAVCDC___Z(
    CMFCRibbonButton* pThis, CDC* pDC);
extern "C" int MS_ABI impl__SetACCData_CMFCRibbonButton__UEAAHPEAVCWnd__AEAVCAccessibilityData___Z(
    CMFCRibbonButton* pThis, CWnd* pWnd, CAccessibilityData& data);

// Symbol: ??0CMFCRibbonLaunchButton@@QEAA@XZ
extern "C" void* MS_ABI impl___0CMFCRibbonLaunchButton__QEAA_XZ(void* pThis) {
    return pThis;
}
// Retail mfc140u RVA 0x1153e0 allocates 0x278 bytes and invokes the constructor
// at RVA 0x115420. The existing constructor thunk in this file only returns
// pThis; it does not initialize the base or install the launch-button vftable.
// Returning that allocation would therefore produce an invalid MFC object.
// Symbol: ?CreateObject@CMFCRibbonLaunchButton@@SAPEAVCObject@@XZ
extern "C" void* MS_ABI impl__CreateObject_CMFCRibbonLaunchButton__SAPEAVCObject__XZ() {
    return nullptr;
}

// Retail mfc140u RVA 0x115520 uses m_pParentPanel at +0x270. With a panel it
// obtains a CSize through vtable byte offset +0x3b8 (slot 119), then returns
// zero if that size is empty or the button rect at +0xc8 is empty. Otherwise
// the output is { panel field +0x114 - size.cx, rect.bottom,
// panel field +0x114, rect.bottom + size.cy }. With no panel retail delegates
// to CMFCRibbonButton::GetKeyTipRect; that in-repo partial fallback writes
// an empty CRect, so this path remains conservative rather than retail-complete.
// The IsRectEmpty call in retail resolves to USER32!IsRectEmpty; the equivalent
// bounds test below avoids introducing a new import.
// Symbol: ?GetKeyTipRect@CMFCRibbonLaunchButton@@UEAA?AVCRect@@PEAVCDC@@H@Z
extern "C" void* MS_ABI impl__GetKeyTipRect_CMFCRibbonLaunchButton__UEAA_AVCRect__PEAVCDC__H_Z(
    void* pThis, void* pResult, CDC* pDC, int bIsMenu) {
    if (!pResult) return nullptr;
    auto* out = static_cast<std::int32_t*>(pResult);
    std::memset(out, 0, sizeof(std::int32_t) * 4);
    if (!pThis) return pResult;

    auto* self = static_cast<S_CMFCRibbonLaunchButtonLayout*>(pThis);
    if (!self->m_pParentPanel) {
        impl__GetKeyTipRect_CMFCRibbonButton__MEAA_AVCRect__PEAVCDC__H_Z(
            reinterpret_cast<CMFCRibbonButton*>(pThis), pResult, pDC, bIsMenu);
        return pResult;
    }

    std::int32_t size[2] = {};
    using GetKeyTipSizeFn = void (MS_ABI *)(void*, std::int32_t*);
    auto** vtable = *reinterpret_cast<void***>(pThis);
    auto getKeyTipSize = reinterpret_cast<GetKeyTipSizeFn>(vtable[0x3b8 / sizeof(void*)]);
    getKeyTipSize(pThis, size);
    if ((size[0] == 0 && size[1] == 0) ||
        self->rect[2] <= self->rect[0] || self->rect[3] <= self->rect[1]) {
        return pResult;
    }

    const auto* panel = static_cast<const S_RibbonPanelLaunchButtonField*>(self->m_pParentPanel);
    const auto panelField = panel->field_0x114;
    out[0] = panelField - size[0];
    out[1] = self->rect[3];
    out[2] = panelField;
    out[3] = self->rect[3] + size[1];
    return pResult;
}

// Retail mfc140u RVA 0x1154c0 writes a zero CSize when m_pParentPanel is set;
// otherwise it calls CMFCRibbonButton::GetRegularSize. OpenMFC's base thunk
// declares the result buffer before this, unlike the retail member ABI, so the
// arguments are deliberately reordered at this call site. The base sizing
// routine is an existing approximation, not a retail-exact implementation.
// Symbol: ?GetRegularSize@CMFCRibbonLaunchButton@@UEAA?AVCSize@@PEAVCDC@@@Z
extern "C" void* MS_ABI impl__GetRegularSize_CMFCRibbonLaunchButton__UEAA_AVCSize__PEAVCDC___Z(
    void* pThis, void* pResult, CDC* pDC) {
    if (!pResult) return nullptr;
    if (!pThis) {
        std::memset(pResult, 0, sizeof(std::int32_t) * 2);
        return pResult;
    }

    auto* self = static_cast<S_CMFCRibbonLaunchButtonLayout*>(pThis);
    if (self->m_pParentPanel) {
        std::memset(pResult, 0, sizeof(std::int32_t) * 2);
    } else {
        impl__GetRegularSize_CMFCRibbonButton__UEAA_AVCSize__PEAVCDC___Z(
            pResult, reinterpret_cast<const CMFCRibbonButton*>(pThis), pDC);
    }
    return pResult;
}

// Retail mfc140u RVA 0x1154f0 reads the pointer at +0xf8. If set, it
// tail-dispatches vtable byte offset +0x8b0 (slot 278) with (that object, this,
// CPoint). If absent, it calls the inherited OnClick with a zero point.
// The base thunk is currently a no-op in OpenMFC, so only that fallback path
// remains conservative; the parent-menu virtual dispatch is transcribed.
// Symbol: ?OnClick@CMFCRibbonLaunchButton@@UEAAXVCPoint@@@Z
extern "C" void MS_ABI impl__OnClick_CMFCRibbonLaunchButton__UEAAXVCPoint___Z(
    void* pThis, long long point) {
    if (!pThis) return;
    auto* self = static_cast<S_CMFCRibbonLaunchButtonLayout*>(pThis);
    if (self->field_0xf8) {
        using ClickDispatchFn = void (MS_ABI *)(void*, void*, long long);
        auto** vtable = *reinterpret_cast<void***>(self->field_0xf8);
        auto onClick = reinterpret_cast<ClickDispatchFn>(vtable[0x8b0 / sizeof(void*)]);
        onClick(self->field_0xf8, pThis, point);
        return;
    }
    impl__OnClick_CMFCRibbonButton__UEAAXVCPoint___Z(
        reinterpret_cast<CMFCRibbonButton*>(pThis), 0);
}

// Retail mfc140u RVA 0x115450 ignores an empty button rect, dispatches to
// visual-manager vtable byte offset +0x460 (slot 140) when m_pParentPanel is
// present, and otherwise calls CMFCRibbonButton::OnDraw. The no-panel path
// delegates to OpenMFC's existing (approximate) base painter. The panel path
// needs retail's private singleton helper at RVA 0x9774 (mfc140u); no callable impl__
// thunk for that helper is available here, and a direct C++ method reference
// fails this file's link audit, so that path remains unavailable. The current
// OpenMFC launch-button renderer thunks also do not paint the glyph.
// Symbol: ?OnDraw@CMFCRibbonLaunchButton@@UEAAXPEAVCDC@@@Z
extern "C" void MS_ABI impl__OnDraw_CMFCRibbonLaunchButton__UEAAXPEAVCDC___Z(
    void* pThis, CDC* pDC) {
    if (!pThis || !pDC) return;

    auto* self = static_cast<S_CMFCRibbonLaunchButtonLayout*>(pThis);
    if (self->rect[2] <= self->rect[0] || self->rect[3] <= self->rect[1]) return;
    if (self->m_pParentPanel) return;

    impl__OnDraw_CMFCRibbonButton__UEAAXPEAVCDC___Z(
        reinterpret_cast<CMFCRibbonButton*>(pThis), pDC);
}

// The ANSI twin's body at RVA 0x116370 (mfc140) calls the button
// accessibility implementation and, on success, sets bit 30 of data state
// at +0x34. No mfc140u RVA is resolved for this export; this transcribes the
// ANSI behavior without treating its RVA as a Unicode address. The delegated
// OpenMFC button implementation remains a return-value approximation.
// Symbol: ?SetACCData@CMFCRibbonLaunchButton@@UEAAHPEAVCWnd@@AEAVCAccessibilityData@@@Z
extern "C" int MS_ABI impl__SetACCData_CMFCRibbonLaunchButton__UEAAHPEAVCWnd__AEAVCAccessibilityData___Z(
    void* pThis, CWnd* pWnd, CAccessibilityData* pData) {
    if (!pThis || !pData) return FALSE;
    if (!impl__SetACCData_CMFCRibbonButton__UEAAHPEAVCWnd__AEAVCAccessibilityData___Z(
            reinterpret_cast<CMFCRibbonButton*>(pThis), pWnd, *pData)) {
        return FALSE;
    }
    pData->m_bAccState |= 0x40000000L;
    return TRUE;
}

