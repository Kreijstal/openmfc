// CSocketFile — OpenMFC implementation.
// Sources: sockcore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/SockcoreSupport.h"

// Symbol: ??0CSocketFile@@QEAA@PEAVCSocket@@H@Z
extern "C" void* MS_ABI impl___0CSocketFile__QEAA_PEAVCSocket__H_Z(
        CSocketFile* pThis, CSocket* pSocket, int bArchiveCompatible) {
    new(pThis) CSocketFile(pSocket, bArchiveCompatible);
    return pThis;
}
// Symbol: ?Flush@CSocketFile@@UEAAXXZ
extern "C" void MS_ABI impl__Flush_CSocketFile__UEAAXXZ(CSocketFile* pThis) {
    pThis->Flush();
}
// Symbol: ?Abort@CSocketFile@@UEAAXXZ
extern "C" void MS_ABI impl__Abort_CSocketFile__UEAAXXZ(CSocketFile* pThis) {
    pThis->Abort();
}
// Symbol: ?GetLength@CSocketFile@@UEBA_KXZ
extern "C" unsigned long long MS_ABI impl__GetLength_CSocketFile__UEBA_KXZ(
        const CSocketFile* pThis) {
    return pThis->GetLength();
}
// Symbol: ?GetPosition@CSocketFile@@UEBA_KXZ
extern "C" unsigned long long MS_ABI impl__GetPosition_CSocketFile__UEBA_KXZ(
        const CSocketFile* pThis) {
    return pThis->GetPosition();
}
// Symbol: ?SetLength@CSocketFile@@UEAAX_K@Z
extern "C" void MS_ABI impl__SetLength_CSocketFile__UEAAX_K_Z(
        CSocketFile* pThis, unsigned long long dwNewLen) {
    pThis->SetLength(dwNewLen);
}
// Symbol: ?Duplicate@CSocketFile@@UEBAPEAVCFile@@XZ
extern "C" CFile* MS_ABI impl__Duplicate_CSocketFile__UEBAPEAVCFile__XZ(
        const CSocketFile* pThis) {
    return pThis->Duplicate();
}
// Symbol: ?GetBufferPtr@CSocketFile@@UEAAIIIPEAPEAX0@Z
extern "C" unsigned int MS_ABI impl__GetBufferPtr_CSocketFile__UEAAIIIPEAPEAX0_Z(
        CSocketFile* pThis, unsigned int nCommand, unsigned int nCount,
        void** ppBufStart, void** ppBufMax) {
    return pThis->GetBufferPtr(nCommand, nCount, ppBufStart, ppBufMax);
}
// Symbol: ?LockRange@CSocketFile@@UEAAX_K0@Z
extern "C" void MS_ABI impl__LockRange_CSocketFile__UEAAX_K0_Z(
        CSocketFile* pThis, unsigned long long dwPos, unsigned long long dwCount) {
    pThis->LockRange(dwPos, dwCount);
}
// Symbol: ?UnlockRange@CSocketFile@@UEAAX_K0@Z
extern "C" void MS_ABI impl__UnlockRange_CSocketFile__UEAAX_K0_Z(
        CSocketFile* pThis, unsigned long long dwPos, unsigned long long dwCount) {
    pThis->UnlockRange(dwPos, dwCount);
}
// Symbol: ?Open@CSocketFile@@UEAAHPEB_WIPEAVCFileException@@@Z
extern "C" int MS_ABI impl__Open_CSocketFile__UEAAHPEB_WIPEAVCFileException___Z(
        CSocketFile* pThis, const wchar_t* lpszFileName,
        unsigned int nOpenFlags, CFileException* pError) {
    return pThis->Open(lpszFileName, nOpenFlags, pError);
}
// Symbol: ?GetRuntimeClass@CSocketFile@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CSocketFile__UEBAPEAUCRuntimeClass__XZ(
        const CSocketFile* pThis) {
    if (!pThis) return &CSocketFile::classCSocketFile;
    return CSocketFile::GetThisClass();
}
// Symbol: ?GetThisClass@CSocketFile@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CSocketFile__SAPEAUCRuntimeClass__XZ() {
    return CSocketFile::GetThisClass();
}
CRuntimeClass CSocketFile::classCSocketFile = {
    "CSocketFile",
    sizeof(CSocketFile),
    0xFFFF,
    nullptr,
    nullptr,
    nullptr,
    nullptr
};
CSocketFile::CSocketFile(CSocket* pSocket, int bArchiveCompatible)
    : CFile(), m_pSocket(pSocket), m_bArchiveCompatible(bArchiveCompatible)
{
    memset(_socketfile_padding, 0, sizeof(_socketfile_padding));
}
CSocketFile::~CSocketFile() {
}
UINT CSocketFile::Read(void* lpBuf, UINT nCount) {
    if (!m_pSocket) return 0;
    int result = m_pSocket->Receive(lpBuf, (int)nCount);
    return (result == SOCKET_ERROR) ? 0 : (UINT)result;
}
void CSocketFile::Write(const void* lpBuf, UINT nCount) {
    if (m_pSocket) {
        m_pSocket->Send(lpBuf, (int)nCount);
    }
}
void CSocketFile::Close() {
    // Detach only - don't close the underlying socket
}
ULONGLONG CSocketFile::Seek(LONGLONG lOff, UINT nFrom) {
    (void)lOff; (void)nFrom;
    return (ULONGLONG)-1;
}
void CSocketFile::Flush() { /* no-op: no buffering */ }
void CSocketFile::Abort() {
    if (m_pSocket) {
        m_pSocket->Close();
    }
}
ULONGLONG CSocketFile::GetLength() const { return (ULONGLONG)-1; }
ULONGLONG CSocketFile::GetPosition() const { return (ULONGLONG)-1; }
void CSocketFile::SetLength(ULONGLONG dwNewLen) { (void)dwNewLen; }
CFile* CSocketFile::Duplicate() const {
    if (!m_pSocket) return nullptr;
    return new CSocketFile(m_pSocket, m_bArchiveCompatible);
}
UINT CSocketFile::GetBufferPtr(UINT nCommand, UINT nCount, void** ppBufStart, void** ppBufMax) {
    (void)nCommand; (void)nCount;
    if (ppBufStart) *ppBufStart = nullptr;
    if (ppBufMax) *ppBufMax = nullptr;
    return 0;
}
void CSocketFile::LockRange(ULONGLONG dwPos, ULONGLONG dwCount) {
    (void)dwPos; (void)dwCount;
}
void CSocketFile::UnlockRange(ULONGLONG dwPos, ULONGLONG dwCount) {
    (void)dwPos; (void)dwCount;
}
int CSocketFile::Open(const wchar_t* lpszFileName, UINT nOpenFlags, CFileException* pError) {
    (void)lpszFileName; (void)nOpenFlags; (void)pError;
    return FALSE;
}
