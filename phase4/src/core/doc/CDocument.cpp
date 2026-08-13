// CDocument — OpenMFC implementation.
// Sources: docview.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/DocviewSupport.h"

// Default Constructor
// Symbol: ??0CDocument@@QEAA@XZ
extern "C" void MS_ABI impl___0CDocument__QEAA_XZ(CDocument* pThis) {
    if (!pThis) return;
    // Initialize members
    pThis->m_bModified = FALSE;
    pThis->m_bAutoDelete = TRUE;
    pThis->m_pFirstView = nullptr;
    pThis->m_pLastView = nullptr;
    pThis->m_pNextDoc = nullptr;
}
// Destructor
// Symbol: ??1CDocument@@UEAA@XZ
extern "C" void MS_ABI impl___1CDocument__UEAA_XZ(CDocument* pThis) {
    if (!pThis) return;
    // Disconnect all views
    CView* pView = pThis->m_pFirstView;
    while (pView) {
        CView* pNext = pView->m_pNextView;
        pView->m_pDocument = nullptr;
        pView = pNext;
    }
    pThis->m_pFirstView = nullptr;
    pThis->m_pLastView = nullptr;
    g_documentExtraStates.erase(pThis);
}
// OnNewDocument
// Symbol: ?OnNewDocument@CDocument@@UEAAHXZ
extern "C" int MS_ABI impl__OnNewDocument_CDocument__UEAAHXZ(CDocument* pThis) {
    if (!pThis) return FALSE;
    pThis->DeleteContents();
    pThis->SetModifiedFlag(FALSE);
    return TRUE;
}
// OnOpenDocument
// Symbol: ?OnOpenDocument@CDocument@@UEAAHPEB_W@Z
extern "C" int MS_ABI impl__OnOpenDocument_CDocument__UEAAHPEB_W_Z(
    CDocument* pThis, const wchar_t* lpszPathName)
{
    if (!pThis || !lpszPathName || !lpszPathName[0]) return FALSE;
    pThis->DeleteContents();
    CFile file;
    CFileException fileException;
    if (!impl__Open_CFile__UEAAHPEB_WIPEAVCFileException___Z(&file, lpszPathName, CFile::modeRead | CFile::shareDenyWrite, &fileException)) {
        return FALSE;
    }
    CArchive archive(&file, CArchive::load);
    pThis->Serialize(archive);
    archive.Close();
    file.Close();
    pThis->SetPathName(lpszPathName, TRUE);
    pThis->SetModifiedFlag(FALSE);
    return TRUE;
}
// OnSaveDocument
// Symbol: ?OnSaveDocument@CDocument@@UEAAHPEB_W@Z
extern "C" int MS_ABI impl__OnSaveDocument_CDocument__UEAAHPEB_W_Z(
    CDocument* pThis, const wchar_t* lpszPathName)
{
    if (!pThis || !lpszPathName || !lpszPathName[0]) return FALSE;
    CFile file;
    CFileException fileException;
    if (!impl__Open_CFile__UEAAHPEB_WIPEAVCFileException___Z(&file, lpszPathName, CFile::modeCreate | CFile::modeWrite | CFile::shareExclusive, &fileException)) {
        return FALSE;
    }
    CArchive archive(&file, CArchive::store);
    pThis->Serialize(archive);
    archive.Close();
    file.Close();
    pThis->SetPathName(lpszPathName, TRUE);
    pThis->SetModifiedFlag(FALSE);
    return TRUE;
}
// OnCloseDocument
// Symbol: ?OnCloseDocument@CDocument@@UEAAXXZ
extern "C" void MS_ABI impl__OnCloseDocument_CDocument__UEAAXXZ(CDocument* pThis) {
    if (!pThis) return;

    // Destroy all views
    CView* pView = pThis->m_pFirstView;
    while (pView) {
        CView* pNext = pView->m_pNextView;
        if (pView->m_hWnd) {
            ::DestroyWindow(pView->m_hWnd);
        }
        pView->m_pDocument = nullptr;
        pView = pNext;
    }
    pThis->m_pFirstView = nullptr;
    pThis->m_pLastView = nullptr;

    // Auto-delete if enabled
    if (pThis->m_bAutoDelete) {
        delete pThis;
    }
}
// DeleteContents
// Symbol: ?DeleteContents@CDocument@@UEAAXXZ
extern "C" void MS_ABI impl__DeleteContents_CDocument__UEAAXXZ(CDocument* pThis) {
    if (pThis) g_documentExtraStates[pThis].chunks.clear();
}
// SetTitle
// Symbol: ?SetTitle@CDocument@@UEAAXPEB_W@Z
extern "C" void MS_ABI impl__SetTitle_CDocument__UEAAXPEB_W_Z(
    CDocument* pThis, const wchar_t* lpszTitle)
{
    if (pThis) {
        pThis->m_strTitle = lpszTitle ? lpszTitle : L"";
    }
}
// SetPathName
// Symbol: ?SetPathName@CDocument@@UEAAXPEB_WH@Z
extern "C" void MS_ABI impl__SetPathName_CDocument__UEAAXPEB_WH_Z(
    CDocument* pThis, const wchar_t* lpszPathName, int bAddToMRU)
{
    (void)bAddToMRU;
    if (pThis) {
        pThis->m_strPathName = lpszPathName ? lpszPathName : L"";
        if (!pThis->m_strPathName.IsEmpty()) {
            const wchar_t* path = pThis->m_strPathName;
            const wchar_t* slash = wcsrchr(path, L'\\');
            const wchar_t* fslash = wcsrchr(path, L'/');
            const wchar_t* name = slash > fslash ? slash : fslash;
            pThis->m_strTitle = name ? name + 1 : path;
        }
    }
}
// SaveModified
// Symbol: ?SaveModified@CDocument@@UEAAHXZ
extern "C" int MS_ABI impl__SaveModified_CDocument__UEAAHXZ(CDocument* pThis) {
    if (!pThis || !pThis->IsModified()) return TRUE;
    const wchar_t* path = pThis->GetPathName();
    return path && *path ? pThis->DoFileSave() : TRUE;
}
// DoSave
// Symbol: ?DoSave@CDocument@@UEAAHPEB_WH@Z
extern "C" int MS_ABI impl__DoSave_CDocument__UEAAHPEB_WH_Z(
    CDocument* pThis, const wchar_t* lpszPathName, int bReplace)
{
    (void)bReplace;
    if (!pThis) return FALSE;
    return impl__OnSaveDocument_CDocument__UEAAHPEB_W_Z(pThis, lpszPathName);
}
// DoFileSave
// Symbol: ?DoFileSave@CDocument@@UEAAHXZ
extern "C" int MS_ABI impl__DoFileSave_CDocument__UEAAHXZ(CDocument* pThis) {
    if (!pThis) return FALSE;
    const wchar_t* path = impl__GetPathName_CDocument__UEBAPEB_WXZ(pThis);
    return impl__DoSave_CDocument__UEAAHPEB_WH_Z(pThis, path, TRUE);
}
// Symbol: ?GetRuntimeClass@CDocument@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CDocument__UEBAPEAUCRuntimeClass__XZ(const CDocument*) { return &CDocument::classCDocument; }
// Symbol: ?GetThisClass@CDocument@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CDocument__SAPEAUCRuntimeClass__XZ() { return &CDocument::classCDocument; }
// Symbol: ?GetMessageMap@CDocument@@MEBAPEBUAFX_MSGMAP@@XZ
extern "C" const AFX_MSGMAP* MS_ABI impl__GetMessageMap_CDocument__MEBAPEBUAFX_MSGMAP__XZ(const CDocument*) { return &g_docviewEmptyMap; }
// Symbol: ?GetThisMessageMap@CDocument@@KAPEBUAFX_MSGMAP@@XZ
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CDocument__KAPEBUAFX_MSGMAP__XZ() { return &g_docviewEmptyMap; }
// Symbol: ?GetInterfaceMap@CDocument@@MEBAPEBUAFX_INTERFACEMAP@@XZ
extern "C" const AFX_INTERFACEMAP* MS_ABI impl__GetInterfaceMap_CDocument__MEBAPEBUAFX_INTERFACEMAP__XZ(const CDocument*) { return g_docviewEmptyInterfaceMap; }
// Symbol: ?GetThisInterfaceMap@CDocument@@KAPEBUAFX_INTERFACEMAP@@XZ
extern "C" const AFX_INTERFACEMAP* MS_ABI impl__GetThisInterfaceMap_CDocument__KAPEBUAFX_INTERFACEMAP__XZ() { return g_docviewEmptyInterfaceMap; }
// Symbol: ?GetFile@CDocument@@UEAAPEAVCFile@@PEB_WIPEAVCFileException@@@Z
extern "C" CFile* MS_ABI impl__GetFile_CDocument__UEAAPEAVCFile__PEB_WIPEAVCFileException___Z(CDocument*, const wchar_t* path, unsigned int openFlags, CFileException* exception) {
    CFile* file = new (std::nothrow) CFile();
    if (!file) return nullptr;
    if (!impl__Open_CFile__UEAAHPEB_WIPEAVCFileException___Z(file, path, openFlags, exception)) {
        delete file;
        return nullptr;
    }
    return file;
}
// Symbol: ?ReleaseFile@CDocument@@UEAAXPEAVCFile@@H@Z
extern "C" void MS_ABI impl__ReleaseFile_CDocument__UEAAXPEAVCFile__H_Z(CDocument*, CFile* file, int abort) {
    if (!file) return;
    if (!abort) file->Close();
    delete file;
}
// Symbol: ?ClearPathName@CDocument@@UEAAXXZ
extern "C" void MS_ABI impl__ClearPathName_CDocument__UEAAXXZ(CDocument* pThis) { if (pThis) pThis->m_strPathName.Empty(); }
// Symbol: ?CanCloseFrame@CDocument@@UEAAHPEAVCFrameWnd@@@Z
extern "C" int MS_ABI impl__CanCloseFrame_CDocument__UEAAHPEAVCFrameWnd___Z(CDocument* pThis, CFrameWnd*) { return pThis ? pThis->SaveModified() : TRUE; }
// Symbol: ?PreCloseFrame@CDocument@@UEAAXPEAVCFrameWnd@@@Z
extern "C" void MS_ABI impl__PreCloseFrame_CDocument__UEAAXPEAVCFrameWnd___Z(CDocument* pThis, CFrameWnd*) { if (pThis) pThis->UpdateAllViews(nullptr, 0, nullptr); }
// Symbol: ?DisconnectViews@CDocument@@QEAAXXZ
extern "C" void MS_ABI impl__DisconnectViews_CDocument__QEAAXXZ(CDocument* pThis) {
    if (!pThis) return;
    for (CView* view = pThis->m_pFirstView; view; ) {
        CView* next = view->m_pNextView;
        view->m_pDocument = nullptr;
        view->m_pNextView = nullptr;
        view = next;
    }
    pThis->m_pFirstView = pThis->m_pLastView = nullptr;
}
// Symbol: ?SendInitialUpdate@CDocument@@QEAAXXZ
extern "C" void MS_ABI impl__SendInitialUpdate_CDocument__QEAAXXZ(CDocument* pThis) {
    if (!pThis) return;
    for (CView* view = pThis->m_pFirstView; view; view = view->m_pNextView) view->OnInitialUpdate();
}
// Symbol: ?UpdateFrameCounts@CDocument@@UEAAXXZ
extern "C" void MS_ABI impl__UpdateFrameCounts_CDocument__UEAAXXZ(CDocument* pThis) { if (pThis) pThis->UpdateAllViews(nullptr, 0, nullptr); }
// Symbol: ?OnChangedViewList@CDocument@@UEAAXXZ
extern "C" void MS_ABI impl__OnChangedViewList_CDocument__UEAAXXZ(CDocument* pThis) { impl__UpdateFrameCounts_CDocument__UEAAXXZ(pThis); }
// Symbol: ?OnCmdMsg@CDocument@@UEAAHIHPEAXPEAUAFX_CMDHANDLERINFO@@@Z
extern "C" int MS_ABI impl__OnCmdMsg_CDocument__UEAAHIHPEAXPEAUAFX_CMDHANDLERINFO___Z(CDocument* pThis, unsigned int id, int code, void* extra, AFX_CMDHANDLERINFO* info) { return pThis ? pThis->CCmdTarget::OnCmdMsg(id, code, extra, info) : FALSE; }
// Symbol: ?OnFileSave@CDocument@@IEAAXXZ
extern "C" void MS_ABI impl__OnFileSave_CDocument__IEAAXXZ(CDocument* pThis) { if (pThis) pThis->DoFileSave(); }
// Symbol: ?OnFileSaveAs@CDocument@@IEAAXXZ
extern "C" void MS_ABI impl__OnFileSaveAs_CDocument__IEAAXXZ(CDocument* pThis) { if (pThis) pThis->DoSave(pThis->GetPathName(), TRUE); }
// Symbol: ?OnFileClose@CDocument@@IEAAXXZ
extern "C" void MS_ABI impl__OnFileClose_CDocument__IEAAXXZ(CDocument* pThis) { if (pThis && pThis->SaveModified()) pThis->OnCloseDocument(); }
// Symbol: ?ReportSaveLoadException@CDocument@@UEAAXPEB_WPEAVCException@@HI@Z
extern "C" void MS_ABI impl__ReportSaveLoadException_CDocument__UEAAXPEB_WPEAVCException__HI_Z(CDocument*, const wchar_t*, CException* ex, int, unsigned int) { if (ex) impl__ReportError_CException__UEAAHII_Z(ex, MB_OK | MB_ICONEXCLAMATION, 0); }
// Chunk/read-preview state.
// Symbol: ?ClearChunkList@CDocument@@UEAAXXZ
extern "C" void MS_ABI impl__ClearChunkList_CDocument__UEAAXXZ(CDocument* pThis) { if (pThis) g_documentExtraStates[pThis].chunks.clear(); }
// Symbol: ?BeginReadChunks@CDocument@@UEAAXXZ
extern "C" void MS_ABI impl__BeginReadChunks_CDocument__UEAAXXZ(CDocument* pThis) { if (pThis) g_documentExtraStates[pThis].chunkReadIndex = 0; }
// Symbol: ?SetChunkValue@CDocument@@UEAAHPEAUIFilterChunkValue@ATL@@@Z
extern "C" int MS_ABI impl__SetChunkValue_CDocument__UEAAHPEAUIFilterChunkValue_ATL___Z(CDocument* pThis, void* value) {
    if (!pThis || !value) return FALSE;
    DocChunkState chunk;
    chunk.id = static_cast<DWORD>(g_documentExtraStates[pThis].chunks.size());
    chunk.value = value;
    g_documentExtraStates[pThis].chunks.push_back(chunk);
    return TRUE;
}
// Symbol: ?ReadNextChunkValue@CDocument@@UEAAHPEAPEAUIFilterChunkValue@ATL@@@Z
extern "C" int MS_ABI impl__ReadNextChunkValue_CDocument__UEAAHPEAPEAUIFilterChunkValue_ATL___Z(CDocument* pThis, void** value) {
    if (value) *value = nullptr;
    if (!pThis || !value) return FALSE;
    auto& state = g_documentExtraStates[pThis];
    if (state.chunkReadIndex >= state.chunks.size()) return FALSE;
    *value = state.chunks[state.chunkReadIndex++].value;
    return TRUE;
}
// Symbol: ?FindChunk@CDocument@@UEAAPEAU__POSITION@@AEBU_GUID@@K@Z
extern "C" void* MS_ABI impl__FindChunk_CDocument__UEAAPEAU__POSITION__AEBU_GUID__K_Z(CDocument* pThis, const GUID* guid, unsigned long id) {
    if (!pThis || !guid) return nullptr;
    auto& chunks = g_documentExtraStates[pThis].chunks;
    for (size_t i = 0; i < chunks.size(); ++i) if (chunks[i].id == id && IsEqualGUID(chunks[i].guid, *guid)) return reinterpret_cast<void*>(i + 1);
    return nullptr;
}
// Symbol: ?RemoveChunk@CDocument@@UEAAXAEBU_GUID@@K@Z
extern "C" void MS_ABI impl__RemoveChunk_CDocument__UEAAXAEBU_GUID__K_Z(CDocument* pThis, const GUID* guid, unsigned long id) {
    if (!pThis || !guid) return;
    auto& chunks = g_documentExtraStates[pThis].chunks;
    chunks.erase(std::remove_if(chunks.begin(), chunks.end(), [&](const DocChunkState& c){ return c.id == id && IsEqualGUID(c.guid, *guid); }), chunks.end());
}
// Symbol: ?LoadDocumentFromStream@CDocument@@UEAAJPEAUIStream@@K@Z
extern "C" long MS_ABI impl__LoadDocumentFromStream_CDocument__UEAAJPEAUIStream__K_Z(CDocument* pThis, void* stream, unsigned long) {
    if (!pThis || !stream) return E_POINTER;
    IStream* pStream = static_cast<IStream*>(stream);
    CMemFile memFile;
    BYTE buffer[4096];
    for (;;) {
        ULONG read = 0;
        HRESULT hr = pStream->Read(buffer, sizeof(buffer), &read);
        if (FAILED(hr)) return hr;
        if (read == 0) break;
        memFile.Write(buffer, read);
    }
    memFile.Seek(0, CFile::begin);
    CArchive archive(&memFile, CArchive::load);
    pThis->DeleteContents();
    pThis->Serialize(archive);
    archive.Close();
    pThis->SetModifiedFlag(FALSE);
    return S_OK;
}
// Symbol: ?OnLoadDocumentFromStream@CDocument@@UEAAJPEAUIStream@@K@Z
extern "C" long MS_ABI impl__OnLoadDocumentFromStream_CDocument__UEAAJPEAUIStream__K_Z(CDocument* pThis, void* stream, unsigned long mode) { return impl__LoadDocumentFromStream_CDocument__UEAAJPEAUIStream__K_Z(pThis, stream, mode); }
// Symbol: ?GetThumbnail@CDocument@@UEAAHIPEAPEAUHBITMAP__@@PEAW4WTS_ALPHATYPE@@@Z
extern "C" void MS_ABI impl__OnDrawThumbnail_CDocument__UEAAXAEAVCDC__PEAUtagRECT___Z(CDocument*, CDC* dc, RECT* rect);
// Symbol: ?OnDrawThumbnail@CDocument@@UEAAXAEAVCDC@@PEAUtagRECT@@@Z
extern "C" void MS_ABI impl__OnDrawThumbnail_CDocument__UEAAXAEAVCDC__PEAUtagRECT___Z(CDocument*, CDC* dc, RECT* rect) { if (dc && rect && dc->m_hDC) ::DrawTextW(dc->m_hDC, L"OpenMFC", -1, rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE); }
// Symbol: ?OnPreviewHandlerQueryFocus@CDocument@@UEAAJPEAPEAUHWND__@@@Z
extern "C" long MS_ABI impl__OnPreviewHandlerQueryFocus_CDocument__UEAAJPEAPEAUHWND_____Z(CDocument* pThis, HWND* hwnd) { if (hwnd) *hwnd = pThis && pThis->m_pFirstView ? pThis->m_pFirstView->GetSafeHwnd() : nullptr; return S_OK; }
// Symbol: ?OnPreviewHandlerTranslateAccelerator@CDocument@@UEAAJPEAUtagMSG@@@Z
extern "C" long MS_ABI impl__OnPreviewHandlerTranslateAccelerator_CDocument__UEAAJPEAUtagMSG___Z(CDocument* pThis, MSG* pMsg) {
    if (!pThis || !pMsg) {
        return S_FALSE;
    }

    void* pos = pThis->GetFirstViewPosition();
    while (pos) {
        CView* pView = pThis->GetNextView(pos);
        if (pView && pView->m_hWnd && ::IsWindow(pView->m_hWnd) && ::IsDialogMessageW(pView->m_hWnd, pMsg)) {
            return S_OK;
        }
    }

    return S_FALSE;
}
// Symbol: ?IsSearchAndOrganizeHandler@CDocument@@QEBAHXZ
extern "C" int MS_ABI impl__IsSearchAndOrganizeHandler_CDocument__QEBAHXZ(const CDocument* pThis) { auto it = g_documentExtraStates.find(pThis); return it != g_documentExtraStates.end() && it->second.searchHandler; }
// Symbol: ?GetDefaultMenu@CDocument@@UEAAPEAUHMENU__@@XZ
extern "C" HMENU MS_ABI impl__GetDefaultMenu_CDocument__UEAAPEAUHMENU____XZ(CDocument* pThis) { return pThis ? g_documentExtraStates[pThis].defaultMenu : nullptr; }
// Symbol: ?GetDefaultAccelerator@CDocument@@UEAAPEAUHACCEL__@@XZ
extern "C" HACCEL MS_ABI impl__GetDefaultAccelerator_CDocument__UEAAPEAUHACCEL____XZ(CDocument* pThis) { return pThis ? g_documentExtraStates[pThis].defaultAccel : nullptr; }
// Symbol: ?OnCreatePreviewFrame@CDocument@@UEAAHXZ
extern "C" int MS_ABI impl__OnCreatePreviewFrame_CDocument__UEAAHXZ(CDocument* pThis) {
    if (!pThis) {
        return FALSE;
    }
    return pThis->m_pFirstView != nullptr ? TRUE : FALSE;
}
// Symbol: ?OnDocumentEvent@CDocument@@UEAAXW4DocumentEvent@1@@Z
extern "C" void MS_ABI impl__OnDocumentEvent_CDocument__UEAAXW4DocumentEvent_1__Z(CDocument* pThis, int eventId) { if (pThis) pThis->UpdateAllViews(nullptr, static_cast<LPARAM>(eventId), nullptr); }
// Symbol: ?OnFinalRelease@CDocument@@UEAAXXZ
extern "C" void MS_ABI impl__OnFinalRelease_CDocument__UEAAXXZ(CDocument* pThis) { if (pThis && pThis->m_bAutoDelete) delete pThis; }
// Symbol: ?OnIdle@CDocument@@UEAAXXZ
extern "C" void MS_ABI impl__OnIdle_CDocument__UEAAXXZ(CDocument* pThis) { if (pThis) pThis->UpdateAllViews(nullptr, 0, nullptr); }
// Symbol: ?OnRichPreviewUnload@CDocument@@UEAAXXZ
extern "C" void MS_ABI impl__OnRichPreviewUnload_CDocument__UEAAXXZ(CDocument* pThis) { impl__ClearChunkList_CDocument__UEAAXXZ(pThis); }
// Symbol: ?OnUnloadHandler@CDocument@@UEAAXXZ
extern "C" void MS_ABI impl__OnUnloadHandler_CDocument__UEAAXXZ(CDocument* pThis) { impl__OnRichPreviewUnload_CDocument__UEAAXXZ(pThis); }
// Symbol: ?OnFileSendMail@CDocument@@IEAAXXZ
extern "C" void MS_ABI impl__OnFileSendMail_CDocument__IEAAXXZ(CDocument* pThis) { if (pThis && pThis->GetPathName()[0]) ::ShellExecuteW(nullptr, L"open", pThis->GetPathName(), nullptr, nullptr, SW_SHOWNORMAL); }
// Symbol: ?OnUpdateFileSendMail@CDocument@@IEAAXPEAVCCmdUI@@@Z
extern "C" void MS_ABI impl__OnUpdateFileSendMail_CDocument__IEAAXPEAVCCmdUI___Z(CDocument* pThis, CCmdUI* ui) { if (ui) impl__Enable_CCmdUI__UEAAXH_Z(ui, pThis && pThis->GetPathName()[0]); }
CDocument::CDocument() { impl___0CDocument__QEAA_XZ(this); }
CDocument::~CDocument() { impl___1CDocument__UEAA_XZ(this); }
int CDocument::OnNewDocument() { return impl__OnNewDocument_CDocument__UEAAHXZ(this); }
int CDocument::OnOpenDocument(const wchar_t* lpszPathName) { return impl__OnOpenDocument_CDocument__UEAAHPEB_W_Z(this, lpszPathName); }
int CDocument::OnSaveDocument(const wchar_t* lpszPathName) { return impl__OnSaveDocument_CDocument__UEAAHPEB_W_Z(this, lpszPathName); }
void CDocument::OnCloseDocument() { impl__OnCloseDocument_CDocument__UEAAXXZ(this); }
void CDocument::DeleteContents() { impl__DeleteContents_CDocument__UEAAXXZ(this); }
void CDocument::SetModifiedFlag(int bModified) { impl__SetModifiedFlag_CDocument__UEAAXH_Z(this, bModified); }
int CDocument::IsModified() const { return impl__IsModified_CDocument__UEBAHXZ(this); }
void CDocument::SetTitle(const wchar_t* lpszTitle) { impl__SetTitle_CDocument__UEAAXPEB_W_Z(this, lpszTitle); }
const wchar_t* CDocument::GetTitle() const { return impl__GetTitle_CDocument__UEBAPEB_WXZ(this); }
void CDocument::SetPathName(const wchar_t* lpszPathName, int bAddToMRU) { impl__SetPathName_CDocument__UEAAXPEB_WH_Z(this, lpszPathName, bAddToMRU); }
const wchar_t* CDocument::GetPathName() const { return impl__GetPathName_CDocument__UEBAPEB_WXZ(this); }
void CDocument::AddView(CView* pView) { impl__AddView_CDocument__UEAAXPEAVCView___Z(this, pView); }
void CDocument::RemoveView(CView* pView) { impl__RemoveView_CDocument__UEAAXPEAVCView___Z(this, pView); }
void* CDocument::GetFirstViewPosition() const { return impl__GetFirstViewPosition_CDocument__UEBAPEAXXZ(this); }
CView* CDocument::GetNextView(void*& pos) const { return impl__GetNextView_CDocument__UEBAPEAVCView__AEAPEAX_Z(this, pos); }
void CDocument::UpdateAllViews(CView* pSender, unsigned long lHint, CObject* pHint) { impl__UpdateAllViews_CDocument__UEAAXPEAVCView__KPEAVCObject___Z(this, pSender, lHint, pHint); }
int CDocument::SaveModified() { return impl__SaveModified_CDocument__UEAAHXZ(this); }
int CDocument::DoSave(const wchar_t* lpszPathName, int bReplace) { return impl__DoSave_CDocument__UEAAHPEB_WH_Z(this, lpszPathName, bReplace); }
int CDocument::DoFileSave() { return impl__DoFileSave_CDocument__UEAAHXZ(this); }
void CDocument::Serialize(CArchive& ar) { impl__Serialize_CDocument__UEAAXAEAVCArchive___Z(this, &ar); }

