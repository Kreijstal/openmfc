#pragma once
// Shared internals of the former cbarcore.cpp translation unit.
// Definitions live in detail/CbarcoreSupport.cpp, so file-scope state keeps a
// single instance across the classes this unit was split into.
namespace openmfc { namespace detail { namespace cbarcore {} } }
using namespace openmfc::detail::cbarcore;
// Toolbar / StatusBar / DialogBar Implementation
// Provides MFC wrappers around Windows common controls
//
// All exports use MSVC ABI via extern "C" + .def aliasing.

#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxmfc.h"
#include "openmfc/afxole.h"
#include "detail/DockingState.h"
#include "detail/RibbonState.h"
#include <commctrl.h>
#include <oleacc.h>
#include <shlobj.h>
#include <algorithm>
#include <climits>
#include <cstring>
#include <memory>
#include <string>
#include <mutex>
#include <new>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

// MinGW compat
#ifndef RT_TOOLBAR
#define RT_TOOLBAR MAKEINTRESOURCEW(241)
#endif
#ifndef ID_SEPARATOR
#define ID_SEPARATOR 0
#endif

namespace openmfc { namespace detail { namespace cbarcore {
using openmfc::ribbon_state::RibbonBarState;
} } }
namespace openmfc { namespace detail { namespace cbarcore {
using openmfc::ribbon_state::RibbonCategoryState;
} } }
namespace openmfc { namespace detail { namespace cbarcore {
using openmfc::ribbon_state::RibbonPanelState;
} } }
namespace openmfc { namespace detail { namespace cbarcore {
using openmfc::docking_state::PaneCoreState;
} } }

namespace openmfc { namespace detail { namespace cbarcore {
extern decltype(openmfc::ribbon_state::RibbonMutex()) g_ribbonMutex;
} } }
namespace openmfc { namespace detail { namespace cbarcore {
extern decltype(openmfc::ribbon_state::RibbonPanelStates()) g_ribbonPanels;
} } }
namespace openmfc { namespace detail { namespace cbarcore {
extern decltype(openmfc::ribbon_state::RibbonCategoryStates()) g_ribbonCategories;
} } }
namespace openmfc { namespace detail { namespace cbarcore {
extern decltype(openmfc::ribbon_state::RibbonBarStates()) g_ribbonBars;
} } }
namespace openmfc { namespace detail { namespace cbarcore {
constexpr int kApproxRibbonCharPx = 6;
} } }

namespace openmfc { namespace detail { namespace cbarcore {
extern std::unordered_map<UINT, std::wstring> g_ribbonToolTips;
} } }
namespace openmfc { namespace detail { namespace cbarcore {
extern std::unordered_map<UINT, std::wstring> g_ribbonDescriptions;
} } }
namespace openmfc { namespace detail { namespace cbarcore {
extern std::unordered_map<UINT, int> g_galleryLastSelectedByID;
} } }
namespace openmfc { namespace detail { namespace cbarcore {
extern std::unordered_map<const CMFCRibbonBaseElement*, bool> g_ribbonElementsEnabled;
} } }
namespace openmfc { namespace detail { namespace cbarcore {
extern std::unordered_map<const CMFCRibbonBaseElement*, CSize> g_ribbonTextSizeById;
} } }

namespace openmfc { namespace detail { namespace cbarcore {
struct RibbonSliderState {
    int nMin = 0;
    int nMax = 100;
    int nPos = 0;
    bool bShowZoomButtons = false;
    int nZoomIncrement = 1;
};
} } }
namespace openmfc { namespace detail { namespace cbarcore {
extern std::unordered_map<UINT, RibbonSliderState> g_sliderStates;
} } }

namespace openmfc { namespace detail { namespace cbarcore {
struct RibbonProgressState {
    int nMin = 0;
    int nMax = 100;
    int nPos = 0;
    bool bInfinite = false;
};
} } }
namespace openmfc { namespace detail { namespace cbarcore {
extern std::unordered_map<UINT, RibbonProgressState> g_progressStates;
} } }

