// CMFCToolBarButton — OpenMFC implementation.
// Sources: cbarcore.cpp, global_cmfctoolbarbutton_msvtbl.cpp, mfccore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/CbarcoreSupport.h"
#include "detail/CMFCToolBarButtonSupport.h"
#include "detail/MfccoreSupport.h"

static_assert(sizeof(CMFCToolBarButton) == 0x88, "retail CMFCToolBarButton size");
static_assert(offsetof(CMFCToolBarButton, m_bUserButton) == 0x08, "m_bUserButton @0x08");
static_assert(offsetof(CMFCToolBarButton, m_bImage) == 0x10, "m_bImage @0x10");
static_assert(offsetof(CMFCToolBarButton, m_bWrap) == 0x14, "m_bWrap @0x14");
static_assert(offsetof(CMFCToolBarButton, m_bWholeText) == 0x18, "m_bWholeText @0x18");
static_assert(offsetof(CMFCToolBarButton, m_bTextBelow) == 0x1c, "m_bTextBelow @0x1c");
static_assert(offsetof(CMFCToolBarButton, m_nID) == 0x24, "m_nID @0x24");
static_assert(offsetof(CMFCToolBarButton, m_nStyle) == 0x28, "m_nStyle @0x28");
static_assert(offsetof(CMFCToolBarButton, m_dwdItemData) == 0x30, "m_dwdItemData @0x30");
static_assert(offsetof(CMFCToolBarButton, m_iImage) == 0x48, "m_iImage @0x48");
static_assert(offsetof(CMFCToolBarButton, m_iUserImage) == 0x4c, "m_iUserImage @0x4c");
static_assert(offsetof(CMFCToolBarButton, m_bLocked) == 0x50, "m_bLocked @0x50");
static_assert(offsetof(CMFCToolBarButton, m_bDisableFill) == 0x58, "m_bDisableFill @0x58");
static_assert(offsetof(CMFCToolBarButton, m_bExtraSize) == 0x5c, "m_bExtraSize @0x5c");
static_assert(offsetof(CMFCToolBarButton, m_bHorz) == 0x60, "m_bHorz @0x60");
static_assert(offsetof(CMFCToolBarButton, m_bVisible) == 0x64, "m_bVisible @0x64");
static_assert(offsetof(CMFCToolBarButton, m_rect) == 0x68, "m_rect @0x68");
static_assert(offsetof(CMFCToolBarButton, m_sizeText) == 0x78, "m_sizeText @0x78");
static_assert(offsetof(CMFCToolBarButton, m_pWndParent) == 0x80, "m_pWndParent @0x80");

// Definitions are in core/runtime/CObjectTypeChecks.cpp and
// featurepack/toolbar/RuntimeClasses.cpp, respectively.
extern "C" BOOL MS_ABI impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(
    const CObject* pThis, const CRuntimeClass* pClass);
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMFCToolBar__SAPEAUCRuntimeClass__XZ();

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
// Stub: mfc140u data RVA 0x3b20c8 needs a 0x38-byte retail CList object; the local CList storage is not ABI-compatible.
// Symbol: ?m_lstProtectedCommands@CMFCToolBarButton@@1V?$CList@II@@A
extern "C" void* MS_ABI impl__m_lstProtectedCommands_CMFCToolBarButton__1V__CList_II__A = {};

// Stub: mfc140u data RVA 0x3c32a0 is not aliased to the separately defined class CString storage.
// Symbol: ?m_strClipboardFormatName@CMFCToolBarButton@@2V?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@A
extern "C" void* MS_ABI impl__m_strClipboardFormatName_CMFCToolBarButton__2V__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__A = {};

// Stub: retail mfc140u RVA 0x15c790 performs OLE clipboard retrieval and format decoding, which is not modeled here.
// Symbol: ?CreateFromOleData@CMFCToolBarButton@@SAPEAV1@PEAVCOleDataObject@@@Z
extern "C" CMFCToolBarButton* MS_ABI impl__CreateFromOleData_CMFCToolBarButton__SAPEAV1_PEAVCOleDataObject___Z(
    COleDataObject* pDataObject) {
    (void)pDataObject;
    return nullptr;
}

// Stub: retail mfc140u RVA 0x15e140 can edit the destination text after command/string lookup; that path is not transcribed.
// Symbol: ?ExportToMenuButton@CMFCToolBarButton@@UEBAHAEAVCMFCToolBarMenuButton@@@Z
extern "C" BOOL MS_ABI impl__ExportToMenuButton_CMFCToolBarButton__UEBAHAEAVCMFCToolBarMenuButton___Z(
    const void* pThis, CMFCToolBarMenuButton* pMenuButton) {
    (void)pThis;
    (void)pMenuButton;
    return FALSE;
}

