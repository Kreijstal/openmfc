// CDHtmlControlSink — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp
//
// CDHtmlControlSink (afxdhtml.h) is the IDispatch event sink CDHtmlDialog advises on an
// ActiveX control named in a DHTML_EVENT_ONCONTROL... event-map entry.  Every body below is
// transcribed from the retail code (disassembled with disas.py; function bodies are
// byte-identical between mfc140.dll and mfc140u.dll, and the RVAs quoted are mfc140u's, read
// with `disas.py --u`).  QueryInterface / AddRef / Release / GetIDsOfNames have no RVA in the
// export map; their bodies were read from the retail vftable's slots (below).  Deviations
// from retail are marked DEVIATION where they occur.  Retail compiled the sink from ATL's
// IDispEventSimpleImpl / AtlGetObjectSourceInterface / AtlGetUserDefinedType / AtlAdvise /
// AtlUnadvise code, which is why several helpers below mirror atlcom.h line for line.
//
// ---- layout, pinned from the retail constructors / destructor --------------------------
// No OpenMFC header declares this class, so the layout is pinned here (S_DHCS).  Offsets are
// the ones the retail code touches; they agree with the afxdhtml.h member order under x64
// MSVC packing, and sizeof 0x60 is the size the retail scalar deleting destructor passes to
// operator delete.
//   +0x00 vfptr            +0x08 m_szControlId (LPCTSTR)   +0x10 m_dwCookie
//   +0x18 m_spunkObj       +0x20 m_iid                     +0x30 m_libid
//   +0x40 m_wMajor         +0x42 m_wMinor                  +0x48 m_spTypeInfo
//   +0x50 m_pHandler (CDHtmlSinkHandler*)                  +0x58 m_dwThunkOffset
//
// ---- the vftable -------------------------------------------------------------------------
// Retail vftable RVA 0x3274f0 (mfc140u), 8 slots, dumped with vtdump_u.py:
//   0 QueryInterface (0x2103c0, no export-map name)  1 AddRef  2 Release (both the shared
//   `mov $1,%eax; ret` body 0x3a60)  3 GetTypeInfoCount (0x20f850)  4 GetTypeInfo (0x20f860)
//   5 GetIDsOfNames (the shared `mov $E_NOTIMPL,%eax; ret` body 0xf4d0)  6 Invoke (0x210450)
//   7 scalar deleting destructor (0x20fdc0).
// There is no exported ??_7CDHtmlControlSink@@6B@, so a client that does
// `new CDHtmlControlSink(...)` relies on the exported constructor to install it.  The
// constructors below install g_DHCS_Vtbl, an MSVC-layout table of this file's thunks (the
// mechanism core/app/CDataRecoveryHandler.cpp uses).  The object is handed to COM
// (IConnectionPoint::Advise) as an IDispatch*, so the table must be MSVC-layout.
//
// ---- the event handler -------------------------------------------------------------------
// m_pHandler is a CDHtmlSinkHandler -- in practice the CDHtmlEventSink subobject of a client
// CDHtmlDialog, i.e. a client MSVC object.  The only virtual of it retail calls here is
// GetDHtmlEventMap, slot 7 (+0x38), read off the call site in Invoke.

#include "detail/ManualSmallStubImplementationsSupport.h"

#include <cerrno>
#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <cwchar>
#include <ocidl.h>
#include <oleauto.h>

// ---------------------------------------------------------------------------------------
// Cross-file thunks.  Each was grepped to its definition; parameter lists follow the
// mangled names.
// ---------------------------------------------------------------------------------------
//   detail/MfcExceptionsSupport.cpp : ?AfxThrowOleException@@YAXJ@Z, ?AfxThrowInvalidArgException@@YAXXZ
extern "C" void MS_ABI impl__AfxThrowOleException__YAXJ_Z(LONG sc);
extern "C" void MS_ABI impl__AfxThrowInvalidArgException__YAXXZ();
//   detail/MemcoreSupport.cpp : ??3@YAXPEAX@Z (operator delete -> free)
extern "C" void MS_ABI impl___3_YAXPEAX_Z(void* p);

// Forward declarations of this file's own thunks that are called before their definition
// (and placed in the vftable).  Signatures derived from the mangled names.
extern "C" long MS_ABI impl__ConnectToControl_CDHtmlControlSink__QEAAJPEAUIUnknown___Z(void* pThis, IUnknown* punkObj);
extern "C" void MS_ABI impl___1CDHtmlControlSink__UEAA_XZ(void* pThis);
extern "C" long MS_ABI impl__QueryInterface_CDHtmlControlSink__UEAAJAEBU_GUID__PEAPEAX_Z(void* pThis, const GUID* riid, void** ppvObj);
extern "C" unsigned long MS_ABI impl__AddRef_CDHtmlControlSink__UEAAKXZ(void* pThis);
extern "C" unsigned long MS_ABI impl__Release_CDHtmlControlSink__UEAAKXZ(void* pThis);
extern "C" long MS_ABI impl__GetTypeInfoCount_CDHtmlControlSink__UEAAJPEAI_Z(void* pThis, unsigned int* pctinfo);
extern "C" long MS_ABI impl__GetTypeInfo_CDHtmlControlSink__UEAAJIKPEAPEAUITypeInfo___Z(void* pThis, unsigned int iTInfo, unsigned long lcid, ITypeInfo** ppTInfo);
extern "C" long MS_ABI impl__GetIDsOfNames_CDHtmlControlSink__UEAAJAEBU_GUID__PEAPEA_WIKPEAJ_Z(void* pThis, const GUID* riid, wchar_t** rgszNames, unsigned int cNames, unsigned long lcid, long* rgDispId);
extern "C" long MS_ABI impl__Invoke_CDHtmlControlSink__UEAAJJAEBU_GUID__KGPEAUtagDISPPARAMS__PEAUtagVARIANT__PEAUtagEXCEPINFO__PEAI_Z(void* pThis, long dispidMember, const GUID* riid, unsigned long lcid, unsigned short wFlags, DISPPARAMS* pdispparams, VARIANT* pvarResult, EXCEPINFO* pExcepInfo, unsigned int* puArgErr);

