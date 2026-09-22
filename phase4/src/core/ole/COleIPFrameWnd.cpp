// COleIPFrameWnd — OpenMFC implementation.
// Sources: olecore.cpp
//
// Every body below marked "retail" was transcribed from the retail
// disassembly, the method phase4/src/core/ole/COleControl.cpp describes.
//
// WHICH IMAGE.  Every RVA in this file is an mfc140u.dll (Unicode) RVA unless
// it says otherwise.  Most of this class's exports are missing from
// mfc140u_rva_symbols.json, so entries were resolved by ordinal through the
// mfc140u export table (mfc_complete_ordinal_mapping.json) and disassembled
// with `disas.py --u --at`.  The fourteen of them that mfc140.dll's (ANSI)
// symbol map also names (BuildSharedMenu, CreateObject, DestroySharedMenu,
// LoadFrame, OnBarCheck, OnContextHelp, OnCreate, OnDestroy, OnRecalcParent,
// OnResizeChild, OnWindowPosChanging, PreTranslateMessage, RecalcLayout,
// RepositionFrame) were diffed against their ANSI twins and are
// instruction-for-instruction identical once addresses are masked.
// OnSetMessageString is the one that differs: its status-text temporary is a
// CStringW built from a char* in mfc140.dll and from a wchar_t* in mfc140u.dll
// (0xdcb0, ??0?$CStringT@_W...@QEAA@PEA_W@Z).
//
// Entry RVAs (mfc140u):
//   ??0 ctor 0x25d010   ??1 dtor 0x25d0d0   OnCreate 0x25d160   OnDestroy 0x25d1c0
//   OnCreateControlBars(CFrameWnd*,CFrameWnd*) 0x25d240   OnIdleUpdateCmdUI 0x25d260
//   OnWindowPosChanging 0x25d2a0   OnSetMessageString 0x25d300   LoadFrame 0x25d420
//   OnResizeChild 0x25d490   OnRecalcParent 0x25d5b0   RecalcLayout 0x25d600
//   RepositionFrame 0x25d800   PreTranslateMessage 0x25d870
//   OnUpdateControlBarMenu 0x25d940   OnBarCheck 0x25d9c0   OnContextHelp 0x25da40
//   GetInPlaceMenu 0x25db60   BuildSharedMenu 0x25db90   CreateObject 0x25dc90
//   DestroySharedMenu 0x256c00 -- COMDAT-folded with COleDocIPFrameWnd's
//       identical body, which is why the symbol map names that RVA
//       ?DestroySharedMenu@COleDocIPFrameWnd@@; the COleIPFrameWnd vftable
//       (slot 119) points at the same address.
//   OnCreateControlBars(CWnd*,CWnd*) 0x3a60 and OnSize 0x27d0 -- both folded
//       into image-wide shared bodies (`mov $1,%eax ; ret` and `ret 0`), which
//       is why the symbol maps attach unrelated names to those RVAs.
//
// VFTABLE.  The class vftable is at mfc140u RVA 0x32fb58 (the ctor loads it
// at 0x25d027 and stores it at 0x25d02e).  Slots this file dispatches
// through, each read out of that table (vtdump_u.py) and matching the byte
// offset at the retail call site:
//   28 +0x0e0 CWnd::CalcWindowRect        93 +0x2e8 CFrameWnd::GetActiveDocument
//   95 +0x2f8 CFrameWnd::GetMessageString 96 +0x300 RecalcLayout (0x25d600)
//  109 +0x368 GetDefaultAccelerator      116 +0x3a0 RepositionFrame (0x25d800)
//  117 +0x3a8 OnSetMessageString          120 +0x3c0 GetInPlaceMenu (0x25db60)
//  121 +0x3c8 OnRequestPositionChange (0x2568d0)
// plus, on the active document, slot 35 (+0x118) = OnCloseDocument: that slot
// holds ?OnCloseDocument@COleDocument@@ in the COleDocument vftable and
// ?OnCloseDocument@COleServerDoc@@ in the COleServerDocEx one.  (Slot 69,
// +0x228, is this class's PreTranslateMessage; slot 115, +0x398, is
// OnCreateControlBars(CWnd*,CWnd*), which OnCreateControlBars(CFrameWnd*,...)
// at 0x25d240 tail-calls.)
//
// Virtual calls on `this` and on the document go through the object's own
// vftable by MSVC slot index, as retail does and as
// core/ole/COleCntrFrameWndEx.cpp already does for the in-place frame: the
// frames and documents that reach these bodies are client classes derived
// from COleIPFrameWnd / COleServerDoc, constructed by MSVC-compiled code and
// carrying MSVC-layout vftables.  CAVEAT: an object built by OpenMFC itself
// (the ctor thunk at core/ole/Thunks.cpp:1158, or CreateObject below, with no
// client derivation) keeps the g++ vftable of the C++ class in this file, and
// its slot indices are not retail's.
//
// MESSAGE MAP.  Retail's map (AFX_MSGMAP at mfc140u VA 0x1803300d0, entries at
// 0x18032ff30, base CFrameWnd) routes WM_SIZE -> 0x27d0 (OnSize),
// WM_RECALCPARENT -> OnRecalcParent, WM_IDLEUPDATECMDUI -> OnIdleUpdateCmdUI,
// WM_WINDOWPOSCHANGING -> OnWindowPosChanging, WM_CREATE -> OnCreate,
// WM_DESTROY -> OnDestroy, WM_SIZECHILD -> OnResizeChild, WM_SETMESSAGESTRING ->
// 0x25dcc0 (a vcall thunk: `jmp *0x3a8(vftable)`, i.e. virtual
// OnSetMessageString), and ID_VIEW_TOOLBAR / ID_VIEW_STATUS_BAR (0xe800,
// 0xe801) -> OnUpdateControlBarMenu / OnBarCheck.  OnContextHelp is NOT in
// the map.  OpenMFC's classCOleIPFrameWnd_msgmap
// (detail/Ole09MsgmapSupport.cpp:18) has no entries yet, so none of the
// handlers below is reached through OpenMFC's own message dispatch.
//
// RETAIL MEMBER LAYOUT.  Retail afxole.h:1426 (atlmfc 14.51) declares, after
// the 0x1d8-byte CFrameWnd body: BOOL m_bUIActive @0x1d8, then the members
// pinned by RetailIPFrameTail below (@0x1e0..0x280); sizeof == 0x280 (the
// `mov $0x280,%ecx` operator-new argument in CreateObject, and the 640 in
// core/ole/RuntimeClasses.cpp:241, which matches retail's descriptor).
// OpenMFC's afxole.h declares instead `COleResizeBar* m_pResizeBar` @0x1d8
// (NOT a retail member -- it overlays m_bUIActive plus its padding) followed
// by char _oleipframewnd_padding[160] @0x1e0.  The total size agrees (640), so
// every retail member from +0x1e0 on lives inside OpenMFC's zero-initialised
// padding and is addressed here at its retail offset.  Nothing below touches
// +0x1d8.
//
// CFrameWnd members at retail offsets.  OpenMFC's CFrameWnd declares only three
// non-static data members (m_pViewActive @0xe8, m_hAccelTable @0xf0,
// m_nIDHelp @0xf8 -- none of them at its retail offset); its
// _framewnd_padding runs from 0xfc to 0x1d4.  Retail's m_bHelpMode (+0x104), m_nIDHelp (+0x164), m_nIDTracking
// (+0x168) and m_nIDLastMessage (+0x16c) -- offsets established by the
// instructions cited at each use and agreeing with retail afxwin.h's
// declaration order -- all fall inside that padding and are accessed at the
// retail offsets, as core/controlbar/CDockBar.cpp does for m_nIdleFlags
// (+0x1d0).  A grep of core/frame, core/controlbar and core/ole finds no
// OpenMFC code reading those offsets; OpenMFC's own CFrameWnd keeps its own
// m_nIDHelp at +0xf8, which LoadFrame below does NOT write.

#define OPENMFC_APPCORE_IMPL

#include "detail/OlecoreSupport.h"

#include <cstddef>
#include <cstring>

