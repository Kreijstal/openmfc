// CMFCVisualManagerWindows — OpenMFC implementation.
// Sources: mfccore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/MfccoreSupport.h"

// Symbol: ?CreateObject@CMFCVisualManagerWindows@@SAPEAVCObject@@XZ
extern "C" CObject* MS_ABI impl__CreateObject_CMFCVisualManagerWindows__SAPEAVCObject__XZ() { return new CMFCVisualManagerWindows(); }
// Symbol: ?GetThisClass@CMFCVisualManagerWindows@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMFCVisualManagerWindows__SAPEAUCRuntimeClass__XZ() { return CMFCVisualManagerWindows::GetThisClass(); }
// Symbol: ?GetRuntimeClass@CMFCVisualManagerWindows@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CMFCVisualManagerWindows__UEBAPEAUCRuntimeClass__XZ(CMFCVisualManagerWindows* pThis) { return CMFCVisualManagerWindows::GetThisClass(); }
// Symbol: ?OnDrawButtonBorder@CMFCVisualManagerWindows@@UEAAXPEAVCDC@@PEAVCMFCToolBarButton@@VCRect@@W4AFX_BUTTON_STATE@CMFCVisualManager@@@Z
extern "C" void MS_ABI impl__OnDrawButtonBorder_CMFCVisualManagerWindows__UEAAXPEAVCDC__PEAVCMFCToolBarButton__VCRect__W4AFX_BUTTON_STATE_CMFCVisualManager___Z(
    CMFCVisualManagerWindows* pThis, CDC* pDC, CMFCToolBarButton* pButton, CRect rect, CMFCVisualManager::AFX_BUTTON_STATE state) {
    pThis->CMFCVisualManager::OnDrawButtonBorder(pDC, pButton, rect, state);
}
// Symbol: ?OnDrawComboDropButton@CMFCVisualManagerWindows@@UEAAXPEAVCDC@@VCRect@@HHHPEAVCMFCToolBarComboBoxButton@@@Z
extern "C" void MS_ABI impl__OnDrawComboDropButton_CMFCVisualManagerWindows__UEAAXPEAVCDC__VCRect__HHHPEAVCMFCToolBarComboBoxButton___Z(
    CMFCVisualManagerWindows* pThis, CDC* pDC, CRect rect, int bDisabled, int bIsDropped, int bIsHighlighted, CMFCToolBarComboBoxButton* pButton) {
    pThis->CMFCVisualManager::OnDrawComboDropButton(pDC, rect, bDisabled, bIsDropped, bIsHighlighted, pButton);
}
// Symbol: ?OnDrawMenuBorder@CMFCVisualManagerWindows@@UEAAXPEAVCDC@@PEAVCMFCPopupMenu@@VCRect@@@Z
extern "C" void MS_ABI impl__OnDrawMenuBorder_CMFCVisualManagerWindows__UEAAXPEAVCDC__PEAVCMFCPopupMenu__VCRect___Z(
    CMFCVisualManagerWindows* pThis, CDC* pDC, CMFCPopupMenu* pMenu, CRect rect) {
    pThis->CMFCVisualManager::OnDrawMenuBorder(pDC, pMenu, rect);
}
// Symbol: ?OnDrawStatusBarProgress@CMFCVisualManagerWindows@@UEAAXPEAVCDC@@PEAVCMFCStatusBar@@VCRect@@HHKKKH@Z
extern "C" void MS_ABI impl__OnDrawStatusBarProgress_CMFCVisualManagerWindows__UEAAXPEAVCDC__PEAVCMFCStatusBar__VCRect__HHKKKH_Z(
    CMFCVisualManagerWindows* pThis, CDC* pDC, CMFCStatusBar* pBar,
    CRect rectProgress, int nProgressTotal, int nProgressCurr,
    unsigned long clrBar, unsigned long clrProgressBarDest, unsigned long clrProgressText, int bProgressText) {
    pThis->OnDrawStatusBarProgress(pDC, pBar, rectProgress, nProgressTotal, nProgressCurr,
                                   clrBar, clrProgressBarDest, clrProgressText, bProgressText);
}
CMFCVisualManagerWindows::CMFCVisualManagerWindows() {
    memset(_pad, 0, sizeof(_pad));
    impl__m_b3DTabsXPTheme_CMFCVisualManagerWindows__2HA = TRUE;
    ApplyVisualPaletteForClass(GetThisClass());
}
CMFCVisualManagerWindows::~CMFCVisualManagerWindows() {}
CObject* CMFCVisualManagerWindows::CreateObject() { return new CMFCVisualManagerWindows(); }

// ---------------------------------------------------------------------------
// CMFCVisualManagerWindows -- the "Windows Native"/XP-theme visual manager.
//
// Every body below was transcribed from the retail mfc140.dll shipped with
// MSVC 14.51.36231 (x64).  The addresses quoted in the comments are mfc140
// (ANSI) RVAs of the FUNCTION ENTRY they name; the bodies are byte-identical
// to mfc140u's, so the control flow and the member offsets are the ones this
// DLL must reproduce, but the addresses themselves are NOT mfc140u addresses.
//
// The one structural fact that shapes the whole class:
//
//   CMFCVisualManagerWindows derives from CMFCVisualManagerOfficeXP, which
//   derives from CMFCVisualManager, which derives from CMFCBaseVisualManager.
//   CMFCBaseVisualManager sits at offset 0 and owns a bank of UxTheme HTHEME
//   handles -- the ones reached below are at +0x08, +0x10, +0x18, +0x20, +0x28,
//   +0x30, +0x40, +0x48, +0x50, +0x58, +0x78, +0x80 and +0x98.  Retail opens
//   all nineteen (+0x08..+0x98) with ::OpenThemeData in
//   CMFCBaseVisualManager::UpdateSystemColors (0x18da80; the class names, in
//   offset order, are WINDOW +0x08, TOOLBAR +0x10, REBAR +0x18, BUTTON +0x20,
//   STATUS +0x28, COMBOBOX +0x30, PROGRESS +0x38, HEADER +0x40, SCROLLBAR
//   +0x48, EXPLORERBAR +0x50, TREEVIEW +0x58, STARTPANEL +0x60, TASKBAND
//   +0x68, TASKBAR +0x70, SPIN +0x78, TAB +0x80, TOOLTIP +0x88, TRACKBAR
//   +0x90, MENU +0x98) and closes them with ::CloseThemeData in CleanUpThemes.
//   Almost every override in this class is shaped as
//
//       if (<the theme handle this override needs> == NULL)
//           <base class>::Method(...);          // the classic, unthemed look
//       else
//           ::DrawThemeBackground(<handle>, ...);
//
//   OpenMFC does not model any of that state: CMFCBaseVisualManager's members
//   are not declared anywhere (see CMFCBaseVisualManager.cpp in this
//   directory, where the UpdateSystemColors and CleanUpThemes thunks are
//   documented no-ops), and OpenMFC's CMFCVisualManager is
//   CObject plus a zero-filled `char _visualmanager_padding[256]`.  No theme is
//   ever opened, so every one of those handles is permanently NULL and the
//   NULL-theme branch is the only reachable one.
//
//   The bodies below therefore take that branch unconditionally, forwarding to
//   the same base-class entry point retail forwards to.  That is a real
//   implementation, not a placeholder: it is exactly what retail does on a
//   machine with visual styles disabled, and it makes this visual manager draw
//   the Office XP chrome instead of drawing nothing.  It is NOT the themed
//   look, and it never will be until the theme handles are modelled -- each
//   comment says which handle would have to become non-NULL for the other
//   branch to matter.
//
//   Note that OpenMFC's C++ vtable for CMFCVisualManagerWindows does not carry
//   the impl__ overrides (the class declared in include/openmfc/afxmfc.h
//   overrides none of these), so the forwarding below calls the base thunk
//   directly rather than dispatching virtually.  Retail's forwarding is a
//   scope-qualified base call too in every case here except OnFillMenuImageRect,
//   which is noted at its own definition.
//
// Recurring retail helpers referenced below, resolved from the RVA map:
//    0x2c53e0  UxTheme!DrawThemeBackground   (resolved with iat.py)
//    0x2c53e8  UxTheme!GetThemeColor
//    0x2c5bd0  __guard_dispatch_icall_fptr (CFG); `mov <slot>,%rax; call
//              *0x2c5bd0` is an ordinary indirect/virtual call through %rax.
//              (Identified in CMFCVisualManagerOfficeXP.cpp in this directory,
//              which confirms it against the PE load-config directory; not
//              re-verified here.)
//    0x6a5c0   AFX_GLOBAL_DATA::Initialize
//    0x233310  CObject::IsKindOf
//    0x2a3b00  CDC::Draw3dRect(LPCRECT, COLORREF clrTopLeft, COLORREF clrBottomRight)
//    0x225b80  AfxThrowInvalidArgException
// The `this->[+0x210]` tested below is the BOOL that SetOfficeStyleMenus
// (0x1b7180, a single `mov %edx,0x210(%rcx)`) stores -- m_bOfficeStyleMenus.
// The afxGlobalData fields read below are named from AFX_GLOBAL_DATA::
// UpdateSysColors (0x6afd0): +0x60 clrBarFace = GetSysColor(COLOR_BTNFACE),
// +0x64 clrBarShadow (COLOR_BTNSHADOW), +0x68 clrBarHilite (COLOR_BTNHIGHLIGHT),
// +0x6c clrBarDkShadow (COLOR_3DDKSHADOW); the other offsets are quoted raw.
// ---------------------------------------------------------------------------

// --- Base-class entry points this unit forwards to. -------------------------
// All of these are defined in this tree (CMFCVisualManagerOfficeXP.cpp,
// CMFCVisualManager.cpp, Thunks.cpp, CMFCBaseVisualManager.cpp).  Several of
// them still carry auto-generated parameter lists at their definition; the
// declarations here are the ones derived from the mangled export name and from
// the retail call sites decoded below, and those definitions have empty bodies
// that read no argument, so the mismatch is inert until they are implemented.
extern "C" void MS_ABI impl__OnDrawBarGripper_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__VCRect__HPEAVCBasePane___Z(
    CMFCVisualManagerOfficeXP* pThis, CDC* pDC, CRect rectGripper, int bHorz, CBasePane* pBar);
extern "C" int MS_ABI impl__OnDrawBrowseButton_CMFCVisualManagerOfficeXP__MEAAHPEAVCDC__VCRect__PEAVCMFCEditBrowseCtrl__W4AFX_BUTTON_STATE_CMFCVisualManager__AEAK_Z(
    CMFCVisualManagerOfficeXP* pThis, CDC* pDC, CRect rect, void* pEdit, int state, unsigned long* pclrText);
extern "C" void MS_ABI impl__OnDrawButtonSeparator_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__PEAVCMFCToolBarButton__VCRect__W4AFX_BUTTON_STATE_CMFCVisualManager__H_Z(
    CMFCVisualManagerOfficeXP* pThis, CDC* pDC, CMFCToolBarButton* pButton, CRect rect, int state, int bHorz);
extern "C" void MS_ABI impl__OnDrawCaptionButton_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__PEAVCMFCCaptionButton__HHHHH_Z(
    CMFCVisualManagerOfficeXP* pThis, CDC* pDC, CMFCCaptionButton* pButton, int p2, int p3, int p4, int p5, int p6);
extern "C" void MS_ABI impl__OnDrawCaptionButtonIcon_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__PEAVCMFCCaptionButton__W4IMAGES_IDS_CMenuImages__HHVCPoint___Z(
    CMFCVisualManagerOfficeXP* pThis, CDC* pDC, CMFCCaptionButton* pButton, int id, int bHorz, int bMaximized,
    long long ptImage /* CPoint by value: an 8-byte aggregate, passed in a register under MS_ABI */);
extern "C" void MS_ABI impl__OnDrawComboBorder_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__VCRect__HHHPEAVCMFCToolBarComboBoxButton___Z(
    CMFCVisualManagerOfficeXP* pThis, CDC* pDC, CRect rect, int bDisabled, int bIsDropped, int bIsHighlighted,
    CMFCToolBarComboBoxButton* pButton);
extern "C" void MS_ABI impl__OnDrawEditBorder_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__VCRect__HHPEAVCMFCToolBarEditBoxButton___Z(
    CMFCVisualManagerOfficeXP* pThis, CDC* pDC, CRect rect, int bDisabled, int bIsHighlighted,
    CMFCToolBarEditBoxButton* pButton);
