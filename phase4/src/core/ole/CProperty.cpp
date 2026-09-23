// CProperty — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp

#include "detail/ManualSmallStubImplementationsSupport.h"

#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <cwchar>

// ---------------------------------------------------------------------------
// CProperty: one property of an OLE property-set section (MFC's src/mfc/
// oleprop.h; it is not declared in any public header, retail or OpenMFC).
// Client code only ever holds it through the exported thunks below, so every
// thunk takes a `void* pThis` and views it through CP_Property.
//
// Layout, pinned by the retail constructors (mfc140u):
//   ??0CProperty@@QEAA@XZ        RVA 0x261960: `xor eax,eax; mov rax,(rcx);
//                                mov rax,0x8(rcx)` -- zeroes +0x00..+0x0F.
//   ??0CProperty@@QEAA@KQEAXK@Z  RVA 0x261970: `mov edx,(rcx)` (id at +0x00),
//                                `movq $0,0x8(rcx)` (value at +0x08),
//                                `mov r9d,0x4(rcx)` (type at +0x04).
// sizeof == 0x10 (CPropertySection::Set operator-new's 0x10 bytes for one; see
// the header comment of core/ole/CPropertySection.cpp).
//
// Value representation (decoded from Set/Get below).  m_pValue is a single
// malloc'ed block.  Scalars are stored raw.  Counted items are a DWORD count
// followed by the payload; for "wide" items the count is in WCHARs (payload =
// count*2 bytes), for "byte" items it is in bytes.  A VT_VECTOR value is a
// DWORD element count followed by the elements back to back.
//
// Which image: every body below was read from mfc140u.dll (disas.py --u, RVAs
// resolved through the export table by ordinal).  For Set, Get,
// WriteToStream and ReadFromStream the ANSI twin mfc140.dll is NOT
// byte-identical: its string handling differs (e.g. its Set, at 0x2609a0 in
// mfc140.dll, counts VT_BSTR in bytes via strlen and calls wcslen only for
// VT_LPWSTR), so the ANSI disassembly must not be used for these four.
//
// Allocation: retail calls the CRT's malloc/calloc/realloc/free directly
// (IAT slots resolved with iatu.py: malloc 0x1802c74c8, free 0x1802c74e8,
// calloc 0x1802c74f0, realloc 0x1802c74e0), never operator new, so this file
// does the same.
// ---------------------------------------------------------------------------

