#define OPENMFC_APPCORE_IMPL
#include "detail/WindowMessageSupport.h"
#include <new>
IMPLEMENT_DYNAMIC(CDialog,CWnd)
void CDialog::Initialize() {
    m_nIDHelp=0;m_lpszTemplateName=nullptr;m_hDialogTemplate=nullptr;
    m_lpDialogTemplate=nullptr;m_lpDialogInit=nullptr;m_pParentWnd=nullptr;
    m_hWndTop=nullptr;m_bClosedByEndDialog=FALSE;m_pOccDialogInfo=nullptr;
}
CDialog::CDialog(){Initialize();}
CDialog::CDialog(UINT id,CWnd* parent){Initialize();m_nIDHelp=id;m_lpszTemplateName=MAKEINTRESOURCEW(id);m_pParentWnd=parent;}
CDialog::CDialog(const wchar_t* name,CWnd* parent){Initialize();m_lpszTemplateName=name;m_pParentWnd=parent;if(IS_INTRESOURCE(name))m_nIDHelp=LOWORD(reinterpret_cast<UINT_PTR>(name));}
CDialog::~CDialog(){if(m_hWnd)DestroyWindow();}
BOOL CDialog::InitModalIndirect(const DLGTEMPLATE* data,CWnd* parent,void* init) {m_lpDialogTemplate=data;m_lpDialogInit=init;if(!m_pParentWnd)m_pParentWnd=parent;return TRUE;}
BOOL CDialog::InitModalIndirect(HGLOBAL data,CWnd* parent) {m_hDialogTemplate=data;if(!m_pParentWnd)m_pParentWnd=parent;return TRUE;}
BOOL CDialog::SetOccDialogInfo(_AFX_OCC_DIALOG_INFO* info){m_pOccDialogInfo=info;return TRUE;}
_AFX_OCC_DIALOG_INFO* CDialog::GetOccDialogInfo(){return m_pOccDialogInfo;}
void CDialog::PreInitDialog(){}
void CDialog::OnSetFont(CFont*){}
// Symbol: ??0CDialog@@QEAA@XZ
extern "C" void* MS_ABI impl___0CDialog__QEAA_XZ(void* self){return new(self) CDialog;}
// Symbol: ??0CDialog@@QEAA@IPEAVCWnd@@@Z
extern "C" void* MS_ABI impl___0CDialog__QEAA_IPEAVCWnd___Z(void* self,UINT id,CWnd* parent){return new(self) CDialog(id,parent);}
// Symbol: ??0CDialog@@QEAA@PEB_WPEAVCWnd@@@Z
extern "C" void* MS_ABI impl___0CDialog__QEAA_PEB_WPEAVCWnd___Z(void* self,const wchar_t* name,CWnd* parent){return new(self) CDialog(name,parent);}
// Symbol: ??1CDialog@@UEAA@XZ
extern "C" void MS_ABI impl___1CDialog__UEAA_XZ(CDialog* self){self->~CDialog();}
// Symbol: ?Initialize@CDialog@@QEAAXXZ
extern "C" void MS_ABI impl__Initialize_CDialog__QEAAXXZ(CDialog* self){self->Initialize();}
// Symbol: ?InitModalIndirect@CDialog@@QEAAHPEAXPEAVCWnd@@@Z
extern "C" BOOL MS_ABI impl__InitModalIndirect_CDialog__QEAAHPEAXPEAVCWnd___Z(CDialog* self,HGLOBAL data,CWnd* parent){return self->InitModalIndirect(data,parent);}
// Symbol: ?InitModalIndirect@CDialog@@QEAAHPEBUDLGTEMPLATE@@PEAVCWnd@@PEAX@Z
extern "C" BOOL MS_ABI impl__InitModalIndirect_CDialog__QEAAHPEBUDLGTEMPLATE__PEAVCWnd__PEAX_Z(CDialog* self,const DLGTEMPLATE* data,CWnd* parent,void* init){return self->InitModalIndirect(data,parent,init);}
// Symbol: ?SetOccDialogInfo@CDialog@@MEAAHPEAU_AFX_OCC_DIALOG_INFO@@@Z
extern "C" BOOL MS_ABI impl__SetOccDialogInfo_CDialog__MEAAHPEAU_AFX_OCC_DIALOG_INFO___Z(CDialog* self,_AFX_OCC_DIALOG_INFO* info){return self->CDialog::SetOccDialogInfo(info);}
// Symbol: ?GetOccDialogInfo@CDialog@@MEAAPEAU_AFX_OCC_DIALOG_INFO@@XZ
extern "C" _AFX_OCC_DIALOG_INFO* MS_ABI impl__GetOccDialogInfo_CDialog__MEAAPEAU_AFX_OCC_DIALOG_INFO__XZ(CDialog* self){return self->CDialog::GetOccDialogInfo();}
// Symbol: ?PreInitDialog@CDialog@@MEAAXXZ
extern "C" void MS_ABI impl__PreInitDialog_CDialog__MEAAXXZ(CDialog* self){self->CDialog::PreInitDialog();}
// Symbol: ?OnSetFont@CDialog@@UEAAXPEAVCFont@@@Z
extern "C" void MS_ABI impl__OnSetFont_CDialog__UEAAXPEAVCFont___Z(CDialog* self,CFont* font){self->CDialog::OnSetFont(font);}
