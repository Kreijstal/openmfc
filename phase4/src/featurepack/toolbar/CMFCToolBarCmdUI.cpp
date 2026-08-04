// CMFCToolBarCmdUI — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp

#include "detail/ManualSmallStubImplementationsSupport.h"

// Symbol: ?Enable@CMFCToolBarCmdUI@@UEAAXH@Z
extern "C" void MS_ABI impl__Enable_CMFCToolBarCmdUI__UEAAXH_Z(void* pThis, int bEnable) {
    if (auto* ui = static_cast<CCmdUI*>(pThis)) {
        ui->Enable(bEnable != 0);
    }
}
// Symbol: ?SetCheck@CMFCToolBarCmdUI@@UEAAXH@Z
extern "C" void MS_ABI impl__SetCheck_CMFCToolBarCmdUI__UEAAXH_Z(void* pThis, int bChecked) {
    if (auto* ui = static_cast<CCmdUI*>(pThis)) {
        ui->SetCheck(bChecked != 0);
    }
}
// Symbol: ?SetRadio@CMFCToolBarCmdUI@@UEAAXH@Z
extern "C" void MS_ABI impl__SetRadio_CMFCToolBarCmdUI__UEAAXH_Z(void* pThis, int bCheck) {
    if (auto* ui = static_cast<CCmdUI*>(pThis)) {
        ui->SetCheck(bCheck != 0);
    }
}
// Symbol: ?SetText@CMFCToolBarCmdUI@@UEAAXPEB_W@Z
extern "C" void MS_ABI impl__SetText_CMFCToolBarCmdUI__UEAAXPEB_W_Z(void* pThis, const wchar_t* text) {
    if (auto* ui = static_cast<CCmdUI*>(pThis)) {
        ui->SetText(text);
    }
}
