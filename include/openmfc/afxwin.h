#pragma once
#include "afx.h"
#include "afxstr.h"
#include <cstring>
#include <cstdint>

// Windows types - use minimal forward declarations when possible
#if defined(_WIN32) || defined(__MINGW32__)
    #include <windows.h>
    #undef LoadString
    #ifndef _WINDOWS_
        #define _WINDOWS_
    #endif
#endif

#ifndef _WINDOWS_
    typedef void* HWND;
    typedef void* HDC;
    typedef void* HMENU;
    typedef void* HINSTANCE;
    typedef unsigned int UINT;
    typedef int BOOL;
    typedef long LONG;
    typedef unsigned long DWORD;
    typedef long long LONG_PTR;
    typedef unsigned long long ULONG_PTR;
    typedef ULONG_PTR UINT_PTR;
    typedef LONG_PTR LRESULT;
    typedef ULONG_PTR WPARAM;
    typedef LONG_PTR LPARAM;
    typedef const wchar_t* LPCWSTR;
    typedef wchar_t* LPWSTR;
    #define FALSE 0
    #define TRUE 1
    
    // Function pointer types
    typedef LRESULT (*WNDPROC)(HWND, UINT, WPARAM, LPARAM);
    
    // Basic rect structure
    struct tagRECT {
        LONG left, top, right, bottom;
    };
    typedef struct tagRECT RECT;
    typedef RECT* LPRECT;
    typedef const RECT* LPCRECT;
    
    // Point structure  
    struct POINT {
        LONG x, y;
    };
#endif

// Forward declarations
class CWnd;
class CWinThread;
class CWinApp;
class CDC;
class CMenu; // Added forward declaration
class CMDIFrameWnd; // Forward declaration

//=============================================================================
// CException and derived classes
//=============================================================================

class CException : public CObject {
    DECLARE_DYNAMIC(CException)
public:
    CException(int bAutoDelete = 1) : m_bAutoDelete(bAutoDelete) {}
    virtual ~CException() = default;
    
    virtual int GetErrorMessage(wchar_t* lpszError, unsigned int nMaxError, unsigned int* pnHelpContext = nullptr) const {
        (void)lpszError; (void)nMaxError; (void)pnHelpContext;
        return 0; // FALSE
    }
    
    void Delete() { if (m_bAutoDelete) delete this; }
    
protected:
    int m_bAutoDelete;
};

// IMPLEMENT_DYNAMIC(CException, CObject) moved to appcore.cpp

class CMemoryException : public CException {
    DECLARE_DYNAMIC(CMemoryException)
public:
    CMemoryException() : CException(0) {} // Memory exceptions are not auto-deleted
};

// IMPLEMENT_DYNAMIC(CMemoryException, CException) moved to appcore.cpp

class CFileException : public CException {
    DECLARE_DYNAMIC(CFileException)
public:
    enum {
        none,
        genericException,
        fileNotFound,
        badPath,
        tooManyOpenFiles,
        accessDenied,
        invalidFile,
        removeCurrentDir,
        directoryFull,
        badSeek,
        hardIO,
        sharingViolation,
        lockViolation,
        diskFull,
        endOfFile
    };
    
    CFileException(int cause = none, long lOsError = -1) 
        : m_cause(cause), m_lOsError(lOsError) {}
    
    int m_cause;
    long m_lOsError;
    CString m_strFileName;
};

// IMPLEMENT_DYNAMIC(CFileException, CException) moved to appcore.cpp

//=============================================================================
// CCmdTarget - base for command message handling
//=============================================================================

class CCmdTarget : public CObject {
    DECLARE_DYNAMIC(CCmdTarget)
public:
    CCmdTarget() = default;
    virtual ~CCmdTarget() = default;
    
    // Message map support (simplified)
    virtual int OnCmdMsg(unsigned int nID, int nCode, void* pExtra, void* pHandlerInfo) {
        (void)nID; (void)nCode; (void)pExtra; (void)pHandlerInfo;
        return 0; // FALSE
    }
    
protected:
    // Padding to reach correct size
    char _padding[24];
};