extern "C" void MS_ABI impl__OnDrawMenuSystemButton_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__VCRect__IIH_Z(
    CMFCVisualManagerOfficeXP* pThis, CDC* pDC, CRect rect, unsigned int uiSystemCommand, unsigned int nStyle, int bHighlight);
extern "C" unsigned long MS_ABI impl__OnDrawPaneCaption_CMFCVisualManagerOfficeXP__MEAAKPEAVCDC__PEAVCDockablePane__HVCRect__2_Z(
    CMFCVisualManagerOfficeXP* pThis, CDC* pDC, CDockablePane* pBar, int bActive, CRect rectCaption, CRect rectButtons);
extern "C" void MS_ABI impl__OnDrawPopupWindowButtonBorder_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__VCRect__PEAVCMFCDesktopAlertWndButton___Z(
    CMFCVisualManagerOfficeXP* pThis, CDC* pDC, CRect rectClient, CMFCDesktopAlertWndButton* pButton);
extern "C" void MS_ABI impl__OnDrawSeparator_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__PEAVCBasePane__VCRect__H_Z(
    CMFCVisualManagerOfficeXP* pThis, CDC* pDC, CBasePane* pBar, CRect rect, int bHorz);
extern "C" void MS_ABI impl__OnDrawSpinButtons_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__VCRect__HHPEAVCMFCSpinButtonCtrl___Z(
    CMFCVisualManagerOfficeXP* pThis, CDC* pDC, CRect rectSpin, int nState, int bOrientation, CMFCSpinButtonCtrl* pSpinCtrl);
extern "C" void MS_ABI impl__OnDrawStatusBarPaneBorder_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__PEAVCMFCStatusBar__VCRect__II_Z(
    CMFCVisualManagerOfficeXP* pThis, CDC* pDC, CMFCStatusBar* pBar, CRect rectPane, unsigned int uiID, unsigned int nStyle);
extern "C" void MS_ABI impl__OnDrawTab_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__VCRect__HHPEBVCMFCBaseTabCtrl___Z(
    CMFCVisualManagerOfficeXP* pThis, CDC* pDC, CRect rectTab, int iTab, int bIsActive, const CMFCBaseTabCtrl* pTabWnd);
extern "C" void MS_ABI impl__OnDrawTabCloseButton_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__VCRect__PEBVCMFCBaseTabCtrl__HHH_Z(
    CMFCVisualManagerOfficeXP* pThis, CDC* pDC, CRect rect, const CMFCBaseTabCtrl* pTabWnd,
    int bIsHilited, int bIsPressed, int bIsDisabled);
extern "C" void MS_ABI impl__OnDrawTabsButtonBorder_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__AEAVCRect__PEAVCMFCButton__IPEAVCMFCBaseTabCtrl___Z(
    CMFCVisualManagerOfficeXP* pThis, CDC* pDC, CRect* pRect, CMFCButton* pButton, unsigned int uiState,
    CMFCBaseTabCtrl* pWndTab);
extern "C" void MS_ABI impl__OnDrawTask_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__PEAVCMFCTasksPaneTask__PEAVCImageList__HH_Z(
    CMFCVisualManagerOfficeXP* pThis, CDC* pDC, CMFCTasksPaneTask* pTask, CImageList* pIcons,
    int bIsHighlighted, int bIsSelected);
extern "C" void MS_ABI impl__OnDrawTasksGroupCaption_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__PEAVCMFCTasksPaneTaskGroup__HHH_Z(
    CMFCVisualManagerOfficeXP* pThis, CDC* pDC, CMFCTasksPaneTaskGroup* pGroup,
    int bIsHighlighted, int bIsSelected, int bCanCollapse);
extern "C" void MS_ABI impl__OnDrawTearOffCaption_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__VCRect__H_Z(
    CMFCVisualManagerOfficeXP* pThis, CDC* pDC, CRect rectCaption, int bIsActive);
extern "C" void MS_ABI impl__OnErasePopupWindowButton_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__VCRect__PEAVCMFCDesktopAlertWndButton___Z(
    CMFCVisualManagerOfficeXP* pThis, CDC* pDC, CRect rectClient, CMFCDesktopAlertWndButton* pButton);
extern "C" void MS_ABI impl__OnEraseTabsArea_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__VCRect__PEBVCMFCBaseTabCtrl___Z(
    CMFCVisualManagerOfficeXP* pThis, CDC* pDC, CRect rect, const CMFCBaseTabCtrl* pTabWnd);
extern "C" void MS_ABI impl__OnEraseTabsButton_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__VCRect__PEAVCMFCButton__PEAVCMFCBaseTabCtrl___Z(
    CMFCVisualManagerOfficeXP* pThis, CDC* pDC, CRect rect, CMFCButton* pButton, CMFCBaseTabCtrl* pWndTab);
extern "C" void MS_ABI impl__OnFillBarBackground_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__PEAVCBasePane__VCRect__2H_Z(
    CMFCVisualManagerOfficeXP* pThis, CDC* pDC, CBasePane* pBar, CRect rectClient, CRect rectClip, int bNCArea);
extern "C" void MS_ABI impl__OnFillButtonInterior_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__PEAVCMFCToolBarButton__VCRect__W4AFX_BUTTON_STATE_CMFCVisualManager___Z(
    CMFCVisualManagerOfficeXP* pThis, CDC* pDC, CMFCToolBarButton* pButton, CRect rect, int state);
extern "C" unsigned long MS_ABI impl__OnFillCommandsListBackground_CMFCVisualManagerOfficeXP__MEAAKPEAVCDC__VCRect__H_Z(
    CMFCVisualManagerOfficeXP* pThis, CDC* pDC, CRect rect, int bIsSelected);
extern "C" unsigned long MS_ABI impl__GetHighlightedMenuItemTextColor_CMFCVisualManagerOfficeXP__MEAAKPEAVCMFCToolBarMenuButton___Z(
    CMFCVisualManagerOfficeXP* pThis, CMFCToolBarMenuButton* pButton);
extern "C" unsigned long MS_ABI impl__GetToolbarButtonTextColor_CMFCVisualManagerOfficeXP__MEAAKPEAVCMFCToolBarButton__W4AFX_BUTTON_STATE_CMFCVisualManager___Z(
    CMFCVisualManagerOfficeXP* pThis, CMFCToolBarButton* pButton, int state);

extern "C" void MS_ABI impl__OnDrawCheckBoxEx_CMFCVisualManager__UEAAXPEAVCDC__VCRect__HHHH_Z(
    CMFCVisualManager* pThis, CDC* pDC, CRect rect, int nState, int bHighlighted, int bPressed, int bEnabled);
extern "C" void MS_ABI impl__OnDrawControlBorder_CMFCVisualManager__UEAAXPEAVCWnd___Z(
    CMFCVisualManager* pThis, CWnd* pWndCtrl);
extern "C" void MS_ABI impl__OnDrawExpandingBox_CMFCVisualManager__UEAAXPEAVCDC__VCRect__HK_Z(
    CMFCVisualManager* pThis, CDC* pDC, CRect rect, int bIsOpened, unsigned long colorBox);
extern "C" void MS_ABI impl__OnDrawFloatingToolbarBorder_CMFCVisualManager__UEAAXPEAVCDC__PEAVCMFCBaseToolBar__VCRect__2_Z(
    CMFCVisualManager* pThis, CDC* pDC, CMFCBaseToolBar* pBar, CRect rectBorder, CRect rectBorderSize);
extern "C" void MS_ABI impl__OnDrawHeaderCtrlBorder_CMFCVisualManager__UEAAXPEAVCMFCHeaderCtrl__PEAVCDC__AEAVCRect__HH_Z(
    CMFCVisualManager* pThis, CMFCHeaderCtrl* pCtrl, CDC* pDC, CRect* pRect, int bIsPressed, int bIsHighlighted);
extern "C" void MS_ABI impl__OnDrawHeaderCtrlSortArrow_CMFCVisualManager__UEAAXPEAVCMFCHeaderCtrl__PEAVCDC__AEAVCRect__H_Z(
    CMFCVisualManager* pThis, CMFCHeaderCtrl* pCtrl, CDC* pDC, CRect* pRect, int bIsAscending);
extern "C" void MS_ABI impl__OnDrawMiniFrameBorder_CMFCVisualManager__UEAAXPEAVCDC__PEAVCPaneFrameWnd__VCRect__2_Z(
    CMFCVisualManager* pThis, CDC* pDC, CPaneFrameWnd* pFrameWnd, CRect rectBorder, CRect rectBorderSize);
extern "C" void MS_ABI impl__OnDrawOutlookPageButtonBorder_CMFCVisualManager__UEAAXPEAVCDC__AEAVCRect__HH_Z(
    CMFCVisualManager* pThis, CDC* pDC, CRect* pRect, int bIsHighlighted, int bIsPressed);
extern "C" void MS_ABI impl__OnDrawPaneBorder_CMFCVisualManager__UEAAXPEAVCDC__PEAVCBasePane__AEAVCRect___Z(
    CMFCVisualManager* pThis, CDC* pDC, CBasePane* pBar, CRect* pRect);
extern "C" void MS_ABI impl__OnDrawStatusBarSizeBox_CMFCVisualManager__UEAAXPEAVCDC__PEAVCMFCStatusBar__VCRect___Z(
    CMFCVisualManager* pThis, CDC* pDC, CMFCStatusBar* pBar, CRect rectSizeBox);
extern "C" int MS_ABI impl__OnEraseTabsFrame_CMFCVisualManager__UEAAHPEAVCDC__VCRect__PEBVCMFCBaseTabCtrl___Z(
    CMFCVisualManager* pThis, CDC* pDC, CRect rect, const CMFCBaseTabCtrl* pTabWnd);

// CMFCBaseVisualManager::DrawCheckBox -- retail dispatches it through the
// vtable at byte offset +0x50 (verified by reading slot +0x50 of the retail
// CMFCVisualManagerWindows vftable, which holds CMFCBaseVisualManager::DrawCheckBox).
// Nothing in MFC overrides it below CMFCBaseVisualManager, so calling the thunk
// directly is equivalent here.
extern "C" int MS_ABI impl__DrawCheckBox_CMFCBaseVisualManager__UEAAHPEAVCDC__VCRect__HHHH_Z(
    void* pThis, CDC* pDC, CRect rect, int bHighlighted, int nState, int bEnabled, int bPressed);

extern "C" void MS_ABI impl__AfxThrowInvalidArgException__YAXXZ();

// CMenuImages::Draw(CDC*, IMAGES_IDS, const CRect&, IMAGE_STATE, const CSize&)
// -- retail 0x8fd40, used by OnDrawScrollButtons below.  Its definition
// (phase4/src/featurepack/controls/CMFCReBar.cpp) still carries a generated
// 25-parameter list and an empty body, so this call currently paints nothing;
// the declaration here is the one the mangled name and the retail call site
// give, so the glyph will appear once that export is implemented.
extern "C" void MS_ABI impl__Draw_CMenuImages__SAXPEAVCDC__W4IMAGES_IDS_1_AEBVCRect__W4IMAGE_STATE_1_AEBVCSize___Z(
    CDC* pDC, int id, const CRect* pRect, int state, const CSize* pSizeDest);

// This class's own OnFillButtonInterior override, defined further down; see
// OnFillMenuImageRect for why it is called by name.
extern "C" void MS_ABI impl__OnFillButtonInterior_CMFCVisualManagerWindows__UEAAXPEAVCDC__PEAVCMFCToolBarButton__VCRect__W4AFX_BUTTON_STATE_CMFCVisualManager___Z(
    CMFCVisualManagerWindows* pThis, CDC* pDC, CMFCToolBarButton* pButton, CRect rect, int state);

