// CMFCPreviewCtrlImpl — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp
//
// Every body in this file is transcribed from the retail mfc140u.dll
// disassembly.  The class (shipping atlmfc/include/afxwin.h:3951, read on this
// host) is the Rich-Preview host window: `class CMFCPreviewCtrlImpl : public
// CWnd`, with members m_clrBackColor, m_clrTextColor, m_font (CFont) and
// m_pDocument (ATL::IDocument*), declared in that order.
//
// ---------------------------------------------------------------------------
// Retail layout, transcribed from the constructor ??0CMFCPreviewCtrlImpl@@QEAA@XZ
// (entry RVA 0x129260, mfc140u) and cross-checked against the destructor
// (entry RVA 0x129300, mfc140u) and SetPreviewVisuals (0x129610, mfc140u):
//   +0x000  CWnd base (0xe8 bytes; the ctor calls ??0CWnd@@QEAA@XZ, 0x28a700
//           mfc140u, then stores the class vftable 0x18030e2b8 (mfc140u))
//   +0x040  CWnd::m_hWnd            (every body below reads the HWND there)
//   +0x0e8  COLORREF m_clrBackColor  ctor: one 8-byte store of 0x00ffffff
//   +0x0ec  COLORREF m_clrTextColor    covers both -> back = white, text = 0
//   +0x0f0  CFont    m_font          ctor stores the CFont vftable 0x1802e8ad0
//                                    (mfc140u) at +0xf0 and NULL at +0xf8
//                                    (m_font.m_hObject) -- inlined CFont ctor
//   +0x100  ATL::IDocument* m_pDocument   NOT written by the retail ctor
//   sizeof == 0x108 (264; matches the MFC_MISC_CTRLS_DESC object size used by
//   featurepack/controls/RuntimeClasses.cpp)
//
// ---------------------------------------------------------------------------
// Where the unnamed bodies live.  mfc140u_rva_symbols.json resolves only the
// ctor, dtor, both Creates, Destroy, Focus, GetRuntimeClass and GetMessageMap.
// The rest were located from retail's own tables and compared instruction by
// instruction against the bodies mfc140.dll (ANSI) names -- identical except
// for rip-relative / call displacements (the images are laid out differently),
// and except where noted:
//   * the vftable 0x18030e2b8 (mfc140u), dumped slot by slot:
//       +0x2d8 slot 91  Create(HWND, const RECT*, CCreateContext*)  0x129370
//       +0x2e0 slot 92  Create(HWND, const RECT*)                   0x129350
//       +0x2e8 slot 93  SetPreviewVisuals                           0x129610
//       +0x2f0 slot 94  Destroy                                     0x129650
//       +0x2f8 slot 95  SetHost                                     0x129680
//       +0x300 slot 96  Redraw                                      0x1296c0
//       +0x308 slot 97  SetRect                                     0x129700
//       +0x310 slot 98  Focus                                       0x129780
//       +0x318 slot 99  DoPaint                                     0x27d0
//     (slots 23/25/26 are CWnd::Create / CWnd::CreateEx(11 args) /
//     CWnd::DestroyWindow -- 0x28b690 / 0x28b500 / 0x28baf0, named by the map)
//   * the message map 0x18030e200 (mfc140u; returned by GetMessageMap 0x129340):
//       WM_NCDESTROY  sig 19  0x1294e0  OnNcDestroy
//       WM_PAINT      sig 19  0x1294f0  OnPaint
//       WM_ERASEBKGND sig  1  0x3a60    OnEraseBkgnd   (AfxSig_bD)
//       WM_SIZE       sig 26  0x129560  OnSize
//
// ---------------------------------------------------------------------------
// Virtual dispatch.  The retail bodies call virtuals on `this` through the
// MSVC-layout vftable slots above.  OpenMFC has no MSVC-layout table for this
// class: ??0CWnd@@QEAA@XZ (core/window/CtorDtorPlacement.cpp) placement-news
// OpenMFC's CWnd, so an object built by the constructor here carries OpenMFC's
// mingw CWnd vtable, on which those slot numbers mean something else.  Same
// convention as featurepack/controls/CMFCEditBrowseCtrl.cpp: the constructor
// records that vptr (g_ownVptr); when `this` still carries it, the object is
// exactly a CMFCPreviewCtrlImpl with no overrides and the call goes straight to
// the thunk the retail slot holds; any other vptr is a client-derived class
// compiled against the real headers (its constructor installs its own MSVC
// vftable after ours returns), and the call is made through that table's slot
// exactly as retail does, so client overrides (DoPaint above all) ARE reached.
// LIMITATION, not fixed here: ATL's CPreviewHandlerImpl (atlhandlerimpl.h)
// drives the control only through virtual calls on an IPreviewCtrl*, which is
// CMFCPreviewCtrlImpl under _MFC_VER (atlhandler.h:227).  Those calls reach
// the thunks below only when the object carries a client MSVC vftable; an
// object built directly by this constructor (no client subclass) keeps the
// mingw vptr, and a client's MSVC-slot call through it lands elsewhere.  The
// destructor stores g_ownVptr back first -- retail's own "vfptr = &vftable"
// store -- so that ??1CWnd@@'s C++ virtual-destructor call dispatches on
// OpenMFC's table and not on the client's.

