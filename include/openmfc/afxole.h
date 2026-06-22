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

    // Faithful MSVC mfc140u CControlBar layout (size 328; members begin where
    // CWnd ends @232). Real MFC keeps these here, so the bar subclasses inherit
    // them instead of re-declaring (which is what made CToolBar/CStatusBar
    // oversized). m_pData is void* in retail; declared UINT* here (same 8-byte
    // pointer at the same offset) so CStatusBar's pane code indexes it directly.
    void* m_pInPlaceOwner;     // 232
    BOOL  m_bAutoDelete;       // 240
    int   m_cxLeftBorder;      // 244
    int   m_cxRightBorder;     // 248
    int   m_cyTopBorder;       // 252
    int   m_cyBottomBorder;    // 256
    int   m_cxDefaultGap;      // 260
    int   m_nMRUWidth;         // 264
    int   m_nCount;            // 268
    UINT* m_pData;             // 272
    HANDLE m_hReBarTheme;      // 280
    int   m_nStateFlags;       // 288
    DWORD m_dwStyle;           // 292
    DWORD m_dwDockStyle;       // 296
    CWnd* m_pDockSite;         // 304
    void* m_pDockBar;          // 312
    void* m_pDockContext;      // 320
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
    char _tooltip_padding[56];
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
    // CToolBar-specific members (size 384; begin where CControlBar ends @328).
    // m_nCount and the docking members are inherited from CControlBar now.
    HANDLE  m_hRsrcImageWell;        // 328
    HANDLE  m_hInstImageWell;        // 336
    HBITMAP m_hbmImageWell;          // 344
    BOOL    m_bDelayedButtonLayout;  // 352
    SIZE    m_sizeImage;             // 356
    SIZE    m_sizeButton;            // 364
    void*   m_pStringMap;            // 376
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
    // CStatusBar adds only m_nMinHeight (size 336; begins where CControlBar
    // ends @328). m_nCount/m_pData/m_cxLeftBorder/m_cxRightBorder are inherited
    // from CControlBar now.
    int m_nMinHeight;            // 328
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
    // Re-tuned from [16] to [8]: the faithful CControlBar no longer has tail
    // padding for mingw's Itanium ABI to fold m_bAutoDelete into, so the base
    // now packs like MSVC. Total 352 to match mfc140u.
    char _dialogbar_padding[8];
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
    char _splitterwnd_padding[72];
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
    CTaskDialog(const CString& strContent, const CString& strMainInstruction,
                const CString& strWindowTitle, int nCommonButtons = TDCBF_OK_BUTTON,
                int nTaskDialogOptions = 0, const CString& strFooter = CString());
    CTaskDialog(const CString& strContent, const CString& strMainInstruction,
                const CString& strWindowTitle, int nCommonButtons, int nTaskDialogOptions,
                int, int, const CString& strFooter);
    virtual ~CTaskDialog();

    // Configuration
    void SetDialogWidth(int nWidth = 0);
    void SetMainIcon(HICON hMainIcon);
    void SetMainIcon(const wchar_t* pszMainIcon);
    void SetMainIcon(UINT nMainIconID);
    void SetFooterIcon(HICON hFooterIcon);
    void SetFooterIcon(const wchar_t* pszFooterIcon);
    void SetFooterText(const wchar_t* pszFooterText);
    void SetFooterText(const CString& strFooterText);
    void SetVerificationCheckboxText(const wchar_t* pszText);
    void SetVerificationCheckboxText(const CString& strText);
    void SetExpandedInformation(const wchar_t* pszText);
    void SetExpandedControlText(const wchar_t* pszText);
    void SetCollapsedControlText(const wchar_t* pszText);
    void SetProgressBarRange(int nMin, int nMax);
    void SetProgressBarPosition(int nPos);
    void SetProgressBarMarquee(BOOL bMarquee = TRUE, int nSpeed = 0);

    // Adding controls
    HRESULT AddCommandControl(int nCommandID, const wchar_t* pszLabel);
    void AddCommandControl(int nCommandID, const CString& strLabel, int nReserved0 = 0, int nReserved1 = 0);
    HRESULT AddRadioButton(int nRadioButtonID, const wchar_t* pszLabel);
    void AddRadioButton(int nRadioButtonID, const CString& strLabel, int nReserved = 0);
    HRESULT AddPushButton(int nButtonID, const wchar_t* pszLabel);

    // Display
    int DoModal(HWND hWndParent = nullptr);
    BOOL GetVerificationCheckboxState() const { return m_bVerificationChecked; }
    int GetSelectedCommandControlID() const { return m_nSelectedCommandID; }
    int GetSelectedRadioButtonID() const { return m_nSelectedRadioButtonID; }
    BOOL IsExpanded() const { return m_bExpanded; }
    void SetOptions(int nTaskDialogOptions);
    int GetOptions() const;
    static BOOL IsSupported();

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
    char _docitem_padding[56];
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
class COleDocObjectItem;

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
// Byte-faithful MSVC mfc140u layout: COleSafeArray : public tagVARIANT (24) plus
// two cached DWORDs (size 32). The SAFEARRAY lives in the variant's `parray`
// field with vt = VT_ARRAY | <element type>; m_dwElementSize / m_dwDims cache
// the element size and dimension count. The impl (ole_csafearray_ext.cpp,
// olecore.cpp) drives the real oleaut32 SafeArray* APIs through parray.
class COleSafeArray : public tagVARIANT {
public:
    DWORD m_dwElementSize;   // @24
    DWORD m_dwDims;          // @28

