// OpenMFC Feature Pack Classes
// ABI-compatible declarations for MFC Feature Pack exports
// (Visual Studio 2008+ MFC extensions: ribbon, docking, visual styles)
//
// These are minimal stub declarations for ABI compatibility.
// Full functionality requires the Feature Pack runtime.
#pragma once

#include "afxwin.h"
#include "afxole.h"

//=============================================================================
// Forward declarations — ALL types used anywhere in this header
//=============================================================================
class CMFCToolBar;
class CMFCToolBarButton;
class CMFCToolBarImages;
class CMFCVisualManager;
class CMFCMenuBar;
class CMFCRibbonBar;
class CMFCRibbonPanel;
class CMFCRibbonCategory;
class CMFCRibbonBaseElement;
class CMFCRibbonButton;
class CMFCBaseTabCtrl;
class CMFCTabCtrl;
class CMFCButton;
class CMFCPopupMenu;
class CMFCPropertyGridCtrl;
class CMFCPropertyGridProperty;
class CMFCCaptionBar;
class CMFCTasksPane;
class CMFCAutoHideBar;
class CPane;
class CDockablePane;
class CBasePane;
class CPaneFrameWnd;
class CDockingManager;
class CPaneContainer;
class CFrameWndEx;
class CMDIFrameWndEx;
class CMDIChildWndEx;

// Types referenced by CMFCVisualManager method signatures
class CMFCCaptionButton;
class CMFCHeaderCtrl;
class CMFCToolBarMenuButton;
class CMFCToolBarMenuButtonsButton;
class CMFCToolBarComboBoxButton;
class CMFCToolBarEditBoxButton;
class CMFCPopupMenuBar;
class CContextMenuManager;
class CKeyboardManager;
class CTooltipManager;
class CMenuHash;
class CGlobalUtils;
class CWinAppEx;
class CMFCToolTipInfo;
class CMouseManager;
class CShellManager;
class CUserToolsManager;
class CMFCDesktopAlertWnd;
class CMFCDesktopAlertWndButton;
class CMFCStatusBar;
class CPaneDivider;
class CMenuImages;
class CMFCRibbonButtonsGroup;
class CMFCRibbonCaptionButton;
class CMFCRibbonCategoryScroll;
class CMFCRibbonContextCaption;
class CMFCRibbonTab;
class CMFCRibbonCheckBox;
class CMFCRibbonEdit;
class CMFCRibbonGallery;
class CMFCRibbonLabel;
class CMFCRibbonMainPanel;
class CMFCRibbonProgressBar;
class CMFCRibbonSeparator;
class CMFCRibbonSlider;
class CMFCRibbonStatusBar;
class CMFCRibbonStatusBarPane;
class CMFCTasksPaneTask;
class CMFCTasksPaneTaskGroup;
class CImageList;

// CMiniFrameWnd — needed by CMFCPopupMenu, CPaneFrameWnd (must be defined before use)
class CMiniFrameWnd : public CFrameWnd {
    DECLARE_DYNAMIC(CMiniFrameWnd)
public: CMiniFrameWnd(); virtual ~CMiniFrameWnd(); protected: char _pad[32]; };

// AFX_IDW_RIBBON_BAR — needed before CMFCRibbonBar::Create default arg
#ifndef AFX_IDW_RIBBON_BAR
#define AFX_IDW_RIBBON_BAR 0xE804
#endif

// CMenuImages — needed by CMFCVisualManager method signatures (uses IMAGES_IDS)
class CMenuImages {
public:
    enum IMAGES_IDS { IdMin = 0, IdMax = 30 };
    CMenuImages() {}
    char _pad[16];
};

//=============================================================================
// CMFCVisualManager - Base visual manager
//=============================================================================
class CMFCVisualManager : public CObject {
    DECLARE_DYNAMIC(CMFCVisualManager)
public:
    enum AFX_BUTTON_STATE { ButtonsIsRegular = 0, ButtonsIsHighlighted = 1, ButtonsIsPressed = 2, ButtonsIsDisabled = 3 };

    CMFCVisualManager();
    CMFCVisualManager(int) : CMFCVisualManager() {}
    virtual ~CMFCVisualManager();

    static CMFCVisualManager* GetInstance();
    static void SetDefaultManager(CRuntimeClass* pRTI);