#include "detail/ManualSmallStubImplementationsSupport.h"

#include <cstddef>

// Sibling thunks, each declared with the signature its mangled name describes;
// the file that defines each one (with a // Symbol: marker) is named.
extern "C" void*    MS_ABI impl___0CWnd__QEAA_XZ(void* pThis);                                   // core/window/CtorDtorPlacement.cpp
extern "C" void     MS_ABI impl___1CWnd__UEAA_XZ(void* pThis);                                   // core/window/CtorDtorPlacement.cpp
extern "C" int      MS_ABI impl__CreateEx_CWnd__UEAAHKPEB_W0KHHHHPEAUHWND____PEAUHMENU____PEAX_Z(
    CWnd* pThis, DWORD dwExStyle, const wchar_t* lpszClassName, const wchar_t* lpszWindowName,
    DWORD dwStyle, int x, int y, int nWidth, int nHeight, HWND hWndParent, HMENU nIDorHMenu,
    void* lpParam);                                                                               // core/window/CWnd.cpp
extern "C" int      MS_ABI impl__DestroyWindow_CWnd__UEAAHXZ(CWnd* pThis);                       // core/window/CWnd.cpp
extern "C" CWnd*    MS_ABI impl__GetDlgItem_CWnd__QEBAPEAV1_H_Z(const CWnd* pThis, int nID);     // core/window/CWnd.cpp
extern "C" int      MS_ABI impl__SetWindowPos_CWnd__QEAAHPEBV1_HHHHI_Z(
    CWnd* pThis, const CWnd* pWndInsertAfter, int x, int y, int cx, int cy, unsigned int nFlags); // core/window/CWnd.cpp
extern "C" __int64  MS_ABI impl__Default_CWnd__IEAA_JXZ(CWnd* pThis);                            // core/window/Thunks.cpp
extern "C" void     MS_ABI impl__OnNcDestroy_CWnd__IEAAXXZ(CWnd* pThis);                         // core/window/Thunks.cpp
extern "C" void*    MS_ABI impl__SetFocus_CWnd__QEAAPEAV1_XZ(CWnd* pThis);                       // core/window/Thunks.cpp
extern "C" CObject* MS_ABI impl__CreateObject_CRuntimeClass__QEAAPEAVCObject__XZ(CRuntimeClass* pThis); // core/runtime/CRuntimeClass.cpp
extern "C" int      MS_ABI impl__DeleteObject_CGdiObject__QEAAHXZ(CGdiObject* pThis);            // core/gdi/CGdiObject.cpp
extern "C" int      MS_ABI impl__Attach_CGdiObject__QEAAHPEAX_Z(CGdiObject* pThis, HGDIOBJ hObject); // core/gdi/CGdiObject.cpp
extern "C" CPaintDC* MS_ABI impl___0CPaintDC__QEAA_PEAVCWnd___Z(CPaintDC* pThis, CWnd* pWnd);   // core/gdi/CPaintDC.cpp
extern "C" void     MS_ABI impl___1CPaintDC__UEAA_XZ(CPaintDC* pThis);                           // core/gdi/CPaintDC.cpp

// This file's own thunks that other bodies here call directly (defined below).
extern "C" int  MS_ABI impl__Create_CMFCPreviewCtrlImpl__UEAAHPEAUHWND____PEBUtagRECT__PEAUCCreateContext___Z(
    void* pThis, HWND hWndParent, const RECT* prc, void* pContext);
