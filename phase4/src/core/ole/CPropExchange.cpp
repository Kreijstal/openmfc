// CPropExchange — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp
//
// Every body in this file was transcribed from the retail mfc140u.dll export
// (ordinal -> export-address-table RVA -> objdump).  The PX_* exports are not in
// mfc140u_rva_symbols.json (the ANSI twin's map only carries the PEBD spellings),
// so each RVA cited below was resolved through mfc_complete_ordinal_mapping.json
// and the mfc140u export address table (scratchpad ordrva.py) and then
// disassembled from mfc140u.dll itself.  Several exports are /OPT:ICF-folded
// onto one body: PX_Color/PX_Long/PX_ULong share 0x1f0bd0 and 0x1f0c10,
// PX_Short/PX_UShort share 0x1f0b50 and 0x1f0b90.
//
// ---------------------------------------------------------------------------
// Retail CPropExchange object layout (mfc140u).  Pinned by three retail bodies:
//   * CPropExchange::ExchangeVersion (RVA 0x1efb30) tests +0x8 (m_bLoading) and
//     reads/writes +0x10 (m_dwVersion);
//   * CAsyncPropExchange::CAsyncPropExchange (RVA 0x1f0b00) stores the vptr at
//     +0x0, 1 at +0x8 and +0xc, and its DWORD argument at +0x10;
//   * CArchivePropExchange::CArchivePropExchange (RVA 0x1efb90) zeroes +0xc..+0x13
//     as one qword, stores m_bLoading at +0x8 and its own CArchive* at +0x18 --
//     so the base class ends at +0x18.
// Every PX_* wrapper tests +0xc (m_bAsync) and never touches the exchanger
// when it is non-zero; all of them then return TRUE except PX_DataPath, whose
// asynchronous path still calls CDataPathProperty::Open when loading (see the
// helper transcription at px_DataPath below).
//
//   +0x00  vptr
//   +0x08  BOOL  m_bLoading
//   +0x0c  BOOL  m_bAsync
//   +0x10  DWORD m_dwVersion
//   sizeof 0x18
//
// Vtable slot order, from the shipping afxctl.h on this host
// (~/msvc/VC/Tools/MSVC/14.51.36231/atlmfc/include/afxctl.h, class CPropExchange,
// virtuals declared in this order) and confirmed by the byte offsets the retail
// wrappers dispatch through:
//   slot 0 +0x00  ExchangeVersion            (non-pure; PX_* never call it)
//   slot 1 +0x08  ExchangeProp               (scalar/string PX_*: `mov 0x8(%rax),%rax`)
//   slot 2 +0x10  ExchangeBlobProp           (PX_Blob:      `mov 0x10(%rax),%rax`)
//   slot 3 +0x18  ExchangeFontProp           (PX_Font:      `mov 0x18(%rax),%rax`)
//   slot 4 +0x20  ExchangePersistentProp     (PX_IUnknown:  `mov 0x20(%rax),%rax`)
//   slot 5 +0x28  ~CPropExchange
// Retail makes every one of these calls through __guard_dispatch_icall_fptr
// (`call *0x1802c7b30(%rip)`, which is the GuardCFDispatchFunctionPointer in the
// mfc140u load-config directory, NOT an import); the real target is the slot
// loaded into %rax on the preceding instruction.
//
// Dispatch caveat (verified, not assumed): the exported constructors of the
// concrete exchangers in this tree (core/ole/CArchivePropExchange.cpp,
// CPropbagPropExchange.cpp, CPropsetPropExchange.cpp, CResetPropExchange.cpp,
// CAsyncPropExchange.cpp) do not yet store a vptr, so an exchanger built by
// those thunks has no vtable to dispatch through.  Retail has no guard here
// either -- the wrappers ARE the virtual call -- so none is added: a CPropExchange
// subclass compiled by an MSVC client (whose vptr points at an MSVC-layout
// vtable in the client image) dispatches exactly as it does under mfc140u.
// ---------------------------------------------------------------------------

#include "detail/ManualSmallStubImplementationsSupport.h"

#include <ocidl.h>
#include <oleauto.h>
#include <wchar.h>

// The out-of-memory path of PX_VBXFontConvert is `call 0x180003160` (mfc140u):
// that stub loads E_OUTOFMEMORY and calls the AtlThrow hook at 0x18000333c, which
// compares the HRESULT against E_OUTOFMEMORY and then CALLS (not tail-jumps; the
// call is followed by int3 because it never returns) ?AfxThrowMemoryException@@YAXXZ
// (RVA 0x2276c0, mfc140u) -- any other HRESULT would go to ?AfxThrowOleException@@YAXJ@Z
// (RVA 0x25f2c0, mfc140u).  The thunk is defined in detail/MfcExceptionsSupport.cpp.
extern "C" void MS_ABI impl__AfxThrowMemoryException__YAXXZ();