// ---- sibling impl__ exports called by the bodies in this file ----
// (C++ methods of other classes exist in this DLL only as impl__ thunks.  The
// fidelity of each callee is its own file's business; where a callee is known
// to diverge from retail in a way that matters here, the call site says so.)
extern "C" void MS_ABI impl__Enable_CCmdUI__UEAAXH_Z(CCmdUI* pThis, int enable);
// core/window/CWnd.cpp
extern "C" int MS_ABI impl__Create_CWnd__UEAAHPEB_W0KAEBUtagRECT__PEAV1_IPEAUCCreateContext___Z(
    CWnd* pThis, const wchar_t* lpszClassName, const wchar_t* lpszWindowName,
    DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext);
extern "C" int MS_ABI impl__ModifyStyleEx_CWnd__QEAAHKKI_Z(
    CWnd* pThis, unsigned long dwRemove, unsigned long dwAdd, unsigned int nFlags);
extern "C" CWnd* MS_ABI impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(HWND hWnd);
extern "C" CWnd* MS_ABI impl__GetDlgItem_CWnd__QEBAPEAV1_H_Z(const CWnd* pThis, int nID);
extern "C" void MS_ABI impl__RepositionBars_CWnd__QEAAXIIIIPEAUtagRECT__PEBU2_H_Z(
    CWnd* pThis, unsigned int nIDFirst, unsigned int nIDLast, unsigned int nIDLeftOver,
    unsigned int nFlag, RECT* lpRectParam, const RECT* lpRectClient, int bStretch);
// core/window/Thunks.cpp
extern "C" void MS_ABI impl__ClientToScreen_CWnd__QEBAXPEAUtagRECT___Z(const CWnd* pThis, RECT* lpRect);
extern "C" void MS_ABI impl__ScreenToClient_CWnd__QEBAXPEAUtagRECT___Z(const CWnd* pThis, RECT* lpRect);
extern "C" __int64 MS_ABI impl__Default_CWnd__IEAA_JXZ(CWnd* pThis);
extern "C" CFrameWnd* MS_ABI impl__GetTopLevelFrame_CWnd__QEBAPEAVCFrameWnd__XZ(const CWnd* pThis);
// core/frame/Thunks.cpp
extern "C" int MS_ABI impl__LoadAccelTable_CFrameWnd__QEAAHPEB_W_Z(CFrameWnd* pThis, const wchar_t* lpszResourceName);
extern "C" CControlBar* MS_ABI impl__GetControlBar_CFrameWnd__QEAAPEAVCControlBar__I_Z(CFrameWnd* pThis, unsigned int nID);
extern "C" int MS_ABI impl__OnBarCheck_CFrameWnd__QEAAHI_Z(CFrameWnd* pThis, unsigned int nID);
extern "C" int MS_ABI impl__CanEnterHelpMode_CFrameWnd__QEAAHXZ(CFrameWnd* pThis);
extern "C" void MS_ABI impl__OnContextHelp_CFrameWnd__QEAAXXZ(CFrameWnd* pThis);
extern "C" int MS_ABI impl__OnCreateHelper_CFrameWnd__IEAAHPEAUtagCREATESTRUCTW__PEAUCCreateContext___Z(
    CFrameWnd* pThis, CREATESTRUCTW* lpcs, CCreateContext* pContext);
extern "C" void MS_ABI impl__DestroyDockBars_CFrameWnd__QEAAXXZ(CFrameWnd* pThis);
extern "C" void MS_ABI impl__OnDestroy_CFrameWnd__IEAAXXZ(CFrameWnd* pThis);
// core/frame/CFrameWnd.cpp
extern "C" int MS_ABI impl__PreTranslateMessage_CFrameWnd__UEAAHPEAUtagMSG___Z(CFrameWnd* pThis, MSG* pMsg);
// core/doc/CDocument.cpp
extern "C" void MS_ABI impl__DisconnectViews_CDocument__QEAAXXZ(CDocument* pThis);
// featurepack/CMFC_misc_stubs.cpp (the definitions there spell each HMENU
// parameter as `void* /*struct*/*`, i.e. void**, and AfxMergeMenus's result as
// void*; every one is a pointer-sized value, so the ABI is identical)
extern "C" HMENU MS_ABI impl__AfxMergeMenus__YAPEAUHMENU____PEAU1_0PEAJHH_Z(
    HMENU hMenuShared, HMENU hMenuSource, LONG* lpMenuWidths, int iWidthIndex, int bMergeHelpMenus);
extern "C" void MS_ABI impl__AfxUnmergeMenus__YAXPEAUHMENU____00_Z(
    HMENU hMenuShared, HMENU hMenuSource, HMENU hHelpMenuPopup);
extern "C" void MS_ABI impl__AfxRepositionWindow__YAXPEAUAFX_SIZEPARENTPARAMS__PEAUHWND____PEBUtagRECT___Z(
    void* lpLayout, HWND hWnd, const RECT* lpRect);
// detail/MfcExceptionsSupport.cpp
extern "C" void MS_ABI impl__AfxThrowInvalidArgException__YAXXZ();
// detail/MemcoreSupport.cpp (the DLL's exported MFC operator new, ??2@YAPEAX_K@Z)
extern "C" void* MS_ABI impl___2_YAPEAX_K_Z(std::size_t size);
// core/ole/Thunks.cpp (??0COleIPFrameWnd@@QEAA@XZ)
extern "C" void* MS_ABI impl___0COleIPFrameWnd__QEAA_XZ(void* pThis);

