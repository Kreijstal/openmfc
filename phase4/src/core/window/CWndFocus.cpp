// Focus, OLE dialog navigation, and the base tooltip-filter callback.
#define OPENMFC_APPCORE_IMPL
#include "detail/WindowMessageSupport.h"
extern "C" CWnd* MS_ABI impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(HWND);
// Symbol: ?SetFocus@CWnd@@QEAAPEAV1@XZ
extern "C" CWnd* MS_ABI impl__SetFocus_CWnd__QEAAPEAV1_XZ(CWnd* self) {
    if(self->m_pCtrlSite) {
        using Focus=CWnd* (MS_ABI*)(void*);
        return openmfc::detail::WindowSlot<Focus>(self->m_pCtrlSite,46)(self->m_pCtrlSite);
    }
    auto parent=[&] {return impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(::GetParent(self->m_hWnd));};
    if(parent() && parent()->m_pCtrlCont) {
        void* container=parent()->m_pCtrlCont;
        *reinterpret_cast<void**>(static_cast<char*>(container)+0xd8)=nullptr;
    }
    return impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(::SetFocus(self->m_hWnd));
}
// Symbol: ?IsDialogMessageW@CWnd@@QEAAHPEAUtagMSG@@@Z
extern "C" BOOL MS_ABI impl__IsDialogMessageW_CWnd__QEAAHPEAUtagMSG___Z(CWnd* self,MSG* message) {
    if(self->m_nFlags&0x100) {
        void* manager=*reinterpret_cast<void**>(reinterpret_cast<char*>(impl__AfxGetModuleState__YAPEAVAFX_MODULE_STATE__XZ())+0xa0);
        using Dialog=BOOL (MS_ABI*)(void*,CWnd*,MSG*);
        return openmfc::detail::WindowSlot<Dialog>(manager,10)(manager,self,message);
    }
    return ::IsDialogMessageW(self->m_hWnd,message);
}
// Symbol: ?PreTranslateMessage@CWnd@@UEAAHPEAUtagMSG@@@Z
extern "C" BOOL MS_ABI impl__PreTranslateMessage_CWnd__UEAAHPEAUtagMSG___Z(CWnd* self,MSG* message) {
    using Filter=void (MS_ABI*)(MSG*,CWnd*);
    auto filter=*reinterpret_cast<Filter*>(reinterpret_cast<char*>(self->m_pModuleState)+0x80);
    if(filter)filter(message,self);
    return FALSE;
}
CWnd* CWnd::SetFocus() { return impl__SetFocus_CWnd__QEAAPEAV1_XZ(this); }
BOOL CWnd::IsDialogMessageW(MSG* message) { return impl__IsDialogMessageW_CWnd__QEAAHPEAUtagMSG___Z(this,message); }
