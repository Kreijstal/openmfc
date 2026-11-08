// Extension libraries register in the module captured by CCmdTarget, under
// CRIT_DYNLINKLIST. The module list's link offset remains authoritative.
#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxdll_.h"
#include "detail/RegcoreSupport.h"
#include <new>
extern "C" void AFXAPI impl__AfxLockGlobals__YAXH_Z(int);
extern "C" void AFXAPI impl__AfxUnlockGlobals__YAXH_Z(int);
extern "C" BOOL AFXAPI impl__Remove_CSimpleList__QEAAHPEAX_Z(CSimpleList*,void*);
IMPLEMENT_DYNAMIC(CDynLinkLibrary,CCmdTarget)
namespace {
void Register(CDynLinkLibrary* self) {
    impl__AfxLockGlobals__YAXH_Z(0);
    auto& list=self->m_pModuleState->m_libraryList;
    *reinterpret_cast<void**>(reinterpret_cast<BYTE*>(self)+list.m_nNextOffset)=list.m_pHead;
    list.m_pHead=self;
    impl__AfxUnlockGlobals__YAXH_Z(0);
}
}
CDynLinkLibrary::CDynLinkLibrary(AFX_EXTENSION_MODULE& state,BOOL system)
    : m_classList(0x20),m_factoryList(0x40) {
    m_hModule=state.hModule;m_hResource=state.hResource;
    m_classList.m_pHead=state.pFirstSharedClass;
    m_factoryList.m_pHead=state.pFirstSharedFactory;
    m_bSystem=system;
    Register(this);
}
CDynLinkLibrary::CDynLinkLibrary(HINSTANCE module,HINSTANCE resources)
    : m_classList(0x20),m_factoryList(0x40) {
    m_hModule=module;m_hResource=resources;m_bSystem=FALSE;
    Register(this);
}
CDynLinkLibrary::~CDynLinkLibrary() {
    impl__AfxLockGlobals__YAXH_Z(0);
    (void)impl__Remove_CSimpleList__QEAAHPEAX_Z(&m_pModuleState->m_libraryList,this);
    impl__AfxUnlockGlobals__YAXH_Z(0);
}
// Symbol: ??0CDynLinkLibrary@@QEAA@AEAUAFX_EXTENSION_MODULE@@H@Z
extern "C" CDynLinkLibrary* AFXAPI impl___0CDynLinkLibrary__QEAA_AEAUAFX_EXTENSION_MODULE__H_Z(CDynLinkLibrary* self,AFX_EXTENSION_MODULE& state,BOOL system) {return ::new(self) CDynLinkLibrary(state,system);}
// Symbol: ??0CDynLinkLibrary@@QEAA@PEAUHINSTANCE__@@0@Z
extern "C" CDynLinkLibrary* AFXAPI impl___0CDynLinkLibrary__QEAA_PEAUHINSTANCE____0_Z(CDynLinkLibrary* self,HINSTANCE module,HINSTANCE resources) {return ::new(self) CDynLinkLibrary(module,resources);}
// Symbol: ??1CDynLinkLibrary@@UEAA@XZ
extern "C" void AFXAPI impl___1CDynLinkLibrary__UEAA_XZ(CDynLinkLibrary* self) {self->CDynLinkLibrary::~CDynLinkLibrary();}
// Symbol: ?GetThisClass@CDynLinkLibrary@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* AFXAPI impl__GetThisClass_CDynLinkLibrary__SAPEAUCRuntimeClass__XZ() {return CDynLinkLibrary::GetThisClass();}
// Symbol: ?GetRuntimeClass@CDynLinkLibrary@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* AFXAPI impl__GetRuntimeClass_CDynLinkLibrary__UEBAPEAUCRuntimeClass__XZ(const CDynLinkLibrary*) {return CDynLinkLibrary::GetThisClass();}
