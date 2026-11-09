// Actual framework exceptions with an independent native DLL oracle.
#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxwin.h"
#include <cstdio>
#include <cstring>
#include <array>
#include <vector>
#include <string>
#include <type_traits>
#include <cstdlib>
#include <cerrno>
extern "C" CException* impl___0CException__QEAA_XZ(CException* p);
extern "C" CException* impl___0CException__QEAA_H_Z(CException* p,BOOL value);
extern "C" int impl__GetErrorMessage_CException__UEBAHPEA_WIPEAI_Z(const CException* p,wchar_t* o,UINT n,UINT* h);
extern "C" int impl__GetErrorMessage_CException__UEAAHPEA_WIPEAI_Z(CException* p,wchar_t* o,UINT n,UINT* h);
extern "C" int impl__ReportError_CException__UEAAHII_Z(CException* p,UINT t,UINT m);
extern "C" void impl__Delete_CException__QEAAXXZ(CException* p);
extern "C" CRuntimeClass* impl__GetThisClass_CException__SAPEAUCRuntimeClass__XZ();
extern "C" CRuntimeClass* impl__GetRuntimeClass_CException__UEBAPEAUCRuntimeClass__XZ(const CException*);
extern "C" int impl__GetErrorMessage_CSimpleException__UEBAHPEA_WIPEAI_Z(const CSimpleException* self,wchar_t* out,UINT capacity,UINT* help);
extern "C" CRuntimeClass* impl__GetRuntimeClass_CSimpleException__UEBAPEAUCRuntimeClass__XZ(const CSimpleException*);
extern "C" CRuntimeClass* impl__GetThisClass_CSimpleException__SAPEAUCRuntimeClass__XZ();
extern "C" void impl__InitString_CSimpleException__QEAAXXZ(CSimpleException* self);
extern "C" void impl__AfxThrowMemoryException__YAXXZ();
extern "C" CRuntimeClass* impl__GetThisClass_CMemoryException__SAPEAUCRuntimeClass__XZ();
extern "C" CRuntimeClass* impl__GetRuntimeClass_CMemoryException__UEBAPEAUCRuntimeClass__XZ(const CMemoryException*);
extern "C" void impl__AfxThrowNotSupportedException__YAXXZ();
extern "C" CRuntimeClass* impl__GetThisClass_CNotSupportedException__SAPEAUCRuntimeClass__XZ();
extern "C" CRuntimeClass* impl__GetRuntimeClass_CNotSupportedException__UEBAPEAUCRuntimeClass__XZ(const CNotSupportedException*);
extern "C" void impl__AfxThrowInvalidArgException__YAXXZ();
extern "C" CRuntimeClass* impl__GetThisClass_CInvalidArgException__SAPEAUCRuntimeClass__XZ();
extern "C" CRuntimeClass* impl__GetRuntimeClass_CInvalidArgException__UEBAPEAUCRuntimeClass__XZ(const CInvalidArgException*);
extern "C" void impl__AfxThrowResourceException__YAXXZ();
extern "C" CRuntimeClass* impl__GetThisClass_CResourceException__SAPEAUCRuntimeClass__XZ();
extern "C" CRuntimeClass* impl__GetRuntimeClass_CResourceException__UEBAPEAUCRuntimeClass__XZ(const CResourceException*);
extern "C" void impl__AfxThrowUserException__YAXXZ();
extern "C" CRuntimeClass* impl__GetThisClass_CUserException__SAPEAUCRuntimeClass__XZ();
extern "C" CRuntimeClass* impl__GetRuntimeClass_CUserException__UEBAPEAUCRuntimeClass__XZ(const CUserException*);
extern "C" int impl__AfxLoadString__YAHIPEA_WI_Z(UINT id,wchar_t* output,UINT capacity);
struct API {
    CException* (*ctor)(CException*,BOOL);
    CException* (*defaultCtor)(CException*);
    void (*remove)(CException*);
    int (*message)(const CException*,wchar_t*,UINT,UINT*);
    int (*forward)(CException*,wchar_t*,UINT,UINT*);
    int (*report)(CException*,UINT,UINT);
    int (*simpleMessage)(const CSimpleException*,wchar_t*,UINT,UINT*);
    void (*init)(CSimpleException*);
    int (*load)(UINT,wchar_t*,UINT);
    void (*throws[5])();
    CRuntimeClass* (*types[7])();
    HMODULE image;
};
API sourceAPI(){API a{};
 a.ctor=&impl___0CException__QEAA_H_Z;
 a.defaultCtor=&impl___0CException__QEAA_XZ;
 a.remove=&impl__Delete_CException__QEAAXXZ;
 a.message=&impl__GetErrorMessage_CException__UEBAHPEA_WIPEAI_Z;
 a.forward=&impl__GetErrorMessage_CException__UEAAHPEA_WIPEAI_Z;
 a.report=&impl__ReportError_CException__UEAAHII_Z;
 a.simpleMessage=&impl__GetErrorMessage_CSimpleException__UEBAHPEA_WIPEAI_Z;
 a.init=&impl__InitString_CSimpleException__QEAAXXZ;
 a.load=&impl__AfxLoadString__YAHIPEA_WI_Z;
 a.types[0]=&impl__GetThisClass_CException__SAPEAUCRuntimeClass__XZ;
 a.types[1]=&impl__GetThisClass_CSimpleException__SAPEAUCRuntimeClass__XZ;
 a.types[2]=&impl__GetThisClass_CMemoryException__SAPEAUCRuntimeClass__XZ;
 a.types[3]=&impl__GetThisClass_CNotSupportedException__SAPEAUCRuntimeClass__XZ;
 a.types[4]=&impl__GetThisClass_CInvalidArgException__SAPEAUCRuntimeClass__XZ;
 a.types[5]=&impl__GetThisClass_CResourceException__SAPEAUCRuntimeClass__XZ;
 a.types[6]=&impl__GetThisClass_CUserException__SAPEAUCRuntimeClass__XZ;
 a.throws[0]=&impl__AfxThrowMemoryException__YAXXZ;
 a.throws[1]=&impl__AfxThrowNotSupportedException__YAXXZ;
 a.throws[2]=&impl__AfxThrowInvalidArgException__YAXXZ;
 a.throws[3]=&impl__AfxThrowResourceException__YAXXZ;
 a.throws[4]=&impl__AfxThrowUserException__YAXXZ;
return a;}
API nativeAPI(HMODULE image){API a{};a.image=image;auto* b=reinterpret_cast<BYTE*>(image);
 a.ctor=reinterpret_cast<std::remove_reference_t<decltype(a.ctor)>>(b+0x227360);
 a.defaultCtor=reinterpret_cast<std::remove_reference_t<decltype(a.defaultCtor)>>(b+0x227350);
 a.remove=reinterpret_cast<std::remove_reference_t<decltype(a.remove)>>(b+0x227370);
 a.message=reinterpret_cast<std::remove_reference_t<decltype(a.message)>>(b+0x2273a0);
 a.forward=reinterpret_cast<std::remove_reference_t<decltype(a.forward)>>(b+0x2273c0);
 a.report=reinterpret_cast<std::remove_reference_t<decltype(a.report)>>(b+0x2273e0);
 a.simpleMessage=reinterpret_cast<std::remove_reference_t<decltype(a.simpleMessage)>>(b+0x227630);
 a.init=reinterpret_cast<std::remove_reference_t<decltype(a.init)>>(b+0x2275f0);
 a.load=reinterpret_cast<std::remove_reference_t<decltype(a.load)>>(b+0x2af0b0);
 a.types[0]=reinterpret_cast<std::remove_reference_t<decltype(a.types[0])>>(b+0x1d3660);
 a.types[1]=reinterpret_cast<std::remove_reference_t<decltype(a.types[1])>>(b+0x2275b0);
 a.types[2]=reinterpret_cast<std::remove_reference_t<decltype(a.types[2])>>(b+0x2275c0);
 a.types[3]=reinterpret_cast<std::remove_reference_t<decltype(a.types[3])>>(b+0x2275d0);
 a.types[4]=reinterpret_cast<std::remove_reference_t<decltype(a.types[4])>>(b+0x2275e0);
 a.types[5]=reinterpret_cast<std::remove_reference_t<decltype(a.types[5])>>(b+0x2a41b0);
 a.types[6]=reinterpret_cast<std::remove_reference_t<decltype(a.types[6])>>(b+0x2a41c0);
 a.throws[0]=reinterpret_cast<std::remove_reference_t<decltype(a.throws[0])>>(b+0x2276c0);
 a.throws[1]=reinterpret_cast<std::remove_reference_t<decltype(a.throws[1])>>(b+0x2276f0);
 a.throws[2]=reinterpret_cast<std::remove_reference_t<decltype(a.throws[2])>>(b+0x227720);
 a.throws[3]=reinterpret_cast<std::remove_reference_t<decltype(a.throws[3])>>(b+0x2a42e0);
 a.throws[4]=reinterpret_cast<std::remove_reference_t<decltype(a.throws[4])>>(b+0x2a4310);
return a;}
const API* api;int failures;
void check(bool condition,const char* message){if(!condition){++failures;printf("FAIL: %s\n",message);}}
template<class T>T& field(void* object,size_t offset){return *reinterpret_cast<T*>(static_cast<BYTE*>(object)+offset);}
struct Hook {
    BYTE* target;BYTE old[12];DWORD protection;
    Hook(size_t rva,void* fn):target(reinterpret_cast<BYTE*>(api->image)+rva){memcpy(old,target,12);VirtualProtect(target,12,PAGE_EXECUTE_READWRITE,&protection);BYTE jump[12]={0x48,0xb8};memcpy(jump+2,&fn,8);jump[10]=0xff;jump[11]=0xe0;memcpy(target,jump,12);FlushInstructionCache(GetCurrentProcess(),target,12);}
    ~Hook(){memcpy(target,old,12);FlushInstructionCache(GetCurrentProcess(),target,12);DWORD ignored;VirtualProtect(target,12,protection,&ignored);}
};
void** imported(HMODULE image,const char* wanted){auto* b=reinterpret_cast<BYTE*>(image);auto* pe=reinterpret_cast<IMAGE_NT_HEADERS*>(b+reinterpret_cast<IMAGE_DOS_HEADER*>(b)->e_lfanew);auto* d=reinterpret_cast<IMAGE_IMPORT_DESCRIPTOR*>(b+pe->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress);
    for(;d->Name;++d){auto* n=reinterpret_cast<IMAGE_THUNK_DATA*>(b+d->OriginalFirstThunk);auto* v=reinterpret_cast<IMAGE_THUNK_DATA*>(b+d->FirstThunk);for(;n->u1.AddressOfData;++n,++v)if(!IMAGE_SNAP_BY_ORDINAL(n->u1.Ordinal)){auto* name=reinterpret_cast<IMAGE_IMPORT_BY_NAME*>(b+n->u1.AddressOfData);if(!strcmp(reinterpret_cast<char*>(name->Name),wanted))return reinterpret_cast<void**>(&v->u1.Function);}}return nullptr;}
