// CMFCTasksPaneTaskGroup — OpenMFC implementation.
// Sources: global_cmfctaskspanetaskgroup.cpp, manual_small_stub_implementations.cpp, mfccore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/CMFCTasksPaneTaskGroupSupport.h"
#include "detail/ManualSmallStubImplementationsSupport.h"
#include "detail/MfccoreSupport.h"

// Implemented in manual_small_stub_implementations.cpp; this unit had a second, unlinked copy.
// Symbol: ?SetACCData@CMFCTasksPaneTaskGroup@@UEAAHPEAVCWnd@@AEAVCAccessibilityData@@@Z
// Symbol: ?SetACCData@CMFCTasksPaneTaskGroup@@UEAAHPEAVCWnd@@AEAVCAccessibilityData@@@Z
extern "C" int MS_ABI impl__SetACCData_CMFCTasksPaneTaskGroup__UEAAHPEAVCWnd__AEAVCAccessibilityData___Z(
    void* pThis, void* pWindow, void* pData) {
    if (pThis == nullptr || pData == nullptr) {
        return 0;
    }

    struct CTaskGroupLayout {
        void* vfptr;
        void* m_pPage;
        void* m_strName;
        char _lstTasks[56];
        int m_bIsBottom;
        int m_bIsSpecial;
        int m_bIsCollapsed;
        RECT m_rect;
        RECT m_rectGroup;
        SIZE m_sizeIcon;
        HICON m_hIcon;
        unsigned long m_clrText;
        unsigned long m_clrTextHot;
    };

    auto* self = static_cast<CTaskGroupLayout*>(pThis);
    auto* data = static_cast<CAccessibilityData*>(pData);
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
    if (pWindow != nullptr) {
        HWND hWnd = *reinterpret_cast<HWND*>(static_cast<char*>(pWindow) + 64);
        if (::IsWindow(hWnd)) {
            POINT ptTL { rect.left, rect.top };
            POINT ptBR { rect.right, rect.bottom };
            ::ClientToScreen(hWnd, &ptTL);
            ::ClientToScreen(hWnd, &ptBR);
            rect = CRect(ptTL.x, ptTL.y, ptBR.x, ptBR.y);
        }
    }

    data->m_rectAccLocation = rect;
    data->m_ptAccHit.x = (rect.left + rect.right) / 2;
    data->m_ptAccHit.y = (rect.top + rect.bottom) / 2;

    return 1;
}
CMFCTasksPaneTaskGroup::CMFCTasksPaneTaskGroup() { memset(_pad, 0, sizeof(_pad)); }
CMFCTasksPaneTaskGroup::~CMFCTasksPaneTaskGroup() {}

// CMFCTasksPaneTaskGroup's MSVC-layout vtable: defined here, next to the
// implementations it points at.
namespace openmfc { namespace detail { namespace cmfctaskspanetaskgroup {
extern void* const g_CMFCTasksPaneTaskGroup_vtbl[6] = {
    (void*)&vslot_GetRuntimeClass_Cmfctaskspanetaskgroup,   // 0 CObject::GetRuntimeClass
    (void*)&vdtor_Cmfctaskspanetaskgroup,                   // 1 CMFCTasksPaneTaskGroup::{dtor}
    (void*)&vslot_Serialize_Cmfctaskspanetaskgroup,         // 2 CObject::Serialize
    (void*)&vslot_AssertValid_Cmfctaskspanetaskgroup,       // 3 CObject::AssertValid
    (void*)&vslot_Dump_Cmfctaskspanetaskgroup,              // 4 CObject::Dump
    (void*)&impl__SetACCData_CMFCTasksPaneTaskGroup__UEAAHPEAVCWnd__AEAVCAccessibilityData___Z, // 5 SetACCData
};
} } }
