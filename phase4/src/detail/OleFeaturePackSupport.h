#pragma once

#include "detail/COleCntrFrameWndSupport.h"
#include "openmfc/afxmfc.h"

class COleCntrFrameWndEx : public COleCntrFrameWnd {
public:
    explicit COleCntrFrameWndEx(COleIPFrameWnd* frame);
    ~COleCntrFrameWndEx() override;
    CRuntimeClass* GetRuntimeClass() const override;
    void RecalcLayout(BOOL notify) override;
    static CRuntimeClass* AFXAPI GetThisClass();

    CDockingManager m_dockManager;
    BYTE m_managerPadding[0x368 - sizeof(CDockingManager)]{};
};
static_assert(sizeof(COleCntrFrameWndEx) == 0x548);
static_assert(offsetof(COleCntrFrameWndEx, m_dockManager) == 0x1e0);

namespace openmfc::detail::olecore {

// Both extended in-place frame types use the same storage for their docking manager.
class FeaturePackInPlaceFrame : public COleIPFrameWnd {
public:
    explicit FeaturePackInPlaceFrame(CRuntimeClass* type) : m_dockManager(this), type_(type) {}
    CRuntimeClass* GetRuntimeClass() const override { return type_; }
    CBasePane* FindPane(UINT id);
    BOOL TogglePane(UINT id);
    void UpdatePaneMenu(CCmdUI* command);

    BYTE m_frameStorage[0x428 - sizeof(COleIPFrameWnd)]{};
    CDockingManager m_dockManager;
    BYTE m_managerPadding[0x368 - sizeof(CDockingManager) - sizeof(CRuntimeClass*)]{};
    CRuntimeClass* type_;

    DECLARE_MESSAGE_MAP()
};
static_assert(sizeof(FeaturePackInPlaceFrame) == 0x790);
static_assert(offsetof(FeaturePackInPlaceFrame, m_dockManager) == 0x428);

extern "C" CRuntimeClass* AFXAPI impl__GetThisClass_COleIPFrameWndEx__SAPEAUCRuntimeClass__XZ();
extern "C" CRuntimeClass* AFXAPI impl__GetThisClass_COleDocIPFrameWndEx__SAPEAUCRuntimeClass__XZ();
extern "C" int AFXAPI impl__ShowPanes_CDockingManager__UEAAHH_Z(CDockingManager*, int);
extern "C" int AFXAPI impl__InsertPane_CDockingManager__QEAAHPEAVCBasePane__0H_Z(
    CDockingManager*, CBasePane*, CBasePane*, int);
}
