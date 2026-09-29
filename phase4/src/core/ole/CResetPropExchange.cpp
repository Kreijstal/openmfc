// CResetPropExchange — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp
//
// MFC's private "reset to defaults" CPropExchange (COleControl::OnResetState): every
// Exchange* override just copies the caller's default into the property.  No header in
// include/openmfc declares the class (retail keeps it in the private ctlimpl.h), so the
// layout is pinned in-file below.  Every body in this file (the exports and the
// unexported helpers/deleting dtor alike) was transcribed from retail code
// disassembled from mfc140u.dll itself (disas.py --u); all RVAs below are mfc140u RVAs.
//
// ---------------------------------------------------------------------------
// Retail object layout, pinned by the retail ctor ??0CResetPropExchange@@QEAA@XZ
// (RVA 0x1f09d0, mfc140u):
//   lea 0x180324ad0,%rax ; mov %rax,(%rcx)   -> vptr
//   movq $0,0xc(%rcx)                        -> CPropExchange::m_bAsync (+0xc) and m_dwVersion (+0x10)
//   movl $1,0x8(%rcx)                        -> CPropExchange::m_bLoading = TRUE
// and by the scalar deleting destructor (vtable slot 5, RVA 0x1f09f0, unexported),
// which frees with `mov $0x18,%edx` -> sizeof == 0x18: the class adds no members to
// the CPropExchange base (the same +0x0..+0x17 layout core/ole/CPropExchange.cpp and
// core/ole/CPropbagPropExchange.cpp pin).
//
// Retail vftable (0x180324ad0 in mfc140u), read slot by slot with vtdump_u.py:
//   0 +0x00  ExchangeVersion         0x1efb30  (CPropExchange's, not overridden)
//   1 +0x08  ExchangeProp            0x1f0a20
//   2 +0x10  ExchangeBlobProp        0x1f0a40
//   3 +0x18  ExchangeFontProp        0x1f0ae0
//   4 +0x20  ExchangePersistentProp  0x1f0a90
//   5 +0x28  scalar deleting dtor    0x1f09f0  (unexported)
// The ctor below installs kResetPropExchangeVtbl, an in-file table of the same six
// entries (same approach as CPropbagPropExchange.cpp).
//
// No body here reads a member: none of the four Exchange* overrides touches `this`,
// and none of them validates pszPropName.  The COM calls retail makes
// (pUnkDefault->QueryInterface in ExchangePersistentProp, and the Release inside
// _AfxRelease at 0x26ccc4) go through
// __guard_dispatch_icall_fptr (`call *0x1802c7b30(%rip)` in mfc140u -- the CFG
// dispatch pointer, not an import; iatu.py reports it as "not an import").
// ---------------------------------------------------------------------------

#include "detail/ManualSmallStubImplementationsSupport.h"

#include <ocidl.h>
#include <olectl.h>
#include <cstring>
#include <wchar.h>

// Thunks defined elsewhere in the tree (each definition checked by grep).
extern "C" int MS_ABI impl__ExchangeVersion_CPropExchange__UEAAHAEAKKH_Z(          // core/ole/CPropExchange.cpp
    void* pThis, unsigned long* pdwVersionLoaded, unsigned long dwVersionDefault, int bConvert);
extern "C" void MS_ABI impl__InitializeFont_CFontHolder__QEAAXPEBUtagFONTDESC__PEAUIDispatch___Z(  // core/ole/CFontHolder.cpp
    void* self, const void* pFontDesc, void* pFontDispAmbient);
extern "C" void MS_ABI impl___3_YAXPEAX_Z(void* ptr);                           // detail/MemcoreSupport.cpp
extern "C" void MS_ABI impl__AfxThrowInvalidArgException__YAXXZ();              // detail/MfcExceptionsSupport.cpp

// This file's own exports, forward-declared for the in-file vtable.
extern "C" int MS_ABI impl__ExchangeProp_CResetPropExchange__UEAAHPEB_WGPEAXPEBX_Z(
    void* pThis, const wchar_t* pszPropName, unsigned short vtProp, void* pvProp, const void* pvDefault);
