// CControlFrameWnd — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp
//
// CControlFrameWnd is the private frame that COleControl::CreateFrameWindow
// creates for out-of-place ("open") activation.  The shipped afxctl.h only
// names it (as a friend of COleControl and as CreateFrameWindow's return
// type); its class declaration is not in the shipped headers.  It is not
// declared anywhere under include/openmfc, so every thunk below takes a raw
// `void* pThis` and the layout is pinned from the retail constructor
// ??0CControlFrameWnd@@QEAA@PEAVCOleControl@@@Z, RVA 0x2b29d0 (mfc140u):
//     call  CWnd::CWnd                      (0x28a700, mfc140u)
//     mov   %rbx,0xe8(%rdi)                 ; m_pCtrl = pCtrl  (first slot past CWnd)
//     lea   0x18033ce40,%rax ; mov %rax,(%rdi)   ; CControlFrameWnd vftable (mfc140u)
// sizeof(CWnd) == 0xe8 is what OpenMFC asserts elsewhere (e.g.
// featurepack/customize/CMFCToolBarsToolsPropertyPage.cpp), and COleControl.cpp
// records the retail `new CControlFrameWnd` allocation as 0xf0 bytes.
//
// Virtual slots used below were read from that vftable (0x33ce40, mfc140u):
//   slot  1 (+0x08)  scalar deleting destructor
//   slot 25 (+0xc8)  ?CreateEx@CWnd@@UEAAHKPEB_W0KHHHHPEAUHWND__@@PEAUHMENU__@@PEAX@Z
//   slot 26 (+0xd0)  ?DestroyWindow@CWnd@@UEAAHXZ
//   slot 27 (+0xd8)  ?PreCreateWindow@CControlFrameWnd@@UEAAHAEAUtagCREATESTRUCTW@@@Z (0x2b2a10)
//   slot 74 (+0x250) ?PostNcDestroy@CControlFrameWnd@@MEAAXXZ (0x20f410)
//   slot 91 (+0x2d8) ?Create@CControlFrameWnd@@UEAAHPEB_W@Z  (0x2b2a40)
// COleControl is AFX_NOVTABLE in afxctl.h, so the DLL holds no COleControl
// vftable to read; its slot +0x4f8 is pinned instead by the declaration order
// (OnOpen, CreateFrameWindow, ResizeFrameWindow, OnFrameClose) together with
// retail COleControl::OnOpen (RVA 0x1e0e10, mfc140u), which calls +0x4e8 and
// stores the result as the open frame (CreateFrameWindow) and calls +0x4f0
// with (cx, cy) (ResizeFrameWindow) -- so +0x4f8 is OnFrameClose.
//
// Note on the constructor (left as it was): OpenMFC's ctor thunk below still
// runs no CWnd constructor and installs no vftable, and COleControl::
// CreateFrameWindow in OpenMFC returns NULL rather than constructing one, so
// today no OpenMFC code path reaches these methods with a live object.  They
// are transcribed so that a retail-layout object dispatches correctly.

#include "detail/ManualSmallStubImplementationsSupport.h"

namespace {

constexpr std::size_t kCControlFrameWnd_m_pCtrl = 0xe8;   // ctor 0x2b29d0 (mfc140u)
static_assert(sizeof(CWnd) == kCControlFrameWnd_m_pCtrl,
              "m_pCtrl is the first member past the CWnd base");
static_assert(offsetof(CREATESTRUCTW, lpszClass) == 0x40,
              "PreCreateWindow (0x2b2a10, mfc140u) tests cs+0x40");

// The class name OpenMFC's AfxEndDeferRegisterClass registers: it calls
// RegisterOpenMFCClass (detail/WincoreSupport.cpp), which registers
// g_szOpenMFCClass = L"OpenMFC_Window" (detail/WincoreSupport.h).  That header
// is not included here because it references CFrameWnd::classCFrameWnd, a C++
// data symbol this TU must not pull in; keep the two spellings in sync.
constexpr const wchar_t kOpenMFCWndClass[] = L"OpenMFC_Window";

inline void** Vtbl(void* p) { return *static_cast<void***>(p); }
inline void* CtrlOf(void* pThis) {
    return *reinterpret_cast<void**>(static_cast<unsigned char*>(pThis) +
                                     kCControlFrameWnd_m_pCtrl);
}

using CreateExFn = int (MS_ABI*)(void* pThis, unsigned long dwExStyle,
                                 const wchar_t* lpszClassName,
                                 const wchar_t* lpszWindowName,
                                 unsigned long dwStyle, int x, int y, int cx,
                                 int cy, HWND hWndParent, HMENU nIDorHMenu,
                                 void* lpParam);
using ThisOnlyIntFn = int (MS_ABI*)(void* pThis);
using ThisOnlyVoidFn = void (MS_ABI*)(void* pThis);
using DeletingDtorFn = void* (MS_ABI*)(void* pThis, unsigned int flags);

} // namespace

// ?AfxEndDeferRegisterClass@@YAHJ@Z -- defined in featurepack/CMFC_misc_stubs.cpp.
extern "C" int MS_ABI impl__AfxEndDeferRegisterClass__YAHJ_Z(long fToRegister);
// ?Default@CWnd@@IEAA_JXZ -- defined in core/window/Thunks.cpp.
extern "C" __int64 MS_ABI impl__Default_CWnd__IEAA_JXZ(CWnd* pThis);
// ?SetFocus@COleControl@@QEAAPEAVCWnd@@XZ -- a non-static member taking only
// `this`.  Its definition in core/ole/COleControl.cpp is still a generated
// placeholder written with an empty parameter list that returns nullptr; this
// declaration is the correct one (extern "C", so the two link to the same
// symbol, and under MS_ABI the placeholder simply ignores rcx).
extern "C" CWnd* MS_ABI impl__SetFocus_COleControl__QEAAPEAVCWnd__XZ(void* pThis);

