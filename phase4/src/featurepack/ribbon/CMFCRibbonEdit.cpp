// CMFCRibbonEdit — OpenMFC implementation.
// Sources: cbarcore.cpp, mfccore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/CbarcoreSupport.h"
#include "detail/MfccoreSupport.h"

// Symbol: ?GetCompactSize@CMFCRibbonEdit@@UEAA?AVCSize@@PEAVCDC@@@Z
extern "C" void MS_ABI impl__GetCompactSize_CMFCRibbonEdit__UEAA_AVCSize__PEAVCDC___Z(void* pRet, const CMFCRibbonEdit* pThis, CDC* /*pDC*/) {
    BuildCSizeResult(pRet, 80 + RibbonTextPixels(pThis), 22);
}
// Symbol: ?GetIntermediateSize@CMFCRibbonEdit@@UEAA?AVCSize@@PEAVCDC@@@Z
extern "C" void MS_ABI impl__GetIntermediateSize_CMFCRibbonEdit__UEAA_AVCSize__PEAVCDC___Z(void* pRet, const CMFCRibbonEdit* pThis, CDC* /*pDC*/) {
    BuildCSizeResult(pRet, 120 + RibbonTextPixels(pThis), 22);
}
// Symbol: ?OnDraw@CMFCRibbonEdit@@UEAAXPEAVCDC@@@Z
extern "C" void MS_ABI impl__OnDraw_CMFCRibbonEdit__UEAAXPEAVCDC___Z(CMFCRibbonEdit* pThis, CDC* pDC) {
    if (pThis == nullptr || pDC == nullptr) {
        return;
    }
    (void)CMFCVisualManager::GetInstance();
}
// Symbol: ?OnEnable@CMFCRibbonEdit@@UEAAXH@Z
extern "C" void MS_ABI impl__OnEnable_CMFCRibbonEdit__UEAAXH_Z(CMFCRibbonEdit* pThis, int bEnable) {
    if (!pThis) return;
    std::lock_guard<std::mutex> lock(g_ribbonMutex);
    g_ribbonElementsEnabled[pThis] = (bEnable != FALSE);
}
CMFCRibbonEdit::CMFCRibbonEdit() { memset(_pad, 0, sizeof(_pad)); }
CMFCRibbonEdit::~CMFCRibbonEdit() {}
// Retail CMFCRibbonEdit derives from CMFCRibbonButton and stores its edit state
// at +0x270..+0x2a8 (retail afxribbonedit.h:31-128; field stores confirmed by
// retail disassembly). include/openmfc/afxmfc.h:1771 instead declares a direct
// CMFCRibbonBaseElement with only a 32-byte tail, so these retail offsets are not
// present in the locally allocated object. Keep affected exports as documented
// stubs until that public class layout is corrected; do not access past the object.
// Symbol: ?CommonInit@CMFCRibbonEdit@@IEAAXXZ
// Retail mfc140u RVA 0xfaf90 initializes members at +0x190, +0x194, +0x208,
// +0x270..+0x2a8; the local class does not contain those fields.
extern "C" void MS_ABI impl__CommonInit_CMFCRibbonEdit__IEAAXXZ(void* pThis) {
    (void)pThis;
}

// Symbol: ?CopyFrom@CMFCRibbonEdit@@UEAAXAEBVCMFCRibbonBaseElement@@@Z
// Retail mfc140u RVA 0xfbd00 delegates base copying, assigns the edit text,
// destroys/clears control pointers at +0x2a0/+0x2a8, then copies scalar edit
// fields through +0x284. That layout is unavailable locally.
extern "C" void MS_ABI impl__CopyFrom_CMFCRibbonEdit__UEAAXAEBVCMFCRibbonBaseElement___Z(
    void* pThis, const CMFCRibbonBaseElement* pSource) {
    (void)pThis;
    (void)pSource;
}

// Symbol: ?CreateEdit@CMFCRibbonEdit@@UEAAPEAVCMFCRibbonRichEditCtrl@@PEAVCWnd@@K@Z
// Retail mfc140u RVA 0xfc1d0 allocates/constructs the edit control and reads
// CMFCRibbonEdit state at +0x118 and +0x28c; it cannot safely use this layout.
extern "C" void* MS_ABI impl__CreateEdit_CMFCRibbonEdit__UEAAPEAVCMFCRibbonRichEditCtrl__PEAVCWnd__K_Z(
    void* pThis, CWnd* pWndParent, unsigned long dwEditStyle) {
    (void)pThis;
    (void)pWndParent;
    (void)dwEditStyle;
    return nullptr;
}

// Symbol: ?CreateSpinButton@CMFCRibbonEdit@@IEAAHPEAVCMFCRibbonRichEditCtrl@@PEAVCWnd@@@Z
// Retail mfc140u RVA 0xfc2a0 conditionally creates a spin control using owner
// fields at +0x278..+0x2a8; the corresponding retail layout is absent locally.
extern "C" int MS_ABI impl__CreateSpinButton_CMFCRibbonEdit__IEAAHPEAVCMFCRibbonRichEditCtrl__PEAVCWnd___Z(
    void* pThis, void* pWndEdit, CWnd* pWndParent) {
    (void)pThis;
    (void)pWndEdit;
    (void)pWndParent;
    return 0;
}

