// File and Archive Core Implementation
//
// Implements CFile, CMemFile, CStdioFile, and CArchive for serialization.

#define OPENMFC_APPCORE_IMPL
#include "openmfc/afx.h"
#include <windows.h>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cerrno>
#include <io.h>

// MS ABI calling convention
#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

// =============================================================================
// CFile Implementation
// =============================================================================

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
    (void)rStatus;
    return 0;
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

// =============================================================================
// CMemFile Implementation
// =============================================================================

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

// =============================================================================
// CStdioFile Implementation
// =============================================================================

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

// =============================================================================
// CArchive Implementation
// =============================================================================

CArchive::CArchive(CFile* pFile, UINT nMode, int nBufSize, void* lpBuf)
    : m_pFile(pFile), m_nMode(nMode), m_nBufSize(nBufSize),
      m_nObjectSchema(0), m_bForceFlat(false)
{
    if (lpBuf) {
        m_lpBufStart = static_cast<unsigned char*>(lpBuf);
        m_bUserBuf = true;
    } else {
        m_lpBufStart = new unsigned char[nBufSize];
        m_bUserBuf = false;
    }

    if (IsLoading()) {
        m_lpBufCur = m_lpBufStart;
        m_lpBufMax = m_lpBufStart;  // Empty buffer, will be filled on first read
    } else {
        m_lpBufCur = m_lpBufStart;
        m_lpBufMax = m_lpBufStart + nBufSize;
    }
}

CArchive::~CArchive() {
    if (!(m_nMode & bNoFlushOnDelete)) {
        Flush();
    }
    if (!m_bUserBuf) {
        delete[] m_lpBufStart;
    }
}

void CArchive::FillBuffer(UINT nBytesNeeded) {
    if (!m_pFile) return;

    // Move remaining data to start of buffer
    UINT nRemaining = (UINT)(m_lpBufMax - m_lpBufCur);
    if (nRemaining > 0 && m_lpBufCur != m_lpBufStart) {
        memmove(m_lpBufStart, m_lpBufCur, nRemaining);
    }
    m_lpBufCur = m_lpBufStart;

    // Read more data
    UINT nRead = m_pFile->Read(m_lpBufStart + nRemaining,
                               m_nBufSize - nRemaining);
    m_lpBufMax = m_lpBufStart + nRemaining + nRead;
}

void CArchive::WriteBuffer() {
    if (!m_pFile) return;

    UINT nBytes = (UINT)(m_lpBufCur - m_lpBufStart);
    if (nBytes > 0) {
        m_pFile->Write(m_lpBufStart, nBytes);
        m_lpBufCur = m_lpBufStart;
    }
}

UINT CArchive::Read(void* lpBuf, UINT nMax) {
    if (!IsLoading() || !lpBuf || nMax == 0) return 0;

    UINT nRead = 0;
    unsigned char* pDest = static_cast<unsigned char*>(lpBuf);

    while (nRead < nMax) {
        UINT nAvail = (UINT)(m_lpBufMax - m_lpBufCur);
        if (nAvail == 0) {
            FillBuffer(nMax - nRead);
            nAvail = (UINT)(m_lpBufMax - m_lpBufCur);
            if (nAvail == 0) break;  // EOF
        }

        UINT nCopy = (nMax - nRead < nAvail) ? (nMax - nRead) : nAvail;
        memcpy(pDest + nRead, m_lpBufCur, nCopy);
        m_lpBufCur += nCopy;
        nRead += nCopy;
    }

    return nRead;
}

void CArchive::Write(const void* lpBuf, UINT nMax) {
    if (!IsStoring() || !lpBuf || nMax == 0) return;

    const unsigned char* pSrc = static_cast<const unsigned char*>(lpBuf);
    UINT nWritten = 0;

    while (nWritten < nMax) {
        UINT nAvail = (UINT)(m_lpBufMax - m_lpBufCur);
        if (nAvail == 0) {
            WriteBuffer();
            nAvail = (UINT)(m_lpBufMax - m_lpBufCur);
        }

        UINT nCopy = (nMax - nWritten < nAvail) ? (nMax - nWritten) : nAvail;
        memcpy(m_lpBufCur, pSrc + nWritten, nCopy);
        m_lpBufCur += nCopy;
        nWritten += nCopy;
    }
}