extern "C" void MS_ABI impl__DoPaint_CMFCPreviewCtrlImpl__MEAAXPEAVCPaintDC___Z(void* pThis, void* pDC);
extern "C" void MS_ABI impl__Redraw_CMFCPreviewCtrlImpl__UEAAXXZ(void* pThis);

namespace {

struct S_PreviewCtrlImpl {
    unsigned char m_cwndHead[0x40];
    HWND          m_hWnd;                     // +0x040  CWnd::m_hWnd
    unsigned char m_cwndTail[0xe8 - 0x48];
    COLORREF      m_clrBackColor;             // +0x0e8
    COLORREF      m_clrTextColor;             // +0x0ec
    void*         m_fontVptr;                 // +0x0f0  CFont m_font (vfptr)
    HGDIOBJ       m_fontHObject;              // +0x0f8  m_font.m_hObject
    void*         m_pDocument;                // +0x100  ATL::IDocument*
};
static_assert(offsetof(S_PreviewCtrlImpl, m_hWnd) == 0x40, "CWnd::m_hWnd");
static_assert(offsetof(S_PreviewCtrlImpl, m_clrBackColor) == 0xe8, "m_clrBackColor");
static_assert(offsetof(S_PreviewCtrlImpl, m_clrTextColor) == 0xec, "m_clrTextColor");
static_assert(offsetof(S_PreviewCtrlImpl, m_fontVptr) == 0xf0, "m_font");
static_assert(offsetof(S_PreviewCtrlImpl, m_fontHObject) == 0xf8, "m_font.m_hObject");
static_assert(offsetof(S_PreviewCtrlImpl, m_pDocument) == 0x100, "m_pDocument");
static_assert(sizeof(S_PreviewCtrlImpl) == 0x108, "sizeof(CMFCPreviewCtrlImpl)");
static_assert(sizeof(CWnd) == 0xe8, "CWnd base must end where m_clrBackColor starts");
// OpenMFC's CGdiObject is vptr + m_hObject, so the thunks above reach
// m_font.m_hObject at +0xf8 when handed the +0xf0 sub-object.
static_assert(sizeof(CGdiObject) == 0x10, "CGdiObject = vptr + m_hObject");

inline S_PreviewCtrlImpl* L(void* p) { return static_cast<S_PreviewCtrlImpl*>(p); }
inline CWnd* W(void* p) { return static_cast<CWnd*>(p); }
inline HWND HwndOf(const void* pWnd) {
    return static_cast<const S_PreviewCtrlImpl*>(pWnd)->m_hWnd;   // +0x40 on any CWnd
}
inline CGdiObject* Font(void* p) { return reinterpret_cast<CGdiObject*>(&L(p)->m_fontVptr); }

// ---------------------------------------------------------------------------
// Virtual dispatch on `this` (file header).
// ---------------------------------------------------------------------------
void* g_ownVptr = nullptr;   // the vptr ??0CWnd@@QEAA@XZ installs, recorded by the first ctor to run
inline bool HasOwnVptr(const void* p) {
    return g_ownVptr != nullptr && *static_cast<void* const*>(p) == g_ownVptr;
}
// Entry at vftable byte offset `off` (retail's `mov off(%rax),%rax`).
inline void* SlotAt(const void* p, std::size_t off) {
    return *reinterpret_cast<void* const*>(*static_cast<const unsigned char* const*>(p) + off);
}

// Does the object's vptr point into this DLL's image?  Used for the view that
// Create(...) builds through CRuntimeClass::CreateObject: a view class this DLL
// constructs carries an OpenMFC (mingw-layout) vtable, a client view class an
// MSVC-layout one in the client's image.
HMODULE ModuleOfAddress(const void* p) {
    HMODULE h = nullptr;
    if (!::GetModuleHandleExW(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS |
                                  GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
                              static_cast<LPCWSTR>(p), &h)) {
        return nullptr;
    }
    return h;
}
bool VptrInThisModule(const void* pObj) {
    HMODULE self = ModuleOfAddress(reinterpret_cast<const void*>(&ModuleOfAddress));
    return self != nullptr && ModuleOfAddress(*static_cast<void* const*>(pObj)) == self;
}

using PFN_CreateEx = int (MS_ABI*)(void*, DWORD, const wchar_t*, const wchar_t*, DWORD,
                                   int, int, int, int, HWND, HMENU, void*);   // CWnd slot 25 (+0xc8)
using PFN_WndCreate = int (MS_ABI*)(void*, const wchar_t*, const wchar_t*, DWORD,
                                    const RECT*, void*, UINT, void*);         // CWnd slot 23 (+0xb8)
using PFN_Bool0 = int (MS_ABI*)(void*);                                       // DestroyWindow, slot 26 (+0xd0)
using PFN_Void0 = void (MS_ABI*)(void*);                                      // Redraw (+0x300), OnInitialUpdate (+0x328)
using PFN_Create3 = int (MS_ABI*)(void*, HWND, const RECT*, void*);           // slot 91 (+0x2d8)
using PFN_DoPaint = void (MS_ABI*)(void*, void*);                             // slot 99 (+0x318)

constexpr DWORD kChildVisible = 0x50000000;   // WS_CHILD | WS_VISIBLE, the retail immediate
constexpr UINT  kViewID = 1;                   // the retail immediate (nIDorHMenu / nID)

} // namespace

