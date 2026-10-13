#define OPENMFC_APPCORE_IMPL
#include "detail/WindowMessageSupport.h"
extern "C" BOOL MS_ABI impl__AfxHelpEnabled__YAHXZ();
BOOL CDialog::OnInitDialog() {
    if(!(m_lpDialogInit ? ExecuteDlgInit(m_lpDialogInit) : ExecuteDlgInit(m_lpszTemplateName))) {EndDialog(-1);return FALSE;}
    LoadDynamicLayoutResource(m_lpszTemplateName);
    if(!UpdateData(FALSE)) {EndDialog(-1);return FALSE;}
    HWND help=::GetDlgItem(m_hWnd,0xe146);
    if(help)::ShowWindow(help,impl__AfxHelpEnabled__YAHXZ() ? SW_SHOW : SW_HIDE);
    return TRUE;
}
void CDialog::OnOK(){if(UpdateData(TRUE))EndDialog(IDOK);}
void CDialog::OnCancel(){EndDialog(IDCANCEL);}
void CDialog::EndDialog(int result) {
    m_bClosedByEndDialog=TRUE;
    if(m_nFlags&0x18) {
        using End=void (MS_ABI*)(CWnd*,int);
        openmfc::detail::WindowSlot<End>(this,37)(this,result);
    }
    ::EndDialog(m_hWnd,result);
}
// Symbol: ?OnInitDialog@CDialog@@UEAAHXZ
extern "C" BOOL MS_ABI impl__OnInitDialog_CDialog__UEAAHXZ(CDialog* self){return self->CDialog::OnInitDialog();}
// Symbol: ?OnOK@CDialog@@MEAAXXZ
extern "C" void MS_ABI impl__OnOK_CDialog__MEAAXXZ(CDialog* self){self->CDialog::OnOK();}
// Symbol: ?OnCancel@CDialog@@MEAAXXZ
extern "C" void MS_ABI impl__OnCancel_CDialog__MEAAXXZ(CDialog* self){self->CDialog::OnCancel();}
// Symbol: ?EndDialog@CDialog@@QEAAXH@Z
extern "C" void MS_ABI impl__EndDialog_CDialog__QEAAXH_Z(CDialog* self,int result){self->EndDialog(result);}

// Symbol: ?OnSetFont@CDialog@@IEAAXPEAVCFont@@H@Z
extern "C" void MS_ABI impl__OnSetFont_CDialog__IEAAXPEAVCFont__H_Z(CDialog* self,CFont* font,BOOL) {
    using SetFont=void (MS_ABI*)(CDialog*,CFont*);
    openmfc::detail::WindowSlot<SetFont>(self,97)(self,font);self->Default();
}
