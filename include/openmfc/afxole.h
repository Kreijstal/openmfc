// OpenMFC OLE/COM Classes
// ABI-compatible declarations for mfc140u.dll OLE exports
#pragma once

#ifndef OPENMFC_AFXOLE_H
#define OPENMFC_AFXOLE_H

#include "afxwin.h"
#include "afxdisp.h"
#include <objbase.h>
#include <oleauto.h>
#include <oleidl.h>
#include <ocidl.h>
#include <docobj.h>
#include <urlmon.h>
#include <oledlg.h>
#include <cstring>

// MinGW compat: DROPEFFECT is used in OLE but sometimes missing from headers
#ifndef DROPEFFECT
typedef DWORD DROPEFFECT;
#endif

// MFC-specific OLE types
typedef DWORD OLE_STATUS;
typedef DWORD OLE_CLOSE;
typedef DWORD OLE_NOTIFICATION;

// OLE item state constants
#ifndef OLE_EMPTY
#define OLE_EMPTY 0
#define OLE_LOADED 1
#define OLE_RUNNING 2
#define OLE_ACTIVE 3
#define OLE_OPEN 4
#define OLE_CHANGED 5
#endif

// AFX_IDW_RESIZE_BAR constant
#ifndef AFX_IDW_RESIZE_BAR
#define AFX_IDW_RESIZE_BAR 0xE803
#endif

//=============================================================================
// Forward declarations and base classes
//=============================================================================
class CString;
class CPropExchange;
class CArchive;
class CView;

// POSITION typedef (used by MFC collection iteration)
typedef void* POSITION;

// OLE application type constants
#ifndef OLE_APPTYPE
typedef DWORD OLE_APPTYPE;
#endif
#ifndef OAT_INPLACE_SERVER
#define OAT_INPLACE_SERVER 0
#define OAT_SERVER 1
#define OAT_CONTAINER 2
#define OAT_DISPATCH_OBJECT 3
#define OAT_DOC_OBJECT_SERVER 4
#endif

// MFC OLE-related base classes not yet in our main headers
class CCmdUI {
public:
    CCmdUI() : m_nID(0), m_pOther(nullptr), m_nIndex(0), m_pMenu(nullptr), m_pSubMenu(nullptr) {}
    virtual ~CCmdUI() {}
    virtual void Enable(BOOL bOn = TRUE);
    virtual void SetCheck(int nCheck = 1);
    virtual void SetText(const wchar_t* lpszText);
    UINT m_nID;
    CCmdUI* m_pOther;
    int m_nIndex;
    CMenu* m_pMenu;
    CMenu* m_pSubMenu;
    char _ccmdui_padding[8];
};

class CControlBar : public CWnd {
    DECLARE_DYNAMIC(CControlBar)
public:
    CControlBar();
    virtual ~CControlBar();
    virtual BOOL Create(CWnd* pParentWnd, DWORD dwStyle, UINT nID);
    int m_nCount;
    CWnd* m_pDockSite;
    void* m_pDockBar;
    void* m_pDockContext;
    DWORD m_dwStyle;
    CRect m_rectBorder;
    char _controlbar_padding[64];
};

// AFX_IDW_* and CBRS_* constants (must be before CToolBar/CStatusBar usage)
#ifndef AFX_IDW_TOOLBAR
#define AFX_IDW_TOOLBAR 0xE800
#endif
#ifndef AFX_IDW_STATUS_BAR
#define AFX_IDW_STATUS_BAR 0xE801
#endif
#ifndef AFX_IDW_DIALOGBAR
#define AFX_IDW_DIALOGBAR 0xE802
#endif
#ifndef CBRS_TOP
#define CBRS_TOP 0x0001
#define CBRS_BOTTOM 0x0002
#define CBRS_LEFT 0x0004
#define CBRS_RIGHT 0x0008
#define CBRS_ALIGN_ANY 0x000F
#define CBRS_FLOATING 0x0010
#define CBRS_SIZE_DYNAMIC 0x0020
#define CBRS_SIZE_FIXED 0x0040
#define CBRS_HIDE_INPLACE 0x0080
#endif

//=============================================================================
// CToolTipCtrl - Tooltip control (must be before CToolBar)
//=============================================================================
class CToolTipCtrl : public CWnd {
public:
    CToolTipCtrl() {}
    virtual ~CToolTipCtrl() {}
    BOOL Create(CWnd* pParentWnd, DWORD dwStyle = 0);
    BOOL AddTool(CWnd* pWnd, const wchar_t* pszText, LPCRECT lpRectTool = nullptr, UINT_PTR nIDTool = 0);
    void UpdateTipText(const wchar_t* pszText, CWnd* pWnd, UINT_PTR nIDTool = 0);
    void Activate(BOOL bActivate = TRUE);
    void SetMaxTipWidth(int iWidth);
    int GetText(CWnd* pWnd, UINT_PTR nIDTool, wchar_t* pszText, int cchMax) const;
    char _tooltip_padding[64];
};

//=============================================================================
// CToolBar - Toolbar Control Bar
//=============================================================================
class CToolBar : public CControlBar {
    DECLARE_DYNAMIC(CToolBar)
public:
    CToolBar();
    virtual ~CToolBar();

    // Creation
    virtual BOOL Create(CWnd* pParentWnd, DWORD dwStyle = WS_CHILD | WS_VISIBLE | CBRS_TOP,
                        UINT nID = AFX_IDW_TOOLBAR);
    virtual BOOL CreateEx(CWnd* pParentWnd, DWORD dwCtrlStyle = TBSTYLE_FLAT,
                          DWORD dwStyle = WS_CHILD | WS_VISIBLE | CBRS_TOP,
                          CRect rcBorders = CRect(0, 0, 0, 0),
                          UINT nID = AFX_IDW_TOOLBAR);

    // Loading
    BOOL LoadToolBar(UINT nIDResource);
    BOOL LoadToolBar(const wchar_t* lpszResourceName);
    BOOL LoadBitmap(UINT nIDResource);
    BOOL LoadBitmap(const wchar_t* lpszResourceName);

    // Button management
    BOOL SetButtons(const UINT* lpIDArray, int nIDCount);
    BOOL SetButtonInfo(int nIndex, UINT nID, UINT nStyle, int iImage);
    void GetButtonInfo(int nIndex, UINT& nID, UINT& nStyle, int& iImage) const;
    int CommandToIndex(UINT nIDFind) const;
    UINT GetItemID(int nIndex) const;
    void GetItemRect(int nIndex, LPRECT lpRect) const;
    int GetButtonCount() const { return m_nCount; }

    // Sizing
    void SetSizes(SIZE sizeButton, SIZE sizeImage);
    CSize GetButtonSize() const;
    SIZE GetToolBarCtrlSize() const;
    void SetHeight(int cyHeight);
    int GetHeight() const;

    // Appearance
    void SetButtonStyle(int nIndex, UINT nStyle);
    UINT GetButtonStyle(int nIndex) const;
    void SetButtonText(int nIndex, const wchar_t* lpszText);
    CString GetButtonText(int nIndex) const;
    void GetButtonText(int nIndex, CString& rString) const;
    BOOL SetBitmap(HBITMAP hbmImageWell);

    // Tooltip support
    void SetToolTips(CToolTipCtrl* pToolTip);
    CToolTipCtrl* GetToolTips() const;

    // Docking support
    void EnableDocking(DWORD dwDockStyle);
    void DockControlBar(UINT nBarID, DWORD dwFlags = 0);

    // State
    BOOL IsVisible() const;
    BOOL IsFloating() const;
    DWORD GetBarStyle() const { return m_dwStyle; }
    void SetBarStyle(DWORD dwStyle) { m_dwStyle = dwStyle; }

