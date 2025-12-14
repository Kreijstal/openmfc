#pragma once
#include <cwchar>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstdarg>

// Windows type definitions
#ifndef UINT
typedef unsigned int UINT;
#endif

#ifndef BOOL
typedef int BOOL;
#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

// Calling convention for MFC API functions
#ifndef AFXAPI
    #if defined(_MSC_VER) || defined(__MINGW32__)
        #define AFXAPI __cdecl
    #else
        #define AFXAPI
    #endif
#endif

// MFC CStringT ABI-compatible implementation
// 
// Layout: CStringT contains a single pointer m_pszData pointing to character data.
// A CStringData header is stored BEFORE the character buffer in memory:
//   [CStringData (16 bytes)] [character data ...]
//                            ^ m_pszData points here

// CStringData structure (matches MSVC MFC layout)
// On Windows: long is 4 bytes, so use int32_t for portability
struct CStringData {
    int32_t nRefs;         // Reference count (-1 = locked, meaning buffer is not shared)
    int32_t nDataLength;   // Length of string in characters (not including null terminator)
    int32_t nAllocLength;  // Allocated buffer length in characters (not including null terminator)
    int32_t padding;       // Padding to 16 bytes

    wchar_t* data() { return reinterpret_cast<wchar_t*>(this + 1); }
    const wchar_t* data() const { return reinterpret_cast<const wchar_t*>(this + 1); }

    void AddRef() { if (nRefs >= 0) ++nRefs; }
    bool Release() { return (nRefs >= 0 && --nRefs <= 0); }
    bool IsLocked() const { return nRefs < 0; }
    bool IsShared() const { return nRefs > 1; }
};

// Static nil string data for empty strings
inline CStringData* GetNilStringData() {
    // Static nil buffer: CStringData + null terminator
    static struct {
        CStringData header;
        wchar_t nul;
    } nilData = { { -1, 0, 0, 0 }, L'\0' };
    return &nilData.header;
}

// CString (Unicode) - ABI compatible with MFC CStringW
class CString {
public:
    // Default constructor - empty string
    CString() noexcept {
        m_pszData = GetNilStringData()->data();
    }

    // Constructor from wide string
    CString(const wchar_t* psz) {
        if (psz == nullptr || *psz == L'\0') {
            m_pszData = GetNilStringData()->data();
        } else {
            int nLen = static_cast<int>(wcslen(psz));
            AllocBuffer(nLen);
            wmemcpy(m_pszData, psz, nLen + 1);
            GetData()->nDataLength = nLen;
        }
    }

    // Copy constructor
    CString(const CString& strSrc) {
        CStringData* pSrcData = strSrc.GetData();
        if (pSrcData->IsLocked()) {
            // Locked (nil) data - share it
            m_pszData = strSrc.m_pszData;
        } else {
            // Reference counted - add ref
            pSrcData->AddRef();
            m_pszData = strSrc.m_pszData;
        }
    }

    // Destructor
    ~CString() {
        CStringData* pData = GetData();
        if (pData->Release()) {
            FreeBuffer(pData);
        }
    }

    // Assignment operator
    CString& operator=(const CString& strSrc) {
        if (m_pszData != strSrc.m_pszData) {
            // Release old
            CStringData* pOldData = GetData();
            if (pOldData->Release()) {
                FreeBuffer(pOldData);
            }
            // Copy new
            CStringData* pSrcData = strSrc.GetData();
            if (pSrcData->IsLocked()) {
                m_pszData = strSrc.m_pszData;
            } else {
                pSrcData->AddRef();
                m_pszData = strSrc.m_pszData;
            }
        }
        return *this;
    }

    CString& operator=(const wchar_t* psz) {
        if (psz == nullptr || *psz == L'\0') {
            Empty();
        } else {
            int nLen = static_cast<int>(wcslen(psz));
            CStringData* pData = GetData();
            if (pData->IsLocked() || pData->IsShared() || nLen > pData->nAllocLength) {
                // Need new buffer
                if (pData->Release()) {
                    FreeBuffer(pData);
                }
                AllocBuffer(nLen);
            }
            wmemcpy(m_pszData, psz, nLen + 1);
            GetData()->nDataLength = nLen;
        }
        return *this;
    }

