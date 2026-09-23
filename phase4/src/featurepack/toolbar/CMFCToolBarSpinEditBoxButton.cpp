// CMFCToolBarSpinEditBoxButton — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp
//
// Every body marked "Retail (RVA 0x..., mfc140u)" below was transcribed from
// the disassembly of that entry point in mfc140u.dll (14.51.36231), the method
// described in the header of core/ole/COleControl.cpp.  Six exports have no
// entry in the rva-symbols JSON (SetRange, OnMove, OnShowEditbox, Serialize,
// OnUpdateToolTip, SetACCData); their RVAs were read from mfc140u.dll's
// export address table by ordinal (SetRange 13531 -> 0x17f490, OnMove 10522
// -> 0x17f210, OnShowEditbox 11179 -> 0x17f600, Serialize 12821 -> 0x17f370,
// OnUpdateToolTip 11561 -> 0x17f500, SetACCData 12884 -> 0x17f130), and every
// virtual one agrees with its slot in the class vftable at 0x18031af98
// (mfc140u; the address both retail constructors install):
//   slot 2  Serialize        0x17f370     slot 7  CopyFrom         0x17f330
//   slot 14 OnMove           0x17f210     slot 44 OnGetCustomToolTipText 0x71e0 (`xor eax,eax; ret`)
//   slot 45 OnUpdateToolTip  0x17f500     slot 48 SetACCData       0x17f130
//   slot 53 CreateEdit       0x17f170     slot 55 GetEditBorder    0x17f260
//   slot 56 OnShowEditbox    0x17f600
//
// INSTANCE LAYOUT.  This class is NOT declared in include/openmfc, so this file
// is the only place its layout lives: every export takes a `void*` this, and a
// real MSVC client (compiled against afxtoolbarspineditboxbutton.h) allocates
// the object with the retail layout.  The constructors (0x17efb0 / 0x17f050)
// and Init (0x17f0a0) write it as:
//
//   CMFCToolBarEditBoxButton base ..... +0x000 .. +0x0b0  (sizeof 0xb0, see
//                                         featurepack/toolbar/CMFCToolBarEditBoxButton.cpp)
//   +0x0b0  CMFCSpinButtonCtrl m_wndSpin (0x100 bytes; its CWnd::m_hWnd is at +0x0f0)
//   +0x1b0  int m_nMin                    +0x1b4  int m_nMax
//   sizeof == 0x1b8 (440: the size ?CreateObject@ allocates, and the value the
//   RTTI descriptor in featurepack/toolbar/RuntimeClasses.cpp carries)
//
// Retail's ??0CMFCSpinButtonCtrl@@QEAA@XZ (RVA 0x132520, mfc140u) is
//     CWnd::CWnd();                                  // 0x28a700
//     vfptr = &CMFCSpinButtonCtrl::`vftable';        // 0x18030ffe8 (mfc140u)
//     8 bytes at +0xe8 = 0; 4 bytes at +0xf0 = 0; 8 bytes at +0xf4 = 0;
// i.e. a 0xe8-byte CWnd followed by 0x14 bytes of CMFCSpinButtonCtrl state.
// OpenMFC's own export of that constructor (featurepack/controls/
// CMFCSpinButtonCtrl.cpp) is a no-op stub that constructs nothing, so the
// constructors below build m_wndSpin through the exported ??0CWnd@@QEAA@XZ
// (a placement-new of OpenMFC's 232-byte CWnd, the same size as retail's) and
// clear the 0x14 CMFCSpinButtonCtrl bytes themselves.  m_wndSpin therefore
// carries OpenMFC's CWnd vftable, not CMFCSpinButtonCtrl's, and has none of
// the owner-draw behaviour of CMFCSpinButtonCtrl's message map; it is a
// plain msctls_updown32 window once CreateEdit has created it.
//
// VFTABLE.  Retail installs &CMFCToolBarSpinEditBoxButton::`vftable'
// (0x18031af98, mfc140u).  OpenMFC has no C++ class for this type, so the
// constructors leave the vptr the base-constructor thunk installed (the
// mingw-built CMFCToolBarEditBoxButton one), as
// featurepack/toolbar/CMFCToolBarDateTimeCtrl.cpp does for its class.
// CONSEQUENCES: (1) OpenMFC's own toolbar code dispatches on that vptr, so it
// reaches the CMFCToolBarEditBoxButton bodies and never the overrides in this
// file -- the spin control is created only when a caller reaches
// ?CreateEdit@CMFCToolBarSpinEditBoxButton@@ directly (a client-derived class
// whose MSVC vftable imports it, or an explicit qualified call).  (2) An
// object deleted through that vptr (OpenMFC-internal `delete pButton`) runs
// only ~CMFCToolBarEditBoxButton and skips the destructor here, so m_wndSpin
// is never torn down.  That is harmless while m_wndSpin has no HWND (the
// OpenMFC CWnd constructor registers nothing), which is the case unless (1)'s
// direct CreateEdit happened; if it did, the msctls_updown32 window is
// leaked and the handle-map entry CWnd::Create added for it
// (core/window/CWnd.cpp, g_hwndMap) is left pointing into freed storage.
// (3) RTTI read from slot 0 answers CMFCToolBarEditBoxButton for an object
// built here.
//
// Every entry point returns early on a NULL `this` or a NULL pointer /
// reference argument; retail tests none of them.  Guards beyond that are
// called out where they occur.
// The destructor reproduces retail's "store my own vftable first" step with
// the vptr recorded by the constructors, so that the base destructor thunk's
// C++ virtual destructor call is safe even when a client-derived class's MSVC
// table was in the slot.
//
// Virtual calls the retail bodies make on `this` or on m_wndSpin are made
// through the exported thunk of the body retail would reach for an object of
// exactly this class; each site says so.  A client subclass's override of one
// of those is NOT reached.

