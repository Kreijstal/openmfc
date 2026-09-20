// CMFCToolBarDateTimeCtrl — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp
//
// Every body marked "Retail (RVA 0x..., mfc140u)" below was transcribed from
// the disassembly of that function in mfc140u.dll (14.51.36231), the method
// described in the header of core/ole/COleControl.cpp.  Where a body is
// reached from the class vftable but has no entry in the export map (OnMove,
// OnSize, OnShow, SetStyle, OnUpdateToolTip, OnGlobalFontsChanged, Serialize,
// the SetTime / SetTimeAll overloads), its RVA was read out of the retail
// vftable at 0x1803170a0 (mfc140u) or off the call site that reaches it, and
// the comment says which.
//
// INSTANCE LAYOUT.  This class is NOT declared in include/openmfc, so this file
// is the only place its layout lives: every export takes a `void*` this and a
// real MSVC client (compiled against afxtoolbardatetimectrl.h) allocates the
// object with the retail layout.  The two exported constructors (0x165af0 /
// 0x165b90) and Initialize (0x165c10) write it as:
//
//   CMFCToolBarButton base ............ +0x00 .. +0x88   (afxmfc.h, sizeof 136)
//   +0x88  CMFCToolBarDateTimeCtrlImpl* m_pWndDateTime
//   +0x90  DWORD m_dwStyle              +0x94  DWORD m_dwTimeStatus
//   +0x98  int   m_iWidth               +0x9c  BOOL  m_bHorz
//   +0xa0  CTime m_time  (one __time64_t)
//   sizeof == 0xa8 (168, the size ?CreateObject@ allocates and the value the
//   RTTI descriptor in featurepack/toolbar/RuntimeClasses.cpp carries)
//
// The names are the SDK's (afxtoolbardatetimectrl.h declares the six members
// in exactly this order); which offset is which was fixed from the bodies that
// use them: OnSize (0x1661c0) stores its iSize at +0x98 and OnCalculateSize
// (0x165f70) its bHorz at +0x9c, NotifyCommand (0x166580) stores the
// CDateTimeCtrl::GetTime status at +0x94 and passes +0xa0 as the CTime, the
// constructors OR 0x50000000 (WS_CHILD|WS_VISIBLE) into +0x90 and
// Initialize stores _time64(NULL) at +0xa0.
//
// VFTABLE.  Retail installs &CMFCToolBarDateTimeCtrl::`vftable' (0x1803170a0,
// mfc140u): 55 MSVC-layout slots, the 53 CMFCToolBarButton ones followed by
// 53 DuplicateData (the `ret` at 0x27d0) and 54 CreateDateTimeCtrl.  OpenMFC
// has no C++ class for this type, so the constructors leave the vptr the base
// constructor thunk installed (the mingw-built CMFCToolBarButton one; the
// MSVC-shaped CMFCToolBarButton table in detail/CMFCToolBarButtonSupport.cpp is
// installed on nothing, see the note above IsEditable in
// featurepack/visualmanager/CMFCVisualManagerOfficeXP.cpp).  Every virtual
// call the retail bodies make on `this` -- NotifyCommand (slot 18), IsEditable
// (26), OnMove (14), OnGetCustomToolTipText (44), DuplicateData (53),
// CreateDateTimeCtrl (54) -- is therefore made through this file's own thunk
// or an inline copy of the retail body instead, as
// featurepack/toolbar/CMFCToolBarComboBoxButton.cpp does; each site says so.
// A client subclass's override of one of those is NOT reached.  The
// destructor does reproduce retail's "store my own vftable first" step, with
// the recorded base vptr, so that the base destructor thunk's C++ virtual
// destructor call is safe even when a client-derived class's MSVC table was
// in the slot.
// CONSEQUENCE FOR RTTI (unlike the combo-box button, which has a mingw class
// of its own in afxmfc.h): slot 0 of the vptr these objects carry is
// CMFCToolBarButton::GetRuntimeClass, so the exported ?IsKindOf@CObject@@
// (core/runtime/CObject.cpp, reads vtable[0]) answers CMFCToolBarButton for an
// object built here and never CMFCToolBarDateTimeCtrl.  Retail's GetByCmd and
// NotifyCommand pick their peers out of CMFCToolBar::GetCommandButtons with
// exactly that IsKindOf, so the constructors additionally record every live
// object (g_pLiveObjects / RecordLive below) and IsDateTimeButton consults
// that record as well as IsKindOf; only a client-derived object carrying its
// own MSVC vftable is recognised by IsKindOf itself.  A client's own
// IsKindOf(RUNTIME_CLASS(CMFCToolBarDateTimeCtrl)) on an object built here
// still reads FALSE.
//
// THE WINDOW OBJECT.  m_pWndDateTime is a CMFCToolBarDateTimeCtrlImpl
// (afxtoolbardatetimectrl.h: CDateTimeCtrl plus `bool m_bMonthCtrlDisplayed`;
// CreateDateTimeCtrl, 0x166ae0, allocates 0xf0 bytes, runs ??0CWnd@@QEAA@XZ,
// stores the Impl vftable 0x1803172f8 and clears the byte at +0xe8).  Its
// constructor is inline in MFC and not exported, so CreateDateTimeCtrl below
// builds it the way CMFCToolBarComboBoxButton.cpp builds its combo:
// ??2@YAPEAX_K@Z + the exported ??0CWnd@@QEAA@XZ + the exported
// ?Create@CDateTimeCtrl@@.  Such an object carries OpenMFC's CWnd vftable and
// its runtime class reads CWnd; its message map (detail/Toolbar21MsgmapSupport.cpp)
// is empty, so the DTN_* reflection handlers of CMFCToolBarDateTimeCtrlImpl.cpp
// are never reached through it.  A window some other code planted in
// m_pWndDateTime (one built against the real MFC headers) is torn down through
// its own MSVC vftable slots exactly as retail does -- see DestroyOwnedWindow.
//
// afxGlobalData.  AFX_GLOBAL_DATA is not declared in OpenMFC's headers and
// ?afxGlobalData@@ is a zeroed 720-byte block whose Initialize thunk is an
// empty stub; the slots below are the byte offsets the retail bodies here
// read, named after afxglobals.h.  Each colour slot was read back out of
// ?UpdateSysColors@AFX_GLOBAL_DATA@@ (RVA 0x6b1c0, mfc140u), which stores
// GetSysColor(n) at:
//   +0x044 clrGrayedText  <- COLOR_GRAYTEXT(17)     +0x060 clrBarFace    <- COLOR_BTNFACE(15)
//   +0x064 clrBarShadow   <- COLOR_BTNSHADOW(16)    +0x068 clrBarHilite  <- COLOR_BTNHIGHLIGHT(20)
//   +0x06c clrBarDkShadow <- COLOR_3DDKSHADOW(21)   +0x070 clrBarLight   <- COLOR_3DLIGHT(22)
//   +0x074 clrBarText     <- COLOR_BTNTEXT(18)      +0x078 clrWindow     <- COLOR_WINDOW(5)
//   +0x07c clrWindowText  <- COLOR_WINDOWTEXT(8)
// and re-creates brWindow (the CBrush at +0x108, handle at +0x110) as
// CreateSolidBrush(clrWindow).  fontRegular is the CFont at +0x1a8 (handle
// +0x1b0): ?UpdateTextMetrics@AFX_GLOBAL_DATA@@IEAAXXZ (RVA 0x6b7a0 mfc140u --
// not in the export map, located by its prologue matching the mfc140.dll
// export at 0x6b5b0) selects it into a screen DC, calls GetTextMetricsW and
// stores `tm.tmHeight + (tm.tmHeight < 15 ? 2 : 5)` at +0x2a8, which is
// m_nTextHeightHorz (afxGlobalData.GetTextHeight()); OnChangeParentWnd and
// OnGlobalFontsChanged below read the handle at +0x1b0 straight into
// WM_SETFONT.
// DEVIATION shared by every reader: because Initialize is a stub the block
// stays zero, so when it is unpopulated the readers fall back to the live
// system value the slot would hold (GetSysColor / GetSysColorBrush /
// DEFAULT_GUI_FONT / the UpdateTextMetrics formula on that font).

#include "detail/ManualSmallStubImplementationsSupport.h"

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <ctime>

// ---------------------------------------------------------------------------
// Cross-file thunks.  Each was grepped to its definition in the file named;
// the parameter lists are derived from the mangled names.
// ---------------------------------------------------------------------------
//   core/runtime/AFX_GLOBAL_DATA.cpp : Initialize (an empty stub today)
//   featurepack/CMFC_misc_stubs.cpp  : ?afxGlobalData@@ storage, a zeroed 720-byte block
extern "C" void MS_ABI impl__Initialize_AFX_GLOBAL_DATA__QEAAXXZ(void* pThis);
extern "C" unsigned char impl__afxGlobalData__3UAFX_GLOBAL_DATA__A[720];
//   featurepack/toolbar/StaticData.cpp : ?m_bCustomizeMode@CMFCToolBar@@1HA (0x3be35c mfc140u)
extern "C" std::int32_t impl__m_bCustomizeMode_CMFCToolBar__1HA;
//   featurepack/toolbar/CMFCToolBar.cpp : GetHotTextColor, GetCommandButtons
extern "C" unsigned long MS_ABI impl__GetHotTextColor_CMFCToolBar__SAKXZ();
extern "C" int MS_ABI impl__GetCommandButtons_CMFCToolBar__SAHIAEAVCObList___Z(unsigned int uiCmd, CObList* pLstButtons);
//   featurepack/toolbar/RuntimeClasses.cpp : GetThisClass (the RTTI descriptor)
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMFCToolBarDateTimeCtrl__SAPEAUCRuntimeClass__XZ();
//   core/runtime/CObject.cpp : IsKindOf
extern "C" int MS_ABI impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(const CObject* pThis, const CRuntimeClass* pClass);
//   core/collections/CObList.cpp : ctor / dtor / FindIndex
extern "C" void* MS_ABI impl___0CObList__QEAA__J_Z(void* pThis, long long nBlockSize);
extern "C" void MS_ABI impl___1CObList__UEAA_XZ(void* pThis);
extern "C" CObList::POSITION MS_ABI impl__FindIndex_CObList__QEBAPEAU__POSITION____J_Z(const CObList* pThis, long long nIndex);
//   core/runtime/Thunks.cpp : CArchive::Read / Write
extern "C" unsigned int MS_ABI impl__Read_CArchive__QEAAIPEAXI_Z(CArchive* pThis, void* pBuf, unsigned int nMax);
extern "C" void MS_ABI impl__Write_CArchive__QEAAXPEBXI_Z(CArchive* pThis, const void* pBuf, unsigned int nMax);
//   featurepack/toolbar/Thunks.cpp : the base constructors and destructor
//   (placement-new / C++ destructor of the mingw CMFCToolBarButton)
extern "C" void* MS_ABI impl___0CMFCToolBarButton__QEAA_XZ(void* pThis);
extern "C" void* MS_ABI impl___0CMFCToolBarButton__QEAA_IHPEB_WHH_Z(void* pThis, unsigned int uiID, int iImage, const wchar_t* lpszText, int bUserButton, int bLocked);
extern "C" void MS_ABI impl___1CMFCToolBarButton__UEAA_XZ(void* pThis);
//   featurepack/toolbar/CMFCToolBarButton.cpp : the base-class overrides.
//   CopyFrom and Serialize are real bodies (tbb_CopyFrom / tbb_Serialize);
//   OnCalculateSize, OnChangeParentWnd, OnDraw and OnDrawOnCustomizeList are
//   still null stubs there (with placeholder parameter lists -- harmless
//   across translation units for extern "C" symbols).  The lists below are
//   the retail ones: SIZE is returned through a hidden pointer in RDX (the
//   retail body writes the result to [rdx] and returns rdx in rax).
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
//   core/window/CWnd.cpp / core/window/Thunks.cpp / core/window/CtorDtorPlacement.cpp
extern "C" CWnd* MS_ABI impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(HWND hWnd);
extern "C" unsigned long MS_ABI impl__GetStyle_CWnd__QEBAKXZ(const CWnd* pThis);
extern "C" int MS_ABI impl__ShowWindow_CWnd__QEAAHH_Z(CWnd* pThis, int nCmdShow);
extern "C" int MS_ABI impl__EnableWindow_CWnd__QEAAHH_Z(CWnd* pThis, int bEnable);
extern "C" int MS_ABI impl__SetWindowPos_CWnd__QEAAHPEBV1_HHHHI_Z(
    CWnd* pThis, const CWnd* pWndInsertAfter, int x, int y, int cx, int cy, unsigned int nFlags);