namespace {

// Retail CPropExchange layout view (see the header comment for the evidence).
struct PX_CPropExchange {
    void**        vptr;          // 0x00
    int           m_bLoading;    // 0x08
    int           m_bAsync;      // 0x0c
    unsigned long m_dwVersion;   // 0x10
};
static_assert(offsetof(PX_CPropExchange, m_bLoading) == 0x08, "CPropExchange::m_bLoading");
static_assert(offsetof(PX_CPropExchange, m_bAsync) == 0x0c, "CPropExchange::m_bAsync");
static_assert(offsetof(PX_CPropExchange, m_dwVersion) == 0x10, "CPropExchange::m_dwVersion");
static_assert(sizeof(PX_CPropExchange) == 0x18, "CPropExchange is 0x18 bytes (CArchivePropExchange::m_pArchive is at +0x18)");

enum {
    kSlotExchangeProp           = 1,   // +0x08
    kSlotExchangeBlobProp       = 2,   // +0x10
    kSlotExchangeFontProp       = 3,   // +0x18
    kSlotExchangePersistentProp = 4,   // +0x20
};

using ExchangePropFn = int (MS_ABI*)(void* pThis, const wchar_t* pszPropName,
                                     unsigned short vtProp, void* pvProp,
                                     const void* pvDefault);
using ExchangeBlobPropFn = int (MS_ABI*)(void* pThis, const wchar_t* pszPropName,
                                         HGLOBAL* phBlob, HGLOBAL hBlobDefault);
using ExchangeFontPropFn = int (MS_ABI*)(void* pThis, const wchar_t* pszPropName,
                                         void* pFontHolder, const FONTDESC* pFontDesc,
                                         IFontDisp* pFontDispAmbient);
using ExchangePersistentPropFn = int (MS_ABI*)(void* pThis, const wchar_t* pszPropName,
                                               IUnknown** ppUnk, const GUID* piid,
                                               IUnknown* pUnkDefault);

inline PX_CPropExchange* px(void* pPX) { return static_cast<PX_CPropExchange*>(pPX); }

inline int px_ExchangeProp(void* pPX, const wchar_t* pszPropName, unsigned short vt,
                           void* pvProp, const void* pvDefault) {
    return reinterpret_cast<ExchangePropFn>(px(pPX)->vptr[kSlotExchangeProp])(
        pPX, pszPropName, vt, pvProp, pvDefault);
}

// CFontHolder as retail lays it out (afxctl.h: `LPFONT m_pFont` first, then
// m_dwConnectCookie, m_pNotify).  Only +0x0 is touched here; retail
// PX_VBXFontConvert dispatches IFont (not IFontDisp) methods through it, so this
// view is used instead of include/openmfc/afxole.h's CFontHolder, whose m_pFont
// is typed IFontDisp*.
struct PX_CFontHolder {
    IFont* m_pFont;              // 0x00
};

// CPictureHolder as retail lays it out: a single `LPPICTURE m_pPict` at +0x0
// (afxctl.h; the retail ctor at RVA 0x76b20 (mfc140u) is `movq $0,(%rcx)`).
// PX_Picture passes the CPictureHolder& straight through as the LPUNKNOWN& of
// PX_IUnknown, which is only correct because m_pPict is at +0x0.
struct PX_CPictureHolder {
    IPicture* m_pPict;           // 0x00
};

// CDataPathProperty as retail lays it out.  CAsyncMonikerFile::CAsyncMonikerFile
// (RVA 0x243110, mfc140u) initialises +0x40, +0x50 and +0x58, and the
// CDataPathProperty CRuntimeClass descriptor in mfc140u .rdata records
// m_nObjectSize == 0x70; the PX_DataPath helper at RVA 0x1f11c8 reads
// m_pControl at +0x60 and assigns m_strPath at +0x68.  Only those two members
// are viewed; the base is opaque.
struct PX_CDataPathProperty {
    void**        vptr;          // 0x00
    unsigned char _base[0x58];   // 0x08..0x5f  CFile/COleStreamFile/CMonikerFile/CAsyncMonikerFile
    COleControl*  m_pControl;    // 0x60
    CString       m_strPath;     // 0x68
};
static_assert(offsetof(PX_CDataPathProperty, m_pControl) == 0x60, "CDataPathProperty::m_pControl");
static_assert(offsetof(PX_CDataPathProperty, m_strPath) == 0x68, "CDataPathProperty::m_strPath");
static_assert(sizeof(PX_CDataPathProperty) == 0x70, "CDataPathProperty is 0x70 bytes (CRuntimeClass m_nObjectSize)");
static_assert(sizeof(CString) == 8, "CString must be a single m_pszData pointer");

// COleControl members the PX_DataPath helper touches, at the offsets retail
// uses (0x1d8 / 0x240 / 0x244); these are the offsets include/openmfc/afxole.h
// already records for them.
static_assert(offsetof(COleControl, m_pClientSite) == 0x1d8, "COleControl::m_pClientSite");
static_assert(offsetof(COleControl, m_bDataPathPropertiesLoaded) == 0x240, "COleControl::m_bDataPathPropertiesLoaded");
static_assert(offsetof(COleControl, m_dwDataPathVersionToReport) == 0x244, "COleControl::m_dwDataPathVersionToReport");

// The client-compiled CDataPathProperty vtable slot PX_DataPath dispatches
// through: retail calls `mov 0x160(%rax),%rax` (slot 44) with %rdx == 0.  The
// slot number was checked two ways: (1) counting the virtuals of CObject ->
// CFile -> COleStreamFile -> CMonikerFile -> CAsyncMonikerFile -> CDataPathProperty
// in the shipping headers on this host with one shared destructor slot and
// MSVC's reversed order for adjacently-declared overloads, which puts
// CDataPathProperty's three Open overloads at 44..46 with the last-declared
// `Open(CFileException* = NULL)` at 44; (2) dumping the retail CAsyncMonikerFile
// vtable (mfc140u .rdata 0x32b520, reached from its ctor): its 44 slots end at
// OnStopBinding (slot 43) and its own six Open overloads sit at 30..35 in
// reverse declaration order, so the CDataPathProperty additions start at 44;
// (3) review re-check with the shipping compiler: cl.exe 14.51 /MD /D_AFXDLL
// /d1reportSingleClassLayout on afxctl.h reports CDataPathProperty size 112,
// m_pControl@96, m_strPath@104, slots 44..46 = CDataPathProperty::Open x3,
// 47 = ResetData, and a derived class overriding only Open(CFileException*)
// replaces slot 44 (Open(LPCTSTR, COleControl*, CFileException*) is 45,
// Open(COleControl*, CFileException*) is 46).
// A single-argument call with %rdx == 0 is `Open((CFileException*)NULL)`.
enum { kSlotDataPathOpen_pError = 44 };
using DataPathOpenFn = int (MS_ABI*)(void* pThis, void* pError);

// IID_IPicture, read out of mfc140u .rdata at RVA 0x2d9d98 (the GUID PX_Picture
// loads into %r9): {7BF80980-BF32-101A-8BBB-00AA00300CAB}.
const GUID kIID_IPicture =
    { 0x7BF80980, 0xBF32, 0x101A, { 0x8B, 0xBB, 0x00, 0xAA, 0x00, 0x30, 0x0C, 0xAB } };

}  // namespace