// Retail ??0CMFCPreviewCtrlImpl@@QEAA@XZ (entry RVA 0x129260, mfc140u):
//     CWnd::CWnd();                        // 0x28a700
//     vfptr = &CMFCPreviewCtrlImpl::`vftable';   // 0x18030e2b8
//     m_font: vfptr = &CFont::`vftable' (0x1802e8ad0); m_hObject = NULL;
//     *(ULONGLONG*)&m_clrBackColor = 0xffffff;   // back = RGB(255,255,255), text = 0
//     return this;
// DEVIATIONS: no MSVC-layout vftable exists for this class or for CFont here,
// so `this` keeps the vptr ??0CWnd@@ installs (recorded as g_ownVptr, file
// header) and m_font's vptr slot is set to NULL: the two CGdiObject thunks
// this file calls on m_font (DeleteObject, Attach; core/gdi/CGdiObject.cpp)
// read and write only m_hObject, but anything that dispatches virtually on
// m_font (e.g. a client IsKindOf) would fault.  m_pDocument is left unwritten,
// as in retail.
// Symbol: ??0CMFCPreviewCtrlImpl@@QEAA@XZ
extern "C" void* MS_ABI impl___0CMFCPreviewCtrlImpl__QEAA_XZ(void* pThis) {
    if (pThis == nullptr) return pThis;   // deviation: retail has no NULL check
    impl___0CWnd__QEAA_XZ(pThis);
    if (g_ownVptr == nullptr) g_ownVptr = *static_cast<void**>(pThis);
    S_PreviewCtrlImpl* self = L(pThis);
    self->m_fontVptr = nullptr;
    self->m_fontHObject = nullptr;
    self->m_clrBackColor = 0x00ffffff;
    self->m_clrTextColor = 0;
    return pThis;
}

// Retail ??1CMFCPreviewCtrlImpl@@UEAA@XZ (entry RVA 0x129300, mfc140u), fully transcribed:
//     vfptr = &CMFCPreviewCtrlImpl::`vftable';   // 0x18030e2b8 -> g_ownVptr here (file header)
//     m_font: vfptr = &CFont::`vftable';          // 0x1802e8ad0
//     m_font.CGdiObject::~CGdiObject();           // 0x1c6f0: CGdiObject vftable store + DeleteObject (0x2a3f60)
//     CWnd::~CWnd();                              // 0x28b740 (tail jump)
// DEVIATION: the own-vftable store becomes the g_ownVptr store; the two stores
// into m_font (CFont vftable here, CGdiObject vftable 0x1802dddd8 (mfc140u)
// inside 0x1c6f0) are not reproduced (no MSVC-layout tables here);
// DeleteObject is called through its exported thunk.
// Symbol: ??1CMFCPreviewCtrlImpl@@UEAA@XZ
extern "C" void MS_ABI impl___1CMFCPreviewCtrlImpl__UEAA_XZ(void* pThis) {
    if (pThis == nullptr) return;   // deviation: retail has no NULL check
    if (g_ownVptr != nullptr) *static_cast<void**>(pThis) = g_ownVptr;
    impl__DeleteObject_CGdiObject__QEAAHXZ(Font(pThis));
    impl___1CWnd__UEAA_XZ(pThis);
}

