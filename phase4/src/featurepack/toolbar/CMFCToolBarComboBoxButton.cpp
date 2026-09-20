// CMFCToolBarComboBoxButton — OpenMFC implementation.
// Sources: cbarcore.cpp, mfccore.cpp
//
// Every body marked "Retail (RVA 0x..., mfc140u)" below was transcribed from the
// disassembly of that export in mfc140u.dll (14.51.36231); the instance layout
// it reads is the one in the SDK header afxtoolbarcomboboxbutton.h, confirmed
// by the member offsets those bodies use (sizeof 336 == 0x150):
//
//   CMFCToolBarButton base ............................. +0x00 .. +0x88
//   +0x88  int        m_iWidth         +0x8c  int   m_iSelIndex
//   +0x90  int        m_nDropDownHeight+0x94  BOOL  m_bHorz
//   +0x98  BOOL       m_bIsHotEdit     +0x9c  BOOL  m_bIsRibbon
//   +0xa0  UINT       m_uiMenuResID    +0xa4  DWORD m_dwStyle
//   +0xa8  CRect      m_rectCombo      +0xb8  CRect m_rectButton
//   +0xc8  CEdit*     m_pWndEdit       +0xd0  CComboBox* m_pWndCombo
//   +0xd8  CString    m_strEdit
//   +0xe0  CStringList m_lstItems (56 bytes)
//   +0x118 CList<DWORD_PTR,DWORD_PTR> m_lstItemData (56 bytes)
//
// include/openmfc/afxmfc.h declares only `BOOL m_bFlat; BOOL m_bCenterVert;
// char _pad[192];` after the base.  In the real class m_bFlat / m_bCenterVert
// are STATIC (?m_bFlat@CMFCToolBarComboBoxButton@@1HA at 0x3b1b74 and
// ?m_bCenterVert@...@@1HA at 0x3b1b70 in mfc140u, both 1 in retail .data); the
// two instance slots the header declares alias m_iWidth / m_iSelIndex.  The
// bodies here therefore reach the retail fields by offset (see the accessors
// in the anonymous namespace) and never through those two header members.
//
// DEVIATION (whole file): m_lstItems / m_lstItemData are NOT modelled at
// +0xe0 / +0x118.  OpenMFC's CStringList carries no in-object state (it is a
// side-table class, see core/collections/CStringList.cpp), so a retail-shaped
// list cannot live in the padding.  The item list lives in the cbarcore side
// table instead (ComboButtonState: EnsureComboState / FindComboState), which
// is what the item API in this file (AddItem, GetItem, FindItem, ...) already
// used before the retail bodies were added.  The 112 bytes at +0xe0 stay zero.
// A real MSVC client that reads those two protected members directly sees an
// empty list; every exported entry point behaves.

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
//   featurepack/toolbar/StaticData.cpp : the class statics (retail .data values in the comments)
extern "C" std::int32_t impl__m_bFlat_CMFCToolBarComboBoxButton__1HA;                      // 0x3b1b74, retail 1 (OpenMFC 0)
extern "C" std::int32_t impl__m_bCenterVert_CMFCToolBarComboBoxButton__1HA;                // 0x3b1b70, retail 1 (OpenMFC 0)
extern "C" std::int32_t impl__m_bCustomizeMode_CMFCToolBar__1HA;                           // 0x3be35c
extern "C" std::int32_t impl__m_bShowTooltips_CMFCToolBar__1HA;                            // 0x3b1b64, retail 1
//   core/runtime/StaticData.cpp : ?m_pVisManager@CMFCVisualManager@@1PEAV1@EA (0x3be3c0)
extern "C" void* impl__m_pVisManager_CMFCVisualManager__1PEAV1_EA;
//   featurepack/visualmanager/Thunks.cpp : dispatch virtually on the C++ manager object
extern "C" void MS_ABI impl__OnDrawComboBorder_CMFCVisualManager__UEAAXPEAVCDC__VCRect__HHHPEAVCMFCToolBarComboBoxButton___Z(
    CMFCVisualManager* pThis, CDC* pDC, CRect rect, int bDisabled, int bIsDropped, int bIsHighlighted, CMFCToolBarComboBoxButton* pButton);
extern "C" void MS_ABI impl__OnDrawComboDropButton_CMFCVisualManager__UEAAXPEAVCDC__VCRect__HHHPEAVCMFCToolBarComboBoxButton___Z(
    CMFCVisualManager* pThis, CDC* pDC, CRect rect, int bDisabled, int bIsDropped, int bIsHighlighted, CMFCToolBarComboBoxButton* pButton);
//   featurepack/toolbar/CMFCToolBar.cpp : GetHotTextColor, GetRowHeight, GetCommandButtons
extern "C" unsigned long MS_ABI impl__GetHotTextColor_CMFCToolBar__SAKXZ();
extern "C" int MS_ABI impl__GetRowHeight_CMFCToolBar__UEBAHXZ(const CMFCToolBar* pThis);
extern "C" int MS_ABI impl__GetCommandButtons_CMFCToolBar__SAHIAEAVCObList___Z(unsigned int uiCmd, CObList* pLstButtons);
//   featurepack/toolbar/RuntimeClasses.cpp : GetThisClass (RTTI descriptors)
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMFCToolBar__SAPEAUCRuntimeClass__XZ();
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMFCToolBarComboBoxButton__SAPEAUCRuntimeClass__XZ();
//   core/runtime/CObject.cpp : IsKindOf
extern "C" int MS_ABI impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(const CObject* pThis, const CRuntimeClass* pClass);
//   core/collections/CObList.cpp : ctor / dtor / FindIndex (the walk pattern of
//   featurepack/docking/CMultiPaneFrameWnd.cpp, VisiblePaneList)
extern "C" void* MS_ABI impl___0CObList__QEAA__J_Z(void* pThis, long long nBlockSize);
extern "C" void MS_ABI impl___1CObList__UEAA_XZ(void* pThis);
extern "C" CObList::POSITION MS_ABI impl__FindIndex_CObList__QEBAPEAU__POSITION____J_Z(const CObList* pThis, long long nIndex);
//   core/collections/CStringList.cpp : ctor / dtor / AddTail / Serialize / FindIndex
extern "C" void* MS_ABI impl___0CStringList__QEAA__J_Z(CStringList* pThis, long long nBlockSize);
extern "C" void MS_ABI impl___1CStringList__UEAA_XZ(CStringList* pThis);
extern "C" CStringList::POSITION MS_ABI impl__AddTail_CStringList__QEAAPEAU__POSITION__AEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
    CStringList* pThis, const CString& value);
extern "C" void MS_ABI impl__Serialize_CStringList__UEAAXAEAVCArchive___Z(CStringList* pThis, CArchive& ar);
extern "C" CStringList::POSITION MS_ABI impl__FindIndex_CStringList__QEBAPEAU__POSITION____J_Z(const CStringList* pThis, long long nIndex);
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
extern "C" void* MS_ABI impl__SetFocus_CWnd__QEAAPEAV1_XZ(CWnd* pThis);
extern "C" void MS_ABI impl__SetWindowTextW_CWnd__QEAAXPEB_W_Z(CWnd* pThis, const wchar_t* lpsz);
extern "C" void MS_ABI impl__GetWindowTextW_CWnd__QEBAXAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
    const CWnd* pThis, CString* pString);
extern "C" int MS_ABI impl__SetWindowPos_CWnd__QEAAHPEBV1_HHHHI_Z(
    CWnd* pThis, const CWnd* pWndInsertAfter, int x, int y, int cx, int cy, unsigned int nFlags);
extern "C" void MS_ABI impl__ScreenToClient_CWnd__QEBAXPEAUtagRECT___Z(const CWnd* pThis, RECT* pRect);
extern "C" int MS_ABI impl__DestroyWindow_CWnd__UEAAHXZ(CWnd* pThis);
extern "C" void* MS_ABI impl___0CWnd__QEAA_XZ(void* pThis);
extern "C" void MS_ABI impl___1CWnd__UEAA_XZ(void* pThis);
//   detail/CtrlcoreSupport.cpp / core/controls/CComboBox.cpp
extern "C" int MS_ABI impl__Create_CComboBox__UEAAHKAEBUtagRECT__PEAVCWnd__I_Z(
    CComboBox* pThis, DWORD dwStyle, const RECT* pRect, CWnd* pParentWnd, UINT nID);
extern "C" void MS_ABI impl__GetLBText_CComboBox__QEBAXHAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
    const CComboBox* pThis, int nIndex, CString* pText);
//   core/controls/Thunks.cpp : CToolTipCtrl::AddTool
extern "C" int MS_ABI impl__AddTool_CToolTipCtrl__QEAAHPEAVCWnd__PEB_WPEBUtagRECT___K_Z(
    CToolTipCtrl* pThis, CWnd* pWnd, const wchar_t* lpszText, const RECT* lpRectTool, unsigned __int64 nIDTool);
//   core/gdi/CDC.cpp / detail/GdicoreSupport.cpp
extern "C" unsigned long MS_ABI impl__SetTextColor_CDC__UEAAKK_Z(CDC* pThis, unsigned long crColor);
extern "C" unsigned long MS_ABI impl__SetBkColor_CDC__UEAAKK_Z(CDC* pThis, unsigned long crColor);
extern "C" void MS_ABI impl__FillSolidRect_CDC__QEAAXPEBUtagRECT__K_Z(CDC* pThis, const RECT* lpRect, unsigned long clr);
extern "C" void MS_ABI impl__Draw3dRect_CDC__QEAAXPEBUtagRECT__KK_Z(CDC* pThis, const RECT* lpRect, unsigned long clrTopLeft, unsigned long clrBottomRight);
extern "C" int MS_ABI impl__DrawTextW_CDC__QEAAHPEB_WHPEAUTAGRECT__I_Z(
    CDC* pThis, const wchar_t* lpszString, int nCount, RECT* lpRect, unsigned int nFormat);
//   detail/MemcoreSupport.cpp : ??2@YAPEAX_K@Z / ??3@YAXPEAX@Z
extern "C" void* MS_ABI impl___2_YAPEAX_K_Z(std::size_t size);
extern "C" void MS_ABI impl___3_YAXPEAX_Z(void* ptr);
//   featurepack/CMFC_misc_stubs.cpp : AfxFindStringResourceHandle
extern "C" void* MS_ABI impl__AfxFindStringResourceHandle__YAPEAUHINSTANCE____I_Z(unsigned int nID);

// Exports defined later in this file that earlier bodies call (all in this TU).
extern "C" void MS_ABI impl__Initialize_CMFCToolBarComboBoxButton__IEAAXXZ(CMFCToolBarComboBoxButton* pThis);
extern "C" void MS_ABI impl__AdjustRect_CMFCToolBarComboBoxButton__IEAAXXZ(CMFCToolBarComboBoxButton* pThis);
extern "C" void MS_ABI impl__OnMove_CMFCToolBarComboBoxButton__UEAAXXZ(CMFCToolBarComboBoxButton* pThis);
extern "C" int MS_ABI impl__NotifyCommand_CMFCToolBarComboBoxButton__UEAAHH_Z(CMFCToolBarComboBoxButton* pThis, int iNotifyCode);
extern "C" int MS_ABI impl__HasFocus_CMFCToolBarComboBoxButton__UEBAHXZ(const CMFCToolBarComboBoxButton* pThis);
extern "C" void MS_ABI impl__CopyFrom_CMFCToolBarComboBoxButton__UEAAXAEBVCMFCToolBarButton___Z(
    CMFCToolBarComboBoxButton* pThis, const CMFCToolBarButton* pSrc);
extern "C" CComboBox* MS_ABI impl__CreateCombo_CMFCToolBarComboBoxButton__UEAAPEAVCComboBox__PEAVCWnd__AEBVCRect___Z(
    CMFCToolBarComboBoxButton* pThis, CWnd* pWndParent, const RECT* pRect);
extern "C" CEdit* MS_ABI impl__CreateEdit_CMFCToolBarComboBoxButton__UEAAPEAVCMFCToolBarComboBoxEdit__PEAVCWnd__AEBVCRect__K_Z(
    CMFCToolBarComboBoxButton* pThis, CWnd* pWndParent, const RECT* pRect, unsigned long dwEditStyle);

