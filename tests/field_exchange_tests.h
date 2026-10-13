#pragma once
#include "openmfc/afxdb.h"
extern "C" void* MS_ABI impl___0CFieldExchange__QEAA_IPEAVCRecordset__PEAX_Z(void*,UINT,CRecordset*,void*);
extern "C" BOOL MS_ABI impl__IsFieldType_CFieldExchange__QEAAHPEAI_Z(CFieldExchange*,UINT*);
namespace field_exchange_test {
using Construct=void* (MS_ABI*)(void*,UINT,CRecordset*,void*);
using Enumerate=BOOL (MS_ABI*)(CFieldExchange*,UINT*);
struct Snapshot {UINT operation,type,field,columns,params;BOOL accepted;};
std::vector<Snapshot> enumerate(Construct construct,Enumerate next,CRecordset* recordset) {
    std::vector<Snapshot> results;
    const UINT types[]={0,1,2,4,6,7,UINT(-1)};
    const UINT operations[]={0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,UINT(-1)};
    bool counts=true,filters=true;
    for(UINT operation:operations) {
        CFieldExchange fx(operation,recordset);
        construct(&fx,operation,recordset,nullptr);
        for(UINT type:types) {
            fx.SetFieldType(type);
            UINT oldColumns=fx.m_nFields,oldParams=fx.m_nParams,field=99;
            BOOL accepted=next(&fx,&field);
            counts &= field==(type==0 ? oldColumns+1 : oldParams+1) &&
                fx.m_nFields==oldColumns+(type==0) && fx.m_nParams==oldParams+(type!=0);
            bool expected=operation<2 ? type!=0 : operation==5 ? (type==0 || type==2 || type==4) : type==0;
            filters &= !!accepted==expected;
            results.push_back({operation,type,field,fx.m_nFields,fx.m_nParams,accepted});
        }
    }
    check(counts,"field exchange counts skipped fields and overwrites the supplied index across mixed field types");
    check(filters,"field exchange routes all native operations, including output and inout parameter fixup");
    return results;
}
}
static void fieldExchange() {
    using namespace field_exchange_test;
    alignas(8) unsigned char recordsetBytes[40]{};
    *reinterpret_cast<HSTMT*>(recordsetBytes+8)=reinterpret_cast<HSTMT>(1);
    auto* recordset=reinterpret_cast<CRecordset*>(recordsetBytes);
    alignas(8) unsigned char storage[104];memset(storage,0xa5,sizeof(storage));
    int selectedField=0;
    auto* fx=static_cast<CFieldExchange*>(impl___0CFieldExchange__QEAA_IPEAVCRecordset__PEAX_Z(storage,CFieldExchange::Fixup,recordset,&selectedField));
    check(fx==reinterpret_cast<CFieldExchange*>(storage) && sizeof(CFieldExchange)==96 && fx->m_nOperation==5 &&
          fx->m_prs==recordset && fx->m_pvField==&selectedField && fx->m_nFieldType==UINT(-1) &&
          !fx->m_pstr && !fx->m_bField && !fx->m_nFields && !fx->m_nParams && !fx->m_nParamFields && !fx->m_hstmt &&
          fx->m_lDefaultLBFetchSize==0x10000 && fx->m_lDefaultLBReallocSize==0x10000,
          "field exchange constructor initializes its native 96-byte object without a vtable");
    check(fx->m_nFieldFound==0xa5a5a5a5 && reinterpret_cast<UINT_PTR>(fx->m_lpszSeparator)==UINT_PTR(0xa5a5a5a5a5a5a5a5ULL) &&
          *reinterpret_cast<UINT64*>(storage+96)==0xa5a5a5a5a5a5a5a5ULL,
          "field exchange constructor preserves native untouched fields and the guard beyond its object");
    fx->SetFieldType(CFieldExchange::outputColumn);
    check(fx->m_nFieldType==0 && !fx->m_nFields && !fx->m_nParams,"inline SetFieldType only changes the native field-type member");
    bool threw=false;try {impl__IsFieldType_CFieldExchange__QEAAHPEAI_Z(fx,nullptr);}catch(int value) {threw=value==19;}
    check(threw && !fx->m_nFields && !fx->m_nParams,"null field index raises invalid-argument failure before advancing counters");
    for(int invalid=0;invalid<2;++invalid) {
        alignas(8) unsigned char untouched[96];memset(untouched,0xa5,sizeof(untouched));
        *reinterpret_cast<HSTMT*>(recordsetBytes+8)=nullptr;threw=false;
        try {impl___0CFieldExchange__QEAA_IPEAVCRecordset__PEAX_Z(untouched,0,invalid?recordset:nullptr,nullptr);}catch(int value) {threw=value==19;}
        bool unchanged=true;for(auto byte:untouched)unchanged &= byte==0xa5;
        check(threw && unchanged,"invalid recordset or statement raises failure before constructor writes");
    }
    *reinterpret_cast<HSTMT*>(recordsetBytes+8)=reinterpret_cast<HSTMT>(1);
    auto production=enumerate(impl___0CFieldExchange__QEAA_IPEAVCRecordset__PEAX_Z,impl__IsFieldType_CFieldExchange__QEAAHPEAI_Z,recordset);
    wchar_t nativePath[1024];
    if(::GetEnvironmentVariableW(L"OPENMFC_NATIVE_DATABASE_DLL",nativePath,1024)) {
        // Inspected MSVC 14.51.36231 release Unicode DLL function entries.
        HMODULE module=::LoadLibraryW(nativePath);check(module!=nullptr,"load native MFC for field-exchange differential checks");
        if(module) {
            auto* base=reinterpret_cast<char*>(module);
            auto construct=reinterpret_cast<Construct>(base+0x1fe690);
            auto next=reinterpret_cast<Enumerate>(base+0x1fe6e0);
            alignas(8) unsigned char nativeStorage[104];memset(nativeStorage,0xa5,sizeof(nativeStorage));
            construct(nativeStorage,CFieldExchange::Fixup,recordset,&selectedField);
            impl___0CFieldExchange__QEAA_IPEAVCRecordset__PEAX_Z(storage,CFieldExchange::Fixup,recordset,&selectedField);
            check(memcmp(storage,nativeStorage,sizeof(storage))==0,"production constructor matches native MFC byte for byte, including untouched fields");
            auto native=enumerate(construct,next,recordset);
            check(production.size()==native.size() && memcmp(production.data(),native.data(),production.size()*sizeof(Snapshot))==0,
                  "native and production field-exchange operation/type matrices match");
            ::FreeLibrary(module);
        }
    }
}
