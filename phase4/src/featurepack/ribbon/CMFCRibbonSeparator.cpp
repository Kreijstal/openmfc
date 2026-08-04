// CMFCRibbonSeparator — OpenMFC implementation.
// Sources: global_mfc-feature-23_impl.cpp, mfccore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/MfcFeature23ImplSupport.h"
#include "detail/MfccoreSupport.h"

// CMFCRibbonSeparator::AddToListBox(CMFCRibbonCommandsListBox_MfcFeature23Impl*, int) --
// retail loads a "separator" description string, inserts it into the list box
// (through the box's +0x40 toolbar reference) and returns TRUE; the box class
// is not declared and its state is not modeled.
// TODO(clean-room): transcribed partially -- commands list-box state is not
// modeled.
// Symbol: ?AddToListBox@CMFCRibbonSeparator@@UEAAHPEAVCMFCRibbonCommandsListBox@@H@Z
extern "C" int MS_ABI impl__AddToListBox_CMFCRibbonSeparator__UEAAHPEAVCMFCRibbonCommandsListBox__H_Z(
    CMFCRibbonSeparator* pThis, CMFCRibbonCommandsListBox_MfcFeature23Impl* /*pListBox*/, int /*nIndex*/)
{
    if (!pThis) return FALSE;
    return FALSE;
}
// CMFCRibbonSeparator::CopyFrom(const CMFCRibbonBaseElement&) -- retail
// delegates to CMFCRibbonBaseElement::CopyFrom (0x180010350) and then copies
// the +0x180 flag.  The base copy is implemented over the modeled ID (see
// global_mfc-feature-22_impl.cpp) and is forwarded here; the flag is not
// modeled.
// TODO(clean-room): transcribed partially -- the +0x180 separator flag is not
// modeled.
// Symbol: ?CopyFrom@CMFCRibbonSeparator@@MEAAXAEBVCMFCRibbonBaseElement@@@Z
extern "C" void MS_ABI impl__CopyFrom_CMFCRibbonSeparator__MEAAXAEBVCMFCRibbonBaseElement___Z(
    CMFCRibbonSeparator* pThis, const CMFCRibbonBaseElement& src)
{
    if (!pThis) return;
    impl__CopyFrom_CMFCRibbonBaseElement__UEAAXAEBV1__Z(pThis, src);
}
// CMFCRibbonSeparator::GetRegularSize(CDC*) -- retail writes CSize(4, 4) into
// the hidden return slot and returns it, ignoring the element and the DC.
// Transcribed faithfully (4px x 4px is the retail separator size).
// Symbol: ?GetRegularSize@CMFCRibbonSeparator@@MEAA?AVCSize@@PEAVCDC@@@Z
extern "C" CSize* MS_ABI impl__GetRegularSize_CMFCRibbonSeparator__MEAA_AVCSize__PEAVCDC___Z(
    CSize* pRet, CMFCRibbonSeparator* /*pThis*/, CDC* /*pDC*/)
{
    if (!pRet) return nullptr;
    pRet->cx = 4;
    pRet->cy = 4;
    return pRet;
}
// CMFCRibbonSeparator::OnDraw(CDC*) -- retail (RVA 0xf780) computes the
// separator line rect from the element rect (+0xc8) and the +0x180 flag and
// renders it through the visual-manager virtuals.  The element geometry and
// the visual-manager slots are not modeled, so nothing is drawn.
// TODO(clean-room): transcribed partially -- separator drawing is not modeled.
// Symbol: ?OnDraw@CMFCRibbonSeparator@@MEAAXPEAVCDC@@@Z
extern "C" void MS_ABI impl__OnDraw_CMFCRibbonSeparator__MEAAXPEAVCDC___Z(
    CMFCRibbonSeparator* pThis, CDC* /*pDC*/)
{
    if (!pThis) return;
}
// CMFCRibbonSeparator::OnDrawOnList(CDC*, CString, int, CRect, int, int) --
// retail (RVA 0xfa80) stores the list index into the +0x148 member, adjusts
// the DC text color and renders the separator through the parent bar's
// text-drawing virtual.  None of that state is modeled.
// TODO(clean-room): transcribed partially -- separator list drawing is not
// modeled.
// Symbol: ?OnDrawOnList@CMFCRibbonSeparator@@MEAAXPEAVCDC@@V?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@HVCRect@@HH@Z
extern "C" void MS_ABI impl__OnDrawOnList_CMFCRibbonSeparator__MEAAXPEAVCDC__V__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__HVCRect__HH_Z(
    CMFCRibbonSeparator* pThis, CDC* /*pDC*/, CString /*strText*/, int /*nTextOffset*/,
    CRect /*rect*/, int /*bIsHighlighted*/, int /*bIsDisabled*/)
{
    if (!pThis) return;
}
CMFCRibbonSeparator::CMFCRibbonSeparator() { memset(_pad, 0, sizeof(_pad)); }
CMFCRibbonSeparator::~CMFCRibbonSeparator() {}
