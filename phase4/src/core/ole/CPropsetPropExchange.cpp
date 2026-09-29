// CPropsetPropExchange — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp
//
// MFC's private property-set-backed CPropExchange (COleControl's OLE property-set
// persistence: each PX_* call becomes one property of a CPropertySection).  No
// header in include/openmfc declares the class (retail keeps it in the private
// ctlimpl.h), so the layout is pinned in-file below.  Every body in this file was
// transcribed from the retail export, disassembled from mfc140u.dll itself
// (disas.py --u); all RVAs below are mfc140u RVAs, and every `call *0x...(%rip)`
// import slot named here was resolved with iatu.py against mfc140u.dll.
//
// ---------------------------------------------------------------------------
// Retail object layout, pinned by the retail ctor
// ??0CPropsetPropExchange@@QEAA@AEAVCPropertySection@@PEAUIStorage@@H@Z
// (RVA 0x1f1a70, mfc140u), which is the whole body, in retail order:
//   lea 0x180324b40,%rax
//   movq $0,0xc(%rcx)                               -> CPropExchange::m_bAsync (+0xc) and m_dwVersion (+0x10)
//   mov %rax,(%rcx)                                 -> vptr
//   mov %rdx,0x18(%rcx)          (CPropertySection&) -> m_psec (a reference, stored as a pointer)
//   mov %r8,0x20(%rcx)           (IStorage*)         -> m_lpStorage (no AddRef)
//   movl $0xff,0x28(%rcx)                           -> m_dwPropID = 255
//   mov %r9d,0x8(%rcx)           (BOOL)              -> m_bLoading
// and by the scalar deleting destructor (vtable slot 5, RVA 0x1f1aa0, unexported),
// which frees with `mov $0x30,%edx` -> sizeof == 0x30.  The base part
// (+0x0..+0x17) is the CPropExchange layout core/ole/CPropExchange.cpp pins.
// None of the four Exchange* bodies reads m_lpStorage.
//
// Retail vftable (0x180324b40 in mfc140u), read slot by slot with vtdump_u.py:
//   0 +0x00  ExchangeVersion         0x1efb30  (CPropExchange's, not overridden)
//   1 +0x08  ExchangeProp            0x1f1cb0
//   2 +0x10  ExchangeBlobProp        0x1f1f40
//   3 +0x18  ExchangeFontProp        0x1f2c60
//   4 +0x20  ExchangePersistentProp  0x1f2840
//   5 +0x28  scalar deleting dtor    0x1f1aa0  (unexported)
// The ctor below installs kPropsetPropExchangeVtbl, an in-file table of the same
// six entries in that order (the order the PX_* wrappers in core/ole/
// CPropExchange.cpp dispatch through).
//
// Every virtual call retail makes here goes through __guard_dispatch_icall_fptr
// (`call *0x1802c7b30(%rip)` in mfc140u -- the CFG dispatch pointer, not an
// import); the real target is the slot loaded into %rax just before.
//
// CPropertySection / CProperty access: retail inlines CPropertySection::
// GetProperty (a walk of the node chain of the section's CPtrList m_PropList --
// the list object is at section+0x18, its m_pNodeHead at section+0x20, which is
// the qword retail loads -- comparing ((CProperty*)node->data)->m_dwPropID)
// wherever it looks a property up.  That is
// exactly the body core/ole/CPropertySection.cpp transcribes for the exported
// ?GetProperty@CPropertySection@@QEAAPEAVCProperty@@K@Z (RVA 0x2627c0), so the
// thunk is called instead of repeating the walk.  The direct calls retail makes
// are ?GetID@CPropertySection@@... (0x262bb0), ?SetName@CPropertySection@@...
// (0x262c30), ?Set@CPropertySection@@QEAAHKPEAXK@Z (0x2625f0) and
// ?Get@CProperty@@QEAAPEAXPEAK@Z (0x261c40); each maps to the thunk with the same
// mangled name in core/ole/CPropertySection.cpp / CProperty.cpp.
// ---------------------------------------------------------------------------

#include "detail/ManualSmallStubImplementationsSupport.h"

#include <ocidl.h>
#include <olectl.h>
#include <oleauto.h>
#include <cerrno>
#include <cstring>
#include <cwchar>
#include <wchar.h>

// Thunks defined elsewhere in the tree (each definition checked by grep).
extern "C" int MS_ABI impl__ExchangeVersion_CPropExchange__UEAAHAEAKKH_Z(          // core/ole/CPropExchange.cpp
    void* pThis, unsigned long* pdwVersionLoaded, unsigned long dwVersionDefault, int bConvert);
extern "C" int MS_ABI impl__GetID_CPropertySection__QEAAHPEB_WPEAK_Z(               // core/ole/CPropertySection.cpp
    void* pThis, const wchar_t* pszName, unsigned long* pdwPropID);
extern "C" void* MS_ABI impl__GetProperty_CPropertySection__QEAAPEAVCProperty__K_Z(  // core/ole/CPropertySection.cpp
    void* pThis, unsigned long dwPropID);
extern "C" int MS_ABI impl__SetName_CPropertySection__QEAAHKPEB_W_Z(                // core/ole/CPropertySection.cpp
    void* pThis, unsigned long dwPropID, const wchar_t* pszName);
extern "C" int MS_ABI impl__Set_CPropertySection__QEAAHKPEAXK_Z(                    // core/ole/CPropertySection.cpp
    void* pThis, unsigned long dwPropID, void* pValue, unsigned long dwType);
extern "C" void* MS_ABI impl__Get_CProperty__QEAAPEAXPEAK_Z(void* pThis, unsigned long* pcb);  // core/ole/CProperty.cpp
// CStringW::operator=(const char*) -- retail ExchangeProp calls this export
// (??4?$CStringT@_W...@@QEAAAEAV01@PEBD@Z, RVA 0x1cd480) directly.
extern "C" void* MS_ABI impl___4__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAAAEAV01_PEBD_Z(  // core/collections/Thunks.cpp
    void* pThis, const char* psz);
extern "C" void MS_ABI impl__SetFont_CFontHolder__QEAAXPEAUIFont___Z(void* self, void* font);  // core/ole/CFontHolder.cpp
extern "C" void MS_ABI impl__InitializeFont_CFontHolder__QEAAXPEBUtagFONTDESC__PEAUIDispatch___Z(  // core/ole/CFontHolder.cpp
    void* self, const void* pFontDesc, void* pFontDispAmbient);
extern "C" void MS_ABI impl___3_YAXPEAX_Z(void* ptr);                           // detail/MemcoreSupport.cpp
extern "C" void MS_ABI impl__AfxThrowInvalidArgException__YAXXZ();              // detail/MfcExceptionsSupport.cpp
extern "C" void MS_ABI impl__AfxThrowMemoryException__YAXXZ();                  // detail/MfcExceptionsSupport.cpp
extern "C" void MS_ABI impl__AfxThrowOleException__YAXJ_Z(LONG sc);             // detail/MfcExceptionsSupport.cpp

// This file's own exports, forward-declared for the in-file vtable.
extern "C" int MS_ABI impl__ExchangeProp_CPropsetPropExchange__UEAAHPEB_WGPEAXPEBX_Z(
    void* pThis, const wchar_t* pszPropName, unsigned short vtProp, void* pvProp, const void* pvDefault);
extern "C" int MS_ABI impl__ExchangeBlobProp_CPropsetPropExchange__UEAAHPEB_WPEAPEAXPEAX_Z(
    void* pThis, const wchar_t* pszPropName, HGLOBAL* phBlob, HGLOBAL hBlobDefault);
extern "C" int MS_ABI impl__ExchangeFontProp_CPropsetPropExchange__UEAAHPEB_WAEAVCFontHolder__PEBUtagFONTDESC__PEAUIFontDisp___Z(
    void* pThis, const wchar_t* pszPropName, void* pFontHolder, const FONTDESC* pFontDesc, IFontDisp* pFontDispAmbient);
extern "C" int MS_ABI impl__ExchangePersistentProp_CPropsetPropExchange__UEAAHPEB_WPEAPEAUIUnknown__AEBU_GUID__PEAU2__Z(
    void* pThis, const wchar_t* pszPropName, IUnknown** ppUnk, const GUID* piid, IUnknown* pUnkDefault);