// Symbol: ?ContextSensitiveHelp@XOleWindow@CDocument@@UEAAJH@Z
extern "C" long MS_ABI impl__ContextSensitiveHelp_XOleWindow_CDocument__UEAAJH_Z(int p0) {
    return 0;
}

// Symbol: ?DoPreview@XPreviewHandler@CDocument@@UEAAJXZ
extern "C" long MS_ABI impl__DoPreview_XPreviewHandler_CDocument__UEAAJXZ() {
    return 0;
}

// Symbol: ?GetSite@XObjectWithSite@CDocument@@UEAAJAEBU_GUID@@PEAPEAX@Z
extern "C" long MS_ABI impl__GetSite_XObjectWithSite_CDocument__UEAAJAEBU_GUID__PEAPEAX_Z(const void* /*struct*/* p0, void** p1) {
    return 0;
}

// Symbol: ?GetWindow@XOleWindow@CDocument@@UEAAJPEAPEAUHWND__@@@Z
extern "C" long MS_ABI impl__GetWindow_XOleWindow_CDocument__UEAAJPEAPEAUHWND_____Z(void* /*struct*/** p0) {
    return 0;
}

// Symbol: ?Initialize@XInitializeWithStream@CDocument@@UEAAJPEAUIStream@@K@Z
extern "C" long MS_ABI impl__Initialize_XInitializeWithStream_CDocument__UEAAJPEAUIStream__K_Z(void* /*struct*/* p0, unsigned long p1) {
    return 0;
}