namespace openmfc { namespace detail { namespace cbarcore {
struct RibbonStatusPaneState {
    bool bAnimating = false;
    UINT nAnimationFrame = 0;
    UINT nAnimationCount = 0;
    DWORD dwAnimationDelay = 0;
    HBITMAP hAnimationBitmap = nullptr;
    int cxAnimation = 0;
};
} } }
namespace openmfc { namespace detail { namespace cbarcore {
extern std::unordered_map<UINT, RibbonStatusPaneState> g_statusPaneStates;
} } }

namespace openmfc { namespace detail { namespace cbarcore {
struct ThreadSlotDataState {
    int nextSlot = 0;
    std::unordered_set<int> freeSlots;
    std::unordered_map<DWORD, std::unordered_map<int, void*>> threadValues;
};
} } }
namespace openmfc { namespace detail { namespace cbarcore {
extern std::mutex g_threadSlotMutex;
} } }
namespace openmfc { namespace detail { namespace cbarcore {
extern std::unordered_map<void*, ThreadSlotDataState> g_threadSlotStates;
} } }

namespace openmfc { namespace detail { namespace cbarcore {
struct TypeLibCacheState {
    bool locked = false;
    std::unordered_map<unsigned long, ITypeLib*> typeLibs;
    std::unordered_map<std::wstring, ITypeInfo*> typeInfos;
};
} } }
namespace openmfc { namespace detail { namespace cbarcore {
struct TypeLibCacheHandle {};
} } }
namespace openmfc { namespace detail { namespace cbarcore {
extern std::mutex g_typeLibCacheMutex;
} } }
namespace openmfc { namespace detail { namespace cbarcore {
extern std::unordered_map<void*, TypeLibCacheState> g_typeLibCaches;
} } }
namespace openmfc { namespace detail { namespace cbarcore {
extern std::unordered_map<const CCmdTarget*, std::unique_ptr<TypeLibCacheHandle>> g_targetTypeLibCaches;
} } }
namespace openmfc { namespace detail { namespace cbarcore {
extern std::unordered_map<std::wstring, std::unique_ptr<TypeLibCacheHandle>> g_guidTypeLibCaches;
} } }

namespace openmfc { namespace detail { namespace cbarcore {
struct UserToolState {
    std::wstring command;
    HICON icon = nullptr;
};
} } }
namespace openmfc { namespace detail { namespace cbarcore {
struct UserToolsManagerState {
    std::vector<void*> tools;
};
} } }

namespace openmfc { namespace detail { namespace cbarcore {
struct TaskDialogButtonState {
    std::vector<std::wstring> labels;
    std::vector<TASKDIALOG_BUTTON> buttons;
    std::unordered_set<int> disabled;
    int defaultButton = 0;
};
} } }

namespace openmfc { namespace detail { namespace cbarcore {
struct SplitterDim {
    int ideal = 0;
    int min = 0;
};
} } }
namespace openmfc { namespace detail { namespace cbarcore {
struct SplitterLayoutState {
    std::vector<std::vector<CWnd*>> panes;
    std::vector<SplitterDim> rowInfo;
    std::vector<SplitterDim> colInfo;
};
} } }

namespace openmfc { namespace detail { namespace cbarcore {
extern std::mutex g_splitterWndMutex;
} } }
namespace openmfc { namespace detail { namespace cbarcore {
extern std::unordered_map<const CSplitterWnd*, SplitterLayoutState> g_splitterWndStates;
} } }

namespace openmfc { namespace detail { namespace cbarcore {
SplitterLayoutState& EnsureSplitterState(const CSplitterWnd* pSplitter);
} } }

namespace openmfc { namespace detail { namespace cbarcore {
CWnd* GetSplitterPane(const CSplitterWnd* pSplitter, int row, int col);
} } }

namespace openmfc { namespace detail { namespace cbarcore {
void NormalizeSplitterState(CSplitterWnd* pThis);
} } }

namespace openmfc { namespace detail { namespace cbarcore {
std::vector<int> SplitterSizesFromInfo(int total, const std::vector<SplitterDim>& info, int fallbackMin);
} } }

