// CMFCRibbonButtonsGroup — OpenMFC implementation.
// Sources: mfccore.cpp, ole_olectors_exports.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/MfccoreSupport.h"

#include "openmfc/afxole.h"
#include "openmfc/afxmfc.h"

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

























CMFCRibbonButtonsGroup::CMFCRibbonButtonsGroup() { memset(_pad, 0, sizeof(_pad)); }
CMFCRibbonButtonsGroup::~CMFCRibbonButtonsGroup() {}
// Symbol: ??0CMFCRibbonButtonsGroup@@QEAA@PEAVCMFCRibbonBaseElement@@@Z
// CMFCRibbonButtonsGroup::CMFCRibbonButtonsGroup()
extern "C" void* MS_ABI impl___0CMFCRibbonButtonsGroup__QEAA_PEAVCMFCRibbonBaseElement___Z(
    void* /*p0*/) {
    return new CMFCRibbonButtonsGroup();
}
// Symbol: ?AddButton@CMFCRibbonButtonsGroup@@QEAAXPEAVCMFCRibbonBaseElement@@@Z
extern "C" void MS_ABI impl__AddButton_CMFCRibbonButtonsGroup__QEAAXPEAVCMFCRibbonBaseElement___Z(void* /*class*/* p0) {}

// Symbol: ?AddButtons@CMFCRibbonButtonsGroup@@QEAAXAEBV?$CList@PEAVCMFCRibbonBaseElement@@PEAV1@@@@Z
extern "C" void MS_ABI impl__AddButtons_CMFCRibbonButtonsGroup__QEAAXAEBV__CList_PEAVCMFCRibbonBaseElement__PEAV1____Z(const void* /*class*/* p0, void* /*class*/* p1) {}

// Symbol: ?AddToKeyList@CMFCRibbonButtonsGroup@@MEAAXAEAV?$CArray@PEAVCMFCRibbonKeyTip@@PEAV1@@@@Z
extern "C" void MS_ABI impl__AddToKeyList_CMFCRibbonButtonsGroup__MEAAXAEAV__CArray_PEAVCMFCRibbonKeyTip__PEAV1____Z(void* /*class*/* p0, void* /*class*/* p1) {}

// Symbol: ?AddToListBox@CMFCRibbonButtonsGroup@@MEAAHPEAVCMFCRibbonCommandsListBox@@H@Z
extern "C" int MS_ABI impl__AddToListBox_CMFCRibbonButtonsGroup__MEAAHPEAVCMFCRibbonCommandsListBox__H_Z(void* /*class*/* p0, int p1) {
    return 0;
}

// Symbol: ?CleanUpSizes@CMFCRibbonButtonsGroup@@MEAAXXZ
extern "C" void MS_ABI impl__CleanUpSizes_CMFCRibbonButtonsGroup__MEAAXXZ() {}

// Symbol: ?CopyFrom@CMFCRibbonButtonsGroup@@MEAAXAEBVCMFCRibbonBaseElement@@@Z
extern "C" void MS_ABI impl__CopyFrom_CMFCRibbonButtonsGroup__MEAAXAEBVCMFCRibbonBaseElement___Z(const void* /*class*/* p0) {}

// Symbol: ?Find@CMFCRibbonButtonsGroup@@MEAAPEAVCMFCRibbonBaseElement@@PEBV2@@Z
extern "C" void* MS_ABI impl__Find_CMFCRibbonButtonsGroup__MEAAPEAVCMFCRibbonBaseElement__PEBV2__Z(const void* /*class*/* p0) {
    return nullptr;
}

// Symbol: ?FindByData@CMFCRibbonButtonsGroup@@MEAAPEAVCMFCRibbonBaseElement@@_K@Z
extern "C" void* MS_ABI impl__FindByData_CMFCRibbonButtonsGroup__MEAAPEAVCMFCRibbonBaseElement___K_Z(unsigned __int64 p0) {
    return nullptr;
}

// Symbol: ?FindByID@CMFCRibbonButtonsGroup@@MEAAPEAVCMFCRibbonBaseElement@@I@Z
extern "C" void* MS_ABI impl__FindByID_CMFCRibbonButtonsGroup__MEAAPEAVCMFCRibbonBaseElement__I_Z(unsigned int p0) {
    return nullptr;
}

// Symbol: ?FindByOriginal@CMFCRibbonButtonsGroup@@MEAAPEAVCMFCRibbonBaseElement@@PEAV2@@Z
extern "C" void* MS_ABI impl__FindByOriginal_CMFCRibbonButtonsGroup__MEAAPEAVCMFCRibbonBaseElement__PEAV2__Z(void* /*class*/* p0) {
    return nullptr;
}