    // Internal access
    HWND GetToolBarCtrl() const { return m_hWnd; }

    // Toolbar buttons structure
    struct TBBUTTON {
        int iBitmap;
        int idCommand;
        BYTE fsState;
        BYTE fsStyle;
        DWORD_PTR dwData;
        INT_PTR iString;
    };

public:
    int m_nCount;
    SIZE m_sizeButton;
    SIZE m_sizeImage;
    HBITMAP m_hbmImageWell;
    CToolTipCtrl* m_pToolTip;
    CWnd* m_pDockBar;
    CWnd* m_pDockSite;
    void* m_pDockContext;
    BOOL m_bInRecalcLayout;
    BOOL m_bDelayedButtonLayout;

protected:
    char _toolbar_padding[128];
};

//=============================================================================
// CStatusBar - Status Bar Control Bar
//=============================================================================
class CStatusBar : public CControlBar {
    DECLARE_DYNAMIC(CStatusBar)
public:
    CStatusBar();
    virtual ~CStatusBar();

    // Creation
    virtual BOOL Create(CWnd* pParentWnd, DWORD dwStyle = WS_CHILD | WS_VISIBLE | CBRS_BOTTOM,
                        UINT nID = AFX_IDW_STATUS_BAR);

    // Pane management
    BOOL SetIndicators(const UINT* lpIDArray, int nIDCount);
    BOOL SetPaneInfo(int nIndex, UINT nID, UINT nStyle, int cxWidth);
    void GetPaneInfo(int nIndex, UINT& nID, UINT& nStyle, int& cxWidth) const;
    int CommandToIndex(UINT nIDFind) const;
    UINT GetItemID(int nIndex) const;
    void GetItemRect(int nIndex, LPRECT lpRect) const;
    int GetCount() const { return m_nCount; }

    // Text and appearance
    BOOL SetPaneText(int nIndex, const wchar_t* lpszNewText, BOOL bUpdate = TRUE);
    CString GetPaneText(int nIndex) const;
    void GetPaneText(int nIndex, CString& rString) const;
    void SetPaneStyle(int nIndex, UINT nStyle);
    UINT GetPaneStyle(int nIndex) const;

    // Sizing
    CSize GetPaneSize(int nIndex) const;
    CSize GetBorders() const;

    // Tooltip
    void SetToolTips(CToolTipCtrl* pToolTip);
    CToolTipCtrl* GetToolTips() const;

    // Docking
    void EnableDocking(DWORD dwDockStyle);
    BOOL IsSimple() const;

public:
    int m_nCount;
    UINT* m_pData;
    int m_cxLeftBorder;
    int m_cxRightBorder;
    CToolTipCtrl* m_pToolTip;

protected:
    char _statusbar_padding[96];
};

//=============================================================================
// CDialogBar - Dialog-based Control Bar
//=============================================================================
class CDialogBar : public CControlBar {
    DECLARE_DYNAMIC(CDialogBar)
public:
    CDialogBar();
    virtual ~CDialogBar();

    // Creation
    virtual BOOL Create(CWnd* pParentWnd, UINT nIDTemplate, UINT nStyle,
                        UINT nID = AFX_IDW_DIALOGBAR);
    virtual BOOL Create(CWnd* pParentWnd, const wchar_t* lpszTemplateName,
                        UINT nStyle, UINT nID = AFX_IDW_DIALOGBAR);

    // Dialog data
    void UpdateData(BOOL bSaveAndValidate = TRUE);
    BOOL IsVisible() const;
    void SetOccDialogInfo(void* pDialogInfo);

public:
    BOOL m_bAutoDelete;
    void* m_pOccDialogInfo;

protected:
    char _dialogbar_padding[48];
};

//=============================================================================
// CSplitterWnd - Splitter Window
//=============================================================================
#ifndef AFX_IDW_PANE_FIRST
#define AFX_IDW_PANE_FIRST 0xE900
#endif

class CSplitterWnd : public CWnd {
    DECLARE_DYNAMIC(CSplitterWnd)
public:
    CSplitterWnd();
    virtual ~CSplitterWnd();

    // Creation
    BOOL Create(CWnd* pParentWnd, int nMaxRows, int nMaxCols, SIZE sizeMin,
                CCreateContext* pContext = nullptr, DWORD dwStyle = WS_CHILD | WS_VISIBLE,
                UINT nID = AFX_IDW_PANE_FIRST);
    BOOL CreateStatic(CWnd* pParentWnd, int nRows, int nCols,
                      DWORD dwStyle = WS_CHILD | WS_VISIBLE, UINT nID = AFX_IDW_PANE_FIRST);
    BOOL CreateView(int row, int col, CRuntimeClass* pViewClass, SIZE sizeInit,
                    CCreateContext* pContext = nullptr);

    // Pane management
    CWnd* GetPane(int row, int col) const;
    int GetRowCount() const { return m_nRows; }
    int GetColumnCount() const { return m_nCols; }
    void GetRowInfo(int row, int& cyCur, int& cyMin) const;
    void SetRowInfo(int row, int cyIdeal, int cyMin);
    void GetColumnInfo(int col, int& cxCur, int& cxMin) const;
    void SetColumnInfo(int col, int cxIdeal, int cxMin);
    void RecalcLayout();

    // Splitter bar
    void SetSplitCursor(int ht);
    int GetActivePane(int* pRow = nullptr, int* pCol = nullptr) const;
    void SetActivePane(int row, int col, CWnd* pWnd = nullptr);
    CWnd* GetActivePane();

    // Scrolling
    BOOL CanActivateNext(BOOL bPrev = FALSE);
    void ActivateNext(BOOL bPrev = FALSE);
    BOOL DoKeyboardSplit();

    // Overrides
    virtual void OnDrawSplitter(CDC* pDC, int nType, const CRect& rect);
    virtual void OnInvertTracker(const CRect& rect);
    virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);

public:
    int m_nRows;
    int m_nCols;
    int m_cxSplitter;
    int m_cySplitter;
    int m_cxBorderShare;
    int m_cyBorderShare;
    int m_cxSplitterGap;
    int m_cySplitterGap;
    int m_nMaxRows;
    int m_nMaxCols;
    SIZE m_sizeMin;
    int m_nId;
    int m_bHasHScroll;
    int m_bHasVScroll;
    CWnd* m_pActivePane;
    int m_nActiveRow;
    int m_nActiveCol;

protected:
    char _splitterwnd_padding[128];
};

//=============================================================================
// CTaskDialog - Vista+ Task Dialog
//=============================================================================
class CTaskDialog {
public:
    // Button IDs
    enum {
        TDCBF_OK_BUTTON = 0x0001,
        TDCBF_YES_BUTTON = 0x0002,
        TDCBF_NO_BUTTON = 0x0004,
        TDCBF_CANCEL_BUTTON = 0x0008,
        TDCBF_RETRY_BUTTON = 0x0010,
        TDCBF_CLOSE_BUTTON = 0x0020
    };

    CTaskDialog(const wchar_t* pszContent, const wchar_t* pszMainInstruction,
                const wchar_t* pszWindowTitle, int nCommonButtons = TDCBF_OK_BUTTON,
                int nTaskDialogOptions = 0);
    virtual ~CTaskDialog();