namespace {

// afxGlobalData COLORREF fields, named by their retail offsets from
// ?afxGlobalData@@3UAFX_GLOBAL_DATA@@A and mapped to the ::GetSysColor index
// that AFX_GLOBAL_DATA::UpdateSysColors (0x6afd0) stores into each of them.
// OpenMFC exports afxGlobalData as a zero-filled blob whose Initialize() is a
// no-op, so reading it would yield black; ::GetSysColor is the substitution the
// already-implemented CMFCVisualManager / CMFCVisualManagerOfficeXP colour
// getters in this directory make, and this file follows it.
inline COLORREF GD_clrBarFace()     { return ::GetSysColor(COLOR_BTNFACE); }        // +0x60
inline COLORREF GD_clrBarHilite()   { return ::GetSysColor(COLOR_BTNHIGHLIGHT); }   // +0x68
inline COLORREF GD_clrBarDkShadow() { return ::GetSysColor(COLOR_3DDKSHADOW); }     // +0x6c

inline HDC VMW_Hdc(CDC* pDC) { return pDC ? pDC->GetSafeHdc() : nullptr; }

// CDC::Draw3dRect(LPCRECT, COLORREF clrTopLeft, COLORREF clrBottomRight)
// (retail 0x2a3b00): one-pixel top/left edges in clrTopLeft, one-pixel
// bottom/right edges in clrBottomRight.  Open-coded with ::FillRect for the
// same reason CMFCVisualManagerOfficeXP.cpp open-codes it -- CDC's methods are
// not linkable from inside this DLL.
void VMW_Draw3dRect(CDC* pDC, const CRect& rect, COLORREF clrTopLeft, COLORREF clrBottomRight)
{
    HDC hdc = VMW_Hdc(pDC);
    if (hdc == nullptr) return;
    const int cx = rect.right - rect.left;
    const int cy = rect.bottom - rect.top;
    if (cx <= 0 || cy <= 0) return;

    HBRUSH hbrTL = ::CreateSolidBrush(clrTopLeft);
    HBRUSH hbrBR = ::CreateSolidBrush(clrBottomRight);
    if (hbrTL != nullptr) {
        RECT r;
        r.left = rect.left; r.top = rect.top; r.right = rect.right; r.bottom = rect.top + 1;
        ::FillRect(hdc, &r, hbrTL);
        r.right = rect.left + 1; r.bottom = rect.bottom;
        ::FillRect(hdc, &r, hbrTL);
        ::DeleteObject(hbrTL);
    }
    if (hbrBR != nullptr) {
        RECT r;
        r.left = rect.left; r.top = rect.bottom - 1; r.right = rect.right; r.bottom = rect.bottom;
        ::FillRect(hdc, &r, hbrBR);
        r.left = rect.right - 1; r.top = rect.top;
        ::FillRect(hdc, &r, hbrBR);
        ::DeleteObject(hbrBR);
    }
}

} // namespace

// CMFCVisualManagerWindows::GetButtonExtraBorder (retail 0x1b9070) is
//     return CSize(m_hTheme@+0x08 != NULL ? 2 : 2*0, same);
// i.e. the whole body is
//     mov 0x8(%rcx),%rax ; neg %rax ; sbb %ecx,%ecx ; and $0x2,%ecx
//     mov %ecx,(%rdx) ; mov %ecx,0x4(%rdx) ; mov %rdx,%rax
// -- CSize(2,2) when the theme handle at +0x08 is non-NULL, CSize(0,0)
// otherwise, written through the hidden return slot in RDX which is also
// returned in RAX.  The handle is never opened here, so this returns CSize(0,0).
// (The previous body returned nullptr without writing the caller's slot at all,
// which left an MSVC caller's CSize uninitialised.)
// Symbol: ?GetButtonExtraBorder@CMFCVisualManagerWindows@@UEBA?AVCSize@@XZ
extern "C" CSize* MS_ABI impl__GetButtonExtraBorder_CMFCVisualManagerWindows__UEBA_AVCSize__XZ(
    const CMFCVisualManagerWindows* /*pThis*/, CSize* pRet)
{
    if (pRet != nullptr) { pRet->cx = 0; pRet->cy = 0; }
    return pRet;
}

// CMFCVisualManagerWindows::GetCaptionButtonExtraBorder (retail 0x1b9090) is
// the same shape one pixel smaller:
//     xor %eax,%eax ; cmp %rax,0x8(%rcx) ; setne %al
//     mov %eax,(%rdx) ; mov %eax,0x4(%rdx) ; mov %rdx,%rax
// -- CSize(1,1) when the +0x08 theme handle is non-NULL, CSize(0,0) otherwise.
// Symbol: ?GetCaptionButtonExtraBorder@CMFCVisualManagerWindows@@UEBA?AVCSize@@XZ
extern "C" CSize* MS_ABI impl__GetCaptionButtonExtraBorder_CMFCVisualManagerWindows__UEBA_AVCSize__XZ(
    const CMFCVisualManagerWindows* /*pThis*/, CSize* pRet)
{
    if (pRet != nullptr) { pRet->cx = 0; pRet->cy = 0; }
    return pRet;
}

// CMFCVisualManagerWindows::GetHighlightedMenuItemTextColor (retail 0x1b7d80):
//     if (m_hTheme@+0x98 != NULL && this->[+0x210] == 0) {
//         COLORREF clr = 0;
//         ::GetThemeColor(+0x98, 14, 0, 0xedb /*TMT_TEXTCOLOR*/, &clr);  // 0x2c53e8
//         return clr;
//     }
//     if (m_hTheme@+0x08 != NULL && this->[+0x210] == 0)
//         return CMFCVisualManager::GetHighlightedMenuItemTextColor(pButton);   // 0x1833c0
//     return CMFCVisualManagerOfficeXP::GetHighlightedMenuItemTextColor(pButton); // 0x1ae130
// Both theme handles are NULL here, so the last line is the reachable one.
// Symbol: ?GetHighlightedMenuItemTextColor@CMFCVisualManagerWindows@@UEAAKPEAVCMFCToolBarMenuButton@@@Z
extern "C" unsigned long MS_ABI impl__GetHighlightedMenuItemTextColor_CMFCVisualManagerWindows__UEAAKPEAVCMFCToolBarMenuButton___Z(
    CMFCVisualManagerWindows* pThis, CMFCToolBarMenuButton* pButton)
{
    return impl__GetHighlightedMenuItemTextColor_CMFCVisualManagerOfficeXP__MEAAKPEAVCMFCToolBarMenuButton___Z(
        reinterpret_cast<CMFCVisualManagerOfficeXP*>(pThis), pButton);
}

// CMFCVisualManagerWindows::GetToolbarButtonTextColor (retail 0x1b7c60):
//     if (m_hTheme@+0x10 != NULL && !pButton->IsKindOf(RUNTIME_CLASS(CMFCOutlookBarPaneButton)))
//         return CMFCVisualManager::GetToolbarButtonTextColor(pButton, state);   // 0x186830
//     return CMFCVisualManagerOfficeXP::GetToolbarButtonTextColor(pButton, state); // 0x1b0550
// The +0x10 handle is NULL here, so the OfficeXP call is the reachable one and
// the IsKindOf test is never reached.  (The CRuntimeClass at 0x3aa3b8 is not an
// export; its m_lpszClassName was read out of the PE: "CMFCOutlookBarPaneButton".)
// Symbol: ?GetToolbarButtonTextColor@CMFCVisualManagerWindows@@UEAAKPEAVCMFCToolBarButton@@W4AFX_BUTTON_STATE@CMFCVisualManager@@@Z
extern "C" unsigned long MS_ABI impl__GetToolbarButtonTextColor_CMFCVisualManagerWindows__UEAAKPEAVCMFCToolBarButton__W4AFX_BUTTON_STATE_CMFCVisualManager___Z(
    CMFCVisualManagerWindows* pThis, CMFCToolBarButton* pButton, int state)
{
    return impl__GetToolbarButtonTextColor_CMFCVisualManagerOfficeXP__MEAAKPEAVCMFCToolBarButton__W4AFX_BUTTON_STATE_CMFCVisualManager___Z(
        reinterpret_cast<CMFCVisualManagerOfficeXP*>(pThis), pButton, state);
}

// CMFCVisualManagerWindows::IsDefaultWinXPPopupButton (retail 0x1ba640) is
//     return m_hTheme@+0x08 != NULL && pButton->[+0xb24] != 0 && pButton->[+0xb20] != 0;
// STUB, not an implementation: the +0x08 theme handle is never opened here, so
// the correct answer under this tree's model is always FALSE, but that is a
// bare `return FALSE` and the two CMFCDesktopAlertWndButton fields at +0xb20 /
// +0xb24 are not modelled and not named here.
// Symbol: ?IsDefaultWinXPPopupButton@CMFCVisualManagerWindows@@UEBAHPEAVCMFCDesktopAlertWndButton@@@Z
extern "C" int MS_ABI impl__IsDefaultWinXPPopupButton_CMFCVisualManagerWindows__UEBAHPEAVCMFCDesktopAlertWndButton___Z(
    const CMFCVisualManagerWindows* /*pThis*/, CMFCDesktopAlertWndButton* /*pButton*/)
{
    return FALSE;
}

// CMFCVisualManagerWindows::IsWinXPThemeAvailable (retail 0x1b7110), a static:
//     CMFCVisualManager* p = CMFCVisualManager::m_pVisManager;      // 0x3b7120
//     if (p != NULL && p->IsKindOf(RUNTIME_CLASS(CMFCVisualManagerWindows)))
//         return p->[+0x08] != NULL;
//     CMFCVisualManagerWindows vm(1);       // ??0CMFCVisualManagerWindows@@QEAA@H@Z, 0x1b6fe0
//     return vm.[+0x08] != NULL;            // then the inlined destructor
// i.e. "did OpenThemeData succeed for the button class".  STUB: OpenMFC opens
// no theme, so the answer is always FALSE and the body is a bare return.
// Symbol: ?IsWinXPThemeAvailable@CMFCVisualManagerWindows@@SAHXZ
extern "C" int MS_ABI impl__IsWinXPThemeAvailable_CMFCVisualManagerWindows__SAHXZ()
{
    return FALSE;
}

// CMFCVisualManagerWindows::OnDrawBarGripper (retail 0x1b7480):
//     if (m_hTheme@+0x18 == NULL || pBar == NULL || pBar->m_hWnd(+0x40) == NULL)
//         { CMFCVisualManagerOfficeXP::OnDrawBarGripper(pDC, rectGripper, bHorz, pBar); return; }  // 0x1aca30
//     ... otherwise a themed REBAR gripper drawn through +0x18 ...
// The +0x18 handle is NULL here, so the OfficeXP call is the reachable branch.
// Symbol: ?OnDrawBarGripper@CMFCVisualManagerWindows@@UEAAXPEAVCDC@@VCRect@@HPEAVCBasePane@@@Z
extern "C" void MS_ABI impl__OnDrawBarGripper_CMFCVisualManagerWindows__UEAAXPEAVCDC__VCRect__HPEAVCBasePane___Z(
    CMFCVisualManagerWindows* pThis, CDC* pDC, CRect rectGripper, int bHorz, CBasePane* pBar)
{
    impl__OnDrawBarGripper_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__VCRect__HPEAVCBasePane___Z(
        reinterpret_cast<CMFCVisualManagerOfficeXP*>(pThis), pDC, rectGripper, bHorz, pBar);
}

// CMFCVisualManagerWindows::OnDrawBrowseButton (retail 0x1b9c70):
//     if (m_hTheme@+0x20 == NULL)
//         return CMFCVisualManagerOfficeXP::OnDrawBrowseButton(pDC, rect, pEdit, state, clrText); // 0x1b1610
//     ::FillRect(pDC->m_hDC, rect, afxGlobalData.[+0x110]);
//     ::DrawThemeBackground(+0x20, pDC->m_hDC, 1, <state 1..3 from `state`>, rect, NULL);
//     return TRUE;
// The +0x20 handle is NULL here, so the OfficeXP call is the reachable branch
// and its return value is this function's.
// Symbol: ?OnDrawBrowseButton@CMFCVisualManagerWindows@@UEAAHPEAVCDC@@VCRect@@PEAVCMFCEditBrowseCtrl@@W4AFX_BUTTON_STATE@CMFCVisualManager@@AEAK@Z
extern "C" int MS_ABI impl__OnDrawBrowseButton_CMFCVisualManagerWindows__UEAAHPEAVCDC__VCRect__PEAVCMFCEditBrowseCtrl__W4AFX_BUTTON_STATE_CMFCVisualManager__AEAK_Z(
    CMFCVisualManagerWindows* pThis, CDC* pDC, CRect rect, void* pEdit, int state, unsigned long* pclrText)
{
    return impl__OnDrawBrowseButton_CMFCVisualManagerOfficeXP__MEAAHPEAVCDC__VCRect__PEAVCMFCEditBrowseCtrl__W4AFX_BUTTON_STATE_CMFCVisualManager__AEAK_Z(
        reinterpret_cast<CMFCVisualManagerOfficeXP*>(pThis), pDC, rect, pEdit, state, pclrText);
}

