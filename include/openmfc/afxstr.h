#pragma once
#include <cwchar>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstdarg>
#ifdef _WIN32
#include <windows.h>
#endif
#ifdef _MSC_VER
#include <intrin.h>
#endif

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
extern void AFXAPI AfxThrowMemoryException();
extern void AFXAPI AfxThrowOleException(long);
// 
// Layout: CStringT contains a single pointer m_pszData pointing to character data.
// A CStringData header is stored BEFORE the character buffer in memory:
//   [CStringData (24 bytes)] [character data ...]
//                            ^ m_pszData points here

// CStringData / IAtlStringMgr are laid out byte-identically to ATL's
// (atlsimpstr.h) so a CString returned by value to a real MSVC client is read and
// destructed correctly. ATL CStringData: pStringMgr@0, nDataLength@8, nAllocLength@12,
// nRefs@16 (data at offset 24). When a client releases the last ref it calls
// pStringMgr->Free through this vtable, so OpenMFC strings carry a real manager.
struct CStringData;

// Matches ATL's __interface IAtlStringMgr exactly (5 pure-virtual methods, no
// virtual destructor -> 5-slot vtable identical under MSVC and mingw/Win64).
struct IAtlStringMgr {
    virtual CStringData* Allocate(int nAllocLength, int nCharSize) = 0;
    virtual void         Free(CStringData* pData) = 0;
    virtual CStringData* Reallocate(CStringData* pData, int nAllocLength, int nCharSize) = 0;
    virtual CStringData* GetNilString() = 0;
    virtual IAtlStringMgr* Clone() = 0;
};

struct CStringData {
    IAtlStringMgr* pStringMgr;  // offset 0
    int   nDataLength;          // offset 8
    int   nAllocLength;         // offset 12
    long  nRefs;                // offset 16 (long == 4 bytes on Win64/LLP64)
    // wchar_t data[] follows at offset 24

    wchar_t* data() { return reinterpret_cast<wchar_t*>(this + 1); }
    const wchar_t* data() const { return reinterpret_cast<const wchar_t*>(this + 1); }

    void AddRef() {
#ifdef _MSC_VER
        _InterlockedIncrement(&nRefs);
#else
        __atomic_add_fetch(&nRefs,1,__ATOMIC_SEQ_CST);
#endif
    }
    bool Release() {
#ifdef _MSC_VER
        return _InterlockedDecrement(&nRefs)<=0;
#else
        return __atomic_sub_fetch(&nRefs,1,__ATOMIC_SEQ_CST)<=0;
#endif
    }
    bool IsLocked() const { return nRefs < 0; }
    bool IsShared() const { return nRefs > 1; }
};

static_assert(sizeof(CStringData) == 24, "CStringData must match ATL (24 bytes)");

inline CStringData* GetNilStringData();  // fwd (manager references it)

// The OpenMFC string manager. Its vtable layout matches IAtlStringMgr, so a MSVC
// client can call Free/Reallocate/etc. on buffers our DLL allocated (and vice versa).
class CAfxStringMgr : public IAtlStringMgr {
public:
    CAfxStringMgr():m_nil{{this,0,0,2},{0,0}} {}
    CStringData* NilData() {return &m_nil.header;}
    CStringData* Allocate(int nAllocLength, int nCharSize) override {
        if(nAllocLength<0)return nullptr;
        int32_t bytes=int32_t((uint32_t(nAllocLength)+1u)*uint32_t(nCharSize));
        size_t nBytes=sizeof(CStringData)+size_t(int64_t(bytes));
        CStringData* pData = static_cast<CStringData*>(malloc(nBytes));
        if (!pData) return nullptr;
        pData->pStringMgr = this;
        pData->nDataLength = 0;
        pData->nAllocLength = nAllocLength;
        pData->nRefs = 1;
        return pData;
    }
    void Free(CStringData* pData) override { free(pData); }
    CStringData* Reallocate(CStringData* pData, int nAllocLength, int nCharSize) override {
        if(nAllocLength<0)return nullptr;
        int32_t bytes=int32_t((uint32_t(nAllocLength)+1u)*uint32_t(nCharSize));
        size_t nBytes=sizeof(CStringData)+size_t(int64_t(bytes));
        CStringData* pNew = static_cast<CStringData*>(realloc(pData, nBytes));
        if (!pNew) return nullptr;
        pNew->nAllocLength = nAllocLength;
        return pNew;
    }
    CStringData* GetNilString() override {auto* data=NilData();data->AddRef();return data;}
    IAtlStringMgr* Clone() override { return this; }
private:
    struct {CStringData header;wchar_t zero[2];} m_nil;
};
static_assert(sizeof(CAfxStringMgr)==40,"native string manager with embedded nil data");
using OpenMFCStringMgr=CAfxStringMgr;