namespace {

struct CP_Property {
    DWORD m_dwPropID;
    DWORD m_dwType;
    void* m_pValue;
};
static_assert(offsetof(CP_Property, m_dwPropID) == 0x00, "CProperty::m_dwPropID");
static_assert(offsetof(CP_Property, m_dwType) == 0x04, "CProperty::m_dwType");
static_assert(offsetof(CP_Property, m_pValue) == 0x08, "CProperty::m_pValue");
static_assert(sizeof(CP_Property) == 0x10, "CProperty is 0x10 bytes");

inline CP_Property* CP(void* pThis) { return static_cast<CP_Property*>(pThis); }

const DWORD kVtVector = 0x1000;  // VT_VECTOR

// Per-element classification.  Every body below inlines the same compiled
// switch; it is factored out here, once per variant.
enum CP_Kind { CP_INVALID, CP_FIXED, CP_COUNTED_BYTES, CP_COUNTED_WCHARS };

// Fixed-size element types, identical in all four switches:
//   VT_EMPTY 0 -> 0; VT_I2 2, VT_BOOL 0xb -> 2; VT_I4 3, VT_R4 4 -> 4;
//   VT_R8 5, VT_CY 6, VT_DATE 7, VT_I8 0x14, VT_FILETIME 0x40 -> 8;
//   VT_CLSID 0x48 -> 16.  Anything not listed in a switch (VT_NULL 1 included)
//   is rejected.
bool cp_fixed_size(DWORD vt, DWORD* pcb) {
    switch (vt) {
    case 0x00: *pcb = 0; return true;
    case 0x02: case 0x0b: *pcb = 2; return true;
    case 0x03: case 0x04: *pcb = 4; return true;
    case 0x05: case 0x06: case 0x07: case 0x14: case 0x40: *pcb = 8; return true;
    case 0x48: *pcb = 16; return true;
    default: return false;
    }
}

// The "TCHAR string" types: in memory (mfc140u) they are WCHAR-counted; on the
// stream they are CP_THREAD_ACP byte strings, converted by cp_convert_strings.
// VT_BSTR 8, VT_STREAM 0x42, VT_STORAGE 0x43, VT_STREAMED_OBJECT 0x44,
// VT_STORED_OBJECT 0x45, 0x49 (VT_VERSIONED_STREAM) and 0x4a.  This exact set
// is the conversion switch in WriteToStream (0x261e3e..0x261e5d) and in
// ReadFromStream (0x2622c6..0x2622e6); 0x49/0x4a are what the compiled switch
// tests, whatever their source spelling.
bool cp_is_tchar_string(DWORD vt) {
    switch (vt) {
    case 0x08: case 0x42: case 0x43: case 0x44: case 0x45: case 0x49: case 0x4a:
        return true;
    default:
        return false;
    }
}

// In-memory classification used by Set (switch at 0x261b03) and Get (switch at
// 0x261c98): WCHAR-counted = the TCHAR-string set plus VT_LPWSTR 0x1f;
// byte-counted = VT_LPSTR 0x1e, VT_BLOB 0x41, VT_BLOB_OBJECT 0x46, VT_CF 0x47
// and 0x4b.
CP_Kind cp_memory_kind(DWORD vt, DWORD* pcbFixed) {
    if (cp_fixed_size(vt, pcbFixed)) return CP_FIXED;
    if (vt == 0x1f || cp_is_tchar_string(vt)) return CP_COUNTED_WCHARS;
    switch (vt) {
    case 0x1e: case 0x41: case 0x46: case 0x47: case 0x4b: return CP_COUNTED_BYTES;
    default: return CP_INVALID;
    }
}

// On-stream classification used by WriteToStream (switch at 0x261e8b) and
// ReadFromStream (switch at 0x2620eb): only VT_LPWSTR stays WCHAR-counted; the
// TCHAR-string set is byte-counted there because it travels as ANSI.
CP_Kind cp_stream_kind(DWORD vt, DWORD* pcbFixed) {
    if (cp_fixed_size(vt, pcbFixed)) return CP_FIXED;
    if (vt == 0x1f) return CP_COUNTED_WCHARS;
    if (cp_is_tchar_string(vt)) return CP_COUNTED_BYTES;
    switch (vt) {
    case 0x1e: case 0x41: case 0x46: case 0x47: case 0x4b: return CP_COUNTED_BYTES;
    default: return CP_INVALID;
    }
}

// Non-exported helper at RVA 0x2616dc (mfc140u), called from Set: copy a
// NUL-terminated wide string into a fresh {DWORD cch; WCHAR sz[cch]} block,
// cch including the terminator.
//   n = (p ? wcslen(p) : 0) + 1;                   (wcslen = IAT 0x1802c7748)
//   buf = malloc(n*2 + 4);  if (!buf) return NULL;
//   *(DWORD*)buf = n;
//   if (!p) ((WCHAR*)(buf+4))[0] = 0; else wcscpy_s(buf+4, n, p);  (IAT 0x1802c77b8)
// Deviation: wcscpy_s is written as a memcpy of n WCHARs.  The destination is
// sized from the same wcslen, so wcscpy_s's error returns (retail routes ENOMEM
// to AfxThrowMemoryException 0x2276c0 and anything but 0/STRUNCATE to
// AfxThrowInvalidArgException 0x227720) cannot occur; likewise the
// SIZE_MAX/n < 2 overflow guard before malloc (AtlThrow 0x80070216) cannot fire
// for a 32-bit n.
BYTE* cp_counted_wide_copy(const WCHAR* p) {
    DWORD n = (p != nullptr ? static_cast<DWORD>(std::wcslen(p)) : 0u) + 1u;
    BYTE* buf = static_cast<BYTE*>(std::malloc(static_cast<size_t>(n) * 2u + 4u));
    if (buf != nullptr) {
        *reinterpret_cast<DWORD*>(buf) = n;
        if (p == nullptr)
            reinterpret_cast<WCHAR*>(buf + 4)[0] = 0;
        else
            std::memcpy(buf + 4, p, static_cast<size_t>(n) * sizeof(WCHAR));
    }
    return buf;
}

// The VT_LPSTR branch inlined in Set (0x261a96..0x261ae3): the same thing for
// a byte string.  n = strlen(p) + 1 (IAT 0x1802c7810); buf = malloc((int)n + 4)
// (`movslq %ebp`, sign-extended); *(DWORD*)buf = n; strcpy_s(buf+4, n, p)
// (IAT 0x1802c7740).  Same memcpy deviation and reasoning as above.
BYTE* cp_counted_ansi_copy(const char* p) {
    DWORD n = static_cast<DWORD>(std::strlen(p)) + 1u;
    BYTE* buf = static_cast<BYTE*>(std::malloc(static_cast<size_t>(static_cast<INT_PTR>(static_cast<int>(n))) + 4u));
    if (buf != nullptr) {
        *reinterpret_cast<DWORD*>(buf) = n;
        std::memcpy(buf + 4, p, n);
    }
    return buf;
}

// Non-exported helper at RVA 0x261780 (mfc140u): convert nReps counted strings
// starting at pCur between WCHAR and CP_THREAD_ACP bytes, into a fresh block.
// nCharSize is the OUTPUT character size: WriteToStream passes 1
// (WideCharToMultiByte), ReadFromStream passes 2 (MultiByteToWideChar).
// Transcription:
//   pTemp = calloc(0x400, nCharSize); if (!pTemp) return NULL;
//   if (dwType & VT_VECTOR) { pResult = malloc(4); if (!pResult) { free(pTemp);
//       return NULL; } *(DWORD*)pResult = nReps; cbOffset = 4; }
//   else { pResult = NULL; cbOffset = 0; }
//   while (nReps) {
//     cch = *(DWORD*)pCur; pStr = pCur + 4; n = min(cch, 0x400);
//     nCharSize == 2: cchOut = n ? MultiByteToWideChar(3, 0, pStr, -1, pTemp, n) : 0;
//                     if (cchOut > 0) pTemp[cchOut-1] = 0;
//     else:           cchOut = n ? WideCharToMultiByte(3, 0, pStr, -1, pTemp, n, 0, 0) : 0;
//     cbBytes = cchOut * nCharSize; cbNew = cbBytes + 4 + cbOffset;  (32-bit)
//     if (cbNew < cbOffset || cbNew < cbBytes || cbNew < 4) -> free both, NULL;
//     p = realloc(pResult, cbNew); if (!p) -> free both, NULL;
//     --nReps; memcpy(p+cbOffset, &cchOut, 4); memcpy(p+cbOffset+4, pTemp, cbBytes);
//     pCur = pStr + cch * nCharSize; cbOffset = cbNew;
//   }
//   free(pTemp); return pResult;
// IATs (iatu.py): MultiByteToWideChar 0x1802c6610, WideCharToMultiByte
// 0x1802c69a0; code page 3 == CP_THREAD_ACP.
// Retail quirk kept as-is: the input stride multiplies by the OUTPUT character
// size, which is the wrong width for the input (a vector with more than one
// string is walked at the wrong stride).  Omitted: in the MultiByteToWideChar
// path, the signed `cchOut > n` check (`cmp %ebx,%edi; jg` at 0x26183f inside
// this helper) that branches to the AtlThrow(E_FAIL) call at 0x26194f --
// MultiByteToWideChar cannot return more than the n it was given.
BYTE* cp_convert_strings(BYTE* pCur, DWORD dwType, DWORD nReps, DWORD nCharSize) {
    void* pTemp = std::calloc(0x400, nCharSize);
    if (pTemp == nullptr) return nullptr;
    DWORD cbOffset = 4;
    BYTE* pResult;
    if (dwType & kVtVector) {
        pResult = static_cast<BYTE*>(std::malloc(4));
        if (pResult == nullptr) {
            std::free(pTemp);
            return nullptr;
        }
        *reinterpret_cast<DWORD*>(pResult) = nReps;
    } else {
        pResult = nullptr;
        cbOffset = 0;
    }
    while (nReps != 0) {
        DWORD cch = *reinterpret_cast<DWORD*>(pCur);
        BYTE* pStr = pCur + 4;
        DWORD n = cch < 0x400u ? cch : 0x400u;
        int cchOut;
        if (nCharSize == 2) {
            if (n == 0) {
                cchOut = 0;
            } else {
                cchOut = ::MultiByteToWideChar(CP_THREAD_ACP, 0, reinterpret_cast<LPCSTR>(pStr), -1,
                                               static_cast<LPWSTR>(pTemp), static_cast<int>(n));
                if (cchOut > 0) static_cast<WCHAR*>(pTemp)[cchOut - 1] = 0;
            }
        } else {
            cchOut = (n == 0) ? 0
                              : ::WideCharToMultiByte(CP_THREAD_ACP, 0, reinterpret_cast<LPCWSTR>(pStr), -1,
                                                      static_cast<LPSTR>(pTemp), static_cast<int>(n),
                                                      nullptr, nullptr);
        }
        DWORD cbBytes = static_cast<DWORD>(cchOut) * nCharSize;
        DWORD cbNew = cbBytes + 4u + cbOffset;
        if (cbNew < cbOffset || cbNew < cbBytes || cbNew < 4u) {
            std::free(pResult);
            std::free(pTemp);
            return nullptr;
        }
        BYTE* p = static_cast<BYTE*>(std::realloc(pResult, cbNew));
        if (p == nullptr) {
            std::free(pResult);
            std::free(pTemp);
            return nullptr;
        }
        --nReps;
        pResult = p;
        std::memcpy(p + cbOffset, &cchOut, 4);
        std::memcpy(p + cbOffset + 4, pTemp, cbBytes);
        if (nReps == 0) break;
        pCur = pStr + static_cast<size_t>(cch * nCharSize);
        cbOffset = cbNew;
    }
    std::free(pTemp);
    return pResult;
}

}  // namespace

