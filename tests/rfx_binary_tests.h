#pragma once
#include <limits>
extern "C" BOOL MS_ABI impl__AfxCompareValueByRef__YAHPEAX0H_Z(void*,void*,int);
extern "C" void MS_ABI impl__AfxCopyValueByRef__YAXPEAX0PEA_JH_Z(void*,void*,SQLLEN*,int);
extern "C" void MS_ABI impl__AfxStoreField__YAXAEAVCRecordset__IPEAX_Z(CRecordset*,UINT,void*);
extern "C" void MS_ABI impl__AfxLoadField__YAXAEAVCRecordset__IPEAXPEA_J_Z(CRecordset*,UINT,void*,SQLLEN*);
extern "C" void MS_ABI impl__Default_CFieldExchange__QEAAXPEB_WPEAXPEA_JH_K3_Z(CFieldExchange*,const wchar_t*,void*,SQLLEN*,int,SQLULEN,SQLULEN);
extern "C" void MS_ABI impl__RFX_Binary__YAXPEAVCFieldExchange__PEB_WAEAVCByteArray___J_Z(CFieldExchange*,const wchar_t*,CByteArray*,SQLLEN);
namespace binary_rfx_test {
struct Binding {bool param=false;SQLHSTMT statement=nullptr;SQLUSMALLINT number=0;SQLSMALLINT direction=0,cType=0,sqlType=0,scale=0;SQLULEN precision=0;SQLLEN capacity=0;void* data=nullptr;SQLLEN* length=nullptr;};
static Binding binding;static int binds,checks,throws;static SQLRETURN sqlResult;static bool accept;
static void MS_ABI throwDB(CRecordset*,RETCODE code,HSTMT statement) {++throws;binding.statement=statement;throw int(code)+700;}
static BOOL MS_ABI checkDB(const CRecordset*,RETCODE) {++checks;return accept;}
struct API {
    decltype(&impl__RFX_Binary__YAXPEAVCFieldExchange__PEB_WAEAVCByteArray___J_Z) binary;
    decltype(&impl__Default_CFieldExchange__QEAAXPEB_WPEAXPEA_JH_K3_Z) defaults;
    decltype(&impl__AfxCompareValueByRef__YAHPEAX0H_Z) compare;
    decltype(&impl__AfxCopyValueByRef__YAXPEAX0PEA_JH_Z) copy;
    decltype(&impl__AfxStoreField__YAXAEAVCRecordset__IPEAX_Z) store;
    decltype(&impl__AfxLoadField__YAXAEAVCRecordset__IPEAXPEA_J_Z) load;
};
static void resetSQL(SQLRETURN code=0,bool accepted=true) {binding={};binds=checks=throws=0;sqlResult=code;accept=accepted;}
struct CacheManager : IAtlStringMgr {
    IAtlStringMgr* clone=nullptr;
    CStringData* Allocate(int n,int size) override {
        auto* p=static_cast<CStringData*>(std::calloc(1,sizeof(CStringData)+(n+1)*size));
        p->pStringMgr=this;p->nRefs=1;p->nAllocLength=n;return p;
    }
    void Free(CStringData* p) override {std::free(p);}
    CStringData* Reallocate(CStringData* p,int n,int size) override {
        p=static_cast<CStringData*>(std::realloc(p,sizeof(CStringData)+(n+1)*size));p->nAllocLength=n;return p;
    }
    CStringData* GetNilString() override {std::abort();}
    IAtlStringMgr* Clone() override {return clone?clone:this;}
};
void valueTypes(const API& api) {
    bool scalarCopies=true,scalarEquals=true;
    for(int type: {1,2,3,4,6,7,8,15,16,22}) {
        alignas(8) unsigned char source[24]{},target[24];memset(target,0xa5,sizeof(target));
        int count=type==2?1:(type==7 || type==8 || type==22)?8:(type==15 || type==16)?16:4;
        for(int i=0;i<count;++i)source[i]=BYTE(i+1);
        if(type==16) {*reinterpret_cast<double*>(source)=10;*reinterpret_cast<int*>(source+8)=0;}
        SQLLEN length=-77;api.copy(source,target,&length,type);
        scalarCopies &= length==(type==8?16:count) && !memcmp(source,target,count) && target[count]==0xa5;
        scalarEquals &= !!api.compare(source,target,type);
    }
    check(scalarCopies && scalarEquals,"cache helpers dispatch every supported scalar, date and timestamp type with exact byte counts");
    alignas(8) unsigned char a[16]{},b[16]{};
    *reinterpret_cast<double*>(a)=10;*reinterpret_cast<double*>(b)=10.0+0.25/86400;
    check(api.compare(a,b,16),"OLE date cache equality accepts differences below half a second");
    *reinterpret_cast<double*>(b)=10.0+1.0/86400;
    check(!api.compare(a,b,16),"OLE date cache equality rejects differences above half a second");
    *reinterpret_cast<int*>(a+8)=2;*reinterpret_cast<int*>(b+8)=2;
    check(api.compare(a,b,16),"OLE date cache equality treats two null-status values as equal");
    *reinterpret_cast<int*>(a+8)=1;*reinterpret_cast<int*>(b+8)=1;
    check(!api.compare(a,b,16),"OLE date cache equality rejects two invalid-status values");
    wchar_t wide[16]=L"wide",wideCopy[16]{};char narrow[16]="narrow",narrowCopy[16]{};SQLLEN length;
    api.copy(wide,wideCopy,&length,18);
    check(length==8 && api.compare(wide,wideCopy,18),"raw wide cache strings copy terminators and report byte lengths");
    api.copy(narrow,narrowCopy,&length,19);
    check(length==6 && api.compare(narrow,narrowCopy,19),"raw ANSI cache strings use native copy and comparison semantics");
    bool strings=true;
    for(int mode=0;mode<5;++mode) {
        CacheManager sourceManager,targetManager,cloneManager;
        IAtlStringMgr* targetOwner=(mode==2 || mode==3)?static_cast<IAtlStringMgr*>(&targetManager):&sourceManager;
        if(mode==4)sourceManager.clone=&cloneManager;
        auto* source=sourceManager.Allocate(5,1);source->nDataLength=5;memcpy(source+1,"value",6);
        auto* target=targetOwner->Allocate(mode==1?8:0,1);
        if(mode==0)source->nRefs=-1;
        if(mode==1)target->nRefs=-1;
        if(mode==3)target->nRefs=2;
        auto* kept=mode==3?target:nullptr;
        char* sourceText=reinterpret_cast<char*>(source+1);char* targetText=reinterpret_cast<char*>(target+1);
        char* originalTarget=targetText;
        api.copy(&sourceText,&targetText,&length,21);
        auto* result=reinterpret_cast<CStringData*>(targetText)-1;
        strings &= length==5 && !strcmp(targetText,"value") && api.compare(&sourceText,&targetText,21);
        if(mode==0)strings &= source->nRefs==-1 && result!=source;
        if(mode==1)strings &= targetText==originalTarget && result->nRefs==-1;
        if(mode==2 || mode==3)strings &= result->pStringMgr==&targetManager;
        if(mode==4)strings &= result->pStringMgr==&cloneManager && source->nRefs==1;
        if(mode==0)source->nRefs=1;
        if(mode==1)result->nRefs=1;
        if(result->Release())result->pStringMgr->Free(result);
        if(source->Release())source->pStringMgr->Free(source);
        if(kept && kept->Release())kept->pStringMgr->Free(kept);
    }
    check(strings,"ANSI string cache copies preserve locks, manager ownership, shared buffers and clone-manager semantics");
}
std::vector<UINT64> exercise(const API& api) {
    recordset_storage_test::Fixture fixture(1,1);auto* rs=fixture.recordset;
    rs->AllocStatusArrays();rs->m_hstmt=reinterpret_cast<HSTMT>(0xcafe);
    void* slots[27]{};slots[21]=reinterpret_cast<void*>(&checkDB);slots[24]=reinterpret_cast<void*>(&throwDB);
    *reinterpret_cast<void***>(rs)=slots;
    CODBCFieldInfo meta{};meta.m_nSQLType=SQL_VARBINARY;meta.m_nPrecision=6;meta.m_nScale=3;rs->m_rgODBCFieldInfos=&meta;
    CByteArray value;value.SetSize(4);for(int i=0;i<4;++i)value[i]=BYTE(i+1);
    auto run=[&](UINT operation,UINT type=0) {CFieldExchange fx(operation,rs);fx.SetFieldType(type);fx.m_hstmt=reinterpret_cast<HSTMT>(0xbeef);
        api.binary(&fx,L"payload",&value,4);return fx;};
    resetSQL();run(CFieldExchange::BindFieldToColumn);
    check(binds==1 && checks==1 && !binding.param && binding.statement==rs->m_hstmt && binding.number==1 && binding.cType==SQL_C_BINARY &&
          binding.capacity==4 && binding.data==value.m_pData && binding.length==&rs->m_rgFieldInfos[0].m_nLength && rs->GetBoundFieldIndex(&value)==1,
          "RFX_Binary binds the metadata-limited native buffer and records the array's one-based field index");
    std::vector<UINT64> result;
    auto save=[&] {result.push_back(rs->m_pbFieldFlags[0]);result.push_back(UINT64(value.m_nSize));result.push_back(rs->m_rgFieldInfos[0].m_nLength);
        for(INT_PTR i=0;i<value.m_nSize;++i)result.push_back(value[i]);};
    save();
    rs->m_rgFieldInfos[0].m_nLength=2;run(CFieldExchange::Fixup);
    check(value.GetSize()==2 && value[0]==1 && value[1]==2,"binary Fixup updates native array size without copying or allocating");save();
    rs->m_rgFieldInfos[0].m_nLength=0x100000001LL;run(CFieldExchange::Fixup);
    check(value.GetSize()==1,"binary Fixup reproduces native signed-DWORD length truncation");
    rs->m_rgFieldInfos[0].m_nLength=SQL_NULL_DATA;run(CFieldExchange::Fixup);
    check(value.GetSize()==1 && value[0]==0xff && rs->IsFieldStatusNull(0)==2,"binary null Fixup installs the native one-byte pseudo-null");save();
    rs->ClearDirtyFieldStatus(0);run(CFieldExchange::MarkForAddNew);
    check(!rs->IsFieldStatusDirty(0) && rs->IsFieldStatusNull(0)==2,"MarkForAddNew leaves a pseudo-null binary field unchanged");
    value[0]=5;run(CFieldExchange::MarkForAddNew);
    check(rs->IsFieldStatusDirty(0)==1 && !rs->IsFieldStatusNull(0),"MarkForAddNew marks real binary values dirty and clears null status");save();
    CFieldExchange nulls(CFieldExchange::SetFieldNull,rs);nulls.SetFieldType(0);nulls.m_bField=TRUE;
    api.binary(&nulls,L"payload",&value,4);
    check(value[0]==0xff && rs->m_rgFieldInfos[0].m_nLength==-1 && rs->IsFieldStatusNull(0)==2,"SetFieldNull sets the native binary sentinel and SQL_NULL_DATA");
    nulls.m_nFields=0;nulls.m_bField=FALSE;api.binary(&nulls,L"payload",&value,4);
    check(!rs->IsFieldStatusNull(0) && rs->m_rgFieldInfos[0].m_nLength==1,"binary null clearing preserves the value and restores its byte length");
    nulls.m_nFields=0;nulls.m_bField=TRUE;int other;nulls.m_pvField=&other;value[0]=7;api.binary(&nulls,L"payload",&value,4);
    check(value[0]==7 && !rs->IsFieldStatusNull(0),"binary null operation ignores a different selected field");
    nulls.m_nFields=0;nulls.m_pvField=&value;api.binary(&nulls,L"payload",&value,4);
    check(value[0]==0xff && rs->IsFieldStatusNull(0)==2,"binary null operation targets the selected native array object");
    run(CFieldExchange::AllocCache);auto* cache=static_cast<CByteArray*>(rs->m_rgFieldInfos[0].m_pvDataCache);
    check(cache && cache->GetSize()==0 && cache->m_pData==nullptr && rs->m_rgFieldInfos[0].m_nDataType==9,"binary cache allocation creates a native 40-byte CByteArray and type nine");
    rs->ClearFieldStatus();value.SetSize(2);value[0]=9;value[1]=10;run(CFieldExchange::StoreField);
    check(cache->GetSize()==2 && (*cache)[0]==9 && (*cache)[1]==10,"StoreField snapshots binary values in the native array cache");
    value[1]=22;auto updated=run(CFieldExchange::MarkForUpdate);
    check(rs->IsFieldStatusDirty(0)==1 && updated.m_bField,"binary update compares objects against the cache and reports a changed selected field");save();
    rs->ClearFieldStatus();run(CFieldExchange::LoadField);
    check(value.GetSize()==2 && value[1]==10 && rs->m_rgFieldInfos[0].m_nLength==2,"LoadField restores cached binary bytes and their SQL length");
    updated=run(CFieldExchange::MarkForUpdate);
    check(!rs->IsFieldStatusDirty(0) && !updated.m_bField,"unchanged cached binary value remains clean");
    rs->SetNullFieldStatus(0);run(CFieldExchange::StoreField);rs->ClearFieldStatus();value[0]=4;run(CFieldExchange::LoadField);
    check(rs->IsFieldStatusNull(0)==2 && rs->m_rgFieldInfos[0].m_nLength==-1 && value[0]==4,"loading a cached null restores status and leaves value bytes untouched");
    rs->ClearFieldStatus();updated=run(CFieldExchange::MarkForUpdate);
    check(rs->IsFieldStatusDirty(0)==1 && updated.m_bField,"cached-null to real binary transition becomes dirty");
    rs->FreeDataCache();
    resetSQL();auto input=run(CFieldExchange::BindParam,4);
    check(binding.param && binding.direction==4 && binding.statement==reinterpret_cast<HSTMT>(0xbeef) && binding.capacity==0 &&
          binding.data==value.m_pData && rs->GetBoundParamIndex(value.m_pData)==1 && rs->m_plParamLength[0]==2,
          "binary parameter binding uses exchange statement, native direction and raw-buffer index");
    resetSQL();rs->SetNullParamStatus(0);run(CFieldExchange::RebindParam,1);
    check(!binds && rs->m_plParamLength[0]==-1,"binary parameter rebinding honors native null flags without an ODBC call");
    CString sql;CFieldExchange names(CFieldExchange::Name,rs);names.SetFieldType(0);names.m_pstr=&sql;names.m_lpszSeparator=L", ";
    rs->SetDirtyFieldStatus(0);api.binary(&names,L"payload",&value,4);
    check(sql==L"payload, ","dirty binary Name operation appends its field name and separator");
    sql=L"";names.m_nOperation=CFieldExchange::NameValue;names.m_nFields=0;names.m_hstmt=reinterpret_cast<HSTMT>(0xface);resetSQL();
    api.binary(&names,L"payload",&value,4);
    check(sql==L"payload=?, " && names.m_nParamFields==1 && binding.param && binding.statement==names.m_hstmt && binding.direction==SQL_PARAM_INPUT &&
          binding.cType==SQL_C_BINARY && binding.sqlType==SQL_VARBINARY && binding.precision==6 && binding.scale==3,
          "binary NameValue builds SQL and binds a marker using native column metadata");
    sql=L"";names.m_nFields=0;rs->m_dwOptions=CRecordset::optimizeBulkAdd;resetSQL();api.binary(&names,L"payload",&value,4);
    check(sql==L"payload=" && !binds && names.m_nParamFields==1,"optimized bulk-add SQL omits value markers and parameter binding");rs->m_dwOptions=0;
    rs->ClearFieldStatus();rs->m_rgFieldInfos[0].m_nLength=33;run(CFieldExchange::BindFieldForUpdate);
    check(rs->m_rgFieldInfos[0].m_nLength==SQL_IGNORE,"binary update binding ignores a clean column");
    rs->SetDirtyFieldStatus(0);rs->SetNullFieldStatus(0);run(CFieldExchange::BindFieldForUpdate);
    check(rs->m_rgFieldInfos[0].m_nLength==-1,"dirty null binary update binding uses SQL_NULL_DATA");
    run(CFieldExchange::UnbindFieldForUpdate);check(rs->m_rgFieldInfos[0].m_nLength==-1,"unbinding a dirty binary column preserves its update length");
    rs->ClearFieldStatus();run(CFieldExchange::UnbindFieldForUpdate);check(rs->m_rgFieldInfos[0].m_nLength==2,"unbinding a clean binary column restores its byte count");
    resetSQL(SQL_SUCCESS_WITH_INFO);bool failed=false;try {run(CFieldExchange::BindParam,1);}catch(int code) {failed=code==701;}
    check(failed && throws==1 && binding.statement==reinterpret_cast<HSTMT>(0xbeef),"binary parameter binding rejects nonzero ODBC codes through virtual exception slot 24");
    resetSQL(SQL_ERROR,false);failed=false;try {run(CFieldExchange::BindFieldToColumn);}catch(int code) {failed=code==699;}
    check(failed && checks==1 && throws==1 && binding.statement==nullptr,"binary column binding delegates failure to virtual Check and ThrowDBException");
    value.SetSize(4);rs->m_rgFieldInfos[0].m_nLength=5;failed=false;try {run(CFieldExchange::Fixup);}catch(...) {failed=true;}
    check(failed && value.GetSize()==4,"binary Fixup rejects lengths beyond native capacity before changing size");
    meta.m_nPrecision=0;resetSQL();failed=false;try {run(CFieldExchange::BindFieldToColumn);}catch(...) {failed=true;}
    check(failed && !binds,"binary binding rejects zero-size metadata before calling ODBC");
    rs->m_rgFieldInfos[0].m_nLength=88;value.SetSize(1);value[0]=11;
    auto skipped=run(CFieldExchange::Fixup,1);
    check(skipped.m_nParams==1 && value[0]==11 && rs->m_rgFieldInfos[0].m_nLength==88,"input parameters are counted but skipped by binary Fixup");
    rs->m_plParamLength[0]=1;auto output=run(CFieldExchange::Fixup,2);
    check(output.m_nParams==1 && value.GetSize()==1,"output-parameter binary Fixup selects the native parameter length");
    // Inline scalar cache must never be interpreted as a heap allocation.
    auto& info=rs->m_rgFieldInfos[0];info.m_nDataType=4;info.m_pvDataCache=nullptr;rs->ClearFieldStatus();int scalar=314;
    api.store(rs,1,&scalar);scalar=9;SQLLEN length=-77;api.load(rs,1,&scalar,&length);
    check(scalar==314 && length==4,"cache store/load preserves inline scalar values and their native copy size");
    rs->FreeDataCache();check(!info.m_pvDataCache,"scalar cache cleanup clears inline values without freeing numeric addresses");
    std::array<unsigned char,16> a{},b{};int marker=7;memcpy(a.data(),&marker,4);memcpy(b.data(),&marker,4);
    check(api.compare(a.data(),b.data(),4)==TRUE && api.compare(a.data(),a.data(),4)==TRUE,"typed cache comparison returns equality rather than memcmp ordering");
    float positive=0,negative=-0.0f,nan=std::numeric_limits<float>::quiet_NaN();
    check(api.compare(&positive,&negative,6) && !api.compare(&nan,&nan,6),"floating cache equality matches native signed-zero and NaN behavior");
    length=70;api.copy(a.data(),b.data(),&length,0);check(length==70,"unsupported copy types preserve the length output");
    uint64_t time=123456;length=0;api.copy(&time,b.data(),&length,8);
    check(length==16 && *reinterpret_cast<uint64_t*>(b.data())==time,"native CTime cache copy writes eight bytes and reports its sixteen-byte length quirk");
    CString source(L"text"),destination;length=0;api.copy(&source,&destination,&length,20);
    check(destination==L"text" && length==8 && api.compare(&source,&destination,20),"wide string value caching copies and compares native string objects");
    // Native code can release default strings using manager reference counting.
    CString empty;CString copied(L"erase");api.copy(&empty,&copied,&length,20);
    check(copied.IsEmpty() && length==0 && GetNilStringData()->nRefs>=2,"empty cached strings retain native ATL manager references");
    valueTypes(api);
    return result;
}
}
extern "C" SQLRETURN SQL_API SQLBindCol(SQLHSTMT stmt,SQLUSMALLINT number,SQLSMALLINT type,SQLPOINTER data,SQLLEN size,SQLLEN* length) {
    using namespace binary_rfx_test;++binds;binding={false,stmt,number,0,type,0,0,0,size,data,length};return sqlResult;
}
extern "C" SQLRETURN SQL_API SQLBindParameter(SQLHSTMT stmt,SQLUSMALLINT number,SQLSMALLINT direction,SQLSMALLINT type,SQLSMALLINT sqlType,
    SQLULEN precision,SQLSMALLINT scale,SQLPOINTER data,SQLLEN size,SQLLEN* length) {
    using namespace binary_rfx_test;++binds;binding={true,stmt,number,direction,type,sqlType,scale,precision,size,data,length};return sqlResult;
}
static void binaryFieldExchange() {
    using namespace binary_rfx_test;
    API production{impl__RFX_Binary__YAXPEAVCFieldExchange__PEB_WAEAVCByteArray___J_Z,impl__Default_CFieldExchange__QEAAXPEB_WPEAXPEA_JH_K3_Z,
        impl__AfxCompareValueByRef__YAHPEAX0H_Z,impl__AfxCopyValueByRef__YAXPEAX0PEA_JH_Z,impl__AfxStoreField__YAXAEAVCRecordset__IPEAX_Z,impl__AfxLoadField__YAXAEAVCRecordset__IPEAXPEA_J_Z};
    auto expected=exercise(production);
    wchar_t path[1024];if(::GetEnvironmentVariableW(L"OPENMFC_NATIVE_BINARY_DLL",path,1024)) {
        HMODULE module=::LoadLibraryW(path);check(module!=nullptr,"load native MFC for binary RFX comparison");
        if(module) {
            auto* base=reinterpret_cast<char*>(module);API native=production;
            native.binary=reinterpret_cast<decltype(native.binary)>(base+0x200dc0);
            native.defaults=reinterpret_cast<decltype(native.defaults)>(base+0x1fe750);
            native.compare=reinterpret_cast<decltype(native.compare)>(base+0x2034b0);
            native.copy=reinterpret_cast<decltype(native.copy)>(base+0x2036c0);
            native.store=reinterpret_cast<decltype(native.store)>(base+0x203390);
            native.load=reinterpret_cast<decltype(native.load)>(base+0x203420);
            void** column=reinterpret_cast<void**>(base+0x3e9038);void** parameter=reinterpret_cast<void**>(base+0x3e9040);
            void* oldColumn=*column;void* oldParameter=*parameter;DWORD oldProtect;
            ::VirtualProtect(column,16,PAGE_READWRITE,&oldProtect);
            *column=reinterpret_cast<void*>(&SQLBindCol);*parameter=reinterpret_cast<void*>(&SQLBindParameter);
            check(exercise(native)==expected,"binary exchange, cache operations, SQL generation and bindings match native MFC");
            *column=oldColumn;*parameter=oldParameter;DWORD ignored;::VirtualProtect(column,16,oldProtect,&ignored);
            ::FreeLibrary(module);
        }
    }
}
