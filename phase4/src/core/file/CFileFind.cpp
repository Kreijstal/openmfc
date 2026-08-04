// CFileFind — OpenMFC implementation.
// Sources: global_ctor_quickwins.cpp, inet_cfilefind_times.cpp, inetcore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/CFileFindSupport.h"
#include "detail/InetcoreSupport.h"

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















// CFileFind::CFileFind(ATL::CAtlTransactionManager*) — the transacted-file
// variant. OpenMFC's CFileFind does not model transactions, so this initializes
// identically to CFileFind::CFileFind(); the transaction-manager argument is
// accepted for ABI compatibility and otherwise unused.
// Symbol: ??0CFileFind@@QEAA@PEAVCAtlTransactionManager@ATL@@@Z
extern "C" void* MS_ABI impl___0CFileFind__QEAA_PEAVCAtlTransactionManager_ATL___Z(void* pThis, void* /*pTM*/)
{
    return new (pThis) CFileFind();
}
// ?GetCreationTime@CFileFind@@UEBAHAEAVCTime@ATL@@@Z
// int GetCreationTime(ATL::CTime& refTime) const
// Symbol: ?GetCreationTime@CFileFind@@UEBAHAEAVCTime@ATL@@@Z
extern "C" int MS_ABI impl__GetCreationTime_CFileFind__UEBAHAEAVCTime_ATL___Z(
    const void* pThis, long long* pRefTime)
{
    if (!pThis) return 0;
    return GetTime(pThis, reinterpret_cast<const CFileFindView*>(pThis)->m_findData.ftCreationTime, pRefTime);
}
// ?GetLastAccessTime@CFileFind@@UEBAHAEAVCTime@ATL@@@Z
// int GetLastAccessTime(ATL::CTime& refTime) const
// Symbol: ?GetLastAccessTime@CFileFind@@UEBAHAEAVCTime@ATL@@@Z
extern "C" int MS_ABI impl__GetLastAccessTime_CFileFind__UEBAHAEAVCTime_ATL___Z(
    const void* pThis, long long* pRefTime)
{
    if (!pThis) return 0;
    return GetTime(pThis, reinterpret_cast<const CFileFindView*>(pThis)->m_findData.ftLastAccessTime, pRefTime);
}
// ?GetLastWriteTime@CFileFind@@UEBAHAEAVCTime@ATL@@@Z
// int GetLastWriteTime(ATL::CTime& refTime) const
// Symbol: ?GetLastWriteTime@CFileFind@@UEBAHAEAVCTime@ATL@@@Z
extern "C" int MS_ABI impl__GetLastWriteTime_CFileFind__UEBAHAEAVCTime_ATL___Z(
    const void* pThis, long long* pRefTime)
{
    if (!pThis) return 0;
    return GetTime(pThis, reinterpret_cast<const CFileFindView*>(pThis)->m_findData.ftLastWriteTime, pRefTime);
}
// Symbol: ?CloseContext@CFileFind@@MEAAXXZ
extern "C" void MS_ABI impl__CloseContext_CFileFind__MEAAXXZ(CFileFind* pThis) {
    if (pThis) pThis->Close();
}
// Symbol: ?FindNextFileW@CFileFind@@UEAAHXZ
extern "C" int MS_ABI impl__FindNextFileW_CFileFind__UEAAHXZ(CFileFind* pThis) {
    return pThis ? pThis->FindNextFile() : FALSE;
}
// Symbol: ?GetFileName@CFileFind@@UEBA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@XZ
extern "C" void MS_ABI impl__GetFileName_CFileFind__UEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__XZ(
    CString* pRet, const CFileFind* pThis) {
    openmfcConstructString(pRet, pThis ? pThis->GetFileName() : CString());
}
// Symbol: ?GetFilePath@CFileFind@@UEBA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@XZ
extern "C" void MS_ABI impl__GetFilePath_CFileFind__UEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__XZ(
    CString* pRet, const CFileFind* pThis) {
    openmfcConstructString(pRet, pThis ? pThis->GetFilePath() : CString());
}
// Symbol: ?GetFileTitle@CFileFind@@UEBA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@XZ
extern "C" void MS_ABI impl__GetFileTitle_CFileFind__UEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__XZ(
    CString* pRet, const CFileFind* pThis) {
    openmfcConstructString(pRet, pThis ? pThis->GetFileTitle() : CString());
}
// Symbol: ?GetFileURL@CFileFind@@UEBA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@XZ
extern "C" void MS_ABI impl__GetFileURL_CFileFind__UEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__XZ(
    CString* pRet, const CFileFind* pThis) {
    openmfcConstructString(pRet, pThis ? pThis->GetFileURL() : CString());
}
// Symbol: ?GetRoot@CFileFind@@UEBA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@XZ
extern "C" void MS_ABI impl__GetRoot_CFileFind__UEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__XZ(
    CString* pRet, const CFileFind* pThis) {
    openmfcConstructString(pRet, pThis ? pThis->GetRoot() : CString());
}
// Symbol: ?GetRuntimeClass@CFileFind@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CFileFind__UEBAPEAUCRuntimeClass__XZ(const CFileFind* pThis) {
    return pThis ? pThis->GetRuntimeClass() : CFileFind::GetThisClass();
}
// Symbol: ?GetThisClass@CFileFind@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CFileFind__SAPEAUCRuntimeClass__XZ() {
    return CFileFind::GetThisClass();
}
CFileFind::CFileFind()
    : m_hFindFile(INVALID_HANDLE_VALUE), m_bGotFirst(0) {
    memset(&m_findData, 0, sizeof(m_findData));
    memset(_filefind_padding, 0, sizeof(_filefind_padding));
}
CFileFind::~CFileFind() { Close(); }
int CFileFind::FindFile(const wchar_t* pstrName, DWORD dwUnused) {
    (void)dwUnused;
    Close();
    m_strRoot.Empty();
    if (!pstrName) return 0;
    if (pstrName && *pstrName) {
        const wchar_t* lastSlash = wcsrchr(pstrName, L'\\');
        const wchar_t* lastForward = wcsrchr(pstrName, L'/');
        const wchar_t* split = lastSlash;
        if (!split || (lastForward && lastForward > split)) {
            split = lastForward;
        }
        if (split) {
            m_strRoot = CString(pstrName).Left((int)(split - pstrName) + 1);
        }
    }
    m_hFindFile = ::FindFirstFileW(pstrName, &m_findData);
    if (m_hFindFile == INVALID_HANDLE_VALUE) return 0;
    m_bGotFirst = 1;
    m_strFileName = m_findData.cFileName;
    g_lastFileName = m_strFileName;
    return 1;
}
int CFileFind::FindNextFile() {
    if (m_hFindFile == INVALID_HANDLE_VALUE) return 0;
    if (m_bGotFirst) {
        m_bGotFirst = 0;
        m_strFileName = m_findData.cFileName;
        return 1;
    }
    if (!::FindNextFileW(m_hFindFile, &m_findData)) return 0;
    m_strFileName = m_findData.cFileName;
    g_lastFileName = m_strFileName;
    return 1;
}
void CFileFind::Close() {
    if (m_hFindFile != INVALID_HANDLE_VALUE) {
        ::FindClose(m_hFindFile);
        m_hFindFile = INVALID_HANDLE_VALUE;
    }
    m_bGotFirst = 0;
}
int CFileFind::IsDirectory() const { return (m_findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0; }
int CFileFind::IsDots() const { return m_strFileName == L"." || m_strFileName == L".."; }
int CFileFind::IsArchived() const { return (m_findData.dwFileAttributes & FILE_ATTRIBUTE_ARCHIVE) != 0; }
int CFileFind::IsCompressed() const { return (m_findData.dwFileAttributes & FILE_ATTRIBUTE_COMPRESSED) != 0; }
int CFileFind::IsHidden() const { return (m_findData.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN) != 0; }
int CFileFind::IsNormal() const { return m_findData.dwFileAttributes == FILE_ATTRIBUTE_NORMAL; }
int CFileFind::IsReadOnly() const { return (m_findData.dwFileAttributes & FILE_ATTRIBUTE_READONLY) != 0; }
int CFileFind::IsSystem() const { return (m_findData.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM) != 0; }
int CFileFind::IsTemporary() const { return (m_findData.dwFileAttributes & FILE_ATTRIBUTE_TEMPORARY) != 0; }
int CFileFind::MatchesMask(DWORD dwMask) const { return (m_findData.dwFileAttributes & dwMask) != 0; }
ULONGLONG CFileFind::GetLength() const { return ((ULONGLONG)m_findData.nFileSizeHigh << 32) | m_findData.nFileSizeLow; }
CString CFileFind::GetFileName() const { return m_strFileName; }
CString CFileFind::GetFilePath() const { return m_strRoot + m_strFileName; }
CString CFileFind::GetFileTitle() const { 
    CString s = m_strFileName;
    int pos = s.ReverseFind(L'.');
    return (pos > 0) ? s.Left(pos) : s;
}
CString CFileFind::GetFileURL() const { return L"file://" + GetFilePath(); }
CString CFileFind::GetRoot() const { return m_strRoot; }
int CFileFind::GetLastWriteTime(FILETIME* pFileTime) const {
    if (pFileTime) { *pFileTime = m_findData.ftLastWriteTime; return 1; }
    return 0;
}
int CFileFind::GetLastAccessTime(FILETIME* pFileTime) const {
    if (pFileTime) { *pFileTime = m_findData.ftLastAccessTime; return 1; }
    return 0;
}
int CFileFind::GetCreationTime(FILETIME* pFileTime) const {
    if (pFileTime) { *pFileTime = m_findData.ftCreationTime; return 1; }
    return 0;
}
int CFileFind::GetFileName(CString& strFileName) {
    strFileName = g_lastFileName;
    return static_cast<int>(!strFileName.IsEmpty());
}
