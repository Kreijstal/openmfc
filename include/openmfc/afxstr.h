#pragma once
#include <cwchar>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>

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
        return CString(m_pszData + nFirst);
    }

private:
    wchar_t* m_pszData;

    CStringData* GetData() const {
        return reinterpret_cast<CStringData*>(m_pszData) - 1;
    }

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
