#pragma once
#include "afxwin.h"

// The release Unicode x64 thread state used by imported MFC clients.
class CNoTrackObject {
public:
    static void* operator new(size_t bytes);
    static void operator delete(void* memory);
    virtual ~CNoTrackObject() = default;
};
struct AFX_EXCEPTION_LINK;
struct AFX_EXCEPTION_CONTEXT { AFX_EXCEPTION_LINK* m_pLinkTop; };
class CPushRoutingView;
class CPushRoutingFrame;
class _AFX_THREAD_STATE : public CNoTrackObject {
public:
    _AFX_THREAD_STATE();
    ~_AFX_THREAD_STATE() override;
    AFX_MODULE_STATE* m_pModuleState;
    AFX_MODULE_STATE* m_pPrevModuleState;
    void* m_pSafetyPoolBuffer;
    AFX_EXCEPTION_CONTEXT m_exceptionContext;
    CWnd* m_pWndInit;
    CWnd* m_pAlternateWndInit;
    DWORD m_dwPropStyle,m_dwPropExStyle;
    HWND m_hWndInit;
    HHOOK m_hHookOldCbtFilter,m_hHookOldMsgFilter;
    MSG m_msgCur;
    CPoint m_ptCursorLast;
    UINT m_nMsgLast;
    MSG m_lastSentMsg;
    HWND m_hTrackingWindow;
    HMENU m_hTrackingMenu;
    wchar_t m_szTempClassName[96];
    HWND m_hLockoutNotifyWindow;
    BOOL m_bInMsgFilter;
    CView* m_pRoutingView;
    CPushRoutingView* m_pPushRoutingView;
    CFrameWnd* m_pRoutingFrame;
    CPushRoutingFrame* m_pPushRoutingFrame;
    BOOL m_bWaitForDataSource;
    CWnd* m_pWndPark;
    LONG m_nCtrlRef;
    BOOL m_bNeedTerm;
};
static_assert(sizeof(_AFX_THREAD_STATE)==0x1e0,"native Unicode thread state size");
static_assert(offsetof(_AFX_THREAD_STATE,m_pWndInit)==0x28,"native pending window");
static_assert(offsetof(_AFX_THREAD_STATE,m_hHookOldCbtFilter)==0x48,"native CBT hook");
static_assert(offsetof(_AFX_THREAD_STATE,m_msgCur)==0x58,"native current message");
static_assert(offsetof(_AFX_THREAD_STATE,m_lastSentMsg)==0x98,"native sent message");
static_assert(offsetof(_AFX_THREAD_STATE,m_pRoutingView)==0x1a8,"native routing view");
static_assert(offsetof(_AFX_THREAD_STATE,m_pRoutingFrame)==0x1b8,"native routing frame");
extern "C" _AFX_THREAD_STATE* AFXAPI impl__AfxGetThreadState__YAPEAV_AFX_THREAD_STATE__XZ();
extern "C" AFX_MODULE_STATE* AFXAPI impl__AfxSetModuleState__YAPEAVAFX_MODULE_STATE__PEAV1__Z(AFX_MODULE_STATE*);
