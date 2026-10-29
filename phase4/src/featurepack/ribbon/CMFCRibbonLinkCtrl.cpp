// CMFCRibbonLinkCtrl — OpenMFC implementation.
// Sources: retail mfc140/mfc140u disassembly; manual_small_stub_implementations.cpp.

#include "detail/ManualSmallStubImplementationsSupport.h"

#include <cstddef>
#include <cstdint>
#include <shellapi.h>

extern "C" void* MS_ABI impl___0CMFCRibbonButton__QEAA_XZ(void* pThis);
extern "C" void MS_ABI impl___1CMFCRibbonButton__UEAA_XZ(void* pThis);
extern "C" void MS_ABI impl___0__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAA_XZ(
    void* pThis);
extern "C" void MS_ABI impl___1__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAA_XZ(
    void* pThis);
extern "C" void* MS_ABI impl___4__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAAAEAV01_AEBV01__Z(
    void* pThis, const void* pSource);
extern "C" void MS_ABI impl__SetID_CMFCRibbonBaseElement__UEAAXI_Z(
    CMFCRibbonBaseElement* pThis, unsigned int id);
extern "C" void MS_ABI impl__SetText_CMFCRibbonButton__UEAAXPEB_W_Z(
    CMFCRibbonButton* pThis, const wchar_t* text);
extern "C" void MS_ABI impl__CopyFrom_CMFCRibbonButton__MEAAXAEBVCMFCRibbonBaseElement___Z(
    CMFCRibbonButton* pThis, const CMFCRibbonBaseElement* pSource);
extern "C" int MS_ABI impl__OpenLink_CMFCRibbonLinkCtrl__QEAAHXZ(void* pThis);
extern "C" int MS_ABI impl__SetACCData_CMFCRibbonButton__UEAAHPEAVCWnd__AEAVCAccessibilityData___Z(
    CMFCRibbonButton* pThis, CWnd* pWnd, CAccessibilityData& data);
extern "C" void MS_ABI impl__GetCompactSize_CMFCRibbonButton__UEAA_AVCSize__PEAVCDC___Z(
    void* pResult, const CMFCRibbonButton* pThis, CDC* pDC);
extern "C" void MS_ABI impl__GetRegularSize_CMFCRibbonButton__UEAA_AVCSize__PEAVCDC___Z(
    void* pResult, const CMFCRibbonButton* pThis, CDC* pDC);
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
static_assert(sizeof(SHELLEXECUTEINFOW) == 0x70,
              "retail CMFCRibbonLinkCtrl ShellExecuteExW structure size");
static_assert(offsetof(S_CMFCRibbonLinkCtrlLayout, m_strLink) == 0x270,
              "retail CMFCRibbonLinkCtrl::m_strLink offset");
static_assert(sizeof(S_CMFCRibbonLinkCtrlLayout) == 0x278,
              "retail CMFCRibbonLinkCtrl size");

CString* link_string(void* pThis) {
    return reinterpret_cast<CString*>(static_cast<std::uint8_t*>(pThis) + 0x270);
}
const CString* link_string(const void* pThis) {
    return reinterpret_cast<const CString*>(static_cast<const std::uint8_t*>(pThis) + 0x270);
}

void construct_link_ctrl(void* pThis) {
    impl___0CMFCRibbonButton__QEAA_XZ(pThis);
    impl___0__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAA_XZ(
        link_string(pThis));
}
} // namespace

// Retail constructors set a LinkCtrl vptr that OpenMFC does not provide. Build
// the modeled button subobject and initialize the offset-asserted CString so
// the non-virtual link operations have a valid lifetime; virtual dispatch still
// uses the clean-room button vtable rather than retail's derived vtable.
// Symbol: ??0CMFCRibbonLinkCtrl@@QEAA@IPEB_W0@Z
extern "C" void* MS_ABI impl___0CMFCRibbonLinkCtrl__QEAA_IPEB_W0_Z(
    void* pThis, unsigned int id, const wchar_t* text, const wchar_t* link) {
    construct_link_ctrl(pThis);
    if (text != nullptr && link != nullptr) {
        impl__SetID_CMFCRibbonBaseElement__UEAAXI_Z(
            static_cast<CMFCRibbonBaseElement*>(pThis), id);
        impl__SetText_CMFCRibbonButton__UEAAXPEB_W_Z(
            static_cast<CMFCRibbonButton*>(pThis), text);
        impl__SetString___CSimpleStringT__W_00_ATL__QEAAXPEB_W_Z(
            link_string(pThis), link);
    }
    return pThis;
}
// Symbol: ??0CMFCRibbonLinkCtrl@@QEAA@XZ
extern "C" void* MS_ABI impl___0CMFCRibbonLinkCtrl__QEAA_XZ(void* pThis) {
    construct_link_ctrl(pThis);
    return pThis;
}

