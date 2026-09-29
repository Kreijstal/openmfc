// CAsyncPropExchange — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp
//
// MFC's private "asynchronous" CPropExchange: an exchanger that reports
// m_bAsync = TRUE.  The PX_* wrappers test +0xc and, when it is non-zero, return
// without calling the exchanger's virtual Exchange* slot (documented for all of them
// in core/ole/CPropExchange.cpp; re-checked here for the 4-argument PX_ULong overload,
// ordinal 11975 -> RVA 0x1f0c10 (mfc140u), an ICF-shared body:
// `cmpl $0x0,0xc(%rcx) ; je ... ; mov $0x1,%eax` before any vtable load).
// No header in include/openmfc declares the class, and neither does any shipping
// header under ~/msvc/.../atlmfc/include (grepped; MFC's private sources are not on
// this host), so the layout is pinned in-file below.  Every body was
// transcribed from mfc140u.dll itself; all RVAs below are mfc140u RVAs.
//
// ---------------------------------------------------------------------------
// Retail object layout, pinned by the retail ctor ??0CAsyncPropExchange@@QEAA@K@Z
// (RVA 0x1f0b00, mfc140u):
//   lea 0x180324a98,%rax ; mov %rax,(%rcx)   -> vptr
//   mov %edx,0x10(%rcx)                      -> CPropExchange::m_dwVersion = dwVersion
//   mov $1,%eax ; mov %eax,0xc(%rcx)         -> CPropExchange::m_bAsync   = TRUE
//              ; mov %eax,0x8(%rcx)          -> CPropExchange::m_bLoading = TRUE
//   mov %rcx,%rax ; ret
// and by the scalar deleting destructor (vtable slot 5, RVA 0x1f09f0, unexported),
// which frees with `mov $0x18,%edx` -> sizeof == 0x18, i.e. no members beyond the
// CPropExchange base (whose layout core/ole/CPropExchange.cpp pins the same way).
//
// Retail vftable (0x180324a98 in mfc140u), read slot by slot with vtdump_u.py:
//   0 +0x00  ExchangeVersion         0x1f0b20  (CAsyncPropExchange's own)
//   1 +0x08  ExchangeProp            0x71e0
//   2 +0x10  ExchangeBlobProp        0x71e0
//   3 +0x18  ExchangeFontProp        0x71e0
//   4 +0x20  ExchangePersistentProp  0x71e0
//   5 +0x28  scalar deleting dtor    0x1f09f0  (unexported)
// Slots 1-4 are the SAME body: /OPT:ICF folded all four overrides onto the shared
// `xor %eax,%eax ; ret` at RVA 0x71e0 (mfc140u), which the RVA map names after an
// unrelated export that folded onto it too (?AddRef@COleUILinkInfo@@UEAAKXZ).
// The export address table agrees: ordinals 4405, 4411, 4416 and 4421 (the four
// Exchange*Prop exports of this class, resolved via mfc_complete_ordinal_mapping.json
// and the mfc140u EAT with the scratchpad ordrva.py) all point at 0x71e0.  That is
// why disas.py reports those four names as NOT FOUND: the RVA map keeps one name
// per RVA.
//
// The ctor below installs kAsyncPropExchangeVtbl, an in-file table of the same
// six entries in the same order.  Deviation: retail's vftable is preceded by an
// RTTI CompleteObjectLocator pointer (the qword at 0x180324a90 in mfc140u points at
// RVA 0x3640e8, a COL with signature 1 whose type descriptor names
// .?AVCAsyncPropExchange@@); the in-file table has no such slot, so MSVC-side
// typeid/dynamic_cast on an object built by this ctor is not supported.
// ---------------------------------------------------------------------------

#include "detail/ManualSmallStubImplementationsSupport.h"

#include <ocidl.h>
#include <olectl.h>

