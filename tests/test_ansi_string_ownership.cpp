// Actual string storage, VARIANT conversion and an independent native oracle.
#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxwin.h"
#include "openmfc/afxdisp.h"
#include <array>
#include <map>
#include <vector>
#include <string>
#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <exception>
#include <mbctype.h>
#include <mbstring.h>
#include <clocale>
#include <cerrno>
#include "ansi_string_exports.h"
#include "wide_string_storage_exports.h"
#include "string_operations_exports.h"
#include "string_search_exports.h"
extern void* (*g_fileExceptionFinder)(UINT);
namespace ansi_string_test {
int failures;unsigned comparisons;HMODULE image;
void check(bool good,const char* text){if(!good){++failures;printf("FAIL: %s\n",text);}}
std::map<UINT_PTR,void*> providers;
template<class R,class... A>R call(UINT_PTR rva,A... args){void* fn=image?static_cast<void*>(reinterpret_cast<BYTE*>(image)+rva):providers.at(rva);return reinterpret_cast<R(*)(A...)>(fn)(args...);}
using Event=std::array<INT64,6>;
std::vector<Event> events;
struct Block {CStringData* data;int bytes,id;bool live;};
std::vector<Block> blocks;
int blockID(CStringData* data){for(auto b:blocks)if(b.data==data)return b.id;return 0;}
CSimpleStringA* mutationTarget;int mutationStage;int mutationWidth=1;
void mutate(int stage){if(mutationTarget&&mutationStage==stage){auto* data=mutationTarget->GetData();data->nDataLength=3;if(mutationWidth==2)static_cast<wchar_t*>(data->data())[3]=0;else CSimpleStringA::Characters(data)[3]=0;}}
struct Manager: IAtlStringMgr {
 struct {CStringData header;char empty[8];} nil;
 int id;Manager* cloned;bool failAllocate=false,failReallocate=false;
 explicit Manager(int number):nil{{this,0,0,2},{}},id(number),cloned(this){}
 CStringData* make(int length,int width){
  int actual=length<0?0:length;int bytes=(actual+1)*width+32;
  auto* data=static_cast<CStringData*>(malloc(sizeof(CStringData)+bytes));
  *data={this,0,actual,1};memset(data+1,0xa5,bytes);blocks.push_back({data,bytes,int(blocks.size()+1),true});return data;
 }
 CStringData* Allocate(int length,int width) noexcept override {events.push_back({1,id,length,width,0,0});mutate(2);return failAllocate?nullptr:make(length,width);}
 void Free(CStringData* data) noexcept override {events.push_back({2,id,blockID(data),data->nDataLength,data->nAllocLength,data->nRefs});for(auto& b:blocks)if(b.data==data){check(b.live,"ANSI buffers are released exactly once");b.live=false;return;}check(false,"manager frees one of its own live buffers");}
 CStringData* Reallocate(CStringData* data,int length,int width) noexcept override {
  events.push_back({3,id,blockID(data),length,width,data->nRefs});mutate(3);if(failReallocate)return nullptr;
  auto* out=make(length,width);int bytes=0;for(auto& b:blocks)if(b.data==data){bytes=b.bytes-32;b.live=false;break;}
  memcpy(out+1,data+1,size_t((std::min)(bytes,((length<0?0:length)+1)*width)));out->nDataLength=data->nDataLength;out->nRefs=data->nRefs;return out;
 }
 CStringData* GetNilString() noexcept override {events.push_back({4,id,0,0,0,0});nil.header.AddRef();return &nil.header;}
 IAtlStringMgr* Clone() noexcept override {events.push_back({5,id,cloned?cloned->id:0,0,0,0});mutate(1);return cloned;}
};
struct Fixture {
 Manager first{1},second{2},third{3};
 Fixture(){check(blocks.empty(),"start ANSI ownership fixture without leftover allocations");}
 ~Fixture(){for(auto block:blocks)free(block.data);blocks.clear();}
};
struct Object {alignas(CStringA) BYTE storage[32];Object(){memset(storage,0xa5,sizeof(storage));}CSimpleStringA* get(){return reinterpret_cast<CSimpleStringA*>(storage);}CStringA* string(){return reinterpret_cast<CStringA*>(storage);}};
struct Result {
 LONG caught=0;INT64 value=0;std::vector<Event> calls,headers;std::vector<std::string> text;int objectID=0;bool guard=true;
 bool operator==(const Result& b)const{return caught==b.caught&&value==b.value&&calls==b.calls&&headers==b.headers&&text==b.text&&objectID==b.objectID&&guard==b.guard;}
};
std::vector<Result> golden;bool safeRead;int snapshotCharSize=1;
int managerID(IAtlStringMgr* manager,Fixture& fixture){return manager==&fixture.first?1:manager==&fixture.second?2:manager==&fixture.third?3:0;}
void snapshot(Result& result,Object& object,Fixture& fixture,bool valid){
 result.calls=events;
 for(auto* m:{&fixture.first,&fixture.second,&fixture.third})result.headers.push_back({0,m->id,m->nil.header.nDataLength,m->nil.header.nAllocLength,m->nil.header.nRefs,1});
 auto* pointer=*reinterpret_cast<char**>(object.storage);
 if(uintptr_t(pointer)==UINT_PTR(0xa5a5a5a5a5a5a5a5ULL))result.objectID=-1;
 else {
  auto* data=reinterpret_cast<CStringData*>(pointer)-1;result.objectID=blockID(data);
  for(auto* m:{&fixture.first,&fixture.second,&fixture.third})if(data==&m->nil.header)result.objectID=100+m->id;
  if(valid){result.headers.push_back({-1,managerID(data->pStringMgr,fixture),data->nDataLength,data->nAllocLength,result.objectID?data->nRefs:(data->nAllocLength?data->nRefs:0),1});result.text.emplace_back(pointer,size_t((std::min)(INT64(data->nDataLength+1)*snapshotCharSize,INT64(256))));}
 }
 for(auto block:blocks){auto* data=block.data;result.headers.push_back({block.id,managerID(data->pStringMgr,fixture),data->nDataLength,data->nAllocLength,data->nRefs,block.live});result.text.emplace_back(reinterpret_cast<char*>(data+1),size_t((std::min)(block.bytes-32,256)));}
 for(size_t at=8;at<32;++at)if(object.storage[at]!=0xa5)result.guard=false;
}
template<class F>void sample(Object& object,Fixture& fixture,bool valid,F fn){
 Result result;safeRead=valid;
 try{result.value=fn();}catch(CMemoryException*){result.caught=12;}catch(CInvalidArgException*){result.caught=22;}catch(COleException* error){result.caught=error->m_sc;error->Delete();}catch(CException* error){check(false,"string storage throws the expected native exception class");error->Delete();result.caught=-1;}
 snapshot(result,object,fixture,safeRead);
 if(image){bool good=comparisons<golden.size()&&result==golden[comparisons];if(!good&&failures<30){auto& old=golden[comparisons];printf("ANSI mismatch %u caught%x/%x value%lld/%lld object%d/%d calls ",comparisons,result.caught,old.caught,result.value,old.value,result.objectID,old.objectID);for(auto e:result.calls)printf("%lld:%lld:%lld,",e[0],e[1],e[2]);printf(" / ");for(auto e:old.calls)printf("%lld:%lld:%lld,",e[0],e[1],e[2]);printf("\n");}check(good,"ANSI ownership, byte contents, manager callbacks and failure states match native MFC");}
 else golden.push_back(result);++comparisons;
}
void construct(Object& object,Manager& manager,const char* text="abcdef",int length=6){call<void*>(0x273840,object.get(),text,length,&manager);}
void destroy(Object& object){call<void>(0x2fd0,object.get());}
void buffers(){
 for(int mode:{0,1,2,3})for(bool cloneOther:{false,true})for(int failure:{0,1,2})for(int operation=0;operation<15;++operation)for(int length:{-2,-1,0,1,6,7,12}) {
  Fixture f;Object object,sibling;construct(object,f.first);
  bool shared=mode==1;if(shared)call<void*>(0x2b6c00,sibling.get(),object.get());
  if(mode>=2){call<char*>(0x203f20,object.get());if(mode==3)call<char*>(0x203f20,object.get());}
  f.first.cloned=cloneOther?&f.second:&f.first;
  f.first.failAllocate=f.second.failAllocate=failure==1;f.first.failReallocate=failure==2;events.clear();
  sample(object,f,true,[&]()->INT64{
   switch(operation){
   case 0:call<char*>(0x1cd8d0,object.get(),length);break;
   case 1:call<char*>(0x27ea40,object.get());break;
   case 2:call<char*>(0x1fdf90,object.get(),length);break;
   case 3:call<void>(0x1fdf40,object.get(),length);break;
   case 4:call<void>(0x1cd920,object.get(),length);break;
   case 5:call<char*>(0x203f20,object.get());break;
   case 6:call<void>(0x2b4680,object.get());break;
   case 7:call<void>(0x1cd950,object.get());break;
   case 8:call<void>(0x2b6ed0,object.get());break;
   case 9:call<void>(0x2b6e60,object.get(),length);break;
   case 10:call<void>(0x2b6e00,object.get(),length);break;
   case 11:call<void>(0x2b6da0,object.get(),length,'Z');break;
   case 12:return call<char>(0x2b6ea0,object.get(),length);
   case 13:{auto* manager=call<IAtlStringMgr*>(0xd830,object.get());return managerID(manager,f);}
   case 14:call<void>(0x2b6fe0,object.get(),char(0));break;
   }return 0;
  });
  f.first.failAllocate=f.second.failAllocate=f.first.failReallocate=false;destroy(object);if(shared)destroy(sibling);
 }
}
void assignment(){
 for(int mode:{0,1,2})for(bool sameManager:{false,true})for(bool cloned:{false,true})for(int failure:{0,1,2})for(int operation=0;operation<8;++operation){
  Fixture f;Object object,source;construct(object,f.first);construct(source,sameManager?f.first:f.third,"AB\0DEF",6);
  if(mode==1)call<char*>(0x203f20,object.get());if(mode==2)call<char*>(0x203f20,source.get());
  f.first.cloned=cloned?&f.second:&f.first;f.third.cloned=cloned?&f.second:&f.third;
  f.first.failAllocate=f.second.failAllocate=f.third.failAllocate=failure==1;f.first.failReallocate=failure==2;events.clear();
  sample(object,f,true,[&]()->INT64{
   switch(operation){
   case 0:call<void*>(0x204180,object.get(),source.get());break;
   case 1:call<void*>(0x204180,object.get(),object.get());break;
   case 2:call<void>(0x203fa0,object.get(),source.get()->GetString(),6);break;
   case 3:call<void>(0x203fa0,object.get(),object.get()->GetString()+1,4);break;
   case 4:call<void>(0x2b7050,object.get(),source.get()->GetString(),6);break;
   case 5:call<void>(0x2b7050,object.get(),object.get()->GetString()+1,4);break;
   case 6:call<void>(0x2b4620,object.get(),&f.third);break;
   case 7:call<void>(0x2b6fd0,object.get(),source.get());break;
   }return 0;
  });
  f.first.failAllocate=f.second.failAllocate=f.third.failAllocate=f.first.failReallocate=false;destroy(object);destroy(source);
 }
}
void constructors(){
 for(int kind:{0,1,2,3,4})for(int length:{-2,-1,0,1,3,6})for(bool nullSource:{false,true})for(bool nullManager:{false,true})for(bool fail:{false,true}){
  Fixture f;Object object;bool made=false;f.first.failAllocate=fail;events.clear();
  sample(object,f,false,[&]()->INT64{
   const char* source=nullSource?nullptr:"A\0BCDE";IAtlStringMgr* manager=nullManager?nullptr:&f.first;
   if(kind==0)call<void*>(0x273840,object.get(),source,length,manager);
   if(kind==1)call<void*>(0x2b7270,object.get(),source,manager);
   if(kind==2)call<void*>(0x3000,object.get(),manager);
   if(kind==3)call<void*>(0x2b66d0,object.string(),source,length,manager);
   if(kind==4)call<void*>(0x2b6b60,object.string(),source,manager);
   made=true;safeRead=true;return object.get()->GetLength();
  });
  if(made)destroy(object);
 }
 for(bool locked:{false,true})for(bool cloned:{false,true})for(bool fail:{false,true}){
  Fixture f;Object source,object;construct(source,f.first);
  if(locked)call<char*>(0x203f20,source.get());f.first.cloned=cloned?&f.second:&f.first;f.first.failAllocate=f.second.failAllocate=fail;events.clear();bool made=false;
  sample(object,f,false,[&]()->INT64{call<void*>(0x2b6c00,object.get(),source.get());made=true;safeRead=true;return object.get()->GetLength();});
  if(made)destroy(object);destroy(source);
 }
}
void reentry(){
 for(int stage:{1,2,3})for(int operation:{0,1,2,3,4})for(bool shared:{false,true}){
  Fixture fixture;Object object,sibling;construct(object,fixture.first);if(shared)call<void*>(0x2b6c00,sibling.get(),object.get());
  if(operation==3)call<char*>(0x1cd8d0,object.get(),12);
  mutationTarget=object.get();mutationStage=stage;events.clear();bool copied=false;Object copy;
  sample(object,fixture,true,[&]()->INT64{
   if(operation==0)call<char*>(0x1cd8d0,object.get(),10);
   if(operation==1){call<void*>(0x2b6c00,copy.get(),object.get());copied=true;}
   if(operation==2)call<void>(0x2b6da0,object.get(),1,'X');
   if(operation==3)call<void>(0x2b6ed0,object.get());
   if(operation==4)call<void>(0x203fa0,object.get(),"123456789",9);
   return copied?copy.get()->GetLength():0;
  });
  mutationTarget=nullptr;mutationStage=0;if(copied)destroy(copy);destroy(object);if(shared)destroy(sibling);
 }
}

void extraStorage(){
 for(int length:{-1,0,3,6,12})for(bool failed:{false,true}){
  Fixture f;Object object;construct(object,f.first);f.first.failReallocate=failed;events.clear();
  sample(object,f,true,[&]()->INT64{call<void>(0x1cd810,object.get(),length);return 0;});
  f.first.failReallocate=false;destroy(object);
 }
 for(int length:{0,3,6})for(int mode:{0,1,2})for(bool failed:{false,true}){
  Fixture f;Object object,sibling;construct(object,f.first,"abcdef",length);call<char*>(0x1cd8d0,object.get(),24);
  if(mode==1)call<void*>(0x2b6c00,sibling.get(),object.get());if(mode==2)call<char*>(0x203f20,object.get());
  f.first.failAllocate=failed;events.clear();sample(object,f,true,[&]()->INT64{call<void>(0x2b6ed0,object.get());return 0;});
  f.first.failAllocate=false;destroy(object);if(mode==1)destroy(sibling);
 }
 for(int mode:{0,1,2}){
  Fixture f;Object object,sibling;construct(object,f.first);
  if(mode==1)call<void*>(0x2b6c00,sibling.get(),object.get());if(mode==2)call<char*>(0x203f20,object.get());
  events.clear();sample(object,f,true,[&]()->INT64{destroy(object);return 0;});if(mode==1)destroy(sibling);
 }
 if(!image){
  Fixture f;
  {CStringA one("abcdef",&f.first);one.LockBuffer();one.LockBuffer();
   {CStringA copy(one);check(copy.GetString()!=one.GetString()&&copy.GetLength()==6,"ordinary CStringA copies a locked buffer into separate storage");}
   one.GetBuffer();one.ReleaseBuffer();check(one.GetData()->nRefs==-2,"ordinary writable buffer access preserves nested lock depth");
   one.UnlockBuffer();one.UnlockBuffer();check(one.GetData()->nRefs==1,"ordinary nested unlock restores unique ownership");
   f.first.cloned=&f.second;CStringA copy(one);check(copy.GetData()->pStringMgr==&f.second,"ordinary CStringA copy uses the source manager's clone");}
  check(f.first.nil.header.nRefs==2&&f.second.nil.header.nRefs==2,"ordinary ANSI lifetimes release their nil references");
  for(auto block:blocks)check(!block.live,"ordinary ANSI lifetimes release every owned buffer");
 }
}
void** imported(HMODULE module,const char* wanted){
 auto* bytes=reinterpret_cast<BYTE*>(module);
 auto* pe=reinterpret_cast<IMAGE_NT_HEADERS*>(bytes+reinterpret_cast<IMAGE_DOS_HEADER*>(bytes)->e_lfanew);
 auto* dll=reinterpret_cast<IMAGE_IMPORT_DESCRIPTOR*>(bytes+pe->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress);
 for(;dll->Name;++dll){
  auto* names=reinterpret_cast<IMAGE_THUNK_DATA*>(bytes+dll->OriginalFirstThunk);
  auto* values=reinterpret_cast<IMAGE_THUNK_DATA*>(bytes+dll->FirstThunk);
  for(;names->u1.AddressOfData;++names,++values){
   bool matched=false;
   if(IMAGE_SNAP_BY_ORDINAL(names->u1.Ordinal)){
    if(!_stricmp(reinterpret_cast<char*>(bytes+dll->Name),"OLEAUT32.dll")){
     WORD ordinal=WORD(IMAGE_ORDINAL(names->u1.Ordinal));
     matched=(ordinal==8&&!strcmp(wanted,"VariantInit")) ||
       (ordinal==9&&!strcmp(wanted,"VariantClear")) ||
       (ordinal==12&&!strcmp(wanted,"VariantChangeType"));
    }
   }else{
    auto* name=reinterpret_cast<IMAGE_IMPORT_BY_NAME*>(bytes+names->u1.AddressOfData);
    matched=!strcmp(reinterpret_cast<char*>(name->Name),wanted);
   }
   if(matched)return reinterpret_cast<void**>(&values->u1.Function);
  }
 }
 return nullptr;
}
struct Patch {void** slot;void* old;DWORD protection;Patch(void** targetSlot,void* target):slot(targetSlot){check(slot!=nullptr,"locate ANSI allocator boundary");if(!slot)abort();old=*slot;VirtualProtect(slot,8,PAGE_READWRITE,&protection);*slot=target;}Patch(const char* name,void* target):Patch(imported(image?image:GetModuleHandleW(nullptr),name),target){}~Patch(){*slot=old;DWORD ignored;VirtualProtect(slot,8,protection,&ignored);}};
CStringData* defaultAllocate(IAtlStringMgr* manager,int length,int width){
 events.push_back({1,0,length,width,0,0});if(length<0)return nullptr;
 int bytes=(length+1)*width+32;auto* data=static_cast<CStringData*>(malloc(sizeof(CStringData)+bytes));
 *data={manager,0,length,1};memset(data+1,0xa5,bytes);blocks.push_back({data,bytes,int(blocks.size()+1),true});return data;
}
void defaultFree(IAtlStringMgr*,CStringData* data){events.push_back({2,0,blockID(data),data->nDataLength,data->nAllocLength,data->nRefs});for(auto& b:blocks)if(b.data==data){check(b.live,"default ANSI manager releases a live buffer");b.live=false;return;}check(false,"default ANSI manager frees its owned storage");}
CStringData* defaultReallocate(IAtlStringMgr* manager,CStringData* old,int length,int width){
 events.push_back({3,0,blockID(old),length,width,old->nRefs});if(length<0)return nullptr;
 int bytes=(length+1)*width+32;auto* data=static_cast<CStringData*>(malloc(sizeof(CStringData)+bytes));*data={manager,old->nDataLength,length,old->nRefs};memset(data+1,0xa5,bytes);
 for(auto& b:blocks)if(b.data==old){memcpy(data+1,old+1,size_t((std::min)(b.bytes-32,bytes-32)));b.live=false;break;}
 blocks.push_back({data,bytes,int(blocks.size()+1),true});return data;
}
std::array<WORD,1024> resource;std::wstring resourceText;UINT resourceID;int resourceFailure,conversionFailure;
using Converter=decltype(&::WideCharToMultiByte);Converter realConvert;
void* finder(UINT id){events.push_back({10,id,0,0,0,0});resourceID=id;return resourceFailure==1?nullptr:reinterpret_cast<void*>(UINT_PTR(0x44));}
HRSRC WINAPI find(HINSTANCE module,const wchar_t* name,const wchar_t* type){events.push_back({11,INT64(UINT_PTR(name)),0,0,0,0});check(UINT_PTR(module)==0x44&&type==RT_STRING,"ANSI string resource lookup preserves the chosen module/type");return resourceFailure==2?nullptr:reinterpret_cast<HRSRC>(UINT_PTR(0x55));}
HRSRC WINAPI findEx(HINSTANCE module,const wchar_t* type,const wchar_t* name,WORD language){events.push_back({12,language,0,0,0,0});return find(module,name,type);}
HGLOBAL WINAPI load(HINSTANCE module,HRSRC entry){events.push_back({13,0,0,0,0,0});return resourceFailure==3?nullptr:reinterpret_cast<HGLOBAL>(UINT_PTR(0x66));}
void* WINAPI lock(HGLOBAL value){events.push_back({14,0,0,0,0,0});resource.fill(0);size_t at=resourceID&15;resource[at++]=resourceFailure==4?0:WORD(resourceText.size());for(wchar_t ch:resourceText)resource[at++]=WORD(ch);return resourceFailure==5?nullptr:resource.data();}
DWORD WINAPI size(HINSTANCE module,HRSRC entry){events.push_back({15,0,0,0,0,0});return resourceFailure==6?0:DWORD(resource.size()*2);}
int WINAPI convert(UINT cp,DWORD flags,const wchar_t* text,int length,char* output,int capacity,const char* replacement,BOOL* used){
 events.push_back({16,cp,length,output!=nullptr,capacity,flags});check(!replacement&&!used,"ANSI conversion uses the thread ACP default replacement policy");
 if(!output&&conversionFailure==1)return 0;if(!output&&conversionFailure==2)return -1;if(output&&conversionFailure==3)return 0;
 return realConvert(cp,flags,text,length,output,capacity,replacement,used);
}
void conversions(){
 Patch conversion("WideCharToMultiByte",reinterpret_cast<void*>(&convert));realConvert=reinterpret_cast<Converter>(conversion.old);
 for(int kind:{0,1,2,3,4,5})for(int failure:{0,1,2,3})for(int count:{-1,0,1,4})for(bool nil:{false,true}){
  Fixture f;Object object;bool made=false;conversionFailure=failure;events.clear();
  sample(object,f,false,[&]()->INT64{
   const wchar_t* text=nil?nullptr:L"A\x3a9\0Z";
   if(kind==0)call<void*>(0x2b6ad0,object.string(),text,&f.first);
   if(kind==1)call<void*>(0x2b65c0,object.string(),text,count,&f.first);
   if(kind==2)call<void*>(0x273740,object.string(),text,count);
   if(kind==3)call<void*>(0x1cdad0,object.string(),text);
   if(kind==4)call<void*>(0x2b68d0,object.string(),char(nil?0:'Z'),count);
   if(kind==5)call<void*>(0x2b66f0,object.string(),wchar_t(nil?0:0x3a9),count);
   made=true;safeRead=true;return object.get()->GetLength();
  });if(made)destroy(object);
 }
 conversionFailure=0;
}
void managerExports(){
 for(int length:{-2,-1,0,1,6,17})for(int width:{1,2})for(int replacement:{-1,0,1,19}){
  Fixture fixture;Object object;events.clear();
  sample(object,fixture,false,[&]()->INT64{
   CAfxStringMgr manager;
   auto* nil=call<CStringData*>(0x273730,&manager);call<CStringData*>(0x273730,&manager);
   events.push_back({20,INT64(uintptr_t(nil)-uintptr_t(&manager)),nil->pStringMgr==&manager,nil->nDataLength,nil->nAllocLength,nil->nRefs});
   check(call<IAtlStringMgr*>(0x10130,&manager)==&manager,"manager Clone preserves its actual identity");
   nil->Release();nil->Release();
   auto* data=call<CStringData*>(0x2736a0,&manager,length,width);
   events.push_back({21,data!=nullptr,data?data->pStringMgr==&manager:0,data?data->nDataLength:0,data?data->nAllocLength:0,data?data->nRefs:0});
   if(data){
    auto* out=call<CStringData*>(0x2736f0,&manager,data,replacement,width);
    events.push_back({22,out!=nullptr,out?out->pStringMgr==&manager:0,out?out->nDataLength:data->nDataLength,out?out->nAllocLength:data->nAllocLength,out?out->nRefs:data->nRefs});
    call<void>(0x22b070,&manager,out?out:data);
   }
   return 0;
  });
 }
}

void resources(){
 auto previous=g_fileExceptionFinder;g_fileExceptionFinder=&finder;
 Patch a("FindResourceW",reinterpret_cast<void*>(&find)),b("FindResourceExW",reinterpret_cast<void*>(&findEx)),c("LoadResource",reinterpret_cast<void*>(&load)),d("LockResource",reinterpret_cast<void*>(&lock)),e("SizeofResource",reinterpret_cast<void*>(&size)),f("WideCharToMultiByte",reinterpret_cast<void*>(&convert));realConvert=reinterpret_cast<Converter>(f.old);
 for(int kind:{0,1,2,3,4})for(int failed=0;failed<7;++failed)for(int conversion:{0,1,2,3}){
  Fixture fixture;Object object;bool owned=true;construct(object,fixture.first,"old",3);resourceText.assign(L"ANSI \x3a9\0tail",11);resourceID=35;resourceFailure=failed;conversionFailure=conversion;events.clear();
  sample(object,fixture,true,[&]()->INT64{
   auto module=reinterpret_cast<HINSTANCE>(UINT_PTR(0x44));
   if(kind==0)return call<int>(0x1cd5d0,object.string(),module,resourceID);
   if(kind==1)return call<int>(0x2b48a0,object.string(),module,resourceID,WORD(0x409));
   if(kind==2)return call<int>(0x1cd6b0,object.string(),resourceID);
   destroy(object);owned=false;safeRead=false;
   if(kind==3)call<void*>(0x2b6b60,object.string(),reinterpret_cast<const char*>(UINT_PTR(resourceID)),&fixture.first);
   if(kind==4)call<void*>(0x2b6ad0,object.string(),reinterpret_cast<const wchar_t*>(UINT_PTR(resourceID)),&fixture.first);
   owned=true;safeRead=true;return 1;
  });
  if(owned)destroy(object);
 }
 resourceFailure=conversionFailure=0;g_fileExceptionFinder=previous;
}

void wideConstructors(){
 for(int kind:{0,1,2,3,4,5})for(int length:{-2,-1,0,1,3,6})for(bool nil:{false,true})for(bool missing:{false,true})for(bool fail:{false,true}){
  if(kind==5&&missing)continue;
  Fixture fixture;Object object;fixture.first.failAllocate=fail;bool owned=false;events.clear();
  sample(object,fixture,false,[&]()->INT64{
   const wchar_t* text=nil?nullptr:L"A\0BCDE";IAtlStringMgr* manager=missing?nullptr:&fixture.first;
   switch(kind){
   case 0:call<void*>(0x12980,object.storage,text,length,manager);break;
   case 1:call<void*>(0x2b47c0,object.storage,text,manager);break;
   case 2:call<void*>(0x3000,object.storage,manager);break;
   case 3:call<void*>(0x12a60,object.storage,text,length,manager);break;
   case 4:call<void*>(0xd790,object.storage,text,manager);break;
   case 5:call<void*>(0x12bc0,object.storage,manager);break;
   }owned=true;safeRead=true;return object.get()->GetLength();
  });
  if(owned)reinterpret_cast<CString*>(object.storage)->~CString();
 }
 for(int state:{0,1,2})for(bool clone:{false,true})for(bool fail:{false,true})for(int length:{0,6}){
  Fixture fixture;Object object,source;auto* text=::new(source.storage) CString(L"abcdef",length,&fixture.first);
  if(state){text->LockBuffer();if(state==2)text->LockBuffer();}
  fixture.first.cloned=clone?&fixture.second:&fixture.first;fixture.first.failAllocate=fixture.second.failAllocate=fail;bool owned=false;events.clear();
  sample(object,fixture,false,[&]()->INT64{call<void*>(0xd740,object.storage,text);owned=true;safeRead=true;return object.get()->GetLength();});
  if(owned)reinterpret_cast<CString*>(object.storage)->~CString();text->~CString();
 }
 for(int stage:{1,2})for(bool locked:{false,true}){
  Fixture fixture;Object object,source;auto* text=::new(source.storage) CString(L"abcdef",6,&fixture.first);
  if(locked)text->LockBuffer();fixture.first.cloned=&fixture.second;mutationTarget=source.get();mutationStage=stage;mutationWidth=2;events.clear();bool owned=false;
  sample(object,fixture,false,[&]()->INT64{call<void*>(0xd740,object.storage,text);owned=true;safeRead=true;return object.get()->GetLength();});
  mutationTarget=nullptr;mutationWidth=1;if(owned)reinterpret_cast<CString*>(object.storage)->~CString();text->~CString();
 }
 {Fixture fixture;{
   CString source(L"abcdef",6,&fixture.first);source.LockBuffer();source.LockBuffer();
   CString copy(source);check(copy.GetString()!=source.GetString()&&copy==L"abcdef","ordinary wide C++ copies own separate storage for locked sources");
   copy.SetAt(0,L'Z');check(source==L"abcdef"&&copy==L"Zbcdef","changing a wide locked copy preserves its source");
  }for(auto block:blocks)check(!block.live,"ordinary wide C++ destruction releases every owned buffer");}
}
using WideConverter=decltype(&::MultiByteToWideChar);WideConverter realWideConvert;int wideConversionFailure;
int WINAPI wideConvert(UINT cp,DWORD flags,const char* text,int length,wchar_t* output,int capacity){
 events.push_back({17,cp,flags,length,capacity,output!=nullptr});
 if(!output&&wideConversionFailure==1)return 0;if(!output&&wideConversionFailure==2)return -1;if(output&&wideConversionFailure==3)return 0;
 return realWideConvert(cp,flags,text,length,output,capacity);
}
void wideConversions(){
 Patch conversion("MultiByteToWideChar",reinterpret_cast<void*>(&wideConvert));realWideConvert=reinterpret_cast<WideConverter>(conversion.old);
 for(int kind=0;kind<9;++kind)for(int failure:{0,1,2,3})for(int count:{-1,0,1,4})for(bool nil:{false,true}){
  Fixture fixture;Object object;bool owned=kind==8;if(owned)::new(object.storage) CString(L"old",3,&fixture.first);
  wideConversionFailure=failure;events.clear();
  sample(object,fixture,owned,[&]()->INT64{
   const char* text=nil?nullptr:"A\xe9\0Z";
   switch(kind){
   case 0:call<void*>(0x264be0,object.storage,text,&fixture.first);break;
   case 1:call<void*>(0x2b4380,object.storage,text,count,&fixture.first);break;
   case 2:call<void*>(0x1cd550,object.storage,text);break;
   case 3:call<void*>(0x1c0e0,object.storage,text,count);break;
   case 4:call<void*>(0x2b4500,object.storage,reinterpret_cast<const unsigned char*>(text),&fixture.first);break;
   case 5:call<void*>(0x2b4560,object.storage,reinterpret_cast<const unsigned char*>(text));break;
   case 6:call<void*>(0x7c260,object.storage,wchar_t(nil?0:0x3a9),count);break;
   case 7:call<void*>(0x2b4470,object.storage,char(nil?0:0xe9),count);break;
   case 8:check(call<void*>(0x1cd480,object.storage,text)==object.storage,"ANSI-to-Unicode assignment returns its receiver");break;
   }owned=true;safeRead=true;return object.get()->GetLength();
  });
  if(owned)reinterpret_cast<CString*>(object.storage)->~CString();
 }
 wideConversionFailure=0;
}
void wideConstructorResources(){
 auto previous=g_fileExceptionFinder;g_fileExceptionFinder=&finder;
 Patch a("FindResourceW",reinterpret_cast<void*>(&find)),b("LoadResource",reinterpret_cast<void*>(&load)),c("LockResource",reinterpret_cast<void*>(&lock)),d("SizeofResource",reinterpret_cast<void*>(&size));
 for(int kind:{0,1,2,3,4,5})for(int failure=0;failure<7;++failure)for(bool fail:{false,true}){
  Fixture fixture;Object object;fixture.first.failAllocate=fail;bool owned=false;resourceText.assign(L"Wide \x3a9\0tail",11);resourceID=35;resourceFailure=failure;events.clear();
  sample(object,fixture,false,[&]()->INT64{
   if(kind==0)call<void*>(0xd790,object.storage,reinterpret_cast<const wchar_t*>(UINT_PTR(resourceID)),&fixture.first);
   if(kind==1)call<void*>(0x264be0,object.storage,reinterpret_cast<const char*>(UINT_PTR(resourceID)),&fixture.first);
   if(kind==2)call<void*>(0xdcb0,object.storage,reinterpret_cast<const wchar_t*>(UINT_PTR(resourceID)));
   if(kind==3)call<void*>(0x1cd550,object.storage,reinterpret_cast<const char*>(UINT_PTR(resourceID)));
   if(kind==4){auto* text=reinterpret_cast<unsigned char*>(UINT_PTR(resourceID));if(image)call<void*>(0x1cd550,object.storage,text);else impl___0__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAA_PEAE_Z(reinterpret_cast<CString*>(object.storage),text);}
   if(kind==5){auto* text=reinterpret_cast<unsigned char*>(UINT_PTR(resourceID));if(image)call<void*>(0x2b69d0,object.storage,text);else impl___0__CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL__QEAA_PEAE_Z(object.string(),text);}
   owned=true;safeRead=true;return object.get()->GetLength();
  });
  if(owned)reinterpret_cast<CString*>(object.storage)->~CString();
 }
 resourceFailure=0;g_fileExceptionFinder=previous;
}

void unsignedConstructors(){
 for(bool wide:{false,true})for(bool manager:{false,true})for(bool nil:{false,true})for(bool missing:{false,true})for(bool fail:{false,true}){
  if(!manager&&(missing||fail))continue;
  Fixture fixture;Object object;bool owned=false;fixture.first.failAllocate=fail;events.clear();
  sample(object,fixture,false,[&]()->INT64{
   auto* text=nil?nullptr:reinterpret_cast<const unsigned char*>("A\0tail");
   if(manager)call<void*>(wide?0x2b4500:0x2b6960,object.storage,text,missing?nullptr:&fixture.first);
   else call<void*>(wide?0x2b4560:0x2b6a60,object.storage,text);
   owned=true;safeRead=true;return object.get()->GetLength();
  });
  if(owned){if(wide)reinterpret_cast<CString*>(object.storage)->~CString();else destroy(object);}
 }
}
void wideAssignments(){
 for(int mode:{0,1,2})for(bool same:{false,true})for(bool clone:{false,true})for(int failure:{0,1,2})for(int operation=0;operation<8;++operation){
  Fixture fixture;Object object,source;auto* text=::new(object.storage) CString(L"abcdef",6,&fixture.first);
  auto* input=::new(source.storage) CString(L"AB\0DEF",6,same?&fixture.first:&fixture.third);
  if(mode==1)text->LockBuffer();if(mode==2)input->LockBuffer();
  fixture.first.cloned=clone?&fixture.second:&fixture.first;fixture.third.cloned=clone?&fixture.second:&fixture.third;
  fixture.first.failAllocate=fixture.second.failAllocate=fixture.third.failAllocate=failure==1;fixture.first.failReallocate=failure==2;events.clear();
  sample(object,fixture,true,[&]()->INT64{
   void* result=nullptr;
   switch(operation){
   case 0:result=call<void*>(0xde30,text,input);break;
   case 1:result=call<void*>(0xdec0,text,input);break;
   case 2:result=call<void*>(0xdec0,text,text);break;
   case 3:result=call<void*>(0x2f90,text,input->GetString());break;
   case 4:result=call<void*>(0x2f90,text,text->GetString()+1);break;
   case 5:result=call<void*>(0x2b4310,text,char(mode?0:0xe9));break;
   case 6:result=call<void*>(0x2b4330,text,wchar_t(mode?0:0x3a9));break;
   case 7:result=call<void*>(0x2b4370,text,reinterpret_cast<const unsigned char*>("ansi"));break;
   }check(result==text,"wide assignments return their actual receiver");return text->GetLength();
  });
  fixture.first.failAllocate=fixture.second.failAllocate=fixture.third.failAllocate=fixture.first.failReallocate=false;
  text->~CString();input->~CString();
 }
 for(int state:{0,1,2,3}){
  Fixture fixture;Object object,sibling;auto* text=::new(object.storage) CString(L"abcdef",6,&fixture.first);
  if(state==1)::new(sibling.storage) CString(*text);
  if(state>=2){text->LockBuffer();if(state==3)text->LockBuffer();}
  events.clear();sample(object,fixture,true,[&]()->INT64{call<void>(0x2fd0,text);return 0;});
  if(state==1)reinterpret_cast<CString*>(sibling.storage)->~CString();
 }
}
using WideMemoryCopy=decltype(&::wmemcpy_s);WideMemoryCopy originalWideCopy;int wideCopyResult;
int __cdecl wideCopy(wchar_t* out,size_t capacity,const wchar_t* text,size_t count){
 events.push_back({18,INT64(capacity),INT64(count),text[0],text[1],0});
 return wideCopyResult?wideCopyResult:originalWideCopy(out,capacity,text,count);
}
void wideRepeatCopies(){
 Patch copy("wmemcpy_s",reinterpret_cast<void*>(&wideCopy));originalWideCopy=reinterpret_cast<WideMemoryCopy>(copy.old);
 for(int result:{0,80,12,22,34,7})for(int count:{-1,0,1,4})for(wchar_t value:std::array<wchar_t,2>{0,0x3a9}){
  Fixture fixture;Object object;bool owned=false;wideCopyResult=result;events.clear();
  sample(object,fixture,false,[&]()->INT64{call<void*>(0x7c260,object.storage,value,count);owned=true;safeRead=true;return object.get()->GetLength();});
  if(owned)reinterpret_cast<CString*>(object.storage)->~CString();
 }
 wideCopyResult=0;
}

void wideAppend(){
 for(int state:{0,1,2,3})for(bool clone:{false,true})for(int failure:{0,1,2})for(int operation=0;operation<7;++operation)for(int maximum:{-1,0,1,4,10}){
  if(operation>=3&&maximum!=0)continue;
  Fixture fixture;Object object,sibling,other;
  auto* text=::new(object.storage) CString(&fixture.first);*text=L"abcdef";
  auto* source=::new(other.storage) CString(&fixture.third);*source=L"ABcdef";source->GetBuffer()[2]=0;
  if(state==1)::new(sibling.storage) CString(*text);
  if(state>=2){text->LockBuffer();if(state==3)text->LockBuffer();}
  fixture.first.cloned=clone?&fixture.second:&fixture.first;
  fixture.first.failAllocate=fixture.second.failAllocate=failure==1;fixture.first.failReallocate=failure==2;events.clear();
  sample(object,fixture,true,[&]()->INT64{
   switch(operation){
   case 0:call<void>(0x2ba0,text,L"AB\0tail",maximum);break;
   case 1:call<void>(0x2ba0,text,text->GetString()+2,maximum);break;
   case 2:call<void>(0x2ba0,text,static_cast<const wchar_t*>(nullptr),maximum);break;
   case 3:call<void>(0x122f0,text,L"AB\0tail");break;
   case 4:call<void>(0x122f0,text,text->GetString()+2);break;
   case 5:call<void>(0x2ca0,text,text);break;
   case 6:call<void>(0x2ca0,text,source);break;
   }return text->GetLength();
  });
  fixture.first.failAllocate=fixture.second.failAllocate=fixture.first.failReallocate=false;
  text->~CString();source->~CString();if(state==1)reinterpret_cast<CString*>(sibling.storage)->~CString();
 }
}

template<class F,class... A>auto exactExport(UINT_PTR rva,F source,A... args)->decltype(source(args...)) {
 return image?reinterpret_cast<F>(reinterpret_cast<BYTE*>(image)+rva)(args...):source(args...);
}
CSimpleStringW* wide(Object& object){return reinterpret_cast<CSimpleStringW*>(object.storage);}
void constructWide(Object& object,Manager& manager,const wchar_t* text=L"abcdef",int length=6){call<void*>(0x12980,wide(object),text,length,&manager);}
void wideBuffers(){
 for(int mode:{0,1,2,3})for(bool cloneOther:{false,true})for(int failure:{0,1,2})for(int operation=0;operation<20;++operation)for(int length:{-2,-1,0,1,6,7,12}) {
  Fixture f;Object object,sibling;constructWide(object,f.first);auto* text=wide(object);
  bool shared=mode==1;if(shared)call<void*>(0xd740,wide(sibling),text);
  if(mode>=2){call<wchar_t*>(0x203f60,text);if(mode==3)call<wchar_t*>(0x203f60,text);}
  f.first.cloned=cloneOther?&f.second:&f.first;
  f.first.failAllocate=f.second.failAllocate=failure==1;f.first.failReallocate=failure==2;events.clear();
  sample(object,f,true,[&]()->INT64{
   switch(operation){
   case 0:{auto* out=call<wchar_t*>(0x3230,text,length);return out==text->GetString();}
   case 1:{auto* out=call<wchar_t*>(0x2cd0,text);return out==text->GetString();}
   case 2:{auto* out=call<wchar_t*>(0x77850,text,length);return out==text->GetString();}
   case 3:call<void>(0x777f0,text,length);break;
   case 4:call<void>(0x3280,text,length);break;
   case 5:{auto* out=call<wchar_t*>(0x203f60,text);return out==text->GetString();}
   case 6:call<void>(0x2b4680,text);break;
   case 7:call<void>(0x33b0,text);break;
   case 8:call<void>(0x2b46b0,text);break;
   case 9:call<void>(0x133ba0,text,length);break;
   case 10:call<void>(0x126f0,text,length);break;
   case 11:call<void>(0x2d00,text,length,wchar_t(0x3a9));break;
   case 12:return exactExport(0x2d70,&impl__GetAt___CSimpleStringT__W_00_ATL__QEBA_WH_Z,text,length);
   case 13:return managerID(call<IAtlStringMgr*>(0xd830,text),f);
   case 14:call<void>(0x32b0,text,wchar_t(0));break;
   case 15:return exactExport(0x2d70,&impl___A__CSimpleStringT__W_00_ATL__QEBA_WH_Z,text,length);
   case 16:{auto* out=exactExport(0x3230,&impl__PrepareWrite___CSimpleStringT__W_00_ATL__AEAAPEA_WH_Z,text,length);return out==text->GetString();}
   case 17:call<void>(0x31c0,text,length);break;
   case 18:exactExport(0x3280,&impl__SetLength___CSimpleStringT__W_00_ATL__AEAAXH_Z,text,length);break;
   case 19:return call<int>(0x3080,text)*100+call<int>(0x2e20,text)*10+call<bool>(0x9720,text);
   }return 0;
  });
  f.first.failAllocate=f.second.failAllocate=f.first.failReallocate=false;destroy(object);if(shared)destroy(sibling);
 }
}
void wideStorageAssignments(){
 for(int mode:{0,1,2})for(bool cloneOther:{false,true})for(int failure:{0,1,2})for(int operation=0;operation<12;++operation){
  Fixture f;Object object,source;constructWide(object,f.first);constructWide(source,f.third,L"AB\0DEF");auto* text=wide(object);
  if(mode==1)text->LockBuffer();if(mode==2)wide(source)->LockBuffer();
  f.first.cloned=cloneOther?&f.second:&f.first;
  f.first.failAllocate=f.second.failAllocate=failure==1;f.first.failReallocate=failure==2;events.clear();
  sample(object,f,true,[&]()->INT64{
   switch(operation){
   case 0:call<void>(0x2e30,text,wide(source)->GetString(),6);break;
   case 1:call<void>(0x2e30,text,text->GetString()+1,4);break;
   case 2:call<void>(0x2e30,text,static_cast<const wchar_t*>(nullptr),0);break;
   case 3:call<void>(0x2e30,text,static_cast<const wchar_t*>(nullptr),1);break;
   case 4:call<void>(0x2e30,text,L"abc",-1);break;
   case 5:call<void>(0x2f50,text,wide(source)->GetString());break;
   case 6:call<void>(0x2f50,text,static_cast<const wchar_t*>(nullptr));break;
   case 7:call<void>(0x2b4620,text,&f.third);break;
   case 8:return call<CSimpleStringW*>(0x2cb0,text,wide(source))==text;
   case 9:return call<CSimpleStringW*>(0x2cb0,text,text)==text;
   case 10:return call<CSimpleStringW*>(0x12330,text,text->GetString()+1)==text;
   case 11:return call<CSimpleStringW*>(0x12330,text,static_cast<const wchar_t*>(nullptr))==text;
   }return 0;
  });
  f.first.failAllocate=f.second.failAllocate=f.first.failReallocate=false;destroy(object);destroy(source);
 }
 // The base += character overloads widen values; they do not use the ACP.
 for(int mode:{0,1,2})for(bool cloneOther:{false,true})for(int failure:{0,1,2})for(int operation:{0,1,2})for(unsigned value:{0,65,128,233,255,0x3a9}){
  Fixture f;Object object,sibling;constructWide(object,f.first);auto* text=wide(object);
  if(mode==1)call<void*>(0xd740,wide(sibling),text);if(mode==2)text->LockBuffer();
  f.first.cloned=cloneOther?&f.second:&f.first;
  f.first.failAllocate=f.second.failAllocate=failure==1;f.first.failReallocate=failure==2;events.clear();
  sample(object,f,true,[&]()->INT64{
   if(operation==0)return call<CSimpleStringW*>(0x3320,text,char(value))==text;
   if(operation==1)return call<CSimpleStringW*>(0x2b42f0,text,static_cast<unsigned char>(value))==text;
   return call<CSimpleStringW*>(0x122d0,text,wchar_t(value))==text;
  });
  f.first.failAllocate=f.second.failAllocate=f.first.failReallocate=false;destroy(object);if(mode==1)destroy(sibling);
 }
}
void wideStorageReentry(){
 mutationWidth=2;
 for(int stage:{1,2,3})for(int operation:{0,1,2,3,4})for(bool shared:{false,true}){
  Fixture f;Object object,sibling,copy;constructWide(object,f.first);auto* text=wide(object);
  if(shared)call<void*>(0xd740,wide(sibling),text);if(operation==3)call<wchar_t*>(0x3230,text,12);
  mutationTarget=object.get();mutationStage=stage;events.clear();bool copied=false;
  sample(object,f,true,[&]()->INT64{
   if(operation==0)call<wchar_t*>(0x3230,text,10);
   if(operation==1){call<void*>(0xd740,wide(copy),text);copied=true;}
   if(operation==2)call<void>(0x2d00,text,1,wchar_t(0x3a9));
   if(operation==3)call<void>(0x2b46b0,text);
   if(operation==4)call<void>(0x2e30,text,L"123456789",9);
   return copied?wide(copy)->GetLength():0;
  });
  mutationTarget=nullptr;mutationStage=0;if(copied)destroy(copy);destroy(object);if(shared)destroy(sibling);
 }
 mutationWidth=1;
}
void wideStorageHelpers(){
 for(int length:{0,3,6})for(int mode:{0,1,2})for(bool failed:{false,true}){
  Fixture f;Object object,sibling;constructWide(object,f.first,L"abcdef",length);auto* text=wide(object);text->GetBuffer(24);
  if(mode==1)call<void*>(0xd740,wide(sibling),text);if(mode==2)text->LockBuffer();
  f.first.failAllocate=failed;events.clear();sample(object,f,true,[&]()->INT64{call<void>(0x2b46b0,text);return 0;});
  f.first.failAllocate=false;destroy(object);if(mode==1)destroy(sibling);
 }
 for(int operation:{0,1,2,3})for(int length:{0,3,6,12})for(bool cloned:{false,true})for(bool failed:{false,true}){
  Fixture f;Object object;constructWide(object,f.first);auto* text=wide(object);f.first.cloned=cloned?&f.second:&f.first;
  f.first.failAllocate=f.second.failAllocate=f.first.failReallocate=failed;events.clear();
  sample(object,f,true,[&]()->INT64{
   if(operation==0)call<void>(0x3090,text,length);
   if(operation==1)call<void>(0x3170,text,length);
   if(operation==2){auto* data=call<CStringData*>(0xdd40,text->GetData());data->Release();}
   if(operation==3){auto* data=text->GetData();call<void>(0x33a0,text,data);return call<CStringData*>(0x3360,text)==data;}
   return 0;
  });
  f.first.failAllocate=f.second.failAllocate=f.first.failReallocate=false;destroy(object);
 }
 for(int first:{0,1,3})for(int second:{0,1,4})for(bool shared:{false,true})for(bool failed:{false,true}){
  Fixture f;Object object,sibling;constructWide(object,f.first);auto* text=wide(object);if(shared)call<void*>(0xd740,wide(sibling),text);
  f.first.failAllocate=f.first.failReallocate=failed;events.clear();
  sample(object,f,true,[&]()->INT64{call<void>(0x12c00,text,L"A\0B",first,L"\u03a9DEF",second);return 0;});
  f.first.failAllocate=f.first.failReallocate=false;destroy(object);if(shared)destroy(sibling);
 }
 for(int operation:{0,1,2})for(int length:{0,1,4}){
  Fixture f;Object object;constructWide(object,f.first);auto* text=wide(object);events.clear();
  sample(object,f,true,[&]()->INT64{
   auto* out=const_cast<wchar_t*>(text->GetString());
   if(operation==0)call<void>(0x2b4600,out,L"\u03a9\0DE",length);
   if(operation==1)call<void>(0x2b4600,out,static_cast<const wchar_t*>(nullptr),length);
   if(operation==2)call<void>(0x2b45f0,out+1,out,length);
   return call<const wchar_t*>(0x2e10,text)==text->GetString();
  });destroy(object);
 }
 {Fixture f;Object object;constructWide(object,f.first);events.clear();sample(object,f,true,[&]()->INT64{call<void>(0x3160);return 0;});destroy(object);}
 {Fixture f;Object object;constructWide(object,f.first);events.clear();sample(object,f,true,[&]()->INT64{return call<int>(0x2f40,L"\u03a9BC")+call<int>(0x2b45e0,"AB")+call<int>(0x2f40,static_cast<const wchar_t*>(nullptr))+call<int>(0x2b45e0,static_cast<const char*>(nullptr));});destroy(object);}
}
void wideStorage(){
 // Native linker folding must not silently select an ANSI source provider.
 auto saved=providers;for(auto entry:wideStorageProviders)providers[entry.first]=entry.second;
 snapshotCharSize=2;
 wideBuffers();wideStorageAssignments();wideStorageReentry();wideStorageHelpers();
 snapshotCharSize=1;providers=std::move(saved);
}


#include "string_operations_tests.h"
#include "string_search_tests.h"

using VariantInitAPI=decltype(&::VariantInit);using VariantClearAPI=decltype(&::VariantClear);using VariantCoerceAPI=decltype(&::VariantChangeType);
VariantInitAPI originalInit;VariantClearAPI originalClear;VariantCoerceAPI originalCoerce;
const VARIANT* expectedVariant;HRESULT coercionStatus,clearStatus;int variantMode;bool nullBSTR;
void WINAPI initVariant(VARIANT* value){
 const BYTE* raw=reinterpret_cast<const BYTE*>(value);bool zero=true;for(size_t at=0;at<sizeof(*value);++at)zero=zero&&!raw[at];
 events.push_back({30,zero,0,0,0,0});check(zero,"converted VARIANT storage is fully zeroed before VariantInit");
 originalInit(value);
}
HRESULT WINAPI coerceVariant(VARIANT* result,VARIANT* source,USHORT flags,VARTYPE type){
 events.push_back({31,source==expectedVariant,flags,type,source?source->vt:-1,0});check(source==expectedVariant&&flags==0&&type==VT_BSTR,"string conversion preserves the original VARIANT and uses VariantChangeType flags zero/VT_BSTR");
 if(variantMode==1)return originalCoerce(result,source,flags,type);
 result->vt=VT_BSTR;result->bstrVal=nullBSTR?nullptr:SysAllocStringLen(L"AB\0CDE",6);return coercionStatus;
}
HRESULT WINAPI clearVariant(VARIANT* value){
 events.push_back({32,value->vt,value->vt==VT_BSTR&&value->bstrVal?SysStringLen(value->bstrVal):-1,0,0,0});
 originalClear(value);return clearStatus;
}
void variantStrings(){
 Patch initialize("VariantInit",reinterpret_cast<void*>(&initVariant)),coerce("VariantChangeType",reinterpret_cast<void*>(&coerceVariant)),clear("VariantClear",reinterpret_cast<void*>(&clearVariant));
 originalInit=reinterpret_cast<VariantInitAPI>(initialize.old);originalCoerce=reinterpret_cast<VariantCoerceAPI>(coerce.old);originalClear=reinterpret_cast<VariantClearAPI>(clear.old);
 for(bool wide:{false,true})for(int operation=0;operation<4;++operation)for(int state:{0,1,2})for(bool clone:{false,true})for(int failure:{0,1,2})for(HRESULT status:std::array<HRESULT,5>{S_OK,S_FALSE,E_FAIL,E_OUTOFMEMORY,DISP_E_TYPEMISMATCH})for(bool empty:{false,true}){
  Fixture fixture;Object object,sibling;bool owned=operation>=2;
  if(owned){if(wide){auto* text=::new(object.storage) CString(&fixture.first);*text=L"prefix";}else construct(object,fixture.first);}
  if(owned&&state==1){if(wide)::new(sibling.storage) CString(*reinterpret_cast<CString*>(object.storage));else call<void*>(0x2b6c00,sibling.get(),object.get());}
  if(owned&&state==2){if(wide)reinterpret_cast<CString*>(object.storage)->LockBuffer();else call<char*>(0x203f20,object.get());}
  fixture.first.cloned=clone?&fixture.second:&fixture.first;fixture.first.failAllocate=fixture.second.failAllocate=failure==1;fixture.first.failReallocate=failure==2;
  VARIANT input{};input.vt=VT_I4;input.lVal=17;expectedVariant=&input;coercionStatus=status;clearStatus=E_FAIL;variantMode=0;nullBSTR=empty;events.clear();
  sample(object,fixture,owned,[&]()->INT64{
   if(operation<2){
    if(operation==0)call<void*>(wide?0x2b31c0:0x2b2be0,object.storage,&input,&fixture.first);
    else call<void*>(wide?0x2b3280:0x2b2c90,object.storage,&input);
    owned=true;safeRead=true;
   }else{
    void* result=call<void*>(wide?(operation==2?0x2b3340:0x2b33d0):(operation==2?0x2b2d30:0x2b2db0),object.storage,&input);
    check(result==object.storage,"VARIANT string operators return their actual receiver");
   }
   return object.get()->GetLength();
  });
  fixture.first.failAllocate=fixture.second.failAllocate=fixture.first.failReallocate=false;
  if(owned){if(wide)reinterpret_cast<CString*>(object.storage)->~CString();else destroy(object);}
  if(operation>=2&&state==1){if(wide)reinterpret_cast<CString*>(sibling.storage)->~CString();else destroy(sibling);}
 }
 for(bool wide:{false,true})for(int operation=0;operation<4;++operation)for(int type:{VT_EMPTY,VT_NULL,VT_I4,VT_BOOL,VT_BSTR,VT_R8,VT_ERROR}){
  Fixture fixture;Object object;bool owned=operation>=2;
  if(owned){if(wide){auto* text=::new(object.storage) CString(&fixture.first);*text=L"prefix";}else construct(object,fixture.first);}
  VARIANT input{};input.vt=VARTYPE(type);if(type==VT_I4)input.lVal=-123;if(type==VT_BOOL)input.boolVal=VARIANT_TRUE;if(type==VT_R8)input.dblVal=1.25;if(type==VT_ERROR)input.scode=E_FAIL;if(type==VT_BSTR)input.bstrVal=SysAllocStringLen(L"A\0tail",6);
  expectedVariant=&input;variantMode=1;events.clear();
  sample(object,fixture,owned,[&]()->INT64{
   if(operation<2){if(operation==0)call<void*>(wide?0x2b31c0:0x2b2be0,object.storage,&input,&fixture.first);else call<void*>(wide?0x2b3280:0x2b2c90,object.storage,&input);owned=true;safeRead=true;}
   else call<void*>(wide?(operation==2?0x2b3340:0x2b33d0):(operation==2?0x2b2d30:0x2b2db0),object.storage,&input);
   return object.get()->GetLength();
  });
  if(owned){if(wide)reinterpret_cast<CString*>(object.storage)->~CString();else destroy(object);}originalClear(&input);
 }
 for(bool wide:{false,true})for(int operation=0;operation<4;++operation)for(bool missingSource:{false,true})for(bool missingManager:{false,true}){
  Fixture fixture;Object object;bool owned=operation>=2;
  if(owned){if(wide){auto* text=::new(object.storage) CString(&fixture.first);*text=L"prefix";}else construct(object,fixture.first);}
  VARIANT input{};input.vt=VT_I4;input.lVal=42;
  expectedVariant=missingSource?nullptr:&input;variantMode=0;coercionStatus=expectedVariant?S_OK:E_INVALIDARG;nullBSTR=false;events.clear();
  sample(object,fixture,owned,[&]()->INT64{
   if(operation<2){if(operation==0)call<void*>(wide?0x2b31c0:0x2b2be0,object.storage,expectedVariant,missingManager?nullptr:&fixture.first);else call<void*>(wide?0x2b3280:0x2b2c90,object.storage,expectedVariant);owned=true;safeRead=true;}
   else call<void*>(wide?(operation==2?0x2b3340:0x2b33d0):(operation==2?0x2b2d30:0x2b2db0),object.storage,expectedVariant);
   return object.get()->GetLength();
  });
  if(owned){if(wide)reinterpret_cast<CString*>(object.storage)->~CString();else destroy(object);}
 }
 variantMode=0;expectedVariant=nullptr;
}

void* throwingFinder(UINT){throw 71;}
struct Hook {BYTE* entry;BYTE saved[12];DWORD protection;Hook(void* target,void* callback):entry(static_cast<BYTE*>(target)){memcpy(saved,entry,12);VirtualProtect(entry,12,PAGE_EXECUTE_READWRITE,&protection);BYTE bytes[12]={0x48,0xb8};memcpy(bytes+2,&callback,8);bytes[10]=0xff;bytes[11]=0xe0;memcpy(entry,bytes,12);FlushInstructionCache(GetCurrentProcess(),entry,12);}~Hook(){memcpy(entry,saved,12);FlushInstructionCache(GetCurrentProcess(),entry,12);DWORD ignored;VirtualProtect(entry,12,protection,&ignored);}};
void noexceptCases(){
 char executable[MAX_PATH];GetModuleFileNameA(nullptr,executable,MAX_PATH);
 for(int kind:{0,1,2,3,4,5,6}){
  std::string command="\""+std::string(executable)+"\" --ansi-noexcept "+(image?"native ":"source ")+std::to_string(kind);
  STARTUPINFOA start{sizeof(start)};PROCESS_INFORMATION process{};
  BOOL launched=CreateProcessA(executable,command.data(),nullptr,nullptr,FALSE,0,nullptr,nullptr,&start,&process);
  check(launched!=FALSE,"launch isolated ANSI resource-traits exception comparison");if(!launched)continue;
  DWORD wait=WaitForSingleObject(process.hProcess,30000);if(wait!=WAIT_OBJECT_0)TerminateProcess(process.hProcess,75);
  DWORD code=0;GetExitCodeProcess(process.hProcess,&code);CloseHandle(process.hThread);CloseHandle(process.hProcess);
  if(code!=73)printf("ANSI child %s kind%d exit%lu\n",image?"Native":"Source",kind,code);
  check(wait==WAIT_OBJECT_0&&code==73,"string constructors and resource traits preserve native no-throw boundaries");
 }
}

}
int TestAnsiStringOwnership(HMODULE native){using namespace ansi_string_test;image=native;failures=0;comparisons=0;
 if(providers.empty())for(auto entry:ansiStringProviders)providers.emplace(entry.first,entry.second);
 auto** table=*reinterpret_cast<void***>(native?static_cast<void*>(reinterpret_cast<BYTE*>(native)+0x3b25e8):static_cast<void*>(OpenMFC_GetStringMgr()));
 {Patch allocate(table,reinterpret_cast<void*>(&defaultAllocate)),release(table+1,reinterpret_cast<void*>(&defaultFree)),reallocate(table+2,reinterpret_cast<void*>(&defaultReallocate));
 constructors();
 buffers();
 assignment();
 reentry();
 extraStorage();
 conversions();
 resources();
 managerExports();
 wideConstructors();
 wideConversions();
 wideConstructorResources();
 unsignedConstructors();wideAssignments();wideRepeatCopies();
 wideAppend();
 wideStorage();
 stringOperations();
 stringSearchAndCase();
 variantStrings();
 noexceptCases();
 }
 printf("%s string storage/conversion: %u native comparisons, %d failures\n",native?"Native":"Source",comparisons,failures);return failures;
}

