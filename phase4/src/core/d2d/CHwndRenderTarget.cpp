// CHwndRenderTarget — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp (original stubs)
//
// Retail layout (afxrendertarget.h; mfc140u ctor at RVA 0xd61f0): CRenderTarget base
// (vptr, ID2D1RenderTarget* m_pRenderTarget at +0x8, CObList m_lstResources at +0x10,
// one more zeroed 8-byte member at +0x48), then ID2D1HwndRenderTarget*
// m_pHwndRenderTarget at +0x50; sizeof == 0x58 (the 88 in RuntimeClasses.cpp's
// descriptor).  Every retail body below touches only +0x8, +0x50 and, in ReCreate, the
// resource list head at +0x18 (m_lstResources.m_pNodeHead).
//
// This tree does NOT keep m_pRenderTarget in the object: CRenderTarget's thunks
// (core/d2d/CRenderTarget.cpp) keep it in the g_renderTargetState[pThis].resource side
// table, and CWnd::GetRenderTarget (core/window/CWnd.cpp) hands out a CDCRenderTarget
// allocated with this tree's (smaller) layout reinterpret_cast to CHwndRenderTarget*, so
// a raw store to +0x50 could write past that allocation.  The bodies below therefore
// model both members through that single side-table slot, reached only through the
// exported CRenderTarget Attach/Detach thunks:
//   * retail's +0x8 (m_pRenderTarget)     == the side-table slot;
//   * retail's +0x50 (m_pHwndRenderTarget) == the side-table slot when it holds an
//     ID2D1HwndRenderTarget (checked by QueryInterface), else NULL.
// CHwndRenderTarget's own retail methods only ever store the same pointer into both
// members (Attach, Create) or clear both (Detach, ReCreate; the ctor zeroes both), and
// ID2D1HwndRenderTarget derives singly from ID2D1RenderTarget, so the two pointers are
// equal whenever those methods left +0x50 non-NULL.
// Known divergences, all from the non-virtual CRenderTarget base methods, which in retail
// touch only +0x8 (mfc140u: Attach 0xd5470, Detach 0xd5480, Destroy 0xd51e0):
//   * CRenderTarget::Detach / Destroy clear +0x8 but leave +0x50 set (Destroy leaves it
//     pointing at the target it just Released); here +0x50 reads as NULL afterwards.
//   * CRenderTarget::Attach of an ID2D1HwndRenderTarget sets only +0x8, leaving +0x50
//     NULL; here +0x50 reads as that target.
// Client-inline accessors (GetHwndRenderTarget, operator ID2D1HwndRenderTarget*, GetHwnd,
// CheckWindowState in afxrendertarget.h) read the object's real +0x50, which nothing in
// this tree writes, so they see NULL whatever the side table holds.

// Standalone TU (as core/d2d/CD2DResource.cpp is): only <windows.h> and <d2d1.h>.
// ManualSmallStubImplementationsSupport.h, which this file used to include, declares its
// own D2D_MATRIX_3X2_F / D2D1_EXTEND_MODE stand-ins that clash with <d2d1.h>, and
// GdicoreSupport.h drags in inline code referencing C++ symbols this file does not need.
#include <windows.h>
#include <d2d1.h>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

// CRenderTarget thunks, defined in phase4/src/core/d2d/CRenderTarget.cpp (there with a
// CRenderTarget* first parameter; void* here, same MS x64 ABI).
//   Attach: stores pRenderTarget in the side-table slot, Releasing a different old one.
//   Detach: returns the slot and clears it, no Release.
extern "C" void MS_ABI impl__Attach_CRenderTarget__QEAAXPEAUID2D1RenderTarget___Z(void* pThis, void* pRenderTarget);
extern "C" void* MS_ABI impl__Detach_CRenderTarget__QEAAPEAUID2D1RenderTarget__XZ(void* pThis);

// Sibling thunk defined below (retail ReCreate, mfc140u 0xd6410, calls Create directly:
// the call at 0xd6451 inside it).
extern "C" int MS_ABI impl__Create_CHwndRenderTarget__QEAAHPEAUHWND_____Z(void* pThis, HWND hWnd);

