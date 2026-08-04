// CMFCTasksPanePropertyPage — OpenMFC implementation.
// Sources: global_cmfctaskspanepropertypage.cpp, manual_small_stub_implementations.cpp

#include "detail/CMFCTasksPanePropertyPageSupport.h"
#include "detail/ManualSmallStubImplementationsSupport.h"

// Implemented in manual_small_stub_implementations.cpp; this unit had a second, unlinked copy.
// Symbol: ?SetACCData@CMFCTasksPanePropertyPage@@UEAAHPEAVCWnd@@AEAVCAccessibilityData@@@Z
// Symbol: ?SetACCData@CMFCTasksPanePropertyPage@@UEAAHPEAVCWnd@@AEAVCAccessibilityData@@@Z
extern "C" int MS_ABI impl__SetACCData_CMFCTasksPanePropertyPage__UEAAHPEAVCWnd__AEAVCAccessibilityData___Z(
    void* pThis, void* pWindow, void* pData) {
    if (pThis == nullptr || pData == nullptr) {
        return 0;
    }

    struct CTaskPanePageLayout {
        void* vfptr;
        void* m_strName;
        void* m_pTaskPane;
    };

    auto* self = static_cast<CTaskPanePageLayout*>(pThis);
    auto* data = static_cast<CAccessibilityData*>(pData);
    data->m_strAccName = self->m_strName ? *static_cast<CString*>(self->m_strName) : CString();
    data->m_strAccValue.Empty();
    data->m_strDescription.Empty();
    data->m_strAccKeys.Empty();
    data->m_strAccHelp.Empty();
    data->m_strAccDefAction.Empty();
    data->m_nAccRole = 0;
    data->m_bAccState = 0;
    data->m_nAccHit = 0;

    if (pWindow != nullptr) {
        HWND hParent = *reinterpret_cast<HWND*>(static_cast<char*>(pWindow) + 64);
        if (::IsWindow(hParent)) {
            RECT rc {};
            if (::GetClientRect(hParent, &rc)) {
                POINT ptTL { rc.left, rc.top };
                POINT ptBR { rc.right, rc.bottom };
                ::ClientToScreen(hParent, &ptTL);
                ::ClientToScreen(hParent, &ptBR);
                data->m_rectAccLocation = CRect(ptTL.x, ptTL.y, ptBR.x, ptBR.y);
                data->m_ptAccHit.x = (ptTL.x + ptBR.x) / 2;
                data->m_ptAccHit.y = (ptTL.y + ptBR.y) / 2;
            }
        }
    }

    return 1;
}