extern "C" void MS_ABI impl__ScreenToClient_CWnd__QEBAXPEAUtagRECT___Z(const CWnd* pThis, RECT* pRect);
extern "C" int MS_ABI impl__DestroyWindow_CWnd__UEAAHXZ(CWnd* pThis);
extern "C" void* MS_ABI impl___0CWnd__QEAA_XZ(void* pThis);
extern "C" void MS_ABI impl___1CWnd__UEAA_XZ(void* pThis);
//   core/controls/CDateTimeCtrl.cpp : Create, GetTime, SetTime (all on a CWnd* this;
//   the CTime overloads take the 8-byte ATL CTime by address as an opaque pointer)
extern "C" int MS_ABI impl__Create_CDateTimeCtrl__UEAAHKAEBUtagRECT__PEAVCWnd__I_Z(
    CWnd* pThis, DWORD dwStyle, const RECT* pRect, CWnd* pParentWnd, UINT nID);
extern "C" int MS_ABI impl__GetTime_CDateTimeCtrl__QEBAHAEAVCOleDateTime_ATL___Z(const CWnd* pThis, COleDateTime* pTime);
extern "C" unsigned long MS_ABI impl__GetTime_CDateTimeCtrl__QEBAKAEAVCTime_ATL___Z(const CWnd* pThis, void* pTime);
extern "C" int MS_ABI impl__SetTime_CDateTimeCtrl__QEAAHAEBVCOleDateTime_ATL___Z(CWnd* pThis, const COleDateTime* pTime);
extern "C" int MS_ABI impl__SetTime_CDateTimeCtrl__QEAAHPEBVCTime_ATL___Z(CWnd* pThis, const void* pTime);
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
//   featurepack/controls/CMFCReBar.cpp : CMenuImages::Draw -- defined there as
//   an EMPTY stub with an auto-generated parameter list that does not match the
//   mangled name; this declaration is the one derived from the name (the same
//   one featurepack/visualmanager/CMFCVisualManagerOffice2003.cpp uses).
extern "C" void MS_ABI impl__Draw_CMenuImages__SAXPEAVCDC__W4IMAGES_IDS_1_AEBVCRect__W4IMAGE_STATE_1_AEBVCSize___Z(
    CDC* pDC, int nImageID, const CRect& rect, int nImageState, const CSize& sizeImage);
//   detail/MemcoreSupport.cpp : ??2@YAPEAX_K@Z / ??3@YAXPEAX@Z
extern "C" void* MS_ABI impl___2_YAPEAX_K_Z(std::size_t size);
extern "C" void MS_ABI impl___3_YAXPEAX_Z(void* ptr);
//   featurepack/CMFC_misc_stubs.cpp : AfxFindStringResourceHandle
extern "C" void* MS_ABI impl__AfxFindStringResourceHandle__YAPEAUHINSTANCE____I_Z(unsigned int nID);
//   core/collections/CStringT.cpp : CStringT::LoadString(HINSTANCE, UINT) (retail 0xdb70, mfc140u)
extern "C" int MS_ABI impl__LoadStringW___CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAAHPEAUHINSTANCE____I_Z(
    CString* pThis, HINSTANCE hInst, UINT nID);

// Exports defined later in this file that earlier bodies call (all in this TU).
extern "C" void* MS_ABI impl___0CMFCToolBarDateTimeCtrl__QEAA_XZ(void* pThis);
extern "C" void MS_ABI impl__Initialize_CMFCToolBarDateTimeCtrl__IEAAXXZ(void* pThis);
extern "C" void MS_ABI impl__AdjustRect_CMFCToolBarDateTimeCtrl__IEAAXXZ(void* pThis);
extern "C" void MS_ABI impl__OnMove_CMFCToolBarDateTimeCtrl__UEAAXXZ(void* pThis);
extern "C" int MS_ABI impl__NotifyCommand_CMFCToolBarDateTimeCtrl__UEAAHH_Z(void* pThis, int iNotifyCode);
extern "C" void MS_ABI impl__CopyFrom_CMFCToolBarDateTimeCtrl__UEAAXAEBVCMFCToolBarButton___Z(void* pThis, const CMFCToolBarButton* pSrc);
extern "C" void* MS_ABI impl__CreateDateTimeCtrl_CMFCToolBarDateTimeCtrl__MEAAPEAVCMFCToolBarDateTimeCtrlImpl__PEAVCWnd__AEBVCRect___Z(
    void* pThis, CWnd* pWndParent, const RECT* pRect);
extern "C" void* MS_ABI impl__GetByCmd_CMFCToolBarDateTimeCtrl__SAPEAV1_I_Z(unsigned int uiCmd);
extern "C" int MS_ABI impl__SetTime_CMFCToolBarDateTimeCtrl__QEAAHPEAU_SYSTEMTIME___Z(void* pThis, SYSTEMTIME* pTimeNew);
extern "C" int MS_ABI impl__SetTime_CMFCToolBarDateTimeCtrl__QEAAHAEBVCOleDateTime_ATL___Z(void* pThis, const COleDateTime* pTimeNew);
extern "C" int MS_ABI impl__SetTime_CMFCToolBarDateTimeCtrl__QEAAHPEBVCTime_ATL___Z(void* pThis, const void* pTimeNew);

namespace {

// Retail member offsets (see the file header).
constexpr int kOffWndDateTime = 0x88;
constexpr int kOffStyle       = 0x90;
constexpr int kOffTimeStatus  = 0x94;
constexpr int kOffWidth       = 0x98;
constexpr int kOffHorz        = 0x9c;
constexpr int kOffTime        = 0xa0;
constexpr std::size_t kObjectSize = 0xa8;

// ATL's CTime is a single __time64_t (the same 8-byte view core/controls/CDateTimeCtrl.cpp uses).
struct CTime_atl { __time64_t m_time; };
static_assert(sizeof(CTime_atl) == 8, "CTime is one __time64_t (+0xa0..+0xa8)");
static_assert(kOffTime + sizeof(CTime_atl) == kObjectSize, "m_time is the last member");

static_assert(sizeof(CMFCToolBarButton) == 0x88, "CMFCToolBarDateTimeCtrl's own members start at +0x88");
static_assert(offsetof(CMFCToolBarButton, m_bTextBelow) == 0x1c, "m_bTextBelow @0x1c");
static_assert(offsetof(CMFCToolBarButton, m_nID) == 0x24, "m_nID @0x24");
static_assert(offsetof(CMFCToolBarButton, m_nStyle) == 0x28, "m_nStyle @0x28");
static_assert(offsetof(CMFCToolBarButton, m_strText) == 0x38, "m_strText @0x38");
static_assert(offsetof(CMFCToolBarButton, m_bIsHidden) == 0x54, "m_bIsHidden @0x54");
static_assert(offsetof(CMFCToolBarButton, m_bVisible) == 0x64, "m_bVisible @0x64");
static_assert(offsetof(CMFCToolBarButton, m_rect) == 0x68, "m_rect @0x68");
static_assert(offsetof(CMFCToolBarButton, m_sizeText) == 0x78, "m_sizeText @0x78");
static_assert(offsetof(CWnd, m_hWnd) == 0x40, "CWnd::m_hWnd @0x40 (every retail body reads the HWND there)");
static_assert(sizeof(CRect) == sizeof(RECT), "CRect is RECT-shaped");
static_assert(sizeof(CString) == sizeof(void*), "CString is one pointer");

// The window object retail allocates in CreateDateTimeCtrl (see the file header).
constexpr std::size_t kImplObjectSize          = 0xf0;
constexpr int         kOffImplMonthCtrlDisplayed = 0xe8;
static_assert(sizeof(CWnd) <= kImplObjectSize, "OpenMFC's CWnd must fit in the retail Impl allocation");
static_assert(sizeof(CWnd) <= static_cast<std::size_t>(kOffImplMonthCtrlDisplayed), "the Impl bool sits past OpenMFC's CWnd");

template <class T> inline T& At(void* p, int off) {
    return *reinterpret_cast<T*>(static_cast<char*>(p) + off);
}
template <class T> inline const T& At(const void* p, int off) {
    return *reinterpret_cast<const T*>(static_cast<const char*>(p) + off);
}

inline CMFCToolBarButton*       Base(void* p)       { return static_cast<CMFCToolBarButton*>(p); }
inline const CMFCToolBarButton* Base(const void* p) { return static_cast<const CMFCToolBarButton*>(p); }
inline CWnd*&       WndDateTime(void* p)       { return At<CWnd*>(p, kOffWndDateTime); }
inline CWnd*        WndDateTime(const void* p) { return At<CWnd*>(p, kOffWndDateTime); }
inline DWORD&       Style(void* p)             { return At<DWORD>(p, kOffStyle); }
inline DWORD        Style(const void* p)       { return At<DWORD>(p, kOffStyle); }
inline DWORD&       TimeStatus(void* p)        { return At<DWORD>(p, kOffTimeStatus); }
inline DWORD        TimeStatus(const void* p)  { return At<DWORD>(p, kOffTimeStatus); }
inline int&         Width(void* p)             { return At<int>(p, kOffWidth); }
inline int          Width(const void* p)       { return At<int>(p, kOffWidth); }
inline int&         Horz(void* p)              { return At<int>(p, kOffHorz); }
inline int          Horz(const void* p)        { return At<int>(p, kOffHorz); }
inline CTime_atl&   Time(void* p)              { return At<CTime_atl>(p, kOffTime); }
inline const CTime_atl& Time(const void* p)    { return At<CTime_atl>(p, kOffTime); }

inline HWND HWndOf(const CWnd* pWnd) { return pWnd ? pWnd->m_hWnd : nullptr; }
// `pWnd->GetSafeHwnd() != NULL` as retail spells it: the pointer, then +0x40.
inline bool HasWindow(const CWnd* pWnd) { return pWnd != nullptr && pWnd->m_hWnd != nullptr; }
// `(pWnd->GetStyle() & WS_VISIBLE) != 0` -- retail: call ?GetStyle@CWnd@@ (0x2a9690), `bt $0x1c,%eax`.
inline bool StyleVisible(const CWnd* pWnd) { return (impl__GetStyle_CWnd__QEBAKXZ(pWnd) & WS_VISIBLE) != 0; }
// The three-part gate almost every body here opens with:
//     m_pWndDateTime != NULL && m_pWndDateTime->m_hWnd != NULL && (m_pWndDateTime->GetStyle() & WS_VISIBLE)
inline bool HasVisibleWindow(const void* p) {
    const CWnd* pWnd = WndDateTime(p);
    return HasWindow(pWnd) && StyleVisible(pWnd);
}

// Message / style numbers the retail bodies use as immediates.
constexpr UINT     kDtmGetSystemTime   = 0x1001;       // DTM_GETSYSTEMTIME
constexpr UINT     kDtmSetSystemTime   = 0x1002;       // DTM_SETSYSTEMTIME
constexpr int      kDtnDateTimeChange  = -759;         // DTN_DATETIMECHANGE (0xFFFFFD09 as retail loads it)
constexpr unsigned kGdtValid           = 0;            // GDT_VALID
constexpr unsigned kGdtNone            = 1;            // GDT_NONE
constexpr UINT     kWmSetFont          = 0x30;         // WM_SETFONT
constexpr unsigned kTbbsDisabled       = 0x40000;      // TBBS_DISABLED
constexpr unsigned kSwpNoZorderNoActivate = 0x14;      // SWP_NOZORDER | SWP_NOACTIVATE
constexpr DWORD    kDefaultStyle       = 0x50000000;   // WS_CHILD | WS_VISIBLE
constexpr int      kDefaultWidth       = 100;          // 0x64
constexpr unsigned kDtTextLabel        = 0x11;         // DT_CENTER | DT_WORDBREAK               (OnDraw label)
constexpr unsigned kDtCalcLabel        = 0x411;        // DT_CALCRECT | DT_CENTER | DT_WORDBREAK (OnCalculateSize)
constexpr int      kCtrlHeightExtra    = 6;            // the "+ 6" OnMove / OnSize / OnChangeParentWnd add to GetTextHeight()
static_assert(kDtmGetSystemTime == DTM_GETSYSTEMTIME && kDtmSetSystemTime == DTM_SETSYSTEMTIME, "DTM numbers");
static_assert(static_cast<unsigned>(kDtnDateTimeChange) == static_cast<unsigned>(DTN_DATETIMECHANGE), "DTN_DATETIMECHANGE");
static_assert(kGdtValid == GDT_VALID && kGdtNone == GDT_NONE, "GDT flags");
static_assert(kWmSetFont == WM_SETFONT, "WM_SETFONT");

// ---------------------------------------------------------------------------
// afxGlobalData (see the file header for how each slot was established).
// ---------------------------------------------------------------------------
constexpr int kGlobalDataInitGate          = 0x000;
constexpr int kGlobalDataClrGrayedText     = 0x044;
constexpr int kGlobalDataClrBarFace        = 0x060;
constexpr int kGlobalDataClrBarShadow      = 0x064;
constexpr int kGlobalDataClrBarHilite      = 0x068;
constexpr int kGlobalDataClrBarDkShadow    = 0x06c;
constexpr int kGlobalDataClrBarLight       = 0x070;
constexpr int kGlobalDataClrBarText        = 0x074;
constexpr int kGlobalDataClrWindow         = 0x078;
constexpr int kGlobalDataClrWindowText     = 0x07c;
constexpr int kGlobalDataBrWindowHandle    = 0x110;
constexpr int kGlobalDataFontRegularHandle = 0x1b0;
constexpr int kGlobalDataTextHeightHorz    = 0x2a8;

inline unsigned char* GlobalDataBytes() { return impl__afxGlobalData__3UAFX_GLOBAL_DATA__A; }
inline int GlobalDataInt(int off) { int v; std::memcpy(&v, GlobalDataBytes() + off, sizeof v); return v; }
inline void* GlobalDataPtr(int off) { void* v; std::memcpy(&v, GlobalDataBytes() + off, sizeof v); return v; }

// The retail one-time gate every reader does first:
//     if (afxGlobalData.<+0> == 0) { afxGlobalData.Initialize(); <+0> = 1; }    // 0x6a790
inline void EnsureGlobalDataInitialized() {
    if (GlobalDataInt(kGlobalDataInitGate) == 0) {
        impl__Initialize_AFX_GLOBAL_DATA__QEAAXXZ(GlobalDataBytes());
        const int one = 1;
        std::memcpy(GlobalDataBytes() + kGlobalDataInitGate, &one, sizeof one);
    }
}
// The block is populated once UpdateSysColors has created brWindow (+0x110).
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
    if (h == nullptr) h = static_cast<HFONT>(::GetStockObject(DEFAULT_GUI_FONT));   // fallback, see the file header
    return h;
}
// afxGlobalData.GetTextHeight() == m_nTextHeightHorz (+0x2a8).  Fallback when
// the slot is still zero: the UpdateTextMetrics formula on the regular font.
inline int GlobalTextHeightHorz() {
    EnsureGlobalDataInitialized();
    const int h = GlobalDataInt(kGlobalDataTextHeightHorz);
    if (h != 0) return h;
    HDC hdc = ::GetDC(nullptr);
    if (hdc == nullptr) return 0;
    HGDIOBJ hOld = ::SelectObject(hdc, GlobalRegularFont());
    TEXTMETRICW tm;
    std::memset(&tm, 0, sizeof tm);
    ::GetTextMetricsW(hdc, &tm);
    if (hOld != nullptr) ::SelectObject(hdc, hOld);
    ::ReleaseDC(nullptr, hdc);
    return tm.tmHeight + (tm.tmHeight < 15 ? 2 : 5);
}

