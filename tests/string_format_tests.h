// Included in the ownership fixture namespace; addresses select the pinned oracle.
template<class T>void printfVEntry(T* text,bool append,const typename T::XCHAR* format,...){
 using E=StringFormatExports<T>;va_list args;va_start(args,format);
 if(append)exactExport(E::AppendFormatVRVA,E::AppendFormatV,text,format,args);
 else exactExport(E::FormatVRVA,E::FormatV,text,format,args);
 va_end(args);
}
template<class T>void messageVEntry(T* text,const typename T::XCHAR* format,...){
 using E=StringFormatExports<T>;va_list args;va_start(args,format);
 exactExport(E::MessageVRVA,E::MessageV,text,format,&args);va_end(args);
}
template<class C>std::basic_string<C> formatText(const char* text){
 std::basic_string<C> out;for(;*text;++text)out+=C(static_cast<unsigned char>(*text));return out;
}
template<class T>INT64 formatDigest(const T& text){
 uint32_t hash=2166136261u;
 for(int at=0;at<text.GetLength();++at)hash=(hash^uint32_t(static_cast<std::make_unsigned_t<typename T::XCHAR>>(text.GetString()[at])))*16777619u;
 return INT64((UINT64(uint32_t(text.GetLength()))<<32)|hash);
}
template<class T,class... A>void printfCall(int mode,T* text,const typename T::XCHAR* format,A... args){
 using E=StringFormatExports<T>;
 if(mode==0)exactExport(E::FormatRVA,E::Format,text,format,args...);
 if(mode==1)exactExport(E::AppendFormatRVA,E::AppendFormat,text,format,args...);
 if(mode==2)printfVEntry(text,false,format,args...);
 if(mode==3)printfVEntry(text,true,format,args...);
}
template<class T>void naturalPrintf(){
 using C=typename T::XCHAR;snapshotCharSize=sizeof(C);
 for(int shape:{0,1,2,3,4})for(int mode:{0,1,2,3})for(int state:{0,1,2,3})for(int alias:{0,1,2})for(bool clone:{false,true})for(int failure:{0,1,2}){
  Fixture f;Object object,sibling;
  auto pattern=formatText<C>(shape==0?"":shape==1?"%s:%08d/%I64d/%.1f":shape==2?"plain":shape==3?"%c/%s":"%*s");
  auto initial=alias==2?formatText<C>("X")+pattern:alias?pattern:formatText<C>("old");
  auto* text=::new(object.storage) T(initial.c_str(),int(initial.size()),&f.first);
  if(state==1)::new(sibling.storage) T(*text);if(state>=2){text->LockBuffer();if(state==3)text->LockBuffer();}
  const C* format=alias?text->GetString()+(alias==2):pattern.c_str();auto argument=formatText<C>("tail");
  f.first.cloned=clone?&f.second:&f.first;f.first.failAllocate=f.second.failAllocate=failure==1;f.first.failReallocate=failure==2;events.clear();
  sample(object,f,true,[&]()->INT64{
   if(shape<=2)printfCall(mode,text,format,argument.c_str(),42,INT64(9876543210),2.5);
   if(shape==3)printfCall(mode,text,format,0,argument.c_str());
   if(shape==4)printfCall(mode,text,format,40000,argument.c_str());
   return formatDigest(*text);
  });f.first.failAllocate=f.second.failAllocate=f.first.failReallocate=false;destroy(object);if(state==1)destroy(sibling);
 }
 for(int mode:{0,1,2,3}){
  Fixture f;Object object;operationConstruct<T>(object,f.first);events.clear();
  sample(object,f,true,[&]()->INT64{printfCall(mode,operationText<T>(object),static_cast<const C*>(nullptr),1);return 0;});destroy(object);
 }
 snapshotCharSize=1;
}
using BytePrintf=int(__cdecl*)(UINT64,char*,size_t,const char*,_locale_t,va_list);
using WidePrintf=int(__cdecl*)(UINT64,wchar_t*,size_t,const wchar_t*,_locale_t,va_list);
BytePrintf realByteMeasure,realByteWrite;WidePrintf realWideMeasure,realWideWrite;
int measuredFormat,writtenFormat;bool changeFormatOptions;const void* originalFormat;
UINT64* printfOptions(){return image?reinterpret_cast<UINT64*>(reinterpret_cast<BYTE*>(image)+0x3be198):__local_stdio_printf_options();}
template<class C>int recordPrintf(UINT64 options,C* output,size_t size,const C* format,_locale_t locale,va_list args,bool measuring){
 va_list copy;va_copy(copy,args);int first=va_arg(copy,int);va_end(copy);
 events.push_back({70+INT64(!measuring),sizeof(C),INT64(options),output!=nullptr,INT64(size),locale==nullptr});
 events.push_back({72,format==originalFormat,format?int(format[0]):0,first,0,0});
 mutate(measuring?4:5);
 if(measuring){if(changeFormatOptions)*printfOptions()^=0x10;return measuredFormat;}
 for(size_t at=0;at<(std::min)(size,size_t(4));++at)output[at]=C('a'+at);
 if(size>2)output[1]=0;if(size)output[size-1]=0;
 return writtenFormat;
}
int __cdecl byteMeasure(UINT64 o,char* b,size_t n,const char* f,_locale_t l,va_list a){return recordPrintf(o,b,n,f,l,a,true);}
int __cdecl wideMeasure(UINT64 o,wchar_t* b,size_t n,const wchar_t* f,_locale_t l,va_list a){return recordPrintf(o,b,n,f,l,a,true);}
int __cdecl byteWrite(UINT64 o,char* b,size_t n,const char* f,_locale_t l,va_list a){return recordPrintf(o,b,n,f,l,a,false);}
int __cdecl wideWrite(UINT64 o,wchar_t* b,size_t n,const wchar_t* f,_locale_t l,va_list a){return recordPrintf(o,b,n,f,l,a,false);}
template<class T>void forcedPrintf(){
 using C=typename T::XCHAR;snapshotCharSize=sizeof(C);mutationWidth=sizeof(C);
 Patch measure(sizeof(C)==1?"__stdio_common_vsprintf":"__stdio_common_vswprintf",sizeof(C)==1?reinterpret_cast<void*>(&byteMeasure):reinterpret_cast<void*>(&wideMeasure));
 Patch write(sizeof(C)==1?"__stdio_common_vsprintf_s":"__stdio_common_vswprintf_s",sizeof(C)==1?reinterpret_cast<void*>(&byteWrite):reinterpret_cast<void*>(&wideWrite));
 UINT64 saved=*printfOptions();
 for(int count:{-9,-1,0,3,8,16})for(int returned:{-1,0,1,99})for(int mode:{0,1,2,3})for(int state:{0,1,2})for(bool change:{false,true}){
  Fixture f;Object object,sibling;auto pattern=formatText<C>("%d");auto* text=::new(object.storage) T(pattern.c_str(),2,&f.first);
  if(state==1)::new(sibling.storage) T(*text);if(state==2)text->LockBuffer();
  measuredFormat=count;writtenFormat=returned;changeFormatOptions=change;*printfOptions()=0x20;originalFormat=text->GetString();events.clear();
  sample(object,f,true,[&]()->INT64{printfCall(mode,text,text->GetString(),42);return formatDigest(*text);});destroy(object);if(state==1)destroy(sibling);
 }
 for(int stage:{1,2,3,4,5})for(int mode:{0,1,2,3})for(bool shared:{false,true}){
  Fixture f;Object object,sibling;auto pattern=formatText<C>("%d-----");auto* text=::new(object.storage) T(pattern.c_str(),7,&f.first);
  if(shared)::new(sibling.storage) T(*text);mutationTarget=object.get();mutationStage=stage;
  measuredFormat=12;writtenFormat=-1;changeFormatOptions=true;*printfOptions()=0;originalFormat=text->GetString();events.clear();
  sample(object,f,true,[&]()->INT64{printfCall(mode,text,text->GetString(),42);return formatDigest(*text);});
  mutationTarget=nullptr;mutationStage=0;destroy(object);if(shared)destroy(sibling);
 }
 for(bool append:{false,true}){
  Fixture f;Object object;operationConstruct<T>(object,f.first);auto* text=operationText<T>(object);auto pattern=formatText<C>("%d");
  measuredFormat=INT32_MAX;writtenFormat=0;changeFormatOptions=false;*printfOptions()=0;originalFormat=pattern.c_str();f.first.failReallocate=true;events.clear();
  sample(object,f,true,[&]()->INT64{printfCall(append?1:0,text,pattern.c_str(),42);return 0;});f.first.failReallocate=false;destroy(object);
 }
 *printfOptions()=saved;changeFormatOptions=false;mutationWidth=1;snapshotCharSize=1;
}
using MessageAAPI=decltype(&::FormatMessageA);using MessageWAPI=decltype(&::FormatMessageW);using LocalFreeAPI=decltype(&::LocalFree);
MessageAAPI actualMessageA;MessageWAPI actualMessageW;LocalFreeAPI actualMessageFree;
int messageMode;DWORD messageStatus;bool messageFailure,messageAlias,messageFreeError;void* messageOutput;const void* messageOriginal;
template<class C>DWORD recordMessage(DWORD flags,const void* format,DWORD id,DWORD language,C* output,DWORD capacity,va_list* args){
 events.push_back({80,sizeof(C),flags,id,language,capacity});events.push_back({81,GetLastError(),format==messageOriginal,args!=nullptr,format?int(*static_cast<const C*>(format)):0,0});
 C* data=nullptr;
 if(messageMode==0||messageMode==3){data=static_cast<C*>(LocalAlloc(LMEM_FIXED,32*sizeof(C)));const char* value=messageMode==3?"":"ABcdefghij";size_t at=0;for(;value[at];++at)data[at]=C(value[at]);data[at]=0;if(messageMode==0)data[2]=0;}
 if(messageMode==2)data=const_cast<C*>(static_cast<const C*>(messageOutput));
 *reinterpret_cast<C**>(output)=data;messageOutput=data;
 SetLastError(messageStatus);return messageFailure?0:20;
}
DWORD WINAPI fakeMessageA(DWORD f,LPCVOID s,DWORD i,DWORD l,LPSTR b,DWORD n,va_list* a){return recordMessage(f,s,i,l,b,n,a);}
DWORD WINAPI fakeMessageW(DWORD f,LPCVOID s,DWORD i,DWORD l,LPWSTR b,DWORD n,va_list* a){return recordMessage(f,s,i,l,b,n,a);}
HLOCAL WINAPI freeMessage(HLOCAL data){
 events.push_back({82,data==messageOutput,data!=nullptr,messageAlias,GetLastError(),0});
 HLOCAL result=messageAlias?nullptr:actualMessageFree(data);if(messageFreeError)SetLastError(0x9876);return result;
}
struct RecordMessageError{~RecordMessageError(){events.push_back({83,GetLastError(),0,0,0,0});}};
template<class T,class... A>void messageCall(int mode,T* text,const typename T::XCHAR* format,A... args){
 using E=StringFormatExports<T>;
 if(mode==0)messageVEntry(text,format,args...);
 if(mode==1)exactExport(E::MessageRVA,E::Message,text,format,args...);
 if(mode==2)exactExport(E::MessageWRVA,E::MessageW,text,format,args...);
}
template<class T>void naturalMessages(){
 using C=typename T::XCHAR;snapshotCharSize=sizeof(C);
 for(int state:{0,1,2})for(bool clone:{false,true})for(int failure:{0,1,2})for(int mode:{0,1,2})for(int patternKind:{0,1,2,3}){
  Fixture f;Object object,sibling;operationConstruct<T>(object,f.first);auto* text=operationText<T>(object);
  if(state==1)::new(sibling.storage) T(*text);if(state==2)text->LockBuffer();
  auto pattern=formatText<C>(patternKind==0?"":patternKind==1?"message":patternKind==2?"%1!s!:%2!d!":"%1!s!/%1!s!");auto arg=formatText<C>("TEXT");
  f.first.cloned=clone?&f.second:&f.first;f.first.failAllocate=f.second.failAllocate=failure==1;f.first.failReallocate=failure==2;events.clear();SetLastError(0x4321);
  sample(object,f,true,[&]()->INT64{RecordMessageError record;messageCall(mode,text,pattern.c_str(),arg.c_str(),42);return formatDigest(*text);});
  f.first.failAllocate=f.second.failAllocate=f.first.failReallocate=false;destroy(object);if(state==1)destroy(sibling);
 }
 snapshotCharSize=1;
}
template<class T>void forcedMessages(){
 using C=typename T::XCHAR;snapshotCharSize=sizeof(C);
 Patch api(sizeof(C)==1?"FormatMessageA":"FormatMessageW",sizeof(C)==1?reinterpret_cast<void*>(&fakeMessageA):reinterpret_cast<void*>(&fakeMessageW));
 Patch release("LocalFree",reinterpret_cast<void*>(&freeMessage));actualMessageFree=reinterpret_cast<LocalFreeAPI>(release.old);
 for(int output:{0,1,2,3})for(bool failed:{false,true})for(DWORD status:{0u,5u,8u,87u})for(int state:{0,1,2})for(int failure:{0,1,2})for(int mode:{0,1,2})for(bool freeError:{false,true}){
  Fixture f;Object object,sibling;operationConstruct<T>(object,f.first);auto* text=operationText<T>(object);if(state==1)::new(sibling.storage) T(*text);if(state==2)text->LockBuffer();
  auto pattern=formatText<C>("message");messageMode=output;messageFailure=failed;messageStatus=status;messageAlias=output==2;messageFreeError=freeError;messageOutput=messageAlias?const_cast<C*>(text->GetString()+1):nullptr;messageOriginal=pattern.c_str();
  f.first.failAllocate=f.second.failAllocate=failure==1;f.first.failReallocate=failure==2;events.clear();SetLastError(0x4321);
  sample(object,f,true,[&]()->INT64{RecordMessageError record;messageCall(mode,text,pattern.c_str(),42);return formatDigest(*text);});
  f.first.failAllocate=f.second.failAllocate=f.first.failReallocate=false;destroy(object);if(state==1)destroy(sibling);
 }
 for(int mode:{0,1,2}){
  Fixture f;Object object;operationConstruct<T>(object,f.first);messageMode=1;messageAlias=false;messageFreeError=false;messageFailure=true;messageStatus=87;messageOriginal=nullptr;messageOutput=nullptr;events.clear();SetLastError(0x4321);
  sample(object,f,true,[&]()->INT64{RecordMessageError record;messageCall(mode,operationText<T>(object),static_cast<const C*>(nullptr),42);return 0;});destroy(object);
 }
 messageAlias=messageFreeError=false;snapshotCharSize=1;
}
template<class T>void resourceFormats(){
 using E=StringFormatExports<T>;using C=typename T::XCHAR;snapshotCharSize=sizeof(C);
 for(int op:{0,1,2,3})for(int failed:{0,1,2,3,4,5,6})for(bool clone:{false,true})for(int allocation:{0,1,2})for(int conversion:{0,1,2,3}){
  if constexpr(sizeof(C)==2)if(conversion)continue;
  Fixture f;Object object;operationConstruct<T>(object,f.first);auto* text=operationText<T>(object);resourceText=op<2?L"value:%d":L"value:%1!d!";resourceID=35;resourceFailure=failed;conversionFailure=conversion;
  f.first.cloned=clone?&f.second:&f.first;f.first.failAllocate=f.second.failAllocate=allocation==1;f.first.failReallocate=allocation==2;events.clear();
  sample(object,f,true,[&]()->INT64{
   if(op==0)exactExport(E::FormatResourceRVA,E::FormatResource,text,UINT(35),42);
   if(op==1)exactExport(E::AppendFormatResourceRVA,E::AppendFormatResource,text,UINT(35),42);
   if(op==2)exactExport(E::MessageResourceRVA,E::MessageResource,text,UINT(35),42);
   if(op==3)exactExport(E::MessageWResourceRVA,E::MessageWResource,text,UINT(35),42);
   return formatDigest(*text);
  });f.first.failAllocate=f.second.failAllocate=f.first.failReallocate=false;destroy(object);
 }
 resourceFailure=conversionFailure=0;snapshotCharSize=1;
}
void stringFormatting(){
 StringCRTState state;_wsetlocale(LC_ALL,L"C");_setmbcp(1252);
 naturalPrintf<CStringA>();naturalPrintf<CString>();forcedPrintf<CStringA>();forcedPrintf<CString>();
 naturalMessages<CStringA>();naturalMessages<CString>();forcedMessages<CStringA>();forcedMessages<CString>();
 auto previous=g_fileExceptionFinder;g_fileExceptionFinder=&finder;
 {Patch a("FindResourceW",reinterpret_cast<void*>(&find)),b("FindResourceExW",reinterpret_cast<void*>(&findEx)),c("LoadResource",reinterpret_cast<void*>(&load)),d("LockResource",reinterpret_cast<void*>(&lock)),e("SizeofResource",reinterpret_cast<void*>(&size)),f("WideCharToMultiByte",reinterpret_cast<void*>(&convert));realConvert=reinterpret_cast<Converter>(f.old);
 resourceFormats<CStringA>();resourceFormats<CString>();}
 g_fileExceptionFinder=previous;
}
