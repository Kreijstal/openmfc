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
// Pre-existing approximation retained with warning: retail RVA 0xea000 (mfc140u)
// initializes the supplied this object and passes the element argument to AddButton;
// this body ignores that argument and instead allocates a separate compact OpenMFC
// object, leaving the caller's this uninitialized. Correct placement construction
// needs a compatible group allocation/layout, which this header lacks.
// Symbol: ??0CMFCRibbonButtonsGroup@@QEAA@PEAVCMFCRibbonBaseElement@@@Z
// CMFCRibbonButtonsGroup::CMFCRibbonButtonsGroup()
extern "C" void* MS_ABI impl___0CMFCRibbonButtonsGroup__QEAA_PEAVCMFCRibbonBaseElement___Z(
    void* /*p0*/) {
    return new CMFCRibbonButtonsGroup();
}
// Symbol: ?AddButton@CMFCRibbonButtonsGroup@@QEAAXPEAVCMFCRibbonBaseElement@@@Z
// Retail RVA 0xea100 (mfc140u) was checked; the required group storage is not modeled.
extern "C" void MS_ABI impl__AddButton_CMFCRibbonButtonsGroup__QEAAXPEAVCMFCRibbonBaseElement___Z(void* /*pThis*/, void* /*pButton*/) {}

// Symbol: ?AddButtons@CMFCRibbonButtonsGroup@@QEAAXAEBV?$CList@PEAVCMFCRibbonBaseElement@@PEAV1@@@@Z
// Retail RVA 0xea180 (mfc140u) was checked; the required group storage is not modeled.
extern "C" void MS_ABI impl__AddButtons_CMFCRibbonButtonsGroup__QEAAXAEBV__CList_PEAVCMFCRibbonBaseElement__PEAV1____Z(void* /*pThis*/, const void* /*pButtons*/) {}

// Symbol: ?AddToKeyList@CMFCRibbonButtonsGroup@@MEAAXAEAV?$CArray@PEAVCMFCRibbonKeyTip@@PEAV1@@@@Z
// Retail RVA 0xeb6e0 (mfc140u) was checked; the required group storage is not modeled.
extern "C" void MS_ABI impl__AddToKeyList_CMFCRibbonButtonsGroup__MEAAXAEAV__CArray_PEAVCMFCRibbonKeyTip__PEAV1____Z(void* /*pThis*/, void* /*pKeyTips*/) {}

// Symbol: ?AddToListBox@CMFCRibbonButtonsGroup@@MEAAHPEAVCMFCRibbonCommandsListBox@@H@Z
// Retail RVA 0xeb4f0 (mfc140u) was checked; the required group storage is not modeled.
extern "C" int MS_ABI impl__AddToListBox_CMFCRibbonButtonsGroup__MEAAHPEAVCMFCRibbonCommandsListBox__H_Z(void* /*pThis*/, void* /*pListBox*/, int /*bDeep*/) {
    return 0;
}

// Symbol: ?CleanUpSizes@CMFCRibbonButtonsGroup@@MEAAXXZ
// Retail RVA 0xeb570 (mfc140u) was checked; the required group storage is not modeled.
extern "C" void MS_ABI impl__CleanUpSizes_CMFCRibbonButtonsGroup__MEAAXXZ(void* /*pThis*/) {}

// Symbol: ?CopyFrom@CMFCRibbonButtonsGroup@@MEAAXAEBVCMFCRibbonBaseElement@@@Z
// Retail RVA 0xeb1a0 (mfc140u) was checked; the required group storage is not modeled.
extern "C" void MS_ABI impl__CopyFrom_CMFCRibbonButtonsGroup__MEAAXAEBVCMFCRibbonBaseElement___Z(void* /*pThis*/, const void* /*pSource*/) {}

// Symbol: ?Find@CMFCRibbonButtonsGroup@@MEAAPEAVCMFCRibbonBaseElement@@PEBV2@@Z
// Retail RVA 0xeaa40 (mfc140u) was checked; the required group storage is not modeled.
extern "C" void* MS_ABI impl__Find_CMFCRibbonButtonsGroup__MEAAPEAVCMFCRibbonBaseElement__PEBV2__Z(void* /*pThis*/, const void* /*pElement*/) {
    return nullptr;
}

// Symbol: ?FindByData@CMFCRibbonButtonsGroup@@MEAAPEAVCMFCRibbonBaseElement@@_K@Z
// Retail RVA 0xeab40 (mfc140u) was checked; the required group storage is not modeled.
extern "C" void* MS_ABI impl__FindByData_CMFCRibbonButtonsGroup__MEAAPEAVCMFCRibbonBaseElement___K_Z(void* /*pThis*/, unsigned long long /*data*/) {
    return nullptr;
}

