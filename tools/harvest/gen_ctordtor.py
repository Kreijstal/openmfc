#!/usr/bin/env python3
"""Generate real default-ctor/dtor placement-new thunks for header-defined
constructible classes.  Emits phase4/src/global_ctordtor_placement.cpp.

Usage: python3 tools/harvest/gen_ctordtor.py
Edit the EXCLUDE_IMPL set below to drop thunks whose class fails to compile.
"""
import sys, os, re
ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..', '..'))
sys.path.insert(0, os.path.join(ROOT, 'tools'))
import stub_coverage as s
import gen_weak_stubs as g

# Per-impl exclusion: impl_ function names whose thunk does NOT compile against the
# repo headers (incomplete/forward-declared type, abstract class, inaccessible or
# deleted default ctor/dtor, or name clash).  To refresh: clear this set, run the
# generator, compile the emitted .cpp with the mingw command in the module docstring,
# collect the "In function <impl_...>" names from the errors, add them here, repeat
# until the file compiles clean.
EXCLUDE_IMPL = set([
    # link-only failure (ctor declared but not defined in the DLL): caught by full build
    'impl___0CMFCRibbonContextCaption__IEAA_XZ',
    'impl___0AFX_EXCEPTION_LINK__QEAA_XZ', 'impl___0AFX_GLOBAL_DATA__QEAA_XZ', 'impl___0AFX_MODULE_THREAD_STATE__QEAA_XZ', 'impl___0CAnimationBaseObject__QEAA_XZ',
    'impl___0CAnimationColor__QEAA_XZ', 'impl___0CAnimationController__QEAA_XZ', 'impl___0CAnimationPoint__QEAA_XZ', 'impl___0CAnimationRect__QEAA_XZ',
    'impl___0CAnimationSize__QEAA_XZ', 'impl___0CAnimationValue__QEAA_XZ', 'impl___0CAppDestinations__QEAA_XZ', 'impl___0CAsyncMonikerFile__QEAA_XZ',
    'impl___0CAutoHideDockSite__QEAA_XZ', 'impl___0CChevronOwnerDrawMenu__QEAA_XZ', 'impl___0CCommandManager__IEAA_XZ', 'impl___0CConnectionPoint__QEAA_XZ',
    'impl___0CControlBarInfo__QEAA_XZ', 'impl___0CControlCreationInfo__QEAA_XZ', 'impl___0CControlSiteFactoryMgr__QEAA_XZ', 'impl___0CD2DBrushProperties__QEAA_XZ',
    'impl___0CDBVariant__QEAA_XZ', 'impl___0CDHtmlControlSink__QEAA_XZ', 'impl___0CDockSite__QEAA_XZ', 'impl___0CDockState__QEAA_XZ',
    'impl___0CDockablePaneAdapter__QEAA_XZ', 'impl___0CGestureConfig__QEAA_XZ', 'impl___0CHtmlEditCtrl__QEAA_XZ', 'impl___0CHtmlEditDoc__QEAA_XZ',
    'impl___0CHtmlEditView__QEAA_XZ', 'impl___0CMDITabInfo__QEAA_XZ', 'impl___0CMDITabProxyWnd__QEAA_XZ', 'impl___0CMFCAcceleratorKeyAssignCtrl__QEAA_XZ',
    'impl___0CMFCAcceleratorKey__QEAA_XZ', 'impl___0CMFCBaseAccessibleObject__QEAA_XZ', 'impl___0CMFCBaseToolBar__QEAA_XZ', 'impl___0CMFCBaseVisualManager__QEAA_XZ',
    'impl___0CMFCCaptionBar__QEAA_XZ', 'impl___0CMFCCaptionMenuButton__QEAA_XZ', 'impl___0CMFCCmdUsageCount__QEAA_XZ', 'impl___0CMFCColorBarCmdUI__QEAA_XZ',
    'impl___0CMFCColorBar__QEAA_XZ', 'impl___0CMFCColorButton__QEAA_XZ', 'impl___0CMFCColorMenuButton__QEAA_XZ', 'impl___0CMFCColorPickerCtrl__QEAA_XZ',
    'impl___0CMFCCustomColorsPropertyPage__QEAA_XZ', 'impl___0CMFCCustomizeButton__QEAA_XZ', 'impl___0CMFCCustomizeMenuButton__QEAA_XZ', 'impl___0CMFCDesktopAlertDialog__QEAA_XZ',
    'impl___0CMFCDragFrameImpl__QEAA_XZ', 'impl___0CMFCDropDownFrame__QEAA_XZ', 'impl___0CMFCDropDownListBox__QEAA_XZ', 'impl___0CMFCDropDownToolbarButton__QEAA_XZ',
    'impl___0CMFCDynamicLayout__QEAA_XZ', 'impl___0CMFCEditBrowseCtrl__QEAA_XZ', 'impl___0CMFCFilterChunkValueImpl__QEAA_XZ', 'impl___0CMFCFontComboBox__QEAA_XZ',
    'impl___0CMFCLinkCtrl__QEAA_XZ', 'impl___0CMFCListCtrl__QEAA_XZ', 'impl___0CMFCMaskedEdit__QEAA_XZ', 'impl___0CMFCMenuButton__QEAA_XZ',
    'impl___0CMFCMousePropertyPage__QEAA_XZ', 'impl___0CMFCOutlookBarPaneAdapter__QEAA_XZ', 'impl___0CMFCOutlookBarPaneButton__QEAA_XZ', 'impl___0CMFCOutlookBarPane__QEAA_XZ',
    'impl___0CMFCOutlookBarTabCtrl__QEAA_XZ', 'impl___0CMFCOutlookBar__QEAA_XZ', 'impl___0CMFCPreviewCtrlImpl__QEAA_XZ', 'impl___0CMFCPropertyGridToolTipCtrl__QEAA_XZ',
    'impl___0CMFCPropertyPage__QEAA_XZ', 'impl___0CMFCPropertySheetListBox__AEAA_XZ', 'impl___0CMFCPropertySheetTabCtrl__AEAA_XZ', 'impl___0CMFCPropertySheet__QEAA_XZ',
    'impl___0CMFCReBar__QEAA_XZ', 'impl___0CMFCRibbonCmdUI__QEAA_XZ', 'impl___0CMFCRibbonColorButton__QEAA_XZ', 'impl___0CMFCRibbonComboBox__IEAA_XZ',
    'impl___0CMFCRibbonFontComboBox__IEAA_XZ', 'impl___0CMFCRibbonGalleryMenuButton__IEAA_XZ', 'impl___0CMFCRibbonInfo__QEAA_XZ', 'impl___0CMFCRibbonLaunchButton__QEAA_XZ',
    'impl___0CMFCRibbonLinkCtrl__QEAA_XZ', 'impl___0CMFCRibbonMainPanelButton__QEAA_XZ', 'impl___0CMFCRibbonMiniToolBar__QEAA_XZ', 'impl___0CMFCRibbonPanelMenuBar__IEAA_XZ',
    'impl___0CMFCRibbonPanelMenu__IEAA_XZ', 'impl___0CMFCRibbonQuickAccessToolBarDefaultState__QEAA_XZ', 'impl___0CMFCRibbonQuickAccessToolBar__QEAA_XZ', 'impl___0CMFCRibbonTabsGroup__QEAA_XZ',
    'impl___0CMFCRibbonUndoButton__QEAA_XZ', 'impl___0CMFCShadowRenderer__QEAA_XZ', 'impl___0CMFCShellListCtrl__QEAA_XZ', 'impl___0CMFCShellTreeCtrl__QEAA_XZ',
    'impl___0CMFCShowAllButton__QEAA_XZ', 'impl___0CMFCSpinButtonCtrl__QEAA_XZ', 'impl___0CMFCStandardColorsPropertyPage__QEAA_XZ', 'impl___0CMFCTasksPaneFrameWnd__QEAA_XZ',
    'impl___0CMFCToolBarButtonsListButton__QEAA_XZ', 'impl___0CMFCToolBarDateTimeCtrl__QEAA_XZ', 'impl___0CMFCToolBarDropSource__QEAA_XZ', 'impl___0CMFCToolBarDropTarget__QEAA_XZ',
    'impl___0CMFCToolBarFontComboBox__IEAA_XZ', 'impl___0CMFCToolBarFontSizeComboBox__IEAA_XZ', 'impl___0CMFCToolBarInfo__QEAA_XZ', 'impl___0CMFCToolBarSpinEditBoxButton__QEAA_XZ',
    'impl___0CMFCToolBarSystemMenuButton__QEAA_XZ', 'impl___0CMFCToolBarsCommandsListBox__QEAA_XZ', 'impl___0CMFCToolBarsCommandsPropertyPage__QEAA_XZ', 'impl___0CMFCToolBarsListCheckBox__QEAA_XZ',
    'impl___0CMFCToolBarsToolsPropertyPage__QEAA_XZ', 'impl___0CMFCVisualManagerBitmapCache__QEAA_XZ', 'impl___0CMFCVisualManagerVS2008__QEAA_XZ', 'impl___0CMFCZoomKernel__QEAA_XZ',
    'impl___0CMiniDockFrameWnd__QEAA_XZ', 'impl___0CMouseManager__QEAA_XZ', 'impl___0CMultiPageDHtmlDialog__QEAA_XZ', 'impl___0COleDocIPFrameWndEx__IEAA_XZ',
    'impl___0COleDocIPFrameWnd__QEAA_XZ', 'impl___0COleIPFrameWndEx__IEAA_XZ', 'impl___0COleServerDocEx__IEAA_XZ', 'impl___0CPaneDialog__QEAA_XZ',
    'impl___0CPngImage__QEAA_XZ', 'impl___0CPreviewDC__QEAA_XZ', 'impl___0CPreviewViewEx__IEAA_XZ', 'impl___0CPreviewView__QEAA_XZ',
    'impl___0CPrintInfo__QEAA_XZ', 'impl___0CPrintPreviewState__QEAA_XZ', 'impl___0CPropertySection__QEAA_XZ', 'impl___0CPropertySet__QEAA_XZ',
    'impl___0CProperty__QEAA_XZ', 'impl___0CReBar__QEAA_XZ', 'impl___0CReObject__QEAA_XZ', 'impl___0CRecentPaneContainerInfo__QEAA_XZ',
    'impl___0CResetPropExchange__QEAA_XZ', 'impl___0CRibbonCategoryScroll__AEAA_XZ', 'impl___0CRichEditDoc__IEAA_XZ', 'impl___0CScreenWnd__QEAA_XZ',
    'impl___0CSmartDockingGroupGuide__QEAA_XZ', 'impl___0CSmartDockingGroupGuidesManager__QEAA_XZ', 'impl___0CSmartDockingGroupGuidesWnd__IEAA_XZ', 'impl___0CSmartDockingHighlighterWnd__QEAA_XZ',
    'impl___0CSmartDockingInfo__QEAA_XZ', 'impl___0CSmartDockingManager__QEAA_XZ', 'impl___0CSmartDockingStandaloneGuideWnd__QEAA_XZ', 'impl___0CSmartDockingStandaloneGuide__QEAA_XZ',
    'impl___0CSplitterWndEx__QEAA_XZ', 'impl___0CTestCmdUI__QEAA_XZ', 'impl___0CUserTool__QEAA_XZ', 'impl___0CUserToolsManager__QEAA_XZ',
    'impl___1AFX_GLOBAL_DATA__QEAA_XZ', 'impl___1AFX_MAINTAIN_STATE2__QEAA_XZ', 'impl___1AFX_MAINTAIN_STATE__QEAA_XZ', 'impl___1AFX_MODULE_STATE__UEAA_XZ',
    'impl___1AFX_MODULE_THREAD_STATE__UEAA_XZ', 'impl___1CAnimateCtrl__UEAA_XZ', 'impl___1CAnimationBaseObject__UEAA_XZ', 'impl___1CAnimationController__UEAA_XZ',
    'impl___1CAnimationGroup__QEAA_XZ', 'impl___1CAsyncMonikerFile__UEAA_XZ', 'impl___1CAutoHideDockSite__UEAA_XZ', 'impl___1CBaseTabbedPane__UEAA_XZ',
    'impl___1CComboBoxEx__UEAA_XZ', 'impl___1CCommandManager__UEAA_XZ', 'impl___1CConnectionPoint__UEAA_XZ', 'impl___1CControlSiteFactoryMgr__UEAA_XZ',
    'impl___1CCtrlView__UEAA_XZ', 'impl___1CD2DBitmapBrush__UEAA_XZ', 'impl___1CD2DBitmap__UEAA_XZ', 'impl___1CD2DBrush__MEAA_XZ',
    'impl___1CD2DGeometrySink__UEAA_XZ', 'impl___1CD2DGeometry__UEAA_XZ', 'impl___1CD2DGradientBrush__UEAA_XZ', 'impl___1CD2DLayer__UEAA_XZ',
    'impl___1CD2DLinearGradientBrush__UEAA_XZ', 'impl___1CD2DMesh__UEAA_XZ', 'impl___1CD2DRadialGradientBrush__UEAA_XZ', 'impl___1CD2DResource__MEAA_XZ',
    'impl___1CD2DSolidColorBrush__UEAA_XZ', 'impl___1CDBVariant__UEAA_XZ', 'impl___1CDHtmlControlSink__UEAA_XZ', 'impl___1CDataRecoveryHandler__UEAA_XZ',
    'impl___1CDataSourceControl__UEAA_XZ', 'impl___1CDateTimeCtrl__UEAA_XZ', 'impl___1CDialogImpl__MEAA_XZ', 'impl___1CDocObjectServerItem__UEAA_XZ',
    'impl___1CDocObjectServer__UEAA_XZ', 'impl___1CDockBar__UEAA_XZ', 'impl___1CDockContext__UEAA_XZ', 'impl___1CDockSite__UEAA_XZ',
    'impl___1CDockState__UEAA_XZ', 'impl___1CDockablePaneAdapter__UEAA_XZ', 'impl___1CDockingPanesRow__UEAA_XZ', 'impl___1CDrawingManager__UEAA_XZ',
    'impl___1CDynLinkLibrary__UEAA_XZ', 'impl___1CEnumArray__UEAA_XZ', 'impl___1CEnumConnPoints__UEAA_XZ', 'impl___1CEnumConnections__UEAA_XZ',
    'impl___1CEnumUnknown__UEAA_XZ', 'impl___1CFolderPickerDialog__UEAA_XZ', 'impl___1CFullScreenImpl__UEAA_XZ', 'impl___1CGestureConfig__UEAA_XZ',
    'impl___1CGopherFileFind__UEAA_XZ', 'impl___1CHeaderCtrl__UEAA_XZ', 'impl___1CHotKeyCtrl__UEAA_XZ', 'impl___1CHtmlControlSite__UEAA_XZ',
    'impl___1CHtmlEditCtrl__UEAA_XZ', 'impl___1CHtmlEditDoc__UEAA_XZ', 'impl___1CHtmlEditView__UEAA_XZ', 'impl___1CIPAddressCtrl__UEAA_XZ',
    'impl___1CJumpList__QEAA_XZ', 'impl___1CMDITabProxyWnd__UEAA_XZ', 'impl___1CMFCAcceleratorKeyAssignCtrl__UEAA_XZ', 'impl___1CMFCAcceleratorKey__UEAA_XZ',
    'impl___1CMFCBaseAccessibleObject__UEAA_XZ', 'impl___1CMFCBaseToolBar__UEAA_XZ', 'impl___1CMFCBaseVisualManager__UEAA_XZ', 'impl___1CMFCCaptionBar__UEAA_XZ',
    'impl___1CMFCCaptionButtonEx__UEAA_XZ', 'impl___1CMFCCaptionMenuButton__UEAA_XZ', 'impl___1CMFCCmdUsageCount__UEAA_XZ', 'impl___1CMFCColorBar__UEAA_XZ',
    'impl___1CMFCColorButton__UEAA_XZ', 'impl___1CMFCColorDialog__UEAA_XZ', 'impl___1CMFCColorMenuButton__UEAA_XZ', 'impl___1CMFCColorPickerCtrl__UEAA_XZ',
    'impl___1CMFCColorPopupMenu__UEAA_XZ', 'impl___1CMFCColorPropertySheet__UEAA_XZ', 'impl___1CMFCControlBarImpl__UEAA_XZ', 'impl___1CMFCControlContainer__UEAA_XZ',
    'impl___1CMFCCustomizeButton__UEAA_XZ', 'impl___1CMFCCustomizeMenuButton__UEAA_XZ', 'impl___1CMFCDesktopAlertDialog__UEAA_XZ', 'impl___1CMFCDragFrameImpl__UEAA_XZ',
    'impl___1CMFCDropDownFrame__UEAA_XZ', 'impl___1CMFCDropDownListBox__UEAA_XZ', 'impl___1CMFCDropDownToolbarButton__UEAA_XZ', 'impl___1CMFCDynamicLayout__UEAA_XZ',
    'impl___1CMFCEditBrowseCtrl__UEAA_XZ', 'impl___1CMFCFilterChunkValueImpl__UEAA_XZ', 'impl___1CMFCFontComboBox__UEAA_XZ', 'impl___1CMFCImagePaintArea__UEAA_XZ',
    'impl___1CMFCKeyMapDialog__UEAA_XZ', 'impl___1CMFCLinkCtrl__UEAA_XZ', 'impl___1CMFCListCtrl__UEAA_XZ', 'impl___1CMFCMaskedEdit__UEAA_XZ',
    'impl___1CMFCMenuButton__UEAA_XZ', 'impl___1CMFCMousePropertyPage__UEAA_XZ', 'impl___1CMFCOutlookBarPaneAdapter__UEAA_XZ', 'impl___1CMFCOutlookBarPaneButton__UEAA_XZ',
    'impl___1CMFCOutlookBarPane__UEAA_XZ', 'impl___1CMFCOutlookBarTabCtrl__UEAA_XZ', 'impl___1CMFCOutlookBar__UEAA_XZ', 'impl___1CMFCPreviewCtrlImpl__UEAA_XZ',
    'impl___1CMFCPropertyGridColorProperty__UEAA_XZ', 'impl___1CMFCPropertyGridFileProperty__UEAA_XZ', 'impl___1CMFCPropertyGridFontProperty__UEAA_XZ', 'impl___1CMFCPropertyGridToolTipCtrl__UEAA_XZ',
    'impl___1CMFCPropertyPage__UEAA_XZ', 'impl___1CMFCPropertySheetCategoryInfo__UEAA_XZ', 'impl___1CMFCPropertySheet__UEAA_XZ', 'impl___1CMFCRibbonCollector__UEAA_XZ',
    'impl___1CMFCRibbonColorButton__UEAA_XZ', 'impl___1CMFCRibbonComboBox__UEAA_XZ', 'impl___1CMFCRibbonCommandsListBox__UEAA_XZ', 'impl___1CMFCRibbonConstructor__UEAA_XZ',
    'impl___1CMFCRibbonCustomizeDialog__UEAA_XZ', 'impl___1CMFCRibbonCustomizePropertyPage__UEAA_XZ', 'impl___1CMFCRibbonFontComboBox__UEAA_XZ', 'impl___1CMFCRibbonGalleryMenuButton__UEAA_XZ',
    'impl___1CMFCRibbonInfoLoader__UEAA_XZ', 'impl___1CMFCRibbonInfo__UEAA_XZ', 'impl___1CMFCRibbonKeyTip__UEAA_XZ', 'impl___1CMFCRibbonKeyboardCustomizeDialog__UEAA_XZ',
    'impl___1CMFCRibbonLinkCtrl__UEAA_XZ', 'impl___1CMFCRibbonMainPanelButton__UEAA_XZ', 'impl___1CMFCRibbonMiniToolBar__UEAA_XZ', 'impl___1CMFCRibbonPanelMenuBar__UEAA_XZ',
    'impl___1CMFCRibbonPanelMenu__UEAA_XZ', 'impl___1CMFCRibbonQuickAccessToolBar__UEAA_XZ', 'impl___1CMFCRibbonRichEditCtrl__UEAA_XZ', 'impl___1CMFCRibbonTabsGroup__UEAA_XZ',
    'impl___1CMFCRibbonUndoButton__UEAA_XZ', 'impl___1CMFCShadowRenderer__UEAA_XZ', 'impl___1CMFCShellListCtrl__UEAA_XZ', 'impl___1CMFCShellTreeCtrl__UEAA_XZ',
    'impl___1CMFCShowAllButton__UEAA_XZ', 'impl___1CMFCSpinButtonCtrl__UEAA_XZ', 'impl___1CMFCTasksPaneFrameWnd__UEAA_XZ', 'impl___1CMFCToolBarButtonCustomizeDialog__UEAA_XZ',
    'impl___1CMFCToolBarButtonsListButton__UEAA_XZ', 'impl___1CMFCToolBarComboBoxEdit__UEAA_XZ', 'impl___1CMFCToolBarDateTimeCtrl__UEAA_XZ', 'impl___1CMFCToolBarDropSource__UEAA_XZ',
    'impl___1CMFCToolBarDropTarget__UEAA_XZ', 'impl___1CMFCToolBarEditCtrl__UEAA_XZ', 'impl___1CMFCToolBarFontComboBox__UEAA_XZ', 'impl___1CMFCToolBarFontSizeComboBox__UEAA_XZ',
    'impl___1CMFCToolBarSpinEditBoxButton__UEAA_XZ', 'impl___1CMFCToolBarSystemMenuButton__UEAA_XZ', 'impl___1CMFCToolBarsCommandsListBox__UEAA_XZ', 'impl___1CMFCToolBarsCommandsPropertyPage__UEAA_XZ',
    'impl___1CMFCToolBarsCustomizeDialog__UEAA_XZ', 'impl___1CMFCToolBarsKeyboardPropertyPage__UEAA_XZ', 'impl___1CMFCToolBarsListCheckBox__UEAA_XZ', 'impl___1CMFCToolBarsListPropertyPage__UEAA_XZ',
    'impl___1CMFCToolBarsMenuPropertyPage__UEAA_XZ', 'impl___1CMFCToolBarsOptionsPropertyPage__UEAA_XZ', 'impl___1CMFCToolBarsToolsPropertyPage__UEAA_XZ', 'impl___1CMFCToolTipCtrl__UEAA_XZ',
    'impl___1CMFCVisualManagerBitmapCache__UEAA_XZ', 'impl___1CMFCVisualManagerVS2008__UEAA_XZ', 'impl___1CMFCZoomKernel__UEAA_XZ', 'impl___1CMemDC__UEAA_XZ',
    'impl___1CMonikerFile__UEAA_XZ', 'impl___1CMonthCalCtrl__UEAA_XZ', 'impl___1CMouseManager__UEAA_XZ', 'impl___1CMultiPageDHtmlDialog__UEAA_XZ',
    'impl___1COleCntrFrameWndEx__MEAA_XZ', 'impl___1COleCntrFrameWnd__UEAA_XZ', 'impl___1COleControlLock__UEAA_XZ', 'impl___1COleDocIPFrameWndEx__MEAA_XZ',
    'impl___1COleDocIPFrameWnd__UEAA_XZ', 'impl___1COleIPFrameWndEx__MEAA_XZ', 'impl___1COleServerDocEx__UEAA_XZ', 'impl___1CPaneDialog__UEAA_XZ',
    'impl___1CPngImage__UEAA_XZ', 'impl___1CPreviewDC__UEAA_XZ', 'impl___1CPreviewViewEx__MEAA_XZ', 'impl___1CPreviewView__UEAA_XZ',
    'impl___1CPrintInfo__QEAA_XZ', 'impl___1CProcessLocalObject__QEAA_XZ', 'impl___1CPropbagPropExchange__UEAA_XZ', 'impl___1CPropertySection__QEAA_XZ',
    'impl___1CPropertySet__QEAA_XZ', 'impl___1CProperty__QEAA_XZ', 'impl___1CReBarCtrl__UEAA_XZ', 'impl___1CReObject__QEAA_XZ',
    'impl___1CRecentDockSiteInfo__UEAA_XZ', 'impl___1CRecentPaneContainerInfo__UEAA_XZ', 'impl___1CRectTracker__UEAA_XZ', 'impl___1CRichEditCntrItem__UEAA_XZ',
    'impl___1CScreenWnd__UEAA_XZ', 'impl___1CSmartDockingGroupGuide__UEAA_XZ', 'impl___1CSmartDockingGroupGuidesManager__UEAA_XZ', 'impl___1CSmartDockingHighlighterWnd__UEAA_XZ',
    'impl___1CSmartDockingManager__UEAA_XZ', 'impl___1CSmartDockingStandaloneGuideWnd__UEAA_XZ', 'impl___1CSmartDockingStandaloneGuide__UEAA_XZ', 'impl___1CSplitterWndEx__UEAA_XZ',
    'impl___1CStatusBarCtrl__UEAA_XZ', 'impl___1CTabbedPane__UEAA_XZ', 'impl___1CThreadLocalObject__QEAA_XZ', 'impl___1CToolBarCtrl__UEAA_XZ',
    'impl___1CUserTool__UEAA_XZ', 'impl___1CUserToolsManager__UEAA_XZ',
])

