// CPropbagPropExchange — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp
//
// MFC's private IPropertyBag-backed CPropExchange (COleControl's IPersistPropertyBag
// Load/Save).  No header in include/openmfc declares the class (retail keeps it in
// the private ctlimpl.h), so the layout is pinned in-file below.  Every body in this
// file was transcribed from the retail export, disassembled from mfc140u.dll itself
// (disas.py --u); all RVAs below are mfc140u RVAs.
//
// ---------------------------------------------------------------------------
// Retail object layout, pinned by the retail ctor ??0CPropbagPropExchange@@QEAA@...
// (RVA 0x1e7b40, mfc140u):
//   movq $0,0xc(%rcx)      -> CPropExchange::m_bAsync (+0xc) and m_dwVersion (+0x10)
//   lea 0x180324848,%rax ; mov %rax,(%rcx)                       -> vptr
//   mov 0x50(%rsp),%eax ; mov %eax,0x28(%rcx)  (5th arg)         -> m_bSaveAllProperties
//   mov %rdx,0x18(%rcx)                        (IPropertyBag*)   -> m_lpPropBag
//   mov %r8,0x20(%rcx)                         (IErrorLog*)      -> m_lpErrorLog
//   mov %r9d,0x8(%rcx)                         (4th arg)         -> m_bLoading
// and by the scalar deleting destructor (vtable slot 5, RVA 0x1e7bb0, unexported),
// which frees with `mov $0x30,%edx` -> sizeof == 0x30.  The base part (+0x0..+0x17)
// is the same CPropExchange layout core/ole/CPropExchange.cpp pins.
// Neither the ctor nor any body in this file touches +0x2c (the 4 bytes between
// m_bSaveAllProperties and the 0x30 size), so no further member is modelled; in
// particular nothing here corresponds to an m_bClearMissing.
//
// Retail vftable (0x180324848 in mfc140u), read slot by slot with vtdump_u.py:
//   0 +0x00  ExchangeVersion         0x1efb30  (CPropExchange's, not overridden)
//   1 +0x08  ExchangeProp            0x1e7c20
//   2 +0x10  ExchangeBlobProp        0x1e7f10
//   3 +0x18  ExchangeFontProp        0x1e80d0
//   4 +0x20  ExchangePersistentProp  0x1e8240
//   5 +0x28  scalar deleting dtor    0x1e7bb0  (unexported)
// That slot order is the one the PX_* wrappers in core/ole/CPropExchange.cpp
// dispatch through.  The ctor/dtor below install kPropbagPropExchangeVtbl, an
// in-file table of the same six entries.
//
// Every virtual call retail makes here goes through __guard_dispatch_icall_fptr
// (`call *0x1802c7b30(%rip)` in mfc140u -- the CFG dispatch pointer, not an
// import); the real target is the slot loaded into %rax just before.  IPropertyBag
// is called at +0x18 (slot 3, Read) and +0x20 (slot 4, Write).
//
// Deviation, applied in every Exchange* body: retail copies pszPropName into a
// CStringW temporary (ctor at RVA 0xdcb0) and passes that buffer to
// IPropertyBag::Read/Write.  OpenMFC's CString(const wchar_t*) (include/openmfc/
// afxstr.h) is a plain copy -- it does not implement ATL's implicit string-resource
// load for an IS_INTRESOURCE pointer -- so the temporary would only duplicate the
// characters; pszPropName is passed directly instead.
// ---------------------------------------------------------------------------

#include "detail/ManualSmallStubImplementationsSupport.h"

#include <ocidl.h>
#include <olectl.h>
#include <oleauto.h>
#include <cerrno>
#include <cstring>
#include <wchar.h>

// Thunks defined elsewhere in the tree (each definition checked by grep).
extern "C" int MS_ABI impl__ExchangeVersion_CPropExchange__UEAAHAEAKKH_Z(          // core/ole/CPropExchange.cpp
    void* pThis, unsigned long* pdwVersionLoaded, unsigned long dwVersionDefault, int bConvert);
extern "C" void* MS_ABI impl___0CBlobProperty__QEAA_PEAX_Z(void* pThis, void* pBlob);  // core/ole/CBlobProperty.cpp
extern "C" void* MS_ABI impl__GetBlob_CBlobProperty__QEAAPEAXXZ(void* pThis);           // core/ole/CBlobProperty.cpp
extern "C" void MS_ABI impl__SetFont_CFontHolder__QEAAXPEAUIFont___Z(void* self, void* font);  // core/ole/CFontHolder.cpp
extern "C" void MS_ABI impl__InitializeFont_CFontHolder__QEAAXPEBUtagFONTDESC__PEAUIDispatch___Z(  // core/ole/CFontHolder.cpp
    void* self, const void* pFontDesc, void* pFontDispAmbient);
