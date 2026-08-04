// CToolCmdUI — OpenMFC implementation.
// Sources: cbarcore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/CbarcoreSupport.h"

// Symbol: ?Enable@CToolCmdUI@@UEAAXH@Z
extern "C" void MS_ABI impl__Enable_CToolCmdUI__UEAAXH_Z(CCmdUI* pThis, int bOn) {
    if (pThis) pThis->Enable(bOn);
}
// Symbol: ?SetCheck@CToolCmdUI@@UEAAXH@Z
extern "C" void MS_ABI impl__SetCheck_CToolCmdUI__UEAAXH_Z(CCmdUI* pThis, int nCheck) {
    if (pThis) pThis->SetCheck(nCheck);
}
// Symbol: ?SetText@CToolCmdUI@@UEAAXPEB_W@Z
extern "C" void MS_ABI impl__SetText_CToolCmdUI__UEAAXPEB_W_Z(CCmdUI* pThis, const wchar_t* lpszText) {
    if (pThis) pThis->SetText(lpszText);
}