PREAMBLE = '''// AUTO-GENERATED by tools/harvest/gen_ctordtor.py -- real default-ctor/dtor thunks
// for header-defined constructible classes.
#include "openmfc/afxwin.h"
#include "openmfc/afxmfc.h"
#include "openmfc/afxole.h"
#include "openmfc/afxdisp.h"
#include "openmfc/afxdao.h"
#include "openmfc/afxdb.h"
#include "openmfc/afxinet.h"
#include "openmfc/afxsock.h"
#include <new>
#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif
'''

CTOR_RE = re.compile(r'^\?\?0([A-Za-z][A-Za-z0-9_]*)@@[A-Z]EAA@XZ$')
DTOR_RE = re.compile(r'^\?\?1([A-Za-z][A-Za-z0-9_]*)@@[A-Z]EAA@XZ$')


def collect():
    m, _ = s.load_current_manifest()
    ctors = []  # (cls, symbol)
    dtors = []
    for e in m['stub']:
        x = e['symbol']
        mc = CTOR_RE.match(x)
        md = DTOR_RE.match(x)
        if mc:
            ctors.append((mc.group(1), x))
        elif md:
            dtors.append((md.group(1), x))
    return ctors, dtors


def main():
    # Remove our own output first so neither the manifest scan (collect) nor the
    # "existing" scan below ever sees it -> idempotent regeneration.
    out = os.path.join(ROOT, 'phase4', 'src', 'global_ctordtor_placement.cpp')
    if os.path.exists(out):
        os.remove(out)
    ctors, dtors = collect()
    # load existing impl names from other files
    existing = set()
    for fn in os.listdir(os.path.join(ROOT, 'phase4', 'src')):
        if not fn.endswith('.cpp') or fn == 'global_ctordtor_placement.cpp':
            continue
        with open(os.path.join(ROOT, 'phase4', 'src', fn)) as f:
            for name in re.findall(r'impl_[A-Za-z0-9_]+', f.read()):
                existing.add(name)

    lines = [PREAMBLE]
    nc = nd = 0
    used = set()
    for cls, sym in ctors:
        impl = g.generate_stub_name(sym)
        if impl in EXCLUDE_IMPL or impl in existing or impl in used:
            continue
        used.add(impl)
        lines.append('// Symbol: %s' % sym)
        lines.append('extern "C" void* MS_ABI %s(void* pThis) { return new (pThis) %s(); }' % (impl, cls))
        nc += 1
    for cls, sym in dtors:
        impl = g.generate_stub_name(sym)
        if impl in EXCLUDE_IMPL or impl in existing or impl in used:
            continue
        used.add(impl)
        lines.append('// Symbol: %s' % sym)
        lines.append('extern "C" void MS_ABI %s(void* pThis) { static_cast<%s*>(pThis)->~%s(); }' % (impl, cls, cls))
        nd += 1

    with open(out, 'w') as f:
        f.write('\n'.join(lines) + '\n')
    print('ctors=%d dtors=%d total=%d -> %s' % (nc, nd, nc + nd, out))


if __name__ == '__main__':
    main()