// IMPLEMENT_DYNAMIC(CCmdTarget, CObject) moved to appcore.cpp

//=============================================================================
// CWnd - window wrapper class
// sizeof(CWnd) = 232
// m_hWnd at offset 64
//=============================================================================

class CWnd : public CCmdTarget {
    DECLARE_DYNAMIC(CWnd)
    
public:
    CWnd();
    virtual ~CWnd();
    
    //-------------------------------------------------------------------------
    // Window handle access
    //-------------------------------------------------------------------------
    HWND GetSafeHwnd() const { return this != nullptr ? m_hWnd : nullptr; }
    operator HWND() const { return m_hWnd; }
    
    //-------------------------------------------------------------------------
    // Window creation/destruction
    //-------------------------------------------------------------------------
    virtual int Create(const wchar_t* lpszClassName, const wchar_t* lpszWindowName, DWORD dwStyle,
                       const struct tagRECT& rect, CWnd* pParentWnd, unsigned int nID, void* pContext = nullptr);
    
    virtual int CreateEx(DWORD dwExStyle, const wchar_t* lpszClassName, const wchar_t* lpszWindowName,
                         DWORD dwStyle, int x, int y, int nWidth, int nHeight,
                         HWND hWndParent, HMENU nIDorHMenu, void* lpParam = nullptr);
    
    virtual int DestroyWindow();
    
    //-------------------------------------------------------------------------
    // Window state
    //-------------------------------------------------------------------------
    int ShowWindow(int nCmdShow);
    int UpdateWindow();
    int IsWindowVisible() const;
    int IsWindowEnabled() const;
    int EnableWindow(int bEnable = 1);
    
    //-------------------------------------------------------------------------
    // Window size and position
    //-------------------------------------------------------------------------
    void GetWindowRect(struct tagRECT* lpRect) const;
    void GetClientRect(struct tagRECT* lpRect) const;
    void MoveWindow(int x, int y, int nWidth, int nHeight, int bRepaint = 1);
    void MoveWindow(const struct tagRECT* lpRect, int bRepaint = 1);
    int SetWindowPos(const CWnd* pWndInsertAfter, int x, int y, int cx, int cy, unsigned int nFlags);
    
    //-------------------------------------------------------------------------
    // Window text
    //-------------------------------------------------------------------------
    void SetWindowTextW(const wchar_t* lpszString);
    int GetWindowTextW(wchar_t* lpszStringBuf, int nMaxCount) const;
    int GetWindowTextLengthW() const;
    
    //-------------------------------------------------------------------------
    // Parent/child relationships
    //-------------------------------------------------------------------------
    CWnd* GetParent() const;
    CWnd* SetParent(CWnd* pWndNewParent);
    CWnd* GetDlgItem(int nID) const;
    
    //-------------------------------------------------------------------------
    // Painting
    //-------------------------------------------------------------------------
    void Invalidate(int bErase = 1);
    void InvalidateRect(const struct tagRECT* lpRect, int bErase = 1);
    int RedrawWindow(const struct tagRECT* lpRectUpdate = nullptr, void* prgnUpdate = nullptr, unsigned int flags = 0);
    
    //-------------------------------------------------------------------------
    // Message handling
    //-------------------------------------------------------------------------
    intptr_t SendMessageW(unsigned int message, uintptr_t wParam = 0, intptr_t lParam = 0);
    int PostMessageW(unsigned int message, uintptr_t wParam = 0, intptr_t lParam = 0);
    
    //-------------------------------------------------------------------------
    // Window procedure
    //-------------------------------------------------------------------------
    virtual intptr_t WindowProc(unsigned int message, uintptr_t wParam, intptr_t lParam);
    virtual int OnCommand(uintptr_t wParam, intptr_t lParam);
    virtual int OnNotify(uintptr_t wParam, intptr_t lParam, intptr_t* pResult);
    
