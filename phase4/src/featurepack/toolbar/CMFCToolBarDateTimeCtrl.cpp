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
// The instance layout in afxmfc.h follows the retail class
// (afxtoolbardatetimectrl.h and the mfc140u placement constructors).
//
//   CMFCToolBarButton base ............ +0x00 .. +0x88   (afxmfc.h, sizeof 136)
//   +0x88  CMFCToolBarDateTimeCtrlImpl* m_pWndDateTime
//   +0x90  DWORD m_dwStyle              +0x94  DWORD m_dwTimeStatus
//   +0x98  int   m_iWidth               +0x9c  BOOL  m_bHorz
//   +0xa0  CTime m_time  (one __time64_t)
//   sizeof == 0xa8 (168, also recorded in the runtime class descriptor)
//
// The names are the SDK's (afxtoolbardatetimectrl.h declares the six members
// in exactly this order); which offset is which was fixed from the bodies that
// use them: OnSize (0x1661c0) stores its iSize at +0x98 and OnCalculateSize
// (0x165f70) its bHorz at +0x9c, NotifyCommand (0x166580) stores the
// CDateTimeCtrl::GetTime status at +0x94 and passes +0xa0 as the CTime, the
// constructors OR 0x50000000 (WS_CHILD|WS_VISIBLE) into +0x90 and
// Initialize stores _time64(NULL) at +0xa0.
//
// The date-time toolbar button uses its own runtime class and the base button
// layout. The layout thunks call the implementations in this translation unit.
// THE WINDOW OBJECT.  m_pWndDateTime is a CMFCToolBarDateTimeCtrlImpl
// (afxtoolbardatetimectrl.h: CDateTimeCtrl plus `bool m_bMonthCtrlDisplayed`;
// CreateDateTimeCtrl, 0x166ae0, allocates 0xf0 bytes, runs ??0CWnd@@QEAA@XZ,
// stores the Impl vftable 0x1803172f8 and clears the byte at +0xe8).  Its
// constructor is inline in MFC and not exported, so CreateDateTimeCtrl below
// builds it the way CMFCToolBarComboBoxButton.cpp builds its combo:
// ??2@YAPEAX_K@Z + the exported ??0CWnd@@QEAA@XZ + the exported
// ?Create@CDateTimeCtrl@@.  Such an object carries OpenMFC's CWnd vftable and
// its runtime class reads CWnd; its message map (detail/Toolbar21MsgmapSupport.cpp)
// is empty. CMFCToolBar handles DTN_DATETIMECHANGE from the picker HWND.  A window some other code planted in
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
#include <utility>

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
//   core/collections/CObList.cpp : ctor / dtor / FindIndex
extern "C" void* MS_ABI impl___0CObList__QEAA__J_Z(void* pThis, long long nBlockSize);
extern "C" void MS_ABI impl___1CObList__UEAA_XZ(void* pThis);
extern "C" CObList::POSITION MS_ABI impl__FindIndex_CObList__QEBAPEAU__POSITION____J_Z(const CObList* pThis, long long nIndex);
//   featurepack/toolbar/CMFCToolBarButton.cpp : the base-class overrides.
//   The SIZE result uses a hidden pointer in RDX under the MSVC ABI.
extern "C" void MS_ABI impl__CopyFrom_CMFCToolBarButton__UEAAXAEBV1__Z(void* pThis, const CMFCToolBarButton* pSrc);
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

// Check the ABI used by the MSVC placement constructors and archive factory.
static_assert(sizeof(CMFCToolBarDateTimeCtrl) == 0xa8);
static_assert(offsetof(CMFCToolBarDateTimeCtrl, m_pWndDateTime) == 0x88);
static_assert(offsetof(CMFCToolBarDateTimeCtrl, m_dwStyle) == 0x90);
static_assert(offsetof(CMFCToolBarDateTimeCtrl, m_dwTimeStatus) == 0x94);
static_assert(offsetof(CMFCToolBarDateTimeCtrl, m_iWidth) == 0x98);
static_assert(offsetof(CMFCToolBarDateTimeCtrl, m_bHorz) == 0x9c);
static_assert(offsetof(CMFCToolBarDateTimeCtrl, m_time) == 0xa0);

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