    // Accessors
    int GetLength() const { return GetData()->nDataLength; }
    bool IsEmpty() const { return GetData()->nDataLength == 0; }
    void Empty() {
        CStringData* pData = GetData();
        if (pData->Release()) {
            FreeBuffer(pData);
        }
        m_pszData = GetNilStringData()->data();
    }

    // Get the character buffer (read-only)
    const wchar_t* GetString() const { return m_pszData; }
    operator const wchar_t*() const { return m_pszData; }

    // Character access
    wchar_t GetAt(int nIndex) const { return m_pszData[nIndex]; }
    wchar_t operator[](int nIndex) const { return m_pszData[nIndex]; }

    // Get writable buffer (releases sharing)
    wchar_t* GetBuffer(int nMinBufLength = -1) {
        if (nMinBufLength < 0) {
            nMinBufLength = GetData()->nDataLength;
        }
        CStringData* pData = GetData();
        if (pData->IsShared() || nMinBufLength > pData->nAllocLength) {
            // Need to copy
            int nOldLen = pData->nDataLength;
            int nNewLen = (nMinBufLength > nOldLen) ? nMinBufLength : nOldLen;
            CStringData* pNewData = AllocData(nNewLen);
            wmemcpy(pNewData->data(), m_pszData, nOldLen + 1);
            pNewData->nDataLength = nOldLen;
            if (pData->Release()) {
                FreeBuffer(pData);
            }
            m_pszData = pNewData->data();
        }
        // Lock it to prevent sharing while exposed
        GetData()->nRefs = -1;
        return m_pszData;
    }

    void ReleaseBuffer(int nNewLength = -1) {
        CStringData* pData = GetData();
        if (nNewLength < 0) {
            nNewLength = static_cast<int>(wcslen(m_pszData));
        }
        pData->nDataLength = nNewLength;
        m_pszData[nNewLength] = L'\0';
        pData->nRefs = 1; // Unlock
    }

    // Concatenation
    CString& operator+=(const wchar_t* psz) {
        if (psz != nullptr && *psz != L'\0') {
            int nSrcLen = static_cast<int>(wcslen(psz));
            int nOldLen = GetData()->nDataLength;
            int nNewLen = nOldLen + nSrcLen;
            CStringData* pData = GetData();
            if (pData->IsLocked() || pData->IsShared() || nNewLen > pData->nAllocLength) {
                CStringData* pNewData = AllocData(nNewLen);
                wmemcpy(pNewData->data(), m_pszData, nOldLen);
                if (pData->Release()) {
                    FreeBuffer(pData);
                }
                m_pszData = pNewData->data();
            }
            wmemcpy(m_pszData + nOldLen, psz, nSrcLen + 1);
            GetData()->nDataLength = nNewLen;
        }
        return *this;
    }

    CString& operator+=(const CString& str) {
        return operator+=(str.GetString());
    }

    // Comparison
    int Compare(const wchar_t* psz) const {
        return wcscmp(m_pszData, psz ? psz : L"");
    }

    bool operator==(const wchar_t* psz) const { return Compare(psz) == 0; }
    bool operator!=(const wchar_t* psz) const { return Compare(psz) != 0; }
    bool operator==(const CString& str) const { return Compare(str.m_pszData) == 0; }
    bool operator!=(const CString& str) const { return Compare(str.m_pszData) != 0; }

    // Substring extraction
    CString Left(int nCount) const {
        if (nCount <= 0) return CString();
        if (nCount >= GetLength()) return *this;
        CString result;
        result.AllocBuffer(nCount);
        wmemcpy(result.m_pszData, m_pszData, nCount);
        result.m_pszData[nCount] = L'\0';
        result.GetData()->nDataLength = nCount;
        return result;
    }

    CString Right(int nCount) const {
        int nLen = GetLength();
        if (nCount <= 0) return CString();
        if (nCount >= nLen) return *this;
        return CString(m_pszData + nLen - nCount);
    }

    CString Mid(int nFirst, int nCount = -1) const {
        int nLen = GetLength();
        if (nFirst < 0) nFirst = 0;
        if (nFirst >= nLen) return CString();
        if (nCount < 0) nCount = nLen - nFirst;
        if (nFirst + nCount > nLen) nCount = nLen - nFirst;
        
        CString dest;
        dest.AllocBuffer(nCount);
        wmemcpy(dest.m_pszData, m_pszData + nFirst, nCount);
        dest.m_pszData[nCount] = L'\0';
        dest.GetData()->nDataLength = nCount;
        return dest;
    }

