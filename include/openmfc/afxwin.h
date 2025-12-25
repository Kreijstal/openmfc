#pragma once
#include "afx.h"
#include "afxstr.h"
#include <cstring>
#include <cstdint>

// Windows types - OpenMFC is Windows-only
#include <windows.h>
#include <commctrl.h>

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
    typedef POINT* LPPOINT;
    typedef const POINT* LPCPOINT;
    
    // Size structure
    struct SIZE {
        LONG cx, cy;
    };
    typedef SIZE* LPSIZE;
    typedef const SIZE* LPCSIZE;
    
    // Windows message constants (minimal set for MFC)
    #ifndef WM_NULL
    #define WM_NULL 0x0000
    #endif
    
    #ifndef WM_CREATE
    #define WM_CREATE 0x0001
    #endif
    
    #ifndef WM_DESTROY
    #define WM_DESTROY 0x0002
    #endif
    
    #ifndef WM_MOVE
    #define WM_MOVE 0x0003
    #endif
    
    #ifndef WM_SIZE
    #define WM_SIZE 0x0005
    #endif
    
    #ifndef WM_PAINT
    #define WM_PAINT 0x000F
    #endif
    
    #ifndef WM_CLOSE
    #define WM_CLOSE 0x0010
    #endif
    
    #ifndef WM_QUIT
    #define WM_QUIT 0x0012
    #endif
    
    #ifndef WM_MOUSEMOVE
    #define WM_MOUSEMOVE 0x0200
    #endif
    
    #ifndef WM_LBUTTONDOWN
    #define WM_LBUTTONDOWN 0x0201
    #endif
    
    #ifndef WM_LBUTTONUP
    #define WM_LBUTTONUP 0x0202
    #endif
    
    #ifndef WM_KEYDOWN
    #define WM_KEYDOWN 0x0100
    #endif
    
    #ifndef WM_KEYUP
    #define WM_KEYUP 0x0101
    #endif
    
    #ifndef WM_CHAR
    #define WM_CHAR 0x0102
    #endif
    
    #ifndef WM_COMMAND
    #define WM_COMMAND 0x0111
    #endif
    
    #ifndef WM_SYSCOMMAND
    #define WM_SYSCOMMAND 0x0112
    #endif
    
    #ifndef WM_TIMER
    #define WM_TIMER 0x0113
    #endif
    
    #ifndef WM_NCMOUSEMOVE
    #define WM_NCMOUSEMOVE 0x00A0
    #endif
    
    // Message structure for window messaging
    struct tagMSG {
        HWND hwnd;
        UINT message;
        WPARAM wParam;
        LPARAM lParam;
        DWORD time;
        POINT pt;
    };
    typedef tagMSG MSG, *PMSG, *LPMSG;
    
    // GDI handle types
    typedef void* HGDIOBJ;
    typedef void* HPEN;
    typedef void* HBRUSH;
    typedef void* HFONT;
    typedef void* HBITMAP;
    typedef void* HPALETTE;
    typedef void* HRGN;
    
    // Paint structure
    struct tagPAINTSTRUCT {
        HDC hdc;
        int fErase;
        RECT rcPaint;
        int fRestore;
        int fIncUpdate;
        char rgbReserved[32];
    };
    typedef struct tagPAINTSTRUCT PAINTSTRUCT;
#endif

// Forward declarations
class CWnd;
class CWinThread;
class CWinApp;
class CDC;
class CMenu; // Added forward declaration
class CMDIFrameWnd; // Forward declaration
struct CCreateContext; // MFC create context (affects MSVC mangling)
class CGdiObject;
class CPen;
class CBrush;
class CFont;
class CBitmap;
class CPalette;
class CRgn;
class CClientDC;
class CPaintDC;
class CWindowDC;
class CMetaFileDC;

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

// CArchiveException - archive exception
class CArchiveException : public CException {
    DECLARE_DYNAMIC(CArchiveException)
public:
    enum {
        none = 0,
        generic = 1,
        readOnly = 2,
        endOfFile = 3,
        writeOnly = 4,
        badIndex = 5,
        badClass = 6,
        badSchema = 7,
        badFormat = 8
    };
    
    CArchiveException(int cause = CArchiveException::none, const wchar_t* lpszArchiveName = nullptr);
    
    virtual int GetErrorMessage(wchar_t* lpszError, UINT nMaxError, UINT* pnHelpContext = nullptr) const override;
    
    int m_cause;
    CString m_strFileName;
};

// IMPLEMENT_DYNAMIC(CFileException, CException) moved to appcore.cpp

//=============================================================================
// Message Map Definitions
//=============================================================================

class CCmdTarget;

typedef void (AFXAPI CCmdTarget::*AFX_PMSG)(void);

struct AFX_MSGMAP_ENTRY
{
    UINT nMessage;   // windows message
    UINT nCode;      // control code or WM_NOTIFY code
    UINT nID;        // control ID (or 0 for windows messages)
    UINT nLastID;    // used for entries specifying a range of control id's
    UINT_PTR nSig;   // signature type (action) or pointer to message #
    AFX_PMSG pfn;    // routine to call (or special value)
};

struct AFX_MSGMAP
{
    const AFX_MSGMAP* (AFXAPI *pfnGetBaseMap)();
    const AFX_MSGMAP_ENTRY* lpEntries;
};

#define DECLARE_MESSAGE_MAP() \
protected: \
    static const AFX_MSGMAP* AFXAPI GetThisMessageMap(); \
    virtual const AFX_MSGMAP* GetMessageMap() const; \
    static const AFX_MSGMAP messageMap; \
    static const AFX_MSGMAP_ENTRY _messageEntries[]; \

#define BEGIN_MESSAGE_MAP(theClass, baseClass) \
    const AFX_MSGMAP* AFXAPI theClass::GetThisMessageMap() \
    { \
        return &theClass::messageMap; \
    } \
    const AFX_MSGMAP* theClass::GetMessageMap() const \
    { \
        return GetThisMessageMap(); \
    } \
    const AFX_MSGMAP theClass::messageMap = \
    { \
        &baseClass::GetThisMessageMap, \
        &theClass::_messageEntries[0] \
    }; \
    const AFX_MSGMAP_ENTRY theClass::_messageEntries[] = \
    { \

#define END_MESSAGE_MAP() \
        {0, 0, 0, 0, AfxSig_end, (AFX_PMSG)0 } \
    }; \

// Message map signature values
enum AfxSig
{
    AfxSig_end = 0,     // [marks end of message map]
    AfxSig_vv = 1,      // void (void)
};

//=============================================================================
// CCmdTarget - base for command message handling
//=============================================================================

class CCmdTarget : public CObject {
    DECLARE_DYNAMIC(CCmdTarget)
    DECLARE_MESSAGE_MAP()
public:
    CCmdTarget() = default;
    virtual ~CCmdTarget();
    
    // Message map support (simplified)
    virtual int OnCmdMsg(unsigned int nID, int nCode, void* pExtra, void* pHandlerInfo);
    
    // Static helper for dispatching
    static int PASCAL DispatchCmdMsg(CCmdTarget* pTarget, unsigned int nID, int nCode,
                                     AFX_PMSG pfn, void* pExtra, unsigned int nSig, void* pHandlerInfo);
    
protected:
    // Padding to reach correct size
    char _padding[24];
};

// IMPLEMENT_DYNAMIC(CCmdTarget, CObject) moved to appcore.cpp

// CWinThread - thread management class
class CWinThread : public CCmdTarget {
    DECLARE_DYNAMIC(CWinThread)
public:
    CWinThread();
    virtual ~CWinThread();
    
    // Thread operations
    virtual BOOL InitInstance();
    virtual int ExitInstance();
    virtual int Run();
    virtual BOOL PreTranslateMessage(MSG* pMsg);
    virtual BOOL OnIdle(LONG lCount);
    virtual BOOL IsIdleMessage(MSG* pMsg);
    
    // Thread control
    BOOL CreateThread(DWORD dwCreateFlags = 0, UINT nStackSize = 0,
                      LPSECURITY_ATTRIBUTES lpSecurityAttrs = nullptr);
    void SuspendThread();
    DWORD ResumeThread();
    BOOL SetThreadPriority(int nPriority);
    int GetThreadPriority() const;
    DWORD GetThreadId() const;
    
    // Thread state
    BOOL IsThreadRunning() const;
    BOOL IsThreadSuspended() const;
    
    // Message pump
    virtual BOOL PumpMessage();
    virtual BOOL PrePumpMessage();
    virtual BOOL PostPumpMessage();
    
    // Static thread management
    static CWinThread* GetCurrentThread();
    static CWinThread* FromHandle(HANDLE hThread);
    static void DeleteCurrentThread();
    
    // Auto-delete control
    void SetAutoDelete(BOOL bAutoDelete) { m_bAutoDelete = bAutoDelete; }
    BOOL GetAutoDelete() const { return m_bAutoDelete; }

    // Public members for compatibility
    CWnd* m_pMainWnd;
    DWORD m_nThreadID;
    HANDLE m_hThread;
    BOOL m_bAutoDelete;
    MSG m_msgCur;

protected:
    // Thread procedure (static wrapper)
    static UINT AFXAPI _ThreadEntry(LPVOID pParam);

    // Thread state
    int m_nThreadPriority;
    UINT m_nThreadStackSize;
    DWORD m_dwThreadCreateFlags;
    BOOL m_bRunning;
    BOOL m_bSuspended;
    
private:
    // Disable copy constructor and assignment
    CWinThread(const CWinThread&);
    CWinThread& operator=(const CWinThread&);
};

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
    HWND GetSafeHwnd() const { return m_hWnd; }
    operator HWND() const { return m_hWnd; }
    
    //-------------------------------------------------------------------------
    // Window creation/destruction
    //-------------------------------------------------------------------------
    virtual int Create(const wchar_t* lpszClassName, const wchar_t* lpszWindowName, DWORD dwStyle,
                       const struct tagRECT& rect, CWnd* pParentWnd, unsigned int nID, CCreateContext* pContext = nullptr);
    
    virtual int CreateEx(DWORD dwExStyle, const wchar_t* lpszClassName, const wchar_t* lpszWindowName,
                         DWORD dwStyle, int x, int y, int nWidth, int nHeight,
                         HWND hWndParent, HMENU nIDorHMenu, void* lpParam = nullptr);

    virtual int PreCreateWindow(CREATESTRUCTW& cs) { (void)cs; return TRUE; }

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
                          CWnd* pParentWnd = nullptr, CCreateContext* pContext = nullptr);
                          
    virtual int Create(const wchar_t* lpszClassName, const wchar_t* lpszWindowName,
                       DWORD dwStyle = 0, const struct tagRECT& rect = {},
                       CWnd* pParentWnd = nullptr, const wchar_t* lpszMenuName = nullptr,
                       DWORD dwExStyle = 0, CCreateContext* pContext = nullptr);

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
                       CMDIFrameWnd* pParentWnd = nullptr, CCreateContext* pContext = nullptr);
                       
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
// CPropertyPage - Property page for property sheets
//=============================================================================

class CPropertySheet; // Forward declaration

class CPropertyPage : public CDialog {
    DECLARE_DYNAMIC(CPropertyPage)
public:
    CPropertyPage();
    explicit CPropertyPage(unsigned int nIDTemplate, unsigned int nIDCaption = 0);
    explicit CPropertyPage(const wchar_t* lpszTemplateName, unsigned int nIDCaption = 0);
    virtual ~CPropertyPage() = default;

    // Overridables
    virtual int OnSetActive();
    virtual int OnKillActive();
    virtual void OnOK();
    virtual void OnCancel();
    virtual int OnApply();
    virtual void OnReset();
    virtual int OnQueryCancel();
    virtual int OnWizardBack();
    virtual int OnWizardNext();
    virtual int OnWizardFinish();

    // Operations
    void SetModified(int bChanged = 1);
    int QuerySiblings(uintptr_t wParam, intptr_t lParam);
    void CancelToClose();

    CPropertySheet* GetParentSheet();

public:
    unsigned int m_nIDCaption;
    int m_bModified;

protected:
    char _propertypage_padding[32];
};

//=============================================================================
// CPropertySheet - Property sheet (tabbed dialog)
//=============================================================================

