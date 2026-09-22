// CMFCToolBarEditBoxButton — OpenMFC implementation.
// Sources: cbarcore.cpp, mfccore.cpp
//
// Every body marked "Retail (RVA 0x..., mfc140u)" below was transcribed from the
// disassembly of that entry point in mfc140u.dll (14.51.36231).  The instance
// layout those bodies read is the one in the SDK header
// afxtoolbareditboxbutton.h, confirmed by the member offsets the retail
// constructor ??0CMFCToolBarEditBoxButton@@QEAA@XZ (RVA 0x167690, mfc140u)
// stores to (sizeof 176 == 0xb0, RTTI m_nObjectSize 0xb0 at 0x3b16b8):
//
//   CMFCToolBarButton base ............................. +0x00 .. +0x88
//   +0x88  int     m_iWidth          +0x8c  DWORD m_dwStyle
//   +0x90  CEdit*  m_pWndEdit        +0x98  CString m_strContents
//   +0xa0  BOOL    m_bChangingText   +0xa4  BOOL  m_bHorz
//   +0xa8  BOOL    m_bIsHotEdit      +0xac  UINT  m_uiMenuResID
//
// include/openmfc/afxmfc.h declares only `BOOL m_bFlat; char _pad[36];` after
// the base.  In the real class m_bFlat is STATIC (?m_bFlat@CMFCToolBarEditBoxButton@@1HA
// at 0x3b1b78 in mfc140u, 1 in retail .data; OpenMFC's copy lives in
// featurepack/toolbar/StaticData.cpp and is 0); the instance slot the header
// declares aliases m_iWidth.  The bodies here therefore reach the retail fields
// by offset (the accessors in the anonymous namespace) and never through that
// header member.
//
// Several exports of this class have no RVA of their own in the rva-symbols
// JSON because the linker folded them (ICF) onto byte-identical bodies of
// CMFCToolBarComboBoxButton, or because that JSON simply lacks them; every
// such body was located through the class vftable at 0x317e80 (mfc140u; the
// address the retail constructor installs), whose slot order is the virtual
// declaration order of afxtoolbarbutton.h / afxtoolbareditboxbutton.h, and
// then confirmed against the DLL's own export address table (ordinal ->
// RVA, read from mfc140u.dll's export directory): Serialize 12818, OnMove
// 10521, OnSize 11260, OnGlobalFontsChanged 10056, OnShow 11165, SetStyle
// 13625, OnUpdateToolTip 11560, SetACCData 12882, SetContents 13067,
// SetContentsAll 13068 and SetHotEdit 13247 all resolve to the RVAs below:
//   slot 2  Serialize        0x167930     slot 14 OnMove       0x167cc0
//   slot 15 OnSize           0x167da0     slot 16 GetHwnd      0x1675d0
//   slot 19 OnAddToCustomizePage 0x163440 (== CMFCToolBarComboBoxButton's)
//   slot 20 OnCtlColor       0x1634d0 (== CMFCToolBarComboBoxButton's)
//   slot 25 OnGlobalFontsChanged 0x168ac0  slot 35 OnShow      0x168620
//   slot 37 SetStyle         0x1688e0     slot 43 HasFocus     0x23920 (base)
//   slot 45 OnUpdateToolTip  0x1689e0     slot 48 SetACCData   0x168b20
//   slot 53 CreateEdit       0x168590     slot 54 SetContents  0x168690
//   slot 55 GetEditBorder    0x1683d0     slot 56 OnShowEditbox (base inline no-op)
// The non-virtual SetHotEdit is 0x168950 (also absent from the JSON): three
// CMFCToolBarEditCtrl message handlers reach it through m_buttonEdit (+0x150
// of the edit ctrl) -- the tail-jump at 0x168dcc inside OnKillFocus (0x168db0,
// ordinal 10270) after CWnd::Default (0x28ac80); the call at 0x168dfa inside
// OnMouseMove (0x168de0, ordinal 10503) after CMFCEditBrowseCtrl::OnMouseMove
// (0x5f520), followed by ::TrackMouseEvent(TME_LEAVE) (IAT 0x2c7310); and the
// call at 0x168e6f inside OnMouseLeave (0x168e40, ordinal 10467) behind a
// ::GetFocus test.  Those three entries and ordinals come from the export
// address table as well.
//
// DEVIATION (whole file): the window object CreateEdit builds is a plain
// OpenMFC CWnd carrying the "EDIT" class, not a CMFCToolBarEditCtrl -- that
// class's exported constructor (featurepack/toolbar/CMFCToolBarEditCtrl.cpp)
// is a null stub which constructs nothing, so there is no object to build.
// The control therefore lacks the retail subclass behaviour (hot-tracking
// via SetHotEdit on focus / mouse, the context menu, PreTranslateMessage);
// EN_UPDATE still reaches NotifyCommand through the toolbar's WM_COMMAND.

#define OPENMFC_APPCORE_IMPL

#include "detail/CbarcoreSupport.h"
#include "detail/MfccoreSupport.h"

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <new>

// ---------------------------------------------------------------------------
// Cross-file thunks.  Each was grepped to its definition in the file named;
// the parameter lists are derived from the mangled names.
// ---------------------------------------------------------------------------
//   core/runtime/AFX_GLOBAL_DATA.cpp : Initialize (an empty stub today)
//   featurepack/CMFC_misc_stubs.cpp  : ?afxGlobalData@@ storage, a zeroed 720-byte block
extern "C" void MS_ABI impl__Initialize_AFX_GLOBAL_DATA__QEAAXXZ(void* pThis);
extern "C" unsigned char impl__afxGlobalData__3UAFX_GLOBAL_DATA__A[720];                 // 0x1803c1620 (mfc140u)
//   featurepack/toolbar/StaticData.cpp : ?m_bFlat@CMFCToolBarEditBoxButton@@1HA (0x3b1b78, retail 1; OpenMFC 0)
extern "C" std::int32_t impl__m_bFlat_CMFCToolBarEditBoxButton__1HA;
//   featurepack/toolbar/StaticData.cpp : ?m_bCustomizeMode@CMFCToolBar@@1HA (0x3be35c)
extern "C" std::int32_t impl__m_bCustomizeMode_CMFCToolBar__1HA;
//   core/runtime/StaticData.cpp : ?m_pVisManager@CMFCVisualManager@@1PEAV1@EA (0x3be3c0)
extern "C" void* impl__m_pVisManager_CMFCVisualManager__1PEAV1_EA;
//   featurepack/visualmanager/Thunks.cpp : dispatches virtually on the C++ manager object
extern "C" void MS_ABI impl__OnDrawEditBorder_CMFCVisualManager__UEAAXPEAVCDC__VCRect__HHPEAVCMFCToolBarEditBoxButton___Z(
    CMFCVisualManager* pThis, CDC* pDC, CRect rect, int bDisabled, int bIsHighlighted, CMFCToolBarEditBoxButton* pButton);
//   featurepack/toolbar/CMFCToolBar.cpp : GetHotTextColor, GetCommandButtons (walks the live toolbars)
extern "C" unsigned long MS_ABI impl__GetHotTextColor_CMFCToolBar__SAKXZ();
extern "C" int MS_ABI impl__GetCommandButtons_CMFCToolBar__SAHIAEAVCObList___Z(unsigned int uiCmd, CObList* pLstButtons);
//   featurepack/toolbar/RuntimeClasses.cpp : GetThisClass (RTTI descriptor)
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMFCToolBarEditBoxButton__SAPEAUCRuntimeClass__XZ();
//   core/runtime/CObject.cpp : IsKindOf
extern "C" int MS_ABI impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(const CObject* pThis, const CRuntimeClass* pClass);
//   core/collections/CObList.cpp : ctor / dtor / FindIndex (the walk pattern of
//   featurepack/toolbar/CMFCToolBarComboBoxButton.cpp, CommandButtonList)
extern "C" void* MS_ABI impl___0CObList__QEAA__J_Z(void* pThis, long long nBlockSize);
extern "C" void MS_ABI impl___1CObList__UEAA_XZ(void* pThis);
extern "C" CObList::POSITION MS_ABI impl__FindIndex_CObList__QEBAPEAU__POSITION____J_Z(const CObList* pThis, long long nIndex);
//   core/runtime/Thunks.cpp : CArchive::Read / Write
extern "C" unsigned int MS_ABI impl__Read_CArchive__QEAAIPEAXI_Z(CArchive* pThis, void* pBuf, unsigned int nMax);
extern "C" void MS_ABI impl__Write_CArchive__QEAAXPEBXI_Z(CArchive* pThis, const void* pBuf, unsigned int nMax);
//   featurepack/toolbar/CMFCToolBarButton.cpp : the base-class overrides.
//   CopyFrom and Serialize are real bodies (tbb_CopyFrom / tbb_Serialize);
//   OnCalculateSize, OnChangeParentWnd, OnDraw, OnDrawOnCustomizeList and
//   SetACCData are still null stubs there (with placeholder parameter lists --
//   harmless across translation units for extern "C" symbols).  The lists
//   below are the retail ones: SIZE is returned through a hidden pointer in
//   RDX (the retail body writes the result to [rdx] and returns rdx in rax).
extern "C" void MS_ABI impl__CopyFrom_CMFCToolBarButton__UEAAXAEBV1__Z(void* pThis, const CMFCToolBarButton* pSrc);
extern "C" void MS_ABI impl__Serialize_CMFCToolBarButton__UEAAXAEAVCArchive___Z(void* pThis, CArchive* ar);
extern "C" SIZE* MS_ABI impl__OnCalculateSize_CMFCToolBarButton__UEAA_AUtagSIZE__PEAVCDC__AEBVCSize__H_Z(
    CMFCToolBarButton* pThis, SIZE* pRet, CDC* pDC, const SIZE* pSizeDefault, int bHorz);
extern "C" void MS_ABI impl__OnChangeParentWnd_CMFCToolBarButton__UEAAXPEAVCWnd___Z(CMFCToolBarButton* pThis, CWnd* pWndParent);
extern "C" void MS_ABI impl__OnDraw_CMFCToolBarButton__UEAAXPEAVCDC__AEBVCRect__PEAVCMFCToolBarImages__HHHHH_Z(
    CMFCToolBarButton* pThis, CDC* pDC, const RECT* pRect, CMFCToolBarImages* pImages,
    int bHorz, int bCustomizeMode, int bHighlight, int bDrawBorder, int bGrayDisabledButtons);
extern "C" int MS_ABI impl__OnDrawOnCustomizeList_CMFCToolBarButton__UEAAHPEAVCDC__AEBVCRect__H_Z(
    CMFCToolBarButton* pThis, CDC* pDC, const RECT* pRect, int bSelected);
extern "C" int MS_ABI impl__SetACCData_CMFCToolBarButton__UEAAHPEAVCWnd__AEAVCAccessibilityData___Z(
    CMFCToolBarButton* pThis, CWnd* pParent, CAccessibilityData* pData);
//   core/window/CWnd.cpp / core/window/Thunks.cpp / core/window/CtorDtorPlacement.cpp
extern "C" CWnd* MS_ABI impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(HWND hWnd);
extern "C" unsigned long MS_ABI impl__GetStyle_CWnd__QEBAKXZ(const CWnd* pThis);
extern "C" int MS_ABI impl__ShowWindow_CWnd__QEAAHH_Z(CWnd* pThis, int nCmdShow);
extern "C" int MS_ABI impl__EnableWindow_CWnd__QEAAHH_Z(CWnd* pThis, int bEnable);
extern "C" void MS_ABI impl__SetWindowTextW_CWnd__QEAAXPEB_W_Z(CWnd* pThis, const wchar_t* lpsz);
extern "C" void MS_ABI impl__GetWindowTextW_CWnd__QEBAXAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
    const CWnd* pThis, CString* pString);
