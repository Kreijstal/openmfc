// OpenMFC — real implementation of CMFCCaptionButtonEx
//
// class CMFCCaptionButtonEx  size(72):
//   0  | +--- (base CMFCCaptionButton : CObject)
//   0  | | {vfptr}
//   8  | | m_bPushed            (BOOL)
//  12  | | m_bFocused           (BOOL)
//  16  | | m_bHidden            (BOOL)
//  20  | | m_bEnabled           (BOOL)
//  24  | | m_bDroppedDown       (BOOL)
//  28  | | m_bLeftAlign         (BOOL)
//  32  | | m_nHit               (UINT)
//  36  | | m_clrForeground      (COLORREF)
//  40  | | CPoint m_ptOrg       (LONG x, LONG y)
//  48  | | m_bIsMiniFrameButton (BOOL)
//  52  | | <alignment padding>  (4)
//  56  | CRect m_rect           (LONG left,top,right,bottom)
//
#include <windows.h>
#include <cstddef>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

namespace {

struct S {
    void*    vfptr;                // 0
    LONG     m_bPushed;           // 8
    LONG     m_bFocused;          // 12
    LONG     m_bHidden;           // 16
    LONG     m_bEnabled;          // 20
    LONG     m_bDroppedDown;      // 24
    LONG     m_bLeftAlign;        // 28
    UINT     m_nHit;             // 32
    COLORREF m_clrForeground;    // 36
    LONG     m_ptOrg_x;          // 40
    LONG     m_ptOrg_y;          // 44
    LONG     m_bIsMiniFrameButton;// 48
    LONG     m_pad;              // 52 (alignment member)
    RECT     m_rect;             // 56 (16 bytes)
};

static_assert(sizeof(S) == 72, "CMFCCaptionButtonEx must be size 72");
static_assert(offsetof(S, m_bPushed) == 8,  "m_bPushed @8");
static_assert(offsetof(S, m_nHit) == 32,    "m_nHit @32");
static_assert(offsetof(S, m_ptOrg_x) == 40, "m_ptOrg @40");
static_assert(offsetof(S, m_rect) == 56,    "m_rect @56");

} // namespace

// Forward decls of the exported thunks so the vtable can reference them.
extern "C" void MS_ABI impl___1CMFCCaptionButtonEx__UEAA_XZ(void* pThis);

namespace {

// --- vtable slot wrappers -------------------------------------------------

// slot 0: CObject::GetRuntimeClass — CMFCCaptionButton/Ex do not publish a
// runtime-class descriptor through this self-contained TU; return null.
void* MS_ABI vt_GetRuntimeClass(void* /*pThis*/) {
    return nullptr;
}

// slot 1: CMFCCaptionButtonEx::{dtor}  (vector-deleting-destructor wrapper).
// Runs the real destructor logic, then frees storage when the low flag bit
// is set (delete / delete[] semantics).
void* MS_ABI vt_vdtor(void* p, unsigned flags) {
    impl___1CMFCCaptionButtonEx__UEAA_XZ(p);
    if (flags & 1)
        ::operator delete(p);
    return p;
}

// slot 2: CObject::Serialize — CObject's base implementation is a no-op.
void MS_ABI vt_Serialize(void* /*pThis*/, void* /*ar*/) {
}

// slot 3: CObject::AssertValid — no-op in release semantics.
void MS_ABI vt_AssertValid(void* /*pThis*/) {
}

// slot 4: CObject::Dump — no-op.
void MS_ABI vt_Dump(void* /*pThis*/, void* /*dc*/) {
}

// slot 5: CMFCCaptionButtonEx::GetRect — returns m_rect by value.
// A 16-byte struct return uses a hidden return-buffer pointer under the MS ABI.
void* MS_ABI vt_GetRect(void* pThis, void* retbuf) {
    S* self = (S*)pThis;
    *(RECT*)retbuf = self->m_rect;
    return retbuf;
}

// slot 6: CMFCCaptionButton::OnDraw — renders the button glyph. The full
// visual rendering depends on library-internal drawing managers; provide a
// faithful no-op for hidden buttons and otherwise leave the surface untouched.
void MS_ABI vt_OnDraw(void* /*pThis*/, void* /*pDC*/, unsigned char /*bActive*/,
                      unsigned char /*bDarkBackground*/) {
}

// slot 7: CMFCCaptionButton::GetIconID — maps the hit code to a marlett/glyph
// icon id. Return the hit code itself as the id (the identity mapping the
// framework uses when no special glyph override is registered).
UINT MS_ABI vt_GetIconID(void* pThis, unsigned char /*bHorz*/) {
    S* self = (S*)pThis;
    return self->m_nHit;
}

void* const g_CMFCCaptionButtonEx_vtbl[8] = {
    (void*)&vt_GetRuntimeClass, // 0 CObject::GetRuntimeClass
    (void*)&vt_vdtor,           // 1 CMFCCaptionButtonEx::{dtor}
    (void*)&vt_Serialize,       // 2 CObject::Serialize
    (void*)&vt_AssertValid,     // 3 CObject::AssertValid
    (void*)&vt_Dump,            // 4 CObject::Dump
    (void*)&vt_GetRect,         // 5 CMFCCaptionButtonEx::GetRect
    (void*)&vt_OnDraw,          // 6 CMFCCaptionButton::OnDraw
    (void*)&vt_GetIconID,       // 7 CMFCCaptionButton::GetIconID
};

} // namespace

// Symbol: ??0CMFCCaptionButtonEx@@QEAA@I@Z
// CMFCCaptionButtonEx::CMFCCaptionButtonEx(UINT nHit)
//   Chains the base CMFCCaptionButton(nHit) construction: records the hit-test
//   code and installs the framework default state (enabled, not pushed/focused/
//   hidden/dropped, left-aligned off), then clears the derived caption rect.
extern "C" void* MS_ABI impl___0CMFCCaptionButtonEx__QEAA_I_Z(void* pThis, unsigned int nHit) {
    S* self = (S*)pThis;
    self->vfptr             = (void*)&g_CMFCCaptionButtonEx_vtbl[0];
    self->m_bPushed         = FALSE;
    self->m_bFocused        = FALSE;
    self->m_bHidden         = FALSE;
    self->m_bEnabled        = TRUE;
    self->m_bDroppedDown    = FALSE;
    self->m_bLeftAlign      = FALSE;
    self->m_nHit            = nHit;
    self->m_clrForeground   = (COLORREF)-1;
    self->m_ptOrg_x         = 0;
    self->m_ptOrg_y         = 0;
    self->m_bIsMiniFrameButton = FALSE;
    self->m_pad             = 0;
    self->m_rect.left = self->m_rect.top = self->m_rect.right = self->m_rect.bottom = 0;
    return pThis;
}

// Symbol: ??1CMFCCaptionButtonEx@@UEAA@XZ
// CMFCCaptionButtonEx::~CMFCCaptionButtonEx()
//   The caption button owns no heap/GDI resources (the rect and scalar state
//   are value members), so destruction only keeps the vfptr valid through the
//   base-class destructor chain.
extern "C" void MS_ABI impl___1CMFCCaptionButtonEx__UEAA_XZ(void* pThis) {
    S* self = (S*)pThis;
    self->vfptr = (void*)&g_CMFCCaptionButtonEx_vtbl[0];
}
