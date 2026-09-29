// CMFCVisualManagerWindows7 — OpenMFC implementation.
// Sources: mfccore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/MfccoreSupport.h"

// Symbol: ?CreateObject@CMFCVisualManagerWindows7@@SAPEAVCObject@@XZ
extern "C" CObject* MS_ABI impl__CreateObject_CMFCVisualManagerWindows7__SAPEAVCObject__XZ() { return new CMFCVisualManagerWindows7(); }
// Symbol: ?GetThisClass@CMFCVisualManagerWindows7@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMFCVisualManagerWindows7__SAPEAUCRuntimeClass__XZ() { return CMFCVisualManagerWindows7::GetThisClass(); }
// Symbol: ?GetRuntimeClass@CMFCVisualManagerWindows7@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CMFCVisualManagerWindows7__UEBAPEAUCRuntimeClass__XZ(CMFCVisualManagerWindows7* pThis) { return CMFCVisualManagerWindows7::GetThisClass(); }
// Symbol: ?SetStyle@CMFCVisualManagerWindows7@@SAHPEB_W@Z
extern "C" int MS_ABI impl__SetStyle_CMFCVisualManagerWindows7__SAHPEB_W_Z(const wchar_t* lpszPath) {
    return CMFCVisualManagerWindows7::SetStyle(lpszPath);
}
// Symbol: ?OnDrawButtonBorder@CMFCVisualManagerWindows7@@UEAAXPEAVCDC@@PEAVCMFCToolBarButton@@VCRect@@W4AFX_BUTTON_STATE@CMFCVisualManager@@@Z
extern "C" void MS_ABI impl__OnDrawButtonBorder_CMFCVisualManagerWindows7__UEAAXPEAVCDC__PEAVCMFCToolBarButton__VCRect__W4AFX_BUTTON_STATE_CMFCVisualManager___Z(
    CMFCVisualManagerWindows7* pThis, CDC* pDC, CMFCToolBarButton* pButton, CRect rect, CMFCVisualManager::AFX_BUTTON_STATE state) {
    pThis->CMFCVisualManager::OnDrawButtonBorder(pDC, pButton, rect, state);
}
// Symbol: ?OnDrawComboDropButton@CMFCVisualManagerWindows7@@UEAAXPEAVCDC@@VCRect@@HHHPEAVCMFCToolBarComboBoxButton@@@Z
extern "C" void MS_ABI impl__OnDrawComboDropButton_CMFCVisualManagerWindows7__UEAAXPEAVCDC__VCRect__HHHPEAVCMFCToolBarComboBoxButton___Z(
    CMFCVisualManagerWindows7* pThis, CDC* pDC, CRect rect, int bDisabled, int bIsDropped, int bIsHighlighted, CMFCToolBarComboBoxButton* pButton) {
    pThis->CMFCVisualManager::OnDrawComboDropButton(pDC, rect, bDisabled, bIsDropped, bIsHighlighted, pButton);
}
// Symbol: ?OnDrawMenuBorder@CMFCVisualManagerWindows7@@UEAAXPEAVCDC@@PEAVCMFCPopupMenu@@VCRect@@@Z
extern "C" void MS_ABI impl__OnDrawMenuBorder_CMFCVisualManagerWindows7__UEAAXPEAVCDC__PEAVCMFCPopupMenu__VCRect___Z(
    CMFCVisualManagerWindows7* pThis, CDC* pDC, CMFCPopupMenu* pMenu, CRect rect) {
    pThis->CMFCVisualManager::OnDrawMenuBorder(pDC, pMenu, rect);
}
// Symbol: ?OnDrawRibbonProgressBar@CMFCVisualManagerWindows7@@UEAAXPEAVCDC@@PEAVCMFCRibbonProgressBar@@VCRect@@2H@Z
extern "C" void MS_ABI impl__OnDrawRibbonProgressBar_CMFCVisualManagerWindows7__UEAAXPEAVCDC__PEAVCMFCRibbonProgressBar__VCRect__2H_Z(
    CMFCVisualManagerWindows7* pThis, CDC* pDC, CMFCRibbonProgressBar* pProgress,
    CRect rectProgress, CRect rectChunk, int bInfiniteMode) {
    pThis->OnDrawRibbonProgressBar(pDC, pProgress, rectProgress, rectChunk, bInfiniteMode);
}
CMFCVisualManagerWindows7::CMFCVisualManagerWindows7() {
    memset(_pad, 0, sizeof(_pad));
    ApplyVisualPaletteForClass(GetThisClass());
}
CMFCVisualManagerWindows7::~CMFCVisualManagerWindows7() {}
CObject* CMFCVisualManagerWindows7::CreateObject() { return new CMFCVisualManagerWindows7(); }
int CMFCVisualManagerWindows7::SetStyle(const wchar_t*) {
    ApplyVisualPaletteForClass(GetThisClass());
    return TRUE;
}

//=============================================================================
// CMFCVisualManagerWindows7 - Windows 7 (Aero) theme overrides
//=============================================================================
//
// Everything below this banner was decoded from the retail disassembly. Read
// this paragraph before adding to it, because almost every override in the
// class has the same shape and the shape is what determines what OpenMFC can
// reproduce.
//
// THE PREDICATE. Nearly every CMFCVisualManagerWindows7 override begins with
// `call 0x1ba7e0` (mfc140; an internal helper with no exported name) and
// branches on its result. That helper is, instruction for instruction:
//
//     if (afxGlobalData[+0x00] == 0) { afxGlobalData.Initialize(); }   // 0x6a5c0
//     if (afxGlobalData[+0x288] <= 8) return 0;
//     if (afxGlobalData[+0x00] == 0) { afxGlobalData.Initialize(); }
//     if (afxGlobalData[+0x264] != 0) return 0;
//     if (afxGlobalData[+0x260] != 0) return 0;
//     return this->[+0x248] != 0;
//
// (afxGlobalData is ?afxGlobalData@@3UAFX_GLOBAL_DATA@@A at mfc140 0x3ba380;
// the bracketed numbers above are byte offsets into it. Their MFC member names
// are NOT asserted here -- they were not identified.)
//
// The last term is what matters: this->[+0x248] is an int that
// ??0CMFCVisualManagerWindows7@@QEAA@XZ (mfc140 0x1ba8b0) zero-initialises at
// 0x1ba914 (`mov %ecx,0x248(%rbx)` with ecx == 0). Only loading the Windows 7
// style resource module sets it. OpenMFC's CMFCVisualManagerWindows7 adds a
// 64-byte opaque block to CMFCVisualManager's 256-byte one, so there is no
// member at +0x248 at all and nothing in this build ever sets one: the
// predicate has no representation here and only its FALSE branch is
// meaningful. Every "themed" branch below is therefore documented, not
// implemented -- and where the FALSE branch is itself just `return 0`, the
// symbol is left an honest stub rather than dressed up as an implementation.
//
// THE SUBSTITUTED BASE CALL. Retail's inheritance chain is
// CMFCVisualManagerWindows7 -> CMFCVisualManagerWindows -> CMFCVisualManagerOfficeXP
// -> CMFCVisualManager (the constructor at 0x1ba8b0 calls
// ??0CMFCVisualManagerWindows@@QEAA@H@Z at 0x1b6fe0; the two links above that
// are the documented MFC hierarchy and were not re-derived here). Several
// FALSE branches therefore call a CMFCVisualManagerOfficeXP:: or
// CMFCVisualManagerWindows:: body. OpenMFC's CMFCVisualManagerWindows7 derives
// directly from CMFCVisualManager, so none of those is reachable as a base
// call; where this tree happens to have a working impl__ thunk for the retail
// callee, invoking it would mean handing a CMFCVisualManagerWindows7* to an
// unrelated class as its `this`. Those calls are routed to
// CMFCVisualManager's implementation instead. Each such case says so at its
// call site; that is a deliberate deviation from retail, not a transcription.
//
// RVAs are mfc140.dll (the ANSI twin) unless a line says (mfc140u). Bodies are
// byte-identical between the two images; the addresses are not.

// File-local drawing helpers, kept per file the way the sibling visual
// manager units do (XP_Hdc in CMFCVisualManagerOfficeXP.cpp,
// Office2007DrawSeparatorLines in CMFCVisualManagerOffice2007.cpp).
// detail/MfccoreSupport.h declares openmfc::detail::mfccore::SafeHdc / ToRECT
// with definitions in MfccoreSupport.cpp; nothing prevents linking to those,
// the local copies simply follow the surrounding convention.
namespace {

inline HDC W7SafeHdc(CDC* pDC) { return pDC ? pDC->GetSafeHdc() : nullptr; }

inline RECT W7ToRECT(const CRect& rect) {
    RECT r; r.left = rect.left; r.top = rect.top; r.right = rect.right; r.bottom = rect.bottom;
    return r;
}

// The shape of ?SetBkMode@CDC@@QEAAHH@Z (mfc140 0x2a07a0) and the virtual
// ?SetTextColor@CDC@@UEAAKK@Z (0x2a08a0, CDC vtable +0x70): apply to m_hDC
// when it differs from m_hAttribDC, then to m_hAttribDC, whose previous value
// is what the caller gets back. DrawNcText below restores through these.
inline int W7SetBkMode(CDC* pDC, int nMode) {
    int nOld = 0;
    if (pDC->m_hDC != nullptr && pDC->m_hDC != pDC->m_hAttribDC) ::SetBkMode(pDC->m_hDC, nMode);
    if (pDC->m_hAttribDC != nullptr) nOld = ::SetBkMode(pDC->m_hAttribDC, nMode);
    return nOld;
}
inline COLORREF W7SetTextColor(CDC* pDC, COLORREF clr) {
    COLORREF clrOld = CLR_INVALID;
    if (pDC->m_hDC != nullptr && pDC->m_hDC != pDC->m_hAttribDC) ::SetTextColor(pDC->m_hDC, clr);
    if (pDC->m_hAttribDC != nullptr) clrOld = ::SetTextColor(pDC->m_hAttribDC, clr);
    return clrOld;
}

// The two style-module statics SetResourceHandle / CleanStyle /
// OnUpdateSystemColors share. In retail they are unnamed .bss variables
// (mfc140 0x3b7140 = the module handle, 0x3b713c = the auto-free flag; in
// mfc140u 0x3be3e0 / 0x3be3dc, read out of SetResourceHandle at RVA 0x1bce00
// (mfc140u)). They are not exports, so file-local statics model them exactly.
// The flag sits in zero-initialised .bss and the only direct store to it in
// the whole mfc140u image is SetResourceHandle's `movl $0x0` at 0x1bce0b
// (mfc140u) -- so, as far as the object code shows, retail never makes it
// TRUE either. It is kept so CleanStyle can be transcribed as written.
HINSTANCE s_hinstW7Res = nullptr;
BOOL      s_bW7AutoFreeRes = FALSE;

} // namespace

// Thunks the style-module entry points call (see SetResourceHandle and
// OnUpdateSystemColors below).
extern "C" int MS_ABI impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(
    const CObject* pThis, const CRuntimeClass* pClass);                  // core/runtime/CObject.cpp
extern "C" void MS_ABI impl__OnUpdateSystemColors_CMFCVisualManager__UEAAXXZ(
    CMFCVisualManager* pThis);                                          // CMFCVisualManager.cpp
extern "C" void MS_ABI impl__CleanUp_CMFCVisualManagerWindows7__UEAAXXZ(
    CMFCVisualManagerWindows7* pThis);                                  // this file
extern "C" void MS_ABI impl__OnUpdateSystemColors_CMFCVisualManagerWindows7__UEAAXXZ(
    CMFCVisualManagerWindows7* pThis);                                  // this file
// Both defined in CMFCVisualManager.cpp; retail's FALSE-predicate paths reach
// them (directly, or as this file's substitute -- see each call site).
extern "C" void MS_ABI impl__OnFillRibbonQuickAccessToolBarPopup_CMFCVisualManager__UEAAXPEAVCDC__PEAVCMFCRibbonPanelMenuBar__VCRect___Z(
    CMFCVisualManager* pThis, CDC* pDC, void* pMenuBar, CRect rect);
extern "C" void MS_ABI impl__OnHighlightMenuItem_CMFCVisualManager__UEAAXPEAVCDC__PEAVCMFCToolBarMenuButton__VCRect__AEAK_Z(
    CMFCVisualManager* pThis, CDC* pDC, CMFCToolBarMenuButton* pButton, CRect rect,
    unsigned long& clrText);

// Base-class entry points this unit calls through their extern "C" impl__
// thunks (all defined in CMFCVisualManager.cpp) because OpenMFC's headers
// declare no matching CMFCVisualManager virtual for them.
extern "C" unsigned long MS_ABI impl__GetRibbonEditBackgroundColor_CMFCVisualManager__UEAAKPEAVCMFCRibbonRichEditCtrl__HHH_Z(
    CMFCVisualManager* pThis, void* pEdit, int bIsHighlighted, int bIsPaneHighlighted, int bIsDisabled);
extern "C" void MS_ABI impl__OnDrawDefaultRibbonImage_CMFCVisualManager__UEAAXPEAVCDC__VCRect__HHH_Z(
    CMFCVisualManager* pThis, CDC* pDC, CRect rectImage, int bIsDisabled, int bIsPressed, int bIsHighlighted);