class CPropertySheet : public CWnd {
    DECLARE_DYNAMIC(CPropertySheet)
public:
    CPropertySheet();
    explicit CPropertySheet(unsigned int nIDCaption, CWnd* pParentWnd = nullptr, unsigned int iSelectPage = 0);
    explicit CPropertySheet(const wchar_t* pszCaption, CWnd* pParentWnd = nullptr, unsigned int iSelectPage = 0);
    virtual ~CPropertySheet() = default;

    // Attributes
    int GetPageCount() const;
    CPropertyPage* GetActivePage() const;
    int GetActiveIndex() const;
    CPropertyPage* GetPage(int nPage) const;
    int GetPageIndex(CPropertyPage* pPage) const;

    // Operations
    int SetActivePage(int nPage);
    int SetActivePage(CPropertyPage* pPage);
    void SetTitle(const wchar_t* lpszText, unsigned int nStyle = 0);
    void SetWizardMode();
    void SetWizardButtons(unsigned long dwFlags);
    void SetFinishText(const wchar_t* lpszText);

    void AddPage(CPropertyPage* pPage);
    void RemovePage(CPropertyPage* pPage);
    void RemovePage(int nPage);

    void PressButton(int nButton);
    void EndDialog(int nEndID);

    // Modal/Modeless
    virtual intptr_t DoModal();
    virtual int Create(CWnd* pParentWnd = nullptr, unsigned long dwStyle = 0xFFFFFFFF, unsigned long dwExStyle = 0);

protected:
    // Overridables
    virtual int OnInitDialog();
    virtual void OnPageChanged();

public:
    const wchar_t* m_pszCaption;
    CWnd* m_pParentWnd;
    unsigned int m_nActivePage;
    int m_bWizardMode;

    // Simple page storage (max 16 pages for simplicity)
    CPropertyPage* m_pages[16];
    int m_nPageCount;

protected:
    char _propertysheet_padding[32];
};

//=============================================================================
// Common Dialogs
//=============================================================================

// Forward declarations
class CFileDialog;
class CColorDialog;
class CFontDialog;
class CPrintDialog;
class CPageSetupDialog;
class CFindReplaceDialog;

// CFileDialog - File Open/Save dialog
class CFileDialog : public CDialog {
    // DECLARE_DYNAMIC(CFileDialog)
public:
    // Constructor parameters based on MFC140u.dll symbols:
    // CFileDialog(int, wchar_t const *, wchar_t const *, unsigned long, wchar_t const *, class CWnd *, unsigned long, int)
    CFileDialog(int bOpenFileDialog, 
                const wchar_t* lpszDefExt = nullptr,
                const wchar_t* lpszFileName = nullptr,
                unsigned long dwFlags = 0,
                const wchar_t* lpszFilter = nullptr,
                CWnd* pParentWnd = nullptr,
                unsigned long dwSize = 0,
                int bVistaStyle = 1);
    
    virtual ~CFileDialog() = default;
    
    // Operations
    virtual intptr_t DoModal();
    
    // File information
    CString GetPathName() const;
    CString GetFileName() const;
    CString GetFileExt() const;
    CString GetFileTitle() const;
    CString GetFolderPath() const;
    
    // Multiple file selection
    CString GetNextPathName(void*& pos) const;
    
    // Customization
    void SetDefExt(const wchar_t* lpszDefExt);
    void SetControlText(int nID, const wchar_t* lpszText);
    
    // Vista-style customization
    long AddCheckButton(unsigned long dwIDCtl, const CString& strLabel, int bChecked);
    long AddControlItem(unsigned long dwIDCtl, unsigned long dwIDItem, const CString& strLabel);
    long AddEditBox(unsigned long dwIDCtl, const CString& strText);
    long AddMenu(unsigned long dwIDCtl, const CString& strLabel);
    long AddPushButton(unsigned long dwIDCtl, const CString& strLabel);
    long AddText(unsigned long dwIDCtl, const CString& strLabel);
    void AddPlace(const wchar_t* lpszFolder, int fdap);
    
    long GetEditBoxText(unsigned long dwIDCtl, CString& strText);
    long SetControlItemText(unsigned long dwIDCtl, unsigned long dwIDItem, const CString& strText);
    long SetControlLabel(unsigned long dwIDCtl, const CString& strLabel);
    long SetEditBoxText(unsigned long dwIDCtl, const CString& strText);
    
    int SetProperties(const wchar_t* lpszPropList);
    void SetTemplate(const wchar_t* lpWin3ID, const wchar_t* lpWin4ID);
    
    // Overridables
    virtual unsigned int OnShareViolation(const wchar_t* lpszPathName);
    
protected:
    int m_bOpenFileDialog;
    unsigned long m_dwFlags;
    CString m_strDefExt;
    CString m_strFileName;
    CString m_strFilter;
    CString m_strInitialDir;
    
    // File results
    CString m_strPathName;
    CString m_strFileNameOnly;
    CString m_strFileExt;
    CString m_strFileTitle;
    CString m_strFolderPath;
    
    // Multiple files
    void* m_pFileList;
    
    char _filedialog_padding[128];
};

// CColorDialog - Color selection dialog
class CColorDialog : public CDialog {
    DECLARE_DYNAMIC(CColorDialog)
public:
    CColorDialog(unsigned long clrInit = 0, unsigned long dwFlags = 0, CWnd* pParentWnd = nullptr);
    virtual ~CColorDialog() = default;
    
    virtual intptr_t DoModal();
    
    unsigned long GetColor() const;
    void SetCurrentColor(unsigned long clr);
    
    // Color customization
    void* GetSavedCustomColors();
    
protected:
    unsigned long m_clrInit;
    unsigned long m_clrResult;
    unsigned long m_dwFlags;
    
    char _colordialog_padding[64];
};

// CFontDialog - Font selection dialog  
class CFontDialog : public CDialog {
    DECLARE_DYNAMIC(CFontDialog)
public:
    CFontDialog(void* lpLogFont = nullptr, unsigned long dwFlags = 0, void* pdcPrinter = nullptr, CWnd* pParentWnd = nullptr);
    virtual ~CFontDialog() = default;
    
    virtual intptr_t DoModal();
    
    // Font information
    void* GetCurrentFont() const;
    CString GetFaceName() const;
    CString GetStyleName() const;
    int GetSize() const;
    unsigned long GetColor() const;
    int GetWeight() const;
    int IsStrikeOut() const;
    int IsUnderline() const;
    int IsBold() const;
    int IsItalic() const;
    
protected:
    void* m_lpLogFont;
    unsigned long m_dwFlags;
    
    char _fontdialog_padding[64];
};

// CPrintDialog - Print dialog
class CPrintDialog : public CDialog {
    DECLARE_DYNAMIC(CPrintDialog)
public:
    CPrintDialog(int bPrintSetupOnly = 0, unsigned long dwFlags = 0, CWnd* pParentWnd = nullptr);
    virtual ~CPrintDialog() = default;
    
    virtual intptr_t DoModal();
    
    // Printer information
    CString GetDeviceName() const;
    CString GetDriverName() const;
    CString GetPortName() const;
    
    // Print settings
    int GetCopies() const;
    int GetFromPage() const;
    int GetToPage() const;
    int GetPortrait() const;
    
protected:
    int m_bPrintSetupOnly;
    unsigned long m_dwFlags;
    
    char _printdialog_padding[64];
};

// CPageSetupDialog - Page setup dialog
class CPageSetupDialog : public CDialog {
    DECLARE_DYNAMIC(CPageSetupDialog)
public:
    CPageSetupDialog(unsigned long dwFlags = 0, CWnd* pParentWnd = nullptr);
    virtual ~CPageSetupDialog() = default;
    
    virtual intptr_t DoModal();
    
    // Printer information
    CString GetDeviceName() const;
    CString GetDriverName() const;
    CString GetPortName() const;
    
    // Page setup
    void* GetMargins() const;
    void* GetPaperSize() const;
    
protected:
    unsigned long m_dwFlags;
    
    char _pagesetupdialog_padding[64];
};

// CFindReplaceDialog - Find/Replace dialog (modeless)
class CFindReplaceDialog : public CDialog {
    DECLARE_DYNAMIC(CFindReplaceDialog)
public:
    CFindReplaceDialog();
    virtual ~CFindReplaceDialog() = default;
    
    // Modeless creation
    virtual int Create(int bFindDialogOnly, 
                      const wchar_t* lpszFindWhat = nullptr,
                      const wchar_t* lpszReplaceWith = nullptr,
                      unsigned long dwFlags = 0,
                      CWnd* pParentWnd = nullptr);
    
    // Find/Replace information
    CString GetFindString() const;
    CString GetReplaceString() const;
    int SearchDown() const;
    int FindNext() const;
    int MatchCase() const;
    int MatchWholeWord() const;
    int ReplaceCurrent() const;
    int ReplaceAll() const;
    
protected:
    int m_bFindDialogOnly;
    CString m_strFindWhat;
    CString m_strReplaceWith;
    unsigned long m_dwFlags;
    
    char _findreplacedialog_padding[64];
};

//=============================================================================
// CWinThread and CWinApp
//=============================================================================

// CWinThread is defined in afx.h
// This is just a forward declaration to avoid circular dependencies

class CWinApp : public CWinThread {
    DECLARE_DYNAMIC(CWinApp)
public:
    CWinApp(const wchar_t* lpszAppName = nullptr);
    virtual ~CWinApp() = default;
    
    virtual int InitApplication() { return 1; }
    virtual BOOL InitInstance() override { return TRUE; }
    virtual int ExitInstance() override { return 0; }
    virtual int Run() override;
    
    // Application info
    const wchar_t* m_pszAppName;
    const wchar_t* m_pszExeName;
    const wchar_t* m_pszHelpFilePath;
    const wchar_t* m_pszProfileName;
    const wchar_t* m_pszRegistryKey;
    HINSTANCE m_hInstance;
    wchar_t* m_lpCmdLine;
    int m_nCmdShow;

protected:
    char _winapp_padding[232];
};

// IMPLEMENT_DYNAMIC(CWinApp, CWinThread) moved to appcore.cpp

// CWinApp constructor is in appcore.cpp

inline int CWinApp::Run() {
    return CWinThread::Run();
}

// Helper functions - these are DLL exports in real MFC
// Symbol: ?AfxGetThread@@YAPEAVCWinThread@@XZ
extern CWinThread* AFXAPI AfxGetThread();

// AfxGetApp is typically inline in real MFC, casting AfxGetThread result
inline CWinApp* AFXAPI AfxGetApp() { return static_cast<CWinApp*>(AfxGetThread()); }

// These functions are declared extern and defined in appcore.cpp when linking with implementation
// For header-only use, they fall back to inline stubs
#ifndef OPENMFC_APPCORE_IMPL
inline HINSTANCE AFXAPI AfxGetInstanceHandle() {
    CWinApp* pApp = AfxGetApp();
    return pApp ? pApp->m_hInstance : nullptr;
}
inline HINSTANCE AFXAPI AfxGetResourceHandle() {
    CWinApp* pApp = AfxGetApp();
    return pApp ? pApp->m_hInstance : nullptr;
}
inline void AFXAPI AfxSetResourceHandle(HINSTANCE) { /* TODO */ }
inline CWnd* AFXAPI AfxGetMainWnd() {
    CWinApp* pApp = AfxGetApp();
    return pApp ? pApp->m_pMainWnd : nullptr;
}
inline int AFXAPI AfxWinInit(HINSTANCE, HINSTANCE, LPWSTR, int) { return TRUE; }
#else
extern HINSTANCE AFXAPI AfxGetInstanceHandle();
extern HINSTANCE AFXAPI AfxGetResourceHandle();
extern void AFXAPI AfxSetResourceHandle(HINSTANCE);
extern CWnd* AFXAPI AfxGetMainWnd();
extern BOOL AFXAPI AfxWinInit(HINSTANCE, HINSTANCE, LPWSTR, int);
#endif

// Exception helpers
extern void AFXAPI AfxThrowMemoryException();
extern void AFXAPI AfxThrowFileException(int cause, long lOsError = -1, const wchar_t* lpszFileName = nullptr);
extern void AFXAPI AfxThrowInvalidArgException();
extern void AFXAPI AfxThrowNotSupportedException();
extern void AFXAPI AfxThrowResourceException();
extern void AFXAPI AfxThrowUserException();
extern void AFXAPI AfxAbort();

