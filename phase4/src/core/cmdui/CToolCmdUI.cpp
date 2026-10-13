#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxpriv.h"
#ifdef __GNUC__
#define MS_ABI __attribute__((ms_abi))
#else
#define MS_ABI
#endif
extern "C" UINT MS_ABI impl__GetButtonStyle_CToolBar__QEBAIH_Z(const void*,int);
extern "C" void MS_ABI impl__SetButtonStyle_CToolBar__QEAAXHI_Z(void*,int,UINT);
void CToolCmdUI::Enable(BOOL enabled) {
    m_bEnableChanged=TRUE;
    UINT style=impl__GetButtonStyle_CToolBar__QEBAIH_Z(m_pOther,m_nIndex)&~0x40000u;
    if(!enabled)style=(style&~0x20000u)|0x40000u;
    impl__SetButtonStyle_CToolBar__QEAAXHI_Z(m_pOther,m_nIndex,style);
}
void CToolCmdUI::SetCheck(int check) {
    UINT style=impl__GetButtonStyle_CToolBar__QEBAIH_Z(m_pOther,m_nIndex)&~0x110000u;
    if(check==1)style|=0x10000u;
    else if(check==2)style|=0x100000u;
    impl__SetButtonStyle_CToolBar__QEAAXHI_Z(m_pOther,m_nIndex,style|2u);
}
// Symbol: ?Enable@CToolCmdUI@@UEAAXH@Z
extern "C" void MS_ABI impl__Enable_CToolCmdUI__UEAAXH_Z(CToolCmdUI* self,int enabled) { self->CToolCmdUI::Enable(enabled); }
// Symbol: ?SetCheck@CToolCmdUI@@UEAAXH@Z
extern "C" void MS_ABI impl__SetCheck_CToolCmdUI__UEAAXH_Z(CToolCmdUI* self,int check) { self->CToolCmdUI::SetCheck(check); }
// Symbol: ?SetText@CToolCmdUI@@UEAAXPEB_W@Z
extern "C" void MS_ABI impl__SetText_CToolCmdUI__UEAAXPEB_W_Z(CToolCmdUI* self,const wchar_t* text) { self->CToolCmdUI::SetText(text); }
