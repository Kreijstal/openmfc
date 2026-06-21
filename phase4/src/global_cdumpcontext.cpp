// CDumpContext implementation for OpenMFC
//
// CDumpContext is a standalone diagnostic class (NO base class, NO vtable).
// MSVC x64 layout (sizeof == 16, align 8):
//   offset 0: int     m_nDepth   (4 bytes)
//   offset 4: padding           (4 bytes)
//   offset 8: CFile*  m_pFile    (8 bytes)
//
// OutputString() writes the wide-char text via m_pFile->Write() (CFile::Write
// is virtual, afx.h:445) or OutputDebugStringW when m_pFile == nullptr.
// operator<< / DumpAsHex format their argument into a wide buffer and call
// OutputString, returning *this. Flush() forwards to m_pFile->Flush().

#define OPENMFC_APPCORE_IMPL
// CDumpContext only needs CObject and CFile, both declared in afx.h. Including
// the lighter afx.h (instead of the full afxmfc.h) avoids dragging in CWnd /
// CCmdTarget vtables that have no out-of-line defs in a standalone build.
#include "openmfc/afx.h"

#include <cstdint>
#include <cstdio>
#include <cstring>
#include <cwchar>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

// ---------------------------------------------------------------------------
// Class definition (not present in include/; defined here with the exact
// MSVC-compatible layout from the authoritative research).
// ---------------------------------------------------------------------------
class CDumpContext {
public:
    explicit CDumpContext(CFile* pFile);

    CDumpContext& operator<<(LPCWSTR lpsz);
    CDumpContext& operator<<(LPCSTR lpsz);
    CDumpContext& operator<<(const void* lp);
    CDumpContext& operator<<(int n);
    CDumpContext& operator<<(unsigned int n);
    CDumpContext& operator<<(long n);
    CDumpContext& operator<<(unsigned long n);
    CDumpContext& operator<<(__int64 n);
    CDumpContext& operator<<(unsigned __int64 n);
    CDumpContext& operator<<(unsigned char ch);
    CDumpContext& operator<<(unsigned short w);
    CDumpContext& operator<<(const CObject* pOb);
    CDumpContext& operator<<(const CObject& ob);

    CDumpContext& DumpAsHex(int n);
    CDumpContext& DumpAsHex(unsigned int n);
    CDumpContext& DumpAsHex(long n);
    CDumpContext& DumpAsHex(unsigned long n);
    CDumpContext& DumpAsHex(__int64 n);
    CDumpContext& DumpAsHex(unsigned __int64 n);
    CDumpContext& DumpAsHex(unsigned char ch);
    CDumpContext& DumpAsHex(unsigned short w);

    void Flush();
    void HexDump(LPCWSTR lpszLine, BYTE* pby, int nBytes, int nWidth);

    // Public storage for cross-ABI thunk access (mirrors CFile pattern).
    int    m_nDepth;
    CFile* m_pFile;

protected:
    void OutputString(LPCWSTR lpsz);
};

static_assert(sizeof(CDumpContext) == 16, "CDumpContext must be 16 bytes");

// ---------------------------------------------------------------------------
// Member implementations
// ---------------------------------------------------------------------------
CDumpContext::CDumpContext(CFile* pFile) {
    m_nDepth = 0;
    m_pFile = pFile;
}

void CDumpContext::OutputString(LPCWSTR lpsz) {
    if (lpsz == nullptr) {
        return;
    }
    if (m_pFile != nullptr) {
        UINT nBytes = static_cast<UINT>(wcslen(lpsz) * sizeof(wchar_t));
        m_pFile->Write(lpsz, nBytes);
    } else {
        OutputDebugStringW(lpsz);
    }
}

CDumpContext& CDumpContext::operator<<(LPCWSTR lpsz) {
    if (lpsz == nullptr) {
        OutputString(L"(NULL)");
    } else {
        OutputString(lpsz);
    }
    return *this;
}