//=============================================================================
// GDI & Graphics Classes
//=============================================================================

// Simple geometry classes (no inheritance needed)
class CPoint {
public:
    int x, y;
    
    CPoint() : x(0), y(0) {}
    CPoint(int initX, int initY) : x(initX), y(initY) {}
    CPoint(const POINT& initPt) : x(initPt.x), y(initPt.y) {}
    
    void SetPoint(int X, int Y) { x = X; y = Y; }
    
    operator POINT() const {
        POINT pt;
        pt.x = x;
        pt.y = y;
        return pt;
    }
    
    CPoint& operator+=(const CPoint& point) {
        x += point.x;
        y += point.y;
        return *this;
    }
    
    CPoint& operator-=(const CPoint& point) {
        x -= point.x;
        y -= point.y;
        return *this;
    }
};

class CSize {
public:
    int cx, cy;
    
    CSize() : cx(0), cy(0) {}
    CSize(int initCX, int initCY) : cx(initCX), cy(initCY) {}
    CSize(const SIZE& initSz) : cx(initSz.cx), cy(initSz.cy) {}
    
    void SetSize(int CX, int CY) { cx = CX; cy = CY; }
    
    operator SIZE() const {
        SIZE sz;
        sz.cx = cx;
        sz.cy = cy;
        return sz;
    }
    
    CSize& operator+=(const CSize& size) {
        cx += size.cx;
        cy += size.cy;
        return *this;
    }
    
    CSize& operator-=(const CSize& size) {
        cx -= size.cx;
        cy -= size.cy;
        return *this;
    }
};

class CRect {
public:
    int left, top, right, bottom;
    
    CRect() : left(0), top(0), right(0), bottom(0) {}
    CRect(int l, int t, int r, int b) : left(l), top(t), right(r), bottom(b) {}
    CRect(const RECT& srcRect) : left(srcRect.left), top(srcRect.top), right(srcRect.right), bottom(srcRect.bottom) {}
    CRect(const CPoint& topLeft, const CPoint& bottomRight) : left(topLeft.x), top(topLeft.y), right(bottomRight.x), bottom(bottomRight.y) {}
    CRect(const CPoint& point, const CSize& size) : left(point.x), top(point.y), right(point.x + size.cx), bottom(point.y + size.cy) {}
    
    int Width() const { return right - left; }
    int Height() const { return bottom - top; }
    CSize Size() const { return CSize(Width(), Height()); }
    
    CPoint TopLeft() const { return CPoint(left, top); }
    CPoint BottomRight() const { return CPoint(right, bottom); }
    CPoint CenterPoint() const { return CPoint((left + right) / 2, (top + bottom) / 2); }
    
    void SetRect(int l, int t, int r, int b) { left = l; top = t; right = r; bottom = b; }
    void SetRectEmpty() { left = top = right = bottom = 0; }
    int IsRectEmpty() const { return (left >= right) || (top >= bottom); }
    int IsRectNull() const { return (left == 0) && (top == 0) && (right == 0) && (bottom == 0); }
    
    void InflateRect(int x, int y) { left -= x; top -= y; right += x; bottom += y; }
    void InflateRect(const CSize& size) { InflateRect(size.cx, size.cy); }
    void InflateRect(const CRect& rect) { left -= rect.left; top -= rect.top; right += rect.right; bottom += rect.bottom; }
    
    void DeflateRect(int x, int y) { left += x; top += y; right -= x; bottom -= y; }
    void DeflateRect(const CSize& size) { DeflateRect(size.cx, size.cy); }
    void DeflateRect(const CRect& rect) { left += rect.left; top += rect.top; right -= rect.right; bottom -= rect.bottom; }
    
    void NormalizeRect() {
        if (left > right) { int temp = left; left = right; right = temp; }
        if (top > bottom) { int temp = top; top = bottom; bottom = temp; }
    }
    
    void OffsetRect(int x, int y) { left += x; top += y; right += x; bottom += y; }
    void OffsetRect(const CPoint& point) { OffsetRect(point.x, point.y); }
    void OffsetRect(const CSize& size) { OffsetRect(size.cx, size.cy); }
    
    operator RECT*() { return (RECT*)this; }
    operator const RECT*() const { return (const RECT*)this; }
    
    int PtInRect(const CPoint& point) const {
        return (point.x >= left && point.x < right && point.y >= top && point.y < bottom);
    }
    
    int IntersectRect(const CRect& rect1, const CRect& rect2) {
        left = (rect1.left > rect2.left) ? rect1.left : rect2.left;
        top = (rect1.top > rect2.top) ? rect1.top : rect2.top;
        right = (rect1.right < rect2.right) ? rect1.right : rect2.right;
        bottom = (rect1.bottom < rect2.bottom) ? rect1.bottom : rect2.bottom;
        return !IsRectEmpty();
    }
    
    int UnionRect(const CRect& rect1, const CRect& rect2) {
        left = (rect1.left < rect2.left) ? rect1.left : rect2.left;
        top = (rect1.top < rect2.top) ? rect1.top : rect2.top;
        right = (rect1.right > rect2.right) ? rect1.right : rect2.right;
        bottom = (rect1.bottom > rect2.bottom) ? rect1.bottom : rect2.bottom;
        return !IsRectEmpty();
    }
};

// CGdiObject - base class for GDI objects
class CGdiObject : public CObject {
    DECLARE_DYNAMIC(CGdiObject)
public:
    CGdiObject() : m_hObject(nullptr) {}
    virtual ~CGdiObject() { DeleteObject(); }
    
    // Object operations
    int DeleteObject();
    int GetObject(int nCount, void* lpObject) const;
    int CreateStockObject(int nIndex);
    int UnrealizeObject();
    
    // Handle operations
    HGDIOBJ GetSafeHandle() const { return m_hObject; }
    operator HGDIOBJ() const { return m_hObject; }
    static CGdiObject* FromHandle(HGDIOBJ hObject);
    static void DeleteTempMap();
    static int DeleteTempMap(int bAutoDeleteOnly);
    
    // Attributes
    int Attach(HGDIOBJ hObject);
    HGDIOBJ Detach();

public:
    HGDIOBJ m_hObject;

    // Padding for ABI compatibility
    char _gdiobject_padding[24];
};

// CPen - Pen GDI object
class CPen : public CGdiObject {
    DECLARE_DYNAMIC(CPen)
public:
    CPen();
    CPen(int nPenStyle, int nWidth, unsigned long crColor);
    CPen(int nPenStyle, int nWidth, const void* pLogBrush, int nStyleCount = 0, const unsigned long* lpStyle = nullptr);
    
    int CreatePen(int nPenStyle, int nWidth, unsigned long crColor);
    int CreatePenIndirect(const void* lpLogPen);
    int CreatePen(int nPenStyle, int nWidth, const void* pLogBrush, int nStyleCount = 0, const unsigned long* lpStyle = nullptr);
    
    int GetLogPen(void* pLogPen) const;
    int GetExtLogPen(void* pLogPen) const;
    
protected:
    char _pen_padding[24];
};

// CBrush - Brush GDI object
class CBrush : public CGdiObject {
    DECLARE_DYNAMIC(CBrush)
public:
    CBrush();
    CBrush(unsigned long crColor);
    CBrush(int nIndex, unsigned long crColor);
    CBrush(const void* pBitmap);
    
    int CreateSolidBrush(unsigned long crColor);
    int CreateHatchBrush(int nIndex, unsigned long crColor);
    int CreateBrushIndirect(const void* lpLogBrush);
    int CreatePatternBrush(const void* pBitmap);
    int CreateDIBPatternBrush(const void* lpPackedDIB, unsigned int nUsage);
    int CreateSysColorBrush(int nIndex);
    
    int GetLogBrush(void* pLogBrush) const;
    
protected:
    char _brush_padding[24];
};

// CFont - Font GDI object
class CFont : public CGdiObject {
    DECLARE_DYNAMIC(CFont)
public:
    CFont();
    
    int CreateFontIndirect(const void* lpLogFont);
    int CreateFont(int nHeight, int nWidth, int nEscapement, int nOrientation, int nWeight,
                   unsigned char bItalic, unsigned char bUnderline, unsigned char cStrikeOut,
                   unsigned char nCharSet, unsigned char nOutPrecision, unsigned char nClipPrecision,
                   unsigned char nQuality, unsigned char nPitchAndFamily, const wchar_t* lpszFacename);
    int CreatePointFont(int nPointSize, const wchar_t* lpszFaceName, void* pDC = nullptr);
    int CreatePointFontIndirect(const void* lpLogFont, void* pDC = nullptr);
    
    int GetLogFont(void* pLogFont) const;
    
protected:
    char _font_padding[24];
};

// CBitmap - Bitmap GDI object
class CBitmap : public CGdiObject {
    DECLARE_DYNAMIC(CBitmap)
public:
    CBitmap();
    
    int LoadBitmap(const wchar_t* lpszResourceName);
    int LoadBitmap(unsigned int nIDResource);
    int LoadOEMBitmap(unsigned int nIDBitmap);
    int LoadMappedBitmap(unsigned int nIDBitmap, unsigned int nFlags = 0, void* lpColorMap = nullptr, int nMapSize = 0);
    
    int CreateBitmap(int nWidth, int nHeight, unsigned int nPlanes, unsigned int nBitcount, const void* lpBits);
    int CreateBitmapIndirect(const void* lpBitmap);
    int CreateCompatibleBitmap(void* pDC, int nWidth, int nHeight);
    int CreateDiscardableBitmap(void* pDC, int nWidth, int nHeight);
    
    int GetBitmap(void* pBitMap) const;
    int GetBitmapBits(unsigned long dwCount, void* lpBits) const;
    int SetBitmapBits(unsigned long dwCount, const void* lpBits);
    int GetBitmapDimension(CSize* pSize) const;
    int SetBitmapDimension(int nWidth, int nHeight, CSize* pSize = nullptr);
    
protected:
    char _bitmap_padding[24];
};

// CPalette - Palette GDI object
class CPalette : public CGdiObject {
    DECLARE_DYNAMIC(CPalette)
public:
    CPalette();
    
    int CreatePalette(const void* lpLogPalette);
    int CreateHalftonePalette(void* pDC);
    
    unsigned int GetPaletteEntries(unsigned int nStartIndex, unsigned int nNumEntries, void* lpPaletteColors) const;
    unsigned int SetPaletteEntries(unsigned int nStartIndex, unsigned int nNumEntries, const void* lpPaletteColors);
    
    void AnimatePalette(unsigned int nStartIndex, unsigned int nNumEntries, const void* lpPaletteColors);
    unsigned int GetNearestPaletteIndex(unsigned long crColor) const;
    int ResizePalette(unsigned int nNumEntries);
    
protected:
    char _palette_padding[24];
};

// CRgn - Region GDI object
class CRgn : public CGdiObject {
    DECLARE_DYNAMIC(CRgn)
public:
    CRgn();
    
    // Creation methods
    int CreateRectRgn(int x1, int y1, int x2, int y2);
    int CreateRectRgnIndirect(const RECT* lpRect);
    int CreateEllipticRgn(int x1, int y1, int x2, int y2);
    int CreateEllipticRgnIndirect(const RECT* lpRect);
    int CreatePolygonRgn(const POINT* lpPoints, int nCount, int nMode);
    int CreatePolyPolygonRgn(const POINT* lpPoints, const int* lpPolyCounts, int nCount, int nMode);
    int CreateRoundRectRgn(int x1, int y1, int x2, int y2, int x3, int y3);
    int CreateFromPath(void* pDC);
    int CreateFromData(const void* lpXForm, const void* lpRgnData, int nCount);
    
    // Operations
    int CombineRgn(const CRgn* pRgn1, const CRgn* pRgn2, int nCombineMode);
    int CopyRgn(const CRgn* pRgnSrc);
    int EqualRgn(const CRgn* pRgn) const;
    
    // Region info
    int GetRgnBox(RECT* pRect) const;
    int OffsetRgn(int x, int y);
    int OffsetRgn(const POINT& point);
    int PtInRegion(int x, int y) const;
    int PtInRegion(const POINT& point) const;
    int RectInRegion(const RECT* pRect) const;
    
    int SetRectRgn(int x1, int y1, int x2, int y2);
    int SetRectRgn(const RECT* lpRect);
    