extern "C" void* MS_ABI impl___2_YAPEAX_K_Z(std::size_t size);                  // detail/MemcoreSupport.cpp
extern "C" void MS_ABI impl___3_YAXPEAX_Z(void* ptr);                           // detail/MemcoreSupport.cpp
extern "C" void MS_ABI impl__AfxThrowInvalidArgException__YAXXZ();              // detail/MfcExceptionsSupport.cpp
extern "C" void MS_ABI impl__AfxThrowMemoryException__YAXXZ();                  // detail/MfcExceptionsSupport.cpp
extern "C" void MS_ABI impl__AfxThrowOleException__YAXJ_Z(LONG sc);             // detail/MfcExceptionsSupport.cpp

// This file's own exports, forward-declared for the in-file vtable.
extern "C" void MS_ABI impl___1CPropbagPropExchange__UEAA_XZ(void* pThis);
extern "C" int MS_ABI impl__ExchangeProp_CPropbagPropExchange__UEAAHPEB_WGPEAXPEBX_Z(
    void* pThis, const wchar_t* pszPropName, unsigned short vtProp, void* pvProp, const void* pvDefault);
extern "C" int MS_ABI impl__ExchangeBlobProp_CPropbagPropExchange__UEAAHPEB_WPEAPEAXPEAX_Z(
    void* pThis, const wchar_t* pszPropName, HGLOBAL* phBlob, HGLOBAL hBlobDefault);
extern "C" int MS_ABI impl__ExchangeFontProp_CPropbagPropExchange__UEAAHPEB_WAEAVCFontHolder__PEBUtagFONTDESC__PEAUIFontDisp___Z(
    void* pThis, const wchar_t* pszPropName, void* pFontHolder, const FONTDESC* pFontDesc, IFontDisp* pFontDispAmbient);
extern "C" int MS_ABI impl__ExchangePersistentProp_CPropbagPropExchange__UEAAHPEB_WPEAPEAUIUnknown__AEBU_GUID__PEAU2__Z(
    void* pThis, const wchar_t* pszPropName, IUnknown** ppUnk, const GUID* piid, IUnknown* pUnkDefault);

