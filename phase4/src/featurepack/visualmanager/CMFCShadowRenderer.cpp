// CMFCShadowRenderer — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp
//
// Retail class (afxcontrolrenderer.h, VC 14.51): CMFCShadowRenderer derives from
// CMFCControlRenderer and adds NO data members; sizeof == 0x200 (the operator new
// size in retail CreateObject, RVA 0x32d80 (mfc140u)).  Its vftable is at
// 0x1802e3008 (mfc140u); slot layout read from that table:
//   0 GetRuntimeClass   1 scalar deleting dtor (0x32df0 (mfc140u))   5 Create(info,bFlip)
//   6 Draw   7 DrawFrame   8/9 FillInterior (inherited)   10 OnSysColorChange
//   11 Mirror   12 CleanUp   13 Create(nDepth,clrBase,iMin,iMax)
//
// Every non-trivial method here works on the RETAIL CMFCControlRenderer layout:
// m_Bitmap (CMFCToolBarImages) at +0x08, m_Params (CMFCControlRendererInfo) at
// +0x1a0 (so m_rectImage +0x1b0, m_rectCorners +0x1c0, m_rectSides +0x1d0,
// m_rectInter +0x1e0, m_clrTransparent +0x1f0, m_bPreMultiplyCheck +0x1f4),
// m_bMirror +0x1f8, m_bIsScaled +0x1fc.  OpenMFC's CMFCControlRenderer
// (include/openmfc/afxmfc.h) does NOT have that layout: it is a 104-byte object
// (vptr + char[96] padding) whose state lives in a side table
// (g_controlRendererStates, detail/CollectionsStringsSupport.h).  So the bodies
// that touch m_Bitmap / m_Params (Create(int,...), Draw, DrawFrame) are left as
// documented stubs -- writing retail offsets into this object model would be
// out of bounds of the 104-byte OpenMFC object and invisible to the base class.

#include "detail/ManualSmallStubImplementationsSupport.h"

#include <atomic>
#include <cstddef>

// Base-class and allocator thunks (definitions verified in the tree):
//   featurepack/visualmanager/CMFCControlRenderer.cpp
extern "C" void* MS_ABI impl___0CMFCControlRenderer__QEAA_XZ(void* self);
extern "C" void  MS_ABI impl___1CMFCControlRenderer__UEAA_XZ(CMFCControlRenderer* self);
//   detail/MemcoreSupport.cpp  (MFC's ::operator new, ??2@YAPEAX_K@Z)
extern "C" void* MS_ABI impl___2_YAPEAX_K_Z(std::size_t size);

namespace {
// The vptr that OpenMFC's CMFCControlRenderer constructor thunk installs (a
// g++-emitted vtable).  Captured on the first CMFCShadowRenderer construction so
// the destructor can re-install it before chaining to the base destructor --
// the OpenMFC analogue of retail's "this->vfptr = own vftable; jmp base dtor".
// Standard C++ has no way to name a vtable, and spelling its mangled symbol
// here would add a C++-symbol dependency this thunk-only TU otherwise avoids.
std::atomic<void*> g_shadowRendererBaseVptr{nullptr};
} // namespace

// Constructor.  Retail ??0CMFCShadowRenderer@@QEAA@XZ, RVA 0x32dc0 (mfc140u):
//   call ??0CMFCControlRenderer (0x320c0 (mfc140u));
//   this->vfptr = CMFCShadowRenderer vftable (0x1802e3008 (mfc140u)); return this;
// DEVIATION: OpenMFC has no CMFCShadowRenderer C++ class and therefore no vtable
// of its own to install, so the object keeps the vtable the base constructor
// thunk installed.  DLL-internal virtual dispatch on it therefore reaches the
// CMFCControlRenderer methods, not the overrides below; and MSVC-compiled client
// code that dispatches through the retail slot numbers listed at the top of this
// file indexes a g++ vtable whose order differs (OpenMFC's CObject declares a
// virtual dtor, which g++ lays out as two entries at slots 1-2, and
// CMFCControlRenderer in afxmfc.h declares CleanUp right after Create).
// Symbol: ??0CMFCShadowRenderer@@QEAA@XZ
extern "C" void* MS_ABI impl___0CMFCShadowRenderer__QEAA_XZ(void* pThis) {
    impl___0CMFCControlRenderer__QEAA_XZ(pThis);
    g_shadowRendererBaseVptr.store(*static_cast<void**>(pThis), std::memory_order_relaxed);
    return pThis;
}

