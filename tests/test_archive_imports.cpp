#ifdef OPENMFC_USE_SDK_MFC
#define _AFX_NOFORCE_LIBS
#define _ATL_NO_DEFAULT_LIBS
#include <afxwin.h>
#include <afxcoll.h>
#include <afxstat_.h>
#else
#include "openmfc/afxwin.h"
#endif
#include <cstdio>
#include <initializer_list>
#include <cstring>
#include <type_traits>
static_assert(sizeof(CArchive)==112&&!std::is_polymorphic_v<CArchive>,"native archive ABI");
struct ImportRecord:CObject {
 DECLARE_SERIAL(ImportRecord)
 int value=19;
 void Serialize(CArchive& archive)override {
  if(archive.IsStoring())archive.Write(&value,sizeof(value));
  else archive.EnsureRead(&value,sizeof(value));
 }
};
IMPLEMENT_SERIAL(ImportRecord,CObject,42)
int main(){int failures=0;auto check=[&](bool value){if(!value)++failures;};
 check(CRuntimeClass::FromName("ImportRecord")==RUNTIME_CLASS(ImportRecord));
 check(CRuntimeClass::FromName(L"ImportRecord")==RUNTIME_CLASS(ImportRecord));
 check(CRuntimeClass::FromName("importrecord")==nullptr);
 check(CRuntimeClass::FromName(static_cast<const wchar_t*>(nullptr))==nullptr);
 auto* appModule=AfxGetAppModuleState();
 check(appModule!=nullptr);
#ifdef OPENMFC_USE_SDK_MFC
 check(AfxGetModuleState()==appModule);
 auto* alternate=new AFX_MODULE_STATE(FALSE,nullptr,0);
 auto* previous=AfxSetModuleState(alternate);
 check(AfxGetModuleState()==alternate && AfxGetAppModuleState()==appModule);
 check(CRuntimeClass::FromName("ImportRecord")==nullptr);
 AfxSetModuleState(previous);
 delete alternate;
 check(CRuntimeClass::FromName("ImportRecord")==RUNTIME_CLASS(ImportRecord));
#endif
 for(CObject* instance:{CRuntimeClass::CreateObject("ImportRecord"),CRuntimeClass::CreateObject(L"ImportRecord")}) {
  check(instance&&instance->GetRuntimeClass()==RUNTIME_CLASS(ImportRecord));delete instance;
 }
 check(CRuntimeClass::CreateObject("unregistered class")==nullptr);
 bool nullName=false;
 try{CRuntimeClass::FromName(static_cast<const char*>(nullptr));}
 catch(CInvalidArgException* error){nullName=true;error->Delete();}
 check(nullName);
 {
  CMemFile graph;ImportRecord record;
  {CArchive archive(&graph,CArchive::store,128);archive.WriteClass(RUNTIME_CLASS(ImportRecord));archive.WriteObject(&record);archive<<static_cast<const CObject*>(&record)<<static_cast<const CObject*>(nullptr);archive.Close();}
  graph.Seek(0,CFile::begin);WORD word=0;char name[13]{};int value=0;
  graph.Read(&word,2);check(word==0xffff);graph.Read(&word,2);check(word==42);graph.Read(&word,2);check(word==12);
  graph.Read(name,12);check(!strcmp(name,"ImportRecord"));graph.Read(&word,2);check(word==0x8001);
  graph.Read(&value,4);check(value==19);graph.Read(&word,2);check(word==2);graph.Read(&word,2);check(word==0&&graph.GetPosition()==graph.GetLength());
  graph.Seek(0,CFile::begin);
  {CArchive archive(&graph,CArchive::load,128);archive.SerializeClass(RUNTIME_CLASS(ImportRecord));
   check(archive.GetObjectSchema()==42);
   ImportRecord* loaded=nullptr;archive>>loaded;
   const CObject* repeated=nullptr;CObject* empty=loaded;archive>>repeated>>empty;
   check(loaded&&loaded->value==19&&repeated==loaded&&empty==nullptr);
   delete loaded;
  }
  CMemFile metadata;{CArchive archive(&metadata,CArchive::store,128);RUNTIME_CLASS(ImportRecord)->Store(archive);archive.Close();}
  check(metadata.GetLength()==16);
  metadata.Seek(0,CFile::begin);
  {CArchive archive(&metadata,CArchive::load,128);UINT schema=0;
   check(CRuntimeClass::Load(archive,nullptr)==nullptr);
   check(CRuntimeClass::Load(archive,&schema)==RUNTIME_CLASS(ImportRecord)&&schema==42);
  }
 }
 {
  auto* arrayType=CRuntimeClass::FromName("CObArray");
  check(arrayType==RUNTIME_CLASS(CObArray)&&arrayType->m_wSchema==0&&arrayType->m_pClassInit);
  CMemFile graph;CObArray cycle;cycle.Add(&cycle);cycle.Add(nullptr);cycle.Add(&cycle);
  {CArchive archive(&graph,CArchive::store,128);archive.WriteObject(&cycle);archive.Close();}
  graph.Seek(0,CFile::begin);
  {CArchive archive(&graph,CArchive::load,128);CObArray* array=nullptr;archive>>array;
   check(array&&array->GetSize()==3&&array->GetAt(0)==array&&array->GetAt(1)==nullptr&&array->GetAt(2)==array);
   delete array;
  }
 }
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
 file.SetFilePath(L"archive memory");
 {bool caught=false;try{file.SetFilePath(nullptr);}catch(CInvalidArgException* e){caught=true;e->Delete();}check(caught&&!wcscmp(file.GetFilePath(),L"archive memory"));}
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