// Sibling exports in this file that other bodies here call directly.
extern "C" int MS_ABI impl__PX_ULong__YAHPEAVCPropExchange__PEB_WAEAK_Z(void* pPX, const wchar_t* pszPropName, unsigned long* pulValue);
extern "C" int MS_ABI impl__PX_ULong__YAHPEAVCPropExchange__PEB_WAEAKK_Z(void* pPX, const wchar_t* pszPropName, unsigned long* pulValue, unsigned long ulDefault);
extern "C" int MS_ABI impl__PX_Bool__YAHPEAVCPropExchange__PEB_WAEAH_Z(void* pPX, const wchar_t* pszPropName, int* pbValue);
extern "C" int MS_ABI impl__PX_Currency__YAHPEAVCPropExchange__PEB_WAEATtagCY___Z(void* pPX, const wchar_t* pszPropName, CY* pcyValue);
extern "C" int MS_ABI impl__PX_String__YAHPEAVCPropExchange__PEB_WAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(void* pPX, const wchar_t* pszPropName, CString* pstrValue);
extern "C" int MS_ABI impl__PX_IUnknown__YAHPEAVCPropExchange__PEB_WAEAPEAUIUnknown__AEBU_GUID__PEAU2__Z(void* pPX, const wchar_t* pszPropName, IUnknown** ppUnk, const GUID* piid, IUnknown* pUnkDefault);

// Symbol: ?ExchangeVersion@CPropExchange@@UEAAHAEAKKH@Z
// Retail RVA 0x1efb30 (mfc140u):
//   if (m_bLoading) {                                   ; cmpl $0,0x8(%rcx)
//       bResult = PX_ULong(this, L"_Version", m_dwVersion, dwVersionDefault);
//                                                       ; call 0x1f0c10 (the folded
//                                                       ; PX_ULong/PX_Long/PX_Color default
//                                                       ; overload), r8 = &this->+0x10
//       dwVersionLoaded = m_dwVersion;                  ; mov (%rbx),%ecx ; mov %ecx,(%rdi)
//   } else {
//       m_dwVersion = bConvert ? dwVersionDefault : dwVersionLoaded;
//                                                       ; test %r9d ; jne / mov (%rdx),%eax
//       bResult = PX_ULong(this, L"_Version", m_dwVersion);   ; call 0x1f0bd0
//   }
//   return bResult;
// The wide string at 0x18034bc30 (mfc140u) is L"_Version".  (The previous body in this
// file never read or wrote m_dwVersion and never reached the exchanger; it is
// replaced by this transcription.)
extern "C" int MS_ABI impl__ExchangeVersion_CPropExchange__UEAAHAEAKKH_Z(
    void* pThis, unsigned long* pdwVersionLoaded, unsigned long dwVersionDefault, int bConvert) {
    PX_CPropExchange* self = px(pThis);
    int bResult;
    if (self->m_bLoading != 0) {
        bResult = impl__PX_ULong__YAHPEAVCPropExchange__PEB_WAEAKK_Z(
            pThis, L"_Version", &self->m_dwVersion, dwVersionDefault);
        *pdwVersionLoaded = self->m_dwVersion;
    } else {
        self->m_dwVersion = (bConvert != 0) ? dwVersionDefault : *pdwVersionLoaded;
        bResult = impl__PX_ULong__YAHPEAVCPropExchange__PEB_WAEAK_Z(
            pThis, L"_Version", &self->m_dwVersion);
    }
    return bResult;
}

// Symbol: ?PX_Blob@@YAHPEAVCPropExchange@@PEB_WAEAPEAXPEAX@Z
// Retail RVA 0x1f0f10 (mfc140u):
//   if (pPX->m_bAsync) return TRUE;                     ; cmpl $0,0xc(%rcx)
//   return pPX->ExchangeBlobProp(pszPropName, &hBlob, hBlobDefault);
//                                                       ; slot 2, rdx/r8/r9 pass through
extern "C" int MS_ABI impl__PX_Blob__YAHPEAVCPropExchange__PEB_WAEAPEAXPEAX_Z(
    void* pPX, const wchar_t* pszPropName, void** phBlob, void* hBlobDefault) {
    if (px(pPX)->m_bAsync != 0) {
        return TRUE;
    }
    return reinterpret_cast<ExchangeBlobPropFn>(px(pPX)->vptr[kSlotExchangeBlobProp])(
        pPX, pszPropName, reinterpret_cast<HGLOBAL*>(phBlob), static_cast<HGLOBAL>(hBlobDefault));
}

// Symbol: ?PX_Bool@@YAHPEAVCPropExchange@@PEB_WAEAH@Z
// Retail RVA 0x1f0c50 (mfc140u):
//   if (pPX->m_bAsync) return TRUE;
//   return pPX->ExchangeProp(pszPropName, VT_BOOL /*0xb*/, &bValue, NULL);
// The wrapper passes the BOOL* itself; any BOOL <-> VARIANT_BOOL conversion is
// the exchanger's business, not this function's.
extern "C" int MS_ABI impl__PX_Bool__YAHPEAVCPropExchange__PEB_WAEAH_Z(
    void* pPX, const wchar_t* pszPropName, int* pbValue) {
    if (px(pPX)->m_bAsync != 0) {
        return TRUE;
    }
    return px_ExchangeProp(pPX, pszPropName, VT_BOOL, pbValue, nullptr);
}

// Symbol: ?PX_Bool@@YAHPEAVCPropExchange@@PEB_WAEAHH@Z
// Retail RVA 0x1f0c90 (mfc140u): as above with pvDefault = &bDefault (the
// register argument is spilled to its home slot and its address passed).
extern "C" int MS_ABI impl__PX_Bool__YAHPEAVCPropExchange__PEB_WAEAHH_Z(
    void* pPX, const wchar_t* pszPropName, int* pbValue, int bDefault) {
    if (px(pPX)->m_bAsync != 0) {
        return TRUE;
    }
    return px_ExchangeProp(pPX, pszPropName, VT_BOOL, pbValue, &bDefault);
}

