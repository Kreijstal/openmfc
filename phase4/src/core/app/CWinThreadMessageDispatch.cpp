// Release-x64 thread messages use message maps without window dispatch.
#define OPENMFC_APPCORE_IMPL
#include "detail/WindowMessageSupport.h"
#include <cstring>

// Symbol: ?DispatchThreadMessageEx@CWinThread@@IEAAHPEAUtagMSG@@@Z
extern "C" BOOL MS_ABI impl__DispatchThreadMessageEx_CWinThread__IEAAHPEAUtagMSG___Z(
    CWinThread* self,MSG* message) {
    using GetMap=const AFX_MSGMAP* (MS_ABI*)(const CWinThread*);
    auto* map=openmfc::detail::WindowSlot<GetMap>(self,12)(self);
    // The root map has no base getter and is not searched by native MFC.
    for(;map->pfnGetBaseMap;map=map->pfnGetBaseMap()) {
        const UINT id=message->message;
        for(auto* entry=map->lpEntries;entry->nSig;++entry) {
            if(entry->nCode || entry->nID)continue;
            bool matches=id<0xc000 ? entry->nMessage==id :
                entry->nMessage==0xc000 && *reinterpret_cast<const UINT*>(entry->nSig)==id;
            if(matches) {
                using Handler=void (MS_ABI*)(CWinThread*,WPARAM,LPARAM);
                static_assert(sizeof(Handler)==sizeof(AFX_PMSG),"native single-inheritance message handler");
                Handler handler;
                std::memcpy(&handler,&entry->pfn,sizeof(handler));
                handler(self,message->wParam,message->lParam);
                return TRUE;
            }
        }
    }
    return FALSE;
}
// Symbol: ?DispatchThreadMessage@CWinThread@@IEAAXPEAUtagMSG@@@Z
extern "C" void MS_ABI impl__DispatchThreadMessage_CWinThread__IEAAXPEAUtagMSG___Z(
    CWinThread* self,MSG* message) {
    (void)impl__DispatchThreadMessageEx_CWinThread__IEAAHPEAUtagMSG___Z(self,message);
}
static_assert(sizeof(AFX_MSGMAP_ENTRY)==32 && offsetof(AFX_MSGMAP_ENTRY,pfn)==24,
    "native thread message-map entries");