namespace {

// Retail layout (see the header comment for the evidence).
struct PS_CPropsetPropExchange {
    void* const*  vptr;                 // 0x00
    int           m_bLoading;           // 0x08  (CPropExchange)
    int           m_bAsync;             // 0x0c  (CPropExchange)
    unsigned long m_dwVersion;          // 0x10  (CPropExchange)
    void*         m_psec;               // 0x18  CPropertySection&
    IStorage*     m_lpStorage;          // 0x20
    unsigned long m_dwPropID;           // 0x28
};
static_assert(offsetof(PS_CPropsetPropExchange, m_bLoading) == 0x08, "CPropExchange::m_bLoading");
static_assert(offsetof(PS_CPropsetPropExchange, m_bAsync) == 0x0c, "CPropExchange::m_bAsync");
static_assert(offsetof(PS_CPropsetPropExchange, m_dwVersion) == 0x10, "CPropExchange::m_dwVersion");
static_assert(offsetof(PS_CPropsetPropExchange, m_psec) == 0x18, "CPropsetPropExchange::m_psec");
static_assert(offsetof(PS_CPropsetPropExchange, m_lpStorage) == 0x20, "CPropsetPropExchange::m_lpStorage");
static_assert(offsetof(PS_CPropsetPropExchange, m_dwPropID) == 0x28, "CPropsetPropExchange::m_dwPropID");
static_assert(sizeof(PS_CPropsetPropExchange) == 0x30, "CPropsetPropExchange is 0x30 bytes (sized delete in the deleting dtor)");

inline PS_CPropsetPropExchange* ps(void* pThis) { return static_cast<PS_CPropsetPropExchange*>(pThis); }

// Retail CProperty (the same view core/ole/CPropertySection.cpp pins): ExchangeProp
// reads the type as `movzwl 0x4(%rdi)` and the stream helper as `mov 0x4(%rdi),%eax`.
struct PS_Property {
    DWORD m_dwPropID;                   // 0x00
    DWORD m_dwType;                     // 0x04
    void* m_pValue;                     // 0x08
};
static_assert(offsetof(PS_Property, m_dwType) == 0x04, "CProperty::m_dwType");
static_assert(sizeof(PS_Property) == 0x10, "CProperty is 0x10 bytes");

// CFontHolder as retail lays it out: afxctl.h's `LPFONT m_pFont` at +0x0.  Retail
// ExchangeFontProp reads +0x0 directly (`cmpq $0x0,(%r8)`, `mov (%rsi),%rcx`).
// include/openmfc/afxole.h types the member IFontDisp*, so this view is used instead
// (same approach as core/ole/CPropbagPropExchange.cpp).
struct PS_CFontHolder {
    IFont* m_pFont;                     // 0x00
};
static_assert(offsetof(CFontHolder, m_pFont) == 0x00, "CFontHolder::m_pFont");

// Property type tags used by the stream-backed properties (retail compares the
// CProperty type against these immediates in the helper at 0x1f2580 and the
// Exchange* bodies, and passes them to the blob writer at 0x1f208c).
constexpr DWORD kVtBlob = VT_BLOB;          // 0x41: raw object stream (IPersistStream / picture)
constexpr DWORD kVtPropsetBlob = 0x4b;      // 0x4b: IDataObject "property set" stream
static_assert(VT_BLOB == 0x41, "VT_BLOB");

// GUIDs, each the 16 bytes at the named mfc140u .rdata address.
// IID_IFont (0x1802d9d88) {BEF6E002-A874-101A-8BBA-00AA00300CAB}
const GUID kIID_IFont =
    { 0xBEF6E002, 0xA874, 0x101A, { 0x8B, 0xBA, 0x00, 0xAA, 0x00, 0x30, 0x0C, 0xAB } };
// IID_IDataObject (0x1802d9ab8) {0000010E-0000-0000-C000-000000000046}
const GUID kIID_IDataObject =
    { 0x0000010E, 0x0000, 0x0000, { 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 } };
// IID_IPersistStream (0x1802d9b18) {00000109-0000-0000-C000-000000000046}
const GUID kIID_IPersistStream =
    { 0x00000109, 0x0000, 0x0000, { 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 } };
// CLSID_StdFont (0x1802d9928) {0BE35203-8F91-11CE-9DE3-00AA004BB851}
const GUID kCLSID_StdFont =
    { 0x0BE35203, 0x8F91, 0x11CE, { 0x9D, 0xE3, 0x00, 0xAA, 0x00, 0x4B, 0xB8, 0x51 } };
// CLSID_StdPicture (0x1802d9938) {0BE35204-8F91-11CE-9DE3-00AA004BB851}
const GUID kCLSID_StdPicture =
    { 0x0BE35204, 0x8F91, 0x11CE, { 0x9D, 0xE3, 0x00, 0xAA, 0x00, 0x4B, 0xB8, 0x51 } };
// Second font class retail accepts alongside CLSID_StdFont in both font tests in
// this file (0x1f21a8, 0x1f2300) (0x18034bd48) {FB8F0823-0164-101B-84ED-08002B2EC713}
const GUID kCLSID_AltFont =
    { 0xFB8F0823, 0x0164, 0x101B, { 0x84, 0xED, 0x08, 0x00, 0x2B, 0x2E, 0xC7, 0x13 } };
// Second picture class retail accepts alongside CLSID_StdPicture in the picture
// tests in this file (0x1f2300, ExchangePersistentProp) (0x18034bd38) {FB8F0824-0164-101B-84ED-08002B2EC713}
const GUID kCLSID_AltPicture =
    { 0xFB8F0824, 0x0164, 0x101B, { 0x84, 0xED, 0x08, 0x00, 0x2B, 0x2E, 0xC7, 0x13 } };
// The CLSID whose string form is registered as the property-set clipboard format
// (0x1802d9908) {FB8F0821-0164-101B-84ED-08002B2EC713}
const GUID kCLSID_PropsetFormat =
    { 0xFB8F0821, 0x0164, 0x101B, { 0x84, 0xED, 0x08, 0x00, 0x2B, 0x2E, 0xC7, 0x13 } };

inline bool SameGuid(const GUID& a, const GUID& b) { return std::memcmp(&a, &b, sizeof(GUID)) == 0; }

// The FONTDESC the font loaders hand to OleCreateFontIndirect: the 0x28 bytes at
// 0x18034bd10 (mfc140u .rdata) read as cbSizeofstruct 0x28, lpstrName ->
// 0x18034b8d8 = L"Helv", cySize 0x1d4c0 (= 12.0 in CY units), sWeight 0x190
// (FW_NORMAL), sCharset 1 (DEFAULT_CHARSET), fItalic/fUnderline/fStrikethrough 0.
wchar_t kHelvFontName[] = L"Helv";
const FONTDESC kFontDescHelv = {
    sizeof(FONTDESC), kHelvFontName, { { 120000, 0 } }, FW_NORMAL, DEFAULT_CHARSET, FALSE, FALSE, FALSE
};

// The default FONTDESC _AfxIsSameFont substitutes for a NULL pFontDesc: the 0x28
// bytes at 0x18034b878 (mfc140u .rdata): cbSizeofstruct 0x28, lpstrName ->
// 0x18034b858 = L"MS Shell Dlg", cySize 0x1d4c0, sWeight 0x190, sCharset 1, flags 0.
wchar_t kShellDlgFontName[] = L"MS Shell Dlg";
const FONTDESC kFontDescDefault = {
    sizeof(FONTDESC), kShellDlgFontName, { { 120000, 0 } }, FW_NORMAL, DEFAULT_CHARSET, FALSE, FALSE, FALSE
};
static_assert(sizeof(FONTDESC) == 0x28, "FONTDESC is 0x28 bytes (cbSizeofstruct in retail)");

// The 8-byte .bss value at 0x1803c3fa8 (mfc140u) that every Seek in these helpers
// loads as its offset.  A scan of mfc140u .text for rip-relative references to it
// finds only `mov 0x...(%rip),%rdx` loads (seven, all in this code) and no store,
// so it is always zero.
const LARGE_INTEGER kLargeZero = {};

// _AfxRelease, RVA 0x26ccc4 (mfc140u, unexported):
//   if (*pp != NULL) { (*pp)->Release(); *pp = NULL; }     ; vtable +0x10
void AfxReleaseUnk(IUnknown** pp) {
    if (*pp != nullptr) {
        (*pp)->Release();
        *pp = nullptr;
    }
}

// --- scalar helpers (shared with CPropbagPropExchange in retail) ---------------

// RVA 0x1f1ac4 (mfc140u, unexported): the byte size of a PX_* scalar type:
//   VT_I2 2, VT_I4 4, VT_R4 4, VT_R8 8, VT_CY 8, VT_BSTR 8, VT_BOOL 2, else 0.
std::size_t AfxGetSizeOfVarType(unsigned short vt) {
    switch (vt) {
    case VT_I2:   return 2;
    case VT_I4:
    case VT_R4:   return 4;
    case VT_R8:
    case VT_CY:
    case VT_BSTR: return 8;
    case VT_BOOL: return 2;
    default:      return 0;
    }
}

// RVA 0x1ef7d8 (mfc140u, unexported):
//   if (pvSrc != NULL) switch (vt) {
//     VT_I2: 16-bit copy;  VT_I4/VT_R4/VT_BOOL: 32-bit;  VT_R8/VT_CY: 64-bit;
//     VT_UI1: byte;
//     VT_BSTR:  *(CString*)pvDest = *(const CString*)pvSrc;   ; call 0xde30
//     VT_LPSTR: *(CString*)pvDest = (LPCWSTR)pvSrc;           ; wcslen + SetString (0x2e30)
//     default:  return FALSE;
//   }
//   return pvSrc != NULL;
int AfxCopyPropValue(unsigned short vt, void* pvDest, const void* pvSrc) {
    if (pvSrc != nullptr) {
        switch (vt) {
        case VT_I2:
            *static_cast<unsigned short*>(pvDest) = *static_cast<const unsigned short*>(pvSrc);
            break;
        case VT_I4:
        case VT_R4:
        case VT_BOOL:
            *static_cast<unsigned int*>(pvDest) = *static_cast<const unsigned int*>(pvSrc);
            break;
        case VT_R8:
        case VT_CY:
            *static_cast<unsigned long long*>(pvDest) = *static_cast<const unsigned long long*>(pvSrc);
            break;
        case VT_UI1:
            *static_cast<unsigned char*>(pvDest) = *static_cast<const unsigned char*>(pvSrc);
            break;
        case VT_BSTR:
            *static_cast<CString*>(pvDest) = *static_cast<const CString*>(pvSrc);
            break;
        case VT_LPSTR:
            *static_cast<CString*>(pvDest) = static_cast<const wchar_t*>(pvSrc);
            break;
        default:
            return FALSE;
        }
    }
    return pvSrc != nullptr ? TRUE : FALSE;
}

// RVA 0x1f1c0c (mfc140u, unexported):
//   if (pv1 == pv2) return TRUE;
//   if (pv1 == NULL || pv2 == NULL) return FALSE;
//   switch (vt) {
//     VT_I2/VT_I4/VT_R4/VT_R8/VT_CY/VT_BOOL:
//         return memcmp(pv1, pv2, size-of(vt)) == 0;
//     VT_BSTR:  psz2 = ((CString*)pv2)->m_pszData;
//               if (psz2 == NULL) AtlThrow(E_FAIL);          ; 0x333c -> AfxThrowOleException
//               return wcscmp(((CString*)pv1)->m_pszData, psz2) == 0;
//     VT_LPSTR: return wcscmp(((CString*)pv1)->m_pszData, (LPCWSTR)pv2) == 0;
//     default:  return FALSE;                                 ; VT_UI1 included
//   }
int AfxIsSamePropValue(unsigned short vt, const void* pv1, const void* pv2) {
    if (pv1 == pv2) {
        return TRUE;
    }
    if (pv1 == nullptr || pv2 == nullptr) {
        return FALSE;
    }
    switch (vt) {
    case VT_I2:
    case VT_I4:
    case VT_R4:
    case VT_R8:
    case VT_CY:
    case VT_BOOL:
        return std::memcmp(pv1, pv2, AfxGetSizeOfVarType(vt)) == 0 ? TRUE : FALSE;
    case VT_BSTR: {
        const wchar_t* psz2 = static_cast<const CString*>(pv2)->GetString();
        if (psz2 == nullptr) {
            impl__AfxThrowOleException__YAXJ_Z(E_FAIL);
        }
        return wcscmp(static_cast<const CString*>(pv1)->GetString(), psz2) == 0 ? TRUE : FALSE;
    }
    case VT_LPSTR:
        return wcscmp(static_cast<const CString*>(pv1)->GetString(),
                      static_cast<const wchar_t*>(pv2)) == 0 ? TRUE : FALSE;
    default:
        return FALSE;
    }
}

// RVA 0x1f1afc (mfc140u, unexported) -- convert one scalar to another:
//   cbSrc = size-of(vtSrc);  if (cbSrc == 0) return FALSE;
//   if (vtSrc == vtDst) { memcpy_s(pvDst, cbSrc, pvSrc, cbSrc); return TRUE; }
//   cbDst = size-of(vtDst);  if (cbDst == 0) return FALSE;
//   VARIANT var; V_VT(&var) = vtSrc;                       ; the other header words are not set
//   memcpy_s(&var + 8, 0x18, pvSrc, cbSrc);
//   if (FAILED(VariantChangeType(&var, &var, 0, vtDst))) return FALSE;   ; OLEAUT32 #12
//   memcpy_s(pvDst, cbDst, &var + 8, cbDst);
//   return TRUE;
// Each memcpy_s is inlined as retail's usual pattern: NULL dest -> errno = EINVAL;
// NULL src -> memset(dest, 0, size), errno = EINVAL; count > size -> memset,
// errno = ERANGE; then _invalid_parameter_noinfo() and AfxThrowInvalidArgException().
// It is written here as memcpy_s (which does the zeroing, errno and
// invalid-parameter report itself) followed by the throw on a non-zero return.
// The in-VARIANT copy is given the 0x10 bytes the VARIANT really has past its
// header instead of retail's 0x18; cbSrc is at most 8, so no call can tell the
// difference.
int AfxCoerceNumber(void* pvDst, unsigned short vtDst, const void* pvSrc, unsigned short vtSrc) {
    const std::size_t cbSrc = AfxGetSizeOfVarType(vtSrc);
    if (cbSrc == 0) {
        return FALSE;
    }
    if (vtSrc == vtDst) {
        if (memcpy_s(pvDst, cbSrc, pvSrc, cbSrc) != 0) {
            impl__AfxThrowInvalidArgException__YAXXZ();
        }
        return TRUE;
    }
    const std::size_t cbDst = AfxGetSizeOfVarType(vtDst);
    if (cbDst == 0) {
        return FALSE;
    }
    VARIANT var;
    V_VT(&var) = vtSrc;
    void* pData = reinterpret_cast<BYTE*>(&var) + 8;
    if (memcpy_s(pData, sizeof(VARIANT) - 8, pvSrc, cbSrc) != 0) {
        impl__AfxThrowInvalidArgException__YAXXZ();
    }
    if (FAILED(::VariantChangeType(&var, &var, 0, vtDst))) {
        return FALSE;
    }
    if (memcpy_s(pvDst, cbDst, pData, cbDst) != 0) {
        impl__AfxThrowInvalidArgException__YAXXZ();
    }
    return TRUE;
}

// --- blob helpers ------------------------------------------------------------

// RVA 0x1ef6e8 (mfc140u, unexported):
//   bResult = FALSE;
//   if (*(DWORD*)pvSrc > 0) {
//       cb = *(DWORD*)pvSrc + 4;                               ; 64-bit add
//       *phDst = GlobalAlloc(GMEM_MOVEABLE, cb);
//       if (*phDst != NULL) {
//           p = GlobalLock(*phDst);
//           if (p != NULL) { memcpy_s(p, GlobalSize(*phDst), pvSrc, cb); bResult = TRUE; }
//           GlobalUnlock(*phDst);                              ; also when the lock failed
//       }
//   }
//   return bResult;
// memcpy_s failure is inlined as memset/errno = ERANGE/_invalid_parameter_noinfo/
// AfxThrowInvalidArgException; written here as memcpy_s plus the throw.
int AfxInitBlob(HGLOBAL* phDst, const void* pvSrc) {
    int bResult = FALSE;
    const DWORD cbData = *static_cast<const DWORD*>(pvSrc);
    if (cbData > 0) {
        const SIZE_T cb = static_cast<SIZE_T>(cbData) + 4;
        *phDst = ::GlobalAlloc(GMEM_MOVEABLE, cb);
        if (*phDst != nullptr) {
            void* p = ::GlobalLock(*phDst);
            if (p != nullptr) {
                if (memcpy_s(p, ::GlobalSize(*phDst), pvSrc, cb) != 0) {
                    impl__AfxThrowInvalidArgException__YAXXZ();
                }
                bResult = TRUE;
            }
            ::GlobalUnlock(*phDst);
        }
    }
    return bResult;
}

// RVA 0x1ef788 (mfc140u, unexported):
//   bResult = FALSE;
//   if ((p = GlobalLock(hSrc)) != NULL) { bResult = AfxInitBlob(phDst, p); GlobalUnlock(hSrc); }
//   return bResult;
int AfxCopyBlob(HGLOBAL* phDst, HGLOBAL hSrc) {
    int bResult = FALSE;
    const void* p = ::GlobalLock(hSrc);
    if (p != nullptr) {
        bResult = AfxInitBlob(phDst, p);
        ::GlobalUnlock(hSrc);
    }
    return bResult;
}

// --- stream-backed property helpers -------------------------------------------

// RVA 0x1f13dc (mfc140u, unexported) -- an empty growable memory stream:
//   pStream = NULL;
//   hMem = GlobalAlloc(GMEM_MOVEABLE|GMEM_SHARE /*0x2002*/, 0);  if (!hMem) return NULL;
//   if (FAILED(CreateStreamOnHGlobal(hMem, TRUE, &pStream))) { GlobalFree(hMem); return NULL; }
//   return pStream;
IStream* PsCreateMemoryStream() {
    IStream* pStream = nullptr;
    HGLOBAL hMem = ::GlobalAlloc(GMEM_MOVEABLE | GMEM_SHARE, 0);
    if (hMem == nullptr) {
        return nullptr;
    }
    if (FAILED(::CreateStreamOnHGlobal(hMem, TRUE, &pStream))) {
        ::GlobalFree(hMem);
        return nullptr;
    }
    return pStream;
}

// RVA 0x1e2bdc (mfc140u, unexported) -- register a CLSID's string form as a
// clipboard format:
//   WCHAR sz[40];
//   swprintf(sz, 0x28, L"{%08lX-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X}", ...);
//                                                           ; fmt at 0x18034b7a0, via 0x1d04a0
//   return RegisterClipboardFormatW(sz);
// Retail keeps the result in a 16-bit static (0x1803c4144) and calls this only
// while that static is 0; PsGetPropsetFormat reproduces the cache.
UINT PsRegisterClsidFormat(const GUID& clsid) {
    wchar_t sz[0x28];
    swprintf(sz, 0x28, L"{%08lX-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X}",
             static_cast<unsigned long>(clsid.Data1), static_cast<unsigned>(clsid.Data2),
             static_cast<unsigned>(clsid.Data3),
             static_cast<unsigned>(clsid.Data4[0]), static_cast<unsigned>(clsid.Data4[1]),
             static_cast<unsigned>(clsid.Data4[2]), static_cast<unsigned>(clsid.Data4[3]),
             static_cast<unsigned>(clsid.Data4[4]), static_cast<unsigned>(clsid.Data4[5]),
             static_cast<unsigned>(clsid.Data4[6]), static_cast<unsigned>(clsid.Data4[7]));
    return ::RegisterClipboardFormatW(sz);
}

// The inlined `movzwl 0x1803c4144 ; test ; jne ; ... call 0x1e2bdc ; mov %ax,0x1803c4144`
// sequence (mfc140u) that every IDataObject user below starts with.
CLIPFORMAT g_cfPropset = 0;
CLIPFORMAT PsGetPropsetFormat() {
    if (g_cfPropset == 0) {
        g_cfPropset = static_cast<CLIPFORMAT>(PsRegisterClsidFormat(kCLSID_PropsetFormat));
    }
    return g_cfPropset;
}

// RVA 0x1f2580 (mfc140u, unexported) -- open a stream-typed property as a stream
// positioned at the start of its data:
//   *pdwType = 0;
//   if (!psec.GetID(pszPropName, &dwPropID)) return NULL;           ; call 0x262bb0
//   pProp = psec.GetProperty(dwPropID) (inlined walk); if (!pProp) return NULL;
//   *pdwType = pProp->m_dwType;
//   if (*pdwType != 0x41 && *pdwType != 0x4b) return NULL;
//   pStream = <memory stream, 0x1f13dc>;  if (!pStream) return NULL;
//   pvData = pProp->Get(&cb);                                       ; call 0x261c40
//   if (pvData != NULL &&
//       SUCCEEDED(pStream->Write((BYTE*)pvData + 4, cb, NULL)) &&   ; vtable +0x20
//       SUCCEEDED(pStream->Seek(li, STREAM_SEEK_CUR, NULL)))        ; vtable +0x28
//       return pStream;
//   pStream->Release(); return NULL;
// where li.LowPart = (DWORD)-cb and li.HighPart = -1 (`neg %ecx` into the low dword,
// `movl $0xffffffff` into the high one) -- i.e. -cb for any cb > 0; transcribed
// literally, so cb == 0 seeks by 0xffffffff00000000 exactly as retail does.
IStream* PsGetStreamFromPropSet(void* psec, const wchar_t* pszPropName, DWORD* pdwType) {
    *pdwType = 0;
    unsigned long dwPropID = 0;
    if (!impl__GetID_CPropertySection__QEAAHPEB_WPEAK_Z(psec, pszPropName, &dwPropID)) {
        return nullptr;
    }
    PS_Property* pProp = static_cast<PS_Property*>(
        impl__GetProperty_CPropertySection__QEAAPEAVCProperty__K_Z(psec, dwPropID));
    if (pProp == nullptr) {
        return nullptr;
    }
    *pdwType = pProp->m_dwType;
    if (*pdwType != kVtBlob && *pdwType != kVtPropsetBlob) {
        return nullptr;
    }
    IStream* pStream = PsCreateMemoryStream();
    if (pStream == nullptr) {
        return nullptr;
    }
    unsigned long cb = 0;
    BYTE* pvData = static_cast<BYTE*>(impl__Get_CProperty__QEAAPEAXPEAK_Z(pProp, &cb));
    if (pvData != nullptr && SUCCEEDED(pStream->Write(pvData + 4, cb, nullptr))) {
        LARGE_INTEGER li;
        li.LowPart = static_cast<DWORD>(0u - cb);
        li.HighPart = -1;
        if (SUCCEEDED(pStream->Seek(li, STREAM_SEEK_CUR, nullptr))) {
            return pStream;
        }
    }
    pStream->Release();
    return nullptr;
}

// RVA 0x1f208c (mfc140u, unexported) -- store the rest of a stream as a counted
// blob property:
//   bResult = FALSE;
//   if (FAILED(pStream->Seek(zero, STREAM_SEEK_CUR, &liCur))) return FALSE;   ; vtable +0x28
//   if (FAILED(pStream->Seek(zero, STREAM_SEEK_END, &liEnd))) return FALSE;
//   if (FAILED(pStream->Seek(liCur, STREAM_SEEK_SET, NULL)))  return FALSE;
//   cb = liEnd.LowPart - liCur.LowPart;                                        ; 32-bit
//   hGlobal = GlobalAlloc(GMEM_MOVEABLE|GMEM_SHARE /*0x2002*/, cb + 4);  if (!hGlobal) return FALSE;
//   if ((p = GlobalLock(hGlobal)) != NULL) {
//       *(DWORD*)p = cb;
//       if (SUCCEEDED(pStream->Read(p + 4, cb, NULL)))                         ; vtable +0x18
//           bResult = psec.Set(dwPropID, p, dwType);                           ; call 0x2625f0
//       GlobalUnlock(hGlobal);
//   }
//   GlobalFree(hGlobal);
//   return bResult;
int PsSaveStreamDataAsBlobProp(IStream* pStream, void* psec, DWORD dwPropID, DWORD dwType) {
    int bResult = FALSE;
    ULARGE_INTEGER liCur = {};
    ULARGE_INTEGER liEnd = {};
    if (FAILED(pStream->Seek(kLargeZero, STREAM_SEEK_CUR, &liCur))) {
        return FALSE;
    }
    if (FAILED(pStream->Seek(kLargeZero, STREAM_SEEK_END, &liEnd))) {
        return FALSE;
    }
    LARGE_INTEGER liBack;
    liBack.QuadPart = static_cast<LONGLONG>(liCur.QuadPart);
    if (FAILED(pStream->Seek(liBack, STREAM_SEEK_SET, nullptr))) {
        return FALSE;
    }
    const DWORD cb = liEnd.LowPart - liCur.LowPart;
    HGLOBAL hGlobal = ::GlobalAlloc(GMEM_MOVEABLE | GMEM_SHARE, static_cast<SIZE_T>(cb + 4));
    if (hGlobal == nullptr) {
        return FALSE;
    }
    BYTE* p = static_cast<BYTE*>(::GlobalLock(hGlobal));
    if (p != nullptr) {
        *reinterpret_cast<DWORD*>(p) = cb;
        if (SUCCEEDED(pStream->Read(p + 4, cb, nullptr))) {
            bResult = impl__Set_CPropertySection__QEAAHKPEAXK_Z(psec, dwPropID, p, dwType);
        }
        ::GlobalUnlock(hGlobal);
    }
    ::GlobalFree(hGlobal);
    return bResult;
}

// The FORMATETC every IDataObject call below builds:
//   { cfFormat = <property-set format>, ptd = NULL, dwAspect = DVASPECT_CONTENT (1),
//     lindex = -1, tymed = TYMED_ISTREAM (4) }
FORMATETC PsPropsetFormatEtc() {
    FORMATETC fe;
    fe.cfFormat = PsGetPropsetFormat();
    fe.ptd = nullptr;
    fe.dwAspect = DVASPECT_CONTENT;
    fe.lindex = -1;
    fe.tymed = TYMED_ISTREAM;
    return fe;
}

// RVA 0x1f2678 (mfc140u, unexported) -- persist an object into a property:
//   if (pUnk == NULL) return FALSE;
//   bResult = FALSE;
//   if (SUCCEEDED(pUnk->QueryInterface(IID_IDataObject, &pDataObj))) {
//       STGMEDIUM stm; stm.tymed = 0; stm.pUnkForRelease = NULL;
//       if (SUCCEEDED(pDataObj->GetData(&fe, &stm))) {                    ; vtable +0x18
//           if (stm.tymed == TYMED_ISTREAM &&
//               SUCCEEDED(stm.pstm->Seek(zero, STREAM_SEEK_SET, NULL)))
//               bResult = <0x1f208c>(stm.pstm, psec, dwPropID, 0x4b);
//           ReleaseStgMedium(&stm);
//       }
//       pDataObj->Release();
//   }
//   if (!bResult) {
//       if (FAILED(pUnk->QueryInterface(IID_IPersistStream, &pPersStm))) return FALSE;
//       if ((pStream = <memory stream>) != NULL) {
//           if (SUCCEEDED(OleSaveToStream(pPersStm, pStream)) &&
//               SUCCEEDED(pStream->Seek(zero, STREAM_SEEK_SET, NULL)))
//               bResult = <0x1f208c>(pStream, psec, dwPropID, VT_BLOB);
//           pStream->Release();
//       }
//       pPersStm->Release();
//   }
//   return bResult;
int PsSaveObjectInPropset(IUnknown* pUnk, void* psec, DWORD dwPropID) {
    if (pUnk == nullptr) {
        return FALSE;
    }
    int bResult = FALSE;
    IDataObject* pDataObj = nullptr;
    if (SUCCEEDED(pUnk->QueryInterface(kIID_IDataObject, reinterpret_cast<void**>(&pDataObj)))) {
        FORMATETC fe = PsPropsetFormatEtc();
        STGMEDIUM stm;
        stm.tymed = TYMED_NULL;
        stm.pUnkForRelease = nullptr;
        if (SUCCEEDED(pDataObj->GetData(&fe, &stm))) {
            if (stm.tymed == TYMED_ISTREAM &&
                SUCCEEDED(stm.pstm->Seek(kLargeZero, STREAM_SEEK_SET, nullptr))) {
                bResult = PsSaveStreamDataAsBlobProp(stm.pstm, psec, dwPropID, kVtPropsetBlob);
            }
            ::ReleaseStgMedium(&stm);
        }
        pDataObj->Release();
    }
    if (bResult) {
        return bResult;
    }
    IPersistStream* pPersStm = nullptr;
    if (FAILED(pUnk->QueryInterface(kIID_IPersistStream, reinterpret_cast<void**>(&pPersStm)))) {
        return FALSE;
    }
    IStream* pStream = PsCreateMemoryStream();
    if (pStream != nullptr) {
        if (SUCCEEDED(::OleSaveToStream(pPersStm, pStream)) &&
            SUCCEEDED(pStream->Seek(kLargeZero, STREAM_SEEK_SET, nullptr))) {
            bResult = PsSaveStreamDataAsBlobProp(pStream, psec, dwPropID, kVtBlob);
        }
        pStream->Release();
    }
    pPersStm->Release();
    return bResult;
}

// RVA 0x1f2300 (mfc140u, unexported) -- create an object from a 0x4b
// (IDataObject property-set) stream:
//   pUnk = NULL;
//   if (FAILED(pStream->Seek(zero, STREAM_SEEK_CUR, &liPos))) return NULL;
//   bOK = SUCCEEDED(pStream->Seek(8, STREAM_SEEK_CUR, NULL)) &&
//         SUCCEEDED(pStream->Read(&clsid, 16, NULL));                   ; vtable +0x18
//   pStream->Seek(liPos, STREAM_SEEK_SET, NULL);                         ; result ignored
//   if (!bOK) return NULL;
//   if (clsid is CLSID_StdFont or {FB8F0823-...})
//       hr = OleCreateFontIndirect(&<Helv FONTDESC>, iid, &pUnk);         ; OLEAUT32 #420
//   else if (clsid is CLSID_StdPicture or {FB8F0824-...})
//       hr = OleCreatePictureIndirect(NULL, iid, FALSE, &pUnk);           ; OLEAUT32 #419
//   else {
//       hr = CoCreateInstance(clsid, NULL, CLSCTX_INPROC_SERVER, iid, &pUnk);
//       if (FAILED(hr)) return NULL;                                      ; pUnk not touched
//   }
//   if (FAILED(hr)) pUnk = NULL;                                          ; (the two Ole* paths)
//   if (pUnk == NULL) return NULL;
//   if (SUCCEEDED(pUnk->QueryInterface(IID_IDataObject, &pDataObj))) {
//       STGMEDIUM stm = { TYMED_ISTREAM, pStream, NULL };
//       hr = pDataObj->SetData(&fe, &stm, FALSE);                         ; vtable +0x38
//       pDataObj->Release();
//       if (SUCCEEDED(hr)) return pUnk;
//   }
//   pUnk->Release(); return NULL;
IUnknown* PsCreateObjectFromStreamedPropset(IStream* pStream, const GUID& iid) {
    IUnknown* pUnk = nullptr;
    ULARGE_INTEGER liPos = {};
    if (FAILED(pStream->Seek(kLargeZero, STREAM_SEEK_CUR, &liPos))) {
        return nullptr;
    }
    CLSID clsid;
    int bOK = FALSE;
    LARGE_INTEGER liSkip;
    liSkip.QuadPart = 8;
    if (SUCCEEDED(pStream->Seek(liSkip, STREAM_SEEK_CUR, nullptr))) {
        bOK = SUCCEEDED(pStream->Read(&clsid, sizeof(CLSID), nullptr)) ? TRUE : FALSE;
    }
    LARGE_INTEGER liBack;
    liBack.QuadPart = static_cast<LONGLONG>(liPos.QuadPart);
    pStream->Seek(liBack, STREAM_SEEK_SET, nullptr);
    if (!bOK) {
        return nullptr;
    }

    HRESULT hr;
    if (SameGuid(clsid, kCLSID_StdFont) || SameGuid(clsid, kCLSID_AltFont)) {
        hr = ::OleCreateFontIndirect(const_cast<FONTDESC*>(&kFontDescHelv), iid, reinterpret_cast<void**>(&pUnk));
    } else if (SameGuid(clsid, kCLSID_StdPicture) || SameGuid(clsid, kCLSID_AltPicture)) {
        hr = ::OleCreatePictureIndirect(nullptr, iid, FALSE, reinterpret_cast<void**>(&pUnk));
    } else {
        hr = ::CoCreateInstance(clsid, nullptr, CLSCTX_INPROC_SERVER, iid, reinterpret_cast<void**>(&pUnk));
        if (FAILED(hr)) {
            return nullptr;
        }
    }
    if (FAILED(hr)) {
        pUnk = nullptr;
    }
    if (pUnk == nullptr) {
        return nullptr;
    }

    IDataObject* pDataObj = nullptr;
    if (SUCCEEDED(pUnk->QueryInterface(kIID_IDataObject, reinterpret_cast<void**>(&pDataObj)))) {
        FORMATETC fe = PsPropsetFormatEtc();
        STGMEDIUM stm;
        stm.tymed = TYMED_ISTREAM;
        stm.pstm = pStream;
        stm.pUnkForRelease = nullptr;
        hr = pDataObj->SetData(&fe, &stm, FALSE);
        pDataObj->Release();
        if (SUCCEEDED(hr)) {
            return pUnk;
        }
    }
    pUnk->Release();
    return nullptr;
}

// RVA 0x1f21a8 (mfc140u, unexported) -- load a font from a VT_BLOB stream:
//   pFont = NULL; pPersStm = NULL;
//   if (FAILED(pStream->Read(&clsid, 16, NULL))) goto fail;               ; vtable +0x18
//   if (clsid is CLSID_StdFont or {FB8F0823-...})
//       hr = OleCreateFontIndirect(&<Helv FONTDESC>, IID_IFont, &pFont);
//   else
//       hr = CoCreateInstance(clsid, NULL, CLSCTX_INPROC_SERVER, IID_IFont, &pFont);
//   if (SUCCEEDED(hr)) {
//       if (pFont == NULL) AfxThrowInvalidArgException();
//       pFont->QueryInterface(IID_IPersistStream, &pPersStm);              ; result ignored
//   }
//   if (pPersStm != NULL) {
//       hr = pPersStm->Load(pStream); pPersStm->Release();                 ; vtable +0x28
//       if (SUCCEEDED(hr)) return pFont;
//   }
// fail:
//   if (pFont) pFont->Release();
//   return NULL;
IFont* PsLoadFontFromStream(IStream* pStream) {
    IFont* pFont = nullptr;
    IPersistStream* pPersStm = nullptr;
    CLSID clsid;
    if (SUCCEEDED(pStream->Read(&clsid, sizeof(CLSID), nullptr))) {
        HRESULT hr;
        if (SameGuid(clsid, kCLSID_StdFont) || SameGuid(clsid, kCLSID_AltFont)) {
            hr = ::OleCreateFontIndirect(const_cast<FONTDESC*>(&kFontDescHelv), kIID_IFont,
                                         reinterpret_cast<void**>(&pFont));
        } else {
            hr = ::CoCreateInstance(clsid, nullptr, CLSCTX_INPROC_SERVER, kIID_IFont,
                                    reinterpret_cast<void**>(&pFont));
        }
        if (SUCCEEDED(hr)) {
            if (pFont == nullptr) {
                impl__AfxThrowInvalidArgException__YAXXZ();
            }
            pFont->QueryInterface(kIID_IPersistStream, reinterpret_cast<void**>(&pPersStm));
        }
        if (pPersStm != nullptr) {
            hr = pPersStm->Load(pStream);
            pPersStm->Release();
            if (SUCCEEDED(hr)) {
                return pFont;
            }
        }
    }
    if (pFont != nullptr) {
        pFont->Release();
    }
    return nullptr;
}

// _AfxIsSameFont, RVA 0x1e4748 (mfc140u, unexported) -- the same helper
// core/ole/CPropbagPropExchange.cpp transcribes; its call-site/offset skeleton
// was re-read here (IID_IFont QI, IsEqual at +0xa8, default FONTDESC 0x18034b878,
// getters at +0x48/+0x58/+0x68/+0x88/+0x78/+0x28/+0x18 against FONTDESC +0x1c/
// +0x20/+0x24/+0x1a/+0x18/+0x10/+0x8):
//   if (font.m_pFont == NULL) return FALSE;
//   if (pFontDispAmbient != NULL) {
//       bSame = FALSE;
//       if (SUCCEEDED(pFontDispAmbient->QueryInterface(IID_IFont, &pFontAmbient))) {
//           bSame = pFontAmbient->IsEqual(font.m_pFont) == S_OK;
//           pFontAmbient->Release();
//       }
//       return bSame;
//   }
//   if (pFontDesc == NULL) pFontDesc = &<MS Shell Dlg FONTDESC>;
//   short-circuit chain, each getter's HRESULT ignored: get_Italic, get_Underline,
//   get_Strikethrough, get_Charset, get_Weight, get_Size (8-byte memcmp), then
//   get_Name compared with wcscmp through two CStrings built by the CString(LPCWSTR)
//   ctor (0xdcb0), and the BSTR freed.  Retail NULL-tests the second CString's
//   buffer and AtlThrow(E_FAIL)s on NULL; that ctor maps a NULL lpstrName to the
//   (non-NULL) empty-string buffer, so the throw is unreachable -- a NULL name just
//   compares as L"".  The same dead test is kept below.
// Deviation: the getter out-parameters are zero-initialised here; retail does not
// initialise them, so a getter that fails without writing its out-parameter compares
// whatever the stack slot held (for get_Name, the slot at 0x20(%rbp) still holds the
// get_Size result, which retail would then pass to SysFreeString).
int AfxIsSameFont(const PS_CFontHolder& font, const FONTDESC* pFontDesc, IFontDisp* pFontDispAmbient) {
    IFont* pFont = font.m_pFont;
    if (pFont == nullptr) {
        return FALSE;
    }
    int bSame = FALSE;
    if (pFontDispAmbient != nullptr) {
        IFont* pFontAmbient = nullptr;
        if (SUCCEEDED(pFontDispAmbient->QueryInterface(kIID_IFont, reinterpret_cast<void**>(&pFontAmbient)))) {
            bSame = (pFontAmbient->IsEqual(font.m_pFont) == S_OK) ? TRUE : FALSE;
            pFontAmbient->Release();
        }
        return bSame;
    }
    if (pFontDesc == nullptr) {
        pFontDesc = &kFontDescDefault;
    }
    BOOL bFlag = FALSE;
    pFont->get_Italic(&bFlag);
    bSame = (bFlag == pFontDesc->fItalic) ? TRUE : FALSE;
    if (bSame) {
        bFlag = FALSE;
        font.m_pFont->get_Underline(&bFlag);
        bSame = (bFlag == pFontDesc->fUnderline) ? TRUE : FALSE;
    }
    if (bSame) {
        bFlag = FALSE;
        font.m_pFont->get_Strikethrough(&bFlag);
        bSame = (bFlag == pFontDesc->fStrikethrough) ? TRUE : FALSE;
    }
    if (bSame) {
        SHORT sTemp = 0;
        font.m_pFont->get_Charset(&sTemp);
        bSame = (sTemp == pFontDesc->sCharset) ? TRUE : FALSE;
    }
    if (bSame) {
        SHORT sTemp = 0;
        font.m_pFont->get_Weight(&sTemp);
        bSame = (sTemp == pFontDesc->sWeight) ? TRUE : FALSE;
    }
    if (bSame) {
        CY cyTemp = {};
        font.m_pFont->get_Size(&cyTemp);
        bSame = (std::memcmp(&cyTemp, &pFontDesc->cySize, sizeof(CY)) == 0) ? TRUE : FALSE;
    }
    if (bSame) {
        BSTR bstrName = nullptr;
        font.m_pFont->get_Name(&bstrName);
        CString strName(bstrName);
        CString strDescName(pFontDesc->lpstrName);
        if (strDescName.GetString() == nullptr) {
            impl__AfxThrowOleException__YAXJ_Z(E_FAIL);
        }
        bSame = (wcscmp(strName.GetString(), strDescName.GetString()) == 0) ? TRUE : FALSE;
        ::SysFreeString(bstrName);
    }
    return bSame;
}

// Scalar deleting destructor, vtable slot 5 -- RVA 0x1f1aa0 (mfc140u, unexported):
//   if (flags & 1) sized-delete(this, 0x30);  return this;
// There is no destructor body (nothing is released; m_lpStorage was never
// AddRef'd).  The sized delete (0x2b77b0) ends in the operator delete body;
// OpenMFC's operator delete is the exported ??3@YAXPEAX@Z thunk (free), which
// pairs with its malloc-based ??2@YAPEAX_K@Z.
void* MS_ABI PropsetScalarDeletingDtor(void* pThis, unsigned int flags) {
    if (flags & 1) {
        impl___3_YAXPEAX_Z(pThis);
    }
    return pThis;
}

// Vtable in retail slot order (see the header comment).
void* const kPropsetPropExchangeVtbl[6] = {
    reinterpret_cast<void*>(&impl__ExchangeVersion_CPropExchange__UEAAHAEAKKH_Z),
    reinterpret_cast<void*>(&impl__ExchangeProp_CPropsetPropExchange__UEAAHPEB_WGPEAXPEBX_Z),
    reinterpret_cast<void*>(&impl__ExchangeBlobProp_CPropsetPropExchange__UEAAHPEB_WPEAPEAXPEAX_Z),
    reinterpret_cast<void*>(&impl__ExchangeFontProp_CPropsetPropExchange__UEAAHPEB_WAEAVCFontHolder__PEBUtagFONTDESC__PEAUIFontDisp___Z),
    reinterpret_cast<void*>(&impl__ExchangePersistentProp_CPropsetPropExchange__UEAAHPEB_WPEAPEAUIUnknown__AEBU_GUID__PEAU2__Z),
    reinterpret_cast<void*>(&PropsetScalarDeletingDtor),
};

}  // namespace

