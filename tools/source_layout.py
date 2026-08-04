#!/usr/bin/env python3
"""Where each MFC class lives in the phase4 source tree.

`phase4/src` is organised one class per file, named after the class, grouped
into subsystem directories:

    phase4/src/core/window/CWnd.cpp
    phase4/src/core/gdi/CDC.cpp
    phase4/src/featurepack/ribbon/CMFCRibbonBar.cpp

This module is the single source of truth for that grouping.  The reorg
tooling, the code generators and the coverage reports all resolve directories
through :func:`directory_for_class`, so a class can be relocated by editing one
rule here.

Rules are matched in order: exact class names first, then prefixes.  Every
class must resolve; :func:`unmapped` is used by the tests to keep it that way.
"""

import re

# --- exact class -> directory ------------------------------------------------
#
# Listed where a prefix rule would put the class somewhere misleading, or where
# a family is too small to deserve a prefix of its own.

EXACT = {
    # core/runtime
    'CObject': 'core/runtime',
    'CRuntimeClass': 'core/runtime',
    'CArchive': 'core/runtime',
    'CDumpContext': 'core/runtime',
    'CCmdTarget': 'core/runtime',
    'CDynLinkLibrary': 'core/runtime',
    'CProcessLocalObject': 'core/runtime',
    'CThreadLocalObject': 'core/runtime',
    'CThreadSlotData': 'core/runtime',
    'CGlobalUtils': 'core/runtime',
    'CNoTrackObject': 'core/runtime',
    'CTime': 'core/runtime',
    'CTimeSpan': 'core/runtime',
    'AFX_GLOBAL_DATA': 'core/runtime',
    'AFX_COM': 'core/runtime',
    'AFX_MODULE_STATE': 'core/runtime',
    'AFX_MODULE_THREAD_STATE': 'core/runtime',
    '_AFX_THREAD_STATE': 'core/runtime',
    'AFX_MAINTAIN_STATE': 'core/runtime',
    'AFX_MAINTAIN_STATE2': 'core/runtime',
    'AFX_EXCEPTION_LINK': 'core/exceptions',

    # core/app
    'CWinApp': 'core/app',
    'CWinAppEx': 'core/app',
    'CWinThread': 'core/app',
    'CDocManager': 'core/app',
    'CCommandLineInfo': 'core/app',
    'CRecentFileList': 'core/app',
    'CDataRecoveryHandler': 'core/app',
    'CShellManager': 'core/app',
    'CJumpList': 'core/app',
    'CSettingsStore': 'core/app',
    'CSettingsStoreSP': 'core/app',

    # core/collections
    'CPlex': 'core/collections',
    'CFixedAlloc': 'core/collections',
    'CFixedAllocNoSync': 'core/collections',
    'CSimpleList': 'core/collections',
    'CHandleMap': 'core/collections',
    'CSimpleStringT': 'core/collections',

    # core/file
    'CFile': 'core/file',
    'CMemFile': 'core/file',
    'CStdioFile': 'core/file',
    'CSharedFile': 'core/file',
    'CMirrorFile': 'core/file',
    'CFileFind': 'core/file',
    'CLongBinary': 'core/file',

    # core/sync
    'CSyncObject': 'core/sync',
    'CMutex': 'core/sync',
    'CEvent': 'core/sync',
    'CSemaphore': 'core/sync',
    'CCriticalSection': 'core/sync',
    'CSingleLock': 'core/sync',
    'CMultiLock': 'core/sync',

    # core/window
    'CWnd': 'core/window',
    'CScreenWnd': 'core/window',
    'CParkingWnd': 'core/window',
    'CReflectorWnd': 'core/window',
    'CMenu': 'core/window',
    'CMenuHash': 'core/window',
    'CMenuTearOffManager': 'core/window',
    'CChevronOwnerDrawMenu': 'core/window',
    'CGestureConfig': 'core/window',

    # core/frame
    'CFrameWnd': 'core/frame',
    'CFrameWndEx': 'core/frame',
    'CFrameImpl': 'core/frame',
    'CFullScreenImpl': 'core/frame',
    'CMDIChildWnd': 'core/frame',
    'CMDIChildWndEx': 'core/frame',
    'CMDIFrameWnd': 'core/frame',
    'CMDIFrameWndEx': 'core/frame',
    'CMDIClientAreaWnd': 'core/frame',
    'CMDITabInfo': 'core/frame',
    'CMDITabProxyWnd': 'core/frame',
    'CMiniFrameWnd': 'core/frame',
    'CControlFrameWnd': 'core/frame',
    'CSplitterWnd': 'core/frame',
    'CSplitterWndEx': 'core/frame',

    # core/dialog
    'CDialog': 'core/dialog',
    'CDialogEx': 'core/dialog',
    'CDialogTemplate': 'core/dialog',
    'CCommonDialog': 'core/dialog',
    'CPropertySheet': 'core/dialog',
    'CPropertyPage': 'core/dialog',
    'CFileDialog': 'core/dialog',
    'CFolderPickerDialog': 'core/dialog',
    'CFontDialog': 'core/dialog',
    'CColorDialog': 'core/dialog',
    'CPrintDialog': 'core/dialog',
    'CPrintDialogEx': 'core/dialog',
    'CPageSetupDialog': 'core/dialog',
    'CFindReplaceDialog': 'core/dialog',
    'CTaskDialog': 'core/dialog',
    'CDataExchange': 'core/dialog',
    'CDialogImpl': 'core/dialog',

    # core/doc
    'CDocument': 'core/doc',
    'CDocItem': 'core/doc',
    'CDocTemplate': 'core/doc',
    'CSingleDocTemplate': 'core/doc',
    'CMultiDocTemplate': 'core/doc',
    'CRichEditDoc': 'core/doc',
    'CHtmlEditDoc': 'core/doc',

    # core/view
    'CView': 'core/view',
    'CCtrlView': 'core/view',
    'CScrollView': 'core/view',
    'CEditView': 'core/view',
    'CFormView': 'core/view',
    'CListView': 'core/view',
    'CTreeView': 'core/view',
    'CRichEditView': 'core/view',
    'CHtmlView': 'core/view',
    'CHtmlEditView': 'core/view',
    'CDHtmlDialog': 'core/view',
    'CMultiPageDHtmlDialog': 'core/view',
    'CPreviewView': 'core/view',
    'CPreviewViewEx': 'core/view',
    'CTabView': 'core/view',
    'CRecordView': 'core/view',
    'COleDBRecordView': 'core/view',
    'CPrintInfo': 'core/view',
    'CPrintPreviewState': 'core/view',
    'CBrowserControlSite': 'core/view',
    'CDHtmlControlSink': 'core/view',
    'CDHtmlElementEventSink': 'core/view',

    # core/gdi
    'CDC': 'core/gdi',
    'CClientDC': 'core/gdi',
    'CPaintDC': 'core/gdi',
    'CWindowDC': 'core/gdi',
    'CMetaFileDC': 'core/gdi',
    'CMemDC': 'core/gdi',
    'CPreviewDC': 'core/gdi',
    'CWindowlessDC': 'core/gdi',
    'CGdiObject': 'core/gdi',
    'CPen': 'core/gdi',
    'CBrush': 'core/gdi',
    'CFont': 'core/gdi',
    'CBitmap': 'core/gdi',
    'CPalette': 'core/gdi',
    'CRgn': 'core/gdi',
    'CImageList': 'core/gdi',
    'CDrawingManager': 'core/gdi',
    'CRectTracker': 'core/gdi',

    # core/controls
    'CButton': 'core/controls',
    'CBitmapButton': 'core/controls',
    'CEdit': 'core/controls',
    'CStatic': 'core/controls',
    'CListBox': 'core/controls',
    'CCheckListBox': 'core/controls',
    'CDragListBox': 'core/controls',
    'CComboBox': 'core/controls',
    'CComboBoxEx': 'core/controls',
    'CScrollBar': 'core/controls',
    'CSplitButton': 'core/controls',

    # core/controlbar
    'CControlBar': 'core/controlbar',
    'CControlBarInfo': 'core/controlbar',
    'CToolBar': 'core/controlbar',
    'CStatusBar': 'core/controlbar',
    'CDialogBar': 'core/controlbar',
    'CReBar': 'core/controlbar',
    'CDockBar': 'core/controlbar',
    'COleResizeBar': 'core/controlbar',
    'CDockState': 'core/controlbar',
    'CDockContext': 'core/controlbar',

    # core/cmdui
    'CCmdUI': 'core/cmdui',
    'CStatusCmdUI': 'core/cmdui',
    'CToolCmdUI': 'core/cmdui',
    'COleCmdUI': 'core/cmdui',
    'CTestCmdUI': 'core/cmdui',

    # core/db
    'CDatabase': 'core/db',
    'CRecordset': 'core/db',
    'CFieldExchange': 'core/db',
    'CDBVariant': 'core/db',
    'CDataSourceControl': 'core/db',

    # core/net
    'CAsyncSocket': 'core/net',
    'CSocket': 'core/net',
    'CSocketFile': 'core/net',
    'CSocketWnd': 'core/net',

    # core/ole
    'CMonikerFile': 'core/ole',
    'CAsyncMonikerFile': 'core/ole',
    'CDataPathProperty': 'core/ole',
    'CCachedDataPathProperty': 'core/ole',
    'CPropExchange': 'core/ole',
    'CPictureHolder': 'core/ole',
    'CFontHolder': 'core/ole',
    'CInnerUnknown': 'core/ole',
    'CHtmlControlSite': 'core/ole',
    'CControlSiteFactoryMgr': 'core/ole',
    'CControlCreationInfo': 'core/ole',
    'CTypeLibCache': 'core/ole',
    'CTypeLibCacheMap': 'core/ole',
    'CRichEditCntrItem': 'core/ole',
    'CDocObjectServer': 'core/ole',
    'CDocObjectServerItem': 'core/ole',
    'CAppDestinations': 'core/ole',
    'CConnectionPoint': 'core/ole',
    'CArchiveStream': 'core/ole',
    'CArchivePropExchange': 'core/ole',
    'CAsyncPropExchange': 'core/ole',
    'CPropbagPropExchange': 'core/ole',
    'CPropsetPropExchange': 'core/ole',
    'CResetPropExchange': 'core/ole',
    'CProperty': 'core/ole',
    'CPropertySection': 'core/ole',
    'CPropertySet': 'core/ole',
    'CBlobProperty': 'core/ole',
    'CDataBoundProperty': 'core/ole',
    'CControlDataSource': 'core/ole',
    'CReObject': 'core/ole',
    'AFX_DDPDATA': 'core/ole',

    # core/d2d
    'CRenderTarget': 'core/d2d',
    'CHwndRenderTarget': 'core/d2d',
    'CDCRenderTarget': 'core/d2d',
    'CBitmapRenderTarget': 'core/d2d',
    '_AFX_D2D_STATE': 'core/d2d',

    # core/animation
    'CBaseKeyFrame': 'core/animation',
    'CKeyFrame': 'core/animation',
    'CBaseTransition': 'core/animation',

    # featurepack/docking
    'CDockSite': 'featurepack/docking',
    'CAutoHideDockSite': 'featurepack/docking',
    'CDockablePane': 'featurepack/docking',
    'CDockablePaneAdapter': 'featurepack/docking',
    'CDummyDockablePane': 'featurepack/docking',
    'CDockingManager': 'featurepack/docking',
    'CDockingPanesRow': 'featurepack/docking',
    'CTabbedPane': 'featurepack/docking',
    'CBaseTabbedPane': 'featurepack/docking',
    'CBasePane': 'featurepack/docking',
    'CMultiPaneFrameWnd': 'featurepack/docking',
    'CMiniDockFrameWnd': 'featurepack/docking',
    'CRecentPaneContainerInfo': 'featurepack/docking',
    'CMFCControlBarImpl': 'featurepack/docking',
    'CRecentDockSiteInfo': 'featurepack/docking',
    'AFX_AUTOHIDE_DOCKSITE_SAVE_INFO': 'featurepack/docking',

    # featurepack/menu
    'CContextMenuManager': 'featurepack/menu',

    # featurepack/customize
    'CKeyboardManager': 'featurepack/customize',
    'CMouseManager': 'featurepack/customize',
    'CUserTool': 'featurepack/customize',
    'CUserToolsManager': 'featurepack/customize',
    'CTooltipManager': 'featurepack/customize',
    'CCommandManager': 'featurepack/customize',
    'COutlookOptionsDlg': 'featurepack/outlookbar',
    'COutlookCustomizeButton': 'featurepack/outlookbar',

    # featurepack/taskspane
    'CTasksPaneHistoryButton': 'featurepack/taskspane',
    'CTasksPaneMenuButton': 'featurepack/taskspane',
    'CTasksPaneNavigateButton': 'featurepack/taskspane',

    # featurepack/ribbon
    'CRibbonCategoryScroll': 'featurepack/ribbon',
    'CRibbonUndoLabel': 'featurepack/ribbon',
    'CXMLParserCollection': 'featurepack/ribbon',
    'CPngImage': 'featurepack/visualmanager',

    # featurepack/toolbar
    'CHelpComboBoxButton': 'featurepack/toolbar',

    # featurepack/controls
    'CVSListBox': 'featurepack/controls',
    'CVSListBoxBase': 'featurepack/controls',
    'CVSListBoxEditCtrl': 'featurepack/controls',
    'CVSToolsListBox': 'featurepack/controls',
    'CTagManager': 'featurepack/controls',
}