namespace {

// ---------------------------------------------------------------------------
// Retail COleIPFrameWnd members from +0x1e0, as declared in retail
// afxole.h:1468-1483.  Each offset is followed by the mfc140u instruction
// that establishes it.
// ---------------------------------------------------------------------------
constexpr size_t kOff_IPFrameTail = 0x1e0;
constexpr size_t kSizeof_COleIPFrameWnd = 0x280;   // CreateObject 0x25dc90: `mov $0x280,%ecx`

#pragma pack(push, 8)
struct RetailIPFrameTail {
    OLEINPLACEFRAMEINFO  m_frameInfo;      // +0x1e0  ctor memset(this+0x1e0,0,0x20) then
                                           //         `movl $0x20,(%rbx)` = cb (0x25d046..0x25d066);
                                           //         PreTranslateMessage copies 0x1e0..0x200
    LPOLEINPLACEFRAME    m_lpFrame;        // +0x200  InsertMenus/RemoveMenus/SetStatusText target
    LPOLEINPLACEUIWINDOW m_lpDocFrame;     // +0x208  OnContextHelp: ContextSensitiveHelp target
    CFrameWnd*           m_pMainFrame;     // +0x210  COleCntrFrameWnd* (OnBarCheck, OnDestroy)
    CFrameWnd*           m_pDocFrame;      // +0x218  COleCntrFrameWnd*
    HMENU                m_hSharedMenu;    // +0x220  BuildSharedMenu `mov %rax,0x220(%rbx)` 0x25dbbb
    OLEMENUGROUPWIDTHS   m_menuWidths;     // +0x228  `lea 0x228(%rbx),%rsi` 0x25dbd0; 0x18-byte memset
    HOLEMENU             m_hOleMenu;       // +0x240  `mov %rax,0x240(%rbx)` 0x25dc6a
    RECT                 m_rectPos;        // +0x248  RepositionFrame CopyRect target
    RECT                 m_rectClip;       // +0x258  RepositionFrame CopyRect target
    BOOL                 m_bInsideRecalc;  // +0x268  RepositionFrame re-entrancy guard
    BOOL                 m_bPreviewMode;   // +0x26c  (declaration order; not touched here)
    HMENU                m_hMenuHelpPopup; // +0x270  `mov %rax,0x270(%rbx)` 0x25dc5b
    HMENU                _m_Reserved;      // +0x278
};
#pragma pack(pop)

static_assert(offsetof(RetailIPFrameTail, m_lpFrame)        + kOff_IPFrameTail == 0x200, "m_lpFrame @0x200");
static_assert(offsetof(RetailIPFrameTail, m_lpDocFrame)     + kOff_IPFrameTail == 0x208, "m_lpDocFrame @0x208");
static_assert(offsetof(RetailIPFrameTail, m_pMainFrame)     + kOff_IPFrameTail == 0x210, "m_pMainFrame @0x210");
static_assert(offsetof(RetailIPFrameTail, m_pDocFrame)      + kOff_IPFrameTail == 0x218, "m_pDocFrame @0x218");
static_assert(offsetof(RetailIPFrameTail, m_hSharedMenu)    + kOff_IPFrameTail == 0x220, "m_hSharedMenu @0x220");
static_assert(offsetof(RetailIPFrameTail, m_menuWidths)     + kOff_IPFrameTail == 0x228, "m_menuWidths @0x228");
static_assert(offsetof(RetailIPFrameTail, m_hOleMenu)       + kOff_IPFrameTail == 0x240, "m_hOleMenu @0x240");
static_assert(offsetof(RetailIPFrameTail, m_rectPos)        + kOff_IPFrameTail == 0x248, "m_rectPos @0x248");
static_assert(offsetof(RetailIPFrameTail, m_rectClip)       + kOff_IPFrameTail == 0x258, "m_rectClip @0x258");
static_assert(offsetof(RetailIPFrameTail, m_bInsideRecalc)  + kOff_IPFrameTail == 0x268, "m_bInsideRecalc @0x268");
static_assert(offsetof(RetailIPFrameTail, m_hMenuHelpPopup) + kOff_IPFrameTail == 0x270, "m_hMenuHelpPopup @0x270");
static_assert(sizeof(OLEINPLACEFRAMEINFO) == 0x20, "OLEINPLACEFRAMEINFO is the 0x20 bytes the ctor clears");
static_assert(sizeof(OLEMENUGROUPWIDTHS) == 0x18, "OLEMENUGROUPWIDTHS is the 0x18 bytes BuildSharedMenu clears");
static_assert(kOff_IPFrameTail + sizeof(RetailIPFrameTail) == kSizeof_COleIPFrameWnd, "retail sizeof == 0x280");

// The retail tail must lie wholly inside OpenMFC's _oleipframewnd_padding
// (protected, so pinned through the public member before it and the size).
static_assert(offsetof(COleIPFrameWnd, m_pResizeBar) + sizeof(void*) <= kOff_IPFrameTail,
              "retail members from +0x1e0 must not overlap OpenMFC's m_pResizeBar");
static_assert(sizeof(COleIPFrameWnd) == kSizeof_COleIPFrameWnd,
              "OpenMFC COleIPFrameWnd must be retail-sized (0x280)");

inline RetailIPFrameTail* Tail(void* pThis) {
    return reinterpret_cast<RetailIPFrameTail*>(static_cast<unsigned char*>(pThis) + kOff_IPFrameTail);
}

// ---- retail CFrameWnd members inside OpenMFC's CFrameWnd padding ----------
constexpr size_t kOff_CFrameWnd_m_bHelpMode       = 0x104;   // OnContextHelp `cmpl $0x1,0x104(%rcx)`
constexpr size_t kOff_CFrameWnd_m_nIDHelp         = 0x164;   // LoadFrame `mov %ebx,0x164(%rcx)`
constexpr size_t kOff_CFrameWnd_m_nIDTracking     = 0x168;   // OnSetMessageString `mov %ebp,0x168(%rsi)`
constexpr size_t kOff_CFrameWnd_m_nIDLastMessage  = 0x16c;   // OnSetMessageString `mov 0x16c(%rsi),%eax`
static_assert(offsetof(CFrameWnd, m_nIDHelp) + sizeof(UINT) <= kOff_CFrameWnd_m_bHelpMode &&
              kOff_CFrameWnd_m_nIDLastMessage + sizeof(UINT) <= sizeof(CFrameWnd),
              "retail CFrameWnd slots +0x104..+0x170 must lie inside OpenMFC's _framewnd_padding");
static_assert(offsetof(CWnd, m_hWnd) == 0x40, "CWnd::m_hWnd @0x40, where every retail body here reads it");

// ---- retail COleServerDoc member read by RecalcLayout ----------------------
constexpr size_t kOff_COleServerDoc_m_pDocObjectServer = 0x268;   // RecalcLayout `cmp %rdi,0x268(%rax)`
static_assert(offsetof(COleServerDoc, m_bEmbedded) + sizeof(BOOL) <= kOff_COleServerDoc_m_pDocObjectServer &&
              kOff_COleServerDoc_m_pDocObjectServer + sizeof(void*) <= sizeof(COleServerDoc) &&
              sizeof(COleServerDoc) == 0x298,
              "retail COleServerDoc::m_pDocObjectServer (+0x268) must lie inside OpenMFC's "
              "zero-filled _coleserverdoc_padding of a retail-sized (0x298) COleServerDoc");

inline int& FrameInt(void* pFrame, size_t off) {
    return *reinterpret_cast<int*>(static_cast<unsigned char*>(pFrame) + off);
}
enum { kHelpInactive = 0, kHelpActive = 1 };   // the two m_bHelpMode values OnContextHelp writes

// ---- MSVC vftable slot dispatch (see the header comment) -------------------
template <typename Fn> inline Fn VSlot(const void* pObj, int slot) {
    return reinterpret_cast<Fn>((*reinterpret_cast<void* const* const*>(pObj))[slot]);
}
enum : int {
    kSlot_CalcWindowRect          = 0x0e0 / 8,   // 28
    kSlot_OnCloseDocument         = 0x118 / 8,   // 35 (document vftable)
    kSlot_GetActiveDocument       = 0x2e8 / 8,   // 93
    kSlot_GetMessageString        = 0x2f8 / 8,   // 95
    kSlot_RecalcLayout            = 0x300 / 8,   // 96
    kSlot_GetDefaultAccelerator   = 0x368 / 8,   // 109
    kSlot_RepositionFrame         = 0x3a0 / 8,   // 116
    kSlot_GetInPlaceMenu          = 0x3c0 / 8,   // 120
    kSlot_OnRequestPositionChange = 0x3c8 / 8,   // 121
};
typedef void      (MS_ABI* Fn_CalcWindowRect)(CWnd*, RECT*, UINT);
typedef void      (MS_ABI* Fn_OnCloseDocument)(CDocument*);
typedef CDocument*(MS_ABI* Fn_GetActiveDocument)(void*);
typedef void      (MS_ABI* Fn_GetMessageString)(const void*, UINT, CString*);
typedef void      (MS_ABI* Fn_RecalcLayout)(void*, BOOL);
typedef HACCEL    (MS_ABI* Fn_GetDefaultAccelerator)(void*);
typedef void      (MS_ABI* Fn_RepositionFrame)(void*, const RECT*, const RECT*);
typedef HMENU     (MS_ABI* Fn_GetInPlaceMenu)(void*);
typedef void      (MS_ABI* Fn_OnRequestPositionChange)(void*, const RECT*);

inline CDocument* GetActiveDocumentV(void* pThis) {
    return VSlot<Fn_GetActiveDocument>(pThis, kSlot_GetActiveDocument)(pThis);
}
inline HMENU GetInPlaceMenuV(void* pThis) {
    return VSlot<Fn_GetInPlaceMenu>(pThis, kSlot_GetInPlaceMenu)(pThis);
}
inline void CalcWindowRectV(CWnd* pWnd, RECT* lpRect, UINT nAdjustType) {
    VSlot<Fn_CalcWindowRect>(pWnd, kSlot_CalcWindowRect)(pWnd, lpRect, nAdjustType);
}

constexpr UINT kAFX_IDW_PANE_FIRST   = 0xE900;   // afxres.h:145
constexpr UINT kWM_SETMESSAGESTRING  = 0x0362;   // afxpriv.h:118
constexpr UINT kWM_KICKIDLE          = 0x036A;   // afxpriv.h:132
constexpr UINT kAdjustBorder         = 0;        // CWnd::adjustBorder
constexpr UINT kAdjustOutside        = 1;        // CWnd::adjustOutside
constexpr UINT kReposDefault         = 0;        // CWnd::reposDefault
constexpr UINT kReposQuery           = 1;        // CWnd::reposQuery

inline CWnd* AsWnd(COleIPFrameWnd* p) { return static_cast<CWnd*>(p); }
inline CFrameWnd* AsFrame(COleIPFrameWnd* p) { return static_cast<CFrameWnd*>(p); }

}  // namespace

// Symbol: ?OnUpdateControlBarMenu@COleIPFrameWnd@@IEAAXPEAVCCmdUI@@@Z
extern "C" void MS_ABI impl__OnUpdateControlBarMenu_COleIPFrameWnd__IEAAXPEAVCCmdUI___Z(COleIPFrameWnd*, CCmdUI* ui) {
    if (ui) impl__Enable_CCmdUI__UEAAXH_Z(ui, TRUE);
}

