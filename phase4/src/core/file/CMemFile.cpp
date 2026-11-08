// CMemFile storage and dispatch use the Windows/MSVC object model.
#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxwin.h"
#include <algorithm>
#include <cerrno>
#include <cstdlib>
#include <cstring>
#include <limits>

extern "C" void impl__AfxThrowFileException__YAXHJPEB_W_Z(int,long,const wchar_t*);
extern "C" void impl__AfxThrowNotSupportedException__YAXXZ();

CMemFile::CMemFile(UINT grow)
    : m_nGrowBytes(grow), m_nPosition(0), m_nBufferSize(0), m_nFileSize(0),
      m_lpBuffer(nullptr), m_bAutoDelete(TRUE) {}
CMemFile::CMemFile(BYTE* buffer, UINT capacity, UINT grow) {
    Attach(buffer, capacity, grow);
}
CMemFile::~CMemFile() {
    if (m_lpBuffer) CMemFile::Close();
    m_nGrowBytes = m_nPosition = m_nBufferSize = m_nFileSize = 0;
}
void CMemFile::Attach(BYTE* buffer, UINT capacity, UINT grow) {
    if (!buffer && capacity) AfxThrowInvalidArgException();
    m_nGrowBytes = grow;
    m_nPosition = 0;
    m_nBufferSize = capacity;
    m_nFileSize = grow ? 0 : capacity;
    m_lpBuffer = buffer;
    m_bAutoDelete = FALSE;
}
BYTE* CMemFile::Detach() {
    BYTE* result = m_lpBuffer;
    m_lpBuffer = nullptr;
    m_nPosition = m_nBufferSize = m_nFileSize = 0;
    return result;
}
BYTE* CMemFile::Alloc(SIZE_T bytes) { return static_cast<BYTE*>(malloc(bytes)); }
BYTE* CMemFile::Realloc(BYTE* buffer, SIZE_T bytes) { return static_cast<BYTE*>(realloc(buffer, bytes)); }
void CMemFile::Free(BYTE* buffer) { free(buffer); }
BYTE* CMemFile::Memcpy(BYTE* dest, const BYTE* src, SIZE_T bytes) {
    if (bytes) {
        if (!dest || !src) {
            if (dest) memset(dest, 0, bytes);
            errno = EINVAL;
            _invalid_parameter_noinfo();
            AfxThrowInvalidArgException();
        }
        memcpy(dest, src, bytes);
    }
    return dest;
}
void CMemFile::GrowFile(SIZE_T requested) {
    SIZE_T capacity = m_nBufferSize;
    if (requested <= capacity) return;
    if (!m_nGrowBytes) AfxThrowMemoryException();
    do { capacity += m_nGrowBytes; } while (capacity < requested);
    BYTE* buffer = m_lpBuffer ? Realloc(m_lpBuffer, capacity) : Alloc(capacity);
    if (!buffer) AfxThrowMemoryException();
    m_lpBuffer = buffer;
    m_nBufferSize = capacity;
}
ULONGLONG CMemFile::GetPosition() const { return m_nPosition; }
ULONGLONG CMemFile::GetLength() const { return m_nFileSize; }
void CMemFile::SetLength(ULONGLONG length) {
    if (length > UINT_MAX) AfxThrowMemoryException();
    if (length > m_nBufferSize) GrowFile(static_cast<SIZE_T>(length));
    if (length < m_nPosition) m_nPosition = length;
    m_nFileSize = length;
}
UINT CMemFile::Read(void* output, UINT count) {
    if (!count) return 0;
    if (!output) AfxThrowInvalidArgException();
    if (m_nPosition > m_nFileSize) return 0;
    ULONGLONG end = m_nPosition + count;
    if (end < m_nPosition || end > m_nFileSize)
        count = static_cast<UINT>(m_nFileSize) - static_cast<UINT>(m_nPosition);
    Memcpy(static_cast<BYTE*>(output), m_lpBuffer + m_nPosition, count);
    m_nPosition += count;
    return count;
}
void CMemFile::Write(const void* input, UINT count) {
    if (!count) return;
    if (!input) AfxThrowInvalidArgException();
    ULONGLONG end = m_nPosition + count;
    if (end < m_nPosition) AfxThrowInvalidArgException();
    if (end > m_nBufferSize) GrowFile(static_cast<SIZE_T>(end));
    if (m_nPosition + count > m_nBufferSize) AfxThrowInvalidArgException();
    Memcpy(m_lpBuffer + m_nPosition, static_cast<const BYTE*>(input), count);
    m_nPosition += count;
    if (m_nPosition > m_nFileSize) m_nFileSize = m_nPosition;
}
ULONGLONG CMemFile::Seek(LONGLONG offset, UINT origin) {
    ULONGLONG position;
    switch (origin) {
    case begin: position = static_cast<ULONGLONG>(offset); break;
    case current: position = m_nPosition + offset; break;
    case end:
        if (offset > 0) impl__AfxThrowFileException__YAXHJPEB_W_Z(CFileException::badSeek, -1, nullptr);
        position = m_nFileSize + offset;
        break;
    default: return m_nPosition;
    }
    if (static_cast<LONGLONG>(position) < 0)
        impl__AfxThrowFileException__YAXHJPEB_W_Z(CFileException::badSeek, -1, nullptr);
    // This DWORD comparison is observable even though the stored position is 64 bit.
    if (static_cast<UINT>(position) > m_nFileSize) GrowFile(static_cast<SIZE_T>(position));
    m_nPosition = position;
    return position;
}
void CMemFile::Close() {
    BYTE* buffer = m_lpBuffer;
    m_nGrowBytes = m_nPosition = m_nBufferSize = m_nFileSize = 0;
    if (buffer && m_bAutoDelete) Free(buffer);
    m_lpBuffer = nullptr;
}
void CMemFile::Abort() { Close(); }
void CMemFile::Flush() {}
CFile* CMemFile::Duplicate() const { impl__AfxThrowNotSupportedException__YAXXZ(); return nullptr; }
void CMemFile::LockRange(ULONGLONG, ULONGLONG) { impl__AfxThrowNotSupportedException__YAXXZ(); }
void CMemFile::UnlockRange(ULONGLONG, ULONGLONG) { impl__AfxThrowNotSupportedException__YAXXZ(); }
UINT CMemFile::GetBufferPtr(UINT command, UINT count, void** start, void** end) {
    if (command == bufferCheck) return m_nGrowBytes ? bufferDirect : 0;
    if (command == bufferCommit) {
        m_nPosition += count;
        if (m_nPosition > m_nFileSize) m_nFileSize = m_nPosition;
        return 0;
    }
    if (!start || !end) return 0;
    if (command == bufferWrite) {
        ULONGLONG requested = m_nPosition + count;
        if (requested < m_nPosition || requested < count) AfxThrowInvalidArgException();
        if (requested > m_nBufferSize) GrowFile(static_cast<SIZE_T>(requested));
    }
    *start = m_lpBuffer + m_nPosition;
    if (command == bufferWrite) {
        *end = m_lpBuffer + (std::min)(static_cast<ULONGLONG>(m_nBufferSize), m_nPosition + count);
    } else {
        if (count == UINT_MAX) count = static_cast<UINT>(m_nBufferSize) - static_cast<UINT>(m_nPosition);
        *end = m_lpBuffer + (std::min)(m_nFileSize, m_nPosition + count);
        m_nPosition += reinterpret_cast<UINT_PTR>(*end) - reinterpret_cast<UINT_PTR>(*start);
    }
    return static_cast<UINT>(reinterpret_cast<UINT_PTR>(*end) - reinterpret_cast<UINT_PTR>(*start));
}
int CMemFile::GetStatus(CFileStatus& status) const {
    status.m_ctime = status.m_mtime = status.m_atime = 0;
    status.m_size = m_nFileSize;
    status.m_attribute = 0;
    status.m_szFullName[0] = 0;
    return TRUE;
}
static CRuntimeClass* AFXAPI MemFileBase() { return CFile::GetThisClass(); }
CRuntimeClass CMemFile::classCMemFile = {"CMemFile", sizeof(CMemFile), 0xffff, nullptr, MemFileBase, nullptr, nullptr};