namespace {

// Retail member offsets (see the file header).  Cross-checked against the
// header's own layout: the base is 136 bytes, the header's two BOOLs sit at
// +0x88/+0x8c and _pad starts at +0x90.
constexpr int kOffWidth          = 0x88;
constexpr int kOffSelIndex       = 0x8c;
constexpr int kOffDropDownHeight = 0x90;
constexpr int kOffHorz           = 0x94;
constexpr int kOffIsHotEdit      = 0x98;
constexpr int kOffIsRibbon       = 0x9c;
constexpr int kOffMenuResID      = 0xa0;
constexpr int kOffStyle          = 0xa4;
constexpr int kOffRectCombo      = 0xa8;
constexpr int kOffRectButton     = 0xb8;
constexpr int kOffWndEdit        = 0xc8;
constexpr int kOffWndCombo       = 0xd0;
constexpr int kOffStrEdit        = 0xd8;

static_assert(sizeof(CMFCToolBarButton) == 0x88, "CMFCToolBarComboBoxButton's own members start at +0x88");
static_assert(offsetof(CMFCToolBarComboBoxButton, m_bFlat) == kOffWidth, "header m_bFlat slot aliases retail m_iWidth");
static_assert(offsetof(CMFCToolBarComboBoxButton, m_bCenterVert) == kOffSelIndex, "header m_bCenterVert slot aliases retail m_iSelIndex");
static_assert(offsetof(CMFCToolBarComboBoxButton, _pad) == kOffDropDownHeight, "retail m_nDropDownHeight is the first padding dword");
static_assert(sizeof(CMFCToolBarComboBoxButton) == 0x150, "retail sizeof(CMFCToolBarComboBoxButton) is 336");
static_assert(offsetof(CMFCToolBarButton, m_bTextBelow) == 0x1c, "m_bTextBelow @0x1c (the label flag every retail body here tests; NOT m_bText @0x0c)");
static_assert(offsetof(CMFCToolBarButton, m_nID) == 0x24, "m_nID @0x24");
static_assert(offsetof(CMFCToolBarButton, m_nStyle) == 0x28, "m_nStyle @0x28");
static_assert(offsetof(CMFCToolBarButton, m_strText) == 0x38, "m_strText @0x38");
static_assert(offsetof(CMFCToolBarButton, m_bIsHidden) == 0x54, "m_bIsHidden @0x54");
static_assert(offsetof(CMFCToolBarButton, m_bVisible) == 0x64, "m_bVisible @0x64");
static_assert(offsetof(CMFCToolBarButton, m_rect) == 0x68, "m_rect @0x68");
static_assert(offsetof(CMFCToolBarButton, m_sizeText) == 0x78, "m_sizeText @0x78");
static_assert(offsetof(CWnd, m_hWnd) == 0x40, "CWnd::m_hWnd @0x40 (every retail body reads the HWND there)");
static_assert(offsetof(CDC, m_hDC) == 0x08 && offsetof(CDC, m_hAttribDC) == 0x10, "CDC handles @0x08 / @0x10");
static_assert(offsetof(CAccessibilityData, m_strAccValue) == 0x08, "CAccessibilityData::m_strAccValue @0x08");
static_assert(offsetof(CAccessibilityData, m_strAccDefAction) == 0x28, "CAccessibilityData::m_strAccDefAction @0x28");
static_assert(offsetof(CAccessibilityData, m_nAccRole) == 0x30, "CAccessibilityData::m_nAccRole @0x30");
static_assert(offsetof(CAccessibilityData, m_bAccState) == 0x34, "CAccessibilityData::m_bAccState @0x34");
static_assert(sizeof(CString) == sizeof(void*), "CString is one pointer (m_strEdit occupies +0xd8..+0xe0)");
static_assert(sizeof(CRect) == sizeof(RECT), "CRect is RECT-shaped");

template <class T> inline T& At(void* p, int off) {
    return *reinterpret_cast<T*>(static_cast<char*>(p) + off);
}
template <class T> inline const T& At(const void* p, int off) {
    return *reinterpret_cast<const T*>(static_cast<const char*>(p) + off);
}

inline int&        Width(CMFCToolBarComboBoxButton* p)          { return At<int>(p, kOffWidth); }
inline int         Width(const CMFCToolBarComboBoxButton* p)    { return At<int>(p, kOffWidth); }
inline int&        SelIndex(CMFCToolBarComboBoxButton* p)       { return At<int>(p, kOffSelIndex); }
inline int         SelIndex(const CMFCToolBarComboBoxButton* p) { return At<int>(p, kOffSelIndex); }
inline int&        DropDownHeight(CMFCToolBarComboBoxButton* p) { return At<int>(p, kOffDropDownHeight); }
inline int         DropDownHeight(const CMFCToolBarComboBoxButton* p) { return At<int>(p, kOffDropDownHeight); }
inline int&        Horz(CMFCToolBarComboBoxButton* p)           { return At<int>(p, kOffHorz); }
inline int         Horz(const CMFCToolBarComboBoxButton* p)     { return At<int>(p, kOffHorz); }
inline int&        IsHotEdit(CMFCToolBarComboBoxButton* p)      { return At<int>(p, kOffIsHotEdit); }
inline int&        IsRibbon(CMFCToolBarComboBoxButton* p)       { return At<int>(p, kOffIsRibbon); }
inline int         IsRibbon(const CMFCToolBarComboBoxButton* p) { return At<int>(p, kOffIsRibbon); }
inline unsigned&   MenuResID(CMFCToolBarComboBoxButton* p)      { return At<unsigned>(p, kOffMenuResID); }
inline unsigned    MenuResID(const CMFCToolBarComboBoxButton* p){ return At<unsigned>(p, kOffMenuResID); }
inline DWORD&      Style(CMFCToolBarComboBoxButton* p)          { return At<DWORD>(p, kOffStyle); }
inline DWORD       Style(const CMFCToolBarComboBoxButton* p)    { return At<DWORD>(p, kOffStyle); }
inline RECT&       RectCombo(CMFCToolBarComboBoxButton* p)      { return At<RECT>(p, kOffRectCombo); }
inline const RECT& RectCombo(const CMFCToolBarComboBoxButton* p){ return At<RECT>(p, kOffRectCombo); }
inline RECT&       RectButton(CMFCToolBarComboBoxButton* p)     { return At<RECT>(p, kOffRectButton); }
inline CEdit*&     WndEdit(CMFCToolBarComboBoxButton* p)        { return At<CEdit*>(p, kOffWndEdit); }
inline CEdit*      WndEdit(const CMFCToolBarComboBoxButton* p)  { return At<CEdit*>(p, kOffWndEdit); }
inline CComboBox*& WndCombo(CMFCToolBarComboBoxButton* p)       { return At<CComboBox*>(p, kOffWndCombo); }
inline CComboBox*  WndCombo(const CMFCToolBarComboBoxButton* p) { return At<CComboBox*>(p, kOffWndCombo); }
inline CString&    StrEdit(CMFCToolBarComboBoxButton* p)        { return At<CString>(p, kOffStrEdit); }
inline const CString& StrEdit(const CMFCToolBarComboBoxButton* p){ return At<CString>(p, kOffStrEdit); }

inline HWND HWndOf(const CWnd* pWnd) { return pWnd ? pWnd->m_hWnd : nullptr; }
// `pWnd->GetSafeHwnd() != NULL` as retail spells it: the pointer, then +0x40.
inline bool HasWindow(const CWnd* pWnd) { return pWnd != nullptr && pWnd->m_hWnd != nullptr; }
inline bool StyleVisible(const CWnd* pWnd) { return (impl__GetStyle_CWnd__QEBAKXZ(pWnd) & WS_VISIBLE) != 0; }

// Message / style numbers the retail bodies use as immediates.
constexpr UINT     kCbSetEditSel      = 0x142;  // CB_SETEDITSEL
constexpr UINT     kCbAddString       = 0x143;  // CB_ADDSTRING
constexpr UINT     kCbDeleteString    = 0x144;  // CB_DELETESTRING
constexpr UINT     kCbGetCount        = 0x146;  // CB_GETCOUNT
constexpr UINT     kCbGetCurSel       = 0x147;  // CB_GETCURSEL
constexpr UINT     kCbResetContent    = 0x14b;  // CB_RESETCONTENT
constexpr UINT     kCbFindString      = 0x14c;  // CB_FINDSTRING
constexpr UINT     kCbSetCurSel       = 0x14e;  // CB_SETCURSEL
constexpr UINT     kCbShowDropDown    = 0x14f;  // CB_SHOWDROPDOWN
constexpr UINT     kCbGetItemData     = 0x150;  // CB_GETITEMDATA
constexpr UINT     kCbSetItemData     = 0x151;  // CB_SETITEMDATA
constexpr UINT     kCbGetDroppedState = 0x157;  // CB_GETDROPPEDSTATE
constexpr UINT     kCbFindStringExact = 0x158;  // CB_FINDSTRINGEXACT
constexpr int      kCbnSelChange      = 1;      // CBN_SELCHANGE
constexpr int      kCbnSetFocus       = 3;      // CBN_SETFOCUS
constexpr int      kCbnKillFocus      = 4;      // CBN_KILLFOCUS
constexpr int      kCbnEditChange     = 5;      // CBN_EDITCHANGE
constexpr int      kCbnEditUpdate     = 6;      // CBN_EDITUPDATE
constexpr int      kCbnSelEndOk       = 9;      // CBN_SELENDOK
constexpr unsigned kTbbsDisabled      = 0x40000;                       // TBBS_DISABLED
constexpr unsigned kSwpNoZorderNoActivate = 0x14;                      // SWP_NOZORDER | SWP_NOACTIVATE
constexpr unsigned kRdwInvalidateEraseUpdate = 0x105;                  // RDW_INVALIDATE | RDW_ERASE | RDW_UPDATENOW
constexpr DWORD    kDefaultComboStyle = 0x50200403;                    // WS_CHILD|WS_VISIBLE|WS_VSCROLL|CBS_NOINTEGRALHEIGHT|CBS_DROPDOWNLIST
constexpr DWORD    kEditStyleBase     = 0x50001080;                    // WS_CHILD|WS_VISIBLE|ES_WANTRETURN|ES_AUTOHSCROLL -- see OnChangeParentWnd
constexpr int      kDefaultWidth      = 150;                           // 0x96
constexpr int      kDefaultDropDownHeight = 150;                       // 0x96
constexpr unsigned kDtTextLabel       = 0x11;                          // DT_CENTER | DT_WORDBREAK       (OnDraw label)
constexpr unsigned kDtEditText        = 0x824;                         // DT_VCENTER | DT_SINGLELINE | DT_NOPREFIX (OnDraw edit text)
constexpr unsigned kDtCalcLabel       = 0x411;                         // DT_CALCRECT | DT_CENTER | DT_WORDBREAK (OnCalculateSize)
constexpr int      kRoleSystemComboBox = 0x2e;                         // ROLE_SYSTEM_COMBOBOX
constexpr int      kRoleSystemDropList = 0x2f;                         // ROLE_SYSTEM_DROPLIST
constexpr long     kStateSystemFocusable = 0x100000;                   // STATE_SYSTEM_FOCUSABLE
constexpr long     kStateSystemFocused   = 0x4;                        // STATE_SYSTEM_FOCUSED
constexpr int      kMenuImageSize     = 9;                             // CMenuImages::m_sizeImage.cx, see MenuImageExtent

// ---------------------------------------------------------------------------
// afxGlobalData.  AFX_GLOBAL_DATA is not declared in OpenMFC's headers;
// ?afxGlobalData@@ is a zeroed 720-byte block whose Initialize thunk is an
// empty stub.  The slots below are the byte offsets the retail bodies in this
// file read.  Their names are the SDK's (atlmfc/include/afxglobals.h member
// order: 4 BOOLs, 3 HBRUSHes, then the COLORREFs from clrBtnFace at +0x28;
// the CBrushes from brBtnFace at +0xa8, 16 bytes each), and each was
// cross-checked against ?UpdateSysColors@AFX_GLOBAL_DATA@@QEAAXXZ (RVA
// 0x6b1c0, mfc140u), which stores GetSysColor(n) at:
//   +0x028 clrBtnFace     <- COLOR_BTNFACE(15)     +0x044 clrGrayedText <- COLOR_GRAYTEXT(17)
//   +0x064 clrBarShadow   <- COLOR_BTNSHADOW(16)   +0x068 clrBarHilite  <- COLOR_BTNHIGHLIGHT(20)
//   +0x074 clrBarText     <- COLOR_BTNTEXT(18)     +0x078 clrWindow     <- COLOR_WINDOW(5)
//   +0x07c clrWindowText  <- COLOR_WINDOWTEXT(8)
//   +0x110 brWindow.m_hObject (CBrush at +0x108; UpdateSysColors recreates it as CreateSolidBrush(clrWindow))
//   +0x260 m_bIsBlackHighContrast (3DDKSHADOW == white && BTNFACE == black)
// and ?Initialize@AFX_GLOBAL_DATA@@QEAAXXZ (RVA 0x6a790), which stores
//   +0x258 bIsWindows7 <- VerifyVersionInfoW(major 6, minor 1)
// plus +0x1b0, fontRegular.m_hObject (CFont at +0x1a8).
constexpr int kGlobalDataInitGate          = 0x000;
constexpr int kGlobalDataClrBtnFace        = 0x028;
constexpr int kGlobalDataClrGrayedText     = 0x044;
constexpr int kGlobalDataClrBarShadow      = 0x064;
constexpr int kGlobalDataClrBarHilite      = 0x068;
constexpr int kGlobalDataClrBarText        = 0x074;
constexpr int kGlobalDataClrWindow         = 0x078;
constexpr int kGlobalDataClrWindowText     = 0x07c;
constexpr int kGlobalDataBrWindowHandle    = 0x110;
constexpr int kGlobalDataFontRegularHandle = 0x1b0;
constexpr int kGlobalDataIsWindows7        = 0x258;
constexpr int kGlobalDataIsBlackHighContrast = 0x260;

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
// DEVIATION (shared by every colour / brush / font read below): because
// OpenMFC's AFX_GLOBAL_DATA::Initialize is an empty stub, the block never gets
// its UpdateSysColors / UpdateFonts pass and every colour slot reads 0 (black),
// the brush NULL and the font NULL.  Retail would have populated them from
// exactly the GetSysColor / GetSysColorBrush / NONCLIENTMETRICS values, so when
// the block is unpopulated (brWindow at +0x110 still NULL -- UpdateSysColors is
// what creates it) the readers fall back to the live system value the slot
// would hold.  Once Initialize is implemented the fallbacks are never taken.
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

// CMenuImages::Size().cx as retail computes it (?Size@CMenuImages@@SA?AVCSize@@XZ,
// RVA 0x8f600, mfc140u -- OpenMFC's thunk for it is a null stub with a
// placeholder list, so the value is inlined here):
//     if (<0x3be258> != 0)              // a re-entrancy guard that ?Initialize@CMenuImages@@
//                                        // (0x8f3c0) sets on entry and clears before it returns
//         return (int)(9 * afxGlobalData.<+0x2b8> + 0.5) if <+0x2c0> && scale != 1.0, else 9;
//     CMenuImages::Initialize();         // no-op once the image well exists
//     return m_sizeImage;                // static CSize at 0x3c1968
// m_sizeImage has exactly one writer in the DLL: inside Initialize, `lea
// 0x8(%rbx)` (rbx == 1) at 0x8f449 and the two stores at 0x8f44c / 0x8f452 set
// it to (9, 9); the SmoothResize pass that follows scales the bitmaps, not
// this size.  AdjustRect never runs while
// CMenuImages::Initialize is on the stack, so the reachable result is 9 in
// every case -- the ribbon-image scale is NOT applied on this path.
inline int MenuImageExtent() {
    return kMenuImageSize;
}

// CMFCToolBarButton::IsEditable() as the retail vftable slot 26 body reads
// (RVA 0x23760, mfc140u -- the inline from afxtoolbarbutton.h, IsStandardCommand
// folded in):
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
// ?m_lstProtectedCommands@ is an 8-byte placeholder, not a CList.
inline bool IsEditableInline(const CMFCToolBarButton* p) {
    const unsigned id = p->m_nID;
    if (id - 0xE110u <= 0xFu) return false;
    if (id - 0xF000u <= 0x1EFu) return false;
    if (static_cast<int>(id) >= 0xFF00) return false;
    if (id - 0xE210u <= 0xFu) return false;
    return true;
}

// CObList walk (the pattern featurepack/docking/CMultiPaneFrameWnd.cpp uses):
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
inline bool IsComboButton(const CObject* p) {
    return p != nullptr &&
           impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(p, impl__GetThisClass_CMFCToolBarComboBoxButton__SAPEAUCRuntimeClass__XZ()) != 0;
}

// The same for a CStringList (used by Serialize so the item list goes through
// OpenMFC's own ?Serialize@CStringList@@ and therefore its own on-disk format).
struct StrNode {
    StrNode* pNext;
    StrNode* pPrev;
    CString  data;
};
static_assert(sizeof(CStringList::POSITION) == sizeof(void*), "POSITION is one pointer");
inline StrNode* StrNodeFromPos(CStringList::POSITION pos) {
    StrNode* p = nullptr;
    std::memcpy(&p, &pos, sizeof(p));
    return p;
}
class ScratchStringList {
public:
    ScratchStringList() {
        std::memset(m_buf, 0, sizeof(m_buf));
        impl___0CStringList__QEAA__J_Z(List(), 10);
    }
    ~ScratchStringList() { impl___1CStringList__UEAA_XZ(List()); }
    CStringList* List() const { return reinterpret_cast<CStringList*>(const_cast<unsigned char*>(m_buf)); }
    void AddTail(const CString& s) {
        impl__AddTail_CStringList__QEAAPEAU__POSITION__AEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(List(), s);
    }
    StrNode* First() const { return StrNodeFromPos(impl__FindIndex_CStringList__QEBAPEAU__POSITION____J_Z(List(), 0)); }
    ScratchStringList(const ScratchStringList&) = delete;
    ScratchStringList& operator=(const ScratchStringList&) = delete;
private:
    alignas(8) unsigned char m_buf[sizeof(CStringList)];
};

// CArchive scalar / CString I/O through the exported Read / Write, in exactly
// the byte layout OpenMFC's own C++ CArchive::operator<< / >> use
// (core/runtime/CArchive.cpp: 4-byte int; CString = 4-byte length + UTF-16
// characters).  Retail inlines the int case as direct buffer stores and
// the CString case as ?AfxWriteStringLength@@ + characters; OpenMFC's archive
// format is the one its base-class Serialize (tbb_Serialize) already writes,
// so this file matches it rather than retail's wire format.
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

// The window objects this button owns.  Retail tears one down with
//     pWnd->DestroyWindow();          // vftable +0xd0 (slot 26)
//     pWnd->`scalar deleting dtor`(1) // vftable +0x08 (slot 1)
// i.e. through the object's own MSVC vftable.  An object this file created
// (CreateCombo below) carries OpenMFC's mingw CWnd vftable, on which those
// slot numbers mean something else, so such an object is released through
// the exported ?DestroyWindow@CWnd@@ / ??1CWnd@@ / ??3@ instead; g_ownCWndVptr
// (the vptr ??0CWnd@@QEAA@XZ installs, recorded when the first one is built)
// tells the two apart.  Any other object -- one a derived class's CreateCombo /
// CreateEdit override handed us, built against the real MFC headers -- is
// released exactly as retail does, through its own vftable slots.
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
// The "repaint my combo area on the parent" sequence NotifyCommand / SetHotEdit share:
//     if (CWnd* pParent = m_pWndCombo->GetParent()) {
//         pParent->InvalidateRect(&m_rectCombo, TRUE);   (::InvalidateRect on pParent->m_hWnd)
//         pParent->UpdateWindow();
//     }
inline void RepaintComboOnParent(CMFCToolBarComboBoxButton* p) {
    CWnd* pParent = ParentOf(WndCombo(p));
    if (pParent == nullptr) return;
    ::InvalidateRect(ParentOf(WndCombo(p))->m_hWnd, &RectCombo(p), TRUE);
    ::UpdateWindow(ParentOf(WndCombo(p))->m_hWnd);
}

} // namespace

// ===========================================================================
// Item API (side-table storage, see the file header).  The scalar state these
// bodies keep -- the selection index and the edit text -- lives in the retail
// fields m_iSelIndex (+0x8c) and m_strEdit (+0xd8) so that a client's inline
// GetCurSel() / GetText() read what the exported entry points wrote.
// ===========================================================================