// Retail ctor, mfc140u 0x25d010: CFrameWnd::CFrameWnd (0x29cae0), plant the
// vftable, zero m_rectPos / m_rectClip (8-byte stores, then ::SetRectEmpty on
// each), memset m_frameInfo to 0 and set
// m_frameInfo.cb = sizeof(OLEINPLACEFRAMEINFO) (0x20, `movl $0x20,(%rbx)` at
// 0x25d066), then zero m_bUIActive (+0x1d8, 4 bytes), m_lpFrame, m_lpDocFrame,
// m_hOleMenu, m_bInsideRecalc (+0x268, 8 bytes, so m_bPreviewMode too),
// m_hSharedMenu, m_pMainFrame, m_pDocFrame and m_hMenuHelpPopup.  The padding
// memset covers every zeroing; the one non-zero store, cb, is reproduced.
// m_pResizeBar(nullptr) also clears the 4 bytes retail writes as m_bUIActive.
COleIPFrameWnd::COleIPFrameWnd()
    : CFrameWnd(), m_pResizeBar(nullptr) {
    memset(_oleipframewnd_padding, 0, sizeof(_oleipframewnd_padding));
    Tail(this)->m_frameInfo.cb = sizeof(OLEINPLACEFRAMEINFO);
}
COleIPFrameWnd::~COleIPFrameWnd() {
}
BOOL COleIPFrameWnd::OnCreateAggregates() {
    return TRUE;
}
BOOL COleIPFrameWnd::OnCreateControlBars(CFrameWnd* pWndFrame, CFrameWnd* pWndDoc) {
    return TRUE;
}
void COleIPFrameWnd::OnRequestPositionChange(LPCRECT lpRect) {
    if (!m_hWnd || !lpRect) return;

    const int width = lpRect->right - lpRect->left;
    const int height = lpRect->bottom - lpRect->top;
    ::SetWindowPos(m_hWnd, nullptr, lpRect->left, lpRect->top,
                   width > 0 ? width : 0, height > 0 ? height : 0,
                   SWP_NOZORDER | SWP_NOACTIVATE);

    if (m_pResizeBar && m_pResizeBar->GetSafeHwnd()) {
        m_pResizeBar->MoveWindow(lpRect->left, lpRect->top,
                                 width > 0 ? width : 0, height > 0 ? height : 0, TRUE);
    }
}

// === Moved from ManualThunks.cpp ===

// COleIPFrameWnd::OnCreateControlBars(CWnd*, CWnd*) -- retail mfc140u 0x3a60,
// vftable slot 115.  That RVA is an image-wide shared `mov $0x1,%eax ; ret`:
// it creates nothing and returns TRUE.
// Symbol: ?OnCreateControlBars@COleIPFrameWnd@@UEAAHPEAVCWnd@@0@Z
extern "C" int MS_ABI impl__OnCreateControlBars_COleIPFrameWnd__UEAAHPEAVCWnd__0_Z(
    COleIPFrameWnd* pThis, CWnd* pWndFrame, CWnd* pWndDoc) {
    (void)pThis; (void)pWndFrame; (void)pWndDoc;
    return TRUE;
}

// COleIPFrameWnd::BuildSharedMenu() -- retail mfc140u 0x25db90, transcribed:
//     HMENU hMenu = GetInPlaceMenu();                     // vslot 120, called FIRST
//     m_hSharedMenu = ::CreateMenu();
//     if (m_hSharedMenu == NULL) return FALSE;
//     memset(&m_menuWidths, 0, sizeof m_menuWidths);      // 0x18 bytes
//     if (m_lpFrame->InsertMenus(m_hSharedMenu, &m_menuWidths) != S_OK) {   // vtbl +0x48
//         ::DestroyMenu(m_hSharedMenu); m_hSharedMenu = NULL; return FALSE;
//     }
//     if (hMenu == NULL) return TRUE;                     // container menus only
//     m_hMenuHelpPopup = AfxMergeMenus(m_hSharedMenu, hMenu, m_menuWidths.width,
//                                      1, m_menuWidths.width[5] != 0);   // +0x23c
//     m_hOleMenu = ::OleCreateMenuDescriptor(m_hSharedMenu, &m_menuWidths);
//     return m_hOleMenu != NULL;
// Retail does not test m_lpFrame before calling through it; neither does this.
// While GetInPlaceMenu below is a stub, a frame that does not override it gets
// hMenu == NULL and so the container-menus-only result.
// Symbol: ?BuildSharedMenu@COleIPFrameWnd@@MEAAHXZ
extern "C" int MS_ABI impl__BuildSharedMenu_COleIPFrameWnd__MEAAHXZ(COleIPFrameWnd* pThis) {
    RetailIPFrameTail* t = Tail(pThis);
    HMENU hMenu = GetInPlaceMenuV(pThis);
    t->m_hSharedMenu = ::CreateMenu();
    if (t->m_hSharedMenu == nullptr)
        return FALSE;
    memset(&t->m_menuWidths, 0, sizeof(t->m_menuWidths));
    if (t->m_lpFrame->InsertMenus(t->m_hSharedMenu, &t->m_menuWidths) != S_OK) {
        ::DestroyMenu(t->m_hSharedMenu);
        t->m_hSharedMenu = nullptr;
        return FALSE;
    }
    if (hMenu == nullptr)
        return TRUE;
    t->m_hMenuHelpPopup = impl__AfxMergeMenus__YAPEAUHMENU____PEAU1_0PEAJHH_Z(
        t->m_hSharedMenu, hMenu, t->m_menuWidths.width, 1, t->m_menuWidths.width[5] != 0);
    t->m_hOleMenu = ::OleCreateMenuDescriptor(t->m_hSharedMenu, &t->m_menuWidths);
    return t->m_hOleMenu != nullptr;
}

// COleIPFrameWnd::CreateObject() -- retail mfc140u 0x25dc90, transcribed:
//     void* p = operator new(0x280);                      // ??2@YAPEAX_K@Z, 0x27f0
//     if (p != NULL) COleIPFrameWnd::COleIPFrameWnd(p);   // 0x25d010
//     return p;
// Both calls go to this DLL's own exports: the MFC operator new
// (detail/MemcoreSupport.cpp:5, malloc-based) and the ctor thunk
// (core/ole/Thunks.cpp:1158), which placement-constructs the C++ class of
// this file.  The object therefore carries OpenMFC's g++ vftable, not
// retail's (see the header comment).  Retail's class descriptor points
// m_pfnCreateObject here; OpenMFC's (core/ole/RuntimeClasses.cpp:241)
// currently holds nullptr, so RUNTIME_CLASS(COleIPFrameWnd)->CreateObject()
// does not reach this yet.
// Symbol: ?CreateObject@COleIPFrameWnd@@SAPEAVCObject@@XZ
extern "C" CObject* MS_ABI impl__CreateObject_COleIPFrameWnd__SAPEAVCObject__XZ() {
    void* p = impl___2_YAPEAX_K_Z(kSizeof_COleIPFrameWnd);
    if (p != nullptr)
        impl___0COleIPFrameWnd__QEAA_XZ(p);
    return static_cast<CObject*>(static_cast<COleIPFrameWnd*>(p));
}

// COleIPFrameWnd::DestroySharedMenu() -- retail mfc140u 0x256c00, transcribed:
//     if (m_hSharedMenu == NULL) return;
//     HMENU hMenu = GetInPlaceMenu();                     // vslot 120
//     if (hMenu == NULL) return;                          // shared menu NOT destroyed
//     AfxUnmergeMenus(m_hSharedMenu, hMenu, m_hMenuHelpPopup);
//     m_lpFrame->RemoveMenus(m_hSharedMenu);              // vtbl +0x58
//     ::DestroyMenu(m_hSharedMenu); m_hSharedMenu = NULL;
//     if (m_hOleMenu != NULL) { ::OleDestroyMenuDescriptor(m_hOleMenu); m_hOleMenu = NULL; }
//     m_hMenuHelpPopup = NULL;                            // on both paths of that test
// Note the early return when GetInPlaceMenu() is NULL: a shared menu that
// BuildSharedMenu built from the container's groups alone is left alive.
// That is retail's behaviour and is kept.
// Symbol: ?DestroySharedMenu@COleIPFrameWnd@@MEAAXXZ
extern "C" void MS_ABI impl__DestroySharedMenu_COleIPFrameWnd__MEAAXXZ(COleIPFrameWnd* pThis) {
    RetailIPFrameTail* t = Tail(pThis);
    if (t->m_hSharedMenu == nullptr)
        return;
    HMENU hMenu = GetInPlaceMenuV(pThis);
    if (hMenu == nullptr)
        return;
    impl__AfxUnmergeMenus__YAXPEAUHMENU____00_Z(t->m_hSharedMenu, hMenu, t->m_hMenuHelpPopup);
    t->m_lpFrame->RemoveMenus(t->m_hSharedMenu);
    ::DestroyMenu(t->m_hSharedMenu);
    t->m_hSharedMenu = nullptr;
    if (t->m_hOleMenu != nullptr) {
        ::OleDestroyMenuDescriptor(t->m_hOleMenu);
        t->m_hOleMenu = nullptr;
    }
    t->m_hMenuHelpPopup = nullptr;
}

