// CMFCStatusBarCmdUI — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp

#include "detail/ManualSmallStubImplementationsSupport.h"

// Symbol: ?Enable@CMFCStatusBarCmdUI@@UEAAXH@Z
extern "C" void MS_ABI impl__Enable_CMFCStatusBarCmdUI__UEAAXH_Z(void* pThis, int bEnable) {
    if (auto* ui = static_cast<CCmdUI*>(pThis)) {
        ui->Enable(bEnable != 0);
    }
}
// Symbol: ?SetCheck@CMFCStatusBarCmdUI@@UEAAXH@Z
extern "C" void MS_ABI impl__SetCheck_CMFCStatusBarCmdUI__UEAAXH_Z(void* pThis, int check) {
    if (auto* ui = static_cast<CCmdUI*>(pThis)) {
        ui->SetCheck(check != 0);
    }
}
// Symbol: ?SetText@CMFCStatusBarCmdUI@@UEAAXPEB_W@Z
extern "C" void MS_ABI impl__SetText_CMFCStatusBarCmdUI__UEAAXPEB_W_Z(void* pThis, const wchar_t* text) {
    if (auto* ui = static_cast<CCmdUI*>(pThis)) {
        ui->SetText(text);
    }
}
