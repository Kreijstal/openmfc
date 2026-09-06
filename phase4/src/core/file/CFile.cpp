// CFile — OpenMFC implementation.
// Sources: filecore.cpp, global_ctor_quickwins.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/FilecoreSupport.h"

// Hand-written real implementations for header-defined classes that were one
// export short of complete. Constructors placement-new into the caller-provided
// storage so the openmfc C++ constructor establishes the validated member
// layout. Kept in a global_*.cpp file so the build's AUTO_EXCLUDES pass (scan of
// `// Symbol:` comments) removes the weak/typed stubs for these ordinals
// automatically.

#include "openmfc/afxwin.h"
#include "openmfc/afxinet.h"
#include "openmfc/afxdisp.h"
#include "openmfc/afxole.h"

#include <cstdio>
#include <new>
#include <cstdlib>
#include <cstring>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

// Runtime-class descriptors used by COleException::Process, defined alongside
// the exception RTTI tables (ole_oleexception_rtti.cpp,
// global_simple_exceptions_rtti.cpp).
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_COleException__SAPEAUCRuntimeClass__XZ();
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMemoryException__SAPEAUCRuntimeClass__XZ();
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CNotSupportedException__SAPEAUCRuntimeClass__XZ();
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CInvalidArgException__SAPEAUCRuntimeClass__XZ();

extern "C" void MS_ABI impl__CommonInit_CFile__IEAAXPEB_WIPEAVCAtlTransactionManager_ATL___Z(
    void* pThis, const wchar_t* lpszFileName, unsigned int nOpenFlags, void* pTM);
extern "C" void MS_ABI impl__CommonBaseInit_CStdioFile__IEAAXPEAU_iobuf__PEAVCAtlTransactionManager_ATL___Z(
    void* pThis, FILE* pStream, void* pTM);
extern "C" void MS_ABI impl__CommonInit_CStdioFile__IEAAXPEB_WIPEAVCAtlTransactionManager_ATL___Z(
    void* pThis, const wchar_t* lpszFileName, unsigned int nOpenFlags, void* pTM);