// COleIPFrameWnd::GetInPlaceMenu() -- retail mfc140u 0x25db60:
//     CDocument* pDoc = GetActiveDocument();              // vslot 93
//     return pDoc->[+0x50]->[+0x88];   // m_pDocTemplate->m_hMenuInPlaceServer
// with no NULL test on either pointer.  (+0x50 is CDocument::m_pDocTemplate,
// as core/ole/COleLinkingDoc.cpp:42 also records.  Template +0x88 is the
// in-place-server menu, MFC's m_hMenuInPlaceServer: CDocTemplate::LoadTemplate
// (mfc140u 0x226240) fills it with ::LoadMenuW of the resource ID at +0x9c,
// which CDocTemplate::SetServerInfo (mfc140u 0x226390) stores from its
// nIDOleInPlaceServer argument.)  STUB: OpenMFC's
// CDocument declares no m_pDocTemplate (see also core/frame/CSplitterWnd.cpp:469)
// and OpenMFC's CDocTemplate declares no in-place-server menu member, so the
// menu cannot be reached.  NULL is
// what BuildSharedMenu / DestroySharedMenu treat as "server has no in-place
// menu".
// Symbol: ?GetInPlaceMenu@COleIPFrameWnd@@MEAAPEAUHMENU__@@XZ
extern "C" HMENU MS_ABI impl__GetInPlaceMenu_COleIPFrameWnd__MEAAPEAUHMENU____XZ(COleIPFrameWnd* pThis) {
    (void)pThis;
    return nullptr;   // TODO(clean-room): needs CDocument::m_pDocTemplate and CDocTemplate::m_hMenuInPlaceServer
}

// COleIPFrameWnd::LoadFrame(UINT, DWORD, CWnd*, CCreateContext*) -- retail
// mfc140u 0x25d420, transcribed:
//     m_nIDHelp = nIDResource;                            // CFrameWnd +0x164
//     CRect rect(0, 0, 0, 0);
//     if (!CWnd::Create(NULL, NULL, dwDefaultStyle, rect, pParentWnd,
//                       nIDResource, pContext))           // direct call, 0x28b690
//         return FALSE;
//     LoadAccelTable(MAKEINTRESOURCE(nIDResource));       // `movzwl %bx,%edx`
//     return TRUE;
// Retail deliberately does not call CFrameWnd::LoadFrame.  m_nIDHelp is
// written at its retail offset; OpenMFC's own CFrameWnd::m_nIDHelp (+0xf8) is
// left alone (see the header comment).
// Symbol: ?LoadFrame@COleIPFrameWnd@@UEAAHIKPEAVCWnd@@PEAUCCreateContext@@@Z
extern "C" int MS_ABI impl__LoadFrame_COleIPFrameWnd__UEAAHIKPEAVCWnd__PEAUCCreateContext___Z(
    COleIPFrameWnd* pThis, UINT nIDResource, DWORD dwDefaultStyle, CWnd* pParentWnd,
    CCreateContext* pContext) {
    FrameInt(pThis, kOff_CFrameWnd_m_nIDHelp) = static_cast<int>(nIDResource);
    RECT rect = {0, 0, 0, 0};
    if (!impl__Create_CWnd__UEAAHPEB_W0KAEBUtagRECT__PEAV1_IPEAUCCreateContext___Z(
            AsWnd(pThis), nullptr, nullptr, dwDefaultStyle, rect, pParentWnd, nIDResource, pContext))
        return FALSE;
    impl__LoadAccelTable_CFrameWnd__QEAAHPEB_W_Z(AsFrame(pThis), MAKEINTRESOURCEW(nIDResource));
    return TRUE;
}

// COleIPFrameWnd::OnBarCheck(UINT) -- retail mfc140u 0x25d9c0, transcribed:
//     if (GetControlBar(nID) != NULL) return CFrameWnd::OnBarCheck(nID);
//     if (m_pMainFrame != NULL && m_pMainFrame->GetControlBar(nID) != NULL)
//         return m_pMainFrame->OnBarCheck(nID);
//     if (m_pDocFrame != NULL && m_pDocFrame->GetControlBar(nID) != NULL)
//         return m_pDocFrame->OnBarCheck(nID);
//     return FALSE;
// Every call is direct (CFrameWnd::GetControlBar 0x29f7d0, three call sites;
// CFrameWnd::OnBarCheck 0x29f890, one call site at 0x25d9de that all three
// successful branches jump to with their own frame in %rcx).
// Symbol: ?OnBarCheck@COleIPFrameWnd@@IEAAHI@Z
extern "C" int MS_ABI impl__OnBarCheck_COleIPFrameWnd__IEAAHI_Z(COleIPFrameWnd* pThis, UINT nID) {
    if (impl__GetControlBar_CFrameWnd__QEAAPEAVCControlBar__I_Z(AsFrame(pThis), nID) != nullptr)
        return impl__OnBarCheck_CFrameWnd__QEAAHI_Z(AsFrame(pThis), nID);
    RetailIPFrameTail* t = Tail(pThis);
    if (t->m_pMainFrame != nullptr &&
        impl__GetControlBar_CFrameWnd__QEAAPEAVCControlBar__I_Z(t->m_pMainFrame, nID) != nullptr)
        return impl__OnBarCheck_CFrameWnd__QEAAHI_Z(t->m_pMainFrame, nID);
    if (t->m_pDocFrame != nullptr &&
        impl__GetControlBar_CFrameWnd__QEAAPEAVCControlBar__I_Z(t->m_pDocFrame, nID) != nullptr)
        return impl__OnBarCheck_CFrameWnd__QEAAHI_Z(t->m_pDocFrame, nID);
    return FALSE;
}

// COleIPFrameWnd::OnContextHelp() -- retail mfc140u 0x25da40, transcribed:
//     if (m_bHelpMode == 1 /*active*/) return;            // CFrameWnd +0x104
//     if (!CFrameWnd::CanEnterHelpMode()) return;         // 0x2a1c70
//     BOOL bHelpMode = m_bHelpMode;  m_bHelpMode = 1;
//     if (m_lpFrame->ContextSensitiveHelp(TRUE) != S_OK ||           // vtbl +0x20
//         (m_lpDocFrame != NULL && m_lpDocFrame->ContextSensitiveHelp(TRUE) != S_OK)) {
//         m_bHelpMode = 0; return;
//     }
//     m_bHelpMode = bHelpMode;
//     CFrameWnd* pFrame = GetTopLevelFrame();             // 0x28e490
//     if (pFrame == NULL) AfxThrowInvalidArgException();
//     if (pFrame != this) pFrame->m_bHelpMode = 1;
//     CFrameWnd::OnContextHelp();                         // 0x2a1cf0, direct
//     if (pFrame != this) pFrame->m_bHelpMode = m_bHelpMode;
//     if (m_bHelpMode == 0) {
//         m_lpFrame->ContextSensitiveHelp(FALSE);
//         if (m_lpDocFrame != NULL) m_lpDocFrame->ContextSensitiveHelp(FALSE);
//     }
// m_lpFrame / m_lpDocFrame are re-read at each use, as retail does.  Retail
// does not test m_lpFrame for NULL; neither does this.  Callee note: OpenMFC's
// impl__OnContextHelp_CFrameWnd (core/frame/Thunks.cpp:1468) forwards through
// `pThis->OnContextHelp()`, which OpenMFC's afxwin.h declares virtual (retail's
// is not virtual: QEAA), and g++ compiles that as a call through vftable
// +0xb0 (slot 22).  In a retail-layout vftable slot 22 is
// CWnd::PreSubclassWindow (checked against the CMFCButton vftable; in the
// COleIPFrameWnd vftable 0x32fb58 it holds the shared `ret` 0x27d0), so on a
// client object the CFrameWnd::OnContextHelp step runs OpenMFC's
// PreSubclassWindow export instead.  Only an object carrying OpenMFC's own g++
// vftable reaches the C++ CFrameWnd::OnContextHelp.
// Symbol: ?OnContextHelp@COleIPFrameWnd@@IEAAXXZ
extern "C" void MS_ABI impl__OnContextHelp_COleIPFrameWnd__IEAAXXZ(COleIPFrameWnd* pThis) {
    int& helpMode = FrameInt(pThis, kOff_CFrameWnd_m_bHelpMode);
    if (helpMode == kHelpActive)
        return;
    if (!impl__CanEnterHelpMode_CFrameWnd__QEAAHXZ(AsFrame(pThis)))
        return;

    RetailIPFrameTail* t = Tail(pThis);
    const int bHelpMode = helpMode;
    helpMode = kHelpActive;
    if (t->m_lpFrame->ContextSensitiveHelp(TRUE) != S_OK ||
        (t->m_lpDocFrame != nullptr && t->m_lpDocFrame->ContextSensitiveHelp(TRUE) != S_OK)) {
        helpMode = kHelpInactive;
        return;
    }
    helpMode = bHelpMode;

    CFrameWnd* pFrame = impl__GetTopLevelFrame_CWnd__QEBAPEAVCFrameWnd__XZ(AsWnd(pThis));
    if (pFrame == nullptr)
        impl__AfxThrowInvalidArgException__YAXXZ();
    int& frameHelpMode = FrameInt(pFrame, kOff_CFrameWnd_m_bHelpMode);
    if (pFrame != AsFrame(pThis))
        frameHelpMode = kHelpActive;

    impl__OnContextHelp_CFrameWnd__QEAAXXZ(AsFrame(pThis));

    if (pFrame != AsFrame(pThis))
        frameHelpMode = helpMode;
    if (helpMode == kHelpInactive) {
        t->m_lpFrame->ContextSensitiveHelp(FALSE);
        if (t->m_lpDocFrame != nullptr)
            t->m_lpDocFrame->ContextSensitiveHelp(FALSE);
    }
}

