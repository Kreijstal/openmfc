// Exercise the production arithmetic and assembly with real x64 MSVC calls.
#include "detail/Numeric128.h"
#include <windows.h>
#include <oleauto.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cstddef>
#include <csetjmp>
#include <cwchar>
extern "C" void openmfc_DHCS_StdCallThunkHelper();
namespace {
int failures;
void check(bool value,const char* text) {
    if(!value) {++failures;std::printf("FAIL: %s\n",text);}
}
struct Thunk {
    void* table;
    void* owner;
    void* function;
    std::intptr_t adjustment;
    void (*helper)();
};
static_assert(offsetof(Thunk,owner)==8 && offsetof(Thunk,function)==0x10 && offsetof(Thunk,helper)==0x20,
              "native DHTML thunk fields");
struct Context {int calls=0;bool throws=false;};
Context* expected;
constexpr std::uint64_t a=0x123456789abcdef0ULL,c=0xaaaabbbbccccddddULL,e=0x7777888899990000ULL;
constexpr std::uint64_t answer=0xfedcba9876543210ULL;
std::uint64_t mixed(void* object,std::uint64_t first,double second,std::uint64_t third,double fourth,std::uint64_t fifth) {
    check(object==expected,"the thunk supplies the handler object as this");
    check(first==a && second==1.25 && third==c && fourth==-6.5 && fifth==e,
          "integer, floating and stack arguments survive the tail call");
    if(object!=expected)return 0;
    ++expected->calls;
    if(expected->throws)throw 77;
    return answer;
}
double floating(void* object,double first,double second,double third,double fourth,double fifth) {
    check(object==expected && first==1.25 && second==2.5 && third==5 && fourth==10 && fifth==20,
          "floating arguments retain their native register and stack positions");
    ++expected->calls;
    return first+second+third+fourth+fifth;
}
void thunk() {
    Context context;expected=&context;
    Thunk value{nullptr,&context,reinterpret_cast<void*>(&mixed),0,&openmfc_DHCS_StdCallThunkHelper};
    value.table=&value.helper;
    using Mixed=std::uint64_t (*)(void*,std::uint64_t,double,std::uint64_t,double,std::uint64_t);
    auto invoke=reinterpret_cast<Mixed>(&openmfc_DHCS_StdCallThunkHelper);
    check(invoke(&value,a,1.25,c,-6.5,e)==answer,"the thunk forwards a 64-bit integer return");

    // This is the actual OLE Automation route used by the event sink, including
    // an implicit this parameter and both register and stack arguments.
    VARTYPE types[]={VT_UI8,VT_R8,VT_UI8,VT_R8,VT_UI8};
    VARIANTARG arguments[5]{};VARIANTARG* pointers[5];
    for(unsigned i=0;i<5;++i) {arguments[i].vt=types[i];pointers[i]=&arguments[i];}
    arguments[0].ullVal=a;arguments[1].dblVal=1.25;arguments[2].ullVal=c;
    arguments[3].dblVal=-6.5;arguments[4].ullVal=e;
    VARIANT result{};
    const HRESULT hr=::DispCallFunc(&value,0,CC_STDCALL,VT_UI8,5,types,pointers,&result);
    check(hr==S_OK && result.vt==VT_UI8 && result.ullVal==answer,
          "DispCallFunc reaches the production DHTML thunk and receives the handler result");
    ::VariantClear(&result);

    context.throws=true;int caught=0;
    try {invoke(&value,a,1.25,c,-6.5,e);}catch(int error) {caught=error;}
    check(caught==77,"native C++ exceptions unwind through the tail call");
    context.throws=false;value.function=reinterpret_cast<void*>(&floating);
    using Floating=double (*)(void*,double,double,double,double,double);
    check(reinterpret_cast<Floating>(&openmfc_DHCS_StdCallThunkHelper)(&value,1.25,2.5,5,10,20)==38.75,
          "the thunk preserves a floating-point return");
    check(context.calls==4,"each dispatch invokes its handler exactly once");
}
void crt() {
    // The DLL uses the actual CRT. Its removed MinGW shim returned zero from
    // setjmp and trapped in longjmp, so exercise the real nonlocal return.
    std::jmp_buf target;
    const int value=setjmp(target);
    if(value==0)std::longjmp(target,7);
    check(value==7,"the native CRT supplies working setjmp/longjmp");
    char bytes[]="ABCDE";std::memmove(bytes+1,bytes,4);
    check(!std::memcmp(bytes,"AABCD",5) && std::strchr(bytes,'C')==bytes+3,
          "native CRT memory and string routines preserve their standard behavior");
    const wchar_t* wide=L"abac";
    check(std::wcschr(wide,L'a')==wide && std::wcsrchr(wide,L'a')==wide+2,
          "wide CRT searches use the native const overloads");
}
int numeric() {
    char text[2048];
    while(std::fgets(text,sizeof(text),stdin)) {
        openmfc::detail::Numeric128 coefficient;
        for(auto* p=text;*p && *p!='\r' && *p!='\n';++p) {
            if(*p<'0' || *p>'9')return 2;
            coefficient.AppendDigit(static_cast<unsigned>(*p-'0'));
        }
        unsigned char storage[18];std::memset(storage,0xa5,sizeof(storage));
        coefficient.Store(storage+1);
        check(storage[0]==0xa5 && storage[17]==0xa5,"coefficient stores tolerate unaligned DB_NUMERIC fields and preserve guards");
        for(int i=16;i>0;--i)std::printf("%02x",unsigned(storage[i]));
        openmfc::detail::Numeric128 restored(storage+1);
        char decimal[40];unsigned length=0;
        do {decimal[length++]=static_cast<char>('0'+restored.RemoveDigit());}while(!restored.IsZero());
        std::putchar(' ');while(length)std::putchar(decimal[--length]);std::putchar('\n');
    }
    return failures ? 1:0;
}
}
int main(int argc,char** argv) {
    if(argc==2 && !std::strcmp(argv[1],"--numeric"))return numeric();
    thunk();crt();std::printf("Native runtime helpers: %d failures\n",failures);
    return failures ? 1:0;
}