// ATL::_ATL_FUNC_INFO (atlcom.h): CALLCONV cc, VARTYPE vtReturn, SHORT nParams,
// VARTYPE pVarTypes[_ATL_MAX_VARTYPES = 8].  The offsets are the ones retail
// GetFuncInfoFromId writes (+0 cc, +4 vtReturn, +6 nParams, +8 pVarTypes[i]); Invoke keeps it
// in a 0x18-byte stack slot.
struct DHCS_ATL_FUNC_INFO {
    CALLCONV cc;
    VARTYPE vtReturn;
    SHORT nParams;
    VARTYPE pVarTypes[8];
};
static_assert(offsetof(DHCS_ATL_FUNC_INFO, vtReturn) == 0x04, "_ATL_FUNC_INFO::vtReturn");
static_assert(offsetof(DHCS_ATL_FUNC_INFO, nParams) == 0x06, "_ATL_FUNC_INFO::nParams");
static_assert(offsetof(DHCS_ATL_FUNC_INFO, pVarTypes) == 0x08, "_ATL_FUNC_INFO::pVarTypes");
static_assert(sizeof(DHCS_ATL_FUNC_INFO) == 0x18, "_ATL_FUNC_INFO size");

// CComStdCallThunkHelper (ATL's asm helper; the retail copy is the 3-instruction body at RVA
// 0x2b82c0 (mfc140u)): `mov %rcx,%rax; mov 8(%rcx),%rcx; jmp *0x10(%rax)`.  It is reached as
// vtable slot 0 of a DHCS_StdCallThunk through DispCallFunc, with the thunk as `this`, and
// swaps in the real handler pointer before tail-jumping to the event function with every
// other argument register and the stack untouched -- which only assembly can do.  The label
// is file-local (no .globl).  COFF gas has no .pushsection; checked with -S that GCC emits
// this block first and issues its own .section directive before the next function.
asm(".text\n"
    ".p2align 4\n"
    "openmfc_DHCS_StdCallThunkHelper:\n"
    "    movq %rcx, %rax\n"
    "    movq 8(%rcx), %rcx\n"
    "    jmp *16(%rax)\n");
extern "C" void openmfc_DHCS_StdCallThunkHelper();

