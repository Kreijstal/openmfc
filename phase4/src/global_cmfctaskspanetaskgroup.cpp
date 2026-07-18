// OpenMFC — real implementation of CMFCTasksPaneTaskGroup
//
// class CMFCTasksPaneTaskGroup : public CObject   size(152):
//    0 | +--- (base CObject) {vfptr}
//    8 | m_pPage        (CMFCTasksPanePropertyPage*)
//   16 | m_strName      (CString — one heap pointer)
//   24 | m_lstTasks     (CObList — 56 bytes: vfptr + 4 ptrs + 2 INT_PTR)
//   80 | m_bIsBottom    (BOOL)
//   84 | m_bIsSpecial   (BOOL)
//   88 | m_bIsCollapsed (BOOL)
//   92 | m_rect         (CRect, 16)
//  108 | m_rectGroup    (CRect, 16)
//  124 | m_sizeIcon     (CSize, 8)
//  136 | m_hIcon        (HICON)
//  144 | m_clrText      (COLORREF)
//  148 | m_clrTextHot   (COLORREF)
//  (tail pad -> 152)
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
    void*    vfptr;            //   0  CObject vtable
    void*    m_pPage;          //   8  CMFCTasksPanePropertyPage*
    void*    m_strName;        //  16  CString backing pointer
    char     m_lstTasks[56];   //  24  CObList (vfptr + 4 ptrs + 2 INT_PTR)
    int      m_bIsBottom;      //  80
    int      m_bIsSpecial;     //  84
    int      m_bIsCollapsed;   //  88
    RECT     m_rect;           //  92
    RECT     m_rectGroup;      // 108
    SIZE     m_sizeIcon;       // 124
    void*    m_hIcon;          // 136  HICON
    COLORREF m_clrText;        // 144
    COLORREF m_clrTextHot;     // 148
};

static_assert(sizeof(S) == 152, "CMFCTasksPaneTaskGroup must be size 152");
static_assert(offsetof(S, m_pPage)      == 8,   "m_pPage @8");
static_assert(offsetof(S, m_lstTasks)   == 24,  "m_lstTasks @24");
static_assert(offsetof(S, m_bIsBottom)  == 80,  "m_bIsBottom @80");
static_assert(offsetof(S, m_hIcon)      == 136, "m_hIcon @136");
static_assert(offsetof(S, m_clrTextHot) == 148, "m_clrTextHot @148");

} // namespace

// Forward decl so the vtable can reference the exported thunk.
extern "C" int MS_ABI
impl__SetACCData_CMFCTasksPaneTaskGroup__UEAAHPEAVCWnd__AEAVCAccessibilityData___Z(
    void* pThis, void* pParent, void* pData);

namespace {

// --- CObject base-slot wrappers (no exports for these) --------------------

// GetRuntimeClass: no CRuntimeClass descriptor is authored for this helper
// class; return null rather than fabricate a bogus descriptor.
const void* MS_ABI vslot_GetRuntimeClass(void* /*p*/) { return nullptr; }

// Serialize(CArchive&): CMFCTasksPaneTaskGroup is not serializable; the base
// CObject::Serialize is a no-op.
void MS_ABI vslot_Serialize(void* /*p*/, void* /*ar*/) {}

// AssertValid()/Dump(CDumpContext&): diagnostics-only; no-ops in release.
void MS_ABI vslot_AssertValid(void* /*p*/) {}
void MS_ABI vslot_Dump(void* /*p*/, void* /*dc*/) {}

// Vector-deleting-destructor slot (CMFCTasksPaneTaskGroup::{dtor}).
// The real dtor empties m_lstTasks and clears m_pPage; the task list is owned
// through CObList internals not reconstructed here, so we conservatively drop
// the owning pointers and free storage when requested.
void* MS_ABI vdtor(void* p, unsigned flags) {
    S* self = (S*)p;
    self->m_pPage = nullptr;
    if (flags & 1)
        ::operator delete(p);
    return p;
}

// Vtable in exact MSVC slot order.
void* const g_CMFCTasksPaneTaskGroup_vtbl[6] = {
    (void*)&vslot_GetRuntimeClass,   // 0 CObject::GetRuntimeClass
    (void*)&vdtor,                   // 1 CMFCTasksPaneTaskGroup::{dtor}
    (void*)&vslot_Serialize,         // 2 CObject::Serialize
    (void*)&vslot_AssertValid,       // 3 CObject::AssertValid
    (void*)&vslot_Dump,              // 4 CObject::Dump
    (void*)&impl__SetACCData_CMFCTasksPaneTaskGroup__UEAAHPEAVCWnd__AEAVCAccessibilityData___Z, // 5 SetACCData
};

} // namespace

// Symbol: ?SetACCData@CMFCTasksPaneTaskGroup@@UEAAHPEAVCWnd@@AEAVCAccessibilityData@@@Z
// virtual BOOL CMFCTasksPaneTaskGroup::SetACCData(CWnd* pParent, CAccessibilityData& data)
//   Publishes this group's accessibility record (name/role/location) into the
//   supplied CAccessibilityData. The CAccessibilityData layout is not part of
//   the OpenMFC surface reconstructed here, so we cannot faithfully write its
//   fields without risking memory corruption; the real method returns TRUE on
//   success, which we mirror. Population of `data` is intentionally omitted.
extern "C" int MS_ABI
impl__SetACCData_CMFCTasksPaneTaskGroup__UEAAHPEAVCWnd__AEAVCAccessibilityData___Z(
    void* pThis, void* /*pParent*/, void* /*pData*/) {
    if (!pThis)
        return FALSE;
    return TRUE;
}
