#pragma once
#include "afxstr.h"
#include <climits>

// Native ANSI simple-string base: one character pointer and an owning manager.
class CSimpleStringA {
public:
    explicit CSimpleStringA(IAtlStringMgr* manager) {
        if(!manager)InvalidManager();
        Attach(manager->GetNilString());
    }
    CSimpleStringA(const CSimpleStringA& source) {Attach(CloneData(source.GetData()));}
    CSimpleStringA(const char* source,int length,IAtlStringMgr* manager) {
        if(!manager)InvalidManager();
        if(!source && length)InvalidLength();
        auto* data=manager->Allocate(length,1);
        if(!data)AfxThrowMemoryException();
        Attach(data);SetLength(length);
        if(length)memcpy_s(m_pszData,size_t(length),source,size_t(length));
    }
    CSimpleStringA(const char* source,IAtlStringMgr* manager)
        :CSimpleStringA(source,StringLength(source),manager) {}
    ~CSimpleStringA() {Release(GetData());}
    CSimpleStringA& operator=(const CSimpleStringA& source) {
        auto* old=GetData();auto* data=source.GetData();
        if(old!=data) {
            if(old->IsLocked() || old->pStringMgr!=data->pStringMgr)SetString(source.GetString(),data->nDataLength);
            else {auto* replacement=CloneData(data);Release(old);Attach(replacement);}
        }
        return *this;
    }
    CSimpleStringA& operator=(const char* source) {SetString(source);return *this;}
    const char* GetString() const {return m_pszData;}
    operator const char*() const {return m_pszData;}
    int GetLength() const {return GetData()->nDataLength;}
    int GetAllocLength() const {return GetData()->nAllocLength;}
    bool IsEmpty() const {return !GetLength();}
    CStringData* GetData() const {return reinterpret_cast<CStringData*>(m_pszData)-1;}
    IAtlStringMgr* GetManager() const {auto* manager=GetData()->pStringMgr;return manager?manager->Clone():nullptr;}
    void SetManager(IAtlStringMgr* manager) {Release(GetData());Attach(manager->GetNilString());}
    void Attach(CStringData* data) {m_pszData=reinterpret_cast<char*>(data+1);}
    static int StringLength(const char* text) {return text?int(strlen(text)):0;}
    static void ThrowMemoryException() {AfxThrowMemoryException();}
    char GetAt(int index) const {if(index<0 || index>GetLength())InvalidLength();return m_pszData[index];}
    char operator[](int index) const {return GetAt(index);}
    void SetAt(int index,char value) {
        int length=GetLength();if(index<0 || index>=length)InvalidLength();
        if(GetData()->IsShared())Fork(length);
        m_pszData[index]=value;SetLength(length);
    }
    void Empty() {
        auto* data=GetData();if(!data->nDataLength)return;
        if(data->IsLocked()){data->nDataLength=0;m_pszData[0]=0;return;}
        auto* manager=data->pStringMgr;Release(data);Attach(manager->GetNilString());
    }
    char* GetBuffer(int minimum=0) {
        if(minimum<0)InvalidLength();
        if(GetData()->IsShared() || minimum>GetAllocLength())PrepareWrite2(minimum);
        return m_pszData;
    }
    char* GetBufferSetLength(int length) {auto* out=GetBuffer(length);SetLength(length);return out;}
    void SetLength(int length) {
        if(length<0 || length>GetAllocLength())InvalidLength();
        GetData()->nDataLength=length;m_pszData[length]=0;
    }
    void ReleaseBufferSetLength(int length) {SetLength(length);}
    void ReleaseBuffer(int length=-1) {SetLength(length==-1?(m_pszData?int(strnlen(m_pszData,GetAllocLength())):0):length);}
    char* LockBuffer() {GetBuffer();if(--GetData()->nRefs==0)GetData()->nRefs=-1;return m_pszData;}
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
        auto* data=GetData();auto* replacement=data->pStringMgr->Reallocate(data,length,1);
        if(!replacement)AfxThrowMemoryException();Attach(replacement);
    }
    void Fork(int length) {
        auto* old=GetData();auto* text=m_pszData;int oldLength=old->nDataLength;
        auto* manager=old->pStringMgr->Clone();auto* data=manager->Allocate(length,1);
        if(!data)AfxThrowMemoryException();
        int copied=length<oldLength?length:oldLength;
        memcpy_s(Characters(data),size_t(int32_t(uint32_t(copied)+1)),text,size_t(int32_t(uint32_t(copied)+1)));
        data->nDataLength=oldLength;Release(old);Attach(data);
    }
    static CStringData* CloneData(CStringData* old) {
        auto* manager=old->pStringMgr->Clone();
        if(!old->IsLocked() && manager==old->pStringMgr){old->AddRef();return old;}
        auto* data=manager->Allocate(old->nDataLength,1);if(!data)AfxThrowMemoryException();
        int length=old->nDataLength;data->nDataLength=length;
        size_t bytes=size_t(int32_t(uint32_t(length)+1));
        memcpy_s(Characters(data),bytes,Characters(old),bytes);return data;
    }
    void SetString(const char* source) {SetString(source,StringLength(source));}
    void SetString(const char* source,int length) {
        if(!length){Empty();return;}
        if(!source || length<0)InvalidLength();
        uintptr_t offset=uintptr_t(source)-uintptr_t(m_pszData);int oldLength=GetLength();
        auto* out=GetBuffer(length);
        if(offset<=uintptr_t(oldLength))memmove_s(out,GetAllocLength(),out+offset,size_t(length));
        else memcpy_s(out,GetAllocLength(),source,size_t(length));
        SetLength(length);
    }
    void Append(const char* source,int maximum) {
        if(maximum<0)InvalidLength();
        auto* old=m_pszData;int length=GetLength();int added=source?int(strnlen(source,size_t(maximum))):0;
        if(length>INT_MAX-added)InvalidLength();
        int total=length+added;if(total<0)InvalidLength();
        uintptr_t offset=uintptr_t(source)-uintptr_t(old);char* out=GetBuffer(total);
        if(offset<=uintptr_t(length))source=out+offset;
        if(added)memcpy_s(out+length,size_t(added),source,size_t(added));SetLength(total);
    }
    void Append(const char* source) {Append(source,StringLength(source));}
    void Append(const CSimpleStringA& source) {Append(source.GetString(),source.GetLength());}
    void AppendChar(char value) {
        int length=GetLength();if(length==INT_MAX)InvalidLength();
        GetBuffer(length+1)[length]=value;SetLength(length+1);
    }
    CSimpleStringA& operator+=(const char* source) {Append(source);return *this;}
    CSimpleStringA& operator+=(const CSimpleStringA& source) {Append(source);return *this;}
    CSimpleStringA& operator+=(char value) {AppendChar(value);return *this;}
    void Truncate(int length) {GetBuffer(length);SetLength(length);}
    void Preallocate(int length) {GetBuffer(length);}
    void FreeExtra() {
        auto* old=GetData();auto* text=m_pszData;int length=old->nDataLength;
        if(length==old->nAllocLength || old->IsLocked())return;
        auto* data=old->pStringMgr->Allocate(length,1);
        if(data){if(length)memcpy_s(Characters(data),size_t(length),text,size_t(length));Release(old);Attach(data);}
        SetLength(length);
    }
    static void Concatenate(CSimpleStringA& text,const char* first,int firstLength,const char* second,int secondLength) {
        int length=int32_t(uint32_t(firstLength)+uint32_t(secondLength));
        char* out=text.GetBuffer(length);
        if(firstLength)memcpy_s(out,size_t(firstLength),first,size_t(firstLength));
        if(secondLength)memcpy_s(out+firstLength,size_t(secondLength),second,size_t(secondLength));
        text.SetLength(length);
    }
    static char* Characters(CStringData* data) {return reinterpret_cast<char*>(data+1);}
