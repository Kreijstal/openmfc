// Release-x64 positioning dispatch for HWND and embedded OLE controls.
#define OPENMFC_APPCORE_IMPL
#include "detail/WindowMessageSupport.h"

int CWnd::SetWindowPos(const CWnd* after,int x,int y,int width,int height,UINT flags) {
    if(m_pCtrlSite) {
        using Position=BOOL (MS_ABI*)(void*,const CWnd*,int,int,int,int,UINT);
        return openmfc::detail::WindowSlot<Position>(m_pCtrlSite,41)(m_pCtrlSite,after,x,y,width,height,flags);
    }
    return ::SetWindowPos(m_hWnd,after?after->m_hWnd:nullptr,x,y,width,height,flags);
}
// Symbol: ?SetWindowPos@CWnd@@QEAAHPEBV1@HHHHI@Z
extern "C" int MS_ABI impl__SetWindowPos_CWnd__QEAAHPEBV1_HHHHI_Z(CWnd* self,const CWnd* after,int x,int y,int width,int height,UINT flags) {
    return self->SetWindowPos(after,x,y,width,height,flags);
}