    // Configuration
    void SetDialogWidth(int nWidth = 0);
    void SetMainIcon(HICON hMainIcon);
    void SetMainIcon(UINT nMainIconID);
    void SetFooterIcon(HICON hFooterIcon);
    void SetFooterText(const wchar_t* pszFooterText);
    void SetVerificationCheckboxText(const wchar_t* pszText);
    void SetExpandedInformation(const wchar_t* pszText);
    void SetExpandedControlText(const wchar_t* pszText);
    void SetCollapsedControlText(const wchar_t* pszText);
    void SetProgressBarRange(int nMin, int nMax);
    void SetProgressBarPosition(int nPos);
    void SetProgressBarMarquee(BOOL bMarquee = TRUE, int nSpeed = 0);

    // Adding controls
    HRESULT AddCommandControl(int nCommandID, const wchar_t* pszLabel);
    HRESULT AddRadioButton(int nRadioButtonID, const wchar_t* pszLabel);
    HRESULT AddPushButton(int nButtonID, const wchar_t* pszLabel);

    // Display
    int DoModal(HWND hWndParent = nullptr);
    BOOL GetVerificationCheckboxState() const { return m_bVerificationChecked; }
    int GetSelectedCommandControlID() const { return m_nSelectedCommandID; }
    int GetSelectedRadioButtonID() const { return m_nSelectedRadioButtonID; }
    BOOL IsExpanded() const { return m_bExpanded; }

public:
    CString m_strContent;
    CString m_strMainInstruction;
    CString m_strWindowTitle;
    CString m_strFooterText;
    CString m_strVerificationText;
    CString m_strExpandedInfo;
    CString m_strExpandedControlText;
    CString m_strCollapsedControlText;

    int m_nCommonButtons;
    int m_nTaskDialogOptions;
    int m_nDialogWidth;
    HICON m_hMainIcon;
    HICON m_hFooterIcon;
    int m_nProgressMin;
    int m_nProgressMax;
    int m_nProgressPos;
    BOOL m_bProgressMarquee;
    BOOL m_bVerificationChecked;
    int m_nSelectedCommandID;
    int m_nSelectedRadioButtonID;
    BOOL m_bExpanded;

protected:
    char _taskdialog_padding[64];
};

// CDocItem - base for OLE client/server items
class CDocItem : public CObject {
    DECLARE_DYNAMIC(CDocItem)
public:
    CDocItem();
    virtual ~CDocItem();
    CDocument* m_pDocument;
    char _docitem_padding[16];
};

// COleDocument forward (needed by CDocItem subclasses)
class COleDocument;
class COleLinkingDoc;
class COleServerDoc;
class COleServerItem;
class COleClientItem;
class COleDataSource;
class COleDropTarget;
class COleDropSource;
class COleDataObject;
class COleMessageFilter;
class COleObjectFactory;
class COleTemplateServer;
class COleControlSite;
class COleControlContainer;
class COleIPFrameWnd;
class COleFrameHook;
class COleCmdUI;
class COleResizeBar;
class COleStreamFile;
class COlePropertyPage;
class COleControl;
class COleControlModule;
class COleSafeArray;
class COleConnPtContainer;
class COleDialog;
class COleBusyDialog;
class COleChangeIconDialog;
class COleChangeSourceDialog;
class COleConvertDialog;
class COleInsertDialog;
class COleLinksDialog;
class COlePasteSpecialDialog;
class COlePropertiesDialog;
class COleUpdateDialog;
class COleUILinkInfo;
class COleDocIPFrameWnd;
class COleCntrFrameWnd;
class COleControlSiteOrWnd;
class CEnumOleVerb;

//=============================================================================
// COleCurrency - OLE Automation Currency type
//=============================================================================
class COleCurrency {
public:
    COleCurrency() { m_cur.int64 = 0; m_status = CY_VALID; }
    COleCurrency(long nUnits, long nFractionalUnits);
    COleCurrency(const CY& cySrc);
    COleCurrency(const COleCurrency& curSrc);
    ~COleCurrency() {}

    // Attributes
    CURRENCY m_cur;
    enum CurrencyStatus { CY_VALID = 0, CY_INVALID = 1, CY_NULL = 2 };
    CurrencyStatus m_status;

    void SetStatus(CurrencyStatus status) { m_status = status; }
    CurrencyStatus GetStatus() const { return m_status; }
    void SetCurrency(long nUnits, long nFractionalUnits);
    operator CURRENCY() const { return m_cur; }
    BOOL GetAsDBLCY(double* pdbl) const { if (pdbl) *pdbl = (double)m_cur.int64 / 10000.0; return TRUE; }

    COleCurrency& operator=(const CY& cySrc) { m_cur = cySrc; m_status = CY_VALID; return *this; }
    COleCurrency& operator=(const COleCurrency& curSrc);

    char _olecurrency_padding[16];
};

//=============================================================================
// COleDateTime - OLE Automation Date/Time type
//=============================================================================
class COleDateTime {
public:
    COleDateTime() { m_dt = 0; m_status = DT_VALID; }
    COleDateTime(const DATE& dtSrc) : m_dt(dtSrc), m_status(DT_VALID) {}
    COleDateTime(const COleDateTime& dtSrc) : m_dt(dtSrc.m_dt), m_status(dtSrc.m_status) {}
    ~COleDateTime() {}

    DATE m_dt;
    enum DateTimeStatus { DT_VALID = 0, DT_INVALID = 1, DT_NULL = 2 };
    DateTimeStatus m_status;

    void SetStatus(DateTimeStatus status) { m_status = status; }
    DateTimeStatus GetStatus() const { return m_status; }
    operator DATE() const { return m_dt; }
    BOOL GetAsDBLTIME(double* pdbl) const { if (pdbl) *pdbl = m_dt; return TRUE; }

    static COleDateTime GetCurrentTime();

    COleDateTime& operator=(const DATE& dtSrc) { m_dt = dtSrc; m_status = DT_VALID; return *this; }
    COleDateTime& operator=(const COleDateTime& dtSrc);

    char _oledatetime_padding[16];
};

//=============================================================================
// COleSafeArray - Safe Array wrapper
//=============================================================================
class COleSafeArray : public SAFEARRAY {
public:
    COleSafeArray() { cbElements = 0; cDims = 0; pvData = nullptr; }
    ~COleSafeArray() { Destroy(); }

    void Destroy();
    void Clear();
    void Create(VARTYPE vt, DWORD cDims, SAFEARRAYBOUND* rgsabound);
    void AccessData(void** ppvData);
    void UnaccessData();
    void Attach(const SAFEARRAY& saSrc);
    SAFEARRAY* Detach();
    void Copy(const COleSafeArray* psaSrc);

    char _olesafearray_padding[24];
};

//=============================================================================
// COleDataObject - OLE Data Object wrapper
//=============================================================================
class COleDataObject {
public:
    COleDataObject();
    ~COleDataObject();

    void Attach(LPDATAOBJECT lpDataObject, BOOL bAutoRelease = TRUE);
    LPDATAOBJECT Detach();
    void Release();

    // Format enumeration
    void BeginEnumFormats();
    BOOL GetNextFormat(FORMATETC* lpFormatEtc);
    BOOL IsDataAvailable(CLIPFORMAT cfFormat, FORMATETC* lpFormatEtc = nullptr);

    // Data retrieval
    HGLOBAL GetGlobalData(CLIPFORMAT cfFormat, FORMATETC* lpFormatEtc = nullptr);
    BOOL GetData(CLIPFORMAT cfFormat, STGMEDIUM* lpStorageMedium, FORMATETC* lpFormatEtc = nullptr);

    // IDataObject access
    LPDATAOBJECT GetIDataObject(BOOL bAddRef = FALSE) const;

public:
    LPDATAOBJECT m_lpDataObject;
    IEnumFORMATETC* m_lpEnumFmtEtc;
    BOOL m_bAutoRelease;

protected:
    char _oledataobject_padding[32];
};