extern "C" int MS_ABI impl__ExchangeBlobProp_CResetPropExchange__UEAAHPEB_WPEAPEAXPEAX_Z(
    void* pThis, const wchar_t* pszPropName, HGLOBAL* phBlob, HGLOBAL hBlobDefault);
extern "C" int MS_ABI impl__ExchangeFontProp_CResetPropExchange__UEAAHPEB_WAEAVCFontHolder__PEBUtagFONTDESC__PEAUIFontDisp___Z(
    void* pThis, const wchar_t* pszPropName, void* pFontHolder, const FONTDESC* pFontDesc, IFontDisp* pFontDispAmbient);
extern "C" int MS_ABI impl__ExchangePersistentProp_CResetPropExchange__UEAAHPEB_WPEAPEAUIUnknown__AEBU_GUID__PEAU2__Z(
    void* pThis, const wchar_t* pszPropName, IUnknown** ppUnk, const GUID* piid, IUnknown* pUnkDefault);

namespace {

// Retail layout (see the header comment for the evidence).
struct RS_CResetPropExchange {
    void* const*  vptr;                 // 0x00
    int           m_bLoading;           // 0x08  (CPropExchange)
    int           m_bAsync;             // 0x0c  (CPropExchange)
    unsigned long m_dwVersion;          // 0x10  (CPropExchange)
};
static_assert(offsetof(RS_CResetPropExchange, m_bLoading) == 0x08, "CPropExchange::m_bLoading");
static_assert(offsetof(RS_CResetPropExchange, m_bAsync) == 0x0c, "CPropExchange::m_bAsync");
static_assert(offsetof(RS_CResetPropExchange, m_dwVersion) == 0x10, "CPropExchange::m_dwVersion");
static_assert(sizeof(RS_CResetPropExchange) == 0x18, "CResetPropExchange is 0x18 bytes (sized delete in the deleting dtor)");

// _AfxRelease, RVA 0x26ccc4 (mfc140u, unexported):
//   if (*pp != NULL) { (*pp)->Release(); *pp = NULL; }     ; vtable +0x10
// (Duplicated from CPropbagPropExchange.cpp, where it is file-local.)
void AfxReleaseUnk(IUnknown** pp) {
    if (*pp != nullptr) {
        (*pp)->Release();
        *pp = nullptr;
    }
}

// _AfxCopyPropValue, RVA 0x1ef7d8 (mfc140u, unexported):
//   if (pvSrc != NULL) switch (vt) {
//     VT_I2: 16-bit copy;  VT_I4/VT_R4/VT_BOOL: 32-bit;  VT_R8/VT_CY: 64-bit;
//     VT_UI1: byte;
//     VT_BSTR:  *(CString*)pvDest = *(const CString*)pvSrc;   ; call 0xde30
//     VT_LPSTR: *(CString*)pvDest = (LPCWSTR)pvSrc;           ; wcslen (IAT 0x1802c7748) + 0x2e30
//     default:  return FALSE;
//   }
//   return pvSrc != NULL;
// (Duplicated from CPropbagPropExchange.cpp, where it is file-local; the switch
// was re-read here from the retail body: `sub $2/$1/$1/$1/$1/$2/$3/$6; cmp $0xd`.)
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

// _AfxInitBlob, RVA 0x1ef6e8 (mfc140u, unexported):
//   bResult = FALSE;
//   if (*(DWORD*)pvSrc > 0) {
//       cb = *(DWORD*)pvSrc + 4;                               ; 64-bit add
//       *phDst = GlobalAlloc(GMEM_MOVEABLE, cb);               ; IAT 0x1802c6620
//       if (*phDst != NULL) {
//           p = GlobalLock(*phDst);                            ; IAT 0x1802c6628
//           if (p != NULL) {
//               Checked::memcpy_s(p, GlobalSize(*phDst), pvSrc, cb);   ; GlobalSize 0x1802c6750
//               bResult = TRUE;
//           }
//           GlobalUnlock(*phDst);                              ; IAT 0x1802c6630, also when the lock failed
//       }
//   }
//   return bResult;
// The memcpy_s failure path is inlined in retail: memset(p, 0, size); errno = ERANGE;
// _invalid_parameter_noinfo(); AfxThrowInvalidArgException() (call 0x227720).  It is
// written here as memcpy_s (which does the zeroing, errno and invalid-parameter
// report itself) followed by the throw on a non-zero return.
// (Duplicated from CPropbagPropExchange.cpp, where it is file-local.)
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
// (Duplicated from CPropbagPropExchange.cpp, where it is file-local.)
int AfxCopyBlob(HGLOBAL* phDst, HGLOBAL hSrc) {
    int bResult = FALSE;
    const void* p = ::GlobalLock(hSrc);
    if (p != nullptr) {
        bResult = AfxInitBlob(phDst, p);
        ::GlobalUnlock(hSrc);
    }
    return bResult;
}

// Scalar deleting destructor, vtable slot 5 -- RVA 0x1f09f0 (mfc140u, unexported):
//   if (flags & 1) sized-delete(this, 0x18);  return this;
// There is no destructor body at all (no vptr re-store, nothing released).  The
// sized delete (call 0x2b77b0, unexported) is a bare `jmp 0x27c0`; 0x27c0 is a
// folded stub that the export map names ??_V@YAXPEAX@Z, and it is itself only
// `jmp *0x1802c74e8(%rip)` -- the CRT `free` import (iatu.py).  So retail's delete
// here is plain free().  OpenMFC's ??3@YAXPEAX@Z thunk is also free(), and it pairs
// with its malloc-based ??2@YAPEAX_K@Z.
void* MS_ABI ResetScalarDeletingDtor(void* pThis, unsigned int flags) {
    if (flags & 1) {
        impl___3_YAXPEAX_Z(pThis);
    }
    return pThis;
}

// Vtable in retail slot order (see the header comment).
void* const kResetPropExchangeVtbl[6] = {
    reinterpret_cast<void*>(&impl__ExchangeVersion_CPropExchange__UEAAHAEAKKH_Z),
    reinterpret_cast<void*>(&impl__ExchangeProp_CResetPropExchange__UEAAHPEB_WGPEAXPEBX_Z),
    reinterpret_cast<void*>(&impl__ExchangeBlobProp_CResetPropExchange__UEAAHPEB_WPEAPEAXPEAX_Z),
    reinterpret_cast<void*>(&impl__ExchangeFontProp_CResetPropExchange__UEAAHPEB_WAEAVCFontHolder__PEBUtagFONTDESC__PEAUIFontDisp___Z),
    reinterpret_cast<void*>(&impl__ExchangePersistentProp_CResetPropExchange__UEAAHPEB_WPEAPEAUIUnknown__AEBU_GUID__PEAU2__Z),
    reinterpret_cast<void*>(&ResetScalarDeletingDtor),
};

}  // namespace