inline auto* DateTimeButton(void* p) { return static_cast<CMFCToolBarDateTimeCtrl*>(p); }
inline const auto* DateTimeButton(const void* p) { return static_cast<const CMFCToolBarDateTimeCtrl*>(p); }
inline CMFCToolBarButton* Base(void* p) { return DateTimeButton(p); }
inline const CMFCToolBarButton* Base(const void* p) { return DateTimeButton(p); }
inline CWnd*& WndDateTime(void* p) { return DateTimeButton(p)->m_pWndDateTime; }
inline CWnd* WndDateTime(const void* p) { return DateTimeButton(p)->m_pWndDateTime; }
inline DWORD& Style(void* p) { return DateTimeButton(p)->m_dwStyle; }
inline DWORD Style(const void* p) { return DateTimeButton(p)->m_dwStyle; }
inline DWORD& TimeStatus(void* p) { return DateTimeButton(p)->m_dwTimeStatus; }
inline int& Width(void* p) { return DateTimeButton(p)->m_iWidth; }
inline int Width(const void* p) { return DateTimeButton(p)->m_iWidth; }
inline BOOL& Horz(void* p) { return DateTimeButton(p)->m_bHorz; }
inline BOOL Horz(const void* p) { return DateTimeButton(p)->m_bHorz; }
inline LONGLONG& Time(void* p) { return DateTimeButton(p)->m_time; }

inline HWND HWndOf(const CWnd* pWnd) { return pWnd ? pWnd->m_hWnd : nullptr; }
// `pWnd->GetSafeHwnd() != NULL` as retail spells it: the pointer, then +0x40.
inline bool HasWindow(const CWnd* pWnd) { return pWnd != nullptr && ::IsWindow(pWnd->m_hWnd); }
// `(pWnd->GetStyle() & WS_VISIBLE) != 0` -- retail: call ?GetStyle@CWnd@@ (0x2a9690), `bt $0x1c,%eax`.
inline bool StyleVisible(const CWnd* pWnd) { return (impl__GetStyle_CWnd__QEBAKXZ(pWnd) & WS_VISIBLE) != 0; }
// The three-part gate almost every body here opens with:
//     m_pWndDateTime != NULL && m_pWndDateTime->m_hWnd != NULL && (m_pWndDateTime->GetStyle() & WS_VISIBLE)
inline bool HasVisibleWindow(const void* p) {
    const CWnd* pWnd = WndDateTime(p);
    return HasWindow(pWnd) && StyleVisible(pWnd);
}

// Message / style numbers the retail bodies use as immediates.
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
bool IsDateTimeButton(const CObject* button) {
    return button && button->IsKindOf(RUNTIME_CLASS(CMFCToolBarDateTimeCtrl));
}

bool MakeLocalTime(const SYSTEMTIME& system, LONGLONG& time) {
    FILETIME valid{};
    if (!::SystemTimeToFileTime(&system, &valid)) return false;
    tm local{};
    local.tm_year = system.wYear - 1900;
    local.tm_mon = system.wMonth - 1;
    local.tm_mday = system.wDay;
    local.tm_hour = system.wHour;
    local.tm_min = system.wMinute;
    local.tm_sec = system.wSecond;
    local.tm_isdst = -1;
    const auto result = _mktime64(&local);
    if (result == -1) return false;
    time = result;
    return true;
}

SYSTEMTIME TimeFields(const tm& local) {
    return SYSTEMTIME{static_cast<WORD>(local.tm_year + 1900), static_cast<WORD>(local.tm_mon + 1),
        static_cast<WORD>(local.tm_wday), static_cast<WORD>(local.tm_mday), static_cast<WORD>(local.tm_hour),
        static_cast<WORD>(local.tm_min), static_cast<WORD>(local.tm_sec), 0};
}