// Symbol: ?AddItem@CMFCToolBarComboBoxButton@@UEAA_JPEB_W_K@Z
INT_PTR CMFCToolBarComboBoxButton::AddItem(const wchar_t* lpszItem, DWORD_PTR dwData) {
    if (!lpszItem) return CB_ERR;
    RegisterComboButton(this);
    ComboButtonState& state = EnsureComboState(this);
    state.items.emplace_back(CString(lpszItem), dwData);
    return static_cast<INT_PTR>(state.items.size() - 1);
}
// Symbol: ?AddSortedItem@CMFCToolBarComboBoxButton@@UEAA_JPEB_W_K@Z
INT_PTR CMFCToolBarComboBoxButton::AddSortedItem(const wchar_t* lpszItem, DWORD_PTR dwData) {
    if (!lpszItem) return CB_ERR;
    RegisterComboButton(this);
    ComboButtonState& state = EnsureComboState(this);
    auto insertPos = state.items.begin();
    while (insertPos != state.items.end() &&
           CompareComboText(static_cast<const wchar_t*>(insertPos->first), lpszItem) <= 0) {
        ++insertPos;
    }
    const int index = static_cast<int>(insertPos - state.items.begin());
    state.items.insert(insertPos, std::make_pair(CString(lpszItem), dwData));
    if (SelIndex(this) >= index) ++SelIndex(this);
    return index;
}
// Symbol: ?Compare@CMFCToolBarComboBoxButton@@UEAAHPEB_W0@Z
int CMFCToolBarComboBoxButton::Compare(const wchar_t* s1, const wchar_t* s2) {
    return CompareComboText(s1, s2);
}
// Retail (RVA 0x163d80, mfc140u), fully transcribed:
//     if (iIndex < 0 || iIndex >= m_lstItems.GetCount()) return FALSE;
//     m_lstItems.RemoveAt(FindIndex(iIndex));  (FALSE if the node is missing)   // 0x232990
//     if (iIndex >= m_lstItemData.GetCount()) return FALSE;  m_lstItemData.RemoveAt(FindIndex(iIndex));
//     if (m_pWndCombo->GetSafeHwnd()) m_pWndCombo->SendMessage(CB_DELETESTRING, iIndex);
//     if (iIndex == m_iSelIndex) {
//         int i = m_iSelIndex;  if (i >= m_lstItems.GetCount()) i = m_lstItems.GetCount() - 1;   // the count AFTER removal
//         SelectItem(i, FALSE);                                                       // 0x163b80
//     }
//     return TRUE;
// The two lists are the one side-table vector of pairs.
// Symbol: ?DeleteItem@CMFCToolBarComboBoxButton@@QEAAHH@Z
BOOL CMFCToolBarComboBoxButton::DeleteItem(int nIndex) {
    RegisterComboButton(this);
    ComboButtonState& state = EnsureComboState(this);
    if (nIndex < 0 || nIndex >= static_cast<int>(state.items.size())) return FALSE;
    state.items.erase(state.items.begin() + nIndex);
    if (CComboBox* pCombo = WndCombo(this); HasWindow(pCombo)) {
        ::SendMessage(pCombo->m_hWnd, kCbDeleteString, static_cast<WPARAM>(nIndex), 0);
    }
    if (nIndex == SelIndex(this)) {
        int iSel = SelIndex(this);
        const int nCount = static_cast<int>(state.items.size());
        if (iSel >= nCount) iSel = nCount - 1;
        SelectItem(iSel, FALSE);
    }
    return TRUE;
}
// Symbol: ?DeleteItem@CMFCToolBarComboBoxButton@@QEAAH_K@Z
BOOL CMFCToolBarComboBoxButton::DeleteItem(DWORD_PTR dwData) {
    RegisterComboButton(this);
    ComboButtonState& state = EnsureComboState(this);
    for (int i = 0; i < static_cast<int>(state.items.size()); ++i) {
        if (state.items[i].second == dwData) {
            return DeleteItem(i);
        }
    }
    return FALSE;
}
// Symbol: ?DeleteItem@CMFCToolBarComboBoxButton@@QEAAHPEB_W@Z
BOOL CMFCToolBarComboBoxButton::DeleteItem(const wchar_t* lpszText) {
    const int index = FindItem(lpszText);
    return (index != CB_ERR) ? DeleteItem(index) : FALSE;
}
// Symbol: ?FindItem@CMFCToolBarComboBoxButton@@QEBAHPEB_W@Z
int CMFCToolBarComboBoxButton::FindItem(const wchar_t* lpszText) const {
    if (!lpszText) return CB_ERR;
    const ComboButtonState* state = FindComboState(this);
    if (!state) return CB_ERR;
    for (int i = 0; i < static_cast<int>(state->items.size()); ++i) {
        if (CompareComboText(static_cast<const wchar_t*>(state->items[i].first), lpszText) == 0) {
            return i;
        }
    }
    return CB_ERR;
}
// Symbol: ?GetItem@CMFCToolBarComboBoxButton@@QEBAPEB_WH@Z
const wchar_t* CMFCToolBarComboBoxButton::GetItem(int nIndex) const {
    const ComboButtonState* state = FindComboState(this);
    if (!state) return nullptr;
    if (nIndex < 0) nIndex = SelIndex(this);
    if (nIndex < 0 || nIndex >= static_cast<int>(state->items.size())) return nullptr;
    return state->items[nIndex].first;
}
// Symbol: ?GetCount@CMFCToolBarComboBoxButton@@QEBA_JXZ
INT_PTR CMFCToolBarComboBoxButton::GetCount() const {
    const ComboButtonState* state = FindComboState(this);
    return state ? static_cast<INT_PTR>(state->items.size()) : 0;
}
// Symbol: ?GetItemData@CMFCToolBarComboBoxButton@@QEBA_KH@Z
DWORD_PTR CMFCToolBarComboBoxButton::GetItemData(int nIndex) const {
    const ComboButtonState* state = FindComboState(this);
    if (!state) return 0;
    if (nIndex < 0) nIndex = SelIndex(this);
    if (nIndex < 0 || nIndex >= static_cast<int>(state->items.size())) return 0;
    return state->items[nIndex].second;
}
// Symbol: ?SelectItem@CMFCToolBarComboBoxButton@@QEAAHPEB_W@Z
BOOL CMFCToolBarComboBoxButton::SelectItem(const wchar_t* lpszItem) {
    const int index = FindItem(lpszItem);
    return (index != CB_ERR) ? SelectItem(index, TRUE) : FALSE;
}
// Retail (RVA 0x163b80, mfc140u), fully transcribed:
//     if (iIndex >= m_lstItems.GetCount()) return FALSE;          // +0xf8 (m_lstItems.m_nCount)
//     m_iSelIndex = iIndex < -1 ? -1 : iIndex;                    // +0x8c
//     if (m_pWndCombo != NULL && m_pWndCombo->m_hWnd != NULL) {
//         if (m_iSelIndex >= 0) m_pWndCombo->GetLBText(iIndex, m_strEdit);   // 0x294250
//         else                  m_strEdit.Empty();
//         if (m_pWndEdit != NULL) {
//             CString str; m_pWndEdit->GetWindowText(str);        // 0x28be00
//             if (wcscmp(str, m_strEdit) != 0) m_pWndEdit->SetWindowText(m_strEdit);   // 0x2a9790
//         }
//         if (m_pWndCombo->GetCurSel() != iIndex) {               // CB_GETCURSEL
//             if (m_pWndCombo->SetCurSel(iIndex) == CB_ERR) return FALSE;   // CB_SETCURSEL
//             if (bNotify) NotifyCommand(CBN_SELENDOK);           // vslot 0x90 (18)
//         }
//     }
//     return TRUE;
// The count comes from the side table (file header); NotifyCommand is reached
// through this file's thunk rather than the vftable.
// Symbol: ?SelectItem@CMFCToolBarComboBoxButton@@QEAAHHH@Z
BOOL CMFCToolBarComboBoxButton::SelectItem(int nIndex, BOOL bNotify) {
    RegisterComboButton(this);
    if (nIndex >= static_cast<int>(GetCount())) return FALSE;
    SelIndex(this) = (nIndex < -1) ? -1 : nIndex;
    CComboBox* pCombo = WndCombo(this);
    if (HasWindow(pCombo)) {
        if (SelIndex(this) >= 0) {
            impl__GetLBText_CComboBox__QEBAXHAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(pCombo, nIndex, &StrEdit(this));
        } else {
            StrEdit(this).Empty();
        }
        if (CEdit* pEdit = WndEdit(this)) {
            CString str;
            impl__GetWindowTextW_CWnd__QEBAXAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(pEdit, &str);
            if (wcscmp(static_cast<const wchar_t*>(str), static_cast<const wchar_t*>(StrEdit(this))) != 0) {
                impl__SetWindowTextW_CWnd__QEAAXPEB_W_Z(pEdit, static_cast<const wchar_t*>(StrEdit(this)));
            }
        }
        if (static_cast<int>(::SendMessage(pCombo->m_hWnd, kCbGetCurSel, 0, 0)) != nIndex) {
            if (static_cast<int>(::SendMessage(pCombo->m_hWnd, kCbSetCurSel, static_cast<WPARAM>(nIndex), 0)) == CB_ERR) return FALSE;
            if (bNotify) impl__NotifyCommand_CMFCToolBarComboBoxButton__UEAAHH_Z(this, kCbnSelEndOk);
        }
    }
    return TRUE;
}
// Symbol: ?SelectItem@CMFCToolBarComboBoxButton@@QEAAH_K@Z
BOOL CMFCToolBarComboBoxButton::SelectItem(DWORD_PTR dwData) {
    RegisterComboButton(this);
    ComboButtonState& state = EnsureComboState(this);
    for (int i = 0; i < static_cast<int>(state.items.size()); ++i) {
        if (state.items[i].second == dwData) {
            return SelectItem(i, TRUE);
        }
    }
    return FALSE;
}
// Retail (RVA 0x164450, mfc140u), fully transcribed:
//     if (lpszText == NULL) AfxThrowInvalidArgException();
//     if (SelectItem(lpszText)) return;                           // 0x163d40
//     m_strEdit = lpszText;                                       // SetString(wcslen)
//     if (m_pWndCombo != NULL && !m_bFlat) {
//         CString str; m_pWndCombo->GetWindowText(str);
//         if (wcscmp(str, lpszText) != 0) { m_pWndCombo->SetWindowText(lpszText); NotifyCommand(CBN_EDITCHANGE); }   // vslot 0x90
//     }
//     if (m_pWndEdit != NULL) {
//         CString str; m_pWndEdit->GetWindowText(str);
//         if (wcscmp(str, lpszText) != 0) m_pWndEdit->SetWindowText(lpszText);
//     }
// DEVIATION: a NULL text returns instead of throwing (the invalid-argument
// exception is not raised from this file).
// Symbol: ?SetText@CMFCToolBarComboBoxButton@@QEAAXPEB_W@Z
void CMFCToolBarComboBoxButton::SetText(const wchar_t* lpszText) {
    if (lpszText == nullptr) return;
    RegisterComboButton(this);
    if (SelectItem(lpszText)) return;
    StrEdit(this) = lpszText;
    if (CComboBox* pCombo = WndCombo(this)) {
        if (impl__m_bFlat_CMFCToolBarComboBoxButton__1HA == 0) {
            CString str;
            impl__GetWindowTextW_CWnd__QEBAXAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(pCombo, &str);
            if (wcscmp(static_cast<const wchar_t*>(str), lpszText) != 0) {
                impl__SetWindowTextW_CWnd__QEAAXPEB_W_Z(pCombo, lpszText);
                impl__NotifyCommand_CMFCToolBarComboBoxButton__UEAAHH_Z(this, kCbnEditChange);
            }
        }
    }
    if (CEdit* pEdit = WndEdit(this)) {
        CString str;
        impl__GetWindowTextW_CWnd__QEBAXAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(pEdit, &str);
        if (wcscmp(static_cast<const wchar_t*>(str), lpszText) != 0) {
            impl__SetWindowTextW_CWnd__QEAAXPEB_W_Z(pEdit, lpszText);
        }
    }
}
// Static methods
// Symbol: ?GetByCmd@CMFCToolBarComboBoxButton@@SAPEAV1@IH@Z
CMFCToolBarComboBoxButton* CMFCToolBarComboBoxButton::GetByCmd(UINT uiCmd, BOOL) {
    const auto buttons = GetComboButtonsByCmd(uiCmd);
    return buttons.empty() ? nullptr : buttons.front();
}
// Symbol: ?GetCountAll@CMFCToolBarComboBoxButton@@SAHI@Z
int CMFCToolBarComboBoxButton::GetCountAll(UINT uiCmd) {
    CMFCToolBarComboBoxButton* pButton = GetByCmd(uiCmd, FALSE);
    return pButton ? static_cast<int>(pButton->GetCount()) : 0;
}
// Symbol: ?GetCurSelAll@CMFCToolBarComboBoxButton@@SAHI@Z
int CMFCToolBarComboBoxButton::GetCurSelAll(UINT uiCmd) {
    CMFCToolBarComboBoxButton* pButton = GetByCmd(uiCmd, FALSE);
    if (!pButton) return CB_ERR;
    return SelIndex(pButton);
}
// Symbol: ?GetItemAll@CMFCToolBarComboBoxButton@@SAPEB_WIH@Z
const wchar_t* CMFCToolBarComboBoxButton::GetItemAll(UINT uiCmd, int iIndex) {
    CMFCToolBarComboBoxButton* pButton = GetByCmd(uiCmd, FALSE);
    return pButton ? pButton->GetItem(iIndex) : nullptr;
}
// Symbol: ?GetItemDataAll@CMFCToolBarComboBoxButton@@SA_KIH@Z
DWORD_PTR CMFCToolBarComboBoxButton::GetItemDataAll(UINT uiCmd, int iIndex) {
    CMFCToolBarComboBoxButton* pButton = GetByCmd(uiCmd, FALSE);
    return pButton ? pButton->GetItemData(iIndex) : 0;
}
// Retail (RVA 0x164840, mfc140u): `p = GetByCmd(uiCmd, FALSE); return p ? p->m_strEdit : NULL;`
// Symbol: ?GetTextAll@CMFCToolBarComboBoxButton@@SAPEB_WI@Z
const wchar_t* CMFCToolBarComboBoxButton::GetTextAll(UINT uiCmd) {
    CMFCToolBarComboBoxButton* pButton = GetByCmd(uiCmd, FALSE);
    return pButton ? static_cast<const wchar_t*>(StrEdit(pButton)) : nullptr;
}
// Symbol: ?SelectItemAll@CMFCToolBarComboBoxButton@@SAHIPEB_W@Z
BOOL CMFCToolBarComboBoxButton::SelectItemAll(UINT uiCmd, const wchar_t* lpszText) {
    BOOL result = FALSE;
    for (CMFCToolBarComboBoxButton* button : GetComboButtonsByCmd(uiCmd)) {
        if (button && button->SelectItem(lpszText)) result = TRUE;
    }
    return result;
}
// Symbol: ?SelectItemAll@CMFCToolBarComboBoxButton@@SAHIH@Z
BOOL CMFCToolBarComboBoxButton::SelectItemAll(UINT uiCmd, int nIndex) {
    BOOL result = FALSE;
    for (CMFCToolBarComboBoxButton* button : GetComboButtonsByCmd(uiCmd)) {
        if (button && button->SelectItem(nIndex, TRUE)) result = TRUE;
    }
    return result;
}
// Symbol: ?SelectItemAll@CMFCToolBarComboBoxButton@@SAHI_K@Z
BOOL CMFCToolBarComboBoxButton::SelectItemAll(UINT uiCmd, DWORD_PTR dwData) {
    BOOL result = FALSE;
    for (CMFCToolBarComboBoxButton* button : GetComboButtonsByCmd(uiCmd)) {
        if (button && button->SelectItem(dwData)) result = TRUE;
    }
    return result;
}
// Symbol: ?AddItem@CMFCToolBarComboBoxButton@@UEAA_JPEB_W_K@Z
extern "C" INT_PTR MS_ABI impl__AddItem_CMFCToolBarComboBoxButton__UEAA_JPEB_W_K_Z(CMFCToolBarComboBoxButton* pThis, const wchar_t* p0, DWORD_PTR p1) {
    return pThis->AddItem(p0, p1);
}
// Symbol: ?AddSortedItem@CMFCToolBarComboBoxButton@@UEAA_JPEB_W_K@Z
extern "C" INT_PTR MS_ABI impl__AddSortedItem_CMFCToolBarComboBoxButton__UEAA_JPEB_W_K_Z(CMFCToolBarComboBoxButton* pThis, const wchar_t* p0, DWORD_PTR p1) {
    return pThis->AddSortedItem(p0, p1);
}
// Symbol: ?Compare@CMFCToolBarComboBoxButton@@UEAAHPEB_W0@Z
extern "C" int MS_ABI impl__Compare_CMFCToolBarComboBoxButton__UEAAHPEB_W0_Z(CMFCToolBarComboBoxButton* pThis, const wchar_t* p0, const wchar_t* p1) {
    return pThis->Compare(p0, p1);
}
// Symbol: ?DeleteItem@CMFCToolBarComboBoxButton@@QEAAHH@Z
extern "C" int MS_ABI impl__DeleteItem_CMFCToolBarComboBoxButton__QEAAHH_Z(CMFCToolBarComboBoxButton* pThis, int p0) {
    return pThis->DeleteItem(p0);
}
// Symbol: ?DeleteItem@CMFCToolBarComboBoxButton@@QEAAH_K@Z
extern "C" int MS_ABI impl__DeleteItem_CMFCToolBarComboBoxButton__QEAAH_K_Z(CMFCToolBarComboBoxButton* pThis, DWORD_PTR p0) {
    return pThis->DeleteItem(p0);
}
// Symbol: ?DeleteItem@CMFCToolBarComboBoxButton@@QEAAHPEB_W@Z
extern "C" int MS_ABI impl__DeleteItem_CMFCToolBarComboBoxButton__QEAAHPEB_W_Z(CMFCToolBarComboBoxButton* pThis, const wchar_t* p0) {
    return pThis->DeleteItem(p0);
}
// Symbol: ?FindItem@CMFCToolBarComboBoxButton@@QEBAHPEB_W@Z
extern "C" int MS_ABI impl__FindItem_CMFCToolBarComboBoxButton__QEBAHPEB_W_Z(const CMFCToolBarComboBoxButton* pThis, const wchar_t* p0) {
    return pThis->FindItem(p0);
}
// Symbol: ?GetByCmd@CMFCToolBarComboBoxButton@@SAPEAV1@IH@Z
extern "C" CMFCToolBarComboBoxButton* MS_ABI impl__GetByCmd_CMFCToolBarComboBoxButton__SAPEAV1_IH_Z(unsigned int p0, int p1) {
    return CMFCToolBarComboBoxButton::GetByCmd(p0, (BOOL)p1);
}
// Symbol: ?GetCount@CMFCToolBarComboBoxButton@@QEBA_JXZ
extern "C" INT_PTR MS_ABI impl__GetCount_CMFCToolBarComboBoxButton__QEBA_JXZ(const CMFCToolBarComboBoxButton* pThis) {
    return pThis->GetCount();
}
// Symbol: ?GetCountAll@CMFCToolBarComboBoxButton@@SAHI@Z
extern "C" int MS_ABI impl__GetCountAll_CMFCToolBarComboBoxButton__SAHI_Z(unsigned int p0) {
    return CMFCToolBarComboBoxButton::GetCountAll(p0);
}
// Symbol: ?GetCurSelAll@CMFCToolBarComboBoxButton@@SAHI@Z
extern "C" int MS_ABI impl__GetCurSelAll_CMFCToolBarComboBoxButton__SAHI_Z(unsigned int p0) {
    return CMFCToolBarComboBoxButton::GetCurSelAll(p0);
}
// Symbol: ?GetItem@CMFCToolBarComboBoxButton@@QEBAPEB_WH@Z
extern "C" const wchar_t* MS_ABI impl__GetItem_CMFCToolBarComboBoxButton__QEBAPEB_WH_Z(const CMFCToolBarComboBoxButton* pThis, int p0) {
    return pThis->GetItem(p0);
}
// Symbol: ?GetItemAll@CMFCToolBarComboBoxButton@@SAPEB_WIH@Z
extern "C" const wchar_t* MS_ABI impl__GetItemAll_CMFCToolBarComboBoxButton__SAPEB_WIH_Z(unsigned int p0, int p1) {
    return CMFCToolBarComboBoxButton::GetItemAll(p0, p1);
}
// Symbol: ?GetItemData@CMFCToolBarComboBoxButton@@QEBA_KH@Z
extern "C" DWORD_PTR MS_ABI impl__GetItemData_CMFCToolBarComboBoxButton__QEBA_KH_Z(const CMFCToolBarComboBoxButton* pThis, int p0) {
    return pThis->GetItemData(p0);
}
// Symbol: ?GetItemDataAll@CMFCToolBarComboBoxButton@@SA_KIH@Z
extern "C" DWORD_PTR MS_ABI impl__GetItemDataAll_CMFCToolBarComboBoxButton__SA_KIH_Z(unsigned int p0, int p1) {
    return CMFCToolBarComboBoxButton::GetItemDataAll(p0, p1);
}
// Symbol: ?GetTextAll@CMFCToolBarComboBoxButton@@SAPEB_WI@Z
extern "C" const wchar_t* MS_ABI impl__GetTextAll_CMFCToolBarComboBoxButton__SAPEB_WI_Z(unsigned int p0) {
    return CMFCToolBarComboBoxButton::GetTextAll(p0);
}
// Symbol: ?SelectItem@CMFCToolBarComboBoxButton@@QEAAHPEB_W@Z
extern "C" int MS_ABI impl__SelectItem_CMFCToolBarComboBoxButton__QEAAHPEB_W_Z(CMFCToolBarComboBoxButton* pThis, const wchar_t* p0) {
    return pThis->SelectItem(p0);
}
// Symbol: ?SelectItem@CMFCToolBarComboBoxButton@@QEAAHHH@Z
extern "C" int MS_ABI impl__SelectItem_CMFCToolBarComboBoxButton__QEAAHHH_Z(CMFCToolBarComboBoxButton* pThis, int p0, int p1) {
    return pThis->SelectItem(p0, (BOOL)p1);
}
// Symbol: ?SelectItem@CMFCToolBarComboBoxButton@@QEAAH_K@Z
extern "C" int MS_ABI impl__SelectItem_CMFCToolBarComboBoxButton__QEAAH_K_Z(CMFCToolBarComboBoxButton* pThis, DWORD_PTR p0) {
    return pThis->SelectItem(p0);
}
// Symbol: ?SelectItemAll@CMFCToolBarComboBoxButton@@SAHIPEB_W@Z
extern "C" int MS_ABI impl__SelectItemAll_CMFCToolBarComboBoxButton__SAHIPEB_W_Z(unsigned int p0, const wchar_t* p1) {
    return CMFCToolBarComboBoxButton::SelectItemAll(p0, p1);
}
// Symbol: ?SelectItemAll@CMFCToolBarComboBoxButton@@SAHIH@Z
extern "C" int MS_ABI impl__SelectItemAll_CMFCToolBarComboBoxButton__SAHIH_Z(unsigned int p0, int p1) {
    return CMFCToolBarComboBoxButton::SelectItemAll(p0, p1);
}
// Symbol: ?SelectItemAll@CMFCToolBarComboBoxButton@@SAHI_K@Z
extern "C" int MS_ABI impl__SelectItemAll_CMFCToolBarComboBoxButton__SAHI_K_Z(unsigned int p0, DWORD_PTR p1) {
    return CMFCToolBarComboBoxButton::SelectItemAll(p0, p1);
}
// Symbol: ?SetText@CMFCToolBarComboBoxButton@@QEAAXPEB_W@Z
extern "C" void MS_ABI impl__SetText_CMFCToolBarComboBoxButton__QEAAXPEB_W_Z(CMFCToolBarComboBoxButton* pThis, const wchar_t* p0) {
    pThis->SetText(p0);
}