// Destructor.  Retail ??1CMFCShadowRenderer@@UEAA@XZ, RVA 0x32e40 (mfc140u), in full:
//   lea  vftable(0x1802e3008 (mfc140u)),%rax ; mov %rax,(%rcx) ; jmp ??1CMFCControlRenderer (0x32150)
// i.e. re-install its own vftable, then tail-call the base destructor.
// OpenMFC: re-install the vptr our constructor left in place (see
// g_shadowRendererBaseVptr), then call the base destructor thunk.  The base thunk
// destroys via a virtual `self->~CMFCControlRenderer()`, so the vptr reset is
// what keeps a client-derived object (whose MSVC vftable is in place at this
// point) from being dispatched through a foreign vtable.
// DEVIATIONS: a null pThis returns (retail would fault on the store); and if no
// CMFCShadowRenderer has been constructed in this process the captured vptr is
// null, pThis cannot be a constructed object, and nothing is chained.
// Symbol: ??1CMFCShadowRenderer@@UEAA@XZ
extern "C" void MS_ABI impl___1CMFCShadowRenderer__UEAA_XZ(void* pThis) {
    void* vptr = g_shadowRendererBaseVptr.load(std::memory_order_relaxed);
    if (pThis == nullptr || vptr == nullptr) return;
    *static_cast<void**>(pThis) = vptr;
    impl___1CMFCControlRenderer__UEAA_XZ(static_cast<CMFCControlRenderer*>(pThis));
}

// Create(const CMFCControlRendererInfo&, BOOL) -- protected override that
// disables the bitmap-resource path.  Export ordinal 3119 resolves (mfc140u
// export table) to RVA 0x71e0 (mfc140u), which is `xor %eax,%eax; ret` -- a
// COMDAT-folded body shared with ?AddRef@COleUILinkInfo@@UEAAKXZ, which is why
// the symbol map lists no RVA under this name.  Slot 5 of the vftable at
// 0x1802e3008 (mfc140u) points at the same 0x71e0.  Retail: return FALSE.
// Symbol: ?Create@CMFCShadowRenderer@@MEAAHAEBVCMFCControlRendererInfo@@H@Z
extern "C" int MS_ABI impl__Create_CMFCShadowRenderer__MEAAHAEBVCMFCControlRendererInfo__H_Z(
    void* pThis, const CMFCControlRendererInfo* params, int bFlipvert) {
    (void)pThis; (void)params; (void)bFlipvert;
    return FALSE;
}

// STUB.  Retail ?Create@CMFCShadowRenderer@@UEAAHHKHH@Z, RVA 0x32e50 (mfc140u)
// (0x32f00 (mfc140)), decoded:
//   this->CleanUp();                                  // vtable slot 12 (+0x60)
//   HBITMAP h = CDrawingManager::PrepareShadowMask(nDepth, clrBase, iMin, iMax);
//                                                     // 0x5beb0 (mfc140u), ordinal 11829
//   if (h == NULL) return FALSE;
//   n = max(nDepth, 3);
//   m_Params.m_rectImage   = CRect(0, 0, 2n+1, 2n+1);           // +0x1b0
//   m_Params.m_rectCorners = m_Params.m_rectSides = CRect(n,n,n,n); // +0x1c0/+0x1d0
//   m_Params.m_rectInter   = CRect(0,0,w,h) deflated by m_rectCorners // +0x1e0
//   m_Bitmap+0x68 = w; m_Bitmap+0x6c = h   (the image-size CSize, by position)
//   m_Bitmap +0x40 = m_Params.m_bPreMultiplyCheck; m_Bitmap +0x38 = 0;
//   m_Bitmap.AddImage(h, TRUE);                       // 0x16d880 (mfc140u)
//   ::DeleteObject(h);                                // GDI32!DeleteObject: slot 0x1802c6278
//                                                     // (mfc140u, iatu.py) / 0x1802c4238 (mfc140, iat.py)
//   return m_Bitmap.GetCount() == 1;                  // m_Bitmap+0x08
// Not transcribed: every store is into the retail m_Params / m_Bitmap layout,
// which OpenMFC's 104-byte side-table CMFCControlRenderer does not have, and
// there is no impl__ thunk for CDrawingManager::PrepareShadowMask in the tree.
// Symbol: ?Create@CMFCShadowRenderer@@UEAAHHKHH@Z
extern "C" int MS_ABI impl__Create_CMFCShadowRenderer__UEAAHHKHH_Z(
    void* pThis, int nDepth, unsigned long clrBase, int iMinBrightness, int iMaxBrightness) {
    (void)pThis; (void)nDepth; (void)clrBase; (void)iMinBrightness; (void)iMaxBrightness;
    return 0;
}