// Symbol: ?GetButtonIndex@CMFCRibbonButtonsGroup@@QEBAHPEBVCMFCRibbonBaseElement@@@Z
extern "C" int MS_ABI impl__GetButtonIndex_CMFCRibbonButtonsGroup__QEBAHPEBVCMFCRibbonBaseElement___Z(const void* /*class*/* p0) {
    return 0;
}

// Symbol: ?GetDroppedDown@CMFCRibbonButtonsGroup@@MEAAPEAVCMFCRibbonBaseElement@@XZ
extern "C" void* MS_ABI impl__GetDroppedDown_CMFCRibbonButtonsGroup__MEAAPEAVCMFCRibbonBaseElement__XZ() {
    return nullptr;
}

// Symbol: ?GetElementsByID@CMFCRibbonButtonsGroup@@MEAAXIAEAV?$CArray@PEAVCMFCRibbonBaseElement@@PEAV1@@@@Z
extern "C" void MS_ABI impl__GetElementsByID_CMFCRibbonButtonsGroup__MEAAXIAEAV__CArray_PEAVCMFCRibbonBaseElement__PEAV1____Z(unsigned int p0, void* /*class*/* p1, void* /*class*/* p2) {}

// Symbol: ?GetFirstTabStop@CMFCRibbonButtonsGroup@@MEAAPEAVCMFCRibbonBaseElement@@XZ
extern "C" void* MS_ABI impl__GetFirstTabStop_CMFCRibbonButtonsGroup__MEAAPEAVCMFCRibbonBaseElement__XZ() {
    return nullptr;
}

// Symbol: ?GetFocused@CMFCRibbonButtonsGroup@@MEAAPEAVCMFCRibbonBaseElement@@XZ
extern "C" void* MS_ABI impl__GetFocused_CMFCRibbonButtonsGroup__MEAAPEAVCMFCRibbonBaseElement__XZ() {
    return nullptr;
}

// Symbol: ?GetHighlighted@CMFCRibbonButtonsGroup@@MEAAPEAVCMFCRibbonBaseElement@@XZ
extern "C" void* MS_ABI impl__GetHighlighted_CMFCRibbonButtonsGroup__MEAAPEAVCMFCRibbonBaseElement__XZ() {
    return nullptr;
}

// Symbol: ?GetImageSize@CMFCRibbonButtonsGroup@@QEBA?BVCSize@@XZ
extern "C" void* MS_ABI impl__GetImageSize_CMFCRibbonButtonsGroup__QEBA_BVCSize__XZ(void* p0, void* /*class*/ p1) {
    return nullptr;
}

// Symbol: ?GetItemIDsList@CMFCRibbonButtonsGroup@@MEBAXAEAV?$CList@II@@@Z
extern "C" void MS_ABI impl__GetItemIDsList_CMFCRibbonButtonsGroup__MEBAXAEAV__CList_II___Z(void* /*class*/* p0) {}

// Symbol: ?GetLastTabStop@CMFCRibbonButtonsGroup@@MEAAPEAVCMFCRibbonBaseElement@@XZ
extern "C" void* MS_ABI impl__GetLastTabStop_CMFCRibbonButtonsGroup__MEAAPEAVCMFCRibbonBaseElement__XZ() {
    return nullptr;
}

// Symbol: ?GetPressed@CMFCRibbonButtonsGroup@@MEAAPEAVCMFCRibbonBaseElement@@XZ
extern "C" void* MS_ABI impl__GetPressed_CMFCRibbonButtonsGroup__MEAAPEAVCMFCRibbonBaseElement__XZ() {
    return nullptr;
}

// Symbol: ?GetRegularSize@CMFCRibbonButtonsGroup@@UEAA?AVCSize@@PEAVCDC@@@Z
extern "C" void* MS_ABI impl__GetRegularSize_CMFCRibbonButtonsGroup__UEAA_AVCSize__PEAVCDC___Z(void* /*class*/* p0, void* /*class*/* p1) {
    return nullptr;
}

// Symbol: ?GetVisibleElements@CMFCRibbonButtonsGroup@@MEAAXAEAV?$CArray@PEAVCMFCRibbonBaseElement@@PEAV1@@@@Z
extern "C" void MS_ABI impl__GetVisibleElements_CMFCRibbonButtonsGroup__MEAAXAEAV__CArray_PEAVCMFCRibbonBaseElement__PEAV1____Z(void* /*class*/* p0, void* /*class*/* p1) {}

// Symbol: ?HitTest@CMFCRibbonButtonsGroup@@MEAAPEAVCMFCRibbonBaseElement@@VCPoint@@@Z
extern "C" void* MS_ABI impl__HitTest_CMFCRibbonButtonsGroup__MEAAPEAVCMFCRibbonBaseElement__VCPoint___Z(void* /*class*/ p0) {
    return nullptr;
}

