# OpenMFC Implementation Progress

> Auto-generated snapshot — run `python3 tools/gen_thunks.py ...` and rebuild to refresh counts.

## Overall ABI Coverage

| Category | Count | % of Total |
|----------|-------|------------|
| **Total MSVC exports** | 14,103 | 100% |
| **Real implementations** | 1,428 | 10.1% |
| ├─ Auto-generated thunks (`thunks.cpp`) | 1,095 | 7.8% |
| └─ Manual implementations (other `phase4/src/*.cpp`) | 333 | 2.4% |
| **Skip list** (signature mismatch, return defaults) | 157 | 1.1% |
| **Remaining stubs** (return 0/nullptr/void) | ~12,518 | 88.8% |

## Per-File Breakdown

| Source file | Implemented symbols |
|-------------|-------------------|
| `thunks.cpp` (auto-generated) | 988 |
| `docview.cpp` | 81 |
| `ctrlcore.cpp` | 69 |
| `gdicore.cpp` | 48 |
| `menucore.cpp` | 36 |
| `wincore.cpp` | 25 |
| `appcore.cpp` | 20 |
| `cobject_impl.cpp` | 15 |
| `dlgcore.cpp` | 15 |
| `regcore.cpp` | 11 |
| `memcore.cpp` | 9 |
| `strcore.cpp` | 6 |
| `mfc_exceptions.cpp` | 2 |

## Per-Class Implementation Counts

### Fully/mostly implemented classes
| Class | Implemented | Skip list |
|-------|-------------|-----------|
| **CWnd** | 200 | 33 |
| **CFrameWnd** | 110 | 0 |
| **CMFCVisualManager** | 65 | 8 |
| **COleControl** | 50 | 0 |
| **CMenu** | 31 | 0 |
| **COleClientItem** | 30 | 3 |
| **CDocument** | 25 | 0 |
| **CDC** | 22 | 0 |
| **CEdit** | 16 | 0 |
| **CListBox** | 15 | 0 |
| **CComboBox** | 15 | 0 |
| **CSplitterWnd** | 15 | 0 |
| **CPropertySheet** | 14 | 0 |
| **CPropertyPage** | 14 | 0 |
| **CToolBar** | 14 | 0 |
| **CAsyncSocket** | 13 | 0 |
| **CHtmlView** | 13 | 5 |
| **CRichEditCtrl** | 11 | 8 |
| **CButton** | 10 | 0 |
| **COleDataSource** | 10 | 0 |

### Partially implemented classes (5–9 symbols)
| Class | Implemented | Skip list |
|-------|-------------|-----------|
| CDialog | 9 | 0 |
| COleDataObject | 9 | 0 |
| CStatusBar | 9 | 0 |
| CWinThread | 8 | 0 |
| CRuntimeClass | 8 | 0 |
| CView | 8 | 5 |
| CMemFile | 8 | 0 |
| COleStreamFile | 8 | 0 |
| CFile | 8 | 4 |
| CFileFind | 8 | 3 |
| CInternetSession | 8 | 0 |
| CDHtmlDialog | 8 | 0 |
| COleObjectFactory | 8 | 0 |
| COleDropTarget | 8 | 0 |
| CScrollBar | 7 | 0 |
| CScrollView | 7 | 0 |
| CArchive | 7 | 0 |
| CFrameWnd | 7 | 0 |
| CMFCRibbonBar | 7 | 0 |
| CMFCPropertyGridCtrl | 7 | 0 |
| CDatabase | 7 | 0 |
| CStdioFile | 7 | 0 |
| CTaskDialog | 7 | 4 |
| CRichEditView | 7 | 0 |
| CStatic | 6 | 0 |
| CSocket | 6 | 0 |
| COleMessageFilter | 6 | 0 |
| CFtpConnection | 6 | 0 |
| COleDocument | 6 | 4 |
| CObject | 5 | 0 |
| CFormView | 5 | 0 |
| COleServerItem | 5 | 0 |
| CMFCToolBar | 5 | 0 |
| COleControlSite | 5 | 3 |
| CHttpFile | 5 | 6 |
| CInternetFile | 5 | 0 |

