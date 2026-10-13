// Native OLE command-state translation and owning CString text storage.
#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxole.h"
#include <new>
#ifdef __GNUC__
#define MS_ABI __attribute__((ms_abi))
#else
#define MS_ABI
#endif
extern "C" void MS_ABI impl__AfxThrowInvalidArgException__YAXXZ();
COleCmdUI::COleCmdUI(OLECMD* commands,ULONG count,const GUID* group)
    : m_rgCmds(commands),m_pguidCmdGroup(group) { m_nIndexMax=count; }
COleCmdUI::~COleCmdUI() = default;
void COleCmdUI::Enable(BOOL enabled) {
    if(!m_rgCmds)return;
    if(enabled)m_rgCmds[m_nIndex].cmdf|=OLECMDF_ENABLED;
    else m_rgCmds[m_nIndex].cmdf&=~OLECMDF_ENABLED;
    m_bEnableChanged=TRUE;
}
void COleCmdUI::SetCheck(int check) {
    if(!m_rgCmds)return;
    DWORD& flags=m_rgCmds[m_nIndex].cmdf;
    flags&=~(OLECMDF_LATCHED|OLECMDF_NINCHED);
    if(check==1)flags|=OLECMDF_LATCHED;
    else if(check==2)flags|=OLECMDF_NINCHED;
}
void COleCmdUI::SetText(const wchar_t* text) { m_strText=text; }
int COleCmdUI::DoUpdate(CCmdTarget* target,BOOL disableIfNoHandler) {
    if(!target) { impl__AfxThrowInvalidArgException__YAXXZ();return FALSE; }
    m_bEnableChanged=FALSE;
    BOOL handled=target->OnCmdMsg(m_nID,CN_OLECOMMAND,this,nullptr);
    if(handled)handled=target->OnCmdMsg(m_nID,CN_UPDATE_COMMAND_UI,this,nullptr);
    if(disableIfNoHandler && !m_bEnableChanged) {
        AFX_CMDHANDLERINFO info{};
        handled=target->OnCmdMsg(m_nID,CN_COMMAND,this,&info);
        DWORD& flags=m_rgCmds[m_nIndex].cmdf;
        if(!handled && !m_bEnableChanged)flags&=~OLECMDF_SUPPORTED;else flags|=OLECMDF_SUPPORTED;
        Enable(handled);
    } else {
        DWORD& flags=m_rgCmds[m_nIndex].cmdf;
        if(m_bEnableChanged)flags|=OLECMDF_SUPPORTED;else flags&=~OLECMDF_SUPPORTED;
    }
    return handled;
}
static_assert(sizeof(COleCmdUI)==96,"native OLE command UI");
// Symbol: ??0COleCmdUI@@QEAA@PEAU_tagOLECMD@@KPEBU_GUID@@@Z
extern "C" void* MS_ABI impl___0COleCmdUI__QEAA_PEAU_tagOLECMD__KPEBU_GUID___Z(void* storage,OLECMD* commands,ULONG count,const GUID* group) { return new(storage) COleCmdUI(commands,count,group); }
// Symbol: ?DoUpdate@COleCmdUI@@UEAAHPEAVCCmdTarget@@H@Z
extern "C" int MS_ABI impl__DoUpdate_COleCmdUI__UEAAHPEAVCCmdTarget__H_Z(COleCmdUI* self,CCmdTarget* target,int disable) { return self->COleCmdUI::DoUpdate(target,disable); }
// Symbol: ?Enable@COleCmdUI@@UEAAXH@Z
extern "C" void MS_ABI impl__Enable_COleCmdUI__UEAAXH_Z(COleCmdUI* self,int enabled) { self->COleCmdUI::Enable(enabled); }
// Symbol: ?SetCheck@COleCmdUI@@UEAAXH@Z
extern "C" void MS_ABI impl__SetCheck_COleCmdUI__UEAAXH_Z(COleCmdUI* self,int check) { self->COleCmdUI::SetCheck(check); }
// Symbol: ?SetText@COleCmdUI@@UEAAXPEB_W@Z
extern "C" void MS_ABI impl__SetText_COleCmdUI__UEAAXPEB_W_Z(COleCmdUI* self,const wchar_t* text) { self->COleCmdUI::SetText(text); }
