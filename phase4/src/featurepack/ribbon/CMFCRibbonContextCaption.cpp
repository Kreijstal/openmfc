// CMFCRibbonContextCaption — OpenMFC implementation.
// Sources: mfccore.cpp, ole_olectors_exports.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/MfccoreSupport.h"

#include "openmfc/afxole.h"
#include "openmfc/afxmfc.h"

#include <new>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

























CMFCRibbonContextCaption::CMFCRibbonContextCaption() { memset(_pad, 0, sizeof(_pad)); }
CMFCRibbonContextCaption::~CMFCRibbonContextCaption() {}
// Symbol: ??0CMFCRibbonContextCaption@@IEAA@PEB_WIW4AFX_RibbonCategoryColor@@@Z
// Retail constructor entry RVA 0xd7e30 (mfc140u) takes (this, name, context ID,
// color) and initializes CMFCRibbonButton-derived state absent from this placeholder
// layout. This thunk only placement-constructs the small OpenMFC shell; it ignores
// those arguments and is not a retail-equivalent caption initializer.
extern "C" void* MS_ABI impl___0CMFCRibbonContextCaption__IEAA_PEB_WIW4AFX_RibbonCategoryColor___Z(
    void* pThis, const wchar_t* pName, unsigned int nContextID, int color) {
    (void)pName;
    (void)nContextID;
    (void)color;
    // Construct only the modeled OpenMFC shell in the caller-provided storage.
    return ::new (pThis) CMFCRibbonContextCaption();
}
// TODO(clean-room): mfc140u RVA 0xd7df0 allocates 0x280 bytes and calls the
// default constructor at RVA 0xd7ef0. OpenMFC declares this as CObject rather than
// CMFCRibbonButton and has a different vtable/layout, so a small fake factory
// object would not be a safe ABI substitute.
// Symbol: ?CreateObject@CMFCRibbonContextCaption@@SAPEAVCObject@@XZ
extern "C" void* MS_ABI impl__CreateObject_CMFCRibbonContextCaption__SAPEAVCObject__XZ() {
    return nullptr;
}

// TODO(clean-room): mfc140u RVA 0xd8390 reads the parent at +0xd8 and scans a
// pointer array; the parent/bar and context-caption array layout is not modeled.
// Symbol: ?GetContextCaptionIndex@CMFCRibbonContextCaption@@QEAAHPEAV1@@Z
extern "C" int MS_ABI impl__GetContextCaptionIndex_CMFCRibbonContextCaption__QEAAHPEAV1__Z(
    void* pThis, void* pContextCaption)
{
    (void)pThis;
    (void)pContextCaption;
    return 0;
}

// TODO(clean-room): mfc140u RVA 0xd82f0 filters the parent's categories by
// context ID and visibility; neither the caption nor category layout is modeled.
// Symbol: ?GetContextCategories@CMFCRibbonContextCaption@@QEAAXAEAV?$CArray@PEAVCMFCRibbonCategory@@PEAV1@@@@Z
extern "C" void MS_ABI impl__GetContextCategories_CMFCRibbonContextCaption__QEAAXAEAV__CArray_PEAVCMFCRibbonCategory__PEAV1____Z(
    void* pThis, void* pCategories)
{
    (void)pThis;
    (void)pCategories;
}

// TODO(clean-room): mfc140u RVA 0xd8170 counts visible parent categories
// matching this caption's context ID; the necessary ribbon layout is absent.
// Symbol: ?GetContextCategoryCount@CMFCRibbonContextCaption@@QEAAHXZ
extern "C" int MS_ABI impl__GetContextCategoryCount_CMFCRibbonContextCaption__QEAAHXZ(
    void* pThis)
{
    (void)pThis;
    return 0;
}

// TODO(clean-room): mfc140u RVA 0xd7f30 uses inherited ribbon geometry,
// parent-bar state and drawing callbacks, none of which this class models.
// Symbol: ?OnDraw@CMFCRibbonContextCaption@@MEAAXPEAVCDC@@@Z
extern "C" void MS_ABI impl__OnDraw_CMFCRibbonContextCaption__MEAAXPEAVCDC___Z(
    void* pThis, void* pDC)
{
    (void)pThis;
    (void)pDC;
}