CDumpContext& CDumpContext::operator<<(LPCSTR lpsz) {
    if (lpsz == nullptr) {
        OutputString(L"(NULL)");
        return *this;
    }
    // Widen ASCII/ANSI to wchar_t.
    wchar_t buf[512];
    size_t n = 0;
    for (; lpsz[n] != '\0' && n < (sizeof(buf) / sizeof(buf[0])) - 1; ++n) {
        buf[n] = static_cast<wchar_t>(static_cast<unsigned char>(lpsz[n]));
    }
    buf[n] = L'\0';
    OutputString(buf);
    return *this;
}

CDumpContext& CDumpContext::operator<<(const void* lp) {
    wchar_t buf[32];
    swprintf(buf, 32, L"$%llX", static_cast<unsigned long long>(reinterpret_cast<uintptr_t>(lp)));
    OutputString(buf);
    return *this;
}

CDumpContext& CDumpContext::operator<<(int n) {
    wchar_t buf[32];
    swprintf(buf, 32, L"%d", n);
    OutputString(buf);
    return *this;
}

CDumpContext& CDumpContext::operator<<(unsigned int n) {
    wchar_t buf[32];
    swprintf(buf, 32, L"%u", n);
    OutputString(buf);
    return *this;
}

CDumpContext& CDumpContext::operator<<(long n) {
    wchar_t buf[32];
    swprintf(buf, 32, L"%ld", n);
    OutputString(buf);
    return *this;
}

CDumpContext& CDumpContext::operator<<(unsigned long n) {
    wchar_t buf[32];
    swprintf(buf, 32, L"%lu", n);
    OutputString(buf);
    return *this;
}

CDumpContext& CDumpContext::operator<<(__int64 n) {
    wchar_t buf[32];
    swprintf(buf, 32, L"%lld", static_cast<long long>(n));
    OutputString(buf);
    return *this;
}

CDumpContext& CDumpContext::operator<<(unsigned __int64 n) {
    wchar_t buf[32];
    swprintf(buf, 32, L"%llu", static_cast<unsigned long long>(n));
    OutputString(buf);
    return *this;
}

CDumpContext& CDumpContext::operator<<(unsigned char ch) {
    wchar_t buf[16];
    swprintf(buf, 16, L"%u", static_cast<unsigned>(ch));
    OutputString(buf);
    return *this;
}

CDumpContext& CDumpContext::operator<<(unsigned short w) {
    wchar_t buf[16];
    swprintf(buf, 16, L"%u", static_cast<unsigned>(w));
    OutputString(buf);
    return *this;
}

CDumpContext& CDumpContext::operator<<(const CObject* pOb) {
    if (pOb == nullptr) {
        OutputString(L"(NULL)");
    } else {
        pOb->Dump();  // virtual call into CObject::Dump
    }
    return *this;
}

CDumpContext& CDumpContext::operator<<(const CObject& ob) {
    ob.Dump();  // virtual call into CObject::Dump
    return *this;
}

CDumpContext& CDumpContext::DumpAsHex(int n) {
    wchar_t buf[32];
    swprintf(buf, 32, L"0x%X", static_cast<unsigned>(n));
    OutputString(buf);
    return *this;
}

CDumpContext& CDumpContext::DumpAsHex(unsigned int n) {
    wchar_t buf[32];
    swprintf(buf, 32, L"0x%X", n);
    OutputString(buf);
    return *this;
}

CDumpContext& CDumpContext::DumpAsHex(long n) {
    wchar_t buf[32];
    swprintf(buf, 32, L"0x%lX", static_cast<unsigned long>(n));
    OutputString(buf);
    return *this;
}

CDumpContext& CDumpContext::DumpAsHex(unsigned long n) {
    wchar_t buf[32];
    swprintf(buf, 32, L"0x%lX", n);
    OutputString(buf);
    return *this;
}

CDumpContext& CDumpContext::DumpAsHex(__int64 n) {
    wchar_t buf[32];
    swprintf(buf, 32, L"0x%llX", static_cast<unsigned long long>(n));
    OutputString(buf);
    return *this;
}

