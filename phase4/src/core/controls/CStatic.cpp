// CStatic — OpenMFC implementation.
// Sources: ctrl_ownerdraw.cpp, ctrlcore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/CtrlOwnerdrawSupport.h"
#include "detail/CtrlcoreSupport.h"

// Symbol: ?DrawItem@CStatic@@UEAAXPEAUtagDRAWITEMSTRUCT@@@Z
extern "C" void MS_ABI impl__DrawItem_CStatic__UEAAXPEAUtagDRAWITEMSTRUCT___Z(
    CStatic* pThis, DRAWITEMSTRUCT* pDraw) {
    (void)pThis;
    FillOwnerDrawItem(pDraw);
}
// Symbol: ?GetRuntimeClass@CStatic@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CStatic__UEBAPEAUCRuntimeClass__XZ(
    const CStatic* pThis) {
    return CStatic::GetThisClass();
}
// Symbol: ?GetThisClass@CStatic@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CStatic__SAPEAUCRuntimeClass__XZ() {
    return CStatic::GetThisClass();
}
// Symbol: ?OnChildNotify@CStatic@@MEAAHI_K_JPEA_J@Z
extern "C" int MS_ABI impl__OnChildNotify_CStatic__MEAAHI_K_JPEA_J_Z(
    CStatic* pThis, UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult) {
    (void)wParam;
    if (pResult) *pResult = 0;
    if (message == WM_DRAWITEM && lParam) {
        impl__DrawItem_CStatic__UEAAXPEAUtagDRAWITEMSTRUCT___Z(pThis, (DRAWITEMSTRUCT*)lParam);
        if (pResult) *pResult = TRUE;
        return TRUE;
    }
    return FALSE;
}
