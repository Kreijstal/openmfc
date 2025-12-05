#pragma once
#include "afx.h"
#include "afxstr.h"
#include <cstring>

// Windows types - use minimal forward declarations when possible
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
    struct RECT {
        LONG left, top, right, bottom;
    };
    typedef RECT* LPRECT;
    typedef const RECT* LPCRECT;
    
    // Point structure  
    struct POINT {
        LONG x, y;
    };
#else
    #include <windows.h>
#endif

// Forward declarations
class CWnd;
class CWinThread;
class CWinApp;
class CDC;

//=============================================================================
// CException and derived classes
//=============================================================================

class CException : public CObject {
    DECLARE_DYNAMIC(CException)
public:
    CException(BOOL bAutoDelete = TRUE) : m_bAutoDelete(bAutoDelete) {}
    virtual ~CException() = default;
    
    virtual BOOL GetErrorMessage(wchar_t* lpszError, UINT nMaxError, UINT* pnHelpContext = nullptr) const {
        (void)lpszError; (void)nMaxError; (void)pnHelpContext;
        return FALSE;
    }
    
    void Delete() { if (m_bAutoDelete) delete this; }
    
protected:
    BOOL m_bAutoDelete;
};

IMPLEMENT_DYNAMIC(CException, CObject)

class CMemoryException : public CException {
    DECLARE_DYNAMIC(CMemoryException)
public:
    CMemoryException() : CException(FALSE) {} // Memory exceptions are not auto-deleted
};

IMPLEMENT_DYNAMIC(CMemoryException, CException)

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
    
    CFileException(int cause = none, LONG lOsError = -1) 
        : m_cause(cause), m_lOsError(lOsError) {}
    
    int m_cause;
    LONG m_lOsError;
    CString m_strFileName;
};

IMPLEMENT_DYNAMIC(CFileException, CException)

//=============================================================================
// CCmdTarget - base for command message handling
//=============================================================================

class CCmdTarget : public CObject {
    DECLARE_DYNAMIC(CCmdTarget)
public:
    CCmdTarget() = default;
    virtual ~CCmdTarget() = default;
    
    // Message map support (simplified)
    virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, void* pHandlerInfo) {
        (void)nID; (void)nCode; (void)pExtra; (void)pHandlerInfo;
        return FALSE;
    }
    
protected:
    // Padding to reach correct size
    char _padding[24];
};

IMPLEMENT_DYNAMIC(CCmdTarget, CObject)

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
    virtual BOOL Create(LPCWSTR lpszClassName, LPCWSTR lpszWindowName, DWORD dwStyle,
                       const RECT& rect, CWnd* pParentWnd, UINT nID, void* pContext = nullptr);
    
    virtual BOOL CreateEx(DWORD dwExStyle, LPCWSTR lpszClassName, LPCWSTR lpszWindowName,
                         DWORD dwStyle, int x, int y, int nWidth, int nHeight,
                         HWND hWndParent, HMENU nIDorHMenu, void* lpParam = nullptr);
    
    virtual BOOL DestroyWindow();
    
    //-------------------------------------------------------------------------
    // Window state
    //-------------------------------------------------------------------------
    BOOL ShowWindow(int nCmdShow);
    BOOL UpdateWindow();
    BOOL IsWindowVisible() const;
    BOOL IsWindowEnabled() const;
    BOOL EnableWindow(BOOL bEnable = TRUE);
    
    //-------------------------------------------------------------------------
    // Window size and position
    //-------------------------------------------------------------------------
    void GetWindowRect(LPRECT lpRect) const;
    void GetClientRect(LPRECT lpRect) const;
    void MoveWindow(int x, int y, int nWidth, int nHeight, BOOL bRepaint = TRUE);
    void MoveWindow(LPCRECT lpRect, BOOL bRepaint = TRUE);
    BOOL SetWindowPos(const CWnd* pWndInsertAfter, int x, int y, int cx, int cy, UINT nFlags);
    
    //-------------------------------------------------------------------------
    // Window text
    //-------------------------------------------------------------------------
    void SetWindowTextW(LPCWSTR lpszString);
    int GetWindowTextW(LPWSTR lpszStringBuf, int nMaxCount) const;
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
    void Invalidate(BOOL bErase = TRUE);
    void InvalidateRect(LPCRECT lpRect, BOOL bErase = TRUE);
    BOOL RedrawWindow(LPCRECT lpRectUpdate = nullptr, void* prgnUpdate = nullptr, UINT flags = 0);
    
    //-------------------------------------------------------------------------
    // Message handling
    //-------------------------------------------------------------------------
    LRESULT SendMessageW(UINT message, WPARAM wParam = 0, LPARAM lParam = 0);
    BOOL PostMessageW(UINT message, WPARAM wParam = 0, LPARAM lParam = 0);
    
    //-------------------------------------------------------------------------
    // Window procedure
    //-------------------------------------------------------------------------
    virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
    virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
    virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
    
    //-------------------------------------------------------------------------
    // Message handlers (overridable)
    //-------------------------------------------------------------------------
    virtual BOOL OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);
    virtual void OnFinalRelease();
    
    // Timer
    UINT_PTR SetTimer(UINT_PTR nIDEvent, UINT nElapse, void* lpfnTimer);
    BOOL KillTimer(UINT_PTR nIDEvent);
    
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

