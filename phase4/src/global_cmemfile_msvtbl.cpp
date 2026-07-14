// OpenMFC SPIKE: hand-authored MSVC-layout vtable for CMemFile.
//
// The DLL is built with mingw-gcc (Itanium C++ ABI), whose vtable layout differs
// from MSVC's — most importantly Itanium emits TWO destructor slots vs MSVC's one,
// so every virtual past the destructor mis-dispatches when a real MSVC client calls
// through the object vtable (cobject_impl.cpp documents that only vtable[0] is safe).
// That is why CMemFile::Write (a post-destructor slot) crashed a drop-in client.
//
// This file provides an explicit MSVC-slot-ordered vtable for CMemFile and patches
// the object's vptr to it in the exported constructors, so a MSVC client's virtual
// calls land on the right method. Each slot is a small MS-ABI wrapper that calls the
// real member NON-virtually (qualified) to avoid re-dispatching back through this
// same vtable (which would recurse).
//
// MSVC vtable order = CObject's 5 slots then CFile's 19 new virtuals in declaration
// order, with CMemFile's overrides occupying their inherited CFile slots:
//   0 GetRuntimeClass  1 ~dtor(vector-deleting)  2 Serialize  3 AssertValid  4 Dump
//   5 GetPosition  6 GetFileName  7 GetFileTitle  8 GetFilePath  9 SetFilePath
//   10 Open(3)  11 Open(4)  12 Duplicate  13 Seek  14 SetLength  15 GetLength
//   16 Read  17 Write  18 LockRange  19 UnlockRange  20 Abort  21 Flush  22 Close
//   23 GetBufferPtr
//
// Named global_*_rtti-adjacent; build_phase4.sh's shard glob compiles global_*.

#include "openmfc/afxwin.h"
#include <cstdlib>
#include <new>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

extern "C" void* MS_ABI impl__Duplicate_CMemFile__UEBAPEAVCFile__XZ(const CMemFile* pThis);
extern "C" unsigned int MS_ABI impl__GetBufferPtr_CMemFile__UEAAIIIPEAPEAX0_Z(
    CMemFile* pThis, unsigned int nCommand, unsigned int nCount, void** ppBufStart, void** ppBufMax);
extern "C" int MS_ABI impl__Open_CFile__UEAAHPEB_WIPEAVCFileException___Z(
    void* pThis, const wchar_t* lpszFileName, unsigned int nOpenFlags, void* pException);
extern "C" int MS_ABI impl__Open_CFile__UEAAHPEB_WIPEAVCAtlTransactionManager_ATL__PEAVCFileException___Z(
    void* pThis, const wchar_t* lpszFileName, unsigned int nOpenFlags, void* pTM, void* pException);
extern "C" void MS_ABI impl__LockRange_CMemFile__UEAAX_K0_Z(CMemFile* pThis, unsigned long long dwPos, unsigned long long dwCount);
extern "C" void MS_ABI impl__UnlockRange_CMemFile__UEAAX_K0_Z(CMemFile* pThis, unsigned long long dwPos, unsigned long long dwCount);
extern "C" void MS_ABI impl__Abort_CMemFile__UEAAXXZ(CMemFile* pThis);
extern "C" void MS_ABI impl__SetLength_CMemFile__UEAAX_K_Z(CMemFile* pThis, unsigned long long dwNewLen);