namespace openmfc { namespace detail { namespace cbarcore {
struct TaskDialogState {
    TaskDialogButtonState commandControls;
    TaskDialogButtonState radioButtons;
    int progressState = 0;
};
} } }

namespace openmfc { namespace detail { namespace cbarcore {
struct TabbedPaneState {
    std::vector<void*> tabs;
    CRect tabArea = CRect(0, 0, 0, 0);
};
} } }

namespace openmfc { namespace detail { namespace cbarcore {
struct GenericFeaturePackObject : CObject {
    GenericFeaturePackObject() = default;
};
} } }

namespace openmfc { namespace detail { namespace cbarcore {
extern std::mutex g_userToolMutex;
} } }
namespace openmfc { namespace detail { namespace cbarcore {
extern std::unordered_map<void*, UserToolState> g_userTools;
} } }
namespace openmfc { namespace detail { namespace cbarcore {
extern std::unordered_map<void*, UserToolsManagerState> g_userToolManagers;
} } }
namespace openmfc { namespace detail { namespace cbarcore {
extern std::mutex g_taskDialogMutex;
} } }
namespace openmfc { namespace detail { namespace cbarcore {
extern std::unordered_map<CTaskDialog*, TaskDialogState> g_taskDialogs;
} } }
namespace openmfc { namespace detail { namespace cbarcore {
extern std::mutex g_tabbedPaneMutex;
} } }
namespace openmfc { namespace detail { namespace cbarcore {
extern std::unordered_map<void*, TabbedPaneState> g_tabbedPanes;
} } }
namespace openmfc { namespace detail { namespace cbarcore {
extern std::unordered_map<void*, HDC> g_windowlessDCs;
} } }
namespace openmfc { namespace detail { namespace cbarcore {
extern std::unordered_set<void*> g_d2dInitialized;
} } }
namespace openmfc { namespace detail { namespace cbarcore {
struct _AFX_D2D_STATE {
    int d2dFactoryType = 0;
    int dWriteFactoryType = 0;
    bool initialized = false;
};
} } }
namespace openmfc { namespace detail { namespace cbarcore {
extern thread_local _AFX_D2D_STATE g_d2dState;
} } }

namespace openmfc { namespace detail { namespace cbarcore {
extern __attribute__((used)) CRuntimeClass g_classCUserException;
} } }





namespace openmfc { namespace detail { namespace cbarcore {
void AddTaskDialogButton(TaskDialogButtonState& state, int id, const wchar_t* label, int enabled);
} } }

namespace openmfc { namespace detail { namespace cbarcore {
TASKDIALOG_BUTTON* FirstTaskDialogButton(TaskDialogButtonState& state);
} } }

namespace openmfc { namespace detail { namespace cbarcore {
ptrdiff_t FindTaskDialogButtonIndex(const TaskDialogButtonState& state, int id);
} } }

namespace openmfc { namespace detail { namespace cbarcore {
int CommonButtonId(int commonButtonFlag);
} } }

namespace openmfc { namespace detail { namespace cbarcore {
int CommonButtonFlagByIndex(int commonButtons, int index);
} } }

namespace openmfc { namespace detail { namespace cbarcore {
void FillTaskDialogConfig(CTaskDialog* dialog, TASKDIALOGCONFIG& config);
} } }

namespace openmfc { namespace detail { namespace cbarcore {
inline UINT RibbonElementID(const CMFCRibbonBaseElement* pElem) {
    return (pElem != nullptr) ? pElem->GetID() : 0;
}
} } }

namespace openmfc { namespace detail { namespace cbarcore {
inline void NormalizeRange(int nMin, int nMax, int& outMin, int& outMax) {
    outMin = std::min(nMin, nMax);
    outMax = std::max(nMin, nMax);
}
} } }

namespace openmfc { namespace detail { namespace cbarcore {
inline int RibbonTextPixels(const CMFCRibbonBaseElement* pElem) {
    if (!pElem) return 0;
    return std::max(0, pElem->GetText().GetLength()) * kApproxRibbonCharPx;
}
} } }

