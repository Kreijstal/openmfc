// CFile uses a CObject root and the native 40-byte file state.
#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxwin.h"
#include "detail/FileStatusSupport.h"
#include <algorithm>

extern "C" void impl__AfxThrowFileException__YAXHJPEB_W_Z(int,long,const wchar_t*);
extern "C" int impl__AfxMessageBox__YAHPEB_WII_Z(const wchar_t*,UINT,UINT);
extern "C" void impl__ThrowOsError_CFileException__SAXJPEB_W_Z(long,const wchar_t*);
extern "C" int impl__OsErrorToException_CFileException__SAHJ_Z(long);

using namespace openmfc::detail::files;
namespace {
void ThrowLastFileError(const CFile* file) {
    const wchar_t* name = file->m_strFileName.GetString();
    impl__ThrowOsError_CFileException__SAXJPEB_W_Z(GetLastError(),name);
}
// ATL's transaction manager is an external ABI type: handle then fallback BOOL.
// Read its documented native fields without constructing an unrelated adapter.
HANDLE TransactionFile(ATL::CAtlTransactionManager* manager,const wchar_t* name,DWORD access,
                       DWORD share,SECURITY_ATTRIBUTES* security,DWORD create,DWORD flags) {
    HANDLE transaction;
    BOOL fallback;
    memcpy(&transaction,manager,sizeof(transaction));
    memcpy(&fallback,reinterpret_cast<const BYTE*>(manager)+sizeof(HANDLE),sizeof(fallback));
    if (!transaction)
        return fallback ? CreateFileW(name,access,share,security,create,flags,nullptr) : INVALID_HANDLE_VALUE;
    HMODULE module = GetModuleHandleW(L"kernel32.dll");
    if (!module) return INVALID_HANDLE_VALUE;
    using CreateTransacted = HANDLE (WINAPI*)(LPCWSTR,DWORD,DWORD,LPSECURITY_ATTRIBUTES,DWORD,DWORD,HANDLE,HANDLE,PUSHORT,PVOID);
    auto call = reinterpret_cast<CreateTransacted>(GetProcAddress(module,"CreateFileTransactedW"));
    return call ? call(name,access,share,security,create,flags,nullptr,transaction,nullptr,nullptr) : INVALID_HANDLE_VALUE;
}
}