// Stub: retail mfc140u RVA 0x15e300 dispatches visual-manager fill behavior using global UI state, not modeled here.
// Symbol: ?FillInterior@CMFCToolBarButton@@IEAAXPEAVCDC@@AEBVCRect@@HH@Z
extern "C" void MS_ABI impl__FillInterior_CMFCToolBarButton__IEAAXPEAVCDC__AEBVCRect__HH_Z(
    void* pThis, CDC* pDC, const CRect* pRect, int bHighlight, int bMenuImage) {
    (void)pThis;
    (void)pDC;
    (void)pRect;
    (void)bHighlight;
    (void)bMenuImage;
}

// Retail mfc140u RVA 0x15eab0 returns zero unless the button is visible and
// has a nonempty rect, then returns the inverse of style bit 0.
// Symbol: ?GetAccCount@CMFCToolBarButton@@UEAAHXZ
extern "C" int MS_ABI impl__GetAccCount_CMFCToolBarButton__UEAAHXZ(void* pThis) {
    const auto* pButton = static_cast<const CMFCToolBarButton*>(pThis);
    if (!pButton->m_bVisible || ::IsRectEmpty(static_cast<const RECT*>(pButton->m_rect))) return 0;
    return (pButton->m_nStyle & 1u) == 0 ? 1 : 0;
}

// Stub: retail mfc140u RVA 0x15c6c0 registers and caches the clipboard format through the static name and MFC global data.
// Symbol: ?GetClipboardFormat@CMFCToolBarButton@@SAGXZ
extern "C" unsigned short MS_ABI impl__GetClipboardFormat_CMFCToolBarButton__SAGXZ() {
    return 0;
}

// Retail mfc140u RVA 0x15c0c0 writes only these members (plus SetRectEmpty);
// notably it leaves m_bText and m_bIsHidden untouched.
// Symbol: ?Initialize@CMFCToolBarButton@@IEAAXXZ
extern "C" void MS_ABI impl__Initialize_CMFCToolBarButton__IEAAXXZ(void* pThis) {
    auto* pButton = static_cast<CMFCToolBarButton*>(pThis);
    pButton->m_bImage = TRUE;
    pButton->m_nID = 0;
    pButton->m_iImage = -1;
    pButton->m_iUserImage = -1;
    pButton->m_bUserButton = FALSE;
    pButton->m_bTextBelow = FALSE;
    pButton->m_bLocked = FALSE;
    pButton->m_dwdItemData = 0;
    pButton->m_bWholeText = TRUE;
    ::SetRectEmpty(static_cast<RECT*>(pButton->m_rect));
    pButton->m_sizeText.cx = 0;
    pButton->m_sizeText.cy = 0;
    pButton->m_bDisableFill = FALSE;
    pButton->m_bHorz = TRUE;
    pButton->m_bVisible = TRUE;
    pButton->m_pWndParent = nullptr;
}

// Stub: retail mfc140u RVA 0x15e5d0 walks the owning toolbar's buttons and applies hidden/group-boundary rules, not reproduced here.
// Symbol: ?IsFirstInGroup@CMFCToolBarButton@@UEBAHXZ
extern "C" BOOL MS_ABI impl__IsFirstInGroup_CMFCToolBarButton__UEBAHXZ(const void* pThis) {
    (void)pThis;
    return FALSE;
}

// Stub: retail mfc140u RVA 0x15e680 walks the owning toolbar's trailing buttons and applies hidden/group-boundary rules, not reproduced here.
// Symbol: ?IsLastInGroup@CMFCToolBarButton@@UEBAHXZ
extern "C" BOOL MS_ABI impl__IsLastInGroup_CMFCToolBarButton__UEBAHXZ(const void* pThis) {
    (void)pThis;
    return FALSE;
}

// Stub: retail mfc140u RVA 0x15d680 calculates text/image dimensions through CDC and font/toolbar state; not transcribed.
// Its tagSIZE result uses the x64 member-function hidden result buffer in RDX.
// Symbol: ?OnCalculateSize@CMFCToolBarButton@@UEAA?AUtagSIZE@@PEAVCDC@@AEBVCSize@@H@Z
extern "C" SIZE* MS_ABI impl__OnCalculateSize_CMFCToolBarButton__UEAA_AUtagSIZE__PEAVCDC__AEBVCSize__H_Z(
    void* pThis, SIZE* pRet, CDC* pDC, const SIZE* pSizeDefault, int bHorz) {
    (void)pThis;
    (void)pDC;
    (void)pSizeDefault;
    (void)bHorz;
    if (pRet != nullptr) {
        pRet->cx = 0;
        pRet->cy = 0;
    }
    return pRet;
}

