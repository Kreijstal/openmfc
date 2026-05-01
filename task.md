# OpenMFC Phase 4 Implementation Checklist

## Completed ✅

### Core Classes
- [x] CString (26 tests, MSVC & MinGW verified)
- [x] CObject + CRuntimeClass (19 tests, MSVC & MinGW verified)
- [x] CCmdTarget (OnCmdMsg handles 15+ signature types)
- [x] CWnd (232 bytes, 29 tests, MSVC & MinGW verified)
- [x] CWinThread / CWinApp
- [x] CException / CMemoryException / CFileException

### Macros
- [x] DECLARE_DYNAMIC / IMPLEMENT_DYNAMIC
- [x] DECLARE_DYNCREATE / IMPLEMENT_DYNCREATE
- [x] RUNTIME_CLASS

---

## Priority 1: Core Application Functions ✅

### Application Info & Management
- [x] AfxGetApp - Get CWinApp pointer
- [x] AfxGetMainWnd - Get main window
- [x] AfxGetInstanceHandle - Get HINSTANCE
- [x] AfxGetResourceHandle - Get resource HINSTANCE
- [x] AfxGetThread - Get current CWinThread
- [/] AfxWinInit - Returns TRUE without initialization (STUB)
- [/] AfxSetResourceHandle - No-op (STUB)
- [/] AfxRegisterClass - Implicit/passthrough
- [/] AfxRegisterWndClass - Implicit/passthrough

### Exception Processing
- [x] AfxThrowMemoryException
- [x] AfxThrowFileException
- [x] AfxThrowResourceException
- [x] AfxThrowNotSupportedException
- [x] AfxThrowInvalidArgException
- [x] AfxThrowUserException
- [x] AfxAbort

### String Functions
- [x] AfxFormatString1 - String substitution
- [x] AfxFormatString2 - Two-string substitution
- [x] AfxMessageBox - Display message box
- [x] AfxExtractSubString - Extract substring

---

## Priority 2: Window & Dialog Classes

### Window Classes
- [x] CFrameWnd - Main frame window (menu/resource loading implemented)
- [/] CMDIFrameWnd - MDI frame (partial)
- [/] CMDIChildWnd - MDI child (partial)
- [x] CDialog - Modal/modeless dialogs
- [/] CDialogEx - Extended dialog (partial)
- [x] CPropertySheet - Property sheets (PropertySheetW-based, supports modal/modeless/wizard)
- [x] CPropertyPage - Property pages (full PS notifications, SetModified, QuerySiblings, CancelToClose)

### Controls
- [x] CButton (GetState, SetState, GetCheck, SetCheck, GetButtonStyle, SetButtonStyle, bitmap/icon)
- [x] CEdit (GetLineCount, GetLine, GetSel, SetSel, ReplaceSel, clipboard ops, undo, readonly, modify, limit)
- [x] CStatic (bitmap, icon, enhanced metafile)
- [x] CListBox (GetCount, GetCurSel, SetCurSel, GetText, AddString, InsertString, DeleteString, FindString, ItemData)
- [x] CComboBox (GetCount, GetCurSel, SetCurSel, GetLBText, AddString, InsertString, DeleteString, FindString, ItemData, ShowDropDown)
- [x] CScrollBar (GetScrollPos, SetScrollPos, GetScrollRange, SetScrollRange, EnableScrollBar)
- [x] CSliderCtrl (GetPos, SetPos, SetRange, GetRangeMin/Max, SetTicFreq, SetPageSize, SetLineSize)
- [x] CProgressCtrl (GetPos, SetPos, SetRange, SetRange32, OffsetPos, SetStep, StepIt, SetMarquee)
- [x] CSpinButtonCtrl (GetPos, SetPos, SetRange, SetRange32, GetRange, SetBuddy, SetBase)
- [x] CListCtrl (GetItemCount, InsertItem, DeleteItem, Get/SetItemText, Get/SetItemData, InsertColumn)
- [x] CTreeCtrl (InsertItem, DeleteItem, Get/SetItemText, GetSelectedItem, SelectItem, Expand, navigation)
- [x] CTabCtrl (GetItemCount, GetCurSel, SetCurSel, InsertItem, DeleteItem, AdjustRect)

---

## Priority 3: Document-View Architecture