// CMFCToolBarButton::IsEditable() as the retail vftable slot 26 body reads
// (RVA 0x23760, mfc140u -- the inline from afxtoolbarbutton.h, IsStandardCommand
// folded in), the same transcription CMFCToolBarComboBoxButton.cpp carries:
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
inline bool IsEditableInline(const void* p) {
    const unsigned id = Base(p)->m_nID;
    if (id - 0xE110u <= 0xFu) return false;
    if (id - 0xF000u <= 0x1EFu) return false;
    if (static_cast<int>(id) >= 0xFF00) return false;
    if (id - 0xE210u <= 0xFu) return false;
    return true;
}

// CObList walk (the pattern of CMFCToolBarComboBoxButton.cpp / docking/CMultiPaneFrameWnd.cpp):
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
// The objects the two constructors below have built and the destructor has
// not yet torn down (see "CONSEQUENCE FOR RTTI" in the file header for why
// this record exists).
// A singly-linked list whose nodes come from the exported ??2@ / ??3@ (no C++
// runtime allocation from this translation unit).
struct LiveNode {
    LiveNode*   pNext;
    const void* pObject;
};
LiveNode* g_pLiveObjects = nullptr;
inline bool IsLive(const void* p) {
    for (const LiveNode* n = g_pLiveObjects; n != nullptr; n = n->pNext)
        if (n->pObject == p) return true;
    return false;
}
inline void RecordLive(const void* p) {
    if (IsLive(p)) return;
    LiveNode* n = static_cast<LiveNode*>(impl___2_YAPEAX_K_Z(sizeof(LiveNode)));
    if (n == nullptr) return;
    n->pNext = g_pLiveObjects;
    n->pObject = p;
    g_pLiveObjects = n;
}
inline void ForgetLive(const void* p) {
    for (LiveNode** pp = &g_pLiveObjects; *pp != nullptr; ) {
        LiveNode* n = *pp;
        if (n->pObject == p) {
            *pp = n->pNext;
            impl___3_YAXPEAX_Z(n);
        } else {
            pp = &n->pNext;
        }
    }
}
// `p->IsKindOf(RUNTIME_CLASS(CMFCToolBarDateTimeCtrl))` -- retail 0x234cf0 against
// the descriptor at 0x3b1688 (mfc140u).  IsKindOf alone only recognises a
// client-derived object with its own MSVC vftable; an object built by this
// file's constructors is recognised through g_pLiveObjects (file header).
inline bool IsDateTimeButton(const CObject* p) {
    if (p == nullptr) return false;
    if (IsLive(p)) return true;
    return impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(p, impl__GetThisClass_CMFCToolBarDateTimeCtrl__SAPEAUCRuntimeClass__XZ()) != 0;
}

// m_pWndDateTime->GetTime(m_time) as retail's callee (0x29a490, mfc140u,
// ?GetTime@CDateTimeCtrl@@QEBAKAEAVCTime@ATL@@@Z) returns it: the raw
// DTM_GETSYSTEMTIME result (GDT_VALID == 0 / GDT_NONE == 1), the CTime written
// only when it is GDT_VALID.  OpenMFC's thunk for that export
// (core/controls/CDateTimeCtrl.cpp) returns 1-on-valid / 0-on-none instead --
// the inverse -- and NotifyCommand / Serialize / GetTimeAll below store or
// return the DTM flag, so the status is read straight off the control here and
// the thunk is used only for its SYSTEMTIME -> __time64_t conversion, which
// is the exact inverse of the one the SetTime(const CTime*) thunk applies when
// the sibling and Serialize paths push the value back.
inline DWORD ReadControlTime(const CWnd* pWnd, CTime_atl& t) {
    SYSTEMTIME st;
    std::memset(&st, 0, sizeof st);
    const DWORD dwStatus = static_cast<DWORD>(::SendMessage(pWnd->m_hWnd, kDtmGetSystemTime, 0, reinterpret_cast<LPARAM>(&st)));
    if (dwStatus == kGdtValid) impl__GetTime_CDateTimeCtrl__QEBAKAEAVCTime_ATL___Z(pWnd, &t);
    return dwStatus;
}
// m_pWndDateTime->SetTime(pTime) as retail's callee (0x29a390, mfc140u,
// ?SetTime@CDateTimeCtrl@@QEAAHPEBVCTime@ATL@@@Z) does it: DTM_SETSYSTEMTIME
// with GDT_NONE for a NULL pointer (or a failed GetAsSystemTime), GDT_VALID
// otherwise.  OpenMFC's thunk returns FALSE for NULL without touching the
// control, so the NULL case is sent here.
inline int PushControlTime(CWnd* pWnd, const void* pTime) {
    if (pTime != nullptr) return impl__SetTime_CDateTimeCtrl__QEAAHPEBVCTime_ATL___Z(pWnd, pTime);
    SYSTEMTIME st;
    std::memset(&st, 0, sizeof st);
    return static_cast<int>(::SendMessage(pWnd->m_hWnd, kDtmSetSystemTime, kGdtNone, reinterpret_cast<LPARAM>(&st)));
}

// CArchive scalar I/O through the exported Read / Write, in the byte layout
// OpenMFC's own CArchive::operator<< / >> use (core/runtime/CArchive.cpp:
// 4-byte int).  Retail inlines the int case as direct buffer stores
// (FillBuffer 0x1d1cc0 / Flush 0x1d1be0 on a short buffer) and the CTime case
// as the outlined ??5 / ??6 CArchive-CTime operators (0x274ac0 / 0x274a20),
// which move the one 8-byte __time64_t; OpenMFC's thunks for those two
// operators are null stubs (core/runtime/CArchive.cpp), so the 8 bytes go
// through Read / Write directly here.
inline void ArWriteInt(CArchive* ar, int v) { impl__Write_CArchive__QEAAXPEBXI_Z(ar, &v, sizeof v); }
inline int ArReadInt(CArchive* ar) { int v = 0; impl__Read_CArchive__QEAAIPEAXI_Z(ar, &v, sizeof v); return v; }
inline void ArWriteTime(CArchive* ar, const CTime_atl& t) { impl__Write_CArchive__QEAAXPEBXI_Z(ar, &t.m_time, sizeof t.m_time); }
inline void ArReadTime(CArchive* ar, CTime_atl& t) { t.m_time = 0; impl__Read_CArchive__QEAAIPEAXI_Z(ar, &t.m_time, sizeof t.m_time); }

// The vptr the base constructor thunk installs (the mingw CMFCToolBarButton
// one), recorded by the first constructor to run; the destructor stores it
// back at +0x00 first, which is what retail's `vfptr = &vftable' store does.
void* g_ownVptr = nullptr;

// The window object this button owns.  Retail tears it down with
//     pWnd->DestroyWindow();          // vftable +0xd0 (slot 26)
//     pWnd->`scalar deleting dtor`(1) // vftable +0x08 (slot 1)
// i.e. through the object's own MSVC vftable.  An object this file created
// (CreateDateTimeCtrl below) carries OpenMFC's mingw CWnd vftable, on which
// those slot numbers mean something else, so such an object is released
// through the exported ?DestroyWindow@CWnd@@ / ??1CWnd@@ / ??3@ instead;
// g_ownCWndVptr (the vptr ??0CWnd@@QEAA@XZ installs, recorded when the first
// one is built) tells the two apart.  Any other object -- one built against
// the real MFC headers that a client planted in m_pWndDateTime -- is released
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

// CWnd::GetParent() as retail inlines it here: CWnd::FromHandle(::GetParent(m_hWnd)).   // 0x28ad70
inline CWnd* ParentOf(const CWnd* pWnd) {
    return impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(::GetParent(pWnd->m_hWnd));
}

} // namespace

// ===========================================================================
// Construction / destruction
// ===========================================================================

// Retail ??0CMFCToolBarDateTimeCtrl@@QEAA@IHKH@Z (RVA 0x165b90, mfc140u), fully transcribed:
//     CMFCToolBarButton::CMFCToolBarButton(uiID, iImage, NULL, FALSE, FALSE);   // 0x15bf40
//     vfptr = &CMFCToolBarDateTimeCtrl::`vftable';                             // 0x1803170a0
//     m_dwStyle = dwStyle | 0x50000000;                                        // WS_CHILD | WS_VISIBLE
//     m_time    = 0;
//     m_iWidth  = iWidth != 0 ? iWidth : 100;
//     Initialize();                                                            // 0x165c10
// The vfptr is left as the base constructor thunk installs it and the object
// is recorded in g_pLiveObjects instead (file header).
// Symbol: ??0CMFCToolBarDateTimeCtrl@@QEAA@IHKH@Z
extern "C" void* MS_ABI impl___0CMFCToolBarDateTimeCtrl__QEAA_IHKH_Z(
    void* pThis, unsigned int uiID, int iImage, unsigned long dwStyle, int iWidth) {
    if (!pThis) return pThis;
    impl___0CMFCToolBarButton__QEAA_IHPEB_WHH_Z(pThis, uiID, iImage, nullptr, 0, 0);
    if (g_ownVptr == nullptr) g_ownVptr = *static_cast<void**>(pThis);
    RecordLive(pThis);
    Style(pThis) = static_cast<DWORD>(dwStyle) | kDefaultStyle;
    Time(pThis).m_time = 0;
    Width(pThis) = iWidth != 0 ? iWidth : kDefaultWidth;
    impl__Initialize_CMFCToolBarDateTimeCtrl__IEAAXXZ(pThis);
    return pThis;
}

