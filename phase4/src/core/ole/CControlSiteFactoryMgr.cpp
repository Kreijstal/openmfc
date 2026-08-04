// CControlSiteFactoryMgr — OpenMFC implementation.
// Sources: global_ccontrolsitefactorymgr.cpp

#include "detail/CControlSiteFactoryMgrSupport.h"

// Symbol: ??0CControlSiteFactoryMgr@@QEAA@XZ
extern "C" void* MS_ABI impl___0CControlSiteFactoryMgr__QEAA_XZ(void* pThis) {
    S_Ccontrolsitefactorymgr* p = static_cast<S_Ccontrolsitefactorymgr*>(pThis);
    p->vfptr = (void*)&g_CControlSiteFactoryMgr_vtbl[0];

    // Construct the embedded CList: empty, default block size 10.
    p->m_lstFactory.vfptr        = nullptr;
    p->m_lstFactory.m_pNodeHead  = nullptr;
    p->m_lstFactory.m_pNodeTail  = nullptr;
    p->m_lstFactory.m_nCount     = 0;
    p->m_lstFactory.m_pNodeFree  = nullptr;
    p->m_lstFactory.m_pBlocks    = nullptr;
    p->m_lstFactory.m_nBlockSize = 10;

    p->m_pOleControlSiteDefaultFactory = nullptr;
    return pThis;
}
// Symbol: ??1CControlSiteFactoryMgr@@UEAA@XZ
extern "C" void MS_ABI impl___1CControlSiteFactoryMgr__UEAA_XZ(void* pThis) {
    S_Ccontrolsitefactorymgr* p = static_cast<S_Ccontrolsitefactorymgr*>(pThis);
    // Free every list node (RemoveAll); the factories themselves are not owned.
    list_destroy(&p->m_lstFactory);
    // The default factory is owned by the framework, not deleted here.
    p->m_pOleControlSiteDefaultFactory = nullptr;
}
// Symbol: ?RegisterSiteFactory@CControlSiteFactoryMgr@@QEAAHPEAVIControlSiteFactory@@@Z
extern "C" BOOL MS_ABI impl__RegisterSiteFactory_CControlSiteFactoryMgr__QEAAHPEAVIControlSiteFactory___Z(
        void* pThis, void* pFactory) {
    if (pFactory == nullptr)
        return FALSE;
    S_Ccontrolsitefactorymgr* p = static_cast<S_Ccontrolsitefactorymgr*>(pThis);
    list_add_tail(&p->m_lstFactory, pFactory);
    return TRUE;
}
// Symbol: ?UnregisterSiteFactory@CControlSiteFactoryMgr@@QEAAHPEAVIControlSiteFactory@@@Z
extern "C" BOOL MS_ABI impl__UnregisterSiteFactory_CControlSiteFactoryMgr__QEAAHPEAVIControlSiteFactory___Z(
        void* pThis, void* pFactory) {
    if (pFactory == nullptr)
        return FALSE;
    S_Ccontrolsitefactorymgr* p = static_cast<S_Ccontrolsitefactorymgr*>(pThis);
    return list_remove_value(&p->m_lstFactory, pFactory);
}
// Symbol: ?CreateSite@CControlSiteFactoryMgr@@QEAAPEAVCOleControlSite@@PEAVCOleControlContainer@@AEBVCControlCreationInfo@@@Z
extern "C" void* MS_ABI impl__CreateSite_CControlSiteFactoryMgr__QEAAPEAVCOleControlSite__PEAVCOleControlContainer__AEBVCControlCreationInfo___Z(
        void* pThis, void* pCtrlCont, const void* creationInfo) {
    S_Ccontrolsitefactorymgr* p = static_cast<S_Ccontrolsitefactorymgr*>(pThis);

    // Give each registered factory a chance to create the site; first non-null wins.
    for (CNode* node = p->m_lstFactory.m_pNodeHead; node != nullptr; node = node->pNext) {
        void* pFactory = node->data;
        if (pFactory == nullptr)
            continue;
        void** vtbl = *static_cast<void***>(pFactory);
        PFN_FactoryCreateSite pfn = reinterpret_cast<PFN_FactoryCreateSite>(vtbl[0]);
        void* pSite = pfn(pFactory, pCtrlCont, creationInfo);
        if (pSite != nullptr)
            return pSite;
    }

    // Fall back to the default OLE control-site factory when present.
    if (p->m_pOleControlSiteDefaultFactory != nullptr) {
        void* pDefault = p->m_pOleControlSiteDefaultFactory;
        void** vtbl = *static_cast<void***>(pDefault);
        PFN_FactoryCreateSite pfn = reinterpret_cast<PFN_FactoryCreateSite>(vtbl[0]);
        return pfn(pDefault, pCtrlCont, creationInfo);
    }
    return nullptr;
}

// CControlSiteFactoryMgr's MSVC-layout vtable: defined here, next to the
// implementations it points at.
namespace openmfc { namespace detail { namespace ccontrolsitefactorymgr {
extern void* const g_CControlSiteFactoryMgr_vtbl[1] = {
    reinterpret_cast<void*>(&vdtor_Ccontrolsitefactorymgr),
};
} } }