    virtual void OnDrawMenuBorder(CDC* pDC, CMFCPopupMenu* pMenu, CRect rect);
    virtual void OnDrawMenuImage(CDC* pDC, const CRect& rect, const CRect& rectImage);
    virtual void OnFillBarBackground(CDC* pDC, CBasePane* pBar, CRect rectClient, CRect rectClip, BOOL bNCArea = FALSE);
    virtual void OnDrawBarGripper(CDC* pDC, CRect rectGripper, BOOL bHorz, CBasePane* pBar);
    virtual void OnDrawButtonBorder(CDC* pDC, CMFCToolBarButton* pButton, CRect rect, CMFCVisualManager::AFX_BUTTON_STATE state);
    virtual void OnDrawButtonSeparator(CDC* pDC, CMFCToolBarButton* pButton, CRect rect, BOOL bHorz);
    virtual void OnDrawCaptionButton(CDC* pDC, CMFCCaptionButton* pButton, BOOL bHorz, BOOL bMaximized, BOOL bDisabled);
    virtual void OnDrawCaptionButtonIcon(CDC* pDC, CMFCCaptionButton* pButton, CMenuImages::IMAGES_IDS id, BOOL bHorz, BOOL bMaximized);
    virtual void OnDrawCheckBoxEx(CDC* pDC, CRect rect, int nState, BOOL bHighlighted, BOOL bPressed, BOOL bEnabled);
    virtual void OnDrawComboBorder(CDC* pDC, CRect rect, BOOL bDisabled, BOOL bIsDropped, BOOL bIsHighlighted, CMFCToolBarComboBoxButton* pButton);
    virtual void OnDrawComboDropButton(CDC* pDC, CRect rect, BOOL bDisabled, BOOL bIsDropped, BOOL bIsHighlighted, CMFCToolBarComboBoxButton* pButton);
    virtual void OnDrawControlBorder(CWnd* pWndCtrl);
    virtual void OnDrawDockingBarScrollButton(CDC* pDC, CMFCToolBarButton* pButton, BOOL bUp, BOOL bHorz);
    virtual void OnDrawEditBorder(CDC* pDC, CRect rect, BOOL bDisabled, BOOL bIsHighlighted, CMFCToolBarEditBoxButton* pButton);
    virtual void OnDrawExpandingBox(CDC* pDC, CRect rect, BOOL bIsOpened, COLORREF colorBox);
    virtual void OnDrawFloatingToolbarBorder(CDC* pDC, CMFCToolBar* pToolBar, CRect rectBorder, CRect rectBorderSize);
    virtual void OnDrawHeaderCtrlBorder(CMFCHeaderCtrl* pCtrl, CDC* pDC, CRect& rect, BOOL bIsPressed, BOOL bIsHighlighted);
    virtual void OnDrawHeaderCtrlSortArrow(CMFCHeaderCtrl* pCtrl, CDC* pDC, CRect& rect, BOOL bIsAscending);
    virtual void OnDrawMenuArrowOnCustomizeList(CDC* pDC, CRect rect, BOOL bSelected);
    virtual void OnDrawMenuCheck(CDC* pDC, CMFCToolBarMenuButton* pButton, CRect rect, BOOL bHighlight, BOOL bIsRadio);
    virtual void OnDrawMenuItemButton(CDC* pDC, CMFCToolBarMenuButton* pButton, CRect rectButton, BOOL bHighlight, BOOL bDisabled);
    virtual void OnDrawMenuResizeBar(CDC* pDC, CRect rect, int nResizeFlags);
    virtual void OnDrawMenuScrollButton(CDC* pDC, CRect rect, BOOL bIsScrollDown, BOOL bIsHighlited, BOOL bIsPressed, BOOL bIsDisabled);
    virtual void OnDrawMenuShadow(CDC* pDC, const CRect& rectClient, const CRect& rectExclude, int nDepth, int iMinBrightness, int iMaxBrightness, CBitmap* pBmpSaveBottom, CBitmap* pBmpSaveRight, COLORREF color = RGB(0,0,0));
    virtual void OnDrawMenuSystemButton(CDC* pDC, CRect rect, UINT uiSystemCommand, UINT nStyle, BOOL bHighlight);
    virtual void OnDrawMiniFrameBorder(CDC* pDC, CPaneFrameWnd* pFrameWnd, CRect rectBorder, CRect rectBorderSize);
    virtual void OnDrawOutlookBarSplitter(CDC* pDC, CRect rectSplitter);
    virtual void OnDrawOutlookPageButtonBorder(CDC* pDC, CRect& rect, BOOL bIsHighlighted, BOOL bIsPressed);
    virtual void OnDrawPaneBorder(CDC* pDC, CBasePane* pBar, CRect& rect);
    virtual unsigned long OnDrawPaneCaption(CDC* pDC, CDockablePane* pBar, int n1, CRect rectCaption, int n2);
    virtual void OnDrawPaneDivider(CDC* pDC, CPaneDivider* pSlider, CRect rect, BOOL bAutoHideMode);
    virtual void OnDrawPopupWindowBorder(CDC* pDC, CRect rect);
    virtual void OnDrawPopupWindowButtonBorder(CDC* pDC, CRect rectClient, CMFCDesktopAlertWndButton* pButton);
    virtual void OnDrawPopupWindowCaption(CDC* pDC, CRect rectCaption, CMFCDesktopAlertWnd* pPopupWnd);
    virtual void OnDrawRibbonApplicationButton(CDC* pDC, CMFCRibbonButton* pButton);
    virtual void OnDrawRibbonButtonBorder(CDC* pDC, CMFCRibbonButton* pButton);
    virtual unsigned long OnDrawRibbonButtonsGroup(CDC* pDC, CMFCRibbonButtonsGroup* pGroup, CRect rectGroup);
    virtual void OnDrawRibbonCaption(CDC* pDC, CMFCRibbonBar* pBar, CRect rectCaption, CRect rectText);
    virtual void OnDrawRibbonCaptionButton(CDC* pDC, CMFCRibbonCaptionButton* pButton);
    virtual void OnDrawRibbonCategory(CDC* pDC, CMFCRibbonCategory* pCategory, CRect rectCategory);
    virtual unsigned long OnDrawRibbonCategoryCaption(CDC* pDC, CMFCRibbonContextCaption* pCaption);
    virtual void OnDrawRibbonCategoryScroll(CDC* pDC, CMFCRibbonCategoryScroll* pScroll);
    virtual void OnDrawRibbonCategoryTab(CDC* pDC, CMFCRibbonTab* pTab, BOOL bIsActive);
    virtual void OnDrawRibbonCheckBoxOnList(CDC* pDC, CMFCRibbonCheckBox* pCheckBox, CRect rect, BOOL bIsSelected, BOOL bHighlighted);
    virtual void OnDrawRibbonDefaultPaneButton(CDC* pDC, CMFCRibbonButton* pButton);
    virtual void OnDrawRibbonDefaultPaneButtonContext(CDC* pDC, CMFCRibbonButton* pButton);
    virtual void OnDrawRibbonDesign(CDC* pDC, CMFCRibbonBar* pBar, CRect rect);
    virtual void OnDrawRibbonEdit(CDC* pDC, CMFCRibbonEdit* pEdit, CRect rect, BOOL bIsHighlighted, BOOL bIsPaneHighlighted, BOOL bIsDisabled);
    virtual void OnDrawRibbonGalleryBorder(CDC* pDC, CMFCRibbonGallery* pGallery, CRect rect);
    virtual void OnDrawRibbonLabel(CDC* pDC, CMFCRibbonLabel* pLabel, CRect rect);
    virtual void OnDrawRibbonMainPanelButtonBorder(CDC* pDC, CMFCRibbonButton* pButton);
    virtual void OnDrawRibbonMainPanelFrame(CDC* pDC, CMFCRibbonMainPanel* pPanel, CRect rect);
    virtual void OnDrawRibbonPanel(CDC* pDC, CMFCRibbonPanel* pPanel, CRect rectPanel, CRect rectCaption);
    virtual void OnDrawRibbonPanelCaption(CDC* pDC, CMFCRibbonPanel* pPanel, CRect rectCaption);
    virtual void OnDrawRibbonProgressBar(CDC* pDC, CMFCRibbonProgressBar* pProgress, CRect rectProgress, CRect rectChunk, BOOL bInfiniteMode);
    virtual void OnDrawRibbonQATSeparator(CDC* pDC, CMFCRibbonSeparator* pSeparator, CRect rect);
    virtual void OnDrawRibbonQuickAccessToolBarSeparator(CDC* pDC, CMFCRibbonSeparator* pSeparator, CRect rect);
    virtual void OnDrawRibbonSliderChannel(CDC* pDC, CMFCRibbonSlider* pSlider, CRect rect);
    virtual void OnDrawRibbonSliderThumb(CDC* pDC, CMFCRibbonSlider* pSlider, CRect rect, BOOL bIsHighlighted, BOOL bIsPressed, BOOL bIsDisabled);
    virtual void OnDrawRibbonSliderZoomButton(CDC* pDC, CMFCRibbonSlider* pSlider, CRect rect, BOOL bIsZoomOut, BOOL bIsHighlighted, BOOL bIsPressed, BOOL bIsDisabled);
    virtual void OnDrawRibbonStatusBarPane(CDC* pDC, CMFCRibbonStatusBar* pBar, CMFCRibbonStatusBarPane* pPane);
    virtual void OnDrawRibbonTabsFrame(CDC* pDC, CMFCRibbonBar* pBar, CRect rectTab);
    virtual void OnDrawScrollButtons(CDC* pDC, const CRect& rect, const int nBorderSize, int iImage, BOOL bHilited);
    virtual void OnDrawSeparator(CDC* pDC, CBasePane* pBar, CRect rect, BOOL bHorz);
    virtual void OnDrawShowAllMenuItems(CDC* pDC, CRect rect, CMFCVisualManager::AFX_BUTTON_STATE state);
    virtual void OnDrawSpinButtons(CDC* pDC, CRect rectSpin, int nState, BOOL bOrientation, BOOL bIsHovered);
    virtual void OnDrawStatusBarPaneBorder(CDC* pDC, CMFCStatusBar* pBar, CRect rectPane, UINT uiID, UINT nStyle);
    virtual void OnDrawStatusBarProgress(CDC* pDC, CMFCStatusBar* pBar, CRect rectProgress, int nProgressTotal, int nProgressCurr, COLORREF clrBar, COLORREF clrProgressBarDest, COLORREF clrProgressText, BOOL bProgressText);
    virtual void OnDrawStatusBarSizeBox(CDC* pDC, CMFCStatusBar* pBar, CRect rectSizeBox);
    virtual void OnDrawTab(CDC* pDC, CRect rectTab, int iTab, BOOL bIsActive, const CMFCBaseTabCtrl* pTabWnd);
    virtual void OnDrawTabButton(CDC* pDC, CRect rect, const CMFCBaseTabCtrl* pTabWnd, int nID, BOOL bIsHilited, BOOL bIsPressed);
    virtual void OnDrawTabCloseButton(CDC* pDC, CRect rect, const CMFCBaseTabCtrl* pTabWnd, BOOL bIsHilited, BOOL bIsPressed, BOOL bIsDisabled);
    virtual void OnDrawTabContent(CDC* pDC, CRect rectTab, int iTab, BOOL bIsActive, const CMFCBaseTabCtrl* pTabWnd, COLORREF clrText);
    virtual void OnDrawTabsButtonBorder(CDC* pDC, CRect& rect, CMFCButton* pButton, UINT uiState, CMFCBaseTabCtrl* pWndTab);
    virtual void OnDrawTask(CDC* pDC, CMFCTasksPaneTask* pTask, CImageList* pIcons, BOOL bIsHighlighted, BOOL bIsSelected);
    virtual void OnDrawTasksGroupCaption(CDC* pDC, CMFCTasksPaneTaskGroup* pGroup, BOOL bIsHighlighted, BOOL bIsSelected, BOOL bCanCollapse);
    virtual void OnDrawTasksGroupAreaBorder(CDC* pDC, CRect rect, BOOL bSpecial, BOOL bNoTitle);
    virtual void OnDrawTearOffCaption(CDC* pDC, CRect rectCaption, BOOL bIsActive);
    virtual void OnDrawToolBoxFrame(CDC* pDC, const CRect& rect);

