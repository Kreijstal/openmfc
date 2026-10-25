// Native MSVC member return-buffer ABI and database string accessors.
#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxwin.h"
#include "openmfc/afxdb.h"
#include <wincrypt.h>
#include <algorithm>
#include <climits>
#include <cstdint>
#include <cstring>
#include <new>
#ifdef __GNUC__
#define MS_ABI __attribute__((ms_abi))
#else
#define MS_ABI
#endif
extern "C" void MS_ABI impl__AfxThrowMemoryException__YAXXZ();
extern "C" void MS_ABI impl__AfxThrowOleException__YAXJ_Z(long);
namespace {
CStringData* Data(const CString& string) {
    return reinterpret_cast<CStringData*>(*reinterpret_cast<wchar_t* const*>(&string))-1;
}
void Attach(CString& string,CStringData* data) {*reinterpret_cast<wchar_t**>(&string)=reinterpret_cast<wchar_t*>(data+1);}
void Release(CStringData* data) {if(data->Release())data->pStringMgr->Free(data);}
CStringData* Allocate(IAtlStringMgr* manager,int length) {
    auto* data=manager->Allocate(length,sizeof(wchar_t));
    if(!data)impl__AfxThrowMemoryException__YAXXZ();return data;
}
CStringData* Clone(const CString& source) {
    auto* data=Data(source);auto* manager=data->pStringMgr->Clone();CStringData* clone;
    if(!data->IsLocked() && manager==data->pStringMgr) {clone=data;clone->AddRef();}
    else {
        clone=Allocate(manager,data->nDataLength);
        std::memcpy(clone+1,data+1,size_t(data->nDataLength+1)*sizeof(wchar_t));clone->nDataLength=data->nDataLength;
    }
    return clone;
}
wchar_t* Prepare(CString& string,int required) {
    if(required<0)impl__AfxThrowOleException__YAXJ_Z(E_INVALIDARG);
    auto* data=Data(string);required=std::max(required,data->nDataLength);
    if(data->IsShared()) {
        auto* copy=Allocate(data->pStringMgr->Clone(),required);
        std::memcpy(copy+1,data+1,size_t(data->nDataLength+1)*sizeof(wchar_t));copy->nDataLength=data->nDataLength;
        Release(data);data=copy;Attach(string,data);
    } else if(data->nAllocLength<required) {
        int64_t capacity=data->nAllocLength;
        capacity+=capacity>1024LL*1024*1024?1024*1024:capacity/2;
        capacity=std::max<int64_t>(capacity,required);
        if(capacity>INT_MAX)impl__AfxThrowMemoryException__YAXXZ();
        auto* grown=data->pStringMgr->Reallocate(data,int(capacity),sizeof(wchar_t));
        if(!grown)impl__AfxThrowMemoryException__YAXXZ();data=grown;Attach(string,data);
    }
    return reinterpret_cast<wchar_t*>(data+1);
}
void SetLength(CString& string,int length) {
    auto* data=Data(string);
    if(length<0 || length>data->nAllocLength)impl__AfxThrowOleException__YAXJ_Z(E_INVALIDARG);
    data->nDataLength=length;reinterpret_cast<wchar_t*>(data+1)[length]=0;
}
void Empty(CString& string) {
    auto* data=Data(string);
    if(!data->nDataLength)return;
    if(data->IsLocked())SetLength(string,0);
    else {
        auto* manager=data->pStringMgr;Release(data);Attach(string,manager->GetNilString());
    }
}
void SetString(CString& string,const wchar_t* text,int length) {
    if(!length) {Empty(string);return;}
    if(!text || length<0)impl__AfxThrowOleException__YAXJ_Z(E_INVALIDARG);
    auto* buffer=Prepare(string,length);
    if(length)std::memmove(buffer,text,size_t(length)*sizeof(wchar_t));SetLength(string,length);
}
CStringData* Detach(CString& string) {
    auto* data=Data(string);auto* nil=GetNilStringData();nil->AddRef();Attach(string,nil);return data;
}
CString* ConstructResult(CString* result,CStringData* data) {
    new(result) CString();Release(Data(*result));Attach(*result,data);return result;
}
CStringData* ConnectionData(const CDatabase& database) {
    // Adopt the cloned buffer directly: an unoptimized compiler may copy a
    // value returned by an inline factory and call the manager's Clone twice.
    auto* cloned=Clone(database.m_strConnect);
    CString result;
    Release(Data(result));Attach(result,cloned);
    if(!result.IsEmpty())return Detach(result);
    DATA_BLOB decoded;
    static_assert(sizeof(CDatabase::ConnectionBlob)==sizeof(DATA_BLOB),"native encrypted connection blob layout");
    if(::CryptUnprotectData(reinterpret_cast<DATA_BLOB*>(const_cast<CDatabase::ConnectionBlob*>(&database.m_blobConnect)),nullptr,nullptr,nullptr,nullptr,0,&decoded)) {
        auto* text=reinterpret_cast<const wchar_t*>(decoded.pbData);
        SetString(result,text,text?static_cast<int>(std::wcslen(text)):0);
        ::LocalFree(decoded.pbData);
    }
    return Detach(result);
}
CStringData* DatabaseNameData(const CDatabase& database) {
    CString result;SQLSMALLINT bytes;
    RETCODE code=::SQLGetInfoW(database.m_hdbc,SQL_DATABASE_NAME,nullptr,0,&bytes);
    if(!database.Check(code))return Detach(result);
    // Retail sign-extends the driver length, then uses an unsigned shift.
    int length=int(uint64_t(int64_t(bytes))>>1);
    auto* buffer=Prepare(result,length);SetLength(result,length);
    SQLSMALLINT capacity=SQLSMALLINT(uint16_t(bytes)+sizeof(wchar_t));
    code=::SQLGetInfoW(database.m_hdbc,SQL_DATABASE_NAME,buffer,capacity,&bytes);
    auto* data=Data(result);int actual=0;
    while(actual<data->nAllocLength && buffer[actual])++actual;
    SetLength(result,actual);
    if(!database.Check(code))Empty(result);
    return Detach(result);
}
}
// The export adapters below construct directly in the caller's return buffer.
CString CDatabase::GetConnect() const {
    return CString::FromOwnedData(ConnectionData(*this));
}
CString CDatabase::GetDatabaseName() const {
    return CString::FromOwnedData(DatabaseNameData(*this));
}
// Symbol: ?GetConnect@CDatabase@@QEBA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@XZ
extern "C" CString* MS_ABI impl__GetConnect_CDatabase__QEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__XZ(
    const CDatabase* database,CString* result) {return ConstructResult(result,ConnectionData(*database));}
// Symbol: ?GetDatabaseName@CDatabase@@QEBA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@XZ
extern "C" CString* MS_ABI impl__GetDatabaseName_CDatabase__QEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__XZ(
    const CDatabase* database,CString* result) {return ConstructResult(result,DatabaseNameData(*database));}