namespace {

struct S_DHCS {
    void* vfptr;                  // +0x00
    const wchar_t* m_szControlId; // +0x08
    DWORD m_dwCookie;             // +0x10
    IUnknown* m_spunkObj;         // +0x18  CComPtr<IUnknown>
    IID m_iid;                    // +0x20
    GUID m_libid;                 // +0x30
    WORD m_wMajor;                // +0x40
    WORD m_wMinor;                // +0x42
    ITypeInfo* m_spTypeInfo;      // +0x48  CComPtr<ITypeInfo>
    void* m_pHandler;             // +0x50  CDHtmlSinkHandler*
    DWORD_PTR m_dwThunkOffset;    // +0x58
};
static_assert(offsetof(S_DHCS, m_szControlId) == 0x08, "CDHtmlControlSink::m_szControlId");
static_assert(offsetof(S_DHCS, m_dwCookie) == 0x10, "CDHtmlControlSink::m_dwCookie");
static_assert(offsetof(S_DHCS, m_spunkObj) == 0x18, "CDHtmlControlSink::m_spunkObj");
static_assert(offsetof(S_DHCS, m_iid) == 0x20, "CDHtmlControlSink::m_iid");
static_assert(offsetof(S_DHCS, m_libid) == 0x30, "CDHtmlControlSink::m_libid");
static_assert(offsetof(S_DHCS, m_wMajor) == 0x40, "CDHtmlControlSink::m_wMajor");
static_assert(offsetof(S_DHCS, m_wMinor) == 0x42, "CDHtmlControlSink::m_wMinor");
static_assert(offsetof(S_DHCS, m_spTypeInfo) == 0x48, "CDHtmlControlSink::m_spTypeInfo");
static_assert(offsetof(S_DHCS, m_pHandler) == 0x50, "CDHtmlControlSink::m_pHandler");
static_assert(offsetof(S_DHCS, m_dwThunkOffset) == 0x58, "CDHtmlControlSink::m_dwThunkOffset");
static_assert(sizeof(S_DHCS) == 0x60, "sizeof(CDHtmlControlSink): 0x60 per the retail deleting dtor");

// DHtmlEventMapEntry (afxdhtml.h): nType, dispId, szName, then a 16-byte pointer-to-member
// (CDHtmlSinkHandler has two bases, so its PMFs use the multiple-inheritance form).  Retail
// Invoke walks the map with a 0x20 stride and reads +0 / +4 / +8 / +0x10..+0x1f.
struct DHCS_EventMapEntry {
    int nType;
    long dispId;
    const wchar_t* szName;
    unsigned char pfnEventFunc[16];
};
static_assert(offsetof(DHCS_EventMapEntry, dispId) == 0x04, "DHtmlEventMapEntry::dispId");
static_assert(offsetof(DHCS_EventMapEntry, szName) == 0x08, "DHtmlEventMapEntry::szName");
static_assert(offsetof(DHCS_EventMapEntry, pfnEventFunc) == 0x10, "DHtmlEventMapEntry::pfnEventFunc");
static_assert(sizeof(DHCS_EventMapEntry) == 0x20, "DHtmlEventMapEntry size");
constexpr int kDHTMLEVENTMAPENTRY_CONTROL = 4;  // afxdhtml.h enum DhtmlEventMapEntryType
constexpr int kDHTMLEVENTMAPENTRY_END = 5;

// ATL::CComStdCallThunk<CDHtmlSinkHandler>: pVTable, pThis, pfn (the 16-byte PMF), pfnHelper.
// Retail InvokeFromFuncInfo builds it at +0x18/+0x20/+0x28/+0x38 of its frame.
struct DHCS_StdCallThunk {
    void* pVTable;
    void* pThis;
    unsigned char pfn[16];
    void (*pfnHelper)();
};
static_assert(offsetof(DHCS_StdCallThunk, pThis) == 0x08, "CComStdCallThunk::pThis");
static_assert(offsetof(DHCS_StdCallThunk, pfn) == 0x10, "CComStdCallThunk::pfn");
static_assert(offsetof(DHCS_StdCallThunk, pfnHelper) == 0x20, "CComStdCallThunk::pfnHelper");

// GUID constants, read from the retail .rdata the code references.
const GUID kIID_IUnknown = {0x00000000, 0x0000, 0x0000, {0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46}};
const GUID kIID_IDispatch = {0x00020400, 0x0000, 0x0000, {0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46}};
const GUID kIID_IPersist = {0x0000010C, 0x0000, 0x0000, {0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46}};
const GUID kIID_IProvideClassInfo2 = {0xA6BC3AC0, 0xDBAA, 0x11CE, {0x9D, 0xE3, 0x00, 0xAA, 0x00, 0x4B, 0xB8, 0x51}};
const GUID kIID_IConnectionPointContainer = {0xB196B284, 0xBAB4, 0x101A, {0xB6, 0x9C, 0x00, 0xAA, 0x00, 0x34, 0x1D, 0x07}};
const GUID kGUID_NULL = {0, 0, 0, {0, 0, 0, 0, 0, 0, 0, 0}};

inline S_DHCS* Self(void* p) { return static_cast<S_DHCS*>(p); }

// Scalar deleting destructor, vftable slot 7.  Retail 0x20fdc0 (mfc140u, not exported):
// call ~CDHtmlControlSink; if (flags & 1) operator delete(this, 0x60) -- the sized delete
// helper tail-jumps to 0x27c0, the free-forwarding operator delete body.
void* MS_ABI DHCS_ScalarDeletingDtor(void* pThis, unsigned int flags) {
    impl___1CDHtmlControlSink__UEAA_XZ(pThis);
    if (flags & 1)
        impl___3_YAXPEAX_Z(pThis);
    return pThis;
}

void* const g_DHCS_Vtbl[8] = {
    reinterpret_cast<void*>(&impl__QueryInterface_CDHtmlControlSink__UEAAJAEBU_GUID__PEAPEAX_Z),
    reinterpret_cast<void*>(&impl__AddRef_CDHtmlControlSink__UEAAKXZ),
    reinterpret_cast<void*>(&impl__Release_CDHtmlControlSink__UEAAKXZ),
    reinterpret_cast<void*>(&impl__GetTypeInfoCount_CDHtmlControlSink__UEAAJPEAI_Z),
    reinterpret_cast<void*>(&impl__GetTypeInfo_CDHtmlControlSink__UEAAJIKPEAPEAUITypeInfo___Z),
    reinterpret_cast<void*>(&impl__GetIDsOfNames_CDHtmlControlSink__UEAAJAEBU_GUID__PEAPEA_WIKPEAJ_Z),
    reinterpret_cast<void*>(&impl__Invoke_CDHtmlControlSink__UEAAJJAEBU_GUID__KGPEAUtagDISPPARAMS__PEAUtagVARIANT__PEAUtagEXCEPINFO__PEAI_Z),
    reinterpret_cast<void*>(&DHCS_ScalarDeletingDtor),
};

// AtlAdvise, retail 0x21579c (mfc140u, not exported; called from ConnectToControl):
// E_INVALIDARG for a null object, else QI(IConnectionPointContainer) -> FindConnectionPoint
// (slot 4) -> IConnectionPoint::Advise (slot 5), releasing both interfaces.
HRESULT DHCS_AtlAdvise(IUnknown* pUnkCP, IUnknown* pUnk, const IID& iid, DWORD* pdw) {
    if (pUnkCP == nullptr)
        return E_INVALIDARG;
    IConnectionPointContainer* pCPC = nullptr;
    IConnectionPoint* pCP = nullptr;
    HRESULT hr = pUnkCP->QueryInterface(kIID_IConnectionPointContainer, reinterpret_cast<void**>(&pCPC));
    if (SUCCEEDED(hr)) {
        hr = pCPC->FindConnectionPoint(iid, &pCP);
        if (SUCCEEDED(hr))
            hr = pCP->Advise(pUnk, pdw);
    }
    if (pCP) pCP->Release();
    if (pCPC) pCPC->Release();
    return hr;
}

// AtlUnadvise, retail 0x2156d0 (mfc140u, not exported; called from the destructor): as
// DHCS_AtlAdvise but IConnectionPoint::Unadvise (slot 6) with the cookie by value.
HRESULT DHCS_AtlUnadvise(IUnknown* pUnkCP, const IID& iid, DWORD dw) {
    if (pUnkCP == nullptr)
        return E_INVALIDARG;
    IConnectionPointContainer* pCPC = nullptr;
    IConnectionPoint* pCP = nullptr;
    HRESULT hr = pUnkCP->QueryInterface(kIID_IConnectionPointContainer, reinterpret_cast<void**>(&pCPC));
    if (SUCCEEDED(hr)) {
        hr = pCPC->FindConnectionPoint(iid, &pCP);
        if (SUCCEEDED(hr))
            hr = pCP->Unadvise(dw);
    }
    if (pCP) pCP->Release();
    if (pCPC) pCPC->Release();
    return hr;
}

// AtlGetUserDefinedType, retail 0x2155b4 (mfc140u, not exported; recursive).  Transcribed:
//   pTI == NULL -> AtlThrow(E_INVALIDARG), which MFC's AtlThrow (0x333c) turns into
//                  AfxThrowOleException(hr) for anything but E_OUTOFMEMORY;
//   GetRefTypeInfo (slot 14) fails -> VT_USERDEFINED;
//   GetTypeAttr (slot 3) succeeds with typekind TKIND_ALIAS/TKIND_ENUM ->
//       tdescAlias.vt == VT_USERDEFINED ? recurse on tdescAlias.hreftype
//       : switch(typekind) { ENUM: VT_I4; INTERFACE: VT_UNKNOWN; DISPATCH: VT_DISPATCH;
//                            default: tdescAlias.vt }
//   ReleaseTypeAttr (slot 19) when an attr was returned; Release the ref'd type info.
VARTYPE DHCS_AtlGetUserDefinedType(ITypeInfo* pTI, HREFTYPE hrt) {
    if (pTI == nullptr)
        impl__AfxThrowOleException__YAXJ_Z(E_INVALIDARG);
    ITypeInfo* spTypeInfo = nullptr;
    VARTYPE vt = VT_USERDEFINED;
    HRESULT hr = pTI->GetRefTypeInfo(hrt, &spTypeInfo);
    if (FAILED(hr)) {
        if (spTypeInfo) spTypeInfo->Release();
        return vt;
    }
    TYPEATTR* pta = nullptr;
    hr = spTypeInfo->GetTypeAttr(&pta);
    if (SUCCEEDED(hr) && pta && (pta->typekind == TKIND_ALIAS || pta->typekind == TKIND_ENUM)) {
        if (pta->tdescAlias.vt == VT_USERDEFINED) {
            vt = DHCS_AtlGetUserDefinedType(spTypeInfo, pta->tdescAlias.hreftype);
        } else {
            switch (pta->typekind) {
            case TKIND_ENUM: vt = VT_I4; break;
            case TKIND_INTERFACE: vt = VT_UNKNOWN; break;
            case TKIND_DISPATCH: vt = VT_DISPATCH; break;
            default: vt = pta->tdescAlias.vt; break;
            }
        }
    }
    if (pta)
        spTypeInfo->ReleaseTypeAttr(pta);
    spTypeInfo->Release();
    return vt;
}

// AtlGetObjectSourceInterface (atlcom.h), inlined into retail ConnectToControl; transcribed
// from that inlined copy, which matches the atlcom.h source statement for statement.
// The four output pointers are members of the sink, so the E_POINTER guard retail keeps is
// dead code and is omitted.
HRESULT DHCS_GetObjectSourceInterface(IUnknown* punkObj, GUID* plibid, IID* piid,
                                      WORD* pdwMajor, WORD* pdwMinor) {
    *plibid = kGUID_NULL;
    *piid = kGUID_NULL;  // IID_NULL == GUID_NULL: retail copies the same 16 zero bytes to both
    *pdwMajor = 0;
    *pdwMinor = 0;

    HRESULT hr = E_FAIL;
    if (punkObj == nullptr)
        return hr;

    IDispatch* spDispatch = nullptr;
    ITypeInfo* spTypeInfo = nullptr;
    ITypeLib* spTypeLib = nullptr;
    hr = punkObj->QueryInterface(kIID_IDispatch, reinterpret_cast<void**>(&spDispatch));
    if (SUCCEEDED(hr)) {
        hr = spDispatch->GetTypeInfo(0, 0, &spTypeInfo);                        // slot 4
        if (SUCCEEDED(hr)) {
            hr = spTypeInfo->GetContainingTypeLib(&spTypeLib, nullptr);          // slot 18
            if (SUCCEEDED(hr)) {
                TLIBATTR* plibAttr = nullptr;
                hr = spTypeLib->GetLibAttr(&plibAttr);                           // slot 7
                if (SUCCEEDED(hr)) {
                    // Checked::memcpy_s(plibid, 16, &plibAttr->guid, 16).  Its null-source
                    // path (memset dst, errno = EINVAL, _invalid_parameter_noinfo,
                    // AfxThrowInvalidArgException) is reached only if GetLibAttr succeeds yet
                    // returns NULL.  DEVIATION: the CRT invalid-parameter handler call is not reproduced.
                    if (plibAttr == nullptr) {
                        std::memset(plibid, 0, sizeof(GUID));
                        errno = EINVAL;
                        impl__AfxThrowInvalidArgException__YAXXZ();
                    }
                    std::memcpy(plibid, &plibAttr->guid, sizeof(GUID));
                    *pdwMajor = plibAttr->wMajorVerNum;
                    *pdwMinor = plibAttr->wMinorVerNum;
                    spTypeLib->ReleaseTLibAttr(plibAttr);                        // slot 12
                    IProvideClassInfo2* spProvideClassInfo = nullptr;
                    hr = punkObj->QueryInterface(kIID_IProvideClassInfo2,
                                                 reinterpret_cast<void**>(&spProvideClassInfo));
                    if (SUCCEEDED(hr) && spProvideClassInfo != nullptr) {
                        hr = spProvideClassInfo->GetGUID(GUIDKIND_DEFAULT_SOURCE_DISP_IID, piid);  // slot 4
                    } else {
                        ITypeInfo* spInfoCoClass = nullptr;
                        IPersist* spPersist = nullptr;
                        CLSID clsid;
                        hr = punkObj->QueryInterface(kIID_IPersist, reinterpret_cast<void**>(&spPersist));
                        if (SUCCEEDED(hr)) {
                            hr = spPersist->GetClassID(&clsid);                  // slot 3
                            if (SUCCEEDED(hr)) {
                                hr = spTypeLib->GetTypeInfoOfGuid(clsid, &spInfoCoClass);  // slot 6
                                if (SUCCEEDED(hr)) {
                                    TYPEATTR* pAttr = nullptr;
                                    spInfoCoClass->GetTypeAttr(&pAttr);          // slot 3, hr ignored
                                    if (pAttr != nullptr) {
                                        HREFTYPE hRef;
                                        for (int i = 0; i < pAttr->cImplTypes; i++) {
                                            int nType;
                                            hr = spInfoCoClass->GetImplTypeFlags(i, &nType);  // slot 9
                                            if (SUCCEEDED(hr)) {
                                                if (nType == (IMPLTYPEFLAG_FDEFAULT | IMPLTYPEFLAG_FSOURCE)) {
                                                    hr = spInfoCoClass->GetRefTypeOfImplType(i, &hRef);  // slot 8
                                                    if (SUCCEEDED(hr)) {
                                                        ITypeInfo* spInfo = nullptr;
                                                        hr = spInfoCoClass->GetRefTypeInfo(hRef, &spInfo);  // slot 14
                                                        if (SUCCEEDED(hr)) {
                                                            // DEVIATION: retail leaves pAttrIF
                                                            // uninitialised (as atlcom.h does);
                                                            // it is NULL-initialised here.
                                                            TYPEATTR* pAttrIF = nullptr;
                                                            spInfo->GetTypeAttr(&pAttrIF);
                                                            if (pAttrIF != nullptr) {
                                                                std::memcpy(piid, &pAttrIF->guid, sizeof(GUID));
                                                                spInfo->ReleaseTypeAttr(pAttrIF);  // slot 19
                                                            }
                                                        }
                                                        if (spInfo) spInfo->Release();
                                                    }
                                                    break;
                                                }
                                            }
                                        }
                                        spInfoCoClass->ReleaseTypeAttr(pAttr);
                                    }
                                }
                            }
                        }
                        if (spPersist) spPersist->Release();
                        if (spInfoCoClass) spInfoCoClass->Release();
                    }
                    if (spProvideClassInfo) spProvideClassInfo->Release();
                }
            }
            // CComPtr destructors: each smart pointer is released whenever it is non-null,
            // whether or not the call that filled it succeeded -- the release blocks at
            // 0x210299 / 0x2102b0 / 0x2102c7 inside ConnectToControl (0x20fec0, mfc140u) test
            // the pointer, not the HRESULT.
            if (spTypeLib) spTypeLib->Release();
        }
        if (spTypeInfo) spTypeInfo->Release();
    }
    if (spDispatch) spDispatch->Release();
    return hr;
}

}  // namespace