    // Searching
    int Find(wchar_t ch, int nStart = 0) const {
        if (nStart < 0) nStart = 0;
        int nLen = GetLength();
        if (nStart >= nLen) return -1;
        const wchar_t* p = wcschr(m_pszData + nStart, ch);
        return p ? static_cast<int>(p - m_pszData) : -1;
    }

    int Find(const wchar_t* pszSub, int nStart = 0) const {
        if (pszSub == nullptr || *pszSub == L'\0') return -1;
        if (nStart < 0) nStart = 0;
        int nLen = GetLength();
        if (nStart >= nLen) return -1;
        const wchar_t* p = wcsstr(m_pszData + nStart, pszSub);
        return p ? static_cast<int>(p - m_pszData) : -1;
    }

    int ReverseFind(wchar_t ch) const {
        const wchar_t* p = wcsrchr(m_pszData, ch);
        return p ? static_cast<int>(p - m_pszData) : -1;
    }

    int FindOneOf(const wchar_t* pszCharSet) const {
        if (pszCharSet == nullptr || *pszCharSet == L'\0') return -1;
        const wchar_t* p = wcspbrk(m_pszData, pszCharSet);
        return p ? static_cast<int>(p - m_pszData) : -1;
    }

    // Case conversion (modifies in place, returns reference)
    CString& MakeUpper() {
        int nLen = GetLength();
        if (nLen > 0) {
            wchar_t* pBuf = GetBuffer(nLen);
            for (int i = 0; i < nLen; i++) {
                if (pBuf[i] >= L'a' && pBuf[i] <= L'z')
                    pBuf[i] -= 32;
            }
            ReleaseBuffer(nLen);
        }
        return *this;
    }

    CString& MakeLower() {
        int nLen = GetLength();
        if (nLen > 0) {
            wchar_t* pBuf = GetBuffer(nLen);
            for (int i = 0; i < nLen; i++) {
                if (pBuf[i] >= L'A' && pBuf[i] <= L'Z')
                    pBuf[i] += 32;
            }
            ReleaseBuffer(nLen);
        }
        return *this;
    }

    // Trimming
    CString& TrimLeft() {
        int nLen = GetLength();
        if (nLen == 0) return *this;
        int nStart = 0;
        while (nStart < nLen && (m_pszData[nStart] == L' ' || m_pszData[nStart] == L'\t' ||
               m_pszData[nStart] == L'\r' || m_pszData[nStart] == L'\n'))
            nStart++;
        if (nStart > 0) {
            *this = Mid(nStart);
        }
        return *this;
    }

    CString& TrimRight() {
        int nLen = GetLength();
        if (nLen == 0) return *this;
        int nEnd = nLen;
        while (nEnd > 0 && (m_pszData[nEnd - 1] == L' ' || m_pszData[nEnd - 1] == L'\t' ||
               m_pszData[nEnd - 1] == L'\r' || m_pszData[nEnd - 1] == L'\n'))
            nEnd--;
        if (nEnd < nLen) {
            wchar_t* pBuf = GetBuffer(nEnd);
            pBuf[nEnd] = L'\0';
            ReleaseBuffer(nEnd);
        }
        return *this;
    }

    CString& Trim() {
        TrimRight();
        TrimLeft();
        return *this;
    }

    // Replace
    int Replace(wchar_t chOld, wchar_t chNew) {
        int nCount = 0;
        int nLen = GetLength();
        if (nLen > 0 && chOld != chNew) {
            wchar_t* pBuf = GetBuffer(nLen);
            for (int i = 0; i < nLen; i++) {
                if (pBuf[i] == chOld) {
                    pBuf[i] = chNew;
                    nCount++;
                }
            }
            ReleaseBuffer(nLen);
        }
        return nCount;
    }