void CArchive::Flush() {
    if (IsStoring()) {
        WriteBuffer();
    }
    if (m_pFile) {
        m_pFile->Flush();
    }
}

void CArchive::Close() {
    Flush();
}

void CArchive::Abort() {
    // Reset without flushing
    m_lpBufCur = m_lpBufStart;
    if (IsLoading()) {
        m_lpBufMax = m_lpBufStart;
    }
}

// Reading operators
CArchive& CArchive::operator>>(bool& b) {
    char ch;
    Read(&ch, sizeof(ch));
    b = (ch != 0);
    return *this;
}

CArchive& CArchive::operator>>(char& ch) {
    Read(&ch, sizeof(ch));
    return *this;
}

CArchive& CArchive::operator>>(unsigned char& ch) {
    Read(&ch, sizeof(ch));
    return *this;
}

CArchive& CArchive::operator>>(short& w) {
    Read(&w, sizeof(w));
    return *this;
}

CArchive& CArchive::operator>>(unsigned short& w) {
    Read(&w, sizeof(w));
    return *this;
}

CArchive& CArchive::operator>>(int& i) {
    Read(&i, sizeof(i));
    return *this;
}

CArchive& CArchive::operator>>(unsigned int& i) {
    Read(&i, sizeof(i));
    return *this;
}

CArchive& CArchive::operator>>(long& l) {
    Read(&l, sizeof(l));
    return *this;
}

CArchive& CArchive::operator>>(unsigned long& l) {
    Read(&l, sizeof(l));
    return *this;
}

CArchive& CArchive::operator>>(float& f) {
    Read(&f, sizeof(f));
    return *this;
}

CArchive& CArchive::operator>>(double& d) {
    Read(&d, sizeof(d));
    return *this;
}

CArchive& CArchive::operator>>(CObject*& pOb) {
    // Simplified: just read null pointer indicator
    (void)pOb;
    pOb = nullptr;
    return *this;
}

CArchive& CArchive::operator>>(CString& str) {
    // Read string length
    UINT nLen = 0;
    Read(&nLen, sizeof(nLen));

    if (nLen > 0 && nLen < 0x10000000) {
        wchar_t* pBuf = str.GetBuffer(nLen + 1);
        Read(pBuf, nLen * sizeof(wchar_t));
        pBuf[nLen] = L'\0';
        str.ReleaseBuffer(nLen);
    } else {
        str.Empty();
    }
    return *this;
}

// Writing operators
CArchive& CArchive::operator<<(bool b) {
    char ch = b ? 1 : 0;
    Write(&ch, sizeof(ch));
    return *this;
}

CArchive& CArchive::operator<<(char ch) {
    Write(&ch, sizeof(ch));
    return *this;
}

CArchive& CArchive::operator<<(unsigned char ch) {
    Write(&ch, sizeof(ch));
    return *this;
}

CArchive& CArchive::operator<<(short w) {
    Write(&w, sizeof(w));
    return *this;
}

CArchive& CArchive::operator<<(unsigned short w) {
    Write(&w, sizeof(w));
    return *this;
}

CArchive& CArchive::operator<<(int i) {
    Write(&i, sizeof(i));
    return *this;
}

CArchive& CArchive::operator<<(unsigned int i) {
    Write(&i, sizeof(i));
    return *this;
}

CArchive& CArchive::operator<<(long l) {
    Write(&l, sizeof(l));
    return *this;
}

CArchive& CArchive::operator<<(unsigned long l) {
    Write(&l, sizeof(l));
    return *this;
}