    COleSafeArray() { vt = VT_EMPTY; parray = nullptr; m_dwElementSize = 0; m_dwDims = 0; }
    ~COleSafeArray() { Destroy(); }

    void Destroy();
    void Clear();
    void Create(VARTYPE vt, DWORD cDims, SAFEARRAYBOUND* rgsabound);
    void AccessData(void** ppvData);
    void UnaccessData();
    void Attach(const SAFEARRAY& saSrc);
    SAFEARRAY* Detach();
    void Copy(const COleSafeArray* psaSrc);
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
    BOOL AttachClipboard();
    void EnsureClipboardObject();
    CFile* GetFileData(CLIPFORMAT cfFormat, FORMATETC* lpFormatEtc = nullptr);

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
    // Data provision
    virtual int OnRenderGlobalData(FORMATETC* lpFormatEtc, void** phGlobal);
    virtual int OnRenderFileData(FORMATETC* lpFormatEtc, CFile* pFile);
    void Empty();

    // IDataObject interface
    LPDATAOBJECT GetInterface(BOOL bAddRef = FALSE);

public:
    BOOL m_bClipboardOwner;
    void* m_pDataCache; // internal cache

protected:
    char _oledatasource_padding[48];
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
    char _oledroptarget_padding[48];
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
    char _oledropsource_padding[64];
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
    int Register();
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
    char _olemessagefilter_padding[48];
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
    virtual int DoUpdate(CCmdTarget* pTarget, BOOL bDisableIfNoHandler);

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
    char _oleresizebar_padding[64];
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
    char _olestreamfile_padding[24];
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
    void SetModifiedFlag(BOOL bModified = TRUE);

    IPropertyPageSite* GetPageSite() const { return m_pPageSite; }

public:
    IPropertyPageSite* m_pPageSite;
    BOOL m_bModified;
    wchar_t* m_pszPageName;
    wchar_t* m_pszHelpFile;
    DWORD m_dwHelpContext;
    wchar_t* m_pszDocString;

protected:
    char _olepropertypage_padding[160];
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
    char _coledocument_padding[96];
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
    char _colelinkingdoc_padding[56];
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
    char _coleserverdoc_padding[96];
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
    int DoVerb(LONG nVerb, CView* pView = nullptr, MSG* lpMsg = nullptr);
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
    virtual void OnShowItem();
    virtual void OnOpen();
    virtual void OnClose();
    virtual BOOL OnShowControlBars(CFrameWnd* pFrameWnd, BOOL bShow);

    // Additional drawing / rendering
    HGLOBAL GetIconicMetafile();
    BOOL SetIconicMetafile(HGLOBAL hMetaPict);
    HGLOBAL GetMetaFile();
    void SetHostNames(const wchar_t* lpszHost, const wchar_t* lpszHostObj);