// CMFCVisualManagerWindows::OnDrawButtonSeparator (retail 0x1b7ff0):
//     if (m_hTheme@+0x10 == NULL)
//         { CMFCVisualManagerOfficeXP::OnDrawButtonSeparator(pDC, pButton, rect, state, bHorz); return; }  // 0x1b1820
//     ::InflateRect(&rect, 2, 2);
//     ::DrawThemeBackground(+0x10, pDC->m_hDC, bHorz ? 5 : 6, 0, &rect, NULL);
// The +0x10 handle is NULL here, so the OfficeXP call is the reachable branch.
// Symbol: ?OnDrawButtonSeparator@CMFCVisualManagerWindows@@UEAAXPEAVCDC@@PEAVCMFCToolBarButton@@VCRect@@W4AFX_BUTTON_STATE@CMFCVisualManager@@H@Z
extern "C" void MS_ABI impl__OnDrawButtonSeparator_CMFCVisualManagerWindows__UEAAXPEAVCDC__PEAVCMFCToolBarButton__VCRect__W4AFX_BUTTON_STATE_CMFCVisualManager__H_Z(
    CMFCVisualManagerWindows* pThis, CDC* pDC, CMFCToolBarButton* pButton, CRect rect, int state, int bHorz)
{
    impl__OnDrawButtonSeparator_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__PEAVCMFCToolBarButton__VCRect__W4AFX_BUTTON_STATE_CMFCVisualManager__H_Z(
        reinterpret_cast<CMFCVisualManagerOfficeXP*>(pThis), pDC, pButton, rect, state, bHorz);
}

// CMFCVisualManagerWindows::OnDrawCaptionButton (retail 0x1b8240):
//     if (m_hTheme@+0x10 == NULL)
//         { CMFCVisualManagerOfficeXP::OnDrawCaptionButton(pDC, pButton, p2, p3, p4, p5, p6); return; } // 0x1aea90
//     ... otherwise a themed caption button drawn through +0x10 ...
// The +0x10 handle is NULL here, so the OfficeXP call is the reachable branch;
// all five trailing BOOLs are forwarded unchanged.
// Symbol: ?OnDrawCaptionButton@CMFCVisualManagerWindows@@UEAAXPEAVCDC@@PEAVCMFCCaptionButton@@HHHHH@Z
extern "C" void MS_ABI impl__OnDrawCaptionButton_CMFCVisualManagerWindows__UEAAXPEAVCDC__PEAVCMFCCaptionButton__HHHHH_Z(
    CMFCVisualManagerWindows* pThis, CDC* pDC, CMFCCaptionButton* pButton, int p2, int p3, int p4, int p5, int p6)
{
    impl__OnDrawCaptionButton_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__PEAVCMFCCaptionButton__HHHHH_Z(
        reinterpret_cast<CMFCVisualManagerOfficeXP*>(pThis), pDC, pButton, p2, p3, p4, p5, p6);
}

// CMFCVisualManagerWindows::OnDrawCaptionButtonIcon (retail 0x1b8460):
//     BOOL bPushed = pButton->[+0x0c] != 0 || pButton->[+0x18] != 0;
//     if (m_hTheme@+0x20 != NULL && bPushed) {
//         CMenuImages::Draw(pDC, id, ptImage, 0 /*IMAGE_STATE*/, CSize(0,0)); // 0x8fc50
//         return;
//     }
//     CMFCVisualManagerOfficeXP::OnDrawCaptionButtonIcon(pDC, pButton, id, bHorz, bMaximized, ptImage); // 0x1aec70
// The +0x20 handle is NULL here, so the OfficeXP call is the reachable branch.
// Symbol: ?OnDrawCaptionButtonIcon@CMFCVisualManagerWindows@@UEAAXPEAVCDC@@PEAVCMFCCaptionButton@@W4IMAGES_IDS@CMenuImages@@HHVCPoint@@@Z
extern "C" void MS_ABI impl__OnDrawCaptionButtonIcon_CMFCVisualManagerWindows__UEAAXPEAVCDC__PEAVCMFCCaptionButton__W4IMAGES_IDS_CMenuImages__HHVCPoint___Z(
    CMFCVisualManagerWindows* pThis, CDC* pDC, CMFCCaptionButton* pButton, int id, int bHorz, int bMaximized,
    long long ptImage)
{
    impl__OnDrawCaptionButtonIcon_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__PEAVCMFCCaptionButton__W4IMAGES_IDS_CMenuImages__HHVCPoint___Z(
        reinterpret_cast<CMFCVisualManagerOfficeXP*>(pThis), pDC, pButton, id, bHorz, bMaximized, ptImage);
}

// CMFCVisualManagerWindows::OnDrawCheckBoxEx (retail 0x195f50) is the one
// override in this class whose gate is a virtual call instead of a theme-handle
// test:
//     if (!this->vtbl[+0x50](pDC, rect, bHighlighted, nState, bEnabled, bPressed))
//         CMFCVisualManager::OnDrawCheckBoxEx(pDC, rect, nState, bHighlighted, bPressed, bEnabled); // 0x1886a0
// Vtable slot +0x50 is CMFCBaseVisualManager::DrawCheckBox (read out of the
// retail CMFCVisualManagerWindows vftable).  Note the argument reordering: the
// DrawCheckBox call passes (bHighlighted, nState, bEnabled, bPressed) while the
// base OnDrawCheckBoxEx call passes this function's own order.  DrawCheckBox is
// a documented no-theme stub returning FALSE in this tree, so the base call
// runs -- which is also what retail does with no checkbox theme open.
// Symbol: ?OnDrawCheckBoxEx@CMFCVisualManagerWindows@@UEAAXPEAVCDC@@VCRect@@HHHH@Z
extern "C" void MS_ABI impl__OnDrawCheckBoxEx_CMFCVisualManagerWindows__UEAAXPEAVCDC__VCRect__HHHH_Z(
    CMFCVisualManagerWindows* pThis, CDC* pDC, CRect rect, int nState, int bHighlighted, int bPressed, int bEnabled)
{
    if (!impl__DrawCheckBox_CMFCBaseVisualManager__UEAAHPEAVCDC__VCRect__HHHH_Z(
            pThis, pDC, rect, bHighlighted, nState, bEnabled, bPressed)) {
        impl__OnDrawCheckBoxEx_CMFCVisualManager__UEAAXPEAVCDC__VCRect__HHHH_Z(
            pThis, pDC, rect, nState, bHighlighted, bPressed, bEnabled);
    }
}

// CMFCVisualManagerWindows::OnDrawComboBorder (retail 0x1b88e0):
//     if (m_hTheme@+0x08 == NULL)
//         { CMFCVisualManagerOfficeXP::OnDrawComboBorder(pDC, rect, bDisabled, bIsDropped, bIsHighlighted, pButton); return; } // 0x1afc80
//     if (!bIsHighlighted && !bIsDropped) return;
//     ::InflateRect(&rect, -1, -1);
//     pDC->Draw3dRect(rect, afxGlobalData.clrHilite(+0x48), afxGlobalData.clrHilite(+0x48));
// The +0x08 handle is NULL here, so the OfficeXP call is the reachable branch.
// Symbol: ?OnDrawComboBorder@CMFCVisualManagerWindows@@UEAAXPEAVCDC@@VCRect@@HHHPEAVCMFCToolBarComboBoxButton@@@Z
extern "C" void MS_ABI impl__OnDrawComboBorder_CMFCVisualManagerWindows__UEAAXPEAVCDC__VCRect__HHHPEAVCMFCToolBarComboBoxButton___Z(
    CMFCVisualManagerWindows* pThis, CDC* pDC, CRect rect, int bDisabled, int bIsDropped, int bIsHighlighted,
    CMFCToolBarComboBoxButton* pButton)
{
    impl__OnDrawComboBorder_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__VCRect__HHHPEAVCMFCToolBarComboBoxButton___Z(
        reinterpret_cast<CMFCVisualManagerOfficeXP*>(pThis), pDC, rect, bDisabled, bIsDropped, bIsHighlighted, pButton);
}

// CMFCVisualManagerWindows::OnDrawControlBorder (retail 0x1955a0):
//     if (m_hTheme@+0x30 == NULL)
//         { CMFCVisualManager::OnDrawControlBorder(pWndCtrl); return; }   // 0x188e00
//     CWindowDC dc(pWndCtrl); ::GetWindowRect(pWndCtrl->m_hWnd, &rect); ...
//     ::GetThemeColor(+0x30, 5, 0, 0xed9 /*TMT_BORDERCOLOR*/, &clr) ...
// The +0x30 handle is NULL here, so the base call is the reachable branch.
// Symbol: ?OnDrawControlBorder@CMFCVisualManagerWindows@@UEAAXPEAVCWnd@@@Z
extern "C" void MS_ABI impl__OnDrawControlBorder_CMFCVisualManagerWindows__UEAAXPEAVCWnd___Z(
    CMFCVisualManagerWindows* pThis, CWnd* pWndCtrl)
{
    impl__OnDrawControlBorder_CMFCVisualManager__UEAAXPEAVCWnd___Z(pThis, pWndCtrl);
}

// CMFCVisualManagerWindows::OnDrawEditBorder (retail 0x1b91e0):
//     if (m_hTheme@+0x08 == NULL)
//         { CMFCVisualManagerOfficeXP::OnDrawEditBorder(pDC, rect, bDisabled, bIsHighlighted, pButton); return; } // 0x1b06d0
//     if (!bIsHighlighted) return;
//     pDC->Draw3dRect(rect, afxGlobalData.clrBtnHilite(+0x30), afxGlobalData.clrBtnHilite(+0x30));
// The +0x08 handle is NULL here, so the OfficeXP call is the reachable branch.
// Symbol: ?OnDrawEditBorder@CMFCVisualManagerWindows@@UEAAXPEAVCDC@@VCRect@@HHPEAVCMFCToolBarEditBoxButton@@@Z
extern "C" void MS_ABI impl__OnDrawEditBorder_CMFCVisualManagerWindows__UEAAXPEAVCDC__VCRect__HHPEAVCMFCToolBarEditBoxButton___Z(
    CMFCVisualManagerWindows* pThis, CDC* pDC, CRect rect, int bDisabled, int bIsHighlighted,
    CMFCToolBarEditBoxButton* pButton)
{
    impl__OnDrawEditBorder_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__VCRect__HHPEAVCMFCToolBarEditBoxButton___Z(
        reinterpret_cast<CMFCVisualManagerOfficeXP*>(pThis), pDC, rect, bDisabled, bIsHighlighted, pButton);
}

// CMFCVisualManagerWindows::OnDrawExpandingBox (retail 0x1956f0):
//     if (m_hTheme@+0x58 == NULL)
//         { CMFCVisualManager::OnDrawExpandingBox(pDC, rect, bIsOpened, colorBox); return; }  // 0x188ce0
//     ::DrawThemeBackground(+0x58, pDC->m_hDC, 2 /*TVP_GLYPH*/, bIsOpened ? 2 : 1, &rect, NULL);
// The +0x58 handle is NULL here, so the base call is the reachable branch.
// Symbol: ?OnDrawExpandingBox@CMFCVisualManagerWindows@@UEAAXPEAVCDC@@VCRect@@HK@Z
extern "C" void MS_ABI impl__OnDrawExpandingBox_CMFCVisualManagerWindows__UEAAXPEAVCDC__VCRect__HK_Z(
    CMFCVisualManagerWindows* pThis, CDC* pDC, CRect rect, int bIsOpened, unsigned long colorBox)
{
    impl__OnDrawExpandingBox_CMFCVisualManager__UEAAXPEAVCDC__VCRect__HK_Z(pThis, pDC, rect, bIsOpened, colorBox);
}

