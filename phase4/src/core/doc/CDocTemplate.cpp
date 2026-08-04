// CDocTemplate — OpenMFC implementation.
// Sources: docview.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/DocviewSupport.h"


// Implementations this unit calls that are defined with their own class.
extern "C" void MS_ABI impl__OnIdle_CDocument__UEAAXXZ(CDocument* pThis);

// Symbol: ??0CDocTemplate@@IEAA@IPEAUCRuntimeClass@@00@Z
extern "C" void MS_ABI impl___0CDocTemplate__IEAA_IPEAUCRuntimeClass__00_Z(
    CDocTemplate* pThis, unsigned int nIDResource,
    CRuntimeClass* pDocClass, CRuntimeClass* pFrameClass, CRuntimeClass* pViewClass)
{
    impl___0CDocTemplate__QEAA_IPEAUCRuntimeClass__00_Z(
        pThis, nIDResource, pDocClass, pFrameClass, pViewClass);
}
// Destructor
// Symbol: ??1CDocTemplate@@UEAA@XZ
extern "C" void MS_ABI impl___1CDocTemplate__UEAA_XZ(CDocTemplate* pThis) {
    (void)pThis;
    // Note: Documents should be closed before template destruction
}
// CreateNewDocument
// Symbol: ?CreateNewDocument@CDocTemplate@@UEAAPEAVCDocument@@XZ
extern "C" CDocument* MS_ABI impl__CreateNewDocument_CDocTemplate__UEAAPEAVCDocument__XZ(
    CDocTemplate* pThis)
{
    if (!pThis || !pThis->m_pDocClass) return nullptr;

    CObject* pObj = pThis->m_pDocClass->CreateObject();
    if (!pObj) return nullptr;

    CDocument* pDoc = static_cast<CDocument*>(pObj);
    pThis->AddDocument(pDoc);
    return pDoc;
}
// CreateNewFrame
// Symbol: ?CreateNewFrame@CDocTemplate@@UEAAPEAVCFrameWnd@@PEAVCDocument@@PEAV2@@Z
extern "C" CFrameWnd* MS_ABI impl__CreateNewFrame_CDocTemplate__UEAAPEAVCFrameWnd__PEAVCDocument__PEAV2__Z(
    CDocTemplate* pThis, CDocument* pDoc, CFrameWnd* pOther)
{
    (void)pOther;
    if (!pThis || !pThis->m_pFrameClass) return nullptr;

    CObject* pObj = pThis->m_pFrameClass->CreateObject();
    if (!pObj) return nullptr;

    CFrameWnd* pFrame = static_cast<CFrameWnd*>(pObj);

    if (!pFrame->m_hWnd) {
        int frameCreated = FALSE;
        if (pThis->m_nIDResource != 0) {
            frameCreated = pFrame->CFrameWnd::LoadFrame(
                pThis->m_nIDResource, WS_OVERLAPPEDWINDOW, nullptr, nullptr);
        }

        if (!frameCreated) {
            RECT rect = { CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT };
            const wchar_t* title = (pDoc && pDoc->GetTitle() && pDoc->GetTitle()[0])
                ? pDoc->GetTitle()
                : L"OpenMFC Document";
            frameCreated = pFrame->CFrameWnd::Create(
                nullptr, title, WS_OVERLAPPEDWINDOW, rect, nullptr, nullptr, 0, nullptr);
        }

        if (!frameCreated) {
            return nullptr;
        }
    }

    if (pThis->m_pViewClass && pDoc) {
        CObject* pViewObj = pThis->m_pViewClass->CreateObject();
        if (pViewObj) {
            CView* pView = static_cast<CView*>(pViewObj);
            pDoc->AddView(pView);

            if (!pView->m_hWnd && pFrame->m_hWnd) {
                RECT rcClient = {};
                ::GetClientRect(pFrame->m_hWnd, &rcClient);
                pView->CWnd::Create(
                    nullptr,
                    nullptr,
                    WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
                    rcClient,
                    pFrame,
                    AFX_IDW_PANE_FIRST,
                    nullptr);
            }

            pFrame->m_pViewActive = pView;
        }
    }

    return pFrame;
}
// AddDocument
// Symbol: ?AddDocument@CDocTemplate@@UEAAXPEAVCDocument@@@Z
extern "C" void MS_ABI impl__AddDocument_CDocTemplate__UEAAXPEAVCDocument___Z(
    CDocTemplate* pThis, CDocument* pDoc)
{
    if (!pThis || !pDoc) return;

    pDoc->m_pNextDoc = nullptr;

    if (!pThis->m_pFirstDoc) {
        pThis->m_pFirstDoc = pDoc;
        pThis->m_pLastDoc = pDoc;
    } else {
        pThis->m_pLastDoc->m_pNextDoc = pDoc;
        pThis->m_pLastDoc = pDoc;
    }
}
// RemoveDocument
// Symbol: ?RemoveDocument@CDocTemplate@@UEAAXPEAVCDocument@@@Z
extern "C" void MS_ABI impl__RemoveDocument_CDocTemplate__UEAAXPEAVCDocument___Z(
    CDocTemplate* pThis, CDocument* pDoc)
{
    if (!pThis || !pDoc) return;

    CDocument* pPrev = nullptr;
    CDocument* pCur = pThis->m_pFirstDoc;
    while (pCur) {
        if (pCur == pDoc) {
            if (pPrev) {
                pPrev->m_pNextDoc = pCur->m_pNextDoc;
            } else {
                pThis->m_pFirstDoc = pCur->m_pNextDoc;
            }
            if (pThis->m_pLastDoc == pDoc) {
                pThis->m_pLastDoc = pPrev;
            }
            pDoc->m_pNextDoc = nullptr;
            break;
        }
        pPrev = pCur;
        pCur = pCur->m_pNextDoc;
    }
}
// InitialUpdateFrame
// Symbol: ?InitialUpdateFrame@CDocTemplate@@UEAAXPEAVCFrameWnd@@PEAVCDocument@@H@Z
extern "C" void MS_ABI impl__InitialUpdateFrame_CDocTemplate__UEAAXPEAVCFrameWnd__PEAVCDocument__H_Z(
    CDocTemplate* pThis, CFrameWnd* pFrame, CDocument* pDoc, int bMakeVisible)
{
    (void)pThis;

    if (pDoc) {
        // Call OnInitialUpdate on all views
        void* pos = impl__GetFirstViewPosition_CDocument__UEBAPEAXXZ(pDoc);
        while (pos) {
            CView* pView = impl__GetNextView_CDocument__UEBAPEAVCView__AEAPEAX_Z(pDoc, pos);
            if (pView) {
                pView->OnInitialUpdate();
            }
        }
    }

    if (pFrame && pFrame->m_hWnd) {
        pFrame->CFrameWnd::RecalcLayout(TRUE);
        if (bMakeVisible) {
            pFrame->CFrameWnd::ActivateFrame(SW_SHOW);
        }
    }
}
// Symbol: ?GetDocString@CDocTemplate@@UEBAHAEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@W4DocStringIndex@1@@Z
extern "C" int MS_ABI impl__GetDocString_CDocTemplate__UEBAHAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__W4DocStringIndex_1__Z(
    const CDocTemplate* pThis, CString* rString, int nID)
{
    return impl__GetDocString_CDocTemplate__UEBAHAEAVCString__H_Z(pThis, rString, nID);
}
// Symbol: ?MatchDocType@CDocTemplate@@UEAA?AW4Confidence@1@PEB_WAEAPEAVCDocument@@@Z
extern "C" int MS_ABI impl__MatchDocType_CDocTemplate__UEAA_AW4Confidence_1_PEB_WAEAPEAVCDocument___Z(
    CDocTemplate* pThis, const wchar_t* lpszPathName, CDocument*& rpDocMatch)
{
    return impl__MatchDocType_CDocTemplate__UEAAHPEB_WAEAPEAVCDocument___Z(
        pThis, lpszPathName, rpDocMatch);
}
// Symbol: ?GetRuntimeClass@CDocTemplate@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CDocTemplate__UEBAPEAUCRuntimeClass__XZ(const CDocTemplate*) { return &CDocTemplate::classCDocTemplate; }
// Symbol: ?GetThisClass@CDocTemplate@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CDocTemplate__SAPEAUCRuntimeClass__XZ() { return &CDocTemplate::classCDocTemplate; }
// CDocTemplate residuals.
// Symbol: ?LoadTemplate@CDocTemplate@@UEAAXXZ
extern "C" void MS_ABI impl__LoadTemplate_CDocTemplate__UEAAXXZ(CDocTemplate* pThis) { if (pThis) { CString s; (void)pThis->GetDocString(s, kDocName); } }
// Symbol: ?SaveAllModified@CDocTemplate@@UEAAHXZ
extern "C" int MS_ABI impl__SaveAllModified_CDocTemplate__UEAAHXZ(CDocTemplate* pThis) { if (!pThis) return FALSE; for (void* pos=pThis->GetFirstDocPosition(); pos;) { CDocument* d=pThis->GetNextDoc(pos); if (d && !d->SaveModified()) return FALSE; } return TRUE; }
// Symbol: ?CloseAllDocuments@CDocTemplate@@UEAAXH@Z
extern "C" void MS_ABI impl__CloseAllDocuments_CDocTemplate__UEAAXH_Z(CDocTemplate* pThis, int) { if (!pThis) return; std::vector<CDocument*> docs; for (void* pos=pThis->GetFirstDocPosition(); pos;) if (CDocument* d=pThis->GetNextDoc(pos)) docs.push_back(d); for (CDocument* d: docs) d->OnCloseDocument(); }
// Symbol: ?OnIdle@CDocTemplate@@UEAAXXZ
extern "C" void MS_ABI impl__OnIdle_CDocTemplate__UEAAXXZ(CDocTemplate* pThis) { if (!pThis) return; for (void* pos=pThis->GetFirstDocPosition(); pos;) if (CDocument* d=pThis->GetNextDoc(pos)) impl__OnIdle_CDocument__UEAAXXZ(d); }
// Symbol: ?OnCmdMsg@CDocTemplate@@UEAAHIHPEAXPEAUAFX_CMDHANDLERINFO@@@Z
extern "C" int MS_ABI impl__OnCmdMsg_CDocTemplate__UEAAHIHPEAXPEAUAFX_CMDHANDLERINFO___Z(CDocTemplate* pThis, unsigned int id, int code, void* extra, AFX_CMDHANDLERINFO* info) { return pThis ? pThis->CCmdTarget::OnCmdMsg(id, code, extra, info) : FALSE; }
// Symbol: ?SetContainerInfo@CDocTemplate@@QEAAXI@Z
extern "C" void MS_ABI impl__SetContainerInfo_CDocTemplate__QEAAXI_Z(CDocTemplate* pThis, unsigned int id) { if (pThis) g_templateExtraStates[pThis].containerId = id; }
// Symbol: ?SetServerInfo@CDocTemplate@@QEAAXIIPEAUCRuntimeClass@@0@Z
extern "C" void MS_ABI impl__SetServerInfo_CDocTemplate__QEAAXIIPEAUCRuntimeClass__0_Z(CDocTemplate* pThis, unsigned int id, unsigned int embedId, CRuntimeClass* docClass, CRuntimeClass* frameClass) { if (pThis) { auto& s=g_templateExtraStates[pThis]; s.serverId=id; s.serverEmbeddingId=embedId; s.serverDocClass=docClass; s.serverFrameClass=frameClass; } }
// Symbol: ?SetPreviewInfo@CDocTemplate@@QEAAXIPEAUCRuntimeClass@@0@Z
extern "C" void MS_ABI impl__SetPreviewInfo_CDocTemplate__QEAAXIPEAUCRuntimeClass__0_Z(CDocTemplate* pThis, unsigned int id, CRuntimeClass* viewClass, CRuntimeClass* frameClass) { if (pThis) { auto& s=g_templateExtraStates[pThis]; s.previewId=id; s.previewViewClass=viewClass; s.previewFrameClass=frameClass; } }
// Symbol: ?CreatePreviewFrame@CDocTemplate@@QEAAPEAVCFrameWnd@@PEAVCWnd@@PEAVCDocument@@@Z
extern "C" CFrameWnd* MS_ABI impl__CreatePreviewFrame_CDocTemplate__QEAAPEAVCFrameWnd__PEAVCWnd__PEAVCDocument___Z(CDocTemplate* pThis, CWnd*, CDocument* doc) { return pThis ? pThis->CreateNewFrame(doc, nullptr) : nullptr; }
// CDocTemplate::CreateOleFrame — builds the in-place container frame used when a
// document is activated inside an OLE container.
//
// Retail mfc140u reads the OLE frame/view runtime classes SetContainerInfo()
// stashed at this+0xC8 / this+0xD0, fills a CCreateContext (m_pCurrentDoc = pDoc,
// m_pCurrentDocTemplate = this, m_pNewViewClass = the OLE view class only when
// bCreateView is set), and creates the frame from it. With SetContainerInfo()
// never called those pointers are null and retail yields no frame.
//
// OpenMFC keeps the container registration as the resource id recorded by
// SetContainerInfo (g_templateExtraStates), so the same contract is honoured
// here: no container id means no OLE frame, otherwise the frame is loaded from
// that id and the view is attached only when bCreateView requests it.
// Symbol: ?CreateOleFrame@CDocTemplate@@QEAAPEAVCFrameWnd@@PEAVCWnd@@PEAVCDocument@@H@Z
extern "C" CFrameWnd* MS_ABI impl__CreateOleFrame_CDocTemplate__QEAAPEAVCFrameWnd__PEAVCWnd__PEAVCDocument__H_Z(
    CDocTemplate* pThis, CWnd* pParentWnd, CDocument* pDoc, int bCreateView)
{
    if (!pThis || !pThis->m_pFrameClass) return nullptr;

    auto itState = g_templateExtraStates.find(pThis);
    const unsigned int containerId =
        (itState == g_templateExtraStates.end()) ? 0u : itState->second.containerId;
    if (containerId == 0) return nullptr;   // SetContainerInfo() never called

    CObject* pObj = pThis->m_pFrameClass->CreateObject();
    if (!pObj) return nullptr;
    CFrameWnd* pFrame = static_cast<CFrameWnd*>(pObj);

    if (!pFrame->m_hWnd) {
        if (!pFrame->CFrameWnd::LoadFrame(containerId, WS_OVERLAPPEDWINDOW, pParentWnd, nullptr)) {
            RECT rect = { CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT };
            if (!pFrame->CFrameWnd::Create(nullptr, L"OpenMFC Container",
                                           WS_OVERLAPPEDWINDOW, rect, pParentWnd,
                                           nullptr, 0, nullptr)) {
                // CreateObject() handed us ownership; nothing else can free the
                // frame once we return null, so release it here.
                delete pFrame;
                return nullptr;
            }
        }
    }

    if (bCreateView && pThis->m_pViewClass && pDoc) {
        CObject* pViewObj = pThis->m_pViewClass->CreateObject();
        if (pViewObj) {
            CView* pView = static_cast<CView*>(pViewObj);
            // Create the view window BEFORE registering it with the document.
            // Registering first would leave a windowless view attached to the
            // document (and installed as m_pViewActive) if creation failed.
            BOOL bViewReady = (pView->m_hWnd != nullptr);
            if (!bViewReady && pFrame->m_hWnd) {
                RECT rcClient = {};
                ::GetClientRect(pFrame->m_hWnd, &rcClient);
                bViewReady = pView->CWnd::Create(
                    nullptr, nullptr,
                    WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
                    rcClient, pFrame, AFX_IDW_PANE_FIRST, nullptr);
            }
            if (bViewReady) {
                pDoc->AddView(pView);
                pFrame->m_pViewActive = pView;
            } else {
                delete pView;   // never reached the document; we still own it
            }
        }
    }
    return pFrame;
}
CDocTemplate::CDocTemplate(unsigned int nIDResource, CRuntimeClass* pDocClass,
                           CRuntimeClass* pFrameClass, CRuntimeClass* pViewClass) {
    impl___0CDocTemplate__QEAA_IPEAUCRuntimeClass__00_Z(
        this, nIDResource, pDocClass, pFrameClass, pViewClass);
}
CDocTemplate::CDocTemplate() {
    m_nIDResource = 0;
    m_pDocClass = nullptr;
    m_pFrameClass = nullptr;
    m_pViewClass = nullptr;
    m_pFirstDoc = nullptr;
    m_pLastDoc = nullptr;
}
CDocTemplate::~CDocTemplate() { impl___1CDocTemplate__UEAA_XZ(this); }
CDocument* CDocTemplate::CreateNewDocument() { return impl__CreateNewDocument_CDocTemplate__UEAAPEAVCDocument__XZ(this); }
CFrameWnd* CDocTemplate::CreateNewFrame(CDocument* pDoc, CFrameWnd* pOther) { return impl__CreateNewFrame_CDocTemplate__UEAAPEAVCFrameWnd__PEAVCDocument__PEAV2__Z(this, pDoc, pOther); }
int CDocTemplate::CreateAndReplaceFrame(CFrameWnd* pFrame, CDocument* pDoc) {
    if (!pFrame || !pDoc) return FALSE;

    // Create a new frame to replace the existing one
    CFrameWnd* pNewFrame = CreateNewFrame(pDoc, pFrame);
    if (!pNewFrame) return FALSE;

    // Copy window position from old frame
    if (pFrame->m_hWnd && pNewFrame->m_hWnd) {
        RECT rc;
        ::GetWindowRect(pFrame->m_hWnd, &rc);
        ::SetWindowPos(pNewFrame->m_hWnd, nullptr,
                       rc.left, rc.top,
                       rc.right - rc.left, rc.bottom - rc.top,
                       SWP_NOZORDER | SWP_NOACTIVATE);

        // Destroy the old frame
        ::DestroyWindow(pFrame->m_hWnd);
        pFrame->m_hWnd = nullptr;
    }

    // Initialize the new frame
    InitialUpdateFrame(pNewFrame, pDoc, TRUE);

    return TRUE;
}
CDocument* CDocTemplate::OpenDocumentFile(const wchar_t* lpszPathName, int bMakeVisible) { return impl__OpenDocumentFile_CDocTemplate__UEAAPEAVCDocument__PEB_WH_Z(this, lpszPathName, bMakeVisible); }
void CDocTemplate::InitialUpdateFrame(CFrameWnd* pFrame, CDocument* pDoc, int bMakeVisible) { impl__InitialUpdateFrame_CDocTemplate__UEAAXPEAVCFrameWnd__PEAVCDocument__H_Z(this, pFrame, pDoc, bMakeVisible); }
void CDocTemplate::SetDefaultTitle(CDocument* pDoc) { impl__SetDefaultTitle_CDocTemplate__UEAAXPEAVCDocument___Z(this, pDoc); }
int CDocTemplate::GetDocString(CString& rString, int nID) const { return impl__GetDocString_CDocTemplate__UEBAHAEAVCString__H_Z(this, &rString, nID); }
int CDocTemplate::MatchDocType(const wchar_t* lpszPathName, CDocument*& rpDocMatch) { return impl__MatchDocType_CDocTemplate__UEAAHPEB_WAEAPEAVCDocument___Z(this, lpszPathName, rpDocMatch); }
void CDocTemplate::AddDocument(CDocument* pDoc) { impl__AddDocument_CDocTemplate__UEAAXPEAVCDocument___Z(this, pDoc); }
void CDocTemplate::RemoveDocument(CDocument* pDoc) { impl__RemoveDocument_CDocTemplate__UEAAXPEAVCDocument___Z(this, pDoc); }
void* CDocTemplate::GetFirstDocPosition() const { return impl__GetFirstDocPosition_CDocTemplate__UEBAPEAXXZ(this); }
CDocument* CDocTemplate::GetNextDoc(void*& rPos) const { return impl__GetNextDoc_CDocTemplate__UEBAPEAVCDocument__AEAPEAX_Z(this, rPos); }
