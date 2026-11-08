// Process-wide recursive locks used by module registration and shared caches.
#define OPENMFC_APPCORE_IMPL
#include "openmfc/afx.h"
namespace {
CRITICAL_SECTION* GlobalLock(int index) {
    constexpr int count=16;
    static CRITICAL_SECTION locks[count];
    static const bool initialized=[] {
        for(auto& lock:locks)::InitializeCriticalSection(&lock);
        return true;
    }();
    (void)initialized;
    return index>=0 && index<count ? &locks[index]:nullptr;
}
}
// Symbol: ?AfxLockGlobals@@YAXH@Z
extern "C" void AFXAPI impl__AfxLockGlobals__YAXH_Z(int index) {
    if(auto* lock=GlobalLock(index))::EnterCriticalSection(lock);
}
// Symbol: ?AfxUnlockGlobals@@YAXH@Z
extern "C" void AFXAPI impl__AfxUnlockGlobals__YAXH_Z(int index) {
    if(auto* lock=GlobalLock(index))::LeaveCriticalSection(lock);
}