// Symbol: ?DestroyCtrl@CMFCRibbonEdit@@UEAAXXZ
// Retail mfc140u RVA 0xfc900 destroys and clears the control pointers at
// +0x2a0/+0x2a8; those members are outside the local class layout.
extern "C" void MS_ABI impl__DestroyCtrl_CMFCRibbonEdit__UEAAXXZ(void* pThis) {
    (void)pThis;
}

// Symbol: ?EnableSpinButtons@CMFCRibbonEdit@@QEAAXHH@Z
// Retail mfc140u RVA 0xfb420 stores the range at +0x278/+0x27c and enables the
// spin flag at +0x28c before conditionally sending message 0x46f; layout missing.
extern "C" void MS_ABI impl__EnableSpinButtons_CMFCRibbonEdit__QEAAXHH_Z(
    void* pThis, int nMin, int nMax) {
    (void)pThis;
    (void)nMin;
    (void)nMax;
}

// Symbol: ?OnAfterChangeRect@CMFCRibbonEdit@@UEAAXPEAVCDC@@@Z
// Retail mfc140u RVA 0xfbe20 delegates to the base then may create/reposition
// edit and spin controls using inherited/edit-specific fields through +0x2a8.
extern "C" void MS_ABI impl__OnAfterChangeRect_CMFCRibbonEdit__UEAAXPEAVCDC___Z(
    void* pThis, CDC* pDC) {
    (void)pThis;
    (void)pDC;
}

// Symbol: ?OnDrawLabelAndImage@CMFCRibbonEdit@@UEAAXPEAVCDC@@@Z
// Retail mfc140u RVA 0xfb720 reads inherited ribbon state and performs virtual
// drawing calls; those base fields/vtable layout do not match this declaration.
extern "C" void MS_ABI impl__OnDrawLabelAndImage_CMFCRibbonEdit__UEAAXPEAVCDC___Z(
    void* pThis, CDC* pDC) {
    (void)pThis;
    (void)pDC;
}

// Symbol: ?OnDrawOnList@CMFCRibbonEdit@@UEAAXPEAVCDC@@V?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@HVCRect@@HH@Z
// Retail mfc140u RVA 0xfb920 draws through inherited ribbon state and virtual
// calls, and temporarily mutates base-element fields; the layout is unavailable.
extern "C" void MS_ABI impl__OnDrawOnList_CMFCRibbonEdit__UEAAXPEAVCDC__V__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__HVCRect__HH_Z(
    void* pThis, CDC* pDC, const CString* strText, int nTextOffset,
    const CRect* rect, int bIsSelected, int bIsHighlighted) {
    (void)pThis;
    (void)pDC;
    (void)strText;
    (void)nTextOffset;
    (void)rect;
    (void)bIsSelected;
    (void)bIsHighlighted;
}

// Symbol: ?OnHighlight@CMFCRibbonEdit@@UEAAXH@Z
// mfc140.dll ANSI-twin entry RVA 0xfd080: when the owned edit HWND exists, sets
// its highlighted state and sends a control message; --u has no resolved entry.
extern "C" void MS_ABI impl__OnHighlight_CMFCRibbonEdit__UEAAXH_Z(
    void* pThis, int bHighlight) {
    (void)pThis;
    (void)bHighlight;
}

// Symbol: ?OnKey@CMFCRibbonEdit@@UEAAHH@Z
// mfc140.dll ANSI-twin entry RVA 0xfd360 reads edit/focus members (+0x148,
// +0x298, +0x2a0) and dispatches through virtual calls; --u has no resolved entry.
extern "C" int MS_ABI impl__OnKey_CMFCRibbonEdit__UEAAHH_Z(void* pThis, int bIsMenuKey) {
    (void)pThis;
    (void)bIsMenuKey;
    return 0;
}

// Symbol: ?OnLButtonDown@CMFCRibbonEdit@@UEAAXVCPoint@@@Z
// mfc140.dll ANSI-twin entry RVA 0xfc8a0 is a tail jump to a fixed helper;
// --u has no resolved entry and the required base-class layout is not modeled.
extern "C" void MS_ABI impl__OnLButtonDown_CMFCRibbonEdit__UEAAXVCPoint___Z(
    void* pThis, long long point) {
    (void)pThis;
    (void)point;
}

// Symbol: ?OnLButtonUp@CMFCRibbonEdit@@UEAAXVCPoint@@@Z
// Neither the Unicode nor ANSI symbol map resolves this entry, and no local
// CMFCRibbonEdit layout is available to safely infer stateful mouse behavior.
extern "C" void MS_ABI impl__OnLButtonUp_CMFCRibbonEdit__UEAAXVCPoint___Z(
    void* pThis, long long point) {
    (void)pThis;
    (void)point;
}

