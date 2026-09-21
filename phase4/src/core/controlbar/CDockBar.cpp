// CDockBar — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp
//
// Method: every body below was decoded from the retail mfc140u.dll export
// (disas.py --u), the way core/ole/COleControl.cpp and core/controlbar/
// CControlBar.cpp do; each function names the RVA it was read from.  The
// handlers that are not in the RVA symbol map were located through the class
// message map (AFX_MSGMAP at 0x322138, entries at 0x322150, mfc140u):
//   WM_NCCALCSIZE 0x1d9280   WM_NCPAINT 0x1d9430   WM_WINDOWPOSCHANGING 0x1d94e0
//   WM_PAINT 0x1d9440        WM_ERASEBKGND 0x1d9310  WM_SIZEPARENT 0x1d9210
// and the virtuals through the CDockBar vftable at 0x321dc8 (mfc140u), which
// shares CControlBar's slots 91..107 (see the CControlBar.cpp file header) and
// adds 108 Create, 109 GetDockedVisibleCount.  Slots checked for this file:
//   91 CalcFixedLayout 0x1d8c20   93 OnUpdateCmdUI 0x27d0 (`ret`)
//   95 DoPaint 0x27d0 (`ret`)     102 IsVisible 0x1d74b0 (CControlBar's)
//   103 RecalcDelayShow 0x1d71f0  104 IsDockBar 0x3a60 (`mov $1,%eax; ret`)
//   109 GetDockedVisibleCount 0x1d82c0
//
// Object layout.  CDockBar is not declared in OpenMFC's public headers; client
// (MSVC) code allocates it with the retail layout and calls the exported
// constructor, so this file is the only place the layout lives.  The base is
// CControlBar (include/openmfc/afxole.h, retail-faithful, sizeof 0x148) and the
// tail was read out of the retail constructor ??0CDockBar@@QEAA@H@Z, RVA
// 0x1d8040 (mfc140u), in the member order the shipping afxpriv.h declares
// (read on this host: m_bFloating, m_arrBars, m_bLayoutQuery, m_rectLayout):
//   +0x148  BOOL      m_bFloating      `mov %edi,0x148(%rsi)`
//   +0x150  CPtrArray m_arrBars        vfptr 0x180321190 stored at +0x150; the
//                                      four qwords +0x158 m_pData, +0x160 m_nSize,
//                                      +0x168 m_nMaxSize, +0x170 m_nGrowBy zeroed
//   +0x178  BOOL      m_bLayoutQuery   `mov %ebp,0x178(%rsi)`
//   +0x17c  CRect     m_rectLayout     `lea 0x17c(%rsi)` then ::SetRectEmpty
//   sizeof 0x190 -- the m_nObjectSize (400) core/controlbar/RuntimeClasses.cpp
//   records for the class.
//
// m_arrBars.  Retail manipulates it through the exported CPtrArray methods
// (SetAtGrow 0x1d4910, InsertAt 0x1d4960, RemoveAt 0x1d4a60) and reads
// m_pData / m_nSize directly (every GetDockedControlBar / FindBar / operator[]
// site below is such a direct read).  OpenMFC's CPtrArray (afx.h
// OPENMFC_DECLARE_ARRAY_WRAPPER) keeps its elements in a side table keyed by
// the object address and never touches +0x158 / +0x160, so routing through
// its impl__ thunks would leave the retail fields -- the ones MSVC clients
// inline GetSize()/operator[] against -- permanently empty.  The bodies below
// therefore keep the retail fields themselves, with the file-local helpers in
// RetailArray reproducing CPtrArray::SetSize / SetAtGrow / InsertAt / RemoveAt
// (MFC array_p.cpp semantics: grow-by heuristic max(4, min(1024, size/8)),
// zero-fill on growth, memmove on insert/remove).  The buffer is allocated and
// freed only inside this DLL (the constructor's Add(NULL), the destructor's
// free), so the allocator is consistent.  DEVIATION: the CPtrArray vfptr at
// +0x150 is stored as nullptr, not the retail vftable (no MSVC-layout vtable
// exists for it here -- the same choice featurepack/ribbon/CMFCRibbonInfo.cpp
// makes for embedded CArrays), and a client that calls the exported
// CPtrArray::SetSize/SetAtGrow on m_arrBars itself would update the side table
// rather than these fields.
//
// vtable pointer.  The retail constructor and destructor store the CDockBar
// vftable (0x180321dc8, mfc140u) at +0.  There is no C++ CDockBar class in
// OpenMFC, so the object keeps the CControlBar (mingw) vtable the base
// constructor thunk installs; nothing in this file dispatches through it.
// Wherever retail calls through a vtable slot the bodies call the target's
// impl__ thunk directly (marked "vslot NN"), as CControlBar.cpp does; a
// client-derived override of such a slot is therefore not honoured.
//
// Placeholders.  m_arrBars elements are CControlBar* for docked bars, NULL for
// a row separator, and small integers (<= 0xffff, a bar's dialog-control ID)
// for a "place holder" that remembers where a hidden bar used to sit.  Retail
// tests `cmp $0xffff` to tell the two apart; so does this file.

#include "detail/ManualSmallStubImplementationsSupport.h"
#include "detail/CControlBarInfoSupport.h"
#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <uxtheme.h>
#include <vssym32.h>

// ---------------------------------------------------------------------------
// Sibling / base thunks, declared with the signature their mangled name
// describes; every definition was located with grep before use.
// ---------------------------------------------------------------------------
extern "C" void* MS_ABI impl___0CControlBar__IEAA_XZ(void* pThis);                                    // core/controlbar/Thunks.cpp
extern "C" void MS_ABI impl___1CControlBar__UEAA_XZ(void* pThis);                                     // core/controlbar/Thunks.cpp
extern "C" void MS_ABI impl__SetBarStyle_CControlBar__QEAAXK_Z(CControlBar* pThis, DWORD dwStyle);   // core/controlbar/CControlBar.cpp
extern "C" int MS_ABI impl__IsVisible_CControlBar__UEBAHXZ(const CControlBar* pThis);                 // core/controlbar/CControlBar.cpp
extern "C" DWORD MS_ABI impl__RecalcDelayShow_CControlBar__UEAAKPEAUAFX_SIZEPARENTPARAMS___Z(CControlBar* pThis, void* lpLayout); // CControlBar.cpp
extern "C" void MS_ABI impl__CalcInsideRect_CControlBar__UEBAXAEAVCRect__H_Z(const CControlBar* pThis, CRect& rect, int bHorz);  // CControlBar.cpp
extern "C" void* MS_ABI impl__CalcDynamicLayout_CControlBar__UEAA_AVCSize__HK_Z(void* pRet, CControlBar* pThis, int nLength, DWORD dwMode); // CControlBar.cpp
extern "C" void MS_ABI impl__CalcDynamicLayout_CToolBar__UEAA_AVCSize__HK_Z(
    CSize* pRet, CToolBar* pThis, int nLength, unsigned long dwMode);                                 // core/controlbar/CToolBar.cpp
extern "C" void MS_ABI impl__EraseNonClient_CControlBar__QEAAXXZ(CControlBar* pThis);                 // core/controlbar/CControlBar.cpp
extern "C" void MS_ABI impl__OnWindowPosChanging_CControlBar__QEAAXPEAUtagWINDOWPOS___Z(CControlBar* pThis, WINDOWPOS* pWndPos); // CControlBar.cpp
extern "C" __int64 MS_ABI impl__OnSizeParent_CControlBar__QEAA_J_K_J_Z(CControlBar* pThis, UINT64 wParam, __int64 lParam);       // CControlBar.cpp
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CToolBar__SAPEAUCRuntimeClass__XZ();              // core/controlbar/RuntimeClasses.cpp
extern "C" int MS_ABI impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(
    const CObject* pThis, const CRuntimeClass* pClass);                                               // core/runtime/CObject.cpp
extern "C" int MS_ABI impl__Create_CWnd__UEAAHPEB_W0KAEBUtagRECT__PEAV1_IPEAUCCreateContext___Z(
    CWnd* pThis, const wchar_t* lpszClassName, const wchar_t* lpszWindowName, DWORD dwStyle,
    const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext);                          // core/window/CWnd.cpp
extern "C" int MS_ABI impl__AfxEndDeferRegisterClass__YAHJ_Z(long fToRegister);                       // featurepack/CMFC_misc_stubs.cpp
extern "C" int MS_ABI impl__SetWindowPos_CWnd__QEAAHPEBV1_HHHHI_Z(
    CWnd* pThis, const CWnd* pWndInsertAfter, int x, int y, int cx, int cy, unsigned int nFlags);      // core/window/CWnd.cpp
extern "C" int MS_ABI impl__GetWindowTextW_CWnd__QEBAHPEA_WH_Z(const CWnd* pThis, wchar_t* lpszStringBuf, int nMaxCount); // core/window/CWnd.cpp
extern "C" void MS_ABI impl__AfxSetWindowText__YAXPEAUHWND____PEB_W_Z(HWND hWnd, const wchar_t* lpszText);  // core/collections/Globals.cpp
extern "C" int MS_ABI impl__GetDlgCtrlID_CWnd__QEBAHXZ(const CWnd* pThis);                            // core/window/Thunks.cpp
extern "C" void MS_ABI impl__ScreenToClient_CWnd__QEBAXPEAUtagRECT___Z(const CWnd* pThis, RECT* lpRect); // core/window/Thunks.cpp
extern "C" CWnd* MS_ABI impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(HWND hWnd);                     // core/window/CWnd.cpp
extern "C" CFrameWnd* MS_ABI impl__GetParentFrame_CWnd__QEBAPEAVCFrameWnd__XZ(const CWnd* pThis);     // core/window/Thunks.cpp
extern "C" int MS_ABI impl__ShowWindow_CWnd__QEAAHH_Z(CWnd* pThis, int nCmdShow);                     // core/window/CWnd.cpp
extern "C" int MS_ABI impl__DestroyWindow_CWnd__UEAAHXZ(CWnd* pThis);                                 // core/window/CWnd.cpp
extern "C" int MS_ABI impl__ModifyStyle_CWnd__QEAAHKKI_Z(CWnd* pThis, unsigned long dwRemove, unsigned long dwAdd, unsigned int nFlags); // core/window/CWnd.cpp
extern "C" __int64 MS_ABI impl__Default_CWnd__IEAA_JXZ(CWnd* pThis);                                  // core/window/Thunks.cpp
extern "C" void MS_ABI impl__ShowControlBar_CFrameWnd__QEAAXPEAVCControlBar__HH_Z(
    CFrameWnd* pThis, CControlBar* pBar, int bShow, int bDelay);                                      // core/frame/Thunks.cpp
extern "C" CControlBar* MS_ABI impl__GetControlBar_CFrameWnd__QEAAPEAVCControlBar__I_Z(CFrameWnd* pThis, unsigned int nID); // core/frame/Thunks.cpp
extern "C" void MS_ABI impl__AfxRepositionWindow__YAXPEAUAFX_SIZEPARENTPARAMS__PEAUHWND____PEBUtagRECT___Z(
    void* lpLayout, HWND hWnd, const RECT* lpRect);                                                   // featurepack/CMFC_misc_stubs.cpp
extern "C" CPaintDC* MS_ABI impl___0CPaintDC__QEAA_PEAVCWnd___Z(CPaintDC* pThis, CWnd* pWnd);        // core/gdi/CPaintDC.cpp
extern "C" void MS_ABI impl___1CPaintDC__UEAA_XZ(CPaintDC* pThis);                                    // core/gdi/CPaintDC.cpp
extern "C" void MS_ABI impl__AfxThrowInvalidArgException__YAXXZ();                                    // detail/MfcExceptionsSupport.cpp
extern "C" void MS_ABI impl__AfxThrowMemoryException__YAXXZ();                                        // detail/MfcExceptionsSupport.cpp

// Thunks defined further down in this file that earlier bodies call.
extern "C" CControlBar* MS_ABI impl__GetDockedControlBar_CDockBar__IEBAPEAVCControlBar__H_Z(const void* pThis, int nPos);
extern "C" int MS_ABI impl__GetDockedCount_CDockBar__QEBAHXZ(const void* pThis);
extern "C" int MS_ABI impl__GetDockedVisibleCount_CDockBar__UEBAHXZ(const void* pThis);
extern "C" int MS_ABI impl__FindBar_CDockBar__QEAAHPEAVCControlBar__H_Z(void* pThis, CControlBar* pBar, int nPosExclude);
extern "C" int MS_ABI impl__Insert_CDockBar__IEAAHPEAVCControlBar__VCRect__VCPoint___Z(void* pThis, CControlBar* pBar, const RECT* pRect, long long ptMid);
extern "C" int MS_ABI impl__RemoveControlBar_CDockBar__QEAAHPEAVCControlBar__HH_Z(void* pThis, CControlBar* pBar, int nPosExclude, int nAddPlaceHolder);
extern "C" void MS_ABI impl__RemovePlaceHolder_CDockBar__QEAAXPEAVCControlBar___Z(void* pThis, CControlBar* pBar);
extern "C" void MS_ABI impl__DoPaint_CDockBar__UEAAXPEAVCDC___Z(void* pThis, CDC* pDC);

