// CDHtmlEventSink — OpenMFC implementation.
// Sources: typed_stubs-generated baseline; honest COM HRESULT returns.

#include <cstddef>
#include <cstring>
#include <windows.h>
#include <objbase.h>
#include <oaidl.h>
#include <ocidl.h>

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

// Symbol: ?AddRef@CDHtmlEventSink@@UEAAKXZ
extern "C" unsigned long MS_ABI impl__AddRef_CDHtmlEventSink__UEAAKXZ(void* pThis) {
    (void)pThis;
    return 1;
}

// Symbol: ?CDHtmlSinkHandlerQueryInterface@CDHtmlEventSink@@UEAAJAEBU_GUID@@PEAPEAX@Z
extern "C" long MS_ABI impl__CDHtmlSinkHandlerQueryInterface_CDHtmlEventSink__UEAAJAEBU_GUID__PEAPEAX_Z(
    void* pThis, const GUID* iid, void** ppvObj) {
    if (!ppvObj || !iid) return E_POINTER;
    *ppvObj = nullptr;
    if (GuidEqual(*iid, IID_IUnknown) || GuidEqual(*iid, IID_IDispatch)) {
        *ppvObj = pThis;
        return S_OK;
    }
    return E_NOINTERFACE;
}

// Symbol: ?ConnectToConnectionPoint@CDHtmlEventSink@@QEAAJPEAUIUnknown@@AEBU_GUID@@PEAK@Z
extern "C" long MS_ABI impl__ConnectToConnectionPoint_CDHtmlEventSink__QEAAJPEAUIUnknown__AEBU_GUID__PEAK_Z(
    void* pThis, IUnknown* pUnk, const GUID* iid, unsigned long* pdwCookie) {
    if (!pUnk || !iid || !pdwCookie) return E_POINTER;
    IConnectionPointContainer* pCPC = nullptr;
    HRESULT hr = pUnk->QueryInterface(IID_IConnectionPointContainer, (void**)&pCPC);
    if (FAILED(hr)) return hr;
    IConnectionPoint* pCP = nullptr;
    hr = pCPC->FindConnectionPoint(*iid, &pCP);
    pCPC->Release();
    if (FAILED(hr)) return hr;
    hr = pCP->Advise(reinterpret_cast<IUnknown*>(pThis), pdwCookie);
    pCP->Release();
    return hr;
}

// Symbol: ?DHtmlEventHook@CDHtmlEventSink@@UEAAHPEAJJPEAUtagDISPPARAMS@@PEAUtagVARIANT@@PEAUtagEXCEPINFO@@PEAI@Z
extern "C" int MS_ABI impl__DHtmlEventHook_CDHtmlEventSink__UEAAHPEAJJPEAUtagDISPPARAMS__PEAUtagVARIANT__PEAUtagEXCEPINFO__PEAI_Z(
    void* pThis, long* pDispId, long lDispId, DISPPARAMS* pDispParams, VARIANT* pVarResult,
    EXCEPINFO* pExcepInfo, unsigned int* puArgErr) {
    (void)pThis; (void)pDispId; (void)lDispId; (void)pDispParams; (void)pExcepInfo;
    if (puArgErr) *puArgErr = 0;
    if (pVarResult) {
        std::memset(pVarResult, 0, sizeof(VARIANT));
        pVarResult->vt = VT_EMPTY;
    }
    return FALSE;
}

// Symbol: ?DisconnectFromConnectionPoint@CDHtmlEventSink@@QEAAXPEAUIUnknown@@AEBU_GUID@@AEAK@Z
extern "C" void MS_ABI impl__DisconnectFromConnectionPoint_CDHtmlEventSink__QEAAXPEAUIUnknown__AEBU_GUID__AEAK_Z(
    void* pThis, IUnknown* pUnk, const GUID* iid, unsigned long* pdwCookie) {
    (void)pThis;
    if (!pUnk || !iid || !pdwCookie) return;
    IConnectionPointContainer* pCPC = nullptr;
    if (FAILED(pUnk->QueryInterface(IID_IConnectionPointContainer, (void**)&pCPC))) return;
    IConnectionPoint* pCP = nullptr;
    if (SUCCEEDED(pCPC->FindConnectionPoint(*iid, &pCP))) {
        pCP->Unadvise(*pdwCookie);
        pCP->Release();
    }
    pCPC->Release();
    *pdwCookie = 0;
}

// Symbol: ?FindDHtmlEventEntry@CDHtmlEventSink@@QEAAHPEBUDHtmlEventMapEntry@@JPEAPEAUIHTMLElement@@@Z
extern "C" int MS_ABI impl__FindDHtmlEventEntry_CDHtmlEventSink__QEAAHPEBUDHtmlEventMapEntry__JPEAPEAUIHTMLElement___Z(
    void* pThis, const void* pMapEntry, long lDispId, void** ppElement) {
    (void)pThis; (void)pMapEntry; (void)lDispId;
    if (ppElement) *ppElement = nullptr;
    return FALSE;
}

// Symbol: ?GetIDsOfNames@CDHtmlEventSink@@UEAAJAEBU_GUID@@PEAPEA_WIKPEAJ@Z
extern "C" long MS_ABI impl__GetIDsOfNames_CDHtmlEventSink__UEAAJAEBU_GUID__PEAPEA_WIKPEAJ_Z(
    void* pThis, const GUID* riid, wchar_t** rgszNames, unsigned int cNames,
    unsigned long lcid, long* rgDispId) {
    (void)pThis; (void)riid; (void)rgszNames; (void)cNames; (void)lcid; (void)rgDispId;
    return E_NOTIMPL;
}

// Symbol: ?GetTypeInfo@CDHtmlEventSink@@UEAAJIKPEAPEAUITypeInfo@@@Z
extern "C" long MS_ABI impl__GetTypeInfo_CDHtmlEventSink__UEAAJIKPEAPEAUITypeInfo___Z(
    void* pThis, unsigned int iTInfo, unsigned long lcid, ITypeInfo** ppTInfo) {
    (void)pThis; (void)iTInfo; (void)lcid;
    if (ppTInfo) *ppTInfo = nullptr;
    return E_NOTIMPL;
}

// Symbol: ?GetTypeInfoCount@CDHtmlEventSink@@UEAAJPEAI@Z
extern "C" long MS_ABI impl__GetTypeInfoCount_CDHtmlEventSink__UEAAJPEAI_Z(
    void* pThis, unsigned int* pctinfo) {
    (void)pThis;
    if (!pctinfo) return E_POINTER;
    *pctinfo = 0;
    return S_OK;
}

// Symbol: ?Invoke@CDHtmlEventSink@@UEAAJJAEBU_GUID@@KGPEAUtagDISPPARAMS@@PEAUtagVARIANT@@PEAUtagEXCEPINFO@@PEAI@Z
extern "C" long MS_ABI impl__Invoke_CDHtmlEventSink__UEAAJJAEBU_GUID__KGPEAUtagDISPPARAMS__PEAUtagVARIANT__PEAUtagEXCEPINFO__PEAI_Z(
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

// Symbol: ?Release@CDHtmlEventSink@@UEAAKXZ
extern "C" unsigned long MS_ABI impl__Release_CDHtmlEventSink__UEAAKXZ(void* pThis) {
    (void)pThis;
    return 1;
}