// CDHtmlControlSink::CDHtmlControlSink(IUnknown*, CDHtmlSinkHandler*, LPCTSTR, DWORD_PTR) --
// retail RVA 0x20fe00 (mfc140u).  Installs the vftable, zeroes m_spunkObj / m_spTypeInfo /
// m_dwCookie, stores m_pHandler, m_szControlId and m_dwThunkOffset, then calls
// ConnectToControl(punkObj) and discards its HRESULT.  m_iid / m_libid / the version words
// are not touched here (ConnectToControl initialises them).
// Symbol: ??0CDHtmlControlSink@@QEAA@PEAUIUnknown@@PEAVCDHtmlSinkHandler@@PEB_W_K@Z
extern "C" void* MS_ABI impl___0CDHtmlControlSink__QEAA_PEAUIUnknown__PEAVCDHtmlSinkHandler__PEB_W_K_Z(
    void* pThis, IUnknown* punkObj, void* pHandler, const wchar_t* szControlId,
    unsigned long long dwThunkOffset) {
    S_DHCS* s = Self(pThis);
    s->vfptr = const_cast<void**>(g_DHCS_Vtbl);
    s->m_spunkObj = nullptr;
    s->m_spTypeInfo = nullptr;
    s->m_dwCookie = 0;
    s->m_pHandler = pHandler;
    s->m_szControlId = szControlId;
    s->m_dwThunkOffset = dwThunkOffset;
    impl__ConnectToControl_CDHtmlControlSink__QEAAJPEAUIUnknown___Z(pThis, punkObj);
    return pThis;
}