namespace {

// ---------------------------------------------------------------------------
// Layout view (see the file header for where every offset comes from).
// ---------------------------------------------------------------------------
struct DockBarTail {
    int      m_bFloating;          // 0x148
    int      _pad14c;              // 0x14c
    void*    m_arrBars_vfptr;      // 0x150  CPtrArray vfptr (retail 0x180321190; nullptr here)
    void**   m_arrBars_pData;      // 0x158  CPtrArray::m_pData
    INT_PTR  m_arrBars_nSize;      // 0x160  CPtrArray::m_nSize
    INT_PTR  m_arrBars_nMaxSize;   // 0x168  CPtrArray::m_nMaxSize
    INT_PTR  m_arrBars_nGrowBy;    // 0x170  CPtrArray::m_nGrowBy
    int      m_bLayoutQuery;       // 0x178
    RECT     m_rectLayout;         // 0x17c
};
constexpr size_t kOffTail = 0x148;
constexpr size_t kSizeofCDockBar = 0x190;
static_assert(sizeof(CControlBar) == kOffTail, "CControlBar must be 0x148 bytes (CDockBar tail starts there)");
static_assert(offsetof(DockBarTail, m_bFloating) + kOffTail == 0x148, "CDockBar::m_bFloating");
static_assert(offsetof(DockBarTail, m_arrBars_vfptr) + kOffTail == 0x150, "CDockBar::m_arrBars");
static_assert(offsetof(DockBarTail, m_arrBars_pData) + kOffTail == 0x158, "CDockBar::m_arrBars.m_pData");
static_assert(offsetof(DockBarTail, m_arrBars_nSize) + kOffTail == 0x160, "CDockBar::m_arrBars.m_nSize");
static_assert(offsetof(DockBarTail, m_arrBars_nMaxSize) + kOffTail == 0x168, "CDockBar::m_arrBars.m_nMaxSize");
static_assert(offsetof(DockBarTail, m_arrBars_nGrowBy) + kOffTail == 0x170, "CDockBar::m_arrBars.m_nGrowBy");
static_assert(offsetof(DockBarTail, m_bLayoutQuery) + kOffTail == 0x178, "CDockBar::m_bLayoutQuery");
static_assert(offsetof(DockBarTail, m_rectLayout) + kOffTail == 0x17c, "CDockBar::m_rectLayout");
static_assert(sizeof(DockBarTail) + kOffTail == kSizeofCDockBar, "sizeof(CDockBar) == 0x190");

// CControlBar members retail touches, pinned to the offsets the disassembly uses.
static_assert(offsetof(CWnd, m_hWnd) == 0x40, "CWnd::m_hWnd");
static_assert(offsetof(CControlBar, m_bAutoDelete) == 0xf0, "CControlBar::m_bAutoDelete");
static_assert(offsetof(CControlBar, m_cxLeftBorder) == 0xf4, "CControlBar::m_cxLeftBorder");
static_assert(offsetof(CControlBar, m_cxRightBorder) == 0xf8, "CControlBar::m_cxRightBorder");
static_assert(offsetof(CControlBar, m_cyTopBorder) == 0xfc, "CControlBar::m_cyTopBorder");
static_assert(offsetof(CControlBar, m_cyBottomBorder) == 0x100, "CControlBar::m_cyBottomBorder");
static_assert(offsetof(CControlBar, m_hReBarTheme) == 0x118, "CControlBar::m_hReBarTheme");
static_assert(offsetof(CControlBar, m_dwStyle) == 0x124, "CControlBar::m_dwStyle");
static_assert(offsetof(CControlBar, m_dwDockStyle) == 0x128, "CControlBar::m_dwDockStyle");
static_assert(offsetof(CControlBar, m_pDockSite) == 0x130, "CControlBar::m_pDockSite");
static_assert(offsetof(CControlBar, m_pDockBar) == 0x138, "CControlBar::m_pDockBar");
static_assert(offsetof(CControlBar, m_pDockContext) == 0x140, "CControlBar::m_pDockContext");

// CDC::m_hDC -- retail reads pDC+8 in OnEraseBkgnd.
static_assert(offsetof(CDC, m_hDC) == 0x8, "CDC::m_hDC");

// CControlBarInfo (detail/CControlBarInfoSupport.h) fields GetBarInfo /
// SetBarInfo touch, at the offsets the disassembly uses.
static_assert(offsetof(S_Ccontrolbarinfo, m_bFloating) == 0x08, "CControlBarInfo::m_bFloating");
static_assert(offsetof(S_Ccontrolbarinfo, m_bHorz) == 0x0c, "CControlBarInfo::m_bHorz");
static_assert(offsetof(S_Ccontrolbarinfo, m_bDockBar) == 0x10, "CControlBarInfo::m_bDockBar");
static_assert(offsetof(S_Ccontrolbarinfo, m_pointPos_x) == 0x14, "CControlBarInfo::m_pointPos");
static_assert(offsetof(S_Ccontrolbarinfo, m_arrBarID_vfptr) == 0x48, "CControlBarInfo::m_arrBarID");
static_assert(offsetof(S_Ccontrolbarinfo, m_arrBarID_pData) == 0x50, "CControlBarInfo::m_arrBarID.m_pData");
static_assert(offsetof(S_Ccontrolbarinfo, m_arrBarID_nSize) == 0x58, "CControlBarInfo::m_arrBarID.m_nSize");

// CFrameWnd::m_nIdleFlags lives at CFrameWnd + 0x1d0: retail
// CFrameWnd::OnIdleUpdateCmdUI (0x2a0140, mfc140u) tests bits 1/2/8 of
// 0x1d0(%rcx) and calls RecalcLayout (vslot 96) for bit 8 with bNotify =
// bit 4, and every DelayRecalcLayout() inline in this file is
// `orl $0xc,0x1d0(%rax)` (idleLayout 8 | idleNotify 4, afxwin.h).  OpenMFC's
// CFrameWnd does not name the member -- the slot is inside the
// zero-initialised _framewnd_padding -- so it is written at the retail
// offset.  KNOWN GAP (not introduced here): OpenMFC's frame idle handler
// (core/frame/CFrameWnd.cpp OnIdleUpdateCmdUI) never reads these flags, so the
// deferred RecalcLayout retail performs at idle time does not happen in this
// tree; the flag is still set exactly as retail sets it.
constexpr size_t kOffFrameIdleFlags = 0x1d0;
constexpr unsigned int kIdleNotify = 4;
constexpr unsigned int kIdleLayout = 8;
// (_framewnd_padding is protected, so the window is pinned through the last
// public member before it and the object size.)
static_assert(offsetof(CFrameWnd, m_nIDHelp) + sizeof(UINT) <= kOffFrameIdleFlags &&
              kOffFrameIdleFlags + sizeof(unsigned int) <= sizeof(CFrameWnd),
              "m_nIdleFlags slot must lie inside CFrameWnd's padding");
inline void DelayRecalcLayout(CFrameWnd* pFrame) {
    unsigned char* p = reinterpret_cast<unsigned char*>(pFrame) + kOffFrameIdleFlags;
    unsigned int flags = 0;
    std::memcpy(&flags, p, sizeof(flags));
    flags |= kIdleLayout | kIdleNotify;
    std::memcpy(p, &flags, sizeof(flags));
}

// CWnd::m_hWndOwner lives at CWnd + 0xa0 (see the note in CControlBar.cpp);
// SetBarInfo reads and writes it on the floating frame:
// `cmpq $0x0,0xa0(%rax)` / `mov %rcx,0xa0(%rax)` at 0x221ee6 / 0x221ef4.
constexpr size_t kOffHWndOwner = 0xa0;
static_assert(offsetof(CWnd, _cwnd_padding2) <= kOffHWndOwner &&
              kOffHWndOwner + sizeof(HWND) <= offsetof(CWnd, _cwnd_padding2) + sizeof(CWnd::_cwnd_padding2),
              "m_hWndOwner slot must lie inside CWnd's padding");
inline HWND& HWndOwnerSlot(CWnd* pWnd) {
    return *reinterpret_cast<HWND*>(reinterpret_cast<unsigned char*>(pWnd) + kOffHWndOwner);
}

// CDockContext::m_rectMRUDockPos: the shipping afxpriv.h (read on this host)
// declares, in order, vptr, CPoint m_ptLast, CRect m_rectLast, CSize m_sizeLast,
// BOOL m_bDitherLast, four CRects m_rectDragHorz/Vert/FrameDragHorz/Vert,
// CControlBar* m_pBar, CFrameWnd* m_pDockSite, DWORD m_dwDockStyle,
// m_dwOverDockStyle, m_dwStyle, BOOL m_bFlip, m_bForceFrame, CDC* m_pDC,
// BOOL m_bDragging, int m_nHitTest, UINT m_uMRUDockID, CRect m_rectMRUDockPos
// -- which puts m_rectMRUDockPos at +0xac, exactly where CalcFixedLayout
// stores it (`movdqu %xmm0,0xac(%rax)` at 0x1d8ee3 / 0x1d9071).  OpenMFC does
// not lay CDockContext out (core/controlbar/CDockContext.cpp is all
// placeholders), so the write goes through this offset.
constexpr size_t kOffDockContextRectMRUDockPos = 0xac;

// Retail CBRS_* / LM_* values as the disassembly tests them (the header's
// CBRS_* macros in include/openmfc/afxole.h do NOT match retail's bits; see
// CControlBar.cpp).
constexpr DWORD kCBRS_FLOATING      = 0x0001;
constexpr DWORD kCBRS_SIZE_FIXED    = 0x0002;
constexpr DWORD kCBRS_SIZE_DYNAMIC  = 0x0004;
constexpr DWORD kCBRS_FLOAT_MULTI   = 0x0040;
constexpr DWORD kCBRS_BORDER_ANY    = 0x0F00;
constexpr DWORD kCBRS_ALIGN_ANY     = 0xF000;
constexpr DWORD kCBRS_ORIENT_HORZ   = 0xA000;   // CBRS_ALIGN_TOP | CBRS_ALIGN_BOTTOM
constexpr DWORD kCBRS_ALL_MASK      = 0x40FFFF; // the mask Create applies to dwStyle
constexpr DWORD kLM_HORZ     = 0x02;
constexpr DWORD kLM_MRUWIDTH = 0x04;
constexpr DWORD kLM_HORZDOCK = 0x08;
constexpr DWORD kLM_VERTDOCK = 0x10;
constexpr long  kAFX_WNDCONTROLBAR_REG = 2;     // AfxEndDeferRegisterClass argument at 0x1d81e7
constexpr DWORD kMFS_MOVEFRAME = 0x800;         // ModifyStyle argument at 0x221f0a
constexpr int   kRP_BACKGROUND = 6;             // vssym32.h REBAR part

// AUX_DATA::cxBorder2 / cyBorder2 (afxData + 0x10 / + 0x14 = 0x3c32d0 /
// 0x3c32d4, mfc140u).  Retail's AUX_DATA constructor stores 2 to both and
// OpenMFC's exported afxData is a zero blob, so the values are folded in as
// literals, as CControlBar.cpp does.
constexpr int kCxBorder2 = 2;
constexpr int kCyBorder2 = 2;

// Mirror of MFC's AFX_SIZEPARENTPARAMS (afxpriv.h) -- the same shape
// CControlBar.cpp uses.  Retail reads hDWP at +0 and rect at +8.
struct SizeParentParams {
    HDWP hDWP;
    RECT rect;
    SIZE sizeTotal;
    BOOL bStretch;
};
static_assert(offsetof(SizeParentParams, hDWP) == 0x00, "AFX_SIZEPARENTPARAMS::hDWP");
static_assert(offsetof(SizeParentParams, rect) == 0x08, "AFX_SIZEPARENTPARAMS::rect");

inline DockBarTail& Tail(const void* pThis) {
    return *reinterpret_cast<DockBarTail*>(reinterpret_cast<unsigned char*>(const_cast<void*>(pThis)) + kOffTail);
}
inline CControlBar* AsBar(void* pThis) { return static_cast<CControlBar*>(pThis); }
inline const CControlBar* AsBar(const void* pThis) { return static_cast<const CControlBar*>(pThis); }
inline CWnd* AsWnd(void* pThis) { return static_cast<CWnd*>(static_cast<CControlBar*>(pThis)); }

// A pointer-sized element that is a place-holder ID rather than a CControlBar*
// (retail: `cmp $0xffff, ...`).
inline bool IsPlaceHolder(const void* p) { return reinterpret_cast<UINT_PTR>(p) <= 0xffff; }

// ---------------------------------------------------------------------------
// Retail CArray semantics on the raw (vfptr, m_pData, m_nSize, m_nMaxSize,
// m_nGrowBy) fields -- MFC array_p.cpp / array_d.cpp, which is what the
// exported CPtrArray / CUIntArray methods retail calls do.  new BYTE[] and
// delete[] become malloc/free; an allocation failure throws
// AfxThrowMemoryException as operator new does in retail.
// ---------------------------------------------------------------------------
template <typename T>
struct RetailArray {
    T*&      pData;
    INT_PTR& nSize;
    INT_PTR& nMaxSize;
    INT_PTR& nGrowBy;