// CMFCVisualManagerWindows::OnDrawFloatingToolbarBorder (retail 0x1b8e40):
//     if (m_hTheme@+0x08 == NULL)
//         { CMFCVisualManager::OnDrawFloatingToolbarBorder(pDC, pToolBar, rectBorder, rectBorderSize); return; } // 0x1866f0
//     ::DrawThemeBackground(+0x08, pDC->m_hDC, 2 /*WP_SMALLCAPTION*/, 0, &rectBorder, NULL);
//     pDC->Draw3dRect(rectBorder, afxGlobalData.clrBarFace(+0x60), afxGlobalData.clrBarDkShadow(+0x6c));
//     ::InflateRect(&rectBorder, -1, -1);
//     pDC->Draw3dRect(rectBorder, afxGlobalData.clrBarHilite(+0x68), afxGlobalData.clrBarShadow(+0x64));
// (two Draw3dRect calls, at 0x1b8f07 and 0x1b8f6c, with the colour reads at
// 0x1b8ef7/0x1b8ed7 and 0x1b8f5c/0x1b8f3c).
// The +0x08 handle is NULL here, so the base call is the reachable branch.
// Symbol: ?OnDrawFloatingToolbarBorder@CMFCVisualManagerWindows@@UEAAXPEAVCDC@@PEAVCMFCBaseToolBar@@VCRect@@2@Z
extern "C" void MS_ABI impl__OnDrawFloatingToolbarBorder_CMFCVisualManagerWindows__UEAAXPEAVCDC__PEAVCMFCBaseToolBar__VCRect__2_Z(
    CMFCVisualManagerWindows* pThis, CDC* pDC, CMFCBaseToolBar* pToolBar, CRect rectBorder, CRect rectBorderSize)
{
    impl__OnDrawFloatingToolbarBorder_CMFCVisualManager__UEAAXPEAVCDC__PEAVCMFCBaseToolBar__VCRect__2_Z(
        pThis, pDC, pToolBar, rectBorder, rectBorderSize);
}

// CMFCVisualManagerWindows::OnDrawHeaderCtrlBorder (retail 0x195a50) is a tail
// call in both directions:
//     if (m_hTheme@+0x40 == NULL)
//         jmp CMFCVisualManager::OnDrawHeaderCtrlBorder(pCtrl, pDC, rect, bIsPressed, bIsHighlighted); // 0x1872d0
//     jmp ::DrawThemeBackground(+0x40, pDC->m_hDC, 1 /*HP_HEADERITEM*/,
//                               bIsPressed ? 3 : (bIsHighlighted ? 2 : 1), &rect, NULL);
// The +0x40 handle is NULL here, so the base call is the reachable branch.
// Symbol: ?OnDrawHeaderCtrlBorder@CMFCVisualManagerWindows@@UEAAXPEAVCMFCHeaderCtrl@@PEAVCDC@@AEAVCRect@@HH@Z
extern "C" void MS_ABI impl__OnDrawHeaderCtrlBorder_CMFCVisualManagerWindows__UEAAXPEAVCMFCHeaderCtrl__PEAVCDC__AEAVCRect__HH_Z(
    CMFCVisualManagerWindows* pThis, CMFCHeaderCtrl* pCtrl, CDC* pDC, CRect* pRect, int bIsPressed, int bIsHighlighted)
{
    impl__OnDrawHeaderCtrlBorder_CMFCVisualManager__UEAAXPEAVCMFCHeaderCtrl__PEAVCDC__AEAVCRect__HH_Z(
        pThis, pCtrl, pDC, pRect, bIsPressed, bIsHighlighted);
}

// CMFCVisualManagerWindows::OnDrawHeaderCtrlSortArrow (retail 0x1b90b0):
//     if (m_hTheme@+0x40 == NULL)
//         { CMFCVisualManager::OnDrawHeaderCtrlSortArrow(pCtrl, pDC, rect, bIsAscending); return; } // 0x187450
//     ... otherwise it builds the arrow polygon and calls ::Polygon through a
//     selected pen/brush pair ...
// The +0x40 handle is NULL here, so the base call is the reachable branch.
// Symbol: ?OnDrawHeaderCtrlSortArrow@CMFCVisualManagerWindows@@UEAAXPEAVCMFCHeaderCtrl@@PEAVCDC@@AEAVCRect@@H@Z
extern "C" void MS_ABI impl__OnDrawHeaderCtrlSortArrow_CMFCVisualManagerWindows__UEAAXPEAVCMFCHeaderCtrl__PEAVCDC__AEAVCRect__H_Z(
    CMFCVisualManagerWindows* pThis, CMFCHeaderCtrl* pCtrl, CDC* pDC, CRect* pRect, int bIsAscending)
{
    impl__OnDrawHeaderCtrlSortArrow_CMFCVisualManager__UEAAXPEAVCMFCHeaderCtrl__PEAVCDC__AEAVCRect__H_Z(
        pThis, pCtrl, pDC, pRect, bIsAscending);
}

// CMFCVisualManagerWindows::OnDrawMenuSystemButton (retail 0x1b8630):
//     if (m_hTheme@+0x08 == NULL)
//         { CMFCVisualManagerOfficeXP::OnDrawMenuSystemButton(pDC, rect, uiSystemCommand, nStyle, bHighlight); return; } // 0x1af880
//     part = 0x10 for uiSystemCommand 0xf020 (SC_MINIMIZE), 0x14 for 0xf060
//            (SC_CLOSE), 0x16 for 0xf120 (SC_RESTORE); every other value returns
//            without drawing;
//     state = (nStyle & 0x40000) ? 4 : ((nStyle & 0x20000) && bHighlight) ? 3
//                                    : (bHighlight ? 2 : 1);
//     ::DrawThemeBackground(+0x08, pDC->m_hDC, part, state, &rect, NULL);
// The +0x08 handle is NULL here, so the OfficeXP call is the reachable branch.
// Symbol: ?OnDrawMenuSystemButton@CMFCVisualManagerWindows@@UEAAXPEAVCDC@@VCRect@@IIH@Z
extern "C" void MS_ABI impl__OnDrawMenuSystemButton_CMFCVisualManagerWindows__UEAAXPEAVCDC__VCRect__IIH_Z(
    CMFCVisualManagerWindows* pThis, CDC* pDC, CRect rect, unsigned int uiSystemCommand, unsigned int nStyle,
    int bHighlight)
{
    impl__OnDrawMenuSystemButton_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__VCRect__IIH_Z(
        reinterpret_cast<CMFCVisualManagerOfficeXP*>(pThis), pDC, rect, uiSystemCommand, nStyle, bHighlight);
}

// CMFCVisualManagerWindows::OnDrawMiniFrameBorder (retail 0x1b8d00):
//     if (m_hTheme@+0x08 == NULL)
//         { CMFCVisualManager::OnDrawMiniFrameBorder(pDC, pFrameWnd, rectBorder, rectBorderSize); return; } // 0x1864c0
//     ::DrawThemeBackground(+0x08, pDC->m_hDC, 2 /*WP_SMALLCAPTION*/, 0, &rectBorder, NULL);
//     pDC->Draw3dRect(rectBorder, afxGlobalData.clrBarFace(+0x60), afxGlobalData.clrBarDkShadow(+0x6c));
//     ::InflateRect(&rectBorder, -1, -1);
//     pDC->Draw3dRect(rectBorder, afxGlobalData.clrBarHilite(+0x68), afxGlobalData.clrBarShadow(+0x64));
// (the same two-frame sequence as OnDrawFloatingToolbarBorder; Draw3dRect calls
// at 0x1b8dca and 0x1b8e2f).
// The +0x08 handle is NULL here, so the base call is the reachable branch.
// Symbol: ?OnDrawMiniFrameBorder@CMFCVisualManagerWindows@@UEAAXPEAVCDC@@PEAVCPaneFrameWnd@@VCRect@@2@Z
extern "C" void MS_ABI impl__OnDrawMiniFrameBorder_CMFCVisualManagerWindows__UEAAXPEAVCDC__PEAVCPaneFrameWnd__VCRect__2_Z(
    CMFCVisualManagerWindows* pThis, CDC* pDC, CPaneFrameWnd* pFrameWnd, CRect rectBorder, CRect rectBorderSize)
{
    impl__OnDrawMiniFrameBorder_CMFCVisualManager__UEAAXPEAVCDC__PEAVCPaneFrameWnd__VCRect__2_Z(
        pThis, pDC, pFrameWnd, rectBorder, rectBorderSize);
}

// CMFCVisualManagerWindows::OnDrawOutlookPageButtonBorder (retail 0x1b9050) --
// the whole body is
//     if (m_hTheme@+0x20 == NULL)
//         CMFCVisualManager::OnDrawOutlookPageButtonBorder(pDC, rect, bIsHighlighted, bIsPressed); // 0x186a10
// When the +0x20 handle IS open retail draws nothing here at all (the themed
// button face is painted elsewhere).  The handle is NULL here, so the base call
// is the reachable branch.
// Symbol: ?OnDrawOutlookPageButtonBorder@CMFCVisualManagerWindows@@UEAAXPEAVCDC@@AEAVCRect@@HH@Z
extern "C" void MS_ABI impl__OnDrawOutlookPageButtonBorder_CMFCVisualManagerWindows__UEAAXPEAVCDC__AEAVCRect__HH_Z(
    CMFCVisualManagerWindows* pThis, CDC* pDC, CRect* pRect, int bIsHighlighted, int bIsPressed)
{
    impl__OnDrawOutlookPageButtonBorder_CMFCVisualManager__UEAAXPEAVCDC__AEAVCRect__HH_Z(
        pThis, pDC, pRect, bIsHighlighted, bIsPressed);
}

// CMFCVisualManagerWindows::OnDrawPaneBorder (retail 0x1b7ac0) is a single
// unconditional tail jump -- `jmp 0x180182df0` -- to
// CMFCVisualManager::OnDrawPaneBorder.  There is no theme test and no other
// code in the function; the override exists only to skip
// CMFCVisualManagerOfficeXP's version.
// Symbol: ?OnDrawPaneBorder@CMFCVisualManagerWindows@@UEAAXPEAVCDC@@PEAVCBasePane@@AEAVCRect@@@Z
extern "C" void MS_ABI impl__OnDrawPaneBorder_CMFCVisualManagerWindows__UEAAXPEAVCDC__PEAVCBasePane__AEAVCRect___Z(
    CMFCVisualManagerWindows* pThis, CDC* pDC, CBasePane* pBar, CRect* pRect)
{
    impl__OnDrawPaneBorder_CMFCVisualManager__UEAAXPEAVCDC__PEAVCBasePane__AEAVCRect___Z(pThis, pDC, pBar, pRect);
}

// CMFCVisualManagerWindows::OnDrawPaneCaption (retail 0x1ba660):
//     if (m_hTheme@+0x50 == NULL)
//         return CMFCVisualManagerOfficeXP::OnDrawPaneCaption(pDC, pBar, bActive, rectCaption, rectButtons); // 0x1ae8e0
//     CDrawingManager dm(*pDC);
//     dm.FillGradient(rectCaption,                                       // 0x57960
//                     /*colorStart*/  bActive ? afxGlobalData.[+0x94] : afxGlobalData.[+0x98],
//                     /*colorFinish*/ bActive ? afxGlobalData.[+0x88] : afxGlobalData.[+0x8c],
//                     /*bHorz*/ TRUE, 0, 0);
//     return bActive ? afxGlobalData.[+0x80] : afxGlobalData.[+0x90];
// (r8d = +0x94/+0x98 and r9d = +0x88/+0x8c at the call at 0x1ba780; only the
// return value comes from +0x80/+0x90).
// The +0x50 handle is NULL here, so the OfficeXP call is the reachable branch
// and its COLORREF is this function's result.
// Symbol: ?OnDrawPaneCaption@CMFCVisualManagerWindows@@UEAAKPEAVCDC@@PEAVCDockablePane@@HVCRect@@2@Z
extern "C" unsigned long MS_ABI impl__OnDrawPaneCaption_CMFCVisualManagerWindows__UEAAKPEAVCDC__PEAVCDockablePane__HVCRect__2_Z(
    CMFCVisualManagerWindows* pThis, CDC* pDC, CDockablePane* pBar, int bActive, CRect rectCaption, CRect rectButtons)
{
    return impl__OnDrawPaneCaption_CMFCVisualManagerOfficeXP__MEAAKPEAVCDC__PEAVCDockablePane__HVCRect__2_Z(
        reinterpret_cast<CMFCVisualManagerOfficeXP*>(pThis), pDC, pBar, bActive, rectCaption, rectButtons);
}

