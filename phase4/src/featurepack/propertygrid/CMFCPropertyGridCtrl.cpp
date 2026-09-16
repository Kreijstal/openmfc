// CMFCPropertyGridCtrl — OpenMFC implementation.
// Sources: mfccore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/MfccoreSupport.h"
#include <cstddef>   // offsetof (AfxThreadStateMirror pins below)


// Implementations this unit calls that are defined with their own class.
extern "C" void MS_ABI impl__OnDrawName_CMFCPropertyGridProperty__UEAAXPEAVCDC__VCRect___Z(
    CMFCPropertyGridProperty* pThis, CDC* pDC, CRect rect);
extern "C" void MS_ABI impl__OnDrawValue_CMFCPropertyGridProperty__UEAAXPEAVCDC__VCRect___Z(
    CMFCPropertyGridProperty* pThis, CDC* pDC, CRect rect);

// ---------------------------------------------------------------------------
// Retail layout notes for the message handlers below (all offsets read from
// the mfc140u.dll disassembly; bodies are byte-identical to mfc140.dll).
//
// Retail CMFCPropertyGridCtrl is 0x6b8 bytes -- its ctor (RVA 0xc5ef0,
// mfc140u) writes members up to +0x6b0.  OpenMFC's object is CWnd (0xe8) plus
// 128 bytes of padding, so none of the members the retail handlers touch has a
// home in the object.  The list/selection members are already modelled by the
// PropertyGridCtrlState side table in detail/MfccoreSupport.h:
//     +0x608 CList m_lstProps            -> state->properties
//     +0x678 CMFCPropertyGridProperty* m_pSel -> state->current
//     +0x5a4 int m_nDescrRows            -> state->descriptionRows
//     +0x684..+0x69c seven COLORREFs     -> state->customColors[0..6]
// The window-level members below have no home anywhere, so this file keeps
// them in a second per-instance side table (PropertyGridWndState), erased by
// the destructor.  Retail member map, as established from the ctor and the
// handlers that read them:
//     +0x0e8 CToolTipCtrl m_ToolTip              (no OpenMFC counterpart)
//     +0x208 CMFCHeaderCtrl m_wndHeader          (vslot 91 returns &this->+0x208)
//     +0x348 CMFCPropertyGridToolTipCtrl m_IPToolTip
//     +0x460 CScrollBar m_wndScrollVert
//     +0x548 BOOL m_bHeaderCtrl (ctor 1)   +0x54c BOOL m_bDescriptionArea
//     +0x554 BOOL m_bAlphabeticMode        +0x558 BOOL m_bVSDotNetLook
//     +0x55c BOOL m_bMarkModifiedProperties
//     +0x578 HFONT m_hFont                 +0x580 CFont m_fontBold
//     +0x590 CRect m_rectList              +0x5a0 int m_nDescrHeight (ctor -1)
//     +0x5a8 int m_nTooltipsCount          +0x5b8 int m_nHeaderHeight
//     +0x5bc int m_nRowHeight              +0x5c0 int m_nLeftColumnWidth
//     +0x5c4 int m_nVertScrollOffset       +0x5c8 int m_nVertScrollTotal
//     +0x5cc int m_nVertScrollPage         +0x5d0 CRect m_rectTrackHeader
//     +0x5e0 CRect m_rectTrackDescr        +0x5f0 BOOL m_bTracking
//     +0x5f4 BOOL m_bTrackingDescr         +0x5f8 BOOL m_bFocused
//     +0x5fc BOOL m_bControlBarColors      +0x604 BOOL (ctor 1; forced to 1
//                                                 around TrackHeader by OnKeyDown)
// Retail CMFCPropertyGridProperty members the handlers read (Init 0xc0e80,
// Reposition 0xc1a80, IsSelected 0xc1840, Expand 0xc1600, HasButton 0xc11a0,
// all mfc140u):
//     +0x40 DWORD m_dwFlags (bit 1 = AFX_PROP_HAS_LIST, bit 2 = HAS_BUTTON)
//     +0x44 CRect m_Rect      +0x54 CRect m_rectButton   +0x64 BOOL m_bButtonIsDown
//     +0x6c BOOL m_bInPlaceEdit  +0x70 BOOL m_bGroup  +0x74 BOOL m_bExpanded
//     +0x78 BOOL m_bEnabled   +0x80 BOOL m_bIsValueList
//     +0x100 CWnd* m_pWndInPlace  +0x108 CComboBox* m_pWndCombo
//     +0x118 CMFCPropertyGridCtrl* m_pWndList  +0x120 m_pParent
//     +0x128 CList m_lstSubItems (head +0x130, tail +0x138, count +0x140)
// OpenMFC's property has no m_Rect / m_rectButton / m_bInPlaceEdit /
// m_pWndInPlace / m_pWndCombo / m_bGroup; the substitutions each handler makes
// for those are stated at the handler.  In particular OpenMFC never creates an
// in-place edit window, so every retail path that starts with
// `if (pProp->m_pWndInPlace == NULL) return;` is taken as the early return.
//
// Property vtable slots (mfc140u vftable 0x1802fb460) dispatched below, all
// through the sibling's impl__ thunks (the OpenMFC vtable is not the retail
// one): 11 FormatProperty, 12 OnUpdateValue, 16 OnEndEdit, 17 OnClickButton,
// 18 OnClickValue, 19 OnDblClk, 20 OnSelectCombo, 21 OnCloseCombo,
// 22 OnSetCursor, 23 PushChar, 26 OnClickName, 27 OnRClickName,
// 28 OnRClickValue (26..28 are `ret` in retail: entry 0x27d0), 34 OnCtlColor,
// 37 OnKillFocus (`mov eax,1; ret`: entry 0x3a60), 40 IsProcessFirstClick
// (same `return TRUE` body).
// Grid vtable slots (mfc140u vftable 0x1802fab48) dispatched below, through
// this file's own thunks: 94 AdjustLayout, 95 OnPropertyChanged, 98 EditItem,
// 99 OnClickButton, 100 EndEditItem, 103 InitHeader, 104 Init, 106 OnDraw,
// 107 OnDrawBorder, 108 OnDrawList, 109 OnDrawDescription,
// 110 ProcessClipboardAccelerators.
// ---------------------------------------------------------------------------

// Thunks defined in this file further down that the handlers call.
extern "C" void MS_ABI impl__CreateBoldFont_CMFCPropertyGridCtrl__IEAAXXZ(CMFCPropertyGridCtrl* pThis);
extern "C" void MS_ABI impl__EnableHeaderCtrl_CMFCPropertyGridCtrl__QEAAXHPEB_W0_Z(
    CMFCPropertyGridCtrl* pThis, int bEnable, const wchar_t* lpszLeftColumn, const wchar_t* lpszRightColumn);
extern "C" void MS_ABI impl__OnKillFocus_CMFCPropertyGridCtrl__IEAAXPEAVCWnd___Z(CMFCPropertyGridCtrl* pThis, CWnd* pNewWnd);
extern "C" void MS_ABI impl__OnPropertyChanged_CMFCPropertyGridCtrl__UEBAXPEAVCMFCPropertyGridProperty___Z(
    const CMFCPropertyGridCtrl* pThis, CMFCPropertyGridProperty* pProp);
extern "C" void MS_ABI impl__OnVScroll_CMFCPropertyGridCtrl__IEAAXIIPEAVCScrollBar___Z(
    CMFCPropertyGridCtrl* pThis, unsigned int nSBCode, unsigned int nPos, CScrollBar* pScrollBar);
extern "C" void MS_ABI impl__ReposProperties_CMFCPropertyGridCtrl__IEAAXXZ(CMFCPropertyGridCtrl* pThis);
extern "C" void MS_ABI impl__SetVSDotNetLook_CMFCPropertyGridCtrl__QEAAXH_Z(CMFCPropertyGridCtrl* pThis, int bSet);
extern "C" void MS_ABI impl__TrackDescr_CMFCPropertyGridCtrl__IEAAXH_Z(CMFCPropertyGridCtrl* pThis, int nOffset);
extern "C" void MS_ABI impl__TrackHeader_CMFCPropertyGridCtrl__IEAAXH_Z(CMFCPropertyGridCtrl* pThis, int nOffset);
extern "C" void MS_ABI impl__TrackToolTip_CMFCPropertyGridCtrl__IEAAXVCPoint___Z(CMFCPropertyGridCtrl* pThis, CPoint point);

// Thunks defined elsewhere in the tree (each grepped to its definition):
//   core/window/Thunks.cpp     : Default@CWnd, OnSetFocus@CWnd, OnDestroy@CWnd,
//                                OnGetObject@CWnd, SetFocus@CWnd
//   core/window/CWnd.cpp       : FromHandle@CWnd, SendChildNotifyLastMsg@CWnd, OnNotify@CWnd
//   core/runtime/CObject.cpp   : IsKindOf@CObject
//   core/runtime/AFX_GLOBAL_DATA.cpp : Initialize@AFX_GLOBAL_DATA (a no-op here)
//   core/runtime/Globals.cpp   : AfxFindResourceHandle, ?AFX_WM_PROPERTY_CHANGED@@3IA
//   core/runtime/StaticData.cpp: ?m_pActivePopupMenu@CMFCPopupMenu@@ (?m_pVisManager@
//                                CMFCVisualManager@@ is declared by detail/MfccoreSupport.h)
//   core/runtime/CCmdTarget.cpp: BeginWaitCursor / EndWaitCursor@CCmdTarget
//   detail/RegcoreSupport.cpp  : impl__AfxGetApp__YAPEAVCWinApp__XZ
//   core/gdi/CPaintDC.cpp      : ??0CPaintDC / ??1CPaintDC
//   featurepack/CMFC_misc_stubs.cpp : ?afxGlobalData@@3UAFX_GLOBAL_DATA@@A (720-byte
//                                blob), ReadBoolProp
//   featurepack/controls/CTagManager.cpp : ??0CTagManager / ??1CTagManager / ExcludeTag
//   featurepack/controls/CMFCControlContainer.cpp : UTF8ToString
//   featurepack/propertygrid/RuntimeClasses.cpp : GetThisClass@CMFCPropertyGridColorProperty
//   featurepack/propertygrid/CMFCPropertyGridProperty.cpp : OnClickButton / OnClickValue /
//                                OnDblClk / PushChar / OnSetCursor / OnDrawDescription /
//                                HasButton / OnCtlColor (placeholder-typed there)
//   featurepack/propertygrid/Thunks.cpp : Redraw@CMFCPropertyGridProperty
//   core/window/CWnd.cpp       : PreTranslateMessage@CWnd
//   featurepack/propertygrid/CMFCPropertyGridProperty.cpp : OnActivateByTab (typed there
//                                with the same (this) list declared here)
//   detail/CWinAppSupport.cpp  : impl__AfxGetThreadState
extern "C" __int64 MS_ABI impl__Default_CWnd__IEAA_JXZ(CWnd* pThis);
extern "C" void MS_ABI impl__OnSetFocus_CWnd__IEAAXPEAV1__Z(CWnd* pThis, void* pOldWnd);
extern "C" void MS_ABI impl__OnDestroy_CWnd__IEAAXXZ(CWnd* pThis);
extern "C" __int64 MS_ABI impl__OnGetObject_CWnd__IEAA_J_K_J_Z(CWnd* pThis, unsigned __int64 wParam, __int64 lParam);
extern "C" void* MS_ABI impl__SetFocus_CWnd__QEAAPEAV1_XZ(CWnd* pThis);
extern "C" CWnd* MS_ABI impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(HWND hWnd);
extern "C" int MS_ABI impl__SendChildNotifyLastMsg_CWnd__QEAAHPEA_J_Z(CWnd* pThis, LONGLONG* pResult);
extern "C" int MS_ABI impl__OnNotify_CWnd__MEAAH_K_JPEA_J_Z(CWnd* pThis, WPARAM wParam, LPARAM lParam, LRESULT* pResult);
extern "C" int MS_ABI impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(const CObject* pThis, const CRuntimeClass* pClass);
extern "C" void MS_ABI impl__Initialize_AFX_GLOBAL_DATA__QEAAXXZ();
extern "C" HINSTANCE MS_ABI impl__AfxFindResourceHandle__YAPEAUHINSTANCE____PEB_W0_Z(const wchar_t* lpszResource, const wchar_t* lpszType);
extern "C" unsigned int impl__AFX_WM_PROPERTY_CHANGED__3IA;
extern "C" void* impl__m_pActivePopupMenu_CMFCPopupMenu__1PEAV1_EA;
extern "C" void MS_ABI impl__BeginWaitCursor_CCmdTarget__QEAAXXZ(CCmdTarget* pThis);
extern "C" void MS_ABI impl__EndWaitCursor_CCmdTarget__QEAAXXZ(CCmdTarget* pThis);
extern "C" CWinApp* MS_ABI impl__AfxGetApp__YAPEAVCWinApp__XZ();
extern "C" CPaintDC* MS_ABI impl___0CPaintDC__QEAA_PEAVCWnd___Z(CPaintDC* pThis, CWnd* pWnd);
extern "C" void MS_ABI impl___1CPaintDC__UEAA_XZ(CPaintDC* pThis);
extern "C" unsigned char impl__afxGlobalData__3UAFX_GLOBAL_DATA__A[720];
extern "C" int MS_ABI impl__ReadBoolProp__YAHAEAVCTagManager__PEB_WAEAH_Z(void* pTagManager, const wchar_t* lpszTag, int* pValue);
extern "C" void MS_ABI impl___0CTagManager__QEAA_PEB_W_Z(void* pThis, const wchar_t* lpszBuffer);
extern "C" void MS_ABI impl___1CTagManager__UEAA_XZ(void* pThis);
extern "C" int MS_ABI impl__ExcludeTag_CTagManager__QEAAHPEB_WAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__H_Z(
    void* pThis, const wchar_t* lpszTag, CString* pValue, int bTrim);
extern "C" int MS_ABI impl__UTF8ToString_CMFCControlContainer__SAHPEBDAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__H_Z(
    const char* pszUTF8, CString* pStrResult, int cbUTF8);
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMFCPropertyGridColorProperty__SAPEAUCRuntimeClass__XZ();
extern "C" void MS_ABI impl__OnClickButton_CMFCPropertyGridProperty__UEAAXVCPoint___Z(CMFCPropertyGridProperty* pThis, CPoint point);
extern "C" int MS_ABI impl__OnClickValue_CMFCPropertyGridProperty__UEAAHIVCPoint___Z(CMFCPropertyGridProperty* pThis, unsigned int uiMsg, CPoint point);
extern "C" int MS_ABI impl__OnDblClk_CMFCPropertyGridProperty__UEAAHVCPoint___Z(CMFCPropertyGridProperty* pThis, CPoint point);
extern "C" int MS_ABI impl__PushChar_CMFCPropertyGridProperty__UEAAHI_Z(CMFCPropertyGridProperty* pThis, unsigned int nChar);
extern "C" int MS_ABI impl__OnSetCursor_CMFCPropertyGridProperty__UEBAHXZ(CMFCPropertyGridProperty* pThis);
extern "C" void MS_ABI impl__OnDrawDescription_CMFCPropertyGridProperty__UEAAXPEAVCDC__VCRect___Z(
    CMFCPropertyGridProperty* pThis, CDC* pDC, CRect rect);
extern "C" int MS_ABI impl__HasButton_CMFCPropertyGridProperty__MEBAHXZ(CMFCPropertyGridProperty* pThis);
extern "C" void MS_ABI impl__Redraw_CMFCPropertyGridProperty__QEAAXXZ(CMFCPropertyGridProperty* pThis);
extern "C" int MS_ABI impl__PreTranslateMessage_CWnd__UEAAHPEAUtagMSG___Z(CWnd* pThis, MSG* pMsg);
extern "C" int MS_ABI impl__OnActivateByTab_CMFCPropertyGridProperty__MEAAHXZ(CMFCPropertyGridProperty* pThis);
// _AFX_THREAD_STATE is only forward-declared by afxwin.h; its definition lives in
// detail/CWinAppSupport.h, which cannot be included here because that header
// also carries top-level asm(".set") aliases that add undefined C++ symbols to
// every unit including it (the campaign's link audit rejects them).
extern "C" _AFX_THREAD_STATE* MS_ABI impl__AfxGetThreadState__YAPEAV_AFX_THREAD_STATE__XZ();
// The sibling defines this one with the auto-generated list `(void** p0, unsigned p1)`
// (no `this`); it is a `return nullptr` stub, so the extra argument is harmless.
// Its real signature per the mangled name is the one declared here.
extern "C" HBRUSH MS_ABI impl__OnCtlColor_CMFCPropertyGridProperty__MEAAPEAUHBRUSH____PEAVCDC__I_Z(
    CMFCPropertyGridProperty* pThis, CDC* pDC, unsigned int nCtlColor);

