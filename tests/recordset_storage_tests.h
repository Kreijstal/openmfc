#pragma once
#include <array>
#include <cstdlib>
extern "C" void MS_ABI impl__AllocStatusArrays_CRecordset__QEAAXXZ(CRecordset* self);
extern "C" SQLLEN* MS_ABI impl__GetFieldLengthBuffer_CRecordset__QEAAPEA_JKH_Z(CRecordset* self,DWORD field,int type);
extern "C" BYTE MS_ABI impl__GetFieldStatus_CRecordset__QEAAEK_Z(CRecordset* self,DWORD field);
extern "C" void MS_ABI impl__SetFieldStatus_CRecordset__QEAAXKE_Z(CRecordset* self,DWORD field,BYTE bits);
extern "C" void MS_ABI impl__ClearFieldStatus_CRecordset__QEAAXXZ(CRecordset* self);
extern "C" void MS_ABI impl__SetDirtyFieldStatus_CRecordset__QEAAXK_Z(CRecordset* self,DWORD field);
extern "C" void MS_ABI impl__ClearDirtyFieldStatus_CRecordset__QEAAXK_Z(CRecordset* self,DWORD field);
extern "C" BOOL MS_ABI impl__IsFieldStatusDirty_CRecordset__QEBAHK_Z(const CRecordset* self,DWORD field);
extern "C" BOOL MS_ABI impl__IsFieldStatusNull_CRecordset__QEBAHK_Z(const CRecordset* self,DWORD field);
extern "C" void MS_ABI impl__SetNullFieldStatus_CRecordset__QEAAXK_Z(CRecordset* self,DWORD field);
extern "C" void MS_ABI impl__ClearNullFieldStatus_CRecordset__QEAAXK_Z(CRecordset* self,DWORD field);
extern "C" BOOL MS_ABI impl__IsParamStatusNull_CRecordset__QEBAHK_Z(const CRecordset* self,DWORD param);
extern "C" void MS_ABI impl__SetNullParamStatus_CRecordset__QEAAXK_Z(CRecordset* self,DWORD param);
extern "C" void MS_ABI impl__ClearNullParamStatus_CRecordset__QEAAXK_Z(CRecordset* self,DWORD param);
extern "C" int MS_ABI impl__GetBoundFieldIndex_CRecordset__QEAAHPEAX_Z(CRecordset* self,void* field);
extern "C" int MS_ABI impl__GetBoundParamIndex_CRecordset__QEAAHPEAX_Z(CRecordset* self,void* param);
extern "C" void MS_ABI impl__FreeDataCache_CRecordset__QEAAXXZ(CRecordset* self);
extern "C" void MS_ABI impl__SetParamNull_CRecordset__QEAAXHH_Z(CRecordset* self,int index,BOOL null);
extern "C" void MS_ABI impl__SetFieldDirty_CRecordset__QEAAXPEAXH_Z(CRecordset* self,void* field,BOOL dirty);
extern "C" BOOL MS_ABI impl__Check_CRecordset__UEBAHF_Z(const CRecordset*,RETCODE);
extern "C" void MS_ABI impl__DoFieldExchange_CRecordset__UEAAXPEAVCFieldExchange___Z(CRecordset*,CFieldExchange*);
extern "C" void MS_ABI impl__DoBulkFieldExchange_CRecordset__UEAAXPEAVCFieldExchange___Z(CRecordset*,CFieldExchange*);
extern "C" SQLLEN MS_ABI impl__GetLBFetchSize_CRecordset__MEAA_J_J_Z(CRecordset*,SQLLEN);
extern "C" SQLLEN MS_ABI impl__GetLBReallocSize_CRecordset__MEAA_J_J_Z(CRecordset*,SQLLEN);
extern "C" void MS_ABI impl__MarkForUpdate_CRecordset__QEAAXXZ(CRecordset*);
extern "C" void MS_ABI impl__MarkForAddNew_CRecordset__QEAAXXZ(CRecordset*);
namespace recordset_storage_test {
struct API {
    decltype(&impl__AllocStatusArrays_CRecordset__QEAAXXZ) AllocStatusArrays;
    decltype(&impl__GetFieldLengthBuffer_CRecordset__QEAAPEA_JKH_Z) GetFieldLengthBuffer;
    decltype(&impl__GetFieldStatus_CRecordset__QEAAEK_Z) GetFieldStatus;
    decltype(&impl__SetFieldStatus_CRecordset__QEAAXKE_Z) SetFieldStatus;
    decltype(&impl__ClearFieldStatus_CRecordset__QEAAXXZ) ClearFieldStatus;
    decltype(&impl__SetDirtyFieldStatus_CRecordset__QEAAXK_Z) SetDirtyFieldStatus;
    decltype(&impl__ClearDirtyFieldStatus_CRecordset__QEAAXK_Z) ClearDirtyFieldStatus;
    decltype(&impl__IsFieldStatusDirty_CRecordset__QEBAHK_Z) IsFieldStatusDirty;
    decltype(&impl__IsFieldStatusNull_CRecordset__QEBAHK_Z) IsFieldStatusNull;
    decltype(&impl__SetNullFieldStatus_CRecordset__QEAAXK_Z) SetNullFieldStatus;
    decltype(&impl__ClearNullFieldStatus_CRecordset__QEAAXK_Z) ClearNullFieldStatus;
    decltype(&impl__IsParamStatusNull_CRecordset__QEBAHK_Z) IsParamStatusNull;
    decltype(&impl__SetNullParamStatus_CRecordset__QEAAXK_Z) SetNullParamStatus;
    decltype(&impl__ClearNullParamStatus_CRecordset__QEAAXK_Z) ClearNullParamStatus;
    decltype(&impl__GetBoundFieldIndex_CRecordset__QEAAHPEAX_Z) GetBoundFieldIndex;
    decltype(&impl__GetBoundParamIndex_CRecordset__QEAAHPEAX_Z) GetBoundParamIndex;
    decltype(&impl__FreeDataCache_CRecordset__QEAAXXZ) FreeDataCache;
    decltype(&impl__SetParamNull_CRecordset__QEAAXHH_Z) SetParamNull;
    decltype(&impl__SetFieldDirty_CRecordset__QEAAXPEAXH_Z) SetFieldDirty;
    decltype(&impl__Check_CRecordset__UEBAHF_Z) Check;
    decltype(&impl__DoFieldExchange_CRecordset__UEAAXPEAVCFieldExchange___Z) DoFieldExchange;
    decltype(&impl__DoBulkFieldExchange_CRecordset__UEAAXPEAVCFieldExchange___Z) DoBulkFieldExchange;
    decltype(&impl__GetLBFetchSize_CRecordset__MEAA_J_J_Z) GetLBFetchSize;
    decltype(&impl__GetLBReallocSize_CRecordset__MEAA_J_J_Z) GetLBReallocSize;
    decltype(&impl__MarkForUpdate_CRecordset__QEAAXXZ) MarkForUpdate;
    decltype(&impl__MarkForAddNew_CRecordset__QEAAXXZ) MarkForAddNew;
};
static int exchangeCalls;
static UINT exchangeOperation;
static bool exchangeValid;
static void MS_ABI exchangeCallback(CRecordset* rs,CFieldExchange* fx) {
    ++exchangeCalls;exchangeOperation=fx->m_nOperation;
    exchangeValid=fx->m_prs==rs && fx->m_nFieldType==UINT(-1) && !fx->m_pvField && !fx->m_nFields && !fx->m_nParams && !fx->m_bField;
    rs->SetDirtyFieldStatus(2);
}
struct Fixture {
    alignas(CRecordset) unsigned char storage[448]{};
    CRecordset* recordset=reinterpret_cast<CRecordset*>(storage);
    Fixture(UINT fields,UINT params) {
        new(&recordset->m_mapFieldIndex) CMapPtrToPtr;
        new(&recordset->m_mapParamIndex) CMapPtrToPtr;
        recordset->m_nFields=fields;recordset->m_nParams=params;
        memset(storage+440,0xa5,8);
    }
    ~Fixture() {
        std::free(recordset->m_rgFieldInfos);std::free(recordset->m_pbFieldFlags);
        std::free(recordset->m_pbParamFlags);std::free(recordset->m_plParamLength);
        recordset->m_mapFieldIndex.~CMapPtrToPtr();recordset->m_mapParamIndex.~CMapPtrToPtr();
    }
};
struct BinaryCache : CByteArray {
    int* destroyed;
    explicit BinaryCache(int* count):destroyed(count) {SetSize(2);}
    ~BinaryCache() override {++*destroyed;}
};
struct StringManager : IAtlStringMgr {
    int releases=0;
    CStringData* Allocate(int n,int size) override {
        auto* data=static_cast<CStringData*>(std::calloc(1,sizeof(CStringData)+(n+1)*size));
        data->pStringMgr=this;data->nRefs=1;data->nDataLength=n;data->nAllocLength=n;return data;
    }
    void Free(CStringData* data) override {++releases;std::free(data);}
    CStringData* Reallocate(CStringData*,int,int) override {std::abort();}
    CStringData* GetNilString() override {std::abort();}
    IAtlStringMgr* Clone() override {return this;}
};
std::vector<UINT64> exercise(const API& api) {
    Fixture fixture(3,2);auto* rs=fixture.recordset;
    api.AllocStatusArrays(rs);
    check(rs->m_rgFieldInfos && rs->m_pbFieldFlags && rs->m_pbParamFlags && rs->m_plParamLength,
          "recordset status allocation publishes native field records, flags and parameter lengths");
    bool zero=true;
    for(unsigned i=0;i<3*sizeof(CFieldInfo);++i)zero &= reinterpret_cast<BYTE*>(rs->m_rgFieldInfos)[i]==0;
    for(unsigned i=0;i<3;++i)zero &= rs->m_pbFieldFlags[i]==0;
    for(unsigned i=0;i<2;++i)zero &= !rs->m_pbParamFlags[i] && !rs->m_plParamLength[i];
    check(zero,"recordset status allocation zeroes complete native records and arrays");
    SQLLEN* column=api.GetFieldLengthBuffer(rs,1,0);SQLLEN* param=api.GetFieldLengthBuffer(rs,1,2);
    check(column==&rs->m_rgFieldInfos[1].m_nLength && param==rs->m_plParamLength+1 && column!=param &&
          api.GetFieldLengthBuffer(rs,1,-1)==param,"recordset length selector chooses native column or parameter storage by field type");
    *column=91;*param=192;
    auto* fields=rs->m_rgFieldInfos;auto* flags=rs->m_pbFieldFlags;auto* params=rs->m_pbParamFlags;auto* lengths=rs->m_plParamLength;
    api.AllocStatusArrays(rs);
    check(fields==rs->m_rgFieldInfos && flags==rs->m_pbFieldFlags && params==rs->m_pbParamFlags && lengths==rs->m_plParamLength &&
          *column==91 && *param==192,"repeated recordset allocation preserves buffer addresses and bound lengths");
    api.SetFieldStatus(rs,0,0x80);api.SetNullFieldStatus(rs,0);api.SetDirtyFieldStatus(rs,0);
    check(api.GetFieldStatus(rs,0)==0x83 && api.IsFieldStatusDirty(rs,0)==1 && api.IsFieldStatusNull(rs,0)==2,
          "recordset flags preserve unrelated bits and null query returns the native bit value two");
    api.ClearDirtyFieldStatus(rs,0);api.ClearNullFieldStatus(rs,0);api.SetFieldStatus(rs,0,0);
    check(rs->m_pbFieldFlags[0]==0x80,"clearing dirty and null bits preserves all remaining flags");
    api.SetNullParamStatus(rs,1);api.SetParamNull(rs,0,TRUE);
    check(api.IsParamStatusNull(rs,1)==2 && rs->m_pbParamFlags[0]==2,"recordset parameter null setters update the separate native flag array");
    api.ClearNullParamStatus(rs,1);api.SetParamNull(rs,0,FALSE);
    check(!rs->m_pbParamFlags[0] && !rs->m_pbParamFlags[1],"recordset parameter null clearing uses physical storage");
    int fieldAddress=0,paramAddress=0;
    rs->m_mapFieldIndex.SetAt(&fieldAddress,reinterpret_cast<void*>(2));
    rs->m_mapParamIndex.SetAt(&paramAddress,reinterpret_cast<void*>(1));
    check(api.GetBoundFieldIndex(rs,&fieldAddress)==2 && api.GetBoundParamIndex(rs,&paramAddress)==1 &&
          api.GetBoundFieldIndex(rs,&paramAddress)==-1,"recordset bound-index exports read one-based indexes from embedded native maps");
    api.SetFieldDirty(rs,&fieldAddress,TRUE);
    check(rs->m_pbFieldFlags[1]==1,"recordset dirty setter converts the stored one-based index to a flag offset");
    api.SetFieldDirty(rs,nullptr,TRUE);
    check(rs->m_pbFieldFlags[0]==0x81 && rs->m_pbFieldFlags[1]==1 && rs->m_pbFieldFlags[2]==1,
          "null field address marks every recordset column dirty");
    api.SetFieldDirty(rs,&paramAddress,FALSE);
    check(rs->m_pbFieldFlags[1]==1,"unknown field address does not allocate an index or change flags");
    bool checked=true;
    for(RETCODE code: {RETCODE(-2),RETCODE(-1),RETCODE(0),RETCODE(1),RETCODE(2),RETCODE(99),RETCODE(100)})
        checked &= !!api.Check(rs,code)==(code==0 || code==1 || code==99 || code==100);
    check(checked,"recordset Check recognizes the four native accepted ODBC return codes");
    bool doubled=true;
    for(SQLLEN size: {SQLLEN(-4),SQLLEN(0),SQLLEN(7),SQLLEN(65536),SQLLEN(0x7fffffffffffffffLL)}) {
        auto expected=static_cast<SQLLEN>(static_cast<SQLULEN>(size)*2);
        doubled &= api.GetLBFetchSize(rs,size)==expected && api.GetLBReallocSize(rs,size)==expected;
    }
    check(doubled,"recordset binary buffer size virtuals double their input with native 64-bit wrap behavior");
    api.DoFieldExchange(rs,nullptr);api.DoBulkFieldExchange(rs,nullptr);
    check(rs->m_nFields==3 && rs->m_nParams==2 && rs->m_pbFieldFlags[1]==1,
          "base recordset field-exchange callbacks are native no-ops");
    void* virtuals[27]{};virtuals[19]=reinterpret_cast<void*>(&exchangeCallback);
    *reinterpret_cast<void***>(rs)=virtuals;rs->m_hstmt=reinterpret_cast<HSTMT>(1);rs->m_nEditMode=37;
    exchangeCalls=0;exchangeValid=false;api.MarkForUpdate(rs);
    check(exchangeCalls==1 && exchangeOperation==7 && exchangeValid && rs->m_nEditMode==37 && rs->m_pbFieldFlags[2]==1,
          "MarkForUpdate sends native operation seven through derived field-exchange slot 19 without entering Edit");
    exchangeValid=false;api.MarkForAddNew(rs);
    check(exchangeCalls==2 && exchangeOperation==6 && exchangeValid && rs->m_nEditMode==37,
          "MarkForAddNew sends native operation six without entering AddNew");
    rs->m_hstmt=nullptr;
    std::vector<UINT64> result;
    for(unsigned i=0;i<3;++i)result.push_back(api.GetFieldStatus(rs,i));
    result.push_back(*column);result.push_back(*param);
    bool caught=false;
    try {api.SetNullFieldStatus(rs,3);}catch(CUserException*) {caught=true;}
    check(caught && rs->m_pbFieldFlags[0]==0x81,"recordset field range failure throws the shared typed user exception without changing flags");
    caught=false;try {api.GetFieldLengthBuffer(rs,2,4);}catch(CUserException*) {caught=true;}
    check(caught && *param==192,"recordset parameter length range failure preserves existing buffers");
    api.ClearFieldStatus(rs);check(!rs->m_pbFieldFlags[0] && !rs->m_pbFieldFlags[1] && !rs->m_pbFieldFlags[2],"ClearFieldStatus resets only column flags");
    check(*reinterpret_cast<UINT64*>(fixture.storage+440)==0xa5a5a5a5a5a5a5a5ULL,"recordset helpers preserve the guard beyond the native 440-byte object");
    Fixture empty(0,0);api.AllocStatusArrays(empty.recordset);
    check(!empty.recordset->m_rgFieldInfos && !empty.recordset->m_pbFieldFlags && !empty.recordset->m_pbParamFlags && !empty.recordset->m_plParamLength,
          "recordset status allocation skips empty field and parameter groups");
    Fixture cache(24,0);rs=cache.recordset;api.AllocStatusArrays(rs);
    StringManager manager;int binaryDestroyed=0;CStringData* shared=nullptr;
    for(int type=0;type<24;++type) {
        auto& f=rs->m_rgFieldInfos[type];f.m_nDataType=type;f.m_nLength=123;f.m_bStatus=0x40;
        if(type==9)f.m_pvDataCache=new BinaryCache(&binaryDestroyed);
        else if(type==20 || type==21) {
            auto* string=static_cast<char**>(std::malloc(sizeof(char*)));
            auto* data=manager.Allocate(1,type==20?2:1);
            *string=reinterpret_cast<char*>(data+1);f.m_pvDataCache=string;
            if(type==21) {data->nRefs=2;shared=data;}
        } else f.m_pvDataCache=std::malloc(32);
    }
    api.FreeDataCache(rs);bool cleared=true;
    for(int type=0;type<24;++type) {
        bool supported=type==1 || type==2 || type==3 || type==4 || type==6 || type==7 || type==8 || type==9 ||
            type==15 || type==16 || type==18 || type==19 || type==20 || type==21 || type==22;
        auto& f=rs->m_rgFieldInfos[type];cleared &= (f.m_pvDataCache==nullptr)==supported && f.m_nLength==123 && f.m_bStatus==0x40;
        result.push_back(f.m_pvDataCache==nullptr);
        if(!supported) {std::free(f.m_pvDataCache);f.m_pvDataCache=nullptr;}
    }
    check(cleared,"FreeDataCache frees native cache types, clears recognized pointers, and preserves unknown types and metadata");
    check(binaryDestroyed==1,"binary data cache cleanup calls the object's virtual deleting destructor");
    check(manager.releases==1 && shared->nRefs==1,"string data cache cleanup releases its manager-owned reference and preserves shared buffers");
    manager.Free(shared);return result;
}
}
static void recordsetFieldStorage() {
    using namespace recordset_storage_test;
    API production{
        impl__AllocStatusArrays_CRecordset__QEAAXXZ,
        impl__GetFieldLengthBuffer_CRecordset__QEAAPEA_JKH_Z,
        impl__GetFieldStatus_CRecordset__QEAAEK_Z,
        impl__SetFieldStatus_CRecordset__QEAAXKE_Z,
        impl__ClearFieldStatus_CRecordset__QEAAXXZ,
        impl__SetDirtyFieldStatus_CRecordset__QEAAXK_Z,
        impl__ClearDirtyFieldStatus_CRecordset__QEAAXK_Z,
        impl__IsFieldStatusDirty_CRecordset__QEBAHK_Z,
        impl__IsFieldStatusNull_CRecordset__QEBAHK_Z,
        impl__SetNullFieldStatus_CRecordset__QEAAXK_Z,
        impl__ClearNullFieldStatus_CRecordset__QEAAXK_Z,
        impl__IsParamStatusNull_CRecordset__QEBAHK_Z,
        impl__SetNullParamStatus_CRecordset__QEAAXK_Z,
        impl__ClearNullParamStatus_CRecordset__QEAAXK_Z,
        impl__GetBoundFieldIndex_CRecordset__QEAAHPEAX_Z,
        impl__GetBoundParamIndex_CRecordset__QEAAHPEAX_Z,
        impl__FreeDataCache_CRecordset__QEAAXXZ,
        impl__SetParamNull_CRecordset__QEAAXHH_Z,
        impl__SetFieldDirty_CRecordset__QEAAXPEAXH_Z,
        impl__Check_CRecordset__UEBAHF_Z,
        impl__DoFieldExchange_CRecordset__UEAAXPEAVCFieldExchange___Z,
        impl__DoBulkFieldExchange_CRecordset__UEAAXPEAVCFieldExchange___Z,
        impl__GetLBFetchSize_CRecordset__MEAA_J_J_Z,
        impl__GetLBReallocSize_CRecordset__MEAA_J_J_Z,
        impl__MarkForUpdate_CRecordset__QEAAXXZ,
        impl__MarkForAddNew_CRecordset__QEAAXXZ
    };
    auto expected=exercise(production);
    wchar_t path[1024];
    if(::GetEnvironmentVariableW(L"OPENMFC_NATIVE_RECORDSET_DLL",path,1024)) {
        HMODULE module=::LoadLibraryW(path);check(module!=nullptr,"load native MFC for recordset storage comparison");
        if(module) {
            // Inspected MSVC 14.51.36231 Unicode release functions.
            auto* base=reinterpret_cast<char*>(module);API native=production;
            native.AllocStatusArrays=reinterpret_cast<decltype(native.AllocStatusArrays)>(base+0x1fc760);
            native.GetFieldLengthBuffer=reinterpret_cast<decltype(native.GetFieldLengthBuffer)>(base+0x1fc950);
            native.GetFieldStatus=reinterpret_cast<decltype(native.GetFieldStatus)>(base+0x1fc9a0);
            native.SetFieldStatus=reinterpret_cast<decltype(native.SetFieldStatus)>(base+0x1fc9d0);
            native.ClearFieldStatus=reinterpret_cast<decltype(native.ClearFieldStatus)>(base+0x1fca00);
            native.SetDirtyFieldStatus=reinterpret_cast<decltype(native.SetDirtyFieldStatus)>(base+0x1fca50);
            native.ClearDirtyFieldStatus=reinterpret_cast<decltype(native.ClearDirtyFieldStatus)>(base+0x1fca80);
            native.IsFieldStatusDirty=reinterpret_cast<decltype(native.IsFieldStatusDirty)>(base+0x1fca20);
            native.IsFieldStatusNull=reinterpret_cast<decltype(native.IsFieldStatusNull)>(base+0x1fcab0);
            native.SetNullFieldStatus=reinterpret_cast<decltype(native.SetNullFieldStatus)>(base+0x1fcae0);
            native.ClearNullFieldStatus=reinterpret_cast<decltype(native.ClearNullFieldStatus)>(base+0x1fcb10);
            native.IsParamStatusNull=reinterpret_cast<decltype(native.IsParamStatusNull)>(base+0x1fcb40);
            native.SetNullParamStatus=reinterpret_cast<decltype(native.SetNullParamStatus)>(base+0x1fcb70);
            native.ClearNullParamStatus=reinterpret_cast<decltype(native.ClearNullParamStatus)>(base+0x1fcba0);
            native.GetBoundFieldIndex=reinterpret_cast<decltype(native.GetBoundFieldIndex)>(base+0x1fc860);
            native.GetBoundParamIndex=reinterpret_cast<decltype(native.GetBoundParamIndex)>(base+0x1fc890);
            native.FreeDataCache=reinterpret_cast<decltype(native.FreeDataCache)>(base+0x1fd160);
            native.SetParamNull=reinterpret_cast<decltype(native.SetParamNull)>(base+0x1f9a50);
            native.SetFieldDirty=reinterpret_cast<decltype(native.SetFieldDirty)>(base+0x1f98f0);
            native.Check=reinterpret_cast<decltype(native.Check)>(base+0x1f9e20);
            native.DoFieldExchange=reinterpret_cast<decltype(native.DoFieldExchange)>(base+0x27d0);
            native.DoBulkFieldExchange=reinterpret_cast<decltype(native.DoBulkFieldExchange)>(base+0x27d0);
            native.GetLBFetchSize=reinterpret_cast<decltype(native.GetLBFetchSize)>(base+0x1fd4a0);
            native.GetLBReallocSize=reinterpret_cast<decltype(native.GetLBReallocSize)>(base+0x1fd4a0);
            native.MarkForUpdate=reinterpret_cast<decltype(native.MarkForUpdate)>(base+0x1fd070);
            native.MarkForAddNew=reinterpret_cast<decltype(native.MarkForAddNew)>(base+0x1fd0c0);
            check(exercise(native)==expected,"recordset flags, lengths, indexes and cache release match native MFC");
            ::FreeLibrary(module);
        }
    }
}
