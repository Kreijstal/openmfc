# OpenMFC Stub Coverage

This directory is generated from `tools/stub_coverage.py`.
It is the work ledger for manifest-scale stub reduction.

Base commit: `788edaf90f6b`
Total exports: `14103`
Real implementations: `8031`
Skip-listed: `146`
Remaining stubs assigned to shards: `5926`
Shard count: `32`

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
| [`feature-pack-ui-01`](shards/feature-pack-ui-01.json) | `feature_pack_ui` | 200 | feature_pack_ui shard 01/17: CDockablePane, CBasePane, CDockSite |
| [`feature-pack-ui-02`](shards/feature-pack-ui-02.json) | `feature_pack_ui` | 200 | feature_pack_ui shard 02/17: CFrameImpl, CDockingPanesRow, CFrameWndEx |
| [`feature-pack-ui-03`](shards/feature-pack-ui-03.json) | `feature_pack_ui` | 200 | feature_pack_ui shard 03/17: CMDIFrameWndEx, CMDIClientAreaWnd, CMFCDesktopAlertWnd |
| [`feature-pack-ui-04`](shards/feature-pack-ui-04.json) | `feature_pack_ui` | 200 | feature_pack_ui shard 04/17: CMFCEditBrowseCtrl, CMFCHeaderCtrl, CMFCKeyMapDialog |
| [`feature-pack-ui-05`](shards/feature-pack-ui-05.json) | `feature_pack_ui` | 200 | feature_pack_ui shard 05/17: CMFCPopupMenu, CMFCPopupMenuBar, CMFCMenuBar |
| [`feature-pack-ui-06`](shards/feature-pack-ui-06.json) | `feature_pack_ui` | 200 | feature_pack_ui shard 06/17: CMFCPropertyGridCtrl, CMFCRibbonBar, CMFCReBar |
| [`feature-pack-ui-07`](shards/feature-pack-ui-07.json) | `feature_pack_ui` | 200 | feature_pack_ui shard 07/17: CMFCRibbonCategory, CMFCRibbonButtonsGroup, CMFCRibbonComboBox |
| [`feature-pack-ui-08`](shards/feature-pack-ui-08.json) | `feature_pack_ui` | 200 | feature_pack_ui shard 08/17: CMFCRibbonGallery, CMFCRibbonEdit, CMFCRibbonConstructor |
| [`feature-pack-ui-09`](shards/feature-pack-ui-09.json) | `feature_pack_ui` | 200 | feature_pack_ui shard 09/17: CMFCRibbonPanel, CMFCRibbonPanelMenuBar, CMFCRibbonQuickAccessToolBar |
| [`feature-pack-ui-10`](shards/feature-pack-ui-10.json) | `feature_pack_ui` | 200 | feature_pack_ui shard 10/17: CMFCStatusBar, CMFCShellListCtrl, CMFCShellTreeCtrl |
| [`feature-pack-ui-11`](shards/feature-pack-ui-11.json) | `feature_pack_ui` | 200 | feature_pack_ui shard 11/17: CMFCToolBar, CMFCTasksPane, CMFCTabCtrl |
| [`feature-pack-ui-12`](shards/feature-pack-ui-12.json) | `feature_pack_ui` | 200 | feature_pack_ui shard 12/17: CMFCToolBar, CMFCToolBarDateTimeCtrl, CMFCToolBarComboBoxButton |
| [`feature-pack-ui-13`](shards/feature-pack-ui-13.json) | `feature_pack_ui` | 200 | feature_pack_ui shard 13/17: CMFCToolBarsCustomizeDialog, CMFCToolBarMenuButton, CMFCToolBarsKeyboardPropertyPage |
| [`feature-pack-ui-14`](shards/feature-pack-ui-14.json) | `feature_pack_ui` | 200 | feature_pack_ui shard 14/17: CMFCVisualManagerOffice2007, CMFCVisualManagerOffice2003, CMFCVisualManager |
| [`feature-pack-ui-15`](shards/feature-pack-ui-15.json) | `feature_pack_ui` | 200 | feature_pack_ui shard 15/17: CMFCVisualManagerOfficeXP, CMFCVisualManagerWindows, CMFCVisualManagerWindows7 |
| [`feature-pack-ui-16`](shards/feature-pack-ui-16.json) | `feature_pack_ui` | 200 | feature_pack_ui shard 16/17: CPane, CPaneContainer, CPaneContainerManager |
| [`feature-pack-ui-17`](shards/feature-pack-ui-17.json) | `feature_pack_ui` | 99 | feature_pack_ui shard 17/17: CPaneFrameWnd, CPaneDivider, CPaneContainerManager |
| [`gdi-drawing-01`](shards/gdi-drawing-01.json) | `gdi_drawing` | 136 | gdi_drawing shard 01/01: CRichEditView, CDrawingManager, CRenderTarget |
| [`global-residuals-01`](shards/global-residuals-01.json) | `global_residuals` | 200 | global_residuals shard 01/05: global |
| [`global-residuals-02`](shards/global-residuals-02.json) | `global_residuals` | 200 | global_residuals shard 02/05: global |
| [`global-residuals-03`](shards/global-residuals-03.json) | `global_residuals` | 200 | global_residuals shard 03/05: global |
| [`global-residuals-04`](shards/global-residuals-04.json) | `global_residuals` | 200 | global_residuals shard 04/05: global |
| [`global-residuals-05`](shards/global-residuals-05.json) | `global_residuals` | 138 | global_residuals shard 05/05: global |
| [`ole-com-activex-01`](shards/ole-com-activex-01.json) | `ole_com_activex` | 200 | ole_com_activex shard 01/04: COleControl, CCmdTarget, COleClientItem |
| [`ole-com-activex-02`](shards/ole-com-activex-02.json) | `ole_com_activex` | 200 | ole_com_activex shard 02/04: COleControl, COleDocIPFrameWndEx, COleControlSite |
| [`ole-com-activex-03`](shards/ole-com-activex-03.json) | `ole_com_activex` | 200 | ole_com_activex shard 03/04: COleIPFrameWndEx, COlePropertyPage, COleDocument |
| [`ole-com-activex-04`](shards/ole-com-activex-04.json) | `ole_com_activex` | 50 | ole_com_activex shard 04/04: COleServerItem, COleUILinkInfo, COleServerDocEx |
| [`other-mfc-01`](shards/other-mfc-01.json) | `other_mfc` | 200 | other_mfc shard 01/05: CAsyncMonikerFile, CBrowserControlSite, AFX_GLOBAL_DATA |
| [`other-mfc-02`](shards/other-mfc-02.json) | `other_mfc` | 200 | other_mfc shard 02/05: CDHtmlDialog, CDockBar, CDockContext |
| [`other-mfc-03`](shards/other-mfc-03.json) | `other_mfc` | 200 | other_mfc shard 03/05: CHtmlView, CMonthCalCtrl, CJumpList |
| [`other-mfc-04`](shards/other-mfc-04.json) | `other_mfc` | 200 | other_mfc shard 04/05: CPropertySet, CPropertySection, CRenderTarget |
| [`other-mfc-05`](shards/other-mfc-05.json) | `other_mfc` | 103 | other_mfc shard 05/05: CSplitterWnd, CStatusBar, CSmartDockingManager |