namespace {

// Window-level retail members (see the map above) that have no home in
// OpenMFC's object.  Defaults are the retail ctor's values except where noted.
struct PropertyGridWndState {
    HFONT hFont = nullptr;                 // +0x578 m_hFont (not owned)
    HFONT hFontBold = nullptr;             // +0x580 CFont m_fontBold: only its HFONT, owned here
    BOOL bHeaderCtrl = TRUE;               // +0x548
    BOOL bVSDotNetLook = FALSE;            // +0x558
    BOOL bMarkModifiedProperties = FALSE;  // +0x55c
    int nDescrHeight = -1;                 // +0x5a0
    int nHeaderHeight = 0;                 // +0x5b8 (retail AdjustLayout fills it; OpenMFC's does not)
    // +0x5bc m_nRowHeight: retail AdjustLayout derives it from the font; OpenMFC's
    // AdjustLayout (above) computes no geometry and PropertyGridItemRect
    // (detail/MfccoreSupport.cpp) lays rows out at a fixed 18, so start there.
    int nRowHeight = 18;
    // +0x5c0 m_nLeftColumnWidth: retail ctor 0, set by AdjustLayout/OnSize.  This
    // file's HitTest splits name/value at x == 120, so start from the same value
    // so the splitter hit-tests below agree with it until OnSize/OnLButtonUp run.
    int nLeftColumnWidth = 120;
    int nVertScrollOffset = 0;             // +0x5c4
    int nVertScrollTotal = 0;              // +0x5c8 (never raised: no scroll layout here)
    int nVertScrollPage = 0;               // +0x5cc (never raised: no scroll layout here)
    BOOL bTracking = FALSE;                // +0x5f0
    BOOL bTrackingDescr = FALSE;           // +0x5f4
    BOOL bFocused = FALSE;                 // +0x5f8
    BOOL bControlBarColors = FALSE;        // +0x5fc
    BOOL bShowDragContext = TRUE;          // +0x604 (name inferred from its use in TrackHeader; retail ctor 1)
};

// Field-for-field mirror of detail/CWinAppSupport.h's `struct _AFX_THREAD_STATE`
// (int nTempMapLock; int nWndCreateLock; void* pModuleState; void* pWndInit;
// void* hHookOldCbtFilter;), used only to read pWndInit in PreSubclassWindow
// below.  The pins record the layout this file was written against; they
// cannot see the real header, so keep the two in step (see the note at the
// impl__AfxGetThreadState declaration above for why it is not included).
struct AfxThreadStateMirror {
    int nTempMapLock;
    int nWndCreateLock;
    void* pModuleState;
    void* pWndInit;
    void* hHookOldCbtFilter;
};
static_assert(sizeof(AfxThreadStateMirror) == 32, "AfxThreadStateMirror: five-field _AFX_THREAD_STATE");
static_assert(offsetof(AfxThreadStateMirror, pWndInit) == 16, "AfxThreadStateMirror::pWndInit");

// Kept as a hand-rolled singly-linked list rather than a std::unordered_map:
// the map would pull libstdc++ rehash/throw helpers into this object that the
// campaign's link audit (checkfile.sh) rejects as new undefined symbols.
struct PropertyGridWndStateNode {
    const CMFCPropertyGridCtrl* pCtrl;
    PropertyGridWndState state;
    PropertyGridWndStateNode* pNext;
};
thread_local PropertyGridWndStateNode* g_propertyGridWndStates = nullptr;

PropertyGridWndStateNode* FindWndStateNode(const CMFCPropertyGridCtrl* pCtrl) {
    for (PropertyGridWndStateNode* p = g_propertyGridWndStates; p != nullptr; p = p->pNext) {
        if (p->pCtrl == pCtrl) return p;
    }
    return nullptr;
}
PropertyGridWndState& WndState(const CMFCPropertyGridCtrl* pCtrl) {
    if (PropertyGridWndStateNode* p = FindWndStateNode(pCtrl)) return p->state;
    PropertyGridWndStateNode* p = new PropertyGridWndStateNode{pCtrl, PropertyGridWndState(), g_propertyGridWndStates};
    g_propertyGridWndStates = p;
    return p->state;
}
// Destructor hook: releases the owned bold font and drops the node.
void EraseWndState(const CMFCPropertyGridCtrl* pCtrl) {
    PropertyGridWndStateNode** pp = &g_propertyGridWndStates;
    while (*pp != nullptr) {
        if ((*pp)->pCtrl == pCtrl) {
            PropertyGridWndStateNode* dead = *pp;
            *pp = dead->pNext;
            if (dead->state.hFontBold != nullptr) ::DeleteObject(dead->state.hFontBold);
            delete dead;
            return;
        }
        pp = &(*pp)->pNext;
    }
}

// m_pSel (+0x678) -> PropertyGridCtrlState::current.
CMFCPropertyGridProperty* CurSelOf(const CMFCPropertyGridCtrl* pCtrl) {
    const PropertyGridCtrlState* state = FindPropertyGridCtrlState(pCtrl);
    return state ? state->current : nullptr;
}

// m_bDescriptionArea (+0x54c): EnableDescriptionArea above models it as
// SetDescriptionRows(2 / 0), so a non-zero row count is the flag.
BOOL DescriptionAreaOf(const CMFCPropertyGridCtrl* pCtrl) {
    const PropertyGridCtrlState* state = FindPropertyGridCtrlState(pCtrl);
    return (state && state->descriptionRows > 0) ? TRUE : FALSE;
}

// m_rectList (+0x590): retail AdjustLayout derives it from the client rect minus
// the header and description area.  OpenMFC's AdjustLayout computes no geometry
// and PropertyGridItemRect treats the whole client area as the list, so the
// client rect is the list rect here.
CRect ListRectOf(const CMFCPropertyGridCtrl* pCtrl) {
    RECT rc = {0, 0, 0, 0};
    if (pCtrl && pCtrl->GetSafeHwnd()) ::GetClientRect(pCtrl->GetSafeHwnd(), &rc);
    return CRect(rc);
}

// pProp->m_Rect (+0x44): OpenMFC keeps no per-property rect; PropertyGridItemRect
// is what this file's drawing and accessibility code use for the same purpose.
CRect PropRectOf(CMFCPropertyGridCtrl* pCtrl, CMFCPropertyGridProperty* pProp) {
    return PropertyGridItemRect(pCtrl, PropertyGridChildIDFromProperty(pCtrl, pProp));
}

// pProp->m_bGroup (+0x70): OpenMFC's property has no group flag (the
// (name, data, rowHeight) ctor that sets it is the sibling's); a property with
// sub-items is the closest observable equivalent.
BOOL IsGroupOf(const CMFCPropertyGridProperty* pProp) {
    return (pProp && pProp->GetSubItemsCount() > 0) ? TRUE : FALSE;
}

int IndexOfVisible(const std::vector<CMFCPropertyGridProperty*>& items, const CMFCPropertyGridProperty* pProp) {
    for (size_t i = 0; i < items.size(); ++i) {
        if (items[i] == pProp) return static_cast<int>(i);
    }
    return -1;
}

// afxGlobalData: the 720-byte zero-filled blob featurepack/CMFC_misc_stubs.cpp
// exports.  Retail's one-time gate `if (!afxGlobalData.<+0>) { Initialize();
// <+0> = 1; }` precedes every read; the slots read here (offsets from
// ?UpdateSysColors@AFX_GLOBAL_DATA@@ 0x6b1c0 mfc140u, ?Init@CMFCPropertyGridCtrl@@
// 0xc6390 and the CPaneDivider.cpp precedent):
//   +0x02c COLORREF clrBtnShadow, +0x064 COLORREF clrBarShadow (both GetSysColor(COLOR_BTNSHADOW))
//   +0x0b0 HBRUSH  brBtnFace.m_hObject, +0x120 HBRUSH brBarFace.m_hObject
//   +0x158 HCURSOR m_hcurStretch, +0x160 HCURSOR m_hcurStretchVert
//   +0x278 BOOL    accessibility-support gate (as CMDIFrameWndEx.cpp reads it)
constexpr int kGlobalDataInitGate     = 0x000;
constexpr int kGlobalDataClrBtnShadow = 0x02c;
constexpr int kGlobalDataClrBarShadow = 0x064;
constexpr int kGlobalDataBrBtnFace    = 0x0b0;
constexpr int kGlobalDataBrBarFace    = 0x120;
constexpr int kGlobalDataCurStretch   = 0x158;
constexpr int kGlobalDataCurStretchV  = 0x160;
constexpr int kGlobalDataAccessibility = 0x278;
// AFX_IDC_HSPLITBAR / AFX_IDC_VSPLITBAR (afxres.h 30980 / 30981), the cursor ids
// retail Init (0xc6390 mfc140u) loads into the two slots above.
constexpr WORD kIdcHSplitBar = 30980;
constexpr WORD kIdcVSplitBar = 30981;

inline unsigned char* GlobalDataBytes() { return impl__afxGlobalData__3UAFX_GLOBAL_DATA__A; }
inline int GlobalDataInt(int off) { int v; std::memcpy(&v, GlobalDataBytes() + off, sizeof v); return v; }
inline void* GlobalDataPtr(int off) { void* v; std::memcpy(&v, GlobalDataBytes() + off, sizeof v); return v; }
inline void SetGlobalDataPtr(int off, void* v) { std::memcpy(GlobalDataBytes() + off, &v, sizeof v); }
inline void EnsureGlobalDataInitialized() {
    if (GlobalDataInt(kGlobalDataInitGate) == 0) {
        impl__Initialize_AFX_GLOBAL_DATA__QEAAXXZ();
        const int one = 1;
        std::memcpy(GlobalDataBytes() + kGlobalDataInitGate, &one, sizeof one);
    }
}
inline HCURSOR LoadAfxCursor(WORD id) {
    HINSTANCE hInst = impl__AfxFindResourceHandle__YAPEAUHINSTANCE____PEB_W0_Z(
        MAKEINTRESOURCEW(id), MAKEINTRESOURCEW(12) /* RT_GROUP_CURSOR */);
    return ::LoadCursorW(hInst, MAKEINTRESOURCEW(id));
}
// Retail loads the two splitter cursors lazily in Init (0xc6390 mfc140u):
//     if (afxGlobalData.m_hcurStretch == NULL)     m_hcurStretch     = AfxGetApp()->LoadCursor(AFX_IDC_HSPLITBAR);
//     if (afxGlobalData.m_hcurStretchVert == NULL) m_hcurStretchVert = AfxGetApp()->LoadCursor(AFX_IDC_VSPLITBAR);
// DEVIATION: this file's Init (above) predates this and does not, so OnSetCursor
// runs the same lazy load before reading the slots.
inline void EnsureSplitterCursors() {
    EnsureGlobalDataInitialized();
    if (GlobalDataPtr(kGlobalDataCurStretch) == nullptr) {
        SetGlobalDataPtr(kGlobalDataCurStretch, LoadAfxCursor(kIdcHSplitBar));
    }
    if (GlobalDataPtr(kGlobalDataCurStretchV) == nullptr) {
        SetGlobalDataPtr(kGlobalDataCurStretchV, LoadAfxCursor(kIdcVSplitBar));
    }
}

// CDC::FillSolidRect / CDC::Draw3dRect as afxwin.h inlines them (SetBkColor +
// ETO_OPAQUE ExtTextOut; four one-pixel edges, top/left in the first colour,
// bottom/right in the second), on the raw HDC so no CDC method is linked.
void FillSolidRectHdc(HDC hdc, int x, int y, int cx, int cy, COLORREF clr) {
    const COLORREF clrOld = ::SetBkColor(hdc, clr);
    RECT rc = {x, y, x + cx, y + cy};
    ::ExtTextOutW(hdc, 0, 0, ETO_OPAQUE, &rc, nullptr, 0, nullptr);
    ::SetBkColor(hdc, clrOld);
}
void Draw3dRectHdc(HDC hdc, const CRect& rect, COLORREF clrTopLeft, COLORREF clrBottomRight) {
    const int x = rect.left, y = rect.top, cx = rect.Width(), cy = rect.Height();
    FillSolidRectHdc(hdc, x, y, cx - 1, 1, clrTopLeft);
    FillSolidRectHdc(hdc, x, y, 1, cy - 1, clrTopLeft);
    FillSolidRectHdc(hdc, x + cx, y, -1, cy, clrBottomRight);
    FillSolidRectHdc(hdc, x, y + cy, cx, -1, clrBottomRight);
}

// CWaitCursor: retail's `CWaitCursor wait;` is AfxGetApp()->BeginWaitCursor()
// (via AfxGetModuleState()->m_pCurrentWinApp) with EndWaitCursor in the dtor.
struct WaitCursorScope {
    CWinApp* pApp;
    WaitCursorScope() : pApp(impl__AfxGetApp__YAPEAVCWinApp__XZ()) {
        if (pApp) impl__BeginWaitCursor_CCmdTarget__QEAAXXZ(pApp);
    }
    ~WaitCursorScope() {
        if (pApp) impl__EndWaitCursor_CCmdTarget__QEAAXXZ(pApp);
    }
};

// The `CString strPrev = pSel->FormatProperty(); CWaitCursor wait;
// pSel->OnClickButton(pt); if (strPrev != pSel->FormatProperty())
// OnPropertyChanged(pSel);` sequence (retail compares the two buffers with
// wcscmp, i.e. CString::operator!=) shared by OnClickButton (0xc8fd0), two
// OnKeyDown arms (0xca3c0) and the WM_SYSKEYDOWN arm of PreTranslateMessage
// (0xc91b0), all mfc140u.  In every one of them retail takes strPrev BEFORE
// BeginWaitCursor (FormatProperty at vslot 0x58, then ?AfxGetModuleState@@ +
// ?BeginWaitCursor@CCmdTarget@@), so the wait-cursor scope lives here, after
// the first FormatProperty, and EndWaitCursor runs after OnPropertyChanged as
// the CWaitCursor destructor does.
void ClickButtonAndNotify(CMFCPropertyGridCtrl* pThis, CMFCPropertyGridProperty* pProp, CPoint point) {
    CString strPrev = pProp->FormatProperty();
    WaitCursorScope wait;
    impl__OnClickButton_CMFCPropertyGridProperty__UEAAXVCPoint___Z(pProp, point);
    CString strNew = pProp->FormatProperty();
    if (strPrev.Compare(strNew) != 0) {
        impl__OnPropertyChanged_CMFCPropertyGridCtrl__UEBAXPEAVCMFCPropertyGridProperty___Z(pThis, pProp);
    }
}

} // namespace


