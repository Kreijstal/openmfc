#pragma once
#include "openmfc/afxinet.h"
#include "openmfc/afxstat_.h"
#include <cstdlib>

// Unused GDI siblings in the shared support TU still need link definitions.
// Abort if a drawing path unexpectedly reaches one of these test doubles.
CImageList::CImageList() { std::abort(); }
CImageList::~CImageList() { std::abort(); }
CDC::CDC() { std::abort(); }
CDC::~CDC() { std::abort(); }
BOOL CGdiObject::DeleteObject() { std::abort(); }
HINSTANCE g_d2dTestResourceModule = nullptr;
#define g_cmdTestCurrentModule (AfxGetThreadState()->m_pModuleState)
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
int CCmdTarget::OnCmdMsg(unsigned int,int,void*,AFX_CMDHANDLERINFO*) { std::abort(); }
CRuntimeClass CCmdTarget::classCCmdTarget{};
CRuntimeClass CWinThread::classCWinThread{};
CRuntimeClass CWinApp::classCWinApp{};
CRuntimeClass CWnd::classCWnd{};
CRuntimeClass CFileFind::classCFileFind{};
CRuntimeClass CDocument::classCDocument{};
CRuntimeClass CHtmlView::classCHtmlView{};


// Module switching is peripheral to rendering; these doubles model thread
// state so the command-target tests can verify capture and balanced restore.
extern "C" AFX_MODULE_STATE* MS_ABI impl__AfxGetModuleState__YAPEAVAFX_MODULE_STATE__XZ() { return g_cmdTestCurrentModule; }

// Property getter allocation/type failures use the framework exception sibling.
extern "C" void MS_ABI impl__AfxThrowOleException__YAXJ_Z(long result) {
    auto* error=new COleException;error->m_sc=result;throw error;
}

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

// Generic clone allocation uses the same malloc/free pair as MFC exports.
extern "C" void* MS_ABI impl___2_YAPEAX_K_Z(size_t bytes) { return std::malloc(bytes); }
extern "C" void MS_ABI impl___3_YAXPEAX_Z(void* memory) { std::free(memory); }
// Generic clone virtuals retain peripheral runtime-class and command siblings.
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CCmdTarget__UEBAPEAUCRuntimeClass__XZ(const CCmdTarget*) { return &CCmdTarget::classCCmdTarget; }
// Command-map lookup is peripheral to the frame/view routing tests. Their
// callback records the real target, arguments and thread routing context.
int (*g_cmdRoutingMapHook)(CCmdTarget*,UINT,int,void*,void*)=nullptr;
extern "C" int MS_ABI impl__OnCmdMsg_CCmdTarget__UEAAHIHPEAXPEAUAFX_CMDHANDLERINFO___Z(
    CCmdTarget* target,UINT id,int code,void* extra,void* info) {
    if(g_cmdRoutingMapHook)return g_cmdRoutingMapHook(target,id,code,extra,info);
    std::abort();
}
