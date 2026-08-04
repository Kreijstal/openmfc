// AFX_COM — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp

#include "detail/ManualSmallStubImplementationsSupport.h"

// Symbol: ?CreateInstance@AFX_COM@@QEAAJAEBU_GUID@@PEAUIUnknown@@0PEAPEAX@Z
extern "C" long long MS_ABI impl__CreateInstance_AFX_COM__QEAAJAEBU_GUID__PEAUIUnknown__0PEAPEAX_Z(
    const GUID* clsid, IUnknown* pOuter, const GUID* iid, void** ppv) {
    if (ppv == nullptr || clsid == nullptr || iid == nullptr) {
        return static_cast<long long>(E_POINTER);
    }
    return static_cast<long long>(::CoCreateInstance(*clsid, pOuter, CLSCTX_ALL, *iid, ppv));
}
// Symbol: ?GetClassObject@AFX_COM@@QEAAJAEBU_GUID@@0PEAPEAX@Z
extern "C" long long MS_ABI impl__GetClassObject_AFX_COM__QEAAJAEBU_GUID__0PEAPEAX_Z(
    const GUID* clsid, const GUID* iid, void** ppv) {
    if (ppv == nullptr || clsid == nullptr || iid == nullptr) {
        return static_cast<long long>(E_POINTER);
    }
    return static_cast<long long>(::CoGetClassObject(*clsid, CLSCTX_ALL, nullptr, *iid, ppv));
}