// Symbol: ?PX_Color@@YAHPEAVCPropExchange@@PEB_WAEAK@Z
// Retail RVA 0x1f0bd0 (mfc140u), one body shared with PX_Long / PX_ULong:
//   if (pPX->m_bAsync) return TRUE;
//   return pPX->ExchangeProp(pszPropName, VT_I4 /*3*/, &clrValue, NULL);
extern "C" int MS_ABI impl__PX_Color__YAHPEAVCPropExchange__PEB_WAEAK_Z(
    void* pPX, const wchar_t* pszPropName, unsigned long* pclrValue) {
    if (px(pPX)->m_bAsync != 0) {
        return TRUE;
    }
    return px_ExchangeProp(pPX, pszPropName, VT_I4, pclrValue, nullptr);
}

// Symbol: ?PX_Color@@YAHPEAVCPropExchange@@PEB_WAEAKK@Z
// Retail RVA 0x1f0c10 (mfc140u), one body shared with PX_Long / PX_ULong:
//   if (pPX->m_bAsync) return TRUE;
//   return pPX->ExchangeProp(pszPropName, VT_I4, &clrValue, &clrDefault);
extern "C" int MS_ABI impl__PX_Color__YAHPEAVCPropExchange__PEB_WAEAKK_Z(
    void* pPX, const wchar_t* pszPropName, unsigned long* pclrValue, unsigned long clrDefault) {
    if (px(pPX)->m_bAsync != 0) {
        return TRUE;
    }
    return px_ExchangeProp(pPX, pszPropName, VT_I4, pclrValue, &clrDefault);
}

// Symbol: ?PX_Currency@@YAHPEAVCPropExchange@@PEB_WAEATtagCY@@@Z
// Retail RVA 0x1f0d90 (mfc140u):
//   if (pPX->m_bAsync) return TRUE;
//   return pPX->ExchangeProp(pszPropName, VT_CY /*6*/, &cyValue, NULL);
extern "C" int MS_ABI impl__PX_Currency__YAHPEAVCPropExchange__PEB_WAEATtagCY___Z(
    void* pPX, const wchar_t* pszPropName, CY* pcyValue) {
    if (px(pPX)->m_bAsync != 0) {
        return TRUE;
    }
    return px_ExchangeProp(pPX, pszPropName, VT_CY, pcyValue, nullptr);
}

// Symbol: ?PX_Currency@@YAHPEAVCPropExchange@@PEB_WAEATtagCY@@T2@@Z
// Retail RVA 0x1f0dd0 (mfc140u): the 8-byte CY default arrives by value in %r9
// (`mov %r9,0x20(%rsp)` spills it) and its home-slot address is pvDefault.
// The by-value CY is modelled as `long long` (an 8-byte aggregate travels in
// the register exactly like an integer under the MS x64 convention).
extern "C" int MS_ABI impl__PX_Currency__YAHPEAVCPropExchange__PEB_WAEATtagCY__T2__Z(
    void* pPX, const wchar_t* pszPropName, CY* pcyValue, long long cyDefault) {
    if (px(pPX)->m_bAsync != 0) {
        return TRUE;
    }
    return px_ExchangeProp(pPX, pszPropName, VT_CY, pcyValue, &cyDefault);
}

// Shared body of both PX_DataPath overloads, transcribed from the retail
// internal helper at RVA 0x1f11c8 (mfc140u), which both exports tail into
// with (pPX, pszPropName, path, vtDefault, pvDefault):
//   BOOL bLoading = pPX->m_bLoading;                                  ; +0x8 -> r12d
//   COleControl* pControl = path.m_pControl;                          ; +0x60 -> rdi
//   BOOL bHasClientSite = pControl != NULL && pControl->m_pClientSite != NULL;   ; +0x1d8 -> esi
//   if (!pPX->m_bAsync) {                                             ; cmpl $0,0xc(%rcx) ; jne
//       CString strPath;                                              ; string-manager nil
//       if (!bLoading) strPath = path.m_strPath;                      ; CloneData + operator=
//       BOOL bResult = pvDefault != NULL
//           ? pPX->ExchangeProp(pszPropName, vtDefault, &strPath, pvDefault)
//           : pPX->ExchangeProp(pszPropName, VT_BSTR, &strPath, NULL); ; slot 1
//       if (!bResult) return FALSE;                                   ; (after strPath dtor)
//       if (bLoading) path.m_strPath = (LPCTSTR)strPath;              ; wcslen + SetString
//       if (pControl != NULL && !bHasClientSite) {
//           pControl->m_bDataPathPropertiesLoaded = FALSE;            ; +0x240 <- esi (0)
//           pControl->m_dwDataPathVersionToReport = pPX->m_dwVersion; ; +0x244 <- +0x10
//       }
//   }                                                                 ; strPath dtor
//   if ((pPX->m_bAsync || bHasClientSite) && bLoading)
//       return path.Open((CFileException*)NULL);                      ; vtable slot 44, rdx = 0
//   return TRUE;
// Note the m_bDataPathPropertiesLoaded / m_dwDataPathVersionToReport store is
// NOT gated on bLoading in retail (the `test %r12d ; je` before it only skips
// the SetString), and it is skipped entirely on the m_bAsync path.
namespace {
int px_DataPath(void* pPX, const wchar_t* pszPropName, void* pPath,
                unsigned short vtDefault, const void* pvDefault) {
    PX_CPropExchange* self = px(pPX);
    PX_CDataPathProperty* path = static_cast<PX_CDataPathProperty*>(pPath);
    const int bLoading = self->m_bLoading;
    COleControl* pControl = path->m_pControl;
    const int bHasClientSite = (pControl != nullptr && pControl->m_pClientSite != nullptr) ? 1 : 0;

    if (self->m_bAsync == 0) {
        CString strPath;
        if (bLoading == 0) {
            strPath = path->m_strPath;
        }
        int bResult;
        if (pvDefault != nullptr) {
            bResult = px_ExchangeProp(pPX, pszPropName, vtDefault, &strPath, pvDefault);
        } else {
            bResult = px_ExchangeProp(pPX, pszPropName, VT_BSTR, &strPath, nullptr);
        }
        if (bResult == 0) {
            return FALSE;
        }
        if (bLoading != 0) {
            path->m_strPath = static_cast<const wchar_t*>(strPath);
        }
        if (pControl != nullptr && bHasClientSite == 0) {
            pControl->m_bDataPathPropertiesLoaded = FALSE;
            pControl->m_dwDataPathVersionToReport = self->m_dwVersion;
        }
    }

    if ((self->m_bAsync != 0 || bHasClientSite != 0) && bLoading != 0) {
        return reinterpret_cast<DataPathOpenFn>(path->vptr[kSlotDataPathOpen_pError])(pPath, nullptr);
    }
    return TRUE;
}
}  // namespace