    COLORREF GetHighlightedColor(UINT nColorIndex) const;
    static COLORREF GetThemeColor(COLORREF clrBase, int nIntensity);

protected:
    char _visualmanager_padding[256];
};

// Office variant visual managers (minimal stubs)
class CMFCVisualManagerOffice2003 : public CMFCVisualManager {
    DECLARE_DYNAMIC(CMFCVisualManagerOffice2003)
public:
    CMFCVisualManagerOffice2003();
    virtual ~CMFCVisualManagerOffice2003();
    static CObject* AFXAPI CreateObject();
protected:
    char _pad[64];
};

class CMFCVisualManagerOffice2007 : public CMFCVisualManager {
    DECLARE_DYNAMIC(CMFCVisualManagerOffice2007)
public:
    enum Style {
        Office2007_LunaBlue = 0,
        Office2007_ObsidianBlack = 1,
        Office2007_Silver = 2,
        Office2007_Aqua = 3
    };
    CMFCVisualManagerOffice2007();
    virtual ~CMFCVisualManagerOffice2007();
    static CObject* AFXAPI CreateObject();
    static int SetStyle(Style style, const wchar_t* lpszPath = nullptr);
    static Style GetStyle();
protected:
    char _pad[64];
};

class CMFCVisualManagerOfficeXP : public CMFCVisualManager {
    DECLARE_DYNAMIC(CMFCVisualManagerOfficeXP)
public:
    CMFCVisualManagerOfficeXP();
    CMFCVisualManagerOfficeXP(int) : CMFCVisualManagerOfficeXP() {}
    virtual ~CMFCVisualManagerOfficeXP();
    static CObject* AFXAPI CreateObject();
protected:
    char _pad[64];
};

class CMFCVisualManagerVS2005 : public CMFCVisualManager {
    DECLARE_DYNAMIC(CMFCVisualManagerVS2005)
public:
    CMFCVisualManagerVS2005();
    virtual ~CMFCVisualManagerVS2005();
    static CObject* AFXAPI CreateObject();
protected:
    char _pad[64];
};

class CMFCVisualManagerWindows : public CMFCVisualManager {
    DECLARE_DYNAMIC(CMFCVisualManagerWindows)
public:
    CMFCVisualManagerWindows();
    CMFCVisualManagerWindows(int) : CMFCVisualManagerWindows() {}
    virtual ~CMFCVisualManagerWindows();
    static CObject* AFXAPI CreateObject();
protected:
    char _pad[64];
};

class CMFCVisualManagerWindows7 : public CMFCVisualManager {
    DECLARE_DYNAMIC(CMFCVisualManagerWindows7)
public:
    CMFCVisualManagerWindows7();
    virtual ~CMFCVisualManagerWindows7();
    static CObject* AFXAPI CreateObject();
    static int SetStyle(const wchar_t* lpszPath);
protected:
    char _pad[64];
};
class CMFCVisualManagerAero : public CMFCVisualManager { DECLARE_DYNAMIC(CMFCVisualManagerAero) public: CMFCVisualManagerAero(); virtual ~CMFCVisualManagerAero(); protected: char _pad[64]; };

//=============================================================================
// CBasePane - Base pane class
//=============================================================================
class CBasePane : public CWnd {
    DECLARE_DYNAMIC(CBasePane)
public:
    CBasePane();
    virtual ~CBasePane();

    virtual BOOL CanAcceptPane(const CBasePane* pBar) const;
    virtual BOOL CanAutoHide() const;
    virtual BOOL CanBeAttached() const;
    virtual BOOL CanBeClosed() const;
    virtual BOOL CanBeDocked() const;
    virtual BOOL CanBeFloating() const;
    virtual BOOL CanBeResized() const;
    virtual BOOL CanFloat() const;
    virtual BOOL DoesAllowSiblingBars() const;
    virtual BOOL HasGripper() const;
    virtual BOOL IsAccessibilityCompatible();
    void SetPaneAlignment(DWORD dwAlignment);
    DWORD GetPaneAlignment() const;
    virtual void* SetWindowPos(const CWnd* pWndInsertAfter, int x, int y, int cx, int cy, unsigned int nFlags, void* pExtra);
    virtual void CalcFixedLayout(BOOL bStretch, BOOL bHorz);
    virtual void RecalcLayout();

protected:
    char _basepane_padding[128];
};

//=============================================================================
// CPane - Standard docking pane
//=============================================================================
class CPane : public CBasePane {
    DECLARE_DYNAMIC(CPane)
public:
    CPane();
    virtual ~CPane();

    virtual BOOL CanBeDocked() const override;
    virtual BOOL CanFloat() const override;
    virtual void RecalcLayout() override;

protected:
    char _pane_padding[64];
};

//=============================================================================
// CDockablePane - Dockable pane with caption
//=============================================================================
class CDockablePane : public CPane {
    DECLARE_DYNAMIC(CDockablePane)
public:
    CDockablePane();
    virtual ~CDockablePane();

    virtual BOOL CanBeAttached() const override;
    virtual BOOL CanAutoHide() const override;
    void EnableAutohideAll();
    virtual CMFCAutoHideBar* SetAutoHideMode(int bAutoHideMode, unsigned long dwAlignment, void* pAutoHideBar = nullptr, int bSetFocus = 0);
    BOOL IsAutoHideMode() const;
    BOOL IsTabbed() const;

protected:
    char _dockablepane_padding[128];
};

//=============================================================================
// CMFCToolBarButton
//=============================================================================
class CMFCToolBarButton : public CObject {
    DECLARE_DYNAMIC(CMFCToolBarButton)
public:
    CMFCToolBarButton();
    CMFCToolBarButton(UINT uiCmdID, int iImage, const wchar_t* lpszText = nullptr, BOOL bUserButton = FALSE, BOOL bLocked = FALSE);
    virtual ~CMFCToolBarButton();

    static void SetClipboardFormatName(const wchar_t* lpszClipboardFormatName);

    UINT m_nID;
    int m_iImage;
    CString m_strText;
    BOOL m_bUserButton;
    BOOL m_bLocked;

    static CString m_strClipboardFormatName;
    static BOOL m_bWrapText;
    static BOOL m_bUpdateImages;

protected:
    char _mfctoolbarbutton_padding[32];
};

//=============================================================================
// CMFCToolBar - Feature Pack toolbar
//=============================================================================
class CMFCToolBar : public CBasePane {
    DECLARE_DYNAMIC(CMFCToolBar)
public:
    CMFCToolBar();
    virtual ~CMFCToolBar();

    virtual BOOL Create(CWnd* pParentWnd, DWORD dwStyle = WS_CHILD | WS_VISIBLE | CBRS_TOP, UINT nID = AFX_IDW_TOOLBAR);
    virtual BOOL CreateEx(CWnd* pParentWnd, DWORD dwCtrlStyle, DWORD dwStyle = WS_CHILD | WS_VISIBLE | CBRS_TOP, CRect rcBorders = CRect(0,0,0,0), UINT nID = AFX_IDW_TOOLBAR);

    virtual BOOL LoadToolBar(UINT uiResID, UINT uiColdResID = 0, UINT uiMenuResID = 0, BOOL bLocked = FALSE, UINT uiDisabledResID = 0, UINT uiMenuDisabledResID = 0, UINT uiHotResID = 0);
    BOOL LoadBitmap(UINT nIDResource);
    virtual BOOL SetButtons(const UINT* lpIDArray, int nIDCount, BOOL bImages = TRUE);
    BOOL ReplaceButton(UINT nID, const CMFCToolBarButton& button, BOOL bNotify = FALSE);
    int GetCount() const;
    CMFCToolBarButton* GetButton(int nIndex) const;
    static void SetSizes(SIZE sizeButton, SIZE sizeImage);
    CSize GetButtonSize() const;
    virtual void EnableDocking(DWORD dwDockStyle);
    virtual void AdjustLayout();
    void AdjustSize();
    CString GetButtonText(int nIndex) const;
    void GetButtonText(int nIndex, CString& rString) const;

protected:
    char _mfctoolbar_padding[128];
};

//=============================================================================
// CMFCMenuBar - Feature Pack menu bar
//=============================================================================
class CMFCMenuBar : public CMFCToolBar {
    DECLARE_DYNAMIC(CMFCMenuBar)
public:
    CMFCMenuBar();
    virtual ~CMFCMenuBar();