namespace openmfc { namespace detail { namespace cbarcore {
inline CSize MeasureRibbonText(const CMFCRibbonBaseElement* pElem, CDC* pDC) {
    if (!pElem) return CSize(0, 0);
    CString text = pElem->GetText();
    const int len = text.GetLength();
    if (len <= 0) return CSize(0, 16);
    HDC hdc = pDC ? pDC->GetSafeHdc() : nullptr;
    if (hdc) {
        SIZE ext{};
        if (::GetTextExtentPoint32W(hdc, (const wchar_t*)text, len, &ext)) {
            return CSize(ext.cx + 6, std::max(16, static_cast<int>(ext.cy)));
        }
    }
    return CSize(RibbonTextPixels(pElem), 16);
}
} } }

namespace openmfc { namespace detail { namespace cbarcore {
inline void BuildCStringResult(void* pRet, const std::wstring& value) {
    if (!pRet) return;
    new(pRet) CString(value.c_str());
}
} } }

namespace openmfc { namespace detail { namespace cbarcore {
inline void BuildCSizeResult(void* pRet, int cx, int cy) {
    if (!pRet) return;
    new(pRet) CSize(cx, cy);
}
} } }

namespace openmfc { namespace detail { namespace cbarcore {
inline CSize ToolbarDefaultSize(const CToolBar* pBar, int nButtons = -1) {
    const int count = nButtons >= 0 ? nButtons : (pBar ? std::max(0, pBar->m_nCount) : 0);
    const CSize button = pBar ? pBar->GetButtonSize() : CSize(23, 22);
    return CSize(button.cx * std::max(1, count), button.cy);
}
} } }

namespace openmfc { namespace detail { namespace cbarcore {
inline std::wstring TypeInfoCacheKey(unsigned long lcid, const GUID& iid) {
    wchar_t guidText[64] = {};
    ::StringFromGUID2(iid, guidText, 64);
    std::wstring key = std::to_wstring(lcid);
    key.push_back(L':');
    key += guidText;
    return key;
}
} } }


namespace openmfc { namespace detail { namespace cbarcore {
void* GetGuidTypeLibCache(const GUID& guid);
} } }


namespace openmfc { namespace detail { namespace cbarcore {
inline void ReleaseTypeLibCache(TypeLibCacheState& state) {
    for (auto& entry : state.typeLibs) {
        if (entry.second) entry.second->Release();
    }
    for (auto& entry : state.typeInfos) {
        if (entry.second) entry.second->Release();
    }
    state.typeLibs.clear();
    state.typeInfos.clear();
}
} } }

namespace openmfc { namespace detail { namespace cbarcore {
inline COLORREF SystemColor(int index) {
    return ::GetSysColor(index);
}
} } }

namespace openmfc { namespace detail { namespace cbarcore {
inline COLORREF FeaturePackBackgroundColor() {
    CMFCVisualManager* manager = CMFCVisualManager::GetInstance();
    if (manager != nullptr) {
        return manager->GetHighlightedColor(COLOR_3DFACE);
    }
    return SystemColor(COLOR_3DFACE);
}
} } }






























//=============================================================================
// CToolTipCtrl
//=============================================================================






//=============================================================================
// CToolBar
//=============================================================================































//=============================================================================
// CThreadSlotData and toolbar ABI residuals
//=============================================================================


































extern "C" int MS_ABI impl__CreateEx_CToolBarCtrl__QEAAHKKAEBUtagRECT__PEAVCWnd__I_Z(
    CWnd* pThis, unsigned long dwExStyle, unsigned long dwStyle, const RECT* rect, CWnd* pParentWnd, unsigned int nID);











































//=============================================================================
// CStatusBar
//=============================================================================

























//=============================================================================
// CDialogBar
//=============================================================================








//=============================================================================
// CSplitterWnd
//=============================================================================






















//=============================================================================
// CTaskDialog
//=============================================================================


















// Ribbon element wrappers (Wave 2 retry)
//=============================================================================


















































//=============================================================================
// CToolBar / CStatusBar ABI wrappers
// These provide proper // Symbol: exclusions for CString-returning methods.
//=============================================================================














//=============================================================================
// CStatusBarCtrl real C++ methods + export wrappers
//=============================================================================