// CDHtmlControlSink::CDHtmlControlSink() -- retail RVA 0x20fd70 (mfc140u).  Installs the
// vftable, zeroes m_spunkObj, m_spTypeInfo, m_dwCookie, m_pHandler, m_dwThunkOffset and
// memset()s m_iid to zero.  Retail leaves m_szControlId, m_libid and the version words
// uninitialised; so does this.
// Symbol: ??0CDHtmlControlSink@@QEAA@XZ
extern "C" void* MS_ABI impl___0CDHtmlControlSink__QEAA_XZ(void* pThis) {
    S_DHCS* s = Self(pThis);
    s->vfptr = const_cast<void**>(g_DHCS_Vtbl);
    s->m_spunkObj = nullptr;
    s->m_spTypeInfo = nullptr;
    s->m_dwCookie = 0;
    s->m_pHandler = nullptr;
    s->m_dwThunkOffset = 0;
    std::memset(&s->m_iid, 0, sizeof(IID));
    return pThis;
}

// CDHtmlControlSink::~CDHtmlControlSink() -- retail RVA 0x20fe50 (mfc140u).  Re-installs the
// vftable; if m_dwCookie != 0, AtlUnadvise(m_spunkObj, m_iid, m_dwCookie) (0x2156d0, result
// ignored, cookie not cleared); then the CComPtr destructors: m_spTypeInfo->Release() and
// m_spunkObj->Release() (IUnknown slot 2) when non-null.
// Symbol: ??1CDHtmlControlSink@@UEAA@XZ
extern "C" void MS_ABI impl___1CDHtmlControlSink__UEAA_XZ(void* pThis) {
    S_DHCS* s = Self(pThis);
    s->vfptr = const_cast<void**>(g_DHCS_Vtbl);
    if (s->m_dwCookie != 0)
        DHCS_AtlUnadvise(s->m_spunkObj, s->m_iid, s->m_dwCookie);
    if (s->m_spTypeInfo)
        s->m_spTypeInfo->Release();
    if (s->m_spunkObj)
        s->m_spunkObj->Release();
}

// CDHtmlControlSink::AddRef / Release -- the vftable's slots 1 and 2 both point at the
// shared body RVA 0x3a60 (mfc140u) `mov $1,%eax; ret` (the export map names that address
// after another symbol it was folded with).  The sink is not reference counted.
// Symbol: ?AddRef@CDHtmlControlSink@@UEAAKXZ
extern "C" unsigned long MS_ABI impl__AddRef_CDHtmlControlSink__UEAAKXZ(void* pThis) {
    (void)pThis;
    return 1;
}

