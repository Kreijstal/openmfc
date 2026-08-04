// CSingleDocTemplate — OpenMFC implementation.
// Sources: docview.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/DocviewSupport.h"

// Constructor with parameters
// Symbol: ??0CSingleDocTemplate@@QEAA@IPEAUCRuntimeClass@@00@Z
extern "C" void MS_ABI impl___0CSingleDocTemplate__QEAA_IPEAUCRuntimeClass__00_Z(
    CSingleDocTemplate* pThis, unsigned int nIDResource,
    CRuntimeClass* pDocClass, CRuntimeClass* pFrameClass, CRuntimeClass* pViewClass)
{
    impl___0CDocTemplate__QEAA_IPEAUCRuntimeClass__00_Z(
        pThis, nIDResource, pDocClass, pFrameClass, pViewClass);
    pThis->m_pOnlyDoc = nullptr;
}
// Destructor
// Symbol: ??1CSingleDocTemplate@@UEAA@XZ
extern "C" void MS_ABI impl___1CSingleDocTemplate__UEAA_XZ(CSingleDocTemplate* pThis) {
    impl___1CDocTemplate__UEAA_XZ(pThis);
}
// OpenDocumentFile (CSingleDocTemplate override)
// Symbol: ?OpenDocumentFile@CSingleDocTemplate@@UEAAPEAVCDocument@@PEB_WH@Z
extern "C" CDocument* MS_ABI impl__OpenDocumentFile_CSingleDocTemplate__UEAAPEAVCDocument__PEB_WH_Z(
    CSingleDocTemplate* pThis, const wchar_t* lpszPathName, int bMakeVisible)
{
    if (!pThis) return nullptr;

    // In SDI, we reuse the existing document
    if (pThis->m_pOnlyDoc) {
        if (!pThis->m_pOnlyDoc->SaveModified()) {
            return nullptr;
        }
        pThis->m_pOnlyDoc->DeleteContents();

        if (lpszPathName) {
            if (!pThis->m_pOnlyDoc->OnOpenDocument(lpszPathName)) {
                return nullptr;
            }
            pThis->m_pOnlyDoc->SetPathName(lpszPathName);
        } else {
            if (!pThis->m_pOnlyDoc->OnNewDocument()) {
                return nullptr;
            }
            pThis->SetDefaultTitle(pThis->m_pOnlyDoc);
        }

        if (!pThis->m_pOnlyDoc->GetFirstViewPosition()) {
            CFrameWnd* pFrame = impl__CreateNewFrame_CDocTemplate__UEAAPEAVCFrameWnd__PEAVCDocument__PEAV2__Z(
                pThis, pThis->m_pOnlyDoc, nullptr);
            if (pFrame) {
                impl__InitialUpdateFrame_CDocTemplate__UEAAXPEAVCFrameWnd__PEAVCDocument__H_Z(
                    pThis, pFrame, pThis->m_pOnlyDoc, bMakeVisible);
            }
        }

        return pThis->m_pOnlyDoc;
    }

    // Create new document
    return impl__OpenDocumentFile_CDocTemplate__UEAAPEAVCDocument__PEB_WH_Z(
        pThis, lpszPathName, bMakeVisible);
}
// Symbol: ?OpenDocumentFile@CSingleDocTemplate@@UEAAPEAVCDocument@@PEB_WHH@Z
extern "C" CDocument* MS_ABI impl__OpenDocumentFile_CSingleDocTemplate__UEAAPEAVCDocument__PEB_WHH_Z(
    CSingleDocTemplate* pThis, const wchar_t* lpszPathName, int bMakeVisible, int bAddToMRU)
{
    (void)bAddToMRU;
    return impl__OpenDocumentFile_CSingleDocTemplate__UEAAPEAVCDocument__PEB_WH_Z(
        pThis, lpszPathName, bMakeVisible);
}
// AddDocument (CSingleDocTemplate override)
// Symbol: ?AddDocument@CSingleDocTemplate@@UEAAXPEAVCDocument@@@Z
extern "C" void MS_ABI impl__AddDocument_CSingleDocTemplate__UEAAXPEAVCDocument___Z(
    CSingleDocTemplate* pThis, CDocument* pDoc)
{
    if (!pThis || !pDoc) return;

    pThis->m_pOnlyDoc = pDoc;
    impl__AddDocument_CDocTemplate__UEAAXPEAVCDocument___Z(pThis, pDoc);
}
// RemoveDocument (CSingleDocTemplate override)
// Symbol: ?RemoveDocument@CSingleDocTemplate@@UEAAXPEAVCDocument@@@Z
extern "C" void MS_ABI impl__RemoveDocument_CSingleDocTemplate__UEAAXPEAVCDocument___Z(
    CSingleDocTemplate* pThis, CDocument* pDoc)
{
    if (!pThis || !pDoc) return;

    if (pThis->m_pOnlyDoc == pDoc) {
        pThis->m_pOnlyDoc = nullptr;
    }
    impl__RemoveDocument_CDocTemplate__UEAAXPEAVCDocument___Z(pThis, pDoc);
}
// Symbol: ?GetFirstDocPosition@CSingleDocTemplate@@UEBAPEAU__POSITION@@XZ
extern "C" void* MS_ABI impl__GetFirstDocPosition_CSingleDocTemplate__UEBAPEAU__POSITION__XZ(
    const CSingleDocTemplate* pThis)
{
    return impl__GetFirstDocPosition_CDocTemplate__UEBAPEAXXZ(pThis);
}
// Symbol: ?GetNextDoc@CSingleDocTemplate@@UEBAPEAVCDocument@@AEAPEAU__POSITION@@@Z
extern "C" CDocument* MS_ABI impl__GetNextDoc_CSingleDocTemplate__UEBAPEAVCDocument__AEAPEAU__POSITION___Z(
    const CSingleDocTemplate* pThis, void*& pos)
{
    return impl__GetNextDoc_CDocTemplate__UEBAPEAVCDocument__AEAPEAX_Z(pThis, pos);
}
// Symbol: ?SetDefaultTitle@CSingleDocTemplate@@UEAAXPEAVCDocument@@@Z
extern "C" void MS_ABI impl__SetDefaultTitle_CSingleDocTemplate__UEAAXPEAVCDocument___Z(
    CSingleDocTemplate* pThis, CDocument* pDoc)
{
    impl__SetDefaultTitle_CDocTemplate__UEAAXPEAVCDocument___Z(pThis, pDoc);
}
// Symbol: ?GetRuntimeClass@CSingleDocTemplate@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CSingleDocTemplate__UEBAPEAUCRuntimeClass__XZ(const CSingleDocTemplate*) { return &CSingleDocTemplate::classCSingleDocTemplate; }
// Symbol: ?GetThisClass@CSingleDocTemplate@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CSingleDocTemplate__SAPEAUCRuntimeClass__XZ() { return &CSingleDocTemplate::classCSingleDocTemplate; }
CSingleDocTemplate::CSingleDocTemplate() { impl___0CSingleDocTemplate__QEAA_XZ(this); }
CSingleDocTemplate::CSingleDocTemplate(unsigned int nIDResource, CRuntimeClass* pDocClass,
                                       CRuntimeClass* pFrameClass, CRuntimeClass* pViewClass) {
    impl___0CSingleDocTemplate__QEAA_IPEAUCRuntimeClass__00_Z(
        this, nIDResource, pDocClass, pFrameClass, pViewClass);
}
CSingleDocTemplate::~CSingleDocTemplate() { impl___1CSingleDocTemplate__UEAA_XZ(this); }
CDocument* CSingleDocTemplate::OpenDocumentFile(const wchar_t* lpszPathName, int bMakeVisible) { return impl__OpenDocumentFile_CSingleDocTemplate__UEAAPEAVCDocument__PEB_WH_Z(this, lpszPathName, bMakeVisible); }
void CSingleDocTemplate::AddDocument(CDocument* pDoc) { impl__AddDocument_CSingleDocTemplate__UEAAXPEAVCDocument___Z(this, pDoc); }
void CSingleDocTemplate::RemoveDocument(CDocument* pDoc) { impl__RemoveDocument_CSingleDocTemplate__UEAAXPEAVCDocument___Z(this, pDoc); }
void* CSingleDocTemplate::GetFirstDocPosition() const { return impl__GetFirstDocPosition_CSingleDocTemplate__UEBAPEAU__POSITION__XZ(this); }
CDocument* CSingleDocTemplate::GetNextDoc(void*& rPos) const { return impl__GetNextDoc_CSingleDocTemplate__UEBAPEAVCDocument__AEAPEAU__POSITION___Z(this, rPos); }
void CSingleDocTemplate::SetDefaultTitle(CDocument* pDoc) { impl__SetDefaultTitle_CSingleDocTemplate__UEAAXPEAVCDocument___Z(this, pDoc); }
