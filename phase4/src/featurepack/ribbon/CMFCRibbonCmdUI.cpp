// CMFCRibbonCmdUI — OpenMFC implementation.
// Sources: global_cmfcribboncmdui.cpp

#include "detail/CMFCRibbonCmdUISupport.h"

// Retail x64 element fields and virtual slots are shared by mfc140/mfc140u.
// Use the native object passed by the client: the provisional public ribbon
// declarations do not yet model the full CMFCRibbonBaseElement ABI.
#include <cstring>
namespace {
template<class T> T Read(const void* object, size_t offset) {
    T value; std::memcpy(&value, static_cast<const char*>(object)+offset, sizeof(value)); return value;
}
template<class T> void Write(void* object, size_t offset, T value) {
    std::memcpy(static_cast<char*>(object)+offset, &value, sizeof(value));
}
template<class T> T Virtual(void* object, size_t slot) {
    return reinterpret_cast<T>(Read<void* const*>(object,0)[slot]);
}
void Redraw(S_Cmfcribboncmdui* ui) {
    RECT rect=Read<RECT>(ui->m_pUpdated,0xc8);
    ::RedrawWindow(Read<HWND>(ui->m_pOther,0x40), &rect, nullptr,
                   RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE);
}
using Query=BOOL (MS_ABI*)(const void*);
using Notify=void (MS_ABI*)(void*,BOOL);
}
extern "C" void MS_ABI impl__AfxThrowInvalidArgException__YAXXZ();

// Symbol: ?Enable@CMFCRibbonCmdUI@@UEAAXH@Z
extern "C" void MS_ABI impl__Enable_CMFCRibbonCmdUI__UEAAXH_Z(void* pThis, int bOn)
{
    auto* ui=static_cast<S_Cmfcribboncmdui*>(pThis);
    ui->m_bEnableChanged=TRUE;
    const BOOL disabled=!bOn;
    if(Virtual<Query>(ui->m_pUpdated,57)(ui->m_pUpdated)==disabled)return;
    Write<BOOL>(ui->m_pUpdated,0x148,disabled);
    Virtual<Notify>(ui->m_pUpdated,144)(ui->m_pUpdated,!!bOn);
    Redraw(ui);
}
// Symbol: ?SetCheck@CMFCRibbonCmdUI@@UEAAXH@Z
extern "C" void MS_ABI impl__SetCheck_CMFCRibbonCmdUI__UEAAXH_Z(void* pThis, int nCheck)
{
    auto* ui=static_cast<S_Cmfcribboncmdui*>(pThis);
    if(Virtual<Query>(ui->m_pUpdated,58)(ui->m_pUpdated)==nCheck)return;
    Write<BOOL>(ui->m_pUpdated,0x14c,nCheck);
    Virtual<Notify>(ui->m_pUpdated,145)(ui->m_pUpdated,nCheck);
    Redraw(ui);
}
// Symbol: ?SetRadio@CMFCRibbonCmdUI@@UEAAXH@Z
extern "C" void MS_ABI impl__SetRadio_CMFCRibbonCmdUI__UEAAXH_Z(void* pThis, int bOn)
{
    auto* ui=static_cast<S_Cmfcribboncmdui*>(pThis);
    Write<BOOL>(ui->m_pUpdated,0x150,bOn);
    Virtual<Notify>(ui,1)(ui,!!bOn);
}
// Symbol: ?SetText@CMFCRibbonCmdUI@@UEAAXPEB_W@Z
extern "C" void MS_ABI impl__SetText_CMFCRibbonCmdUI__UEAAXPEB_W_Z(void* pThis, const wchar_t* lpszText)
{
    if(!lpszText) {impl__AfxThrowInvalidArgException__YAXXZ();return;}
    auto* ui=static_cast<S_Cmfcribboncmdui*>(pThis);
    if(::lstrcmpW(Read<const wchar_t*>(ui->m_pUpdated,0xa0),lpszText)==0)return;
    using SetText=void (MS_ABI*)(void*,const wchar_t*);
    Virtual<SetText>(ui->m_pUpdated,47)(ui->m_pUpdated,lpszText);
    Redraw(ui);
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