// Symbol: ?GetLength@CFile@@UEBA_KXZ
extern "C" unsigned long long MS_ABI impl__GetLength_CFile__UEBA_KXZ(void* pThis) {
    CFile* self = static_cast<CFile*>(pThis);
    if (!self || self->m_hFile == (void*)INVALID_HANDLE_VALUE) {
        return 0;
    }
    LARGE_INTEGER li;
    li.QuadPart = 0;
    if (GetFileSizeEx((HANDLE)self->m_hFile, &li))
        return li.QuadPart;
    return 0;
}
// Symbol: ?Seek@CFile@@UEAA_K_JI@Z
extern "C" unsigned long long MS_ABI impl__Seek_CFile__UEAA_K_JI_Z(void* pThis, long long lOff, unsigned int nFrom) {
    CFile* self = static_cast<CFile*>(pThis);
    if (!self || self->m_hFile == (void*)INVALID_HANDLE_VALUE) {
        return 0;
    }
    LARGE_INTEGER li;
    li.QuadPart = lOff;
    LARGE_INTEGER result;
    if (SetFilePointerEx((HANDLE)self->m_hFile, li, &result, nFrom))
        return result.QuadPart;
    return 0;
}
// Symbol: ?Read@CFile@@UEAAIPEAXI@Z
extern "C" unsigned int MS_ABI impl__Read_CFile__UEAAIPEAXI_Z(void* pThis, void* lpBuf, unsigned int nCount) {
    CFile* self = static_cast<CFile*>(pThis);
    if (!self || self->m_hFile == (void*)INVALID_HANDLE_VALUE || !lpBuf) {
        return 0;
    }
    if (nCount == 0) {
        return 0;
    }
    DWORD dwRead = 0;
    ReadFile((HANDLE)self->m_hFile, lpBuf, nCount, &dwRead, nullptr);
    return dwRead;
}
// Symbol: ?Write@CFile@@UEAAXPEBXI@Z
extern "C" void MS_ABI impl__Write_CFile__UEAAXPEBXI_Z(void* pThis, const void* lpBuf, unsigned int nCount) {
    CFile* self = static_cast<CFile*>(pThis);
    if (!self || self->m_hFile == (void*)INVALID_HANDLE_VALUE || !lpBuf) {
        return;
    }
    if (nCount == 0) {
        return;
    }
    DWORD dwWritten = 0;
    WriteFile((HANDLE)self->m_hFile, lpBuf, nCount, &dwWritten, nullptr);
}
// Symbol: ?GetPosition@CFile@@UEBA_KXZ
extern "C" unsigned long long MS_ABI impl__GetPosition_CFile__UEBA_KXZ(void* pThis) {
    CFile* self = static_cast<CFile*>(pThis);
    if (!self || self->m_hFile == (void*)INVALID_HANDLE_VALUE) {
        return 0;
    }
    LARGE_INTEGER li, result;
    li.QuadPart = 0;
    if (SetFilePointerEx((HANDLE)self->m_hFile, li, &result, FILE_CURRENT))
        return result.QuadPart;
    return 0;
}
// Symbol: ?Flush@CFile@@UEAAXXZ
extern "C" void MS_ABI impl__Flush_CFile__UEAAXXZ(void* pThis) {
    CFile* self = static_cast<CFile*>(pThis);
    if (!self || self->m_hFile == (void*)INVALID_HANDLE_VALUE) {
        return;
    }
    FlushFileBuffers((HANDLE)self->m_hFile);
}
// Symbol: ?Close@CFile@@UEAAXXZ
extern "C" void MS_ABI impl__Close_CFile__UEAAXXZ(void* pThis) {
    CFile* self = static_cast<CFile*>(pThis);
    if (self->m_hFile != (void*)INVALID_HANDLE_VALUE) {
        CloseHandle((HANDLE)self->m_hFile);
        self->m_hFile = (void*)INVALID_HANDLE_VALUE;
    }
}
// Symbol: ?Abort@CFile@@UEAAXXZ
extern "C" void MS_ABI impl__Abort_CFile__UEAAXXZ(void* pThis) {
    CFile* self = static_cast<CFile*>(pThis);
    if (self->m_hFile != (void*)INVALID_HANDLE_VALUE) {
        CloseHandle((HANDLE)self->m_hFile);
        self->m_hFile = (void*)INVALID_HANDLE_VALUE;
    }
}
// Symbol: ?GetFileName@CFile@@UEBA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@XZ
extern "C" void MS_ABI impl__GetFileName_CFile__UEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__XZ(void* pThis, void* ret) {
    CFile* self = static_cast<CFile*>(pThis);
    new(ret) CString(self->GetFileName());
}
// Symbol: ?Duplicate@CFile@@UEBAPEAV1@XZ
extern "C" void* MS_ABI impl__Duplicate_CFile__UEBAPEAV1_XZ(void* pThis) {
    CFile* self = static_cast<CFile*>(pThis);
    CFile* pDup = new CFile();
    HANDLE hDup = INVALID_HANDLE_VALUE;
    if (self->m_hFile != (void*)INVALID_HANDLE_VALUE) {
        HANDLE hProc = GetCurrentProcess();
        DuplicateHandle(hProc, (HANDLE)self->m_hFile, hProc, &hDup, 0, FALSE, DUPLICATE_SAME_ACCESS);
    }
    pDup->m_hFile = hDup;
    return pDup;
}
// Symbol: ?GetFilePath@CFile@@UEBA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@XZ
extern "C" void MS_ABI impl__GetFilePath_CFile__UEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__XZ(void* pThis, void* ret) {
    CFile* self = static_cast<CFile*>(pThis);
    new(ret) CString(self->GetFilePath());
}
// Symbol: ?GetFileTitle@CFile@@UEBA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@XZ
extern "C" void MS_ABI impl__GetFileTitle_CFile__UEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__XZ(void* pThis, void* ret) {
    CFile* self = static_cast<CFile*>(pThis);
    new(ret) CString(self->GetFileTitle());
}
// Symbol: ?GetBufferPtr@CFile@@UEAAIIIPEAPEAX0@Z
extern "C" unsigned int MS_ABI impl__GetBufferPtr_CFile__UEAAIIIPEAPEAX0_Z(
    void* /*pThis*/, unsigned int /*nCommand*/, unsigned int /*nCount*/, void** ppBufStart, void** ppBufMax) {
    if (ppBufStart) *ppBufStart = nullptr;
    if (ppBufMax) *ppBufMax = nullptr;
    return 0;
}
// Symbol: ?CommonBaseInit@CFile@@IEAAXPEAXPEAVCAtlTransactionManager@ATL@@@Z
extern "C" void MS_ABI impl__CommonBaseInit_CFile__IEAAXPEAXPEAVCAtlTransactionManager_ATL___Z(
    void* pThis, void* hFile, void* /*pTM*/) {
    CFile* self = static_cast<CFile*>(pThis);
    self->m_hFile = hFile ? hFile : (void*)INVALID_HANDLE_VALUE;
}
// Symbol: ?CommonInit@CFile@@IEAAXPEB_WIPEAVCAtlTransactionManager@ATL@@@Z
extern "C" void MS_ABI impl__CommonInit_CFile__IEAAXPEB_WIPEAVCAtlTransactionManager_ATL___Z(
    void* pThis, const wchar_t* lpszFileName, unsigned int nOpenFlags, void* pTM) {
    if (!pThis) {
        return;
    }
    CFileAccessor::InvokeCommonInit(static_cast<CFile*>(pThis), lpszFileName, nOpenFlags, pTM);
}
// Symbol: ?Rename@CFile@@SAXPEB_W0PEAVCAtlTransactionManager@ATL@@@Z
extern "C" void MS_ABI impl__Rename_CFile__SAXPEB_W0PEAVCAtlTransactionManager_ATL___Z(
    const wchar_t* lpszOldName, const wchar_t* lpszNewName, void* /*pTM*/) {
    CFile::Rename(lpszOldName, lpszNewName);
}
// Symbol: ?Remove@CFile@@SAXPEB_WPEAVCAtlTransactionManager@ATL@@@Z
extern "C" void MS_ABI impl__Remove_CFile__SAXPEB_WPEAVCAtlTransactionManager_ATL___Z(
    const wchar_t* lpszFileName, void* /*pTM*/) {
    CFile::Remove(lpszFileName);
}
// Symbol: ?SetStatus@CFile@@SAXPEB_WAEBUCFileStatus@@PEAVCAtlTransactionManager@ATL@@@Z
extern "C" void MS_ABI impl__SetStatus_CFile__SAXPEB_WAEBUCFileStatus__PEAVCAtlTransactionManager_ATL___Z(
    const wchar_t* lpszFileName, const CFileStatus* pStatus, void* /*pTM*/) {
    if (!pStatus) {
        return;
    }
    CFile::SetStatus(lpszFileName, *pStatus);
}
// Symbol: ?Open@CFile@@UEAAHPEB_WIPEAVCFileException@@@Z
extern "C" int MS_ABI impl__Open_CFile__UEAAHPEB_WIPEAVCFileException___Z(
    void* pThis, const wchar_t* lpszFileName, unsigned int nOpenFlags, void* /*pException*/) {
    CFile* self = static_cast<CFile*>(pThis);
    CFileAccessor::InvokeCommonInit(self, lpszFileName, nOpenFlags, nullptr);
    return self->m_hFile != (void*)INVALID_HANDLE_VALUE ? 1 : 0;
}
// Symbol: ?Open@CFile@@UEAAHPEB_WIPEAVCAtlTransactionManager@ATL@@PEAVCFileException@@@Z
extern "C" int MS_ABI impl__Open_CFile__UEAAHPEB_WIPEAVCAtlTransactionManager_ATL__PEAVCFileException___Z(
    void* pThis, const wchar_t* lpszFileName, unsigned int nOpenFlags, void* pTM, void* pException) {
    (void)pTM;
    return impl__Open_CFile__UEAAHPEB_WIPEAVCFileException___Z(pThis, lpszFileName, nOpenFlags, pException);
}
// Symbol: ?LockRange@CFile@@UEAAX_K0@Z
extern "C" void MS_ABI impl__LockRange_CFile__UEAAX_K0_Z(void* pThis, unsigned long long dwPos, unsigned long long dwCount) {
    CFile* self = static_cast<CFile*>(pThis);
    if (self->m_hFile == (void*)INVALID_HANDLE_VALUE) return;
    OVERLAPPED ov = {};
    ov.Offset = static_cast<DWORD>(dwPos & 0xFFFFFFFFULL);
    ov.OffsetHigh = static_cast<DWORD>(dwPos >> 32);
    LockFileEx((HANDLE)self->m_hFile, LOCKFILE_EXCLUSIVE_LOCK, 0,
               static_cast<DWORD>(dwCount & 0xFFFFFFFFULL), static_cast<DWORD>(dwCount >> 32), &ov);
}
// Symbol: ?UnlockRange@CFile@@UEAAX_K0@Z
extern "C" void MS_ABI impl__UnlockRange_CFile__UEAAX_K0_Z(void* pThis, unsigned long long dwPos, unsigned long long dwCount) {
    CFile* self = static_cast<CFile*>(pThis);
    if (self->m_hFile == (void*)INVALID_HANDLE_VALUE) return;
    OVERLAPPED ov = {};
    ov.Offset = static_cast<DWORD>(dwPos & 0xFFFFFFFFULL);
    ov.OffsetHigh = static_cast<DWORD>(dwPos >> 32);
    UnlockFileEx((HANDLE)self->m_hFile, 0,
                 static_cast<DWORD>(dwCount & 0xFFFFFFFFULL), static_cast<DWORD>(dwCount >> 32), &ov);
}
// Symbol: ?GetStatus@CFile@@SAHPEB_WAEAUCFileStatus@@PEAVCAtlTransactionManager@ATL@@@Z
extern "C" int MS_ABI impl__GetStatus_CFile__SAHPEB_WAEAUCFileStatus__PEAVCAtlTransactionManager_ATL___Z(
    const wchar_t* lpszFileName, CFileStatus& rStatus, void* pTM) {
    return CFile::GetStatus(lpszFileName, rStatus, pTM);
}
// Symbol: ?GetThisClass@CFile@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CFile__SAPEAUCRuntimeClass__XZ() {
    return &CFile::classCFile;
}
// Symbol: ?GetRuntimeClass@CFile@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CFile__UEBAPEAUCRuntimeClass__XZ(const void* pThis) {
    (void)pThis;
    return &CFile::classCFile;
}
// Symbol: ?hFileNull@CFile@@2QEAXEA
#ifdef __GNUC__
asm(".globl \"?hFileNull@CFile@@2QEAXEA\"\n"
    ".set \"?hFileNull@CFile@@2QEAXEA\", impl__hFileNull_CFile__2QEAXEA\n");