namespace {

// --- slot wrappers: qualified (non-virtual) member calls, no re-dispatch ---
CRuntimeClass* MS_ABI v_GetRuntimeClass(CMemFile*)            { return &CMemFile::classCMemFile; }
void*          MS_ABI v_dtor(CMemFile* p, unsigned int flags) { p->CMemFile::~CMemFile(); if (flags & 1) ::operator delete(p); return p; }
void           MS_ABI v_Serialize(CMemFile*, void*)           {}
void           MS_ABI v_AssertValid(const CMemFile*)          {}
void           MS_ABI v_Dump(const CMemFile*, void*)          {}
unsigned long long MS_ABI v_GetPosition(const CMemFile* p)    { return const_cast<CMemFile*>(p)->CMemFile::Seek(0, 1 /*current*/); }
// CString returned by value via sret. Proven repo convention (impl__GetFileName_CFile
// thunk): this=RCX, sret=RDX, void return — construct the CString into the caller's
// buffer. Stubbing (or swapping the params) crashes the client.
void           MS_ABI v_GetFileName(CMemFile* p, CString* pRet)  { new(pRet) CString(p->CFile::GetFileName());  }
void           MS_ABI v_GetFileTitle(CMemFile* p, CString* pRet) { new(pRet) CString(p->CFile::GetFileTitle()); }
void           MS_ABI v_GetFilePath(CMemFile* p, CString* pRet)  { new(pRet) CString(p->CFile::GetFilePath());  }
void           MS_ABI v_SetFilePath(CMemFile* p, const wchar_t* n) { p->CFile::SetFilePath(n); }
int            MS_ABI v_Open3(CMemFile* p, const wchar_t* lpszFileName, unsigned nOpenFlags, void* pException) {
    return impl__Open_CFile__UEAAHPEB_WIPEAVCFileException___Z(p, lpszFileName, nOpenFlags, pException);
}
int            MS_ABI v_Open4(CMemFile* p, const wchar_t* lpszFileName, unsigned nOpenFlags, void* pTM, void* pException) {
    return impl__Open_CFile__UEAAHPEB_WIPEAVCAtlTransactionManager_ATL__PEAVCFileException___Z(
        p, lpszFileName, nOpenFlags, pTM, pException);
}
void*          MS_ABI v_Duplicate(const CMemFile* p)            { return impl__Duplicate_CMemFile__UEBAPEAVCFile__XZ(p); }
unsigned long long MS_ABI v_Seek(CMemFile* p, long long off, unsigned from) { return p->CMemFile::Seek(off, from); }
void           MS_ABI v_SetLength(CMemFile* p, unsigned long long n)        { p->CMemFile::SetLength(n); }
unsigned long long MS_ABI v_GetLength(const CMemFile* p)      { return const_cast<CMemFile*>(p)->CMemFile::GetLength(); }
unsigned int   MS_ABI v_Read(CMemFile* p, void* b, unsigned int n)         { return p->CMemFile::Read(b, n); }
void           MS_ABI v_Write(CMemFile* p, const void* b, unsigned int n)  { p->CMemFile::Write(b, n); }
void           MS_ABI v_LockRange(CMemFile* p, unsigned long long dwPos, unsigned long long dwCount)   {
    impl__LockRange_CMemFile__UEAAX_K0_Z(p, dwPos, dwCount);
}
void           MS_ABI v_UnlockRange(CMemFile* p, unsigned long long dwPos, unsigned long long dwCount) {
    impl__UnlockRange_CMemFile__UEAAX_K0_Z(p, dwPos, dwCount);
}
void           MS_ABI v_Abort(CMemFile* p)                      { impl__Abort_CMemFile__UEAAXXZ(p); }
void           MS_ABI v_Flush(CMemFile* p)                    { p->CMemFile::Flush(); }
void           MS_ABI v_Close(CMemFile* p)                    { p->CFile::Close(); }
unsigned int   MS_ABI v_GetBufferPtr(CMemFile* p, unsigned int nCommand, unsigned int nCount, void** ppBufStart, void** ppBufMax) {
    return impl__GetBufferPtr_CMemFile__UEAAIIIPEAPEAX0_Z(p, nCommand, nCount, ppBufStart, ppBufMax);
}

// The 24-slot MSVC-layout vtable. vptr points at &slot[0].
void* const g_CMemFile_msvtbl[24] = {
    (void*)v_GetRuntimeClass, (void*)v_dtor,        (void*)v_Serialize,   (void*)v_AssertValid,
    (void*)v_Dump,            (void*)v_GetPosition, (void*)v_GetFileName, (void*)v_GetFileTitle,
    (void*)v_GetFilePath,     (void*)v_SetFilePath, (void*)v_Open3,       (void*)v_Open4,
    (void*)v_Duplicate,       (void*)v_Seek,        (void*)v_SetLength,   (void*)v_GetLength,
    (void*)v_Read,            (void*)v_Write,       (void*)v_LockRange,   (void*)v_UnlockRange,
    (void*)v_Abort,           (void*)v_Flush,       (void*)v_Close,       (void*)v_GetBufferPtr,
};

} // namespace

// Called from the exported CMemFile constructors to install the MSVC-layout vptr so
// drop-in MSVC clients dispatch virtuals correctly. Returns pObj for convenience.
extern "C" void* OpenMFC_PatchCMemFileVtable(void* pObj) {
    if (pObj) *(const void**)pObj = (const void*)&g_CMemFile_msvtbl[0];
    return pObj;
}

extern "C" const void* OpenMFC_CMemFileVtableAddr() { return (const void*)&g_CMemFile_msvtbl[0]; }