//=============================================================================
// COleDataSource - OLE Data Source
//=============================================================================
class COleDataSource : public CCmdTarget {
    DECLARE_DYNAMIC(COleDataSource)
public:
    COleDataSource();
    virtual ~COleDataSource();

    // Cache management
    void CacheGlobalData(CLIPFORMAT cfFormat, HGLOBAL hGlobal, FORMATETC* lpFormatEtc = nullptr);
    void CacheData(CLIPFORMAT cfFormat, STGMEDIUM* lpStorageMedium, FORMATETC* lpFormatEtc = nullptr);
    void DelayRenderData(CLIPFORMAT cfFormat, FORMATETC* lpFormatEtc = nullptr);
    void DelayRenderFileData(CLIPFORMAT cfFormat, FORMATETC* lpFormatEtc = nullptr);
    void SetClipboard();

    // Clipboard operations
    static COleDataSource* PASCAL GetClipboardOwner();

    // Drag and Drop
    DROPEFFECT DoDragDrop(DWORD dwEffects = DROPEFFECT_COPY | DROPEFFECT_MOVE | DROPEFFECT_LINK,
                          LPCRECT lpRectStartDrag = nullptr,
                          COleDropSource* pDropSource = nullptr);
    void OnRenderGlobalData(FORMATETC* lpFormatEtc, HGLOBAL* phGlobal);
    void OnRenderFileData(FORMATETC* lpFormatEtc, CFile* pFile);

    // Data provision
    virtual HGLOBAL OnRenderGlobalData(LPFORMATETC lpFormatEtc);
    virtual HGLOBAL OnRenderFileData(LPFORMATETC lpFormatEtc);
    void Empty();

    // IDataObject interface
    LPDATAOBJECT GetInterface(BOOL bAddRef = FALSE);

public:
    BOOL m_bClipboardOwner;
    void* m_pDataCache; // internal cache

protected:
    char _oledatasource_padding[96];
};

//=============================================================================
// COleDropTarget - OLE Drag and Drop Target
//=============================================================================
class COleDropTarget : public CCmdTarget {
    DECLARE_DYNAMIC(COleDropTarget)
public:
    COleDropTarget();
    virtual ~COleDropTarget();

    BOOL Register(CWnd* pWnd);
    void Revoke();

    // Drop target callbacks
    virtual DROPEFFECT OnDragEnter(CWnd* pWnd, COleDataObject* pDataObject,
                                   DWORD dwKeyState, CPoint point);
    virtual DROPEFFECT OnDragOver(CWnd* pWnd, COleDataObject* pDataObject,
                                  DWORD dwKeyState, CPoint point);
    virtual void OnDragLeave(CWnd* pWnd);
    virtual BOOL OnDrop(CWnd* pWnd, COleDataObject* pDataObject,
                        DROPEFFECT dropEffect, CPoint point);
    virtual DROPEFFECT OnDropEx(CWnd* pWnd, COleDataObject* pDataObject,
                                DROPEFFECT dropDefault, DROPEFFECT dropList, CPoint point);
    virtual DROPEFFECT OnDragScroll(CWnd* pWnd, DWORD dwKeyState, CPoint point);

public:
    ULONG m_lRefCount;
    CWnd* m_pWnd;
    BOOL m_bRegistered;

protected:
    char _oledroptarget_padding[56];
};

//=============================================================================
// COleDropSource - OLE Drag and Drop Source
//=============================================================================
class COleDropSource : public CCmdTarget {
    DECLARE_DYNAMIC(COleDropSource)
public:
    COleDropSource();
    virtual ~COleDropSource();

    virtual SCODE QueryContinueDrag(BOOL bEscapePressed, DWORD dwKeyState);
    virtual SCODE GiveFeedback(DROPEFFECT dropEffect);

public:
    ULONG m_lRefCount;

protected:
    char _oledropsource_padding[32];
};

//=============================================================================
// COleMessageFilter - OLE Message Filter
//=============================================================================
class COleMessageFilter : public CCmdTarget {
    DECLARE_DYNAMIC(COleMessageFilter)
public:
    COleMessageFilter();
    virtual ~COleMessageFilter();

    // Registration
    static COleMessageFilter* PASCAL Register();
    static void PASCAL Revoke();

    // Busy dialog
    BOOL EnableBusyDialog(BOOL bEnableBusy = TRUE);
    BOOL EnableNotRespondingDialog(BOOL bEnableNotResponding = TRUE);
    void SetBusyReply(SERVERCALL nBusyReply);
    void SetRetryReply(DWORD nRetryReply);
    void SetMessagePendingDelay(DWORD nTimeout = 5000);
    void BeginBusyState();
    void EndBusyState();
    BOOL IsBusy() const;

    // Filter overrides
    virtual BOOL OnMessagePending(const MSG* pMsg);
    virtual int OnBusyDialog(HTASK hTaskBusy);

public:
    int m_nBusyCount;
    BOOL m_bEnableBusy;
    BOOL m_bEnableNotResponding;
    SERVERCALL m_nBusyReply;
    DWORD m_nRetryReply;
    DWORD m_nTimeout;

protected:
    char _olemessagefilter_padding[64];
};

//=============================================================================
// COleCmdUI - OLE Command UI
//=============================================================================
class COleCmdUI : public CCmdUI {
public:
    COleCmdUI(OLECMD* rgCmds, ULONG cCmds, const GUID* pGuid);
    virtual ~COleCmdUI();

    virtual void Enable(BOOL bOn = TRUE) override;
    virtual void SetCheck(int nCheck = 1) override;
    virtual void SetText(const wchar_t* lpszText) override;

public:
    OLECMD* m_pCmd;
    ULONG m_cCmds;
    const GUID* m_pGuid;
    OLECMD* m_rgCmds;

protected:
    char _olecmdui_padding[16];
};

//=============================================================================
// COleFrameHook - OLE In-place Frame Hook
//=============================================================================
class COleFrameHook : public CCmdTarget {
    DECLARE_DYNAMIC(COleFrameHook)
public:
    COleFrameHook(CFrameWnd* pFrameWnd, COleClientItem* pItem);
    virtual ~COleFrameHook();

    virtual BOOL OnDocActivate(BOOL bActivate);
    virtual BOOL OnContextHelp();
    virtual BOOL OnResizeBorder(LPCRECT lpRect, LPOLEINPLACEUIWINDOW lpUIWindow, BOOL bMainWindow);
    virtual BOOL OnGetClipboardData(COleDataSource* pDataSource);

    CFrameWnd* GetFrameWnd() const { return m_pFrameWnd; }

public:
    CFrameWnd* m_pFrameWnd;
    COleClientItem* m_pActiveItem;
    LPOLEINPLACEUIWINDOW m_lpActiveUIWindow;

protected:
    char _oleframehook_padding[48];
};

//=============================================================================
// COleResizeBar - OLE Resize Bar
//=============================================================================
class COleResizeBar : public CControlBar {
    DECLARE_DYNAMIC(COleResizeBar)
public:
    COleResizeBar();
    virtual ~COleResizeBar();

    virtual BOOL Create(CWnd* pParentWnd, DWORD dwStyle = WS_CHILD | WS_VISIBLE, UINT nID = AFX_IDW_RESIZE_BAR);

public:
    COleClientItem* m_pClientItem;
    int m_nHandleSize;

protected:
    char _oleresizebar_padding[24];
};

//=============================================================================
// COleStreamFile - OLE Stream File
//=============================================================================
class COleStreamFile : public CFile {
public:
    COleStreamFile(LPSTREAM lpStream = nullptr);
    virtual ~COleStreamFile();