CArchive& CArchive::operator<<(float f) {
    Write(&f, sizeof(f));
    return *this;
}

CArchive& CArchive::operator<<(double d) {
    Write(&d, sizeof(d));
    return *this;
}

CArchive& CArchive::operator<<(const CObject* pOb) {
    // Simplified: just write null pointer indicator
    (void)pOb;
    return *this;
}

CArchive& CArchive::operator<<(const CString& str) {
    // Write string length and data
    UINT nLen = str.GetLength();
    Write(&nLen, sizeof(nLen));

    if (nLen > 0) {
        Write((const wchar_t*)str, nLen * sizeof(wchar_t));
    }
    return *this;
}

// String operations
int CArchive::ReadString(wchar_t* lpsz, UINT nMax) {
    if (!lpsz || nMax == 0) return 0;

    UINT nRead = 0;
    while (nRead < nMax - 1) {
        wchar_t ch;
        if (Read(&ch, sizeof(ch)) != sizeof(ch)) break;
        if (ch == L'\n') {
            lpsz[nRead++] = ch;
            break;
        }
        if (ch == L'\r') continue;  // Skip CR
        lpsz[nRead++] = ch;
    }
    lpsz[nRead] = L'\0';
    return nRead;
}

int CArchive::ReadString(CString& rString) {
    rString.Empty();
    wchar_t buf[256];
    int nTotal = 0;

    while (true) {
        int nRead = ReadString(buf, 256);
        if (nRead == 0) break;
        rString += buf;
        nTotal += nRead;
        if (buf[nRead - 1] == L'\n') break;
    }
    return nTotal;
}

void CArchive::WriteString(const wchar_t* lpsz) {
    if (!lpsz) return;
    UINT nLen = (UINT)wcslen(lpsz);
    Write(lpsz, nLen * sizeof(wchar_t));
}

// =============================================================================
// CFile extern "C" MS_ABI Thunks
// These are cross-ABI vtable entry points for CFile virtual methods.
// =============================================================================

// Symbol: ?GetLength@CFile@@UEBA_KXZ
extern "C" unsigned long long MS_ABI impl__GetLength_CFile__UEBA_KXZ(void* pThis) {
    CFile* self = static_cast<CFile*>(pThis);
    LARGE_INTEGER li;
    li.QuadPart = 0;
    if (GetFileSizeEx((HANDLE)self->m_hFile, &li))
        return li.QuadPart;
    return 0;
}

// Symbol: ?Seek@CFile@@UEAA_K_JI@Z
extern "C" unsigned long long MS_ABI impl__Seek_CFile__UEAA_K_JI_Z(void* pThis, long long lOff, unsigned int nFrom) {
    CFile* self = static_cast<CFile*>(pThis);
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
    DWORD dwRead = 0;
    ReadFile((HANDLE)self->m_hFile, lpBuf, nCount, &dwRead, nullptr);
    return dwRead;
}

// Symbol: ?Write@CFile@@UEAAXPEBXI@Z
extern "C" void MS_ABI impl__Write_CFile__UEAAXPEBXI_Z(void* pThis, const void* lpBuf, unsigned int nCount) {
    CFile* self = static_cast<CFile*>(pThis);
    DWORD dwWritten = 0;
    WriteFile((HANDLE)self->m_hFile, lpBuf, nCount, &dwWritten, nullptr);
}

// Symbol: ?GetPosition@CFile@@UEBA_KXZ
extern "C" unsigned long long MS_ABI impl__GetPosition_CFile__UEBA_KXZ(void* pThis) {
    CFile* self = static_cast<CFile*>(pThis);
    LARGE_INTEGER li, result;
    li.QuadPart = 0;
    if (SetFilePointerEx((HANDLE)self->m_hFile, li, &result, FILE_CURRENT))
        return result.QuadPart;
    return 0;
}