namespace openmfc { namespace detail { namespace cbarcore {
struct DockSiteCoreState {
    CWnd* parent = nullptr;
    CRect recentRect = CRect(0, 0, 0, 0);
    int rowCount = 0;
    unsigned int layoutRevision = 0;
};
} } }

namespace openmfc { namespace detail { namespace cbarcore {
extern decltype(openmfc::docking_state::PaneCoreStateMutex()) g_paneCoreStateMutex_Cbarcore;
} } }
namespace openmfc { namespace detail { namespace cbarcore {
extern decltype(openmfc::docking_state::PaneCoreStates()) g_paneCoreState_Cbarcore;
} } }
namespace openmfc { namespace detail { namespace cbarcore {
extern std::unordered_map<const void*, DockSiteCoreState> g_dockSiteCoreState;
} } }

namespace openmfc { namespace detail { namespace cbarcore {
CRect NormalizeRect(const RECT* rect, int fallbackWidth = 200, int fallbackHeight = 120);
} } }

namespace openmfc { namespace detail { namespace cbarcore {
CRect NormalizeRect(const CRect* rect, int fallbackWidth = 200, int fallbackHeight = 120);
} } }

namespace openmfc { namespace detail { namespace cbarcore {
BOOL TryCreatePaneWindow(CWnd* paneWnd, const wchar_t* className, DWORD style, const CRect& rect, CWnd* parentWnd, UINT id, CCreateContext* context);
} } }


























static constexpr unsigned int kNoSerializationSchema = 0xFFFF;

// CDockSite: DECLARE_DYNAMIC, real MSVC sizeof=544, base CBasePane (harvested from
// mfc140u.dll). Chain via m_pfnGetBaseClass to CBasePane's exported getter so the
// IsKindOf chain reaches CObject correctly (was sizeof 0 / base CObject — wrong).
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CBasePane__SAPEAUCRuntimeClass__XZ();
namespace openmfc { namespace detail { namespace cbarcore {
CRuntimeClass* AFXAPI gb_CDockSite();
} } }

namespace openmfc { namespace detail { namespace cbarcore {
extern CRuntimeClass g_runtimeClassDockSite;
} } }


















namespace openmfc { namespace detail { namespace cbarcore {
class CFrameImpl;
} } }
namespace openmfc { namespace detail { namespace cbarcore {
struct FrameImplState {
    CFrameWnd* ownerFrame = nullptr;
    void* menuBar = nullptr;
    CRuntimeClass* visualManagerClass = nullptr;
};
} } }

namespace openmfc { namespace detail { namespace cbarcore {
extern std::mutex g_featurePackStateMutex;
} } }
namespace openmfc { namespace detail { namespace cbarcore {
extern std::unordered_map<CFrameImpl*, FrameImplState> g_frameImplStates;
} } }
namespace openmfc { namespace detail { namespace cbarcore {
extern std::unordered_map<CMDIChildWndEx*, std::wstring> g_taskbarTabText;
} } }

namespace openmfc { namespace detail { namespace cbarcore {
extern CRuntimeClass g_cmdiClientAreaRuntimeClass;
} } }













































namespace openmfc { namespace detail { namespace cbarcore {
extern std::mutex g_statusBarTextMutex;
} } }
namespace openmfc { namespace detail { namespace cbarcore {
extern std::unordered_map<const CMFCStatusBar*, std::unordered_map<int, std::wstring>> g_statusBarPaneText;
} } }

namespace openmfc { namespace detail { namespace cbarcore {
extern std::mutex g_captionBarTextMutex;
} } }
namespace openmfc { namespace detail { namespace cbarcore {
struct CaptionBarState {
    std::wstring text;
    int alignment = 0;
    int layoutVersion = 0;
};
} } }
namespace openmfc { namespace detail { namespace cbarcore {
extern std::unordered_map<const CMFCCaptionBar*, CaptionBarState> g_captionBarText;
} } }

namespace openmfc { namespace detail { namespace cbarcore {
CString GetStoredPaneText(const CMFCStatusBar* pThis, int nIndex);
} } }