    // Conversion
    BOOL ConvertTo(REFCLSID clsidNew);
    BOOL ActivateAs(REFCLSID clsidNew, REFCLSID clsidOld);
    BOOL Reload();
    void UpdateLink();
    BOOL IsLinkUpToDate() const;
    BOOL CanActivate();
    BOOL IsOpen() const;
    BOOL IsRunning() const;

    // Verb operations
    HRESULT EnumVerbs(IEnumOLEVERB** ppEnumOleVerb);
    LONG GetActiveVerb() const;
    void SetActiveVerb(LONG nVerb);

    // Misc
    void AssertValid() const;
    void Serialize(CArchive& ar) override;
    BOOL IsModified() const;
    void SetModifiedFlag(BOOL bModified = TRUE);

    // OLE Control support
    void SetControlSite(COleControlSite* pSite);
    COleControlSite* GetControlSite() const;
    void AttachDataObject(COleDataObject& dataObject) const;

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
// COleDocObjectItem - Active Document Container Item
//=============================================================================
class COleDocObjectItem : public COleClientItem {
    DECLARE_DYNAMIC(COleDocObjectItem)
public:
    COleDocObjectItem(COleDocument* pContainerDoc = nullptr);
    virtual ~COleDocObjectItem();

    BOOL IsDocObject() const;
    BOOL IsActive() const;
    IOleDocumentView* GetActiveView() const;
    HRESULT GetDocument(IUnknown** ppDocument);
    void ActivateAndShow();
    BOOL IsOpen() const;
    void OnActivateView();
    virtual BOOL OnPreparePrinting(void* pInfo);
    virtual void OnBeginPrinting(CDC* pDC, void* pInfo);
    virtual void OnPrint(CDC* pDC, void* pInfo);
    virtual void OnEndPrinting(CDC* pDC, void* pInfo);
    HRESULT ExecCommand(DWORD nCmdID, DWORD nCmdExecOpt = OLECMDEXECOPT_DONTPROMPTUSER,
                        const GUID* pguidCmdGroup = nullptr);

protected:
    char _coledocobjectitem_padding[40];
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
    char _oleserveritem_padding[136];
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
    COleObjectFactory(REFCLSID clsid, CRuntimeClass* pRuntimeClass,
                      BOOL bMultiInstance, BOOL bFreeOnRelease, const wchar_t* lpszProgID);
    virtual ~COleObjectFactory();

    BOOL Register();
    void Revoke();
    BOOL IsRegistered() const;
    BOOL IsLicenseValid();
    static int RegisterAll();
    static BOOL PASCAL UpdateRegistryAll(BOOL bRegister = TRUE);
    static void PASCAL RevokeAll();
    BOOL Unregister();
    static BOOL PASCAL UnregisterAll();

    virtual CCmdTarget* OnCreateObject();
    virtual BOOL OnVerifyFile(LPCTSTR lpszFileName);
    virtual void UpdateRegistry(BOOL bRegister = TRUE);
    virtual BOOL VerifyUserLicense();
    virtual BOOL VerifyLicenseKey(BSTR bstrKey);
    virtual BOOL GetLicenseKey(DWORD dwReserved, BSTR* pbstrKey);

    CLSID GetClassID() const { return m_clsid; }
    const wchar_t* GetProgID() const { return m_strProgID; }

    // Connection points
    void ConnectTemplate(COleTemplateServer* pTemplate);
    COleTemplateServer* GetTemplate() const { return m_pTemplate; }

    void CommonConstruct(REFCLSID clsid, CRuntimeClass* pRuntimeClass,
                         BOOL bMultiInstance, BOOL bFreeOnRelease, const wchar_t* lpszProgID);

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
    BOOL Register();
    BOOL Unregister();
    virtual void UpdateRegistry(OLE_APPTYPE nAppType = OAT_INPLACE_SERVER,
                               const wchar_t** rglpszRegister = nullptr,
                               const wchar_t** rglpszOverwrite = nullptr,
                               BOOL bRegister = TRUE);
    virtual CCmdTarget* OnCreateObject();
    virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);

public:
    CDocTemplate* m_pDocTemplate;

protected:
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
    long GetWindow(HWND__** phWnd);
    COleControlContainer* GetContainer() const;
    long GetContainer(IOleContainer** ppContainer);
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
    char _olecontrolsite_padding[296];
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
    char _olecontrolcontainer_padding[160];
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
    char _oleipframewnd_padding[160];
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
// CEnumFormatEtc - FORMATETC Enumerator
//=============================================================================
class CEnumFormatEtc : public IEnumFORMATETC {
public:
    CEnumFormatEtc();
    virtual ~CEnumFormatEtc();