// Sibling thunk defined further down in this file (the constructor calls it).
extern "C" int MS_ABI impl__Set_CProperty__QEAAHQEAX_Z(void* pThis, void* pValue);

// Symbol: ??0CProperty@@QEAA@KQEAXK@Z
extern "C" void* MS_ABI impl___0CProperty__QEAA_KQEAXK_Z(
    void* pThis, unsigned long dwPropID, void* pValue, unsigned long dwType) {
    // Retail RVA 0x261970 (mfc140u): m_dwPropID = dwPropID; m_pValue = NULL;
    // m_dwType = dwType; Set(pValue) (call 0x2619d0, result ignored); return this.
    CP_Property* p = CP(pThis);
    p->m_dwPropID = dwPropID;
    p->m_pValue = nullptr;
    p->m_dwType = dwType;
    impl__Set_CProperty__QEAAHQEAX_Z(pThis, pValue);
    return pThis;
}
// Symbol: ??0CProperty@@QEAA@XZ
extern "C" void* MS_ABI impl___0CProperty__QEAA_XZ(void* pThis) {
    // Retail RVA 0x261960 (mfc140u): zero the id, the type and m_pValue.
    CP_Property* p = CP(pThis);
    p->m_dwPropID = 0;
    p->m_dwType = 0;
    p->m_pValue = nullptr;
    return pThis;
}
// Symbol: ?AllocValue@CProperty@@AEAAPEAXK@Z
extern "C" void* MS_ABI impl__AllocValue_CProperty__AEAAPEAXK_Z(void* pThis, unsigned long cb) {
    // Retail RVA 0x262380 (mfc140u): m_pValue = malloc((int)cb) (`movslq %edx`,
    // sign-extended), returning the new pointer.  The old value is not freed.
    CP_Property* p = CP(pThis);
    p->m_pValue = std::malloc(static_cast<size_t>(static_cast<INT_PTR>(static_cast<int>(cb))));
    return p->m_pValue;
}

