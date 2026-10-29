// CPagerCtrl — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp

#include "detail/ManualSmallStubImplementationsSupport.h"

extern CRuntimeClass classCPagerCtrl;
extern "C" int MS_ABI impl__AfxEndDeferRegisterClass__YAHJ_Z(long);
extern "C" void MS_ABI impl__AfxThrowInvalidArgException__YAXXZ();
extern "C" int MS_ABI impl__CreateEx_CWnd__UEAAHKPEB_W0KAEBUtagRECT__PEAV1_IPEAX_Z(
    CWnd* pThis, DWORD dwExStyle, const wchar_t* lpszClassName, const wchar_t* lpszWindowName,
    DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, void* lpParam);

// Symbol: ?Create@CPagerCtrl@@UEAAHKAEBUtagRECT@@PEAVCWnd@@I@Z
extern "C" int MS_ABI impl__Create_CPagerCtrl__UEAAHKAEBUtagRECT__PEAVCWnd__I_Z(
    void* pThis, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID) {
    // Retail CPagerCtrl::Create (RVA 0x29c430, mfc140u) calls
    // AfxEndDeferRegisterClass(0x80000), then dispatches vtable byte offset 0xb8
    // with this class and the remaining creation arguments on success.
    if (!impl__AfxEndDeferRegisterClass__YAHJ_Z(0x80000)) {
        impl__AfxThrowInvalidArgException__YAXXZ();
        return FALSE;
    }

    using CreateFn = BOOL (MS_ABI*)(void*, const wchar_t*, const wchar_t*, DWORD,
                                    const RECT&, CWnd*, UINT, CCreateContext*);
    auto** vtable = *reinterpret_cast<void***>(pThis);
    auto create = reinterpret_cast<CreateFn>(vtable[0xb8 / sizeof(void*)]);
    return create(pThis, L"SysPager", nullptr, dwStyle, rect,
                  pParentWnd, nID, nullptr);
}
// CPagerCtrl::CreateEx has no resolved RVA in the mfc140u symbol map. It follows
// the afxcmn CreateEx pattern shared by the other common controls: register
// the pager class like Create above, then CWnd::CreateEx with "SysPager".
// Symbol: ?CreateEx@CPagerCtrl@@UEAAHKKAEBUtagRECT@@PEAVCWnd@@I@Z
extern "C" int MS_ABI impl__CreateEx_CPagerCtrl__UEAAHKKAEBUtagRECT__PEAVCWnd__I_Z(
    void* pThis, DWORD dwExStyle, DWORD dwStyle, const RECT& rect,
    CWnd* pParentWnd, UINT nID) {
    if (!impl__AfxEndDeferRegisterClass__YAHJ_Z(0x80000)) {
        impl__AfxThrowInvalidArgException__YAXXZ();
    }
    return impl__CreateEx_CWnd__UEAAHKPEB_W0KAEBUtagRECT__PEAV1_IPEAX_Z(
        static_cast<CWnd*>(pThis), dwExStyle, L"SysPager", nullptr, dwStyle,
        rect, pParentWnd, nID, nullptr);
}
// Retail returns the static CPagerCtrl descriptor directly (RVA 0x29c700, mfc140u).
// The corresponding descriptor is defined in RuntimeClasses.cpp.
// Symbol: ?GetRuntimeClass@CPagerCtrl@@UEBAPEAUCRuntimeClass@@XZ
extern "C" void* MS_ABI impl__GetRuntimeClass_CPagerCtrl__UEBAPEAUCRuntimeClass__XZ(
    void* pThis) {
    (void)pThis;
    return &classCPagerCtrl;
}
// No resolved retail RVA for this static getter; use the same descriptor, following the
// common-control RTTI getter pattern in RuntimeClasses.cpp.
// Symbol: ?GetThisClass@CPagerCtrl@@SAPEAUCRuntimeClass@@XZ
extern "C" void* MS_ABI impl__GetThisClass_CPagerCtrl__SAPEAUCRuntimeClass__XZ() {
    return &classCPagerCtrl;
}
