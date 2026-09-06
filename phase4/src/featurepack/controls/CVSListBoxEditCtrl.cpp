// CVSListBoxEditCtrl — OpenMFC implementation.
// Sources: dlgcommon.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/DlgcommonSupport.h"

// GetMessageMap/GetThisMessageMap for CVSListBoxEditCtrl live in
// featurepack/controls/MessageMaps.cpp (classCVSListBoxEditCtrl_msgmap, base map
// CMFCEditBrowseCtrl). Retail agrees: ?GetThisMessageMap@CVSListBoxEditCtrl@@KAPEBU
// AFX_MSGMAP@@XZ in mfc140.dll returns the AFX_MSGMAP at 0x18031de78, whose pfnGetBaseMap
// is ?GetThisMessageMap@CMFCEditBrowseCtrl@@KAPEBUAFX_MSGMAP@@XZ. The copies that used to
// be here returned CEdit's map, skipping CMFCEditBrowseCtrl.
// Symbol: ?GetRuntimeClass@CVSListBoxEditCtrl@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CVSListBoxEditCtrl__UEBAPEAUCRuntimeClass__XZ(const CVSListBoxEditCtrl* pThis) {
    return pThis ? pThis->GetRuntimeClass() : CVSListBoxEditCtrl::GetThisClass();
}
// Symbol: ?GetThisClass@CVSListBoxEditCtrl@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CVSListBoxEditCtrl__SAPEAUCRuntimeClass__XZ() {
    return CVSListBoxEditCtrl::GetThisClass();
}
// Symbol: ?LockSize@CVSListBoxEditCtrl@@QEAAXPEAVCVSListBoxBase@@H@Z
extern "C" void MS_ABI impl__LockSize_CVSListBoxEditCtrl__QEAAXPEAVCVSListBoxBase__H_Z(CVSListBoxEditCtrl* pThis, CVSListBoxBase*, int) {
    (void)pThis;
}
// Symbol: ?OnKeyDown@CVSListBoxEditCtrl@@IEAAXIII@Z
extern "C" void MS_ABI impl__OnKeyDown_CVSListBoxEditCtrl__IEAAXIII_Z(CVSListBoxEditCtrl* pThis, unsigned int nChar, unsigned int, unsigned int) {
    if (pThis && pThis->GetSafeHwnd()) ::SendMessageW(pThis->GetSafeHwnd(), WM_KEYDOWN, nChar, 0);
}
// Symbol: ?OnNcCalcSize@CVSListBoxEditCtrl@@IEAAXHPEAUtagNCCALCSIZE_PARAMS@@@Z
extern "C" void MS_ABI impl__OnNcCalcSize_CVSListBoxEditCtrl__IEAAXHPEAUtagNCCALCSIZE_PARAMS___Z(CVSListBoxEditCtrl*, int, NCCALCSIZE_PARAMS*) {
}
// Symbol: ?OnWindowPosChanging@CVSListBoxEditCtrl@@IEAAXPEAUtagWINDOWPOS@@@Z
extern "C" void MS_ABI impl__OnWindowPosChanging_CVSListBoxEditCtrl__IEAAXPEAUtagWINDOWPOS___Z(CVSListBoxEditCtrl*, WINDOWPOS*) {
}