    LPSTREAM GetStream() const;
    BOOL OpenStream(LPSTORAGE lpStorage, const wchar_t* lpszStreamName, DWORD nOpenFlags = STGM_READWRITE | STGM_SHARE_EXCLUSIVE);
    BOOL CreateStream(LPSTORAGE lpStorage, const wchar_t* lpszStreamName, DWORD nOpenFlags = STGM_READWRITE | STGM_SHARE_EXCLUSIVE | STGM_CREATE);
    void Attach(LPSTREAM lpStream);
    LPSTREAM Detach();

    // Override CFile
    virtual UINT Read(void* lpBuf, UINT nCount) override;
    virtual void Write(const void* lpBuf, UINT nCount) override;
    virtual ULONGLONG Seek(LONGLONG lOff, UINT nFrom) override;
    void LockRange(ULONGLONG dwPos, ULONGLONG dwCount);
    void UnlockRange(ULONGLONG dwPos, ULONGLONG dwCount);

public:
    LPSTREAM m_lpStream;

protected:
    char _olestreamfile_padding[32];
};

//=============================================================================
// COlePropertyPage - OLE Property Page
//=============================================================================
class COlePropertyPage : public CDialog {
    DECLARE_DYNAMIC(COlePropertyPage)
public:
    COlePropertyPage();
    COlePropertyPage(UINT nIDTemplate, UINT nIDCaption = 0);
    virtual ~COlePropertyPage();

    void SetPageName(const wchar_t* pszName);
    void SetHelpInfo(const wchar_t* lpszDocString, const wchar_t* lpszHelpFile = nullptr, DWORD dwHelpContext = 0);

    // IPropertyPage helpers
    virtual BOOL OnSetPageSite();
    virtual void OnObjectsChanged();
    virtual BOOL OnApply();
    virtual void OnEditProperty(DISPID dispid);

    BOOL IsModified() const { return m_bModified; }
    void SetModified(BOOL bModified = TRUE) { m_bModified = bModified; }

    IPropertyPageSite* GetPageSite() const { return m_pPageSite; }

public:
    IPropertyPageSite* m_pPageSite;
    BOOL m_bModified;
    wchar_t* m_pszPageName;
    wchar_t* m_pszHelpFile;
    DWORD m_dwHelpContext;
    wchar_t* m_pszDocString;

protected:
    char _olepropertypage_padding[32];
};

//=============================================================================
// COleDocument - OLE Container Document
//=============================================================================
class COleDocument : public CDocument {
    DECLARE_DYNAMIC(COleDocument)
public:
    COleDocument();
    virtual ~COleDocument();

    // Item management
    void AddItem(COleClientItem* pItem);
    void RemoveItem(COleClientItem* pItem);
    COleClientItem* GetStartPosition() const;
    COleClientItem* GetNextClientItem(POSITION& pos) const;
    COleClientItem* GetPrimarySelectedItem(CView* pView = nullptr) const;
    int GetItemCount() const;

    // In-place activation
    void EnableCompoundFile(BOOL bEnable = TRUE);
    BOOL HasBlankItems() const;
    BOOL IsInPlaceActive() const;
    void OnShowViews(BOOL bVisible);
    virtual COleClientItem* OnGetLinkedItem(const wchar_t* lpszItemName);
    virtual COleClientItem* OnGetEmbeddedItem();

    // Notifications
    virtual void OnEditChangeIcon(COleClientItem* pItem);
    virtual void OnEditConvert(COleClientItem* pItem);
    virtual void OnEditLinks();

    // OLE support
    LPOLEITEMCONTAINER GetItemContainer();

public:
    BOOL m_bCompoundFile;
    BOOL m_bRemember;

protected:
    char _coledocument_padding[48];
};

//=============================================================================
// COleLinkingDoc - OLE Linking Document
//=============================================================================
class COleLinkingDoc : public COleDocument {
    DECLARE_DYNAMIC(COleLinkingDoc)
public:
    COleLinkingDoc();
    virtual ~COleLinkingDoc();

    // Moniker support
    LPMONIKER GetMoniker(OLEGETMONIKER nAssign);
    virtual LPMONIKER GetFileMoniker();
    virtual BOOL RegisterIfServerAttached(const wchar_t* lpszFileName, BOOL bSetModified);
    void Revoke();
    BOOL IsRegistered() const;

    // Overrides
    virtual void OnShowDocument(BOOL bShow);

public:
    BOOL m_bRegistered;
    LPMONIKER m_lpMoniker;

protected:
    char _colelinkingdoc_padding[32];
};

//=============================================================================
// COleServerDoc - OLE Server Document
//=============================================================================
class COleServerDoc : public COleLinkingDoc {
    DECLARE_DYNAMIC(COleServerDoc)
public:
    COleServerDoc();
    virtual ~COleServerDoc();

    // Server activation
    void NotifyChanged();
    void NotifyClosed();
    void NotifyRename(const wchar_t* lpszNewName);
    void NotifySaved();
    void SaveEmbedding();

    // Client tracking
    COleClientItem* GetEmbeddedItem();
    COleServerItem* GetEmbeddedServerItem();  // Returns COleServerItem instead
    COleServerItem* GetLinkedServerItem(const wchar_t* lpszItemName);
    virtual BOOL OnSetHostNames(const wchar_t* lpszHost, const wchar_t* lpszHostObj);

    // Server state
    BOOL IsEmbedded() const { return m_bEmbedded; }

public:
    BOOL m_bEmbedded;

protected:
    char _coleserverdoc_padding[32];
};

//=============================================================================
// COleClientItem - OLE Client Item
//=============================================================================
class COleClientItem : public CDocItem {
    DECLARE_DYNAMIC(COleClientItem)
public:
    COleClientItem(COleDocument* pContainerDoc = nullptr);
    virtual ~COleClientItem();

    // Creation
    BOOL CreateFromClipboard(OLERENDER render = OLERENDER_DRAW,
                             CLIPFORMAT cfFormat = 0, FORMATETC* lpFormatEtc = nullptr);
    BOOL CreateNewItem(REFCLSID clsid, OLERENDER render = OLERENDER_DRAW,
                       CLIPFORMAT cfFormat = 0, FORMATETC* lpFormatEtc = nullptr);
    BOOL CreateStaticFromClipboard(OLERENDER render = OLERENDER_DRAW,
                                   CLIPFORMAT cfFormat = 0, FORMATETC* lpFormatEtc = nullptr);
    BOOL CreateFromData(COleDataObject* pDataObject, OLERENDER render = OLERENDER_DRAW,
                        CLIPFORMAT cfFormat = 0, FORMATETC* lpFormatEtc = nullptr);
    BOOL CreateLinkFromClipboard(OLERENDER render = OLERENDER_DRAW,
                                 CLIPFORMAT cfFormat = 0, FORMATETC* lpFormatEtc = nullptr);

    // Activation
    void Activate(LONG nVerb = OLEIVERB_PRIMARY, CView* pView = nullptr, HWND hwndParent = nullptr,
                  LPCRECT lpRect = nullptr, LPCRECT lpClipRect = nullptr,
                  BOOL bSplit = FALSE);
    void Deactivate();
    void DoVerb(LONG nVerb, CView* pView = nullptr, LPCRECT lpRect = nullptr);
    BOOL IsInPlaceActive() const;

    // State
    OLE_STATUS GetItemState() const;
    void SetItemState(OLE_STATUS nNewState);
    void Close(OLE_CLOSE dwCloseOption = OLECLOSE_SAVEIFDIRTY);

