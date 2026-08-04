// CFrameWnd — OpenMFC implementation.
// Sources: frame_font_exports.cpp, wincore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/FrameFontExportsSupport.h"
#include "detail/WincoreSupport.h"

// Symbol: ?CreateObject@CFrameWnd@@SAPEAVCObject@@XZ
extern "C" CObject* MS_ABI impl__CreateObject_CFrameWnd__SAPEAVCObject__XZ() {
    return CFrameWnd::CreateObject();
}
// Symbol: ?GetMessageMap@CFrameWnd@@MEBAPEBUAFX_MSGMAP@@XZ
extern "C" const AFX_MSGMAP* MS_ABI impl__GetMessageMap_CFrameWnd__MEBAPEBUAFX_MSGMAP__XZ(
    const CFrameWnd* pThis) {
    (void)pThis;
    return EmptyMessageMap_FrameFontExports();
}
// Symbol: ?GetRuntimeClass@CFrameWnd@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CFrameWnd__UEBAPEAUCRuntimeClass__XZ(
    const CFrameWnd* pThis) {
    return CFrameWnd::GetThisClass();
}
// Symbol: ?GetThisMessageMap@CFrameWnd@@KAPEBUAFX_MSGMAP@@XZ
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CFrameWnd__KAPEBUAFX_MSGMAP__XZ() {
    return EmptyMessageMap_FrameFontExports();
}
// Symbol: ?OnCmdMsg@CFrameWnd@@UEAAHIHPEAXPEAUAFX_CMDHANDLERINFO@@@Z
// CFrameWnd::OnCmdMsg
extern "C" int MS_ABI impl__OnCmdMsg_CFrameWnd__UEAAHIHPEAXPEAUAFX_CMDHANDLERINFO___Z(
    CFrameWnd* pThis, UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) {
    return pThis ? pThis->OnCmdMsg(nID, nCode, pExtra, pHandlerInfo) : FALSE;
}
// Symbol: ?OnCommand@CFrameWnd@@MEAAH_K_J@Z
// CFrameWnd::OnCommand
extern "C" int MS_ABI impl__OnCommand_CFrameWnd__MEAAH_K_J_Z(CFrameWnd* pThis, WPARAM wParam, LPARAM lParam) {
    return pThis ? pThis->OnCommand(wParam, lParam) : FALSE;
}
// Symbol: ?PreTranslateMessage@CFrameWnd@@UEAAHPEAUtagMSG@@@Z
// CFrameWnd::PreTranslateMessage
extern "C" int MS_ABI impl__PreTranslateMessage_CFrameWnd__UEAAHPEAUtagMSG___Z(CFrameWnd* pThis, MSG* pMsg) {
    return pThis ? pThis->PreTranslateMessage(pMsg) : FALSE;
}
// Symbol: ?rectDefault@CFrameWnd@@2VCRect@@B
extern "C" const CRect MS_ABI impl__rectDefault_CFrameWnd__2VCRect__B = {};
// Symbol: ?GetThisClass@CFrameWnd@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CFrameWnd__SAPEAUCRuntimeClass__XZ() {
    return &CFrameWnd::classCFrameWnd;
}
// Symbol: ??0CFrameWnd@@QEAA@XZ
// CFrameWnd constructor
// Ordinal: 502
extern "C" CFrameWnd* MS_ABI impl___0CFrameWnd__QEAA_XZ(CFrameWnd* pThis) {
    if (!pThis) {
        return nullptr;
    }

    // Do NOT memset the full object: MSVC may set the vptr in the caller thunk
    // before invoking this imported constructor. Wiping the vptr breaks all
    // subsequent virtual calls and crashes immediately (Hello World test).
    void* savedVptr = *reinterpret_cast<void**>(pThis);
    std::memset(reinterpret_cast<unsigned char*>(pThis) + sizeof(void*), 0, sizeof(CFrameWnd) - sizeof(void*));
    *reinterpret_cast<void**>(pThis) = savedVptr;
    return pThis;
}
// Symbol: ??1CFrameWnd@@UEAA@XZ
// CFrameWnd destructor
// Ordinal: 1129
extern "C" void MS_ABI impl___1CFrameWnd__UEAA_XZ(CFrameWnd* pThis) {
    if (pThis && pThis->m_hWnd) {
        g_hwndMap.erase(pThis->m_hWnd);
        ::DestroyWindow(pThis->m_hWnd);
        pThis->m_hWnd = nullptr;
    }
}
// Symbol: ?Create@CFrameWnd@@UEAAHPEB_W0KAEBUtagRECT@@PEAVCWnd@@0KPEAUCCreateContext@@@Z
// CFrameWnd::Create
// Ordinal: 3091
extern "C" int MS_ABI impl__Create_CFrameWnd__UEAAHPEB_W0KAEBUtagRECT__PEAVCWnd__0KPEAUCCreateContext___Z(
    CFrameWnd* pThis,
    const wchar_t* lpszClassName,
    const wchar_t* lpszWindowName,
    DWORD dwStyle,
    const RECT& rect,
    CWnd* pParentWnd,
    const wchar_t* lpszMenuName,
    DWORD dwExStyle,
    CCreateContext* pContext)
{
    (void)pContext;

    HINSTANCE hInst = AfxGetInstanceHandle();
    if (!hInst) {
        hInst = GetModuleHandle(nullptr);
    }

    // Use default class if none specified
    const wchar_t* className = lpszClassName;
    if (!className || className[0] == 0) {
        if (!g_atomOpenMFCClass) {
            g_atomOpenMFCClass = RegisterOpenMFCClass(hInst);
        }
        className = g_szOpenMFCClass;
    }

    // Default style for frame window
    if (dwStyle == 0) {
        dwStyle = WS_OVERLAPPEDWINDOW;
    }

    // Default rect
    RECT useRect = rect;
    if (rect.left == 0 && rect.top == 0 && rect.right == 0 && rect.bottom == 0) {
        useRect.left = CW_USEDEFAULT;
        useRect.top = CW_USEDEFAULT;
        useRect.right = CW_USEDEFAULT;
        useRect.bottom = CW_USEDEFAULT;
    }

    // Load menu if specified
    HMENU hMenu = nullptr;
    if (lpszMenuName) {
        hMenu = ::LoadMenuW(hInst, lpszMenuName);
    }

    // Set up CREATESTRUCT for PreCreateWindow
    CREATESTRUCTW cs = {};
    cs.lpszClass = className;
    cs.lpszName = lpszWindowName;
    cs.style = dwStyle;
    cs.x = useRect.left;
    cs.y = useRect.top;
    cs.cx = (useRect.right == CW_USEDEFAULT) ? CW_USEDEFAULT : (useRect.right - useRect.left);
    cs.cy = (useRect.bottom == CW_USEDEFAULT) ? CW_USEDEFAULT : (useRect.bottom - useRect.top);
    cs.hwndParent = pParentWnd ? pParentWnd->m_hWnd : nullptr;
    cs.hMenu = hMenu;
    cs.hInstance = hInst;
    cs.dwExStyle = dwExStyle;

    // Call PreCreateWindow (virtual)
    // Avoid virtual dispatch: see note in CWnd::Create.
    if (!impl__PreCreateWindow_CFrameWnd__MEAAHAEAUtagCREATESTRUCTW___Z(pThis, cs)) {
        return FALSE;
    }

    // Create the window
    HWND hWnd = CreateWindowExW(
        cs.dwExStyle,
        cs.lpszClass,
        cs.lpszName,
        cs.style,
        cs.x, cs.y, cs.cx, cs.cy,
        cs.hwndParent,
        cs.hMenu,
        cs.hInstance,
        pThis
    );

    if (!hWnd) {
        return FALSE;
    }

    pThis->m_hWnd = hWnd;
    g_hwndMap[hWnd] = pThis;

    return TRUE;
}
// Symbol: ?PreCreateWindow@CFrameWnd@@MEAAHAEAUtagCREATESTRUCTW@@@Z
// CFrameWnd::PreCreateWindow
// Ordinal: 11791
extern "C" int MS_ABI impl__PreCreateWindow_CFrameWnd__MEAAHAEAUtagCREATESTRUCTW___Z(
    CFrameWnd* pThis, CREATESTRUCTW& cs)
{
    if (!pThis) {
        return FALSE;
    }
    if (cs.style == 0) {
        cs.style = WS_OVERLAPPEDWINDOW;
    }
    if (!cs.lpszClass || cs.lpszClass[0] == 0) {
        if (!g_atomOpenMFCClass) {
            HINSTANCE hInst = AfxGetInstanceHandle();
            if (!hInst) {
                hInst = GetModuleHandle(nullptr);
            }
            if (hInst) {
                g_atomOpenMFCClass = RegisterOpenMFCClass(hInst);
            }
        }
        cs.lpszClass = g_szOpenMFCClass;
    }

    // Reuse the base behavior so any default window checks remain consistent.
    if (!impl__PreCreateWindow_CWnd__UEAAHAEAUtagCREATESTRUCTW___Z(pThis, cs)) {
        return FALSE;
    }
    return TRUE;
}
// Symbol: ?LoadFrame@CFrameWnd@@UEAAHIKPEAVCWnd@@PEAUCCreateContext@@@Z
// CFrameWnd::LoadFrame
// Ordinal: 8105
extern "C" int MS_ABI impl__LoadFrame_CFrameWnd__UEAAHIKPEAVCWnd__PEAUCCreateContext___Z(
    CFrameWnd* pThis,
    UINT nIDResource,
    DWORD dwDefaultStyle,
    CWnd* pParentWnd,
    CCreateContext* pContext)
{
    HINSTANCE hInst = AfxGetInstanceHandle();
    if (!hInst) {
        hInst = ::GetModuleHandleW(nullptr);
    }

    // Try to load window title from string table
    wchar_t szTitle[256] = L"OpenMFC Window";
    ::LoadStringW(hInst, nIDResource, szTitle, 256);

    // Try to load menu from resource
    HMENU hMenu = ::LoadMenuW(hInst, MAKEINTRESOURCEW(nIDResource));

    // Try to load icon from resource
    HICON hIcon = ::LoadIconW(hInst, MAKEINTRESOURCEW(nIDResource));
    if (!hIcon) {
        hIcon = ::LoadIconW(nullptr, MAKEINTRESOURCEW(IDI_APPLICATION));
    }

    // Try to load accelerator table
    HACCEL hAccel = ::LoadAcceleratorsW(hInst, MAKEINTRESOURCEW(nIDResource));
    if (hAccel) {
        pThis->m_hAccelTable = hAccel;
    }

    // Store menu resource ID
    pThis->m_nIDHelp = nIDResource;

    RECT rect = {CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT};

    // Create the window with the loaded menu
    int result = impl__Create_CFrameWnd__UEAAHPEB_W0KAEBUtagRECT__PEAVCWnd__0KPEAUCCreateContext___Z(
        pThis,
        nullptr,           // Default class
        szTitle,           // Title from resource
        dwDefaultStyle ? dwDefaultStyle : WS_OVERLAPPEDWINDOW,
        rect,
        pParentWnd,
        MAKEINTRESOURCEW(nIDResource),  // Menu resource ID
        0,                 // No extended style
        pContext
    );

    // Set the menu if window was created successfully
    if (result && pThis->m_hWnd) {
        if (hMenu) {
            ::SetMenu(pThis->m_hWnd, hMenu);
        }
        // Set icon
        if (hIcon) {
            ::SendMessageW(pThis->m_hWnd, WM_SETICON, ICON_BIG, (LPARAM)hIcon);
            ::SendMessageW(pThis->m_hWnd, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
        }
    }

    return result;
}
// Symbol: ?GetMessageString@CFrameWnd@@UEBAXIAEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@@Z
extern "C" void MS_ABI impl__GetMessageString_CFrameWnd__UEBAXIAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
    const CFrameWnd* pThis, unsigned int nID, CString* rMessage) {
    if (!rMessage) {
        return;
    }
    rMessage->Empty();
    if (pThis) {
        pThis->GetMessageString(nID, *rMessage);
    }
}
CFrameWnd::CFrameWnd() {
    impl___0CFrameWnd__QEAA_XZ(this);
}
CFrameWnd::~CFrameWnd() {
    impl___1CFrameWnd__UEAA_XZ(this);
}
int CFrameWnd::Create(const wchar_t* lpszClassName, const wchar_t* lpszWindowName,
                      DWORD dwStyle, const RECT& rect, CWnd* pParentWnd,
                      const wchar_t* lpszMenuName, DWORD dwExStyle, CCreateContext* pContext) {
    return impl__Create_CFrameWnd__UEAAHPEB_W0KAEBUtagRECT__PEAVCWnd__0KPEAUCCreateContext___Z(
        this, lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, lpszMenuName, dwExStyle, pContext);
}
int CFrameWnd::PreCreateWindow(CREATESTRUCTW& cs) {
    return impl__PreCreateWindow_CFrameWnd__MEAAHAEAUtagCREATESTRUCTW___Z(this, cs);
}
int CFrameWnd::LoadFrame(UINT nIDResource, DWORD dwDefaultStyle, CWnd* pParentWnd, CCreateContext* pContext) {
    return impl__LoadFrame_CFrameWnd__UEAAHIKPEAVCWnd__PEAUCCreateContext___Z(
        this, nIDResource, dwDefaultStyle, pParentWnd, pContext);
}
void CFrameWnd::ActivateFrame(int nCmdShow) {
    if (!m_hWnd) {
        return;
    }
    int cmd = (nCmdShow == -1) ? SW_SHOW : nCmdShow;
    ::ShowWindow(m_hWnd, cmd);
    ::UpdateWindow(m_hWnd);
}
void CFrameWnd::RecalcLayout(int bNotify) {
    (void)bNotify;
    if (m_hWnd) {
        ::SendMessageW(m_hWnd, WM_SIZE, 0, 0);
    }
}
int CFrameWnd::OnCreate(void* lpCreateStruct) {
    CREATESTRUCTW* lpcs = reinterpret_cast<CREATESTRUCTW*>(lpCreateStruct);
    if (!m_hWnd) {
        return -1;
    }

    CCreateContext* pContext = nullptr;
    if (lpcs) {
        pContext = reinterpret_cast<CCreateContext*>(lpcs->lpCreateParams);
    }

    return OnCreateHelper(lpcs, pContext) ? 0 : -1;
}
const CRect CFrameWnd::rectDefault = {};
void CFrameWnd::AddFrameWnd() {
    auto& state = GetFrameRuntimeState(this);
    state.menuBarState = 0;
    state.menuBarVisibility = 1;
    state.progressMin = 0;
    state.progressMax = 100;
    state.progressPos = 0;
}
void CFrameWnd::BeginModalState() {
    auto& state = GetFrameRuntimeState(this);
    HWND hOwner = static_cast<HWND>(GetTopLevelOwner());
    if (!hOwner || hOwner == m_hWnd) {
        ++state.modalDepth;
        return;
    }

    if (state.modalDepth == 0) {
        state.modalOwner = hOwner;
        ::EnableWindow(hOwner, FALSE);
    }
    ++state.modalDepth;
}
void CFrameWnd::BringToTop(int nCmdShow) {
    if (!m_hWnd) {
        return;
    }

    int showCmd = nCmdShow > 0 ? nCmdShow : SW_SHOW;
    ::ShowWindow(m_hWnd, showCmd);
    ::BringWindowToTop(m_hWnd);
}
DWORD CFrameWnd::CanDock(CRect rect, DWORD dwDockStyle, CDockBar** ppDockBar) {
    (void)rect;
    if (ppDockBar) {
        *ppDockBar = nullptr;
    }
    return (m_hWnd && dwDockStyle != 0) ? 1 : 0;
}
int CFrameWnd::CanEnterHelpMode() {
    return m_hWnd ? TRUE : FALSE;
}
CMiniDockFrameWnd* CFrameWnd::CreateFloatingFrame(DWORD dwStyle) {
    if (!m_hWnd) {
        return nullptr;
    }
    if (dwStyle == 0) {
        ::ShowWindow(m_hWnd, SW_SHOWNORMAL);
        return nullptr;
    }

    RECT hostRect{};
    if (!::GetWindowRect(m_hWnd, &hostRect)) {
        hostRect.left = 0;
        hostRect.top = 0;
        hostRect.right = 320;
        hostRect.bottom = 240;
    }

    int width = hostRect.right - hostRect.left;
    int height = hostRect.bottom - hostRect.top;
    if (width <= 0) {
        width = 320;
    }
    if (height <= 0) {
        height = 240;
    }
    RECT frameRect{
        hostRect.left,
        hostRect.top,
        hostRect.left + width,
        hostRect.top + height,
    };

    DWORD effectiveStyle = dwStyle | WS_POPUP | WS_CAPTION | WS_THICKFRAME | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
    CString title;
    if (m_hWnd) {
        int textLen = ::GetWindowTextLengthW(m_hWnd);
        if (textLen > 0) {
            int cap = std::max(textLen + 1, 256);
            wchar_t* text = title.GetBuffer(cap);
            int actual = ::GetWindowTextW(m_hWnd, text, cap);
            title.ReleaseBuffer(actual > 0 ? actual : 0);
        }
    }
    if (title.IsEmpty()) {
        title = L"OpenMFC Floating";
    }

    CMiniFrameWnd* pFrame = new CMiniFrameWnd();
    if (!pFrame) {
        return nullptr;
    }

    if (!pFrame->Create(
            g_szOpenMFCClass,
            title.GetString(),
            effectiveStyle,
            frameRect,
            this,
            nullptr,
            WS_EX_TOOLWINDOW,
            nullptr)) {
        delete pFrame;
        return nullptr;
    }

    pFrame->ShowWindow(SW_SHOWNORMAL);
    pFrame->SetWindowTextW(title.GetString());

    return reinterpret_cast<CMiniDockFrameWnd*>(pFrame);
}
CWnd* CFrameWnd::CreateView(CCreateContext* pContext, unsigned int nID) {
    (void)pContext;
    if (!m_hWnd || !nID) {
        return nullptr;
    }
    HWND hWndView = ::GetDlgItem(m_hWnd, static_cast<int>(nID));
    return hWndView ? CWnd::FromHandle(hWndView) : nullptr;
}
void CFrameWnd::DelayUpdateFrameMenu(HMENU hMenuAlt) {
    OnUpdateFrameMenu(hMenuAlt);
}
void CFrameWnd::DestroyDockBars() {
    auto& state = GetFrameRuntimeState(this);
    for (CControlBar* pBar : state.controlBars) {
        if (pBar && pBar->m_hWnd) {
            ::ShowWindow(pBar->m_hWnd, SW_HIDE);
        }
    }
    state.controlBars.clear();
}
void CFrameWnd::DockControlBar(CControlBar* pBar, unsigned int nDockBarID, const RECT* lpRect) {
    (void)nDockBarID;
    if (!pBar) {
        return;
    }

    auto& state = GetFrameRuntimeState(this);
    bool already = false;
    for (CControlBar* bar : state.controlBars) {
        if (bar == pBar) {
            already = true;
            break;
        }
    }
    if (!already) {
        state.controlBars.push_back(pBar);
    }

    if (pBar->m_hWnd && lpRect) {
        ::MoveWindow(pBar->m_hWnd,
                     lpRect->left, lpRect->top,
                     lpRect->right - lpRect->left,
                     lpRect->bottom - lpRect->top,
                     TRUE);
    }
    if (pBar->m_hWnd) {
        ::ShowWindow(pBar->m_hWnd, SW_SHOW);
    }
}
void CFrameWnd::DockControlBar(CControlBar* pBar, CDockBar* pDockBar, const RECT* lpRect) {
    (void)pDockBar;
    // Delegate to the nDockBarID overload; a CDockBar* argument would resolve
    // back to this same overload and recurse infinitely.
    DockControlBar(pBar, 0u, lpRect);
}
void CFrameWnd::EnableDocking(DWORD dwDockStyle) {
    GetFrameRuntimeState(this).dockingStyle = dwDockStyle;
}
void CFrameWnd::EndModalState() {
    auto& state = GetFrameRuntimeState(this);
    if (state.modalDepth == 0) {
        return;
    }
    --state.modalDepth;
    if (state.modalDepth == 0 && state.modalOwner) {
        if (::IsWindow(state.modalOwner)) {
            ::EnableWindow(state.modalOwner, TRUE);
        }
        state.modalOwner = nullptr;
    }
}
void CFrameWnd::ExitHelpMode() {
    // End SHIFT+F1 context-help mode: drop any mouse capture taken for the help
    // cursor and restore the normal arrow pointer.
    if (m_hWnd && ::GetCapture() == m_hWnd) {
        ::ReleaseCapture();
    }
    ::SetCursor(::LoadCursorW(nullptr, MAKEINTRESOURCEW(IDC_ARROW)));
}
void CFrameWnd::FloatControlBar(CControlBar* pBar, CPoint pt, DWORD dwStyle) {
    (void)dwStyle;
    if (!pBar || !pBar->m_hWnd) {
        return;
    }
    RECT windowRect{};
    int width = 200;
    int height = 150;
    if (::GetWindowRect(pBar->m_hWnd, &windowRect)) {
        width = windowRect.right - windowRect.left;
        height = windowRect.bottom - windowRect.top;
    }
    ::SetWindowPos(pBar->m_hWnd, nullptr,
                   pt.x, pt.y, width, height,
                   SWP_NOZORDER | SWP_NOACTIVATE);
}
CDocument* CFrameWnd::GetActiveDocument() {
    CView* pView = GetActiveView();
    return pView ? pView->GetDocument() : nullptr;
}
CFrameWnd* CFrameWnd::GetActiveFrame() { return this; }
CView* CFrameWnd::GetActiveView() const {
    if (m_pViewActive) {
        return dynamic_cast<CView*>(m_pViewActive);
    }

    if (!m_hWnd) {
        return nullptr;
    }

    HWND hWndView = ::GetDlgItem(m_hWnd, AFX_IDW_PANE_FIRST);
    if (!hWndView) {
        return nullptr;
    }
    return dynamic_cast<CView*>(CWnd::FromHandle(hWndView));
}
void CFrameWnd::GetDockState(CDockState& state) const {
    if (!m_hWnd) {
        return;
    }
    auto& frameState = GetFrameRuntimeState(const_cast<CFrameWnd*>(this));
    std::vector<DockBarSnapshot> snapshot;
    snapshot.reserve(frameState.controlBars.size());
    for (CControlBar* bar : frameState.controlBars) {
        if (!bar) {
            continue;
        }
        DockBarSnapshot item;
        item.bar = bar;
        item.dockStyle = bar->m_dwDockStyle;
        if (bar->m_hWnd) {
            item.id = static_cast<UINT>(::GetWindowLongPtrW(bar->m_hWnd, GWLP_ID));
            item.visible = ::IsWindowVisible(bar->m_hWnd) != FALSE;
            ::GetWindowRect(bar->m_hWnd, &item.windowRect);
        }
        snapshot.push_back(item);
    }

    std::lock_guard<std::mutex> lk(g_wndStateMutex);
    g_dockStateSnapshots[&state] = std::move(snapshot);
}
const wchar_t* CFrameWnd::GetIconWndClass(DWORD dwDefaultStyle, unsigned int nIDResource) {
    (void)dwDefaultStyle;
    (void)nIDResource;
    return g_szOpenMFCClass;
}
CControlBar* CFrameWnd::GetControlBar(unsigned int nID) {
    if (!m_hWnd || !nID) {
        return nullptr;
    }
    HWND hWndBar = ::GetDlgItem(m_hWnd, static_cast<int>(nID));
    if (!hWndBar) {
        return nullptr;
    }
    CWnd* pWnd = CWnd::FromHandle(hWndBar);
    if (!pWnd) {
        return nullptr;
    }
    auto& state = GetFrameRuntimeState(this);
    for (CControlBar* pBar : state.controlBars) {
        if (pBar && pBar->m_hWnd == hWndBar) {
            return pBar;
        }
    }
    return dynamic_cast<CControlBar*>(pWnd);
}
HACCEL CFrameWnd::GetDefaultAccelerator() { return m_hAccelTable; }
CMenu* CFrameWnd::GetMenu() const {
    if (!m_hWnd) {
        return nullptr;
    }
    return CMenu::FromHandle(::GetMenu(m_hWnd));
}
int CFrameWnd::GetMenuBarInfo(long id, long lParam, MENUBARINFO* pmbi) const {
    if (!m_hWnd || !pmbi) {
        return FALSE;
    }
    pmbi->cbSize = sizeof(*pmbi);
    return ::GetMenuBarInfo(m_hWnd, id, lParam, pmbi) ? TRUE : FALSE;
}
DWORD CFrameWnd::GetMenuBarState() const { return GetFrameRuntimeState(const_cast<CFrameWnd*>(this)).menuBarState; }
DWORD CFrameWnd::GetMenuBarVisibility() const { return GetFrameRuntimeState(const_cast<CFrameWnd*>(this)).menuBarVisibility; }
CWnd* CFrameWnd::GetMessageBar() {
    if (!m_hWnd) {
        return nullptr;
    }
    HWND hWndStatus = ::GetDlgItem(m_hWnd, AFX_IDW_STATUS_BAR);
    return hWndStatus ? CWnd::FromHandle(hWndStatus) : nullptr;
}
void CFrameWnd::GetMessageString(unsigned int nID, CString& rMessage) const {
    rMessage.Empty();
    if (!nID) {
        return;
    }
    wchar_t buffer[1024];
    int len = ::LoadStringW(AfxGetResourceHandle(), nID, buffer, 1024);
    if (len > 0) {
        rMessage = buffer;
    }
}
void CFrameWnd::InitialUpdateFrame(CDocument* pDoc, int bMakeVisible) {
    CView* pFirstView = nullptr;
    if (pDoc) {
        void* pos = pDoc->GetFirstViewPosition();
        while (pos) {
            CView* pView = pDoc->GetNextView(pos);
            if (!pView) {
                continue;
            }
            if (!pFirstView) {
                pFirstView = pView;
            }
            pView->OnInitialUpdate();
        }

        const wchar_t* docTitle = pDoc->GetTitle();
        if (docTitle && *docTitle) {
            UpdateFrameTitleForDocument(docTitle);
        }
    }

    if (pFirstView && !m_pViewActive) {
        SetActiveView(pFirstView, TRUE);
    }
    if (m_hWnd) {
        RecalcLayout(TRUE);
        if (bMakeVisible) {
            ActivateFrame(SW_SHOW);
        }
    }
}
int CFrameWnd::IsFrameWnd() const { return 1; }
int CFrameWnd::IsTracking() {
    if (!m_hWnd) {
        return FALSE;
    }
    return (::GetCapture() == m_hWnd) ? TRUE : FALSE;
}
BOOL CFrameWnd::LoadAccelTable(const wchar_t* lpszAccelTable) {
    m_hAccelTable = nullptr;
    if (!lpszAccelTable) {
        return FALSE;
    }
    m_hAccelTable = ::LoadAcceleratorsW(AfxGetResourceHandle(), lpszAccelTable);
    return m_hAccelTable != nullptr;
}
void CFrameWnd::LoadBarState(const wchar_t* lpszProfileName) {
    if (!lpszProfileName) {
        return;
    }
    auto& state = GetFrameRuntimeState(this);
    std::vector<int> vis;
    {
        std::lock_guard<std::mutex> lk(g_wndStateMutex);
        auto it = g_barStateStore.find(lpszProfileName);
        if (it != g_barStateStore.end()) {
            vis = it->second;
        }
    }
    for (size_t i = 0; i < state.controlBars.size() && i < vis.size(); ++i) {
        if (state.controlBars[i]) {
            ShowControlBar(state.controlBars[i], vis[i], FALSE);
        }
    }
    if (state.menuBarVisibility == 0) {
        OnShowMenuBar();
    }
}
int CFrameWnd::NegotiateBorderSpace(unsigned int nBorderCmd, RECT* lpRectBorder) {
    if (!m_hWnd || !lpRectBorder || nBorderCmd == 0) {
        return 0;
    }
    RECT clientRect;
    if (!::GetClientRect(m_hWnd, &clientRect)) {
        return 0;
    }
    *lpRectBorder = clientRect;
    return 1;
}
void CFrameWnd::NotifyFloatingWindows(DWORD dwFlags) {
    ShowOwnedWindows(dwFlags ? TRUE : FALSE);
}
void CFrameWnd::OnActivate(unsigned int nState, CWnd* pWndOther, int bMinimized) {
    (void)bMinimized;
    if (!m_pViewActive) {
        return;
    }
    CView* pActiveView = dynamic_cast<CView*>(m_pViewActive);
    if (!pActiveView) {
        return;
    }
    int bActivate = (nState == WA_ACTIVE || nState == WA_CLICKACTIVE) ? TRUE : FALSE;
    pActiveView->OnActivateView(bActivate, pActiveView, pWndOther ? dynamic_cast<CView*>(pWndOther) : nullptr);
}
__int64 CFrameWnd::OnActivateTopLevel(unsigned __int64 wParam, __int64 lParam) {
    OnActivate(static_cast<unsigned int>(wParam), lParam ? CWnd::FromHandle(reinterpret_cast<HWND>(lParam)) : nullptr, 0);
    return 0;
}
int CFrameWnd::OnBarCheck(unsigned int nID) {
    CControlBar* pBar = GetControlBar(nID);
    return (pBar && pBar->m_hWnd && ::IsWindowVisible(pBar->m_hWnd)) ? TRUE : FALSE;
}
BOOL CFrameWnd::OnChevronPushed(unsigned int nIndex, NMHDR* pNMHDR, __int64* lResult) {
    (void)nIndex;
    (void)pNMHDR;
    // Base frame has no rebar chevron popup to expand; report "not handled" so
    // the notification falls through to default processing.
    if (lResult) {
        *lResult = 0;
    }
    return FALSE;
}
void CFrameWnd::OnClose() {
    if (!m_hWnd) {
        return;
    }
    DestroyWindow();
}
int CFrameWnd::OnCmdMsg(unsigned int nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) {
    CWnd* pView = m_pViewActive ? m_pViewActive : GetActiveView();
    if (pView && pView->OnCmdMsg(nID, nCode, pExtra, pHandlerInfo)) {
            return TRUE;
    }

    if (CWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo)) {
        return TRUE;
    }

    return FALSE;
}
int CFrameWnd::OnCommand(unsigned __int64 wParam, __int64 lParam) {
    (void)lParam;
    unsigned int nID = static_cast<unsigned int>(LOWORD(static_cast<WPARAM>(wParam)));
    int nCode = static_cast<int>(HIWORD(static_cast<WPARAM>(wParam)));
    if (OnCmdMsg(nID, nCode, nullptr, nullptr)) {
        return TRUE;
    }
    return CWnd::OnCommand(static_cast<uintptr_t>(wParam), static_cast<intptr_t>(lParam));
}
__int64 CFrameWnd::OnCommandHelp(unsigned __int64 wParam, __int64 lParam) {
    if (!m_hWnd) {
        return 0;
    }

    if (lParam) {
        HELPINFO* pHelp = reinterpret_cast<HELPINFO*>(static_cast<LPARAM>(lParam));
        if (pHelp && pHelp->dwContextId) {
            CString text;
            GetMessageString(static_cast<unsigned int>(pHelp->dwContextId), text);
            SetMessageText(static_cast<const wchar_t*>(text));
            return 1;
        }
    }

    if (wParam) {
        SetMessageText(static_cast<unsigned int>(wParam));
        return 1;
    }

    OnHelp();
    return 1;
}
void CFrameWnd::OnContextHelp() {
    OnHelp();
}
int CFrameWnd::OnCreateClient(CREATESTRUCTW* lpcs, CCreateContext* pContext) {
    (void)lpcs;
    if (!m_hWnd) {
        return FALSE;
    }

    CWnd* pClient = CreateView(pContext, AFX_IDW_PANE_FIRST);
    if (pClient && pClient->m_hWnd) {
        ::ShowWindow(pClient->m_hWnd, SW_SHOW);
    }

    return TRUE;
}
int CFrameWnd::OnCreateHelper(CREATESTRUCTW* lpcs, CCreateContext* pContext) {
    AddFrameWnd();
    ::DragAcceptFiles(m_hWnd, TRUE);
    return OnCreateClient(lpcs, pContext);
}
void CFrameWnd::OnDDEExecute(CWnd* pWnd, void* pData) {
    // Real MFC unpacks the WM_DDE_EXECUTE command block and routes it through
    // CWinApp::OnDDECommand. This clean-room CWinApp exposes no DDE command
    // dispatch yet, so acknowledge negatively rather than fabricate a status
    // string: post WM_DDE_ACK with a zero (refused) status back to the partner.
    (void)pData;
    if (pWnd && pWnd->m_hWnd && m_hWnd) {
        ::PostMessageW(pWnd->m_hWnd, WM_DDE_ACK,
                       reinterpret_cast<WPARAM>(m_hWnd), 0);
    }
}
void CFrameWnd::OnDDEInitiate(CWnd* pWnd, unsigned int nAtomApp, unsigned int nAtomTopic) {
    wchar_t appName[128] = {};
    wchar_t topicName[128] = {};
    if (nAtomApp) {
        if (::GlobalGetAtomNameW(nAtomApp, appName, static_cast<int>(std::size(appName))) <= 0) {
            appName[0] = L'\0';
        }
    }
    if (nAtomTopic) {
        if (::GlobalGetAtomNameW(nAtomTopic, topicName, static_cast<int>(std::size(topicName))) <= 0) {
            topicName[0] = L'\0';
        }
    }

    std::wstring message = L"DDE initiate";
    if (appName[0] || topicName[0]) {
        message += L": ";
        if (appName[0]) {
            message += appName;
        }
        if (topicName[0]) {
            if (appName[0]) {
                message += L"/";
            }
            message += topicName;
        }
    }
    SetMessageText(message.c_str());

    if (pWnd && pWnd->m_hWnd) {
        ::PostMessageW(pWnd->m_hWnd, WM_DDE_ACK,
                       reinterpret_cast<WPARAM>(m_hWnd), 0);
    }
}
void CFrameWnd::OnDDETerminate(CWnd* pWnd) {
    // Complete the DDE conversation teardown by echoing WM_DDE_TERMINATE back to
    // the partner, as the protocol requires.
    if (pWnd && pWnd->m_hWnd && m_hWnd) {
        ::PostMessageW(pWnd->m_hWnd, WM_DDE_TERMINATE,
                       reinterpret_cast<WPARAM>(m_hWnd), 0);
    }
}
void CFrameWnd::OnDestroy() {
    auto* pApp = AfxGetApp();
    if (pApp && pApp->m_pMainWnd == this) {
        pApp->m_pMainWnd = nullptr;
    }
    RemoveFrameWnd();
    ShowOwnedWindows(FALSE);
}
void CFrameWnd::OnDropFiles(HDROP hDropInfo) {
    if (!hDropInfo) {
        return;
    }

    // Real MFC activates the frame then hands each dropped path to
    // CWinApp::OpenDocumentFile. This clean-room CWinApp has no document manager,
    // so we perform the frame-level part faithfully (activate + notify each file
    // via WM_COPYDATA to the active view for app-level handling) and always
    // DragFinish to release the drop buffer.
    ::SetActiveWindow(m_hWnd);
    UINT count = ::DragQueryFileW(hDropInfo, 0xFFFFFFFF, nullptr, 0);
    CWnd* pView = GetActiveView();
    for (UINT i = 0; i < count; ++i) {
        wchar_t filePath[MAX_PATH] = {};
        UINT len = ::DragQueryFileW(hDropInfo, i, filePath, MAX_PATH);
        if (len == 0 || !pView || !pView->m_hWnd) {
            continue;
        }
        COPYDATASTRUCT cds = {};
        cds.dwData = static_cast<ULONG_PTR>(WM_DROPFILES);
        cds.cbData = static_cast<DWORD>((len + 1) * sizeof(wchar_t));
        cds.lpData = filePath;
        ::SendMessageW(pView->m_hWnd, WM_COPYDATA,
                       reinterpret_cast<WPARAM>(m_hWnd),
                       reinterpret_cast<LPARAM>(&cds));
    }
    ::DragFinish(hDropInfo);
}
void CFrameWnd::OnEnable(int bEnable) {
    if (!m_hWnd) {
        return;
    }
    SetMessageText(bEnable ? L"Frame enabled" : L"Frame disabled");
    if (::IsWindowEnabled(m_hWnd) != (bEnable != 0)) {
        ::EnableWindow(m_hWnd, bEnable ? TRUE : FALSE);
    }
}
void CFrameWnd::OnEndSession(int bEnding) {
    if (!m_hWnd) {
        return;
    }
    SetMessageText(bEnding ? L"Session ending" : L"Session continues");
}
void CFrameWnd::OnEnterIdle(unsigned int nWhy, CWnd* pWho) {
    if (!m_hWnd) {
        return;
    }
    if (nWhy == MSGF_MENU || nWhy == MSGF_DIALOGBOX || nWhy == MSGF_MESSAGEBOX) {
        CMenu* pMenu = GetMenu();
        if (auto* pFrame = pWho ? dynamic_cast<CFrameWnd*>(pWho) : nullptr) {
            CMenu* pCandidate = pFrame->GetMenu();
            if (pCandidate) {
                pMenu = pCandidate;
            }
        }
        HMENU hMenu = pMenu ? pMenu->m_hMenu : nullptr;
        OnUpdateFrameMenu(hMenu);
        SetMessageText(pWho ? L"Idle in command mode" : L"Idle");
    } else if (pWho && pWho->m_hWnd == m_hWnd) {
        SetMessageText(L"Idle");
    }
}
int CFrameWnd::OnEraseBkgnd(CDC* pDC) {
    if (!pDC || !pDC->m_hDC) {
        return FALSE;
    }

    RECT clientRect = {};
    if (m_hWnd) {
        ::GetClientRect(m_hWnd, &clientRect);
        HBRUSH bgBrush = ::GetSysColorBrush(COLOR_3DFACE);
        ::FillRect(pDC->m_hDC, &clientRect, bgBrush);
        return TRUE;
    }

    return FALSE;
}
void CFrameWnd::OnHelp() {
    SetMessageText(L"Help requested");
    if (m_nIDHelp) {
        CString message;
        message.Format(L"Help topic id: %u", m_nIDHelp);
        SetMessageText(message.GetString());
    }
}
__int64 CFrameWnd::OnHelpHitTest(unsigned __int64 wParam, __int64 lParam) {
    (void)wParam;
    auto* pHelp = reinterpret_cast<HELPINFO*>(static_cast<LPARAM>(lParam));
    if (!pHelp) {
        return 0;
    }
    return static_cast<__int64>(pHelp->dwContextId);
}
__int64 CFrameWnd::OnHelpPromptAddr(unsigned __int64 wParam, __int64 lParam) {
    (void)wParam;
    auto* pHelp = reinterpret_cast<HELPINFO*>(static_cast<LPARAM>(lParam));
    if (pHelp && pHelp->dwContextId) {
        CString message;
        GetMessageString(static_cast<unsigned int>(pHelp->dwContextId), message);
        SetMessageText(message.GetString());
        return 1;
    }
    return 0;
}
void CFrameWnd::OnHideMenuBar() {
    if (!m_hWnd) {
        return;
    }

    auto& state = GetFrameRuntimeState(this);
    if (state.menuBarVisibility == 0) {
        return;
    }

    state.hiddenMenu = ::GetMenu(m_hWnd);
    ::SetMenu(m_hWnd, nullptr);
    state.menuBarVisibility = 0;
    ::DrawMenuBar(m_hWnd);
}
void CFrameWnd::OnHScroll(unsigned int nSBCode, unsigned int nPos, CScrollBar* pScrollBar) {
    if (pScrollBar && pScrollBar->m_hWnd) {
        ::SendMessageW(pScrollBar->m_hWnd, WM_HSCROLL, MAKELONG(nPos, nSBCode), 0);
    }
    if (m_pViewActive) {
        m_pViewActive->OnHScroll(nSBCode, nPos, pScrollBar);
    }
}
void CFrameWnd::OnIdleUpdateCmdUI() {
    OnUpdateFrameMenu(nullptr);
}
void CFrameWnd::OnInitMenu(CMenu* pMenu) {
    HMENU hMenu = pMenu ? pMenu->m_hMenu : GetMenu() ? GetMenu()->m_hMenu : nullptr;
    if (hMenu) {
        OnUpdateFrameMenu(hMenu);
    }
}
void CFrameWnd::OnInitMenuPopup(CMenu* pPopupMenu, unsigned int nIndex, int bSysMenu) {
    (void)nIndex;
    (void)bSysMenu;
    if (!pPopupMenu || !pPopupMenu->m_hMenu) {
        return;
    }
    OnUpdateFrameMenu(pPopupMenu->m_hMenu);
}
__int64 CFrameWnd::OnMenuChar(unsigned int nChar, unsigned int nFlags, CMenu* pMenu) {
    // Base-frame behavior: mnemonic matching against owner-drawn menu items is a
    // feature-pack (CMFCToolBar/CMFCPopupMenu) concern. With standard menus the
    // system already resolves the accelerator, so return MNC_IGNORE (high word)
    // to let default processing proceed.
    (void)nChar;
    (void)nFlags;
    (void)pMenu;
    return static_cast<__int64>(MAKELONG(0, MNC_IGNORE));
}
void CFrameWnd::OnMenuSelect(unsigned int nItemID, unsigned int nFlags, HMENU hSysMenu) {
    (void)nFlags;
    (void)hSysMenu;
    if (!m_hWnd) {
        return;
    }
    if (nItemID == 0 || nItemID == 0xFFFF) {
        SetMessageText(L"");
        return;
    }
    CString text;
    GetMessageString(nItemID, text);
    SetMessageText(text.GetString());
}
int CFrameWnd::OnNcActivate(int bActive) {
    return m_hWnd ? static_cast<int>(::DefWindowProcW(m_hWnd, WM_NCACTIVATE, static_cast<WPARAM>(bActive), 0)) : FALSE;
}
void CFrameWnd::OnPaletteChanged(CWnd* pFocusWnd) {
    if (!m_hWnd || !pFocusWnd) {
        return;
    }

    CFrameWnd* pFocusFrame = pFocusWnd->GetTopLevelFrame();
    if (pFocusFrame == this && OnQueryNewPalette()) {
        ::InvalidateRect(m_hWnd, nullptr, TRUE);
    }
}
__int64 CFrameWnd::OnPopMessageString(unsigned __int64 wParam, __int64 lParam) {
    // Restore the previously pushed message-line string. Real MFC delegates the
    // restore to OnSetMessageString with the saved id/text, which is exactly the
    // parameters forwarded here.
    return OnSetMessageString(wParam, lParam);
}
int CFrameWnd::OnQueryEndSession() {
    // Allow the session to end. Documents veto shutdown from their own
    // SaveModified path, not from the frame; the frame answers TRUE.
    return TRUE;
}
int CFrameWnd::OnQueryNewPalette() {
    return m_hWnd ? TRUE : FALSE;
}
int CFrameWnd::OnSetCursor(CWnd* pWnd, unsigned int nHitTest, unsigned int message) {
    (void)pWnd;
    (void)message;

    if (!m_hWnd) {
        return FALSE;
    }

    if (nHitTest == HTCLIENT) {
        HCURSOR hCursor = ::LoadCursorW(nullptr, MAKEINTRESOURCEW(IDC_ARROW));
        ::SetCursor(hCursor);
        return TRUE;
    }
    return FALSE;
}
void CFrameWnd::OnSetFocus(CWnd* pOldWnd) {
    (void)pOldWnd;
    if (!m_hWnd) {
        return;
    }

    if (m_pViewActive && m_pViewActive->m_hWnd) {
        ::SetFocus(m_pViewActive->m_hWnd);
    }
}
__int64 CFrameWnd::OnSetMessageString(unsigned __int64 wParam, __int64 lParam) {
    if (wParam) {
        SetMessageText(static_cast<unsigned int>(wParam));
        return 0;
    }
    if (lParam) {
        SetMessageText(reinterpret_cast<const wchar_t*>(lParam));
        return 0;
    }
    SetMessageText(L"");
    return 0;
}
void CFrameWnd::OnSetPreviewMode(int bPreview, CPrintPreviewState* pState) {
    // Entering print preview hides the frame's control bars so the preview view
    // owns the client area; leaving it restores them. This is the real,
    // observable frame behavior for WM print-preview mode.
    (void)pState;
    if (!m_hWnd) {
        return;
    }
    auto& state = GetFrameRuntimeState(this);
    for (CControlBar* pBar : state.controlBars) {
        if (pBar) {
            ShowControlBar(pBar, bPreview ? FALSE : TRUE, FALSE);
        }
    }
}
void CFrameWnd::OnShowMenuBar() {
    if (!m_hWnd) {
        return;
    }

    auto& state = GetFrameRuntimeState(this);
    if (state.menuBarVisibility != 0) {
        return;
    }

    HMENU hMenuToShow = state.hiddenMenu;
    if (!hMenuToShow && state.menuResourceId) {
        HINSTANCE hInst = AfxGetInstanceHandle();
        hMenuToShow = ::LoadMenuW(hInst ? hInst : ::GetModuleHandleW(nullptr),
                                  MAKEINTRESOURCEW(state.menuResourceId));
        // Track ownership so a previously loaded menu is not leaked when a fresh
        // one is loaded here.
        if (state.ownedMenu && state.ownedMenu != hMenuToShow) {
            ::DestroyMenu(state.ownedMenu);
        }
        state.ownedMenu = hMenuToShow;
    }

    if (hMenuToShow) {
        ::SetMenu(m_hWnd, hMenuToShow);
        ::DrawMenuBar(m_hWnd);
    }

    state.menuBarVisibility = 1;
}
void CFrameWnd::OnSize(unsigned int nType, int cx, int cy) {
    (void)nType;
    if (!m_hWnd) {
        return;
    }

    if (cx <= 0 || cy <= 0) {
        return;
    }

    if (m_pViewActive && m_pViewActive->m_hWnd) {
        ::MoveWindow(m_pViewActive->m_hWnd, 0, 0, cx, cy, TRUE);
    }

    auto& state = GetFrameRuntimeState(this);
    for (CControlBar* pBar : state.controlBars) {
        if (pBar && pBar->m_hWnd) {
            ::MoveWindow(pBar->m_hWnd, 0, 0, cx, cy, FALSE);
        }
    }
}
void CFrameWnd::OnSysCommand(unsigned int nID, __int64 lParam) {
    (void)lParam;
    if (!m_hWnd) {
        return;
    }

    const unsigned int command = nID & 0xFFF0;
    if (command == SC_CLOSE) {
        OnClose();
        return;
    }

    ::DefWindowProcW(m_hWnd, WM_SYSCOMMAND, nID, lParam);
}
BOOL CFrameWnd::OnToolTipText(unsigned int nID, NMHDR* pNMHDR, __int64* lResult) {
    (void)nID;
    if (!pNMHDR || !lResult) {
        return FALSE;
    }

    TOOLTIPTEXTW* pTTT = reinterpret_cast<TOOLTIPTEXTW*>(pNMHDR);
    if (pNMHDR->code != TTN_NEEDTEXTW && pNMHDR->code != TTN_NEEDTEXTA) {
        *lResult = 0;
        return FALSE;
    }

    BOOL handled = FALSE;
    if (pTTT->lpszText && pTTT->lpszText != LPSTR_TEXTCALLBACKW) {
        UINT toolId = 0;
        if (pTTT->uFlags & TTF_IDISHWND) {
            CWnd* pWnd = CWnd::FromHandle(reinterpret_cast<HWND>(static_cast<INT_PTR>(pNMHDR->idFrom))); 
            if (pWnd && pWnd->GetDlgCtrlID()) {
                toolId = static_cast<UINT>(pWnd->GetDlgCtrlID());
            }
        } else {
            toolId = static_cast<UINT>(pNMHDR->idFrom);
        }

        CString text;
        if (toolId) {
            GetMessageString(toolId, text);
            if (!text.IsEmpty()) {
                lstrcpynW(pTTT->lpszText, text.GetString(), 80);
                handled = TRUE;
            }
        }
    }

    *lResult = 0;
    return handled;
}
void CFrameWnd::OnUpdateContextHelp(CCmdUI* pCmdUI) {
    if (!pCmdUI) {
        return;
    }

    pCmdUI->Enable(TRUE);
    pCmdUI->SetCheck(1);
}
void CFrameWnd::OnUpdateControlBarMenu(CCmdUI* pCmdUI) {
    if (!pCmdUI) {
        return;
    }

    if (pCmdUI->m_nID) {
        CControlBar* pBar = GetControlBar(pCmdUI->m_nID);
        pCmdUI->Enable(pBar != nullptr);
        pCmdUI->SetCheck(pBar ? 1 : 0);
    } else {
        pCmdUI->Enable(TRUE);
        pCmdUI->SetCheck(0);
    }
}
void CFrameWnd::OnUpdateFrameMenu(HMENU hMenuAlt) {
    // Install the frame's menu (MDI passes the active child's menu as hMenuAlt;
    // SDI passes null to keep the current one). Set and redraw the menu bar so
    // the change is visible.
    if (!m_hWnd) {
        return;
    }
    HMENU hMenu = hMenuAlt ? hMenuAlt : ::GetMenu(m_hWnd);
    if (hMenu) {
        ::SetMenu(m_hWnd, hMenu);
        ::DrawMenuBar(m_hWnd);
    }
}
void CFrameWnd::OnUpdateFrameTitle(int bAddToTitle) {
    // Compose the caption bar text: "<document> - <app>" when the frame adds the
    // active document's title, otherwise just the app name. This updates the
    // window title (SetWindowText) — never the status/message line.
    CDocument* pDoc = bAddToTitle ? GetActiveDocument() : nullptr;
    UpdateFrameTitleForDocument(pDoc ? pDoc->GetTitle() : nullptr);
}
void CFrameWnd::OnUpdateKeyIndicator(CCmdUI* pCmdUI) {
    if (!pCmdUI) {
        return;
    }

    pCmdUI->Enable(TRUE);
    WCHAR capsState[16] = L"";
    if ((::GetKeyState(VK_CAPITAL) & 0x0001) != 0) {
        lstrcpynW(capsState, L"CAP", 4);
    }
    if ((::GetKeyState(VK_NUMLOCK) & 0x0001) != 0) {
        lstrcpynW(capsState, L"CAP NUM", 8);
    }
    pCmdUI->SetText(capsState);
}
void CFrameWnd::OnVScroll(unsigned int nSBCode, unsigned int nPos, CScrollBar* pScrollBar) {
    if (pScrollBar && pScrollBar->m_hWnd) {
        ::SendMessageW(pScrollBar->m_hWnd, WM_VSCROLL, MAKELONG(nPos, nSBCode), 0);
    }
    if (m_pViewActive) {
        m_pViewActive->OnVScroll(nSBCode, nPos, pScrollBar);
    }
}
void CFrameWnd::PostNcDestroy() {
    RemoveFrameWnd();
}
int CFrameWnd::PreTranslateMessage(MSG* pMsg) {
    if (!pMsg) {
        return FALSE;
    }

    if (!m_hWnd) {
        return FALSE;
    }

    if (m_hAccelTable && pMsg->message >= WM_KEYFIRST && pMsg->message <= WM_KEYLAST) {
        if (::TranslateAccelerator(m_hWnd, m_hAccelTable, pMsg)) {
            return TRUE;
        }
    }

    if (CWnd::WalkPreTranslateTree(m_hWnd, pMsg)) {
        return TRUE;
    }

    return CWnd_PreTranslateMessageCompat(this, pMsg);
}
int CFrameWnd::ProcessHelpMsg(MSG& msg, DWORD* pContext) {
    (void)pContext;
    if (!m_hWnd) {
        return 0;
    }
    if (msg.message == WM_HELP) {
        OnHelp();
        return 1;
    }
    return 0;
}
void CFrameWnd::ReDockControlBar(CControlBar* pBar, CDockBar* pDockBar, const RECT* lpRect) {
    (void)pDockBar;
    DockControlBar(pBar, 0u, lpRect);
}
void CFrameWnd::RemoveControlBar(CControlBar* pBar) {
    if (!pBar) {
        return;
    }

    auto& state = GetFrameRuntimeState(this);
    state.controlBars.erase(std::remove(state.controlBars.begin(), state.controlBars.end(), pBar), state.controlBars.end());
    if (pBar->m_hWnd) {
        ::ShowWindow(pBar->m_hWnd, SW_HIDE);
    }
}
void CFrameWnd::RemoveFrameWnd() {
    g_frameWndRuntimeStates.erase(this);
}
void CFrameWnd::SaveBarState(const wchar_t* lpszProfileName) const {
    if (!m_hWnd || !lpszProfileName) {
        return;
    }
    auto& state = GetFrameRuntimeState(const_cast<CFrameWnd*>(this));
    std::vector<int> vis;
    vis.reserve(state.controlBars.size());
    for (CControlBar* pBar : state.controlBars) {
        vis.push_back(pBar && pBar->m_hWnd && ::IsWindowVisible(pBar->m_hWnd) ? 1 : 0);
    }
    std::lock_guard<std::mutex> lk(g_wndStateMutex);
    g_barStateStore[lpszProfileName] = std::move(vis);
}
void CFrameWnd::SetActivePreviewView(CView* pViewNew) { SetActiveView(pViewNew, TRUE); }
void CFrameWnd::SetActiveView(CView* pViewNew, int bNotify) {
    CView* pOldView = dynamic_cast<CView*>(m_pViewActive);
    if (pOldView == pViewNew) {
        return;
    }

    if (bNotify && pOldView) {
        pOldView->OnActivateView(FALSE, pViewNew, pOldView);
    }

    m_pViewActive = pViewNew;

    if (bNotify && pViewNew) {
        pViewNew->OnActivateView(TRUE, pViewNew, pOldView);
        if (pViewNew->m_hWnd) {
            ::SetFocus(pViewNew->m_hWnd);
        }
    }
}
void CFrameWnd::SetDockState(const CDockState& state) {
    std::vector<DockBarSnapshot> snapshot;
    {
        std::lock_guard<std::mutex> lk(g_wndStateMutex);
        auto it = g_dockStateSnapshots.find(&state);
        if (it == g_dockStateSnapshots.end()) {
            return;
        }
        snapshot = it->second;
    }

    auto& frameState = GetFrameRuntimeState(this);
    for (const DockBarSnapshot& item : snapshot) {
        CControlBar* bar = nullptr;
        auto byPointer = std::find(frameState.controlBars.begin(), frameState.controlBars.end(), item.bar);
        if (byPointer != frameState.controlBars.end()) {
            bar = *byPointer;
        } else if (item.id != 0) {
            for (CControlBar* candidate : frameState.controlBars) {
                if (candidate && candidate->m_hWnd &&
                    static_cast<UINT>(::GetWindowLongPtrW(candidate->m_hWnd, GWLP_ID)) == item.id) {
                    bar = candidate;
                    break;
                }
            }
        }
        if (!bar) {
            continue;
        }

        bar->m_dwDockStyle = item.dockStyle;
        if (bar->m_hWnd) {
            const int width = item.windowRect.right - item.windowRect.left;
            const int height = item.windowRect.bottom - item.windowRect.top;
            if (width > 0 && height > 0) {
                ::SetWindowPos(bar->m_hWnd, nullptr,
                               item.windowRect.left, item.windowRect.top,
                               width, height, SWP_NOZORDER | SWP_NOACTIVATE);
            }
            ::ShowWindow(bar->m_hWnd, item.visible ? SW_SHOWNA : SW_HIDE);
        }
    }

    if (m_hWnd) {
        RECT clientRect = {};
        if (::GetClientRect(m_hWnd, &clientRect)) {
            OnSize(SIZE_RESTORED, clientRect.right - clientRect.left, clientRect.bottom - clientRect.top);
        } else {
            OnSize(SIZE_RESTORED, 0, 0);
        }
    }
}
HWND CFrameWnd::SetHelpCapture(POINT pt, int* pHit) {
    if (!m_hWnd) {
        if (pHit) {
            *pHit = HTNOWHERE;
        }
        return nullptr;
    }

    if (pHit) {
        *pHit = HTCLIENT;
    }
    return m_hWnd;
}
int CFrameWnd::SetMenu(CMenu* pMenu) {
    if (!m_hWnd) {
        return FALSE;
    }
    HMENU hMenu = pMenu ? pMenu->m_hMenu : nullptr;
    ::SetMenu(m_hWnd, hMenu);
    ::DrawMenuBar(m_hWnd);
    return TRUE;
}
int CFrameWnd::SetMenuBarState(DWORD dwState) {
    auto& state = GetFrameRuntimeState(this);
    DWORD oldState = state.menuBarState;
    state.menuBarState = dwState;
    return static_cast<int>(oldState);
}
void CFrameWnd::SetMenuBarVisibility(DWORD dwStyle) {
    // Do NOT assign state.menuBarVisibility here: OnHideMenuBar/OnShowMenuBar
    // early-return when the recorded visibility already matches, so writing the
    // new value first would make them no-ops. Let the handlers update the state.
    if (dwStyle == 0) {
        OnHideMenuBar();
    } else {
        OnShowMenuBar();
    }
}
void CFrameWnd::SetMessageText(unsigned int nStringID) {
    CString message;
    GetMessageString(nStringID, message);
    SetMessageText(message.GetString());
}
void CFrameWnd::SetMessageText(const wchar_t* lpszText) {
    const wchar_t* text = lpszText ? lpszText : L"";
    if (!m_hWnd) {
        return;
    }

    CWnd* pMessageBar = GetMessageBar();
    if (pMessageBar && pMessageBar->m_hWnd) {
        ::SendMessageW(pMessageBar->m_hWnd, SB_SETTEXTW, 0, (LPARAM)text);
    } else {
        ::SendMessageW(m_hWnd, WM_SETTEXT, 0, (LPARAM)text);
    }
}
void CFrameWnd::SetProgressBarPosition(int nPos) {
    if (!m_hWnd) {
        return;
    }

    auto& state = GetFrameRuntimeState(this);
    int minPos = state.progressMin;
    int maxPos = state.progressMax;
    if (maxPos < minPos) {
        int tmp = minPos;
        minPos = maxPos;
        maxPos = tmp;
    }
    if (nPos < minPos) {
        nPos = minPos;
    } else if (nPos > maxPos) {
        nPos = maxPos;
    }
    state.progressPos = nPos;

    ITaskbarList3* taskbar = AcquireTaskbarList3ForWindow(m_hWnd);
    if (!taskbar) {
        return;
    }
    ULONGLONG total = (maxPos > minPos) ? static_cast<ULONGLONG>(maxPos - minPos) : 1;
    ULONGLONG completed = static_cast<ULONGLONG>(nPos - minPos);
    taskbar->SetProgressValue(m_hWnd, completed, total);
    taskbar->Release();
}
void CFrameWnd::SetProgressBarRange(int nLower, int nUpper) {
    auto& state = GetFrameRuntimeState(this);
    if (nLower <= nUpper) {
        state.progressMin = nLower;
        state.progressMax = nUpper;
    } else {
        state.progressMin = nUpper;
        state.progressMax = nLower;
    }

    if (!m_hWnd) {
        return;
    }
    ITaskbarList3* taskbar = AcquireTaskbarList3ForWindow(m_hWnd);
    if (!taskbar) {
        return;
    }
    ULONGLONG total = (state.progressMax > state.progressMin)
        ? static_cast<ULONGLONG>(state.progressMax - state.progressMin)
        : 1ULL;
    taskbar->SetProgressValue(m_hWnd,
                              static_cast<ULONGLONG>(state.progressPos - state.progressMin),
                              total);
    taskbar->Release();
}
void CFrameWnd::SetProgressBarState(TBPFLAG tbpFlags) {
    if (!m_hWnd) {
        return;
    }
    ITaskbarList3* taskbar = AcquireTaskbarList3ForWindow(m_hWnd);
    if (!taskbar) {
        return;
    }
    taskbar->SetProgressState(m_hWnd, tbpFlags);
    taskbar->Release();
}
int CFrameWnd::SetTaskbarOverlayIcon(unsigned int nTaskbarButtonCreatedMsg, const wchar_t* lpszDescription) {
    if (!m_hWnd) {
        return FALSE;
    }
    if (nTaskbarButtonCreatedMsg) {
        UINT taskbarMsg = ::RegisterWindowMessageW(L"TaskbarButtonCreated");
        if (nTaskbarButtonCreatedMsg != taskbarMsg) {
            return TRUE;
        }
    }

    HICON hIcon = reinterpret_cast<HICON>(::SendMessageW(m_hWnd, WM_GETICON, ICON_SMALL, 0));
    if (!hIcon) {
        hIcon = reinterpret_cast<HICON>(::GetClassLongPtrW(m_hWnd, GCLP_HICON));
    }
    return SetTaskbarOverlayIcon(hIcon, lpszDescription);
}
int CFrameWnd::SetTaskbarOverlayIcon(HICON hIcon, const wchar_t* lpszDescription) {
    if (!m_hWnd) {
        return FALSE;
    }
    ITaskbarList3* taskbar = AcquireTaskbarList3ForWindow(m_hWnd);
    if (!taskbar) {
        return FALSE;
    }
    HRESULT hr = taskbar->SetOverlayIcon(m_hWnd, hIcon, lpszDescription);
    taskbar->Release();
    return SUCCEEDED(hr) ? TRUE : FALSE;
}
void CFrameWnd::ShowControlBar(CControlBar* pBar, int bShow, int bDelay) {
    (void)bDelay;
    if (!pBar || !pBar->m_hWnd) {
        return;
    }

    auto& state = GetFrameRuntimeState(this);
    bool isDocked = false;
    for (CControlBar* item : state.controlBars) {
        if (item == pBar) {
            isDocked = true;
            break;
        }
    }
    if (!isDocked) {
        state.controlBars.push_back(pBar);
    }

    ::ShowWindow(pBar->m_hWnd, bShow ? SW_SHOW : SW_HIDE);
}
void CFrameWnd::ShowOwnedWindows(int bShow) {
    if (!m_hWnd) {
        return;
    }

    for (HWND hWndChild = ::GetTopWindow(nullptr); hWndChild; hWndChild = ::GetWindow(hWndChild, GW_HWNDNEXT)) {
        HWND hOwner = ::GetWindow(hWndChild, GW_OWNER);
        while (hOwner) {
            if (hOwner == m_hWnd) {
                ::ShowWindow(hWndChild, bShow ? SW_SHOW : SW_HIDE);
                break;
            }
            hOwner = ::GetWindow(hOwner, GW_OWNER);
        }
    }
}
void CFrameWnd::UpdateFrameTitleForDocument(const wchar_t* lpszDocName) {
    const wchar_t* baseName = L"OpenMFC";
    if (AfxGetApp() && AfxGetApp()->m_pszAppName && AfxGetApp()->m_pszAppName[0] != L'\0') {
        baseName = AfxGetApp()->m_pszAppName;
    }

    std::wstring text = baseName;
    if (lpszDocName && lpszDocName[0] != L'\0') {
        text = std::wstring(lpszDocName) + L" - " + text;
    }

    if (m_hWnd) {
        ::SetWindowTextW(m_hWnd, text.c_str());
    }
}