extern "C" unsigned long MS_ABI impl__OnDrawMenuLabel_CMFCVisualManager__UEAAKPEAVCDC__VCRect___Z(
    CMFCVisualManager* pThis, CDC* pDC, CRect rect);
extern "C" void MS_ABI impl__OnDrawRibbonGalleryButton_CMFCVisualManager__UEAAXPEAVCDC__PEAVCMFCRibbonGalleryIcon___Z(
    CMFCVisualManager* pThis, CDC* pDC, void* pButton);
extern "C" void MS_ABI impl__OnDrawRibbonLaunchButton_CMFCVisualManager__UEAAXPEAVCDC__PEAVCMFCRibbonLaunchButton__PEAVCMFCRibbonPanel___Z(
    CMFCVisualManager* pThis, CDC* pDC, void* pButton, CMFCRibbonPanel* pPanel);
extern "C" void MS_ABI impl__OnDrawRibbonMenuCheckFrame_CMFCVisualManager__UEAAXPEAVCDC__PEAVCMFCRibbonButton__VCRect___Z(
    CMFCVisualManager* pThis, CDC* pDC, CMFCRibbonButton* pButton, CRect rect);
extern "C" void MS_ABI impl__OnDrawRibbonRecentFilesFrame_CMFCVisualManager__UEAAXPEAVCDC__PEAVCMFCRibbonMainPanel__VCRect___Z(
    CMFCVisualManager* pThis, CDC* pDC, CMFCRibbonMainPanel* pPanel, CRect rect);
// CMFCVisualManager vtable +0x5a8 in retail (mfc140 0x18d7e0, read out of the
// CMFCVisualManagerWindows7 vftable at 0x31cdd0 -- Windows7 does not override
// it). Defined in CMFCVisualManager.cpp.
extern "C" int MS_ABI impl__DrawTextOnGlass_CMFCVisualManager__UEAAHPEAVCDC__V__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__VCRect__KHK_Z(
    CMFCVisualManager* pThis, CDC* pDC, const CString& strText, CRect rect,
    unsigned long dwFlags, int nGlowSize, unsigned long clrText);
// CMFCBaseVisualManager vtable +0x58 in retail (mfc140 0x18e6f0, same vftable
// read; not overridden by Windows7). Defined in CMFCBaseVisualManager.cpp,
// where its first parameter is typed void*.
extern "C" int MS_ABI impl__DrawRadioButton_CMFCBaseVisualManager__UEAAHPEAVCDC__VCRect__HHHH_Z(
    void* pThis, CDC* pDC, CRect rect, int bHighlighted, int bChecked, int bEnabled, int bPressed);

// Symbol: ?CleanStyle@CMFCVisualManagerWindows7@@SAXXZ
// Retail 0x1bb500, complete and short:
//     if (g_bAutoFreeRes /*0x3b713c*/ && (UINT_PTR)g_hinstRes /*0x3b7140*/ > 0x20)
//         ::FreeLibrary(g_hinstRes);          // import slot 0x2c4698
//     g_hinstRes = NULL;
//     g_strStylePrefix /*0x3bbff0*/ .Empty(); // tail-jmp 0x3430 =
//                                             // ?Empty@?$CSimpleStringT@D$00@ATL@@QEAAXXZ
// None of the three statics is an exported name. The module handle and the
// auto-free flag are modelled by this file's s_hinstW7Res / s_bW7AutoFreeRes
// (SetResourceHandle is what fills the handle), so those two steps are
// transcribed. g_strStylePrefix is not modelled: nothing in OpenMFC ever
// fills it (see MakeResourceID), so emptying it has nothing to do.
// s_bW7AutoFreeRes is never TRUE (see its declaration: retail itself has no
// store of a non-zero value to it), so the FreeLibrary is transcribed but
// unreachable -- a handle given to SetResourceHandle stays the caller's.
extern "C" void MS_ABI impl__CleanStyle_CMFCVisualManagerWindows7__SAXXZ()
{
    if (s_bW7AutoFreeRes && reinterpret_cast<UINT_PTR>(s_hinstW7Res) > 0x20) {
        ::FreeLibrary(s_hinstW7Res);
    }
    s_hinstW7Res = nullptr;
}

// Symbol: ?CleanUp@CMFCVisualManagerWindows7@@UEAAXXZ
// Retail 0x1bb9a0 tears down the whole Windows 7 theme block: it calls
// ?DeleteObject@CGdiObject@@QEAAHXZ (0x2a1ea0) on the four GDI objects at
// this->[+0x218, +0x228, +0x238, +0x250], ?Clear@CMFCToolBarImages@@QEAAXXZ
// (0x16dcb0) on the images at [+0x698] and [+0x830], and a virtual at vtable
// +0x60 on each member of the long run of CMFCControlRenderer sub-objects that
// starts at [+0x298] and continues 0x200 bytes apart ([+0x498], [+0x9c8],
// [+0xbc8], [+0xdc8], [+0xfd8], [+0x11d8] ...). They are CMFCControlRenderers
// because ??0CMFCVisualManagerWindows7@@QEAA@XZ constructs them with
// ??0CMFCControlRenderer@@QEAA@XZ (0x32170).
// Those members are all inside the ~0x9300-byte retail object; OpenMFC models
// none of them (the derived block here is 64 opaque bytes), so there is no
// state to reset. Left a no-op.
extern "C" void MS_ABI impl__CleanUp_CMFCVisualManagerWindows7__UEAAXXZ(CMFCVisualManagerWindows7* /*pThis*/) {}

// Symbol: ?DrawNcBtn@CMFCVisualManagerWindows7@@UEAAXPEAVCDC@@AEBVCRect@@IW4AFX_BUTTON_STATE@CMFCVisualManager@@HHH@Z
// Retail 0x1bb580. This one does NOT use the predicate above; it branches on
// this->[+0x8], the HTHEME the base visual manager opens for the window class
// (proved by the use it is put to: it is passed as the first argument of
// UxTheme!DrawThemeBackground, import slot 0x2c53e0).
//
// THEMED branch (this->[+0x8] != NULL), transcribed but NOT implemented -- see
// the note after it:
//     SC_CLOSE    (0xF060): part = p7 ? 0x14 : 0x12 + (p5 ? 1 : 0)
//     SC_MINIMIZE (0xF020): part = p7 ? 0x10 : (p5 ? 0    : 0x0F)
//     SC_MAXIMIZE (0xF030): part = p7 ? 0    : (p5 ? 0    : 0x11)
//     SC_RESTORE  (0xF120): part = 0x15 + (p7 ? 1 : 0)
//     SC_HELP     (0xF180): part = p7 ? 0x18 : (p5 ? 0    : 0x17)
//     anything else: return
//     state = (nState == 0) ? (p6 ? 1 : 4) : (nState == 2 ? 2 : 3)
//     if (part == 0) return;
//     ::DrawThemeBackground(this->[+0x8], pDC ? pDC->m_hDC : NULL,
//                           part, state, &rect /*the caller's, not a copy*/, NULL);
//
// NON-THEMED branch (this->[+0x8] == NULL) -- this is the one implemented
// below, verbatim:
//     SC_MINIMIZE -> DFCS_CAPTIONMIN(1), SC_MAXIMIZE -> DFCS_CAPTIONMAX(2),
//     SC_CLOSE -> DFCS_CAPTIONCLOSE(0), SC_RESTORE -> DFCS_CAPTIONRESTORE(3),
//     SC_HELP -> DFCS_CAPTIONHELP(4) (the `lea 0x4(%rcx)` at 0x1bb5ce adds
//     this->[+0x8], which is NULL on this branch); anything else returns.
//     uState = part | (p6 ? 0 : DFCS_INACTIVE);          // `bts $8` + cmovne
//     if (nState != 0) uState |= (nState == 2) ? DFCS_HOT : DFCS_PUSHED;
//     ::DrawFrameControl(pDC->m_hDC, &rectCopy, DFC_CAPTION, uState);
// The `cmp $0x2` then `cmove` of 0x1000 over 0x200 at 0x1bb606..0x1bb614
// selects DFCS_HOT for state 2, which in afxvisualmanager.h's enum order
// (Regular=0, Pressed=1, Highlighted=2) is ButtonsIsHighlighted; every other
// non-regular state selects DFCS_PUSHED.
//
// OpenMFC's CMFCVisualManager has no theme-handle member -- its derived
// storage is one opaque padding array -- so reading this->[+0x8] would be an
// invented offset. The themed branch is therefore documented above and not
// executed; this build always takes the DrawFrameControl path.
// One addition to the transcription: retail's GDI branch reads pDC->m_hDC
// without a NULL test (only the themed branch tests pDC); the `if (!hdc)`
// guard below is OpenMFC's, not retail's.
extern "C" void MS_ABI impl__DrawNcBtn_CMFCVisualManagerWindows7__UEAAXPEAVCDC__AEBVCRect__IW4AFX_BUTTON_STATE_CMFCVisualManager__HHH_Z(
    CMFCVisualManagerWindows7* pThis, CDC* pDC, const CRect& rect, unsigned int nButton,
    CMFCVisualManager::AFX_BUTTON_STATE nState, int /*p5*/, int p6, int /*p7*/)
{
    if (!pThis) return;
    HDC hdc = W7SafeHdc(pDC);
    if (!hdc) return;

    UINT uState;
    switch (nButton) {
    case SC_MINIMIZE: uState = DFCS_CAPTIONMIN;     break;
    case SC_MAXIMIZE: uState = DFCS_CAPTIONMAX;     break;
    case SC_CLOSE:    uState = DFCS_CAPTIONCLOSE;   break;
    case SC_RESTORE:  uState = DFCS_CAPTIONRESTORE; break;
    case 0xF180U /* SC_HELP; not defined by this SDK's WINVER */:
                      uState = DFCS_CAPTIONHELP;    break;
    default: return;
    }
    if (!p6) uState |= DFCS_INACTIVE;
    if (nState != CMFCVisualManager::ButtonsIsRegular) {
        uState |= (nState == CMFCVisualManager::ButtonsIsHighlighted) ? DFCS_HOT : DFCS_PUSHED;
    }
    RECT nativeRect = W7ToRECT(rect);
    ::DrawFrameControl(hdc, &nativeRect, DFC_CAPTION, uState);
}

// Symbol: ?DrawNcText@CMFCVisualManagerWindows7@@UEAAXPEAVCDC@@AEAVCRect@@AEBV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@HHHHHK@Z
// Retail entry 0x1bd190 (mfc140u; the ANSI twin with the `D` CStringT is at
// mfc140 0x1bb790 and is the same body). Transcribed in full. Parameter
// names below are assigned from the roles the object code gives them -- the
// 5th selects DT_RTLREADING, the 6th centres, the 7th selects the glass path,
// the 8th/9th are passed straight through to DrawTextOnGlass -- and match
// the public MFC header's (bActive, bIsRTL, bTextCenter, bGlass, nGlowSize,
// clrText); the names are not asserted from the object code beyond that.
//     if (str.GetLength() == 0) return;                 // `cmp %ebx,-0x10(%rax)`
//     if (rect.right <= rect.left) return;
//     nOldBk  = pDC->SetBkMode(TRANSPARENT);            // 0x2a2860 (mfc140u)
//     clrOld  = ::GetTextColor(pDC->m_hAttribDC);       // slot 0x2c6268 (mfc140u)
//     dt      = DT_END_ELLIPSIS|DT_NOPREFIX|DT_SINGLELINE|DT_VCENTER (0x8824)
//               | (bIsRTL ? DT_RTLREADING (0x20000) : 0);
//     clr     = bActive ? afxGlobalData[+0x80] : afxGlobalData[+0x90];
//     ::GetTextExtentPoint32W(pDC->m_hAttribDC, str, len, &sz);   // slot 0x2c62a0
//     if (bTextCenter && sz.cx < rect.Width())
//         rect.left += (rect.Width() - sz.cx) / 2;      // stores into the caller's rect
//     rect.right = min(rect.left + sz.cx, rect.right);
//     if (rect.right > rect.left) {
//         if (bGlass) this->vtable[+0x5a8](pDC, CString(str), CRect(rect), dt, nGlowSize, clrText);
//                                                       // = ?DrawTextOnGlass@CMFCVisualManager@@
//         else { pDC->SetTextColor(clr);                // CDC vtable +0x70
//                pDC->DrawText(str, len, &rect, dt); }  // CDC vtable +0xe0 -> USER32!DrawText
//     }
//     pDC->SetBkMode(nOldBk);
//     pDC->SetTextColor(clrOld);
// afxGlobalData+0x80 / +0x90 are what ?UpdateSysColors@AFX_GLOBAL_DATA@@QEAAXXZ
// (mfc140 0x6afd0) fills from ::GetSysColor(COLOR_CAPTIONTEXT /*9*/) at
// 0x6b118..0x6b123 and ::GetSysColor(COLOR_INACTIVECAPTIONTEXT /*19*/) at
// 0x6b1a0..0x6b1ab. OpenMFC does not model the AFX_GLOBAL_DATA colour table,
// so -- as the sibling files do for every afxGlobalData colour -- the two
// values are read live from ::GetSysColor. That is the one deviation: retail
// serves the value cached at the last UpdateSysColors.
// Retail never NULL-tests pDC; the `if (!hdc)` guard is OpenMFC's.
extern "C" void MS_ABI impl__DrawNcText_CMFCVisualManagerWindows7__UEAAXPEAVCDC__AEAVCRect__AEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__HHHHHK_Z(
    CMFCVisualManagerWindows7* pThis, CDC* pDC, CRect& rect, const CString& strTitle,
    int bActive, int bIsRTL, int bTextCenter, int bGlass, int nGlowSize, unsigned long clrText)
{
    if (!pThis) return;
    HDC hdc = W7SafeHdc(pDC);
    if (!hdc) return;
    const int nLen = strTitle.GetLength();
    if (nLen == 0) return;
    if (rect.right <= rect.left) return;

    HDC hAttribDC = pDC->m_hAttribDC != nullptr ? pDC->m_hAttribDC : hdc;
    const int nOldBkMode = W7SetBkMode(pDC, TRANSPARENT);
    const COLORREF clrOld = ::GetTextColor(hAttribDC);

    UINT dtFlags = DT_END_ELLIPSIS | DT_NOPREFIX | DT_SINGLELINE | DT_VCENTER;
    if (bIsRTL) dtFlags |= DT_RTLREADING;
    const COLORREF clrTitle = bActive ? ::GetSysColor(COLOR_CAPTIONTEXT)
                                      : ::GetSysColor(COLOR_INACTIVECAPTIONTEXT);

    SIZE sz; sz.cx = 0; sz.cy = 0;
    ::GetTextExtentPoint32W(hAttribDC, strTitle.GetString(), nLen, &sz);

    if (bTextCenter) {
        const int nWidth = rect.right - rect.left;
        if (sz.cx < nWidth) rect.left += (nWidth - sz.cx) / 2;
    }
    {
        const int nRight = rect.left + sz.cx;
        if (nRight < rect.right) rect.right = nRight;
    }

    if (rect.right > rect.left) {
        if (bGlass) {
            impl__DrawTextOnGlass_CMFCVisualManager__UEAAHPEAVCDC__V__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__VCRect__KHK_Z(
                pThis, pDC, strTitle, rect, dtFlags, nGlowSize, clrText);
        } else {
            W7SetTextColor(pDC, clrTitle);
            RECT r = W7ToRECT(rect);
            ::DrawTextW(hdc, strTitle.GetString(), nLen, &r, dtFlags);
        }
    }

    W7SetBkMode(pDC, nOldBkMode);
    W7SetTextColor(pDC, clrOld);
}