extern "C" int MS_ABI impl__SetWindowPos_CWnd__QEAAHPEBV1_HHHHI_Z(
    CWnd* pThis, const CWnd* pWndInsertAfter, int x, int y, int cx, int cy, unsigned int nFlags);
extern "C" void MS_ABI impl__ScreenToClient_CWnd__QEBAXPEAUtagRECT___Z(const CWnd* pThis, RECT* pRect);
extern "C" int MS_ABI impl__DestroyWindow_CWnd__UEAAHXZ(CWnd* pThis);
extern "C" void* MS_ABI impl___0CWnd__QEAA_XZ(void* pThis);
extern "C" void MS_ABI impl___1CWnd__UEAA_XZ(void* pThis);
//   detail/CtrlcoreSupport.cpp : CEdit::Create (CWnd::Create with the "EDIT" class)
extern "C" int MS_ABI impl__Create_CEdit__QEAAHKAEBUtagRECT__PEAVCWnd__I_Z(
    CEdit* pThis, DWORD dwStyle, const RECT* pRect, CWnd* pParentWnd, UINT nID);
//   core/controls/Thunks.cpp : CToolTipCtrl::AddTool
extern "C" int MS_ABI impl__AddTool_CToolTipCtrl__QEAAHPEAVCWnd__PEB_WPEBUtagRECT___K_Z(
    CToolTipCtrl* pThis, CWnd* pWnd, const wchar_t* lpszText, const RECT* lpRectTool, unsigned __int64 nIDTool);
//   core/gdi/CDC.cpp / detail/GdicoreSupport.cpp
extern "C" unsigned long MS_ABI impl__SetTextColor_CDC__UEAAKK_Z(CDC* pThis, unsigned long crColor);
extern "C" unsigned long MS_ABI impl__SetBkColor_CDC__UEAAKK_Z(CDC* pThis, unsigned long crColor);
extern "C" void MS_ABI impl__Draw3dRect_CDC__QEAAXPEBUtagRECT__KK_Z(CDC* pThis, const RECT* lpRect, unsigned long clrTopLeft, unsigned long clrBottomRight);
extern "C" int MS_ABI impl__DrawTextW_CDC__QEAAHPEB_WHPEAUTAGRECT__I_Z(
    CDC* pThis, const wchar_t* lpszString, int nCount, RECT* lpRect, unsigned int nFormat);
//   detail/MemcoreSupport.cpp : ??2@YAPEAX_K@Z / ??3@YAXPEAX@Z
extern "C" void* MS_ABI impl___2_YAPEAX_K_Z(std::size_t size);
extern "C" void MS_ABI impl___3_YAXPEAX_Z(void* ptr);

// Exports defined later in this file that earlier bodies call (all in this TU).
extern "C" void MS_ABI impl__Initialize_CMFCToolBarEditBoxButton__IEAAXXZ(CMFCToolBarEditBoxButton* pThis);
extern "C" void MS_ABI impl__OnMove_CMFCToolBarEditBoxButton__UEAAXXZ(CMFCToolBarEditBoxButton* pThis);
extern "C" void MS_ABI impl__CopyFrom_CMFCToolBarEditBoxButton__UEAAXAEBVCMFCToolBarButton___Z(
    CMFCToolBarEditBoxButton* pThis, const CMFCToolBarButton* pSrc);
extern "C" CEdit* MS_ABI impl__CreateEdit_CMFCToolBarEditBoxButton__UEAAPEAVCEdit__PEAVCWnd__AEBVCRect___Z(
    CMFCToolBarEditBoxButton* pThis, CWnd* pWndParent, const RECT* pRect);
extern "C" void MS_ABI impl__GetEditBorder_CMFCToolBarEditBoxButton__UEAAXAEAVCRect___Z(
    CMFCToolBarEditBoxButton* pThis, RECT* pRectBorder);
extern "C" void MS_ABI impl__SetContents_CMFCToolBarEditBoxButton__UEAAXAEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
    CMFCToolBarEditBoxButton* pThis, const CString* pContents);
extern "C" CMFCToolBarEditBoxButton* MS_ABI impl__GetByCmd_CMFCToolBarEditBoxButton__SAPEAV1_I_Z(unsigned int uiCmd);