    //-------------------------------------------------------------------------
    // Message handlers (overridable)
    //-------------------------------------------------------------------------
    virtual int OnWndMsg(unsigned int message, uintptr_t wParam, intptr_t lParam, intptr_t* pResult);
    virtual void OnFinalRelease();
    
    // Timer
    uintptr_t SetTimer(uintptr_t nIDEvent, unsigned int nElapse, void* lpfnTimer);
    int KillTimer(uintptr_t nIDEvent);
    
public:
    // Window handle - at offset 64 from start of CWnd
    // Padding to ensure m_hWnd is at the right offset
    // CCmdTarget adds: vptr (8) + CObject data (0) + CCmdTarget data (24) = 32 bytes
    // Need 32 more bytes of padding to reach offset 64
    char _cwnd_padding1[32];
    
    HWND m_hWnd;
    
    // Additional CWnd members to reach sizeof = 232
    WNDPROC m_pfnSuper;         // 8 bytes - window superclass procedure
    DWORD m_nFlags;              // 4 bytes - internal flags
    DWORD m_dwStyle;             // 4 bytes - cached window style
    
    // Platform-specific padding to reach 232 bytes total
    // On Windows/MinGW: DWORD is 4 bytes, need 144 bytes padding
    // On Linux: DWORD typedef is 8 bytes (unsigned long), need 136 bytes padding
#if defined(_WIN32) || defined(__MINGW32__)
    char _cwnd_padding2[144];    
#else
    char _cwnd_padding2[136];    
#endif
};

// Verify layout
static_assert(sizeof(CWnd) == 232, "CWnd must be 232 bytes");

// IMPLEMENT_DYNAMIC(CWnd, CCmdTarget) moved to appcore.cpp

//=============================================================================
// CFrameWnd - Main frame window
//=============================================================================

class CFrameWnd : public CWnd {
    DECLARE_DYNCREATE(CFrameWnd)
public:
    CFrameWnd();
    virtual ~CFrameWnd() = default;

    virtual int LoadFrame(unsigned int nIDResource, DWORD dwDefaultStyle = 0,
                          CWnd* pParentWnd = nullptr, void* pContext = nullptr);
                          
    virtual int Create(const wchar_t* lpszClassName, const wchar_t* lpszWindowName,
                       DWORD dwStyle = 0, const struct tagRECT& rect = {},
                       CWnd* pParentWnd = nullptr, const wchar_t* lpszMenuName = nullptr,
                       DWORD dwExStyle = 0, void* pContext = nullptr);

    virtual void ActivateFrame(int nCmdShow = -1);
    virtual void RecalcLayout(int bNotify = 1);

    CWnd* GetActiveView() const;
    void SetActiveView(CWnd* pViewNew, int bNotify = 1);

    // Implementation helpers
    virtual int OnCreate(void* lpCreateStruct); // LPCREATESTRUCT
    
public:
    CWnd* m_pViewActive;
    
protected:
    char _framewnd_padding[32]; // Padding for member variables
};

//=============================================================================
// CMDIFrameWnd - MDI Frame Window
//=============================================================================

class CMDIFrameWnd : public CFrameWnd {
    DECLARE_DYNCREATE(CMDIFrameWnd)
public:
    CMDIFrameWnd();
    virtual ~CMDIFrameWnd() = default;

    virtual int CreateClient(void* lpCreateStruct, CMenu* pWindowMenu);
    virtual HWND GetWindowMenuPopup(HMENU hMenuBar);
    
    // MDI helpers
    void MDIActivate(CWnd* pWndActivate);
    CWnd* MDIGetActive(int* pbMaximized = nullptr) const;
    void MDIIconArrange();
    void MDIMaximize(CWnd* pWnd);
    void MDINext();
    void MDIRestore(CWnd* pWnd);
    void MDISetMenu(CMenu* pFrameMenu, CMenu* pWindowMenu);
    void MDITile(int nType);
    void MDICascade(int nType);
    
public:
    HWND m_hWndMDIClient; // Handle to MDI client window
    
protected:
    char _mdiframe_padding[32];
};