    void SetSize(INT_PTR nNewSize, INT_PTR nNewGrowBy) {
        if (nNewGrowBy >= 0) nGrowBy = nNewGrowBy;
        if (nNewSize == 0) {
            std::free(pData);
            pData = nullptr;
            nSize = nMaxSize = 0;
        } else if (pData == nullptr) {
            pData = static_cast<T*>(std::malloc(static_cast<size_t>(nNewSize) * sizeof(T)));
            if (pData == nullptr) impl__AfxThrowMemoryException__YAXXZ();
            std::memset(pData, 0, static_cast<size_t>(nNewSize) * sizeof(T));
            nSize = nMaxSize = nNewSize;
        } else if (nNewSize <= nMaxSize) {
            if (nNewSize > nSize) {
                std::memset(pData + nSize, 0, static_cast<size_t>(nNewSize - nSize) * sizeof(T));
            }
            nSize = nNewSize;
        } else {
            INT_PTR grow = nGrowBy;
            if (grow == 0) {
                grow = nSize / 8;
                grow = (grow < 4) ? 4 : ((grow > 1024) ? 1024 : grow);
            }
            INT_PTR nNewMax = (nNewSize < nMaxSize + grow) ? nMaxSize + grow : nNewSize;
            T* pNewData = static_cast<T*>(std::malloc(static_cast<size_t>(nNewMax) * sizeof(T)));
            if (pNewData == nullptr) impl__AfxThrowMemoryException__YAXXZ();
            std::memcpy(pNewData, pData, static_cast<size_t>(nSize) * sizeof(T));
            std::memset(pNewData + nSize, 0, static_cast<size_t>(nNewSize - nSize) * sizeof(T));
            std::free(pData);
            pData = pNewData;
            nSize = nNewSize;
            nMaxSize = nNewMax;
        }
    }
    void SetAtGrow(INT_PTR nIndex, T newElement) {
        if (nIndex >= nSize) SetSize(nIndex + 1, -1);
        pData[nIndex] = newElement;
    }
    void InsertAt(INT_PTR nIndex, T newElement, INT_PTR nCount) {
        if (nIndex >= nSize) {
            SetSize(nIndex + nCount, -1);
        } else {
            INT_PTR nOldSize = nSize;
            SetSize(nSize + nCount, -1);
            std::memmove(pData + nIndex + nCount, pData + nIndex, static_cast<size_t>(nOldSize - nIndex) * sizeof(T));
            std::memset(pData + nIndex, 0, static_cast<size_t>(nCount) * sizeof(T));
        }
        while (nCount--) pData[nIndex++] = newElement;
    }
    void RemoveAt(INT_PTR nIndex, INT_PTR nCount) {
        INT_PTR nMoveCount = nSize - (nIndex + nCount);
        if (nMoveCount) {
            std::memmove(pData + nIndex, pData + nIndex + nCount, static_cast<size_t>(nMoveCount) * sizeof(T));
        }
        nSize -= nCount;
    }
};
inline RetailArray<void*> Bars(void* pThis) {
    DockBarTail& t = Tail(pThis);
    return RetailArray<void*>{t.m_arrBars_pData, t.m_arrBars_nSize, t.m_arrBars_nMaxSize, t.m_arrBars_nGrowBy};
}
// Bounds-checked element read: CPtrArray::operator[] / GetAt with its ENSURE,
// which retail compiles to `js/jge -> AfxThrowInvalidArgException`.
inline void* BarAt(void* pThis, INT_PTR nIndex) {
    DockBarTail& t = Tail(pThis);
    if (nIndex < 0 || nIndex >= t.m_arrBars_nSize) {
        impl__AfxThrowInvalidArgException__YAXXZ();
        return nullptr;
    }
    return t.m_arrBars_pData[nIndex];
}

inline int KindOf(const CControlBar* pBar, CRuntimeClass* pClass) {
    return impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(static_cast<const CObject*>(pBar), pClass);
}
// vslot 92 (CalcDynamicLayout) on a docked bar, devirtualised the way
// CControlBar.cpp's CallCalcDynamicLayout does: CToolBar's override carries a
// correctly typed thunk, everything else gets CControlBar's.  (CReBar's
// override is still a placeholder; a client-derived override is not honoured.)
inline CSize CallCalcDynamicLayout(CControlBar* pBar, int nLength, DWORD dwMode) {
    CSize size(0, 0);
    if (KindOf(pBar, impl__GetThisClass_CToolBar__SAPEAUCRuntimeClass__XZ())) {
        impl__CalcDynamicLayout_CToolBar__UEAA_AVCSize__HK_Z(&size, static_cast<CToolBar*>(pBar), nLength, dwMode);
    } else {
        impl__CalcDynamicLayout_CControlBar__UEAA_AVCSize__HK_Z(&size, pBar, nLength, dwMode);
    }
    return size;
}

// GetDockingFrame() as every retail body in this file inlines it:
//     CFrameWnd* pFrame = GetParentFrame();      // 0x28e200
//     if (pFrame == NULL) pFrame = m_pDockSite;  // +0x130
inline CFrameWnd* DockingFrame(void* pThis) {
    CFrameWnd* pFrame = impl__GetParentFrame_CWnd__QEBAPEAVCFrameWnd__XZ(AsWnd(pThis));
    if (pFrame == nullptr) pFrame = static_cast<CFrameWnd*>(AsBar(pThis)->m_pDockSite);
    return pFrame;
}

// The style-adoption prologue DockControlBar (0x1d83b5..0x1d844c) and
// ReDockControlBar (0x1d8701..0x1d8792) share instruction for instruction
// (not byte for byte: the rel32 call displacements differ, and DockControlBar
// keeps the CBRS_FLOATING bit in %r14d where ReDockControlBar uses `or $1`):
//     if (m_bFloating && (pBar->m_dwDockStyle & CBRS_FLOAT_MULTI)) m_dwStyle |= CBRS_FLOAT_MULTI;
//     m_dwStyle &= ~(CBRS_SIZE_FIXED | CBRS_SIZE_DYNAMIC);
//     m_dwStyle |= pBar->m_dwStyle & (CBRS_SIZE_FIXED | CBRS_SIZE_DYNAMIC);
//     if (!(m_dwStyle & CBRS_FLOAT_MULTI)) {
//         WCHAR szTitle[_MAX_PATH];                       // 0x104
//         pBar->GetWindowText(szTitle, _MAX_PATH);        // 0x2a9810
//         AfxSetWindowText(m_hWnd, szTitle);              // 0x2ae4b0
//     }
//     pBar->SetBarStyle(AdoptedBarStyle(pBar));           // 0x1d6020
inline DWORD AdoptedBarStyle(void* pThis, const CControlBar* pBar) {
    // dwStyle = (pBar->m_dwStyle & ~CBRS_ALIGN_ANY) | (m_dwStyle & CBRS_ALIGN_ANY) | CBRS_BORDER_ANY,
    // then CBRS_FLOATING set or cleared from m_bFloating (0x1d8415..0x1d8449).
    DWORD dwStyle = (pBar->m_dwStyle & ~kCBRS_ALIGN_ANY) | (AsBar(pThis)->m_dwStyle & kCBRS_ALIGN_ANY) | kCBRS_BORDER_ANY;
    if (Tail(pThis).m_bFloating) dwStyle |= kCBRS_FLOATING;
    else dwStyle &= ~kCBRS_FLOATING;
    return dwStyle;
}
inline void AdoptBarStyle(void* pThis, CControlBar* pBar) {
    CControlBar* pSelf = AsBar(pThis);
    if (Tail(pThis).m_bFloating && (pBar->m_dwDockStyle & kCBRS_FLOAT_MULTI)) {
        pSelf->m_dwStyle |= kCBRS_FLOAT_MULTI;
    }
    pSelf->m_dwStyle &= ~(kCBRS_SIZE_FIXED | kCBRS_SIZE_DYNAMIC);
    pSelf->m_dwStyle |= pBar->m_dwStyle & (kCBRS_SIZE_FIXED | kCBRS_SIZE_DYNAMIC);
    if (!(pSelf->m_dwStyle & kCBRS_FLOAT_MULTI)) {
        wchar_t szTitle[_MAX_PATH];
        impl__GetWindowTextW_CWnd__QEBAHPEA_WH_Z(pBar, szTitle, _MAX_PATH);
        impl__AfxSetWindowText__YAXPEAUHWND____PEB_W_Z(pSelf->m_hWnd, szTitle);
    }
    // SetBarStyle goes to the CControlBar.cpp thunk, which stores m_dwStyle
    // only; retail 0x1d6020 also calls vslot 105 OnBarStyleChange when the
    // style changes (a CControlBar.cpp gap, not reproduced here).
    impl__SetBarStyle_CControlBar__QEAAXK_Z(pBar, AdoptedBarStyle(pThis, pBar));
}

// The re-parent step DockControlBar / ReDockControlBar / SetBarInfo share:
//     if (CWnd::FromHandle(::GetParent(pBar->m_hWnd)) != this)      // IAT GetParent; 0x28ad70
//         CWnd::FromHandle(::SetParent(pBar->m_hWnd, m_hWnd));      // IAT SetParent; result discarded
inline void ReparentToThis(void* pThis, CControlBar* pBar) {
    if (impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(::GetParent(pBar->m_hWnd)) != AsWnd(pThis)) {
        impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(::SetParent(pBar->m_hWnd, AsWnd(pThis)->m_hWnd));
    }
}

// "m_arrBars.RemoveAt(nPos); if (m_arrBars[nPos-1] == NULL && m_arrBars[nPos] == NULL)
//  m_arrBars.RemoveAt(nPos);" -- the row-collapse step RemovePlaceHolder and
// both RemoveControlBar branches perform (RemoveAt 0x1d4a60; both reads are
// bounds-checked operator[] calls that throw).
inline void RemoveAtAndCollapseRow(void* pThis, INT_PTR nPos) {
    Bars(pThis).RemoveAt(nPos, 1);
    if (BarAt(pThis, nPos - 1) == nullptr && BarAt(pThis, nPos) == nullptr) {
        Bars(pThis).RemoveAt(nPos, 1);
    }
}

} // namespace

