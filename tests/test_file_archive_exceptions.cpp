// Real file/archive classes, string resources, formatting and buffer owners.
#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxdisp.h"
#include <array>
#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <climits>
#include <cerrno>
#include <exception>
extern void* (*g_fileExceptionFinder)(UINT);
extern "C" {
int impl__GetErrorMessage_CFileException__UEBAHPEA_WIPEAI_Z(const CFileException*,wchar_t*,UINT,UINT*);
int impl__GetErrorMessage_CArchiveException__UEBAHPEA_WIPEAI_Z(const CArchiveException*,wchar_t*,UINT,UINT*);
void impl__AfxThrowFileException__YAXHJPEB_W_Z(int,LONG,const wchar_t*);
void impl__AfxThrowArchiveException__YAXHPEB_W_Z(int,const wchar_t*);
int impl__ErrnoToException_CFileException__SAHH_Z(int);
void impl__ThrowErrno_CFileException__SAXHPEB_W_Z(int,const wchar_t*);
void impl__AfxFormatStrings__YAXAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__PEB_WPEBQEB_WH_Z(CString*,const wchar_t*,const wchar_t* const*,int);
void impl__AfxFormatStrings__YAXAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__IPEBQEB_WH_Z(CString*,UINT,const wchar_t* const*,int);
void impl__AfxFormatString1__YAXAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__IPEB_W_Z(CString*,UINT,const wchar_t*);
void impl__AfxFormatString2__YAXAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__IPEB_W1_Z(CString*,UINT,const wchar_t*,const wchar_t*);
int impl__LoadStringW___CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAAHI_Z(CString*,UINT);
int impl__LoadStringW___CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAAHPEAUHINSTANCE____I_Z(CString*,HINSTANCE,UINT);
int impl__LoadStringW___CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAAHPEAUHINSTANCE____IG_Z(CString*,HINSTANCE,UINT,WORD);
wchar_t* impl__GetBuffer___CSimpleStringT__W_00_ATL__QEAAPEA_WH_Z(CString*,int);
wchar_t* impl__GetBuffer___CSimpleStringT__W_00_ATL__QEAAPEA_WXZ(CString*);
wchar_t* impl__GetBufferSetLength___CSimpleStringT__W_00_ATL__QEAAPEA_WH_Z(CString*,int);
void impl__ReleaseBuffer___CSimpleStringT__W_00_ATL__QEAAXH_Z(CString*,int);
void impl__ReleaseBufferSetLength___CSimpleStringT__W_00_ATL__QEAAXH_Z(CString*,int);
wchar_t* impl__LockBuffer___CSimpleStringT__W_00_ATL__QEAAPEA_WXZ(CString*);
void impl__UnlockBuffer___CSimpleStringT__W_00_ATL__QEAAXXZ(CString*);
}
namespace file_exception_test {
int failures;unsigned comparisons;
void check(bool good,const char* message){if(!good){++failures;printf("FAIL: %s\n",message);}}
struct API {
 decltype(&impl__GetErrorMessage_CFileException__UEBAHPEA_WIPEAI_Z) file;
 decltype(&impl__GetErrorMessage_CArchiveException__UEBAHPEA_WIPEAI_Z) archive;
 decltype(&impl__AfxThrowFileException__YAXHJPEB_W_Z) throwFile;
 decltype(&impl__AfxThrowArchiveException__YAXHPEB_W_Z) throwArchive;
 decltype(&impl__ErrnoToException_CFileException__SAHH_Z) errnoCause;
 decltype(&impl__ThrowErrno_CFileException__SAXHPEB_W_Z) throwErrno;
 decltype(&impl__AfxFormatStrings__YAXAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__PEB_WPEBQEB_WH_Z) format;
 decltype(&impl__AfxFormatStrings__YAXAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__IPEBQEB_WH_Z) formatID;
 decltype(&impl__LoadStringW___CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAAHI_Z) load;
 decltype(&impl__LoadStringW___CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAAHPEAUHINSTANCE____I_Z) loadModule;
 decltype(&impl__LoadStringW___CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAAHPEAUHINSTANCE____IG_Z) loadLang;
 decltype(&impl__GetBuffer___CSimpleStringT__W_00_ATL__QEAAPEA_WH_Z) buffer;
 decltype(&impl__GetBuffer___CSimpleStringT__W_00_ATL__QEAAPEA_WXZ) bufferDefault;
 decltype(&impl__GetBufferSetLength___CSimpleStringT__W_00_ATL__QEAAPEA_WH_Z) sizedBuffer;
 decltype(&impl__ReleaseBuffer___CSimpleStringT__W_00_ATL__QEAAXH_Z) release;
 decltype(&impl__ReleaseBufferSetLength___CSimpleStringT__W_00_ATL__QEAAXH_Z) releaseLength;
 decltype(&impl__LockBuffer___CSimpleStringT__W_00_ATL__QEAAPEA_WXZ) lock;
 decltype(&impl__UnlockBuffer___CSimpleStringT__W_00_ATL__QEAAXXZ) unlock;
 HMODULE image;
};
const API* api;
std::vector<int> events;std::map<UINT,std::wstring> strings;
std::array<WORD,1024> block;DWORD blockSize;UINT requested;WORD language;
int resourceFailure,finderFailure,mutation;CException* activeError;bool archiveKind;
HINSTANCE module=reinterpret_cast<HINSTANCE>(UINT_PTR(0x44));
void* finder(UINT id){events.push_back(1);requested=id;if(mutation==1&&id==0xf006){mutation=0;if(archiveKind)static_cast<CArchiveException*>(activeError)->m_cause=3;else static_cast<CFileException*>(activeError)->m_cause=3;}if(finderFailure==2)throw 71;return finderFailure==1||!strings.count(id)?nullptr:module;}
HRSRC WINAPI find(HMODULE image,const wchar_t* name,const wchar_t* type){events.push_back(2);check(image==module&&UINT_PTR(name)==WORD((requested>>4)+1)&&type==RT_STRING,"resource loaders preserve the explicit module and low-WORD STRING block ID");return resourceFailure==1?nullptr:reinterpret_cast<HRSRC>(UINT_PTR(0x55));}
HRSRC WINAPI findEx(HMODULE image,const wchar_t* type,const wchar_t* name,WORD lang){check(lang==language,"language-specific string loading forwards LANGID to FindResourceExW");return find(image,name,type);}
HGLOBAL WINAPI load(HMODULE image,HRSRC value){events.push_back(3);check(image==module&&UINT_PTR(value)==0x55,"string loader loads its located resource block");return resourceFailure==2?nullptr:reinterpret_cast<HGLOBAL>(UINT_PTR(0x66));}
void* WINAPI lock(HGLOBAL value){events.push_back(4);check(UINT_PTR(value)==0x66,"string loader locks the loaded resource");if(resourceFailure==3)return nullptr;block.fill(0);auto it=strings.find(requested);std::wstring text=it==strings.end()?L"":it->second;size_t at=requested&15;block[at++]=resourceFailure==5?0:WORD(text.size());for(wchar_t ch:text)block[at++]=WORD(ch);blockSize=DWORD((at+16)*2);return block.data();}
DWORD WINAPI size(HMODULE image,HRSRC value){events.push_back(5);return resourceFailure==4?0:blockSize;}
void** imported(HMODULE image,const char* wanted){auto* b=reinterpret_cast<BYTE*>(image);auto* pe=reinterpret_cast<IMAGE_NT_HEADERS*>(b+reinterpret_cast<IMAGE_DOS_HEADER*>(b)->e_lfanew);auto* d=reinterpret_cast<IMAGE_IMPORT_DESCRIPTOR*>(b+pe->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress);for(;d->Name;++d){auto* n=reinterpret_cast<IMAGE_THUNK_DATA*>(b+d->OriginalFirstThunk);auto* v=reinterpret_cast<IMAGE_THUNK_DATA*>(b+d->FirstThunk);for(;n->u1.AddressOfData;++n,++v)if(!IMAGE_SNAP_BY_ORDINAL(n->u1.Ordinal)){auto* name=reinterpret_cast<IMAGE_IMPORT_BY_NAME*>(b+n->u1.AddressOfData);if(!strcmp(reinterpret_cast<char*>(name->Name),wanted))return reinterpret_cast<void**>(&v->u1.Function);}}return nullptr;}
struct Patch{void** slot;void* old;DWORD protection;Patch(const char* name,void* target){slot=imported(api->image?api->image:GetModuleHandleW(nullptr),name);check(slot!=nullptr,name);if(!slot)abort();old=*slot;VirtualProtect(slot,8,PAGE_READWRITE,&protection);*slot=target;}~Patch(){*slot=old;DWORD ignored;VirtualProtect(slot,8,protection,&ignored);}};
struct Hook {
 BYTE* address;BYTE saved[12];DWORD protection;
 Hook(void* source,void* target):address(static_cast<BYTE*>(source)){memcpy(saved,address,12);VirtualProtect(address,12,PAGE_EXECUTE_READWRITE,&protection);BYTE code[12]={0x48,0xb8};memcpy(code+2,&target,8);code[10]=0xff;code[11]=0xe0;memcpy(address,code,12);FlushInstructionCache(GetCurrentProcess(),address,12);}
 ~Hook(){memcpy(address,saved,12);FlushInstructionCache(GetCurrentProcess(),address,12);DWORD ignored;VirtualProtect(address,12,protection,&ignored);}
};
void noexceptCases(){
 char path[MAX_PATH];GetModuleFileNameA(nullptr,path,MAX_PATH);
 for(int kind=0;kind<4;++kind){std::string command="\\\""+std::string(path)+"\\\" --file-noexcept "+(api->image?"native ":"source ")+std::to_string(kind);
  STARTUPINFOA start{sizeof(start)};PROCESS_INFORMATION process{};
  BOOL started=CreateProcessA(path,command.data(),nullptr,nullptr,FALSE,0,nullptr,nullptr,&start,&process);
  check(started!=FALSE,"start isolated resource-traits termination comparison");if(!started)continue;
  DWORD wait=WaitForSingleObject(process.hProcess,30000);if(wait!=WAIT_OBJECT_0)TerminateProcess(process.hProcess,75);
  DWORD code=0;GetExitCodeProcess(process.hProcess,&code);CloseHandle(process.hThread);CloseHandle(process.hProcess);
  check(wait==WAIT_OBJECT_0&&code==73,"resource-traits lookup exceptions terminate at the native no-throw boundary");
 }
}
CStringData* data(const CString& text){return reinterpret_cast<CStringData*>(const_cast<wchar_t*>(text.GetString()))-1;}
template<class F>int invoke(F function){try{function();return 0;}catch(CInvalidArgException*){return 22;}catch(CMemoryException*){return 12;}catch(COleException* error){LONG code=error->m_sc;error->Delete();return code==E_INVALIDARG?87:99;}catch(int value){return value;}catch(...){check(false,"framework exceptions preserve their native typed hierarchy");return -1;}}
struct Result {
 std::wstring text;int length,capacity,refs,result,caught;UINT help;std::vector<int> calls;
 bool operator==(const Result& other)const{return text==other.text&&length==other.length&&capacity==other.capacity&&refs==other.refs&&result==other.result&&caught==other.caught&&help==other.help&&calls==other.calls;}
};
std::vector<Result> golden;
void record(const Result& value){
 if(api->image) {
  bool good=comparisons<golden.size()&&value==golden[comparisons];
  if(!good && failures<4 && comparisons<golden.size()) {
   auto& expected=golden[comparisons];printf("mismatch %u result %d/%d caught %d/%d help %x/%x text [%ls]/[%ls] calls ",comparisons,value.result,expected.result,value.caught,expected.caught,value.help,expected.help,value.text.c_str(),expected.text.c_str());for(int v:value.calls)printf("%d",v);printf("/");for(int v:expected.calls)printf("%d",v);printf("\\n");
  }
  check(good,"native/source text, lengths, capacities, locks, help, resource calls and exceptions match");
 } else golden.push_back(value);++comparisons;
}
Result state(const CString& text,int result=0,int caught=0,UINT help=0){return {std::wstring(text.GetString(),text.GetLength()),text.GetLength(),data(text)->nAllocLength,data(text)->nRefs,result,caught,help,events};}
void resources(){
 strings.clear();strings[17]=std::wstring(L"AB\0CD",5);strings[0]=L"zero";strings[UINT_MAX]=L"last";strings[33]=std::wstring(400,L'z');
 for(UINT id:{0u,17u,33u,UINT_MAX})for(int failure=0;failure<6;++failure)for(int kind=0;kind<3;++kind)for(bool locked:{false,true}){
  CString text(L"existing");if(locked)api->lock(&text);requested=id;language=0x407;resourceFailure=failure;finderFailure=0;events.clear();
  int result=0,caught=invoke([&]{result=kind==0?api->load(&text,id):kind==1?api->loadModule(&text,module,id):api->loadLang(&text,module,id,language);});
  record(state(text,result,caught));
  if(!failure)check(result==TRUE&&text.GetLength()==int(strings[id].size())&&!memcmp(text.GetString(),strings[id].data(),strings[id].size()*2),"resource loading returns BOOL and copies full UTF16 data including embedded nulls and long entries");
  else check(!result&&text==L"existing","missing or malformed string images preserve their original destination");
  if(locked)api->unlock(&text);
 }
 resourceFailure=0;for(int mode:{1}){CString text(L"unchanged");events.clear();finderFailure=mode;int result=0,caught=invoke([&]{result=api->load(&text,17);});record(state(text,result,caught));check(text==L"unchanged"&&(mode==1?!result:caught==71),"current-module string misses and finder exceptions preserve destination storage");}
 finderFailure=0;
}
void formats(){
 auto one=api->image?reinterpret_cast<decltype(&impl__AfxFormatString1__YAXAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__IPEB_W_Z)>(reinterpret_cast<BYTE*>(api->image)+0x1cdf30):&impl__AfxFormatString1__YAXAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__IPEB_W_Z;
 auto two=api->image?reinterpret_cast<decltype(&impl__AfxFormatString2__YAXAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__IPEB_W1_Z)>(reinterpret_cast<BYTE*>(api->image)+0x1cdf50):&impl__AfxFormatString2__YAXAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__IPEB_W1_Z;

 const wchar_t* arguments[35];std::wstring owned[35];for(int i=0;i<35;++i){owned[i]=L"<"+std::to_wstring(i)+L">";arguments[i]=owned[i].c_str();}
 const wchar_t* patterns[]={L"",L"plain",L"%1/%9/%A/%Z",L"%0/%a/%%/%%1/%",L"[%2] [%A] [%Z]"};
 for(auto* format:patterns)for(int count:{-1,0,1,9,10,35})for(bool locked:{false,true})for(bool nullArgument:{false,true}){
  const wchar_t* saved=arguments[0];if(nullArgument)arguments[0]=nullptr;
  CString text(L"existing storage");if(locked)api->lock(&text);events.clear();int caught=invoke([&]{api->format(&text,format,arguments,count);});record(state(text,0,caught));if(locked)api->unlock(&text);arguments[0]=saved;
 }
 for(bool missingFormat:{false,true}){CString text(L"untouched");events.clear();int caught=invoke([&]{api->format(&text,missingFormat?nullptr:L"text",missingFormat?arguments:nullptr,0);});record(state(text,0,caught));check(caught==22&&text==L"untouched","null format or argument array raises invalid-argument before altering destination");}
 strings.clear();strings[17]=L"item %1 %A";resourceFailure=0;
 for(int mode:{0,1}){finderFailure=mode;CString text(L"preserved");events.clear();int caught=invoke([&]{api->formatID(&text,17,arguments,35);});record(state(text,0,caught));if(mode)check(text==L"preserved","resource-format misses and lookup exceptions preserve the original string");}finderFailure=0;
 strings[17]=L"[%1]/[%2]/[%A]";
 for(bool missing:{false,true})for(bool nullArgument:{false,true})for(bool second:{false,true}) {
  finderFailure=missing?1:0;CString text(L"preserved");events.clear();
  int caught=invoke([&]{if(second)two(&text,17,nullArgument?nullptr:L"one",L"two");else one(&text,17,nullArgument?nullptr:L"one");});
  record(state(text,0,caught));
 }
 finderFailure=0;
}
void buffers(){
 for(bool shared:{false,true})for(int minimum:{0,2,6,32})for(int locks:{0,1,2}){
  CString text(L"abcdef"),copy;if(shared)copy=text;for(int i=0;i<locks;++i)api->lock(&text);events.clear();
  int caught=invoke([&]{api->buffer(&text,minimum);});record(state(text,0,caught));
  check(data(text)->nRefs==(locks?-locks:1),"GetBuffer preserves lock depth without locking an exposed ordinary buffer");
  for(int i=0;i<locks;++i)api->unlock(&text);
 }
 for(int requested:{-2,-1,0,3,6,7}){CString text(L"abcdef");events.clear();int caught=invoke([&]{api->release(&text,requested);});record(state(text,0,caught));}
 for(int requested:{-2,-1,0,3,6,32}){CString text(L"abcdef");events.clear();int caught=invoke([&]{api->sizedBuffer(&text,requested);});auto result=state(text,0,caught);if(result.text.size()>6)result.text.resize(6);record(result);}
 for(int minimum:{-2,-1}){CString text(L"abc");events.clear();int caught=invoke([&]{api->buffer(&text,minimum);});record(state(text,0,caught));check(caught==87,"negative buffer requests raise the native OLE invalid-argument exception");}
 CString text(L"abc");api->lock(&text);api->lock(&text);auto* value=api->bufferDefault(&text);value[0]=0;events.clear();api->release(&text,-1);record(state(text));check(!text.GetLength()&&data(text)->nRefs==-2,"bounded ReleaseBuffer preserves nested locks");api->unlock(&text);api->unlock(&text);
}
void destroy(CException* object){using Function=void*(*)(void*,UINT);reinterpret_cast<Function>((*reinterpret_cast<void***>(object))[1])(object,0);}
void messages(){
 strings.clear();strings[0xf006]=L"unnamed";
 for(int cause:{-1,0,1,3,14,200}){strings[UINT(cause)+0xf1a0]=L"file [%1]";strings[UINT(cause)+0xf1b0]=L"archive [%1]";}
 for(bool archive:{false,true})for(int cause:{-1,0,1,14,200})for(const wchar_t* name:std::array<const wchar_t*,3>{nullptr,L"",L"file.txt"})for(UINT capacity:{1u,4u,64u})for(int missing:{0,1,2}){
  alignas(8) BYTE storage[48];memset(storage,0xa5,sizeof(storage));CException* error;
  if(archive)error=api->image?reinterpret_cast<CArchiveException*(*)(void*,int,const wchar_t*)>(reinterpret_cast<BYTE*>(api->image)+0xaa8e0)(storage,cause,name):new(storage)CArchiveException(cause,name);
  else error=api->image?reinterpret_cast<CFileException*(*)(void*,int,LONG,const wchar_t*)>(reinterpret_cast<BYTE*>(api->image)+0x21e6a8)(storage,cause,-1,name):new(storage)CFileException(cause,-1,name);
  activeError=error;archiveKind=archive;mutation=missing==2?1:0;finderFailure=missing==1?1:0;resourceFailure=0;events.clear();UINT help=91;wchar_t output[64];std::fill(std::begin(output),std::end(output),L'x');int result=0;
  int caught=invoke([&]{result=archive?api->archive(static_cast<CArchiveException*>(error),output,capacity,&help):api->file(static_cast<CFileException*>(error),output,capacity,&help);});
  Result value{std::wstring(output,capacity),0,0,0,result,caught,help,events};record(value);
  check(help==UINT(cause)+(archive?0xf1b0:0xf1a0),"message help selects the original native cause before resource callbacks");
  for(unsigned i=32;i<48;++i)check(storage[i]==0xa5,"file/archive objects preserve caller guards");
  for(unsigned i=12;i<16;++i)check(storage[i]==0xa5,"file/archive constructors preserve base tail padding");
  if(archive)for(unsigned i=20;i<24;++i)check(storage[i]==0xa5,"archive constructors preserve padding between cause and filename");
  destroy(error);
 }
 finderFailure=0;mutation=0;
 // The file and archive guards intentionally differ for zero-capacity output.
 CFileException file;CArchiveException archive;UINT help=91;wchar_t output[8];events.clear();check(!api->file(&file,output,0,&help)&&help==91&&events.empty(),"file zero-capacity output returns before help and resource lookup");help=91;check(!api->archive(&archive,nullptr,0,&help)&&help==91,"archive null output returns before help");
}
int checkedResult;
using MemCopy=int (__cdecl*)(wchar_t*,size_t,const wchar_t*,size_t);
using StringCopy=int (__cdecl*)(wchar_t*,size_t,const wchar_t*);
using BoundedCopy=int (__cdecl*)(wchar_t*,size_t,const wchar_t*,size_t);
MemCopy realMemoryCopy;StringCopy realStringCopy;BoundedCopy realBoundedCopy;
int __cdecl memoryCopy(wchar_t* out,size_t capacity,const wchar_t* source,size_t length){return checkedResult?checkedResult:realMemoryCopy(out,capacity,source,length);}
int __cdecl stringCopy(wchar_t* out,size_t capacity,const wchar_t* source){return checkedResult?checkedResult:realStringCopy(out,capacity,source);}
int __cdecl boundedCopy(wchar_t* out,size_t capacity,const wchar_t* source,size_t length){return checkedResult?checkedResult:realBoundedCopy(out,capacity,source,length);}
void checkedCopies(){
 strings.clear();strings[17]=L"resource";strings[0xf1a1]=L"file %1";strings[0xf1b1]=L"archive %1";resourceFailure=finderFailure=mutation=0;
 {
  Patch patch("wmemcpy_s",reinterpret_cast<void*>(&memoryCopy));realMemoryCopy=reinterpret_cast<MemCopy>(patch.old);
  for(int result:{0,80,12,22,34,7}){checkedResult=result;CString text(L"existing");requested=17;events.clear();int returned=0,caught=invoke([&]{returned=api->loadModule(&text,module,17);});record(state(text,returned,caught));check(caught==((!result||result==80)?0:result==12?12:22),"resource-copy failures retain native memory/invalid exception types");}
 }
 {
  Patch patch("wcscpy_s",reinterpret_cast<void*>(&stringCopy));realStringCopy=reinterpret_cast<StringCopy>(patch.old);
  for(int result:{0,80,12,22,34,7}){checkedResult=result;CString text;std::fill_n(text.GetBuffer(32),33,L'x');text.ReleaseBuffer(3);const wchar_t* argument=L"element";events.clear();int caught=invoke([&]{api->format(&text,L"X%1",&argument,1);});record(state(text,0,caught));check(caught==((!result||result==80)?0:result==12?12:22),"format checked-copy failures preserve partial storage and native exception types");}
 }
 {
  Patch patch("wcsncpy_s",reinterpret_cast<void*>(&boundedCopy));realBoundedCopy=reinterpret_cast<BoundedCopy>(patch.old);
  for(bool archive:{false,true})for(int result:{0,80,12,22,34,7}){
   alignas(8) BYTE storage[32];CException* error;
   if(archive)error=api->image?reinterpret_cast<CArchiveException*(*)(void*,int,const wchar_t*)>(reinterpret_cast<BYTE*>(api->image)+0xaa8e0)(storage,1,L"name"):new(storage)CArchiveException(1,L"name");
   else error=api->image?reinterpret_cast<CFileException*(*)(void*,int,LONG,const wchar_t*)>(reinterpret_cast<BYTE*>(api->image)+0x21e6a8)(storage,1,-1,L"name"):new(storage)CFileException(1,-1,L"name");
   checkedResult=result;events.clear();int returned=0;UINT help=0;wchar_t output[32];std::fill(std::begin(output),std::end(output),L'x');
   int caught=invoke([&]{returned=archive?api->archive(static_cast<CArchiveException*>(error),output,32,&help):api->file(static_cast<CFileException*>(error),output,32,&help);});
   record(Result{std::wstring(output,32),0,0,0,returned,caught,help,events});
   check(archive?caught==0&&returned==int(!result||result==80):caught==((!result||result==80)?0:result==12?12:22),"archive messages catch framework copy failures while file messages propagate them");
   destroy(error);
  }
 }
 checkedResult=0;
}

template<class T>void catchFactory(bool archive){
 try{if(archive)api->throwArchive(1,L"typed");else api->throwFile(1,9,L"typed");check(false,"factory never returns");}
 catch(T* pointer){auto* value=reinterpret_cast<CException*>(const_cast<void*>(static_cast<const void*>(pointer)));check(value!=nullptr,"ordinary factory produces an owned object");if(value)value->Delete();}
 catch(...){check(false,"file/archive factories preserve compiler-generated derived/base/void pointer catches");}
}
void exceptions(){
 catchFactory<CFileException>(false);catchFactory<CArchiveException>(true);
 for(bool archive:{false,true}){catchFactory<CObject>(archive);catchFactory<void>(archive);catchFactory<const CException>(archive);}
 for(int error=-100;error<300;++error){events.clear();int actual=api->errnoCause(error);record(Result{L"",0,0,0,actual,0,0,{}});}
 for(bool archive:{false,true})for(int cause:{-1,0,1,14,200})for(const wchar_t* name:std::array<const wchar_t*,2>{nullptr,L"name"}){
  try{if(archive)api->throwArchive(cause,name);else api->throwFile(cause,0x12345678,name);check(false,"file/archive factory always throws");}
  catch(CException* error){check(error&&error->m_bAutoDelete==TRUE&&error->GetRuntimeClass()->m_nObjectSize==32,"factories throw owned actual framework objects");if(error){check(error->GetRuntimeClass()->BaseClass()&&!strcmp(error->GetRuntimeClass()->BaseClass()->m_lpszClassName,"CException"),"file/archive runtime metadata preserves actual exception ancestry");
   if(archive){auto* e=static_cast<CArchiveException*>(error);check(e->m_cause==cause&&e->m_strFileName==(name?name:L""),"archive typed throw preserves cause and owned filename");}
   else{auto* e=static_cast<CFileException*>(error);check(e->m_cause==cause&&e->m_lOsError==0x12345678&&e->m_strFileName==(name?name:L""),"file typed throw preserves cause, OS error and owned filename");}error->Delete();}}
 }
 auto* target=api->image?reinterpret_cast<ULONG*(*)()>(GetProcAddress(GetModuleHandleW(L"ucrtbase.dll"),"__doserrno"))():__doserrno();ULONG saved=*target;*target=777;
 for(int error:{0,1,2,5,9,13,22,23,24,28,36,100}){bool caught=false;try{api->throwErrno(error,L"errno.txt");}catch(CFileException* value){caught=true;check(value->m_lOsError==777&&value->m_cause==api->errnoCause(error)&&value->m_strFileName==L"errno.txt","ThrowErrno uses DOS error storage and the native errno mapping");value->Delete();}check(caught==(error!=0),"ThrowErrno preserves zero-errno no-throw behavior");}*target=saved;
}
}
int TestFileExceptions(HMODULE image){using namespace file_exception_test;
 API functions{impl__GetErrorMessage_CFileException__UEBAHPEA_WIPEAI_Z,impl__GetErrorMessage_CArchiveException__UEBAHPEA_WIPEAI_Z,impl__AfxThrowFileException__YAXHJPEB_W_Z,impl__AfxThrowArchiveException__YAXHPEB_W_Z,impl__ErrnoToException_CFileException__SAHH_Z,impl__ThrowErrno_CFileException__SAXHPEB_W_Z,
 impl__AfxFormatStrings__YAXAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__PEB_WPEBQEB_WH_Z,impl__AfxFormatStrings__YAXAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__IPEBQEB_WH_Z,
 impl__LoadStringW___CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAAHI_Z,impl__LoadStringW___CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAAHPEAUHINSTANCE____I_Z,impl__LoadStringW___CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAAHPEAUHINSTANCE____IG_Z,
 impl__GetBuffer___CSimpleStringT__W_00_ATL__QEAAPEA_WH_Z,impl__GetBuffer___CSimpleStringT__W_00_ATL__QEAAPEA_WXZ,impl__GetBufferSetLength___CSimpleStringT__W_00_ATL__QEAAPEA_WH_Z,impl__ReleaseBuffer___CSimpleStringT__W_00_ATL__QEAAXH_Z,impl__ReleaseBufferSetLength___CSimpleStringT__W_00_ATL__QEAAXH_Z,impl__LockBuffer___CSimpleStringT__W_00_ATL__QEAAPEA_WXZ,impl__UnlockBuffer___CSimpleStringT__W_00_ATL__QEAAXXZ,image};
 if(image){auto* base=reinterpret_cast<BYTE*>(image);
#define RVA(member,value) functions.member=reinterpret_cast<decltype(functions.member)>(base+value)
 RVA(file,0x22cce0);RVA(archive,0x1d34a0);RVA(throwFile,0x22ce40);RVA(throwArchive,0x1d3610);RVA(errnoCause,0x22cea0);RVA(throwErrno,0x22cca0);RVA(format,0x1cdd30);RVA(formatID,0x1cdc90);RVA(load,0xdc20);RVA(loadModule,0xdb70);RVA(loadLang,0x2b36d0);RVA(buffer,0x3230);RVA(bufferDefault,0x2cd0);RVA(sizedBuffer,0x77850);RVA(release,0x777f0);RVA(releaseLength,0x3280);RVA(lock,0x203f60);RVA(unlock,0x2b4680);
#undef RVA
 }
 api=&functions;events.reserve(64);golden.reserve(2000);comparisons=0;g_fileExceptionFinder=&finder;
 {Patch a("FindResourceW",reinterpret_cast<void*>(&find)),b("FindResourceExW",reinterpret_cast<void*>(&findEx)),c("LoadResource",reinterpret_cast<void*>(&load)),d("LockResource",reinterpret_cast<void*>(&lock)),e("SizeofResource",reinterpret_cast<void*>(&size));printf("file resources\n");resources();printf("file formats\n");formats();printf("file buffers\n");buffers();printf("file messages\n");messages();printf("file throws\n");exceptions();checkedCopies();noexceptCases();}
 g_fileExceptionFinder=nullptr;printf("%s file/archive/string: %u native comparisons, %d failures\n",image?"Native":"Source",comparisons,failures);return failures;
}

