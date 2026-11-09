// Independent native-oracle comparisons for string services and Win32 failures.
INT64 serviceBstrDigest(BSTR text) {
 if(!text)return -1;uint32_t hash=2166136261u;UINT length=SysStringLen(text);
 for(UINT at=0;at<length;++at)hash=(hash^uint32_t(text[at]))*16777619u;
 return INT64((UINT64(length)<<32)|hash);
}
struct RecordServiceBstr {
 BSTR& text;~RecordServiceBstr(){events.push_back({90,text!=nullptr,serviceBstrDigest(text),0,0,0});SysFreeString(text);text=nullptr;}
};
template<class T>void naturalServiceBstr() {
 using E=StringServiceExports<T>;using C=typename T::XCHAR;snapshotCharSize=sizeof(C);
 for(int kind:{0,1,2,3,4})for(bool setting:{false,true})for(int state:{0,1,2}) {
  Fixture f;Object object,sibling;mutationConstruct<T>(object,f.first,kind);auto* text=operationText<T>(object);
  if(state==1)::new(sibling.storage) T(*text);if(state==2)text->LockBuffer();events.clear();
  sample(object,f,true,[&]()->INT64 {
   BSTR output=setting?SysAllocString(L"previous"):nullptr;RecordServiceBstr record{output};
   if(setting){auto* result=exactExport(E::SetSysStringRVA,E::SetSysString,text,&output);check(result==output,"SetSysString returns the caller BSTR");}
   else output=exactExport(E::AllocSysStringRVA,E::AllocSysString,text);
   return serviceBstrDigest(output);
  });destroy(object);if(state==1)destroy(sibling);
 }
 snapshotCharSize=1;
}
using ServiceAllocate=decltype(&::SysAllocStringLen);using ServiceReallocate=decltype(&::SysReAllocStringLen);
ServiceAllocate serviceAllocate;ServiceReallocate serviceReallocate;
const void* serviceSource;BSTR* serviceTarget;bool serviceBstrFailure;int serviceQuery,serviceWrite;
BSTR WINAPI allocateServiceBstr(const OLECHAR* source,UINT length) {
 events.push_back({91,source!=nullptr,source==serviceSource,length,0,0});mutate(2);
 if(serviceBstrFailure)return nullptr;
 UINT count=length>32?3:length;BSTR result=serviceAllocate(source,count);
 if(result&&!source)for(UINT at=0;at<count;++at)result[at]=L'Q';return result;
}
INT WINAPI reallocateServiceBstr(BSTR* target,const OLECHAR* source,UINT length) {
 events.push_back({92,target==serviceTarget,source!=nullptr,source==serviceSource,length,target!=nullptr});mutate(2);
 if(serviceBstrFailure)return FALSE;
 UINT count=length>32?3:length;int result=serviceReallocate(target,source,count);
 if(result&&target&&*target&&!source)for(UINT at=0;at<count;++at)(*target)[at]=L'Q';return result;
}
int WINAPI convertServiceBstr(UINT cp,DWORD flags,const char* source,int length,wchar_t* output,int capacity) {
 events.push_back({93,cp,flags,source==serviceSource,length,output!=nullptr});events.push_back({94,capacity,output&&serviceTarget?output==*serviceTarget:0,0,0,0});
 mutate(output?5:4);
 if(!output)return serviceQuery;
 if(capacity>0&&capacity<=32)for(int at=0;at<capacity;++at)output[at]=wchar_t(L'A'+at);
 return serviceWrite;
}
template<class T>void forcedServiceBstr() {
 using E=StringServiceExports<T>;using C=typename T::XCHAR;snapshotCharSize=mutationWidth=sizeof(C);
 Patch allocate("SysAllocStringLen",reinterpret_cast<void*>(&allocateServiceBstr)),reallocate("SysReAllocStringLen",reinterpret_cast<void*>(&reallocateServiceBstr));
 serviceAllocate=reinterpret_cast<ServiceAllocate>(allocate.old);serviceReallocate=reinterpret_cast<ServiceReallocate>(reallocate.old);
 Patch conversion("MultiByteToWideChar",reinterpret_cast<void*>(&convertServiceBstr));
 for(int kind:{0,1,2,3})for(bool setting:{false,true})for(bool failure:{false,true})for(int query:{-1,0,3,8})for(int written:{-1,0,2})for(int stage:{0,2,4,5}) {
  Fixture f;Object object;operationConstruct<T>(object,f.first,kind==1?2:kind==2?1:0);auto* text=operationText<T>(object);
  if(kind==3)text->LockBuffer();BSTR output=setting?serviceAllocate(L"previous",8):nullptr;
  serviceTarget=&output;serviceSource=text->GetString();serviceBstrFailure=failure;serviceQuery=query;serviceWrite=written;
  mutationTarget=object.get();mutationStage=stage;events.clear();
  sample(object,f,true,[&]()->INT64 {
   RecordServiceBstr record{output};
   if(setting)return exactExport(E::SetSysStringRVA,E::SetSysString,text,&output)==output;
   output=exactExport(E::AllocSysStringRVA,E::AllocSysString,text);return output!=nullptr;
  });mutationTarget=nullptr;mutationStage=0;destroy(object);
 }
 serviceTarget=nullptr;serviceSource=nullptr;serviceBstrFailure=false;mutationWidth=snapshotCharSize=1;
}
using ServiceOEM=decltype(&::CharToOemBuffA);ServiceOEM serviceToOEM,serviceToANSI;
bool serviceOEMFailure,serviceOEMNatural;DWORD serviceOEMError;
BOOL WINAPI convertServiceOEM(const char* source,char* output,DWORD count,bool toOEM) {
 events.push_back({95,toOEM,source==output,count,GetLastError(),0});mutate(4);
 if(serviceOEMNatural)return (toOEM?serviceToOEM:serviceToANSI)(source,output,count);
 for(DWORD at=0;at<count;++at)output[at]=char('A'+at%26);
 SetLastError(serviceOEMError);return !serviceOEMFailure;
}
BOOL WINAPI toServiceOEM(const char* source,char* output,DWORD count){return convertServiceOEM(source,output,count,true);}
BOOL WINAPI toServiceANSI(const char* source,char* output,DWORD count){return convertServiceOEM(source,output,count,false);}
template<class T>void serviceOEM() {
 using E=StringServiceExports<T>;using C=typename T::XCHAR;snapshotCharSize=mutationWidth=sizeof(C);
 Patch toOEM("CharToOemBuffA",reinterpret_cast<void*>(&toServiceOEM)),toANSI("OemToCharBuffA",reinterpret_cast<void*>(&toServiceANSI));
 serviceToOEM=reinterpret_cast<ServiceOEM>(toOEM.old);serviceToANSI=reinterpret_cast<ServiceOEM>(toANSI.old);
 for(int kind:{0,1,2,3,4})for(int state:{0,1,2,3})for(bool clone:{false,true})for(int failure:{0,1,2})for(int op:{0,1})for(int mode:{0,1,2})for(DWORD error:std::array<DWORD,4>{0,5,0x12345,0x80004001}) {
  if(mode==0&&error)continue;
  Fixture f;Object object,sibling;mutationConstruct<T>(object,f.first,kind);auto* text=operationText<T>(object);
  if(state==1)::new(sibling.storage) T(*text);if(state>=2){text->LockBuffer();if(state==3)text->LockBuffer();}
  f.first.cloned=clone?&f.second:&f.first;f.first.failAllocate=f.second.failAllocate=failure==1;f.first.failReallocate=failure==2;
  serviceOEMNatural=mode==0;serviceOEMFailure=mode==2;serviceOEMError=error;mutationTarget=object.get();mutationStage=4;events.clear();
  sample(object,f,true,[&]()->INT64 {
   SetLastError(0x1234);RecordMessageError record;
   if(op==0)exactExport(E::CharToOemARVA,E::CharToOemA,text);else exactExport(E::OemToCharARVA,E::OemToCharA,text);
   return text->GetLength();
  });mutationTarget=nullptr;mutationStage=0;f.first.failAllocate=f.second.failAllocate=f.first.failReallocate=false;destroy(object);if(state==1)destroy(sibling);
 }
 mutationWidth=snapshotCharSize=1;
}
const void* serviceEnvironmentName;DWORD serviceEnvironmentQuery,serviceEnvironmentReturn;int serviceEnvironmentMode;
template<class C>DWORD environmentService(const C* name,C* output,DWORD capacity) {
 events.push_back({96,sizeof(C),name==serviceEnvironmentName,output!=nullptr,capacity,0});
 mutate(output?5:4);SetLastError(output?0x5678:0x1234);
 if(!output)return serviceEnvironmentQuery;
 if(serviceEnvironmentMode==1){if(capacity)output[0]=0;}
 if(serviceEnvironmentMode==2){for(DWORD at=0;at<capacity;++at)output[at]=C('X');if(capacity>2)output[2]=0;}
 if(serviceEnvironmentMode==3)for(DWORD at=0;at<capacity;++at)output[at]=C('Z');
 return serviceEnvironmentReturn;
}
DWORD WINAPI byteEnvironmentService(const char* name,char* output,DWORD capacity){return environmentService(name,output,capacity);}
DWORD WINAPI wideEnvironmentService(const wchar_t* name,wchar_t* output,DWORD capacity){return environmentService(name,output,capacity);}
template<class T>void forcedServiceEnvironment() {
 using E=StringServiceExports<T>;using C=typename T::XCHAR;snapshotCharSize=mutationWidth=sizeof(C);
 Patch api(sizeof(C)==1?"GetEnvironmentVariableA":"GetEnvironmentVariableW",sizeof(C)==1?reinterpret_cast<void*>(&byteEnvironmentService):reinterpret_cast<void*>(&wideEnvironmentService));
 const C name[]={C('v'),C('a'),C('r'),0};
 for(int kind:{0,1,2})for(int state:{0,1,2})for(bool spare:{false,true})for(bool clone:{false,true})for(int failure:{0,1,2})for(bool generic:{false,true})for(DWORD query:std::array<DWORD,5>{0,3,12,0x80000000,0xffffffff})for(int mode:{0,1,2,3}) {
  if(query>=0x80000000&&mode)continue;
  Fixture f;Object object,sibling;operationConstruct<T>(object,f.first,kind);auto* text=operationText<T>(object);if(spare)text->Preallocate(24);
  if(state==1)::new(sibling.storage) T(*text);if(state==2)text->LockBuffer();
  f.first.cloned=clone?&f.second:&f.first;f.first.failAllocate=f.second.failAllocate=failure==1;f.first.failReallocate=failure==2;
  const C* argument=mode==0?nullptr:mode==1?text->GetString():name;serviceEnvironmentName=argument;
  serviceEnvironmentQuery=query;serviceEnvironmentReturn=mode==0?0:mode==1?100:mode==2?2:query;serviceEnvironmentMode=mode;
  events.clear();
  sample(object,f,true,[&]()->INT64 {
   SetLastError(0);RecordMessageError record;
   return generic?exactExport(E::EnvironmentRVA,E::Environment,text,argument):exactExport(E::EnvironmentWRVA,E::EnvironmentW,text,argument);
  });f.first.failAllocate=f.second.failAllocate=f.first.failReallocate=false;destroy(object);if(state==1)destroy(sibling);
 }
 // Query and write callbacks may change the receiver before ReleaseBuffer scans it.
 for(int stage:{1,2,3,4,5})for(bool shared:{false,true}) {
  Fixture f;Object object,sibling;operationConstruct<T>(object,f.first);auto* text=operationText<T>(object);if(shared)::new(sibling.storage) T(*text);
  mutationTarget=object.get();mutationStage=stage;serviceEnvironmentName=name;serviceEnvironmentQuery=12;serviceEnvironmentReturn=0;serviceEnvironmentMode=0;events.clear();
  sample(object,f,true,[&]()->INT64{return exactExport(E::EnvironmentWRVA,E::EnvironmentW,text,name);});mutationTarget=nullptr;mutationStage=0;destroy(object);if(shared)destroy(sibling);
 }
 mutationWidth=snapshotCharSize=1;
}
template<class T>void implicitServiceResources() {
 using E=StringServiceExports<T>;using C=typename T::XCHAR;snapshotCharSize=sizeof(C);
 for(int state:{0,1,2})for(int failed:{0,1,2,3,4,5,6})for(int allocation:{0,1,2})for(int conversion:{0,1,2,3})for(UINT_PTR id:std::array<UINT_PTR,7>{0,1,35,65535,65536,UINT_PTR(-1),0x12345678}) {
  if constexpr(sizeof(C)==2)if(conversion)continue;
  Fixture f;Object object,sibling;operationConstruct<T>(object,f.first);auto* text=operationText<T>(object);if(state==1)::new(sibling.storage) T(*text);if(state==2)text->LockBuffer();
  resourceText=L"resource content";resourceFailure=failed;conversionFailure=conversion;f.first.failAllocate=f.second.failAllocate=allocation==1;f.first.failReallocate=allocation==2;events.clear();
  sample(object,f,true,[&]()->INT64{return exactExport(E::CheckImplicitLoadRVA,E::CheckImplicitLoad,text,reinterpret_cast<const void*>(id));});
  f.first.failAllocate=f.second.failAllocate=f.first.failReallocate=false;destroy(object);if(state==1)destroy(sibling);
 }
 resourceFailure=conversionFailure=0;snapshotCharSize=1;
}
template<class T>void serviceConstruct() {
 using E=StringServiceExports<T>;using C=typename T::XCHAR;snapshotCharSize=sizeof(C);
 for(int kind:{0,1,2}) {
  Fixture f;Object object;CStringData* previous=nullptr;
  if(kind){operationConstruct<T>(object,f.first);previous=object.get()->GetData();if(kind==2)memset(object.storage,0,sizeof(T));}
  events.clear();
  sample(object,f,false,[&]()->INT64{exactExport(E::ConstructRVA,E::Construct,operationText<T>(object));safeRead=true;return operationText<T>(object)->GetLength();});
  destroy(object);if(previous)previous->Release();
 }
 snapshotCharSize=1;
}
void stringServices() {
 StringCRTState state;_wsetlocale(LC_ALL,L"C");_setmbcp(1252);
 naturalServiceBstr<CStringA>();naturalServiceBstr<CStringW>();forcedServiceBstr<CStringA>();forcedServiceBstr<CStringW>();
 serviceOEM<CStringA>();serviceOEM<CStringW>();forcedServiceEnvironment<CStringA>();forcedServiceEnvironment<CStringW>();
 auto previous=g_fileExceptionFinder;g_fileExceptionFinder=&finder;
 {Patch a("FindResourceW",reinterpret_cast<void*>(&find)),b("LoadResource",reinterpret_cast<void*>(&load)),c("LockResource",reinterpret_cast<void*>(&lock)),d("SizeofResource",reinterpret_cast<void*>(&size)),e("WideCharToMultiByte",reinterpret_cast<void*>(&convert));realConvert=reinterpret_cast<Converter>(e.old);
 implicitServiceResources<CStringA>();implicitServiceResources<CStringW>();}
 g_fileExceptionFinder=previous;serviceConstruct<CStringA>();serviceConstruct<CStringW>();
}