// Transcribed from retail RVA 0x1d8040 (mfc140u):
//     CControlBar::CControlBar();                                  // 0x1d5ef0
//     vfptr = &CDockBar::`vftable' (0x180321dc8);                  // not reproduced (file header)
//     m_arrBars.vfptr = &CPtrArray::`vftable' (0x180321190);       // stored as nullptr (file header)
//     m_arrBars.m_pData = m_nSize = m_nMaxSize = m_nGrowBy = 0;
//     m_rectLayout = {0};
//     m_bFloating = bFloating;                                     // +0x148
//     m_bAutoDelete = TRUE;                                        // +0xf0
//     m_arrBars.Add(NULL);   i.e. SetAtGrow(m_arrBars.m_nSize, NULL)   // 0x1d4910
//     m_bLayoutQuery = FALSE;                                      // +0x178
//     ::SetRectEmpty(&m_rectLayout);                               // IAT SetRectEmpty
//     m_cyTopBorder = m_cyBottomBorder = 0;                        // one qword store at +0xfc
//     m_cxLeftBorder = m_cxRightBorder = 0;                        // one qword store at +0xf4
//     return this;
// Symbol: ??0CDockBar@@QEAA@H@Z
extern "C" void* MS_ABI impl___0CDockBar__QEAA_H_Z(void* pThis, int bFloating) {
    if (pThis == nullptr) return nullptr;   // deviation: retail has no NULL check
    impl___0CControlBar__IEAA_XZ(pThis);
    DockBarTail& t = Tail(pThis);
    t.m_arrBars_vfptr = nullptr;
    t.m_arrBars_pData = nullptr;
    t.m_arrBars_nSize = 0;
    t.m_arrBars_nMaxSize = 0;
    t.m_arrBars_nGrowBy = 0;
    std::memset(&t.m_rectLayout, 0, sizeof(t.m_rectLayout));
    t.m_bFloating = bFloating;
    AsBar(pThis)->m_bAutoDelete = TRUE;
    Bars(pThis).SetAtGrow(t.m_arrBars_nSize, nullptr);
    t.m_bLayoutQuery = FALSE;
    ::SetRectEmpty(&t.m_rectLayout);
    AsBar(pThis)->m_cyTopBorder = 0;
    AsBar(pThis)->m_cyBottomBorder = 0;
    AsBar(pThis)->m_cxLeftBorder = 0;
    AsBar(pThis)->m_cxRightBorder = 0;
    return pThis;
}
// Transcribed from retail RVA 0x1d8130 (mfc140u):
//     vfptr = &CDockBar::`vftable' (0x180321dc8);                  // not reproduced (file header)
//     for (int i = 0; i < m_arrBars.m_nSize; i++) {
//         CControlBar* pBar = GetDockedControlBar(i);              // 0x1d95f0
//         if (pBar != NULL && pBar->m_pDockBar == this) pBar->m_pDockBar = NULL;   // +0x138
//     }
//     m_arrBars.~CPtrArray();   inlined: vfptr = &CPtrArray::`vftable'; free(m_pData)   // IAT free
//     CControlBar::~CControlBar();                                 // tail jump 0x1d6100
// Symbol: ??1CDockBar@@UEAA@XZ
extern "C" void MS_ABI impl___1CDockBar__UEAA_XZ(void* pThis) {
    if (pThis == nullptr) return;   // deviation: retail has no NULL check
    DockBarTail& t = Tail(pThis);
    for (int i = 0; i < t.m_arrBars_nSize; i++) {
        CControlBar* pBar = impl__GetDockedControlBar_CDockBar__IEBAPEAVCControlBar__H_Z(pThis, i);
        if (pBar != nullptr && pBar->m_pDockBar == pThis) {
            pBar->m_pDockBar = nullptr;
        }
    }
    std::free(t.m_arrBars_pData);
    t.m_arrBars_pData = nullptr;
    impl___1CControlBar__UEAA_XZ(pThis);
}
// Transcribed from retail RVA 0x1d8c20 (mfc140u; vslot 91):
//     CSize sizeFixed = CControlBar::CalcFixedLayout(bStretch, bHorz);   // inlined; the retail base (0xac30) is exactly:
//         sizeFixed.cx = (bStretch && bHorz) ? 32767 : 0;  sizeFixed.cy = (bStretch && !bHorz) ? 32767 : 0;
//     CSize sizeMax;
//     if (!::IsRectEmpty(&m_rectLayout)) {                         // IAT IsRectEmpty; +0x17c
//         CRect rect = m_rectLayout;  CalcInsideRect(rect, bHorz);   // vslot 94
//         sizeMax = rect.Size();
//     } else {
//         CRect rectFrame = {0};
//         CFrameWnd* pFrame = GetParentFrame();                    // 0x28e200
//         if (pFrame == NULL) AfxThrowInvalidArgException();       // 0x227720
//         ::GetClientRect(pFrame->m_hWnd, &rectFrame);             // IAT GetClientRect
//         sizeMax = rectFrame.Size();
//     }
//     AFX_SIZEPARENTPARAMS layout;   // only hDWP is initialised
//     layout.hDWP = m_bLayoutQuery ? NULL : ::BeginDeferWindowPos((int)m_arrBars.m_nSize);   // +0x178; IAT
//     CPoint pt(-afxData.cxBorder2, -afxData.cyBorder2);           // 0x3c32d0 / 0x3c32d4
//     int nWidth = 0;  BOOL bWrapped = FALSE;
//     for (int nPos = 0; nPos < m_arrBars.m_nSize; nPos++) {
//         CControlBar* pBar = GetDockedControlBar(nPos);           // 0x1d95f0
//         void* pVoid = m_arrBars[nPos];                           // bounds-checked read
//         if (pBar != NULL) {
//             if (pBar->IsVisible()) {                             // vslot 102
//                 DWORD dwMode;
//                 if ((pBar->m_dwStyle & (CBRS_SIZE_DYNAMIC|CBRS_FLOATING)) == (CBRS_SIZE_DYNAMIC|CBRS_FLOATING))
//                     dwMode = LM_HORZ | LM_MRUWIDTH;                                   // 6
//                 else if (pBar->m_dwStyle & CBRS_ORIENT_HORZ) dwMode = LM_HORZ | LM_HORZDOCK;   // 0xa
//                 else dwMode = LM_VERTDOCK;                                            // 0x10
//                 CSize sizeBar = pBar->CalcDynamicLayout(-1, dwMode);                  // vslot 92
//                 CRect rect(pt, sizeBar);
//                 CRect rectBar = {0};  ::GetWindowRect(pBar->m_hWnd, &rectBar);  ScreenToClient(&rectBar);   // IAT; 0x2a32b0
//                 if (bHorz) {
//                     if (rectBar.left > rect.left && !m_bFloating) ::OffsetRect(&rect, rectBar.left - rect.left, 0);   // +0x148; IAT OffsetRect
//                     if (rect.right > sizeMax.cx && !m_bFloating) {
//                         int x = max(sizeMax.cx - (rect.Width() - afxData.cxBorder2), pt.x);
//                         ::OffsetRect(&rect, x - rect.left, 0);
//                     }
//                     if (bWrapped) { bWrapped = FALSE; ::OffsetRect(&rect, -(rect.left + afxData.cxBorder2), 0); }
//                     else if (rect.left >= sizeMax.cx - afxData.cxBorder2 && nPos > 0 && m_arrBars[nPos-1] != NULL) {
//                         m_arrBars.InsertAt(nPos, NULL, 1);       // 0x1d4960
//                         bWrapped = TRUE;  pBar = NULL;  pVoid = NULL;   -> falls into the end-of-row block
//                     }
//                     if (!bWrapped) {
//                         if (!::EqualRect(&rect, &rectBar)) {     // IAT EqualRect
//                             if (!m_bLayoutQuery && !(pBar->m_dwStyle & CBRS_FLOATING))
//                                 pBar->m_pDockContext->m_rectMRUDockPos = rect;   // +0x140, +0xac
//                             AfxRepositionWindow(&layout, pBar->m_hWnd, &rect);   // 0x28ecc0
//                         }
//                         pt.x = rect.left + sizeBar.cx - afxData.cxBorder2;
//                         nWidth = max(nWidth, sizeBar.cy);
//                     }
//                 } else {  (same with top/bottom, cy, cyBorder2, sizeMax.cy, nWidth = max(nWidth, sizeBar.cx))
//                 }
//             }
//             if (!bWrapped) pBar->RecalcDelayShow(&layout);       // vslot 103
//         }
//         if (pBar == NULL && pVoid == NULL && nWidth != 0) {      // end of row
//             if (bHorz) { pt.y += nWidth - afxData.cyBorder2;  sizeFixed.cx = max(sizeFixed.cx, pt.x);
//                          sizeFixed.cy = max(sizeFixed.cy, pt.y);  pt.x = -afxData.cxBorder2; }
//             else       { pt.x += nWidth - afxData.cxBorder2;  sizeFixed.cx = max(sizeFixed.cx, pt.x);
//                          sizeFixed.cy = max(sizeFixed.cy, pt.y);  pt.y = -afxData.cyBorder2; }
//             nWidth = 0;
//         }
//     }
//     if (!m_bLayoutQuery && layout.hDWP != NULL) ::EndDeferWindowPos(layout.hDWP);   // IAT EndDeferWindowPos
//     CRect rect;  ::SetRectEmpty(&rect);  CalcInsideRect(rect, bHorz);               // IAT; vslot 94
//     if ((!bStretch || !bHorz) && sizeFixed.cx != 0) sizeFixed.cx += rect.left - rect.right;
//     if ((!bStretch || bHorz)  && sizeFixed.cy != 0) sizeFixed.cy += rect.top - rect.bottom;
//     return sizeFixed;
// The invisible-bar path skips the end-of-row block (pVoid == pBar != NULL), as
// does a place-holder element (pVoid != NULL); only a genuine NULL separator or
// a just-inserted wrap reaches it.  vslots 94/102/103 go to the CControlBar.cpp
// thunks, vslot 92 through CallCalcDynamicLayout.  The base CalcFixedLayout is
// written out inline as retail has it (OpenMFC's CControlBar thunk returns the
// window size instead, so it is deliberately not called).  DEVIATION: retail writes
// m_pDockContext->m_rectMRUDockPos without a NULL test; OpenMFC's
// CControlBar::EnableDocking never creates a CDockContext, so the store is
// skipped when m_pDockContext is NULL.  The afxData reads are folded in as
// literals (see the constants above).  KNOWN GAP (not introduced here): retail
// reaches this slot from CControlBar::CalcDynamicLayout (0x1d61d0, mfc140u),
// which is just `return CalcFixedLayout(dwMode & LM_STRETCH, dwMode & LM_HORZ)`
// through vslot 91; OpenMFC's CControlBar.cpp CalcDynamicLayout thunk returns
// the window size instead and never dispatches slot 91, so this body only runs
// when a client calls the export directly.
// Symbol: ?CalcFixedLayout@CDockBar@@UEAA?AVCSize@@HH@Z
extern "C" void* MS_ABI impl__CalcFixedLayout_CDockBar__UEAA_AVCSize__HH_Z(void* pRet, void* pThis, int bStretch, int bHorz) {
    CSize* pSize = new (pRet) CSize(0, 0);
    if (pThis == nullptr) {
        impl__AfxThrowInvalidArgException__YAXXZ();
        return pRet;
    }
    CControlBar* pSelf = AsBar(pThis);
    DockBarTail& t = Tail(pThis);

    CSize sizeFixed((bStretch && bHorz) ? 32767 : 0, (bStretch && !bHorz) ? 32767 : 0);

    SIZE sizeMax;
    if (!::IsRectEmpty(&t.m_rectLayout)) {
        CRect rect(t.m_rectLayout);
        impl__CalcInsideRect_CControlBar__UEBAXAEAVCRect__H_Z(pSelf, rect, bHorz);   // vslot 94
        sizeMax.cx = rect.right - rect.left;
        sizeMax.cy = rect.bottom - rect.top;
    } else {
        RECT rectFrame = {};
        CFrameWnd* pFrame = impl__GetParentFrame_CWnd__QEBAPEAVCFrameWnd__XZ(pSelf);
        if (pFrame == nullptr) {
            impl__AfxThrowInvalidArgException__YAXXZ();
            return pRet;
        }
        ::GetClientRect(pFrame->m_hWnd, &rectFrame);
        sizeMax.cx = rectFrame.right - rectFrame.left;
        sizeMax.cy = rectFrame.bottom - rectFrame.top;
    }

    SizeParentParams layout = {};
    layout.hDWP = t.m_bLayoutQuery ? nullptr : ::BeginDeferWindowPos(static_cast<int>(t.m_arrBars_nSize));
    POINT pt = { -kCxBorder2, -kCyBorder2 };
    int nWidth = 0;
    BOOL bWrapped = FALSE;

    for (int nPos = 0; nPos < t.m_arrBars_nSize; nPos++) {
        CControlBar* pBar = impl__GetDockedControlBar_CDockBar__IEBAPEAVCControlBar__H_Z(pThis, nPos);
        void* pVoid = BarAt(pThis, nPos);
        if (pBar != nullptr) {
            if (impl__IsVisible_CControlBar__UEBAHXZ(pBar)) {   // vslot 102
                DWORD dwMode;
                if ((pBar->m_dwStyle & (kCBRS_SIZE_DYNAMIC | kCBRS_FLOATING)) == (kCBRS_SIZE_DYNAMIC | kCBRS_FLOATING)) {
                    dwMode = kLM_HORZ | kLM_MRUWIDTH;
                } else if (pBar->m_dwStyle & kCBRS_ORIENT_HORZ) {
                    dwMode = kLM_HORZ | kLM_HORZDOCK;
                } else {
                    dwMode = kLM_VERTDOCK;
                }
                CSize sizeBar = CallCalcDynamicLayout(pBar, -1, dwMode);   // vslot 92
                RECT rect = { pt.x, pt.y, pt.x + sizeBar.cx, pt.y + sizeBar.cy };
                RECT rectBar = {};
                ::GetWindowRect(pBar->m_hWnd, &rectBar);
                impl__ScreenToClient_CWnd__QEBAXPEAUtagRECT___Z(pSelf, &rectBar);

                if (bHorz) {
                    if (rectBar.left > rect.left && !t.m_bFloating) {
                        ::OffsetRect(&rect, rectBar.left - rect.left, 0);
                    }
                    if (rect.right > sizeMax.cx && !t.m_bFloating) {
                        int x = sizeMax.cx - ((rect.right - rect.left) - kCxBorder2);
                        if (x < pt.x) x = pt.x;
                        ::OffsetRect(&rect, x - rect.left, 0);
                    }
                    if (bWrapped) {
                        bWrapped = FALSE;
                        ::OffsetRect(&rect, -(rect.left + kCxBorder2), 0);
                    } else if (rect.left >= sizeMax.cx - kCxBorder2 && nPos > 0 && BarAt(pThis, nPos - 1) != nullptr) {
                        Bars(pThis).InsertAt(nPos, nullptr, 1);
                        bWrapped = TRUE;
                        pBar = nullptr;
                        pVoid = nullptr;
                    }
                    if (!bWrapped) {
                        if (!::EqualRect(&rect, &rectBar)) {
                            if (!t.m_bLayoutQuery && !(pBar->m_dwStyle & kCBRS_FLOATING) && pBar->m_pDockContext != nullptr) {
                                std::memcpy(reinterpret_cast<unsigned char*>(pBar->m_pDockContext) + kOffDockContextRectMRUDockPos, &rect, sizeof(rect));
                            }
                            impl__AfxRepositionWindow__YAXPEAUAFX_SIZEPARENTPARAMS__PEAUHWND____PEBUtagRECT___Z(&layout, pBar->m_hWnd, &rect);
                        }
                        pt.x = rect.left + sizeBar.cx - kCxBorder2;
                        if (sizeBar.cy > nWidth) nWidth = sizeBar.cy;
                    }
                } else {
                    if (rectBar.top > rect.top && !t.m_bFloating) {
                        ::OffsetRect(&rect, 0, rectBar.top - rect.top);
                    }
                    if (rect.bottom > sizeMax.cy && !t.m_bFloating) {
                        int y = sizeMax.cy - ((rect.bottom - rect.top) - kCyBorder2);
                        if (y < pt.y) y = pt.y;
                        ::OffsetRect(&rect, 0, y - rect.top);
                    }
                    if (bWrapped) {
                        bWrapped = FALSE;
                        ::OffsetRect(&rect, 0, -(rect.top + kCyBorder2));
                    } else if (rect.top >= sizeMax.cy - kCyBorder2 && nPos > 0 && BarAt(pThis, nPos - 1) != nullptr) {
                        Bars(pThis).InsertAt(nPos, nullptr, 1);
                        bWrapped = TRUE;
                        pBar = nullptr;
                        pVoid = nullptr;
                    }
                    if (!bWrapped) {
                        if (!::EqualRect(&rect, &rectBar)) {
                            if (!t.m_bLayoutQuery && !(pBar->m_dwStyle & kCBRS_FLOATING) && pBar->m_pDockContext != nullptr) {
                                std::memcpy(reinterpret_cast<unsigned char*>(pBar->m_pDockContext) + kOffDockContextRectMRUDockPos, &rect, sizeof(rect));
                            }
                            impl__AfxRepositionWindow__YAXPEAUAFX_SIZEPARENTPARAMS__PEAUHWND____PEBUtagRECT___Z(&layout, pBar->m_hWnd, &rect);
                        }
                        pt.y = rect.top + sizeBar.cy - kCyBorder2;
                        if (sizeBar.cx > nWidth) nWidth = sizeBar.cx;
                    }
                }
            }
            if (!bWrapped) {
                impl__RecalcDelayShow_CControlBar__UEAAKPEAUAFX_SIZEPARENTPARAMS___Z(pBar, &layout);   // vslot 103
            }
        }
        if (pBar == nullptr && pVoid == nullptr && nWidth != 0) {
            if (bHorz) {
                pt.y += nWidth - kCyBorder2;
                if (pt.x > sizeFixed.cx) sizeFixed.cx = pt.x;
                if (pt.y > sizeFixed.cy) sizeFixed.cy = pt.y;
                pt.x = -kCxBorder2;
            } else {
                pt.x += nWidth - kCxBorder2;
                if (pt.x > sizeFixed.cx) sizeFixed.cx = pt.x;
                if (pt.y > sizeFixed.cy) sizeFixed.cy = pt.y;
                pt.y = -kCyBorder2;
            }
            nWidth = 0;
        }
    }

    if (!t.m_bLayoutQuery && layout.hDWP != nullptr) {
        ::EndDeferWindowPos(layout.hDWP);
    }

    CRect rect;
    ::SetRectEmpty(static_cast<RECT*>(rect));
    impl__CalcInsideRect_CControlBar__UEBAXAEAVCRect__H_Z(pSelf, rect, bHorz);   // vslot 94
    if ((!bStretch || !bHorz) && sizeFixed.cx != 0) sizeFixed.cx += rect.left - rect.right;
    if ((!bStretch || bHorz) && sizeFixed.cy != 0) sizeFixed.cy += rect.top - rect.bottom;
    *pSize = sizeFixed;
    return pRet;
}

