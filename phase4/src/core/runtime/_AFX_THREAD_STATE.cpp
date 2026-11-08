// Native release Unicode thread storage and cleanup (mfc140u 0x1331e0/0x133250).
#define OPENMFC_APPCORE_IMPL
#include "detail/ThreadStateSupport.h"
#include "detail/CThreadLocalObjectSupport.h"
#include <cstdlib>
extern "C" void AFXAPI impl__AfxThrowMemoryException__YAXXZ();

// Symbol: ??2CNoTrackObject@@SAPEAX_K@Z
extern "C" void* AFXAPI impl___2CNoTrackObject__SAPEAX_K_Z(size_t bytes) {
    void* memory=LocalAlloc(LMEM_FIXED|LMEM_ZEROINIT,bytes);
    if(!memory)impl__AfxThrowMemoryException__YAXXZ();
    return memory;
}
// Symbol: ??3CNoTrackObject@@SAXPEAX@Z
extern "C" void AFXAPI impl___3CNoTrackObject__SAXPEAX_Z(void* memory) { if(memory)LocalFree(memory); }
void* CNoTrackObject::operator new(size_t bytes) { return impl___2CNoTrackObject__SAPEAX_K_Z(bytes); }
void CNoTrackObject::operator delete(void* memory) { impl___3CNoTrackObject__SAXPEAX_Z(memory); }

_AFX_THREAD_STATE::_AFX_THREAD_STATE() {
    // Other fields are zeroed by CNoTrackObject allocation, not this ctor.
    m_msgCur.message=0;m_ptCursorLast.x=0;m_ptCursorLast.y=0;m_nMsgLast=0;
}
_AFX_THREAD_STATE::~_AFX_THREAD_STATE() {
    if(m_hHookOldMsgFilter)UnhookWindowsHookEx(m_hHookOldMsgFilter);
    if(m_hHookOldCbtFilter)UnhookWindowsHookEx(m_hHookOldCbtFilter);
    if(m_pSafetyPoolBuffer)std::free(m_pSafetyPoolBuffer);
}
// Symbol: ??0_AFX_THREAD_STATE@@QEAA@XZ
extern "C" _AFX_THREAD_STATE* AFXAPI impl___0_AFX_THREAD_STATE__QEAA_XZ(_AFX_THREAD_STATE* state) {
    return state ? ::new(state)_AFX_THREAD_STATE : nullptr;
}
// Symbol: ??1_AFX_THREAD_STATE@@UEAA@XZ
extern "C" void AFXAPI impl___1_AFX_THREAD_STATE__UEAA_XZ(_AFX_THREAD_STATE* state) {
    if(state)state->~_AFX_THREAD_STATE();
}
extern "C" void* MS_ABI impl__GetData_CThreadLocalObject__QEAAPEAVCNoTrackObject__P6APEAV2_XZ_Z(S_Cthreadlocalobject*,void*);
extern "C" void* MS_ABI impl__GetDataNA_CThreadLocalObject__QEAAPEAVCNoTrackObject__XZ(S_Cthreadlocalobject*);
extern "C" void AFXAPI impl__AfxThrowInvalidArgException__YAXXZ();
namespace {
struct ThreadStorage {
    S_Cthreadlocalobject slot{};
    ~ThreadStorage() {impl___1CThreadLocalObject__QEAA_XZ(&slot);}
} storage;
void* MS_ABI CreateThreadState() {return new _AFX_THREAD_STATE;}
}
_AFX_THREAD_STATE* openmfc::detail::threadstate::Existing() {
    return static_cast<_AFX_THREAD_STATE*>(
        impl__GetDataNA_CThreadLocalObject__QEAAPEAVCNoTrackObject__XZ(&storage.slot));
}
// Symbol: ?AfxGetThreadState@@YAPEAV_AFX_THREAD_STATE@@XZ
_AFX_THREAD_STATE* AFXAPI AfxGetThreadState() {
    auto* state=static_cast<_AFX_THREAD_STATE*>(
        impl__GetData_CThreadLocalObject__QEAAPEAVCNoTrackObject__P6APEAV2_XZ_Z(
            &storage.slot,reinterpret_cast<void*>(&CreateThreadState)));
    if(!state)impl__AfxThrowInvalidArgException__YAXXZ();
    return state;
}
extern "C" _AFX_THREAD_STATE* AFXAPI impl__AfxGetThreadState__YAPEAV_AFX_THREAD_STATE__XZ() {
    return AfxGetThreadState();
}
// Symbol: ?AfxSetModuleState@@YAPEAVAFX_MODULE_STATE@@PEAV1@@Z
extern "C" AFX_MODULE_STATE* AFXAPI impl__AfxSetModuleState__YAPEAVAFX_MODULE_STATE__PEAV1__Z(AFX_MODULE_STATE* next) {
    auto* state=AfxGetThreadState();
    AFX_MODULE_STATE* previous=state->m_pModuleState;
    state->m_pModuleState=next;return previous;
}
