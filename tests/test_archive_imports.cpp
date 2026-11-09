#ifdef OPENMFC_USE_SDK_MFC
#define _AFX_NOFORCE_LIBS
#include <afx.h>
#include <afxcoll.h>
#else
#include "openmfc/afxwin.h"
#endif
#include <cstdio>
#include <initializer_list>
#include <cstring>
#include <type_traits>
static_assert(sizeof(CArchive)==112&&!std::is_polymorphic_v<CArchive>,"native archive ABI");
int main(){int failures=0;auto check=[&](bool value){if(!value)++failures;};
 {
  static_assert(sizeof(CPtrArray)==40,"native pointer array ABI");
  CPtrArray a,b;check(a.IsEmpty()&&a.GetUpperBound()==-1);
  a.SetSize(1,17);INT_PTR capacity=0;memcpy(&capacity,reinterpret_cast<const char*>(&a)+24,8);check(capacity==1);
  void* one=reinterpret_cast<void*>(UINT_PTR(0x1234));void* two=reinterpret_cast<void*>(UINT_PTR(0x5678));
  a.SetAt(0,one);check(a.Add(two)==1&&a.GetCount()==2&&a[0]==one&&a[1]==two);
  a.InsertAt(1,one,2);check(a.GetSize()==4&&a[3]==two);a.RemoveAt(1,2);
  b.Copy(a);check(b.GetSize()==2&&b.GetAt(0)==one);check(b.Append(a)==2&&b.GetSize()==4);
  a.InsertAt(1,&b);check(a.GetSize()==6&&a[5]==two);a.SetAtGrow(9,two);check(a[9]==two&&a[8]==nullptr);
  a.FreeExtra();memcpy(&capacity,reinterpret_cast<const char*>(&a)+24,8);check(capacity==a.GetSize());
  check(!strcmp(a.GetRuntimeClass()->m_lpszClassName,"CPtrArray")&&a.GetRuntimeClass()==CPtrArray::GetThisClass());
  bool caught=false;try{a.Append(a);}catch(CInvalidArgException* e){caught=true;e->Delete();}check(caught);
  a.RemoveAll();check(a.GetData()==nullptr&&a.GetSize()==0);
 }
 CMemFile file;
 {
  CArchive store(&file,CArchive::store,128);check(store.IsStoring()&&!store.IsLoading()&&!store.IsByteSwapping());
  store.MapObject(nullptr);store.MapObject(&file);store.EnsureSchemaMapExists(nullptr);store.EnsureSchemaMapExists(nullptr);
  const char input[]="archive ABI";store.Write(input,sizeof(input));
  for(ULONGLONG n:{0ull,65535ull,0xffffffffull,0x123456789abcdef0ull})store.WriteCount(n);
  for(DWORD i=0;i<73;++i) {
   DWORD value=0x12340000+i;
#ifdef OPENMFC_USE_SDK_MFC
   store<<value;
#else
   store.Write(&value,sizeof(value));
#endif
  }
  store.Flush();check(file.GetLength()>sizeof(input));store.Close();check(store.GetFile()==nullptr);
 }
 file.Seek(0,CFile::begin);
 {
  CArchive load(&file,CArchive::load,128);char output[12]={};load.EnsureRead(output,sizeof(output));check(!strcmp(output,"archive ABI"));
  load.MapObject(nullptr);load.MapObject(&file);load.EnsureSchemaMapExists(nullptr);
  for(ULONGLONG n:{0ull,65535ull,0xffffffffull,0x123456789abcdef0ull})check(load.ReadCount()==n);
  for(DWORD i=0;i<73;++i) {
   DWORD value=0;
#ifdef OPENMFC_USE_SDK_MFC
   load>>value;
#else
   load.EnsureRead(&value,sizeof(value));
#endif
   check(value==0x12340000+i);
  }
  load.SetObjectSchema(19);check(load.GetObjectSchema()==19&&load.GetObjectSchema()==UINT(-1));
  bool caught=false;try{load.EnsureRead(output,1);}catch(CArchiveException* e){caught=e->m_cause==CArchiveException::endOfFile;e->Delete();}check(caught);
 }
 bool caught=false;try{CArchive invalid(nullptr,0);}catch(CInvalidArgException* e){caught=true;e->Delete();}check(caught);
 printf("Archive DLL imports: %d failures\n",failures);return failures?1:0;
}