CDumpContext& CDumpContext::DumpAsHex(unsigned __int64 n) {
    wchar_t buf[32];
    swprintf(buf, 32, L"0x%llX", n);
    OutputString(buf);
    return *this;
}

CDumpContext& CDumpContext::DumpAsHex(unsigned char ch) {
    wchar_t buf[16];
    swprintf(buf, 16, L"0x%X", static_cast<unsigned>(ch));
    OutputString(buf);
    return *this;
}

CDumpContext& CDumpContext::DumpAsHex(unsigned short w) {
    wchar_t buf[16];
    swprintf(buf, 16, L"0x%X", static_cast<unsigned>(w));
    OutputString(buf);
    return *this;
}

void CDumpContext::Flush() {
    if (m_pFile != nullptr) {
        m_pFile->Flush();
    }
}

void CDumpContext::HexDump(LPCWSTR lpszLine, BYTE* pby, int nBytes, int nWidth) {
    if (lpszLine != nullptr) {
        OutputString(lpszLine);
    }
    if (pby == nullptr || nBytes <= 0) {
        return;
    }
    if (nWidth <= 0) {
        nWidth = 16;
    }
    int col = 0;
    for (int i = 0; i < nBytes; ++i) {
        wchar_t buf[8];
        swprintf(buf, 8, L"%02X ", static_cast<unsigned>(pby[i]));
        OutputString(buf);
        if (++col >= nWidth && i + 1 < nBytes) {
            OutputString(L"\r\n");
            col = 0;
        }
    }
}

// ===========================================================================
// Exported MS-ABI thunks (this in RCX). Each delegates to the member above.
// ===========================================================================

// Symbol: ??0CDumpContext@@QEAA@PEAVCFile@@@Z
extern "C" void MS_ABI impl___0CDumpContext__QEAA_PEAVCFile___Z(CDumpContext* pThis, CFile* pFile) {
    if (!pThis) return;
    pThis->m_nDepth = 0;
    pThis->m_pFile = pFile;
}

// Symbol: ??6CDumpContext@@QEAAAEAV0@_J@Z
extern "C" CDumpContext* MS_ABI impl___6CDumpContext__QEAAAEAV0__J_Z(CDumpContext* pThis, __int64 n) {
    if (!pThis) return pThis;
    return &(*pThis << n);
}

// Symbol: ??6CDumpContext@@QEAAAEAV0@_K@Z
extern "C" CDumpContext* MS_ABI impl___6CDumpContext__QEAAAEAV0__K_Z(CDumpContext* pThis, unsigned __int64 n) {
    if (!pThis) return pThis;
    return &(*pThis << n);
}

// Symbol: ??6CDumpContext@@QEAAAEAV0@AEBVCObject@@@Z
extern "C" CDumpContext* MS_ABI impl___6CDumpContext__QEAAAEAV0_AEBVCObject___Z(CDumpContext* pThis, const CObject* pOb) {
    if (!pThis) return pThis;
    if (!pOb) return pThis;
    return &(*pThis << *pOb);
}

// Symbol: ??6CDumpContext@@QEAAAEAV0@E@Z
extern "C" CDumpContext* MS_ABI impl___6CDumpContext__QEAAAEAV0_E_Z(CDumpContext* pThis, unsigned char ch) {
    if (!pThis) return pThis;
    return &(*pThis << ch);
}

// Symbol: ??6CDumpContext@@QEAAAEAV0@G@Z
extern "C" CDumpContext* MS_ABI impl___6CDumpContext__QEAAAEAV0_G_Z(CDumpContext* pThis, unsigned short w) {
    if (!pThis) return pThis;
    return &(*pThis << w);
}

// Symbol: ??6CDumpContext@@QEAAAEAV0@H@Z
extern "C" CDumpContext* MS_ABI impl___6CDumpContext__QEAAAEAV0_H_Z(CDumpContext* pThis, int n) {
    if (!pThis) return pThis;
    return &(*pThis << n);
}