namespace {

void* Base(void* pThis) { return pThis; }   // CRenderTarget is the sole, offset-0 base

// IID_ID2D1HwndRenderTarget {2cd90698-12e2-11dc-9fed-001143a055f9}, as the Windows SDK
// d2d1.h declares it.  Spelled out because mingw-w64's d2d1.h has no __CRT_UUID_DECL for
// this interface, so __uuidof(ID2D1HwndRenderTarget) would leave an unresolved
// __mingw_uuidof<ID2D1HwndRenderTarget> reference.
const IID kIID_ID2D1HwndRenderTarget =
    {0x2cd90698, 0x12e2, 0x11dc, {0x9f, 0xed, 0x00, 0x11, 0x43, 0xa0, 0x55, 0xf9}};

// Retail m_pRenderTarget (+0x8).  Read through the exported pair, the same
// Detach-then-reAttach idiom core/d2d/CRenderTarget.cpp uses for brushes: after Detach
// the slot is NULL, so the re-Attach only stores and Releases nothing.  The pair is two
// separately locked calls, not one atomic read: another thread reading the slot between
// them sees NULL.  Detach's operator[] also creates an empty side-table entry for a
// pThis that had none.
ID2D1RenderTarget* RenderTargetOf(void* pThis) {
    void* p = impl__Detach_CRenderTarget__QEAAPEAUID2D1RenderTarget__XZ(Base(pThis));
    if (p) impl__Attach_CRenderTarget__QEAAXPEAUID2D1RenderTarget___Z(Base(pThis), p);
    return static_cast<ID2D1RenderTarget*>(p);
}

// Retail m_pHwndRenderTarget (+0x50), per the model in the header comment.  The
// QueryInterface is a deviation retail does not need (its +0x50 is typed): without it a
// CDCRenderTarget handed out by this tree's CWnd::GetRenderTarget would have its
// ID2D1DCRenderTarget dispatched through ID2D1HwndRenderTarget vtable slots that
// interface does not have.  The extra reference QI adds is dropped at once; the side
// table keeps the owning one.
ID2D1HwndRenderTarget* HwndRenderTargetOf(void* pThis) {
    ID2D1RenderTarget* pTarget = RenderTargetOf(pThis);
    if (pTarget == nullptr) return nullptr;
    ID2D1HwndRenderTarget* pHwnd = nullptr;
    if (FAILED(pTarget->QueryInterface(kIID_ID2D1HwndRenderTarget,
                                       reinterpret_cast<void**>(&pHwnd))) || pHwnd == nullptr) {
        return nullptr;
    }
    pHwnd->Release();
    return pHwnd;
}

} // namespace

// Symbol: ??0CHwndRenderTarget@@QEAA@PEAUHWND__@@@Z
extern "C" void* MS_ABI impl___0CHwndRenderTarget__QEAA_PEAUHWND_____Z(void* pThis, void* hWnd) {
    (void)hWnd;
    return pThis;
}
// Symbol: ?Attach@CHwndRenderTarget@@QEAAXPEAUID2D1HwndRenderTarget@@@Z
// Retail RVA 0xd64b0 (mfc140u; export ordinal 2476, identical-code-folded with
// CBitmapRenderTarget::Attach, ordinal 2462, and CDCRenderTarget::Attach, ordinal 2474):
//   cmpq $0x0,0x8(%rcx); jne ret; mov %rdx,0x50(%rcx); mov %rdx,0x8(%rcx); ret
// i.e. only when m_pRenderTarget is NULL, store pTarget into m_pHwndRenderTarget and
// m_pRenderTarget.  No AddRef, no Release.  Here the NULL test reads the side-table slot
// and the store goes through CRenderTarget's Attach thunk, which Releases nothing because
// the slot it replaces is NULL.
extern "C" void MS_ABI impl__Attach_CHwndRenderTarget__QEAAXPEAUID2D1HwndRenderTarget___Z(
    void* pThis, ID2D1HwndRenderTarget* pTarget) {
    if (RenderTargetOf(pThis) != nullptr) return;
    impl__Attach_CRenderTarget__QEAAXPEAUID2D1RenderTarget___Z(Base(pThis), pTarget);
}

