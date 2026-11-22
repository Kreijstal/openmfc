#pragma once

#include "openmfc/afxole.h"

namespace openmfc::detail::dock {
struct DockContext {
    const void*  vfptr;                // 0x00
    POINT        m_ptLast;             // 0x08
    RECT         m_rectLast;           // 0x10
    SIZE         m_sizeLast;           // 0x20
    BOOL         m_bDitherLast;        // 0x28
    RECT         m_rectDragHorz;       // 0x2c
    RECT         m_rectDragVert;       // 0x3c
    RECT         m_rectFrameDragHorz;  // 0x4c
    RECT         m_rectFrameDragVert;  // 0x5c
    CControlBar* m_pBar;               // 0x70
    CFrameWnd*   m_pDockSite;          // 0x78
    DWORD        m_dwDockStyle;        // 0x80
    DWORD        m_dwOverDockStyle;    // 0x84
    DWORD        m_dwStyle;            // 0x88
    BOOL         m_bFlip;              // 0x8c
    BOOL         m_bForceFrame;        // 0x90
    CDC*         m_pDC;                // 0x98
    BOOL         m_bDragging;          // 0xa0
    int          m_nHitTest;           // 0xa4
    UINT         m_uMRUDockID;         // 0xa8
    RECT         m_rectMRUDockPos;     // 0xac
    DWORD        m_dwMRUFloatStyle;    // 0xbc
    POINT        m_ptMRUFloatPos;      // 0xc0
};
static_assert(offsetof(DockContext, m_uMRUDockID) == 0xa8);
static_assert(sizeof(DockContext) == 0xc8);
}