// ===========================================================================
// Construction / destruction
// ===========================================================================

// Retail ??0CMFCToolBarComboBoxButton@@QEAA@XZ (RVA 0x1614d0, mfc140u):
//     CMFCToolBarButton::CMFCToolBarButton();
//     vfptr = &CMFCToolBarComboBoxButton::`vftable';
//     m_rectCombo = m_rectButton = {0};
//     m_strEdit = CString();                       // the nil string
//     m_lstItems / m_lstItemData constructed (block size 10)
//     m_dwStyle = 0x50200403;                      // WS_CHILD|WS_VISIBLE|WS_VSCROLL|CBS_NOINTEGRALHEIGHT|CBS_DROPDOWNLIST
//     m_iWidth  = 150;
//     Initialize();                                // 0x161740
// The (UINT, int, DWORD, int) constructor (RVA 0x161620) differs only in
// `CMFCToolBarButton(uiID, iImage, NULL, FALSE, FALSE)`, `m_dwStyle = dwStyle |
// 0x50200000` and `m_iWidth = iWidth ? iWidth : 150`; it is declared inline in
// include/openmfc/afxmfc.h as a delegation to this constructor that DROPS all
// four arguments (see headerRequests).  The vfptr is left as mingw builds it.
CMFCToolBarComboBoxButton::CMFCToolBarComboBoxButton() {
    memset(_pad, 0, sizeof(_pad));
    m_bFlat = 0;          // the header's alias of m_iWidth
    m_bCenterVert = 0;    // the header's alias of m_iSelIndex
    new (&StrEdit(this)) CString();
    Style(this) = kDefaultComboStyle;
    Width(this) = kDefaultWidth;
    impl__Initialize_CMFCToolBarComboBoxButton__IEAAXXZ(this);
}
// Retail ??1CMFCToolBarComboBoxButton@@UEAA@XZ (RVA 0x1617c0, mfc140u):
//     if (m_pWndCombo) { m_pWndCombo->DestroyWindow(); delete m_pWndCombo; }   // vslots 0xd0, 0x08(1)
//     if (m_pWndEdit)  { m_pWndEdit->DestroyWindow();  delete m_pWndEdit;  }
//     m_lstItemData.~CList(); m_lstItems.~CStringList(); m_strEdit.~CString();
//     CMFCToolBarButton::~CMFCToolBarButton();
// UnregisterComboButton drops the side-table entries (the two lists).
CMFCToolBarComboBoxButton::~CMFCToolBarComboBoxButton() {
    if (CComboBox* pCombo = WndCombo(this)) {
        DestroyOwnedWindow(pCombo);
        DeleteOwnedWindow(pCombo);
        WndCombo(this) = nullptr;
    }
    if (CEdit* pEdit = WndEdit(this)) {
        DestroyOwnedWindow(pEdit);
        DeleteOwnedWindow(pEdit);
        WndEdit(this) = nullptr;
    }
    UnregisterComboButton(this);
    StrEdit(this).~CString();
}

// Retail (RVA 0x161740, mfc140u), fully transcribed:
//     m_iSelIndex = -1;                    // +0x8c
//     m_pWndCombo = NULL;                  // +0xd0
//     m_pWndEdit  = NULL;                  // +0xc8
//     m_bHorz     = TRUE;                  // +0x94
//     ::SetRectEmpty(&m_rectCombo);        // +0xa8
//     ::SetRectEmpty(&m_rectButton);       // +0xb8
//     m_nDropDownHeight = 150;             // +0x90
//     m_bIsHotEdit = m_bIsRibbon = FALSE;  // +0x98 / +0x9c (one 8-byte store)
//     m_uiMenuResID = 0;                   // +0xa0
// Symbol: ?Initialize@CMFCToolBarComboBoxButton@@IEAAXXZ
extern "C" void MS_ABI impl__Initialize_CMFCToolBarComboBoxButton__IEAAXXZ(CMFCToolBarComboBoxButton* pThis) {
    if (!pThis) return;
    SelIndex(pThis) = -1;
    WndCombo(pThis) = nullptr;
    WndEdit(pThis) = nullptr;
    Horz(pThis) = TRUE;
    ::SetRectEmpty(&RectCombo(pThis));
    ::SetRectEmpty(&RectButton(pThis));
    DropDownHeight(pThis) = kDefaultDropDownHeight;
    IsHotEdit(pThis) = FALSE;
    IsRibbon(pThis) = FALSE;
    MenuResID(pThis) = 0;
}

// ===========================================================================
// Layout
// ===========================================================================

// Retail (RVA 0x162be0, mfc140u), fully transcribed:
//     if (m_pWndCombo == NULL || m_pWndCombo->m_hWnd == NULL ||
//         ::IsRectEmpty(&m_rect) || !m_bHorz) {
//         ::SetRectEmpty(&m_rectCombo); ::SetRectEmpty(&m_rectButton); return;
//     }
//     if (m_bCenterVert && (!m_bTextBelow || m_strText.IsEmpty())) {  // 0x3b1b70, +0x1c, +0x38
//         CWnd* pToolBar = NULL;
//         for (CWnd* p = m_pWndCombo->GetParent(); p != NULL; p = p->GetParent())
//             if (p->IsKindOf(RUNTIME_CLASS(CMFCToolBar))) { pToolBar = p; break; }   // RTC 0x3b15f8
//         if (pToolBar != NULL) {
//             int dy = (m_rect.top - m_rect.bottom + pToolBar->GetRowHeight()) / 2;    // vslot 0x6b8 (215)
//             if (dy < 0) dy = 0;
//             ::OffsetRect(&m_rectButton, 0, dy); ::OffsetRect(&m_rectCombo, 0, dy); ::OffsetRect(&m_rect, 0, dy);
//         }
//     }
//     m_pWndCombo->SetWindowPos(NULL, m_rect.left + 1, m_rect.top, m_rect.Width() - 2, m_nDropDownHeight,
//                               SWP_NOZORDER | SWP_NOACTIVATE);                  // 0x2a9a60
//     m_pWndCombo->SetEditSel(-1, 0);                                             // CB_SETEDITSEL, lParam 0xffff
//     ::GetWindowRect(m_pWndCombo->m_hWnd, &m_rectCombo);
//     m_pWndCombo->ScreenToClient(&m_rectCombo);                                  // 0x2a32b0
//     ::MapWindowPoints(m_pWndCombo->m_hWnd, m_pWndCombo->GetParent()->m_hWnd (NULL if none), (POINT*)&m_rectCombo, 2);
//     if (m_bFlat) {                                                              // 0x3b1b74
//         m_rectButton = m_rectCombo;
//         m_rectButton.left = m_rectButton.right - 2 * CMenuImages::Size().cx;    // 0x8f600
//         ::InflateRect(&m_rectButton, -2, -2);
//         m_rect.left  = m_rectCombo.left - 1;
//         m_rect.right = m_rectCombo.right + 1;
//         if (!m_bTextBelow || m_strText.IsEmpty()) { m_rect.top = m_rectCombo.top; m_rect.bottom = m_rectCombo.bottom; }   // +0x1c
//         if (m_pWndEdit != NULL)
//             m_pWndEdit->SetWindowPos(NULL, m_rect.left + 4, m_rect.top + 3,
//                                      m_rect.Width() - 8 - (m_rectButton.right - m_rectButton.left),
//                                      m_rectCombo.bottom - m_rectCombo.top - 6, SWP_NOZORDER | SWP_NOACTIVATE);
//     } else {
//         ::SetRectEmpty(&m_rectButton);
//     }
// The parent walk goes through CWnd::FromHandle(::GetParent()) as retail
// inlines GetParent(); GetRowHeight goes through the exported CMFCToolBar
// thunk instead of the vftable.
// Symbol: ?AdjustRect@CMFCToolBarComboBoxButton@@IEAAXXZ
extern "C" void MS_ABI impl__AdjustRect_CMFCToolBarComboBoxButton__IEAAXXZ(CMFCToolBarComboBoxButton* pThis) {
    if (!pThis) return;
    CComboBox* pCombo = WndCombo(pThis);
    CRect& rect = pThis->m_rect;
    RECT* const pRect = static_cast<RECT*>(rect);    // CRect::operator RECT*()
    if (!HasWindow(pCombo) || ::IsRectEmpty(pRect) || Horz(pThis) == 0) {
        ::SetRectEmpty(&RectCombo(pThis));
        ::SetRectEmpty(&RectButton(pThis));
        return;
    }
    if (impl__m_bCenterVert_CMFCToolBarComboBoxButton__1HA != 0 &&
        (pThis->m_bTextBelow == 0 || pThis->m_strText.GetLength() == 0)) {
        CWnd* pToolBar = nullptr;
        for (CWnd* p = ParentOf(pCombo); p != nullptr; p = ParentOf(p)) {
            if (impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(p, impl__GetThisClass_CMFCToolBar__SAPEAUCRuntimeClass__XZ())) {
                pToolBar = p;
                break;
            }
        }
        if (pToolBar != nullptr) {
            const int rowHeight = impl__GetRowHeight_CMFCToolBar__UEBAHXZ(static_cast<const CMFCToolBar*>(pToolBar));
            int dy = (rect.top - rect.bottom + rowHeight) / 2;
            if (dy < 0) dy = 0;
            ::OffsetRect(&RectButton(pThis), 0, dy);
            ::OffsetRect(&RectCombo(pThis), 0, dy);
            ::OffsetRect(pRect, 0, dy);
        }
    }
    impl__SetWindowPos_CWnd__QEAAHPEBV1_HHHHI_Z(pCombo, nullptr, rect.left + 1, rect.top,
                                                rect.right - rect.left - 2, DropDownHeight(pThis),
                                                kSwpNoZorderNoActivate);
    ::SendMessage(pCombo->m_hWnd, kCbSetEditSel, 0, static_cast<LPARAM>(0xffff));
    RECT& rectCombo = RectCombo(pThis);
    ::GetWindowRect(pCombo->m_hWnd, &rectCombo);
    impl__ScreenToClient_CWnd__QEBAXPEAUtagRECT___Z(pCombo, &rectCombo);
    CWnd* pParent = ParentOf(pCombo);
    ::MapWindowPoints(pCombo->m_hWnd, HWndOf(pParent), reinterpret_cast<POINT*>(&rectCombo), 2);

    RECT& rectButton = RectButton(pThis);
    if (impl__m_bFlat_CMFCToolBarComboBoxButton__1HA != 0) {
        rectButton = rectCombo;
        rectButton.left = rectButton.right - 2 * MenuImageExtent();
        ::InflateRect(&rectButton, -2, -2);
        rect.left = rectCombo.left - 1;
        rect.right = rectCombo.right + 1;
        if (pThis->m_bTextBelow == 0 || pThis->m_strText.GetLength() == 0) {
            rect.top = rectCombo.top;
            rect.bottom = rectCombo.bottom;
        }
        if (CEdit* pEdit = WndEdit(pThis)) {
            impl__SetWindowPos_CWnd__QEAAHPEBV1_HHHHI_Z(pEdit, nullptr, rect.left + 4, rect.top + 3,
                                                        (rect.right - rect.left) - 8 - (rectButton.right - rectButton.left),
                                                        rectCombo.bottom - rectCombo.top - 6,
                                                        kSwpNoZorderNoActivate);
        }
    } else {
        ::SetRectEmpty(&rectButton);
    }
}

// Retail (RVA 0x1621e0, mfc140u), fully transcribed:
//     if (m_pWndCombo != NULL && m_pWndCombo->m_hWnd != NULL) AdjustRect();   // 0x162be0
// Symbol: ?OnMove@CMFCToolBarComboBoxButton@@UEAAXXZ
extern "C" void MS_ABI impl__OnMove_CMFCToolBarComboBoxButton__UEAAXXZ(CMFCToolBarComboBoxButton* pThis) {
    if (!pThis) return;
    if (HasWindow(WndCombo(pThis))) impl__AdjustRect_CMFCToolBarComboBoxButton__IEAAXXZ(pThis);
}

// Retail (RVA 0x162210, mfc140u), fully transcribed:
//     m_iWidth = iSize;                              // +0x88
//     m_rect.right = m_rect.left + iSize;            // +0x70 = +0x68 + iSize
//     if (m_pWndCombo != NULL && m_pWndCombo->m_hWnd != NULL) AdjustRect();
// Symbol: ?OnSize@CMFCToolBarComboBoxButton@@UEAAXH@Z
extern "C" void MS_ABI impl__OnSize_CMFCToolBarComboBoxButton__UEAAXH_Z(CMFCToolBarComboBoxButton* pThis, int iSize) {
    if (!pThis) return;
    Width(pThis) = iSize;
    pThis->m_rect.right = pThis->m_rect.left + iSize;
    if (HasWindow(WndCombo(pThis))) impl__AdjustRect_CMFCToolBarComboBoxButton__IEAAXXZ(pThis);
}

// Retail (RVA 0x164420, mfc140u), fully transcribed:
//     if (m_nDropDownHeight != nHeight) { m_nDropDownHeight = nHeight; OnMove(); }   // vslot 0x70 (14)
// OnMove is reached through this file's thunk rather than the vftable.
// Symbol: ?SetDropDownHeight@CMFCToolBarComboBoxButton@@QEAAXH@Z
extern "C" void MS_ABI impl__SetDropDownHeight_CMFCToolBarComboBoxButton__QEAAXH_Z(CMFCToolBarComboBoxButton* pThis, int nHeight) {
    if (!pThis) return;
    if (DropDownHeight(pThis) != nHeight) {
        DropDownHeight(pThis) = nHeight;
        impl__OnMove_CMFCToolBarComboBoxButton__UEAAXXZ(pThis);
    }
}