// Symbol: ?DrawSeparator@CMFCVisualManagerWindows7@@UEAAXPEAVCDC@@AEBVCRect@@AEAVCPen@@2H@Z
// Retail 0x1a2370, no predicate. (In mfc140u this name and
// ?DrawSeparator@CMFCVisualManagerOffice2007@@IEAAX...AEAVCPen@@2H@Z both
// resolve to the single RVA 0x1a3d70 through the export table -- the same
// COMDAT fold CMFCVisualManagerOffice2007.cpp records.) It collapses the
// caller's rect onto the separator's centre line and draws two adjacent
// 1-pixel lines, one per pen:
//     if (bHorz) {                                  // 0x1a23c5..0x1a23f4
//         y  = rect.top - 1 + (rect.bottom - rect.top) / 2;
//         line1 = (rect.left, y) -> (rect.right, y);
//         line2 = line1 offset by (0, +1);          // USER32!OffsetRect, 0x2c5318
//     } else {                                      // 0x1a23f6..0x1a2427
//         x  = rect.left - 1 + (rect.right - rect.left) / 2;
//         line1 = (x, rect.top) -> (x, rect.bottom);
//         line2 = line1 offset by (+1, 0);
//     }
// It then paints the two lines one of two ways, selected at 0x1a2427 by the
// exported static ?m_bIsDrawOnGlass@CMFCToolBarImages@@2HA (mfc140 0x3b70f4):
//   * non-zero: it pulls each pen's colour out with GDI32!GetObjectA
//     (slot 0x2c4250, 16 bytes == sizeof(LOGPEN), colour at +0xc) and calls
//     ?DrawLine@CDrawingManager@@QEAAXHHHHK@Z (0x59350) once per line;
//   * zero: it selects pen1 into the DC (?SelectObject@CDC@@, 0x2a0670),
//     MoveTo/LineTo (0x2a1000 / 0x2a1060) the first line, selects pen2, draws
//     the second line, then restores the original pen.
// This reproduces the zero branch exactly (select the caller's pen, MoveTo /
// LineTo, restore) on pDC->m_hDC. The glass branch is not reproduced:
// OpenMFC's ?DrawLine@CDrawingManager@@ thunk is an empty stub
// (core/gdi/CDrawingManager.cpp), so there is nothing behind it to call, and
// the flag is not consulted here. The pen NULL tests are OpenMFC's, not
// retail's: retail selects both pens unconditionally (the ?SelectObject@CDC@@
// calls at 0x1a24af and 0x1a24e7 inside DrawSeparator), so a pen whose
// m_hObject is NULL makes that ::SelectObject fail and its line is drawn with
// whatever pen the DC already holds; here such a line is skipped.
extern "C" void MS_ABI impl__DrawSeparator_CMFCVisualManagerWindows7__UEAAXPEAVCDC__AEBVCRect__AEAVCPen__2H_Z(
    CMFCVisualManagerWindows7* pThis, CDC* pDC, const CRect& rect, CPen& pen1, CPen& pen2, int bHorz)
{
    if (!pThis) return;
    HDC hdc = W7SafeHdc(pDC);
    if (!hdc) return;

    int x1, y1, x2, y2;
    int dx, dy;
    if (bHorz) {
        const int y = rect.top - 1 + (rect.bottom - rect.top) / 2;
        x1 = rect.left; y1 = y; x2 = rect.right; y2 = y;
        dx = 0; dy = 1;
    } else {
        const int x = rect.left - 1 + (rect.right - rect.left) / 2;
        x1 = x; y1 = rect.top; x2 = x; y2 = rect.bottom;
        dx = 1; dy = 0;
    }

    HGDIOBJ hOld = nullptr;
    if (pen1.m_hObject != nullptr) {
        hOld = ::SelectObject(hdc, pen1.m_hObject);
        ::MoveToEx(hdc, x1, y1, nullptr);
        ::LineTo(hdc, x2, y2);
    }
    if (pen2.m_hObject != nullptr) {
        HGDIOBJ hPrev = ::SelectObject(hdc, pen2.m_hObject);
        if (hOld == nullptr) hOld = hPrev;
        ::MoveToEx(hdc, x1 + dx, y1 + dy, nullptr);
        ::LineTo(hdc, x2 + dx, y2 + dy);
    }
    if (hOld != nullptr) ::SelectObject(hdc, hOld);
}

// Symbol: ?GetRibbonBar@CMFCVisualManagerWindows7@@QEBAPEAVCMFCRibbonBar@@PEAVCWnd@@@Z
// Retail 0x1a16b0, resolved in full:
//     if (pWnd == NULL) pWnd = <AfxGetMainWnd>:
//                       ?AfxGetModuleThreadState@@YAPEAVAFX_MODULE_THREAD_STATE@@XZ
//                       (0x1346a0), read its [+0x8], call that object's
//                       vtable +0xf8;
//     if (pWnd == NULL || pWnd->m_hWnd (+0x40) == NULL) return NULL;
//     if (pWnd->IsKindOf(RUNTIME_CLASS(CFrameWndEx)))    return pWnd->[+0x370];
//     if (pWnd->IsKindOf(RUNTIME_CLASS(CMDIFrameWndEx))) return pWnd->[+0x710];
//     return NULL;
// The two descriptors were identified by reading their m_lpszClassName: the
// one at mfc140 0x2e7160 names "CFrameWndEx" and the one at 0x2ec3c8 names
// "CMDIFrameWndEx". +0x370 and +0x710 are the ribbon-bar pointers inside those
// frames, and OpenMFC's CFrameWndEx / CMDIFrameWndEx do not model them (their
// retail sizes are 0x6f0 / 0x4710, nothing like this tree's). Resolved but not
// reproducible: left a stub returning NULL. What is missing is the frame
// layout, not the disassembly.
// (?GetRibbonBar@CMFCVisualManagerOffice2007@@... is COMDAT-folded onto this
// same body and is stubbed the same way in CMFCVisualManagerOffice2007.cpp.)
extern "C" CMFCRibbonBar* MS_ABI impl__GetRibbonBar_CMFCVisualManagerWindows7__QEBAPEAVCMFCRibbonBar__PEAVCWnd___Z(
    const CMFCVisualManagerWindows7* /*pThis*/, CWnd* /*pWnd*/)
{
    return nullptr;
}

// Symbol: ?GetRibbonEditBackgroundColor@CMFCVisualManagerWindows7@@UEAAKPEAVCMFCRibbonRichEditCtrl@@HHH@Z
// Retail 0x1c2b40. Predicate FALSE (0x1c2b75) tail-calls 0x18d0e0 =
// ?GetRibbonEditBackgroundColor@CMFCVisualManager@@... with the same four
// arguments; predicate TRUE returns this->[+0x274] when bIsDisabled,
// this->[+0x278] when bIsHighlighted, else this->[+0x270].
// Only the FALSE branch is reachable here (see the banner), so the call is
// what this reproduces.
extern "C" unsigned long MS_ABI impl__GetRibbonEditBackgroundColor_CMFCVisualManagerWindows7__UEAAKPEAVCMFCRibbonRichEditCtrl__HHH_Z(
    CMFCVisualManagerWindows7* pThis, void* pEdit, int bIsHighlighted, int bIsPaneHighlighted, int bIsDisabled)
{
    if (!pThis) return 0;
    return impl__GetRibbonEditBackgroundColor_CMFCVisualManager__UEAAKPEAVCMFCRibbonRichEditCtrl__HHH_Z(
        pThis, pEdit, bIsHighlighted, bIsPaneHighlighted, bIsDisabled);
}

// Symbol: ?GetRibbonPopupBorderSize@CMFCVisualManagerWindows7@@UEBAHPEBVCMFCRibbonPanelMenu@@@Z
// Retail 0x1c2550. Predicate FALSE returns this->[+0xf4] directly
// (`mov 0xf4(%rbx),%eax` at 0x1c2569). Predicate TRUE walks the panel menu
// (vtable +0x3a0, IsKindOf against the CRuntimeClass at 0x307f90, then a
// chain of menu/manager members) before falling back to a virtual at vtable
// +0x5f8 on `this`. Only the FALSE branch is reachable here.
// +0xf4 is a CMFCVisualManager base member. It lies inside OpenMFC's
// zero-filled `_visualmanager_padding`, but no OpenMFC constructor stores the
// retail value there (only this file's OnUpdateSystemColors, on its
// non-NULL-handle path, writes it), so reading it would yield 0 on a fresh
// manager. Its retail value on a constructed Windows7 manager IS established
// by the constructor chain 0x1ba8b0 -> 0x1b6fe0 -> 0x1abe10 -> 0x182640:
//   ??0CMFCVisualManager@@QEAA@H@Z          (0x182640) `movl $0x2,0xf4(%rbx)` at 0x18270e
//   ??0CMFCVisualManagerOfficeXP@@IEAA@H@Z  (0x1abe10) `mov  %edi,0xf4(%rbx)` at 0x1abf49,
//                                                      edi = 1 (`lea 0x1(%rdx)` at 0x1abeea)
//   ??0CMFCVisualManagerWindows@@QEAA@H@Z   (0x1b6fe0) does not touch +0xf4
//   ??0CMFCVisualManagerWindows7@@QEAA@XZ   (0x1ba8b0) does not touch +0xf4
// and the only other store to +0xf4 anywhere in 0x1b6000..0x1c3000 (the
// Windows / Windows7 text) is ?OnUpdateSystemColors@CMFCVisualManagerWindows7@@
// (0x1bbea0), which stores 1 again at 0x1bbf1e..0x1bbf25. So the member reads
// 1 for every Windows7 manager, before and after a style load, and 1 is
// returned here -- the same reading, by the same argument, that
// CMFCVisualManagerOffice2007.cpp records for its twin at 0x1aad20.
extern "C" int MS_ABI impl__GetRibbonPopupBorderSize_CMFCVisualManagerWindows7__UEBAHPEBVCMFCRibbonPanelMenu___Z(
    const CMFCVisualManagerWindows7* /*pThis*/, const void* /*pMenu*/)
{
    return 1;
}

// Symbol: ?GetStyleResourceID@CMFCVisualManagerWindows7@@KA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@XZ
// Read in BOTH images, because the literal is the whole function: the ANSI
// twin is at mfc140 0x1bb3e0 and the Unicode export itself is at
// mfc140u 0x1bcde0. Each is four instructions -- construct the caller's return
// CString from one string constant (mfc140 0x180340e58 = "WINDOWS7_IDX_STYLE",
// mfc140u 0x180346880 = L"WINDOWS7_IDX_STYLE") and echo the buffer in RAX.
// Reproduced exactly. This also repairs the generated stub's ABI, which
// returned nullptr instead of the caller's return buffer.
extern "C" CString* MS_ABI impl__GetStyleResourceID_CMFCVisualManagerWindows7__KA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__XZ(
    CString* pRet)
{
    if (!pRet) return pRet;
    new (pRet) CString(L"WINDOWS7_IDX_STYLE");
    return pRet;
}

