// Included inside the ownership fixture namespace; all exports use the native oracle.
void __cdecl stringInvalidParameter(const wchar_t* expression,const wchar_t* function,const wchar_t* file,unsigned line,uintptr_t reserved){
 events.push_back({50,expression!=nullptr,function!=nullptr,file!=nullptr,line,INT64(reserved)});
}
struct StringCRTState {
 _invalid_parameter_handler handler;std::wstring locale;int codepage;
 StringCRTState():handler(_set_invalid_parameter_handler(&stringInvalidParameter)),locale(_wsetlocale(LC_ALL,nullptr)),codepage(_getmbcp()){}
 ~StringCRTState(){_wsetlocale(LC_ALL,locale.c_str());_setmbcp(codepage);_set_invalid_parameter_handler(handler);}
};
struct RecordStringErrno {~RecordStringErrno(){events.push_back({51,errno,0,0,0,0});}};
// Some unchecked CRT entry points can fault on a null argument. Compare that
// boundary without replacing native faults with a successful API result.
int probeStringCRT(void* function,const void* self,const void* argument,DWORD* fault){
 *fault=0;
 __try{return reinterpret_cast<int(*)(const void*,const void*)>(function)(self,argument);}
 __except(EXCEPTION_EXECUTE_HANDLER){*fault=GetExceptionCode();return 0;}
}
template<class F,class T,class C>int uncheckedStringCRT(UINT_PTR rva,F source,const T* text,const C* argument){
 void* fn=image?static_cast<void*>(reinterpret_cast<BYTE*>(image)+rva):reinterpret_cast<void*>(source);
 DWORD fault=0;int value=probeStringCRT(fn,text,argument,&fault);events.push_back({52,fault,0,0,0,0});return value;
}
template<class T>void searchConstruct(Object& object,Manager& manager,int kind){
 using C=typename T::XCHAR;
 C text[8]={C('A'),C('b'),C('I'),C('a'),C('B'),C('c'),0,0};
 int length=6;
 if(kind==1){::new(object.storage) T(&manager);return;}
 if(kind==2)length=0;
 if(kind==3)text[2]=0;
 if(kind==4){
  if constexpr(sizeof(C)==1){text[0]=C(0x82);text[1]=C(0xa0);text[2]='a';text[3]=C(0xe9);text[4]='I';text[5]='z';}
  else{text[0]=C(0x3042);text[1]=C(0x3a9);text[2]='a';text[3]=C(0xe9);text[4]='I';text[5]='z';}
 }
 ::new(object.storage) T(text,length,&manager);
}
template<class T>void readStringOperations(){
 using E=StringSearchExports<T>;using C=typename T::XCHAR;snapshotCharSize=sizeof(C);
 const C empty[]={0},same[]={C('A'),C('b'),C('I'),C('a'),C('B'),C('c'),0},lower[]={C('a'),C('b'),C('i'),C('a'),C('b'),C('c'),0};
 const C part[]={C('B'),C('c'),0},other[]={C('z'),0},high[]={C(0xe9),C('I'),0};
 for(int kind:{0,1,2,3,4})for(int op:{0,1,2,3})for(const C* argument:{static_cast<const C*>(nullptr),empty,same,lower,part,high}){
  Fixture f;Object object;searchConstruct<T>(object,f.first,kind);auto* text=operationText<T>(object);events.clear();
  sample(object,f,true,[&]()->INT64{
   errno=0;RecordStringErrno record;
   if(op==0)return exactExport(E::CompareRVA,E::Compare,text,argument);
   if(op==1)return exactExport(E::CompareNoCaseRVA,E::CompareNoCase,text,argument);
   if(op==2)return uncheckedStringCRT(E::CollateRVA,E::Collate,text,argument);
   return uncheckedStringCRT(E::CollateNoCaseRVA,E::CollateNoCase,text,argument);
  });destroy(object);
 }
 for(int kind:{0,1,2,3,4})for(int op:{0,1,2,3})for(int start:{INT32_MIN,-1,0,1,2,5,6,INT32_MAX})for(int query:{0,1,2,3,4}){
  if(op>=2&&start!=0)continue;
  Fixture f;Object object;searchConstruct<T>(object,f.first,kind);auto* text=operationText<T>(object);
  const C* argument=query==0?nullptr:query==1?empty:query==2?part:query==3?other:high;
  C value=query==0?0:query==1?C('A'):query==2?C('b'):query==3?C(0xe9):C(0xa0);events.clear();
  sample(object,f,true,[&]()->INT64{
   errno=0;RecordStringErrno record;
   if(op==0)return exactExport(E::FindCharRVA,E::FindChar,text,value,start);
   if(op==1)return exactExport(E::FindTextRVA,E::FindText,text,argument,start);
   if(op==2)return uncheckedStringCRT(E::FindOneOfRVA,E::FindOneOf,text,argument);
   return exactExport(E::ReverseFindRVA,E::ReverseFind,text,value);
  });destroy(object);
 }
 snapshotCharSize=1;
}
template<class T>void caseStringOperations(){
 using E=StringSearchExports<T>;using C=typename T::XCHAR;snapshotCharSize=sizeof(C);
 for(int kind:{0,1,2,3,4})for(int state:{0,1,2,3})for(bool clone:{false,true})for(int failed:{0,1})for(bool extra:{false,true})for(int op:{0,1,2}){
  Fixture f;Object object,sibling;searchConstruct<T>(object,f.first,kind);auto* text=operationText<T>(object);
  if(extra)text->GetBuffer(24);
  if(state==1)::new(sibling.storage) T(*text);
  if(state>=2){text->LockBuffer();if(state==3)text->LockBuffer();}
  f.first.cloned=clone?&f.second:&f.first;f.first.failAllocate=f.second.failAllocate=failed;events.clear();
  sample(object,f,true,[&]()->INT64{
   if(op==0)return exactExport(E::MakeLowerRVA,E::MakeLower,text)==text;
   if(op==1)return exactExport(E::MakeUpperRVA,E::MakeUpper,text)==text;
   return exactExport(E::MakeReverseRVA,E::MakeReverse,text)==text;
  });
  f.first.failAllocate=f.second.failAllocate=false;destroy(object);if(state==1)destroy(sibling);
 }
 mutationWidth=sizeof(C);
 for(int stage:{1,2})for(bool shared:{false,true})for(int op:{0,1,2}){
  Fixture f;Object object,sibling;searchConstruct<T>(object,f.first,0);auto* text=operationText<T>(object);
  if(shared)::new(sibling.storage) T(*text);
  mutationTarget=object.get();mutationStage=stage;events.clear();
  sample(object,f,true,[&]()->INT64{
   if(op==0)return exactExport(E::MakeLowerRVA,E::MakeLower,text)==text;
   if(op==1)return exactExport(E::MakeUpperRVA,E::MakeUpper,text)==text;
   return exactExport(E::MakeReverseRVA,E::MakeReverse,text)==text;
  });mutationTarget=nullptr;mutationStage=0;destroy(object);if(shared)destroy(sibling);
 }
 mutationWidth=1;snapshotCharSize=1;
}
using ByteCaseCRT=int(__cdecl*)(unsigned char*,size_t);using WideCaseCRT=int(__cdecl*)(wchar_t*,size_t);
using ByteReverseCRT=unsigned char*(__cdecl*)(unsigned char*);using WideReverseCRT=wchar_t*(__cdecl*)(wchar_t*);
ByteCaseCRT actualByteCase;WideCaseCRT actualWideCase;ByteReverseCRT actualByteReverse;WideReverseCRT actualWideReverse;
int caseStatus,caseOperation;bool caseMutates;
template<class C>void recordCase(C* text,size_t size){
 auto* header=reinterpret_cast<CStringData*>(text)-1;
 events.push_back({53,sizeof(C),caseOperation,INT64(size),header->nDataLength,header->nRefs});
 if(caseMutates&&(size>1||(caseOperation==2&&header->nDataLength>1))){header->nDataLength=1;text[1]=0;}
}
int __cdecl byteCase(unsigned char* text,size_t size){recordCase(text,size);actualByteCase(text,size);return caseStatus;}
int __cdecl wideCase(wchar_t* text,size_t size){recordCase(text,size);actualWideCase(text,size);return caseStatus;}
unsigned char* __cdecl byteReverse(unsigned char* text){recordCase(text,0);actualByteReverse(text);return caseStatus?nullptr:text;}
wchar_t* __cdecl wideReverse(wchar_t* text){recordCase(text,0);actualWideReverse(text);return caseStatus?nullptr:text;}
template<class T>void forcedCaseOperations(){
 using E=StringSearchExports<T>;using C=typename T::XCHAR;snapshotCharSize=sizeof(C);
 for(int op:{0,1,2}){
  caseOperation=op;
  const char* name=sizeof(C)==1?(op==0?"_mbslwr_s":op==1?"_mbsupr_s":"_mbsrev"):(op==0?"_wcslwr_s":op==1?"_wcsupr_s":"_wcsrev");
  void* callback=sizeof(C)==1?(op==2?reinterpret_cast<void*>(&byteReverse):reinterpret_cast<void*>(&byteCase)):(op==2?reinterpret_cast<void*>(&wideReverse):reinterpret_cast<void*>(&wideCase));
  Patch patch(name,callback);
  if constexpr(sizeof(C)==1){actualByteCase=reinterpret_cast<ByteCaseCRT>(patch.old);actualByteReverse=reinterpret_cast<ByteReverseCRT>(patch.old);}
  else{actualWideCase=reinterpret_cast<WideCaseCRT>(patch.old);actualWideReverse=reinterpret_cast<WideReverseCRT>(patch.old);}
  for(int state:{0,1,2})for(bool extra:{false,true})for(bool mutateHeader:{false,true})for(int status:{-1,0,7,12,22,34,42,80}){
   Fixture f;Object object,sibling;searchConstruct<T>(object,f.first,3);auto* text=operationText<T>(object);
   if(extra)text->GetBuffer(24);
   if(state==1)::new(sibling.storage) T(*text);if(state==2)text->LockBuffer();
   caseStatus=status;caseMutates=mutateHeader;events.clear();
   sample(object,f,true,[&]()->INT64{
    if(op==0)return exactExport(E::MakeLowerRVA,E::MakeLower,text)==text;
    if(op==1)return exactExport(E::MakeUpperRVA,E::MakeUpper,text)==text;
    return exactExport(E::MakeReverseRVA,E::MakeReverse,text)==text;
   });destroy(object);if(state==1)destroy(sibling);
  }
 }
 caseStatus=0;caseMutates=false;snapshotCharSize=1;
}
void stringSearchAndCase(){
 StringCRTState state;
 struct LocaleCase{const wchar_t* name;int cp;};
 for(auto settings:{LocaleCase{L"C",1252},LocaleCase{L"en-US",1252},LocaleCase{L"de-DE",1252},LocaleCase{L"tr-TR",1254},LocaleCase{L"C",932},LocaleCase{L"C",65001}}){
  check(_wsetlocale(LC_ALL,settings.name)!=nullptr,"requested native comparison/case locale is available");
  check(_setmbcp(settings.cp)==0,"requested native multibyte codepage is available");
  readStringOperations<CStringA>();readStringOperations<CString>();
  caseStringOperations<CStringA>();caseStringOperations<CString>();
 }
 _wsetlocale(LC_ALL,L"C");_setmbcp(1252);
 forcedCaseOperations<CStringA>();forcedCaseOperations<CString>();
}