// Transcribed from retail RVA 0x1d81b0 (mfc140u; vslot 108):
//     if (pParentWnd == NULL) AfxThrowInvalidArgException();        // 0x227720
//     m_dwStyle = dwStyle & 0x40FFFF;                                // +0x124
//     AfxEndDeferRegisterClass(AFX_WNDCONTROLBAR_REG /*2*/);         // 0x2918f0
//     CRect rect = {0};  ::SetRectEmpty(&rect);                      // IAT SetRectEmpty
//     return CWnd::Create(L"AfxControlBar140u" /*0x33eb90*/, NULL, dwStyle, rect, pParentWnd, nID, NULL);   // 0x28b690
// DEVIATION: OpenMFC registers a single window class (AfxEndDeferRegisterClass
// in featurepack/CMFC_misc_stubs.cpp ignores the bit and never registers
// "AfxControlBar140u"), so the class name is passed as NULL and CWnd::Create
// substitutes its own -- the same choice core/frame/CSplitterWnd.cpp makes.
// Symbol: ?Create@CDockBar@@UEAAHPEAVCWnd@@KI@Z
extern "C" int MS_ABI impl__Create_CDockBar__UEAAHPEAVCWnd__KI_Z(void* pThis, CWnd* pParentWnd, unsigned long dwStyle, unsigned int nID) {
    if (pThis == nullptr) return FALSE;   // deviation: retail has no NULL check on this
    if (pParentWnd == nullptr) {
        impl__AfxThrowInvalidArgException__YAXXZ();
        return FALSE;
    }
    AsBar(pThis)->m_dwStyle = static_cast<DWORD>(dwStyle & kCBRS_ALL_MASK);
    impl__AfxEndDeferRegisterClass__YAHJ_Z(kAFX_WNDCONTROLBAR_REG);
    RECT rect = {};
    ::SetRectEmpty(&rect);
    return impl__Create_CWnd__UEAAHPEB_W0KAEBUtagRECT__PEAV1_IPEAUCCreateContext___Z(
        AsWnd(pThis), nullptr, nullptr, static_cast<DWORD>(dwStyle), rect, pParentWnd, nID, nullptr);
}

// Transcribed from retail RVA 0x1d8330 (mfc140u):
//     if (this == NULL || pBar == NULL) AfxThrowInvalidArgException();   // 0x227720
//     CRect rectBar = {0};  ::GetWindowRect(pBar->m_hWnd, &rectBar);      // IAT GetWindowRect
//     if (pBar->m_pDockBar == this && (lpRect == NULL || ::EqualRect(&rectBar, lpRect)))   // +0x138; IAT EqualRect
//         return;                                                          // already docked here, no move
//     <style adoption + title + SetBarStyle -- see AdoptBarStyle above>
//     BOOL bShow = FALSE;
//     if (pBar->m_pDockBar != this && ::IsWindowVisible(pBar->m_hWnd)) {   // IAT IsWindowVisible
//         pBar->SetWindowPos(NULL, 0, 0, 0, 0, 0x97 /*NOSIZE|NOMOVE|NOZORDER|NOACTIVATE|HIDEWINDOW*/);   // 0x2a9a60
//         bShow = TRUE;
//     }
//     int nPos = -1;
//     if (lpRect != NULL) {
//         CRect rect;  ::CopyRect(&rect, lpRect);  ScreenToClient(&rect);   // IAT CopyRect; 0x2a32b0
//         CPoint ptMid(rect.left + rect.Width()/2, rect.top + rect.Height()/2);   // signed division (cltd/sar)
//         nPos = Insert(pBar, rect, ptMid);                                 // 0x1d9630
//         pBar->SetWindowPos(NULL, rect.left, rect.top, rect.Width(), rect.Height(), 0x114 /*NOZORDER|NOACTIVATE|NOCOPYBITS*/);
//     } else {
//         m_arrBars.Add(pBar);  m_arrBars.Add(NULL);                        // SetAtGrow 0x1d4910, twice
//         pBar->SetWindowPos(NULL, -afxData.cxBorder2, -afxData.cyBorder2, 0, 0, 0x115 /*NOSIZE|NOZORDER|NOACTIVATE|NOCOPYBITS*/);
//     }
//     <re-parent to this -- see ReparentToThis above>
//     if (pBar->m_pDockBar == this)
//         pBar->m_pDockBar->RemoveControlBar(pBar, nPos, 0);                // 0x1d8a10
//     else if (pBar->m_pDockBar != NULL)
//         pBar->m_pDockBar->RemoveControlBar(pBar, -1, m_bFloating && !pBar->m_pDockBar->m_bFloating);
//     pBar->m_pDockBar = this;
//     if (bShow) pBar->SetWindowPos(NULL, 0, 0, 0, 0, 0x57 /*NOSIZE|NOMOVE|NOZORDER|NOACTIVATE|SHOWWINDOW*/);
//     RemovePlaceHolder(pBar);                                              // 0x1d8960
//     GetDockingFrame()->DelayRecalcLayout();                               // 0x28e200 / +0x130; orl $0xc,0x1d0
// RemoveControlBar is a direct (non-virtual) call on the OTHER dock bar in the
// second branch; it goes to this file's thunk.  DEVIATION: a NULL docking
// frame is skipped (retail would fault).
// Symbol: ?DockControlBar@CDockBar@@QEAAXPEAVCControlBar@@PEBUtagRECT@@@Z
extern "C" void MS_ABI impl__DockControlBar_CDockBar__QEAAXPEAVCControlBar__PEBUtagRECT___Z(void* pThis, CControlBar* pBar, const RECT* lpRect) {
    if (pThis == nullptr || pBar == nullptr) {
        impl__AfxThrowInvalidArgException__YAXXZ();
        return;
    }
    DockBarTail& t = Tail(pThis);
    RECT rectBar = {};
    ::GetWindowRect(pBar->m_hWnd, &rectBar);
    if (pBar->m_pDockBar == pThis && (lpRect == nullptr || ::EqualRect(&rectBar, lpRect))) {
        return;
    }

    AdoptBarStyle(pThis, pBar);

    BOOL bShow = FALSE;
    if (pBar->m_pDockBar != pThis && ::IsWindowVisible(pBar->m_hWnd)) {
        impl__SetWindowPos_CWnd__QEAAHPEBV1_HHHHI_Z(pBar, nullptr, 0, 0, 0, 0,
            SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE | SWP_HIDEWINDOW);
        bShow = TRUE;
    }

    int nPos = -1;
    if (lpRect != nullptr) {
        RECT rect;
        ::CopyRect(&rect, lpRect);
        impl__ScreenToClient_CWnd__QEBAXPEAUtagRECT___Z(AsWnd(pThis), &rect);
        const int xMid = rect.left + (rect.right - rect.left) / 2;
        const int yMid = rect.top + (rect.bottom - rect.top) / 2;
        const long long ptMid = (static_cast<long long>(static_cast<unsigned int>(yMid)) << 32) | static_cast<unsigned int>(xMid);
        nPos = impl__Insert_CDockBar__IEAAHPEAVCControlBar__VCRect__VCPoint___Z(pThis, pBar, &rect, ptMid);
        impl__SetWindowPos_CWnd__QEAAHPEBV1_HHHHI_Z(pBar, nullptr, rect.left, rect.top,
            rect.right - rect.left, rect.bottom - rect.top, SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOCOPYBITS);
    } else {
        Bars(pThis).SetAtGrow(t.m_arrBars_nSize, pBar);
        Bars(pThis).SetAtGrow(t.m_arrBars_nSize, nullptr);
        impl__SetWindowPos_CWnd__QEAAHPEBV1_HHHHI_Z(pBar, nullptr, -kCxBorder2, -kCyBorder2, 0, 0,
            SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOCOPYBITS);
    }

    ReparentToThis(pThis, pBar);
    if (pBar->m_pDockBar == pThis) {
        impl__RemoveControlBar_CDockBar__QEAAHPEAVCControlBar__HH_Z(pThis, pBar, nPos, 0);
    } else if (pBar->m_pDockBar != nullptr) {
        void* pOther = pBar->m_pDockBar;
        const int nAddPlaceHolder = (t.m_bFloating && !Tail(pOther).m_bFloating) ? 1 : 0;
        impl__RemoveControlBar_CDockBar__QEAAHPEAVCControlBar__HH_Z(pOther, pBar, -1, nAddPlaceHolder);
    }
    pBar->m_pDockBar = pThis;

    if (bShow) {
        impl__SetWindowPos_CWnd__QEAAHPEBV1_HHHHI_Z(pBar, nullptr, 0, 0, 0, 0,
            SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE | SWP_SHOWWINDOW);
    }
    impl__RemovePlaceHolder_CDockBar__QEAAXPEAVCControlBar___Z(pThis, pBar);

    CFrameWnd* pFrame = DockingFrame(pThis);
    if (pFrame != nullptr) DelayRecalcLayout(pFrame);
}

// Retail: the CDockBar vftable (0x321dc8, mfc140u) holds 0x27d0 = `ret` in
// slot 95, and the mfc140u export table resolves ?DoPaint@CDockBar@@UEAAXPEAVCDC@@@Z
// to that same ICF-folded 0x27d0.  CDockBar paints nothing of its own (the
// borders come from OnEraseBkgnd / OnNcPaint); this empty body is the complete
// retail behaviour, not a placeholder.
// Symbol: ?DoPaint@CDockBar@@UEAAXPEAVCDC@@@Z
extern "C" void MS_ABI impl__DoPaint_CDockBar__UEAAXPEAVCDC___Z(void* /*pThis*/, CDC* /*pDC*/) {
}

// Transcribed from retail RVA 0x1d9520 (mfc140u):
//     for (int i = 0; i < m_arrBars.m_nSize; i++) {          // +0x160
//         if (i == nPosExclude) continue;
//         if (m_arrBars[i] == pBar) return i;                  // bounds-checked read of +0x158[i]
//     }
//     return -1;
// pBar is compared as a raw pointer value, so callers pass either a
// CControlBar* or a place-holder ID.
// Symbol: ?FindBar@CDockBar@@QEAAHPEAVCControlBar@@H@Z
extern "C" int MS_ABI impl__FindBar_CDockBar__QEAAHPEAVCControlBar__H_Z(void* pThis, CControlBar* pBar, int nPosExclude) {
    if (pThis == nullptr) return -1;   // deviation: retail has no NULL check
    DockBarTail& t = Tail(pThis);
    for (int i = 0; i < t.m_arrBars_nSize; i++) {
        if (i == nPosExclude) continue;
        if (BarAt(pThis, i) == static_cast<void*>(pBar)) return i;
    }
    return -1;
}

// Transcribed from retail RVA 0x221be0 (mfc140u):
//     pInfo->m_bDockBar = TRUE;                                    // +0x10
//     pInfo->m_bFloating = m_bFloating;                            // +0x8 <- +0x148
//     if (m_bFloating) {
//         CRect rect = {0};  ::GetWindowRect(m_hWnd, &rect);       // IAT GetWindowRect
//         pInfo->m_pointPos = rect.TopLeft();                      // +0x14 (one qword: left, top)
//     }
//     pInfo->m_bHorz = (m_dwStyle & CBRS_ORIENT_HORZ) != 0;        // +0xc
//     for (int i = 0; i < m_arrBars.m_nSize; i++) {
//         void* pVoid = m_arrBars[i];                              // bounds-checked read
//         UINT nID;
//         if (pVoid == NULL)              nID = 0;
//         else if ((UINT_PTR)pVoid <= 0xffff) nID = (WORD)pVoid | 0x10000;   // place holder: `bts $0x10`
//         else                            nID = ::GetDlgCtrlID(((CControlBar*)pVoid)->m_hWnd);   // IAT GetDlgCtrlID
//         pInfo->m_arrBarID.SetAtGrow(pInfo->m_arrBarID.m_nSize, nID);   // 0x1d4260; +0x48 / +0x58
//     }
// The CUIntArray append keeps the retail fields of the embedded array (see the
// file header on why the exported thunks are not used); the buffer it
// allocates is not released by OpenMFC's CUIntArray destructor thunk (side
// table only), so it is leaked rather than cross-freed.
// Symbol: ?GetBarInfo@CDockBar@@QEAAXPEAVCControlBarInfo@@@Z
extern "C" void MS_ABI impl__GetBarInfo_CDockBar__QEAAXPEAVCControlBarInfo___Z(void* pThis, void* pInfoRaw) {
    if (pThis == nullptr || pInfoRaw == nullptr) return;   // deviation: retail has no NULL checks
    S_Ccontrolbarinfo* pInfo = static_cast<S_Ccontrolbarinfo*>(pInfoRaw);
    DockBarTail& t = Tail(pThis);
    pInfo->m_bDockBar = TRUE;
    pInfo->m_bFloating = t.m_bFloating;
    if (t.m_bFloating) {
        RECT rect = {};
        ::GetWindowRect(AsWnd(pThis)->m_hWnd, &rect);
        pInfo->m_pointPos_x = rect.left;
        pInfo->m_pointPos_y = rect.top;
    }
    pInfo->m_bHorz = (AsBar(pThis)->m_dwStyle & kCBRS_ORIENT_HORZ) ? TRUE : FALSE;

    unsigned int*& pIDs = *reinterpret_cast<unsigned int**>(&pInfo->m_arrBarID_pData);
    RetailArray<unsigned int> ids{pIDs, pInfo->m_arrBarID_nSize, pInfo->m_arrBarID_nMaxSize, pInfo->m_arrBarID_nGrowBy};
    for (int i = 0; i < t.m_arrBars_nSize; i++) {
        void* pVoid = BarAt(pThis, i);
        unsigned int nID;
        if (pVoid == nullptr) {
            nID = 0;
        } else if (IsPlaceHolder(pVoid)) {
            nID = static_cast<unsigned int>(reinterpret_cast<UINT_PTR>(pVoid) & 0xffff) | 0x10000u;
        } else {
            nID = static_cast<unsigned int>(::GetDlgCtrlID(static_cast<CControlBar*>(pVoid)->m_hWnd));
        }
        ids.SetAtGrow(pInfo->m_arrBarID_nSize, nID);
    }
}