// Retail entry RVA 0x129350 (mfc140u), fully transcribed:
//     return this->Create(hWndParent, prc, NULL);   // vftable slot 91 (+0x2d8)
// Symbol: ?Create@CMFCPreviewCtrlImpl@@UEAAHPEAUHWND__@@PEBUtagRECT@@@Z
extern "C" int MS_ABI impl__Create_CMFCPreviewCtrlImpl__UEAAHPEAUHWND____PEBUtagRECT___Z(
    void* pThis, HWND hWndParent, const RECT* prc) {
    if (HasOwnVptr(pThis)) {
        return impl__Create_CMFCPreviewCtrlImpl__UEAAHPEAUHWND____PEBUtagRECT__PEAUCCreateContext___Z(
            pThis, hWndParent, prc, nullptr);
    }
    return reinterpret_cast<PFN_Create3>(SlotAt(pThis, 0x2d8))(pThis, hWndParent, prc, nullptr);
}

// Retail entry RVA 0x129370 (mfc140u), fully transcribed:
//     if (prc == NULL) return FALSE;
//     this->CreateEx(0, NULL, L"", WS_CHILD | WS_VISIBLE,          // vftable slot 25 (+0xc8)
//                    prc->left, prc->top, prc->right - prc->left, prc->bottom - prc->top,
//                    hWndParent, (HMENU)1, NULL);                  // result NOT tested
//     if (pContext == NULL || pContext->m_pNewViewClass == NULL) return TRUE;
//     CWnd* pView = (CWnd*)pContext->m_pNewViewClass->CreateObject();   // 0x234d60; result NOT tested
//     CRect rect; ::CopyRect(&rect, prc);
//     CRect rectView(0, 0, rect.Width(), rect.Height());
//     if (!pView->Create(NULL, NULL, WS_CHILD | WS_VISIBLE, rectView, this, 1, pContext))  // slot 23 (+0xb8)
//         return FALSE;
//     pView->OnInitialUpdate();                                     // slot 101 (+0x328)
//     return TRUE;
// (The window name at 0x18033d19c (mfc140u) is an empty wide string.  Slot 101
// is OnInitialUpdate: CFormView's vftable 0x180330e18 (mfc140) holds
// ?OnInitialUpdate@CFormView@@ at +0x328.)
// DEVIATIONS: (1) a NULL pView returns FALSE instead of faulting.  (2) When
// the view's vptr points into this DLL (a view class OpenMFC itself
// constructs, mingw vtable) Create and OnInitialUpdate are made as C++ virtual
// calls rather than through the MSVC slot numbers, which would index a
// different table; a client view class is dispatched through its MSVC slots.
// Symbol: ?Create@CMFCPreviewCtrlImpl@@UEAAHPEAUHWND__@@PEBUtagRECT@@PEAUCCreateContext@@@Z
extern "C" int MS_ABI impl__Create_CMFCPreviewCtrlImpl__UEAAHPEAUHWND____PEBUtagRECT__PEAUCCreateContext___Z(
    void* pThis, HWND hWndParent, const RECT* prc, void* pContext) {
    if (prc == nullptr) return FALSE;

    const int x = prc->left;
    const int y = prc->top;
    const int cx = prc->right - prc->left;
    const int cy = prc->bottom - prc->top;
    HMENU hMenu = reinterpret_cast<HMENU>(static_cast<UINT_PTR>(kViewID));
    if (HasOwnVptr(pThis)) {
        (void)impl__CreateEx_CWnd__UEAAHKPEB_W0KHHHHPEAUHWND____PEAUHMENU____PEAX_Z(
            W(pThis), 0, nullptr, L"", kChildVisible, x, y, cx, cy, hWndParent, hMenu, nullptr);
    } else {
        (void)reinterpret_cast<PFN_CreateEx>(SlotAt(pThis, 0xc8))(
            pThis, 0, nullptr, L"", kChildVisible, x, y, cx, cy, hWndParent, hMenu, nullptr);
    }

    if (pContext == nullptr) return TRUE;
    // CCreateContext::m_pNewViewClass is its first member (retail reads (%rsi)).
    CRuntimeClass* pNewViewClass = *static_cast<CRuntimeClass* const*>(pContext);
    if (pNewViewClass == nullptr) return TRUE;

    CObject* pView = impl__CreateObject_CRuntimeClass__QEAAPEAVCObject__XZ(pNewViewClass);
    if (pView == nullptr) return FALSE;   // deviation (1)

    RECT rect;
    ::CopyRect(&rect, prc);
    RECT rectView = { 0, 0, rect.right - rect.left, rect.bottom - rect.top };

    if (VptrInThisModule(pView)) {   // deviation (2)
        CWnd* pViewWnd = static_cast<CWnd*>(pView);
        if (!pViewWnd->Create(nullptr, nullptr, kChildVisible, rectView, W(pThis), kViewID,
                              static_cast<CCreateContext*>(pContext))) {
            return FALSE;
        }
        static_cast<CView*>(pView)->OnInitialUpdate();
        return TRUE;
    }
    if (!reinterpret_cast<PFN_WndCreate>(SlotAt(pView, 0xb8))(
            pView, nullptr, nullptr, kChildVisible, &rectView, pThis, kViewID, pContext)) {
        return FALSE;
    }
    reinterpret_cast<PFN_Void0>(SlotAt(pView, 0x328))(pView);
    return TRUE;
}