// Symbol: ?FindByID@CMFCRibbonButtonsGroup@@MEAAPEAVCMFCRibbonBaseElement@@I@Z
// Retail RVA 0xeaac0 (mfc140u) was checked; the required group storage is not modeled.
extern "C" void* MS_ABI impl__FindByID_CMFCRibbonButtonsGroup__MEAAPEAVCMFCRibbonBaseElement__I_Z(void* /*pThis*/, unsigned int /*id*/) {
    return nullptr;
}

// Symbol: ?FindByOriginal@CMFCRibbonButtonsGroup@@MEAAPEAVCMFCRibbonBaseElement@@PEAV2@@Z
// Retail RVA 0xeabc0 (mfc140u) was checked; the required group storage is not modeled.
extern "C" void* MS_ABI impl__FindByOriginal_CMFCRibbonButtonsGroup__MEAAPEAVCMFCRibbonBaseElement__PEAV2__Z(void* /*pThis*/, void* /*pOriginal*/) {
    return nullptr;
}

// Symbol: ?GetButtonIndex@CMFCRibbonButtonsGroup@@QEBAHPEBVCMFCRibbonBaseElement@@@Z
// Retail RVA 0xeb9c0 (mfc140u) was checked; the required group storage is not modeled.
extern "C" int MS_ABI impl__GetButtonIndex_CMFCRibbonButtonsGroup__QEBAHPEBVCMFCRibbonBaseElement___Z(const void* /*pThis*/, const void* /*pButton*/) {
    return 0;
}

// Symbol: ?GetDroppedDown@CMFCRibbonButtonsGroup@@MEAAPEAVCMFCRibbonBaseElement@@XZ
// Retail RVA 0xeacb0 (mfc140u) was checked; the required group storage is not modeled.
extern "C" void* MS_ABI impl__GetDroppedDown_CMFCRibbonButtonsGroup__MEAAPEAVCMFCRibbonBaseElement__XZ(void* /*pThis*/) {
    return nullptr;
}

// Symbol: ?GetElementsByID@CMFCRibbonButtonsGroup@@MEAAXIAEAV?$CArray@PEAVCMFCRibbonBaseElement@@PEAV1@@@@Z
// Retail RVA 0xeb480 (mfc140u) was checked; the required group storage is not modeled.
extern "C" void MS_ABI impl__GetElementsByID_CMFCRibbonButtonsGroup__MEAAXIAEAV__CArray_PEAVCMFCRibbonBaseElement__PEAV1____Z(void* /*pThis*/, unsigned int /*id*/, void* /*pElements*/) {}

// Symbol: ?GetFirstTabStop@CMFCRibbonButtonsGroup@@MEAAPEAVCMFCRibbonBaseElement@@XZ
// Retail RVA 0xeb8f0 (mfc140u) was checked; the required group storage is not modeled.
extern "C" void* MS_ABI impl__GetFirstTabStop_CMFCRibbonButtonsGroup__MEAAPEAVCMFCRibbonBaseElement__XZ(void* /*pThis*/) {
    return nullptr;
}

// Symbol: ?GetFocused@CMFCRibbonButtonsGroup@@MEAAPEAVCMFCRibbonBaseElement@@XZ
// Retail RVA 0xeb800 (mfc140u) was checked; the required group storage is not modeled.
extern "C" void* MS_ABI impl__GetFocused_CMFCRibbonButtonsGroup__MEAAPEAVCMFCRibbonBaseElement__XZ(void* /*pThis*/) {
    return nullptr;
}

// Symbol: ?GetHighlighted@CMFCRibbonButtonsGroup@@MEAAPEAVCMFCRibbonBaseElement@@XZ
// Retail RVA 0xead20 (mfc140u) was checked; the required group storage is not modeled.
extern "C" void* MS_ABI impl__GetHighlighted_CMFCRibbonButtonsGroup__MEAAPEAVCMFCRibbonBaseElement__XZ(void* /*pThis*/) {
    return nullptr;
}

// Symbol: ?GetImageSize@CMFCRibbonButtonsGroup@@QEBA?BVCSize@@XZ
// Retail RVA 0xeb7e0 (mfc140u) writes its CSize through the hidden result pointer
// and returns that pointer. Preserve the ABI with a zero-size stub; the required
// image fields at +0x1b0/+0x210 are not modeled here.
extern "C" long long* MS_ABI impl__GetImageSize_CMFCRibbonButtonsGroup__QEBA_BVCSize__XZ(
    const void* /*pThis*/, long long* pResult) {
    *pResult = 0;
    return pResult;
}

