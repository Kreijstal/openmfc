// CMFCRibbonQuickAccessToolBarDefaultState — OpenMFC implementation.
// Sources: global_cmfcribbonquickaccesstoolbardefaultstate.cpp

#include "detail/CMFCRibbonQuickAccessToolBarDefaultStateSupport.h"

// Symbol: ??0CMFCRibbonQuickAccessToolBarDefaultState@@QEAA@XZ
extern "C" void* MS_ABI
impl___0CMFCRibbonQuickAccessToolBarDefaultState__QEAA_XZ(void* pThis)
{
    S_Cmfcribbonquickaccesstoolbardefaultstate* s = (S_Cmfcribbonquickaccesstoolbardefaultstate*)pThis;
    // Both CArray members default-construct to all-zero (null vfptr, null data,
    // zero sizes/growby) exactly like CObject-derived CArray::CArray().
    memset(s, 0, sizeof(S_Cmfcribbonquickaccesstoolbardefaultstate));
    return pThis;
}
// Symbol: ?AddCommand@CMFCRibbonQuickAccessToolBarDefaultState@@QEAAXIH@Z
extern "C" void MS_ABI
impl__AddCommand_CMFCRibbonQuickAccessToolBarDefaultState__QEAAXIH_Z(
    void* pThis, unsigned uiCmd, int bIsVisible)
{
    S_Cmfcribbonquickaccesstoolbardefaultstate* s = (S_Cmfcribbonquickaccesstoolbardefaultstate*)pThis;
    arr_Add4(&s->m_arCommands, uiCmd);
    arr_Add4(&s->m_arVisibleState, (unsigned)bIsVisible);
}
// Symbol: ?RemoveAll@CMFCRibbonQuickAccessToolBarDefaultState@@QEAAXXZ
extern "C" void MS_ABI
impl__RemoveAll_CMFCRibbonQuickAccessToolBarDefaultState__QEAAXXZ(void* pThis)
{
    S_Cmfcribbonquickaccesstoolbardefaultstate* s = (S_Cmfcribbonquickaccesstoolbardefaultstate*)pThis;
    arr_SetSize(&s->m_arCommands, 0, 4);
    arr_SetSize(&s->m_arVisibleState, 0, 4);
}
// Symbol: ?CopyFrom@CMFCRibbonQuickAccessToolBarDefaultState@@QEAAXAEBV1@@Z
extern "C" void MS_ABI
impl__CopyFrom_CMFCRibbonQuickAccessToolBarDefaultState__QEAAXAEBV1__Z(
    void* pThis, const void* pSrc)
{
    S_Cmfcribbonquickaccesstoolbardefaultstate* s = (S_Cmfcribbonquickaccesstoolbardefaultstate*)pThis;
    const S_Cmfcribbonquickaccesstoolbardefaultstate* src = (const S_Cmfcribbonquickaccesstoolbardefaultstate*)pSrc;
    arr_Copy(&s->m_arCommands, &src->m_arCommands, 4);
    arr_Copy(&s->m_arVisibleState, &src->m_arVisibleState, 4);
}