// Symbol: ?PX_DataPath@@YAHPEAVCPropExchange@@PEB_WAEAVCDataPathProperty@@1@Z
// Retail RVA 0x1f13a0 (mfc140u): helper(pPX, pszPropName, path, VT_LPWSTR /*0x1e*/, pszDefault).
extern "C" int MS_ABI impl__PX_DataPath__YAHPEAVCPropExchange__PEB_WAEAVCDataPathProperty__1_Z(
    void* pPX, const wchar_t* pszPropName, void* pDataPathProp, const wchar_t* pszDefault) {
    return px_DataPath(pPX, pszPropName, pDataPathProp, VT_LPWSTR, pszDefault);
}

// Symbol: ?PX_DataPath@@YAHPEAVCPropExchange@@PEB_WAEAVCDataPathProperty@@AEBV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@@Z
// Retail RVA 0x1f13c0 (mfc140u): helper(pPX, pszPropName, path, VT_BSTR /*8*/, &strDefault).
extern "C" int MS_ABI impl__PX_DataPath__YAHPEAVCPropExchange__PEB_WAEAVCDataPathProperty__AEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
    void* pPX, const wchar_t* pszPropName, void* pDataPathProp, const CString* pstrDefault) {
    return px_DataPath(pPX, pszPropName, pDataPathProp, VT_BSTR, pstrDefault);
}

// Symbol: ?PX_Double@@YAHPEAVCPropExchange@@PEB_WAEAN@Z
// Retail RVA 0x1f0e90 (mfc140u):
//   if (pPX->m_bAsync) return TRUE;
//   return pPX->ExchangeProp(pszPropName, VT_R8 /*5*/, &doubleValue, NULL);
extern "C" int MS_ABI impl__PX_Double__YAHPEAVCPropExchange__PEB_WAEAN_Z(
    void* pPX, const wchar_t* pszPropName, double* pdoubleValue) {
    if (px(pPX)->m_bAsync != 0) {
        return TRUE;
    }
    return px_ExchangeProp(pPX, pszPropName, VT_R8, pdoubleValue, nullptr);
}

// Symbol: ?PX_Double@@YAHPEAVCPropExchange@@PEB_WAEANN@Z
// Retail RVA 0x1f0ed0 (mfc140u): the default arrives in %xmm3 (`movsd %xmm3,0x20(%rsp)`)
// and its home-slot address is pvDefault.
extern "C" int MS_ABI impl__PX_Double__YAHPEAVCPropExchange__PEB_WAEANN_Z(
    void* pPX, const wchar_t* pszPropName, double* pdoubleValue, double doubleDefault) {
    if (px(pPX)->m_bAsync != 0) {
        return TRUE;
    }
    return px_ExchangeProp(pPX, pszPropName, VT_R8, pdoubleValue, &doubleDefault);
}

// Symbol: ?PX_Float@@YAHPEAVCPropExchange@@PEB_WAEAM@Z
// Retail RVA 0x1f0e10 (mfc140u):
//   if (pPX->m_bAsync) return TRUE;
//   return pPX->ExchangeProp(pszPropName, VT_R4 /*4*/, &floatValue, NULL);
extern "C" int MS_ABI impl__PX_Float__YAHPEAVCPropExchange__PEB_WAEAM_Z(
    void* pPX, const wchar_t* pszPropName, float* pfloatValue) {
    if (px(pPX)->m_bAsync != 0) {
        return TRUE;
    }
    return px_ExchangeProp(pPX, pszPropName, VT_R4, pfloatValue, nullptr);
}

// Symbol: ?PX_Float@@YAHPEAVCPropExchange@@PEB_WAEAMM@Z
// Retail RVA 0x1f0e50 (mfc140u): the default arrives in %xmm3 (`movss %xmm3,0x20(%rsp)`)
// and its home-slot address is pvDefault.
extern "C" int MS_ABI impl__PX_Float__YAHPEAVCPropExchange__PEB_WAEAMM_Z(
    void* pPX, const wchar_t* pszPropName, float* pfloatValue, float floatDefault) {
    if (px(pPX)->m_bAsync != 0) {
        return TRUE;
    }
    return px_ExchangeProp(pPX, pszPropName, VT_R4, pfloatValue, &floatDefault);
}

