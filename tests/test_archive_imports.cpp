#ifdef OPENMFC_USE_SDK_MFC
#define _AFX_NOFORCE_LIBS
#include <afx.h>
#else
#include "openmfc/afxwin.h"
#endif
#include <cstdio>
#include <initializer_list>
#include <cstring>
#include <type_traits>
static_assert(sizeof(CArchive)==112&&!std::is_polymorphic_v<CArchive>,"native archive ABI");
int main(){int failures=0;auto check=[&](bool value){if(!value)++failures;};
 CMemFile file;
 {
  CArchive store(&file,CArchive::store,128);check(store.IsStoring()&&!store.IsLoading()&&!store.IsByteSwapping());
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