// Retail ??0CMFCToolBarDateTimeCtrl@@QEAA@XZ (RVA 0x165af0, mfc140u), fully transcribed:
//     CMFCToolBarButton::CMFCToolBarButton();          // 0x15bec0
//     vfptr = &CMFCToolBarDateTimeCtrl::`vftable';     // 0x1803170a0
//     m_dwStyle = 0x50000000;                          // WS_CHILD | WS_VISIBLE
//     m_time    = 0;
//     m_iWidth  = 100;
//     Initialize();                                    // 0x165c10
// The vfptr / g_pLiveObjects handling is the four-argument constructor's.
// Symbol: ??0CMFCToolBarDateTimeCtrl@@QEAA@XZ
extern "C" void* MS_ABI impl___0CMFCToolBarDateTimeCtrl__QEAA_XZ(void* pThis) {
    if (!pThis) return pThis;
    impl___0CMFCToolBarButton__QEAA_XZ(pThis);
    if (g_ownVptr == nullptr) g_ownVptr = *static_cast<void**>(pThis);
    RecordLive(pThis);
    Style(pThis) = kDefaultStyle;
    Time(pThis).m_time = 0;
    Width(pThis) = kDefaultWidth;
    impl__Initialize_CMFCToolBarDateTimeCtrl__IEAAXXZ(pThis);
    return pThis;
}

// Retail ??1CMFCToolBarDateTimeCtrl@@UEAA@XZ (RVA 0x165c50, mfc140u), fully transcribed:
//     vfptr = &CMFCToolBarDateTimeCtrl::`vftable';                                   // 0x1803170a0
//     if (m_pWndDateTime != NULL) {
//         m_pWndDateTime->DestroyWindow();                                           // vslot 0xd0 (26)
//         if (m_pWndDateTime != NULL) m_pWndDateTime->`scalar deleting dtor'(1);     // vslot 0x08 (1)
//     }
//     CMFCToolBarButton::~CMFCToolBarButton();                                       // 0x15c130 (tail jump)
// The vfptr store is reproduced with the recorded base vptr (file header);
// the window goes through DestroyOwnedWindow / DeleteOwnedWindow.  Retail does
// not NULL m_pWndDateTime (the object is dying); it is cleared here so that a
// second call cannot double-free.  The object leaves g_pLiveObjects first.
// Symbol: ??1CMFCToolBarDateTimeCtrl@@UEAA@XZ
extern "C" void MS_ABI impl___1CMFCToolBarDateTimeCtrl__UEAA_XZ(void* pThis) {
    if (!pThis) return;
    ForgetLive(pThis);
    if (g_ownVptr != nullptr) *static_cast<void**>(pThis) = g_ownVptr;
    if (CWnd* pWnd = WndDateTime(pThis)) {
        DestroyOwnedWindow(pWnd);
        DeleteOwnedWindow(pWnd);
        WndDateTime(pThis) = nullptr;
    }
    impl___1CMFCToolBarButton__UEAA_XZ(pThis);
}

// Retail (RVA 0x165c10, mfc140u), fully transcribed:
//     m_pWndDateTime = NULL;          // +0x88
//     m_bHorz        = TRUE;          // +0x9c
//     m_dwTimeStatus = 0;             // +0x94 (GDT_VALID)
//     m_time         = _time64(NULL); // +0xa0 -- CTime::GetCurrentTime(), the CRT import at IAT 0x1802c7848
// Symbol: ?Initialize@CMFCToolBarDateTimeCtrl@@IEAAXXZ
extern "C" void MS_ABI impl__Initialize_CMFCToolBarDateTimeCtrl__IEAAXXZ(void* pThis) {
    if (!pThis) return;
    WndDateTime(pThis) = nullptr;
    Horz(pThis) = TRUE;
    TimeStatus(pThis) = kGdtValid;
    Time(pThis).m_time = _time64(nullptr);
}

// Retail (RVA 0x165980, mfc140u), fully transcribed:
//     void* p = operator new(0xa8);                              // 0x27f0
//     return p != NULL ? new (p) CMFCToolBarDateTimeCtrl() : NULL;   // 0x165af0
// Symbol: ?CreateObject@CMFCToolBarDateTimeCtrl@@SAPEAVCObject@@XZ
extern "C" CObject* MS_ABI impl__CreateObject_CMFCToolBarDateTimeCtrl__SAPEAVCObject__XZ() {
    void* p = impl___2_YAPEAX_K_Z(kObjectSize);
    if (p == nullptr) return nullptr;
    std::memset(p, 0, kObjectSize);
    return static_cast<CObject*>(impl___0CMFCToolBarDateTimeCtrl__QEAA_XZ(p));
}

// Retail (RVA 0x165cb0, mfc140u), fully transcribed:
//     CMFCToolBarButton::CopyFrom(src);     // 0x15c1a0
//     DuplicateData();                      // vslot 0x1a8 (53) -- the `ret` at 0x27d0 in this class
//     m_dwStyle = src.m_dwStyle;            // +0x90
//     m_iWidth  = src.m_iWidth;             // +0x98
// (m_dwTimeStatus, m_time and m_bHorz are not copied.)  DuplicateData is the
// retail no-op and is not dispatched (file header).
// Symbol: ?CopyFrom@CMFCToolBarDateTimeCtrl@@UEAAXAEBVCMFCToolBarButton@@@Z
extern "C" void MS_ABI impl__CopyFrom_CMFCToolBarDateTimeCtrl__UEAAXAEBVCMFCToolBarButton___Z(void* pThis, const CMFCToolBarButton* pSrc) {
    if (!pThis || !pSrc) return;
    impl__CopyFrom_CMFCToolBarButton__UEAAXAEBV1__Z(pThis, pSrc);
    Style(pThis) = Style(pSrc);
    Width(pThis) = Width(pSrc);
}

// Retail (vftable slot 2 of 0x1803170a0 -> RVA 0x165d00, mfc140u), fully
// transcribed.  The int reads / writes are CArchive's inline buffer accesses,
// the CTime ones the outlined archive operators at 0x274ac0 (>>) / 0x274a20 (<<):
//     CMFCToolBarButton::Serialize(ar);                              // 0x15c240
//     if (ar.IsLoading()) {                                          // ar.m_nMode & 1 (+0x20)
//         ar >> m_iWidth;  m_rect.right = m_rect.left + m_iWidth;
//         ar >> m_dwStyle;
//         ar >> m_dwTimeStatus;
//         ar >> m_time;
//         if (m_pWndDateTime->GetSafeHwnd() != NULL && m_dwTimeStatus == GDT_VALID)
//             m_pWndDateTime->SetTime(&m_time);                      // 0x29a390, CDateTimeCtrl::SetTime(const CTime*)
//         DuplicateData();                                           // vslot 0x1a8 (53), the retail no-op
//     } else {
//         if (m_pWndDateTime->GetSafeHwnd() != NULL) m_dwTimeStatus = m_pWndDateTime->GetTime(m_time);   // 0x29a490
//         ar << m_iWidth;  ar << m_dwStyle;  ar << m_dwTimeStatus;  ar << m_time;
//     }
// The scalars use OpenMFC's own archive byte layout (ArWriteInt / ArWriteTime);
// the two CDateTimeCtrl calls go through PushControlTime / ReadControlTime.
// Symbol: ?Serialize@CMFCToolBarDateTimeCtrl@@UEAAXAEAVCArchive@@@Z
extern "C" void MS_ABI impl__Serialize_CMFCToolBarDateTimeCtrl__UEAAXAEAVCArchive___Z(void* pThis, CArchive* ar) {
    if (!pThis || !ar) return;
    impl__Serialize_CMFCToolBarButton__UEAAXAEAVCArchive___Z(pThis, ar);
    CWnd* pWnd = WndDateTime(pThis);
    if (ar->IsLoading()) {
        Width(pThis) = ArReadInt(ar);
        Base(pThis)->m_rect.right = Base(pThis)->m_rect.left + Width(pThis);
        Style(pThis) = static_cast<DWORD>(ArReadInt(ar));
        TimeStatus(pThis) = static_cast<DWORD>(ArReadInt(ar));
        ArReadTime(ar, Time(pThis));
        if (HasWindow(pWnd) && TimeStatus(pThis) == kGdtValid) {
            PushControlTime(pWnd, &Time(pThis));
        }
    } else {
        if (HasWindow(pWnd)) {
            TimeStatus(pThis) = ReadControlTime(pWnd, Time(pThis));
        }
        ArWriteInt(ar, Width(pThis));
        ArWriteInt(ar, static_cast<int>(Style(pThis)));
        ArWriteInt(ar, static_cast<int>(TimeStatus(pThis)));
        ArWriteTime(ar, Time(pThis));
    }
}

// ===========================================================================
// Layout
// ===========================================================================

// Retail (RVA 0x1664c0, mfc140u), fully transcribed:
//     if (m_pWndDateTime == NULL || m_pWndDateTime->m_hWnd == NULL ||
//         !(m_pWndDateTime->GetStyle() & WS_VISIBLE) || ::IsRectEmpty(&m_rect)) return;
//     ::GetWindowRect(m_pWndDateTime->m_hWnd, &m_rect);
//     m_pWndDateTime->ScreenToClient(&m_rect);                                        // 0x2a32b0
//     CWnd* pParent = CWnd::FromHandle(::GetParent(m_pWndDateTime->m_hWnd));          // 0x28ad70
//     ::MapWindowPoints(m_pWndDateTime->m_hWnd, pParent ? pParent->m_hWnd : NULL, (POINT*)&m_rect, 2);
//     ::InflateRect(&m_rect, 3, 3);
// Symbol: ?AdjustRect@CMFCToolBarDateTimeCtrl@@IEAAXXZ
extern "C" void MS_ABI impl__AdjustRect_CMFCToolBarDateTimeCtrl__IEAAXXZ(void* pThis) {
    if (!pThis) return;
    CWnd* pWnd = WndDateTime(pThis);
    RECT* const pRect = static_cast<RECT*>(Base(pThis)->m_rect);    // CRect::operator RECT*()
    if (!HasVisibleWindow(pThis) || ::IsRectEmpty(pRect)) return;
    ::GetWindowRect(pWnd->m_hWnd, pRect);
    impl__ScreenToClient_CWnd__QEBAXPEAUtagRECT___Z(pWnd, pRect);
    CWnd* pParent = ParentOf(pWnd);
    ::MapWindowPoints(pWnd->m_hWnd, HWndOf(pParent), reinterpret_cast<POINT*>(pRect), 2);
    ::InflateRect(pRect, 3, 3);
}

// Retail (vftable slot 14 of 0x1803170a0 -> RVA 0x1660b0, mfc140u), fully transcribed:
//     if (m_pWndDateTime == NULL || m_pWndDateTime->m_hWnd == NULL || !(GetStyle() & WS_VISIBLE)) return;
//     CRect rectCtrl(0, 0, 0, 0);  ::GetWindowRect(m_pWndDateTime->m_hWnd, &rectCtrl);
//     (afxGlobalData init gate)
//     m_pWndDateTime->SetWindowPos(NULL,                                              // 0x2a9a60
//         m_rect.left + 3,
//         m_rect.top + (m_rect.Height() - m_sizeText.cy - rectCtrl.Height()) / 2,
//         m_rect.Width() - 6,
//         afxGlobalData.GetTextHeight() /*+0x2a8*/ + 6,
//         SWP_NOZORDER | SWP_NOACTIVATE);
//     AdjustRect();                                                                   // 0x1664c0
// Symbol: ?OnMove@CMFCToolBarDateTimeCtrl@@UEAAXXZ
extern "C" void MS_ABI impl__OnMove_CMFCToolBarDateTimeCtrl__UEAAXXZ(void* pThis) {
    if (!pThis) return;
    if (!HasVisibleWindow(pThis)) return;
    CWnd* pWnd = WndDateTime(pThis);
    RECT rectCtrl = { 0, 0, 0, 0 };
    ::GetWindowRect(pWnd->m_hWnd, &rectCtrl);
    const int cyText = GlobalTextHeightHorz();
    const CRect& rect = Base(pThis)->m_rect;
    const int y = rect.top + ((rect.bottom - rect.top) - Base(pThis)->m_sizeText.cy - (rectCtrl.bottom - rectCtrl.top)) / 2;
    impl__SetWindowPos_CWnd__QEAAHPEBV1_HHHHI_Z(pWnd, nullptr, rect.left + 3, y,
                                                (rect.right - rect.left) - 6, cyText + kCtrlHeightExtra,
                                                kSwpNoZorderNoActivate);
    impl__AdjustRect_CMFCToolBarDateTimeCtrl__IEAAXXZ(pThis);
}