protected:
    char* m_pszData;
    static void Release(CStringData* data) {if(data->Release())data->pStringMgr->Free(data);}
    static void InvalidLength() {AfxThrowOleException(static_cast<long>(0x80070057u));}
    static void InvalidManager() {AfxThrowOleException(static_cast<long>(0x80004005u));}
};
static_assert(sizeof(CSimpleStringA)==8,"native ANSI simple string");

struct tagVARIANT;
class CStringA : public CSimpleStringA {
public:
    CStringA():CSimpleStringA(OpenMFC_GetStringMgr()) {}
    explicit CStringA(IAtlStringMgr* manager):CSimpleStringA(manager) {}
    CStringA(const CStringA& source):CSimpleStringA(source) {}
    CStringA(const char* source);
    CStringA(const char* source,IAtlStringMgr* manager);
    CStringA(const char* source,int length):CSimpleStringA(source,length,OpenMFC_GetStringMgr()) {}
    CStringA(const char* source,int length,IAtlStringMgr* manager):CSimpleStringA(source,length,manager) {}
    CStringA(const unsigned char* source):CStringA(reinterpret_cast<const char*>(source)) {}
    CStringA(const unsigned char* source,IAtlStringMgr* manager):CStringA(reinterpret_cast<const char*>(source),manager) {}
    CStringA(const wchar_t* source);
    CStringA(const wchar_t* source,IAtlStringMgr* manager);
    CStringA(const wchar_t* source,int length);
    CStringA(const wchar_t* source,int length,IAtlStringMgr* manager);
    CStringA(const tagVARIANT&);
    CStringA(const tagVARIANT&,IAtlStringMgr*);
    CStringA& operator=(const tagVARIANT&);
    using CSimpleStringA::operator+=;
    CStringA& operator+=(const wchar_t*);
    CStringA& operator+=(const tagVARIANT&);
    CStringA(char value,int count);
    CStringA(wchar_t value,int count);
    CStringA& operator=(const CStringA& source) {CSimpleStringA::operator=(source);return *this;}
    CStringA& operator=(const char* source) {SetString(source);return *this;}
    CStringA& operator=(const unsigned char* source) {SetString(reinterpret_cast<const char*>(source));return *this;}
    CStringA& operator=(const wchar_t* source);
    CStringA& operator=(char value) {char text[]={value,0};SetString(text);return *this;}
    CStringA& operator=(wchar_t value);
    int LoadString(UINT id);
    int LoadString(HINSTANCE module,UINT id);
    int LoadString(HINSTANCE module,UINT id,unsigned short language);
private:
    void Convert(const wchar_t*,int);
};
static_assert(sizeof(CStringA)==8,"native ANSI CString");
