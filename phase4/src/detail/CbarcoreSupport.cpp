#include "CbarcoreSupport.h"

IMPLEMENT_DYNAMIC(CToolBar, CControlBar)
IMPLEMENT_DYNAMIC(CStatusBar, CControlBar)
IMPLEMENT_DYNAMIC(CDialogBar, CControlBar)
IMPLEMENT_DYNAMIC(CSplitterWnd, CWnd)
void OpenMFC_CopyComboButtonState(CMFCToolBarComboBoxButton* dst, const CMFCToolBarComboBoxButton* src) {
    if (!dst || !src) return;
    if (const ComboButtonState* state = FindComboState(src)) {
        g_comboStates[dst] = *state;
    }
    RegisterComboButton(dst);
}
void OpenMFC_CopyEditButtonState(CMFCToolBarEditBoxButton* dst, const CMFCToolBarEditBoxButton* src) {
    if (!dst || !src) return;
    RegisterEditButton(dst);
    auto it = g_editContentsByCmd.find(src->m_nID);
    if (it != g_editContentsByCmd.end()) {
        g_editContentsByCmd[dst->m_nID] = it->second;
    }
}
namespace openmfc { namespace detail { namespace cbarcore {
decltype(openmfc::ribbon_state::RibbonMutex()) g_ribbonMutex = openmfc::ribbon_state::RibbonMutex();
decltype(openmfc::ribbon_state::RibbonPanelStates()) g_ribbonPanels = openmfc::ribbon_state::RibbonPanelStates();
decltype(openmfc::ribbon_state::RibbonCategoryStates()) g_ribbonCategories = openmfc::ribbon_state::RibbonCategoryStates();
decltype(openmfc::ribbon_state::RibbonBarStates()) g_ribbonBars = openmfc::ribbon_state::RibbonBarStates();
std::unordered_map<UINT, std::wstring> g_ribbonToolTips;
std::unordered_map<UINT, std::wstring> g_ribbonDescriptions;
std::unordered_map<UINT, int> g_galleryLastSelectedByID;
std::unordered_map<const CMFCRibbonBaseElement*, bool> g_ribbonElementsEnabled;
std::unordered_map<const CMFCRibbonBaseElement*, CSize> g_ribbonTextSizeById;
std::unordered_map<UINT, RibbonSliderState> g_sliderStates;
std::unordered_map<UINT, RibbonProgressState> g_progressStates;
std::unordered_map<UINT, RibbonStatusPaneState> g_statusPaneStates;
std::mutex g_threadSlotMutex;
std::unordered_map<void*, ThreadSlotDataState> g_threadSlotStates;
std::mutex g_typeLibCacheMutex;
std::unordered_map<void*, TypeLibCacheState> g_typeLibCaches;
std::unordered_map<const CCmdTarget*, std::unique_ptr<TypeLibCacheHandle>> g_targetTypeLibCaches;
std::unordered_map<std::wstring, std::unique_ptr<TypeLibCacheHandle>> g_guidTypeLibCaches;
std::mutex g_splitterWndMutex;
std::unordered_map<const CSplitterWnd*, SplitterLayoutState> g_splitterWndStates;
SplitterLayoutState& EnsureSplitterState(const CSplitterWnd* pSplitter) {
    auto it = g_splitterWndStates.find(pSplitter);
    if (it == g_splitterWndStates.end()) {
        it = g_splitterWndStates.emplace(pSplitter, SplitterLayoutState()).first;
    }

    const int rows = std::max(1, pSplitter ? pSplitter->m_nRows : 1);
    const int cols = std::max(1, pSplitter ? pSplitter->m_nCols : 1);

    if (static_cast<int>(it->second.rowInfo.size()) != rows) {
        it->second.rowInfo.assign(rows, {});
    }
    if (static_cast<int>(it->second.colInfo.size()) != cols) {
        it->second.colInfo.assign(cols, {});
    }
    if (static_cast<int>(it->second.panes.size()) != rows) {
        it->second.panes.assign(rows, std::vector<CWnd*>(cols, nullptr));
    }
    for (auto& row : it->second.panes) {
        if (static_cast<int>(row.size()) != cols) {
            row.assign(cols, nullptr);
        }
    }
    return it->second;
}
CWnd* GetSplitterPane(const CSplitterWnd* pSplitter, int row, int col) {
    if (!pSplitter) return nullptr;
    const SplitterLayoutState& state = EnsureSplitterState(pSplitter);
    if (row < 0 || col < 0 || row >= pSplitter->m_nRows || col >= pSplitter->m_nCols) return nullptr;
    if (row >= static_cast<int>(state.panes.size()) || col >= static_cast<int>(state.panes[row].size())) {
        return nullptr;
    }
    return state.panes[row][col];
}
void NormalizeSplitterState(CSplitterWnd* pThis) {
    if (!pThis) return;
    SplitterLayoutState& state = EnsureSplitterState(pThis);
    const int rows = std::max(1, pThis->m_nRows);
    const int cols = std::max(1, pThis->m_nCols);

    if (static_cast<int>(state.panes.size()) != rows) {
        state.panes.assign(rows, std::vector<CWnd*>(cols, nullptr));
    } else {
        for (auto& row : state.panes) {
            row.assign(cols, nullptr);
        }
    }
    if (static_cast<int>(state.rowInfo.size()) != rows) {
        state.rowInfo.assign(rows, {});
    }
    if (static_cast<int>(state.colInfo.size()) != cols) {
        state.colInfo.assign(cols, {});
    }

    for (auto& row : state.rowInfo) {
        if (row.min <= 0) row.min = pThis->m_sizeMin.cy;
        if (row.ideal < row.min) row.ideal = row.min;
    }
    for (auto& col : state.colInfo) {
        if (col.min <= 0) col.min = pThis->m_sizeMin.cx;
        if (col.ideal < col.min) col.ideal = col.min;
    }
}
std::vector<int> SplitterSizesFromInfo(int total, const std::vector<SplitterDim>& info, int fallbackMin) {
    const int count = static_cast<int>(info.size());
    if (count <= 0) return {};
    std::vector<int> sizes(count, std::max(0, fallbackMin));
    int base = 0;
    int weightTotal = 0;
    std::vector<int> weights(count, 1);

    for (int i = 0; i < count; ++i) {
        const int minSize = std::max(info[i].min, fallbackMin);
        sizes[i] = minSize;
        base += minSize;
        weights[i] = std::max(1, info[i].ideal);
        weightTotal += weights[i];
    }

    if (base >= total || weightTotal == 0) return sizes;

    int remaining = total - base;
    for (int i = 0; i < count; ++i) {
        int extra = (remaining * weights[i]) / weightTotal;
        sizes[i] += extra;
    }
    int used = 0;
    for (int v : sizes) used += v;
    int rem = total - used;
    for (int i = 0; rem > 0 && i < count; ++i, --rem) {
        ++sizes[i];
    }
    return sizes;
}
std::mutex g_userToolMutex;
std::unordered_map<void*, UserToolState> g_userTools;
std::unordered_map<void*, UserToolsManagerState> g_userToolManagers;
std::mutex g_taskDialogMutex;
std::unordered_map<CTaskDialog*, TaskDialogState> g_taskDialogs;
std::mutex g_tabbedPaneMutex;
std::unordered_map<void*, TabbedPaneState> g_tabbedPanes;
std::unordered_map<void*, HDC> g_windowlessDCs;
std::unordered_set<void*> g_d2dInitialized;
thread_local _AFX_D2D_STATE g_d2dState;
__attribute__((used)) CRuntimeClass g_classCUserException = {
    "CUserException", sizeof(CException), 0xFFFF, nullptr, nullptr, &CException::classCException, nullptr
};
__attribute__((used)) CRuntimeClass g_classCUserTool = {
    "CUserTool", sizeof(void*), 0xFFFF, nullptr, nullptr, &CObject::classCObject, nullptr
};
__attribute__((used)) CRuntimeClass g_classCTabbedPane = {
    "CTabbedPane", sizeof(void*), 0xFFFF, nullptr, nullptr, &CWnd::classCWnd, nullptr
};
__attribute__((used)) CRuntimeClass g_classCTaskDialog = {
    "CTaskDialog", sizeof(CTaskDialog), 0xFFFF, nullptr, nullptr, &CObject::classCObject, nullptr
};
__attribute__((used)) CRuntimeClass g_classCWindowlessDC = {
    "CWindowlessDC", sizeof(CDC), 0xFFFF, nullptr, nullptr, &CDC::classCDC, nullptr
};
void AddTaskDialogButton(TaskDialogButtonState& state, int id, const wchar_t* label, int enabled) {
    auto it = std::find_if(state.buttons.begin(), state.buttons.end(),
        [id](const TASKDIALOG_BUTTON& button) { return button.nButtonID == id; });
    std::wstring text = label ? label : L"";
    if (it == state.buttons.end()) {
        state.labels.push_back(text);
        state.buttons.push_back({ id, nullptr });
    } else {
        const size_t index = static_cast<size_t>(std::distance(state.buttons.begin(), it));
        state.labels[index] = text;
    }
    for (size_t i = 0; i < state.buttons.size(); ++i) {
        state.buttons[i].pszButtonText = state.labels[i].c_str();
    }
    if (enabled) {
        state.disabled.erase(id);
    } else {
        state.disabled.insert(id);
    }
}
TASKDIALOG_BUTTON* FirstTaskDialogButton(TaskDialogButtonState& state) {
    return state.buttons.empty() ? nullptr : state.buttons.data();
}
ptrdiff_t FindTaskDialogButtonIndex(const TaskDialogButtonState& state, int id) {
    for (size_t i = 0; i < state.buttons.size(); ++i) {
        if (state.buttons[i].nButtonID == id) return static_cast<ptrdiff_t>(i);
    }
    return -1;
}
int CommonButtonId(int commonButtonFlag) {
    switch (commonButtonFlag) {
        case TDCBF_OK_BUTTON: return IDOK;
        case TDCBF_YES_BUTTON: return IDYES;
        case TDCBF_NO_BUTTON: return IDNO;
        case TDCBF_CANCEL_BUTTON: return IDCANCEL;
        case TDCBF_RETRY_BUTTON: return IDRETRY;
        case TDCBF_CLOSE_BUTTON: return IDCLOSE;
        default: return 0;
    }
}
int CommonButtonFlagByIndex(int commonButtons, int index) {
    const int flags[] = {
        TDCBF_OK_BUTTON, TDCBF_YES_BUTTON, TDCBF_NO_BUTTON,
        TDCBF_CANCEL_BUTTON, TDCBF_RETRY_BUTTON, TDCBF_CLOSE_BUTTON
    };
    int seen = 0;
    for (int flag : flags) {
        if ((commonButtons & flag) == 0) continue;
        if (seen == index) return flag;
        ++seen;
    }
    return 0;
}
void FillTaskDialogConfig(CTaskDialog* dialog, TASKDIALOGCONFIG& config) {
    memset(&config, 0, sizeof(config));
    config.cbSize = sizeof(TASKDIALOGCONFIG);
    if (!dialog) return;

    config.dwFlags = dialog->m_nTaskDialogOptions;
    config.dwCommonButtons = dialog->m_nCommonButtons;
    config.pszWindowTitle = dialog->m_strWindowTitle;
    config.pszMainInstruction = dialog->m_strMainInstruction;
    config.pszContent = dialog->m_strContent;
    config.hMainIcon = dialog->m_hMainIcon;
    config.hFooterIcon = dialog->m_hFooterIcon;
    config.pszFooter = dialog->m_strFooterText.IsEmpty() ? nullptr : (const wchar_t*)dialog->m_strFooterText;
    config.pszVerificationText =
        dialog->m_strVerificationText.IsEmpty() ? nullptr : (const wchar_t*)dialog->m_strVerificationText;
    config.pszExpandedInformation =
        dialog->m_strExpandedInfo.IsEmpty() ? nullptr : (const wchar_t*)dialog->m_strExpandedInfo;
    config.pszExpandedControlText =
        dialog->m_strExpandedControlText.IsEmpty() ? nullptr : (const wchar_t*)dialog->m_strExpandedControlText;
    config.pszCollapsedControlText =
        dialog->m_strCollapsedControlText.IsEmpty() ? nullptr : (const wchar_t*)dialog->m_strCollapsedControlText;
    if (dialog->m_nDialogWidth > 0) config.cxWidth = dialog->m_nDialogWidth;

    std::lock_guard<std::mutex> lock(g_taskDialogMutex);
    TaskDialogState& state = g_taskDialogs[dialog];
    config.pButtons = FirstTaskDialogButton(state.commandControls);
    config.cButtons = static_cast<UINT>(state.commandControls.buttons.size());
    config.nDefaultButton = state.commandControls.defaultButton;
    config.pRadioButtons = FirstTaskDialogButton(state.radioButtons);
    config.cRadioButtons = static_cast<UINT>(state.radioButtons.buttons.size());
    config.nDefaultRadioButton = state.radioButtons.defaultButton;
}
void* GetGuidTypeLibCache(const GUID& guid) {
    wchar_t guidText[64] = {};
    ::StringFromGUID2(guid, guidText, 64);
    std::wstring key = guidText;
    auto it = g_guidTypeLibCaches.find(key);
    if (it != g_guidTypeLibCaches.end()) {
        return it->second.get();
    }
    auto ownedCache = std::make_unique<TypeLibCacheHandle>();
    void* cache = ownedCache.get();
    g_guidTypeLibCaches.emplace(std::move(key), std::move(ownedCache));
    return cache;
}
decltype(openmfc::docking_state::PaneCoreStateMutex()) g_paneCoreStateMutex_Cbarcore = openmfc::docking_state::PaneCoreStateMutex();
decltype(openmfc::docking_state::PaneCoreStates()) g_paneCoreState_Cbarcore = openmfc::docking_state::PaneCoreStates();
std::unordered_map<const void*, DockSiteCoreState> g_dockSiteCoreState;
CRect NormalizeRect(const RECT* rect, int fallbackWidth, int fallbackHeight) {
    if (rect != nullptr) {
        CRect out(*rect);
        if (out.Width() <= 0) out.right = out.left + fallbackWidth;
        if (out.Height() <= 0) out.bottom = out.top + fallbackHeight;
        return out;
    }
    return CRect(0, 0, fallbackWidth, fallbackHeight);
}
CRect NormalizeRect(const CRect* rect, int fallbackWidth, int fallbackHeight) {
    if (rect != nullptr) {
        CRect out(*rect);
        if (out.Width() <= 0) out.right = out.left + fallbackWidth;
        if (out.Height() <= 0) out.bottom = out.top + fallbackHeight;
        return out;
    }
    return CRect(0, 0, fallbackWidth, fallbackHeight);
}
BOOL TryCreatePaneWindow(CWnd* paneWnd, const wchar_t* className, DWORD style, const CRect& rect, CWnd* parentWnd, UINT id, CCreateContext* context) {
    if (paneWnd == nullptr || parentWnd == nullptr) return FALSE;
    if (paneWnd->GetSafeHwnd() != nullptr) return TRUE;
    const wchar_t* useClass = className != nullptr ? className : L"STATIC";
    const DWORD useStyle = style != 0 ? style : (WS_CHILD | WS_VISIBLE);
    return paneWnd->Create(useClass, L"", useStyle, *static_cast<const RECT*>(rect), parentWnd, id, context);
}
CRuntimeClass* AFXAPI gb_CDockSite() {
    return impl__GetThisClass_CBasePane__SAPEAUCRuntimeClass__XZ();
}
CRuntimeClass g_runtimeClassDockSite = {
    "CDockSite",
    544,
    kNoSerializationSchema,
    nullptr,
    gb_CDockSite,
    nullptr,
    nullptr
};
std::mutex g_featurePackStateMutex;
std::unordered_map<CFrameImpl*, FrameImplState> g_frameImplStates;
std::unordered_map<CMDIChildWndEx*, std::wstring> g_taskbarTabText;
CRuntimeClass g_cmdiClientAreaRuntimeClass = {
    "CMDIClientAreaWnd",
    sizeof(void*),
    0xFFFF,
    nullptr,
    nullptr,
    &CWnd::classCWnd,
    nullptr
};
std::mutex g_statusBarTextMutex;
std::unordered_map<const CMFCStatusBar*, std::unordered_map<int, std::wstring>> g_statusBarPaneText;
std::mutex g_captionBarTextMutex;
std::unordered_map<const CMFCCaptionBar*, CaptionBarState> g_captionBarText;
CString GetStoredPaneText(const CMFCStatusBar* pThis, int nIndex) {
    std::lock_guard<std::mutex> lock(g_statusBarTextMutex);
    auto itBar = g_statusBarPaneText.find(pThis);
    if (itBar == g_statusBarPaneText.end()) {
        return CString();
    }

    auto itText = itBar->second.find(nIndex);
    if (itText == itBar->second.end()) {
        return CString();
    }

    return CString(itText->second.c_str());
}
std::mutex g_wave2Mutex;
std::unordered_map<void*, PaneState> g_framePanes;
std::unordered_map<void*, PaneState> g_multiFramePanes;
std::unordered_map<void*, PaneState> g_containerPanes;
std::unordered_map<void*, PaneState> g_containerManagerPanes;
std::unordered_map<void*, PaneState> g_dividerPanes;
std::unordered_map<void*, PaneState> g_autoHideBarWindows;
std::unordered_map<void*, void*> g_paneToContainer;
std::unordered_map<void*, void*> g_paneToAutoHideButton;
std::unordered_map<void*, std::unique_ptr<int>> g_autoHideButtonStorage;
std::unordered_map<void*, AutoHideButtonState> g_autoHideButtons;
void AddUniquePane(PaneState& state, void* pane) {
    if (!pane) return;
    if (std::find(state.panes.begin(), state.panes.end(), pane) == state.panes.end()) {
        state.panes.push_back(pane);
    }
    if (state.minSize.cx <= 0) state.minSize.cx = kPaneFrameDefaultMinWidth;
    if (state.minSize.cy <= 0) state.minSize.cy = kPaneFrameDefaultMinHeight;
    if (state.recentRect.Width() <= 0 || state.recentRect.Height() <= 0) {
        state.recentRect = CRect(0, 0, kPaneFrameDefaultWidth, kPaneFrameDefaultHeight);
    }
}
void RemovePane(PaneState& state, void* pane) {
    if (!pane) return;
    state.panes.erase(std::remove(state.panes.begin(), state.panes.end(), pane), state.panes.end());
}
void* FirstPane(const PaneState& state) {
    return state.panes.empty() ? nullptr : state.panes.front();
}
thread_local std::unordered_map<const CMFCToolBarComboBoxButton*, ComboButtonState> g_comboStates;
thread_local std::unordered_map<UINT, std::vector<CMFCToolBarComboBoxButton*>> g_comboByCmd;
thread_local std::unordered_map<UINT, CString> g_editContentsByCmd;
thread_local std::unordered_map<UINT, std::vector<CMFCToolBarEditBoxButton*>> g_editByCmd;
ComboButtonState& EnsureComboState(const CMFCToolBarComboBoxButton* pButton) {
    return g_comboStates[pButton];
}
const ComboButtonState* FindComboState(const CMFCToolBarComboBoxButton* pButton) {
    auto it = g_comboStates.find(pButton);
    return (it != g_comboStates.end()) ? &it->second : nullptr;
}
void RegisterComboButton(CMFCToolBarComboBoxButton* pButton) {
    if (!pButton || pButton->m_nID == 0) return;
    auto& buttons = g_comboByCmd[pButton->m_nID];
    for (CMFCToolBarComboBoxButton* existing : buttons) {
        if (existing == pButton) return;
    }
    buttons.push_back(pButton);
}
void UnregisterComboButton(CMFCToolBarComboBoxButton* pButton) {
    if (!pButton) return;
    g_comboStates.erase(pButton);
    for (auto it = g_comboByCmd.begin(); it != g_comboByCmd.end(); ) {
        auto& buttons = it->second;
        buttons.erase(std::remove(buttons.begin(), buttons.end(), pButton), buttons.end());
        if (buttons.empty()) {
            it = g_comboByCmd.erase(it);
        } else {
            ++it;
        }
    }
}
std::vector<CMFCToolBarComboBoxButton*> GetComboButtonsByCmd(UINT uiCmd) {
    auto it = g_comboByCmd.find(uiCmd);
    if (it == g_comboByCmd.end()) return {};
    return it->second;
}
int CompareComboText(const wchar_t* s1, const wchar_t* s2) {
    if (!s1 && !s2) return 0;
    if (!s1) return -1;
    if (!s2) return 1;
    return ::CompareStringW(LOCALE_USER_DEFAULT, NORM_IGNORECASE, s1, -1, s2, -1) - 2;
}
void RegisterEditButton(CMFCToolBarEditBoxButton* pButton) {
    if (!pButton || pButton->m_nID == 0) return;
    auto& buttons = g_editByCmd[pButton->m_nID];
    for (CMFCToolBarEditBoxButton* existing : buttons) {
        if (existing == pButton) return;
    }
    buttons.push_back(pButton);
}
void UnregisterEditButton(CMFCToolBarEditBoxButton* pButton) {
    if (!pButton) return;
    for (auto it = g_editByCmd.begin(); it != g_editByCmd.end(); ) {
        auto& buttons = it->second;
        buttons.erase(std::remove(buttons.begin(), buttons.end(), pButton), buttons.end());
        if (buttons.empty()) {
            g_editContentsByCmd.erase(it->first);
            it = g_editByCmd.erase(it);
        } else {
            ++it;
        }
    }
}
extern "C" void MS_ABI impl__SetClipboardFormatName_CMFCToolBarButton__SAXPEB_W_Z(const wchar_t* p0) {
    CMFCToolBarButton::SetClipboardFormatName(p0);
}
extern "C" void* MS_ABI impl___0CMFCToolBarMenuButton__QEAA_IPEAUHMENU____HPEB_WH_Z(void* pThis, unsigned int p0, HMENU p1, int p2, const wchar_t* p3, int p4) {
    return new(pThis) CMFCToolBarMenuButton(p0, p1, (BOOL)p2, p3, (BOOL)p4);
}
    std::unordered_map<int, StatusBarProgressState> g_statusBarProgress;
    std::mutex g_statusBarProgressMutex;
} } }  // namespace openmfc::detail::cbarcore