    int Replace(const wchar_t* pszOld, const wchar_t* pszNew) {
        if (pszOld == nullptr || *pszOld == L'\0') return 0;
        if (pszNew == nullptr) pszNew = L"";
        int nOldLen = static_cast<int>(wcslen(pszOld));
        int nNewLen = static_cast<int>(wcslen(pszNew));
        int nCount = 0;
        // Count occurrences
        const wchar_t* pStart = m_pszData;
        while ((pStart = wcsstr(pStart, pszOld)) != nullptr) {
            nCount++;
            pStart += nOldLen;
        }
        if (nCount == 0) return 0;
        // Build new string
        int nSrcLen = GetLength();
        int nNewStrLen = nSrcLen + (nNewLen - nOldLen) * nCount;
        CString strResult;
        strResult.AllocBuffer(nNewStrLen);
        wchar_t* pDest = strResult.m_pszData;
        const wchar_t* pSrc = m_pszData;
        while (*pSrc) {
            if (wcsncmp(pSrc, pszOld, nOldLen) == 0) {
                wmemcpy(pDest, pszNew, nNewLen);
                pDest += nNewLen;
                pSrc += nOldLen;
            } else {
                *pDest++ = *pSrc++;
            }
        }
        *pDest = L'\0';
        strResult.GetData()->nDataLength = nNewStrLen;
        *this = strResult;
        return nCount;
    }

    // Character modification
    void SetAt(int nIndex, wchar_t ch) {
        if (nIndex >= 0 && nIndex < GetLength()) {
            wchar_t* pBuf = GetBuffer(GetLength());
            pBuf[nIndex] = ch;
            ReleaseBuffer(GetLength());
        }
    }

    // Delete characters from string
    int Delete(int nIndex, int nCount = 1) {
        if (nIndex < 0) nIndex = 0;
        int nLen = GetLength();
        if (nIndex >= nLen || nCount <= 0) return nLen;
        if (nIndex + nCount > nLen) nCount = nLen - nIndex;
        int nNewLen = nLen - nCount;
        wchar_t* pBuf = GetBuffer(nLen);
        wmemmove(pBuf + nIndex, pBuf + nIndex + nCount, nLen - nIndex - nCount + 1);
        ReleaseBuffer(nNewLen);
        return nNewLen;
    }

    // Insert character at position
    int Insert(int nIndex, wchar_t ch) {
        if (nIndex < 0) nIndex = 0;
        int nLen = GetLength();
        if (nIndex > nLen) nIndex = nLen;
        wchar_t* pBuf = GetBuffer(nLen + 1);
        wmemmove(pBuf + nIndex + 1, pBuf + nIndex, nLen - nIndex + 1);
        pBuf[nIndex] = ch;
        ReleaseBuffer(nLen + 1);
        return nLen + 1;
    }

    // Insert string at position
    int Insert(int nIndex, const wchar_t* pstr) {
        if (pstr == nullptr || *pstr == L'\0') return GetLength();
        if (nIndex < 0) nIndex = 0;
        int nLen = GetLength();
        if (nIndex > nLen) nIndex = nLen;
        int nInsertLen = static_cast<int>(wcslen(pstr));
        int nNewLen = nLen + nInsertLen;
        wchar_t* pBuf = GetBuffer(nNewLen);
        wmemmove(pBuf + nIndex + nInsertLen, pBuf + nIndex, nLen - nIndex + 1);
        wmemcpy(pBuf + nIndex, pstr, nInsertLen);
        ReleaseBuffer(nNewLen);
        return nNewLen;
    }

    // Reverse string in place
    CString& MakeReverse() {
        int nLen = GetLength();
        if (nLen > 1) {
            wchar_t* pBuf = GetBuffer(nLen);
            for (int i = 0; i < nLen / 2; i++) {
                wchar_t tmp = pBuf[i];
                pBuf[i] = pBuf[nLen - 1 - i];
                pBuf[nLen - 1 - i] = tmp;
            }
            ReleaseBuffer(nLen);
        }
        return *this;
    }

    // Get span of characters matching set
    CString SpanIncluding(const wchar_t* pszCharSet) const {
        if (pszCharSet == nullptr || *pszCharSet == L'\0') return CString();
        int nLen = static_cast<int>(wcsspn(m_pszData, pszCharSet));
        return Left(nLen);
    }

    // Get span of characters NOT in set
    CString SpanExcluding(const wchar_t* pszCharSet) const {
        if (pszCharSet == nullptr || *pszCharSet == L'\0') return *this;
        int nLen = static_cast<int>(wcscspn(m_pszData, pszCharSet));
        return Left(nLen);
    }

