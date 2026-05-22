# OpenMFC Stub Coverage

This directory is generated from `tools/stub_coverage.py`.
It is the work ledger for manifest-scale stub reduction.

Base commit: `51f61b0b6b02`
Total exports: `14103`
Real implementations: `2492`
Skip-listed: `183`
Remaining stubs assigned to shards: `11428`
Shard count: `64`

Coverage invariant:

```
union(shard symbols) == current manifest stubs
intersection(any two shard symbol sets) == empty
```

Validate with:

```sh
python3 tools/stub_coverage.py validate
```

Track progress after shard merges with:

```sh
python3 tools/stub_coverage.py status --by-shard
```

Agent completion rule for a symbol:

- `real`: exact `// Symbol:` comment plus ABI-safe implementation that passes the phase4 build.
- `skip`: explicit skip-list entry with a reason for truly compiler-generated or impractical exports.
- `stub`: not completed.

Shards:

| Shard | Category | Symbols | Focus |
|---|---|---:|---|
| [`app-runtime-globals-01`](shards/app-runtime-globals-01.json) | `app_runtime_globals` | 113 | app_runtime_globals shard 01/01: CWinApp, CWinAppEx, CWinThread |
| [`collections-strings-01`](shards/collections-strings-01.json) | `collections_strings` | 85 | collections_strings shard 01/01: CTagManager, CFileDialog, CDHtmlDialog |
| [`core-windows-common-controls-01`](shards/core-windows-common-controls-01.json) | `core_windows_common_controls` | 200 | core_windows_common_controls shard 01/03: CFileDialog, CCheckListBox, CListCtrl |
| [`core-windows-common-controls-02`](shards/core-windows-common-controls-02.json) | `core_windows_common_controls` | 200 | core_windows_common_controls shard 02/03: CWnd, CMDIFrameWnd, CPropertySheet |
| [`core-windows-common-controls-03`](shards/core-windows-common-controls-03.json) | `core_windows_common_controls` | 2 | core_windows_common_controls shard 03/03: CWnd |
| [`database-dao-odbc-01`](shards/database-dao-odbc-01.json) | `database_dao_odbc` | 111 | database_dao_odbc shard 01/01: CRecordset, CDatabase, CDBException |
| [`docview-files-archive-01`](shards/docview-files-archive-01.json) | `docview_files_archive` | 200 | docview_files_archive shard 01/02: CEditView, CDocument, CScrollView |
| [`docview-files-archive-02`](shards/docview-files-archive-02.json) | `docview_files_archive` | 12 | docview_files_archive shard 02/02: CView |
| [`feature-pack-ui-01`](shards/feature-pack-ui-01.json) | `feature_pack_ui` | 200 | feature_pack_ui shard 01/27: CBasePane, CAnimationController, CBaseTabbedPane |
| [`feature-pack-ui-02`](shards/feature-pack-ui-02.json) | `feature_pack_ui` | 200 | feature_pack_ui shard 02/27: CDockablePane, CFrameImpl, CDockingPanesRow |
| [`feature-pack-ui-03`](shards/feature-pack-ui-03.json) | `feature_pack_ui` | 200 | feature_pack_ui shard 03/27: CMDIChildWndEx, CFrameWndEx, CMDIClientAreaWnd |
| [`feature-pack-ui-04`](shards/feature-pack-ui-04.json) | `feature_pack_ui` | 200 | feature_pack_ui shard 04/27: CMDIFrameWndEx, CMFCBaseTabCtrl, CMFCBaseAccessibleObject |
| [`feature-pack-ui-05`](shards/feature-pack-ui-05.json) | `feature_pack_ui` | 200 | feature_pack_ui shard 05/27: CMFCButton, CMFCBaseTabCtrl, CMFCCaptionBar |
| [`feature-pack-ui-06`](shards/feature-pack-ui-06.json) | `feature_pack_ui` | 200 | feature_pack_ui shard 06/27: CMFCColorBar, CMFCColorPickerCtrl, CMFCColorButton |
| [`feature-pack-ui-07`](shards/feature-pack-ui-07.json) | `feature_pack_ui` | 200 | feature_pack_ui shard 07/27: CMFCDesktopAlertWnd, CMFCEditBrowseCtrl, CMFCDynamicLayout |
| [`feature-pack-ui-08`](shards/feature-pack-ui-08.json) | `feature_pack_ui` | 200 | feature_pack_ui shard 08/27: CMFCMaskedEdit, CMFCHeaderCtrl, CMFCImageEditorDialog |
| [`feature-pack-ui-09`](shards/feature-pack-ui-09.json) | `feature_pack_ui` | 200 | feature_pack_ui shard 09/27: CMFCOutlookBarPane, CMFCOutlookBarTabCtrl, CMFCMenuBar |
| [`feature-pack-ui-10`](shards/feature-pack-ui-10.json) | `feature_pack_ui` | 200 | feature_pack_ui shard 10/27: CMFCPopupMenu, CMFCPopupMenuBar, CMFCPropertyGridCtrl |
| [`feature-pack-ui-11`](shards/feature-pack-ui-11.json) | `feature_pack_ui` | 200 | feature_pack_ui shard 11/27: CMFCPropertyGridCtrl, CMFCPropertyGridProperty, CMFCPropertySheet |
| [`feature-pack-ui-12`](shards/feature-pack-ui-12.json) | `feature_pack_ui` | 200 | feature_pack_ui shard 12/27: CMFCRibbonBar, CMFCRibbonBaseElement, CMFCReBar |
| [`feature-pack-ui-13`](shards/feature-pack-ui-13.json) | `feature_pack_ui` | 200 | feature_pack_ui shard 13/27: CMFCRibbonCategory, CMFCRibbonButton, CMFCRibbonButtonsGroup |
| [`feature-pack-ui-14`](shards/feature-pack-ui-14.json) | `feature_pack_ui` | 200 | feature_pack_ui shard 14/27: CMFCRibbonGallery, CMFCRibbonComboBox, CMFCRibbonEdit |
| [`feature-pack-ui-15`](shards/feature-pack-ui-15.json) | `feature_pack_ui` | 200 | feature_pack_ui shard 15/27: CMFCRibbonPanel, CMFCRibbonGalleryIcon, CMFCRibbonMainPanel |
| [`feature-pack-ui-16`](shards/feature-pack-ui-16.json) | `feature_pack_ui` | 200 | feature_pack_ui shard 16/27: CMFCRibbonPanelMenuBar, CMFCRibbonStatusBar, CMFCRibbonQuickAccessToolBar |
| [`feature-pack-ui-17`](shards/feature-pack-ui-17.json) | `feature_pack_ui` | 200 | feature_pack_ui shard 17/27: CMFCStatusBar, CMFCShellListCtrl, CMFCShellTreeCtrl |
| [`feature-pack-ui-18`](shards/feature-pack-ui-18.json) | `feature_pack_ui` | 200 | feature_pack_ui shard 18/27: CMFCTasksPane, CMFCTabCtrl, CMFCTasksPaneFrameWnd |
| [`feature-pack-ui-19`](shards/feature-pack-ui-19.json) | `feature_pack_ui` | 200 | feature_pack_ui shard 19/27: CMFCToolBar, CMFCTasksPaneToolBar, CMFCTasksPaneFrameWnd |
| [`feature-pack-ui-20`](shards/feature-pack-ui-20.json) | `feature_pack_ui` | 200 | feature_pack_ui shard 20/27: CMFCToolBar, CMFCToolBarDateTimeCtrl, CMFCToolBarComboBoxButton |
| [`feature-pack-ui-21`](shards/feature-pack-ui-21.json) | `feature_pack_ui` | 200 | feature_pack_ui shard 21/27: CMFCToolBarImages, CMFCToolBarEditBoxButton, CMFCToolBarMenuButton |
| [`feature-pack-ui-22`](shards/feature-pack-ui-22.json) | `feature_pack_ui` | 200 | feature_pack_ui shard 22/27: CMFCVisualManager, CMFCToolBarsCustomizeDialog, CMFCToolTipCtrl |
| [`feature-pack-ui-23`](shards/feature-pack-ui-23.json) | `feature_pack_ui` | 200 | feature_pack_ui shard 23/27: CMFCVisualManagerOffice2007, CMFCVisualManagerOffice2003, CMFCVisualManager |
| [`feature-pack-ui-24`](shards/feature-pack-ui-24.json) | `feature_pack_ui` | 200 | feature_pack_ui shard 24/27: CMFCVisualManagerOfficeXP, CMFCVisualManagerWindows, CMFCVisualManagerVS2008 |
| [`feature-pack-ui-25`](shards/feature-pack-ui-25.json) | `feature_pack_ui` | 200 | feature_pack_ui shard 25/27: CPane, CPaneContainer, CMultiPaneFrameWnd |
| [`feature-pack-ui-26`](shards/feature-pack-ui-26.json) | `feature_pack_ui` | 200 | feature_pack_ui shard 26/27: CPaneFrameWnd, CPaneContainerManager, CPaneDivider |
| [`feature-pack-ui-27`](shards/feature-pack-ui-27.json) | `feature_pack_ui` | 85 | feature_pack_ui shard 27/27: CTaskDialog, CTabbedPane, CShellManager |
| [`gdi-drawing-01`](shards/gdi-drawing-01.json) | `gdi_drawing` | 200 | gdi_drawing shard 01/02: CDrawingManager, CPreviewDC, CD2DBitmap |
| [`gdi-drawing-02`](shards/gdi-drawing-02.json) | `gdi_drawing` | 198 | gdi_drawing shard 02/02: CRichEditView, CPreviewView, CPreviewDC |
| [`global-residuals-01`](shards/global-residuals-01.json) | `global_residuals` | 200 | global_residuals shard 01/12: global |
| [`global-residuals-02`](shards/global-residuals-02.json) | `global_residuals` | 200 | global_residuals shard 02/12: global |
| [`global-residuals-03`](shards/global-residuals-03.json) | `global_residuals` | 200 | global_residuals shard 03/12: global |
| [`global-residuals-04`](shards/global-residuals-04.json) | `global_residuals` | 200 | global_residuals shard 04/12: global |
| [`global-residuals-05`](shards/global-residuals-05.json) | `global_residuals` | 200 | global_residuals shard 05/12: global |
| [`global-residuals-06`](shards/global-residuals-06.json) | `global_residuals` | 200 | global_residuals shard 06/12: global |
| [`global-residuals-07`](shards/global-residuals-07.json) | `global_residuals` | 200 | global_residuals shard 07/12: global |
| [`global-residuals-08`](shards/global-residuals-08.json) | `global_residuals` | 200 | global_residuals shard 08/12: global |
| [`global-residuals-09`](shards/global-residuals-09.json) | `global_residuals` | 200 | global_residuals shard 09/12: global |
| [`global-residuals-10`](shards/global-residuals-10.json) | `global_residuals` | 200 | global_residuals shard 10/12: global |
| [`global-residuals-11`](shards/global-residuals-11.json) | `global_residuals` | 200 | global_residuals shard 11/12: global |
| [`global-residuals-12`](shards/global-residuals-12.json) | `global_residuals` | 22 | global_residuals shard 12/12: global |
| [`internet-wininet-01`](shards/internet-wininet-01.json) | `internet_wininet` | 42 | internet_wininet shard 01/01: CGopherFileFind, CFileFind, CFtpFileFind |
| [`ole-com-activex-01`](shards/ole-com-activex-01.json) | `ole_com_activex` | 200 | ole_com_activex shard 01/06: COleClientItem, CCmdTarget, CDocObjectServer |
| [`ole-com-activex-02`](shards/ole-com-activex-02.json) | `ole_com_activex` | 200 | ole_com_activex shard 02/06: COleControl, COleCntrFrameWndEx |
| [`ole-com-activex-03`](shards/ole-com-activex-03.json) | `ole_com_activex` | 200 | ole_com_activex shard 03/06: COleDocIPFrameWndEx, COleControlSite, COleControlContainer |
| [`ole-com-activex-04`](shards/ole-com-activex-04.json) | `ole_com_activex` | 200 | ole_com_activex shard 04/06: COleIPFrameWndEx, COleDocument, COleIPFrameWnd |
| [`ole-com-activex-05`](shards/ole-com-activex-05.json) | `ole_com_activex` | 200 | ole_com_activex shard 05/06: COleServerDoc, COlePropertyPage, COleServerItem |
| [`ole-com-activex-06`](shards/ole-com-activex-06.json) | `ole_com_activex` | 21 | ole_com_activex shard 06/06: CRichEditDoc, CView, CRichEditView |
| [`other-mfc-01`](shards/other-mfc-01.json) | `other_mfc` | 200 | other_mfc shard 01/08: CAsyncMonikerFile, CBrowserControlSite, AFX_GLOBAL_DATA |
| [`other-mfc-02`](shards/other-mfc-02.json) | `other_mfc` | 200 | other_mfc shard 02/08: CControlBar, CDHtmlDialog, CD2DGeometry |
| [`other-mfc-03`](shards/other-mfc-03.json) | `other_mfc` | 200 | other_mfc shard 03/08: CDockBar, CDockContext, CDataRecoveryHandler |
| [`other-mfc-04`](shards/other-mfc-04.json) | `other_mfc` | 200 | other_mfc shard 04/08: CHtmlView, CMetaFileDC, CJumpList |
| [`other-mfc-05`](shards/other-mfc-05.json) | `other_mfc` | 200 | other_mfc shard 05/08: CMonthCalCtrl, CPrintDialogEx, COccManager |
| [`other-mfc-06`](shards/other-mfc-06.json) | `other_mfc` | 200 | other_mfc shard 06/08: CPropertySet, CSettingsStore, CRenderTarget |
| [`other-mfc-07`](shards/other-mfc-07.json) | `other_mfc` | 200 | other_mfc shard 07/08: CSplitterWnd, CStatusBar, CTabView |
| [`other-mfc-08`](shards/other-mfc-08.json) | `other_mfc` | 137 | other_mfc shard 08/08: CVSListBoxBase, CToolBar, CVSListBox |