#include "detail/ManualSmallStubImplementationsSupport.h"

#include <climits>
#include <cstddef>
#include <cstring>

// ---------------------------------------------------------------------------
// Cross-file thunks.  Each was grepped to its definition in the file named;
// the parameter lists are derived from the mangled names.
// ---------------------------------------------------------------------------
//   featurepack/toolbar/Thunks.cpp : the base constructors (placement-new of
//   the mingw CMFCToolBarEditBoxButton) and destructor (its C++ virtual dtor)
extern "C" void* MS_ABI impl___0CMFCToolBarEditBoxButton__QEAA_XZ(void* pThis);
extern "C" void* MS_ABI impl___0CMFCToolBarEditBoxButton__QEAA_IHKH_Z(void* pThis, unsigned int uiID, int iImage, unsigned long dwStyle, int iWidth);
extern "C" void MS_ABI impl___1CMFCToolBarEditBoxButton__UEAA_XZ(void* pThis);
//   featurepack/toolbar/CMFCToolBarEditBoxButton.cpp : the base-class overrides this class extends
extern "C" void MS_ABI impl__CopyFrom_CMFCToolBarEditBoxButton__UEAAXAEBVCMFCToolBarButton___Z(
    CMFCToolBarEditBoxButton* pThis, const CMFCToolBarButton* pSrc);
extern "C" void MS_ABI impl__Serialize_CMFCToolBarEditBoxButton__UEAAXAEAVCArchive___Z(CMFCToolBarEditBoxButton* pThis, CArchive* ar);
extern "C" CEdit* MS_ABI impl__CreateEdit_CMFCToolBarEditBoxButton__UEAAPEAVCEdit__PEAVCWnd__AEBVCRect___Z(
    CMFCToolBarEditBoxButton* pThis, CWnd* pWndParent, const RECT* pRect);
extern "C" void MS_ABI impl__OnMove_CMFCToolBarEditBoxButton__UEAAXXZ(CMFCToolBarEditBoxButton* pThis);
extern "C" int MS_ABI impl__SetACCData_CMFCToolBarEditBoxButton__UEAAHPEAVCWnd__AEAVCAccessibilityData___Z(
    CMFCToolBarEditBoxButton* pThis, CWnd* pParent, CAccessibilityData* pData);
//   core/window/CtorDtorPlacement.cpp : CWnd constructor / destructor (placement-new / C++ dtor)
extern "C" void* MS_ABI impl___0CWnd__QEAA_XZ(void* pThis);
extern "C" void MS_ABI impl___1CWnd__UEAA_XZ(void* pThis);
//   core/window/CWnd.cpp / core/window/Thunks.cpp
extern "C" CWnd* MS_ABI impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(HWND hWnd);
extern "C" int MS_ABI impl__DestroyWindow_CWnd__UEAAHXZ(CWnd* pThis);
extern "C" int MS_ABI impl__ShowWindow_CWnd__QEAAHH_Z(CWnd* pThis, int nCmdShow);
extern "C" void MS_ABI impl__ScreenToClient_CWnd__QEBAXPEAUtagRECT___Z(const CWnd* pThis, RECT* pRect);
//   detail/CtrlcoreSupport.cpp : CSpinButtonCtrl::Create (CWnd::Create with "msctls_updown32")
extern "C" int MS_ABI impl__Create_CSpinButtonCtrl__UEAAHKAEBUtagRECT__PEAVCWnd__I_Z(
    CSpinButtonCtrl* pThis, DWORD dwStyle, const RECT* pRect, CWnd* pParentWnd, UINT nID);
//   core/controls/Thunks.cpp : CToolTipCtrl::AddTool
extern "C" int MS_ABI impl__AddTool_CToolTipCtrl__QEAAHPEAVCWnd__PEB_WPEBUtagRECT___K_Z(
    CToolTipCtrl* pThis, CWnd* pWnd, const wchar_t* lpszText, const RECT* lpRectTool, unsigned __int64 nIDTool);