// Retail entry RVA 0x129650 (mfc140u), fully transcribed:
//     if (::IsWindow(m_hWnd)) DestroyWindow();   // vftable slot 26 (+0xd0)
// Symbol: ?Destroy@CMFCPreviewCtrlImpl@@UEAAXXZ
extern "C" void MS_ABI impl__Destroy_CMFCPreviewCtrlImpl__UEAAXXZ(void* pThis) {
    if (!::IsWindow(HwndOf(pThis))) return;
    if (HasOwnVptr(pThis)) {
        (void)impl__DestroyWindow_CWnd__UEAAHXZ(W(pThis));
    } else {
        (void)reinterpret_cast<PFN_Bool0>(SlotAt(pThis, 0xd0))(pThis);
    }
}

// Retail DoPaint is vftable slot 99 (+0x318) of 0x18030e2b8 (mfc140u), whose
// entry is 0x27d0 -- a lone `ret` that the linker folded with every other
// empty void member (the map names it ?AddDockSite@CFrameWndEx@@QEAAXXZ).  The
// base DoPaint therefore does nothing; it exists to be overridden (OnPaint
// below calls it virtually).  This empty body IS the retail body.
// Symbol: ?DoPaint@CMFCPreviewCtrlImpl@@MEAAXPEAVCPaintDC@@@Z
extern "C" void MS_ABI impl__DoPaint_CMFCPreviewCtrlImpl__MEAAXPEAVCPaintDC___Z(void* pThis, void* pDC) {
    (void)pThis;
    (void)pDC;
}

// Retail entry RVA 0x129780 (mfc140u), fully transcribed:
//     if (::IsWindow(m_hWnd)) CWnd::SetFocus();   // direct call (u 0x2a9b60; ANSI twin
//                                                 // 0x2a7a70 = ?SetFocus@CWnd@@QEAAPEAV1@XZ)
// Symbol: ?Focus@CMFCPreviewCtrlImpl@@UEAAXXZ
extern "C" void MS_ABI impl__Focus_CMFCPreviewCtrlImpl__UEAAXXZ(void* pThis) {
    if (::IsWindow(HwndOf(pThis))) {
        (void)impl__SetFocus_CWnd__QEAAPEAV1_XZ(W(pThis));
    }
}

// The WM_ERASEBKGND entry of the retail message map 0x18030e200 (mfc140u)
// points at 0x3a60, `mov $0x1,%eax; ret` (folded; the map names it
// ?accDoDefaultAction@CMFCBaseAccessibleObject@@).  Fully transcribed:
//     return TRUE;
// Symbol: ?OnEraseBkgnd@CMFCPreviewCtrlImpl@@QEAAHPEAVCDC@@@Z
extern "C" int MS_ABI impl__OnEraseBkgnd_CMFCPreviewCtrlImpl__QEAAHPEAVCDC___Z(void* pThis, void* pDC) {
    (void)pThis;
    (void)pDC;
    return TRUE;
}

