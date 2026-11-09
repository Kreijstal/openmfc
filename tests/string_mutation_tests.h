// Included inside the ownership fixture namespace; canonical exports run against
// both the production implementation and the independently loaded retail DLL.
template<class T>void mutationConstruct(Object& object,Manager& manager,int kind) {
 using C=typename T::XCHAR;
 C input[]={C('a'),C('b'),C('a'),C('b'),C('a'),C('e'),C('f'),0};
 if(kind==1){::new(object.storage) T(&manager);return;}
 if(kind==3){input[2]=0;input[5]=0;}
 if(kind==4){input[0]=C(0x82);input[1]=C(0xa0);input[3]=C(0x82);input[4]=C(0xa2);}
 ::new(object.storage) T(input,kind==2?0:7,&manager);
}
template<class T>void basicStringMutation() {
 using C=typename T::XCHAR;using E=StringMutationExports<T>;snapshotCharSize=sizeof(C);
 const C empty[]={0},insert[]={C('X'),C('Y'),0};
 for(int kind:{0,1,2,3,4})for(int state:{0,1,2,3})for(bool spare:{false,true})for(bool clone:{false,true})for(int failure:{0,1,2})for(int op:{0,1,2,3,4})for(int query:{0,1,2,3}) {
  Fixture f;Object object,sibling;mutationConstruct<T>(object,f.first,kind);T* text=operationText<T>(object);
  if(spare)text->Preallocate(24);
  if(state==1)::new(sibling.storage) T(*text);
  if(state>=2){text->LockBuffer();if(state==3)text->LockBuffer();}
  f.first.cloned=clone?&f.second:&f.first;f.first.failAllocate=f.second.failAllocate=failure==1;f.first.failReallocate=failure==2;
  int index=query==0?-1:query==1?0:query==2?2:INT32_MAX;
  C value=query==0?0:query==1?C('a'):query==2?C(0x82):C(0xa0);
  const C* source=query==0?nullptr:query==1?empty:insert;
  events.clear();events.push_back({70,op,kind,state,query,spare});
  sample(object,f,true,[&]()->INT64 {
   errno=0;RecordStringErrno record;
   if(op==0)return exactExport(E::DeleteRVA,E::Delete,text,index,query==3?INT32_MAX:2);
   if(op==1)return exactExport(E::InsertCharRVA,E::InsertChar,text,index,value);
   if(op==2)return exactExport(E::InsertTextRVA,E::InsertText,text,index,source);
   if(op==3)return exactExport(E::RemoveRVA,E::Remove,text,value);
   return exactExport(E::ReplaceCharRVA,E::ReplaceChar,text,value,query==3?value:C('Z'));
  });
  f.first.failAllocate=f.second.failAllocate=f.first.failReallocate=false;
  destroy(object);if(state==1)destroy(sibling);
 }
 // Boundary arithmetic is checked even when an index lies beyond the data.
 for(int index:{INT32_MIN,-1,0,1,7,8,INT32_MAX})for(int count:{INT32_MIN,-1,0,1,7,8,INT32_MAX}) {
  Fixture f;Object object;mutationConstruct<T>(object,f.first,0);events.clear();
  sample(object,f,true,[&]()->INT64{return exactExport(E::DeleteRVA,E::Delete,operationText<T>(object),index,count);});destroy(object);
 }
 snapshotCharSize=1;
}
template<class T>void replaceStringMutation() {
 using C=typename T::XCHAR;using E=StringMutationExports<T>;snapshotCharSize=sizeof(C);
 const C empty[]={0},one[]={C('a'),0},two[]={C('a'),C('b'),0},missing[]={C('x'),0},large[]={C('X'),C('Y'),C('Z'),C('W'),0};
 for(int kind:{0,1,2,3,4})for(int state:{0,1,2})for(bool spare:{false,true})for(bool clone:{false,true})for(int failure:{0,1,2})for(const C* before:{static_cast<const C*>(nullptr),empty,one,two,missing})for(const C* after:{static_cast<const C*>(nullptr),empty,one,large}) {
  Fixture f;Object object,sibling;mutationConstruct<T>(object,f.first,kind);T* text=operationText<T>(object);
  if(spare)text->Preallocate(40);if(state==1)::new(sibling.storage) T(*text);if(state==2)text->LockBuffer();
  f.first.cloned=clone?&f.second:&f.first;f.first.failAllocate=f.second.failAllocate=failure==1;f.first.failReallocate=failure==2;
  events.clear();events.push_back({71,kind,state,spare,clone,failure});
  sample(object,f,true,[&]()->INT64 {errno=0;RecordStringErrno record;return exactExport(E::ReplaceTextRVA,E::ReplaceText,text,before,after);});
  f.first.failAllocate=f.second.failAllocate=f.first.failReallocate=false;destroy(object);if(state==1)destroy(sibling);
 }
 snapshotCharSize=1;
}
template<class T>void mutationCallbacks() {
 using C=typename T::XCHAR;using E=StringMutationExports<T>;snapshotCharSize=mutationWidth=sizeof(C);
 const C source[]={C('a'),C('b'),0},replacement[]={C('X'),C('Y'),C('Z'),0};
 for(int stage:{1,2,3})for(bool shared:{false,true})for(int op:{0,1,2,3,4,5}) {
  Fixture f;Object object,sibling;mutationConstruct<T>(object,f.first,0);T* text=operationText<T>(object);
  if(shared)::new(sibling.storage) T(*text);
  mutationTarget=object.get();mutationStage=stage;events.clear();events.push_back({72,stage,shared,op,0,0});
  sample(object,f,true,[&]()->INT64 {
   if(op==0)return exactExport(E::DeleteRVA,E::Delete,text,1,2);
   if(op==1)return exactExport(E::InsertCharRVA,E::InsertChar,text,2,C('X'));
   if(op==2)return exactExport(E::InsertTextRVA,E::InsertText,text,2,source);
   if(op==3)return exactExport(E::RemoveRVA,E::Remove,text,C('a'));
   if(op==4)return exactExport(E::ReplaceCharRVA,E::ReplaceChar,text,C('a'),C('X'));
   return exactExport(E::ReplaceTextRVA,E::ReplaceText,text,source,replacement);
  });mutationTarget=nullptr;mutationStage=0;destroy(object);if(shared)destroy(sibling);
 }
 mutationWidth=snapshotCharSize=1;
}
template<class T>void trimConstruct(Object& object,Manager& manager,int kind) {
 using C=typename T::XCHAR;
 C input[]={C(' '),C('\t'),C('a'),C('b'),C(' '),C('\v'),C('\f'),0};
 if(kind==1){::new(object.storage) T(&manager);return;}
 if(kind==3)input[4]=0;
 if(kind==4)for(int i=0;i<7;++i)input[i]=C(' ');
 if(kind==5){input[0]=C(0x82);input[1]=C(0xa0);input[2]=C('a');input[3]=C(' ');input[4]=C(0x82);input[5]=C(0xa0);input[6]=C(' ');}
 if(kind==6){input[0]=C('a');input[1]=C('a');input[2]=C('b');input[3]=C('a');input[4]=C('a');input[5]=0;input[6]=C('a');}
 if(kind==7){input[0]=C(0xa0);input[1]=C(0x3000);input[2]=C('a');input[3]=C(0x3000);input[4]=C(0xa0);input[5]=0;input[6]=C(' ');}
 ::new(object.storage) T(input,kind==2?0:7,&manager);
}
template<class T>void trimStringMutation() {
 using C=typename T::XCHAR;using E=StringMutationExports<T>;snapshotCharSize=sizeof(C);
 const C empty[]={0},space[]={C(' '),C('\t'),C('\v'),C('\f'),0},letter[]={C('a'),0},high[]={C(0x82),C(0xa0),C(' '),0};
 for(int kind:{0,1,2,3,4,5,6,7})for(int state:{0,1,2,3})for(bool clone:{false,true})for(int failure:{0,1})for(int op:{0,1,2,3,4,5,6,7,8})for(int query:{0,1,2}) {
  // No-argument trims have no query. Native TrimLeft(0) scans beyond a
  // terminator, so character targets here are nonzero and within its contract.
  if(op%3==0&&query)continue;
  Fixture f;Object object,sibling;trimConstruct<T>(object,f.first,kind);T* text=operationText<T>(object);
  if(state==1)::new(sibling.storage) T(*text);if(state>=2){text->LockBuffer();if(state==3)text->LockBuffer();}
  f.first.cloned=clone?&f.second:&f.first;f.first.failAllocate=f.second.failAllocate=failure!=0;
  const C* set=query==0?nullptr:query==1?empty:kind==6?letter:kind==5?high:space;
  C value=query==0?C(' '):query==1?C('a'):C(0x82);
  events.clear();events.push_back({73,kind,state,op,query,clone});
  sample(object,f,true,[&]()->INT64 {
   errno=0;RecordStringErrno record;T* result=nullptr;
   if(op==0)result=exactExport(E::TrimRVA,E::Trim,text);
   if(op==1)result=exactExport(E::TrimCharRVA,E::TrimChar,text,value);
   if(op==2)result=exactExport(E::TrimSetRVA,E::TrimSet,text,set);
   if(op==3)result=exactExport(E::TrimLeftRVA,E::TrimLeft,text);
   if(op==4)result=exactExport(E::TrimLeftCharRVA,E::TrimLeftChar,text,value);
   if(op==5)result=exactExport(E::TrimLeftSetRVA,E::TrimLeftSet,text,set);
   if(op==6)result=exactExport(E::TrimRightRVA,E::TrimRight,text);
   if(op==7)result=exactExport(E::TrimRightCharRVA,E::TrimRightChar,text,value);
   if(op==8)result=exactExport(E::TrimRightSetRVA,E::TrimRightSet,text,set);
   return result==text;
  });
  f.first.failAllocate=f.second.failAllocate=false;destroy(object);if(state==1)destroy(sibling);
 }
 snapshotCharSize=1;
}
template<class T>void trimMutationCallbacks() {
 using C=typename T::XCHAR;using E=StringMutationExports<T>;snapshotCharSize=mutationWidth=sizeof(C);
 const C input[]={C(' '),C(' '),C('a'),C('b'),C(' '),C(' '),0},set[]={C(' '),0};
 for(int stage:{1,2})for(int op:{0,1,2,3,4,5,6,7,8}) {
  Fixture f;Object object,sibling;::new(object.storage) T(input,6,&f.first);T* text=operationText<T>(object);::new(sibling.storage) T(*text);
  f.first.cloned=&f.second;mutationTarget=object.get();mutationStage=stage;events.clear();
  sample(object,f,true,[&]()->INT64 {
   T* result=nullptr;
   if(op==0)result=exactExport(E::TrimRVA,E::Trim,text);
   if(op==1)result=exactExport(E::TrimCharRVA,E::TrimChar,text,C(' '));
   if(op==2)result=exactExport(E::TrimSetRVA,E::TrimSet,text,set);
   if(op==3)result=exactExport(E::TrimLeftRVA,E::TrimLeft,text);
   if(op==4)result=exactExport(E::TrimLeftCharRVA,E::TrimLeftChar,text,C(' '));
   if(op==5)result=exactExport(E::TrimLeftSetRVA,E::TrimLeftSet,text,set);
   if(op==6)result=exactExport(E::TrimRightRVA,E::TrimRight,text);
   if(op==7)result=exactExport(E::TrimRightCharRVA,E::TrimRightChar,text,C(' '));
   if(op==8)result=exactExport(E::TrimRightSetRVA,E::TrimRightSet,text,set);
   return result==text;
  });mutationTarget=nullptr;mutationStage=0;destroy(object);destroy(sibling);
 }
 mutationWidth=snapshotCharSize=1;
}
void stringMutation() {
 StringCRTState state;_wsetlocale(LC_ALL,L"C");_setmbcp(1252);
 basicStringMutation<CStringW>();basicStringMutation<CStringA>();
 replaceStringMutation<CStringW>();replaceStringMutation<CStringA>();
 mutationCallbacks<CStringW>();mutationCallbacks<CStringA>();
 trimStringMutation<CStringW>();trimStringMutation<CStringA>();trimMutationCallbacks<CStringW>();trimMutationCallbacks<CStringA>();
 // ANSI removal drops whole multibyte spans; character replacement advances
 // after modifying the leading byte. Both continue past counted embedded NULs.
 _setmbcp(932);basicStringMutation<CStringA>();replaceStringMutation<CStringA>();trimStringMutation<CStringA>();
 if(_wsetlocale(LC_ALL,L"English_United States.1252")){_setmbcp(1252);trimStringMutation<CStringA>();trimStringMutation<CStringW>();}
}