// Symbol: ?GetSystemBorders@CMFCVisualManagerWindows7@@IEBA?AVCSize@@H@Z
// Retail 0x1ab260, self-contained and complete:
//     size.cy = ::GetSystemMetrics(SM_CXSIZEFRAME);   // 0x20, import 0x2c4c50
//     size.cx = ::GetSystemMetrics(SM_CYSIZEFRAME);   // 0x21
//     if (bAdjust) { size.cx -= 1; size.cy -= 1; }
// The cx/cy cross-assignment is what the instructions do -- `mov %esi,0x4(%rdi)`
// at 0x1ab28d stores the SM_CXSIZEFRAME result into .cy -- and it is
// reproduced verbatim rather than tidied up.
// ?GetSystemBorders@CMFCVisualManagerOffice2007@@... is COMDAT-folded onto this
// body (in mfc140u both names resolve to the single RVA 0x1acc60) and carries
// the identical implementation in CMFCVisualManagerOffice2007.cpp.
extern "C" CSize* MS_ABI impl__GetSystemBorders_CMFCVisualManagerWindows7__IEBA_AVCSize__H_Z(
    const CMFCVisualManagerWindows7* /*pThis*/, CSize* pRet, int bAdjust)
{
    if (!pRet) return pRet;
    const int cxFrame = ::GetSystemMetrics(SM_CXSIZEFRAME);
    const int cyFrame = ::GetSystemMetrics(SM_CYSIZEFRAME);
    pRet->cy = cxFrame;
    pRet->cx = cyFrame;
    if (bAdjust) {
        pRet->cx -= 1;
        pRet->cy -= 1;
    }
    return pRet;
}

// Symbol: ?IsHighlightWholeMenuItem@CMFCVisualManagerWindows7@@UEAAHXZ
// Retail 0x1bb550, complete:
//     return <predicate>(this) || this->[+0x210] != 0 || this->[+0x98] != NULL;
// (the `je 0x1bb579` at 0x1bb572 falls out with EAX still zero, so a NULL
// [+0x98] is the only way to reach FALSE once the first two terms fail).
// All three terms are unavailable here -- the predicate has no representation
// and neither +0x210 nor +0x98 is modelled -- so the value would be FALSE
// unconditionally, which is indistinguishable from a stub. Left a stub rather
// than dressed up as an implementation.
extern "C" int MS_ABI impl__IsHighlightWholeMenuItem_CMFCVisualManagerWindows7__UEAAHXZ(
    CMFCVisualManagerWindows7* /*pThis*/)
{
    return 0;
}

// Symbol: ?IsOwnerDrawMenuCheck@CMFCVisualManagerWindows7@@UEAAHXZ
// Retail 0x1bb540 is five instructions: `sub $0x28,%rsp; call <predicate>;
// xor %eax,%eax; add $0x28,%rsp; ret`. The predicate is still called (it is
// not inlinable and has the AFX_GLOBAL_DATA::Initialize side effect) but its
// result is never tested, so FALSE is the complete, unconditional retail
// answer for every object state. Why the source looks that way is not visible
// in the object code and is not asserted here.
// (Identical in shape to ?IsOwnerDrawMenuCheck@CMFCVisualManagerOffice2007@@
// at 0x1a57f0.)
extern "C" int MS_ABI impl__IsOwnerDrawMenuCheck_CMFCVisualManagerWindows7__UEAAHXZ(
    CMFCVisualManagerWindows7* /*pThis*/)
{
    return FALSE;
}

// Symbol: ?IsRibbonPresent@CMFCVisualManagerWindows7@@QEBAHPEAVCWnd@@@Z
// Retail 0x1a1740, complete:
//     pBar = GetRibbonBar(pWnd);              // direct call to 0x1a16b0
//     return pBar != NULL && ::IsWindowVisible(pBar->m_hWnd /*+0x40*/);
// (the import slot at 0x2c5350 resolves to USER32!IsWindowVisible.)
// GetRibbonBar cannot be implemented here (see its comment), so the result
// would be FALSE unconditionally -- indistinguishable from a stub, so it is
// left one.
extern "C" int MS_ABI impl__IsRibbonPresent_CMFCVisualManagerWindows7__QEBAHPEAVCWnd___Z(
    const CMFCVisualManagerWindows7* /*pThis*/, CWnd* /*pWnd*/)
{
    return 0;
}

// Symbol: ?IsWindowActive@CMFCVisualManagerWindows7@@IEBAHPEAVCWnd@@@Z
// Retail 0x1c2bb0, transcribed completely:
//     if (pWnd == NULL) return FALSE;
//     if (pWnd->m_hWnd (+0x40) == NULL) return FALSE;
//     entry = <hash-map assoc lookup, 0x1c07c>(this + 0x9258, hWnd, &tmp, &tmp);
//     return entry ? entry->[+8] : TRUE;
// +0x9258 is a CMap-shaped hash table keyed by HWND (its element type is not
// named in the object code; the layout is CMap's: table pointer at [+0x8],
// bucket count at [+0x10], and each association holds key at +0, value at
// +0x8, next at +0x10, hash at +0x18). 0x1c07c is the shared bucket-lookup
// helper: it hashes the key with an ldiv-based multiply (constants 0x1f31d /
// 0x41a7 / 0xb14), takes it modulo the bucket count and walks the chain
// comparing hash then key. Retail populates that map from
// ?OnNcActivate@CMFCVisualManagerWindows7@@... (0x1c2bf0 touches +0x9258 at
// 0x1c2ce8). OpenMFC models neither the map nor a working OnNcActivate
// override (this file's is a documented stub), so the lookup can never
// hit and the retail answer reduces here to the two NULL guards followed by
// TRUE. The lookup branch is a deliberate omission, not an oversight.
extern "C" int MS_ABI impl__IsWindowActive_CMFCVisualManagerWindows7__IEBAHPEAVCWnd___Z(
    const CMFCVisualManagerWindows7* /*pThis*/, CWnd* pWnd)
{
    if (pWnd == nullptr) return FALSE;
    if (pWnd->m_hWnd == nullptr) return FALSE;
    return TRUE;
}

// Symbol: ?MakeResourceID@CMFCVisualManagerWindows7@@KA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@PEB_W@Z
// Retail entry 0x1bcd60 (mfc140u); the ANSI twin at mfc140 0x1bb360 is the
// same body with the `D` CStringT and is where the callees below were named:
//     CString str(lpszBase);                       // ctor at 0xdd70
//     if (g_strStylePrefix /*0x3bbff0*/ .GetLength() != 0)
//         str = g_strStylePrefix + str;            // operator+ at 0x793e0,
//                                                  // operator= at 0xdee0
//     return str;                                  // buffer echoed in RAX
// g_strStylePrefix is the same .bss CString that CleanStyle empties, and
// nothing in OpenMFC ever fills it (SetStyle loads no resource module), so
// only the empty-prefix path is reachable and the result is lpszBase itself.
// This also repairs the ABI: the generated stub returned nullptr rather than
// the caller's return buffer.
extern "C" CString* MS_ABI impl__MakeResourceID_CMFCVisualManagerWindows7__KA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__PEB_W_Z(
    CString* pRet, const wchar_t* lpszBase)
{
    if (!pRet) return pRet;
    new (pRet) CString();
    if (lpszBase != nullptr) *pRet = lpszBase;
    return pRet;
}

// Symbol: ?OnDrawCheckBoxEx@CMFCVisualManagerWindows7@@UEAAXPEAVCDC@@VCRect@@HHHH@Z
// Retail 0x1c1340. No predicate: it branches on this->[+0x20] (a second theme
// handle, sibling of the one DrawNcBtn uses). When it is NULL it calls
// 0x1886a0 = ?OnDrawCheckBoxEx@CMFCVisualManager@@... with the same six
// arguments; when it is non-NULL it calls the virtual at vtable +0x50 on
// `this` -- ?DrawCheckBox@CMFCBaseVisualManager@@UEAAHPEAVCDC@@VCRect@@HHHH@Z
// (mfc140 0x18e620, read from the Windows7 vftable at 0x31cdd0) -- with the
// last four arguments permuted (0x1c1388..0x1c13aa): that callee gets
// (pDC, &rect, bHighlighted, nState, bEnabled, bPressed) where this function
// received (pDC, rect, nState, bHighlighted, bPressed, bEnabled).
// +0x20 has no representation here, so the base call is the reachable path
// and is what this reproduces.
extern "C" void MS_ABI impl__OnDrawCheckBoxEx_CMFCVisualManagerWindows7__UEAAXPEAVCDC__VCRect__HHHH_Z(
    CMFCVisualManagerWindows7* pThis, CDC* pDC, CRect rect, int nState,
    int bHighlighted, int bPressed, int bEnabled)
{
    if (!pThis) return;
    pThis->CMFCVisualManager::OnDrawCheckBoxEx(pDC, rect, nState, bHighlighted, bPressed, bEnabled);
}

// Symbol: ?OnDrawDefaultRibbonImage@CMFCVisualManagerWindows7@@UEAAXPEAVCDC@@VCRect@@HHH@Z
// Retail 0x1c1190. Predicate FALSE (0x1c11db) calls 0x18a340 =
// ?OnDrawDefaultRibbonImage@CMFCVisualManager@@... with the same five
// arguments; predicate TRUE draws the themed placeholder image out of the
// CMFCToolBarImages at this->[+0x4b70] via
// ?DrawEx@CMFCToolBarImages@@QEAAH... (0x16b6d0). Only the FALSE branch is
// reachable here.
extern "C" void MS_ABI impl__OnDrawDefaultRibbonImage_CMFCVisualManagerWindows7__UEAAXPEAVCDC__VCRect__HHH_Z(
    CMFCVisualManagerWindows7* pThis, CDC* pDC, CRect rectImage,
    int bIsDisabled, int bIsPressed, int bIsHighlighted)
{
    if (!pThis) return;
    impl__OnDrawDefaultRibbonImage_CMFCVisualManager__UEAAXPEAVCDC__VCRect__HHH_Z(
        pThis, pDC, rectImage, bIsDisabled, bIsPressed, bIsHighlighted);
}

// Symbol: ?OnDrawMenuCheck@CMFCVisualManagerWindows7@@UEAAXPEAVCDC@@PEAVCMFCToolBarMenuButton@@VCRect@@HH@Z
// Retail 0x1bf230. Two guards fall through to 0x1834e0 =
// ?OnDrawMenuCheck@CMFCVisualManager@@... with the same five arguments: the
// predicate being FALSE, or the CMFCToolBarImages selected by bIsRadio
// (this->[+0x698] for the check glyph, this->[+0x830] for the radio glyph --
// the `and $0x198` / `add $0x698` pair at 0x1bf26b) having a zero [+0x8].
// Only past both does retail mirror the image for RTL
// (?Mirror@CMFCToolBarImages@@, 0x16ee30) and blit it with DrawEx (0x16b6d0).
// The predicate is FALSE here, so the base call is the reachable path.
extern "C" void MS_ABI impl__OnDrawMenuCheck_CMFCVisualManagerWindows7__UEAAXPEAVCDC__PEAVCMFCToolBarMenuButton__VCRect__HH_Z(
    CMFCVisualManagerWindows7* pThis, CDC* pDC, CMFCToolBarMenuButton* pButton,
    CRect rect, int bHighlight, int bIsRadio)
{
    if (!pThis) return;
    pThis->CMFCVisualManager::OnDrawMenuCheck(pDC, pButton, rect, bHighlight, bIsRadio);
}

// Symbol: ?OnDrawMenuLabel@CMFCVisualManagerWindows7@@UEAAKPEAVCDC@@VCRect@@@Z
// Retail 0x1c2a50. Predicate FALSE (0x1c2a8d) calls 0x1b2e20 =
// ?OnDrawMenuLabel@CMFCVisualManagerOfficeXP@@MEAAK... with the same three
// arguments; predicate TRUE fills the label with the brush at this->[+0x250]
// (USER32!FillRect, slot 0x2c5230), draws a two-pixel-shorter frame through a
// virtual at vtable +0x640 using the brushes at [+0x228]/[+0x238], and returns
// the COLORREF at afxGlobalData+0x74.
// Only the FALSE branch is reachable. OpenMFC has no CMFCVisualManagerOfficeXP
// ancestor, so retail's callee cannot be reached as a base call. This tree
// does carry a working impl__OnDrawMenuLabel_CMFCVisualManagerOfficeXP__ body,
// but reaching it would mean passing a CMFCVisualManagerWindows7* as that
// class's `this`, so the call is routed to CMFCVisualManager's implementation
// instead -- a deliberate substitution, not a transcription.
extern "C" unsigned long MS_ABI impl__OnDrawMenuLabel_CMFCVisualManagerWindows7__UEAAKPEAVCDC__VCRect___Z(
    CMFCVisualManagerWindows7* pThis, CDC* pDC, CRect rect)
{
    if (!pThis) return 0;
    return impl__OnDrawMenuLabel_CMFCVisualManager__UEAAKPEAVCDC__VCRect___Z(pThis, pDC, rect);
}