// Symbol: ?Create@CHwndRenderTarget@@QEAAHPEAUHWND__@@@Z
// STUB (guards only).  Retail RVA 0xd62a0 (mfc140u):
//   if (hWnd == NULL || m_pRenderTarget /* +0x8 */ != NULL) return FALSE;
//   state = inlined AfxGetD2DState() -- CProcessLocalObject::GetData (RVA 0x14d100), a
//           NULL result calls AfxThrowInvalidArgException (RVA 0x227720);
//   state->InitD2D(0, 0) (RVA 0xd22e0);
//   if (state->m_pDirect2dFactory /* +0x18 of the state */ == NULL) return FALSE;
//   RECT rc = {0}; ::GetClientRect(hWnd, &rc);           // IAT slot resolved: USER32!GetClientRect
//   state = AfxGetD2DState() again (same inlined NULL -> throw); state->InitD2D(0, 0) again;
//   D2D1_RENDER_TARGET_PROPERTIES props = {};              // all 28 bytes zeroed, = D2D1::RenderTargetProperties()
//   D2D1_HWND_RENDER_TARGET_PROPERTIES hp = { hWnd, { rc.right - rc.left, rc.bottom - rc.top },
//                                             D2D1_PRESENT_OPTIONS_NONE };
//   hr = factory->CreateHwndRenderTarget(&props, &hp, &m_pHwndRenderTarget /* +0x50 */);
//                                                          // ID2D1Factory vtable offset 0x70, slot 14
//   if (FAILED(hr)) return FALSE;                          // `js` to the zero path
//   m_pRenderTarget = m_pHwndRenderTarget; return TRUE;
// Only the two leading guards are transcribed.  This tree's _AFX_D2D_STATE
// (detail/CbarcoreSupport.h: d2dFactoryType, dWriteFactoryType, initialized) holds no
// ID2D1Factory and its InitD2D creates none, so the factory test always takes retail's
// return-FALSE path; creating a private factory instead (as CDCRenderTarget::Create does)
// was not done, because a render target from a different factory than the process-wide
// one rejects that factory's geometries (D2DERR_WRONG_FACTORY) -- the same reasoning
// CD2DPathGeometry::Create records.
extern "C" int MS_ABI impl__Create_CHwndRenderTarget__QEAAHPEAUHWND_____Z(void* pThis, HWND hWnd) {
    if (hWnd == nullptr || RenderTargetOf(pThis) != nullptr) return FALSE;
    return FALSE;   // no process-wide ID2D1Factory in this tree's _AFX_D2D_STATE
}

// Symbol: ?Detach@CHwndRenderTarget@@QEAAPEAUID2D1HwndRenderTarget@@XZ
// Retail RVA 0xd64c0 (mfc140u; export ordinal 3829, identical-code-folded with
// CDCRenderTarget::Detach, ordinal 3826, which is the name the RVA map shows there):
//   mov 0x50(%rcx),%rax; movq $0x0,0x50(%rcx); movq $0x0,0x8(%rcx); ret
// Returns m_pHwndRenderTarget and clears both it and m_pRenderTarget, no Release.  Here:
// the HWND view of the slot is read first, then CRenderTarget's Detach clears the slot
// (its return value is the same reference, or a non-HWND target that retail would also
// drop from +0x8 while returning NULL).
extern "C" ID2D1HwndRenderTarget* MS_ABI impl__Detach_CHwndRenderTarget__QEAAPEAUID2D1HwndRenderTarget__XZ(void* pThis) {
    ID2D1HwndRenderTarget* pHwnd = HwndRenderTargetOf(pThis);
    impl__Detach_CRenderTarget__QEAAPEAUID2D1RenderTarget__XZ(Base(pThis));
    return pHwnd;
}

