#pragma once
// Shared internals of the former global_cdumpcontext.cpp translation unit.
// Definitions live in detail/CDumpContextSupport.cpp, so file-scope state keeps a
// single instance across the classes this unit was split into.
namespace openmfc { namespace detail { namespace cdumpcontext {} } }
using namespace openmfc::detail::cdumpcontext;
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

// Polymorphic dispatch on the CFile-family target via its MSVC-layout vtable
// (global_file_dispatch.cpp); mingw virtual syntax would mis-index the vtable.
extern "C" {

void MS_ABI OpenMFC_File_Write(CFile*, const void*, unsigned int);
void MS_ABI OpenMFC_File_Flush(CFile*);
}

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


// ---------------------------------------------------------------------------
// Member implementations
// ---------------------------------------------------------------------------

























// ===========================================================================
// Exported MS-ABI thunks (this in RCX). Each delegates to the member above.
// ===========================================================================






