// Symbol: ?OnDrawRadioButton@CMFCVisualManagerWindows7@@UEAAXPEAVCDC@@VCRect@@HHHH@Z
// Retail 0x1c13c0 is a single unconditional forwarding call -- no predicate,
// no NULL test, no theme-handle test. It copies the rect to the stack and
// calls the virtual at vtable +0x58 on `this`, which the Windows7 vftable at
// 0x31cdd0 resolves to ?DrawRadioButton@CMFCBaseVisualManager@@UEAAHPEAVCDC@@VCRect@@HHHH@Z
// (mfc140 0x18e6f0; Windows7 does not override it). The trailing arguments
// are permuted: naming this function's parameters (pDC, rect, a, b, c, d)
// with `a` the one that arrives in R9D, the callee receives
// (pDC, &rectCopy, b, a, d, c) -- read at 0x1c13cb..0x1c13fa. The callee's
// BOOL result is discarded.
// Reproduced as that one call, through the thunk CMFCBaseVisualManager.cpp
// defines. That thunk is today retail's NULL-theme terminal (it returns FALSE
// without painting, because the theme handle at CMFCBaseVisualManager+0x20 is
// not modelled), so nothing is drawn yet; the dispatch is nonetheless the
// retail one and will paint as soon as that body does.
extern "C" void MS_ABI impl__OnDrawRadioButton_CMFCVisualManagerWindows7__UEAAXPEAVCDC__VCRect__HHHH_Z(
    CMFCVisualManagerWindows7* pThis, CDC* pDC, CRect rect,
    int bOn, int bHighlighted, int bPressed, int bEnabled)
{
    if (!pThis) return;
    impl__DrawRadioButton_CMFCBaseVisualManager__UEAAHPEAVCDC__VCRect__HHHH_Z(
        pThis, pDC, rect, bHighlighted, bOn, bEnabled, bPressed);
}

// Symbol: ?OnDrawRibbonApplicationButton@CMFCVisualManagerWindows7@@UEAAXPEAVCDC@@PEAVCMFCRibbonButton@@@Z
// Retail 0x1c1240. Predicate FALSE (0x1c1268) calls 0x18a530 =
// ?OnDrawRibbonApplicationButton@CMFCVisualManager@@... with the same three
// arguments; predicate TRUE asks the button four state virtuals
// (vtable +0x1b0, +0x1b8, +0x1c0, +0x1d8), folds them into a renderer state
// index and draws through the CMFCControlRenderer at this->[+0x4570].
// Only the FALSE branch is reachable here.
extern "C" void MS_ABI impl__OnDrawRibbonApplicationButton_CMFCVisualManagerWindows7__UEAAXPEAVCDC__PEAVCMFCRibbonButton___Z(
    CMFCVisualManagerWindows7* pThis, CDC* pDC, CMFCRibbonButton* pButton)
{
    if (!pThis) return;
    pThis->CMFCVisualManager::OnDrawRibbonApplicationButton(pDC, pButton);
}

// Symbol: ?OnDrawRibbonButtonBorder@CMFCVisualManagerWindows7@@UEAAXPEAVCDC@@PEAVCMFCRibbonButton@@@Z
// Retail 0x1c1410. Predicate FALSE (0x1c1434) calls 0x1b21d0 =
// ?OnDrawRibbonButtonBorder@CMFCVisualManagerOfficeXP@@MEAAX... with the same
// three arguments. Predicate TRUE returns immediately unless the button
// IsKindOf the CRuntimeClass at 0x302a10 -- whose m_lpszClassName reads
// "CMFCRibbonEdit" -- and only then picks one of this->[+0x280..+0x28c] and
// frames the edit with ?Draw3dRect@CDC@@ (0x2a3b00) or a CDrawingManager.
// Only the FALSE branch is reachable. Routed to CMFCVisualManager's
// implementation because OpenMFC has no CMFCVisualManagerOfficeXP ancestor and
// this tree's OfficeXP override is an empty stub -- a deliberate substitution.
extern "C" void MS_ABI impl__OnDrawRibbonButtonBorder_CMFCVisualManagerWindows7__UEAAXPEAVCDC__PEAVCMFCRibbonButton___Z(
    CMFCVisualManagerWindows7* pThis, CDC* pDC, CMFCRibbonButton* pButton)
{
    if (!pThis) return;
    pThis->CMFCVisualManager::OnDrawRibbonButtonBorder(pDC, pButton);
}

// Symbol: ?OnDrawRibbonCaption@CMFCVisualManagerWindows7@@UEAAXPEAVCDC@@PEAVCMFCRibbonBar@@VCRect@@2@Z
// Retail 0x1bf5a0. Predicate FALSE (0x1bf60e) calls 0x189f00 =
// ?OnDrawRibbonCaption@CMFCVisualManager@@... with the same five arguments in
// the same order (rectCaption stays the 4th, rectText the 5th). Predicate TRUE
// runs the Aero caption path -- IsWindowActive, the caption CMFCControlRenderer
// sub-objects, and DrawNcText for the title.
// Only the FALSE branch is reachable here.
extern "C" void MS_ABI impl__OnDrawRibbonCaption_CMFCVisualManagerWindows7__UEAAXPEAVCDC__PEAVCMFCRibbonBar__VCRect__2_Z(
    CMFCVisualManagerWindows7* pThis, CDC* pDC, CMFCRibbonBar* pBar, CRect rectCaption, CRect rectText)
{
    if (!pThis) return;
    pThis->CMFCVisualManager::OnDrawRibbonCaption(pDC, pBar, rectCaption, rectText);
}

// Symbol: ?OnDrawRibbonCaptionButton@CMFCVisualManagerWindows7@@UEAAXPEAVCDC@@PEAVCMFCRibbonCaptionButton@@@Z
// Retail 0x1bfa70 has no predicate and no base-class branch at all. It builds
// a button state from three virtuals on the caption button (vtable +0x1b0,
// +0x1b8, +0x1c0), works out whether the owning window is active -- when
// pButton->[+0x270] is NULL it goes through pButton->[+0xd8]->m_hWnd,
// USER32!GetParent (slot 0x2c5300), ?FromHandle@CWnd@@ (0x289180) and
// IsWindowActive (0x1c2bb0) -- inflates a copy of the button rect at
// pButton->[+0xc8] by -1/-1 when the manager's [+0x8] theme handle is NULL
// and by -2/-2 when it is not (`neg %rcx; sbb %edx,%edx; dec %edx` at
// 0x1bfb3c..0x1bfb46, USER32!InflateRect slot 0x2c5310), and finally calls the virtual at vtable +0x648 on `this`, which the argument
// layout identifies as DrawNcBtn (pDC, rect, pButton->[+0x118], state, 0,
// bActive, bMaximized).
// Every input except pDC comes from CMFCRibbonCaptionButton members OpenMFC
// does not declare, so there is no reachable branch to reproduce. Left a stub.
extern "C" void MS_ABI impl__OnDrawRibbonCaptionButton_CMFCVisualManagerWindows7__UEAAXPEAVCDC__PEAVCMFCRibbonCaptionButton___Z(
    CMFCVisualManagerWindows7* /*pThis*/, CDC* /*pDC*/, CMFCRibbonCaptionButton* /*pButton*/) {}

// Symbol: ?OnDrawRibbonCategory@CMFCVisualManagerWindows7@@UEAAXPEAVCDC@@PEAVCMFCRibbonCategory@@VCRect@@@Z
// Retail 0x1bff60. Predicate FALSE (0x1bff9c) calls 0x18a780 =
// ?OnDrawRibbonCategory@CMFCVisualManager@@... with the same four arguments;
// predicate TRUE paints the category background from the CMFCControlRenderer
// at this->[+0xfd8] through the bitmap cache at this->[+0x8de0]
// (?CacheY@CMFCVisualManagerBitmapCache@@ 0x197ac0, ?FindIndex@... 0x197b10,
// ?DrawY@CMFCVisualManagerBitmapCacheItem@... 0x1976e0), or from the
// 0x668-strided pair at this->[+0x6308] / [+0x6708] when the category carries
// a context colour (0x1bffd5..0x1bfff3).
// Only the FALSE branch is reachable here.
extern "C" void MS_ABI impl__OnDrawRibbonCategory_CMFCVisualManagerWindows7__UEAAXPEAVCDC__PEAVCMFCRibbonCategory__VCRect___Z(
    CMFCVisualManagerWindows7* pThis, CDC* pDC, CMFCRibbonCategory* pCategory, CRect rectCategory)
{
    if (!pThis) return;
    pThis->CMFCVisualManager::OnDrawRibbonCategory(pDC, pCategory, rectCategory);
}

// Symbol: ?OnDrawRibbonCategoryCaption@CMFCVisualManagerWindows7@@UEAAKPEAVCDC@@PEAVCMFCRibbonContextCaption@@@Z
// Retail 0x1c0aa0. Two guards reach 0x18c390 =
// ?OnDrawRibbonCategoryCaption@CMFCVisualManager@@... with the same three
// arguments: the predicate being FALSE, or pCaption->[+0x270] being zero.
// Only past both does retail index a 0x668-byte-strided renderer array at
// this->[+0x5aa0] by that colour index, paint the caption and -- only when
// pCaption->[+0x278] is greater than zero -- a second strip through the
// renderer at this->[+0x5f08], and return the COLORREF at
// this->[+0x60f8 + stride].
// The predicate is FALSE here, so the base call is the reachable path.
extern "C" unsigned long MS_ABI impl__OnDrawRibbonCategoryCaption_CMFCVisualManagerWindows7__UEAAKPEAVCDC__PEAVCMFCRibbonContextCaption___Z(
    CMFCVisualManagerWindows7* pThis, CDC* pDC, CMFCRibbonContextCaption* pCaption)
{
    if (!pThis) return 0;
    return (unsigned long)pThis->CMFCVisualManager::OnDrawRibbonCategoryCaption(pDC, pCaption);
}

// Symbol: ?OnDrawRibbonCategoryScroll@CMFCVisualManagerWindows7@@UEAAXPEAVCDC@@PEAVCRibbonCategoryScroll@@@Z
// Retail 0x1c0150. Predicate FALSE (0x1c017f) calls 0x1b24d0 =
// ?OnDrawRibbonCategoryScroll@CMFCVisualManagerOfficeXP@@MEAAX... with the
// same three arguments; predicate TRUE draws the scroll body from the
// CMFCControlRenderer at this->[+0x17d8], or this->[+0x15d8] when
// pScroll->[+0x270] is non-zero (the `and $0xfffffe00` / `add $0x17d8` pair at
// 0x1c019b), and then stamps the arrow with ?Draw@CMenuImages@@ (0x8fd40).
// Only the FALSE branch is reachable. Routed to CMFCVisualManager's
// implementation because OpenMFC has no CMFCVisualManagerOfficeXP ancestor and
// this tree's OfficeXP override is an empty stub -- a deliberate substitution.
extern "C" void MS_ABI impl__OnDrawRibbonCategoryScroll_CMFCVisualManagerWindows7__UEAAXPEAVCDC__PEAVCRibbonCategoryScroll___Z(
    CMFCVisualManagerWindows7* pThis, CDC* pDC, CMFCRibbonCategoryScroll* pScroll)
{
    if (!pThis) return;
    pThis->CMFCVisualManager::OnDrawRibbonCategoryScroll(pDC, pScroll);
}

// Symbol: ?OnDrawRibbonCategoryTab@CMFCVisualManagerWindows7@@UEAAKPEAVCDC@@PEAVCMFCRibbonTab@@H@Z
// Retail 0x1c02a0. Predicate FALSE (0x1c02d6) calls 0x18ab00 =
// ?OnDrawRibbonCategoryTab@CMFCVisualManager@@... with the same four
// arguments; predicate TRUE walks pTab->[+0xe0]->[+0x760] to reach the owning
// ribbon bar's state, selects one of the tab renderers and returns a themed
// text colour. Only the FALSE branch is reachable here.
extern "C" unsigned long MS_ABI impl__OnDrawRibbonCategoryTab_CMFCVisualManagerWindows7__UEAAKPEAVCDC__PEAVCMFCRibbonTab__H_Z(
    CMFCVisualManagerWindows7* pThis, CDC* pDC, CMFCRibbonTab* pTab, int bIsActive)
{
    if (!pThis) return 0;
    return (unsigned long)pThis->CMFCVisualManager::OnDrawRibbonCategoryTab(pDC, pTab, bIsActive);
}

