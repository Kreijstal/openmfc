// Actual OLE exception objects, roots, system messages and typed pointer throws.
#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxdisp.h"
#include "detail/RegcoreSupport.h"
#include <array>
#include <vector>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <algorithm>
#include <cerrno>
extern "C" {
int impl__GetErrorMessage_COleException__UEBAHPEA_WIPEAI_Z(const COleException*,wchar_t*,UINT,UINT*);
int impl__GetErrorMessage_COleDispatchException__UEBAHPEA_WIPEAI_Z(const COleDispatchException*,wchar_t*,UINT,UINT*);
COleDispatchException* impl___0COleDispatchException__QEAA_PEB_WIG_Z(COleDispatchException*,const wchar_t*,UINT,WORD);
CRuntimeClass* impl__GetThisClass_COleException__SAPEAUCRuntimeClass__XZ();
CRuntimeClass* impl__GetRuntimeClass_COleException__UEBAPEAUCRuntimeClass__XZ(const COleException*);
CRuntimeClass* impl__GetThisClass_COleDispatchException__SAPEAUCRuntimeClass__XZ();
CRuntimeClass* impl__GetRuntimeClass_COleDispatchException__UEBAPEAUCRuntimeClass__XZ(const COleDispatchException*);
void impl__AfxThrowOleException__YAXJ_Z(LONG);
void impl__AfxThrowOleDispatchException__YAXGPEB_WI_Z(WORD,const wchar_t*,UINT);
void impl__AfxThrowOleDispatchException__YAXGII_Z(WORD,UINT,UINT);
LONG impl__Process_COleException__SAJPEBVCException___Z(const CException*);
int impl__AfxLoadString__YAHIPEA_WI_Z(UINT,wchar_t*,UINT);
void impl__AfxThrowMemoryException__YAXXZ();
void impl__AfxThrowNotSupportedException__YAXXZ();
}
namespace ole_exception_test {
int failures;
void check(bool good,const char* message){if(!good){++failures;printf("FAIL: %s\n",message);}}
struct API {
    decltype(&impl__GetErrorMessage_COleException__UEBAHPEA_WIPEAI_Z) ole;
    decltype(&impl__GetErrorMessage_COleDispatchException__UEBAHPEA_WIPEAI_Z) dispatch;
    decltype(&impl___0COleDispatchException__QEAA_PEB_WIG_Z) construct;
    decltype(&impl__GetThisClass_COleException__SAPEAUCRuntimeClass__XZ) types[2];
    decltype(&impl__GetRuntimeClass_COleException__UEBAPEAUCRuntimeClass__XZ) runtime;
    decltype(&impl__GetRuntimeClass_COleDispatchException__UEBAPEAUCRuntimeClass__XZ) dispatchRuntime;
    decltype(&impl__AfxThrowOleException__YAXJ_Z) raise;
    decltype(&impl__AfxThrowOleDispatchException__YAXGPEB_WI_Z) raiseText;
    decltype(&impl__AfxThrowOleDispatchException__YAXGII_Z) raiseID;
    decltype(&impl__Process_COleException__SAJPEBVCException___Z) process;
    HMODULE image;
};
void** imported(HMODULE image,const char* wanted){auto* b=reinterpret_cast<BYTE*>(image);auto* pe=reinterpret_cast<IMAGE_NT_HEADERS*>(b+reinterpret_cast<IMAGE_DOS_HEADER*>(b)->e_lfanew);auto* d=reinterpret_cast<IMAGE_IMPORT_DESCRIPTOR*>(b+pe->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress);for(;d->Name;++d){auto* n=reinterpret_cast<IMAGE_THUNK_DATA*>(b+d->OriginalFirstThunk);auto* v=reinterpret_cast<IMAGE_THUNK_DATA*>(b+d->FirstThunk);for(;n->u1.AddressOfData;++n,++v)if(!IMAGE_SNAP_BY_ORDINAL(n->u1.Ordinal)){auto* name=reinterpret_cast<IMAGE_IMPORT_BY_NAME*>(b+n->u1.AddressOfData);if(!strcmp(reinterpret_cast<char*>(name->Name),wanted))return reinterpret_cast<void**>(&v->u1.Function);}}return nullptr;}
struct Patch{void** slot;void* old;DWORD protection;Patch(HMODULE image,const char* name,void* replacement){slot=imported(image,name);if(!slot)abort();old=*slot;VirtualProtect(slot,8,PAGE_READWRITE,&protection);*slot=replacement;}~Patch(){*slot=old;DWORD ignored;VirtualProtect(slot,8,protection,&ignored);}};
struct Hook{BYTE* p;BYTE old[12];DWORD protection;Hook(void* entry,void* target):p(static_cast<BYTE*>(entry)){memcpy(old,p,12);VirtualProtect(p,12,PAGE_EXECUTE_READWRITE,&protection);BYTE jump[12]={0x48,0xb8};memcpy(jump+2,&target,8);jump[10]=0xff;jump[11]=0xe0;memcpy(p,jump,12);FlushInstructionCache(GetCurrentProcess(),p,12);}~Hook(){memcpy(p,old,12);FlushInstructionCache(GetCurrentProcess(),p,12);DWORD ignored;VirtualProtect(p,12,protection,&ignored);}};
std::vector<int> events;
int copyResult=-1;bool formatted;UINT* helpOutput;LONG expectedCode;
const wchar_t text[]=L"system text\r\n";
using Copy=int (__cdecl*)(wchar_t*,size_t,const wchar_t*,size_t);
Copy realCopy;
DWORD WINAPI format(DWORD flags,const void* source,DWORD code,DWORD language,wchar_t* output,DWORD size,va_list* arguments){events.push_back(1);check(flags==0x1100&&!source&&code==DWORD(expectedCode)&&language==0x800&&!size&&!arguments,"OLE messages use native allocation flags, SCODE and system-default language");check(!helpOutput||!*helpOutput,"OLE message clears help before FormatMessage callbacks");if(!formatted)return 0;*reinterpret_cast<const wchar_t**>(output)=text;return DWORD(wcslen(text));}
HLOCAL WINAPI release(HLOCAL memory){events.push_back(3);check(memory==reinterpret_cast<HLOCAL>(const_cast<wchar_t*>(text)),"OLE messages release exactly the allocated system buffer");return reinterpret_cast<HLOCAL>(UINT_PTR(1));}
int __cdecl copy(wchar_t* output,size_t capacity,const wchar_t* source,size_t count){events.push_back(2);check(count==_TRUNCATE,"message copies use checked CRT truncation");return copyResult<0 ? realCopy(output,capacity,source,count):copyResult;}
int caughtMessage(const API& api,const COleException* error,wchar_t* output,UINT capacity,UINT* help){try{return api.ole(error,output,capacity,help);}catch(CMemoryException*){return 12;}catch(CInvalidArgException*){return 22;}catch(...){check(false,"OLE message throws the native typed framework exception");return -1;}}
void dispose(CObject* object){using Destroy=void*(*)(void*,UINT);reinterpret_cast<Destroy>((*reinterpret_cast<void***>(object))[1])(object,0);}
void messages(const API& api){
 auto image=api.image?api.image:GetModuleHandleW(nullptr);
 Patch a(image,"FormatMessageW",reinterpret_cast<void*>(&format)),b(image,"LocalFree",reinterpret_cast<void*>(&release)),c(image,"wcsncpy_s",reinterpret_cast<void*>(&copy));realCopy=reinterpret_cast<Copy>(c.old);
 COleException local;
 alignas(8) BYTE raw[24];COleException* error=&local;
 if(api.image)error=reinterpret_cast<COleException*(*)(void*)>(reinterpret_cast<BYTE*>(api.image)+0x2608b8)(raw);
 for(LONG code:{LONG(0),LONG(-1),LONG(0x8007000e)})for(bool success:{false,true})for(UINT capacity:{0u,1u,4u,32u}) {
  expectedCode=error->m_sc=code;formatted=success;copyResult=capacity? -1:22;
  wchar_t output[32];std::fill(std::begin(output),std::end(output),L'x');UINT help=91;helpOutput=&help;events.clear();
  int result=caughtMessage(api,error,output,capacity,&help);
  check(!help && result==(success?(capacity?1:22):0),"OLE messages preserve native success, failure and invalid-copy exceptions");
  check(events==(success?(capacity?std::vector<int>{1,2,3}:std::vector<int>{1,2}):std::vector<int>{1}),"OLE message allocation, checked copy and freeing follow native order including failure paths");
  if(!success)check(!output[0]&&output[1]=='x',"FormatMessage failure clears only the first output character even for zero capacity");
  else if(capacity){size_t n=(std::min)(size_t(capacity-1),wcslen(text));check(!memcmp(output,text,n*2)&&!output[n],"OLE messages retain CRLF and terminate truncated system text");}
 }
 for(int result:{0,80,12,22,34,7}){formatted=true;copyResult=result;events.clear();helpOutput=nullptr;wchar_t output[8];int actual=caughtMessage(api,error,output,8,nullptr);check(actual==((!result||result==80)?1:result==12?12:22)&&events==((!result||result==80)?std::vector<int>{1,2,3}:std::vector<int>{1,2}),"OLE checked-copy failures throw before freeing the system allocation");}
 // Both virtual slots must reach the class implementation, not the empty base.
 formatted=true;copyResult=-1;expectedCode=error->m_sc;helpOutput=nullptr;wchar_t output[32];
 check(static_cast<const CException*>(error)->GetErrorMessage(output,32,nullptr)==TRUE && !wcscmp(output,text),"actual OLE const virtual root selects the restored message method");
 check(static_cast<CException*>(error)->GetErrorMessage(output,32,nullptr)==TRUE && !wcscmp(output,text),"actual OLE non-const base virtual forwards to the restored const slot");
 if(api.image)dispose(error);
}
size_t allocatedSize;int allocationCount,loadMode;bool failAllocation;UINT wantedID;
extern "C" void* impl___2_YAPEAX_K_Z(size_t size){events.push_back(4);allocatedSize=size;++allocationCount;if(failAllocation)return nullptr;void* result=malloc(size+16);memset(result,0xa5,size+16);return result;}
extern "C" void impl___3_YAXPEAX_Z(void* memory){events.push_back(5);free(memory);}
int load(UINT id,wchar_t* output,UINT capacity){events.push_back(6);check(id==wantedID&&capacity==256,"resource dispatch factory always loads its supplied ID into 256 wchar_t storage");if(loadMode==3)throw 71;UINT count=loadMode==1?0:loadMode==2?255:8;for(UINT i=0;i<count;++i)output[i]=L'a';output[count]=0;return count;}
void validate(CException* object,const API& api,bool dispatch,WORD code,UINT help,const wchar_t* description){
 if(failAllocation){check(!object&&allocatedSize==64,"dispatch factory preserves a typed null pointer when framework allocation returns null");return;}
 check(object&&object->m_bAutoDelete==TRUE&&object->GetRuntimeClass()==api.types[dispatch?1:0](),"factories create owned objects with their real runtime roots");
 if(!object)return;
 check(allocatedSize==(dispatch?64u:24u)&&allocationCount==1,"OLE factories use the framework object allocation service with native size");
 auto* bytes=reinterpret_cast<BYTE*>(object);for(size_t i=allocatedSize;i<allocatedSize+16;++i)check(bytes[i]==0xa5,"OLE constructors preserve allocation guards");
 for(size_t i:{12u,13u,14u,15u})check(bytes[i]==0xa5,"OLE constructors preserve base tail padding");
 if(dispatch){auto* error=static_cast<COleDispatchException*>(object);check(error->m_wCode==code&&error->m_dwHelpContext==(help?help+0x60000:0)&&error->m_scError==(code?S_OK:E_UNEXPECTED)&&!wcscmp(error->m_strDescription.GetString(),description?description:L"")&&error->m_strSource.IsEmpty()&&error->m_strHelpFile.IsEmpty(),"dispatch factories preserve code, biased help, owned description and empty optional strings");}
 if(!dispatch)check(api.process(object)==static_cast<COleException*>(object)->m_sc,"OLE Process classifies actual factory objects through their runtime root");
 object->Delete();
}
template<class T,class Function>void catchAs(Function invoke,const API& api,bool dispatch,WORD code=0,UINT help=0,const wchar_t* description=nullptr){allocationCount=0;try{invoke();check(false,"OLE factory always throws");}catch(T* pointer){validate(reinterpret_cast<CException*>(const_cast<void*>(static_cast<const void*>(pointer))),api,dispatch,code,help,description);}catch(...){check(false,"OLE factories use compiler-generated pointer catch hierarchies");}}
void factories(const API& api){
 // Source allocation is the explicit framework service above; native uses its
 // corresponding outbound service. No class roots or throw metadata are patched.
 auto* base=reinterpret_cast<BYTE*>(api.image);
 Hook* allocator=api.image?new Hook(base+0x27e0,reinterpret_cast<void*>(&impl___2_YAPEAX_K_Z)):nullptr;
 Hook loader(api.image?static_cast<void*>(base+0x2af0b0):reinterpret_cast<void*>(&impl__AfxLoadString__YAHIPEA_WI_Z),reinterpret_cast<void*>(&load));
 failAllocation=false;
 for(LONG code:{LONG(0),LONG(-1),LONG(0x8007000e)}) {
  auto invoke=[&]{api.raise(code);};events.clear();catchAs<COleException>(invoke,api,false);check(events==std::vector<int>{4},"OLE factory uses one object allocation before a typed throw");
  catchAs<CException>(invoke,api,false);catchAs<CObject>(invoke,api,false);catchAs<void>(invoke,api,false);catchAs<const CException>(invoke,api,false);
 }
 for(WORD code:{WORD(0),WORD(1),WORD(0xffff)})for(UINT help:{0u,1u,0xffffffffu})for(const wchar_t* description:std::array<const wchar_t*,3>{nullptr,L"",L"text\u7de8\x0tail"}) {
  auto invoke=[&]{api.raiseText(code,description,help);};catchAs<COleDispatchException>(invoke,api,true,code,help,description);catchAs<CException>(invoke,api,true,code,help,description);catchAs<CObject>(invoke,api,true,code,help,description);catchAs<void>(invoke,api,true,code,help,description);
 }
 for(UINT id:{0u,17u})for(UINT help:{0u,1u,UINT_MAX})for(int mode:{0,1,2,3}) {
  wantedID=id;loadMode=mode;events.clear();allocationCount=0;wchar_t expected[256];std::fill(std::begin(expected),std::end(expected),L'a');expected[mode==1?0:mode==2?255:8]=0;
  if(mode==3){bool caught=false;try{api.raiseID(7,id,help);}catch(int value){caught=value==71;}check(caught&&!allocationCount&&events==std::vector<int>{6},"resource-loader exceptions propagate before dispatch object allocation");}
  else{catchAs<COleDispatchException>([&]{api.raiseID(7,id,help);},api,true,7,help==UINT_MAX?id:help,expected);check(events==std::vector<int>({6,4}),"resource dispatch factory loads before allocating even for zero or missing resource IDs");}
 }
 failAllocation=true;catchAs<COleDispatchException>([&]{api.raiseText(7,L"text",0);},api,true);wantedID=17;loadMode=0;catchAs<CException>([&]{api.raiseID(7,17,0);},api,true);failAllocation=false;
 delete allocator;
}
void dispatchMessages(const API& api){
 alignas(8) BYTE bytes[80];memset(bytes,0xa5,sizeof(bytes));auto* error=api.construct(reinterpret_cast<COleDispatchException*>(bytes),L"AB\0CD",0x123,7);
 auto image=api.image?api.image:GetModuleHandleW(nullptr);Patch patch(image,"wcsncpy_s",reinterpret_cast<void*>(&copy));realCopy=reinterpret_cast<Copy>(patch.old);
 for(UINT capacity:{1u,2u,16u}){copyResult=-1;events.clear();UINT help=91;wchar_t output[16];check(api.dispatch(error,output,capacity,&help)==TRUE&&!help&&!wcsncmp(output,L"AB",capacity-1)&&output[(std::min)(capacity-1,2u)]==0,"dispatch messages clear help and use null-terminated checked truncation");}
 for(int result:{0,80,12,22,34,7}){copyResult=result;events.clear();UINT help=91;wchar_t output[4];int actual=0;try{actual=api.dispatch(error,output,4,&help);}catch(CMemoryException*){actual=12;}catch(CInvalidArgException*){actual=22;}check(!help&&actual==((!result||result==80)?1:result==12?12:22),"dispatch checked-copy results preserve native typed exceptions");}
 copyResult=-1;UINT help=91;wchar_t output[16];check(static_cast<CException*>(error)->GetErrorMessage(output,16,&help)&&!help&&!wcscmp(output,L"AB"),"actual dispatch non-const virtual root forwards to its own const message method");
 dispose(error);for(unsigned i=64;i<80;++i)check(bytes[i]==0xa5,"dispatch constructor and nondeleting virtual destruction preserve guards");
}
void process(const API& api){
 COleException ole;ole.m_sc=E_ACCESSDENIED;check(api.process(&ole)==E_ACCESSDENIED,"OLE Process preserves stored SCODE for OLE exceptions");
 if(api.image)return; // Native needs its own runtime identity, checked below via factory objects.
 try{impl__AfxThrowMemoryException__YAXXZ();}catch(CException* error){check(api.process(error)==E_OUTOFMEMORY,"OLE Process recognizes actual memory exception ancestry");}
 try{impl__AfxThrowNotSupportedException__YAXXZ();}catch(CException* error){check(api.process(error)==E_NOTIMPL,"OLE Process recognizes actual not-supported exception ancestry");}
}
}
// Module-dependent EXCEPINFO conversion is exercised with real module owners
// by the extension-library fixture; this test isolates the object and services.
extern "C" AFX_MODULE_STATE* impl__AfxGetModuleState__YAPEAVAFX_MODULE_STATE__XZ(){std::abort();}
int TestOleExceptions(HMODULE image){using namespace ole_exception_test;
 API api{impl__GetErrorMessage_COleException__UEBAHPEA_WIPEAI_Z,impl__GetErrorMessage_COleDispatchException__UEBAHPEA_WIPEAI_Z,impl___0COleDispatchException__QEAA_PEB_WIG_Z,
 {impl__GetThisClass_COleException__SAPEAUCRuntimeClass__XZ,impl__GetThisClass_COleDispatchException__SAPEAUCRuntimeClass__XZ},impl__GetRuntimeClass_COleException__UEBAPEAUCRuntimeClass__XZ,impl__GetRuntimeClass_COleDispatchException__UEBAPEAUCRuntimeClass__XZ,
 impl__AfxThrowOleException__YAXJ_Z,impl__AfxThrowOleDispatchException__YAXGPEB_WI_Z,impl__AfxThrowOleDispatchException__YAXGII_Z,impl__Process_COleException__SAJPEBVCException___Z,image};
 if(image){auto* b=reinterpret_cast<BYTE*>(image);
#define RVA(member,value) api.member=reinterpret_cast<decltype(api.member)>(b+value)
 RVA(ole,0x25f300);RVA(dispatch,0x2515b0);RVA(construct,0x2514c0);RVA(runtime,0x260870);RVA(dispatchRuntime,0x2516e0);RVA(raise,0x25f2c0);RVA(raiseText,0x251600);RVA(raiseID,0x251660);RVA(process,0x25f3b0);
#undef RVA
 api.types[0]=reinterpret_cast<CRuntimeClass*(*)()>(b+0x260870);api.types[1]=reinterpret_cast<CRuntimeClass*(*)()>(b+0x2516e0);
 }
 events.reserve(32);for(int i=0;i<2;++i){auto* type=api.types[i]();check(type&&!strcmp(type->m_lpszClassName,i?"COleDispatchException":"COleException")&&type->m_nObjectSize==(i?64:24)&&type->m_wSchema==0xffff&&!type->m_pfnCreateObject&&type->BaseClass()&&!strcmp(type->BaseClass()->m_lpszClassName,"CException"),"actual OLE runtime metadata has native size, schema, factory and base ancestry");}
 check(api.runtime(nullptr)==api.types[0]()&&api.dispatchRuntime(nullptr)==api.types[1](),"canonical OLE runtime getters do not inspect the supplied object");
 messages(api);dispatchMessages(api);factories(api);if(!image)process(api);
 printf("%s OLE exceptions: %d failures\n",image?"Native":"Source",failures);return failures;
}