    // IUnknown
    STDMETHOD(QueryInterface)(REFIID riid, void** ppv) override;
    STDMETHOD_(ULONG, AddRef)() override;
    STDMETHOD_(ULONG, Release)() override;

    // IEnumFORMATETC
    STDMETHOD(Next)(ULONG celt, FORMATETC* rgelt, ULONG* pceltFetched) override;
    STDMETHOD(Skip)(ULONG celt) override;
    STDMETHOD(Reset)() override;
    STDMETHOD(Clone)(IEnumFORMATETC** ppEnum) override;

    void AddFormat(const FORMATETC& formatEtc);

private:
    ULONG m_refCount;
    FORMATETC* m_formats;
    ULONG m_count;
    ULONG m_capacity;
    ULONG m_position;
    char _cenumformatetc_padding[16];
};

//=============================================================================
// COleControlSiteOrWnd - OLE Control Site or Window wrapper
//=============================================================================
class COleControlSiteOrWnd {
public:
    HWND GetSafeHwnd() const;
    DWORD GetStyle() const;
    DWORD GetExStyle() const;
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
    explicit COleDialog(CWnd* pParentWnd);
    virtual ~COleDialog();
    int MapResult(UINT nResult);
    char _olediag_padding[8];
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
    int DoChangeIcon(COleClientItem* pItem);
    OLEUICHANGEICONW* GetOleUIChangeIcon() { return &m_ci; }

public:
    // Faithful MSVC layout: COleDialog (312) + OLEUICHANGEICONW m_ci@312 only,
    // total 928. dwFlags/the item live inside m_ci (m_ci.dwFlags etc.).
    OLEUICHANGEICONW m_ci;
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
    virtual void PreInitDialog();

public:
    OLEUICHANGESOURCEW m_cs;
    DWORD m_dwFlags;
    COleClientItem* m_pItem;

protected:
    char _olechangesourcedialog_padding[32];
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
    int DoConvert(COleClientItem* pItem);
    UINT GetSelectionType() const;

public:
    // Faithful MSVC layout: COleDialog (312) + OLEUICONVERTW m_cv@312 only,
    // total 504. dwFlags/the item live inside m_cv (m_cv.dwFlags etc.).
    OLEUICONVERTW m_cv;
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

    virtual __int64 DoModal();
    __int64 DoModal(unsigned long);
    OLEUIINSERTOBJECTW* GetOleUIInsertObject() { return &m_io; }

    // Helpers
    CLSID GetClassID() const;
    CString GetPathName() const;
    BOOL IsCreateNew() const;
    BOOL IsDisplayAsIcon() const;
    int CreateItem(COleClientItem* pItem);
    COleClientItem* CreateItem(COleDocument* pDoc = nullptr);
    void GetIconMetafile(HGLOBAL* phMetaPict);
    UINT GetSelectionType() const;
    void AddClassIDToList(CLSID*& rgclsid, int& nCount, int& nAlloc, CLSID* pClassID);

public:
    OLEUIINSERTOBJECTW m_io;
    DWORD m_dwFlags;

protected:
    char _oleinsertdialog_padding[512];
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
    char _olelinksdialog_padding[32];
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
    int CreateItem(COleClientItem* pItem);
    COleClientItem* CreateItem(COleDocument* pDoc);
    UINT GetSelectionType() const;
    void AddFormat(const FORMATETC& formatEtc, wchar_t* lpszFormat,
                   wchar_t* lpszResult, DWORD flags);
    void AddFormat(UINT cfFormat, TYMED tymed, UINT nFormatID,
                   BOOL bEnableIcon, BOOL bLink);
    OLEUIPASTEFLAG AddLinkEntry(UINT nFormatID);
    void AddStandardFormats(BOOL bEnableLink = TRUE);

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
    char _olepropertiesdialog_padding[328];
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