//   core/runtime/Thunks.cpp : CArchive::Read / Write
extern "C" unsigned int MS_ABI impl__Read_CArchive__QEAAIPEAXI_Z(CArchive* pThis, void* pBuf, unsigned int nMax);
extern "C" void MS_ABI impl__Write_CArchive__QEAAXPEBXI_Z(CArchive* pThis, const void* pBuf, unsigned int nMax);
//   detail/MfcExceptionsSupport.cpp : AfxThrowArchiveException
extern "C" void MS_ABI impl__AfxThrowArchiveException__YAXHPEB_W_Z(int cause, const wchar_t* lpszArchiveName);
//   detail/MemcoreSupport.cpp : ??2@YAPEAX_K@Z
extern "C" void* MS_ABI impl___2_YAPEAX_K_Z(std::size_t size);

// Exports defined later in this file that earlier bodies call (all in this TU).
extern "C" void* MS_ABI impl___0CMFCToolBarSpinEditBoxButton__QEAA_XZ(void* pThis);

namespace {

// Retail member offsets (see the file header).
constexpr std::size_t kOffWndEdit        = 0x90;    // CMFCToolBarEditBoxButton::m_pWndEdit
constexpr std::size_t kOffWndSpin        = 0xb0;    // m_wndSpin (CMFCSpinButtonCtrl)
constexpr std::size_t kOffSpinOwnState   = 0xb0 + 0xe8;   // CMFCSpinButtonCtrl's own 0x14 bytes
constexpr std::size_t kSpinOwnStateBytes = 0x14;    // +0xe8 (8) +0xf0 (4) +0xf4 (8)
constexpr std::size_t kOffMin            = 0x1b0;   // m_nMin
constexpr std::size_t kOffMax            = 0x1b4;   // m_nMax
constexpr std::size_t kObjectSize        = 0x1b8;   // 440

static_assert(sizeof(CMFCToolBarEditBoxButton) == kOffWndSpin, "retail sizeof(CMFCToolBarEditBoxButton) is 0xb0; m_wndSpin follows it");
static_assert(sizeof(CWnd) == 0xe8, "CWnd is 0xe8 bytes, as retail's CMFCSpinButtonCtrl ctor assumes (its own state starts at +0xe8)");
static_assert(kOffWndSpin + 0x100 == kOffMin, "CMFCSpinButtonCtrl occupies 0x100 bytes");
static_assert(kOffMax + sizeof(int) == kObjectSize, "sizeof(CMFCToolBarSpinEditBoxButton) == 0x1b8");
static_assert(offsetof(CWnd, m_hWnd) == 0x40, "CWnd::m_hWnd @0x40 (m_wndSpin's HWND is +0xf0 of the button)");
static_assert(offsetof(CMFCToolBarButton, m_nID) == 0x24, "m_nID @0x24 (CreateEdit's spin control ID)");
static_assert(offsetof(CMFCToolBarButton, m_strText) == 0x38, "m_strText @0x38 (SetACCData's value)");
static_assert(offsetof(CAccessibilityData, m_strAccValue) == 0x08, "CAccessibilityData::m_strAccValue @0x08");

// Message / style numbers the retail bodies use as immediates.
constexpr UINT  kUdmSetBuddy   = 0x469;       // UDM_SETBUDDY
constexpr UINT  kUdmSetRange32 = 0x46f;       // UDM_SETRANGE32
constexpr DWORD kSpinStyle     = 0x50000026;  // WS_CHILD|WS_VISIBLE|UDS_ARROWKEYS|UDS_ALIGNRIGHT|UDS_SETBUDDYINT
constexpr int   kSwShowNoActivate = 4;        // SW_SHOWNOACTIVATE
constexpr int   kSwHide        = 0;           // SW_HIDE
constexpr int   kArchiveEndOfFile = 3;        // CArchiveException::endOfFile
static_assert(kUdmSetBuddy == UDM_SETBUDDY && kUdmSetRange32 == UDM_SETRANGE32, "commctrl message numbers");
static_assert(kSpinStyle == (WS_CHILD | WS_VISIBLE | UDS_ARROWKEYS | UDS_ALIGNRIGHT | UDS_SETBUDDYINT), "spin style");

template <class T> inline T& At(void* p, std::size_t off) {
    return *reinterpret_cast<T*>(static_cast<char*>(p) + off);
}
template <class T> inline const T& At(const void* p, std::size_t off) {
    return *reinterpret_cast<const T*>(static_cast<const char*>(p) + off);
}

inline CEdit*  WndEdit(const void* p) { return At<CEdit*>(p, kOffWndEdit); }
inline CWnd*   WndSpin(void* p)       { return reinterpret_cast<CWnd*>(static_cast<char*>(p) + kOffWndSpin); }
inline int&    MinOf(void* p)         { return At<int>(p, kOffMin); }
inline int&    MaxOf(void* p)         { return At<int>(p, kOffMax); }
inline int     MinOf(const void* p)   { return At<int>(p, kOffMin); }
inline int     MaxOf(const void* p)   { return At<int>(p, kOffMax); }
inline CMFCToolBarEditBoxButton* Base(void* p) { return static_cast<CMFCToolBarEditBoxButton*>(p); }

// `pWnd->GetSafeHwnd() != NULL` as retail spells it: the pointer, then +0x40.
inline bool HasWindow(const CWnd* pWnd) { return pWnd != nullptr && pWnd->m_hWnd != nullptr; }

// The mingw CMFCToolBarEditBoxButton vptr the base-constructor thunk installs
// (recorded by the constructors; every object built here gets the same one).
void* g_ownVptr = nullptr;

// m_wndSpin's construction, standing in for ??0CMFCSpinButtonCtrl@@QEAA@XZ
// (RVA 0x132520, mfc140u) -- see the file header.
inline void ConstructSpin(void* pThis) {
    impl___0CWnd__QEAA_XZ(WndSpin(pThis));
    std::memset(static_cast<char*>(pThis) + kOffSpinOwnState, 0, kSpinOwnStateBytes);
}

// The two stores retail's constructors make after m_wndSpin (and Init makes).
inline void InitRange(void* pThis) {
    MinOf(pThis) = INT_MIN;   // 0x80000000
    MaxOf(pThis) = INT_MAX;   // 0x7fffffff
}

// CSpinButtonCtrl::SetBuddy(pWndBuddy), inline in MFC: SendMessage(UDM_SETBUDDY,
// (WPARAM)pWndBuddy->m_hWnd) on m_wndSpin's HWND (+0xf0), then
// CWnd::FromHandle (0x28ad70) on the result, which retail discards.
inline void SpinSetBuddy(void* pThis, HWND hWndBuddy) {
    LRESULT lPrev = ::SendMessage(WndSpin(pThis)->m_hWnd, kUdmSetBuddy, reinterpret_cast<WPARAM>(hWndBuddy), 0);
    (void)impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(reinterpret_cast<HWND>(lPrev));
}
// CSpinButtonCtrl::SetRange32(nMin, nMax), inline in MFC: both sign-extended (movslq).
inline void SpinSetRange32(void* pThis, int nMin, int nMax) {
    ::SendMessage(WndSpin(pThis)->m_hWnd, kUdmSetRange32,
                  static_cast<WPARAM>(static_cast<INT_PTR>(nMin)), static_cast<LPARAM>(nMax));
}

// CWnd::GetParent() as retail inlines it: CWnd::FromHandle(::GetParent(m_hWnd)).
inline CWnd* ParentOf(const CWnd* pWnd) {
    return impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(::GetParent(pWnd->m_hWnd));
}

} // namespace

