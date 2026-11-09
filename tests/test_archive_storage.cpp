// Native archive oracle: real production owners and ABI-compatible CFile callbacks.
#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxwin.h"
#include "archive_exports.h"
#include <array>
#include <vector>
#include <string>
#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cerrno>
extern "C" void* impl___2_YAPEAX_K_Z(size_t);
#define MEMBER(key,fn,symbol,rva) decltype(&fn) key;
struct API {ARCHIVE_API(MEMBER) HMODULE image;};
#undef MEMBER
API sourceAPI(){API a{};
#define ASSIGN(key,fn,symbol,rva) a.key=&fn;
 ARCHIVE_API(ASSIGN)
#undef ASSIGN
 return a;
}
API nativeAPI(HMODULE image){API a{};a.image=image;
#define ASSIGN(key,fn,symbol,rva) a.key=reinterpret_cast<decltype(a.key)>(reinterpret_cast<BYTE*>(image)+rva);
 ARCHIVE_API(ASSIGN)
#undef ASSIGN
 return a;
}
const API* api;int failures;unsigned cases;int expectedError=-1;
void check(bool good,const char* text){if(!good){++failures;printf("FAIL: %s\n",text);}}
template<class T>T field(const void* p,size_t offset){T v;memcpy(&v,static_cast<const BYTE*>(p)+offset,sizeof(v));return v;}
template<class T>void put(void* p,size_t offset,T value){memcpy(static_cast<BYTE*>(p)+offset,&value,sizeof(value));}
using Event=std::array<INT64,8>;std::vector<Event> events;
struct Block {void* data;size_t size;bool live;};std::vector<Block> blocks;
BYTE* active;BYTE* userBuffer;BYTE* fileBuffer;const BYTE* observedOutput;size_t observedSize;bool allocationNull,allocationThrows,trackAllocation=true;int failCallback,callbackNumber;
INT64 pointerID(const void* p){
 if(!p)return 0;auto v=uintptr_t(p);
 if(v==UINT_PTR(0xa5a5a5a5a5a5a5a5ULL))return -1;
 if(active&&v>=uintptr_t(active)&&v<uintptr_t(active)+144)return 100000+INT64(v-uintptr_t(active));
 if(userBuffer&&v>=uintptr_t(userBuffer)&&v<=uintptr_t(userBuffer)+4096)return 200000+INT64(v-uintptr_t(userBuffer));
 if(fileBuffer&&v>=uintptr_t(fileBuffer)&&v<=uintptr_t(fileBuffer)+8192)return 300000+INT64(v-uintptr_t(fileBuffer));
 for(size_t i=0;i<blocks.size();++i){auto base=uintptr_t(blocks[i].data);if(v>=base&&v<=base+blocks[i].size)return 400000+INT64(i*16384+v-base);}
 return 999999;
}
void event(int kind,INT64 one=0,INT64 two=0,INT64 three=0){
 events.push_back({kind,one,two,three,active?INT64(field<UINT>(active,32)):0,active?pointerID(field<void*>(active,56)):0,active?pointerID(field<void*>(active,64)):0,active?pointerID(field<void*>(active,72)):0});
 if(++callbackNumber==failCallback)throw 71;
}
void* allocate(size_t n){
 if(n<128 || !trackAllocation)return malloc(n);
 event(10,n);if(allocationThrows)AfxThrowMemoryException();if(allocationNull)return nullptr;
 void* p=malloc(n);memset(p,0xa5,n);blocks.push_back({p,n,true});return p;
}
using Free=void(__cdecl*)(void*);Free originalFree;
void __cdecl release(void* p){
 for(auto& b:blocks)if(b.data==p){event(11,pointerID(p));check(b.live,"archive releases an allocation once");b.live=false;return;}
 originalFree(p);
}
struct Hook {
 BYTE* address;BYTE saved[12];DWORD protection;
 Hook(void* target,void* replacement):address(static_cast<BYTE*>(target)){
  memcpy(saved,address,12);VirtualProtect(address,12,PAGE_EXECUTE_READWRITE,&protection);BYTE code[12]={0x48,0xb8};memcpy(code+2,&replacement,8);code[10]=0xff;code[11]=0xe0;memcpy(address,code,12);FlushInstructionCache(GetCurrentProcess(),address,12);
 }
 ~Hook(){memcpy(address,saved,12);FlushInstructionCache(GetCurrentProcess(),address,12);DWORD ignored;VirtualProtect(address,12,protection,&ignored);}
};
void** importSlot(HMODULE image,const char* wanted){
 BYTE* base=reinterpret_cast<BYTE*>(image);auto* nt=reinterpret_cast<IMAGE_NT_HEADERS*>(base+reinterpret_cast<IMAGE_DOS_HEADER*>(base)->e_lfanew);
 auto* dll=reinterpret_cast<IMAGE_IMPORT_DESCRIPTOR*>(base+nt->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress);
 for(;dll->Name;++dll){auto* name=reinterpret_cast<IMAGE_THUNK_DATA*>(base+dll->OriginalFirstThunk);auto* value=reinterpret_cast<IMAGE_THUNK_DATA*>(base+dll->FirstThunk);
  for(;name->u1.AddressOfData;++name,++value)if(!IMAGE_SNAP_BY_ORDINAL(name->u1.Ordinal)&&!strcmp(reinterpret_cast<char*>(reinterpret_cast<IMAGE_IMPORT_BY_NAME*>(base+name->u1.AddressOfData)->Name),wanted))return reinterpret_cast<void**>(&value->u1.Function);
 }return nullptr;
}
struct Patch {
 void** slot;void* old;DWORD protection;
 Patch(HMODULE image,const char* name,void* replacement):slot(importSlot(image,name)){check(slot!=nullptr,"locate archive CRT import");if(!slot)abort();old=*slot;VirtualProtect(slot,8,PAGE_READWRITE,&protection);*slot=replacement;}
 ~Patch(){*slot=old;DWORD ignored;VirtualProtect(slot,8,protection,&ignored);}
};
struct ProbeFile:CFile {
 std::array<BYTE,8192> bytes;UINT flags=0,chunk=UINT32_MAX;size_t position=0,length=1024;
 ProbeFile(){for(size_t i=0;i<bytes.size();++i)bytes[i]=BYTE(i*13+7);fileBuffer=bytes.data();}
 CString GetFilePath()const override {event(1);return CString(L"archive.dat");}
 UINT Read(void* output,UINT requested)override {
  event(2,requested,pointerID(output),position);UINT count=UINT((std::min)({size_t(requested),size_t(chunk),length-position}));
  if(count)memcpy(output,bytes.data()+position,count);position+=count;return count;
 }
 void Write(const void* source,UINT count)override {event(3,count,pointerID(source),position);if(count)memcpy(bytes.data()+position,source,count);position+=count;length=(std::max)(length,position);}
 ULONGLONG Seek(LONGLONG offset,UINT origin)override {event(4,offset,origin,position);position=size_t((origin==current?LONGLONG(position):origin==end?LONGLONG(length):0)+offset);return position;}
 void Flush()override {event(5);}
 UINT GetBufferPtr(UINT command,UINT count,void** start,void** end)override {
  event(6,command,count,INT64(pointerID(start)*1000000+pointerID(end)));
  if(command==bufferCheck)return flags;
  if(command==bufferCommit){position+=count;length=(std::max)(length,position);return 0;}
  UINT size=command==bufferRead?UINT((std::min)({size_t(count),size_t(chunk),length-position})):count;
  *start=bytes.data()+position;*end=bytes.data()+position+size;if(command==bufferRead)position+=size;return size;
 }
};
struct Storage {alignas(CArchive) BYTE bytes[144];Storage(){memset(bytes,0xa5,sizeof(bytes));}CArchive* get(){return reinterpret_cast<CArchive*>(bytes);}};
struct Result {
 std::vector<Event> calls;std::vector<UINT64> state;std::vector<std::string> buffers;std::wstring filename,errorName;int error=0;INT64 value=0;
 bool operator==(const Result& b)const{return calls==b.calls&&state==b.state&&buffers==b.buffers&&filename==b.filename&&errorName==b.errorName&&error==b.error&&value==b.value;}
};
std::vector<Result> golden;
void capture(Result& r,Storage& storage,ProbeFile& file,bool owned) {
 r.calls=events;
 for(size_t at:{0,24,48,56,64,72,88,96})r.state.push_back(at==24?0:at==48?(field<void*>(storage.bytes,at)==&file?1:!field<void*>(storage.bytes,at)?0:UINT64(-1)):UINT64(pointerID(field<void*>(storage.bytes,at))));
 for(size_t at:{8,12,16,20,32,36,40,44,80,84,104,108})r.state.push_back(field<UINT>(storage.bytes,at));
 if(owned)r.filename=storage.get()->m_strFileName.GetString();
 r.state.push_back(file.position);r.state.push_back(file.length);
 r.buffers.emplace_back(reinterpret_cast<char*>(file.bytes.data()),1024);
 r.buffers.emplace_back(reinterpret_cast<char*>(userBuffer),512);
 r.buffers.emplace_back(reinterpret_cast<char*>(storage.bytes+112),32);
 if(observedOutput)r.buffers.emplace_back(reinterpret_cast<const char*>(observedOutput),observedSize);
 for(auto b:blocks){r.state.push_back(b.live);r.buffers.emplace_back(static_cast<char*>(b.data),b.size);}
}
template<class F>void sample(Storage& storage,ProbeFile& file,bool& owned,F action){
 Result result;
 try{result.value=action();}
 catch(CArchiveException* e){result.error=100+e->m_cause;result.errorName=e->m_strFileName.GetString();e->Delete();}
 catch(CInvalidArgException* e){result.error=22;e->Delete();}
 catch(CMemoryException* e){result.error=12;e->Delete();}
 catch(int error){result.error=error;}
 if(expectedError>=0&&result.error!=expectedError)printf("Expected-error mismatch case%u: got%d expected%d\n",cases,result.error,expectedError);
 if(expectedError>=0)check(result.error==expectedError,"archive API preserves its documented success and error contract");
 capture(result,storage,file,owned);
 if(!api->image)golden.push_back(result);
 else {
  bool good=cases<golden.size()&&result==golden[cases];
  if(!good&&failures<12){const auto& old=golden[cases];printf("Archive mismatch%u error%d/%d result%lld/%lld calls%zu/%zu\n",cases,result.error,old.error,result.value,old.value,result.calls.size(),old.calls.size());for(auto x:result.calls)printf("N %lld %lld %lld %lld %lld %lld %lld %lld\n",x[0],x[1],x[2],x[3],x[4],x[5],x[6],x[7]);for(auto x:old.calls)printf("S %lld %lld %lld %lld %lld %lld %lld %lld\n",x[0],x[1],x[2],x[3],x[4],x[5],x[6],x[7]);for(size_t i=0;i<result.state.size()&&i<old.state.size();++i)if(result.state[i]!=old.state[i])printf("field%zu N%llx S%llx\n",i,result.state[i],old.state[i]);}
  check(good,"archive bytes, ownership, callbacks and exceptions match retail MFC");
 }++cases;
}
void reset(){expectedError=-1;observedOutput=nullptr;observedSize=0;events.clear();callbackNumber=0;failCallback=0;allocationNull=allocationThrows=false;trackAllocation=true;}
void finish(Storage& storage,bool owned){
 failCallback=0;if(owned){put<UINT>(storage.bytes,32,field<UINT>(storage.bytes,32)|2);api->dtor(storage.get());}
 active=nullptr;for(auto b:blocks)originalFree(b.data);blocks.clear();
}
void constructors(){
 for(UINT mode:std::array<UINT,6>{0,1,2,3,4,UINT32_MAX})for(int size:{-1,0,127,128,129,256})for(bool user:{false,true})for(UINT flags:{0,1,2,3})for(int failed:{0,1,2,3}) {
  ProbeFile file;std::array<BYTE,4096> buffer;buffer.fill(0xa5);userBuffer=buffer.data();file.flags=flags;Storage storage;active=storage.bytes;reset();failCallback=failed;expectedError=(failed==1 || failed==2 || (failed==3 && (!user || size<128)))?71:0;bool owned=false;
  sample(storage,file,owned,[&]()->INT64{auto* result=api->ctor(storage.bytes,&file,mode,size,user?buffer.data():nullptr);owned=true;return result==storage.get();});finish(storage,owned);
 }
 for(bool nil:{false,true})for(int allocation:{0,1,2}) {
  ProbeFile file;std::array<BYTE,4096> buffer;buffer.fill(0xa5);userBuffer=buffer.data();Storage storage;active=storage.bytes;reset();allocationNull=allocation==1;allocationThrows=allocation==2;expectedError=nil?22:allocation==2?12:0;bool owned=false;
  sample(storage,file,owned,[&]()->INT64{api->ctor(storage.bytes,nil?nullptr:&file,0,128,nullptr);owned=true;return 0;});finish(storage,owned);
 }
}
void operations(){
 for(UINT mode:{0,1,2,3})for(bool user:{false,true})for(UINT flags:{0,1,2,3})for(UINT chunk:std::array<UINT,2>{7,UINT32_MAX})for(int op:{0,1,2,3,4,5,6})for(UINT count:{0,1,127,128,129,257,513}) {
  ProbeFile file;std::array<BYTE,4096> buffer;buffer.fill(0xa5);userBuffer=buffer.data();file.flags=flags;file.chunk=chunk;Storage storage;active=storage.bytes;reset();bool owned=false;api->ctor(storage.bytes,&file,mode,128,user?buffer.data():nullptr);owned=true;
  std::array<BYTE,1024> output;output.fill(0xa5);observedOutput=output.data();observedSize=output.size();events.clear();callbackNumber=0;
  expectedError=0;
  if(op==0 && count && !(mode&1))expectedError=104;
  if(op==1 && count && (mode&1))expectedError=102;
  if(op==2){if(!(mode&1))expectedError=104;else if(count>128)expectedError=108;else if((flags&1)&&!user&&count>chunk)expectedError=103;}

  sample(storage,file,owned,[&]()->INT64 {
   if(op==0)return api->Read(storage.get(),count?output.data():nullptr,count);
   if(op==1){api->Write(storage.get(),output.data(),count);return 0;}
   if(op==2){api->FillBuffer(storage.get(),count);return 0;}
   if(op==3){api->Flush(storage.get());return 0;}
   if(op==4){api->Close(storage.get());return 0;}
   if(op==5){api->Abort(storage.get());return 0;}
   if(op==6){api->dtor(storage.get());owned=false;return 0;}
   return 0;
  });finish(storage,owned);
 }
}
void counts(){
 for(bool direct:{false,true})for(UINT mode:{0,1})for(ULONGLONG value:{0ull,1ull,65534ull,65535ull,0xfffffffeull,0xffffffffull,0xffffffffffffffffull})for(UINT initial:{0,120,126,127,128}) {
  ProbeFile file;std::array<BYTE,4096> buffer;buffer.fill(0xa5);userBuffer=buffer.data();file.flags=direct?1:0;Storage storage;active=storage.bytes;reset();bool owned=false;api->ctor(storage.bytes,&file,mode,128,nullptr);owned=true;
  std::array<BYTE,128> input{};
  if(mode) {
   BYTE* output=file.bytes.data()+initial;
   if(value<0xffff){WORD n=WORD(value);memcpy(output,&n,2);}
   else {WORD marker=0xffff;memcpy(output,&marker,2);output+=2;
    if(value<0xffffffff){DWORD n=DWORD(value);memcpy(output,&n,4);}
    else {DWORD marker=0xffffffff;memcpy(output,&marker,4);memcpy(output+4,&value,8);}}
   if(initial)api->Read(storage.get(),input.data(),initial);
  }
  if(!mode && initial)api->Write(storage.get(),input.data(),initial);
  events.clear();callbackNumber=0;
  sample(storage,file,owned,[&]()->INT64 {
   if(mode)return INT64(api->ReadCount(storage.get()));
   api->WriteCount(storage.get(),value);api->Flush(storage.get());return 0;
  });finish(storage,owned);
 }
 for(UINT schema:std::array<UINT,3>{0,1,0xffffffff})for(UINT count:std::array<UINT,5>{0,1,0x3ffffffd,0x3ffffffe,0xffffffff}) {
  ProbeFile file;std::array<BYTE,4096> buffer;buffer.fill(0xa5);userBuffer=buffer.data();Storage storage;active=storage.bytes;reset();bool owned=false;api->ctor(storage.bytes,&file,2,128,buffer.data());owned=true;put<UINT>(storage.bytes,20,schema);put<UINT>(storage.bytes,80,count);events.clear();expectedError=count>=0x3ffffffe?105:0;
  sample(storage,file,owned,[&]()->INT64{UINT result=api->GetObjectSchema(storage.get());api->CheckCount(storage.get());return result;});finish(storage,owned);
 }
}
void callbackFailures(){
 for(UINT mode:{0,1})for(bool user:{false,true})for(UINT flags:{0,1,2,3})for(int op:{0,1,2,3,4})for(int failed:{1,2,3,4}) {
  ProbeFile file;std::array<BYTE,4096> buffer;buffer.fill(0xa5);userBuffer=buffer.data();file.flags=flags;file.chunk=7;Storage storage;active=storage.bytes;reset();bool owned=false;api->ctor(storage.bytes,&file,mode,128,user?buffer.data():nullptr);owned=true;
  std::array<BYTE,1024> output;output.fill(0xa5);
  if(mode)api->Read(storage.get(),output.data(),1);else api->Write(storage.get(),output.data(),3);
  observedOutput=output.data();observedSize=output.size();events.clear();callbackNumber=0;failCallback=failed;
  sample(storage,file,owned,[&]()->INT64 {
   if(op==0)return api->Read(storage.get(),output.data(),257);
   if(op==1)api->Write(storage.get(),output.data(),257);
   if(op==2)api->FillBuffer(storage.get(),1);
   if(op==3)api->Flush(storage.get());
   if(op==4)api->Close(storage.get());
   return 0;
  });finish(storage,owned);
 }
}
struct TrackedObject:CObject {int tag;explicit TrackedObject(int id):tag(id){}~TrackedObject()override{event(14,tag);}};
struct TrackedMap:CMapPtrToPtr {int tag;explicit TrackedMap(int id):tag(id){}~TrackedMap()override{event(14,tag);}};
void abortMaps(){
 for(bool user:{false,true})for(int shape:{0,1,2,3}) {
  ProbeFile file;std::array<BYTE,4096> buffer;buffer.fill(0xa5);userBuffer=buffer.data();Storage storage;active=storage.bytes;reset();bool owned=false;api->ctor(storage.bytes,&file,2,128,user?buffer.data():nullptr);owned=true;
  trackAllocation=false;
  auto* schema=new TrackedMap(2);auto* objects=new TrackedMap(3);
  if(shape) schema->SetAt(reinterpret_cast<void*>(UINT_PTR(1)),shape==1?nullptr:new TrackedObject(1));
  if(shape==3)objects->SetAt(reinterpret_cast<void*>(UINT_PTR(7)),reinterpret_cast<void*>(UINT_PTR(9)));
  put<void*>(storage.bytes,96,schema);put<void*>(storage.bytes,88,objects);trackAllocation=true;events.clear();callbackNumber=0;
  sample(storage,file,owned,[&]()->INT64{api->Abort(storage.get());return 0;});finish(storage,owned);
 }
}
void run(){cases=0;failures=0;
 void* allocator=api->image?reinterpret_cast<BYTE*>(api->image)+0x27f0:reinterpret_cast<void*>(&impl___2_YAPEAX_K_Z);
 Hook allocation(allocator,reinterpret_cast<void*>(&allocate));Patch freeing(api->image?api->image:GetModuleHandleW(nullptr),"free",reinterpret_cast<void*>(&release));originalFree=reinterpret_cast<Free>(freeing.old);
 constructors();operations();counts();callbackFailures();abortMaps();printf("%s archives: %u comparisons, %d failures\n",api->image?"Native":"Source",cases,failures);
}
int main(){setvbuf(stdout,nullptr,_IONBF,0);auto source=sourceAPI();api=&source;run();int total=failures;
 if(auto* path=getenv("OPENMFC_NATIVE_ARCHIVE_DLL")){HMODULE image=LoadLibraryA(path);check(image!=nullptr,"load native archive oracle");if(!image)return 1;auto* bytes=reinterpret_cast<BYTE*>(image);auto* pe=reinterpret_cast<IMAGE_NT_HEADERS*>(bytes+reinterpret_cast<IMAGE_DOS_HEADER*>(bytes)->e_lfanew);if(pe->FileHeader.TimeDateStamp!=0x442cba30||pe->OptionalHeader.SizeOfImage!=0x55c000)return 1;auto native=nativeAPI(image);api=&native;run();total+=failures;}
 return total?1:0;
}