// Symbol: ?get_accChildCount@CMFCPropertyGridCtrl@@UEAAJPEAJ@Z
extern "C" HRESULT MS_ABI impl__get_accChildCount_CMFCPropertyGridCtrl__UEAAJPEAJ_Z(
    CMFCPropertyGridCtrl* pThis, long* pCountChildren) {
    if (!pCountChildren) return E_POINTER;
    *pCountChildren = pThis ? static_cast<long>(VisiblePropertyGridItems(pThis).size()) : 0;
    return S_OK;
}
// Symbol: ?get_accChild@CMFCPropertyGridCtrl@@UEAAJUtagVARIANT@@PEAPEAUIDispatch@@@Z
extern "C" HRESULT MS_ABI impl__get_accChild_CMFCPropertyGridCtrl__UEAAJUtagVARIANT__PEAPEAUIDispatch___Z(
    CMFCPropertyGridCtrl*, VARIANT varChild, IDispatch** ppdispChild) {
    if (!ppdispChild) return E_POINTER;
    *ppdispChild = nullptr;
    return PropertyGridChildIDFromVariant(varChild) >= CHILDID_SELF ? S_FALSE : E_INVALIDARG;
}
// Symbol: ?get_accName@CMFCPropertyGridCtrl@@UEAAJUtagVARIANT@@PEAPEA_W@Z
extern "C" HRESULT MS_ABI impl__get_accName_CMFCPropertyGridCtrl__UEAAJUtagVARIANT__PEAPEA_W_Z(
    CMFCPropertyGridCtrl* pThis, VARIANT varChild, BSTR* pszName) {
    if (!pszName) return E_POINTER;
    *pszName = nullptr;
    int childID = PropertyGridChildIDFromVariant(varChild);
    if (childID == CHILDID_SELF) return AllocPropertyGridString(L"Property Grid", pszName);
    CMFCPropertyGridProperty* prop = PropertyGridPropertyFromChildID(pThis, childID);
    if (!prop) return E_INVALIDARG;
    return AllocPropertyGridString(static_cast<const wchar_t*>(prop->GetName()), pszName);
}
// Symbol: ?get_accValue@CMFCPropertyGridCtrl@@UEAAJUtagVARIANT@@PEAPEA_W@Z
extern "C" HRESULT MS_ABI impl__get_accValue_CMFCPropertyGridCtrl__UEAAJUtagVARIANT__PEAPEA_W_Z(
    CMFCPropertyGridCtrl* pThis, VARIANT varChild, BSTR* pszValue) {
    if (!pszValue) return E_POINTER;
    *pszValue = nullptr;
    int childID = PropertyGridChildIDFromVariant(varChild);
    CMFCPropertyGridProperty* prop = childID == CHILDID_SELF ? (FindPropertyGridCtrlState(pThis) ? FindPropertyGridCtrlState(pThis)->current : nullptr)
                                                             : PropertyGridPropertyFromChildID(pThis, childID);
    if (!prop) return S_FALSE;
    CString value = prop->FormatProperty();
    return AllocPropertyGridString(static_cast<const wchar_t*>(value), pszValue);
}
// Symbol: ?get_accDescription@CMFCPropertyGridCtrl@@UEAAJUtagVARIANT@@PEAPEA_W@Z
extern "C" HRESULT MS_ABI impl__get_accDescription_CMFCPropertyGridCtrl__UEAAJUtagVARIANT__PEAPEA_W_Z(
    CMFCPropertyGridCtrl* pThis, VARIANT varChild, BSTR* pszDescription) {
    return impl__get_accName_CMFCPropertyGridCtrl__UEAAJUtagVARIANT__PEAPEA_W_Z(pThis, varChild, pszDescription);
}
// Symbol: ?get_accDefaultAction@CMFCPropertyGridCtrl@@UEAAJUtagVARIANT@@PEAPEA_W@Z
extern "C" HRESULT MS_ABI impl__get_accDefaultAction_CMFCPropertyGridCtrl__UEAAJUtagVARIANT__PEAPEA_W_Z(
    CMFCPropertyGridCtrl* pThis, VARIANT varChild, BSTR* pszDefaultAction) {
    int childID = PropertyGridChildIDFromVariant(varChild);
    if (childID == CHILDID_SELF) return S_FALSE;
    CMFCPropertyGridProperty* prop = PropertyGridPropertyFromChildID(pThis, childID);
    if (!prop) return E_INVALIDARG;
    return AllocPropertyGridString(prop->GetSubItemsCount() > 0 ? L"Expand or collapse" : L"Edit", pszDefaultAction);
}
// Symbol: ?get_accRole@CMFCPropertyGridCtrl@@UEAAJUtagVARIANT@@PEAU2@@Z
extern "C" HRESULT MS_ABI impl__get_accRole_CMFCPropertyGridCtrl__UEAAJUtagVARIANT__PEAU2__Z(
    CMFCPropertyGridCtrl* pThis, VARIANT varChild, VARIANT* pvarRole) {
    (void)pThis;
    if (!pvarRole) return E_POINTER;
    int childID = PropertyGridChildIDFromVariant(varChild);
    if (childID < CHILDID_SELF) return E_INVALIDARG;
    VariantInit(pvarRole);
    pvarRole->vt = VT_I4;
    pvarRole->lVal = childID == CHILDID_SELF ? ROLE_SYSTEM_OUTLINE : ROLE_SYSTEM_OUTLINEITEM;
    return S_OK;
}
// Symbol: ?get_accState@CMFCPropertyGridCtrl@@UEAAJUtagVARIANT@@PEAU2@@Z
extern "C" HRESULT MS_ABI impl__get_accState_CMFCPropertyGridCtrl__UEAAJUtagVARIANT__PEAU2__Z(
    CMFCPropertyGridCtrl* pThis, VARIANT varChild, VARIANT* pvarState) {
    if (!pvarState) return E_POINTER;
    int childID = PropertyGridChildIDFromVariant(varChild);
    if (childID < CHILDID_SELF) return E_INVALIDARG;
    VariantInit(pvarState);
    pvarState->vt = VT_I4;
    pvarState->lVal = 0;
    if (childID == CHILDID_SELF) return S_OK;

    CMFCPropertyGridProperty* prop = PropertyGridPropertyFromChildID(pThis, childID);
    if (!prop) return E_INVALIDARG;
    const PropertyGridCtrlState* ctrlState = FindPropertyGridCtrlState(pThis);
    if (!prop->IsEnabled()) pvarState->lVal |= STATE_SYSTEM_UNAVAILABLE;
    if (!prop->IsVisible()) pvarState->lVal |= STATE_SYSTEM_INVISIBLE;
    if (ctrlState && ctrlState->current == prop) pvarState->lVal |= STATE_SYSTEM_SELECTED | STATE_SYSTEM_FOCUSED;
    if (prop->GetSubItemsCount() > 0) pvarState->lVal |= prop->IsExpanded() ? STATE_SYSTEM_EXPANDED : STATE_SYSTEM_COLLAPSED;
    return S_OK;
}
// Symbol: ?get_accFocus@CMFCPropertyGridCtrl@@UEAAJPEAUtagVARIANT@@@Z
extern "C" HRESULT MS_ABI impl__get_accFocus_CMFCPropertyGridCtrl__UEAAJPEAUtagVARIANT___Z(
    CMFCPropertyGridCtrl* pThis, VARIANT* pvarChild) {
    if (!pvarChild) return E_POINTER;
    const PropertyGridCtrlState* state = FindPropertyGridCtrlState(pThis);
    SetPropertyGridVariantChild(pvarChild, state ? PropertyGridChildIDFromProperty(pThis, state->current) : CHILDID_SELF);
    return S_OK;
}
// Symbol: ?get_accSelection@CMFCPropertyGridCtrl@@UEAAJPEAUtagVARIANT@@@Z
extern "C" HRESULT MS_ABI impl__get_accSelection_CMFCPropertyGridCtrl__UEAAJPEAUtagVARIANT___Z(
    CMFCPropertyGridCtrl* pThis, VARIANT* pvarChildren) {
    return impl__get_accFocus_CMFCPropertyGridCtrl__UEAAJPEAUtagVARIANT___Z(pThis, pvarChildren);
}
// Symbol: ?accSelect@CMFCPropertyGridCtrl@@UEAAJJUtagVARIANT@@@Z
extern "C" HRESULT MS_ABI impl__accSelect_CMFCPropertyGridCtrl__UEAAJJUtagVARIANT___Z(
    CMFCPropertyGridCtrl* pThis, long, VARIANT varChild) {
    int childID = PropertyGridChildIDFromVariant(varChild);
    CMFCPropertyGridProperty* prop = PropertyGridPropertyFromChildID(pThis, childID);
    if (!prop) return E_INVALIDARG;
    pThis->SetCurSel(prop, TRUE);
    return S_OK;
}
// Symbol: ?accHitTest@CMFCPropertyGridCtrl@@UEAAJJJPEAUtagVARIANT@@@Z
extern "C" HRESULT MS_ABI impl__accHitTest_CMFCPropertyGridCtrl__UEAAJJJPEAUtagVARIANT___Z(
    CMFCPropertyGridCtrl* pThis, long xLeft, long yTop, VARIANT* pvarChild) {
    if (!pvarChild) return E_POINTER;
    POINT pt{xLeft, yTop};
    if (pThis && pThis->GetSafeHwnd()) ::ScreenToClient(pThis->GetSafeHwnd(), &pt);
    CMFCPropertyGridProperty* prop = PropertyGridHitTestByPoint(pThis, CPoint(pt.x, pt.y));
    SetPropertyGridVariantChild(pvarChild, PropertyGridChildIDFromProperty(pThis, prop));
    return S_OK;
}
// Symbol: ?accLocation@CMFCPropertyGridCtrl@@UEAAJPEAJ000UtagVARIANT@@@Z
extern "C" HRESULT MS_ABI impl__accLocation_CMFCPropertyGridCtrl__UEAAJPEAJ000UtagVARIANT___Z(
    CMFCPropertyGridCtrl* pThis, long* pxLeft, long* pyTop, long* pcxWidth, long* pcyHeight, VARIANT varChild) {
    if (!pxLeft || !pyTop || !pcxWidth || !pcyHeight) return E_POINTER;
    int childID = PropertyGridChildIDFromVariant(varChild);
    CRect itemRect = PropertyGridItemRect(pThis, childID);
    POINT pt{itemRect.left, itemRect.top};
    if (pThis && pThis->GetSafeHwnd()) ::ClientToScreen(pThis->GetSafeHwnd(), &pt);
    *pxLeft = pt.x;
    *pyTop = pt.y;
    *pcxWidth = itemRect.Width();
    *pcyHeight = itemRect.Height();
    return S_OK;
}
// Symbol: ?get_accHelp@CMFCPropertyGridCtrl@@UEAAJUtagVARIANT@@PEAPEA_W@Z
extern "C" HRESULT MS_ABI impl__get_accHelp_CMFCPropertyGridCtrl__UEAAJUtagVARIANT__PEAPEA_W_Z(
    CMFCPropertyGridCtrl*, VARIANT, BSTR* pszHelp) {
    if (!pszHelp) return E_POINTER;
    *pszHelp = nullptr;
    return S_FALSE;
}
// Symbol: ?get_accHelpTopic@CMFCPropertyGridCtrl@@UEAAJPEAPEA_WUtagVARIANT@@PEAJ@Z
extern "C" HRESULT MS_ABI impl__get_accHelpTopic_CMFCPropertyGridCtrl__UEAAJPEAPEA_WUtagVARIANT__PEAJ_Z(
    CMFCPropertyGridCtrl*, BSTR* pszHelpFile, VARIANT, long* pidTopic) {
    if (pszHelpFile) *pszHelpFile = nullptr;
    if (pidTopic) *pidTopic = 0;
    return S_FALSE;
}
// Symbol: ?get_accKeyboardShortcut@CMFCPropertyGridCtrl@@UEAAJUtagVARIANT@@PEAPEA_W@Z
extern "C" HRESULT MS_ABI impl__get_accKeyboardShortcut_CMFCPropertyGridCtrl__UEAAJUtagVARIANT__PEAPEA_W_Z(
    CMFCPropertyGridCtrl*, VARIANT, BSTR* pszKeyboardShortcut) {
    if (!pszKeyboardShortcut) return E_POINTER;
    *pszKeyboardShortcut = nullptr;
    return S_FALSE;
}
// Symbol: ?EditItem@CMFCPropertyGridCtrl@@UEAAHPEAVCMFCPropertyGridProperty@@PEAUtagPOINT@@@Z
extern "C" int MS_ABI impl__EditItem_CMFCPropertyGridCtrl__UEAAHPEAVCMFCPropertyGridProperty__PEAUtagPOINT___Z(
    CMFCPropertyGridCtrl* pThis, CMFCPropertyGridProperty* pProp, POINT* point) {
    (void)point;
    if (!pThis || !pProp || !pProp->IsEnabled()) return FALSE;
    pThis->SetCurSel(pProp, TRUE);
    return TRUE;
}
// Symbol: ?EndEditItem@CMFCPropertyGridCtrl@@UEAAHH@Z
extern "C" int MS_ABI impl__EndEditItem_CMFCPropertyGridCtrl__UEAAHH_Z(CMFCPropertyGridCtrl* pThis, int bUpdateData) {
    const PropertyGridCtrlState* state = FindPropertyGridCtrlState(pThis);
    if (!state || !state->current) return FALSE;
    return bUpdateData ? state->current->OnUpdateValue() : state->current->OnEndEdit();
}
// Symbol: ?EnsureVisible@CMFCPropertyGridCtrl@@QEAAXPEAVCMFCPropertyGridProperty@@H@Z
extern "C" void MS_ABI impl__EnsureVisible_CMFCPropertyGridCtrl__QEAAXPEAVCMFCPropertyGridProperty__H_Z(
    CMFCPropertyGridCtrl* pThis, CMFCPropertyGridProperty* pProp, int bExpandParents) {
    if (!pThis || !pProp) return;
    if (bExpandParents) {
        PropertyGridPropertyState* state = FindMutablePropertyGridPropertyState(pProp);
        while (state && state->parent) {
            state->parent->Expand(TRUE);
            state = FindMutablePropertyGridPropertyState(state->parent);
        }
    }
    pThis->SetCurSel(pProp, TRUE);
}
// Symbol: ?HitTest@CMFCPropertyGridCtrl@@QEBAPEAVCMFCPropertyGridProperty@@VCPoint@@PEAW4ClickArea@2@H@Z
extern "C" CMFCPropertyGridProperty* MS_ABI impl__HitTest_CMFCPropertyGridCtrl__QEBAPEAVCMFCPropertyGridProperty__VCPoint__PEAW4ClickArea_2_H_Z(
    CMFCPropertyGridCtrl* pThis, CPoint point, int* pClickArea, int) {
    if (pClickArea) *pClickArea = point.x < 120 ? 1 : 2;
    return PropertyGridHitTestByPoint(pThis, point);
}
// Symbol: ?OnDrawProperty@CMFCPropertyGridCtrl@@UEBAHPEAVCDC@@PEAVCMFCPropertyGridProperty@@@Z
extern "C" int MS_ABI impl__OnDrawProperty_CMFCPropertyGridCtrl__UEBAHPEAVCDC__PEAVCMFCPropertyGridProperty___Z(
    CMFCPropertyGridCtrl* pThis, CDC* pDC, CMFCPropertyGridProperty* pProp) {
    if (!pDC || !pDC->GetSafeHdc() || !pProp) return FALSE;
    int childID = PropertyGridChildIDFromProperty(pThis, pProp);
    CRect rect = PropertyGridItemRect(pThis, childID);
    COLORREF back = pProp->IsEnabled() ? ::GetSysColor(COLOR_WINDOW) : ::GetSysColor(COLOR_BTNFACE);
    ::SetBkColor(pDC->GetSafeHdc(), back);
    RECT nativeRect = NativeRect(rect);
    ::ExtTextOutW(pDC->GetSafeHdc(), 0, 0, ETO_OPAQUE, &nativeRect, nullptr, 0, nullptr);
    CRect nameRect(rect.left + 4 + pProp->GetHierarchyLevel() * 12, rect.top, rect.left + rect.Width() / 2, rect.bottom);
    CRect valueRect(rect.left + rect.Width() / 2 + 4, rect.top, rect.right - 2, rect.bottom);
    impl__OnDrawName_CMFCPropertyGridProperty__UEAAXPEAVCDC__VCRect___Z(pProp, pDC, nameRect);
    impl__OnDrawValue_CMFCPropertyGridProperty__UEAAXPEAVCDC__VCRect___Z(pProp, pDC, valueRect);
    return TRUE;
}
// Symbol: ?OnDraw@CMFCPropertyGridCtrl@@MEAAXPEAVCDC@@@Z
extern "C" void MS_ABI impl__OnDraw_CMFCPropertyGridCtrl__MEAAXPEAVCDC___Z(CMFCPropertyGridCtrl* pThis, CDC* pDC) {
    if (!pThis || !pDC || !pDC->GetSafeHdc()) return;
    for (CMFCPropertyGridProperty* prop : VisiblePropertyGridItems(pThis)) {
        impl__OnDrawProperty_CMFCPropertyGridCtrl__UEBAHPEAVCDC__PEAVCMFCPropertyGridProperty___Z(pThis, pDC, prop);
    }
}
// Symbol: ?OnFillBackground@CMFCPropertyGridCtrl@@MEAAXPEAVCDC@@VCRect@@@Z
extern "C" void MS_ABI impl__OnFillBackground_CMFCPropertyGridCtrl__MEAAXPEAVCDC__VCRect___Z(
    CMFCPropertyGridCtrl*, CDC* pDC, CRect rect) {
    if (!pDC || !pDC->GetSafeHdc()) return;
    RECT nativeRect = NativeRect(rect);
    ::ExtTextOutW(pDC->GetSafeHdc(), 0, 0, ETO_OPAQUE, &nativeRect, nullptr, 0, nullptr);
}
// Symbol: ?OnDrawList@CMFCPropertyGridCtrl@@MEAAXPEAVCDC@@@Z
extern "C" void MS_ABI impl__OnDrawList_CMFCPropertyGridCtrl__MEAAXPEAVCDC___Z(CMFCPropertyGridCtrl* pThis, CDC* pDC) {
    impl__OnDraw_CMFCPropertyGridCtrl__MEAAXPEAVCDC___Z(pThis, pDC);
}
// Symbol: ?SetCustomColors@CMFCPropertyGridCtrl@@QEAAXKKKKKKK@Z
extern "C" void MS_ABI impl__SetCustomColors_CMFCPropertyGridCtrl__QEAAXKKKKKKK_Z(
    CMFCPropertyGridCtrl* pThis, COLORREF c1, COLORREF c2, COLORREF c3, COLORREF c4, COLORREF c5, COLORREF c6, COLORREF c7) {
    if (!pThis) return;
    PropertyGridCtrlState& state = EnsurePropertyGridCtrlState(pThis);
    COLORREF values[7] = {c1, c2, c3, c4, c5, c6, c7};
    std::copy(std::begin(values), std::end(values), std::begin(state.customColors));
}
// Symbol: ?GetCustomColors@CMFCPropertyGridCtrl@@QEAAXAEAK000000@Z
extern "C" void MS_ABI impl__GetCustomColors_CMFCPropertyGridCtrl__QEAAXAEAK000000_Z(
    CMFCPropertyGridCtrl* pThis, COLORREF* c1, COLORREF* c2, COLORREF* c3, COLORREF* c4, COLORREF* c5, COLORREF* c6, COLORREF* c7) {
    const PropertyGridCtrlState* state = FindPropertyGridCtrlState(pThis);
    const COLORREF defaults[7] = {RGB(0, 0, 0), RGB(128, 128, 128), RGB(128, 0, 0), RGB(0, 128, 0), RGB(0, 0, 128), RGB(255, 255, 255), RGB(255, 255, 0)};
    const COLORREF* colors = state ? state->customColors : defaults;
    if (c1) *c1 = colors[0];
    if (c2) *c2 = colors[1];
    if (c3) *c3 = colors[2];
    if (c4) *c4 = colors[3];
    if (c5) *c5 = colors[4];
    if (c6) *c6 = colors[5];
    if (c7) *c7 = colors[6];
}
// Symbol: ?EnableDescriptionArea@CMFCPropertyGridCtrl@@QEAAXH@Z
extern "C" void MS_ABI impl__EnableDescriptionArea_CMFCPropertyGridCtrl__QEAAXH_Z(CMFCPropertyGridCtrl* pThis, int bEnable) {
    if (pThis) pThis->SetDescriptionRows(bEnable ? 2 : 0);
}
// Symbol: ?Init@CMFCPropertyGridCtrl@@MEAAXXZ
extern "C" void MS_ABI impl__Init_CMFCPropertyGridCtrl__MEAAXXZ(CMFCPropertyGridCtrl* pThis) {
    if (pThis) EnsurePropertyGridCtrlState(pThis);
}
// Symbol: ?NotifyAccessibility@CMFCPropertyGridCtrl@@MEAAXPEAVCMFCPropertyGridProperty@@@Z
extern "C" void MS_ABI impl__NotifyAccessibility_CMFCPropertyGridCtrl__MEAAXPEAVCMFCPropertyGridProperty___Z(
    CMFCPropertyGridCtrl* pThis, CMFCPropertyGridProperty* pProp) {
    (void)pProp;
    TouchPropertyGridCtrl(pThis, FALSE);
}
// Symbol: ?CalcEditMargin@CMFCPropertyGridCtrl@@IEAAXXZ
extern "C" void MS_ABI impl__CalcEditMargin_CMFCPropertyGridCtrl__IEAAXXZ(CMFCPropertyGridCtrl* pThis) {
    TouchPropertyGridCtrl(pThis, FALSE);
}
CMFCPropertyGridCtrl::CMFCPropertyGridCtrl() {
    memset(_propgridctrl_padding, 0, sizeof(_propgridctrl_padding));
    EnsurePropertyGridCtrlState(this);
}
CMFCPropertyGridCtrl::~CMFCPropertyGridCtrl() {
    PropertyGridCtrlState* state = FindMutablePropertyGridCtrlState(this);
    if (state) {
        std::vector<CMFCPropertyGridProperty*> properties = state->properties;
        state->properties.clear();
        state->current = nullptr;
        for (CMFCPropertyGridProperty* prop : properties) {
            SetPropertyGridOwnerRecursive(prop, nullptr);
            delete prop;
        }
    }
    g_propertyGridCtrlStates.erase(this);
    // Window-level side table (see PropertyGridWndState above): the bold font is
    // the only owned handle in it (retail: CFont m_fontBold's own destructor).
    EraseWndState(this);
}
BOOL CMFCPropertyGridCtrl::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID) {
    EnsurePropertyGridCtrlState(this);
    return CWnd::CreateEx(0, L"SysListView32", L"", dwStyle, rect.left, rect.top,
                          rect.right - rect.left, rect.bottom - rect.top,
                          pParentWnd ? pParentWnd->GetSafeHwnd() : nullptr,
                          reinterpret_cast<HMENU>(static_cast<UINT_PTR>(nID)), this);
}
int CMFCPropertyGridCtrl::AddProperty(CMFCPropertyGridProperty* pProp, int nPos, int bRedraw) {
    if (!pProp) return -1;

    auto& properties = EnsurePropertyGridCtrlState(this).properties;
    properties.erase(std::remove(properties.begin(), properties.end(), pProp), properties.end());
    DetachPropertyFromParent(pProp);
    DetachPropertyFromGrid(pProp);
    PropertyGridPropertyState& propState = EnsurePropertyGridPropertyState(pProp);
    propState.parent = nullptr;
    SetPropertyGridOwnerRecursive(pProp, this);

    if (nPos < 0 || nPos > static_cast<int>(properties.size())) {
        properties.push_back(pProp);
        if (bRedraw) {
            TouchPropertyGridCtrl(this, TRUE);
        }
        return static_cast<int>(properties.size() - 1);
    }

    auto it = properties.begin() + nPos;
    properties.insert(it, pProp);
    if (bRedraw) {
        TouchPropertyGridCtrl(this, TRUE);
    }
    return nPos;
}
int CMFCPropertyGridCtrl::GetPropertyCount() const {
    const PropertyGridCtrlState* state = FindPropertyGridCtrlState(this);
    return state ? static_cast<int>(state->properties.size()) : 0;
}
CMFCPropertyGridProperty* CMFCPropertyGridCtrl::GetProperty(int nIndex) const {
    const PropertyGridCtrlState* state = FindPropertyGridCtrlState(this);
    if (!state || nIndex < 0 || nIndex >= static_cast<int>(state->properties.size())) return nullptr;
    return state->properties[static_cast<size_t>(nIndex)];
}
BOOL CMFCPropertyGridCtrl::DeleteProperty(CMFCPropertyGridProperty*& pProp, BOOL bRedraw, BOOL bAdjustLayout) {
    if (!pProp) return FALSE;
    PropertyGridCtrlState& state = EnsurePropertyGridCtrlState(this);
    auto top = std::find(state.properties.begin(), state.properties.end(), pProp);
    if (top != state.properties.end()) {
        CMFCPropertyGridProperty* removed = *top;
        state.properties.erase(top);
        if (state.current == removed || IsPropertyGridAncestorOf(removed, state.current)) {
            state.current = nullptr;
        }
        SetPropertyGridOwnerRecursive(removed, nullptr);
        delete removed;
        pProp = nullptr;
        if (bRedraw) {
            TouchPropertyGridCtrl(this, bAdjustLayout);
        }
        return TRUE;
    }

    PropertyGridPropertyState* propState = FindMutablePropertyGridPropertyState(pProp);
    if (propState && propState->parent) {
        if (propState->owner != this) return FALSE;
        CMFCPropertyGridProperty* parent = propState->parent;
        CMFCPropertyGridProperty* removed = pProp;
        if (state.current == removed || IsPropertyGridAncestorOf(removed, state.current)) {
            state.current = nullptr;
        }
        if (parent->RemoveSubItem(pProp, TRUE)) {
            if (bRedraw) {
                TouchPropertyGridCtrl(this, bAdjustLayout);
            }
            return TRUE;
        }
    }
    return FALSE;
}
BOOL CMFCPropertyGridCtrl::DeleteProperty(void*& pProp, BOOL bRedraw, BOOL bAdjustLayout) {
    CMFCPropertyGridProperty* typed = static_cast<CMFCPropertyGridProperty*>(pProp);
    BOOL result = DeleteProperty(typed, bRedraw, bAdjustLayout);
    pProp = typed;
    return result;
}
CMFCPropertyGridProperty* CMFCPropertyGridCtrl::FindItemByData(DWORD_PTR dwData, BOOL bSearchSubItems) const {
    const PropertyGridCtrlState* state = FindPropertyGridCtrlState(this);
    if (!state) return nullptr;
    for (CMFCPropertyGridProperty* prop : state->properties) {
        if (!prop) continue;
        if (prop->GetData() == dwData) return prop;
        if (bSearchSubItems) {
            if (CMFCPropertyGridProperty* found = prop->FindSubItemByData(dwData)) {
                return found;
            }
        }
    }
    return nullptr;
}
void CMFCPropertyGridCtrl::RemoveAll() {
    auto& state = EnsurePropertyGridCtrlState(this);
    std::vector<CMFCPropertyGridProperty*> properties = state.properties;
    state.properties.clear();
    state.current = nullptr;
    for (CMFCPropertyGridProperty* pProp : properties) {
        SetPropertyGridOwnerRecursive(pProp, nullptr);
        delete pProp;
    }
    TouchPropertyGridCtrl(this, TRUE);
}
void CMFCPropertyGridCtrl::ExpandAll(BOOL bExpand) {
    const PropertyGridCtrlState* state = FindPropertyGridCtrlState(this);
    if (!state) return;
    for (CMFCPropertyGridProperty* pProp : state->properties) {
        ExpandPropertyRecursive(pProp, bExpand);
    }
    TouchPropertyGridCtrl(this, TRUE);
}
void CMFCPropertyGridCtrl::AdjustLayout() {
    ++EnsurePropertyGridCtrlState(this).layoutRevision;
    Invalidate(FALSE);
}
void CMFCPropertyGridCtrl::SetDescriptionRows(int nRows) {
    EnsurePropertyGridCtrlState(this).descriptionRows = (nRows < 0) ? 0 : nRows;
    TouchPropertyGridCtrl(this, TRUE);
}
void CMFCPropertyGridCtrl::SetCurSel(CMFCPropertyGridProperty* pProp, BOOL bRedraw) {
    auto& state = EnsurePropertyGridCtrlState(this);
    if (pProp) {
        const PropertyGridPropertyState* propState = FindPropertyGridPropertyState(pProp);
        if (!propState || propState->owner != this) return;
    }
    state.current = pProp;
    if (bRedraw) {
        TouchPropertyGridCtrl(this, FALSE);
    }
}
void CMFCPropertyGridCtrl::ResetOriginalValues(BOOL bRedraw) {
    const PropertyGridCtrlState* state = FindPropertyGridCtrlState(this);
    if (!state) return;
    for (CMFCPropertyGridProperty* prop : state->properties) {
        ResetPropertyGridOriginalValueRecursive(prop);
    }
    if (bRedraw) {
        TouchPropertyGridCtrl(this, FALSE);
    }
}
void CMFCPropertyGridCtrl::MarkModifiedProperties(BOOL bModified, BOOL bRedraw) {
    const PropertyGridCtrlState* state = FindPropertyGridCtrlState(this);
    if (!state) return;
    for (CMFCPropertyGridProperty* prop : state->properties) {
        MarkPropertyGridModifiedRecursive(prop, bModified);
    }
    if (bRedraw) {
        TouchPropertyGridCtrl(this, FALSE);
    }
}
void CMFCPropertyGridCtrl::SetBoolLabels(const wchar_t* lpszTrue, const wchar_t* lpszFalse) {
    auto& state = EnsurePropertyGridCtrlState(this);
    state.boolTrue = lpszTrue ? lpszTrue : L"";
    state.boolFalse = lpszFalse ? lpszFalse : L"";
    TouchPropertyGridCtrl(this, FALSE);
}
void CMFCPropertyGridCtrl::SetListDelimiter(wchar_t c) {
    EnsurePropertyGridCtrlState(this).listDelimiter = c;
    TouchPropertyGridCtrl(this, FALSE);
}
void CMFCPropertyGridCtrl::SetAlphabeticMode(BOOL bSet) {
    auto& state = EnsurePropertyGridCtrlState(this);
    state.alphabeticMode = bSet ? TRUE : FALSE;
    if (state.alphabeticMode) {
        auto compareProps = [this](const CMFCPropertyGridProperty* lhs, const CMFCPropertyGridProperty* rhs) {
            return CompareProps(lhs, rhs) < 0;
        };
        std::stable_sort(state.properties.begin(), state.properties.end(),
            compareProps);
        for (CMFCPropertyGridProperty* prop : state.properties) {
            SortPropertyGridSubItemsRecursive(prop, compareProps);
        }
    }
    TouchPropertyGridCtrl(this, TRUE);
}
void CMFCPropertyGridCtrl::SetGroupNameFullWidth(BOOL bSet, BOOL bRedraw) {
    EnsurePropertyGridCtrlState(this).groupNameFullWidth = bSet ? TRUE : FALSE;
    if (bRedraw) {
        TouchPropertyGridCtrl(this, TRUE);
    }
}
int CMFCPropertyGridCtrl::GetTotalItems(BOOL bIncludeHidden) const {
    const PropertyGridCtrlState* state = FindPropertyGridCtrlState(this);
    if (!state) return 0;
    int count = 0;
    for (CMFCPropertyGridProperty* prop : state->properties) {
        count += CountPropertyGridItems(prop, bIncludeHidden);
    }
    return count;
}
int CMFCPropertyGridCtrl::CompareProps(const CMFCPropertyGridProperty* pProp1, const CMFCPropertyGridProperty* pProp2) const {
    if (pProp1 == pProp2) return 0;
    if (!pProp1) return -1;
    if (!pProp2) return 1;
    return pProp1->GetName().Compare(pProp2->GetName());
}
// Symbol: ?CloseColorPopup@CMFCPropertyGridCtrl@@UEAAXXZ
// Transcribed from retail entry RVA 0xcadc0 (mfc140u):
//     CMFCPropertyGridProperty* pSel = m_pSel;                              // +0x678
//     if (pSel != NULL && pSel->IsKindOf(RUNTIME_CLASS(CMFCPropertyGridColorProperty))) {
//         ((CMFCPropertyGridColorProperty*)pSel)->m_pPopup = NULL;          // +0x180
//         pSel->m_bButtonIsDown = FALSE;                                    // +0x64
//         pSel->Redraw();                                                   // ?Redraw@CMFCPropertyGridProperty@@ 0xc1730 (mfc140u)
//         if (pSel->m_pWndInPlace != NULL) pSel->m_pWndInPlace->SetFocus(); // +0x100
//     }
// DEVIATION: OpenMFC's colour property (CMFCPropertyGridColorProperty.cpp) keeps
// no popup pointer and no button-down flag, and no in-place edit window is ever
// created, so of the four statements only the Redraw has an effect here.
extern "C" void MS_ABI impl__CloseColorPopup_CMFCPropertyGridCtrl__UEAAXXZ(CMFCPropertyGridCtrl* pThis) {
    if (!pThis) return;
    CMFCPropertyGridProperty* pSel = CurSelOf(pThis);
    if (pSel == nullptr) return;
    if (!impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(
            pSel, impl__GetThisClass_CMFCPropertyGridColorProperty__SAPEAUCRuntimeClass__XZ())) {
        return;
    }
    impl__Redraw_CMFCPropertyGridProperty__QEAAXXZ(pSel);
}

// Symbol: ?CreateBoldFont@CMFCPropertyGridCtrl@@IEAAXXZ
// Transcribed from retail entry RVA 0xc6c20 (mfc140u):
//     if (m_fontBold.GetSafeHandle() != NULL) m_fontBold.DeleteObject();        // +0x580 / +0x588
//     HFONT hFont = m_hFont != NULL ? m_hFont : (HFONT)::GetStockObject(DEFAULT_GUI_FONT);   // +0x578, 0x11
//     CFont* pFont = CFont::FromHandle(hFont);                                   // ?FromHandle@CGdiObject@@
//     LOGFONTW lf; memset(&lf, 0, sizeof(LOGFONTW) /* 0x5c in mfc140u; the ANSI twin uses LOGFONTA, 0x3c */);
//     pFont->GetLogFont(&lf);                                                    // ::GetObjectW(hFont, 0x5c, &lf)
//     lf.lfWeight = FW_BOLD;                                                     // 0x2bc at +0x10
//     m_fontBold.Attach(::CreateFontIndirectW(&lf));
// The CFont member has no OpenMFC home; the side table owns the HFONT directly
// (DeleteObject / CreateFontIndirectW on the handle), which is what CFont's
// DeleteObject / Attach reduce to.  The CGdiObject::FromHandle round trip is
// skipped: GetObject reads the same handle either way.
extern "C" void MS_ABI impl__CreateBoldFont_CMFCPropertyGridCtrl__IEAAXXZ(CMFCPropertyGridCtrl* pThis) {
    if (!pThis) return;
    PropertyGridWndState& wnd = WndState(pThis);
    if (wnd.hFontBold != nullptr) {
        ::DeleteObject(wnd.hFontBold);
        wnd.hFontBold = nullptr;
    }
    HFONT hFont = wnd.hFont != nullptr ? wnd.hFont : static_cast<HFONT>(::GetStockObject(DEFAULT_GUI_FONT));
    LOGFONTW lf;
    std::memset(&lf, 0, sizeof(lf));
    ::GetObjectW(hFont, sizeof(lf), &lf);
    lf.lfWeight = FW_BOLD;
    wnd.hFontBold = ::CreateFontIndirectW(&lf);
}

// Symbol: ?EnableHeaderCtrl@CMFCPropertyGridCtrl@@QEAAXHPEB_W0@Z
// Transcribed from retail entry RVA 0xc7b80 (mfc140u):
//     ENSURE(lpszLeftColumn != NULL && lpszRightColumn != NULL);   // AfxThrowInvalidArgException
//     if (m_pSel != NULL) m_pSel->OnEndEdit();                      // property vslot 16
//     m_bHeaderCtrl = bEnable;                                      // +0x548
//     if (bEnable) {
//         HDITEM hdItem; hdItem.mask = HDI_TEXT; hdItem.pszText = lpszLeftColumn; hdItem.cchTextMax = wcslen + 1;
//         GetHeaderCtrl().SetItem(0, &hdItem);                       // vslot 91 -> &m_wndHeader; HDM_SETITEM
//         hdItem.pszText = lpszRightColumn; ...; GetHeaderCtrl().SetItem(1, &hdItem);
//     }
//     AdjustLayout();                                               // vslot 94
//     RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE);   // 0x105
// DEVIATION: OpenMFC has no CMFCHeaderCtrl member (and the sibling's Create is a
// stub), so the two HDM_SETITEM calls have nowhere to go and are skipped; the
// column captions are dropped.  The ENSURE is reproduced as an early return
// rather than a throw.
extern "C" void MS_ABI impl__EnableHeaderCtrl_CMFCPropertyGridCtrl__QEAAXHPEB_W0_Z(
    CMFCPropertyGridCtrl* pThis, int bEnable, const wchar_t* lpszLeftColumn, const wchar_t* lpszRightColumn) {
    if (!pThis || lpszLeftColumn == nullptr || lpszRightColumn == nullptr) return;
    if (CMFCPropertyGridProperty* pSel = CurSelOf(pThis)) {
        pSel->OnEndEdit();
    }
    WndState(pThis).bHeaderCtrl = bEnable ? TRUE : FALSE;
    pThis->AdjustLayout();
    pThis->RedrawWindow(nullptr, nullptr, RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE);
}

// Symbol: ?GetScrollBarCtrl@CMFCPropertyGridCtrl@@UEBAPEAVCScrollBar@@H@Z
// Retail entry RVA 0xc9fb0 (mfc140u):
//     if (nBar == SB_HORZ) return NULL;
//     return m_wndScrollVert.GetSafeHwnd() != NULL ? &m_wndScrollVert : NULL;   // +0x460, +0x4a0
// STUB: OpenMFC has no embedded CScrollBar member to hand back (nothing creates
// one -- this file's Init does not run retail's m_wndScrollVert.Create), so the
// only honest answer is the "not created" branch for both bars.
extern "C" CScrollBar* MS_ABI impl__GetScrollBarCtrl_CMFCPropertyGridCtrl__UEBAPEAVCScrollBar__H_Z(
    const CMFCPropertyGridCtrl* pThis, int nBar) {
    (void)pThis; (void)nBar;
    return nullptr;
}

