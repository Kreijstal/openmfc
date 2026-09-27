// CMDIChildWnd — OpenMFC implementation.
// Sources: frame_font_exports.cpp, wincore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/FrameFontExportsSupport.h"
#include "detail/WincoreSupport.h"

#include <cstddef>

// ---------------------------------------------------------------------------
// Retail layout used by the bodies transcribed below (mfc140u.dll; the
// mfc140.dll ANSI twin carries byte-identical bodies at different RVAs).
//   CMDIChildWnd (retail afxwin.h, class CMDIChildWnd : public CFrameWnd):
//     HMENU m_hMenuShared      +0x1d8   SetHandles `mov %rdx,0x1d8(%rcx)` (0x2a7710)
//     BOOL  m_bPseudoInactive  +0x1e0   OnMDIActivate `movl $0x0,0x1e0(%rcx)` (0x2a7a40);
//                                       also read by MDIGetActive (0x2a6ad0)
//   CFrameWnd members the retail bodies touch:
//     HACCEL m_hAccelTable     +0xf8    SetHandles `mov %r8,0xf8(%rcx)`
//     COleFrameHook* m_pNotifyHook +0x120  OnMDIActivate `mov 0x120(%rdi),%rcx`
//     CView* m_pViewActive     +0x170   OnMDIActivate `mov 0x170(%rdi),%rsi`
//   CMDIFrameWnd::m_hWndMDIClient +0x1d8  UpdateClientEdge `mov 0x1d8(%rbp),%rcx`
// OpenMFC's public CMDIChildWnd declares its two members as one
// `char _mdichild_padding[16]`; mingw folds it into CFrameWnd's tail padding
// so it starts at 0x1d4 and ends at 0x1e4, covering both retail members
// (asserted in the constructor at the bottom of this file).  OpenMFC's
// CFrameWnd declares m_pViewActive at 0xe8 and m_hAccelTable at 0xf0 -- NOT
// their retail offsets -- and OpenMFC's own CFrameWnd code reads and writes
// those named members, so the bodies below use the named members for those
// two (a DEVIATION from the retail offsets, called out at each use).
// m_pNotifyHook is undeclared; retail +0x120 lies inside OpenMFC's
// zero-initialised _framewnd_padding (0xfc..0x1d4) and nothing in OpenMFC
// stores there (core/ole/COleFrameHook.cpp does not install the hook), so it
// is read at the retail offset and is NULL in practice.
//
// Retail vftable slots (mfc140u CMDIChildWnd vftable at 0x18033b678, the table
// whose slot 0 is ?GetRuntimeClass@CMDIChildWnd@@; each slot read out of it):
//   27  (+0x0d8) ?PreCreateWindow@CMDIChildWnd@@
//   107 (+0x358) ?OnUpdateFrameTitle@CMDIChildWnd@@  (CMDIFrameWnd vftable
//                0x18033bb98 slot 107: ?OnUpdateFrameTitle@CMDIFrameWnd@@)
//   115 (+0x398) ?OnUpdateFrameMenu@CMDIChildWnd@@
//   116 (+0x3a0) IsTabbedMDIChild (base body 0x71e0 is `xor %eax,%eax; ret`;
//                the CMDIChildWndEx vftable 0x1802ee018 slot 116 is
//                ?IsTabbedMDIChild@CMDIChildWndEx@@)
//   CView slot 102 (+0x330) ?OnActivateView@CView@@ (CEditView vftable
//                0x1803328c8 and CListView vftable 0x1803321e8, slot 102)
//   COleFrameHook slot 25 (+0x0c8) OnDocActivate (see core/ole/COleFrameHook.cpp)
// All addresses above are mfc140u.
// ---------------------------------------------------------------------------

// Sibling / base-class thunks (signatures from the mangled names; each
// definition was checked at the path given).
extern "C" __int64 MS_ABI impl__Default_CWnd__IEAA_JXZ(CWnd* pThis);                              // core/window/Thunks.cpp
extern "C" unsigned long MS_ABI impl__GetStyle_CWnd__QEBAKXZ(const CWnd* pThis);                 // core/window/Thunks.cpp
extern "C" unsigned long MS_ABI impl__GetExStyle_CWnd__QEBAKXZ(const CWnd* pThis);               // core/window/Thunks.cpp
extern "C" CWnd* MS_ABI impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(HWND hWnd);                // core/window/CWnd.cpp
extern "C" int MS_ABI impl__OnCreateHelper_CFrameWnd__IEAAHPEAUtagCREATESTRUCTW__PEAUCCreateContext___Z(
    CFrameWnd* pThis, CREATESTRUCTW* lpcs, CCreateContext* pContext);                          // core/frame/Thunks.cpp
extern "C" void MS_ABI impl__OnSize_CFrameWnd__IEAAXIHH_Z(
    CFrameWnd* pThis, unsigned int nType, int cx, int cy);                                     // core/frame/Thunks.cpp
extern "C" CMDIFrameWnd* MS_ABI impl__GetMDIFrame_CMDIChildWnd__QEAAPEAVCMDIFrameWnd__XZ(
    CMDIChildWnd* pThis);                                                                      // core/frame/Thunks.cpp