// Retail mfc140u entry RVA 0x10ed60 releases m_strLink (+0x270), then enters
// the inherited destructor path. Both subobjects are initialized by these
// OpenMFC constructors; invoke their matching destructors in member/base order.
// Symbol: ??1CMFCRibbonLinkCtrl@@UEAA@XZ
extern "C" void MS_ABI impl___1CMFCRibbonLinkCtrl__UEAA_XZ(void* pThis) {
    impl___1__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAA_XZ(
        link_string(pThis));
    impl___1CMFCRibbonButton__UEAA_XZ(pThis);
}

// Retail mfc140u RVA 0x10f050 calls CMFCRibbonButton::CopyFrom (entry RVA
// 0xe7aa0, mfc140u), then assigns the source CString at +0x270 (operator= entry
// RVA 0xde30, mfc140u). The existing button CopyFrom thunk is a no-op, so
// this only copies the modeled link member and leaves button state unchanged.
// Symbol: ?CopyFrom@CMFCRibbonLinkCtrl@@UEAAXAEBVCMFCRibbonBaseElement@@@Z
extern "C" void MS_ABI impl__CopyFrom_CMFCRibbonLinkCtrl__UEAAXAEBVCMFCRibbonBaseElement___Z(
    void* pThis, const CMFCRibbonBaseElement* pSource) {
    if (pThis == nullptr || pSource == nullptr) return;
    impl__CopyFrom_CMFCRibbonButton__MEAAXAEBVCMFCRibbonBaseElement___Z(
        static_cast<CMFCRibbonButton*>(pThis), pSource);
    impl___4__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAAAEAV01_AEBV01__Z(
        link_string(pThis), link_string(pSource));
}

// Retail mfc140u entry RVA 0x10ebf0 allocates 0x278 bytes and calls the
// constructor at RVA 0x10ec30. The descriptor's factory callback is null in
// RuntimeClasses.cpp, and OpenMFC still lacks the LinkCtrl-derived vtable; a
// button-vtable approximation is not a retail-compatible factory result.
// Symbol: ?CreateObject@CMFCRibbonLinkCtrl@@SAPEAVCObject@@XZ
extern "C" void* MS_ABI impl__CreateObject_CMFCRibbonLinkCtrl__SAPEAVCObject__XZ() {
    return nullptr;
}

// Retail's ANSI twin (entry RVA 0xe160, mfc140.dll) forwards through vtable
// byte offset +0x200 (slot 64). OpenMFC does not model that LinkCtrl vtable
// slot, so use its available CMFCRibbonButton sizing approximation instead.
// Symbol: ?GetCompactSize@CMFCRibbonLinkCtrl@@UEAA?AVCSize@@PEAVCDC@@@Z
extern "C" void* MS_ABI impl__GetCompactSize_CMFCRibbonLinkCtrl__UEAA_AVCSize__PEAVCDC___Z(
    void* pThis, void* pResult, CDC* pDC) {
    impl__GetCompactSize_CMFCRibbonButton__UEAA_AVCSize__PEAVCDC___Z(
        pResult, static_cast<const CMFCRibbonButton*>(pThis), pDC);
    return pResult;
}

// Retail mfc140u entry RVA 0x10edb0 forwards through vtable byte offset
// +0x510 and then applies inherited size/flag logic. Use the existing button
// sizing approximation because the LinkCtrl vtable and retail state aren't modeled.
// Symbol: ?GetRegularSize@CMFCRibbonLinkCtrl@@UEAA?AVCSize@@PEAVCDC@@@Z
extern "C" void* MS_ABI impl__GetRegularSize_CMFCRibbonLinkCtrl__UEAA_AVCSize__PEAVCDC___Z(
    void* pThis, void* pResult, CDC* pDC) {
    impl__GetRegularSize_CMFCRibbonButton__UEAA_AVCSize__PEAVCDC___Z(
        pResult, static_cast<const CMFCRibbonButton*>(pThis), pDC);
    return pResult;
}