// Symbol: ?GetItemIDsList@CMFCRibbonButtonsGroup@@MEBAXAEAV?$CList@II@@@Z
// Retail RVA 0xeb400 (mfc140u) was checked; the required group storage is not modeled.
extern "C" void MS_ABI impl__GetItemIDsList_CMFCRibbonButtonsGroup__MEBAXAEAV__CList_II___Z(const void* /*pThis*/, void* /*pItemIDs*/) {}

// Symbol: ?GetLastTabStop@CMFCRibbonButtonsGroup@@MEAAPEAVCMFCRibbonBaseElement@@XZ
// Retail RVA 0xeb960 (mfc140u) was checked; the required group storage is not modeled.
extern "C" void* MS_ABI impl__GetLastTabStop_CMFCRibbonButtonsGroup__MEAAPEAVCMFCRibbonBaseElement__XZ(void* /*pThis*/) {
    return nullptr;
}

// Symbol: ?GetPressed@CMFCRibbonButtonsGroup@@MEAAPEAVCMFCRibbonBaseElement@@XZ
// Retail RVA 0xeac40 (mfc140u) was checked; the required group storage is not modeled.
extern "C" void* MS_ABI impl__GetPressed_CMFCRibbonButtonsGroup__MEAAPEAVCMFCRibbonBaseElement__XZ(void* /*pThis*/) {
    return nullptr;
}

// Symbol: ?GetRegularSize@CMFCRibbonButtonsGroup@@UEAA?AVCSize@@PEAVCDC@@@Z
// Retail RVA 0xea3f0 (mfc140u) receives a hidden CSize result pointer in RDX,
// the CDC pointer in R8, and returns the result pointer. Preserve that ABI with
// a zero-size stub; the required button and image storage is not modeled here.
extern "C" long long* MS_ABI impl__GetRegularSize_CMFCRibbonButtonsGroup__UEAA_AVCSize__PEAVCDC___Z(
    void* /*pThis*/, long long* pResult, void* /*pDC*/) {
    *pResult = 0;
    return pResult;
}

// Symbol: ?GetVisibleElements@CMFCRibbonButtonsGroup@@MEAAXAEAV?$CArray@PEAVCMFCRibbonBaseElement@@PEAV1@@@@Z
// Retail RVA 0xeb870 (mfc140u) was checked; the required group storage is not modeled.
extern "C" void MS_ABI impl__GetVisibleElements_CMFCRibbonButtonsGroup__MEAAXAEAV__CArray_PEAVCMFCRibbonBaseElement__PEAV1____Z(void* /*pThis*/, void* /*pElements*/) {}

// Symbol: ?HitTest@CMFCRibbonButtonsGroup@@MEAAPEAVCMFCRibbonBaseElement@@VCPoint@@@Z
// Retail RVA 0xea9d0 (mfc140u) was checked; the required group storage is not modeled.
extern "C" void* MS_ABI impl__HitTest_CMFCRibbonButtonsGroup__MEAAPEAVCMFCRibbonBaseElement__VCPoint___Z(void* /*pThis*/, long long /*point*/) {
    return nullptr;
}

// Symbol: ?OnAfterChangeRect@CMFCRibbonButtonsGroup@@MEAAXPEAVCDC@@@Z
// Retail RVA 0xea580 (mfc140u) was checked; the required group storage is not modeled.
extern "C" void MS_ABI impl__OnAfterChangeRect_CMFCRibbonButtonsGroup__MEAAXPEAVCDC___Z(void* /*pThis*/, void* /*pDC*/) {}

// Symbol: ?OnDraw@CMFCRibbonButtonsGroup@@MEAAXPEAVCDC@@@Z
// Retail RVA 0xea240 (mfc140u) was checked; the required group storage is not modeled.
extern "C" void MS_ABI impl__OnDraw_CMFCRibbonButtonsGroup__MEAAXPEAVCDC___Z(void* /*pThis*/, void* /*pDC*/) {}

// Symbol: ?OnDrawImage@CMFCRibbonButtonsGroup@@UEAAXPEAVCDC@@VCRect@@PEAVCMFCRibbonBaseElement@@H@Z
// Retail RVA 0xeb000 (mfc140u) was checked; the image storage is not modeled.
extern "C" void MS_ABI impl__OnDrawImage_CMFCRibbonButtonsGroup__UEAAXPEAVCDC__VCRect__PEAVCMFCRibbonBaseElement__H_Z(void* /*pThis*/, void* /*pDC*/, const void* /*rect*/, void* /*pButton*/, int /*imageIndex*/) {}

// Symbol: ?OnRTLChanged@CMFCRibbonButtonsGroup@@MEAAXH@Z
// Unicode map has no RVA for this export; its ANSI twin is at RVA 0xec360 (mfc140).
// The group storage needed by that body is not modeled.
extern "C" void MS_ABI impl__OnRTLChanged_CMFCRibbonButtonsGroup__MEAAXH_Z(void* /*pThis*/, int /*isRTL*/) {}