// Transcribed from retail RVA 0x1d95f0 (mfc140u):
//     if (nPos < 0 || nPos >= m_arrBars.m_nSize) AfxThrowInvalidArgException();   // 0x227720
//     void* p = m_arrBars.m_pData[nPos];                                       // +0x158
//     return ((UINT_PTR)p <= 0xffff) ? NULL : (CControlBar*)p;                 // cmovbe
// Symbol: ?GetDockedControlBar@CDockBar@@IEBAPEAVCControlBar@@H@Z
extern "C" CControlBar* MS_ABI impl__GetDockedControlBar_CDockBar__IEBAPEAVCControlBar__H_Z(const void* pThis, int nPos) {
    if (pThis == nullptr) return nullptr;   // deviation: retail has no NULL check
    const DockBarTail& t = Tail(pThis);
    if (nPos < 0 || nPos >= t.m_arrBars_nSize) {
        impl__AfxThrowInvalidArgException__YAXXZ();
        return nullptr;
    }
    void* p = t.m_arrBars_pData[nPos];
    return IsPlaceHolder(p) ? nullptr : static_cast<CControlBar*>(p);
}

// Transcribed from retail RVA 0x1d8260 (mfc140u):
//     int nCount = 0;
//     for (int i = 0; i < m_arrBars.m_nSize; i++)
//         if (GetDockedControlBar(i) != NULL) nCount++;             // 0x1d95f0
//     return nCount;
// Symbol: ?GetDockedCount@CDockBar@@QEBAHXZ
extern "C" int MS_ABI impl__GetDockedCount_CDockBar__QEBAHXZ(const void* pThis) {
    if (pThis == nullptr) return 0;   // deviation: retail has no NULL check
    int nCount = 0;
    for (int i = 0; i < Tail(pThis).m_arrBars_nSize; i++) {
        if (impl__GetDockedControlBar_CDockBar__IEBAPEAVCControlBar__H_Z(pThis, i) != nullptr) nCount++;
    }
    return nCount;
}

// Transcribed from retail RVA 0x1d82c0 (mfc140u; vslot 109):
//     int nCount = 0;
//     for (int i = 0; i < m_arrBars.m_nSize; i++) {
//         CControlBar* pBar = GetDockedControlBar(i);              // 0x1d95f0
//         if (pBar != NULL && pBar->IsVisible()) nCount++;         // vslot 102
//     }
//     return nCount;
// vslot 102 goes to the CControlBar.cpp thunk.
// Symbol: ?GetDockedVisibleCount@CDockBar@@UEBAHXZ
extern "C" int MS_ABI impl__GetDockedVisibleCount_CDockBar__UEBAHXZ(const void* pThis) {
    if (pThis == nullptr) return 0;   // deviation: retail has no NULL check
    int nCount = 0;
    for (int i = 0; i < Tail(pThis).m_arrBars_nSize; i++) {
        CControlBar* pBar = impl__GetDockedControlBar_CDockBar__IEBAPEAVCControlBar__H_Z(pThis, i);
        if (pBar != nullptr && impl__IsVisible_CControlBar__UEBAHXZ(pBar)) nCount++;
    }
    return nCount;
}

// Transcribed from retail RVA 0x1d9630 (mfc140u).  CRect arrives by value as a
// pointer to the caller's 16-byte copy (r8), CPoint by value in r9 (x in the
// low dword, y in the high dword):
//     if (this == NULL || pBarIns == NULL) AfxThrowInvalidArgException();   // 0x227720
//     int nPosInsAfter = 0, nWidth = 0, nTotalWidth = 0;
//     BOOL bHorz = m_dwStyle & CBRS_ORIENT_HORZ;                          // +0x124 & 0xa000
//     for (int nPos = 0; nPos < m_arrBars.m_nSize; nPos++) {
//         CControlBar* pBar = GetDockedControlBar(nPos);                    // 0x1d95f0
//         if (pBar != NULL) {
//             if (pBar->IsVisible()) {                                      // vslot 102
//                 CRect rectBar = {0};  ::GetWindowRect(pBar->m_hWnd, &rectBar);  ScreenToClient(&rectBar);   // IAT; 0x2a32b0
//                 nWidth = max(nWidth, bHorz ? rectBar.Height() : rectBar.Width() - 1);
//                 if (bHorz ? rect.left > rectBar.left : rect.top > rectBar.top) nPosInsAfter = nPos;
//             }
//         } else if (m_arrBars[nPos] == NULL) {      // bounds-checked read; a place-holder is neither
//             if (nWidth != 0) nTotalWidth += nWidth - afxData.cyBorder2;   // 0x3c32d4 in both orientations
//             if ((bHorz ? ptMid.y : ptMid.x) < nTotalWidth) {
//                 if (nPos == 0) m_arrBars.InsertAt(nPosInsAfter + 1, NULL, 1);   // 0x1d4960
//                 m_arrBars.InsertAt(nPosInsAfter + 1, pBarIns, 1);
//                 return nPosInsAfter + 1;
//             }
//             nWidth = 0;
//             nPosInsAfter = nPos;
//         }
//     }
//     m_arrBars.InsertAt(nPosInsAfter + 1, NULL, 1);       // new row
//     m_arrBars.InsertAt(nPosInsAfter + 1, pBarIns, 1);
//     return nPosInsAfter + 1;
// An invisible bar and a place-holder element both fall through to the next
// element without touching the row bookkeeping.  vslot 102 goes to the
// CControlBar.cpp thunk.
// Symbol: ?Insert@CDockBar@@IEAAHPEAVCControlBar@@VCRect@@VCPoint@@@Z
extern "C" int MS_ABI impl__Insert_CDockBar__IEAAHPEAVCControlBar__VCRect__VCPoint___Z(void* pThis, CControlBar* pBarIns, const RECT* pRect, long long ptMid) {
    if (pThis == nullptr || pBarIns == nullptr || pRect == nullptr) {
        impl__AfxThrowInvalidArgException__YAXXZ();
        return -1;
    }
    DockBarTail& t = Tail(pThis);
    const int ptMidX = static_cast<int>(static_cast<unsigned long long>(ptMid) & 0xffffffffu);
    const int ptMidY = static_cast<int>(static_cast<unsigned long long>(ptMid) >> 32);
    const bool bHorz = (AsBar(pThis)->m_dwStyle & kCBRS_ORIENT_HORZ) != 0;
    int nPosInsAfter = 0;
    int nWidth = 0;
    int nTotalWidth = 0;
    for (int nPos = 0; nPos < t.m_arrBars_nSize; nPos++) {
        CControlBar* pBar = impl__GetDockedControlBar_CDockBar__IEBAPEAVCControlBar__H_Z(pThis, nPos);
        if (pBar != nullptr) {
            if (impl__IsVisible_CControlBar__UEBAHXZ(pBar)) {   // vslot 102
                RECT rectBar = {};
                ::GetWindowRect(pBar->m_hWnd, &rectBar);
                impl__ScreenToClient_CWnd__QEBAXPEAUtagRECT___Z(AsWnd(pThis), &rectBar);
                const int nExtent = bHorz ? (rectBar.bottom - rectBar.top) : (rectBar.right - rectBar.left - 1);
                if (nExtent > nWidth) nWidth = nExtent;
                if (bHorz ? (pRect->left > rectBar.left) : (pRect->top > rectBar.top)) {
                    nPosInsAfter = nPos;
                }
            }
        } else if (BarAt(pThis, nPos) == nullptr) {
            if (nWidth != 0) nTotalWidth += nWidth - kCyBorder2;
            if ((bHorz ? ptMidY : ptMidX) < nTotalWidth) {
                if (nPos == 0) Bars(pThis).InsertAt(nPosInsAfter + 1, nullptr, 1);
                Bars(pThis).InsertAt(nPosInsAfter + 1, pBarIns, 1);
                return nPosInsAfter + 1;
            }
            nWidth = 0;
            nPosInsAfter = nPos;
        }
    }
    Bars(pThis).InsertAt(nPosInsAfter + 1, nullptr, 1);
    Bars(pThis).InsertAt(nPosInsAfter + 1, pBarIns, 1);
    return nPosInsAfter + 1;
}

// Retail: slot 104 of the CDockBar vftable (0x321dc8, mfc140u) holds 0x3a60 =
// `mov $1,%eax; ret`, and the mfc140u export table resolves
// ?IsDockBar@CDockBar@@UEBAHXZ to that same ICF-folded 0x3a60.  This body is
// the complete retail behaviour.
// Symbol: ?IsDockBar@CDockBar@@UEBAHXZ
extern "C" int MS_ABI impl__IsDockBar_CDockBar__UEBAHXZ(const void* /*pThis*/) {
    return TRUE;
}

// Transcribed from retail RVA 0x1d9310 (mfc140u; WM_ERASEBKGND (0x14) entry of
// the class message map at 0x3221d0):
//     if (pDC == NULL) AfxThrowInvalidArgException();                      // 0x227720
//     CRect rect = {0};  ::GetClientRect(m_hWnd, &rect);                   // IAT GetClientRect
//     if (m_hReBarTheme != NULL) {                                          // +0x118
//         if (::IsThemeBackgroundPartiallyTransparent(m_hReBarTheme, RP_BACKGROUND /*6*/, 0))   // IAT
//             ::DrawThemeParentBackground(m_hWnd, pDC->m_hDC, &rect);       // IAT; +0x8
//         CRect rectWorkArea = {0};  ::SetRectEmpty(&rectWorkArea);        // IAT SetRectEmpty
//         ::SystemParametersInfo(SPI_GETWORKAREA /*0x30*/, 0, &rectWorkArea, 0);   // IAT SystemParametersInfoW
//         rect.right  = max(rect.right + 1, rectWorkArea.right);
//         rect.top   -= 1;
//         rect.bottom = max(rect.bottom + 1, rectWorkArea.bottom);
//         if (SUCCEEDED(::DrawThemeBackground(m_hReBarTheme, pDC->m_hDC, RP_BACKGROUND, 0, &rect, NULL)))   // IAT
//             return TRUE;
//     }
//     return (BOOL)Default();                                               // 0x28ac80
// Symbol: ?OnEraseBkgnd@CDockBar@@IEAAHPEAVCDC@@@Z
extern "C" int MS_ABI impl__OnEraseBkgnd_CDockBar__IEAAHPEAVCDC___Z(void* pThis, CDC* pDC) {
    if (pThis == nullptr) return FALSE;   // deviation: retail has no NULL check on this
    if (pDC == nullptr) {
        impl__AfxThrowInvalidArgException__YAXXZ();
        return FALSE;
    }
    CControlBar* pSelf = AsBar(pThis);
    RECT rect = {};
    ::GetClientRect(pSelf->m_hWnd, &rect);
    HTHEME hTheme = static_cast<HTHEME>(pSelf->m_hReBarTheme);
    if (hTheme != nullptr) {
        if (::IsThemeBackgroundPartiallyTransparent(hTheme, kRP_BACKGROUND, 0)) {
            ::DrawThemeParentBackground(pSelf->m_hWnd, pDC->m_hDC, &rect);
        }
        RECT rectWorkArea = {};
        ::SetRectEmpty(&rectWorkArea);
        ::SystemParametersInfoW(SPI_GETWORKAREA, 0, &rectWorkArea, 0);
        rect.right = (rectWorkArea.right > rect.right + 1) ? rectWorkArea.right : rect.right + 1;
        rect.top -= 1;
        rect.bottom = (rectWorkArea.bottom > rect.bottom + 1) ? rectWorkArea.bottom : rect.bottom + 1;
        if (SUCCEEDED(::DrawThemeBackground(hTheme, pDC->m_hDC, kRP_BACKGROUND, 0, &rect, nullptr))) {
            return TRUE;
        }
    }
    return static_cast<int>(impl__Default_CWnd__IEAA_JXZ(pSelf));
}

// Transcribed from retail RVA 0x1d9280 (mfc140u; WM_NCCALCSIZE (0x83) entry of
// the class message map at 0x322150):
//     CRect rect = {0};  ::SetRectEmpty(&rect);                            // IAT SetRectEmpty
//     CalcInsideRect(rect, m_dwStyle & CBRS_ORIENT_HORZ);                  // vslot 94; +0x124 & 0xa000 passed as-is
//     for (int i = 0; i < 4; i++) ((LONG*)&lpncsp->rgrc[0])[i] += ((LONG*)&rect)[i];
// bCalcValidRects is not read.  vslot 94 goes to the CControlBar.cpp thunk.
// Symbol: ?OnNcCalcSize@CDockBar@@IEAAXHPEAUtagNCCALCSIZE_PARAMS@@@Z
extern "C" void MS_ABI impl__OnNcCalcSize_CDockBar__IEAAXHPEAUtagNCCALCSIZE_PARAMS___Z(void* pThis, int bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp) {
    (void)bCalcValidRects;
    if (pThis == nullptr || lpncsp == nullptr) return;   // deviation: retail has no NULL checks
    CRect rect;
    ::SetRectEmpty(static_cast<RECT*>(rect));
    impl__CalcInsideRect_CControlBar__UEBAXAEAVCRect__H_Z(AsBar(pThis), rect,
        static_cast<int>(AsBar(pThis)->m_dwStyle & kCBRS_ORIENT_HORZ));   // vslot 94
    lpncsp->rgrc[0].left += rect.left;
    lpncsp->rgrc[0].top += rect.top;
    lpncsp->rgrc[0].right += rect.right;
    lpncsp->rgrc[0].bottom += rect.bottom;
}