### Classes with 1–4 implementations
CSpinButtonCtrl, CListCtrl, CTreeCtrl, CTabCtrl, CSliderCtrl, CProgressCtrl, CWinApp, CHeaderCtrl, CMFCToolBarButton, CEditView, CListView, CTreeView, CMultiDocTemplate, CSingleDocTemplate, CGopherConnection, CHttpConnection, CGopherFile, CFtpFileFind, CInternetException, CFontDialog, CColorDialog, CPrintDialog, CPageSetupDialog, CFindReplaceDialog, CFieldExchange, CRecordView, CControlBar, CToolTipCtrl, CMFCTasksPane, CRibbonPanel, CDockingManager, CBasePane, CPaneFrameWnd, CMFCBaseTabCtrl, CMFCTabCtrl, COleSafeArray, COleException, COleDispatchException, COleDialog, COleUILinkInfo, COleChangeSourceDialog, COlePasteSpecialDialog, COleBusyDialog, COleConvertDialog, COleUpdateDialog, COleLinksDialog, COleInsertDialog, COleChangeIconDialog, CRecentFileList, CSharedFile, CNoTrackObject, CException, CMemoryException, CFileException, CArchiveException, CMetaFileDC, CPen, CBrush, CFont, CBitmap, CPalette, CRgn, CClientDC, CPaintDC, CWindowDC, CCriticalSection, CMutex, CSemaphore, CEvent, CSyncObject, CMultiLock, CSingleLock, CConnectionPoint, CEnumFormatEtc, COleDispatchDriver, CEnumOleVerb, CFontHolder, CPictureHolder, + many more

### Top skip-list offenders
| Class | Skip count | Reason |
|-------|-----------|--------|
| CWnd | 33 | Overloads with incompatible signatures |
| CMFCVisualManager | 8 | Complex drawing overloads |
| CRichEditCtrl | 8 | ABI struct incompatibilities |
| CHttpFile | 6 | QueryInfo overloads |
| CHtmlView | 5 | Navigate2/event handlers |
| CView | 5 | OnUpdate/OnActivateView |
| CFile | 4 | CommonInit/Remove/Rename/SetStatus |
| COleDocument | 4 | EditChangeIcon/EditConvert |
| CTaskDialog | 4 | Button/task management overloads |
| COleSafeArray | 4 | Attach/Create/Copy/Detach |

## Architecture

```
MSVC .def file (14,103 exports)
         │
         ▼
  extern "C" MS_ABI wrapper functions
         │
    ┌────┴────┐
    ▼         ▼
  thunks    stubs
  (real)    (return 0)
```

- **thunks**: Call real C++ method implementations via `pThis->Method(args)` — 988 auto-generated
- **manual implementations**: Implemented directly in `phase4/src/*.cpp` with `// Symbol:` comments — 337 symbols
- **skip list**: Correct ABI signatures but return defaults because method signatures fundamentally differ from MSVC exports — 157 symbols
- **stubs**: Generated by `gen_typed_stubs.py`, return 0/nullptr/void — ~11,773 symbols

## Key Tools

| Tool | Purpose |
|------|---------|
| `tools/gen_thunks.py` | Auto-generates routing wrappers from header declarations |
| `tools/gen_typed_stubs.py` | Generates properly-typed stubs for all remaining exports |
| `tools/gen_rtti.py` | Generates RTTI metadata |
| `tools/demangle_msvc.py` | MSVC name demangler used by all tools |
| `tools/thunks_skip.txt` | Symbols excluded from auto-generation (signature mismatch) |
| `phase4/scripts/build_phase4.sh` | Build script — compile + link DLL |

## How to Add More Implementations

1. Add method declaration to the appropriate header (`include/openmfc/afx*.h`)
2. Add implementation to the matching source file (`phase4/src/*.cpp`) with `// Symbol: ?mangled@@name` comment
3. Run `python3 tools/gen_thunks.py --mapping mfc_complete_ordinal_mapping.json --source-dir phase4/src --include-dir include/openmfc --out phase4/src/thunks.cpp`
4. Build: `bash phase4/scripts/build_phase4.sh`
5. If build fails due to signature mismatch: add the symbol to `tools/thunks_skip.txt`