struct Patch {void** slot;void* old;DWORD protection;Patch(const char* name,void* fn){slot=imported(api->image?api->image:GetModuleHandleW(nullptr),name);check(slot!=nullptr,name);if(!slot)abort();old=*slot;VirtualProtect(slot,8,PAGE_READWRITE,&protection);*slot=fn;}~Patch(){*slot=old;DWORD ignored;VirtualProtect(slot,8,protection,&ignored);}};
std::vector<int> events;UINT expectedResource;CException* initializing;int resourceMode;bool reenter;
std::array<WORD,128> resourceBlock;UINT resourceSize;
extern "C" void* impl__AfxFindStringResourceHandle__YAPEAUHINSTANCE____I_Z(UINT id){events.push_back(1);check(id==expectedResource,"resource finder receives native resource ID");if(initializing){check(field<BOOL>(initializing,20)==TRUE,"InitString marks initialized before entering resource service");if(reenter){wchar_t out[8]={L'x'};UINT help=7;check(initializing->GetErrorMessage(out,8,&help)==FALSE&&!out[0]&&!help,"reentrant message lookup observes the initialized flag and avoids loading again");}}
    if(resourceMode==2)throw 37;return resourceMode==1?nullptr:reinterpret_cast<void*>(0x44);}
HRSRC WINAPI findResource(HMODULE module,LPCWSTR name,LPCWSTR type){events.push_back(2);check(module==reinterpret_cast<HMODULE>(0x44)&&UINT_PTR(name)==WORD((expectedResource>>4)+1)&&UINT_PTR(type)==6,"resource image uses native STRING block and low WORD block ID");return reinterpret_cast<HRSRC>(0x55);}
HGLOBAL WINAPI loadResource(HMODULE module,HRSRC resource){events.push_back(3);check(module==reinterpret_cast<HMODULE>(0x44)&&resource==reinterpret_cast<HRSRC>(0x55),"resource image loads the located block");return reinterpret_cast<HGLOBAL>(0x66);}
void* WINAPI lockResource(HGLOBAL resource){events.push_back(4);check(resource==reinterpret_cast<HGLOBAL>(0x66),"resource image locks loaded bytes");return resourceBlock.data();}
DWORD WINAPI sizeofResource(HMODULE module,HRSRC resource){events.push_back(5);check(module==reinterpret_cast<HMODULE>(0x44)&&resource==reinterpret_cast<HRSRC>(0x55),"resource bounds use the located block");return resourceSize;}
void stringBlock(){resourceBlock.fill(0);size_t offset=0;for(int slot=0;slot<16;++slot){resourceBlock[offset++]=5;for(WORD value:{WORD('A'),WORD('B'),WORD(0),WORD('C'),WORD('D')})resourceBlock[offset++]=value;}resourceSize=UINT(offset*2);}
UINT boxMessage,boxType,boxHelp;std::wstring boxText;bool textBox;
extern "C" int impl__AfxMessageBox__YAHPEB_WII_Z(const wchar_t* text,UINT type,UINT help){textBox=true;boxText=text;boxType=type;boxHelp=help;return 23;}
extern "C" int impl__AfxMessageBox__YAHIII_Z(UINT message,UINT type,UINT help){textBox=false;boxMessage=message;boxType=type;boxHelp=help;return 29;}
struct ClientError:CException {int* deleted;int mode=0;ClientError(BOOL owned,int* count=nullptr):CException(owned),deleted(count){}~ClientError()override{if(deleted)++*deleted;}int GetErrorMessage(wchar_t* out,UINT size,UINT* help)override{check(size==512,"ReportError selects non-const virtual message slot with 512 characters");if(mode==2)throw 42;if(help)*help=91;if(size)wcscpy_s(out,size,L"client text");return mode;}};
struct ClientSimple:CSimpleException {using CSimpleException::CSimpleException;~ClientSimple()override=default;};
void base(){
    for(BOOL value:{-1,0,1,7}){alignas(8) BYTE bytes[32];memset(bytes,0xa5,sizeof(bytes));auto* p=reinterpret_cast<CException*>(bytes+8);check(api->ctor(p,value)==p&&field<BOOL>(p,8)==value,"base constructor returns the supplied subobject and writes raw BOOL");for(size_t i=0;i<sizeof(bytes);++i)if(i<16||i>=20)check(bytes[i]==0xa5,"abstract NOVTABLE constructor preserves root, padding and guards");}
    alignas(8) BYTE bytes[16];memset(bytes,0xa5,sizeof(bytes));api->defaultCtor(reinterpret_cast<CException*>(bytes));check(field<BOOL>(bytes,8)==TRUE&&field<DWORD>(bytes,12)==0xa5a5a5a5,"default base constructor preserves tail padding");
    ClientError error(FALSE);wchar_t text[8];UINT help=7;
    for(UINT size:{0u,1u,8u})for(bool output:{false,true}){std::fill(std::begin(text),std::end(text),L'x');help=7;check(api->message(&error,output?text:nullptr,size,&help)==FALSE&&!help&&text[0]==(output&&size?0:L'x'),"base const message clears only its native outputs");}
    check(api->message(nullptr,nullptr,0,nullptr)==FALSE,"base const message does not inspect this");
    void* root[8];memcpy(root,*reinterpret_cast<void***>(&error),sizeof(root));
    // A canonical non-const call forwards to the const slot, bypassing a client
    // non-const override; ReportError deliberately selects that override.
    text[0]=L'x';help=7;check(api->forward(&error,text,8,&help)==FALSE&&!text[0]&&!help,"canonical non-const message forwards to virtual const slot");
    for(int mode:{0,1,2})for(UINT message:{0u,57u})for(UINT type:{0u,0x1234u}){error.mode=mode;bool escaped=false;int result=0;try{result=api->report(&error,type,message);}catch(int code){escaped=code==42;}check(mode==2?escaped:result==(mode?23:29),"ReportError preserves callback exceptions and returns the UI service result");if(mode!=2)check(boxType==type&&boxHelp==91&&(mode?textBox&&boxText==L"client text":!textBox&&boxMessage==(message?message:0xf020)),"ReportError selects text/resource fallback and forwards type/help unchanged");}
    for(BOOL value:{-1,0,1,7}){int deleted=0;auto* p=new ClientError(value,&deleted);api->remove(p);check(deleted==int(value>0),"Delete uses only positive base auto-delete values");if(!deleted)delete p;}
}
template<class T>CException* catchAs(void (*thrower)()){try{thrower();}catch(T* error){return reinterpret_cast<CException*>(const_cast<void*>(static_cast<const void*>(error)));}catch(...){check(false,"compiler-generated exception hierarchy catch");}return nullptr;}
void factories(){
    const char* names[]={"CMemoryException","CNotSupportedException","CInvalidArgException","CResourceException","CUserException"};UINT ids[]={0xf023,0xf021,0xf025,0xf022,0xf024};
    for(int i=0;i<5;++i){CException* p=catchAs<CException>(api->throws[i]);check(p&&p->GetRuntimeClass()==api->types[i+2]()&&!p->m_bAutoDelete,"factory throws actual shared exception of the native runtime type");check(field<UINT>(p,284)==ids[i]&&sizeof(CMemoryException)==288,"shared exception has native message resource and size");
        auto* desc=p->GetRuntimeClass();check(desc->m_nObjectSize==288&&!strcmp(desc->m_lpszClassName,names[i])&&desc->m_pfnGetBaseClass()==api->types[1]()&&!desc->m_pfnCreateObject&&desc->m_wSchema==0xffff,"runtime metadata describes the real simple-exception class and ancestry");
        auto repeat=catchAs<CSimpleException>(api->throws[i]);check(repeat==p&&catchAs<CObject>(api->throws[i])==p&&catchAs<void>(api->throws[i])==p&&catchAs<const CSimpleException>(api->throws[i])==p,"shared pointer survives repeated throws and catches as every native base/void pointer");api->remove(p);check(catchAs<CException>(api->throws[i])==p,"Delete retains shared resource-critical exceptions");
        // Restore the native shared cache to test lazy resource behavior below.
        memset(reinterpret_cast<BYTE*>(p)+8,0,276);field<UINT>(p,284)=ids[i];
    }
    check(catchAs<CMemoryException>(api->throws[0])&&catchAs<CNotSupportedException>(api->throws[1])&&catchAs<CInvalidArgException>(api->throws[2])&&catchAs<CResourceException>(api->throws[3])&&catchAs<CUserException>(api->throws[4]),"factories support typed catches without manually fabricated throw metadata");
    check(api->types[1]()->m_pfnGetBaseClass()==api->types[0]()&&api->types[0]()->m_nObjectSize==16,"simple runtime metadata chains through native exception base");
}
void constructors(){
    alignas(8) BYTE bytes[288];
#define CONSTRUCT(C) for(int kind:{0,1,2}){memset(bytes,0xa5,sizeof(bytes));auto* p=kind==0?new(bytes) C:kind==1?new(bytes) C(-1):new(bytes) C(-1,77);check(p->CException::m_bAutoDelete==(kind? -1:TRUE)&&field<DWORD>(p,12)==0xa5a5a5a5&&field<DWORD>(p,16)==0xa5a5a5a5&&!field<BOOL>(p,20)&&!field<BOOL>(p,24)&&field<WORD>(p,28)==0xa5a5,"SDK constructor writes only base ownership and cache flags");check(field<UINT>(p,284)==(kind==0?0xa5a5a5a5u:kind==2?77u:resource),"default constructor preserves resource field; BOOL constructor selects class resource");p->~C();}
    {UINT resource=0xf023;CONSTRUCT(CMemoryException)}{UINT resource=0xf021;CONSTRUCT(CNotSupportedException)}{UINT resource=0xf025;CONSTRUCT(CInvalidArgException)}{UINT resource=0xf022;CONSTRUCT(CResourceException)}{UINT resource=0xf024;CONSTRUCT(CUserException)}
#undef CONSTRUCT
}
void resources(){
    stringBlock();Patch a("FindResourceW",reinterpret_cast<void*>(&findResource)),b("LoadResource",reinterpret_cast<void*>(&loadResource)),c("LockResource",reinterpret_cast<void*>(&lockResource)),d("SizeofResource",reinterpret_cast<void*>(&sizeofResource));
    for(UINT capacity:{1u,2u,4u,8u,100u})for(int mode:{0,1}){expectedResource=0xf023;resourceMode=mode;initializing=nullptr;events.clear();wchar_t output[100];std::fill(std::begin(output),std::end(output),L'x');int result=api->load(expectedResource,output,capacity);UINT length=(std::min)(capacity-1,5u);check(result==(mode?0:int(length))&&output[mode?0:length]==0&&events==(mode?std::vector<int>{1}:std::vector<int>{1,2,3,4,5}),"string loading uses raw resource image length, truncation and native OS query order");if(!mode)check(!memcmp(output,L"AB\0CD",length*2),"string loader copies embedded NUL characters by resource length");}
    for(int type=0;type<5;++type)for(int mode:{0,1,2}){
        auto* p=catchAs<CException>(api->throws[type]);field<BOOL>(p,20)=FALSE;field<BOOL>(p,24)=FALSE;initializing=p;expectedResource=field<UINT>(p,284);resourceMode=mode;reenter=true;events.clear();wchar_t output[8];std::fill(std::begin(output),std::end(output),L'x');UINT help=77;bool escaped=false;int result=0;try{result=api->simpleMessage(static_cast<CSimpleException*>(p),output,8,&help);}catch(int code){escaped=code==37;}
        check(!help&&field<BOOL>(p,20)==TRUE&&field<BOOL>(p,24)==(mode?FALSE:TRUE),"simple cache commits initialized before loading and loaded only after a normal return");check(mode==2?escaped:result==(mode?FALSE:TRUE)&&!wcscmp(output,mode?L"":L"AB"),"simple cache preserves failed-load/exception output behavior");events.clear();help=77;api->simpleMessage(static_cast<CSimpleException*>(p),output,8,&help);check(events.empty()&&!help,"message cache does not retry failed or interrupted initialization");
    }
    initializing=nullptr;resourceMode=0;reenter=false;
    auto* p=static_cast<CSimpleException*>(catchAs<CException>(api->throws[0]));field<BOOL>(p,20)=FALSE;events.clear();UINT help=77;check(!api->simpleMessage(p,nullptr,8,&help)&&help==77&&events.empty()&&!field<BOOL>(p,20),"invalid simple-message output leaves help/cache untouched");wchar_t output[8]={L'x'};check(!api->simpleMessage(p,output,0,&help)&&help==77&&output[0]==L'x'&&events.empty(),"zero-capacity simple message returns before all side effects");
}
void run(){printf("%s framework exceptions\n",api->image?"Native":"Source");base();factories();resources();if(!api->image)constructors();}
int main(){setvbuf(stdout,nullptr,_IONBF,0);auto source=sourceAPI();api=&source;run();if(auto* path=getenv("OPENMFC_NATIVE_EXCEPTION_DLL")){auto image=LoadLibraryA(path);check(image!=nullptr,"load native exception oracle");if(image){auto* b=reinterpret_cast<BYTE*>(image);auto* pe=reinterpret_cast<IMAGE_NT_HEADERS*>(b+reinterpret_cast<IMAGE_DOS_HEADER*>(b)->e_lfanew);if(pe->FileHeader.TimeDateStamp!=0x442cba30||pe->OptionalHeader.SizeOfImage!=0x55c000)return 1;auto native=nativeAPI(image);api=&native;Hook finder(0x2aee00,reinterpret_cast<void*>(&impl__AfxFindStringResourceHandle__YAPEAUHINSTANCE____I_Z)),text(0x1cec10,reinterpret_cast<void*>(&impl__AfxMessageBox__YAHPEB_WII_Z)),resource(0x1cec70,reinterpret_cast<void*>(&impl__AfxMessageBox__YAHIII_Z));run();}}
    printf("Framework exceptions: %d failures\n",failures);return failures?1:0;}
