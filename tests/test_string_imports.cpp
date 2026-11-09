// The same client is compiled against OpenMFC and the installed SDK MFC headers.
#ifdef OPENMFC_USE_SDK_MFC
#define _AFX_NOFORCE_LIBS
#include <afx.h>
#include <afxdisp.h>
#else
#include "openmfc/afxdisp.h"
#endif
#include <cstdio>
#include <cstring>
#include <cwchar>
#include <cstdlib>
#include <type_traits>
#include <utility>

static_assert(sizeof(CStringA)==8&&sizeof(CStringW)==8&&sizeof(ATL::CStringData)==24,"native string representation");
static_assert(std::is_same_v<CStringA,ATL::CStringT<char,StrTraitMFC_DLL<char,ATL::ChTraitsCRT<char>>>>,"native ANSI type identity");
static_assert(std::is_same_v<CStringW,ATL::CStringT<wchar_t,StrTraitMFC_DLL<wchar_t,ATL::ChTraitsCRT<wchar_t>>>>,"native Unicode type identity");
static_assert(std::is_same_v<decltype(std::declval<ATL::CStringData&>().data()),void*>,"native untyped storage");
static_assert(std::is_same_v<decltype(std::declval<ATL::CStringData&>().Release()),void>,"native owning release");
static_assert(std::is_same_v<decltype(std::declval<CStringA&>()+=wchar_t()),CStringA&>,"native ANSI append return type");
static_assert(std::is_same_v<decltype(std::declval<CStringA&>()+=char()),CStringA&>,"native ANSI character append return type");
static_assert(std::is_same_v<decltype(std::declval<CStringW&>()+=char()),CStringW&>,"native Unicode append return type");
static_assert(std::is_same_v<decltype(std::declval<CStringW&>()+=static_cast<unsigned char>(0)),CStringW&>,"native unsigned-character append return type");
namespace {
int failures;
void check(bool value,const char* message){if(!value){++failures;std::printf("FAIL: %s\n",message);}}
template<class T>ATL::CStringData* header(const T& text){return reinterpret_cast<ATL::CStringData*>(const_cast<typename T::XCHAR*>(text.GetString()))-1;}
struct Manager:ATL::IAtlStringMgr {
    struct {ATL::CStringData data;wchar_t empty[2];} nil{{this,0,0,2},{}};
    int live=0;bool fail=false;Manager* cloned=this;
    ATL::CStringData* Allocate(int length,int width) noexcept override {
        if(fail||length<0)return nullptr;
        auto* data=static_cast<ATL::CStringData*>(std::malloc(sizeof(ATL::CStringData)+size_t(length+1)*width));
        if(!data)return nullptr;*data={this,0,length,1};++live;return data;
    }
    void Free(ATL::CStringData* data) noexcept override {--live;std::free(data);}
    ATL::CStringData* Reallocate(ATL::CStringData* data,int length,int width) noexcept override {
        if(fail||length<0)return nullptr;
        auto* out=static_cast<ATL::CStringData*>(std::realloc(data,sizeof(*data)+size_t(length+1)*width));
        if(out)out->nAllocLength=length;return out;
    }
    ATL::CStringData* GetNilString() noexcept override {nil.data.AddRef();return &nil.data;}
    ATL::IAtlStringMgr* Clone() noexcept override {return cloned;}
};
}
int main(){
    auto* manager=AfxGetStringManager();
    check(manager!=nullptr,"DLL exposes the actual string manager");
    {
        CStringW wide(L"wide");CStringA narrow("ansi");CStringW emptyW;CStringA emptyA;
        check(header(wide)->pStringMgr==manager&&header(narrow)->pStringMgr==manager,"imported constructors use the producer manager");
        check(header(emptyW)->pStringMgr==manager&&header(emptyA)->pStringMgr==manager,"default constructors share the producer singleton");
        wide.LockBuffer();CStringW wideCopy(wide);wideCopy.GetBuffer()[0]=L'W';wideCopy.ReleaseBuffer();
        narrow.LockBuffer();CStringA narrowCopy(narrow);narrowCopy.GetBuffer()[0]='A';narrowCopy.ReleaseBuffer();
        check(wideCopy.GetString()!=wide.GetString()&&!wcscmp(wide.GetString(),L"wide")&&!wcscmp(wideCopy.GetString(),L"Wide"),"wide locked copies own independent DLL storage");
        check(narrowCopy.GetString()!=narrow.GetString()&&!strcmp(narrow.GetString(),"ansi")&&!strcmp(narrowCopy.GetString(),"Ansi"),"ANSI locked copies own independent DLL storage");
        VARIANT input{};input.vt=VT_I4;input.lVal=-123;
        CStringW convertedW(input);CStringA convertedA(input);
        check(!wcscmp(convertedW.GetString(),L"-123")&&!strcmp(convertedA.GetString(),"-123"),"VARIANT constructors resolve through the native imports");
        convertedW=input;convertedA=input;convertedW+=input;convertedA+=input;
        check(!wcscmp(convertedW.GetString(),L"-123-123")&&!strcmp(convertedA.GetString(),"-123-123"),"VARIANT assignment and append cross the DLL boundary");
        HINSTANCE module=GetModuleHandleW(L"openmfc_string_types.dll");
        check(emptyW.LoadString(module,35)&&!wcscmp(emptyW.GetString(),L"string DLL resource"),"wide resource loading imports the native W member");
        check(emptyA.LoadString(module,35)&&!strcmp(emptyA.GetString(),"string DLL resource"),"ANSI resource loading imports the native W member");
    }
    Manager custom,other;
    {
        CStringW wide(L"owned",&custom);CStringA narrow("owned",&custom);
        check(header(wide)->pStringMgr==&custom&&header(narrow)->pStringMgr==&custom,"DLL constructors accept a client manager with the native interface");
        {
            CStringW sourceW(L"AB\0CDE",6,&custom);CStringA sourceA("AB\0CDE",6,&custom);
            auto midW=sourceW.Mid(1,4);auto midA=sourceA.Mid(1,4);
            auto rightW=sourceW.Right(4);auto rightA=sourceA.Right(4);
            check(midW.GetLength()==4&&midA.GetLength()==4&&midW.GetAt(1)==0&&midA.GetAt(1)==0,"imported Mid preserves counted embedded NUL data");
            check(rightW.GetLength()==4&&rightA.GetLength()==4&&rightW.GetAt(0)==0&&rightA.GetAt(0)==0,"imported Right preserves the counted suffix");
            auto fullW=sourceW.Left(99);auto fullA=sourceA.Left(99);
            check(fullW.GetString()==sourceW.GetString()&&fullA.GetString()==sourceA.GetString(),"full substrings preserve native shared storage");
            custom.cloned=&other;
            auto leftW=sourceW.Left(1);auto leftA=sourceA.Left(1);
            check(header(leftW)->pStringMgr==&other&&header(leftA)->pStringMgr==&other,"substring imports use the source manager's clone");
            custom.cloned=nullptr;
            check(sourceW.GetManager()==manager&&sourceA.GetManager()==manager,"derived GetManager falls back to the DLL manager when Clone returns null");
            auto fallbackW=sourceW.Right(1);auto fallbackA=sourceA.Right(1);
            check(header(fallbackW)->pStringMgr==manager&&header(fallbackA)->pStringMgr==manager,"substring manager fallback remains in the producer DLL");
            custom.cloned=&custom;
            int startW=0,startA=0;
            auto tokenW=sourceW.Tokenize(L"A",startW);auto tokenA=sourceA.Tokenize("A",startA);
            check(tokenW.GetLength()==1&&tokenA.GetLength()==1&&startW==3&&startA==3,"imported tokenization updates the caller's index");
            auto spanW=sourceW.SpanIncluding(L"AB");auto spanA=sourceA.SpanExcluding("B");
            check(spanW.GetLength()==2&&spanA.GetLength()==1,"imported span methods use the native character set rules");
            bool overflow=false;
            try{auto invalid=sourceW.Mid(1,INT32_MAX);}catch(COleException* error){overflow=error->m_sc==static_cast<HRESULT>(0x80070216u);error->Delete();}
            check(overflow,"Mid reports checked length overflow across the DLL boundary");
            CStringW charsW(&custom);CStringA charsA(&custom);
            charsW+=char(0xe9);charsW+=static_cast<unsigned char>(0xe9);charsA+=wchar_t(0x3a9);
            check(charsW.GetLength()==2&&charsW.GetAt(0)==wchar_t(0xffe9)&&charsW.GetAt(1)==wchar_t(0xe9)&&static_cast<unsigned char>(charsA.GetAt(0))==0xa9,"derived character appends widen or truncate without encoding conversion");
        }
        {
            ATL::CSimpleStringT<char,false> plain("foreign",&custom);
            ATL::CSimpleStringT<char,true> managed(plain);managed=plain;
            check(!strcmp(managed.GetString(),"foreign"),"boolean-specialized simple strings cross the native constructor and assignment imports");
        }
        {
            using Wide=ATL::CSimpleStringT<wchar_t,true>;
            Wide text(L"abcdef",&custom);
            HMODULE dll=GetModuleHandleW(L"openmfc_string_types.dll");
            auto signedByte=reinterpret_cast<Wide*(*)(Wide*,char)>(GetProcAddress(dll,"??Y?$CSimpleStringT@_W$00@ATL@@QEAAAEAV01@D@Z"));
            auto unsignedByte=reinterpret_cast<Wide*(*)(Wide*,unsigned char)>(GetProcAddress(dll,"??Y?$CSimpleStringT@_W$00@ATL@@QEAAAEAV01@E@Z"));
            auto compact=reinterpret_cast<void(*)(Wide*)>(GetProcAddress(dll,"?FreeExtra@?$CSimpleStringT@_W$00@ATL@@QEAAXXZ"));
            auto reallocate=reinterpret_cast<void(*)(Wide*,int)>(GetProcAddress(dll,"?Reallocate@?$CSimpleStringT@_W$00@ATL@@AEAAXH@Z"));
            check(signedByte&&unsignedByte&&compact&&reallocate,"native wide storage aliases are exported by the DLL");
            if(signedByte&&unsignedByte&&compact&&reallocate){
                check(signedByte(&text,char(0xe9))==&text&&text.GetAt(6)==wchar_t(0xffe9),"base += char sign-extends across the DLL boundary");
                check(unsignedByte(&text,0xe9)==&text&&text.GetAt(7)==wchar_t(0xe9),"base += unsigned char zero-extends across the DLL boundary");
                text.Preallocate(32);compact(&text);
                check(header(text)->nAllocLength==text.GetLength(),"exported FreeExtra compacts client-managed wide storage");
                bool rejected=false;
                try{reallocate(&text,text.GetLength());}catch(CMemoryException* error){rejected=true;error->Delete();}
                check(rejected,"native Reallocate rejects non-growing requests with a typed memory exception");
            }
        }
        auto* data=custom.Allocate(3,1);data->Lock();data->Lock();data->Unlock();data->Unlock();
        check(data->nRefs==1&&data->data()==data+1,"native data locking and untyped storage access");
        data->AddRef();data->Release();check(data->nRefs==1,"data release retains shared storage");data->Release();
        custom.fail=true;bool caught=false;
        try{CStringW failed(L"allocation failure",&custom);}catch(CMemoryException* error){caught=true;error->Delete();}
        check(caught,"actual memory-exception pointers cross the DLL boundary");
        try{CStringW failed(L"text",static_cast<ATL::IAtlStringMgr*>(nullptr));check(false,"invalid manager throws");}
        catch(COleException* error){check(error->m_sc==E_FAIL,"actual OLE exceptions preserve their HRESULT");error->Delete();}
    }
    check(custom.live==0&&custom.nil.data.nRefs==2,"client callbacks release all imported string storage and nil references");
    check(other.live==0&&other.nil.data.nRefs==2,"cloned client manager releases all imported substring results");
    std::printf("String DLL imports: %d failures\n",failures);return failures?1:0;
}
