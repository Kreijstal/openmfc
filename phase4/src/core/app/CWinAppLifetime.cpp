// Release Unicode x64 application registration and owned-resource teardown.
#define OPENMFC_APPCORE_IMPL
#include "detail/ModuleThreadStateSupport.h"
#include "detail/RegcoreSupport.h"
#include "detail/AppDocumentManagerState.h"
#include <cstdlib>
#include <cwchar>
#include <initializer_list>

extern "C" void MS_ABI impl__AfxGlobalFree__YAXPEAX_Z(void*);
extern "C" void MS_ABI impl__AfxThrowInvalidArgException__YAXXZ();
namespace {
template<class T> T& slot(CWinApp* self,size_t offset) {
    return *reinterpret_cast<T*>(reinterpret_cast<BYTE*>(self)+offset);
}
void dispose(void* object,size_t index) {
    if(object) {
        using Delete=void* (MS_ABI*)(void*,unsigned);
        reinterpret_cast<Delete>((*reinterpret_cast<void***>(object))[index])(object,1);
    }
}
void release(CWinApp* self,size_t offset) {
    auto*& object=slot<IUnknown*>(self,offset);
    if(object) {object->Release();object=nullptr;}
}
}
CWinApp::CWinApp(const wchar_t* name) {
    m_pszAppName=name ? ::_wcsdup(name) : nullptr;
    auto* module=m_pModuleState;
    if(!module)impl__AfxThrowInvalidArgException__YAXXZ();
    openmfc::detail::modulethreadstate::ForModule(module)->m_pCurrentWinThread=this;
    m_hThread=::GetCurrentThread();
    m_nThreadID=::GetCurrentThreadId();
    module->m_pCurrentWinApp=this;
    m_hInstance=nullptr;m_lpCmdLine=nullptr;m_pszAppID=nullptr;
    m_pszRegistryKey=nullptr;m_pDocManager=nullptr;m_bHelpMode=FALSE;
    m_pszExeName=nullptr;m_pszHelpFilePath=nullptr;m_pszProfileName=nullptr;m_eHelpType=0;
    // Match the native initialized fields while preserving padding, command
    // show state and the reserved fields not written by the constructor.
    for(size_t offset : {0xe8,0xf0,0x100,0x110,0x118,0x120,0x128,0x130,0x140,0x168,0x170})
        slot<UINT_PTR>(this,offset)=0;
    slot<DWORD>(this,0x108)=0;
    slot<UINT_PTR>(this,0x138)=0x200;
    slot<DWORD>(this,0x14c)=0;
    slot<DWORD>(this,0x150)=300000;
    slot<BOOL>(this,0x154)=TRUE;
    OSVERSIONINFOW version{};version.dwOSVersionInfoSize=sizeof(version);
    ::GetVersionExW(&version);
    slot<BOOL>(this,0x158)=version.dwMajorVersion>6 ||
        (version.dwMajorVersion==6 && version.dwMinorVersion>=1);
    slot<BOOL>(this,0x15c)=FALSE;
    slot<BOOL>(this,0x160)=TRUE;
}
CWinApp::~CWinApp() {
    slot<BOOL>(this,0x154)=FALSE;
    release(this,0x168);release(this,0x170);
    if(slot<BOOL>(this,0x15c)) {
        ::CoUninitialize();slot<BOOL>(this,0x15c)=FALSE;
    }
    dispose(m_pDocManager,1);
    dispose(slot<void*>(this,0x118),7);
    dispose(slot<void*>(this,0x120),1);
    if(!impl__AfxGetModuleState__YAPEAVAFX_MODULE_STATE__XZ()->m_bDLL) {
        using namespace openmfc::detail::docmanager;
        if(staticDocumentManager) {dispose(staticDocumentManager,1);staticDocumentManager=nullptr;}
        if(staticTemplateList) {dispose(staticTemplateList,1);staticTemplateList=nullptr;}
    }
    if(auto handle=slot<void*>(this,0xe8))impl__AfxGlobalFree__YAXPEAX_Z(handle);
    if(auto handle=slot<void*>(this,0xf0))impl__AfxGlobalFree__YAXPEAX_Z(handle);
    if(auto atom=slot<ATOM>(this,0x130))::GlobalDeleteAtom(atom);
    if(auto atom=slot<ATOM>(this,0x132))::GlobalDeleteAtom(atom);
    dispose(slot<void*>(this,0x128),1);
    if(m_pModuleState->m_lpszCurrentAppName==m_pszAppName)m_pModuleState->m_lpszCurrentAppName=nullptr;
    if(m_pModuleState->m_pCurrentWinApp==this)m_pModuleState->m_pCurrentWinApp=nullptr;
    std::free(const_cast<wchar_t*>(m_pszAppName));
    std::free(const_cast<wchar_t*>(m_pszRegistryKey));
    std::free(const_cast<wchar_t*>(m_pszExeName));
    std::free(const_cast<wchar_t*>(m_pszHelpFilePath));
    std::free(const_cast<wchar_t*>(m_pszProfileName));
    m_hThread=nullptr;
}
// Symbol: ??0CWinApp@@QEAA@PEB_W@Z
extern "C" CWinApp* MS_ABI impl___0CWinApp__QEAA_PEB_W_Z(CWinApp* self,const wchar_t* name) {
    return ::new(self) CWinApp(name);
}
// Symbol: ??1CWinApp@@UEAA@XZ
extern "C" void MS_ABI impl___1CWinApp__UEAA_XZ(CWinApp* self) {self->CWinApp::~CWinApp();}