    int GetRegionData(void* lpRgnData, unsigned long dwCount) const;
    
protected:
    char _rgn_padding[24];
};

// CDC - Device Context base class
class CDC : public CObject {
    DECLARE_DYNAMIC(CDC)
public:
    CDC();
    virtual ~CDC();
    
    // Creation/destruction
    int CreateDC(const wchar_t* lpszDriverName, const wchar_t* lpszDeviceName,
                 const wchar_t* lpszOutput, const void* lpInitData);
    int CreateIC(const wchar_t* lpszDriverName, const wchar_t* lpszDeviceName,
                 const wchar_t* lpszOutput, const void* lpInitData);
    int CreateCompatibleDC(CDC* pDC);
    int DeleteDC();
    
    // Handle operations
    HDC GetSafeHdc() const { return m_hDC; }
    operator HDC() const { return m_hDC; }
    static CDC* FromHandle(HDC hDC);
    static void DeleteTempMap();
    static int DeleteTempMap(int bAutoDeleteOnly);
    
    // Device context state
    int SaveDC();
    int RestoreDC(int nSavedDC);
    int ResetDC(const void* lpDevMode);
    
    // Drawing tool selection
    CGdiObject* SelectObject(CGdiObject* pObject);
    CPen* SelectObject(CPen* pPen);
    CBrush* SelectObject(CBrush* pBrush);
    CFont* SelectObject(CFont* pFont);
    CBitmap* SelectObject(CBitmap* pBitmap);
    int SelectObject(CRgn* pRgn);
    int SelectStockObject(int nIndex);
    
    // Drawing attributes
    unsigned long GetBkColor() const;
    unsigned long SetBkColor(unsigned long crColor);
    int GetBkMode() const;
    int SetBkMode(int nBkMode);
    int GetPolyFillMode() const;
    int SetPolyFillMode(int nPolyFillMode);
    int GetROP2() const;
    int SetROP2(int nDrawMode);
    int GetStretchBltMode() const;
    int SetStretchBltMode(int nStretchMode);
    int GetTextColor() const;
    unsigned long SetTextColor(unsigned long crColor);
    int GetMapMode() const;
    int SetMapMode(int nMapMode);
    
    // Coordinate transformation
    int GetViewportOrg(CPoint* pPoint) const;
    CPoint GetViewportOrg() const;
    CPoint SetViewportOrg(int x, int y);
    CPoint SetViewportOrg(const CPoint& point);
    CPoint OffsetViewportOrg(int nWidth, int nHeight);
    
    int GetWindowOrg(CPoint* pPoint) const;
    CPoint GetWindowOrg() const;
    CPoint SetWindowOrg(int x, int y);
    CPoint SetWindowOrg(const CPoint& point);
    CPoint OffsetWindowOrg(int nWidth, int nHeight);
    
    int GetViewportExt(CSize* pSize) const;
    CSize GetViewportExt() const;
    CSize SetViewportExt(int x, int y);
    CSize SetViewportExt(const CSize& size);
    CSize ScaleViewportExt(int xNum, int xDenom, int yNum, int yDenom);
    
    int GetWindowExt(CSize* pSize) const;
    CSize GetWindowExt() const;
    CSize SetWindowExt(int x, int y);
    CSize SetWindowExt(const CSize& size);
    CSize ScaleWindowExt(int xNum, int xDenom, int yNum, int yDenom);
    
    // Drawing operations
    int Rectangle(int x1, int y1, int x2, int y2);
    int Rectangle(const RECT* lpRect);
    int FillRect(const RECT* lpRect, CBrush* pBrush);
    int FrameRect(const RECT* lpRect, CBrush* pBrush);
    int InvertRect(const RECT* lpRect);
    int DrawIcon(int x, int y, void* hIcon);
    int DrawIcon(const POINT& point, void* hIcon);
    
    // Text operations
    int TextOut(int x, int y, const wchar_t* lpszString, int nCount = -1);
    int TextOut(const CPoint& point, const wchar_t* lpszString, int nCount = -1);
    int ExtTextOut(int x, int y, unsigned int nOptions, const RECT* lpRect,
                   const wchar_t* lpszString, unsigned int nCount, const int* lpDxWidths);
    int TabbedTextOut(int x, int y, const wchar_t* lpszString, int nCount,
                      int nTabPositions, const int* lpnTabStopPositions, int nTabOrigin);
    int DrawText(const wchar_t* lpszString, int nCount, RECT* lpRect, unsigned int nFormat);
    int DrawTextEx(wchar_t* lpszString, int nCount, RECT* lpRect, unsigned int nFormat, void* lpDTParams);
    
    // Bitmap operations
    int BitBlt(int x, int y, int nWidth, int nHeight, CDC* pSrcDC,
               int xSrc, int ySrc, unsigned long dwRop);
    int StretchBlt(int x, int y, int nWidth, int nHeight, CDC* pSrcDC,
                   int xSrc, int ySrc, int nSrcWidth, int nSrcHeight, unsigned long dwRop);
    int PatBlt(int x, int y, int nWidth, int nHeight, unsigned long dwRop);
    
    // Clipping operations
    int SelectClipRgn(CRgn* pRgn);
    int SelectClipRgn(CRgn* pRgn, int nMode);
    int ExcludeClipRect(int x1, int y1, int x2, int y2);
    int ExcludeClipRect(const RECT* lpRect);
    int ExcludeUpdateRgn(CWnd* pWnd);
    int IntersectClipRect(int x1, int y1, int x2, int y2);
    int IntersectClipRect(const RECT* lpRect);
    int OffsetClipRgn(int x, int y);
    int OffsetClipRgn(const CSize& size);
    
    // Line and curve drawing
    int MoveTo(int x, int y);
    int MoveTo(const CPoint& point);
    int LineTo(int x, int y);
    int LineTo(const CPoint& point);
    int Arc(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4);
    int Arc(const RECT* lpRect, const CPoint& ptStart, const CPoint& ptEnd);
    int Polyline(const POINT* lpPoints, int nCount);
    int Polygon(const POINT* lpPoints, int nCount);
    int PolyPolygon(const POINT* lpPoints, const int* lpPolyCounts, int nCount);
    
    // Ellipse and chord
    int Ellipse(int x1, int y1, int x2, int y2);
    int Ellipse(const RECT* lpRect);
    int Chord(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4);
    int Chord(const RECT* lpRect, const CPoint& ptStart, const CPoint& ptEnd);
    int Pie(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4);
    int Pie(const RECT* lpRect, const CPoint& ptStart, const CPoint& ptEnd);
    
    // Round rectangle
    int RoundRect(int x1, int y1, int x2, int y2, int x3, int y3);
    int RoundRect(const RECT* lpRect, const CPoint& point);
    
    // Path operations
    int BeginPath();
    int EndPath();
    int CloseFigure();
    int StrokePath();
    int FillPath();
    int StrokeAndFillPath();
    
    // Miscellaneous
    int GetCurrentPosition(CPoint* pPoint) const;
    CPoint GetCurrentPosition() const;
    int DPtoLP(LPPOINT lpPoints, int nCount = 1) const;
    int DPtoLP(LPRECT lpRect) const;
    int DPtoLP(LPSIZE lpSize) const;
    int LPtoDP(LPPOINT lpPoints, int nCount = 1) const;
    int LPtoDP(LPRECT lpRect) const;
    int LPtoDP(LPSIZE lpSize) const;
    
    // Printer escape
    int Escape(int nEscape, int nCount, const char* lpszInData, void* lpszOutData);
    
public:
    HDC m_hDC;
    HDC m_hAttribDC;
    
protected:
    // Padding for ABI compatibility
    char _cdc_padding[64];
};

// CClientDC - Client area device context
class CClientDC : public CDC {
    DECLARE_DYNAMIC(CClientDC)
public:
    CClientDC(CWnd* pWnd);
    virtual ~CClientDC();

public:
    CWnd* m_pWnd;

protected:
    char _clientdc_padding[24];
};

// CPaintDC - Painting device context
class CPaintDC : public CDC {
    DECLARE_DYNAMIC(CPaintDC)
public:
    CPaintDC(CWnd* pWnd);
    virtual ~CPaintDC();

    PAINTSTRUCT m_ps;

public:
    CWnd* m_pWnd;

protected:
    char _paintdc_padding[24];
};

// CWindowDC - Whole window device context
class CWindowDC : public CDC {
    DECLARE_DYNAMIC(CWindowDC)
public:
    CWindowDC(CWnd* pWnd);
    virtual ~CWindowDC();

public:
    CWnd* m_pWnd;

protected:
    char _windowdc_padding[24];
};

// CMetaFileDC - Metafile device context
class CMetaFileDC : public CDC {
    DECLARE_DYNAMIC(CMetaFileDC)
public:
    CMetaFileDC();
    virtual ~CMetaFileDC();
    
    int Create(const wchar_t* lpszFilename = nullptr);
    int CreateEnhanced(void* pDC, const wchar_t* lpszFilename, const RECT* lpBounds, const wchar_t* lpszDescription);
    
    void* Close();
    void* CloseEnhanced();
    
protected:
    char _metafiledc_padding[24];
};

//=============================================================================
// CMenu - Menu wrapper class
//=============================================================================

class CMenu : public CObject {
    DECLARE_DYNAMIC(CMenu)
public:
    CMenu() : m_hMenu(nullptr) {}
    virtual ~CMenu();

    // Initialization
    int CreateMenu();
    int CreatePopupMenu();
    int LoadMenu(UINT nIDResource);
    int LoadMenu(const wchar_t* lpszResourceName);
    int DestroyMenu();

    // Handle operations
    HMENU GetSafeHmenu() const { return m_hMenu; }
    operator HMENU() const { return m_hMenu; }
    static CMenu* FromHandle(HMENU hMenu);
    static void DeleteTempMap();
    int Attach(HMENU hMenu);
    HMENU Detach();

    // Menu item operations
    int AppendMenu(UINT nFlags, UINT_PTR nIDNewItem = 0, const wchar_t* lpszNewItem = nullptr);
    int AppendMenu(UINT nFlags, UINT_PTR nIDNewItem, CBitmap* pBmp);
    int InsertMenu(UINT nPosition, UINT nFlags, UINT_PTR nIDNewItem = 0, const wchar_t* lpszNewItem = nullptr);
    int ModifyMenu(UINT nPosition, UINT nFlags, UINT_PTR nIDNewItem = 0, const wchar_t* lpszNewItem = nullptr);
    int DeleteMenu(UINT nPosition, UINT nFlags);
    int RemoveMenu(UINT nPosition, UINT nFlags);

    // Menu item state
    UINT EnableMenuItem(UINT nIDEnableItem, UINT nEnable);
    UINT CheckMenuItem(UINT nIDCheckItem, UINT nCheck);
    int CheckMenuRadioItem(UINT nIDFirst, UINT nIDLast, UINT nIDItem, UINT nFlags);
    int SetMenuItemBitmaps(UINT nPosition, UINT nFlags, HBITMAP hBmpUnchecked, HBITMAP hBmpChecked);

    // Menu info
    int GetMenuItemCount() const;
    UINT GetMenuItemID(int nPos) const;
    CMenu* GetSubMenu(int nPos) const;
    int GetMenuString(UINT nIDItem, wchar_t* lpString, int nMaxCount, UINT nFlags) const;
    UINT GetMenuState(UINT nID, UINT nFlags) const;

    // Default item
    int SetDefaultItem(UINT uItem, UINT fByPos = FALSE);
    UINT GetDefaultItem(UINT gmdiFlags, UINT fByPos = TRUE) const;

    // Popup menu
    int TrackPopupMenu(UINT nFlags, int x, int y, CWnd* pWnd, const RECT* lpRect = nullptr);
    int TrackPopupMenuEx(UINT nFlags, int x, int y, CWnd* pWnd, LPTPMPARAMS lpParams);

    // Drawing (for owner-draw)
    virtual void DrawItem(void* lpDrawItemStruct);
    virtual void MeasureItem(void* lpMeasureItemStruct);

public:
    HMENU m_hMenu;

protected:
    char _menu_padding[24];
};