//=============================================================================
// CMDIChildWnd - MDI Child Window
//=============================================================================

class CMDIChildWnd : public CFrameWnd {
    DECLARE_DYNCREATE(CMDIChildWnd)
public:
    CMDIChildWnd();
    virtual ~CMDIChildWnd() = default;

    virtual int Create(const wchar_t* lpszClassName, const wchar_t* lpszWindowName,
                       DWORD dwStyle = 0, const struct tagRECT& rect = {},
                       CMDIFrameWnd* pParentWnd = nullptr, void* pContext = nullptr);
                       
    virtual void ActivateFrame(int nCmdShow = -1);
    virtual int DestroyWindow();

    CMDIFrameWnd* GetMDIFrame();
    
protected:
    char _mdichild_padding[32];
};

//=============================================================================
// CDialog - Modal and Modeless Dialogs
//=============================================================================

class CDialog : public CWnd {
    DECLARE_DYNAMIC(CDialog)
public:
    CDialog();
    explicit CDialog(const wchar_t* lpszTemplateName, CWnd* pParentWnd = nullptr);
    explicit CDialog(unsigned int nIDTemplate, CWnd* pParentWnd = nullptr);
    virtual ~CDialog() = default;

    virtual intptr_t DoModal();
    virtual int Create(const wchar_t* lpszTemplateName, CWnd* pParentWnd = nullptr);
    virtual int Create(unsigned int nIDTemplate, CWnd* pParentWnd = nullptr);

    virtual int OnInitDialog();
    virtual void OnOK();
    virtual void OnCancel();
    void EndDialog(int nResult);

protected:
    // Overridables
    virtual void OnSetFont(CWnd* pFont);
    
public:
    const wchar_t* m_lpszTemplateName;
    unsigned int m_nIDHelp;
    
protected:
    char _dialog_padding[32];
};

//=============================================================================
// CDialogEx - Extended Dialog
//=============================================================================

class CDialogEx : public CDialog {
    DECLARE_DYNAMIC(CDialogEx)
public:
    CDialogEx();
    explicit CDialogEx(unsigned int nIDTemplate, CWnd* pParentWnd = nullptr);
    virtual ~CDialogEx() = default;

    void SetBackgroundColor(unsigned long color, int bRepaint = 1);
    void SetBackgroundImage(void* hBitmap, int nStyle = 0, int bRepaint = 1);

protected:
    unsigned long m_clrBackground;
    void* m_hBackgroundImage;
    char _dialogex_padding[32];
};

//=============================================================================
// CWinThread and CWinApp
//=============================================================================

class CWinThread : public CCmdTarget {
    DECLARE_DYNAMIC(CWinThread)
public:
    CWinThread() : m_pMainWnd(nullptr), m_nThreadID(0) {}
    virtual ~CWinThread() = default;
    
    virtual int InitInstance() { return 1; }
    virtual int ExitInstance() { return 0; }
    virtual int Run();
    
    CWnd* m_pMainWnd;
    DWORD m_nThreadID;
    
protected:
    char _winthread_padding[64];
};

// IMPLEMENT_DYNAMIC(CWinThread, CCmdTarget) moved to appcore.cpp

inline int CWinThread::Run() {
    // Simplified message loop
    return 0;
}

class CWinApp : public CWinThread {
    DECLARE_DYNAMIC(CWinApp)
public:
    CWinApp(const wchar_t* lpszAppName = nullptr);
    virtual ~CWinApp() = default;
    
    virtual int InitApplication() { return 1; }
    virtual int InitInstance() override { return 1; }
    virtual int ExitInstance() override { return 0; }
    virtual int Run() override;
    
    // Application info
    const wchar_t* m_pszAppName;
    const wchar_t* m_pszExeName;
    const wchar_t* m_pszHelpFilePath;
    const wchar_t* m_pszProfileName;
    HINSTANCE m_hInstance;
    
protected:
    char _winapp_padding[256];
};