// Thunks defined elsewhere in the tree (definition checked by grep).
extern "C" void MS_ABI impl___3_YAXPEAX_Z(void* ptr);                           // detail/MemcoreSupport.cpp

// This file's own exports, forward-declared for the in-file vtable.
extern "C" int MS_ABI impl__ExchangeVersion_CAsyncPropExchange__UEAAHAEAKKH_Z(
    void* pThis, unsigned long* pdwVersionLoaded, unsigned long dwVersionDefault, int bConvert);
extern "C" int MS_ABI impl__ExchangeProp_CAsyncPropExchange__UEAAHPEB_WGPEAXPEBX_Z(
    void* pThis, const wchar_t* pszPropName, unsigned short vtProp, void* pvProp, const void* pvDefault);
extern "C" int MS_ABI impl__ExchangeBlobProp_CAsyncPropExchange__UEAAHPEB_WPEAPEAXPEAX_Z(
    void* pThis, const wchar_t* pszPropName, HGLOBAL* phBlob, HGLOBAL hBlobDefault);
extern "C" int MS_ABI impl__ExchangeFontProp_CAsyncPropExchange__UEAAHPEB_WAEAVCFontHolder__PEBUtagFONTDESC__PEAUIFontDisp___Z(
    void* pThis, const wchar_t* pszPropName, void* pFontHolder, const FONTDESC* pFontDesc, IFontDisp* pFontDispAmbient);
extern "C" int MS_ABI impl__ExchangePersistentProp_CAsyncPropExchange__UEAAHPEB_WPEAPEAUIUnknown__AEBU_GUID__PEAU2__Z(
    void* pThis, const wchar_t* pszPropName, IUnknown** ppUnk, const GUID* piid, IUnknown* pUnkDefault);

namespace {

// Retail layout (see the header comment for the evidence).
struct AP_CAsyncPropExchange {
    void* const*  vptr;                 // 0x00
    int           m_bLoading;           // 0x08  (CPropExchange)
    int           m_bAsync;             // 0x0c  (CPropExchange)
    unsigned long m_dwVersion;          // 0x10  (CPropExchange)
};
static_assert(offsetof(AP_CAsyncPropExchange, m_bLoading) == 0x08, "CPropExchange::m_bLoading");
static_assert(offsetof(AP_CAsyncPropExchange, m_bAsync) == 0x0c, "CPropExchange::m_bAsync");
static_assert(offsetof(AP_CAsyncPropExchange, m_dwVersion) == 0x10, "CPropExchange::m_dwVersion");
static_assert(sizeof(AP_CAsyncPropExchange) == 0x18, "CAsyncPropExchange is 0x18 bytes (sized delete in the deleting dtor)");

inline AP_CAsyncPropExchange* ap(void* pThis) { return static_cast<AP_CAsyncPropExchange*>(pThis); }

// Scalar deleting destructor, vtable slot 5 -- RVA 0x1f09f0 (mfc140u, unexported):
//   if (flags & 1) sized-delete(this, 0x18);  return this;
// There is no destructor body at all: no code runs between the flag test and the
// delete (afxctl.h declares `virtual ~CPropExchange() = 0 { }`, and the class
// exports no ??1).  The sized-delete callee (RVA 0x2b77b0, mfc140u, unexported) is
// `jmp 0x1800027c0`, and RVA 0x27c0 (mfc140u; the map names it after the ICF-folded
// ??_V@YAXPEAX@Z) is itself `jmp *0x1802c74e8(%rip)`, an import slot that resolves
// in mfc140u to api-ms-win-crt-heap-l1-1-0.dll!free.  OpenMFC's operator delete is
// the exported ??3@YAXPEAX@Z thunk (std::free), which pairs with its malloc-based
// ??2@YAPEAX_K@Z.
void* MS_ABI AsyncScalarDeletingDtor(void* pThis, unsigned int flags) {
    if (flags & 1) {
        impl___3_YAXPEAX_Z(pThis);
    }
    return pThis;
}

// Vtable in retail slot order (see the header comment).
void* const kAsyncPropExchangeVtbl[6] = {
    reinterpret_cast<void*>(&impl__ExchangeVersion_CAsyncPropExchange__UEAAHAEAKKH_Z),
    reinterpret_cast<void*>(&impl__ExchangeProp_CAsyncPropExchange__UEAAHPEB_WGPEAXPEBX_Z),
    reinterpret_cast<void*>(&impl__ExchangeBlobProp_CAsyncPropExchange__UEAAHPEB_WPEAPEAXPEAX_Z),
    reinterpret_cast<void*>(&impl__ExchangeFontProp_CAsyncPropExchange__UEAAHPEB_WAEAVCFontHolder__PEBUtagFONTDESC__PEAUIFontDisp___Z),
    reinterpret_cast<void*>(&impl__ExchangePersistentProp_CAsyncPropExchange__UEAAHPEB_WPEAPEAUIUnknown__AEBU_GUID__PEAU2__Z),
    reinterpret_cast<void*>(&AsyncScalarDeletingDtor),
};

}  // namespace