const HANDLE CFile::hFileNull = INVALID_HANDLE_VALUE;
CFile::CFile() { CommonBaseInit(hFileNull,nullptr); }
CFile::CFile(HANDLE handle) { CommonBaseInit(handle,nullptr); }
CFile::CFile(ATL::CAtlTransactionManager* manager) { CommonBaseInit(hFileNull,manager); }
CFile::CFile(const wchar_t* name, UINT flags) { CommonInit(name,flags,nullptr); }
CFile::CFile(const wchar_t* name, UINT flags, ATL::CAtlTransactionManager* manager) { CommonInit(name,flags,manager); }
CFile::~CFile() {
    try { if (m_hFile != hFileNull && m_bCloseOnDelete) CFile::Close(); }
    catch (CException* error) { ReportDestructor(error,L"D:\\a\\_work\\1\\s\\src\\vctools\\VC7Libs\\Ship\\ATLMFC\\Src\\MFC\\filecore.cpp",137); }
}
void CFile::CommonBaseInit(HANDLE handle, ATL::CAtlTransactionManager* manager) {
    m_hFile = handle;
    m_bCloseOnDelete = FALSE;
    m_pTM = manager;
}
void CFile::CommonInit(const wchar_t* name, UINT flags, void* manager) {
    if (!name) AfxThrowInvalidArgException();
    CommonBaseInit(hFileNull,static_cast<ATL::CAtlTransactionManager*>(manager));
    CFileException error;
    if (!Open(name,flags,&error)) impl__AfxThrowFileException__YAXHJPEB_W_Z(error.m_cause,error.m_lOsError,error.m_strFileName);
}
BOOL CFile::Open(const wchar_t* name, UINT flags, ATL::CAtlTransactionManager* manager, CFileException* error) {
    m_pTM = manager;
    return Open(name,flags,error);
}
BOOL CFile::Open(const wchar_t* name, UINT flags, CFileException* error) {
    m_hFile = hFileNull;
    m_bCloseOnDelete = FALSE;
    m_strFileName.Empty();
    if (!name || wcsnlen(name,MAX_PATH) == MAX_PATH) { BadFilePath(error,name); return FALSE; }
    wchar_t full[MAX_PATH];
    if (!FullFilePath(full,name,error)) return FALSE;
    m_strFileName = full;
    DWORD access = 0, share = 0;
    switch (flags & 3) {
    case modeRead: access = GENERIC_READ; break;
    case modeWrite: access = GENERIC_WRITE; break;
    case modeReadWrite: access = GENERIC_READ|GENERIC_WRITE; break;
    }
    switch (flags & 0x70) {
    case shareDenyWrite: share = FILE_SHARE_READ; break;
    case shareDenyRead: share = FILE_SHARE_WRITE; break;
    case shareDenyNone: share = FILE_SHARE_READ|FILE_SHARE_WRITE; break;
    }
    SECURITY_ATTRIBUTES security{sizeof(SECURITY_ATTRIBUTES),nullptr,!(flags & modeNoInherit)};
    DWORD create = flags & modeCreate ? (flags & modeNoTruncate ? OPEN_ALWAYS : CREATE_ALWAYS) : OPEN_EXISTING;
    DWORD attributes = FILE_ATTRIBUTE_NORMAL;
    if (flags & osNoBuffer) attributes |= FILE_FLAG_NO_BUFFERING;
    if (flags & osWriteThrough) attributes |= FILE_FLAG_WRITE_THROUGH;
    if (flags & osRandomAccess) attributes |= FILE_FLAG_RANDOM_ACCESS;
    if (flags & osSequentialScan) attributes |= FILE_FLAG_SEQUENTIAL_SCAN;
    HANDLE handle = m_pTM ? TransactionFile(m_pTM,name,access,share,&security,create,attributes)
                         : CreateFileW(name,access,share,&security,create,attributes,nullptr);
    if (handle == hFileNull) { FillFileError(error,name); return FALSE; }
    m_hFile = handle;
    m_bCloseOnDelete = TRUE;
    return TRUE;
}
CFile* CFile::Duplicate() const {
    CFile* copy = new CFile;
    HANDLE duplicate;
    HANDLE target = GetCurrentProcess();
    HANDLE handle = m_hFile;
    HANDLE source = GetCurrentProcess();
    if (!DuplicateHandle(source,handle,target,&duplicate,0,FALSE,DUPLICATE_SAME_ACCESS)) {
        delete copy;
        ThrowLastFileError(this);
    }
    copy->m_hFile = duplicate;
    copy->m_bCloseOnDelete = m_bCloseOnDelete;
    copy->m_pTM = m_pTM;
    return copy;
}
UINT CFile::Read(void* output, UINT count) {
    if (!count) return 0;
    DWORD read;
    if (!ReadFile(m_hFile,output,count,&read,nullptr)) ThrowLastFileError(this);
    return read;
}
void CFile::Write(const void* input, UINT count) {
    if (!count) return;
    DWORD written;
    if (!WriteFile(m_hFile,input,count,&written,nullptr)) ThrowLastFileError(this);
    if (written != count) impl__AfxThrowFileException__YAXHJPEB_W_Z(CFileException::diskFull,-1,m_strFileName.GetString());
}
ULONGLONG CFile::Seek(LONGLONG offset, UINT origin) {
    LONG high = static_cast<LONG>(static_cast<ULONGLONG>(offset)>>32);
    DWORD low = SetFilePointer(m_hFile,static_cast<LONG>(offset),&high,origin);
    if (low == INVALID_SET_FILE_POINTER && GetLastError()) ThrowLastFileError(this);
    return static_cast<ULONGLONG>(static_cast<DWORD>(high))<<32 | low;
}
ULONGLONG CFile::GetPosition() const {
    LONG high = 0;
    DWORD low = SetFilePointer(m_hFile,0,&high,FILE_CURRENT);
    if (low == INVALID_SET_FILE_POINTER && GetLastError()) ThrowLastFileError(this);
    return static_cast<ULONGLONG>(static_cast<DWORD>(high))<<32 | low;
}
void CFile::SetLength(ULONGLONG length) {
    Seek(static_cast<LONGLONG>(length),begin);
    if (!SetEndOfFile(m_hFile)) ThrowLastFileError(this);
}
ULONGLONG CFile::GetLength() const {
    DWORD high;
    DWORD low = GetFileSize(m_hFile,&high);
    if (low == INVALID_FILE_SIZE && GetLastError()) ThrowLastFileError(this);
    return static_cast<ULONGLONG>(high)<<32 | low;
}
void CFile::LockRange(ULONGLONG position, ULONGLONG count) {
    if (!LockFile(m_hFile,static_cast<DWORD>(position),static_cast<DWORD>(position>>32),
                  static_cast<DWORD>(count),static_cast<DWORD>(count>>32))) ThrowLastFileError(this);
}
void CFile::UnlockRange(ULONGLONG position, ULONGLONG count) {
    if (!UnlockFile(m_hFile,static_cast<DWORD>(position),static_cast<DWORD>(position>>32),
                    static_cast<DWORD>(count),static_cast<DWORD>(count>>32))) ThrowLastFileError(this);
}
void CFile::Flush() {
    if (m_hFile != hFileNull && !FlushFileBuffers(m_hFile)) ThrowLastFileError(this);
}
void CFile::Close() {
    BOOL failed = m_hFile != hFileNull && !CloseHandle(m_hFile);
    m_hFile = hFileNull;
    m_bCloseOnDelete = FALSE;
    m_strFileName.Empty();
    if (failed) ThrowLastFileError(this);
}
void CFile::Abort() {
    if (m_hFile != hFileNull) { CloseHandle(m_hFile); m_hFile = hFileNull; }
    m_strFileName.Empty();
}
void CFile::SetFilePath(const wchar_t* name) {
    if (!name) AfxThrowInvalidArgException();
    m_strFileName = name;
}
CString CFile::GetFilePath() const { return m_strFileName; }
CString CFile::GetFileName() const {
    const wchar_t* path = m_strFileName.GetString();
    const wchar_t* name = path;
    for (; *path; ++path) if (*path == L'\\' || *path == L'/' || *path == L':') name = path+1;
    return CString(name);
}
CString CFile::GetFileTitle() const {
    wchar_t title[MAX_PATH];
    if (::GetFileTitleW(m_strFileName.GetString(),title,MAX_PATH) == 0) return CString(title);
    return CFile::GetFileName();
}
UINT CFile::GetBufferPtr(UINT,UINT,void**,void**) { return 0; }
static CRuntimeClass* AFXAPI FileBase() { return CObject::GetThisClass(); }
CRuntimeClass CFile::classCFile = {"CFile",sizeof(CFile),0xffff,nullptr,FileBase,nullptr,nullptr};
void CFile::Rename(const wchar_t* lpszOldName, const wchar_t* lpszNewName) {
    if (lpszOldName && lpszNewName) {
        MoveFileW(lpszOldName, lpszNewName);
    }
}
void CFile::Remove(const wchar_t* lpszFileName) {
    if (lpszFileName) {
        DeleteFileW(lpszFileName);
    }
}
int CFile::GetStatus(CFileStatus& rStatus) const {
    if (!m_strFileName.IsEmpty()) {
        return GetStatus(m_strFileName.GetString(), rStatus, nullptr);
    }
    if (m_hFile == (void*)INVALID_HANDLE_VALUE) {
        return 0;
    }

    BY_HANDLE_FILE_INFORMATION info;
    if (!GetFileInformationByHandle((HANDLE)m_hFile, &info)) {
        return 0;
    }

    memset(&rStatus, 0, sizeof(rStatus));
    ULARGE_INTEGER ul;

    ul.LowPart = info.ftCreationTime.dwLowDateTime;
    ul.HighPart = info.ftCreationTime.dwHighDateTime;
    rStatus.m_ctime = ul.QuadPart;

    ul.LowPart = info.ftLastWriteTime.dwLowDateTime;
    ul.HighPart = info.ftLastWriteTime.dwHighDateTime;
    rStatus.m_mtime = ul.QuadPart;

    ul.LowPart = info.ftLastAccessTime.dwLowDateTime;
    ul.HighPart = info.ftLastAccessTime.dwHighDateTime;
    rStatus.m_atime = ul.QuadPart;

    ul.LowPart = info.nFileSizeLow;
    ul.HighPart = info.nFileSizeHigh;
    rStatus.m_size = ul.QuadPart;
    rStatus.m_attribute = (BYTE)info.dwFileAttributes;
    rStatus.m_szFullName[0] = L'\0';
    return 1;
}
int CFile::GetStatus(const wchar_t* lpszFileName, CFileStatus& rStatus, void* pTM) { (void)pTM;
    if (!lpszFileName) return 0;

    WIN32_FILE_ATTRIBUTE_DATA data;
    if (!GetFileAttributesExW(lpszFileName, GetFileExInfoStandard, &data)) {
        return 0;
    }

    // Convert FILETIME to ULONGLONG
    ULARGE_INTEGER ul;
    ul.LowPart = data.ftCreationTime.dwLowDateTime;
    ul.HighPart = data.ftCreationTime.dwHighDateTime;
    rStatus.m_ctime = ul.QuadPart;

    ul.LowPart = data.ftLastWriteTime.dwLowDateTime;
    ul.HighPart = data.ftLastWriteTime.dwHighDateTime;
    rStatus.m_mtime = ul.QuadPart;

    ul.LowPart = data.ftLastAccessTime.dwLowDateTime;
    ul.HighPart = data.ftLastAccessTime.dwHighDateTime;
    rStatus.m_atime = ul.QuadPart;

    ul.LowPart = data.nFileSizeLow;
    ul.HighPart = data.nFileSizeHigh;
    rStatus.m_size = ul.QuadPart;

    rStatus.m_attribute = (BYTE)data.dwFileAttributes;

    // Copy full path
    wcsncpy(rStatus.m_szFullName, lpszFileName, _MAX_PATH - 1);
    rStatus.m_szFullName[_MAX_PATH - 1] = L'\0';

    return 1;
}
void CFile::SetStatus(const wchar_t* lpszFileName, const CFileStatus& status) {
    if (!lpszFileName) return;

    // Set file attributes
    SetFileAttributesW(lpszFileName, status.m_attribute);

    // Set file times (requires opening the file)
    HANDLE hFile = CreateFileW(lpszFileName, FILE_WRITE_ATTRIBUTES,
                               FILE_SHARE_READ | FILE_SHARE_WRITE,
                               nullptr, OPEN_EXISTING, 0, nullptr);
    if (hFile != INVALID_HANDLE_VALUE) {
        FILETIME ftCreate, ftAccess, ftWrite;
        ULARGE_INTEGER ul;

        ul.QuadPart = status.m_ctime;
        ftCreate.dwLowDateTime = ul.LowPart;
        ftCreate.dwHighDateTime = ul.HighPart;

        ul.QuadPart = status.m_atime;
        ftAccess.dwLowDateTime = ul.LowPart;
        ftAccess.dwHighDateTime = ul.HighPart;

        ul.QuadPart = status.m_mtime;
        ftWrite.dwLowDateTime = ul.LowPart;
        ftWrite.dwHighDateTime = ul.HighPart;

        SetFileTime(hFile, &ftCreate, &ftAccess, &ftWrite);
        CloseHandle(hFile);
    }
}

