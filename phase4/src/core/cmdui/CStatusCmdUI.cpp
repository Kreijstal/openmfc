// CStatusCmdUI — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp

#include "detail/ManualSmallStubImplementationsSupport.h"

// Symbol: ?Enable@CStatusCmdUI@@UEAAXH@Z
extern "C" void MS_ABI impl__Enable_CStatusCmdUI__UEAAXH_Z(void* pThis, int bEnable) {
    if (auto* ui = static_cast<CCmdUI*>(pThis)) {
        ui->Enable(bEnable != 0);
    }
}
// Symbol: ?SetCheck@CStatusCmdUI@@UEAAXH@Z
extern "C" void MS_ABI impl__SetCheck_CStatusCmdUI__UEAAXH_Z(void* pThis, int check) {
    if (auto* ui = static_cast<CCmdUI*>(pThis)) {
        ui->SetCheck(check != 0);
    }
}
// Symbol: ?SetText@CStatusCmdUI@@UEAAXPEB_W@Z
extern "C" void MS_ABI impl__SetText_CStatusCmdUI__UEAAXPEB_W_Z(void* pThis, const wchar_t* text) {
    if (auto* ui = static_cast<CCmdUI*>(pThis)) {
        ui->SetText(text);
    }
}
