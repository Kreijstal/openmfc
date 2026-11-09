// Real handler links, cleanup and rethrow ownership with a native DLL oracle.
#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxstat_.h"
#include <array>
#include <vector>
#include <cstdio>
#include <cstring>
#include <cstdlib>
extern "C" AFX_EXCEPTION_LINK* impl___0AFX_EXCEPTION_LINK__QEAA_XZ(AFX_EXCEPTION_LINK*);
extern "C" void impl__AfxTryCleanup__YAXXZ();
extern "C" void impl__AfxThrowLastCleanup__YAXXZ();
namespace exception_link_test {
int failures;unsigned comparisons;
void check(bool good,const char* message){if(!good){++failures;printf("FAIL: %s\n",message);}}
using Event=std::array<INT64,3>;
std::vector<Event> events;
alignas(_AFX_THREAD_STATE) BYTE stateBytes[sizeof(_AFX_THREAD_STATE)];
auto* state=reinterpret_cast<_AFX_THREAD_STATE*>(stateBytes);
AFX_EXCEPTION_LINK *constructing,*inner,*outer;
int getterMode,mutation,destroyed;
void* getState(){
 events.push_back({1,GetLastError(),constructing?constructing->m_pException==nullptr:1});
 SetLastError(0x9999);
 if(getterMode==2)throw 77;
 return getterMode==1?nullptr:state;
}
void* nativeGetState(void* slot,void* factory){check(slot&&factory,"native handler uses the allocating thread-state getter");return getState();}
struct Hook {
 BYTE* address;BYTE old[12];DWORD protection;
 Hook(void* entry,void* target):address(static_cast<BYTE*>(entry)){memcpy(old,address,12);VirtualProtect(address,12,PAGE_EXECUTE_READWRITE,&protection);BYTE jump[12]={0x48,0xb8};memcpy(jump+2,&target,8);jump[10]=0xff;jump[11]=0xe0;memcpy(address,jump,12);FlushInstructionCache(GetCurrentProcess(),address,12);}
 ~Hook(){memcpy(address,old,12);FlushInstructionCache(GetCurrentProcess(),address,12);DWORD ignored;VirtualProtect(address,12,protection,&ignored);}
};
struct API {
 decltype(&impl___0AFX_EXCEPTION_LINK__QEAA_XZ) construct;
 void (*cleanup)();void (*rethrow)();HMODULE image;
};
const API* api;
struct Counted : CException {
 Counted(bool owned):CException(owned){}
 ~Counted() override {
  ++destroyed;events.push_back({2,state->m_exceptionContext.m_pLinkTop==inner,GetLastError()});
  if(mutation==1){inner->m_pLinkPrev=nullptr;state->m_exceptionContext.m_pLinkTop=outer;}
  if(mutation==2){inner->m_pLinkPrev=outer;state->m_exceptionContext.m_pLinkTop=nullptr;}
  SetLastError(0x7777);
 }
};
struct Result {
 int caught,destroyed;DWORD last;bool topInner,topOuter,topNull,previousOuter,exceptionNull;std::vector<Event> calls;
 bool operator==(const Result& b)const{return caught==b.caught&&destroyed==b.destroyed&&last==b.last&&topInner==b.topInner&&topOuter==b.topOuter&&topNull==b.topNull&&previousOuter==b.previousOuter&&exceptionNull==b.exceptionNull&&calls==b.calls;}
};
std::vector<Result> golden;
void sample(int kind){
 Result out{};SetLastError(0x12345678);events.clear();
 try{if(kind==0){constructing=inner;check(api->construct(inner)==inner,"handler constructor returns the actual caller object");}else if(kind==1)api->cleanup();else api->rethrow();}
 catch(CInvalidArgException*){out.caught=22;}catch(int value){out.caught=value;}catch(...){check(false,"handler cleanup preserves native typed failures");out.caught=-1;}
 constructing=nullptr;out.last=GetLastError();out.destroyed=destroyed;out.topInner=state->m_exceptionContext.m_pLinkTop==inner;out.topOuter=state->m_exceptionContext.m_pLinkTop==outer;out.topNull=state->m_exceptionContext.m_pLinkTop==nullptr;out.previousOuter=inner->m_pLinkPrev==outer;out.exceptionNull=inner->m_pException==nullptr;out.calls=events;
 if(api->image){bool good=comparisons<golden.size()&&out==golden[comparisons];if(!good&&failures<4)printf("handler mismatch %u caught%d/%d destroyed%d/%d last%x/%x\n",comparisons,out.caught,golden[comparisons].caught,out.destroyed,golden[comparisons].destroyed,out.last,golden[comparisons].last);check(good,"handler construction, cleanup and rethrow match native state, errors and callback order");}else golden.push_back(out);++comparisons;
}
void run(){
 alignas(AFX_EXCEPTION_LINK) BYTE innerBytes[32],outerBytes[32];inner=reinterpret_cast<AFX_EXCEPTION_LINK*>(innerBytes);outer=reinterpret_cast<AFX_EXCEPTION_LINK*>(outerBytes);
 for(int kind:{0,1,2})for(int mode:{0,1,2})for(int top:{0,1})for(int owned:{-1,0,1})for(int change:{0,1,2}){
  memset(stateBytes,0,sizeof(stateBytes));memset(innerBytes,0xa5,sizeof(innerBytes));memset(outerBytes,0xa5,sizeof(outerBytes));
  getterMode=mode;mutation=change;destroyed=0;constructing=nullptr;inner->m_pLinkPrev=outer;
  auto* error=owned>=0?new Counted(owned!=0):nullptr;inner->m_pException=error;
  state->m_exceptionContext.m_pLinkTop=top?(kind?inner:outer):nullptr;
  sample(kind);
  for(size_t at:{16u,17u,31u})check(innerBytes[at]==0xa5,"handler operations preserve object guards");
  if(error&&!destroyed){mutation=0;error->m_bAutoDelete=TRUE;delete error;}
 }
 if(!api->image){
  getterMode=mutation=destroyed=0;state->m_exceptionContext.m_pLinkTop=nullptr;SetLastError(81);
  {AFX_EXCEPTION_LINK first;check(state->m_exceptionContext.m_pLinkTop==&first&&!first.m_pLinkPrev,"ordinary handler object pushes onto the native thread context");
   {AFX_EXCEPTION_LINK second;check(state->m_exceptionContext.m_pLinkTop==&second&&second.m_pLinkPrev==&first,"ordinary nested handlers form a stack");}
   check(state->m_exceptionContext.m_pLinkTop==&first,"ordinary handler destructor pops exactly its handler");}
  check(!state->m_exceptionContext.m_pLinkTop&&GetLastError()==81,"ordinary handler lifetime restores the empty stack and preserves last error");
 }
}
}
_AFX_THREAD_STATE* AFXAPI AfxGetThreadState(){return static_cast<_AFX_THREAD_STATE*>(exception_link_test::getState());}
int TestExceptionLinks(HMODULE image){using namespace exception_link_test;failures=0;comparisons=0;
 API source{impl___0AFX_EXCEPTION_LINK__QEAA_XZ,impl__AfxTryCleanup__YAXXZ,impl__AfxThrowLastCleanup__YAXXZ,nullptr},native{};
 if(image){auto* b=reinterpret_cast<BYTE*>(image);native={reinterpret_cast<decltype(native.construct)>(b+0x227470),reinterpret_cast<void(*)()>(b+0x2274e0),reinterpret_cast<void(*)()>(b+0x227550),image};}
 api=image?&native:&source;
 if(image){Hook getter(reinterpret_cast<BYTE*>(image)+0x14cf40,reinterpret_cast<void*>(&nativeGetState));run();}else run();
 printf("%s exception handlers: %u native comparisons, %d failures\n",image?"Native":"Source",comparisons,failures);return failures;
}