extern "C" CMDIChildWnd* MS_ABI impl__MDIGetActive_CMDIFrameWnd__QEBAPEAVCMDIChildWnd__PEAH_Z(
    const CMDIFrameWnd* pThis, int* pbMaximized);                                              // core/frame/Thunks.cpp
extern "C" void MS_ABI impl__OnUpdateFrameTitle_CMDIFrameWnd__UEAAXH_Z(
    CMDIFrameWnd* pThis, int bAddToTitle);                                                     // core/frame/CMDIFrameWnd.cpp
extern "C" void MS_ABI impl__OnActivateView_CView__MEAAXHPEAV1_0_Z(
    CView* pThis, int bActivate, CView* pActivateView, CView* pDeactiveView);                  // core/view/CView.cpp
extern "C" int MS_ABI impl__OnDocActivate_COleFrameHook__UEAAHH_Z(
    COleFrameHook* pThis, int bActivate);                                                      // core/ole/Thunks.cpp
// This file's own thunks, defined further down.
extern "C" int MS_ABI impl__PreCreateWindow_CMDIChildWnd__UEAAHAEAUtagCREATESTRUCTW___Z(
    CMDIChildWnd* pThis, CREATESTRUCTW& cs);
extern "C" void MS_ABI impl__OnUpdateFrameTitle_CMDIChildWnd__MEAAXH_Z(CMDIChildWnd* pThis, int bAddToTitle);
extern "C" void MS_ABI impl__OnUpdateFrameMenu_CMDIChildWnd__UEAAXHPEAVCWnd__PEAUHMENU_____Z(
    CMDIChildWnd* pThis, int bActive, CWnd* pActivateWnd, HMENU hMenuAlt);
extern "C" int MS_ABI impl__UpdateClientEdge_CMDIChildWnd__IEAAHPEAUtagRECT___Z(
    CMDIChildWnd* pThis, RECT* lpRect);

// The linker-provided base of this DLL's own image (IsOwnObject).
extern "C" IMAGE_DOS_HEADER __ImageBase;