// Retail RVA 0x1d9430 (mfc140u; WM_NCPAINT (0x85) entry of the class message
// map at 0x322170) is a single `jmp 0x1d6e50` = CControlBar::EraseNonClient(),
// which goes to the CControlBar.cpp thunk.
// Symbol: ?OnNcPaint@CDockBar@@IEAAXXZ
extern "C" void MS_ABI impl__OnNcPaint_CDockBar__IEAAXXZ(void* pThis) {
    if (pThis == nullptr) return;   // deviation: retail has no NULL check
    impl__EraseNonClient_CControlBar__QEAAXXZ(AsBar(pThis));
}

// Transcribed from retail RVA 0x1d9440 (mfc140u; WM_PAINT (0xf) entry of the
// class message map at 0x3221b0):
//     CPaintDC dc(this);                                                   // 0x2a3d20
//     if (IsVisible() && GetDockedVisibleCount() != 0)                     // vslot 102, vslot 109
//         DoPaint(&dc);                                                    // vslot 95
//     ~CPaintDC                                                            // 0x2a3dd0
// vslot 102 goes to the CControlBar.cpp thunk, 109 and 95 to this file's.
// CPaintDC exists in this DLL only as its thunks, so it is built in raw
// storage the way CControlBar.cpp does.
// Symbol: ?OnPaint@CDockBar@@IEAAXXZ
extern "C" void MS_ABI impl__OnPaint_CDockBar__IEAAXXZ(void* pThis) {
    if (pThis == nullptr) return;   // deviation: retail has no NULL check
    alignas(void*) unsigned char dcStorage[sizeof(CPaintDC)] = {};
    CPaintDC* pDC = reinterpret_cast<CPaintDC*>(dcStorage);
    impl___0CPaintDC__QEAA_PEAVCWnd___Z(pDC, AsWnd(pThis));
    if (impl__IsVisible_CControlBar__UEBAHXZ(AsBar(pThis)) &&                 // vslot 102
        impl__GetDockedVisibleCount_CDockBar__UEBAHXZ(pThis) != 0) {           // vslot 109
        impl__DoPaint_CDockBar__UEAAXPEAVCDC___Z(pThis, pDC);                  // vslot 95
    }
    impl___1CPaintDC__UEAA_XZ(pDC);
}

// Transcribed from retail RVA 0x1d9210 (mfc140u; WM_SIZEPARENT (0x361) entry
// of the class message map at 0x3221f0):
//     AFX_SIZEPARENTPARAMS* lpLayout = (AFX_SIZEPARENTPARAMS*)lParam;
//     BOOL bSaveQuery = m_bLayoutQuery;  CRect rectSave = m_rectLayout;   // +0x178, +0x17c
//     m_bLayoutQuery = (lpLayout->hDWP == NULL);                          // +0
//     ::CopyRect(&m_rectLayout, &lpLayout->rect);                          // IAT CopyRect; +8
//     LRESULT lResult = CControlBar::OnSizeParent(wParam, lParam);         // 0x1d72c0
//     m_rectLayout = rectSave;  m_bLayoutQuery = bSaveQuery;
//     return lResult;
// Symbol: ?OnSizeParent@CDockBar@@IEAA_J_K_J@Z
extern "C" __int64 MS_ABI impl__OnSizeParent_CDockBar__IEAA_J_K_J_Z(void* pThis, unsigned __int64 wParam, __int64 lParam) {
    if (pThis == nullptr || lParam == 0) return 0;   // deviation: retail has no NULL checks
    DockBarTail& t = Tail(pThis);
    SizeParentParams* lpLayout = reinterpret_cast<SizeParentParams*>(lParam);
    const int bSaveQuery = t.m_bLayoutQuery;
    const RECT rectSave = t.m_rectLayout;
    t.m_bLayoutQuery = (lpLayout->hDWP == nullptr) ? TRUE : FALSE;
    ::CopyRect(&t.m_rectLayout, &lpLayout->rect);
    const __int64 lResult = impl__OnSizeParent_CControlBar__QEAA_J_K_J_Z(AsBar(pThis), wParam, lParam);
    t.m_rectLayout = rectSave;
    t.m_bLayoutQuery = bSaveQuery;
    return lResult;
}

// Retail: slot 93 of the CDockBar vftable (0x321dc8, mfc140u) holds 0x27d0 =
// `ret`, and the mfc140u export table resolves
// ?OnUpdateCmdUI@CDockBar@@MEAAXPEAVCFrameWnd@@H@Z to that same ICF-folded
// 0x27d0: a dock bar has no UI of its own to update.  This empty body is the
// complete retail behaviour, not a placeholder.
// Symbol: ?OnUpdateCmdUI@CDockBar@@MEAAXPEAVCFrameWnd@@H@Z
extern "C" void MS_ABI impl__OnUpdateCmdUI_CDockBar__MEAAXPEAVCFrameWnd__H_Z(void* /*pThis*/, CFrameWnd* /*pTarget*/, int /*bDisableIfNoHndler*/) {
}

// Transcribed from retail RVA 0x1d94e0 (mfc140u; WM_WINDOWPOSCHANGING (0x46)
// entry of the class message map at 0x322190):
//     DWORD dwStyle = m_dwStyle;                                           // +0x124
//     m_dwStyle &= ~CBRS_BORDER_ANY;                                       // & 0xfffff0ff
//     CControlBar::OnWindowPosChanging(lpWndPos);                          // 0x1d69c0
//     m_dwStyle = dwStyle;
// Symbol: ?OnWindowPosChanging@CDockBar@@IEAAXPEAUtagWINDOWPOS@@@Z
extern "C" void MS_ABI impl__OnWindowPosChanging_CDockBar__IEAAXPEAUtagWINDOWPOS___Z(void* pThis, WINDOWPOS* lpWndPos) {
    if (pThis == nullptr) return;   // deviation: retail has no NULL check
    CControlBar* pSelf = AsBar(pThis);
    const DWORD dwStyle = pSelf->m_dwStyle;
    pSelf->m_dwStyle &= ~kCBRS_BORDER_ANY;
    impl__OnWindowPosChanging_CControlBar__QEAAXPEAUtagWINDOWPOS___Z(pSelf, lpWndPos);
    pSelf->m_dwStyle = dwStyle;
}

// Transcribed from retail RVA 0x1d8680 (mfc140u):
//     if (this == NULL || pBar == NULL) AfxThrowInvalidArgException();   // 0x227720
//     CRect rectBar = {0};  ::GetWindowRect(pBar->m_hWnd, &rectBar);      // IAT GetWindowRect
//     if (pBar->m_pDockBar == this && (lpRect == NULL || ::EqualRect(&rectBar, lpRect)))   // +0x138; IAT EqualRect
//         return;
//     <style adoption + title + SetBarStyle -- see AdoptBarStyle above>
//     int nPos = FindBar((CControlBar*)(UINT_PTR)::GetDlgCtrlID(pBar->m_hWnd), -1);   // IAT GetDlgCtrlID; 0x1d9520
//     if (nPos > 0) m_arrBars[nPos] = pBar;                                // bounds-checked store
//     if (lpRect != NULL) {
//         CRect rect;  ::CopyRect(&rect, lpRect);  ScreenToClient(&rect);   // IAT CopyRect; 0x2a32b0
//         if (nPos < 1) {
//             CPoint ptMid(rect.left + rect.Width()/2, rect.top + rect.Height()/2);
//             Insert(pBar, rect, ptMid);                                    // 0x1d9630 (result discarded)
//         }
//         pBar->SetWindowPos(NULL, rect.left, rect.top, rect.Width(), rect.Height(), 0x114);   // 0x2a9a60
//     } else {
//         if (nPos < 1) { m_arrBars.Add(pBar);  m_arrBars.Add(NULL); }     // SetAtGrow 0x1d4910, twice
//         pBar->SetWindowPos(NULL, -afxData.cxBorder2, -afxData.cyBorder2, 0, 0, 0x115);
//     }
//     <re-parent to this -- see ReparentToThis above>
//     if (pBar->m_pDockBar != NULL) pBar->m_pDockBar->RemoveControlBar(pBar, -1, 0);   // 0x1d8a10
//     pBar->m_pDockBar = this;
//     GetDockingFrame()->DelayRecalcLayout();                               // 0x28e200 / +0x130; orl $0xc,0x1d0
// Unlike DockControlBar there is no hide/show around the move and no
// RemovePlaceHolder: the place holder found by FindBar is overwritten in
// place.  DEVIATION: a NULL docking frame is skipped (retail would fault).
// Symbol: ?ReDockControlBar@CDockBar@@QEAAXPEAVCControlBar@@PEBUtagRECT@@@Z
extern "C" void MS_ABI impl__ReDockControlBar_CDockBar__QEAAXPEAVCControlBar__PEBUtagRECT___Z(void* pThis, CControlBar* pBar, const RECT* lpRect) {
    if (pThis == nullptr || pBar == nullptr) {
        impl__AfxThrowInvalidArgException__YAXXZ();
        return;
    }
    DockBarTail& t = Tail(pThis);
    RECT rectBar = {};
    ::GetWindowRect(pBar->m_hWnd, &rectBar);
    if (pBar->m_pDockBar == pThis && (lpRect == nullptr || ::EqualRect(&rectBar, lpRect))) {
        return;
    }

    AdoptBarStyle(pThis, pBar);

    const UINT_PTR nID = static_cast<unsigned int>(::GetDlgCtrlID(pBar->m_hWnd));
    const int nPos = impl__FindBar_CDockBar__QEAAHPEAVCControlBar__H_Z(pThis, reinterpret_cast<CControlBar*>(nID), -1);
    if (nPos > 0) {
        if (nPos >= t.m_arrBars_nSize) {
            impl__AfxThrowInvalidArgException__YAXXZ();
            return;
        }
        t.m_arrBars_pData[nPos] = pBar;
    }

    if (lpRect != nullptr) {
        RECT rect;
        ::CopyRect(&rect, lpRect);
        impl__ScreenToClient_CWnd__QEBAXPEAUtagRECT___Z(AsWnd(pThis), &rect);
        if (nPos < 1) {
            const int xMid = rect.left + (rect.right - rect.left) / 2;
            const int yMid = rect.top + (rect.bottom - rect.top) / 2;
            const long long ptMid = (static_cast<long long>(static_cast<unsigned int>(yMid)) << 32) | static_cast<unsigned int>(xMid);
            impl__Insert_CDockBar__IEAAHPEAVCControlBar__VCRect__VCPoint___Z(pThis, pBar, &rect, ptMid);
        }
        impl__SetWindowPos_CWnd__QEAAHPEBV1_HHHHI_Z(pBar, nullptr, rect.left, rect.top,
            rect.right - rect.left, rect.bottom - rect.top, SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOCOPYBITS);
    } else {
        if (nPos < 1) {
            Bars(pThis).SetAtGrow(t.m_arrBars_nSize, pBar);
            Bars(pThis).SetAtGrow(t.m_arrBars_nSize, nullptr);
        }
        impl__SetWindowPos_CWnd__QEAAHPEBV1_HHHHI_Z(pBar, nullptr, -kCxBorder2, -kCyBorder2, 0, 0,
            SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOCOPYBITS);
    }

    ReparentToThis(pThis, pBar);
    if (pBar->m_pDockBar != nullptr) {
        impl__RemoveControlBar_CDockBar__QEAAHPEAVCControlBar__HH_Z(pBar->m_pDockBar, pBar, -1, 0);
    }
    pBar->m_pDockBar = pThis;

    CFrameWnd* pFrame = DockingFrame(pThis);
    if (pFrame != nullptr) DelayRecalcLayout(pFrame);
}

