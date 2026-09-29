// CArchivePropExchange — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp
//
// MFC's private CArchive-backed CPropExchange (COleControl::Serialize builds one
// over the document archive and runs DoPropExchange through it).  No header in
// include/openmfc declares the class (retail keeps it in the private ctlimpl.h),
// so the layout is pinned in-file below.  Every body in this file was
// transcribed from the retail export, disassembled from mfc140u.dll itself
// (disas.py --u); all RVAs below are mfc140u RVAs.
//
// ---------------------------------------------------------------------------
// Retail object layout, pinned by the retail ctor ??0CArchivePropExchange@@...
// (RVA 0x1efb90, mfc140u), which is the whole of:
//   lea 0x180324b08,%rax ; movq $0,0xc(%rcx) ; mov %rax,(%rcx)
//   mov %rdx,0x18(%rcx)                      -> m_ar (a CArchive&, stored as a pointer)
//   mov 0x20(%rdx),%eax ; and $1,%eax ; mov %eax,0x8(%rcx)
//                                            -> m_bLoading = ar.m_nMode & CArchive::load
// and by the scalar deleting destructor (vtable slot 5, RVA 0x1efbc0, unexported),
// which frees with `mov $0x20,%edx` -> sizeof == 0x20.  The base part
// (+0x0..+0x17) is the CPropExchange layout core/ole/CPropExchange.cpp pins.
//
// Retail vftable (0x180324b08 in mfc140u), read slot by slot with vtdump_u.py:
//   0 +0x00  ExchangeVersion         0x1efb30  (CPropExchange's, not overridden)
//   1 +0x08  ExchangeProp            0x1efbf0
//   2 +0x10  ExchangeBlobProp        0x1f0120
//   3 +0x18  ExchangeFontProp        0x1f07d0
//   4 +0x20  ExchangePersistentProp  0x1f0270
//   5 +0x28  scalar deleting dtor    0x1efbc0  (unexported)
// The ctor below installs kArchivePropExchangeVtbl, an in-file table of the same
// six entries (the technique core/ole/CPropbagPropExchange.cpp uses).
//
// Every virtual call retail makes here goes through __guard_dispatch_icall_fptr
// (`call *0x1802c7b30(%rip)` in mfc140u -- the CFG dispatch pointer, not an
// import); the real target is the slot loaded into %rax just before.
//
// OpenMFC deviations, applied throughout (all deliberate):
//   * CArchive layout.  Retail inlines CArchive's primitive operator<< / >>
//     straight onto retail's CArchive layout (m_nMode +0x20, m_strFileName +0x18,
//     m_lpBufCur +0x38, m_lpBufMax +0x40, FillBuffer 0x1d1cc0 / Flush 0x1d1be0
//     when the buffer runs short).  OpenMFC's CArchive (include/openmfc/afx.h) is
//     a different layout, and OpenMFC's exported CArchive::Read / Write / Flush
//     thunks (core/runtime/Thunks.cpp) operate on that layout, so every primitive
//     is reproduced by ArLoad / ArStore below on top of those thunks.  They keep
//     retail's observable semantics: the mode test and its cause (writeOnly on a
//     load from a storing archive, readOnly on a store into a loading one), and
//     FillBuffer's endOfFile throw when fewer bytes remain than requested
//     (`lea 0x3(%rdx),%ecx` at 0x1d1e57 inside FillBuffer 0x1d1cc0; the archive
//     name argument there is NULL, `xor %edx,%edx`).  Retail's mode-test throw
//     passes m_strFileName; OpenMFC's CArchive has no such member, so NULL is
//     passed -- the same choice core/runtime/CArchive.cpp makes.
//   * pszPropName is unused by every body, exactly as in retail (the archive
//     format is positional).
// ---------------------------------------------------------------------------

#include "detail/ManualSmallStubImplementationsSupport.h"

#include <ocidl.h>
#include <olectl.h>
#include <oleauto.h>
#include <cstdlib>
#include <cstring>
#include <wchar.h>

// Thunks defined elsewhere in the tree (each definition checked by grep).
extern "C" int MS_ABI impl__ExchangeVersion_CPropExchange__UEAAHAEAKKH_Z(          // core/ole/CPropExchange.cpp
    void* pThis, unsigned long* pdwVersionLoaded, unsigned long dwVersionDefault, int bConvert);
extern "C" unsigned int MS_ABI impl__Read_CArchive__QEAAIPEAXI_Z(CArchive* pThis, void* p0, unsigned int p1);  // core/runtime/Thunks.cpp
extern "C" void MS_ABI impl__Write_CArchive__QEAAXPEBXI_Z(CArchive* pThis, const void* p0, unsigned int p1);   // core/runtime/Thunks.cpp
extern "C" void MS_ABI impl__Flush_CArchive__QEAAXXZ(CArchive* pThis);                                        // core/runtime/Thunks.cpp
extern "C" unsigned long long MS_ABI impl__AfxReadStringLength__YA_KAEAVCArchive__AEAH_Z(                      // core/runtime/CArchive.cpp
    CArchive* ar, int* pnCharSize);
extern "C" void MS_ABI impl__AfxWriteStringLength__YAXAEAVCArchive___KH_Z(                                    // core/runtime/CArchive.cpp
    CArchive* ar, unsigned long long nLength, int bUnicode);
extern "C" void* MS_ABI impl___0CArchiveStream__QEAA_PEAVCArchive___Z(void* pThis, CArchive* pArchive);        // core/ole/CArchiveStream.cpp
extern "C" void MS_ABI impl__SetFont_CFontHolder__QEAAXPEAUIFont___Z(void* self, void* font);                 // core/ole/CFontHolder.cpp
extern "C" void MS_ABI impl__InitializeFont_CFontHolder__QEAAXPEBUtagFONTDESC__PEAUIDispatch___Z(              // core/ole/CFontHolder.cpp
    void* self, const void* pFontDesc, void* pFontDispAmbient);
extern "C" void MS_ABI impl___3_YAXPEAX_Z(void* ptr);                                                         // detail/MemcoreSupport.cpp
extern "C" void MS_ABI impl__AfxThrowArchiveException__YAXHPEB_W_Z(int cause, const wchar_t* lpszArchiveName); // detail/MfcExceptionsSupport.cpp
extern "C" void MS_ABI impl__AfxThrowInvalidArgException__YAXXZ();                                            // detail/MfcExceptionsSupport.cpp
extern "C" void MS_ABI impl__AfxThrowMemoryException__YAXXZ();                                                // detail/MfcExceptionsSupport.cpp
extern "C" void MS_ABI impl__AfxThrowOleException__YAXJ_Z(LONG sc);                                           // detail/MfcExceptionsSupport.cpp

// This file's own exports, forward-declared for the in-file vtable.
extern "C" int MS_ABI impl__ExchangeProp_CArchivePropExchange__UEAAHPEB_WGPEAXPEBX_Z(
    void* pThis, const wchar_t* pszPropName, unsigned short vtProp, void* pvProp, const void* pvDefault);