DWORD ReadControlTime(const CWnd* window, LONGLONG& time) {
    SYSTEMTIME system{};
    const auto status = static_cast<DWORD>(::SendMessageW(window->m_hWnd, DTM_GETSYSTEMTIME, 0,
        reinterpret_cast<LPARAM>(&system)));
    if (status == GDT_VALID && !MakeLocalTime(system, time)) return GDT_ERROR;
    return status;
}

BOOL PushControlTime(CWnd* window, const LONGLONG* time) {
    SYSTEMTIME system{};
    if (time) {
        tm local{};
        if (_localtime64_s(&local, time) != 0) return FALSE;
        system = TimeFields(local);
    }
    return static_cast<BOOL>(::SendMessageW(window->m_hWnd, DTM_SETSYSTEMTIME,
        time ? GDT_VALID : GDT_NONE, reinterpret_cast<LPARAM>(&system)));
}

constexpr DWORD time64Marker = 0x8000000a;
LONGLONG ReadArchivedTime(CArchive& archive, bool objectRecord) {
    DWORD prefix = 0;
    archive >> prefix;
    LONGLONG time = 0;
    if (prefix == time64Marker) {
        archive >> time;
    } else if (objectRecord) {
        time = static_cast<LONG>(prefix);
    } else {
        // The old OpenMFC direct serializer wrote an unmarked UTC-based value.
        DWORD high = 0;
        archive >> high;
        time = static_cast<LONGLONG>((ULONGLONG(high) << 32) | prefix);
        tm utc{};
        if (_gmtime64_s(&utc, &time) == 0) MakeLocalTime(TimeFields(utc), time);
    }
    return time;
}

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

IMPLEMENT_SERIAL(CMFCToolBarDateTimeCtrl, CMFCToolBarButton, 1)

CMFCToolBarDateTimeCtrl::CMFCToolBarDateTimeCtrl()
    : m_dwStyle(kDefaultStyle), m_iWidth(kDefaultWidth)
{
    impl__Initialize_CMFCToolBarDateTimeCtrl__IEAAXXZ(this);
}

CMFCToolBarDateTimeCtrl::CMFCToolBarDateTimeCtrl(UINT id, int image, DWORD style, int width)
    : CMFCToolBarButton(id, image, nullptr), m_dwStyle(style | kDefaultStyle),
      m_iWidth(width ? width : kDefaultWidth)
{
    impl__Initialize_CMFCToolBarDateTimeCtrl__IEAAXXZ(this);
}

// Symbol: ??0CMFCToolBarDateTimeCtrl@@QEAA@IHKH@Z
extern "C" void* MS_ABI impl___0CMFCToolBarDateTimeCtrl__QEAA_IHKH_Z(
    void* storage, unsigned int id, int image, unsigned long style, int width) {
    return storage ? new (storage) CMFCToolBarDateTimeCtrl(id, image, style, width) : nullptr;
}

// Symbol: ??0CMFCToolBarDateTimeCtrl@@QEAA@XZ
extern "C" void* MS_ABI impl___0CMFCToolBarDateTimeCtrl__QEAA_XZ(void* storage) {
    return storage ? new (storage) CMFCToolBarDateTimeCtrl() : nullptr;
}

CMFCToolBarDateTimeCtrl::~CMFCToolBarDateTimeCtrl() {
    if (CWnd* window = std::exchange(m_pWndDateTime, nullptr)) {
        DestroyOwnedWindow(window);
        DeleteOwnedWindow(window);
    }
}

// Symbol: ??1CMFCToolBarDateTimeCtrl@@UEAA@XZ
extern "C" void MS_ABI impl___1CMFCToolBarDateTimeCtrl__UEAA_XZ(void* button) {
    if (button) DateTimeButton(button)->CMFCToolBarDateTimeCtrl::~CMFCToolBarDateTimeCtrl();
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
    Time(pThis) = _time64(nullptr);
}

