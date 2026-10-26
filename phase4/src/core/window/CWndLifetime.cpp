// Native window destruction: messages release live-window services; the C++
// destructor releases the remaining owned members and then CCmdTarget.
#include "detail/WindowHandleMapSupport.h"
#include "detail/WindowMessageSupport.h"
#include <cstdlib>
extern "C" void AFXAPI impl__AfxThrowInvalidArgException__YAXXZ();
extern "C" void AFXAPI impl__AfxPostQuitMessage__YAXH_Z(int);
namespace {
template<class F> F Slot(void* object,size_t index) {
    return reinterpret_cast<F>((*static_cast<void***>(object))[index]);
}
void DeleteMember(void* object) {
    using Delete=void* (AFXAPI*)(void*,unsigned);
    Slot<Delete>(object,1)(object,1);
}
template<class T> T& Field(void* object,size_t offset) {
    return *reinterpret_cast<T*>(static_cast<BYTE*>(object)+offset);
}
}
extern "C" void AFXAPI OpenMfcDestroyCWndMembers(CWnd* self) {
    const void* identity=self;
    if(self->m_hWnd && identity!=&impl__wndBottom_CWnd__2V1_B &&
        identity!=&impl__wndNoTopMost_CWnd__2V1_B && identity!=&impl__wndTop_CWnd__2V1_B &&
        identity!=&impl__wndTopMost_CWnd__2V1_B)
        impl__DestroyWindow_CWnd__UEAAHXZ(self);
    if(self->m_pCtrlCont)DeleteMember(self->m_pCtrlCont);
    if(self->m_pCtrlSite && Field<CWnd*>(self->m_pCtrlSite,0x50)==self)
        Field<CWnd*>(self->m_pCtrlSite,0x50)=nullptr;
    if(self->m_pMFCCtrlContainer)DeleteMember(self->m_pMFCCtrlContainer);
    if(self->m_pCurrentGestureInfo)std::free(self->m_pCurrentGestureInfo);
    if(self->m_pDynamicLayout) {
        DeleteMember(self->m_pDynamicLayout);
        self->m_pDynamicLayout=nullptr;
    }
}
// Symbol: ?DestroyWindow@CWnd@@UEAAHXZ
extern "C" BOOL AFXAPI impl__DestroyWindow_CWnd__UEAAHXZ(CWnd* self) {
    if(self->m_hWnd) {
        auto* map=openmfc::detail::GetWindowHandleMap(false);
        if(!map) {impl__AfxThrowInvalidArgException__YAXXZ();__assume(0);}
        (void)map->m_permanentMap.GetValueAt(self->m_hWnd);
    }
    if(!self->m_hWnd && !self->m_pCtrlSite)return FALSE;
    if(self->m_pCtrlSite) {
        using Destroy=BOOL (AFXAPI*)(void*);
        return Slot<Destroy>(self->m_pCtrlSite,24)(self->m_pCtrlSite);
    }
    return ::DestroyWindow(self->m_hWnd);
}
void CWnd::OnDestroy() {
    if(m_pCtrlCont)DeleteMember(m_pCtrlCont);
    m_pCtrlCont=nullptr;
    if(m_pProxy) {
        using SetServer=HRESULT (AFXAPI*)(void*,void*,void*);
        Slot<SetServer>(m_pProxy,3)(m_pProxy,nullptr,nullptr);
        m_pProxy=nullptr;
    }
    if(m_pStdObject) {
        using Release=ULONG (AFXAPI*)(void*);
        Slot<Release>(m_pStdObject,2)(m_pStdObject);
        m_pStdObject=nullptr;
    }
    if(m_bIsTouchWindowRegistered)RegisterTouchWindow(FALSE,0);
    if(m_pRenderTarget) {
        DeleteMember(m_pRenderTarget);
        m_pRenderTarget=nullptr;
    }
    Default();
}
void CWnd::OnNcDestroy() {
    auto* thread=impl__AfxGetModuleThreadState__YAPEAVAFX_MODULE_THREAD_STATE__XZ()->m_pCurrentWinThread;
    if(thread) {
        if(Field<CWnd*>(thread,0x40)==this) {
            auto* module=impl__AfxGetModuleState__YAPEAVAFX_MODULE_STATE__XZ();
            if(!Field<BYTE>(module,0x28)) {
                module=impl__AfxGetModuleState__YAPEAVAFX_MODULE_STATE__XZ();
                bool quit=thread!=Field<CWinThread*>(module,8);
                if(!quit) {
                    module=impl__AfxGetModuleState__YAPEAVAFX_MODULE_STATE__XZ();
                    quit=!Field<DWORD>(module,0x60);
                }
                if(quit)impl__AfxPostQuitMessage__YAXH_Z(0);
            }
            Field<CWnd*>(thread,0x40)=nullptr;
        }
        if(Field<CWnd*>(thread,0x48)==this)Field<CWnd*>(thread,0x48)=nullptr;
    }
    if(m_pDropTarget) {
        using Revoke=void (AFXAPI*)(void*);
        Slot<Revoke>(m_pDropTarget,22)(m_pDropTarget);
        m_pDropTarget=nullptr;
    }
    if(m_pCtrlCont)DeleteMember(m_pCtrlCont);
    m_pCtrlCont=nullptr;
    if(m_nFlags&1) {
        auto* tooltip=impl__AfxGetModuleThreadState__YAPEAVAFX_MODULE_THREAD_STATE__XZ()->m_pToolTip;
        if(tooltip && Field<HWND>(tooltip,64)) {
            TOOLINFOW info{};info.cbSize=64;info.uFlags=TTF_IDISHWND;
            info.hwnd=m_hWnd;info.uId=reinterpret_cast<UINT_PTR>(m_hWnd);
            ::SendMessageW(Field<HWND>(tooltip,64),0x433,0,reinterpret_cast<LPARAM>(&info));
        }
    }
    LONG_PTR procedure=::GetWindowLongPtrW(m_hWnd,GWLP_WNDPROC);
    Default();
    if(procedure==::GetWindowLongPtrW(m_hWnd,GWLP_WNDPROC)) {
        using Getter=WNDPROC* (AFXAPI*)(void*);
        WNDPROC super=*Slot<Getter>(this,65)(this);
        if(super)::SetWindowLongPtrW(m_hWnd,GWLP_WNDPROC,reinterpret_cast<LONG_PTR>(super));
    }
    impl__Detach_CWnd__QEAAPEAUHWND____XZ(this);
    using Post=void (AFXAPI*)(void*);
    Slot<Post>(this,74)(this);
}
void CWnd::PostNcDestroy() {}
// Symbol: ?OnDestroy@CWnd@@IEAAXXZ
extern "C" void AFXAPI impl__OnDestroy_CWnd__IEAAXXZ(CWnd* self) {self->OnDestroy();}
// Symbol: ?OnNcDestroy@CWnd@@IEAAXXZ
extern "C" void AFXAPI impl__OnNcDestroy_CWnd__IEAAXXZ(CWnd* self) {self->OnNcDestroy();}
// Symbol: ?PostNcDestroy@CWnd@@MEAAXXZ
extern "C" void AFXAPI impl__PostNcDestroy_CWnd__MEAAXXZ(CWnd* self) {self->CWnd::PostNcDestroy();}
