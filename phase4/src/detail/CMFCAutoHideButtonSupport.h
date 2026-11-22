#pragma once

#include "openmfc/afxmfc.h"

class CMFCAutoHideButton : public CObject {
    DECLARE_DYNCREATE(CMFCAutoHideButton)
public:
    ~CMFCAutoHideButton() override;
    virtual BOOL Create(CMFCAutoHideBar* bar, CDockablePane* pane, DWORD alignment);
    void ShowAttachedWindow(BOOL show);
    void Move(int offset);

    virtual void ShowButton(BOOL show);
    virtual BOOL IsVisible() const { return m_bVisible; }
    virtual void HighlightButton(BOOL highlight);
    virtual BOOL IsHighlighted() const { return m_bHighlighted; }
    virtual void OnDraw(CDC* dc);
    virtual void OnFillBackground(CDC* dc, CRect rect);
    virtual void OnDrawBorder(CDC* dc, CRect bounds, CRect border);
    virtual void UnSetAutoHideMode(CDockablePane* firstInGroup);

    CSize GetSize() const;
    virtual CSize GetTextSize() const;
    BOOL IsHorizontal() const { return (m_dwAlignment & 0xa000) != 0; }
    void ReplacePane(CDockablePane* pane);

    BOOL m_bTop = FALSE;
    BOOL m_bVisible = FALSE;
    BOOL m_bHighlighted = FALSE;
    DWORD m_dwAlignment = 0;
    int m_nOffset = 0;
    CRect m_rect{};
    CMFCAutoHideBar* m_pParentBar = nullptr;
    CDockablePane* m_pAutoHideWindow = nullptr;
};
