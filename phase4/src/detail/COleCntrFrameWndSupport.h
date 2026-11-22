#pragma once

#include "openmfc/afxole.h"

class COleCntrFrameWnd : public CFrameWnd {
public:
    explicit COleCntrFrameWnd(COleIPFrameWnd* inPlaceFrame);
    ~COleCntrFrameWnd() override;

    int OnCmdMsg(UINT id, int code, void* extra, AFX_CMDHANDLERINFO* handler) override;
    void RecalcLayout(BOOL notify) override;
    void PostNcDestroy() override;
    void OnIdleUpdateCmdUI();
    void DestroyControlBars();
    COleIPFrameWnd* m_pInPlaceFrame;
};

namespace openmfc::detail::colecntrframewnd {

// COleCntrFrameWndEx uses this layout to locate its extra members.
struct S_Colecntrframewnd {
    void* vfptr;
    char frame[sizeof(CFrameWnd) - sizeof(void*)];
    COleIPFrameWnd* m_pInPlaceFrame;
};
static_assert(sizeof(COleCntrFrameWnd) == 0x1e0);
static_assert(sizeof(S_Colecntrframewnd) == sizeof(COleCntrFrameWnd));
static_assert(offsetof(COleCntrFrameWnd, m_pInPlaceFrame) == 0x1d8);
static_assert(offsetof(S_Colecntrframewnd, m_pInPlaceFrame) == 0x1d8);

class InPlaceBarCreation {
public:
    InPlaceBarCreation(COleCntrFrameWnd* main, COleCntrFrameWnd* document);
    ~InPlaceBarCreation();
    InPlaceBarCreation(const InPlaceBarCreation&) = delete;
    InPlaceBarCreation& operator=(const InPlaceBarCreation&) = delete;
    static COleCntrFrameWnd* FindFrame(HWND window);

private:
    InPlaceBarCreation* previous_;
    COleCntrFrameWnd* main_;
    COleCntrFrameWnd* document_;
};
}