// Symbol: ?InitHeader@CMFCPropertyGridCtrl@@UEAAXXZ
// Retail entry RVA 0xc66c0 (mfc140u):
//     CRect rectDummy; rectDummy.SetRectEmpty();
//     GetHeaderCtrl().Create(WS_CHILD | WS_VISIBLE, rectDummy, this, 1);   // vslot 91 -> +0x208; CWnd vslot 91
// STUB: OpenMFC has no CMFCHeaderCtrl member to create, and the sibling's
// ?Create@CMFCHeaderCtrl@@ is itself a stub.
extern "C" void MS_ABI impl__InitHeader_CMFCPropertyGridCtrl__UEAAXXZ(CMFCPropertyGridCtrl* pThis) {
    (void)pThis;
}

// Symbol: ?OnCancelMode@CMFCPropertyGridCtrl@@IEAAXXZ
// Transcribed from retail entry RVA 0xc9a80 (mfc140u):
//     if (m_bTracking) {                                                   // +0x5f0
//         TrackHeader(-1); m_bTracking = FALSE;
//         if (::GetCapture() == m_hWnd) ::ReleaseCapture();
//     }
//     if (m_bTrackingDescr) {                                              // +0x5f4
//         TrackDescr(INT_MIN /* 0x80000000 */); m_bTrackingDescr = FALSE;
//         if (::GetCapture() == m_hWnd) ::ReleaseCapture();
//     }
//     if (::GetCapture() == m_hWnd) ::ReleaseCapture();
//     m_IPToolTip.Deactivate();                                            // +0x348
//     EndEditItem(TRUE);                                                   // vslot 100
//     CWnd::OnCancelMode();  -> tail-jump to ?Default@CWnd@@
// DEVIATION: m_IPToolTip has no OpenMFC member; its Deactivate is skipped.
extern "C" void MS_ABI impl__OnCancelMode_CMFCPropertyGridCtrl__IEAAXXZ(CMFCPropertyGridCtrl* pThis) {
    if (!pThis) return;
    PropertyGridWndState& wnd = WndState(pThis);
    if (wnd.bTracking) {
        impl__TrackHeader_CMFCPropertyGridCtrl__IEAAXH_Z(pThis, -1);
        wnd.bTracking = FALSE;
        if (::GetCapture() == pThis->m_hWnd) ::ReleaseCapture();
    }
    if (wnd.bTrackingDescr) {
        impl__TrackDescr_CMFCPropertyGridCtrl__IEAAXH_Z(pThis, INT_MIN);
        wnd.bTrackingDescr = FALSE;
        if (::GetCapture() == pThis->m_hWnd) ::ReleaseCapture();
    }
    if (::GetCapture() == pThis->m_hWnd) ::ReleaseCapture();
    impl__EndEditItem_CMFCPropertyGridCtrl__UEAAHH_Z(pThis, TRUE);
    impl__Default_CWnd__IEAA_JXZ(pThis);
}

// Symbol: ?OnChar@CMFCPropertyGridCtrl@@IEAAXIII@Z
// Transcribed from retail entry RVA 0xcabf0 (mfc140u):
//     CWnd::OnChar(...);  -> ?Default@CWnd@@
//     if (m_pSel != NULL && m_pSel->m_bEnabled) {                         // +0x678, +0x78
//         if (EditItem(m_pSel, NULL)) m_pSel->PushChar(nChar);             // grid vslot 98, property vslot 23
//     }
// nRepCnt / nFlags are not read by retail.
extern "C" void MS_ABI impl__OnChar_CMFCPropertyGridCtrl__IEAAXIII_Z(
    CMFCPropertyGridCtrl* pThis, unsigned int nChar, unsigned int nRepCnt, unsigned int nFlags) {
    (void)nRepCnt; (void)nFlags;
    if (!pThis) return;
    impl__Default_CWnd__IEAA_JXZ(pThis);
    CMFCPropertyGridProperty* pSel = CurSelOf(pThis);
    if (pSel != nullptr && pSel->IsEnabled()) {
        if (impl__EditItem_CMFCPropertyGridCtrl__UEAAHPEAVCMFCPropertyGridProperty__PEAUtagPOINT___Z(pThis, pSel, nullptr)) {
            impl__PushChar_CMFCPropertyGridProperty__UEAAHI_Z(CurSelOf(pThis), nChar);
        }
    }
}

// Symbol: ?OnClickButton@CMFCPropertyGridCtrl@@UEAAXVCPoint@@@Z
// Transcribed from retail entry RVA 0xc8fd0 (mfc140u):
//     if (m_pSel->OnUpdateValue()) {                              // property vslot 12 (no NULL test in retail)
//         CString strPrev = m_pSel->FormatProperty();             // vslot 11
//         CWaitCursor wait;                                       // AfxGetModuleState()->m_pCurrentWinApp->BeginWaitCursor()
//         m_pSel->OnClickButton(point);                           // vslot 17
//         if (strPrev != m_pSel->FormatProperty()) OnPropertyChanged(m_pSel);   // wcscmp; grid vslot 95
//     }
// A NULL m_pSel guard is added (retail has none).
extern "C" void MS_ABI impl__OnClickButton_CMFCPropertyGridCtrl__UEAAXVCPoint___Z(CMFCPropertyGridCtrl* pThis, CPoint point) {
    if (!pThis) return;
    CMFCPropertyGridProperty* pSel = CurSelOf(pThis);
    if (pSel == nullptr) return;
    if (!pSel->OnUpdateValue()) return;
    ClickButtonAndNotify(pThis, pSel, point);   // strPrev / CWaitCursor / click / compare
}

// Symbol: ?OnCloseCombo@CMFCPropertyGridCtrl@@IEAAXXZ
// Transcribed from retail entry RVA 0xcae50 (mfc140u):
//     if (m_pSel != NULL) m_pSel->OnCloseCombo();     // property vslot 21
extern "C" void MS_ABI impl__OnCloseCombo_CMFCPropertyGridCtrl__IEAAXXZ(CMFCPropertyGridCtrl* pThis) {
    if (!pThis) return;
    if (CMFCPropertyGridProperty* pSel = CurSelOf(pThis)) {
        pSel->OnCloseCombo();
    }
}

// Symbol: ?OnComboKillFocus@CMFCPropertyGridCtrl@@IEAAXXZ
// Retail entry RVA 0xcaf50 (mfc140u):
//     if (m_pSel == NULL || m_pSel->m_pWndCombo == NULL || !m_pSel->m_bEnabled) return;   // +0x678, +0x108, +0x78
//     CWnd* pFocus = CWnd::FromHandle(::GetFocus());
//     if (::IsChild(m_hWnd, pFocus ? pFocus->m_hWnd : NULL)) return;
//     if (!EndEditItem(TRUE)) m_pSel->m_pWndCombo->SetFocus();               // grid vslot 100
//     else OnKillFocus(CWnd::FromHandle(::GetFocus()));
// STUB: OpenMFC's property has no m_pWndCombo and never creates a combo (the
// sibling's CreateCombo is a stub), so retail's first test always fails here;
// nothing past it can be exercised honestly.
extern "C" void MS_ABI impl__OnComboKillFocus_CMFCPropertyGridCtrl__IEAAXXZ(CMFCPropertyGridCtrl* pThis) {
    (void)pThis;
}

// Symbol: ?OnCreate@CMFCPropertyGridCtrl@@IEAAHPEAUtagCREATESTRUCTW@@@Z
// Transcribed from retail entry RVA 0xc6350 (mfc140u):
//     if (CWnd::OnCreate(lpCreateStruct) == -1) return -1;   // ?Default@CWnd@@, `cmp $-1,%eax`
//     Init();                                                // grid vslot 104
//     return 0;
// lpCreateStruct is never read by retail.
extern "C" int MS_ABI impl__OnCreate_CMFCPropertyGridCtrl__IEAAHPEAUtagCREATESTRUCTW___Z(
    CMFCPropertyGridCtrl* pThis, CREATESTRUCTW* lpCreateStruct) {
    (void)lpCreateStruct;
    if (!pThis) return -1;
    if (static_cast<int>(impl__Default_CWnd__IEAA_JXZ(pThis)) == -1) return -1;
    impl__Init_CMFCPropertyGridCtrl__MEAAXXZ(pThis);
    return 0;
}

// Symbol: ?OnCtlColor@CMFCPropertyGridCtrl@@IEAAPEAUHBRUSH__@@PEAVCDC@@PEAVCWnd@@I@Z
// Transcribed from retail entry RVA 0xcac60 (mfc140u):
//     LRESULT lResult;
//     HBRUSH hbr = pWnd->SendChildNotifyLastMsg(&lResult) ? (HBRUSH)lResult : (HBRUSH)Default();
//     if (m_pSel != NULL) {                                                    // +0x678
//         HWND hWnd     = pWnd != NULL ? pWnd->m_hWnd : NULL;
//         HWND hInPlace = m_pSel->m_pWndInPlace != NULL ? m_pSel->m_pWndInPlace->m_hWnd : NULL;   // +0x100
//         if (hWnd == hInPlace) {
//             HBRUSH hbrProp = m_pSel->OnCtlColor(pDC, nCtlColor);           // property vslot 34
//             if (hbrProp != NULL) hbr = hbrProp;
//         }
//     }
//     return hbr;
// Retail calls SendChildNotifyLastMsg through pWnd without a NULL test; a guard
// is added.  DEVIATION: OpenMFC never creates an in-place edit window, so
// hInPlace is always NULL here and the property's OnCtlColor is consulted only
// when pWnd is NULL (the same comparison retail makes with m_pWndInPlace == NULL).
extern "C" HBRUSH MS_ABI impl__OnCtlColor_CMFCPropertyGridCtrl__IEAAPEAUHBRUSH____PEAVCDC__PEAVCWnd__I_Z(
    CMFCPropertyGridCtrl* pThis, CDC* pDC, CWnd* pWnd, unsigned int nCtlColor) {
    if (!pThis) return nullptr;
    LONGLONG lResult = 0;
    HBRUSH hbr = nullptr;
    if (pWnd != nullptr && impl__SendChildNotifyLastMsg_CWnd__QEAAHPEA_J_Z(pWnd, &lResult)) {
        hbr = reinterpret_cast<HBRUSH>(lResult);
    } else {
        hbr = reinterpret_cast<HBRUSH>(impl__Default_CWnd__IEAA_JXZ(pThis));
    }
    CMFCPropertyGridProperty* pSel = CurSelOf(pThis);
    if (pSel != nullptr) {
        const HWND hWnd = pWnd != nullptr ? pWnd->m_hWnd : nullptr;
        const HWND hInPlace = nullptr;   // no m_pWndInPlace in OpenMFC
        if (hWnd == hInPlace) {
            HBRUSH hbrProp = impl__OnCtlColor_CMFCPropertyGridProperty__MEAAPEAUHBRUSH____PEAVCDC__I_Z(pSel, pDC, nCtlColor);
            if (hbrProp != nullptr) hbr = hbrProp;
        }
    }
    return hbr;
}

// Symbol: ?OnDestroy@CMFCPropertyGridCtrl@@IEAAXXZ
// Transcribed from retail entry RVA 0xcb210 (mfc140u):
//     while (!m_lstProps.IsEmpty()) {                                 // +0x620 m_nCount
//         CMFCPropertyGridProperty* pProp = m_lstProps.RemoveHead();  // CList::RemoveHead helper 0xcc2a4 (mfc140u)
//         if (pProp != NULL) delete pProp;                            // vslot 1, flag 1
//     }
//     m_pSel = NULL;                                                  // +0x678
//     m_IPToolTip.DestroyWindow();                                    // +0x348, CWnd vslot 26
//     m_ToolTip.DestroyWindow();                                      // +0x0e8
//     CWnd::OnDestroy();  -> tail-jump to ?OnDestroy@CWnd@@
// The property list lives in PropertyGridCtrlState; each property is detached
// from the side tables (SetPropertyGridOwnerRecursive(NULL), as RemoveAll above
// does) before delete.  DEVIATION: neither tooltip has an OpenMFC member; their
// DestroyWindow calls are skipped.
extern "C" void MS_ABI impl__OnDestroy_CMFCPropertyGridCtrl__IEAAXXZ(CMFCPropertyGridCtrl* pThis) {
    if (!pThis) return;
    if (PropertyGridCtrlState* state = FindMutablePropertyGridCtrlState(pThis)) {
        while (!state->properties.empty()) {
            CMFCPropertyGridProperty* pProp = state->properties.front();
            state->properties.erase(state->properties.begin());
            if (pProp != nullptr) {
                SetPropertyGridOwnerRecursive(pProp, nullptr);
                delete pProp;
            }
        }
        state->current = nullptr;
    }
    impl__OnDestroy_CWnd__IEAAXXZ(pThis);
}

// Symbol: ?OnDrawBorder@CMFCPropertyGridCtrl@@MEAAXPEAVCDC@@@Z
// Retail: mfc140u ordinal 9291 resolves through the export address table to RVA
// 0x27d0, the shared `ret` body (ICF-folded, which is why the RVA symbol map
// lists it under another name); grid vftable slot 107 holds the same address.
// The retail method is empty, so this body is the complete transcription.
extern "C" void MS_ABI impl__OnDrawBorder_CMFCPropertyGridCtrl__MEAAXPEAVCDC___Z(CMFCPropertyGridCtrl* pThis, CDC* pDC) {
    (void)pThis; (void)pDC;
}

// Symbol: ?OnDrawDescription@CMFCPropertyGridCtrl@@MEAAXPEAVCDC@@VCRect@@@Z
// Transcribed from retail entry RVA 0xc7250 (mfc140u):
//     if (m_clrDescriptionBackground != (COLORREF)-1) {                       // +0x694
//         CBrush br(m_clrDescriptionBackground); ::FillRect(pDC->m_hDC, &rect, br);
//     } else {
//         ::FillRect(pDC->m_hDC, &rect, m_bControlBarColors ? afxGlobalData.brBarFace : afxGlobalData.brBtnFace);  // +0x5fc; +0x120 / +0x0b0
//     }
//     rect.top += 4;
//     COLORREF clrLine = m_bControlBarColors ? afxGlobalData.clrBarShadow : afxGlobalData.clrBtnShadow;   // +0x64 / +0x2c
//     pDC->Draw3dRect(&rect, clrLine, clrLine);
//     if (m_pSel != NULL) {                                                   // +0x678
//         ::InflateRect(&rect, -4, -4);
//         COLORREF clrTextOld = (COLORREF)-1;
//         if (m_clrDescriptionText != (COLORREF)-1) clrTextOld = pDC->SetTextColor(m_clrDescriptionText);   // +0x698; CDC vslot 14
//         m_pSel->OnDrawDescription(pDC, rect);                                // property vslot 10
//         if (clrTextOld == (COLORREF)-1) pDC->SetTextColor((COLORREF)-1);    // sic: `cmp $-1,%ebp; jne end; SetTextColor(-1)`
//     }
// The last statement is what retail does (the restore condition is inverted, so
// a custom text colour is never restored and SetTextColor(CLR_INVALID) is a
// no-op); it is transcribed as-is.  m_clrDescriptionBackground / Text are
// state->customColors[4] / [5] (the +0x684.. array in SetCustomColors order);
// note that side table defaults them to real colours rather than retail's -1,
// so the afxGlobalData branch is only reached after SetCustomColors(..., -1, ...).
// DEVIATION: OpenMFC's afxGlobalData blob is zero-filled, so when a brush /
// colour slot is empty the system value UpdateSysColors would have put there is
// used (GetSysColorBrush(COLOR_BTNFACE), GetSysColor(COLOR_BTNSHADOW)) instead
// of a NULL brush / black line.
extern "C" void MS_ABI impl__OnDrawDescription_CMFCPropertyGridCtrl__MEAAXPEAVCDC__VCRect___Z(
    CMFCPropertyGridCtrl* pThis, CDC* pDC, CRect rect) {
    if (!pThis || !pDC || !pDC->GetSafeHdc()) return;
    const HDC hdc = pDC->GetSafeHdc();
    const PropertyGridCtrlState* state = FindPropertyGridCtrlState(pThis);
    const COLORREF clrDescrBack = state ? state->customColors[4] : static_cast<COLORREF>(-1);
    const COLORREF clrDescrText = state ? state->customColors[5] : static_cast<COLORREF>(-1);
    const BOOL bControlBarColors = WndState(pThis).bControlBarColors;

    RECT rc = NativeRect(rect);
    if (clrDescrBack != static_cast<COLORREF>(-1)) {
        HBRUSH hbr = ::CreateSolidBrush(clrDescrBack);
        ::FillRect(hdc, &rc, hbr);
        ::DeleteObject(hbr);
    } else {
        EnsureGlobalDataInitialized();
        HBRUSH hbr = static_cast<HBRUSH>(GlobalDataPtr(bControlBarColors ? kGlobalDataBrBarFace : kGlobalDataBrBtnFace));
        if (hbr == nullptr) hbr = ::GetSysColorBrush(COLOR_BTNFACE);
        ::FillRect(hdc, &rc, hbr);
    }

    rect.top += 4;
    EnsureGlobalDataInitialized();
    COLORREF clrLine = static_cast<COLORREF>(GlobalDataInt(bControlBarColors ? kGlobalDataClrBarShadow : kGlobalDataClrBtnShadow));
    if (clrLine == 0) clrLine = ::GetSysColor(COLOR_BTNSHADOW);   // slot never filled (see DEVIATION above)
    Draw3dRectHdc(hdc, rect, clrLine, clrLine);

    CMFCPropertyGridProperty* pSel = CurSelOf(pThis);
    if (pSel != nullptr) {
        rect.InflateRect(-4, -4);
        COLORREF clrTextOld = static_cast<COLORREF>(-1);
        if (clrDescrText != static_cast<COLORREF>(-1)) {
            clrTextOld = ::SetTextColor(hdc, clrDescrText);
        }
        impl__OnDrawDescription_CMFCPropertyGridProperty__UEAAXPEAVCDC__VCRect___Z(pSel, pDC, rect);
        if (clrTextOld == static_cast<COLORREF>(-1)) {
            ::SetTextColor(hdc, static_cast<COLORREF>(-1));
        }
    }
}

// Symbol: ?OnEditKillFocus@CMFCPropertyGridCtrl@@IEAAXXZ
// Retail entry RVA 0xcae80 (mfc140u):
//     if (m_pSel == NULL || !m_pSel->m_bInPlaceEdit || !m_pSel->m_bEnabled) return;   // +0x678, +0x6c, +0x78
//     CWnd* pFocus = CWnd::FromHandle(::GetFocus());
//     if (::IsChild(m_hWnd, pFocus ? pFocus->m_hWnd : NULL)) return;
//     if (!m_pSel->OnKillFocus(pFocus)) return;                                // property vslot 37 (base: TRUE)
//     if (!EndEditItem(TRUE)) m_pSel->m_pWndInPlace->SetFocus();               // grid vslot 100; +0x100
//     else OnKillFocus(CWnd::FromHandle(::GetFocus()));
// STUB: OpenMFC's property has no m_bInPlaceEdit / m_pWndInPlace and never
// enters in-place editing, so retail's first test always fails here; the
// SetFocus branch would dereference a member OpenMFC does not have.
extern "C" void MS_ABI impl__OnEditKillFocus_CMFCPropertyGridCtrl__IEAAXXZ(CMFCPropertyGridCtrl* pThis) {
    (void)pThis;
}

// Symbol: ?OnEraseBkgnd@CMFCPropertyGridCtrl@@IEAAHPEAVCDC@@@Z
// Retail: mfc140u ordinal 9767 resolves through the export address table to RVA
// 0x3a60, the shared `mov eax,1; ret` body (ICF-folded).  The handler returns
// TRUE without touching the DC: the background is painted by OnDraw.
extern "C" int MS_ABI impl__OnEraseBkgnd_CMFCPropertyGridCtrl__IEAAHPEAVCDC___Z(CMFCPropertyGridCtrl* pThis, CDC* pDC) {
    (void)pThis; (void)pDC;
    return TRUE;
}

// Symbol: ?OnGetDlgCode@CMFCPropertyGridCtrl@@IEAAIXZ
// Retail entry RVA 0xc9c50 (mfc140u): `mov $0x81,%eax; ret` ==
// DLGC_WANTARROWS | DLGC_WANTCHARS.
extern "C" unsigned int MS_ABI impl__OnGetDlgCode_CMFCPropertyGridCtrl__IEAAIXZ(CMFCPropertyGridCtrl* pThis) {
    (void)pThis;
    return DLGC_WANTARROWS | DLGC_WANTCHARS;
}

// Symbol: ?OnGetFont@CMFCPropertyGridCtrl@@IEAAPEAUHFONT__@@XZ
// Transcribed from retail entry RVA 0xc6c00 (mfc140u):
//     return m_hFont != NULL ? m_hFont : (HFONT)::GetStockObject(DEFAULT_GUI_FONT);   // +0x578, 0x11
extern "C" HFONT MS_ABI impl__OnGetFont_CMFCPropertyGridCtrl__IEAAPEAUHFONT____XZ(CMFCPropertyGridCtrl* pThis) {
    if (!pThis) return static_cast<HFONT>(::GetStockObject(DEFAULT_GUI_FONT));
    const HFONT hFont = WndState(pThis).hFont;
    return hFont != nullptr ? hFont : static_cast<HFONT>(::GetStockObject(DEFAULT_GUI_FONT));
}

// Symbol: ?OnGetObject@CMFCPropertyGridCtrl@@IEAA_J_K_J@Z
// Transcribed from retail entry RVA 0xcb980 (mfc140u):
//     <afxGlobalData init gate>
//     if (afxGlobalData.<+0x278> != 0) return CWnd::OnGetObject(wParam, lParam);   // ?OnGetObject@CWnd@@
//     return 0;
extern "C" __int64 MS_ABI impl__OnGetObject_CMFCPropertyGridCtrl__IEAA_J_K_J_Z(
    CMFCPropertyGridCtrl* pThis, unsigned __int64 wParam, __int64 lParam) {
    if (!pThis) return 0;
    EnsureGlobalDataInitialized();
    if (GlobalDataInt(kGlobalDataAccessibility) != 0) {
        return impl__OnGetObject_CWnd__IEAA_J_K_J_Z(pThis, wParam, lParam);
    }
    return 0;
}

// Symbol: ?OnHeaderEndTrack@CMFCPropertyGridCtrl@@IEAAXPEAUtagNMHDR@@PEA_J@Z
// Transcribed from retail entry RVA 0xc7d30 (mfc140u):
//     TrackHeader(-1);
//     *pResult = 0;
// pNMHDR is not read by retail.
extern "C" void MS_ABI impl__OnHeaderEndTrack_CMFCPropertyGridCtrl__IEAAXPEAUtagNMHDR__PEA_J_Z(
    CMFCPropertyGridCtrl* pThis, NMHDR* pNMHDR, LRESULT* pResult) {
    (void)pNMHDR;
    if (pThis) impl__TrackHeader_CMFCPropertyGridCtrl__IEAAXH_Z(pThis, -1);
    if (pResult) *pResult = 0;
}

