// CFrameWnd — OpenMFC implementation.
// Sources: frame_font_exports.cpp, wincore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/FrameFontExportsSupport.h"
#include "detail/FrameLifetimeSupport.h"
#include "detail/ClassicDockingSupport.h"
#include "detail/WincoreSupport.h"
#include "detail/WindowCreationSupport.h"
#include <dde.h>

using openmfc::detail::frame::ControlBars;

BEGIN_MESSAGE_MAP(CFrameWnd, CWnd)
    ON_WM_SIZE()
    ON_WM_CLOSE()
    ON_MESSAGE(0x0363, &ThisClass::OnIdleUpdateCmdUIMessage)
    ON_MESSAGE(0x0362, &ThisClass::OnSetMessageString)
END_MESSAGE_MAP()

// Symbol: ?CreateObject@CFrameWnd@@SAPEAVCObject@@XZ
extern "C" CObject* MS_ABI impl__CreateObject_CFrameWnd__SAPEAVCObject__XZ() {
    return CFrameWnd::CreateObject();
}
// Symbol: ?GetMessageMap@CFrameWnd@@MEBAPEBUAFX_MSGMAP@@XZ
extern "C" const AFX_MSGMAP* MS_ABI impl__GetMessageMap_CFrameWnd__MEBAPEBUAFX_MSGMAP__XZ(
    const CFrameWnd* pThis) {
    (void)pThis;
    return CFrameWnd::GetThisMessageMap();
}
// Symbol: ?GetRuntimeClass@CFrameWnd@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CFrameWnd__UEBAPEAUCRuntimeClass__XZ(
    const CFrameWnd* pThis) {
    return CFrameWnd::GetThisClass();
}
// Symbol: ?GetThisMessageMap@CFrameWnd@@KAPEBUAFX_MSGMAP@@XZ
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CFrameWnd__KAPEBUAFX_MSGMAP__XZ() {
    return CFrameWnd::GetThisMessageMap();
}
// Native command routing lives in core/runtime/CommandRouting.cpp.
// Symbol: ?OnCommand@CFrameWnd@@MEAAH_K_J@Z
// CFrameWnd::OnCommand
extern "C" int MS_ABI impl__OnCommand_CFrameWnd__MEAAH_K_J_Z(CFrameWnd* pThis, WPARAM wParam, LPARAM lParam) {
    return pThis ? pThis->OnCommand(wParam, lParam) : FALSE;
}
// Symbol: ?rectDefault@CFrameWnd@@2VCRect@@B
// Retail data is {CW_USEDEFAULT, CW_USEDEFAULT, 0, 0}; mfc140 RVA 0x3ab470
// contains bytes 00 00 00 80 00 00 00 80 00 00 00 00 00 00 00 00.
extern "C" const CRect MS_ABI impl__rectDefault_CFrameWnd__2VCRect__B = {
    static_cast<int>(CW_USEDEFAULT), static_cast<int>(CW_USEDEFAULT), 0, 0};
