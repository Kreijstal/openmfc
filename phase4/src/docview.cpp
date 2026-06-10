// Document-View Architecture Implementation
//
// Implements CDocument, CView, CDocTemplate and related classes.
// This enables Document-View applications (SDI/MDI).

// Define OPENMFC_APPCORE_IMPL to prevent inline implementations conflicting
#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxwin.h"
#include <windows.h>
#include <algorithm>
#include <cstring>
#include <deque>
#include <new>
#include <string>
#include <unordered_map>
#include <vector>

namespace {

constexpr int kDocStringCount = 7;

enum DocStringIndexCompat {
    kWindowTitle = 0,
    kDocName = 1,
    kFileNewName = 2,
    kFilterName = 3,
    kFilterExt = 4,
    kRegFileTypeId = 5,
    kRegFileTypeName = 6,
};

static bool LoadTemplateDocString(UINT resourceId, int index, CString& out) {
    out.Empty();
    if (resourceId == 0 || index < 0 || index >= kDocStringCount) {
        return false;
    }

    wchar_t buffer[1024] = {};
    int len = ::LoadStringW(AfxGetInstanceHandle(), resourceId, buffer, sizeof(buffer) / sizeof(buffer[0]));
    if (len <= 0) {
        return false;
    }

    const wchar_t* start = buffer;
    for (int i = 0; i < index; ++i) {
        const wchar_t* sep = wcschr(start, L'\n');
        if (!sep) {
            return false;
        }
        start = sep + 1;
    }

    const wchar_t* end = wcschr(start, L'\n');
    if (!end) {
        end = start + wcslen(start);
    }

    if (end < start) {
        return false;
    }

    int outLen = static_cast<int>(end - start);
    out = CString();
    wchar_t* outBuf = out.GetBuffer(outLen);
    if (outLen > 0) {
        memcpy(outBuf, start, static_cast<size_t>(outLen) * sizeof(wchar_t));
    }
    outBuf[outLen] = L'\0';
    out.ReleaseBuffer(outLen);
    return true;
}

static std::wstring NormalizePathForCompare(const wchar_t* path) {
    if (!path || !path[0]) {
        return std::wstring();
    }

    wchar_t fullPath[MAX_PATH] = {};
    DWORD len = ::GetFullPathNameW(path, MAX_PATH, fullPath, nullptr);
    const wchar_t* source = (len > 0 && len < MAX_PATH) ? fullPath : path;

    std::wstring normalized(source);
    for (wchar_t& ch : normalized) {
        if (ch == L'/') {
            ch = L'\\';
        }
    }
    return normalized;
}

static bool PathsEqualInsensitive(const wchar_t* lhs, const wchar_t* rhs) {
    std::wstring left = NormalizePathForCompare(lhs);
    std::wstring right = NormalizePathForCompare(rhs);
    if (left.empty() || right.empty()) {
        return false;
    }

    return ::CompareStringOrdinal(
               left.c_str(), -1,
               right.c_str(), -1,
               TRUE) == CSTR_EQUAL;
}

static bool PathHasExtensionInsensitive(const wchar_t* path, const wchar_t* expectedExt) {
    if (!path || !expectedExt || !expectedExt[0]) {
        return false;
    }

    const wchar_t* dot = wcsrchr(path, L'.');
    if (!dot) {
        return false;
    }

    if (expectedExt[0] == L'*') {
        ++expectedExt;
        if (expectedExt[0] == L'.') {
            // already normalized to dot-prefixed extension
        }
    }
    if (!expectedExt[0]) {
        return false;
    }

    return ::CompareStringOrdinal(dot, -1, expectedExt, -1, TRUE) == CSTR_EQUAL;
}

struct DocChunkState {
    GUID guid = {};
    DWORD id = 0;
    void* value = nullptr;
};

struct DocumentExtraState {
    std::vector<DocChunkState> chunks;
    size_t chunkReadIndex = 0;
    bool searchHandler = false;
    HMENU defaultMenu = nullptr;
    HACCEL defaultAccel = nullptr;
};

struct TemplateExtraState {
    UINT containerId = 0;
    UINT serverId = 0;
    UINT serverEmbeddingId = 0;
    UINT previewId = 0;
    CRuntimeClass* serverDocClass = nullptr;
    CRuntimeClass* serverFrameClass = nullptr;
    CRuntimeClass* previewViewClass = nullptr;
    CRuntimeClass* previewFrameClass = nullptr;
};

struct EditViewExtraState {
    std::wstring buffer;
    std::wstring findText;
    std::wstring replaceText;
    CFont* printerFont = nullptr;
    const wchar_t* locked = nullptr;
    UINT pageCount = 0;
};

std::unordered_map<const CDocument*, DocumentExtraState> g_documentExtraStates;
std::unordered_map<const CDocTemplate*, TemplateExtraState> g_templateExtraStates;
std::unordered_map<const CEditView*, EditViewExtraState> g_editViewExtraStates;

std::wstring EditViewText(const CEditView* view) {
    if (!view) return std::wstring();
    if (view->m_hWnd) {
        int len = ::GetWindowTextLengthW(view->m_hWnd);
        std::wstring text(static_cast<size_t>(std::max(0, len)), L'\0');
        if (len > 0) ::GetWindowTextW(view->m_hWnd, text.data(), len + 1);
        return text;
    }
    auto it = g_editViewExtraStates.find(view);
    return it == g_editViewExtraStates.end() ? std::wstring() : it->second.buffer;
}

void SetEditViewText(CEditView* view, const std::wstring& text) {
    if (!view) return;
    g_editViewExtraStates[view].buffer = text;
    if (view->m_hWnd) ::SetWindowTextW(view->m_hWnd, text.c_str());
}

} // namespace

// MS ABI calling convention
#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

// =============================================================================
// Runtime Class Registration
// =============================================================================

IMPLEMENT_DYNCREATE(CDocument, CCmdTarget)
IMPLEMENT_DYNAMIC(CView, CWnd)
IMPLEMENT_DYNCREATE(CScrollView, CView)
IMPLEMENT_DYNCREATE(CFormView, CScrollView)
IMPLEMENT_DYNCREATE(CEditView, CView)
IMPLEMENT_DYNCREATE(CListView, CView)
IMPLEMENT_DYNCREATE(CTreeView, CView)
IMPLEMENT_DYNAMIC(CDocTemplate, CCmdTarget)
IMPLEMENT_DYNCREATE(CSingleDocTemplate, CDocTemplate)
IMPLEMENT_DYNCREATE(CMultiDocTemplate, CDocTemplate)

// =============================================================================
// C++ Constructors/Destructors (required for IMPLEMENT_DYNCREATE)
// These call the impl_ functions which contain the actual logic
// =============================================================================

// Forward declarations
extern "C" void MS_ABI impl___0CDocument__QEAA_XZ(CDocument* pThis);
extern "C" void MS_ABI impl___1CDocument__UEAA_XZ(CDocument* pThis);
extern "C" void MS_ABI impl___0CView__QEAA_XZ(CView* pThis);
extern "C" void MS_ABI impl___1CView__UEAA_XZ(CView* pThis);
extern "C" void MS_ABI impl___0CScrollView__QEAA_XZ(CScrollView* pThis);
extern "C" void MS_ABI impl___1CScrollView__UEAA_XZ(CScrollView* pThis);
extern "C" void MS_ABI impl___0CFormView__QEAA_XZ(CFormView* pThis);
extern "C" void MS_ABI impl___0CFormView__IEAA_PEB_W_Z(CFormView* pThis, const wchar_t* lpszTemplateName);
extern "C" void MS_ABI impl___0CFormView__IEAA_I_Z(CFormView* pThis, unsigned int nIDTemplate);
extern "C" void MS_ABI impl___1CFormView__UEAA_XZ(CFormView* pThis);
extern "C" void MS_ABI impl___0CEditView__QEAA_XZ(CEditView* pThis);
extern "C" void MS_ABI impl___1CEditView__UEAA_XZ(CEditView* pThis);
extern "C" void MS_ABI impl___0CListView__QEAA_XZ(CListView* pThis);
extern "C" void MS_ABI impl___1CListView__UEAA_XZ(CListView* pThis);
extern "C" void MS_ABI impl___0CTreeView__QEAA_XZ(CTreeView* pThis);
extern "C" void MS_ABI impl___1CTreeView__UEAA_XZ(CTreeView* pThis);
extern "C" void MS_ABI impl___0CDocTemplate__QEAA_IPEAUCRuntimeClass__00_Z(
    CDocTemplate* pThis, unsigned int nIDResource,
    CRuntimeClass* pDocClass, CRuntimeClass* pFrameClass, CRuntimeClass* pViewClass);
extern "C" void MS_ABI impl___0CDocTemplate__IEAA_IPEAUCRuntimeClass__00_Z(
    CDocTemplate* pThis, unsigned int nIDResource,
    CRuntimeClass* pDocClass, CRuntimeClass* pFrameClass, CRuntimeClass* pViewClass);
extern "C" void MS_ABI impl___1CDocTemplate__UEAA_XZ(CDocTemplate* pThis);
extern "C" void MS_ABI impl___0CSingleDocTemplate__QEAA_XZ(CSingleDocTemplate* pThis);
extern "C" void MS_ABI impl___0CSingleDocTemplate__QEAA_IPEAUCRuntimeClass__00_Z(
    CSingleDocTemplate* pThis, unsigned int nIDResource,
    CRuntimeClass* pDocClass, CRuntimeClass* pFrameClass, CRuntimeClass* pViewClass);
extern "C" void MS_ABI impl___1CSingleDocTemplate__UEAA_XZ(CSingleDocTemplate* pThis);
extern "C" void MS_ABI impl___0CMultiDocTemplate__QEAA_XZ(CMultiDocTemplate* pThis);
extern "C" void MS_ABI impl___0CMultiDocTemplate__QEAA_IPEAUCRuntimeClass__00_Z(
    CMultiDocTemplate* pThis, unsigned int nIDResource,
    CRuntimeClass* pDocClass, CRuntimeClass* pFrameClass, CRuntimeClass* pViewClass);
extern "C" void MS_ABI impl___1CMultiDocTemplate__UEAA_XZ(CMultiDocTemplate* pThis);

// CDocument constructors
CDocument::CDocument() { impl___0CDocument__QEAA_XZ(this); }
CDocument::~CDocument() { impl___1CDocument__UEAA_XZ(this); }

// CView constructors
CView::CView() { impl___0CView__QEAA_XZ(this); }
CView::~CView() { impl___1CView__UEAA_XZ(this); }

// CScrollView constructors
CScrollView::CScrollView() { impl___0CScrollView__QEAA_XZ(this); }
CScrollView::~CScrollView() { impl___1CScrollView__UEAA_XZ(this); }

// CFormView constructors
// CFormView constructor is inline in the header; extern "C" thunk below if needed
CFormView::~CFormView() { impl___1CFormView__UEAA_XZ(this); }

// CEditView constructors
CEditView::CEditView() { impl___0CEditView__QEAA_XZ(this); }
CEditView::~CEditView() { impl___1CEditView__UEAA_XZ(this); }

// CListView constructors
CListView::CListView() { impl___0CListView__QEAA_XZ(this); }
CListView::~CListView() { impl___1CListView__UEAA_XZ(this); }

// CTreeView constructors
CTreeView::CTreeView() { impl___0CTreeView__QEAA_XZ(this); }
CTreeView::~CTreeView() { impl___1CTreeView__UEAA_XZ(this); }

// CDocTemplate constructors
CDocTemplate::CDocTemplate(unsigned int nIDResource, CRuntimeClass* pDocClass,
                           CRuntimeClass* pFrameClass, CRuntimeClass* pViewClass) {
    impl___0CDocTemplate__QEAA_IPEAUCRuntimeClass__00_Z(
        this, nIDResource, pDocClass, pFrameClass, pViewClass);
}
// Protected default constructor
CDocTemplate::CDocTemplate() {
    m_nIDResource = 0;
    m_pDocClass = nullptr;
    m_pFrameClass = nullptr;
    m_pViewClass = nullptr;
    m_pFirstDoc = nullptr;
    m_pLastDoc = nullptr;
}
CDocTemplate::~CDocTemplate() { impl___1CDocTemplate__UEAA_XZ(this); }

// CSingleDocTemplate constructors
CSingleDocTemplate::CSingleDocTemplate() { impl___0CSingleDocTemplate__QEAA_XZ(this); }
CSingleDocTemplate::CSingleDocTemplate(unsigned int nIDResource, CRuntimeClass* pDocClass,
                                       CRuntimeClass* pFrameClass, CRuntimeClass* pViewClass) {
    impl___0CSingleDocTemplate__QEAA_IPEAUCRuntimeClass__00_Z(
        this, nIDResource, pDocClass, pFrameClass, pViewClass);
}
CSingleDocTemplate::~CSingleDocTemplate() { impl___1CSingleDocTemplate__UEAA_XZ(this); }

// CMultiDocTemplate constructors
CMultiDocTemplate::CMultiDocTemplate() { impl___0CMultiDocTemplate__QEAA_XZ(this); }
CMultiDocTemplate::CMultiDocTemplate(unsigned int nIDResource, CRuntimeClass* pDocClass,
                                     CRuntimeClass* pFrameClass, CRuntimeClass* pViewClass) {
    impl___0CMultiDocTemplate__QEAA_IPEAUCRuntimeClass__00_Z(
        this, nIDResource, pDocClass, pFrameClass, pViewClass);
}
CMultiDocTemplate::~CMultiDocTemplate() { impl___1CMultiDocTemplate__UEAA_XZ(this); }