// Symbol: ?OnHeaderItemChanged@CMFCPropertyGridCtrl@@IEAAXPEAUtagNMHDR@@PEA_J@Z
// Transcribed from retail entry RVA 0xc7a30 (mfc140u):
//     ENSURE(pNMHDR != NULL);
//     if (((NMHEADER*)pNMHDR)->iItem == 0) {                                   // +0x18
//         HDITEM hdItem; hdItem.mask = HDI_WIDTH;
//         GetHeaderCtrl().GetItem(0, &hdItem);                                 // vslot 91; HDM_GETITEM
//         CRect rectClient; ::GetClientRect(m_hWnd, &rectClient);
//         m_nLeftColumnWidth = min(max(m_nRowHeight, hdItem.cxy - 2),          // +0x5c0, +0x5bc
//                                  rectClient.Width() - ::GetSystemMetrics(SM_CXHSCROLL) - 5);   // 0x15
//         ReposProperties();
//         ::InvalidateRect(m_hWnd, &m_rectList, TRUE);                         // +0x590
//         ::UpdateWindow(m_hWnd);
//     }
//     *pResult = 0;
// DEVIATION: OpenMFC has no m_wndHeader, so HDM_GETITEM is sent to the header
// that raised the notification (pNMHDR->hwndFrom), which is that member in
// retail; the ENSURE is an early return, and m_rectList is the list rect above.
extern "C" void MS_ABI impl__OnHeaderItemChanged_CMFCPropertyGridCtrl__IEAAXPEAUtagNMHDR__PEA_J_Z(
    CMFCPropertyGridCtrl* pThis, NMHDR* pNMHDR, LRESULT* pResult) {
    if (!pThis || pNMHDR == nullptr) return;
    const NMHEADERW* pNMHeader = reinterpret_cast<const NMHEADERW*>(pNMHDR);
    if (pNMHeader->iItem == 0) {
        HDITEMW hdItem;
        std::memset(&hdItem, 0, sizeof(hdItem));
        hdItem.mask = HDI_WIDTH;
        if (pNMHDR->hwndFrom != nullptr) {
            ::SendMessageW(pNMHDR->hwndFrom, HDM_GETITEMW, 0, reinterpret_cast<LPARAM>(&hdItem));
        }
        RECT rectClient = {0, 0, 0, 0};
        ::GetClientRect(pThis->m_hWnd, &rectClient);
        PropertyGridWndState& wnd = WndState(pThis);
        const int nMaxWidth = static_cast<int>(rectClient.right - rectClient.left) - ::GetSystemMetrics(SM_CXHSCROLL) - 5;
        wnd.nLeftColumnWidth = std::min(std::max(wnd.nRowHeight, hdItem.cxy - 2), nMaxWidth);
        impl__ReposProperties_CMFCPropertyGridCtrl__IEAAXXZ(pThis);
        const RECT rcList = NativeRect(ListRectOf(pThis));
        ::InvalidateRect(pThis->m_hWnd, &rcList, TRUE);
        ::UpdateWindow(pThis->m_hWnd);
    }
    if (pResult) *pResult = 0;
}

// Symbol: ?OnHeaderTrack@CMFCPropertyGridCtrl@@IEAAXPEAUtagNMHDR@@PEA_J@Z
// Transcribed from retail entry RVA 0xc7cd0 (mfc140u):
//     ENSURE(pNMHDR != NULL);
//     HDITEM* pItem = ((NMHEADER*)pNMHDR)->pitem;                       // +0x20
//     pItem->cxy = min(pItem->cxy, m_rectList.Width());                 // +0x590 / +0x598
//     TrackHeader(pItem->cxy);
//     *pResult = 0;
// The ENSURE is an early return; a NULL pitem guard is added (retail has none).
extern "C" void MS_ABI impl__OnHeaderTrack_CMFCPropertyGridCtrl__IEAAXPEAUtagNMHDR__PEA_J_Z(
    CMFCPropertyGridCtrl* pThis, NMHDR* pNMHDR, LRESULT* pResult) {
    if (!pThis || pNMHDR == nullptr) return;
    HDITEMW* pItem = reinterpret_cast<NMHEADERW*>(pNMHDR)->pitem;
    if (pItem != nullptr) {
        pItem->cxy = std::min(pItem->cxy, ListRectOf(pThis).Width());
        impl__TrackHeader_CMFCPropertyGridCtrl__IEAAXH_Z(pThis, pItem->cxy);
    }
    if (pResult) *pResult = 0;
}

// Symbol: ?OnInitControl@CMFCPropertyGridCtrl@@IEAA_J_K_J@Z
// Transcribed from retail entry RVA 0xcc070 (mfc140u).  wParam is the byte
// count and lParam the UTF-8 initialisation string (the dialog-editor control
// properties); every key is a literal in the image:
//     CString strDst; CMFCControlContainer::UTF8ToString((LPCSTR)lParam, strDst, (int)wParam);
//     CTagManager tagManager(strDst);
//     BOOL bDescriptionArea = TRUE;
//     if (ReadBoolProp(tagManager, L"MFCPropertyGrid_DescriptionArea", bDescriptionArea)) EnableDescriptionArea(bDescriptionArea);
//     CString strDescriptionRows;
//     if (tagManager.ExcludeTag(L"MFCPropertyGrid_DescriptionRows", strDescriptionRows) && !strDescriptionRows.IsEmpty()) {
//         int nRows = _wtoi(strDescriptionRows); if (nRows >= 0) SetDescriptionRows(nRows);
//     }
//     BOOL bHeaderCtrl = TRUE;
//     if (ReadBoolProp(tagManager, L"MFCPropertyGrid_HeaderCtrl", bHeaderCtrl)) EnableHeaderCtrl(bHeaderCtrl, L"Property", L"Value");
//     BOOL bAlphabeticMode = FALSE;
//     if (ReadBoolProp(tagManager, L"MFCPropertyGrid_AlphabeticMode", bAlphabeticMode)) SetAlphabeticMode(bAlphabeticMode);
//     BOOL bModifiedProperties = TRUE;
//     if (ReadBoolProp(tagManager, L"MFCPropertyGrid_ModifiedProperties", bModifiedProperties)) {
//         m_bMarkModifiedProperties = bModifiedProperties;                   // +0x55c
//         if (m_hWnd != NULL) RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE);
//     }
//     BOOL bVSDotNetLook = TRUE;
//     if (ReadBoolProp(tagManager, L"MFCPropertyGrid_VSDotNetLook", bVSDotNetLook)) SetVSDotNetLook(bVSDotNetLook);
//     return 0;
// CTagManager exists here only as its impl__ thunks over a side table keyed by
// the object address, so it is built in raw storage and torn down through the
// matching thunk (neither dispatches a virtual).
extern "C" __int64 MS_ABI impl__OnInitControl_CMFCPropertyGridCtrl__IEAA_J_K_J_Z(
    CMFCPropertyGridCtrl* pThis, unsigned __int64 wParam, __int64 lParam) {
    if (!pThis) return 0;
    CString strDst;
    impl__UTF8ToString_CMFCControlContainer__SAHPEBDAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__H_Z(
        reinterpret_cast<const char*>(lParam), &strDst, static_cast<int>(wParam));

    alignas(void*) unsigned char tagStorage[sizeof(CTagManager)] = {};
    void* pTagManager = tagStorage;
    impl___0CTagManager__QEAA_PEB_W_Z(pTagManager, static_cast<const wchar_t*>(strDst));

    int bDescriptionArea = TRUE;
    if (impl__ReadBoolProp__YAHAEAVCTagManager__PEB_WAEAH_Z(pTagManager, L"MFCPropertyGrid_DescriptionArea", &bDescriptionArea)) {
        impl__EnableDescriptionArea_CMFCPropertyGridCtrl__QEAAXH_Z(pThis, bDescriptionArea);
    }

    CString strDescriptionRows;
    if (impl__ExcludeTag_CTagManager__QEAAHPEB_WAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__H_Z(
            pTagManager, L"MFCPropertyGrid_DescriptionRows", &strDescriptionRows, TRUE) &&
        !strDescriptionRows.IsEmpty()) {
        const int nRows = static_cast<int>(::wcstol(static_cast<const wchar_t*>(strDescriptionRows), nullptr, 10));
        if (nRows >= 0) pThis->SetDescriptionRows(nRows);
    }

    int bHeaderCtrl = TRUE;
    if (impl__ReadBoolProp__YAHAEAVCTagManager__PEB_WAEAH_Z(pTagManager, L"MFCPropertyGrid_HeaderCtrl", &bHeaderCtrl)) {
        impl__EnableHeaderCtrl_CMFCPropertyGridCtrl__QEAAXHPEB_W0_Z(pThis, bHeaderCtrl, L"Property", L"Value");
    }

    int bAlphabeticMode = FALSE;
    if (impl__ReadBoolProp__YAHAEAVCTagManager__PEB_WAEAH_Z(pTagManager, L"MFCPropertyGrid_AlphabeticMode", &bAlphabeticMode)) {
        pThis->SetAlphabeticMode(bAlphabeticMode);
    }

    int bModifiedProperties = TRUE;
    if (impl__ReadBoolProp__YAHAEAVCTagManager__PEB_WAEAH_Z(pTagManager, L"MFCPropertyGrid_ModifiedProperties", &bModifiedProperties)) {
        WndState(pThis).bMarkModifiedProperties = bModifiedProperties ? TRUE : FALSE;
        if (pThis->m_hWnd != nullptr) {
            pThis->RedrawWindow(nullptr, nullptr, RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE);
        }
    }

    int bVSDotNetLook = TRUE;
    if (impl__ReadBoolProp__YAHAEAVCTagManager__PEB_WAEAH_Z(pTagManager, L"MFCPropertyGrid_VSDotNetLook", &bVSDotNetLook)) {
        impl__SetVSDotNetLook_CMFCPropertyGridCtrl__QEAAXH_Z(pThis, bVSDotNetLook);
    }

    impl___1CTagManager__UEAA_XZ(pTagManager);
    return 0;
}

// Symbol: ?OnKeyDown@CMFCPropertyGridCtrl@@IEAAXIII@Z
// Transcribed from retail entry RVA 0xca3c0 (mfc140u); nRepCnt / nFlags are not read.
//     if (m_lstProps.IsEmpty()) { CWnd::OnKeyDown(); return; }                // +0x620; ?Default@CWnd@@
//     switch (nChar) {
//     case VK_LEFT:
//         if (::GetAsyncKeyState(VK_CONTROL) & 0x8000) {
//             BOOL bSave = <+0x604>; <+0x604> = TRUE; TrackHeader(m_nLeftColumnWidth - 5); <+0x604> = bSave; return;
//         }
//         if (m_pSel != NULL && m_pSel->m_bGroup && m_pSel->m_bExpanded) { m_pSel->Expand(FALSE); return; }   // +0x70, +0x74
//         /* fall through to VK_UP */
//     case VK_UP:
//         if (m_pSel == NULL) goto SelectFirst;
//         pProp = HitTest(CPoint(m_pSel->m_Rect.right - 1, m_pSel->m_Rect.top - 2), NULL, TRUE); goto SelectHit;
//     case VK_RIGHT:
//         if (Ctrl) { <+0x604> forced as above around TrackHeader(m_nLeftColumnWidth + 5); return; }
//         if (m_pSel != NULL && m_pSel->m_bGroup && !m_pSel->m_bExpanded) { m_pSel->Expand(TRUE); return; }
//         /* fall through to VK_DOWN */
//     case VK_DOWN:
//         if (m_pSel == NULL) goto SelectFirst;
//         if ((::GetAsyncKeyState(VK_MENU) & 0x8000) && nChar == VK_DOWN) {      // Alt+Down
//             CString strPrev = m_pSel->FormatProperty(); CWaitCursor wait;
//             m_pSel->OnClickButton(CPoint(-1, -1));
//             if (strPrev != m_pSel->FormatProperty()) OnPropertyChanged(m_pSel);
//             return;
//         }
//         pProp = HitTest(CPoint(m_pSel->m_Rect.right - 1, m_pSel->m_Rect.bottom + 2), NULL, TRUE);
//     SelectHit:
//         if (pProp != NULL) { SetCurSel(pProp, TRUE); EnsureVisible(pProp, FALSE); }
//         return;
//     case VK_PRIOR:
//         if (m_pSel == NULL || m_nVertScrollPage == 0) goto Home;                 // +0x5cc
//         EnsureVisible(m_pSel, FALSE);
//         pProp = HitTest(CPoint(m_pSel->m_Rect.right - 1, m_pSel->m_Rect.top - m_nRowHeight * m_nVertScrollPage), NULL, TRUE);
//         if (pProp == NULL) goto Home;
//         SetCurSel(pProp, TRUE); OnVScroll(SB_PAGEUP, 0, NULL); return;
//     case VK_NEXT:
//         if (m_pSel == NULL) goto SelectFirstPage;
//         if (m_nVertScrollPage == 0) goto End;
//         EnsureVisible(m_pSel, FALSE);
//         pProp = HitTest(CPoint(m_pSel->m_Rect.right - 1, m_pSel->m_Rect.top + m_nRowHeight * m_nVertScrollPage), NULL, TRUE);
//         if (pProp == NULL) goto End;
//         SetCurSel(pProp, TRUE); OnVScroll(SB_PAGEDOWN, 0, NULL); return;
//     case VK_HOME: Home:
//     SelectFirst / SelectFirstPage:
//         pProp = (m_bAlphabeticMode ? m_lstTerminalProps : m_lstProps).GetHead();  // +0x554; +0x648 / +0x610 (ENSURE non-empty)
//         SetCurSel(pProp, TRUE); OnVScroll(SB_TOP, 0, NULL); return;
//     case VK_END: End:
//         pProp = (m_bAlphabeticMode ? m_lstTerminalProps : m_lstProps).GetTail();  // +0x650 / +0x618
//         while (pProp->m_bExpanded && !pProp->m_lstSubItems.IsEmpty()) pProp = pProp->m_lstSubItems.GetTail();   // +0x140, +0x138
//         SetCurSel(pProp, TRUE); OnVScroll(SB_BOTTOM, 0, NULL); return;
//     case VK_ADD:      if (m_pSel && m_pSel->m_bGroup && !m_pSel->m_bExpanded && !m_pSel->m_bInPlaceEdit) m_pSel->Expand(TRUE);  return;   // +0x6c
//     case VK_SUBTRACT: if (m_pSel && m_pSel->m_bGroup &&  m_pSel->m_bExpanded && !m_pSel->m_bInPlaceEdit) m_pSel->Expand(FALSE); return;
//     case VK_F4:
//         if (m_pSel == NULL || !m_pSel->m_bEnabled || !EditItem(m_pSel, NULL)) break;   // -> Default()
//         if (m_pSel->m_dwFlags & AFX_PROP_HAS_BUTTON) { strPrev/OnClickButton(CPoint(-1,-1))/OnPropertyChanged as for Alt+Down }   // +0x40 bit 2
//         return;
//     default: CWnd::OnKeyDown(); return;
//     }
// DEVIATIONS (each replaces a retail member OpenMFC does not keep):
//  * the neighbour above/below is found by index in VisiblePropertyGridItems
//    instead of a HitTest 2px outside m_pSel->m_Rect (no per-property rect);
//  * m_nVertScrollPage is 0 in the side table (nothing computes a scroll layout
//    here), so PgUp / PgDn take retail's page == 0 route to Home / End, and the
//    Home / End walks use state->properties (alphabetic mode keeps no separate
//    terminal list here) and sub-item vectors;
//  * m_bGroup is IsGroupOf (has sub-items), m_bInPlaceEdit is always FALSE, and
//    AFX_PROP_HAS_BUTTON is the sibling's HasButton thunk (which reports an
//    option list, retail's HAS_LIST bit, since the flags word is not modelled);
//  * TrackHeader is still a stub, so the Ctrl+Left / Ctrl+Right arms only run the
//    +0x604 save / restore around it.
extern "C" void MS_ABI impl__OnKeyDown_CMFCPropertyGridCtrl__IEAAXIII_Z(
    CMFCPropertyGridCtrl* pThis, unsigned int nChar, unsigned int nRepCnt, unsigned int nFlags) {
    (void)nRepCnt; (void)nFlags;
    if (!pThis) return;
    const PropertyGridCtrlState* state = FindPropertyGridCtrlState(pThis);
    if (state == nullptr || state->properties.empty()) {
        impl__Default_CWnd__IEAA_JXZ(pThis);
        return;
    }
    PropertyGridWndState& wnd = WndState(pThis);
    CMFCPropertyGridProperty* pSel = state->current;

    auto selectFirst = [&]() {
        CMFCPropertyGridProperty* pFirst = state->properties.front();
        if (pFirst == nullptr) return;
        pThis->SetCurSel(pFirst, TRUE);
        impl__OnVScroll_CMFCPropertyGridCtrl__IEAAXIIPEAVCScrollBar___Z(pThis, SB_TOP, 0, nullptr);
    };
    auto selectLast = [&]() {
        CMFCPropertyGridProperty* pLast = state->properties.back();
        if (pLast == nullptr) return;
        for (;;) {
            const PropertyGridPropertyState* propState = FindPropertyGridPropertyState(pLast);
            if (!pLast->IsExpanded() || propState == nullptr || propState->subItems.empty()) break;
            pLast = propState->subItems.back();
        }
        pThis->SetCurSel(pLast, TRUE);
        impl__OnVScroll_CMFCPropertyGridCtrl__IEAAXIIPEAVCScrollBar___Z(pThis, SB_BOTTOM, 0, nullptr);
    };
    // The visible item `delta` rows away from m_pSel, or NULL: what retail's
    // HitTest 2px above / below m_pSel->m_Rect (or one page away) resolves to.
    auto neighbourAt = [&](int delta) -> CMFCPropertyGridProperty* {
        std::vector<CMFCPropertyGridProperty*> items = VisiblePropertyGridItems(pThis);
        const int index = IndexOfVisible(items, pSel);
        if (index < 0) return nullptr;
        const int target = index + delta;
        if (target < 0 || target >= static_cast<int>(items.size())) return nullptr;
        return items[static_cast<size_t>(target)];
    };
    auto selectNeighbour = [&](int delta) {
        CMFCPropertyGridProperty* pProp = neighbourAt(delta);
        if (pProp == nullptr) return;
        pThis->SetCurSel(pProp, TRUE);
        impl__EnsureVisible_CMFCPropertyGridCtrl__QEAAXPEAVCMFCPropertyGridProperty__H_Z(pThis, pProp, FALSE);
    };
    auto clickButtonAtMinusOne = [&]() {
        ClickButtonAndNotify(pThis, pSel, CPoint(-1, -1));   // strPrev / CWaitCursor / click / compare
    };
    auto trackHeaderWithContext = [&](int nOffset) {
        const BOOL bSave = wnd.bShowDragContext;
        wnd.bShowDragContext = TRUE;
        impl__TrackHeader_CMFCPropertyGridCtrl__IEAAXH_Z(pThis, nOffset);
        wnd.bShowDragContext = bSave;
    };

    switch (nChar) {
    case VK_LEFT:
        if (::GetAsyncKeyState(VK_CONTROL) & 0x8000) {
            trackHeaderWithContext(wnd.nLeftColumnWidth - 5);
            return;
        }
        if (pSel != nullptr && IsGroupOf(pSel) && pSel->IsExpanded()) {
            pSel->Expand(FALSE);
            return;
        }
        /* fall through: retail continues into the VK_UP arm */
    case VK_UP:
        if (pSel == nullptr) { selectFirst(); return; }
        selectNeighbour(-1);
        return;

    case VK_RIGHT:
        if (::GetAsyncKeyState(VK_CONTROL) & 0x8000) {
            trackHeaderWithContext(wnd.nLeftColumnWidth + 5);
            return;
        }
        if (pSel != nullptr && IsGroupOf(pSel) && !pSel->IsExpanded()) {
            pSel->Expand(TRUE);
            return;
        }
        /* fall through: retail continues into the VK_DOWN arm */
    case VK_DOWN:
        if (pSel == nullptr) { selectFirst(); return; }
        if ((::GetAsyncKeyState(VK_MENU) & 0x8000) && nChar == VK_DOWN) {
            clickButtonAtMinusOne();
            return;
        }
        selectNeighbour(1);
        return;

    case VK_PRIOR: {
        if (pSel == nullptr || wnd.nVertScrollPage == 0) { selectFirst(); return; }
        // Not reachable while nVertScrollPage stays 0 (see DEVIATIONS).
        impl__EnsureVisible_CMFCPropertyGridCtrl__QEAAXPEAVCMFCPropertyGridProperty__H_Z(pThis, pSel, FALSE);
        CMFCPropertyGridProperty* pProp = neighbourAt(-wnd.nVertScrollPage);
        if (pProp == nullptr) { selectFirst(); return; }
        pThis->SetCurSel(pProp, TRUE);
        impl__OnVScroll_CMFCPropertyGridCtrl__IEAAXIIPEAVCScrollBar___Z(pThis, SB_PAGEUP, 0, nullptr);
        return;
    }

    case VK_NEXT: {
        if (pSel == nullptr) { selectFirst(); return; }
        if (wnd.nVertScrollPage == 0) { selectLast(); return; }
        // Not reachable while nVertScrollPage stays 0 (see DEVIATIONS).
        impl__EnsureVisible_CMFCPropertyGridCtrl__QEAAXPEAVCMFCPropertyGridProperty__H_Z(pThis, pSel, FALSE);
        CMFCPropertyGridProperty* pProp = neighbourAt(wnd.nVertScrollPage);
        if (pProp == nullptr) { selectLast(); return; }
        pThis->SetCurSel(pProp, TRUE);
        impl__OnVScroll_CMFCPropertyGridCtrl__IEAAXIIPEAVCScrollBar___Z(pThis, SB_PAGEDOWN, 0, nullptr);
        return;
    }

    case VK_HOME:
        selectFirst();
        return;

    case VK_END:
        selectLast();
        return;

    case VK_ADD:
        if (pSel != nullptr && IsGroupOf(pSel) && !pSel->IsExpanded()) {
            pSel->Expand(TRUE);   // m_bInPlaceEdit is never set in OpenMFC
        }
        return;

    case VK_SUBTRACT:
        if (pSel != nullptr && IsGroupOf(pSel) && pSel->IsExpanded()) {
            pSel->Expand(FALSE);
        }
        return;

    case VK_F4:
        if (pSel != nullptr && pSel->IsEnabled() &&
            impl__EditItem_CMFCPropertyGridCtrl__UEAAHPEAVCMFCPropertyGridProperty__PEAUtagPOINT___Z(pThis, pSel, nullptr)) {
            pSel = CurSelOf(pThis);
            if (pSel != nullptr && impl__HasButton_CMFCPropertyGridProperty__MEBAHXZ(pSel)) {
                clickButtonAtMinusOne();
            }
            return;
        }
        break;

    default:
        break;
    }
    impl__Default_CWnd__IEAA_JXZ(pThis);
}