namespace {

// Retail member offsets (see the file header).  Cross-checked against the
// header's own layout: the base is 136 bytes, the header's BOOL sits at
// +0x88 and _pad starts at +0x8c.
constexpr int kOffWidth        = 0x88;
constexpr int kOffStyle        = 0x8c;
constexpr int kOffWndEdit      = 0x90;
constexpr int kOffStrContents  = 0x98;
constexpr int kOffChangingText = 0xa0;
constexpr int kOffHorz         = 0xa4;
constexpr int kOffIsHotEdit    = 0xa8;
constexpr int kOffMenuResID    = 0xac;

static_assert(sizeof(CMFCToolBarButton) == 0x88, "CMFCToolBarEditBoxButton's own members start at +0x88");
static_assert(offsetof(CMFCToolBarEditBoxButton, m_bFlat) == kOffWidth, "header m_bFlat slot aliases retail m_iWidth");
static_assert(offsetof(CMFCToolBarEditBoxButton, _pad) == kOffStyle, "retail m_dwStyle is the first padding dword");
static_assert(sizeof(CMFCToolBarEditBoxButton) == 0xb0, "retail sizeof(CMFCToolBarEditBoxButton) is 176");
static_assert(offsetof(CMFCToolBarButton, m_bTextBelow) == 0x1c, "m_bTextBelow @0x1c (the label flag the retail bodies test)");
static_assert(offsetof(CMFCToolBarButton, m_nID) == 0x24, "m_nID @0x24");
static_assert(offsetof(CMFCToolBarButton, m_nStyle) == 0x28, "m_nStyle @0x28");
static_assert(offsetof(CMFCToolBarButton, m_strText) == 0x38, "m_strText @0x38");
static_assert(offsetof(CMFCToolBarButton, m_bIsHidden) == 0x54, "m_bIsHidden @0x54");
static_assert(offsetof(CMFCToolBarButton, m_bVisible) == 0x64, "m_bVisible @0x64");
static_assert(offsetof(CMFCToolBarButton, m_rect) == 0x68, "m_rect @0x68");
static_assert(offsetof(CMFCToolBarButton, m_sizeText) == 0x78, "m_sizeText @0x78");
static_assert(offsetof(CWnd, m_hWnd) == 0x40, "CWnd::m_hWnd @0x40 (every retail body reads the HWND there)");
static_assert(offsetof(CDC, m_hDC) == 0x08, "CDC::m_hDC @0x08 (OnDrawOnCustomizeList's ::FillRect target)");
static_assert(offsetof(CAccessibilityData, m_strAccValue) == 0x08, "CAccessibilityData::m_strAccValue @0x08");
static_assert(offsetof(CAccessibilityData, m_strAccDefAction) == 0x28, "CAccessibilityData::m_strAccDefAction @0x28");
static_assert(offsetof(CAccessibilityData, m_nAccRole) == 0x30, "CAccessibilityData::m_nAccRole @0x30");
static_assert(offsetof(CAccessibilityData, m_bAccState) == 0x34, "CAccessibilityData::m_bAccState @0x34");
static_assert(sizeof(CString) == sizeof(void*), "CString is one pointer (m_strContents occupies +0x98..+0xa0)");
static_assert(sizeof(CRect) == sizeof(RECT), "CRect is RECT-shaped");

template <class T> inline T& At(void* p, int off) {
    return *reinterpret_cast<T*>(static_cast<char*>(p) + off);
}
template <class T> inline const T& At(const void* p, int off) {
    return *reinterpret_cast<const T*>(static_cast<const char*>(p) + off);
}

inline int&           Width(CMFCToolBarEditBoxButton* p)               { return At<int>(p, kOffWidth); }
inline int            Width(const CMFCToolBarEditBoxButton* p)         { return At<int>(p, kOffWidth); }
inline DWORD&         Style(CMFCToolBarEditBoxButton* p)               { return At<DWORD>(p, kOffStyle); }
inline DWORD          Style(const CMFCToolBarEditBoxButton* p)         { return At<DWORD>(p, kOffStyle); }
inline CEdit*&        WndEdit(CMFCToolBarEditBoxButton* p)             { return At<CEdit*>(p, kOffWndEdit); }
inline CEdit*         WndEdit(const CMFCToolBarEditBoxButton* p)       { return At<CEdit*>(p, kOffWndEdit); }
inline CString&       StrContents(CMFCToolBarEditBoxButton* p)         { return At<CString>(p, kOffStrContents); }
inline const CString& StrContents(const CMFCToolBarEditBoxButton* p)   { return At<CString>(p, kOffStrContents); }
inline int&           ChangingText(CMFCToolBarEditBoxButton* p)        { return At<int>(p, kOffChangingText); }
inline int            ChangingText(const CMFCToolBarEditBoxButton* p)  { return At<int>(p, kOffChangingText); }
inline int&           Horz(CMFCToolBarEditBoxButton* p)                { return At<int>(p, kOffHorz); }
inline int            Horz(const CMFCToolBarEditBoxButton* p)          { return At<int>(p, kOffHorz); }
inline int&           IsHotEdit(CMFCToolBarEditBoxButton* p)           { return At<int>(p, kOffIsHotEdit); }
inline int            IsHotEdit(const CMFCToolBarEditBoxButton* p)     { return At<int>(p, kOffIsHotEdit); }
inline unsigned&      MenuResID(CMFCToolBarEditBoxButton* p)           { return At<unsigned>(p, kOffMenuResID); }
inline unsigned       MenuResID(const CMFCToolBarEditBoxButton* p)     { return At<unsigned>(p, kOffMenuResID); }

inline HWND HWndOf(const CWnd* pWnd) { return pWnd ? pWnd->m_hWnd : nullptr; }
// `pWnd->GetSafeHwnd() != NULL` as retail spells it: the pointer, then +0x40.
inline bool HasWindow(const CWnd* pWnd) { return pWnd != nullptr && pWnd->m_hWnd != nullptr; }
inline bool StyleVisible(const CWnd* pWnd) { return (impl__GetStyle_CWnd__QEBAKXZ(pWnd) & WS_VISIBLE) != 0; }

// Message / style numbers the retail bodies use as immediates.
constexpr int      kEnUpdate            = 0x400;                     // EN_UPDATE
constexpr UINT     kEmSetSel            = 0xb1;                      // EM_SETSEL
constexpr UINT     kEmScrollCaret       = 0xb7;                      // EM_SCROLLCARET
constexpr unsigned kTbbsDisabled        = 0x40000;                   // TBBS_DISABLED
constexpr unsigned kSwpNoZorderNoActivate = 0x14;                    // SWP_NOZORDER | SWP_NOACTIVATE
constexpr DWORD    kDefaultEditStyle    = 0x50000080;                // WS_CHILD|WS_VISIBLE|ES_AUTOHSCROLL
constexpr int      kDefaultWidth        = 150;                       // 0x96
constexpr unsigned kDtTextLabel         = 0x11;                      // DT_CENTER | DT_WORDBREAK              (OnDraw label)
constexpr unsigned kDtCalcLabel         = 0x411;                     // DT_CALCRECT | DT_CENTER | DT_WORDBREAK (OnCalculateSize)
constexpr int      kRoleSystemText      = 0x2a;                      // ROLE_SYSTEM_TEXT
constexpr long     kStateSystemFocusable = 0x100000;                 // STATE_SYSTEM_FOCUSABLE
constexpr long     kStateSystemFocused   = 0x4;                      // STATE_SYSTEM_FOCUSED

// ---------------------------------------------------------------------------
// afxGlobalData.  AFX_GLOBAL_DATA is not declared in OpenMFC's headers;
// ?afxGlobalData@@ is a zeroed 720-byte block whose Initialize thunk is an
// empty stub.  The slots below are the byte offsets the retail bodies in this
// file read, named after atlmfc/include/afxglobals.h (4 BOOLs, 3 HBRUSHes,
// then the COLORREFs from clrBtnFace at +0x28; the CBrushes from brBtnFace at
// +0xa8, 16 bytes each; fontRegular is the CFont at +0x1a8, its HFONT at
// +0x1b0; m_nTextHeightHorz at +0x2a8).  Each colour slot was cross-checked
// against ?UpdateSysColors@AFX_GLOBAL_DATA@@QEAAXXZ (RVA 0x6b1c0, mfc140u),
// which stores GetSysColor(n) at:
//   +0x034 clrBtnText    <- COLOR_BTNTEXT(18)      +0x044 clrGrayedText <- COLOR_GRAYTEXT(17)
//   +0x064 clrBarShadow  <- COLOR_BTNSHADOW(16)    +0x078 clrWindow     <- COLOR_WINDOW(5)
//   +0x07c clrWindowText <- COLOR_WINDOWTEXT(8)
//   +0x110 brWindow.m_hObject (CBrush at +0x108; UpdateSysColors recreates it as CreateSolidBrush(clrWindow))
// and +0x2a8 against ?UpdateTextMetrics@AFX_GLOBAL_DATA@@IEAAXXZ (RVA 0x6b7a0,
// mfc140u -- exported at ordinal 14166 although the rva-symbols JSON lacks
// it), which selects fontRegular (+0x1a8) into a CWindowDC(NULL), calls
// ::GetTextMetricsW (IAT 0x2c62a8) and stores `tm.tmHeight + (tm.tmHeight < 15
// ? 2 : 5)` at +0x2a8 (the store at 0x6b818 inside it).
constexpr int kGlobalDataInitGate          = 0x000;
constexpr int kGlobalDataClrBtnText        = 0x034;
constexpr int kGlobalDataClrGrayedText     = 0x044;
constexpr int kGlobalDataClrBarShadow      = 0x064;
constexpr int kGlobalDataClrWindow         = 0x078;
constexpr int kGlobalDataClrWindowText     = 0x07c;
constexpr int kGlobalDataBrWindowHandle    = 0x110;
constexpr int kGlobalDataFontRegularHandle = 0x1b0;
constexpr int kGlobalDataTextHeightHorz    = 0x2a8;

inline unsigned char* GlobalDataBytes() { return impl__afxGlobalData__3UAFX_GLOBAL_DATA__A; }
inline int GlobalDataInt(int off) { int v; std::memcpy(&v, GlobalDataBytes() + off, sizeof v); return v; }
inline void* GlobalDataPtr(int off) { void* v; std::memcpy(&v, GlobalDataBytes() + off, sizeof v); return v; }

// The retail one-time gate every reader does first:
//     if (afxGlobalData.<+0> == 0) { afxGlobalData.Initialize(); <+0> = 1; }
inline void EnsureGlobalDataInitialized() {
    if (GlobalDataInt(kGlobalDataInitGate) == 0) {
        impl__Initialize_AFX_GLOBAL_DATA__QEAAXXZ(GlobalDataBytes());
        const int one = 1;
        std::memcpy(GlobalDataBytes() + kGlobalDataInitGate, &one, sizeof one);
    }
}
// DEVIATION (shared by every colour / brush / font / metric read below):
// because OpenMFC's AFX_GLOBAL_DATA::Initialize is an empty stub, the block
// never gets its UpdateSysColors / UpdateFonts / UpdateTextMetrics pass and
// every slot reads 0.  Retail would have populated them from exactly the
// GetSysColor / GetSysColorBrush / NONCLIENTMETRICS / GetTextMetrics values,
// so when the block is unpopulated (brWindow at +0x110 still NULL --
// UpdateSysColors is what creates it) the readers fall back to the live
// system value the slot would hold.  Once Initialize is implemented the
// fallbacks are never taken.
inline bool GlobalDataPopulated() { return GlobalDataPtr(kGlobalDataBrWindowHandle) != nullptr; }
inline COLORREF GlobalColor(int off, int sysColorIndex) {
    EnsureGlobalDataInitialized();
    if (GlobalDataPopulated()) return static_cast<COLORREF>(GlobalDataInt(off));
    return ::GetSysColor(sysColorIndex);
}
inline HBRUSH GlobalWindowBrush() {
    EnsureGlobalDataInitialized();
    if (GlobalDataPopulated()) return static_cast<HBRUSH>(GlobalDataPtr(kGlobalDataBrWindowHandle));
    return ::GetSysColorBrush(COLOR_WINDOW);
}
inline HFONT GlobalRegularFont() {
    EnsureGlobalDataInitialized();
    HFONT h = static_cast<HFONT>(GlobalDataPtr(kGlobalDataFontRegularHandle));
    if (h == nullptr) h = static_cast<HFONT>(::GetStockObject(DEFAULT_GUI_FONT));   // fallback, see above
    return h;
}
// afxGlobalData.GetTextHeight(TRUE), i.e. m_nTextHeightHorz (+0x2a8).  The
// fallback recomputes it the way UpdateTextMetrics (0x6b7a0) does (a screen
// DC via ::GetDC(NULL) here, CWindowDC(NULL) there), on the fallback font,
// so the edit control gets a usable height instead of 0.
inline int GlobalTextHeightHorz() {
    EnsureGlobalDataInitialized();
    if (GlobalDataPopulated()) return GlobalDataInt(kGlobalDataTextHeightHorz);
    int nHeight = 0;
    if (HDC hdc = ::GetDC(nullptr)) {
        HGDIOBJ hOld = ::SelectObject(hdc, GlobalRegularFont());
        TEXTMETRICW tm;
        std::memset(&tm, 0, sizeof tm);
        if (::GetTextMetricsW(hdc, &tm)) nHeight = tm.tmHeight + (tm.tmHeight < 15 ? 2 : 5);
        ::SelectObject(hdc, hOld);
        ::ReleaseDC(nullptr, hdc);
    }
    return nHeight;
}

// CMFCToolBarButton::IsEditable() as the retail vftable slot 26 body reads
// (RVA 0x23760, mfc140u -- the inline from afxtoolbarbutton.h, IsStandardCommand
// folded in), exactly as featurepack/toolbar/CMFCToolBarComboBoxButton.cpp
// transcribed it:
//     UINT id = m_nID;
//     if (id - 0xE110 <= 0xF)  return FALSE;      // ID_FILE_MRU_FILE1..16
//     if (id - 0xF000 <= 0x1EF) return FALSE;     // 0xF000..0xF1EF
//     if ((int)id >= 0xFF00) return FALSE;
//     if (id - 0xE210 <= 0xF)  return FALSE;      // 0xE210..0xE21F
//     if (afxUserToolsManager && afxUserToolsManager->+0x40 == id) return FALSE;
//     if (CMFCToolBarButton::m_lstProtectedCommands.Find(id)) return FALSE;
//     return TRUE;
// DEVIATION: the last two tests are dropped -- OpenMFC's CUserToolsManager is
// 40 bytes (no +0x40 member; the export is always NULL) and
// ?m_lstProtectedCommands@ is an 8-byte placeholder, not a CList.  Retail
// reaches IsEditable through the vftable; here it is this inline (a derived
// override is not dispatched).
inline bool IsEditableInline(const CMFCToolBarButton* p) {
    const unsigned id = p->m_nID;
    if (id - 0xE110u <= 0xFu) return false;
    if (id - 0xF000u <= 0x1EFu) return false;
    if (static_cast<int>(id) >= 0xFF00) return false;
    if (id - 0xE210u <= 0xFu) return false;
    return true;
}

// CMFCToolBarButton::HasFocus() as the retail vftable slot 43 body reads
// (RVA 0x23920, mfc140u; GetHwnd -- slot 16, 0x1675d0 for this class -- is
// `m_pWndEdit ? m_pWndEdit->m_hWnd : NULL`):
//     HWND hwndBtn = GetHwnd();
//     return hwndBtn != NULL && (hwndBtn == ::GetFocus() || ::IsChild(hwndBtn, ::GetFocus()));
// Neither virtual is dispatched here (see the OnShowEditbox note in the file header).
inline bool HasFocusInline(const CMFCToolBarEditBoxButton* p) {
    const HWND hwndBtn = HWndOf(WndEdit(p));
    if (hwndBtn == nullptr) return false;
    if (hwndBtn == ::GetFocus()) return true;
    return ::IsChild(hwndBtn, ::GetFocus()) != 0;
}

// CObList walk (the pattern of featurepack/toolbar/CMFCToolBarComboBoxButton.cpp):
// a CObList built and torn down through the exported ctor/dtor so nothing here
// links a C++ method; the nodes a POSITION points at mirror
// CList<CObject*, CObject*>::CNode (include/openmfc/afx.h).
struct ObNode {
    ObNode*  pNext;
    ObNode*  pPrev;
    CObject* data;
};
static_assert(sizeof(CObList::POSITION) == sizeof(void*), "POSITION is one pointer");
inline ObNode* ObNodeFromPos(CObList::POSITION pos) {
    ObNode* p = nullptr;
    std::memcpy(&p, &pos, sizeof(p));
    return p;
}
class CommandButtonList {
public:
    explicit CommandButtonList(unsigned uiCmd) {
        std::memset(m_buf, 0, sizeof(m_buf));
        impl___0CObList__QEAA__J_Z(List(), 10);
        m_count = impl__GetCommandButtons_CMFCToolBar__SAHIAEAVCObList___Z(uiCmd, List());
    }
    ~CommandButtonList() { impl___1CObList__UEAA_XZ(List()); }
    int Count() const { return m_count; }
    ObNode* First() const { return ObNodeFromPos(impl__FindIndex_CObList__QEBAPEAU__POSITION____J_Z(List(), 0)); }
    CommandButtonList(const CommandButtonList&) = delete;
    CommandButtonList& operator=(const CommandButtonList&) = delete;
private:
    CObList* List() const { return reinterpret_cast<CObList*>(const_cast<unsigned char*>(m_buf)); }
    alignas(8) unsigned char m_buf[sizeof(CObList)];
    int m_count;
};
inline bool IsEditBoxButton(const CObject* p) {
    return p != nullptr &&
           impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(p, impl__GetThisClass_CMFCToolBarEditBoxButton__SAPEAUCRuntimeClass__XZ()) != 0;
}

// CArchive scalar / CString I/O through the exported Read / Write, in exactly
// the byte layout OpenMFC's own C++ CArchive::operator<< / >> use
// (core/runtime/CArchive.cpp: 4-byte int; CString = 4-byte length + UTF-16
// characters).  Retail inlines the int case as direct buffer stores
// (FillBuffer 0x1d1cc0 / Flush 0x1d1be0 on a short buffer) and the CString
// case as the outlined CStringT archive operators at 0x1b5e4 (>>) and 0x1b818
// (<<), which are not exported; OpenMFC's archive format is the one its
// base-class Serialize (tbb_Serialize) already writes, so this file matches it
// rather than retail's wire format.
inline void ArWriteInt(CArchive* ar, int v) { impl__Write_CArchive__QEAAXPEBXI_Z(ar, &v, sizeof v); }
inline int ArReadInt(CArchive* ar) { int v = 0; impl__Read_CArchive__QEAAIPEAXI_Z(ar, &v, sizeof v); return v; }
inline void ArWriteString(CArchive* ar, const CString& s) {
    const unsigned nLen = static_cast<unsigned>(s.GetLength());
    impl__Write_CArchive__QEAAXPEBXI_Z(ar, &nLen, sizeof nLen);
    if (nLen > 0) impl__Write_CArchive__QEAAXPEBXI_Z(ar, static_cast<const wchar_t*>(s), nLen * sizeof(wchar_t));
}
inline void ArReadString(CArchive* ar, CString& s) {
    unsigned nLen = 0;
    impl__Read_CArchive__QEAAIPEAXI_Z(ar, &nLen, sizeof nLen);
    if (nLen > 0 && nLen < 0x10000000u) {
        wchar_t* pBuf = s.GetBuffer(static_cast<int>(nLen) + 1);
        impl__Read_CArchive__QEAAIPEAXI_Z(ar, pBuf, nLen * sizeof(wchar_t));
        pBuf[nLen] = L'\0';
        s.ReleaseBuffer(static_cast<int>(nLen));
    } else {
        s.Empty();
    }
}

// The window object this button owns.  Retail tears it down with
//     pWnd->DestroyWindow();          // vftable +0xd0 (slot 26)
//     pWnd->`scalar deleting dtor`(1) // vftable +0x08 (slot 1)
// i.e. through the object's own MSVC vftable.  An object this file created
// (CreateEdit below) carries OpenMFC's mingw CWnd vftable, on which those
// slot numbers mean something else, so such an object is released through
// the exported ?DestroyWindow@CWnd@@ / ??1CWnd@@ / ??3@ instead; g_ownCWndVptr
// (the vptr ??0CWnd@@QEAA@XZ installs, recorded when the first one is built)
// tells the two apart.  Any other object -- one a derived class's CreateEdit
// override handed us, built against the real MFC headers -- is released
// exactly as retail does, through its own vftable slots.
void* g_ownCWndVptr = nullptr;
inline bool IsOwnCWnd(const CWnd* pWnd) {
    return g_ownCWndVptr != nullptr && *reinterpret_cast<void* const*>(pWnd) == g_ownCWndVptr;
}
inline void DestroyOwnedWindow(CWnd* pWnd) {
    if (pWnd == nullptr) return;
    if (IsOwnCWnd(pWnd)) {
        impl__DestroyWindow_CWnd__UEAAHXZ(pWnd);
        return;
    }
    using DestroyFn = int (MS_ABI*)(CWnd*);
    reinterpret_cast<DestroyFn>((*reinterpret_cast<void***>(pWnd))[26])(pWnd);
}
inline void DeleteOwnedWindow(CWnd* pWnd) {
    if (pWnd == nullptr) return;
    if (IsOwnCWnd(pWnd)) {
        impl___1CWnd__UEAA_XZ(pWnd);
        impl___3_YAXPEAX_Z(pWnd);
        return;
    }
    using DeletingDtorFn = void* (MS_ABI*)(CWnd*, unsigned int);
    reinterpret_cast<DeletingDtorFn>((*reinterpret_cast<void***>(pWnd))[1])(pWnd, 1u);
}

// CWnd::GetParent() as retail inlines it here: CWnd::FromHandle(::GetParent(m_hWnd)).
inline CWnd* ParentOf(const CWnd* pWnd) {
    return impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(::GetParent(pWnd->m_hWnd));
}

} // namespace