// Symbol: ??0CAsyncPropExchange@@QEAA@K@Z
// Transcribed from RVA 0x1f0b00 (mfc140u):
//   m_dwVersion = dwVersion;                      ; mov %edx,0x10(%rcx)
//   vptr = CAsyncPropExchange vftable;            ; mov %rax,(%rcx)
//   m_bAsync = TRUE; m_bLoading = TRUE;           ; mov %eax,0xc(%rcx) / 0x8(%rcx), eax = 1
//   return this;
// (The previous parameter list typed the DWORD as `unsigned long long`; the mangled
// name's `K` is a 32-bit unsigned long, and retail reads only %edx.)
extern "C" void* MS_ABI impl___0CAsyncPropExchange__QEAA_K_Z(void* pThis, unsigned long dwVersion) {
    AP_CAsyncPropExchange* self = ap(pThis);
    self->m_dwVersion = dwVersion;
    self->vptr = kAsyncPropExchangeVtbl;
    self->m_bAsync = TRUE;
    self->m_bLoading = TRUE;
    return pThis;
}

// Symbol: ?ExchangeBlobProp@CAsyncPropExchange@@UEAAHPEB_WPEAPEAXPEAX@Z
// Retail: ordinal 4405 -> RVA 0x71e0 (mfc140u), the ICF-folded `xor %eax,%eax ; ret`
// (see the header comment).  Retail returns FALSE and touches nothing -- not this,
// not *phBlob -- so this body is the complete transcription, not a placeholder.
// (Parameter list rewritten from the generated placeholder, which had no this.)
extern "C" int MS_ABI impl__ExchangeBlobProp_CAsyncPropExchange__UEAAHPEB_WPEAPEAXPEAX_Z(
    void* pThis, const wchar_t* pszPropName, HGLOBAL* phBlob, HGLOBAL hBlobDefault) {
    (void)pThis;
    (void)pszPropName;
    (void)phBlob;
    (void)hBlobDefault;
    return FALSE;
}

// Symbol: ?ExchangeFontProp@CAsyncPropExchange@@UEAAHPEB_WAEAVCFontHolder@@PEBUtagFONTDESC@@PEAUIFontDisp@@@Z
// Retail: ordinal 4411 -> RVA 0x71e0 (mfc140u), the ICF-folded `xor %eax,%eax ; ret`
// (see the header comment).  Retail returns FALSE and touches nothing -- in
// particular it does not initialise the CFontHolder -- so this body is the complete
// transcription.  (Parameter list rewritten from the generated placeholder, which
// had no this; the CFontHolder& is passed as one pointer.)
extern "C" int MS_ABI impl__ExchangeFontProp_CAsyncPropExchange__UEAAHPEB_WAEAVCFontHolder__PEBUtagFONTDESC__PEAUIFontDisp___Z(
    void* pThis, const wchar_t* pszPropName, void* pFontHolder, const FONTDESC* pFontDesc, IFontDisp* pFontDispAmbient) {
    (void)pThis;
    (void)pszPropName;
    (void)pFontHolder;
    (void)pFontDesc;
    (void)pFontDispAmbient;
    return FALSE;
}