extern "C" int MS_ABI impl__ExchangeBlobProp_CArchivePropExchange__UEAAHPEB_WPEAPEAXPEAX_Z(
    void* pThis, const wchar_t* pszPropName, HGLOBAL* phBlob, HGLOBAL hBlobDefault);
extern "C" int MS_ABI impl__ExchangeFontProp_CArchivePropExchange__UEAAHPEB_WAEAVCFontHolder__PEBUtagFONTDESC__PEAUIFontDisp___Z(
    void* pThis, const wchar_t* pszPropName, void* pFontHolder, const FONTDESC* pFontDesc, IFontDisp* pFontDispAmbient);
extern "C" int MS_ABI impl__ExchangePersistentProp_CArchivePropExchange__UEAAHPEB_WPEAPEAUIUnknown__AEBU_GUID__PEAU2__Z(
    void* pThis, const wchar_t* pszPropName, IUnknown** ppUnk, const GUID* piid, IUnknown* pUnkDefault);

namespace {

// Retail layout (see the header comment for the evidence).
struct AR_CArchivePropExchange {
    void* const*  vptr;                 // 0x00
    int           m_bLoading;           // 0x08  (CPropExchange)
    int           m_bAsync;             // 0x0c  (CPropExchange)
    unsigned long m_dwVersion;          // 0x10  (CPropExchange)
    CArchive*     m_pArchive;           // 0x18  (CArchive& m_ar)
};
static_assert(offsetof(AR_CArchivePropExchange, m_bLoading) == 0x08, "CPropExchange::m_bLoading");
static_assert(offsetof(AR_CArchivePropExchange, m_bAsync) == 0x0c, "CPropExchange::m_bAsync");
static_assert(offsetof(AR_CArchivePropExchange, m_dwVersion) == 0x10, "CPropExchange::m_dwVersion");
static_assert(offsetof(AR_CArchivePropExchange, m_pArchive) == 0x18, "CArchivePropExchange::m_ar (ctor 0x1efb90: mov %rdx,0x18(%rcx))");
static_assert(sizeof(AR_CArchivePropExchange) == 0x20, "CArchivePropExchange is 0x20 bytes (sized delete in the deleting dtor)");

inline AR_CArchivePropExchange* ape(void* pThis) { return static_cast<AR_CArchivePropExchange*>(pThis); }

// CArchiveException causes retail passes to AfxThrowArchiveException (0x1d3610).
constexpr int kArchiveGenericException = 1;
constexpr int kArchiveReadOnly         = 2;
constexpr int kArchiveEndOfFile        = 3;
constexpr int kArchiveWriteOnly        = 4;

// Retail's inline `ar >> x` primitive (see the header comment):
//   if (!(m_nMode & load)) AfxThrowArchiveException(writeOnly, m_strFileName);
//   if (m_lpBufCur + n > m_lpBufMax) FillBuffer(n - (m_lpBufMax - m_lpBufCur));   ; throws endOfFile
//   x = *(T*)m_lpBufCur; m_lpBufCur += n;
template <class T> T ArLoad(CArchive* ar) {
    if (!ar->IsLoading()) {
        impl__AfxThrowArchiveException__YAXHPEB_W_Z(kArchiveWriteOnly, nullptr);
    }
    T value{};
    if (impl__Read_CArchive__QEAAIPEAXI_Z(ar, &value, sizeof(value)) != sizeof(value)) {
        impl__AfxThrowArchiveException__YAXHPEB_W_Z(kArchiveEndOfFile, nullptr);
    }
    return value;
}

// Retail's inline `ar << x` primitive:
//   if (m_nMode & load) AfxThrowArchiveException(readOnly, m_strFileName);
//   if (m_lpBufCur + n > m_lpBufMax) Flush();
//   *(T*)m_lpBufCur = x; m_lpBufCur += n;
template <class T> void ArStore(CArchive* ar, T value) {
    if (ar->IsLoading()) {
        impl__AfxThrowArchiveException__YAXHPEB_W_Z(kArchiveReadOnly, nullptr);
    }
    impl__Write_CArchive__QEAAXPEBXI_Z(ar, &value, sizeof(value));
}

// CTempBuffer<.., 128> as retail's CString operator>> uses it: a 128-byte stack
// buffer, else malloc (0x1c090: `call *0x1802c74c8` = api-ms-win-crt-heap malloc;
// NULL -> AtlThrow(E_OUTOFMEMORY), which the AtlThrow hook at 0x333c routes to
// AfxThrowMemoryException), freed on exit when heap-allocated.
struct TempBuffer {
    unsigned char  stack[128];
    unsigned char* p;
    explicit TempBuffer(unsigned long long cb) : p(stack) {
        if (cb > sizeof(stack)) {
            p = static_cast<unsigned char*>(std::malloc(static_cast<std::size_t>(cb)));
            if (p == nullptr) {
                impl__AfxThrowMemoryException__YAXXZ();
            }
        }
    }
    void Free() {
        if (p != stack) {
            std::free(p);
        }
        p = stack;
    }
    ~TempBuffer() { Free(); }
};

// The wide-string construction retail uses here is the CSimpleStringT ctor
// (??0?$CSimpleStringT@_W$00@ATL@@QEAA@PEB_WHPEAUIAtlStringMgr@1@@Z, RVA 0x12980
// (mfc140u), called with the module string manager at 0x1803b25e8): it copies
// exactly nLength characters (embedded NULs kept) and terminates.  A negative
// length is AtlThrow(E_INVALIDARG) there (`test %ebx,%ebx; js`); it cannot arise
// from the one caller below (see ArReadCString), so it is not modelled.
void AssignWide(CString& str, const wchar_t* pch, int nLength) {
    if (nLength > 0) {
        wchar_t* pBuf = str.GetBuffer(nLength);
        std::memcpy(pBuf, pch, static_cast<std::size_t>(nLength) * sizeof(wchar_t));
        str.ReleaseBuffer(nLength);
    }
}

// CStringT(const char* pch, int nLength) (RVA 0x1c0e0, mfc140u):
//   if (nLength > 0) {
//       n = MultiByteToWideChar(CP_THREAD_ACP, 0, pch, nLength, NULL, 0);   ; ecx = 3; slot
//       GetBuffer(n); MultiByteToWideChar(CP_THREAD_ACP, 0, pch, nLength, buf, n);  ; 0x1802c6610
//       length = n;                                                          ; KERNEL32 import
//   }
// (nLength <= 0 leaves the string empty: `test %ebp,%ebp; jle`.)
void AssignAnsi(CString& str, const char* pch, int nLength) {
    if (nLength > 0) {
        const int n = ::MultiByteToWideChar(CP_THREAD_ACP, 0, pch, nLength, nullptr, 0);
        if (n > 0) {
            wchar_t* pBuf = str.GetBuffer(n);
            ::MultiByteToWideChar(CP_THREAD_ACP, 0, pch, nLength, pBuf, n);
            str.ReleaseBuffer(n);
        }
    }
}

// operator>>(CArchive&, CStringW&) -- RVA 0x1b5e4 (mfc140u, unexported):
//   nLength = AfxReadStringLength(ar, nCharSize);       ; call 0x1d0c10
//   nLen = (UINT)nLength;                               ; `mov %eax,%ebx` -- only the
//                                                       ; low 32 bits are used below
//   if (nCharSize == 1) {
//       CTempBuffer<char> buf(nLen);
//       if (ar.Read(buf, nLen) != nLen) AfxThrowArchiveException(endOfFile, NULL);   ; 0x1d1840
//       str = CStringW((const char*)buf, (int)nLen);    ; 0x1c0e0, then operator= (0xde30)
//   } else {
//       CTempBuffer<wchar_t> buf(2 * nLen bytes);
//       if ((ULONGLONG)ar.Read(buf, (UINT)(2 * nLen)) != 2 * nLen)                ; 64-bit compare
//           AfxThrowArchiveException(endOfFile, NULL);
//       str = CStringW((const wchar_t*)buf, (int)nLen); ; 0x12980
//   }
// Retail also carries size_t-overflow checks on the buffer size (`div` by nLen,
// AtlThrow(0x80070216)); with a 32-bit nLen they can never fire, so they are
// omitted.  In the wide branch a read can only match when 2*nLen < 2^32, so the
// (int)nLen handed to the ctor is never negative.
// The temporary buffer is released before the endOfFile throw here: the MSVC
// exception this DLL raises does not run gcc frame cleanups (see
// core/ole/CArchiveStream.cpp, "Exceptions"), where retail's unwinder frees it.
void ArReadCString(CArchive* ar, CString& str) {
    int nCharSize = 0;
    const unsigned long long nLength = impl__AfxReadStringLength__YA_KAEAVCArchive__AEAH_Z(ar, &nCharSize);
    const unsigned int nLen = static_cast<unsigned int>(nLength);
    CString strTemp;
    if (nCharSize == 1) {
        TempBuffer buf(nLen);
        if (impl__Read_CArchive__QEAAIPEAXI_Z(ar, buf.p, nLen) != nLen) {
            buf.Free();
            impl__AfxThrowArchiveException__YAXHPEB_W_Z(kArchiveEndOfFile, nullptr);
        }
        AssignAnsi(strTemp, reinterpret_cast<const char*>(buf.p), static_cast<int>(nLen));
    } else {
        const unsigned long long cb = 2ull * nLen;
        TempBuffer buf(cb);
        if (static_cast<unsigned long long>(
                impl__Read_CArchive__QEAAIPEAXI_Z(ar, buf.p, static_cast<unsigned int>(cb))) != cb) {
            buf.Free();
            impl__AfxThrowArchiveException__YAXHPEB_W_Z(kArchiveEndOfFile, nullptr);
        }
        AssignWide(strTemp, reinterpret_cast<const wchar_t*>(buf.p), static_cast<int>(nLen));
    }
    str = strTemp;
}

// operator<<(CArchive&, const CStringW&) -- RVA 0x1b818 (mfc140u, unexported), whole body:
//   AfxWriteStringLength(ar, (LONGLONG)str.GetLength(), TRUE);   ; movslq -0x10(%rax); r8d = 1; 0x1d0e40
//   ar.Write(str.m_pszData, str.GetLength() * 2);                ; 32-bit `add %r8d,%r8d`; 0x1d1a70
void ArWriteCString(CArchive* ar, const CString& str) {
    impl__AfxWriteStringLength__YAXAEAVCArchive___KH_Z(
        ar, static_cast<unsigned long long>(static_cast<long long>(str.GetLength())), TRUE);
    impl__Write_CArchive__QEAAXPEBXI_Z(
        ar, str.GetString(), static_cast<unsigned int>(str.GetLength()) * 2u);
}

// A CArchiveStream on the stack, as retail builds it inline (vftable 0x180321088
// + m_pArchive, sizeof 0x10 -- core/ole/CArchiveStream.cpp pins that layout).
// Here it is built through the exported constructor, which installs OpenMFC's
// hand-authored IStream vtable.
struct ArchiveStreamStorage {
    void*     vptr;          // +0x00
    CArchive* m_pArchive;    // +0x08
};
static_assert(sizeof(ArchiveStreamStorage) == 0x10, "retail CArchiveStream is 0x10 bytes");

// _AfxGetArchiveStream -- RVA 0x1ef698 (mfc140u, unexported):
//   ar.Flush();                                                   ; 0x1d1be0
//   CFile* pFile = ar.m_pFile;                                    ; +0x30
//   if (pFile->IsKindOf(RUNTIME_CLASS(COleStreamFile)))           ; 0x234cf0, class at 0x180330ae8
//       return ((COleStreamFile*)pFile)->m_lpStream;              ; +0x28
//   stm.m_pArchive = &ar;
//   return &stm;
// Deviation: the COleStreamFile shortcut is not taken; the CArchiveStream is
// always returned.  The shortcut is only sound because retail's Flush of a
// loading archive seeks the file back over the buffered-but-unconsumed bytes
// (Flush 0x1d1be0: `m_pFile->Seek(m_lpBufCur - m_lpBufMax, current)`, vtable
// +0x68) so the raw IStream is positioned at the archive's logical offset.
// OpenMFC's CArchive::Flush does no such rewind (core/runtime/CArchive.cpp), so
// handing out the raw stream would skip those bytes.  IStream::Read and ::Write
// on the CArchiveStream go through CArchive::Read/Write and are position-correct;
// IStream::Seek on it is NOT (it flushes and seeks the file underneath the still
// buffered bytes), which is why ExchangePersistentProp below does not issue
// retail's Seek(-16).  A persistent object whose IPersistStream::Load seeks the
// stream itself will still see a mispositioned stream -- a limitation of
// OpenMFC's CArchive, not fixable in this file.  Flush is still called, as in
// retail.
IStream* AfxGetArchiveStream(CArchive* ar, ArchiveStreamStorage* pStm) {
    impl__Flush_CArchive__QEAAXXZ(ar);
    pStm->m_pArchive = ar;
    return reinterpret_cast<IStream*>(pStm);
}

// GUIDs, each the 16 bytes read from mfc140u .rdata at the address retail loads.
const GUID kGUID_NULL =                 // 0x1802d98d8
    { 0x00000000, 0x0000, 0x0000, { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } };
const GUID kCLSID_StdPicture =          // 0x1802d9938  {0BE35204-8F91-11CE-9DE3-00AA004BB851}
    { 0x0BE35204, 0x8F91, 0x11CE, { 0x9D, 0xE3, 0x00, 0xAA, 0x00, 0x4B, 0xB8, 0x51 } };
const GUID kCLSID_StdPicture_V1 =       // 0x18034bce8  {FB8F0824-0164-101B-84ED-08002B2EC713}
    { 0xFB8F0824, 0x0164, 0x101B, { 0x84, 0xED, 0x08, 0x00, 0x2B, 0x2E, 0xC7, 0x13 } };
const GUID kCLSID_StdFont =             // 0x1802d9928  {0BE35203-8F91-11CE-9DE3-00AA004BB851}
    { 0x0BE35203, 0x8F91, 0x11CE, { 0x9D, 0xE3, 0x00, 0xAA, 0x00, 0x4B, 0xB8, 0x51 } };
const GUID kCLSID_StdFont_V1 =          // 0x18034bd48  {FB8F0823-0164-101B-84ED-08002B2EC713}
    { 0xFB8F0823, 0x0164, 0x101B, { 0x84, 0xED, 0x08, 0x00, 0x2B, 0x2E, 0xC7, 0x13 } };
const GUID kIID_IPersistStream =        // 0x1802d9b18  {00000109-0000-0000-C000-000000000046}
    { 0x00000109, 0x0000, 0x0000, { 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 } };
const GUID kIID_IPersistStreamInit =    // 0x1802d9d58  {7FD52380-4E07-101B-AE2D-08002B2EC713}
    { 0x7FD52380, 0x4E07, 0x101B, { 0xAE, 0x2D, 0x08, 0x00, 0x2B, 0x2E, 0xC7, 0x13 } };
const GUID kIID_IFont =                 // 0x1802d9d88  {BEF6E002-A874-101A-8BBA-00AA00300CAB}
    { 0xBEF6E002, 0xA874, 0x101A, { 0x8B, 0xBA, 0x00, 0xAA, 0x00, 0x30, 0x0C, 0xAB } };

inline bool SameGuid(const GUID& a, const GUID& b) {
    return std::memcmp(&a, &b, sizeof(GUID)) == 0;      // retail: memcmp(.., 0x10)
}

// CFontHolder as retail lays it out: afxctl.h's `LPFONT m_pFont` at +0x0.  Retail
// ExchangeFontProp reads +0x0 directly (`mov (%r8),%rbx`) and dispatches IFont
// methods through it.  include/openmfc/afxole.h types the member IFontDisp*, so
// this view is used instead (as core/ole/CPropbagPropExchange.cpp does).
struct AR_CFontHolder {
    IFont* m_pFont;                     // 0x00
};
static_assert(offsetof(CFontHolder, m_pFont) == 0x00, "CFontHolder::m_pFont");

// _afxFontDescHelv, the 0x28 bytes at 0x18034bd10 (mfc140u .rdata) that
// _AfxCreateFontFromStream passes to OleCreateFontIndirect:
//   cbSizeofstruct 0x28, lpstrName -> 0x18034b8d8 = L"Helv",
//   cySize 120000 (12.0 in CY units), sWeight 0x190 (FW_NORMAL),
//   sCharset 1 (DEFAULT_CHARSET), fItalic/fUnderline/fStrikethrough 0.
wchar_t kFontNameHelv[] = L"Helv";
const FONTDESC kFontDescHelv = {
    sizeof(FONTDESC), kFontNameHelv, { { 120000, 0 } }, FW_NORMAL, DEFAULT_CHARSET, FALSE, FALSE, FALSE
};

// The default FONTDESC _AfxIsSameFont substitutes for a NULL pFontDesc: the 0x28
// bytes at 0x18034b878 (mfc140u .rdata) -- L"MS Shell Dlg" (0x18034b858), 12pt,
// FW_NORMAL, DEFAULT_CHARSET.  Same data core/ole/CPropbagPropExchange.cpp uses.
wchar_t kFontNameShellDlg[] = L"MS Shell Dlg";
const FONTDESC kFontDescDefault = {
    sizeof(FONTDESC), kFontNameShellDlg, { { 120000, 0 } }, FW_NORMAL, DEFAULT_CHARSET, FALSE, FALSE, FALSE
};
static_assert(sizeof(FONTDESC) == 0x28, "FONTDESC is 0x28 bytes (cbSizeofstruct in retail)");

// _AfxIsSameFont, RVA 0x1e4748 (mfc140u, unexported).  This is the transcription
// core/ole/CPropbagPropExchange.cpp carries (it lives in that file's anonymous
// namespace, so it is repeated here):
//   if (font.m_pFont == NULL) return FALSE;
//   if (pFontDispAmbient != NULL) {
//       bSame = FALSE;
//       if (SUCCEEDED(pFontDispAmbient->QueryInterface(IID_IFont, &pFontAmbient))) {
//           bSame = pFontAmbient->IsEqual(font.m_pFont) == S_OK;   ; IFont +0xa8
//           pFontAmbient->Release();
//       }
//       return bSame;
//   }
//   if (pFontDesc == NULL) pFontDesc = &_afxFontDescDefault;
//   then a short-circuit chain, each getter's HRESULT ignored:
//     get_Italic == fItalic, get_Underline == fUnderline,
//     get_Strikethrough == fStrikethrough, get_Charset == sCharset,
//     get_Weight == sWeight, get_Size memcmp 8 bytes with cySize,
//     get_Name: wcscmp(CString(bstr), CString(lpstrName)) == 0 (NULL desc
//     buffer -> AtlThrow(E_FAIL)); SysFreeString(bstr).
// Deviation (as in that file): the getter out-parameters are zero-initialised;
// retail leaves them uninitialised.
int AfxIsSameFont(const AR_CFontHolder& font, const FONTDESC* pFontDesc, IFontDisp* pFontDispAmbient) {
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

// _AfxCreateFontFromStream -- RVA 0x1f21a8 (mfc140u, unexported):
//   LPUNKNOWN pUnk = NULL; LPPERSISTSTREAM pps = NULL; CLSID clsid;
//   if (SUCCEEDED(pstm->Read(&clsid, 0x10, NULL))) {                  ; IStream +0x18
//       if (clsid == CLSID_StdFont || clsid == _afx_CLSID_StdFont_V1)  ; memcmp x2
//           hr = OleCreateFontIndirect(&_afxFontDescHelv, IID_IFont, &pUnk);   ; OLEAUT32 #420
//       else
//           hr = CoCreateInstance(clsid, NULL, CLSCTX_INPROC_SERVER, IID_IFont, &pUnk);
//       if (SUCCEEDED(hr)) {
//           if (pUnk == NULL) AfxThrowInvalidArgException();           ; 0x227720 (ENSURE)
//           pUnk->QueryInterface(IID_IPersistStream, &pps);            ; HRESULT ignored
//       }
//       if (pps != NULL) {
//           hr = pps->Load(pstm);                                      ; +0x28
//           pps->Release();
//           if (SUCCEEDED(hr)) return (LPFONT)pUnk;
//       }
//   }
//   if (pUnk != NULL) pUnk->Release();
//   return NULL;
IFont* AfxCreateFontFromStream(IStream* pstm) {
    IUnknown* pUnk = nullptr;
    IPersistStream* pps = nullptr;
    CLSID clsid;
    if (SUCCEEDED(pstm->Read(&clsid, sizeof(CLSID), nullptr))) {
        HRESULT hr;
        if (SameGuid(clsid, kCLSID_StdFont) || SameGuid(clsid, kCLSID_StdFont_V1)) {
            hr = ::OleCreateFontIndirect(const_cast<FONTDESC*>(&kFontDescHelv), kIID_IFont,
                                         reinterpret_cast<void**>(&pUnk));
        } else {
            hr = ::CoCreateInstance(clsid, nullptr, CLSCTX_INPROC_SERVER, kIID_IFont,
                                    reinterpret_cast<void**>(&pUnk));
        }
        if (SUCCEEDED(hr)) {
            if (pUnk == nullptr) {
                impl__AfxThrowInvalidArgException__YAXXZ();
            }
            pUnk->QueryInterface(kIID_IPersistStream, reinterpret_cast<void**>(&pps));
        }
        if (pps != nullptr) {
            hr = pps->Load(pstm);
            pps->Release();
            if (SUCCEEDED(hr)) {
                return reinterpret_cast<IFont*>(pUnk);
            }
        }
    }
    if (pUnk != nullptr) {
        pUnk->Release();
    }
    return nullptr;
}

// _AfxRelease, RVA 0x26ccc4 (mfc140u, unexported):
//   if (*pp != NULL) { (*pp)->Release(); *pp = NULL; }     ; vtable +0x10
void AfxReleaseUnk(IUnknown** pp) {
    if (*pp != nullptr) {
        (*pp)->Release();
        *pp = nullptr;
    }
}

// Scalar deleting destructor, vtable slot 5 -- RVA 0x1efbc0 (mfc140u, unexported),
// the whole body:
//   if (flags & 1) sized-delete(this, 0x20);  return this;
// There is no destructor body at all (the class owns nothing; the vptr is not
// even re-stored).  The sized delete (0x2b77b0) is a `jmp 0x1800027c0`; 0x27c0
// (mfc140u) is the COMDAT-folded body exported as ??_V@YAXPEAX@Z, a bare tail
// jump through the CRT `free` import (0x1802c74e8).  OpenMFC's ??3@YAXPEAX@Z
// thunk (detail/MemcoreSupport.cpp) is likewise std::free.
void* MS_ABI ArchivePropScalarDeletingDtor(void* pThis, unsigned int flags) {
    if (flags & 1) {
        impl___3_YAXPEAX_Z(pThis);
    }
    return pThis;
}

// Vtable in retail slot order (see the header comment).
void* const kArchivePropExchangeVtbl[6] = {
    reinterpret_cast<void*>(&impl__ExchangeVersion_CPropExchange__UEAAHAEAKKH_Z),
    reinterpret_cast<void*>(&impl__ExchangeProp_CArchivePropExchange__UEAAHPEB_WGPEAXPEBX_Z),
    reinterpret_cast<void*>(&impl__ExchangeBlobProp_CArchivePropExchange__UEAAHPEB_WPEAPEAXPEAX_Z),
    reinterpret_cast<void*>(&impl__ExchangeFontProp_CArchivePropExchange__UEAAHPEB_WAEAVCFontHolder__PEBUtagFONTDESC__PEAUIFontDisp___Z),
    reinterpret_cast<void*>(&impl__ExchangePersistentProp_CArchivePropExchange__UEAAHPEB_WPEAPEAUIUnknown__AEBU_GUID__PEAU2__Z),
    reinterpret_cast<void*>(&ArchivePropScalarDeletingDtor),
};

}  // namespace

// Symbol: ??0CArchivePropExchange@@QEAA@AEAVCArchive@@@Z
// Transcribed from RVA 0x1efb90 (mfc140u), the whole body:
//   m_bAsync = 0; m_dwVersion = 0;               ; movq $0,0xc(%rcx) (inlined base ctor)
//   vptr = CArchivePropExchange vftable;
//   m_ar = &ar;                                  ; +0x18
//   m_bLoading = ar.m_nMode & CArchive::load;    ; retail CArchive +0x20, bit 0
//   return this;
// m_bLoading is read through OpenMFC's inline CArchive::IsLoading(), which tests
// the same bit of OpenMFC's own m_nMode (a different offset: see the header
// comment on the CArchive layout).
extern "C" void* MS_ABI impl___0CArchivePropExchange__QEAA_AEAVCArchive___Z(void* pThis, CArchive* pArchive) {
    AR_CArchivePropExchange* self = ape(pThis);
    self->m_bAsync = 0;
    self->m_dwVersion = 0;
    self->vptr = kArchivePropExchangeVtbl;
    self->m_pArchive = pArchive;
    self->m_bLoading = pArchive->IsLoading() ? 1 : 0;
    return pThis;
}

// Symbol: ?ExchangeBlobProp@CArchivePropExchange@@UEAAHPEB_WPEAPEAXPEAX@Z
// Transcribed from RVA 0x1f0120 (mfc140u):
//   hBlob = *phBlob;                                      ; read before the mode test
//   if (m_bLoading) {
//       if (hBlob != NULL) { GlobalFree(hBlob); *phBlob = NULL; }
//       DWORD cb; m_ar >> cb;
//       *phBlob = GlobalAlloc(GMEM_MOVEABLE, cb + sizeof(DWORD));   ; 64-bit add
//       if (*phBlob == NULL) return TRUE;                  ; `je 0x1f0226` -> mov $1,%eax
//       void* pvBlob = GlobalLock(*phBlob);                ; result NOT checked
//       *(DWORD*)pvBlob = cb;
//       if (m_ar.Read((BYTE*)pvBlob + 4, cb) != cb)        ; 0x1d1840
//           AfxThrowArchiveException(endOfFile, NULL);
//       GlobalUnlock(*phBlob);
//   } else if (hBlob != NULL) {
//       void* pvBlob = GlobalLock(hBlob);
//       if (pvBlob == NULL) return FALSE;                  ; `je 0x1f022b` with rax == 0
//       m_ar.Write(pvBlob, *(DWORD*)pvBlob + 4);           ; 0x1d1a70, 32-bit add
//       GlobalUnlock(*phBlob);
//   } else {
//       m_ar << (DWORD)0;
//   }
//   return TRUE;
// hBlobDefault (r9) is never read.  Imports resolved with iatu.py: 0x1802c66d0
// GlobalFree, 0x1802c6620 GlobalAlloc, 0x1802c6628 GlobalLock, 0x1802c6630
// GlobalUnlock (all KERNEL32).
extern "C" int MS_ABI impl__ExchangeBlobProp_CArchivePropExchange__UEAAHPEB_WPEAPEAXPEAX_Z(
    void* pThis, const wchar_t* pszPropName, HGLOBAL* phBlob, HGLOBAL hBlobDefault) {
    (void)pszPropName;
    (void)hBlobDefault;
    AR_CArchivePropExchange* self = ape(pThis);
    CArchive* ar = self->m_pArchive;
    HGLOBAL hBlob = *phBlob;
    if (self->m_bLoading != 0) {
        if (hBlob != nullptr) {
            ::GlobalFree(hBlob);
            *phBlob = nullptr;
        }
        const DWORD cb = ArLoad<DWORD>(ar);
        *phBlob = ::GlobalAlloc(GMEM_MOVEABLE, static_cast<SIZE_T>(cb) + sizeof(DWORD));
        if (*phBlob == nullptr) {
            return TRUE;
        }
        void* pvBlob = ::GlobalLock(*phBlob);
        *static_cast<DWORD*>(pvBlob) = cb;
        if (impl__Read_CArchive__QEAAIPEAXI_Z(self->m_pArchive, static_cast<BYTE*>(pvBlob) + sizeof(DWORD), cb) != cb) {
            impl__AfxThrowArchiveException__YAXHPEB_W_Z(kArchiveEndOfFile, nullptr);
        }
        ::GlobalUnlock(*phBlob);
        return TRUE;
    }
    if (hBlob != nullptr) {
        void* pvBlob = ::GlobalLock(hBlob);
        if (pvBlob == nullptr) {
            return FALSE;
        }
        impl__Write_CArchive__QEAAXPEBXI_Z(self->m_pArchive, pvBlob,
                                           *static_cast<const DWORD*>(pvBlob) + static_cast<DWORD>(sizeof(DWORD)));
        ::GlobalUnlock(*phBlob);
        return TRUE;
    }
    ArStore<DWORD>(ar, 0);
    return TRUE;
}

// Symbol: ?ExchangeFontProp@CArchivePropExchange@@UEAAHPEB_WAEAVCFontHolder@@PEBUtagFONTDESC@@PEAUIFontDisp@@@Z
// Transcribed from RVA 0x1f07d0 (mfc140u):
//   CArchiveStream stm(&m_ar);                            ; inlined: vftable 0x180321088, m_pArchive
//   if (m_bLoading) {
//       BYTE bFlag; m_ar >> bFlag;
//       if (bFlag != 0xFF) {
//           LPFONT pFont = _AfxCreateFontFromStream(_AfxGetArchiveStream(m_ar, stm));  ; 0x1ef698, 0x1f21a8
//           if (pFont != NULL) { font.SetFont(pFont); return TRUE; }                   ; 0x1e4b10
//       }
//       font.InitializeFont(pFontDesc, pFontDispAmbient);  ; 0x1e4680
//       return TRUE;
//   }
//   if (font.m_pFont == NULL || _AfxIsSameFont(font, pFontDesc, pFontDispAmbient)) {  ; 0x1e4748
//       m_ar << (BYTE)0xFF; return TRUE;
//   }
//   LPPERSISTSTREAM pps = NULL;
//   if (FAILED(font.m_pFont->QueryInterface(IID_IPersistStream, &pps))) {
//       m_ar << (BYTE)0xFF; return TRUE;
//   }
//   m_ar << (BYTE)0x00;
//   bSuccess = SUCCEEDED(OleSaveToStream(pps, _AfxGetArchiveStream(m_ar, stm)));  ; ole32 (0x1802c7a90)
//   pps->Release();
//   if (!bSuccess) AfxThrowArchiveException(genericException, NULL);
//   return TRUE;
// Unlike CPropbagPropExchange, the save path has no m_bSaveAllProperties test
// (this class has no such member).  The same CFontHolder caveat as
// core/ole/CPropbagPropExchange.cpp's ExchangeFontProp applies: OpenMFC's
// CFontHolder thunks keep the font in a side table and do not write +0x0, which
// this body reads exactly as retail reads its real m_pFont.
extern "C" int MS_ABI impl__ExchangeFontProp_CArchivePropExchange__UEAAHPEB_WAEAVCFontHolder__PEBUtagFONTDESC__PEAUIFontDisp___Z(
    void* pThis, const wchar_t* pszPropName, void* pFontHolder, const FONTDESC* pFontDesc, IFontDisp* pFontDispAmbient) {
    (void)pszPropName;
    AR_CArchivePropExchange* self = ape(pThis);
    CArchive* ar = self->m_pArchive;
    ArchiveStreamStorage stm;
    impl___0CArchiveStream__QEAA_PEAVCArchive___Z(&stm, ar);
    const AR_CFontHolder& font = *static_cast<const AR_CFontHolder*>(pFontHolder);

    if (self->m_bLoading != 0) {
        const BYTE bFlag = ArLoad<BYTE>(ar);
        if (bFlag != 0xFF) {
            IFont* pFont = AfxCreateFontFromStream(AfxGetArchiveStream(ar, &stm));
            if (pFont != nullptr) {
                impl__SetFont_CFontHolder__QEAAXPEAUIFont___Z(pFontHolder, pFont);
                return TRUE;
            }
        }
        impl__InitializeFont_CFontHolder__QEAAXPEBUtagFONTDESC__PEAUIDispatch___Z(
            pFontHolder, pFontDesc, pFontDispAmbient);
        return TRUE;
    }

    IFont* pFont = font.m_pFont;
    if (pFont != nullptr && !AfxIsSameFont(font, pFontDesc, pFontDispAmbient)) {
        IPersistStream* pps = nullptr;
        if (SUCCEEDED(pFont->QueryInterface(kIID_IPersistStream, reinterpret_cast<void**>(&pps)))) {
            ArStore<BYTE>(ar, 0x00);
            IStream* pstm = AfxGetArchiveStream(ar, &stm);
            const int bSuccess = SUCCEEDED(::OleSaveToStream(pps, pstm)) ? TRUE : FALSE;
            pps->Release();
            if (!bSuccess) {
                impl__AfxThrowArchiveException__YAXHPEB_W_Z(kArchiveGenericException, nullptr);
            }
            return TRUE;
        }
    }
    ArStore<BYTE>(ar, 0xFF);
    return TRUE;
}

// Symbol: ?ExchangePersistentProp@CArchivePropExchange@@UEAAHPEB_WPEAPEAUIUnknown@@AEBU_GUID@@PEAU2@@Z
// Transcribed from RVA 0x1f0270 (mfc140u):
//   CArchiveStream stm(&m_ar);                            ; inlined: vftable 0x180321088, m_pArchive
//   bSuccess = FALSE;
//   if (m_bLoading) {
//       _AfxRelease(ppUnk); *ppUnk = NULL;                ; call 0x26ccc4
//       BYTE bFlag; m_ar >> bFlag;
//       if (bFlag == 0xFF) {                              ; use the default
//           bSuccess = pUnkDefault == NULL ||
//                      SUCCEEDED(pUnkDefault->QueryInterface(iid, ppUnk));
//       } else {
//           CLSID clsid; m_ar >> clsid.Data1 >> clsid.Data2 >> clsid.Data3;
//           if (m_ar.Read(clsid.Data4, 8) != 8) AfxThrowArchiveException(endOfFile, NULL);
//           if (clsid == GUID_NULL) return TRUE;
//           LPSTREAM pstm = _AfxGetArchiveStream(m_ar, stm);            ; 0x1ef698
//           if (clsid == CLSID_StdPicture || clsid == _afx_CLSID_StdPicture_V1)
//               bSuccess = SUCCEEDED(OleLoadPicture(pstm, 0, FALSE, iid, ppUnk));   ; OLEAUT32 #418
//           else {
//               pstm->Seek(-16, STREAM_SEEK_CUR, NULL);   ; IStream +0x28, HRESULT ignored
//               // _AfxOleLoadFromStream, inlined:
//               CLSID clsid2;
//               if (SUCCEEDED(ReadClassStm(pstm, &clsid2)) &&
//                   (SUCCEEDED(CoCreateInstance(clsid2, NULL, CLSCTX_SERVER, iid, ppUnk)) ||      ; 0x15
//                    SUCCEEDED(CoCreateInstance(clsid2, NULL, CLSCTX_INPROC_SERVER |
//                                                            CLSCTX_LOCAL_SERVER, iid, ppUnk)))) {  ; 5
//                   LPPERSISTSTREAM pps = NULL;
//                   if (SUCCEEDED((*ppUnk)->QueryInterface(IID_IPersistStream, &pps)) ||
//                       SUCCEEDED((*ppUnk)->QueryInterface(IID_IPersistStreamInit, &pps))) {
//                       hr = pps->Load(pstm); pps->Release();                                  ; +0x28
//                       bSuccess = SUCCEEDED(hr);
//                   }
//                   if (!bSuccess) { (*ppUnk)->Release(); *ppUnk = NULL; }
//               }
//           }
//       }
//   } else {
//       // same-object test, inlined: pointer equality, else (both non-NULL and)
//       // both QueryInterface(iid) succeed and return the same pointer; each
//       // interface obtained is Released.
//       if (bSame) { m_ar << (BYTE)0xFF; bSuccess = TRUE; }
//       else {
//           m_ar << (BYTE)0x00;
//           if (*ppUnk != NULL) {
//               if (SUCCEEDED(QI(IID_IPersistStream, &pps)) || SUCCEEDED(QI(IID_IPersistStreamInit, &pps))) {
//                   bSuccess = SUCCEEDED(OleSaveToStream(pps, _AfxGetArchiveStream(m_ar, stm)));  ; ole32
//                   pps->Release();
//               }
//           } else {
//               m_ar.Write(&GUID_NULL, 16);               ; 0x1d1a70 -- bSuccess stays FALSE
//           }
//       }
//   }
//   if (!bSuccess) AfxThrowArchiveException(genericException, NULL);   ; 0x1f0742
//   return TRUE;
// Note the retail quirk transcribed as-is: saving a NULL *ppUnk that differs from
// pUnkDefault writes the 0x00 flag and GUID_NULL and then throws
// genericException, because nothing sets bSuccess on that path (the load side
// would read GUID_NULL back and return TRUE).
// Deviation: the Seek(-16) + ReadClassStm pair on the non-picture load path is
// replaced by reusing the CLSID already read (see the comment at that point and
// at AfxGetArchiveStream above); ReadClassStm's failure arm therefore cannot be
// taken.  Imports resolved with iatu.py:
// 0x1802c6b30 OLEAUT32 #418 (OleLoadPicture), 0x1802c7a88 ole32 ReadClassStm,
// 0x1802c78a8 ole32 CoCreateInstance, 0x1802c7a90 ole32 OleSaveToStream.
extern "C" int MS_ABI impl__ExchangePersistentProp_CArchivePropExchange__UEAAHPEB_WPEAPEAUIUnknown__AEBU_GUID__PEAU2__Z(
    void* pThis, const wchar_t* pszPropName, IUnknown** ppUnk, const GUID* piid, IUnknown* pUnkDefault) {
    (void)pszPropName;
    AR_CArchivePropExchange* self = ape(pThis);
    CArchive* ar = self->m_pArchive;
    ArchiveStreamStorage stm;
    impl___0CArchiveStream__QEAA_PEAVCArchive___Z(&stm, ar);
    const GUID& iid = *piid;
    int bSuccess = FALSE;

    if (self->m_bLoading != 0) {
        AfxReleaseUnk(ppUnk);
        *ppUnk = nullptr;
        const BYTE bFlag = ArLoad<BYTE>(ar);
        if (bFlag == 0xFF) {
            bSuccess = (pUnkDefault == nullptr ||
                        SUCCEEDED(pUnkDefault->QueryInterface(iid, reinterpret_cast<void**>(ppUnk)))) ? TRUE : FALSE;
        } else {
            CLSID clsid;
            clsid.Data1 = ArLoad<DWORD>(ar);
            clsid.Data2 = ArLoad<WORD>(ar);
            clsid.Data3 = ArLoad<WORD>(ar);
            if (impl__Read_CArchive__QEAAIPEAXI_Z(ar, &clsid.Data4[0], sizeof(clsid.Data4)) != sizeof(clsid.Data4)) {
                impl__AfxThrowArchiveException__YAXHPEB_W_Z(kArchiveEndOfFile, nullptr);
            }
            if (SameGuid(clsid, kGUID_NULL)) {
                return TRUE;
            }
            IStream* pstm = AfxGetArchiveStream(ar, &stm);
            if (SameGuid(clsid, kCLSID_StdPicture) || SameGuid(clsid, kCLSID_StdPicture_V1)) {
                bSuccess = SUCCEEDED(::OleLoadPicture(pstm, 0, FALSE, iid, reinterpret_cast<void**>(ppUnk))) ? TRUE : FALSE;
            } else {
                // Deviation: retail's Seek(-16, STREAM_SEEK_CUR) + ReadClassStm pair is
                // not issued.  Through OpenMFC's CArchiveStream::Seek (core/ole/
                // CArchiveStream.cpp) it would move the FILE back 16 bytes while the
                // loading archive's buffer still holds the bytes after the CLSID
                // (OpenMFC's CArchive::Flush neither rewinds nor empties the buffer;
                // retail's Flush 0x1d1be0 does both), so ReadClassStm would read object
                // data as the CLSID and every later read would be mispositioned.  On a
                // correctly positioned stream the pair re-reads exactly the 16 bytes
                // just consumed and succeeds, so clsid2 == clsid and the stream ends
                // up just past the CLSID -- which is where the archive already is.
                const CLSID clsid2 = clsid;
                {
                    HRESULT hr = ::CoCreateInstance(clsid2, nullptr, CLSCTX_SERVER, iid,
                                                    reinterpret_cast<void**>(ppUnk));
                    if (FAILED(hr)) {
                        hr = ::CoCreateInstance(clsid2, nullptr, CLSCTX_INPROC_SERVER | CLSCTX_LOCAL_SERVER, iid,
                                                reinterpret_cast<void**>(ppUnk));
                    }
                    if (SUCCEEDED(hr)) {
                        IPersistStream* pps = nullptr;
                        if (SUCCEEDED((*ppUnk)->QueryInterface(kIID_IPersistStream, reinterpret_cast<void**>(&pps))) ||
                            SUCCEEDED((*ppUnk)->QueryInterface(kIID_IPersistStreamInit, reinterpret_cast<void**>(&pps)))) {
                            hr = pps->Load(pstm);
                            pps->Release();
                            bSuccess = SUCCEEDED(hr) ? TRUE : FALSE;
                        }
                        if (!bSuccess) {
                            (*ppUnk)->Release();
                            *ppUnk = nullptr;
                        }
                    }
                }
            }
        }
    } else {
        IUnknown* pUnk = *ppUnk;
        int bSame = FALSE;
        if (pUnk == pUnkDefault) {
            bSame = TRUE;
        } else if (pUnk != nullptr && pUnkDefault != nullptr) {
            // Retail order: QI *ppUnk into a local (-0x40(%rbp)); only on success QI
            // pUnkDefault (-0x28(%rbp)); on that success compare, then Release the
            // default's interface; then Release the first one.
            IUnknown* pUnk1 = nullptr;
            IUnknown* pUnk2 = nullptr;
            if (SUCCEEDED(pUnk->QueryInterface(iid, reinterpret_cast<void**>(&pUnk1)))) {
                if (SUCCEEDED(pUnkDefault->QueryInterface(iid, reinterpret_cast<void**>(&pUnk2)))) {
                    bSame = (pUnk1 == pUnk2) ? TRUE : FALSE;
                    pUnk2->Release();
                }
                pUnk1->Release();
            }
        }
        if (bSame) {
            ArStore<BYTE>(ar, 0xFF);
            bSuccess = TRUE;
        } else {
            ArStore<BYTE>(ar, 0x00);
            if (*ppUnk != nullptr) {
                IPersistStream* pps = nullptr;
                if (SUCCEEDED((*ppUnk)->QueryInterface(kIID_IPersistStream, reinterpret_cast<void**>(&pps))) ||
                    SUCCEEDED((*ppUnk)->QueryInterface(kIID_IPersistStreamInit, reinterpret_cast<void**>(&pps)))) {
                    IStream* pstm = AfxGetArchiveStream(ar, &stm);
                    bSuccess = SUCCEEDED(::OleSaveToStream(pps, pstm)) ? TRUE : FALSE;
                    pps->Release();
                }
            } else {
                impl__Write_CArchive__QEAAXPEBXI_Z(ar, &kGUID_NULL, sizeof(GUID));
            }
        }
    }

    if (!bSuccess) {
        impl__AfxThrowArchiveException__YAXHPEB_W_Z(kArchiveGenericException, nullptr);
    }
    return TRUE;
}

// Symbol: ?ExchangeProp@CArchivePropExchange@@UEAAHPEB_WGPEAXPEBX@Z
// Transcribed from RVA 0x1efbf0 (mfc140u):
//   if (m_bLoading) switch (vtProp) {
//     VT_UI1:  m_ar >> *(BYTE*)pvProp;
//     VT_I2:   m_ar >> *(WORD*)pvProp;
//     VT_I4, VT_R4: m_ar >> *(DWORD*)pvProp;               ; one shared 4-byte path
//     VT_R8:   m_ar >> *(ULONGLONG*)pvProp;
//     VT_CY:   m_ar >> ((CY*)pvProp)->Lo; m_ar >> ((CY*)pvProp)->Hi;
//     VT_BOOL: *(BOOL*)pvProp = 0; m_ar >> *(BYTE*)pvProp; ; zeroed BEFORE the mode test
//     VT_BSTR, VT_LPSTR: m_ar >> *(CString*)pvProp;       ; call 0x1b5e4
//     default: nothing;
//   } else switch (vtProp) {
//     same types, `m_ar << ...`; VT_BOOL stores only the low byte of the BOOL;
//     VT_BSTR, VT_LPSTR: m_ar << *(CString*)pvProp;       ; call 0x1b818
//   }
//   return TRUE;                                          ; r15d == 1 on every path
// pvDefault (the stack argument) is never read.
extern "C" int MS_ABI impl__ExchangeProp_CArchivePropExchange__UEAAHPEB_WGPEAXPEBX_Z(
    void* pThis, const wchar_t* pszPropName, unsigned short vtProp, void* pvProp, const void* pvDefault) {
    (void)pszPropName;
    (void)pvDefault;
    AR_CArchivePropExchange* self = ape(pThis);
    CArchive* ar = self->m_pArchive;
    if (self->m_bLoading != 0) {
        switch (vtProp) {
        case VT_UI1:
            *static_cast<BYTE*>(pvProp) = ArLoad<BYTE>(ar);
            break;
        case VT_I2:
            *static_cast<WORD*>(pvProp) = ArLoad<WORD>(ar);
            break;
        case VT_I4:
        case VT_R4:
            *static_cast<DWORD*>(pvProp) = ArLoad<DWORD>(ar);
            break;
        case VT_R8:
            *static_cast<unsigned long long*>(pvProp) = ArLoad<unsigned long long>(ar);
            break;
        case VT_CY: {
            CY* pcy = static_cast<CY*>(pvProp);
            pcy->Lo = ArLoad<DWORD>(ar);
            pcy->Hi = static_cast<LONG>(ArLoad<DWORD>(ar));
            break;
        }
        case VT_BOOL:
            *static_cast<DWORD*>(pvProp) = 0;
            *static_cast<BYTE*>(pvProp) = ArLoad<BYTE>(ar);
            break;
        case VT_BSTR:
        case VT_LPSTR:
            ArReadCString(ar, *static_cast<CString*>(pvProp));
            break;
        default:
            break;
        }
    } else {
        switch (vtProp) {
        case VT_UI1:
        case VT_BOOL:
            ArStore<BYTE>(ar, *static_cast<const BYTE*>(pvProp));
            break;
        case VT_I2:
            ArStore<WORD>(ar, *static_cast<const WORD*>(pvProp));
            break;
        case VT_I4:
        case VT_R4:
            ArStore<DWORD>(ar, *static_cast<const DWORD*>(pvProp));
            break;
        case VT_R8:
            ArStore<unsigned long long>(ar, *static_cast<const unsigned long long*>(pvProp));
            break;
        case VT_CY: {
            const CY* pcy = static_cast<const CY*>(pvProp);
            ArStore<DWORD>(ar, pcy->Lo);
            ArStore<DWORD>(ar, static_cast<DWORD>(pcy->Hi));
            break;
        }
        case VT_BSTR:
        case VT_LPSTR:
            ArWriteCString(ar, *static_cast<const CString*>(pvProp));
            break;
        default:
            break;
        }
    }
    return TRUE;
}