// ===========================================================================
// Construction / destruction
// ===========================================================================

// Retail ??0CMFCToolBarEditBoxButton@@QEAA@XZ (RVA 0x167690, mfc140u):
//     CMFCToolBarButton::CMFCToolBarButton();                    // 0x15bec0
//     vfptr = &CMFCToolBarEditBoxButton::`vftable' (0x317e80);
//     m_strContents = CString();                                 // the nil string, +0x98
//     m_dwStyle = 0x50000080;                                    // WS_CHILD|WS_VISIBLE|ES_AUTOHSCROLL, +0x8c
//     m_iWidth  = 150;                                           // +0x88
//     m_pWndEdit = NULL;  m_bHorz = TRUE;  m_bChangingText = FALSE;   // +0x90, +0xa4, +0xa0
//     m_bIsHotEdit = FALSE;  m_uiMenuResID = 0;                  // +0xa8 / +0xac (one 8-byte store)
// (the same stores ?Initialize@ makes, inlined; here they go through the
// Initialize thunk).  The (UINT, int, DWORD, int) constructor (RVA 0x167760)
// differs only in `CMFCToolBarButton(uiID, iImage, NULL, FALSE, FALSE)`
// (0x15bf40), `m_dwStyle = dwStyle | 0x50000000` and `m_iWidth = iWidth ?
// iWidth : 150`; it is declared inline in include/openmfc/afxmfc.h as a
// delegation to this constructor that DROPS all four arguments (see
// headerRequests).  The vfptr is left as mingw builds it.
CMFCToolBarEditBoxButton::CMFCToolBarEditBoxButton() {
    memset(_pad, 0, sizeof(_pad));
    m_bFlat = 0;          // the header's alias of m_iWidth
    new (&StrContents(this)) CString();
    Style(this) = kDefaultEditStyle;
    Width(this) = kDefaultWidth;
    impl__Initialize_CMFCToolBarEditBoxButton__IEAAXXZ(this);
}
// Retail ??1CMFCToolBarEditBoxButton@@UEAA@XZ (RVA 0x167840, mfc140u):
//     if (m_pWndEdit) { m_pWndEdit->DestroyWindow(); if (m_pWndEdit) delete m_pWndEdit; }   // vslots 0xd0, 0x08(1)
//     m_strContents.~CString();
//     CMFCToolBarButton::~CMFCToolBarButton();                   // 0x15c130
// (m_pWndEdit is not cleared by retail; it is here, the object is dying
// anyway.)  UnregisterEditButton drops any side-table entry
// OpenMFC_CopyEditButtonState (detail/CbarcoreSupport.cpp) may have made
// for this object.
CMFCToolBarEditBoxButton::~CMFCToolBarEditBoxButton() {
    if (CEdit* pEdit = WndEdit(this)) {
        DestroyOwnedWindow(pEdit);
        DeleteOwnedWindow(pEdit);
        WndEdit(this) = nullptr;
    }
    UnregisterEditButton(this);
    StrContents(this).~CString();
}

// Retail (RVA 0x167810, mfc140u), fully transcribed:
//     m_bHorz = TRUE;                      // +0xa4
//     m_pWndEdit = NULL;                   // +0x90
//     m_bChangingText = FALSE;             // +0xa0
//     m_bIsHotEdit = FALSE; m_uiMenuResID = 0;   // +0xa8 / +0xac (one 8-byte store)
// Symbol: ?Initialize@CMFCToolBarEditBoxButton@@IEAAXXZ
extern "C" void MS_ABI impl__Initialize_CMFCToolBarEditBoxButton__IEAAXXZ(CMFCToolBarEditBoxButton* pThis) {
    if (!pThis) return;
    Horz(pThis) = TRUE;
    WndEdit(pThis) = nullptr;
    ChangingText(pThis) = FALSE;
    IsHotEdit(pThis) = FALSE;
    MenuResID(pThis) = 0;
}

// ===========================================================================
// Contents
// ===========================================================================

// Retail (vftable slot 54, RVA 0x168690, mfc140u; the DLL export table maps
// ordinal 13067 there -- the rva-symbols JSON simply lacks the name), fully
// transcribed:
//     if (m_strContents != sContents) {                    // CStringT::Compare: ATLENSURE(sContents.GetString() != NULL)
//                                                          //   -> the call at 0x16870a throws AfxThrowOleException(E_FAIL)
//                                                          //   (helper 0x333c); then wcscmp (IAT 0x2c7770) on the two buffers, +0x98
//         m_strContents = sContents;                       // 0xde30 (CSimpleStringT::operator=)
//         if (m_pWndEdit != NULL) {
//             m_bChangingText = TRUE;                      // +0xa0
//             m_pWndEdit->SetWindowText(m_strContents);    // 0x2a9790
//             m_bChangingText = FALSE;
//         }
//     }
// DEVIATION: a CString whose buffer pointer is NULL (never the case for a
// live CStringT) makes this body return where retail throws E_FAIL.
// Symbol: ?SetContents@CMFCToolBarEditBoxButton@@UEAAXAEBV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@@Z
extern "C" void MS_ABI impl__SetContents_CMFCToolBarEditBoxButton__UEAAXAEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
    CMFCToolBarEditBoxButton* pThis, const CString* pContents)
{
    if (!pThis || !pContents) return;
    const wchar_t* lpszNew = static_cast<const wchar_t*>(*pContents);
    if (lpszNew == nullptr) return;
    if (wcscmp(static_cast<const wchar_t*>(StrContents(pThis)), lpszNew) == 0) return;
    StrContents(pThis) = *pContents;
    if (CEdit* pEdit = WndEdit(pThis)) {
        ChangingText(pThis) = TRUE;
        impl__SetWindowTextW_CWnd__QEAAXPEB_W_Z(pEdit, static_cast<const wchar_t*>(StrContents(pThis)));
        ChangingText(pThis) = FALSE;
    }
}
// The C++ entry the header declares routes to the export body (no virtual
// dispatch on this object: a client-built object carries the MSVC vftable).
void CMFCToolBarEditBoxButton::SetContents(const CString& sContents) {
    impl__SetContents_CMFCToolBarEditBoxButton__UEAAXAEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(this, &sContents);
}

// Retail (RVA 0x168790, mfc140u), fully transcribed:
//     CObList lst;
//     CMFCToolBarEditBoxButton* p = NULL;
//     if (CMFCToolBar::GetCommandButtons(uiCmd, lst) > 0 && lst.GetHeadPosition() != NULL) {   // 0x152990
//         p = (CMFCToolBarEditBoxButton*)lst.GetHead();
//         if (p == NULL) AfxThrowInvalidArgException();                                          // 0x227720
//         if (!p->IsKindOf(RUNTIME_CLASS(CMFCToolBarEditBoxButton))) AfxThrowInvalidArgException();   // 0x234cf0, RTC 0x3b16b8
//     }
//     return p;
// DEVIATION: the two exception paths return NULL instead of throwing (the
// invalid-argument exception is not raised from this file).
// Symbol: ?GetByCmd@CMFCToolBarEditBoxButton@@SAPEAV1@I@Z
extern "C" CMFCToolBarEditBoxButton* MS_ABI impl__GetByCmd_CMFCToolBarEditBoxButton__SAPEAV1_I_Z(unsigned int uiCmd) {
    CommandButtonList lst(uiCmd);
    if (lst.Count() <= 0) return nullptr;
    ObNode* pHead = lst.First();
    if (pHead == nullptr) return nullptr;
    CObject* pObj = pHead->data;
    if (!IsEditBoxButton(pObj)) return nullptr;
    return static_cast<CMFCToolBarEditBoxButton*>(pObj);
}
CMFCToolBarEditBoxButton* CMFCToolBarEditBoxButton::GetByCmd(UINT uiCmd) {
    return impl__GetByCmd_CMFCToolBarEditBoxButton__SAPEAV1_I_Z(uiCmd);
}

// Retail (RVA 0x168870, mfc140u), fully transcribed.  The CString comes back
// through the hidden pointer in RCX (uiCmd in EDX):
//     CString strContents;                                   // the nil string
//     CMFCToolBarEditBoxButton* p = GetByCmd(uiCmd);          // 0x168790
//     if (p != NULL) p->m_pWndEdit->GetWindowText(strContents);   // 0x28be00; m_pWndEdit dereferenced unconditionally
//     return strContents;
// Note that retail reads the live window text, not m_strContents; a button
// whose edit exists but has no HWND yields "" (CWnd::GetWindowText on a NULL
// m_hWnd empties the string).
// DEVIATION: a button without an edit object yields "" where retail would fault.
// Symbol: ?GetContentsAll@CMFCToolBarEditBoxButton@@SA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@I@Z
extern "C" CString* MS_ABI impl__GetContentsAll_CMFCToolBarEditBoxButton__SA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__I_Z(CString* pRet, unsigned int uiCmd) {
    if (!pRet) return pRet;
    new (pRet) CString();
    CMFCToolBarEditBoxButton* pButton = impl__GetByCmd_CMFCToolBarEditBoxButton__SAPEAV1_I_Z(uiCmd);
    if (pButton != nullptr) {
        if (CEdit* pEdit = WndEdit(pButton)) {
            impl__GetWindowTextW_CWnd__QEBAXAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(pEdit, pRet);
        }
    }
    return pRet;
}
CString CMFCToolBarEditBoxButton::GetContentsAll(UINT uiCmd) {
    alignas(CString) unsigned char buf[sizeof(CString)];
    CString* pRet = impl__GetContentsAll_CMFCToolBarEditBoxButton__SA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__I_Z(
        reinterpret_cast<CString*>(buf), uiCmd);
    CString result(*pRet);
    pRet->~CString();
    return result;
}