// Symbol: ?PX_Font@@YAHPEAVCPropExchange@@PEB_WAEAVCFontHolder@@PEBUtagFONTDESC@@PEAUIFontDisp@@@Z
// Retail RVA 0x1f0f40 (mfc140u):
//   if (pPX->m_bAsync) return TRUE;
//   return pPX->ExchangeFontProp(pszPropName, font, pFontDesc, pFontDispAmbient);
//                                                       ; slot 3; the 5th argument is
//                                                       ; re-read from 0x60(%rsp) and
//                                                       ; forwarded in 0x20(%rsp)
extern "C" int MS_ABI impl__PX_Font__YAHPEAVCPropExchange__PEB_WAEAVCFontHolder__PEBUtagFONTDESC__PEAUIFontDisp___Z(
    void* pPX, const wchar_t* pszPropName, void* pFontHolder, const FONTDESC* pFontDesc,
    IFontDisp* pFontDispAmbient) {
    if (px(pPX)->m_bAsync != 0) {
        return TRUE;
    }
    return reinterpret_cast<ExchangeFontPropFn>(px(pPX)->vptr[kSlotExchangeFontProp])(
        pPX, pszPropName, pFontHolder, pFontDesc, pFontDispAmbient);
}

// Symbol: ?PX_IUnknown@@YAHPEAVCPropExchange@@PEB_WAEAPEAUIUnknown@@AEBU_GUID@@PEAU2@@Z
// Retail RVA 0x1f0f80 (mfc140u):
//   if (pPX->m_bAsync) return TRUE;
//   return pPX->ExchangePersistentProp(pszPropName, &pUnk, iid, pUnkDefault);   ; slot 4
extern "C" int MS_ABI impl__PX_IUnknown__YAHPEAVCPropExchange__PEB_WAEAPEAUIUnknown__AEBU_GUID__PEAU2__Z(
    void* pPX, const wchar_t* pszPropName, IUnknown** ppUnk, const GUID* piid, IUnknown* pUnkDefault) {
    if (px(pPX)->m_bAsync != 0) {
        return TRUE;
    }
    return reinterpret_cast<ExchangePersistentPropFn>(px(pPX)->vptr[kSlotExchangePersistentProp])(
        pPX, pszPropName, ppUnk, piid, pUnkDefault);
}

// Symbol: ?PX_Long@@YAHPEAVCPropExchange@@PEB_WAEAJ@Z
// Retail RVA 0x1f0bd0 (mfc140u), the body shared with PX_Color / PX_ULong:
//   if (pPX->m_bAsync) return TRUE;
//   return pPX->ExchangeProp(pszPropName, VT_I4 /*3*/, &lValue, NULL);
extern "C" int MS_ABI impl__PX_Long__YAHPEAVCPropExchange__PEB_WAEAJ_Z(
    void* pPX, const wchar_t* pszPropName, long* plValue) {
    if (px(pPX)->m_bAsync != 0) {
        return TRUE;
    }
    return px_ExchangeProp(pPX, pszPropName, VT_I4, plValue, nullptr);
}

// Symbol: ?PX_Long@@YAHPEAVCPropExchange@@PEB_WAEAJJ@Z
// Retail RVA 0x1f0c10 (mfc140u), the body shared with PX_Color / PX_ULong:
//   if (pPX->m_bAsync) return TRUE;
//   return pPX->ExchangeProp(pszPropName, VT_I4, &lValue, &lDefault);
extern "C" int MS_ABI impl__PX_Long__YAHPEAVCPropExchange__PEB_WAEAJJ_Z(
    void* pPX, const wchar_t* pszPropName, long* plValue, long lDefault) {
    if (px(pPX)->m_bAsync != 0) {
        return TRUE;
    }
    return px_ExchangeProp(pPX, pszPropName, VT_I4, plValue, &lDefault);
}

// Symbol: ?PX_Picture@@YAHPEAVCPropExchange@@PEB_WAEAVCPictureHolder@@2@Z
// Retail RVA 0x1f0ff0 (mfc140u):
//   if (pPX->m_bAsync) return TRUE;
//   return PX_IUnknown(pPX, pszPropName, (LPUNKNOWN&)pict.m_pPict, IID_IPicture,
//                      pictDefault.m_pPict);            ; `mov (%r9),%rax` reads +0x0
//                                                       ; of pictDefault; %r8 (pict) is
//                                                       ; forwarded unchanged as the
//                                                       ; LPUNKNOWN&, i.e. &pict.m_pPict
extern "C" int MS_ABI impl__PX_Picture__YAHPEAVCPropExchange__PEB_WAEAVCPictureHolder__2_Z(
    void* pPX, const wchar_t* pszPropName, void* pPict, void* pPictDefault) {
    if (px(pPX)->m_bAsync != 0) {
        return TRUE;
    }
    PX_CPictureHolder* pict = static_cast<PX_CPictureHolder*>(pPict);
    PX_CPictureHolder* pictDefault = static_cast<PX_CPictureHolder*>(pPictDefault);
    return impl__PX_IUnknown__YAHPEAVCPropExchange__PEB_WAEAPEAUIUnknown__AEBU_GUID__PEAU2__Z(
        pPX, pszPropName, reinterpret_cast<IUnknown**>(&pict->m_pPict), &kIID_IPicture,
        static_cast<IUnknown*>(pictDefault->m_pPict));
}

// Symbol: ?PX_Picture@@YAHPEAVCPropExchange@@PEB_WAEAVCPictureHolder@@@Z
// Retail RVA 0x1f0fc0 (mfc140u):
//   if (pPX->m_bAsync) return TRUE;
//   return PX_IUnknown(pPX, pszPropName, (LPUNKNOWN&)pict.m_pPict, IID_IPicture, NULL);
extern "C" int MS_ABI impl__PX_Picture__YAHPEAVCPropExchange__PEB_WAEAVCPictureHolder___Z(
    void* pPX, const wchar_t* pszPropName, void* pPict) {
    if (px(pPX)->m_bAsync != 0) {
        return TRUE;
    }
    PX_CPictureHolder* pict = static_cast<PX_CPictureHolder*>(pPict);
    return impl__PX_IUnknown__YAHPEAVCPropExchange__PEB_WAEAPEAUIUnknown__AEBU_GUID__PEAU2__Z(
        pPX, pszPropName, reinterpret_cast<IUnknown**>(&pict->m_pPict), &kIID_IPicture, nullptr);
}