// Symbol: ?OnDrawRibbonDefaultPaneButton@CMFCVisualManagerWindows7@@UEAAXPEAVCDC@@PEAVCMFCRibbonButton@@@Z
// Retail 0x1c2650. Predicate FALSE (0x1c2681) calls 0x18b3e0 =
// ?OnDrawRibbonDefaultPaneButton@CMFCVisualManager@@... with the same three
// arguments; predicate TRUE first calls the virtual at vtable +0x480 on
// `this` (this class's own ?OnFillRibbonButton@CMFCVisualManagerWindows7@@,
// 0x1c17d0, per the vftable at 0x31cdd0) and then renders the button rect
// (pButton->[+0xc8]) with the themed renderers.
// Only the FALSE branch is reachable here.
extern "C" void MS_ABI impl__OnDrawRibbonDefaultPaneButton_CMFCVisualManagerWindows7__UEAAXPEAVCDC__PEAVCMFCRibbonButton___Z(
    CMFCVisualManagerWindows7* pThis, CDC* pDC, CMFCRibbonButton* pButton)
{
    if (!pThis) return;
    pThis->CMFCVisualManager::OnDrawRibbonDefaultPaneButton(pDC, pButton);
}

// Symbol: ?OnDrawRibbonGalleryButton@CMFCVisualManagerWindows7@@UEAAXPEAVCDC@@PEAVCMFCRibbonGalleryIcon@@@Z
// Retail 0x1c0990. Predicate FALSE (0x1c09b6) calls 0x18c190 =
// ?OnDrawRibbonGalleryButton@CMFCVisualManager@@... with the same three
// arguments; predicate TRUE reads the icon's state virtual (vtable +0x1c8) and
// paints the themed gallery frame. Only the FALSE branch is reachable here.
extern "C" void MS_ABI impl__OnDrawRibbonGalleryButton_CMFCVisualManagerWindows7__UEAAXPEAVCDC__PEAVCMFCRibbonGalleryIcon___Z(
    CMFCVisualManagerWindows7* pThis, CDC* pDC, void* pButton)
{
    if (!pThis) return;
    impl__OnDrawRibbonGalleryButton_CMFCVisualManager__UEAAXPEAVCDC__PEAVCMFCRibbonGalleryIcon___Z(
        pThis, pDC, pButton);
}

// Symbol: ?OnDrawRibbonLaunchButton@CMFCVisualManagerWindows7@@UEAAXPEAVCDC@@PEAVCMFCRibbonLaunchButton@@PEAVCMFCRibbonPanel@@@Z
// Retail 0x1c1550. Predicate FALSE (0x1c1585) calls 0x18b2f0 =
// ?OnDrawRibbonLaunchButton@CMFCVisualManager@@... with the same four
// arguments; predicate TRUE draws the themed launch glyph from the button rect
// at pButton->[+0xc8]. Only the FALSE branch is reachable here.
extern "C" void MS_ABI impl__OnDrawRibbonLaunchButton_CMFCVisualManagerWindows7__UEAAXPEAVCDC__PEAVCMFCRibbonLaunchButton__PEAVCMFCRibbonPanel___Z(
    CMFCVisualManagerWindows7* pThis, CDC* pDC, void* pButton, CMFCRibbonPanel* pPanel)
{
    if (!pThis) return;
    impl__OnDrawRibbonLaunchButton_CMFCVisualManager__UEAAXPEAVCDC__PEAVCMFCRibbonLaunchButton__PEAVCMFCRibbonPanel___Z(
        pThis, pDC, pButton, pPanel);
}

// Symbol: ?OnDrawRibbonMenuCheckFrame@CMFCVisualManagerWindows7@@UEAAXPEAVCDC@@PEAVCMFCRibbonButton@@VCRect@@@Z
// Retail 0x1c2460. Predicate FALSE (0x1c2495) calls 0x1b2650 =
// ?OnDrawRibbonMenuCheckFrame@CMFCVisualManagerOfficeXP@@MEAAX... with the
// same four arguments; predicate TRUE draws the frame with the
// CMFCControlRenderer at this->[+0x498] (virtual +0x30, alpha 0xff).
// Only the FALSE branch is reachable. OpenMFC has no CMFCVisualManagerOfficeXP
// ancestor, so retail's callee is not reachable as a base call; this tree does
// carry a working impl__OnDrawRibbonMenuCheckFrame_CMFCVisualManagerOfficeXP__
// body, but reaching it would mean passing a CMFCVisualManagerWindows7* as
// that class's `this`. Routed to CMFCVisualManager's thunk instead -- a
// deliberate substitution.
extern "C" void MS_ABI impl__OnDrawRibbonMenuCheckFrame_CMFCVisualManagerWindows7__UEAAXPEAVCDC__PEAVCMFCRibbonButton__VCRect___Z(
    CMFCVisualManagerWindows7* pThis, CDC* pDC, CMFCRibbonButton* pButton, CRect rect)
{
    if (!pThis) return;
    impl__OnDrawRibbonMenuCheckFrame_CMFCVisualManager__UEAAXPEAVCDC__PEAVCMFCRibbonButton__VCRect___Z(
        pThis, pDC, pButton, rect);
}

// Symbol: ?OnDrawRibbonPanel@CMFCVisualManagerWindows7@@UEAAKPEAVCDC@@PEAVCMFCRibbonPanel@@VCRect@@2@Z
// Retail 0x1bfbb0. Predicate FALSE (0x1bfc09) calls 0x18af70 =
// ?OnDrawRibbonPanel@CMFCVisualManager@@... with the same five arguments in
// the same order (rectPanel 4th, rectCaption 5th). Predicate TRUE first tests
// the panel against the CRuntimeClass at 0x305378 -- m_lpszClassName
// "CMFCRibbonMainPanel" -- and then paints either the main-panel frame or the
// ordinary panel body from the renderer at this->[+0x1dd8] plus the bitmap
// cache. Only the FALSE branch is reachable here.
extern "C" unsigned long MS_ABI impl__OnDrawRibbonPanel_CMFCVisualManagerWindows7__UEAAKPEAVCDC__PEAVCMFCRibbonPanel__VCRect__2_Z(
    CMFCVisualManagerWindows7* pThis, CDC* pDC, CMFCRibbonPanel* pPanel, CRect rectPanel, CRect rectCaption)
{
    if (!pThis) return 0;
    return (unsigned long)pThis->CMFCVisualManager::OnDrawRibbonPanel(pDC, pPanel, rectPanel, rectCaption);
}

// Symbol: ?OnDrawRibbonPanelCaption@CMFCVisualManagerWindows7@@UEAAXPEAVCDC@@PEAVCMFCRibbonPanel@@VCRect@@@Z
// Retail 0x1bfe00. Predicate FALSE (0x1bfe37) calls 0x18b140 =
// ?OnDrawRibbonPanelCaption@CMFCVisualManager@@... with the same four
// arguments; predicate TRUE returns early unless the panel IsKindOf the
// CRuntimeClass at 0x305378 ("CMFCRibbonMainPanel") and otherwise paints the
// themed caption. Only the FALSE branch is reachable here.
extern "C" void MS_ABI impl__OnDrawRibbonPanelCaption_CMFCVisualManagerWindows7__UEAAXPEAVCDC__PEAVCMFCRibbonPanel__VCRect___Z(
    CMFCVisualManagerWindows7* pThis, CDC* pDC, CMFCRibbonPanel* pPanel, CRect rectCaption)
{
    if (!pThis) return;
    pThis->CMFCVisualManager::OnDrawRibbonPanelCaption(pDC, pPanel, rectCaption);
}

// Symbol: ?OnDrawRibbonRecentFilesFrame@CMFCVisualManagerWindows7@@UEAAXPEAVCDC@@PEAVCMFCRibbonMainPanel@@VCRect@@@Z
// Retail 0x1c2830. Predicate FALSE (0x1c2868) calls 0x1b26e0 =
// ?OnDrawRibbonRecentFilesFrame@CMFCVisualManagerOfficeXP@@MEAAX... with the
// same four arguments; predicate TRUE first widens its copy of the rect by
// two pixels on the RIGHT (`addl $0x2,0x8(%rbx)` at 0x1c2872 -- +0x8 is
// RECT.right), fills it with USER32!FillRect (slot 0x2c5230) using the
// m_hObject (+0x8) of the CGdiObject at this->[+0x160], and then frames it.
// Only the FALSE branch is reachable. OpenMFC has no CMFCVisualManagerOfficeXP
// ancestor, so retail's callee is not reachable as a base call; this tree does
// carry a working impl__OnDrawRibbonRecentFilesFrame_CMFCVisualManagerOfficeXP__
// body, but reaching it would mean passing a CMFCVisualManagerWindows7* as
// that class's `this`. Routed to CMFCVisualManager's thunk instead -- a
// deliberate substitution.
extern "C" void MS_ABI impl__OnDrawRibbonRecentFilesFrame_CMFCVisualManagerWindows7__UEAAXPEAVCDC__PEAVCMFCRibbonMainPanel__VCRect___Z(
    CMFCVisualManagerWindows7* pThis, CDC* pDC, CMFCRibbonMainPanel* pPanel, CRect rect)
{
    if (!pThis) return;
    impl__OnDrawRibbonRecentFilesFrame_CMFCVisualManager__UEAAXPEAVCDC__PEAVCMFCRibbonMainPanel__VCRect___Z(
        pThis, pDC, pPanel, rect);
}

// Symbol: ?OnDrawRibbonSliderChannel@CMFCVisualManagerWindows7@@UEAAXPEAVCDC@@PEAVCMFCRibbonSlider@@VCRect@@@Z
// Retail 0x1c0e90. No predicate: it branches on this->[+0x90], a theme handle
// (it is the first argument of UxTheme!DrawThemeBackground, slot 0x2c53e0).
// When NULL it calls 0x18c880 = ?OnDrawRibbonSliderChannel@CMFCVisualManager@@
// with the same four arguments. When non-NULL it adds one pixel to
// rect.bottom if the height is under 3 (`cmp $0x3` / `lea 0x1(%rcx)` at
// 0x1c0ec0) and calls
// ::DrawThemeBackground(this->[+0x90], pDC ? pDC->m_hDC : NULL, 1, 1, &rect,
// NULL) -- both the part id and the state id are 1 (`mov $0x1,%r8d` then
// `mov %r8d,%r9d`). OpenMFC has no theme-handle member, so the base call is
// the reachable path and is what this reproduces.
extern "C" void MS_ABI impl__OnDrawRibbonSliderChannel_CMFCVisualManagerWindows7__UEAAXPEAVCDC__PEAVCMFCRibbonSlider__VCRect___Z(
    CMFCVisualManagerWindows7* pThis, CDC* pDC, CMFCRibbonSlider* pSlider, CRect rect)
{
    if (!pThis) return;
    pThis->CMFCVisualManager::OnDrawRibbonSliderChannel(pDC, pSlider, rect);
}

// Symbol: ?OnDrawRibbonSliderThumb@CMFCVisualManagerWindows7@@UEAAXPEAVCDC@@PEAVCMFCRibbonSlider@@VCRect@@HHH@Z
// Retail 0x1c0f00, the same shape as OnDrawRibbonSliderChannel: when
// this->[+0x90] is NULL it calls 0x18c960 =
// ?OnDrawRibbonSliderThumb@CMFCVisualManager@@... with the same six arguments,
// otherwise it calls ::DrawThemeBackground(this->[+0x90], pDC->m_hDC, 4,
// state, &rect, NULL) with
// state = bIsDisabled ? 5 : bIsPressed ? 3 : bIsHighlighted ? 2 : 1
// (0x1c0f4a..0x1c0f7d). The theme handle has no representation here, so the
// base call is the reachable path.
extern "C" void MS_ABI impl__OnDrawRibbonSliderThumb_CMFCVisualManagerWindows7__UEAAXPEAVCDC__PEAVCMFCRibbonSlider__VCRect__HHH_Z(
    CMFCVisualManagerWindows7* pThis, CDC* pDC, CMFCRibbonSlider* pSlider, CRect rect,
    int bIsHighlighted, int bIsPressed, int bIsDisabled)
{
    if (!pThis) return;
    pThis->CMFCVisualManager::OnDrawRibbonSliderThumb(pDC, pSlider, rect, bIsHighlighted, bIsPressed, bIsDisabled);
}

// Symbol: ?OnDrawRibbonSliderZoomButton@CMFCVisualManagerWindows7@@UEAAXPEAVCDC@@PEAVCMFCRibbonSlider@@VCRect@@HHHH@Z
// Retail 0x1c0cf0. Predicate FALSE (0x1c0d58) calls 0x18c6b0 =
// ?OnDrawRibbonSliderZoomButton@CMFCVisualManager@@... with the same seven
// arguments; predicate TRUE selects the zoom-in or zoom-out renderer
// (this->[+0x4770], plus 0x200 when bIsZoomOut is non-zero -- the
// `and $0x200` at 0x1c0d7a) and a state index built from the three remaining
// BOOLs. Only the FALSE branch is reachable here.
extern "C" void MS_ABI impl__OnDrawRibbonSliderZoomButton_CMFCVisualManagerWindows7__UEAAXPEAVCDC__PEAVCMFCRibbonSlider__VCRect__HHHH_Z(
    CMFCVisualManagerWindows7* pThis, CDC* pDC, CMFCRibbonSlider* pSlider, CRect rect,
    int bIsZoomOut, int bIsHighlighted, int bIsPressed, int bIsDisabled)
{
    if (!pThis) return;
    pThis->CMFCVisualManager::OnDrawRibbonSliderZoomButton(pDC, pSlider, rect, bIsZoomOut,
                                                           bIsHighlighted, bIsPressed, bIsDisabled);
}