// COleIPFrameWnd::OnCreate(LPCREATESTRUCT) -- retail mfc140u 0x25d160
// (WM_CREATE), transcribed:
//     if (lpcs == NULL) AfxThrowInvalidArgException();
//     if (CFrameWnd::OnCreateHelper(lpcs, (CCreateContext*)lpcs->lpCreateParams) < 0)
//         return -1;          // CFrameWnd::OnCreate (0x29db80) inlined: it is exactly
//                             // that NULL test plus a tail call to OnCreateHelper
//     ModifyStyleEx(WS_EX_CLIENTEDGE, 0, 0);              // 0x2a9740
//     MSG msg;
//     ::PeekMessage(&msg, m_hWnd, WM_SETMESSAGESTRING, WM_SETMESSAGESTRING,
//                   PM_REMOVE | PM_NOYIELD);              // ONE call, not a loop
//     return 0;
// Symbol: ?OnCreate@COleIPFrameWnd@@IEAAHPEAUtagCREATESTRUCTW@@@Z
extern "C" int MS_ABI impl__OnCreate_COleIPFrameWnd__IEAAHPEAUtagCREATESTRUCTW___Z(
    COleIPFrameWnd* pThis, CREATESTRUCTW* lpCreateStruct) {
    if (lpCreateStruct == nullptr)
        impl__AfxThrowInvalidArgException__YAXXZ();
    if (impl__OnCreateHelper_CFrameWnd__IEAAHPEAUtagCREATESTRUCTW__PEAUCCreateContext___Z(
            AsFrame(pThis), lpCreateStruct,
            static_cast<CCreateContext*>(lpCreateStruct->lpCreateParams)) < 0)
        return -1;
    impl__ModifyStyleEx_CWnd__QEAAHKKI_Z(AsWnd(pThis), WS_EX_CLIENTEDGE, 0, 0);
    MSG msg;
    ::PeekMessageW(&msg, AsWnd(pThis)->m_hWnd, kWM_SETMESSAGESTRING, kWM_SETMESSAGESTRING,
                   PM_REMOVE | PM_NOYIELD);
    return 0;
}

// COleIPFrameWnd::OnDestroy() -- retail mfc140u 0x25d1c0 (WM_DESTROY),
// transcribed:
//     CDocument* pDoc = GetActiveDocument();              // vslot 93
//     if (pDoc != NULL) {
//         pDoc->DisconnectViews();                        // 0x21ad20, direct
//         pDoc->OnCloseDocument();                        // document vslot 35
//     }
//     if (m_pMainFrame != NULL) m_pMainFrame->DestroyDockBars();   // 0x29f640
//     if (m_pDocFrame != NULL)  m_pDocFrame->DestroyDockBars();
//     CFrameWnd::OnDestroy();                             // tail jump, 0x29e2a0
// Symbol: ?OnDestroy@COleIPFrameWnd@@IEAAXXZ
extern "C" void MS_ABI impl__OnDestroy_COleIPFrameWnd__IEAAXXZ(COleIPFrameWnd* pThis) {
    CDocument* pDoc = GetActiveDocumentV(pThis);
    if (pDoc != nullptr) {
        impl__DisconnectViews_CDocument__QEAAXXZ(pDoc);
        VSlot<Fn_OnCloseDocument>(pDoc, kSlot_OnCloseDocument)(pDoc);
    }
    RetailIPFrameTail* t = Tail(pThis);
    if (t->m_pMainFrame != nullptr)
        impl__DestroyDockBars_CFrameWnd__QEAAXXZ(t->m_pMainFrame);
    if (t->m_pDocFrame != nullptr)
        impl__DestroyDockBars_CFrameWnd__QEAAXXZ(t->m_pDocFrame);
    impl__OnDestroy_CFrameWnd__IEAAXXZ(AsFrame(pThis));
}

// COleIPFrameWnd::OnRecalcParent(WPARAM, LPARAM) -- retail mfc140u 0x25d5b0
// (WM_RECALCPARENT), transcribed:
//     RepositionFrame(&m_rectPos, &m_rectClip);           // vslot 116
//     if ((LPRECT)lParam != NULL) *(LPRECT)lParam = m_rectPos;
//     return TRUE;
// Symbol: ?OnRecalcParent@COleIPFrameWnd@@IEAA_J_K_J@Z
extern "C" LRESULT MS_ABI impl__OnRecalcParent_COleIPFrameWnd__IEAA_J_K_J_Z(
    COleIPFrameWnd* pThis, WPARAM wParam, LPARAM lParam) {
    (void)wParam;
    RetailIPFrameTail* t = Tail(pThis);
    VSlot<Fn_RepositionFrame>(pThis, kSlot_RepositionFrame)(pThis, &t->m_rectPos, &t->m_rectClip);
    if (reinterpret_cast<RECT*>(lParam) != nullptr)
        *reinterpret_cast<RECT*>(lParam) = t->m_rectPos;
    return TRUE;
}

