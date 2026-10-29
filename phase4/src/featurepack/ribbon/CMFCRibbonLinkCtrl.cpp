// CMFCRibbonLinkCtrl — OpenMFC implementation.
// Sources: retail mfc140/mfc140u disassembly; manual_small_stub_implementations.cpp.

#include "detail/ManualSmallStubImplementationsSupport.h"

#include <cstddef>
#include <cstdint>

extern "C" int MS_ABI impl__SetACCData_CMFCRibbonButton__UEAAHPEAVCWnd__AEAVCAccessibilityData___Z(
    CMFCRibbonButton* pThis, CWnd* pWnd, CAccessibilityData& data);
extern "C" void MS_ABI impl__SetString___CSimpleStringT__W_00_ATL__QEAAXPEB_W_Z(
    void* pThis, const wchar_t* text);

namespace {
// CMFCRibbonLinkCtrl is not declared in include/openmfc. Retail's Unicode
// default constructor stores m_strLink at +0x270 (RVA 0x10ec30, mfc140u),
// and CreateObject allocates 0x278 bytes (RVA 0x10ebf0, mfc140u).
struct S_CMFCRibbonLinkCtrlLayout {
    std::uint8_t inheritedButtonStorage[0x270];
    alignas(8) std::uint8_t m_strLink[sizeof(CString)];
};
static_assert(sizeof(CString) == 8, "CString storage size used by retail CMFCRibbonLinkCtrl");
static_assert(offsetof(S_CMFCRibbonLinkCtrlLayout, m_strLink) == 0x270,
              "retail CMFCRibbonLinkCtrl::m_strLink offset");
static_assert(sizeof(S_CMFCRibbonLinkCtrlLayout) == 0x278,
              "retail CMFCRibbonLinkCtrl size");
} // namespace

// Stub: OpenMFC has no CMFCRibbonLinkCtrl class/vtable to construct. Retail's
// constructors initialize the CMFCRibbonButton base and CString at +0x270
// (RVA 0x10ec30 and 0x10ecd0, mfc140u); keep the ABI return without touching
// storage whose lifetime/layout is not modeled here.
// Symbol: ??0CMFCRibbonLinkCtrl@@QEAA@IPEB_W0@Z
extern "C" void* MS_ABI impl___0CMFCRibbonLinkCtrl__QEAA_IPEB_W0_Z(
    void* pThis, int id, const wchar_t* text, const wchar_t* link) {
    (void)id;
    (void)text;
    (void)link;
    return pThis;
}
// Symbol: ??0CMFCRibbonLinkCtrl@@QEAA@XZ
extern "C" void* MS_ABI impl___0CMFCRibbonLinkCtrl__QEAA_XZ(void* pThis) {
    return pThis;
}

// Retail mfc140u RVA 0x10ed60 releases the CString storage at +0x270 and then
// tail-calls the inherited destructor path. The current constructor thunks in
// this file do not initialize the CString or the base, so cleanup is unsafe.
// Symbol: ??1CMFCRibbonLinkCtrl@@UEAA@XZ
extern "C" void MS_ABI impl___1CMFCRibbonLinkCtrl__UEAA_XZ(void* pThis) {
    (void)pThis;
}

// Retail mfc140u RVA 0x10f050 calls CMFCRibbonButton::CopyFrom (RVA 0xe7aa0 (mfc140u))
// and assigns the source CString at +0x270; both CString lifetimes are absent.
// Symbol: ?CopyFrom@CMFCRibbonLinkCtrl@@UEAAXAEBVCMFCRibbonBaseElement@@@Z
extern "C" void MS_ABI impl__CopyFrom_CMFCRibbonLinkCtrl__UEAAXAEBVCMFCRibbonBaseElement___Z(
    void* pThis, const CMFCRibbonBaseElement* pSource) {
    (void)pThis;
    (void)pSource;
}

// Retail mfc140u RVA 0x10ebf0 allocates 0x278 bytes and calls the default
// constructor at RVA 0x10ec30. That constructor is currently a no-op here.
// Symbol: ?CreateObject@CMFCRibbonLinkCtrl@@SAPEAVCObject@@XZ
extern "C" void* MS_ABI impl__CreateObject_CMFCRibbonLinkCtrl__SAPEAVCObject__XZ() {
    return nullptr;
}

// The mfc140u map has no resolved entry; the ANSI twin at RVA 0xe160 (mfc140.dll) dispatches
// through vtable byte offset +0x200 (slot 64), which OpenMFC does not model.
// Symbol: ?GetCompactSize@CMFCRibbonLinkCtrl@@UEAA?AVCSize@@PEAVCDC@@@Z
extern "C" void* MS_ABI impl__GetCompactSize_CMFCRibbonLinkCtrl__UEAA_AVCSize__PEAVCDC___Z(
    void* pThis, void* pResult, CDC* pDC) {
    (void)pThis;
    (void)pDC;
    // Keep the MSVC hidden-result-pointer return ABI; the size is unavailable.
    return pResult;
}