// Symbol: ??0CControlFrameWnd@@QEAA@PEAVCOleControl@@@Z
extern "C" void* MS_ABI impl___0CControlFrameWnd__QEAA_PEAVCOleControl___Z(
    void* pThis, void* pControl) {
    (void)pControl;
    return pThis;
}
// Symbol: ?Create@CControlFrameWnd@@UEAAHPEB_W@Z
// Transcribed from RVA 0x2b2a40 (mfc140u):
//     return CreateEx(0, NULL, pszFrameTitle, 0x00C80000 /*WS_CAPTION|WS_SYSMENU*/,
//                     CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
//                     NULL, NULL, NULL) != 0;
// CreateEx is a virtual call through this->vftable +0xc8 (slot 25, the
// x/y/cx/cy overload); the result is normalised with test/setne.
extern "C" int MS_ABI impl__Create_CControlFrameWnd__UEAAHPEB_W_Z(
    void* pThis, const wchar_t* pszFrameTitle) {
    const int ok = reinterpret_cast<CreateExFn>(Vtbl(pThis)[0xc8 / 8])(
        pThis, 0, nullptr, pszFrameTitle, 0x00C80000u,
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
        nullptr, nullptr, nullptr);
    return ok != 0 ? TRUE : FALSE;
}

// Symbol: ?OnActivate@CControlFrameWnd@@IEAAXIPEAVCWnd@@H@Z
// Transcribed from RVA 0x2b2af0 (mfc140u): two calls, both unconditional, the
// arguments unused and no NULL test on m_pCtrl:
//     CWnd::Default();                 // call 0x28ac80 = ?Default@CWnd@@IEAA_JXZ
//     m_pCtrl->SetFocus();             // tail jmp 0x1e6690 = ?SetFocus@COleControl@@QEAAPEAVCWnd@@XZ
// The SetFocus half currently reaches OpenMFC's placeholder COleControl::
// SetFocus (a no-op returning nullptr, see the declaration above); it starts
// working unchanged once that thunk is implemented.
extern "C" void MS_ABI impl__OnActivate_CControlFrameWnd__IEAAXIPEAVCWnd__H_Z(
    void* pThis, unsigned int nState, void* pWndOther, int bMinimized) {
    (void)nState; (void)pWndOther; (void)bMinimized;
    (void)impl__Default_CWnd__IEAA_JXZ(static_cast<CWnd*>(pThis));
    (void)impl__SetFocus_COleControl__QEAAPEAVCWnd__XZ(CtrlOf(pThis));
}

// Symbol: ?OnClose@CControlFrameWnd@@IEAAXXZ
// Transcribed from RVA 0x2b2ab0 (mfc140u), both calls unconditional and with
// no NULL test on m_pCtrl:
//     m_pCtrl->OnFrameClose();         // m_pCtrl (+0xe8) vftable +0x4f8
//     DestroyWindow();                 // this vftable +0xd0 (slot 26), tail jmp
extern "C" void MS_ABI impl__OnClose_CControlFrameWnd__IEAAXXZ(void* pThis) {
    void* pCtrl = CtrlOf(pThis);
    reinterpret_cast<ThisOnlyVoidFn>(Vtbl(pCtrl)[0x4f8 / 8])(pCtrl);
    reinterpret_cast<ThisOnlyIntFn>(Vtbl(pThis)[0xd0 / 8])(pThis);
}

// Symbol: ?PostNcDestroy@CControlFrameWnd@@MEAAXXZ
// Resolved through the mfc140u ordinal table to RVA 0x20f410 (mfc140u), a body
// ICF-folded with CFrameWnd/CView/CFindReplaceDialog::PostNcDestroy; it is also
// slot 74 of the CControlFrameWnd vftable.  It is `delete this`:
//     if (this) this->vftable[+0x08](this, 1);   // scalar deleting destructor
extern "C" void MS_ABI impl__PostNcDestroy_CControlFrameWnd__MEAAXXZ(void* pThis) {
    if (pThis) {
        reinterpret_cast<DeletingDtorFn>(Vtbl(pThis)[0x08 / 8])(pThis, 1);
    }
}

// Symbol: ?PreCreateWindow@CControlFrameWnd@@UEAAHAEAUtagCREATESTRUCTW@@@Z
// Resolved through the mfc140u ordinal table to RVA 0x2b2a10 (mfc140u); body:
//     AfxEndDeferRegisterClass(1 /*AFX_WND_REG*/);  // call 0x2918f0; result ignored
//     if (cs.lpszClass == NULL)                      // CREATESTRUCTW +0x40
//         cs.lpszClass = <0x34fe80, L"AfxWnd140u">;
//     return TRUE;
// Deviation: OpenMFC's AfxEndDeferRegisterClass registers a single class named
// g_szOpenMFCClass (detail/WincoreSupport.h; kOpenMFCWndClass above), not
// "AfxWnd140u", so that is the name stored here: nothing in OpenMFC registers
// "AfxWnd140u", so CreateWindowExW would reject the retail name.  (OpenMFC's
// own CWnd::CreateEx calls CWnd::PreCreateWindow directly, not this override.)
extern "C" int MS_ABI impl__PreCreateWindow_CControlFrameWnd__UEAAHAEAUtagCREATESTRUCTW___Z(
    void* pThis, CREATESTRUCTW& cs) {
    (void)pThis;
    impl__AfxEndDeferRegisterClass__YAHJ_Z(1);
    if (cs.lpszClass == nullptr) {
        cs.lpszClass = kOpenMFCWndClass;
    }
    return TRUE;
}