// Symbol: ??0CPropsetPropExchange@@QEAA@AEAVCPropertySection@@PEAUIStorage@@H@Z
// Transcribed from RVA 0x1f1a70 (mfc140u) -- see the header comment for the full
// body.  (The previous body stored nothing, not even the vptr, so an exchanger built
// through this export could not dispatch.)
extern "C" void* MS_ABI impl___0CPropsetPropExchange__QEAA_AEAVCPropertySection__PEAUIStorage__H_Z(
    void* pThis, void* pSection, void* pStorage, int bLoading) {
    PS_CPropsetPropExchange* self = ps(pThis);
    self->vptr = kPropsetPropExchangeVtbl;
    self->m_bAsync = 0;
    self->m_dwVersion = 0;
    self->m_psec = pSection;
    self->m_lpStorage = static_cast<IStorage*>(pStorage);
    self->m_dwPropID = 0xff;
    self->m_bLoading = bLoading;
    return pThis;
}

// Symbol: ?ExchangeBlobProp@CPropsetPropExchange@@UEAAHPEB_WPEAPEAXPEAX@Z
// Transcribed from RVA 0x1f1f40 (mfc140u):
//   if (phBlob == NULL || pszPropName == NULL) AfxThrowInvalidArgException();
//   if (m_bLoading) {
//       if (*phBlob != NULL) { GlobalFree(*phBlob); *phBlob = NULL; }
//       if (m_psec.GetID(pszPropName, &dwPropID) &&
//           (pProp = m_psec.GetProperty(dwPropID)) != NULL &&          ; inlined walk
//           (pvBlob = pProp->Get(NULL)) != NULL) {                    ; call 0x261c40
//           if (*(DWORD*)pvBlob == 0) return TRUE;                    ; empty blob: nothing loaded
//           if (_AfxInitBlob(phBlob, pvBlob)) return TRUE;            ; call 0x1ef6e8
//       }
//       if (hBlobDefault != NULL) _AfxCopyBlob(phBlob, hBlobDefault);  ; call 0x1ef788, result ignored
//       return TRUE;
//   }
//   bSuccess = TRUE;  ++m_dwPropID;
//   pvBlob = NULL;  if (*phBlob != NULL) pvBlob = GlobalLock(*phBlob);
//   DWORD dwEmpty = 0;
//   if (!m_psec.SetName(m_dwPropID, pszPropName) ||                   ; call 0x262c30
//       !m_psec.Set(m_dwPropID, pvBlob ? pvBlob : &dwEmpty, VT_BLOB))  ; call 0x2625f0
//       bSuccess = FALSE;
//   if (*phBlob != NULL && pvBlob != NULL) GlobalUnlock(*phBlob);
//   return bSuccess;
// The load path returns TRUE on every route, and the save path always writes
// (there is no default comparison).
extern "C" int MS_ABI impl__ExchangeBlobProp_CPropsetPropExchange__UEAAHPEB_WPEAPEAXPEAX_Z(
    void* pThis, const wchar_t* pszPropName, HGLOBAL* phBlob, HGLOBAL hBlobDefault) {
    PS_CPropsetPropExchange* self = ps(pThis);
    if (phBlob == nullptr || pszPropName == nullptr) {
        impl__AfxThrowInvalidArgException__YAXXZ();
    }
    if (self->m_bLoading != 0) {
        if (*phBlob != nullptr) {
            ::GlobalFree(*phBlob);
            *phBlob = nullptr;
        }
        unsigned long dwPropID = 0;
        if (impl__GetID_CPropertySection__QEAAHPEB_WPEAK_Z(self->m_psec, pszPropName, &dwPropID)) {
            void* pProp = impl__GetProperty_CPropertySection__QEAAPEAVCProperty__K_Z(self->m_psec, dwPropID);
            if (pProp != nullptr) {
                const void* pvBlob = impl__Get_CProperty__QEAAPEAXPEAK_Z(pProp, nullptr);
                if (pvBlob != nullptr) {
                    if (*static_cast<const DWORD*>(pvBlob) == 0) {
                        return TRUE;
                    }
                    if (AfxInitBlob(phBlob, pvBlob)) {
                        return TRUE;
                    }
                }
            }
        }
        if (hBlobDefault != nullptr) {
            AfxCopyBlob(phBlob, hBlobDefault);
        }
        return TRUE;
    }

    int bSuccess = TRUE;
    ++self->m_dwPropID;
    void* pvBlob = nullptr;
    if (*phBlob != nullptr) {
        pvBlob = ::GlobalLock(*phBlob);
    }
    DWORD dwEmpty = 0;
    if (!impl__SetName_CPropertySection__QEAAHKPEB_W_Z(self->m_psec, self->m_dwPropID, pszPropName) ||
        !impl__Set_CPropertySection__QEAAHKPEAXK_Z(self->m_psec, self->m_dwPropID,
                                                   pvBlob != nullptr ? pvBlob : &dwEmpty, VT_BLOB)) {
        bSuccess = FALSE;
    }
    if (*phBlob != nullptr && pvBlob != nullptr) {
        ::GlobalUnlock(*phBlob);
    }
    return bSuccess;
}