// Symbol: ??6CDumpContext@@QEAAAEAV0@I@Z
extern "C" CDumpContext* MS_ABI impl___6CDumpContext__QEAAAEAV0_I_Z(CDumpContext* pThis, unsigned int n) {
    if (!pThis) return pThis;
    return &(*pThis << n);
}

// Symbol: ??6CDumpContext@@QEAAAEAV0@J@Z
extern "C" CDumpContext* MS_ABI impl___6CDumpContext__QEAAAEAV0_J_Z(CDumpContext* pThis, long n) {
    if (!pThis) return pThis;
    return &(*pThis << n);
}

// Symbol: ??6CDumpContext@@QEAAAEAV0@K@Z
extern "C" CDumpContext* MS_ABI impl___6CDumpContext__QEAAAEAV0_K_Z(CDumpContext* pThis, unsigned long n) {
    if (!pThis) return pThis;
    return &(*pThis << n);
}

// Symbol: ??6CDumpContext@@QEAAAEAV0@PEAUHACCEL__@@@Z
extern "C" CDumpContext* MS_ABI impl___6CDumpContext__QEAAAEAV0_PEAUHACCEL_____Z(CDumpContext* pThis, void* h) {
    if (!pThis) return pThis;
    return &(*pThis << static_cast<const void*>(h));
}

// Symbol: ??6CDumpContext@@QEAAAEAV0@PEAUHDC__@@@Z
extern "C" CDumpContext* MS_ABI impl___6CDumpContext__QEAAAEAV0_PEAUHDC_____Z(CDumpContext* pThis, void* h) {
    if (!pThis) return pThis;
    return &(*pThis << static_cast<const void*>(h));
}

// Symbol: ??6CDumpContext@@QEAAAEAV0@PEAUHFONT__@@@Z
extern "C" CDumpContext* MS_ABI impl___6CDumpContext__QEAAAEAV0_PEAUHFONT_____Z(CDumpContext* pThis, void* h) {
    if (!pThis) return pThis;
    return &(*pThis << static_cast<const void*>(h));
}

// Symbol: ??6CDumpContext@@QEAAAEAV0@PEAUHMENU__@@@Z
extern "C" CDumpContext* MS_ABI impl___6CDumpContext__QEAAAEAV0_PEAUHMENU_____Z(CDumpContext* pThis, void* h) {
    if (!pThis) return pThis;
    return &(*pThis << static_cast<const void*>(h));
}

// Symbol: ??6CDumpContext@@QEAAAEAV0@PEAUHWND__@@@Z
extern "C" CDumpContext* MS_ABI impl___6CDumpContext__QEAAAEAV0_PEAUHWND_____Z(CDumpContext* pThis, void* h) {
    if (!pThis) return pThis;
    return &(*pThis << static_cast<const void*>(h));
}

// Symbol: ??6CDumpContext@@QEAAAEAV0@PEB_W@Z
extern "C" CDumpContext* MS_ABI impl___6CDumpContext__QEAAAEAV0_PEB_W_Z(CDumpContext* pThis, const wchar_t* lpsz) {
    if (!pThis) return pThis;
    return &(*pThis << lpsz);
}

// Symbol: ??6CDumpContext@@QEAAAEAV0@PEBD@Z
extern "C" CDumpContext* MS_ABI impl___6CDumpContext__QEAAAEAV0_PEBD_Z(CDumpContext* pThis, const char* lpsz) {
    if (!pThis) return pThis;
    return &(*pThis << lpsz);
}

// Symbol: ??6CDumpContext@@QEAAAEAV0@PEBVCObject@@@Z
extern "C" CDumpContext* MS_ABI impl___6CDumpContext__QEAAAEAV0_PEBVCObject___Z(CDumpContext* pThis, const CObject* pOb) {
    if (!pThis) return pThis;
    return &(*pThis << pOb);
}

// Symbol: ??6CDumpContext@@QEAAAEAV0@PEBX@Z
extern "C" CDumpContext* MS_ABI impl___6CDumpContext__QEAAAEAV0_PEBX_Z(CDumpContext* pThis, const void* lp) {
    if (!pThis) return pThis;
    return &(*pThis << lp);
}