// Symbol: ?OnKillFocus@CMFCPropertyGridCtrl@@IEAAXPEAVCWnd@@@Z
// Transcribed from retail entry RVA 0xc9b90 (mfc140u):
//     if (!::IsChild(m_hWnd, pNewWnd != NULL ? pNewWnd->m_hWnd : NULL)) {
//         if (m_pSel == NULL || m_pSel->OnKillFocus(pNewWnd)) {         // +0x678; property vslot 37 (base body: return TRUE)
//             EndEditItem(TRUE);                                        // grid vslot 100
//             m_bFocused = FALSE;                                       // +0x5f8
//             if (m_pSel != NULL) m_pSel->Redraw();
//         }
//     }
//     CWnd::OnKillFocus(pNewWnd);  -> tail-jump to ?Default@CWnd@@
// The property-side OnKillFocus is the `mov eax,1; ret` base body (entry
// 0x3a60) in retail and is not modelled in OpenMFC, so it is taken as TRUE.
extern "C" void MS_ABI impl__OnKillFocus_CMFCPropertyGridCtrl__IEAAXPEAVCWnd___Z(CMFCPropertyGridCtrl* pThis, CWnd* pNewWnd) {
    if (!pThis) return;
    const HWND hNew = pNewWnd != nullptr ? pNewWnd->m_hWnd : nullptr;
    if (!::IsChild(pThis->m_hWnd, hNew)) {
        impl__EndEditItem_CMFCPropertyGridCtrl__UEAAHH_Z(pThis, TRUE);
        WndState(pThis).bFocused = FALSE;
        if (CMFCPropertyGridProperty* pSel = CurSelOf(pThis)) {
            impl__Redraw_CMFCPropertyGridProperty__QEAAXXZ(pSel);
        }
    }
    impl__Default_CWnd__IEAA_JXZ(pThis);
}

// Symbol: ?OnLButtonDblClk@CMFCPropertyGridCtrl@@IEAAXIVCPoint@@@Z
// Transcribed from retail entry RVA 0xca050 (mfc140u); nFlags is not read.
//     CWnd::OnLButtonDblClk();  -> ?Default@CWnd@@
//     if (point.y <= m_rectList.bottom && abs(point.x - m_nLeftColumnWidth - m_rectList.left) <= 2) {   // +0x59c, +0x5c0, +0x590
//         CRect rectClient; ::GetClientRect(m_hWnd, &rectClient);
//         m_nLeftColumnWidth = rectClient.Width() / 2;
//         HDITEM hdItem; hdItem.mask = HDI_WIDTH; hdItem.cxy = m_nLeftColumnWidth + 2;
//         GetHeaderCtrl().SetItem(0, &hdItem);                       // HDM_SETITEM
//         hdItem.cxy = rectClient.Width() + 10; GetHeaderCtrl().SetItem(1, &hdItem);
//         return;
//     }
//     if (m_pSel == NULL) return;
//     if (m_pSel->m_bGroup && (!m_pSel->m_bIsValueList || point.x < m_nLeftColumnWidth + m_rectList.left)) {   // +0x70, +0x80
//         m_pSel->Expand(!m_pSel->m_bExpanded); return;
//     }
//     if (!m_pSel->m_bEnabled) return;                                // +0x78
//     if (EditItem(m_pSel, NULL) && m_pSel->m_pWndInPlace != NULL) {  // grid vslot 98; +0x100
//         ::SendMessage(m_pSel->m_pWndInPlace->m_hWnd, WM_LBUTTONDOWN, 0, 0);
//         ::SendMessage(m_pSel->m_pWndInPlace->m_hWnd, WM_LBUTTONUP, 0, 0);
//     }
//     if (::PtInRect(&m_pSel->m_Rect, point)) m_pSel->OnDblClk(point);  // +0x44; property vslot 19
// DEVIATIONS: no header control (the two HDM_SETITEM calls are skipped; the
// column width is still stored), m_bGroup is IsGroupOf, m_bIsValueList is not
// modelled (taken as FALSE, so a group toggles on either column), no in-place
// window (the two forwarded button messages are skipped), m_Rect is PropRectOf.
extern "C" void MS_ABI impl__OnLButtonDblClk_CMFCPropertyGridCtrl__IEAAXIVCPoint___Z(
    CMFCPropertyGridCtrl* pThis, unsigned int nFlags, CPoint point) {
    (void)nFlags;
    if (!pThis) return;
    impl__Default_CWnd__IEAA_JXZ(pThis);
    PropertyGridWndState& wnd = WndState(pThis);
    const CRect rectList = ListRectOf(pThis);
    if (point.y <= rectList.bottom && std::abs(point.x - wnd.nLeftColumnWidth - rectList.left) <= 2) {
        RECT rectClient = {0, 0, 0, 0};
        ::GetClientRect(pThis->m_hWnd, &rectClient);
        wnd.nLeftColumnWidth = static_cast<int>(rectClient.right - rectClient.left) / 2;
        return;
    }
    CMFCPropertyGridProperty* pSel = CurSelOf(pThis);
    if (pSel == nullptr) return;
    if (IsGroupOf(pSel)) {
        pSel->Expand(!pSel->IsExpanded());
        return;
    }
    if (!pSel->IsEnabled()) return;
    impl__EditItem_CMFCPropertyGridCtrl__UEAAHPEAVCMFCPropertyGridProperty__PEAUtagPOINT___Z(pThis, pSel, nullptr);
    pSel = CurSelOf(pThis);
    if (pSel != nullptr && PropRectOf(pThis, pSel).PtInRect(point)) {
        impl__OnDblClk_CMFCPropertyGridProperty__UEAAHVCPoint___Z(pSel, point);
    }
}

// Symbol: ?OnLButtonDown@CMFCPropertyGridCtrl@@IEAAXIVCPoint@@@Z
// Transcribed from retail entry RVA 0xc8980 (mfc140u); nFlags is not read.
//     CWnd::OnLButtonDown();  -> ?Default@CWnd@@
//     SetFocus();                                                       // ?SetFocus@CWnd@@
//     CRect rectClient; ::GetClientRect(m_hWnd, &rectClient);           // (computed, then unused)
//     if (m_bDescriptionArea) {                                         // +0x54c
//         if (abs(point.y - m_rectList.bottom - 4) <= 2) {              // +0x59c
//             SetCapture(); TrackDescr(point.y); m_bTrackingDescr = TRUE; return;   // +0x5f4
//         }
//         if (point.y > m_rectList.bottom) return;
//     }
//     if (abs(point.x - m_nLeftColumnWidth - m_rectList.left) <= 2) {   // +0x5c0, +0x590
//         SetCapture(); TrackHeader(point.x); m_bTracking = TRUE; return;         // +0x5f0
//     }
//     ClickArea clickArea;
//     CMFCPropertyGridProperty* pHit = HitTest(point, &clickArea, FALSE);
//     CMFCPropertyGridProperty* pOldSel = m_pSel;
//     SetCurSel(pHit, TRUE);
//     if (pHit == NULL) return;
//     if (m_rectList.Height() > pHit->m_Rect.Height()) EnsureVisible(pHit, FALSE);
//     switch (clickArea) {
//     case ClickExpandBox: pHit->Expand(!pHit->m_bExpanded); break;
//     case ClickName:      pHit->OnClickName(point); break;                    // property vslot 26 (`ret` in retail)
//     case ClickValue:
//         if (!pHit->m_bEnabled) break;
//         if (!EditItem(pHit, &point)) break;                                  // grid vslot 98
//         if (pHit->m_pWndInPlace == NULL) break;                              // +0x100
//         if (::PtInRect(&pHit->m_rectButton, point)) {                        // +0x54
//             CString strPrev = pHit->FormatProperty();
//             if (::GetCapture() == m_hWnd) ::ReleaseCapture();
//             pHit->OnClickButton(point);
//             if (strPrev != pHit->FormatProperty()) OnPropertyChanged(pHit);
//         } else if (pHit == pOldSel || pHit->IsProcessFirstClick()) {         // property vslot 40 (base: TRUE)
//             pHit->OnClickValue(WM_LBUTTONDOWN, point);                       // property vslot 18
//         }
//         break;
//     }
// DEVIATIONS: TrackDescr / TrackHeader are still stubs (the capture and flag
// handling around them is kept); m_Rect is PropRectOf; ClickExpandBox is never
// produced by this file's HitTest (it reports name or value only), and
// OnClickName is the empty retail base, so those two arms are inert here.
// OpenMFC never creates an in-place window, so the ClickValue arm ends at
// retail's `m_pWndInPlace == NULL` test after EditItem -- the button /
// OnClickValue tail cannot be reached in this model.
extern "C" void MS_ABI impl__OnLButtonDown_CMFCPropertyGridCtrl__IEAAXIVCPoint___Z(
    CMFCPropertyGridCtrl* pThis, unsigned int nFlags, CPoint point) {
    (void)nFlags;
    if (!pThis) return;
    impl__Default_CWnd__IEAA_JXZ(pThis);
    impl__SetFocus_CWnd__QEAAPEAV1_XZ(pThis);
    PropertyGridWndState& wnd = WndState(pThis);
    const CRect rectList = ListRectOf(pThis);
    if (DescriptionAreaOf(pThis)) {
        if (std::abs(point.y - rectList.bottom - 4) <= 2) {
            ::SetCapture(pThis->m_hWnd);
            impl__TrackDescr_CMFCPropertyGridCtrl__IEAAXH_Z(pThis, point.y);
            wnd.bTrackingDescr = TRUE;
            return;
        }
        if (point.y > rectList.bottom) return;
    }
    if (std::abs(point.x - wnd.nLeftColumnWidth - rectList.left) <= 2) {
        ::SetCapture(pThis->m_hWnd);
        impl__TrackHeader_CMFCPropertyGridCtrl__IEAAXH_Z(pThis, point.x);
        wnd.bTracking = TRUE;
        return;
    }
    int clickArea = 0;
    CMFCPropertyGridProperty* pHit =
        impl__HitTest_CMFCPropertyGridCtrl__QEBAPEAVCMFCPropertyGridProperty__VCPoint__PEAW4ClickArea_2_H_Z(pThis, point, &clickArea, FALSE);
    pThis->SetCurSel(pHit, TRUE);
    if (pHit == nullptr) return;
    if (rectList.Height() > PropRectOf(pThis, pHit).Height()) {
        impl__EnsureVisible_CMFCPropertyGridCtrl__QEAAXPEAVCMFCPropertyGridProperty__H_Z(pThis, pHit, FALSE);
    }
    POINT ptClick = {point.x, point.y};
    switch (clickArea) {
    case 0:   // ClickExpandBox
        pHit->Expand(!pHit->IsExpanded());
        break;
    case 1:   // ClickName: retail dispatches the empty OnClickName
        break;
    case 2:   // ClickValue
        if (!pHit->IsEnabled()) break;
        if (!impl__EditItem_CMFCPropertyGridCtrl__UEAAHPEAVCMFCPropertyGridProperty__PEAUtagPOINT___Z(pThis, pHit, &ptClick)) break;
        // pHit->m_pWndInPlace == NULL in OpenMFC: retail returns here.
        break;
    default:
        break;
    }
}

// Symbol: ?OnLButtonUp@CMFCPropertyGridCtrl@@IEAAXIVCPoint@@@Z
// Transcribed from retail entry RVA 0xcb490 (mfc140u); nFlags is not read.
//     CWnd::OnLButtonUp();  -> ?Default@CWnd@@
//     if (m_bTracking) {                                                     // +0x5f0
//         TrackHeader(-1); m_bTracking = FALSE;
//         if (::GetCapture() == m_hWnd) ::ReleaseCapture();
//         CRect rectClient; ::GetClientRect(m_hWnd, &rectClient);
//         m_nLeftColumnWidth = min(max(m_nRowHeight, point.x),                 // +0x5c0, +0x5bc
//                                  rectClient.Width() - ::GetSystemMetrics(SM_CXHSCROLL) - 5);
//         HDITEM hdItem; hdItem.mask = HDI_WIDTH; hdItem.cxy = m_nLeftColumnWidth + 2;
//         GetHeaderCtrl().SetItem(0, &hdItem);                                // HDM_SETITEM
//         hdItem.cxy = rectClient.Width() + 10; GetHeaderCtrl().SetItem(1, &hdItem);
//     }
//     if (m_bTrackingDescr) {                                                // +0x5f4
//         TrackDescr(INT_MIN); m_bTrackingDescr = FALSE;
//         if (::GetCapture() == m_hWnd) ::ReleaseCapture();
//         CRect rectClient; ::GetClientRect(m_hWnd, &rectClient);
//         int y = max(point.y, m_nHeaderHeight + m_nRowHeight);              // +0x5b8
//         m_nDescrHeight = max(m_nRowHeight, rectClient.Height() - y + 2);   // +0x5a0
//         AdjustLayout();                                                    // grid vslot 94
//         RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE);
//     }
// DEVIATION: no header control, so the two HDM_SETITEM calls are skipped (the
// width is still stored); m_nHeaderHeight stays 0 in the side table.
extern "C" void MS_ABI impl__OnLButtonUp_CMFCPropertyGridCtrl__IEAAXIVCPoint___Z(
    CMFCPropertyGridCtrl* pThis, unsigned int nFlags, CPoint point) {
    (void)nFlags;
    if (!pThis) return;
    impl__Default_CWnd__IEAA_JXZ(pThis);
    PropertyGridWndState& wnd = WndState(pThis);
    if (wnd.bTracking) {
        impl__TrackHeader_CMFCPropertyGridCtrl__IEAAXH_Z(pThis, -1);
        wnd.bTracking = FALSE;
        if (::GetCapture() == pThis->m_hWnd) ::ReleaseCapture();
        RECT rectClient = {0, 0, 0, 0};
        ::GetClientRect(pThis->m_hWnd, &rectClient);
        const int nMaxWidth = static_cast<int>(rectClient.right - rectClient.left) - ::GetSystemMetrics(SM_CXHSCROLL) - 5;
        wnd.nLeftColumnWidth = std::min(std::max(wnd.nRowHeight, static_cast<int>(point.x)), nMaxWidth);
    }
    if (wnd.bTrackingDescr) {
        impl__TrackDescr_CMFCPropertyGridCtrl__IEAAXH_Z(pThis, INT_MIN);
        wnd.bTrackingDescr = FALSE;
        if (::GetCapture() == pThis->m_hWnd) ::ReleaseCapture();
        RECT rectClient = {0, 0, 0, 0};
        ::GetClientRect(pThis->m_hWnd, &rectClient);
        const int y = std::max(static_cast<int>(point.y), wnd.nHeaderHeight + wnd.nRowHeight);
        wnd.nDescrHeight = std::max(wnd.nRowHeight, static_cast<int>(rectClient.bottom - rectClient.top) - y + 2);
        pThis->AdjustLayout();
        pThis->RedrawWindow(nullptr, nullptr, RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE);
    }
}

// Symbol: ?OnMouseMove@CMFCPropertyGridCtrl@@IEAAXIVCPoint@@@Z
// Transcribed from retail entry RVA 0xcb440 (mfc140u); nFlags is not read.
//     CWnd::OnMouseMove();  -> ?Default@CWnd@@
//     if (m_bTracking) TrackHeader(point.x);            // +0x5f0
//     else if (m_bTrackingDescr) TrackDescr(point.y);   // +0x5f4
extern "C" void MS_ABI impl__OnMouseMove_CMFCPropertyGridCtrl__IEAAXIVCPoint___Z(
    CMFCPropertyGridCtrl* pThis, unsigned int nFlags, CPoint point) {
    (void)nFlags;
    if (!pThis) return;
    impl__Default_CWnd__IEAA_JXZ(pThis);
    const PropertyGridWndState& wnd = WndState(pThis);
    if (wnd.bTracking) {
        impl__TrackHeader_CMFCPropertyGridCtrl__IEAAXH_Z(pThis, point.x);
    } else if (wnd.bTrackingDescr) {
        impl__TrackDescr_CMFCPropertyGridCtrl__IEAAXH_Z(pThis, point.y);
    }
}

// Symbol: ?OnMouseWheel@CMFCPropertyGridCtrl@@IEAAHIFVCPoint@@@Z
// Transcribed from retail entry RVA 0xc9fd0 (mfc140u); nFlags and pt are not read.
//     if (CMFCPopupMenu::m_pActivePopupMenu == NULL && m_nVertScrollTotal > 0) {   // +0x5c8
//         int nSteps = abs(zDelta) / WHEEL_DELTA;                                   // 0x88888889 magic == / 120
//         for (int i = 0; i < nSteps; i++) OnVScroll(zDelta < 0 ? SB_LINEDOWN : SB_LINEUP, 0, NULL);   // `shr $0xf` of the 16-bit delta
//     }
//     return TRUE;
// m_nVertScrollTotal never leaves 0 in the side table (no scroll layout is
// computed here), so the loop is inert until that changes.
extern "C" int MS_ABI impl__OnMouseWheel_CMFCPropertyGridCtrl__IEAAHIFVCPoint___Z(
    CMFCPropertyGridCtrl* pThis, unsigned int nFlags, short zDelta, CPoint pt) {
    (void)nFlags; (void)pt;
    if (!pThis) return TRUE;
    if (impl__m_pActivePopupMenu_CMFCPopupMenu__1PEAV1_EA == nullptr && WndState(pThis).nVertScrollTotal > 0) {
        const int nSteps = std::abs(static_cast<int>(zDelta)) / WHEEL_DELTA;
        for (int i = 0; i < nSteps; ++i) {
            impl__OnVScroll_CMFCPropertyGridCtrl__IEAAXIIPEAVCScrollBar___Z(
                pThis, zDelta < 0 ? SB_LINEDOWN : SB_LINEUP, 0, nullptr);
        }
    }
    return TRUE;
}

// Symbol: ?OnNcCalcSize@CMFCPropertyGridCtrl@@IEAAXHPEAUtagNCCALCSIZE_PARAMS@@@Z
// Transcribed from retail entry RVA 0xcb6b0 (mfc140u); bCalcValidRects is not read.
//     CWnd::OnNcCalcSize();  -> ?Default@CWnd@@
//     if (GetStyle() & WS_BORDER) {                     // `bt $0x17,%eax`
//         lpncsp->rgrc[0].left++; .top++; .right--; .bottom--;
//     }
// GetStyle is read with ::GetWindowLongW(m_hWnd, GWL_STYLE) (CWnd::GetStyle
// exists in this DLL only as a C++ member outside this unit).
extern "C" void MS_ABI impl__OnNcCalcSize_CMFCPropertyGridCtrl__IEAAXHPEAUtagNCCALCSIZE_PARAMS___Z(
    CMFCPropertyGridCtrl* pThis, int bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp) {
    (void)bCalcValidRects;
    if (!pThis) return;
    impl__Default_CWnd__IEAA_JXZ(pThis);
    if (lpncsp == nullptr || pThis->m_hWnd == nullptr) return;
    if (::GetWindowLongW(pThis->m_hWnd, GWL_STYLE) & WS_BORDER) {
        lpncsp->rgrc[0].left++;
        lpncsp->rgrc[0].top++;
        lpncsp->rgrc[0].right--;
        lpncsp->rgrc[0].bottom--;
    }
}

// Symbol: ?OnNcPaint@CMFCPropertyGridCtrl@@IEAAXXZ
// Transcribed from retail entry RVA 0xcb6f0 (mfc140u):
//     if (m_nDescrHeight == -1) {                                          // +0x5a0
//         m_nDescrHeight = m_nRowHeight * m_nDescrRows + m_nRowHeight / 2;  // +0x5bc, +0x5a4
//         AdjustLayout();                                                  // grid vslot 94
//     }
//     if (GetStyle() & WS_BORDER) {
//         CMFCVisualManager::GetInstance()->OnDrawControlBorder(this);     // GetInstance helper 0x9774 (mfc140u); manager vslot 108
//     }
// m_nDescrRows is state->descriptionRows (side-table default 0; retail's ctor
// value is 3).  The manager is read from the exported ?m_pVisManager@ mirror of
// CMFCVisualManager::GetInstance(), as docking/CPaneDivider.cpp does; the draw
// is skipped when no manager exists yet (the C++ static is not linkable here).
extern "C" void MS_ABI impl__OnNcPaint_CMFCPropertyGridCtrl__IEAAXXZ(CMFCPropertyGridCtrl* pThis) {
    if (!pThis) return;
    PropertyGridWndState& wnd = WndState(pThis);
    if (wnd.nDescrHeight == -1) {
        const PropertyGridCtrlState* state = FindPropertyGridCtrlState(pThis);
        const int nDescrRows = state ? state->descriptionRows : 0;
        wnd.nDescrHeight = wnd.nRowHeight * nDescrRows + wnd.nRowHeight / 2;
        pThis->AdjustLayout();
    }
    if (pThis->m_hWnd != nullptr && (::GetWindowLongW(pThis->m_hWnd, GWL_STYLE) & WS_BORDER)) {
        CMFCVisualManager* pManager = static_cast<CMFCVisualManager*>(impl__m_pVisManager_CMFCVisualManager__1PEAV1_EA);
        if (pManager != nullptr) {
            pManager->OnDrawControlBorder(pThis);
        }
    }
}

// Symbol: ?OnNeedTipText@CMFCPropertyGridCtrl@@IEAAHIPEAUtagNMHDR@@PEA_J@Z
// Retail entry RVA 0xcb2b0 (mfc140u):
//     if (m_ToolTip.m_hWnd == NULL || pNMH->hwndFrom != m_ToolTip.m_hWnd) return FALSE;   // +0x0e8 / +0x128
//     CPoint pt; ::GetCursorPos(&pt); ::ScreenToClient(m_hWnd, &pt);
//     CMFCPropertyGridProperty* pProp = HitTest(pt, NULL, FALSE);
//     if (pProp == NULL) return FALSE;
//     if (pt.x < m_rectList.left + m_nLeftColumnWidth) { if (!pProp-><+0x84> || <+0x550>) strTipText = pProp->GetNameTooltip(); }
//     else                                             { if (!pProp-><+0x88> || <+0x550>) strTipText = pProp->GetValueTooltip(); }
//     if (strTipText.IsEmpty()) return FALSE;
//     ((NMTTDISPINFO*)pNMH)->lpszText = strTipText;                          // static CString at 0x3bb1c8 (mfc140)
//     m_ToolTip.SetFont(CFont::FromHandle((HFONT)::SendMessage(m_hWnd, WM_GETFONT, 0, 0)));   // WM_SETFONT to +0x128
//     return TRUE;
// STUB: the first test is against m_ToolTip, the CToolTipCtrl member OpenMFC
// does not have and never creates, so retail's very first early return is the
// whole reachable body here.
extern "C" int MS_ABI impl__OnNeedTipText_CMFCPropertyGridCtrl__IEAAHIPEAUtagNMHDR__PEA_J_Z(
    CMFCPropertyGridCtrl* pThis, unsigned int id, NMHDR* pNMH, LRESULT* pResult) {
    (void)pThis; (void)id; (void)pNMH; (void)pResult;
    return FALSE;
}

