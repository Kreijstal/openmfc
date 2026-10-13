#pragma once
#include <wincrypt.h>
extern "C" CString* MS_ABI impl__GetConnect_CDatabase__QEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__XZ(const CDatabase*,CString*);
extern "C" CString* MS_ABI impl__GetDatabaseName_CDatabase__QEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__XZ(const CDatabase*,CString*);
namespace database_strings_test {
using Getter=CString* (MS_ABI*)(const CDatabase*,CString*);
struct API {Getter connect,name;};
struct InfoCall {SQLHDBC connection;SQLUSMALLINT kind;SQLSMALLINT capacity;bool output;};
static std::vector<InfoCall> infoCalls;
static std::vector<RETCODE> checkCodes;
static std::wstring infoText;
static SQLSMALLINT reported;
static RETCODE firstResult,secondResult;
static bool firstAccepted,secondAccepted;
static BOOL MS_ABI checkInfo(const CDatabase*,RETCODE code) {
    checkCodes.push_back(code);return checkCodes.size()==1?firstAccepted:secondAccepted;
}
static CStringData* data(const CString& text) {return reinterpret_cast<CStringData*>(*reinterpret_cast<wchar_t* const*>(&text))-1;}
static void attach(CString& text,CStringData* buffer) {*reinterpret_cast<wchar_t**>(&text)=reinterpret_cast<wchar_t*>(buffer+1);}
struct Manager : IAtlStringMgr {
    Manager* clone=this;int allocations=0,frees=0,reallocations=0,clones=0;
    alignas(8) BYTE nil[32]{};
    Manager() {auto* p=reinterpret_cast<CStringData*>(nil);p->pStringMgr=this;p->nRefs=2;}
    CStringData* Allocate(int length,int size) override {
        ++allocations;auto* p=static_cast<CStringData*>(std::calloc(1,sizeof(CStringData)+(length+1)*size));
        p->pStringMgr=this;p->nAllocLength=length;p->nRefs=1;return p;
    }
    void Free(CStringData* p) override {++frees;std::free(p);}
    CStringData* Reallocate(CStringData* p,int length,int size) override {
        ++reallocations;p=static_cast<CStringData*>(std::realloc(p,sizeof(CStringData)+(length+1)*size));p->nAllocLength=length;return p;
    }
    CStringData* GetNilString() override {auto* p=reinterpret_cast<CStringData*>(nil);p->AddRef();return p;}
    IAtlStringMgr* Clone() override {++clones;return clone;}
};
std::vector<UINT64> exercise(const API& api) {
    CDatabase database;std::vector<UINT64> result;
    auto invoke=[&](Getter getter) {
        alignas(CString) BYTE storage[24];memset(storage,0xa5,sizeof(storage));
        auto* returned=getter(&database,reinterpret_cast<CString*>(storage));
        check(returned==reinterpret_cast<CString*>(storage) && *reinterpret_cast<UINT64*>(storage+8)==0xa5a5a5a5a5a5a5a5ULL,
            "database string getter uses this-first MSVC return-buffer arguments and preserves the caller guard");
        auto* metadata=data(*returned);std::wstring text(returned->GetString(),returned->GetLength());
        result.push_back(metadata->nDataLength);result.push_back(metadata->nAllocLength);
        for(wchar_t ch:text)result.push_back(ch);result.push_back(0xffff);
        returned->~CString();return text;
    };
    database.m_strConnect=L"ODBC;DSN=fixture;";
    check(invoke(api.connect)==L"ODBC;DSN=fixture;","database connection getter returns the stored connection before considering its blob");
    database.m_strConnect.Empty();database.m_blobConnect.cbData=0;
    check(invoke(api.connect).empty(),"database connection getter returns an empty string when decryption fails");
    const wchar_t secret[]=L"ODBC;DSN=fixture;UID=test;PWD=not-a-secret;";
    DATA_BLOB plaintext{sizeof(secret),reinterpret_cast<BYTE*>(const_cast<wchar_t*>(secret))},encrypted{};
    bool encryptedOK=::CryptProtectData(&plaintext,nullptr,nullptr,nullptr,nullptr,0,&encrypted)!=FALSE;
    check(encryptedOK,"encrypt a nonsecret connection fixture using the real Windows DPAPI service");
    if(encryptedOK) {
        database.m_blobConnect.cbData=encrypted.cbData;database.m_blobConnect.pbData=encrypted.pbData;
        std::vector<BYTE> original(encrypted.pbData,encrypted.pbData+encrypted.cbData);
        check(invoke(api.connect)==secret && !memcmp(original.data(),encrypted.pbData,encrypted.cbData),
            "database connection getter decrypts its native blob without changing encrypted input storage");
        database.m_strConnect=L"stored wins";check(invoke(api.connect)==L"stored wins","stored connection text takes precedence over a valid encrypted blob");
        database.m_strConnect.Empty();::LocalFree(encrypted.pbData);database.m_blobConnect.pbData=nullptr;database.m_blobConnect.cbData=0;
    }
    for(bool locked: {false,true})for(bool different: {false,true}) {
        Manager source,other;source.clone=different?&other:&source;
        auto* owned=source.Allocate(7,2);owned->nDataLength=7;owned->nRefs=locked?-1:1;
        memcpy(owned+1,L"fixture",16);auto* old=data(database.m_strConnect);attach(database.m_strConnect,owned);
        int oldAlloc=source.allocations;int oldRefs=owned->nRefs;
        alignas(CString) BYTE storage[16];auto* output=api.connect(&database,reinterpret_cast<CString*>(storage));auto* clone=data(*output);
        check(output->GetLength()==7 && !wcscmp(output->GetString(),L"fixture") && source.clones==1 &&
            clone->pStringMgr==(different?&other:&source) && (locked || different ? clone!=owned && clone->nRefs==1 : clone==owned && owned->nRefs==2),
            "database connection clone honors locked buffers, custom managers, Clone and native reference ownership");
        result.push_back(clone==owned);result.push_back(clone->nRefs);result.push_back(source.clones);result.push_back(other.allocations);
        output->~CString();check(owned->nRefs==oldRefs && source.allocations==oldAlloc+(locked && !different?1:0) &&
            source.frees==(locked && !different?1:0) && other.frees==(different?1:0),
            "database connection result destruction preserves the source string ownership");
        attach(database.m_strConnect,old);owned->nRefs=1;source.Free(owned);
    }
    struct Case {SQLSMALLINT bytes;const wchar_t* text;RETCODE first,second;bool acceptFirst,acceptSecond;const wchar_t* expected;};
    Case cases[]={{10,L"alpha",0,0,true,true,L"alpha"},{12,L"\x03b4\x03bf\x03ba\x03b9\x03bc\x03ae",1,1,true,true,L"\x03b4\x03bf\x03ba\x03b9\x03bc\x03ae"},
        {4,L"longer",0,1,true,true,L"lo"},{0,L"",0,0,true,true,L""},{5,L"odd",0,0,true,true,L"od"},
        {10,L"alpha",-1,0,false,true,L""},{10,L"partial",0,-1,true,false,L""},{10,L"alpha",-1,-1,true,true,L"alpha"},
        {10,L"",0,-1,true,false,L""},{-1,L"",0,0,true,true,nullptr}};
    void** originalTable=*reinterpret_cast<void***>(&database);void* table[12];memcpy(table,originalTable,sizeof(table));
    table[10]=reinterpret_cast<void*>(&checkInfo);*reinterpret_cast<void***>(&database)=table;
    database.m_hdbc=reinterpret_cast<HDBC>(0xface);
    for(const auto& test:cases) {
        infoCalls.clear();checkCodes.clear();reported=test.bytes;infoText=test.text;firstResult=test.first;secondResult=test.second;
        firstAccepted=test.acceptFirst;secondAccepted=test.acceptSecond;bool invalid=false;
        try {auto text=invoke(api.name);check(test.expected && text==test.expected,"database name getter follows the ODBC result and virtual validation callbacks");}
        catch(COleException* error) {invalid=error->m_sc==E_INVALIDARG;error->Delete();}
        if(!test.expected)check(invalid,"database name getter rejects a negative driver length with native E_INVALIDARG");
        bool twice=test.acceptFirst && test.bytes>=0;
        check(infoCalls.size()==(twice?2:1) && checkCodes.size()==infoCalls.size() && infoCalls[0].connection==database.m_hdbc &&
            infoCalls[0].kind==SQL_DATABASE_NAME && !infoCalls[0].output && !infoCalls[0].capacity &&
            (!twice || (infoCalls[1].output && infoCalls[1].capacity==test.bytes+2)),
            "database name queries ODBC for byte length, allocates UTF-16 characters and passes exact terminator capacity");
        result.push_back(invalid);result.push_back(infoCalls.size());for(RETCODE rc:checkCodes)result.push_back(rc);
    }
    database.m_hdbc=nullptr;*reinterpret_cast<void***>(&database)=originalTable;return result;
}
}
extern "C" SQLRETURN SQL_API SQLGetInfoW(SQLHDBC connection,SQLUSMALLINT kind,SQLPOINTER output,SQLSMALLINT capacity,SQLSMALLINT* length) {
    using namespace database_strings_test;
    infoCalls.push_back({connection,kind,capacity,output!=nullptr});*length=reported;
    if(!output)return firstResult;
    if(capacity>=2) {
        size_t count=std::min(infoText.size(),size_t(capacity/2-1));
        memcpy(output,infoText.data(),count*2);static_cast<wchar_t*>(output)[count]=0;
    }
    return secondResult;
}
static void databaseStrings() {
    using namespace database_strings_test;
    API api{&impl__GetConnect_CDatabase__QEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__XZ,
        &impl__GetDatabaseName_CDatabase__QEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__XZ};
    auto expected=exercise(api);wchar_t path[1024];
    if(::GetEnvironmentVariableW(L"OPENMFC_NATIVE_DATABASE_DLL",path,1024)) {
        HMODULE module=::LoadLibraryW(path);check(module!=nullptr,"load native MFC for database string accessor comparison");
        if(module) {
            auto* base=reinterpret_cast<char*>(module);auto** slot=reinterpret_cast<void**>(base+0x3e9078);auto* saved=*slot;DWORD protection;
            ::VirtualProtect(slot,8,PAGE_READWRITE,&protection);*slot=reinterpret_cast<void*>(&SQLGetInfoW);
            API native{reinterpret_cast<Getter>(base+0x1f73c0),reinterpret_cast<Getter>(base+0x1f7260)};
            check(exercise(native)==expected,"database connection and name accessors match native text, buffer metadata, ownership and ODBC callbacks");
            *slot=saved;DWORD ignored;::VirtualProtect(slot,8,protection,&ignored);::FreeLibrary(module);
        }
    }
}