inline IAtlStringMgr* OpenMFC_GetStringMgr() {
    static OpenMFCStringMgr mgr;
    return &mgr;
}

// Native ATL nil data retains two manager-owned references. Each attached
// string adds one reference, so native client destruction cannot free it.
inline CStringData* GetNilStringData() {
    return static_cast<CAfxStringMgr*>(OpenMFC_GetStringMgr())->NilData();
}

// Native Unicode simple-string base: one pointer and an owning manager.
class CSimpleStringW {
public:
    explicit CSimpleStringW(IAtlStringMgr* manager) {
        if(!manager)InvalidManager();
        Attach(manager->GetNilString());
    }
    CSimpleStringW(const CSimpleStringW& source) {Attach(CloneData(source.GetData()));}
    CSimpleStringW(const wchar_t* source,int length,IAtlStringMgr* manager) {Initialize(source,length,manager);}
    CSimpleStringW(const wchar_t* source,IAtlStringMgr* manager) {
        if(!manager)InvalidManager();
        Initialize(source,StringLength(source),manager);
    }
    ~CSimpleStringW() {Release(GetData());}
    CSimpleStringW& operator=(const CSimpleStringW& source) {
        auto* old=GetData();auto* data=source.GetData();
        if(old!=data) {
            if(old->IsLocked() || old->pStringMgr!=data->pStringMgr)SetString(source.GetString(),data->nDataLength);
            else {auto* replacement=CloneData(data);Release(old);Attach(replacement);}
        }
        return *this;
    }
    CSimpleStringW& operator=(const wchar_t* source) {SetString(source);return *this;}
    const wchar_t* GetString() const {return m_pszData;}
    operator const wchar_t*() const {return m_pszData;}
    int GetLength() const {return GetData()->nDataLength;}
    int GetAllocLength() const {return GetData()->nAllocLength;}
    bool IsEmpty() const {return !GetLength();}
    CStringData* GetData() const {return reinterpret_cast<CStringData*>(m_pszData)-1;}
    IAtlStringMgr* GetManager() const {auto* manager=GetData()->pStringMgr;return manager?manager->Clone():nullptr;}
    void SetManager(IAtlStringMgr* manager) {Release(GetData());Attach(manager->GetNilString());}
    void Attach(CStringData* data) {m_pszData=reinterpret_cast<wchar_t*>(data+1);}
    static int StringLength(const wchar_t* text) {return text?int(wcslen(text)):0;}
    static void ThrowMemoryException() {AfxThrowMemoryException();}
    wchar_t GetAt(int index) const {if(index<0 || index>GetLength())InvalidLength();return m_pszData[index];}
    wchar_t operator[](int index) const {return GetAt(index);}
    void SetAt(int index,wchar_t value) {
        int length=GetLength();if(index<0 || index>=length)InvalidLength();
        if(GetData()->IsShared())Fork(length);
        m_pszData[index]=value;SetLength(length);
    }
    void Empty() {
        auto* data=GetData();if(!data->nDataLength)return;
        if(data->IsLocked()){data->nDataLength=0;m_pszData[0]=0;return;}
        auto* manager=data->pStringMgr;Release(data);Attach(manager->GetNilString());
    }
    wchar_t* GetBuffer(int minimum=0) {
        if(minimum<0)InvalidLength();
        if(GetData()->IsShared() || minimum>GetAllocLength())PrepareWrite2(minimum);
        return m_pszData;
    }
    wchar_t* GetBufferSetLength(int length) {auto* out=GetBuffer(length);SetLength(length);return out;}
    void SetLength(int length) {
        if(length<0 || length>GetAllocLength())InvalidLength();
        GetData()->nDataLength=length;m_pszData[length]=0;
    }
    void ReleaseBufferSetLength(int length) {SetLength(length);}
    void ReleaseBuffer(int length=-1) {SetLength(length==-1?(m_pszData?int(wcsnlen(m_pszData,GetAllocLength())):0):length);}
    wchar_t* LockBuffer() {GetBuffer();if(--GetData()->nRefs==0)GetData()->nRefs=-1;return m_pszData;}
    void UnlockBuffer() {if(GetData()->nRefs<0 && ++GetData()->nRefs==0)GetData()->nRefs=1;}
    void PrepareWrite2(int length) {
        auto* data=GetData();if(length<data->nDataLength)length=data->nDataLength;
        if(data->IsShared())Fork(length);
        else if(length>data->nAllocLength) {
            int capacity=data->nAllocLength;
            int extra=capacity>0x40000000?0x100000:capacity/2;
            int grown=int32_t(uint32_t(capacity)+uint32_t(extra));
            Reallocate(length>grown?length:grown);
        }
    }
    void Reallocate(int length) {
        auto* data=GetData();auto* replacement=data->pStringMgr->Reallocate(data,length,2);
        if(!replacement)AfxThrowMemoryException();Attach(replacement);
    }
    void Fork(int length) {
        auto* old=GetData();auto* text=m_pszData;int oldLength=old->nDataLength;
        auto* manager=old->pStringMgr->Clone();auto* data=manager->Allocate(length,2);
        if(!data)AfxThrowMemoryException();
        int copied=length<oldLength?length:oldLength;
        memcpy_s(Characters(data),Bytes(int32_t(uint32_t(copied)+1)),text,Bytes(int32_t(uint32_t(copied)+1)));
        data->nDataLength=oldLength;Release(old);Attach(data);
    }
    static CStringData* CloneData(CStringData* old) {
        auto* manager=old->pStringMgr->Clone();
        if(!old->IsLocked() && manager==old->pStringMgr){old->AddRef();return old;}
        auto* data=manager->Allocate(old->nDataLength,2);if(!data)AfxThrowMemoryException();
        int length=old->nDataLength;data->nDataLength=length;
        size_t bytes=Bytes(int32_t(uint32_t(length)+1));
        memcpy_s(Characters(data),bytes,Characters(old),bytes);return data;
    }
    void SetString(const wchar_t* source) {SetString(source,StringLength(source));}
    void SetString(const wchar_t* source,int length) {
        if(!length){Empty();return;}
        if(!source || length<0)InvalidLength();
        uintptr_t offset=uintptr_t(intptr_t(uintptr_t(source)-uintptr_t(m_pszData))>>1);int oldLength=GetLength();
        auto* out=GetBuffer(length);
        if(offset<=uintptr_t(oldLength))memmove_s(out,Bytes(GetAllocLength()),out+offset,Bytes(length));
        else memcpy_s(out,Bytes(GetAllocLength()),source,Bytes(length));
        SetLength(length);
    }
    void Append(const wchar_t* source,int maximum) {
        if(maximum<0)InvalidLength();
        auto* old=m_pszData;int length=GetLength();int added=source?int(wcsnlen(source,size_t(maximum))):0;
        if(length>INT32_MAX-added)InvalidLength();
        int total=length+added;if(total<0)InvalidLength();
        uintptr_t offset=uintptr_t(intptr_t(uintptr_t(source)-uintptr_t(old))>>1);wchar_t* out=GetBuffer(total);
        if(offset<=uintptr_t(length))source=out+offset;
        if(added)memcpy_s(out+length,Bytes(added),source,Bytes(added));SetLength(total);
    }
    void Append(const wchar_t* source) {Append(source,StringLength(source));}
    void Append(const CSimpleStringW& source) {Append(source.GetString(),source.GetLength());}
    void AppendChar(wchar_t value) {
        int length=GetLength();if(length==INT32_MAX)InvalidLength();
        GetBuffer(length+1)[length]=value;SetLength(length+1);
    }
    CSimpleStringW& operator+=(const wchar_t* source) {Append(source);return *this;}
    CSimpleStringW& operator+=(const CSimpleStringW& source) {Append(source);return *this;}
    CSimpleStringW& operator+=(wchar_t value) {AppendChar(value);return *this;}
    void Truncate(int length) {GetBuffer(length);SetLength(length);}
    void Preallocate(int length) {GetBuffer(length);}
    void FreeExtra() {
        auto* old=GetData();auto* text=m_pszData;int length=old->nDataLength;
        if(length==old->nAllocLength || old->IsLocked())return;
        auto* data=old->pStringMgr->Allocate(length,2);
        if(data){if(length)memcpy_s(Characters(data),Bytes(length),text,Bytes(length));Release(old);Attach(data);}
        SetLength(length);
    }
    static void Concatenate(CSimpleStringW& text,const wchar_t* first,int firstLength,const wchar_t* second,int secondLength) {
        int length=int32_t(uint32_t(firstLength)+uint32_t(secondLength));
        wchar_t* out=text.GetBuffer(length);
        if(firstLength)memcpy_s(out,Bytes(firstLength),first,Bytes(firstLength));
        if(secondLength)memcpy_s(out+firstLength,Bytes(secondLength),second,Bytes(secondLength));
        text.SetLength(length);
    }
    static wchar_t* Characters(CStringData* data) {return reinterpret_cast<wchar_t*>(data+1);}
protected:
    static size_t Bytes(int characters) {return size_t(int64_t(characters))*sizeof(wchar_t);}
    explicit CSimpleStringW(CStringData* data):m_pszData(Characters(data)) {}
    void Initialize(const wchar_t* source,int length,IAtlStringMgr* manager) {
        if(!manager)InvalidManager();
        if(!source && length)InvalidLength();
        auto* data=manager->Allocate(length,2);
        if(!data)AfxThrowMemoryException();
        Attach(data);SetLength(length);
        if(length)memcpy_s(m_pszData,Bytes(length),source,Bytes(length));
    }
    wchar_t* m_pszData;
    static void Release(CStringData* data) {if(data->Release())data->pStringMgr->Free(data);}
    static void InvalidLength() {AfxThrowOleException(static_cast<long>(0x80070057u));}
    static void InvalidManager() {AfxThrowOleException(static_cast<long>(0x80004005u));}
};
static_assert(sizeof(CSimpleStringW)==8,"native Unicode simple string");