// =============================================================================
// Forward declarations of impl_ functions
// =============================================================================
extern "C" int MS_ABI impl__OnNewDocument_CDocument__UEAAHXZ(CDocument* pThis);
extern "C" int MS_ABI impl__OnOpenDocument_CDocument__UEAAHPEB_W_Z(CDocument* pThis, const wchar_t* lpszPathName);
extern "C" int MS_ABI impl__OnSaveDocument_CDocument__UEAAHPEB_W_Z(CDocument* pThis, const wchar_t* lpszPathName);
extern "C" void MS_ABI impl__OnCloseDocument_CDocument__UEAAXXZ(CDocument* pThis);
extern "C" void MS_ABI impl__DeleteContents_CDocument__UEAAXXZ(CDocument* pThis);
extern "C" int MS_ABI impl__Open_CFile__UEAAHPEB_WIPEAVCFileException___Z(CFile* pThis, const wchar_t* lpszFileName, unsigned int nOpenFlags, CFileException* pException);
extern "C" int MS_ABI impl__ReportError_CException__UEAAHII_Z(CException* pThis, unsigned int type, unsigned int);
extern "C" void MS_ABI impl__Enable_CCmdUI__UEAAXH_Z(CCmdUI* pThis, int enable);
extern "C" void MS_ABI impl__SetModifiedFlag_CDocument__UEAAXH_Z(CDocument* pThis, int bModified);
extern "C" int MS_ABI impl__IsModified_CDocument__UEBAHXZ(const CDocument* pThis);
extern "C" void MS_ABI impl__SetTitle_CDocument__UEAAXPEB_W_Z(CDocument* pThis, const wchar_t* lpszTitle);
extern "C" const wchar_t* MS_ABI impl__GetTitle_CDocument__UEBAPEB_WXZ(const CDocument* pThis);
extern "C" void MS_ABI impl__SetPathName_CDocument__UEAAXPEB_WH_Z(CDocument* pThis, const wchar_t* lpszPathName, int bAddToMRU);
extern "C" const wchar_t* MS_ABI impl__GetPathName_CDocument__UEBAPEB_WXZ(const CDocument* pThis);
extern "C" void MS_ABI impl__AddView_CDocument__UEAAXPEAVCView___Z(CDocument* pThis, CView* pView);
extern "C" void MS_ABI impl__RemoveView_CDocument__UEAAXPEAVCView___Z(CDocument* pThis, CView* pView);
extern "C" void* MS_ABI impl__GetFirstViewPosition_CDocument__UEBAPEAXXZ(const CDocument* pThis);
extern "C" CView* MS_ABI impl__GetNextView_CDocument__UEBAPEAVCView__AEAPEAX_Z(const CDocument* pThis, void*& pos);
extern "C" void MS_ABI impl__UpdateAllViews_CDocument__UEAAXPEAVCView__KPEAVCObject___Z(CDocument* pThis, CView* pSender, unsigned long lHint, CObject* pHint);
extern "C" int MS_ABI impl__SaveModified_CDocument__UEAAHXZ(CDocument* pThis);
extern "C" int MS_ABI impl__DoSave_CDocument__UEAAHPEB_WH_Z(CDocument* pThis, const wchar_t* lpszPathName, int bReplace);
extern "C" int MS_ABI impl__DoFileSave_CDocument__UEAAHXZ(CDocument* pThis);
extern "C" void MS_ABI impl__Serialize_CDocument__UEAAXAEAVCArchive___Z(CDocument* pThis, CArchive* ar);
extern "C" CDocument* MS_ABI impl__GetDocument_CView__UEBAPEAVCDocument__XZ(const CView* pThis);
extern "C" void MS_ABI impl__OnUpdate_CView__UEAAXPEAV1_KPEAVCObject___Z(CView* pThis, CView* pSender, unsigned long lHint, CObject* pHint);
extern "C" void MS_ABI impl__OnInitialUpdate_CView__UEAAXXZ(CView* pThis);
extern "C" int MS_ABI impl__OnPreparePrinting_CView__UEAAHPEAX_Z(CView* pThis, void* pInfo);
extern "C" void MS_ABI impl__OnBeginPrinting_CView__UEAAXPEAX0_Z(CView* pThis, void* pDC, void* pInfo);
extern "C" void MS_ABI impl__OnEndPrinting_CView__UEAAXPEAX0_Z(CView* pThis, void* pDC, void* pInfo);
extern "C" void MS_ABI impl__OnActivateView_CView__UEAAXHPEAV1_0_Z(CView* pThis, int bActivate, CView* pActivateView, CView* pDeactiveView);
extern "C" CFrameWnd* MS_ABI impl__GetParentFrame_CView__UEBAPEAVCFrameWnd__XZ(const CView* pThis);
extern "C" void MS_ABI impl__SetScrollSizes_CScrollView__UEAAXHAEBUtagSIZE__00_Z(CScrollView* pThis, int nMapMode, const SIZE* sizeTotal, const SIZE* sizePage, const SIZE* sizeLine);
extern "C" void MS_ABI impl__OnInitialUpdate_CScrollView__UEAAXXZ(CScrollView* pThis);
extern "C" void MS_ABI impl__OnDraw_CScrollView__UEAAXPEAX_Z(CScrollView* pThis, void* pDC);
extern "C" void MS_ABI impl__OnDraw_CFormView__UEAAXPEAX_Z(CFormView* pThis, void* pDC);
extern "C" void MS_ABI impl__OnInitialUpdate_CFormView__UEAAXXZ(CFormView* pThis);
extern "C" void MS_ABI impl__DoDataExchange_CFormView__UEAAXPEAX_Z(CFormView* pThis, void* pDX);
extern "C" CEdit* MS_ABI impl__GetEditCtrl_CEditView__QEBAPEAVCEdit__XZ(const CEditView* pThis);
extern "C" CObject* MS_ABI impl__CreateObject_CEditView__SAPEAVCObject__XZ();
extern "C" void MS_ABI impl__OnDraw_CEditView__UEAAXPEAX_Z(CEditView* pThis, void* pDC);
extern "C" CListCtrl* MS_ABI impl__GetListCtrl_CListView__QEBAPEAVCListCtrl__XZ(const CListView* pThis);
extern "C" CObject* MS_ABI impl__CreateObject_CListView__SAPEAVCObject__XZ();
extern "C" void MS_ABI impl__OnDraw_CListView__UEAAXPEAX_Z(CListView* pThis, void* pDC);
extern "C" void MS_ABI impl__OnInitialUpdate_CListView__UEAAXXZ(CListView* pThis);
extern "C" CTreeCtrl* MS_ABI impl__GetTreeCtrl_CTreeView__QEBAPEAVCTreeCtrl__XZ(const CTreeView* pThis);
extern "C" CObject* MS_ABI impl__CreateObject_CTreeView__SAPEAVCObject__XZ();
extern "C" void MS_ABI impl__OnDraw_CTreeView__UEAAXPEAX_Z(CTreeView* pThis, void* pDC);
extern "C" void MS_ABI impl__OnInitialUpdate_CTreeView__UEAAXXZ(CTreeView* pThis);
extern "C" CDocument* MS_ABI impl__CreateNewDocument_CDocTemplate__UEAAPEAVCDocument__XZ(CDocTemplate* pThis);
extern "C" CFrameWnd* MS_ABI impl__CreateNewFrame_CDocTemplate__UEAAPEAVCFrameWnd__PEAVCDocument__PEAV2__Z(CDocTemplate* pThis, CDocument* pDoc, CFrameWnd* pOther);
extern "C" CDocument* MS_ABI impl__OpenDocumentFile_CDocTemplate__UEAAPEAVCDocument__PEB_WH_Z(CDocTemplate* pThis, const wchar_t* lpszPathName, int bMakeVisible);
extern "C" void MS_ABI impl__AddDocument_CDocTemplate__UEAAXPEAVCDocument___Z(CDocTemplate* pThis, CDocument* pDoc);
extern "C" void MS_ABI impl__RemoveDocument_CDocTemplate__UEAAXPEAVCDocument___Z(CDocTemplate* pThis, CDocument* pDoc);
extern "C" void* MS_ABI impl__GetFirstDocPosition_CDocTemplate__UEBAPEAXXZ(const CDocTemplate* pThis);
extern "C" CDocument* MS_ABI impl__GetNextDoc_CDocTemplate__UEBAPEAVCDocument__AEAPEAX_Z(const CDocTemplate* pThis, void*& pos);
extern "C" void MS_ABI impl__InitialUpdateFrame_CDocTemplate__UEAAXPEAVCFrameWnd__PEAVCDocument__H_Z(CDocTemplate* pThis, CFrameWnd* pFrame, CDocument* pDoc, int bMakeVisible);
extern "C" void MS_ABI impl__SetDefaultTitle_CDocTemplate__UEAAXPEAVCDocument___Z(CDocTemplate* pThis, CDocument* pDoc);
extern "C" int MS_ABI impl__GetDocString_CDocTemplate__UEBAHAEAVCString__H_Z(const CDocTemplate* pThis, CString* rString, int nID);
extern "C" int MS_ABI impl__MatchDocType_CDocTemplate__UEAAHPEB_WAEAPEAVCDocument___Z(CDocTemplate* pThis, const wchar_t* lpszPathName, CDocument*& rpDocMatch);
extern "C" CDocument* MS_ABI impl__OpenDocumentFile_CSingleDocTemplate__UEAAPEAVCDocument__PEB_WH_Z(CSingleDocTemplate* pThis, const wchar_t* lpszPathName, int bMakeVisible);
extern "C" void MS_ABI impl__AddDocument_CSingleDocTemplate__UEAAXPEAVCDocument___Z(CSingleDocTemplate* pThis, CDocument* pDoc);
extern "C" void MS_ABI impl__RemoveDocument_CSingleDocTemplate__UEAAXPEAVCDocument___Z(CSingleDocTemplate* pThis, CDocument* pDoc);
extern "C" void* MS_ABI impl__GetFirstDocPosition_CSingleDocTemplate__UEBAPEAU__POSITION__XZ(const CSingleDocTemplate* pThis);
extern "C" CDocument* MS_ABI impl__GetNextDoc_CSingleDocTemplate__UEBAPEAVCDocument__AEAPEAU__POSITION___Z(const CSingleDocTemplate* pThis, void*& pos);
extern "C" void MS_ABI impl__SetDefaultTitle_CSingleDocTemplate__UEAAXPEAVCDocument___Z(CSingleDocTemplate* pThis, CDocument* pDoc);
extern "C" CDocument* MS_ABI impl__OpenDocumentFile_CMultiDocTemplate__UEAAPEAVCDocument__PEB_WH_Z(CMultiDocTemplate* pThis, const wchar_t* lpszPathName, int bMakeVisible);
extern "C" void MS_ABI impl__AddDocument_CMultiDocTemplate__UEAAXPEAVCDocument___Z(CMultiDocTemplate* pThis, CDocument* pDoc);
extern "C" void MS_ABI impl__RemoveDocument_CMultiDocTemplate__UEAAXPEAVCDocument___Z(CMultiDocTemplate* pThis, CDocument* pDoc);
extern "C" void* MS_ABI impl__GetFirstDocPosition_CMultiDocTemplate__UEBAPEAU__POSITION__XZ(const CMultiDocTemplate* pThis);
extern "C" CDocument* MS_ABI impl__GetNextDoc_CMultiDocTemplate__UEBAPEAVCDocument__AEAPEAU__POSITION___Z(const CMultiDocTemplate* pThis, void*& pos);
extern "C" void MS_ABI impl__SetDefaultTitle_CMultiDocTemplate__UEAAXPEAVCDocument___Z(CMultiDocTemplate* pThis, CDocument* pDoc);

// =============================================================================
// CDocument Member Function Implementations (vtable entries)
// =============================================================================
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

// =============================================================================
// CView Member Function Implementations (vtable entries)
// =============================================================================
CDocument* CView::GetDocument() const { return impl__GetDocument_CView__UEBAPEAVCDocument__XZ(this); }
void CView::OnUpdate(CView* pSender, unsigned long lHint, CObject* pHint) { impl__OnUpdate_CView__UEAAXPEAV1_KPEAVCObject___Z(this, pSender, lHint, pHint); }
void CView::OnInitialUpdate() { impl__OnInitialUpdate_CView__UEAAXXZ(this); }
int CView::OnPreparePrinting(void* pInfo) { return impl__OnPreparePrinting_CView__UEAAHPEAX_Z(this, pInfo); }
void CView::OnBeginPrinting(void* pDC, void* pInfo) { impl__OnBeginPrinting_CView__UEAAXPEAX0_Z(this, pDC, pInfo); }
void CView::OnEndPrinting(void* pDC, void* pInfo) { impl__OnEndPrinting_CView__UEAAXPEAX0_Z(this, pDC, pInfo); }
void CView::OnActivateView(int bActivate, CView* pActivateView, CView* pDeactiveView) { impl__OnActivateView_CView__UEAAXHPEAV1_0_Z(this, bActivate, pActivateView, pDeactiveView); }
CFrameWnd* CView::GetParentFrame() const { return impl__GetParentFrame_CView__UEBAPEAVCFrameWnd__XZ(this); }

// =============================================================================
// CScrollView Member Function Implementations (vtable entries)
// =============================================================================
void CScrollView::SetScrollSizes(int nMapMode, const SIZE& sizeTotal, const SIZE& sizePage, const SIZE& sizeLine) { impl__SetScrollSizes_CScrollView__UEAAXHAEBUtagSIZE__00_Z(this, nMapMode, &sizeTotal, &sizePage, &sizeLine); }
void CScrollView::OnInitialUpdate() { impl__OnInitialUpdate_CScrollView__UEAAXXZ(this); }
void CScrollView::OnDraw(void* pDC) { impl__OnDraw_CScrollView__UEAAXPEAX_Z(this, pDC); }
void CScrollView::OnUpdate(CView* pSender, unsigned long lHint, CObject* pHint) { impl__OnUpdate_CView__UEAAXPEAV1_KPEAVCObject___Z(this, pSender, lHint, pHint); }

void CScrollView::GetScrollBarSizes(SIZE& sizeSb) {
    sizeSb.cx = ::GetSystemMetrics(SM_CXVSCROLL);
    sizeSb.cy = ::GetSystemMetrics(SM_CYHSCROLL);
}

void CScrollView::GetTrueClientSize(SIZE& size, SIZE& sizeSb) const {
    if (m_hWnd) {
        RECT rc;
        ::GetClientRect(m_hWnd, &rc);
        size.cx = static_cast<int>(rc.right - rc.left);
        size.cy = static_cast<int>(rc.bottom - rc.top);
    } else {
        size.cx = 0;
        size.cy = 0;
    }
    sizeSb.cx = ::GetSystemMetrics(SM_CXVSCROLL);
    sizeSb.cy = ::GetSystemMetrics(SM_CYHSCROLL);
}

// Symbol: ?GetScrollBarSizes@CScrollView@@IEAAXAEAVCSize@@@Z
extern "C" void MS_ABI impl__GetScrollBarSizes_CScrollView__IEAAXAEAVCSize___Z(
    CScrollView* pThis, SIZE* pSizeSb)
{
    if (!pSizeSb) {
        return;
    }
    if (pThis) {
        pThis->GetScrollBarSizes(*pSizeSb);
    } else {
        pSizeSb->cx = 0;
        pSizeSb->cy = 0;
    }
}

// Symbol: ?GetTrueClientSize@CScrollView@@IEAAHAEAVCSize@@0@Z
extern "C" int MS_ABI impl__GetTrueClientSize_CScrollView__IEAAHAEAVCSize__0_Z(
    const CScrollView* pThis, SIZE* pSize, SIZE* pSizeSb)
{
    if (!pSize || !pSizeSb) {
        return FALSE;
    }
    if (pThis) {
        pThis->GetTrueClientSize(*pSize, *pSizeSb);
        return TRUE;
    }
    pSize->cx = 0;
    pSize->cy = 0;
    pSizeSb->cx = 0;
    pSizeSb->cy = 0;
    return FALSE;
}

void CScrollView::ScrollToPosition(POINT pt) {
    if (!m_hWnd) return;

    // Clamp to valid range
    if (pt.x < 0) pt.x = 0;
    if (pt.y < 0) pt.y = 0;
    if (pt.x > m_totalLog.cx) pt.x = m_totalLog.cx;
    if (pt.y > m_totalLog.cy) pt.y = m_totalLog.cy;

    // Set scroll positions
    ::SetScrollPos(m_hWnd, SB_HORZ, pt.x, TRUE);
    ::SetScrollPos(m_hWnd, SB_VERT, pt.y, TRUE);

    // Scroll the window content
    ::ScrollWindow(m_hWnd, 0, 0, nullptr, nullptr);
    ::InvalidateRect(m_hWnd, nullptr, TRUE);
}

CPoint CScrollView::GetScrollPosition() const {
    if (!m_hWnd) {
        return CPoint(0, 0);
    }
    return CPoint(::GetScrollPos(m_hWnd, SB_HORZ), ::GetScrollPos(m_hWnd, SB_VERT));
}

void CScrollView::FillOutsideRect(void* pDC, void* pBrush) {
    if (!m_hWnd || !pDC) return;

    HDC hDC = static_cast<HDC>(pDC);
    HBRUSH hBrush = pBrush ? static_cast<HBRUSH>(pBrush) :
                            static_cast<HBRUSH>(::GetStockObject(WHITE_BRUSH));

    RECT rcClient;
    ::GetClientRect(m_hWnd, &rcClient);

    // Fill areas outside the scroll view content
    RECT rcFill;

    // Right of content
    if (rcClient.right > m_totalLog.cx) {
        rcFill.left = m_totalLog.cx;
        rcFill.top = rcClient.top;
        rcFill.right = rcClient.right;
        rcFill.bottom = rcClient.bottom;
        ::FillRect(hDC, &rcFill, hBrush);
    }

    // Below content
    if (rcClient.bottom > m_totalLog.cy) {
        rcFill.left = rcClient.left;
        rcFill.top = m_totalLog.cy;
        rcFill.right = m_totalLog.cx;
        rcFill.bottom = rcClient.bottom;
        ::FillRect(hDC, &rcFill, hBrush);
    }
}

