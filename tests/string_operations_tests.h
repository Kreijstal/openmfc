// Included inside the ownership fixture namespace, using its native oracle.
template<class T>T* operationText(Object& object){return reinterpret_cast<T*>(object.storage);}
template<class T>void operationConstruct(Object& object,Manager& manager,int kind=0){
 using C=typename T::XCHAR;
 C text[]={C('a'),C('b'),0,C('c'),C('d'),C('e'),C('f'),0};
 if(kind==2)::new(object.storage) T(&manager);
 else ::new(object.storage) T(text,kind==1?0:7,&manager);
}
struct RecordStart {
 int& start;~RecordStart(){events.push_back({40,start,0,0,0,0});}
};
template<class T>void substringOperations(){
 using E=StringOperationExports<T>;using C=typename T::XCHAR;
 snapshotCharSize=sizeof(C);
 for(int kind:{0,1,2})for(int state:{0,1,2})for(bool clone:{false,true})for(bool fail:{false,true})for(int op:{0,1,2,3})for(int first:{INT32_MIN,-1,0,1,3,7,9,INT32_MAX}){
  Fixture f;Object source,result,sibling;operationConstruct<T>(source,f.first,kind);auto* text=operationText<T>(source);bool owned=false;
  if(state==1)::new(sibling.storage) T(*text);if(state==2)text->LockBuffer();
  f.first.cloned=clone?&f.second:&f.first;f.first.failAllocate=f.second.failAllocate=fail;events.clear();
  sample(result,f,false,[&]()->INT64{
   T* out=nullptr;
   if(op==0)out=exactExport(E::LeftRVA,E::Left,text,operationText<T>(result),first);
   if(op==1)out=exactExport(E::RightRVA,E::Right,text,operationText<T>(result),first);
   if(op==2)out=exactExport(E::MidRVA,E::Mid,text,operationText<T>(result),first);
   if(op==3)out=exactExport(E::MidCountRVA,E::MidCount,text,operationText<T>(result),first,3);
   safeRead=owned=true;return out==operationText<T>(result);
  });
  f.first.failAllocate=f.second.failAllocate=false;if(owned)destroy(result);destroy(source);if(state==1)destroy(sibling);
 }
 // Two-argument Mid clamps each negative argument to zero and checks addition.
 for(int first:{-1,0,1,7,9,INT32_MAX})for(int count:{INT32_MIN,-1,0,1,7,INT32_MAX}){
  Fixture f;Object source,result;operationConstruct<T>(source,f.first);bool owned=false;events.clear();
  sample(result,f,false,[&]()->INT64{auto* out=exactExport(E::MidCountRVA,E::MidCount,operationText<T>(source),operationText<T>(result),first,count);safeRead=owned=true;return out==operationText<T>(result);});
  if(owned)destroy(result);destroy(source);
 }
 const C empty[]={0},match[]={C('a'),C('b'),0},other[]={C('x'),0},all[]={C('a'),C('b'),C('c'),C('d'),C('e'),C('f'),0};
 for(int kind:{0,1,2})for(int state:{0,1,2})for(bool clone:{false,true})for(bool fail:{false,true})for(bool including:{false,true})for(const C* set:{static_cast<const C*>(nullptr),empty,match,other,all}){
  Fixture f;Object source,result;operationConstruct<T>(source,f.first,kind);auto* text=operationText<T>(source);bool owned=false;if(state)text->LockBuffer();if(state==2)text->LockBuffer();
  f.first.cloned=clone?&f.second:&f.first;f.first.failAllocate=f.second.failAllocate=fail;events.clear();
  sample(result,f,false,[&]()->INT64{
   T* out=including?exactExport(E::SpanIncludingRVA,E::SpanIncluding,text,operationText<T>(result),set):exactExport(E::SpanExcludingRVA,E::SpanExcluding,text,operationText<T>(result),set);
   safeRead=owned=true;return out==operationText<T>(result);
  });
  f.first.failAllocate=f.second.failAllocate=false;if(owned)destroy(result);destroy(source);
 }
 for(int kind:{0,1,2})for(bool clone:{false,true})for(bool fail:{false,true})for(int initial:{-1,0,1,2,3,6,7,9,INT32_MAX})for(const C* delimiters:{static_cast<const C*>(nullptr),empty,match,other,all}){
  Fixture f;Object source,result;operationConstruct<T>(source,f.first,kind);bool owned=false;int start=initial;
  f.first.cloned=clone?&f.second:&f.first;f.first.failAllocate=f.second.failAllocate=fail;events.clear();
  sample(result,f,false,[&]()->INT64{RecordStart record{start};auto* out=exactExport(E::TokenizeRVA,E::Tokenize,operationText<T>(source),operationText<T>(result),delimiters,&start);safeRead=owned=true;return out==operationText<T>(result);});
  f.first.failAllocate=f.second.failAllocate=false;if(owned)destroy(result);destroy(source);
 }
 // Clone/allocate callbacks can mutate the source while a result is created.
 mutationWidth=sizeof(C);
 for(int stage:{1,2})for(int op:{0,1,2,3,4}){
  Fixture f;Object source,result;operationConstruct<T>(source,f.first);auto* text=operationText<T>(source);bool owned=false;int start=0;
  mutationTarget=source.get();mutationStage=stage;events.clear();
  sample(result,f,false,[&]()->INT64{
   RecordStart record{start};T* out=nullptr;
   if(op==0)out=exactExport(E::LeftRVA,E::Left,text,operationText<T>(result),4);
   if(op==1)out=exactExport(E::RightRVA,E::Right,text,operationText<T>(result),4);
   if(op==2)out=exactExport(E::MidCountRVA,E::MidCount,text,operationText<T>(result),1,4);
   if(op==3)out=exactExport(E::MidRVA,E::Mid,text,operationText<T>(result),1);
   if(op==4)out=exactExport(E::TokenizeRVA,E::Tokenize,text,operationText<T>(result),empty,&start);
   safeRead=owned=true;return out==operationText<T>(result);
  });mutationTarget=nullptr;mutationStage=0;
  if(owned)destroy(result);destroy(source);
 }
 mutationWidth=1;snapshotCharSize=1;
}
template<class T>void substringFallbackOperations(){
 using E=StringOperationExports<T>;using C=typename T::XCHAR;snapshotCharSize=sizeof(C);
 const C set[]={C('a'),0};
 for(bool missingOwner:{false,true})for(int op:{0,1,2,3,4,5,6,7}){
  Fixture f;Object source,result;operationConstruct<T>(source,f.first);auto* text=operationText<T>(source);auto* data=source.get()->GetData();
  if(missingOwner)data->pStringMgr=nullptr;else f.first.cloned=nullptr;
  bool owned=false;int start=op==7?7:0;events.clear();
  sample(result,f,false,[&]()->INT64{
   RecordStart record{start};T* out=nullptr;
   if(op==0)out=exactExport(E::LeftRVA,E::Left,text,operationText<T>(result),1);
   if(op==1)out=exactExport(E::RightRVA,E::Right,text,operationText<T>(result),1);
   if(op==2)out=exactExport(E::MidRVA,E::Mid,text,operationText<T>(result),1);
   if(op==3)out=exactExport(E::MidCountRVA,E::MidCount,text,operationText<T>(result),1,1);
   if(op==4)out=exactExport(E::SpanIncludingRVA,E::SpanIncluding,text,operationText<T>(result),set);
   if(op==5)out=exactExport(E::SpanExcludingRVA,E::SpanExcluding,text,operationText<T>(result),set);
   if(op>=6)out=exactExport(E::TokenizeRVA,E::Tokenize,text,operationText<T>(result),set,&start);
   safeRead=owned=true;return out==operationText<T>(result);
  });
  data->pStringMgr=&f.first;if(owned)destroy(result);destroy(source);
 }
 snapshotCharSize=1;
}
template<class T>void derivedAppendOperations(){
 using E=StringOperationExports<T>;using C=typename T::XCHAR;snapshotCharSize=sizeof(C);
 for(int state:{0,1,2,3})for(bool clone:{false,true})for(int failure:{0,1,2})for(int op:{0,1,2,3,4})for(unsigned value:{0,65,128,233,255,0x3a9}){
  Fixture f;Object object,sibling;operationConstruct<T>(object,f.first);auto* text=operationText<T>(object);
  if(state==1)::new(sibling.storage) T(*text);if(state>=2){text->LockBuffer();if(state==3)text->LockBuffer();}
  f.first.cloned=clone?&f.second:&f.first;f.first.failAllocate=f.second.failAllocate=failure==1;f.first.failReallocate=failure==2;events.clear();
  sample(object,f,true,[&]()->INT64{
   if(op==0)return exactExport(E::AddCharRVA,E::AddChar,text,char(value))==text;
   if(op==1)return exactExport(E::AddByteRVA,E::AddByte,text,static_cast<unsigned char>(value))==text;
   if(op==2)return exactExport(E::AddWideCharRVA,E::AddWideChar,text,wchar_t(value))==text;
   if(op==3)return exactExport(E::AddStringRVA,E::AddString,text,static_cast<const ATL::CSimpleStringT<C,true>*>(text))==text;
   if constexpr(sizeof(C)==2)return exactExport(E::AddWideRVA,E::AddWide,text,text->GetString()+1)==text;
   else return exactExport(E::AddNarrowRVA,E::AddNarrow,text,text->GetString()+1)==text;
  });
  f.first.failAllocate=f.second.failAllocate=f.first.failReallocate=false;destroy(object);if(state==1)destroy(sibling);
 }
 for(int owner:{0,1,2})for(int cloned:{0,1,2}){
  Fixture f;Object object;operationConstruct<T>(object,f.first,owner==2?2:0);auto* text=operationText<T>(object);auto* data=object.get()->GetData();
  f.first.cloned=cloned==0?&f.first:cloned==1?&f.second:nullptr;if(owner==1)data->pStringMgr=nullptr;events.clear();
  sample(object,f,true,[&]()->INT64{return managerID(exactExport(E::GetManagerRVA,E::GetManager,text),f);});
  data->pStringMgr=&f.first;destroy(object);
 }
 snapshotCharSize=1;
}
void pointerAppendOperations(){
 Patch a("MultiByteToWideChar",reinterpret_cast<void*>(&wideConvert));realWideConvert=reinterpret_cast<WideConverter>(a.old);
 Patch b("WideCharToMultiByte",reinterpret_cast<void*>(&convert));realConvert=reinterpret_cast<Converter>(b.old);
 for(bool wideResult:{false,true})for(int state:{0,1,2})for(bool clone:{false,true})for(int fail:{0,1,2})for(int conversion:{0,1,2,3})for(bool nil:{false,true}){
  Fixture f;Object object,sibling;bool shared=state==1;
  if(wideResult){operationConstruct<CString>(object,f.first);if(shared)::new(sibling.storage) CString(*operationText<CString>(object));if(state==2)operationText<CString>(object)->LockBuffer();}
  else{operationConstruct<CStringA>(object,f.first);if(shared)::new(sibling.storage) CStringA(*object.string());if(state==2)object.string()->LockBuffer();}
  f.first.cloned=clone?&f.second:&f.first;f.first.failAllocate=f.second.failAllocate=fail==1;f.first.failReallocate=fail==2;
  wideConversionFailure=conversion;conversionFailure=conversion;snapshotCharSize=wideResult?2:1;events.clear();
  sample(object,f,true,[&]()->INT64{
   if(wideResult){using E=StringOperationExports<CString>;auto* text=operationText<CString>(object);return exactExport(E::AddNarrowRVA,E::AddNarrow,text,nil?nullptr:"A\xe9\0tail")==text;}
   using E=StringOperationExports<CStringA>;return exactExport(E::AddWideRVA,E::AddWide,object.string(),nil?nullptr:L"A\u03a9\0tail")==object.string();
  });
  f.first.failAllocate=f.second.failAllocate=f.first.failReallocate=false;destroy(object);if(shared)destroy(sibling);
 }
 wideConversionFailure=conversionFailure=0;snapshotCharSize=1;
}
void multibyteSubstringOperations(){
 using E=StringOperationExports<CStringA>;int previous=_getmbcp();
 for(int codepage:{932,65001}){
  _setmbcp(codepage);
  for(int op:{0,1,2})for(int start:{0,1,2,3,4,6}){
   Fixture f;Object source,result;const char sjis[]={char(0x82),char(0xa0),'a',char(0x82),char(0xa2),'b',0};const char utf8[]={char(0xc3),char(0xa9),'a',char(0xce),char(0xa9),'b',0};
   const char* data=codepage==932?sjis:utf8;char delimiters[]={data[0],data[1],0};::new(source.storage) CStringA(data,6,&f.first);bool owned=false;int at=start;events.clear();
   sample(result,f,false,[&]()->INT64{RecordStart record{at};CStringA* out=nullptr;
    if(op==0)out=exactExport(E::SpanIncludingRVA,E::SpanIncluding,source.string(),result.string(),delimiters);
    if(op==1)out=exactExport(E::SpanExcludingRVA,E::SpanExcluding,source.string(),result.string(),delimiters);
    if(op==2)out=exactExport(E::TokenizeRVA,E::Tokenize,source.string(),result.string(),delimiters,&at);
    safeRead=owned=true;return out==result.string();
   });if(owned)destroy(result);destroy(source);
  }
 }
 _setmbcp(previous);
}
void stringOperations(){
 derivedAppendOperations<CStringA>();derivedAppendOperations<CString>();pointerAppendOperations();
 substringOperations<CStringA>();substringOperations<CString>();multibyteSubstringOperations();
 substringFallbackOperations<CStringA>();substringFallbackOperations<CString>();
}