// ===========================================================================
// Construction / destruction
// ===========================================================================

// Retail (RVA 0x17f050, mfc140u), fully transcribed:
//     CMFCToolBarEditBoxButton::CMFCToolBarEditBoxButton(uiID, iImage, dwStyle, iWidth);   // 0x167760
//     vfptr = &CMFCToolBarSpinEditBoxButton::`vftable';          // 0x18031af98 (mfc140u)
//     m_wndSpin.CMFCSpinButtonCtrl::CMFCSpinButtonCtrl();       // 0x132520
//     m_nMin = INT_MIN;  m_nMax = INT_MAX;                      // +0x1b0 / +0x1b4
// DEVIATIONS: the vfptr is left as the base thunk installs it and m_wndSpin is
// built as described in the file header.  The base thunk
// (featurepack/toolbar/Thunks.cpp) placement-news the afxmfc.h constructor,
// which today DROPS all four arguments (see headerRequests), so uiID / iImage
// / dwStyle / iWidth do not reach the object until that is fixed there.
// Symbol: ??0CMFCToolBarSpinEditBoxButton@@QEAA@IHKH@Z
extern "C" void* MS_ABI impl___0CMFCToolBarSpinEditBoxButton__QEAA_IHKH_Z(
    void* pThis, unsigned int uiID, int iImage, unsigned long dwStyle, int iWidth) {
    if (!pThis) return pThis;
    impl___0CMFCToolBarEditBoxButton__QEAA_IHKH_Z(pThis, uiID, iImage, dwStyle, iWidth);
    if (g_ownVptr == nullptr) g_ownVptr = *static_cast<void**>(pThis);
    ConstructSpin(pThis);
    InitRange(pThis);
    return pThis;
}
// Retail (RVA 0x17efb0, mfc140u): the same body over the default base
// constructor ??0CMFCToolBarEditBoxButton@@QEAA@XZ (0x167690).
// Symbol: ??0CMFCToolBarSpinEditBoxButton@@QEAA@XZ
extern "C" void* MS_ABI impl___0CMFCToolBarSpinEditBoxButton__QEAA_XZ(void* pThis) {
    if (!pThis) return pThis;
    impl___0CMFCToolBarEditBoxButton__QEAA_XZ(pThis);
    if (g_ownVptr == nullptr) g_ownVptr = *static_cast<void**>(pThis);
    ConstructSpin(pThis);
    InitRange(pThis);
    return pThis;
}
// Retail (RVA 0x17f0c0, mfc140u), fully transcribed:
//     vfptr = &CMFCToolBarSpinEditBoxButton::`vftable';          // 0x18031af98
//     if (m_wndSpin.GetSafeHwnd() != NULL)                      // +0xb0 non-null, then +0xf0
//         m_wndSpin.DestroyWindow();                            // m_wndSpin vslot 0xd0 (26), CWnd::DestroyWindow
//     m_wndSpin.~CMFCSpinButtonCtrl();   (inlined: vfptr = 0x18030ffe8; ??1CSpinButtonCtrl 0x296040)
//     CMFCToolBarEditBoxButton::~CMFCToolBarEditBoxButton();    // tail-jump to 0x167840
// DEVIATIONS: the vfptr stored first is the recorded base vptr (file header);
// DestroyWindow goes through the exported ?DestroyWindow@CWnd@@ (which also
// drops the handle-map entry) and m_wndSpin, being an OpenMFC CWnd, is torn
// down with the exported ??1CWnd@@ rather than ??1CSpinButtonCtrl@@.
// Symbol: ??1CMFCToolBarSpinEditBoxButton@@UEAA@XZ
extern "C" void MS_ABI impl___1CMFCToolBarSpinEditBoxButton__UEAA_XZ(void* pThis) {
    if (!pThis) return;
    if (g_ownVptr != nullptr) *static_cast<void**>(pThis) = g_ownVptr;
    CWnd* pSpin = WndSpin(pThis);
    if (HasWindow(pSpin)) impl__DestroyWindow_CWnd__UEAAHXZ(pSpin);
    impl___1CWnd__UEAA_XZ(pSpin);
    impl___1CMFCToolBarEditBoxButton__UEAA_XZ(pThis);
}