int TestAnsiNoexceptChild(int argc,char** argv){
 using namespace ansi_string_test;
 if(argc!=4)return 76;
 _set_abort_behavior(0,_WRITE_ABORT_MSG|_CALL_REPORTFAULT);
 std::set_terminate([]{ExitProcess(73);});
 bool native=!strcmp(argv[2],"native");int kind=atoi(argv[3]);
 try {
  if(native){
   const char* path=getenv("OPENMFC_NATIVE_EXCEPTION_DLL");if(!path)return 78;image=LoadLibraryA(path);if(!image)return 79;
   auto* bytes=reinterpret_cast<BYTE*>(image);auto* pe=reinterpret_cast<IMAGE_NT_HEADERS*>(bytes+reinterpret_cast<IMAGE_DOS_HEADER*>(bytes)->e_lfanew);
   if(pe->FileHeader.TimeDateStamp!=0x442cba30||pe->OptionalHeader.SizeOfImage!=0x55c000)return 80;
   Hook finder(bytes+0x2aee00,reinterpret_cast<void*>(&throwingFinder));Object object;
   if(kind==0){reinterpret_cast<void*(*)(void*)>(bytes+0x3040)(object.storage);call<int>(0x1cd6b0,object.string(),UINT(35));}
   if(kind==1)call<void*>(0x2b69d0,object.string(),reinterpret_cast<const char*>(UINT_PTR(35)));
   if(kind==2)call<void*>(0x1cdad0,object.string(),reinterpret_cast<const wchar_t*>(UINT_PTR(35)));
   if(kind==3||kind==4)call<void*>(0x12bc0,object.storage,static_cast<IAtlStringMgr*>(nullptr));
   if(kind==5)call<void*>(0xdcb0,object.storage,reinterpret_cast<const wchar_t*>(UINT_PTR(35)));
   if(kind==6)call<void*>(0x1cd550,object.storage,reinterpret_cast<const char*>(UINT_PTR(35)));
  }else{
   g_fileExceptionFinder=&throwingFinder;
   if(kind==0){CStringA text;text.LoadString(35);}
   if(kind==1){CStringA text(reinterpret_cast<const char*>(UINT_PTR(35)));}
   if(kind==2){CStringA text(reinterpret_cast<const wchar_t*>(UINT_PTR(35)));}
   if(kind==3){CString text(static_cast<IAtlStringMgr*>(nullptr));}
   if(kind==4){CStringA text(static_cast<IAtlStringMgr*>(nullptr));}
   if(kind==5){CString text(reinterpret_cast<const wchar_t*>(UINT_PTR(35)));}
   if(kind==6){CString text(reinterpret_cast<const char*>(UINT_PTR(35)));}
  }
 }catch(...){return 74;}
 return 77;
}