namespace {

constexpr std::size_t kOffMenuShared      = 0x1d8;   // CMDIChildWnd::m_hMenuShared
constexpr std::size_t kOffPseudoInactive  = 0x1e0;   // CMDIChildWnd::m_bPseudoInactive
constexpr std::size_t kOffMDIChildEnd     = 0x1e8;   // retail sizeof(CMDIChildWnd)
constexpr std::size_t kOffNotifyHook      = 0x120;   // CFrameWnd::m_pNotifyHook

constexpr int kSlotPreCreateWindow    = 27;    // +0x0d8
constexpr int kSlotOnUpdateFrameTitle = 107;   // +0x358
constexpr int kSlotOnUpdateFrameMenu  = 115;   // +0x398
constexpr int kSlotIsTabbedMDIChild   = 116;   // +0x3a0
constexpr int kSlotOnActivateView     = 102;   // +0x330 (CView)
constexpr int kSlotOnDocActivate      = 25;    // +0x0c8 (COleFrameHook)

static_assert(offsetof(CWnd, m_hWnd) == 0x40, "retail reads m_hWnd at +0x40");
static_assert(offsetof(CMDIFrameWnd, m_hWndMDIClient) == 0x1d8, "retail CMDIFrameWnd::m_hWndMDIClient");
static_assert(offsetof(CFrameWnd, m_nIDHelp) + sizeof(UINT) <= kOffNotifyHook &&
              kOffNotifyHook + sizeof(void*) <= sizeof(CFrameWnd),
              "retail CFrameWnd::m_pNotifyHook (+0x120) must lie inside OpenMFC's _framewnd_padding");

template <typename T>
inline T& At(void* pObject, std::size_t off) {
    return *reinterpret_cast<T*>(static_cast<unsigned char*>(pObject) + off);
}

// Virtual dispatch on objects of either origin (the same scheme as
// core/frame/CFrameImpl.cpp and core/dialog/CDialog.cpp): an object a CLIENT
// built carries an MSVC vftable in the client's image, where retail's slot
// numbers are exact, so the call goes through the slot as retail's does; an
// object THIS DLL built carries g++'s Itanium-shaped vtable, where those slot
// numbers mean something else, so it is handed to the sibling impl__ thunk for
// the retail base implementation instead (a DEVIATION: an OpenMFC-side
// override in a g++-built derived class is not reached).
bool IsOwnObject(const void* pObject) {
    const void* vptr = *static_cast<const void* const*>(pObject);
    const unsigned char* base = reinterpret_cast<const unsigned char*>(&__ImageBase);
    const IMAGE_NT_HEADERS* nt = reinterpret_cast<const IMAGE_NT_HEADERS*>(
        base + reinterpret_cast<const IMAGE_DOS_HEADER*>(base)->e_lfanew);
    const unsigned char* p = static_cast<const unsigned char*>(vptr);
    return p >= base && p < base + nt->OptionalHeader.SizeOfImage;
}

template <typename Fn>
inline Fn VSlot(const void* pObject, int nSlot) {
    return reinterpret_cast<Fn>((*static_cast<void* const* const*>(pObject))[nSlot]);
}

using PreCreateWindowFn    = int (MS_ABI*)(void* pThis, CREATESTRUCTW& cs);
using OnUpdateFrameTitleFn = void (MS_ABI*)(void* pThis, int bAddToTitle);
using OnUpdateFrameMenuFn  = void (MS_ABI*)(void* pThis, int bActive, CWnd* pActivateWnd, HMENU hMenuAlt);
using IsTabbedMDIChildFn   = int (MS_ABI*)(void* pThis);
using OnActivateViewFn     = void (MS_ABI*)(void* pThis, int bActivate, CView* pActivateView, CView* pDeactiveView);
using OnDocActivateFn      = int (MS_ABI*)(void* pThis, int bActivate);

int VPreCreateWindow(CMDIChildWnd* pWnd, CREATESTRUCTW& cs) {
    if (IsOwnObject(pWnd)) {
        return impl__PreCreateWindow_CMDIChildWnd__UEAAHAEAUtagCREATESTRUCTW___Z(pWnd, cs);
    }
    return VSlot<PreCreateWindowFn>(pWnd, kSlotPreCreateWindow)(pWnd, cs);
}

void VChildOnUpdateFrameTitle(CMDIChildWnd* pWnd, int bAddToTitle) {
    if (IsOwnObject(pWnd)) {
        impl__OnUpdateFrameTitle_CMDIChildWnd__MEAAXH_Z(pWnd, bAddToTitle);
        return;
    }
    VSlot<OnUpdateFrameTitleFn>(pWnd, kSlotOnUpdateFrameTitle)(pWnd, bAddToTitle);
}

void VFrameOnUpdateFrameTitle(CMDIFrameWnd* pFrame, int bAddToTitle) {
    if (IsOwnObject(pFrame)) {
        impl__OnUpdateFrameTitle_CMDIFrameWnd__UEAAXH_Z(pFrame, bAddToTitle);
        return;
    }
    VSlot<OnUpdateFrameTitleFn>(pFrame, kSlotOnUpdateFrameTitle)(pFrame, bAddToTitle);
}

void VOnUpdateFrameMenu(CMDIChildWnd* pWnd, int bActive, CWnd* pActivateWnd, HMENU hMenuAlt) {
    if (IsOwnObject(pWnd)) {
        impl__OnUpdateFrameMenu_CMDIChildWnd__UEAAXHPEAVCWnd__PEAUHMENU_____Z(pWnd, bActive, pActivateWnd, hMenuAlt);
        return;
    }
    VSlot<OnUpdateFrameMenuFn>(pWnd, kSlotOnUpdateFrameMenu)(pWnd, bActive, pActivateWnd, hMenuAlt);
}

// IsTabbedMDIChild is an inline virtual in retail afxwin.h (`return FALSE;`)
// with no export of its own for CMDIChildWnd, so a g++-built object gets the
// base answer FALSE (DEVIATION: a g++-built CMDIChildWndEx is not asked).
int VIsTabbedMDIChild(CMDIChildWnd* pWnd) {
    if (IsOwnObject(pWnd)) {
        return FALSE;
    }
    return VSlot<IsTabbedMDIChildFn>(pWnd, kSlotIsTabbedMDIChild)(pWnd);
}

void VOnActivateView(CView* pView, int bActivate, CView* pActivateView, CView* pDeactiveView) {
    if (IsOwnObject(pView)) {
        impl__OnActivateView_CView__MEAAXHPEAV1_0_Z(pView, bActivate, pActivateView, pDeactiveView);
        return;
    }
    VSlot<OnActivateViewFn>(pView, kSlotOnActivateView)(pView, bActivate, pActivateView, pDeactiveView);
}

int VOnDocActivate(COleFrameHook* pHook, int bActivate) {
    if (IsOwnObject(pHook)) {
        return impl__OnDocActivate_COleFrameHook__UEAAHH_Z(pHook, bActivate);
    }
    return VSlot<OnDocActivateFn>(pHook, kSlotOnDocActivate)(pHook, bActivate);
}

} // namespace