// Symbol: ?QueryFocus@XPreviewHandler@CDocument@@UEAAJPEAPEAUHWND__@@@Z
extern "C" long MS_ABI impl__QueryFocus_XPreviewHandler_CDocument__UEAAJPEAPEAUHWND_____Z(void* /*struct*/** p0) {
    return 0;
}

// Symbol: ?SetBackgroundColor@XPreviewHandlerVisuals@CDocument@@UEAAJK@Z
extern "C" long MS_ABI impl__SetBackgroundColor_XPreviewHandlerVisuals_CDocument__UEAAJK_Z(unsigned long p0) {
    return 0;
}

// Symbol: ?SetFocus@XPreviewHandler@CDocument@@UEAAJXZ
extern "C" long MS_ABI impl__SetFocus_XPreviewHandler_CDocument__UEAAJXZ() {
    return 0;
}

// Symbol: ?SetFont@XPreviewHandlerVisuals@CDocument@@UEAAJPEBUtagLOGFONTW@@@Z
extern "C" long MS_ABI impl__SetFont_XPreviewHandlerVisuals_CDocument__UEAAJPEBUtagLOGFONTW___Z(const void* /*struct*/* p0) {
    return 0;
}

// Symbol: ?SetRect@XPreviewHandler@CDocument@@UEAAJPEBUtagRECT@@@Z
extern "C" long MS_ABI impl__SetRect_XPreviewHandler_CDocument__UEAAJPEBUtagRECT___Z(const void* /*struct*/* p0) {
    return 0;
}