// CDHtmlControlSink::ConnectToControl(IUnknown* punkObj) -- retail RVA 0x20fec0 (mfc140u).
//   m_spunkObj = punkObj;                  (CComPtr assignment: AddRef new, Release old,
//                                           skipped when unchanged)
//   hr = AtlGetObjectSourceInterface(punkObj, &m_libid, &m_iid, &m_wMajor, &m_wMinor);
//   if (SUCCEEDED(hr)) {
//       CComPtr<ITypeLib> spTypeLib;
//       hr = LoadRegTypeLib(m_libid, m_wMajor, m_wMinor, 0x400 /*LANG_USER_DEFAULT*/, &spTypeLib);
//       if (SUCCEEDED(hr)) {
//           hr = spTypeLib->GetTypeInfoOfGuid(m_iid, &m_spTypeInfo);   // slot 6, straight
//                               into the member's storage: a previous m_spTypeInfo is
//                               overwritten without Release, exactly as retail does
//           if (SUCCEEDED(hr))
//               hr = AtlAdvise(punkObj, (IUnknown*)this, m_iid, &m_dwCookie);   // 0x21579c
//       }
//   }
//   return hr;
// LoadRegTypeLib is OLEAUT32 ordinal 162 (IAT slot resolved with iat.py).
// Symbol: ?ConnectToControl@CDHtmlControlSink@@QEAAJPEAUIUnknown@@@Z
extern "C" long MS_ABI impl__ConnectToControl_CDHtmlControlSink__QEAAJPEAUIUnknown___Z(void* pThis, IUnknown* punkObj) {
    S_DHCS* s = Self(pThis);
    if (s->m_spunkObj != punkObj) {
        if (punkObj)
            punkObj->AddRef();
        IUnknown* pOld = s->m_spunkObj;
        s->m_spunkObj = punkObj;
        if (pOld)
            pOld->Release();
    }
    HRESULT hr = DHCS_GetObjectSourceInterface(punkObj, &s->m_libid, &s->m_iid, &s->m_wMajor, &s->m_wMinor);
    if (FAILED(hr))
        return hr;
    ITypeLib* spTypeLib = nullptr;
    hr = LoadRegTypeLib(s->m_libid, s->m_wMajor, s->m_wMinor, 0x400, &spTypeLib);
    if (SUCCEEDED(hr)) {
        hr = spTypeLib->GetTypeInfoOfGuid(s->m_iid, &s->m_spTypeInfo);
        if (SUCCEEDED(hr))
            hr = DHCS_AtlAdvise(punkObj, static_cast<IUnknown*>(pThis), s->m_iid, &s->m_dwCookie);
    }
    if (spTypeLib)
        spTypeLib->Release();
    return hr;
}

// CDHtmlControlSink::GetFuncInfoFromId(const IID&, DISPID, LCID, _ATL_FUNC_INFO&) -- retail
// RVA 0x210780 (mfc140u).  IDispEventImpl::GetFuncInfoFromId (atlcom.h) run against
// m_spTypeInfo; the iid and lcid arguments are never read.  Transcribed:
//   if (!m_spTypeInfo) return E_FAIL;
//   GetTypeAttr (slot 3) -- on failure return its hr;
//   for i < pAttr->cFuncs: GetFuncDesc(i) (slot 5) -- on failure return its hr (pAttr is
//       leaked, as retail/ATL do); stop at memid == dispidMember, else ReleaseFuncDesc
//       (slot 20) and continue;
//   ReleaseTypeAttr (slot 19); no match -> E_FAIL; cParams > 8 -> E_FAIL (the FUNCDESC is
//       leaked, as retail does);
//   per parameter: vt, with VT_PTR -> lptdesc->vt | VT_BYREF, VT_SAFEARRAY -> lptdesc->vt |
//       VT_ARRAY, VT_USERDEFINED -> AtlGetUserDefinedType(m_spTypeInfo, hreftype), applied
//       as three sequential tests exactly as retail does;
//   return vt: VT_INT -> VT_I4, VT_UINT -> VT_UI4, VT_VOID -> VT_EMPTY, VT_HRESULT -> VT_ERROR;
//   info = { callconv, vtReturn, cParams }; ReleaseFuncDesc; return S_OK.
// Symbol: ?GetFuncInfoFromId@CDHtmlControlSink@@QEAAJAEBU_GUID@@JKAEAU_ATL_FUNC_INFO@ATL@@@Z
extern "C" long MS_ABI impl__GetFuncInfoFromId_CDHtmlControlSink__QEAAJAEBU_GUID__JKAEAU_ATL_FUNC_INFO_ATL___Z(
    void* pThis, const GUID* iid, long dispidMember, unsigned long lcid, DHCS_ATL_FUNC_INFO* info) {
    (void)iid;
    (void)lcid;
    ITypeInfo* pTypeInfo = Self(pThis)->m_spTypeInfo;
    if (pTypeInfo == nullptr)
        return E_FAIL;
    FUNCDESC* pFuncDesc = nullptr;
    TYPEATTR* pAttr;
    HRESULT hr = pTypeInfo->GetTypeAttr(&pAttr);
    if (FAILED(hr))
        return hr;
    for (int i = 0; i < pAttr->cFuncs; i++) {
        hr = pTypeInfo->GetFuncDesc(i, &pFuncDesc);
        if (FAILED(hr))
            return hr;
        if (pFuncDesc->memid == dispidMember)
            break;
        pTypeInfo->ReleaseFuncDesc(pFuncDesc);
        pFuncDesc = nullptr;
    }
    pTypeInfo->ReleaseTypeAttr(pAttr);
    if (pFuncDesc == nullptr)
        return E_FAIL;
    if (pFuncDesc->cParams > 8)
        return E_FAIL;
    for (int i = 0; i < pFuncDesc->cParams; i++) {
        const TYPEDESC& td = pFuncDesc->lprgelemdescParam[i].tdesc;
        info->pVarTypes[i] = td.vt;
        if (info->pVarTypes[i] == VT_PTR)
            info->pVarTypes[i] = static_cast<VARTYPE>(td.lptdesc->vt | VT_BYREF);
        if (info->pVarTypes[i] == VT_SAFEARRAY)
            info->pVarTypes[i] = static_cast<VARTYPE>(td.lptdesc->vt | VT_ARRAY);
        if (info->pVarTypes[i] == VT_USERDEFINED)
            info->pVarTypes[i] = DHCS_AtlGetUserDefinedType(pTypeInfo, td.hreftype);
    }
    VARTYPE vtReturn = pFuncDesc->elemdescFunc.tdesc.vt;
    switch (vtReturn) {
    case VT_INT: vtReturn = VT_I4; break;
    case VT_UINT: vtReturn = VT_UI4; break;
    case VT_VOID: vtReturn = VT_EMPTY; break;
    case VT_HRESULT: vtReturn = VT_ERROR; break;
    default: break;
    }
    info->vtReturn = vtReturn;
    info->cc = pFuncDesc->callconv;
    info->nParams = pFuncDesc->cParams;
    pTypeInfo->ReleaseFuncDesc(pFuncDesc);
    return S_OK;
}

