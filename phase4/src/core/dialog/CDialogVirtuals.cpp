#define OPENMFC_APPCORE_IMPL
#include "detail/WindowMessageSupport.h"
extern "C" {
INT_PTR MS_ABI impl__DoModal_CDialog__UEAA_JXZ(CDialog*);
BOOL MS_ABI impl__Create_CDialog__UEAAHPEB_WPEAVCWnd___Z(CDialog*,const wchar_t*,CWnd*);
BOOL MS_ABI impl__Create_CDialog__UEAAHI_PEAVCWnd___Z(CDialog*,UINT,CWnd*);
BOOL MS_ABI impl__CreateIndirect_CDialog__UEAAHPEAXPEAVCWnd___Z(CDialog*,HGLOBAL,CWnd*);
BOOL MS_ABI impl__CreateIndirect_CDialog__UEAAHPEBUDLGTEMPLATE__PEAVCWnd__PEAX_Z(CDialog*,const DLGTEMPLATE*,CWnd*,void*);
BOOL MS_ABI impl__PreTranslateMessage_CDialog__UEAAHPEAUtagMSG___Z(CDialog*,MSG*);
BOOL MS_ABI impl__OnCmdMsg_CDialog__UEAAHIHPEAXPEAUAFX_CMDHANDLERINFO___Z(CDialog*,UINT,int,void*,AFX_CMDHANDLERINFO*);
BOOL MS_ABI impl__CheckAutoCenter_CDialog__UEAAHXZ(CDialog*);
}
INT_PTR CDialog::DoModal(){return impl__DoModal_CDialog__UEAA_JXZ(this);}
BOOL CDialog::Create(const wchar_t* name,CWnd* parent){return impl__Create_CDialog__UEAAHPEB_WPEAVCWnd___Z(this,name,parent);}
BOOL CDialog::Create(UINT id,CWnd* parent){return impl__Create_CDialog__UEAAHI_PEAVCWnd___Z(this,id,parent);}
BOOL CDialog::CreateIndirect(HGLOBAL data,CWnd* parent){return impl__CreateIndirect_CDialog__UEAAHPEAXPEAVCWnd___Z(this,data,parent);}
BOOL CDialog::CreateIndirect(const DLGTEMPLATE* data,CWnd* parent,void* init){return impl__CreateIndirect_CDialog__UEAAHPEBUDLGTEMPLATE__PEAVCWnd__PEAX_Z(this,data,parent,init);}
BOOL CDialog::PreTranslateMessage(MSG* message){return impl__PreTranslateMessage_CDialog__UEAAHPEAUtagMSG___Z(this,message);}
BOOL CDialog::OnCmdMsg(UINT id,int code,void* extra,AFX_CMDHANDLERINFO* info){return impl__OnCmdMsg_CDialog__UEAAHIHPEAXPEAUAFX_CMDHANDLERINFO___Z(this,id,code,extra,info);}
BOOL CDialog::CheckAutoCenter(){return impl__CheckAutoCenter_CDialog__UEAAHXZ(this);}
