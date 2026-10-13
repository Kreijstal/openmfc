#pragma once
#include "openmfc/afxinet.h"
#include <cstdlib>

// Unused GDI siblings in the shared support TU still need link definitions.
// Abort if a drawing path unexpectedly reaches one of these test doubles.
CImageList::CImageList() { std::abort(); }
CImageList::~CImageList() { std::abort(); }
CDC::CDC() { std::abort(); }
CDC::~CDC() { std::abort(); }
BOOL CGdiObject::DeleteObject() { std::abort(); }
HINSTANCE g_d2dTestResourceModule = nullptr;
HINSTANCE AFXAPI AfxGetInstanceHandle() { return GetModuleHandleW(nullptr); }
HINSTANCE AFXAPI AfxGetResourceHandle() { return g_d2dTestResourceModule ? g_d2dTestResourceModule : GetModuleHandleW(nullptr); }
// List serialization is linked for its vtable but is outside these drawing tests.
CArchive& CArchive::operator<<(unsigned short) { std::abort(); }
CArchive& CArchive::operator<<(const CObject*) { std::abort(); }
CArchive& CArchive::operator>>(unsigned short&) { std::abort(); }
CArchive& CArchive::operator>>(CObject*&) { std::abort(); }
// Command routing is outside these rendering tests.
CCmdTarget::~CCmdTarget() = default;
const AFX_MSGMAP* CCmdTarget::GetMessageMap() const { std::abort(); }
int CCmdTarget::OnCmdMsg(unsigned int,int,void*,void*) { std::abort(); }
CRuntimeClass CCmdTarget::classCCmdTarget{};
CRuntimeClass CWinThread::classCWinThread{};
CRuntimeClass CWinApp::classCWinApp{};
CRuntimeClass CWnd::classCWnd{};
CRuntimeClass CFileFind::classCFileFind{};
CRuntimeClass CDocument::classCDocument{};
CRuntimeClass CHtmlView::classCHtmlView{};