void CScrollView::ResizeParentToFit(int bShrinkOnly) {
    if (!m_hWnd) return;

    HWND hWndParent = ::GetParent(m_hWnd);
    if (!hWndParent) return;

    // Calculate desired parent size based on scroll view content
    SIZE sizeSb;
    GetScrollBarSizes(sizeSb);

    int nWidth = m_totalLog.cx + sizeSb.cx +
                 ::GetSystemMetrics(SM_CXFRAME) * 2;
    int nHeight = m_totalLog.cy + sizeSb.cy +
                  ::GetSystemMetrics(SM_CYFRAME) * 2 +
                  ::GetSystemMetrics(SM_CYCAPTION);

    RECT rcParent;
    ::GetWindowRect(hWndParent, &rcParent);
    int nCurrentWidth = rcParent.right - rcParent.left;
    int nCurrentHeight = rcParent.bottom - rcParent.top;

    // Apply shrink-only constraint
    if (bShrinkOnly) {
        if (nWidth > nCurrentWidth) nWidth = nCurrentWidth;
        if (nHeight > nCurrentHeight) nHeight = nCurrentHeight;
    }

    ::SetWindowPos(hWndParent, nullptr, 0, 0, nWidth, nHeight,
                   SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
}

// =============================================================================
// CFormView Member Function Implementations (vtable entries)
// =============================================================================
void CFormView::OnDraw(void* pDC) { impl__OnDraw_CFormView__UEAAXPEAX_Z(this, pDC); }
void CFormView::OnInitialUpdate() { impl__OnInitialUpdate_CFormView__UEAAXXZ(this); }
void CFormView::DoDataExchange(void* pDX) { impl__DoDataExchange_CFormView__UEAAXPEAX_Z(this, pDX); }
int CFormView::Create(const wchar_t* lpszClassName, const wchar_t* lpszWindowName,
                      DWORD dwStyle, const struct tagRECT& rect, CWnd* pParentWnd,
                      unsigned int nID, CCreateContext* pContext) {
    (void)lpszClassName;
    (void)lpszWindowName;
    (void)dwStyle;
    (void)pContext;

    if (!pParentWnd || !pParentWnd->m_hWnd) return FALSE;

    // Get resource handle
    HINSTANCE hInst = ::GetModuleHandleW(nullptr);

    // Create the form view as a dialog-based child window
    m_hWnd = ::CreateDialogParamW(
        hInst,
        m_lpszTemplateName,
        pParentWnd->m_hWnd,
        nullptr,  // Dialog proc not needed - we're using it as a child window
        0
    );

    if (!m_hWnd) return FALSE;

    // Set the control ID
    ::SetWindowLongPtrW(m_hWnd, GWLP_ID, nID);

    // Position the window
    ::SetWindowPos(m_hWnd, nullptr, rect.left, rect.top,
                   rect.right - rect.left, rect.bottom - rect.top,
                   SWP_NOZORDER | SWP_NOACTIVATE);

    // Show the window
    ::ShowWindow(m_hWnd, SW_SHOW);

    return TRUE;
}

// =============================================================================
// CEditView Member Function Implementations (vtable entries)
// =============================================================================
CEdit* CEditView::GetEditCtrl() const { return impl__GetEditCtrl_CEditView__QEBAPEAVCEdit__XZ(this); }
void CEditView::OnDraw(void* pDC) { impl__OnDraw_CEditView__UEAAXPEAX_Z(this, pDC); }

void CEditView::Serialize(CArchive& ar) {
    if (!m_hWnd) return;

    if (ar.IsStoring()) {
        // Get text from edit control and write to archive
        int nLen = ::GetWindowTextLengthW(m_hWnd);
        if (nLen > 0) {
            wchar_t* pBuf = new wchar_t[nLen + 1];
            ::GetWindowTextW(m_hWnd, pBuf, nLen + 1);
            CString str(pBuf);
            ar << str;
            delete[] pBuf;
        } else {
            CString str;
            ar << str;
        }
    } else {
        // Read text from archive and set in edit control
        CString str;
        ar >> str;
        ::SetWindowTextW(m_hWnd, (const wchar_t*)str);
    }
}

void CEditView::GetSelectedText(CString& strResult) const {
    strResult.Empty();
    if (!m_hWnd) return;

    DWORD dwStart = 0, dwEnd = 0;
    ::SendMessageW(m_hWnd, EM_GETSEL, (WPARAM)&dwStart, (LPARAM)&dwEnd);

    if (dwEnd > dwStart) {
        int nLen = ::GetWindowTextLengthW(m_hWnd);
        if (nLen > 0) {
            wchar_t* pBuf = new wchar_t[nLen + 1];
            ::GetWindowTextW(m_hWnd, pBuf, nLen + 1);

            // Extract selected portion
            int nSelLen = dwEnd - dwStart;
            wchar_t* pSel = new wchar_t[nSelLen + 1];
            wcsncpy(pSel, pBuf + dwStart, nSelLen);
            pSel[nSelLen] = L'\0';

            strResult = pSel;
            delete[] pSel;
            delete[] pBuf;
        }
    }
}

int CEditView::FindText(const wchar_t* lpszFind, int nStart, int nDirection) {
    if (!m_hWnd || !lpszFind || !lpszFind[0]) return -1;

    int nLen = ::GetWindowTextLengthW(m_hWnd);
    if (nLen <= 0 || nStart >= nLen) return -1;

    wchar_t* pBuf = new wchar_t[nLen + 1];
    ::GetWindowTextW(m_hWnd, pBuf, nLen + 1);

    int nFindLen = (int)wcslen(lpszFind);
    int nResult = -1;

    if (nDirection >= 0) {
        // Search forward
        for (int i = nStart; i <= nLen - nFindLen; i++) {
            if (wcsncmp(pBuf + i, lpszFind, nFindLen) == 0) {
                nResult = i;
                break;
            }
        }
    } else {
        // Search backward
        for (int i = nStart - nFindLen; i >= 0; i--) {
            if (wcsncmp(pBuf + i, lpszFind, nFindLen) == 0) {
                nResult = i;
                break;
            }
        }
    }

    delete[] pBuf;
    return nResult;
}

int CEditView::PrintInsideRect(void* pDC, struct tagRECT& rectLayout, struct tagRECT& rectDraw, int nPage) {
    (void)nPage;
    if (!m_hWnd || !pDC) return 0;

    HDC hDC = static_cast<HDC>(pDC);

    // Get text from edit control
    int nLen = ::GetWindowTextLengthW(m_hWnd);
    if (nLen <= 0) return 0;

    wchar_t* pBuf = new wchar_t[nLen + 1];
    ::GetWindowTextW(m_hWnd, pBuf, nLen + 1);

    // Draw text in the specified rectangle
    int nHeight = ::DrawTextW(hDC, pBuf, nLen, &rectDraw,
                              DT_LEFT | DT_TOP | DT_WORDBREAK);

    // Update layout rect with text height
    rectLayout.bottom = rectLayout.top + nHeight;

    delete[] pBuf;
    return nHeight;
}

// =============================================================================
// CListView Member Function Implementations (vtable entries)
// =============================================================================
CListCtrl* CListView::GetListCtrl() const { return impl__GetListCtrl_CListView__QEBAPEAVCListCtrl__XZ(this); }
void CListView::OnDraw(void* pDC) { impl__OnDraw_CListView__UEAAXPEAX_Z(this, pDC); }
void CListView::OnInitialUpdate() { impl__OnInitialUpdate_CListView__UEAAXXZ(this); }

// =============================================================================
// CTreeView Member Function Implementations (vtable entries)
// =============================================================================
CTreeCtrl* CTreeView::GetTreeCtrl() const { return impl__GetTreeCtrl_CTreeView__QEBAPEAVCTreeCtrl__XZ(this); }
void CTreeView::OnDraw(void* pDC) { impl__OnDraw_CTreeView__UEAAXPEAX_Z(this, pDC); }
void CTreeView::OnInitialUpdate() { impl__OnInitialUpdate_CTreeView__UEAAXXZ(this); }

// =============================================================================
// CDocTemplate Member Function Implementations (vtable entries)
// =============================================================================
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

// =============================================================================
// CSingleDocTemplate Member Function Implementations (vtable entries)
// =============================================================================
CDocument* CSingleDocTemplate::OpenDocumentFile(const wchar_t* lpszPathName, int bMakeVisible) { return impl__OpenDocumentFile_CSingleDocTemplate__UEAAPEAVCDocument__PEB_WH_Z(this, lpszPathName, bMakeVisible); }
void CSingleDocTemplate::AddDocument(CDocument* pDoc) { impl__AddDocument_CSingleDocTemplate__UEAAXPEAVCDocument___Z(this, pDoc); }
void CSingleDocTemplate::RemoveDocument(CDocument* pDoc) { impl__RemoveDocument_CSingleDocTemplate__UEAAXPEAVCDocument___Z(this, pDoc); }
void* CSingleDocTemplate::GetFirstDocPosition() const { return impl__GetFirstDocPosition_CSingleDocTemplate__UEBAPEAU__POSITION__XZ(this); }
CDocument* CSingleDocTemplate::GetNextDoc(void*& rPos) const { return impl__GetNextDoc_CSingleDocTemplate__UEBAPEAVCDocument__AEAPEAU__POSITION___Z(this, rPos); }
void CSingleDocTemplate::SetDefaultTitle(CDocument* pDoc) { impl__SetDefaultTitle_CSingleDocTemplate__UEAAXPEAVCDocument___Z(this, pDoc); }

// =============================================================================
// CMultiDocTemplate Member Function Implementations (vtable entries)
// =============================================================================
CDocument* CMultiDocTemplate::OpenDocumentFile(const wchar_t* lpszPathName, int bMakeVisible) { return impl__OpenDocumentFile_CMultiDocTemplate__UEAAPEAVCDocument__PEB_WH_Z(this, lpszPathName, bMakeVisible); }
void CMultiDocTemplate::AddDocument(CDocument* pDoc) { impl__AddDocument_CMultiDocTemplate__UEAAXPEAVCDocument___Z(this, pDoc); }
void CMultiDocTemplate::RemoveDocument(CDocument* pDoc) { impl__RemoveDocument_CMultiDocTemplate__UEAAXPEAVCDocument___Z(this, pDoc); }
void* CMultiDocTemplate::GetFirstDocPosition() const { return impl__GetFirstDocPosition_CMultiDocTemplate__UEBAPEAU__POSITION__XZ(this); }
CDocument* CMultiDocTemplate::GetNextDoc(void*& rPos) const { return impl__GetNextDoc_CMultiDocTemplate__UEBAPEAVCDocument__AEAPEAU__POSITION___Z(this, rPos); }
void CMultiDocTemplate::SetDefaultTitle(CDocument* pDoc) { impl__SetDefaultTitle_CMultiDocTemplate__UEAAXPEAVCDocument___Z(this, pDoc); }

// NOTE: No asm aliases needed here - the .def file handles the MSVC symbol aliasing
// The runtime class symbols are emitted with GCC mangling and the .def file maps them
// to MSVC-mangled exports

// =============================================================================
// CDocument Implementation
// =============================================================================

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

// SetModifiedFlag
extern "C" void MS_ABI impl__SetModifiedFlag_CDocument__UEAAXH_Z(
    CDocument* pThis, int bModified)
{
    if (pThis) {
        pThis->m_bModified = bModified;
    }
}

// IsModified
extern "C" int MS_ABI impl__IsModified_CDocument__UEBAHXZ(const CDocument* pThis) {
    return pThis ? pThis->m_bModified : FALSE;
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

// GetTitle
extern "C" const wchar_t* MS_ABI impl__GetTitle_CDocument__UEBAPEB_WXZ(
    const CDocument* pThis)
{
    return pThis ? (const wchar_t*)pThis->m_strTitle : L"";
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

// GetPathName
extern "C" const wchar_t* MS_ABI impl__GetPathName_CDocument__UEBAPEB_WXZ(
    const CDocument* pThis)
{
    return pThis ? (const wchar_t*)pThis->m_strPathName : L"";
}

// AddView
extern "C" void MS_ABI impl__AddView_CDocument__UEAAXPEAVCView___Z(
    CDocument* pThis, CView* pView)
{
    if (!pThis || !pView) return;

    // Set view's document
    pView->m_pDocument = pThis;
    pView->m_pNextView = nullptr;

    // Add to end of list
    if (!pThis->m_pFirstView) {
        pThis->m_pFirstView = pView;
        pThis->m_pLastView = pView;
    } else {
        pThis->m_pLastView->m_pNextView = pView;
        pThis->m_pLastView = pView;
    }
}

// RemoveView
extern "C" void MS_ABI impl__RemoveView_CDocument__UEAAXPEAVCView___Z(
    CDocument* pThis, CView* pView)
{
    if (!pThis || !pView) return;

    // Find and remove from list
    CView* pPrev = nullptr;
    CView* pCur = pThis->m_pFirstView;
    while (pCur) {
        if (pCur == pView) {
            if (pPrev) {
                pPrev->m_pNextView = pCur->m_pNextView;
            } else {
                pThis->m_pFirstView = pCur->m_pNextView;
            }
            if (pThis->m_pLastView == pView) {
                pThis->m_pLastView = pPrev;
            }
            pView->m_pDocument = nullptr;
            pView->m_pNextView = nullptr;
            break;
        }
        pPrev = pCur;
        pCur = pCur->m_pNextView;
    }
}

// GetFirstViewPosition
extern "C" void* MS_ABI impl__GetFirstViewPosition_CDocument__UEBAPEAXXZ(
    const CDocument* pThis)
{
    return pThis ? pThis->m_pFirstView : nullptr;
}

// GetNextView
extern "C" CView* MS_ABI impl__GetNextView_CDocument__UEBAPEAVCView__AEAPEAX_Z(
    const CDocument* pThis, void*& pos)
{
    if (!pThis || !pos) return nullptr;

    CView* pView = static_cast<CView*>(pos);
    pos = pView->m_pNextView;
    return pView;
}

// UpdateAllViews
extern "C" void MS_ABI impl__UpdateAllViews_CDocument__UEAAXPEAVCView__KPEAVCObject___Z(
    CDocument* pThis, CView* pSender, unsigned long lHint, CObject* pHint)
{
    if (!pThis) return;

    CView* pView = pThis->m_pFirstView;
    while (pView) {
        if (pView != pSender) {
            pView->OnUpdate(pSender, lHint, pHint);
        }
        pView = pView->m_pNextView;
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

// Serialize (CDocument override)
extern "C" void MS_ABI impl__Serialize_CDocument__UEAAXAEAVCArchive___Z(
    CDocument* pThis, CArchive* ar)
{
    if (!pThis || !ar) return;
    if (ar->IsStoring()) {
        (*ar) << pThis->m_strTitle;
        (*ar) << pThis->m_strPathName;
    } else {
        (*ar) >> pThis->m_strTitle;
        (*ar) >> pThis->m_strPathName;
    }
}

// =============================================================================
// CView Implementation
// =============================================================================

// Default Constructor
extern "C" void MS_ABI impl___0CView__QEAA_XZ(CView* pThis) {
    if (!pThis) return;
    pThis->m_hWnd = nullptr;
    pThis->m_pDocument = nullptr;
    pThis->m_pNextView = nullptr;
}

// Destructor
// Symbol: ??1CView@@UEAA@XZ
extern "C" void MS_ABI impl___1CView__UEAA_XZ(CView* pThis) {
    if (!pThis) return;
    if (pThis->m_pDocument) {
        pThis->m_pDocument->RemoveView(pThis);
    }
}

// GetDocument
extern "C" CDocument* MS_ABI impl__GetDocument_CView__UEBAPEAVCDocument__XZ(
    const CView* pThis)
{
    return pThis ? pThis->m_pDocument : nullptr;
}

// OnUpdate
extern "C" void MS_ABI impl__OnUpdate_CView__UEAAXPEAV1_KPEAVCObject___Z(
    CView* pThis, CView* pSender, unsigned long lHint, CObject* pHint)
{
    (void)pSender;
    (void)lHint;
    (void)pHint;

    if (pThis && pThis->m_hWnd) {
        // Invalidate the view to force a redraw
        ::InvalidateRect(pThis->m_hWnd, nullptr, TRUE);
    }
}

// OnInitialUpdate
// Symbol: ?OnInitialUpdate@CView@@UEAAXXZ
extern "C" void MS_ABI impl__OnInitialUpdate_CView__UEAAXXZ(CView* pThis) {
    if (pThis) {
        pThis->OnUpdate(nullptr, 0, nullptr);
    }
}

// OnPreparePrinting
extern "C" int MS_ABI impl__OnPreparePrinting_CView__UEAAHPEAX_Z(
    CView* pThis, void* pInfo)
{
    (void)pThis;
    (void)pInfo;
    return TRUE;
}

// Symbol: ?OnPreparePrinting@CView@@MEAAHPEAUCPrintInfo@@@Z
extern "C" int MS_ABI impl__OnPreparePrinting_CView__MEAAHPEAUCPrintInfo___Z(
    CView* pThis, void* pInfo)
{
    return impl__OnPreparePrinting_CView__UEAAHPEAX_Z(pThis, pInfo);
}

// OnBeginPrinting
extern "C" void MS_ABI impl__OnBeginPrinting_CView__UEAAXPEAX0_Z(
    CView* pThis, void* pDC, void* pInfo)
{
    (void)pThis;
    (void)pDC;
    (void)pInfo;
}

// Symbol: ?OnBeginPrinting@CView@@MEAAXPEAVCDC@@PEAUCPrintInfo@@@Z
extern "C" void MS_ABI impl__OnBeginPrinting_CView__MEAAXPEAVCDC__PEAUCPrintInfo___Z(
    CView* pThis, void* pDC, void* pInfo)
{
    impl__OnBeginPrinting_CView__UEAAXPEAX0_Z(pThis, pDC, pInfo);
}

// OnEndPrinting
extern "C" void MS_ABI impl__OnEndPrinting_CView__UEAAXPEAX0_Z(
    CView* pThis, void* pDC, void* pInfo)
{
    (void)pThis;
    (void)pDC;
    (void)pInfo;
}

// Symbol: ?OnEndPrinting@CView@@MEAAXPEAVCDC@@PEAUCPrintInfo@@@Z
extern "C" void MS_ABI impl__OnEndPrinting_CView__MEAAXPEAVCDC__PEAUCPrintInfo___Z(
    CView* pThis, void* pDC, void* pInfo)
{
    impl__OnEndPrinting_CView__UEAAXPEAX0_Z(pThis, pDC, pInfo);
}

// OnActivateView
extern "C" void MS_ABI impl__OnActivateView_CView__UEAAXHPEAV1_0_Z(
    CView* pThis, int bActivate, CView* pActivateView, CView* pDeactiveView)
{
    (void)pActivateView;
    (void)pDeactiveView;

    if (!pThis || !pThis->m_hWnd) {
        return;
    }

    if (bActivate) {
        ::SetFocus(pThis->m_hWnd);
    }
}

// Symbol: ?OnActivateView@CView@@MEAAXHPEAV1@0@Z
extern "C" void MS_ABI impl__OnActivateView_CView__MEAAXHPEAV1_0_Z(
    CView* pThis, int bActivate, CView* pActivateView, CView* pDeactiveView)
{
    impl__OnActivateView_CView__UEAAXHPEAV1_0_Z(pThis, bActivate, pActivateView, pDeactiveView);
}

// GetParentFrame
extern "C" CFrameWnd* MS_ABI impl__GetParentFrame_CView__UEBAPEAVCFrameWnd__XZ(
    const CView* pThis)
{
    if (!pThis || !pThis->m_hWnd) return nullptr;

    HWND hWndParent = ::GetParent(pThis->m_hWnd);
    // In a real implementation, we'd look up the CFrameWnd from HWND
    // For now, return nullptr
    (void)hWndParent;
    return nullptr;
}

// =============================================================================
// CScrollView Implementation
// =============================================================================

// Default Constructor
extern "C" void MS_ABI impl___0CScrollView__QEAA_XZ(CScrollView* pThis) {
    if (!pThis) return;
    impl___0CView__QEAA_XZ(pThis);
    pThis->m_totalLog.cx = 0;
    pThis->m_totalLog.cy = 0;
    pThis->m_pageDev.cx = 0;
    pThis->m_pageDev.cy = 0;
    pThis->m_lineDev.cx = 0;
    pThis->m_lineDev.cy = 0;
    pThis->m_nMapMode = MM_TEXT;
}

// Destructor
// Symbol: ??1CScrollView@@UEAA@XZ
extern "C" void MS_ABI impl___1CScrollView__UEAA_XZ(CScrollView* pThis) {
    impl___1CView__UEAA_XZ(pThis);
}

// SetScrollSizes
extern "C" void MS_ABI impl__SetScrollSizes_CScrollView__UEAAXHAEBUtagSIZE__00_Z(
    CScrollView* pThis, int nMapMode, const SIZE* sizeTotal,
    const SIZE* sizePage, const SIZE* sizeLine)
{
    if (!pThis) return;

    pThis->m_nMapMode = nMapMode;
    pThis->m_totalLog = *sizeTotal;

    if (sizePage && (sizePage->cx > 0 || sizePage->cy > 0)) {
        pThis->m_pageDev = *sizePage;
    } else {
        pThis->m_pageDev.cx = sizeTotal->cx / 10;
        pThis->m_pageDev.cy = sizeTotal->cy / 10;
    }

    if (sizeLine && (sizeLine->cx > 0 || sizeLine->cy > 0)) {
        pThis->m_lineDev = *sizeLine;
    } else {
        pThis->m_lineDev.cx = pThis->m_pageDev.cx / 10;
        pThis->m_lineDev.cy = pThis->m_pageDev.cy / 10;
    }

    if (pThis->m_hWnd) {
        // Update scroll bars
        SCROLLINFO si = { sizeof(si) };
        si.fMask = SIF_PAGE | SIF_RANGE;
        si.nMin = 0;
        si.nMax = sizeTotal->cx;
        si.nPage = pThis->m_pageDev.cx;
        ::SetScrollInfo(pThis->m_hWnd, SB_HORZ, &si, TRUE);

        si.nMax = sizeTotal->cy;
        si.nPage = pThis->m_pageDev.cy;
        ::SetScrollInfo(pThis->m_hWnd, SB_VERT, &si, TRUE);
    }
}

// OnInitialUpdate (CScrollView override)
extern "C" void MS_ABI impl__OnInitialUpdate_CScrollView__UEAAXXZ(CScrollView* pThis) {
    if (pThis) {
        // Set default scroll sizes if not set
        if (pThis->m_totalLog.cx == 0 && pThis->m_totalLog.cy == 0) {
            SIZE sizeDefault = { 100, 100 };
            impl__SetScrollSizes_CScrollView__UEAAXHAEBUtagSIZE__00_Z(
                pThis, MM_TEXT, &sizeDefault, nullptr, nullptr);
        }
        impl__OnInitialUpdate_CView__UEAAXXZ(pThis);
    }
}

// OnDraw (CScrollView) - pure virtual in CView, needs implementation
extern "C" void MS_ABI impl__OnDraw_CScrollView__UEAAXPEAX_Z(
    CScrollView* pThis, void* pDC)
{
    (void)pThis;
    (void)pDC;
    // Default: nothing to draw
}

// =============================================================================
// CFormView Implementation
// =============================================================================

// Default Constructor
extern "C" void MS_ABI impl___0CFormView__QEAA_XZ(CFormView* pThis) {
    if (!pThis) return;
    impl___0CScrollView__QEAA_XZ(pThis);
    pThis->m_lpszTemplateName = nullptr;
    pThis->m_nIDHelp = 0;
}

// Constructor with template name
extern "C" void MS_ABI impl___0CFormView__QEAA_PEB_W_Z(
    CFormView* pThis, const wchar_t* lpszTemplateName)
{
    if (!pThis) return;
    impl___0CScrollView__QEAA_XZ(pThis);
    pThis->m_lpszTemplateName = lpszTemplateName;
    pThis->m_nIDHelp = 0;
}

// Constructor with resource ID
extern "C" void MS_ABI impl___0CFormView__QEAA_I_Z(
    CFormView* pThis, unsigned int nIDTemplate)
{
    if (!pThis) return;
    impl___0CScrollView__QEAA_XZ(pThis);
    pThis->m_lpszTemplateName = MAKEINTRESOURCEW(nIDTemplate);
    pThis->m_nIDHelp = nIDTemplate;
}

// Symbol: ??0CFormView@@IEAA@PEB_W@Z
extern "C" void MS_ABI impl___0CFormView__IEAA_PEB_W_Z(
    CFormView* pThis, const wchar_t* lpszTemplateName)
{
    impl___0CFormView__QEAA_PEB_W_Z(pThis, lpszTemplateName);
}

// Symbol: ??0CFormView@@IEAA@I@Z
extern "C" void MS_ABI impl___0CFormView__IEAA_I_Z(
    CFormView* pThis, unsigned int nIDTemplate)
{
    impl___0CFormView__QEAA_I_Z(pThis, nIDTemplate);
}

// Destructor
// Symbol: ??1CFormView@@UEAA@XZ
extern "C" void MS_ABI impl___1CFormView__UEAA_XZ(CFormView* pThis) {
    impl___1CScrollView__UEAA_XZ(pThis);
}

// OnDraw (CFormView) - form views don't draw, the dialog template does
extern "C" void MS_ABI impl__OnDraw_CFormView__UEAAXPEAX_Z(
    CFormView* pThis, void* pDC)
{
    (void)pThis;
    (void)pDC;
    // Form views are drawn by their child controls
}

// OnInitialUpdate (CFormView)
// Symbol: ?OnInitialUpdate@CFormView@@UEAAXXZ
extern "C" void MS_ABI impl__OnInitialUpdate_CFormView__UEAAXXZ(CFormView* pThis) {
    impl__OnInitialUpdate_CScrollView__UEAAXXZ(pThis);
}

// DoDataExchange
extern "C" void MS_ABI impl__DoDataExchange_CFormView__UEAAXPEAX_Z(
    CFormView* pThis, void* pDX)
{
    (void)pThis;
    (void)pDX;
    // Override in derived class to transfer data
}

// =============================================================================
// CEditView Implementation
// =============================================================================

// Default Constructor
// Symbol: ??0CEditView@@QEAA@XZ
extern "C" void MS_ABI impl___0CEditView__QEAA_XZ(CEditView* pThis) {
    if (!pThis) return;
    impl___0CView__QEAA_XZ(pThis);
    pThis->m_pEditCtrl = nullptr;
}

// Destructor
// Symbol: ??1CEditView@@UEAA@XZ
extern "C" void MS_ABI impl___1CEditView__UEAA_XZ(CEditView* pThis) {
    impl___1CView__UEAA_XZ(pThis);
}

// GetEditCtrl
extern "C" CEdit* MS_ABI impl__GetEditCtrl_CEditView__QEBAPEAVCEdit__XZ(
    const CEditView* pThis)
{
    return pThis ? pThis->m_pEditCtrl : nullptr;
}

// Symbol: ?CreateObject@CEditView@@SAPEAVCObject@@XZ
extern "C" CObject* MS_ABI impl__CreateObject_CEditView__SAPEAVCObject__XZ() {
    return new CEditView();
}

// OnDraw (CEditView)
extern "C" void MS_ABI impl__OnDraw_CEditView__UEAAXPEAX_Z(
    CEditView* pThis, void* pDC)
{
    (void)pThis;
    (void)pDC;
    // Edit control draws itself
}

// =============================================================================
// CListView Implementation
// =============================================================================

// Default Constructor
extern "C" void MS_ABI impl___0CListView__QEAA_XZ(CListView* pThis) {
    if (!pThis) return;
    impl___0CView__QEAA_XZ(pThis);
    pThis->m_pListCtrl = nullptr;
}

// Destructor
extern "C" void MS_ABI impl___1CListView__UEAA_XZ(CListView* pThis) {
    impl___1CView__UEAA_XZ(pThis);
}

// GetListCtrl
extern "C" CListCtrl* MS_ABI impl__GetListCtrl_CListView__QEBAPEAVCListCtrl__XZ(
    const CListView* pThis)
{
    return pThis ? pThis->m_pListCtrl : nullptr;
}

// Symbol: ?CreateObject@CListView@@SAPEAVCObject@@XZ
extern "C" CObject* MS_ABI impl__CreateObject_CListView__SAPEAVCObject__XZ() {
    return new CListView();
}

// OnDraw (CListView)
extern "C" void MS_ABI impl__OnDraw_CListView__UEAAXPEAX_Z(
    CListView* pThis, void* pDC)
{
    (void)pThis;
    (void)pDC;
    // List control draws itself
}

// OnInitialUpdate (CListView)
extern "C" void MS_ABI impl__OnInitialUpdate_CListView__UEAAXXZ(CListView* pThis) {
    impl__OnInitialUpdate_CView__UEAAXXZ(pThis);
}

// =============================================================================
// CTreeView Implementation
// =============================================================================

// Default Constructor
extern "C" void MS_ABI impl___0CTreeView__QEAA_XZ(CTreeView* pThis) {
    if (!pThis) return;
    impl___0CView__QEAA_XZ(pThis);
    pThis->m_pTreeCtrl = nullptr;
}

// Destructor
extern "C" void MS_ABI impl___1CTreeView__UEAA_XZ(CTreeView* pThis) {
    impl___1CView__UEAA_XZ(pThis);
}

// GetTreeCtrl
extern "C" CTreeCtrl* MS_ABI impl__GetTreeCtrl_CTreeView__QEBAPEAVCTreeCtrl__XZ(
    const CTreeView* pThis)
{
    return pThis ? pThis->m_pTreeCtrl : nullptr;
}

// Symbol: ?CreateObject@CTreeView@@SAPEAVCObject@@XZ
extern "C" CObject* MS_ABI impl__CreateObject_CTreeView__SAPEAVCObject__XZ() {
    return new CTreeView();
}

// OnDraw (CTreeView)
extern "C" void MS_ABI impl__OnDraw_CTreeView__UEAAXPEAX_Z(
    CTreeView* pThis, void* pDC)
{
    (void)pThis;
    (void)pDC;
    // Tree control draws itself
}

// OnInitialUpdate (CTreeView)
extern "C" void MS_ABI impl__OnInitialUpdate_CTreeView__UEAAXXZ(CTreeView* pThis) {
    impl__OnInitialUpdate_CView__UEAAXXZ(pThis);
}

// =============================================================================
// CDocTemplate Implementation
// =============================================================================

// Constructor
extern "C" void MS_ABI impl___0CDocTemplate__QEAA_IPEAUCRuntimeClass__00_Z(
    CDocTemplate* pThis, unsigned int nIDResource,
    CRuntimeClass* pDocClass, CRuntimeClass* pFrameClass, CRuntimeClass* pViewClass)
{
    if (!pThis) return;
    pThis->m_nIDResource = nIDResource;
    pThis->m_pDocClass = pDocClass;
    pThis->m_pFrameClass = pFrameClass;
    pThis->m_pViewClass = pViewClass;
    pThis->m_pFirstDoc = nullptr;
    pThis->m_pLastDoc = nullptr;
}

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

// OpenDocumentFile
extern "C" CDocument* MS_ABI impl__OpenDocumentFile_CDocTemplate__UEAAPEAVCDocument__PEB_WH_Z(
    CDocTemplate* pThis, const wchar_t* lpszPathName, int bMakeVisible)
{
    if (!pThis) return nullptr;

    CDocument* pDoc = impl__CreateNewDocument_CDocTemplate__UEAAPEAVCDocument__XZ(pThis);
    if (!pDoc) return nullptr;

    if (lpszPathName) {
        if (!pDoc->OnOpenDocument(lpszPathName)) {
            pDoc->OnCloseDocument();
            return nullptr;
        }
        pDoc->SetPathName(lpszPathName);
    } else {
        if (!pDoc->OnNewDocument()) {
            pDoc->OnCloseDocument();
            return nullptr;
        }
        pThis->SetDefaultTitle(pDoc);
    }

    CFrameWnd* pFrame = impl__CreateNewFrame_CDocTemplate__UEAAPEAVCFrameWnd__PEAVCDocument__PEAV2__Z(
        pThis, pDoc, nullptr);
    if (!pFrame) {
        pDoc->OnCloseDocument();
        return nullptr;
    }

    impl__InitialUpdateFrame_CDocTemplate__UEAAXPEAVCFrameWnd__PEAVCDocument__H_Z(
        pThis, pFrame, pDoc, bMakeVisible);

    return pDoc;
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

// GetFirstDocPosition
extern "C" void* MS_ABI impl__GetFirstDocPosition_CDocTemplate__UEBAPEAXXZ(
    const CDocTemplate* pThis)
{
    return pThis ? pThis->m_pFirstDoc : nullptr;
}

// GetNextDoc
extern "C" CDocument* MS_ABI impl__GetNextDoc_CDocTemplate__UEBAPEAVCDocument__AEAPEAX_Z(
    const CDocTemplate* pThis, void*& pos)
{
    if (!pThis || !pos) return nullptr;

    CDocument* pDoc = static_cast<CDocument*>(pos);
    pos = pDoc->m_pNextDoc;
    return pDoc;
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

// SetDefaultTitle
extern "C" void MS_ABI impl__SetDefaultTitle_CDocTemplate__UEAAXPEAVCDocument___Z(
    CDocTemplate* pThis, CDocument* pDoc)
{
    (void)pThis;
    if (pDoc) {
        pDoc->SetTitle(L"Untitled");
    }
}

// GetDocString
extern "C" int MS_ABI impl__GetDocString_CDocTemplate__UEBAHAEAVCString__H_Z(
    const CDocTemplate* pThis, CString* rString, int nID)
{
    (void)pThis;
    (void)nID;
    if (rString) {
        rString->Empty();
    }
    return FALSE;  // String not found
}

// Symbol: ?GetDocString@CDocTemplate@@UEBAHAEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@W4DocStringIndex@1@@Z
extern "C" int MS_ABI impl__GetDocString_CDocTemplate__UEBAHAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__W4DocStringIndex_1__Z(
    const CDocTemplate* pThis, CString* rString, int nID)
{
    return impl__GetDocString_CDocTemplate__UEBAHAEAVCString__H_Z(pThis, rString, nID);
}

// MatchDocType
extern "C" int MS_ABI impl__MatchDocType_CDocTemplate__UEAAHPEB_WAEAPEAVCDocument___Z(
    CDocTemplate* pThis, const wchar_t* lpszPathName, CDocument*& rpDocMatch)
{
    enum ConfidenceCompat {
        noAttempt = 0,
        maybeAttemptForeign = 1,
        maybeAttemptNative = 2,
        yesAttemptForeign = 3,
        yesAttemptNative = 4,
        yesAlreadyOpen = 5,
    };

    rpDocMatch = nullptr;
    if (!pThis || !lpszPathName || !lpszPathName[0]) {
        return noAttempt;
    }

    void* pos = pThis->GetFirstDocPosition();
    while (pos) {
        CDocument* pDoc = pThis->GetNextDoc(pos);
        if (!pDoc) {
            continue;
        }

        const wchar_t* existingPath = pDoc->GetPathName();
        if (existingPath && existingPath[0] && PathsEqualInsensitive(existingPath, lpszPathName)) {
            rpDocMatch = pDoc;
            return yesAlreadyOpen;
        }
    }

    CString filterExt;
    if (LoadTemplateDocString(pThis->m_nIDResource, kFilterExt, filterExt) &&
        !filterExt.IsEmpty() &&
        PathHasExtensionInsensitive(lpszPathName, filterExt.GetString())) {
        return yesAttemptNative;
    }

    return yesAttemptForeign;
}

// Symbol: ?MatchDocType@CDocTemplate@@UEAA?AW4Confidence@1@PEB_WAEAPEAVCDocument@@@Z
extern "C" int MS_ABI impl__MatchDocType_CDocTemplate__UEAA_AW4Confidence_1_PEB_WAEAPEAVCDocument___Z(
    CDocTemplate* pThis, const wchar_t* lpszPathName, CDocument*& rpDocMatch)
{
    return impl__MatchDocType_CDocTemplate__UEAAHPEB_WAEAPEAVCDocument___Z(
        pThis, lpszPathName, rpDocMatch);
}

// =============================================================================
// CSingleDocTemplate Implementation
// =============================================================================

// Default Constructor
extern "C" void MS_ABI impl___0CSingleDocTemplate__QEAA_XZ(CSingleDocTemplate* pThis) {
    if (!pThis) return;
    pThis->m_nIDResource = 0;
    pThis->m_pDocClass = nullptr;
    pThis->m_pFrameClass = nullptr;
    pThis->m_pViewClass = nullptr;
    pThis->m_pFirstDoc = nullptr;
    pThis->m_pLastDoc = nullptr;
    pThis->m_pOnlyDoc = nullptr;
}

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

// =============================================================================
// CMultiDocTemplate Implementation
// =============================================================================

// Default Constructor
extern "C" void MS_ABI impl___0CMultiDocTemplate__QEAA_XZ(CMultiDocTemplate* pThis) {
    if (!pThis) return;
    pThis->m_nIDResource = 0;
    pThis->m_pDocClass = nullptr;
    pThis->m_pFrameClass = nullptr;
    pThis->m_pViewClass = nullptr;
    pThis->m_pFirstDoc = nullptr;
    pThis->m_pLastDoc = nullptr;
}

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

namespace {
const AFX_MSGMAP_ENTRY g_docviewEmptyEntries[] = {
    {0, 0, 0, 0, AfxSig_end, (AFX_PMSG)0}
};
const AFX_MSGMAP g_docviewEmptyMap = { nullptr, g_docviewEmptyEntries };
constexpr const AFX_INTERFACEMAP* g_docviewEmptyInterfaceMap = nullptr;
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

// Symbol: ?GetRuntimeClass@CView@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CView__UEBAPEAUCRuntimeClass__XZ(const CView*) { return &CView::classCView; }
// Symbol: ?GetThisClass@CView@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CView__SAPEAUCRuntimeClass__XZ() { return &CView::classCView; }
// Symbol: ?GetMessageMap@CView@@MEBAPEBUAFX_MSGMAP@@XZ
extern "C" const AFX_MSGMAP* MS_ABI impl__GetMessageMap_CView__MEBAPEBUAFX_MSGMAP__XZ(const CView*) { return &g_docviewEmptyMap; }
// Symbol: ?GetThisMessageMap@CView@@KAPEBUAFX_MSGMAP@@XZ
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CView__KAPEBUAFX_MSGMAP__XZ() { return &g_docviewEmptyMap; }

// Symbol: ?GetRuntimeClass@CScrollView@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CScrollView__UEBAPEAUCRuntimeClass__XZ(const CScrollView*) { return &CScrollView::classCScrollView; }
// Symbol: ?GetThisClass@CScrollView@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CScrollView__SAPEAUCRuntimeClass__XZ() { return &CScrollView::classCScrollView; }
// Symbol: ?GetMessageMap@CScrollView@@MEBAPEBUAFX_MSGMAP@@XZ
extern "C" const AFX_MSGMAP* MS_ABI impl__GetMessageMap_CScrollView__MEBAPEBUAFX_MSGMAP__XZ(const CScrollView*) { return &g_docviewEmptyMap; }
// Symbol: ?GetThisMessageMap@CScrollView@@KAPEBUAFX_MSGMAP@@XZ
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CScrollView__KAPEBUAFX_MSGMAP__XZ() { return &g_docviewEmptyMap; }

// Symbol: ?GetRuntimeClass@CFormView@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CFormView__UEBAPEAUCRuntimeClass__XZ(const CFormView*) { return &CFormView::classCFormView; }
// Symbol: ?GetThisClass@CFormView@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CFormView__SAPEAUCRuntimeClass__XZ() { return &CFormView::classCFormView; }
// Symbol: ?GetMessageMap@CFormView@@MEBAPEBUAFX_MSGMAP@@XZ
extern "C" const AFX_MSGMAP* MS_ABI impl__GetMessageMap_CFormView__MEBAPEBUAFX_MSGMAP__XZ(const CFormView*) { return &g_docviewEmptyMap; }
// Symbol: ?GetThisMessageMap@CFormView@@KAPEBUAFX_MSGMAP@@XZ
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CFormView__KAPEBUAFX_MSGMAP__XZ() { return &g_docviewEmptyMap; }

// Symbol: ?GetRuntimeClass@CEditView@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CEditView__UEBAPEAUCRuntimeClass__XZ(const CEditView*) { return &CEditView::classCEditView; }
// Symbol: ?GetThisClass@CEditView@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CEditView__SAPEAUCRuntimeClass__XZ() { return &CEditView::classCEditView; }
// Symbol: ?GetMessageMap@CEditView@@MEBAPEBUAFX_MSGMAP@@XZ
extern "C" const AFX_MSGMAP* MS_ABI impl__GetMessageMap_CEditView__MEBAPEBUAFX_MSGMAP__XZ(const CEditView*) { return &g_docviewEmptyMap; }
// Symbol: ?GetThisMessageMap@CEditView@@KAPEBUAFX_MSGMAP@@XZ
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CEditView__KAPEBUAFX_MSGMAP__XZ() { return &g_docviewEmptyMap; }

// Symbol: ?GetRuntimeClass@CListView@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CListView__UEBAPEAUCRuntimeClass__XZ(const CListView*) { return &CListView::classCListView; }
// Symbol: ?GetThisClass@CListView@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CListView__SAPEAUCRuntimeClass__XZ() { return &CListView::classCListView; }
// Symbol: ?GetMessageMap@CListView@@MEBAPEBUAFX_MSGMAP@@XZ
extern "C" const AFX_MSGMAP* MS_ABI impl__GetMessageMap_CListView__MEBAPEBUAFX_MSGMAP__XZ(const CListView*) { return &g_docviewEmptyMap; }
// Symbol: ?GetThisMessageMap@CListView@@KAPEBUAFX_MSGMAP@@XZ
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CListView__KAPEBUAFX_MSGMAP__XZ() { return &g_docviewEmptyMap; }

// Symbol: ?GetRuntimeClass@CTreeView@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CTreeView__UEBAPEAUCRuntimeClass__XZ(const CTreeView*) { return &CTreeView::classCTreeView; }
// Symbol: ?GetThisClass@CTreeView@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CTreeView__SAPEAUCRuntimeClass__XZ() { return &CTreeView::classCTreeView; }
// Symbol: ?GetMessageMap@CTreeView@@MEBAPEBUAFX_MSGMAP@@XZ
extern "C" const AFX_MSGMAP* MS_ABI impl__GetMessageMap_CTreeView__MEBAPEBUAFX_MSGMAP__XZ(const CTreeView*) { return &g_docviewEmptyMap; }
// Symbol: ?GetThisMessageMap@CTreeView@@KAPEBUAFX_MSGMAP@@XZ
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CTreeView__KAPEBUAFX_MSGMAP__XZ() { return &g_docviewEmptyMap; }

// Symbol: ?GetRuntimeClass@CDocTemplate@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CDocTemplate__UEBAPEAUCRuntimeClass__XZ(const CDocTemplate*) { return &CDocTemplate::classCDocTemplate; }
// Symbol: ?GetThisClass@CDocTemplate@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CDocTemplate__SAPEAUCRuntimeClass__XZ() { return &CDocTemplate::classCDocTemplate; }
// Symbol: ?GetRuntimeClass@CSingleDocTemplate@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CSingleDocTemplate__UEBAPEAUCRuntimeClass__XZ(const CSingleDocTemplate*) { return &CSingleDocTemplate::classCSingleDocTemplate; }
// Symbol: ?GetThisClass@CSingleDocTemplate@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CSingleDocTemplate__SAPEAUCRuntimeClass__XZ() { return &CSingleDocTemplate::classCSingleDocTemplate; }
// Symbol: ?GetRuntimeClass@CMultiDocTemplate@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CMultiDocTemplate__UEBAPEAUCRuntimeClass__XZ(const CMultiDocTemplate*) { return &CMultiDocTemplate::classCMultiDocTemplate; }
// Symbol: ?GetThisClass@CMultiDocTemplate@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMultiDocTemplate__SAPEAUCRuntimeClass__XZ() { return &CMultiDocTemplate::classCMultiDocTemplate; }

// Symbol: ?GetRuntimeClass@CArchiveException@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CArchiveException__UEBAPEAUCRuntimeClass__XZ(const CArchiveException*) { return &CArchiveException::classCArchiveException; }

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
extern "C" void MS_ABI impl__OnDrawThumbnail_CDocument__UEAAXAEAVCDC__PEAUtagRECT___Z(CDocument*, CDC*, RECT*);
extern "C" int MS_ABI impl__GetThumbnail_CDocument__UEAAHIPEAPEAUHBITMAP____PEAW4WTS_ALPHATYPE___Z(CDocument* pThis, unsigned int size, HBITMAP* bitmap, int* alphaType) {
    if (bitmap) *bitmap = nullptr;
    if (alphaType) *alphaType = 0;
    if (!bitmap || size == 0) return FALSE;
    HDC screen = ::GetDC(nullptr);
    HDC mem = ::CreateCompatibleDC(screen);
    HBITMAP hbmp = ::CreateCompatibleBitmap(screen, size, size);
    HGDIOBJ old = ::SelectObject(mem, hbmp);
    RECT rc = {0, 0, static_cast<LONG>(size), static_cast<LONG>(size)};
    ::FillRect(mem, &rc, static_cast<HBRUSH>(::GetStockObject(WHITE_BRUSH)));
    CDC dc; dc.m_hDC = mem; dc.m_hAttribDC = mem;
    if (pThis) impl__OnDrawThumbnail_CDocument__UEAAXAEAVCDC__PEAUtagRECT___Z(pThis, &dc, &rc);
    ::SelectObject(mem, old);
    ::DeleteDC(mem);
    ::ReleaseDC(nullptr, screen);
    *bitmap = hbmp;
    return hbmp != nullptr;
}
// Symbol: ?OnDrawThumbnail@CDocument@@UEAAXAEAVCDC@@PEAUtagRECT@@@Z
extern "C" void MS_ABI impl__OnDrawThumbnail_CDocument__UEAAXAEAVCDC__PEAUtagRECT___Z(CDocument*, CDC* dc, RECT* rect) { if (dc && rect && dc->m_hDC) ::DrawTextW(dc->m_hDC, L"OpenMFC", -1, rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE); }
// Symbol: ?OnPreviewHandlerQueryFocus@CDocument@@UEAAJPEAPEAUHWND__@@@Z
extern "C" long MS_ABI impl__OnPreviewHandlerQueryFocus_CDocument__UEAAJPEAPEAUHWND_____Z(CDocument* pThis, HWND* hwnd) { if (hwnd) *hwnd = pThis && pThis->m_pFirstView ? pThis->m_pFirstView->GetSafeHwnd() : nullptr; return S_OK; }
// Symbol: ?OnPreviewHandlerTranslateAccelerator@CDocument@@UEAAJPEAUtagMSG@@@Z
extern "C" long MS_ABI impl__OnPreviewHandlerTranslateAccelerator_CDocument__UEAAJPEAUtagMSG___Z(CDocument*, MSG*) { return S_FALSE; }
// Symbol: ?IsSearchAndOrganizeHandler@CDocument@@QEBAHXZ
extern "C" int MS_ABI impl__IsSearchAndOrganizeHandler_CDocument__QEBAHXZ(const CDocument* pThis) { auto it = g_documentExtraStates.find(pThis); return it != g_documentExtraStates.end() && it->second.searchHandler; }
// Symbol: ?GetDefaultMenu@CDocument@@UEAAPEAUHMENU__@@XZ
extern "C" HMENU MS_ABI impl__GetDefaultMenu_CDocument__UEAAPEAUHMENU____XZ(CDocument* pThis) { return pThis ? g_documentExtraStates[pThis].defaultMenu : nullptr; }
// Symbol: ?GetDefaultAccelerator@CDocument@@UEAAPEAUHACCEL__@@XZ
extern "C" HACCEL MS_ABI impl__GetDefaultAccelerator_CDocument__UEAAPEAUHACCEL____XZ(CDocument* pThis) { return pThis ? g_documentExtraStates[pThis].defaultAccel : nullptr; }
// Symbol: ?OnCreatePreviewFrame@CDocument@@UEAAHXZ
extern "C" int MS_ABI impl__OnCreatePreviewFrame_CDocument__UEAAHXZ(CDocument*) { return TRUE; }
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

// CDocTemplate residuals.
// Symbol: ?LoadTemplate@CDocTemplate@@UEAAXXZ
extern "C" void MS_ABI impl__LoadTemplate_CDocTemplate__UEAAXXZ(CDocTemplate* pThis) { if (pThis) { CString s; (void)pThis->GetDocString(s, kDocName); } }
// Symbol: ?LoadTemplate@CMultiDocTemplate@@UEAAXXZ
extern "C" void MS_ABI impl__LoadTemplate_CMultiDocTemplate__UEAAXXZ(CMultiDocTemplate* pThis) { impl__LoadTemplate_CDocTemplate__UEAAXXZ(pThis); }
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

// CView residuals.
// Symbol: ?CalcWindowRect@CView@@UEAAXPEAUtagRECT@@I@Z
extern "C" void MS_ABI impl__CalcWindowRect_CView__UEAAXPEAUtagRECT__I_Z(CView* pThis, RECT* rect, unsigned int adjustType) {
    if (!pThis || !rect) return;
    DWORD style = pThis->m_hWnd ? static_cast<DWORD>(::GetWindowLongPtrW(pThis->m_hWnd, GWL_STYLE)) : WS_CHILD;
    DWORD exStyle = pThis->m_hWnd ? static_cast<DWORD>(::GetWindowLongPtrW(pThis->m_hWnd, GWL_EXSTYLE)) : 0;
    BOOL hasMenu = (adjustType != 0 && pThis->m_hWnd && ::GetMenu(pThis->m_hWnd));
    ::AdjustWindowRectEx(rect, style, hasMenu, exStyle);
}
// Symbol: ?DoPreparePrinting@CView@@QEAAHPEAUCPrintInfo@@@Z
extern "C" int MS_ABI impl__DoPreparePrinting_CView__QEAAHPEAUCPrintInfo___Z(CView* pThis, void* info) { return pThis ? pThis->OnPreparePrinting(info) : FALSE; }
// Symbol: ?DoPrintPreview@CView@@QEAAHIPEAV1@PEAUCRuntimeClass@@PEAUCPrintPreviewState@@@Z
extern "C" int MS_ABI impl__DoPrintPreview_CView__QEAAHIPEAV1_PEAUCRuntimeClass__PEAUCPrintPreviewState___Z(CView* pThis, unsigned int, CView* previewView, CRuntimeClass*, void*) { if (pThis && pThis->m_hWnd) ::ShowWindow(pThis->m_hWnd, SW_HIDE); if (previewView && previewView->m_hWnd) ::ShowWindow(previewView->m_hWnd, SW_SHOW); return previewView != nullptr; }
// Symbol: ?GetParentSplitter@CView@@SAPEAVCSplitterWnd@@PEBVCWnd@@H@Z
extern "C" void* MS_ABI impl__GetParentSplitter_CView__SAPEAVCSplitterWnd__PEBVCWnd__H_Z(const CWnd*, int) { return nullptr; }
// Symbol: ?GetScrollBarCtrl@CView@@UEBAPEAVCScrollBar@@H@Z
extern "C" CScrollBar* MS_ABI impl__GetScrollBarCtrl_CView__UEBAPEAVCScrollBar__H_Z(const CView*, int) { return nullptr; }
// Symbol: ?IsSelected@CView@@UEBAHPEBVCObject@@@Z
extern "C" int MS_ABI impl__IsSelected_CView__UEBAHPEBVCObject___Z(const CView*, const CObject*) { return FALSE; }
// Symbol: ?OnActivateFrame@CView@@MEAAXIPEAVCFrameWnd@@@Z
extern "C" void MS_ABI impl__OnActivateFrame_CView__MEAAXIPEAVCFrameWnd___Z(CView* pThis, unsigned int, CFrameWnd*) { if (pThis && pThis->m_hWnd) ::SetFocus(pThis->m_hWnd); }
// Symbol: ?OnCmdMsg@CView@@UEAAHIHPEAXPEAUAFX_CMDHANDLERINFO@@@Z
extern "C" int MS_ABI impl__OnCmdMsg_CView__UEAAHIHPEAXPEAUAFX_CMDHANDLERINFO___Z(CView* pThis, unsigned int id, int code, void* extra, AFX_CMDHANDLERINFO* info) { return pThis ? pThis->CWnd::OnCmdMsg(id, code, extra, info) : FALSE; }
// Symbol: ?OnCreate@CView@@IEAAHPEAUtagCREATESTRUCTW@@@Z
extern "C" int MS_ABI impl__OnCreate_CView__IEAAHPEAUtagCREATESTRUCTW___Z(CView*, CREATESTRUCTW*) { return 0; }
// Symbol: ?OnDestroy@CView@@IEAAXXZ
extern "C" void MS_ABI impl__OnDestroy_CView__IEAAXXZ(CView* pThis) { if (pThis && pThis->m_pDocument) pThis->m_pDocument->RemoveView(pThis); }
// Symbol: ?OnDragLeave@CView@@UEAAXXZ
extern "C" void MS_ABI impl__OnDragLeave_CView__UEAAXXZ(CView* pThis) { if (pThis && pThis->m_hWnd && ::GetCapture() == pThis->m_hWnd) ::ReleaseCapture(); }
// Symbol: ?OnDragScroll@CView@@UEAAKKVCPoint@@@Z
extern "C" unsigned long MS_ABI impl__OnDragScroll_CView__UEAAKKVCPoint___Z(CView*, unsigned long keyState, CPoint) { return keyState; }
// Symbol: ?OnDraw@CView@@MEAAXPEAVCDC@@@Z
extern "C" void MS_ABI impl__OnDraw_CView__MEAAXPEAVCDC___Z(CView* pThis, CDC* dc) { if (pThis && dc && dc->m_hDC && pThis->m_pDocument) { RECT rc = {0, 0, 10000, 10000}; ::DrawTextW(dc->m_hDC, pThis->m_pDocument->GetTitle(), -1, &rc, DT_LEFT | DT_TOP | DT_SINGLELINE); } }
// Symbol: ?OnEndPrintPreview@CView@@MEAAXPEAVCDC@@PEAUCPrintInfo@@UtagPOINT@@PEAVCPreviewView@@@Z
extern "C" void MS_ABI impl__OnEndPrintPreview_CView__MEAAXPEAVCDC__PEAUCPrintInfo__UtagPOINT__PEAVCPreviewView___Z(CView* pThis, CDC*, void*, POINT, void*) { if (pThis && pThis->m_hWnd) ::ShowWindow(pThis->m_hWnd, SW_SHOW); }
// Symbol: ?OnFilePrint@CView@@IEAAXXZ
extern "C" void MS_ABI impl__OnFilePrint_CView__IEAAXXZ(CView* pThis) { if (pThis && pThis->m_hWnd) ::InvalidateRect(pThis->m_hWnd, nullptr, TRUE); }
// Symbol: ?OnFilePrintPreview@CView@@IEAAXXZ
extern "C" void MS_ABI impl__OnFilePrintPreview_CView__IEAAXXZ(CView* pThis) { if (pThis && pThis->m_hWnd) ::ShowWindow(pThis->m_hWnd, SW_SHOW); }
// Symbol: ?OnMouseActivate@CView@@IEAAHPEAVCWnd@@II@Z
extern "C" int MS_ABI impl__OnMouseActivate_CView__IEAAHPEAVCWnd__II_Z(CView*, CWnd*, unsigned int, unsigned int) { return MA_ACTIVATE; }
// Symbol: ?OnNextPaneCmd@CView@@IEAAHI@Z
extern "C" int MS_ABI impl__OnNextPaneCmd_CView__IEAAHI_Z(CView* pThis, unsigned int commandId) {
    if (!pThis || !pThis->m_hWnd) return FALSE;
    HWND next = ::GetWindow(pThis->m_hWnd, GW_HWNDNEXT);
    if (!next) next = ::GetWindow(pThis->m_hWnd, GW_HWNDFIRST);
    if (next && next != pThis->m_hWnd) {
        ::SetFocus(next);
        return TRUE;
    }
    HWND parent = ::GetParent(pThis->m_hWnd);
    return parent ? static_cast<int>(::SendMessageW(parent, WM_COMMAND, commandId, 0) != 0) : FALSE;
}
// Symbol: ?OnPaint@CView@@IEAAXXZ
extern "C" void MS_ABI impl__OnPaint_CView__IEAAXXZ(CView* pThis) {
    if (!pThis || !pThis->m_hWnd) return;
    PAINTSTRUCT ps = {};
    HDC hdc = ::BeginPaint(pThis->m_hWnd, &ps);
    if (hdc) {
        CDC dc;
        dc.m_hDC = hdc;
        dc.m_hAttribDC = hdc;
        pThis->OnDraw(&dc);
    }
    ::EndPaint(pThis->m_hWnd, &ps);
}
// Symbol: ?OnPrepareDC@CView@@UEAAXPEAVCDC@@PEAUCPrintInfo@@@Z
extern "C" void MS_ABI impl__OnPrepareDC_CView__UEAAXPEAVCDC__PEAUCPrintInfo___Z(CView*, CDC* dc, void*) {
    if (dc && dc->m_hDC) {
        ::SetMapMode(dc->m_hDC, MM_TEXT);
        ::SetViewportOrgEx(dc->m_hDC, 0, 0, nullptr);
    }
}
// Symbol: ?OnPrint@CView@@MEAAXPEAVCDC@@PEAUCPrintInfo@@@Z
extern "C" void MS_ABI impl__OnPrint_CView__MEAAXPEAVCDC__PEAUCPrintInfo___Z(CView* pThis, CDC* dc, void* info) {
    impl__OnPrepareDC_CView__UEAAXPEAVCDC__PEAUCPrintInfo___Z(pThis, dc, info);
    if (pThis) pThis->OnDraw(dc);
}
// Symbol: ?OnPrintClient@CView@@IEAA_JPEAVCDC@@I@Z
extern "C" intptr_t MS_ABI impl__OnPrintClient_CView__IEAA_JPEAVCDC__I_Z(CView* pThis, CDC* dc, unsigned int) {
    impl__OnPrint_CView__MEAAXPEAVCDC__PEAUCPrintInfo___Z(pThis, dc, nullptr);
    return TRUE;
}
// Symbol: ?OnScroll@CView@@UEAAHIIH@Z
extern "C" int MS_ABI impl__OnScroll_CView__UEAAHIIH_Z(CView* pThis, unsigned int scrollCode, unsigned int pos, int doScroll) {
    if (!pThis || !pThis->m_hWnd) return FALSE;
    int delta = 0;
    switch (scrollCode) {
    case SB_LINEUP: delta = -16; break;
    case SB_LINEDOWN: delta = 16; break;
    case SB_PAGEUP: delta = -64; break;
    case SB_PAGEDOWN: delta = 64; break;
    case SB_THUMBPOSITION:
    case SB_THUMBTRACK: delta = static_cast<int>(pos); break;
    default: return FALSE;
    }
    if (doScroll) {
        ::ScrollWindowEx(pThis->m_hWnd, 0, -delta, nullptr, nullptr, nullptr, nullptr, SW_INVALIDATE | SW_ERASE);
        ::UpdateWindow(pThis->m_hWnd);
    }
    return TRUE;
}
// Symbol: ?OnScrollBy@CView@@UEAAHVCSize@@H@Z
extern "C" int MS_ABI impl__OnScrollBy_CView__UEAAHVCSize__H_Z(CView* pThis, CSize size, int doScroll) {
    if (!pThis || !pThis->m_hWnd) return FALSE;
    if (doScroll) {
        ::ScrollWindowEx(pThis->m_hWnd, -size.cx, -size.cy, nullptr, nullptr, nullptr, nullptr, SW_INVALIDATE | SW_ERASE);
        ::UpdateWindow(pThis->m_hWnd);
    }
    return TRUE;
}
// Symbol: ?OnSplitCmd@CView@@IEAAHI@Z
extern "C" int MS_ABI impl__OnSplitCmd_CView__IEAAHI_Z(CView* pThis, unsigned int commandId) {
    if (!pThis || !pThis->m_hWnd) return FALSE;
    HWND parent = ::GetParent(pThis->m_hWnd);
    return parent ? static_cast<int>(::SendMessageW(parent, WM_COMMAND, commandId, reinterpret_cast<LPARAM>(pThis->m_hWnd)) != 0) : FALSE;
}
// Symbol: ?OnUpdateNextPaneMenu@CView@@IEAAXPEAVCCmdUI@@@Z
extern "C" void MS_ABI impl__OnUpdateNextPaneMenu_CView__IEAAXPEAVCCmdUI___Z(CView* pThis, CCmdUI* ui) {
    BOOL enable = FALSE;
    if (pThis && pThis->m_hWnd) {
        HWND next = ::GetWindow(pThis->m_hWnd, GW_HWNDNEXT);
        HWND first = ::GetWindow(pThis->m_hWnd, GW_HWNDFIRST);
        enable = (next && next != pThis->m_hWnd) || (first && first != pThis->m_hWnd);
    }
    if (ui) impl__Enable_CCmdUI__UEAAXH_Z(ui, enable);
}
// Symbol: ?OnUpdateSplitCmd@CView@@IEAAXPEAVCCmdUI@@@Z
extern "C" void MS_ABI impl__OnUpdateSplitCmd_CView__IEAAXPEAVCCmdUI___Z(CView* pThis, CCmdUI* ui) {
    if (ui) impl__Enable_CCmdUI__UEAAXH_Z(ui, pThis && pThis->m_hWnd && ::GetParent(pThis->m_hWnd));
}
// Symbol: ?PostNcDestroy@CView@@MEAAXXZ
extern "C" void MS_ABI impl__PostNcDestroy_CView__MEAAXXZ(CView* pThis) {
    if (pThis && pThis->m_pDocument) pThis->m_pDocument->RemoveView(pThis);
}
// Symbol: ?PreCreateWindow@CView@@MEAAHAEAUtagCREATESTRUCTW@@@Z
extern "C" int MS_ABI impl__PreCreateWindow_CView__MEAAHAEAUtagCREATESTRUCTW___Z(CView*, CREATESTRUCTW* cs) {
    if (!cs) return FALSE;
    cs->style |= WS_CHILD | WS_VISIBLE;
    if (!cs->lpszClass) cs->lpszClass = L"OpenMFCView";
    return TRUE;
}

// CScrollView residuals.
// Symbol: ?sizeDefault@CScrollView@@2UtagSIZE@@B
extern "C" SIZE impl__sizeDefault_CScrollView__2UtagSIZE__B = {0, 0};
// Symbol: ?CalcWindowRect@CScrollView@@UEAAXPEAUtagRECT@@I@Z
extern "C" void MS_ABI impl__CalcWindowRect_CScrollView__UEAAXPEAUtagRECT__I_Z(CScrollView* pThis, RECT* rect, unsigned int adjustType) { impl__CalcWindowRect_CView__UEAAXPEAUtagRECT__I_Z(pThis, rect, adjustType); }
// Symbol: ?GetDeviceScrollPosition@CScrollView@@QEBA?AVCPoint@@XZ
extern "C" void MS_ABI impl__GetDeviceScrollPosition_CScrollView__QEBA_AVCPoint__XZ(CPoint* ret, const CScrollView* pThis) { new (ret) CPoint(pThis ? pThis->GetScrollPosition() : CPoint(0, 0)); }
// Symbol: ?GetDeviceScrollSizes@CScrollView@@QEBAXAEAHAEAUtagSIZE@@11@Z
extern "C" void MS_ABI impl__GetDeviceScrollSizes_CScrollView__QEBAXAEAHAEAUtagSIZE__11_Z(const CScrollView* pThis, int* mapMode, SIZE* total, SIZE* page, SIZE* line) { if (mapMode) *mapMode=pThis?pThis->m_nMapMode:MM_TEXT; if (total) *total=pThis?pThis->m_totalLog:SIZE{0,0}; if (page) *page=pThis?pThis->m_pageDev:SIZE{0,0}; if (line) *line=pThis?pThis->m_lineDev:SIZE{0,0}; }
// Symbol: ?CheckScrollBars@CScrollView@@QEBAXAEAH0@Z
extern "C" void MS_ABI impl__CheckScrollBars_CScrollView__QEBAXAEAH0_Z(const CScrollView* pThis, int* needH, int* needV) { if (needH) *needH=pThis&&pThis->m_totalLog.cx>0; if (needV) *needV=pThis&&pThis->m_totalLog.cy>0; }
// Symbol: ?ScrollToDevicePosition@CScrollView@@IEAAXUtagPOINT@@@Z
extern "C" void MS_ABI impl__ScrollToDevicePosition_CScrollView__IEAAXUtagPOINT___Z(CScrollView* pThis, POINT pt) { if (pThis) pThis->ScrollToPosition(pt); }
// Symbol: ?CenterOnPoint@CScrollView@@IEAAXVCPoint@@@Z
extern "C" void MS_ABI impl__CenterOnPoint_CScrollView__IEAAXVCPoint___Z(CScrollView* pThis, CPoint pt) { if (!pThis) return; RECT rc={}; if (pThis->m_hWnd) ::GetClientRect(pThis->m_hWnd,&rc); POINT dest={pt.x-(rc.right-rc.left)/2, pt.y-(rc.bottom-rc.top)/2}; pThis->ScrollToPosition(dest); }
// Symbol: ?OnScrollBy@CScrollView@@UEAAHVCSize@@H@Z
extern "C" int MS_ABI impl__OnScrollBy_CScrollView__UEAAHVCSize__H_Z(CScrollView* pThis, CSize size, int) { if (!pThis) return FALSE; CPoint pt=pThis->GetScrollPosition(); pt.x+=size.cx; pt.y+=size.cy; pThis->ScrollToPosition(pt); return TRUE; }
// Symbol: ?OnScroll@CScrollView@@UEAAHIIH@Z
extern "C" int MS_ABI impl__OnScroll_CScrollView__UEAAHIIH_Z(CScrollView* pThis, unsigned int code, unsigned int pos, int doScroll) { if (!pThis) return FALSE; CSize delta(0,0); if (code==SB_LINEUP) delta.cy=-pThis->m_lineDev.cy; else if (code==SB_LINEDOWN) delta.cy=pThis->m_lineDev.cy; else if (code==SB_PAGEUP) delta.cy=-pThis->m_pageDev.cy; else if (code==SB_PAGEDOWN) delta.cy=pThis->m_pageDev.cy; else if (code==SB_THUMBPOSITION||code==SB_THUMBTRACK) { CPoint pt=pThis->GetScrollPosition(); pt.y=pos; pThis->ScrollToPosition(pt); return TRUE; } return doScroll ? impl__OnScrollBy_CScrollView__UEAAHVCSize__H_Z(pThis, delta, TRUE) : TRUE; }
// Symbol: ?OnHScroll@CScrollView@@QEAAXIIPEAVCScrollBar@@@Z
extern "C" void MS_ABI impl__OnHScroll_CScrollView__QEAAXIIPEAVCScrollBar___Z(CScrollView* pThis, unsigned int code, unsigned int pos, CScrollBar*) { if (pThis) { CPoint pt=pThis->GetScrollPosition(); if (code==SB_LINELEFT) pt.x-=pThis->m_lineDev.cx; else if (code==SB_LINERIGHT) pt.x+=pThis->m_lineDev.cx; else if (code==SB_PAGELEFT) pt.x-=pThis->m_pageDev.cx; else if (code==SB_PAGERIGHT) pt.x+=pThis->m_pageDev.cx; else if (code==SB_THUMBPOSITION||code==SB_THUMBTRACK) pt.x=pos; pThis->ScrollToPosition(pt); } }
// Symbol: ?OnVScroll@CScrollView@@QEAAXIIPEAVCScrollBar@@@Z
extern "C" void MS_ABI impl__OnVScroll_CScrollView__QEAAXIIPEAVCScrollBar___Z(CScrollView* pThis, unsigned int code, unsigned int pos, CScrollBar*) { impl__OnScroll_CScrollView__UEAAHIIH_Z(pThis, code, pos, TRUE); }
// Symbol: ?DoMouseWheel@CScrollView@@QEAAHIFVCPoint@@@Z
extern "C" int MS_ABI impl__DoMouseWheel_CScrollView__QEAAHIFVCPoint___Z(CScrollView* pThis, unsigned int, short zDelta, CPoint) { if (!pThis) return FALSE; CSize delta(0, zDelta < 0 ? pThis->m_lineDev.cy*3 : -pThis->m_lineDev.cy*3); return impl__OnScrollBy_CScrollView__UEAAHVCSize__H_Z(pThis, delta, TRUE); }
// Symbol: ?OnMouseWheel@CScrollView@@QEAAHIFVCPoint@@@Z
extern "C" int MS_ABI impl__OnMouseWheel_CScrollView__QEAAHIFVCPoint___Z(CScrollView* pThis, unsigned int flags, short zDelta, CPoint pt) { return impl__DoMouseWheel_CScrollView__QEAAHIFVCPoint___Z(pThis, flags, zDelta, pt); }
// Symbol: ?GetWheelScrollDistance@CScrollView@@UEAA?AVCSize@@V2@HH@Z
extern "C" void MS_ABI impl__GetWheelScrollDistance_CScrollView__UEAA_AVCSize__V2_HH_Z(CSize* ret, CScrollView*, CSize size, int, int) { new (ret) CSize(size.cx, size.cy * 3); }
// Symbol: ?OnSize@CScrollView@@QEAAXIHH@Z
extern "C" void MS_ABI impl__OnSize_CScrollView__QEAAXIHH_Z(CScrollView* pThis, unsigned int, int, int) { if (pThis && pThis->m_hWnd) ::InvalidateRect(pThis->m_hWnd,nullptr,TRUE); }
// Symbol: ?UpdateBars@CScrollView@@IEAAXXZ
extern "C" void MS_ABI impl__UpdateBars_CScrollView__IEAAXXZ(CScrollView* pThis) { if (pThis) pThis->SetScrollSizes(pThis->m_nMapMode, pThis->m_totalLog, pThis->m_pageDev, pThis->m_lineDev); }
// Symbol: ?GetScrollBarState@CScrollView@@IEAAXVCSize@@AEAV2@1AEAVCPoint@@H@Z
extern "C" void MS_ABI impl__GetScrollBarState_CScrollView__IEAAXVCSize__AEAV2_1AEAVCPoint__H_Z(CScrollView* pThis, CSize, CSize* needSb, CSize* range, CPoint* move, int) { if (needSb) *needSb=CSize(pThis&&pThis->m_totalLog.cx>0,pThis&&pThis->m_totalLog.cy>0); if (range) *range=CSize(pThis?pThis->m_totalLog.cx:0,pThis?pThis->m_totalLog.cy:0); if (move) *move=pThis?pThis->GetScrollPosition():CPoint(0,0); }
// Symbol: ?HandleMButtonDown@CScrollView@@QEAA_J_K_J@Z
extern "C" intptr_t MS_ABI impl__HandleMButtonDown_CScrollView__QEAA_J_K_J_Z(CScrollView*, uintptr_t, intptr_t) { return 0; }
// Symbol: ?OnPrepareDC@CScrollView@@UEAAXPEAVCDC@@PEAUCPrintInfo@@@Z
extern "C" void MS_ABI impl__OnPrepareDC_CScrollView__UEAAXPEAVCDC__PEAUCPrintInfo___Z(CScrollView* pThis, CDC* dc, void*) { if (dc && dc->m_hDC && pThis) ::SetMapMode(dc->m_hDC, pThis->m_nMapMode); }
// Symbol: ?OnPrintClient@CScrollView@@QEAA_JPEAVCDC@@I@Z
extern "C" intptr_t MS_ABI impl__OnPrintClient_CScrollView__QEAA_JPEAVCDC__I_Z(CScrollView* pThis, CDC* dc, unsigned int) { if (pThis) pThis->OnDraw(dc); return TRUE; }
// Symbol: ?SetScaleToFitSize@CScrollView@@QEAAXUtagSIZE@@@Z
extern "C" void MS_ABI impl__SetScaleToFitSize_CScrollView__QEAAXUtagSIZE___Z(CScrollView* pThis, SIZE size) { if (pThis) pThis->SetScrollSizes(MM_TEXT, size, size, SIZE{1,1}); }

// CFormView residuals.
// Symbol: ?GetOccDialogInfo@CFormView@@MEAAPEAU_AFX_OCC_DIALOG_INFO@@XZ
extern "C" void* MS_ABI impl__GetOccDialogInfo_CFormView__MEAAPEAU_AFX_OCC_DIALOG_INFO__XZ(CFormView*) { return nullptr; }
// Symbol: ?SetOccDialogInfo@CFormView@@MEAAHPEAU_AFX_OCC_DIALOG_INFO@@@Z
extern "C" int MS_ABI impl__SetOccDialogInfo_CFormView__MEAAHPEAU_AFX_OCC_DIALOG_INFO___Z(CFormView*, void*) { return TRUE; }
// Symbol: ?HandleInitDialog@CFormView@@IEAA_J_K_J@Z
extern "C" intptr_t MS_ABI impl__HandleInitDialog_CFormView__IEAA_J_K_J_Z(CFormView* pThis, uintptr_t, intptr_t) { if (pThis) pThis->OnInitialUpdate(); return TRUE; }
// Symbol: ?OnActivateFrame@CFormView@@MEAAXIPEAVCFrameWnd@@@Z
extern "C" void MS_ABI impl__OnActivateFrame_CFormView__MEAAXIPEAVCFrameWnd___Z(CFormView* pThis, unsigned int state, CFrameWnd* frame) { impl__OnActivateFrame_CView__MEAAXIPEAVCFrameWnd___Z(pThis, state, frame); }
// Symbol: ?OnActivateView@CFormView@@MEAAXHPEAVCView@@0@Z
extern "C" void MS_ABI impl__OnActivateView_CFormView__MEAAXHPEAVCView__0_Z(CFormView* pThis, int active, CView* av, CView* dv) { impl__OnActivateView_CView__MEAAXHPEAV1_0_Z(pThis, active, av, dv); }
// Symbol: ?OnCreate@CFormView@@IEAAHPEAUtagCREATESTRUCTW@@@Z
extern "C" int MS_ABI impl__OnCreate_CFormView__IEAAHPEAUtagCREATESTRUCTW___Z(CFormView*, CREATESTRUCTW*) { return 0; }
// Symbol: ?OnPrintClient@CFormView@@IEAA_JPEAVCDC@@I@Z
extern "C" intptr_t MS_ABI impl__OnPrintClient_CFormView__IEAA_JPEAVCDC__I_Z(CFormView* pThis, CDC* dc, unsigned int flags) { return impl__OnPrintClient_CScrollView__QEAA_JPEAVCDC__I_Z(pThis, dc, flags); }
// Symbol: ?OnSetFocus@CFormView@@IEAAXPEAVCWnd@@@Z
extern "C" void MS_ABI impl__OnSetFocus_CFormView__IEAAXPEAVCWnd___Z(CFormView* pThis, CWnd*) { if (pThis && pThis->m_hWnd) ::SetFocus(pThis->m_hWnd); }
// Symbol: ?PreTranslateMessage@CFormView@@MEAAHPEAUtagMSG@@@Z
extern "C" int MS_ABI impl__PreTranslateMessage_CFormView__MEAAHPEAUtagMSG___Z(CFormView* pThis, MSG* msg) { return pThis && pThis->m_hWnd && msg ? ::IsDialogMessageW(pThis->m_hWnd, msg) : FALSE; }
// Symbol: ?SaveFocusControl@CFormView@@IEAAHXZ
extern "C" int MS_ABI impl__SaveFocusControl_CFormView__IEAAHXZ(CFormView* pThis) { return pThis && pThis->m_hWnd && ::GetFocus() != nullptr; }

// CEditView residuals and static data.
// Symbol: ?dwStyleDefault@CEditView@@2KB
extern "C" unsigned long impl__dwStyleDefault_CEditView__2KB = WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_MULTILINE | ES_AUTOVSCROLL | ES_AUTOHSCROLL;
// Symbol: ?nMaxSize@CEditView@@2IB
extern "C" unsigned int impl__nMaxSize_CEditView__2IB = 0x00ffffff;
// Symbol: ?GetBufferLength@CEditView@@QEBAIXZ
extern "C" unsigned int MS_ABI impl__GetBufferLength_CEditView__QEBAIXZ(const CEditView* pThis) { return static_cast<unsigned int>(EditViewText(pThis).size()); }
// Symbol: ?GetSelectedText@CEditView@@QEBAXAEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@@Z
extern "C" void MS_ABI impl__GetSelectedText_CEditView__QEBAXAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(const CEditView* pThis, CString* out) { if (out) const_cast<CEditView*>(pThis)->GetSelectedText(*out); }
// Symbol: ?FindTextW@CEditView@@QEAAHPEB_WHH@Z
extern "C" int MS_ABI impl__FindTextW_CEditView__QEAAHPEB_WHH_Z(CEditView* pThis, const wchar_t* find, int next, int caseSensitive) {
    if (!pThis || !find) return -1;
    std::wstring text = EditViewText(pThis), needle(find);
    if (!caseSensitive) { std::transform(text.begin(),text.end(),text.begin(),::towlower); std::transform(needle.begin(),needle.end(),needle.begin(),::towlower); }
    size_t start = 0;
    if (pThis->m_hWnd) { DWORD s=0,e=0; ::SendMessageW(pThis->m_hWnd, EM_GETSEL, (WPARAM)&s, (LPARAM)&e); start = next ? e : s; }
    size_t pos = next ? text.find(needle, start) : text.rfind(needle, start);
    if (pos == std::wstring::npos) return -1;
    if (pThis->m_hWnd) ::SendMessageW(pThis->m_hWnd, EM_SETSEL, pos, pos + needle.size());
    return static_cast<int>(pos);
}
// Symbol: ?LockBuffer@CEditView@@QEBAPEB_WXZ
extern "C" const wchar_t* MS_ABI impl__LockBuffer_CEditView__QEBAPEB_WXZ(const CEditView* pThis) { if (!pThis) return L""; auto& s=g_editViewExtraStates[pThis]; s.buffer=EditViewText(pThis); s.locked=s.buffer.c_str(); return s.locked; }
// Symbol: ?UnlockBuffer@CEditView@@QEBAXXZ
extern "C" void MS_ABI impl__UnlockBuffer_CEditView__QEBAXXZ(const CEditView* pThis) { if (pThis) g_editViewExtraStates[pThis].locked=nullptr; }
// Symbol: ?DeleteContents@CEditView@@UEAAXXZ
extern "C" void MS_ABI impl__DeleteContents_CEditView__UEAAXXZ(CEditView* pThis) { SetEditViewText(pThis, L""); }
// Symbol: ?ReadFromArchive@CEditView@@QEAAXAEAVCArchive@@I@Z
extern "C" void MS_ABI impl__ReadFromArchive_CEditView__QEAAXAEAVCArchive__I_Z(CEditView* pThis, CArchive* ar, unsigned int) { if (!pThis||!ar) return; CString s; (*ar)>>s; SetEditViewText(pThis, static_cast<const wchar_t*>(s)); }
// Symbol: ?WriteToArchive@CEditView@@QEAAXAEAVCArchive@@@Z
extern "C" void MS_ABI impl__WriteToArchive_CEditView__QEAAXAEAVCArchive___Z(CEditView* pThis, CArchive* ar) { if (!pThis||!ar) return; CString s(EditViewText(pThis).c_str()); (*ar)<<s; }
// Symbol: ?SerializeRaw@CEditView@@QEAAXAEAVCArchive@@@Z
extern "C" void MS_ABI impl__SerializeRaw_CEditView__QEAAXAEAVCArchive___Z(CEditView* pThis, CArchive* ar) { if (!pThis||!ar) return; if (ar->IsStoring()) impl__WriteToArchive_CEditView__QEAAXAEAVCArchive___Z(pThis, ar); else impl__ReadFromArchive_CEditView__QEAAXAEAVCArchive__I_Z(pThis, ar, 0); }
// Symbol: ?SameAsSelected@CEditView@@IEAAHPEB_WH@Z
extern "C" int MS_ABI impl__SameAsSelected_CEditView__IEAAHPEB_WH_Z(CEditView* pThis, const wchar_t* text, int caseSensitive) { CString sel; pThis->GetSelectedText(sel); return caseSensitive ? wcscmp(sel, text?text:L"")==0 : _wcsicmp(sel, text?text:L"")==0; }
// Symbol: ?OnReplaceSel@CEditView@@MEAAXPEB_WHH0@Z
extern "C" void MS_ABI impl__OnReplaceSel_CEditView__MEAAXPEB_WHH0_Z(CEditView* pThis, const wchar_t* find, int next, int caseSensitive, const wchar_t* repl) { if (!pThis) return; int pos=impl__FindTextW_CEditView__QEAAHPEB_WHH_Z(pThis, find, next, caseSensitive); if (pos>=0 && pThis->m_hWnd) ::SendMessageW(pThis->m_hWnd, EM_REPLACESEL, TRUE, (LPARAM)(repl?repl:L"")); }
// Symbol: ?OnReplaceAll@CEditView@@MEAAXPEB_W0H@Z
extern "C" void MS_ABI impl__OnReplaceAll_CEditView__MEAAXPEB_W0H_Z(CEditView* pThis, const wchar_t* find, const wchar_t* repl, int caseSensitive) { if (!pThis||!find||!*find) return; std::wstring text=EditViewText(pThis), needle(find), replacement(repl?repl:L""); size_t pos=0; while ((pos=text.find(needle,pos))!=std::wstring::npos) { text.replace(pos,needle.size(),replacement); pos+=replacement.size(); } SetEditViewText(pThis,text); (void)caseSensitive; }
// Edit command handlers.
// Symbol: ?OnEditClear@CEditView@@IEAAXXZ
extern "C" void MS_ABI impl__OnEditClear_CEditView__IEAAXXZ(CEditView* pThis) { if (pThis&&pThis->m_hWnd) ::SendMessageW(pThis->m_hWnd, WM_CLEAR,0,0); }
// Symbol: ?OnEditCopy@CEditView@@IEAAXXZ
extern "C" void MS_ABI impl__OnEditCopy_CEditView__IEAAXXZ(CEditView* pThis) { if (pThis&&pThis->m_hWnd) ::SendMessageW(pThis->m_hWnd, WM_COPY,0,0); }
// Symbol: ?OnEditCut@CEditView@@IEAAXXZ
extern "C" void MS_ABI impl__OnEditCut_CEditView__IEAAXXZ(CEditView* pThis) { if (pThis&&pThis->m_hWnd) ::SendMessageW(pThis->m_hWnd, WM_CUT,0,0); }
// Symbol: ?OnEditPaste@CEditView@@IEAAXXZ
extern "C" void MS_ABI impl__OnEditPaste_CEditView__IEAAXXZ(CEditView* pThis) { if (pThis&&pThis->m_hWnd) ::SendMessageW(pThis->m_hWnd, WM_PASTE,0,0); }
// Symbol: ?OnEditUndo@CEditView@@IEAAXXZ
extern "C" void MS_ABI impl__OnEditUndo_CEditView__IEAAXXZ(CEditView* pThis) { if (pThis&&pThis->m_hWnd) ::SendMessageW(pThis->m_hWnd, EM_UNDO,0,0); }
// Symbol: ?OnEditSelectAll@CEditView@@IEAAXXZ
extern "C" void MS_ABI impl__OnEditSelectAll_CEditView__IEAAXXZ(CEditView* pThis) { if (pThis&&pThis->m_hWnd) ::SendMessageW(pThis->m_hWnd, EM_SETSEL,0,-1); }
// Symbol: ?OnEditChange@CEditView@@IEAAHXZ
extern "C" int MS_ABI impl__OnEditChange_CEditView__IEAAHXZ(CEditView* pThis) { if (pThis&&pThis->m_pDocument) pThis->m_pDocument->SetModifiedFlag(TRUE); if (pThis) g_editViewExtraStates[pThis].buffer=EditViewText(pThis); return TRUE; }
// Symbol: ?OnEditFind@CEditView@@IEAAXXZ
extern "C" void MS_ABI impl__OnEditFind_CEditView__IEAAXXZ(CEditView* pThis) { if (pThis) g_editViewExtraStates[pThis].findText.clear(); }
// Symbol: ?OnEditReplace@CEditView@@IEAAXXZ
extern "C" void MS_ABI impl__OnEditReplace_CEditView__IEAAXXZ(CEditView* pThis) { if (pThis) g_editViewExtraStates[pThis].replaceText.clear(); }
// Symbol: ?OnEditRepeat@CEditView@@IEAAXXZ
extern "C" void MS_ABI impl__OnEditRepeat_CEditView__IEAAXXZ(CEditView* pThis) { auto& s=g_editViewExtraStates[pThis]; if (!s.findText.empty()) impl__FindTextW_CEditView__QEAAHPEB_WHH_Z(pThis, s.findText.c_str(), TRUE, TRUE); }
// Symbol: ?OnEditFindReplace@CEditView@@IEAAXH@Z
extern "C" void MS_ABI impl__OnEditFindReplace_CEditView__IEAAXH_Z(CEditView* pThis, int replace) { if (pThis) { auto& s=g_editViewExtraStates[pThis]; if (replace) s.replaceText.clear(); else s.findText.clear(); } }
// Symbol: ?OnFindReplaceCmd@CEditView@@IEAA_J_K_J@Z
extern "C" intptr_t MS_ABI impl__OnFindReplaceCmd_CEditView__IEAA_J_K_J_Z(CEditView* pThis, uintptr_t, intptr_t) { if (!pThis) return 0; auto& s=g_editViewExtraStates[pThis]; return s.findText.empty() ? 0 : impl__FindTextW_CEditView__QEAAHPEB_WHH_Z(pThis, s.findText.c_str(), TRUE, TRUE) >= 0; }
// Symbol: ?InitializeReplace@CEditView@@IEAAHXZ
extern "C" int MS_ABI impl__InitializeReplace_CEditView__IEAAHXZ(CEditView* pThis) { if (pThis) { g_editViewExtraStates[pThis].findText.clear(); g_editViewExtraStates[pThis].replaceText.clear(); } return TRUE; }
// Symbol: ?OnFindNext@CEditView@@MEAAXPEB_WHH@Z
extern "C" void MS_ABI impl__OnFindNext_CEditView__MEAAXPEB_WHH_Z(CEditView* pThis, const wchar_t* find, int next, int caseSensitive) { if (pThis) { g_editViewExtraStates[pThis].findText=find?find:L""; impl__FindTextW_CEditView__QEAAHPEB_WHH_Z(pThis, find, next, caseSensitive); } }
// Symbol: ?OnTextNotFound@CEditView@@MEAAXPEB_W@Z
extern "C" void MS_ABI impl__OnTextNotFound_CEditView__MEAAXPEB_W_Z(CEditView*, const wchar_t*) { ::MessageBeep(MB_ICONINFORMATION); }
// Symbol: ?OnPreparePrinting@CEditView@@MEAAHPEAUCPrintInfo@@@Z
extern "C" int MS_ABI impl__OnPreparePrinting_CEditView__MEAAHPEAUCPrintInfo___Z(CEditView*, void*) { return TRUE; }
// Symbol: ?OnBeginPrinting@CEditView@@MEAAXPEAVCDC@@PEAUCPrintInfo@@@Z
extern "C" void MS_ABI impl__OnBeginPrinting_CEditView__MEAAXPEAVCDC__PEAUCPrintInfo___Z(CEditView* pThis, CDC* dc, void*) { if (pThis && dc && dc->m_hDC) { auto it = g_editViewExtraStates.find(pThis); CFont* font = it == g_editViewExtraStates.end() ? nullptr : it->second.printerFont; if (font) ::SelectObject(dc->m_hDC, font->GetSafeHandle()); } }
// Symbol: ?OnEndPrinting@CEditView@@MEAAXPEAVCDC@@PEAUCPrintInfo@@@Z
extern "C" void MS_ABI impl__OnEndPrinting_CEditView__MEAAXPEAVCDC__PEAUCPrintInfo___Z(CEditView* pThis, CDC*, void*) { if (pThis) g_editViewExtraStates[pThis].pageCount = 0; }
// Symbol: ?OnPrepareDC@CEditView@@UEAAXPEAVCDC@@PEAUCPrintInfo@@@Z
extern "C" void MS_ABI impl__OnPrepareDC_CEditView__UEAAXPEAVCDC__PEAUCPrintInfo___Z(CEditView* pThis, CDC* dc, void*) { if (pThis && dc && dc->m_hDC) { auto it = g_editViewExtraStates.find(pThis); CFont* font = it == g_editViewExtraStates.end() ? nullptr : it->second.printerFont; if (font) ::SelectObject(dc->m_hDC, font->GetSafeHandle()); } }
// Symbol: ?OnPrint@CEditView@@MEAAXPEAVCDC@@PEAUCPrintInfo@@@Z
extern "C" void MS_ABI impl__OnPrint_CEditView__MEAAXPEAVCDC__PEAUCPrintInfo___Z(CEditView* pThis, CDC* dc, void*) { if (!pThis||!dc||!dc->m_hDC) return; std::wstring t=EditViewText(pThis); RECT rc={0,0,10000,10000}; ::DrawTextW(dc->m_hDC,t.c_str(),-1,&rc,DT_LEFT|DT_TOP|DT_WORDBREAK); }
// Symbol: ?PaginateTo@CEditView@@IEAAHPEAVCDC@@PEAUCPrintInfo@@@Z
extern "C" int MS_ABI impl__PaginateTo_CEditView__IEAAHPEAVCDC__PEAUCPrintInfo___Z(CEditView* pThis, CDC*, void*) { if (pThis) g_editViewExtraStates[pThis].pageCount=1; return TRUE; }
// Symbol: ?SetPrinterFont@CEditView@@QEAAXPEAVCFont@@@Z
extern "C" void MS_ABI impl__SetPrinterFont_CEditView__QEAAXPEAVCFont___Z(CEditView* pThis, CFont* font) { if (pThis) g_editViewExtraStates[pThis].printerFont=font; }
// Symbol: ?GetPrinterFont@CEditView@@QEBAPEAVCFont@@XZ
extern "C" CFont* MS_ABI impl__GetPrinterFont_CEditView__QEBAPEAVCFont__XZ(const CEditView* pThis) { auto it=g_editViewExtraStates.find(pThis); return it==g_editViewExtraStates.end()?nullptr:it->second.printerFont; }
// Symbol: ?SetTabStops@CEditView@@QEAAXH@Z
extern "C" void MS_ABI impl__SetTabStops_CEditView__QEAAXH_Z(CEditView* pThis, int tabs) { if (pThis&&pThis->m_hWnd) ::SendMessageW(pThis->m_hWnd, EM_SETTABSTOPS,1,(LPARAM)&tabs); }
// Symbol: ?OnCreate@CEditView@@IEAAHPEAUtagCREATESTRUCTW@@@Z
extern "C" int MS_ABI impl__OnCreate_CEditView__IEAAHPEAUtagCREATESTRUCTW___Z(CEditView* pThis, CREATESTRUCTW*) { if (!pThis||pThis->m_hWnd) return 0; pThis->m_hWnd=::CreateWindowExW(0,L"EDIT",L"",impl__dwStyleDefault_CEditView__2KB,0,0,0,0,nullptr,nullptr,AfxGetInstanceHandle(),nullptr); return pThis->m_hWnd?0:-1; }
// Symbol: ?OnDestroy@CEditView@@IEAAXXZ
extern "C" void MS_ABI impl__OnDestroy_CEditView__IEAAXXZ(CEditView* pThis) { if (pThis) g_editViewExtraStates.erase(pThis); }
// Symbol: ?CalcWindowRect@CEditView@@MEAAXPEAUtagRECT@@I@Z
extern "C" void MS_ABI impl__CalcWindowRect_CEditView__MEAAXPEAUtagRECT__I_Z(CEditView* pThis, RECT* rect, unsigned int adjustType) { impl__CalcWindowRect_CView__UEAAXPEAUtagRECT__I_Z(pThis, rect, adjustType); }
// Symbol: ?PreCreateWindow@CEditView@@MEAAHAEAUtagCREATESTRUCTW@@@Z
extern "C" int MS_ABI impl__PreCreateWindow_CEditView__MEAAHAEAUtagCREATESTRUCTW___Z(CEditView*, CREATESTRUCTW* cs) { if (cs) cs->style |= impl__dwStyleDefault_CEditView__2KB; return TRUE; }
// Symbol: ?OnSetFont@CEditView@@IEAAXPEAVCFont@@H@Z
extern "C" void MS_ABI impl__OnSetFont_CEditView__IEAAXPEAVCFont__H_Z(CEditView* pThis, CFont* font, int redraw) { if (pThis&&pThis->m_hWnd) ::SendMessageW(pThis->m_hWnd, WM_SETFONT, (WPARAM)(font?font->GetSafeHandle():nullptr), redraw); }
// Symbol: ?OnUpdateEditUndo@CEditView@@IEAAXPEAVCCmdUI@@@Z
extern "C" void MS_ABI impl__OnUpdateEditUndo_CEditView__IEAAXPEAVCCmdUI___Z(CEditView* pThis, CCmdUI* ui) { if (ui) impl__Enable_CCmdUI__UEAAXH_Z(ui, pThis && pThis->m_hWnd && ::SendMessageW(pThis->m_hWnd, EM_CANUNDO, 0, 0)); }
// Symbol: ?OnUpdateNeedClip@CEditView@@IEAAXPEAVCCmdUI@@@Z
extern "C" void MS_ABI impl__OnUpdateNeedClip_CEditView__IEAAXPEAVCCmdUI___Z(CEditView*, CCmdUI* ui) { if (ui) impl__Enable_CCmdUI__UEAAXH_Z(ui, ::IsClipboardFormatAvailable(CF_UNICODETEXT)); }
// Symbol: ?OnUpdateNeedFind@CEditView@@IEAAXPEAVCCmdUI@@@Z
extern "C" void MS_ABI impl__OnUpdateNeedFind_CEditView__IEAAXPEAVCCmdUI___Z(CEditView* pThis, CCmdUI* ui) { if (ui) impl__Enable_CCmdUI__UEAAXH_Z(ui, pThis && !EditViewText(pThis).empty()); }
// Symbol: ?OnUpdateNeedSel@CEditView@@IEAAXPEAVCCmdUI@@@Z
extern "C" void MS_ABI impl__OnUpdateNeedSel_CEditView__IEAAXPEAVCCmdUI___Z(CEditView* pThis, CCmdUI* ui) { DWORD s=0,e=0; if (pThis&&pThis->m_hWnd) ::SendMessageW(pThis->m_hWnd, EM_GETSEL, (WPARAM)&s, (LPARAM)&e); if (ui) impl__Enable_CCmdUI__UEAAXH_Z(ui, s != e); }
// Symbol: ?OnUpdateNeedText@CEditView@@IEAAXPEAVCCmdUI@@@Z
extern "C" void MS_ABI impl__OnUpdateNeedText_CEditView__IEAAXPEAVCCmdUI___Z(CEditView* pThis, CCmdUI* ui) { if (ui) impl__Enable_CCmdUI__UEAAXH_Z(ui, pThis && !EditViewText(pThis).empty()); }

// CListView/CTreeView residuals.
// Symbol: ?DrawItem@CListView@@UEAAXPEAUtagDRAWITEMSTRUCT@@@Z
extern "C" void MS_ABI impl__DrawItem_CListView__UEAAXPEAUtagDRAWITEMSTRUCT___Z(CListView*, DRAWITEMSTRUCT*) {}
// Symbol: ?OnChildNotify@CListView@@MEAAHI_K_JPEA_J@Z
extern "C" int MS_ABI impl__OnChildNotify_CListView__MEAAHI_K_JPEA_J_Z(CListView*, unsigned int, unsigned long long, long long, long long*) { return FALSE; }
// Symbol: ?OnNcDestroy@CListView@@QEAAXXZ
extern "C" void MS_ABI impl__OnNcDestroy_CListView__QEAAXXZ(CListView* pThis) { if (pThis) pThis->m_pListCtrl=nullptr; }
// Symbol: ?PreCreateWindow@CListView@@UEAAHAEAUtagCREATESTRUCTW@@@Z
extern "C" int MS_ABI impl__PreCreateWindow_CListView__UEAAHAEAUtagCREATESTRUCTW___Z(CListView*, CREATESTRUCTW* cs) { if (cs) cs->lpszClass = WC_LISTVIEWW; return TRUE; }
// Symbol: ?RemoveImageList@CListView@@IEAAXH@Z
extern "C" void MS_ABI impl__RemoveImageList_CListView__IEAAXH_Z(CListView* pThis, int type) { if (pThis&&pThis->m_hWnd) ListView_SetImageList(pThis->m_hWnd, nullptr, type); }
// Symbol: ?OnDestroy@CTreeView@@QEAAXXZ
extern "C" void MS_ABI impl__OnDestroy_CTreeView__QEAAXXZ(CTreeView* pThis) { if (pThis) pThis->m_pTreeCtrl=nullptr; }
// Symbol: ?PreCreateWindow@CTreeView@@UEAAHAEAUtagCREATESTRUCTW@@@Z
extern "C" int MS_ABI impl__PreCreateWindow_CTreeView__UEAAHAEAUtagCREATESTRUCTW___Z(CTreeView*, CREATESTRUCTW* cs) { if (cs) cs->lpszClass = WC_TREEVIEWW; return TRUE; }
// Symbol: ?RemoveImageList@CTreeView@@IEAAXH@Z
extern "C" void MS_ABI impl__RemoveImageList_CTreeView__IEAAXH_Z(CTreeView* pThis, int type) { if (pThis&&pThis->m_hWnd) TreeView_SetImageList(pThis->m_hWnd, nullptr, type); }