// Symbol: ?GetThisClass@CFrameWnd@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CFrameWnd__SAPEAUCRuntimeClass__XZ() {
    return &CFrameWnd::classCFrameWnd;
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

    pThis->Attach(hWnd);

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
int CFrameWnd::OnCreate(CREATESTRUCTW* lpcs) {
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

void CFrameWnd::BeginModalState() {
    auto& state = GetFrameRuntimeState(this);
    CWnd* owner=GetTopLevelOwner();
    HWND hOwner=owner ? owner->GetSafeHwnd() : nullptr;
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
int CFrameWnd::CanEnterHelpMode() {
    return m_hWnd ? TRUE : FALSE;
}

CWnd* CFrameWnd::CreateView(CCreateContext* pContext, unsigned int nID) {
    if (!m_hWnd) return nullptr;
    if (!pContext || !pContext->m_pNewViewClass) {
        HWND existing = nID ? ::GetDlgItem(m_hWnd, static_cast<int>(nID)) : nullptr;
        return existing ? CWnd::FromHandle(existing) : nullptr;
    }

    auto* viewClass = pContext->m_pNewViewClass;
    if (!viewClass->IsDerivedFrom(RUNTIME_CLASS(CView))) return nullptr;
    auto* view = static_cast<CView*>(viewClass->CreateObject());
    if (!view) return nullptr;

    openmfc::detail::WindowCreationOwner owner(view);
    if (auto* document = pContext->m_pCurrentDoc) document->AddView(view);
    view->m_bFrameOwnsView = TRUE;
    RECT bounds{};
    ::GetClientRect(m_hWnd, &bounds);
    if (!view->Create(nullptr, nullptr, WS_CHILD | WS_VISIBLE | WS_BORDER,
                      bounds, this, nID, pContext)) return nullptr;
    return owner.Release();
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


const wchar_t* CFrameWnd::GetIconWndClass(DWORD dwDefaultStyle, unsigned int nIDResource) {
    (void)dwDefaultStyle;
    (void)nIDResource;
    return g_szOpenMFCClass;
}

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

BOOL CFrameWnd::LoadAccelTable(const wchar_t* lpszAccelTable) {
    m_hAccelTable = nullptr;
    if (!lpszAccelTable) {
        return FALSE;
    }
    m_hAccelTable = ::LoadAcceleratorsW(AfxGetResourceHandle(), lpszAccelTable);
    return m_hAccelTable != nullptr;
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
    return pBar && pBar->IsVisible();
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

    return pClient || !pContext || !pContext->m_pNewViewClass;
}
int CFrameWnd::OnCreateHelper(CREATESTRUCTW* lpcs, CCreateContext* pContext) {
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

void CFrameWnd::OnHScroll(unsigned int nSBCode, unsigned int nPos, CScrollBar* pScrollBar) {
    if (pScrollBar && pScrollBar->m_hWnd) {
        ::SendMessageW(pScrollBar->m_hWnd, WM_HSCROLL, MAKELONG(nPos, nSBCode), 0);
    }
    if (m_pViewActive) {
        m_pViewActive->OnHScroll(nSBCode, nPos, pScrollBar);
    }
}
void CFrameWnd::OnIdleUpdateCmdUI() {
    using namespace openmfc::detail::dock;
    constexpr DWORD idleMenu = 0x01;
    constexpr DWORD idleTitle = 0x02;
    auto& flags = m_retailTail.m_dwBorderFlags;
    if (flags & idleMenu) {
        flags &= ~idleMenu;
        OnUpdateFrameMenu(m_retailTail.m_hMenuAlt);
    }
    if (flags & idleTitle) OnUpdateFrameTitle(TRUE);
    if (flags & idleLayout) {
        RecalcLayout((flags & idleNotify) != 0);
        ::UpdateWindow(m_hWnd);
    }
    if (m_nIDTracking != m_nIDLastMessage) ::SendMessageW(m_hWnd, 0x0362, m_nIDTracking, 0);
    flags = 0;
}
LRESULT CFrameWnd::OnIdleUpdateCmdUIMessage(WPARAM, LPARAM) {
    OnIdleUpdateCmdUI();
    return 0;
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
    const UINT previous = m_nIDLastMessage;
    UINT id = static_cast<UINT>(wParam);
    m_nFlags &= ~0x40;
    if (auto* messageBar = GetMessageBar()) {
        CString message;
        auto* text = reinterpret_cast<const wchar_t*>(lParam);
        if (!text && id) {
            if (id == 0xef06 && m_retailTail.m_lpfnCloseProc) id += 0xff;
            GetMessageString(id, message);
            text = message.GetString();
        }
        ::SetWindowTextW(messageBar->m_hWnd, text);
        if (auto* frame = messageBar->GetParentFrame()) {
            frame->m_nIDTracking = id;
            frame->m_nIDLastMessage = id;
        }
    }
    m_nIDTracking = id;
    m_nIDLastMessage = id;
    return previous;
}
void CFrameWnd::OnSetPreviewMode(int bPreview, CPrintPreviewState* pState) {
    // Entering print preview hides the frame's control bars so the preview view
    // owns the client area; leaving it restores them. This is the real,
    // observable frame behavior for WM print-preview mode.
    (void)pState;
    if (!m_hWnd) {
        return;
    }
    for (auto* node = ControlBars(this).m_pNodeHead; node; node = node->pNext) {
        auto* pBar = static_cast<CControlBar*>(node->data);
        if (pBar) {
            ShowControlBar(pBar, bPreview ? FALSE : TRUE, FALSE);
        }
    }
}

void CFrameWnd::OnSize(unsigned int nType, int cx, int cy) {
    (void)cx;
    (void)cy;
    Default();
    if (nType != SIZE_MINIMIZED) RecalcLayout(TRUE);
}
void CFrameWnd::OnSysCommand(unsigned int nID, __int64 lParam) {
    (void)lParam;
    if (!m_hWnd) {
        return;
    }

    const unsigned int command = nID & 0xFFF0;
    if (command == SC_CLOSE) {
        ::SendMessageW(m_hWnd, WM_CLOSE, 0, 0);
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