// Symbol: ?OnDrawRibbonStatusBarPane@CMFCVisualManagerWindows7@@UEAAKPEAVCDC@@PEAVCMFCRibbonStatusBar@@PEAVCMFCRibbonStatusBarPane@@@Z
// Retail 0x1c0bb0. Predicate FALSE (0x1c0bf0) calls 0x1b2790 =
// ?OnDrawRibbonStatusBarPane@CMFCVisualManagerOfficeXP@@MEAAK... with the same
// four arguments; predicate TRUE asks the pane's state virtual (vtable +0x1b0),
// paints the pane rect (pPane->[+0xc8]) from a themed renderer and returns a
// themed text colour.
// Only the FALSE branch is reachable. Routed to CMFCVisualManager's
// implementation because OpenMFC has no CMFCVisualManagerOfficeXP ancestor and
// this tree's OfficeXP override is an empty stub -- a deliberate substitution.
extern "C" unsigned long MS_ABI impl__OnDrawRibbonStatusBarPane_CMFCVisualManagerWindows7__UEAAKPEAVCDC__PEAVCMFCRibbonStatusBar__PEAVCMFCRibbonStatusBarPane___Z(
    CMFCVisualManagerWindows7* pThis, CDC* pDC, CMFCRibbonStatusBar* pBar, CMFCRibbonStatusBarPane* pPane)
{
    if (!pThis) return 0;
    return (unsigned long)pThis->CMFCVisualManager::OnDrawRibbonStatusBarPane(pDC, pBar, pPane);
}

// Symbol: ?OnDrawRibbonTabsFrame@CMFCVisualManagerWindows7@@UEAAKPEAVCDC@@PEAVCMFCRibbonBar@@VCRect@@@Z
// This name is absent from both RVA-map JSON files (they keep one name per
// RVA), but it is a real export -- ordinal 9590 -- and the mfc140u export
// table resolves it to RVA 0x88a0, which is two instructions:
//     or   $0xffffffff,%eax
//     ret
// i.e. `return (COLORREF)-1;` -- the linker folded it together with
// ?GetMaxConnections@CConnectionPoint@@UEAAHXZ,
// ?OnDrawRibbonButtonsGroup@CMFCVisualManager@@... and ?OnDropEx@CView@@...,
// which is why the map shows another name there. Retail therefore paints
// nothing for the tabs frame and returns -1 (no colour), and that is what
// this does. It deliberately does NOT delegate to CMFCVisualManager's
// OnDrawRibbonTabsFrame, which would paint a frame retail never draws.
extern "C" unsigned long MS_ABI impl__OnDrawRibbonTabsFrame_CMFCVisualManagerWindows7__UEAAKPEAVCDC__PEAVCMFCRibbonBar__VCRect___Z(
    CMFCVisualManagerWindows7* /*pThis*/, CDC* /*pDC*/, CMFCRibbonBar* /*pBar*/, CRect /*rectTab*/)
{
    return (unsigned long)-1;
}

// Symbol: ?OnDrawStatusBarSizeBox@CMFCVisualManagerWindows7@@UEAAXPEAVCDC@@PEAVCMFCStatusBar@@VCRect@@@Z
// Retail 0x1c0910. Two guards reach 0x1958e0 =
// ?OnDrawStatusBarSizeBox@CMFCVisualManagerWindows@@... with the same four
// arguments: the predicate being FALSE, or this->[+0x28] (a theme handle)
// being NULL. Only past both does retail call
// ::DrawThemeBackground(this->[+0x28], pDC ? pDC->m_hDC : NULL, 3, 0,
// &rectSizeBox, NULL) (0x1c095c).
// Neither guard can fail here, so the base call is the reachable path. Routed
// to CMFCVisualManager's implementation because OpenMFC's Windows7 manager
// derives directly from CMFCVisualManager and this tree's
// CMFCVisualManagerWindows::OnDrawStatusBarSizeBox is an empty generated stub
// -- a deliberate substitution, not a transcription.
extern "C" void MS_ABI impl__OnDrawStatusBarSizeBox_CMFCVisualManagerWindows7__UEAAXPEAVCDC__PEAVCMFCStatusBar__VCRect___Z(
    CMFCVisualManagerWindows7* pThis, CDC* pDC, CMFCStatusBar* pBar, CRect rectSizeBox)
{
    if (!pThis) return;
    pThis->CMFCVisualManager::OnDrawStatusBarSizeBox(pDC, pBar, rectSizeBox);
}

// Symbol: ?OnFillBarBackground@CMFCVisualManagerWindows7@@UEAAXPEAVCDC@@PEAVCBasePane@@VCRect@@2H@Z
// Retail 0x1c07d0. Predicate FALSE (0x1c08fd) calls 0x1b7870 =
// ?OnFillBarBackground@CMFCVisualManagerWindows@@... with the same six
// arguments in the same order; predicate TRUE tests the pane against the
// CRuntimeClass at 0x30b0c0 -- m_lpszClassName "CMFCRibbonStatusBar" -- and
// picks a themed fill accordingly.
// Only the FALSE branch is reachable. Routed to CMFCVisualManager's
// implementation because OpenMFC's Windows7 manager derives directly from
// CMFCVisualManager and this tree's CMFCVisualManagerWindows::OnFillBarBackground
// is an empty generated stub -- a deliberate substitution.
extern "C" void MS_ABI impl__OnFillBarBackground_CMFCVisualManagerWindows7__UEAAXPEAVCDC__PEAVCBasePane__VCRect__2H_Z(
    CMFCVisualManagerWindows7* pThis, CDC* pDC, CBasePane* pBar, CRect rectClient, CRect rectClip, int bNCArea)
{
    if (!pThis) return;
    pThis->CMFCVisualManager::OnFillBarBackground(pDC, pBar, rectClient, rectClip, bNCArea);
}

// Symbol: ?OnFillButtonInterior@CMFCVisualManagerWindows7@@UEAAXPEAVCDC@@PEAVCMFCToolBarButton@@VCRect@@W4AFX_BUTTON_STATE@CMFCVisualManager@@@Z
// Retail 0x1c05a0. Predicate FALSE (0x1c0715) calls 0x1b7ad0 =
// ?OnFillButtonInterior@CMFCVisualManagerWindows@@... with the same five
// arguments; predicate TRUE lands on that very same call when the button
// IsKindOf the CRuntimeClass at 0x3aa148 -- m_lpszClassName
// "CMFCCustomizeButton" -- (the `jne 0x1c06f9` at 0x1c05f2 falls into the base
// call at 0x1c06fe) and otherwise fills from the themed button renderers.
// The reachable callee, CMFCVisualManagerWindows::OnFillButtonInterior, is an
// empty generated stub in this tree, and CMFCVisualManager declares no
// OnFillButtonInterior virtual at all (its impl__ thunk is likewise an empty
// generated stub). There is therefore nothing to delegate to that would do
// anything, so this is left a stub rather than a call that paints nothing.
extern "C" void MS_ABI impl__OnFillButtonInterior_CMFCVisualManagerWindows7__UEAAXPEAVCDC__PEAVCMFCToolBarButton__VCRect__W4AFX_BUTTON_STATE_CMFCVisualManager___Z(
    CMFCVisualManagerWindows7* /*pThis*/, CDC* /*pDC*/, CMFCToolBarButton* /*pButton*/,
    CRect /*rect*/, CMFCVisualManager::AFX_BUTTON_STATE /*state*/) {}

// Symbol: ?OnFillMenuImageRect@CMFCVisualManagerWindows7@@UEAAXPEAVCDC@@PEAVCMFCToolBarButton@@VCRect@@W4AFX_BUTTON_STATE@CMFCVisualManager@@@Z
// Retail 0x1c0530. Predicate FALSE (0x1c0570) calls 0x1b7ea0 =
// ?OnFillMenuImageRect@CMFCVisualManagerWindows@@... with the same five
// arguments; predicate TRUE dispatches a virtual on `this` instead.
// The reachable callee is an empty generated stub in this tree and
// CMFCVisualManager has neither an OnFillMenuImageRect virtual nor an impl__
// thunk for one, so there is nothing to delegate to. Left a stub.
extern "C" void MS_ABI impl__OnFillMenuImageRect_CMFCVisualManagerWindows7__UEAAXPEAVCDC__PEAVCMFCToolBarButton__VCRect__W4AFX_BUTTON_STATE_CMFCVisualManager___Z(
    CMFCVisualManagerWindows7* /*pThis*/, CDC* /*pDC*/, CMFCToolBarButton* /*pButton*/,
    CRect /*rect*/, CMFCVisualManager::AFX_BUTTON_STATE /*state*/) {}

// Symbol: ?OnFillRibbonButton@CMFCVisualManagerWindows7@@UEAAKPEAVCDC@@PEAVCMFCRibbonButton@@@Z
// Retail 0x1c17d0. Predicate FALSE (0x1c1822) calls 0x1b1bf0 =
// ?OnFillRibbonButton@CMFCVisualManagerOfficeXP@@MEAAK... with the same three
// arguments; predicate TRUE is the largest body in this class -- it starts
// from ?IsMenuMode@CMFCRibbonBaseElement@@QEBAHXZ (0x10580) and then selects
// among a dozen CMFCControlRenderer members and the bitmap cache before
// returning a text colour.
// The reachable callee is CMFCVisualManagerOfficeXP's, which OpenMFC does not
// have as an ancestor; this tree's OfficeXP override and the CMFCVisualManager
// impl__ thunk of the same name are both empty generated stubs returning 0, and
// CMFCVisualManager declares no OnFillRibbonButton virtual. There is nothing to
// delegate to, so this is left a stub.
extern "C" unsigned long MS_ABI impl__OnFillRibbonButton_CMFCVisualManagerWindows7__UEAAKPEAVCDC__PEAVCMFCRibbonButton___Z(
    CMFCVisualManagerWindows7* /*pThis*/, CDC* /*pDC*/, CMFCRibbonButton* /*pButton*/)
{
    return 0;
}

// Symbol: ?OnFillRibbonQuickAccessToolBarPopup@CMFCVisualManagerWindows7@@UEAAXPEAVCDC@@PEAVCMFCRibbonPanelMenuBar@@VCRect@@@Z
// Retail entry 0x1c24d0 (mfc140; 0x1c3ed0 in mfc140u). Predicate FALSE, or
// the CMFCControlRenderer at this->[+0x5b08] having a zero [+0xa8]
// (`cmpq $0x0,0xa8(%rcx)` at 0x1c24f5), calls 0x18cc80 =
// ?OnFillRibbonQuickAccessToolBarPopup@CMFCVisualManager@@... with the same
// three arguments (the rect copied to the stack); past both guards retail
// instead calls that renderer's vtable +0x48 with (pDC, &rectCopy, NULL, 0xff).
// The predicate is FALSE here (see the banner), so the base call is the
// reachable path, and it is retail's own callee -- no substitution.
extern "C" void MS_ABI impl__OnFillRibbonQuickAccessToolBarPopup_CMFCVisualManagerWindows7__UEAAXPEAVCDC__PEAVCMFCRibbonPanelMenuBar__VCRect___Z(
    CMFCVisualManagerWindows7* pThis, CDC* pDC, void* pMenuBar, CRect rect)
{
    if (!pThis) return;
    impl__OnFillRibbonQuickAccessToolBarPopup_CMFCVisualManager__UEAAXPEAVCDC__PEAVCMFCRibbonPanelMenuBar__VCRect___Z(
        pThis, pDC, pMenuBar, rect);
}