// CDHtmlControlSink::GetIDsOfNames -- vftable slot 5 is the shared body RVA 0xf4d0 (mfc140u)
// `mov $E_NOTIMPL,%eax; ret`: nothing is written to rgDispId.
// Symbol: ?GetIDsOfNames@CDHtmlControlSink@@UEAAJAEBU_GUID@@PEAPEA_WIKPEAJ@Z
extern "C" long MS_ABI impl__GetIDsOfNames_CDHtmlControlSink__UEAAJAEBU_GUID__PEAPEA_WIKPEAJ_Z(
    void* pThis, const GUID* riid, wchar_t** rgszNames, unsigned int cNames, unsigned long lcid, long* rgDispId) {
    (void)pThis; (void)riid; (void)rgszNames; (void)cNames; (void)lcid; (void)rgDispId;
    return E_NOTIMPL;
}

// CDHtmlControlSink::GetTypeInfo -- retail RVA 0x20f860 (mfc140u): *ppTInfo = NULL (written
// without a null check); return E_NOTIMPL.
// Symbol: ?GetTypeInfo@CDHtmlControlSink@@UEAAJIKPEAPEAUITypeInfo@@@Z
extern "C" long MS_ABI impl__GetTypeInfo_CDHtmlControlSink__UEAAJIKPEAPEAUITypeInfo___Z(
    void* pThis, unsigned int iTInfo, unsigned long lcid, ITypeInfo** ppTInfo) {
    (void)pThis; (void)iTInfo; (void)lcid;
    *ppTInfo = nullptr;
    return E_NOTIMPL;
}

// CDHtmlControlSink::GetTypeInfoCount -- retail RVA 0x20f850 (mfc140u): *pctinfo = 0
// (no null check); return E_NOTIMPL.
// Symbol: ?GetTypeInfoCount@CDHtmlControlSink@@UEAAJPEAI@Z
extern "C" long MS_ABI impl__GetTypeInfoCount_CDHtmlControlSink__UEAAJPEAI_Z(void* pThis, unsigned int* pctinfo) {
    (void)pThis;
    *pctinfo = 0;
    return E_NOTIMPL;
}

// CDHtmlControlSink::GetUserDefinedType(ITypeInfo*, HREFTYPE) -- retail RVA 0x210950
// (mfc140u): drops `this` and tail-jumps to AtlGetUserDefinedType (0x2155b4).
// Symbol: ?GetUserDefinedType@CDHtmlControlSink@@QEAAGPEAUITypeInfo@@K@Z
extern "C" unsigned short MS_ABI impl__GetUserDefinedType_CDHtmlControlSink__QEAAGPEAUITypeInfo__K_Z(
    void* pThis, ITypeInfo* pTI, unsigned long hrt) {
    (void)pThis;
    return DHCS_AtlGetUserDefinedType(pTI, hrt);
}

// CDHtmlControlSink::InvokeFromFuncInfo(DHEVTFUNCCONTROL, _ATL_FUNC_INFO&, DISPPARAMS*,
// VARIANT*) -- retail RVA 0x210560 (mfc140u).  The 16-byte multiple-inheritance
// pointer-to-member is passed by hidden reference (the Invoke call site passes the address of
// a stack copy).  Transcribed:
//   if (m_pHandler == NULL)  return E_FAIL;
//   if (info.nParams < 0)     return E_INVALIDARG;
//   pVarArgs = info.nParams ? _ATL_SAFE_ALLOCA(nParams * 8) : NULL;
//   if (pVarArgs == NULL)     return E_OUTOFMEMORY;
//        -- NOTE, retail quirk reproduced as-is: with info.nParams == 0 pVarArgs stays NULL
//           and retail returns E_OUTOFMEMORY without calling the handler (the `test %ax,%ax;
//           je` to the NULL path, then `test %rdi,%rdi` -> 0x8007000E).
//   pVarArgs[i] = &pdispparams->rgvarg[nParams - i - 1];
//   CComStdCallThunk thunk = { &pfnHelper, m_pHandler - m_dwThunkOffset, pEvent,
//                              CComStdCallThunkHelper };
//   CComVariant tmpResult;  (memset 0x18 + VariantInit)
//   hr = DispCallFunc(&thunk, 0, info.cc, info.vtReturn, nParams, info.pVarTypes, pVarArgs,
//                     pvarResult ? pvarResult : &tmpResult);
//   VariantClear(&tmpResult); free any heap block; return hr;
// DispCallFunc / VariantInit / VariantClear are OLEAUT32 ordinals 146 / 8 / 9 (IAT slots
// resolved with iat.py).  DEVIATION: _ATL_SAFE_ALLOCA puts blocks of <= 0x400 bytes on the
// stack (after a stack-availability probe) and larger ones on the malloc heap; here blocks of
// <= 0x400 bytes use a fixed local array and larger ones malloc, with the same
// E_OUTOFMEMORY result when malloc fails.  The overflow checks retail performs on
// nParams * 8 cannot fire for a SHORT count and are omitted.
// Symbol: ?InvokeFromFuncInfo@CDHtmlControlSink@@QEAAJP8CDHtmlSinkHandler@@EAAXXZAEAU_ATL_FUNC_INFO@ATL@@PEAUtagDISPPARAMS@@PEAUtagVARIANT@@@Z
extern "C" long MS_ABI impl__InvokeFromFuncInfo_CDHtmlControlSink__QEAAJP8CDHtmlSinkHandler__EAAXXZAEAU_ATL_FUNC_INFO_ATL__PEAUtagDISPPARAMS__PEAUtagVARIANT___Z(
    void* pThis, const void* pEvent, DHCS_ATL_FUNC_INFO* info, DISPPARAMS* pdispparams, VARIANT* pvarResult) {
    S_DHCS* s = Self(pThis);
    if (s->m_pHandler == nullptr)
        return E_FAIL;
    const int nParams = info->nParams;
    if (nParams < 0)
        return E_INVALIDARG;

    VARIANTARG* localArgs[0x400 / sizeof(VARIANTARG*)];
    void* heapBlock = nullptr;
    VARIANTARG** pVarArgs = nullptr;
    if (nParams != 0) {
        const size_t cb = static_cast<size_t>(nParams) * sizeof(VARIANTARG*);
        if (cb <= sizeof(localArgs)) {
            pVarArgs = localArgs;
        } else {
            heapBlock = std::malloc(cb);
            pVarArgs = static_cast<VARIANTARG**>(heapBlock);
        }
    }
    if (pVarArgs == nullptr)
        return E_OUTOFMEMORY;

    for (int i = 0; i < nParams; i++)
        pVarArgs[i] = &pdispparams->rgvarg[nParams - i - 1];

    DHCS_StdCallThunk thunk;
    thunk.pfnHelper = &openmfc_DHCS_StdCallThunkHelper;
    thunk.pVTable = &thunk.pfnHelper;
    thunk.pThis = static_cast<char*>(s->m_pHandler) - s->m_dwThunkOffset;
    std::memcpy(thunk.pfn, pEvent, sizeof(thunk.pfn));

    VARIANT tmpResult;
    std::memset(&tmpResult, 0, sizeof(tmpResult));
    VariantInit(&tmpResult);
    HRESULT hr = DispCallFunc(&thunk, 0, info->cc, info->vtReturn, static_cast<UINT>(nParams),
                              info->pVarTypes, pVarArgs, pvarResult ? pvarResult : &tmpResult);
    VariantClear(&tmpResult);
    std::free(heapBlock);
    return hr;
}