// Symbol: ?OnShow@CMFCRibbonButtonsGroup@@MEAAXH@Z
// Unicode map has no RVA for this export; its ANSI twin is at RVA 0xeb550 (mfc140).
// The group storage needed by that body is not modeled.
extern "C" void MS_ABI impl__OnShow_CMFCRibbonButtonsGroup__MEAAXH_Z(void* /*pThis*/, int /*show*/) {}

// Symbol: ?OnUpdateCmdUI@CMFCRibbonButtonsGroup@@MEAAXPEAVCMFCRibbonCmdUI@@PEAVCFrameWnd@@H@Z
// Unicode map has no RVA for this export; its ANSI twin is at RVA 0xeb100 (mfc140).
// The group storage needed by that body is not modeled.
extern "C" void MS_ABI impl__OnUpdateCmdUI_CMFCRibbonButtonsGroup__MEAAXPEAVCMFCRibbonCmdUI__PEAVCFrameWnd__H_Z(void* /*pThis*/, void* /*pCmdUI*/, void* /*pTarget*/, int /*disableIfNoHandler*/) {}

// Symbol: ?RemoveAll@CMFCRibbonButtonsGroup@@QEAAXXZ
// Unicode map has no RVA for this export; its ANSI twin is at RVA 0xeadc0 (mfc140).
// The group storage needed by that body is not modeled.
extern "C" void MS_ABI impl__RemoveAll_CMFCRibbonButtonsGroup__QEAAXXZ(void* /*pThis*/) {}

// Symbol: ?ReplaceByID@CMFCRibbonButtonsGroup@@MEAAHIPEAVCMFCRibbonBaseElement@@@Z
// Unicode map has no RVA for this export; its ANSI twin is at RVA 0xeb990 (mfc140).
// The group storage needed by that body is not modeled.
extern "C" int MS_ABI impl__ReplaceByID_CMFCRibbonButtonsGroup__MEAAHIPEAVCMFCRibbonBaseElement___Z(void* /*pThis*/, unsigned int /*id*/, void* /*pElement*/) {
    return 0;
}

// Symbol: ?SetImages@CMFCRibbonButtonsGroup@@QEAAXPEAVCMFCToolBarImages@@00@Z
// Unicode map has no RVA for this export; its ANSI twin is at RVA 0xeba70 (mfc140).
// The image storage needed by that body is not modeled.
extern "C" void MS_ABI impl__SetImages_CMFCRibbonButtonsGroup__QEAAXPEAVCMFCToolBarImages__00_Z(void* /*pThis*/, void* /*pImages*/, void* /*pHotImages*/, void* /*pDisabledImages*/) {}

// Symbol: ?SetOriginal@CMFCRibbonButtonsGroup@@MEAAXPEAVCMFCRibbonBaseElement@@@Z
// Unicode map has no RVA for this export; its ANSI twin is at RVA 0xebf30 (mfc140).
// The group storage needed by that body is not modeled.
extern "C" void MS_ABI impl__SetOriginal_CMFCRibbonButtonsGroup__MEAAXPEAVCMFCRibbonBaseElement___Z(void* /*pThis*/, void* /*pOriginal*/) {}

// Symbol: ?SetParentCategory@CMFCRibbonButtonsGroup@@UEAAXPEAVCMFCRibbonCategory@@@Z
// Unicode map has no RVA for this export; its ANSI twin is at RVA 0xec260 (mfc140).
// The group storage needed by that body is not modeled.
extern "C" void MS_ABI impl__SetParentCategory_CMFCRibbonButtonsGroup__UEAAXPEAVCMFCRibbonCategory___Z(void* /*pThis*/, void* /*pCategory*/) {}

// Symbol: ?SetParentMenu@CMFCRibbonButtonsGroup@@MEAAXPEAVCMFCRibbonPanelMenuBar@@@Z
// Unicode map has no RVA for this export; its ANSI twin is at RVA 0xebeb0 (mfc140).
// The group storage needed by that body is not modeled.
extern "C" void MS_ABI impl__SetParentMenu_CMFCRibbonButtonsGroup__MEAAXPEAVCMFCRibbonPanelMenuBar___Z(void* /*pThis*/, void* /*pMenu*/) {}

// Symbol: ?SetParentRibbonBar@CMFCRibbonButtonsGroup@@MEAAXPEAVCMFCRibbonBar@@@Z
// Unicode map has no RVA for this export; its ANSI twin is at RVA 0xec1e0 (mfc140).
// The group storage needed by that body is not modeled.
extern "C" void MS_ABI impl__SetParentRibbonBar_CMFCRibbonButtonsGroup__MEAAXPEAVCMFCRibbonBar___Z(void* /*pThis*/, void* /*pRibbonBar*/) {}