// IMPLEMENT_DYNAMIC(CMenu, CObject) in menucore.cpp

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
                         const struct tagRECT& rect, CWnd* pParentWnd, unsigned int nID, CCreateContext* pContext) {
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

//=============================================================================
// Common Controls
//=============================================================================

// CButton - Button control wrapper
class CButton : public CWnd {
    DECLARE_DYNAMIC(CButton)
public:
    CButton() = default;
    virtual ~CButton() = default;

    int Create(const wchar_t* lpszCaption, DWORD dwStyle, const struct tagRECT& rect, CWnd* pParentWnd, unsigned int nID) {
        return CWnd::Create(L"BUTTON", lpszCaption, dwStyle, rect, pParentWnd, nID);
    }

    unsigned int GetState() const {
        return m_hWnd ? (unsigned int)::SendMessageW(m_hWnd, BM_GETSTATE, 0, 0) : 0;
    }
    void SetState(int bHighlight) {
        if (m_hWnd) ::SendMessageW(m_hWnd, BM_SETSTATE, bHighlight ? TRUE : FALSE, 0);
    }
    int GetCheck() const {
        return m_hWnd ? (int)::SendMessageW(m_hWnd, BM_GETCHECK, 0, 0) : 0;
    }
    void SetCheck(int nCheck) {
        if (m_hWnd) ::SendMessageW(m_hWnd, BM_SETCHECK, nCheck, 0);
    }
    unsigned int GetButtonStyle() const {
        return m_hWnd ? (unsigned int)(::GetWindowLongW(m_hWnd, GWL_STYLE) & 0xFFFF) : 0;
    }
    void SetButtonStyle(unsigned int nStyle, int bRedraw = 1) {
        if (m_hWnd) ::SendMessageW(m_hWnd, BM_SETSTYLE, nStyle, MAKELPARAM(bRedraw, 0));
    }
    void* GetBitmap() const {
        return m_hWnd ? (void*)::SendMessageW(m_hWnd, BM_GETIMAGE, IMAGE_BITMAP, 0) : nullptr;
    }
    void* SetBitmap(void* hBitmap) {
        return m_hWnd ? (void*)::SendMessageW(m_hWnd, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hBitmap) : nullptr;
    }
    void* GetIcon() const {
        return m_hWnd ? (void*)::SendMessageW(m_hWnd, BM_GETIMAGE, IMAGE_ICON, 0) : nullptr;
    }
    void* SetIcon(void* hIcon) {
        return m_hWnd ? (void*)::SendMessageW(m_hWnd, BM_SETIMAGE, IMAGE_ICON, (LPARAM)hIcon) : nullptr;
    }
};

// CEdit - Edit control wrapper
class CEdit : public CWnd {
    DECLARE_DYNAMIC(CEdit)
public:
    CEdit() = default;
    virtual ~CEdit() = default;

    int Create(DWORD dwStyle, const struct tagRECT& rect, CWnd* pParentWnd, unsigned int nID) {
        return CWnd::Create(L"EDIT", L"", dwStyle, rect, pParentWnd, nID);
    }

    int GetLineCount() const {
        return m_hWnd ? (int)::SendMessageW(m_hWnd, EM_GETLINECOUNT, 0, 0) : 1;
    }
    int GetLine(int nIndex, wchar_t* lpszBuffer, int nMaxLength) const {
        if (!m_hWnd || !lpszBuffer || nMaxLength <= 0) return 0;
        *(WORD*)lpszBuffer = (WORD)nMaxLength;
        return (int)::SendMessageW(m_hWnd, EM_GETLINE, nIndex, (LPARAM)lpszBuffer);
    }
    void GetSel(int& nStartChar, int& nEndChar) const {
        DWORD dwStart = 0, dwEnd = 0;
        if (m_hWnd) ::SendMessageW(m_hWnd, EM_GETSEL, (WPARAM)&dwStart, (LPARAM)&dwEnd);
        nStartChar = (int)dwStart; nEndChar = (int)dwEnd;
    }
    void SetSel(int nStartChar, int nEndChar, int bNoScroll = 0) {
        if (m_hWnd) {
            ::SendMessageW(m_hWnd, EM_SETSEL, nStartChar, nEndChar);
            if (!bNoScroll) ::SendMessageW(m_hWnd, EM_SCROLLCARET, 0, 0);
        }
    }
    void ReplaceSel(const wchar_t* lpszNewText, int bCanUndo = 0) {
        if (m_hWnd) ::SendMessageW(m_hWnd, EM_REPLACESEL, bCanUndo ? TRUE : FALSE, (LPARAM)lpszNewText);
    }
    void Clear() { if (m_hWnd) ::SendMessageW(m_hWnd, WM_CLEAR, 0, 0); }
    void Copy() { if (m_hWnd) ::SendMessageW(m_hWnd, WM_COPY, 0, 0); }
    void Cut() { if (m_hWnd) ::SendMessageW(m_hWnd, WM_CUT, 0, 0); }
    void Paste() { if (m_hWnd) ::SendMessageW(m_hWnd, WM_PASTE, 0, 0); }
    int Undo() { return m_hWnd ? (int)::SendMessageW(m_hWnd, EM_UNDO, 0, 0) : 0; }
    void SetReadOnly(int bReadOnly = 1) {
        if (m_hWnd) ::SendMessageW(m_hWnd, EM_SETREADONLY, bReadOnly ? TRUE : FALSE, 0);
    }
    int GetModify() const {
        return m_hWnd ? (int)::SendMessageW(m_hWnd, EM_GETMODIFY, 0, 0) : 0;
    }
    void SetModify(int bModified = 1) {
        if (m_hWnd) ::SendMessageW(m_hWnd, EM_SETMODIFY, bModified ? TRUE : FALSE, 0);
    }
    void LimitText(int nChars = 0) {
        if (m_hWnd) ::SendMessageW(m_hWnd, EM_LIMITTEXT, nChars, 0);
    }
    int GetLimitText() const {
        return m_hWnd ? (int)::SendMessageW(m_hWnd, EM_GETLIMITTEXT, 0, 0) : 0;
    }
    void SetPasswordChar(wchar_t ch) {
        if (m_hWnd) ::SendMessageW(m_hWnd, EM_SETPASSWORDCHAR, ch, 0);
    }
};

// CStatic - Static control wrapper
class CStatic : public CWnd {
    DECLARE_DYNAMIC(CStatic)
public:
    CStatic() = default;
    virtual ~CStatic() = default;

    int Create(const wchar_t* lpszText, DWORD dwStyle, const struct tagRECT& rect, CWnd* pParentWnd, unsigned int nID = 0xFFFF) {
        return CWnd::Create(L"STATIC", lpszText, dwStyle, rect, pParentWnd, nID);
    }

    void* GetBitmap() const {
        return m_hWnd ? (void*)::SendMessageW(m_hWnd, STM_GETIMAGE, IMAGE_BITMAP, 0) : nullptr;
    }
    void* SetBitmap(void* hBitmap) {
        return m_hWnd ? (void*)::SendMessageW(m_hWnd, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hBitmap) : nullptr;
    }
    void* GetIcon() const {
        return m_hWnd ? (void*)::SendMessageW(m_hWnd, STM_GETICON, 0, 0) : nullptr;
    }
    void* SetIcon(void* hIcon) {
        return m_hWnd ? (void*)::SendMessageW(m_hWnd, STM_SETICON, (WPARAM)hIcon, 0) : nullptr;
    }
    void* GetEnhMetaFile() const {
        return m_hWnd ? (void*)::SendMessageW(m_hWnd, STM_GETIMAGE, IMAGE_ENHMETAFILE, 0) : nullptr;
    }
    void* SetEnhMetaFile(void* hMetaFile) {
        return m_hWnd ? (void*)::SendMessageW(m_hWnd, STM_SETIMAGE, IMAGE_ENHMETAFILE, (LPARAM)hMetaFile) : nullptr;
    }
};

// CListBox - List box control wrapper
class CListBox : public CWnd {
    DECLARE_DYNAMIC(CListBox)
public:
    CListBox() = default;
    virtual ~CListBox() = default;

    int Create(DWORD dwStyle, const struct tagRECT& rect, CWnd* pParentWnd, unsigned int nID) {
        return CWnd::Create(L"LISTBOX", L"", dwStyle, rect, pParentWnd, nID);
    }

    int GetCount() const {
        return m_hWnd ? (int)::SendMessageW(m_hWnd, LB_GETCOUNT, 0, 0) : 0;
    }
    int GetCurSel() const {
        return m_hWnd ? (int)::SendMessageW(m_hWnd, LB_GETCURSEL, 0, 0) : LB_ERR;
    }
    int SetCurSel(int nSelect) {
        return m_hWnd ? (int)::SendMessageW(m_hWnd, LB_SETCURSEL, nSelect, 0) : LB_ERR;
    }
    int GetText(int nIndex, wchar_t* lpszBuffer) const {
        return (m_hWnd && lpszBuffer) ? (int)::SendMessageW(m_hWnd, LB_GETTEXT, nIndex, (LPARAM)lpszBuffer) : LB_ERR;
    }
    int GetTextLen(int nIndex) const {
        return m_hWnd ? (int)::SendMessageW(m_hWnd, LB_GETTEXTLEN, nIndex, 0) : LB_ERR;
    }
    int AddString(const wchar_t* lpszItem) {
        return m_hWnd ? (int)::SendMessageW(m_hWnd, LB_ADDSTRING, 0, (LPARAM)lpszItem) : LB_ERR;
    }
    int InsertString(int nIndex, const wchar_t* lpszItem) {
        return m_hWnd ? (int)::SendMessageW(m_hWnd, LB_INSERTSTRING, nIndex, (LPARAM)lpszItem) : LB_ERR;
    }
    int DeleteString(unsigned int nIndex) {
        return m_hWnd ? (int)::SendMessageW(m_hWnd, LB_DELETESTRING, nIndex, 0) : LB_ERR;
    }
    void ResetContent() {
        if (m_hWnd) ::SendMessageW(m_hWnd, LB_RESETCONTENT, 0, 0);
    }
    int FindString(int nStartAfter, const wchar_t* lpszItem) const {
        return m_hWnd ? (int)::SendMessageW(m_hWnd, LB_FINDSTRING, nStartAfter, (LPARAM)lpszItem) : LB_ERR;
    }
    int SelectString(int nStartAfter, const wchar_t* lpszItem) {
        return m_hWnd ? (int)::SendMessageW(m_hWnd, LB_SELECTSTRING, nStartAfter, (LPARAM)lpszItem) : LB_ERR;
    }
    uintptr_t GetItemData(int nIndex) const {
        return m_hWnd ? (uintptr_t)::SendMessageW(m_hWnd, LB_GETITEMDATA, nIndex, 0) : 0;
    }
    int SetItemData(int nIndex, uintptr_t dwItemData) {
        return m_hWnd ? (int)::SendMessageW(m_hWnd, LB_SETITEMDATA, nIndex, (LPARAM)dwItemData) : LB_ERR;
    }
    void* GetItemDataPtr(int nIndex) const {
        return m_hWnd ? (void*)::SendMessageW(m_hWnd, LB_GETITEMDATA, nIndex, 0) : nullptr;
    }
    int SetItemDataPtr(int nIndex, void* pData) {
        return m_hWnd ? (int)::SendMessageW(m_hWnd, LB_SETITEMDATA, nIndex, (LPARAM)pData) : LB_ERR;
    }
};

// CComboBox - Combo box control wrapper
class CComboBox : public CWnd {
    DECLARE_DYNAMIC(CComboBox)
public:
    CComboBox() = default;
    virtual ~CComboBox() = default;

    int Create(DWORD dwStyle, const struct tagRECT& rect, CWnd* pParentWnd, unsigned int nID) {
        return CWnd::Create(L"COMBOBOX", L"", dwStyle, rect, pParentWnd, nID);
    }

    int GetCount() const {
        return m_hWnd ? (int)::SendMessageW(m_hWnd, CB_GETCOUNT, 0, 0) : 0;
    }
    int GetCurSel() const {
        return m_hWnd ? (int)::SendMessageW(m_hWnd, CB_GETCURSEL, 0, 0) : CB_ERR;
    }
    int SetCurSel(int nSelect) {
        return m_hWnd ? (int)::SendMessageW(m_hWnd, CB_SETCURSEL, nSelect, 0) : CB_ERR;
    }
    int GetLBText(int nIndex, wchar_t* lpszText) const {
        return (m_hWnd && lpszText) ? (int)::SendMessageW(m_hWnd, CB_GETLBTEXT, nIndex, (LPARAM)lpszText) : CB_ERR;
    }
    int GetLBTextLen(int nIndex) const {
        return m_hWnd ? (int)::SendMessageW(m_hWnd, CB_GETLBTEXTLEN, nIndex, 0) : CB_ERR;
    }
    int AddString(const wchar_t* lpszString) {
        return m_hWnd ? (int)::SendMessageW(m_hWnd, CB_ADDSTRING, 0, (LPARAM)lpszString) : CB_ERR;
    }
    int InsertString(int nIndex, const wchar_t* lpszString) {
        return m_hWnd ? (int)::SendMessageW(m_hWnd, CB_INSERTSTRING, nIndex, (LPARAM)lpszString) : CB_ERR;
    }
    int DeleteString(unsigned int nIndex) {
        return m_hWnd ? (int)::SendMessageW(m_hWnd, CB_DELETESTRING, nIndex, 0) : CB_ERR;
    }
    void ResetContent() {
        if (m_hWnd) ::SendMessageW(m_hWnd, CB_RESETCONTENT, 0, 0);
    }
    int FindString(int nStartAfter, const wchar_t* lpszString) const {
        return m_hWnd ? (int)::SendMessageW(m_hWnd, CB_FINDSTRING, nStartAfter, (LPARAM)lpszString) : CB_ERR;
    }
    int SelectString(int nStartAfter, const wchar_t* lpszString) {
        return m_hWnd ? (int)::SendMessageW(m_hWnd, CB_SELECTSTRING, nStartAfter, (LPARAM)lpszString) : CB_ERR;
    }
    uintptr_t GetItemData(int nIndex) const {
        return m_hWnd ? (uintptr_t)::SendMessageW(m_hWnd, CB_GETITEMDATA, nIndex, 0) : 0;
    }
    int SetItemData(int nIndex, uintptr_t dwItemData) {
        return m_hWnd ? (int)::SendMessageW(m_hWnd, CB_SETITEMDATA, nIndex, (LPARAM)dwItemData) : CB_ERR;
    }
    void ShowDropDown(int bShowIt = 1) {
        if (m_hWnd) ::SendMessageW(m_hWnd, CB_SHOWDROPDOWN, bShowIt ? TRUE : FALSE, 0);
    }
    int GetDroppedState() const {
        return m_hWnd ? (int)::SendMessageW(m_hWnd, CB_GETDROPPEDSTATE, 0, 0) : FALSE;
    }
};

// CScrollBar - Scroll bar control wrapper
class CScrollBar : public CWnd {
    DECLARE_DYNAMIC(CScrollBar)
public:
    CScrollBar() = default;
    virtual ~CScrollBar() = default;

    int Create(DWORD dwStyle, const struct tagRECT& rect, CWnd* pParentWnd, unsigned int nID) {
        return CWnd::Create(L"SCROLLBAR", L"", dwStyle, rect, pParentWnd, nID);
    }

    int GetScrollPos() const {
        return m_hWnd ? ::GetScrollPos(m_hWnd, SB_CTL) : 0;
    }
    int SetScrollPos(int nPos, int bRedraw = 1) {
        return m_hWnd ? ::SetScrollPos(m_hWnd, SB_CTL, nPos, bRedraw ? TRUE : FALSE) : 0;
    }
    void GetScrollRange(int* lpMinPos, int* lpMaxPos) const {
        if (m_hWnd) {
            ::GetScrollRange(m_hWnd, SB_CTL, lpMinPos, lpMaxPos);
        } else {
            if (lpMinPos) *lpMinPos = 0;
            if (lpMaxPos) *lpMaxPos = 0;
        }
    }
    void SetScrollRange(int nMinPos, int nMaxPos, int bRedraw = 1) {
        if (m_hWnd) ::SetScrollRange(m_hWnd, SB_CTL, nMinPos, nMaxPos, bRedraw ? TRUE : FALSE);
    }
    int EnableScrollBar(unsigned int nArrowFlags = ESB_ENABLE_BOTH) {
        return m_hWnd ? ::EnableScrollBar(m_hWnd, SB_CTL, nArrowFlags) : FALSE;
    }
};

// CSliderCtrl - Slider/Trackbar control wrapper
class CSliderCtrl : public CWnd {
    DECLARE_DYNAMIC(CSliderCtrl)
public:
    CSliderCtrl() = default;
    virtual ~CSliderCtrl() = default;

    int Create(DWORD dwStyle, const struct tagRECT& rect, CWnd* pParentWnd, unsigned int nID) {
        return CWnd::CreateEx(0, L"msctls_trackbar32", L"", dwStyle, rect.left, rect.top,
            rect.right - rect.left, rect.bottom - rect.top,
            pParentWnd ? pParentWnd->GetSafeHwnd() : nullptr, (HMENU)(uintptr_t)nID, nullptr);
    }

    int GetPos() const {
        return m_hWnd ? (int)::SendMessageW(m_hWnd, TBM_GETPOS, 0, 0) : 0;
    }
    void SetPos(int nPos) {
        if (m_hWnd) ::SendMessageW(m_hWnd, TBM_SETPOS, TRUE, nPos);
    }
    void SetRange(int nMin, int nMax, int bRedraw = 0) {
        if (m_hWnd) ::SendMessageW(m_hWnd, TBM_SETRANGE, bRedraw ? TRUE : FALSE, MAKELPARAM(nMin, nMax));
    }
    void SetRangeMin(int nMin, int bRedraw = 0) {
        if (m_hWnd) ::SendMessageW(m_hWnd, TBM_SETRANGEMIN, bRedraw ? TRUE : FALSE, nMin);
    }
    void SetRangeMax(int nMax, int bRedraw = 0) {
        if (m_hWnd) ::SendMessageW(m_hWnd, TBM_SETRANGEMAX, bRedraw ? TRUE : FALSE, nMax);
    }
    int GetRangeMin() const {
        return m_hWnd ? (int)::SendMessageW(m_hWnd, TBM_GETRANGEMIN, 0, 0) : 0;
    }
    int GetRangeMax() const {
        return m_hWnd ? (int)::SendMessageW(m_hWnd, TBM_GETRANGEMAX, 0, 0) : 100;
    }
    void SetTicFreq(int nFreq) {
        if (m_hWnd) ::SendMessageW(m_hWnd, TBM_SETTICFREQ, nFreq, 0);
    }
    void SetPageSize(int nSize) {
        if (m_hWnd) ::SendMessageW(m_hWnd, TBM_SETPAGESIZE, 0, nSize);
    }
    int GetPageSize() const {
        return m_hWnd ? (int)::SendMessageW(m_hWnd, TBM_GETPAGESIZE, 0, 0) : 1;
    }
    void SetLineSize(int nSize) {
        if (m_hWnd) ::SendMessageW(m_hWnd, TBM_SETLINESIZE, 0, nSize);
    }
    int GetLineSize() const {
        return m_hWnd ? (int)::SendMessageW(m_hWnd, TBM_GETLINESIZE, 0, 0) : 1;
    }
};

// CProgressCtrl - Progress bar control wrapper
class CProgressCtrl : public CWnd {
    DECLARE_DYNAMIC(CProgressCtrl)
public:
    CProgressCtrl() = default;
    virtual ~CProgressCtrl() = default;

    int Create(DWORD dwStyle, const struct tagRECT& rect, CWnd* pParentWnd, unsigned int nID) {
        return CWnd::CreateEx(0, L"msctls_progress32", L"", dwStyle, rect.left, rect.top,
            rect.right - rect.left, rect.bottom - rect.top,
            pParentWnd ? pParentWnd->GetSafeHwnd() : nullptr, (HMENU)(uintptr_t)nID, nullptr);
    }

    int GetPos() const {
        return m_hWnd ? (int)::SendMessageW(m_hWnd, PBM_GETPOS, 0, 0) : 0;
    }
    int SetPos(int nPos) {
        return m_hWnd ? (int)::SendMessageW(m_hWnd, PBM_SETPOS, nPos, 0) : 0;
    }
    void SetRange(short nLower, short nUpper) {
        if (m_hWnd) ::SendMessageW(m_hWnd, PBM_SETRANGE, 0, MAKELPARAM(nLower, nUpper));
    }
    void SetRange32(int nLower, int nUpper) {
        if (m_hWnd) ::SendMessageW(m_hWnd, PBM_SETRANGE32, nLower, nUpper);
    }
    int OffsetPos(int nPos) {
        return m_hWnd ? (int)::SendMessageW(m_hWnd, PBM_DELTAPOS, nPos, 0) : 0;
    }
    int SetStep(int nStep) {
        return m_hWnd ? (int)::SendMessageW(m_hWnd, PBM_SETSTEP, nStep, 0) : 1;
    }
    int StepIt() {
        return m_hWnd ? (int)::SendMessageW(m_hWnd, PBM_STEPIT, 0, 0) : 0;
    }
    void SetMarquee(int fMarqueeMode, int nInterval) {
        if (m_hWnd) ::SendMessageW(m_hWnd, PBM_SETMARQUEE, fMarqueeMode ? TRUE : FALSE, nInterval);
    }
};

// CSpinButtonCtrl - Spin button (up-down) control wrapper
class CSpinButtonCtrl : public CWnd {
    DECLARE_DYNAMIC(CSpinButtonCtrl)
public:
    CSpinButtonCtrl() = default;
    virtual ~CSpinButtonCtrl() = default;

    int Create(DWORD dwStyle, const struct tagRECT& rect, CWnd* pParentWnd, unsigned int nID) {
        return CWnd::CreateEx(0, L"msctls_updown32", L"", dwStyle, rect.left, rect.top,
            rect.right - rect.left, rect.bottom - rect.top,
            pParentWnd ? pParentWnd->GetSafeHwnd() : nullptr, (HMENU)(uintptr_t)nID, nullptr);
    }

    int GetPos() const {
        return m_hWnd ? (int)LOWORD(::SendMessageW(m_hWnd, UDM_GETPOS, 0, 0)) : 0;
    }
    int SetPos(int nPos) {
        return m_hWnd ? (int)::SendMessageW(m_hWnd, UDM_SETPOS, 0, MAKELPARAM(nPos, 0)) : 0;
    }
    void SetRange(short nLower, short nUpper) {
        if (m_hWnd) ::SendMessageW(m_hWnd, UDM_SETRANGE, 0, MAKELPARAM(nUpper, nLower));
    }
    void SetRange32(int nLower, int nUpper) {
        if (m_hWnd) ::SendMessageW(m_hWnd, UDM_SETRANGE32, nLower, nUpper);
    }
    void GetRange(int& lower, int& upper) const {
        if (m_hWnd) {
            DWORD dwRange = (DWORD)::SendMessageW(m_hWnd, UDM_GETRANGE, 0, 0);
            lower = (short)HIWORD(dwRange);
            upper = (short)LOWORD(dwRange);
        } else {
            lower = 0; upper = 100;
        }
    }
    void GetRange32(int& lower, int& upper) const {
        if (m_hWnd) {
            ::SendMessageW(m_hWnd, UDM_GETRANGE32, (WPARAM)&lower, (LPARAM)&upper);
        } else {
            lower = 0; upper = 100;
        }
    }
    CWnd* SetBuddy(CWnd* pWndBuddy) {
        // Returns previous buddy - we don't track it
        if (m_hWnd) ::SendMessageW(m_hWnd, UDM_SETBUDDY, (WPARAM)(pWndBuddy ? pWndBuddy->GetSafeHwnd() : nullptr), 0);
        return nullptr;
    }
    CWnd* GetBuddy() const {
        // Would need to wrap the HWND - return nullptr for now
        return nullptr;
    }
    unsigned int SetBase(int nBase) {
        return m_hWnd ? (unsigned int)::SendMessageW(m_hWnd, UDM_SETBASE, nBase, 0) : 10;
    }
    unsigned int GetBase() const {
        return m_hWnd ? (unsigned int)::SendMessageW(m_hWnd, UDM_GETBASE, 0, 0) : 10;
    }
};

// CListCtrl - List view control wrapper
class CListCtrl : public CWnd {
    DECLARE_DYNAMIC(CListCtrl)
public:
    CListCtrl() = default;
    virtual ~CListCtrl() = default;

    int Create(DWORD dwStyle, const struct tagRECT& rect, CWnd* pParentWnd, unsigned int nID) {
        return CWnd::CreateEx(0, L"SysListView32", L"", dwStyle, rect.left, rect.top,
            rect.right - rect.left, rect.bottom - rect.top,
            pParentWnd ? pParentWnd->GetSafeHwnd() : nullptr, (HMENU)(uintptr_t)nID, nullptr);
    }

    int GetItemCount() const {
        return m_hWnd ? (int)::SendMessageW(m_hWnd, LVM_GETITEMCOUNT, 0, 0) : 0;
    }
    int InsertItem(int nItem, const wchar_t* lpszItem) {
        if (!m_hWnd) return -1;
        LVITEMW lvi = {};
        lvi.mask = LVIF_TEXT;
        lvi.iItem = nItem;
        lvi.pszText = const_cast<wchar_t*>(lpszItem);
        return (int)::SendMessageW(m_hWnd, LVM_INSERTITEMW, 0, (LPARAM)&lvi);
    }
    int DeleteItem(int nItem) {
        return m_hWnd ? (int)::SendMessageW(m_hWnd, LVM_DELETEITEM, nItem, 0) : 0;
    }
    int DeleteAllItems() {
        return m_hWnd ? (int)::SendMessageW(m_hWnd, LVM_DELETEALLITEMS, 0, 0) : 0;
    }
    int GetItemText(int nItem, int nSubItem, wchar_t* lpszText, int nLen) const {
        if (!m_hWnd || !lpszText) return 0;
        LVITEMW lvi = {};
        lvi.iSubItem = nSubItem;
        lvi.cchTextMax = nLen;
        lvi.pszText = lpszText;
        return (int)::SendMessageW(m_hWnd, LVM_GETITEMTEXTW, nItem, (LPARAM)&lvi);
    }
    int SetItemText(int nItem, int nSubItem, const wchar_t* lpszText) {
        if (!m_hWnd) return 0;
        LVITEMW lvi = {};
        lvi.iSubItem = nSubItem;
        lvi.pszText = const_cast<wchar_t*>(lpszText);
        return (int)::SendMessageW(m_hWnd, LVM_SETITEMTEXTW, nItem, (LPARAM)&lvi);
    }
    uintptr_t GetItemData(int nItem) const {
        if (!m_hWnd) return 0;
        LVITEMW lvi = {};
        lvi.mask = LVIF_PARAM;
        lvi.iItem = nItem;
        ::SendMessageW(m_hWnd, LVM_GETITEMW, 0, (LPARAM)&lvi);
        return (uintptr_t)lvi.lParam;
    }
    int SetItemData(int nItem, uintptr_t dwData) {
        if (!m_hWnd) return 0;
        LVITEMW lvi = {};
        lvi.mask = LVIF_PARAM;
        lvi.iItem = nItem;
        lvi.lParam = (LPARAM)dwData;
        return (int)::SendMessageW(m_hWnd, LVM_SETITEMW, 0, (LPARAM)&lvi);
    }
    int GetSelectedCount() const {
        return m_hWnd ? (int)::SendMessageW(m_hWnd, LVM_GETSELECTEDCOUNT, 0, 0) : 0;
    }
    int GetNextItem(int nItem, int nFlags) const {
        return m_hWnd ? (int)::SendMessageW(m_hWnd, LVM_GETNEXTITEM, nItem, MAKELPARAM(nFlags, 0)) : -1;
    }
    int GetSelectionMark() const {
        return m_hWnd ? (int)::SendMessageW(m_hWnd, LVM_GETSELECTIONMARK, 0, 0) : -1;
    }
    int SetSelectionMark(int iIndex) {
        return m_hWnd ? (int)::SendMessageW(m_hWnd, LVM_SETSELECTIONMARK, 0, iIndex) : -1;
    }
    int InsertColumn(int nCol, const wchar_t* lpszColumnHeading, int nFormat = 0, int nWidth = -1, int nSubItem = -1) {
        if (!m_hWnd) return -1;
        LVCOLUMNW lvc = {};
        lvc.mask = LVCF_TEXT | LVCF_FMT;
        lvc.pszText = const_cast<wchar_t*>(lpszColumnHeading);
        lvc.fmt = nFormat;
        if (nWidth >= 0) { lvc.mask |= LVCF_WIDTH; lvc.cx = nWidth; }
        if (nSubItem >= 0) { lvc.mask |= LVCF_SUBITEM; lvc.iSubItem = nSubItem; }
        return (int)::SendMessageW(m_hWnd, LVM_INSERTCOLUMNW, nCol, (LPARAM)&lvc);
    }
    int DeleteColumn(int nCol) {
        return m_hWnd ? (int)::SendMessageW(m_hWnd, LVM_DELETECOLUMN, nCol, 0) : 0;
    }
};

// CTreeCtrl - Tree view control wrapper
class CTreeCtrl : public CWnd {
    DECLARE_DYNAMIC(CTreeCtrl)
public:
    CTreeCtrl() = default;
    virtual ~CTreeCtrl() = default;

    int Create(DWORD dwStyle, const struct tagRECT& rect, CWnd* pParentWnd, unsigned int nID) {
        return CWnd::CreateEx(0, L"SysTreeView32", L"", dwStyle, rect.left, rect.top,
            rect.right - rect.left, rect.bottom - rect.top,
            pParentWnd ? pParentWnd->GetSafeHwnd() : nullptr, (HMENU)(uintptr_t)nID, nullptr);
    }

    void* InsertItem(const wchar_t* lpszItem, void* hParent = TVI_ROOT, void* hInsertAfter = TVI_LAST) {
        if (!m_hWnd) return nullptr;
        TVINSERTSTRUCTW tvis = {};
        tvis.hParent = (HTREEITEM)hParent;
        tvis.hInsertAfter = (HTREEITEM)hInsertAfter;
        tvis.item.mask = TVIF_TEXT;
        tvis.item.pszText = const_cast<wchar_t*>(lpszItem);
        return (void*)::SendMessageW(m_hWnd, TVM_INSERTITEMW, 0, (LPARAM)&tvis);
    }
    int DeleteItem(void* hItem) {
        return m_hWnd ? (int)::SendMessageW(m_hWnd, TVM_DELETEITEM, 0, (LPARAM)hItem) : 0;
    }
    int DeleteAllItems() {
        return m_hWnd ? (int)::SendMessageW(m_hWnd, TVM_DELETEITEM, 0, (LPARAM)TVI_ROOT) : 0;
    }
    int GetItemText(void* hItem, wchar_t* lpszText, int nLen) const {
        if (!m_hWnd || !lpszText) return 0;
        TVITEMW tvi = {};
        tvi.mask = TVIF_TEXT;
        tvi.hItem = (HTREEITEM)hItem;
        tvi.pszText = lpszText;
        tvi.cchTextMax = nLen;
        return ::SendMessageW(m_hWnd, TVM_GETITEMW, 0, (LPARAM)&tvi) ? (int)wcslen(lpszText) : 0;
    }
    int SetItemText(void* hItem, const wchar_t* lpszText) {
        if (!m_hWnd) return 0;
        TVITEMW tvi = {};
        tvi.mask = TVIF_TEXT;
        tvi.hItem = (HTREEITEM)hItem;
        tvi.pszText = const_cast<wchar_t*>(lpszText);
        return (int)::SendMessageW(m_hWnd, TVM_SETITEMW, 0, (LPARAM)&tvi);
    }
    void* GetSelectedItem() const {
        return m_hWnd ? (void*)::SendMessageW(m_hWnd, TVM_GETNEXTITEM, TVGN_CARET, 0) : nullptr;
    }
    int SelectItem(void* hItem) {
        return m_hWnd ? (int)::SendMessageW(m_hWnd, TVM_SELECTITEM, TVGN_CARET, (LPARAM)hItem) : 0;
    }
    int Expand(void* hItem, unsigned int nCode) {
        return m_hWnd ? (int)::SendMessageW(m_hWnd, TVM_EXPAND, nCode, (LPARAM)hItem) : 0;
    }
    void* GetRootItem() const {
        return m_hWnd ? (void*)::SendMessageW(m_hWnd, TVM_GETNEXTITEM, TVGN_ROOT, 0) : nullptr;
    }
    void* GetChildItem(void* hItem) const {
        return m_hWnd ? (void*)::SendMessageW(m_hWnd, TVM_GETNEXTITEM, TVGN_CHILD, (LPARAM)hItem) : nullptr;
    }
    void* GetNextSiblingItem(void* hItem) const {
        return m_hWnd ? (void*)::SendMessageW(m_hWnd, TVM_GETNEXTITEM, TVGN_NEXT, (LPARAM)hItem) : nullptr;
    }
    void* GetParentItem(void* hItem) const {
        return m_hWnd ? (void*)::SendMessageW(m_hWnd, TVM_GETNEXTITEM, TVGN_PARENT, (LPARAM)hItem) : nullptr;
    }
    uintptr_t GetItemData(void* hItem) const {
        if (!m_hWnd) return 0;
        TVITEMW tvi = {};
        tvi.mask = TVIF_PARAM;
        tvi.hItem = (HTREEITEM)hItem;
        ::SendMessageW(m_hWnd, TVM_GETITEMW, 0, (LPARAM)&tvi);
        return (uintptr_t)tvi.lParam;
    }
    int SetItemData(void* hItem, uintptr_t dwData) {
        if (!m_hWnd) return 0;
        TVITEMW tvi = {};
        tvi.mask = TVIF_PARAM;
        tvi.hItem = (HTREEITEM)hItem;
        tvi.lParam = (LPARAM)dwData;
        return (int)::SendMessageW(m_hWnd, TVM_SETITEMW, 0, (LPARAM)&tvi);
    }
};

// CTabCtrl - Tab control wrapper
class CTabCtrl : public CWnd {
    DECLARE_DYNAMIC(CTabCtrl)
public:
    CTabCtrl() = default;
    virtual ~CTabCtrl() = default;

    int Create(DWORD dwStyle, const struct tagRECT& rect, CWnd* pParentWnd, unsigned int nID) {
        return CWnd::CreateEx(0, L"SysTabControl32", L"", dwStyle, rect.left, rect.top,
            rect.right - rect.left, rect.bottom - rect.top,
            pParentWnd ? pParentWnd->GetSafeHwnd() : nullptr, (HMENU)(uintptr_t)nID, nullptr);
    }

    int GetItemCount() const {
        return m_hWnd ? (int)::SendMessageW(m_hWnd, TCM_GETITEMCOUNT, 0, 0) : 0;
    }
    int GetCurSel() const {
        return m_hWnd ? (int)::SendMessageW(m_hWnd, TCM_GETCURSEL, 0, 0) : -1;
    }
    int SetCurSel(int nItem) {
        return m_hWnd ? (int)::SendMessageW(m_hWnd, TCM_SETCURSEL, nItem, 0) : -1;
    }
    long InsertItem(int nItem, const wchar_t* lpszItem) {
        if (!m_hWnd) return -1;
        TCITEMW tci = {};
        tci.mask = TCIF_TEXT;
        tci.pszText = const_cast<wchar_t*>(lpszItem);
        return (long)::SendMessageW(m_hWnd, TCM_INSERTITEMW, nItem, (LPARAM)&tci);
    }
    int DeleteItem(int nItem) {
        return m_hWnd ? (int)::SendMessageW(m_hWnd, TCM_DELETEITEM, nItem, 0) : 0;
    }
    int DeleteAllItems() {
        return m_hWnd ? (int)::SendMessageW(m_hWnd, TCM_DELETEALLITEMS, 0, 0) : 0;
    }
    void AdjustRect(int bLarger, struct tagRECT* lpRect) {
        if (m_hWnd && lpRect) ::SendMessageW(m_hWnd, TCM_ADJUSTRECT, bLarger ? TRUE : FALSE, (LPARAM)lpRect);
    }
};

//=============================================================================
// Document-View Architecture Classes
//=============================================================================

// Forward declarations
class CDocTemplate;
class CDocument;
class CView;
class CArchive;

// CDocument - Document base class
class CDocument : public CCmdTarget {
    DECLARE_DYNCREATE(CDocument)
public:
    CDocument();
    virtual ~CDocument();

    // Document operations
    virtual int OnNewDocument();
    virtual int OnOpenDocument(const wchar_t* lpszPathName);
    virtual int OnSaveDocument(const wchar_t* lpszPathName);
    virtual void OnCloseDocument();
    virtual void DeleteContents();
    
    virtual void SetModifiedFlag(int bModified = 1);
    virtual int IsModified() const;
    
    virtual void SetTitle(const wchar_t* lpszTitle);
    virtual const wchar_t* GetTitle() const;
    
    virtual void SetPathName(const wchar_t* lpszPathName, int bAddToMRU = 1);
    virtual const wchar_t* GetPathName() const;
    
    // View management
    virtual void AddView(CView* pView);
    virtual void RemoveView(CView* pView);
    virtual void* GetFirstViewPosition() const;
    virtual CView* GetNextView(void*& pos) const;
    virtual void UpdateAllViews(CView* pSender = nullptr, unsigned long lHint = 0, CObject* pHint = nullptr);
    
    // File operations
    virtual int SaveModified();
    virtual int DoSave(const wchar_t* lpszPathName = nullptr, int bReplace = 1);
    virtual int DoFileSave();
    
    // Serialization
    virtual void Serialize(CArchive& ar) override;
    
public:
    CString m_strTitle;           // Document title
    CString m_strPathName;        // Full path to document file
    int m_bModified;              // Modified flag
    int m_bAutoDelete;            // Auto-delete flag

    // View list (simplified - real MFC uses CPtrList)
    CView* m_pFirstView;          // First view in list
    CView* m_pLastView;           // Last view in list

    // For template's document list
    CDocument* m_pNextDoc;        // Next document in template's list

protected:
    // Padding for ABI compatibility
    char _document_padding[56];
};

// CView - View base class
class CView : public CWnd {
    DECLARE_DYNAMIC(CView)
public:
    CView();
    virtual ~CView();

    // Document access
    virtual CDocument* GetDocument() const;
    virtual void OnUpdate(CView* pSender, unsigned long lHint = 0, CObject* pHint = nullptr);
    virtual void OnInitialUpdate();
    
    // Drawing
    virtual void OnDraw(void* pDC) = 0; // CDC* pDC
    
    // Printing
    virtual int OnPreparePrinting(void* pInfo); // CPrintInfo* pInfo
    virtual void OnBeginPrinting(void* pDC, void* pInfo); // CDC* pDC, CPrintInfo* pInfo
    virtual void OnEndPrinting(void* pDC, void* pInfo); // CDC* pDC, CPrintInfo* pInfo
    
    // Activation
    virtual void OnActivateView(int bActivate, CView* pActivateView, CView* pDeactiveView);
    
    // Frame window
    virtual CFrameWnd* GetParentFrame() const;
    
public:
    CDocument* m_pDocument;       // Associated document
    CView* m_pNextView;           // Next view in document's view list
    
protected:
    // Padding for ABI compatibility
    char _view_padding[56];
};

// CScrollView - Scrollable view
class CScrollView : public CView {
    DECLARE_DYNCREATE(CScrollView)
public:
    CScrollView();
    virtual ~CScrollView();

    // Scroll operations
    virtual void SetScrollSizes(int nMapMode, const SIZE& sizeTotal,
                                const SIZE& sizePage = {0, 0},
                                const SIZE& sizeLine = {0, 0});
    virtual void GetScrollBarSizes(SIZE& sizeSb) const;
    virtual void GetTrueClientSize(SIZE& size, SIZE& sizeSb) const;
    
    virtual void ScrollToPosition(POINT pt);
    virtual void GetScrollPosition(POINT& pt) const;
    
    virtual void FillOutsideRect(void* pDC, void* pBrush); // CDC* pDC, CBrush* pBrush
    virtual void ResizeParentToFit(int bShrinkOnly = 1);
    
    // Overrides
    virtual void OnDraw(void* pDC) override;
    virtual void OnInitialUpdate() override;
    virtual void OnUpdate(CView* pSender, unsigned long lHint = 0, CObject* pHint = nullptr) override;
    
public:
    SIZE m_totalLog;    // Total logical size
    SIZE m_pageDev;     // Page size in device units
    SIZE m_lineDev;     // Line size in device units
    int m_nMapMode;               // Mapping mode

protected:
    // Padding for ABI compatibility
    char _scrollview_padding[64];
};

// CFormView - Form-based view
class CFormView : public CScrollView {
    DECLARE_DYNCREATE(CFormView)
public:
    CFormView();
    CFormView(const wchar_t* lpszTemplateName);
    CFormView(unsigned int nIDTemplate);
    virtual ~CFormView();

    // Creation
    virtual int Create(const wchar_t* lpszClassName, const wchar_t* lpszWindowName,
                       DWORD dwStyle, const struct tagRECT& rect,
                       CWnd* pParentWnd, unsigned int nID, CCreateContext* pContext = nullptr) override;
    
    // Dialog data exchange
    virtual void DoDataExchange(void* pDX); // CDataExchange* pDX
    
    // Overrides
    virtual void OnInitialUpdate() override;
    virtual void OnDraw(void* pDC) override;
    
public:
    const wchar_t* m_lpszTemplateName; // Dialog template name
    unsigned int m_nIDHelp;            // Help ID
    
protected:
    // Padding for ABI compatibility
    char _formview_padding[64];
};

// CEditView - Edit control-based view
class CEditView : public CView {
    DECLARE_DYNCREATE(CEditView)
public:
    CEditView();
    virtual ~CEditView();

    // Text operations
    virtual void GetSelectedText(CString& strResult) const;
    virtual int FindText(const wchar_t* lpszFind, int bNext = 1, int bCase = 1);
    virtual int PrintInsideRect(void* pDC, struct tagRECT& rectLayout, 
                                struct tagRECT& rectPage, int bPrintOnly = 0); // CDC* pDC
    
    // Overrides
    virtual void OnDraw(void* pDC) override;
    virtual void Serialize(CArchive& ar) override;
    
    // Edit control access
    CEdit* GetEditCtrl() const;

public:
    CEdit* m_pEditCtrl;           // Embedded edit control

protected:
    // Padding for ABI compatibility
    char _editview_padding[64];
};

// CListView - List control-based view
class CListView : public CView {
    DECLARE_DYNCREATE(CListView)
public:
    CListView();
    virtual ~CListView();

    // List control access
    CListCtrl* GetListCtrl() const;

    // Overrides
    virtual void OnDraw(void* pDC) override;
    virtual void OnInitialUpdate() override;

public:
    CListCtrl* m_pListCtrl;       // Embedded list control

protected:
    // Padding for ABI compatibility
    char _listview_padding[64];
};

// CTreeView - Tree control-based view
class CTreeView : public CView {
    DECLARE_DYNCREATE(CTreeView)
public:
    CTreeView();
    virtual ~CTreeView();

    // Tree control access
    CTreeCtrl* GetTreeCtrl() const;

    // Overrides
    virtual void OnDraw(void* pDC) override;
    virtual void OnInitialUpdate() override;

public:
    CTreeCtrl* m_pTreeCtrl;       // Embedded tree control

protected:
    // Padding for ABI compatibility
    char _treeview_padding[64];
};

// CDocTemplate - Document template base class
class CDocTemplate : public CCmdTarget {
    DECLARE_DYNAMIC(CDocTemplate)
public:
    CDocTemplate(unsigned int nIDResource, CRuntimeClass* pDocClass,
                 CRuntimeClass* pFrameClass, CRuntimeClass* pViewClass);
    virtual ~CDocTemplate();

    // Document creation
    virtual CDocument* CreateNewDocument();

protected:
    CDocTemplate();  // Default constructor for derived classes

public:
    virtual CFrameWnd* CreateNewFrame(CDocument* pDoc = nullptr, CFrameWnd* pOther = nullptr);
    virtual int CreateAndReplaceFrame(CFrameWnd* pTargetFrame, CDocument* pDoc);
    
    // Document management
    virtual CDocument* OpenDocumentFile(const wchar_t* lpszPathName = nullptr, int bMakeVisible = 1);
    virtual void InitialUpdateFrame(CFrameWnd* pFrame, CDocument* pDoc, int bMakeVisible = 1);
    virtual void SetDefaultTitle(CDocument* pDocument);
    
    // String resources
    virtual int GetDocString(CString& rString, int nID) const;
    
    // Matching
    virtual int MatchDocType(const wchar_t* lpszPathName, CDocument*& rpDocMatch);
    
    // Document list
    virtual void AddDocument(CDocument* pDoc);
    virtual void RemoveDocument(CDocument* pDoc);
    virtual void* GetFirstDocPosition() const;
    virtual CDocument* GetNextDoc(void*& rPos) const;
    
public:
    CRuntimeClass* m_pDocClass;   // Document class
    CRuntimeClass* m_pFrameClass; // Frame class
    CRuntimeClass* m_pViewClass;  // View class
    
    unsigned int m_nIDResource;   // Resource ID for menu/icon/etc.
    
    // Document list (simplified)
    CDocument* m_pFirstDoc;       // First document in list
    CDocument* m_pLastDoc;        // Last document in list
    
protected:
    // Padding for ABI compatibility
    char _doctemplate_padding[64];
};

// CSingleDocTemplate - Single document interface template
class CSingleDocTemplate : public CDocTemplate {
    DECLARE_DYNCREATE(CSingleDocTemplate)
public:
    CSingleDocTemplate();
    CSingleDocTemplate(unsigned int nIDResource, CRuntimeClass* pDocClass,
                       CRuntimeClass* pFrameClass, CRuntimeClass* pViewClass);
    virtual ~CSingleDocTemplate();

    // Overrides
    virtual CDocument* OpenDocumentFile(const wchar_t* lpszPathName = nullptr, int bMakeVisible = 1) override;
    virtual void AddDocument(CDocument* pDoc) override;
    virtual void RemoveDocument(CDocument* pDoc) override;
    
public:
    CDocument* m_pOnlyDoc;        // Single document
    
protected:
    // Padding for ABI compatibility
    char _singledoctemplate_padding[64];
};

// CMultiDocTemplate - Multiple document interface template
class CMultiDocTemplate : public CDocTemplate {
    DECLARE_DYNCREATE(CMultiDocTemplate)
public:
    CMultiDocTemplate();
    CMultiDocTemplate(unsigned int nIDResource, CRuntimeClass* pDocClass,
                      CRuntimeClass* pFrameClass, CRuntimeClass* pViewClass);
    virtual ~CMultiDocTemplate();

    // Overrides
    virtual CDocument* OpenDocumentFile(const wchar_t* lpszPathName = nullptr, int bMakeVisible = 1) override;
    
protected:
    // Padding for ABI compatibility
    char _multidoctemplate_padding[64];
};

//=============================================================================
// Inline stub implementations for header-only use
// These provide minimal implementations to satisfy linking requirements
// when not linking with the full implementation files.
//=============================================================================

// CCmdTarget and CWinThread implementations
// Guard against redefinition when linking with appcore.cpp
#ifndef OPENMFC_APPCORE_IMPL
inline CCmdTarget::~CCmdTarget() {}
inline int CCmdTarget::OnCmdMsg(unsigned int, int, void*, void*) { return 0; }
inline const AFX_MSGMAP* CCmdTarget::GetMessageMap() const { return nullptr; }

// CWinThread implementations
inline CWinThread::CWinThread()
    : m_pMainWnd(nullptr), m_nThreadID(0), m_hThread(nullptr), 
      m_bAutoDelete(FALSE), m_nThreadPriority(0), m_nThreadStackSize(0),
      m_dwThreadCreateFlags(0), m_bRunning(FALSE), m_bSuspended(FALSE) {
    memset(&m_msgCur, 0, sizeof(m_msgCur));
}
inline CWinThread::~CWinThread() {}
inline int CWinThread::Run() { return 0; }
inline BOOL CWinThread::PreTranslateMessage(MSG*) { return FALSE; }
inline BOOL CWinThread::OnIdle(LONG) { return FALSE; }
inline BOOL CWinThread::IsIdleMessage(MSG*) { return FALSE; }
inline BOOL CWinThread::PumpMessage() { return FALSE; }
inline BOOL CWinThread::PrePumpMessage() { return TRUE; }
inline BOOL CWinThread::PostPumpMessage() { return TRUE; }
inline BOOL CWinThread::InitInstance() { return FALSE; }
inline int CWinThread::ExitInstance() { return 0; }

#endif // OPENMFC_APPCORE_IMPL
//=============================================================================
// End of inline implementations
//=============================================================================