# --- prefix -> directory -----------------------------------------------------
#
# Longest prefix wins, so `CMFCRibbon` beats `CMFC` and `CDaoQueryDef` lands in
# core/dao rather than core/db.

PREFIXES = (
    ('CMFCRibbon', 'featurepack/ribbon'),
    ('CMFCToolBars', 'featurepack/customize'),
    ('CMFCToolBar', 'featurepack/toolbar'),
    ('CMFCBaseToolBar', 'featurepack/toolbar'),
    ('CMFCDropDownToolB', 'featurepack/toolbar'),
    ('CMFCDropDownToolb', 'featurepack/toolbar'),
    ('CMFCPrintPreviewToolBar', 'featurepack/toolbar'),
    ('CMFCVisualManager', 'featurepack/visualmanager'),
    ('CMFCBaseVisualManager', 'featurepack/visualmanager'),
    ('CMFCControlRenderer', 'featurepack/visualmanager'),
    ('CMFCShadowRenderer', 'featurepack/visualmanager'),
    ('CMFCPropertyGrid', 'featurepack/propertygrid'),
    ('CMFCOutlookBar', 'featurepack/outlookbar'),
    ('CMFCTasksPane', 'featurepack/taskspane'),
    ('CMFCAutoHide', 'featurepack/docking'),
    ('CMFCBaseTabCtrl', 'featurepack/tabs'),
    ('CMFCTabCtrl', 'featurepack/tabs'),
    ('CMFCTabButton', 'featurepack/tabs'),
    ('CMFCTabDropTarget', 'featurepack/tabs'),
    ('CMFCPopupMenu', 'featurepack/menu'),
    ('CMFCMenuBar', 'featurepack/menu'),
    ('CMFCMenuButton', 'featurepack/menu'),
    ('CMFCCaptionMenuButton', 'featurepack/menu'),
    ('CMFCColorMenuButton', 'featurepack/menu'),
    ('CMFCCustomizeMenuButton', 'featurepack/menu'),
    ('CMFCCustomizeButton', 'featurepack/customize'),
    ('CMFCKeyMapDialog', 'featurepack/customize'),
    ('CMFCMousePropertyPage', 'featurepack/customize'),
    ('CMFCCmdUsageCount', 'featurepack/customize'),
    ('CMFCWindowsManagerDialog', 'featurepack/customize'),
    ('CMFCAcceleratorKey', 'featurepack/customize'),
    ('CMFCPropertySheet', 'featurepack/propertygrid'),
    ('CMFCPropertyPage', 'featurepack/propertygrid'),
    ('CSmartDocking', 'featurepack/docking'),
    ('CPaneContainer', 'featurepack/docking'),
    ('CPaneDivider', 'featurepack/docking'),
    ('CPaneFrameWnd', 'featurepack/docking'),
    ('CPaneTrackingWnd', 'featurepack/docking'),
    ('CPaneDialog', 'featurepack/docking'),
    ('CPane', 'featurepack/docking'),
    ('CMFC', 'featurepack/controls'),

    ('CDao', 'core/dao'),
    ('CD2D', 'core/d2d'),
    ('CAnimation', 'core/animation'),
    ('COle', 'core/ole'),
    ('CEnum', 'core/ole'),
    ('CInternet', 'core/net'),
    ('CHttp', 'core/net'),
    ('CFtp', 'core/net'),
    ('CGopher', 'core/net'),
    ('CMap', 'core/collections'),
    ('CObList', 'core/collections'),
    ('CObArray', 'core/collections'),
    ('CPtr', 'core/collections'),
    ('CString', 'core/collections'),
    ('CByteArray', 'core/collections'),
    ('CDWordArray', 'core/collections'),
    ('CWordArray', 'core/collections'),
    ('CUIntArray', 'core/collections'),
)