    virtual BOOL Create(CWnd* pParentWnd, DWORD dwStyle = WS_CHILD | WS_VISIBLE | CBRS_TOP, UINT nID = AFX_IDW_TOOLBAR) override;
    virtual BOOL CreateEx(CWnd* pParentWnd, DWORD dwCtrlStyle, DWORD dwStyle = WS_CHILD | WS_VISIBLE | CBRS_TOP, CRect rcBorders = CRect(0,0,0,0), UINT nID = AFX_IDW_TOOLBAR) override;
    virtual void CreateFromMenu(HMENU hMenu, BOOL bDefaultMenu = FALSE, BOOL bForceUpdate = FALSE);
    CMFCToolBarButton* GetMenuItem(int nIndex) const;
    virtual CSize CalcLayout(DWORD dwMode, int nLength = -1);
    virtual void AdjustLocations();
    static CFont& GetMenuFont(BOOL bHorz = TRUE);

protected:
    char _mfcmenubar_padding[96];
};

//=============================================================================
// CMFCToolBarImages - Feature Pack toolbar image collection
//=============================================================================
class CMFCToolBarImages {
public:
    CMFCToolBarImages();
    explicit CMFCToolBarImages(BOOL bReadOnly);
    virtual ~CMFCToolBarImages();

    BOOL Load(UINT nIDResource, HINSTANCE hInstRes = nullptr, BOOL bAdd = FALSE);
    BOOL Load(const wchar_t* lpszBitmapFileName, DWORD dwMaxFileSize = 0);
    BOOL LoadStr(const wchar_t* lpszResourceName, HINSTANCE hInstRes, BOOL bAdd = FALSE);
    BOOL Save(const wchar_t* lpszBitmapFileName = nullptr);

    int AddImage(HBITMAP hbmp, BOOL bSetBitmapSize = FALSE);
    int AddImage(const CMFCToolBarImages& images, BOOL bSetBitmapSize = FALSE);
    int AddIcon(HICON hIcon, BOOL bAutoDestroy = FALSE);

    void Clear();
    void Initialize();
    void CommonInit(BOOL bFreeImageList = TRUE);

    int GetCount() const;
    BOOL IsValid() const;

    BOOL m_bIsRTL;
    BYTE m_nDisabledImageAlpha;
    BYTE m_nFadedImageAlpha;

    static BOOL m_bDisableTrueColorAlpha;
    static BOOL m_bIsDrawOnGlass;
    static BOOL m_bMultiThreaded;

protected:
    char _mfctoolbarimages_padding[64];
};

//=============================================================================
// CMFCRibbonBaseElement - Ribbon element base
//=============================================================================
class CMFCRibbonBaseElement : public CObject {
    DECLARE_DYNAMIC(CMFCRibbonBaseElement)
public:
    CMFCRibbonBaseElement();
    virtual ~CMFCRibbonBaseElement();

    UINT GetID() const;
    void SetID(UINT nID);
    CString GetText() const;
    void SetText(const wchar_t* lpszText);

protected:
    UINT m_nID;
    CString m_strText;
    char _ribbonbaseelement_padding[32];
};

//=============================================================================
// CMFCRibbonButton - Ribbon button
//=============================================================================
class CMFCRibbonButton : public CMFCRibbonBaseElement {
    DECLARE_DYNAMIC(CMFCRibbonButton)
public:
    CMFCRibbonButton();
    CMFCRibbonButton(UINT nID, const wchar_t* lpszText, int nSmallImageIndex = -1, int nLargeImageIndex = -1, BOOL bAlwaysShowDescription = FALSE);
    CMFCRibbonButton(UINT nID, const wchar_t* lpszText, HICON hIcon, BOOL bAlwaysShowDescription = FALSE, HICON hIconSmall = nullptr, BOOL bAutoDestroyIcon = FALSE, BOOL bAlphaBlendIcon = FALSE);
    virtual ~CMFCRibbonButton();

protected:
    char _ribbonbutton_padding[64];
};

//=============================================================================
// CMFCRibbonPanel - Ribbon panel
//=============================================================================
class CMFCRibbonPanel : public CObject {
    DECLARE_DYNAMIC(CMFCRibbonPanel)
public:
    CMFCRibbonPanel(const wchar_t* lpszName = nullptr, HICON hIcon = nullptr);
    virtual ~CMFCRibbonPanel();

    void Add(CMFCRibbonBaseElement* pElem);
    void AddSeparator();
    int GetCount() const;
    CMFCRibbonBaseElement* GetElement(int nIndex) const;
    CString GetName() const;

protected:
    CString m_strName;
    HICON m_hIcon;
    char _ribbonpanel_padding[64];
};

//=============================================================================
// CMFCRibbonCategory - Ribbon category (tab)
//=============================================================================
class CMFCRibbonCategory : public CObject {
    DECLARE_DYNAMIC(CMFCRibbonCategory)
public:
    CMFCRibbonCategory(const wchar_t* lpszName = nullptr, UINT uiSmallImageResID = 0, UINT uiLargeImageResID = 0);
    virtual ~CMFCRibbonCategory();

    CMFCRibbonPanel* AddPanel(const wchar_t* lpszLabel, HICON hIcon, CRuntimeClass* pRTI = nullptr);
    void AddPanel(CMFCRibbonPanel* pPanel);
    int GetPanelCount() const;
    CMFCRibbonPanel* GetPanel(int nIndex) const;
    CString GetName() const;

protected:
    CString m_strName;
    char _ribboncategory_padding[64];
};

//=============================================================================
// CMFCRibbonBar - Ribbon bar control
//=============================================================================
class CMFCRibbonBar : public CPane {
    DECLARE_DYNAMIC(CMFCRibbonBar)
public:
    CMFCRibbonBar(BOOL bReplaceFrameCaption = TRUE);
    virtual ~CMFCRibbonBar();

    virtual BOOL Create(CWnd* pParentWnd, DWORD dwStyle = WS_CHILD | WS_VISIBLE | CBRS_TOP, UINT nID = AFX_IDW_RIBBON_BAR);
    CMFCRibbonCategory* AddCategory(const wchar_t* lpszName, unsigned int uiSmallImage, unsigned int uiLargeImage, CSize sizeSmall, unsigned int uiAnimLargeImage, int nInsertAt = -1, CRuntimeClass* pRTI = nullptr);
    BOOL AddCategory(CMFCRibbonCategory* pCategory);
    int GetCategoryCount() const;
    CMFCRibbonCategory* GetCategory(int nIndex) const;
    BOOL SetActiveCategory(CMFCRibbonCategory* pCategory, BOOL bForceRestore = FALSE);
    void AddToTabs(CMFCRibbonBaseElement* pElement);
    void SetQuickAccessToolbar(CMFCRibbonButtonsGroup* pQAT);
    void ToggleMimimizeState();
    BOOL IsMinimized() const;

protected:
    char _ribbonbar_padding[128];
};

//=============================================================================
// CMFCButton - Feature Pack button (CMiniFrameWnd defined above)
//=============================================================================
class CMFCButton : public CButton {
    DECLARE_DYNAMIC(CMFCButton)
public:
    CMFCButton();
    virtual ~CMFCButton();

    void SetImage(HICON hIcon, BOOL bAutoDestroy = TRUE, HICON hIconHot = nullptr, BOOL bAlphaBlend = FALSE);
    void SetFaceColor(COLORREF crFace, BOOL bRedraw = TRUE);
    void SetTextColor(COLORREF crText);
    void SetTextHotColor(COLORREF crTextHot);
    BOOL IsOwnerDraw() const;
    void EnableFullTextTooltip(BOOL bEnable = TRUE);
    void SetTooltip(const wchar_t* lpszToolTipText);

protected:
    char _mfcbutton_padding[64];
};

//=============================================================================
// CMFCPopupMenu - Feature Pack popup menu
//=============================================================================
class CMFCPopupMenu : public CMiniFrameWnd {
    DECLARE_DYNAMIC(CMFCPopupMenu)
public:
    CMFCPopupMenu();
    virtual ~CMFCPopupMenu();

