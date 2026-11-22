#pragma once

#include "openmfc/afxmfc.h"

class CDockSite : public CBasePane {
    DECLARE_DYNCREATE(CDockSite)
public:
    CDockSite() = default;
    ~CDockSite() override;

    virtual BOOL Create(DWORD style, const RECT& rect, CWnd* parent, DWORD controlBarStyle,
                        CCreateContext* context);
    virtual BOOL CreateEx(DWORD extended, DWORD style, const RECT& rect, CWnd* parent,
                          DWORD controlBarStyle, CCreateContext* context);
    void SetDockingManager(CDockingManager* manager);

    CObList m_lstControlBars;
    CObList m_lstDockBarRows;
    UINT m_nDockBarID = 0;

protected:
    void PostNcDestroy() override;
};
static_assert(sizeof(CDockSite) == 0x220);
static_assert(offsetof(CDockSite, m_lstControlBars) == 0x1a8);
static_assert(offsetof(CDockSite, m_lstDockBarRows) == 0x1e0);
static_assert(offsetof(CDockSite, m_nDockBarID) == 0x218);

class CAutoHideDockSite : public CDockSite {
    DECLARE_DYNCREATE(CAutoHideDockSite)
public:
    BOOL CanAcceptPane(const CBasePane* pane) const override;

    int m_nOffsetLeft = 0;
    int m_nOffsetRight = 0;
};
static_assert(sizeof(CAutoHideDockSite) == 0x228);
static_assert(offsetof(CAutoHideDockSite, m_nOffsetLeft) == 0x220);

namespace openmfc::detail::mfccore {
RECT EmptyDockSiteBounds(RECT bounds, DWORD alignment);
void ForgetDockSite(CDockingManager* manager, CDockSite* site);
}