    char _oleupdatedialog_padding[112];
};

//=============================================================================
// COleControl - ActiveX Control Base Class
//=============================================================================
class COleControl : public CWnd {
    DECLARE_DYNAMIC(COleControl)
public:
    // Nested control data source
    class CControlDataSource {
    public:
        CControlDataSource(COleControl*);
        char _pad[16];
    };

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
    virtual BOOL VerifyUserLicense();
    virtual BOOL VerifyLicenseKey(BSTR bstrKey);
    BOOL SetLicenseKey(const wchar_t* lpszLicenseKey);

    // Persistence
    void Serialize(CArchive& ar) override;
    BOOL DoPropExchange(CPropExchange* pPX);
    virtual void DoDataExchange(void* pDX);
    virtual void OnResetState();
    virtual DWORD GetControlFlags();
    virtual BOOL OnSetExtent(DVASPECT dwDrawAspect, const SIZE& size);
    virtual BOOL OnGetExtent(DVASPECT dwDrawAspect, SIZE& size);
    virtual BOOL OnMapPropertyToPage(DISPID dispid, CLSID* pclsid, BOOL* pbPageOptional);

    // Ambient properties
    BOOL GetAmbientProperty(DISPID dwDispid, VARTYPE vtProp, void* pvProp);
    COLORREF AmbientBackColor();
    COLORREF AmbientForeColor();
    COLORREF AmbientAppearance();
    OLE_COLOR AmbientBackColorOle();
    OLE_COLOR AmbientForeColorOle();
    IFontDisp* AmbientFont();
    IDispatch* AmbientFontDisp();
    short AmbientTextAlign();
    BOOL AmbientUserMode();
    BOOL AmbientUIDead();
    BOOL AmbientShowGrabHandles();
    BOOL AmbientShowHatching();
    CString AmbientDisplayName();
    BOOL AmbientDisplayAsDefault();
    BOOL AmbientAutoClip();
    BOOL AmbientSupportsMnemonics();
    CString AmbientScaleUnits();
    unsigned long AmbientLocaleID();

    // Events
    void FireEvent(DISPID dispId, BYTE* pbParams, ...);
    void FireEventV(DISPID dispId, BYTE* pbParams, va_list argList);
    void FireClick();
    void FireDblClick();
    void FireKeyDown(USHORT* pnChar, short nShiftState);
    void FireKeyPress(USHORT* pnChar);
    void FireKeyUp(USHORT* pnChar, short nShiftState);
    void FireMouseDown(short nButton, short nShiftState, long x, long y);
    void FireMouseMove(short nButton, short nShiftState, long x, long y);
    void FireMouseUp(short nButton, short nShiftState, long x, long y);
    void FireReadyStateChange();

    // Stock properties
    COLORREF GetBackColor() const;
    void SetBackColor(COLORREF clr);
    COLORREF GetForeColor() const;
    void SetForeColor(COLORREF clr);
    BOOL GetEnabled() const;
    void SetEnabled(BOOL bEnabled);
    void* InternalGetFont() { return nullptr; }
    void SetFont(LPFONTDISP pFontDisp);
    void SetFont(CFont* pFont);
    unsigned int GetHwnd();
    void SetHwnd(HWND hWnd);
    OLE_COLOR GetBackColorOle() const;
    OLE_COLOR GetForeColorOle() const;
    void SetBackColorOle(OLE_COLOR clr);
    void SetForeColorOle(OLE_COLOR clr);
    short GetAppearance() const;
    void SetAppearance(short nAppearance);
    short GetBorderStyle() const;
    void SetBorderStyle(short nBorderStyle);
    CString GetText() const;
    void SetText(const wchar_t* lpszText);
    void GetText(CString& strText) const;
    long GetReadyState() const;