// Symbol: ??0CResetPropExchange@@QEAA@XZ
// Transcribed from RVA 0x1f09d0 (mfc140u):
//   m_bAsync = 0; m_dwVersion = 0;                ; movq $0,0xc(%rcx) (inlined base ctor)
//   vptr = CResetPropExchange vftable;
//   m_bLoading = TRUE;                            ; movl $1,0x8(%rcx)
//   return this;
// (Previously this body only returned pThis and stored no vptr.)
extern "C" void* MS_ABI impl___0CResetPropExchange__QEAA_XZ(void* pThis) {
    RS_CResetPropExchange* self = static_cast<RS_CResetPropExchange*>(pThis);
    self->m_bAsync = 0;
    self->m_dwVersion = 0;
    self->vptr = kResetPropExchangeVtbl;
    self->m_bLoading = TRUE;
    return pThis;
}

// Symbol: ?ExchangeBlobProp@CResetPropExchange@@UEAAHPEB_WPEAPEAXPEAX@Z
// Transcribed from RVA 0x1f0a40 (mfc140u):
//   if (*phBlob != NULL) { GlobalFree(*phBlob); *phBlob = NULL; }   ; IAT 0x1802c66d0
//   bResult = TRUE;
//   if (hBlobDefault != NULL) bResult = _AfxCopyBlob(phBlob, hBlobDefault);  ; call 0x1ef788
//   return bResult;
// Neither pszPropName nor phBlob is checked; `this` is not read.
// (The previous parameter list omitted `this`; the definition is an instance method.)
extern "C" int MS_ABI impl__ExchangeBlobProp_CResetPropExchange__UEAAHPEB_WPEAPEAXPEAX_Z(
    void* pThis, const wchar_t* pszPropName, HGLOBAL* phBlob, HGLOBAL hBlobDefault) {
    (void)pThis;
    (void)pszPropName;
    if (*phBlob != nullptr) {
        ::GlobalFree(*phBlob);
        *phBlob = nullptr;
    }
    int bResult = TRUE;
    if (hBlobDefault != nullptr) {
        bResult = AfxCopyBlob(phBlob, hBlobDefault);
    }
    return bResult;
}

