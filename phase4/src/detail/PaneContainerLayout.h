#pragma once

#include "detail/CbarcoreSupport.h"

namespace openmfc::detail {

// CList<UINT,UINT> has the CObList field shape. The retail constructor writes
// exactly seven slots per list: the vftable at +0x00, zero at +0x08..+0x28 and
// 10 (the CList default block size) at +0x30.
struct S_CListUINT {
    void*   vfptr;         // 0x00
    void*   m_pNodeHead;   // 0x08
    void*   m_pNodeTail;   // 0x10
    INT_PTR m_nCount;      // 0x18
    void*   m_pNodeFree;   // 0x20
    void*   m_pBlocks;     // 0x28
    INT_PTR m_nBlockSize;  // 0x30
};
static_assert(offsetof(S_CListUINT, vfptr) == 0x00, "CList<UINT> vfptr");
static_assert(offsetof(S_CListUINT, m_pNodeHead) == 0x08, "CList<UINT> head");
static_assert(offsetof(S_CListUINT, m_pNodeTail) == 0x10, "CList<UINT> tail");
static_assert(offsetof(S_CListUINT, m_nCount) == 0x18, "CList<UINT> count");
static_assert(offsetof(S_CListUINT, m_pNodeFree) == 0x20, "CList<UINT> free node");
static_assert(offsetof(S_CListUINT, m_pBlocks) == 0x28, "CList<UINT> blocks");
static_assert(offsetof(S_CListUINT, m_nBlockSize) == 0x30, "CList<UINT> block size");
static_assert(sizeof(S_CListUINT) == 0x38, "CList<UINT> retail size");

struct S_CPaneContainer {
    void*  vfptr;                              // 0x00
    void*  m_pLeftPane;                        // 0x08 CDockablePane*
    void*  m_pRightPane;                       // 0x10 CDockablePane*
    void*  m_pSlider;                          // 0x18 CPaneDivider*
    S_CPaneContainer* m_pLeftPaneContainer;    // 0x20
    S_CPaneContainer* m_pRightPaneContainer;   // 0x28
    S_CPaneContainer* m_pParentContainer;      // 0x30
    void*  m_pContainerManager;                // 0x38 CPaneContainerManager*
    int    m_nPercent;                         // 0x40 ctor stores 50
    int    m_nField44;                         // 0x44 ctor stores -1; ?SetUpByID@
                                               //      (0xa7860) matches it against a
                                               //      persisted LEFT-pane id
    int    m_nField48;                         // 0x48 ctor stores -1; the same, for
                                               //      the RIGHT pane
    int    m_nField4C;                         // 0x4c ctor stores -1
    int    m_nRefCount;                        // 0x50 AddRef/Release
    int    m_bDividerHorz;                     // 0x54 cached CPaneDivider orientation
    int    m_nField58;                         // 0x58
    int    m_bReleased;                        // 0x5c FreeReleasedPaneContainer sets 1
    int    m_nField60;                         // 0x60
    RECT   m_rect64;                           // 0x64 ctor ::SetRectEmpty's it
    RECT   m_rect74;                           // 0x74 ctor ::SetRectEmpty's it
    unsigned char m_pad84[4];                  // 0x84 alignment padding
    S_CListUINT m_lstLeftSiblingIDs;           // 0x88
    S_CListUINT m_lstRightSiblingIDs;          // 0xc0
};

static_assert(offsetof(S_CPaneContainer, vfptr) == 0x00, "CPaneContainer vfptr");
static_assert(offsetof(S_CPaneContainer, m_pLeftPane) == 0x08, "CPaneContainer m_pLeftPane");
static_assert(offsetof(S_CPaneContainer, m_pRightPane) == 0x10, "CPaneContainer m_pRightPane");
static_assert(offsetof(S_CPaneContainer, m_pSlider) == 0x18, "CPaneContainer m_pSlider");
static_assert(offsetof(S_CPaneContainer, m_pLeftPaneContainer) == 0x20, "CPaneContainer m_pLeftPaneContainer");
static_assert(offsetof(S_CPaneContainer, m_pRightPaneContainer) == 0x28, "CPaneContainer m_pRightPaneContainer");
static_assert(offsetof(S_CPaneContainer, m_pParentContainer) == 0x30, "CPaneContainer m_pParentContainer");
static_assert(offsetof(S_CPaneContainer, m_pContainerManager) == 0x38, "CPaneContainer m_pContainerManager");
static_assert(offsetof(S_CPaneContainer, m_nPercent) == 0x40, "CPaneContainer m_nPercent");
static_assert(offsetof(S_CPaneContainer, m_nField44) == 0x44, "CPaneContainer field 0x44");
static_assert(offsetof(S_CPaneContainer, m_nField48) == 0x48, "CPaneContainer field 0x48");
static_assert(offsetof(S_CPaneContainer, m_nField4C) == 0x4c, "CPaneContainer field 0x4c");
static_assert(offsetof(S_CPaneContainer, m_nRefCount) == 0x50, "CPaneContainer m_nRefCount");
static_assert(offsetof(S_CPaneContainer, m_bDividerHorz) == 0x54, "CPaneContainer m_bDividerHorz");
static_assert(offsetof(S_CPaneContainer, m_nField58) == 0x58, "CPaneContainer field 0x58");
static_assert(offsetof(S_CPaneContainer, m_bReleased) == 0x5c, "CPaneContainer m_bReleased");
static_assert(offsetof(S_CPaneContainer, m_nField60) == 0x60, "CPaneContainer field 0x60");
static_assert(offsetof(S_CPaneContainer, m_rect64) == 0x64, "CPaneContainer m_rect64");
static_assert(offsetof(S_CPaneContainer, m_rect74) == 0x74, "CPaneContainer m_rect74");
static_assert(offsetof(S_CPaneContainer, m_pad84) == 0x84, "CPaneContainer list alignment pad");
static_assert(offsetof(S_CPaneContainer, m_lstLeftSiblingIDs) == 0x88, "CPaneContainer m_lstLeftSiblingIDs");
static_assert(offsetof(S_CPaneContainer, m_lstRightSiblingIDs) == 0xc0, "CPaneContainer m_lstRightSiblingIDs");
static_assert(sizeof(S_CPaneContainer) == 0xf8, "CPaneContainer size (operator new(0xf8) in ?Copy@ 0xa7ca0)");

// Bounds and offsets are in the dock site's client coordinates.
bool PaneSplitOffsets(const S_CPaneContainer& node, int& low, int& high);
bool MovePaneSplit(S_CPaneContainer& node, int offset, void** hdwp);
bool MovePaneDivider(CPaneDivider& divider, int offset, void** hdwp);
void ResizeDividerPanes(CPaneDivider* divider, const RECT& bounds);
bool DividerHasVisiblePanes(const CPaneDivider* divider);
void RefreshDividerPanes(CPaneDivider* divider);
void AttachDividerPane(CPaneDivider* divider, CDockablePane* pane);
bool ReserveDividerPane(CPaneDivider& divider, CDockablePane& pane, const RECT& bounds);
CPaneDivider* ReservedPaneDivider(CPane& pane);
void ForgetDividerPane(CPane& pane);
void TransferDividerPane(CPane& previous, CPane& replacement);
std::vector<CDockablePane*> ContainerPanes(const void* manager);
CWnd* PaneContainerDockSite(const void* manager);
bool ContainerHasPane(const void* manager, const CDockablePane* pane);
bool DetachContainerPane(void* manager, CDockablePane& pane);
S_CPaneContainer* CopyVisiblePaneContainer(S_CPaneContainer& source, S_CPaneContainer* parent);
void CleanUpPaneContainer(S_CPaneContainer& container);
void DestroyPaneContainerState(S_CPaneContainer& container);

} // namespace openmfc::detail
