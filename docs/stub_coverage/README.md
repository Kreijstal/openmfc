# OpenMFC Stub Coverage

This directory is generated from `tools/stub_coverage.py`.
It is the work ledger for manifest-scale stub reduction.

Base commit: `223ee39cddc7`
Total exports: `14103`
Real implementations: `6720`
Skip-listed: `148`
Remaining stubs assigned to shards: `7235`
Shard count: `726`

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
| [`feature-pack-ui-01`](shards/feature-pack-ui-01.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 01/439: CAnimationController |
| [`feature-pack-ui-02`](shards/feature-pack-ui-02.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 02/439: CAnimationController |
| [`feature-pack-ui-03`](shards/feature-pack-ui-03.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 03/439: CAnimationController |
| [`feature-pack-ui-04`](shards/feature-pack-ui-04.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 04/439: CAnimationController |
| [`feature-pack-ui-05`](shards/feature-pack-ui-05.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 05/439: CBasePane |
| [`feature-pack-ui-06`](shards/feature-pack-ui-06.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 06/439: CBasePane |
| [`feature-pack-ui-07`](shards/feature-pack-ui-07.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 07/439: CBasePane |
| [`feature-pack-ui-08`](shards/feature-pack-ui-08.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 08/439: CBasePane |
| [`feature-pack-ui-09`](shards/feature-pack-ui-09.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 09/439: CBasePane |
| [`feature-pack-ui-10`](shards/feature-pack-ui-10.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 10/439: CBaseTabbedPane, CBasePane |
| [`feature-pack-ui-11`](shards/feature-pack-ui-11.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 11/439: CBaseTabbedPane |
| [`feature-pack-ui-12`](shards/feature-pack-ui-12.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 12/439: CBaseTabbedPane |
| [`feature-pack-ui-13`](shards/feature-pack-ui-13.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 13/439: CBaseTabbedPane, CDockSite |
| [`feature-pack-ui-14`](shards/feature-pack-ui-14.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 14/439: CDockSite |
| [`feature-pack-ui-15`](shards/feature-pack-ui-15.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 15/439: CDockSite |
| [`feature-pack-ui-16`](shards/feature-pack-ui-16.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 16/439: CDockSite, CDockablePane |
| [`feature-pack-ui-17`](shards/feature-pack-ui-17.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 17/439: CDockablePane |
| [`feature-pack-ui-18`](shards/feature-pack-ui-18.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 18/439: CDockablePane |
| [`feature-pack-ui-19`](shards/feature-pack-ui-19.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 19/439: CDockablePane |
| [`feature-pack-ui-20`](shards/feature-pack-ui-20.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 20/439: CDockablePane |
| [`feature-pack-ui-21`](shards/feature-pack-ui-21.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 21/439: CDockablePane |
| [`feature-pack-ui-22`](shards/feature-pack-ui-22.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 22/439: CDockablePane |
| [`feature-pack-ui-23`](shards/feature-pack-ui-23.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 23/439: CDockablePane |
| [`feature-pack-ui-24`](shards/feature-pack-ui-24.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 24/439: CDockablePane, CDockingManager |
| [`feature-pack-ui-25`](shards/feature-pack-ui-25.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 25/439: CDockingManager |
| [`feature-pack-ui-26`](shards/feature-pack-ui-26.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 26/439: CDockingManager, CDockingPanesRow |
| [`feature-pack-ui-27`](shards/feature-pack-ui-27.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 27/439: CDockingPanesRow |
| [`feature-pack-ui-28`](shards/feature-pack-ui-28.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 28/439: CDockingPanesRow |
| [`feature-pack-ui-29`](shards/feature-pack-ui-29.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 29/439: CDockingPanesRow |
| [`feature-pack-ui-30`](shards/feature-pack-ui-30.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 30/439: CDockingPanesRow |
| [`feature-pack-ui-31`](shards/feature-pack-ui-31.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 31/439: CDockingPanesRow, CFrameImpl |
| [`feature-pack-ui-32`](shards/feature-pack-ui-32.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 32/439: CFrameImpl |
| [`feature-pack-ui-33`](shards/feature-pack-ui-33.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 33/439: CFrameImpl |
| [`feature-pack-ui-34`](shards/feature-pack-ui-34.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 34/439: CFrameImpl |
| [`feature-pack-ui-35`](shards/feature-pack-ui-35.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 35/439: CFrameImpl |
| [`feature-pack-ui-36`](shards/feature-pack-ui-36.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 36/439: CFrameImpl |
| [`feature-pack-ui-37`](shards/feature-pack-ui-37.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 37/439: CFrameWndEx, CFrameImpl |
| [`feature-pack-ui-38`](shards/feature-pack-ui-38.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 38/439: CFrameWndEx |
| [`feature-pack-ui-39`](shards/feature-pack-ui-39.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 39/439: CFrameWndEx |
| [`feature-pack-ui-40`](shards/feature-pack-ui-40.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 40/439: CFrameWndEx |
| [`feature-pack-ui-41`](shards/feature-pack-ui-41.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 41/439: CFrameWndEx |
| [`feature-pack-ui-42`](shards/feature-pack-ui-42.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 42/439: CFrameWndEx |
| [`feature-pack-ui-43`](shards/feature-pack-ui-43.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 43/439: CFrameWndEx, CGlobalUtils |
| [`feature-pack-ui-44`](shards/feature-pack-ui-44.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 44/439: CKeyboardManager, CGlobalUtils |
| [`feature-pack-ui-45`](shards/feature-pack-ui-45.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 45/439: CMDIChildWndEx |
| [`feature-pack-ui-46`](shards/feature-pack-ui-46.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 46/439: CMDIChildWndEx |
| [`feature-pack-ui-47`](shards/feature-pack-ui-47.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 47/439: CMDIChildWndEx |
| [`feature-pack-ui-48`](shards/feature-pack-ui-48.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 48/439: CMDIChildWndEx |
| [`feature-pack-ui-49`](shards/feature-pack-ui-49.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 49/439: CMDIChildWndEx |
| [`feature-pack-ui-50`](shards/feature-pack-ui-50.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 50/439: CMDIChildWndEx |
| [`feature-pack-ui-51`](shards/feature-pack-ui-51.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 51/439: CMDIClientAreaWnd, CMDIChildWndEx |
| [`feature-pack-ui-52`](shards/feature-pack-ui-52.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 52/439: CMDIClientAreaWnd |
| [`feature-pack-ui-53`](shards/feature-pack-ui-53.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 53/439: CMDIClientAreaWnd |
| [`feature-pack-ui-54`](shards/feature-pack-ui-54.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 54/439: CMDIClientAreaWnd |
| [`feature-pack-ui-55`](shards/feature-pack-ui-55.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 55/439: CMDIClientAreaWnd, CMDIFrameWndEx |
| [`feature-pack-ui-56`](shards/feature-pack-ui-56.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 56/439: CMDIFrameWndEx |
| [`feature-pack-ui-57`](shards/feature-pack-ui-57.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 57/439: CMDIFrameWndEx |
| [`feature-pack-ui-58`](shards/feature-pack-ui-58.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 58/439: CMDIFrameWndEx |
| [`feature-pack-ui-59`](shards/feature-pack-ui-59.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 59/439: CMDIFrameWndEx |
| [`feature-pack-ui-60`](shards/feature-pack-ui-60.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 60/439: CMDIFrameWndEx |
| [`feature-pack-ui-61`](shards/feature-pack-ui-61.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 61/439: CMDIFrameWndEx |
| [`feature-pack-ui-62`](shards/feature-pack-ui-62.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 62/439: CMDIFrameWndEx |
| [`feature-pack-ui-63`](shards/feature-pack-ui-63.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 63/439: CMDIFrameWndEx, CMFCAcceleratorKeyAssignCtrl |
| [`feature-pack-ui-64`](shards/feature-pack-ui-64.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 64/439: CMFCAutoHideBar, CMFCAcceleratorKeyAssignCtrl |
| [`feature-pack-ui-65`](shards/feature-pack-ui-65.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 65/439: CMFCAutoHideBar, CMFCAutoHideButton |
| [`feature-pack-ui-66`](shards/feature-pack-ui-66.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 66/439: CMFCAutoHideButton, CMFCBaseAccessibleObject |
| [`feature-pack-ui-67`](shards/feature-pack-ui-67.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 67/439: CMFCBaseAccessibleObject |
| [`feature-pack-ui-68`](shards/feature-pack-ui-68.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 68/439: CMFCBaseAccessibleObject |
| [`feature-pack-ui-69`](shards/feature-pack-ui-69.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 69/439: CMFCBaseTabCtrl, CMFCBaseAccessibleObject |
| [`feature-pack-ui-70`](shards/feature-pack-ui-70.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 70/439: CMFCBaseTabCtrl |
| [`feature-pack-ui-71`](shards/feature-pack-ui-71.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 71/439: CMFCBaseTabCtrl |
| [`feature-pack-ui-72`](shards/feature-pack-ui-72.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 72/439: CMFCBaseTabCtrl |
| [`feature-pack-ui-73`](shards/feature-pack-ui-73.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 73/439: CMFCBaseTabCtrl |
| [`feature-pack-ui-74`](shards/feature-pack-ui-74.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 74/439: CMFCBaseTabCtrl |
| [`feature-pack-ui-75`](shards/feature-pack-ui-75.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 75/439: CMFCBaseTabCtrl |
| [`feature-pack-ui-76`](shards/feature-pack-ui-76.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 76/439: CMFCBaseTabCtrl |
| [`feature-pack-ui-77`](shards/feature-pack-ui-77.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 77/439: CMFCBaseTabCtrl, CMFCBaseVisualManager |
| [`feature-pack-ui-78`](shards/feature-pack-ui-78.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 78/439: CMFCBaseVisualManager, CMFCButton |
| [`feature-pack-ui-79`](shards/feature-pack-ui-79.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 79/439: CMFCButton |
| [`feature-pack-ui-80`](shards/feature-pack-ui-80.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 80/439: CMFCButton |
| [`feature-pack-ui-81`](shards/feature-pack-ui-81.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 81/439: CMFCButton |
| [`feature-pack-ui-82`](shards/feature-pack-ui-82.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 82/439: CMFCButton |
| [`feature-pack-ui-83`](shards/feature-pack-ui-83.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 83/439: CMFCButton |
| [`feature-pack-ui-84`](shards/feature-pack-ui-84.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 84/439: CMFCCaptionBar |
| [`feature-pack-ui-85`](shards/feature-pack-ui-85.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 85/439: CMFCCaptionBar |
| [`feature-pack-ui-86`](shards/feature-pack-ui-86.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 86/439: CMFCCaptionBar |
| [`feature-pack-ui-87`](shards/feature-pack-ui-87.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 87/439: CMFCCaptionBar |
| [`feature-pack-ui-88`](shards/feature-pack-ui-88.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 88/439: CMFCCaptionBar, CMFCCmdUsageCount |
| [`feature-pack-ui-89`](shards/feature-pack-ui-89.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 89/439: CMFCColorBar, CMFCCmdUsageCount |
| [`feature-pack-ui-90`](shards/feature-pack-ui-90.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 90/439: CMFCColorBar |
| [`feature-pack-ui-91`](shards/feature-pack-ui-91.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 91/439: CMFCColorBar |
| [`feature-pack-ui-92`](shards/feature-pack-ui-92.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 92/439: CMFCColorBar |
| [`feature-pack-ui-93`](shards/feature-pack-ui-93.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 93/439: CMFCColorButton, CMFCColorBar |
| [`feature-pack-ui-94`](shards/feature-pack-ui-94.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 94/439: CMFCColorButton |
| [`feature-pack-ui-95`](shards/feature-pack-ui-95.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 95/439: CMFCColorDialog, CMFCColorButton |
| [`feature-pack-ui-96`](shards/feature-pack-ui-96.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 96/439: CMFCColorDialog, CMFCColorMenuButton |
| [`feature-pack-ui-97`](shards/feature-pack-ui-97.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 97/439: CMFCColorMenuButton |
| [`feature-pack-ui-98`](shards/feature-pack-ui-98.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 98/439: CMFCColorMenuButton, CMFCColorPickerCtrl |
| [`feature-pack-ui-99`](shards/feature-pack-ui-99.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 99/439: CMFCColorPickerCtrl |
| [`feature-pack-ui-100`](shards/feature-pack-ui-100.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 100/439: CMFCColorPickerCtrl |
| [`feature-pack-ui-101`](shards/feature-pack-ui-101.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 101/439: CMFCColorPickerCtrl |
| [`feature-pack-ui-102`](shards/feature-pack-ui-102.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 102/439: CMFCControlContainer, CMFCColorPickerCtrl |
| [`feature-pack-ui-103`](shards/feature-pack-ui-103.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 103/439: CMFCCustomColorsPropertyPage, CMFCControlContainer |
| [`feature-pack-ui-104`](shards/feature-pack-ui-104.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 104/439: CMFCCustomizeButton, CMFCCustomColorsPropertyPage |
| [`feature-pack-ui-105`](shards/feature-pack-ui-105.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 105/439: CMFCCustomizeMenuButton, CMFCCustomizeButton |
| [`feature-pack-ui-106`](shards/feature-pack-ui-106.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 106/439: CMFCCustomizeMenuButton, CMFCDesktopAlertDialog |
| [`feature-pack-ui-107`](shards/feature-pack-ui-107.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 107/439: CMFCDesktopAlertDialog |
| [`feature-pack-ui-108`](shards/feature-pack-ui-108.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 108/439: CMFCDesktopAlertWnd, CMFCDesktopAlertDialog |
| [`feature-pack-ui-109`](shards/feature-pack-ui-109.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 109/439: CMFCDesktopAlertWnd |
| [`feature-pack-ui-110`](shards/feature-pack-ui-110.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 110/439: CMFCDesktopAlertWnd, CMFCDragFrameImpl |
| [`feature-pack-ui-111`](shards/feature-pack-ui-111.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 111/439: CMFCDragFrameImpl, CMFCDropDownFrame |
| [`feature-pack-ui-112`](shards/feature-pack-ui-112.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 112/439: CMFCDropDownFrame |
| [`feature-pack-ui-113`](shards/feature-pack-ui-113.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 113/439: CMFCDropDownListBox, CMFCDropDownFrame |
| [`feature-pack-ui-114`](shards/feature-pack-ui-114.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 114/439: CMFCDropDownListBox, CMFCDropDownToolBar |
| [`feature-pack-ui-115`](shards/feature-pack-ui-115.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 115/439: CMFCDropDownToolbarButton, CMFCDropDownToolBar |
| [`feature-pack-ui-116`](shards/feature-pack-ui-116.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 116/439: CMFCDropDownToolbarButton, CMFCDynamicLayout |
| [`feature-pack-ui-117`](shards/feature-pack-ui-117.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 117/439: CMFCDynamicLayout, CMFCEditBrowseCtrl |
| [`feature-pack-ui-118`](shards/feature-pack-ui-118.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 118/439: CMFCEditBrowseCtrl |
| [`feature-pack-ui-119`](shards/feature-pack-ui-119.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 119/439: CMFCEditBrowseCtrl |
| [`feature-pack-ui-120`](shards/feature-pack-ui-120.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 120/439: CMFCEditBrowseCtrl, CMFCFilterChunkValueImpl |
| [`feature-pack-ui-121`](shards/feature-pack-ui-121.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 121/439: CMFCFilterChunkValueImpl |
| [`feature-pack-ui-122`](shards/feature-pack-ui-122.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 122/439: CMFCFontComboBox |
| [`feature-pack-ui-123`](shards/feature-pack-ui-123.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 123/439: CMFCFontComboBox, CMFCHeaderCtrl |
| [`feature-pack-ui-124`](shards/feature-pack-ui-124.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 124/439: CMFCHeaderCtrl |
| [`feature-pack-ui-125`](shards/feature-pack-ui-125.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 125/439: CMFCHeaderCtrl, CMFCImageEditorDialog |
| [`feature-pack-ui-126`](shards/feature-pack-ui-126.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 126/439: CMFCImageEditorDialog |
| [`feature-pack-ui-127`](shards/feature-pack-ui-127.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 127/439: CMFCImagePaintArea, CMFCImageEditorDialog |
| [`feature-pack-ui-128`](shards/feature-pack-ui-128.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 128/439: CMFCImagePaintArea, CMFCKeyMapDialog |
| [`feature-pack-ui-129`](shards/feature-pack-ui-129.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 129/439: CMFCKeyMapDialog |
| [`feature-pack-ui-130`](shards/feature-pack-ui-130.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 130/439: CMFCKeyMapDialog, CMFCLinkCtrl |
| [`feature-pack-ui-131`](shards/feature-pack-ui-131.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 131/439: CMFCLinkCtrl, CMFCListCtrl |
| [`feature-pack-ui-132`](shards/feature-pack-ui-132.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 132/439: CMFCListCtrl |
| [`feature-pack-ui-133`](shards/feature-pack-ui-133.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 133/439: CMFCListCtrl, CMFCMaskedEdit |
| [`feature-pack-ui-134`](shards/feature-pack-ui-134.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 134/439: CMFCMaskedEdit |
| [`feature-pack-ui-135`](shards/feature-pack-ui-135.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 135/439: CMFCMaskedEdit |
| [`feature-pack-ui-136`](shards/feature-pack-ui-136.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 136/439: CMFCMenuBar, CMFCMaskedEdit |
| [`feature-pack-ui-137`](shards/feature-pack-ui-137.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 137/439: CMFCMenuBar |
| [`feature-pack-ui-138`](shards/feature-pack-ui-138.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 138/439: CMFCMenuBar |
| [`feature-pack-ui-139`](shards/feature-pack-ui-139.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 139/439: CMFCMenuBar, CMFCMenuButton |
| [`feature-pack-ui-140`](shards/feature-pack-ui-140.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 140/439: CMFCMenuButton |
| [`feature-pack-ui-141`](shards/feature-pack-ui-141.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 141/439: CMFCMousePropertyPage, CMFCMenuButton |
| [`feature-pack-ui-142`](shards/feature-pack-ui-142.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 142/439: CMFCOutlookBar, CMFCMousePropertyPage |
| [`feature-pack-ui-143`](shards/feature-pack-ui-143.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 143/439: CMFCOutlookBar, CMFCOutlookBarPane |
| [`feature-pack-ui-144`](shards/feature-pack-ui-144.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 144/439: CMFCOutlookBarPane |
| [`feature-pack-ui-145`](shards/feature-pack-ui-145.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 145/439: CMFCOutlookBarPane |
| [`feature-pack-ui-146`](shards/feature-pack-ui-146.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 146/439: CMFCOutlookBarPane |
| [`feature-pack-ui-147`](shards/feature-pack-ui-147.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 147/439: CMFCOutlookBarPane |
| [`feature-pack-ui-148`](shards/feature-pack-ui-148.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 148/439: CMFCOutlookBarPane, CMFCOutlookBarPaneButton |
| [`feature-pack-ui-149`](shards/feature-pack-ui-149.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 149/439: CMFCOutlookBarTabCtrl, CMFCOutlookBarPaneButton |
| [`feature-pack-ui-150`](shards/feature-pack-ui-150.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 150/439: CMFCOutlookBarTabCtrl |
| [`feature-pack-ui-151`](shards/feature-pack-ui-151.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 151/439: CMFCOutlookBarTabCtrl |
| [`feature-pack-ui-152`](shards/feature-pack-ui-152.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 152/439: CMFCOutlookBarTabCtrl |
| [`feature-pack-ui-153`](shards/feature-pack-ui-153.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 153/439: CMFCOutlookBarToolBar, CMFCPopupMenu |
| [`feature-pack-ui-154`](shards/feature-pack-ui-154.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 154/439: CMFCPopupMenu |
| [`feature-pack-ui-155`](shards/feature-pack-ui-155.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 155/439: CMFCPopupMenu |
| [`feature-pack-ui-156`](shards/feature-pack-ui-156.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 156/439: CMFCPopupMenu |
| [`feature-pack-ui-157`](shards/feature-pack-ui-157.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 157/439: CMFCPopupMenu |
| [`feature-pack-ui-158`](shards/feature-pack-ui-158.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 158/439: CMFCPopupMenu |
| [`feature-pack-ui-159`](shards/feature-pack-ui-159.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 159/439: CMFCPopupMenu |
| [`feature-pack-ui-160`](shards/feature-pack-ui-160.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 160/439: CMFCPopupMenuBar, CMFCPopupMenu |
| [`feature-pack-ui-161`](shards/feature-pack-ui-161.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 161/439: CMFCPopupMenuBar |
| [`feature-pack-ui-162`](shards/feature-pack-ui-162.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 162/439: CMFCPopupMenuBar |
| [`feature-pack-ui-163`](shards/feature-pack-ui-163.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 163/439: CMFCPopupMenuBar, CMFCPreviewCtrlImpl |
| [`feature-pack-ui-164`](shards/feature-pack-ui-164.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 164/439: CMFCPreviewCtrlImpl |
| [`feature-pack-ui-165`](shards/feature-pack-ui-165.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 165/439: CMFCPropertyGridColorProperty, CMFCPreviewCtrlImpl |
| [`feature-pack-ui-166`](shards/feature-pack-ui-166.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 166/439: CMFCPropertyGridCtrl, CMFCPropertyGridColorProperty |
| [`feature-pack-ui-167`](shards/feature-pack-ui-167.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 167/439: CMFCPropertyGridCtrl |
| [`feature-pack-ui-168`](shards/feature-pack-ui-168.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 168/439: CMFCPropertyGridCtrl |
| [`feature-pack-ui-169`](shards/feature-pack-ui-169.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 169/439: CMFCPropertyGridCtrl |
| [`feature-pack-ui-170`](shards/feature-pack-ui-170.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 170/439: CMFCPropertyGridCtrl |
| [`feature-pack-ui-171`](shards/feature-pack-ui-171.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 171/439: CMFCPropertyGridCtrl |
| [`feature-pack-ui-172`](shards/feature-pack-ui-172.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 172/439: CMFCPropertyGridFontProperty, CMFCPropertyGridCtrl, CMFCPropertyGridProperty |
| [`feature-pack-ui-173`](shards/feature-pack-ui-173.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 173/439: CMFCPropertyGridProperty, CMFCPropertyGridToolTipCtrl |
| [`feature-pack-ui-174`](shards/feature-pack-ui-174.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 174/439: CMFCPropertyGridToolTipCtrl, CMFCPropertyPage |
| [`feature-pack-ui-175`](shards/feature-pack-ui-175.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 175/439: CMFCPropertyPage, CMFCReBar |
| [`feature-pack-ui-176`](shards/feature-pack-ui-176.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 176/439: CMFCReBar |
| [`feature-pack-ui-177`](shards/feature-pack-ui-177.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 177/439: CMFCReBar, CMFCRibbonApplicationButton |
| [`feature-pack-ui-178`](shards/feature-pack-ui-178.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 178/439: CMFCRibbonApplicationButton, CMFCRibbonBar |
| [`feature-pack-ui-179`](shards/feature-pack-ui-179.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 179/439: CMFCRibbonBar |
| [`feature-pack-ui-180`](shards/feature-pack-ui-180.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 180/439: CMFCRibbonBar |
| [`feature-pack-ui-181`](shards/feature-pack-ui-181.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 181/439: CMFCRibbonBar |
| [`feature-pack-ui-182`](shards/feature-pack-ui-182.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 182/439: CMFCRibbonBar |
| [`feature-pack-ui-183`](shards/feature-pack-ui-183.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 183/439: CMFCRibbonBar |
| [`feature-pack-ui-184`](shards/feature-pack-ui-184.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 184/439: CMFCRibbonBar |
| [`feature-pack-ui-185`](shards/feature-pack-ui-185.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 185/439: CMFCRibbonBar |
| [`feature-pack-ui-186`](shards/feature-pack-ui-186.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 186/439: CMFCRibbonBar |
| [`feature-pack-ui-187`](shards/feature-pack-ui-187.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 187/439: CMFCRibbonBar |
| [`feature-pack-ui-188`](shards/feature-pack-ui-188.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 188/439: CMFCRibbonBar |
| [`feature-pack-ui-189`](shards/feature-pack-ui-189.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 189/439: CMFCRibbonBar |
| [`feature-pack-ui-190`](shards/feature-pack-ui-190.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 190/439: CMFCRibbonBar, CMFCRibbonBaseElement |
| [`feature-pack-ui-191`](shards/feature-pack-ui-191.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 191/439: CMFCRibbonBaseElement |
| [`feature-pack-ui-192`](shards/feature-pack-ui-192.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 192/439: CMFCRibbonBaseElement |
| [`feature-pack-ui-193`](shards/feature-pack-ui-193.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 193/439: CMFCRibbonBaseElement |
| [`feature-pack-ui-194`](shards/feature-pack-ui-194.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 194/439: CMFCRibbonBaseElement |
| [`feature-pack-ui-195`](shards/feature-pack-ui-195.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 195/439: CMFCRibbonBaseElement |
| [`feature-pack-ui-196`](shards/feature-pack-ui-196.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 196/439: CMFCRibbonButton, CMFCRibbonBaseElement |
| [`feature-pack-ui-197`](shards/feature-pack-ui-197.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 197/439: CMFCRibbonButton |
| [`feature-pack-ui-198`](shards/feature-pack-ui-198.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 198/439: CMFCRibbonButton |
| [`feature-pack-ui-199`](shards/feature-pack-ui-199.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 199/439: CMFCRibbonButton, CMFCRibbonButtonsGroup |
| [`feature-pack-ui-200`](shards/feature-pack-ui-200.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 200/439: CMFCRibbonButtonsGroup |
| [`feature-pack-ui-201`](shards/feature-pack-ui-201.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 201/439: CMFCRibbonButtonsGroup |
| [`feature-pack-ui-202`](shards/feature-pack-ui-202.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 202/439: CMFCRibbonButtonsGroup |
| [`feature-pack-ui-203`](shards/feature-pack-ui-203.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 203/439: CMFCRibbonCaptionButton, CMFCRibbonButtonsGroup, CMFCRibbonCategory |
| [`feature-pack-ui-204`](shards/feature-pack-ui-204.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 204/439: CMFCRibbonCategory |
| [`feature-pack-ui-205`](shards/feature-pack-ui-205.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 205/439: CMFCRibbonCategory |
| [`feature-pack-ui-206`](shards/feature-pack-ui-206.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 206/439: CMFCRibbonCategory |
| [`feature-pack-ui-207`](shards/feature-pack-ui-207.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 207/439: CMFCRibbonCategory |
| [`feature-pack-ui-208`](shards/feature-pack-ui-208.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 208/439: CMFCRibbonCategory |
| [`feature-pack-ui-209`](shards/feature-pack-ui-209.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 209/439: CMFCRibbonCategory, CMFCRibbonCollector |
| [`feature-pack-ui-210`](shards/feature-pack-ui-210.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 210/439: CMFCRibbonCollector |
| [`feature-pack-ui-211`](shards/feature-pack-ui-211.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 211/439: CMFCRibbonCollector, CMFCRibbonColorButton |
| [`feature-pack-ui-212`](shards/feature-pack-ui-212.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 212/439: CMFCRibbonColorButton |
| [`feature-pack-ui-213`](shards/feature-pack-ui-213.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 213/439: CMFCRibbonColorButton, CMFCRibbonComboBox |
| [`feature-pack-ui-214`](shards/feature-pack-ui-214.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 214/439: CMFCRibbonComboBox |
| [`feature-pack-ui-215`](shards/feature-pack-ui-215.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 215/439: CMFCRibbonComboBox |
| [`feature-pack-ui-216`](shards/feature-pack-ui-216.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 216/439: CMFCRibbonCommandsListBox, CMFCRibbonComboBox |
| [`feature-pack-ui-217`](shards/feature-pack-ui-217.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 217/439: CMFCRibbonConstructor, CMFCRibbonCommandsListBox |
| [`feature-pack-ui-218`](shards/feature-pack-ui-218.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 218/439: CMFCRibbonConstructor |
| [`feature-pack-ui-219`](shards/feature-pack-ui-219.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 219/439: CMFCRibbonContextCaption, CMFCRibbonConstructor |
| [`feature-pack-ui-220`](shards/feature-pack-ui-220.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 220/439: CMFCRibbonCustomizePropertyPage, CMFCRibbonContextCaption |
| [`feature-pack-ui-221`](shards/feature-pack-ui-221.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 221/439: CMFCRibbonCustomizePropertyPage, CMFCRibbonDefaultPanelButton |
| [`feature-pack-ui-222`](shards/feature-pack-ui-222.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 222/439: CMFCRibbonDefaultPanelButton, CMFCRibbonEdit |
| [`feature-pack-ui-223`](shards/feature-pack-ui-223.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 223/439: CMFCRibbonEdit |
| [`feature-pack-ui-224`](shards/feature-pack-ui-224.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 224/439: CMFCRibbonEdit |
| [`feature-pack-ui-225`](shards/feature-pack-ui-225.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 225/439: CMFCRibbonFontComboBox, CMFCRibbonEdit |
| [`feature-pack-ui-226`](shards/feature-pack-ui-226.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 226/439: CMFCRibbonGallery, CMFCRibbonFontComboBox |
| [`feature-pack-ui-227`](shards/feature-pack-ui-227.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 227/439: CMFCRibbonGallery |
| [`feature-pack-ui-228`](shards/feature-pack-ui-228.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 228/439: CMFCRibbonGallery |
| [`feature-pack-ui-229`](shards/feature-pack-ui-229.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 229/439: CMFCRibbonGallery |
| [`feature-pack-ui-230`](shards/feature-pack-ui-230.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 230/439: CMFCRibbonGalleryIcon |
| [`feature-pack-ui-231`](shards/feature-pack-ui-231.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 231/439: CMFCRibbonGalleryIcon, CMFCRibbonGalleryMenuButton |
| [`feature-pack-ui-232`](shards/feature-pack-ui-232.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 232/439: CMFCRibbonInfo, CMFCRibbonGalleryMenuButton |
| [`feature-pack-ui-233`](shards/feature-pack-ui-233.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 233/439: CMFCRibbonInfoLoader, CMFCRibbonInfo, CMFCRibbonKeyTip |
| [`feature-pack-ui-234`](shards/feature-pack-ui-234.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 234/439: CMFCRibbonKeyTip, CMFCRibbonKeyboardCustomizeDialog |
| [`feature-pack-ui-235`](shards/feature-pack-ui-235.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 235/439: CMFCRibbonKeyboardCustomizeDialog |
| [`feature-pack-ui-236`](shards/feature-pack-ui-236.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 236/439: CMFCRibbonLabel, CMFCRibbonLaunchButton |
| [`feature-pack-ui-237`](shards/feature-pack-ui-237.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 237/439: CMFCRibbonLinkCtrl, CMFCRibbonLaunchButton |
| [`feature-pack-ui-238`](shards/feature-pack-ui-238.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 238/439: CMFCRibbonMainPanel, CMFCRibbonLinkCtrl |
| [`feature-pack-ui-239`](shards/feature-pack-ui-239.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 239/439: CMFCRibbonMainPanel, CMFCRibbonMainPanelButton |
| [`feature-pack-ui-240`](shards/feature-pack-ui-240.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 240/439: CMFCRibbonMiniToolBar, CMFCRibbonMainPanelButton, CMFCRibbonPanel |
| [`feature-pack-ui-241`](shards/feature-pack-ui-241.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 241/439: CMFCRibbonPanel |
| [`feature-pack-ui-242`](shards/feature-pack-ui-242.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 242/439: CMFCRibbonPanel |
| [`feature-pack-ui-243`](shards/feature-pack-ui-243.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 243/439: CMFCRibbonPanel |
| [`feature-pack-ui-244`](shards/feature-pack-ui-244.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 244/439: CMFCRibbonPanel |
| [`feature-pack-ui-245`](shards/feature-pack-ui-245.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 245/439: CMFCRibbonPanel |
| [`feature-pack-ui-246`](shards/feature-pack-ui-246.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 246/439: CMFCRibbonPanel |
| [`feature-pack-ui-247`](shards/feature-pack-ui-247.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 247/439: CMFCRibbonPanel |
| [`feature-pack-ui-248`](shards/feature-pack-ui-248.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 248/439: CMFCRibbonPanelMenu, CMFCRibbonPanel |
| [`feature-pack-ui-249`](shards/feature-pack-ui-249.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 249/439: CMFCRibbonPanelMenuBar, CMFCRibbonPanelMenu |
| [`feature-pack-ui-250`](shards/feature-pack-ui-250.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 250/439: CMFCRibbonPanelMenuBar |
| [`feature-pack-ui-251`](shards/feature-pack-ui-251.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 251/439: CMFCRibbonPanelMenuBar |
| [`feature-pack-ui-252`](shards/feature-pack-ui-252.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 252/439: CMFCRibbonPanelMenuBar |
| [`feature-pack-ui-253`](shards/feature-pack-ui-253.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 253/439: CMFCRibbonQuickAccessToolBar, CMFCRibbonPanelMenuBar |
| [`feature-pack-ui-254`](shards/feature-pack-ui-254.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 254/439: CMFCRibbonQuickAccessToolBar |
| [`feature-pack-ui-255`](shards/feature-pack-ui-255.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 255/439: CMFCRibbonRecentFilesList, CMFCRibbonQuickAccessToolBar |
| [`feature-pack-ui-256`](shards/feature-pack-ui-256.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 256/439: CMFCRibbonRichEditCtrl, CMFCRibbonSeparator |
| [`feature-pack-ui-257`](shards/feature-pack-ui-257.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 257/439: CMFCRibbonSlider, CMFCRibbonSeparator |
| [`feature-pack-ui-258`](shards/feature-pack-ui-258.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 258/439: CMFCRibbonSlider, CMFCRibbonStatusBar |
| [`feature-pack-ui-259`](shards/feature-pack-ui-259.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 259/439: CMFCRibbonStatusBar |
| [`feature-pack-ui-260`](shards/feature-pack-ui-260.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 260/439: CMFCRibbonStatusBar |
| [`feature-pack-ui-261`](shards/feature-pack-ui-261.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 261/439: CMFCRibbonStatusBar |
| [`feature-pack-ui-262`](shards/feature-pack-ui-262.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 262/439: CMFCRibbonStatusBarPane, CMFCRibbonStatusBar, CMFCRibbonTab |
| [`feature-pack-ui-263`](shards/feature-pack-ui-263.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 263/439: CMFCRibbonTab |
| [`feature-pack-ui-264`](shards/feature-pack-ui-264.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 264/439: CMFCRibbonTab, CMFCRibbonTabsGroup |
| [`feature-pack-ui-265`](shards/feature-pack-ui-265.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 265/439: CMFCRibbonTabsGroup, CMFCRibbonUndoButton |
| [`feature-pack-ui-266`](shards/feature-pack-ui-266.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 266/439: CMFCRibbonUndoButton, CMFCShadowRenderer |
| [`feature-pack-ui-267`](shards/feature-pack-ui-267.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 267/439: CMFCShadowRenderer, CMFCShadowWnd, CMFCShellListCtrl |
| [`feature-pack-ui-268`](shards/feature-pack-ui-268.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 268/439: CMFCShellListCtrl |
| [`feature-pack-ui-269`](shards/feature-pack-ui-269.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 269/439: CMFCShellListCtrl |
| [`feature-pack-ui-270`](shards/feature-pack-ui-270.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 270/439: CMFCShellListCtrl |
| [`feature-pack-ui-271`](shards/feature-pack-ui-271.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 271/439: CMFCShellTreeCtrl, CMFCShellListCtrl |
| [`feature-pack-ui-272`](shards/feature-pack-ui-272.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 272/439: CMFCShellTreeCtrl |
| [`feature-pack-ui-273`](shards/feature-pack-ui-273.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 273/439: CMFCShellTreeCtrl, CMFCShowAllButton |
| [`feature-pack-ui-274`](shards/feature-pack-ui-274.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 274/439: CMFCShowAllButton, CMFCSpinButtonCtrl |
| [`feature-pack-ui-275`](shards/feature-pack-ui-275.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 275/439: CMFCStandardColorsPropertyPage, CMFCSpinButtonCtrl |
| [`feature-pack-ui-276`](shards/feature-pack-ui-276.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 276/439: CMFCStatusBar, CMFCStandardColorsPropertyPage |
| [`feature-pack-ui-277`](shards/feature-pack-ui-277.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 277/439: CMFCStatusBar |
| [`feature-pack-ui-278`](shards/feature-pack-ui-278.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 278/439: CMFCStatusBar |
| [`feature-pack-ui-279`](shards/feature-pack-ui-279.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 279/439: CMFCStatusBar |
| [`feature-pack-ui-280`](shards/feature-pack-ui-280.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 280/439: CMFCStatusBar, CMFCTabCtrl |
| [`feature-pack-ui-281`](shards/feature-pack-ui-281.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 281/439: CMFCTabCtrl |
| [`feature-pack-ui-282`](shards/feature-pack-ui-282.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 282/439: CMFCTabCtrl |
| [`feature-pack-ui-283`](shards/feature-pack-ui-283.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 283/439: CMFCTabCtrl |
| [`feature-pack-ui-284`](shards/feature-pack-ui-284.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 284/439: CMFCTabCtrl |
| [`feature-pack-ui-285`](shards/feature-pack-ui-285.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 285/439: CMFCTabCtrl |
| [`feature-pack-ui-286`](shards/feature-pack-ui-286.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 286/439: CMFCTabCtrl |
| [`feature-pack-ui-287`](shards/feature-pack-ui-287.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 287/439: CMFCTasksPane |
| [`feature-pack-ui-288`](shards/feature-pack-ui-288.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 288/439: CMFCTasksPane |
| [`feature-pack-ui-289`](shards/feature-pack-ui-289.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 289/439: CMFCTasksPane |
| [`feature-pack-ui-290`](shards/feature-pack-ui-290.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 290/439: CMFCTasksPane |
| [`feature-pack-ui-291`](shards/feature-pack-ui-291.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 291/439: CMFCTasksPane |
| [`feature-pack-ui-292`](shards/feature-pack-ui-292.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 292/439: CMFCTasksPane |
| [`feature-pack-ui-293`](shards/feature-pack-ui-293.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 293/439: CMFCTasksPane |
| [`feature-pack-ui-294`](shards/feature-pack-ui-294.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 294/439: CMFCTasksPane |
| [`feature-pack-ui-295`](shards/feature-pack-ui-295.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 295/439: CMFCTasksPane |
| [`feature-pack-ui-296`](shards/feature-pack-ui-296.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 296/439: CMFCTasksPane, CMFCTasksPaneFrameWnd |
| [`feature-pack-ui-297`](shards/feature-pack-ui-297.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 297/439: CMFCTasksPaneFrameWnd |
| [`feature-pack-ui-298`](shards/feature-pack-ui-298.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 298/439: CMFCTasksPaneToolBar, CMFCToolBar |
| [`feature-pack-ui-299`](shards/feature-pack-ui-299.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 299/439: CMFCToolBar |
| [`feature-pack-ui-300`](shards/feature-pack-ui-300.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 300/439: CMFCToolBar |
| [`feature-pack-ui-301`](shards/feature-pack-ui-301.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 301/439: CMFCToolBar |
| [`feature-pack-ui-302`](shards/feature-pack-ui-302.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 302/439: CMFCToolBar |
| [`feature-pack-ui-303`](shards/feature-pack-ui-303.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 303/439: CMFCToolBar |
| [`feature-pack-ui-304`](shards/feature-pack-ui-304.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 304/439: CMFCToolBar |
| [`feature-pack-ui-305`](shards/feature-pack-ui-305.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 305/439: CMFCToolBar |
| [`feature-pack-ui-306`](shards/feature-pack-ui-306.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 306/439: CMFCToolBar |
| [`feature-pack-ui-307`](shards/feature-pack-ui-307.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 307/439: CMFCToolBar |
| [`feature-pack-ui-308`](shards/feature-pack-ui-308.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 308/439: CMFCToolBar |
| [`feature-pack-ui-309`](shards/feature-pack-ui-309.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 309/439: CMFCToolBar |
| [`feature-pack-ui-310`](shards/feature-pack-ui-310.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 310/439: CMFCToolBar |
| [`feature-pack-ui-311`](shards/feature-pack-ui-311.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 311/439: CMFCToolBar |
| [`feature-pack-ui-312`](shards/feature-pack-ui-312.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 312/439: CMFCToolBar |
| [`feature-pack-ui-313`](shards/feature-pack-ui-313.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 313/439: CMFCToolBar |
| [`feature-pack-ui-314`](shards/feature-pack-ui-314.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 314/439: CMFCToolBar |
| [`feature-pack-ui-315`](shards/feature-pack-ui-315.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 315/439: CMFCToolBar |
| [`feature-pack-ui-316`](shards/feature-pack-ui-316.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 316/439: CMFCToolBar |
| [`feature-pack-ui-317`](shards/feature-pack-ui-317.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 317/439: CMFCToolBar |
| [`feature-pack-ui-318`](shards/feature-pack-ui-318.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 318/439: CMFCToolBar, CMFCToolBarButton |
| [`feature-pack-ui-319`](shards/feature-pack-ui-319.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 319/439: CMFCToolBarButton |
| [`feature-pack-ui-320`](shards/feature-pack-ui-320.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 320/439: CMFCToolBarButtonCustomizeDialog, CMFCToolBarButton |
| [`feature-pack-ui-321`](shards/feature-pack-ui-321.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 321/439: CMFCToolBarButtonCustomizeDialog, CMFCToolBarButtonsListButton |
| [`feature-pack-ui-322`](shards/feature-pack-ui-322.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 322/439: CMFCToolBarButtonsListButton |
| [`feature-pack-ui-323`](shards/feature-pack-ui-323.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 323/439: CMFCToolBarButtonsListButton, CMFCToolBarComboBoxButton |
| [`feature-pack-ui-324`](shards/feature-pack-ui-324.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 324/439: CMFCToolBarComboBoxButton |
| [`feature-pack-ui-325`](shards/feature-pack-ui-325.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 325/439: CMFCToolBarComboBoxButton |
| [`feature-pack-ui-326`](shards/feature-pack-ui-326.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 326/439: CMFCToolBarComboBoxButton, CMFCToolBarComboBoxEdit |
| [`feature-pack-ui-327`](shards/feature-pack-ui-327.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 327/439: CMFCToolBarComboBoxEdit, CMFCToolBarDateTimeCtrl |
| [`feature-pack-ui-328`](shards/feature-pack-ui-328.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 328/439: CMFCToolBarDateTimeCtrl |
| [`feature-pack-ui-329`](shards/feature-pack-ui-329.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 329/439: CMFCToolBarDateTimeCtrl |
| [`feature-pack-ui-330`](shards/feature-pack-ui-330.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 330/439: CMFCToolBarDateTimeCtrl, CMFCToolBarDropSource |
| [`feature-pack-ui-331`](shards/feature-pack-ui-331.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 331/439: CMFCToolBarDropTarget, CMFCToolBarEditBoxButton, CMFCToolBarDropSource |
| [`feature-pack-ui-332`](shards/feature-pack-ui-332.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 332/439: CMFCToolBarEditBoxButton |
| [`feature-pack-ui-333`](shards/feature-pack-ui-333.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 333/439: CMFCToolBarEditBoxButton, CMFCToolBarEditCtrl |
| [`feature-pack-ui-334`](shards/feature-pack-ui-334.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 334/439: CMFCToolBarEditCtrl, CMFCToolBarFontComboBox |
| [`feature-pack-ui-335`](shards/feature-pack-ui-335.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 335/439: CMFCToolBarFontComboBox |
| [`feature-pack-ui-336`](shards/feature-pack-ui-336.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 336/439: CMFCToolBarFontSizeComboBox, CMFCToolBarImages |
| [`feature-pack-ui-337`](shards/feature-pack-ui-337.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 337/439: CMFCToolBarImages |
| [`feature-pack-ui-338`](shards/feature-pack-ui-338.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 338/439: CMFCToolBarImages |
| [`feature-pack-ui-339`](shards/feature-pack-ui-339.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 339/439: CMFCToolBarImages |
| [`feature-pack-ui-340`](shards/feature-pack-ui-340.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 340/439: CMFCToolBarImages, CMFCToolBarMenuButton |
| [`feature-pack-ui-341`](shards/feature-pack-ui-341.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 341/439: CMFCToolBarMenuButton |
| [`feature-pack-ui-342`](shards/feature-pack-ui-342.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 342/439: CMFCToolBarMenuButton, CMFCToolBarSpinEditBoxButton |
| [`feature-pack-ui-343`](shards/feature-pack-ui-343.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 343/439: CMFCToolBarSpinEditBoxButton, CMFCToolBarSystemMenuButton |
| [`feature-pack-ui-344`](shards/feature-pack-ui-344.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 344/439: CMFCToolBarSystemMenuButton, CMFCToolBarsCommandsListBox |
| [`feature-pack-ui-345`](shards/feature-pack-ui-345.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 345/439: CMFCToolBarsCommandsListBox, CMFCToolBarsCommandsPropertyPage |
| [`feature-pack-ui-346`](shards/feature-pack-ui-346.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 346/439: CMFCToolBarsCustomizeDialog, CMFCToolBarsCommandsPropertyPage |
| [`feature-pack-ui-347`](shards/feature-pack-ui-347.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 347/439: CMFCToolBarsCustomizeDialog |
| [`feature-pack-ui-348`](shards/feature-pack-ui-348.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 348/439: CMFCToolBarsCustomizeDialog |
| [`feature-pack-ui-349`](shards/feature-pack-ui-349.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 349/439: CMFCToolBarsKeyboardPropertyPage, CMFCToolBarsCustomizeDialog |
| [`feature-pack-ui-350`](shards/feature-pack-ui-350.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 350/439: CMFCToolBarsKeyboardPropertyPage, CMFCToolBarsListCheckBox |
| [`feature-pack-ui-351`](shards/feature-pack-ui-351.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 351/439: CMFCToolBarsListCheckBox, CMFCToolBarsListPropertyPage |
| [`feature-pack-ui-352`](shards/feature-pack-ui-352.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 352/439: CMFCToolBarsListPropertyPage |
| [`feature-pack-ui-353`](shards/feature-pack-ui-353.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 353/439: CMFCToolBarsMenuPropertyPage, CMFCToolBarsListPropertyPage |
| [`feature-pack-ui-354`](shards/feature-pack-ui-354.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 354/439: CMFCToolBarsMenuPropertyPage, CMFCToolBarsOptionsPropertyPage |
| [`feature-pack-ui-355`](shards/feature-pack-ui-355.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 355/439: CMFCToolBarsOptionsPropertyPage, CMFCToolBarsToolsPropertyPage |
| [`feature-pack-ui-356`](shards/feature-pack-ui-356.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 356/439: CMFCToolBarsToolsPropertyPage, CMFCToolTipCtrl |
| [`feature-pack-ui-357`](shards/feature-pack-ui-357.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 357/439: CMFCToolTipCtrl |
| [`feature-pack-ui-358`](shards/feature-pack-ui-358.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 358/439: CMFCToolTipCtrl, CMFCVisualManager |
| [`feature-pack-ui-359`](shards/feature-pack-ui-359.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 359/439: CMFCVisualManager |
| [`feature-pack-ui-360`](shards/feature-pack-ui-360.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 360/439: CMFCVisualManager |
| [`feature-pack-ui-361`](shards/feature-pack-ui-361.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 361/439: CMFCVisualManager |
| [`feature-pack-ui-362`](shards/feature-pack-ui-362.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 362/439: CMFCVisualManager |
| [`feature-pack-ui-363`](shards/feature-pack-ui-363.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 363/439: CMFCVisualManager |
| [`feature-pack-ui-364`](shards/feature-pack-ui-364.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 364/439: CMFCVisualManager |
| [`feature-pack-ui-365`](shards/feature-pack-ui-365.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 365/439: CMFCVisualManager, CMFCVisualManagerBitmapCache |
| [`feature-pack-ui-366`](shards/feature-pack-ui-366.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 366/439: CMFCVisualManagerBitmapCache, CMFCVisualManagerOffice2003 |
| [`feature-pack-ui-367`](shards/feature-pack-ui-367.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 367/439: CMFCVisualManagerOffice2003 |
| [`feature-pack-ui-368`](shards/feature-pack-ui-368.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 368/439: CMFCVisualManagerOffice2003 |
| [`feature-pack-ui-369`](shards/feature-pack-ui-369.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 369/439: CMFCVisualManagerOffice2003 |
| [`feature-pack-ui-370`](shards/feature-pack-ui-370.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 370/439: CMFCVisualManagerOffice2003 |
| [`feature-pack-ui-371`](shards/feature-pack-ui-371.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 371/439: CMFCVisualManagerOffice2003 |
| [`feature-pack-ui-372`](shards/feature-pack-ui-372.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 372/439: CMFCVisualManagerOffice2003 |
| [`feature-pack-ui-373`](shards/feature-pack-ui-373.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 373/439: CMFCVisualManagerOffice2003, CMFCVisualManagerOffice2007 |
| [`feature-pack-ui-374`](shards/feature-pack-ui-374.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 374/439: CMFCVisualManagerOffice2007 |
| [`feature-pack-ui-375`](shards/feature-pack-ui-375.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 375/439: CMFCVisualManagerOffice2007 |
| [`feature-pack-ui-376`](shards/feature-pack-ui-376.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 376/439: CMFCVisualManagerOffice2007 |
| [`feature-pack-ui-377`](shards/feature-pack-ui-377.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 377/439: CMFCVisualManagerOffice2007 |
| [`feature-pack-ui-378`](shards/feature-pack-ui-378.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 378/439: CMFCVisualManagerOffice2007 |
| [`feature-pack-ui-379`](shards/feature-pack-ui-379.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 379/439: CMFCVisualManagerOffice2007 |
| [`feature-pack-ui-380`](shards/feature-pack-ui-380.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 380/439: CMFCVisualManagerOffice2007 |
| [`feature-pack-ui-381`](shards/feature-pack-ui-381.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 381/439: CMFCVisualManagerOffice2007 |
| [`feature-pack-ui-382`](shards/feature-pack-ui-382.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 382/439: CMFCVisualManagerOffice2007 |
| [`feature-pack-ui-383`](shards/feature-pack-ui-383.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 383/439: CMFCVisualManagerOffice2007 |
| [`feature-pack-ui-384`](shards/feature-pack-ui-384.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 384/439: CMFCVisualManagerOffice2007 |
| [`feature-pack-ui-385`](shards/feature-pack-ui-385.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 385/439: CMFCVisualManagerOfficeXP, CMFCVisualManagerOffice2007 |
| [`feature-pack-ui-386`](shards/feature-pack-ui-386.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 386/439: CMFCVisualManagerOfficeXP |
| [`feature-pack-ui-387`](shards/feature-pack-ui-387.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 387/439: CMFCVisualManagerOfficeXP |
| [`feature-pack-ui-388`](shards/feature-pack-ui-388.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 388/439: CMFCVisualManagerOfficeXP |
| [`feature-pack-ui-389`](shards/feature-pack-ui-389.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 389/439: CMFCVisualManagerOfficeXP |
| [`feature-pack-ui-390`](shards/feature-pack-ui-390.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 390/439: CMFCVisualManagerOfficeXP |
| [`feature-pack-ui-391`](shards/feature-pack-ui-391.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 391/439: CMFCVisualManagerOfficeXP |
| [`feature-pack-ui-392`](shards/feature-pack-ui-392.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 392/439: CMFCVisualManagerOfficeXP, CMFCVisualManagerVS2005 |
| [`feature-pack-ui-393`](shards/feature-pack-ui-393.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 393/439: CMFCVisualManagerVS2005 |
| [`feature-pack-ui-394`](shards/feature-pack-ui-394.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 394/439: CMFCVisualManagerVS2008, CMFCVisualManagerVS2005 |
| [`feature-pack-ui-395`](shards/feature-pack-ui-395.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 395/439: CMFCVisualManagerVS2008 |
| [`feature-pack-ui-396`](shards/feature-pack-ui-396.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 396/439: CMFCVisualManagerWindows |
| [`feature-pack-ui-397`](shards/feature-pack-ui-397.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 397/439: CMFCVisualManagerWindows |
| [`feature-pack-ui-398`](shards/feature-pack-ui-398.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 398/439: CMFCVisualManagerWindows |
| [`feature-pack-ui-399`](shards/feature-pack-ui-399.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 399/439: CMFCVisualManagerWindows |
| [`feature-pack-ui-400`](shards/feature-pack-ui-400.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 400/439: CMFCVisualManagerWindows |
| [`feature-pack-ui-401`](shards/feature-pack-ui-401.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 401/439: CMFCVisualManagerWindows7, CMFCVisualManagerWindows |
| [`feature-pack-ui-402`](shards/feature-pack-ui-402.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 402/439: CMFCVisualManagerWindows7 |
| [`feature-pack-ui-403`](shards/feature-pack-ui-403.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 403/439: CMFCVisualManagerWindows7 |
| [`feature-pack-ui-404`](shards/feature-pack-ui-404.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 404/439: CMFCVisualManagerWindows7 |
| [`feature-pack-ui-405`](shards/feature-pack-ui-405.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 405/439: CMFCVisualManagerWindows7 |
| [`feature-pack-ui-406`](shards/feature-pack-ui-406.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 406/439: CMFCWindowsManagerDialog, CMFCVisualManagerWindows7 |
| [`feature-pack-ui-407`](shards/feature-pack-ui-407.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 407/439: CMFCWindowsManagerDialog |
| [`feature-pack-ui-408`](shards/feature-pack-ui-408.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 408/439: CMFCZoomKernel, CMFCWindowsManagerDialog, CMultiPaneFrameWnd |
| [`feature-pack-ui-409`](shards/feature-pack-ui-409.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 409/439: CMultiPaneFrameWnd |
| [`feature-pack-ui-410`](shards/feature-pack-ui-410.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 410/439: CMultiPaneFrameWnd |
| [`feature-pack-ui-411`](shards/feature-pack-ui-411.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 411/439: CMultiPaneFrameWnd |
| [`feature-pack-ui-412`](shards/feature-pack-ui-412.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 412/439: CMultiPaneFrameWnd, CPane |
| [`feature-pack-ui-413`](shards/feature-pack-ui-413.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 413/439: CPane |
| [`feature-pack-ui-414`](shards/feature-pack-ui-414.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 414/439: CPane |
| [`feature-pack-ui-415`](shards/feature-pack-ui-415.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 415/439: CPane |
| [`feature-pack-ui-416`](shards/feature-pack-ui-416.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 416/439: CPane |
| [`feature-pack-ui-417`](shards/feature-pack-ui-417.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 417/439: CPane, CPaneContainer |
| [`feature-pack-ui-418`](shards/feature-pack-ui-418.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 418/439: CPaneContainer |
| [`feature-pack-ui-419`](shards/feature-pack-ui-419.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 419/439: CPaneContainer |
| [`feature-pack-ui-420`](shards/feature-pack-ui-420.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 420/439: CPaneContainer |
| [`feature-pack-ui-421`](shards/feature-pack-ui-421.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 421/439: CPaneContainer |
| [`feature-pack-ui-422`](shards/feature-pack-ui-422.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 422/439: CPaneContainer, CPaneContainerManager |
| [`feature-pack-ui-423`](shards/feature-pack-ui-423.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 423/439: CPaneContainerManager |
| [`feature-pack-ui-424`](shards/feature-pack-ui-424.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 424/439: CPaneContainerManager |
| [`feature-pack-ui-425`](shards/feature-pack-ui-425.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 425/439: CPaneContainerManager |
| [`feature-pack-ui-426`](shards/feature-pack-ui-426.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 426/439: CPaneContainerManager |
| [`feature-pack-ui-427`](shards/feature-pack-ui-427.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 427/439: CPaneDivider, CPaneContainerManager |
| [`feature-pack-ui-428`](shards/feature-pack-ui-428.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 428/439: CPaneDivider |
| [`feature-pack-ui-429`](shards/feature-pack-ui-429.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 429/439: CPaneDivider |
| [`feature-pack-ui-430`](shards/feature-pack-ui-430.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 430/439: CPaneDivider, CPaneFrameWnd |
| [`feature-pack-ui-431`](shards/feature-pack-ui-431.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 431/439: CPaneFrameWnd |
| [`feature-pack-ui-432`](shards/feature-pack-ui-432.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 432/439: CPaneFrameWnd |
| [`feature-pack-ui-433`](shards/feature-pack-ui-433.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 433/439: CPaneFrameWnd |
| [`feature-pack-ui-434`](shards/feature-pack-ui-434.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 434/439: CPaneFrameWnd |
| [`feature-pack-ui-435`](shards/feature-pack-ui-435.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 435/439: CPaneFrameWnd |
| [`feature-pack-ui-436`](shards/feature-pack-ui-436.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 436/439: CPaneFrameWnd |
| [`feature-pack-ui-437`](shards/feature-pack-ui-437.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 437/439: CPaneFrameWnd |
| [`feature-pack-ui-438`](shards/feature-pack-ui-438.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 438/439: CPaneFrameWnd |
| [`feature-pack-ui-439`](shards/feature-pack-ui-439.json) | `feature_pack_ui` | 10 | feature_pack_ui shard 439/439: CPaneFrameWnd |
| [`gdi-drawing-01`](shards/gdi-drawing-01.json) | `gdi_drawing` | 10 | gdi_drawing shard 01/24: AFX_GLOBAL_DATA, CCtrlView, CArchivePropExchange |
| [`gdi-drawing-02`](shards/gdi-drawing-02.json) | `gdi_drawing` | 10 | gdi_drawing shard 02/24: CD2DBitmap |
| [`gdi-drawing-03`](shards/gdi-drawing-03.json) | `gdi_drawing` | 10 | gdi_drawing shard 03/24: CD2DBitmapBrush, CD2DBitmap |
| [`gdi-drawing-04`](shards/gdi-drawing-04.json) | `gdi_drawing` | 10 | gdi_drawing shard 04/24: CD2DBrush, CD2DBitmapBrush |
| [`gdi-drawing-05`](shards/gdi-drawing-05.json) | `gdi_drawing` | 10 | gdi_drawing shard 05/24: CD2DLinearGradientBrush, CD2DRadialGradientBrush |
| [`gdi-drawing-06`](shards/gdi-drawing-06.json) | `gdi_drawing` | 10 | gdi_drawing shard 06/24: CD2DRadialGradientBrush |
| [`gdi-drawing-07`](shards/gdi-drawing-07.json) | `gdi_drawing` | 10 | gdi_drawing shard 07/24: CD2DSolidColorBrush, CDialogTemplate |
| [`gdi-drawing-08`](shards/gdi-drawing-08.json) | `gdi_drawing` | 10 | gdi_drawing shard 08/24: CDrawingManager, CDockBar, CDockContext |
| [`gdi-drawing-09`](shards/gdi-drawing-09.json) | `gdi_drawing` | 10 | gdi_drawing shard 09/24: CDrawingManager |
| [`gdi-drawing-10`](shards/gdi-drawing-10.json) | `gdi_drawing` | 10 | gdi_drawing shard 10/24: CDrawingManager, CHeaderCtrl |
| [`gdi-drawing-11`](shards/gdi-drawing-11.json) | `gdi_drawing` | 10 | gdi_drawing shard 11/24: CHtmlView, CMDITabProxyWnd, CMenuImages |
| [`gdi-drawing-12`](shards/gdi-drawing-12.json) | `gdi_drawing` | 10 | gdi_drawing shard 12/24: CPreviewView |
| [`gdi-drawing-13`](shards/gdi-drawing-13.json) | `gdi_drawing` | 10 | gdi_drawing shard 13/24: CPreviewView |
| [`gdi-drawing-14`](shards/gdi-drawing-14.json) | `gdi_drawing` | 10 | gdi_drawing shard 14/24: CPreviewView, CPropbagPropExchange, CPropsetPropExchange |
| [`gdi-drawing-15`](shards/gdi-drawing-15.json) | `gdi_drawing` | 10 | gdi_drawing shard 15/24: CRenderTarget, CReBar |
| [`gdi-drawing-16`](shards/gdi-drawing-16.json) | `gdi_drawing` | 10 | gdi_drawing shard 16/24: CRenderTarget, CRichEditView, CResetPropExchange |
| [`gdi-drawing-17`](shards/gdi-drawing-17.json) | `gdi_drawing` | 10 | gdi_drawing shard 17/24: CRichEditView |
| [`gdi-drawing-18`](shards/gdi-drawing-18.json) | `gdi_drawing` | 10 | gdi_drawing shard 18/24: CRichEditView |
| [`gdi-drawing-19`](shards/gdi-drawing-19.json) | `gdi_drawing` | 10 | gdi_drawing shard 19/24: CRichEditView |
| [`gdi-drawing-20`](shards/gdi-drawing-20.json) | `gdi_drawing` | 10 | gdi_drawing shard 20/24: CRichEditView |
| [`gdi-drawing-21`](shards/gdi-drawing-21.json) | `gdi_drawing` | 10 | gdi_drawing shard 21/24: CRichEditView |
| [`gdi-drawing-22`](shards/gdi-drawing-22.json) | `gdi_drawing` | 10 | gdi_drawing shard 22/24: CRichEditView, CSmartDockingGroupGuide, CSmartDockingGroupGuidesManager |
| [`gdi-drawing-23`](shards/gdi-drawing-23.json) | `gdi_drawing` | 10 | gdi_drawing shard 23/24: CSplitterWnd, CStatusBar, CToolBar |
| [`gdi-drawing-24`](shards/gdi-drawing-24.json) | `gdi_drawing` | 3 | gdi_drawing shard 24/24: CToolBar |
| [`global-residuals-01`](shards/global-residuals-01.json) | `global_residuals` | 10 | global_residuals shard 01/95: global |
| [`global-residuals-02`](shards/global-residuals-02.json) | `global_residuals` | 10 | global_residuals shard 02/95: global |
| [`global-residuals-03`](shards/global-residuals-03.json) | `global_residuals` | 10 | global_residuals shard 03/95: global |
| [`global-residuals-04`](shards/global-residuals-04.json) | `global_residuals` | 10 | global_residuals shard 04/95: global |
| [`global-residuals-05`](shards/global-residuals-05.json) | `global_residuals` | 10 | global_residuals shard 05/95: global |
| [`global-residuals-06`](shards/global-residuals-06.json) | `global_residuals` | 10 | global_residuals shard 06/95: global |
| [`global-residuals-07`](shards/global-residuals-07.json) | `global_residuals` | 10 | global_residuals shard 07/95: global |
| [`global-residuals-08`](shards/global-residuals-08.json) | `global_residuals` | 10 | global_residuals shard 08/95: global |
| [`global-residuals-09`](shards/global-residuals-09.json) | `global_residuals` | 10 | global_residuals shard 09/95: global |
| [`global-residuals-10`](shards/global-residuals-10.json) | `global_residuals` | 10 | global_residuals shard 10/95: global |
| [`global-residuals-11`](shards/global-residuals-11.json) | `global_residuals` | 10 | global_residuals shard 11/95: global |
| [`global-residuals-12`](shards/global-residuals-12.json) | `global_residuals` | 10 | global_residuals shard 12/95: global |
| [`global-residuals-13`](shards/global-residuals-13.json) | `global_residuals` | 10 | global_residuals shard 13/95: global |
| [`global-residuals-14`](shards/global-residuals-14.json) | `global_residuals` | 10 | global_residuals shard 14/95: global |
| [`global-residuals-15`](shards/global-residuals-15.json) | `global_residuals` | 10 | global_residuals shard 15/95: global |
| [`global-residuals-16`](shards/global-residuals-16.json) | `global_residuals` | 10 | global_residuals shard 16/95: global |
| [`global-residuals-17`](shards/global-residuals-17.json) | `global_residuals` | 10 | global_residuals shard 17/95: global |
| [`global-residuals-18`](shards/global-residuals-18.json) | `global_residuals` | 10 | global_residuals shard 18/95: global |
| [`global-residuals-19`](shards/global-residuals-19.json) | `global_residuals` | 10 | global_residuals shard 19/95: global |
| [`global-residuals-20`](shards/global-residuals-20.json) | `global_residuals` | 10 | global_residuals shard 20/95: global |
| [`global-residuals-21`](shards/global-residuals-21.json) | `global_residuals` | 10 | global_residuals shard 21/95: global |
| [`global-residuals-22`](shards/global-residuals-22.json) | `global_residuals` | 10 | global_residuals shard 22/95: global |
| [`global-residuals-23`](shards/global-residuals-23.json) | `global_residuals` | 10 | global_residuals shard 23/95: global |
| [`global-residuals-24`](shards/global-residuals-24.json) | `global_residuals` | 10 | global_residuals shard 24/95: global |
| [`global-residuals-25`](shards/global-residuals-25.json) | `global_residuals` | 10 | global_residuals shard 25/95: global |
| [`global-residuals-26`](shards/global-residuals-26.json) | `global_residuals` | 10 | global_residuals shard 26/95: global |
| [`global-residuals-27`](shards/global-residuals-27.json) | `global_residuals` | 10 | global_residuals shard 27/95: global |
| [`global-residuals-28`](shards/global-residuals-28.json) | `global_residuals` | 10 | global_residuals shard 28/95: global |
| [`global-residuals-29`](shards/global-residuals-29.json) | `global_residuals` | 10 | global_residuals shard 29/95: global |
| [`global-residuals-30`](shards/global-residuals-30.json) | `global_residuals` | 10 | global_residuals shard 30/95: global |
| [`global-residuals-31`](shards/global-residuals-31.json) | `global_residuals` | 10 | global_residuals shard 31/95: global |
| [`global-residuals-32`](shards/global-residuals-32.json) | `global_residuals` | 10 | global_residuals shard 32/95: global |
| [`global-residuals-33`](shards/global-residuals-33.json) | `global_residuals` | 10 | global_residuals shard 33/95: global |
| [`global-residuals-34`](shards/global-residuals-34.json) | `global_residuals` | 10 | global_residuals shard 34/95: global |
| [`global-residuals-35`](shards/global-residuals-35.json) | `global_residuals` | 10 | global_residuals shard 35/95: global |
| [`global-residuals-36`](shards/global-residuals-36.json) | `global_residuals` | 10 | global_residuals shard 36/95: global |
| [`global-residuals-37`](shards/global-residuals-37.json) | `global_residuals` | 10 | global_residuals shard 37/95: global |
| [`global-residuals-38`](shards/global-residuals-38.json) | `global_residuals` | 10 | global_residuals shard 38/95: global |
| [`global-residuals-39`](shards/global-residuals-39.json) | `global_residuals` | 10 | global_residuals shard 39/95: global |
| [`global-residuals-40`](shards/global-residuals-40.json) | `global_residuals` | 10 | global_residuals shard 40/95: global |
| [`global-residuals-41`](shards/global-residuals-41.json) | `global_residuals` | 10 | global_residuals shard 41/95: global |
| [`global-residuals-42`](shards/global-residuals-42.json) | `global_residuals` | 10 | global_residuals shard 42/95: global |
| [`global-residuals-43`](shards/global-residuals-43.json) | `global_residuals` | 10 | global_residuals shard 43/95: global |
| [`global-residuals-44`](shards/global-residuals-44.json) | `global_residuals` | 10 | global_residuals shard 44/95: global |
| [`global-residuals-45`](shards/global-residuals-45.json) | `global_residuals` | 10 | global_residuals shard 45/95: global |
| [`global-residuals-46`](shards/global-residuals-46.json) | `global_residuals` | 10 | global_residuals shard 46/95: global |
| [`global-residuals-47`](shards/global-residuals-47.json) | `global_residuals` | 10 | global_residuals shard 47/95: global |
| [`global-residuals-48`](shards/global-residuals-48.json) | `global_residuals` | 10 | global_residuals shard 48/95: global |
| [`global-residuals-49`](shards/global-residuals-49.json) | `global_residuals` | 10 | global_residuals shard 49/95: global |
| [`global-residuals-50`](shards/global-residuals-50.json) | `global_residuals` | 10 | global_residuals shard 50/95: global |
| [`global-residuals-51`](shards/global-residuals-51.json) | `global_residuals` | 10 | global_residuals shard 51/95: global |
| [`global-residuals-52`](shards/global-residuals-52.json) | `global_residuals` | 10 | global_residuals shard 52/95: global |
| [`global-residuals-53`](shards/global-residuals-53.json) | `global_residuals` | 10 | global_residuals shard 53/95: global |
| [`global-residuals-54`](shards/global-residuals-54.json) | `global_residuals` | 10 | global_residuals shard 54/95: global |
| [`global-residuals-55`](shards/global-residuals-55.json) | `global_residuals` | 10 | global_residuals shard 55/95: global |
| [`global-residuals-56`](shards/global-residuals-56.json) | `global_residuals` | 10 | global_residuals shard 56/95: global |
| [`global-residuals-57`](shards/global-residuals-57.json) | `global_residuals` | 10 | global_residuals shard 57/95: global |
| [`global-residuals-58`](shards/global-residuals-58.json) | `global_residuals` | 10 | global_residuals shard 58/95: global |
| [`global-residuals-59`](shards/global-residuals-59.json) | `global_residuals` | 10 | global_residuals shard 59/95: global |
| [`global-residuals-60`](shards/global-residuals-60.json) | `global_residuals` | 10 | global_residuals shard 60/95: global |
| [`global-residuals-61`](shards/global-residuals-61.json) | `global_residuals` | 10 | global_residuals shard 61/95: global |
| [`global-residuals-62`](shards/global-residuals-62.json) | `global_residuals` | 10 | global_residuals shard 62/95: global |
| [`global-residuals-63`](shards/global-residuals-63.json) | `global_residuals` | 10 | global_residuals shard 63/95: global |
| [`global-residuals-64`](shards/global-residuals-64.json) | `global_residuals` | 10 | global_residuals shard 64/95: global |
| [`global-residuals-65`](shards/global-residuals-65.json) | `global_residuals` | 10 | global_residuals shard 65/95: global |
| [`global-residuals-66`](shards/global-residuals-66.json) | `global_residuals` | 10 | global_residuals shard 66/95: global |
| [`global-residuals-67`](shards/global-residuals-67.json) | `global_residuals` | 10 | global_residuals shard 67/95: global |
| [`global-residuals-68`](shards/global-residuals-68.json) | `global_residuals` | 10 | global_residuals shard 68/95: global |
| [`global-residuals-69`](shards/global-residuals-69.json) | `global_residuals` | 10 | global_residuals shard 69/95: global |
| [`global-residuals-70`](shards/global-residuals-70.json) | `global_residuals` | 10 | global_residuals shard 70/95: global |
| [`global-residuals-71`](shards/global-residuals-71.json) | `global_residuals` | 10 | global_residuals shard 71/95: global |
| [`global-residuals-72`](shards/global-residuals-72.json) | `global_residuals` | 10 | global_residuals shard 72/95: global |
| [`global-residuals-73`](shards/global-residuals-73.json) | `global_residuals` | 10 | global_residuals shard 73/95: global |
| [`global-residuals-74`](shards/global-residuals-74.json) | `global_residuals` | 10 | global_residuals shard 74/95: global |
| [`global-residuals-75`](shards/global-residuals-75.json) | `global_residuals` | 10 | global_residuals shard 75/95: global |
| [`global-residuals-76`](shards/global-residuals-76.json) | `global_residuals` | 10 | global_residuals shard 76/95: global |
| [`global-residuals-77`](shards/global-residuals-77.json) | `global_residuals` | 10 | global_residuals shard 77/95: global |
| [`global-residuals-78`](shards/global-residuals-78.json) | `global_residuals` | 10 | global_residuals shard 78/95: global |
| [`global-residuals-79`](shards/global-residuals-79.json) | `global_residuals` | 10 | global_residuals shard 79/95: global |
| [`global-residuals-80`](shards/global-residuals-80.json) | `global_residuals` | 10 | global_residuals shard 80/95: global |
| [`global-residuals-81`](shards/global-residuals-81.json) | `global_residuals` | 10 | global_residuals shard 81/95: global |
| [`global-residuals-82`](shards/global-residuals-82.json) | `global_residuals` | 10 | global_residuals shard 82/95: global |
| [`global-residuals-83`](shards/global-residuals-83.json) | `global_residuals` | 10 | global_residuals shard 83/95: global |
| [`global-residuals-84`](shards/global-residuals-84.json) | `global_residuals` | 10 | global_residuals shard 84/95: global |
| [`global-residuals-85`](shards/global-residuals-85.json) | `global_residuals` | 10 | global_residuals shard 85/95: global |
| [`global-residuals-86`](shards/global-residuals-86.json) | `global_residuals` | 10 | global_residuals shard 86/95: global |
| [`global-residuals-87`](shards/global-residuals-87.json) | `global_residuals` | 10 | global_residuals shard 87/95: global |
| [`global-residuals-88`](shards/global-residuals-88.json) | `global_residuals` | 10 | global_residuals shard 88/95: global |
| [`global-residuals-89`](shards/global-residuals-89.json) | `global_residuals` | 10 | global_residuals shard 89/95: global |
| [`global-residuals-90`](shards/global-residuals-90.json) | `global_residuals` | 10 | global_residuals shard 90/95: global |
| [`global-residuals-91`](shards/global-residuals-91.json) | `global_residuals` | 10 | global_residuals shard 91/95: global |
| [`global-residuals-92`](shards/global-residuals-92.json) | `global_residuals` | 10 | global_residuals shard 92/95: global |
| [`global-residuals-93`](shards/global-residuals-93.json) | `global_residuals` | 10 | global_residuals shard 93/95: global |
| [`global-residuals-94`](shards/global-residuals-94.json) | `global_residuals` | 10 | global_residuals shard 94/95: global |
| [`global-residuals-95`](shards/global-residuals-95.json) | `global_residuals` | 2 | global_residuals shard 95/95: global |
| [`ole-com-activex-01`](shards/ole-com-activex-01.json) | `ole_com_activex` | 10 | ole_com_activex shard 01/77: CCmdTarget, CBrowserControlSite |
| [`ole-com-activex-02`](shards/ole-com-activex-02.json) | `ole_com_activex` | 10 | ole_com_activex shard 02/77: CCmdTarget |
| [`ole-com-activex-03`](shards/ole-com-activex-03.json) | `ole_com_activex` | 10 | ole_com_activex shard 03/77: CCmdTarget |
| [`ole-com-activex-04`](shards/ole-com-activex-04.json) | `ole_com_activex` | 10 | ole_com_activex shard 04/77: CCmdTarget |
| [`ole-com-activex-05`](shards/ole-com-activex-05.json) | `ole_com_activex` | 10 | ole_com_activex shard 05/77: CCmdTarget |
| [`ole-com-activex-06`](shards/ole-com-activex-06.json) | `ole_com_activex` | 10 | ole_com_activex shard 06/77: CCmdTarget, CDataBoundProperty, CDataPathProperty |
| [`ole-com-activex-07`](shards/ole-com-activex-07.json) | `ole_com_activex` | 10 | ole_com_activex shard 07/77: CDocObjectServer, CDateTimeCtrl |
| [`ole-com-activex-08`](shards/ole-com-activex-08.json) | `ole_com_activex` | 10 | ole_com_activex shard 08/77: CDocObjectServer, CHtmlView |
| [`ole-com-activex-09`](shards/ole-com-activex-09.json) | `ole_com_activex` | 10 | ole_com_activex shard 09/77: CMonthCalCtrl, CHtmlView |
| [`ole-com-activex-10`](shards/ole-com-activex-10.json) | `ole_com_activex` | 10 | ole_com_activex shard 10/77: COccManager, COleClientItem, CMonthCalCtrl |
| [`ole-com-activex-11`](shards/ole-com-activex-11.json) | `ole_com_activex` | 10 | ole_com_activex shard 11/77: COleClientItem |
| [`ole-com-activex-12`](shards/ole-com-activex-12.json) | `ole_com_activex` | 10 | ole_com_activex shard 12/77: COleClientItem |
| [`ole-com-activex-13`](shards/ole-com-activex-13.json) | `ole_com_activex` | 10 | ole_com_activex shard 13/77: COleClientItem |
| [`ole-com-activex-14`](shards/ole-com-activex-14.json) | `ole_com_activex` | 10 | ole_com_activex shard 14/77: COleClientItem |
| [`ole-com-activex-15`](shards/ole-com-activex-15.json) | `ole_com_activex` | 10 | ole_com_activex shard 15/77: COleClientItem |
| [`ole-com-activex-16`](shards/ole-com-activex-16.json) | `ole_com_activex` | 10 | ole_com_activex shard 16/77: COleClientItem, COleCntrFrameWndEx |
| [`ole-com-activex-17`](shards/ole-com-activex-17.json) | `ole_com_activex` | 10 | ole_com_activex shard 17/77: COleCntrFrameWndEx |
| [`ole-com-activex-18`](shards/ole-com-activex-18.json) | `ole_com_activex` | 10 | ole_com_activex shard 18/77: COleCntrFrameWndEx |
| [`ole-com-activex-19`](shards/ole-com-activex-19.json) | `ole_com_activex` | 10 | ole_com_activex shard 19/77: COleControl, COleCntrFrameWndEx |
| [`ole-com-activex-20`](shards/ole-com-activex-20.json) | `ole_com_activex` | 10 | ole_com_activex shard 20/77: COleControl |
| [`ole-com-activex-21`](shards/ole-com-activex-21.json) | `ole_com_activex` | 10 | ole_com_activex shard 21/77: COleControl |
| [`ole-com-activex-22`](shards/ole-com-activex-22.json) | `ole_com_activex` | 10 | ole_com_activex shard 22/77: COleControl |
| [`ole-com-activex-23`](shards/ole-com-activex-23.json) | `ole_com_activex` | 10 | ole_com_activex shard 23/77: COleControl |
| [`ole-com-activex-24`](shards/ole-com-activex-24.json) | `ole_com_activex` | 10 | ole_com_activex shard 24/77: COleControl |
| [`ole-com-activex-25`](shards/ole-com-activex-25.json) | `ole_com_activex` | 10 | ole_com_activex shard 25/77: COleControl |
| [`ole-com-activex-26`](shards/ole-com-activex-26.json) | `ole_com_activex` | 10 | ole_com_activex shard 26/77: COleControl |
| [`ole-com-activex-27`](shards/ole-com-activex-27.json) | `ole_com_activex` | 10 | ole_com_activex shard 27/77: COleControl |
| [`ole-com-activex-28`](shards/ole-com-activex-28.json) | `ole_com_activex` | 10 | ole_com_activex shard 28/77: COleControl |
| [`ole-com-activex-29`](shards/ole-com-activex-29.json) | `ole_com_activex` | 10 | ole_com_activex shard 29/77: COleControl |
| [`ole-com-activex-30`](shards/ole-com-activex-30.json) | `ole_com_activex` | 10 | ole_com_activex shard 30/77: COleControl |
| [`ole-com-activex-31`](shards/ole-com-activex-31.json) | `ole_com_activex` | 10 | ole_com_activex shard 31/77: COleControl |
| [`ole-com-activex-32`](shards/ole-com-activex-32.json) | `ole_com_activex` | 10 | ole_com_activex shard 32/77: COleControl |
| [`ole-com-activex-33`](shards/ole-com-activex-33.json) | `ole_com_activex` | 10 | ole_com_activex shard 33/77: COleControl |
| [`ole-com-activex-34`](shards/ole-com-activex-34.json) | `ole_com_activex` | 10 | ole_com_activex shard 34/77: COleControl |
| [`ole-com-activex-35`](shards/ole-com-activex-35.json) | `ole_com_activex` | 10 | ole_com_activex shard 35/77: COleControl |
| [`ole-com-activex-36`](shards/ole-com-activex-36.json) | `ole_com_activex` | 10 | ole_com_activex shard 36/77: COleControl |
| [`ole-com-activex-37`](shards/ole-com-activex-37.json) | `ole_com_activex` | 10 | ole_com_activex shard 37/77: COleControl, COleControlContainer |
| [`ole-com-activex-38`](shards/ole-com-activex-38.json) | `ole_com_activex` | 10 | ole_com_activex shard 38/77: COleControlContainer |
| [`ole-com-activex-39`](shards/ole-com-activex-39.json) | `ole_com_activex` | 10 | ole_com_activex shard 39/77: COleControlContainer |
| [`ole-com-activex-40`](shards/ole-com-activex-40.json) | `ole_com_activex` | 10 | ole_com_activex shard 40/77: COleControlContainer, COleControlSite |
| [`ole-com-activex-41`](shards/ole-com-activex-41.json) | `ole_com_activex` | 10 | ole_com_activex shard 41/77: COleControlSite |
| [`ole-com-activex-42`](shards/ole-com-activex-42.json) | `ole_com_activex` | 10 | ole_com_activex shard 42/77: COleControlSite |
| [`ole-com-activex-43`](shards/ole-com-activex-43.json) | `ole_com_activex` | 10 | ole_com_activex shard 43/77: COleControlSite |
| [`ole-com-activex-44`](shards/ole-com-activex-44.json) | `ole_com_activex` | 10 | ole_com_activex shard 44/77: COleControlSite, COleDataSource |
| [`ole-com-activex-45`](shards/ole-com-activex-45.json) | `ole_com_activex` | 10 | ole_com_activex shard 45/77: COleDispatchImpl, COleDataSource, COleDocIPFrameWnd |
| [`ole-com-activex-46`](shards/ole-com-activex-46.json) | `ole_com_activex` | 10 | ole_com_activex shard 46/77: COleDocIPFrameWnd, COleDocIPFrameWndEx |
| [`ole-com-activex-47`](shards/ole-com-activex-47.json) | `ole_com_activex` | 10 | ole_com_activex shard 47/77: COleDocIPFrameWndEx |
| [`ole-com-activex-48`](shards/ole-com-activex-48.json) | `ole_com_activex` | 10 | ole_com_activex shard 48/77: COleDocIPFrameWndEx |
| [`ole-com-activex-49`](shards/ole-com-activex-49.json) | `ole_com_activex` | 10 | ole_com_activex shard 49/77: COleDocIPFrameWndEx |
| [`ole-com-activex-50`](shards/ole-com-activex-50.json) | `ole_com_activex` | 10 | ole_com_activex shard 50/77: COleDocIPFrameWndEx, COleDocObjectItem |
| [`ole-com-activex-51`](shards/ole-com-activex-51.json) | `ole_com_activex` | 10 | ole_com_activex shard 51/77: COleDocObjectItem |
| [`ole-com-activex-52`](shards/ole-com-activex-52.json) | `ole_com_activex` | 10 | ole_com_activex shard 52/77: COleDocument |
| [`ole-com-activex-53`](shards/ole-com-activex-53.json) | `ole_com_activex` | 10 | ole_com_activex shard 53/77: COleDocument |
| [`ole-com-activex-54`](shards/ole-com-activex-54.json) | `ole_com_activex` | 10 | ole_com_activex shard 54/77: COleDocument, COleFrameHook |
| [`ole-com-activex-55`](shards/ole-com-activex-55.json) | `ole_com_activex` | 10 | ole_com_activex shard 55/77: COleFrameHook |
| [`ole-com-activex-56`](shards/ole-com-activex-56.json) | `ole_com_activex` | 10 | ole_com_activex shard 56/77: COleIPFrameWnd |
| [`ole-com-activex-57`](shards/ole-com-activex-57.json) | `ole_com_activex` | 10 | ole_com_activex shard 57/77: COleIPFrameWnd, COleIPFrameWndEx |
| [`ole-com-activex-58`](shards/ole-com-activex-58.json) | `ole_com_activex` | 10 | ole_com_activex shard 58/77: COleIPFrameWndEx |
| [`ole-com-activex-59`](shards/ole-com-activex-59.json) | `ole_com_activex` | 10 | ole_com_activex shard 59/77: COleIPFrameWndEx |
| [`ole-com-activex-60`](shards/ole-com-activex-60.json) | `ole_com_activex` | 10 | ole_com_activex shard 60/77: COleIPFrameWndEx |
| [`ole-com-activex-61`](shards/ole-com-activex-61.json) | `ole_com_activex` | 10 | ole_com_activex shard 61/77: COleIPFrameWndEx |
| [`ole-com-activex-62`](shards/ole-com-activex-62.json) | `ole_com_activex` | 10 | ole_com_activex shard 62/77: COleLinkingDoc, COleIPFrameWndEx |
| [`ole-com-activex-63`](shards/ole-com-activex-63.json) | `ole_com_activex` | 10 | ole_com_activex shard 63/77: COleLinkingDoc, COlePropertyPage |
| [`ole-com-activex-64`](shards/ole-com-activex-64.json) | `ole_com_activex` | 10 | ole_com_activex shard 64/77: COlePropertyPage |
| [`ole-com-activex-65`](shards/ole-com-activex-65.json) | `ole_com_activex` | 10 | ole_com_activex shard 65/77: COlePropertyPage |
| [`ole-com-activex-66`](shards/ole-com-activex-66.json) | `ole_com_activex` | 10 | ole_com_activex shard 66/77: COlePropertyPage |
| [`ole-com-activex-67`](shards/ole-com-activex-67.json) | `ole_com_activex` | 10 | ole_com_activex shard 67/77: COlePropertyPage, COleResizeBar |
| [`ole-com-activex-68`](shards/ole-com-activex-68.json) | `ole_com_activex` | 10 | ole_com_activex shard 68/77: COleServerDoc, COleResizeBar |
| [`ole-com-activex-69`](shards/ole-com-activex-69.json) | `ole_com_activex` | 10 | ole_com_activex shard 69/77: COleServerDoc |
| [`ole-com-activex-70`](shards/ole-com-activex-70.json) | `ole_com_activex` | 10 | ole_com_activex shard 70/77: COleServerDoc |
| [`ole-com-activex-71`](shards/ole-com-activex-71.json) | `ole_com_activex` | 10 | ole_com_activex shard 71/77: COleServerDoc |
| [`ole-com-activex-72`](shards/ole-com-activex-72.json) | `ole_com_activex` | 10 | ole_com_activex shard 72/77: COleServerDoc, COleServerDocEx |
| [`ole-com-activex-73`](shards/ole-com-activex-73.json) | `ole_com_activex` | 10 | ole_com_activex shard 73/77: COleServerDocEx, COleServerItem |
| [`ole-com-activex-74`](shards/ole-com-activex-74.json) | `ole_com_activex` | 10 | ole_com_activex shard 74/77: COleServerItem |
| [`ole-com-activex-75`](shards/ole-com-activex-75.json) | `ole_com_activex` | 10 | ole_com_activex shard 75/77: COleServerItem |
| [`ole-com-activex-76`](shards/ole-com-activex-76.json) | `ole_com_activex` | 10 | ole_com_activex shard 76/77: COleServerItem, COleUILinkInfo |
| [`ole-com-activex-77`](shards/ole-com-activex-77.json) | `ole_com_activex` | 7 | ole_com_activex shard 77/77: COleUILinkInfo |
| [`other-mfc-01`](shards/other-mfc-01.json) | `other_mfc` | 10 | other_mfc shard 01/91: AFX_GLOBAL_DATA |
| [`other-mfc-02`](shards/other-mfc-02.json) | `other_mfc` | 10 | other_mfc shard 02/91: AFX_GLOBAL_DATA, CAfxStringMgr |
| [`other-mfc-03`](shards/other-mfc-03.json) | `other_mfc` | 10 | other_mfc shard 03/91: CAnimationBaseObject, CAfxStringMgr, CAnimationColor |
| [`other-mfc-04`](shards/other-mfc-04.json) | `other_mfc` | 10 | other_mfc shard 04/91: CAnimationGroup, CAnimationColor |
| [`other-mfc-05`](shards/other-mfc-05.json) | `other_mfc` | 10 | other_mfc shard 05/91: CAnimationPoint, CAnimationGroup, CAnimationRect |
| [`other-mfc-06`](shards/other-mfc-06.json) | `other_mfc` | 10 | other_mfc shard 06/91: CAnimationSize, CAnimationRect, CArchivePropExchange |
| [`other-mfc-07`](shards/other-mfc-07.json) | `other_mfc` | 10 | other_mfc shard 07/91: CArchiveStream, CArchivePropExchange |
| [`other-mfc-08`](shards/other-mfc-08.json) | `other_mfc` | 10 | other_mfc shard 08/91: CArchiveStream, CAsyncMonikerFile |
| [`other-mfc-09`](shards/other-mfc-09.json) | `other_mfc` | 10 | other_mfc shard 09/91: CAsyncMonikerFile |
| [`other-mfc-10`](shards/other-mfc-10.json) | `other_mfc` | 10 | other_mfc shard 10/91: CAsyncMonikerFile, CAsyncPropExchange, CAutoHideDockSite |
| [`other-mfc-11`](shards/other-mfc-11.json) | `other_mfc` | 10 | other_mfc shard 11/91: CAutoHideDockSite, CBlobProperty |
| [`other-mfc-12`](shards/other-mfc-12.json) | `other_mfc` | 10 | other_mfc shard 12/91: CBlobProperty, CBrowserControlSite |
| [`other-mfc-13`](shards/other-mfc-13.json) | `other_mfc` | 10 | other_mfc shard 13/91: CBrowserControlSite |
| [`other-mfc-14`](shards/other-mfc-14.json) | `other_mfc` | 10 | other_mfc shard 14/91: CComboBoxEx, CBrowserControlSite, CCommandManager |
| [`other-mfc-15`](shards/other-mfc-15.json) | `other_mfc` | 10 | other_mfc shard 15/91: CCommandManager, CConnectionPoint |
| [`other-mfc-16`](shards/other-mfc-16.json) | `other_mfc` | 10 | other_mfc shard 16/91: CConnectionPoint, CControlFrameWnd |
| [`other-mfc-17`](shards/other-mfc-17.json) | `other_mfc` | 10 | other_mfc shard 17/91: CD2DGeometry, CCtrlView, CControlFrameWnd |
| [`other-mfc-18`](shards/other-mfc-18.json) | `other_mfc` | 10 | other_mfc shard 18/91: CD2DGeometry, CD2DGeometrySink |
| [`other-mfc-19`](shards/other-mfc-19.json) | `other_mfc` | 10 | other_mfc shard 19/91: CD2DGeometrySink |
| [`other-mfc-20`](shards/other-mfc-20.json) | `other_mfc` | 10 | other_mfc shard 20/91: CD2DLayer, CD2DMesh, CD2DGeometrySink |
| [`other-mfc-21`](shards/other-mfc-21.json) | `other_mfc` | 10 | other_mfc shard 21/91: CD2DPathGeometry, CD2DMesh, CDHtmlControlSink |
| [`other-mfc-22`](shards/other-mfc-22.json) | `other_mfc` | 10 | other_mfc shard 22/91: CDHtmlControlSink |
| [`other-mfc-23`](shards/other-mfc-23.json) | `other_mfc` | 10 | other_mfc shard 23/91: CDHtmlDialog |
| [`other-mfc-24`](shards/other-mfc-24.json) | `other_mfc` | 10 | other_mfc shard 24/91: CDHtmlDialog |
| [`other-mfc-25`](shards/other-mfc-25.json) | `other_mfc` | 10 | other_mfc shard 25/91: CDHtmlDialog |
| [`other-mfc-26`](shards/other-mfc-26.json) | `other_mfc` | 10 | other_mfc shard 26/91: CDHtmlDialog, CDHtmlElementEventSink |
| [`other-mfc-27`](shards/other-mfc-27.json) | `other_mfc` | 10 | other_mfc shard 27/91: CDHtmlElementEventSink, CDHtmlEventSink |
| [`other-mfc-28`](shards/other-mfc-28.json) | `other_mfc` | 10 | other_mfc shard 28/91: CDHtmlEventSink, CDataBoundProperty |
| [`other-mfc-29`](shards/other-mfc-29.json) | `other_mfc` | 10 | other_mfc shard 29/91: CDataRecoveryHandler, CDataPathProperty, CDataBoundProperty |
| [`other-mfc-30`](shards/other-mfc-30.json) | `other_mfc` | 10 | other_mfc shard 30/91: CDataRecoveryHandler, CDataSourceControl |
| [`other-mfc-31`](shards/other-mfc-31.json) | `other_mfc` | 10 | other_mfc shard 31/91: CDataSourceControl, CDateTimeCtrl |
| [`other-mfc-32`](shards/other-mfc-32.json) | `other_mfc` | 10 | other_mfc shard 32/91: CDialogImpl, CDateTimeCtrl |
| [`other-mfc-33`](shards/other-mfc-33.json) | `other_mfc` | 10 | other_mfc shard 33/91: CDialogTemplate, CDialogImpl, CDockBar |
| [`other-mfc-34`](shards/other-mfc-34.json) | `other_mfc` | 10 | other_mfc shard 34/91: CDockBar |
| [`other-mfc-35`](shards/other-mfc-35.json) | `other_mfc` | 10 | other_mfc shard 35/91: CDockBar, CDockContext |
| [`other-mfc-36`](shards/other-mfc-36.json) | `other_mfc` | 10 | other_mfc shard 36/91: CDockContext |
| [`other-mfc-37`](shards/other-mfc-37.json) | `other_mfc` | 10 | other_mfc shard 37/91: CDockState, CDockContext |
| [`other-mfc-38`](shards/other-mfc-38.json) | `other_mfc` | 10 | other_mfc shard 38/91: CDockablePaneAdapter, CEnumArray, CDockState |
| [`other-mfc-39`](shards/other-mfc-39.json) | `other_mfc` | 10 | other_mfc shard 39/91: CGestureConfig, CHeaderCtrl, CEnumArray |
| [`other-mfc-40`](shards/other-mfc-40.json) | `other_mfc` | 10 | other_mfc shard 40/91: CHtmlEditCtrl, CHeaderCtrl, CHtmlEditDoc |
| [`other-mfc-41`](shards/other-mfc-41.json) | `other_mfc` | 10 | other_mfc shard 41/91: CHtmlEditDoc, CHtmlEditView |
| [`other-mfc-42`](shards/other-mfc-42.json) | `other_mfc` | 10 | other_mfc shard 42/91: CHtmlView, CHtmlEditView |
| [`other-mfc-43`](shards/other-mfc-43.json) | `other_mfc` | 10 | other_mfc shard 43/91: CHtmlView |
| [`other-mfc-44`](shards/other-mfc-44.json) | `other_mfc` | 10 | other_mfc shard 44/91: CHtmlView |
| [`other-mfc-45`](shards/other-mfc-45.json) | `other_mfc` | 10 | other_mfc shard 45/91: CHwndRenderTarget, CJumpList, CHtmlView |
| [`other-mfc-46`](shards/other-mfc-46.json) | `other_mfc` | 10 | other_mfc shard 46/91: CJumpList |
| [`other-mfc-47`](shards/other-mfc-47.json) | `other_mfc` | 10 | other_mfc shard 47/91: CLinkCtrl, CMDITabProxyWnd, CJumpList |
| [`other-mfc-48`](shards/other-mfc-48.json) | `other_mfc` | 10 | other_mfc shard 48/91: CMenuImages, CMDITabProxyWnd |
| [`other-mfc-49`](shards/other-mfc-49.json) | `other_mfc` | 10 | other_mfc shard 49/91: CMenuImages, CMetaFileDC |
| [`other-mfc-50`](shards/other-mfc-50.json) | `other_mfc` | 10 | other_mfc shard 50/91: CMetaFileDC |
| [`other-mfc-51`](shards/other-mfc-51.json) | `other_mfc` | 10 | other_mfc shard 51/91: CMiniDockFrameWnd, CMiniFrameWnd |
| [`other-mfc-52`](shards/other-mfc-52.json) | `other_mfc` | 10 | other_mfc shard 52/91: CMiniFrameWnd, CMonikerFile |
| [`other-mfc-53`](shards/other-mfc-53.json) | `other_mfc` | 10 | other_mfc shard 53/91: CMonikerFile |
| [`other-mfc-54`](shards/other-mfc-54.json) | `other_mfc` | 10 | other_mfc shard 54/91: CMonthCalCtrl, CMonikerFile |
| [`other-mfc-55`](shards/other-mfc-55.json) | `other_mfc` | 10 | other_mfc shard 55/91: CMonthCalCtrl |
| [`other-mfc-56`](shards/other-mfc-56.json) | `other_mfc` | 10 | other_mfc shard 56/91: CMouseManager, CMonthCalCtrl |
| [`other-mfc-57`](shards/other-mfc-57.json) | `other_mfc` | 10 | other_mfc shard 57/91: CMouseManager, CMultiPageDHtmlDialog, COccManager |
| [`other-mfc-58`](shards/other-mfc-58.json) | `other_mfc` | 10 | other_mfc shard 58/91: COccManager |
| [`other-mfc-59`](shards/other-mfc-59.json) | `other_mfc` | 10 | other_mfc shard 59/91: COutlookOptionsDlg, COccManager |
| [`other-mfc-60`](shards/other-mfc-60.json) | `other_mfc` | 10 | other_mfc shard 60/91: CPaneDialog, COutlookOptionsDlg |
| [`other-mfc-61`](shards/other-mfc-61.json) | `other_mfc` | 10 | other_mfc shard 61/91: CPaneDialog, CPreviewViewEx |
| [`other-mfc-62`](shards/other-mfc-62.json) | `other_mfc` | 10 | other_mfc shard 62/91: CPrintDialogEx, CPreviewViewEx |
| [`other-mfc-63`](shards/other-mfc-63.json) | `other_mfc` | 10 | other_mfc shard 63/91: CPrintDialogEx, CPropbagPropExchange |
| [`other-mfc-64`](shards/other-mfc-64.json) | `other_mfc` | 10 | other_mfc shard 64/91: CProperty |
| [`other-mfc-65`](shards/other-mfc-65.json) | `other_mfc` | 10 | other_mfc shard 65/91: CProperty, CPropertySection |
| [`other-mfc-66`](shards/other-mfc-66.json) | `other_mfc` | 10 | other_mfc shard 66/91: CPropertySection |
| [`other-mfc-67`](shards/other-mfc-67.json) | `other_mfc` | 10 | other_mfc shard 67/91: CPropertySection, CPropertySet |
| [`other-mfc-68`](shards/other-mfc-68.json) | `other_mfc` | 10 | other_mfc shard 68/91: CPropertySet |
| [`other-mfc-69`](shards/other-mfc-69.json) | `other_mfc` | 10 | other_mfc shard 69/91: CPropertySet |
| [`other-mfc-70`](shards/other-mfc-70.json) | `other_mfc` | 10 | other_mfc shard 70/91: CReBar, CPropsetPropExchange, CPropertySet |
| [`other-mfc-71`](shards/other-mfc-71.json) | `other_mfc` | 10 | other_mfc shard 71/91: CReBar, CReBarCtrl |
| [`other-mfc-72`](shards/other-mfc-72.json) | `other_mfc` | 10 | other_mfc shard 72/91: CRecentDockSiteInfo, CReBarCtrl |
| [`other-mfc-73`](shards/other-mfc-73.json) | `other_mfc` | 10 | other_mfc shard 73/91: CRecentDockSiteInfo, CReflectorWnd |
| [`other-mfc-74`](shards/other-mfc-74.json) | `other_mfc` | 10 | other_mfc shard 74/91: CRenderTarget |
| [`other-mfc-75`](shards/other-mfc-75.json) | `other_mfc` | 10 | other_mfc shard 75/91: CRenderTarget, CResetPropExchange |
| [`other-mfc-76`](shards/other-mfc-76.json) | `other_mfc` | 10 | other_mfc shard 76/91: CRichEditCntrItem, CRichEditDoc |
| [`other-mfc-77`](shards/other-mfc-77.json) | `other_mfc` | 10 | other_mfc shard 77/91: CRichEditDoc |
| [`other-mfc-78`](shards/other-mfc-78.json) | `other_mfc` | 10 | other_mfc shard 78/91: CScreenWnd, CRichEditDoc, CSmartDockingGroupGuide |
| [`other-mfc-79`](shards/other-mfc-79.json) | `other_mfc` | 10 | other_mfc shard 79/91: CSmartDockingGroupGuide, CSmartDockingGroupGuidesManager |
| [`other-mfc-80`](shards/other-mfc-80.json) | `other_mfc` | 10 | other_mfc shard 80/91: CSmartDockingGroupGuidesManager, CSmartDockingGroupGuidesWnd, CSmartDockingHighlighterWnd |
| [`other-mfc-81`](shards/other-mfc-81.json) | `other_mfc` | 10 | other_mfc shard 81/91: CSmartDockingHighlighterWnd, CSmartDockingManager |
| [`other-mfc-82`](shards/other-mfc-82.json) | `other_mfc` | 10 | other_mfc shard 82/91: CSmartDockingManager |
| [`other-mfc-83`](shards/other-mfc-83.json) | `other_mfc` | 10 | other_mfc shard 83/91: CSmartDockingStandaloneGuide, CSmartDockingManager |
| [`other-mfc-84`](shards/other-mfc-84.json) | `other_mfc` | 10 | other_mfc shard 84/91: CSmartDockingStandaloneGuideWnd, CSmartDockingStandaloneGuide |
| [`other-mfc-85`](shards/other-mfc-85.json) | `other_mfc` | 10 | other_mfc shard 85/91: CSplitterWnd, CSmartDockingStandaloneGuideWnd |
| [`other-mfc-86`](shards/other-mfc-86.json) | `other_mfc` | 10 | other_mfc shard 86/91: CSplitterWnd |
| [`other-mfc-87`](shards/other-mfc-87.json) | `other_mfc` | 10 | other_mfc shard 87/91: CSplitterWnd |
| [`other-mfc-88`](shards/other-mfc-88.json) | `other_mfc` | 10 | other_mfc shard 88/91: CSplitterWnd, CStatusBar |
| [`other-mfc-89`](shards/other-mfc-89.json) | `other_mfc` | 10 | other_mfc shard 89/91: CStatusBar |
| [`other-mfc-90`](shards/other-mfc-90.json) | `other_mfc` | 10 | other_mfc shard 90/91: CStatusBar, CTabView |
| [`other-mfc-91`](shards/other-mfc-91.json) | `other_mfc` | 3 | other_mfc shard 91/91: CTabView |