// CreateObject.  Retail RVA 0x32d80 (mfc140u), in full:
//   p = operator new(0x200);   // ??2@YAPEAX_K@Z
//   return p ? CMFCShadowRenderer::CMFCShadowRenderer(p) : NULL;
// Transcribed through the ??2 thunk and this file's constructor thunk (which
// carries the constructor deviation described above).  Note the CRuntimeClass
// descriptor (featurepack/visualmanager/RuntimeClasses.cpp) has a NULL
// m_pfnCreateObject, so RUNTIME_CLASS(CMFCShadowRenderer)->CreateObject() does not
// reach this export; only a direct call does.
// Symbol: ?CreateObject@CMFCShadowRenderer@@SAPEAVCObject@@XZ
extern "C" void* MS_ABI impl__CreateObject_CMFCShadowRenderer__SAPEAVCObject__XZ() {
    void* p = impl___2_YAPEAX_K_Z(0x200);
    if (p == nullptr) return nullptr;
    return impl___0CMFCShadowRenderer__QEAA_XZ(p);
}

// STUB.  Retail ?Draw@CMFCShadowRenderer@@UEAAXPEAVCDC@@VCRect@@IE@Z, RVA 0x32f90
// (mfc140u), decoded (CRect by value arrives as a pointer to the caller's copy):
//   CRect r;  d = m_Params.m_rectSides (+0x1d0..+0x1dc)
//   if (CMFCToolBarImages::m_bIsRTL)  { r.left = rect.left + d.left;  r.right = r.left + d.left; }
//   else                              { r.right = rect.right - d.right; r.left = r.right - d.right; }
//   r.bottom = rect.bottom - d.bottom;  r.top = r.bottom - d.bottom;
//   this->FillInterior(pDC, r, index, alphaSrc);      // vtable slot 9 (+0x48)
//   this->DrawFrame(pDC, rect, index, alphaSrc);      // vtable slot 7 (+0x38)
// Not transcribed: reads m_Params.m_rectSides at the retail offset, which the
// OpenMFC object does not have, and both calls are virtual calls whose MSVC
// slot numbers do not correspond to the g++ vtable an OpenMFC-constructed object
// carries.
// Symbol: ?Draw@CMFCShadowRenderer@@UEAAXPEAVCDC@@VCRect@@IE@Z
extern "C" void MS_ABI impl__Draw_CMFCShadowRenderer__UEAAXPEAVCDC__VCRect__IE_Z(
    void* pThis, void* pDC, const RECT* rect, unsigned int index, unsigned char alphaSrc) {
    (void)pThis; (void)pDC; (void)rect; (void)index; (void)alphaSrc;
}

// STUB.  Retail ?DrawFrame@CMFCShadowRenderer@@UEAAXPEAVCDC@@VCRect@@IE@Z, RVA
// 0x33050 (mfc140u) (0x33100 (mfc140)).  Observed, not fully transcribed: it
// copies m_Params.m_rectImage (+0x1b0); if m_Bitmap's image count (this+0x10,
// i.e. m_Bitmap+0x08) == 1 it ::OffsetRect()s that copy by (0, index * image
// height) (USER32!OffsetRect, slot 0x1802c72f0 (mfc140u), iatu.py) and then
// passes index 0, not index, to the DrawEx calls; it then computes the frame
// pieces from m_Params.m_rectCorners (+0x1c0..+0x1cc) and m_rectSides (+0x1d0,
// +0x1d8, +0x1dc), branches on CMFCToolBarImages::m_bIsRTL (0x3be390 (mfc140u),
// ordinal 8260), and paints the pieces with CMFCToolBarImages::DrawEx
// (0x16d0b0 (mfc140u), ordinal 4058) on m_Bitmap -- six call sites in the
// listing.  Left a stub for the same layout reason as Create(int,...) above.
// Symbol: ?DrawFrame@CMFCShadowRenderer@@UEAAXPEAVCDC@@VCRect@@IE@Z
extern "C" void MS_ABI impl__DrawFrame_CMFCShadowRenderer__UEAAXPEAVCDC__VCRect__IE_Z(
    void* pThis, void* pDC, const RECT* rect, unsigned int index, unsigned char alphaSrc) {
    (void)pThis; (void)pDC; (void)rect; (void)index; (void)alphaSrc;
}

// OnSysColorChange.  Retail is an EMPTY override: export ordinal 11316 resolves
// (mfc140u export table) to RVA 0x27d0 (mfc140u), a bare `ret` COMDAT-folded
// with ?AddDockSite@CFrameWndEx@@QEAAXXZ; slot 10 of the vftable at 0x1802e3008
// (mfc140u) points at the same 0x27d0.  (The base CMFCControlRenderer slot 10,
// 0x32be0 (mfc140u), calls m_Bitmap.OnSysColorChange() (0x16d500 (mfc140u))
// when the 8-byte field at m_Bitmap+0xa0 is non-null; the shadow renderer does
// not.)  The empty body is the transcription.
// Symbol: ?OnSysColorChange@CMFCShadowRenderer@@UEAAXXZ
extern "C" void MS_ABI impl__OnSysColorChange_CMFCShadowRenderer__UEAAXXZ(void* pThis) {
    (void)pThis;
}