### Document/View
- [x] CDocument (OnNewDocument, OnOpenDocument, OnSaveDocument, views, serialization)
- [x] CView (GetDocument, OnUpdate, OnInitialUpdate, printing hooks)
- [x] CScrollView - All 6 scroll methods implemented (GetScrollBarSizes, GetTrueClientSize, ScrollToPosition, GetScrollPosition, FillOutsideRect, ResizeParentToFit)
- [x] CFormView - Create() implemented using dialog template
- [x] CEditView - Serialize(), GetSelectedText(), FindText(), PrintInsideRect() all implemented
- [x] CListView (list control view)
- [x] CTreeView (tree control view)
- [x] CDocTemplate - CreateAndReplaceFrame() implemented
- [x] CSingleDocTemplate (SDI application support)
- [x] CMultiDocTemplate (MDI application support)

### Serialization
- [x] CArchive (buffered read/write, operator<< / operator>>, string operations)
- [x] CArchiveException
- [x] DECLARE_SERIAL / IMPLEMENT_SERIAL - CRuntimeClass::Load/Store implemented
- [x] CFile (CreateFile, Read, Write, Seek, GetLength, status operations)
- [x] CStdioFile (fopen-based file operations, ReadString, WriteString)
- [x] CMemFile (in-memory file buffer, grow/detach/attach)

---

## Priority 4: GDI & Graphics ✅

### Device Contexts
- [x] CDC (core methods: CreateCompatibleDC, DeleteDC, SaveDC, RestoreDC, drawing, text, blit)
- [x] CClientDC
- [x] CPaintDC
- [x] CWindowDC
- [x] CMetaFileDC (Create, CreateEnhanced, Close, CloseEnhanced)

### GDI Objects
- [x] CGdiObject (DeleteObject, Attach, Detach)
- [x] CPen (constructors, CreatePen)
- [x] CBrush (CreateSolidBrush, CreateHatchBrush)
- [x] CFont (CreateFontIndirectW, CreatePointFont)
- [x] CBitmap (CreateCompatibleBitmap, LoadBitmapW)
- [/] CPalette (runtime class only)
- [/] CRgn (runtime class only)

### Geometry
- [x] CPoint (fully inline)
- [x] CSize (fully inline)
- [x] CRect (fully inline)

---

## Priority 5: Collections ✅

- [x] CArray (template class with SetSize, Add, InsertAt, RemoveAt)
- [x] CList (doubly-linked list with block allocation)
- [x] CMap (hash table with block allocation)
- [x] CStringArray (typedef for CArray<CString>)
- [x] CStringList (typedef for CList<CString>)
- [x] CPtrArray (typedef for CArray<void*>)
- [x] CPtrList (typedef for CList<void*>)
- [x] CObArray (typedef for CArray<CObject*>)
- [x] CTypedPtrArray (type-safe wrapper template)
- [x] CTypedPtrList (type-safe wrapper template)
- [x] CTypedPtrMap (type-safe wrapper template)
- [x] CMapStringToOb, CMapStringToPtr, CMapStringToString (string key maps)

---

## Priority 6: Threading & Sync ✅

- [x] AfxBeginThread (worker thread creation with AFX_THREADPROC)
- [x] AfxEndThread (clean thread termination)
- [x] CSyncObject (base class with Lock/Unlock)
- [x] CCriticalSection (InitializeCriticalSection/EnterCriticalSection)
- [x] CMutex (CreateMutexW/ReleaseMutex)
- [x] CSemaphore (CreateSemaphoreW/ReleaseSemaphore)
- [x] CEvent (CreateEventW/SetEvent/ResetEvent)
- [x] CSingleLock (RAII single object lock)
- [x] CMultiLock (RAII multiple object lock with WaitForMultipleObjects)

---

## Priority 7: Common Dialogs ✅

- [x] CFileDialog (GetOpenFileName/GetSaveFileName - full path/name parsing)
- [x] CFileDialog::GetNextPathName - Multi-file selection implemented
- [x] CColorDialog (ChooseColor - with custom colors persistence)
- [x] CFontDialog (ChooseFont - face, size, style, color, weight)
- [x] CPrintDialog (PrintDlg - copies, pages, orientation, DC, DevMode/DevNames)
- [x] CPageSetupDialog (PageSetupDlg - margins, paper size, DevMode/DevNames)
- [x] CFindReplaceDialog (FindText/ReplaceText - real modeless dialogs)

---

## Priority 8: Advanced (Lower Priority)