// Retail mfc140u entry RVA 0x10f2d0 tail-jumps to the OpenLink helper at
// RVA 0x10f090 (mfc140u).
// Symbol: ?OnAccDefaultAction@CMFCRibbonLinkCtrl@@MEAAXXZ
extern "C" void MS_ABI impl__OnAccDefaultAction_CMFCRibbonLinkCtrl__MEAAXXZ(void* pThis) {
    (void)impl__OpenLink_CMFCRibbonLinkCtrl__QEAAHXZ(pThis);
}

// Retail mfc140u RVA 0x10ee60 first checks the rect at +0xc8 with
// USER32!IsRectEmpty (IAT slot 0x1802c72a0, mfc140u, resolved with iatu.py),
// then gates drawing on flags at +0x134/+0x138 and uses additional button,
// visual-manager, and CDC state. Those retail members and drawing paths are not modeled.
// Symbol: ?OnDraw@CMFCRibbonLinkCtrl@@UEAAXPEAVCDC@@@Z
extern "C" void MS_ABI impl__OnDraw_CMFCRibbonLinkCtrl__UEAAXPEAVCDC___Z(
    void* pThis, CDC* pDC) {
    (void)pThis;
    (void)pDC;
}

// No mfc140u export RVA resolves. The ANSI twin entry RVA 0x10fec0
// (mfc140.dll) first calls CMFCRibbonButton::OnMouseMove (entry RVA
// 0xe7fc0, mfc140.dll), then follows module-global state and an indirect
// dispatch path not modeled here.
// Symbol: ?OnMouseMove@CMFCRibbonLinkCtrl@@UEAAXVCPoint@@@Z
extern "C" void MS_ABI impl__OnMouseMove_CMFCRibbonLinkCtrl__UEAAXVCPoint___Z(
    void* pThis, long long point) {
    (void)pThis;
    (void)point;
}

// No mfc140u export RVA resolves. The ANSI twin entry RVA 0x10ff10
// (mfc140.dll) tests icon state at +0x1d8 and initializes a shared icon through
// module-global state; that cache and its initialization path are not modeled.
// Symbol: ?OnSetIcon@CMFCRibbonLinkCtrl@@UEAAXXZ
extern "C" void MS_ABI impl__OnSetIcon_CMFCRibbonLinkCtrl__UEAAXXZ(void* pThis) {
    (void)pThis;
}

// The mfc140u export has no resolved entry; helper entry RVA 0x10f090
// (mfc140u) returns FALSE for an empty CString at +0x270, otherwise fills the
// 0x70-byte SHELLEXECUTEINFOW and calls SHELL32!ShellExecuteExW (delay-load
// slot 0x1803e91b0, mfc140u). The fields below match that structure setup.
// Symbol: ?OpenLink@CMFCRibbonLinkCtrl@@QEAAHXZ
extern "C" int MS_ABI impl__OpenLink_CMFCRibbonLinkCtrl__QEAAHXZ(void* pThis) {
    if (pThis == nullptr || link_string(pThis)->IsEmpty()) return FALSE;
    SHELLEXECUTEINFOW info{};
    info.cbSize = sizeof(info);
    info.fMask = 0x40;
    info.lpFile = link_string(pThis)->GetString();
    info.nShow = SW_SHOWNORMAL;
    return ::ShellExecuteExW(&info) ? TRUE : FALSE;
}

// The mfc140u map has no resolved entry. ANSI twin entry RVA 0x110030
// (mfc140.dll) delegates to CMFCRibbonButton::SetACCData; on success it sets
// role 0x1e, action text "Jump", and state 0x400000. The OpenMFC base thunk
// currently only returns TRUE and does not initialize the other data fields;
// this body writes the link-specific fields but remains an incomplete approximation.
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

// No mfc140u export RVA resolves. ANSI entry RVA 0x10fff0 (mfc140.dll)
// chooses length zero for null input, otherwise calls strlen through slot
// 0x1802c5890 and assigns the CString at +0x270 with SetString. OpenMFC's
// SetString thunk likewise empties the Unicode member for null input.
// Symbol: ?SetLink@CMFCRibbonLinkCtrl@@QEAAXPEB_W@Z
extern "C" void MS_ABI impl__SetLink_CMFCRibbonLinkCtrl__QEAAXPEB_W_Z(
    void* pThis, const wchar_t* link) {
    if (pThis == nullptr) return;
    impl__SetString___CSimpleStringT__W_00_ATL__QEAAXPEB_W_Z(
        link_string(pThis), link);
}