// Symbol: ?OnRTLChanged@CMFCRibbonEdit@@UEAAXH@Z
// mfc140.dll ANSI-twin entry RVA 0xfd410 adjusts inherited rectangles and
// conditionally dispatches virtual calls; --u has no resolved entry.
extern "C" void MS_ABI impl__OnRTLChanged_CMFCRibbonEdit__UEAAXH_Z(
    void* pThis, int bIsRTL) {
    (void)pThis;
    (void)bIsRTL;
}

// Symbol: ?OnSetFocus@CMFCRibbonEdit@@UEAAXH@Z
// mfc140.dll ANSI-twin entry RVA 0xfd0f0 accesses the edit control at +0x2a0,
// updates focus state at +0x290 and calls virtual slots; --u has no resolved entry.
extern "C" void MS_ABI impl__OnSetFocus_CMFCRibbonEdit__UEAAXH_Z(
    void* pThis, int bSet) {
    (void)pThis;
    (void)bSet;
}

// Symbol: ?OnShow@CMFCRibbonEdit@@UEAAXH@Z
// mfc140.dll ANSI-twin entry RVA 0xfcf90 conditionally toggles visibility on
// the edit/spin HWNDs at +0x2a0/+0x2a8; --u has no resolved entry.
extern "C" void MS_ABI impl__OnShow_CMFCRibbonEdit__UEAAXH_Z(
    void* pThis, int bShow) {
    (void)pThis;
    (void)bShow;
}

// Symbol: ?PreLMouseDown@CMFCRibbonEdit@@UEAAHVCPoint@@@Z
// mfc140.dll ANSI-twin entry RVA 0xfd2c0 checks inherited/edit focus fields
// and invokes a virtual slot; --u has no resolved entry and the layout is absent.
extern "C" int MS_ABI impl__PreLMouseDown_CMFCRibbonEdit__UEAAHVCPoint___Z(
    void* pThis, long long point) {
    (void)pThis;
    (void)point;
    return 0;
}

// Symbol: ?Redraw@CMFCRibbonEdit@@UEAAXXZ
// mfc140.dll ANSI-twin entry RVA 0xfcff0 calls the base redraw then redraws
// the edit/spin controls at +0x2a0/+0x2a8; --u has no resolved entry.
extern "C" void MS_ABI impl__Redraw_CMFCRibbonEdit__UEAAXXZ(void* pThis) {
    (void)pThis;
}

// Symbol: ?RepositionRibbonEditCtrl@CMFCRibbonEdit@@IEAAXXZ
// mfc140.dll ANSI-twin entry RVA 0xfc9f0 creates and positions controls using
// a large set of inherited/edit members; --u has no resolved entry or layout.
extern "C" void MS_ABI impl__RepositionRibbonEditCtrl_CMFCRibbonEdit__IEAAXXZ(
    void* pThis) {
    (void)pThis;
}

// Symbol: ?SetACCData@CMFCRibbonEdit@@UEAAHPEAVCWnd@@AEAVCAccessibilityData@@@Z
// mfc140.dll ANSI-twin entry RVA 0xfd550 calls base accessibility handling,
// copies text from +0x298 and dispatches virtual getters; --u has no resolved entry.
extern "C" int MS_ABI impl__SetACCData_CMFCRibbonEdit__UEAAHPEAVCWnd__AEAVCAccessibilityData___Z(
    void* pThis, CWnd* pParent, CAccessibilityData* pData) {
    (void)pThis;
    (void)pParent;
    (void)pData;
    return 0;
}

// Symbol: ?SetEditText@CMFCRibbonEdit@@QEAAXV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@@Z
// The Unicode symbol has no resolved RVA; the ANSI twin has a distinct
// CStringA overload, not this CStringW export. The edit-string field is absent.
extern "C" void MS_ABI impl__SetEditText_CMFCRibbonEdit__QEAAXV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
    void* pThis, const CString* strText) {
    (void)pThis;
    (void)strText;
}

// Symbol: ?SetTextAlign@CMFCRibbonEdit@@QEAAXH@Z
// mfc140.dll ANSI-twin entry RVA 0xfc020 stores alignment at +0x280; --u has
// no resolved entry, and +0x280 is outside the local class layout.
extern "C" void MS_ABI impl__SetTextAlign_CMFCRibbonEdit__QEAAXH_Z(
    void* pThis, int nAlign) {
    (void)pThis;
    (void)nAlign;
}

// Symbol: ?SetWidth@CMFCRibbonEdit@@QEAAXHH@Z
// mfc140.dll ANSI-twin entry RVA 0xfc030 conditionally stores +0x270/+0x274
// only when no edit HWND exists; --u has no resolved entry and those fields are absent.
extern "C" void MS_ABI impl__SetWidth_CMFCRibbonEdit__QEAAXHH_Z(
    void* pThis, int nWidth, int bInFloatyMode) {
    (void)pThis;
    (void)nWidth;
    (void)bInFloatyMode;
}