// Retail (RVA 0x17ef40, mfc140u), fully transcribed:
//     void* p = operator new(0x1b8);                            // 0x27f0
//     return p != NULL ? new (p) CMFCToolBarSpinEditBoxButton() : NULL;   // 0x17efb0
// (the memset is OpenMFC's, as in CMFCToolBarDateTimeCtrl.cpp: the storage is
// fully initialised before the constructors run.)
// Symbol: ?CreateObject@CMFCToolBarSpinEditBoxButton@@SAPEAVCObject@@XZ
extern "C" void* MS_ABI impl__CreateObject_CMFCToolBarSpinEditBoxButton__SAPEAVCObject__XZ() {
    void* p = impl___2_YAPEAX_K_Z(kObjectSize);
    if (p == nullptr) return nullptr;
    std::memset(p, 0, kObjectSize);
    return impl___0CMFCToolBarSpinEditBoxButton__QEAA_XZ(p);
}

// Retail (RVA 0x17f0a0, mfc140u), fully transcribed:
//     m_nMin = INT_MIN;  m_nMax = INT_MAX;                      // +0x1b0 = 0x80000000, +0x1b4 = 0x7fffffff
// Symbol: ?Init@CMFCToolBarSpinEditBoxButton@@IEAAXXZ
extern "C" void MS_ABI impl__Init_CMFCToolBarSpinEditBoxButton__IEAAXXZ(void* pThis) {
    if (!pThis) return;
    InitRange(pThis);
}

// ===========================================================================
// Range
// ===========================================================================

// Retail (RVA 0x17f490, mfc140u -- export ordinal 13531), fully transcribed:
//     m_nMin = nMin;  m_nMax = nMax;                            // +0x1b0 / +0x1b4
//     if (m_wndSpin.GetSafeHwnd() != NULL)
//         m_wndSpin.SetRange32(nMin, nMax);                     // ::SendMessage(+0xf0, UDM_SETRANGE32, nMin, nMax), IAT 0x2c7120
// Symbol: ?SetRange@CMFCToolBarSpinEditBoxButton@@QEAAXHH@Z
extern "C" void MS_ABI impl__SetRange_CMFCToolBarSpinEditBoxButton__QEAAXHH_Z(void* pThis, int nMin, int nMax) {
    if (!pThis) return;
    MinOf(pThis) = nMin;
    MaxOf(pThis) = nMax;
    if (HasWindow(WndSpin(pThis))) SpinSetRange32(pThis, nMin, nMax);
}