// Retail (RVA 0x161ef0, mfc140u), fully transcribed.  SIZE comes back through
// the hidden pointer in RDX (pDC in R8, sizeDefault in R9, bHorz on the stack):
//     m_bHorz = bHorz;  m_sizeText = CSize(0, 0);                    // +0x94, +0x78
//     if (!m_bVisible) {                                              // +0x64
//         if (m_bFlat && m_pWndEdit->GetSafeHwnd() && (m_pWndEdit->GetStyle() & WS_VISIBLE)) m_pWndEdit->ShowWindow(SW_HIDE);
//         if (m_pWndCombo->GetSafeHwnd() && (m_pWndCombo->GetStyle() & WS_VISIBLE)) m_pWndCombo->ShowWindow(SW_HIDE);
//         return CSize(0, 0);
//     }
//     if (m_bFlat && m_pWndCombo->GetSafeHwnd() && (m_pWndCombo->GetStyle() & WS_VISIBLE)) m_pWndCombo->ShowWindow(SW_HIDE);
//     if (!bHorz) {
//         if (m_pWndCombo->GetSafeHwnd() && (GetStyle() & WS_VISIBLE)) m_pWndCombo->ShowWindow(SW_HIDE);
//         if (m_pWndEdit->GetSafeHwnd()  && (GetStyle() & WS_VISIBLE)) m_pWndEdit->ShowWindow(SW_HIDE);
//         return CMFCToolBarButton::OnCalculateSize(pDC, sizeDefault, FALSE);    // 0x15d680
//     }
//     if (!m_bFlat && m_pWndCombo->GetSafeHwnd() && !m_bIsHidden) m_pWndCombo->ShowWindow(SW_SHOWNOACTIVATE);   // +0x54
//     if (m_bTextBelow && !m_strText.IsEmpty()) {                     // +0x1c, +0x38
//         CRect rectText(0, 0, m_iWidth, sizeDefault.cy);
//         pDC->DrawText(m_strText, rectText, DT_CALCRECT | DT_CENTER | DT_WORDBREAK);   // CDC vslot 0xe0 (28)
//         m_sizeText = rectText.Size();
//     }
//     int cy = sizeDefault.cy;
//     if (m_pWndCombo->GetSafeHwnd()) {
//         if (afxGlobalData.bIsWindows7 /*+0x258*/) { COMBOBOXINFO cbi = {sizeof cbi}; ::GetComboBoxInfo(m_hWnd, &cbi); cy = cbi.rcButton.bottom - cbi.rcButton.top; }
//         else                                       { RECT rc; ::GetWindowRect(m_hWnd, &rc); cy = rc.bottom - rc.top; }
//     }
//     if (!m_bIsHidden && m_pWndEdit->GetSafeHwnd() && m_pWndCombo && !(m_pWndCombo->GetStyle() & WS_VISIBLE))
//         m_pWndEdit->ShowWindow(SW_SHOWNOACTIVATE);
//     return CSize(m_iWidth, m_sizeText.cy + cy);
// DEVIATIONS: the base OnCalculateSize is a null stub in OpenMFC
// (featurepack/toolbar/CMFCToolBarButton.cpp), so the vertical case yields
// (0, 0); DrawText goes through the exported CDC thunk; afxGlobalData's
// "Windows 7" flag reads 0 (Initialize is a stub) so the GetWindowRect branch
// is the one taken.
// Symbol: ?OnCalculateSize@CMFCToolBarComboBoxButton@@UEAA?AUtagSIZE@@PEAVCDC@@AEBVCSize@@H@Z
extern "C" SIZE* MS_ABI impl__OnCalculateSize_CMFCToolBarComboBoxButton__UEAA_AUtagSIZE__PEAVCDC__AEBVCSize__H_Z(
    CMFCToolBarComboBoxButton* pThis, SIZE* pRet, CDC* pDC, const SIZE* pSizeDefault, int bHorz)
{
    if (!pRet) return pRet;
    pRet->cx = 0; pRet->cy = 0;
    if (!pThis || !pSizeDefault) return pRet;

    Horz(pThis) = bHorz;
    pThis->m_sizeText.cx = 0;
    pThis->m_sizeText.cy = 0;
    CComboBox* pCombo = WndCombo(pThis);
    CEdit* pEdit = WndEdit(pThis);

    if (pThis->m_bVisible == 0) {
        if (impl__m_bFlat_CMFCToolBarComboBoxButton__1HA != 0 && HasWindow(pEdit) && StyleVisible(pEdit)) {
            impl__ShowWindow_CWnd__QEAAHH_Z(pEdit, SW_HIDE);
        }
        if (HasWindow(pCombo) && StyleVisible(pCombo)) impl__ShowWindow_CWnd__QEAAHH_Z(pCombo, SW_HIDE);
        return pRet;
    }
    if (impl__m_bFlat_CMFCToolBarComboBoxButton__1HA != 0 && HasWindow(pCombo) && StyleVisible(pCombo)) {
        impl__ShowWindow_CWnd__QEAAHH_Z(pCombo, SW_HIDE);
    }
    if (bHorz == 0) {
        if (HasWindow(pCombo) && StyleVisible(pCombo)) impl__ShowWindow_CWnd__QEAAHH_Z(pCombo, SW_HIDE);
        if (HasWindow(pEdit) && StyleVisible(pEdit)) impl__ShowWindow_CWnd__QEAAHH_Z(pEdit, SW_HIDE);
        impl__OnCalculateSize_CMFCToolBarButton__UEAA_AUtagSIZE__PEAVCDC__AEBVCSize__H_Z(pThis, pRet, pDC, pSizeDefault, 0);
        return pRet;
    }
    if (impl__m_bFlat_CMFCToolBarComboBoxButton__1HA == 0 && HasWindow(pCombo) && pThis->m_bIsHidden == 0) {
        impl__ShowWindow_CWnd__QEAAHH_Z(pCombo, SW_SHOWNOACTIVATE);
    }
    if (pThis->m_bTextBelow != 0 && pThis->m_strText.GetLength() != 0 && pDC != nullptr) {
        RECT rectText = { 0, 0, Width(pThis), pSizeDefault->cy };
        impl__DrawTextW_CDC__QEAAHPEB_WHPEAUTAGRECT__I_Z(pDC, static_cast<const wchar_t*>(pThis->m_strText),
                                                         pThis->m_strText.GetLength(), &rectText, kDtCalcLabel);
        pThis->m_sizeText.cx = rectText.right - rectText.left;
        pThis->m_sizeText.cy = rectText.bottom - rectText.top;
    }
    long cy = pSizeDefault->cy;
    if (HasWindow(pCombo)) {
        EnsureGlobalDataInitialized();
        if (GlobalDataInt(kGlobalDataIsWindows7) != 0) {
            COMBOBOXINFO cbi;
            std::memset(&cbi, 0, sizeof cbi);
            cbi.cbSize = sizeof cbi;
            ::GetComboBoxInfo(pCombo->m_hWnd, &cbi);
            cy = cbi.rcButton.bottom - cbi.rcButton.top;
        } else {
            RECT rc = { 0, 0, 0, 0 };
            ::GetWindowRect(pCombo->m_hWnd, &rc);
            cy = rc.bottom - rc.top;
        }
    }
    if (pThis->m_bIsHidden == 0 && HasWindow(pEdit) && pCombo != nullptr && !StyleVisible(pCombo)) {
        impl__ShowWindow_CWnd__QEAAHH_Z(pEdit, SW_SHOWNOACTIVATE);
    }
    pRet->cx = Width(pThis);
    pRet->cy = pThis->m_sizeText.cy + cy;
    return pRet;
}

// ===========================================================================
// Window creation / parent changes
// ===========================================================================

// Retail (RVA 0x1640e0, mfc140u), fully transcribed:
//     CComboBox* pWndCombo = new CComboBox;              // 0xe8 bytes, CWnd ctor + CComboBox vftable
//     if (!pWndCombo->Create(m_dwStyle, rect, pWndParent, m_nID)) {   // vslot 0x2d8 (91), devirtualised to 0x294150
//         delete pWndCombo;                                            // vslot 0x08 (1)
//         return NULL;
//     }
//     return pWndCombo;
// DEVIATION: the object is built through ??2@YAPEAX_K@Z + the exported
// ??0CWnd@@QEAA@XZ (CComboBox's own constructor is inline in MFC and not
// exported, and `new CComboBox` would drag C++ CWnd machinery into this
// translation unit), so its runtime class reads CWnd and its vftable is
// OpenMFC's CWnd one; Create goes through the exported CComboBox::Create.
// Symbol: ?CreateCombo@CMFCToolBarComboBoxButton@@UEAAPEAVCComboBox@@PEAVCWnd@@AEBVCRect@@@Z
extern "C" CComboBox* MS_ABI impl__CreateCombo_CMFCToolBarComboBoxButton__UEAAPEAVCComboBox__PEAVCWnd__AEBVCRect___Z(
    CMFCToolBarComboBoxButton* pThis, CWnd* pWndParent, const RECT* pRect)
{
    if (!pThis || !pRect) return nullptr;
    void* pStorage = impl___2_YAPEAX_K_Z(sizeof(CComboBox));
    if (!pStorage) return nullptr;
    std::memset(pStorage, 0, sizeof(CComboBox));
    CComboBox* pWndCombo = static_cast<CComboBox*>(impl___0CWnd__QEAA_XZ(pStorage));
    if (g_ownCWndVptr == nullptr) g_ownCWndVptr = *reinterpret_cast<void**>(pWndCombo);
    if (!impl__Create_CComboBox__UEAAHKAEBUtagRECT__PEAVCWnd__I_Z(pWndCombo, Style(pThis), pRect, pWndParent, pThis->m_nID)) {
        DeleteOwnedWindow(pWndCombo);
        return nullptr;
    }
    return pWndCombo;
}

// Retail (RVA 0x1641d0, mfc140u), fully decoded but NOT implemented:
//     CMFCToolBarComboBoxEdit* pWndEdit = new CMFCToolBarComboBoxEdit(*this);   // 0xf8 bytes, 0x1649e0
//     if (!pWndEdit->Create(dwEditStyle, rect, pWndParent, m_nID)) {             // 0x294320 (CEdit::Create)
//         delete pWndEdit; return NULL;                                          // vslot 0x08 (1)
//     }
//     return pWndEdit;
// Blocked: CMFCToolBarComboBoxEdit is not declared in include/openmfc, and its
// exported constructor (featurepack/toolbar/CMFCToolBarComboBoxEdit.cpp) is a
// null stub that constructs nothing -- there is no object to hand back.
// Returns NULL, which is also what retail returns when the edit cannot be created.
// Symbol: ?CreateEdit@CMFCToolBarComboBoxButton@@UEAAPEAVCMFCToolBarComboBoxEdit@@PEAVCWnd@@AEBVCRect@@K@Z
extern "C" CEdit* MS_ABI impl__CreateEdit_CMFCToolBarComboBoxButton__UEAAPEAVCMFCToolBarComboBoxEdit__PEAVCWnd__AEBVCRect__K_Z(
    CMFCToolBarComboBoxButton* pThis, CWnd* pWndParent, const RECT* pRect, unsigned long dwEditStyle)
{
    (void)pThis; (void)pWndParent; (void)pRect; (void)dwEditStyle;
    return nullptr;
}

// Retail (RVA 0x162240, mfc140u), fully transcribed:
//     CMFCToolBarButton::OnChangeParentWnd(pWndParent);                       // 0x15e570
//     if (m_pWndCombo->GetSafeHwnd() != NULL) {
//         CWnd* pOldParent = m_pWndCombo->GetParent();  (AfxThrowInvalidArgException if NULL)
//         if (pWndParent != NULL && pOldParent->m_hWnd == pWndParent->m_hWnd) return;
//         m_pWndCombo->DestroyWindow(); delete m_pWndCombo; m_pWndCombo = NULL;   // vslots 0xd0, 0x08(1)
//         if (m_pWndEdit) { m_pWndEdit->DestroyWindow(); delete m_pWndEdit; m_pWndEdit = NULL; }
//     }
//     if (pWndParent == NULL || pWndParent->m_hWnd == NULL) return;
//     BOOL bEnable = !CMFCToolBar::m_bCustomizeMode && !(m_nStyle & TBBS_DISABLED);   // 0x3be35c, +0x28
//     CRect rect = m_rect; rect.InflateRect(-2, 0);  rect.bottom = rect.top + m_nDropDownHeight;
//     m_pWndCombo = CreateCombo(pWndParent, rect);                                 // vslot 0x1b8 (55)
//     if (m_pWndCombo == NULL) return;
//     if (m_pWndCombo->m_hWnd) { m_pWndCombo->EnableWindow(bEnable); ::RedrawWindow(hwnd, NULL, NULL, RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW); }
//     if (m_bFlat && (m_pWndCombo->GetStyle() & 3) == CBS_DROPDOWN) {
//         DWORD dwEditStyle = 0x50001080 | (m_pWndCombo->GetStyle() & WS_TABSTOP);
//         m_pWndEdit = CreateEdit(pWndParent, rect, dwEditStyle);                 // vslot 0x1c0 (56)
//         if (m_pWndEdit == NULL) return;
//         m_pWndEdit->SendMessage(WM_SETFONT, afxGlobalData.fontRegular /*+0x1b0*/, TRUE);
//         m_pWndEdit->m_hWndOwner (+0xa0) = (m_pWndCombo->GetParent()->GetOwner())->m_hWnd;   -- see below
//         if (m_pWndEdit->m_hWnd) { m_pWndEdit->EnableWindow(bEnable); ::RedrawWindow(hwnd, NULL, NULL, 0x105); }
//     }
//     AdjustRect();                                                               // 0x162be0
//     m_pWndCombo->SendMessage(WM_SETFONT, afxGlobalData.fontRegular, TRUE);
//     if (m_pWndCombo->GetCount() > 0) {              // the control already has items: harvest them
//         m_lstItems.RemoveAll(); ClearData(); m_lstItemData.RemoveAll();         // vslot 0x1d8 (59)
//         for (i = 0; i < GetCount(); i++) { GetLBText(i, s); m_lstItems.AddTail(s); m_lstItemData.AddTail(CB_GETITEMDATA i); }
//         m_iSelIndex = m_pWndCombo->GetCurSel();
//     } else {                                        // fill the control from the lists
//         m_pWndCombo->ResetContent();
//         for each (item, data) { int i = CB_ADDSTRING(item); CB_SETITEMDATA(i, data); }   (AfxThrowInvalidArgException on a list mismatch)
//         if (m_iSelIndex == -1) return;
//         m_pWndCombo->SetCurSel(m_iSelIndex);
//     }
//     if (m_iSelIndex == -1 || m_iSelIndex >= m_pWndCombo->GetCount()) return;
//     m_pWndCombo->GetLBText(m_iSelIndex, m_strEdit);
//     m_pWndCombo->SetWindowText(m_strEdit);          // inline: ::IsWindow check, COleControlSite path at +0xd0
//     if (m_pWndEdit) m_pWndEdit->SetWindowText(m_strEdit);
// DEVIATIONS: the base OnChangeParentWnd is a null stub in OpenMFC (so
// m_pWndParent is not recorded here); a combo whose parent cannot be resolved
// is torn down and recreated where retail throws AfxThrowInvalidArgException
// (the call at 0x16289b inside OnChangeParentWnd, 0x162240); the owner-window line for the edit is
// dropped -- OpenMFC's CWnd does not model m_hWndOwner (+0xa0) and CreateEdit
// returns NULL anyway; CreateCombo / CreateEdit / ClearData are reached
// through this file's thunks (ClearData is the inline no-op) instead of the
// vftable; the list mismatch exception is not raised; SetWindowText goes
// through the exported CWnd thunk (no control-site path).
// Symbol: ?OnChangeParentWnd@CMFCToolBarComboBoxButton@@UEAAXPEAVCWnd@@@Z
extern "C" void MS_ABI impl__OnChangeParentWnd_CMFCToolBarComboBoxButton__UEAAXPEAVCWnd___Z(CMFCToolBarComboBoxButton* pThis, CWnd* pWndParent) {
    if (!pThis) return;
    impl__OnChangeParentWnd_CMFCToolBarButton__UEAAXPEAVCWnd___Z(pThis, pWndParent);

    if (HasWindow(WndCombo(pThis))) {
        CWnd* pOldParent = ParentOf(WndCombo(pThis));
        if (pOldParent != nullptr && pWndParent != nullptr && pOldParent->m_hWnd == pWndParent->m_hWnd) return;
        DestroyOwnedWindow(WndCombo(pThis));
        DeleteOwnedWindow(WndCombo(pThis));
        WndCombo(pThis) = nullptr;
        if (CEdit* pEdit = WndEdit(pThis)) {
            DestroyOwnedWindow(pEdit);
            DeleteOwnedWindow(pEdit);
            WndEdit(pThis) = nullptr;
        }
    }
    if (!HasWindow(pWndParent)) return;

    const int bEnable = (impl__m_bCustomizeMode_CMFCToolBar__1HA == 0 && (pThis->m_nStyle & kTbbsDisabled) == 0) ? 1 : 0;
    RECT rect = { pThis->m_rect.left, pThis->m_rect.top, pThis->m_rect.right, pThis->m_rect.bottom };
    ::InflateRect(&rect, -2, 0);
    rect.bottom = rect.top + DropDownHeight(pThis);

    CComboBox* pCombo = impl__CreateCombo_CMFCToolBarComboBoxButton__UEAAPEAVCComboBox__PEAVCWnd__AEBVCRect___Z(pThis, pWndParent, &rect);
    WndCombo(pThis) = pCombo;
    if (pCombo == nullptr) return;
    if (pCombo->m_hWnd != nullptr) {
        impl__EnableWindow_CWnd__QEAAHH_Z(pCombo, bEnable);
        ::RedrawWindow(pCombo->m_hWnd, nullptr, nullptr, kRdwInvalidateEraseUpdate);
    }
    if (impl__m_bFlat_CMFCToolBarComboBoxButton__1HA != 0 &&
        (impl__GetStyle_CWnd__QEBAKXZ(pCombo) & 3) == CBS_DROPDOWN) {
        const DWORD dwEditStyle = kEditStyleBase | (impl__GetStyle_CWnd__QEBAKXZ(pCombo) & WS_TABSTOP);
        CEdit* pEdit = impl__CreateEdit_CMFCToolBarComboBoxButton__UEAAPEAVCMFCToolBarComboBoxEdit__PEAVCWnd__AEBVCRect__K_Z(pThis, pWndParent, &rect, dwEditStyle);
        WndEdit(pThis) = pEdit;
        if (pEdit == nullptr) return;
        ::SendMessage(pEdit->m_hWnd, WM_SETFONT, reinterpret_cast<WPARAM>(GlobalRegularFont()), TRUE);
        if (pEdit->m_hWnd != nullptr) {
            impl__EnableWindow_CWnd__QEAAHH_Z(pEdit, bEnable);
            ::RedrawWindow(pEdit->m_hWnd, nullptr, nullptr, kRdwInvalidateEraseUpdate);
        }
    }
    impl__AdjustRect_CMFCToolBarComboBoxButton__IEAAXXZ(pThis);
    ::SendMessage(pCombo->m_hWnd, WM_SETFONT, reinterpret_cast<WPARAM>(GlobalRegularFont()), TRUE);

    RegisterComboButton(pThis);
    ComboButtonState& state = EnsureComboState(pThis);
    if (static_cast<int>(::SendMessage(pCombo->m_hWnd, kCbGetCount, 0, 0)) > 0) {
        state.items.clear();
        const int nCount = static_cast<int>(::SendMessage(pCombo->m_hWnd, kCbGetCount, 0, 0));
        for (int i = 0; i < nCount; ++i) {
            CString str;
            impl__GetLBText_CComboBox__QEBAXHAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(pCombo, i, &str);
            const DWORD_PTR dwData = static_cast<DWORD_PTR>(::SendMessage(pCombo->m_hWnd, kCbGetItemData, static_cast<WPARAM>(i), 0));
            state.items.emplace_back(str, dwData);
        }
        SelIndex(pThis) = static_cast<int>(::SendMessage(pCombo->m_hWnd, kCbGetCurSel, 0, 0));
    } else {
        ::SendMessage(pCombo->m_hWnd, kCbResetContent, 0, 0);
        for (const auto& item : state.items) {
            const int iIndex = static_cast<int>(::SendMessage(pCombo->m_hWnd, kCbAddString, 0,
                                                              reinterpret_cast<LPARAM>(static_cast<const wchar_t*>(item.first))));
            ::SendMessage(pCombo->m_hWnd, kCbSetItemData, static_cast<WPARAM>(iIndex), static_cast<LPARAM>(item.second));
        }
        if (SelIndex(pThis) == -1) return;
        ::SendMessage(pCombo->m_hWnd, kCbSetCurSel, static_cast<WPARAM>(SelIndex(pThis)), 0);
    }
    if (SelIndex(pThis) == -1) return;
    if (SelIndex(pThis) >= static_cast<int>(::SendMessage(pCombo->m_hWnd, kCbGetCount, 0, 0))) return;
    impl__GetLBText_CComboBox__QEBAXHAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(pCombo, SelIndex(pThis), &StrEdit(pThis));
    impl__SetWindowTextW_CWnd__QEAAXPEB_W_Z(pCombo, static_cast<const wchar_t*>(StrEdit(pThis)));
    if (CEdit* pEdit = WndEdit(pThis)) {
        impl__SetWindowTextW_CWnd__QEAAXPEB_W_Z(pEdit, static_cast<const wchar_t*>(StrEdit(pThis)));
    }
}