// CDHtmlControlSink::Invoke -- retail RVA 0x210450 (mfc140u).  Transcribed:
//   const DHtmlEventMapEntry* pEventMap = m_pHandler->GetDHtmlEventMap();   // slot 7 (+0x38),
//                                         no null check on m_pHandler or on the map
//   for (i = 0; pEventMap[i].nType != DHTMLEVENTMAPENTRY_END; i++)
//       if (nType == DHTMLEVENTMAPENTRY_CONTROL && dispId == dispidMember &&
//           !_tcscmp(pEventMap[i].szName, m_szControlId))      // wcscmp in mfc140u (iatu.py)
//       {
//           _ATL_FUNC_INFO info;
//           if (FAILED(GetFuncInfoFromId(m_iid, dispidMember, lcid, info)))   // m_iid, not riid
//               return S_OK;
//           return InvokeFromFuncInfo(pEventMap[i].pfnEventFunc, info, pdispparams, pvarResult);
//       }
//   return DISP_E_MEMBERNOTFOUND;
// riid, wFlags, pExcepInfo and puArgErr are never read.
// Symbol: ?Invoke@CDHtmlControlSink@@UEAAJJAEBU_GUID@@KGPEAUtagDISPPARAMS@@PEAUtagVARIANT@@PEAUtagEXCEPINFO@@PEAI@Z
extern "C" long MS_ABI impl__Invoke_CDHtmlControlSink__UEAAJJAEBU_GUID__KGPEAUtagDISPPARAMS__PEAUtagVARIANT__PEAUtagEXCEPINFO__PEAI_Z(
    void* pThis, long dispidMember, const GUID* riid, unsigned long lcid, unsigned short wFlags,
    DISPPARAMS* pdispparams, VARIANT* pvarResult, EXCEPINFO* pExcepInfo, unsigned int* puArgErr) {
    (void)riid; (void)wFlags; (void)pExcepInfo; (void)puArgErr;
    S_DHCS* s = Self(pThis);
    using PFN_GetDHtmlEventMap = const DHCS_EventMapEntry* (MS_ABI*)(void*);
    void* pHandler = s->m_pHandler;
    const DHCS_EventMapEntry* pEventMap =
        reinterpret_cast<PFN_GetDHtmlEventMap>((*static_cast<void***>(pHandler))[7])(pHandler);
    for (int i = 0; pEventMap[i].nType != kDHTMLEVENTMAPENTRY_END; i++) {
        const DHCS_EventMapEntry& e = pEventMap[i];
        if (e.nType == kDHTMLEVENTMAPENTRY_CONTROL && e.dispId == dispidMember &&
            std::wcscmp(e.szName, s->m_szControlId) == 0) {
            DHCS_ATL_FUNC_INFO info;
            if (FAILED(impl__GetFuncInfoFromId_CDHtmlControlSink__QEAAJAEBU_GUID__JKAEAU_ATL_FUNC_INFO_ATL___Z(
                    pThis, &s->m_iid, dispidMember, lcid, &info)))
                return S_OK;
            unsigned char pfn[16];
            std::memcpy(pfn, e.pfnEventFunc, sizeof(pfn));
            return impl__InvokeFromFuncInfo_CDHtmlControlSink__QEAAJP8CDHtmlSinkHandler__EAAXXZAEAU_ATL_FUNC_INFO_ATL__PEAUtagDISPPARAMS__PEAUtagVARIANT___Z(
                pThis, pfn, &info, pdispparams, pvarResult);
        }
    }
    return DISP_E_MEMBERNOTFOUND;
}

// CDHtmlControlSink::QueryInterface -- the vftable's slot 0, RVA 0x2103c0 (mfc140u; the
// export map has no name for it, the body is byte-identical to the one the ANSI map names at
// 0x20e690).  ppvObj == NULL -> E_POINTER; *ppvObj = NULL; riid equal (memcmp, 16 bytes) to
// IID_IUnknown, IID_IDispatch or m_iid -> *ppvObj = this, S_OK -- with NO AddRef (AddRef is a
// constant anyway); otherwise E_NOINTERFACE.
// Symbol: ?QueryInterface@CDHtmlControlSink@@UEAAJAEBU_GUID@@PEAPEAX@Z
extern "C" long MS_ABI impl__QueryInterface_CDHtmlControlSink__UEAAJAEBU_GUID__PEAPEAX_Z(void* pThis, const GUID* riid, void** ppvObj) {
    if (ppvObj == nullptr)
        return E_POINTER;
    *ppvObj = nullptr;
    if (std::memcmp(riid, &kIID_IUnknown, sizeof(GUID)) != 0 &&
        std::memcmp(riid, &kIID_IDispatch, sizeof(GUID)) != 0 &&
        std::memcmp(riid, &Self(pThis)->m_iid, sizeof(GUID)) != 0)
        return E_NOINTERFACE;
    *ppvObj = pThis;
    return S_OK;
}

// CDHtmlControlSink::Release -- see AddRef: the same shared `return 1` body (RVA 0x3a60,
// mfc140u).
// Symbol: ?Release@CDHtmlControlSink@@UEAAKXZ
extern "C" unsigned long MS_ABI impl__Release_CDHtmlControlSink__UEAAKXZ(void* pThis) {
    (void)pThis;
    return 1;
}