// COleIPFrameWnd::OnResizeChild(WPARAM, LPARAM) -- retail mfc140u 0x25d490
// (WM_SIZECHILD, sent by COleResizeBar), transcribed:
//     if (GetActiveDocument() == NULL) return 0;          // vslot 93
//     CRect rectNew; ::CopyRect(&rectNew, (LPCRECT)lParam);
//     CWnd* pParentWnd = CWnd::FromHandle(::GetParent(m_hWnd));   // GetParent, inlined
//     ClientToScreen(&rectNew);                           // this
//     pParentWnd->ScreenToClient(&rectNew);
//     CWnd* pLeftOver = GetDlgItem(AFX_IDW_PANE_FIRST);
//     CRect rectCur = m_rectPos;
//     pLeftOver->CalcWindowRect(&rectCur, CWnd::adjustOutside);   // vslot 28
//     rectNew.left   += m_rectPos.left   - rectCur.left;
//     rectNew.top    += m_rectPos.top    - rectCur.top;
//     rectNew.right  += m_rectPos.right  - rectCur.right;
//     rectNew.bottom += m_rectPos.bottom - rectCur.bottom;
//     OnRequestPositionChange(&rectNew);                  // vslot 121
//     return 0;
// Retail tests neither pParentWnd nor pLeftOver for NULL; neither does this.
// Symbol: ?OnResizeChild@COleIPFrameWnd@@IEAA_J_K_J@Z
extern "C" LRESULT MS_ABI impl__OnResizeChild_COleIPFrameWnd__IEAA_J_K_J_Z(
    COleIPFrameWnd* pThis, WPARAM wParam, LPARAM lParam) {
    (void)wParam;
    if (GetActiveDocumentV(pThis) == nullptr)
        return 0;
    RECT rectNew = {0, 0, 0, 0};
    ::CopyRect(&rectNew, reinterpret_cast<const RECT*>(lParam));
    CWnd* pParentWnd = impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(::GetParent(AsWnd(pThis)->m_hWnd));
    impl__ClientToScreen_CWnd__QEBAXPEAUtagRECT___Z(AsWnd(pThis), &rectNew);
    impl__ScreenToClient_CWnd__QEBAXPEAUtagRECT___Z(pParentWnd, &rectNew);

    RetailIPFrameTail* t = Tail(pThis);
    CWnd* pLeftOver = impl__GetDlgItem_CWnd__QEBAPEAV1_H_Z(AsWnd(pThis), kAFX_IDW_PANE_FIRST);
    RECT rectCur = t->m_rectPos;
    CalcWindowRectV(pLeftOver, &rectCur, kAdjustOutside);
    rectNew.left   += t->m_rectPos.left   - rectCur.left;
    rectNew.top    += t->m_rectPos.top    - rectCur.top;
    rectNew.right  += t->m_rectPos.right  - rectCur.right;
    rectNew.bottom += t->m_rectPos.bottom - rectCur.bottom;
    VSlot<Fn_OnRequestPositionChange>(pThis, kSlot_OnRequestPositionChange)(pThis, &rectNew);
    return 0;
}

// COleIPFrameWnd::OnSetMessageString(WPARAM, LPARAM) -- retail mfc140u
// 0x25d300 (vslot 117; WM_SETMESSAGESTRING reaches it through the vcall thunk
// 0x25dcc0), transcribed:
//     if (m_lpFrame != NULL) {
//         CString strMessage;
//         LPCTSTR lpsz = (LPCTSTR)lParam;
//         if (lpsz == NULL) {
//             if (wParam != 0) {
//                 GetMessageString((UINT)wParam, strMessage);   // vslot 95
//                 lpsz = strMessage;
//             }
//             if (lpsz == NULL) lpsz = _T("");
//         }
//         m_lpFrame->SetStatusText(CStringW(lpsz));        // vtbl +0x60; temporary
//     }
//     UINT nIDLast = m_nIDLastMessage;                    // CFrameWnd +0x16c
//     m_nIDLastMessage = (UINT)wParam;
//     m_nIDTracking    = (UINT)wParam;                    // CFrameWnd +0x168
//     return nIDLast;                                     // zero-extended 32-bit
// An explicit lParam string wins without consulting wParam; the GetMessageString
// branch is taken only when lParam is NULL.
// Symbol: ?OnSetMessageString@COleIPFrameWnd@@UEAA_J_K_J@Z
extern "C" LRESULT MS_ABI impl__OnSetMessageString_COleIPFrameWnd__UEAA_J_K_J_Z(
    COleIPFrameWnd* pThis, WPARAM wParam, LPARAM lParam) {
    RetailIPFrameTail* t = Tail(pThis);
    if (t->m_lpFrame != nullptr) {
        CString strMessage;
        const wchar_t* lpsz = reinterpret_cast<const wchar_t*>(lParam);
        if (lpsz == nullptr) {
            if (wParam != 0) {
                VSlot<Fn_GetMessageString>(pThis, kSlot_GetMessageString)(
                    pThis, static_cast<UINT>(wParam), &strMessage);
                lpsz = strMessage.GetString();
            }
            if (lpsz == nullptr)
                lpsz = L"";
        }
        CString strStatus(lpsz);
        t->m_lpFrame->SetStatusText(strStatus.GetString());
    }
    const UINT nIDLast = static_cast<UINT>(FrameInt(pThis, kOff_CFrameWnd_m_nIDLastMessage));
    FrameInt(pThis, kOff_CFrameWnd_m_nIDLastMessage) = static_cast<int>(static_cast<UINT>(wParam));
    FrameInt(pThis, kOff_CFrameWnd_m_nIDTracking) = static_cast<int>(static_cast<UINT>(wParam));
    return static_cast<LRESULT>(nIDLast);
}

// COleIPFrameWnd::OnSize(UINT, int, int) -- retail mfc140u 0x27d0 (the
// WM_SIZE entry of the message map).  That RVA is an image-wide shared
// `ret 0`, so the handler does nothing at all -- in particular it does not
// chain to CFrameWnd::OnSize.  The empty body IS the transcription.
// Symbol: ?OnSize@COleIPFrameWnd@@IEAAXIHH@Z
extern "C" void MS_ABI impl__OnSize_COleIPFrameWnd__IEAAXIHH_Z(
    COleIPFrameWnd* pThis, UINT nType, int cx, int cy) {
    (void)pThis; (void)nType; (void)cx; (void)cy;
}

// COleIPFrameWnd::OnWindowPosChanging(LPWINDOWPOS) -- retail mfc140u 0x25d2a0
// (WM_WINDOWPOSCHANGING), transcribed:
//     MSG msg;
//     if (!::PeekMessage(&msg, NULL, WM_KICKIDLE, WM_KICKIDLE, PM_NOREMOVE))
//         ::PostThreadMessage(::GetCurrentThreadId(), WM_KICKIDLE, 0, 0);
//     Default();                                          // 0x28ac80
// (IAT slots, mfc140u: PeekMessageW 0x1802c7198, GetCurrentThreadId
// 0x1802c6638, PostThreadMessageW 0x1802c6bf8.)
// Symbol: ?OnWindowPosChanging@COleIPFrameWnd@@IEAAXPEAUtagWINDOWPOS@@@Z
extern "C" void MS_ABI impl__OnWindowPosChanging_COleIPFrameWnd__IEAAXPEAUtagWINDOWPOS___Z(
    COleIPFrameWnd* pThis, WINDOWPOS* lpWndPos) {
    (void)lpWndPos;
    MSG msg;
    if (!::PeekMessageW(&msg, nullptr, kWM_KICKIDLE, kWM_KICKIDLE, PM_NOREMOVE))
        ::PostThreadMessageW(::GetCurrentThreadId(), kWM_KICKIDLE, 0, 0);
    impl__Default_CWnd__IEAA_JXZ(AsWnd(pThis));
}

// COleIPFrameWnd::PreTranslateMessage(MSG*) -- retail mfc140u 0x25d870
// (vslot 69), transcribed:
//     if (CFrameWnd::PreTranslateMessage(pMsg)) return TRUE;   // 0x29ceb0, direct
//     if (pMsg->message >= WM_KEYFIRST && pMsg->message <= 0x109) {
//         HACCEL hAccel = GetDefaultAccelerator();        // vslot 109
//         if (hAccel != NULL &&
//             ::IsAccelerator(hAccel, ::CopyAcceleratorTable(hAccel, NULL, 0), pMsg, NULL))
//             return TRUE;
//         OLEINPLACEFRAMEINFO frameInfo = m_frameInfo;    // 32-byte copy of +0x1e0
//         return ::OleTranslateAccelerator(m_lpFrame, &frameInfo, pMsg) == S_OK;
//     }
//     return FALSE;
// Callee note: OpenMFC's impl__PreTranslateMessage_CFrameWnd
// (core/frame/CFrameWnd.cpp:41) forwards through `pThis->PreTranslateMessage`,
// a g++-virtual call through vftable +0x158 (slot 43), not retail's +0x228
// (slot 69).  In a retail-layout vftable slot 43 is CWnd::get_accName(VARIANT,
// BSTR*) (0x290520 in the COleIPFrameWnd vftable 0x32fb58), so on a client
// object this base step calls get_accName with pMsg as the VARIANT* and an
// unset third argument as the BSTR* out-pointer.  It is harmless today only
// because OpenMFC's get_accName export (core/window/CWnd.cpp:3867) is a
// `return 0` stub, which reads as "not translated"; a faithful get_accName (or
// a client override of it) would not be.  Only an object carrying OpenMFC's own
// g++ vftable reaches the C++ CFrameWnd::PreTranslateMessage.
// Symbol: ?PreTranslateMessage@COleIPFrameWnd@@UEAAHPEAUtagMSG@@@Z
extern "C" int MS_ABI impl__PreTranslateMessage_COleIPFrameWnd__UEAAHPEAUtagMSG___Z(
    COleIPFrameWnd* pThis, MSG* pMsg) {
    if (impl__PreTranslateMessage_CFrameWnd__UEAAHPEAUtagMSG___Z(AsFrame(pThis), pMsg))
        return TRUE;
    if (pMsg->message < WM_KEYFIRST || pMsg->message > 0x109)
        return FALSE;
    HACCEL hAccel = VSlot<Fn_GetDefaultAccelerator>(pThis, kSlot_GetDefaultAccelerator)(pThis);
    if (hAccel != nullptr &&
        ::IsAccelerator(hAccel, ::CopyAcceleratorTableW(hAccel, nullptr, 0), pMsg, nullptr))
        return TRUE;
    RetailIPFrameTail* t = Tail(pThis);
    OLEINPLACEFRAMEINFO frameInfo = t->m_frameInfo;
    return ::OleTranslateAccelerator(t->m_lpFrame, &frameInfo, pMsg) == S_OK;
}

