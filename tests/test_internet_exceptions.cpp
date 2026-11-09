// Real Internet exception objects and WinInet/CRT service-boundary comparisons.
#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxinet.h"
#include "detail/WindowActivationSupport.h"
#include <array>
#include <vector>
#include <string>
#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cerrno>
extern "C" {
CInternetException* impl___0CInternetException__QEAA_K_Z(CInternetException*,DWORD);
void impl___1CInternetException__UEAA_XZ(CInternetException*);
int impl__GetErrorMessage_CInternetException__UEBAHPEA_WIPEAI_Z(const CInternetException*,wchar_t*,UINT,UINT*);
void impl__AfxThrowInternetException__YAX_KK_Z(DWORD_PTR,DWORD);
CRuntimeClass* impl__GetThisClass_CInternetException__SAPEAUCRuntimeClass__XZ();
CRuntimeClass* impl__GetRuntimeClass_CInternetException__UEBAPEAUCRuntimeClass__XZ(const CInternetException*);
void* impl___2_YAPEAX_K_Z(size_t);
}
namespace internet_exception_test {
int failures;unsigned comparisons;
void check(bool good,const char* message){if(!good){++failures;printf("FAIL: %s\n",message);}}
struct API {
 decltype(&impl___0CInternetException__QEAA_K_Z) construct;
 decltype(&impl___1CInternetException__UEAA_XZ) destroy;
 decltype(&impl__GetErrorMessage_CInternetException__UEBAHPEA_WIPEAI_Z) message;
 decltype(&impl__AfxThrowInternetException__YAX_KK_Z) raise;
 decltype(&impl__GetThisClass_CInternetException__SAPEAUCRuntimeClass__XZ) type;
 decltype(&impl__GetRuntimeClass_CInternetException__UEBAPEAUCRuntimeClass__XZ) runtime;
 BOOL *cleanup,*bypass,*unavailable;HANDLE* context;HMODULE image;
};
const API* api;
using Event=std::array<INT64,4>;
std::vector<Event> events;
void event(INT64 kind,INT64 first=0,INT64 second=0,INT64 third=0){events.push_back({kind,first,second,third});}
void** imported(HMODULE image,const char* wanted){auto* b=reinterpret_cast<BYTE*>(image);auto* pe=reinterpret_cast<IMAGE_NT_HEADERS*>(b+reinterpret_cast<IMAGE_DOS_HEADER*>(b)->e_lfanew);auto* d=reinterpret_cast<IMAGE_IMPORT_DESCRIPTOR*>(b+pe->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress);for(;d->Name;++d){auto* n=reinterpret_cast<IMAGE_THUNK_DATA*>(b+d->OriginalFirstThunk);auto* v=reinterpret_cast<IMAGE_THUNK_DATA*>(b+d->FirstThunk);for(;n->u1.AddressOfData;++n,++v)if(!IMAGE_SNAP_BY_ORDINAL(n->u1.Ordinal)){auto* name=reinterpret_cast<IMAGE_IMPORT_BY_NAME*>(b+n->u1.AddressOfData);if(!strcmp(reinterpret_cast<char*>(name->Name),wanted))return reinterpret_cast<void**>(&v->u1.Function);}}return nullptr;}
struct Patch {
 void** slot;void* old;DWORD protection;
 Patch(void** target,void* callback):slot(target){check(slot!=nullptr,"locate Internet exception service");if(!slot)abort();old=*slot;VirtualProtect(slot,8,PAGE_READWRITE,&protection);*slot=callback;}
 Patch(HMODULE image,const char* name,void* callback):Patch(imported(image,name),callback){}
 ~Patch(){*slot=old;DWORD ignored;VirtualProtect(slot,8,protection,&ignored);}
};
struct Hook {
 BYTE* address;BYTE old[12];DWORD protection;
 Hook(void* entry,void* callback):address(static_cast<BYTE*>(entry)){memcpy(old,address,12);VirtualProtect(address,12,PAGE_EXECUTE_READWRITE,&protection);BYTE jump[12]={0x48,0xb8};memcpy(jump+2,&callback,8);jump[10]=0xff;jump[11]=0xe0;memcpy(address,jump,12);FlushInstructionCache(GetCurrentProcess(),address,12);}
 ~Hook(){memcpy(address,old,12);FlushInstructionCache(GetCurrentProcess(),address,12);DWORD ignored;VirtualProtect(address,12,protection,&ignored);}
};
using Copy=int (__cdecl*)(wchar_t*,size_t,const wchar_t*,size_t);
Copy realCopy;
const wchar_t messageText[]=L"Internet message\r\n";
wchar_t responseText[1024];
CInternetException* activeObject;UINT* activeHelp;
int loadMode,formatMode,responseMode,copyResult,mutation,foreignFailure;
bool allocationFailure,activateResult;
DWORD lastError,responseLength,secondLength,activationError;
DWORD WINAPI getError(){event(4,lastError);return lastError;}
void WINAPI setError(DWORD value){event(11,value);lastError=value;}
HMODULE WINAPI loadLibrary(const wchar_t* name,HANDLE file,DWORD flags){
 event(1,flags);check(!wcscmp(name,L"WININET.DLL")&&!file,"Internet messages load native WinInet resource DLL flags");check(!activeHelp||!*activeHelp,"help is cleared before library loading");
 if(mutation==1)activeObject->m_dwError=ERROR_INTERNET_EXTENDED_ERROR;
 if(foreignFailure==1)throw 71;
 lastError=73;
 return loadMode==2||(loadMode==1&&flags==0x60)?nullptr:reinterpret_cast<HMODULE>(UINT_PTR(0x44));
}
DWORD WINAPI format(DWORD flags,const void* module,DWORD code,DWORD language,wchar_t* output,DWORD size,va_list* args){
 event(2,flags,code);check(language==0x800&&!size&&!args,"Internet messages allocate with system-default language and no insert array");
 check(flags==(module?0x900u:0x1100u)&&(!module||module==reinterpret_cast<void*>(UINT_PTR(0x44))),"Internet messages try module resources before system resources");
 if(mutation==2)activeObject->m_dwError=ERROR_INTERNET_EXTENDED_ERROR;
 if(foreignFailure==2)throw 72;
 if((module&&formatMode==1)||(!module&&formatMode==2)){
  *reinterpret_cast<const wchar_t**>(output)=messageText;return DWORD(wcslen(messageText));
 }
 return 0;
}
BOOL WINAPI response(DWORD* code,wchar_t* output,DWORD* length){
 event(3,output!=nullptr,*length);*code=99;
 if(!output){check(!*length,"extended response first probes an empty buffer");*length=responseLength;lastError=responseMode==1?5:ERROR_INSUFFICIENT_BUFFER;return responseMode==0;}
 check(output==responseText,"extended response uses the LocalAlloc buffer");
 wcscpy_s(responseText,1024,L"reply");*length=secondLength;return responseMode!=4;
}
HLOCAL WINAPI allocate(UINT flags,SIZE_T size){event(5,flags,INT64(size));check(flags==LPTR,"extended response allocation is zero initialized");std::fill(std::begin(responseText),std::end(responseText),0);return allocationFailure?nullptr:reinterpret_cast<HLOCAL>(responseText);}
HLOCAL WINAPI release(HLOCAL memory){event(7,memory==reinterpret_cast<HLOCAL>(responseText)?2:1);check(memory==reinterpret_cast<HLOCAL>(responseText)||memory==reinterpret_cast<HLOCAL>(const_cast<wchar_t*>(messageText)),"Internet messages free only their allocated message buffers");return nullptr;}
int __cdecl copy(wchar_t* output,size_t capacity,const wchar_t* source,size_t count){event(6,capacity,source==responseText?2:1,count==_TRUNCATE);check(count==_TRUNCATE,"Internet messages copy with checked truncation");return copyResult<0?realCopy(output,capacity,source,count):copyResult;}
BOOL WINAPI activate(HANDLE context,ULONG_PTR* cookie){event(8,INT_PTR(context),*cookie);lastError=activationError;if(activateResult)*cookie=0x123456789ULL;return activateResult;}
BOOL WINAPI deactivate(DWORD flags,ULONG_PTR cookie){event(9,flags,INT64(cookie));lastError=0x54321;return TRUE;}
void* allocateObject(size_t size){event(10,size);void* out=malloc(size+16);memset(out,0xa5,size+16);lastError=0xeeee;return out;}
struct Result {
 std::array<wchar_t,64> output;UINT help;int result,caught;DWORD last;BOOL unavailable;std::vector<Event> calls;
 bool operator==(const Result& b)const{return output==b.output&&help==b.help&&result==b.result&&caught==b.caught&&last==b.last&&unavailable==b.unavailable&&calls==b.calls;}
};
std::vector<Result> golden;
void record(const Result& value){
 if(api->image){bool good=comparisons<golden.size()&&value==golden[comparisons];if(!good&&failures<5){printf("Internet mismatch %u result%d/%d caught%d/%d last%x/%x calls ",comparisons,value.result,golden[comparisons].result,value.caught,golden[comparisons].caught,value.last,golden[comparisons].last);for(auto e:value.calls)printf("%lld,",e[0]);printf(" / ");for(auto e:golden[comparisons].calls)printf("%lld,",e[0]);printf("\n");}check(good,"Internet messages match native outputs, callback arguments, errors and operation order");}
 else golden.push_back(value);
 ++comparisons;
}
void reset(){events.clear();loadMode=0;formatMode=1;responseMode=3;copyResult=-1;mutation=foreignFailure=0;allocationFailure=false;activateResult=true;responseLength=8;secondLength=6;lastError=0x43210;activationError=5;*api->cleanup=FALSE;*api->bypass=*api->unavailable=TRUE;*api->context=reinterpret_cast<HANDLE>(UINT_PTR(0x6677));}
void sample(CInternetException* error,UINT capacity,bool help=true,bool nullOutput=false){
 Result out{};out.output.fill(L'x');out.help=91;activeObject=error;activeHelp=help?&out.help:nullptr;
 try{out.result=api->message(error,nullOutput?nullptr:out.output.data(),capacity,activeHelp);}
 catch(CMemoryException*){out.caught=12;}catch(CInvalidArgException*){out.caught=22;}catch(int n){out.caught=n;}catch(...){check(false,"Internet messages preserve typed framework exceptions");out.caught=-1;}
 out.last=lastError;out.unavailable=*api->unavailable;out.calls=events;activeHelp=nullptr;record(out);
}
int derivedDestructions;
struct DerivedInternet : CInternetException {
 DerivedInternet():CInternetException(5){}
 ~DerivedInternet() override {++derivedDestructions;}
};
void constructors(){
 {alignas(DerivedInternet) BYTE raw[sizeof(DerivedInternet)];derivedDestructions=0;
 auto* object=::new(raw) DerivedInternet;api->destroy(object);
 check(!derivedDestructions,"exported Internet base destructor suppresses derived virtual dispatch");}
 for(DWORD code:std::array<DWORD,4>{0u,1u,ERROR_INTERNET_EXTENDED_ERROR,0xffffffffu}){
  alignas(8) BYTE raw[48];memset(raw,0xa5,sizeof(raw));auto* error=api->construct(reinterpret_cast<CInternetException*>(raw),code);
  check(reinterpret_cast<void*>(error)==raw&&error->m_dwError==code&&error->m_bAutoDelete==TRUE,"Internet constructor initializes error and auto-delete in the actual object");
  check(error->m_dwContext==DWORD_PTR(0xa5a5a5a5a5a5a5a5ULL),"Internet constructor leaves context untouched until the factory assigns it");
  for(size_t at:{12u,13u,14u,15u,20u,21u,22u,23u,32u,47u})check(raw[at]==0xa5,"Internet constructor preserves padding and caller guards");
  auto* type=api->type();check(type==error->GetRuntimeClass()&&api->runtime(nullptr)==type&&!strcmp(type->m_lpszClassName,"CInternetException")&&type->m_nObjectSize==32&&type->m_wSchema==0xffff&&!type->m_pfnCreateObject,"Internet metadata describes the actual non-serializable 32-byte class");
  check(type->m_pfnGetBaseClass()&& !strcmp(type->m_pfnGetBaseClass()->m_lpszClassName,"CException"),"Internet runtime ancestry resolves the exception base");api->destroy(error);
 }
}
template<class T>void catchFactory(DWORD_PTR context,DWORD code){
 reset();lastError=0x12345678;
 try{api->raise(context,code);check(false,"Internet factory always throws");}
 catch(T* caught){auto* error=reinterpret_cast<CInternetException*>(const_cast<void*>(static_cast<const void*>(caught)));
  check(error&&error->m_dwError==(code?code:0x12345678)&&error->m_dwContext==context&&error->m_bAutoDelete&&error->GetRuntimeClass()==api->type(),"Internet factory preserves pointer-sized context, explicit or captured last error, and real metadata");
  check(events==(code?std::vector<Event>{{10,32,0,0}}:std::vector<Event>{{4,0x12345678,0,0},{10,32,0,0}}),"Internet factory captures last error before its single native-sized allocation");
  for(size_t at:{12u,13u,14u,15u,20u,21u,22u,23u,32u,47u})check(reinterpret_cast<BYTE*>(error)[at]==0xa5,"Internet factory and constructor preserve allocation padding and guards");error->Delete();
 }catch(...){check(false,"Internet factory supports ordinary compiler-generated derived/base/void pointer catches");}
}
void messages(){
 alignas(8) BYTE raw[48];auto* error=api->construct(reinterpret_cast<CInternetException*>(raw),7);
 for(DWORD code:std::array<DWORD,4>{0u,5u,ERROR_INTERNET_EXTENDED_ERROR,0xffffffffu})for(int loader:{0,1,2})for(int formatter:{0,1,2})for(UINT cap:{0u,1u,4u,8u,64u})for(bool help:{false,true}){
  reset();error->m_dwError=code;loadMode=loader;formatMode=formatter;if(!cap)copyResult=22;sample(error,cap,help);
 }
 for(int mode:{0,1,3,4})for(bool fail:{false,true})for(DWORD length:{0u,1u,6u,8u,0xffffffffu})for(UINT cap:{0u,1u,4u,8u,64u}){
  reset();error->m_dwError=ERROR_INTERNET_EXTENDED_ERROR;responseMode=mode;allocationFailure=fail;responseLength=length;secondLength=length;copyResult=cap?-1:22;sample(error,cap);
 }
 for(int action:{1,2})for(DWORD code:std::array<DWORD,2>{5u,ERROR_INTERNET_EXTENDED_ERROR}){
  reset();error->m_dwError=code;mutation=action;formatMode=2;sample(error,64);
 }
 for(bool extended:{false,true})for(int result:{0,80,12,22,34,7}){
  reset();error->m_dwError=extended?ERROR_INTERNET_EXTENDED_ERROR:5;copyResult=result;sample(error,64);
 }
 for(int failedCall:{1,2}){
  reset();error->m_dwError=5;foreignFailure=failedCall;sample(error,64);
 }
 for(bool bypass:{false,true})for(bool unavailable:{false,true})for(bool activated:{false,true})for(int loader:{0,1,2})for(DWORD failure:std::array<DWORD,2>{5u,ERROR_CALL_NOT_IMPLEMENTED}){
  reset();error->m_dwError=5;*api->bypass=bypass;*api->unavailable=unavailable;activateResult=activated;loadMode=loader;activationError=failure;formatMode=2;sample(error,64);
 }
 for(bool activated:{false,true}){
  reset();error->m_dwError=5;*api->bypass=*api->unavailable=FALSE;activateResult=activated;foreignFailure=1;sample(error,64);
 }
 reset();error->m_dwError=5;copyResult=22;sample(error,0,true,true);
 api->destroy(error);
}
}
int TestInternetExceptions(HMODULE image){using namespace internet_exception_test;failures=0;comparisons=0;
 auto& state=openmfc::detail::windowActivation;
 API source{impl___0CInternetException__QEAA_K_Z,impl___1CInternetException__UEAA_XZ,impl__GetErrorMessage_CInternetException__UEBAHPEA_WIPEAI_Z,impl__AfxThrowInternetException__YAX_KK_Z,impl__GetThisClass_CInternetException__SAPEAUCRuntimeClass__XZ,impl__GetRuntimeClass_CInternetException__UEBAPEAUCRuntimeClass__XZ,&state.cleanupStarted,&state.bypass,&state.unavailable,&state.context,nullptr};
 API native{};if(image){auto* b=reinterpret_cast<BYTE*>(image);native={reinterpret_cast<decltype(native.construct)>(b+0x231bb0),reinterpret_cast<decltype(native.destroy)>(b+0x231c10),reinterpret_cast<decltype(native.message)>(b+0x2319e0),reinterpret_cast<decltype(native.raise)>(b+0x231980),reinterpret_cast<decltype(native.type)>(b+0x231c20),reinterpret_cast<decltype(native.runtime)>(b+0x231c20),reinterpret_cast<BOOL*>(b+0x3c3a58),reinterpret_cast<BOOL*>(b+0x3c3a68),reinterpret_cast<BOOL*>(b+0x3c3a6c),reinterpret_cast<HANDLE*>(b+0x3b2860),image};}
 api=image?&native:&source;BOOL cleanup=*api->cleanup,bypass=*api->bypass,unavailable=*api->unavailable;HANDLE context=*api->context;
 constructors();
 auto module=image?image:GetModuleHandleW(nullptr);
 {
 Patch a(module,"LoadLibraryExW",reinterpret_cast<void*>(&loadLibrary)),b(module,"FormatMessageW",reinterpret_cast<void*>(&format)),c(module,"GetLastError",reinterpret_cast<void*>(&getError)),d(module,"SetLastError",reinterpret_cast<void*>(&setError)),e(module,"LocalAlloc",reinterpret_cast<void*>(&allocate)),f(module,"LocalFree",reinterpret_cast<void*>(&release)),g(module,"wcsncpy_s",reinterpret_cast<void*>(&copy)),h(module,"ActivateActCtx",reinterpret_cast<void*>(&activate)),i(module,"DeactivateActCtx",reinterpret_cast<void*>(&deactivate));
 Patch j(image?reinterpret_cast<void**>(reinterpret_cast<BYTE*>(image)+0x3e91d0):imported(module,"InternetGetLastResponseInfoW"),reinterpret_cast<void*>(&response));realCopy=reinterpret_cast<Copy>(g.old);
 messages();
 {Hook allocation(image?static_cast<void*>(reinterpret_cast<BYTE*>(image)+0x27e0):reinterpret_cast<void*>(&impl___2_YAPEAX_K_Z),reinterpret_cast<void*>(&allocateObject));
 for(DWORD_PTR context:{DWORD_PTR(0),DWORD_PTR(1),DWORD_PTR(0x1234567887654321ULL),DWORD_PTR(-1)})for(DWORD code:std::array<DWORD,4>{0u,1u,ERROR_INTERNET_EXTENDED_ERROR,0xffffffffu}){
 catchFactory<CInternetException>(context,code);catchFactory<CException>(context,code);catchFactory<CObject>(context,code);catchFactory<void>(context,code);catchFactory<const CException>(context,code);
 }}
 }
 *api->cleanup=cleanup;*api->bypass=bypass;*api->unavailable=unavailable;*api->context=context;
 printf("%s Internet exceptions: %u native comparisons, %d failures\n",image?"Native":"Source",comparisons,failures);return failures;
}