    // Drawing
    BOOL Draw(CDC* pDC, LPCRECT lpBounds, DVASPECT nDrawAspect = DVASPECT_CONTENT);

    // Linking
    BOOL GetExtent(LPSIZE lpSize, DVASPECT nDrawAspect = DVASPECT_CONTENT);
    BOOL SetExtent(const CSize& size, DVASPECT nDrawAspect = DVASPECT_CONTENT);

    // Storage
    void GetClassID(CLSID* pClassID);
    LPOLEOBJECT GetObject() const { return m_lpObject; }
    LPOLELINK GetLinkObject() const { return m_lpLink; }
    LPOLEINPLACEOBJECT GetInPlaceObject() const { return m_lpInPlaceObject; }

    // Clipboard
    void CopyToClipboard(BOOL bIncludeLink = FALSE);
    static COleClientItem* PASCAL GetClipboardData();

    // Overrides
    virtual void OnChange(OLE_NOTIFICATION nCode, DWORD dwParam);
    virtual void OnActivate();
    virtual void OnDeactivate();
    virtual void OnGetItemPosition(CRect& rPosition);
    virtual BOOL OnChangeItemPosition(const CRect& rectPos);
    virtual void OnDiscardUndoState();

    // OLE Control support
    void SetControlSite(COleControlSite* pSite);
    COleControlSite* GetControlSite() const;

public:
    COleDocument* m_pContainerDoc;
    LPOLEOBJECT m_lpObject;
    LPOLELINK m_lpLink;
    LPOLEINPLACEOBJECT m_lpInPlaceObject;
    OLE_STATUS m_nStatus;
    int m_nDrawAspect;
    COleControlSite* m_pControlSite;
    LPOLEINPLACEFRAME m_lpFrame;
    LPOLEINPLACEUIWINDOW m_lpDocFrame;
    BOOL m_bInPlaceActive;

protected:
    char _oleclientitem_padding[96];
};

//=============================================================================
// COleServerItem - OLE Server Item
//=============================================================================
class COleServerItem : public CDocItem {
    DECLARE_DYNAMIC(COleServerItem)
public:
    COleServerItem(COleServerDoc* pServerDoc, BOOL bAutoDelete = FALSE);
    virtual ~COleServerItem();

    // Drawing / rendering
    virtual BOOL OnDraw(CDC* pDC, CSize& rSize);
    virtual BOOL OnDrawEx(CDC* pDC, DVASPECT nDrawAspect, CSize& rSize);
    virtual BOOL OnGetExtent(DVASPECT nDrawAspect, CSize& rSize);
    virtual void OnSetExtent(DVASPECT nDrawAspect, const CSize& size);

    // Data
    virtual void Serialize(CArchive& ar) override;
    COleDataSource* GetDataSource();
    void CopyToClipboard(BOOL bIncludeLink = FALSE);

    // Embedding support
    void NotifyChanged();
    BOOL IsConnected() const;

    // Document
    COleServerDoc* GetDocument() const;

public:
    COleServerDoc* m_pServerDoc;
    BOOL m_bAutoDelete;

protected:
    char _oleserveritem_padding[48];
};

//=============================================================================
// COleObjectFactory - OLE Class Factory
//=============================================================================
class COleObjectFactory : public CCmdTarget {
    DECLARE_DYNAMIC(COleObjectFactory)
public:
    COleObjectFactory();
    COleObjectFactory(REFCLSID clsid, CRuntimeClass* pRuntimeClass,
                      BOOL bMultiInstance, const wchar_t* lpszProgID);
    virtual ~COleObjectFactory();

    BOOL Register();
    void Revoke();
    BOOL IsRegistered() const;
    BOOL IsLicenseValid();
    BOOL RegisterAll();
    static BOOL PASCAL UpdateRegistryAll(BOOL bRegister = TRUE);
    static void PASCAL RevokeAll();

    virtual CCmdTarget* OnCreateObject();
    virtual BOOL OnVerifyFile(LPCTSTR lpszFileName);
    virtual void UpdateRegistry(BOOL bRegister = TRUE);

    CLSID GetClassID() const { return m_clsid; }
    const wchar_t* GetProgID() const { return m_strProgID; }

    // Connection points
    void ConnectTemplate(COleTemplateServer* pTemplate);
    COleTemplateServer* GetTemplate() const { return m_pTemplate; }

public:
    CLSID m_clsid;
    CRuntimeClass* m_pRuntimeClass;
    BOOL m_bMultiInstance;
    CString m_strProgID;
    DWORD m_dwRegister;
    COleTemplateServer* m_pTemplate;

protected:
    char _oleobjectfactory_padding[48];
};

//=============================================================================
// COleTemplateServer - OLE Template Server
//=============================================================================
class COleTemplateServer : public COleObjectFactory {
    DECLARE_DYNAMIC(COleTemplateServer)
public:
    COleTemplateServer();
    virtual ~COleTemplateServer();

    void ConnectTemplate(REFCLSID clsid, CDocTemplate* pDocTemplate, BOOL bMultiInstance);
    CDocTemplate* GetDocTemplate() const { return m_pDocTemplate; }

    // Registration
    virtual void UpdateRegistry(OLE_APPTYPE nAppType = OAT_INPLACE_SERVER,
                               const wchar_t** rglpszRegister = nullptr,
                               const wchar_t** rglpszOverwrite = nullptr,
                               BOOL bRegister = TRUE);

public:
    CDocTemplate* m_pDocTemplate;

protected:
    char _oletemplateserver_padding[24];
};

//=============================================================================
// COleControlSite - OLE Control Site
//=============================================================================
class COleControlSite : public CCmdTarget {
    DECLARE_DYNAMIC(COleControlSite)
public:
    COleControlSite(COleControlContainer* pCtrlCont = nullptr);
    virtual ~COleControlSite();

    // Control management
    BOOL CreateControl(CWnd* pWndCtrl, REFCLSID clsid, const wchar_t* lpszWindowName,
                       DWORD dwStyle, const RECT& rect, UINT nID, CFile* pPersist = nullptr,
                       BOOL bStorage = FALSE, BSTR bstrLicKey = nullptr);
    BOOL CreateControl(CWnd* pWndCtrl, const wchar_t* lpszProgID,
                       DWORD dwStyle, const RECT& rect, UINT nID);
    void DestroyControl();

    // Activation
    void Activate(BOOL bActivate);
    void Deactivate();
    BOOL IsInPlaceActive() const;

    // Properties / Methods
    void SetProperty(DISPID dwDispID, VARTYPE vtProp, ...);
    void GetProperty(DISPID dwDispID, VARTYPE vtProp, void* pvProp);
    HRESULT InvokeHelper(DISPID dwDispID, WORD wFlags, VARTYPE vtRet,
                         void* pvRet, const BYTE* pbParamInfo, ...);
    HRESULT InvokeHelperV(DISPID dwDispID, WORD wFlags, VARTYPE vtRet,
                          void* pvRet, const BYTE* pbParamInfo, va_list argList);

    // Ambient properties
    BOOL GetAmbientProperty(DISPID dwDispid, VARTYPE vtProp, void* pvProp);

    // Extended control features
    void ShowPropertyFrame();
    void EnableWindow(BOOL bEnable);

    // CWnd wrapper
    CWnd* GetWindow() const;
    COleControlContainer* GetContainer() const;
    COleControl* GetControl() const;

    // IOleControlSite / IDispatch support
    LPOLEOBJECT GetOleObject();
    LPOLEINPLACEOBJECT GetInPlaceObject();
    LPDISPATCH GetDispatch();

public:
    COleControlContainer* m_pCtrlCont;
    COleControl* m_pControl;
    LPOLEOBJECT m_lpObject;
    LPOLEINPLACEOBJECT m_lpInPlaceObject;
    LPDISPATCH m_lpDispatch;
    HWND m_hWnd;
    BOOL m_bInPlaceActive;
    DWORD m_dwStyle;

protected:
    char _olecontrolsite_padding[80];
};

