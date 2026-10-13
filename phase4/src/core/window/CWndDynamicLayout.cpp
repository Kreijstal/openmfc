// Native per-window dynamic layout ownership and resource initialization.
#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxlayout.h"
#include <new>
#ifdef __GNUC__
#define MS_ABI __attribute__((ms_abi))
#else
#define MS_ABI
#endif
extern "C" {
void* MS_ABI impl___2_YAPEAX_K_Z(size_t);
HINSTANCE MS_ABI impl__AfxFindResourceHandle__YAPEAUHINSTANCE____PEB_W0_Z(const wchar_t*,const wchar_t*);
CRuntimeClass* MS_ABI impl__GetThisClass_CDialog__SAPEAUCRuntimeClass__XZ();
CRuntimeClass* MS_ABI impl__GetThisClass_CPropertySheet__SAPEAUCRuntimeClass__XZ();
CRuntimeClass* MS_ABI impl__GetThisClass_CPropertyPage__SAPEAUCRuntimeClass__XZ();
}

void CWnd::EnableDynamicLayout(int enable) {
    delete m_pDynamicLayout;
    m_pDynamicLayout=nullptr;
    if(enable) {
        void* storage=impl___2_YAPEAX_K_Z(sizeof(CMFCDynamicLayout));
        if(storage)m_pDynamicLayout=new(storage) CMFCDynamicLayout;
    }
}
void CWnd::ResizeDynamicLayout() {
    if(m_pDynamicLayout && !::IsIconic(m_hWnd))m_pDynamicLayout->Adjust();
}
void CWnd::InitDynamicLayout() {
    if(!m_pDynamicLayout)return;
    const bool dialog=IsKindOf(impl__GetThisClass_CDialog__SAPEAUCRuntimeClass__XZ());
    const bool sheet=IsKindOf(impl__GetThisClass_CPropertySheet__SAPEAUCRuntimeClass__XZ());
    DWORD style=static_cast<DWORD>(::GetWindowLongW(m_hWnd,GWL_STYLE));
    if(!(style & WS_CHILD) && (dialog || sheet)) {
        RECT rect{};::GetClientRect(m_hWnd,&rect);
        const DWORD resizedStyle=(style & ~DS_MODALFRAME) | WS_THICKFRAME | WS_MAXIMIZEBOX;
        if(resizedStyle!=style)::SetWindowLongW(m_hWnd,GWL_STYLE,resizedStyle);
        style=resizedStyle;
        const DWORD exStyle=static_cast<DWORD>(::GetWindowLongW(m_hWnd,GWL_EXSTYLE));
        CMenu* menu=GetMenu();
        ::AdjustWindowRectEx(&rect,style,::IsMenu(menu ? menu->GetSafeHmenu() : nullptr),exStyle);
        ::SetWindowPos(m_hWnd,nullptr,0,0,rect.right-rect.left,rect.bottom-rect.top,
            SWP_NOMOVE|SWP_NOZORDER|SWP_NOACTIVATE|SWP_FRAMECHANGED|SWP_NOOWNERZORDER);
    }
    if(sheet || IsKindOf(impl__GetThisClass_CPropertyPage__SAPEAUCRuntimeClass__XZ()))return;
    RECT rect{};::GetClientRect(m_hWnd,&rect);
    m_pDynamicLayout->SetMinSize(CSize(rect.right-rect.left,rect.bottom-rect.top));
}
int CWnd::LoadDynamicLayoutResource(const WCHAR* name) {
    if(!::IsWindow(m_hWnd) || !name)return FALSE;
    const wchar_t* type=L"AFX_DIALOG_LAYOUT";
    HINSTANCE instance=impl__AfxFindResourceHandle__YAPEAUHINSTANCE____PEB_W0_Z(name,type);
    HRSRC resource=::FindResourceW(instance,name,type);
    void* data=nullptr;DWORD bytes=0;
    if(resource) {
        bytes=::SizeofResource(instance,resource);
        HGLOBAL handle=::LoadResource(instance,resource);
        if(!handle)return FALSE;
        data=::LockResource(handle);
    }
    BOOL result=CMFCDynamicLayout::LoadResource(this,data,bytes);
    if(result)InitDynamicLayout();
    return result;
}
// Symbol: ?EnableDynamicLayout@CWnd@@QEAAXH@Z
extern "C" void MS_ABI impl__EnableDynamicLayout_CWnd__QEAAXH_Z(CWnd* self,int enable) { self->EnableDynamicLayout(enable); }
// Symbol: ?ResizeDynamicLayout@CWnd@@MEAAXXZ
extern "C" void MS_ABI impl__ResizeDynamicLayout_CWnd__MEAAXXZ(CWnd* self) { self->CWnd::ResizeDynamicLayout(); }
// Symbol: ?InitDynamicLayout@CWnd@@IEAAXXZ
extern "C" void MS_ABI impl__InitDynamicLayout_CWnd__IEAAXXZ(CWnd* self) { self->InitDynamicLayout(); }
// Symbol: ?LoadDynamicLayoutResource@CWnd@@IEAAHPEB_W@Z
extern "C" int MS_ABI impl__LoadDynamicLayoutResource_CWnd__IEAAHPEB_W_Z(CWnd* self,const wchar_t* name) { return self->LoadDynamicLayoutResource(name); }