// Symbol: ?ExchangeFontProp@CPropsetPropExchange@@UEAAHPEB_WAEAVCFontHolder@@PEBUtagFONTDESC@@PEAUIFontDisp@@@Z
// Transcribed from RVA 0x1f2c60 (mfc140u):
//   if (m_bLoading) {
//       bSuccess = FALSE;
//       if ((pStream = <0x1f2580>(m_psec, pszPropName, &dwType)) != NULL) {
//           pFont = NULL;
//           if (dwType == VT_BLOB)      pFont = <0x1f21a8>(pStream);
//           else if (dwType == 0x4b)    pFont = <0x1f2300>(pStream, IID_IFont);
//           if (pFont != NULL) { font.SetFont(pFont); bSuccess = TRUE; }   ; call 0x1e4b10
//           pStream->Release();
//           if (bSuccess) return TRUE;
//       }
//       font.InitializeFont(pFontDesc, pFontDispAmbient);                 ; call 0x1e4680
//       return FALSE;
//   }
//   if (font.m_pFont == NULL) return TRUE;
//   if (_AfxIsSameFont(font, pFontDesc, pFontDispAmbient)) return TRUE;   ; call 0x1e4748
//   ++m_dwPropID;
//   return m_psec.SetName(m_dwPropID, pszPropName) &&
//          <0x1f2678>(font.m_pFont, m_psec, m_dwPropID);
// No argument is validated.  (A non-NULL pFont handed to SetFont is not Released
// here; transcribed as-is.)
// Note (not a deviation of this body): core/ole/CFontHolder.cpp currently keeps the
// font in a side table -- its exported ctor, SetFont and InitializeFont thunks never
// write +0x0 -- so after a load through this function +0x0 is unchanged, and the
// save path tests and dispatches through whatever +0x0 holds, exactly as retail does
// through its real m_pFont.  See the same note in core/ole/CPropbagPropExchange.cpp.
extern "C" int MS_ABI impl__ExchangeFontProp_CPropsetPropExchange__UEAAHPEB_WAEAVCFontHolder__PEBUtagFONTDESC__PEAUIFontDisp___Z(
    void* pThis, const wchar_t* pszPropName, void* pFontHolder, const FONTDESC* pFontDesc, IFontDisp* pFontDispAmbient) {
    PS_CPropsetPropExchange* self = ps(pThis);
    PS_CFontHolder& font = *static_cast<PS_CFontHolder*>(pFontHolder);
    if (self->m_bLoading != 0) {
        int bSuccess = FALSE;
        DWORD dwType = 0;
        IStream* pStream = PsGetStreamFromPropSet(self->m_psec, pszPropName, &dwType);
        if (pStream != nullptr) {
            IFont* pFont = nullptr;
            if (dwType == kVtBlob) {
                pFont = PsLoadFontFromStream(pStream);
            } else if (dwType == kVtPropsetBlob) {
                pFont = static_cast<IFont*>(PsCreateObjectFromStreamedPropset(pStream, kIID_IFont));
            }
            if (pFont != nullptr) {
                impl__SetFont_CFontHolder__QEAAXPEAUIFont___Z(pFontHolder, pFont);
                bSuccess = TRUE;
            }
            pStream->Release();
            if (bSuccess) {
                return TRUE;
            }
        }
        impl__InitializeFont_CFontHolder__QEAAXPEBUtagFONTDESC__PEAUIDispatch___Z(
            pFontHolder, pFontDesc, pFontDispAmbient);
        return FALSE;
    }

    if (font.m_pFont == nullptr) {
        return TRUE;
    }
    if (AfxIsSameFont(font, pFontDesc, pFontDispAmbient)) {
        return TRUE;
    }
    ++self->m_dwPropID;
    if (impl__SetName_CPropertySection__QEAAHKPEB_W_Z(self->m_psec, self->m_dwPropID, pszPropName) &&
        PsSaveObjectInPropset(font.m_pFont, self->m_psec, self->m_dwPropID)) {
        return TRUE;
    }
    return FALSE;
}