struct tagVARIANT;

// CString (Unicode) - ABI compatible with MFC CStringW
class CString : public CSimpleStringW {
public:
    CString() noexcept:CSimpleStringW(OpenMFC_GetStringMgr()) {}
    explicit CString(IAtlStringMgr* manager) noexcept:CSimpleStringW(manager) {}
    CString(const wchar_t*);
    CString(const wchar_t*,IAtlStringMgr*);
    CString(const wchar_t* source,int length):CSimpleStringW(source,length,OpenMFC_GetStringMgr()) {}
    CString(const wchar_t* source,int length,IAtlStringMgr* manager):CSimpleStringW(source,length,manager) {}
    CString(const char*);
    CString(const char*,IAtlStringMgr*);
    CString(const char*,int);
    CString(const char*,int,IAtlStringMgr*);
    CString(unsigned char* source):CString(reinterpret_cast<const char*>(source)) {}
    CString(const unsigned char*);
    CString(const unsigned char*,IAtlStringMgr*);
    CString(wchar_t,int);
    CString(char,int);
    CString(const CString& source):CSimpleStringW(source) {}
    CString(const tagVARIANT&);
    CString(const tagVARIANT&,IAtlStringMgr*);
    CString& operator=(const tagVARIANT&);
    CString& operator+=(const tagVARIANT&);
    CString& operator=(const CString& source) {CSimpleStringW::operator=(source);return *this;}
    CString& operator=(const wchar_t* source) {SetString(source);return *this;}
    CString& operator=(const char*);
    CString& operator=(const unsigned char* source) {return operator=(reinterpret_cast<const char*>(source));}
    CString& operator=(wchar_t value) {wchar_t text[]={value,0};SetString(text);return *this;}
    CString& operator=(char value) {char text[]={value,0};return operator=(text);}
    CString& operator+=(const wchar_t* source) {Append(source);return *this;}
    CString& operator+=(const CString& source) {Append(source);return *this;}

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
        constexpr int kInitialFormatBuffer = 256;
        constexpr int kMaxFormatBuffer = 32768;
        int nBufLen = kInitialFormatBuffer;
        while (true) {
            wchar_t* pBuf = GetBuffer(nBufLen);
            va_list argsCopy;
            va_copy(argsCopy, args);
            int nWritten = vswprintf(pBuf, nBufLen, pszFormat, argsCopy);
            va_end(argsCopy);

            if (nWritten >= 0 && nWritten < nBufLen) {
                ReleaseBuffer(nWritten);
                return;
            }

            ReleaseBuffer(0);
            if (nBufLen >= kMaxFormatBuffer) {
                break;
            }
            if (nWritten >= nBufLen && nWritten > 0) {
                nBufLen = nWritten + 1;
            } else {
                nBufLen *= 2;
            }
            if (nBufLen > kMaxFormatBuffer) {
                nBufLen = kMaxFormatBuffer;
            }
        }
        Empty();
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
    // Note: Windows defines LoadString as a macro to LoadStringW/LoadStringA
    // We undef it here to declare our own method, then redefine if needed
    #ifdef LoadString
    #undef LoadString
    #endif
    int LoadString(UINT nID);
#ifdef _WIN32
    int LoadString(HINSTANCE module,UINT id);
    int LoadString(HINSTANCE module,UINT id,unsigned short language);
#endif

private:
    friend class CDatabase;
    friend class CFrameWnd;
    struct OwnedData {};
    CString(CStringData* data,OwnedData) : CSimpleStringW(data) {}
    static CString FromOwnedData(CStringData* data) {return CString(data,OwnedData{});}