    // Case-insensitive comparison
    int CompareNoCase(const wchar_t* psz) const {
        if (psz == nullptr) psz = L"";
        const wchar_t* p1 = m_pszData;
        const wchar_t* p2 = psz;
        while (*p1 && *p2) {
            wchar_t c1 = (*p1 >= L'A' && *p1 <= L'Z') ? *p1 + 32 : *p1;
            wchar_t c2 = (*p2 >= L'A' && *p2 <= L'Z') ? *p2 + 32 : *p2;
            if (c1 != c2) return c1 - c2;
            p1++; p2++;
        }
        wchar_t c1 = (*p1 >= L'A' && *p1 <= L'Z') ? *p1 + 32 : *p1;
        wchar_t c2 = (*p2 >= L'A' && *p2 <= L'Z') ? *p2 + 32 : *p2;
        return c1 - c2;
    }

    // Printf-style formatting
    void FormatV(const wchar_t* pszFormat, va_list args) {
        if (pszFormat == nullptr) {
            Empty();
            return;
        }
        // Get required buffer size
        va_list argsCopy;
        va_copy(argsCopy, args);
        int nLen = vswprintf(nullptr, 0, pszFormat, argsCopy);
        va_end(argsCopy);
        if (nLen <= 0) {
            Empty();
            return;
        }
        // Allocate and format
        wchar_t* pBuf = GetBuffer(nLen);
        vswprintf(pBuf, nLen + 1, pszFormat, args);
        ReleaseBuffer(nLen);
    }

    void Format(const wchar_t* pszFormat, ...) {
        va_list args;
        va_start(args, pszFormat);
        FormatV(pszFormat, args);
        va_end(args);
    }

    void AppendFormat(const wchar_t* pszFormat, ...) {
        CString strAppend;
        va_list args;
        va_start(args, pszFormat);
        strAppend.FormatV(pszFormat, args);
        va_end(args);
        *this += strAppend;
    }

    // Resource loading
    int LoadString(UINT nID); // Returns length, 0 on failure
    // Alias for Windows API macro compatibility (LoadString -> LoadStringW)
    int LoadStringW(UINT nID) { return LoadString(nID); }

private:
    wchar_t* m_pszData;

    CStringData* GetData() const {
        return reinterpret_cast<CStringData*>(m_pszData) - 1;
    }
    
    // ... (rest of private members)
    static CStringData* AllocData(int nLength) {
        // Allocate CStringData + (nLength + 1) wchar_t characters
        size_t nSize = sizeof(CStringData) + (nLength + 1) * sizeof(wchar_t);
        CStringData* pData = static_cast<CStringData*>(malloc(nSize));
        if (pData == nullptr) {
            // Out of memory - in real MFC this throws
            std::fprintf(stderr, "CString: out of memory\n");
            std::abort();
        }
        pData->nRefs = 1;
        pData->nDataLength = 0;
        pData->nAllocLength = nLength;
        pData->padding = 0;
        pData->data()[0] = L'\0';
        return pData;
    }

    void AllocBuffer(int nLength) {
        CStringData* pData = AllocData(nLength);
        m_pszData = pData->data();
    }

    static void FreeBuffer(CStringData* pData) {
        if (pData != GetNilStringData()) {
            free(pData);
        }
    }
};

// Concatenation operators
inline CString operator+(const CString& str1, const CString& str2) {
    CString result(str1);
    result += str2;
    return result;
}

inline CString operator+(const CString& str, const wchar_t* psz) {
    CString result(str);
    result += psz;
    return result;
}

inline CString operator+(const wchar_t* psz, const CString& str) {
    CString result(psz);
    result += str;
    return result;
}

// Global String Helpers
// Note: These require linking with strcore.cpp

// Extract substring from source string using separator
// Returns TRUE if substring found, FALSE otherwise
BOOL AfxExtractSubString(CString& rString, const wchar_t* lpszFullString, int iSubString, wchar_t chSep = L'\n');

// Format string using resource template and one argument (%1)
void AfxFormatString1(CString& rString, UINT nIDS, const wchar_t* lpsz1);

// Format string using resource template and two arguments (%1, %2)
void AfxFormatString2(CString& rString, UINT nIDS, const wchar_t* lpsz1, const wchar_t* lpsz2);

// Display message box
int AfxMessageBox(const wchar_t* lpszText, UINT nType = 0, UINT nIDHelp = 0);
int AfxMessageBox(UINT nIDPrompt, UINT nType = 0, UINT nIDHelp = 0);
