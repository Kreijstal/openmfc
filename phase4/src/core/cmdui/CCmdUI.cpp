// CCmdUI — OpenMFC implementation.
// Sources: menucore.cpp, olecore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/CMenuSupport.h"
#include "detail/OlecoreSupport.h"

// CCmdUI::DoUpdate
// Symbol: ?DoUpdate@CCmdUI@@QEAAHPEAVCCmdTarget@@H@Z
extern "C" int MS_ABI impl__DoUpdate_CCmdUI__QEAAHPEAVCCmdTarget__H_Z(CCmdUI* pThis, CCmdTarget* pTarget, int bDisableIfNoHndler) {
    if (!pThis) return FALSE;

    auto* ui = reinterpret_cast<CCmdUIShim_Menucore*>(pThis);
    CMenu* pMenu = ui->m_pSubMenu ? ui->m_pSubMenu : ui->m_pMenu;
    if (pMenu && ui->m_nID != 0U && ui->m_nID != static_cast<UINT>(-1)) {
        UINT enableFlags = MF_BYCOMMAND;
        if (!pTarget && bDisableIfNoHndler) {
            enableFlags |= MF_DISABLED | MF_GRAYED;
        } else {
            enableFlags |= MF_ENABLED;
        }
        ::EnableMenuItem(pMenu->m_hMenu, ui->m_nID, enableFlags);
    }

    return TRUE;
}
// CCmdUI::SetRadio
// Symbol: ?SetRadio@CCmdUI@@UEAAXH@Z
extern "C" void MS_ABI impl__SetRadio_CCmdUI__UEAAXH_Z(CCmdUI* pThis, int bOn) {
    if (!pThis) return;

    auto* ui = reinterpret_cast<CCmdUIShim_Menucore*>(pThis);
    CMenu* pMenu = ui->m_pSubMenu ? ui->m_pSubMenu : ui->m_pMenu;
    if (!pMenu || ui->m_nID == 0U || ui->m_nID == static_cast<UINT>(-1)) return;

    const UINT checkState = MF_BYCOMMAND | (bOn ? MF_CHECKED : MF_UNCHECKED);
    ::CheckMenuItem(pMenu->m_hMenu, ui->m_nID, checkState);
}
// Symbol: ?Enable@CCmdUI@@UEAAXH@Z
// Ordinal: 4188
extern "C" void MS_ABI impl__Enable_CCmdUI__UEAAXH_Z(CCmdUI* pThis, int enable) {
    if (pThis) pThis->Enable(enable != 0);
}
// Symbol: ?SetCheck@CCmdUI@@UEAAXH@Z
// Ordinal: 4192
extern "C" void MS_ABI impl__SetCheck_CCmdUI__UEAAXH_Z(CCmdUI* pThis, int nCheck) {
    if (pThis) pThis->SetCheck(nCheck);
}
// Symbol: ?SetText@CCmdUI@@UEAAXPEB_W@Z
// Ordinal: 4196
extern "C" void MS_ABI impl__SetText_CCmdUI__UEAAXPEB_W_Z(CCmdUI* pThis, const wchar_t* lpszText) {
    if (pThis) pThis->SetText(lpszText);
}
void CCmdUI::Enable(BOOL bOn) { CCmdUIEnableForMenu(this, bOn); }
void CCmdUI::SetCheck(int nCheck) { CCmdUICheckForMenu(this, nCheck); }
void CCmdUI::SetText(const wchar_t* lpszText) { CCmdUISetTextForMenu(this, lpszText); }