// Symbol: ?OnHighlightMenuItem@CMFCVisualManagerWindows7@@UEAAXPEAVCDC@@PEAVCMFCToolBarMenuButton@@VCRect@@AEAK@Z
// Retail entry 0x1bf4e0 (mfc140; 0x1c0ee0 in mfc140u). Predicate FALSE
// (`jne` at 0x1bf4fc not taken) calls 0x1b7cc0 =
// ?OnHighlightMenuItem@CMFCVisualManagerWindows@@... with the same four
// arguments and returns. Predicate TRUE instead stores the COLORREF at
// afxGlobalData+0x84 into clrText and draws the CMFCControlRenderer at
// this->[+0x9c8], or [+0xbc8] when pButton->[+0x28] has bit 0x40000 set
// (the `and $0x40000` / `or $0x139000` / `shr $9` at 0x1bf56e..0x1bf57b),
// through its vtable +0x30 with (pDC, &rectCopy, 0, 0xff).
// Only the FALSE branch is reachable here. Its retail callee (0x1b7cc0) has
// three exits: when the window theme handle at [+0x8] is NULL or [+0x210] is
// non-zero it calls ?OnHighlightMenuItem@CMFCVisualManagerOfficeXP@@
// (0x1ae070); otherwise, when the menu theme handle at [+0x98] is NULL, it
// calls ?OnHighlightMenuItem@CMFCVisualManager@@ (0x183370); otherwise it
// draws UxTheme!DrawThemeBackground([+0x98], hdc, 14, 2, &rect, NULL) and
// reads clrText with UxTheme!GetThemeColor([+0x98], 14, 2, 0xedb, &clrText)
// (import slots 0x2c53e0 / 0x2c53e8). [+0x8] lies inside OpenMFC's
// zero-filled CMFCVisualManager padding and no OpenMFC body opens a theme
// into it, so retail's code over this object state would take the OfficeXP
// exit. OpenMFC has neither CMFCVisualManagerWindows nor OfficeXP as an
// ancestor of this class, and the OfficeXP body hands its fill virtual the
// brush member at this+0x180 or this+0x1a0, past the end of this 0x148-byte
// object, so the call is routed to
// CMFCVisualManager's implementation (a COLOR_HIGHLIGHT fill that leaves
// clrText untouched) -- a deliberate substitution, not a transcription. It is
// retail's own second exit, but not the one this object state selects: the
// OfficeXP body (InflateRect by -1 horizontally, a brush fill through
// vtable +0x638, a Draw3dRect frame, clrText from vtable +0x138) also frames
// the item and writes clrText; this does neither.
extern "C" void MS_ABI impl__OnHighlightMenuItem_CMFCVisualManagerWindows7__UEAAXPEAVCDC__PEAVCMFCToolBarMenuButton__VCRect__AEAK_Z(
    CMFCVisualManagerWindows7* pThis, CDC* pDC, CMFCToolBarMenuButton* pButton, CRect rect,
    unsigned long& clrText)
{
    if (!pThis) return;
    impl__OnHighlightMenuItem_CMFCVisualManager__UEAAXPEAVCDC__PEAVCMFCToolBarMenuButton__VCRect__AEAK_Z(
        pThis, pDC, pButton, rect, clrText);
}

// Symbol: ?OnNcActivate@CMFCVisualManagerWindows7@@UEAAHPEAVCWnd@@H@Z
// Retail entry 0x1c2bf0 (mfc140; 0x1c45f0 in mfc140u), transcribed:
//     if (pWnd == NULL || pWnd->m_hWnd (+0x40) == NULL) return FALSE;
//     if (afxGlobalData.IsDwmCompositionEnabled()) return FALSE;   // 0x6c260
//     pBar = GetRibbonBar(pWnd);                                    // 0x1a16b0
//     if (pBar == NULL || pBar->m_hWnd == NULL
//         || !::IsWindowVisible(pBar->m_hWnd)                       // slot 0x2c5350
//         || pBar->[+0x468] == 0) return FALSE;
//     if (pWnd->[+0xa8] & 0x20) bActive = TRUE;                     // 0x1c2c8a..0x1c2c99
//     bNew = pWnd->IsWindowEnabled() ? bActive : FALSE;             // 0x2a7a10
//     if (this->vtable[+0x3f8]()) {
//         bMDI = pWnd->IsKindOf(<CRuntimeClass at 0x339c20>);       // 0x233310
//                // (that descriptor's m_lpszClassName is "CMDIFrameWnd")
//         bOld = IsWindowActive(pWnd);                              // 0x1c2bb0
//     } else bMDI = bOld = FALSE;
//     m_mapActive(this+0x9258)[pWnd->m_hWnd] = bNew;                // CMap operator[] at 0x1ab868
//     ::SendMessage(pWnd->m_hWnd, WM_NCPAINT, 0, 0);                // slot 0x2c5378
//     if (this->vtable[+0x3f8]() && bMDI && bOld != bNew)
//         ::RedrawWindow(pWnd->[+0x1d8], NULL, NULL,
//                        RDW_INVALIDATE | RDW_ALLCHILDREN);          // slot 0x2c5388, flags 0x81
//     return TRUE;
// (vtable +0x3f8 of the Windows7 vftable at mfc140 0x31cdd0 is 0x7260, a
// shared `xor %eax,%eax; ret` body, so for this class itself bMDI and bOld
// are always FALSE and the RedrawWindow is reached only by a derived override.)
// Every TRUE path goes through GetRibbonBar returning a visible ribbon bar,
// and this file's GetRibbonBar is a stub returning NULL (the frame layout it
// needs is not modelled); the per-HWND map at this+0x9258 has no storage in
// OpenMFC's 64-byte derived block either. The reachable result is therefore
// FALSE for every input, which is indistinguishable from a stub, so it is left
// one -- the same call IsRibbonPresent makes above.
extern "C" int MS_ABI impl__OnNcActivate_CMFCVisualManagerWindows7__UEAAHPEAVCWnd__H_Z(
    CMFCVisualManagerWindows7* /*pThis*/, CWnd* /*pWnd*/, int /*bActive*/)
{
    return FALSE;
}

// Symbol: ?OnNcPaint@CMFCVisualManagerWindows7@@UEAAHPEAVCWnd@@AEBVCObList@@VCRect@@@Z
// Retail entry 0x1c2d60 (mfc140; 0x1c4760 in mfc140u). It returns FALSE
// (the `xor %eax,%eax` at 0x1c30cf) when
// afxGlobalData.IsDwmCompositionEnabled() (0x6c260) is
// non-zero, when pWnd or pWnd->m_hWnd is NULL, when the theme handle at
// this->[+0x8] is NULL, when GetRibbonBar(pWnd) (0x1a16b0) is NULL or not
// ::IsWindowVisible (slot 0x2c5350), or when that bar's [+0x468] is zero.
// Past those guards it returns TRUE immediately if pWnd->GetStyle()
// (0x2a75a0) has bit 24 set (WS_MAXIMIZE). Otherwise it builds a CWindowDC
// (0x2a1b60) and, if that DC's m_hDC is NULL, destroys it and returns FALSE
// (the `mov %r12d,%r14d` at 0x1c30be, r12d still zero); else it returns TRUE
// after painting the frame through it: clipped to the caller's rect
// (CreateRectRgnIndirect slot 0x2c41a0 + ?SelectClipRgn@CDC@@ 0x2a0d60)
// unless ::IsRectEmpty (slot 0x2c52c8), the client area removed with
// ?ExcludeClipRect@CDC@@ (0x2a0e20) after GetWindowRect / ScreenToClient /
// GetClientRect / OffsetRect (GetWindowRect slot 0x2c5370,
// ?ScreenToClient@CWnd@@ 0x2a11f0, slots 0x2c5358 and 0x2c5318),
// then four UxTheme!DrawThemeBackground calls (slot 0x2c53e0) on
// this->[+0x8] with parts 1, 9, 7 and 8 and a state of 1 when the window
// counts as active, else 2 -- activity from IsWindowActive (0x1c2bb0);
// when that is TRUE, the virtual at vtable +0x3f8 returns TRUE (it is
// `xor %eax,%eax; ret` for this class) and pWnd IsKindOf the descriptor
// at 0x339bf0 ("CMDIChildWnd"), it is re-derived through
// ?GetMDIFrame@CMDIChildWnd@@ (0x2a5790) and ?MDIGetActive@CMDIFrameWnd@@
// (0x2a4a00): when both return non-NULL it stays active only if the active
// child's m_hWnd is pWnd's and IsWindowActive(frame) holds (a NULL from
// either leaves it active).
// [+0x8] lies inside OpenMFC's zero-filled CMFCVisualManager padding and no
// OpenMFC body opens a theme into it, and GetRibbonBar is a stub returning
// NULL, so every reachable path returns FALSE -- indistinguishable from a
// stub, and left one. (Retail never reads the system-button list at
// all -- R8 is not even saved -- and reads the rect only on the paint path.)
extern "C" int MS_ABI impl__OnNcPaint_CMFCVisualManagerWindows7__UEAAHPEAVCWnd__AEBVCObList__VCRect___Z(
    CMFCVisualManagerWindows7* /*pThis*/, CWnd* /*pWnd*/, const CObList& /*lstSysButtons*/,
    CRect /*rectRedraw*/)
{
    return FALSE;
}

// Symbol: ?OnUpdateSystemColors@CMFCVisualManagerWindows7@@UEAAXXZ
// Retail entry 0x1bbea0 (mfc140; 0x1bd8a0 in mfc140u), vtable +0x70 of the
// Windows7 vftable (mfc140 0x31cdd0). Its head, which decides everything:
//     if (g_hinstRes /*0x3b7140*/ == NULL) {
//         SetStyle(NULL);                     // `xor %ecx,%ecx; call 0x1bb460`
//         return;                             // jmp 0x1bf200 = the epilogue
//     }
//     hOld = AfxGetModuleState()->[+0x18];    // 0x1345b0; swapped to g_hinstRes
//     this->vtable[+0x658]();                 // = CleanUp (this class's, 0x1bb9a0)
//     CMFCVisualManagerWindows::OnUpdateSystemColors(this);   // 0x1b7190
//     this->[+0xf4] = 1;
//     ...then load "WINDOWS7_IDX_STYLE" as a STYLE_XML resource through a
//     CTagManager and, if that fails, put the saved resource handle back
//     (only when it was non-NULL) and return; otherwise parse the style XML
//     into the renderer, image and colour members -- the rest of a body
//     that runs to 0x1bf222, some 13 KB of code, not transcribed here.
// OpenMFC reproduces the head:
//   * NULL handle: calls this file's CMFCVisualManagerWindows7::SetStyle
//     (NULL), which is what retail calls. OpenMFC's SetStyle re-applies the
//     Windows 7 palette where retail's would look for the style resource.
//   * non-NULL handle (only after SetResourceHandle): the CleanUp call, made
//     directly to this file's thunk rather than through vtable +0x658, then
//     the base update routed to CMFCVisualManager's thunk -- a deliberate
//     substitution for CMFCVisualManagerWindows's, which OpenMFC's hierarchy
//     does not reach (see the banner) -- then the +0xf4 store, transcribed:
//     +0xf4 is a CMFCVisualManager member (m_nMenuBorderSize, as
//     featurepack/menu/CMFCPopupMenu.cpp's GetBorderSize reads it) that lies
//     inside OpenMFC's zero-filled `_visualmanager_padding`, so it is written
//     at the retail offset exactly as that reader expects.
//     The module-state resource-handle swap and the style-XML load are NOT
//     reproduced: nothing the XML would fill has storage in this 0x148-byte
//     object, and with no load between the swap and its undo, swapping the
//     handle would have no reader.
extern "C" void MS_ABI impl__OnUpdateSystemColors_CMFCVisualManagerWindows7__UEAAXXZ(
    CMFCVisualManagerWindows7* pThis)
{
    if (s_hinstW7Res == nullptr) {
        CMFCVisualManagerWindows7::SetStyle(nullptr);
        return;
    }
    if (!pThis) return;
    impl__CleanUp_CMFCVisualManagerWindows7__UEAAXXZ(pThis);
    impl__OnUpdateSystemColors_CMFCVisualManager__UEAAXXZ(pThis);
    static_assert(sizeof(CMFCVisualManager) >= 0xf4 + sizeof(int),
                  "CMFCVisualManager +0xf4 lies inside OpenMFC's object");
    const int nOne = 1;
    std::memcpy(reinterpret_cast<char*>(pThis) + 0xf4, &nOne, sizeof(nOne));
}

// Symbol: ?SetResourceHandle@CMFCVisualManagerWindows7@@SAXPEAUHINSTANCE__@@@Z
// Retail entry 0x1bce00 (mfc140u; the ANSI twin is mfc140 0x1bb400), complete:
//     g_bAutoFreeRes = FALSE;                     // unconditional: the store at
//                                                 // 0x1bce0b sits between the
//                                                 // cmp and its je
//     if (g_hinstRes != hinstRes) {
//         g_hinstRes = hinstRes;
//         if (<GetInstance>()->IsKindOf(RUNTIME_CLASS(CMFCVisualManagerWindows7)))
//             <GetInstance>()->OnUpdateSystemColors();   // vtable +0x70
//     }
// <GetInstance> is the internal helper at mfc140u 0x9774 (mfc140 0x97f4)
// that creates the manager singleton on first use; the descriptor is the one
// ?GetThisClass@CMFCVisualManagerWindows7@@ returns (mfc140 0x31d438).
// DEVIATIONS, the same two CMFCVisualManagerOffice2007.cpp makes in its
// SetResourceHandle: the exported m_pVisManager is read instead of calling
// the creating helper, so a missing manager is not created; and
// OnUpdateSystemColors is called through this file's thunk rather than
// dispatched, so an override in a class derived from this one is not reached.
extern "C" void MS_ABI impl__SetResourceHandle_CMFCVisualManagerWindows7__SAXPEAUHINSTANCE_____Z(
    HINSTANCE hinstRes)
{
    s_bW7AutoFreeRes = FALSE;
    if (s_hinstW7Res == hinstRes) return;
    s_hinstW7Res = hinstRes;
    CMFCVisualManager* pManager =
        static_cast<CMFCVisualManager*>(impl__m_pVisManager_CMFCVisualManager__1PEAV1_EA);
    if (pManager != nullptr &&
        impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(pManager, CMFCVisualManagerWindows7::GetThisClass())) {
        impl__OnUpdateSystemColors_CMFCVisualManagerWindows7__UEAAXXZ(
            static_cast<CMFCVisualManagerWindows7*>(pManager));
    }
}