// Retail (vftable slot 15 of 0x1803170a0 -> RVA 0x1661c0, mfc140u), fully transcribed:
//     m_iWidth = iSize;                                      // +0x98
//     m_rect.right = m_rect.left + iSize;                    // +0x70 = +0x68 + iSize
//     if (m_pWndDateTime == NULL || m_pWndDateTime->m_hWnd == NULL || !(GetStyle() & WS_VISIBLE)) return;
//     (afxGlobalData init gate)
//     m_pWndDateTime->SetWindowPos(NULL, m_rect.left + 3, m_rect.top, m_rect.Width() - 6,
//                                  afxGlobalData.GetTextHeight() + 6, SWP_NOZORDER | SWP_NOACTIVATE);   // 0x2a9a60
//     AdjustRect();                                          // 0x1664c0
// Symbol: ?OnSize@CMFCToolBarDateTimeCtrl@@UEAAXH@Z
extern "C" void MS_ABI impl__OnSize_CMFCToolBarDateTimeCtrl__UEAAXH_Z(void* pThis, int iSize) {
    if (!pThis) return;
    Width(pThis) = iSize;
    CRect& rect = Base(pThis)->m_rect;
    rect.right = rect.left + iSize;
    if (!HasVisibleWindow(pThis)) return;
    const int cyText = GlobalTextHeightHorz();
    impl__SetWindowPos_CWnd__QEAAHPEBV1_HHHHI_Z(WndDateTime(pThis), nullptr, rect.left + 3, rect.top,
                                                (rect.right - rect.left) - 6, cyText + kCtrlHeightExtra,
                                                kSwpNoZorderNoActivate);
    impl__AdjustRect_CMFCToolBarDateTimeCtrl__IEAAXXZ(pThis);
}

// Retail (RVA 0x165f70, mfc140u), fully transcribed.  SIZE comes back through
// the hidden pointer in RDX (pDC in R8, sizeDefault in R9, bHorz on the stack):
//     if (!m_bVisible) return CSize(0, 0);                            // +0x64
//     m_bHorz = bHorz;  m_sizeText = CSize(0, 0);                     // +0x9c, +0x78
//     if (!bHorz) {
//         if (m_pWndDateTime->GetSafeHwnd() && (m_pWndDateTime->GetStyle() & WS_VISIBLE)) m_pWndDateTime->ShowWindow(SW_HIDE);   // 0x2a9ad0
//         return CMFCToolBarButton::OnCalculateSize(pDC, sizeDefault, FALSE);     // 0x15d680
//     }
//     if (m_pWndDateTime->GetSafeHwnd() && !m_bIsHidden) m_pWndDateTime->ShowWindow(SW_SHOWNOACTIVATE);   // +0x54
//     if (m_bTextBelow && !m_strText.IsEmpty()) {                      // +0x1c, +0x38
//         CRect rectText(0, 0, m_iWidth, sizeDefault.cy);
//         pDC->DrawText(m_strText, rectText, DT_CALCRECT | DT_CENTER | DT_WORDBREAK);   // CDC vslot 0xe0 (28)
//         m_sizeText = rectText.Size();
//     }
//     return CSize(m_iWidth, sizeDefault.cy + m_sizeText.cy);
// DEVIATIONS: the base OnCalculateSize is a null stub in OpenMFC
// (featurepack/toolbar/CMFCToolBarButton.cpp), so the vertical case yields
// (0, 0); DrawText goes through the exported CDC thunk.
// Symbol: ?OnCalculateSize@CMFCToolBarDateTimeCtrl@@UEAA?AUtagSIZE@@PEAVCDC@@AEBVCSize@@H@Z
extern "C" SIZE* MS_ABI impl__OnCalculateSize_CMFCToolBarDateTimeCtrl__UEAA_AUtagSIZE__PEAVCDC__AEBVCSize__H_Z(
    void* pThis, SIZE* pRet, CDC* pDC, const SIZE* pSizeDefault, int bHorz)
{
    if (!pRet) return pRet;
    pRet->cx = 0; pRet->cy = 0;
    if (!pThis || !pSizeDefault) return pRet;
    CMFCToolBarButton* pBase = Base(pThis);
    if (pBase->m_bVisible == 0) return pRet;

    Horz(pThis) = bHorz;
    pBase->m_sizeText.cx = 0;
    pBase->m_sizeText.cy = 0;
    CWnd* pWnd = WndDateTime(pThis);

    if (bHorz == 0) {
        if (HasWindow(pWnd) && StyleVisible(pWnd)) impl__ShowWindow_CWnd__QEAAHH_Z(pWnd, SW_HIDE);
        impl__OnCalculateSize_CMFCToolBarButton__UEAA_AUtagSIZE__PEAVCDC__AEBVCSize__H_Z(pBase, pRet, pDC, pSizeDefault, 0);
        return pRet;
    }
    if (HasWindow(pWnd) && pBase->m_bIsHidden == 0) impl__ShowWindow_CWnd__QEAAHH_Z(pWnd, SW_SHOWNOACTIVATE);
    if (pBase->m_bTextBelow != 0 && pBase->m_strText.GetLength() != 0 && pDC != nullptr) {
        RECT rectText = { 0, 0, Width(pThis), pSizeDefault->cy };
        impl__DrawTextW_CDC__QEAAHPEB_WHPEAUTAGRECT__I_Z(pDC, static_cast<const wchar_t*>(pBase->m_strText),
                                                         pBase->m_strText.GetLength(), &rectText, kDtCalcLabel);
        pBase->m_sizeText.cx = rectText.right - rectText.left;
        pBase->m_sizeText.cy = rectText.bottom - rectText.top;
    }
    pRet->cx = Width(pThis);
    pRet->cy = pSizeDefault->cy + pBase->m_sizeText.cy;
    return pRet;
}

// ===========================================================================
// Window creation / parent changes
// ===========================================================================

// Retail (RVA 0x166ae0, mfc140u), fully transcribed:
//     CMFCToolBarDateTimeCtrlImpl* pWnd = new CMFCToolBarDateTimeCtrlImpl;   // operator new(0xf0) 0x27f0, ??0CWnd@@ 0x28a700,
//                                                                            // vfptr = 0x1803172f8, m_bMonthCtrlDisplayed (+0xe8) = false
//     if (!pWnd->Create(m_dwStyle, rect, pWndParent, m_nID)) {               // vslot 0x2d8 (91); devirtualised to
//         delete pWnd;                                                       // ?Create@CDateTimeCtrl@@ (0x299f40) =
//         return NULL;                                                       // AfxEndDeferRegisterClass(0x20000) +
//     }                                                                      // CreateEx(0, "SysDateTimePick32", ...) when the slot is the base one
//     return pWnd;
// DEVIATIONS: the object is built through ??2@YAPEAX_K@Z + the exported
// ??0CWnd@@QEAA@XZ (file header), so its runtime class reads CWnd and its
// vftable is OpenMFC's CWnd one; Create goes through the exported
// CDateTimeCtrl::Create; an allocation failure returns NULL where retail
// (which does not test the operator-new result before `mov (%rbx),%rax`) faults.
// Symbol: ?CreateDateTimeCtrl@CMFCToolBarDateTimeCtrl@@MEAAPEAVCMFCToolBarDateTimeCtrlImpl@@PEAVCWnd@@AEBVCRect@@@Z
extern "C" void* MS_ABI impl__CreateDateTimeCtrl_CMFCToolBarDateTimeCtrl__MEAAPEAVCMFCToolBarDateTimeCtrlImpl__PEAVCWnd__AEBVCRect___Z(
    void* pThis, CWnd* pWndParent, const RECT* pRect)
{
    if (!pThis || !pRect) return nullptr;
    void* pStorage = impl___2_YAPEAX_K_Z(kImplObjectSize);
    if (!pStorage) return nullptr;
    std::memset(pStorage, 0, kImplObjectSize);
    CWnd* pWnd = static_cast<CWnd*>(impl___0CWnd__QEAA_XZ(pStorage));
    if (g_ownCWndVptr == nullptr) g_ownCWndVptr = *reinterpret_cast<void**>(pWnd);
    At<bool>(pWnd, kOffImplMonthCtrlDisplayed) = false;
    if (!impl__Create_CDateTimeCtrl__UEAAHKAEBUtagRECT__PEAVCWnd__I_Z(pWnd, Style(pThis), pRect, pWndParent, Base(pThis)->m_nID)) {
        DeleteOwnedWindow(pWnd);
        return nullptr;
    }
    return pWnd;
}

// Retail (RVA 0x166270, mfc140u), fully transcribed:
//     CMFCToolBarButton::OnChangeParentWnd(pWndParent);                              // 0x15e570
//     if (m_pWndDateTime->GetSafeHwnd() != NULL) {
//         CWnd* pOldParent = m_pWndDateTime->GetParent();  (AfxThrowInvalidArgException if NULL, the call at 0x1664b8)
//         if (pWndParent != NULL && pOldParent->m_hWnd == pWndParent->m_hWnd) return;
//         m_pWndDateTime->DestroyWindow();                                           // vslot 0xd0 (26)
//         if (m_pWndDateTime) delete m_pWndDateTime;                                 // vslot 0x08 (1)
//         m_pWndDateTime = NULL;
//     }
//     if (pWndParent == NULL || pWndParent->m_hWnd == NULL) return;
//     CRect rect = m_rect;  ::InflateRect(&rect, -2, 0);
//     (afxGlobalData init gate)
//     rect.bottom = rect.top + afxGlobalData.GetTextHeight() /*+0x2a8*/ + 6;
//     m_pWndDateTime = CreateDateTimeCtrl(pWndParent, rect);                          // vslot 0x1b0 (54)
//     if (m_pWndDateTime == NULL) return;
//     if (m_pWndDateTime->m_hWnd && (GetStyle() & WS_VISIBLE) && !::IsRectEmpty(&m_rect)) {
//         ... AdjustRect's body inlined (GetWindowRect / ScreenToClient / MapWindowPoints / InflateRect 3,3) ...
//     }
//     (afxGlobalData init gate)
//     m_pWndDateTime->SendMessage(WM_SETFONT, afxGlobalData.fontRegular /*+0x1b0*/, TRUE);
//     if (m_dwTimeStatus == GDT_VALID) m_pWndDateTime->SetTime(&m_time);              // 0x29a390
// DEVIATIONS: the base OnChangeParentWnd is a null stub in OpenMFC (so
// m_pWndParent is not recorded here); a window whose parent cannot be resolved
// is torn down and recreated where retail throws; CreateDateTimeCtrl is reached
// through this file's thunk instead of the vftable (file header); the inlined
// AdjustRect is the exported one (same guards).
// Symbol: ?OnChangeParentWnd@CMFCToolBarDateTimeCtrl@@UEAAXPEAVCWnd@@@Z
extern "C" void MS_ABI impl__OnChangeParentWnd_CMFCToolBarDateTimeCtrl__UEAAXPEAVCWnd___Z(void* pThis, CWnd* pWndParent) {
    if (!pThis) return;
    impl__OnChangeParentWnd_CMFCToolBarButton__UEAAXPEAVCWnd___Z(Base(pThis), pWndParent);

    if (HasWindow(WndDateTime(pThis))) {
        CWnd* pOldParent = ParentOf(WndDateTime(pThis));
        if (pOldParent != nullptr && pWndParent != nullptr && pOldParent->m_hWnd == pWndParent->m_hWnd) return;
        CWnd* pOld = WndDateTime(pThis);
        DestroyOwnedWindow(pOld);
        DeleteOwnedWindow(pOld);
        WndDateTime(pThis) = nullptr;
    }
    if (!HasWindow(pWndParent)) return;

    RECT rect = *static_cast<const RECT*>(Base(pThis)->m_rect);    // CRect::operator const RECT*()
    ::InflateRect(&rect, -2, 0);
    rect.bottom = rect.top + GlobalTextHeightHorz() + kCtrlHeightExtra;
    CWnd* pWnd = static_cast<CWnd*>(
        impl__CreateDateTimeCtrl_CMFCToolBarDateTimeCtrl__MEAAPEAVCMFCToolBarDateTimeCtrlImpl__PEAVCWnd__AEBVCRect___Z(pThis, pWndParent, &rect));
    WndDateTime(pThis) = pWnd;
    if (pWnd == nullptr) return;

    impl__AdjustRect_CMFCToolBarDateTimeCtrl__IEAAXXZ(pThis);
    ::SendMessage(pWnd->m_hWnd, kWmSetFont, reinterpret_cast<WPARAM>(GlobalRegularFont()), TRUE);
    if (TimeStatus(pThis) == kGdtValid) {
        PushControlTime(pWnd, &Time(pThis));
    }
}