// Retail mfc140u RVA 0x15e570 stores the parent and clears m_bExtraSize, then tests
// CMFCToolBar RTTI and calls virtual slot +0x6c8. No MSVC-layout CMFCToolBar
// vtable is defined in this tree, so this uses afxtoolbar.h:174's base TRUE result
// and deliberately does not dispatch derived overrides.
// Symbol: ?OnChangeParentWnd@CMFCToolBarButton@@UEAAXPEAVCWnd@@@Z
extern "C" void MS_ABI impl__OnChangeParentWnd_CMFCToolBarButton__UEAAXPEAVCWnd___Z(
    void* pThis, CWnd* pWndParent) {
    auto* pButton = static_cast<CMFCToolBarButton*>(pThis);
    pButton->m_bExtraSize = FALSE;
    pButton->m_pWndParent = pWndParent;
    if (pWndParent != nullptr &&
        impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(
            pWndParent, impl__GetThisClass_CMFCToolBar__SAPEAUCRuntimeClass__XZ())) {
        pButton->m_bExtraSize = TRUE;
    }
}

// Stub: retail mfc140u RVA 0x15c910 performs visual-manager and image-list drawing; the rendering path is not modeled.
// Symbol: ?OnDraw@CMFCToolBarButton@@UEAAXPEAVCDC@@AEBVCRect@@PEAVCMFCToolBarImages@@HHHHH@Z
extern "C" void MS_ABI impl__OnDraw_CMFCToolBarButton__UEAAXPEAVCDC__AEBVCRect__PEAVCMFCToolBarImages__HHHHH_Z(
    void* pThis, CDC* pDC, const RECT* pRect, CMFCToolBarImages* pImages,
    int bHorz, int bCustomizeMode, int bHighlight, int bDrawBorder, int bGrayDisabledButtons) {
    (void)pThis;
    (void)pDC;
    (void)pRect;
    (void)pImages;
    (void)bHorz;
    (void)bCustomizeMode;
    (void)bHighlight;
    (void)bDrawBorder;
    (void)bGrayDisabledButtons;
}

// Stub: retail mfc140u RVA 0x15db10 draws the customization item with theme, font, image, and text paths; not transcribed.
// Symbol: ?OnDrawOnCustomizeList@CMFCToolBarButton@@UEAAHPEAVCDC@@AEBVCRect@@H@Z
extern "C" int MS_ABI impl__OnDrawOnCustomizeList_CMFCToolBarButton__UEAAHPEAVCDC__AEBVCRect__H_Z(
    void* pThis, CDC* pDC, const RECT* pRect, int bSelected) {
    (void)pThis;
    (void)pDC;
    (void)pRect;
    (void)bSelected;
    return 0;
}

// Stub: the mfc140u toolbar-button vtable points to RVA 0x15e070 (mfc140u); its parent/tool-tip virtual dispatch is not reproduced.
// Symbol: ?OnToolHitTest@CMFCToolBarButton@@UEAAHPEBVCWnd@@PEAUtagTOOLINFOW@@@Z
extern "C" int MS_ABI impl__OnToolHitTest_CMFCToolBarButton__UEAAHPEBVCWnd__PEAUtagTOOLINFOW___Z(
    void* pThis, const CWnd* pWnd, TOOLINFOW* pToolInfo) {
    (void)pThis;
    (void)pWnd;
    (void)pToolInfo;
    return 0;
}

// Stub: the mfc140u toolbar-button vtable points to RVA 0x15d900 (mfc140u); its OLE data-object publication path is not reproduced.
// Symbol: ?PrepareDrag@CMFCToolBarButton@@UEAAHAEAVCOleDataSource@@@Z
extern "C" int MS_ABI impl__PrepareDrag_CMFCToolBarButton__UEAAHAEAVCOleDataSource___Z(
    void* pThis, COleDataSource* pSource) {
    (void)pThis;
    (void)pSource;
    return 0;
}

// Stub: retail mfc140u RVA 0x15e730 builds accessibility data from the parent, command, strings, and image; that path is not reproduced.
// Symbol: ?SetACCData@CMFCToolBarButton@@UEAAHPEAVCWnd@@AEAVCAccessibilityData@@@Z
extern "C" int MS_ABI impl__SetACCData_CMFCToolBarButton__UEAAHPEAVCWnd__AEAVCAccessibilityData___Z(
    void* pThis, CWnd* pParent, CAccessibilityData* pData) {
    (void)pThis;
    (void)pParent;
    (void)pData;
    return 0;
}

// Stub: no mfc140u RVA resolves, and the retail 0x38-byte CList object is not modeled as usable CList<UINT, UINT> storage.
// Symbol: ?SetProtectedCommands@CMFCToolBarButton@@SAXAEBV?$CList@II@@@Z
extern "C" void MS_ABI impl__SetProtectedCommands_CMFCToolBarButton__SAXAEBV__CList_II___Z(
    const CList<UINT, UINT>* pCommands) {
    (void)pCommands;
}