    static CMFCPopupMenu* GetActiveMenu();
    static CMFCPopupMenu* GetSafeActivePopupMenu();
    static void SetForceMenuFocus(BOOL bForceFocus = TRUE);
    static BOOL ActivatePopupMenu(CFrameWnd* pTopFrame, CMFCPopupMenu* pPopupMenu);
    BOOL Create(CWnd* pParentWnd, int x, int y, HMENU hMenu, BOOL bLocked = FALSE, BOOL bIsMainMenu = FALSE);
    void CloseMenu(BOOL bSetFocusToBar = FALSE);
    int GetMenuItemCount() const;
    CMFCToolBarMenuButton* GetMenuItem(int nIndex) const;
    CMFCToolBarMenuButton* GetSelItem();
    CMFCToolBarMenuButton* FindSubItemByCommand(UINT uiCmd) const;
    BOOL InsertItem(const CMFCToolBarMenuButton& button, int iInsertAt = -1);
    BOOL InsertSeparator(int iInsertAt = -1);
    void EnableResize(CSize sizeMinResize);
    void EnableVertResize(BOOL bEnable = TRUE);
    BOOL HideRarelyUsedCommands() const;

protected:
    char _mfcpopupmenu_padding[64];
};

//=============================================================================
// CMFCPopupMenuBar - Toolbar hosted inside popup menus
//=============================================================================
class CMFCPopupMenuBar : public CMFCToolBar {
    DECLARE_DYNAMIC(CMFCPopupMenuBar)
public:
    CMFCPopupMenuBar();
    virtual ~CMFCPopupMenuBar();

    virtual BOOL ImportFromMenu(HMENU hMenu, BOOL bShowAllCommands = FALSE);
    virtual HMENU ExportToMenu() const;
    BOOL BuildOrigItems(UINT uiMenuResID);
    CMFCToolBarMenuButton* GetMenuItem(int nIndex) const;
    int GetGutterWidth() const;
    virtual CSize CalcSize(BOOL bVertDock);
    virtual void AdjustLayout();
    virtual void AdjustLocations();
    virtual void CloseDelayedSubMenu();

protected:
    char _mfcpopupmenubar_padding[96];
};

//=============================================================================
// CMFCBaseTabCtrl - Base tab control
//=============================================================================
class CMFCBaseTabCtrl : public CWnd {
    DECLARE_DYNAMIC(CMFCBaseTabCtrl)
public:
    CMFCBaseTabCtrl();
    virtual ~CMFCBaseTabCtrl();

    virtual BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);
    int GetTabsCount() const;
    void AddTab(CWnd* pWnd, const wchar_t* lpszLabel, UINT uiId = (UINT)-1);
    int RemoveTab(int nIndex, int nOption = 0);
    void SetActiveTab(int nIndex);
    int GetActiveTab() const;
    CWnd* GetTabWnd(int nIndex) const;

protected:
    char _mfcbasetabctrl_padding[128];
};

//=============================================================================
// CMFCTabCtrl - Feature Pack tab control
//=============================================================================
class CMFCTabCtrl : public CMFCBaseTabCtrl {
    DECLARE_DYNAMIC(CMFCTabCtrl)
public:
    CMFCTabCtrl();
    virtual ~CMFCTabCtrl();

    void EnableTabSwap(BOOL bEnable = TRUE);
    void SetTabBorderSize(int nTabBorderSize);
    void SetTabsHeight(int nTabHeight);

protected:
    char _mfctabctrl_padding[64];
};

//=============================================================================
// CMFCPropertyGridProperty - Property grid property
//=============================================================================
class CMFCPropertyGridProperty : public CObject {
    DECLARE_DYNAMIC(CMFCPropertyGridProperty)
public:
    CMFCPropertyGridProperty(const wchar_t* lpszName, const COleVariant& varValue,
                              const wchar_t* lpszDescr = nullptr, DWORD_PTR dwData = 0);
    virtual ~CMFCPropertyGridProperty();

    const CString& GetName() const;
    const COleVariant& GetValue() const;
    void SetValue(const COleVariant& varValue);
    BOOL IsModified() const;
    void SetModified(BOOL bModified = TRUE);
    BOOL IsEnabled() const;
    void SetEnabled(BOOL bEnable = TRUE);
    BOOL IsVisible() const;
    void Show(BOOL bShow = TRUE);
    int AddSubItem(CMFCPropertyGridProperty* pProp);
    int GetSubItemsCount() const;
    CMFCPropertyGridProperty* GetSubItem(int nIndex) const;
    void RemoveAllSubItems();
    void Expand(BOOL bExpand = TRUE);
    BOOL IsExpanded() const;
    void SetData(DWORD_PTR dwData);
    DWORD_PTR GetData() const;

protected:
    CString m_strName;
    COleVariant m_varValue;
    CString m_strDescr;
    DWORD_PTR m_dwData;
    BOOL m_bModified;
    BOOL m_bEnabled;
    BOOL m_bVisible;
    BOOL m_bExpanded;
    char _propgridproperty_padding[64];
};

//=============================================================================
// CMFCPropertyGridCtrl - Property grid control
//=============================================================================
class CMFCPropertyGridCtrl : public CWnd {
    DECLARE_DYNAMIC(CMFCPropertyGridCtrl)
public:
    CMFCPropertyGridCtrl();
    virtual ~CMFCPropertyGridCtrl();

    virtual BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);
    int AddProperty(CMFCPropertyGridProperty* pProp, int nPos = -1, int bRedraw = TRUE);
    int GetPropertyCount() const;
    CMFCPropertyGridProperty* GetProperty(int nIndex) const;
    void RemoveAll();
    void ExpandAll(BOOL bExpand = TRUE);
    void AdjustLayout();
    void SetDescriptionRows(int nRows);

protected:
    char _propgridctrl_padding[128];
};

//=============================================================================
// CMFCTasksPaneTask - Tasks pane task item
//=============================================================================
class CMFCTasksPaneTask : public CObject {
    DECLARE_DYNAMIC(CMFCTasksPaneTask)
public:
    CMFCTasksPaneTask(UINT nID, const wchar_t* lpszLabel, int nIcon = -1, BOOL bIsSeparator = FALSE);
    virtual ~CMFCTasksPaneTask();

protected:
    char _taskspanetask_padding[32];
};

//=============================================================================
// CMFCTasksPane - Tasks pane control
//=============================================================================
class CMFCTasksPane : public CBasePane {
    DECLARE_DYNAMIC(CMFCTasksPane)
public:
    CMFCTasksPane();
    virtual ~CMFCTasksPane();

    virtual BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);
    int AddTask(int nGroup, const wchar_t* lpszName, int nIcon = -1, unsigned int uiCmdID = 0, unsigned __int64 dwUserData = 0);
    void AddTask(int nGroup, CMFCTasksPaneTask* pTask);
    void RemoveAllTasks();
    void SetCaption(int nGroup, const wchar_t* lpszCaption);
    BOOL SetGroupName(int nGroup, const wchar_t* lpszName);
    int AddGroup(const wchar_t* lpszName, BOOL bBottomHasGripper = FALSE, BOOL bSpecial = FALSE, int nIcon = -1);

protected:
    char _taskspane_padding[128];
};

//=============================================================================
// CPaneFrameWnd - Floating pane frame window
//=============================================================================
class CPaneFrameWnd : public CMiniFrameWnd {
    DECLARE_DYNAMIC(CPaneFrameWnd)
public:
    CPaneFrameWnd();
    virtual ~CPaneFrameWnd();

    virtual BOOL Create(const wchar_t* lpszClassName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, CCreateContext* pContext = nullptr);
    virtual BOOL Create(CWnd* pParentWnd, DWORD dwStyle, UINT nID);
    virtual void AddPane(CBasePane* pBar);
    virtual void RemovePane(CBasePane* pBar, BOOL bDestroy = FALSE);
    virtual void AdjustLayout();
    virtual void CalcBorderSize(CRect& rect) const;
    virtual void CalcMinSize(CSize& size, MINMAXINFO* pMMI);
    virtual void GetCaptionRect(CRect& rect) const;
    virtual CString GetCaptionText();
    virtual CWnd* GetFirstVisiblePane() const;
    virtual CWnd* GetPane() const;
    virtual BOOL IsRollDown() const;
    virtual BOOL IsRollUp() const;

protected:
    char _paneframewnd_padding[128];
};

//=============================================================================
// CDockingManager - Docking layout manager
//=============================================================================
class CDockingManager : public CObject {
    DECLARE_DYNAMIC(CDockingManager)
public:
    CDockingManager();
    CDockingManager(CFrameWnd* pParentFrameWnd);
    virtual ~CDockingManager();