// Retail (RVA 0x17f4e0, mfc140u), fully transcribed:
//     nMin = m_nMin;  nMax = m_nMax;
// Symbol: ?GetRange@CMFCToolBarSpinEditBoxButton@@QEAAXAEAH0@Z
extern "C" void MS_ABI impl__GetRange_CMFCToolBarSpinEditBoxButton__QEAAXAEAH0_Z(void* pThis, int* pnMin, int* pnMax) {
    if (!pThis) return;
    if (pnMin) *pnMin = MinOf(pThis);
    if (pnMax) *pnMax = MaxOf(pThis);
}

// ===========================================================================
// Copy / serialization
// ===========================================================================

// Retail (vftable slot 7, RVA 0x17f330, mfc140u), fully transcribed:
//     CMFCToolBarEditBoxButton::CopyFrom(src);                  // 0x1678d0
//     m_nMin = src.m_nMin;  m_nMax = src.m_nMax;               // +0x1b0 / +0x1b4, src not type-checked
// Symbol: ?CopyFrom@CMFCToolBarSpinEditBoxButton@@MEAAXAEBVCMFCToolBarButton@@@Z
extern "C" void MS_ABI impl__CopyFrom_CMFCToolBarSpinEditBoxButton__MEAAXAEBVCMFCToolBarButton___Z(
    void* pThis, const CMFCToolBarButton* pSrc) {
    if (!pThis || !pSrc) return;
    impl__CopyFrom_CMFCToolBarEditBoxButton__UEAAXAEBVCMFCToolBarButton___Z(Base(pThis), pSrc);
    MinOf(pThis) = MinOf(pSrc);
    MaxOf(pThis) = MaxOf(pSrc);
}

// Retail (vftable slot 2, RVA 0x17f370, mfc140u), fully transcribed:
//     CMFCToolBarEditBoxButton::Serialize(ar);                  // 0x167930
//     if (ar.IsLoading()) ar >> m_nMin >> m_nMax;               // CArchive+0x20 bit 0
//     else                ar << m_nMin << m_nMax;
// The int operators are CArchive's inline 4-byte buffer accesses (FillBuffer
// 0x1d1cc0 / Flush 0x1d1be0 when the buffer runs out); each re-tests the mode
// and throws AfxThrowArchiveException (0x1d3610) with writeOnly (4) on the
// load path / readOnly (2) on the store path on a mismatch, which cannot
// happen here since the mode was just tested.  They go through the exported
// CArchive::Read / Write here.  FillBuffer itself throws
// AfxThrowArchiveException(endOfFile (3), NULL) when fewer than the requested
// bytes remain (the `lea 0x3(%rdx),%ecx` at 0x1d1e57 inside FillBuffer
// 0x1d1cc0, mfc140u); CArchive::Read returns a short count instead, so a
// short read is turned into that same throw here.  As in retail, m_nMin is
// stored before m_nMax is read.
// Symbol: ?Serialize@CMFCToolBarSpinEditBoxButton@@MEAAXAEAVCArchive@@@Z
extern "C" void MS_ABI impl__Serialize_CMFCToolBarSpinEditBoxButton__MEAAXAEAVCArchive___Z(void* pThis, CArchive* ar) {
    if (!pThis || !ar) return;
    impl__Serialize_CMFCToolBarEditBoxButton__UEAAXAEAVCArchive___Z(Base(pThis), ar);
    if (ar->IsLoading()) {
        int v = 0;
        if (impl__Read_CArchive__QEAAIPEAXI_Z(ar, &v, sizeof v) != sizeof v)
            impl__AfxThrowArchiveException__YAXHPEB_W_Z(kArchiveEndOfFile, nullptr);
        MinOf(pThis) = v;
        v = 0;
        if (impl__Read_CArchive__QEAAIPEAXI_Z(ar, &v, sizeof v) != sizeof v)
            impl__AfxThrowArchiveException__YAXHPEB_W_Z(kArchiveEndOfFile, nullptr);
        MaxOf(pThis) = v;
    } else {
        int v = MinOf(pThis);
        impl__Write_CArchive__QEAAXPEBXI_Z(ar, &v, sizeof v);
        v = MaxOf(pThis);
        impl__Write_CArchive__QEAAXPEBXI_Z(ar, &v, sizeof v);
    }
}

// ===========================================================================
// The edit / spin pair
// ===========================================================================