// Exported implementations are qualified calls; client overrides are used only
// by the virtual callback sites inside the methods above.
struct CMemFileExports {
    static BYTE* Alloc(CMemFile* p, SIZE_T n) { return p->CMemFile::Alloc(n); }
    static BYTE* Realloc(CMemFile* p, BYTE* b, SIZE_T n) { return p->CMemFile::Realloc(b,n); }
    static BYTE* Memcpy(CMemFile* p, BYTE* d, const BYTE* s, SIZE_T n) { return p->CMemFile::Memcpy(d,s,n); }
    static void Free(CMemFile* p, BYTE* b) { p->CMemFile::Free(b); }
    static void GrowFile(CMemFile* p, SIZE_T n) { p->CMemFile::GrowFile(n); }
};

// Symbol: ?Write@CMemFile@@UEAAXPEBXI@Z
extern "C" void impl__Write_CMemFile__UEAAXPEBXI_Z(CMemFile* p, const void* b, UINT n) { p->CMemFile::Write(b,n); }

// Symbol: ?UnlockRange@CMemFile@@UEAAX_K0@Z
extern "C" void impl__UnlockRange_CMemFile__UEAAX_K0_Z(CMemFile* p, ULONGLONG o, ULONGLONG n) { p->CMemFile::UnlockRange(o,n); }