    void DockPane(CBasePane* pBar, UINT nDockBarID = 0, LPCRECT lpRect = nullptr);
    BOOL DockPaneLeftOf(CBasePane* pBarToDock, CBasePane* pBar);
    BOOL EnableDocking(DWORD dwDockStyle);
    void FloatPane(CBasePane* pBar, CPoint ptOffset, DWORD dwAlignment);
    void HidePane(CBasePane* pBar);
    void ShowPane(CBasePane* pBar, BOOL bDelay = FALSE);
    void RecalcLayout();
    void SetDockState();

protected:
    CFrameWnd* m_pParentWnd;
    char _dockingmanager_padding[128];
};

//=============================================================================
// Frame Window Extensions
//=============================================================================
class CFrameWndEx : public CFrameWnd {
    DECLARE_DYNAMIC(CFrameWndEx)
public:
    CFrameWndEx();
    virtual ~CFrameWndEx();

    virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle,
                        const RECT& rect, CWnd* pParentWnd, LPCTSTR lpszMenuName,
                        DWORD dwExStyle, CCreateContext* pContext) override;
    BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle, CWnd* pParentWnd, CCreateContext* pContext) override;

    CMFCVisualManager* GetVisualManager() const;
    CDockingManager* GetDockingManager();
    void EnableDocking(DWORD dwDockStyle);

protected:
    CMFCVisualManager* m_pVisualManager;
    CDockingManager* m_pDockingManager;
    char _framewndex_padding[128];
};

class CMDIFrameWndEx : public CMDIFrameWnd {
    DECLARE_DYNAMIC(CMDIFrameWndEx)
public:
    CMDIFrameWndEx();
    virtual ~CMDIFrameWndEx();

    CMFCVisualManager* GetVisualManager() const;

protected:
    CMFCVisualManager* m_pVisualManager;
    char _mdiframewndex_padding[128];
};

class CMDIChildWndEx : public CMDIChildWnd {
    DECLARE_DYNAMIC(CMDIChildWndEx)
public:
    CMDIChildWndEx();
    virtual ~CMDIChildWndEx();

protected:
    char _mdichildwndex_padding[64];
};

//=============================================================================
// Forward type stubs — classes referenced by CMFCVisualManager methods
//=============================================================================
class CMFCCaptionButton : public CObject {
public:
    CMFCCaptionButton() {}
    char _pad[16];
};

class CMFCHeaderCtrl : public CWnd {
public:
    CMFCHeaderCtrl() {}
    char _pad[16];
};

class CMFCToolBarComboBoxButton : public CMFCToolBarButton {
    DECLARE_DYNAMIC(CMFCToolBarComboBoxButton)
public:
    CMFCToolBarComboBoxButton();
    CMFCToolBarComboBoxButton(UINT uiID, int iImage, ULONG dwStyle = CBS_DROPDOWNLIST, int iWidth = 0) : CMFCToolBarComboBoxButton() {}
    virtual ~CMFCToolBarComboBoxButton();

    virtual INT_PTR AddItem(const wchar_t* lpszItem, DWORD_PTR dwData = 0);
    virtual INT_PTR AddSortedItem(const wchar_t* lpszItem, DWORD_PTR dwData = 0);
    virtual int Compare(const wchar_t* lpszItem1, const wchar_t* lpszItem2);
    BOOL DeleteItem(int nIndex);
    BOOL DeleteItem(DWORD_PTR dwData);
    BOOL DeleteItem(const wchar_t* lpszText);
    int FindItem(const wchar_t* lpszText) const;
    const wchar_t* GetItem(int nIndex = -1) const;
    INT_PTR GetCount() const;
    DWORD_PTR GetItemData(int nIndex = -1) const;
    BOOL SelectItem(const wchar_t* lpszItem);
    BOOL SelectItem(int nIndex, BOOL bNotify = TRUE);
    BOOL SelectItem(DWORD_PTR dwData);
    void SetText(const wchar_t* lpszText);

    static CMFCToolBarComboBoxButton* GetByCmd(UINT uiCmd, BOOL bIsFocus = FALSE);
    static int GetCountAll(UINT uiCmd);
    static int GetCurSelAll(UINT uiCmd);
    static const wchar_t* GetItemAll(UINT uiCmd, int iIndex);
    static DWORD_PTR GetItemDataAll(UINT uiCmd, int iIndex);
    static const wchar_t* GetTextAll(UINT uiCmd);
    static BOOL SelectItemAll(UINT uiCmd, const wchar_t* lpszText);
    static BOOL SelectItemAll(UINT uiCmd, int nIndex);
    static BOOL SelectItemAll(UINT uiCmd, DWORD_PTR dwData);

    BOOL m_bFlat;
    BOOL m_bCenterVert;

    char _pad[32];
};

class CMFCToolBarEditBoxButton : public CMFCToolBarButton {
    DECLARE_DYNAMIC(CMFCToolBarEditBoxButton)
public:
    CMFCToolBarEditBoxButton();
    CMFCToolBarEditBoxButton(UINT uiID, int iImage, ULONG dwStyle = ES_AUTOHSCROLL, int iWidth = 0) : CMFCToolBarEditBoxButton() {}
    virtual ~CMFCToolBarEditBoxButton();

    virtual void SetContents(const CString& sContents);

    static CMFCToolBarEditBoxButton* GetByCmd(UINT uiCmd);
    static CString GetContentsAll(UINT uiCmd);
    static BOOL SetContentsAll(UINT uiCmd, const CString& sContents);

    BOOL m_bFlat;

    char _pad[32];
};

class CMFCToolBarMenuButton : public CMFCToolBarButton {
    DECLARE_DYNAMIC(CMFCToolBarMenuButton)
public:
    CMFCToolBarMenuButton();
    CMFCToolBarMenuButton(UINT uiID, HMENU hMenu, BOOL bHasDropDownArrow, const wchar_t* lpszText = nullptr, BOOL bUserButton = FALSE);
    CMFCToolBarMenuButton(const CMFCToolBarMenuButton& src);
    virtual ~CMFCToolBarMenuButton();

    void Initialize(UINT uiID, HMENU hMenu, BOOL bHasDropDownArrow, const wchar_t* lpszText, BOOL bUserButton);
    virtual void CreateFromMenu(HMENU hMenu);
    virtual HMENU CreateMenu() const;
    virtual void CopyFrom(const CMFCToolBarButton& src);
    virtual int CompareWith(const CMFCToolBarButton& other) const;

    HMENU m_hMenu;
    char _pad[64];
};

class CMFCToolBarMenuButtonsButton : public CMFCToolBarMenuButton {
    DECLARE_DYNAMIC(CMFCToolBarMenuButtonsButton)
public:
    CMFCToolBarMenuButtonsButton();
    explicit CMFCToolBarMenuButtonsButton(UINT uiSystemCommand);
    virtual ~CMFCToolBarMenuButtonsButton();

    UINT m_uiSystemCommand;
    char _padButtons[32];
};

//=============================================================================
// Feature Pack managers and app settings helpers
//=============================================================================
class CContextMenuManager : public CObject {
    DECLARE_DYNAMIC(CContextMenuManager)
public:
    CContextMenuManager();
    virtual ~CContextMenuManager();

    BOOL AddMenu(UINT uiMenuNameResId, UINT uiMenuResId);
    BOOL AddMenu(const wchar_t* lpszName, UINT uiMenuResId);
    HMENU GetMenuById(UINT uiMenuResId) const;
    HMENU GetMenuByName(const wchar_t* lpszName, UINT* puiMenuResId = nullptr) const;
    void GetMenuNames(CStringList& listOfNames) const;
    virtual BOOL ShowPopupMenu(UINT uiMenuResId, int x, int y, CWnd* pWndOwner, BOOL bOwnMessage = FALSE, BOOL bRightAlign = FALSE);
    virtual CMFCPopupMenu* ShowPopupMenu(HMENU hmenuPopup, int x, int y, CWnd* pWndOwner, BOOL bOwnMessage = FALSE, BOOL bAutoDestroy = TRUE, BOOL bRightAlign = FALSE);
    virtual UINT TrackPopupMenu(HMENU hmenuPopup, int x, int y, CWnd* pWndOwner, BOOL bRightAlign = FALSE);
    virtual BOOL LoadState(const wchar_t* lpszProfileName = nullptr);
    virtual BOOL SaveState(const wchar_t* lpszProfileName = nullptr);
    virtual BOOL ResetState();

protected:
    char _contextmenumanager_padding[64];
};