// Symbol: ?ExchangeFontProp@CResetPropExchange@@UEAAHPEB_WAEAVCFontHolder@@PEBUtagFONTDESC@@PEAUIFontDisp@@@Z
// Transcribed from RVA 0x1f0ae0 (mfc140u):
//   font.InitializeFont(pFontDesc, pFontDispAmbient);   ; call 0x1e4680 = ?InitializeFont@CFontHolder@@...
//   return TRUE;
// (rcx <- r8 = &font, rdx <- r9 = pFontDesc, r8 <- 0x50(%rsp) = 5th arg pFontDispAmbient.)
// Note (not a deviation of this body): core/ole/CFontHolder.cpp's InitializeFont
// thunk currently keeps the font in a side table rather than at CFontHolder +0x0;
// this body simply forwards to that thunk exactly as retail forwards to its own.
extern "C" int MS_ABI impl__ExchangeFontProp_CResetPropExchange__UEAAHPEB_WAEAVCFontHolder__PEBUtagFONTDESC__PEAUIFontDisp___Z(
    void* pThis, const wchar_t* pszPropName, void* pFontHolder, const FONTDESC* pFontDesc, IFontDisp* pFontDispAmbient) {
    (void)pThis;
    (void)pszPropName;
    impl__InitializeFont_CFontHolder__QEAAXPEBUtagFONTDESC__PEAUIDispatch___Z(
        pFontHolder, pFontDesc, pFontDispAmbient);
    return TRUE;
}

// Symbol: ?ExchangePersistentProp@CResetPropExchange@@UEAAHPEB_WPEAPEAUIUnknown@@AEBU_GUID@@PEAU2@@Z
// Transcribed from RVA 0x1f0a90 (mfc140u):
//   _AfxRelease(ppUnk);                                  ; call 0x26ccc4
//   if (pUnkDefault == NULL) return TRUE;
//   return SUCCEEDED(pUnkDefault->QueryInterface(iid, (void**)ppUnk));  ; vtable +0x0, `not; shr $0x1f`
// ppUnk is not NULL-checked; `this` is not read.
extern "C" int MS_ABI impl__ExchangePersistentProp_CResetPropExchange__UEAAHPEB_WPEAPEAUIUnknown__AEBU_GUID__PEAU2__Z(
    void* pThis, const wchar_t* pszPropName, IUnknown** ppUnk, const GUID* piid, IUnknown* pUnkDefault) {
    (void)pThis;
    (void)pszPropName;
    AfxReleaseUnk(ppUnk);
    if (pUnkDefault == nullptr) {
        return TRUE;
    }
    return SUCCEEDED(pUnkDefault->QueryInterface(*piid, reinterpret_cast<void**>(ppUnk))) ? TRUE : FALSE;
}

// Symbol: ?ExchangeProp@CResetPropExchange@@UEAAHPEB_WGPEAXPEBX@Z
// Transcribed from RVA 0x1f0a20 (mfc140u):
//   return _AfxCopyPropValue(vtProp, pvProp, pvDefault);   ; tail `jmp 0x1ef7d8`
// (cx <- r8w = vtProp, rdx <- r9 = pvProp, r8 <- 0x28(%rsp) = 5th arg pvDefault.)
extern "C" int MS_ABI impl__ExchangeProp_CResetPropExchange__UEAAHPEB_WGPEAXPEBX_Z(
    void* pThis, const wchar_t* pszPropName, unsigned short vtProp, void* pvProp, const void* pvDefault) {
    (void)pThis;
    (void)pszPropName;
    return AfxCopyPropValue(vtProp, pvProp, pvDefault);
}
