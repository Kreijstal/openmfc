// Native per-module lock transitions, map disposal and application safety pool.
#define OPENMFC_APPCORE_IMPL
#include "detail/ModuleThreadStateSupport.h"
#include "detail/RegcoreSupport.h"
#include "detail/CHandleMapSupport.h"
#include "detail/ThreadStateSupport.h"
#include <cstdlib>
#include <malloc.h>
#include <cstdint>
extern "C" void MS_ABI impl__DeleteTemp_CHandleMap__QEAAXXZ(CHandleMap*);
// Symbol: ?AfxLockTempMaps@@YAXXZ
extern "C" void MS_ABI impl__AfxLockTempMaps__YAXXZ() {
    auto& count=impl__AfxGetModuleThreadState__YAPEAVAFX_MODULE_THREAD_STATE__XZ()->m_nTempMapLock;
    count=static_cast<int>(static_cast<uint32_t>(count)+1);
}
// Symbol: ?AfxUnlockTempMaps@@YAHH@Z
extern "C" BOOL MS_ABI impl__AfxUnlockTempMaps__YAHH_Z(int deleteTemps) {
    auto* state=impl__AfxGetModuleThreadState__YAPEAVAFX_MODULE_THREAD_STATE__XZ();
    if(state->m_nTempMapLock &&
       !(state->m_nTempMapLock=static_cast<int>(static_cast<uint32_t>(state->m_nTempMapLock)-1))) {
        if(deleteTemps) {
            if(deleteTemps!=-1) {
                auto* thread=impl__AfxGetModuleThreadState__YAPEAVAFX_MODULE_THREAD_STATE__XZ()->m_pCurrentWinThread;
                if(thread && thread->m_lpfnOleTermOrFreeLib)thread->m_lpfnOleTermOrFreeLib(FALSE,FALSE);
            }
            impl__DeleteTemp_CHandleMap__QEAAXXZ(state->m_pmapHGDIOBJ);
            impl__DeleteTemp_CHandleMap__QEAAXXZ(state->m_pmapHDC);
            impl__DeleteTemp_CHandleMap__QEAAXXZ(state->m_pmapHMENU);
            impl__DeleteTemp_CHandleMap__QEAAXXZ(state->m_pmapHWND);
            impl__DeleteTemp_CHandleMap__QEAAXXZ(state->m_pmapHIMAGELIST);
        }
        auto* app=impl__AfxGetModuleState__YAPEAVAFX_MODULE_STATE__XZ()->m_pCurrentWinApp;
        auto* thread=openmfc::detail::threadstate::Existing();
        if(thread && app) {
            auto& requested=*reinterpret_cast<size_t*>(reinterpret_cast<BYTE*>(app)+0x138);
            bool grow=!thread->m_pSafetyPoolBuffer;
            if(!grow) {
                const size_t minimum=requested;
                grow=::_msize(thread->m_pSafetyPoolBuffer)<minimum;
            }
            if(grow && requested) {
                size_t previous=0;
                if(thread->m_pSafetyPoolBuffer) {
                    previous=::_msize(thread->m_pSafetyPoolBuffer);
                    std::free(thread->m_pSafetyPoolBuffer);
                }
                thread->m_pSafetyPoolBuffer=std::malloc(requested);
                if(!thread->m_pSafetyPoolBuffer && previous)
                    thread->m_pSafetyPoolBuffer=std::malloc(previous);
            }
        }
    }
    return state->m_nTempMapLock!=0;
}
