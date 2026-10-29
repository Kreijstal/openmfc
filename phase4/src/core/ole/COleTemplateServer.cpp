// COleTemplateServer — OpenMFC implementation.
// Sources: olecore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/OlecoreSupport.h"

COleTemplateServer::COleTemplateServer()
    : m_pDocTemplate(nullptr) {
}
COleTemplateServer::~COleTemplateServer() {
}
void COleTemplateServer::ConnectTemplate(REFCLSID clsid, CDocTemplate* pDocTemplate, BOOL bMultiInstance) {
    m_clsid = clsid;
    m_pRuntimeClass = nullptr;
    m_bMultiInstance = bMultiInstance;
    m_pDocTemplate = pDocTemplate;
    COleObjectFactory::ConnectTemplate(this);
}
BOOL COleTemplateServer::Register() {
    return COleObjectFactory::Register();
}
BOOL COleTemplateServer::Unregister() {
    return COleObjectFactory::Unregister();
}
void COleTemplateServer::UpdateRegistry(OLE_APPTYPE nAppType,
                                         const wchar_t** rglpszRegister,
                                         const wchar_t** rglpszOverwrite,
                                         BOOL bRegister) {
    (void)nAppType; (void)rglpszRegister; (void)rglpszOverwrite;
    COleObjectFactory::UpdateRegistry(bRegister);
}
CCmdTarget* COleTemplateServer::OnCreateObject() {
    if (!m_pDocTemplate) {
        return nullptr;
    }
    CDocument* pDoc = m_pDocTemplate->CreateNewDocument();
    return pDoc ? static_cast<CCmdTarget*>(pDoc) : nullptr;
}
BOOL COleTemplateServer::OnCmdMsg(UINT nID, int nCode, void* pExtra,
                                  AFX_CMDHANDLERINFO* pHandlerInfo) {
    if (m_pDocTemplate && m_pDocTemplate->OnCmdMsg(nID, nCode, pExtra, pHandlerInfo)) {
        return TRUE;
    }
    return COleObjectFactory::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

// === Moved from ManualThunks.cpp ===
// Retail mfc140u RVA 0x26bb00 writes through retail-specific object offsets and conditionally creates rich-preview metadata. Use the OpenMFC member implementation for its declared layout; rich-preview metadata is not modeled here.
// Symbol: ?ConnectTemplate@COleTemplateServer@@QEAAXAEBU_GUID@@PEAVCDocTemplate@@HH@Z
extern "C" void MS_ABI impl__ConnectTemplate_COleTemplateServer__QEAAXAEBU_GUID__PEAVCDocTemplate__HH_Z(
    COleTemplateServer* pThis, REFCLSID clsid, CDocTemplate* pDocTemplate,
    BOOL bMultiInstance, BOOL bRegisterRichPreviewHandler) {
    (void)bRegisterRichPreviewHandler;
    pThis->ConnectTemplate(clsid, pDocTemplate, bMultiInstance);
}