namespace openmfc { namespace detail { namespace cbarcore {
struct PaneState {
    std::vector<void*> panes;
    CWnd* parent = nullptr;
    CRect recentRect = CRect(0, 0, 0, 0);
    CSize minSize = CSize(0, 0);
    BOOL rollUp = FALSE;
};
} } }

namespace openmfc { namespace detail { namespace cbarcore {
struct AutoHideButtonState {
    BOOL highlighted;
    BOOL horizontal;
};
} } }

namespace openmfc { namespace detail { namespace cbarcore {
extern std::mutex g_wave2Mutex;
} } }
namespace openmfc { namespace detail { namespace cbarcore {
static constexpr int kPaneFrameDefaultWidth = 240;
} } }
namespace openmfc { namespace detail { namespace cbarcore {
static constexpr int kPaneFrameDefaultHeight = 140;
} } }
namespace openmfc { namespace detail { namespace cbarcore {
static constexpr int kPaneFrameDefaultMinWidth = 120;
} } }
namespace openmfc { namespace detail { namespace cbarcore {
static constexpr int kPaneFrameDefaultMinHeight = 80;
} } }
namespace openmfc { namespace detail { namespace cbarcore {
static constexpr int kPaneFrameCaptionHeight = 22;
} } }
namespace openmfc { namespace detail { namespace cbarcore {
static constexpr int kPaneFrameThinBorder = 1;
} } }
namespace openmfc { namespace detail { namespace cbarcore {
static constexpr int kPaneFrameBorder = 2;
} } }
namespace openmfc { namespace detail { namespace cbarcore {
static constexpr int kPaneFrameWideBorder = 4;
} } }
namespace openmfc { namespace detail { namespace cbarcore {
static constexpr int kPaneDividerThickness = 8;
} } }
namespace openmfc { namespace detail { namespace cbarcore {
static constexpr int kPaneDividerUnitWidth = 120;
} } }
namespace openmfc { namespace detail { namespace cbarcore {
extern std::unordered_map<void*, PaneState> g_framePanes;
} } }
namespace openmfc { namespace detail { namespace cbarcore {
extern std::unordered_map<void*, PaneState> g_multiFramePanes;
} } }
namespace openmfc { namespace detail { namespace cbarcore {
extern std::unordered_map<void*, PaneState> g_containerPanes;
} } }
namespace openmfc { namespace detail { namespace cbarcore {
extern std::unordered_map<void*, PaneState> g_containerManagerPanes;
} } }
namespace openmfc { namespace detail { namespace cbarcore {
extern std::unordered_map<void*, PaneState> g_dividerPanes;
} } }
namespace openmfc { namespace detail { namespace cbarcore {
extern std::unordered_map<void*, PaneState> g_autoHideBarWindows;
} } }
namespace openmfc { namespace detail { namespace cbarcore {
extern std::unordered_map<void*, void*> g_paneToContainer;
} } }
namespace openmfc { namespace detail { namespace cbarcore {
extern std::unordered_map<void*, void*> g_paneToAutoHideButton;
} } }
namespace openmfc { namespace detail { namespace cbarcore {
extern std::unordered_map<void*, std::unique_ptr<int>> g_autoHideButtonStorage;
} } }
namespace openmfc { namespace detail { namespace cbarcore {
extern std::unordered_map<void*, AutoHideButtonState> g_autoHideButtons;
} } }

namespace openmfc { namespace detail { namespace cbarcore {
void AddUniquePane(PaneState& state, void* pane);
} } }

namespace openmfc { namespace detail { namespace cbarcore {
void RemovePane(PaneState& state, void* pane);
} } }

namespace openmfc { namespace detail { namespace cbarcore {
void* FirstPane(const PaneState& state);
} } }



























































































































































//=============================================================================
// CMFCToolBarButton - static members and additional methods
//=============================================================================

namespace openmfc { namespace detail { namespace cbarcore {
struct ComboButtonState {
    std::vector<std::pair<CString, DWORD_PTR>> items;
    int curSel = -1;
};
} } }