// Retail (RVA 0x164270, mfc140u), fully transcribed:
//     if (m_pWndCombo->GetSafeHwnd() != NULL) {
//         if (bShow && m_bHorz) { OnMove(); m_pWndCombo->ShowWindow(m_bFlat ? SW_HIDE : SW_SHOWNOACTIVATE); }   // vslot 0x70 (14)
//         else                    m_pWndCombo->ShowWindow(SW_HIDE);
//     }
//     if (m_pWndEdit->GetSafeHwnd() != NULL)
//         m_pWndEdit->ShowWindow((bShow && m_bHorz) ? SW_SHOWNOACTIVATE : SW_HIDE);
// Symbol: ?OnShow@CMFCToolBarComboBoxButton@@UEAAXH@Z
extern "C" void MS_ABI impl__OnShow_CMFCToolBarComboBoxButton__UEAAXH_Z(CMFCToolBarComboBoxButton* pThis, int bShow) {
    if (!pThis) return;
    if (CComboBox* pCombo = WndCombo(pThis); HasWindow(pCombo)) {
        int nCmdShow = SW_HIDE;
        if (bShow != 0 && Horz(pThis) != 0) {
            impl__OnMove_CMFCToolBarComboBoxButton__UEAAXXZ(pThis);
            nCmdShow = (impl__m_bFlat_CMFCToolBarComboBoxButton__1HA != 0) ? SW_HIDE : SW_SHOWNOACTIVATE;
        }
        impl__ShowWindow_CWnd__QEAAHH_Z(pCombo, nCmdShow);
    }
    if (CEdit* pEdit = WndEdit(pThis); HasWindow(pEdit)) {
        impl__ShowWindow_CWnd__QEAAHH_Z(pEdit, (bShow != 0 && Horz(pThis) != 0) ? SW_SHOWNOACTIVATE : SW_HIDE);
    }
}

// Retail (RVA 0x164860, mfc140u), fully transcribed:
//     m_nStyle = nStyle;                                                   // +0x28 (the base inline)
//     BOOL bEnable = !CMFCToolBar::m_bCustomizeMode && IsEditable() && !(m_nStyle & TBBS_DISABLED);   // vslot 0xd0 (26)
//     if (m_pWndCombo != NULL && m_pWndCombo->m_hWnd != NULL) { m_pWndCombo->EnableWindow(bEnable); ::RedrawWindow(hwnd, NULL, NULL, 0x105); }
//     if (m_pWndEdit  != NULL && m_pWndEdit->m_hWnd  != NULL) { m_pWndEdit->EnableWindow(bEnable);  ::RedrawWindow(hwnd, NULL, NULL, 0x105); }
// IsEditable is the inlined base body (IsEditableInline above).
// Symbol: ?SetStyle@CMFCToolBarComboBoxButton@@UEAAXI@Z
extern "C" void MS_ABI impl__SetStyle_CMFCToolBarComboBoxButton__UEAAXI_Z(CMFCToolBarComboBoxButton* pThis, unsigned int nStyle) {
    if (!pThis) return;
    pThis->m_nStyle = nStyle;
    const int bEnable = (impl__m_bCustomizeMode_CMFCToolBar__1HA == 0 && IsEditableInline(pThis) &&
                         (pThis->m_nStyle & kTbbsDisabled) == 0) ? 1 : 0;
    if (CComboBox* pCombo = WndCombo(pThis); HasWindow(pCombo)) {
        impl__EnableWindow_CWnd__QEAAHH_Z(pCombo, bEnable);
        ::RedrawWindow(pCombo->m_hWnd, nullptr, nullptr, kRdwInvalidateEraseUpdate);
    }
    if (CEdit* pEdit = WndEdit(pThis); HasWindow(pEdit)) {
        impl__EnableWindow_CWnd__QEAAHH_Z(pEdit, bEnable);
        ::RedrawWindow(pEdit->m_hWnd, nullptr, nullptr, kRdwInvalidateEraseUpdate);
    }
}

// Retail (RVA 0x162ed0, mfc140u), fully transcribed:
//     if (m_bIsHotEdit == bHot) return;
//     m_bIsHotEdit = bHot;                                                       // +0x98
//     if (CWnd* pParent = m_pWndCombo->GetParent()) {   (m_pWndCombo dereferenced unconditionally)
//         ::InvalidateRect(m_pWndCombo->GetParent()->m_hWnd, &m_rectCombo, TRUE);
//         ::UpdateWindow(m_pWndCombo->GetParent()->m_hWnd);
//     }
// DEVIATION: a NULL m_pWndCombo returns after the assignment where retail would fault.
// Symbol: ?SetHotEdit@CMFCToolBarComboBoxButton@@IEAAXH@Z
extern "C" void MS_ABI impl__SetHotEdit_CMFCToolBarComboBoxButton__IEAAXH_Z(CMFCToolBarComboBoxButton* pThis, int bHot) {
    if (!pThis) return;
    if (IsHotEdit(pThis) == bHot) return;
    IsHotEdit(pThis) = bHot;
    if (WndCombo(pThis) == nullptr) return;
    RepaintComboOnParent(pThis);
}

// Retail (RVA 0x1653a0, mfc140u), fully transcribed:
//     if (m_pWndEdit->GetSafeHwnd())  m_pWndEdit->SendMessage(WM_SETFONT, afxGlobalData.fontRegular /*+0x1b0*/, TRUE);
//     if (m_pWndCombo->GetSafeHwnd()) m_pWndCombo->SendMessage(WM_SETFONT, afxGlobalData.fontRegular, TRUE);
// (each preceded by the afxGlobalData init gate; the font read is GlobalRegularFont above)
// Symbol: ?OnGlobalFontsChanged@CMFCToolBarComboBoxButton@@UEAAXXZ
extern "C" void MS_ABI impl__OnGlobalFontsChanged_CMFCToolBarComboBoxButton__UEAAXXZ(CMFCToolBarComboBoxButton* pThis) {
    if (!pThis) return;
    if (CEdit* pEdit = WndEdit(pThis); HasWindow(pEdit)) {
        ::SendMessage(pEdit->m_hWnd, WM_SETFONT, reinterpret_cast<WPARAM>(GlobalRegularFont()), TRUE);
    }
    if (CComboBox* pCombo = WndCombo(pThis); HasWindow(pCombo)) {
        ::SendMessage(pCombo->m_hWnd, WM_SETFONT, reinterpret_cast<WPARAM>(GlobalRegularFont()), TRUE);
    }
}

// ===========================================================================
// Painting
// ===========================================================================

// Retail (RVA 0x1634d0, mfc140u), fully transcribed:
//     pDC->SetTextColor(afxGlobalData.clrWindowText /*+0x7c*/);   // CDC vslot 0x70 (14)
//     pDC->SetBkColor(afxGlobalData.clrWindow /*+0x78*/);         // CDC vslot 0x68 (13)
//     return afxGlobalData.brWindow.m_hObject /*+0x110*/;
// (nCtlColor is not read; each global read is behind the init gate.)  The two
// CDC virtuals go through their exported thunks.
// Symbol: ?OnCtlColor@CMFCToolBarComboBoxButton@@UEAAPEAUHBRUSH__@@PEAVCDC@@I@Z
extern "C" HBRUSH MS_ABI impl__OnCtlColor_CMFCToolBarComboBoxButton__UEAAPEAUHBRUSH____PEAVCDC__I_Z(
    CMFCToolBarComboBoxButton* pThis, CDC* pDC, unsigned int nCtlColor)
{
    (void)pThis; (void)nCtlColor;
    if (pDC != nullptr) {
        impl__SetTextColor_CDC__UEAAKK_Z(pDC, GlobalColor(kGlobalDataClrWindowText, COLOR_WINDOWTEXT));
        impl__SetBkColor_CDC__UEAAKK_Z(pDC, GlobalColor(kGlobalDataClrWindow, COLOR_WINDOW));
    }
    return GlobalWindowBrush();
}

// Retail (RVA 0x163580, mfc140u), fully transcribed:
//     if (m_pWndCombo->GetSafeHwnd() == NULL || !bHorz) {
//         CMFCToolBarButton::OnDraw(pDC, rect, pImages, bHorz, bCustomizeMode, bHighlight, bDrawBorder, bGrayDisabledButtons);   // 0x15c910
//         return;
//     }
//     BOOL bDisabled = bCustomizeMode ? !IsEditable() : (m_nStyle & TBBS_DISABLED) != 0;   // vslot 0xd0 (26)
//     pDC->SetTextColor(bDisabled  ? afxGlobalData.clrGrayedText /*+0x44*/
//                     : bHighlight ? CMFCToolBar::GetHotTextColor()                       // 0x156a70
//                                  : afxGlobalData.clrBarText /*+0x74*/);                 // CDC vslot 0x70
//     if (m_bFlat) {
//         BOOL bIsHighlighted = m_bIsHotEdit ? TRUE : bHighlight;                         // +0x98
//         CRect rectCombo = m_rectCombo;
//         BOOL bIsDropped = m_pWndCombo->SendMessage(CB_GETDROPPEDSTATE);
//         CMFCVisualManager::GetInstance()->OnDrawComboBorder(pDC, rectCombo, bDisabled, bIsDropped, bIsHighlighted, this);   // vslot 0xf0 (30)
//         ::InflateRect(&rectCombo, -2, -2);
//         COLORREF clrText = ::GetTextColor(pDC->m_hAttribDC /*+0x10*/);
//         pDC->FillSolidRect(rectCombo, bDisabled ? afxGlobalData.clrBtnFace /*+0x28*/ : afxGlobalData.clrWindow /*+0x78*/);   // 0x2a5aa0
//         if (bDisabled) pDC->Draw3dRect(rectCombo, afxGlobalData.clrBarHilite /*+0x68*/, same);                                // 0x2a5bc0
//         CRect rectButton = m_rectButton;
//         if (afxGlobalData.bIsBlackHighContrast /*+0x260*/) ::InflateRect(&rectButton, -1, -1);
//         if (rectButton.left > rectCombo.left + 1)      // the compare at 0x16382c inside OnDraw: rectButton.LEFT vs the deflated rectCombo.left + 1
//             CMFCVisualManager::GetInstance()->OnDrawComboDropButton(pDC, rectButton, bDisabled, bIsDropped, bIsHighlighted, this);   // vslot 0xe8 (29)
//         pDC->SetTextColor(clrText);
//         if (!m_strEdit.IsEmpty() && m_pWndEdit == NULL) {
//             CRect rectText = rectCombo; rectText.right = m_rectButton.left; ::InflateRect(&rectText, -2, -2);
//             if (m_pWndCombo->GetStyle() & (CBS_OWNERDRAWFIXED | CBS_OWNERDRAWVARIABLE)) {
//                 DRAWITEMSTRUCT dis = {ODT_COMBOBOX, m_nID, GetCurSel(), ODA_DRAWENTIRE, ODS_COMBOBOXEDIT | (bDisabled ? ODS_DISABLED : 0),
//                                       m_pWndCombo->m_hWnd, pDC->m_hDC, rectText, CB_GETITEMDATA(GetCurSel())};
//                 m_pWndCombo->DrawItem(&dis);                                            // CComboBox vslot 0x2e0 (92)
//             } else {
//                 COLORREF clrOld = pDC->SetTextColor(afxGlobalData.clrWindowText /*+0x7c*/);
//                 pDC->DrawText(m_strEdit, rectText, DT_VCENTER | DT_SINGLELINE | DT_NOPREFIX);   // CDC vslot 0xe0 (28)
//                 pDC->SetTextColor(clrOld);
//             }
//         }
//         pDC->SetTextColor(clrText);
//     }
//     if (m_bTextBelow && !m_strText.IsEmpty()) {                     // +0x1c, +0x38
//         CRect rectText = rect;
//         rectText.top = (m_rectCombo.bottom /*+0xb4*/ - m_sizeText.cy /*+0x7c*/ + rect.bottom) / 2;
//         pDC->DrawText(m_strText, rectText, DT_CENTER | DT_WORDBREAK);        // CDC vslot 0xe0 (28)
//     }
// DEVIATIONS: the base OnDraw is a null stub in OpenMFC, so the non-horizontal
// / no-window case draws nothing; the owner-draw (DrawItem) branch is not
// reproduced -- CComboBox::DrawItem is not reachable from here (no thunk, and
// the control object carries OpenMFC's CWnd vftable) -- the text branch is
// used for owner-draw combos too; the visual manager is the current
// ?m_pVisManager@ (no lazy default construction, as in CMFCToolBar.cpp);
// IsEditable is the inlined base body; CDC virtuals go through their thunks.
// Symbol: ?OnDraw@CMFCToolBarComboBoxButton@@UEAAXPEAVCDC@@AEBVCRect@@PEAVCMFCToolBarImages@@HHHHH@Z
extern "C" void MS_ABI impl__OnDraw_CMFCToolBarComboBoxButton__UEAAXPEAVCDC__AEBVCRect__PEAVCMFCToolBarImages__HHHHH_Z(
    CMFCToolBarComboBoxButton* pThis, CDC* pDC, const RECT* pRect, CMFCToolBarImages* pImages,
    int bHorz, int bCustomizeMode, int bHighlight, int bDrawBorder, int bGrayDisabledButtons)
{
    if (!pThis || !pDC || !pRect) return;
    CComboBox* pCombo = WndCombo(pThis);
    if (!HasWindow(pCombo) || bHorz == 0) {
        impl__OnDraw_CMFCToolBarButton__UEAAXPEAVCDC__AEBVCRect__PEAVCMFCToolBarImages__HHHHH_Z(
            pThis, pDC, pRect, pImages, bHorz, bCustomizeMode, bHighlight, bDrawBorder, bGrayDisabledButtons);
        return;
    }
    const int bDisabled = (bCustomizeMode != 0) ? (IsEditableInline(pThis) ? 0 : 1)
                                                : (((pThis->m_nStyle & kTbbsDisabled) != 0) ? 1 : 0);
    COLORREF clrTextNormal;
    if (bDisabled) clrTextNormal = GlobalColor(kGlobalDataClrGrayedText, COLOR_GRAYTEXT);
    else if (bHighlight != 0) clrTextNormal = impl__GetHotTextColor_CMFCToolBar__SAKXZ();
    else clrTextNormal = GlobalColor(kGlobalDataClrBarText, COLOR_BTNTEXT);
    impl__SetTextColor_CDC__UEAAKK_Z(pDC, clrTextNormal);

    if (impl__m_bFlat_CMFCToolBarComboBoxButton__1HA != 0) {
        const int bIsHighlighted = (IsHotEdit(pThis) != 0) ? 1 : bHighlight;
        RECT rectCombo = RectCombo(pThis);
        const int bIsDropped = static_cast<int>(::SendMessage(pCombo->m_hWnd, kCbGetDroppedState, 0, 0));
        CMFCVisualManager* pVM = static_cast<CMFCVisualManager*>(impl__m_pVisManager_CMFCVisualManager__1PEAV1_EA);
        if (pVM != nullptr) {
            impl__OnDrawComboBorder_CMFCVisualManager__UEAAXPEAVCDC__VCRect__HHHPEAVCMFCToolBarComboBoxButton___Z(
                pVM, pDC, CRect(rectCombo), bDisabled, bIsDropped, bIsHighlighted, pThis);
        }
        ::InflateRect(&rectCombo, -2, -2);
        const COLORREF clrText = ::GetTextColor(pDC->m_hAttribDC);
        impl__FillSolidRect_CDC__QEAAXPEBUtagRECT__K_Z(pDC, &rectCombo,
            bDisabled ? GlobalColor(kGlobalDataClrBtnFace, COLOR_BTNFACE) : GlobalColor(kGlobalDataClrWindow, COLOR_WINDOW));
        if (bDisabled) {
            const COLORREF clrHilite = GlobalColor(kGlobalDataClrBarHilite, COLOR_BTNHIGHLIGHT);
            impl__Draw3dRect_CDC__QEAAXPEBUtagRECT__KK_Z(pDC, &rectCombo, clrHilite, clrHilite);
        }
        RECT rectButton = RectButton(pThis);
        EnsureGlobalDataInitialized();
        if (GlobalDataInt(kGlobalDataIsBlackHighContrast) != 0) ::InflateRect(&rectButton, -1, -1);
        if (rectButton.left > rectCombo.left + 1 && pVM != nullptr) {
            impl__OnDrawComboDropButton_CMFCVisualManager__UEAAXPEAVCDC__VCRect__HHHPEAVCMFCToolBarComboBoxButton___Z(
                pVM, pDC, CRect(rectButton), bDisabled, bIsDropped, bIsHighlighted, pThis);
        }
        impl__SetTextColor_CDC__UEAAKK_Z(pDC, clrText);
        if (StrEdit(pThis).GetLength() != 0 && WndEdit(pThis) == nullptr) {
            RECT rectText = rectCombo;
            rectText.right = RectButton(pThis).left;
            ::InflateRect(&rectText, -2, -2);
            // TODO(clean-room): the CBS_OWNERDRAWFIXED / CBS_OWNERDRAWVARIABLE
            // DrawItem branch is not reproduced (see the deviation note).
            const COLORREF clrOld = impl__SetTextColor_CDC__UEAAKK_Z(pDC, GlobalColor(kGlobalDataClrWindowText, COLOR_WINDOWTEXT));
            impl__DrawTextW_CDC__QEAAHPEB_WHPEAUTAGRECT__I_Z(pDC, static_cast<const wchar_t*>(StrEdit(pThis)),
                                                             StrEdit(pThis).GetLength(), &rectText, kDtEditText);
            impl__SetTextColor_CDC__UEAAKK_Z(pDC, clrOld);
        }
        impl__SetTextColor_CDC__UEAAKK_Z(pDC, clrText);
    }
    if (pThis->m_bTextBelow != 0 && pThis->m_strText.GetLength() != 0) {
        RECT rectText = *pRect;
        rectText.top = (RectCombo(pThis).bottom - pThis->m_sizeText.cy + pRect->bottom) / 2;
        impl__DrawTextW_CDC__QEAAHPEB_WHPEAUTAGRECT__I_Z(pDC, static_cast<const wchar_t*>(pThis->m_strText),
                                                         pThis->m_strText.GetLength(), &rectText, kDtTextLabel);
    }
}