// ===========================================================================
// Painting
// ===========================================================================

// Retail (RVA 0x166690, mfc140u), fully transcribed:
//     if (m_pWndDateTime == NULL || m_pWndDateTime->m_hWnd == NULL || !(m_pWndDateTime->GetStyle() & WS_VISIBLE)) {
//         CMFCToolBarButton::OnDraw(pDC, rect, pImages, bHorz, bCustomizeMode, bHighlight, bDrawBorder, bGrayDisabledButtons);   // 0x15c910
//         return;
//     }
//     if (!m_bTextBelow || !bHorz || m_strText.IsEmpty()) return;               // +0x1c, +0x38
//     BOOL bDisabled = bCustomizeMode ? !IsEditable() : (m_nStyle & TBBS_DISABLED) != 0;   // vslot 0xd0 (26), +0x28
//     pDC->SetTextColor(bDisabled  ? afxGlobalData.clrGrayedText /*+0x44*/     // CDC vslot 0x70 (14)
//                     : bHighlight ? CMFCToolBar::GetHotTextColor()            // 0x156a70
//                                  : afxGlobalData.clrBarText /*+0x74*/);
//     CRect rectText;
//     rectText.left   = (rect.left + rect.right - m_sizeText.cx) / 2;          // +0x78
//     rectText.right  = (rect.left + rect.right + m_sizeText.cx) / 2;
//     rectText.top    = rect.top + rect.bottom;                                 // sic -- retail adds the two, no halving
//     rectText.bottom = rect.top + rect.bottom + m_sizeText.cy;                // +0x7c
//     pDC->DrawText(m_strText, rectText, DT_CENTER | DT_WORDBREAK);            // CDC vslot 0xe0 (28)
// The rectText.top expression is transcribed as retail computes it (the
// instructions at 0x1667de..0x1667ef: rect.bottom + rect.top stored straight
// into the top field); it places the label below the toolbar client area and
// looks like an upstream defect, but it is what the retail body does.
// IsEditable is the inline copy (IsEditableInline, not dispatched); the CDC
// virtuals go through their exported thunks; the base OnDraw is a null stub in
// OpenMFC (featurepack/toolbar/CMFCToolBarButton.cpp).
// Symbol: ?OnDraw@CMFCToolBarDateTimeCtrl@@UEAAXPEAVCDC@@AEBVCRect@@PEAVCMFCToolBarImages@@HHHHH@Z
extern "C" void MS_ABI impl__OnDraw_CMFCToolBarDateTimeCtrl__UEAAXPEAVCDC__AEBVCRect__PEAVCMFCToolBarImages__HHHHH_Z(
    void* pThis, CDC* pDC, const RECT* pRect, CMFCToolBarImages* pImages,
    int bHorz, int bCustomizeMode, int bHighlight, int bDrawBorder, int bGrayDisabledButtons)
{
    if (!pThis || !pRect) return;
    CMFCToolBarButton* pBase = Base(pThis);
    if (!HasVisibleWindow(pThis)) {
        impl__OnDraw_CMFCToolBarButton__UEAAXPEAVCDC__AEBVCRect__PEAVCMFCToolBarImages__HHHHH_Z(
            pBase, pDC, pRect, pImages, bHorz, bCustomizeMode, bHighlight, bDrawBorder, bGrayDisabledButtons);
        return;
    }
    if (pBase->m_bTextBelow == 0 || bHorz == 0 || pBase->m_strText.GetLength() == 0 || pDC == nullptr) return;

    const bool bDisabled = bCustomizeMode != 0 ? !IsEditableInline(pThis) : (pBase->m_nStyle & kTbbsDisabled) != 0;
    COLORREF clrText;
    if (bDisabled) {
        clrText = GlobalColor(kGlobalDataClrGrayedText, COLOR_GRAYTEXT);
    } else if (bHighlight != 0) {
        clrText = impl__GetHotTextColor_CMFCToolBar__SAKXZ();
    } else {
        clrText = GlobalColor(kGlobalDataClrBarText, COLOR_BTNTEXT);
    }
    impl__SetTextColor_CDC__UEAAKK_Z(pDC, clrText);

    RECT rectText;
    rectText.left   = (pRect->left + pRect->right - pBase->m_sizeText.cx) / 2;
    rectText.right  = (pRect->left + pRect->right + pBase->m_sizeText.cx) / 2;
    rectText.top    = pRect->top + pRect->bottom;
    rectText.bottom = pRect->top + pRect->bottom + pBase->m_sizeText.cy;
    impl__DrawTextW_CDC__QEAAHPEB_WHPEAUTAGRECT__I_Z(pDC, static_cast<const wchar_t*>(pBase->m_strText),
                                                     pBase->m_strText.GetLength(), &rectText, kDtTextLabel);
}

// Retail (RVA 0x1668a0, mfc140u), fully transcribed:
//     int iWidthUsed = CMFCToolBarButton::OnDrawOnCustomizeList(pDC, rect, bSelected);   // 0x15db10
//     int cx = rect.Width() - (iWidthUsed + 10);  if (cx < 20) cx = 20;
//     CRect rectCombo = rect;  rectCombo.left = rect.right - cx;
//     ::InflateRect(&rectCombo, -2, -3);
//     (afxGlobalData init gate before each read)
//     pDC->FillSolidRect(&rectCombo, afxGlobalData.clrWindow /*+0x78*/);                    // 0x2a5aa0
//     pDC->Draw3dRect(&rectCombo, afxGlobalData.clrBarDkShadow /*+0x6c*/, afxGlobalData.clrBarHilite /*+0x68*/);   // 0x2a5bc0
//     ::InflateRect(&rectCombo, -1, -1);
//     pDC->Draw3dRect(&rectCombo, afxGlobalData.clrBarShadow /*+0x64*/, afxGlobalData.clrBarLight /*+0x70*/);
//     CRect rectButton = rectCombo;  rectButton.left = rectCombo.right - rectCombo.Height();
//     ::InflateRect(&rectButton, -1, -1);
//     pDC->FillSolidRect(&rectButton, afxGlobalData.clrBarFace /*+0x60*/);
//     pDC->Draw3dRect(&rectButton, afxGlobalData.clrBarHilite /*+0x68*/, afxGlobalData.clrBarDkShadow /*+0x6c*/);
//     CMenuImages::Draw(pDC, CMenuImages::IdArrowDown /*0*/, rectButton, CMenuImages::ImageBlack /*0*/, CSize(0, 0));   // 0x8f790
//     return rect.Width();
// DEVIATIONS: the base OnDrawOnCustomizeList is a null stub in OpenMFC (returns
// 0, so cx = rect.Width() - 10), and ?Draw@CMenuImages@@ is an empty stub in
// featurepack/controls/CMFCReBar.cpp, so the arrow glyph is not painted until
// that is implemented (the call is made regardless).
// Symbol: ?OnDrawOnCustomizeList@CMFCToolBarDateTimeCtrl@@UEAAHPEAVCDC@@AEBVCRect@@H@Z
extern "C" int MS_ABI impl__OnDrawOnCustomizeList_CMFCToolBarDateTimeCtrl__UEAAHPEAVCDC__AEBVCRect__H_Z(
    void* pThis, CDC* pDC, const RECT* pRect, int bSelected)
{
    if (!pThis || !pDC || !pRect) return 0;
    const int iWidthUsed = impl__OnDrawOnCustomizeList_CMFCToolBarButton__UEAAHPEAVCDC__AEBVCRect__H_Z(Base(pThis), pDC, pRect, bSelected);
    int cx = (pRect->right - pRect->left) - (iWidthUsed + 10);
    if (cx < 20) cx = 20;
    RECT rectCombo = *pRect;
    rectCombo.left = pRect->right - cx;
    ::InflateRect(&rectCombo, -2, -3);
    impl__FillSolidRect_CDC__QEAAXPEBUtagRECT__K_Z(pDC, &rectCombo, GlobalColor(kGlobalDataClrWindow, COLOR_WINDOW));
    impl__Draw3dRect_CDC__QEAAXPEBUtagRECT__KK_Z(pDC, &rectCombo,
                                                 GlobalColor(kGlobalDataClrBarDkShadow, COLOR_3DDKSHADOW),
                                                 GlobalColor(kGlobalDataClrBarHilite, COLOR_BTNHIGHLIGHT));
    ::InflateRect(&rectCombo, -1, -1);
    impl__Draw3dRect_CDC__QEAAXPEBUtagRECT__KK_Z(pDC, &rectCombo,
                                                 GlobalColor(kGlobalDataClrBarShadow, COLOR_BTNSHADOW),
                                                 GlobalColor(kGlobalDataClrBarLight, COLOR_3DLIGHT));
    RECT rectButton = rectCombo;
    rectButton.left = rectCombo.right - (rectCombo.bottom - rectCombo.top);
    ::InflateRect(&rectButton, -1, -1);
    impl__FillSolidRect_CDC__QEAAXPEBUtagRECT__K_Z(pDC, &rectButton, GlobalColor(kGlobalDataClrBarFace, COLOR_BTNFACE));
    impl__Draw3dRect_CDC__QEAAXPEBUtagRECT__KK_Z(pDC, &rectButton,
                                                 GlobalColor(kGlobalDataClrBarHilite, COLOR_BTNHIGHLIGHT),
                                                 GlobalColor(kGlobalDataClrBarDkShadow, COLOR_3DDKSHADOW));
    impl__Draw_CMenuImages__SAXPEAVCDC__W4IMAGES_IDS_1_AEBVCRect__W4IMAGE_STATE_1_AEBVCSize___Z(
        pDC, 0, CRect(rectButton), 0, CSize(0, 0));
    return pRect->right - pRect->left;
}

// Retail (vftable slot 20 of 0x1803170a0 -> RVA 0x1634d0, mfc140u -- the body
// is COMDAT-folded with ?OnCtlColor@CMFCToolBarComboBoxButton@@, which is the
// name the export map carries for that RVA), fully transcribed:
//     pDC->SetTextColor(afxGlobalData.clrWindowText /*+0x7c*/);   // CDC vslot 0x70 (14)
//     pDC->SetBkColor(afxGlobalData.clrWindow /*+0x78*/);         // CDC vslot 0x68 (13)
//     return afxGlobalData.brWindow.m_hObject /*+0x110*/;
// (nCtlColor is not read; each global read is behind the init gate.)
// Symbol: ?OnCtlColor@CMFCToolBarDateTimeCtrl@@UEAAPEAUHBRUSH__@@PEAVCDC@@I@Z
extern "C" HBRUSH MS_ABI impl__OnCtlColor_CMFCToolBarDateTimeCtrl__UEAAPEAUHBRUSH____PEAVCDC__I_Z(void* pThis, CDC* pDC, unsigned int nCtlColor) {
    (void)pThis; (void)nCtlColor;
    if (pDC != nullptr) {
        impl__SetTextColor_CDC__UEAAKK_Z(pDC, GlobalColor(kGlobalDataClrWindowText, COLOR_WINDOWTEXT));
        impl__SetBkColor_CDC__UEAAKK_Z(pDC, GlobalColor(kGlobalDataClrWindow, COLOR_WINDOW));
    }
    return GlobalWindowBrush();
}

// ===========================================================================
// Input / notifications / state
// ===========================================================================

// Retail (RVA 0x166870, mfc140u), fully transcribed (pWnd and bDelay are not read):
//     return m_pWndDateTime != NULL && m_pWndDateTime->m_hWnd != NULL && (m_pWndDateTime->GetStyle() & WS_VISIBLE);
// Symbol: ?OnClick@CMFCToolBarDateTimeCtrl@@UEAAHPEAVCWnd@@H@Z
extern "C" int MS_ABI impl__OnClick_CMFCToolBarDateTimeCtrl__UEAAHPEAVCWnd__H_Z(void* pThis, CWnd* pWnd, int bDelay) {
    (void)pWnd; (void)bDelay;
    if (!pThis) return 0;
    return HasVisibleWindow(pThis) ? 1 : 0;
}

