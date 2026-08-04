// CMFCRibbonCmdUI — OpenMFC implementation.
// Sources: global_cmfcribboncmdui.cpp

#include "detail/CMFCRibbonCmdUISupport.h"

// Symbol: ?Enable@CMFCRibbonCmdUI@@UEAAXH@Z
extern "C" void MS_ABI impl__Enable_CMFCRibbonCmdUI__UEAAXH_Z(void* pThis, int bOn)
{
    S_Cmfcribboncmdui* p = static_cast<S_Cmfcribboncmdui*>(pThis);
    // CCmdUI::Enable core behavior: record that an explicit enable state was set,
    // so the update machinery knows the handler took control of the item.
    p->m_bEnableChanged = TRUE;
    // The ribbon element itself is enabled/disabled by the ribbon update pass;
    // there is no reachable per-element enable mutator to forward to here.
    (void)bOn;
}
// Symbol: ?SetCheck@CMFCRibbonCmdUI@@UEAAXH@Z
extern "C" void MS_ABI impl__SetCheck_CMFCRibbonCmdUI__UEAAXH_Z(void* pThis, int nCheck)
{
    S_Cmfcribboncmdui* p = static_cast<S_Cmfcribboncmdui*>(pThis);
    // nCheck: 0 => off, 1 => on, 2 => indeterminate. Real MFC pushes the checked
    // state onto the associated ribbon element; that element mutator is not part
    // of the exported surface, so we validate and guard the target here.
    if (p->m_pUpdated != nullptr) {
        // no reachable element check-state setter to forward to
        (void)nCheck;
    }
}
// Symbol: ?SetRadio@CMFCRibbonCmdUI@@UEAAXH@Z
extern "C" void MS_ABI impl__SetRadio_CMFCRibbonCmdUI__UEAAXH_Z(void* pThis, int bOn)
{
    // CCmdUI::SetRadio simply forwards to SetCheck with the boolean state.
    impl__SetCheck_CMFCRibbonCmdUI__UEAAXH_Z(pThis, bOn ? 1 : 0);
}
// Symbol: ?SetText@CMFCRibbonCmdUI@@UEAAXPEB_W@Z
extern "C" void MS_ABI impl__SetText_CMFCRibbonCmdUI__UEAAXPEB_W_Z(void* pThis, const wchar_t* lpszText)
{
    S_Cmfcribboncmdui* p = static_cast<S_Cmfcribboncmdui*>(pThis);
    // Forward the new text to the associated ribbon element, which owns the
    // displayed caption (real MFC calls m_pUpdated->SetText).
    if (p->m_pUpdated != nullptr) {
        impl__SetText_CMFCRibbonBaseElement__UEAAXPEB_W_Z(p->m_pUpdated, lpszText);
    }
}
// Symbol: ??0CMFCRibbonCmdUI@@QEAA@XZ
extern "C" void* MS_ABI impl___0CMFCRibbonCmdUI__QEAA_XZ(void* pThis)
{
    S_Cmfcribboncmdui* p = static_cast<S_Cmfcribboncmdui*>(pThis);
    p->vfptr             = reinterpret_cast<void*>(const_cast<void**>(&g_CMFCRibbonCmdUI_vtbl[0]));
    p->m_nID             = 0;
    p->m_nIndex          = 0;
    p->m_pMenu           = nullptr;
    p->m_pSubMenu        = nullptr;
    p->m_pParentMenu     = nullptr;
    p->m_nIndexMax       = 0;
    p->m_bEnableChanged  = FALSE;
    p->m_bContinueRouting= FALSE;
    p->m_pOther          = nullptr;
    p->m_pUpdated        = nullptr;   // CMFCRibbonCmdUI() { m_pUpdated = NULL; }
    return pThis;
}

// CMFCRibbonCmdUI's MSVC-layout vtable: defined here, next to the
// implementations it points at.
extern void* const g_CMFCRibbonCmdUI_vtbl[4] = {
    reinterpret_cast<void*>(&vt_Enable_Cmfcribboncmdui),
    reinterpret_cast<void*>(&vt_SetCheck_Cmfcribboncmdui),
    reinterpret_cast<void*>(&vt_SetRadio_Cmfcribboncmdui),
    reinterpret_cast<void*>(&vt_SetText_Cmfcribboncmdui),
};