// Retail (RVA 0x163f40, mfc140u), fully transcribed:
//     int iWidthUsed = CMFCToolBarButton::OnDrawOnCustomizeList(pDC, rect, bSelected);   // 0x15db10
//     int cx = rect.Width() - (iWidthUsed + 10);  if (cx < 20) cx = 20;
//     CRect rectCombo = rect;  rectCombo.left = rect.right - cx;  ::InflateRect(&rectCombo, -1, -1);
//     ::FillRect(pDC->m_hDC /*+0x08*/, &rectCombo, afxGlobalData.brWindow.m_hObject /*+0x110*/);
//     pDC->Draw3dRect(rectCombo, afxGlobalData.clrBarShadow /*+0x64*/, same);          // 0x2a5bc0
//     CRect rectButton = rectCombo;  rectButton.left = rectCombo.right - rectCombo.Height() + 2;  ::InflateRect(&rectButton, -1, -1);
//     CMFCVisualManager::GetInstance()->OnDrawComboDropButton(pDC, rectButton, FALSE, FALSE, FALSE, this);   // vslot 0xe8 (29)
//     return rect.Width();
// DEVIATIONS: the base OnDrawOnCustomizeList is a null stub in OpenMFC
// (returns 0, so cx = rect.Width() - 10); the visual manager is the current
// ?m_pVisManager@ (no lazy default construction).
// Symbol: ?OnDrawOnCustomizeList@CMFCToolBarComboBoxButton@@UEAAHPEAVCDC@@AEBVCRect@@H@Z
extern "C" int MS_ABI impl__OnDrawOnCustomizeList_CMFCToolBarComboBoxButton__UEAAHPEAVCDC__AEBVCRect__H_Z(
    CMFCToolBarComboBoxButton* pThis, CDC* pDC, const RECT* pRect, int bSelected)
{
    if (!pThis || !pDC || !pRect) return 0;
    const int iWidthUsed = impl__OnDrawOnCustomizeList_CMFCToolBarButton__UEAAHPEAVCDC__AEBVCRect__H_Z(pThis, pDC, pRect, bSelected);
    int cx = (pRect->right - pRect->left) - (iWidthUsed + 10);
    if (cx < 20) cx = 20;
    RECT rectCombo = *pRect;
    rectCombo.left = pRect->right - cx;
    ::InflateRect(&rectCombo, -1, -1);
    ::FillRect(pDC->m_hDC, &rectCombo, GlobalWindowBrush());
    const COLORREF clrShadow = GlobalColor(kGlobalDataClrBarShadow, COLOR_BTNSHADOW);
    impl__Draw3dRect_CDC__QEAAXPEBUtagRECT__KK_Z(pDC, &rectCombo, clrShadow, clrShadow);
    RECT rectButton = rectCombo;
    rectButton.left = rectCombo.right - (rectCombo.bottom - rectCombo.top) + 2;
    ::InflateRect(&rectButton, -1, -1);
    if (CMFCVisualManager* pVM = static_cast<CMFCVisualManager*>(impl__m_pVisManager_CMFCVisualManager__1PEAV1_EA)) {
        impl__OnDrawComboDropButton_CMFCVisualManager__UEAAXPEAVCDC__VCRect__HHHPEAVCMFCToolBarComboBoxButton___Z(
            pVM, pDC, CRect(rectButton), 0, 0, 0, pThis);
    }
    return pRect->right - pRect->left;
}

// ===========================================================================
// Input / notifications
// ===========================================================================

// Retail (RVA 0x163ae0, mfc140u), fully transcribed (bDelay is not read):
//     if (m_pWndCombo->GetSafeHwnd() == NULL || !m_bHorz) return FALSE;
//     if (m_bFlat) {
//         (m_pWndEdit != NULL ? m_pWndEdit : m_pWndCombo)->SetFocus();                 // 0x2a9b60
//         m_pWndCombo->SendMessage(CB_SHOWDROPDOWN, TRUE, 0);
//         if (pWnd != NULL) ::InvalidateRect(pWnd->m_hWnd, &m_rectCombo, TRUE);
//     }
//     return TRUE;
// Symbol: ?OnClick@CMFCToolBarComboBoxButton@@UEAAHPEAVCWnd@@H@Z
extern "C" int MS_ABI impl__OnClick_CMFCToolBarComboBoxButton__UEAAHPEAVCWnd__H_Z(CMFCToolBarComboBoxButton* pThis, CWnd* pWnd, int bDelay) {
    (void)bDelay;
    if (!pThis) return 0;
    CComboBox* pCombo = WndCombo(pThis);
    if (!HasWindow(pCombo) || Horz(pThis) == 0) return 0;
    if (impl__m_bFlat_CMFCToolBarComboBoxButton__1HA != 0) {
        CWnd* pFocus = WndEdit(pThis) != nullptr ? static_cast<CWnd*>(WndEdit(pThis)) : static_cast<CWnd*>(pCombo);
        impl__SetFocus_CWnd__QEAAPEAV1_XZ(pFocus);
        ::SendMessage(pCombo->m_hWnd, kCbShowDropDown, TRUE, 0);
        if (pWnd != nullptr) ::InvalidateRect(pWnd->m_hWnd, &RectCombo(pThis), TRUE);
    }
    return 1;
}

// Retail (RVA 0x164920, mfc140u), fully transcribed:
//     if (m_pWndCombo == NULL) return FALSE;
//     CWnd* pFocus = CWnd::FromHandle(::GetFocus());                               // 0x28ad70
//     if (m_pWndCombo->SendMessage(CB_GETDROPPEDSTATE)) return TRUE;               (m_pWndCombo->m_hWnd dereferenced unconditionally)
//     if (pFocus == m_pWndCombo) return TRUE;
//     if (::IsChild(m_pWndCombo->m_hWnd, pFocus ? pFocus->m_hWnd : NULL)) return TRUE;
//     if (m_pWndEdit == NULL) return FALSE;
//     if (pFocus == m_pWndEdit) return TRUE;
//     return ::IsChild(m_pWndEdit->m_hWnd, pFocus ? pFocus->m_hWnd : NULL);
// Symbol: ?HasFocus@CMFCToolBarComboBoxButton@@UEBAHXZ
extern "C" int MS_ABI impl__HasFocus_CMFCToolBarComboBoxButton__UEBAHXZ(const CMFCToolBarComboBoxButton* pThis) {
    if (!pThis) return 0;
    CComboBox* pCombo = WndCombo(pThis);
    if (pCombo == nullptr) return 0;
    CWnd* pFocus = impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(::GetFocus());
    if (::SendMessage(pCombo->m_hWnd, kCbGetDroppedState, 0, 0) != 0) return 1;
    if (pFocus == static_cast<CWnd*>(pCombo)) return 1;
    if (::IsChild(pCombo->m_hWnd, HWndOf(pFocus))) return 1;
    CEdit* pEdit = WndEdit(pThis);
    if (pEdit == nullptr) return 0;
    if (pFocus == static_cast<CWnd*>(pEdit)) return 1;
    return ::IsChild(pEdit->m_hWnd, HWndOf(pFocus)) ? 1 : 0;
}

// Retail (RVA 0x162f60, mfc140u), fully transcribed:
//     if (m_pWndCombo == NULL || m_pWndCombo->m_hWnd == NULL) return FALSE;
//     if (m_bFlat) {
//         if (iNotifyCode == 0) return TRUE;
//         if (CWnd* pParent = m_pWndCombo->GetParent()) { pParent->InvalidateRect(&m_rectCombo, TRUE); pParent->UpdateWindow(); }
//     }
//     switch (iNotifyCode) {
//     case CBN_SELENDOK:
//         m_iSelIndex = m_pWndCombo->GetCurSel();  if (m_iSelIndex < 0) return FALSE;
//         m_pWndCombo->GetLBText(m_iSelIndex, m_strEdit);
//         if (m_pWndEdit != NULL) m_pWndEdit->SetWindowText(m_strEdit);
//         { CObList lst; if (CMFCToolBar::GetCommandButtons(m_nID, lst) > 0)                     // 0x152990
//             for each CMFCToolBarButton* p in lst: if (p && p->IsKindOf(RUNTIME_CLASS(CMFCToolBarComboBoxButton)) && p != this) {
//                 p->SelectItem(m_pWndCombo->GetCurSel(), FALSE);                                 // 0x163b80
//                 if (p->m_pWndCombo->GetSafeHwnd() && p->m_pWndCombo->GetParent()) { parent->InvalidateRect(&p->m_rectCombo, TRUE); parent->UpdateWindow(); }
//             } }
//         /* fall through */
//     case CBN_SETFOCUS:
//         if (m_pWndEdit != NULL) m_pWndEdit->SetFocus();
//         return TRUE;
//     case CBN_EDITCHANGE:
//         m_pWndCombo->GetWindowText(m_strEdit);
//         if (m_pWndEdit->GetSafeHwnd()) {
//             CString str; m_pWndEdit->GetWindowText(str);
//             if (m_pWndCombo->GetSafeHwnd()) {
//                 int iStart = m_pWndCombo->GetCurSel() + 1;
//                 int i = CB_FINDSTRINGEXACT(iStart, str); if (i == CB_ERR) i = CB_FINDSTRING(iStart, str);
//                 if (i != CB_ERR) CB_SETCURSEL(i);
//                 m_pWndCombo->SetWindowText(str);
//             }
//         }
//         { CObList lst; if (GetCommandButtons(m_nID, lst) > 0)
//             for each combo button p != this: { if (p->m_pWndCombo) p->m_pWndCombo->SetWindowText(m_strEdit); p->m_strEdit = m_strEdit; } }
//         return TRUE;
//     case CBN_SELCHANGE:
//         if (m_pWndEdit != NULL) { CString s; m_pWndCombo->GetLBText(m_pWndCombo->GetCurSel(), s); m_pWndEdit->SetWindowText(s); }
//         return TRUE;
//     case CBN_KILLFOCUS: case CBN_EDITUPDATE:
//         return TRUE;
//     default:
//         return FALSE;
//     }
// The sibling SelectItem is this file's C++ body (the retail transcription
// above); the CObList walk is the exported-thunk pattern of CommandButtonList.
// Symbol: ?NotifyCommand@CMFCToolBarComboBoxButton@@UEAAHH@Z
extern "C" int MS_ABI impl__NotifyCommand_CMFCToolBarComboBoxButton__UEAAHH_Z(CMFCToolBarComboBoxButton* pThis, int iNotifyCode) {
    if (!pThis) return 0;
    CComboBox* pCombo = WndCombo(pThis);
    if (!HasWindow(pCombo)) return 0;
    if (impl__m_bFlat_CMFCToolBarComboBoxButton__1HA != 0) {
        if (iNotifyCode == 0) return 1;
        RepaintComboOnParent(pThis);
    }
    switch (iNotifyCode) {
    case kCbnSelEndOk: {
        SelIndex(pThis) = static_cast<int>(::SendMessage(pCombo->m_hWnd, kCbGetCurSel, 0, 0));
        if (SelIndex(pThis) < 0) return 0;
        impl__GetLBText_CComboBox__QEBAXHAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(pCombo, SelIndex(pThis), &StrEdit(pThis));
        if (CEdit* pEdit = WndEdit(pThis)) {
            impl__SetWindowTextW_CWnd__QEAAXPEB_W_Z(pEdit, static_cast<const wchar_t*>(StrEdit(pThis)));
        }
        CommandButtonList lst(pThis->m_nID);
        if (lst.Count() > 0) {
            for (ObNode* pNode = lst.First(); pNode != nullptr; pNode = pNode->pNext) {
                CObject* pObj = pNode->data;
                if (!IsComboButton(pObj)) continue;
                CMFCToolBarComboBoxButton* pOther = static_cast<CMFCToolBarComboBoxButton*>(pObj);
                if (pOther == pThis) continue;
                pOther->SelectItem(static_cast<int>(::SendMessage(pCombo->m_hWnd, kCbGetCurSel, 0, 0)), FALSE);
                if (HasWindow(WndCombo(pOther))) RepaintComboOnParent(pOther);
            }
        }
    }
    /* fall through */
    case kCbnSetFocus:
        if (CEdit* pEdit = WndEdit(pThis)) impl__SetFocus_CWnd__QEAAPEAV1_XZ(pEdit);
        return 1;
    case kCbnEditChange: {
        impl__GetWindowTextW_CWnd__QEBAXAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(pCombo, &StrEdit(pThis));
        if (CEdit* pEdit = WndEdit(pThis); HasWindow(pEdit)) {
            CString str;
            impl__GetWindowTextW_CWnd__QEBAXAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(pEdit, &str);
            if (HasWindow(pCombo)) {
                const int iStart = static_cast<int>(::SendMessage(pCombo->m_hWnd, kCbGetCurSel, 0, 0)) + 1;
                const LPARAM lpText = reinterpret_cast<LPARAM>(static_cast<const wchar_t*>(str));
                int iFound = static_cast<int>(::SendMessage(pCombo->m_hWnd, kCbFindStringExact, static_cast<WPARAM>(iStart), lpText));
                if (iFound == CB_ERR) iFound = static_cast<int>(::SendMessage(pCombo->m_hWnd, kCbFindString, static_cast<WPARAM>(iStart), lpText));
                if (iFound != CB_ERR) ::SendMessage(pCombo->m_hWnd, kCbSetCurSel, static_cast<WPARAM>(iFound), 0);
                impl__SetWindowTextW_CWnd__QEAAXPEB_W_Z(pCombo, static_cast<const wchar_t*>(str));
            }
        }
        CommandButtonList lst(pThis->m_nID);
        if (lst.Count() > 0) {
            for (ObNode* pNode = lst.First(); pNode != nullptr; pNode = pNode->pNext) {
                CObject* pObj = pNode->data;
                if (!IsComboButton(pObj)) continue;
                CMFCToolBarComboBoxButton* pOther = static_cast<CMFCToolBarComboBoxButton*>(pObj);
                if (pOther == pThis) continue;
                if (WndCombo(pOther) != nullptr) {
                    impl__SetWindowTextW_CWnd__QEAAXPEB_W_Z(WndCombo(pOther), static_cast<const wchar_t*>(StrEdit(pThis)));
                }
                StrEdit(pOther) = StrEdit(pThis);
            }
        }
        return 1;
    }
    case kCbnSelChange:
        if (CEdit* pEdit = WndEdit(pThis)) {
            CString str;
            impl__GetLBText_CComboBox__QEBAXHAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
                pCombo, static_cast<int>(::SendMessage(pCombo->m_hWnd, kCbGetCurSel, 0, 0)), &str);
            impl__SetWindowTextW_CWnd__QEAAXPEB_W_Z(pEdit, static_cast<const wchar_t*>(str));
        }
        return 1;
    case kCbnKillFocus:
    case kCbnEditUpdate:
        return 1;
    default:
        return 0;
    }
}