class CKeyboardManager : public CObject {
    DECLARE_DYNAMIC(CKeyboardManager)
public:
    CKeyboardManager();
    virtual ~CKeyboardManager();

    static BOOL IsKeyPrintable(UINT nChar);
    static UINT TranslateCharToUpper(UINT nChar);
    static void ShowAllAccelerators(BOOL bShowAll = TRUE);
    static void CleanUp();
    void ResetAll();
    BOOL LoadState(const wchar_t* lpszProfileName = nullptr, CFrameWnd* pDefaultFrame = nullptr);
    BOOL SaveState(const wchar_t* lpszProfileName = nullptr, CFrameWnd* pDefaultFrame = nullptr);

protected:
    char _keyboardmanager_padding[64];
};

class CMFCToolTipInfo {
public:
    CMFCToolTipInfo() { memset(_pad, 0, sizeof(_pad)); }
    char _pad[64];
};

class CTooltipManager : public CObject {
    DECLARE_DYNAMIC(CTooltipManager)
public:
    CTooltipManager();
    virtual ~CTooltipManager();

    static BOOL CreateToolTip(CToolTipCtrl*& pToolTip, CWnd* pWndParent, UINT nType = 0);
    static void DeleteToolTip(CToolTipCtrl*& pToolTip);
    void SetTooltipParams(UINT nTypes, CRuntimeClass* pRTC = nullptr, CMFCToolTipInfo* pParams = nullptr);
    void UpdateTooltips();

protected:
    char _tooltipmanager_padding[64];
};

class CMenuHash : public CObject {
    DECLARE_DYNAMIC(CMenuHash)
public:
    CMenuHash();
    virtual ~CMenuHash();

    BOOL LoadMenuBar(HMENU hMenu, CMFCToolBar* pBar);
    BOOL SaveMenuBar(HMENU hMenu, CMFCToolBar* pBar);
    BOOL RemoveMenu(HMENU hMenu);
    void CleanUp();

protected:
    char _menuhash_padding[64];
};

class CGlobalUtils {
public:
    CGlobalUtils();
    virtual ~CGlobalUtils();

    void AdjustRectToWorkArea(CRect& rect, CRect* pRectDelta = nullptr);
    void FlipRect(CRect& rect, BOOL bHorz);
    DWORD GetOppositeAlignment(DWORD dwAlign);
    CSize GetSystemBorders(DWORD dwStyle);
    CSize GetSystemBorders(CWnd* pWnd);
    HICON GetWndIcon(CWnd* pWnd);
    BOOL CanBeAttached(CWnd* pWnd) const;
    BOOL CanPaneBeInFloatingMultiPaneFrameWnd(CWnd* pWnd) const;
    CDockingManager* GetDockingManager(CWnd* pWnd);

protected:
    char _globalutils_padding[32];
};

class CWinAppEx : public CWinApp {
    DECLARE_DYNAMIC(CWinAppEx)
public:
    explicit CWinAppEx(BOOL bResourceSmartUpdate = FALSE);
    virtual ~CWinAppEx();

    BOOL InitContextMenuManager();
    BOOL InitKeyboardManager();
    BOOL InitTooltipManager();
    CContextMenuManager* GetContextMenuManager();
    CKeyboardManager* GetKeyboardManager();
    CTooltipManager* GetTooltipManager();
    int GetDataVersion() const;
    const wchar_t* SetRegistryBase(const wchar_t* lpszSectionName);
    CString GetRegSectionPath(const wchar_t* lpszSectionAdd = nullptr);
    int GetInt(const wchar_t* lpszEntry, int nDefault = 0);
    BOOL WriteInt(const wchar_t* lpszEntry, int nValue);
    CString GetString(const wchar_t* lpszEntry, const wchar_t* lpszDefault = nullptr);
    BOOL WriteString(const wchar_t* lpszEntry, const wchar_t* lpszValue);
    virtual BOOL LoadState(const wchar_t* lpszSectionName = nullptr, void* pFrameImpl = nullptr);
    virtual BOOL SaveState(const wchar_t* lpszSectionName = nullptr, void* pFrameImpl = nullptr);
    virtual BOOL CleanState(const wchar_t* lpszSectionName = nullptr);
    BOOL IsStateExists(const wchar_t* lpszSectionName);
    virtual int ExitInstance() override;

protected:
    CContextMenuManager* m_pContextMenuManager;
    CKeyboardManager* m_pKeyboardManager;
    CTooltipManager* m_pTooltipManager;
    CString m_strRegSection;
    int m_nDataVersion;
    char _winappex_padding[96];
};

class CMFCDesktopAlertWnd : public CWnd {
public: CMFCDesktopAlertWnd() {} char _pad[16]; };
class CMFCDesktopAlertWndButton : public CObject {
public: CMFCDesktopAlertWndButton() {} char _pad[16]; };
class CMFCStatusBar : public CStatusBar {
    DECLARE_DYNAMIC(CMFCStatusBar)
public: CMFCStatusBar(); virtual ~CMFCStatusBar(); char _pad[64]; };
class CPaneDivider : public CObject {
public:
    CPaneDivider() {}
    virtual ~CPaneDivider() {}
    virtual void AddPane(CDockablePane* pPane);
    virtual void RemovePane(CDockablePane* pPane);
    virtual BOOL CheckVisibility();
    CPaneContainer* FindPaneContainer(CDockablePane* pBar, int& nIndex);
    const CBasePane* GetFirstPane() const;
    void GetPaneDividers(CObList& lst);
    void GetPanes(CObList& lst);
    CRect GetRootContainerRect();
    virtual CSize CalcFixedLayout(BOOL bStretch, BOOL bHorz);
    char _pad[16];
};

class CMFCRibbonButtonsGroup : public CMFCRibbonBaseElement { DECLARE_DYNAMIC(CMFCRibbonButtonsGroup) public: CMFCRibbonButtonsGroup(); virtual ~CMFCRibbonButtonsGroup(); char _pad[32]; };
class CMFCRibbonCaptionButton : public CMFCRibbonBaseElement { DECLARE_DYNAMIC(CMFCRibbonCaptionButton) public: CMFCRibbonCaptionButton(); virtual ~CMFCRibbonCaptionButton(); char _pad[32]; };
class CMFCRibbonCategoryScroll : public CMFCRibbonBaseElement { DECLARE_DYNAMIC(CMFCRibbonCategoryScroll) public: CMFCRibbonCategoryScroll(); virtual ~CMFCRibbonCategoryScroll(); char _pad[32]; };
class CMFCRibbonTab : public CMFCRibbonBaseElement { DECLARE_DYNAMIC(CMFCRibbonTab) public: CMFCRibbonTab(); virtual ~CMFCRibbonTab(); char _pad[32]; };
class CMFCRibbonContextCaption : public CObject { DECLARE_DYNAMIC(CMFCRibbonContextCaption) public: CMFCRibbonContextCaption(); virtual ~CMFCRibbonContextCaption(); char _pad[32]; };
class CMFCAutoHideBar : public CPane { DECLARE_DYNAMIC(CMFCAutoHideBar) public: CMFCAutoHideBar(); virtual ~CMFCAutoHideBar(); char _pad[32]; };
class CMFCRibbonCheckBox : public CMFCRibbonBaseElement { DECLARE_DYNAMIC(CMFCRibbonCheckBox) public: CMFCRibbonCheckBox(); CMFCRibbonCheckBox(UINT, const wchar_t*) : CMFCRibbonCheckBox() {} virtual ~CMFCRibbonCheckBox(); char _pad[32]; };
class CMFCRibbonEdit : public CMFCRibbonBaseElement { DECLARE_DYNAMIC(CMFCRibbonEdit) public: CMFCRibbonEdit(); CMFCRibbonEdit(UINT, int, const wchar_t*, int) : CMFCRibbonEdit() {} virtual ~CMFCRibbonEdit(); char _pad[32]; };
class CMFCRibbonGallery : public CMFCRibbonBaseElement { DECLARE_DYNAMIC(CMFCRibbonGallery) public: CMFCRibbonGallery(); CMFCRibbonGallery(UINT, const wchar_t*, int, int, UINT, int) : CMFCRibbonGallery() {} virtual ~CMFCRibbonGallery(); char _pad[32]; };
class CMFCRibbonLabel : public CMFCRibbonBaseElement { DECLARE_DYNAMIC(CMFCRibbonLabel) public: CMFCRibbonLabel(); CMFCRibbonLabel(const wchar_t*, int) : CMFCRibbonLabel() {} virtual ~CMFCRibbonLabel(); char _pad[32]; };
class CMFCRibbonMainPanel : public CMFCRibbonBaseElement { DECLARE_DYNAMIC(CMFCRibbonMainPanel) public: CMFCRibbonMainPanel(); virtual ~CMFCRibbonMainPanel(); char _pad[32]; };
class CMFCRibbonProgressBar : public CMFCRibbonBaseElement { DECLARE_DYNAMIC(CMFCRibbonProgressBar) public: CMFCRibbonProgressBar(); CMFCRibbonProgressBar(UINT, int, int) : CMFCRibbonProgressBar() {} virtual ~CMFCRibbonProgressBar(); char _pad[32]; };
class CMFCRibbonSeparator : public CMFCRibbonBaseElement { DECLARE_DYNAMIC(CMFCRibbonSeparator) public: CMFCRibbonSeparator(); CMFCRibbonSeparator(int) : CMFCRibbonSeparator() {} virtual ~CMFCRibbonSeparator(); char _pad[32]; };
class CMFCRibbonSlider : public CMFCRibbonBaseElement { DECLARE_DYNAMIC(CMFCRibbonSlider) public: CMFCRibbonSlider(); CMFCRibbonSlider(UINT, int) : CMFCRibbonSlider() {} virtual ~CMFCRibbonSlider(); char _pad[32]; };
class CMFCRibbonStatusBar : public CBasePane { DECLARE_DYNAMIC(CMFCRibbonStatusBar) public: CMFCRibbonStatusBar(); virtual ~CMFCRibbonStatusBar(); char _pad[64]; };
class CMFCRibbonStatusBarPane : public CMFCRibbonBaseElement { DECLARE_DYNAMIC(CMFCRibbonStatusBarPane) public: CMFCRibbonStatusBarPane(); virtual ~CMFCRibbonStatusBarPane(); char _pad[32]; };
class CMFCTasksPaneTaskGroup : public CObject { DECLARE_DYNAMIC(CMFCTasksPaneTaskGroup) public: CMFCTasksPaneTaskGroup(); virtual ~CMFCTasksPaneTaskGroup(); char _pad[32]; };