// Symbol: ?SetLength@CMemFile@@UEAAX_K@Z
extern "C" void impl__SetLength_CMemFile__UEAAX_K_Z(CMemFile* p, ULONGLONG n) { p->CMemFile::SetLength(n); }

// Symbol: ?Seek@CMemFile@@UEAA_K_JI@Z
extern "C" ULONGLONG impl__Seek_CMemFile__UEAA_K_JI_Z(CMemFile* p, LONGLONG o, UINT f) { return p->CMemFile::Seek(o,f); }

// Symbol: ?Realloc@CMemFile@@MEAAPEAEPEAE_K@Z
extern "C" BYTE* impl__Realloc_CMemFile__MEAAPEAEPEAE_K_Z(CMemFile* p, BYTE* b, SIZE_T n) { return CMemFileExports::Realloc(p,b,n); }

// Symbol: ?Read@CMemFile@@UEAAIPEAXI@Z
extern "C" UINT impl__Read_CMemFile__UEAAIPEAXI_Z(CMemFile* p, void* b, UINT n) { return p->CMemFile::Read(b,n); }

// Symbol: ?Memcpy@CMemFile@@MEAAPEAEPEAEPEBE_K@Z
extern "C" BYTE* impl__Memcpy_CMemFile__MEAAPEAEPEAEPEBE_K_Z(CMemFile* p, BYTE* d, const BYTE* b, SIZE_T n) { return CMemFileExports::Memcpy(p,d,b,n); }

// Symbol: ?LockRange@CMemFile@@UEAAX_K0@Z
extern "C" void impl__LockRange_CMemFile__UEAAX_K0_Z(CMemFile* p, ULONGLONG o, ULONGLONG n) { p->CMemFile::LockRange(o,n); }

// Symbol: ?GrowFile@CMemFile@@MEAAX_K@Z
extern "C" void impl__GrowFile_CMemFile__MEAAX_K_Z(CMemFile* p, SIZE_T n) { CMemFileExports::GrowFile(p,n); }

// Symbol: ?GetThisClass@CMemFile@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* impl__GetThisClass_CMemFile__SAPEAUCRuntimeClass__XZ() { return CMemFile::GetThisClass(); }