//=============================================================================
// COleControlContainer - OLE Control Container
//=============================================================================
class COleControlContainer : public CCmdTarget {
    DECLARE_DYNAMIC(COleControlContainer)
public:
    COleControlContainer(CWnd* pWnd);
    virtual ~COleControlContainer();

    COleControlSite* CreateSite(COleControlContainer* pCtrlCont);
    BOOL DeleteSite(COleControlSite* pSite);
    COleControlSite* FindItem(UINT nID) const;

    CWnd* GetWnd() const { return m_pWnd; }

public:
    CWnd* m_pWnd;
    CPtrList m_listSites;

protected:
    char _olecontrolcontainer_padding[48];
};

//=============================================================================
// COleIPFrameWnd - OLE In-place Frame Window
//=============================================================================
class COleIPFrameWnd : public CFrameWnd {
    DECLARE_DYNAMIC(COleIPFrameWnd)
public:
    COleIPFrameWnd();
    virtual ~COleIPFrameWnd();

    virtual BOOL OnCreateAggregates();
    virtual BOOL OnCreateControlBars(CFrameWnd* pWndFrame, CFrameWnd* pWndDoc);
    virtual void OnRequestPositionChange(LPCRECT lpRect);

    COleResizeBar* m_pResizeBar;

protected:
    char _oleipframewnd_padding[32];
};

//=============================================================================
// COleConnPtContainer - OLE Connection Point Container
//=============================================================================
class COleConnPtContainer {
public:
    COleConnPtContainer();
    ~COleConnPtContainer();
    char _oleconnptcontainer_padding[32];
};

//=============================================================================
// CEnumOleVerb - OLE Verb Enumerator (minimal)
//=============================================================================
class CEnumOleVerb {
public:
    CEnumOleVerb();
    ~CEnumOleVerb();
    char _cenumoleverb_padding[16];
};

//=============================================================================
// COleControlSiteOrWnd - OLE Control Site or Window wrapper
//=============================================================================
class COleControlSiteOrWnd {
public:
    COleControlSite* m_pSite;
    CWnd* m_pWnd;
    char _olecontrolsiteorwnd_padding[16];
};

//=============================================================================
// OLE Dialog Base Class
//=============================================================================
class COleDialog : public CDialog {
    DECLARE_DYNAMIC(COleDialog)
public:
    COleDialog(UINT nIDTemplate, CWnd* pParentWnd = nullptr);
    virtual ~COleDialog();
    char _olediag_padding[32];
};

//=============================================================================
// COleBusyDialog - OLE Server Busy Dialog
//=============================================================================
class COleBusyDialog : public COleDialog {
    DECLARE_DYNAMIC(COleBusyDialog)
public:
    COleBusyDialog(HTASK hTaskBusy, BOOL bNotResponding = FALSE,
                   DWORD dwFlags = 0, CWnd* pParentWnd = nullptr);
    virtual ~COleBusyDialog();

    intptr_t DoModal();
    OLEUIBUSYW* GetOleUIBusy() { return &m_bz; }

public:
    OLEUIBUSYW m_bz;
    DWORD m_dwFlags;

protected:
    char _olebusydialog_padding[16];
};

//=============================================================================
// COleChangeIconDialog - OLE Change Icon Dialog
//=============================================================================
class COleChangeIconDialog : public COleDialog {
    DECLARE_DYNAMIC(COleChangeIconDialog)
public:
    COleChangeIconDialog(COleClientItem* pItem, DWORD dwFlags = CIF_SELECTCURRENT,
                         CWnd* pParentWnd = nullptr);
    virtual ~COleChangeIconDialog();

    intptr_t DoModal();
    OLEUICHANGEICONW* GetOleUIChangeIcon() { return &m_ci; }

public:
    OLEUICHANGEICONW m_ci;
    DWORD m_dwFlags;
    COleClientItem* m_pItem;

protected:
    char _olechangeicondialog_padding[16];
};

//=============================================================================
// COleChangeSourceDialog - OLE Change Source Dialog
//=============================================================================
class COleChangeSourceDialog : public COleDialog {
    DECLARE_DYNAMIC(COleChangeSourceDialog)
public:
    COleChangeSourceDialog(COleClientItem* pItem, DWORD dwFlags = 0,
                           CWnd* pParentWnd = nullptr);
    virtual ~COleChangeSourceDialog();

    intptr_t DoModal();
    OLEUICHANGESOURCEW* GetOleUIChangeSource() { return &m_cs; }
    CString GetDisplayName();
    CString GetFrom();
    CString GetTo();

public:
    OLEUICHANGESOURCEW m_cs;
    DWORD m_dwFlags;
    COleClientItem* m_pItem;

protected:
    char _olechangesourcedialog_padding[16];
};

//=============================================================================
// COleConvertDialog - OLE Convert Dialog
//=============================================================================
class COleConvertDialog : public COleDialog {
    DECLARE_DYNAMIC(COleConvertDialog)
public:
    COleConvertDialog(COleClientItem* pItem, DWORD dwFlags = CF_SELECTCONVERTTO,
                      CLSID* pClassID = nullptr, CWnd* pParentWnd = nullptr);
    virtual ~COleConvertDialog();

    intptr_t DoModal();
    OLEUICONVERTW* GetOleUIConvert() { return &m_cv; }
    CLSID GetClassID() const;
    BOOL IsConvertTo() const;
    BOOL IsActivateAs() const;
    CString GetDisplayIcon() const;

public:
    OLEUICONVERTW m_cv;
    DWORD m_dwFlags;
    COleClientItem* m_pItem;

protected:
    char _oleconvertdialog_padding[16];
};

//=============================================================================
// COleInsertDialog - OLE Insert Object Dialog
//=============================================================================
class COleInsertDialog : public COleDialog {
    DECLARE_DYNAMIC(COleInsertDialog)
public:
    COleInsertDialog(DWORD dwFlags = IOF_SELECTCREATENEW,
                     CWnd* pParentWnd = nullptr);
    virtual ~COleInsertDialog();

    intptr_t DoModal();
    OLEUIINSERTOBJECTW* GetOleUIInsertObject() { return &m_io; }

    // Helpers
    CLSID GetClassID() const;
    CString GetPathName() const;
    BOOL IsCreateNew() const;
    BOOL IsDisplayAsIcon() const;
    COleClientItem* CreateItem(COleDocument* pDoc = nullptr);
    void GetIconMetafile(HGLOBAL* phMetaPict);

public:
    OLEUIINSERTOBJECTW m_io;
    DWORD m_dwFlags;

protected:
    char _oleinsertdialog_padding[16];
};

//=============================================================================
// COleLinksDialog - OLE Edit Links Dialog
//=============================================================================
class COleLinksDialog : public COleDialog {
    DECLARE_DYNAMIC(COleLinksDialog)
public:
    COleLinksDialog(COleDocument* pDoc, CView* pView = nullptr,
                    DWORD dwFlags = 0, CWnd* pParentWnd = nullptr);
    virtual ~COleLinksDialog();

    intptr_t DoModal();
    OLEUIEDITLINKSW* GetOleUIEditLinks() { return &m_el; }

public:
    OLEUIEDITLINKSW m_el;
    DWORD m_dwFlags;
    COleDocument* m_pDoc;

protected:
    char _olelinksdialog_padding[16];
};