// Symbol: ?OnNotify@CMFCPropertyGridCtrl@@MEAAH_K_JPEA_J@Z
// Transcribed from retail entry RVA 0xcb1a0 (mfc140u):
//     BOOL bRes = CWnd::OnNotify(wParam, lParam, pResult);           // ?OnNotify@CWnd@@
//     NMHDR* pNMHDR = (NMHDR*)lParam; ENSURE(pNMHDR != NULL);
//     if (pNMHDR->code == TTN_SHOW)                                  // 0xfffffdf7
//         m_ToolTip.SetWindowPos(&CWnd::wndTop, -1, -1, -1, -1, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);   // +0x0e8; 0x13
//     return bRes;
// DEVIATION: m_ToolTip has no OpenMFC member, so the TTN_SHOW z-order fix-up
// has nothing to act on and is skipped; the ENSURE is an early return.
extern "C" int MS_ABI impl__OnNotify_CMFCPropertyGridCtrl__MEAAH_K_JPEA_J_Z(
    CMFCPropertyGridCtrl* pThis, unsigned __int64 wParam, __int64 lParam, __int64* pResult) {
    if (!pThis) return FALSE;
    const int bRes = impl__OnNotify_CWnd__MEAAH_K_JPEA_J_Z(pThis, static_cast<WPARAM>(wParam), static_cast<LPARAM>(lParam),
                                                          reinterpret_cast<LRESULT*>(pResult));
    const NMHDR* pNMHDR = reinterpret_cast<const NMHDR*>(lParam);
    if (pNMHDR == nullptr) return bRes;
    if (pNMHDR->code == static_cast<UINT>(TTN_SHOW)) {
        // m_ToolTip.SetWindowPos(&wndTop, ...) -- no tooltip member here.
    }
    return bRes;
}

// Symbol: ?OnPaint@CMFCPropertyGridCtrl@@IEAAXXZ
// Transcribed from retail entry RVA 0xc6e70 (mfc140u):
//     CPaintDC dc(this);       // ??0CPaintDC@@
//     OnDraw(&dc);             // grid vslot 106
//     (dc.~CPaintDC())
// CPaintDC exists in this DLL only as its impl__ thunks, so the object is built
// in raw storage and torn down through the matching thunk (as
// docking/CDockablePane.cpp does); neither thunk dispatches a virtual.
extern "C" void MS_ABI impl__OnPaint_CMFCPropertyGridCtrl__IEAAXXZ(CMFCPropertyGridCtrl* pThis) {
    if (!pThis) return;
    alignas(void*) unsigned char dcStorage[sizeof(CPaintDC)] = {};
    CPaintDC* pDC = reinterpret_cast<CPaintDC*>(dcStorage);
    impl___0CPaintDC__QEAA_PEAVCWnd___Z(pDC, pThis);
    impl__OnDraw_CMFCPropertyGridCtrl__MEAAXPEAVCDC___Z(pThis, pDC);
    impl___1CPaintDC__UEAA_XZ(pDC);
}

// Symbol: ?OnPrintClient@CMFCPropertyGridCtrl@@IEAA_JPEAVCDC@@I@Z
// Transcribed from retail entry RVA 0xcc280 (mfc140u):
//     if (dwFlags & PRF_CLIENT) OnDraw(pDC);   // 0x4; grid vslot 106
//     return 0;
extern "C" __int64 MS_ABI impl__OnPrintClient_CMFCPropertyGridCtrl__IEAA_JPEAVCDC__I_Z(
    CMFCPropertyGridCtrl* pThis, CDC* pDC, unsigned int dwFlags) {
    if (!pThis) return 0;
    if (dwFlags & PRF_CLIENT) {
        impl__OnDraw_CMFCPropertyGridCtrl__MEAAXPEAVCDC___Z(pThis, pDC);
    }
    return 0;
}

// Symbol: ?OnPropertyChanged@CMFCPropertyGridCtrl@@UEBAXPEAVCMFCPropertyGridProperty@@@Z
// Transcribed from retail entry RVA 0xc79c0 (mfc140u):
//     pProp->SetModifiedFlag();                                              // ?SetModifiedFlag@CMFCPropertyGridProperty@@ 0xc43e0 (mfc140u)
//     CWnd* pOwner = GetOwner();   // inline: m_hWndOwner (CWnd +0xa0) ? it : ::GetParent(m_hWnd); then ?FromHandle@CWnd@@
//     pOwner->SendMessage(AFX_WM_PROPERTY_CHANGED, GetDlgCtrlID(), (LPARAM)pProp);   // ?AFX_WM_PROPERTY_CHANGED@@3IA
// DEVIATION: OpenMFC's CWnd does not name m_hWndOwner (that range is padding),
// so the ::GetParent branch is taken unconditionally, as toolbar/CMFCToolBar.cpp
// does for the same inline.  GetDlgCtrlID is ::GetDlgCtrlID on m_hWnd.  Retail
// has no NULL tests on pProp or the owner; both are guarded here.
extern "C" void MS_ABI impl__OnPropertyChanged_CMFCPropertyGridCtrl__UEBAXPEAVCMFCPropertyGridProperty___Z(
    const CMFCPropertyGridCtrl* pThis, CMFCPropertyGridProperty* pProp) {
    if (!pThis || !pProp) return;
    impl__SetModifiedFlag_CMFCPropertyGridProperty__IEAAXXZ(pProp);
    if (pThis->m_hWnd == nullptr) return;
    CWnd* pOwner = impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(::GetParent(pThis->m_hWnd));
    if (pOwner == nullptr || pOwner->m_hWnd == nullptr) return;
    ::SendMessageW(pOwner->m_hWnd, impl__AFX_WM_PROPERTY_CHANGED__3IA,
                   static_cast<WPARAM>(::GetDlgCtrlID(pThis->m_hWnd)), reinterpret_cast<LPARAM>(pProp));
}

// Symbol: ?OnRButtonDown@CMFCPropertyGridCtrl@@IEAAXIVCPoint@@@Z
// Transcribed from retail entry RVA 0xc8c90 (mfc140u); nFlags is not read.
//     CWnd::OnRButtonDown();  -> ?Default@CWnd@@
//     SetFocus();
//     CRect rectClient; ::GetClientRect(m_hWnd, &rectClient);            // (computed, then unused)
//     if (m_bDescriptionArea) {                                          // +0x54c
//         if (abs(point.y - m_rectList.bottom - 4) <= 2) return;
//         if (point.y > m_rectList.bottom) return;
//     }
//     if (abs(point.x - m_nLeftColumnWidth - m_rectList.left) <= 2) return;
//     ClickArea clickArea;
//     CMFCPropertyGridProperty* pHit = HitTest(point, &clickArea, FALSE);
//     CMFCPropertyGridProperty* pOldSel = m_pSel;
//     BOOL bSelChanged = (pHit != pOldSel);
//     SetCurSel(pHit, TRUE);
//     if (pHit == NULL) return;
//     EnsureVisible(pHit, FALSE);
//     switch (clickArea) {
//     case ClickName:  pHit->OnRClickName(point); break;                   // property vslot 27 (`ret`)
//     case ClickValue:
//         pHit->OnRClickValue(point, bSelChanged);                         // property vslot 28 (`ret`)
//         if (!pHit->m_bEnabled || pHit != pOldSel) break;
//         if (!EditItem(pHit, &point)) break;                              // grid vslot 98
//         if (pHit->m_pWndInPlace == NULL) break;                          // +0x100
//         if (::PtInRect(&pHit->m_rectButton, point)) break;
//         if (pHit->IsProcessFirstClick()) pHit->OnClickValue(WM_RBUTTONDOWN, point);   // vslots 40, 18
//         break;
//     }
// DEVIATIONS as for OnLButtonDown: OnRClickName / OnRClickValue are the empty
// retail bases, and the ClickValue arm ends at retail's `m_pWndInPlace == NULL`
// test after EditItem.
extern "C" void MS_ABI impl__OnRButtonDown_CMFCPropertyGridCtrl__IEAAXIVCPoint___Z(
    CMFCPropertyGridCtrl* pThis, unsigned int nFlags, CPoint point) {
    (void)nFlags;
    if (!pThis) return;
    impl__Default_CWnd__IEAA_JXZ(pThis);
    impl__SetFocus_CWnd__QEAAPEAV1_XZ(pThis);
    const PropertyGridWndState& wnd = WndState(pThis);
    const CRect rectList = ListRectOf(pThis);
    if (DescriptionAreaOf(pThis)) {
        if (std::abs(point.y - rectList.bottom - 4) <= 2) return;
        if (point.y > rectList.bottom) return;
    }
    if (std::abs(point.x - wnd.nLeftColumnWidth - rectList.left) <= 2) return;
    int clickArea = 0;
    CMFCPropertyGridProperty* pHit =
        impl__HitTest_CMFCPropertyGridCtrl__QEBAPEAVCMFCPropertyGridProperty__VCPoint__PEAW4ClickArea_2_H_Z(pThis, point, &clickArea, FALSE);
    CMFCPropertyGridProperty* pOldSel = CurSelOf(pThis);
    pThis->SetCurSel(pHit, TRUE);
    if (pHit == nullptr) return;
    impl__EnsureVisible_CMFCPropertyGridCtrl__QEAAXPEAVCMFCPropertyGridProperty__H_Z(pThis, pHit, FALSE);
    switch (clickArea) {
    case 1:   // ClickName: retail dispatches the empty OnRClickName
        break;
    case 2: { // ClickValue: retail dispatches the empty OnRClickValue first
        if (!pHit->IsEnabled() || pHit != pOldSel) break;
        POINT ptClick = {point.x, point.y};
        if (!impl__EditItem_CMFCPropertyGridCtrl__UEAAHPEAVCMFCPropertyGridProperty__PEAUtagPOINT___Z(pThis, pHit, &ptClick)) break;
        // pHit->m_pWndInPlace == NULL in OpenMFC: retail returns here.
        break;
    }
    default:
        break;
    }
}

// Symbol: ?OnSelectCombo@CMFCPropertyGridCtrl@@IEAAXXZ
// Transcribed from retail entry RVA 0xcae20 (mfc140u):
//     if (m_pSel != NULL) m_pSel->OnSelectCombo();     // property vslot 20
extern "C" void MS_ABI impl__OnSelectCombo_CMFCPropertyGridCtrl__IEAAXXZ(CMFCPropertyGridCtrl* pThis) {
    if (!pThis) return;
    if (CMFCPropertyGridProperty* pSel = CurSelOf(pThis)) {
        pSel->OnSelectCombo();
    }
}

// Symbol: ?OnSetCursor@CMFCPropertyGridCtrl@@IEAAHPEAVCWnd@@II@Z
// Transcribed from retail entry RVA 0xca260 (mfc140u); pWnd and message are not read.
//     if (nHitTest == HTCLIENT) {
//         CPoint pt; ::GetCursorPos(&pt); ::ScreenToClient(m_hWnd, &pt);
//         if (m_bDescriptionArea && abs(pt.y - m_rectList.bottom - 4) <= 2) {          // +0x54c, +0x59c
//             <afxGlobalData init gate>; ::SetCursor(afxGlobalData.m_hcurStretchVert); return TRUE;   // +0x160
//         }
//         if (pt.y <= m_rectList.bottom) {
//             if (abs(pt.x - m_nLeftColumnWidth - m_rectList.left) <= 2) {              // +0x5c0, +0x590
//                 <init gate>; ::SetCursor(afxGlobalData.m_hcurStretch); return TRUE;    // +0x158
//             }
//             ClickArea clickArea;
//             CMFCPropertyGridProperty* pHit = HitTest(pt, &clickArea, FALSE);
//             if (pHit != NULL && pHit == m_pSel && clickArea == ClickValue &&
//                 !::PtInRect(&pHit->m_rectButton, pt) && pHit->OnSetCursor()) return TRUE;   // +0x54; property vslot 22
//         }
//     }
//     return CWnd::OnSetCursor();  -> ?Default@CWnd@@
// DEVIATION: m_rectButton is not modelled (an empty rect, so PtInRect is FALSE);
// the cursor slots are lazily filled first (see EnsureSplitterCursors) because
// this file's Init does not run retail's load.
extern "C" int MS_ABI impl__OnSetCursor_CMFCPropertyGridCtrl__IEAAHPEAVCWnd__II_Z(
    CMFCPropertyGridCtrl* pThis, CWnd* pWnd, unsigned int nHitTest, unsigned int message) {
    (void)pWnd; (void)message;
    if (!pThis) return FALSE;
    if (nHitTest == HTCLIENT && pThis->m_hWnd != nullptr) {
        POINT pt = {0, 0};
        ::GetCursorPos(&pt);
        ::ScreenToClient(pThis->m_hWnd, &pt);
        const PropertyGridWndState& wnd = WndState(pThis);
        const CRect rectList = ListRectOf(pThis);
        if (DescriptionAreaOf(pThis) && std::abs(pt.y - rectList.bottom - 4) <= 2) {
            EnsureSplitterCursors();
            ::SetCursor(static_cast<HCURSOR>(GlobalDataPtr(kGlobalDataCurStretchV)));
            return TRUE;
        }
        if (pt.y <= rectList.bottom) {
            if (std::abs(pt.x - wnd.nLeftColumnWidth - rectList.left) <= 2) {
                EnsureSplitterCursors();
                ::SetCursor(static_cast<HCURSOR>(GlobalDataPtr(kGlobalDataCurStretch)));
                return TRUE;
            }
            int clickArea = 0;
            CMFCPropertyGridProperty* pHit =
                impl__HitTest_CMFCPropertyGridCtrl__QEBAPEAVCMFCPropertyGridProperty__VCPoint__PEAW4ClickArea_2_H_Z(
                    pThis, CPoint(pt.x, pt.y), &clickArea, FALSE);
            if (pHit != nullptr && pHit == CurSelOf(pThis) && clickArea == 2 &&
                impl__OnSetCursor_CMFCPropertyGridProperty__UEBAHXZ(pHit)) {
                return TRUE;
            }
        }
    }
    return static_cast<int>(impl__Default_CWnd__IEAA_JXZ(pThis));
}

// Symbol: ?OnSetFocus@CMFCPropertyGridCtrl@@IEAAXPEAVCWnd@@@Z
// Transcribed from retail entry RVA 0xc9b40 (mfc140u):
//     CWnd::OnSetFocus(pOldWnd);                                          // ?OnSetFocus@CWnd@@
//     m_bFocused = TRUE;                                                  // +0x5f8
//     if (m_pSel != NULL)                                                 // +0x678
//         ::RedrawWindow(m_hWnd, &m_pSel->m_Rect, NULL, RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE);   // +0x44; 0x105
// m_Rect is PropRectOf (see the map above).
extern "C" void MS_ABI impl__OnSetFocus_CMFCPropertyGridCtrl__IEAAXPEAVCWnd___Z(CMFCPropertyGridCtrl* pThis, CWnd* pOldWnd) {
    if (!pThis) return;
    impl__OnSetFocus_CWnd__IEAAXPEAV1__Z(pThis, pOldWnd);
    WndState(pThis).bFocused = TRUE;
    if (CMFCPropertyGridProperty* pSel = CurSelOf(pThis)) {
        const RECT rc = NativeRect(PropRectOf(pThis, pSel));
        if (pThis->m_hWnd != nullptr) {
            ::RedrawWindow(pThis->m_hWnd, &rc, nullptr, RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE);
        }
    }
}

// Symbol: ?OnSetFont@CMFCPropertyGridCtrl@@IEAAXPEAVCFont@@H@Z
// Transcribed from retail entry RVA 0xc6bb0 (mfc140u); bRedraw is not read.
//     m_hFont = pFont != NULL ? (HFONT)pFont->m_hObject : NULL;   // +0x578; CGdiObject +0x8
//     CreateBoldFont();
//     CalcEditMargin();
//     AdjustLayout();  -> tail-jump through grid vslot 94
extern "C" void MS_ABI impl__OnSetFont_CMFCPropertyGridCtrl__IEAAXPEAVCFont__H_Z(CMFCPropertyGridCtrl* pThis, CFont* pFont, int bRedraw) {
    (void)bRedraw;
    if (!pThis) return;
    WndState(pThis).hFont = pFont != nullptr ? static_cast<HFONT>(pFont->m_hObject) : nullptr;
    impl__CreateBoldFont_CMFCPropertyGridCtrl__IEAAXXZ(pThis);
    impl__CalcEditMargin_CMFCPropertyGridCtrl__IEAAXXZ(pThis);
    pThis->AdjustLayout();
}

// Symbol: ?OnSize@CMFCPropertyGridCtrl@@IEAAXIHH@Z
// Transcribed from retail entry RVA 0xc6b50 (mfc140u); nType and cy are not read.
//     CWnd::OnSize();  -> ?Default@CWnd@@
//     EndEditItem(TRUE);                    // grid vslot 100
//     m_nLeftColumnWidth = cx / 2;          // +0x5c0
//     AdjustLayout();  -> tail-jump through grid vslot 94
extern "C" void MS_ABI impl__OnSize_CMFCPropertyGridCtrl__IEAAXIHH_Z(CMFCPropertyGridCtrl* pThis, unsigned int nType, int cx, int cy) {
    (void)nType; (void)cy;
    if (!pThis) return;
    impl__Default_CWnd__IEAA_JXZ(pThis);
    impl__EndEditItem_CMFCPropertyGridCtrl__UEAAHH_Z(pThis, TRUE);
    WndState(pThis).nLeftColumnWidth = cx / 2;
    pThis->AdjustLayout();
}

// Symbol: ?OnSpinDeltaPos@CMFCPropertyGridCtrl@@IEAAXPEAUtagNMHDR@@PEA_J@Z
// Transcribed from retail entry RVA 0xc7d50 (mfc140u); pNMHDR is not read.
//     *pResult = 0;
//     ::PostMessage(m_hWnd, AFX_UM_UPDATESPIN /* WM_USER + 101 == 0x465 */, 0, 0);
// (The posted message lands in OnUpdateSpin below.)
extern "C" void MS_ABI impl__OnSpinDeltaPos_CMFCPropertyGridCtrl__IEAAXPEAUtagNMHDR__PEA_J_Z(
    CMFCPropertyGridCtrl* pThis, NMHDR* pNMHDR, LRESULT* pResult) {
    (void)pNMHDR;
    if (pResult) *pResult = 0;
    if (pThis && pThis->m_hWnd != nullptr) {
        ::PostMessageW(pThis->m_hWnd, WM_USER + 101, 0, 0);
    }
}

// Symbol: ?OnStyleChanged@CMFCPropertyGridCtrl@@IEAAXHPEAUtagSTYLESTRUCT@@@Z
// Transcribed from retail entry RVA 0xc9c20 (mfc140u); nStyleType and
// lpStyleStruct are not read:
//     CWnd::OnStyleChanged(nStyleType, lpStyleStruct);   // the afxwin.h inline: ?Default@CWnd@@ (0x28ac80 mfc140u)
//     AdjustLayout();  -> tail-jump through grid vslot 94
extern "C" void MS_ABI impl__OnStyleChanged_CMFCPropertyGridCtrl__IEAAXHPEAUtagSTYLESTRUCT___Z(
    CMFCPropertyGridCtrl* pThis, int nStyleType, STYLESTRUCT* lpStyleStruct) {
    (void)nStyleType; (void)lpStyleStruct;
    if (!pThis) return;
    impl__Default_CWnd__IEAA_JXZ(pThis);
    pThis->AdjustLayout();
}

// Symbol: ?OnUpdateSpin@CMFCPropertyGridCtrl@@IEAA_J_K_J@Z
// Retail entry RVA 0xc7d70 (mfc140u); wParam and lParam are not read:
//     if (m_pSel != NULL && m_pSel->m_bInPlaceEdit && m_pSel->m_bEnabled)   // +0x678, +0x6c, +0x78
//         m_pSel->OnUpdateValue();                                          // property vslot 12
//     return 0;
// STUB: OpenMFC's property has no m_bInPlaceEdit and never enters in-place
// editing (this file's EditItem only selects), so retail's gate is always
// closed here and the whole reachable body is `return 0`.  Parameter list
// corrected to the mangled signature (this, wParam, lParam).
extern "C" __int64 MS_ABI impl__OnUpdateSpin_CMFCPropertyGridCtrl__IEAA_J_K_J_Z(
    CMFCPropertyGridCtrl* pThis, unsigned __int64 wParam, __int64 lParam) {
    (void)pThis; (void)wParam; (void)lParam;
    return 0;
}

// Symbol: ?OnVScroll@CMFCPropertyGridCtrl@@IEAAXIIPEAVCScrollBar@@@Z
// Parameter list corrected to the mangled signature (this, nSBCode, nPos,
// pScrollBar); the body is still a stub (retail entry RVA 0xc9e40 mfc140u).
extern "C" void MS_ABI impl__OnVScroll_CMFCPropertyGridCtrl__IEAAXIIPEAVCScrollBar___Z(
    CMFCPropertyGridCtrl* pThis, unsigned int nSBCode, unsigned int nPos, CScrollBar* pScrollBar) {
    (void)pThis; (void)nSBCode; (void)nPos; (void)pScrollBar;
}

// Symbol: ?PreSubclassWindow@CMFCPropertyGridCtrl@@MEAAXXZ
// Transcribed from retail entry RVA 0xc6320 (mfc140u); the body makes no
// base-class call before the thread-state read:
//     _AFX_THREAD_STATE* pThreadState = AfxGetThreadState();   // ?AfxGetThreadState@@ 0x1332a0 (mfc140u)
//     if (pThreadState->m_pWndInit == NULL) Init();            // +0x28; grid vslot 104
// OpenMFC keeps the same member as _AFX_THREAD_STATE::pWndInit
// (detail/CWinAppSupport.h; AfxHookWindowCreate / the CBT hook in
// CMFC_misc_stubs.cpp set and clear it for the same purpose), read here
// through AfxThreadStateMirror (see the anonymous namespace above).  Retail
// dereferences the thread state unconditionally; a NULL guard is added.
extern "C" void MS_ABI impl__PreSubclassWindow_CMFCPropertyGridCtrl__MEAAXXZ(CMFCPropertyGridCtrl* pThis) {
    if (!pThis) return;
    const AfxThreadStateMirror* pThreadState =
        reinterpret_cast<const AfxThreadStateMirror*>(impl__AfxGetThreadState__YAPEAV_AFX_THREAD_STATE__XZ());
    if (pThreadState == nullptr || pThreadState->pWndInit == nullptr) {
        impl__Init_CMFCPropertyGridCtrl__MEAAXXZ(pThis);
    }
}

