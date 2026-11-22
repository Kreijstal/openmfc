// Native Unicode x64 module-state lifetime.
#define OPENMFC_APPCORE_IMPL
#include "detail/RegcoreSupport.h"

AFX_MODULE_STATE::AFX_MODULE_STATE(BOOL dll,WNDPROC wndProc,DWORD version,BOOL system)
    : m_bDLL(static_cast<BYTE>(dll)),m_bSystem(static_cast<BYTE>(system)),
      m_fRegisteredClasses(0),m_classList(offsetof(CRuntimeClass,m_pNextClass)),
      m_factoryList(0x40),m_pfnAfxWndProc(wndProc),m_dwVersion(version),
      m_typeLibCache(CTypeLibCache::Embedded{}) {
    // The remaining fields retain the zero-initialized module allocation.
    try {
        m_strUnregisterList.GetBuffer(4096);
    } catch(CMemoryException* error) {
        if(error)error->Delete();
    }
    m_bUserCtrl=TRUE;
    m_lockList.Construct(0x20);
    m_libraryList.Construct(0x78);
    m_bInitNetworkAddressControl=FALSE;
    m_bInitNetworkAddressControlCalled=FALSE;
}

// Symbol: ??0AFX_MODULE_STATE@@QEAA@HP6A_JPEAUHWND__@@I_K_J@ZKH@Z
extern "C" AFX_MODULE_STATE* AFXAPI impl___0AFX_MODULE_STATE__QEAA_HP6A_JPEAUHWND____I_K_J_ZKH_Z(
    AFX_MODULE_STATE* state,BOOL dll,WNDPROC wndProc,DWORD version,BOOL system) {
    return ::new(state) AFX_MODULE_STATE(dll,wndProc,version,system);
}
// Symbol: ??1AFX_MODULE_STATE@@UEAA@XZ
extern "C" void AFXAPI impl___1AFX_MODULE_STATE__UEAA_XZ(AFX_MODULE_STATE* state) {
    state->AFX_MODULE_STATE::~AFX_MODULE_STATE();
}