// Symbol: ?CreateObject@CMDIChildWnd@@SAPEAVCObject@@XZ
extern "C" CObject* MS_ABI impl__CreateObject_CMDIChildWnd__SAPEAVCObject__XZ() {
    return CMDIChildWnd::CreateObject();
}
// Symbol: ?DefWindowProcW@CMDIChildWnd@@MEAA_JI_K_J@Z
extern "C" LRESULT MS_ABI impl__DefWindowProcW_CMDIChildWnd__MEAA_JI_K_J_Z(
    CMDIChildWnd* pThis, UINT message, WPARAM wParam, LPARAM lParam) {
    return impl__DefWindowProcW_CWnd__MEAA_JI_K_J_Z(pThis, message, wParam, lParam);
}
// Symbol: ?GetMessageBar@CMDIChildWnd@@MEAAPEAVCWnd@@XZ
extern "C" CWnd* MS_ABI impl__GetMessageBar_CMDIChildWnd__MEAAPEAVCWnd__XZ(CMDIChildWnd* pThis) {
    return pThis ? pThis->CFrameWnd::GetMessageBar() : nullptr;
}
// Symbol: ?GetMessageMap@CMDIChildWnd@@MEBAPEBUAFX_MSGMAP@@XZ
extern "C" const AFX_MSGMAP* MS_ABI impl__GetMessageMap_CMDIChildWnd__MEBAPEBUAFX_MSGMAP__XZ(
    const CMDIChildWnd* pThis) {
    (void)pThis;
    return EmptyMessageMap_FrameFontExports();
}
// Symbol: ?GetRuntimeClass@CMDIChildWnd@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CMDIChildWnd__UEBAPEAUCRuntimeClass__XZ(
    const CMDIChildWnd* pThis) {
    return CMDIChildWnd::GetThisClass();
}
// Symbol: ?GetThisClass@CMDIChildWnd@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMDIChildWnd__SAPEAUCRuntimeClass__XZ() {
    return CMDIChildWnd::GetThisClass();
}
// Symbol: ?GetThisMessageMap@CMDIChildWnd@@KAPEBUAFX_MSGMAP@@XZ
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CMDIChildWnd__KAPEBUAFX_MSGMAP__XZ() {
    return EmptyMessageMap_FrameFontExports();
}
// Symbol: ?GetTrackingID@CMDIChildWnd@@UEAAIXZ
extern "C" UINT MS_ABI impl__GetTrackingID_CMDIChildWnd__UEAAIXZ(CMDIChildWnd* pThis) {
    return pThis && pThis->m_hWnd ? (UINT)::GetWindowLongPtrW(pThis->m_hWnd, GWLP_ID) : 0;
}
// Symbol: ?LoadFrame@CMDIChildWnd@@UEAAHIKPEAVCWnd@@PEAUCCreateContext@@@Z
extern "C" int MS_ABI impl__LoadFrame_CMDIChildWnd__UEAAHIKPEAVCWnd__PEAUCCreateContext___Z(
    CMDIChildWnd* pThis, UINT nIDResource, DWORD dwDefaultStyle, CWnd* pParentWnd, CCreateContext* pContext) {
    (void)nIDResource;
    CMDIFrameWnd* pFrame = dynamic_cast<CMDIFrameWnd*>(pParentWnd);
    RECT rect = {CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT};
    return pThis && pFrame ? pThis->Create(nullptr, L"", dwDefaultStyle, rect, pFrame, pContext) : FALSE;
}
// Symbol: ?OnCreate@CMDIChildWnd@@IEAAHPEAUtagCREATESTRUCTW@@@Z
// Transcribed from RVA 0x2a7ca0 (mfc140u), three instructions:
//     mov (%rdx),%r8          ; lpCreateStruct->lpCreateParams (the MDICREATESTRUCTW*)
//     mov 0x30(%r8),%r8       ; MDICREATESTRUCTW::lParam -- the CCreateContext*
//     jmp 0x29dba0            ; ?OnCreateHelper@CFrameWnd@@ (mfc140u), tail call
// i.e. `return OnCreateHelper(lpcs, (CCreateContext*)((MDICREATESTRUCT*)
// lpcs->lpCreateParams)->lParam);`.  Create (below) stores pContext in that
// lParam, as retail's Create does.
// DEVIATION: retail dereferences lpCreateParams unconditionally; a NULL one is
// passed on as a NULL context here instead of faulting.
// Note: OpenMFC's CFrameWnd::OnCreateHelper currently returns TRUE/FALSE, not
// retail's 0 / -1 (RVA 0x29dba0 returns 0 on success, -1 on failure); the value
// is passed through unchanged, as retail's tail jump does.
extern "C" int MS_ABI impl__OnCreate_CMDIChildWnd__IEAAHPEAUtagCREATESTRUCTW___Z(
    CMDIChildWnd* pThis, CREATESTRUCTW* pCreateStruct) {
    const MDICREATESTRUCTW* pMDICreate =
        static_cast<const MDICREATESTRUCTW*>(pCreateStruct->lpCreateParams);
    CCreateContext* pContext =
        pMDICreate != nullptr ? reinterpret_cast<CCreateContext*>(pMDICreate->lParam) : nullptr;
    return impl__OnCreateHelper_CFrameWnd__IEAAHPEAUtagCREATESTRUCTW__PEAUCCreateContext___Z(
        pThis, pCreateStruct, pContext);
}
// Symbol: ?OnDestroy@CMDIChildWnd@@IEAAXXZ
extern "C" void MS_ABI impl__OnDestroy_CMDIChildWnd__IEAAXXZ(CMDIChildWnd* pThis) {
    if (pThis) pThis->m_hWnd = nullptr;
}
// Symbol: ?OnMDIActivate@CMDIChildWnd@@IEAAXHPEAVCWnd@@0@Z
// Transcribed from RVA 0x2a7a40 (mfc140u; the same bytes are at 0x2a5960 in
// mfc140.dll).  Retail, in order:
//     m_bPseudoInactive (+0x1e0) = FALSE;
//     UpdateClientEdge(NULL);                                  // call 0x2a72f0
//     CView* pActiveView = m_pViewActive (+0x170);
//     if (!bActivate && pActiveView != NULL)
//         pActiveView->OnActivateView(FALSE, pActiveView, pActiveView);   // slot 102
//     BOOL bHooked = m_pNotifyHook (+0x120) != NULL
//                 && m_pNotifyHook->OnDocActivate(bActivate);  // slot 25
//     if (!bHooked)
//         OnUpdateFrameTitle(bActivate || pActivateWnd != NULL);  // slot 107
//     if (bActivate && pActiveView != NULL &&
//         CWnd::FromHandle(::GetActiveWindow()) == GetMDIFrame())    // 0x28ad70, 0x2a7870
//         pActiveView->OnActivateView(TRUE, pActiveView, pActiveView);
//     if (!bHooked) {
//         OnUpdateFrameMenu(bActivate, pActivateWnd, NULL);    // slot 115
//         ::DrawMenuBar(GetMDIFrame()->m_hWnd);
//     }
// The third argument (pDeactivateWnd) is never read.  IAT slots resolved in
// mfc140u: 0x1802c6e60 GetActiveWindow, 0x1802c70b0 DrawMenuBar.
// DEVIATIONS: m_pViewActive is read from OpenMFC's named member (+0xe8, where
// OpenMFC's CFrameWnd keeps the active view) rather than retail +0x170; and
// ::DrawMenuBar is skipped when GetMDIFrame() is NULL (retail dereferences it
// unconditionally and faults).  Both GetMDIFrames can return NULL: retail's
// (0x2a7870) is CWnd::FromHandle(::GetParent(::GetParent(m_hWnd))), and
// CHandleMap::FromHandle (0x2a6080) returns NULL for a NULL handle; OpenMFC's
// additionally returns NULL when its dynamic_cast to CMDIFrameWnd fails.
extern "C" void MS_ABI impl__OnMDIActivate_CMDIChildWnd__IEAAXHPEAVCWnd__0_Z(
    CMDIChildWnd* pThis, int bActivate, CWnd* pActivateWnd, CWnd* pDeactivateWnd) {
    (void)pDeactivateWnd;
    At<BOOL>(pThis, kOffPseudoInactive) = FALSE;
    impl__UpdateClientEdge_CMDIChildWnd__IEAAHPEAUtagRECT___Z(pThis, nullptr);

    CView* pActiveView = reinterpret_cast<CView*>(pThis->m_pViewActive);
    if (!bActivate && pActiveView != nullptr) {
        VOnActivateView(pActiveView, FALSE, pActiveView, pActiveView);
    }

    BOOL bHooked = FALSE;
    COleFrameHook* pHook = At<COleFrameHook*>(pThis, kOffNotifyHook);
    if (pHook != nullptr && VOnDocActivate(pHook, bActivate) != 0) {
        bHooked = TRUE;
    }

    if (!bHooked) {
        VChildOnUpdateFrameTitle(pThis, (bActivate || pActivateWnd != nullptr) ? TRUE : FALSE);
    }

    if (bActivate && pActiveView != nullptr) {
        CWnd* pActiveWnd = impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(::GetActiveWindow());
        if (pActiveWnd == impl__GetMDIFrame_CMDIChildWnd__QEAAPEAVCMDIFrameWnd__XZ(pThis)) {
            VOnActivateView(pActiveView, TRUE, pActiveView, pActiveView);
        }
    }

    if (!bHooked) {
        VOnUpdateFrameMenu(pThis, bActivate, pActivateWnd, nullptr);
        CMDIFrameWnd* pFrame = impl__GetMDIFrame_CMDIChildWnd__QEAAPEAVCMDIFrameWnd__XZ(pThis);
        if (pFrame != nullptr) {
            ::DrawMenuBar(pFrame->m_hWnd);
        }
    }
}
// Symbol: ?OnMouseActivate@CMDIChildWnd@@IEAAHPEAVCWnd@@II@Z
extern "C" int MS_ABI impl__OnMouseActivate_CMDIChildWnd__IEAAHPEAVCWnd__II_Z(
    CMDIChildWnd* pThis, CWnd* pDesktopWnd, UINT nHitTest, UINT message) {
    (void)pThis;
    (void)pDesktopWnd;
    (void)nHitTest;
    (void)message;
    return MA_ACTIVATE;
}
// Symbol: ?OnNcActivate@CMDIChildWnd@@IEAAHH@Z
// The export (ordinal lookup in mfc_complete_ordinal_mapping.json + the mfc140u
// export address table) lands on RVA 0xda30 (mfc140u), a body shared by
// identical-COMDAT folding with other exports: a single `jmp 0x28ac80`, which is
// ?Default@CWnd@@IEAA_JXZ (mfc140u).  So this bypasses CFrameWnd::OnNcActivate
// and returns CWnd::Default()'s result (truncated to int, as retail's is).
extern "C" int MS_ABI impl__OnNcActivate_CMDIChildWnd__IEAAHH_Z(CMDIChildWnd* pThis, int bActive) {
    (void)bActive;
    return static_cast<int>(impl__Default_CWnd__IEAA_JXZ(pThis));
}
// Symbol: ?OnNcCreate@CMDIChildWnd@@IEAAHPEAUtagCREATESTRUCTW@@@Z
// Transcribed from RVA 0x2a7c40 (mfc140u):
//     if ((BOOL)Default() == 0) return 0;                    // call 0x28ac80, ?Default@CWnd@@
//     PreCreateWindow(*lpCreateStruct);                      // vftable slot 27, result ignored
//     ::SetWindowLongW(m_hWnd, GWL_EXSTYLE, lpCreateStruct->dwExStyle);   // `mov 0x48(%rdi),%r8d`
//     return TRUE;
// (IAT slot 0x1802c6eb0 resolves to SetWindowLongW in mfc140u.)
extern "C" int MS_ABI impl__OnNcCreate_CMDIChildWnd__IEAAHPEAUtagCREATESTRUCTW___Z(
    CMDIChildWnd* pThis, CREATESTRUCTW* pCreateStruct) {
    const int nDefault = static_cast<int>(impl__Default_CWnd__IEAA_JXZ(pThis));
    if (nDefault == 0) {
        return nDefault;
    }
    (void)VPreCreateWindow(pThis, *pCreateStruct);
    ::SetWindowLong(pThis->m_hWnd, GWL_EXSTYLE, static_cast<LONG>(pCreateStruct->dwExStyle));
    return TRUE;
}
// Symbol: ?OnSize@CMDIChildWnd@@IEAAXIHH@Z
// Transcribed from RVA 0x2a72b0 (mfc140u):
//     CFrameWnd::OnSize(nType, cx, cy);                      // call 0x2a0450, args passed through
//     GetMDIFrame()->OnUpdateFrameTitle(TRUE);               // call 0x2a7870, then a tail jump
//                                                            // through the frame's vftable slot 107
// DEVIATION: retail dereferences GetMDIFrame()'s result unconditionally (and
// faults when it is NULL, e.g. no grandparent window); a NULL frame is skipped
// here.  See OnMDIActivate for when either GetMDIFrame returns NULL.
extern "C" void MS_ABI impl__OnSize_CMDIChildWnd__IEAAXIHH_Z(
    CMDIChildWnd* pThis, UINT nType, int cx, int cy) {
    impl__OnSize_CFrameWnd__IEAAXIHH_Z(pThis, nType, cx, cy);
    CMDIFrameWnd* pFrame = impl__GetMDIFrame_CMDIChildWnd__QEAAPEAVCMDIFrameWnd__XZ(pThis);
    if (pFrame != nullptr) {
        VFrameOnUpdateFrameTitle(pFrame, TRUE);
    }
}
// Symbol: ?OnToolTipText@CMDIChildWnd@@IEAAHIPEAUtagNMHDR@@PEA_J@Z
extern "C" int MS_ABI impl__OnToolTipText_CMDIChildWnd__IEAAHIPEAUtagNMHDR__PEA_J_Z(
    CMDIChildWnd* pThis, UINT id, NMHDR* pNMHDR, LRESULT* pResult) {
    (void)pThis;
    (void)id;
    (void)pNMHDR;
    if (pResult) *pResult = 0;
    return FALSE;
}
// Symbol: ?OnUpdateFrameMenu@CMDIChildWnd@@UEAAXHPEAVCWnd@@PEAUHMENU__@@@Z
extern "C" void MS_ABI impl__OnUpdateFrameMenu_CMDIChildWnd__UEAAXHPEAVCWnd__PEAUHMENU_____Z(
    CMDIChildWnd* pThis, int bActive, CWnd* pActivateWnd, HMENU hMenuAlt) {
    (void)bActive;
    (void)pActivateWnd;
    if (pThis) {
        pThis->CFrameWnd::OnUpdateFrameMenu(hMenuAlt);
    }
}
// Symbol: ?OnUpdateFrameTitle@CMDIChildWnd@@MEAAXH@Z
extern "C" void MS_ABI impl__OnUpdateFrameTitle_CMDIChildWnd__MEAAXH_Z(CMDIChildWnd* pThis, int bAddToTitle) {
    if (pThis) {
        pThis->CFrameWnd::OnUpdateFrameTitle(bAddToTitle);
    }
}
// Symbol: ?OnWindowPosChanging@CMDIChildWnd@@IEAAXPEAUtagWINDOWPOS@@@Z
extern "C" void MS_ABI impl__OnWindowPosChanging_CMDIChildWnd__IEAAXPEAUtagWINDOWPOS___Z(
    CMDIChildWnd* pThis, WINDOWPOS* lpWndPos) {
    if (pThis && lpWndPos) {
        if (lpWndPos->x < 0) lpWndPos->x = 0;
        if (lpWndPos->y < 0) lpWndPos->y = 0;
        if (lpWndPos->cx < 0) lpWndPos->cx = 0;
        if (lpWndPos->cy < 0) lpWndPos->cy = 0;
    }
}
// Symbol: ?PreCreateWindow@CMDIChildWnd@@UEAAHAEAUtagCREATESTRUCTW@@@Z
extern "C" int MS_ABI impl__PreCreateWindow_CMDIChildWnd__UEAAHAEAUtagCREATESTRUCTW___Z(
    CMDIChildWnd* pThis, CREATESTRUCTW& cs) {
    return impl__PreCreateWindow_CFrameWnd__MEAAHAEAUtagCREATESTRUCTW___Z(pThis, cs);
}
// Symbol: ?PreTranslateMessage@CMDIChildWnd@@UEAAHPEAUtagMSG@@@Z
extern "C" int MS_ABI impl__PreTranslateMessage_CMDIChildWnd__UEAAHPEAUtagMSG___Z(
    CMDIChildWnd* pThis, MSG* pMsg) {
    return impl__PreTranslateMessage_CFrameWnd__UEAAHPEAUtagMSG___Z(pThis, pMsg);
}
// Symbol: ?SetHandles@CMDIChildWnd@@QEAAXPEAUHMENU__@@PEAUHACCEL__@@@Z
// Transcribed from RVA 0x2a7710 (mfc140u), two stores and a ret:
//     m_hMenuShared (+0x1d8) = hMenu;  m_hAccelTable (+0xf8) = hAccel;
// DEVIATION: m_hAccelTable is written through OpenMFC's named CFrameWnd member
// (+0xf0), which is what OpenMFC's CFrameWnd::PreTranslateMessage translates
// with; retail +0xf8 is OpenMFC's m_nIDHelp and must not be overwritten.
extern "C" void MS_ABI impl__SetHandles_CMDIChildWnd__QEAAXPEAUHMENU____PEAUHACCEL_____Z(
    CMDIChildWnd* pThis, HMENU hMenu, HACCEL hAccel) {
    At<HMENU>(pThis, kOffMenuShared) = hMenu;
    pThis->m_hAccelTable = hAccel;
}
// Symbol: ?UpdateClientEdge@CMDIChildWnd@@IEAAHPEAUtagRECT@@@Z
// Transcribed from RVA 0x2a72f0 (mfc140u; the same bytes are at 0x2a5210 in
// mfc140.dll):
//     CMDIFrameWnd* pFrameWnd = GetMDIFrame();                         // 0x2a7870
//     CMDIChildWnd* pChild = pFrameWnd->MDIGetActive(NULL);            // 0x2a6ad0
//     BOOL bTabbed = pChild != NULL && pChild->IsTabbedMDIChild();     // slot 116
//     if ((pChild == NULL || pChild == this) && !bTabbed) {
//         HWND hClient = pFrameWnd->m_hWndMDIClient;                   // +0x1d8
//         DWORD dwStyle = ::GetWindowLongW(hClient, GWL_EXSTYLE);
//         DWORD dwNewStyle = dwStyle;
//         if (pChild != NULL && !(GetExStyle() & WS_EX_CLIENTEDGE)     // 0x2a96c0, bit 9
//                            && (GetStyle() & WS_MAXIMIZE))            // 0x2a9690, bit 24
//             dwNewStyle &= ~WS_EX_CLIENTEDGE;
//         else
//             dwNewStyle |= WS_EX_CLIENTEDGE;
//         if (dwStyle != dwNewStyle) {
//             ::RedrawWindow(hClient, NULL, NULL, 0x81);   // RDW_INVALIDATE | RDW_ALLCHILDREN
//             ::SetWindowLongW(hClient, GWL_EXSTYLE, dwNewStyle);
//             ::SetWindowPos(hClient, NULL, 0, 0, 0, 0, 0x137);
//                 // SWP_NOSIZE|NOMOVE|NOZORDER|NOACTIVATE|FRAMECHANGED|NOCOPYBITS
//             if (lpRect != NULL) ::GetClientRect(hClient, lpRect);
//             return TRUE;
//         }
//     }
//     return FALSE;
// IAT slots resolved in mfc140u: 0x1802c71e8 GetWindowLongW, 0x1802c7130
// RedrawWindow, 0x1802c6eb0 SetWindowLongW, 0x1802c6c90 SetWindowPos,
// 0x1802c7330 GetClientRect.  hClient is re-read from the frame before every
// call in retail; it does not change in between, so it is read once here.
// DEVIATION: a NULL GetMDIFrame() returns FALSE here; retail dereferences it.
extern "C" int MS_ABI impl__UpdateClientEdge_CMDIChildWnd__IEAAHPEAUtagRECT___Z(
    CMDIChildWnd* pThis, RECT* lpRect) {
    CMDIFrameWnd* pFrameWnd = impl__GetMDIFrame_CMDIChildWnd__QEAAPEAVCMDIFrameWnd__XZ(pThis);
    if (pFrameWnd == nullptr) {
        return FALSE;
    }
    CMDIChildWnd* pChild = impl__MDIGetActive_CMDIFrameWnd__QEBAPEAVCMDIChildWnd__PEAH_Z(pFrameWnd, nullptr);
    const BOOL bTabbed = (pChild != nullptr && VIsTabbedMDIChild(pChild) != 0) ? TRUE : FALSE;
    if ((pChild != nullptr && pChild != pThis) || bTabbed) {
        return FALSE;
    }

    const HWND hClient = pFrameWnd->m_hWndMDIClient;
    const DWORD dwStyle = static_cast<DWORD>(::GetWindowLong(hClient, GWL_EXSTYLE));
    DWORD dwNewStyle = dwStyle;
    if (pChild != nullptr &&
        (impl__GetExStyle_CWnd__QEBAKXZ(pThis) & WS_EX_CLIENTEDGE) == 0 &&
        (impl__GetStyle_CWnd__QEBAKXZ(pThis) & WS_MAXIMIZE) != 0) {
        dwNewStyle &= ~static_cast<DWORD>(WS_EX_CLIENTEDGE);
    } else {
        dwNewStyle |= WS_EX_CLIENTEDGE;
    }
    if (dwStyle == dwNewStyle) {
        return FALSE;
    }

    ::RedrawWindow(hClient, nullptr, nullptr, RDW_INVALIDATE | RDW_ALLCHILDREN);
    ::SetWindowLong(hClient, GWL_EXSTYLE, static_cast<LONG>(dwNewStyle));
    ::SetWindowPos(hClient, nullptr, 0, 0, 0, 0,
                   SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE |
                   SWP_FRAMECHANGED | SWP_NOCOPYBITS);
    if (lpRect != nullptr) {
        ::GetClientRect(hClient, lpRect);
    }
    return TRUE;
}
CMDIChildWnd::CMDIChildWnd() {
    // Retail m_hMenuShared (+0x1d8) and m_bPseudoInactive (+0x1e0..+0x1e4) must
    // lie inside _mdichild_padding (which mingw starts at 0x1d4, see the file
    // header), so the memset below clears both.
    static_assert(offsetof(CMDIChildWnd, _mdichild_padding) <= kOffMenuShared,
                  "retail m_hMenuShared (+0x1d8) must lie inside _mdichild_padding");
    static_assert(offsetof(CMDIChildWnd, _mdichild_padding) + sizeof(_mdichild_padding) >=
                      kOffPseudoInactive + sizeof(BOOL),
                  "retail m_bPseudoInactive (+0x1e0) must lie inside _mdichild_padding");
    static_assert(sizeof(CMDIChildWnd) == kOffMDIChildEnd, "retail sizeof(CMDIChildWnd) is 0x1e8");
    memset(_mdichild_padding, 0, sizeof(_mdichild_padding));
}
int CMDIChildWnd::Create(const wchar_t* lpszClassName, const wchar_t* lpszWindowName,
                         DWORD dwStyle, const struct tagRECT& rect,
                         CMDIFrameWnd* pParentWnd, CCreateContext* pContext) {
    (void)lpszClassName;

    if (!pParentWnd || !pParentWnd->m_hWndMDIClient) {
        return FALSE;
    }

    MDICREATESTRUCTW mcs = {};
    mcs.szClass = lpszClassName ? lpszClassName : L"MDICHILD";
    mcs.szTitle = lpszWindowName;
    mcs.hOwner = AfxGetInstanceHandle();
    mcs.x = rect.left ? rect.left : CW_USEDEFAULT;
    mcs.y = rect.top ? rect.top : CW_USEDEFAULT;
    mcs.cx = (rect.right - rect.left) ? (rect.right - rect.left) : CW_USEDEFAULT;
    mcs.cy = (rect.bottom - rect.top) ? (rect.bottom - rect.top) : CW_USEDEFAULT;
    mcs.style = dwStyle ? dwStyle : (WS_CHILD | WS_VISIBLE | WS_OVERLAPPEDWINDOW);
    // Retail Create (RVA 0x2a6f10, mfc140u) builds a CREATESTRUCT whose
    // lpCreateParams is pContext and copies that into MDICREATESTRUCT::lParam;
    // OnCreate reads the CCreateContext* back from there.
    mcs.lParam = (LPARAM)pContext;

    m_hWnd = (HWND)::SendMessageW(pParentWnd->m_hWndMDIClient, WM_MDICREATE, 0, (LPARAM)&mcs);

    if (m_hWnd) {
        g_hwndMap[m_hWnd] = this;
        return TRUE;
    }

    return FALSE;
}
void CMDIChildWnd::ActivateFrame(int nCmdShow) {
    CMDIFrameWnd* pFrame = GetMDIFrame();
    if (pFrame) {
        pFrame->MDIActivate(this);
        if (nCmdShow != -1) {
            ::ShowWindow(m_hWnd, nCmdShow);
        }
    }
}
int CMDIChildWnd::DestroyWindow() {
    CMDIFrameWnd* pFrame = GetMDIFrame();
    if (pFrame && pFrame->m_hWndMDIClient && m_hWnd) {
        ::SendMessageW(pFrame->m_hWndMDIClient, WM_MDIDESTROY, (WPARAM)m_hWnd, 0);
        m_hWnd = nullptr;
        return TRUE;
    }
    return FALSE;
}
CMDIFrameWnd* CMDIChildWnd::GetMDIFrame() {
    HWND hWndParent = ::GetParent(m_hWnd);  // MDI client
    if (hWndParent) {
        hWndParent = ::GetParent(hWndParent);  // MDI frame
        CWnd* pWnd = CWnd::FromHandle(hWndParent);
        return dynamic_cast<CMDIFrameWnd*>(pWnd);
    }
    return nullptr;
}