// Symbol: ?PreTranslateMessage@CMFCPropertyGridCtrl@@UEAAHPEAUtagMSG@@@Z
// Transcribed from retail entry RVA 0xc91b0 (mfc140u):
//     switch (pMsg->message) {
//     case WM_KEYDOWN: case WM_SYSKEYDOWN:
//     case WM_LBUTTONDOWN: case WM_LBUTTONUP: case WM_RBUTTONDOWN: case WM_RBUTTONUP:
//     case WM_MBUTTONDOWN: case WM_MBUTTONUP:
//     case WM_NCLBUTTONDOWN: case WM_NCLBUTTONUP: case WM_NCRBUTTONDOWN: case WM_NCRBUTTONUP:
//     case WM_NCMBUTTONDOWN: case WM_NCMBUTTONUP:
//         ::SendMessage(m_ToolTip.m_hWnd, TTM_RELAYEVENT, 0, (LPARAM)pMsg);      // +0x128
//         if (m_IPToolTip.m_hWnd != NULL) m_IPToolTip.ShowWindow(SW_HIDE);         // +0x388; ?ShowWindow@CWnd@@ 0x2a9ad0
//         break;
//     case WM_MOUSEMOVE:
//         ::SendMessage(m_ToolTip.m_hWnd, TTM_RELAYEVENT, 0, (LPARAM)pMsg);
//         if (pMsg->wParam == 0) { CPoint pt; ::GetCursorPos(&pt); ::ScreenToClient(m_hWnd, &pt); TrackToolTip(pt); }
//         break;
//     }
//     if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_TAB && m_pSel != NULL &&
//         m_pSel->OnActivateByTab()) return TRUE;                                  // property vslot 44
//     if (pMsg->message == WM_SYSKEYDOWN && (pMsg->wParam == VK_RIGHT || pMsg->wParam == VK_DOWN) &&
//         m_pSel != NULL && m_pSel->m_bEnabled && (m_pSel->m_dwFlags & AFX_PROP_HAS_BUTTON) &&   // +0x78; +0x40 bit 2
//         EditItem(m_pSel, NULL)) {                                                // grid vslot 98
//         CString strPrev = m_pSel->FormatProperty(); CWaitCursor wait;
//         m_pSel->OnClickButton(CPoint(-1, -1));                                   // property vslot 17
//         if (strPrev != m_pSel->FormatProperty()) OnPropertyChanged(m_pSel);     // wcscmp; grid vslot 95
//         return TRUE;
//     }
//     if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN && m_pSel != NULL &&
//         m_pSel->m_bGroup && !m_pSel->m_bInPlaceEdit) {                           // +0x70, +0x6c
//         m_pSel->Expand(!m_pSel->m_bExpanded); return TRUE;                       // +0x74; ?Expand@ 0xc1600
//     }
//     if (m_pSel == NULL || !m_pSel->m_bInPlaceEdit || !m_pSel->m_bEnabled)
//         return CWnd::PreTranslateMessage(pMsg);                                  // ?PreTranslateMessage@CWnd@@ 0x28bc00
//     -- in-place edit active (0xc9454 .. 0xc9a55 inside this body) --
//     BOOL bDropped = m_pSel->m_pWndCombo != NULL &&                               // +0x108
//                     ::SendMessage(m_pSel->m_pWndCombo->m_hWnd, CB_GETDROPPEDSTATE, 0, 0);
//     if (pMsg->message == WM_KEYDOWN) {
//         if (bDropped && pMsg->wParam != VK_RETURN) return FALSE;
//         switch (pMsg->wParam) {
//         case VK_RETURN:
//             if (m_pSel->m_bButtonIsFocused) { strPrev / OnClickButton(CPoint(-1,-1)) / OnPropertyChanged as above; return TRUE; }   // +0x68
//             if (bDropped) { HWND h = m_pSel->m_pWndInPlace ? m_pSel->m_pWndInPlace->m_hWnd : NULL;   // +0x100, read before the call
//                             m_pSel->OnSelectCombo(); if (::IsWindow(h)) m_pSel->m_pWndInPlace->SetFocus(); }   // vslot 20; ?SetFocus@CWnd@@ 0x2a9b60
//             if (!EndEditItem(TRUE)) ::MessageBeep((UINT)-1);                     // grid vslot 100
//             SetFocus(); return TRUE;
//         case VK_ESCAPE: EndEditItem(FALSE); SetFocus(); return TRUE;
//         case VK_UP: case VK_DOWN:
//             if (m_pSel->m_lstOptions.GetCount() > 1) { m_pSel->OnRotateListValue(pMsg->wParam != VK_UP); return TRUE; }   // +0xe0; vslot 45
//             if (::IsWindow(m_pSel->m_pWndInPlace->m_hWnd)) ::SendMessage(that, WM_KEYDOWN, pMsg->wParam, pMsg->lParam);
//             return TRUE;
//         default:
//             if (!m_pSel->m_bAllowEdit) { m_pSel->PushChar((UINT)pMsg->wParam); return TRUE; }   // +0x7c; vslot 23
//             return ProcessClipboardAccelerators((UINT)pMsg->wParam);             // grid vslot 110
//         }
//     }
//     if (pMsg->message < 0x200 || pMsg->message > 0x20e) return FALSE;            // WM_MOUSEFIRST..WM_MOUSELAST
//     CPoint pt; ::GetCursorPos(&pt); ::ScreenToClient(m_hWnd, &pt);
//     if (m_pSel->m_pWndSpin != NULL) {                                            // +0x110
//         CRect rectSpin; ::GetClientRect(spin, &rectSpin); ::MapWindowPoints(spin, m_hWnd, &rectSpin, 2);
//         if (::PtInRect(&rectSpin, pt)) { ::MapWindowPoints(m_hWnd, spin, &pt, 1);
//             ::SendMessage(spin, pMsg->message, pMsg->wParam, MAKELPARAM(pt.x, pt.y)); return TRUE; }
//     }
//     if (!m_pSel->m_bAllowEdit) ::HideCaret(m_pSel->m_pWndInPlace->m_hWnd);
//     CRect rectEdit; ::GetClientRect(inplace, &rectEdit); ::MapWindowPoints(inplace, m_hWnd, &rectEdit, 2);
//     if (::PtInRect(&rectEdit, pt) && pMsg->message == WM_LBUTTONDBLCLK && m_pSel->OnDblClk(pt)) return TRUE;   // vslot 19
//     if (::PtInRect(&rectEdit, pt) && pMsg->message == WM_RBUTTONDOWN && !m_pSel->m_bAllowEdit) return TRUE;
//     if (!::PtInRect(&rectEdit, pt) && (pMsg->message == WM_LBUTTONDOWN || pMsg->message == WM_RBUTTONDOWN ||
//                                        pMsg->message == WM_MBUTTONDOWN || pMsg->message == WM_NCLBUTTONDOWN)) {
//         if (::PtInRect(&m_pSel->m_rectButton, pt)) {                             // +0x54
//             strPrev; CWaitCursor; OnClickButton(pt) /* grid vslot 99 */; compare / OnPropertyChanged; return TRUE;
//         }
//         if (!EndEditItem(TRUE)) return TRUE;
//         return CWnd::PreTranslateMessage(pMsg);
//     }
//     ::MapWindowPoints(m_hWnd, inplace, &pt, 1);
//     ::SendMessage(inplace, pMsg->message, pMsg->wParam, MAKELPARAM(pt.x, pt.y)); return TRUE;
// (Offsets are read from the image; the member names used for the property's
// +0x68 / +0x7c / +0x110 and its +0xc8 option list follow the
// afxpropertygridctrl.h declaration order around the members already pinned in
// the map at the top of this file.)
// DEVIATIONS: m_ToolTip / m_IPToolTip have no OpenMFC member, so the two
// TTM_RELAYEVENT sends and the SW_HIDE are skipped (TrackToolTip is still
// dispatched, as retail does); AFX_PROP_HAS_BUTTON is the sibling's HasButton
// thunk and m_bGroup is IsGroupOf, as in OnKeyDown; and because OpenMFC never
// enters in-place editing (m_bInPlaceEdit is not modelled and is never set),
// the `!m_pSel->m_bInPlaceEdit` test always hands off to CWnd::PreTranslateMessage
// here -- nothing from the in-place branch above is reachable in this model.
// Retail has no NULL test on pMsg; one is added.
extern "C" int MS_ABI impl__PreTranslateMessage_CMFCPropertyGridCtrl__UEAAHPEAUtagMSG___Z(
    CMFCPropertyGridCtrl* pThis, MSG* pMsg) {
    if (!pThis || pMsg == nullptr) return FALSE;
    switch (pMsg->message) {
    case WM_KEYDOWN: case WM_SYSKEYDOWN:
    case WM_LBUTTONDOWN: case WM_LBUTTONUP: case WM_RBUTTONDOWN: case WM_RBUTTONUP:
    case WM_MBUTTONDOWN: case WM_MBUTTONUP:
    case WM_NCLBUTTONDOWN: case WM_NCLBUTTONUP: case WM_NCRBUTTONDOWN: case WM_NCRBUTTONUP:
    case WM_NCMBUTTONDOWN: case WM_NCMBUTTONUP:
        // m_ToolTip.RelayEvent(pMsg) / m_IPToolTip.ShowWindow(SW_HIDE): no tooltip members here.
        break;
    case WM_MOUSEMOVE:
        // m_ToolTip.RelayEvent(pMsg): no tooltip member here.
        if (pMsg->wParam == 0 && pThis->m_hWnd != nullptr) {
            POINT pt = {0, 0};
            ::GetCursorPos(&pt);
            ::ScreenToClient(pThis->m_hWnd, &pt);
            impl__TrackToolTip_CMFCPropertyGridCtrl__IEAAXVCPoint___Z(pThis, CPoint(pt.x, pt.y));
        }
        break;
    default:
        break;
    }

    CMFCPropertyGridProperty* pSel = CurSelOf(pThis);
    if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_TAB && pSel != nullptr &&
        impl__OnActivateByTab_CMFCPropertyGridProperty__MEAAHXZ(pSel)) {
        return TRUE;
    }
    if (pMsg->message == WM_SYSKEYDOWN && (pMsg->wParam == VK_RIGHT || pMsg->wParam == VK_DOWN) &&
        pSel != nullptr && pSel->IsEnabled() && impl__HasButton_CMFCPropertyGridProperty__MEBAHXZ(pSel) &&
        impl__EditItem_CMFCPropertyGridCtrl__UEAAHPEAVCMFCPropertyGridProperty__PEAUtagPOINT___Z(pThis, pSel, nullptr)) {
        pSel = CurSelOf(pThis);   // retail re-reads m_pSel after EditItem (and has no NULL test; one is added)
        if (pSel != nullptr) {
            ClickButtonAndNotify(pThis, pSel, CPoint(-1, -1));   // strPrev / CWaitCursor / click / compare
        }
        return TRUE;
    }
    if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN && pSel != nullptr && IsGroupOf(pSel)) {
        pSel->Expand(!pSel->IsExpanded());   // m_bInPlaceEdit is never set in OpenMFC
        return TRUE;
    }
    // m_pSel == NULL || !m_bInPlaceEdit || !m_bEnabled: always the base hand-off here.
    return impl__PreTranslateMessage_CWnd__UEAAHPEAUtagMSG___Z(pThis, pMsg);
}

// Symbol: ?ProcessClipboardAccelerators@CMFCPropertyGridCtrl@@MEAAHI@Z
// Retail entry RVA 0xcb870 (mfc140u):
//     if (m_pSel == NULL || m_pSel->m_pWndInPlace == NULL || m_pSel->m_pWndInPlace->m_hWnd == NULL) return FALSE;   // +0x678, +0x100, +0x40
//     BOOL bCtrl  = ::GetAsyncKeyState(VK_CONTROL) & 0x8000;
//     BOOL bShift = ::GetAsyncKeyState(VK_SHIFT) & 0x8000;
//     UINT uiMsg;
//     if      ((bCtrl && (nChar == 'C' || nChar == VK_INSERT)))          uiMsg = WM_COPY;
//     else if ((bCtrl && nChar == 'V') || (bShift && nChar == VK_INSERT)) uiMsg = WM_PASTE;
//     else if ((bCtrl && nChar == 'X') || (bShift && nChar == VK_DELETE)) uiMsg = WM_CUT;
//     else return FALSE;
//     ::SendMessage(m_pSel->m_pWndInPlace->m_hWnd, uiMsg, 0, 0);
//     return TRUE;
// STUB: everything past the first line targets m_pWndInPlace, which OpenMFC's
// property does not have and never creates, so retail's first test is the
// whole reachable body.  Parameter list corrected to the mangled signature
// (this, nChar).
extern "C" int MS_ABI impl__ProcessClipboardAccelerators_CMFCPropertyGridCtrl__MEAAHI_Z(
    CMFCPropertyGridCtrl* pThis, unsigned int nChar) {
    (void)pThis; (void)nChar;
    return FALSE;
}

// Symbol: ?ReposProperties@CMFCPropertyGridCtrl@@IEAAXXZ
// Parameter list corrected to the mangled signature (this); the body is still a
// stub (retail entry RVA 0xc6a60 mfc140u).
extern "C" void MS_ABI impl__ReposProperties_CMFCPropertyGridCtrl__IEAAXXZ(CMFCPropertyGridCtrl* pThis) {
    (void)pThis;
}

// Symbol: ?SetCurrFont@CMFCPropertyGridCtrl@@IEAAPEAUHFONT__@@PEAVCDC@@@Z
// Transcribed from retail entry RVA 0xc6e30 (mfc140u):
//     HFONT hFont = m_hFont != NULL ? m_hFont : (HFONT)::GetStockObject(DEFAULT_GUI_FONT);   // +0x578; 0x11
//     return (HFONT)::SelectObject(pDC != NULL ? pDC->m_hDC : NULL, hFont);                  // CDC +0x8; tail-jump
// (Retail selects into a NULL HDC when pDC is NULL; that is transcribed as-is,
// SelectObject then fails and returns NULL.)
extern "C" HFONT MS_ABI impl__SetCurrFont_CMFCPropertyGridCtrl__IEAAPEAUHFONT____PEAVCDC___Z(
    CMFCPropertyGridCtrl* pThis, CDC* pDC) {
    HFONT hFont = pThis != nullptr ? WndState(pThis).hFont : nullptr;
    if (hFont == nullptr) hFont = static_cast<HFONT>(::GetStockObject(DEFAULT_GUI_FONT));
    const HDC hdc = pDC != nullptr ? pDC->GetSafeHdc() : nullptr;
    return static_cast<HFONT>(::SelectObject(hdc, hFont));
}

// Symbol: ?SetScrollSizes@CMFCPropertyGridCtrl@@IEAAXXZ
// Retail entry RVA 0xc9c60 (mfc140u):
//     if (m_wndScrollVert.m_hWnd == NULL) return;                                  // +0x460 / +0x4a0
//     if (m_nRowHeight == 0) { m_nVertScrollTotal = m_nVertScrollPage = 0; m_nVertScrollOffset = 0; }   // +0x5bc; +0x5c8/+0x5cc; +0x5c4
//     else {
//         m_nVertScrollPage  = m_rectList.Height() / m_nRowHeight - 1;             // +0x590
//         m_nVertScrollTotal = GetTotalItems(FALSE);                               // ?GetTotalItems@ 0xc9da0
//         if (m_nVertScrollTotal <= m_nVertScrollPage) m_nVertScrollTotal = m_nVertScrollPage = 0;
//         m_nVertScrollOffset = min(m_nVertScrollOffset, m_nVertScrollTotal);
//     }
//     SCROLLINFO si = {sizeof si, SIF_RANGE | SIF_PAGE | SIF_POS, 0, m_nVertScrollTotal, m_nVertScrollPage, m_nVertScrollOffset};
//     SetScrollInfo(SB_VERT, &si, TRUE);                                           // CWnd::SetScrollInfo 0x28e890 -> the GetScrollBarCtrl override
//     ::EnableScrollBar(m_wndScrollVert.m_hWnd, SB_CTL, m_nVertScrollTotal > 0 ? ESB_ENABLE_BOTH : ESB_DISABLE_BOTH);
//     m_wndScrollVert.EnableWindow(TRUE);                                          // ?EnableWindow@CWnd@@ 0x2a9b30
// STUB: OpenMFC has no embedded CScrollBar and nothing creates one (this file's
// Init does not run retail's m_wndScrollVert.Create, and GetScrollBarCtrl above
// answers NULL for the same reason), so retail's first test is the whole
// reachable body; the side table's scroll totals stay 0.  Parameter list
// corrected to the mangled signature (this).
extern "C" void MS_ABI impl__SetScrollSizes_CMFCPropertyGridCtrl__IEAAXXZ(CMFCPropertyGridCtrl* pThis) {
    (void)pThis;
}

// Symbol: ?SetVSDotNetLook@CMFCPropertyGridCtrl@@QEAAXH@Z
// Transcribed from retail entry RVA 0xcb170 (mfc140u):
//     m_bVSDotNetLook = bSet;                                                       // +0x558
//     if (m_hWnd != NULL) ::RedrawWindow(m_hWnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE);
extern "C" void MS_ABI impl__SetVSDotNetLook_CMFCPropertyGridCtrl__QEAAXH_Z(CMFCPropertyGridCtrl* pThis, int bSet) {
    if (!pThis) return;
    WndState(pThis).bVSDotNetLook = bSet ? TRUE : FALSE;
    if (pThis->m_hWnd != nullptr) {
        ::RedrawWindow(pThis->m_hWnd, nullptr, nullptr, RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE);
    }
}

// Symbol: ?TrackDescr@CMFCPropertyGridCtrl@@IEAAXH@Z
// Parameter list corrected to the mangled signature (this, nOffset); the body is
// still a stub (retail entry RVA 0xc7f60 mfc140u: InvertRect tracking of
// m_rectTrackDescr, which has no OpenMFC home).
extern "C" void MS_ABI impl__TrackDescr_CMFCPropertyGridCtrl__IEAAXH_Z(CMFCPropertyGridCtrl* pThis, int nOffset) {
    (void)pThis; (void)nOffset;
}

// Symbol: ?TrackHeader@CMFCPropertyGridCtrl@@IEAAXH@Z
// Parameter list corrected to the mangled signature (this, nOffset); the body is
// still a stub (retail entry RVA 0xc7da0 mfc140u: InvertRect tracking of
// m_rectTrackHeader plus the live header resize, neither of which has an OpenMFC home).
extern "C" void MS_ABI impl__TrackHeader_CMFCPropertyGridCtrl__IEAAXH_Z(CMFCPropertyGridCtrl* pThis, int nOffset) {
    (void)pThis; (void)nOffset;
}

// Symbol: ?TrackToolTip@CMFCPropertyGridCtrl@@IEAAXVCPoint@@@Z
// Retail entry RVA 0xc8060 (mfc140u):
//     if (m_bTracking || m_bTrackingDescr) return;                                     // +0x5f0, +0x5f4
//     CPoint ptScreen = point; ::ClientToScreen(m_hWnd, &ptScreen);                     // a stack copy; `point` stays in client coordinates
//     CRect rectTT; ::GetWindowRect(m_IPToolTip.m_hWnd, &rectTT);                        // +0x388
//     if (::PtInRect(&rectTT, ptScreen) && ::IsWindowVisible(m_IPToolTip.m_hWnd)) return;
//     if (!::IsWindowVisible(m_IPToolTip.m_hWnd)) ::SetRectEmpty(&rectTT);
//     if (::GetCapture() == m_hWnd) ::ReleaseCapture();
//     CMFCPropertyGridProperty* pProp = HitTest(point, NULL, FALSE);                    // ?HitTest@ 0xc8630; the client point
//     if (pProp == NULL || labs(point.x - m_nLeftColumnWidth - m_rectList.left) <= 2) { m_IPToolTip.Deactivate(); return; }   // ?Deactivate@CMFCPropertyGridToolTipCtrl@@ 0xccd80
//     if (pProp->m_bInPlaceEdit) return;                                                // +0x6c
//     CString strTip; CRect rectTip = pProp->m_Rect; BOOL bValue = FALSE;               // +0x44
//     if (point.x < m_rectList.left + m_nLeftColumnWidth) {
//         if (pProp->m_bGroup) { rectTip.left += m_nRowHeight; if (point.x <= rectTip.left) { m_IPToolTip.Deactivate(); return; } }   // +0x70
//         if (pProp-><+0x84 name-truncated flag>) {
//             BOOL bShow = TRUE;
//             if (<+0x550 grid flag>) bShow = pProp->GetNameTooltip().IsEmpty();       // property vslot 24
//             if (bShow) strTip = pProp->m_strName;                                     // +0xa0
//         }
//     } else {
//         if (pProp-><+0x88 value-truncated flag>) {
//             BOOL bShow = TRUE;
//             if (<+0x550>) bShow = pProp->GetValueTooltip().IsEmpty();                 // property vslot 25
//             if (bShow) strTip = pProp->FormatProperty();                              // vslot 11
//         }
//         rectTip.left = m_rectList.left + m_nLeftColumnWidth + 1; bValue = TRUE;
//     }
//     if (strTip.IsEmpty()) { m_IPToolTip.Deactivate(); return; }
//     ClientToScreen(&rectTip);
//     if (rectTip.TopLeft() == rectTT.TopLeft()) return;                                 // already shown there
//     m_IPToolTip.<+0x100> = 4;                                                          // grid +0x448 = m_IPToolTip (+0x348) + 0x100
//     m_IPToolTip.SetFont(bValue && pProp-><+0x8c modified flag> && m_bMarkModifiedProperties   // +0x55c
//                         ? &m_fontBold : CFont::FromHandle((HFONT)::SendMessage(m_hWnd, WM_GETFONT, 0, 0)));   // WM_SETFONT to +0x388
//     m_IPToolTip.Track(rectTip, strTip);                                               // ?Track@CMFCPropertyGridToolTipCtrl@@ 0xcc9f0
//     SetCapture();                                                                     // ::SetCapture(m_hWnd) -> ?FromHandle@CWnd@@
// (Only the offsets above are read from the image; the +0x84 / +0x88 / +0x8c /
// +0x550 flag names are not established here.)
// STUB: the body exists to position m_IPToolTip (+0x348 CMFCPropertyGridToolTipCtrl),
// which has no OpenMFC member and whose sibling Create/Track are themselves
// stubs; every path that does not return early ends in a call on it, so there
// is nothing honest to transcribe.  The truncation flags it reads are not
// modelled either.  Parameter list corrected to the mangled signature (this, point).
extern "C" void MS_ABI impl__TrackToolTip_CMFCPropertyGridCtrl__IEAAXVCPoint___Z(CMFCPropertyGridCtrl* pThis, CPoint point) {
    (void)pThis; (void)point;
}

// Symbol: ?UpdateColor@CMFCPropertyGridCtrl@@UEAAXK@Z
// Retail entry RVA 0xcad10 (mfc140u):
//     if (m_pSel == NULL || !m_pSel->IsKindOf(RUNTIME_CLASS(CMFCPropertyGridColorProperty))) return;   // ?IsKindOf@CObject@@ 0x234cf0
//     CMFCPropertyGridColorProperty* pColorProp = (CMFCPropertyGridColorProperty*)m_pSel;
//     COLORREF colorOld = pColorProp->m_Color;                                      // +0x160
//     pColorProp->SetColor(color);                                                  // ?SetColor@CMFCPropertyGridColorProperty@@ 0xc51e0
//     if (color != colorOld) OnPropertyChanged(m_pSel);                             // grid vslot 95
//     if (color == (COLORREF)-1 && m_pSel->m_pWndInPlace != NULL && ::IsWindow(m_pSel->m_pWndInPlace->m_hWnd))   // +0x100
//         m_pSel->m_pWndInPlace->SetWindowText(L"");                                // ?SetWindowTextW@CWnd@@ 0x2a9790
//     m_pSel->OnUpdateValue();                                                      // property vslot 12
// STUB: OpenMFC declares no CMFCPropertyGridColorProperty class at all (no
// m_Color; the sibling's SetColor is a placeholder-typed no-op without `this`),
// so neither the colour store nor the changed-comparison that gates
// OnPropertyChanged can be reproduced; calling only the trailing OnUpdateValue
// would notify a value that never changed.  Parameter list corrected to the
// mangled signature (this, color).
extern "C" void MS_ABI impl__UpdateColor_CMFCPropertyGridCtrl__UEAAXK_Z(CMFCPropertyGridCtrl* pThis, unsigned long color) {
    (void)pThis; (void)color;
}