// Symbol: ?FreeValue@CProperty@@AEAAXXZ
extern "C" void MS_ABI impl__FreeValue_CProperty__AEAAXXZ(void* pThis) {
    // Retail RVA 0x2623a0 (mfc140u): if (m_pValue) { free(m_pValue); m_pValue = NULL; }
    CP_Property* p = CP(pThis);
    if (p->m_pValue != nullptr) {
        std::free(p->m_pValue);
        p->m_pValue = nullptr;
    }
}

// Symbol: ??1CProperty@@QEAA@XZ
extern "C" void MS_ABI impl___1CProperty__QEAA_XZ(void* pThis) {
    // Retail RVA 0x2619a0 (mfc140u): `jmp 0x2623a0` -- tail-call FreeValue.
    impl__FreeValue_CProperty__AEAAXXZ(pThis);
}

// Symbol: ?Get@CProperty@@QEAAPEAXPEAK@Z
extern "C" void* MS_ABI impl__Get_CProperty__QEAAPEAXPEAK_Z(void* pThis, unsigned long* pcb) {
    // Retail RVA 0x261c40 (mfc140u).  Walks the value to compute its size:
    //   ret = m_pValue; dwType = m_dwType;
    //   vector:  nReps = *(DWORD*)m_pValue; pCur = m_pValue+4; cb = 4; strip VT_VECTOR
    //   else:    nReps = 1; pCur = m_pValue; cb = 0
    //   (vector with nReps == 0 goes straight to the tail)
    //   per element (switch at 0x261c98, cp_memory_kind):
    //     fixed:   cb += size
    //     counted: cbItem = *(DWORD*)pCur (x2 if WCHAR-counted); pCur += 4;
    //              if vector cb += 4;
    //              if !vector and dwType is VT_LPWSTR (0x1f) or VT_LPSTR (0x1e):
    //                  ret = pCur   (the string, past its count)
    //              cb += cbItem
    //     invalid: return NULL, *pcb untouched
    //     pCur += item size
    //   if (pcb) *pcb = cb; return ret;
    // So a non-vector count is excluded from *pcb, and only the two LPxSTR
    // types return a pointer past the count (VT_BSTR returns the counted block).
    // As in retail, m_pValue is dereferenced without a NULL check for a vector
    // or a counted type.
    CP_Property* p = CP(pThis);
    BYTE* ret = static_cast<BYTE*>(p->m_pValue);
    DWORD dwType = p->m_dwType;
    const bool bVector = (dwType & kVtVector) != 0;
    DWORD nReps;
    DWORD cb;
    BYTE* pCur;
    if (bVector) {
        nReps = *reinterpret_cast<DWORD*>(ret);
        pCur = ret + 4;
        dwType &= ~kVtVector;
        cb = 4;
    } else {
        nReps = 1;
        cb = 0;
        pCur = ret;
    }
    for (; nReps != 0; --nReps) {
        DWORD cbItem = 0;
        DWORD cbFixed = 0;
        switch (cp_memory_kind(dwType, &cbItem)) {
        case CP_FIXED:
            break;
        case CP_COUNTED_WCHARS:
        case CP_COUNTED_BYTES:
            cbItem = *reinterpret_cast<DWORD*>(pCur);
            if (cp_memory_kind(dwType, &cbFixed) == CP_COUNTED_WCHARS) cbItem *= 2u;
            pCur += 4;
            if (bVector) cb += 4;
            if (!bVector && (dwType == 0x1f || dwType == 0x1e)) ret = pCur;
            break;
        default:
            return nullptr;
        }
        cb += cbItem;
        pCur += cbItem;
    }
    if (pcb != nullptr) *pcb = cb;
    return ret;
}