// CMFCVisualManagerWindows::OnDrawPopupWindowButtonBorder (retail 0x1ba510)
// first computes a UxTheme push-button state from CWnd::IsWindowEnabled
// (0x2a7a10), the button fields at +0x114 and +0x11c (named m_bHover and
// m_bClickStarted in the sibling CMFCVisualManagerOfficeXP.cpp comments; not
// re-verified here) and a BM_GETCHECK ::SendMessage, then:
//     if (m_hTheme@+0x08 != NULL && pButton->[+0xb24] != 0 && pButton->[+0xb20] != 0)
//         ::DrawThemeBackground(+0x08, ..., 0x13, state, rect, NULL);
//     else if (m_hTheme@+0x20 == NULL)
//         CMFCVisualManagerOfficeXP::OnDrawPopupWindowButtonBorder(pDC, rectClient, pButton); // 0x195f00
//     else
//         AFX_GLOBAL_DATA::DrawParentBackground(pButton, pDC, rectClient) then
//         ::DrawThemeBackground(+0x20, ..., 1, state, ...);
// Both handles are NULL here, so the OfficeXP call is the reachable branch and
// the computed state is dead on it.
// Symbol: ?OnDrawPopupWindowButtonBorder@CMFCVisualManagerWindows@@UEAAXPEAVCDC@@VCRect@@PEAVCMFCDesktopAlertWndButton@@@Z
extern "C" void MS_ABI impl__OnDrawPopupWindowButtonBorder_CMFCVisualManagerWindows__UEAAXPEAVCDC__VCRect__PEAVCMFCDesktopAlertWndButton___Z(
    CMFCVisualManagerWindows* pThis, CDC* pDC, CRect rectClient, CMFCDesktopAlertWndButton* pButton)
{
    impl__OnDrawPopupWindowButtonBorder_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__VCRect__PEAVCMFCDesktopAlertWndButton___Z(
        reinterpret_cast<CMFCVisualManagerOfficeXP*>(pThis), pDC, rectClient, pButton);
}

// CMFCVisualManagerWindows::OnDrawScrollButtons (retail 0x188110).  This one has
// no theme test at all -- it is a plain GDI body, transcribed here:
//     CSize sizeImage = CMenuImages::Size();          // 0x8fbb0; retail computes
//                                                     // it and never uses it
//     CRect rectFill = rect;  rectFill.top -= nBorderSize;
//     ::FillRect(pDC->m_hDC, &rectFill, afxGlobalData.brBarFace);
//         -- the HBRUSH read at afxGlobalData+0x120, i.e. the m_hObject of the
//            CBrush at +0x118, which AFX_GLOBAL_DATA::UpdateSysColors (0x6afd0)
//            creates with ::CreateSolidBrush(clrBarFace, +0x60) at 0x6b25e.
//     if (bHilited) {
//         CDrawingManager dm(*pDC);
//         dm.HighlightRect(rect, -1, (COLORREF)-1, 0, (COLORREF)-1);   // 0x56750
//         pDC->Draw3dRect(rect, afxGlobalData.clrBarHilite(+0x68),
//                               afxGlobalData.clrBarDkShadow(+0x6c));  // 0x2a3b00
//     }
//     CMenuImages::Draw(pDC, (IMAGES_IDS)iImage, rect, 0 /*IMAGE_STATE*/, CSize(0,0)); // 0x8fd40
// DEVIATIONS, both stated: (1) the cached afxGlobalData brush is replaced by a
// temporary ::CreateSolidBrush of ::GetSysColor(COLOR_BTNFACE), which is the
// colour UpdateSysColors puts in it; (2) the CDrawingManager::HighlightRect step
// is omitted, because CDrawingManager is not declared anywhere under
// include/openmfc (there is no class to construct) and its exported HighlightRect
// (phase4/src/core/gdi/CDrawingManager.cpp) is a stub that returns 0 and draws
// nothing, so the omission changes no pixel today.  The dead CMenuImages::Size
// call is omitted as well.
// Symbol: ?OnDrawScrollButtons@CMFCVisualManagerWindows@@UEAAXPEAVCDC@@AEBVCRect@@HHH@Z
extern "C" void MS_ABI impl__OnDrawScrollButtons_CMFCVisualManagerWindows__UEAAXPEAVCDC__AEBVCRect__HHH_Z(
    CMFCVisualManagerWindows* /*pThis*/, CDC* pDC, const CRect* pRect, int nBorderSize, int iImage, int bHilited)
{
    if (pDC == nullptr || pRect == nullptr) return;

    const HDC hdc = VMW_Hdc(pDC);

    CRect rectFill(*pRect);
    rectFill.top -= nBorderSize;
    if (hdc != nullptr) {
        HBRUSH hbrBarFace = ::CreateSolidBrush(GD_clrBarFace());
        if (hbrBarFace != nullptr) {
            RECT r;
            r.left = rectFill.left; r.top = rectFill.top;
            r.right = rectFill.right; r.bottom = rectFill.bottom;
            ::FillRect(hdc, &r, hbrBarFace);
            ::DeleteObject(hbrBarFace);
        }
    }

    if (bHilited) {
        VMW_Draw3dRect(pDC, *pRect, GD_clrBarHilite(), GD_clrBarDkShadow());
    }

    const CSize sizeZero(0, 0);
    impl__Draw_CMenuImages__SAXPEAVCDC__W4IMAGES_IDS_1_AEBVCRect__W4IMAGE_STATE_1_AEBVCSize___Z(
        pDC, iImage, pRect, 0 /* the IMAGE_STATE retail passes */, &sizeZero);
}

// CMFCVisualManagerWindows::OnDrawSeparator (retail 0x1b8090):
//     if (m_hTheme@+0x10 == NULL || pBar->[+0xf4] != 0)
//         { CMFCVisualManagerOfficeXP::OnDrawSeparator(pDC, pBar, rect, bHorz); return; }  // 0x1ada00
//     ... otherwise a themed separator drawn through +0x10, with an extra
//     CObject::IsKindOf test on pBar ...
// The +0x10 handle is NULL here, so the OfficeXP call is the reachable branch.
// Symbol: ?OnDrawSeparator@CMFCVisualManagerWindows@@UEAAXPEAVCDC@@PEAVCBasePane@@VCRect@@H@Z
extern "C" void MS_ABI impl__OnDrawSeparator_CMFCVisualManagerWindows__UEAAXPEAVCDC__PEAVCBasePane__VCRect__H_Z(
    CMFCVisualManagerWindows* pThis, CDC* pDC, CBasePane* pBar, CRect rect, int bHorz)
{
    impl__OnDrawSeparator_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__PEAVCBasePane__VCRect__H_Z(
        reinterpret_cast<CMFCVisualManagerOfficeXP*>(pThis), pDC, pBar, rect, bHorz);
}

// CMFCVisualManagerWindows::OnDrawSpinButtons (retail 0x1b9d40):
//     if (m_hTheme@+0x78 == NULL)
//         { CMFCVisualManagerOfficeXP::OnDrawSpinButtons(pDC, rectSpin, nState, bOrientation, pSpinCtrl); return; } // 0x1b10a0
//     ... otherwise it splits rectSpin and draws the two themed spin parts ...
// The +0x78 handle is NULL here, so the OfficeXP call is the reachable branch.
// Symbol: ?OnDrawSpinButtons@CMFCVisualManagerWindows@@UEAAXPEAVCDC@@VCRect@@HHPEAVCMFCSpinButtonCtrl@@@Z
extern "C" void MS_ABI impl__OnDrawSpinButtons_CMFCVisualManagerWindows__UEAAXPEAVCDC__VCRect__HHPEAVCMFCSpinButtonCtrl___Z(
    CMFCVisualManagerWindows* pThis, CDC* pDC, CRect rectSpin, int nState, int bOrientation,
    CMFCSpinButtonCtrl* pSpinCtrl)
{
    impl__OnDrawSpinButtons_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__VCRect__HHPEAVCMFCSpinButtonCtrl___Z(
        reinterpret_cast<CMFCVisualManagerOfficeXP*>(pThis), pDC, rectSpin, nState, bOrientation, pSpinCtrl);
}

// CMFCVisualManagerWindows::OnDrawStatusBarPaneBorder (retail 0x1b8700):
//     if (m_hTheme@+0x28 == NULL)
//         { CMFCVisualManagerOfficeXP::OnDrawStatusBarPaneBorder(pDC, pBar, rectPane, uiID, nStyle); return; } // 0x1af990
//     if (nStyle & 0x100 /*SBPS_NOBORDERS*/) return;
//     ::DrawThemeBackground(+0x28, pDC->m_hDC, 1 /*SP_PANE*/, 0, &rectPane, NULL);
// The +0x28 handle is NULL here, so the OfficeXP call is the reachable branch.
// Symbol: ?OnDrawStatusBarPaneBorder@CMFCVisualManagerWindows@@UEAAXPEAVCDC@@PEAVCMFCStatusBar@@VCRect@@II@Z
extern "C" void MS_ABI impl__OnDrawStatusBarPaneBorder_CMFCVisualManagerWindows__UEAAXPEAVCDC__PEAVCMFCStatusBar__VCRect__II_Z(
    CMFCVisualManagerWindows* pThis, CDC* pDC, CMFCStatusBar* pBar, CRect rectPane, unsigned int uiID,
    unsigned int nStyle)
{
    impl__OnDrawStatusBarPaneBorder_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__PEAVCMFCStatusBar__VCRect__II_Z(
        reinterpret_cast<CMFCVisualManagerOfficeXP*>(pThis), pDC, pBar, rectPane, uiID, nStyle);
}

// CMFCVisualManagerWindows::OnDrawStatusBarSizeBox (retail 0x1958e0):
//     if (m_hTheme@+0x48 == NULL)
//         { CMFCVisualManager::OnDrawStatusBarSizeBox(pDC, pBar, rectSizeBox); return; } // 0x187490
//     ::DrawThemeBackground(+0x48, pDC->m_hDC, 10 /*SBP_SIZEBOX*/, 1 /*SZB_RIGHTALIGN*/,
//                           &rectSizeBox, NULL);
// (+0x48 is the SCROLLBAR theme, so part 10 is SBP_SIZEBOX, not a STATUS part.)
// The +0x48 handle is NULL here, so the base call is the reachable branch.
// Symbol: ?OnDrawStatusBarSizeBox@CMFCVisualManagerWindows@@UEAAXPEAVCDC@@PEAVCMFCStatusBar@@VCRect@@@Z
extern "C" void MS_ABI impl__OnDrawStatusBarSizeBox_CMFCVisualManagerWindows__UEAAXPEAVCDC__PEAVCMFCStatusBar__VCRect___Z(
    CMFCVisualManagerWindows* pThis, CDC* pDC, CMFCStatusBar* pBar, CRect rectSizeBox)
{
    impl__OnDrawStatusBarSizeBox_CMFCVisualManager__UEAAXPEAVCDC__PEAVCMFCStatusBar__VCRect___Z(
        pThis, pDC, pBar, rectSizeBox);
}

// CMFCVisualManagerWindows::OnDrawTab (retail 0x1b9ef0):
//     if (!CMFCVisualManagerWindows::m_b3DTabsXPTheme || m_hTheme@+0x80 == NULL
//         || <FOUR CMFCBaseTabCtrl virtuals at its vtable +0x510/+0x520/+0x528/+0x530,
//             any returning non-zero>)
//         { CMFCVisualManagerOfficeXP::OnDrawTab(pDC, rectTab, iTab, bIsActive, pTabWnd); return; } // 0x1ae260
//     ... otherwise a themed tab item ...
// (This gate has one more virtual, +0x530, than the OnEraseTabs* gates below.)
// m_b3DTabsXPTheme is TRUE here (the constructor above sets it), but the +0x80
// handle is NULL, so the OfficeXP call is the reachable branch either way.
// Symbol: ?OnDrawTab@CMFCVisualManagerWindows@@UEAAXPEAVCDC@@VCRect@@HHPEBVCMFCBaseTabCtrl@@@Z
extern "C" void MS_ABI impl__OnDrawTab_CMFCVisualManagerWindows__UEAAXPEAVCDC__VCRect__HHPEBVCMFCBaseTabCtrl___Z(
    CMFCVisualManagerWindows* pThis, CDC* pDC, CRect rectTab, int iTab, int bIsActive,
    const CMFCBaseTabCtrl* pTabWnd)
{
    impl__OnDrawTab_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__VCRect__HHPEBVCMFCBaseTabCtrl___Z(
        reinterpret_cast<CMFCVisualManagerOfficeXP*>(pThis), pDC, rectTab, iTab, bIsActive, pTabWnd);
}

