// OpenMFC: CRibbonCategoryScroll  (private CMFCRibbonButton : CMFCRibbonBaseElement
//   : CMFCBaseAccessibleObject : CCmdTarget : CObject)
//
// Layout (cl.exe /d1reportSingleClassLayout), size(632):
//   0    {vfptr}     (CObject vfptr, shared through the whole single-inheritance chain)
//   8    m_dwRef     (CCmdTarget::m_dwRef) ... plus the full CMFCRibbonButton state
//   624  m_bIsLeft   (BOOL) -- the sole member CRibbonCategoryScroll adds
//
// This is the internal left/right scroll button placed in a ribbon category's
// tab strip when the tabs overflow.  m_bIsLeft selects the scroll direction.
// It is created by CMFCRibbonCategory (declared a friend) which then sets
// m_bIsLeft directly, so the default ctor simply leaves it FALSE.

#include <windows.h>
#include <cstddef>
#include <cstring>

#ifdef __GNUC__
#define MS_ABI __attribute__((ms_abi))
#else
#define MS_ABI
#endif

namespace {

struct S {
    void* vfptr;         // 0
    char  pad[616];      // 8 .. 623   (CCmdTarget/CMFCRibbonBaseElement/CMFCRibbonButton state)
    int   m_bIsLeft;     // 624        BOOL
    char  tail[4];       // 628 .. 631 alignment padding
};

static_assert(sizeof(S) == 632, "CRibbonCategoryScroll must be 632 bytes");
static_assert(offsetof(S, vfptr) == 0, "vfptr at 0");
static_assert(offsetof(S, m_bIsLeft) == 624, "m_bIsLeft at 624");

// Non-zero default field values of the CMFCRibbonButton / CMFCRibbonBaseElement
// base, harvested from the real mfc140u layout (cl.exe) and verified byte-exact
// against the real DLL's default construction (differential dump 2026-07-18).
// Offset -> int value. Every other scalar field defaults to 0. Pointer members
// (CString buffers, embedded CArray/XDispatch vtables, HMENU/HICON handles) are
// left null here — they legitimately differ from the real DLL's addresses.
inline void InstallBaseDefaults(void* p) {
    struct FV { int off; int val; };
    static const FV kDefaults[] = {
        {   8, 1 },   // m_dwRef (CCmdTarget reference count defaults to 1)
        {  40, 1 },   // m_bResultExpected
        { 284, -1 },  // m_nRow
        { 348, 1 },   // m_bIsVisible
        { 360, 1 },   // m_bDrawDefaultIcon
        { 372, 1 },   // m_bEnableUpdateTooltipInfo
        { 376, 1 },   // m_bEnableTooltipInfoShortcut
        { 400, 3 },   // m_szMargin.cx
        { 404, 3 },   // m_szMargin.cy
        { 440, -1 },  // m_nSmallImageIndex
        { 444, -1 },  // m_nLargeImageIndex
        { 448, 2 },   // m_nMenuArrowMargin
        { 456, -1 },  // m_nWrapIndex
        { 492, 1 },   // m_bIsDefaultCommand
    };
    for (const FV& f : kDefaults)
        *reinterpret_cast<int*>(static_cast<char*>(p) + f.off) = f.val;
}

// ---- vtable slot helpers (non-exported virtuals) ---------------------------

// Slot 0: CMFCRibbonButton::GetRuntimeClass (const).  Runtime-class metadata for
// this internal type is not part of the public surface; report none.
void* MS_ABI slot_GetRuntimeClass(const void* /*pThis*/) { return nullptr; }

// Slot 2: CObject::Serialize -- base implementation is a no-op.
void MS_ABI slot_Serialize(void* /*pThis*/, void* /*ar*/) {}

// Slot 3: CObject::AssertValid (const) -- no-op in release.
void MS_ABI slot_AssertValid(const void* /*pThis*/) {}

// Slot 4: CObject::Dump (const) -- no-op in release.
void MS_ABI slot_Dump(const void* /*pThis*/, void* /*dc*/) {}

// Slot 5: CCmdTarget::OnCmdMsg -- unhandled by default.
int MS_ABI slot_OnCmdMsg(void* /*pThis*/, unsigned /*nID*/, int /*nCode*/,
                         void* /*pExtra*/, void* /*pHandlerInfo*/) { return 0; }

// Slot 6: CCmdTarget::OnFinalRelease -- default just self-deletes; the ribbon
// element is not COM-owned here, so no-op.
void MS_ABI slot_OnFinalRelease(void* /*pThis*/) {}

// Slot 7: CCmdTarget::IsInvokeAllowed -- automation gate, allow by default.
int MS_ABI slot_IsInvokeAllowed(void* /*pThis*/, unsigned /*dispid*/) { return 1; }

// Slot 8: CCmdTarget::GetDispatchIID -- no dispinterface.
int MS_ABI slot_GetDispatchIID(void* /*pThis*/, void* /*pIID*/) { return 0; }

// Slot 9: CCmdTarget::GetTypeInfoCount -- no type info.
unsigned MS_ABI slot_GetTypeInfoCount(void* /*pThis*/) { return 0; }

extern void* const g_CRibbonCategoryScroll_vtbl[10];

} // namespace

