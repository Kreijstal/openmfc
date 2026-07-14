// OpenMFC SPIKE: hand-authored MSVC-layout vtable for CStdioFile.
//
// The DLL is built with mingw-gcc (Itanium C++ ABI), whose vtable layout differs
// from MSVC's — most importantly Itanium emits TWO destructor slots vs MSVC's one,
// so every virtual past the destructor mis-dispatches when a real MSVC client calls
// through the object vtable (cobject_impl.cpp documents that only vtable[0] is safe).
//
// This file provides an explicit MSVC-slot-ordered vtable for CStdioFile and patches
// the object's vptr to it. Each slot is a small MS-ABI wrapper that calls the real
// member NON-virtually (qualified) to avoid re-dispatching back through this same
// vtable (which would recurse).
//
// MSVC vtable order = CObject's 5 slots then CFile's 19 new virtuals in declaration
// order, with CStdioFile's overrides occupying their inherited CFile slots, plus
// CStdioFile's own trailing virtuals (WriteString/ReadString):
//   0 GetRuntimeClass  1 ~dtor(vector-deleting)  2 Serialize  3 AssertValid  4 Dump
//   5 GetPosition  6 GetFileName  7 GetFileTitle  8 GetFilePath  9 SetFilePath
//   10 Open(3)  11 Open(4)  12 Duplicate  13 Seek  14 SetLength  15 GetLength
//   16 Read  17 Write  18 LockRange  19 UnlockRange  20 Abort  21 Flush  22 Close
//   23 GetBufferPtr  24 WriteString  25 ReadString(wchar)  26 ReadString(CString&)
//
// NOTE: CStdioFile has no classCStdioFile / GetThisClass() in afx.h, so slot 0 falls
// back to &CFile::classCFile. CStdioFile also has no GetLength/SetLength/GetPosition/
// SetFilePath overrides; those slots stub or call the CFile member as appropriate.
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

// Real CStdioFile::GetLength logic lives in this exported thunk (fstat on the FILE*);
// CStdioFile uses a FILE* (not m_hFile), so CFile::GetLength would be wrong here.
extern "C" unsigned long long MS_ABI impl__GetLength_CStdioFile__UEBA_KXZ(const CStdioFile*);
extern "C" int MS_ABI impl__Open_CStdioFile__UEAAHPEB_WIPEAVCFileException___Z(
    CStdioFile* pThis, const wchar_t* lpszFileName, unsigned int nOpenFlags, void* pException);
extern "C" int MS_ABI impl__Open_CStdioFile__UEAAHPEB_WIPEAVCAtlTransactionManager_ATL__PEAVCFileException___Z(
    CStdioFile* pThis, const wchar_t* lpszFileName, unsigned int nOpenFlags, void* pTM, void* pException);
extern "C" CFile* MS_ABI impl__Duplicate_CStdioFile__UEBAPEAVCFile__XZ(const CStdioFile* pThis);
extern "C" unsigned int MS_ABI impl__GetBufferPtr_CFile__UEAAIIIPEAPEAX0_Z(
    void* pThis, unsigned int nCommand, unsigned int nCount, void** ppBufStart, void** ppBufMax);
extern "C" void MS_ABI impl__LockRange_CStdioFile__UEAAX_K0_Z(
    CStdioFile* pThis, unsigned long long dwPos, unsigned long long dwCount);
extern "C" void MS_ABI impl__UnlockRange_CStdioFile__UEAAX_K0_Z(
    CStdioFile* pThis, unsigned long long dwPos, unsigned long long dwCount);
extern "C" void MS_ABI impl__Abort_CStdioFile__UEAAXXZ(CStdioFile* pThis);