# Exceptions all live together regardless of the subsystem that throws them.
EXCEPTION_RE = re.compile(r'Exception$')

# Remaining common-control classes: `C<Name>Ctrl` that is not a feature-pack or
# Direct2D/animation class by the rules above.
CTRL_RE = re.compile(r'^C[A-Z].*Ctrl$')

FALLBACK = 'core/controls'


def directory_for_class(cls):
    """Return the phase4/src-relative directory that owns *cls*."""
    if cls in EXACT:
        return EXACT[cls]
    if EXCEPTION_RE.search(cls):
        return 'core/exceptions'
    best = None
    for prefix, directory in PREFIXES:
        if cls.startswith(prefix) and (best is None or len(prefix) > len(best[0])):
            best = (prefix, directory)
    if best:
        return best[1]
    if CTRL_RE.match(cls):
        return 'core/controls'
    return FALLBACK


def path_for_class(cls):
    """Return the phase4/src-relative path of the file implementing *cls*."""
    return f'{directory_for_class(cls)}/{cls}.cpp'


def unmapped(classes):
    """Return the classes that only resolve through the catch-all fallback."""
    out = []
    for cls in classes:
        if cls in EXACT or EXCEPTION_RE.search(cls) or CTRL_RE.match(cls):
            continue
        if any(cls.startswith(p) for p, _ in PREFIXES):
            continue
        out.append(cls)
    return sorted(out)


if __name__ == '__main__':
    import sys
    for name in sys.argv[1:]:
        print(name, '->', path_for_class(name))