// Retail (RVA 0x168820, mfc140u -- unnamed in the rva-symbols JSON; it is the
// body that follows GetByCmd, and the DLL export table maps ordinal 13068,
// ?SetContentsAll@..., to exactly this RVA), fully transcribed:
//     CMFCToolBarEditBoxButton* p = GetByCmd(uiCmd);          // 0x168790
//     if (p != NULL) p->SetContents(strContents);             // vslot 0x1b0 (54)
//     return p != NULL;
// SetContents is reached through this file's thunk rather than the vftable.
// Symbol: ?SetContentsAll@CMFCToolBarEditBoxButton@@SAHIAEBV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@@Z
extern "C" int MS_ABI impl__SetContentsAll_CMFCToolBarEditBoxButton__SAHIAEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(unsigned int uiCmd, const CString* pContents) {
    CMFCToolBarEditBoxButton* pButton = impl__GetByCmd_CMFCToolBarEditBoxButton__SAPEAV1_I_Z(uiCmd);
    if (pButton == nullptr) return 0;
    impl__SetContents_CMFCToolBarEditBoxButton__UEAAXAEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(pButton, pContents);
    return 1;
}
BOOL CMFCToolBarEditBoxButton::SetContentsAll(UINT uiCmd, const CString& sContents) {
    return impl__SetContentsAll_CMFCToolBarEditBoxButton__SAHIAEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(uiCmd, &sContents);
}

// ===========================================================================
// Copy / serialization / customization
// ===========================================================================

// Retail (RVA 0x1678d0, mfc140u), fully transcribed:
//     CMFCToolBarButton::CopyFrom(src);                          // 0x15c1a0 (real in OpenMFC)
//     m_dwStyle = src.m_dwStyle;  m_iWidth = src.m_iWidth;       // +0x8c, +0x88
//     m_strContents = src.m_strContents;                          // 0xde30
//     m_uiMenuResID = src.m_uiMenuResID;                          // +0xac
// (m_pWndEdit, m_bHorz, m_bChangingText and m_bIsHotEdit are NOT copied.)
// Symbol: ?CopyFrom@CMFCToolBarEditBoxButton@@UEAAXAEBVCMFCToolBarButton@@@Z
extern "C" void MS_ABI impl__CopyFrom_CMFCToolBarEditBoxButton__UEAAXAEBVCMFCToolBarButton___Z(
    CMFCToolBarEditBoxButton* pThis, const CMFCToolBarButton* pSrc)
{
    if (!pThis || !pSrc) return;
    impl__CopyFrom_CMFCToolBarButton__UEAAXAEBV1__Z(pThis, pSrc);
    const CMFCToolBarEditBoxButton* pSrcEdit = static_cast<const CMFCToolBarEditBoxButton*>(pSrc);
    Style(pThis) = Style(pSrcEdit);
    Width(pThis) = Width(pSrcEdit);
    StrContents(pThis) = StrContents(pSrcEdit);
    MenuResID(pThis) = MenuResID(pSrcEdit);
}

// Retail (vftable slot 2, RVA 0x167930, mfc140u), fully transcribed.  The int
// reads / writes are CArchive's inline buffer accesses, the CString ones the
// outlined CStringT archive operators (see ArWriteInt / ArWriteString):
//     CMFCToolBarButton::Serialize(ar);                          // 0x15c240
//     if (ar.IsLoading()) {
//         ar >> m_iWidth;  m_rect.right = m_rect.left + m_iWidth;
//         ar >> m_dwStyle;  ar >> m_strContents;  ar >> m_uiMenuResID;
//     } else {
//         ar << m_iWidth;  ar << m_dwStyle;
//         if (m_pWndEdit != NULL) m_pWndEdit->GetWindowText(m_strContents);   // 0x28be00
//         else                    m_strContents.Empty();                       // 0x33b0
//         ar << m_strContents;  ar << m_uiMenuResID;
//     }
// The scalars and the string use OpenMFC's own archive byte layout.
// Symbol: ?Serialize@CMFCToolBarEditBoxButton@@UEAAXAEAVCArchive@@@Z
extern "C" void MS_ABI impl__Serialize_CMFCToolBarEditBoxButton__UEAAXAEAVCArchive___Z(CMFCToolBarEditBoxButton* pThis, CArchive* ar) {
    if (!pThis || !ar) return;
    impl__Serialize_CMFCToolBarButton__UEAAXAEAVCArchive___Z(pThis, ar);
    if (ar->IsLoading()) {
        Width(pThis) = ArReadInt(ar);
        pThis->m_rect.right = pThis->m_rect.left + Width(pThis);
        Style(pThis) = static_cast<DWORD>(ArReadInt(ar));
        ArReadString(ar, StrContents(pThis));
        MenuResID(pThis) = static_cast<unsigned>(ArReadInt(ar));
    } else {
        ArWriteInt(ar, Width(pThis));
        ArWriteInt(ar, static_cast<int>(Style(pThis)));
        if (CEdit* pEdit = WndEdit(pThis)) {
            impl__GetWindowTextW_CWnd__QEBAXAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(pEdit, &StrContents(pThis));
        } else {
            StrContents(pThis).Empty();
        }
        ArWriteString(ar, StrContents(pThis));
        ArWriteInt(ar, static_cast<int>(MenuResID(pThis)));
    }
}

// Retail (vftable slot 19, RVA 0x163440, mfc140u -- the body is byte-identical
// to ?OnAddToCustomizePage@CMFCToolBarComboBoxButton@@ and was folded onto it,
// which is why the ordinal map carries only the combo-box name), fully transcribed:
//     CObList lst;
//     if (CMFCToolBar::GetCommandButtons(m_nID, lst) != 0)                 // 0x152990
//         CopyFrom(*(CMFCToolBarButton*)lst.GetHead());                    // vslot 0x38 (7); the head node is not NULL-checked
// CopyFrom is reached through this file's thunk rather than the vftable.
// DEVIATION: a missing head node or a NULL head element returns here where
// retail would hand CopyFrom a null reference.
// Symbol: ?OnAddToCustomizePage@CMFCToolBarEditBoxButton@@UEAAXXZ
extern "C" void MS_ABI impl__OnAddToCustomizePage_CMFCToolBarEditBoxButton__UEAAXXZ(CMFCToolBarEditBoxButton* pThis) {
    if (!pThis) return;
    CommandButtonList lst(pThis->m_nID);
    if (lst.Count() == 0) return;
    ObNode* pHead = lst.First();
    if (pHead == nullptr || pHead->data == nullptr) return;
    impl__CopyFrom_CMFCToolBarEditBoxButton__UEAAXAEBVCMFCToolBarButton___Z(pThis, static_cast<const CMFCToolBarButton*>(pHead->data));
}

// ===========================================================================
// Layout
// ===========================================================================

// Retail (RVA 0x167b30, mfc140u), fully transcribed.  SIZE comes back through
// the hidden pointer in RDX (pDC in R8, sizeDefault in R9, bHorz on the stack):
//     if (!m_bVisible) {                                              // +0x64
//         if (m_pWndEdit->GetSafeHwnd()) m_pWndEdit->ShowWindow(SW_HIDE);   // 0x2a9ad0
//         OnShowEditbox(FALSE);                                       // vslot 0x1c0 (56)
//         return CSize(0, 0);
//     }
//     m_bHorz = bHorz;                                                // +0xa4
//     if (bHorz) {
//         if (m_pWndEdit->GetSafeHwnd() && !m_bIsHidden) {            // +0x54
//             m_pWndEdit->ShowWindow(SW_SHOWNOACTIVATE);
//             OnShowEditbox(TRUE);
//         }
//         if (m_bTextBelow && !m_strText.IsEmpty()) {                 // +0x1c, +0x38
//             CRect rectText(0, 0, m_iWidth, sizeDefault.cy);
//             pDC->DrawText(m_strText, rectText, DT_CALCRECT | DT_CENTER | DT_WORDBREAK);   // CDC vslot 0xe0 (28)
//             m_sizeText = rectText.Size();                           // +0x78
//         } else m_sizeText = CSize(0, 0);
//         return CSize(m_iWidth, m_sizeText.cy + sizeDefault.cy);
//     }
//     if (m_pWndEdit->GetSafeHwnd()) m_pWndEdit->ShowWindow(SW_HIDE);
//     OnShowEditbox(FALSE);
//     m_sizeText = CSize(0, 0);
//     return CMFCToolBarButton::OnCalculateSize(pDC, sizeDefault, FALSE);   // 0x15d680
// DEVIATIONS: OnShowEditbox is the base inline no-op and is not dispatched
// (see the file header); the base OnCalculateSize is a null stub in OpenMFC
// (featurepack/toolbar/CMFCToolBarButton.cpp) that does not write the result,
// so the vertical case yields the (0, 0) pre-set here; DrawText goes through
// the exported CDC thunk, and a NULL pDC (retail would fault) leaves
// m_sizeText at (0, 0).
// Symbol: ?OnCalculateSize@CMFCToolBarEditBoxButton@@UEAA?AUtagSIZE@@PEAVCDC@@AEBVCSize@@H@Z
extern "C" SIZE* MS_ABI impl__OnCalculateSize_CMFCToolBarEditBoxButton__UEAA_AUtagSIZE__PEAVCDC__AEBVCSize__H_Z(
    CMFCToolBarEditBoxButton* pThis, SIZE* pRet, CDC* pDC, const SIZE* pSizeDefault, int bHorz)
{
    if (!pRet) return pRet;
    pRet->cx = 0; pRet->cy = 0;
    if (!pThis || !pSizeDefault) return pRet;

    CEdit* pEdit = WndEdit(pThis);
    if (pThis->m_bVisible == 0) {
        if (HasWindow(pEdit)) impl__ShowWindow_CWnd__QEAAHH_Z(pEdit, SW_HIDE);
        return pRet;
    }
    Horz(pThis) = bHorz;
    if (bHorz != 0) {
        if (HasWindow(pEdit) && pThis->m_bIsHidden == 0) {
            impl__ShowWindow_CWnd__QEAAHH_Z(pEdit, SW_SHOWNOACTIVATE);
        }
        if (pThis->m_bTextBelow != 0 && pThis->m_strText.GetLength() != 0 && pDC != nullptr) {
            RECT rectText = { 0, 0, Width(pThis), pSizeDefault->cy };
            impl__DrawTextW_CDC__QEAAHPEB_WHPEAUTAGRECT__I_Z(pDC, static_cast<const wchar_t*>(pThis->m_strText),
                                                             pThis->m_strText.GetLength(), &rectText, kDtCalcLabel);
            pThis->m_sizeText.cx = rectText.right - rectText.left;
            pThis->m_sizeText.cy = rectText.bottom - rectText.top;
        } else {
            pThis->m_sizeText.cx = 0;
            pThis->m_sizeText.cy = 0;
        }
        pRet->cx = Width(pThis);
        pRet->cy = pThis->m_sizeText.cy + pSizeDefault->cy;
        return pRet;
    }
    if (HasWindow(pEdit)) impl__ShowWindow_CWnd__QEAAHH_Z(pEdit, SW_HIDE);
    pThis->m_sizeText.cx = 0;
    pThis->m_sizeText.cy = 0;
    impl__OnCalculateSize_CMFCToolBarButton__UEAA_AUtagSIZE__PEAVCDC__AEBVCSize__H_Z(pThis, pRet, pDC, pSizeDefault, 0);
    return pRet;
}

