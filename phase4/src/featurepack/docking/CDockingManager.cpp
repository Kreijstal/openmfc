// CDockingManager — OpenMFC implementation.
// Sources: mfccore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/MfccoreSupport.h"
#include "detail/CSmartDockingInfoSupport.h"

#include <cstdlib>
#include <cstring>

static_assert(sizeof(CDockingManager) == 0x90,
              "preserve the existing compact CDockingManager allocation");

extern "C" void* MS_ABI impl___0CSmartDockingInfo__QEAA_XZ(void* pThis);
extern "C" std::int32_t impl__m_bSDParamsModified_CDockingManager__1HA;
extern "C" std::uint32_t impl__m_dockModeGlobal_CDockingManager__1W4AFX_DOCK_TYPE__A;
extern "C" int impl__m_nDockSensitivity_CDockingManager__2HA;
extern "C" CObject* MS_ABI impl__CreateObject_CRuntimeClass__QEAAPEAVCObject__XZ(CRuntimeClass*);
extern "C" CObject* MS_ABI impl__CreateObject_CDockSite__SAPEAVCObject__XZ();
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CDockSite__SAPEAUCRuntimeClass__XZ();
extern "C" int MS_ABI impl__Create_CDockSite__UEAAHKAEBUtagRECT__PEAVCWnd__KPEAUCCreateContext___Z(
    void*, unsigned long, const RECT&, CWnd*, unsigned long, CCreateContext*);
extern "C" void MS_ABI impl___1CDockSite__UEAA_XZ(void*);
extern "C" void MS_ABI impl___3_YAXPEAX_Z(void*);
extern "C" unsigned long MS_ABI impl__GetCurrentAlignment_CBasePane__UEBAKXZ(const CBasePane*);
extern "C" int MS_ABI impl__GetDlgCtrlID_CWnd__QEBAHXZ(const CWnd*);
extern "C" void* MS_ABI impl__FindPaneByID_CDockSite__QEAAPEAVCPane__I_Z(void*, unsigned int);
extern "C" HWND MS_ABI impl__GetSafeHwnd_CWnd__QEBAPEAUHWND____XZ(const CWnd*);

namespace {
struct DockSiteInfoShadow {
    DWORD alignment;
    CRuntimeClass* dockSiteRuntimeClass;
};
static_assert(offsetof(DockSiteInfoShadow, alignment) == 0x00, "AFX_DOCKSITE_INFO alignment");
static_assert(offsetof(DockSiteInfoShadow, dockSiteRuntimeClass) == 0x08, "AFX_DOCKSITE_INFO runtime class");
static_assert(sizeof(DockSiteInfoShadow) == 0x10, "AFX_DOCKSITE_INFO size");

template <typename T>
bool InsertManagerStateEntry(T**& entries, std::size_t& count, std::size_t& capacity,
                             std::size_t index, T* value) {
    if (index > count) index = count;
    if (count == capacity) {
        const std::size_t newCapacity = capacity == 0 ? 4 : capacity * 2;
        T** grown = static_cast<T**>(std::realloc(entries, newCapacity * sizeof(T*)));
        if (!grown) return false;
        entries = grown;
        capacity = newCapacity;
    }
    if (index < count) {
        std::memmove(entries + index + 1, entries + index, (count - index) * sizeof(T*));
    }
    entries[index] = value;
    ++count;
    return true;
}

void* CreateDockSiteForManager(CDockingManager* manager, const DockSiteInfoShadow* info) {
    if (!manager || !info || !info->dockSiteRuntimeClass) return nullptr;
    CObject* object = impl__CreateObject_CRuntimeClass__QEAAPEAVCObject__XZ(
        info->dockSiteRuntimeClass);
    // Deviation: this calls the local CDockSite::Create thunk directly rather
    // than dispatching through the retail vtable, which OpenMFC does not model.
    // OpenMFC's CDockSite CRuntimeClass currently has no
    // m_pfnCreateObject; use its verified dedicated factory only for that exact
    // class. Other runtime classes still follow CRuntimeClass::CreateObject.
    if (!object && info->dockSiteRuntimeClass ==
                       impl__GetThisClass_CDockSite__SAPEAUCRuntimeClass__XZ()) {
        object = impl__CreateObject_CDockSite__SAPEAVCObject__XZ();
    }
    if (!object) return nullptr;

    RECT initialBounds{};
    const DockingManagerState* managerState = FindDockingState(manager);
    CFrameWnd* parent = managerState ? managerState->parentFrame : nullptr;
    if (!impl__Create_CDockSite__UEAAHKAEBUtagRECT__PEAVCWnd__KPEAUCCreateContext___Z(
            object, info->alignment, initialBounds, parent, 0, nullptr)) {
        impl___1CDockSite__UEAA_XZ(object);
        impl___3_YAXPEAX_Z(object);
        return nullptr;
    }
    // The local Create thunk only records the parent/rectangle. Retail Create
    // also leaves the alignment in CBasePane::m_dwStyle, read by GetCurrentAlignment.
    static_cast<CBasePane*>(object)->m_dwStyle = info->alignment;
    return object;
}
} // namespace

