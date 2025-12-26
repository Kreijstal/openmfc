// File and Archive Core Implementation
//
// Implements CFile, CMemFile, CStdioFile, and CArchive for serialization.

#define OPENMFC_APPCORE_IMPL
#include "openmfc/afx.h"
#include <windows.h>
#include <cstring>
#include <cstdlib>
#include <cstdio>

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

bool CFile::GetStatus(const wchar_t* lpszFileName, CFileStatus& rStatus) {
    if (!lpszFileName) return false;

    WIN32_FILE_ATTRIBUTE_DATA data;
    if (!GetFileAttributesExW(lpszFileName, GetFileExInfoStandard, &data)) {
        return false;
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

    return true;
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

bool CStdioFile::ReadString(wchar_t* lpsz, UINT nMax) {
    if (!m_pStream || !lpsz || nMax == 0) return false;
    return fgetws(lpsz, nMax, m_pStream) != nullptr;
}

bool CStdioFile::ReadString(CString& rString) {
    rString.Empty();
    if (!m_pStream) return false;

    wchar_t buf[256];
    while (fgetws(buf, 256, m_pStream)) {
        rString += buf;
        // Check if we got a complete line
        int len = rString.GetLength();
        if (len > 0 && ((const wchar_t*)rString)[len - 1] == L'\n') {
            break;
        }
    }
    return rString.GetLength() > 0;
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