// Symbol: ?ExchangePersistentProp@CPropsetPropExchange@@UEAAHPEB_WPEAPEAUIUnknown@@AEBU_GUID@@PEAU2@@Z
// Transcribed from RVA 0x1f2840 (mfc140u):
//   if (ppUnk == NULL) AfxThrowInvalidArgException();
//   if (m_bLoading) {
//       bSuccess = FALSE;
//       _AfxRelease(ppUnk); *ppUnk = NULL;                            ; call 0x26ccc4
//       if ((pStream = <0x1f2580>(m_psec, pszPropName, &dwType)) != NULL) {
//           if (dwType == VT_BLOB) {
//               // class-ID peek, inlined: read it, then seek back 16 bytes
//               if (SUCCEEDED(ReadClassStm(pStream, &clsid)) &&
//                   SUCCEEDED(pStream->Seek(-16, STREAM_SEEK_CUR, NULL))) {
//                   if (clsid is CLSID_StdPicture or {FB8F0824-...})
//                       bSuccess = SUCCEEDED(ReadClassStm(pStream, &clsid)) &&
//                                  SUCCEEDED(OleLoadPicture(pStream, 0, FALSE, iid, ppUnk));  ; OLEAUT32 #418
//                   else
//                       bSuccess = SUCCEEDED(OleLoadFromStream(pStream, iid, ppUnk));
//               }
//           } else if (dwType == 0x4b) {
//               *ppUnk = <0x1f2300>(pStream, iid);                     ; bSuccess stays FALSE
//           }
//           pStream->Release();
//       }
//       if (!bSuccess && pUnkDefault != NULL)
//           bSuccess = SUCCEEDED(pUnkDefault->QueryInterface(iid, ppUnk));
//       return bSuccess;
//   }
//   if (*ppUnk == NULL || *ppUnk == pUnkDefault) return TRUE;
//   if (pUnkDefault != NULL) {
//       // same-object test: both QueryInterface(iid) succeed and return the same
//       // pointer; each interface obtained is Released (the default's first).
//       if (bSame) return TRUE;
//   }
//   ++m_dwPropID;
//   if (!m_psec.SetName(m_dwPropID, pszPropName)) return FALSE;
//   return <inlined copy of 0x1f2678>(*ppUnk, m_psec, m_dwPropID);
// Two retail quirks transcribed as-is: the 0x4b load path stores the created object
// in *ppUnk but leaves bSuccess FALSE, so the function then returns FALSE -- or, with
// a pUnkDefault, overwrites *ppUnk with the default's interface without Releasing the
// created object; and pszPropName is not validated.
// The save tail is retail's inlined copy of the helper at 0x1f2678 (same
// IDataObject/GetData/0x4b-blob sequence, same IPersistStream/OleSaveToStream/
// VT_BLOB fallback, same NULL-*ppUnk -> FALSE test, compared call by call and
// branch by branch), so that helper is called here.
extern "C" int MS_ABI impl__ExchangePersistentProp_CPropsetPropExchange__UEAAHPEB_WPEAPEAUIUnknown__AEBU_GUID__PEAU2__Z(
    void* pThis, const wchar_t* pszPropName, IUnknown** ppUnk, const GUID* piid, IUnknown* pUnkDefault) {
    PS_CPropsetPropExchange* self = ps(pThis);
    if (ppUnk == nullptr) {
        impl__AfxThrowInvalidArgException__YAXXZ();
    }
    if (self->m_bLoading != 0) {
        int bSuccess = FALSE;
        AfxReleaseUnk(ppUnk);
        *ppUnk = nullptr;
        DWORD dwType = 0;
        IStream* pStream = PsGetStreamFromPropSet(self->m_psec, pszPropName, &dwType);
        if (pStream != nullptr) {
            if (dwType == kVtBlob) {
                CLSID clsid;
                LARGE_INTEGER liBack;
                liBack.QuadPart = -16;
                if (SUCCEEDED(::ReadClassStm(pStream, &clsid)) &&
                    SUCCEEDED(pStream->Seek(liBack, STREAM_SEEK_CUR, nullptr))) {
                    if (SameGuid(clsid, kCLSID_StdPicture) || SameGuid(clsid, kCLSID_AltPicture)) {
                        bSuccess = (SUCCEEDED(::ReadClassStm(pStream, &clsid)) &&
                                    SUCCEEDED(::OleLoadPicture(pStream, 0, FALSE, *piid,
                                                               reinterpret_cast<void**>(ppUnk)))) ? TRUE : FALSE;
                    } else {
                        bSuccess = SUCCEEDED(::OleLoadFromStream(pStream, *piid,
                                                                 reinterpret_cast<void**>(ppUnk))) ? TRUE : FALSE;
                    }
                }
            } else if (dwType == kVtPropsetBlob) {
                *ppUnk = PsCreateObjectFromStreamedPropset(pStream, *piid);
            }
            pStream->Release();
        }
        if (!bSuccess && pUnkDefault != nullptr) {
            bSuccess = SUCCEEDED(pUnkDefault->QueryInterface(*piid, reinterpret_cast<void**>(ppUnk))) ? TRUE : FALSE;
        }
        return bSuccess;
    }

    IUnknown* pUnk = *ppUnk;
    if (pUnk == nullptr || pUnk == pUnkDefault) {
        return TRUE;
    }
    if (pUnkDefault != nullptr) {
        // Retail order: QI *ppUnk into a local (-0x41(%rbp)); only on success QI
        // pUnkDefault (-0x1(%rbp)); on that success compare and Release the
        // default's interface; then Release the first one.
        IUnknown* pUnk1 = nullptr;
        IUnknown* pUnk2 = nullptr;
        int bSame = FALSE;
        if (SUCCEEDED(pUnk->QueryInterface(*piid, reinterpret_cast<void**>(&pUnk1)))) {
            if (SUCCEEDED(pUnkDefault->QueryInterface(*piid, reinterpret_cast<void**>(&pUnk2)))) {
                bSame = (pUnk1 == pUnk2) ? TRUE : FALSE;
                pUnk2->Release();
            }
            pUnk1->Release();
        }
        if (bSame) {
            return TRUE;
        }
    }
    ++self->m_dwPropID;
    if (!impl__SetName_CPropertySection__QEAAHKPEB_W_Z(self->m_psec, self->m_dwPropID, pszPropName)) {
        return FALSE;
    }
    return PsSaveObjectInPropset(*ppUnk, self->m_psec, self->m_dwPropID) ? TRUE : FALSE;
}

