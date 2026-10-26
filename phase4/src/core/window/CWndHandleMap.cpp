// Native HWND attachment, lookup and temporary-wrapper APIs.
#include "detail/WindowHandleMapSupport.h"
extern "C" void AFXAPI impl__AttachControlSite_COleControlContainer__QEAAXPEAVCWnd__I_Z(
    COleControlContainer*,CWnd*,UINT);
CWnd* CWnd::FromHandle(HWND handle) {
    auto* map=openmfc::detail::GetWindowHandleMap(true);
    auto* window=static_cast<CWnd*>(map->FromHandle(handle));
    // This private helper accepts NULL this, including FromHandle(NULL).
    impl__AttachControlSite_CWnd__IEAAXPEAVCHandleMap___Z(window,map);
    return window;
}
void* CWnd::FromHandlePermanent(HWND handle) {
    auto* map=openmfc::detail::GetWindowHandleMap(false);
    return map ? map->m_permanentMap.GetValueAt(handle) : nullptr;
}
BOOL CWnd::Attach(HWND handle) {
    if(!handle)return FALSE;
    auto* map=openmfc::detail::GetWindowHandleMap(true);
    m_hWnd=handle;
    map->m_permanentMap[handle]=this;
    AttachControlSite(map);
    return TRUE;
}
HWND CWnd::Detach() {
    HWND previous=m_hWnd;
    if(previous) {
        auto* map=openmfc::detail::GetWindowHandleMap(false);
        if(map)map->m_permanentMap.RemoveKey(m_hWnd);
        m_hWnd=nullptr;
    }
    m_pCtrlSite=nullptr;
    return previous;
}
void CWnd::DeleteTempMap() {
    auto* map=openmfc::detail::GetWindowHandleMap(false);
    impl__DeleteTemp_CHandleMap__QEAAXXZ(map);
}
void CWnd::AttachControlSite(CHandleMap* map) {
    if(!m_pCtrlSite) {
        auto* parent=static_cast<CWnd*>(map->m_permanentMap.GetValueAt(::GetParent(m_hWnd)));
        if(parent && parent->m_pCtrlCont)
            impl__AttachControlSite_COleControlContainer__QEAAXPEAVCWnd__I_Z(parent->m_pCtrlCont,this,0);
    }
}
// Symbol: ?FromHandle@CWnd@@SAPEAV1@PEAUHWND__@@@Z
extern "C" CWnd* AFXAPI impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(HWND handle) {return CWnd::FromHandle(handle);}
// Symbol: ?FromHandlePermanent@CWnd@@SAPEAV1@PEAUHWND__@@@Z
extern "C" CWnd* AFXAPI impl__FromHandlePermanent_CWnd__SAPEAV1_PEAUHWND_____Z(HWND handle) {return static_cast<CWnd*>(CWnd::FromHandlePermanent(handle));}
// Symbol: ?Attach@CWnd@@QEAAHPEAUHWND__@@@Z
extern "C" BOOL AFXAPI impl__Attach_CWnd__QEAAHPEAUHWND_____Z(CWnd* self,HWND handle) {return self->Attach(handle);}
// Symbol: ?Detach@CWnd@@QEAAPEAUHWND__@@XZ
extern "C" HWND AFXAPI impl__Detach_CWnd__QEAAPEAUHWND____XZ(CWnd* self) {return self->Detach();}
// Symbol: ?DeleteTempMap@CWnd@@SAXXZ
extern "C" void AFXAPI impl__DeleteTempMap_CWnd__SAXXZ() {CWnd::DeleteTempMap();}
// Symbol: ?AttachControlSite@CWnd@@IEAAXPEAVCHandleMap@@@Z
extern "C" void AFXAPI impl__AttachControlSite_CWnd__IEAAXPEAVCHandleMap___Z(CWnd* self,CHandleMap* map) {if(self)self->AttachControlSite(map);}
