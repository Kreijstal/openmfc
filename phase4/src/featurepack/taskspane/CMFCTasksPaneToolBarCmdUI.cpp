// CMFCTasksPaneToolBarCmdUI — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp

#include "detail/ManualSmallStubImplementationsSupport.h"

// Symbol: ?Enable@CMFCTasksPaneToolBarCmdUI@@UEAAXH@Z
extern "C" void MS_ABI impl__Enable_CMFCTasksPaneToolBarCmdUI__UEAAXH_Z(void* pThis, int bOn) {
    if (auto* pUI = static_cast<CCmdUI*>(pThis)) {
        pUI->Enable(bOn != 0);
    }
}
// Symbol: ?SetText@CMFCTasksPaneToolBarCmdUI@@UEAAXPEB_W@Z
extern "C" void MS_ABI impl__SetText_CMFCTasksPaneToolBarCmdUI__UEAAXPEB_W_Z(void* pThis, const wchar_t* lpszText) {
    if (auto* pUI = static_cast<CCmdUI*>(pThis)) {
        pUI->SetText(lpszText);
    }
}