    CStringData* GetData() const {
        return reinterpret_cast<CStringData*>(m_pszData) - 1;
    }
    
    // ... (rest of private members)
    void Convert(const char* source,int length);
    static CStringData* AllocData(int nLength) {
        // Allocate via the string manager so the buffer carries a valid pStringMgr
        // (an MSVC client releasing the last ref calls pStringMgr->Free through it).
        CStringData* pData = OpenMFC_GetStringMgr()->Allocate(nLength, sizeof(wchar_t));
        if (pData == nullptr) {
            AfxThrowMemoryException();
        }
        pData->data()[0] = L'\0';
        return pData;
    }

    void AllocBuffer(int nLength) {
        CStringData* pData = AllocData(nLength);
        m_pszData = pData->data();
    }

    static void FreeBuffer(CStringData* pData) {
        if (pData != GetNilStringData()) {
            pData->pStringMgr->Free(pData);
        }
    }
};

static_assert(sizeof(CString)==8,"native Unicode CString");

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
void AfxFormatStrings(CString& rString, UINT nIDS, const wchar_t* const* rglpsz, int nString);
void AfxFormatStrings(CString& rString, const wchar_t* lpszFormat, const wchar_t* const* rglpsz, int nString);

wchar_t* AfxA2WHelper(wchar_t* lpw, const char* lpa, int nChars);
char* AfxW2AHelper(char* lpa, const wchar_t* lpw, int nChars);
void AfxBSTR2CString(CString* pStr, wchar_t* bstr);
int AfxComparePath(const wchar_t* pszPath1, const wchar_t* pszPath2);

// Display message box
int AfxMessageBox(const wchar_t* lpszText, UINT nType = 0, UINT nIDHelp = 0);
int AfxMessageBox(UINT nIDPrompt, UINT nType = 0, UINT nIDHelp = 0);

#include "afxstra.h"
