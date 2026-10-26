// Native release-x64 CWnd creation protocol (0x28b440..0x28b71d).
#define OPENMFC_APPCORE_IMPL
#include "detail/WindowCreationSupport.h"
#include "detail/WindowActivationSupport.h"
extern "C" {
void MS_ABI impl__AfxHookWindowCreate__YAXPEAVCWnd___Z(CWnd*);
BOOL MS_ABI impl__AfxUnhookWindowCreate__YAHXZ();
}
BOOL CWnd::CreateEx(DWORD extended,const wchar_t* cls,const wchar_t* title,DWORD style,
                   int x,int y,int width,int height,HWND parent,HMENU menu,void* parameter) {
    CREATESTRUCTW create;
    create.lpCreateParams=parameter;
    auto* module=reinterpret_cast<BYTE*>(impl__AfxGetModuleState__YAPEAVAFX_MODULE_STATE__XZ());
    create.hInstance=*reinterpret_cast<HINSTANCE*>(module+0x10);
    create.hMenu=menu;create.hwndParent=parent;create.cy=height;create.cx=width;
    create.y=y;create.x=x;create.style=style;create.lpszName=title;create.lpszClass=cls;create.dwExStyle=extended;
    if(!PreCreateWindow(create)){PostNcDestroy();return FALSE;}
    impl__AfxHookWindowCreate__YAXPEAVCWnd___Z(this);
    HWND result=openmfc::detail::ContextCreateWindow(create);
    if(!impl__AfxUnhookWindowCreate__YAHXZ())PostNcDestroy();
    return result!=nullptr;
}
BOOL CWnd::Create(const wchar_t* cls,const wchar_t* title,DWORD style,const RECT& rect,CWnd* parent,UINT id,CCreateContext* context) {
    return CreateEx(0,cls,title,style|WS_CHILD,rect.left,rect.top,
        static_cast<int>(static_cast<UINT>(rect.right)-static_cast<UINT>(rect.left)),
        static_cast<int>(static_cast<UINT>(rect.bottom)-static_cast<UINT>(rect.top)),
        parent?parent->m_hWnd:nullptr,reinterpret_cast<HMENU>(UINT_PTR(id)),context);
}
BOOL CWnd::CreateEx(DWORD extended,const wchar_t* cls,const wchar_t* title,DWORD style,const RECT& rect,CWnd* parent,UINT id,void* parameter) {
    return CreateEx(extended,cls,title,style,rect.left,rect.top,
        static_cast<int>(UINT(rect.right)-UINT(rect.left)),static_cast<int>(UINT(rect.bottom)-UINT(rect.top)),
        parent?parent->m_hWnd:nullptr,reinterpret_cast<HMENU>(UINT_PTR(id)),parameter);
}
// Symbol: ?CreateEx@CWnd@@UEAAHKPEB_W0KHHHHPEAUHWND__@@PEAUHMENU__@@PEAX@Z
extern "C" BOOL MS_ABI impl__CreateEx_CWnd__UEAAHKPEB_W0KHHHHPEAUHWND____PEAUHMENU____PEAX_Z(
    CWnd* self,DWORD extended,const wchar_t* cls,const wchar_t* title,DWORD style,
    int x,int y,int width,int height,HWND parent,HMENU menu,void* parameter) {
    return self->CWnd::CreateEx(extended,cls,title,style,x,y,width,height,parent,menu,parameter);
}
// Symbol: ?CreateEx@CWnd@@UEAAHKPEB_W0KAEBUtagRECT@@PEAV1@IPEAX@Z
extern "C" BOOL MS_ABI impl__CreateEx_CWnd__UEAAHKPEB_W0KAEBUtagRECT__PEAV1_IPEAX_Z(
    CWnd* self,DWORD extended,const wchar_t* cls,const wchar_t* title,DWORD style,
    const RECT& rect,CWnd* parent,UINT id,void* parameter) {
    return self->CWnd::CreateEx(extended,cls,title,style,rect,parent,id,parameter);
}
// Symbol: ?Create@CWnd@@UEAAHPEB_W0KAEBUtagRECT@@PEAV1@IPEAUCCreateContext@@@Z
extern "C" BOOL MS_ABI impl__Create_CWnd__UEAAHPEB_W0KAEBUtagRECT__PEAV1_IPEAUCCreateContext___Z(
    CWnd* self,const wchar_t* cls,const wchar_t* title,DWORD style,const RECT& rect,CWnd* parent,UINT id,CCreateContext* context) {
    return self->CWnd::Create(cls,title,style,rect,parent,id,context);
}
