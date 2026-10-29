// CMFCOutlookBarPaneAdapter — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp

#include "detail/ManualSmallStubImplementationsSupport.h"

extern "C" void MS_ABI impl___1CDockablePaneAdapter__UEAA_XZ(void* pThis);
extern "C" void MS_ABI impl__OnNcDestroy_CPane__IEAAXXZ(CPane* pThis);

// Symbol: ??0CMFCOutlookBarPaneAdapter@@QEAA@XZ
extern "C" void* MS_ABI impl___0CMFCOutlookBarPaneAdapter__QEAA_XZ(void* pThis) {
    return pThis;
}
// Symbol: ??1CMFCOutlookBarPaneAdapter@@UEAA@XZ
// The ANSI twin at RVA 0x467a0 (mfc140) stores the derived vftable then tail-jumps
// to CDockablePaneAdapter::~CDockablePaneAdapter. OpenMFC does not install retail
// vftables here; delegate the inherited teardown to that adapter destructor thunk.
extern "C" void MS_ABI impl___1CMFCOutlookBarPaneAdapter__UEAA_XZ(void* pThis) {
    impl___1CDockablePaneAdapter__UEAA_XZ(pThis);
}
// Symbol: ?CreateObject@CMFCOutlookBarPaneAdapter@@SAPEAVCObject@@XZ
extern "C" void* MS_ABI impl__CreateObject_CMFCOutlookBarPaneAdapter__SAPEAVCObject__XZ() {
    return new (std::nothrow) CMFCOutlookBarPaneAdapterObject();
}
// Symbol: ?OnNcDestroy@CMFCOutlookBarPaneAdapter@@IEAAXXZ
// The ANSI twin at RVA 0x9abe0 (mfc140) calls CPane::OnNcDestroy, then, if this
// is non-null, invokes vtable slot 1 with flag 1 (the scalar deleting destructor).
extern "C" void MS_ABI impl__OnNcDestroy_CMFCOutlookBarPaneAdapter__IEAAXXZ(void* pThis) {
    impl__OnNcDestroy_CPane__IEAAXXZ(static_cast<CPane*>(pThis));
    if (pThis == nullptr) return;

    void** vtable = *reinterpret_cast<void***>(pThis);
    using ScalarDeletingDestructor = void* (MS_ABI *)(void*, unsigned int);
    auto deletingDestructor = reinterpret_cast<ScalarDeletingDestructor>(vtable[1]);
    deletingDestructor(pThis, 1);
}