// Symbol: ?Flush@CFile@@UEAAXXZ
extern "C" void MS_ABI impl__Flush_CFile__UEAAXXZ(void* pThis) {
    CFile* self = static_cast<CFile*>(pThis);
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

struct CFileAccessor : CFile {
    static void InvokeCommonInit(CFile* pFile, const wchar_t* lpszFileName, unsigned int nOpenFlags, void* pTM) {
        static_cast<CFileAccessor*>(pFile)->CommonInit(lpszFileName, nOpenFlags, pTM);
    }
};

struct CMemFileAccessor : CMemFile {
    static UINT& Position(CMemFile* pFile) { return static_cast<CMemFileAccessor*>(pFile)->m_nPosition; }
    static UINT Position(const CMemFile* pFile) { return static_cast<const CMemFileAccessor*>(pFile)->m_nPosition; }
    static UINT GrowBytes(const CMemFile* pFile) { return static_cast<const CMemFileAccessor*>(pFile)->m_nGrowBytes; }
    static BYTE* Buffer(CMemFile* pFile) { return static_cast<CMemFileAccessor*>(pFile)->m_lpBuffer; }
    static const BYTE* Buffer(const CMemFile* pFile) { return static_cast<const CMemFileAccessor*>(pFile)->m_lpBuffer; }
    static UINT BufferSize(const CMemFile* pFile) { return static_cast<const CMemFileAccessor*>(pFile)->m_nBufferSize; }
};

struct CStdioFileAccessor : CStdioFile {
    static FILE*& Stream(CStdioFile* pFile) { return static_cast<CStdioFileAccessor*>(pFile)->m_pStream; }
    static FILE* Stream(const CStdioFile* pFile) { return static_cast<const CStdioFileAccessor*>(pFile)->m_pStream; }
};

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
    return nullptr;
}

// Symbol: ?GetRuntimeClass@CFile@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CFile__UEBAPEAUCRuntimeClass__XZ(const void* pThis) {
    (void)pThis;
    return nullptr;
}

extern "C" void* openmfc_cfile_hFileNull = (void*)INVALID_HANDLE_VALUE;
// Symbol: ?hFileNull@CFile@@2QEAXEA
#ifdef __GNUC__
asm(".globl \"?hFileNull@CFile@@2QEAXEA\"\n"
    ".set \"?hFileNull@CFile@@2QEAXEA\", openmfc_cfile_hFileNull\n");
#endif

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
extern "C" void MS_ABI impl__LockRange_CMemFile__UEAAX_K0_Z(CMemFile* /*pThis*/, unsigned long long /*dwPos*/, unsigned long long /*dwCount*/) {}

// Symbol: ?UnlockRange@CMemFile@@UEAAX_K0@Z
extern "C" void MS_ABI impl__UnlockRange_CMemFile__UEAAX_K0_Z(CMemFile* /*pThis*/, unsigned long long /*dwPos*/, unsigned long long /*dwCount*/) {}

// Symbol: ?GetThisClass@CMemFile@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMemFile__SAPEAUCRuntimeClass__XZ() {
    return nullptr;
}

// Symbol: ?GetRuntimeClass@CMemFile@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CMemFile__UEBAPEAUCRuntimeClass__XZ(const CMemFile* pThis) {
    (void)pThis;
    return nullptr;
}

static void OpenStdioFile(CStdioFile* pThis, const wchar_t* lpszFileName, unsigned int nOpenFlags) {
    if (!pThis) return;
    FILE*& stream = CStdioFileAccessor::Stream(pThis);
    if (stream) {
        fclose(stream);
        stream = nullptr;
    }
    if (!lpszFileName) return;
    wchar_t szMode[4] = {0};
    int idx = 0;
    bool bText = (nOpenFlags & CFile::typeText) != 0;
    unsigned int accessMode = nOpenFlags & 0x0003;
    if (accessMode == CFile::modeRead) szMode[idx++] = L'r';
    else if (accessMode == CFile::modeWrite) szMode[idx++] = L'w';
    else { szMode[idx++] = (nOpenFlags & CFile::modeCreate) ? L'w' : L'r'; szMode[idx++] = L'+'; }
    if (!bText) szMode[idx++] = L'b';
    stream = _wfopen(lpszFileName, szMode);
    pThis->m_hFile = (void*)INVALID_HANDLE_VALUE;
    if (stream) {
        int fd = _fileno(stream);
        if (fd >= 0) {
            pThis->m_hFile = (void*)_get_osfhandle(fd);
        }
    }
    pThis->m_strFileName = lpszFileName;
}

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
    return nullptr;
}