IMPLEMENT_DYNAMIC(CWnd, CCmdTarget)

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

inline BOOL CWnd::Create(LPCWSTR lpszClassName, LPCWSTR lpszWindowName, DWORD dwStyle,
                         const RECT& rect, CWnd* pParentWnd, UINT nID, void* pContext) {
    (void)lpszClassName; (void)lpszWindowName; (void)dwStyle;
    (void)rect; (void)pParentWnd; (void)nID; (void)pContext;
    // Stub - needs Win32 CreateWindowExW implementation
    return FALSE;
}

inline BOOL CWnd::CreateEx(DWORD dwExStyle, LPCWSTR lpszClassName, LPCWSTR lpszWindowName,
                           DWORD dwStyle, int x, int y, int nWidth, int nHeight,
                           HWND hWndParent, HMENU nIDorHMenu, void* lpParam) {
    (void)dwExStyle; (void)lpszClassName; (void)lpszWindowName; (void)dwStyle;
    (void)x; (void)y; (void)nWidth; (void)nHeight;
    (void)hWndParent; (void)nIDorHMenu; (void)lpParam;
    // Stub - needs Win32 CreateWindowExW implementation
    return FALSE;
}

inline BOOL CWnd::DestroyWindow() {
    if (m_hWnd == nullptr) return FALSE;
    // Stub - needs Win32 DestroyWindow
    m_hWnd = nullptr;
    return TRUE;
}

inline BOOL CWnd::ShowWindow(int nCmdShow) {
    (void)nCmdShow;
    // Stub - needs Win32 ShowWindow
    return TRUE;
}

inline BOOL CWnd::UpdateWindow() {
    // Stub - needs Win32 UpdateWindow
    return TRUE;
}

inline BOOL CWnd::IsWindowVisible() const {
    // Stub
    return m_hWnd != nullptr;
}

inline BOOL CWnd::IsWindowEnabled() const {
    // Stub
    return TRUE;
}

inline BOOL CWnd::EnableWindow(BOOL bEnable) {
    (void)bEnable;
    // Stub
    return TRUE;
}

inline void CWnd::GetWindowRect(LPRECT lpRect) const {
    if (lpRect) {
        lpRect->left = lpRect->top = lpRect->right = lpRect->bottom = 0;
    }
}

inline void CWnd::GetClientRect(LPRECT lpRect) const {
    if (lpRect) {
        lpRect->left = lpRect->top = lpRect->right = lpRect->bottom = 0;
    }
}

inline void CWnd::MoveWindow(int x, int y, int nWidth, int nHeight, BOOL bRepaint) {
    (void)x; (void)y; (void)nWidth; (void)nHeight; (void)bRepaint;
    // Stub
}

inline void CWnd::MoveWindow(LPCRECT lpRect, BOOL bRepaint) {
    if (lpRect) {
        MoveWindow(lpRect->left, lpRect->top, 
                  lpRect->right - lpRect->left, lpRect->bottom - lpRect->top, bRepaint);
    }
}

inline BOOL CWnd::SetWindowPos(const CWnd* pWndInsertAfter, int x, int y, int cx, int cy, UINT nFlags) {
    (void)pWndInsertAfter; (void)x; (void)y; (void)cx; (void)cy; (void)nFlags;
    // Stub
    return TRUE;
}