// The Unicode RVA map has no entry for this export. The ANSI twin's body at RVA
// 0xd8cf0 (mfc140.dll) reads the parent bar at this+0xd8, scans its categories,
// and may make an indirect call. OpenMFC lacks those ribbon layouts; CPoint
// by value is modeled as an 8-byte MS_ABI value.
// Symbol: ?OnLButtonUp@CMFCRibbonContextCaption@@MEAAXVCPoint@@@Z
extern "C" void MS_ABI impl__OnLButtonUp_CMFCRibbonContextCaption__MEAAXVCPoint___Z(
    void* pThis, long long point)
{
    (void)pThis;
    (void)point;
}

// The Unicode RVA map has no entry for this export. The ANSI twin at RVA 0xd8e20
// (mfc140.dll) reads the parent at this+0xd8 and finds a context category before
// dispatching through it. Those parent/category layouts are absent here.
// Symbol: ?OnSetAccData@CMFCRibbonContextCaption@@MEAAHJ@Z
extern "C" int MS_ABI impl__OnSetAccData_CMFCRibbonContextCaption__MEAAHJ_Z(
    void* pThis, long lVal)
{
    (void)pThis;
    (void)lVal;
    return 0;
}

// The Unicode RVA map has no entry for this export. The ANSI twin at RVA 0xd9060
// (mfc140.dll) calls a ribbon accessibility helper, sets the accessibility role
// at pData+0x30 to 0x2b, and returns 1. Its required ribbon base state is not
// present in this CObject placeholder, so do not call that helper on this object.
// Symbol: ?SetACCData@CMFCRibbonContextCaption@@MEAAHPEAVCWnd@@AEAVCAccessibilityData@@@Z
extern "C" int MS_ABI impl__SetACCData_CMFCRibbonContextCaption__MEAAHPEAVCWnd__AEAVCAccessibilityData___Z(
    void* pThis, void* pParentWnd, void* pData)
{
    (void)pThis;
    (void)pParentWnd;
    (void)pData;
    return 0;
}

// TODO(clean-room): mfc140u RVA 0xd84f0 validates the child VARIANT and resolves
// a context-category target; the context-category object graph is not modeled.
// Symbol: ?accDoDefaultAction@CMFCRibbonContextCaption@@MEAAJUtagVARIANT@@@Z
extern "C" long MS_ABI impl__accDoDefaultAction_CMFCRibbonContextCaption__MEAAJUtagVARIANT___Z(
    void* pThis, VARIANT varChild)
{
    (void)pThis;
    (void)varChild;
    return 0;
}

// TODO(clean-room): mfc140u RVA 0xd85f0 navigates among context categories
// and parent-bar elements; those layouts and accessibility interfaces are absent.
// Symbol: ?accNavigate@CMFCRibbonContextCaption@@MEAAJJUtagVARIANT@@PEAU2@@Z
extern "C" long MS_ABI impl__accNavigate_CMFCRibbonContextCaption__MEAAJJUtagVARIANT__PEAU2__Z(
    void* pThis, long navDir, VARIANT varStart, VARIANT* pvarEndUpAt)
{
    (void)pThis;
    (void)navDir;
    (void)varStart;
    (void)pvarEndUpAt;
    return 0;
}

// Retail RVA 0xd84c0 (mfc140u) rejects a null output with E_INVALIDARG and
// otherwise writes GetContextCategoryCount's result and returns S_OK. The
// count thunk remains a zero-result stub because the parent/category layout
// is not modeled; preserve the independently verifiable output-pointer behavior.
// Symbol: ?get_accChildCount@CMFCRibbonContextCaption@@MEAAJPEAJ@Z
extern "C" long MS_ABI impl__get_accChildCount_CMFCRibbonContextCaption__MEAAJPEAJ_Z(
    void* pThis, long* pCountChildren)
{
    if (pCountChildren == nullptr) return static_cast<long>(0x80070057u);
    *pCountChildren = impl__GetContextCategoryCount_CMFCRibbonContextCaption__QEAAHXZ(pThis);
    return 0;
}

// TODO(clean-room): --u did not resolve this export, so its retail behavior
// could not be established from the available disassembly.
// Symbol: ?get_accParent@CMFCRibbonContextCaption@@MEAAJPEAPEAUIDispatch@@@Z
extern "C" long MS_ABI impl__get_accParent_CMFCRibbonContextCaption__MEAAJPEAPEAUIDispatch___Z(
    void* pThis, void** ppdispParent)
{
    (void)pThis;
    (void)ppdispParent;
    return 0;
}