// Symbol: ?Get@CProperty@@QEAAPEAXXZ
extern "C" void* MS_ABI impl__Get_CProperty__QEAAPEAXXZ(void* pThis) {
    // Retail RVA 0x261c30 (mfc140u): `xor edx,edx; jmp 0x261c40` -- Get(NULL).
    return impl__Get_CProperty__QEAAPEAXPEAK_Z(pThis, nullptr);
}

// Symbol: ?GetID@CProperty@@QEAAKXZ
extern "C" unsigned long MS_ABI impl__GetID_CProperty__QEAAKXZ(void* pThis) {
    // Retail RVA 0x261dd0 (mfc140u): `mov (%rcx),%eax` -- m_dwPropID.
    return CP(pThis)->m_dwPropID;
}

// Symbol: ?GetRawValue@CProperty@@QEAAPEAXXZ
extern "C" void* MS_ABI impl__GetRawValue_CProperty__QEAAPEAXXZ(void* pThis) {
    // Retail RVA 0x13d830 (mfc140u): `mov 0x8(%rcx),%rax` -- m_pValue.
    return CP(pThis)->m_pValue;
}

// Symbol: ?GetType@CProperty@@QEAAKXZ
extern "C" unsigned long MS_ABI impl__GetType_CProperty__QEAAKXZ(void* pThis) {
    // Retail RVA 0x261db0 (mfc140u; the u symbol map names that folded body
    // CPropertySet::GetOSVersion): `mov 0x4(%rcx),%eax` -- m_dwType.
    return CP(pThis)->m_dwType;
}

