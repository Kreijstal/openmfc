// Release x64 form focus, OLE metadata and dynamic-layout initialization (mfc140u 0x27a600..0x27a979).
#define OPENMFC_APPCORE_IMPL
#include "detail/WindowMessageSupport.h"
#include <cstddef>
extern "C" {
HINSTANCE MS_ABI impl__AfxFindResourceHandle__YAPEAUHINSTANCE____PEB_W0_Z(const wchar_t*,const wchar_t*);
void* MS_ABI impl___2_YAPEAX_K_Z(size_t);
void MS_ABI impl___3_YAXPEAX_Z(void*);
void MS_ABI impl__AfxThrowMemoryException__YAXXZ();
int MS_ABI impl__AfxMessageBox__YAHPEB_WII_Z(const wchar_t*,UINT,UINT);
int MS_ABI impl__OnCreate_CView__IEAAHPEAUtagCREATESTRUCTW___Z(CView*,CREATESTRUCTW*);
LRESULT MS_ABI impl__OnPrintClient_CView__IEAA_JPEAVCDC__I_Z(CView*,CDC*,UINT);
}
namespace {
void* OccManager() {
    return *reinterpret_cast<void**>(reinterpret_cast<char*>(impl__AfxGetModuleState__YAPEAVAFX_MODULE_STATE__XZ())+0xa0);
}
}
namespace openmfc { namespace detail { namespace docview {
void CleanupFormView(CFormView* self) noexcept {
    try {
        if(self->m_pCreatedOccDialogInfo) {
            if(void* manager=OccManager()) {
                using Post=void (MS_ABI*)(void*,_AFX_OCC_DIALOG_INFO*);
                openmfc::detail::WindowSlot<Post>(manager,6)(manager,self->m_pCreatedOccDialogInfo);
            }
            impl___3_YAXPEAX_Z(self->m_pCreatedOccDialogInfo);
            self->m_pCreatedOccDialogInfo=nullptr;
            self->m_pOccDialogInfo=nullptr;
        }
    } catch(CException* error) {
        wchar_t message[512];
        using Describe=BOOL (MS_ABI*)(CException*,wchar_t*,UINT,UINT*);
        BOOL described=openmfc::detail::WindowSlot<Describe>(error,5)(error,message,512,nullptr);
        // Preserve the native warning and its source location for this diagnostic.
        {
        CString report(L"Exception thrown in destructor (D:\\a\\_work\\1\\s\\src\\vctools\\VC7Libs\\Ship\\ATLMFC\\Src\\MFC\\viewform.cpp:66)");
        if(described) { report+=L"\n";report+=message; }
        impl__AfxMessageBox__YAHPEB_WII_Z(report.GetString(),0,0);
        }
        // The native destructor catch deletes the exception unconditionally.
        delete error;
    }
}
} } }
extern "C" void MS_ABI impl___1CScrollView__UEAA_XZ(CScrollView*);
// Symbol: ??1CFormView@@UEAA@XZ
extern "C" void MS_ABI impl___1CFormView__UEAA_XZ(CFormView* self) noexcept {
    openmfc::detail::docview::CleanupFormView(self);
    impl___1CScrollView__UEAA_XZ(self);
}
BOOL CFormView::SaveFocusControl() {
    HWND focus=::GetFocus();
    if(focus && ::IsChild(m_hWnd,focus)) { m_hWndFocus=focus;return TRUE; }
    return FALSE;
}
void CFormView::OnSetFocus(CWnd*) {
    if(::IsWindow(m_hWndFocus) && ::IsChild(m_hWnd,m_hWndFocus)) { ::SetFocus(m_hWndFocus);return; }
    m_hWndFocus=nullptr;
    Default();
}
void CFormView::OnActivateFrame(UINT state,CFrameWnd*) { if(state==WA_INACTIVE)SaveFocusControl(); }
BOOL CFormView::SetOccDialogInfo(_AFX_OCC_DIALOG_INFO* info) { m_pOccDialogInfo=info;return TRUE; }
_AFX_OCC_DIALOG_INFO* CFormView::GetOccDialogInfo() {
    if(!m_pOccDialogInfo && m_lpszTemplateName) {
        HINSTANCE module=impl__AfxFindResourceHandle__YAPEAUHINSTANCE____PEB_W0_Z(m_lpszTemplateName,RT_DIALOG);
        HRSRC resource=::FindResourceW(module,m_lpszTemplateName,RT_DIALOG);
        if(resource) {
            HGLOBAL loaded=::LoadResource(module,resource);
            if(loaded) {
                const auto* dialog=static_cast<const DLGTEMPLATE*>(::LockResource(loaded));
                if(dialog)if(void* manager=OccManager()) {
                    auto* info=static_cast<_AFX_OCC_DIALOG_INFO*>(impl___2_YAPEAX_K_Z(sizeof(_AFX_OCC_DIALOG_INFO)));
                    if(!info)impl__AfxThrowMemoryException__YAXXZ();
                    using Pre=const DLGTEMPLATE* (MS_ABI*)(void*,_AFX_OCC_DIALOG_INFO*,const DLGTEMPLATE*);
                    using Post=void (MS_ABI*)(void*,_AFX_OCC_DIALOG_INFO*);
                    // Native keeps the allocation when a manager callback throws.
                    openmfc::detail::WindowSlot<Pre>(manager,5)(manager,info,dialog);
                    m_pOccDialogInfo=info;
                    if(m_pCreatedOccDialogInfo) {
                        openmfc::detail::WindowSlot<Post>(manager,6)(manager,m_pCreatedOccDialogInfo);
                        // PostCreateDialog may replace the stored owned pointer.
                        impl___3_YAXPEAX_Z(m_pCreatedOccDialogInfo);
                    }
                    m_pCreatedOccDialogInfo=info;
                }
            }
        }
    }
    return m_pOccDialogInfo;
}
LRESULT CFormView::HandleInitDialog(WPARAM,LPARAM) {
    Default();
    void* manager=OccManager();
    if(manager && m_pOccDialogInfo) {
        using Create=BOOL (MS_ABI*)(void*,CWnd*,const wchar_t*,_AFX_OCC_DIALOG_INFO*);
        if(!openmfc::detail::WindowSlot<Create>(manager,9)(manager,this,m_lpszTemplateName,m_pOccDialogInfo))return 0;
    }
    LoadDynamicLayoutResource(m_lpszTemplateName);
    return 0;
}
int CFormView::OnCreate(CREATESTRUCTW* create) {
    create->lpCreateParams=m_pCreateContext;
    return impl__OnCreate_CView__IEAAHPEAUtagCREATESTRUCTW___Z(this,create);
}
LRESULT CFormView::OnPrintClient(CDC* dc,UINT flags) { return impl__OnPrintClient_CView__IEAA_JPEAVCDC__I_Z(this,dc,flags); }
// The inherited creation/printing handlers retain their own implementations.
BEGIN_MESSAGE_MAP(CFormView,CScrollView)
    ON_WM_SETFOCUS()
    ON_WM_CREATE()
    OPENMFC_MAP_ENTRY(WM_PRINTCLIENT,0,0,0,AfxSig_l_D_u,&CFormView::OnPrintClient)
    ON_MESSAGE(WM_INITDIALOG,&CFormView::HandleInitDialog)