// Transcribed from retail RVA 0x1d8a10 (mfc140u):
//     if (this == NULL || pBar == NULL) AfxThrowInvalidArgException();   // 0x227720
//     int nPos = FindBar(pBar, nPosExclude);                               // 0x1d9520
//     if (nPos <= 0) AfxThrowInvalidArgException();                        // ENSURE(nPos > 0)
//     if (nAddPlaceHolder == 1) {
//         m_arrBars[nPos] = (void*)(UINT_PTR)::GetDlgCtrlID(pBar->m_hWnd);   // IAT GetDlgCtrlID; bounds-checked store
//         if (FindBar((CControlBar*)m_arrBars[nPos], nPos) > 0) {          // a place holder for this ID already exists
//             m_arrBars.RemoveAt(nPos, 1);                                  // 0x1d4a60
//             if (m_arrBars[nPos-1] == NULL && m_arrBars[nPos] == NULL) m_arrBars.RemoveAt(nPos, 1);
//         }
//     } else {
//         m_arrBars.RemoveAt(nPos, 1);
//         if (m_arrBars[nPos-1] == NULL && m_arrBars[nPos] == NULL) m_arrBars.RemoveAt(nPos, 1);
//         if (nAddPlaceHolder != -1) RemovePlaceHolder(pBar);              // 0x1d8960
//     }
//     if (pBar->m_pDockContext == NULL) return FALSE;                      // +0x140: shutdown case
//     CFrameWnd* pFrame = GetDockingFrame();                               // 0x28e200 / +0x130
//     if (m_bFloating && GetDockedVisibleCount() == 0) {                   // +0x148; vslot 109
//         if (GetDockedCount() == 0) {                                     // 0x1d8260
//             pFrame->DestroyWindow();                                     // CWnd vslot 26
//             return TRUE;
//         }
//         pFrame->ShowWindow(SW_HIDE);                                     // 0x2a9ad0
//         return FALSE;
//     }
//     pFrame->DelayRecalcLayout();                                         // orl $0xc,0x1d0
//     return FALSE;
// vslot 109 goes to this file's thunk; the frame's vslot 26 is devirtualised
// to the CWnd DestroyWindow thunk (no CFrameWnd / CMiniFrameWnd override is
// exported; a client-derived override is not honoured).  DEVIATION: a NULL
// docking frame is skipped (retail would fault).
// Symbol: ?RemoveControlBar@CDockBar@@QEAAHPEAVCControlBar@@HH@Z
extern "C" int MS_ABI impl__RemoveControlBar_CDockBar__QEAAHPEAVCControlBar__HH_Z(void* pThis, CControlBar* pBar, int nPosExclude, int nAddPlaceHolder) {
    if (pThis == nullptr || pBar == nullptr) {
        impl__AfxThrowInvalidArgException__YAXXZ();
        return FALSE;
    }
    DockBarTail& t = Tail(pThis);
    const int nPos = impl__FindBar_CDockBar__QEAAHPEAVCControlBar__H_Z(pThis, pBar, nPosExclude);
    if (nPos <= 0) {
        impl__AfxThrowInvalidArgException__YAXXZ();
        return FALSE;
    }
    if (nAddPlaceHolder == 1) {
        const UINT_PTR nID = static_cast<unsigned int>(::GetDlgCtrlID(pBar->m_hWnd));
        if (nPos >= t.m_arrBars_nSize) {
            impl__AfxThrowInvalidArgException__YAXXZ();
            return FALSE;
        }
        t.m_arrBars_pData[nPos] = reinterpret_cast<void*>(nID);
        if (impl__FindBar_CDockBar__QEAAHPEAVCControlBar__H_Z(pThis, static_cast<CControlBar*>(BarAt(pThis, nPos)), nPos) > 0) {
            RemoveAtAndCollapseRow(pThis, nPos);
        }
    } else {
        RemoveAtAndCollapseRow(pThis, nPos);
        if (nAddPlaceHolder != -1) {
            impl__RemovePlaceHolder_CDockBar__QEAAXPEAVCControlBar___Z(pThis, pBar);
        }
    }

    if (pBar->m_pDockContext == nullptr) return FALSE;

    CFrameWnd* pFrame = DockingFrame(pThis);
    if (pFrame == nullptr) return FALSE;   // deviation: retail would fault here
    if (t.m_bFloating && impl__GetDockedVisibleCount_CDockBar__UEBAHXZ(pThis) == 0) {   // vslot 109
        if (impl__GetDockedCount_CDockBar__QEBAHXZ(pThis) == 0) {
            impl__DestroyWindow_CWnd__UEAAHXZ(pFrame);   // CWnd vslot 26
            return TRUE;
        }
        impl__ShowWindow_CWnd__QEAAHH_Z(pFrame, SW_HIDE);
        return FALSE;
    }
    DelayRecalcLayout(pFrame);
    return FALSE;
}

// Transcribed from retail RVA 0x1d8960 (mfc140u):
//     if ((UINT_PTR)pBar > 0xffff)                                          // a real bar: use its ID
//         pBar = (CControlBar*)(UINT_PTR)::GetDlgCtrlID(pBar->m_hWnd);      // IAT GetDlgCtrlID
//     int nPos = FindBar(pBar, -1);                                         // 0x1d9520
//     if (nPos > 0) {
//         m_arrBars.RemoveAt(nPos, 1);                                      // 0x1d4a60
//         if (m_arrBars[nPos-1] == NULL && m_arrBars[nPos] == NULL)         // bounds-checked reads
//             m_arrBars.RemoveAt(nPos, 1);
//     }
// Symbol: ?RemovePlaceHolder@CDockBar@@QEAAXPEAVCControlBar@@@Z
extern "C" void MS_ABI impl__RemovePlaceHolder_CDockBar__QEAAXPEAVCControlBar___Z(void* pThis, CControlBar* pBar) {
    if (pThis == nullptr) return;   // deviation: retail has no NULL check
    void* pKey = pBar;
    if (!IsPlaceHolder(pKey)) {
        pKey = reinterpret_cast<void*>(static_cast<UINT_PTR>(static_cast<unsigned int>(::GetDlgCtrlID(pBar->m_hWnd))));
    }
    const int nPos = impl__FindBar_CDockBar__QEAAHPEAVCControlBar__H_Z(pThis, static_cast<CControlBar*>(pKey), -1);
    if (nPos > 0) {
        RemoveAtAndCollapseRow(pThis, nPos);
    }
}

// Transcribed from retail RVA 0x221cf0 (mfc140u):
//     int nSize = (int)pInfo->m_arrBarID.m_nSize;                          // +0x58
//     while (nSize > 0 && (pInfo->m_arrBarID[nSize-1] & ~0x10000) == 0)    // bounds-checked; +0x50
//         nSize--;                                                          // drop trailing empty entries
//     for (int i = 1; i < nSize; i++) {                                     // element 0 is the leading NULL
//         UINT nID = pInfo->m_arrBarID[i];                                  // bounds-checked
//         void* pElem;
//         if (nID <= 0xffff) {
//             CControlBar* pBar = pFrameWnd->GetControlBar(nID);            // 0x29f7d0
//             pElem = pBar;
//             if (pBar != NULL) {
//                 <re-parent to this -- see ReparentToThis above>
//                 if (pBar->m_pDockBar != NULL) pBar->m_pDockBar->RemoveControlBar(pBar, -1, -1);   // 0x1d8a10
//                 RemovePlaceHolder(pBar);                                  // 0x1d8960
//                 pBar->m_pDockBar = this;
//                 pBar->SetBarStyle(<AdoptedBarStyle(pBar)>);               // 0x1d6020
//                 if (m_bFloating) {                                        // +0x148
//                     if (pBar->m_dwDockStyle & CBRS_FLOAT_MULTI) m_dwStyle |= CBRS_FLOAT_MULTI;   // +0x128 / +0x124
//                     CFrameWnd* pFrame = pBar->GetParentFrame();           // 0x28e200 on pBar (rcx = pBar at 0x221ed5), not on this
//                     if (pFrame == NULL) AfxThrowInvalidArgException();
//                     if (pFrame->m_hWndOwner == NULL) pFrame->m_hWndOwner = pBar->m_hWnd;   // +0xa0
//                     if (pBar->m_dwStyle & CBRS_SIZE_DYNAMIC) pFrame->ModifyStyle(MFS_MOVEFRAME /*0x800*/, 0, 0);   // 0x2a96f0
//                 }
//                 if (i == 1 && !(m_dwStyle & CBRS_FLOAT_MULTI)) {
//                     CString strTitle;  pBar->GetWindowText(strTitle);     // 0x28be00
//                     AfxSetWindowText(m_hWnd, strTitle);                   // 0x2ae4b0
//                 }
//             }
//         } else {
//             pElem = (void*)(UINT_PTR)(WORD)nID;                           // place holder
//             RemovePlaceHolder((CControlBar*)pElem);                       // 0x1d8960
//         }
//         m_arrBars.InsertAt(i, pElem, 1);                                  // 0x1d4960 (NULL when GetControlBar failed)
//     }
//     int nCount = (int)m_arrBars.m_nSize;
//     if (nSize < nCount && m_arrBars[nSize] != NULL) { m_arrBars.InsertAt(nSize, NULL, 1); nCount++; }
//     if (m_arrBars[nCount-1] != NULL) m_arrBars.InsertAt(nCount, NULL, 1);  // bounds-checked read
// The title is fetched through the CString overload of GetWindowText in
// retail (0x28be00: GetWindowTextLength + GetWindowText into
// GetBufferSetLength, unless the bar hosts an OLE control site at CWnd+0xd0,
// in which case the text comes from the site -- not modelled here); here it
// is read with GetWindowTextLength + GetWindowText into a heap buffer of that
// length (same text for an ordinary window, no CString).  DEVIATION: a NULL
// this / pInfo / pFrameWnd returns (retail has no such checks).
// Symbol: ?SetBarInfo@CDockBar@@QEAAXPEAVCControlBarInfo@@PEAVCFrameWnd@@@Z
extern "C" void MS_ABI impl__SetBarInfo_CDockBar__QEAAXPEAVCControlBarInfo__PEAVCFrameWnd___Z(void* pThis, void* pInfoRaw, CFrameWnd* pFrameWnd) {
    if (pThis == nullptr || pInfoRaw == nullptr || pFrameWnd == nullptr) return;
    S_Ccontrolbarinfo* pInfo = static_cast<S_Ccontrolbarinfo*>(pInfoRaw);
    DockBarTail& t = Tail(pThis);
    CControlBar* pSelf = AsBar(pThis);
    const unsigned int* pIDs = static_cast<const unsigned int*>(pInfo->m_arrBarID_pData);

    int nSize = static_cast<int>(pInfo->m_arrBarID_nSize);
    while (nSize > 0) {
        if (nSize - 1 >= pInfo->m_arrBarID_nSize) {
            impl__AfxThrowInvalidArgException__YAXXZ();
            return;
        }
        if (pIDs[nSize - 1] & 0xfffeffffu) break;
        nSize--;
    }

    for (int i = 1; i < nSize; i++) {
        if (i >= pInfo->m_arrBarID_nSize) {
            impl__AfxThrowInvalidArgException__YAXXZ();
            return;
        }
        const unsigned int nID = pIDs[i];
        void* pElem;
        if (nID <= 0xffff) {
            CControlBar* pBar = impl__GetControlBar_CFrameWnd__QEAAPEAVCControlBar__I_Z(pFrameWnd, nID);
            pElem = pBar;
            if (pBar != nullptr) {
                ReparentToThis(pThis, pBar);
                if (pBar->m_pDockBar != nullptr) {
                    impl__RemoveControlBar_CDockBar__QEAAHPEAVCControlBar__HH_Z(pBar->m_pDockBar, pBar, -1, -1);
                }
                impl__RemovePlaceHolder_CDockBar__QEAAXPEAVCControlBar___Z(pThis, pBar);
                pBar->m_pDockBar = pThis;
                impl__SetBarStyle_CControlBar__QEAAXK_Z(pBar, AdoptedBarStyle(pThis, pBar));
                if (t.m_bFloating) {
                    if (pBar->m_dwDockStyle & kCBRS_FLOAT_MULTI) {
                        pSelf->m_dwStyle |= kCBRS_FLOAT_MULTI;
                    }
                    CFrameWnd* pFrame = impl__GetParentFrame_CWnd__QEBAPEAVCFrameWnd__XZ(pBar);   // on the bar, as retail
                    if (pFrame == nullptr) {
                        impl__AfxThrowInvalidArgException__YAXXZ();
                        return;
                    }
                    if (HWndOwnerSlot(pFrame) == nullptr) {
                        HWndOwnerSlot(pFrame) = pBar->m_hWnd;
                    }
                    if (pBar->m_dwStyle & kCBRS_SIZE_DYNAMIC) {
                        impl__ModifyStyle_CWnd__QEAAHKKI_Z(pFrame, kMFS_MOVEFRAME, 0, 0);
                    }
                }
                if (i == 1 && !(pSelf->m_dwStyle & kCBRS_FLOAT_MULTI)) {
                    const int nLen = pBar->m_hWnd ? ::GetWindowTextLengthW(pBar->m_hWnd) : 0;
                    wchar_t* pszTitle = static_cast<wchar_t*>(std::malloc(static_cast<size_t>(nLen + 1) * sizeof(wchar_t)));
                    if (pszTitle == nullptr) {
                        impl__AfxThrowMemoryException__YAXXZ();
                        return;
                    }
                    pszTitle[0] = L'\0';
                    if (nLen > 0) ::GetWindowTextW(pBar->m_hWnd, pszTitle, nLen + 1);
                    impl__AfxSetWindowText__YAXPEAUHWND____PEB_W_Z(pSelf->m_hWnd, pszTitle);
                    std::free(pszTitle);
                }
            }
        } else {
            pElem = reinterpret_cast<void*>(static_cast<UINT_PTR>(nID & 0xffffu));
            impl__RemovePlaceHolder_CDockBar__QEAAXPEAVCControlBar___Z(pThis, static_cast<CControlBar*>(pElem));
        }
        Bars(pThis).InsertAt(i, pElem, 1);
    }

    int nCount = static_cast<int>(t.m_arrBars_nSize);
    if (nSize < nCount && BarAt(pThis, nSize) != nullptr) {
        Bars(pThis).InsertAt(nSize, nullptr, 1);
        nCount++;
    }
    if (BarAt(pThis, nCount - 1) != nullptr) {
        Bars(pThis).InsertAt(nCount, nullptr, 1);
    }
}

// Transcribed from retail RVA 0x1d9580 (mfc140u):
//     for (int i = 0; i < m_arrBars.m_nSize; i++) {
//         CControlBar* pBar = GetDockedControlBar(i);                   // 0x1d95f0
//         if (pBar != NULL) {
//             CFrameWnd* pFrame = pBar->GetParentFrame();               // 0x28e200
//             if (pFrame == NULL) pFrame = pBar->m_pDockSite;           // +0x130 of pBar
//             pFrame->ShowControlBar(pBar, bShow, TRUE);                // 0x29ee00
//         }
//     }
// DEVIATION: a NULL frame is skipped (retail would fault).
// Symbol: ?ShowAll@CDockBar@@QEAAXH@Z
extern "C" void MS_ABI impl__ShowAll_CDockBar__QEAAXH_Z(void* pThis, int bShow) {
    if (pThis == nullptr) return;   // deviation: retail has no NULL check
    for (int i = 0; i < Tail(pThis).m_arrBars_nSize; i++) {
        CControlBar* pBar = impl__GetDockedControlBar_CDockBar__IEBAPEAVCControlBar__H_Z(pThis, i);
        if (pBar == nullptr) continue;
        CFrameWnd* pFrame = impl__GetParentFrame_CWnd__QEBAPEAVCFrameWnd__XZ(pBar);
        if (pFrame == nullptr) pFrame = static_cast<CFrameWnd*>(pBar->m_pDockSite);
        if (pFrame == nullptr) continue;
        impl__ShowControlBar_CFrameWnd__QEAAXPEAVCControlBar__HH_Z(pFrame, pBar, bShow, TRUE);
    }
}
