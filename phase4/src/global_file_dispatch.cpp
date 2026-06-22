// OpenMFC: internal polymorphic dispatch helpers for CFile-family objects.
//
// CFile-family objects carry a MSVC-layout vtable (hand-authored; see
// global_*_msvtbl.cpp) so drop-in MSVC clients dispatch virtuals correctly. But
// the DLL is mingw-compiled (Itanium ABI), so DLL-internal code must NOT use C++
// virtual syntax (pFile->Read()) on these objects — mingw would index the vtable
// with Itanium slot numbers and land on the wrong entry. Internal code calls these
// helpers instead, which dispatch through the fixed MSVC slot index. The slot
// numbers are uniform across the whole CFile family (verified via
// cl.exe /d1reportSingleClassLayout): 5 GetPosition, 13 Seek, 14 SetLength,
// 15 GetLength, 16 Read, 17 Write, 21 Flush, 22 Close. This is also correct for
// CFile objects supplied by a real-MSVC client (their vtable is MSVC-layout too).

#include "openmfc/afx.h"

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

namespace {
inline void* const* vtbl(const void* p) { return *reinterpret_cast<void* const* const*>(p); }
}

extern "C" {

unsigned int MS_ABI OpenMFC_File_Read(CFile* p, void* buf, unsigned int n) {
    typedef unsigned int (MS_ABI *Fn)(CFile*, void*, unsigned int);
    return ((Fn)vtbl(p)[16])(p, buf, n);
}
void MS_ABI OpenMFC_File_Write(CFile* p, const void* buf, unsigned int n) {
    typedef void (MS_ABI *Fn)(CFile*, const void*, unsigned int);
    ((Fn)vtbl(p)[17])(p, buf, n);
}
unsigned long long MS_ABI OpenMFC_File_Seek(CFile* p, long long off, unsigned int from) {
    typedef unsigned long long (MS_ABI *Fn)(CFile*, long long, unsigned int);
    return ((Fn)vtbl(p)[13])(p, off, from);
}
void MS_ABI OpenMFC_File_SetLength(CFile* p, unsigned long long n) {
    typedef void (MS_ABI *Fn)(CFile*, unsigned long long);
    ((Fn)vtbl(p)[14])(p, n);
}
unsigned long long MS_ABI OpenMFC_File_GetLength(CFile* p) {
    typedef unsigned long long (MS_ABI *Fn)(CFile*);
    return ((Fn)vtbl(p)[15])(p);
}
unsigned long long MS_ABI OpenMFC_File_GetPosition(CFile* p) {
    typedef unsigned long long (MS_ABI *Fn)(CFile*);
    return ((Fn)vtbl(p)[5])(p);
}
void MS_ABI OpenMFC_File_Flush(CFile* p) {
    typedef void (MS_ABI *Fn)(CFile*);
    ((Fn)vtbl(p)[21])(p);
}
void MS_ABI OpenMFC_File_Close(CFile* p) {
    typedef void (MS_ABI *Fn)(CFile*);
    ((Fn)vtbl(p)[22])(p);
}

} // extern "C"
