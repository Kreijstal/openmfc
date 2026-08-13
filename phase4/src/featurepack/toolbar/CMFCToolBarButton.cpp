// CMFCToolBarButton — OpenMFC implementation.
// Sources: cbarcore.cpp, global_cmfctoolbarbutton_msvtbl.cpp, mfccore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/CbarcoreSupport.h"
#include "detail/CMFCToolBarButtonSupport.h"
#include "detail/MfccoreSupport.h"

// Symbol: ?SetClipboardFormatName@CMFCToolBarButton@@SAXPEB_W@Z
void CMFCToolBarButton::SetClipboardFormatName(const wchar_t* lpszName) {
    if (lpszName && *lpszName) {
        m_strClipboardFormatName = lpszName;
    } else {
        m_strClipboardFormatName.Empty();
    }
}
CString CMFCToolBarButton::m_strClipboardFormatName;
BOOL CMFCToolBarButton::m_bWrapText = TRUE;
BOOL CMFCToolBarButton::m_bUpdateImages = FALSE;
// ---- named exports: real bodies (flip the weak null stubs to real behavior) ----
// Symbol: ?Serialize@CMFCToolBarButton@@UEAAXAEAVCArchive@@@Z
extern "C" void MS_ABI impl__Serialize_CMFCToolBarButton__UEAAXAEAVCArchive___Z(void* pThis, CArchive* ar) {
    tbb_Serialize((CMFCToolBarButton*)pThis, ar);
}
// Symbol: ?CopyFrom@CMFCToolBarButton@@UEAAXAEBV1@@Z
extern "C" void MS_ABI impl__CopyFrom_CMFCToolBarButton__UEAAXAEBV1__Z(void* pThis, const CMFCToolBarButton* src) {
    tbb_CopyFrom((CMFCToolBarButton*)pThis, src);
}
// Symbol: ?CompareWith@CMFCToolBarButton@@UEBAHAEBV1@@Z
extern "C" int MS_ABI impl__CompareWith_CMFCToolBarButton__UEBAHAEBV1__Z(const void* pThis, const CMFCToolBarButton* other) {
    return tbb_CompareWith((const CMFCToolBarButton*)pThis, other);
}
// Symbol: ?SetImage@CMFCToolBarButton@@UEAAXH@Z
extern "C" void MS_ABI impl__SetImage_CMFCToolBarButton__UEAAXH_Z(void* pThis, int iImage) {
    tbb_SetImage((CMFCToolBarButton*)pThis, iImage);
}
// Symbol: ?ResetImageToDefault@CMFCToolBarButton@@UEAAXXZ
extern "C" void MS_ABI impl__ResetImageToDefault_CMFCToolBarButton__UEAAXXZ(void*) {
    /* base class: no-op (harvested from real mfc140u) */
}
CMFCToolBarButton::CMFCToolBarButton() {
    OpenMFC_InitToolBarButton(this);
}
CMFCToolBarButton::CMFCToolBarButton(UINT uiCmdID, int iImage, const wchar_t* lpszText, BOOL bUserButton, BOOL bLocked) {
    OpenMFC_InitToolBarButton(this);
    m_nID = uiCmdID;
    if (lpszText) m_strText = lpszText;
    m_bUserButton = bUserButton;
    m_bLocked = bLocked;
    if (bUserButton) m_iUserImage = iImage; else m_iImage = iImage;
}
CMFCToolBarButton::~CMFCToolBarButton() {}
// Symbol: ?m_lstProtectedCommands@CMFCToolBarButton@@1V?$CList@II@@A
extern "C" void* /*CRuntimeClass*/ MS_ABI impl__m_lstProtectedCommands_CMFCToolBarButton__1V__CList_II__A = {};

// Symbol: ?m_strClipboardFormatName@CMFCToolBarButton@@2V?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@A
extern "C" void* /*CRuntimeClass*/ MS_ABI impl__m_strClipboardFormatName_CMFCToolBarButton__2V__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__A = {};

// Symbol: ?CreateFromOleData@CMFCToolBarButton@@SAPEAV1@PEAVCOleDataObject@@@Z
extern "C" void* MS_ABI impl__CreateFromOleData_CMFCToolBarButton__SAPEAV1_PEAVCOleDataObject___Z() {
    return nullptr;
}

// Symbol: ?ExportToMenuButton@CMFCToolBarButton@@UEBAHAEAVCMFCToolBarMenuButton@@@Z
extern "C" int MS_ABI impl__ExportToMenuButton_CMFCToolBarButton__UEBAHAEAVCMFCToolBarMenuButton___Z(void* /*class*/* p0) {
    return 0;
}