// Symbol: ?PX_Short@@YAHPEAVCPropExchange@@PEB_WAEAF@Z
// Retail RVA 0x1f0b50 (mfc140u), one body shared with PX_UShort:
//   if (pPX->m_bAsync) return TRUE;
//   return pPX->ExchangeProp(pszPropName, VT_I2 /*2*/, &sValue, NULL);
extern "C" int MS_ABI impl__PX_Short__YAHPEAVCPropExchange__PEB_WAEAF_Z(
    void* pPX, const wchar_t* pszPropName, short* psValue) {
    if (px(pPX)->m_bAsync != 0) {
        return TRUE;
    }
    return px_ExchangeProp(pPX, pszPropName, VT_I2, psValue, nullptr);
}

// Symbol: ?PX_Short@@YAHPEAVCPropExchange@@PEB_WAEAFF@Z
// Retail RVA 0x1f0b90 (mfc140u), one body shared with PX_UShort
// (`mov %r9w,0x20(%rsp)` spills the 16-bit default; its address is pvDefault):
//   if (pPX->m_bAsync) return TRUE;
//   return pPX->ExchangeProp(pszPropName, VT_I2, &sValue, &sDefault);
extern "C" int MS_ABI impl__PX_Short__YAHPEAVCPropExchange__PEB_WAEAFF_Z(
    void* pPX, const wchar_t* pszPropName, short* psValue, short sDefault) {
    if (px(pPX)->m_bAsync != 0) {
        return TRUE;
    }
    return px_ExchangeProp(pPX, pszPropName, VT_I2, psValue, &sDefault);
}

// Symbol: ?PX_String@@YAHPEAVCPropExchange@@PEB_WAEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@1@Z
// Retail RVA 0x1f0d50 (mfc140u) -- the LPCTSTR-default overload:
//   if (pPX->m_bAsync) return TRUE;
//   return pPX->ExchangeProp(pszPropName, VT_LPWSTR /*0x1e*/, &strValue, lpszDefault);
//                                                       ; r9 (lpszDefault) -> 0x20(%rsp),
//                                                       ; r8 (&strValue) -> r9
extern "C" int MS_ABI impl__PX_String__YAHPEAVCPropExchange__PEB_WAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__1_Z(
    void* pPX, const wchar_t* pszPropName, CString* pstrValue, const wchar_t* lpszDefault) {
    if (px(pPX)->m_bAsync != 0) {
        return TRUE;
    }
    return px_ExchangeProp(pPX, pszPropName, VT_LPWSTR, pstrValue, lpszDefault);
}

// Symbol: ?PX_String@@YAHPEAVCPropExchange@@PEB_WAEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@@Z
// Retail RVA 0x1f0cd0 (mfc140u):
//   if (pPX->m_bAsync) return TRUE;
//   return pPX->ExchangeProp(pszPropName, VT_BSTR /*8*/, &strValue, NULL);
extern "C" int MS_ABI impl__PX_String__YAHPEAVCPropExchange__PEB_WAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
    void* pPX, const wchar_t* pszPropName, CString* pstrValue) {
    if (px(pPX)->m_bAsync != 0) {
        return TRUE;
    }
    return px_ExchangeProp(pPX, pszPropName, VT_BSTR, pstrValue, nullptr);
}

// Symbol: ?PX_String@@YAHPEAVCPropExchange@@PEB_WAEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@AEBV23@@Z
// Retail RVA 0x1f0d10 (mfc140u) -- the CString-default overload:
//   if (pPX->m_bAsync) return TRUE;
//   return pPX->ExchangeProp(pszPropName, VT_BSTR /*8*/, &strValue, &strDefault);
extern "C" int MS_ABI impl__PX_String__YAHPEAVCPropExchange__PEB_WAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__AEBV23__Z(
    void* pPX, const wchar_t* pszPropName, CString* pstrValue, const CString* pstrDefault) {
    if (px(pPX)->m_bAsync != 0) {
        return TRUE;
    }
    return px_ExchangeProp(pPX, pszPropName, VT_BSTR, pstrValue, pstrDefault);
}

// Symbol: ?PX_ULong@@YAHPEAVCPropExchange@@PEB_WAEAK@Z
// Retail RVA 0x1f0bd0 (mfc140u), the body shared with PX_Color / PX_Long:
//   if (pPX->m_bAsync) return TRUE;
//   return pPX->ExchangeProp(pszPropName, VT_I4 /*3*/, &ulValue, NULL);
extern "C" int MS_ABI impl__PX_ULong__YAHPEAVCPropExchange__PEB_WAEAK_Z(
    void* pPX, const wchar_t* pszPropName, unsigned long* pulValue) {
    if (px(pPX)->m_bAsync != 0) {
        return TRUE;
    }
    return px_ExchangeProp(pPX, pszPropName, VT_I4, pulValue, nullptr);
}

// Symbol: ?PX_ULong@@YAHPEAVCPropExchange@@PEB_WAEAKK@Z
// Retail RVA 0x1f0c10 (mfc140u), the body shared with PX_Color / PX_Long:
//   if (pPX->m_bAsync) return TRUE;
//   return pPX->ExchangeProp(pszPropName, VT_I4, &ulValue, &ulDefault);
extern "C" int MS_ABI impl__PX_ULong__YAHPEAVCPropExchange__PEB_WAEAKK_Z(
    void* pPX, const wchar_t* pszPropName, unsigned long* pulValue, unsigned long ulDefault) {
    if (px(pPX)->m_bAsync != 0) {
        return TRUE;
    }
    return px_ExchangeProp(pPX, pszPropName, VT_I4, pulValue, &ulDefault);
}

// Symbol: ?PX_UShort@@YAHPEAVCPropExchange@@PEB_WAEAG@Z
// Retail RVA 0x1f0b50 (mfc140u), one body shared with PX_Short:
//   if (pPX->m_bAsync) return TRUE;
//   return pPX->ExchangeProp(pszPropName, VT_I2 /*2*/, &usValue, NULL);
extern "C" int MS_ABI impl__PX_UShort__YAHPEAVCPropExchange__PEB_WAEAG_Z(
    void* pPX, const wchar_t* pszPropName, unsigned short* pusValue) {
    if (px(pPX)->m_bAsync != 0) {
        return TRUE;
    }
    return px_ExchangeProp(pPX, pszPropName, VT_I2, pusValue, nullptr);
}

