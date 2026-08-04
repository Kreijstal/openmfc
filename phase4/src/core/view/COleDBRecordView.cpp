// COleDBRecordView — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp, olecore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/ManualSmallStubImplementationsSupport.h"
#include "detail/OlecoreSupport.h"


// Implementations this unit calls that are defined with their own class.
extern "C" void MS_ABI impl__Enable_CCmdUI__UEAAXH_Z(CCmdUI* pThis, int enable);

// Symbol: ?OnInitialUpdate@COleDBRecordView@@UEAAXXZ
extern "C" void MS_ABI impl__OnInitialUpdate_COleDBRecordView__UEAAXXZ(void* pThis) {
    if (auto* pView = static_cast<CRecordView*>(pThis)) {
        pView->OnInitialUpdate();
    }
}
// Symbol: ?OnMove@COleDBRecordView@@UEAAHI@Z
extern "C" int MS_ABI impl__OnMove_COleDBRecordView__UEAAHI_Z(void* pThis, unsigned int nIDMoveCommand) {
    if (auto* pView = static_cast<CRecordView*>(pThis)) {
        return static_cast<int>(pView->OnMove(nIDMoveCommand));
    }
    return 0;
}
// Symbol: ?OnUpdateRecordFirst@COleDBRecordView@@IEAAXPEAVCCmdUI@@@Z
extern "C" void MS_ABI impl__OnUpdateRecordFirst_COleDBRecordView__IEAAXPEAVCCmdUI___Z(void*, CCmdUI* ui) {
    if (ui) impl__Enable_CCmdUI__UEAAXH_Z(ui, TRUE);
}
// Symbol: ?OnUpdateRecordLast@COleDBRecordView@@IEAAXPEAVCCmdUI@@@Z
extern "C" void MS_ABI impl__OnUpdateRecordLast_COleDBRecordView__IEAAXPEAVCCmdUI___Z(void*, CCmdUI* ui) {
    if (ui) impl__Enable_CCmdUI__UEAAXH_Z(ui, TRUE);
}
// Symbol: ?OnUpdateRecordNext@COleDBRecordView@@IEAAXPEAVCCmdUI@@@Z
extern "C" void MS_ABI impl__OnUpdateRecordNext_COleDBRecordView__IEAAXPEAVCCmdUI___Z(void*, CCmdUI* ui) {
    if (ui) impl__Enable_CCmdUI__UEAAXH_Z(ui, TRUE);
}
// Symbol: ?OnUpdateRecordPrev@COleDBRecordView@@IEAAXPEAVCCmdUI@@@Z
extern "C" void MS_ABI impl__OnUpdateRecordPrev_COleDBRecordView__IEAAXPEAVCCmdUI___Z(void*, CCmdUI* ui) {
    if (ui) impl__Enable_CCmdUI__UEAAXH_Z(ui, TRUE);
}