// Retail (vftable slot 14, RVA 0x167cc0, mfc140u), fully transcribed:
//     if (m_pWndEdit->GetSafeHwnd() == NULL || !(m_pWndEdit->GetStyle() & WS_VISIBLE)) return;   // 0x2a9690
//     int nHeight = afxGlobalData.GetTextHeight(TRUE);                       // +0x2a8, behind the init gate
//     int dy = (m_rect.Height() - m_sizeText.cy - nHeight) / 2;              // (+0x74 - +0x7c - +0x6c - nHeight) / 2, signed
//     if (dy < 0) dy = 0;
//     m_pWndEdit->SetWindowPos(NULL, m_rect.left + 3, m_rect.top + dy, m_rect.Width() - 6, nHeight,
//                              SWP_NOZORDER | SWP_NOACTIVATE);                // 0x2a9a60
//     m_pWndEdit->SendMessage(EM_SETSEL, (WPARAM)-1, 0);                     // ::SendMessageW
//     m_pWndEdit->SendMessage(EM_SCROLLCARET, 0, 0);
// Symbol: ?OnMove@CMFCToolBarEditBoxButton@@UEAAXXZ
extern "C" void MS_ABI impl__OnMove_CMFCToolBarEditBoxButton__UEAAXXZ(CMFCToolBarEditBoxButton* pThis) {
    if (!pThis) return;
    CEdit* pEdit = WndEdit(pThis);
    if (!HasWindow(pEdit) || !StyleVisible(pEdit)) return;
    const int nHeight = GlobalTextHeightHorz();
    const CRect& rect = pThis->m_rect;
    int dy = ((rect.bottom - rect.top) - pThis->m_sizeText.cy - nHeight) / 2;
    if (dy < 0) dy = 0;
    impl__SetWindowPos_CWnd__QEAAHPEBV1_HHHHI_Z(pEdit, nullptr, rect.left + 3, rect.top + dy,
                                                (rect.right - rect.left) - 6, nHeight, kSwpNoZorderNoActivate);
    ::SendMessage(pEdit->m_hWnd, kEmSetSel, static_cast<WPARAM>(-1), 0);
    ::SendMessage(pEdit->m_hWnd, kEmScrollCaret, 0, 0);
}

// Retail (vftable slot 15, RVA 0x167da0, mfc140u), fully transcribed:
//     m_iWidth = iSize;                              // +0x88
//     m_rect.right = m_rect.left + iSize;            // +0x70 = +0x68 + iSize
//     OnMove();                                      // vslot 0x70 (14), a tail jump
// OnMove is reached through this file's thunk rather than the vftable.
// Symbol: ?OnSize@CMFCToolBarEditBoxButton@@UEAAXH@Z
extern "C" void MS_ABI impl__OnSize_CMFCToolBarEditBoxButton__UEAAXH_Z(CMFCToolBarEditBoxButton* pThis, int iSize) {
    if (!pThis) return;
    Width(pThis) = iSize;
    pThis->m_rect.right = pThis->m_rect.left + iSize;
    impl__OnMove_CMFCToolBarEditBoxButton__UEAAXXZ(pThis);
}

// Retail (vftable slot 35, RVA 0x168620, mfc140u), fully transcribed:
//     if (m_pWndEdit->GetSafeHwnd() == NULL) return;
//     if (bShow) { m_pWndEdit->ShowWindow(SW_SHOWNOACTIVATE); OnMove(); }   // 0x2a9ad0, vslot 0x70 (14)
//     else         m_pWndEdit->ShowWindow(SW_HIDE);
//     OnShowEditbox(bShow);                                                 // vslot 0x1c0 (56)
// DEVIATION: OnShowEditbox is the base inline no-op and is not dispatched
// (see the file header); OnMove goes through this file's thunk.
// Symbol: ?OnShow@CMFCToolBarEditBoxButton@@UEAAXH@Z
extern "C" void MS_ABI impl__OnShow_CMFCToolBarEditBoxButton__UEAAXH_Z(CMFCToolBarEditBoxButton* pThis, int bShow) {
    if (!pThis) return;
    CEdit* pEdit = WndEdit(pThis);
    if (!HasWindow(pEdit)) return;
    if (bShow != 0) {
        impl__ShowWindow_CWnd__QEAAHH_Z(pEdit, SW_SHOWNOACTIVATE);
        impl__OnMove_CMFCToolBarEditBoxButton__UEAAXXZ(pThis);
    } else {
        impl__ShowWindow_CWnd__QEAAHH_Z(pEdit, SW_HIDE);
    }
}

// Retail (vftable slot 36, RVA 0x168710, mfc140u), fully transcribed.  The
// CRect comes back through the hidden pointer in RDX:
//     if (m_bTextBelow && m_bHorz && !m_strText.IsEmpty()) {          // +0x1c, +0xa4, +0x38
//         rect.left   = (m_rect.left - m_sizeText.cx + m_rect.right) / 2;   // signed halves
//         rect.top    = m_rect.top;
//         rect.right  = (m_rect.left + m_sizeText.cx + m_rect.right) / 2;
//         rect.bottom = m_rect.top + m_rect.bottom + m_sizeText.cy;   // exactly what the add/lea at 0x168745..0x16874d inside GetInvalidateRect (0x168710) compute
//     } else rect = m_rect;
//     return rect;
// Symbol: ?GetInvalidateRect@CMFCToolBarEditBoxButton@@UEBA?BVCRect@@XZ
extern "C" RECT* MS_ABI impl__GetInvalidateRect_CMFCToolBarEditBoxButton__UEBA_BVCRect__XZ(const CMFCToolBarEditBoxButton* pThis, RECT* pRet) {
    if (!pRet) return pRet;
    if (!pThis) { pRet->left = pRet->top = pRet->right = pRet->bottom = 0; return pRet; }
    const CRect& r = pThis->m_rect;
    if (pThis->m_bTextBelow != 0 && Horz(pThis) != 0 && pThis->m_strText.GetLength() != 0) {
        const int cx = pThis->m_sizeText.cx;
        const int cy = pThis->m_sizeText.cy;
        pRet->left   = (r.left - cx + r.right) / 2;
        pRet->top    = r.top;
        pRet->right  = (r.left + cx + r.right) / 2;
        pRet->bottom = r.top + r.bottom + cy;
    } else {
        pRet->left = r.left; pRet->top = r.top; pRet->right = r.right; pRet->bottom = r.bottom;
    }
    return pRet;
}

// Retail (RVA 0x1683d0, mfc140u), fully transcribed:
//     if (m_pWndEdit == NULL || m_pWndEdit->m_hWnd == NULL) AfxThrowInvalidArgException();   // 0x227720
//     ::GetWindowRect(m_pWndEdit->m_hWnd, &rectBorder);
//     m_pWndEdit->GetParent()->ScreenToClient(&rectBorder);   // CWnd::FromHandle(::GetParent()) 0x28ad70, then 0x2a32b0 (RTL-aware) on it, NULL not checked
//     ::InflateRect(&rectBorder, 1, 1);
// DEVIATIONS: a missing edit window returns with rectBorder untouched instead
// of throwing; a NULL parent skips the ScreenToClient where retail would fault.
// Symbol: ?GetEditBorder@CMFCToolBarEditBoxButton@@UEAAXAEAVCRect@@@Z
extern "C" void MS_ABI impl__GetEditBorder_CMFCToolBarEditBoxButton__UEAAXAEAVCRect___Z(
    CMFCToolBarEditBoxButton* pThis, RECT* pRectBorder)
{
    if (!pThis || !pRectBorder) return;
    CEdit* pEdit = WndEdit(pThis);
    if (!HasWindow(pEdit)) return;
    ::GetWindowRect(pEdit->m_hWnd, pRectBorder);
    if (CWnd* pParent = ParentOf(pEdit)) {
        impl__ScreenToClient_CWnd__QEBAXPEAUtagRECT___Z(pParent, pRectBorder);
    }
    ::InflateRect(pRectBorder, 1, 1);
}

// ===========================================================================
// Window creation / parent changes
// ===========================================================================

// Retail (RVA 0x168590, mfc140u), fully transcribed:
//     CMFCToolBarEditCtrl* pWndEdit = new CMFCToolBarEditCtrl(*this);   // 0x160 bytes (0x27f0), ctor 0x168ba0
//     if (!pWndEdit->Create(m_dwStyle, rect, pWndParent, m_nID)) {      // 0x294320 (CEdit::Create), called even when new returned NULL
//         if (pWndEdit) delete pWndEdit;                                 // vslot 0x08 (1)
//         return NULL;
//     }
//     return pWndEdit;
// DEVIATION: the object is built through ??2@YAPEAX_K@Z + the exported
// ??0CWnd@@QEAA@XZ (see the file header: CMFCToolBarEditCtrl's exported
// constructor is a null stub, and `new CEdit` would drag C++ CWnd machinery
// into this translation unit), so its runtime class reads CWnd and its
// vftable is OpenMFC's CWnd one; Create goes through the exported
// CEdit::Create; an allocation failure returns NULL where retail would fault.
// Symbol: ?CreateEdit@CMFCToolBarEditBoxButton@@UEAAPEAVCEdit@@PEAVCWnd@@AEBVCRect@@@Z
extern "C" CEdit* MS_ABI impl__CreateEdit_CMFCToolBarEditBoxButton__UEAAPEAVCEdit__PEAVCWnd__AEBVCRect___Z(
    CMFCToolBarEditBoxButton* pThis, CWnd* pWndParent, const RECT* pRect)
{
    if (!pThis || !pRect) return nullptr;
    void* pStorage = impl___2_YAPEAX_K_Z(sizeof(CEdit));
    if (!pStorage) return nullptr;
    std::memset(pStorage, 0, sizeof(CEdit));
    CEdit* pWndEdit = static_cast<CEdit*>(impl___0CWnd__QEAA_XZ(pStorage));
    if (g_ownCWndVptr == nullptr) g_ownCWndVptr = *reinterpret_cast<void**>(pWndEdit);
    if (!impl__Create_CEdit__QEAAHKAEBUtagRECT__PEAVCWnd__I_Z(pWndEdit, Style(pThis), pRect, pWndParent, pThis->m_nID)) {
        DeleteOwnedWindow(pWndEdit);
        return nullptr;
    }
    return pWndEdit;
}