// Symbol: ?ExchangeProp@CPropsetPropExchange@@UEAAHPEB_WGPEAXPEBX@Z
// Transcribed from RVA 0x1f1cb0 (mfc140u):
//   if (m_bLoading) {
//       if (!m_psec.GetID(pszPropName, &dwPropID) ||                   ; call 0x262bb0
//           (pProp = m_psec.GetProperty(dwPropID)) == NULL ||          ; inlined walk
//           (pvData = pProp->Get(NULL)) == NULL)                       ; call 0x261c40
//           return _AfxCopyPropValue(vtProp, pvProp, pvDefault);      ; call 0x1ef7d8
//       bSuccess = FALSE;
//       vtData = (VARTYPE)pProp->m_dwType;                             ; movzwl 0x4(%rdi)
//       CString strTmp;
//       if (vtData == VT_BSTR || vtData == VT_LPWSTR)
//           strTmp.SetString((LPCWSTR)pvData, wcslen(pvData));         ; 0x2e30
//       else if (vtData == VT_LPSTR)
//           strTmp = (LPCSTR)pvData;                                   ; call 0x1cd480
//       switch (vtProp) {
//         VT_I2, VT_I4, VT_R4, VT_R8, VT_CY:
//           bSuccess = <0x1f1afc>(pvProp, vtProp, pvData, vtData);  break;
//         VT_BSTR, VT_LPSTR:
//           bSuccess = _AfxCopyPropValue(VT_BSTR, pvProp, &strTmp);  break;
//         VT_BOOL: {
//           BSTR bstr = NULL;  pSrc = pvData;  vtSrc = vtData;
//           if (vtData is VT_BSTR, VT_LPSTR or VT_LPWSTR) {
//               bstr = SysAllocStringLen(strTmp, strTmp.GetLength());  ; OLEAUT32 #4
//               if (bstr == NULL) AtlThrow(E_OUTOFMEMORY);             ; 0x3160
//               pSrc = &bstr;  vtSrc = VT_BSTR;
//           }
//           bSuccess = <0x1f1afc>(&boolTmp, VT_BOOL, pSrc, vtSrc);
//           SysFreeString(bstr);                                       ; OLEAUT32 #6, unconditional
//           if (bSuccess) *(BOOL*)pvProp = (boolTmp != 0);             ; 16-bit test
//           break; }
//         default: break;                                              ; VT_UI1 included -> FALSE
//       }
//       return bSuccess;                                               ; ~strTmp
//   }
//   if (_AfxIsSamePropValue(vtProp, pvProp, pvDefault)) return TRUE;  ; call 0x1f1c0c
//   ++m_dwPropID;
//   switch (vtProp) {
//     VT_I2, VT_I4, VT_R4, VT_R8, VT_CY: pvData = pvProp;  break;
//     VT_BSTR, VT_LPSTR: pvData = ((CString*)pvProp)->m_pszData;  break;
//     VT_BOOL: dwTmp = *(BOOL*)pvProp ? -1 : 0;  pvData = &dwTmp;  break;   ; neg/sbb
//     default: pvData = NULL;
//   }
//   return m_psec.SetName(m_dwPropID, pszPropName) &&                  ; call 0x262c30
//          m_psec.Set(m_dwPropID, pvData, vtProp);                     ; call 0x2625f0
// Retail quirks transcribed as-is: no argument is validated; a VT_BSTR-typed stored
// property reaching the numeric conversion is copied into the VARIANT as its first 8
// raw bytes (<0x1f1afc> sizes VT_BSTR as 8 and pProp->Get returns the counted block
// for VT_BSTR); a numeric stored property read into a string yields an empty string
// with bSuccess TRUE; and <0x1f1afc> converts in place (same VARIANT as source and
// destination) while the BSTR it was given is freed afterwards regardless.
// AtlThrow(E_OUTOFMEMORY) is AfxThrowMemoryException (the 0x333c hook, see
// core/ole/CPropExchange.cpp).
extern "C" int MS_ABI impl__ExchangeProp_CPropsetPropExchange__UEAAHPEB_WGPEAXPEBX_Z(
    void* pThis, const wchar_t* pszPropName, unsigned short vtProp, void* pvProp, const void* pvDefault) {
    PS_CPropsetPropExchange* self = ps(pThis);
    if (self->m_bLoading != 0) {
        unsigned long dwPropID = 0;
        PS_Property* pProp = nullptr;
        void* pvData = nullptr;
        if (!impl__GetID_CPropertySection__QEAAHPEB_WPEAK_Z(self->m_psec, pszPropName, &dwPropID) ||
            (pProp = static_cast<PS_Property*>(
                 impl__GetProperty_CPropertySection__QEAAPEAVCProperty__K_Z(self->m_psec, dwPropID))) == nullptr ||
            (pvData = impl__Get_CProperty__QEAAPEAXPEAK_Z(pProp, nullptr)) == nullptr) {
            return AfxCopyPropValue(vtProp, pvProp, pvDefault);
        }

        int bSuccess = FALSE;
        const unsigned short vtData = static_cast<unsigned short>(pProp->m_dwType);
        CString strTmp;
        if (vtData == VT_BSTR || vtData == VT_LPWSTR) {
            strTmp = static_cast<const wchar_t*>(pvData);
        } else if (vtData == VT_LPSTR) {
            impl___4__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAAAEAV01_PEBD_Z(
                &strTmp, static_cast<const char*>(pvData));
        }
        switch (vtProp) {
        case VT_I2:
        case VT_I4:
        case VT_R4:
        case VT_R8:
        case VT_CY:
            bSuccess = AfxCoerceNumber(pvProp, vtProp, pvData, vtData);
            break;
        case VT_BSTR:
        case VT_LPSTR:
            bSuccess = AfxCopyPropValue(VT_BSTR, pvProp, &strTmp);
            break;
        case VT_BOOL: {
            BSTR bstr = nullptr;
            const void* pSrc = pvData;
            unsigned short vtSrc = vtData;
            if (vtData == VT_BSTR || vtData == VT_LPSTR || vtData == VT_LPWSTR) {
                bstr = ::SysAllocStringLen(strTmp.GetString(), static_cast<UINT>(strTmp.GetLength()));
                if (bstr == nullptr) {
                    impl__AfxThrowMemoryException__YAXXZ();
                }
                pSrc = &bstr;
                vtSrc = VT_BSTR;
            }
            VARIANT_BOOL boolTmp = 0;
            bSuccess = AfxCoerceNumber(&boolTmp, VT_BOOL, pSrc, vtSrc);
            ::SysFreeString(bstr);
            if (bSuccess) {
                *static_cast<BOOL*>(pvProp) = (boolTmp != 0) ? TRUE : FALSE;
            }
            break;
        }
        default:
            break;
        }
        return bSuccess;
    }

    if (AfxIsSamePropValue(vtProp, pvProp, pvDefault)) {
        return TRUE;
    }
    ++self->m_dwPropID;
    void* pvData = nullptr;
    int dwTmp = 0;
    switch (vtProp) {
    case VT_I2:
    case VT_I4:
    case VT_R4:
    case VT_R8:
    case VT_CY:
        pvData = pvProp;
        break;
    case VT_BSTR:
    case VT_LPSTR:
        pvData = const_cast<wchar_t*>(static_cast<const CString*>(pvProp)->GetString());
        break;
    case VT_BOOL:
        dwTmp = (*static_cast<const BOOL*>(pvProp) != 0) ? -1 : 0;
        pvData = &dwTmp;
        break;
    default:
        pvData = nullptr;
        break;
    }
    if (impl__SetName_CPropertySection__QEAAHKPEB_W_Z(self->m_psec, self->m_dwPropID, pszPropName) &&
        impl__Set_CPropertySection__QEAAHKPEAXK_Z(self->m_psec, self->m_dwPropID, pvData, vtProp)) {
        return TRUE;
    }
    return FALSE;
}