// COleIPFrameWnd::RecalcLayout(BOOL) -- retail mfc140u 0x25d600 (vslot 96),
// transcribed (bNotify is ignored):
//     CWnd* pParentWnd = CWnd::FromHandle(::GetParent(m_hWnd));
//     COleServerDoc* pDoc = (COleServerDoc*)GetActiveDocument();   // vslot 93
//     BOOL bDocObject = pDoc != NULL && pDoc->[+0x268] != NULL;    // IsDocObject(), inlined
//     CRect rectBig(0, 0, INT_MAX/2, INT_MAX/2);          // .rdata 0x350240 = {0,0,0x3fffffff,0x3fffffff}
//     CRect rectLeft(0, 0, 0, 0);
//     RepositionBars(0, 0xffff, AFX_IDW_PANE_FIRST, reposQuery, &rectLeft, &rectBig, TRUE);
//     CRect rect = m_rectPos;
//     rect.left  -= rectLeft.left;           rect.top    -= rectLeft.top;
//     rect.right += INT_MAX/2 - rectLeft.right;  rect.bottom += INT_MAX/2 - rectLeft.bottom;
//     CWnd* pLeftOver = GetDlgItem(AFX_IDW_PANE_FIRST);
//     if (pLeftOver != NULL) {
//         rectBig = m_rectPos;
//         pLeftOver->CalcWindowRect(&rectBig, CWnd::adjustOutside);   // vslot 28
//         rect.left   += rectBig.left   - m_rectPos.left;
//         rect.top    += rectBig.top    - m_rectPos.top;
//         rect.right  += rectBig.right  - m_rectPos.right;
//         rect.bottom += rectBig.bottom - m_rectPos.bottom;
//     }
//     CalcWindowRect(&rect, bDocObject ? adjustOutside : adjustBorder);  // this, vslot 28
//     CRect rectClip; ::IntersectRect(&rectClip, &rect, &m_rectClip);
//     AfxRepositionWindow(NULL, m_hWnd, &rectClip);
//     pParentWnd->ClientToScreen(&rect);
//     ScreenToClient(&rect);                              // this
//     RepositionBars(0, 0xffff, AFX_IDW_PANE_FIRST, reposDefault, NULL, &rect, TRUE);
// Retail uses the constant INT_MAX/2 for rectBig.right/bottom after the query
// pass (it never re-reads them); the same constant is used here.
// bDocObject: retail reads COleServerDoc::m_pDocObjectServer at document
// +0x268 (`cmp %rdi,0x268(%rax)` at 0x25d651) with no class check, and so does
// this.  OpenMFC's COleServerDoc declares no such member, but it is
// retail-sized (0x298) and +0x268 lies inside its _coleserverdoc_padding,
// which its ctor zero-fills (core/ole/COleServerDoc.cpp:443); the same offset
// is read raw by core/ole/CDocObjectServer.cpp (kOwnerOff_m_pDocObjectServer).
// No OpenMFC code writes it, so on an OpenMFC-constructed server document the
// test yields FALSE, as retail's does for a document with no DocObject server.
// Symbol: ?RecalcLayout@COleIPFrameWnd@@UEAAXH@Z
extern "C" void MS_ABI impl__RecalcLayout_COleIPFrameWnd__UEAAXH_Z(COleIPFrameWnd* pThis, BOOL bNotify) {
    (void)bNotify;
    CWnd* pSelf = AsWnd(pThis);
    CWnd* pParentWnd = impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(::GetParent(pSelf->m_hWnd));
    CDocument* pDoc = GetActiveDocumentV(pThis);
    const BOOL bDocObject = pDoc != nullptr &&
        *reinterpret_cast<void* const*>(reinterpret_cast<const unsigned char*>(pDoc) +
                                        kOff_COleServerDoc_m_pDocObjectServer) != nullptr;

    constexpr LONG kBig = 0x3fffffff;   // INT_MAX / 2
    RECT rectBig = {0, 0, kBig, kBig};
    RECT rectLeft = {0, 0, 0, 0};
    impl__RepositionBars_CWnd__QEAAXIIIIPEAUtagRECT__PEBU2_H_Z(
        pSelf, 0, 0xffff, kAFX_IDW_PANE_FIRST, kReposQuery, &rectLeft, &rectBig, TRUE);

    RetailIPFrameTail* t = Tail(pThis);
    RECT rect = t->m_rectPos;
    rect.left   -= rectLeft.left;
    rect.top    -= rectLeft.top;
    rect.right  += kBig - rectLeft.right;
    rect.bottom += kBig - rectLeft.bottom;

    CWnd* pLeftOver = impl__GetDlgItem_CWnd__QEBAPEAV1_H_Z(pSelf, kAFX_IDW_PANE_FIRST);
    if (pLeftOver != nullptr) {
        rectBig = t->m_rectPos;
        CalcWindowRectV(pLeftOver, &rectBig, kAdjustOutside);
        rect.left   += rectBig.left   - t->m_rectPos.left;
        rect.top    += rectBig.top    - t->m_rectPos.top;
        rect.right  += rectBig.right  - t->m_rectPos.right;
        rect.bottom += rectBig.bottom - t->m_rectPos.bottom;
    }

    CalcWindowRectV(pSelf, &rect, bDocObject ? kAdjustOutside : kAdjustBorder);

    RECT rectClip = {0, 0, 0, 0};
    ::IntersectRect(&rectClip, &rect, &t->m_rectClip);
    impl__AfxRepositionWindow__YAXPEAUAFX_SIZEPARENTPARAMS__PEAUHWND____PEBUtagRECT___Z(
        nullptr, pSelf->m_hWnd, &rectClip);

    impl__ClientToScreen_CWnd__QEBAXPEAUtagRECT___Z(pParentWnd, &rect);
    impl__ScreenToClient_CWnd__QEBAXPEAUtagRECT___Z(pSelf, &rect);

    impl__RepositionBars_CWnd__QEAAXIIIIPEAUtagRECT__PEBU2_H_Z(
        pSelf, 0, 0xffff, kAFX_IDW_PANE_FIRST, kReposDefault, nullptr, &rect, TRUE);
}

// COleIPFrameWnd::RepositionFrame(LPCRECT, LPCRECT) -- retail mfc140u 0x25d800
// (vslot 116), transcribed:
//     if (m_bInsideRecalc) return;
//     m_bInsideRecalc = TRUE;
//     ::CopyRect(&m_rectPos, lpPosRect);
//     ::CopyRect(&m_rectClip, lpClipRect);
//     RecalcLayout(TRUE);                                 // vslot 96
//     m_bInsideRecalc = FALSE;
// Symbol: ?RepositionFrame@COleIPFrameWnd@@UEAAXPEBUtagRECT@@0@Z
extern "C" void MS_ABI impl__RepositionFrame_COleIPFrameWnd__UEAAXPEBUtagRECT__0_Z(
    COleIPFrameWnd* pThis, const RECT* lpPosRect, const RECT* lpClipRect) {
    RetailIPFrameTail* t = Tail(pThis);
    if (t->m_bInsideRecalc)
        return;
    t->m_bInsideRecalc = TRUE;
    ::CopyRect(&t->m_rectPos, lpPosRect);
    ::CopyRect(&t->m_rectClip, lpClipRect);
    VSlot<Fn_RecalcLayout>(pThis, kSlot_RecalcLayout)(pThis, TRUE);
    t->m_bInsideRecalc = FALSE;
}