// Retail (RVA 0x165980, mfc140u), fully transcribed:
//     void* p = operator new(0xa8);                              // 0x27f0
//     return p != NULL ? new (p) CMFCToolBarDateTimeCtrl() : NULL;   // 0x165af0
// Symbol: ?CreateObject@CMFCToolBarDateTimeCtrl@@SAPEAVCObject@@XZ
extern "C" CObject* MS_ABI impl__CreateObject_CMFCToolBarDateTimeCtrl__SAPEAVCObject__XZ() {
    return CMFCToolBarDateTimeCtrl::CreateObject();
}

// Symbol: ?CopyFrom@CMFCToolBarDateTimeCtrl@@UEAAXAEBVCMFCToolBarButton@@@Z
extern "C" void MS_ABI impl__CopyFrom_CMFCToolBarDateTimeCtrl__UEAAXAEBVCMFCToolBarButton___Z(void* target, const CMFCToolBarButton* source) {
    if (!target || !IsDateTimeButton(source) || target == source) return;
    impl__CopyFrom_CMFCToolBarButton__UEAAXAEBV1__Z(target, source);
    auto* button = DateTimeButton(target);
    const auto* other = static_cast<const CMFCToolBarDateTimeCtrl*>(source);
    button->m_dwStyle = other->m_dwStyle;
    button->m_iWidth = other->m_iWidth;
    button->m_rect.right = button->m_rect.left + button->m_iWidth;
    button->m_dwTimeStatus = other->m_dwTimeStatus;
    button->m_time = other->m_time;
    if (auto* window = other->GetDateTimeCtrl(); HasWindow(window))
        button->m_dwTimeStatus = ReadControlTime(window, button->m_time);
}

void CMFCToolBarDateTimeCtrl::Serialize(CArchive& archive) {
    if (archive.IsStoring()) {
        CMFCToolBarButton::Serialize(archive);
        auto status = m_dwTimeStatus;
        auto time = m_time;
        if (HasWindow(m_pWndDateTime)) status = ReadControlTime(m_pWndDateTime, time);
        archive << m_iWidth << m_dwStyle << status << time64Marker << time;
        return;
    }

    const bool objectRecord = archive.GetObjectSchema() != UINT(-1);
    CMFCToolBarButton base;
    impl__CopyFrom_CMFCToolBarButton__UEAAXAEBV1__Z(&base, this);
    base.Serialize(archive);
    int width = 0;
    DWORD style = 0, status = 0;
    archive >> width >> style >> status;
    const auto time = ReadArchivedTime(archive, objectRecord);

    impl__CopyFrom_CMFCToolBarButton__UEAAXAEBV1__Z(this, &base);
    m_iWidth = width;
    m_rect.right = m_rect.left + width;
    m_dwStyle = style;
    m_dwTimeStatus = status;
    m_time = time;
    if (HasWindow(m_pWndDateTime) && (status == GDT_VALID || status == GDT_NONE))
        PushControlTime(m_pWndDateTime, status == GDT_VALID ? &m_time : nullptr);
}

