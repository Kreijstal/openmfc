#include "WincoreSupport.h"
#include "detail/WindowHandleMapSupport.h"

// Implementations this unit calls that are defined with their own class.
extern "C" LRESULT MS_ABI impl__AfxCallWndProc__YA_JPEAVCWnd__PEAUHWND____I_K_J_Z(
    CWnd* pThis, HWND, UINT message, WPARAM wParam, LPARAM lParam);

extern "C" void OpenMfcResetDockState(void* pState) {
    if (!pState) {
        return;
    }
    std::lock_guard<std::mutex> lk(g_wndStateMutex);
    g_dockStateSnapshots.erase(static_cast<const CDockState*>(pState));
}
IMPLEMENT_DYNAMIC(CWnd, CCmdTarget)
IMPLEMENT_DYNCREATE(CFrameWnd, CWnd)
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
std::map<const CWnd*, COleControlContainer*> g_controlContainerMap;
std::map<CWnd*, CWndRuntimeState> g_wndRuntimeStates;
std::map<CFrameWnd*, CFrameWndRuntimeState> g_frameWndRuntimeStates;
std::map<const CDockState*, std::vector<DockBarSnapshot>> g_dockStateSnapshots;
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
    // Resource destructors and COM releases may call back into window APIs.
    pWnd->ReleaseAccessibility();
    pWnd->EnableD2DSupport(FALSE);
    std::lock_guard<std::mutex> lk(g_wndStateMutex);
    g_wndRuntimeStates.erase(pWnd);
    if (auto* pFrameWnd = dynamic_cast<CFrameWnd*>(pWnd)) g_frameWndRuntimeStates.erase(pFrameWnd);
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

CWnd* OpenMfcAttachCWnd(HWND handle) { return CWnd::FromHandle(handle); }
void OpenMfcCleanupTempWrappers() { CWnd::DeleteTempMap(); }
namespace openmfc { namespace detail { namespace wincore {
CWnd* OpenMfcLookupCWnd(HWND handle) { return static_cast<CWnd*>(CWnd::FromHandlePermanent(handle)); }
void OpenMfcDetachCWnd(HWND handle) {
    if(auto* window=OpenMfcLookupCWnd(handle))window->Detach();
}
} } }
