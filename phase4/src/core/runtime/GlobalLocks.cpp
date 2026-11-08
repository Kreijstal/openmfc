// Native recursive process locks, initialized individually under the init lock.
#define OPENMFC_APPCORE_IMPL
#include "openmfc/afx.h"
extern "C" void AFXAPI impl__AfxThrowInvalidArgException__YAXXZ();
namespace {
constexpr UINT lockCount=17;
int initialized;
CRITICAL_SECTION initializeLock;
int ready[lockCount];
CRITICAL_SECTION locks[lockCount];
void Validate(int index){if(static_cast<UINT>(index)>=lockCount)impl__AfxThrowInvalidArgException__YAXXZ();}
}
// Symbol: ?AfxCriticalInit@@YAHXZ
extern "C" int AFXAPI impl__AfxCriticalInit__YAHXZ(){
    if(!initialized){initialized=1;::InitializeCriticalSection(&initializeLock);}
    return initialized;
}
// Symbol: ?AfxCriticalTerm@@YAXXZ
extern "C" void AFXAPI impl__AfxCriticalTerm__YAXXZ(){
    if(initialized){
        --initialized;::DeleteCriticalSection(&initializeLock);
        for(UINT i=0;i<lockCount;++i)if(ready[i]){::DeleteCriticalSection(&locks[i]);--ready[i];}
    }
}
// Symbol: ?AfxLockGlobals@@YAXH@Z
extern "C" void AFXAPI impl__AfxLockGlobals__YAXH_Z(int index){
    Validate(index);
    if(!initialized)impl__AfxCriticalInit__YAHXZ();
    if(!ready[index]){
        ::EnterCriticalSection(&initializeLock);
        if(!ready[index]){::InitializeCriticalSection(&locks[index]);++ready[index];}
        ::LeaveCriticalSection(&initializeLock);
    }
    ::EnterCriticalSection(&locks[index]);
}
// Symbol: ?AfxUnlockGlobals@@YAXH@Z
extern "C" void AFXAPI impl__AfxUnlockGlobals__YAXH_Z(int index){Validate(index);::LeaveCriticalSection(&locks[index]);}
