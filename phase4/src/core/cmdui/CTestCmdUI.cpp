// CTestCmdUI — OpenMFC implementation.
// Sources: global_ctestcmdui.cpp

#include "detail/CTestCmdUISupport.h"

// Symbol: ??0CTestCmdUI@@QEAA@XZ
extern "C" void* MS_ABI impl___0CTestCmdUI__QEAA_XZ(void* pThis) {
    if (!pThis) {
        return nullptr;
    }
    auto* ui = static_cast<CCmdUIShim_Ctestcmdui*>(pThis);
    std::memset(ui, 0, sizeof(*ui));
    ui->vfptr = g_CTestCmdUI_vtbl;
    return ui;
}
// Symbol: ?Enable@CTestCmdUI@@UEAAXH@Z
extern "C" void MS_ABI impl__Enable_CTestCmdUI__UEAAXH_Z(CCmdUI* pThis, int bOn) {
    if (!pThis) {
        return;
    }
    auto* ui = reinterpret_cast<CCmdUIShim_Ctestcmdui*>(pThis);
    if (!ui->m_pMenu && !ui->m_pSubMenu) {
        return;
    }
    pThis->CCmdUI::Enable(bOn);
}
// Symbol: ?SetCheck@CTestCmdUI@@UEAAXH@Z
extern "C" void MS_ABI impl__SetCheck_CTestCmdUI__UEAAXH_Z(CCmdUI* pThis, int nCheck) {
    if (!pThis) {
        return;
    }
    pThis->CCmdUI::SetCheck(nCheck);
}
// Symbol: ?SetRadio@CTestCmdUI@@UEAAXH@Z
extern "C" void MS_ABI impl__SetRadio_CTestCmdUI__UEAAXH_Z(CCmdUI* pThis, int bOn) {
    if (!pThis) {
        return;
    }
    pThis->CCmdUI::SetCheck(bOn ? 1 : 0);
}
// Symbol: ?SetText@CTestCmdUI@@UEAAXPEB_W@Z
extern "C" void MS_ABI impl__SetText_CTestCmdUI__UEAAXPEB_W_Z(CCmdUI* pThis, const wchar_t* lpszText) {
    if (!pThis) {
        return;
    }
    pThis->CCmdUI::SetText(lpszText);
}

// CTestCmdUI's MSVC-layout vtable: defined here, next to the
// implementations it points at.
extern void* const g_CTestCmdUI_vtbl[4] = {
    reinterpret_cast<void*>(&vt_Enable_Ctestcmdui),
    reinterpret_cast<void*>(&vt_SetCheck_Ctestcmdui),
    reinterpret_cast<void*>(&vt_SetRadio_Ctestcmdui),
    reinterpret_cast<void*>(&vt_SetText_Ctestcmdui),
};