// Symbol: ?ReCreate@CHwndRenderTarget@@QEAAHPEAUHWND__@@@Z
// Retail RVA 0xd6410 (mfc140u).  Not exported by the 14.51 mfc140u.dll on this host (its
// export table stops at ordinal 9566; this symbol's ordinal 12174 exists only in the
// mfc_complete_ordinal_mapping.json .lib list), so the address is identified by its
// body and its caller: CWnd::DoD2DPaint (mfc140u 0x2920c0) calls it at 0x29215f,
// right after CRenderTarget::EndDraw (0xd5510) returns D2DERR_RECREATE_TARGET
// (0x8899000c), passing the target and CWnd::m_hWnd (+0x40).  Body:
//   if (m_pHwndRenderTarget /* +0x50 */ == NULL) return FALSE;
//   m_pHwndRenderTarget->Release();                        // vtable offset 0x10, slot 2
//   m_pHwndRenderTarget = NULL; m_pRenderTarget = NULL;
//   if (!Create(hWnd)) return FALSE;                       // direct call to 0xd62a0
//   for (POSITION pos = m_lstResources.m_pNodeHead /* +0x18 */; pos != NULL; ) {
//       CObject* pObj = node->data (+0x10); pos = node->pNext (+0x0);
//       CD2DResource* pRes = pObj && pObj->IsKindOf(RUNTIME_CLASS(CD2DResource))  // IsKindOf RVA 0x234cf0
//                            ? (CD2DResource*)pObj : NULL;
//       pRes->ReCreate(this);                              // vtable offset 0x28, slot 5; no NULL test,
//   }                                                      // result ignored
//   return TRUE;
// Transcribed up to and including the Create call.  The resource walk is NOT reproduced:
// this tree's CRenderTarget keeps no m_lstResources (its resources live in
// g_d2dResourceStates, core/d2d/CD2DResource.cpp, which is keyed by resource and not
// reachable from here).  It is unreachable today because Create above always returns
// FALSE; if Create is ever implemented, this path returns TRUE without re-creating the
// target's resources and needs revisiting.
extern "C" int MS_ABI impl__ReCreate_CHwndRenderTarget__QEAAHPEAUHWND_____Z(void* pThis, HWND hWnd) {
    ID2D1HwndRenderTarget* pHwnd = HwndRenderTargetOf(pThis);
    if (pHwnd == nullptr) return FALSE;
    pHwnd->Release();
    impl__Detach_CRenderTarget__QEAAPEAUID2D1RenderTarget__XZ(Base(pThis));  // clears +0x50 and +0x8
    if (!impl__Create_CHwndRenderTarget__QEAAHPEAUHWND_____Z(pThis, hWnd)) return FALSE;
    return TRUE;   // deviation: m_lstResources walk not reproduced (see above)
}

// Symbol: ?Resize@CHwndRenderTarget@@QEAAHAEBVCD2DSizeU@@@Z
// Retail RVA 0xd64e0 (mfc140u).  Not exported by the 14.51 mfc140u.dll on this host
// (ordinal 12522 exists only in the .lib-derived ordinal mapping), so the address is
// identified by its body and its caller: the WM_SIZE (msg 5) branch of an unnamed CWnd
// message routine at mfc140u 0x28d1b0 calls it at 0x28d3aa on CWnd::GetRenderTarget()'s
// result with {LOWORD(lParam), HIWORD(lParam)}.  Body:
//   if (m_pHwndRenderTarget /* +0x50 */ == NULL) return FALSE;
//   hr = m_pHwndRenderTarget->Resize(&size);               // vtable offset 0x1d0, slot 58
//   return hr >= 0;                                        // `not %eax; shr $0x1f,%eax`
// The CD2DSizeU reference is passed straight through (CD2DSizeU derives from D2D1_SIZE_U,
// so it is the same address); retail does not test it for NULL.
extern "C" int MS_ABI impl__Resize_CHwndRenderTarget__QEAAHAEBVCD2DSizeU___Z(void* pThis, const D2D1_SIZE_U* pSize) {
    ID2D1HwndRenderTarget* pHwnd = HwndRenderTargetOf(pThis);
    if (pHwnd == nullptr) return FALSE;
    return pHwnd->Resize(pSize) >= 0 ? TRUE : FALSE;
}
