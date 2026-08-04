#include "WincoreSupport.h"

// Implementations this unit calls that are defined with their own class.
extern "C" LRESULT MS_ABI impl__WindowProc_CWnd__MEAA_JI_K_J_Z(
    CWnd* pThis, UINT message, WPARAM wParam, LPARAM lParam);

extern "C" void OpenMfcResetDockState(void* pState) {
    if (!pState) {
        return;
    }
    std::lock_guard<std::mutex> lk(g_wndStateMutex);
    g_dockStateSnapshots.erase(static_cast<const CDockState*>(pState));
}
CWnd* OpenMfcAttachCWnd(HWND hWnd) {
    if (!hWnd) {
        return nullptr;
    }
    if (CWnd* existing = OpenMfcLookupCWnd(hWnd)) {
        return existing;
    }

    // Create a temporary wrapper - tracked for cleanup on window destruction
    CWnd* wrapper = new CWnd();
    wrapper->m_hWnd = hWnd;
    g_hwndMap[hWnd] = wrapper;
    g_tempWrappers.insert(wrapper);  // Track for deletion
    return wrapper;
}
void OpenMfcCleanupTempWrappers() {
    // Build list of stale entries (can't modify map while iterating)
    std::vector<HWND> staleHandles;
    for (auto& pair : g_hwndMap) {
        // Check if the window still exists
        if (!::IsWindow(pair.first)) {
            staleHandles.push_back(pair.first);
        }
    }

    // Clean up stale entries
    for (HWND hWnd : staleHandles) {
        OpenMfcDetachCWnd(hWnd);
    }
}
static_assert(sizeof(StaticCWndExport) == sizeof(CWnd), "static CWnd export must match CWnd size");
static_assert(offsetof(StaticCWndExport, hWnd) == 64, "static CWnd export must place HWND at CWnd::m_hWnd");
IMPLEMENT_DYNAMIC(CWnd, CCmdTarget)
IMPLEMENT_DYNCREATE(CFrameWnd, CWnd)
LRESULT CALLBACK AfxWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    CWnd* pWnd = nullptr;

    // On WM_NCCREATE, store the CWnd* pointer
    if (message == WM_NCCREATE) {
        CREATESTRUCTW* pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
        pWnd = reinterpret_cast<CWnd*>(pCreate->lpCreateParams);
        if (pWnd) {
            pWnd->m_hWnd = hWnd;
            g_hwndMap[hWnd] = pWnd;
        }
    } else {
        // Look up CWnd from HWND
        auto it = g_hwndMap.find(hWnd);
        if (it != g_hwndMap.end()) {
            pWnd = it->second;
        }
    }

    // Route to CWnd::WindowProc if we have a CWnd
    if (pWnd) {
        LRESULT result = impl__WindowProc_CWnd__MEAA_JI_K_J_Z(pWnd, message, wParam, lParam);

        // WM_NCDESTROY is the final message - clean up the wrapper
        // This must come after WindowProc since the window is still valid during the call.
        if (message == WM_NCDESTROY) {
            OpenMfcDetachCWnd(hWnd);
        }

        return result;
    }

    return DefWindowProcW(hWnd, message, wParam, lParam);
}
ATOM RegisterOpenMFCClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex = {};
    wcex.cbSize = sizeof(WNDCLASSEXW);
    wcex.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
    wcex.lpfnWndProc = AfxWndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = nullptr;
    wcex.lpszClassName = g_szOpenMFCClass;
    wcex.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);

    return RegisterClassExW(&wcex);
}
IMPLEMENT_DYNCREATE(CMDIFrameWnd, CFrameWnd)
IMPLEMENT_DYNCREATE(CMDIChildWnd, CFrameWnd)
namespace openmfc { namespace detail { namespace wincore {
std::map<HWND, CWnd*> g_hwndMap;
std::map<const CWnd*, COleControlContainer*> g_controlContainerMap;
std::map<CWnd*, CWndRuntimeState> g_wndRuntimeStates;
std::map<CFrameWnd*, CFrameWndRuntimeState> g_frameWndRuntimeStates;
std::map<const CDockState*, std::vector<DockBarSnapshot>> g_dockStateSnapshots;
std::set<CWnd*> g_tempWrappers;
std::mutex g_wndStateMutex;
std::map<std::wstring, std::vector<int>> g_barStateStore;
CWndRuntimeState& GetWindowRuntimeState(CWnd* pWnd) {
    std::lock_guard<std::mutex> lk(g_wndStateMutex);
    return g_wndRuntimeStates[pWnd];
}
CFrameWndRuntimeState& GetFrameRuntimeState(CFrameWnd* pWnd) {
    std::lock_guard<std::mutex> lk(g_wndStateMutex);
    return g_frameWndRuntimeStates[pWnd];
}
void CleanupWindowRuntimeState(CWnd* pWnd) {
    if (!pWnd) {
        return;
    }
    std::lock_guard<std::mutex> lk(g_wndStateMutex);
    auto stateIt = g_wndRuntimeStates.find(pWnd);
    if (stateIt != g_wndRuntimeStates.end()) {
        auto it = stateIt->second.properties.find({0x4F44, 0});
        if (it != stateIt->second.properties.end()) {
            delete static_cast<CDCRenderTarget*>(it->second);
            stateIt->second.properties.erase(it);
        }
        g_wndRuntimeStates.erase(stateIt);
    }
    if (auto* pFrameWnd = dynamic_cast<CFrameWnd*>(pWnd)) {
        g_frameWndRuntimeStates.erase(pFrameWnd);
    }
}
ITaskbarList3* AcquireTaskbarList3ForWindow(HWND hWnd) {
    if (!hWnd) {
        return nullptr;
    }

    ITaskbarList3* taskbarList = nullptr;
    HRESULT hr = ::CoCreateInstance(
        CLSID_TaskbarList,
        nullptr,
        CLSCTX_INPROC_SERVER,
        IID_ITaskbarList3,
        reinterpret_cast<void**>(&taskbarList));

    if (FAILED(hr) || !taskbarList) {
        return nullptr;
    }

    taskbarList->HrInit();
    return taskbarList;
}
CWnd* OpenMfcLookupCWnd(HWND hWnd) {
    if (!hWnd) {
        return nullptr;
    }
    auto it = g_hwndMap.find(hWnd);
    if (it != g_hwndMap.end()) {
        return it->second;
    }
    return nullptr;
}
void OpenMfcDetachCWnd(HWND hWnd) {
    auto it = g_hwndMap.find(hWnd);
    if (it != g_hwndMap.end()) {
        CWnd* pWnd = it->second;
        g_hwndMap.erase(it);
        g_wndRuntimeStates.erase(pWnd);
        if (auto* pFrameWnd = dynamic_cast<CFrameWnd*>(pWnd)) {
            g_frameWndRuntimeStates.erase(pFrameWnd);
        }

        auto ccIt = g_controlContainerMap.find(pWnd);
        if (ccIt != g_controlContainerMap.end()) {
            delete ccIt->second;
            g_controlContainerMap.erase(ccIt);
        }

        // Delete if this was a temporary wrapper allocated by OpenMfcAttachCWnd
        auto tempIt = g_tempWrappers.find(pWnd);
        if (tempIt != g_tempWrappers.end()) {
            g_tempWrappers.erase(tempIt);
            pWnd->m_hWnd = nullptr;  // Clear before delete to prevent double-detach
            delete pWnd;
        }
    }
}
void CWnd_PreSubclassWindowCompat(CWnd* pThis) {
    if (!pThis) {
        return;
    }
    auto& state = GetWindowRuntimeState(pThis);
    state.properties[{0x5053, 0}] = reinterpret_cast<void*>(1);
    if (pThis->m_hWnd) {
        WNDPROC current = reinterpret_cast<WNDPROC>(::GetWindowLongPtrW(pThis->m_hWnd, GWLP_WNDPROC));
        if (current) {
            state.properties[{0x5054, 0}] = reinterpret_cast<void*>(current);
        }
    }
}
void CWnd_SetMessageText(CWnd* pThis, const wchar_t* lpszText) {
    const wchar_t* text = lpszText ? lpszText : L"";
    CWnd* pTop = pThis ? pThis->GetTopLevelFrame() : nullptr;
    auto* pFrame = pTop ? dynamic_cast<CFrameWnd*>(pTop) : nullptr;
    if (pFrame) {
        pFrame->SetMessageText(text);
    }
}
ATOM g_atomOpenMFCClass = 0;
extern "C" CWnd* MS_ABI impl__FromHandle_CWnd__SAPAV1_PAUHWND_____Z(HWND hWnd) {
    return OpenMfcAttachCWnd(hWnd);
}
extern "C" void MS_ABI impl__UpdateWindow_CWnd__QEAAXXZ(CWnd* pThis) {
    if (pThis && pThis->m_hWnd) {
        ::UpdateWindow(pThis->m_hWnd);
    }
}
extern "C" HWND MS_ABI impl__GetSafeHwnd_CWnd__QEBAPEAUHWND____XZ(const CWnd* pThis) {
    return pThis ? pThis->m_hWnd : nullptr;
}
const AFX_MSGMAP_ENTRY g_cwndEmptyMsgEntries[] = { {0,0,0,0, AfxSig_end, (AFX_PMSG)0} };
const AFX_MSGMAP* AFXAPI gbm_CWnd_base() { return impl__GetThisMessageMap_CCmdTarget__KAPEBUAFX_MSGMAP__XZ(); }
const AFX_MSGMAP g_cwndMessageMap = { gbm_CWnd_base, g_cwndEmptyMsgEntries };
int CWnd_PreTranslateMessageCompat(CWnd* pThis, MSG* pMsg) {
    if (!pThis || !pMsg || !pThis->m_hWnd) {
        return FALSE;
    }
    if (!::IsWindow(pThis->m_hWnd)) {
        return FALSE;
    }
    return ::IsDialogMessageW(pThis->m_hWnd, pMsg) ? TRUE : FALSE;
}
} } }  // namespace openmfc::detail::wincore
