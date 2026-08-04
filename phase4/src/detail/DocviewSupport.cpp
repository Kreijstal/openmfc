#include "DocviewSupport.h"

// Implementations this unit calls that are defined with their own class.
extern "C" void MS_ABI impl__OnDrawThumbnail_CDocument__UEAAXAEAVCDC__PEAUtagRECT___Z(CDocument*, CDC* dc, RECT* rect);

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
extern "C" void MS_ABI impl__SetModifiedFlag_CDocument__UEAAXH_Z(
    CDocument* pThis, int bModified)
{
    if (pThis) {
        pThis->m_bModified = bModified;
    }
}
extern "C" int MS_ABI impl__IsModified_CDocument__UEBAHXZ(const CDocument* pThis) {
    return pThis ? pThis->m_bModified : FALSE;
}
extern "C" const wchar_t* MS_ABI impl__GetTitle_CDocument__UEBAPEB_WXZ(
    const CDocument* pThis)
{
    return pThis ? (const wchar_t*)pThis->m_strTitle : L"";
}
extern "C" const wchar_t* MS_ABI impl__GetPathName_CDocument__UEBAPEB_WXZ(
    const CDocument* pThis)
{
    return pThis ? (const wchar_t*)pThis->m_strPathName : L"";
}
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
extern "C" void* MS_ABI impl__GetFirstViewPosition_CDocument__UEBAPEAXXZ(
    const CDocument* pThis)
{
    return pThis ? pThis->m_pFirstView : nullptr;
}
extern "C" CView* MS_ABI impl__GetNextView_CDocument__UEBAPEAVCView__AEAPEAX_Z(
    const CDocument* pThis, void*& pos)
{
    if (!pThis || !pos) return nullptr;

    CView* pView = static_cast<CView*>(pos);
    pos = pView->m_pNextView;
    return pView;
}
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
extern "C" void MS_ABI impl___0CView__QEAA_XZ(CView* pThis) {
    if (!pThis) return;
    pThis->m_hWnd = nullptr;
    pThis->m_pDocument = nullptr;
    pThis->m_pNextView = nullptr;
}
extern "C" CDocument* MS_ABI impl__GetDocument_CView__UEBAPEAVCDocument__XZ(
    const CView* pThis)
{
    return pThis ? pThis->m_pDocument : nullptr;
}
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
extern "C" int MS_ABI impl__OnPreparePrinting_CView__UEAAHPEAX_Z(
    CView* pThis, void* pInfo)
{
    if (!pThis || !pInfo) {
        return FALSE;
    }
    (void)pThis;
    (void)pInfo;
    return TRUE;
}
extern "C" void MS_ABI impl__OnBeginPrinting_CView__UEAAXPEAX0_Z(
    CView* pThis, void* pDC, void* pInfo)
{
    (void)pThis;
    (void)pDC;
    (void)pInfo;
}
extern "C" void MS_ABI impl__OnEndPrinting_CView__UEAAXPEAX0_Z(
    CView* pThis, void* pDC, void* pInfo)
{
    (void)pThis;
    (void)pDC;
    (void)pInfo;
}
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
extern "C" CFrameWnd* MS_ABI impl__GetParentFrame_CView__UEBAPEAVCFrameWnd__XZ(
    const CView* pThis)
{
    if (!pThis) return nullptr;
    return const_cast<CView*>(pThis)->CWnd::GetParentFrame();
}
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
extern "C" void MS_ABI impl__SetScrollSizes_CScrollView__QEAAXHUtagSIZE__AEBU2_1_Z(
    CScrollView* pThis, int nMapMode, SIZE sizeTotal, const SIZE& sizePage, const SIZE& sizeLine)
{
    if (!pThis) return;

    impl__SetScrollSizes_CScrollView__UEAAXHAEBUtagSIZE__00_Z(
        pThis, nMapMode, &sizeTotal, &sizePage, &sizeLine);
}
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
extern "C" void MS_ABI impl__OnDraw_CScrollView__UEAAXPEAX_Z(
    CScrollView* pThis, void* pDC)
{
    (void)pThis;
    (void)pDC;
    // Default: nothing to draw
}
extern "C" void MS_ABI impl___0CFormView__QEAA_XZ(CFormView* pThis) {
    if (!pThis) return;
    impl___0CScrollView__QEAA_XZ(pThis);
    pThis->m_lpszTemplateName = nullptr;
    pThis->m_nIDHelp = 0;
}
extern "C" void MS_ABI impl__OnDraw_CFormView__UEAAXPEAX_Z(
    CFormView* pThis, void* pDC)
{
    (void)pThis;
    (void)pDC;
    // Form views are drawn by their child controls
}
extern "C" void MS_ABI impl__DoDataExchange_CFormView__UEAAXPEAX_Z(
    CFormView* pThis, void* pDX)
{
    (void)pThis;
    (void)pDX;
    // Override in derived class to transfer data
}
extern "C" CEdit* MS_ABI impl__GetEditCtrl_CEditView__QEBAPEAVCEdit__XZ(
    const CEditView* pThis)
{
    return pThis ? pThis->m_pEditCtrl : nullptr;
}
extern "C" void MS_ABI impl__OnDraw_CEditView__UEAAXPEAX_Z(
    CEditView* pThis, void* pDC)
{
    (void)pThis;
    (void)pDC;
    // Edit control draws itself
}
extern "C" void MS_ABI impl___0CListView__QEAA_XZ(CListView* pThis) {
    if (!pThis) return;
    impl___0CView__QEAA_XZ(pThis);
    pThis->m_pListCtrl = nullptr;
}
extern "C" void MS_ABI impl___1CListView__UEAA_XZ(CListView* pThis) {
    impl___1CView__UEAA_XZ(pThis);
}
extern "C" CListCtrl* MS_ABI impl__GetListCtrl_CListView__QEBAPEAVCListCtrl__XZ(
    const CListView* pThis)
{
    return pThis ? pThis->m_pListCtrl : nullptr;
}
extern "C" void MS_ABI impl__OnDraw_CListView__UEAAXPEAX_Z(
    CListView* pThis, void* pDC)
{
    (void)pThis;
    (void)pDC;
    // List control draws itself
}
extern "C" void MS_ABI impl__OnInitialUpdate_CListView__UEAAXXZ(CListView* pThis) {
    impl__OnInitialUpdate_CView__UEAAXXZ(pThis);
}
extern "C" void MS_ABI impl___0CTreeView__QEAA_XZ(CTreeView* pThis) {
    if (!pThis) return;
    impl___0CView__QEAA_XZ(pThis);
    pThis->m_pTreeCtrl = nullptr;
}
extern "C" void MS_ABI impl___1CTreeView__UEAA_XZ(CTreeView* pThis) {
    impl___1CView__UEAA_XZ(pThis);
}
extern "C" CTreeCtrl* MS_ABI impl__GetTreeCtrl_CTreeView__QEBAPEAVCTreeCtrl__XZ(
    const CTreeView* pThis)
{
    return pThis ? pThis->m_pTreeCtrl : nullptr;
}
extern "C" void MS_ABI impl__OnDraw_CTreeView__UEAAXPEAX_Z(
    CTreeView* pThis, void* pDC)
{
    (void)pThis;
    (void)pDC;
    // Tree control draws itself
}
extern "C" void MS_ABI impl__OnInitialUpdate_CTreeView__UEAAXXZ(CTreeView* pThis) {
    impl__OnInitialUpdate_CView__UEAAXXZ(pThis);
}
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
extern "C" void* MS_ABI impl__GetFirstDocPosition_CDocTemplate__UEBAPEAXXZ(
    const CDocTemplate* pThis)
{
    return pThis ? pThis->m_pFirstDoc : nullptr;
}
extern "C" CDocument* MS_ABI impl__GetNextDoc_CDocTemplate__UEBAPEAVCDocument__AEAPEAX_Z(
    const CDocTemplate* pThis, void*& pos)
{
    if (!pThis || !pos) return nullptr;

    CDocument* pDoc = static_cast<CDocument*>(pos);
    pos = pDoc->m_pNextDoc;
    return pDoc;
}
extern "C" void MS_ABI impl__SetDefaultTitle_CDocTemplate__UEAAXPEAVCDocument___Z(
    CDocTemplate* pThis, CDocument* pDoc)
{
    (void)pThis;
    if (pDoc) {
        pDoc->SetTitle(L"Untitled");
    }
}
extern "C" int MS_ABI impl__GetDocString_CDocTemplate__UEBAHAEAVCString__H_Z(
    const CDocTemplate* pThis, CString* rString, int nID)
{
    if (!pThis || !rString) return FALSE;

    if (LoadTemplateDocString(pThis->m_nIDResource, nID, *rString)) {
        return TRUE;
    }

    switch (nID) {
        case kWindowTitle:
            *rString = L"OpenMFC Document";
            return TRUE;
        case kDocName:
        case kFileNewName:
            *rString = L"Document";
            return TRUE;
        case kFilterName:
            *rString = L"Document Files";
            return TRUE;
        case kFilterExt:
            *rString = L"*.*";
            return TRUE;
        case kRegFileTypeId:
            *rString = L"OpenMFC.Document";
            return TRUE;
        case kRegFileTypeName:
            *rString = L"OpenMFC Document";
            return TRUE;
        default:
            rString->Empty();
            return FALSE;
    }
}
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
extern "C" void MS_ABI impl___0CMultiDocTemplate__QEAA_XZ(CMultiDocTemplate* pThis) {
    if (!pThis) return;
    pThis->m_nIDResource = 0;
    pThis->m_pDocClass = nullptr;
    pThis->m_pFrameClass = nullptr;
    pThis->m_pViewClass = nullptr;
    pThis->m_pFirstDoc = nullptr;
    pThis->m_pLastDoc = nullptr;
}
namespace openmfc { namespace detail { namespace docview {
bool LoadTemplateDocString(UINT resourceId, int index, CString& out) {
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
std::wstring NormalizePathForCompare(const wchar_t* path) {
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
bool PathsEqualInsensitive(const wchar_t* lhs, const wchar_t* rhs) {
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
bool PathHasExtensionInsensitive(const wchar_t* path, const wchar_t* expectedExt) {
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
extern "C" void MS_ABI impl___0CFormView__QEAA_PEB_W_Z(
    CFormView* pThis, const wchar_t* lpszTemplateName)
{
    if (!pThis) return;
    impl___0CScrollView__QEAA_XZ(pThis);
    pThis->m_lpszTemplateName = lpszTemplateName;
    pThis->m_nIDHelp = 0;
}
extern "C" void MS_ABI impl___0CFormView__QEAA_I_Z(
    CFormView* pThis, unsigned int nIDTemplate)
{
    if (!pThis) return;
    impl___0CScrollView__QEAA_XZ(pThis);
    pThis->m_lpszTemplateName = MAKEINTRESOURCEW(nIDTemplate);
    pThis->m_nIDHelp = nIDTemplate;
}
const AFX_MSGMAP_ENTRY g_docviewEmptyEntries[] = {
    {0, 0, 0, 0, AfxSig_end, (AFX_PMSG)0}
};
const AFX_MSGMAP g_docviewEmptyMap = { nullptr, g_docviewEmptyEntries };
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
} } }  // namespace openmfc::detail::docview