// Retail (RVA 0x166580, mfc140u), fully transcribed:
//     if (m_pWndDateTime == NULL || m_pWndDateTime->m_hWnd == NULL) return FALSE;
//     if (((iNotifyCode + 759) & ~0x10000) != 0) return TRUE;          // only DTN_DATETIMECHANGE (-759) and its
//                                                                       // 16-bit HIWORD(wParam) form (0xFD09) go on
//     m_dwTimeStatus = m_pWndDateTime->GetTime(m_time);                  // 0x29a490, CDateTimeCtrl::GetTime(CTime&)  -> +0x94
//     CObList lst;
//     if (CMFCToolBar::GetCommandButtons(m_nID, lst) > 0) {              // 0x152990
//         for (node = lst.m_pNodeHead; node != NULL; node = node->pNext) {
//             CMFCToolBarDateTimeCtrl* p = (CMFCToolBarDateTimeCtrl*)node->data;
//             if (p == NULL || !p->IsKindOf(RUNTIME_CLASS(CMFCToolBarDateTimeCtrl))) AfxThrowInvalidArgException();   // 0x234cf0 / 0x227720
//             if (p != this && m_dwTimeStatus == GDT_VALID) p->m_pWndDateTime->SetTime(&m_time);   // 0x29a390 (p->m_pWndDateTime dereferenced unconditionally)
//         }
//     }
//     return TRUE;
// DEVIATIONS: the ENSURE failure skips the node instead of throwing; a sibling
// without a window is skipped where retail would fault; the GetTime status is
// read by ReadControlTime (the DTM flag, as retail stores it) and the peers
// are recognised by IsDateTimeButton (IsKindOf plus g_pLiveObjects, file header).
// Symbol: ?NotifyCommand@CMFCToolBarDateTimeCtrl@@UEAAHH@Z
extern "C" int MS_ABI impl__NotifyCommand_CMFCToolBarDateTimeCtrl__UEAAHH_Z(void* pThis, int iNotifyCode) {
    if (!pThis) return 0;
    CWnd* pWnd = WndDateTime(pThis);
    if (!HasWindow(pWnd)) return 0;
    if (((static_cast<unsigned>(iNotifyCode) + 759u) & ~0x10000u) != 0) return 1;

    TimeStatus(pThis) = ReadControlTime(pWnd, Time(pThis));
    CommandButtonList lst(Base(pThis)->m_nID);
    if (lst.Count() > 0) {
        for (ObNode* node = lst.First(); node != nullptr; node = node->pNext) {
            CObject* pObj = node->data;
            if (!IsDateTimeButton(pObj)) continue;
            void* p = static_cast<void*>(pObj);
            if (p != pThis && TimeStatus(pThis) == kGdtValid) {
                CWnd* pSibling = WndDateTime(p);
                if (HasWindow(pSibling)) PushControlTime(pSibling, &Time(pThis));
            }
        }
    }
    return 1;
}

// Retail (vftable slot 19 of 0x1803170a0 -> RVA 0x163440, mfc140u -- COMDAT-folded
// with ?OnAddToCustomizePage@CMFCToolBarComboBoxButton@@, the name the export
// map carries for that RVA), fully transcribed:
//     CObList lst;
//     if (CMFCToolBar::GetCommandButtons(m_nID, lst) != 0)                           // 0x152990
//         CopyFrom(*(CMFCToolBarButton*)lst.m_pNodeHead->data);                      // vslot 0x38 (7)
// CopyFrom is reached through this file's thunk rather than the vftable.
// Symbol: ?OnAddToCustomizePage@CMFCToolBarDateTimeCtrl@@UEAAXXZ
extern "C" void MS_ABI impl__OnAddToCustomizePage_CMFCToolBarDateTimeCtrl__UEAAXXZ(void* pThis) {
    if (!pThis) return;
    CommandButtonList lst(Base(pThis)->m_nID);
    if (lst.Count() == 0) return;
    ObNode* pHead = lst.First();
    if (pHead == nullptr || pHead->data == nullptr) return;
    impl__CopyFrom_CMFCToolBarDateTimeCtrl__UEAAXAEBVCMFCToolBarButton___Z(pThis, static_cast<const CMFCToolBarButton*>(pHead->data));
}

// Retail (vftable slot 35 of 0x1803170a0 -> RVA 0x166be0, mfc140u), fully transcribed:
//     if (m_pWndDateTime == NULL || m_pWndDateTime->m_hWnd == NULL) return;
//     if (bShow && m_bHorz) { m_pWndDateTime->ShowWindow(SW_SHOWNOACTIVATE); OnMove(); }   // 0x2a9ad0; vslot 0x70 (14), tail call
//     else                    m_pWndDateTime->ShowWindow(SW_HIDE);
// OnMove is reached through this file's thunk rather than the vftable.
// Symbol: ?OnShow@CMFCToolBarDateTimeCtrl@@UEAAXH@Z
extern "C" void MS_ABI impl__OnShow_CMFCToolBarDateTimeCtrl__UEAAXH_Z(void* pThis, int bShow) {
    if (!pThis) return;
    CWnd* pWnd = WndDateTime(pThis);
    if (!HasWindow(pWnd)) return;
    if (bShow != 0 && Horz(pThis) != 0) {
        impl__ShowWindow_CWnd__QEAAHH_Z(pWnd, SW_SHOWNOACTIVATE);
        impl__OnMove_CMFCToolBarDateTimeCtrl__UEAAXXZ(pThis);
    } else {
        impl__ShowWindow_CWnd__QEAAHH_Z(pWnd, SW_HIDE);
    }
}

// Retail (vftable slot 37 of 0x1803170a0 -> RVA 0x166f20, mfc140u), fully transcribed:
//     m_nStyle = nStyle;                                                       // +0x28
//     if (m_pWndDateTime == NULL || m_pWndDateTime->m_hWnd == NULL) return;
//     BOOL bEnable = CMFCToolBar::m_bCustomizeMode /*0x3be35c*/ ? IsEditable()  // vslot 0xd0 (26)
//                                                                : !(m_nStyle & TBBS_DISABLED);
//     m_pWndDateTime->EnableWindow(bEnable);                                   // 0x2a9b30
// IsEditable is the inline copy (IsEditableInline, not dispatched).
// Symbol: ?SetStyle@CMFCToolBarDateTimeCtrl@@UEAAXI@Z
extern "C" void MS_ABI impl__SetStyle_CMFCToolBarDateTimeCtrl__UEAAXI_Z(void* pThis, unsigned int nStyle) {
    if (!pThis) return;
    Base(pThis)->m_nStyle = nStyle;
    CWnd* pWnd = WndDateTime(pThis);
    if (!HasWindow(pWnd)) return;
    const bool bEnable = impl__m_bCustomizeMode_CMFCToolBar__1HA != 0
                             ? IsEditableInline(pThis)
                             : (Base(pThis)->m_nStyle & kTbbsDisabled) == 0;
    impl__EnableWindow_CWnd__QEAAHH_Z(pWnd, bEnable ? 1 : 0);
}

// Retail (vftable slot 45 of 0x1803170a0 -> RVA 0x166f90, mfc140u), fully transcribed
// (pWndParent and iButtonIndex are not read):
//     if (!m_bHorz) return FALSE;                                   // +0x9c
//     CString strTipText;
//     if (OnGetCustomToolTipText(strTipText)) str = strTipText;     // vslot 0x160 (44); CString::operator= 0xde30
//     if (m_pWndDateTime == NULL) return FALSE;
//     wndToolTip.AddTool(m_pWndDateTime, str, NULL, 0);             // 0x275060
//     return TRUE;
// DEVIATION: OnGetCustomToolTipText is not dispatched (file header); the
// CMFCToolBarButton base returns FALSE, so `str` is passed through unchanged.
// Symbol: ?OnUpdateToolTip@CMFCToolBarDateTimeCtrl@@UEAAHPEAVCWnd@@HAEAVCToolTipCtrl@@AEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@@Z
extern "C" int MS_ABI impl__OnUpdateToolTip_CMFCToolBarDateTimeCtrl__UEAAHPEAVCWnd__HAEAVCToolTipCtrl__AEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
    void* pThis, CWnd* pWndParent, int iButtonIndex, CToolTipCtrl* pWndToolTip, CString* pStr)
{
    (void)pWndParent; (void)iButtonIndex;
    if (!pThis || !pWndToolTip || !pStr) return 0;
    if (Horz(pThis) == 0) return 0;
    CWnd* pWnd = WndDateTime(pThis);
    if (pWnd == nullptr) return 0;
    impl__AddTool_CToolTipCtrl__QEAAHPEAVCWnd__PEB_WPEBUtagRECT___K_Z(pWndToolTip, pWnd, static_cast<const wchar_t*>(*pStr), nullptr, 0);
    return 1;
}

// Retail (vftable slot 25 of 0x1803170a0 -> RVA 0x167070, mfc140u), fully transcribed:
//     if (m_pWndDateTime == NULL || m_pWndDateTime->m_hWnd == NULL) return;
//     (afxGlobalData init gate)
//     m_pWndDateTime->SendMessage(WM_SETFONT, afxGlobalData.fontRegular /*+0x1b0*/, TRUE);   // ::SendMessage on m_hWnd
// Symbol: ?OnGlobalFontsChanged@CMFCToolBarDateTimeCtrl@@UEAAXXZ
extern "C" void MS_ABI impl__OnGlobalFontsChanged_CMFCToolBarDateTimeCtrl__UEAAXXZ(void* pThis) {
    if (!pThis) return;
    CWnd* pWnd = WndDateTime(pThis);
    if (!HasWindow(pWnd)) return;
    ::SendMessage(pWnd->m_hWnd, kWmSetFont, reinterpret_cast<WPARAM>(GlobalRegularFont()), TRUE);
}

// Retail (vftable slot 13 of 0x1803170a0 -> RVA 0x164310, mfc140u -- COMDAT-folded
// with ?ExportToMenuButton@CMFCToolBarComboBoxButton@@, the name the export map
// carries for that RVA), fully transcribed:
//     CString strMessage;
//     HINSTANCE hInst = AfxFindStringResourceHandle(m_nID);                          // 0x2aee00
//     if (hInst != NULL && strMessage.LoadString(hInst, m_nID) && strMessage.GetLength() > 0) {   // 0xdb70
//         LPCTSTR p = wcschr(strMessage, L'\n');
//         if (p != NULL) { int i = (int)(p - strMessage); if (i != -1) menuButton.m_strText = strMessage.Mid(i + 1, len - (i + 1)); }   // +0x38
//     }
//     return TRUE;
// The CStringT::LoadString(HINSTANCE, UINT) step (0xdb70 is the exported
// ?LoadStringW@?$CStringT@...@@QEAAHPEAUHINSTANCE__@@I@Z) goes through
// OpenMFC's thunk for that export (core/collections/CStringT.cpp), and
// `Mid(i + 1, len - (i + 1))` is the tail after the '\n'.  (The `i != -1`
// test can never fail once wcschr returned non-NULL; it is dropped.)
// Symbol: ?ExportToMenuButton@CMFCToolBarDateTimeCtrl@@UEBAHAEAVCMFCToolBarMenuButton@@@Z
extern "C" int MS_ABI impl__ExportToMenuButton_CMFCToolBarDateTimeCtrl__UEBAHAEAVCMFCToolBarMenuButton___Z(
    const void* pThis, CMFCToolBarMenuButton* pMenuButton)
{
    if (!pThis || !pMenuButton) return 0;
    const unsigned nID = Base(pThis)->m_nID;
    HINSTANCE hInst = static_cast<HINSTANCE>(impl__AfxFindStringResourceHandle__YAPEAUHINSTANCE____I_Z(nID));
    if (hInst != nullptr) {
        CString strMessage;
        if (impl__LoadStringW___CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAAHPEAUHINSTANCE____I_Z(&strMessage, hInst, nID) &&
            strMessage.GetLength() > 0) {
            const wchar_t* p = wcschr(static_cast<const wchar_t*>(strMessage), L'\n');
            if (p != nullptr) {
                pMenuButton->m_strText = p + 1;
            }
        }
    }
    return 1;
}

// ===========================================================================
// Time access
// ===========================================================================