// CMFCVisualManagerWindows::OnDrawTabCloseButton (retail 0x1ba120):
//     if (m_hTheme@+0x08 == NULL)
//         { CMFCVisualManagerOfficeXP::OnDrawTabCloseButton(pDC, rect, pTabWnd, bIsHilited, bIsPressed, bIsDisabled); return; } // 0x1afce0
//     state = bIsDisabled ? 4 : ((bIsPressed && bIsHilited) ? 3 : (bIsHilited ? 2 : 1));
//     ::DrawThemeBackground(+0x08, pDC->m_hDC, 0x13, state, &rect, NULL);
// The +0x08 handle is NULL here, so the OfficeXP call is the reachable branch.
// Symbol: ?OnDrawTabCloseButton@CMFCVisualManagerWindows@@UEAAXPEAVCDC@@VCRect@@PEBVCMFCBaseTabCtrl@@HHH@Z
extern "C" void MS_ABI impl__OnDrawTabCloseButton_CMFCVisualManagerWindows__UEAAXPEAVCDC__VCRect__PEBVCMFCBaseTabCtrl__HHH_Z(
    CMFCVisualManagerWindows* pThis, CDC* pDC, CRect rect, const CMFCBaseTabCtrl* pTabWnd,
    int bIsHilited, int bIsPressed, int bIsDisabled)
{
    impl__OnDrawTabCloseButton_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__VCRect__PEBVCMFCBaseTabCtrl__HHH_Z(
        reinterpret_cast<CMFCVisualManagerOfficeXP*>(pThis), pDC, rect, pTabWnd, bIsHilited, bIsPressed, bIsDisabled);
}

// CMFCVisualManagerWindows::OnDrawTabsButtonBorder (retail 0x1b8ba0):
//     if (m_hTheme@+0x10 == NULL)
//         { CMFCVisualManagerOfficeXP::OnDrawTabsButtonBorder(pDC, rect, pButton, uiState, pWndTab); return; } // 0x1afed0
//     ... otherwise a themed toolbar button drawn through +0x10, with the state
//     taken from CWnd::IsWindowEnabled (0x2a7a10) and the button fields at
//     +0x114 / +0x11c ...
// The +0x10 handle is NULL here, so the OfficeXP call is the reachable branch.
// Symbol: ?OnDrawTabsButtonBorder@CMFCVisualManagerWindows@@UEAAXPEAVCDC@@AEAVCRect@@PEAVCMFCButton@@IPEAVCMFCBaseTabCtrl@@@Z
extern "C" void MS_ABI impl__OnDrawTabsButtonBorder_CMFCVisualManagerWindows__UEAAXPEAVCDC__AEAVCRect__PEAVCMFCButton__IPEAVCMFCBaseTabCtrl___Z(
    CMFCVisualManagerWindows* pThis, CDC* pDC, CRect* pRect, CMFCButton* pButton, unsigned int uiState,
    CMFCBaseTabCtrl* pWndTab)
{
    impl__OnDrawTabsButtonBorder_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__AEAVCRect__PEAVCMFCButton__IPEAVCMFCBaseTabCtrl___Z(
        reinterpret_cast<CMFCVisualManagerOfficeXP*>(pThis), pDC, pRect, pButton, uiState, pWndTab);
}

// CMFCVisualManagerWindows::OnDrawTask (retail 0x1b9820):
//     if (pTask == NULL || pIcons == NULL) AfxThrowInvalidArgException();  // 0x225b80
//     if (m_hTheme@+0x50 == NULL)
//         { CMFCVisualManagerOfficeXP::OnDrawTask(pDC, pTask, pIcons, bIsHighlighted, bIsSelected); return; } // 0x1b0b80
//     ... otherwise the themed EXPLORERBAR task item ...
// The +0x50 handle is NULL here, so the OfficeXP call is the reachable branch.
// Symbol: ?OnDrawTask@CMFCVisualManagerWindows@@UEAAXPEAVCDC@@PEAVCMFCTasksPaneTask@@PEAVCImageList@@HH@Z
extern "C" void MS_ABI impl__OnDrawTask_CMFCVisualManagerWindows__UEAAXPEAVCDC__PEAVCMFCTasksPaneTask__PEAVCImageList__HH_Z(
    CMFCVisualManagerWindows* pThis, CDC* pDC, CMFCTasksPaneTask* pTask, CImageList* pIcons,
    int bIsHighlighted, int bIsSelected)
{
    if (pTask == nullptr || pIcons == nullptr) {
        impl__AfxThrowInvalidArgException__YAXXZ();
        return;
    }
    impl__OnDrawTask_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__PEAVCMFCTasksPaneTask__PEAVCImageList__HH_Z(
        reinterpret_cast<CMFCVisualManagerOfficeXP*>(pThis), pDC, pTask, pIcons, bIsHighlighted, bIsSelected);
}

// CMFCVisualManagerWindows::OnDrawTasksGroupAreaBorder (retail 0x1b9780):
//     if (m_hTheme@+0x50 == NULL || bNoTitle == 0) return;
//     CRect r = rect; r.bottom = r.top + 1;      // a one-pixel strip
//     ::DrawThemeBackground(+0x50, pDC->m_hDC, ..., ..., &r, NULL);   // part/state
//                                                // chosen from bSpecial
// STUB by the campaign's definition -- the body below is empty -- but the empty
// body IS the retail terminal here: the +0x50 handle is never opened in this
// tree, so retail returns without drawing anything.  Note there is no
// base-class call on this path: the theme test is the only guard, and failing
// it simply falls through to the epilogue.
// Symbol: ?OnDrawTasksGroupAreaBorder@CMFCVisualManagerWindows@@UEAAXPEAVCDC@@VCRect@@HH@Z
extern "C" void MS_ABI impl__OnDrawTasksGroupAreaBorder_CMFCVisualManagerWindows__UEAAXPEAVCDC__VCRect__HH_Z(
    CMFCVisualManagerWindows* /*pThis*/, CDC* /*pDC*/, CRect /*rect*/, int /*bSpecial*/, int /*bNoTitle*/)
{
}

// CMFCVisualManagerWindows::OnDrawTasksGroupCaption (retail 0x1b92f0):
//     if (pGroup == NULL || pGroup->[+0x08] == NULL) AfxThrowInvalidArgException(); // 0x225b80
//     if (m_hTheme@+0x50 == NULL)
//         { CMFCVisualManagerOfficeXP::OnDrawTasksGroupCaption(pDC, pGroup, bIsHighlighted, bIsSelected, bCanCollapse); return; } // 0x1b0730
//     ... otherwise the themed EXPLORERBAR header ...
// The +0x50 handle is NULL here, so the OfficeXP call is the reachable branch.
// The second guard reads a pointer field at CMFCTasksPaneTaskGroup+0x08 that
// OpenMFC does not model, so only the pGroup NULL test is reproduced below.
// Symbol: ?OnDrawTasksGroupCaption@CMFCVisualManagerWindows@@UEAAXPEAVCDC@@PEAVCMFCTasksPaneTaskGroup@@HHH@Z
extern "C" void MS_ABI impl__OnDrawTasksGroupCaption_CMFCVisualManagerWindows__UEAAXPEAVCDC__PEAVCMFCTasksPaneTaskGroup__HHH_Z(
    CMFCVisualManagerWindows* pThis, CDC* pDC, CMFCTasksPaneTaskGroup* pGroup,
    int bIsHighlighted, int bIsSelected, int bCanCollapse)
{
    if (pGroup == nullptr) {
        impl__AfxThrowInvalidArgException__YAXXZ();
        return;
    }
    impl__OnDrawTasksGroupCaption_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__PEAVCMFCTasksPaneTaskGroup__HHH_Z(
        reinterpret_cast<CMFCVisualManagerOfficeXP*>(pThis), pDC, pGroup, bIsHighlighted, bIsSelected, bCanCollapse);
}

// CMFCVisualManagerWindows::OnDrawTearOffCaption (retail 0x1b8610) is an
// unconditional forward -- the whole body copies the CRect onto its own stack
// and calls CMFCVisualManagerOfficeXP::OnDrawTearOffCaption (0x1aefd0).  There
// is no theme test.
// Symbol: ?OnDrawTearOffCaption@CMFCVisualManagerWindows@@UEAAXPEAVCDC@@VCRect@@H@Z
extern "C" void MS_ABI impl__OnDrawTearOffCaption_CMFCVisualManagerWindows__UEAAXPEAVCDC__VCRect__H_Z(
    CMFCVisualManagerWindows* pThis, CDC* pDC, CRect rectCaption, int bIsActive)
{
    impl__OnDrawTearOffCaption_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__VCRect__H_Z(
        reinterpret_cast<CMFCVisualManagerOfficeXP*>(pThis), pDC, rectCaption, bIsActive);
}

// CMFCVisualManagerWindows::OnErasePopupWindowButton (retail 0x1ba430):
//     if (m_hTheme@+0x20 == NULL || pButton->[+0xb20] != 0)
//         { CMFCVisualManagerOfficeXP::OnErasePopupWindowButton(pDC, rectClient, pButton); return; } // 0x1b1990
//     ... otherwise it maps the button rect into its parent and calls the
//     visual-manager virtual at vtable +0x3c8 (OnFillPopupWindowBackground) ...
// The +0x20 handle is NULL here, so the OfficeXP call is the reachable branch.
// Symbol: ?OnErasePopupWindowButton@CMFCVisualManagerWindows@@UEAAXPEAVCDC@@VCRect@@PEAVCMFCDesktopAlertWndButton@@@Z
extern "C" void MS_ABI impl__OnErasePopupWindowButton_CMFCVisualManagerWindows__UEAAXPEAVCDC__VCRect__PEAVCMFCDesktopAlertWndButton___Z(
    CMFCVisualManagerWindows* pThis, CDC* pDC, CRect rectClient, CMFCDesktopAlertWndButton* pButton)
{
    impl__OnErasePopupWindowButton_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__VCRect__PEAVCMFCDesktopAlertWndButton___Z(
        reinterpret_cast<CMFCVisualManagerOfficeXP*>(pThis), pDC, rectClient, pButton);
}

// CMFCVisualManagerWindows::OnEraseTabsArea (retail 0x1ba1e0):
//     if (!CMFCVisualManagerWindows::m_b3DTabsXPTheme || m_hTheme@+0x80 == NULL
//         || <three CMFCBaseTabCtrl virtuals at +0x510/+0x520/+0x528> || pTabWnd->[+0x1fc] != 0)
//         { CMFCVisualManagerOfficeXP::OnEraseTabsArea(pDC, rect, pTabWnd); return; }  // 0x1ae830
//     ... otherwise a themed TAB body fill, optionally through a CMemDC ...
// m_b3DTabsXPTheme is TRUE here, but the +0x80 handle is NULL, so the OfficeXP
// call is the reachable branch either way.
// Symbol: ?OnEraseTabsArea@CMFCVisualManagerWindows@@UEAAXPEAVCDC@@VCRect@@PEBVCMFCBaseTabCtrl@@@Z
extern "C" void MS_ABI impl__OnEraseTabsArea_CMFCVisualManagerWindows__UEAAXPEAVCDC__VCRect__PEBVCMFCBaseTabCtrl___Z(
    CMFCVisualManagerWindows* pThis, CDC* pDC, CRect rect, const CMFCBaseTabCtrl* pTabWnd)
{
    impl__OnEraseTabsArea_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__VCRect__PEBVCMFCBaseTabCtrl___Z(
        reinterpret_cast<CMFCVisualManagerOfficeXP*>(pThis), pDC, rect, pTabWnd);
}

// CMFCVisualManagerWindows::OnEraseTabsButton (retail 0x1b89d0):
//     if (!CMFCVisualManagerWindows::m_b3DTabsXPTheme || m_hTheme@+0x80 == NULL
//         || <three CMFCBaseTabCtrl virtuals at +0x510/+0x520/+0x528>)
//         { CMFCVisualManagerOfficeXP::OnEraseTabsButton(pDC, rect, pButton, pWndTab); return; } // 0x1afd90
//     ... otherwise it clips to the button, maps the tab's client rect and calls
//     the visual-manager virtual at vtable +0x1f0 (OnEraseTabsArea) ...
// m_b3DTabsXPTheme is TRUE here, but the +0x80 handle is NULL, so the OfficeXP
// call is the reachable branch either way.
// Symbol: ?OnEraseTabsButton@CMFCVisualManagerWindows@@UEAAXPEAVCDC@@VCRect@@PEAVCMFCButton@@PEAVCMFCBaseTabCtrl@@@Z
extern "C" void MS_ABI impl__OnEraseTabsButton_CMFCVisualManagerWindows__UEAAXPEAVCDC__VCRect__PEAVCMFCButton__PEAVCMFCBaseTabCtrl___Z(
    CMFCVisualManagerWindows* pThis, CDC* pDC, CRect rect, CMFCButton* pButton, CMFCBaseTabCtrl* pWndTab)
{
    impl__OnEraseTabsButton_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__VCRect__PEAVCMFCButton__PEAVCMFCBaseTabCtrl___Z(
        reinterpret_cast<CMFCVisualManagerOfficeXP*>(pThis), pDC, rect, pButton, pWndTab);
}