// Symbol: ?GetRuntimeClass@CStdioFile@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CStdioFile__UEBAPEAUCRuntimeClass__XZ(const CStdioFile* pThis) {
    (void)pThis;
    return nullptr;
}

extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CFileException__SAPEAUCRuntimeClass__XZ();
extern "C" void MS_ABI impl__AfxThrowFileException__YAXHJPEB_W_Z(int cause, long lOsError, const wchar_t* lpszFileName);

static int FileExceptionFromErrno(int nErrno) {
    enum FileCause {
        kGenericException = 1,
        kFileNotFound = 2,
        kTooManyOpenFiles = 4,
        kAccessDenied = 5,
        kSharingViolation = 10,
        kLockViolation = 11,
        kDiskFull = 13
    };
    switch (nErrno) {
        case ENOENT: return kFileNotFound;
        case EACCES: return kAccessDenied;
        case EMFILE: return kTooManyOpenFiles;
        case ENOSPC: return kDiskFull;
        default: return kGenericException;
    }
}

static int FileExceptionFromOsError(long lOsError) {
    enum FileCause {
        kGenericException = 1,
        kFileNotFound = 2,
        kAccessDenied = 5,
        kSharingViolation = 10,
        kLockViolation = 11,
        kDiskFull = 13
    };
    switch (lOsError) {
        case ERROR_FILE_NOT_FOUND:
        case ERROR_PATH_NOT_FOUND: return kFileNotFound;
        case ERROR_ACCESS_DENIED: return kAccessDenied;
        case ERROR_SHARING_VIOLATION: return kSharingViolation;
        case ERROR_LOCK_VIOLATION: return kLockViolation;
        case ERROR_DISK_FULL: return kDiskFull;
        default: return kGenericException;
    }
}

// Symbol: ?ErrnoToException@CFileException@@SAHH@Z
extern "C" int MS_ABI impl__ErrnoToException_CFileException__SAHH_Z(int nErrno) {
    return FileExceptionFromErrno(nErrno);
}

// Symbol: ?OsErrorToException@CFileException@@SAHJ@Z
extern "C" int MS_ABI impl__OsErrorToException_CFileException__SAHJ_Z(long lOsError) {
    return FileExceptionFromOsError(lOsError);
}

// Symbol: ?ThrowErrno@CFileException@@SAXHPEB_W@Z
extern "C" void MS_ABI impl__ThrowErrno_CFileException__SAXHPEB_W_Z(int nErrno, const wchar_t* lpszFileName) {
    impl__AfxThrowFileException__YAXHJPEB_W_Z(FileExceptionFromErrno(nErrno), static_cast<long>(nErrno), lpszFileName);
}

// Symbol: ?ThrowOsError@CFileException@@SAXJPEB_W@Z
extern "C" void MS_ABI impl__ThrowOsError_CFileException__SAXJPEB_W_Z(long lOsError, const wchar_t* lpszFileName) {
    impl__AfxThrowFileException__YAXHJPEB_W_Z(FileExceptionFromOsError(lOsError), lOsError, lpszFileName);
}

// Symbol: ?GetRuntimeClass@CFileException@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CFileException__UEBAPEAUCRuntimeClass__XZ(const void* pThis) {
    (void)pThis;
    return impl__GetThisClass_CFileException__SAPEAUCRuntimeClass__XZ();
}