// Symbol: ?ReadFromStream@CProperty@@QEAAHPEAUIStream@@@Z
extern "C" int MS_ABI impl__ReadFromStream_CProperty__QEAAHPEAUIStream___Z(void* pThis, IStream* pIStream) {
    // Retail RVA 0x262020 (mfc140u).  IStream vtable slots used: 0x10 Release,
    // 0x18 Read, 0x28 Seek, 0x68 Clone.  Transcription:
    //   if (FAILED(pIStream->Read(&m_dwType, 4, &cbRead)) || cbRead != 4) return FALSE;
    //   dwType = m_dwType; nReps = 1;
    //   if (FAILED(pIStream->Clone(&pIStrm))) return FALSE;
    //   cb = 0;
    //   if (m_dwType & VT_VECTOR) {
    //     if (FAILED(pIStrm->Read(&nReps, 4, &cbRead)) || cbRead != 4) return FALSE;
    //     cb = 4; dwType &= ~VT_VECTOR;
    //   }
    //   cbRead = 0; cbItem = 0;
    //   for (n = nReps; n; --n) {           // sizing pass on the clone
    //     switch (dwType) (cp_stream_kind):
    //       fixed:   cbItem = size;
    //       counted: if (FAILED(pIStrm->Read(&cbItem, 4, &cbRead)) || cbRead != 4) return FALSE;
    //                pIStrm->Seek(-4, STREAM_SEEK_CUR, NULL);
    //                cb += cbRead;  if WCHAR-counted cbItem *= 2;
    //       invalid: pIStrm->Release(); return FALSE;
    //     pIStrm->Seek((ULONG)(cbRead + cbItem), STREAM_SEEK_CUR, NULL);
    //     cb += cbItem;
    //   }
    //   pIStrm->Release();
    //   if (dwType is a TCHAR-string type) {
    //     buf = malloc((int)cb);
    //     if (FAILED(pIStream->Read(buf, cb, &cbRead))) return FALSE;
    //     m_pValue = cp_convert_strings(buf, m_dwType, nReps, 2);  (0x261780)
    //     free(buf); return TRUE;
    //   }
    //   m_pValue = malloc((int)cb); if (!m_pValue) return FALSE;
    //   if (FAILED(pIStream->Read(m_pValue, cb, &cbRead)) || cbRead != cb) return FALSE;
    //   return TRUE;
    // Retail behaviour kept as-is: the clone is not released on the early
    // failure returns, the previous m_pValue is overwritten without being
    // freed, the Seek results are ignored, the string branch checks neither
    // malloc nor cbRead and returns TRUE even if the conversion returned NULL,
    // and for a vector the conversion starts at the element-count DWORD rather
    // than at the first element.  The fixed-size path reuses the last cbRead,
    // which is 0 (reset before the loop) because a vector's elements all share
    // one type.
    CP_Property* p = CP(pThis);
    ULONG cbRead = 0;
    HRESULT hr = pIStream->Read(&p->m_dwType, 4, &cbRead);
    if (FAILED(hr) || cbRead != 4) return FALSE;
    DWORD dwType = p->m_dwType;
    DWORD nReps = 1;
    IStream* pIStrm = nullptr;
    hr = pIStream->Clone(&pIStrm);
    if (FAILED(hr)) return FALSE;
    DWORD cb = 0;
    if (p->m_dwType & kVtVector) {
        hr = pIStrm->Read(&nReps, 4, &cbRead);
        if (FAILED(hr) || cbRead != 4) return FALSE;
        cb = 4;
        dwType &= ~kVtVector;
    }
    cbRead = 0;
    DWORD cbItem = 0;
    for (DWORD n = nReps; n != 0; --n) {
        DWORD cbFixed = 0;
        CP_Kind kind = cp_stream_kind(dwType, &cbFixed);
        if (kind == CP_FIXED) {
            cbItem = cbFixed;
        } else if (kind == CP_COUNTED_BYTES || kind == CP_COUNTED_WCHARS) {
            hr = pIStrm->Read(&cbItem, 4, &cbRead);
            if (FAILED(hr) || cbRead != 4) return FALSE;
            LARGE_INTEGER liBack;
            liBack.QuadPart = -4;
            pIStrm->Seek(liBack, STREAM_SEEK_CUR, nullptr);
            cb += cbRead;
            if (kind == CP_COUNTED_WCHARS) cbItem *= 2u;
        } else {
            pIStrm->Release();
            return FALSE;
        }
        LARGE_INTEGER li;
        li.QuadPart = static_cast<LONGLONG>(static_cast<ULONG>(cbRead + cbItem));
        pIStrm->Seek(li, STREAM_SEEK_CUR, nullptr);
        cb += cbItem;
    }
    pIStrm->Release();
    const size_t cbAlloc = static_cast<size_t>(static_cast<INT_PTR>(static_cast<int>(cb)));
    if (cp_is_tchar_string(dwType)) {
        BYTE* buf = static_cast<BYTE*>(std::malloc(cbAlloc));
        hr = pIStream->Read(buf, cb, &cbRead);
        if (FAILED(hr)) return FALSE;
        p->m_pValue = cp_convert_strings(buf, p->m_dwType, nReps, 2);
        std::free(buf);
        return TRUE;
    }
    p->m_pValue = std::malloc(cbAlloc);
    if (p->m_pValue == nullptr) return FALSE;
    hr = pIStream->Read(p->m_pValue, cb, &cbRead);
    if (FAILED(hr) || cbRead != cb) return FALSE;
    return TRUE;
}

// Symbol: ?Set@CProperty@@QEAAHKQEAXK@Z
extern "C" int MS_ABI impl__Set_CProperty__QEAAHKQEAXK_Z(void* pThis, unsigned long dwPropID, void* pValue, unsigned long dwType) {
    // Retail RVA 0x2619b0 (mfc140u): m_dwPropID = dwPropID; m_dwType = dwType;
    // `jmp 0x2619d0` -- tail-call Set(pValue).
    CP_Property* p = CP(pThis);
    p->m_dwPropID = dwPropID;
    p->m_dwType = dwType;
    return impl__Set_CProperty__QEAAHQEAX_Z(pThis, pValue);
}

