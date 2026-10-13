// Native RFX cache type dispatch and recordset snapshot/restore.
#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxwin.h"
#include "openmfc/afxdb.h"
#include <mbstring.h>
#include <cstring>
#include <algorithm>
#include <cstdint>
#include <climits>
#ifdef __GNUC__
#define MS_ABI __attribute__((ms_abi))
#else
#define MS_ABI
#endif
extern "C" void MS_ABI impl__AfxThrowInvalidArgException__YAXXZ();
extern "C" void MS_ABI impl__AfxThrowMemoryException__YAXXZ();
extern "C" void MS_ABI impl__AfxThrowUserException__YAXXZ();
extern "C" void MS_ABI impl__AfxThrowOleException__YAXJ_Z(long);
namespace {
template<class T> T Read(const void* p) {T value;std::memcpy(&value,p,sizeof(value));return value;}
bool Inline(int type) {return (type>=1 && type<=4) || type==6;}
void Require(bool condition) {if(!condition)impl__AfxThrowInvalidArgException__YAXXZ();}
void Release(CStringData* data) {if(data->Release())data->pStringMgr->Free(data);}
void AssignString(void* destination,const void* source,int charSize) {
    auto*& target=*static_cast<char**>(destination);auto* text=Read<char*>(source);
    auto* old=reinterpret_cast<CStringData*>(target)-1;
    auto* src=reinterpret_cast<CStringData*>(text)-1;
    if(old==src)return;
    int length=src->nDataLength;
    if(old->IsLocked() || old->pStringMgr!=src->pStringMgr) {
        int required=std::max(length,old->nDataLength);
        if(old->IsShared()) {
            auto* fresh=old->pStringMgr->Clone()->Allocate(required,charSize);
            if(!fresh)impl__AfxThrowMemoryException__YAXXZ();
            std::memcpy(fresh+1,old+1,size_t(old->nDataLength+1)*charSize);
            fresh->nDataLength=old->nDataLength;Release(old);old=fresh;target=reinterpret_cast<char*>(old+1);
        } else if(old->nAllocLength<required) {
            int64_t capacity=old->nAllocLength;
            capacity=capacity>1024LL*1024*1024 ? capacity+1024*1024 : capacity+capacity/2;
            capacity=std::max<int64_t>(capacity,required);
            if(capacity>INT_MAX)impl__AfxThrowMemoryException__YAXXZ();
            auto* fresh=old->pStringMgr->Reallocate(old,int(capacity),charSize);
            if(!fresh)impl__AfxThrowMemoryException__YAXXZ();
            old=fresh;target=reinterpret_cast<char*>(old+1);
        }
        std::memmove(target,text,size_t(length)*charSize);
        std::memset(target+size_t(length)*charSize,0,charSize);old->nDataLength=length;
    } else {
        auto* manager=src->pStringMgr->Clone();CStringData* fresh;
        if(!src->IsLocked() && manager==src->pStringMgr) {fresh=src;fresh->AddRef();}
        else {
            fresh=manager->Allocate(length,charSize);
            if(!fresh)impl__AfxThrowMemoryException__YAXXZ();
            std::memcpy(fresh+1,src+1,size_t(length+1)*charSize);fresh->nDataLength=length;
        }
        Release(old);target=reinterpret_cast<char*>(fresh+1);
    }
}
}
// Symbol: ?AfxCompareValueByRef@@YAHPEAX0H@Z
extern "C" BOOL MS_ABI impl__AfxCompareValueByRef__YAHPEAX0H_Z(void* left,void* right,int type) {
    Require(left && right);
    switch(type) {
    case 1:case 3:case 4:return Read<uint32_t>(left)==Read<uint32_t>(right);
    case 2:return Read<BYTE>(left)==Read<BYTE>(right);
    case 6:return Read<float>(left)==Read<float>(right);
    case 7:return Read<double>(left)==Read<double>(right);
    case 8:case 22:return Read<uint64_t>(left)==Read<uint64_t>(right);
    case 9: {
        auto* a=static_cast<CByteArray*>(left);auto* b=static_cast<CByteArray*>(right);
        return a->m_nSize==b->m_nSize && !std::memcmp(a->m_pData,b->m_pData,size_t(a->m_nSize));
    }
    case 15:return !std::memcmp(left,right,16);
    case 16: {
        int a=Read<int>(static_cast<char*>(left)+8),b=Read<int>(static_cast<char*>(right)+8);
        if(a!=b)return FALSE;if(a)return a==2;
        constexpr double tolerance=5.787037037037037e-06;
        double x=Read<double>(left),y=Read<double>(right);
        return x<y+tolerance && x>y-tolerance;
    }
    case 18:return ::lstrcmpW(static_cast<wchar_t*>(left),static_cast<wchar_t*>(right))==0;
    case 19:return ::lstrcmpA(static_cast<char*>(left),static_cast<char*>(right))==0;
    case 20: {
        auto* text=Read<wchar_t*>(left);if(!text)impl__AfxThrowOleException__YAXJ_Z(E_FAIL);
        return !std::wcscmp(text,Read<wchar_t*>(right));
    }
    case 21: {
        auto* text=Read<unsigned char*>(left);if(!text)impl__AfxThrowOleException__YAXJ_Z(E_FAIL);
        return !_mbscmp(text,Read<unsigned char*>(right));
    }
    default:return FALSE;
    }
}
// Symbol: ?AfxCopyValueByRef@@YAXPEAX0PEA_JH@Z
extern "C" void MS_ABI impl__AfxCopyValueByRef__YAXPEAX0PEA_JH_Z(void* source,void* destination,SQLLEN* length,int type) {
    Require(source && destination && length);
    switch(type) {
    case 1:case 3:case 4:case 6:std::memcpy(destination,source,4);*length=4;break;
    case 2:std::memcpy(destination,source,1);*length=1;break;
    case 7:case 22:std::memcpy(destination,source,8);*length=8;break;
    case 8:std::memcpy(destination,source,8);*length=16;break; // native CTime length quirk
    case 9:static_cast<CByteArray*>(destination)->Copy(*static_cast<CByteArray*>(source));*length=static_cast<CByteArray*>(source)->m_nSize;break;
    case 15:case 16:std::memcpy(destination,source,16);*length=16;break;
    case 18: ::lstrcpyW(static_cast<wchar_t*>(destination),static_cast<wchar_t*>(source));*length=std::wcslen(static_cast<wchar_t*>(destination))*2;break;
    case 19: ::lstrcpyA(static_cast<char*>(destination),static_cast<char*>(source));*length=std::strlen(static_cast<char*>(destination));break;
    case 20:case 21:AssignString(destination,source,type==20?2:1);*length=(reinterpret_cast<CStringData*>(Read<char*>(destination))-1)->nDataLength*(type==20?2:1);break;
    default:break;
    }
}
// Symbol: ?AfxStoreField@@YAXAEAVCRecordset@@IPEAX@Z
extern "C" void MS_ABI impl__AfxStoreField__YAXAEAVCRecordset__IPEAX_Z(CRecordset* rs,UINT field,void* value) {
    Require(rs->m_rgFieldInfos && field>=1 && field<=rs->m_nFields);
    auto& info=rs->m_rgFieldInfos[field-1];info.m_bStatus=rs->GetFieldStatus(field-1);
    if(rs->IsFieldStatusNull(field-1))return;
    SQLLEN ignored;
    impl__AfxCopyValueByRef__YAXPEAX0PEA_JH_Z(value,Inline(info.m_nDataType)?&info:info.m_pvDataCache,&ignored,info.m_nDataType);
}
// Symbol: ?AfxLoadField@@YAXAEAVCRecordset@@IPEAXPEA_J@Z
extern "C" void MS_ABI impl__AfxLoadField__YAXAEAVCRecordset__IPEAXPEA_J_Z(CRecordset* rs,UINT field,void* value,SQLLEN* length) {
    if(field-1>=rs->m_nFields)impl__AfxThrowUserException__YAXXZ();
    rs->SetFieldStatus(field-1,rs->m_rgFieldInfos[field-1].m_bStatus);
    if(rs->IsFieldStatusNull(field-1)) {*length=SQL_NULL_DATA;return;}
    auto& info=rs->m_rgFieldInfos[field-1];
    impl__AfxCopyValueByRef__YAXPEAX0PEA_JH_Z(Inline(info.m_nDataType)?&info:info.m_pvDataCache,value,length,info.m_nDataType);
}