// Symbol: ?GetStatus@CMemFile@@QEBAHAEAUCFileStatus@@@Z
extern "C" int impl__GetStatus_CMemFile__QEBAHAEAUCFileStatus___Z(const CMemFile* p, CFileStatus& s) { return p->CMemFile::GetStatus(s); }

// Symbol: ?GetRuntimeClass@CMemFile@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* impl__GetRuntimeClass_CMemFile__UEBAPEAUCRuntimeClass__XZ(const CMemFile* p) { return p->CMemFile::GetRuntimeClass(); }

// Symbol: ?GetPosition@CMemFile@@UEBA_KXZ
extern "C" ULONGLONG impl__GetPosition_CMemFile__UEBA_KXZ(const CMemFile* p) { return p->CMemFile::GetPosition(); }

// Symbol: ?GetLength@CMemFile@@UEBA_KXZ
extern "C" ULONGLONG impl__GetLength_CMemFile__UEBA_KXZ(const CMemFile* p) { return p->CMemFile::GetLength(); }

// Symbol: ?GetBufferPtr@CMemFile@@UEAAIIIPEAPEAX0@Z
extern "C" UINT impl__GetBufferPtr_CMemFile__UEAAIIIPEAPEAX0_Z(CMemFile* p, UINT c, UINT n, void** b, void** e) { return p->CMemFile::GetBufferPtr(c,n,b,e); }

// Symbol: ?Free@CMemFile@@MEAAXPEAE@Z
extern "C" void impl__Free_CMemFile__MEAAXPEAE_Z(CMemFile* p, BYTE* b) { CMemFileExports::Free(p,b); }

// Symbol: ?Flush@CMemFile@@UEAAXXZ
extern "C" void impl__Flush_CMemFile__UEAAXXZ(CMemFile* p) { p->CMemFile::Flush(); }

// Symbol: ?Duplicate@CMemFile@@UEBAPEAVCFile@@XZ
extern "C" CFile* impl__Duplicate_CMemFile__UEBAPEAVCFile__XZ(const CMemFile* p) { return p->CMemFile::Duplicate(); }

// Symbol: ?Detach@CMemFile@@QEAAPEAEXZ
extern "C" BYTE* impl__Detach_CMemFile__QEAAPEAEXZ(CMemFile* p) { return p->Detach(); }

// Symbol: ?Close@CMemFile@@UEAAXXZ
extern "C" void impl__Close_CMemFile__UEAAXXZ(CMemFile* p) { p->CMemFile::Close(); }

// Symbol: ?Attach@CMemFile@@QEAAXPEAEII@Z
extern "C" void impl__Attach_CMemFile__QEAAXPEAEII_Z(CMemFile* p, BYTE* b, UINT n, UINT g) { p->Attach(b,n,g); }

// Symbol: ?Alloc@CMemFile@@MEAAPEAE_K@Z
extern "C" BYTE* impl__Alloc_CMemFile__MEAAPEAE_K_Z(CMemFile* p, SIZE_T n) { return CMemFileExports::Alloc(p,n); }

// Symbol: ?Abort@CMemFile@@UEAAXXZ
extern "C" void impl__Abort_CMemFile__UEAAXXZ(CMemFile* p) { p->CMemFile::Abort(); }

// Symbol: ??1CMemFile@@UEAA@XZ
extern "C" void impl___1CMemFile__UEAA_XZ(CMemFile* p) { p->CMemFile::~CMemFile(); }

// Symbol: ??0CMemFile@@QEAA@PEAEII@Z
extern "C" CMemFile* impl___0CMemFile__QEAA_PEAEII_Z(void* p, BYTE* b, UINT n, UINT g) { return new(p) CMemFile(b,n,g); }

// Symbol: ??0CMemFile@@QEAA@I@Z
extern "C" CMemFile* impl___0CMemFile__QEAA_I_Z(void* p, UINT n) { return new(p) CMemFile(n); }