struct CFileExports {
    static void CommonBaseInit(CFile* p,HANDLE h,ATL::CAtlTransactionManager* t) { p->CommonBaseInit(h,t); }
    static void CommonInit(CFile* p,const wchar_t* n,UINT f,void* t) { p->CommonInit(n,f,t); }
};

// Symbol: ?Write@CFile@@UEAAXPEBXI@Z
extern "C" void impl__Write_CFile__UEAAXPEBXI_Z(CFile* p, const void* b, UINT n) { p->CFile::Write(b,n); }

// Symbol: ?UnlockRange@CFile@@UEAAX_K0@Z
extern "C" void impl__UnlockRange_CFile__UEAAX_K0_Z(CFile* p, ULONGLONG o, ULONGLONG n) { p->CFile::UnlockRange(o,n); }

// Symbol: ?SetStatus@CFile@@SAXPEB_WAEBUCFileStatus@@PEAVCAtlTransactionManager@ATL@@@Z
extern "C" void impl__SetStatus_CFile__SAXPEB_WAEBUCFileStatus__PEAVCAtlTransactionManager_ATL___Z(const wchar_t* n, const CFileStatus& t, void*) { CFile::SetStatus(n,t); }

// Symbol: ?SetLength@CFile@@UEAAX_K@Z
extern "C" void impl__SetLength_CFile__UEAAX_K_Z(CFile* p, ULONGLONG n) { p->CFile::SetLength(n); }