namespace {

// --- slot wrappers: qualified (non-virtual) member calls, no re-dispatch ---
CRuntimeClass* MS_ABI v_GetRuntimeClass(CStdioFile*)             { return &CFile::classCFile; }
void*          MS_ABI v_dtor(CStdioFile* p, unsigned int flags)  { p->CStdioFile::~CStdioFile(); if (flags & 1) ::operator delete(p); return p; }
void           MS_ABI v_Serialize(CStdioFile*, void*)            {}
void           MS_ABI v_AssertValid(const CStdioFile*)           {}
void           MS_ABI v_Dump(const CStdioFile*, void*)           {}
unsigned long long MS_ABI v_GetPosition(const CStdioFile* p)     { return const_cast<CStdioFile*>(p)->CStdioFile::Seek(0, 1 /*current*/); }
// CString returned by value via sret. Proven repo convention (impl__GetFileName_CFile
// thunk): this=RCX, sret=RDX, void return — construct into the caller's buffer.
void           MS_ABI v_GetFileName(CStdioFile* p, CString* pRet)  { new(pRet) CString(p->CFile::GetFileName());  }
void           MS_ABI v_GetFileTitle(CStdioFile* p, CString* pRet) { new(pRet) CString(p->CFile::GetFileTitle()); }
void           MS_ABI v_GetFilePath(CStdioFile* p, CString* pRet)  { new(pRet) CString(p->CFile::GetFilePath());  }
void           MS_ABI v_SetFilePath(CStdioFile* p, const wchar_t* n) { p->CFile::SetFilePath(n); }
int            MS_ABI v_Open3(CStdioFile* p, const wchar_t* lpszFileName, unsigned nOpenFlags, void* pException) {
    return impl__Open_CStdioFile__UEAAHPEB_WIPEAVCFileException___Z(p, lpszFileName, nOpenFlags, pException);
}
int            MS_ABI v_Open4(CStdioFile* p, const wchar_t* lpszFileName, unsigned nOpenFlags, void* pTM, void* pException) {
    return impl__Open_CStdioFile__UEAAHPEB_WIPEAVCAtlTransactionManager_ATL__PEAVCFileException___Z(
        p, lpszFileName, nOpenFlags, pTM, pException);
}
void*          MS_ABI v_Duplicate(const CStdioFile* p)             { return impl__Duplicate_CStdioFile__UEBAPEAVCFile__XZ(p); }
unsigned long long MS_ABI v_Seek(CStdioFile* p, long long off, unsigned from) { return p->CStdioFile::Seek(off, from); }
void           MS_ABI v_SetLength(CStdioFile* p, unsigned long long n)        { p->CFile::SetLength(n); }
unsigned long long MS_ABI v_GetLength(const CStdioFile* p)       { return impl__GetLength_CStdioFile__UEBA_KXZ(p); }
unsigned int   MS_ABI v_Read(CStdioFile* p, void* b, unsigned int n)         { return p->CStdioFile::Read(b, n); }
void           MS_ABI v_Write(CStdioFile* p, const void* b, unsigned int n)  { p->CStdioFile::Write(b, n); }
void           MS_ABI v_LockRange(CStdioFile* p, unsigned long long dwPos, unsigned long long dwCount) {
    impl__LockRange_CStdioFile__UEAAX_K0_Z(p, dwPos, dwCount);
}
void           MS_ABI v_UnlockRange(CStdioFile* p, unsigned long long dwPos, unsigned long long dwCount) {
    impl__UnlockRange_CStdioFile__UEAAX_K0_Z(p, dwPos, dwCount);
}
void           MS_ABI v_Abort(CStdioFile* p) { impl__Abort_CStdioFile__UEAAXXZ(p); }
void           MS_ABI v_Flush(CStdioFile* p)                     { p->CStdioFile::Flush(); }
void           MS_ABI v_Close(CStdioFile* p)                     { p->CStdioFile::Close(); }
unsigned int   MS_ABI v_GetBufferPtr(CStdioFile* p, unsigned int nCommand, unsigned int nCount, void** ppBufStart, void** ppBufMax) {
    return impl__GetBufferPtr_CFile__UEAAIIIPEAPEAX0_Z(p, nCommand, nCount, ppBufStart, ppBufMax);
}
void           MS_ABI v_WriteString(CStdioFile* p, const wchar_t* s) { p->CStdioFile::WriteString(s); }
void*          MS_ABI v_ReadStringW(CStdioFile* p, wchar_t* s, unsigned n) { return p->CStdioFile::ReadString(s, n); }
int            MS_ABI v_ReadStringS(CStdioFile* p, CString& s)      { return p->CStdioFile::ReadString(s); }

// The 27-slot MSVC-layout vtable. vptr points at &slot[0].
void* const g_CStdioFile_msvtbl[27] = {
    (void*)v_GetRuntimeClass, (void*)v_dtor,        (void*)v_Serialize,   (void*)v_AssertValid,
    (void*)v_Dump,            (void*)v_GetPosition, (void*)v_GetFileName, (void*)v_GetFileTitle,
    (void*)v_GetFilePath,     (void*)v_SetFilePath, (void*)v_Open3,       (void*)v_Open4,
    (void*)v_Duplicate,       (void*)v_Seek,        (void*)v_SetLength,   (void*)v_GetLength,
    (void*)v_Read,            (void*)v_Write,       (void*)v_LockRange,   (void*)v_UnlockRange,
    (void*)v_Abort,           (void*)v_Flush,       (void*)v_Close,       (void*)v_GetBufferPtr,
    (void*)v_WriteString,     (void*)v_ReadStringW, (void*)v_ReadStringS,
};

} // namespace

// Called from the exported CStdioFile constructors to install the MSVC-layout vptr so
// drop-in MSVC clients dispatch virtuals correctly. Returns pObj for convenience.
extern "C" void* OpenMFC_PatchCStdioFileVtable(void* pObj) {
    if (pObj) *(const void**)pObj = (const void*)&g_CStdioFile_msvtbl[0];
    return pObj;
}

extern "C" const void* OpenMFC_CStdioFileVtableAddr() { return (const void*)&g_CStdioFile_msvtbl[0]; }