### Internet/Sockets
- [x] CInternetSession (WinInet session management, GetHttpConnection, GetFtpConnection, OpenURL)
- [x] CHttpConnection (HTTP connections, OpenRequest with multiple overloads)
- [x] CFtpConnection (FTP connections, GetFile, PutFile, directory ops)
- [x] CInternetFile / CHttpFile (Internet-based file I/O, HTTP request/response)
- [x] CInternetException (WinInet error handling)
- [x] CAsyncSocket (Real WinSock2 wrappers - Create, Bind, Listen, Accept, Connect, Send, Receive, etc.)
- [x] CSocket (Blocking socket with message pump - Accept, Send, Receive, CancelBlockingCall)
- [x] CSocketFile (CFile interface for sockets - Read, Write)
- [x] AfxSocketInit (WSAStartup singleton)

### OLE/COM ✅
- [x] COleDispatchDriver (CreateDispatch, InvokeHelper with full VARIANT marshaling)
- [x] COleException / COleDispatchException
- [x] COleVariant (VARIANT wrapper)
- [x] COleSafeArray (SafeArray wrapper with Create/Destroy/AccessData)
- [x] COleCurrency / COleDateTime
- [x] COleDataObject (IDataObject wrapper, clipboard/enumeration)
- [x] COleDataSource (Clipboard/drag-drop source)
- [x] COleDropTarget (RegisterDragDrop/RevokeDragDrop)
- [x] COleDropSource (QueryContinueDrag/GiveFeedback)
- [x] COleMessageFilter (OLE busy/retry management, CoRegisterMessageFilter)
- [x] COleCmdUI
- [x] COleFrameHook
- [x] COleResizeBar
- [x] COleStreamFile (LPSTREAM wrapping, Read/Write/Seek/LockRegion)
- [x] COlePropertyPage
- [x] COleDocument / COleLinkingDoc / COleServerDoc
- [x] COleClientItem (OLE object embedding/linking)
- [x] COleServerItem
- [x] COleObjectFactory / COleTemplateServer
- [x] COleControlSite / COleControlContainer
- [x] COleControl (ActiveX control base)
- [x] COleIPFrameWnd
- [x] OLE Dialogs: COleBusyDialog, COleChangeIconDialog, COleChangeSourceDialog,
      COleConvertDialog, COleInsertDialog, COleLinksDialog, COlePasteSpecialDialog,
      COlePropertiesDialog, COleUpdateDialog
- [x] AfxOleInit/AfxOleTerm (OleInitialize/OleUninitialize)
- [x] AfxOleLockApp/AfxOleUnlockApp/AfxOleCanExitApp
- [x] AfxOleRegisterTypeLib/AfxOleUnregisterTypeLib

### Database
- [x] CDatabase (ODBC connection: Open/Close/BeginTrans/CommitTrans/Rollback/ExecuteSQL)
- [x] CRecordset (ODBC record navigation: MoveFirst/Last/Next/Prev, AddNew/Edit/Update/Delete)
- [x] CDBException
- [x] CRecordView
- [x] CFieldExchange

### Resource Management
- [x] AfxLoadLibrary / AfxFreeLibrary
- [x] AfxFindResourceHandle
- [/] AfxSetResourceHandle - No-op stub

---

## Diagnostic Functions (Debug Only)
- [x] AfxIsValidAddress - Memory validity check via VirtualQuery
- [x] AfxIsValidString - String validity check (wide + narrow)
- [x] AfxIsValidAtom - Atom validity check (WORD + string)
- [x] AfxCheckMemory - Heap integrity validation (_CrtCheckMemory)
- [x] AfxDebugBreak - Debug breakpoint (DebugBreak)
- [x] AfxDump - Debug output via CDumpContext/OutputDebugString
- [x] AfxDumpStack - Stack trace output (OutputDebugString)

---

## Recently Implemented (Previously Stubs)

### High Priority ✅
1. **CScrollView scrolling** - All 6 scroll methods now implemented
2. **CFormView::Create** - Now creates form views using dialog templates
3. **CEditView text operations** - Serialize, GetSelectedText, FindText, PrintInsideRect implemented
4. **CRuntimeClass::Load/Store** - Serialization now works with class name lookup
5. **Menu/Resource loading** - CFrameWnd loads menus, icons, accelerators, titles from resources

### Medium Priority ✅
1. **CCmdTarget::OnCmdMsg** - Now handles 15+ signature types
2. **CDocTemplate::CreateAndReplaceFrame** - Implemented
3. **CFileDialog multi-select** - GetNextPathName now iterates through selected files
4. **CException::GetErrorMessage** - CFileException and CArchiveException provide error messages