// Symbol: ?AddMiniFrame@CDockingManager@@UEAAHPEAVCPaneFrameWnd@@@Z
extern "C" int MS_ABI impl__AddMiniFrame_CDockingManager__UEAAHPEAVCPaneFrameWnd___Z(CDockingManager* pThis, void* pFrame) {
    if (!pThis || !pFrame) return FALSE;
    auto& frames = EnsureDockingState(pThis).miniFrames;
    if (std::find(frames.begin(), frames.end(), pFrame) == frames.end()) {
        frames.push_back(pFrame);
    }
    return TRUE;
}
// Symbol: ?AddPane@CDockingManager@@QEAAHPEAVCBasePane@@HHH@Z
extern "C" int MS_ABI impl__AddPane_CDockingManager__QEAAHPEAVCBasePane__HHH_Z(
    CDockingManager* pThis, CBasePane* pPane, int, int, int) {
    if (!pThis || !pPane) return FALSE;
    AddDockingPane(pThis, pPane);
    return TRUE;
}
// Symbol: ?AdjustDockingLayout@CDockingManager@@UEAAXPEAX@Z
extern "C" void MS_ABI impl__AdjustDockingLayout_CDockingManager__UEAAXPEAX_Z(CDockingManager* pThis, void*) {
    if (pThis) pThis->RecalcLayout();
}
// Symbol: ?AdjustPaneFrames@CDockingManager@@UEAAXXZ
extern "C" void MS_ABI impl__AdjustPaneFrames_CDockingManager__UEAAXXZ(CDockingManager* pThis) {
    const DockingManagerState* state = FindDockingState(pThis);
    if (!state || state->miniFrames.empty()) return;

    int index = 0;
    for (void* pFrame : state->miniFrames) {
        if (!pFrame) continue;
        CWnd* frame = static_cast<CWnd*>(pFrame);
        HWND hWnd = frame ? frame->GetSafeHwnd() : nullptr;
        if (!hWnd) continue;

        RECT currentRect{};
        ::GetWindowRect(hWnd, &currentRect);
        CRect rect(currentRect);
        if (!IsDrawableRect(rect)) {
            rect = DefaultMiniFrameRect(index++);
        }

        frame->SetWindowPos(nullptr,
                            rect.left, rect.top,
                            std::max(rect.Width(), 80),
                            std::max(rect.Height(), 60),
                            SWP_NOZORDER | SWP_NOACTIVATE);
    }
}
// Symbol: ?AdjustRectToClientArea@CDockingManager@@UEAAHAEAVCRect@@K@Z
extern "C" int MS_ABI impl__AdjustRectToClientArea_CDockingManager__UEAAHAEAVCRect__K_Z(CDockingManager* pThis, CRect* pRect, unsigned long) {
    if (!pThis || !pRect) return FALSE;

    const DockingManagerState* state = FindDockingState(pThis);
    if (!state || state->panes.empty()) return FALSE;

    CWnd* parentWnd = nullptr;
    for (CBasePane* pane : state->panes) {
        if (!pane || !pane->GetSafeHwnd()) continue;
        if (CWnd* parent = pane->GetParent()) {
            if (parent->GetSafeHwnd()) {
                parentWnd = parent;
                break;
            }
        }
    }

    if (!parentWnd) return FALSE;

    CRect original = *pRect;
    POINT topLeft = original.TopLeft();
    POINT bottomRight = original.BottomRight();
    ::ScreenToClient(parentWnd->GetSafeHwnd(), &topLeft);
    ::ScreenToClient(parentWnd->GetSafeHwnd(), &bottomRight);
    original = CRect(topLeft, bottomRight);
    RECT client{};
    parentWnd->GetClientRect(&client);
    CRect clientRect(client);
    original.IntersectRect(original, clientRect);
    if (original.IsRectEmpty()) return FALSE;
    *pRect = original;
    return TRUE;
}
// Symbol: ?AlignAutoHidePane@CDockingManager@@QEAAXPEAVCPaneDivider@@H@Z
extern "C" void MS_ABI impl__AlignAutoHidePane_CDockingManager__QEAAXPEAVCPaneDivider__H_Z(
    CDockingManager* pThis, void* pDivider, int nAlignment) {
    if (!pThis || !pDivider) return;
    std::lock_guard<std::mutex> lock(g_paneCoreStateMutex_Mfccore);
    DockingManagerState& state = EnsureDockingState(pThis);
    for (CBasePane* pane : state.panes) {
        if (!pane) continue;
        auto& paneState = g_paneCoreState_Mfccore[pane];
        paneState.autoHideMode = TRUE;
        paneState.autoHideAlignment = static_cast<DWORD>(nAlignment);
        paneState.autoHideBar = pDivider;
    }
    (void)state;
}
// Symbol: ?AutoHidePane@CDockingManager@@QEAAPEAVCMFCAutoHideBar@@PEAVCDockablePane@@PEAV2@@Z
extern "C" CMFCAutoHideBar* MS_ABI impl__AutoHidePane_CDockingManager__QEAAPEAVCMFCAutoHideBar__PEAVCDockablePane__PEAV2__Z(
    CDockingManager* pThis, CDockablePane* pPane, CDockablePane*) {
    if (!pThis || !pPane) return nullptr;
    AddDockingPane(pThis, pPane);
    pThis->HidePane(pPane);

    DockingManagerState& state = EnsureDockingState(pThis);
    state.floatingPanes.erase(pPane);
    state.hiddenPanes.insert(pPane);

    std::lock_guard<std::mutex> lock(g_paneCoreStateMutex_Mfccore);
    PaneCoreState& paneState = g_paneCoreState_Mfccore[pPane];
    if (!paneState.autoHideAlignment) {
        paneState.autoHideAlignment = state.enabledAlignment ? state.enabledAlignment : 0;
    }

    CMFCAutoHideBar* bar = static_cast<CMFCAutoHideBar*>(paneState.autoHideBar);
    if (!bar) {
        bar = new (std::nothrow) CMFCAutoHideBar();
        if (!bar) return nullptr;
        paneState.autoHideBar = bar;
    }
    pPane->SetAutoHideMode(TRUE, paneState.autoHideAlignment, bar, FALSE);
    return bar;
}
// Symbol: ?BringBarsToTop@CDockingManager@@QEAAXKH@Z
extern "C" void MS_ABI impl__BringBarsToTop_CDockingManager__QEAAXKH_Z(
    CDockingManager* pThis, unsigned long nFlags, int bActivate) {
    const DockingManagerState* state = FindDockingState(pThis);
    if (!state) return;

    UINT flags = SWP_NOMOVE | SWP_NOSIZE | (bActivate ? 0 : SWP_NOACTIVATE);
    for (CBasePane* pane : state->panes) {
        if (!pane) continue;
        HWND hWnd = pane->GetSafeHwnd();
        if (!hWnd) continue;
        ::SetWindowPos(hWnd, HWND_TOP, 0, 0, 0, 0, flags);
        pane->InvalidateRect(nullptr, FALSE);
    }

    for (void* pFrame : state->miniFrames) {
        if (!pFrame) continue;
        CWnd* frame = static_cast<CWnd*>(pFrame);
        HWND hWnd = frame ? frame->GetSafeHwnd() : nullptr;
        if (!hWnd) continue;
        ::SetWindowPos(hWnd, HWND_TOP, 0, 0, 0, 0, flags);
        ::InvalidateRect(hWnd, nullptr, FALSE);
    }
    (void)nFlags;
}
// Symbol: ?BuildPanesMenu@CDockingManager@@QEAAXAEAVCMenu@@H@Z
extern "C" void MS_ABI impl__BuildPanesMenu_CDockingManager__QEAAXAEAVCMenu__H_Z(
    CDockingManager* pThis, CMenu* pMenu, int nIDBase) {
    if (!pThis || !pMenu) return;
    const DockingManagerState* state = FindDockingState(pThis);
    if (!state) return;

    HMENU hMenu = pMenu->GetSafeHmenu();
    if (!hMenu) {
        if (!pMenu->CreatePopupMenu()) return;
        hMenu = pMenu->GetSafeHmenu();
    }
    if (!hMenu) return;

    while (::GetMenuItemCount(hMenu) > 0) {
        ::RemoveMenu(hMenu, 0, MF_BYPOSITION);
    }

    int idx = 0;
    const UINT baseId = static_cast<UINT>(nIDBase == 0 ? 5000 : nIDBase);
    for (CBasePane* pane : state->panes) {
        if (!pane) continue;
        UINT cmd = PaneCommandID(pane);
        if (cmd == 0) cmd = baseId + idx;
        ++idx;

        CString label;
        const int len = pane->GetWindowTextLengthW();
        if (len > 0) {
            std::wstring tmp(static_cast<size_t>(len + 1), L'\0');
            pane->GetWindowTextW(&tmp[0], len + 1);
            label = tmp.c_str();
        } else {
            label = L"Pane";
        }
        pMenu->AppendMenu(MF_STRING | MF_ENABLED, cmd, (const wchar_t*)label);
    }

    if (idx == 0) {
        pMenu->AppendMenu(MF_STRING | MF_ENABLED | MF_GRAYED, baseId, L"(No panes)");
    }
}
// Symbol: ?CalcExpectedDockedRect@CDockingManager@@QEAAXPEAVCWnd@@VCPoint@@AEAVCRect@@AEAHPEAPEAVCDockablePane@@@Z
extern "C" void MS_ABI impl__CalcExpectedDockedRect_CDockingManager__QEAAXPEAVCWnd__VCPoint__AEAVCRect__AEAHPEAPEAVCDockablePane___Z(
    CDockingManager* pThis, CWnd* pWnd, CPoint ptMouse, CRect* rectResult, int* pnAlignment, CDockablePane** ppTargetBar) {
    if (!rectResult) return;

    CRect bestRect;
    int alignment = 0;
    CDockablePane* targetBar = nullptr;

    const DockingManagerState* state = FindDockingState(pThis);
    if (state && !state->panes.empty()) {
        CBasePane* target = impl__PaneFromPoint_CDockingManager__UEBAPEAVCBasePane__VCPoint__H_NPEAUCRuntimeClass__HPEBV2__Z(
            pThis, ptMouse, FALSE, FALSE, nullptr, FALSE, nullptr);
        if (!target) target = FirstDockingPane(pThis, true);
        if (target) {
            RECT rect{};
            target->GetWindowRect(&rect);
            targetBar = static_cast<CDockablePane*>(target);

            int left = rect.left;
            int right = rect.right;
            int top = rect.top;
            int bottom = rect.bottom;
            const int cx = right - left;
            const int cy = bottom - top;
            const int midX = left + cx / 2;
            const int midY = top + cy / 2;

            if (cx > cy) {
                if (ptMouse.x < midX) {
                    alignment = 2;
                    right = midX;
                } else {
                    alignment = 3;
                    left = midX;
                }
            } else {
                if (ptMouse.y < midY) {
                    alignment = 4;
                    bottom = midY;
                } else {
                    alignment = 5;
                    top = midY;
                }
            }

            bestRect.SetRect(left, top, right, bottom);
        }
    }

    if (bestRect.IsRectEmpty() && pWnd && pWnd->GetSafeHwnd()) {
        RECT r{};
        pWnd->GetWindowRect(&r);
        bestRect.SetRect(r.left, r.top, r.right, r.bottom);
        alignment = 0;
    } else if (bestRect.IsRectEmpty()) {
        bestRect = CRect(ptMouse.x, ptMouse.y, ptMouse.x, ptMouse.y);
    }

    if (pnAlignment) *pnAlignment = alignment;
    if (ppTargetBar) *ppTargetBar = targetBar;
    *rectResult = bestRect;
}
// Symbol: ?DeterminePaneAndStatus@CDockingManager@@UEAA?AW4AFX_CS_STATUS@@VCPoint@@HKPEAPEAVCBasePane@@PEBV4@2@Z
extern "C" int MS_ABI impl__DeterminePaneAndStatus_CDockingManager__UEAA_AW4AFX_CS_STATUS__VCPoint__HKPEAPEAVCBasePane__PEBV4_2_Z(
    CDockingManager* pThis, CPoint point, int nAlignment, unsigned long, CBasePane** ppTargetBar, const CDockingManager*, const CDockingManager*) {
    if (!pThis || !ppTargetBar) return 0;
    const DockingManagerState* state = FindDockingState(pThis);
    if (!state || state->panes.empty()) return 0;

    CPoint pt = point;
    CBasePane* bestPane = nullptr;
    for (CBasePane* pane : state->panes) {
        if (!pane || !pane->GetSafeHwnd()) continue;
        RECT rect{};
        pane->GetWindowRect(&rect);
        if (::PtInRect(&rect, pt)) {
            bestPane = pane;
            break;
        }
    }

    if (!bestPane) {
        bestPane = FirstDockingPane(pThis, true);
        if (!bestPane) return 0;
    }

    *ppTargetBar = bestPane;
    RECT rect{};
    bestPane->GetWindowRect(&rect);
    int status = 1;
    if (nAlignment & 1) {
        status = pt.x < (rect.left + rect.right) / 2 ? 2 : 3;
    } else if (nAlignment & 2) {
        status = pt.y < (rect.top + rect.bottom) / 2 ? 4 : 5;
    } else if (!state->panes.empty()) {
        status = 1;
    }
    return status;
}
// Symbol: ?EnableAutoHidePanes@CDockingManager@@QEAAHK@Z
extern "C" int MS_ABI impl__EnableAutoHidePanes_CDockingManager__QEAAHK_Z(CDockingManager* pThis, unsigned long dwStyle) {
    if (pThis) EnsureDockingState(pThis).enabledAlignment |= dwStyle;
    return TRUE;
}
// Symbol: ?EnablePaneContextMenu@CDockingManager@@QEAAXHIAEBV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@H@Z
extern "C" void MS_ABI impl__EnablePaneContextMenu_CDockingManager__QEAAXHIAEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__H_Z(
    CDockingManager* pThis, int bEnable, unsigned int uiCustomizeCmd, const CString* strCustomizeText, int) {
    if (!pThis) return;
    DockingManagerState& state = EnsureDockingState(pThis);
    state.paneContextMenu = bEnable != FALSE;
    state.paneContextMenuID = uiCustomizeCmd;
    state.paneContextMenuName = strCustomizeText ? *strCustomizeText : CString();
}
// Symbol: ?FindPaneByID@CDockingManager@@UEAAPEAVCBasePane@@IH@Z
extern "C" CBasePane* MS_ABI impl__FindPaneByID_CDockingManager__UEAAPEAVCBasePane__IH_Z(
    CDockingManager* pThis, unsigned int nID, int bSearchMiniFrames) {
    const DockingManagerState* state = FindDockingState(pThis);
    if (!state) return nullptr;

    for (CBasePane* pane : state->panes) {
        if (!bSearchMiniFrames && state->hiddenPanes.find(pane) != state->hiddenPanes.end()) {
            continue;
        }
        if (PaneCommandID(pane) == nID) return pane;
    }

    if (!bSearchMiniFrames) return nullptr;
    return nullptr;
}
// Symbol: ?FixupVirtualRects@CDockingManager@@UEAAXXZ
extern "C" void MS_ABI impl__FixupVirtualRects_CDockingManager__UEAAXXZ(CDockingManager* pThis) {
    if (!pThis) return;
    const DockingManagerState* state = FindDockingState(pThis);
    if (!state) return;

    int idx = 0;
    for (CBasePane* pane : state->panes) {
        if (!pane || !pane->GetSafeHwnd()) continue;
        CRect rect{};
        {
            std::lock_guard<std::mutex> lock(g_paneCoreStateMutex_Mfccore);
            PaneCoreState& paneState = g_paneCoreState_Mfccore[pane];
            rect = paneState.recentRect;
            paneState.visible = TRUE;
        }

        if (!IsDrawableRect(rect)) rect = DefaultMiniFrameRect(idx++);
        if (rect.Width() <= 0 || rect.Height() <= 0) {
            rect.right = rect.left + 240;
            rect.bottom = rect.top + 180;
        }

        HWND hWnd = pane->GetSafeHwnd();
        if (hWnd) {
            pane->SetWindowPos(nullptr, rect.left, rect.top, rect.Width(), rect.Height(), SWP_NOZORDER | SWP_NOACTIVATE, nullptr);
        }
    }
}
// Symbol: ?FloatPane@CDockingManager@@QEAAXPEAVCBasePane@@VCPoint@@K@Z
extern "C" void MS_ABI impl__FloatPane_CDockingManager__QEAAXPEAVCBasePane__VCPoint__K_Z(
    CDockingManager* pThis, CBasePane* pPane, CPoint ptOffset, unsigned long dwAlignment) {
    if (pThis) pThis->FloatPane(pPane, ptOffset, dwAlignment);
}
// Symbol: ?FrameFromPoint@CDockingManager@@UEBAPEAVCPaneFrameWnd@@VCPoint@@PEAV2@H@Z
extern "C" void* MS_ABI impl__FrameFromPoint_CDockingManager__UEBAPEAVCPaneFrameWnd__VCPoint__PEAV2_H_Z(
    const CDockingManager* pThis, CPoint point, void*, int) {
    const DockingManagerState* state = FindDockingState(pThis);
    if (!pThis || !state || state->miniFrames.empty()) return nullptr;

    void* bestFrame = nullptr;
    int bestRank = INT_MAX;
    CPoint pt = point;
    for (void* frame : state->miniFrames) {
        if (!frame) continue;
        CWnd* wnd = static_cast<CWnd*>(frame);
        HWND hWnd = wnd ? wnd->GetSafeHwnd() : nullptr;
        if (!hWnd) continue;
        RECT rect{};
        ::GetWindowRect(hWnd, &rect);
        if (!::PtInRect(&rect, pt)) continue;
        int rank = MiniFrameZRank(hWnd);
        if (rank < bestRank) {
            bestRank = rank;
            bestFrame = frame;
        }
    }

    return bestFrame ? bestFrame : state->miniFrames.front();
}
// Symbol: ?GetPaneList@CDockingManager@@QEAAXAEAVCObList@@HPEAUCRuntimeClass@@H@Z
extern "C" void MS_ABI impl__GetPaneList_CDockingManager__QEAAXAEAVCObList__HPEAUCRuntimeClass__H_Z(
    CDockingManager* pThis, CObList* pList, int bIncludeHidden, CRuntimeClass* pRTCFilter, int) {
    if (!pList) return;
    const DockingManagerState* state = FindDockingState(pThis);
    if (!state) return;

    for (CBasePane* pane : state->panes) {
        if (!pane) continue;
        if (!bIncludeHidden && state->hiddenPanes.find(pane) != state->hiddenPanes.end()) continue;
        if (pRTCFilter && !pane->IsKindOf(pRTCFilter)) continue;
        pList->AddTail(static_cast<CObject*>(pane));
    }
}
// Symbol: ?HideAutoHidePanes@CDockingManager@@QEAAXPEAVCDockablePane@@H@Z
extern "C" void MS_ABI impl__HideAutoHidePanes_CDockingManager__QEAAXPEAVCDockablePane__H_Z(
    CDockingManager* pThis, CDockablePane* pPane, int) {
    if (pThis && pPane) pThis->HidePane(pPane);
}
// Symbol: ?InsertPane@CDockingManager@@QEAAHPEAVCBasePane@@0H@Z
extern "C" int MS_ABI impl__InsertPane_CDockingManager__QEAAHPEAVCBasePane__0H_Z(
    CDockingManager* pThis, CBasePane* pPane, CBasePane* pTarget, int) {
    if (!pThis || !pPane) return FALSE;
    AddDockingPane(pThis, pPane, pTarget);
    return TRUE;
}
// Symbol: ?LoadState@CDockingManager@@UEAAHPEB_WI@Z
extern "C" int MS_ABI impl__LoadState_CDockingManager__UEAAHPEB_WI_Z(CDockingManager* pThis, const wchar_t* lpszProfileName, unsigned int) {
    if (!pThis) return FALSE;
    DockingManagerState& state = EnsureDockingState(pThis);
    std::lock_guard<std::mutex> lock(g_dockingProfileMutex);
    DockingManagerSnapshot* snapshot = FindDockingProfileSnapshot(pThis, lpszProfileName);
    if (!snapshot) return TRUE;

    const std::size_t kCap = snapshot->ids.size();
    std::vector<CBasePane*> ordered;
    std::unordered_set<CBasePane*> restored;
    ordered.reserve(state.panes.size());

    for (std::size_t i = 0; i < kCap; ++i) {
        if (i >= snapshot->rects.size() || i >= snapshot->hidden.size() || i >= snapshot->floating.size()) break;
        CBasePane* pane = impl__FindPaneByID_CDockingManager__UEAAPEAVCBasePane__IH_Z(
            pThis, snapshot->ids[i], TRUE);
        if (!pane) continue;
        if (restored.find(pane) != restored.end()) continue;

        if (snapshot->hidden[i]) state.hiddenPanes.insert(pane); else state.hiddenPanes.erase(pane);
        if (snapshot->floating[i]) state.floatingPanes.insert(pane); else state.floatingPanes.erase(pane);

        std::lock_guard<std::mutex> paneLock(g_paneCoreStateMutex_Mfccore);
        g_paneCoreState_Mfccore[pane].recentRect = snapshot->rects[i];
        g_paneCoreState_Mfccore[pane].visible = !snapshot->hidden[i];

        ordered.push_back(pane);
        restored.insert(pane);
    }

    for (CBasePane* pane : state.panes) {
        if (pane && restored.find(pane) == restored.end()) ordered.push_back(pane);
    }
    if (!ordered.empty()) state.panes = ordered;

    pThis->RecalcLayout();
    return TRUE;
}
// Symbol: ?LockUpdate@CDockingManager@@QEAAXH@Z
extern "C" void MS_ABI impl__LockUpdate_CDockingManager__QEAAXH_Z(CDockingManager* pThis, int bLock) {
    if (pThis) EnsureDockingState(pThis).lockUpdate = bLock != FALSE;
}
// Symbol: ?OnActivateFrame@CDockingManager@@UEAAXH@Z
extern "C" void MS_ABI impl__OnActivateFrame_CDockingManager__UEAAXH_Z(CDockingManager* pThis, int bActivate) {
    if (!pThis || !bActivate) return;
    pThis->RecalcLayout();
    impl__RedrawAllMiniFrames_CDockingManager__QEAAXXZ(pThis);
}
// Symbol: ?OnClosePopupMenu@CDockingManager@@QEAAXXZ
extern "C" void MS_ABI impl__OnClosePopupMenu_CDockingManager__QEAAXXZ(CDockingManager*) {
    if (CMFCPopupMenu* popup = CMFCPopupMenu::GetSafeActivePopupMenu()) {
        popup->CloseMenu(TRUE);
    }
}
// Symbol: ?OnMoveMiniFrame@CDockingManager@@UEAAHPEAVCWnd@@@Z
extern "C" int MS_ABI impl__OnMoveMiniFrame_CDockingManager__UEAAHPEAVCWnd___Z(CDockingManager* pThis, CWnd* pFrame) {
    if (!pThis || !pFrame) return FALSE;

    auto& frames = EnsureDockingState(pThis).miniFrames;
    auto it = std::find(frames.begin(), frames.end(), pFrame);
    if (it == frames.end()) {
        frames.push_back(pFrame);
    }

    HWND hWnd = pFrame->GetSafeHwnd();
    if (!hWnd) return TRUE;

    RECT rect{};
    ::GetWindowRect(hWnd, &rect);
    if (!IsDrawableRect(CRect(rect))) return FALSE;

    pFrame->InvalidateRect(nullptr, FALSE);
    return TRUE;
}
// Symbol: ?OnPaneContextMenu@CDockingManager@@QEAAXVCPoint@@@Z
extern "C" void MS_ABI impl__OnPaneContextMenu_CDockingManager__QEAAXVCPoint___Z(CDockingManager* pThis, CPoint pt) {
    if (!pThis) return;
    const DockingManagerState* state = FindDockingState(pThis);
    if (!state || state->panes.empty()) return;

    CMenu menu;
    if (!menu.CreatePopupMenu()) return;
    impl__BuildPanesMenu_CDockingManager__QEAAXAEAVCMenu__H_Z(pThis, &menu, 5000);

    HMENU hMenu = menu.GetSafeHmenu();
    if (!hMenu) return;
    if (::GetMenuItemCount(hMenu) <= 0) return;
    ::TrackPopupMenu(hMenu, TPM_LEFTBUTTON | TPM_TOPALIGN | TPM_LEFTALIGN,
                     pt.x, pt.y, 0, nullptr, nullptr);
}
// Symbol: ?PaneFromPoint@CDockingManager@@UEBAPEAVCBasePane@@VCPoint@@H_NPEAUCRuntimeClass@@HPEBV2@@Z
extern "C" CBasePane* MS_ABI impl__PaneFromPoint_CDockingManager__UEBAPEAVCBasePane__VCPoint__H_NPEAUCRuntimeClass__HPEBV2__Z(
    const CDockingManager* pThis, CPoint point, int bIncludeHidden, int, CRuntimeClass* pRTCFilter, int, const CDockingManager*) {
    const DockingManagerState* state = FindDockingState(pThis);
    if (!state) return nullptr;

    CBasePane* bestPane = nullptr;
    int bestArea = INT_MAX;
    for (CBasePane* pane : state->panes) {
        if (!pane || !pane->GetSafeHwnd()) continue;
        if (!bIncludeHidden && state->hiddenPanes.find(pane) != state->hiddenPanes.end()) continue;
        if (pRTCFilter && !pane->IsKindOf(pRTCFilter)) continue;
        RECT r;
        pane->GetWindowRect(&r);
        if (!::PtInRect(&r, point)) continue;
        CRect rect(r);
        const int area = rect.Width() * rect.Height();
        if (!bestPane || area < bestArea) {
            bestPane = pane;
            bestArea = area;
        }
    }

    return bestPane ? bestPane : FirstDockingPane(pThis, !bIncludeHidden);
}
// Symbol: ?PaneFromPoint@CDockingManager@@UEBAPEAVCBasePane@@VCPoint@@HAEAKPEAUCRuntimeClass@@PEBV2@@Z
extern "C" CBasePane* MS_ABI impl__PaneFromPoint_CDockingManager__UEBAPEAVCBasePane__VCPoint__HAEAKPEAUCRuntimeClass__PEBV2__Z(
    const CDockingManager* pThis, CPoint point, int bIncludeHidden, unsigned long* dwAlignment, CRuntimeClass* pRTCFilter, const CDockingManager* pDockManager) {
    if (!pThis) return nullptr;
    if (dwAlignment) *dwAlignment = 0;
    CBasePane* target = nullptr;
    if (pDockManager) {
        target = impl__PaneFromPoint_CDockingManager__UEBAPEAVCBasePane__VCPoint__H_NPEAUCRuntimeClass__HPEBV2__Z(
            pDockManager, point, bIncludeHidden, FALSE, pRTCFilter, FALSE, pThis);
    }

    if (!target) {
        target = impl__PaneFromPoint_CDockingManager__UEBAPEAVCBasePane__VCPoint__H_NPEAUCRuntimeClass__HPEBV2__Z(
            pThis, point, bIncludeHidden, FALSE, pRTCFilter, FALSE, pDockManager);
    }

    if (!target) return nullptr;
    if (!dwAlignment) return target;

    CBasePane* targetFromManager = nullptr;
    const int status = impl__DeterminePaneAndStatus_CDockingManager__UEAA_AW4AFX_CS_STATUS__VCPoint__HKPEAPEAVCBasePane__PEBV4_2_Z(
        const_cast<CDockingManager*>(pThis), point, 0, 0, &targetFromManager, pThis, pDockManager);
    *dwAlignment = status;
    if (targetFromManager && targetFromManager != target) {
        return targetFromManager;
    }
    return target;
}
// Symbol: ?ProcessPaneContextMenuCommand@CDockingManager@@QEAAHIHPEAXPEAUAFX_CMDHANDLERINFO@@@Z
extern "C" int MS_ABI impl__ProcessPaneContextMenuCommand_CDockingManager__QEAAHIHPEAXPEAUAFX_CMDHANDLERINFO___Z(
    CDockingManager* pThis, unsigned int nID, int, void*, void*) {
    if (!pThis || nID == 0) return FALSE;
    const DockingManagerState* state = FindDockingState(pThis);
    if (!state) return FALSE;

    CBasePane* target = impl__FindPaneByID_CDockingManager__UEAAPEAVCBasePane__IH_Z(pThis, nID, TRUE);
    if (!target) return FALSE;

    if (state->hiddenPanes.find(target) == state->hiddenPanes.end()) {
        pThis->HidePane(target);
    } else {
        pThis->ShowPane(target, FALSE);
    }

    return TRUE;
}
// Symbol: ?RecalcLayout@CDockingManager@@UEAAXH@Z
extern "C" void MS_ABI impl__RecalcLayout_CDockingManager__UEAAXH_Z(CDockingManager* pThis, int) {
    if (pThis) pThis->RecalcLayout();
}
// Symbol: ?RedrawAllMiniFrames@CDockingManager@@QEAAXXZ
extern "C" void MS_ABI impl__RedrawAllMiniFrames_CDockingManager__QEAAXXZ(CDockingManager* pThis) {
    if (!pThis) return;
    const DockingManagerState* state = FindDockingState(pThis);
    if (!state) return;

    for (void* pFrame : state->miniFrames) {
        if (!pFrame) continue;
        CWnd* frame = static_cast<CWnd*>(pFrame);
        HWND hWnd = frame ? frame->GetSafeHwnd() : nullptr;
        if (!hWnd) continue;
        ::InvalidateRect(hWnd, nullptr, TRUE);
        ::UpdateWindow(hWnd);
    }
}
// Symbol: ?ReleaseEmptyPaneContainers@CDockingManager@@QEAAXXZ
extern "C" void MS_ABI impl__ReleaseEmptyPaneContainers_CDockingManager__QEAAXXZ(CDockingManager* pThis) {
    if (!pThis) return;
    DockingManagerState& state = EnsureDockingState(pThis);
    std::vector<CBasePane*> oldPanes = state.panes;
    state.panes.erase(std::remove_if(state.panes.begin(), state.panes.end(),
                                    [](CBasePane* pane) { return pane == nullptr || !pane->GetSafeHwnd(); }),
                     state.panes.end());

    const auto isAlive = [&state](CBasePane* pane) {
        return std::find(state.panes.begin(), state.panes.end(), pane) != state.panes.end();
    };
    for (CBasePane* pane : oldPanes) {
        if (!pane || isAlive(pane)) continue;
        state.hiddenPanes.erase(pane);
        state.floatingPanes.erase(pane);
        g_paneCoreState_Mfccore.erase(pane);
    }

    state.miniFrames.erase(std::remove_if(state.miniFrames.begin(), state.miniFrames.end(),
                                          [](void* frame) {
                                              if (!frame) return true;
                                              CWnd* wnd = static_cast<CWnd*>(frame);
                                              return wnd->GetSafeHwnd() == nullptr;
                                          }),
                           state.miniFrames.end());
}
// Symbol: ?RemoveHiddenMDITabbedBar@CDockingManager@@QEAAXPEAVCDockablePane@@@Z
extern "C" void MS_ABI impl__RemoveHiddenMDITabbedBar_CDockingManager__QEAAXPEAVCDockablePane___Z(CDockingManager* pThis, CDockablePane* pPane) {
    RemoveDockingPane(pThis, pPane);
}
// Symbol: ?RemoveMiniFrame@CDockingManager@@UEAAHPEAVCPaneFrameWnd@@@Z
extern "C" int MS_ABI impl__RemoveMiniFrame_CDockingManager__UEAAHPEAVCPaneFrameWnd___Z(CDockingManager* pThis, void* pFrame) {
    if (!pThis || !pFrame) return FALSE;
    auto& frames = EnsureDockingState(pThis).miniFrames;
    auto oldSize = frames.size();
    frames.erase(std::remove(frames.begin(), frames.end(), pFrame), frames.end());
    return frames.size() != oldSize;
}
// Symbol: ?RemovePaneFromDockManager@CDockingManager@@QEAAXPEAVCBasePane@@HHH0@Z
extern "C" void MS_ABI impl__RemovePaneFromDockManager_CDockingManager__QEAAXPEAVCBasePane__HHH0_Z(
    CDockingManager* pThis, CBasePane* pPane, int, int, int, CBasePane*) {
    RemoveDockingPane(pThis, pPane);
}
// Symbol: ?ReplacePane@CDockingManager@@QEAAHPEAVCDockablePane@@0@Z
extern "C" int MS_ABI impl__ReplacePane_CDockingManager__QEAAHPEAVCDockablePane__0_Z(
    CDockingManager* pThis, CDockablePane* pOldPane, CDockablePane* pNewPane) {
    if (!pThis || !pOldPane || !pNewPane) return FALSE;
    DockingManagerState& state = EnsureDockingState(pThis);
    auto it = std::find(state.panes.begin(), state.panes.end(), static_cast<CBasePane*>(pOldPane));
    if (it == state.panes.end()) return FALSE;
    *it = pNewPane;
    state.hiddenPanes.erase(pOldPane);
    state.floatingPanes.erase(pOldPane);
    return TRUE;
}
// Symbol: ?ResortMiniFramesForZOrder@CDockingManager@@QEAAXXZ
extern "C" void MS_ABI impl__ResortMiniFramesForZOrder_CDockingManager__QEAAXXZ(CDockingManager* pThis) {
    if (!pThis) return;
    DockingManagerState& state = EnsureDockingState(pThis);
    if (state.miniFrames.size() < 2) return;

    std::sort(state.miniFrames.begin(), state.miniFrames.end(),
              [](void* lhs, void* rhs) {
                  HWND left = lhs ? static_cast<CWnd*>(lhs)->GetSafeHwnd() : nullptr;
                  HWND right = rhs ? static_cast<CWnd*>(rhs)->GetSafeHwnd() : nullptr;
                  return MiniFrameZRank(left) < MiniFrameZRank(right);
              });

    for (void* frame : state.miniFrames) {
        if (!frame) continue;
        CWnd* wnd = static_cast<CWnd*>(frame);
        HWND hWnd = wnd ? wnd->GetSafeHwnd() : nullptr;
        if (!hWnd) continue;
        ::SetWindowPos(hWnd, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
    }
}
// Symbol: ?SaveState@CDockingManager@@UEAAHPEB_WI@Z
extern "C" int MS_ABI impl__SaveState_CDockingManager__UEAAHPEB_WI_Z(CDockingManager* pThis, const wchar_t* lpszProfileName, unsigned int) {
    if (!pThis) return FALSE;
    const DockingManagerState& state = EnsureDockingState(pThis);
    DockingManagerSnapshot snapshot;

    std::unordered_set<UINT> usedIds;
    for (CBasePane* pane : state.panes) {
        if (!pane) continue;
        UINT id = PaneCommandID(pane);
        if (id == 0 || usedIds.find(id) != usedIds.end()) continue;
        std::lock_guard<std::mutex> paneLock(g_paneCoreStateMutex_Mfccore);
        const auto paneStateIt = g_paneCoreState_Mfccore.find(pane);

        CRect rect = paneStateIt == g_paneCoreState_Mfccore.end() ? CRect() : paneStateIt->second.recentRect;
        if (rect.Width() <= 0 || rect.Height() <= 0) {
            HWND hWnd = pane->GetSafeHwnd();
            if (hWnd) {
                RECT rc{};
                pane->GetWindowRect(&rc);
                rect = rc;
            }
        }

        snapshot.ids.push_back(id);
        snapshot.rects.push_back(rect);
        snapshot.hidden.push_back(state.hiddenPanes.find(pane) != state.hiddenPanes.end() ? TRUE : FALSE);
        snapshot.floating.push_back(state.floatingPanes.find(pane) != state.floatingPanes.end() ? TRUE : FALSE);
        usedIds.insert(id);
    }

    const std::wstring profile = DockingProfileName(lpszProfileName);
    std::lock_guard<std::mutex> lock(g_dockingProfileMutex);
    g_dockingProfileStates[pThis][profile] = std::move(snapshot);
    return TRUE;
}
// Symbol: ?SendMessageToMiniFrames@CDockingManager@@QEAAHI_K_J@Z
extern "C" int MS_ABI impl__SendMessageToMiniFrames_CDockingManager__QEAAHI_K_J_Z(
    CDockingManager* pThis, unsigned int message, unsigned __int64 wParam, __int64 lParam) {
    if (!pThis) return FALSE;
    const DockingManagerState* state = FindDockingState(pThis);
    if (!state) return FALSE;

    int sent = 0;
    for (void* pFrame : state->miniFrames) {
        CWnd* frame = static_cast<CWnd*>(pFrame);
        if (!frame) continue;
        HWND hWnd = frame->GetSafeHwnd();
        if (!hWnd) continue;
        ::SendMessageW(hWnd, message, static_cast<WPARAM>(wParam), static_cast<LPARAM>(lParam));
        ++sent;
    }
    return sent > 0 ? TRUE : FALSE;
}
CDockingManager::CDockingManager()
    : m_pParentWnd(nullptr) {
    memset(_dockingmanager_padding, 0, sizeof(_dockingmanager_padding));
    EnsureDockingState(this).parentFrame = nullptr;
}
CDockingManager::CDockingManager(CFrameWnd* pParentFrameWnd)
    : m_pParentWnd(pParentFrameWnd) {
    memset(_dockingmanager_padding, 0, sizeof(_dockingmanager_padding));
    EnsureDockingState(this).parentFrame = pParentFrameWnd;
}
CDockingManager::~CDockingManager() {
    if (const DockingManagerState* found = FindDockingState(this)) {
        DockingManagerState& state = const_cast<DockingManagerState&>(*found);
        std::free(state.dockSites);
        std::free(state.hiddenMDITabbedBars);
        std::free(state.printPreviewHiddenBars);
        state.dockSites = nullptr;
        state.hiddenMDITabbedBars = nullptr;
        state.printPreviewHiddenBars = nullptr;
        state.dockSiteCount = state.dockSiteCapacity = 0;
        state.hiddenMDITabbedBarCount = state.hiddenMDITabbedBarCapacity = 0;
        state.printPreviewHiddenBarCount = state.printPreviewHiddenBarCapacity = 0;
    }
    RemoveDockingState(this);
    std::lock_guard<std::mutex> lock(g_dockingProfileMutex);
    g_dockingProfileStates.erase(this);
}
void CDockingManager::DockPane(CBasePane* pBar, UINT, LPCRECT) {
    AddDockingPane(this, pBar);
    DockingManagerState& state = EnsureDockingState(this);
    state.hiddenPanes.erase(pBar);
    state.floatingPanes.erase(pBar);
}
BOOL CDockingManager::DockPaneLeftOf(CBasePane* pBarToDock, CBasePane* pBar) {
    if (!pBarToDock) return FALSE;

    DockingManagerState& state = EnsureDockingState(this);
    state.panes.erase(std::remove(state.panes.begin(), state.panes.end(), pBarToDock), state.panes.end());

    auto it = std::find(state.panes.begin(), state.panes.end(), pBar);
    if (it != state.panes.end()) {
        state.panes.insert(it, pBarToDock);
    } else {
        state.panes.push_back(pBarToDock);
    }

    state.hiddenPanes.erase(pBarToDock);
    state.floatingPanes.erase(pBarToDock);
    return TRUE;
}
BOOL CDockingManager::EnableDocking(DWORD dwDockStyle) {
    EnsureDockingState(this).enabledAlignment = dwDockStyle;
    return TRUE;
}
void CDockingManager::FloatPane(CBasePane* pBar, CPoint, DWORD) {
    AddDockingPane(this, pBar);
    DockingManagerState& state = EnsureDockingState(this);
    state.hiddenPanes.erase(pBar);
    state.floatingPanes.insert(pBar);
    if (pBar) {
        std::lock_guard<std::mutex> lock(g_paneCoreStateMutex_Mfccore);
        PaneCoreState& paneState = g_paneCoreState_Mfccore[pBar];
        paneState.canFloat = TRUE;
        paneState.visible = TRUE;
    }
}
void CDockingManager::HidePane(CBasePane* pBar) {
    if (!pBar) return;
    AddDockingPane(this, pBar);
    EnsureDockingState(this).hiddenPanes.insert(pBar);
    std::lock_guard<std::mutex> lock(g_paneCoreStateMutex_Mfccore);
    g_paneCoreState_Mfccore[pBar].visible = FALSE;
}
void CDockingManager::ShowPane(CBasePane* pBar, BOOL) {
    if (!pBar) return;
    AddDockingPane(this, pBar);
    EnsureDockingState(this).hiddenPanes.erase(pBar);
    std::lock_guard<std::mutex> lock(g_paneCoreStateMutex_Mfccore);
    g_paneCoreState_Mfccore[pBar].visible = TRUE;
}
void CDockingManager::RecalcLayout() {
    DockingManagerState& state = EnsureDockingState(this);
    if (state.lockUpdate) return;

    RECT client{0, 0, 0, 0};
    HWND parent = m_pParentWnd ? m_pParentWnd->GetSafeHwnd() : nullptr;
    if (parent) {
        ::GetClientRect(parent, &client);
    }
    int visibleCount = 0;
    for (CBasePane* pane : state.panes) {
        if (IsPaneVisibleForDocking(state, pane)) ++visibleCount;
    }
    if (visibleCount <= 0) return;

    int index = 0;
    int width = (client.right > client.left) ? (client.right - client.left) / visibleCount : 0;
    for (CBasePane* pane : state.panes) {
        if (!IsPaneVisibleForDocking(state, pane)) continue;
        CRect rect(client.left + index * width, client.top,
                   index == visibleCount - 1 ? client.right : client.left + (index + 1) * width,
                   client.bottom);
        {
            std::lock_guard<std::mutex> lock(g_paneCoreStateMutex_Mfccore);
            g_paneCoreState_Mfccore[pane].recentRect = rect;
            g_paneCoreState_Mfccore[pane].visible = TRUE;
        }
        if (pane->GetSafeHwnd() && rect.Width() > 0 && rect.Height() > 0) {
            pane->SetWindowPos(nullptr, rect.left, rect.top, rect.Width(), rect.Height(), SWP_NOZORDER | SWP_NOACTIVATE, nullptr);
        } else {
            pane->RecalcLayout();
        }
        ++index;
    }
}
void CDockingManager::SetDockState() {
    DockingManagerState& state = EnsureDockingState(this);
    std::lock_guard<std::mutex> lock(g_paneCoreStateMutex_Mfccore);
    for (CBasePane* pane : state.panes) {
        if (!pane) continue;
        PaneCoreState& paneState = g_paneCoreState_Mfccore[pane];
        paneState.visible = state.hiddenPanes.find(pane) == state.hiddenPanes.end();
        paneState.canFloat = paneState.canFloat || state.floatingPanes.find(pane) != state.floatingPanes.end();
        if (state.enabledAlignment != 0 && paneState.alignment == 0) {
            paneState.alignment = state.enabledAlignment;
        }
    }
}
// Symbol: ?m_SDParams@CDockingManager@@1VCSmartDockingInfo@@A
// Retail's exported static is a CSmartDockingInfo at RVA 0x3c15b0 (mfc140u), not a pointer.
// Seed its verified constructor defaults in static storage; the ctor thunk below installs
// the vtable and applies the same defaults before exported calls can observe the object.
extern "C" S_Csmartdockinginfo impl__m_SDParams_CDockingManager__1VCSmartDockingInfo__A = {
    nullptr, 93, 93, 5,
    0x00FF00FFu, 0xFFFFFFFFu, 0xFFFFFFFFu, 0xFFFFFFFFu, 0xFFFFFFFFu,
    {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, 0, FALSE, FALSE
};

namespace {
struct DockingSmartParamsInitializer {
    DockingSmartParamsInitializer() {
        // Use the already-defined constructor thunk: retail's CSmartDockingInfo ctor
        // initializes a 93x93 marker size, offset 5, magenta transparency, and zero IDs.
        impl___0CSmartDockingInfo__QEAA_XZ(&impl__m_SDParams_CDockingManager__1VCSmartDockingInfo__A);
    }
};
DockingSmartParamsInitializer g_dockingSmartParamsInitializer;
}

// The following side-table implementations use defensive null checks; retail often
// dereferences these inputs. Side-array allocation failure also returns FALSE rather
// than reproducing an unmodeled allocation-exception path.
// Symbol: ?AddDockSite@CDockingManager@@QEAAHAEBUAFX_DOCKSITE_INFO@@PEAPEAVCDockSite@@@Z
// Retail entry RVA 0x47660 (mfc140u) creates the supplied runtime class and
// virtually calls Create with the manager's parent before appending the site. This
// implementation calls the local CDockSite::Create thunk directly (vtable deviation);
// that local thunk records parent/rectangle state but does not create a native window.
// dockSites is an ordered side-table of created dock sites, not a complete
// representation of the manager's retail m_lstControlBars.
extern "C" int MS_ABI impl__AddDockSite_CDockingManager__QEAAHAEBUAFX_DOCKSITE_INFO__PEAPEAVCDockSite___Z(
    CDockingManager* pThis, const void* pInfo, void** ppDockSite) {
    if (ppDockSite) *ppDockSite = nullptr;
    if (!pThis || !pInfo) return FALSE;

    const auto* info = static_cast<const DockSiteInfoShadow*>(pInfo);
    DockingManagerState& state = EnsureDockingState(pThis);
    void* site = CreateDockSiteForManager(pThis, info);
    if (!site) return FALSE;
    if (!InsertManagerStateEntry(state.dockSites, state.dockSiteCount,
                                 state.dockSiteCapacity, state.dockSiteCount, site)) {
        impl___1CDockSite__UEAA_XZ(site);
        impl___3_YAXPEAX_Z(site);
        return FALSE;
    }
    if (ppDockSite) *ppDockSite = site;
    return TRUE;
}

// Symbol: ?AddHiddenMDITabbedBar@CDockingManager@@QEAAXPEAVCDockablePane@@@Z
// Retail entry RVA 0x4ebc0 (mfc140u) appends pPane->m_hWnd (+0x40) to the
// manager's hidden-MDI-tabbed-bar list (+0x2d0). The side table stores that HWND
// bit-pattern in its pointer-sized hiddenMDITabbedBars entries.
extern "C" void MS_ABI impl__AddHiddenMDITabbedBar_CDockingManager__QEAAXPEAVCDockablePane___Z(
    CDockingManager* pThis, CDockablePane* pPane) {
    if (!pThis) return;
    HWND hWnd = pPane ? impl__GetSafeHwnd_CWnd__QEBAPEAUHWND____XZ(
                            static_cast<const CWnd*>(pPane)) : nullptr;
    DockingManagerState& state = EnsureDockingState(pThis);
    InsertManagerStateEntry(state.hiddenMDITabbedBars,
                            state.hiddenMDITabbedBarCount,
                            state.hiddenMDITabbedBarCapacity,
                            state.hiddenMDITabbedBarCount,
                            reinterpret_cast<CBasePane*>(hWnd));
}

// Symbol: ?AlignByRect@CDockingManager@@IEAAXAEBVCRect@@AEAV2@KHH@Z
// Retail RVA 0x4a340 (mfc140u): apply the 0xF000 alignment case and the two
// size-adjustment flags to the target rectangle relative to the anchor rectangle.
// WARNING: This pre-existing approximation does not preserve the retail per-case
// coordinates: retail leaves one or both target vertical edges unchanged in cases
// where this body recomputes them from the anchor. Kept because this does not write
// outside either caller-provided CRect; it is not a faithful retail transcription.
extern "C" void MS_ABI impl__AlignByRect_CDockingManager__IEAAXAEBVCRect__AEAV2_KHH_Z(
    CDockingManager*, const CRect* pAnchor, CRect* pTarget, unsigned long dwAlignment,
    int bAdjustFirst, int bAdjustSecond) {
    const int anchorLeft = pAnchor->left;
    const int anchorTop = pAnchor->top;
    const int anchorRight = pAnchor->right;
    const int anchorBottom = pAnchor->bottom;
    const int targetWidth = pTarget->right - pTarget->left;
    const int targetHeight = pTarget->bottom - pTarget->top;
    const unsigned long alignment = dwAlignment & 0xf000;

    switch (alignment) {
    case 0x1000:
        pTarget->left = anchorLeft;
        pTarget->top = anchorTop;
        pTarget->right = anchorLeft + targetWidth;
        pTarget->bottom = anchorBottom;
        break;
    case 0x2000:
        pTarget->left = anchorLeft;
        pTarget->top = anchorTop;
        pTarget->right = anchorRight;
        pTarget->bottom = anchorTop + targetHeight;
        break;
    case 0x4000:
        pTarget->right = anchorRight;
        pTarget->bottom = anchorBottom;
        pTarget->left = anchorRight - targetWidth;
        pTarget->top = anchorTop;
        break;
    case 0x8000:
        pTarget->right = anchorRight;
        pTarget->bottom = anchorBottom;
        pTarget->left = anchorLeft;
        pTarget->top = anchorBottom - targetHeight;
        break;
    default:
        break;
    }

    if (bAdjustFirst) {
        const int widthDelta = (pAnchor->left - pAnchor->right) - pTarget->left + pTarget->right;
        if (widthDelta != 0) pTarget->right += widthDelta;

        const int heightDelta = (pAnchor->top - pAnchor->bottom) - pTarget->top + pTarget->bottom;
        if (heightDelta > 0 && bAdjustSecond) {
            if ((alignment & 0x2000) != 0) {
                pTarget->bottom -= heightDelta;
            } else if ((alignment & 0x8000) != 0) {
                pTarget->top += heightDelta;
            }
        }
    } else {
        const int heightDelta = (pAnchor->top - pAnchor->bottom) - pTarget->top + pTarget->bottom;
        if (heightDelta != 0) pTarget->bottom += heightDelta;

        const int widthDelta = (pTarget->right - pTarget->left) - (pAnchor->right - pAnchor->left);
        if (widthDelta > 0 && bAdjustSecond) {
            if ((alignment & 0x1000) != 0) {
                pTarget->right -= widthDelta;
            } else if ((alignment & 0x4000) != 0) {
                pTarget->left += widthDelta;
            }
        }
    }
}

// Symbol: ?CalcPaneOffset@CDockingManager@@IEAAXPEAVCAutoHideDockSite@@@Z
// STUB: Retail entry RVA 0x4a100 (mfc140u) compares neighboring auto-hide dock-site
// rectangles/orientations and updates their +0x220/+0x224 offsets. The manager's
// dockSites side array exists, but this TU has no typed accessor for the auto-hide
// site's offset fields and adjacent-site geometry.
extern "C" void MS_ABI impl__CalcPaneOffset_CDockingManager__IEAAXPEAVCAutoHideDockSite___Z(
    CDockingManager*, void*) {}

// Symbol: ?Create@CDockingManager@@QEAAHPEAVCFrameWnd@@@Z
// Retail RVA 0x473e0 (mfc140u) stores the frame at this+0x1b0 and returns TRUE.
// OpenMFC's m_pParentWnd is at this+sizeof(CObject), so store through that local
// member location rather than writing the incompatible retail offset.
extern "C" int MS_ABI impl__Create_CDockingManager__QEAAHPEAVCFrameWnd___Z(
    CDockingManager* pThis, CFrameWnd* pFrame) {
    *reinterpret_cast<CFrameWnd**>(reinterpret_cast<unsigned char*>(pThis) + sizeof(CObject)) = pFrame;
    EnsureDockingState(pThis).parentFrame = pFrame;
    return TRUE;
}

// Symbol: ?FindDockSite@CDockingManager@@UEAAPEAVCDockSite@@KH@Z
// Retail entry RVA 0x481e0 (mfc140u) starts at the head when bOuter is set and
// walks forward; otherwise it starts at the tail and walks backward. It returns the
// first CDockSite whose alignment mask matches. dockSites preserves only the relative
// order of dock sites, not the complete m_lstControlBars sequence.
extern "C" void* MS_ABI impl__FindDockSite_CDockingManager__UEAAPEAVCDockSite__KH_Z(
    CDockingManager* pThis, unsigned long alignment, int bOuter) {
    const DockingManagerState* state = FindDockingState(pThis);
    if (!state) return nullptr;

    for (std::size_t step = 0; step < state->dockSiteCount; ++step) {
        const std::size_t index = bOuter ? step : state->dockSiteCount - step - 1;
        void* site = state->dockSites[index];
        if (!site) continue;
        const auto* pane = reinterpret_cast<const CBasePane*>(site);
        if ((impl__GetCurrentAlignment_CBasePane__UEBAKXZ(pane) & 0xf000UL) ==
            (alignment & 0xf000UL)) {
            return site;
        }
    }
    return nullptr;
}

// Symbol: ?FindDockSiteByPane@CDockingManager@@UEAAPEAVCDockSite@@PEAVCPane@@@Z
// Retail entry RVA 0x48500 (mfc140u) obtains the pane's control ID, scans the
// manager dock-site list, and asks each site to FindPaneByID before comparing pointers.
extern "C" void* MS_ABI impl__FindDockSiteByPane_CDockingManager__UEAAPEAVCDockSite__PEAVCPane___Z(
    CDockingManager* pThis, CPane* pPane) {
    const DockingManagerState* state = FindDockingState(pThis);
    if (!pThis || !pPane || !state) return nullptr;

    const unsigned int paneID = static_cast<unsigned int>(
        impl__GetDlgCtrlID_CWnd__QEBAHXZ(static_cast<const CWnd*>(pPane)));
    for (std::size_t i = 0; i < state->dockSiteCount; ++i) {
        void* site = state->dockSites[i];
        if (site && impl__FindPaneByID_CDockSite__QEAAPEAVCPane__I_Z(site, paneID) == pPane) {
            return site;
        }
    }
    return nullptr;
}

// Symbol: ?HideForPrintPreview@CDockingManager@@IEAAXAEBVCObList@@@Z
// STUB: Retail entry RVA 0x4cfb0 (mfc140u) applies virtual type/visibility/eligibility
// checks to the supplied CObList. It separately scans manager mini-frames, validates
// their HWNDs, hides valid windows, and records eligible objects in
// m_lstBarsHiddenInPreview (+0x210). The side table has a saved-entry array, but not
// the input-list traversal and eligibility state.
extern "C" void MS_ABI impl__HideForPrintPreview_CDockingManager__IEAAXAEBVCObList___Z(
    CDockingManager*, const CObList*) {}

// Symbol: ?InsertDockSite@CDockingManager@@QEAAHAEBUAFX_DOCKSITE_INFO@@KPEAPEAVCDockSite@@@Z
// Retail entry RVA 0x47730 (mfc140u) creates the supplied runtime class, virtually
// calls Create, inserts after the first site matching dwAlignToInsertAfter & 0xf000
// (or appends if none matches), and stores the optional result. The local Create call
// is through the base thunk rather than a retail vtable and only records side state;
// dockSites preserves relative order among dock sites, not all manager-list entries.
extern "C" int MS_ABI impl__InsertDockSite_CDockingManager__QEAAHAEBUAFX_DOCKSITE_INFO__KPEAPEAVCDockSite___Z(
    CDockingManager* pThis, const void* pInfo, unsigned long dwAlignToInsertAfter,
    void** ppDockSite) {
    if (ppDockSite) *ppDockSite = nullptr;
    if (!pThis || !pInfo) return FALSE;

    const auto* info = static_cast<const DockSiteInfoShadow*>(pInfo);
    DockingManagerState& state = EnsureDockingState(pThis);
    std::size_t insertionIndex = state.dockSiteCount;
    for (std::size_t i = 0; i < state.dockSiteCount; ++i) {
        void* existing = state.dockSites[i];
        if (!existing) continue;
        const auto* pane = reinterpret_cast<const CBasePane*>(existing);
        if ((impl__GetCurrentAlignment_CBasePane__UEBAKXZ(pane) & 0xf000UL) ==
            (dwAlignToInsertAfter & 0xf000UL)) {
            insertionIndex = i + 1;
            break;
        }
    }

    void* site = CreateDockSiteForManager(pThis, info);
    if (!site) return FALSE;
    if (!InsertManagerStateEntry(state.dockSites, state.dockSiteCount,
                                 state.dockSiteCapacity, insertionIndex, site)) {
        impl___1CDockSite__UEAA_XZ(site);
        impl___3_YAXPEAX_Z(site);
        return FALSE;
    }
    if (ppDockSite) *ppDockSite = site;
    return TRUE;
}

// Symbol: ?IsPointNearDockSite@CDockingManager@@QEBAHVCPoint@@AEAKAEAH@Z
// STUB: Retail entry RVA 0x48310 (mfc140u) converts manager bounds at +0x1d8/+0x1c8
// through the parent frame before calling CGlobalUtils::CheckAlignment at entry RVA
// 0x6caf0 (mfc140u). The side-table bounds have no writer, and the local sensitivity
// datum defaults to 0 rather than retail's 15; usable inputs are not represented here.
extern "C" int MS_ABI impl__IsPointNearDockSite_CDockingManager__QEBAHVCPoint__AEAKAEAH_Z(
    const CDockingManager*, long long, unsigned long* pAlignment, int* pOuterEdge) {
    if (pAlignment) *pAlignment = 0;
    if (pOuterEdge) *pOuterEdge = FALSE;
    return FALSE;
}

// Symbol: ?Serialize@CDockingManager@@UEAAXAEAVCArchive@@@Z
// STUB: The ANSI twin's retail entry at RVA 0x4aa90 (mfc140) has a long archive traversal
// over retail dock-site and manager-list state. The side table tracks pane/frame pointers,
// but it is not the retail CObList layout/state required by this serialization routine.
extern "C" void MS_ABI impl__Serialize_CDockingManager__UEAAXAEAVCArchive___Z(
    CDockingManager* pThis, CArchive& archive) {
    (void)pThis;
    (void)archive;
}

// Symbol: ?SetAutohideZOrder@CDockingManager@@QEAAXPEAVCDockablePane@@@Z
// STUB: The ANSI twin's retail entry RVA 0x49640 (mfc140) walks the manager control-bar
// list from its tail, filters pane/auto-hide-bar runtime classes, compares alignment
// and divider relationships, then dispatches virtual positioning operations. The local
// side table does not preserve the required complete membership/order and relationships.
extern "C" void MS_ABI impl__SetAutohideZOrder_CDockingManager__QEAAXPEAVCDockablePane___Z(
    CDockingManager*, CDockablePane*) {}

// Symbol: ?SetDockingMode@CDockingManager@@SAXW4AFX_DOCK_TYPE@@W4AFX_SMARTDOCK_THEME@@@Z
// Retail ANSI-twin entry RVA 0x4da50 (mfc140; --u does not resolve this name): store
// the mode; only mode 0x80 becomes 0x81 and stores the theme. The theme is stored in
// DockingSmartTheme() from detail/DockingState.h, the shared local shadow for m_SDTheme.
extern "C" void MS_ABI impl__SetDockingMode_CDockingManager__SAXW4AFX_DOCK_TYPE__W4AFX_SMARTDOCK_THEME___Z(
    int dockingMode, int theme) {
    impl__m_dockModeGlobal_CDockingManager__1W4AFX_DOCK_TYPE__A =
        static_cast<std::uint32_t>(dockingMode);
    if (dockingMode == 0x80) {
        impl__m_dockModeGlobal_CDockingManager__1W4AFX_DOCK_TYPE__A = 0x81;
        openmfc::docking_state::DockingSmartTheme() = theme;
    }
}

// Symbol: ?SetPrintPreviewMode@CDockingManager@@QEAAXHPEAUCPrintPreviewState@@@Z
// STUB: The ANSI twin's retail entry RVA 0x4ced0 (mfc140) toggles +0x208 and, when
// parent/OLE conditions permit, hides/restores eligible entries through manager lists.
// printPreviewMode and printPreviewHiddenBars are modeled, but the second manager list
// and parent-frame OLE-notify-hook state needed to reproduce the path are not.
extern "C" void MS_ABI impl__SetPrintPreviewMode_CDockingManager__QEAAXHPEAUCPrintPreviewState___Z(
    CDockingManager*, int, void*) {}

// Symbol: ?SetSmartDockingParams@CDockingManager@@SAXAEAVCSmartDockingInfo@@@Z
// Retail ANSI-twin entry RVA 0x4da70 (mfc140; --u does not resolve this name): copy
// the object fields only when all five marker IDs are zero or all are nonzero, then
// set m_bSDParamsModified. CopyTo at ANSI-twin entry RVA 0x4ebd0 copies the 84 bytes
// after the vfptr through m_bIsAlphaMarkers at offset 0x58, leaving tail padding.
extern "C" void MS_ABI impl__SetSmartDockingParams_CDockingManager__SAXAEAVCSmartDockingInfo___Z(
    void* pParams) {
    const auto* source = static_cast<const S_Csmartdockinginfo*>(pParams);
    unsigned int marked = 0;
    for (UINT id : source->m_uiMarkerBmpResID) {
        if (id != 0) ++marked;
    }
    if (marked != 0 && marked != AFX_SD_MARKERS_NUM) return;

    auto* destination = &impl__m_SDParams_CDockingManager__1VCSmartDockingInfo__A;
    constexpr std::size_t copiedBytes =
        offsetof(S_Csmartdockinginfo, m_bIsAlphaMarkers) + sizeof(BOOL) - sizeof(void*);
    if (source != destination) {
        std::memcpy(reinterpret_cast<unsigned char*>(destination) + sizeof(void*),
                    reinterpret_cast<const unsigned char*>(source) + sizeof(void*),
                    copiedBytes);
    }
    impl__m_bSDParamsModified_CDockingManager__1HA = TRUE;
}

// Symbol: ?ShowDelayShowMiniFrames@CDockingManager@@UEAAXH@Z
// The ANSI twin's retail entry RVA 0x4d6c0 (mfc140) scans mini-frames, tests
// m_bDelayShow (+0x108), calls USER32 IsWindow and (when valid) ShowWindow, then
// clears the flag. The flag is modeled in PaneFrameState, but its only accessor is a
// C++ helper in another TU; this file cannot call it without violating the thunk rule.
extern "C" void MS_ABI impl__ShowDelayShowMiniFrames_CDockingManager__UEAAXH_Z(
    CDockingManager*, int) {}

// Symbol: ?ShowPanes@CDockingManager@@UEAAHH@Z
// STUB: The ANSI twin's retail entry RVA 0x4d490 (mfc140) has separate show/hide
// paths over docked-bar and mini-frame lists, saves/restores HWNDs, updates global
// restoring/saving flags, and toggles +0x354. showPanesMode and basic hidden flags are
// modeled, but the saved HWND list and global-mode state are not.
extern "C" int MS_ABI impl__ShowPanes_CDockingManager__UEAAHH_Z(
    CDockingManager*, int) {
    return FALSE;
}