#endif

CFile::CFile() : m_hFile(INVALID_HANDLE_VALUE) {
}
CFile::CFile(HANDLE hFile) : m_hFile(hFile) {
}
CFile::CFile(const wchar_t* lpszFileName, UINT nOpenFlags) : m_hFile(INVALID_HANDLE_VALUE) {
    CommonInit(lpszFileName, nOpenFlags, nullptr);
}
CFile::~CFile() {
    if (m_hFile != INVALID_HANDLE_VALUE) {
        Close();
    }
}
void CFile::CommonInit(const wchar_t* lpszFileName, UINT nOpenFlags, void* /*pTM*/) {
    if (!lpszFileName) return;

    m_strFileName = lpszFileName;

    // Map MFC flags to Win32 flags
    DWORD dwAccess = 0;
    DWORD dwShare = 0;
    DWORD dwCreate = OPEN_EXISTING;

    // Access mode
    if ((nOpenFlags & 0x0003) == modeRead) {
        dwAccess = GENERIC_READ;
    } else if ((nOpenFlags & 0x0003) == modeWrite) {
        dwAccess = GENERIC_WRITE;
    } else if ((nOpenFlags & 0x0003) == modeReadWrite) {
        dwAccess = GENERIC_READ | GENERIC_WRITE;
    }

    // Share mode
    if ((nOpenFlags & 0x0070) == shareExclusive) {
        dwShare = 0;
    } else if ((nOpenFlags & 0x0070) == shareDenyWrite) {
        dwShare = FILE_SHARE_READ;
    } else if ((nOpenFlags & 0x0070) == shareDenyRead) {
        dwShare = FILE_SHARE_WRITE;
    } else if ((nOpenFlags & 0x0070) == shareDenyNone) {
        dwShare = FILE_SHARE_READ | FILE_SHARE_WRITE;
    } else {
        dwShare = FILE_SHARE_READ | FILE_SHARE_WRITE;  // shareCompat
    }

    // Creation disposition
    if (nOpenFlags & modeCreate) {
        if (nOpenFlags & modeNoTruncate) {
            dwCreate = OPEN_ALWAYS;
        } else {
            dwCreate = CREATE_ALWAYS;
        }
    } else {
        dwCreate = OPEN_EXISTING;
    }

    m_hFile = CreateFileW(
        lpszFileName,
        dwAccess,
        dwShare,
        nullptr,
        dwCreate,
        FILE_ATTRIBUTE_NORMAL,
        nullptr
    );
}
UINT CFile::Read(void* lpBuf, UINT nCount) {
    if (m_hFile == INVALID_HANDLE_VALUE || !lpBuf || nCount == 0) {
        return 0;
    }

    DWORD dwRead = 0;
    if (!ReadFile(m_hFile, lpBuf, nCount, &dwRead, nullptr)) {
        return 0;
    }
    return dwRead;
}
void CFile::Write(const void* lpBuf, UINT nCount) {
    if (m_hFile == INVALID_HANDLE_VALUE || !lpBuf || nCount == 0) {
        return;
    }

    DWORD dwWritten = 0;
    WriteFile(m_hFile, lpBuf, nCount, &dwWritten, nullptr);
}
ULONGLONG CFile::Seek(LONGLONG lOff, UINT nFrom) {
    if (m_hFile == INVALID_HANDLE_VALUE) {
        return 0;
    }

    DWORD dwMethod;
    switch (nFrom) {
        case begin:   dwMethod = FILE_BEGIN; break;
        case current: dwMethod = FILE_CURRENT; break;
        case end:     dwMethod = FILE_END; break;
        default:      dwMethod = FILE_BEGIN; break;
    }

    LARGE_INTEGER liOff;
    liOff.QuadPart = lOff;
    LARGE_INTEGER liNew;
    if (!SetFilePointerEx(m_hFile, liOff, &liNew, dwMethod)) {
        return 0;
    }
    return liNew.QuadPart;
}
void CFile::SetLength(ULONGLONG dwNewLen) {
    if (m_hFile == INVALID_HANDLE_VALUE) {
        return;
    }

    LARGE_INTEGER liPos;
    liPos.QuadPart = dwNewLen;
    SetFilePointerEx(m_hFile, liPos, nullptr, FILE_BEGIN);
    SetEndOfFile(m_hFile);
}
ULONGLONG CFile::GetLength() const {
    if (m_hFile == INVALID_HANDLE_VALUE) {
        return 0;
    }

    LARGE_INTEGER liSize;
    if (!GetFileSizeEx(m_hFile, &liSize)) {
        return 0;
    }
    return liSize.QuadPart;
}
void CFile::Flush() {
    if (m_hFile != INVALID_HANDLE_VALUE) {
        FlushFileBuffers(m_hFile);
    }
}
void CFile::Close() {
    if (m_hFile != INVALID_HANDLE_VALUE) {
        CloseHandle(m_hFile);
        m_hFile = INVALID_HANDLE_VALUE;
    }
}
void CFile::SetFilePath(const wchar_t* lpszNewName) {
    m_strFileName = lpszNewName ? lpszNewName : L"";
}
CString CFile::GetFileName() const {
    // Extract filename from path
    const wchar_t* pPath = (const wchar_t*)m_strFileName;
    const wchar_t* pName = pPath;
    while (*pPath) {
        if (*pPath == L'\\' || *pPath == L'/') {
            pName = pPath + 1;
        }
        pPath++;
    }
    return CString(pName);
}
CString CFile::GetFileTitle() const {
    CString strName = GetFileName();
    // Remove extension
    int nDot = strName.ReverseFind(L'.');
    if (nDot >= 0) {
        return strName.Left(nDot);
    }
    return strName;
}
CString CFile::GetFilePath() const {
    return m_strFileName;
}
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
static CRuntimeClass* AFXAPI _openmfc_gb_classCFile() { return &CObject::classCObject; }
CRuntimeClass CFile::classCFile = {
    "CFile", 40, 0xFFFF, nullptr, &_openmfc_gb_classCFile, nullptr, nullptr
};
// CFile::CFile(ATL::CAtlTransactionManager*) — transaction-aware constructor.
// There is no file handle until Open(...) is called, so initialize a default
// CFile and keep the transaction pointer unused.
// Symbol: ??0CFile@@QEAA@PEAVCAtlTransactionManager@ATL@@@Z
extern "C" void* MS_ABI impl___0CFile__QEAA_PEAVCAtlTransactionManager_ATL___Z(void* pThis, void* /*pTM*/)
{
    return pThis ? new (pThis) CFile() : nullptr;
}
// CFile::CFile(const wchar_t*, UINT, ATL::CAtlTransactionManager*) — route through
// the existing CommonInit helper so behavior matches the normal constructor/Open
// path with the same transaction-manager signature.
// Symbol: ??0CFile@@QEAA@PEB_WIPEAVCAtlTransactionManager@ATL@@@Z
extern "C" void* MS_ABI impl___0CFile__QEAA_PEB_WIPEAVCAtlTransactionManager_ATL___Z(
    void* pThis, const wchar_t* p0, unsigned int p1, void* pTM)
{
    if (!pThis) return nullptr;
    CFile* p = new(pThis) CFile();
    impl__CommonInit_CFile__IEAAXPEB_WIPEAVCAtlTransactionManager_ATL___Z(pThis, p0, p1, pTM);
    return p;
}