// Retail (vftable slot 53, RVA 0x17f170, mfc140u), fully transcribed:
//     CEdit* pEdit = CMFCToolBarEditBoxButton::CreateEdit(pWndParent, rect);   // 0x168590
//     if (pEdit == NULL) return NULL;
//     if (!m_wndSpin.Create(WS_CHILD|WS_VISIBLE|UDS_ARROWKEYS|UDS_ALIGNRIGHT|UDS_SETBUDDYINT,   // 0x50000026
//                           rect, pWndParent, m_nID))           // m_wndSpin vslot 0x2d8 (91), ?Create@CSpinButtonCtrl@@ 0x295f60
//         return NULL;                                          // pEdit is neither destroyed nor deleted
//     m_wndSpin.SetBuddy(pEdit);                                // UDM_SETBUDDY, (WPARAM)pEdit->m_hWnd; FromHandle on the result
//     m_wndSpin.SetRange32(m_nMin, m_nMax);                     // UDM_SETRANGE32 (::SendMessage, IAT 0x2c7120)
//     return pEdit;
// Create is reached through the exported ?Create@CSpinButtonCtrl@@ thunk
// rather than m_wndSpin's vftable (m_wndSpin is an OpenMFC CWnd).  The
// NULL-on-spin-failure path, which leaks the edit retail just created, is
// retail's and is kept.
// Symbol: ?CreateEdit@CMFCToolBarSpinEditBoxButton@@MEAAPEAVCEdit@@PEAVCWnd@@AEBVCRect@@@Z
extern "C" CEdit* MS_ABI impl__CreateEdit_CMFCToolBarSpinEditBoxButton__MEAAPEAVCEdit__PEAVCWnd__AEBVCRect___Z(
    void* pThis, CWnd* pWndParent, const RECT* pRect) {
    if (!pThis || !pRect) return nullptr;
    CEdit* pEdit = impl__CreateEdit_CMFCToolBarEditBoxButton__UEAAPEAVCEdit__PEAVCWnd__AEBVCRect___Z(Base(pThis), pWndParent, pRect);
    if (pEdit == nullptr) return nullptr;
    if (!impl__Create_CSpinButtonCtrl__UEAAHKAEBUtagRECT__PEAVCWnd__I_Z(
            reinterpret_cast<CSpinButtonCtrl*>(WndSpin(pThis)), kSpinStyle, pRect, pWndParent, Base(pThis)->m_nID)) {
        return nullptr;
    }
    SpinSetBuddy(pThis, pEdit->m_hWnd);
    SpinSetRange32(pThis, MinOf(pThis), MaxOf(pThis));
    return pEdit;
}

// Retail (vftable slot 14, RVA 0x17f210, mfc140u -- export ordinal 10522), fully transcribed:
//     CMFCToolBarEditBoxButton::OnMove();                       // 0x167cc0
//     if (m_pWndEdit != NULL && m_pWndEdit->m_hWnd != NULL)     // +0x90, then +0x40
//         m_wndSpin.SetBuddy(m_pWndEdit);                       // UDM_SETBUDDY; m_wndSpin's own HWND is not tested
// Symbol: ?OnMove@CMFCToolBarSpinEditBoxButton@@MEAAXXZ
extern "C" void MS_ABI impl__OnMove_CMFCToolBarSpinEditBoxButton__MEAAXXZ(void* pThis) {
    if (!pThis) return;
    impl__OnMove_CMFCToolBarEditBoxButton__UEAAXXZ(Base(pThis));
    CEdit* pEdit = WndEdit(pThis);
    if (HasWindow(pEdit)) SpinSetBuddy(pThis, pEdit->m_hWnd);
}

// Retail (vftable slot 55, RVA 0x17f260, mfc140u), fully transcribed
// (m_pWndEdit and both FromHandle results are dereferenced unguarded):
//     ::GetWindowRect(m_pWndEdit->m_hWnd, &rectBorder);          // IAT 0x2c6c08
//     m_pWndEdit->GetParent()->ScreenToClient(&rectBorder);      // ::GetParent (IAT 0x2c72d8) + FromHandle + 0x2a32b0
//     CRect rectSpin(0, 0, 0, 0);
//     ::GetWindowRect(m_wndSpin.m_hWnd, &rectSpin);
//     m_wndSpin.GetParent()->ScreenToClient(&rectSpin);
//     rectBorder.right = rectSpin.right;
//     ::InflateRect(&rectBorder, 1, 1);                          // IAT 0x2c72e8
// DEVIATION: a NULL m_pWndEdit returns at once, leaving rectBorder untouched,
// and a NULL parent from FromHandle skips only that ScreenToClient; retail
// would fault in both cases.
// Symbol: ?GetEditBorder@CMFCToolBarSpinEditBoxButton@@MEAAXAEAVCRect@@@Z
extern "C" void MS_ABI impl__GetEditBorder_CMFCToolBarSpinEditBoxButton__MEAAXAEAVCRect___Z(void* pThis, RECT* pRectBorder) {
    if (!pThis || !pRectBorder) return;
    CEdit* pEdit = WndEdit(pThis);
    if (pEdit == nullptr) return;
    ::GetWindowRect(pEdit->m_hWnd, pRectBorder);
    if (CWnd* pParent = ParentOf(pEdit)) impl__ScreenToClient_CWnd__QEBAXPEAUtagRECT___Z(pParent, pRectBorder);
    CWnd* pSpin = WndSpin(pThis);
    RECT rectSpin = {0, 0, 0, 0};
    ::GetWindowRect(pSpin->m_hWnd, &rectSpin);
    if (CWnd* pParent = ParentOf(pSpin)) impl__ScreenToClient_CWnd__QEBAXPEAUtagRECT___Z(pParent, &rectSpin);
    pRectBorder->right = rectSpin.right;
    ::InflateRect(pRectBorder, 1, 1);
}

