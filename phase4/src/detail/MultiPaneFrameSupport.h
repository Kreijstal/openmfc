#pragma once

#include "openmfc/afxmfc.h"

class CMultiPaneFrameWnd : public CPaneFrameWnd {
public:
    CMultiPaneFrameWnd();
    ~CMultiPaneFrameWnd() override;

    static CRuntimeClass* AFXAPI GetThisClass();
    CRuntimeClass* GetRuntimeClass() const override;
    void AddPane(CBasePane* pane) override;
    void RemovePane(CBasePane* pane, BOOL destroy = FALSE) override;
    void AdjustLayout() override;
    CWnd* GetPane() const override;
    CWnd* GetFirstVisiblePane() const override;

    void* GetContainerManager() { return m_containerManager; }
    const void* GetContainerManager() const { return m_containerManager; }
    int OnCreate(CREATESTRUCTW* create);
    void OnClose();
    void OnSize(UINT type, int width, int height);
    LRESULT OnCaptionDoubleClick(WPARAM hit, LPARAM);

protected:
    const AFX_MSGMAP* GetMessageMap() const override;

private:
    alignas(void*) unsigned char m_containerManager[0xa0];

public:
    HWND m_hWndLastFocused = nullptr;
};

namespace openmfc::detail {
void AdoptTransferredPane(CDockablePane& pane, CWnd& host, CWnd* previous, CPaneDivider* divider);
CDockablePane* DockMultiPaneFrameAtPoint(CMultiPaneFrameWnd& frame, CPoint point, BOOL& docked);
}