int TestFileNoexceptChild(int argc,char** argv){
 using namespace file_exception_test;
 if(argc!=4||strcmp(argv[1],"--file-noexcept"))return 76;
 SetErrorMode(SEM_FAILCRITICALERRORS|SEM_NOGPFAULTERRORBOX);
 _set_abort_behavior(0,_WRITE_ABORT_MSG|_CALL_REPORTFAULT);
 std::set_terminate([]{ExitProcess(73);});
 int kind=atoi(argv[3]);bool native=!strcmp(argv[2],"native");HMODULE image=nullptr;
 if(native){const char* path=getenv("OPENMFC_NATIVE_EXCEPTION_DLL");image=path?LoadLibraryA(path):nullptr;if(!image)return 77;}
 g_fileExceptionFinder=[](UINT)->void*{throw 71;};
 Hook* patch=image?new Hook(reinterpret_cast<BYTE*>(image)+0x2aee00,reinterpret_cast<void*>(+[](UINT)->void*{throw 71;})):nullptr;
 CString value;const wchar_t* argument=L"argument";
 alignas(8) BYTE storage[32];CException* error=nullptr;
 if(kind==2)error=image?reinterpret_cast<CFileException*(*)(void*,int,LONG,const wchar_t*)>(reinterpret_cast<BYTE*>(image)+0x21e6a8)(storage,1,-1,nullptr):new(storage)CFileException;
 if(kind==3)error=image?reinterpret_cast<CArchiveException*(*)(void*,int,const wchar_t*)>(reinterpret_cast<BYTE*>(image)+0xaa8e0)(storage,1,nullptr):new(storage)CArchiveException;
 wchar_t output[32];UINT help;
 try {
  if(kind==0){if(image)reinterpret_cast<int(*)(CString*,UINT)>(reinterpret_cast<BYTE*>(image)+0xdc20)(&value,17);else value.LoadString(17);}
  if(kind==1){if(image)reinterpret_cast<decltype(&impl__AfxFormatStrings__YAXAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__IPEBQEB_WH_Z)>(reinterpret_cast<BYTE*>(image)+0x1cdc90)(&value,17,&argument,1);else AfxFormatStrings(value,UINT(17),&argument,1);}
  if(kind==2){if(image)reinterpret_cast<decltype(&impl__GetErrorMessage_CFileException__UEBAHPEA_WIPEAI_Z)>(reinterpret_cast<BYTE*>(image)+0x22cce0)(static_cast<CFileException*>(error),output,32,&help);else static_cast<CFileException*>(error)->GetErrorMessage(output,32,&help);}
  if(kind==3){if(image)reinterpret_cast<decltype(&impl__GetErrorMessage_CArchiveException__UEBAHPEA_WIPEAI_Z)>(reinterpret_cast<BYTE*>(image)+0x1d34a0)(static_cast<CArchiveException*>(error),output,32,&help);else static_cast<CArchiveException*>(error)->GetErrorMessage(output,32,&help);}
 }catch(...){ExitProcess(74);}
 delete patch;return 0;
}