// Retail (RVA 0x167dc0, mfc140u), fully transcribed:
//     CMFCToolBarButton::OnChangeParentWnd(pWndParent);                       // 0x15e570
//     if (m_pWndEdit->GetSafeHwnd() != NULL) {
//         CWnd* pOldParent = m_pWndEdit->GetParent();  (AfxThrowInvalidArgException if NULL, the call at 0x16804e)
//         if (pWndParent != NULL && pOldParent->m_hWnd == pWndParent->m_hWnd) return;
//         m_pWndEdit->GetWindowText(m_strContents);                          // 0x28be00
//         m_pWndEdit->DestroyWindow();  if (m_pWndEdit) delete m_pWndEdit;   // vslots 0xd0, 0x08(1)
//         m_pWndEdit = NULL;
//     }
//     if (pWndParent == NULL || pWndParent->m_hWnd == NULL) return;
//     CRect rect = m_rect;  ::InflateRect(&rect, -3, -1);
//     rect.bottom = rect.top + afxGlobalData.GetTextHeight(TRUE);            // +0x2a8, behind the init gate
//     m_pWndEdit = CreateEdit(pWndParent, rect);                              // vslot 0x1a8 (53)
//     if (m_pWndEdit == NULL) return;
//     OnMove();                                                               // vslot 0x70 (14)
//     m_pWndEdit->SendMessage(WM_SETFONT, afxGlobalData.fontRegular /*+0x1b0*/, TRUE);   // ::SendMessageW
//     CString strText;  m_pWndEdit->GetWindowText(strText);
//     if (strText.IsEmpty()) {
//         m_bChangingText = TRUE;                                             // +0xa0
//         ENSURE(m_pWndEdit != NULL);                                         // the call at 0x168048
//         m_pWndEdit->SetWindowText(m_strContents);   // inline: ::IsWindow check, COleControlSite path at +0xd0, else ::SetWindowTextW
//         m_bChangingText = FALSE;
//     } else {
//         m_strContents = strText;                                            // 0xde30
//     }
// DEVIATIONS: the base OnChangeParentWnd is a null stub in OpenMFC (so
// m_pWndParent is not recorded here); an edit whose parent cannot be resolved
// is torn down and recreated where retail throws; CreateEdit / OnMove are
// reached through this file's thunks instead of the vftable; SetWindowText
// goes through the exported CWnd thunk (no control-site path).
// Symbol: ?OnChangeParentWnd@CMFCToolBarEditBoxButton@@UEAAXPEAVCWnd@@@Z
extern "C" void MS_ABI impl__OnChangeParentWnd_CMFCToolBarEditBoxButton__UEAAXPEAVCWnd___Z(CMFCToolBarEditBoxButton* pThis, CWnd* pWndParent) {
    if (!pThis) return;
    impl__OnChangeParentWnd_CMFCToolBarButton__UEAAXPEAVCWnd___Z(pThis, pWndParent);

    if (HasWindow(WndEdit(pThis))) {
        CWnd* pOldParent = ParentOf(WndEdit(pThis));
        if (pOldParent != nullptr && pWndParent != nullptr && pOldParent->m_hWnd == pWndParent->m_hWnd) return;
        impl__GetWindowTextW_CWnd__QEBAXAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(WndEdit(pThis), &StrContents(pThis));
        DestroyOwnedWindow(WndEdit(pThis));
        DeleteOwnedWindow(WndEdit(pThis));
        WndEdit(pThis) = nullptr;
    }
    if (!HasWindow(pWndParent)) return;

    RECT rect = { pThis->m_rect.left, pThis->m_rect.top, pThis->m_rect.right, pThis->m_rect.bottom };
    ::InflateRect(&rect, -3, -1);
    rect.bottom = rect.top + GlobalTextHeightHorz();

    CEdit* pEdit = impl__CreateEdit_CMFCToolBarEditBoxButton__UEAAPEAVCEdit__PEAVCWnd__AEBVCRect___Z(pThis, pWndParent, &rect);
    WndEdit(pThis) = pEdit;
    if (pEdit == nullptr) return;
    impl__OnMove_CMFCToolBarEditBoxButton__UEAAXXZ(pThis);
    ::SendMessage(pEdit->m_hWnd, WM_SETFONT, reinterpret_cast<WPARAM>(GlobalRegularFont()), TRUE);

    CString strText;
    impl__GetWindowTextW_CWnd__QEBAXAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(pEdit, &strText);
    if (strText.GetLength() == 0) {
        ChangingText(pThis) = TRUE;
        impl__SetWindowTextW_CWnd__QEAAXPEB_W_Z(pEdit, static_cast<const wchar_t*>(StrContents(pThis)));
        ChangingText(pThis) = FALSE;
    } else {
        StrContents(pThis) = strText;
    }
}

// Retail (vftable slot 25, RVA 0x168ac0, mfc140u), fully transcribed:
//     if (m_pWndEdit->GetSafeHwnd()) m_pWndEdit->SendMessage(WM_SETFONT, afxGlobalData.fontRegular /*+0x1b0*/, TRUE);
// (preceded by the afxGlobalData init gate; the font read is GlobalRegularFont above)
// Symbol: ?OnGlobalFontsChanged@CMFCToolBarEditBoxButton@@UEAAXXZ
extern "C" void MS_ABI impl__OnGlobalFontsChanged_CMFCToolBarEditBoxButton__UEAAXXZ(CMFCToolBarEditBoxButton* pThis) {
    if (!pThis) return;
    if (CEdit* pEdit = WndEdit(pThis); HasWindow(pEdit)) {
        ::SendMessage(pEdit->m_hWnd, WM_SETFONT, reinterpret_cast<WPARAM>(GlobalRegularFont()), TRUE);
    }
}

// Retail (vftable slot 37, RVA 0x1688e0, mfc140u), fully transcribed:
//     m_nStyle = nStyle;                                                   // +0x28 (the base inline)
//     if (m_pWndEdit == NULL || m_pWndEdit->m_hWnd == NULL) return;
//     BOOL bEnable;
//     if (CMFCToolBar::m_bCustomizeMode /*0x3be35c*/ && !IsEditable())   // vslot 0xd0 (26)
//         bEnable = FALSE;
//     else if (CMFCToolBar::m_bCustomizeMode)
//         bEnable = TRUE;
//     else
//         bEnable = !(m_nStyle & TBBS_DISABLED);
//     m_pWndEdit->EnableWindow(bEnable);                                   // 0x2a9b30
// IsEditable is the inlined base body (IsEditableInline above).
// Symbol: ?SetStyle@CMFCToolBarEditBoxButton@@UEAAXI@Z
extern "C" void MS_ABI impl__SetStyle_CMFCToolBarEditBoxButton__UEAAXI_Z(CMFCToolBarEditBoxButton* pThis, unsigned int nStyle) {
    if (!pThis) return;
    pThis->m_nStyle = nStyle;
    CEdit* pEdit = WndEdit(pThis);
    if (!HasWindow(pEdit)) return;
    int bEnable;
    if (impl__m_bCustomizeMode_CMFCToolBar__1HA != 0 && !IsEditableInline(pThis)) bEnable = 0;
    else if (impl__m_bCustomizeMode_CMFCToolBar__1HA != 0) bEnable = 1;
    else bEnable = ((pThis->m_nStyle & kTbbsDisabled) == 0) ? 1 : 0;
    impl__EnableWindow_CWnd__QEAAHH_Z(pEdit, bEnable);
}

// Retail (RVA 0x168950, mfc140u; see the file header for how it was located),
// fully transcribed:
//     if (m_bIsHotEdit == bHot) return;
//     m_bIsHotEdit = bHot;                                                       // +0xa8
//     if (CWnd* pParent = m_pWndEdit->GetParent()) {   (m_pWndEdit dereferenced unconditionally)
//         ::InvalidateRect(m_pWndEdit->GetParent()->m_hWnd, &m_rect, TRUE);       // +0x68
//         ::UpdateWindow(m_pWndEdit->GetParent()->m_hWnd);
//     }
// DEVIATION: a NULL m_pWndEdit returns after the assignment where retail would fault.
// Symbol: ?SetHotEdit@CMFCToolBarEditBoxButton@@IEAAXH@Z
extern "C" void MS_ABI impl__SetHotEdit_CMFCToolBarEditBoxButton__IEAAXH_Z(CMFCToolBarEditBoxButton* pThis, int bHot) {
    if (!pThis) return;
    if (IsHotEdit(pThis) == bHot) return;
    IsHotEdit(pThis) = bHot;
    CEdit* pEdit = WndEdit(pThis);
    if (pEdit == nullptr) return;
    CWnd* pParent = ParentOf(pEdit);
    if (pParent == nullptr) return;
    ::InvalidateRect(ParentOf(pEdit)->m_hWnd, static_cast<const RECT*>(pThis->m_rect), TRUE);
    ::UpdateWindow(ParentOf(pEdit)->m_hWnd);
}

// ===========================================================================
// Painting
// ===========================================================================

// Retail (vftable slot 20, RVA 0x1634d0, mfc140u -- byte-identical to
// ?OnCtlColor@CMFCToolBarComboBoxButton@@ and folded onto it), fully transcribed:
//     pDC->SetTextColor(afxGlobalData.clrWindowText /*+0x7c*/);   // CDC vslot 0x70 (14)
//     pDC->SetBkColor(afxGlobalData.clrWindow /*+0x78*/);         // CDC vslot 0x68 (13)
//     return afxGlobalData.brWindow.m_hObject /*+0x110*/;
// (nCtlColor is not read; each global read is behind the init gate.)  The two
// CDC virtuals go through their exported thunks.
// Symbol: ?OnCtlColor@CMFCToolBarEditBoxButton@@UEAAPEAUHBRUSH__@@PEAVCDC@@I@Z
extern "C" HBRUSH MS_ABI impl__OnCtlColor_CMFCToolBarEditBoxButton__UEAAPEAUHBRUSH____PEAVCDC__I_Z(
    CMFCToolBarEditBoxButton* pThis, CDC* pDC, unsigned int nCtlColor)
{
    (void)pThis; (void)nCtlColor;
    if (pDC != nullptr) {
        impl__SetTextColor_CDC__UEAAKK_Z(pDC, GlobalColor(kGlobalDataClrWindowText, COLOR_WINDOWTEXT));
        impl__SetBkColor_CDC__UEAAKK_Z(pDC, GlobalColor(kGlobalDataClrWindow, COLOR_WINDOW));
    }
    return GlobalWindowBrush();
}

// Retail (RVA 0x168160, mfc140u), fully transcribed:
//     if (m_pWndEdit->GetSafeHwnd() == NULL || !(m_pWndEdit->GetStyle() & WS_VISIBLE)) {
//         CMFCToolBarButton::OnDraw(pDC, rect, pImages, bHorz, bCustomizeMode, bHighlight, bDrawBorder, bGrayDisabledButtons);   // 0x15c910
//         return;
//     }
//     BOOL bDisabled = bCustomizeMode ? !IsEditable() : (m_nStyle & TBBS_DISABLED) != 0;   // vslot 0xd0 (26)
//     CRect rectBorder(0, 0, 0, 0);
//     GetEditBorder(rectBorder);                                                   // vslot 0x1b8 (55)
//     BOOL bIsHighlighted = (m_bFlat /*0x3b1b78*/ && !m_bIsHotEdit /*+0xa8*/) ? FALSE : TRUE;
//     CMFCVisualManager::GetInstance()->OnDrawEditBorder(pDC, rectBorder, bDisabled, bIsHighlighted, this);   // 0x9774, vslot 0xf8 (31)
//     if (m_bTextBelow && bHorz && !m_strText.IsEmpty()) {                          // +0x1c, +0x38
//         (bDisabled recomputed identically)
//         pDC->SetTextColor(bDisabled  ? afxGlobalData.clrGrayedText /*+0x44*/
//                         : bHighlight ? CMFCToolBar::GetHotTextColor()            // 0x156a70
//                                      : afxGlobalData.clrBtnText /*+0x34*/);      // CDC vslot 0x70 (14)
//         CRect rectText = rect;
//         rectText.top = (rect.bottom - m_sizeText.cy /*+0x7c*/ + rectBorder.bottom) / 2;
//         pDC->DrawText(m_strText, rectText, DT_CENTER | DT_WORDBREAK);            // CDC vslot 0xe0 (28)
//     }
// DEVIATIONS: the base OnDraw is a null stub in OpenMFC, so the no-window /
// hidden case draws nothing; the visual manager is the current
// ?m_pVisManager@ (no lazy default construction, as in CMFCToolBar.cpp), and
// a NULL manager skips the border; GetEditBorder goes through this file's
// thunk; IsEditable is the inlined base body; CDC virtuals go through their
// thunks.  Note also that OpenMFC's ?m_bFlat@ static (StaticData.cpp) is
// initialised to 0 whereas retail's .data holds 1, so until that initialiser
// is corrected bIsHighlighted is always TRUE here (retail: only while hot).
// Symbol: ?OnDraw@CMFCToolBarEditBoxButton@@UEAAXPEAVCDC@@AEBVCRect@@PEAVCMFCToolBarImages@@HHHHH@Z
extern "C" void MS_ABI impl__OnDraw_CMFCToolBarEditBoxButton__UEAAXPEAVCDC__AEBVCRect__PEAVCMFCToolBarImages__HHHHH_Z(
    CMFCToolBarEditBoxButton* pThis, CDC* pDC, const RECT* pRect, CMFCToolBarImages* pImages,
    int bHorz, int bCustomizeMode, int bHighlight, int bDrawBorder, int bGrayDisabledButtons)
{
    if (!pThis || !pDC || !pRect) return;
    CEdit* pEdit = WndEdit(pThis);
    if (!HasWindow(pEdit) || !StyleVisible(pEdit)) {
        impl__OnDraw_CMFCToolBarButton__UEAAXPEAVCDC__AEBVCRect__PEAVCMFCToolBarImages__HHHHH_Z(
            pThis, pDC, pRect, pImages, bHorz, bCustomizeMode, bHighlight, bDrawBorder, bGrayDisabledButtons);
        return;
    }
    const int bDisabled = (bCustomizeMode != 0) ? (IsEditableInline(pThis) ? 0 : 1)
                                                : (((pThis->m_nStyle & kTbbsDisabled) != 0) ? 1 : 0);
    RECT rectBorder = { 0, 0, 0, 0 };
    impl__GetEditBorder_CMFCToolBarEditBoxButton__UEAAXAEAVCRect___Z(pThis, &rectBorder);
    const int bIsHighlighted = (impl__m_bFlat_CMFCToolBarEditBoxButton__1HA != 0 && IsHotEdit(pThis) == 0) ? 0 : 1;
    if (CMFCVisualManager* pVM = static_cast<CMFCVisualManager*>(impl__m_pVisManager_CMFCVisualManager__1PEAV1_EA)) {
        impl__OnDrawEditBorder_CMFCVisualManager__UEAAXPEAVCDC__VCRect__HHPEAVCMFCToolBarEditBoxButton___Z(
            pVM, pDC, CRect(rectBorder), bDisabled, bIsHighlighted, pThis);
    }
    if (pThis->m_bTextBelow != 0 && bHorz != 0 && pThis->m_strText.GetLength() != 0) {
        COLORREF clrText;
        if (bDisabled) clrText = GlobalColor(kGlobalDataClrGrayedText, COLOR_GRAYTEXT);
        else if (bHighlight != 0) clrText = impl__GetHotTextColor_CMFCToolBar__SAKXZ();
        else clrText = GlobalColor(kGlobalDataClrBtnText, COLOR_BTNTEXT);
        impl__SetTextColor_CDC__UEAAKK_Z(pDC, clrText);
        RECT rectText = *pRect;
        rectText.top = (pRect->bottom - pThis->m_sizeText.cy + rectBorder.bottom) / 2;
        impl__DrawTextW_CDC__QEAAHPEB_WHPEAUTAGRECT__I_Z(pDC, static_cast<const wchar_t*>(pThis->m_strText),
                                                         pThis->m_strText.GetLength(), &rectText, kDtTextLabel);
    }
}

