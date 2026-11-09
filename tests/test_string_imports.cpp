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
static_assert(std::is_same_v<decltype(std::declval<CStringW&>().MakeUpper()),CStringW&>,"native Unicode case return type");
static_assert(std::is_same_v<decltype(std::declval<CStringA&>().MakeReverse()),CStringA&>,"native ANSI case return type");
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
template<class T>void printfClient(T& text,bool append,const typename T::XCHAR* format,...){
    va_list args;va_start(args,format);
    if(append)text.AppendFormatV(format,args);else text.FormatV(format,args);
    va_end(args);
}
template<class T>void messageClient(T& text,const typename T::XCHAR* format,...){
    va_list args;va_start(args,format);text.FormatMessageV(format,&args);va_end(args);
}
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
            CStringW textW(L"Ab\0aB",5,&custom);CStringA textA("Ab\0aB",5,&custom);
            check(textW.Compare(L"Ab")==0&&textA.Compare("Ab")==0&&textW.CompareNoCase(L"aB")==0&&textA.CompareNoCase("aB")==0,"comparison imports use the terminated prefix");
            check(textW.Collate(L"Ab")==0&&textA.Collate("Ab")==0&&textW.CollateNoCase(L"aB")==0&&textA.CollateNoCase("aB")==0,"collation imports use the CRT rules");
            check(textW.Find(L"",5)==5&&textA.Find("",5)==5&&textW.Find(L"",-1)==-1&&textA.Find("",-1)==-1,"substring searches allow the end position and reject negative offsets");
            check(textW.Find(L'a',3)==3&&textA.Find('a',3)==3&&textW.Find(L'a',0)==-1&&textA.Find('a',0)==-1,"search offsets can begin after an embedded NUL");
            check(textW.ReverseFind(wchar_t(0))==2&&textA.ReverseFind(char(0))==2&&textW.FindOneOf(L"b")==1&&textA.FindOneOf("b")==1,"reverse and set search imports preserve native indices");
            bool rejectedW=false,rejectedA=false;
            try{textW.Compare(nullptr);}catch(COleException* error){rejectedW=error->m_sc==E_FAIL;error->Delete();}
            try{textA.CompareNoCase(nullptr);}catch(COleException* error){rejectedA=error->m_sc==E_FAIL;error->Delete();}
            check(rejectedW&&rejectedA,"null comparisons throw native typed E_FAIL exceptions across the DLL boundary");
            CStringW sharedW(textW);CStringA sharedA(textA);
            check(&textW.MakeUpper()==&textW && &textA.MakeUpper()==&textA,"case imports return the original object");
            check(textW.GetString()!=sharedW.GetString()&&textA.GetString()!=sharedA.GetString()&&sharedW.GetAt(1)==L'b'&&sharedA.GetAt(1)=='b',"case conversion detaches shared storage");
            check(textW.GetAt(1)==L'B'&&textA.GetAt(1)=='B'&&textW.GetAt(3)==L'a'&&textA.GetAt(3)=='a',"case conversion preserves data following an embedded NUL");
            textW.LockBuffer();textW.LockBuffer();textA.LockBuffer();textA.LockBuffer();
            textW.MakeLower().MakeReverse();textA.MakeLower().MakeReverse();
            check(textW.GetLength()==5&&textA.GetLength()==5&&textW.GetAt(0)==L'b'&&textA.GetAt(0)=='b'&&header(textW)->nRefs==-2&&header(textA)->nRefs==-2,"case conversion and reversal preserve counted length and nested locks");
        }
        {
            CStringW formatW(L"[%d]",&custom);CStringA formatA("[%d]",&custom);
            formatW.Format(formatW.GetString(),7);formatA.Format(formatA.GetString(),7);
            check(!wcscmp(formatW.GetString(),L"[7]")&&!strcmp(formatA.GetString(),"[7]"),"Format preserves a format string that aliases its destination");
            formatW.Format(L"%s-%d",L"wide",42);formatA.Format("%s-%d","ansi",42);
            formatW.AppendFormat(L"|%0*d",40000,1);formatA.AppendFormat("|%0*d",40000,1);
            check(formatW.GetLength()==40008&&formatA.GetLength()==40008&&formatW.GetAt(40007)==L'1'&&formatA.GetAt(40007)=='1',"format imports support output beyond the old fixed limit");
            printfClient(formatW,false,L"%c/%s",0,L"tail");printfClient(formatA,false,"%c/%s",0,"tail");
            check(formatW.GetLength()==6&&formatA.GetLength()==6&&formatW.GetAt(0)==0&&formatA.GetAt(0)==0&&formatW.GetAt(5)==L'l'&&formatA.GetAt(5)=='l',"FormatV preserves the counted length of embedded NUL output");
            printfClient(formatW,true,L"=%d",9);printfClient(formatA,true,"=%d",9);
            check(formatW.GetLength()==8&&formatA.GetLength()==8&&formatW.GetAt(7)==L'9'&&formatA.GetAt(7)=='9',"AppendFormatV appends at the counted end");
            formatW.Format(UINT(36),42);formatA.Format(UINT(36),42);
            formatW.AppendFormat(UINT(36),9);formatA.AppendFormat(UINT(36),9);
            check(!wcscmp(formatW.GetString(),L"resource:42resource:9")&&!strcmp(formatA.GetString(),"resource:42resource:9"),"resource formatting imports use production resource loading");
            SetLastError(0x4321);messageClient(formatW,L"text:%1!s!",L"wide");
            check(GetLastError()==0x4321&&!wcscmp(formatW.GetString(),L"text:wide"),"wide FormatMessageV preserves the caller's last error");
            messageClient(formatA,"text:%1!s!","ansi");
            formatW.FormatMessageW(L"number:%1!d!",7);formatA.FormatMessageW("number:%1!d!",7);
            check(!wcscmp(formatW.GetString(),L"number:7")&&!strcmp(formatA.GetString(),"number:7"),"message-format pointer imports use the native Windows argument list");
            formatW.FormatMessageW(UINT(37),42);formatA.FormatMessageW(UINT(37),42);
            check(!wcscmp(formatW.GetString(),L"message:42")&&!strcmp(formatA.GetString(),"message:42"),"message-format resource imports load the native template");
            bool missing=false,invalid=false;
            try{formatW.Format(UINT(999));}catch(COleException* error){missing=error->m_sc==E_FAIL;error->Delete();}
            try{formatA.Format(static_cast<const char*>(nullptr));}catch(COleException* error){invalid=error->m_sc==E_INVALIDARG;error->Delete();}
            check(missing&&invalid,"formatting imports preserve typed resource and invalid-argument failures");
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