// Retail (reached from ?SetTimeAll@...@@SAHIPEAU_SYSTEMTIME@@@Z at 0x166db0 -> RVA 0x166c40, mfc140u), fully transcribed:
//     BOOL bResult = (BOOL)m_pWndDateTime->SendMessage(DTM_SETSYSTEMTIME, pTimeNew == NULL ? GDT_NONE : GDT_VALID, (LPARAM)pTimeNew);
//     NotifyCommand(DTN_DATETIMECHANGE);                     // vslot 0x90 (18), -759
//     return bResult;
// (m_pWndDateTime is dereferenced unconditionally.)  NotifyCommand is reached
// through this file's thunk rather than the vftable.
// DEVIATION: a button without a window returns FALSE where retail would fault.
// Symbol: ?SetTime@CMFCToolBarDateTimeCtrl@@QEAAHPEAU_SYSTEMTIME@@@Z
extern "C" int MS_ABI impl__SetTime_CMFCToolBarDateTimeCtrl__QEAAHPEAU_SYSTEMTIME___Z(void* pThis, SYSTEMTIME* pTimeNew) {
    if (!pThis) return 0;
    CWnd* pWnd = WndDateTime(pThis);
    if (!HasWindow(pWnd)) return 0;
    const int bResult = static_cast<int>(::SendMessage(pWnd->m_hWnd, kDtmSetSystemTime,
                                                       pTimeNew == nullptr ? kGdtNone : kGdtValid,
                                                       reinterpret_cast<LPARAM>(pTimeNew)));
    impl__NotifyCommand_CMFCToolBarDateTimeCtrl__UEAAHH_Z(pThis, kDtnDateTimeChange);
    return bResult;
}

// Retail (reached from ?SetTimeAll@...@@SAHIAEBVCOleDateTime@ATL@@@Z at 0x166df0 -> RVA 0x166ca0, mfc140u), fully transcribed:
//     BOOL bResult = m_pWndDateTime->SetTime(timeNew);        // 0x29a310, CDateTimeCtrl::SetTime(const COleDateTime&):
//                                                             //   DTM_SETSYSTEMTIME with GDT_VALID iff m_status == valid && VariantTimeToSystemTime succeeded
//     NotifyCommand(DTN_DATETIMECHANGE);                      // vslot 0x90 (18)
//     return bResult;
// DEVIATION: a button without a window returns FALSE where retail would fault.
// Symbol: ?SetTime@CMFCToolBarDateTimeCtrl@@QEAAHAEBVCOleDateTime@ATL@@@Z
extern "C" int MS_ABI impl__SetTime_CMFCToolBarDateTimeCtrl__QEAAHAEBVCOleDateTime_ATL___Z(void* pThis, const COleDateTime* pTimeNew) {
    if (!pThis) return 0;
    CWnd* pWnd = WndDateTime(pThis);
    if (!HasWindow(pWnd)) return 0;
    const int bResult = impl__SetTime_CDateTimeCtrl__QEAAHAEBVCOleDateTime_ATL___Z(pWnd, pTimeNew);
    impl__NotifyCommand_CMFCToolBarDateTimeCtrl__UEAAHH_Z(pThis, kDtnDateTimeChange);
    return bResult;
}

// Retail (reached from ?SetTimeAll@...@@SAHIPEBVCTime@ATL@@@Z at 0x166e30 -> RVA 0x166ce0, mfc140u), fully transcribed:
//     BOOL bResult = m_pWndDateTime->SetTime(pTimeNew);       // 0x29a390, CDateTimeCtrl::SetTime(const CTime*):
//                                                             //   DTM_SETSYSTEMTIME with GDT_NONE for NULL / a failed GetAsSystemTime
//     NotifyCommand(DTN_DATETIMECHANGE);                      // vslot 0x90 (18)
//     return bResult;
// The callee is PushControlTime (the NULL -> GDT_NONE case is sent here).
// DEVIATION: a button without a window returns FALSE where retail would fault.
// Symbol: ?SetTime@CMFCToolBarDateTimeCtrl@@QEAAHPEBVCTime@ATL@@@Z
extern "C" int MS_ABI impl__SetTime_CMFCToolBarDateTimeCtrl__QEAAHPEBVCTime_ATL___Z(void* pThis, const void* pTimeNew) {
    if (!pThis) return 0;
    CWnd* pWnd = WndDateTime(pThis);
    if (!HasWindow(pWnd)) return 0;
    const int bResult = PushControlTime(pWnd, pTimeNew);
    impl__NotifyCommand_CMFCToolBarDateTimeCtrl__UEAAHH_Z(pThis, kDtnDateTimeChange);
    return bResult;
}

// ===========================================================================
// Statics
// ===========================================================================

// Retail (RVA 0x166d20, mfc140u), fully transcribed:
//     CObList lst;                                                    (CObList vftable 0x18032a6f8, block size 10)
//     CMFCToolBarDateTimeCtrl* p = NULL;
//     if (CMFCToolBar::GetCommandButtons(uiCmd, lst) > 0 && lst.m_pNodeHead != NULL) {   // 0x152990
//         p = (CMFCToolBarDateTimeCtrl*)lst.m_pNodeHead->data;
//         if (p == NULL || !p->IsKindOf(RUNTIME_CLASS(CMFCToolBarDateTimeCtrl))) AfxThrowInvalidArgException();   // 0x234cf0 / 0x227720
//     }
//     return p;                                                       (lst destroyed, 0x8350)
// DEVIATION: the ENSURE failure returns NULL instead of throwing; the IsKindOf
// is IsDateTimeButton (IsKindOf plus g_pLiveObjects, file header).
// Symbol: ?GetByCmd@CMFCToolBarDateTimeCtrl@@SAPEAV1@I@Z
extern "C" void* MS_ABI impl__GetByCmd_CMFCToolBarDateTimeCtrl__SAPEAV1_I_Z(unsigned int uiCmd) {
    CommandButtonList lst(uiCmd);
    if (lst.Count() <= 0) return nullptr;
    ObNode* pHead = lst.First();
    if (pHead == nullptr) return nullptr;
    CObject* p = pHead->data;
    if (!IsDateTimeButton(p)) return nullptr;
    return static_cast<void*>(p);
}

// Retail (RVA 0x166e70, mfc140u), fully transcribed:
//     CMFCToolBarDateTimeCtrl* p = GetByCmd(uiCmd);                  // 0x166d20
//     if (p == NULL) return GDT_NONE;
//     return (DWORD)::SendMessage(p->m_pWndDateTime->m_hWnd, DTM_GETSYSTEMTIME, 0, (LPARAM)pTimeDest);   (m_pWndDateTime dereferenced unconditionally)
// DEVIATION: a button without a window yields GDT_NONE where retail would fault.
// Symbol: ?GetTimeAll@CMFCToolBarDateTimeCtrl@@SAKIPEAU_SYSTEMTIME@@@Z
extern "C" unsigned long MS_ABI impl__GetTimeAll_CMFCToolBarDateTimeCtrl__SAKIPEAU_SYSTEMTIME___Z(unsigned int uiCmd, SYSTEMTIME* pTimeDest) {
    void* p = impl__GetByCmd_CMFCToolBarDateTimeCtrl__SAPEAV1_I_Z(uiCmd);
    if (p == nullptr) return kGdtNone;
    CWnd* pWnd = WndDateTime(p);
    if (!HasWindow(pWnd)) return kGdtNone;
    return static_cast<unsigned long>(::SendMessage(pWnd->m_hWnd, kDtmGetSystemTime, 0, reinterpret_cast<LPARAM>(pTimeDest)));
}

// Retail (RVA 0x166eb0, mfc140u), fully transcribed:
//     CMFCToolBarDateTimeCtrl* p = GetByCmd(uiCmd);                  // 0x166d20
//     if (p == NULL) return FALSE;                                   (the NULL from GetByCmd is what comes back in eax)
//     return p->m_pWndDateTime->GetTime(timeDest);                   // 0x29a400, CDateTimeCtrl::GetTime(COleDateTime&) (tail call)
// DEVIATION: a button without a window yields FALSE where retail would fault.
// Symbol: ?GetTimeAll@CMFCToolBarDateTimeCtrl@@SAHIAEAVCOleDateTime@ATL@@@Z
extern "C" int MS_ABI impl__GetTimeAll_CMFCToolBarDateTimeCtrl__SAHIAEAVCOleDateTime_ATL___Z(unsigned int uiCmd, COleDateTime* pTimeDest) {
    void* p = impl__GetByCmd_CMFCToolBarDateTimeCtrl__SAPEAV1_I_Z(uiCmd);
    if (p == nullptr) return 0;
    CWnd* pWnd = WndDateTime(p);
    if (!HasWindow(pWnd)) return 0;
    return impl__GetTime_CDateTimeCtrl__QEBAHAEAVCOleDateTime_ATL___Z(pWnd, pTimeDest);
}

// Retail (RVA 0x166ee0, mfc140u), fully transcribed:
//     CMFCToolBarDateTimeCtrl* p = GetByCmd(uiCmd);                  // 0x166d20
//     if (p == NULL) return GDT_NONE;
//     return p->m_pWndDateTime->GetTime(timeDest);                   // 0x29a490, CDateTimeCtrl::GetTime(CTime&) (tail call)
// DEVIATION: a button without a window (or a NULL destination) yields GDT_NONE
// where retail would fault.  The callee is ReadControlTime, so the DTM flag
// comes back as retail returns it.
// Symbol: ?GetTimeAll@CMFCToolBarDateTimeCtrl@@SAKIAEAVCTime@ATL@@@Z
extern "C" unsigned long MS_ABI impl__GetTimeAll_CMFCToolBarDateTimeCtrl__SAKIAEAVCTime_ATL___Z(unsigned int uiCmd, void* pTimeDest) {
    void* p = impl__GetByCmd_CMFCToolBarDateTimeCtrl__SAPEAV1_I_Z(uiCmd);
    if (p == nullptr || pTimeDest == nullptr) return kGdtNone;
    CWnd* pWnd = WndDateTime(p);
    if (!HasWindow(pWnd)) return kGdtNone;
    return ReadControlTime(pWnd, *static_cast<CTime_atl*>(pTimeDest));
}

// Retail (RVA 0x166db0, mfc140u -- the function right after GetByCmd; not in
// the export map), fully transcribed:
//     CMFCToolBarDateTimeCtrl* p = GetByCmd(uiCmd);                  // 0x166d20
//     if (p != NULL) p->SetTime(pTimeNew);                           // 0x166c40 (the SYSTEMTIME overload)
//     return p != NULL;
// Symbol: ?SetTimeAll@CMFCToolBarDateTimeCtrl@@SAHIPEAU_SYSTEMTIME@@@Z
extern "C" int MS_ABI impl__SetTimeAll_CMFCToolBarDateTimeCtrl__SAHIPEAU_SYSTEMTIME___Z(unsigned int uiCmd, SYSTEMTIME* pTimeNew) {
    void* p = impl__GetByCmd_CMFCToolBarDateTimeCtrl__SAPEAV1_I_Z(uiCmd);
    if (p != nullptr) impl__SetTime_CMFCToolBarDateTimeCtrl__QEAAHPEAU_SYSTEMTIME___Z(p, pTimeNew);
    return p != nullptr ? 1 : 0;
}

// Retail (RVA 0x166df0, mfc140u; not in the export map), fully transcribed:
//     CMFCToolBarDateTimeCtrl* p = GetByCmd(uiCmd);                  // 0x166d20
//     if (p != NULL) p->SetTime(timeNew);                            // 0x166ca0 (the COleDateTime overload)
//     return p != NULL;
// Symbol: ?SetTimeAll@CMFCToolBarDateTimeCtrl@@SAHIAEBVCOleDateTime@ATL@@@Z
extern "C" int MS_ABI impl__SetTimeAll_CMFCToolBarDateTimeCtrl__SAHIAEBVCOleDateTime_ATL___Z(unsigned int uiCmd, const COleDateTime* pTimeNew) {
    void* p = impl__GetByCmd_CMFCToolBarDateTimeCtrl__SAPEAV1_I_Z(uiCmd);
    if (p != nullptr) impl__SetTime_CMFCToolBarDateTimeCtrl__QEAAHAEBVCOleDateTime_ATL___Z(p, pTimeNew);
    return p != nullptr ? 1 : 0;
}

// Retail (RVA 0x166e30, mfc140u; not in the export map), fully transcribed:
//     CMFCToolBarDateTimeCtrl* p = GetByCmd(uiCmd);                  // 0x166d20
//     if (p != NULL) p->SetTime(pTimeNew);                           // 0x166ce0 (the CTime* overload)
//     return p != NULL;
// Symbol: ?SetTimeAll@CMFCToolBarDateTimeCtrl@@SAHIPEBVCTime@ATL@@@Z
extern "C" int MS_ABI impl__SetTimeAll_CMFCToolBarDateTimeCtrl__SAHIPEBVCTime_ATL___Z(unsigned int uiCmd, const void* pTimeNew) {
    void* p = impl__GetByCmd_CMFCToolBarDateTimeCtrl__SAPEAV1_I_Z(uiCmd);
    if (p != nullptr) impl__SetTime_CMFCToolBarDateTimeCtrl__QEAAHPEBVCTime_ATL___Z(p, pTimeNew);
    return p != nullptr ? 1 : 0;
}