//=============================================================================
// Visual Studio list box wrappers
//=============================================================================
class CVSListBoxBase : public CStatic {
    DECLARE_DYNAMIC(CVSListBoxBase)
public:
    CVSListBoxBase() = default;
    virtual ~CVSListBoxBase() = default;
};

class CVSListBoxEditCtrl : public CEdit {
    DECLARE_DYNAMIC(CVSListBoxEditCtrl)
public:
    CVSListBoxEditCtrl() = default;
    virtual ~CVSListBoxEditCtrl() = default;
};

class CVSListBox : public CListBox {
    DECLARE_DYNAMIC(CVSListBox)
public:
    CVSListBox() = default;
    virtual ~CVSListBox() = default;

    virtual int AddItem(const CString& strText, uintptr_t dwData = 0, int bSelect = 1);
    virtual int RemoveItem(int nIndex);
    virtual int GetCount() const;
    virtual int GetSelItem() const;
    virtual int SelectItem(int nIndex);
    virtual int EditItem(int nIndex);
    virtual uintptr_t GetItemData(int nIndex) const;
    virtual void SetItemData(int nIndex, uintptr_t dwData);
    virtual CString GetItemText(int nIndex) const;
    virtual void SetItemText(int nIndex, const CString& strText);
};

//=============================================================================
// Wave 2 D2D/Animation minimal declarations
//=============================================================================
struct CD2DColorF {
    float r;
    float g;
    float b;
    float a;
};
struct D2D_POINT_2F;
struct D2D_SIZE_F;
struct D2D_RECT_F;
struct D2D1_ELLIPSE;
struct D2D1_ROUNDED_RECT;

class CD2DPointF {
public:
    CD2DPointF();
    CD2DPointF(float x, float y);
    CD2DPointF(const CPoint& point);
    CD2DPointF(const D2D_POINT_2F& point);
    CD2DPointF(const D2D_POINT_2F* point);

    float x;
    float y;
};

class CD2DSizeF {
public:
    CD2DSizeF();
    CD2DSizeF(float width, float height);
    CD2DSizeF(const CSize& size);
    CD2DSizeF(const D2D_SIZE_F& size);
    CD2DSizeF(const D2D_SIZE_F* size);

    float width;
    float height;
};

class CD2DRectF {
public:
    CD2DRectF();
    CD2DRectF(float left, float top, float right, float bottom);
    CD2DRectF(const CRect& rect);
    CD2DRectF(const D2D_RECT_F& rect);
    CD2DRectF(const D2D_RECT_F* rect);

    float left;
    float top;
    float right;
    float bottom;
};

class CD2DEllipse {
public:
    CD2DEllipse();
    CD2DEllipse(const CD2DPointF& point, const CD2DSizeF& radius);
    CD2DEllipse(const CD2DRectF& rect);
    CD2DEllipse(const D2D1_ELLIPSE& ellipse);
    CD2DEllipse(const D2D1_ELLIPSE* ellipse);

    CD2DPointF point;
    CD2DSizeF radius;
};

class CD2DRoundedRect {
public:
    CD2DRoundedRect();
    CD2DRoundedRect(const CD2DRectF& rect, const CD2DSizeF& radius);
    CD2DRoundedRect(const D2D1_ROUNDED_RECT& roundedRect);
    CD2DRoundedRect(const D2D1_ROUNDED_RECT* roundedRect);

    CD2DRectF rect;
    CD2DSizeF radius;
};

class CRenderTarget : public CObject {
    DECLARE_DYNAMIC(CRenderTarget)
public:
    CRenderTarget();
    virtual ~CRenderTarget();

    void Attach(void* pRenderTarget);
    void* Detach();
    void BeginDraw();
    long EndDraw();
    int Destroy(int bReleasing = TRUE);
    void Clear(CD2DColorF color);

    void DrawLine(const CD2DPointF& p0, const CD2DPointF& p1);
    void DrawRectangle(const CD2DRectF& rect);
    void DrawEllipse(const CD2DEllipse& ellipse);
    void DrawRoundedRectangle(const CD2DRoundedRect& rect);
    void FillRectangle(const CD2DRectF& rect);
    void FillEllipse(const CD2DEllipse& ellipse);
    void FillRoundedRectangle(const CD2DRoundedRect& rect);

    static CD2DColorF COLORREF_TO_D2DCOLOR(COLORREF color, int alpha = 255);
    CD2DSizeF GetDpi() const;
    void SetDpi(const CD2DSizeF& dpi);
    CD2DSizeF GetSize() const;
    void GetTags(unsigned __int64* pTag1, unsigned __int64* pTag2) const;
    void SetTags(unsigned __int64 tag1, unsigned __int64 tag2);

protected:
    char _rendertarget_padding[32];
};

class CDCRenderTarget : public CRenderTarget {
    DECLARE_DYNAMIC(CDCRenderTarget)
public:
    CDCRenderTarget();

    void Attach(void* pRenderTarget);
    void* Detach();
    int Create(const void* pRenderTargetProperties);
    int BindDC(const CDC& dc, const CRect& rect);
};

class CAnimationVariable : public CObject {
    DECLARE_DYNAMIC(CAnimationVariable)
public:
    explicit CAnimationVariable(double defaultValue = 0.0);
    virtual ~CAnimationVariable();

    int Create(void* pAnimationManager);
    void AddTransition(void* pTransition);
    void ClearTransitions(int bAutodestroy = TRUE);
    void EnableValueChangedEvent(void* pController, int bEnable);
    void EnableIntegerValueChangedEvent(void* pController, int bEnable);
    void SetDefaultValue(double value);
    long GetValue(double& value);
    long GetValue(int& value);
    int CreateTransitions(void* pTransitionLibrary, void* pTransitionFactory);
    void ApplyTransitions(void* pController, void* pStoryboard, int bAutodestroy = TRUE);

protected:
    char _animationvariable_padding[32];
};