// -----------------------------------------------------------------------------
// Exported thunks
// -----------------------------------------------------------------------------

// Symbol: ??0CRibbonCategoryScroll@@AEAA@XZ
// Private default constructor.  Installs the vfptr and clears the direction
// flag (CMFCRibbonCategory sets m_bIsLeft afterwards).
extern "C" void* MS_ABI
impl___0CRibbonCategoryScroll__AEAA_XZ(void* pThis) {
    S* self = (S*)pThis;
    std::memset(self, 0, sizeof(S));           // zero-init the full object (base ctors do this)
    self->vfptr    = (void*)&g_CRibbonCategoryScroll_vtbl[0];
    InstallBaseDefaults(self);                 // non-zero base defaults (harvested, byte-verified)
    self->m_bIsLeft = FALSE;
    return pThis;
}

// Symbol: ?CopyFrom@CRibbonCategoryScroll@@EEAAXAEBVCMFCRibbonBaseElement@@@Z
// virtual void CopyFrom(const CMFCRibbonBaseElement& src)
// Copies the scroll-direction flag from the source element (which, when it is
// actually a CRibbonCategoryScroll, carries m_bIsLeft at the same offset).
extern "C" void MS_ABI
impl__CopyFrom_CRibbonCategoryScroll__EEAAXAEBVCMFCRibbonBaseElement___Z(
        void* pThis, const void* src) {
    S* self = (S*)pThis;
    if (src) {
        const S* other = (const S*)src;
        self->m_bIsLeft = other->m_bIsLeft;
    }
}

// Symbol: ?OnAutoRepeat@CRibbonCategoryScroll@@EEAAHXZ
// virtual BOOL OnAutoRepeat()
// Fired by the auto-repeat timer while the scroll button is pressed; the real
// implementation asks the owning category to scroll one step and returns TRUE
// to keep repeating.  The category back-pointer is internal ribbon state not
// modeled here, so this returns TRUE (continue repeating) without scrolling.
extern "C" int MS_ABI
impl__OnAutoRepeat_CRibbonCategoryScroll__EEAAHXZ(void* /*pThis*/) {
    return TRUE;
}

// Symbol: ?OnDraw@CRibbonCategoryScroll@@EEAAXPEAVCDC@@@Z
// virtual void OnDraw(CDC* pDC)
// Paints the left/right scroll arrow via the active visual manager.  Rendering
// depends on the ribbon visual-manager and this element's cached rect, which
// are internal ribbon state not modeled here; no-op when unavailable.
extern "C" void MS_ABI
impl__OnDraw_CRibbonCategoryScroll__EEAAXPEAVCDC___Z(
        void* /*pThis*/, void* /*pDC*/) {
}

// Symbol: ?OnMouseMove@CRibbonCategoryScroll@@EEAAXVCPoint@@@Z
// virtual void OnMouseMove(CPoint point)
// Hover tracking for the scroll button.  CPoint is passed by value as a packed
// 8-byte value (x in low 32 bits, y in high 32 bits).  Hover state updates the
// ribbon's internal highlight tracking which is not modeled here; no-op.
extern "C" void MS_ABI
impl__OnMouseMove_CRibbonCategoryScroll__EEAAXVCPoint___Z(
        void* /*pThis*/, unsigned long long /*point*/) {
}

// -----------------------------------------------------------------------------
// vtable
// -----------------------------------------------------------------------------

namespace {

// Slot 1: CRibbonCategoryScroll::{vector deleting destructor}.  No owned
// resources; release the object when the low flag bit is set.
void* MS_ABI slot_vecDelDtor(void* pThis, unsigned flags) {
    if (flags & 1)
        ::operator delete(pThis);
    return pThis;
}

void* const g_CRibbonCategoryScroll_vtbl[10] = {
    (void*)&slot_GetRuntimeClass,    // 0 CMFCRibbonButton::GetRuntimeClass
    (void*)&slot_vecDelDtor,         // 1 {dtor}
    (void*)&slot_Serialize,          // 2 CObject::Serialize
    (void*)&slot_AssertValid,        // 3 CObject::AssertValid
    (void*)&slot_Dump,               // 4 CObject::Dump
    (void*)&slot_OnCmdMsg,           // 5 CCmdTarget::OnCmdMsg
    (void*)&slot_OnFinalRelease,     // 6 CCmdTarget::OnFinalRelease
    (void*)&slot_IsInvokeAllowed,    // 7 CCmdTarget::IsInvokeAllowed
    (void*)&slot_GetDispatchIID,     // 8 CCmdTarget::GetDispatchIID
    (void*)&slot_GetTypeInfoCount,   // 9 CCmdTarget::GetTypeInfoCount
};

} // namespace