// Symbol: ?Seek@CFile@@UEAA_K_JI@Z
extern "C" ULONGLONG impl__Seek_CFile__UEAA_K_JI_Z(CFile* p, LONGLONG o, UINT f) { return p->CFile::Seek(o,f); }

// Symbol: ?Rename@CFile@@SAXPEB_W0PEAVCAtlTransactionManager@ATL@@@Z
extern "C" void impl__Rename_CFile__SAXPEB_W0PEAVCAtlTransactionManager_ATL___Z(const wchar_t* a, const wchar_t* b, void*) { CFile::Rename(a,b); }

// Symbol: ?Remove@CFile@@SAXPEB_WPEAVCAtlTransactionManager@ATL@@@Z
extern "C" void impl__Remove_CFile__SAXPEB_WPEAVCAtlTransactionManager_ATL___Z(const wchar_t* n, void*) { CFile::Remove(n); }

// Symbol: ?Read@CFile@@UEAAIPEAXI@Z
extern "C" UINT impl__Read_CFile__UEAAIPEAXI_Z(CFile* p, void* b, UINT n) { return p->CFile::Read(b,n); }

// Symbol: ?Open@CFile@@UEAAHPEB_WIPEAVCFileException@@@Z
extern "C" BOOL impl__Open_CFile__UEAAHPEB_WIPEAVCFileException___Z(CFile* p,const wchar_t* n,UINT f,CFileException* e) { return p->CFile::Open(n,f,e); }

