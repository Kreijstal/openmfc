// CFile uses a CObject root and the native 40-byte file state.
#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxwin.h"
#include "detail/FileStatusSupport.h"
#include <algorithm>
#include <cerrno>

extern "C" void impl__AfxThrowFileException__YAXHJPEB_W_Z(int,long,const wchar_t*);
extern "C" int impl__AfxMessageBox__YAHPEB_WII_Z(const wchar_t*,UINT,UINT);
extern "C" void impl__ThrowOsError_CFileException__SAXJPEB_W_Z(long,const wchar_t*);
extern "C" int impl__OsErrorToException_CFileException__SAHJ_Z(long);

extern "C" UINT impl__AfxGetFileName__YAIPEB_WPEA_WI_Z(const wchar_t*,wchar_t*,UINT);
extern "C" UINT impl__AfxGetFileTitle__YAIPEB_WPEA_WI_Z(const wchar_t*,wchar_t*,UINT);
using namespace openmfc::detail::files;
namespace {
void ThrowLastFileError(const CFile* file) {
    const wchar_t* name = file->m_strFileName.GetString();
    impl__ThrowOsError_CFileException__SAXJPEB_W_Z(GetLastError(),name);
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
    HANDLE handle = m_pTM ? OpenTransactionFile(m_pTM,name,access,share,&security,create,attributes)
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
    CFileStatus status;
    CFile::GetStatus(status);
    CString result;
    impl__AfxGetFileName__YAIPEB_WPEA_WI_Z(status.m_szFullName,result.GetBuffer(256),256);
    result.ReleaseBuffer();
    return result;
}
CString CFile::GetFileTitle() const {
    CFileStatus status;
    CFile::GetStatus(status);
    CString result;
    impl__AfxGetFileTitle__YAIPEB_WPEA_WI_Z(status.m_szFullName,result.GetBuffer(256),256);
    result.ReleaseBuffer();
    return result;
}
UINT CFile::GetBufferPtr(UINT,UINT,void**,void**) { return 0; }
static CRuntimeClass* AFXAPI FileBase() { return CObject::GetThisClass(); }
CRuntimeClass CFile::classCFile = {"CFile",sizeof(CFile),0xffff,nullptr,FileBase,nullptr,nullptr};
void CFile::Rename(const wchar_t* oldName,const wchar_t* newName,ATL::CAtlTransactionManager* manager) {
    if (!openmfc::detail::files::MovePath(oldName,newName,manager))
        impl__ThrowOsError_CFileException__SAXJPEB_W_Z(GetLastError(),oldName);
}
void CFile::Remove(const wchar_t* name,ATL::CAtlTransactionManager* manager) {
    if (!openmfc::detail::files::DeletePath(name,manager))
        impl__ThrowOsError_CFileException__SAXJPEB_W_Z(GetLastError(),name);
}
int CFile::GetStatus(CFileStatus& status) const {
    memset(&status,0,sizeof(status));
    int error=wcsncpy_s(status.m_szFullName,MAX_PATH,m_strFileName.GetString(),_TRUNCATE);
    if (error && error!=STRUNCATE) { if (error==ENOMEM) AfxThrowMemoryException(); AfxThrowInvalidArgException(); }
    if (m_hFile==hFileNull) return TRUE;
    FILETIME created,accessed,written;
    if (!GetFileTime(m_hFile,&created,&accessed,&written)) return FALSE;
    LARGE_INTEGER size;
    if (!GetFileSizeEx(m_hFile,&size)) return FALSE;
    status.m_size=size.QuadPart;
    DWORD attributes=m_strFileName.IsEmpty()?0:FileAttributes(m_strFileName.GetString(),m_pTM);
    status.m_attribute=attributes==INVALID_FILE_ATTRIBUTES?0:attributes;
    status.m_ctime=StatusTime(created);
    status.m_atime=StatusTime(accessed);
    status.m_mtime=StatusTime(written);
    if (!status.m_ctime) status.m_ctime=status.m_mtime;
    if (!status.m_atime) status.m_atime=status.m_mtime;
    return TRUE;
}
int CFile::GetStatus(const wchar_t* name,CFileStatus& status,ATL::CAtlTransactionManager* manager) {
    if (!name || wcslen(name)>=MAX_PATH) return FALSE;
    if (!FullFilePath(status.m_szFullName,name,nullptr)) {status.m_szFullName[0]=0;return FALSE;}
    WIN32_FILE_ATTRIBUTE_DATA data;
    if (!FileAttributes(name,&data,manager)) return FALSE;
    status.m_attribute=data.dwFileAttributes&~FILE_ATTRIBUTE_NORMAL;
    status.m_size=(ULONGLONG(data.nFileSizeHigh)<<32)|data.nFileSizeLow;
    status.m_ctime=StatusTime(data.ftCreationTime);
    status.m_atime=StatusTime(data.ftLastAccessTime);
    status.m_mtime=StatusTime(data.ftLastWriteTime);
    if (!status.m_ctime) status.m_ctime=status.m_mtime;
    if (!status.m_atime) status.m_atime=status.m_mtime;
    return TRUE;
}
void CFile::SetStatus(const wchar_t* name,const CFileStatus& status,ATL::CAtlTransactionManager* manager) {
    DWORD original=FileAttributes(name,manager);
    if (original==INVALID_FILE_ATTRIBUTES) impl__ThrowOsError_CFileException__SAXJPEB_W_Z(GetLastError(),name);
    DWORD attributes=status.m_attribute;
    if (attributes!=original && (original&FILE_ATTRIBUTE_READONLY) &&
        !openmfc::detail::files::ChangeAttributes(name,attributes,manager))
        impl__ThrowOsError_CFileException__SAXJPEB_W_Z(GetLastError(),name);
    if (status.m_mtime) {
        FILETIME written,accessed,created;
        TimeToFileTime(&status.m_mtime,&written);
        FILETIME *access=nullptr,*creation=nullptr;
        if (status.m_atime) {TimeToFileTime(&status.m_atime,&accessed);access=&accessed;}
        if (status.m_ctime) {TimeToFileTime(&status.m_ctime,&created);creation=&created;}
        HANDLE file=manager?OpenTransactionFile(manager,name,GENERIC_READ|GENERIC_WRITE,FILE_SHARE_READ,nullptr,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL)
                           :CreateFileW(name,GENERIC_READ|GENERIC_WRITE,FILE_SHARE_READ,nullptr,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,nullptr);
        if (file==hFileNull) impl__ThrowOsError_CFileException__SAXJPEB_W_Z(GetLastError(),name);
        if (!SetFileTime(file,creation,access,&written)) {
            DWORD error=GetLastError();
            CloseHandle(file);
            impl__ThrowOsError_CFileException__SAXJPEB_W_Z(error,name);
        }
        if (!CloseHandle(file)) impl__ThrowOsError_CFileException__SAXJPEB_W_Z(GetLastError(),name);
    }
    attributes=status.m_attribute;
    if (attributes!=original && !(original&FILE_ATTRIBUTE_READONLY) &&
        !openmfc::detail::files::ChangeAttributes(name,attributes,manager))
        impl__ThrowOsError_CFileException__SAXJPEB_W_Z(GetLastError(),name);
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
extern "C" void impl__SetStatus_CFile__SAXPEB_WAEBUCFileStatus__PEAVCAtlTransactionManager_ATL___Z(const wchar_t* n, const CFileStatus& t, ATL::CAtlTransactionManager* m) { CFile::SetStatus(n,t,m); }

// Symbol: ?SetLength@CFile@@UEAAX_K@Z
extern "C" void impl__SetLength_CFile__UEAAX_K_Z(CFile* p, ULONGLONG n) { p->CFile::SetLength(n); }

// Symbol: ?Seek@CFile@@UEAA_K_JI@Z
extern "C" ULONGLONG impl__Seek_CFile__UEAA_K_JI_Z(CFile* p, LONGLONG o, UINT f) { return p->CFile::Seek(o,f); }

// Symbol: ?Rename@CFile@@SAXPEB_W0PEAVCAtlTransactionManager@ATL@@@Z
extern "C" void impl__Rename_CFile__SAXPEB_W0PEAVCAtlTransactionManager_ATL___Z(const wchar_t* a, const wchar_t* b, ATL::CAtlTransactionManager* m) { CFile::Rename(a,b,m); }

// Symbol: ?Remove@CFile@@SAXPEB_WPEAVCAtlTransactionManager@ATL@@@Z
extern "C" void impl__Remove_CFile__SAXPEB_WPEAVCAtlTransactionManager_ATL___Z(const wchar_t* n, ATL::CAtlTransactionManager* m) { CFile::Remove(n,m); }

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
extern "C" int impl__GetStatus_CFile__SAHPEB_WAEAUCFileStatus__PEAVCAtlTransactionManager_ATL___Z(const wchar_t* n,CFileStatus& t,ATL::CAtlTransactionManager* m) { return CFile::GetStatus(n,t,m); }

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