// IMPLEMENT_DYNAMIC(CWinApp, CWinThread) moved to appcore.cpp

// CWinApp constructor is in appcore.cpp

inline int CWinApp::Run() {
    return CWinThread::Run();
}

// Global application pointer
extern CWinApp* AFXAPI AfxGetApp();
extern CWinThread* AFXAPI AfxGetThread();
extern HINSTANCE AFXAPI AfxGetInstanceHandle();
extern HINSTANCE AFXAPI AfxGetResourceHandle();
extern void AFXAPI AfxSetResourceHandle(HINSTANCE hInstResource);
extern CWnd* AFXAPI AfxGetMainWnd();
extern int AFXAPI AfxWinInit(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow);

// Exception helpers
extern void AFXAPI AfxThrowMemoryException();
extern void AFXAPI AfxThrowFileException(int cause, long lOsError = -1, const wchar_t* lpszFileName = nullptr);
extern void AFXAPI AfxThrowInvalidArgException();
extern void AFXAPI AfxThrowNotSupportedException();
extern void AFXAPI AfxThrowResourceException();
extern void AFXAPI AfxThrowUserException();
extern void AFXAPI AfxAbort();

//=============================================================================
// CWnd implementation (inline for header-only)
//=============================================================================

inline CWnd::CWnd() 
    : m_hWnd(nullptr)
    , m_pfnSuper(nullptr)
    , m_nFlags(0)
    , m_dwStyle(0)
{
    // Zero padding
    memset(_cwnd_padding1, 0, sizeof(_cwnd_padding1));
    memset(_cwnd_padding2, 0, sizeof(_cwnd_padding2));
}

inline CWnd::~CWnd() {
    if (m_hWnd != nullptr) {
        DestroyWindow();
    }
}

inline int CWnd::Create(const wchar_t* lpszClassName, const wchar_t* lpszWindowName, DWORD dwStyle,
                         const struct tagRECT& rect, CWnd* pParentWnd, unsigned int nID, void* pContext) {
    (void)lpszClassName; (void)lpszWindowName; (void)dwStyle;
    (void)rect; (void)pParentWnd; (void)nID; (void)pContext;
    // Stub - needs Win32 CreateWindowExW implementation
    return 0;
}

inline int CWnd::CreateEx(DWORD dwExStyle, const wchar_t* lpszClassName, const wchar_t* lpszWindowName,
                           DWORD dwStyle, int x, int y, int nWidth, int nHeight,
                           HWND hWndParent, HMENU nIDorHMenu, void* lpParam) {
    (void)dwExStyle; (void)lpszClassName; (void)lpszWindowName; (void)dwStyle;
    (void)x; (void)y; (void)nWidth; (void)nHeight;
    (void)hWndParent; (void)nIDorHMenu; (void)lpParam;
    // Stub - simulate success for testing
    m_hWnd = (HWND)(uintptr_t)0x1234; // Dummy handle
    return 1;
}

inline int CWnd::DestroyWindow() {
    if (m_hWnd == nullptr) return 0;
    // Stub - needs Win32 DestroyWindow
    m_hWnd = nullptr;
    return 1;
}

inline int CWnd::ShowWindow(int nCmdShow) {
    (void)nCmdShow;
    // Stub - needs Win32 ShowWindow
    return 1;
}

inline int CWnd::UpdateWindow() {
    // Stub - needs Win32 UpdateWindow
    return 1;
}

inline int CWnd::IsWindowVisible() const {
    // Stub
    return m_hWnd != nullptr;
}

inline int CWnd::IsWindowEnabled() const {
    // Stub
    return 1;
}

inline int CWnd::EnableWindow(int bEnable) {
    (void)bEnable;
    // Stub
    return 1;
}

inline void CWnd::GetWindowRect(struct tagRECT* lpRect) const {
    if (lpRect) {
        lpRect->left = lpRect->top = lpRect->right = lpRect->bottom = 0;
    }
}

