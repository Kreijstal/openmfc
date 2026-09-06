// CMemFile — OpenMFC implementation.
// Sources: filecore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/FilecoreSupport.h"

// Symbol: ?GetPosition@CMemFile@@UEBA_KXZ
extern "C" unsigned long long MS_ABI impl__GetPosition_CMemFile__UEBA_KXZ(const CMemFile* pThis) {
    return pThis ? CMemFileAccessor::Position(pThis) : 0;
}
// Symbol: ?Abort@CMemFile@@UEAAXXZ
extern "C" void MS_ABI impl__Abort_CMemFile__UEAAXXZ(CMemFile* pThis) {
    if (!pThis) return;
    CMemFileAccessor::Position(pThis) = 0;
}
// Symbol: ?Close@CMemFile@@UEAAXXZ
extern "C" void MS_ABI impl__Close_CMemFile__UEAAXXZ(CMemFile* pThis) {
    if (!pThis) return;
    CMemFileAccessor::Position(pThis) = 0;
}
// Symbol: ?Duplicate@CMemFile@@UEBAPEAVCFile@@XZ
extern "C" CFile* MS_ABI impl__Duplicate_CMemFile__UEBAPEAVCFile__XZ(const CMemFile* pThis) {
    if (!pThis) return nullptr;
    CMemFile* pDup = new CMemFile(CMemFileAccessor::GrowBytes(pThis));
    ULONGLONG len = pThis->GetLength();
    if (len > 0) {
        pDup->SetLength(len);
        memcpy(CMemFileAccessor::Buffer(pDup), CMemFileAccessor::Buffer(pThis), static_cast<size_t>(len));
    }
    CMemFileAccessor::Position(pDup) = CMemFileAccessor::Position(pThis);
    return pDup;
}
// Symbol: ?GetBufferPtr@CMemFile@@UEAAIIIPEAPEAX0@Z
extern "C" unsigned int MS_ABI impl__GetBufferPtr_CMemFile__UEAAIIIPEAPEAX0_Z(
    CMemFile* pThis, unsigned int /*nCommand*/, unsigned int nCount, void** ppBufStart, void** ppBufMax) {
    if (!pThis) return 0;
    if (ppBufStart) {
        BYTE* buf = CMemFileAccessor::Buffer(pThis);
        UINT pos = CMemFileAccessor::Position(pThis);
        UINT size = CMemFileAccessor::BufferSize(pThis);
        *ppBufStart = (buf && pos < size) ? (void*)(buf + pos) : nullptr;
    }
    if (ppBufMax) {
        BYTE* buf = CMemFileAccessor::Buffer(pThis);
        *ppBufMax = buf ? (void*)(buf + CMemFileAccessor::BufferSize(pThis)) : nullptr;
    }
    UINT pos = CMemFileAccessor::Position(pThis);
    UINT size = CMemFileAccessor::BufferSize(pThis);
    UINT nAvail = (pos < size) ? (size - pos) : 0;
    return (nCount < nAvail) ? nCount : nAvail;
}
// Symbol: ?GetStatus@CMemFile@@QEBAHAEAUCFileStatus@@@Z
extern "C" int MS_ABI impl__GetStatus_CMemFile__QEBAHAEAUCFileStatus___Z(const CMemFile* pThis, CFileStatus& rStatus) {
    if (!pThis) return 0;
    memset(&rStatus, 0, sizeof(rStatus));
    rStatus.m_size = pThis->GetLength();
    return 1;
}
// Symbol: ?Alloc@CMemFile@@MEAAPEAE_K@Z
extern "C" unsigned char* MS_ABI impl__Alloc_CMemFile__MEAAPEAE_K_Z(CMemFile* /*pThis*/, unsigned long long nBytes) {
    return static_cast<unsigned char*>(malloc(static_cast<size_t>(nBytes)));
}
// Symbol: ?Realloc@CMemFile@@MEAAPEAEPEAE_K@Z
extern "C" unsigned char* MS_ABI impl__Realloc_CMemFile__MEAAPEAEPEAE_K_Z(
    CMemFile* /*pThis*/, unsigned char* pMem, unsigned long long nBytes) {
    return static_cast<unsigned char*>(realloc(pMem, static_cast<size_t>(nBytes)));
}
// Symbol: ?Free@CMemFile@@MEAAXPEAE@Z
extern "C" void MS_ABI impl__Free_CMemFile__MEAAXPEAE_Z(CMemFile* /*pThis*/, unsigned char* pMem) {
    free(pMem);
}
// Symbol: ?Memcpy@CMemFile@@MEAAPEAEPEAEPEBE_K@Z
extern "C" unsigned char* MS_ABI impl__Memcpy_CMemFile__MEAAPEAEPEAEPEBE_K_Z(
    CMemFile* /*pThis*/, unsigned char* pDest, unsigned char* pOrigDest, const unsigned char* pSrc, unsigned long long nBytes) {
    // pOrigDest is part of the exported ABI signature.
    (void)pOrigDest;
    if (!pDest || !pSrc) return pDest;
    return static_cast<unsigned char*>(memcpy(pDest, pSrc, static_cast<size_t>(nBytes)));
}
// Symbol: ?GrowFile@CMemFile@@MEAAX_K@Z
extern "C" void MS_ABI impl__GrowFile_CMemFile__MEAAX_K_Z(CMemFile* pThis, unsigned long long dwNewLen) {
    if (!pThis) return;
    pThis->SetLength(dwNewLen);
}
// Symbol: ?LockRange@CMemFile@@UEAAX_K0@Z
extern "C" void MS_ABI impl__LockRange_CMemFile__UEAAX_K0_Z(CMemFile* pThis, unsigned long long dwPos, unsigned long long dwCount) {
    if (!pThis) return;
    std::lock_guard<std::mutex> lock(g_collectionStateMutex);
    auto& state = EnsureCollectionState<CMemFile, CMemFileLockState>(pThis);
    AddMemFileRange(state, dwPos, dwCount);
}
// Symbol: ?UnlockRange@CMemFile@@UEAAX_K0@Z
extern "C" void MS_ABI impl__UnlockRange_CMemFile__UEAAX_K0_Z(CMemFile* pThis, unsigned long long dwPos, unsigned long long dwCount) {
    if (!pThis) return;
    std::lock_guard<std::mutex> lock(g_collectionStateMutex);
    auto* state = FindCollectionState<CMemFile, CMemFileLockState>(pThis);
    if (!state) return;
    RemoveMemFileRange(*state, dwPos, dwCount);
    if (state->ranges.empty()) {
        RemoveCollectionState<CMemFile, CMemFileLockState>(pThis);
    }
}
// Symbol: ?GetThisClass@CMemFile@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMemFile__SAPEAUCRuntimeClass__XZ() {
    return &CMemFile::classCMemFile;
}
// Symbol: ?GetRuntimeClass@CMemFile@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CMemFile__UEBAPEAUCRuntimeClass__XZ(const CMemFile* pThis) {
    (void)pThis;
    return &CMemFile::classCMemFile;
}
CMemFile::CMemFile(UINT nGrowBytes)
    : m_lpBuffer(nullptr), m_nBufferSize(0), m_nFileSize(0),
      m_nGrowBytes(nGrowBytes), m_nPosition(0), m_bAutoDelete(true)
{
    m_hFile = INVALID_HANDLE_VALUE;
}
CMemFile::CMemFile(BYTE* lpBuffer, UINT nBufferSize, UINT nGrowBytes)
    : m_lpBuffer(lpBuffer), m_nBufferSize(nBufferSize), m_nFileSize(nBufferSize),
      m_nGrowBytes(nGrowBytes), m_nPosition(0), m_bAutoDelete(false)
{
    m_hFile = INVALID_HANDLE_VALUE;
}
CMemFile::~CMemFile() {
    if (m_bAutoDelete && m_lpBuffer) {
        free(m_lpBuffer);
    }
}
UINT CMemFile::Read(void* lpBuf, UINT nCount) {
    if (!lpBuf || nCount == 0) return 0;

    UINT nAvail = (m_nPosition < m_nFileSize) ? (m_nFileSize - m_nPosition) : 0;
    UINT nRead = (nCount < nAvail) ? nCount : nAvail;

    if (nRead > 0 && m_lpBuffer) {
        memcpy(lpBuf, m_lpBuffer + m_nPosition, nRead);
        m_nPosition += nRead;
    }

    return nRead;
}
void CMemFile::Write(const void* lpBuf, UINT nCount) {
    if (!lpBuf || nCount == 0) return;

    // Grow buffer if needed
    UINT nNewSize = m_nPosition + nCount;
    if (nNewSize > m_nBufferSize) {
        UINT nGrow = (m_nGrowBytes > 0) ? m_nGrowBytes : 1024;
        UINT nNewBufSize = ((nNewSize + nGrow - 1) / nGrow) * nGrow;

        BYTE* pNewBuf = (BYTE*)realloc(m_lpBuffer, nNewBufSize);
        if (!pNewBuf) return;

        m_lpBuffer = pNewBuf;
        m_nBufferSize = nNewBufSize;
        m_bAutoDelete = true;
    }

    memcpy(m_lpBuffer + m_nPosition, lpBuf, nCount);
    m_nPosition += nCount;

    if (m_nPosition > m_nFileSize) {
        m_nFileSize = m_nPosition;
    }
}
ULONGLONG CMemFile::Seek(LONGLONG lOff, UINT nFrom) {
    LONGLONG lNewPos;
    switch (nFrom) {
        case begin:   lNewPos = lOff; break;
        case current: lNewPos = m_nPosition + lOff; break;
        case end:     lNewPos = m_nFileSize + lOff; break;
        default:      lNewPos = lOff; break;
    }

    if (lNewPos < 0) lNewPos = 0;
    m_nPosition = (UINT)lNewPos;
    return m_nPosition;
}
void CMemFile::SetLength(ULONGLONG dwNewLen) {
    UINT nNewLen = (UINT)dwNewLen;

    if (nNewLen > m_nBufferSize) {
        UINT nGrow = (m_nGrowBytes > 0) ? m_nGrowBytes : 1024;
        UINT nNewBufSize = ((nNewLen + nGrow - 1) / nGrow) * nGrow;

        BYTE* pNewBuf = (BYTE*)realloc(m_lpBuffer, nNewBufSize);
        if (!pNewBuf) return;

        m_lpBuffer = pNewBuf;
        m_nBufferSize = nNewBufSize;
        m_bAutoDelete = true;
    }

    m_nFileSize = nNewLen;
    if (m_nPosition > m_nFileSize) {
        m_nPosition = m_nFileSize;
    }
}
ULONGLONG CMemFile::GetLength() const {
    return m_nFileSize;
}
void CMemFile::Flush() {
    // Nothing to flush for memory file
}
BYTE* CMemFile::Detach() {
    BYTE* lpBuffer = m_lpBuffer;
    m_lpBuffer = nullptr;
    m_nBufferSize = 0;
    m_nFileSize = 0;
    m_nPosition = 0;
    m_bAutoDelete = true;
    return lpBuffer;
}
void CMemFile::Attach(BYTE* lpBuffer, UINT nBufferSize, UINT nGrowBytes) {
    if (m_bAutoDelete && m_lpBuffer) {
        free(m_lpBuffer);
    }
    m_lpBuffer = lpBuffer;
    m_nBufferSize = nBufferSize;
    m_nFileSize = nBufferSize;
    m_nGrowBytes = nGrowBytes;
    m_nPosition = 0;
    m_bAutoDelete = false;
}
static CRuntimeClass* AFXAPI _openmfc_gb_classCMemFile() { return &CFile::classCFile; }
CRuntimeClass CMemFile::classCMemFile = {
    "CMemFile", 88, 0xFFFF, nullptr, &_openmfc_gb_classCMemFile, nullptr, nullptr
};