// Retail (RVA 0x168480, mfc140u), fully transcribed:
//     int iWidthUsed = CMFCToolBarButton::OnDrawOnCustomizeList(pDC, rect, bSelected);   // 0x15db10
//     int cx = rect.Width() - (iWidthUsed + 10);  if (cx < 8) cx = 8;
//     CRect rectEdit = rect;  rectEdit.left = rect.right - cx;  ::InflateRect(&rectEdit, -2, -2);
//     ::FillRect(pDC->m_hDC /*+0x08*/, &rectEdit, afxGlobalData.brWindow.m_hObject /*+0x110*/);
//     pDC->Draw3dRect(rectEdit, afxGlobalData.clrBarShadow /*+0x64*/, same);            // 0x2a5bc0
//     return rect.Width();
// DEVIATION: the base OnDrawOnCustomizeList is a null stub in OpenMFC
// (returns 0, so cx = rect.Width() - 10).
// Symbol: ?OnDrawOnCustomizeList@CMFCToolBarEditBoxButton@@UEAAHPEAVCDC@@AEBVCRect@@H@Z
extern "C" int MS_ABI impl__OnDrawOnCustomizeList_CMFCToolBarEditBoxButton__UEAAHPEAVCDC__AEBVCRect__H_Z(
    CMFCToolBarEditBoxButton* pThis, CDC* pDC, const RECT* pRect, int bSelected)
{
    if (!pThis || !pDC || !pRect) return 0;
    const int iWidthUsed = impl__OnDrawOnCustomizeList_CMFCToolBarButton__UEAAHPEAVCDC__AEBVCRect__H_Z(pThis, pDC, pRect, bSelected);
    int cx = (pRect->right - pRect->left) - (iWidthUsed + 10);
    if (cx < 8) cx = 8;
    RECT rectEdit = *pRect;
    rectEdit.left = pRect->right - cx;
    ::InflateRect(&rectEdit, -2, -2);
    ::FillRect(pDC->m_hDC, &rectEdit, GlobalWindowBrush());
    const COLORREF clrShadow = GlobalColor(kGlobalDataClrBarShadow, COLOR_BTNSHADOW);
    impl__Draw3dRect_CDC__QEAAXPEBUtagRECT__KK_Z(pDC, &rectEdit, clrShadow, clrShadow);
    return pRect->right - pRect->left;
}

// ===========================================================================
// Input / notifications
// ===========================================================================

// Retail (RVA 0x168450, mfc140u), fully transcribed (pWnd and bDelay are not read):
//     return m_pWndEdit != NULL && m_pWndEdit->m_hWnd != NULL && (m_pWndEdit->GetStyle() & WS_VISIBLE) != 0;   // 0x2a9690
// Symbol: ?OnClick@CMFCToolBarEditBoxButton@@UEAAHPEAVCWnd@@H@Z
extern "C" int MS_ABI impl__OnClick_CMFCToolBarEditBoxButton__UEAAHPEAVCWnd__H_Z(CMFCToolBarEditBoxButton* pThis, CWnd* pWnd, int bDelay) {
    (void)pWnd; (void)bDelay;
    if (!pThis) return 0;
    CEdit* pEdit = WndEdit(pThis);
    if (!HasWindow(pEdit)) return 0;
    return StyleVisible(pEdit) ? 1 : 0;
}

// Retail (RVA 0x168060, mfc140u), fully transcribed:
//     if (m_pWndEdit->GetSafeHwnd() == NULL || iNotifyCode != EN_UPDATE) return FALSE;
//     m_pWndEdit->GetWindowText(m_strContents);                                    // 0x28be00, +0x98
//     CObList lst;
//     if (CMFCToolBar::GetCommandButtons(m_nID, lst) > 0)                          // 0x152990
//         for each CMFCToolBarButton* p in lst:
//             if (p && p->IsKindOf(RUNTIME_CLASS(CMFCToolBarEditBoxButton)) && p != this)   // 0x234cf0, RTC 0x3b16b8
//                 p->SetContents(m_strContents);                                   // vslot 0x1b0 (54)
//     return !m_bChangingText;                                                     // +0xa0
// SetContents on the siblings goes through this file's thunk rather than the
// vftable; the CObList walk is the exported-thunk pattern of CommandButtonList.
// Symbol: ?NotifyCommand@CMFCToolBarEditBoxButton@@UEAAHH@Z
extern "C" int MS_ABI impl__NotifyCommand_CMFCToolBarEditBoxButton__UEAAHH_Z(CMFCToolBarEditBoxButton* pThis, int iNotifyCode) {
    if (!pThis) return 0;
    CEdit* pEdit = WndEdit(pThis);
    if (!HasWindow(pEdit) || iNotifyCode != kEnUpdate) return 0;
    impl__GetWindowTextW_CWnd__QEBAXAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(pEdit, &StrContents(pThis));
    CommandButtonList lst(pThis->m_nID);
    if (lst.Count() > 0) {
        for (ObNode* pNode = lst.First(); pNode != nullptr; pNode = pNode->pNext) {
            CObject* pObj = pNode->data;
            if (!IsEditBoxButton(pObj)) continue;
            CMFCToolBarEditBoxButton* pOther = static_cast<CMFCToolBarEditBoxButton*>(pObj);
            if (pOther == pThis) continue;
            impl__SetContents_CMFCToolBarEditBoxButton__UEAAXAEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(pOther, &StrContents(pThis));
        }
    }
    return ChangingText(pThis) == 0 ? 1 : 0;
}

// Retail (vftable slot 45, RVA 0x1689e0, mfc140u), fully transcribed (str is
// the 5th argument, on the stack; pWndParent and iButtonIndex are not read):
//     if (m_pWndEdit == NULL || !::IsWindow(m_pWndEdit->m_hWnd)) return FALSE;
//     CString strTipText;
//     BOOL bCustom = OnGetCustomToolTipText(strTipText);                          // vslot 0x160 (44): the base inline returns FALSE
//     wndToolTip.AddTool(m_pWndEdit, bCustom ? strTipText : str, NULL, 0);        // 0x275060
//     return TRUE;
// OnGetCustomToolTipText is the base inline (FALSE) -- not dispatched.
// Symbol: ?OnUpdateToolTip@CMFCToolBarEditBoxButton@@UEAAHPEAVCWnd@@HAEAVCToolTipCtrl@@AEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@@Z
extern "C" int MS_ABI impl__OnUpdateToolTip_CMFCToolBarEditBoxButton__UEAAHPEAVCWnd__HAEAVCToolTipCtrl__AEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
    CMFCToolBarEditBoxButton* pThis, CWnd* pWndParent, int iButtonIndex, CToolTipCtrl* pWndToolTip, CString* pStr)
{
    (void)pWndParent; (void)iButtonIndex;
    if (!pThis || !pWndToolTip || !pStr) return 0;
    CEdit* pEdit = WndEdit(pThis);
    if (pEdit == nullptr || !::IsWindow(pEdit->m_hWnd)) return 0;
    impl__AddTool_CToolTipCtrl__QEAAHPEAVCWnd__PEB_WPEBUtagRECT___K_Z(pWndToolTip, pEdit, static_cast<const wchar_t*>(*pStr), nullptr, 0);
    return 1;
}

// Retail (vftable slot 48, RVA 0x168b20, mfc140u), fully transcribed:
//     if (!CMFCToolBarButton::SetACCData(pParent, data)) return FALSE;              // 0x15e730
//     data.m_nAccRole  = ROLE_SYSTEM_TEXT;                                          // +0x30
//     data.m_bAccState = STATE_SYSTEM_FOCUSABLE;                                    // +0x34
//     if (HasFocus()) data.m_bAccState |= STATE_SYSTEM_FOCUSED;                     // vslot 0x158 (43)
//     data.m_strAccDefAction = L"Edit";                                             // +0x28, .rdata 0x33ee70
//     data.m_strAccValue = m_strText;                                               // +0x08 <- +0x38 (the label, NOT m_strContents)
//     return TRUE;
// DEVIATION: the base SetACCData is a null stub returning 0 in OpenMFC
// (featurepack/toolbar/CMFCToolBarButton.cpp), so this returns FALSE without
// filling the record until that base body exists; HasFocus is the inlined
// base body (HasFocusInline above).
// Symbol: ?SetACCData@CMFCToolBarEditBoxButton@@UEAAHPEAVCWnd@@AEAVCAccessibilityData@@@Z
extern "C" int MS_ABI impl__SetACCData_CMFCToolBarEditBoxButton__UEAAHPEAVCWnd__AEAVCAccessibilityData___Z(
    CMFCToolBarEditBoxButton* pThis, CWnd* pParent, CAccessibilityData* pData)
{
    if (!pThis || !pData) return 0;
    if (!impl__SetACCData_CMFCToolBarButton__UEAAHPEAVCWnd__AEAVCAccessibilityData___Z(pThis, pParent, pData)) return 0;
    pData->m_nAccRole = kRoleSystemText;
    pData->m_bAccState = kStateSystemFocusable;
    if (HasFocusInline(pThis)) pData->m_bAccState |= kStateSystemFocused;
    pData->m_strAccDefAction = L"Edit";
    pData->m_strAccValue = pThis->m_strText;
    return 1;
}