END_MESSAGE_MAP()
// Symbol: ?SaveFocusControl@CFormView@@IEAAHXZ
extern "C" BOOL MS_ABI impl__SaveFocusControl_CFormView__IEAAHXZ(CFormView* self) { return self->SaveFocusControl(); }
// Symbol: ?OnSetFocus@CFormView@@IEAAXPEAVCWnd@@@Z
extern "C" void MS_ABI impl__OnSetFocus_CFormView__IEAAXPEAVCWnd___Z(CFormView* self,CWnd* old) { self->OnSetFocus(old); }
// Symbol: ?OnActivateFrame@CFormView@@MEAAXIPEAVCFrameWnd@@@Z
extern "C" void MS_ABI impl__OnActivateFrame_CFormView__MEAAXIPEAVCFrameWnd___Z(CFormView* self,UINT state,CFrameWnd* frame) { self->CFormView::OnActivateFrame(state,frame); }
// Symbol: ?SetOccDialogInfo@CFormView@@MEAAHPEAU_AFX_OCC_DIALOG_INFO@@@Z
extern "C" BOOL MS_ABI impl__SetOccDialogInfo_CFormView__MEAAHPEAU_AFX_OCC_DIALOG_INFO___Z(CFormView* self,_AFX_OCC_DIALOG_INFO* info) { return self->CFormView::SetOccDialogInfo(info); }
// Symbol: ?GetOccDialogInfo@CFormView@@MEAAPEAU_AFX_OCC_DIALOG_INFO@@XZ
extern "C" _AFX_OCC_DIALOG_INFO* MS_ABI impl__GetOccDialogInfo_CFormView__MEAAPEAU_AFX_OCC_DIALOG_INFO__XZ(CFormView* self) { return self->CFormView::GetOccDialogInfo(); }
// Symbol: ?HandleInitDialog@CFormView@@IEAA_J_K_J@Z
extern "C" LRESULT MS_ABI impl__HandleInitDialog_CFormView__IEAA_J_K_J_Z(CFormView* self,WPARAM wparam,LPARAM lparam) { return self->HandleInitDialog(wparam,lparam); }
// Symbol: ?OnCreate@CFormView@@IEAAHPEAUtagCREATESTRUCTW@@@Z
extern "C" int MS_ABI impl__OnCreate_CFormView__IEAAHPEAUtagCREATESTRUCTW___Z(CFormView* self,CREATESTRUCTW* create) { return self->OnCreate(create); }
// Symbol: ?OnPrintClient@CFormView@@IEAA_JPEAVCDC@@I@Z
extern "C" LRESULT MS_ABI impl__OnPrintClient_CFormView__IEAA_JPEAVCDC__I_Z(CFormView* self,CDC* dc,UINT flags) { return self->OnPrintClient(dc,flags); }
// Symbol: ?GetThisMessageMap@CFormView@@KAPEBUAFX_MSGMAP@@XZ
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CFormView__KAPEBUAFX_MSGMAP__XZ() { return CFormView::GetThisMessageMap(); }
// Symbol: ?GetMessageMap@CFormView@@MEBAPEBUAFX_MSGMAP@@XZ
extern "C" const AFX_MSGMAP* MS_ABI impl__GetMessageMap_CFormView__MEBAPEBUAFX_MSGMAP__XZ(const CFormView*) { return CFormView::GetThisMessageMap(); }