// Symbol: ?FillInterior@CMFCToolBarButton@@IEAAXPEAVCDC@@AEBVCRect@@HH@Z
extern "C" void MS_ABI impl__FillInterior_CMFCToolBarButton__IEAAXPEAVCDC__AEBVCRect__HH_Z(void* /*class*/* p0, const void* /*class*/* p1, int p2, int p3) {}

// Symbol: ?GetAccCount@CMFCToolBarButton@@UEAAHXZ
extern "C" int MS_ABI impl__GetAccCount_CMFCToolBarButton__UEAAHXZ() {
    return 0;
}

// Symbol: ?GetClipboardFormat@CMFCToolBarButton@@SAGXZ
extern "C" unsigned short MS_ABI impl__GetClipboardFormat_CMFCToolBarButton__SAGXZ() {
    return 0;
}

// Symbol: ?Initialize@CMFCToolBarButton@@IEAAXXZ
extern "C" void MS_ABI impl__Initialize_CMFCToolBarButton__IEAAXXZ() {}

// Symbol: ?IsFirstInGroup@CMFCToolBarButton@@UEBAHXZ
extern "C" int MS_ABI impl__IsFirstInGroup_CMFCToolBarButton__UEBAHXZ() {
    return 0;
}

// Symbol: ?IsLastInGroup@CMFCToolBarButton@@UEBAHXZ
extern "C" int MS_ABI impl__IsLastInGroup_CMFCToolBarButton__UEBAHXZ() {
    return 0;
}

// Symbol: ?OnCalculateSize@CMFCToolBarButton@@UEAA?AUtagSIZE@@PEAVCDC@@AEBVCSize@@H@Z
extern "C" void* MS_ABI impl__OnCalculateSize_CMFCToolBarButton__UEAA_AUtagSIZE__PEAVCDC__AEBVCSize__H_Z(void* /*struct*/* p0, void* /*class*/* p1, const void* /*class*/* p2, int p3) {
    return nullptr;
}

// Symbol: ?OnChangeParentWnd@CMFCToolBarButton@@UEAAXPEAVCWnd@@@Z
extern "C" void MS_ABI impl__OnChangeParentWnd_CMFCToolBarButton__UEAAXPEAVCWnd___Z(void* /*class*/* p0) {}

// Symbol: ?OnDraw@CMFCToolBarButton@@UEAAXPEAVCDC@@AEBVCRect@@PEAVCMFCToolBarImages@@HHHHH@Z
extern "C" void MS_ABI impl__OnDraw_CMFCToolBarButton__UEAAXPEAVCDC__AEBVCRect__PEAVCMFCToolBarImages__HHHHH_Z(void* /*class*/* p0, const void* /*class*/* p1, void* /*class*/* p2, int p3, int p4, int p5, int p6, int p7) {}

// Symbol: ?OnDrawOnCustomizeList@CMFCToolBarButton@@UEAAHPEAVCDC@@AEBVCRect@@H@Z
extern "C" int MS_ABI impl__OnDrawOnCustomizeList_CMFCToolBarButton__UEAAHPEAVCDC__AEBVCRect__H_Z(void* /*class*/* p0, const void* /*class*/* p1, int p2) {
    return 0;
}

// Symbol: ?OnToolHitTest@CMFCToolBarButton@@UEAAHPEBVCWnd@@PEAUtagTOOLINFOW@@@Z
extern "C" int MS_ABI impl__OnToolHitTest_CMFCToolBarButton__UEAAHPEBVCWnd__PEAUtagTOOLINFOW___Z(const void* /*class*/* p0, void* /*struct*/* p1) {
    return 0;
}

// Symbol: ?PrepareDrag@CMFCToolBarButton@@UEAAHAEAVCOleDataSource@@@Z
extern "C" int MS_ABI impl__PrepareDrag_CMFCToolBarButton__UEAAHAEAVCOleDataSource___Z(void* /*class*/* p0) {
    return 0;
}

// Symbol: ?SetACCData@CMFCToolBarButton@@UEAAHPEAVCWnd@@AEAVCAccessibilityData@@@Z
extern "C" int MS_ABI impl__SetACCData_CMFCToolBarButton__UEAAHPEAVCWnd__AEAVCAccessibilityData___Z(void* /*class*/* p0, void* /*class*/* p1) {
    return 0;
}

// Symbol: ?SetProtectedCommands@CMFCToolBarButton@@SAXAEBV?$CList@II@@@Z
extern "C" void MS_ABI impl__SetProtectedCommands_CMFCToolBarButton__SAXAEBV__CList_II___Z(const void* /*class*/* p0) {}
