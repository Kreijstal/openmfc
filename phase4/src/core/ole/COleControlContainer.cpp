// COleControlContainer — OpenMFC implementation.
// Sources: ole_olectors_exports.cpp, olecore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/OlecoreSupport.h"

#include "openmfc/afxole.h"
#include "openmfc/afxmfc.h"

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

























// Symbol: ??0COleControlContainer@@QEAA@PEAVCWnd@@@Z
// COleControlContainer::COleControlContainer(CWnd* pWnd)
extern "C" void* MS_ABI impl___0COleControlContainer__QEAA_PEAVCWnd___Z(void* p0) {
    return new COleControlContainer(static_cast<CWnd*>(p0));
}
COleControlContainer::COleControlContainer(CWnd* pWnd)
    : m_pWnd(pWnd) {
    memset(_olecontrolcontainer_padding, 0, sizeof(_olecontrolcontainer_padding));
}
COleControlContainer::~COleControlContainer() {
    // Cleanup sites without using POSITION
    while (!m_listSites.IsEmpty()) {
        COleControlSite* pSite = (COleControlSite*)m_listSites.RemoveHead();
        delete pSite;
    }
}
COleControlSite* COleControlContainer::CreateSite(COleControlContainer* pCtrlCont) {
    COleControlSite* pSite = new COleControlSite(pCtrlCont);
    m_listSites.AddTail(pSite);
    return pSite;
}
BOOL COleControlContainer::DeleteSite(COleControlSite* pSite) {
    // Look for pSite in the list and remove it
    CPtrList::POSITION pos = m_listSites.Find(pSite);
    if (pos != CPtrList::POSITION(nullptr)) {
        m_listSites.RemoveAt(pos);
        delete pSite;
        return TRUE;
    }
    return FALSE;
}
COleControlSite* COleControlContainer::FindItem(UINT nID) const {
    CPtrList::POSITION pos = m_listSites.GetHeadPosition();
    while (pos != CPtrList::POSITION(nullptr)) {
        COleControlSite* pSite = static_cast<COleControlSite*>(m_listSites.GetNext(pos));
        if (!pSite) {
            continue;
        }

        UINT siteId = 0;
        if (TryGetControlSiteId(pSite, &siteId) && siteId == nID) {
            return pSite;
        }

        HWND hWnd = pSite->m_hWnd;
        if (hWnd && ::IsWindow(hWnd) && static_cast<UINT>(::GetDlgCtrlID(hWnd)) == nID) {
            return pSite;
        }
    }
    return nullptr;
}