## Remaining Stubs (13,677 of 14,103 exports)

### Current state: 426 real implementations, 13,677 weak stubs

---

### Tier 1 — High Impact, Moderate Effort (~1,600 stubs)

- [x] **CToolBar** (1,047) — Toolbar creation, LoadToolBar, SetButtons, SetButtonInfo, GetItemRect, SetSizes, etc.
- [x] **CStatusBar** (127) — Status bar: Create, SetIndicators, SetPaneText/GetPaneText, SetPaneInfo, CommandToIndex
- [x] **CDialogBar** (11) — Dialog-based control bar: Create with dialog template, UpdateData
- [x] **CRichEditView** (115) — Rich edit 2.0 view with formatting, printing
- [ ] **CHtmlView** (103) — WebBrowser control-based view
- [ ] **CDHtmlDialog** (105) — DHTML-based dialog
- [x] **CSplitterWnd** (77) — Splitter window (dynamic/static panes)
- [x] **CTaskDialog** (65) — Vista+ task dialog (TaskDialogIndirect)

### Tier 2 — DAO Database (~170 stubs)

- [ ] **CDaoDatabase** — DAO database connection (Open/Close/Execute)
- [ ] **CDaoRecordset** — DAO recordset navigation and editing
- [ ] **CDaoWorkspace** — DAO workspace/transaction management
- [ ] **CDaoTableDef** — DAO table definition
- [ ] **CDaoQueryDef** — DAO query definition
- [ ] **CDaoException** — DAO error handling
- [ ] **CDaoFieldExchange** — DAO field exchange (DFX)
- [ ] **CDaoRecordView** — DAO record view

### Tier 3 — Fill in OLE/COM exports (~550 stubs)

- [ ] **COleControl** additional methods (375) — FireEvent, DoPropExchange, ambient properties, licensing, persistence
- [ ] **COleClientItem** additional methods (146) — CreateFromData, Draw, GetExtent, DoVerb overrides
- [ ] **COleDocObjectItem** (24) — DocObject (Active Document) support
- [ ] **CEnumFormatEtc** (6) — FORMATETC enumerator for clipboard

### Tier 4 — Fill in overload gaps (~380 stubs)

- [ ] **Internet** (141) — CInternetSession/CHttpConnection/CFtpConnection/CInternetFile additional overloads
- [ ] **Sockets** (73) — CAsyncSocket/CSocket/CSocketFile additional overloads
- [ ] **Common Dialogs** (168) — CFileDialog/CColorDialog/CFontDialog/CPrintDialog extra overloads
- [ ] **CPrintDialogEx** (25) — Extended print dialog (PrintDlgEx)

### Tier 5 — MFC Feature Pack (~5,500 stubs)

_These are Visual Studio 2008+ MFC Feature Pack classes — ribbon, docking, visual styles. Lower priority: rarely used by real MFC consumers._

- [ ] **CMFCToolBar** (239) — Feature pack toolbar
- [ ] **CMFCVisualManager** (157) + Office 2003/2007/XP variants (286)
- [ ] **CMFCRibbonBar** (132) + Panel (82) + Category (68) + Elements (62)
- [ ] **Docking system** — CPane (69), CDockablePane (101), CBasePane (64), CDockingManager (73), CPaneFrameWnd (120)
- [ ] **CMFCPropertyGridCtrl** (111) + CMFCPropertyGridProperty (62)
- [ ] **CMFCTasksPane** (113)
- [ ] **CMFCBaseTabCtrl** (100) + CMFCTabCtrl (69)
- [ ] **CMFCPopupMenu** (90), CMFCButton (66)
- [ ] **Frame extensions** — CFrameWndEx (75), CMDIFrameWndEx (92), CMDIChildWndEx (80)

### Impractical — Compiler-generated (~5,900 stubs)

_These cannot be hand-written: vtable dispatch thunks, CStringT<wchar_t> template instantiations, ATL:: internal exports, QEAA/UEAA mangled name fragments. Generated automatically by MSVC when building real MFC._

---

## Exit Criteria
- [x] Hello World MFC app runs on Wine ✅ (MSVC-clang + lld-link + wine: exception catching, CString, RTTI all verified)
- [x] Simple dialog app compiles and links with MSVC ABI (runtime needs display/Xvfb)
- [x] Document-View app skeleton compiles and links with MSVC ABI (runtime needs display/Xvfb)
