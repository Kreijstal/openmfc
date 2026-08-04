// CMultiDocTemplate — OpenMFC implementation.
// Sources: docview.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/DocviewSupport.h"


// Implementations this unit calls that are defined with their own class.
extern "C" void MS_ABI impl__LoadTemplate_CDocTemplate__UEAAXXZ(CDocTemplate* pThis);

// Constructor with parameters
// Symbol: ??0CMultiDocTemplate@@QEAA@IPEAUCRuntimeClass@@00@Z
extern "C" void MS_ABI impl___0CMultiDocTemplate__QEAA_IPEAUCRuntimeClass__00_Z(
    CMultiDocTemplate* pThis, unsigned int nIDResource,
    CRuntimeClass* pDocClass, CRuntimeClass* pFrameClass, CRuntimeClass* pViewClass)
{
    impl___0CDocTemplate__QEAA_IPEAUCRuntimeClass__00_Z(
        pThis, nIDResource, pDocClass, pFrameClass, pViewClass);
}
// Destructor
// Symbol: ??1CMultiDocTemplate@@UEAA@XZ
extern "C" void MS_ABI impl___1CMultiDocTemplate__UEAA_XZ(CMultiDocTemplate* pThis) {
    impl___1CDocTemplate__UEAA_XZ(pThis);
}
// OpenDocumentFile (CMultiDocTemplate override)
// Symbol: ?OpenDocumentFile@CMultiDocTemplate@@UEAAPEAVCDocument@@PEB_WH@Z
extern "C" CDocument* MS_ABI impl__OpenDocumentFile_CMultiDocTemplate__UEAAPEAVCDocument__PEB_WH_Z(
    CMultiDocTemplate* pThis, const wchar_t* lpszPathName, int bMakeVisible)
{
    // MDI always creates a new document
    return impl__OpenDocumentFile_CDocTemplate__UEAAPEAVCDocument__PEB_WH_Z(
        pThis, lpszPathName, bMakeVisible);
}
// Symbol: ?OpenDocumentFile@CMultiDocTemplate@@UEAAPEAVCDocument@@PEB_WHH@Z
extern "C" CDocument* MS_ABI impl__OpenDocumentFile_CMultiDocTemplate__UEAAPEAVCDocument__PEB_WHH_Z(
    CMultiDocTemplate* pThis, const wchar_t* lpszPathName, int bMakeVisible, int bAddToMRU)
{
    (void)bAddToMRU;
    return impl__OpenDocumentFile_CMultiDocTemplate__UEAAPEAVCDocument__PEB_WH_Z(
        pThis, lpszPathName, bMakeVisible);
}
// Symbol: ?AddDocument@CMultiDocTemplate@@UEAAXPEAVCDocument@@@Z
extern "C" void MS_ABI impl__AddDocument_CMultiDocTemplate__UEAAXPEAVCDocument___Z(
    CMultiDocTemplate* pThis, CDocument* pDoc)
{
    impl__AddDocument_CDocTemplate__UEAAXPEAVCDocument___Z(pThis, pDoc);
}
// Symbol: ?RemoveDocument@CMultiDocTemplate@@UEAAXPEAVCDocument@@@Z
extern "C" void MS_ABI impl__RemoveDocument_CMultiDocTemplate__UEAAXPEAVCDocument___Z(
    CMultiDocTemplate* pThis, CDocument* pDoc)
{
    impl__RemoveDocument_CDocTemplate__UEAAXPEAVCDocument___Z(pThis, pDoc);
}
// Symbol: ?GetFirstDocPosition@CMultiDocTemplate@@UEBAPEAU__POSITION@@XZ
extern "C" void* MS_ABI impl__GetFirstDocPosition_CMultiDocTemplate__UEBAPEAU__POSITION__XZ(
    const CMultiDocTemplate* pThis)
{
    return impl__GetFirstDocPosition_CDocTemplate__UEBAPEAXXZ(pThis);
}
// Symbol: ?GetNextDoc@CMultiDocTemplate@@UEBAPEAVCDocument@@AEAPEAU__POSITION@@@Z
extern "C" CDocument* MS_ABI impl__GetNextDoc_CMultiDocTemplate__UEBAPEAVCDocument__AEAPEAU__POSITION___Z(
    const CMultiDocTemplate* pThis, void*& pos)
{
    return impl__GetNextDoc_CDocTemplate__UEBAPEAVCDocument__AEAPEAX_Z(pThis, pos);
}
// Symbol: ?SetDefaultTitle@CMultiDocTemplate@@UEAAXPEAVCDocument@@@Z
extern "C" void MS_ABI impl__SetDefaultTitle_CMultiDocTemplate__UEAAXPEAVCDocument___Z(
    CMultiDocTemplate* pThis, CDocument* pDoc)
{
    impl__SetDefaultTitle_CDocTemplate__UEAAXPEAVCDocument___Z(pThis, pDoc);
}
// Symbol: ?GetRuntimeClass@CMultiDocTemplate@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CMultiDocTemplate__UEBAPEAUCRuntimeClass__XZ(const CMultiDocTemplate*) { return &CMultiDocTemplate::classCMultiDocTemplate; }
// Symbol: ?GetThisClass@CMultiDocTemplate@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMultiDocTemplate__SAPEAUCRuntimeClass__XZ() { return &CMultiDocTemplate::classCMultiDocTemplate; }
// Symbol: ?LoadTemplate@CMultiDocTemplate@@UEAAXXZ
extern "C" void MS_ABI impl__LoadTemplate_CMultiDocTemplate__UEAAXXZ(CMultiDocTemplate* pThis) { impl__LoadTemplate_CDocTemplate__UEAAXXZ(pThis); }
CMultiDocTemplate::CMultiDocTemplate() { impl___0CMultiDocTemplate__QEAA_XZ(this); }
CMultiDocTemplate::CMultiDocTemplate(unsigned int nIDResource, CRuntimeClass* pDocClass,
                                     CRuntimeClass* pFrameClass, CRuntimeClass* pViewClass) {
    impl___0CMultiDocTemplate__QEAA_IPEAUCRuntimeClass__00_Z(
        this, nIDResource, pDocClass, pFrameClass, pViewClass);
}
CMultiDocTemplate::~CMultiDocTemplate() { impl___1CMultiDocTemplate__UEAA_XZ(this); }
CDocument* CMultiDocTemplate::OpenDocumentFile(const wchar_t* lpszPathName, int bMakeVisible) { return impl__OpenDocumentFile_CMultiDocTemplate__UEAAPEAVCDocument__PEB_WH_Z(this, lpszPathName, bMakeVisible); }
void CMultiDocTemplate::AddDocument(CDocument* pDoc) { impl__AddDocument_CMultiDocTemplate__UEAAXPEAVCDocument___Z(this, pDoc); }
void CMultiDocTemplate::RemoveDocument(CDocument* pDoc) { impl__RemoveDocument_CMultiDocTemplate__UEAAXPEAVCDocument___Z(this, pDoc); }
void* CMultiDocTemplate::GetFirstDocPosition() const { return impl__GetFirstDocPosition_CMultiDocTemplate__UEBAPEAU__POSITION__XZ(this); }
CDocument* CMultiDocTemplate::GetNextDoc(void*& rPos) const { return impl__GetNextDoc_CMultiDocTemplate__UEBAPEAVCDocument__AEAPEAU__POSITION___Z(this, rPos); }
void CMultiDocTemplate::SetDefaultTitle(CDocument* pDoc) { impl__SetDefaultTitle_CMultiDocTemplate__UEAAXPEAVCDocument___Z(this, pDoc); }