//=============================================================================
// COlePasteSpecialDialog - OLE Paste Special Dialog
//=============================================================================
class COlePasteSpecialDialog : public COleDialog {
    DECLARE_DYNAMIC(COlePasteSpecialDialog)
public:
    COlePasteSpecialDialog(DWORD dwFlags = PSF_SELECTPASTE,
                           COleDataObject* pDataObject = nullptr,
                           CWnd* pParentWnd = nullptr);
    virtual ~COlePasteSpecialDialog();

    intptr_t DoModal();
    OLEUIPASTESPECIALW* GetOleUIPasteSpecial() { return &m_ps; }

    // Helpers
    CLSID GetClassID() const;
    BOOL IsPasteLink() const;
    COleClientItem* CreateItem(COleDocument* pDoc);

public:
    OLEUIPASTESPECIALW m_ps;
    DWORD m_dwFlags;
    COleDataObject* m_pDataObject;

protected:
    char _olepastepecialdialog_padding[16];
};

//=============================================================================
// COlePropertiesDialog - OLE Properties Dialog
//=============================================================================
class COlePropertiesDialog : public COleDialog {
    DECLARE_DYNAMIC(COlePropertiesDialog)
public:
    COlePropertiesDialog(COleClientItem* pItem, UINT nScaleMin = 10, UINT nScaleMax = 500,
                         CWnd* pParentWnd = nullptr);
    virtual ~COlePropertiesDialog();

    intptr_t DoModal();
    OLEUIOBJECTPROPSW* GetOleUIObjectProps() { return &m_op; }

public:
    OLEUIOBJECTPROPSW m_op;
    COleClientItem* m_pItem;

protected:
    char _olepropertiesdialog_padding[16];
};

//=============================================================================
// COleUpdateDialog - OLE Update Links Dialog
//=============================================================================
class COleUpdateDialog : public COleDialog {
    DECLARE_DYNAMIC(COleUpdateDialog)
public:
    COleUpdateDialog(COleDocument* pDoc, BOOL bUpdateLinks = TRUE,
                     BOOL bUpdateEmbeddings = FALSE, CWnd* pParentWnd = nullptr);
    virtual ~COleUpdateDialog();

    intptr_t DoModal();

protected:
    COleDocument* m_pDoc;
    BOOL m_bUpdateLinks;
    BOOL m_bUpdateEmbeddings;

    char _oleupdatedialog_padding[8];
};

//=============================================================================
// COleControl - ActiveX Control Base Class
//=============================================================================
class COleControl : public CWnd {
    DECLARE_DYNAMIC(COleControl)
public:
    COleControl();
    virtual ~COleControl();

    // Control creation
    BOOL CreateControl(REFCLSID clsid, const wchar_t* lpszWindowName,
                       DWORD dwStyle, const RECT& rect, CWnd* pParentWnd,
                       UINT nID, CFile* pPersist = nullptr, BOOL bStorage = FALSE,
                       BSTR bstrLicKey = nullptr);
    BOOL CreateControl(const wchar_t* lpszProgID, const wchar_t* lpszWindowName,
                       DWORD dwStyle, const RECT& rect, CWnd* pParentWnd,
                       UINT nID, CFile* pPersist = nullptr, BOOL bStorage = FALSE,
                       BSTR bstrLicKey = nullptr);

    // Licensing
    BOOL GetLicenseKey(DWORD dwReserved, BSTR* pbstrKey);
    static BSTR PASCAL GetLicenseKey(REFCLSID clsid);

    // Persistence
    void Serialize(CArchive& ar) override;
    BOOL DoPropExchange(CPropExchange* pPX);

    // Ambient properties
    BOOL GetAmbientProperty(DISPID dwDispid, VARTYPE vtProp, void* pvProp);

    // Events
    void FireEvent(DISPID dispId, BYTE* pbParams, ...);
    void FireEventV(DISPID dispId, BYTE* pbParams, va_list argList);

    // Control state
    BOOL IsOptimizedDraw() const;
    void SetInitialSize(int cx, int cy);

    // Overrides
    virtual void OnDraw(CDC* pDC, const CRect& rcBounds, const CRect& rcInvalid);

public:
    COleControlSite* m_pControlSite;
    COleControlContainer* m_pContainer;
    BOOL m_bInitialized;
    BOOL m_bInPlaceActive;
    CSize m_cxExtent;
    CSize m_cyExtent;
    BOOL m_bOptimizedDraw;

protected:
    char _olecontrol_padding[64];
};

//=============================================================================
// Forward references for OLE helper functions
//=============================================================================

// OLE initialization (implemented as extern "C" thunks)
extern "C" {
#ifdef __GNUC__
    void __attribute__((ms_abi)) impl__AfxOleInit();
    void __attribute__((ms_abi)) impl__AfxOleTerm(int bExplicit);
    void __attribute__((ms_abi)) impl__AfxOleLockApp();
#else
    void impl__AfxOleInit();
    void impl__AfxOleTerm(int bExplicit);
    void impl__AfxOleLockApp();
#endif
}
inline void AFXAPI AfxOleInit() { impl__AfxOleInit(); }
inline void AFXAPI AfxOleTerm(int bExplicit = 0) { impl__AfxOleTerm(bExplicit); }
inline void AFXAPI AfxOleLockApp() { impl__AfxOleLockApp(); }
void AFXAPI AfxOleUnlockApp();
BOOL AFXAPI AfxOleCanExitApp();
BOOL AFXAPI AfxOleGetMessageFilter();

// OLE registration
BOOL AFXAPI AfxOleRegisterTypeLib(REFGUID guid, const wchar_t* lpszFileName);
BOOL AFXAPI AfxOleRegisterServerClass(REFCLSID clsid, const wchar_t* lpszClassName,
                                       const wchar_t* lpszShortTypeName, const wchar_t* lpszLongTypeName,
                                       OLE_APPTYPE nAppType, const wchar_t** rglpszRegister = nullptr,
                                       const wchar_t** rglpszOverwrite = nullptr);
BOOL AFXAPI AfxOleRegisterControlClass(REFCLSID clsid, const wchar_t* lpszClassName,
                                        const wchar_t* lpszShortTypeName, const wchar_t* lpszLongTypeName,
                                        DWORD dwMiscStatus, DWORD dwFlags, DWORD dwVersion);
BOOL AFXAPI AfxOleUnregisterClass(REFCLSID clsid, const wchar_t* lpszClassName);

// OLE utilities
BOOL AFXAPI AfxOleSetEditMenu(COleClientItem* pItem, CMenu* pMenu, UINT iMenuItem,
                               UINT nIDVerbMin, UINT nIDVerbMax = 0, UINT nIDConvert = 0);
BOOL AFXAPI AfxOleSetUserCtrl(BOOL bUserCtrl);

// In-place activation helpers
BOOL AFXAPI AfxOleInProcessRegister(COleObjectFactory* pFactory);
BOOL AFXAPI AfxOleCanActivate();
void AFXAPI AfxOleOnFrameClosed(CFrameWnd* pFrameWnd);

// Control helpers
BOOL AFXAPI AfxOleRegisterPropertyPageClass(REFCLSID clsid);
COLORREF AFXAPI AfxOleTranslateColor(OLE_COLOR clrColor, HPALETTE hpal = nullptr);

// Type library
BOOL AFXAPI AfxOleRegisterTypeLib(REFGUID guid, const wchar_t* lpszFileName);
BOOL AFXAPI AfxOleUnregisterTypeLib(REFGUID guid, WORD wVerMajor, WORD wVerMinor, LCID lcid = LOCALE_NEUTRAL);

#endif // OPENMFC_AFXOLE_H