// Symbol: ?PX_UShort@@YAHPEAVCPropExchange@@PEB_WAEAGG@Z
// Retail RVA 0x1f0b90 (mfc140u), one body shared with PX_Short:
//   if (pPX->m_bAsync) return TRUE;
//   return pPX->ExchangeProp(pszPropName, VT_I2, &usValue, &usDefault);
extern "C" int MS_ABI impl__PX_UShort__YAHPEAVCPropExchange__PEB_WAEAGG_Z(
    void* pPX, const wchar_t* pszPropName, unsigned short* pusValue, unsigned short usDefault) {
    if (px(pPX)->m_bAsync != 0) {
        return TRUE;
    }
    return px_ExchangeProp(pPX, pszPropName, VT_I2, pusValue, &usDefault);
}

// Symbol: ?PX_VBXFontConvert@@YAHPEAVCPropExchange@@AEAVCFontHolder@@@Z
// Retail RVA 0x1f1020 (mfc140u):
//   if (pPX->m_bAsync) return TRUE;                     ; cmpl $0,0xc(%rcx) ; jne -> eax=1
//   if (font.m_pFont == NULL) return TRUE;              ; cmpq $0,(%rdx) ; je -> eax=1
//   CString strFontName;                                ; string manager GetNilString()+0x18
//   if (PX_String(pPX, L"FontName", strFontName)) {     ; call 0x1f0cd0, string at 0x34bc48
//       BSTR bstr = ::SysAllocStringLen(strFontName, strFontName.GetLength());
//                                                       ; OLEAUT32 #4; length read from the
//                                                       ; CStringData header at data-0x10
//       if (bstr == NULL) AfxThrowMemoryException();    ; 0x180003160 -> AtlThrow(E_OUTOFMEMORY)
//       font.m_pFont->put_Name(bstr);                   ; IFont slot 4 (+0x20)
//       ::SysFreeString(bstr);                          ; OLEAUT32 #6
//   }
//   CY cy;
//   if (PX_Currency(pPX, L"FontSize", cy))              ; call 0x1f0d90, string at 0x34bc60
//       font.m_pFont->put_Size(cy);                     ; IFont slot 6 (+0x30), CY by value in rdx
//   BOOL b;                                             ; one uninitialised local reused
//   if (PX_Bool(pPX, L"FontBold", b))       font.m_pFont->put_Bold(b);           ; 0x34bc78, slot 8  (+0x40)
//   if (PX_Bool(pPX, L"FontItalic", b))     font.m_pFont->put_Italic(b);         ; 0x34bc90, slot 10 (+0x50)
//   if (PX_Bool(pPX, L"FontUnderline", b))  font.m_pFont->put_Underline(b);      ; 0x34bca8, slot 12 (+0x60)
//   if (PX_Bool(pPX, L"FontStrikethru", b)) font.m_pFont->put_Strikethrough(b);  ; 0x34bcc8, slot 14 (+0x70)
//   return TRUE;                                        ; (after strFontName dtor)
// IFont slot numbers are ocidl.h's: QueryInterface 0, AddRef 1, Release 2,
// get_Name 3, put_Name 4, get_Size 5, put_Size 6, get_Bold 7, put_Bold 8,
// get_Italic 9, put_Italic 10, get_Underline 11, put_Underline 12,
// get_Strikethrough 13, put_Strikethrough 14.  Retail re-reads font.m_pFont
// before each put_ call; every property name is exactly as retail spells it
// (the VBX names, "FontStrikethru" without the trailing "ough").
extern "C" int MS_ABI impl__PX_VBXFontConvert__YAHPEAVCPropExchange__AEAVCFontHolder___Z(
    void* pPX, void* pFontHolder) {
    PX_CFontHolder* font = static_cast<PX_CFontHolder*>(pFontHolder);
    if (px(pPX)->m_bAsync != 0) {
        return TRUE;
    }
    if (font->m_pFont == nullptr) {
        return TRUE;
    }

    CString strFontName;
    if (impl__PX_String__YAHPEAVCPropExchange__PEB_WAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
            pPX, L"FontName", &strFontName) != 0) {
        BSTR bstrName = ::SysAllocStringLen(strFontName.GetString(), static_cast<UINT>(strFontName.GetLength()));
        if (bstrName == nullptr) {
            impl__AfxThrowMemoryException__YAXXZ();
        }
        font->m_pFont->put_Name(bstrName);
        ::SysFreeString(bstrName);
    }

    CY cySize;
    if (impl__PX_Currency__YAHPEAVCPropExchange__PEB_WAEATtagCY___Z(pPX, L"FontSize", &cySize) != 0) {
        font->m_pFont->put_Size(cySize);
    }

    int bFlag = 0;   // retail leaves this uninitialised; zeroed here so a
                     // successful exchange that writes nothing stays deterministic
    if (impl__PX_Bool__YAHPEAVCPropExchange__PEB_WAEAH_Z(pPX, L"FontBold", &bFlag) != 0) {
        font->m_pFont->put_Bold(bFlag);
    }
    if (impl__PX_Bool__YAHPEAVCPropExchange__PEB_WAEAH_Z(pPX, L"FontItalic", &bFlag) != 0) {
        font->m_pFont->put_Italic(bFlag);
    }
    if (impl__PX_Bool__YAHPEAVCPropExchange__PEB_WAEAH_Z(pPX, L"FontUnderline", &bFlag) != 0) {
        font->m_pFont->put_Underline(bFlag);
    }
    if (impl__PX_Bool__YAHPEAVCPropExchange__PEB_WAEAH_Z(pPX, L"FontStrikethru", &bFlag) != 0) {
        font->m_pFont->put_Strikethrough(bFlag);
    }
    return TRUE;
}
