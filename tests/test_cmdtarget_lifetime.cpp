// Byte-level lifetime checks use the same raw storage and dispatch callbacks
// with our exports and, optionally, a native MFC DLL. No base vtable is faked.
#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxwin.h"
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <exception>
#include <type_traits>

extern "C" CCmdTarget* AFXAPI impl___0CCmdTarget__QEAA_XZ(CCmdTarget*);
extern "C" void AFXAPI impl___1CCmdTarget__UEAA_XZ(CCmdTarget*);
using Constructor = void* (AFXAPI*)(void*);
using Destructor = void (AFXAPI*)(void*);
using ModuleGetter = AFX_MODULE_STATE* (AFXAPI*)();
static int failures;
static void check(bool condition, const char* message) {
    if (!condition) { ++failures; std::printf("FAIL: %s\n",message); }
}
struct alignas(16) Storage {
    unsigned char bytes[96];
    void* object() { return bytes+16; }
    void reset() { std::memset(bytes,0xa5,sizeof(bytes)); }
};
template<class T> static void store(void* object,size_t offset,T value) {
    std::memcpy(static_cast<unsigned char*>(object)+offset,&value,sizeof(value));
}
template<class T> static T load(const void* object,size_t offset) {
    T result;std::memcpy(&result,static_cast<const unsigned char*>(object)+offset,sizeof(result));return result;
}
static Storage* observed;
static int getterCalls;
static bool throwGetter;
static auto* const sourceModule=reinterpret_cast<AFX_MODULE_STATE*>(UINT_PTR(0x12345000));
extern "C" AFX_MODULE_STATE* AFXAPI impl__AfxGetModuleState__YAPEAVAFX_MODULE_STATE__XZ() {
    ++getterCalls;
    if (observed) {
        unsigned char untouched[96];std::memset(untouched,0xa5,sizeof(untouched));
        check(!std::memcmp(observed->bytes,untouched,sizeof(untouched)),
            "module lookup happens before any constructor writes, including vfptr writes");
    }
    if (throwGetter) throw 37;
    return sourceModule;
}
static void constructorChecks(Constructor ctor,AFX_MODULE_STATE* module,bool source) {
    Storage actual,expected;actual.reset();expected.reset();
    if(source)observed=&actual;
    check(ctor(actual.object())==actual.object(),"constructor returns the supplied storage");
    observed=nullptr;
    store(expected.object(),8,DWORD(1));
    store(expected.object(),16,UINT_PTR(0));
    store(expected.object(),24,UINT_PTR(0));
    store(expected.object(),32,UINT_PTR(0));
    store(expected.object(),40,BOOL(TRUE));
    store(expected.object(),48,UINT_PTR(0));
    store(expected.object(),56,module);
    check(!std::memcmp(actual.bytes,expected.bytes,sizeof(actual.bytes)),
        "constructor initializes native fields and preserves root vfptr, padding and canaries");
}
static void* callbackObject;
static void* callbackModule;
static int disconnectCalls;
static bool mutate;
static bool throwDisconnect;
static void AFXAPI disconnect(void* embedded) {
    ++disconnectCalls;
    check(embedded==static_cast<unsigned char*>(callbackObject)+32,
        "destructor passes the embedded dispatch interface to slot seven");
    check(load<void*>(callbackObject,56)==callbackModule,
        "module state remains available during dispatch disconnection");
    check(load<UINT_PTR>(callbackObject,0)==UINT_PTR(0xa5a5a5a5a5a5a5a5ull),
        "base destruction preserves the incoming derived vfptr");
    if(throwDisconnect)throw 41;
    if(mutate) {
        store(callbackObject,0,UINT_PTR(0x1111222233334444ull));
        store(callbackObject,8,DWORD(23));
        store(callbackObject,32,UINT_PTR(0));
        store(callbackObject,56,reinterpret_cast<void*>(UINT_PTR(0x33330000)));
    }
}
static void destructorChecks(Destructor dtor) {
    for(int mode=0;mode!=3;++mode) {
        Storage actual,expected;actual.reset();expected.reset();
        void* table[8]{};table[7]=reinterpret_cast<void*>(&disconnect);
        store(actual.object(),32,mode ? static_cast<void*>(table) : nullptr);
        store(expected.object(),32,mode ? static_cast<void*>(table) : nullptr);
        callbackObject=actual.object();callbackModule=load<void*>(actual.object(),56);
        disconnectCalls=0;mutate=mode==2;
        if(mutate) {
            store(expected.object(),0,UINT_PTR(0x1111222233334444ull));
            store(expected.object(),8,DWORD(23));
            store(expected.object(),32,UINT_PTR(0));
        }
        store(expected.object(),56,UINT_PTR(0));
        dtor(actual.object());
        check(disconnectCalls==(mode ? 1:0),"only a live dispatch interface is disconnected");
        check(!std::memcmp(actual.bytes,expected.bytes,sizeof(actual.bytes)),
            "destructor clears module state after callbacks and preserves all other bytes");
    }
}
static void crossover(Constructor ctor,Destructor dtor) {
    Storage actual;actual.reset();ctor(actual.object());
    unsigned char expected[96];std::memcpy(expected,actual.bytes,sizeof(expected));
    std::memset(expected+16+56,0,sizeof(void*));
    dtor(actual.object());
    check(!std::memcmp(actual.bytes,expected,sizeof(expected)),
        "constructors and destructors from either DLL accept the same native object layout");
}
[[noreturn]] static void terminateChild(Destructor dtor) {
    Storage actual;actual.reset();
    void* table[8]{};table[7]=reinterpret_cast<void*>(&disconnect);
    store(actual.object(),32,static_cast<void*>(table));
    callbackObject=actual.object();callbackModule=load<void*>(callbackObject,56);
    disconnectCalls=0;mutate=false;throwDisconnect=true;
    std::set_terminate([] {
        bool intact=!failures && disconnectCalls==1 &&
            load<void*>(callbackObject,56)==callbackModule &&
            load<UINT_PTR>(callbackObject,0)==UINT_PTR(0xa5a5a5a5a5a5a5a5ull);
        ExitProcess(intact ? 86:87);
    });
    try { dtor(actual.object()); }catch(...) { ExitProcess(88); }
    ExitProcess(89);
}
static void terminationCheck(const wchar_t* argument) {
    wchar_t path[1024],command[1200];
    GetModuleFileNameW(nullptr,path,1024);
    _snwprintf_s(command,_countof(command),_TRUNCATE,L"\"%ls\" %ls",path,argument);
    STARTUPINFOW startup{};startup.cb=sizeof(startup);PROCESS_INFORMATION child{};
    bool created=CreateProcessW(nullptr,command,nullptr,nullptr,FALSE,0,nullptr,nullptr,&startup,&child)!=FALSE;
    check(created,"launch destructor exception-contract check");
    if(!created)return;
    DWORD wait=WaitForSingleObject(child.hProcess,30000),code=0;
    if(wait!=WAIT_OBJECT_0)TerminateProcess(child.hProcess,90);
    GetExitCodeProcess(child.hProcess,&code);
    check(wait==WAIT_OBJECT_0 && code==86,
        "throwing dispatch disconnection terminates before module/vfptr cleanup");
    CloseHandle(child.hThread);CloseHandle(child.hProcess);
}
int main(int argc,char** argv) {
    static_assert(sizeof(CCmdTarget)==64,"native base layout");
    static_assert(std::is_abstract<CObject>::value,"CObject has a pure destructor in the SDK");
    auto sourceCtor=reinterpret_cast<Constructor>(&impl___0CCmdTarget__QEAA_XZ);
    auto sourceDtor=reinterpret_cast<Destructor>(&impl___1CCmdTarget__UEAA_XZ);
    if(argc>1 && !std::strcmp(argv[1],"--terminate-source"))terminateChild(sourceDtor);
    constructorChecks(sourceCtor,sourceModule,true);
    check(getterCalls==1,"base constructor looks up module state once");
    Storage actual,expected;actual.reset();expected.reset();observed=&actual;throwGetter=true;
    bool propagated=false;
    try { sourceCtor(actual.object()); }catch(int value) { propagated=value==37; }
    throwGetter=false;observed=nullptr;
    check(propagated && !std::memcmp(actual.bytes,expected.bytes,sizeof(actual.bytes)),
        "module lookup failure propagates without modifying storage");
    destructorChecks(sourceDtor);crossover(sourceCtor,sourceDtor);
    terminationCheck(L"--terminate-source");
    if(const char* path=std::getenv("OPENMFC_NATIVE_CMDTARGET_DLL")) {
        HMODULE native=LoadLibraryA(path);
        check(native!=nullptr,"load the native MFC comparison DLL");
        bool pinned=false;
        if(native) {
            auto* bytes=reinterpret_cast<BYTE*>(native);
            auto* pe=reinterpret_cast<IMAGE_NT_HEADERS*>(bytes+reinterpret_cast<IMAGE_DOS_HEADER*>(bytes)->e_lfanew);
            pinned=pe->FileHeader.TimeDateStamp==0x442cba30 && pe->OptionalHeader.SizeOfImage==0x55c000;
            check(pinned,"native comparison uses the verified MFC build");
        }
        if(pinned) {
            // This retail DLL exports by ordinal. These RVAs are for the
            // timestamp/image-size-checked build above, not the ANSI DLL.
            auto* bytes=reinterpret_cast<BYTE*>(native);
            auto ctor=reinterpret_cast<Constructor>(bytes+0x1de3f0);
            auto dtor=reinterpret_cast<Destructor>(bytes+0x1de430);
            auto get=reinterpret_cast<ModuleGetter>(bytes+0x133930);
            if(argc>1 && !std::strcmp(argv[1],"--terminate-native"))terminateChild(dtor);
            constructorChecks(ctor,get(),false);destructorChecks(dtor);
            crossover(sourceCtor,dtor);crossover(ctor,sourceDtor);
            terminationCheck(L"--terminate-native");
        }
        if(native)FreeLibrary(native);
    }
    std::printf("Command-target lifetime: %d failures\n",failures);
    return failures ? 1:0;
}
