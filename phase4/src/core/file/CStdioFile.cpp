// CStdioFile — OpenMFC implementation.
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
    CStdioFile* pThis, FILE* pStream, void* /*pTM*/);
extern "C" void MS_ABI impl__CommonInit_CStdioFile__IEAAXPEB_WIPEAVCAtlTransactionManager_ATL___Z(
    CStdioFile* pThis, const wchar_t* lpszFileName, unsigned int nOpenFlags, void* /*pTM*/);















// Symbol: ?CommonBaseInit@CStdioFile@@IEAAXPEAU_iobuf@@PEAVCAtlTransactionManager@ATL@@@Z
extern "C" void MS_ABI impl__CommonBaseInit_CStdioFile__IEAAXPEAU_iobuf__PEAVCAtlTransactionManager_ATL___Z(
    CStdioFile* pThis, FILE* pStream, void* /*pTM*/) {
    if (!pThis) return;
    CStdioFileAccessor::Stream(pThis) = pStream;
    pThis->m_hFile = (void*)INVALID_HANDLE_VALUE;
    if (CStdioFileAccessor::Stream(pThis)) {
        int fd = _fileno(CStdioFileAccessor::Stream(pThis));
        if (fd >= 0) pThis->m_hFile = (void*)_get_osfhandle(fd);
    }
}
// Symbol: ?CommonInit@CStdioFile@@IEAAXPEB_WIPEAVCAtlTransactionManager@ATL@@@Z
extern "C" void MS_ABI impl__CommonInit_CStdioFile__IEAAXPEB_WIPEAVCAtlTransactionManager_ATL___Z(
    CStdioFile* pThis, const wchar_t* lpszFileName, unsigned int nOpenFlags, void* /*pTM*/) {
    OpenStdioFile(pThis, lpszFileName, nOpenFlags);
}
// Symbol: ?Open@CStdioFile@@UEAAHPEB_WIPEAVCFileException@@@Z
extern "C" int MS_ABI impl__Open_CStdioFile__UEAAHPEB_WIPEAVCFileException___Z(
    CStdioFile* pThis, const wchar_t* lpszFileName, unsigned int nOpenFlags, void* /*pException*/) {
    OpenStdioFile(pThis, lpszFileName, nOpenFlags);
    return (pThis && CStdioFileAccessor::Stream(pThis)) ? 1 : 0;
}
// Symbol: ?Open@CStdioFile@@UEAAHPEB_WIPEAVCAtlTransactionManager@ATL@@PEAVCFileException@@@Z
extern "C" int MS_ABI impl__Open_CStdioFile__UEAAHPEB_WIPEAVCAtlTransactionManager_ATL__PEAVCFileException___Z(
    CStdioFile* pThis, const wchar_t* lpszFileName, unsigned int nOpenFlags, void* pTM, void* pException) {
    (void)pTM;
    return impl__Open_CStdioFile__UEAAHPEB_WIPEAVCFileException___Z(pThis, lpszFileName, nOpenFlags, pException);
}
// Symbol: ?Abort@CStdioFile@@UEAAXXZ
extern "C" void MS_ABI impl__Abort_CStdioFile__UEAAXXZ(CStdioFile* pThis) {
    if (pThis) pThis->Close();
}
// Symbol: ?Duplicate@CStdioFile@@UEBAPEAVCFile@@XZ
extern "C" CFile* MS_ABI impl__Duplicate_CStdioFile__UEBAPEAVCFile__XZ(const CStdioFile* pThis) {
    if (!pThis || !CStdioFileAccessor::Stream(pThis)) return nullptr;
    int fd = _fileno(CStdioFileAccessor::Stream(pThis));
    if (fd < 0) return nullptr;
    int dupFd = _dup(fd);
    if (dupFd < 0) return nullptr;
    FILE* dupStream = _fdopen(dupFd, "rb+");
    if (!dupStream) return nullptr;
    CStdioFile* pDup = new CStdioFile();
    CStdioFileAccessor::Stream(pDup) = dupStream;
    pDup->m_hFile = (void*)_get_osfhandle(dupFd);
    pDup->m_strFileName = pThis->m_strFileName;
    return pDup;
}
// Symbol: ?GetLength@CStdioFile@@UEBA_KXZ
extern "C" unsigned long long MS_ABI impl__GetLength_CStdioFile__UEBA_KXZ(const CStdioFile* pThis) {
    if (!pThis || !CStdioFileAccessor::Stream(pThis)) return 0;
    FILE* stream = CStdioFileAccessor::Stream(pThis);
    __int64 cur = _ftelli64(stream);
    if (cur < 0) return 0;
    if (_fseeki64(stream, 0, SEEK_END) != 0) return 0;
    __int64 len = _ftelli64(stream);
    _fseeki64(stream, cur, SEEK_SET);
    return len > 0 ? static_cast<unsigned long long>(len) : 0;
}
// Symbol: ?GetPosition@CStdioFile@@UEBA_KXZ
extern "C" unsigned long long MS_ABI impl__GetPosition_CStdioFile__UEBA_KXZ(const CStdioFile* pThis) {
    if (!pThis || !CStdioFileAccessor::Stream(pThis)) return 0;
    __int64 pos = _ftelli64(CStdioFileAccessor::Stream(pThis));
    return pos > 0 ? static_cast<unsigned long long>(pos) : 0;
}
// Symbol: ?ReadString@CStdioFile@@UEAAHAEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@@Z
extern "C" int MS_ABI impl__ReadString_CStdioFile__UEAAHAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
    CStdioFile* pThis, CString& rString) {
    return pThis ? pThis->ReadString(rString) : 0;
}
// Symbol: ?LockRange@CStdioFile@@UEAAX_K0@Z
extern "C" void MS_ABI impl__LockRange_CStdioFile__UEAAX_K0_Z(CStdioFile* pThis, unsigned long long dwPos, unsigned long long dwCount) {
    if (!pThis || !CStdioFileAccessor::Stream(pThis)) return;
    HANDLE h = (HANDLE)_get_osfhandle(_fileno(CStdioFileAccessor::Stream(pThis)));
    if (h == INVALID_HANDLE_VALUE) return;
    OVERLAPPED ov = {};
    ov.Offset = static_cast<DWORD>(dwPos & 0xFFFFFFFFULL);
    ov.OffsetHigh = static_cast<DWORD>(dwPos >> 32);
    LockFileEx(h, LOCKFILE_EXCLUSIVE_LOCK, 0,
               static_cast<DWORD>(dwCount & 0xFFFFFFFFULL), static_cast<DWORD>(dwCount >> 32), &ov);
}
// Symbol: ?UnlockRange@CStdioFile@@UEAAX_K0@Z
extern "C" void MS_ABI impl__UnlockRange_CStdioFile__UEAAX_K0_Z(CStdioFile* pThis, unsigned long long dwPos, unsigned long long dwCount) {
    if (!pThis || !CStdioFileAccessor::Stream(pThis)) return;
    HANDLE h = (HANDLE)_get_osfhandle(_fileno(CStdioFileAccessor::Stream(pThis)));
    if (h == INVALID_HANDLE_VALUE) return;
    OVERLAPPED ov = {};
    ov.Offset = static_cast<DWORD>(dwPos & 0xFFFFFFFFULL);
    ov.OffsetHigh = static_cast<DWORD>(dwPos >> 32);
    UnlockFileEx(h, 0,
                 static_cast<DWORD>(dwCount & 0xFFFFFFFFULL), static_cast<DWORD>(dwCount >> 32), &ov);
}
// Symbol: ?GetThisClass@CStdioFile@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CStdioFile__SAPEAUCRuntimeClass__XZ() {
    return &g_classCStdioFile;
}
// Symbol: ?GetRuntimeClass@CStdioFile@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CStdioFile__UEBAPEAUCRuntimeClass__XZ(const CStdioFile* pThis) {
    (void)pThis;
    return &g_classCStdioFile;
}
CStdioFile::CStdioFile() : m_pStream(nullptr) {
    m_hFile = INVALID_HANDLE_VALUE;
}
CStdioFile::CStdioFile(const wchar_t* lpszFileName, UINT nOpenFlags) : m_pStream(nullptr) {
    m_hFile = INVALID_HANDLE_VALUE;
    if (!lpszFileName) return;

    m_strFileName = lpszFileName;

    // Build mode string
    wchar_t szMode[4] = { 0 };
    int nModeIdx = 0;

    bool bText = (nOpenFlags & typeText) != 0;

    if ((nOpenFlags & 0x0003) == modeRead) {
        szMode[nModeIdx++] = L'r';
    } else if ((nOpenFlags & 0x0003) == modeWrite) {
        szMode[nModeIdx++] = L'w';
    } else if ((nOpenFlags & 0x0003) == modeReadWrite) {
        if (nOpenFlags & modeCreate) {
            szMode[nModeIdx++] = L'w';
        } else {
            szMode[nModeIdx++] = L'r';
        }
        szMode[nModeIdx++] = L'+';
    }

    if (!bText) {
        szMode[nModeIdx++] = L'b';
    }

    m_pStream = _wfopen(lpszFileName, szMode);
}
CStdioFile::~CStdioFile() {
    if (m_pStream) {
        fclose(m_pStream);
    }
}
UINT CStdioFile::Read(void* lpBuf, UINT nCount) {
    if (!m_pStream || !lpBuf || nCount == 0) return 0;
    return (UINT)fread(lpBuf, 1, nCount, m_pStream);
}
void CStdioFile::Write(const void* lpBuf, UINT nCount) {
    if (!m_pStream || !lpBuf || nCount == 0) return;
    fwrite(lpBuf, 1, nCount, m_pStream);
}
ULONGLONG CStdioFile::Seek(LONGLONG lOff, UINT nFrom) {
    if (!m_pStream) return 0;

    int nOrigin;
    switch (nFrom) {
        case begin:   nOrigin = SEEK_SET; break;
        case current: nOrigin = SEEK_CUR; break;
        case end:     nOrigin = SEEK_END; break;
        default:      nOrigin = SEEK_SET; break;
    }

    if (_fseeki64(m_pStream, lOff, nOrigin) != 0) {
        return 0;
    }
    return _ftelli64(m_pStream);
}
void CStdioFile::Flush() {
    if (m_pStream) {
        fflush(m_pStream);
    }
}
void CStdioFile::Close() {
    if (m_pStream) {
        fclose(m_pStream);
        m_pStream = nullptr;
    }
}
wchar_t* CStdioFile::ReadString(wchar_t* lpsz, UINT nMax) {
    if (!m_pStream || !lpsz || nMax == 0) return nullptr;
    return fgetws(lpsz, nMax, m_pStream);
}
int CStdioFile::ReadString(CString& rString) {
    rString.Empty();
    if (!m_pStream) return 0;

    wchar_t buf[256];
    while (fgetws(buf, 256, m_pStream)) {
        rString += buf;
        // Check if we got a complete line
        int len = rString.GetLength();
        if (len > 0 && ((const wchar_t*)rString)[len - 1] == L'\n') {
            break;
        }
    }
    return rString.GetLength() > 0 ? 1 : 0;
}
void CStdioFile::WriteString(const wchar_t* lpsz) {
    if (!m_pStream || !lpsz) return;
    fputws(lpsz, m_pStream);
}
// CStdioFile::CStdioFile(_iobuf*, ATL::CAtlTransactionManager*) — set up an
// stdio-backed file wrapper over the supplied FILE stream using the existing
// CommonBaseInit helper. Transaction-manager behavior is not modeled, so pTM is
// currently unused.
// Symbol: ??0CStdioFile@@QEAA@PEAU_iobuf@@@Z
extern "C" void* MS_ABI impl___0CStdioFile__QEAA_PEAU_iobuf___Z(
    void* pThis, FILE* p0)
{
    if (!pThis) return nullptr;
    CStdioFile* p = new (pThis) CStdioFile();
    impl__CommonBaseInit_CStdioFile__IEAAXPEAU_iobuf__PEAVCAtlTransactionManager_ATL___Z(pThis, p0, nullptr);
    return p;
}
// CStdioFile::CStdioFile(ATL::CAtlTransactionManager*) — transaction-aware
// constructor. The transaction manager is not tracked by the wrapper state in
// openmfc, so this simply initializes the same baseline CStdioFile state and
// keeps the argument unused.
// Symbol: ??0CStdioFile@@QEAA@PEAVCAtlTransactionManager@ATL@@@Z
extern "C" void* MS_ABI impl___0CStdioFile__QEAA_PEAVCAtlTransactionManager_ATL___Z(
    void* pThis, void* pTM)
{
    if (!pThis) return nullptr;
    return impl___0CStdioFile__QEAA_PEAU_iobuf___Z(pThis, nullptr);
}
// CStdioFile::CStdioFile(const wchar_t*, UINT, ATL::CAtlTransactionManager*)
// follows the same path as CommonInit after placement construction.
// Symbol: ??0CStdioFile@@QEAA@PEB_WIPEAVCAtlTransactionManager@ATL@@@Z
extern "C" void* MS_ABI impl___0CStdioFile__QEAA_PEB_WIPEAVCAtlTransactionManager_ATL___Z(
    void* pThis, const wchar_t* p0, unsigned int p1, void* pTM)
{
    if (!pThis) return nullptr;
    CStdioFile* p = new (pThis) CStdioFile();
    impl__CommonInit_CStdioFile__IEAAXPEB_WIPEAVCAtlTransactionManager_ATL___Z(pThis, p0, p1, pTM);
    return p;
}