// Symbol: ?ExchangePersistentProp@CAsyncPropExchange@@UEAAHPEB_WPEAPEAUIUnknown@@AEBU_GUID@@PEAU2@@Z
// Retail: ordinal 4416 -> RVA 0x71e0 (mfc140u), the ICF-folded `xor %eax,%eax ; ret`
// (see the header comment).  Retail returns FALSE and touches nothing -- *ppUnk is
// left as it was -- so this body is the complete transcription.  (Parameter list
// rewritten from the generated placeholder, which had no this; the const GUID& is
// passed as one pointer.)
extern "C" int MS_ABI impl__ExchangePersistentProp_CAsyncPropExchange__UEAAHPEB_WPEAPEAUIUnknown__AEBU_GUID__PEAU2__Z(
    void* pThis, const wchar_t* pszPropName, IUnknown** ppUnk, const GUID* piid, IUnknown* pUnkDefault) {
    (void)pThis;
    (void)pszPropName;
    (void)ppUnk;
    (void)piid;
    (void)pUnkDefault;
    return FALSE;
}

// Symbol: ?ExchangeProp@CAsyncPropExchange@@UEAAHPEB_WGPEAXPEBX@Z
// Retail: ordinal 4421 -> RVA 0x71e0 (mfc140u), the ICF-folded `xor %eax,%eax ; ret`
// (see the header comment).  Retail returns FALSE and touches nothing -- *pvProp is
// left as it was -- so this body is the complete transcription.  (Parameter list
// rewritten from the generated placeholder, which had no this.)
extern "C" int MS_ABI impl__ExchangeProp_CAsyncPropExchange__UEAAHPEB_WGPEAXPEBX_Z(
    void* pThis, const wchar_t* pszPropName, unsigned short vtProp, void* pvProp, const void* pvDefault) {
    (void)pThis;
    (void)pszPropName;
    (void)vtProp;
    (void)pvProp;
    (void)pvDefault;
    return FALSE;
}

// Symbol: ?ExchangeVersion@CAsyncPropExchange@@UEAAHAEAKKH@Z
// Transcribed from RVA 0x1f0b20 (mfc140u):
//   if (m_bLoading != 0) {                        ; cmp %eax(=0),0x8(%rcx) ; je
//       dwVersionLoaded = m_dwVersion;            ; mov 0x10(%rcx),%eax ; mov %eax,(%rdx)
//       return TRUE;
//   }
//   if (!bConvert)                                ; test %r9d,%r9d ; jne
//       dwVersionDefault = dwVersionLoaded;       ; mov (%rdx),%r8d
//   return m_dwVersion == dwVersionDefault;       ; cmp %r8d,0x10(%rcx) ; sete %al
// Unlike CPropExchange::ExchangeVersion (RVA 0x1efb30) this override never calls
// PX_ULong: it only reports or compares the version the ctor was given.
extern "C" int MS_ABI impl__ExchangeVersion_CAsyncPropExchange__UEAAHAEAKKH_Z(
    void* pThis, unsigned long* pdwVersionLoaded, unsigned long dwVersionDefault, int bConvert) {
    AP_CAsyncPropExchange* self = ap(pThis);
    if (self->m_bLoading != 0) {
        *pdwVersionLoaded = self->m_dwVersion;
        return TRUE;
    }
    if (!bConvert) {
        dwVersionDefault = *pdwVersionLoaded;
    }
    return (self->m_dwVersion == dwVersionDefault) ? TRUE : FALSE;
}