// Symbol: ?OnAfterChangeRect@CMFCRibbonButtonsGroup@@MEAAXPEAVCDC@@@Z
extern "C" void MS_ABI impl__OnAfterChangeRect_CMFCRibbonButtonsGroup__MEAAXPEAVCDC___Z(void* /*class*/* p0) {}

// Symbol: ?OnDraw@CMFCRibbonButtonsGroup@@MEAAXPEAVCDC@@@Z
extern "C" void MS_ABI impl__OnDraw_CMFCRibbonButtonsGroup__MEAAXPEAVCDC___Z(void* /*class*/* p0) {}

// Symbol: ?OnDrawImage@CMFCRibbonButtonsGroup@@UEAAXPEAVCDC@@VCRect@@PEAVCMFCRibbonBaseElement@@H@Z
extern "C" void MS_ABI impl__OnDrawImage_CMFCRibbonButtonsGroup__UEAAXPEAVCDC__VCRect__PEAVCMFCRibbonBaseElement__H_Z(void* /*class*/* p0, void* /*class*/ p1, void* /*class*/* p2, int p3) {}

// Symbol: ?OnRTLChanged@CMFCRibbonButtonsGroup@@MEAAXH@Z
extern "C" void MS_ABI impl__OnRTLChanged_CMFCRibbonButtonsGroup__MEAAXH_Z(int p0) {}

// Symbol: ?OnShow@CMFCRibbonButtonsGroup@@MEAAXH@Z
extern "C" void MS_ABI impl__OnShow_CMFCRibbonButtonsGroup__MEAAXH_Z(int p0) {}

// Symbol: ?OnUpdateCmdUI@CMFCRibbonButtonsGroup@@MEAAXPEAVCMFCRibbonCmdUI@@PEAVCFrameWnd@@H@Z
extern "C" void MS_ABI impl__OnUpdateCmdUI_CMFCRibbonButtonsGroup__MEAAXPEAVCMFCRibbonCmdUI__PEAVCFrameWnd__H_Z(void* /*class*/* p0, void* /*class*/* p1, int p2) {}

// Symbol: ?RemoveAll@CMFCRibbonButtonsGroup@@QEAAXXZ
extern "C" void MS_ABI impl__RemoveAll_CMFCRibbonButtonsGroup__QEAAXXZ() {}

// Symbol: ?ReplaceByID@CMFCRibbonButtonsGroup@@MEAAHIPEAVCMFCRibbonBaseElement@@@Z
extern "C" int MS_ABI impl__ReplaceByID_CMFCRibbonButtonsGroup__MEAAHIPEAVCMFCRibbonBaseElement___Z(unsigned int p0, void* /*class*/* p1) {
    return 0;
}

// Symbol: ?SetImages@CMFCRibbonButtonsGroup@@QEAAXPEAVCMFCToolBarImages@@00@Z
extern "C" void MS_ABI impl__SetImages_CMFCRibbonButtonsGroup__QEAAXPEAVCMFCToolBarImages__00_Z(void* /*class*/* p0, void* /*class*/* p1, void* /*class*/* p2) {}

// Symbol: ?SetOriginal@CMFCRibbonButtonsGroup@@MEAAXPEAVCMFCRibbonBaseElement@@@Z
extern "C" void MS_ABI impl__SetOriginal_CMFCRibbonButtonsGroup__MEAAXPEAVCMFCRibbonBaseElement___Z(void* /*class*/* p0) {}

// Symbol: ?SetParentCategory@CMFCRibbonButtonsGroup@@UEAAXPEAVCMFCRibbonCategory@@@Z
extern "C" void MS_ABI impl__SetParentCategory_CMFCRibbonButtonsGroup__UEAAXPEAVCMFCRibbonCategory___Z(void* /*class*/* p0) {}

// Symbol: ?SetParentMenu@CMFCRibbonButtonsGroup@@MEAAXPEAVCMFCRibbonPanelMenuBar@@@Z
extern "C" void MS_ABI impl__SetParentMenu_CMFCRibbonButtonsGroup__MEAAXPEAVCMFCRibbonPanelMenuBar___Z(void* /*class*/* p0) {}

// Symbol: ?SetParentRibbonBar@CMFCRibbonButtonsGroup@@MEAAXPEAVCMFCRibbonBar@@@Z
extern "C" void MS_ABI impl__SetParentRibbonBar_CMFCRibbonButtonsGroup__MEAAXPEAVCMFCRibbonBar___Z(void* /*class*/* p0) {}
