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
extern thread_local AFX_MODULE_STATE* g_cmdTestCurrentModule;
HINSTANCE g_typeCacheInstance=nullptr;
AFX_MODULE_STATE* g_typeCacheInstanceModule=nullptr;
bool g_typeCacheForceNoInstance=false;
HINSTANCE AFXAPI AfxGetInstanceHandle() {
    g_typeCacheInstanceModule=g_cmdTestCurrentModule;
    return g_typeCacheForceNoInstance ? nullptr : g_typeCacheInstance ? g_typeCacheInstance : GetModuleHandleW(nullptr);
}
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

// Rendering tests never delete a target through the base final-release body.
extern "C" void MS_ABI impl__OnFinalRelease_CCmdTarget__UEAAXXZ(CCmdTarget*) { std::abort(); }

// Module switching is peripheral to rendering; these doubles model thread
// state so the command-target tests can verify capture and balanced restore.
thread_local AFX_MODULE_STATE* g_cmdTestCurrentModule=nullptr;
extern "C" AFX_MODULE_STATE* MS_ABI impl__AfxGetModuleState__YAPEAVAFX_MODULE_STATE__XZ() { return g_cmdTestCurrentModule; }
extern "C" AFX_MODULE_STATE* MS_ABI impl__AfxSetModuleState__YAPEAVAFX_MODULE_STATE__PEAV1__Z(AFX_MODULE_STATE* state) {
    AFX_MODULE_STATE* previous=g_cmdTestCurrentModule; g_cmdTestCurrentModule=state; return previous;
}

// Property getter allocation/type failures use the framework exception sibling.
extern "C" void MS_ABI impl__AfxThrowOleException__YAXJ_Z(long) { std::abort(); }

// Invalid event arguments exercise the framework exception path.
extern "C" void MS_ABI impl__AfxThrowInvalidArgException__YAXXZ() { throw 19; }

// Pointer-array serialization is unrelated to connection enumeration.
CArchive& CArchive::operator<<(const void*) { std::abort(); }
CArchive& CArchive::operator>>(void*&) { std::abort(); }
CArchive& CArchive::operator<<(unsigned int) { std::abort(); }
CArchive& CArchive::operator>>(unsigned int&) { std::abort(); }

// Connection-array allocation failures use the framework memory exception.
extern "C" void MS_ABI impl__AfxThrowMemoryException__YAXXZ() { throw std::bad_alloc(); }

// Exception diagnostics and application lookup are peripheral to automation.
CRuntimeClass CException::classCException{"CException",sizeof(CException),0xffff,nullptr,&CObject::GetThisClass,nullptr,nullptr};
int CException::GetErrorMessage(wchar_t*,UINT,UINT*) const { std::abort(); }
void CException::Dump() const { std::abort(); }
void CException::AssertValid() const { std::abort(); }
CWinThread* AFXAPI AfxGetThread() { return nullptr; }
CRuntimeClass g_automationMemoryExceptionClass{"CMemoryException",sizeof(CException),0xffff,nullptr,nullptr,nullptr,nullptr};
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMemoryException__SAPEAUCRuntimeClass__XZ() { return &g_automationMemoryExceptionClass; }

// Model the recursive CRIT_TYPELIBCACHE lock used by the production lookup.
std::recursive_mutex g_typeCacheTestMutex;
thread_local int g_typeCacheLockDepth=0;
extern "C" void MS_ABI impl__AfxLockGlobals__YAXH_Z(int id) {
    if(id!=13)std::abort();g_typeCacheTestMutex.lock();++g_typeCacheLockDepth;
}
extern "C" void MS_ABI impl__AfxUnlockGlobals__YAXH_Z(int id) {
    if(id!=13 || g_typeCacheLockDepth<=0)std::abort();--g_typeCacheLockDepth;g_typeCacheTestMutex.unlock();
}