// Symbol: ?Set@CProperty@@QEAAHQEAX@Z
extern "C" int MS_ABI impl__Set_CProperty__QEAAHQEAX_Z(void* pThis, void* pValue) {
    // Retail RVA 0x2619d0 (mfc140u).  Transcription:
    //   dwType = m_dwType;
    //   if (m_pValue) FreeValue();                                   (0x2623a0)
    //   if (pValue == NULL || m_dwType == 0) return TRUE;
    //   bConverted = FALSE; pSrc = pValue;
    //   if (m_dwType & VT_VECTOR) {
    //     nReps = *(DWORD*)pValue; cb = 4; dwType &= ~VT_VECTOR; pCur = pValue + 4;
    //   } else {
    //     switch (dwType):
    //       VT_LPSTR 0x1e:                       pSrc = counted ANSI copy (inline)
    //       VT_BSTR 8, VT_LPWSTR 0x1f, 0x42..0x45: pSrc = counted wide copy (0x2616dc)
    //     bConverted = TRUE for those; if the copy failed, return FALSE;
    //     nReps = 1; cb = 0; pCur = pSrc;
    //   }
    //   per element (switch at 0x261b03, cp_memory_kind):
    //     fixed: cbItem = size;  counted: cbItem = count (x2 if WCHAR-counted),
    //     pCur += 4, cb += 4;  invalid: free pSrc if converted, return FALSE;
    //     cb += cbItem; pCur += cbItem;
    //   m_pValue = malloc((int)cb);
    //   ok = (m_pValue != NULL); if (ok && cb) memcpy(m_pValue, pSrc, cb);
    //   if (bConverted && pSrc) free(pSrc);
    //   return ok;
    // The memcpy's NULL-source branch (memset + _errno = EINVAL +
    // _invalid_parameter_noinfo at 0x261c10) is unreachable -- pSrc is non-NULL
    // there -- and is not reproduced.
    CP_Property* p = CP(pThis);
    DWORD dwType = p->m_dwType;
    if (p->m_pValue != nullptr) impl__FreeValue_CProperty__AEAAXXZ(pThis);
    if (pValue == nullptr || p->m_dwType == 0) return TRUE;

    BOOL bConverted = FALSE;
    BYTE* pSrc = static_cast<BYTE*>(pValue);
    DWORD nReps;
    DWORD cb;
    BYTE* pCur;
    if (p->m_dwType & kVtVector) {
        nReps = *reinterpret_cast<DWORD*>(pSrc);
        cb = 4;
        dwType &= ~kVtVector;
        pCur = pSrc + 4;
    } else {
        switch (dwType) {
        case 0x1e:
            pSrc = cp_counted_ansi_copy(static_cast<const char*>(pValue));
            bConverted = TRUE;
            break;
        case 0x08: case 0x1f: case 0x42: case 0x43: case 0x44: case 0x45:
            pSrc = cp_counted_wide_copy(static_cast<const WCHAR*>(pValue));
            bConverted = TRUE;
            break;
        default:
            break;
        }
        if (bConverted && pSrc == nullptr) return FALSE;
        nReps = 1;
        cb = 0;
        pCur = pSrc;
    }
    for (; nReps != 0; --nReps) {
        DWORD cbItem = 0;
        switch (cp_memory_kind(dwType, &cbItem)) {
        case CP_FIXED:
            break;
        case CP_COUNTED_WCHARS:
            cbItem = *reinterpret_cast<DWORD*>(pCur) * 2u;
            pCur += 4;
            cb += 4;
            break;
        case CP_COUNTED_BYTES:
            cbItem = *reinterpret_cast<DWORD*>(pCur);
            pCur += 4;
            cb += 4;
            break;
        default:
            if (bConverted && pSrc != nullptr) std::free(pSrc);
            return FALSE;
        }
        cb += cbItem;
        pCur += cbItem;
    }
    p->m_pValue = std::malloc(static_cast<size_t>(static_cast<INT_PTR>(static_cast<int>(cb))));
    BOOL ok = TRUE;
    if (p->m_pValue == nullptr)
        ok = FALSE;
    else if (cb != 0)
        std::memcpy(p->m_pValue, pSrc, cb);
    if (bConverted && pSrc != nullptr) std::free(pSrc);
    return ok;
}

// Symbol: ?Set@CProperty@@QEAAHQEAXK@Z
extern "C" int MS_ABI impl__Set_CProperty__QEAAHQEAXK_Z(void* pThis, void* pValue, unsigned long dwType) {
    // Retail RVA 0x2619c0 (mfc140u): m_dwType = dwType; `jmp 0x2619d0` --
    // tail-call Set(pValue).
    CP(pThis)->m_dwType = dwType;
    return impl__Set_CProperty__QEAAHQEAX_Z(pThis, pValue);
}

// Symbol: ?SetID@CProperty@@QEAAXK@Z
extern "C" void MS_ABI impl__SetID_CProperty__QEAAXK_Z(void* pThis, unsigned long dwPropID) {
    // Retail RVA 0x261de0 (mfc140u): `mov %edx,(%rcx)` -- m_dwPropID = dwPropID.
    CP(pThis)->m_dwPropID = dwPropID;
}

// Symbol: ?SetType@CProperty@@QEAAXK@Z
extern "C" void MS_ABI impl__SetType_CProperty__QEAAXK_Z(void* pThis, unsigned long dwType) {
    // Retail RVA 0x261dc0 (mfc140u): `mov %edx,0x4(%rcx)` -- m_dwType = dwType.
    CP(pThis)->m_dwType = dwType;
}

