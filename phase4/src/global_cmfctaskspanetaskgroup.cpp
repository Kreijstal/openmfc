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
#include "openmfc/afxmfc.h"

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

struct CRuntimeClass;
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CObject__UEBAPEAUCRuntimeClass__XZ(const void* pThis);

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

struct OpenMfcRuntimeClass {
    const char* m_lpszClassName;
    int m_nObjectSize;
    unsigned short m_wSchema;
    void* m_pfnCreateObject;
    void* m_pfnGetBaseClass;
    OpenMfcRuntimeClass* m_pBaseClass;
    void* m_pClassContext;
};

static OpenMfcRuntimeClass g_CMFCTasksPaneTaskGroup_rtti = {
    "CMFCTasksPaneTaskGroup",
    sizeof(S),
    0xFFFF,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
};

static OpenMfcRuntimeClass* GetBaseClass() {
    static OpenMfcRuntimeClass* base = reinterpret_cast<OpenMfcRuntimeClass*>(
        impl__GetRuntimeClass_CObject__UEBAPEAUCRuntimeClass__XZ(nullptr));
    return base ? base : nullptr;
}

} // namespace

// Forward decl so the vtable can reference the exported thunk.
extern "C" int MS_ABI
impl__SetACCData_CMFCTasksPaneTaskGroup__UEAAHPEAVCWnd__AEAVCAccessibilityData___Z(
    void* pThis, void* pParent, void* pData);

namespace {

// --- CObject base-slot wrappers (no exports for these) --------------------

// GetRuntimeClass: no CRuntimeClass descriptor is authored for this helper
// class; install a local descriptor with CObject as base.
const void* MS_ABI vslot_GetRuntimeClass(void* /*p*/) {
    if (!g_CMFCTasksPaneTaskGroup_rtti.m_pBaseClass) {
        g_CMFCTasksPaneTaskGroup_rtti.m_pBaseClass = GetBaseClass();
    }
    return &g_CMFCTasksPaneTaskGroup_rtti;
}

// Serialize(CArchive&): CMFCTasksPaneTaskGroup is not serializable; the base
// CObject::Serialize is a no-op.
void MS_ABI vslot_Serialize(void* pThis, void* pAr) {
    if (!pThis || !pAr) return;
    static_cast<CObject*>(pThis)->CObject::Serialize(*static_cast<CArchive*>(pAr));
}

void MS_ABI vslot_AssertValid(void* pThis) {
    if (!pThis) return;
    static_cast<CObject*>(pThis)->CObject::AssertValid();
}
void MS_ABI vslot_Dump(void* pThis, void* /*dc*/) {
    if (!pThis) return;
    static_cast<CObject*>(pThis)->CObject::Dump();
}

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
//   supplied CAccessibilityData. This implementation now mirrors the key observable
//   fields (name, state, and location) that are compatible with the header-shimmed
//   CAccessibilityData shape.
extern "C" int MS_ABI
impl__SetACCData_CMFCTasksPaneTaskGroup__UEAAHPEAVCWnd__AEAVCAccessibilityData___Z(
    void* pThis, void* pParent, void* pData) {
    if (!pThis || !pData)
        return FALSE;

    const S* self = static_cast<const S*>(pThis);
    CAccessibilityData* data = static_cast<CAccessibilityData*>(pData);
    data->m_strAccName = self->m_strName ? *static_cast<CString*>(self->m_strName) : CString();
    data->m_strAccValue.Empty();
    data->m_strDescription.Empty();
    data->m_strAccKeys.Empty();
    data->m_strAccHelp.Empty();
    data->m_strAccDefAction.Empty();
    data->m_nAccRole = 0;
    data->m_bAccState = self->m_bIsCollapsed ? 0 : 1;
    data->m_nAccHit = 0;

    CRect rect(self->m_rect.left, self->m_rect.top, self->m_rect.right, self->m_rect.bottom);
    if (pParent) {
        CWnd* pWnd = static_cast<CWnd*>(pParent);
        HWND hWnd = pWnd->GetSafeHwnd();
        if (hWnd) {
            POINT tl{rect.left, rect.top};
            POINT br{rect.right, rect.bottom};
            ::ClientToScreen(hWnd, &tl);
            ::ClientToScreen(hWnd, &br);
            rect = CRect(tl.x, tl.y, br.x, br.y);
        }
    }

    data->m_rectAccLocation = rect;
    data->m_ptAccHit.x = (rect.left + rect.right) / 2;
    data->m_ptAccHit.y = (rect.top + rect.bottom) / 2;
    return TRUE;
}