// Symbol: ?Open@CFile@@UEAAHPEB_WIPEAVCAtlTransactionManager@ATL@@PEAVCFileException@@@Z
extern "C" BOOL impl__Open_CFile__UEAAHPEB_WIPEAVCAtlTransactionManager_ATL__PEAVCFileException___Z(CFile* p,const wchar_t* n,UINT f,ATL::CAtlTransactionManager* t,CFileException* e) { return p->CFile::Open(n,f,t,e); }

// Symbol: ?LockRange@CFile@@UEAAX_K0@Z
extern "C" void impl__LockRange_CFile__UEAAX_K0_Z(CFile* p, ULONGLONG o, ULONGLONG n) { p->CFile::LockRange(o,n); }

// Symbol: ?GetThisClass@CFile@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* impl__GetThisClass_CFile__SAPEAUCRuntimeClass__XZ() { return CFile::GetThisClass(); }

// Symbol: ?GetStatus@CFile@@SAHPEB_WAEAUCFileStatus@@PEAVCAtlTransactionManager@ATL@@@Z
extern "C" int impl__GetStatus_CFile__SAHPEB_WAEAUCFileStatus__PEAVCAtlTransactionManager_ATL___Z(const wchar_t* n,CFileStatus& t,void* m) { return CFile::GetStatus(n,t,m); }

// Symbol: ?GetStatus@CFile@@QEBAHAEAUCFileStatus@@@Z
extern "C" int impl__GetStatus_CFile__QEBAHAEAUCFileStatus___Z(const CFile* p,CFileStatus& t) { return p->CFile::GetStatus(t); }

// Symbol: ?GetRuntimeClass@CFile@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* impl__GetRuntimeClass_CFile__UEBAPEAUCRuntimeClass__XZ(const CFile* p) { return p->CFile::GetRuntimeClass(); }

// Symbol: ?GetPosition@CFile@@UEBA_KXZ
extern "C" ULONGLONG impl__GetPosition_CFile__UEBA_KXZ(const CFile* p) { return p->CFile::GetPosition(); }

// Symbol: ?GetLength@CFile@@UEBA_KXZ
extern "C" ULONGLONG impl__GetLength_CFile__UEBA_KXZ(const CFile* p) { return p->CFile::GetLength(); }