// Symbol: ?Serialize@CMFCToolBarDateTimeCtrl@@UEAAXAEAVCArchive@@@Z
extern "C" void MS_ABI impl__Serialize_CMFCToolBarDateTimeCtrl__UEAAXAEAVCArchive___Z(void* button, CArchive* archive) {
    if (button && archive) DateTimeButton(button)->CMFCToolBarDateTimeCtrl::Serialize(*archive);
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

void CMFCToolBarDateTimeCtrl::OnMove() {
    impl__OnMove_CMFCToolBarDateTimeCtrl__UEAAXXZ(this);
}

void CMFCToolBarDateTimeCtrl::OnSize(int width) {
    impl__OnSize_CMFCToolBarDateTimeCtrl__UEAAXH_Z(this, width);
}

HWND CMFCToolBarDateTimeCtrl::GetHwnd() {
    const auto* control = m_pWndDateTime;
    return control ? control->GetSafeHwnd() : nullptr;
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
    if (!pThis || !HasWindow(pWndParent) || !pRect) return nullptr;
    INITCOMMONCONTROLSEX controls{sizeof(controls), ICC_DATE_CLASSES};
    if (!::InitCommonControlsEx(&controls)) return nullptr;
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
// Retain the old picker value when moving between toolbars, then apply the
// saved value or empty status to the newly created control.
// Symbol: ?OnChangeParentWnd@CMFCToolBarDateTimeCtrl@@UEAAXPEAVCWnd@@@Z
extern "C" void MS_ABI impl__OnChangeParentWnd_CMFCToolBarDateTimeCtrl__UEAAXPEAVCWnd___Z(void* pThis, CWnd* pWndParent) {
    if (!pThis) return;
    impl__OnChangeParentWnd_CMFCToolBarButton__UEAAXPEAVCWnd___Z(Base(pThis), pWndParent);

    if (auto* old = WndDateTime(pThis)) {
        if (HasWindow(old)) {
            if (pWndParent && ::GetParent(old->m_hWnd) == pWndParent->m_hWnd) return;
            TimeStatus(pThis) = ReadControlTime(old, Time(pThis));
        }
        WndDateTime(pThis) = nullptr;
        DestroyOwnedWindow(old);
        DeleteOwnedWindow(old);
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
    if (TimeStatus(pThis) == kGdtValid || TimeStatus(pThis) == kGdtNone)
        PushControlTime(pWnd, TimeStatus(pThis) == kGdtValid ? &Time(pThis) : nullptr);
    ::EnableWindow(pWnd->m_hWnd, !(Base(pThis)->m_nStyle & kTbbsDisabled));
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
// Update peers only when the value changes. Store their state before updating
// their windows so synchronous DTN_DATETIMECHANGE notifications stop there.
// Symbol: ?NotifyCommand@CMFCToolBarDateTimeCtrl@@UEAAHH@Z
extern "C" int MS_ABI impl__NotifyCommand_CMFCToolBarDateTimeCtrl__UEAAHH_Z(void* pThis, int code) {
    if (!pThis || !HasWindow(WndDateTime(pThis))) return FALSE;
    if (static_cast<WORD>(code) != static_cast<WORD>(DTN_DATETIMECHANGE)) return TRUE;

    auto time = Time(pThis);
    const DWORD status = ReadControlTime(WndDateTime(pThis), time);
    if (status == GDT_ERROR || (TimeStatus(pThis) == status && Time(pThis) == time)) return TRUE;
    TimeStatus(pThis) = status;
    Time(pThis) = time;
    CommandButtonList peers(Base(pThis)->m_nID);
    for (ObNode* node = peers.First(); node; node = node->pNext) {
        if (!IsDateTimeButton(node->data) || node->data == Base(pThis)) continue;
        auto* peer = static_cast<CMFCToolBarDateTimeCtrl*>(node->data);
        if (status == GDT_NONE && !(peer->m_dwStyle & DTS_SHOWNONE)) continue;
        peer->m_dwTimeStatus = status;
        peer->m_time = time;
        if (auto* window = peer->GetDateTimeCtrl(); HasWindow(window))
            PushControlTime(window, status == GDT_VALID ? &time : nullptr);
    }
    return TRUE;
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
    if (pHead == nullptr || !IsDateTimeButton(pHead->data)) return;
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

BOOL CMFCToolBarDateTimeCtrl::SetTime(SYSTEMTIME* system) {
    if (HasWindow(m_pWndDateTime)) {
        const auto result = system
            ? static_cast<BOOL>(::SendMessageW(m_pWndDateTime->m_hWnd, DTM_SETSYSTEMTIME, GDT_VALID,
                reinterpret_cast<LPARAM>(system)))
            : PushControlTime(m_pWndDateTime, nullptr);
        if (result) impl__NotifyCommand_CMFCToolBarDateTimeCtrl__UEAAHH_Z(this, DTN_DATETIMECHANGE);
        return result;
    }
    if (system) {
        if (!MakeLocalTime(*system, m_time)) return FALSE;
        m_dwTimeStatus = GDT_VALID;
    } else {
        if (!(m_dwStyle & DTS_SHOWNONE)) return FALSE;
        m_dwTimeStatus = GDT_NONE;
    }
    return TRUE;
}

DWORD CMFCToolBarDateTimeCtrl::GetTime(SYSTEMTIME* system) const {
    if (HasWindow(m_pWndDateTime)) {
        SYSTEMTIME local{};
        return static_cast<DWORD>(::SendMessageW(m_pWndDateTime->m_hWnd, DTM_GETSYSTEMTIME,
            0, reinterpret_cast<LPARAM>(system ? system : &local)));
    }
    if (system && m_dwTimeStatus == GDT_VALID) {
        tm local{};
        if (_localtime64_s(&local, &m_time) != 0) return GDT_ERROR;
        *system = TimeFields(local);
    }
    return m_dwTimeStatus;
}

// Symbol: ?SetTime@CMFCToolBarDateTimeCtrl@@QEAAHPEAU_SYSTEMTIME@@@Z
extern "C" int MS_ABI impl__SetTime_CMFCToolBarDateTimeCtrl__QEAAHPEAU_SYSTEMTIME___Z(void* button, SYSTEMTIME* system) {
    return button && DateTimeButton(button)->SetTime(system);
}

// Symbol: ?SetTime@CMFCToolBarDateTimeCtrl@@QEAAHAEBVCOleDateTime@ATL@@@Z
extern "C" int MS_ABI impl__SetTime_CMFCToolBarDateTimeCtrl__QEAAHAEBVCOleDateTime_ATL___Z(void* button, const COleDateTime* time) {
    SYSTEMTIME system{};
    if (!button || !time) return FALSE;
    const bool valid = time->m_status == COleDateTime::DT_VALID && ::VariantTimeToSystemTime(time->m_dt, &system);
    return DateTimeButton(button)->SetTime(valid ? &system : nullptr);
}

// Symbol: ?SetTime@CMFCToolBarDateTimeCtrl@@QEAAHPEBVCTime@ATL@@@Z
extern "C" int MS_ABI impl__SetTime_CMFCToolBarDateTimeCtrl__QEAAHPEBVCTime_ATL___Z(void* button, const void* time) {
    if (!button) return FALSE;
    if (!time) return DateTimeButton(button)->SetTime(nullptr);
    tm local{};
    if (_localtime64_s(&local, static_cast<const LONGLONG*>(time)) != 0) return FALSE;
    SYSTEMTIME system = TimeFields(local);
    return DateTimeButton(button)->SetTime(&system);
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
// Return the first date-time control for the command.
// Symbol: ?GetByCmd@CMFCToolBarDateTimeCtrl@@SAPEAV1@I@Z
extern "C" void* MS_ABI impl__GetByCmd_CMFCToolBarDateTimeCtrl__SAPEAV1_I_Z(unsigned int uiCmd) {
    CommandButtonList lst(uiCmd);
    for (ObNode* node = lst.First(); node; node = node->pNext)
        if (IsDateTimeButton(node->data)) return node->data;
    return nullptr;
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
    return DateTimeButton(p)->GetTime(pTimeDest);
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
    if (HasWindow(pWnd)) return ReadControlTime(pWnd, *static_cast<LONGLONG*>(pTimeDest));
    if (TimeStatus(p) == kGdtValid) *static_cast<LONGLONG*>(pTimeDest) = Time(p);
    return TimeStatus(p);
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