// CMFCVisualManagerWindows::OnEraseTabsFrame (retail 0x1ba390):
//     if (!CMFCVisualManagerWindows::m_b3DTabsXPTheme || m_hTheme@+0x80 == NULL
//         || <three CMFCBaseTabCtrl virtuals at +0x510/+0x520/+0x528>)
//         return CMFCVisualManager::OnEraseTabsFrame(pDC, rect, pTabWnd);   // 0x185220
//     return <fall through with EAX from the last virtual, i.e. 0>;
// m_b3DTabsXPTheme is TRUE here, but the +0x80 handle is NULL, so the base call
// is the reachable branch either way, and its BOOL is this function's result.
// Symbol: ?OnEraseTabsFrame@CMFCVisualManagerWindows@@UEAAHPEAVCDC@@VCRect@@PEBVCMFCBaseTabCtrl@@@Z
extern "C" int MS_ABI impl__OnEraseTabsFrame_CMFCVisualManagerWindows__UEAAHPEAVCDC__VCRect__PEBVCMFCBaseTabCtrl___Z(
    CMFCVisualManagerWindows* pThis, CDC* pDC, CRect rect, const CMFCBaseTabCtrl* pTabWnd)
{
    return impl__OnEraseTabsFrame_CMFCVisualManager__UEAAHPEAVCDC__VCRect__PEBVCMFCBaseTabCtrl___Z(
        pThis, pDC, rect, pTabWnd);
}

// CMFCVisualManagerWindows::OnFillBarBackground (retail 0x1b7870):
//     if ((pBar->IsKindOf(RUNTIME_CLASS(CMFCStatusBar)) || pBar->IsKindOf(RUNTIME_CLASS(CMFCRibbonStatusBar)))
//         && m_hTheme@+0x28 != NULL) { ::DrawThemeBackground(+0x28, ..., 0, 0, rectClient, NULL); return; }
//     if (m_hTheme@+0x18 == NULL || pBar->[+0xf4] != 0
//         || pBar->IsKindOf(RUNTIME_CLASS(CMFCCaptionBar)) || pBar->IsKindOf(RUNTIME_CLASS(CMFCColorBar)))
//         { CMFCVisualManagerOfficeXP::OnFillBarBackground(pDC, pBar, rectClient, rectClip, bNCArea); return; } // 0x1ad730
//     (the CRuntimeClass statics at 0x30f690 / 0x30b0c0 / 0x2ddcf8 / 0x3aa0e8 are
//     not exports; their m_lpszClassName strings were read out of the PE)
//     ... otherwise a rebar / toolbar themed fill, or the visual-manager virtual
//     at vtable +0x48 (CMFCBaseVisualManager::FillReBarPane) ...
// Both handles are NULL here, so the OfficeXP call is the reachable branch.
// Symbol: ?OnFillBarBackground@CMFCVisualManagerWindows@@UEAAXPEAVCDC@@PEAVCBasePane@@VCRect@@2H@Z
extern "C" void MS_ABI impl__OnFillBarBackground_CMFCVisualManagerWindows__UEAAXPEAVCDC__PEAVCBasePane__VCRect__2H_Z(
    CMFCVisualManagerWindows* pThis, CDC* pDC, CBasePane* pBar, CRect rectClient, CRect rectClip, int bNCArea)
{
    impl__OnFillBarBackground_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__PEAVCBasePane__VCRect__2H_Z(
        reinterpret_cast<CMFCVisualManagerOfficeXP*>(pThis), pDC, pBar, rectClient, rectClip, bNCArea);
}

// CMFCVisualManagerWindows::OnFillButtonInterior (retail 0x1b7ad0):
//     if (m_hTheme@+0x10 == NULL)
//         { CMFCVisualManagerOfficeXP::OnFillButtonInterior(pDC, pButton, rect, state); return; } // 0x1aded0
//     ... otherwise it derives a TP_BUTTON state from the button's runtime class
//     and its +0x80 parent, then ::DrawThemeBackground(+0x10, ..., 1, state, rect, NULL) ...
// The +0x10 handle is NULL here, so the OfficeXP call is the reachable branch.
// Symbol: ?OnFillButtonInterior@CMFCVisualManagerWindows@@UEAAXPEAVCDC@@PEAVCMFCToolBarButton@@VCRect@@W4AFX_BUTTON_STATE@CMFCVisualManager@@@Z
extern "C" void MS_ABI impl__OnFillButtonInterior_CMFCVisualManagerWindows__UEAAXPEAVCDC__PEAVCMFCToolBarButton__VCRect__W4AFX_BUTTON_STATE_CMFCVisualManager___Z(
    CMFCVisualManagerWindows* pThis, CDC* pDC, CMFCToolBarButton* pButton, CRect rect, int state)
{
    impl__OnFillButtonInterior_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__PEAVCMFCToolBarButton__VCRect__W4AFX_BUTTON_STATE_CMFCVisualManager___Z(
        reinterpret_cast<CMFCVisualManagerOfficeXP*>(pThis), pDC, pButton, rect, state);
}

// CMFCVisualManagerWindows::OnFillCommandsListBackground (retail 0x1b84c0):
//     if (m_hTheme@+0x08 == NULL || this->[+0x210] != 0)
//         return CMFCVisualManagerOfficeXP::OnFillCommandsListBackground(pDC, rect, bIsSelected); // 0x1aed30
//     ::FillRect(pDC->m_hDC, rect, ::GetSysColorBrush(4 /*COLOR_MENU*/));
//     if (!bIsSelected) return afxGlobalData.[+0x84];
//     ::FillRect(pDC->m_hDC, rect, afxGlobalData.[+0xc0]);
//     pDC->Draw3dRect(rect, afxGlobalData.[+0x84], afxGlobalData.[+0x84]);
//     return afxGlobalData.[+0x4c];
// The +0x08 handle is NULL here, so the OfficeXP call is the reachable branch
// and its COLORREF is this function's result.
// Symbol: ?OnFillCommandsListBackground@CMFCVisualManagerWindows@@UEAAKPEAVCDC@@VCRect@@H@Z
extern "C" unsigned long MS_ABI impl__OnFillCommandsListBackground_CMFCVisualManagerWindows__UEAAKPEAVCDC__VCRect__H_Z(
    CMFCVisualManagerWindows* pThis, CDC* pDC, CRect rect, int bIsSelected)
{
    return impl__OnFillCommandsListBackground_CMFCVisualManagerOfficeXP__MEAAKPEAVCDC__VCRect__H_Z(
        reinterpret_cast<CMFCVisualManagerOfficeXP*>(pThis), pDC, rect, bIsSelected);
}

// CMFCVisualManagerWindows::OnFillMenuImageRect (retail 0x1b7ea0):
//     if (m_hTheme@+0x98 != NULL && this->[+0x210] == 0) {
//         if (!(pButton->m_nStyle(+0x28) & 0x10000 /*TBBS_CHECKED*/)) return;
//         ::DrawThemeBackground(+0x98, pDC->m_hDC, 12, 2, &rect, NULL);
//         return;
//     }
//     this->vtbl[+0x118](pDC, pButton, rect, state);
// Vtable slot +0x118 is OnFillButtonInterior (read out of the retail
// CMFCVisualManagerWindows vftable, where it holds this class's own override at
// 0x1b7ad0).  The +0x98 handle is NULL here, so that dispatch is the reachable
// branch.  DEVIATION: retail dispatches it virtually; OpenMFC's C++ vtable for
// this class does not carry the impl__ overrides, so the call below names this
// class's own OnFillButtonInterior thunk directly.  That matches retail for a
// CMFCVisualManagerWindows, but a class deriving from it and overriding
// OnFillButtonInterior would be bypassed.
// Symbol: ?OnFillMenuImageRect@CMFCVisualManagerWindows@@UEAAXPEAVCDC@@PEAVCMFCToolBarButton@@VCRect@@W4AFX_BUTTON_STATE@CMFCVisualManager@@@Z
extern "C" void MS_ABI impl__OnFillMenuImageRect_CMFCVisualManagerWindows__UEAAXPEAVCDC__PEAVCMFCToolBarButton__VCRect__W4AFX_BUTTON_STATE_CMFCVisualManager___Z(
    CMFCVisualManagerWindows* pThis, CDC* pDC, CMFCToolBarButton* pButton, CRect rect, int state)
{
    impl__OnFillButtonInterior_CMFCVisualManagerWindows__UEAAXPEAVCDC__PEAVCMFCToolBarButton__VCRect__W4AFX_BUTTON_STATE_CMFCVisualManager___Z(
        pThis, pDC, pButton, rect, state);
}

// Symbol: ?OnFillMiniFrameCaption@CMFCVisualManagerWindows@@UEAAKPEAVCDC@@VCRect@@PEAVCPaneFrameWnd@@H@Z
extern "C" unsigned long MS_ABI impl__OnFillMiniFrameCaption_CMFCVisualManagerWindows__UEAAKPEAVCDC__VCRect__PEAVCPaneFrameWnd__H_Z(void* /*class*/* p0, void* /*class*/ p1, void* /*class*/* p2, int p3) {
    return 0;
}

// Symbol: ?OnFillOutlookPageButton@CMFCVisualManagerWindows@@UEAAXPEAVCDC@@AEBVCRect@@HHAEAK@Z
extern "C" void MS_ABI impl__OnFillOutlookPageButton_CMFCVisualManagerWindows__UEAAXPEAVCDC__AEBVCRect__HHAEAK_Z(void* /*class*/* p0, const void* /*class*/* p1, int p2, int p3, unsigned long* p4) {}

// Symbol: ?OnFillTasksGroupInterior@CMFCVisualManagerWindows@@UEAAXPEAVCDC@@VCRect@@H@Z
extern "C" void MS_ABI impl__OnFillTasksGroupInterior_CMFCVisualManagerWindows__UEAAXPEAVCDC__VCRect__H_Z(void* /*class*/* p0, void* /*class*/ p1, int p2) {}

// Symbol: ?OnFillTasksPaneBackground@CMFCVisualManagerWindows@@UEAAXPEAVCDC@@VCRect@@@Z
extern "C" void MS_ABI impl__OnFillTasksPaneBackground_CMFCVisualManagerWindows__UEAAXPEAVCDC__VCRect___Z(void* /*class*/* p0, void* /*class*/ p1) {}

// Symbol: ?OnHighlightMenuItem@CMFCVisualManagerWindows@@UEAAXPEAVCDC@@PEAVCMFCToolBarMenuButton@@VCRect@@AEAK@Z
extern "C" void MS_ABI impl__OnHighlightMenuItem_CMFCVisualManagerWindows__UEAAXPEAVCDC__PEAVCMFCToolBarMenuButton__VCRect__AEAK_Z(void* /*class*/* p0, void* /*class*/* p1, void* /*class*/ p2, unsigned long* p3) {}

// Symbol: ?OnHighlightRarelyUsedMenuItems@CMFCVisualManagerWindows@@UEAAXPEAVCDC@@VCRect@@@Z
extern "C" void MS_ABI impl__OnHighlightRarelyUsedMenuItems_CMFCVisualManagerWindows__UEAAXPEAVCDC__VCRect___Z(void* /*class*/* p0, void* /*class*/ p1) {}

// Symbol: ?OnUpdateSystemColors@CMFCVisualManagerWindows@@UEAAXXZ
extern "C" void MS_ABI impl__OnUpdateSystemColors_CMFCVisualManagerWindows__UEAAXXZ() {}

// Symbol: ?SetOfficeStyleMenus@CMFCVisualManagerWindows@@QEAAXH@Z
extern "C" void MS_ABI impl__SetOfficeStyleMenus_CMFCVisualManagerWindows__QEAAXH_Z(int p0) {}
