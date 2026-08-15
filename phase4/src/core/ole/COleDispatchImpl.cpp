// COleDispatchImpl — OpenMFC implementation.
// Sources: typed_stubs-generated baseline; honest COM HRESULT returns.

#include <cstddef>
#include <cstring>
#include <windows.h>
#include <objbase.h>
#include <oaidl.h>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

namespace {
bool GuidEqual(const GUID& a, const GUID& b) {
    return std::memcmp(&a, &b, sizeof(GUID)) == 0;
}
}

// The class is an IDispatch mix-in without a modeled refcount slot, so AddRef /
// Release keep a stable non-zero count (1) instead of falsely reporting that
// the object was destroyed.

// Symbol: ?AddRef@COleDispatchImpl@@UEAAKXZ
extern "C" unsigned long MS_ABI impl__AddRef_COleDispatchImpl__UEAAKXZ(void* pThis) {
    (void)pThis;
    return 1;
}

// Symbol: ?Disconnect@COleDispatchImpl@@UEAAXXZ
extern "C" void MS_ABI impl__Disconnect_COleDispatchImpl__UEAAXXZ(void* pThis) {
    (void)pThis;
}

// Symbol: ?GetIDsOfNames@COleDispatchImpl@@UEAAJAEBU_GUID@@PEAPEA_WIKPEAJ@Z
extern "C" long MS_ABI impl__GetIDsOfNames_COleDispatchImpl__UEAAJAEBU_GUID__PEAPEA_WIKPEAJ_Z(
    void* pThis, const GUID* riid, wchar_t** rgszNames, unsigned int cNames,
    unsigned long lcid, long* rgDispId) {
    (void)pThis; (void)riid; (void)rgszNames; (void)cNames; (void)lcid; (void)rgDispId;
    return E_NOTIMPL;
}

// Symbol: ?GetTypeInfo@COleDispatchImpl@@UEAAJIKPEAPEAUITypeInfo@@@Z
extern "C" long MS_ABI impl__GetTypeInfo_COleDispatchImpl__UEAAJIKPEAPEAUITypeInfo___Z(
    void* pThis, unsigned int iTInfo, unsigned long lcid, ITypeInfo** ppTInfo) {
    (void)pThis; (void)iTInfo; (void)lcid;
    if (ppTInfo) *ppTInfo = nullptr;
    return E_NOTIMPL;
}

// Symbol: ?GetTypeInfoCount@COleDispatchImpl@@UEAAJPEAI@Z
extern "C" long MS_ABI impl__GetTypeInfoCount_COleDispatchImpl__UEAAJPEAI_Z(
    void* pThis, unsigned int* pctinfo) {
    (void)pThis;
    if (!pctinfo) return E_POINTER;
    *pctinfo = 0;
    return S_OK;
}

// Symbol: ?Invoke@COleDispatchImpl@@UEAAJJAEBU_GUID@@KGPEAUtagDISPPARAMS@@PEAUtagVARIANT@@PEAUtagEXCEPINFO@@PEAI@Z
extern "C" long MS_ABI impl__Invoke_COleDispatchImpl__UEAAJJAEBU_GUID__KGPEAUtagDISPPARAMS__PEAUtagVARIANT__PEAUtagEXCEPINFO__PEAI_Z(
    void* pThis, long dispIdMember, const GUID* riid, unsigned long lcid,
    unsigned short wFlags, DISPPARAMS* pDispParams, VARIANT* pVarResult,
    EXCEPINFO* pExcepInfo, unsigned int* puArgErr) {
    (void)pThis; (void)dispIdMember; (void)riid; (void)lcid; (void)wFlags;
    (void)pDispParams; (void)pExcepInfo;
    if (puArgErr) *puArgErr = 0;
    if (pVarResult) {
        std::memset(pVarResult, 0, sizeof(VARIANT));
        pVarResult->vt = VT_EMPTY;
    }
    return DISP_E_MEMBERNOTFOUND;
}

// Symbol: ?QueryInterface@COleDispatchImpl@@UEAAJAEBU_GUID@@PEAPEAX@Z
extern "C" long MS_ABI impl__QueryInterface_COleDispatchImpl__UEAAJAEBU_GUID__PEAPEAX_Z(
    void* pThis, const GUID* iid, void** ppvObj) {
    if (!ppvObj || !iid) return E_POINTER;
    *ppvObj = nullptr;
    if (GuidEqual(*iid, IID_IUnknown) || GuidEqual(*iid, IID_IDispatch)) {
        *ppvObj = pThis;
        return S_OK;
    }
    return E_NOINTERFACE;
}

// Symbol: ?Release@COleDispatchImpl@@UEAAKXZ
extern "C" unsigned long MS_ABI impl__Release_COleDispatchImpl__UEAAKXZ(void* pThis) {
    (void)pThis;
    return 1;
}