namespace {

// Retail layout (see the header comment for the evidence).
struct PB_CPropbagPropExchange {
    void* const*  vptr;                 // 0x00
    int           m_bLoading;           // 0x08  (CPropExchange)
    int           m_bAsync;             // 0x0c  (CPropExchange)
    unsigned long m_dwVersion;          // 0x10  (CPropExchange)
    IPropertyBag* m_lpPropBag;          // 0x18
    IErrorLog*    m_lpErrorLog;         // 0x20
    int           m_bSaveAllProperties; // 0x28
};
static_assert(offsetof(PB_CPropbagPropExchange, m_bLoading) == 0x08, "CPropExchange::m_bLoading");
static_assert(offsetof(PB_CPropbagPropExchange, m_bAsync) == 0x0c, "CPropExchange::m_bAsync");
static_assert(offsetof(PB_CPropbagPropExchange, m_dwVersion) == 0x10, "CPropExchange::m_dwVersion");
static_assert(offsetof(PB_CPropbagPropExchange, m_lpPropBag) == 0x18, "CPropbagPropExchange::m_lpPropBag");
static_assert(offsetof(PB_CPropbagPropExchange, m_lpErrorLog) == 0x20, "CPropbagPropExchange::m_lpErrorLog");
static_assert(offsetof(PB_CPropbagPropExchange, m_bSaveAllProperties) == 0x28, "CPropbagPropExchange::m_bSaveAllProperties");
static_assert(sizeof(PB_CPropbagPropExchange) == 0x30, "CPropbagPropExchange is 0x30 bytes (sized delete in the deleting dtor)");

inline PB_CPropbagPropExchange* pb(void* pThis) { return static_cast<PB_CPropbagPropExchange*>(pThis); }

// CFontHolder as retail lays it out: afxctl.h's `LPFONT m_pFont` at +0x0.  Retail
// ExchangeFontProp reads +0x0 directly (`cmpq $0x0,(%rdi)`) and dispatches IFont
// methods through it.  include/openmfc/afxole.h types the member IFontDisp*, so this
// view is used instead (same approach as core/ole/CPropExchange.cpp's PX_CFontHolder).
struct PB_CFontHolder {
    IFont* m_pFont;                     // 0x00
};
static_assert(offsetof(CFontHolder, m_pFont) == 0x00, "CFontHolder::m_pFont");

// IID_IFont, the 16 bytes at 0x1802d9d88 (mfc140u .rdata) that both ExchangeFontProp
// and the same-font helper load into %rdx for QueryInterface:
// {BEF6E002-A874-101A-8BBA-00AA00300CAB}.
const GUID kIID_IFont =
    { 0xBEF6E002, 0xA874, 0x101A, { 0x8B, 0xBA, 0x00, 0xAA, 0x00, 0x30, 0x0C, 0xAB } };

// The default FONTDESC the same-font helper substitutes for a NULL pFontDesc: the
// 0x28 bytes at 0x18034b878 (mfc140u .rdata) read as
//   cbSizeofstruct 0x28, lpstrName -> 0x18034b858 = L"MS Shell Dlg",
//   cySize 0x1d4c0 (= 12.0 in CY units), sWeight 0x190 (FW_NORMAL),
//   sCharset 1 (DEFAULT_CHARSET), fItalic/fUnderline/fStrikethrough 0.
wchar_t kDefaultFontName[] = L"MS Shell Dlg";
const FONTDESC kFontDescDefault = {
    sizeof(FONTDESC), kDefaultFontName, { { 120000, 0 } }, FW_NORMAL, DEFAULT_CHARSET, FALSE, FALSE, FALSE
};
static_assert(sizeof(FONTDESC) == 0x28, "FONTDESC is 0x28 bytes (cbSizeofstruct in retail)");

// CBlobProperty's Release is vtable slot 2 (core/ole/CBlobProperty.cpp, retail
// vftable 0x180324880 in mfc140u); retail calls it as `mov 0x10(%rdx),%rax`.
inline void BlobRelease(void* pBlob) {
    using ReleaseFn = unsigned long (MS_ABI*)(void*);
    reinterpret_cast<ReleaseFn>((*static_cast<void* const* const*>(pBlob))[2])(pBlob);
}

// `new CBlobProperty(hBlob)`, inlined in retail ExchangeBlobProp as
// `call 0x1800027f0` (??2@YAPEAX_K@Z) with 0x18, then the ctor body stored inline.
// Deviation: retail's operator new (RVA 0x27f0) loops through the module's new
// handler and returns NULL only if that handler gives up, after which the retail
// Read/Write path would fault on the NULL object.  OpenMFC's ??2 is a bare malloc
// with no new handler, so a NULL is raised as AfxThrowMemoryException instead.
void* NewBlobProperty(HGLOBAL hBlob) {
    void* p = impl___2_YAPEAX_K_Z(0x18);
    if (p == nullptr) {
        impl__AfxThrowMemoryException__YAXXZ();
    }
    return impl___0CBlobProperty__QEAA_PEAX_Z(p, hBlob);
}

// _AfxRelease, RVA 0x26ccc4 (mfc140u, unexported):
//   if (*pp != NULL) { (*pp)->Release(); *pp = NULL; }     ; vtable +0x10
void AfxReleaseUnk(IUnknown** pp) {
    if (*pp != nullptr) {
        (*pp)->Release();
        *pp = nullptr;
    }
}

// _AfxGetSizeOfVarType, RVA 0x1f1ac4 (mfc140u, unexported):
//   VT_I2 2, VT_I4 4, VT_R4 4, VT_R8 8, VT_CY 8, VT_BSTR 8, VT_BOOL 2, else 0.
// (VT_BOOL yields 2 -- sizeof(VARIANT_BOOL) -- and VT_UI1 is not listed.)
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

// _AfxCopyPropValue, RVA 0x1ef7d8 (mfc140u, unexported):
//   if (pvSrc != NULL) switch (vt) {
//     VT_I2: 16-bit copy;  VT_I4/VT_BOOL/VT_R4: 32-bit;  VT_R8/VT_CY: 64-bit;
//     VT_UI1: byte;
//     VT_BSTR:  *(CString*)pvDest = *(const CString*)pvSrc;   ; call 0xde30 (CSimpleStringT::operator=)
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

// _AfxIsSamePropValue, RVA 0x1f1c0c (mfc140u, unexported):
//   if (pv1 == pv2) return TRUE;
//   if (pv1 == NULL || pv2 == NULL) return FALSE;
//   switch (vt) {
//     VT_I2/VT_I4/VT_R4/VT_R8/VT_CY/VT_BOOL:
//         return memcmp(pv1, pv2, _AfxGetSizeOfVarType(vt)) == 0;
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

// _AfxInitBlob, RVA 0x1ef6e8 (mfc140u, unexported):
//   bResult = FALSE;
//   if (*(DWORD*)pvSrc > 0) {
//       cb = *(DWORD*)pvSrc + 4;                               ; 64-bit add
//       *phDst = GlobalAlloc(GMEM_MOVEABLE, cb);
//       if (*phDst != NULL) {
//           p = GlobalLock(*phDst);
//           if (p != NULL) {
//               Checked::memcpy_s(p, GlobalSize(*phDst), pvSrc, cb);
//               bResult = TRUE;
//           }
//           GlobalUnlock(*phDst);                              ; also when the lock failed
//       }
//   }
//   return bResult;
// The memcpy_s failure path is inlined: memset(p, 0, size); errno = ERANGE;
// _invalid_parameter_noinfo(); AfxThrowInvalidArgException().  It is written here
// as memcpy_s (which does the zeroing, errno and invalid-parameter report itself)
// followed by the throw on a non-zero return.
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

// _AfxCopyBlob, RVA 0x1ef788 (mfc140u, unexported):
//   bResult = FALSE;
//   if ((p = GlobalLock(hSrc)) != NULL) { bResult = _AfxInitBlob(phDst, p); GlobalUnlock(hSrc); }
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

// _AfxIsSameFont, RVA 0x1e4748 (mfc140u, unexported):
//   if (font.m_pFont == NULL) return FALSE;
//   if (pFontDispAmbient != NULL) {
//       bSame = FALSE;
//       if (SUCCEEDED(pFontDispAmbient->QueryInterface(IID_IFont, &pFontAmbient))) {
//           bSame = pFontAmbient->IsEqual(font.m_pFont) == S_OK;   ; IFont +0xa8
//           pFontAmbient->Release();
//       }
//       return bSame;
//   }
//   if (pFontDesc == NULL) pFontDesc = &_afxFontDescDefault;       ; cmovne
//   then a short-circuit chain, each getter's HRESULT ignored:
//     get_Italic (+0x48)        == pFontDesc->fItalic        (+0x1c, 32-bit)
//     get_Underline (+0x58)     == pFontDesc->fUnderline     (+0x20, 32-bit)
//     get_Strikethrough (+0x68) == pFontDesc->fStrikethrough (+0x24, 32-bit)
//     get_Charset (+0x88)       == pFontDesc->sCharset       (+0x1a, 16-bit)
//     get_Weight (+0x78)        == pFontDesc->sWeight        (+0x18, 16-bit)
//     get_Size (+0x28)          memcmp 8 bytes with pFontDesc->cySize (+0x10)
//     get_Name (+0x18): CString strName(bstr); CString strDesc(pFontDesc->lpstrName);
//         strDesc buffer NULL -> AtlThrow(E_FAIL);
//         bSame = wcscmp(strName, strDesc) == 0; SysFreeString(bstr).
// Deviation: the getter out-parameters are zero-initialised here; retail leaves them
// uninitialised, so a failing getter compares (or, for get_Name, frees) stack garbage.
int AfxIsSameFont(const PB_CFontHolder& font, const FONTDESC* pFontDesc, IFontDisp* pFontDispAmbient) {
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

// Scalar deleting destructor, vtable slot 5 -- RVA 0x1e7bb0 (mfc140u, unexported):
//   ~CPropbagPropExchange();  if (flags & 1) sized-delete(this, 0x30);  return this;
// The sized delete (0x2b77b0) jumps to 0x27c0, the operator delete body; OpenMFC's
// operator delete is the exported ??3@YAXPEAX@Z thunk (free), which pairs with its
// malloc-based ??2@YAPEAX_K@Z.
void* MS_ABI PropbagScalarDeletingDtor(void* pThis, unsigned int flags) {
    impl___1CPropbagPropExchange__UEAA_XZ(pThis);
    if (flags & 1) {
        impl___3_YAXPEAX_Z(pThis);
    }
    return pThis;
}

// Vtable in retail slot order (see the header comment).
void* const kPropbagPropExchangeVtbl[6] = {
    reinterpret_cast<void*>(&impl__ExchangeVersion_CPropExchange__UEAAHAEAKKH_Z),
    reinterpret_cast<void*>(&impl__ExchangeProp_CPropbagPropExchange__UEAAHPEB_WGPEAXPEBX_Z),
    reinterpret_cast<void*>(&impl__ExchangeBlobProp_CPropbagPropExchange__UEAAHPEB_WPEAPEAXPEAX_Z),
    reinterpret_cast<void*>(&impl__ExchangeFontProp_CPropbagPropExchange__UEAAHPEB_WAEAVCFontHolder__PEBUtagFONTDESC__PEAUIFontDisp___Z),
    reinterpret_cast<void*>(&impl__ExchangePersistentProp_CPropbagPropExchange__UEAAHPEB_WPEAPEAUIUnknown__AEBU_GUID__PEAU2__Z),
    reinterpret_cast<void*>(&PropbagScalarDeletingDtor),
};

}  // namespace

// Symbol: ??0CPropbagPropExchange@@QEAA@PEAUIPropertyBag@@PEAUIErrorLog@@HH@Z
// Transcribed from RVA 0x1e7b40 (mfc140u):
//   m_bAsync = 0; m_dwVersion = 0;                ; movq $0,0xc(%rcx) (inlined base ctor)
//   vptr = CPropbagPropExchange vftable;
//   m_bSaveAllProperties = bSaveAllProperties;    ; +0x28 <- 5th arg
//   m_lpPropBag = pPropBag; m_lpErrorLog = pErrorLog; m_bLoading = bLoading;
//   if (pPropBag)  pPropBag->AddRef();            ; vtable +0x8
//   if (pErrorLog) pErrorLog->AddRef();
//   return this;
// (The previous parameter list typed the two BOOLs as `short`; the mangled name's
// `HH` is two ints.)
extern "C" void* MS_ABI impl___0CPropbagPropExchange__QEAA_PEAUIPropertyBag__PEAUIErrorLog__HH_Z(
    void* pThis, IPropertyBag* pPropBag, IErrorLog* pErrorLog, int bLoading, int bSaveAllProperties) {
    PB_CPropbagPropExchange* self = pb(pThis);
    self->m_bAsync = 0;
    self->m_dwVersion = 0;
    self->vptr = kPropbagPropExchangeVtbl;
    self->m_bSaveAllProperties = bSaveAllProperties;
    self->m_lpPropBag = pPropBag;
    self->m_lpErrorLog = pErrorLog;
    self->m_bLoading = bLoading;
    if (pPropBag != nullptr) {
        pPropBag->AddRef();
    }
    if (pErrorLog != nullptr) {
        pErrorLog->AddRef();
    }
    return pThis;
}

// Symbol: ??1CPropbagPropExchange@@UEAA@XZ
// Transcribed from RVA 0x1e7bf0 (mfc140u):
//   vptr = CPropbagPropExchange vftable;
//   _AfxRelease(&m_lpPropBag);                    ; call 0x26ccc4 with this+0x18
//   _AfxRelease(&m_lpErrorLog);                   ; call 0x26ccc4 with this+0x20
// The CPropExchange base destructor contributes nothing (inlined away).
extern "C" void MS_ABI impl___1CPropbagPropExchange__UEAA_XZ(void* pThis) {
    PB_CPropbagPropExchange* self = pb(pThis);
    self->vptr = kPropbagPropExchangeVtbl;
    AfxReleaseUnk(reinterpret_cast<IUnknown**>(&self->m_lpPropBag));
    AfxReleaseUnk(reinterpret_cast<IUnknown**>(&self->m_lpErrorLog));
}

// Symbol: ?ExchangeBlobProp@CPropbagPropExchange@@UEAAHPEB_WPEAPEAXPEAX@Z
// Transcribed from RVA 0x1e7f10 (mfc140u):
//   if (m_lpPropBag == NULL) return FALSE;
//   if (pszPropName == NULL || phBlob == NULL) AfxThrowInvalidArgException();
//   VARIANT var; memset(&var, 0, 0x18); V_VT(&var) = VT_UNKNOWN;
//   if (m_bLoading) {
//       if (*phBlob != NULL) { GlobalFree(*phBlob); *phBlob = NULL; }
//       CBlobProperty* pBlobProp = new CBlobProperty(NULL);   ; +0x0 vftable 0x180324880,
//       bSuccess = TRUE;                                      ; +0x8 = 1, +0x10 = NULL
//       V_UNKNOWN(&var) = pBlobProp;
//       if (SUCCEEDED(m_lpPropBag->Read(pszPropName, &var, m_lpErrorLog)))
//           *phBlob = pBlobProp->GetBlob();                   ; mov 0x10(%rbx),%rax
//       else {
//           bSuccess = FALSE;
//           if (hBlobDefault != NULL) bSuccess = _AfxCopyBlob(phBlob, hBlobDefault);
//       }
//   } else {
//       CBlobProperty* pBlobProp = new CBlobProperty(*phBlob);
//       V_UNKNOWN(&var) = pBlobProp;
//       bSuccess = SUCCEEDED(m_lpPropBag->Write(pszPropName, &var));
//   }
//   pBlobProp->Release();                                     ; vtable +0x10, both paths
//   return bSuccess;
// No VariantClear on either path (var only borrows pBlobProp), and the save path
// has no m_bSaveAllProperties / default comparison.
extern "C" int MS_ABI impl__ExchangeBlobProp_CPropbagPropExchange__UEAAHPEB_WPEAPEAXPEAX_Z(
    void* pThis, const wchar_t* pszPropName, HGLOBAL* phBlob, HGLOBAL hBlobDefault) {
    PB_CPropbagPropExchange* self = pb(pThis);
    if (self->m_lpPropBag == nullptr) {
        return FALSE;
    }
    if (pszPropName == nullptr || phBlob == nullptr) {
        impl__AfxThrowInvalidArgException__YAXXZ();
    }
    VARIANT var;
    std::memset(&var, 0, sizeof(var));
    V_VT(&var) = VT_UNKNOWN;

    int bSuccess;
    void* pBlobProp;
    if (self->m_bLoading != 0) {
        if (*phBlob != nullptr) {
            ::GlobalFree(*phBlob);
            *phBlob = nullptr;
        }
        pBlobProp = NewBlobProperty(nullptr);
        bSuccess = TRUE;
        V_UNKNOWN(&var) = static_cast<IUnknown*>(pBlobProp);
        if (SUCCEEDED(self->m_lpPropBag->Read(pszPropName, &var, self->m_lpErrorLog))) {
            *phBlob = static_cast<HGLOBAL>(impl__GetBlob_CBlobProperty__QEAAPEAXXZ(pBlobProp));
        } else {
            bSuccess = FALSE;
            if (hBlobDefault != nullptr) {
                bSuccess = AfxCopyBlob(phBlob, hBlobDefault);
            }
        }
    } else {
        pBlobProp = NewBlobProperty(*phBlob);
        V_UNKNOWN(&var) = static_cast<IUnknown*>(pBlobProp);
        bSuccess = SUCCEEDED(self->m_lpPropBag->Write(pszPropName, &var)) ? TRUE : FALSE;
    }
    BlobRelease(pBlobProp);
    return bSuccess;
}

// Symbol: ?ExchangeFontProp@CPropbagPropExchange@@UEAAHPEB_WAEAVCFontHolder@@PEBUtagFONTDESC@@PEAUIFontDisp@@@Z
// Transcribed from RVA 0x1e80d0 (mfc140u):
//   if (m_lpPropBag == NULL) return FALSE;
//   if (pszPropName == NULL) AfxThrowInvalidArgException();
//   VARIANT var; memset(&var, 0, 0x18); V_VT(&var) = VT_UNKNOWN;
//   if (m_bLoading) {
//       LPFONT pFont = NULL;
//       if (SUCCEEDED(m_lpPropBag->Read(pszPropName, &var, m_lpErrorLog)) &&
//           SUCCEEDED(V_UNKNOWN(&var)->QueryInterface(IID_IFont, (void**)&pFont))) {
//           bSuccess = TRUE;  font.SetFont(pFont);            ; call 0x1e4b10
//       } else {
//           bSuccess = FALSE; font.InitializeFont(pFontDesc, pFontDispAmbient);  ; call 0x1e4680
//       }
//       VariantClear(&var);                                   ; OLEAUT32 #9
//   } else if (font.m_pFont == NULL) {
//       bSuccess = TRUE;
//   } else if (_AfxIsSameFont(font, pFontDesc, pFontDispAmbient) && !m_bSaveAllProperties) {
//       bSuccess = TRUE;                                      ; call 0x1e4748, then +0x28
//   } else {
//       V_UNKNOWN(&var) = font.m_pFont;                       ; borrowed: no AddRef, no VariantClear
//       bSuccess = SUCCEEDED(m_lpPropBag->Write(pszPropName, &var));
//   }
//   return bSuccess;
// Retail does not NULL-check V_UNKNOWN(&var) after a successful Read, and hands the
// QueryInterface'd reference to SetFont without releasing it; both transcribed as-is.
// Note (not a deviation of this body): core/ole/CFontHolder.cpp currently keeps the
// font in a side table -- its exported ctor, SetFont and InitializeFont thunks never
// write +0x0.  So after a load through this function +0x0 is unchanged, and for a
// holder built by the exported ctor thunk (which is what MSVC clients call; retail
// afxctl.h does not define the ctor inline) +0x0 is whatever the storage held, and
// the save path below will test and dispatch through that value exactly as retail
// dispatches through its real m_pFont.  Only a holder built by include/openmfc's
// inline ctor (m_pFont(nullptr)) has a defined +0x0, and it is NULL: the save path
// then returns TRUE without writing.
extern "C" int MS_ABI impl__ExchangeFontProp_CPropbagPropExchange__UEAAHPEB_WAEAVCFontHolder__PEBUtagFONTDESC__PEAUIFontDisp___Z(
    void* pThis, const wchar_t* pszPropName, void* pFontHolder, const FONTDESC* pFontDesc, IFontDisp* pFontDispAmbient) {
    PB_CPropbagPropExchange* self = pb(pThis);
    if (self->m_lpPropBag == nullptr) {
        return FALSE;
    }
    if (pszPropName == nullptr) {
        impl__AfxThrowInvalidArgException__YAXXZ();
    }
    VARIANT var;
    std::memset(&var, 0, sizeof(var));
    V_VT(&var) = VT_UNKNOWN;

    PB_CFontHolder& font = *static_cast<PB_CFontHolder*>(pFontHolder);
    int bSuccess;
    if (self->m_bLoading != 0) {
        IFont* pFont = nullptr;
        if (SUCCEEDED(self->m_lpPropBag->Read(pszPropName, &var, self->m_lpErrorLog)) &&
            SUCCEEDED(V_UNKNOWN(&var)->QueryInterface(kIID_IFont, reinterpret_cast<void**>(&pFont)))) {
            bSuccess = TRUE;
            impl__SetFont_CFontHolder__QEAAXPEAUIFont___Z(pFontHolder, pFont);
        } else {
            bSuccess = FALSE;
            impl__InitializeFont_CFontHolder__QEAAXPEBUtagFONTDESC__PEAUIDispatch___Z(
                pFontHolder, pFontDesc, pFontDispAmbient);
        }
        ::VariantClear(&var);
    } else if (font.m_pFont == nullptr) {
        bSuccess = TRUE;
    } else if (AfxIsSameFont(font, pFontDesc, pFontDispAmbient) && self->m_bSaveAllProperties == 0) {
        bSuccess = TRUE;
    } else {
        V_UNKNOWN(&var) = font.m_pFont;
        bSuccess = SUCCEEDED(self->m_lpPropBag->Write(pszPropName, &var)) ? TRUE : FALSE;
    }
    return bSuccess;
}

// Symbol: ?ExchangePersistentProp@CPropbagPropExchange@@UEAAHPEB_WPEAPEAUIUnknown@@AEBU_GUID@@PEAU2@@Z
// Transcribed from RVA 0x1e8240 (mfc140u):
//   if (m_lpPropBag == NULL) return FALSE;
//   if (pszPropName == NULL || ppUnk == NULL) AfxThrowInvalidArgException();
//   VARIANT var; memset(&var, 0, 0x18); V_VT(&var) = VT_UNKNOWN;
//   if (m_bLoading) {
//       _AfxRelease(ppUnk); *ppUnk = NULL;                    ; call 0x26ccc4
//       if (SUCCEEDED(m_lpPropBag->Read(pszPropName, &var, m_lpErrorLog)) &&
//           SUCCEEDED(V_UNKNOWN(&var)->QueryInterface(iid, (void**)ppUnk)))
//           bSuccess = TRUE;
//       else if (pUnkDefault != NULL)
//           bSuccess = SUCCEEDED(pUnkDefault->QueryInterface(iid, (void**)ppUnk));
//       else
//           bSuccess = TRUE;                                  ; retail: mov $0x1,%ebx
//       VariantClear(&var);
//   } else if (*ppUnk == NULL) {
//       bSuccess = TRUE;
//   } else {
//       // same-object test, inlined: pointer equality, else (pUnkDefault != NULL and)
//       // both QueryInterface(iid) succeed and return the same pointer; each
//       // interface obtained is Released.
//       if (bSame && !m_bSaveAllProperties) bSuccess = TRUE;
//       else {
//           V_UNKNOWN(&var) = *ppUnk;                         ; borrowed: no AddRef, no VariantClear
//           bSuccess = SUCCEEDED(m_lpPropBag->Write(pszPropName, &var));
//       }
//   }
//   return bSuccess;
extern "C" int MS_ABI impl__ExchangePersistentProp_CPropbagPropExchange__UEAAHPEB_WPEAPEAUIUnknown__AEBU_GUID__PEAU2__Z(
    void* pThis, const wchar_t* pszPropName, IUnknown** ppUnk, const GUID* piid, IUnknown* pUnkDefault) {
    PB_CPropbagPropExchange* self = pb(pThis);
    if (self->m_lpPropBag == nullptr) {
        return FALSE;
    }
    if (pszPropName == nullptr || ppUnk == nullptr) {
        impl__AfxThrowInvalidArgException__YAXXZ();
    }
    VARIANT var;
    std::memset(&var, 0, sizeof(var));
    V_VT(&var) = VT_UNKNOWN;

    int bSuccess;
    if (self->m_bLoading != 0) {
        AfxReleaseUnk(ppUnk);
        *ppUnk = nullptr;
        if (SUCCEEDED(self->m_lpPropBag->Read(pszPropName, &var, self->m_lpErrorLog)) &&
            SUCCEEDED(V_UNKNOWN(&var)->QueryInterface(*piid, reinterpret_cast<void**>(ppUnk)))) {
            bSuccess = TRUE;
        } else if (pUnkDefault != nullptr) {
            bSuccess = SUCCEEDED(pUnkDefault->QueryInterface(*piid, reinterpret_cast<void**>(ppUnk))) ? TRUE : FALSE;
        } else {
            bSuccess = TRUE;
        }
        ::VariantClear(&var);
        return bSuccess;
    }

    IUnknown* pUnk = *ppUnk;
    if (pUnk == nullptr) {
        return TRUE;
    }
    int bSame;
    if (pUnk == pUnkDefault) {
        bSame = TRUE;
    } else if (pUnkDefault == nullptr) {
        bSame = FALSE;
    } else {
        // Retail order: QI *ppUnk into a local (0x38(%rbp)); only on success QI
        // pUnkDefault (-0x30(%rbp)); on that success compare, then Release the
        // default's interface; then Release the first one.
        IUnknown* pUnk1 = nullptr;
        IUnknown* pUnk2 = nullptr;
        bSame = FALSE;
        if (SUCCEEDED(pUnk->QueryInterface(*piid, reinterpret_cast<void**>(&pUnk1)))) {
            if (SUCCEEDED(pUnkDefault->QueryInterface(*piid, reinterpret_cast<void**>(&pUnk2)))) {
                bSame = (pUnk1 == pUnk2) ? TRUE : FALSE;
                pUnk2->Release();
            }
            pUnk1->Release();
        }
    }
    if (bSame && self->m_bSaveAllProperties == 0) {
        bSuccess = TRUE;
    } else {
        V_UNKNOWN(&var) = pUnk;
        bSuccess = SUCCEEDED(self->m_lpPropBag->Write(pszPropName, &var)) ? TRUE : FALSE;
    }
    return bSuccess;
}

// Symbol: ?ExchangeProp@CPropbagPropExchange@@UEAAHPEB_WGPEAXPEBX@Z
// Transcribed from RVA 0x1e7c20 (mfc140u):
//   if (m_lpPropBag == NULL) return FALSE;
//   if (pszPropName == NULL || pvProp == NULL) AfxThrowInvalidArgException();
//   bSuccess = TRUE;
//   VARIANT var; memset(&var, 0, 0x18); V_VT(&var) = vtProp;
//   if (vtProp == VT_LPSTR) V_VT(&var) = VT_BSTR;
//   if (m_bLoading) {
//       if (FAILED(m_lpPropBag->Read(pszPropName, &var, m_lpErrorLog)))
//           return _AfxCopyPropValue(vtProp, pvProp, pvDefault); ; call 0x1ef7d8, no VariantClear
//       switch (vtProp) {                                     ; the union is read per vtProp,
//         VT_UI1: byte;  VT_I2: 16-bit;  VT_I4: 32-bit;       ; V_VT(&var) is not re-checked
//         VT_BOOL: *(BOOL*)pvProp = (BOOL)V_BOOL(&var);       ; movswl (sign-extends)
//         VT_R4: memcpy 4;  VT_R8: memcpy 8;  VT_CY: 64-bit;
//         VT_BSTR, VT_LPSTR: *(CString*)pvProp = V_BSTR(&var); ; wcslen (NULL -> 0) + SetString
//         default: bSuccess = FALSE;
//       }
//   } else if (m_bSaveAllProperties || !_AfxIsSamePropValue(vtProp, pvProp, pvDefault)) {
//       switch (vtProp) {                                     ; call 0x1f1c0c above
//         VT_UI1: byte;  VT_I2: 16-bit;  VT_I4: 32-bit;
//         VT_BOOL: V_BOOL(&var) = (VARIANT_BOOL)*(BOOL*)pvProp;   ; movzwl: low 16 bits
//         VT_R4: memcpy 4;  VT_R8: memcpy 8;  VT_CY: 64-bit;
//         VT_BSTR, VT_LPSTR: V_BSTR(&var) = SysAllocStringLen(str, str.GetLength());
//                            NULL -> AtlThrow(E_OUTOFMEMORY)  ; OLEAUT32 #4; 0x3160
//         default: return FALSE;                              ; no VariantClear
//       }
//       bSuccess = SUCCEEDED(m_lpPropBag->Write(pszPropName, &var));
//   }
//   VariantClear(&var);                                       ; OLEAUT32 #9
//   return bSuccess;
extern "C" int MS_ABI impl__ExchangeProp_CPropbagPropExchange__UEAAHPEB_WGPEAXPEBX_Z(
    void* pThis, const wchar_t* pszPropName, unsigned short vtProp, void* pvProp, const void* pvDefault) {
    PB_CPropbagPropExchange* self = pb(pThis);
    if (self->m_lpPropBag == nullptr) {
        return FALSE;
    }
    if (pszPropName == nullptr || pvProp == nullptr) {
        impl__AfxThrowInvalidArgException__YAXXZ();
    }
    int bSuccess = TRUE;
    VARIANT var;
    std::memset(&var, 0, sizeof(var));
    V_VT(&var) = vtProp;
    if (vtProp == VT_LPSTR) {
        V_VT(&var) = VT_BSTR;
    }

    if (self->m_bLoading != 0) {
        if (FAILED(self->m_lpPropBag->Read(pszPropName, &var, self->m_lpErrorLog))) {
            return AfxCopyPropValue(vtProp, pvProp, pvDefault);
        }
        switch (vtProp) {
        case VT_UI1:
            *static_cast<BYTE*>(pvProp) = V_UI1(&var);
            break;
        case VT_I2:
            *static_cast<short*>(pvProp) = V_I2(&var);
            break;
        case VT_I4:
            *static_cast<long*>(pvProp) = V_I4(&var);
            break;
        case VT_BOOL:
            *static_cast<BOOL*>(pvProp) = static_cast<BOOL>(V_BOOL(&var));
            break;
        case VT_LPSTR:
        case VT_BSTR:
            *static_cast<CString*>(pvProp) = static_cast<const wchar_t*>(V_BSTR(&var));
            break;
        case VT_CY:
            *static_cast<CY*>(pvProp) = V_CY(&var);
            break;
        case VT_R4:
            std::memcpy(pvProp, &V_R4(&var), sizeof(float));
            break;
        case VT_R8:
            std::memcpy(pvProp, &V_R8(&var), sizeof(double));
            break;
        default:
            bSuccess = FALSE;
            break;
        }
    } else if (self->m_bSaveAllProperties != 0 || !AfxIsSamePropValue(vtProp, pvProp, pvDefault)) {
        switch (vtProp) {
        case VT_UI1:
            V_UI1(&var) = *static_cast<const BYTE*>(pvProp);
            break;
        case VT_I2:
            V_I2(&var) = *static_cast<const short*>(pvProp);
            break;
        case VT_I4:
            V_I4(&var) = *static_cast<const long*>(pvProp);
            break;
        case VT_BOOL:
            V_BOOL(&var) = static_cast<VARIANT_BOOL>(*static_cast<const BOOL*>(pvProp));
            break;
        case VT_LPSTR:
        case VT_BSTR: {
            const CString* pstr = static_cast<const CString*>(pvProp);
            V_BSTR(&var) = ::SysAllocStringLen(pstr->GetString(), static_cast<UINT>(pstr->GetLength()));
            if (V_BSTR(&var) == nullptr) {
                impl__AfxThrowMemoryException__YAXXZ();
            }
            break;
        }
        case VT_CY:
            V_CY(&var) = *static_cast<const CY*>(pvProp);
            break;
        case VT_R4:
            std::memcpy(&V_R4(&var), pvProp, sizeof(float));
            break;
        case VT_R8:
            std::memcpy(&V_R8(&var), pvProp, sizeof(double));
            break;
        default:
            return FALSE;
        }
        bSuccess = SUCCEEDED(self->m_lpPropBag->Write(pszPropName, &var)) ? TRUE : FALSE;
    }

    ::VariantClear(&var);
    return bSuccess;
}
