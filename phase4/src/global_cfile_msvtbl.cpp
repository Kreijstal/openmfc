// OpenMFC SPIKE: hand-authored MSVC-layout vtable for CFile.
//
// The DLL is built with mingw-gcc (Itanium C++ ABI), whose vtable layout differs
// from MSVC's — most importantly Itanium emits TWO destructor slots vs MSVC's one,
// so every virtual past the destructor mis-dispatches when a real MSVC client calls
// through the object vtable (cobject_impl.cpp documents that only vtable[0] is safe).
//
// This file provides an explicit MSVC-slot-ordered vtable for CFile and patches the
// object's vptr to it via OpenMFC_PatchCFileVtable, so a MSVC client's virtual calls
// land on the right method. Each slot is a small MS-ABI wrapper that calls the real
// member NON-virtually (qualified) to avoid re-dispatching back through this same
// vtable (which would recurse). Slots without a real CFile member are minimal stubs.
//
// MSVC vtable order = CObject's 5 slots then CFile's 19 new virtuals in declaration
// order:
//   0 GetRuntimeClass  1 ~dtor(vector-deleting)  2 Serialize  3 AssertValid  4 Dump
//   5 GetPosition  6 GetFileName  7 GetFileTitle  8 GetFilePath  9 SetFilePath
//   10 Open(3)  11 Open(4)  12 Duplicate  13 Seek  14 SetLength  15 GetLength
//   16 Read  17 Write  18 LockRange  19 UnlockRange  20 Abort  21 Flush  22 Close
//   23 GetBufferPtr
//
// Named global_*; build_phase4.sh's shard glob compiles global_*.

#include "openmfc/afxwin.h"
#include <cstdlib>
#include <new>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

namespace {

// --- slot wrappers: qualified (non-virtual) member calls for real members, stubs otherwise ---
CRuntimeClass* MS_ABI v_GetRuntimeClass(CFile*)              { return &CFile::classCFile; }
void*          MS_ABI v_dtor(CFile* p, unsigned int flags)   { p->CFile::~CFile(); if (flags & 1) ::operator delete(p); return p; }
void           MS_ABI v_Serialize(CFile*, void*)             {}
void           MS_ABI v_AssertValid(const CFile*)            {}
void           MS_ABI v_Dump(const CFile*, void*)            {}
unsigned long long MS_ABI v_GetPosition(const CFile* p)      { return const_cast<CFile*>(p)->CFile::Seek(0, 1 /*current*/); }
// CString is returned by value via sret. For these member-returning-CString slots
// the proven repo convention (see impl__GetFileName_CFile thunk) is this=RCX,
// sret=RDX, void return — construct the CString into the caller's buffer. Stubbing
// (or swapping the params) crashes the client.
void           MS_ABI v_GetFileName(CFile* p, CString* pRet)  { new(pRet) CString(p->CFile::GetFileName());  }
void           MS_ABI v_GetFileTitle(CFile* p, CString* pRet) { new(pRet) CString(p->CFile::GetFileTitle()); }
void           MS_ABI v_GetFilePath(CFile* p, CString* pRet)  { new(pRet) CString(p->CFile::GetFilePath());  }
void           MS_ABI v_SetFilePath(CFile* p, const wchar_t* n) { p->CFile::SetFilePath(n); }
int            MS_ABI v_Open3(CFile*, const wchar_t*, unsigned, void*) { return 0; }
int            MS_ABI v_Open4(CFile*, const wchar_t*, unsigned, void*, void*) { return 0; }
void*          MS_ABI v_Duplicate(const CFile*)              { return nullptr; }
unsigned long long MS_ABI v_Seek(CFile* p, long long off, unsigned from) { return p->CFile::Seek(off, from); }
void           MS_ABI v_SetLength(CFile* p, unsigned long long n)        { p->CFile::SetLength(n); }
unsigned long long MS_ABI v_GetLength(const CFile* p)        { return const_cast<CFile*>(p)->CFile::GetLength(); }
unsigned int   MS_ABI v_Read(CFile* p, void* b, unsigned int n)         { return p->CFile::Read(b, n); }
void           MS_ABI v_Write(CFile* p, const void* b, unsigned int n)  { p->CFile::Write(b, n); }
void           MS_ABI v_LockRange(CFile*, unsigned long long, unsigned long long)   {}
void           MS_ABI v_UnlockRange(CFile*, unsigned long long, unsigned long long) {}
void           MS_ABI v_Abort(CFile*)                        {}
void           MS_ABI v_Flush(CFile* p)                      { p->CFile::Flush(); }
void           MS_ABI v_Close(CFile* p)                      { p->CFile::Close(); }
unsigned int   MS_ABI v_GetBufferPtr(CFile*, unsigned int, unsigned int, void**, void**) { return 0; }

// The 24-slot MSVC-layout vtable. vptr points at &slot[0].
void* const g_CFile_msvtbl[24] = {
    (void*)v_GetRuntimeClass, (void*)v_dtor,        (void*)v_Serialize,   (void*)v_AssertValid,
    (void*)v_Dump,            (void*)v_GetPosition, (void*)v_GetFileName, (void*)v_GetFileTitle,
    (void*)v_GetFilePath,     (void*)v_SetFilePath, (void*)v_Open3,       (void*)v_Open4,
    (void*)v_Duplicate,       (void*)v_Seek,        (void*)v_SetLength,   (void*)v_GetLength,
    (void*)v_Read,            (void*)v_Write,       (void*)v_LockRange,   (void*)v_UnlockRange,
    (void*)v_Abort,           (void*)v_Flush,       (void*)v_Close,       (void*)v_GetBufferPtr,
};

} // namespace

// Called from the exported CFile constructors to install the MSVC-layout vptr so
// drop-in MSVC clients dispatch virtuals correctly. Returns pObj for convenience.
extern "C" void* OpenMFC_PatchCFileVtable(void* pObj) {
    if (pObj) *(const void**)pObj = (const void*)&g_CFile_msvtbl[0];
    return pObj;
}

extern "C" const void* OpenMFC_CFileVtableAddr() { return (const void*)&g_CFile_msvtbl[0]; }