// Retail entry RVA 0x1294e0 (mfc140u; the WM_NCDESTROY map entry), fully
// transcribed: a tail jump to CWnd::OnNcDestroy (u 0x28b900; the ANSI twin
// 0x12a240 jumps to 0x289d80 = ?OnNcDestroy@CWnd@@IEAAXXZ).
// Symbol: ?OnNcDestroy@CMFCPreviewCtrlImpl@@QEAAXXZ
extern "C" void MS_ABI impl__OnNcDestroy_CMFCPreviewCtrlImpl__QEAAXXZ(void* pThis) {
    impl__OnNcDestroy_CWnd__IEAAXXZ(W(pThis));
}

// Retail entry RVA 0x1294f0 (mfc140u; the WM_PAINT map entry), fully transcribed:
//     CPaintDC dc(this);     // 0x2a3d20
//     DoPaint(&dc);          // vftable slot 99 (+0x318)
//     // ~CPaintDC           // 0x2a3dd0
// (Retail's frame holds the CPaintDC in 0x70 bytes, 0x20..0x90.)
// KNOWN GAP (core/gdi/CPaintDC.cpp, not this file): the CPaintDC handed to a
// client DoPaint override is OpenMFC's -- the ctor thunk installs no vptr (so
// a virtual CDC call from the override faults) and its layout puts m_ps at
// +0x20, where the shipping header has HWND m_hWnd at +0x20 and m_ps at +0x28.
// Symbol: ?OnPaint@CMFCPreviewCtrlImpl@@QEAAXXZ
extern "C" void MS_ABI impl__OnPaint_CMFCPreviewCtrlImpl__QEAAXXZ(void* pThis) {
    alignas(void*) unsigned char dcStorage[sizeof(CPaintDC)] = {};
    CPaintDC* pDC = reinterpret_cast<CPaintDC*>(dcStorage);
    impl___0CPaintDC__QEAA_PEAVCWnd___Z(pDC, W(pThis));
    if (HasOwnVptr(pThis)) {
        impl__DoPaint_CMFCPreviewCtrlImpl__MEAAXPEAVCPaintDC___Z(pThis, pDC);
    } else {
        reinterpret_cast<PFN_DoPaint>(SlotAt(pThis, 0x318))(pThis, pDC);
    }
    impl___1CPaintDC__UEAA_XZ(pDC);
}

// Retail entry RVA 0x129560 (mfc140u; the WM_SIZE map entry), fully transcribed
// -- nType/cx/cy are not read:
//     CWnd::Default();                                   // 0x28ac80
//     CWnd* pChild = GetDlgItem(1);                      // 0x2a9390
//     if (pChild != NULL && ::IsWindow(pChild->m_hWnd)) {
//         CRect rect; ::GetClientRect(m_hWnd, &rect);    // rect zeroed first
//         pChild->SetWindowPos(NULL, 0, 0, rect.Width(), rect.Height(),
//                              SWP_NOZORDER | SWP_NOACTIVATE);   // 0x14; u 0x2a9a60, ANSI
//     }                                                   // twin 0x2a7970 = ?SetWindowPos@CWnd@@
// Symbol: ?OnSize@CMFCPreviewCtrlImpl@@QEAAXIHH@Z
extern "C" void MS_ABI impl__OnSize_CMFCPreviewCtrlImpl__QEAAXIHH_Z(void* pThis, unsigned int nType, int cx, int cy) {
    (void)nType;
    (void)cx;
    (void)cy;
    (void)impl__Default_CWnd__IEAA_JXZ(W(pThis));
    CWnd* pChild = impl__GetDlgItem_CWnd__QEBAPEAV1_H_Z(W(pThis), static_cast<int>(kViewID));
    if (pChild == nullptr || !::IsWindow(HwndOf(pChild))) return;
    RECT rect = { 0, 0, 0, 0 };
    ::GetClientRect(HwndOf(pThis), &rect);
    (void)impl__SetWindowPos_CWnd__QEAAHPEBV1_HHHHI_Z(pChild, nullptr, 0, 0,
                                                      rect.right - rect.left, rect.bottom - rect.top,
                                                      SWP_NOZORDER | SWP_NOACTIVATE);
}

