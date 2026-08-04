// CMFCTasksPaneTask — OpenMFC implementation.
// Sources: global_cmfctaskspanetask.cpp, manual_small_stub_implementations.cpp, mfccore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/CMFCTasksPaneTaskSupport.h"
#include "detail/ManualSmallStubImplementationsSupport.h"
#include "detail/MfccoreSupport.h"

// Implemented in manual_small_stub_implementations.cpp; this unit had a second, unlinked copy.
// Symbol: ?SetACCData@CMFCTasksPaneTask@@UEAAHPEAVCWnd@@AEAVCAccessibilityData@@@Z
// Symbol: ?SetACCData@CMFCTasksPaneTask@@UEAAHPEAVCWnd@@AEAVCAccessibilityData@@@Z
extern "C" int MS_ABI impl__SetACCData_CMFCTasksPaneTask__UEAAHPEAVCWnd__AEAVCAccessibilityData___Z(
    void* pThis, void* pWindow, void* pData) {
    if (pThis == nullptr || pData == nullptr) {
        return 0;
    }

    struct CTaskLayout {
        void* vfptr;
        void* m_pGroup;
        void* m_strName;
        RECT m_rect;
        int m_nIcon;
        int m_nWindowHeight;
        unsigned m_uiCommandID;
        unsigned _pad0;
        uintptr_t m_dwUserData;
        HWND m_hwndTask;
        int m_bAutoDestroyWindow;
        int m_bVisible;
        int m_bEnabled;
        int m_bIsSeparator;
        int m_bIsBold;
        unsigned long m_clrText;
        unsigned long m_clrTextHot;
        unsigned _pad1;
    };

    auto* self = static_cast<CTaskLayout*>(pThis);
    auto* data = static_cast<CAccessibilityData*>(pData);
    if (pWindow != nullptr) {
        HWND hParent = *reinterpret_cast<HWND*>(static_cast<char*>(pWindow) + 64);
        if (::IsWindow(hParent)) {
            RECT rc = self->m_rect;
            POINT ptTL { rc.left, rc.top };
            POINT ptBR { rc.right, rc.bottom };
            ::ClientToScreen(hParent, &ptTL);
            ::ClientToScreen(hParent, &ptBR);
            data->m_rectAccLocation = CRect(ptTL.x, ptTL.y, ptBR.x, ptBR.y);
        }
    }

    return 1;
}
CMFCTasksPaneTask::CMFCTasksPaneTask(UINT nID, const wchar_t* lpszLabel, int nIcon, BOOL bIsSeparator) {
    memset(_taskspanetask_padding, 0, sizeof(_taskspanetask_padding));
    auto& state = EnsureTasksPaneTaskState(this);
    state.id = nID;
    state.label = lpszLabel ? lpszLabel : L"";
    state.icon = nIcon;
    state.isSeparator = bIsSeparator ? TRUE : FALSE;
}
CMFCTasksPaneTask::~CMFCTasksPaneTask() { RemoveTaskFromAllGroups(this); }
