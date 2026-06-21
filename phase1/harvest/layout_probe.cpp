// Layout probe: a TU that pulls in the MFC class definitions so MSVC's
// /d1reportSingleClassLayout<Class> flag can dump each class's full member
// layout (offsets, sizes, vfptr, bases). Compiled on CI against the real
// ATLMFC headers; the report goes to stdout and is captured as an artifact.
//
// The flag only emits a report for a class that is referenced/instantiated in
// the TU, so we take sizeof() of each target to force completion.
#ifndef _AFXDLL
#define _AFXDLL
#endif
#include <afxwin.h>
#include <afxext.h>
#include <afxadv.h>
#include <afxole.h>
#include <afxdisp.h>

#define TOUCH(C) volatile size_t s_##C = sizeof(C);

TOUCH(CObject)
TOUCH(CCmdTarget)
TOUCH(CWnd)
TOUCH(CWinThread)
TOUCH(CWinApp)
TOUCH(CDocument)
TOUCH(CView)
TOUCH(CCtrlView)
TOUCH(CScrollView)
TOUCH(CFrameWnd)
TOUCH(CMDIFrameWnd)
TOUCH(CMDIChildWnd)
TOUCH(CDialog)
TOUCH(CDocTemplate)
TOUCH(CSingleDocTemplate)
TOUCH(CMultiDocTemplate)
TOUCH(CDocManager)
TOUCH(COleDocument)
TOUCH(COleLinkingDoc)
TOUCH(COleServerDoc)
TOUCH(COleClientItem)
TOUCH(COleServerItem)
TOUCH(COleObjectFactory)
TOUCH(COleDataSource)
TOUCH(CControlBar)
TOUCH(CStatusBar)
TOUCH(CToolBar)
TOUCH(CSplitterWnd)
TOUCH(CDockState)
TOUCH(CRecentFileList)
TOUCH(CCommandLineInfo)

int main() { return 0; }