// Retail entry RVA 0x1296c0 (mfc140u; vftable slot 96), fully transcribed:
//     if (::IsWindow(m_hWnd)) { ::InvalidateRect(m_hWnd, NULL, TRUE); ::UpdateWindow(m_hWnd); }
// Symbol: ?Redraw@CMFCPreviewCtrlImpl@@UEAAXXZ
extern "C" void MS_ABI impl__Redraw_CMFCPreviewCtrlImpl__UEAAXXZ(void* pThis) {
    if (!::IsWindow(HwndOf(pThis))) return;
    ::InvalidateRect(HwndOf(pThis), nullptr, TRUE);
    ::UpdateWindow(HwndOf(pThis));
}

// Retail entry RVA 0x129680 (mfc140u; vftable slot 95), fully transcribed:
//     if (::IsWindow(m_hWnd)) ::SetParent(m_hWnd, hWndParent);
// Symbol: ?SetHost@CMFCPreviewCtrlImpl@@UEAAXPEAUHWND__@@@Z
extern "C" void MS_ABI impl__SetHost_CMFCPreviewCtrlImpl__UEAAXPEAUHWND_____Z(void* pThis, HWND hWndParent) {
    if (::IsWindow(HwndOf(pThis))) {
        ::SetParent(HwndOf(pThis), hWndParent);
    }
}

// Retail entry RVA 0x129610 (mfc140u; vftable slot 93), fully transcribed:
//     m_clrBackColor = clrBack;
//     m_clrTextColor = clrText;
//     if (plf != NULL) m_font.Attach(::CreateFontIndirectW(plf));   // u 0x2a3ed0 = ?Attach@CGdiObject@@
// The previous font is NOT deleted first.  Retail Attach (0x2a3ed0, mfc140u)
// returns FALSE without writing m_hObject when handed NULL (CreateFontIndirectW
// failed), whereas OpenMFC's Attach thunk stores the NULL; the NULL test below
// keeps retail's behaviour (old handle kept) on that path.
// (This body is one of the few that differs between the images: the ANSI
// twin, 0x12a370 in mfc140.dll, copies the LOGFONTW into a LOGFONTA, converts
// lfFaceName with wcstombs_s and calls CreateFontIndirectA.)
// Symbol: ?SetPreviewVisuals@CMFCPreviewCtrlImpl@@UEAAXKKPEBUtagLOGFONTW@@@Z
extern "C" void MS_ABI impl__SetPreviewVisuals_CMFCPreviewCtrlImpl__UEAAXKKPEBUtagLOGFONTW___Z(
    void* pThis, COLORREF clrBack, COLORREF clrText, const LOGFONTW* plf) {
    S_PreviewCtrlImpl* self = L(pThis);
    self->m_clrBackColor = clrBack;
    self->m_clrTextColor = clrText;
    if (plf != nullptr) {
        HFONT hFont = ::CreateFontIndirectW(plf);
        if (hFont != nullptr) {
            (void)impl__Attach_CGdiObject__QEAAHPEAX_Z(Font(pThis), hFont);
        }
    }
}

// Retail entry RVA 0x129700 (mfc140u; vftable slot 97), fully transcribed:
//     if (::IsWindow(m_hWnd) && prc != NULL) {
//         SetWindowPos(NULL, prc->left, prc->top, prc->right - prc->left, prc->bottom - prc->top,
//                      SWP_NOZORDER | SWP_NOREDRAW | SWP_NOACTIVATE);   // 0x1c
//         if (bRedraw) Redraw();                                        // vftable slot 96 (+0x300)
//     }
// Symbol: ?SetRect@CMFCPreviewCtrlImpl@@UEAAXPEBUtagRECT@@H@Z
extern "C" void MS_ABI impl__SetRect_CMFCPreviewCtrlImpl__UEAAXPEBUtagRECT__H_Z(void* pThis, const RECT* prc, int bRedraw) {
    if (!::IsWindow(HwndOf(pThis)) || prc == nullptr) return;
    (void)impl__SetWindowPos_CWnd__QEAAHPEBV1_HHHHI_Z(W(pThis), nullptr, prc->left, prc->top,
                                                      prc->right - prc->left, prc->bottom - prc->top,
                                                      SWP_NOZORDER | SWP_NOREDRAW | SWP_NOACTIVATE);
    if (!bRedraw) return;
    if (HasOwnVptr(pThis)) {
        impl__Redraw_CMFCPreviewCtrlImpl__UEAAXXZ(pThis);
    } else {
        reinterpret_cast<PFN_Void0>(SlotAt(pThis, 0x300))(pThis);
    }
}