inline void CWnd::SetWindowTextW(LPCWSTR lpszString) {
    (void)lpszString;
    // Stub
}

inline int CWnd::GetWindowTextW(LPWSTR lpszStringBuf, int nMaxCount) const {
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

inline void CWnd::Invalidate(BOOL bErase) {
    (void)bErase;
    // Stub
}

inline void CWnd::InvalidateRect(LPCRECT lpRect, BOOL bErase) {
    (void)lpRect; (void)bErase;
    // Stub
}

inline BOOL CWnd::RedrawWindow(LPCRECT lpRectUpdate, void* prgnUpdate, UINT flags) {
    (void)lpRectUpdate; (void)prgnUpdate; (void)flags;
    // Stub
    return TRUE;
}

inline LRESULT CWnd::SendMessageW(UINT message, WPARAM wParam, LPARAM lParam) {
    (void)message; (void)wParam; (void)lParam;
    // Stub
    return 0;
}

inline BOOL CWnd::PostMessageW(UINT message, WPARAM wParam, LPARAM lParam) {
    (void)message; (void)wParam; (void)lParam;
    // Stub  
    return TRUE;
}

inline LRESULT CWnd::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) {
    LRESULT lResult = 0;
    if (!OnWndMsg(message, wParam, lParam, &lResult)) {
        // Default handling
    }
    return lResult;
}

inline BOOL CWnd::OnCommand(WPARAM wParam, LPARAM lParam) {
    (void)wParam; (void)lParam;
    return FALSE;
}

inline BOOL CWnd::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) {
    (void)wParam; (void)lParam; (void)pResult;
    return FALSE;
}

inline BOOL CWnd::OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult) {
    (void)message; (void)wParam; (void)lParam; (void)pResult;
    return FALSE;
}

inline void CWnd::OnFinalRelease() {
    // Override in derived classes
}

inline UINT_PTR CWnd::SetTimer(UINT_PTR nIDEvent, UINT nElapse, void* lpfnTimer) {
    (void)nIDEvent; (void)nElapse; (void)lpfnTimer;
    // Stub
    return 0;
}

inline BOOL CWnd::KillTimer(UINT_PTR nIDEvent) {
    (void)nIDEvent;
    // Stub
    return TRUE;
}

//=============================================================================
// CWinThread and CWinApp
//=============================================================================

class CWinThread : public CCmdTarget {
    DECLARE_DYNAMIC(CWinThread)
public:
    CWinThread() : m_pMainWnd(nullptr), m_nThreadID(0) {}
    virtual ~CWinThread() = default;
    
    virtual BOOL InitInstance() { return TRUE; }
    virtual int ExitInstance() { return 0; }
    virtual int Run();
    
    CWnd* m_pMainWnd;
    DWORD m_nThreadID;
    
protected:
    char _winthread_padding[64];
};

IMPLEMENT_DYNAMIC(CWinThread, CCmdTarget)

inline int CWinThread::Run() {
    // Simplified message loop
    return 0;
}

class CWinApp : public CWinThread {
    DECLARE_DYNAMIC(CWinApp)
public:
    CWinApp(LPCWSTR lpszAppName = nullptr);
    virtual ~CWinApp() = default;
    
    virtual BOOL InitApplication() { return TRUE; }
    virtual BOOL InitInstance() override { return TRUE; }
    virtual int ExitInstance() override { return 0; }
    virtual int Run() override;
    
    // Application info
    LPCWSTR m_pszAppName;
    LPCWSTR m_pszExeName;
    LPCWSTR m_pszHelpFilePath;
    LPCWSTR m_pszProfileName;
    HINSTANCE m_hInstance;
    
protected:
    char _winapp_padding[256];
};

IMPLEMENT_DYNAMIC(CWinApp, CWinThread)

inline CWinApp::CWinApp(LPCWSTR lpszAppName)
    : m_pszAppName(lpszAppName)
    , m_pszExeName(nullptr)
    , m_pszHelpFilePath(nullptr)
    , m_pszProfileName(nullptr)
    , m_hInstance(nullptr)
{
    memset(_winapp_padding, 0, sizeof(_winapp_padding));
}

inline int CWinApp::Run() {
    return CWinThread::Run();
}

// Global application pointer
extern CWinApp* AfxGetApp();