    // Control state
    BOOL IsOptimizedDraw() const;
    void SetInitialSize(int cx, int cy);
    BOOL IsSubclassedControl();
    void SetModifiedFlag(BOOL bModified = TRUE);
    BOOL GetModifiedFlag() const;
    ULONG InternalAddRef();
    ULONG InternalRelease();
    ULONG InternalQueryInterface(REFIID riid, void** ppv);
    void GetControlSize(int* pCX, int* pCY);
    void SetControlSize(int cx, int cy);
    virtual void OnSetClientSite();
    virtual void OnGetControlInfo(LPCONTROLINFO pControlInfo);
    virtual BOOL OnMnemonic(LPMSG pMsg);
    virtual void OnAmbientPropertyChange(DISPID dispid);
    void BoundPropertyChanged(DISPID dispid);
    void BoundPropertyRequestEdit(DISPID dispid);
    void InvalidateControl(LPCRECT lpRect = nullptr, BOOL bErase = TRUE);

    // Property pages
    virtual int OnProperties(MSG* pMsg, HWND hWnd, const RECT* lpRect);
    void ShowPropertyPages();
    int GetPropertyPageCount() const;
    BOOL IsPropertyPage(LPUNKNOWN lpUnk);

    // Connection points
    BOOL CanCreateConnectionPoints();
    void EnableConnectionPoints();
    BOOL IsConnectionPointEnabled(REFIID riid);
    void FirePropChanged(DISPID dispid);

    // Message handling
    virtual BOOL PreTranslateMessage(MSG* pMsg);
    virtual void OnDraw(CDC* pDC, const CRect& rcBounds, const CRect& rcInvalid);

    // Window
    virtual LONG OnPosRectChange(LPCRECT lprcPosRect);
    virtual BOOL OnSetObjectRects(LPCRECT lprcPosRect, LPCRECT lprcClipRect);
    virtual void OnClose(DWORD dwSaveOption);
    void SetCapture();
    void ReleaseCapture();
    void BringWindowToTop();
    void MoveWindow(int X, int Y, int nWidth, int nHeight, BOOL bRepaint = TRUE);
    void MoveWindow(LPCRECT lpRect, BOOL bRepaint = TRUE);

public:
    COleControlSite* m_pControlSite;
    COleControlContainer* m_pContainer;
    BOOL m_bInitialized;
    BOOL m_bInPlaceActive;
    CSize m_cxExtent;
    CSize m_cyExtent;
    BOOL m_bOptimizedDraw;

protected:
    char _olecontrol_padding[632];
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

// HINSTANCE-based registration helpers (used by MFC control/server registration)
BOOL AFXAPI AfxOleRegisterServerClass(REFCLSID clsid, const wchar_t* lpszClassName,
                                       const wchar_t* lpszShortTypeName,
                                       const wchar_t* lpszLongTypeName,
                                       OLE_APPTYPE nAppType, const wchar_t** rglpszRegister,
                                       const wchar_t** rglpszOverwrite,
                                       int nFlags, const wchar_t* lpszLocalServerPath);
BOOL AFXAPI AfxOleRegisterControlClass(HINSTANCE hInstance, REFCLSID clsid,
                                        const wchar_t* lpszProgID, UINT idTypeName,
                                        UINT idBitmap, int nRegFlags,
                                        DWORD dwMiscStatus, REFGUID tlid,
                                        WORD wVerMajor, WORD wVerMinor);
BOOL AFXAPI AfxOleRegisterPropertyPageClass(HINSTANCE hInstance, REFCLSID clsid, UINT idTypeName);
BOOL AFXAPI AfxOleRegisterPropertyPageClass(HINSTANCE hInstance, REFCLSID clsid,
                                             UINT idTypeName, int nHelpContext);
BOOL AFXAPI AfxOleRegisterTypeLib(HINSTANCE hInstance, REFGUID tlid,
                                   const wchar_t* lpszFileName = nullptr,
                                   const wchar_t* lpszHelpDir = nullptr);
BOOL AFXAPI AfxOleRegisterHelper(const wchar_t* const* rglpszRegister,
                                  const wchar_t* const* rglpszOverwrite,
                                  int nRegFlags, BOOL bRegister,
                                  HKEY hkey = nullptr);

// OLE utilities
BOOL AFXAPI AfxOleSetEditMenu(COleClientItem* pItem, CMenu* pMenu, UINT iMenuItem,
                               UINT nIDVerbMin, UINT nIDVerbMax = 0, UINT nIDConvert = 0);
void AFXAPI AfxOleSetUserCtrl(BOOL bUserCtrl);

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