// Retail (vftable slot 56, RVA 0x17f600, mfc140u -- export ordinal 11179), fully transcribed:
//     if (m_wndSpin.GetSafeHwnd() != NULL)
//         m_wndSpin.ShowWindow(bShow ? SW_SHOWNOACTIVATE : SW_HIDE);   // 0x2a9ad0
// Symbol: ?OnShowEditbox@CMFCToolBarSpinEditBoxButton@@MEAAXH@Z
extern "C" void MS_ABI impl__OnShowEditbox_CMFCToolBarSpinEditBoxButton__MEAAXH_Z(void* pThis, int bShow) {
    if (!pThis) return;
    CWnd* pSpin = WndSpin(pThis);
    if (HasWindow(pSpin)) impl__ShowWindow_CWnd__QEAAHH_Z(pSpin, bShow ? kSwShowNoActivate : kSwHide);
}

// Retail (vftable slot 45, RVA 0x17f500, mfc140u), fully transcribed (str is
// the 5th argument, on the stack; pWndParent and iButtonIndex are not read):
//     if (m_pWndEdit == NULL || !::IsWindow(m_pWndEdit->m_hWnd)) return FALSE;   // IAT 0x2c7138
//     CString strTipText;
//     LPCTSTR lpszTip = OnGetCustomToolTipText(strTipText) ? strTipText : str;  // vslot 0x160 (44)
//     wndToolTip.AddTool(m_pWndEdit, lpszTip, NULL, 0);                        // 0x275060
//     wndToolTip.AddTool(&m_wndSpin, lpszTip, NULL, 0);                        // unconditional
//     return TRUE;
// OnGetCustomToolTipText in this class's retail vftable is 0x71e0
// (`xor eax,eax; ret`), so for an object of exactly this class the tip is
// always `str`; it is not dispatched (file header), so a client subclass's
// override is not reached.
// Symbol: ?OnUpdateToolTip@CMFCToolBarSpinEditBoxButton@@MEAAHPEAVCWnd@@HAEAVCToolTipCtrl@@AEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@@Z
extern "C" int MS_ABI impl__OnUpdateToolTip_CMFCToolBarSpinEditBoxButton__MEAAHPEAVCWnd__HAEAVCToolTipCtrl__AEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
    void* pThis, CWnd* pWndParent, int iButtonIndex, CToolTipCtrl* pWndToolTip, CString* pStr) {
    (void)pWndParent; (void)iButtonIndex;
    if (!pThis || !pWndToolTip || !pStr) return 0;
    CEdit* pEdit = WndEdit(pThis);
    if (pEdit == nullptr || !::IsWindow(pEdit->m_hWnd)) return 0;
    const wchar_t* lpszTip = static_cast<const wchar_t*>(*pStr);
    impl__AddTool_CToolTipCtrl__QEAAHPEAVCWnd__PEB_WPEBUtagRECT___K_Z(pWndToolTip, pEdit, lpszTip, nullptr, 0);
    impl__AddTool_CToolTipCtrl__QEAAHPEAVCWnd__PEB_WPEBUtagRECT___K_Z(pWndToolTip, WndSpin(pThis), lpszTip, nullptr, 0);
    return 1;
}

// Retail (vftable slot 48, RVA 0x17f130, mfc140u), fully transcribed:
//     if (!CMFCToolBarEditBoxButton::SetACCData(pParent, data)) return FALSE;  // 0x168b20
//     data.m_strAccValue = m_strText;                                         // +0x08 <- +0x38 (0xde30)
//     return TRUE;
// (Retail's base body 0x168b20 already made that same assignment; retail
// repeats it.)  In OpenMFC today the base chain ends in
// ?SetACCData@CMFCToolBarButton@@, a stub returning 0
// (featurepack/toolbar/CMFCToolBarButton.cpp), so this returns FALSE and
// fills nothing until that base body exists.
// Symbol: ?SetACCData@CMFCToolBarSpinEditBoxButton@@UEAAHPEAVCWnd@@AEAVCAccessibilityData@@@Z
extern "C" int MS_ABI impl__SetACCData_CMFCToolBarSpinEditBoxButton__UEAAHPEAVCWnd__AEAVCAccessibilityData___Z(
    void* pThis, CWnd* pParent, CAccessibilityData* pData) {
    if (!pThis || !pData) return 0;
    if (!impl__SetACCData_CMFCToolBarEditBoxButton__UEAAHPEAVCWnd__AEAVCAccessibilityData___Z(Base(pThis), pParent, pData)) return 0;
    pData->m_strAccValue = Base(pThis)->m_strText;
    return 1;
}