// Retail (RVA 0x165460, mfc140u), fully transcribed (str is the 5th argument, on the stack):
//     if (!m_bHorz || !CMFCToolBar::m_bShowTooltips) return FALSE;                 // +0x94, 0x3b1b64
//     CString strTipText;
//     if (OnGetCustomToolTipText(strTipText)) str = strTipText;                     // vslot 0x160 (44): the base inline returns FALSE
//     CWnd* pWnd = m_pWndCombo;
//     if (m_bFlat) {
//         if (pWnd != NULL && (pWnd->GetStyle() & 3) == CBS_DROPDOWN) {
//             pWnd = GetEditCtrl();                                                  // vslot 0x1c8 (57): returns m_pWndEdit
//             if (pWnd == NULL) return TRUE;
//             wndToolTip.AddTool(pWnd, str, NULL, 0);                                // 0x275060
//         } else {
//             wndToolTip.AddTool(pWndParent, str, &m_rect, iButtonIndex + 1);
//         }
//     } else {
//         if (pWnd == NULL) return TRUE;
//         wndToolTip.AddTool(pWnd, str, NULL, 0);
//     }
//     return TRUE;
// OnGetCustomToolTipText is the base inline (FALSE) -- not dispatched.
// Symbol: ?OnUpdateToolTip@CMFCToolBarComboBoxButton@@UEAAHPEAVCWnd@@HAEAVCToolTipCtrl@@AEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@@Z
extern "C" int MS_ABI impl__OnUpdateToolTip_CMFCToolBarComboBoxButton__UEAAHPEAVCWnd__HAEAVCToolTipCtrl__AEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
    CMFCToolBarComboBoxButton* pThis, CWnd* pWndParent, int iButtonIndex, CToolTipCtrl* pWndToolTip, CString* pStr)
{
    if (!pThis || !pWndToolTip || !pStr) return 0;
    if (Horz(pThis) == 0 || impl__m_bShowTooltips_CMFCToolBar__1HA == 0) return 0;
    const wchar_t* lpszText = static_cast<const wchar_t*>(*pStr);
    CWnd* pWnd = WndCombo(pThis);
    if (impl__m_bFlat_CMFCToolBarComboBoxButton__1HA != 0) {
        if (pWnd != nullptr && (impl__GetStyle_CWnd__QEBAKXZ(pWnd) & 3) == CBS_DROPDOWN) {
            pWnd = WndEdit(pThis);
            if (pWnd == nullptr) return 1;
            impl__AddTool_CToolTipCtrl__QEAAHPEAVCWnd__PEB_WPEBUtagRECT___K_Z(pWndToolTip, pWnd, lpszText, nullptr, 0);
        } else {
            impl__AddTool_CToolTipCtrl__QEAAHPEAVCWnd__PEB_WPEBUtagRECT___K_Z(
                pWndToolTip, pWndParent, lpszText, reinterpret_cast<const RECT*>(&pThis->m_rect),
                static_cast<unsigned __int64>(static_cast<long long>(iButtonIndex + 1)));
        }
    } else {
        if (pWnd == nullptr) return 1;
        impl__AddTool_CToolTipCtrl__QEAAHPEAVCWnd__PEB_WPEBUtagRECT___K_Z(pWndToolTip, pWnd, lpszText, nullptr, 0);
    }
    return 1;
}

// Retail (RVA 0x1655a0, mfc140u), fully transcribed:
//     if (!CMFCToolBarButton::SetACCData(pParent, data)) return FALSE;              // 0x15e730
//     data.m_nAccRole  = (m_pWndCombo && (m_pWndCombo->GetStyle() & 3) == CBS_DROPDOWNLIST) ? ROLE_SYSTEM_DROPLIST : ROLE_SYSTEM_COMBOBOX;   // +0x30
//     data.m_bAccState = STATE_SYSTEM_FOCUSABLE;                                    // +0x34
//     if (HasFocus()) data.m_bAccState |= STATE_SYSTEM_FOCUSED;                     // vslot 0x158 (43)
//     data.m_strAccDefAction = L"Open";                                             // +0x28, .rdata 0x340ce0
//     data.m_strAccValue = m_strEdit;                                               // +0x08
//     return TRUE;
// DEVIATION: the base SetACCData is a null stub returning 0 in OpenMFC
// (featurepack/toolbar/CMFCToolBarButton.cpp), so this returns FALSE without
// filling the record until that base body exists; HasFocus is reached through
// this file's thunk.
// Symbol: ?SetACCData@CMFCToolBarComboBoxButton@@UEAAHPEAVCWnd@@AEAVCAccessibilityData@@@Z
extern "C" int MS_ABI impl__SetACCData_CMFCToolBarComboBoxButton__UEAAHPEAVCWnd__AEAVCAccessibilityData___Z(
    CMFCToolBarComboBoxButton* pThis, CWnd* pParent, CAccessibilityData* pData)
{
    if (!pThis || !pData) return 0;
    if (!impl__SetACCData_CMFCToolBarButton__UEAAHPEAVCWnd__AEAVCAccessibilityData___Z(pThis, pParent, pData)) return 0;
    CComboBox* pCombo = WndCombo(pThis);
    pData->m_nAccRole = (pCombo != nullptr && (impl__GetStyle_CWnd__QEBAKXZ(pCombo) & 3) == CBS_DROPDOWNLIST)
                            ? kRoleSystemDropList : kRoleSystemComboBox;
    pData->m_bAccState = kStateSystemFocusable;
    if (impl__HasFocus_CMFCToolBarComboBoxButton__UEBAHXZ(pThis)) pData->m_bAccState |= kStateSystemFocused;
    pData->m_strAccDefAction = L"Open";
    pData->m_strAccValue = StrEdit(pThis);
    return 1;
}

// ===========================================================================
// Copy / serialization / customization
// ===========================================================================

// Retail (RVA 0x1618b0, mfc140u), fully transcribed:
//     CMFCToolBarButton::CopyFrom(src);                                             // 0x15c1a0 (real in OpenMFC)
//     m_lstItems.RemoveAll();    for each s in src.m_lstItems:    m_lstItems.AddTail(s);
//     ClearData();                                                                  // vslot 0x1d8 (59), inline no-op
//     m_lstItemData.RemoveAll(); for each d in src.m_lstItemData: m_lstItemData.AddTail(d);
//     DuplicateData();                                                              // vslot 0x1d0 (58), inline no-op
//     m_dwStyle = src.m_dwStyle;  m_iWidth = src.m_iWidth;  m_iSelIndex = src.m_iSelIndex;
//     m_nDropDownHeight = src.m_nDropDownHeight;  m_uiMenuResID = src.m_uiMenuResID;  m_bIsRibbon = src.m_bIsRibbon;
// (m_strEdit, m_bHorz and m_bIsHotEdit are NOT copied by retail.)  The item
// lists are the side-table vectors; ClearData / DuplicateData are the base
// inline no-ops and are not dispatched.
// Symbol: ?CopyFrom@CMFCToolBarComboBoxButton@@UEAAXAEBVCMFCToolBarButton@@@Z
extern "C" void MS_ABI impl__CopyFrom_CMFCToolBarComboBoxButton__UEAAXAEBVCMFCToolBarButton___Z(
    CMFCToolBarComboBoxButton* pThis, const CMFCToolBarButton* pSrc)
{
    if (!pThis || !pSrc) return;
    impl__CopyFrom_CMFCToolBarButton__UEAAXAEBV1__Z(pThis, pSrc);
    const CMFCToolBarComboBoxButton* pSrcCombo = static_cast<const CMFCToolBarComboBoxButton*>(pSrc);
    RegisterComboButton(pThis);
    ComboButtonState& state = EnsureComboState(pThis);
    state.items.clear();
    if (const ComboButtonState* pSrcState = FindComboState(pSrcCombo)) {
        state.items = pSrcState->items;
    }
    Style(pThis) = Style(pSrcCombo);
    Width(pThis) = Width(pSrcCombo);
    SelIndex(pThis) = SelIndex(pSrcCombo);
    DropDownHeight(pThis) = DropDownHeight(pSrcCombo);
    MenuResID(pThis) = MenuResID(pSrcCombo);
    IsRibbon(pThis) = IsRibbon(pSrcCombo);
}

// Retail (RVA 0x1619a0, mfc140u), fully transcribed.  The int reads / writes
// are CArchive's inline buffer accesses (FillBuffer 0x1d1cc0 / Flush 0x1d1be0
// on a short buffer), the CString ones the outlined CStringT archive operators
// at 0x1b5e4 (operator>>: AfxReadStringLength + characters) and 0x1b818
// (operator<<: AfxWriteStringLength + characters), not exported:
//     CMFCToolBarButton::Serialize(ar);                                              // 0x15c240
//     if (ar.IsLoading()) {
//         ar >> m_iWidth;  m_rect.right = m_rect.left + m_iWidth;
//         ar >> m_dwStyle;  ar >> m_iSelIndex;  ar >> m_strEdit;  ar >> m_nDropDownHeight;  ar >> m_uiMenuResID;
//         m_lstItems.Serialize(ar);                                                   // CStringList vslot 2
//         ClearData();  m_lstItemData.RemoveAll();                                    // vslot 0x1d8 (59)
//         for (i = 0; i < m_lstItems.GetCount(); i++) { int d; ar >> d; m_lstItemData.AddTail((DWORD_PTR)d); }
//         DuplicateData();                                                            // vslot 0x1d0 (58)
//         SelectItem(m_iSelIndex, TRUE);                                              // 0x163b80
//     } else {
//         ar << m_iWidth;  ar << m_dwStyle;  ar << m_iSelIndex;  ar << m_strEdit;  ar << m_nDropDownHeight;  ar << m_uiMenuResID;
//         if (m_pWndCombo != NULL) {          // refresh the lists from the live control first
//             m_lstItems.RemoveAll();  ClearData();  m_lstItemData.RemoveAll();
//             for (i = 0; i < CB_GETCOUNT; i++) { GetLBText(i, s); m_lstItems.AddTail(s); m_lstItemData.AddTail(CB_GETITEMDATA(i)); }
//         }
//         m_lstItems.Serialize(ar);
//         for each d in m_lstItemData: ar << (int)d;
//     }
// The scalars use OpenMFC's own archive byte layout (see ArWriteInt /
// ArWriteString); the item strings go through ?Serialize@CStringList@@ on a
// scratch CStringList so that they use OpenMFC's CStringList format.
// Symbol: ?Serialize@CMFCToolBarComboBoxButton@@UEAAXAEAVCArchive@@@Z
extern "C" void MS_ABI impl__Serialize_CMFCToolBarComboBoxButton__UEAAXAEAVCArchive___Z(CMFCToolBarComboBoxButton* pThis, CArchive* ar) {
    if (!pThis || !ar) return;
    impl__Serialize_CMFCToolBarButton__UEAAXAEAVCArchive___Z(pThis, ar);
    RegisterComboButton(pThis);
    ComboButtonState& state = EnsureComboState(pThis);

    if (ar->IsLoading()) {
        Width(pThis) = ArReadInt(ar);
        pThis->m_rect.right = pThis->m_rect.left + Width(pThis);
        Style(pThis) = static_cast<DWORD>(ArReadInt(ar));
        SelIndex(pThis) = ArReadInt(ar);
        ArReadString(ar, StrEdit(pThis));
        DropDownHeight(pThis) = ArReadInt(ar);
        MenuResID(pThis) = static_cast<unsigned>(ArReadInt(ar));

        ScratchStringList lstItems;
        impl__Serialize_CStringList__UEAAXAEAVCArchive___Z(lstItems.List(), *ar);
        state.items.clear();
        for (StrNode* pNode = lstItems.First(); pNode != nullptr; pNode = pNode->pNext) {
            state.items.emplace_back(pNode->data, static_cast<DWORD_PTR>(0));
        }
        for (auto& item : state.items) {
            item.second = static_cast<DWORD_PTR>(static_cast<long long>(ArReadInt(ar)));
        }
        pThis->SelectItem(SelIndex(pThis), TRUE);
    } else {
        ArWriteInt(ar, Width(pThis));
        ArWriteInt(ar, static_cast<int>(Style(pThis)));
        ArWriteInt(ar, SelIndex(pThis));
        ArWriteString(ar, StrEdit(pThis));
        ArWriteInt(ar, DropDownHeight(pThis));
        ArWriteInt(ar, static_cast<int>(MenuResID(pThis)));

        if (CComboBox* pCombo = WndCombo(pThis)) {
            state.items.clear();
            const HWND hwnd = pCombo->m_hWnd;
            const int nCount = static_cast<int>(::SendMessage(hwnd, kCbGetCount, 0, 0));
            for (int i = 0; i < nCount; ++i) {
                CString str;
                impl__GetLBText_CComboBox__QEBAXHAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(pCombo, i, &str);
                state.items.emplace_back(str, static_cast<DWORD_PTR>(::SendMessage(hwnd, kCbGetItemData, static_cast<WPARAM>(i), 0)));
            }
        }
        ScratchStringList lstItems;
        for (const auto& item : state.items) lstItems.AddTail(item.first);
        impl__Serialize_CStringList__UEAAXAEAVCArchive___Z(lstItems.List(), *ar);
        for (const auto& item : state.items) ArWriteInt(ar, static_cast<int>(item.second));
    }
}

// Retail (RVA 0x163440, mfc140u), fully transcribed:
//     CObList lst;
//     if (CMFCToolBar::GetCommandButtons(m_nID, lst) != 0)                           // 0x152990
//         CopyFrom(*(CMFCToolBarButton*)lst.GetHead());                              // vslot 0x38 (7)
// CopyFrom is reached through this file's thunk rather than the vftable.
// Symbol: ?OnAddToCustomizePage@CMFCToolBarComboBoxButton@@UEAAXXZ
extern "C" void MS_ABI impl__OnAddToCustomizePage_CMFCToolBarComboBoxButton__UEAAXXZ(CMFCToolBarComboBoxButton* pThis) {
    if (!pThis) return;
    CommandButtonList lst(pThis->m_nID);
    if (lst.Count() == 0) return;
    ObNode* pHead = lst.First();
    if (pHead == nullptr || pHead->data == nullptr) return;
    impl__CopyFrom_CMFCToolBarComboBoxButton__UEAAXAEBVCMFCToolBarButton___Z(pThis, static_cast<const CMFCToolBarButton*>(pHead->data));
}

// Retail (RVA 0x164310, mfc140u), fully transcribed:
//     CString strMessage;
//     HINSTANCE hInst = AfxFindStringResourceHandle(m_nID);                          // 0x2aee00
//     if (hInst != NULL && strMessage.LoadString(hInst, m_nID) && strMessage.GetLength() > 0) {   // 0xdb70
//         LPCTSTR p = wcschr(strMessage, L'\n');
//         if (p != NULL) { int i = (int)(p - strMessage); if (i != -1) menuButton.m_strText = strMessage.Mid(i + 1, len - (i + 1)); }   // +0x38
//     }
//     return TRUE;
// The CStringT::LoadString(HINSTANCE, UINT) step is done with ::LoadStringW
// into a 1024-character local buffer (the ATL helper is inline, not exported),
// and `Mid(i + 1, len - (i + 1))` is the tail after the '\n'.
// Symbol: ?ExportToMenuButton@CMFCToolBarComboBoxButton@@UEBAHAEAVCMFCToolBarMenuButton@@@Z
extern "C" int MS_ABI impl__ExportToMenuButton_CMFCToolBarComboBoxButton__UEBAHAEAVCMFCToolBarMenuButton___Z(
    const CMFCToolBarComboBoxButton* pThis, CMFCToolBarMenuButton* pMenuButton)
{
    if (!pThis || !pMenuButton) return 0;
    HINSTANCE hInst = static_cast<HINSTANCE>(impl__AfxFindStringResourceHandle__YAPEAUHINSTANCE____I_Z(pThis->m_nID));
    if (hInst != nullptr) {
        wchar_t buf[1024];
        const int nLen = ::LoadStringW(hInst, pThis->m_nID, buf, static_cast<int>(sizeof(buf) / sizeof(buf[0])));
        if (nLen > 0) {
            const wchar_t* p = wcschr(buf, L'\n');
            if (p != nullptr) {
                pMenuButton->m_strText = p + 1;
            }
        }
    }
    return 1;
}

// Retail (RVA 0x162b30, mfc140u), fully transcribed:
//     m_lstItems.RemoveAll();  ClearData();  m_lstItemData.RemoveAll();               // vslot 0x1d8 (59)
//     if (m_pWndCombo->GetSafeHwnd()) m_pWndCombo->SendMessage(CB_RESETCONTENT);
//     m_strEdit.Empty();
//     if (m_pWndEdit->GetSafeHwnd()) m_pWndEdit->SetWindowText(m_strEdit);
// (m_iSelIndex is NOT reset.)  The lists are the side-table vector.
// Symbol: ?RemoveAllItems@CMFCToolBarComboBoxButton@@QEAAXXZ
extern "C" void MS_ABI impl__RemoveAllItems_CMFCToolBarComboBoxButton__QEAAXXZ(CMFCToolBarComboBoxButton* pThis) {
    if (!pThis) return;
    RegisterComboButton(pThis);
    EnsureComboState(pThis).items.clear();
    if (CComboBox* pCombo = WndCombo(pThis); HasWindow(pCombo)) {
        ::SendMessage(pCombo->m_hWnd, kCbResetContent, 0, 0);
    }
    StrEdit(pThis).Empty();
    if (CEdit* pEdit = WndEdit(pThis); HasWindow(pEdit)) {
        impl__SetWindowTextW_CWnd__QEAAXPEB_W_Z(pEdit, static_cast<const wchar_t*>(StrEdit(pThis)));
    }
}

// Retail (RVA 0x164800, mfc140u), fully transcribed:
//     CMFCToolBarComboBoxButton* p = GetByCmd(uiCmd, FALSE);                         // 0x1645e0
//     if (p == NULL) return NULL;
//     return (void*)p->m_pWndCombo->SendMessage(CB_GETITEMDATA, iIndex, 0);         (m_pWndCombo dereferenced unconditionally)
// DEVIATION: a button without a combo window yields NULL where retail would fault.
// Symbol: ?GetItemDataPtrAll@CMFCToolBarComboBoxButton@@SAPEAXIH@Z
extern "C" void* MS_ABI impl__GetItemDataPtrAll_CMFCToolBarComboBoxButton__SAPEAXIH_Z(unsigned int uiCmd, int iIndex) {
    CMFCToolBarComboBoxButton* pButton = CMFCToolBarComboBoxButton::GetByCmd(uiCmd, FALSE);
    if (pButton == nullptr) return nullptr;
    CComboBox* pCombo = WndCombo(pButton);
    if (!HasWindow(pCombo)) return nullptr;
    return reinterpret_cast<void*>(::SendMessage(pCombo->m_hWnd, kCbGetItemData, static_cast<WPARAM>(static_cast<long long>(iIndex)), 0));
}