// Symbol: ?WriteToStream@CProperty@@QEAAHPEAUIStream@@@Z
extern "C" int MS_ABI impl__WriteToStream_CProperty__QEAAHPEAUIStream___Z(void* pThis, IStream* pIStream) {
    // Retail RVA 0x261df0 (mfc140u).  IStream vtable slot used: 0x20 Write.
    // Transcription:
    //   pValue = m_pValue; dwType = m_dwType; BYTE zero = 0;
    //   vector: nReps = *(DWORD*)pValue; cb = 4; pCur = pValue + 4; strip VT_VECTOR
    //   else:   nReps = 1; pCur = pValue; cb = 0
    //   if (dwType is a TCHAR-string type) {
    //     pValue = cp_convert_strings(pCur, m_dwType, nReps, 1);   (0x261780)
    //     if (m_dwType & VT_VECTOR) pCur = pValue + 4;
    //     -- a non-vector pCur keeps pointing at the ORIGINAL wide value, so
    //        retail sizes the write from the wide count, not from the
    //        converted byte count (the same is done here).
    //   }
    //   per element (switch at 0x261e8b, cp_stream_kind):
    //     fixed: size; WCHAR-counted: count*2 + 4; byte-counted: count + 4;
    //     invalid: return FALSE;
    //     cb += cbItem; pCur += cbItem;
    //   ok = FALSE;
    //   pIStream->Write(&m_dwType, 4, &cbWritten); if (cbWritten != 4) goto out;
    //   pIStream->Write(pValue, cb, &cbWritten);   if (cbWritten != cb) goto out;
    //   for (nPad = ((cb + 3) & ~3) - cb; nPad; --nPad) {
    //     pIStream->Write(&zero, 1, &cbWritten);   if (cbWritten != 1) goto out;
    //   }
    //   ok = TRUE;
    // out:
    //   if (pValue != m_pValue) free(pValue);
    //   return ok;
    // The Write HRESULTs are ignored; only the byte counts are checked, as in
    // retail.  Only m_dwType and the value are written, never m_dwPropID.
    // Deviation: retail never initialises cbWritten itself.  Its stack slot
    // (0x80(%rsp), the r8 home slot) is shared with the sizing loop, which
    // stores each cbItem there (`mov %ecx,0x80(%rsp)` at 0x261f35), so before
    // the first Write it holds the last element's cbItem, or the caller's
    // home-slot garbage when nReps == 0.  It is zeroed here, so a stream that
    // fails without setting it reads as a short write.  Retail does not
    // NULL-check the conversion result before walking it; neither does this
    // body.  On an invalid element type the converted buffer is leaked, as in
    // retail (0x26201b returns without freeing it).
    CP_Property* p = CP(pThis);
    BYTE* pValue = static_cast<BYTE*>(p->m_pValue);
    DWORD dwType = p->m_dwType;
    BYTE zero = 0;
    DWORD nReps;
    DWORD cb;
    BYTE* pCur;
    if (dwType & kVtVector) {
        nReps = *reinterpret_cast<DWORD*>(pValue);
        cb = 4;
        pCur = pValue + 4;
        dwType &= ~kVtVector;
    } else {
        nReps = 1;
        pCur = pValue;
        cb = 0;
    }
    if (cp_is_tchar_string(dwType)) {
        pValue = cp_convert_strings(pCur, p->m_dwType, nReps, 1);
        if (p->m_dwType & kVtVector) pCur = pValue + 4;
    }
    for (; nReps != 0; --nReps) {
        DWORD cbItem = 0;
        switch (cp_stream_kind(dwType, &cbItem)) {
        case CP_FIXED:
            break;
        case CP_COUNTED_WCHARS:
            cbItem = *reinterpret_cast<DWORD*>(pCur) * 2u + 4u;
            break;
        case CP_COUNTED_BYTES:
            cbItem = *reinterpret_cast<DWORD*>(pCur) + 4u;
            break;
        default:
            return FALSE;
        }
        cb += cbItem;
        pCur += cbItem;
    }

    BOOL ok = FALSE;
    ULONG cbWritten = 0;
    pIStream->Write(&p->m_dwType, 4, &cbWritten);
    if (cbWritten == 4) {
        pIStream->Write(pValue, cb, &cbWritten);
        if (cbWritten == cb) {
            DWORD nPad = ((cb + 3u) & ~3u) - cb;
            for (; nPad != 0; --nPad) {
                pIStream->Write(&zero, 1, &cbWritten);
                if (cbWritten != 1) break;
            }
            if (nPad == 0) ok = TRUE;
        }
    }
    if (pValue != p->m_pValue) std::free(pValue);
    return ok;
}