// Symbol: ?GetFileTitle@CFile@@UEBA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@XZ
extern "C" void* impl__GetFileTitle_CFile__UEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__XZ(const CFile* p, void* r) { new(r) CString(p->CFile::GetFileTitle()); return r; }

// Symbol: ?GetFilePath@CFile@@UEBA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@XZ
extern "C" void* impl__GetFilePath_CFile__UEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__XZ(const CFile* p, void* r) { new(r) CString(p->CFile::GetFilePath()); return r; }

// Symbol: ?GetFileName@CFile@@UEBA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@XZ
extern "C" void* impl__GetFileName_CFile__UEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__XZ(const CFile* p, void* r) { new(r) CString(p->CFile::GetFileName()); return r; }

// Symbol: ?GetBufferPtr@CFile@@UEAAIIIPEAPEAX0@Z
extern "C" UINT impl__GetBufferPtr_CFile__UEAAIIIPEAPEAX0_Z(CFile* p, UINT c, UINT n, void** b, void** e) { return p->CFile::GetBufferPtr(c,n,b,e); }

// Symbol: ?Flush@CFile@@UEAAXXZ
extern "C" void impl__Flush_CFile__UEAAXXZ(CFile* p) { p->CFile::Flush(); }

// Symbol: ?Duplicate@CFile@@UEBAPEAV1@XZ
extern "C" CFile* impl__Duplicate_CFile__UEBAPEAV1_XZ(const CFile* p) { return p->CFile::Duplicate(); }

// Symbol: ?CommonInit@CFile@@IEAAXPEB_WIPEAVCAtlTransactionManager@ATL@@@Z
extern "C" void impl__CommonInit_CFile__IEAAXPEB_WIPEAVCAtlTransactionManager_ATL___Z(CFile* p, const wchar_t* n, UINT f, void* t) { CFileExports::CommonInit(p,n,f,t); }

// Symbol: ?CommonBaseInit@CFile@@IEAAXPEAXPEAVCAtlTransactionManager@ATL@@@Z
extern "C" void impl__CommonBaseInit_CFile__IEAAXPEAXPEAVCAtlTransactionManager_ATL___Z(CFile* p, HANDLE h, ATL::CAtlTransactionManager* t) { CFileExports::CommonBaseInit(p,h,t); }

// Symbol: ?Close@CFile@@UEAAXXZ
extern "C" void impl__Close_CFile__UEAAXXZ(CFile* p) { p->CFile::Close(); }

// Symbol: ?Abort@CFile@@UEAAXXZ
extern "C" void impl__Abort_CFile__UEAAXXZ(CFile* p) { p->CFile::Abort(); }

// Symbol: ??1CFile@@UEAA@XZ
extern "C" void impl___1CFile__UEAA_XZ(CFile* p) { p->CFile::~CFile(); }

// Symbol: ??0CFile@@QEAA@XZ
extern "C" CFile* impl___0CFile__QEAA_XZ(void* p) { return new(p) CFile; }

// Symbol: ??0CFile@@QEAA@PEB_WIPEAVCAtlTransactionManager@ATL@@@Z
extern "C" CFile* impl___0CFile__QEAA_PEB_WIPEAVCAtlTransactionManager_ATL___Z(void* p,const wchar_t* n,UINT f,ATL::CAtlTransactionManager* t) { return new(p) CFile(n,f,t); }

// Symbol: ??0CFile@@QEAA@PEB_WI@Z
extern "C" CFile* impl___0CFile__QEAA_PEB_WI_Z(void* p,const wchar_t* n,UINT f) { return new(p) CFile(n,f); }

// Symbol: ??0CFile@@QEAA@PEAX@Z
extern "C" CFile* impl___0CFile__QEAA_PEAX_Z(void* p,HANDLE h) { return new(p) CFile(h); }

// Symbol: ??0CFile@@QEAA@PEAVCAtlTransactionManager@ATL@@@Z
extern "C" CFile* impl___0CFile__QEAA_PEAVCAtlTransactionManager_ATL___Z(void* p,ATL::CAtlTransactionManager* t) { return new(p) CFile(t); }

// Symbol: ?hFileNull@CFile@@2QEAXEA
// Provided by the actual CFile::hFileNull data definition above.