namespace openmfc { namespace detail { namespace cbarcore {
extern thread_local std::unordered_map<const CMFCToolBarComboBoxButton*, ComboButtonState> g_comboStates;
} } }
namespace openmfc { namespace detail { namespace cbarcore {
extern thread_local std::unordered_map<UINT, std::vector<CMFCToolBarComboBoxButton*>> g_comboByCmd;
} } }

namespace openmfc { namespace detail { namespace cbarcore {
extern thread_local std::unordered_map<UINT, CString> g_editContentsByCmd;
} } }
namespace openmfc { namespace detail { namespace cbarcore {
extern thread_local std::unordered_map<UINT, std::vector<CMFCToolBarEditBoxButton*>> g_editByCmd;
} } }

namespace openmfc { namespace detail { namespace cbarcore {
ComboButtonState& EnsureComboState(const CMFCToolBarComboBoxButton* pButton);
} } }

namespace openmfc { namespace detail { namespace cbarcore {
const ComboButtonState* FindComboState(const CMFCToolBarComboBoxButton* pButton);
} } }

namespace openmfc { namespace detail { namespace cbarcore {
void RegisterComboButton(CMFCToolBarComboBoxButton* pButton);
} } }

namespace openmfc { namespace detail { namespace cbarcore {
void UnregisterComboButton(CMFCToolBarComboBoxButton* pButton);
} } }

namespace openmfc { namespace detail { namespace cbarcore {
std::vector<CMFCToolBarComboBoxButton*> GetComboButtonsByCmd(UINT uiCmd);
} } }

namespace openmfc { namespace detail { namespace cbarcore {
int CompareComboText(const wchar_t* s1, const wchar_t* s2);
} } }

namespace openmfc { namespace detail { namespace cbarcore {
void RegisterEditButton(CMFCToolBarEditBoxButton* pButton);
} } }

namespace openmfc { namespace detail { namespace cbarcore {
void UnregisterEditButton(CMFCToolBarEditBoxButton* pButton);
} } }

void OpenMFC_CopyComboButtonState(CMFCToolBarComboBoxButton* dst, const CMFCToolBarComboBoxButton* src);

void OpenMFC_CopyEditButtonState(CMFCToolBarEditBoxButton* dst, const CMFCToolBarEditBoxButton* src);

// Static members - provided without // Symbol: comments so typed_stubs handles
// the MSVC-named exports. Definitions needed for C++ ODR completeness.


namespace openmfc { namespace detail { namespace cbarcore {
extern "C" void MS_ABI impl__SetClipboardFormatName_CMFCToolBarButton__SAXPEB_W_Z(const wchar_t* p0);
} } }

//=============================================================================
// CMFCToolBarComboBoxButton
//=============================================================================

























// --- extern "C" MS_ABI thunks for CMFCToolBarComboBoxButton ---
























//=============================================================================
// CMFCToolBarEditBoxButton
//=============================================================================






// --- extern "C" MS_ABI thunks for CMFCToolBarEditBoxButton ---





//=============================================================================
// CMFCToolBarMenuButton - parameterized constructor + Initialize
//=============================================================================



namespace openmfc { namespace detail { namespace cbarcore {
extern "C" void* MS_ABI impl___0CMFCToolBarMenuButton__QEAA_IPEAUHMENU____HPEB_WH_Z(void* pThis, unsigned int p0, HMENU p1, int p2, const wchar_t* p3, int p4);
} } }


//=== CMFCRibbonProgressBar remaining exports ==================================





//=== CMFCStatusBar progress exports ===========================================

namespace openmfc { namespace detail { namespace cbarcore {
    struct StatusBarProgressState {
        long nTotal = 100;
        int nMax = 100;
        long nProgress = 0;
        unsigned long clrBar = 0;
        unsigned long clrProgressBarDest = 0;
        unsigned long clrProgressText = 0;
        bool bEnabled = false;
    };
} } }
namespace openmfc { namespace detail { namespace cbarcore {
extern std::unordered_map<int, StatusBarProgressState> g_statusBarProgress;
} } }
namespace openmfc { namespace detail { namespace cbarcore {
extern std::mutex g_statusBarProgressMutex;
} } }