// Symbol: ?DumpAsHex@CDumpContext@@QEAAAEAV1@_J@Z
extern "C" CDumpContext* MS_ABI impl__DumpAsHex_CDumpContext__QEAAAEAV1__J_Z(CDumpContext* pThis, __int64 n) {
    if (!pThis) return pThis;
    return &pThis->DumpAsHex(n);
}

// Symbol: ?DumpAsHex@CDumpContext@@QEAAAEAV1@_K@Z
extern "C" CDumpContext* MS_ABI impl__DumpAsHex_CDumpContext__QEAAAEAV1__K_Z(CDumpContext* pThis, unsigned __int64 n) {
    if (!pThis) return pThis;
    return &pThis->DumpAsHex(n);
}

// Symbol: ?DumpAsHex@CDumpContext@@QEAAAEAV1@E@Z
extern "C" CDumpContext* MS_ABI impl__DumpAsHex_CDumpContext__QEAAAEAV1_E_Z(CDumpContext* pThis, unsigned char ch) {
    if (!pThis) return pThis;
    return &pThis->DumpAsHex(ch);
}

// Symbol: ?DumpAsHex@CDumpContext@@QEAAAEAV1@G@Z
extern "C" CDumpContext* MS_ABI impl__DumpAsHex_CDumpContext__QEAAAEAV1_G_Z(CDumpContext* pThis, unsigned short w) {
    if (!pThis) return pThis;
    return &pThis->DumpAsHex(w);
}

// Symbol: ?DumpAsHex@CDumpContext@@QEAAAEAV1@H@Z
extern "C" CDumpContext* MS_ABI impl__DumpAsHex_CDumpContext__QEAAAEAV1_H_Z(CDumpContext* pThis, int n) {
    if (!pThis) return pThis;
    return &pThis->DumpAsHex(n);
}

// Symbol: ?DumpAsHex@CDumpContext@@QEAAAEAV1@I@Z
extern "C" CDumpContext* MS_ABI impl__DumpAsHex_CDumpContext__QEAAAEAV1_I_Z(CDumpContext* pThis, unsigned int n) {
    if (!pThis) return pThis;
    return &pThis->DumpAsHex(n);
}

// Symbol: ?DumpAsHex@CDumpContext@@QEAAAEAV1@J@Z
extern "C" CDumpContext* MS_ABI impl__DumpAsHex_CDumpContext__QEAAAEAV1_J_Z(CDumpContext* pThis, long n) {
    if (!pThis) return pThis;
    return &pThis->DumpAsHex(n);
}

// Symbol: ?DumpAsHex@CDumpContext@@QEAAAEAV1@K@Z
extern "C" CDumpContext* MS_ABI impl__DumpAsHex_CDumpContext__QEAAAEAV1_K_Z(CDumpContext* pThis, unsigned long n) {
    if (!pThis) return pThis;
    return &pThis->DumpAsHex(n);
}

// Symbol: ?Flush@CDumpContext@@QEAAXXZ
extern "C" void MS_ABI impl__Flush_CDumpContext__QEAAXXZ(CDumpContext* pThis) {
    if (!pThis) return;
    pThis->Flush();
}

// Symbol: ?HexDump@CDumpContext@@QEAAXPEB_WPEAEHH@Z
extern "C" void MS_ABI impl__HexDump_CDumpContext__QEAAXPEB_WPEAEHH_Z(CDumpContext* pThis, const wchar_t* lpszLine, unsigned char* pby, int nBytes, int nWidth) {
    if (!pThis) return;
    pThis->HexDump(lpszLine, pby, nBytes, nWidth);
}

// Symbol: ?OutputString@CDumpContext@@IEAAXPEB_W@Z
extern "C" void MS_ABI impl__OutputString_CDumpContext__IEAAXPEB_W_Z(CDumpContext* pThis, const wchar_t* lpsz) {
    if (!pThis) return;
    // OutputString is protected; invoke via operator<< which delegates to it.
    if (lpsz) {
        *pThis << lpsz;
    }
}