// Symbol: ?SetSite@XObjectWithSite@CDocument@@UEAAJPEAUIUnknown@@@Z
extern "C" long MS_ABI impl__SetSite_XObjectWithSite_CDocument__UEAAJPEAUIUnknown___Z(void* /*struct*/* p0) {
    return 0;
}

// Symbol: ?SetTextColor@XPreviewHandlerVisuals@CDocument@@UEAAJK@Z
extern "C" long MS_ABI impl__SetTextColor_XPreviewHandlerVisuals_CDocument__UEAAJK_Z(unsigned long p0) {
    return 0;
}

// Symbol: ?SetWindow@XPreviewHandler@CDocument@@UEAAJPEAUHWND__@@PEBUtagRECT@@@Z
extern "C" long MS_ABI impl__SetWindow_XPreviewHandler_CDocument__UEAAJPEAUHWND____PEBUtagRECT___Z(void* /*struct*/* p0, const void* /*struct*/* p1) {
    return 0;
}

// Symbol: ?TranslateAcceleratorW@XPreviewHandler@CDocument@@UEAAJPEAUtagMSG@@@Z
extern "C" long MS_ABI impl__TranslateAcceleratorW_XPreviewHandler_CDocument__UEAAJPEAUtagMSG___Z(void* /*struct*/* p0) {
    return 0;
}

// Symbol: ?Unload@XPreviewHandler@CDocument@@UEAAJXZ
extern "C" long MS_ABI impl__Unload_XPreviewHandler_CDocument__UEAAJXZ() {
    return 0;
}