inline void CWnd::GetClientRect(struct tagRECT* lpRect) const {
    if (lpRect) {
        lpRect->left = lpRect->top = lpRect->right = lpRect->bottom = 0;
    }
}

inline void CWnd::MoveWindow(int x, int y, int nWidth, int nHeight, int bRepaint) {
    (void)x; (void)y; (void)nWidth; (void)nHeight; (void)bRepaint;
    // Stub
}

inline void CWnd::MoveWindow(const struct tagRECT* lpRect, int bRepaint) {
    if (lpRect) {
        MoveWindow(lpRect->left, lpRect->top, 
                  lpRect->right - lpRect->left, lpRect->bottom - lpRect->top, bRepaint);
    }
}

inline int CWnd::SetWindowPos(const CWnd* pWndInsertAfter, int x, int y, int cx, int cy, unsigned int nFlags) {
    (void)pWndInsertAfter; (void)x; (void)y; (void)cx; (void)cy; (void)nFlags;
    // Stub
    return 1;
}

inline void CWnd::SetWindowTextW(const wchar_t* lpszString) {
    (void)lpszString;
    // Stub
}

inline int CWnd::GetWindowTextW(wchar_t* lpszStringBuf, int nMaxCount) const {
    if (lpszStringBuf && nMaxCount > 0) {
        lpszStringBuf[0] = L'\0';
    }
    return 0;
}

inline int CWnd::GetWindowTextLengthW() const {
    return 0;
}

inline CWnd* CWnd::GetParent() const {
    // Stub
    return nullptr;
}

inline CWnd* CWnd::SetParent(CWnd* pWndNewParent) {
    (void)pWndNewParent;
    // Stub
    return nullptr;
}

inline CWnd* CWnd::GetDlgItem(int nID) const {
    (void)nID;
    // Stub
    return nullptr;
}

inline void CWnd::Invalidate(int bErase) {
    (void)bErase;
    // Stub
}

inline void CWnd::InvalidateRect(const struct tagRECT* lpRect, int bErase) {
    (void)lpRect; (void)bErase;
    // Stub
}

inline int CWnd::RedrawWindow(const struct tagRECT* lpRectUpdate, void* prgnUpdate, unsigned int flags) {
    (void)lpRectUpdate; (void)prgnUpdate; (void)flags;
    // Stub
    return 1;
}

inline intptr_t CWnd::SendMessageW(unsigned int message, uintptr_t wParam, intptr_t lParam) {
    (void)message; (void)wParam; (void)lParam;
    // Stub
    return 0;
}

inline int CWnd::PostMessageW(unsigned int message, uintptr_t wParam, intptr_t lParam) {
    (void)message; (void)wParam; (void)lParam;
    // Stub  
    return 1;
}

inline intptr_t CWnd::WindowProc(unsigned int message, uintptr_t wParam, intptr_t lParam) {
    intptr_t lResult = 0;
    if (!OnWndMsg(message, wParam, lParam, &lResult)) {
        // Default handling
    }
    return lResult;
}

inline int CWnd::OnCommand(uintptr_t wParam, intptr_t lParam) {
    (void)wParam; (void)lParam;
    return 0;
}

inline int CWnd::OnNotify(uintptr_t wParam, intptr_t lParam, intptr_t* pResult) {
    (void)wParam; (void)lParam; (void)pResult;
    return 0;
}

inline int CWnd::OnWndMsg(unsigned int message, uintptr_t wParam, intptr_t lParam, intptr_t* pResult) {
    (void)message; (void)wParam; (void)lParam; (void)pResult;
    return 0;
}

inline void CWnd::OnFinalRelease() {
    // Override in derived classes
}

inline uintptr_t CWnd::SetTimer(uintptr_t nIDEvent, unsigned int nElapse, void* lpfnTimer) {
    (void)nIDEvent; (void)nElapse; (void)lpfnTimer;
    // Stub
    return 0;
}

inline int CWnd::KillTimer(uintptr_t nIDEvent) {
    (void)nIDEvent;
    // Stub
    return 1;
}