// Retail mfc140u RVA 0x10edb0 dispatches to vtable +0x510, then uses flags at
// +0x138/+0x134 and sizing state at +0xa0/+0x190/+0x194; those fields are absent.
// Symbol: ?GetRegularSize@CMFCRibbonLinkCtrl@@UEAA?AVCSize@@PEAVCDC@@@Z
extern "C" void* MS_ABI impl__GetRegularSize_CMFCRibbonLinkCtrl__UEAA_AVCSize__PEAVCDC___Z(
    void* pThis, void* pResult, CDC* pDC) {
    (void)pThis;
    (void)pDC;
    // Keep the MSVC hidden-result-pointer return ABI; the size is unavailable.
    return pResult;
}

// Retail mfc140u RVA 0x10f2d0 tail-jumps to the OpenLink helper at RVA 0x10f090 (mfc140u);
// it requires the uninitialized CString at +0x270 and calls SHELL32!ShellExecuteExW
// through delay-load slot 0x1803e91b0 (mfc140u; resolved with dlyu.py).
// Symbol: ?OnAccDefaultAction@CMFCRibbonLinkCtrl@@MEAAXXZ
extern "C" void MS_ABI impl__OnAccDefaultAction_CMFCRibbonLinkCtrl__MEAAXXZ(void* pThis) {
    (void)pThis;
}

// Retail mfc140u RVA 0x10ee60 checks the button rect at +0xc8 with
// USER32!IsRectEmpty, then paints through visual-manager and CDC state not modeled here.
// Symbol: ?OnDraw@CMFCRibbonLinkCtrl@@UEAAXPEAVCDC@@@Z
extern "C" void MS_ABI impl__OnDraw_CMFCRibbonLinkCtrl__UEAAXPEAVCDC___Z(
    void* pThis, CDC* pDC) {
    (void)pThis;
    (void)pDC;
}

// The mfc140u map has no resolved entry; ANSI twin RVA 0x10fec0 (mfc140.dll)
// calls an internal routine and follows a module-global path; that state is unavailable.
// Symbol: ?OnMouseMove@CMFCRibbonLinkCtrl@@UEAAXVCPoint@@@Z
extern "C" void MS_ABI impl__OnMouseMove_CMFCRibbonLinkCtrl__UEAAXVCPoint___Z(
    void* pThis, long long point) {
    (void)pThis;
    (void)point;
}

// The mfc140u map has no resolved entry; ANSI twin RVA 0x10ff10 (mfc140.dll)
// tests icon state at +0x1d8 and may load/store shared icon data absent here.
// Symbol: ?OnSetIcon@CMFCRibbonLinkCtrl@@UEAAXXZ
extern "C" void MS_ABI impl__OnSetIcon_CMFCRibbonLinkCtrl__UEAAXXZ(void* pThis) {
    (void)pThis;
}

// The mfc140u map has no resolved entry; the ANSI twin entry is RVA 0x10fe40
// (mfc140.dll). The U helper at RVA 0x10f090 (mfc140u) calls ShellExecuteExW but needs CString state at +0x270.
// Symbol: ?OpenLink@CMFCRibbonLinkCtrl@@QEAAHXZ
extern "C" int MS_ABI impl__OpenLink_CMFCRibbonLinkCtrl__QEAAHXZ(void* pThis) {
    (void)pThis;
    return 0;
}

// The mfc140u map has no resolved entry. ANSI twin entry RVA 0x110030
// (mfc140.dll) delegates to CMFCRibbonButton::SetACCData; on success it sets
// role 0x1e, action text "Jump", and state 0x400000. The existing base thunk
// supplies OpenMFC's base approximation; the link-specific output is transcribed.
// Symbol: ?SetACCData@CMFCRibbonLinkCtrl@@MEAAHPEAVCWnd@@AEAVCAccessibilityData@@@Z
extern "C" int MS_ABI impl__SetACCData_CMFCRibbonLinkCtrl__MEAAHPEAVCWnd__AEAVCAccessibilityData___Z(
    void* pThis, CWnd* pWnd, CAccessibilityData* pData) {
    if (!impl__SetACCData_CMFCRibbonButton__UEAAHPEAVCWnd__AEAVCAccessibilityData___Z(
            static_cast<CMFCRibbonButton*>(pThis), pWnd, *pData)) {
        return FALSE;
    }
    pData->m_nAccRole = 0x1e;
    impl__SetString___CSimpleStringT__W_00_ATL__QEAAXPEB_W_Z(
        &pData->m_strAccDefAction, L"Jump");
    pData->m_bAccState = 0x400000;
    return TRUE;
}

// The mfc140u map has no resolved entry; ANSI twin RVA 0x10fff0 (mfc140.dll)
// checks for a null input to choose length zero, then assigns it to the CString at
// +0x270, which is not initialized.
// Symbol: ?SetLink@CMFCRibbonLinkCtrl@@QEAAXPEB_W@Z
extern "C" void MS_ABI impl__SetLink_CMFCRibbonLinkCtrl__QEAAXPEB_W_Z(
    void* pThis, const wchar_t* link) {
    (void)pThis;
    (void)link;
}
