// OLE/COM Implementation
// Provides MFC wrappers around the OLE32/OLEAUT32 COM APIs
//
// All exports use MSVC ABI via extern "C" + .def aliasing.

#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxole.h"
#include <cstring>
#include <cstdio>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

//=============================================================================
// Base classes needed by OLE
//=============================================================================
IMPLEMENT_DYNAMIC(CControlBar, CWnd)
IMPLEMENT_DYNAMIC(CDocItem, CObject)

void CCmdUI::Enable(BOOL bOn) { (void)bOn; }
void CCmdUI::SetCheck(int nCheck) { (void)nCheck; }
void CCmdUI::SetText(const wchar_t* lpszText) { (void)lpszText; }

CControlBar::CControlBar() : m_nCount(0), m_pDockSite(nullptr), m_pDockBar(nullptr),
    m_pDockContext(nullptr), m_dwStyle(0) {
    memset(_controlbar_padding, 0, sizeof(_controlbar_padding));
}

CControlBar::~CControlBar() {}
BOOL CControlBar::Create(CWnd* pParentWnd, DWORD dwStyle, UINT nID) { (void)pParentWnd; (void)dwStyle; (void)nID; return FALSE; }

CDocItem::CDocItem() : m_pDocument(nullptr) { memset(_docitem_padding, 0, sizeof(_docitem_padding)); }
CDocItem::~CDocItem() {}

//=============================================================================
// OLE State
//=============================================================================
static int g_bOleInitialized = FALSE;
static int g_nOleLockCount = 0;
static COleMessageFilter* g_pMessageFilter = nullptr;

//=============================================================================
// OLE Initialization / Termination
//=============================================================================
extern "C" {

MS_ABI void impl__AfxOleInit() {
    if (!g_bOleInitialized) {
        HRESULT hr = OleInitialize(nullptr);
        if (SUCCEEDED(hr)) {
            g_bOleInitialized = TRUE;
            // Create default message filter
            COleMessageFilter* pFilter = new COleMessageFilter();
            g_pMessageFilter = pFilter;
        }
    }
}

MS_ABI void impl__AfxOleTerm(int bExplicit) {
    if (g_bOleInitialized) {
        if (g_pMessageFilter) {
            delete g_pMessageFilter;
            g_pMessageFilter = nullptr;
        }
        if (bExplicit) {
            OleUninitialize();
        }
        g_bOleInitialized = FALSE;
    }
}

MS_ABI void impl__AfxOleLockApp() {
    g_nOleLockCount++;
    CoAddRefServerProcess();
}

MS_ABI void impl__AfxOleUnlockApp() {
    if (g_nOleLockCount > 0) {
        g_nOleLockCount--;
        CoReleaseServerProcess();
    }
}

MS_ABI int impl__AfxOleCanExitApp() {
    return (g_nOleLockCount == 0) ? TRUE : FALSE;
}

MS_ABI int impl__AfxOleGetMessageFilter() {
    return (g_pMessageFilter != nullptr) ? TRUE : FALSE;
}

} // extern "C"

//=============================================================================
// COleCurrency
//=============================================================================
COleCurrency::COleCurrency(long nUnits, long nFractionalUnits) {
    m_cur.int64 = ((long long)nUnits * 10000) + nFractionalUnits;
    m_status = CY_VALID;
}

COleCurrency::COleCurrency(const CY& cySrc) {
    m_cur = cySrc;
    m_status = CY_VALID;
}

COleCurrency::COleCurrency(const COleCurrency& curSrc) {
    m_cur = curSrc.m_cur;
    m_status = curSrc.m_status;
}

COleCurrency& COleCurrency::operator=(const COleCurrency& curSrc) {
    m_cur = curSrc.m_cur;
    m_status = curSrc.m_status;
    return *this;
}

void COleCurrency::SetCurrency(long nUnits, long nFractionalUnits) {
    m_cur.int64 = ((long long)nUnits * 10000) + nFractionalUnits;
    m_status = CY_VALID;
}

//=============================================================================
// COleDateTime
//=============================================================================
COleDateTime COleDateTime::GetCurrentTime() {
    COleDateTime dt;
    SYSTEMTIME st;
    GetSystemTime(&st);
    SystemTimeToVariantTime(&st, &dt.m_dt);
    dt.m_status = DT_VALID;
    return dt;
}

COleDateTime& COleDateTime::operator=(const COleDateTime& dtSrc) {
    m_dt = dtSrc.m_dt;
    m_status = dtSrc.m_status;
    return *this;
}

//=============================================================================
// COleSafeArray
//=============================================================================
void COleSafeArray::Destroy() {
    if (pvData) {
        SafeArrayDestroy(this);
        cbElements = 0;
        cDims = 0;
        pvData = nullptr;
    }
}

void COleSafeArray::Clear() {
    SafeArrayDestroyData(this);
}

void COleSafeArray::Create(VARTYPE vt, DWORD cDims, SAFEARRAYBOUND* rgsabound) {
    Destroy();
    SAFEARRAY* psa = SafeArrayCreate(vt, cDims, rgsabound);
    if (psa) {
        memcpy(this, psa, sizeof(SAFEARRAY));
    }
}

void COleSafeArray::AccessData(void** ppvData) {
    SafeArrayAccessData(this, ppvData);
}

void COleSafeArray::UnaccessData() {
    SafeArrayUnaccessData(this);
}

void COleSafeArray::Attach(const SAFEARRAY& saSrc) {
    Destroy();
    memcpy(this, &saSrc, sizeof(SAFEARRAY));
}

SAFEARRAY* COleSafeArray::Detach() {
    SAFEARRAY* psa = (SAFEARRAY*)malloc(sizeof(SAFEARRAY));
    if (psa) {
        memcpy(psa, this, sizeof(SAFEARRAY));
    }
    pvData = nullptr;
    cbElements = 0;
    cDims = 0;
    return psa;
}

void COleSafeArray::Copy(const COleSafeArray* psaSrc) {
    Destroy();
    if (psaSrc && psaSrc->pvData) {
        SAFEARRAY* psaNew = nullptr;
        SafeArrayCopy((SAFEARRAY*)psaSrc, &psaNew);
        if (psaNew) {
            memcpy(this, psaNew, sizeof(SAFEARRAY));
            SafeArrayDestroy(psaNew);
        }
    }
}

//=============================================================================
// COleDataObject
//=============================================================================
COleDataObject::COleDataObject()
    : m_lpDataObject(nullptr), m_lpEnumFmtEtc(nullptr), m_bAutoRelease(TRUE) {
    memset(_oledataobject_padding, 0, sizeof(_oledataobject_padding));
}

COleDataObject::~COleDataObject() {
    Release();
}

void COleDataObject::Attach(LPDATAOBJECT lpDataObject, BOOL bAutoRelease) {
    Release();
    m_lpDataObject = lpDataObject;
    m_bAutoRelease = bAutoRelease;
}

LPDATAOBJECT COleDataObject::Detach() {
    LPDATAOBJECT lp = m_lpDataObject;
    m_lpDataObject = nullptr;
    return lp;
}

void COleDataObject::Release() {
    if (m_lpEnumFmtEtc) {
        m_lpEnumFmtEtc->Release();
        m_lpEnumFmtEtc = nullptr;
    }
    if (m_lpDataObject && m_bAutoRelease) {
        m_lpDataObject->Release();
    }
    m_lpDataObject = nullptr;
}

void COleDataObject::BeginEnumFormats() {
    if (m_lpEnumFmtEtc) {
        m_lpEnumFmtEtc->Release();
        m_lpEnumFmtEtc = nullptr;
    }
    if (m_lpDataObject) {
        m_lpDataObject->EnumFormatEtc(DATADIR_GET, &m_lpEnumFmtEtc);
    }
}

BOOL COleDataObject::GetNextFormat(FORMATETC* lpFormatEtc) {
    if (!m_lpEnumFmtEtc) return FALSE;
    return m_lpEnumFmtEtc->Next(1, lpFormatEtc, nullptr) == S_OK;
}

BOOL COleDataObject::IsDataAvailable(CLIPFORMAT cfFormat, FORMATETC* lpFormatEtc) {
    if (!m_lpDataObject) return FALSE;
    FORMATETC fmt;
    if (!lpFormatEtc) {
        fmt.cfFormat = cfFormat;
        fmt.ptd = nullptr;
        fmt.dwAspect = DVASPECT_CONTENT;
        fmt.lindex = -1;
        fmt.tymed = TYMED_HGLOBAL;
        lpFormatEtc = &fmt;
    }
    return m_lpDataObject->QueryGetData(lpFormatEtc) == S_OK;
}

HGLOBAL COleDataObject::GetGlobalData(CLIPFORMAT cfFormat, FORMATETC* lpFormatEtc) {
    if (!m_lpDataObject) return nullptr;
    FORMATETC fmt;
    if (!lpFormatEtc) {
        fmt.cfFormat = cfFormat;
        fmt.ptd = nullptr;
        fmt.dwAspect = DVASPECT_CONTENT;
        fmt.lindex = -1;
        fmt.tymed = TYMED_HGLOBAL;
        lpFormatEtc = &fmt;
    }
    STGMEDIUM stg = {};
    if (m_lpDataObject->GetData(lpFormatEtc, &stg) == S_OK) {
        return stg.hGlobal;
    }
    return nullptr;
}

BOOL COleDataObject::GetData(CLIPFORMAT cfFormat, STGMEDIUM* lpStorageMedium, FORMATETC* lpFormatEtc) {
    if (!m_lpDataObject) return FALSE;
    FORMATETC fmt;
    if (!lpFormatEtc) {
        fmt.cfFormat = cfFormat;
        fmt.ptd = nullptr;
        fmt.dwAspect = DVASPECT_CONTENT;
        fmt.lindex = -1;
        fmt.tymed = TYMED_HGLOBAL;
        lpFormatEtc = &fmt;
    }
    return m_lpDataObject->GetData(lpFormatEtc, lpStorageMedium) == S_OK;
}

LPDATAOBJECT COleDataObject::GetIDataObject(BOOL bAddRef) const {
    if (m_lpDataObject && bAddRef) {
        m_lpDataObject->AddRef();
    }
    return m_lpDataObject;
}

//=============================================================================
// COleDataSource
//=============================================================================
IMPLEMENT_DYNAMIC(COleDataSource, CCmdTarget)

COleDataSource::COleDataSource()
    : m_bClipboardOwner(FALSE), m_pDataCache(nullptr) {
    memset(_oledatasource_padding, 0, sizeof(_oledatasource_padding));
}

COleDataSource::~COleDataSource() {
    Empty();
}

void COleDataSource::CacheGlobalData(CLIPFORMAT cfFormat, HGLOBAL hGlobal, FORMATETC* lpFormatEtc) {
    (void)lpFormatEtc;
    // Stub: store in cache
    m_pDataCache = hGlobal;
}

void COleDataSource::CacheData(CLIPFORMAT cfFormat, STGMEDIUM* lpStorageMedium, FORMATETC* lpFormatEtc) {
    (void)cfFormat; (void)lpStorageMedium; (void)lpFormatEtc;
}

void COleDataSource::DelayRenderData(CLIPFORMAT cfFormat, FORMATETC* lpFormatEtc) {
    (void)cfFormat; (void)lpFormatEtc;
}

void COleDataSource::DelayRenderFileData(CLIPFORMAT cfFormat, FORMATETC* lpFormatEtc) {
    (void)cfFormat; (void)lpFormatEtc;
}

void COleDataSource::SetClipboard() {
    if (m_pDataCache) {
        m_bClipboardOwner = TRUE;
    }
}

COleDataSource* COleDataSource::GetClipboardOwner() {
    return nullptr; // Stub
}

DROPEFFECT COleDataSource::DoDragDrop(DWORD dwEffects, LPCRECT lpRectStartDrag,
                                       COleDropSource* pDropSource) {
    (void)lpRectStartDrag; (void)pDropSource;
    return DROPEFFECT_NONE; // Stub
}

void COleDataSource::OnRenderGlobalData(FORMATETC* lpFormatEtc, HGLOBAL* phGlobal) {
    (void)lpFormatEtc; (void)phGlobal;
}

void COleDataSource::OnRenderFileData(FORMATETC* lpFormatEtc, CFile* pFile) {
    (void)lpFormatEtc; (void)pFile;
}

HGLOBAL COleDataSource::OnRenderGlobalData(LPFORMATETC lpFormatEtc) {
    (void)lpFormatEtc;
    return nullptr;
}

HGLOBAL COleDataSource::OnRenderFileData(LPFORMATETC lpFormatEtc) {
    (void)lpFormatEtc;
    return nullptr;
}

void COleDataSource::Empty() {
    if (m_pDataCache) {
        GlobalFree((HGLOBAL)m_pDataCache);
        m_pDataCache = nullptr;
    }
}

//=============================================================================
// COleDropTarget
//=============================================================================
IMPLEMENT_DYNAMIC(COleDropTarget, CCmdTarget)

COleDropTarget::COleDropTarget()
    : m_lRefCount(0), m_pWnd(nullptr), m_bRegistered(FALSE) {
    memset(_oledroptarget_padding, 0, sizeof(_oledroptarget_padding));
}

COleDropTarget::~COleDropTarget() {
    Revoke();
}

BOOL COleDropTarget::Register(CWnd* pWnd) {
    if (m_bRegistered) return TRUE;
    if (!pWnd || !pWnd->GetSafeHwnd()) return FALSE;
    m_pWnd = pWnd;
    HRESULT hr = RegisterDragDrop(pWnd->GetSafeHwnd(), (IDropTarget*)this);
    if (SUCCEEDED(hr)) {
        m_bRegistered = TRUE;
        return TRUE;
    }
    return FALSE;
}

void COleDropTarget::Revoke() {
    if (m_bRegistered && m_pWnd) {
        RevokeDragDrop(m_pWnd->GetSafeHwnd());
        m_bRegistered = FALSE;
    }
    m_pWnd = nullptr;
}

DROPEFFECT COleDropTarget::OnDragEnter(CWnd* pWnd, COleDataObject* pDataObject,
                                        DWORD dwKeyState, CPoint point) {
    return DROPEFFECT_NONE;
}

DROPEFFECT COleDropTarget::OnDragOver(CWnd* pWnd, COleDataObject* pDataObject,
                                       DWORD dwKeyState, CPoint point) {
    return DROPEFFECT_NONE;
}

void COleDropTarget::OnDragLeave(CWnd* pWnd) {
}

BOOL COleDropTarget::OnDrop(CWnd* pWnd, COleDataObject* pDataObject,
                             DROPEFFECT dropEffect, CPoint point) {
    return FALSE;
}

DROPEFFECT COleDropTarget::OnDropEx(CWnd* pWnd, COleDataObject* pDataObject,
                                     DROPEFFECT dropDefault, DROPEFFECT dropList, CPoint point) {
    return OnDrop(pWnd, pDataObject, dropDefault, point);
}

DROPEFFECT COleDropTarget::OnDragScroll(CWnd* pWnd, DWORD dwKeyState, CPoint point) {
    return DROPEFFECT_SCROLL;
}

//=============================================================================
// COleDropSource
//=============================================================================
IMPLEMENT_DYNAMIC(COleDropSource, CCmdTarget)

COleDropSource::COleDropSource()
    : m_lRefCount(0) {
    memset(_oledropsource_padding, 0, sizeof(_oledropsource_padding));
}

COleDropSource::~COleDropSource() {
}

SCODE COleDropSource::QueryContinueDrag(BOOL bEscapePressed, DWORD dwKeyState) {
    if (bEscapePressed) return DRAGDROP_S_CANCEL;
    if (!(dwKeyState & (MK_LBUTTON | MK_RBUTTON))) return DRAGDROP_S_DROP;
    return S_OK;
}

SCODE COleDropSource::GiveFeedback(DROPEFFECT dropEffect) {
    (void)dropEffect;
    return DRAGDROP_S_USEDEFAULTCURSORS;
}

//=============================================================================
// COleMessageFilter
//=============================================================================
IMPLEMENT_DYNAMIC(COleMessageFilter, CCmdTarget)

COleMessageFilter::COleMessageFilter()
    : m_nBusyCount(0), m_bEnableBusy(TRUE), m_bEnableNotResponding(TRUE),
      m_nBusyReply(SERVERCALL_RETRYLATER), m_nRetryReply(0), m_nTimeout(5000) {
    memset(_olemessagefilter_padding, 0, sizeof(_olemessagefilter_padding));
}

COleMessageFilter::~COleMessageFilter() {
}

COleMessageFilter* COleMessageFilter::Register() {
    COleMessageFilter* pFilter = new COleMessageFilter();
    CoRegisterMessageFilter((IMessageFilter*)pFilter, nullptr);
    g_pMessageFilter = pFilter;
    return pFilter;
}

void COleMessageFilter::Revoke() {
    CoRegisterMessageFilter(nullptr, nullptr);
}

BOOL COleMessageFilter::EnableBusyDialog(BOOL bEnableBusy) {
    BOOL bOld = m_bEnableBusy;
    m_bEnableBusy = bEnableBusy;
    return bOld;
}

BOOL COleMessageFilter::EnableNotRespondingDialog(BOOL bEnableNotResponding) {
    BOOL bOld = m_bEnableNotResponding;
    m_bEnableNotResponding = bEnableNotResponding;
    return bOld;
}

void COleMessageFilter::SetBusyReply(SERVERCALL nBusyReply) {
    m_nBusyReply = nBusyReply;
}

void COleMessageFilter::SetRetryReply(DWORD nRetryReply) {
    m_nRetryReply = nRetryReply;
}

void COleMessageFilter::SetMessagePendingDelay(DWORD nTimeout) {
    m_nTimeout = (nTimeout >= 0) ? nTimeout : 5000;
}

void COleMessageFilter::BeginBusyState() {
    m_nBusyCount++;
}

void COleMessageFilter::EndBusyState() {
    if (m_nBusyCount > 0) m_nBusyCount--;
}

BOOL COleMessageFilter::IsBusy() const {
    return m_nBusyCount > 0;
}

BOOL COleMessageFilter::OnMessagePending(const MSG* pMsg) {
    return FALSE;
}

int COleMessageFilter::OnBusyDialog(HTASK hTaskBusy) {
    return -1;
}

//=============================================================================
// COleCmdUI
//=============================================================================
COleCmdUI::COleCmdUI(OLECMD* rgCmds, ULONG cCmds, const GUID* pGuid)
    : m_pCmd(nullptr), m_cCmds(cCmds), m_pGuid(pGuid), m_rgCmds(rgCmds) {
    memset(_olecmdui_padding, 0, sizeof(_olecmdui_padding));
}

COleCmdUI::~COleCmdUI() {
}

void COleCmdUI::Enable(BOOL bOn) {
    if (m_rgCmds && m_cCmds > 0) {
        m_rgCmds[0].cmdf = bOn ? OLECMDF_ENABLED : OLECMDF_SUPPORTED;
    }
}

void COleCmdUI::SetCheck(int nCheck) {
    if (m_rgCmds && m_cCmds > 0) {
        if (nCheck == 1)
            m_rgCmds[0].cmdf |= OLECMDF_LATCHED;
        else
            m_rgCmds[0].cmdf &= ~OLECMDF_LATCHED;
    }
}

void COleCmdUI::SetText(const wchar_t* lpszText) {
    (void)lpszText;
}

//=============================================================================
// COleFrameHook
//=============================================================================
IMPLEMENT_DYNAMIC(COleFrameHook, CCmdTarget)

COleFrameHook::COleFrameHook(CFrameWnd* pFrameWnd, COleClientItem* pItem)
    : m_pFrameWnd(pFrameWnd), m_pActiveItem(pItem), m_lpActiveUIWindow(nullptr) {
    memset(_oleframehook_padding, 0, sizeof(_oleframehook_padding));
}

COleFrameHook::~COleFrameHook() {
}

BOOL COleFrameHook::OnDocActivate(BOOL bActivate) {
    return TRUE;
}

BOOL COleFrameHook::OnContextHelp() {
    return FALSE;
}

BOOL COleFrameHook::OnResizeBorder(LPCRECT lpRect, LPOLEINPLACEUIWINDOW lpUIWindow, BOOL bMainWindow) {
    return FALSE;
}

BOOL COleFrameHook::OnGetClipboardData(COleDataSource* pDataSource) {
    return FALSE;
}

//=============================================================================
// COleResizeBar
//=============================================================================
IMPLEMENT_DYNAMIC(COleResizeBar, CControlBar)

COleResizeBar::COleResizeBar()
    : m_pClientItem(nullptr), m_nHandleSize(4) {
    memset(_oleresizebar_padding, 0, sizeof(_oleresizebar_padding));
}

COleResizeBar::~COleResizeBar() {
}

BOOL COleResizeBar::Create(CWnd* pParentWnd, DWORD dwStyle, UINT nID) {
    (void)dwStyle;
    if (!pParentWnd) return FALSE;
    m_hWnd = CreateWindowW(L"STATIC", L"", WS_CHILD | WS_VISIBLE | SS_GRAYRECT,
                           0, 0, 0, 0, pParentWnd->GetSafeHwnd(), (HMENU)(UINT_PTR)nID,
                           AfxGetInstanceHandle(), nullptr);
    return m_hWnd != nullptr;
}

//=============================================================================
// COleStreamFile
//=============================================================================
COleStreamFile::COleStreamFile(LPSTREAM lpStream)
    : CFile(), m_lpStream(lpStream) {
    memset(_olestreamfile_padding, 0, sizeof(_olestreamfile_padding));
}

COleStreamFile::~COleStreamFile() {
    if (m_lpStream) {
        m_lpStream->Release();
        m_lpStream = nullptr;
    }
}

LPSTREAM COleStreamFile::GetStream() const {
    return m_lpStream;
}

BOOL COleStreamFile::OpenStream(LPSTORAGE lpStorage, const wchar_t* lpszStreamName, DWORD nOpenFlags) {
    if (!lpStorage || !lpszStreamName) return FALSE;
    if (m_lpStream) m_lpStream->Release();
    HRESULT hr = lpStorage->OpenStream(lpszStreamName, nullptr, nOpenFlags, 0, &m_lpStream);
    return SUCCEEDED(hr);
}

BOOL COleStreamFile::CreateStream(LPSTORAGE lpStorage, const wchar_t* lpszStreamName, DWORD nOpenFlags) {
    if (!lpStorage || !lpszStreamName) return FALSE;
    if (m_lpStream) m_lpStream->Release();
    HRESULT hr = lpStorage->CreateStream(lpszStreamName, nOpenFlags | STGM_CREATE, 0, 0, &m_lpStream);
    return SUCCEEDED(hr);
}

void COleStreamFile::Attach(LPSTREAM lpStream) {
    if (m_lpStream) m_lpStream->Release();
    m_lpStream = lpStream;
}

LPSTREAM COleStreamFile::Detach() {
    LPSTREAM lp = m_lpStream;
    m_lpStream = nullptr;
    return lp;
}

UINT COleStreamFile::Read(void* lpBuf, UINT nCount) {
    if (!m_lpStream) return 0;
    ULONG cbRead = 0;
    HRESULT hr = m_lpStream->Read(lpBuf, nCount, &cbRead);
    if (FAILED(hr)) return 0;
    return cbRead;
}

void COleStreamFile::Write(const void* lpBuf, UINT nCount) {
    if (!m_lpStream) return;
    ULONG cbWritten = 0;
    m_lpStream->Write(lpBuf, nCount, &cbWritten);
}

ULONGLONG COleStreamFile::Seek(LONGLONG lOff, UINT nFrom) {
    if (!m_lpStream) return (ULONGLONG)-1;
    LARGE_INTEGER li;
    li.QuadPart = lOff;
    ULARGE_INTEGER uli;
    HRESULT hr = m_lpStream->Seek(li, nFrom, &uli);
    if (FAILED(hr)) return (ULONGLONG)-1;
    return uli.QuadPart;
}

void COleStreamFile::LockRange(ULONGLONG dwPos, ULONGLONG dwCount) {
    if (!m_lpStream) return;
    ULARGE_INTEGER uliPos;
    ULARGE_INTEGER uliCount;
    uliPos.QuadPart = dwPos;
    uliCount.QuadPart = dwCount;
    m_lpStream->LockRegion(uliPos, uliCount, LOCK_EXCLUSIVE);
}

void COleStreamFile::UnlockRange(ULONGLONG dwPos, ULONGLONG dwCount) {
    if (!m_lpStream) return;
    ULARGE_INTEGER uliPos;
    ULARGE_INTEGER uliCount;
    uliPos.QuadPart = dwPos;
    uliCount.QuadPart = dwCount;
    m_lpStream->UnlockRegion(uliPos, uliCount, LOCK_EXCLUSIVE);
}

//=============================================================================
// COlePropertyPage
//=============================================================================
IMPLEMENT_DYNAMIC(COlePropertyPage, CDialog)

COlePropertyPage::COlePropertyPage()
    : CDialog(), m_pPageSite(nullptr), m_bModified(FALSE),
      m_pszPageName(nullptr), m_pszHelpFile(nullptr),
      m_dwHelpContext(0), m_pszDocString(nullptr) {
    memset(_olepropertypage_padding, 0, sizeof(_olepropertypage_padding));
}

COlePropertyPage::COlePropertyPage(UINT nIDTemplate, UINT nIDCaption)
    : CDialog(nIDTemplate), m_pPageSite(nullptr), m_bModified(FALSE),
      m_pszPageName(nullptr), m_pszHelpFile(nullptr),
      m_dwHelpContext(0), m_pszDocString(nullptr) {
    memset(_olepropertypage_padding, 0, sizeof(_olepropertypage_padding));
}

COlePropertyPage::~COlePropertyPage() {
}

void COlePropertyPage::SetPageName(const wchar_t* pszName) {
    m_pszPageName = (wchar_t*)pszName;
}

void COlePropertyPage::SetHelpInfo(const wchar_t* lpszDocString, const wchar_t* lpszHelpFile, DWORD dwHelpContext) {
    m_pszDocString = (wchar_t*)lpszDocString;
    m_pszHelpFile = (wchar_t*)lpszHelpFile;
    m_dwHelpContext = dwHelpContext;
}

BOOL COlePropertyPage::OnSetPageSite() {
    return TRUE;
}

void COlePropertyPage::OnObjectsChanged() {
}

BOOL COlePropertyPage::OnApply() {
    return TRUE;
}

void COlePropertyPage::OnEditProperty(DISPID dispid) {
    (void)dispid;
}

//=============================================================================
// COleDocument
//=============================================================================
IMPLEMENT_DYNAMIC(COleDocument, CDocument)

COleDocument::COleDocument()
    : CDocument(), m_bCompoundFile(FALSE), m_bRemember(TRUE) {
    memset(_coledocument_padding, 0, sizeof(_coledocument_padding));
}

COleDocument::~COleDocument() {
}

void COleDocument::AddItem(COleClientItem* pItem) {
    (void)pItem;
}

void COleDocument::RemoveItem(COleClientItem* pItem) {
    (void)pItem;
}

COleClientItem* COleDocument::GetStartPosition() const {
    return nullptr;
}

COleClientItem* COleDocument::GetNextClientItem(POSITION& pos) const {
    return nullptr;
}

COleClientItem* COleDocument::GetPrimarySelectedItem(CView* pView) const {
    return nullptr;
}

int COleDocument::GetItemCount() const {
    return 0;
}

void COleDocument::EnableCompoundFile(BOOL bEnable) {
    m_bCompoundFile = bEnable;
}

BOOL COleDocument::HasBlankItems() const {
    return FALSE;
}

BOOL COleDocument::IsInPlaceActive() const {
    return FALSE;
}

void COleDocument::OnShowViews(BOOL bVisible) {
}

COleClientItem* COleDocument::OnGetLinkedItem(const wchar_t* lpszItemName) {
    return nullptr;
}

COleClientItem* COleDocument::OnGetEmbeddedItem() {
    return nullptr;
}

void COleDocument::OnEditChangeIcon(COleClientItem* pItem) {
}

void COleDocument::OnEditConvert(COleClientItem* pItem) {
}

void COleDocument::OnEditLinks() {
}

LPOLEITEMCONTAINER COleDocument::GetItemContainer() {
    return nullptr;
}

//=============================================================================
// COleLinkingDoc
//=============================================================================
IMPLEMENT_DYNAMIC(COleLinkingDoc, COleDocument)

COleLinkingDoc::COleLinkingDoc()
    : COleDocument(), m_bRegistered(FALSE), m_lpMoniker(nullptr) {
    memset(_colelinkingdoc_padding, 0, sizeof(_colelinkingdoc_padding));
}

COleLinkingDoc::~COleLinkingDoc() {
    Revoke();
}

LPMONIKER COleLinkingDoc::GetMoniker(OLEGETMONIKER nAssign) {
    return m_lpMoniker;
}

LPMONIKER COleLinkingDoc::GetFileMoniker() {
    return m_lpMoniker;
}

BOOL COleLinkingDoc::RegisterIfServerAttached(const wchar_t* lpszFileName, BOOL bSetModified) {
    return FALSE;
}

void COleLinkingDoc::Revoke() {
    if (m_lpMoniker) {
        m_lpMoniker->Release();
        m_lpMoniker = nullptr;
    }
    m_bRegistered = FALSE;
}

BOOL COleLinkingDoc::IsRegistered() const {
    return m_bRegistered;
}

void COleLinkingDoc::OnShowDocument(BOOL bShow) {
}

//=============================================================================
// COleServerDoc
//=============================================================================
IMPLEMENT_DYNAMIC(COleServerDoc, COleLinkingDoc)

COleServerDoc::COleServerDoc()
    : COleLinkingDoc(), m_bEmbedded(FALSE) {
    memset(_coleserverdoc_padding, 0, sizeof(_coleserverdoc_padding));
}

COleServerDoc::~COleServerDoc() {
}

void COleServerDoc::NotifyChanged() {
}

void COleServerDoc::NotifyClosed() {
}

void COleServerDoc::NotifyRename(const wchar_t* lpszNewName) {
}

void COleServerDoc::NotifySaved() {
}

void COleServerDoc::SaveEmbedding() {
}

COleClientItem* COleServerDoc::GetEmbeddedItem() {
    return nullptr;
}

COleServerItem* COleServerDoc::GetEmbeddedServerItem() {
    return nullptr;
}

COleServerItem* COleServerDoc::GetLinkedServerItem(const wchar_t* lpszItemName) {
    return nullptr;
}

BOOL COleServerDoc::OnSetHostNames(const wchar_t* lpszHost, const wchar_t* lpszHostObj) {
    return TRUE;
}

//=============================================================================
// COleClientItem
//=============================================================================
IMPLEMENT_DYNAMIC(COleClientItem, CDocItem)

COleClientItem::COleClientItem(COleDocument* pContainerDoc)
    : m_pContainerDoc(pContainerDoc), m_lpObject(nullptr), m_lpLink(nullptr),
      m_lpInPlaceObject(nullptr), m_nStatus(OLE_EMPTY), m_nDrawAspect(DVASPECT_CONTENT),
      m_pControlSite(nullptr), m_lpFrame(nullptr), m_lpDocFrame(nullptr),
      m_bInPlaceActive(FALSE) {
    memset(_oleclientitem_padding, 0, sizeof(_oleclientitem_padding));
}

COleClientItem::~COleClientItem() {
    Close(OLECLOSE_NOSAVE);
}

BOOL COleClientItem::CreateFromClipboard(OLERENDER render, CLIPFORMAT cfFormat, FORMATETC* lpFormatEtc) {
    (void)render; (void)cfFormat; (void)lpFormatEtc;
    return FALSE;
}

BOOL COleClientItem::CreateNewItem(REFCLSID clsid, OLERENDER render, CLIPFORMAT cfFormat, FORMATETC* lpFormatEtc) {
    (void)clsid; (void)render; (void)cfFormat; (void)lpFormatEtc;
    return FALSE;
}

BOOL COleClientItem::CreateStaticFromClipboard(OLERENDER render, CLIPFORMAT cfFormat, FORMATETC* lpFormatEtc) {
    return FALSE;
}

BOOL COleClientItem::CreateFromData(COleDataObject* pDataObject, OLERENDER render, CLIPFORMAT cfFormat, FORMATETC* lpFormatEtc) {
    return FALSE;
}

BOOL COleClientItem::CreateLinkFromClipboard(OLERENDER render, CLIPFORMAT cfFormat, FORMATETC* lpFormatEtc) {
    return FALSE;
}

void COleClientItem::Activate(LONG nVerb, CView* pView, HWND hwndParent, LPCRECT lpRect, LPCRECT lpClipRect, BOOL bSplit) {
    (void)nVerb; (void)pView; (void)hwndParent; (void)lpRect; (void)lpClipRect; (void)bSplit;
    if (m_lpObject) {
        m_lpObject->DoVerb(nVerb, nullptr, nullptr, 0, nullptr, nullptr);
    }
}

void COleClientItem::Deactivate() {
    if (m_lpInPlaceObject) {
        m_lpInPlaceObject->InPlaceDeactivate();
    }
    m_bInPlaceActive = FALSE;
}

void COleClientItem::DoVerb(LONG nVerb, CView* pView, LPCRECT lpRect) {
    if (m_lpObject) {
        m_lpObject->DoVerb(nVerb, nullptr, nullptr, 0, nullptr, lpRect);
    }
}

BOOL COleClientItem::IsInPlaceActive() const {
    return m_bInPlaceActive;
}

OLE_STATUS COleClientItem::GetItemState() const {
    return m_nStatus;
}

void COleClientItem::SetItemState(OLE_STATUS nNewState) {
    m_nStatus = nNewState;
}

void COleClientItem::Close(OLE_CLOSE dwCloseOption) {
    if (m_lpObject) {
        m_lpObject->Close(dwCloseOption);
        m_lpObject->Release();
        m_lpObject = nullptr;
    }
    if (m_lpLink) {
        m_lpLink->Release();
        m_lpLink = nullptr;
    }
    if (m_lpInPlaceObject) {
        m_lpInPlaceObject->Release();
        m_lpInPlaceObject = nullptr;
    }
}

BOOL COleClientItem::Draw(CDC* pDC, LPCRECT lpBounds, DVASPECT nDrawAspect) {
    (void)pDC; (void)lpBounds; (void)nDrawAspect;
    return FALSE;
}

BOOL COleClientItem::GetExtent(LPSIZE lpSize, DVASPECT nDrawAspect) {
    (void)lpSize; (void)nDrawAspect;
    return FALSE;
}

BOOL COleClientItem::SetExtent(const CSize& size, DVASPECT nDrawAspect) {
    (void)size; (void)nDrawAspect;
    return FALSE;
}

void COleClientItem::GetClassID(CLSID* pClassID) {
    if (m_lpObject && pClassID) {
        m_lpObject->GetUserClassID(pClassID);
    }
}

void COleClientItem::CopyToClipboard(BOOL bIncludeLink) {
    (void)bIncludeLink;
}

void COleClientItem::OnChange(OLE_NOTIFICATION nCode, DWORD dwParam) {
}

void COleClientItem::OnActivate() {
}

void COleClientItem::OnDeactivate() {
}

void COleClientItem::OnGetItemPosition(CRect& rPosition) {
}

BOOL COleClientItem::OnChangeItemPosition(const CRect& rectPos) {
    return FALSE;
}

void COleClientItem::OnDiscardUndoState() {
}

void COleClientItem::SetControlSite(COleControlSite* pSite) {
    m_pControlSite = pSite;
}

COleControlSite* COleClientItem::GetControlSite() const {
    return m_pControlSite;
}

//=============================================================================
// COleServerItem
//=============================================================================
IMPLEMENT_DYNAMIC(COleServerItem, CDocItem)

COleServerItem::COleServerItem(COleServerDoc* pServerDoc, BOOL bAutoDelete)
    : m_pServerDoc(pServerDoc), m_bAutoDelete(bAutoDelete) {
    memset(_oleserveritem_padding, 0, sizeof(_oleserveritem_padding));
}

COleServerItem::~COleServerItem() {
}

BOOL COleServerItem::OnDraw(CDC* pDC, CSize& rSize) {
    return FALSE;
}

BOOL COleServerItem::OnDrawEx(CDC* pDC, DVASPECT nDrawAspect, CSize& rSize) {
    return FALSE;
}

BOOL COleServerItem::OnGetExtent(DVASPECT nDrawAspect, CSize& rSize) {
    return FALSE;
}

void COleServerItem::OnSetExtent(DVASPECT nDrawAspect, const CSize& size) {
}

void COleServerItem::Serialize(CArchive& ar) {
    CDocItem::Serialize(ar);
}

COleDataSource* COleServerItem::GetDataSource() {
    return nullptr;
}

void COleServerItem::CopyToClipboard(BOOL bIncludeLink) {
}

void COleServerItem::NotifyChanged() {
}

BOOL COleServerItem::IsConnected() const {
    return FALSE;
}

COleServerDoc* COleServerItem::GetDocument() const {
    return m_pServerDoc;
}

//=============================================================================
// COleObjectFactory
//=============================================================================
IMPLEMENT_DYNAMIC(COleObjectFactory, CCmdTarget)

COleObjectFactory::COleObjectFactory()
    : m_pRuntimeClass(nullptr), m_bMultiInstance(FALSE), m_strProgID(L""),
      m_dwRegister(0), m_pTemplate(nullptr) {
    memset(&m_clsid, 0, sizeof(m_clsid));
    memset(_oleobjectfactory_padding, 0, sizeof(_oleobjectfactory_padding));
}

COleObjectFactory::COleObjectFactory(REFCLSID clsid, CRuntimeClass* pRuntimeClass,
                                      BOOL bMultiInstance, const wchar_t* lpszProgID)
    : m_clsid(clsid), m_pRuntimeClass(pRuntimeClass),
      m_bMultiInstance(bMultiInstance), m_strProgID(lpszProgID ? lpszProgID : L""),
      m_dwRegister(0), m_pTemplate(nullptr) {
    memset(_oleobjectfactory_padding, 0, sizeof(_oleobjectfactory_padding));
}

COleObjectFactory::~COleObjectFactory() {
    Revoke();
}

BOOL COleObjectFactory::Register() {
    AfxOleInit();
    HRESULT hr = CoRegisterClassObject(m_clsid, (IUnknown*)this,
                                        CLSCTX_LOCAL_SERVER,
                                        REGCLS_MULTIPLEUSE | REGCLS_SUSPENDED,
                                        &m_dwRegister);
    return SUCCEEDED(hr);
}

void COleObjectFactory::Revoke() {
    if (m_dwRegister) {
        CoRevokeClassObject(m_dwRegister);
        m_dwRegister = 0;
    }
}

BOOL COleObjectFactory::IsRegistered() const {
    return m_dwRegister != 0;
}

BOOL COleObjectFactory::IsLicenseValid() {
    return TRUE;
}

BOOL COleObjectFactory::RegisterAll() {
    return TRUE;
}

BOOL COleObjectFactory::UpdateRegistryAll(BOOL bRegister) {
    return TRUE;
}

void COleObjectFactory::RevokeAll() {
}

CCmdTarget* COleObjectFactory::OnCreateObject() {
    return nullptr;
}

BOOL COleObjectFactory::OnVerifyFile(LPCTSTR lpszFileName) {
    return FALSE;
}

void COleObjectFactory::UpdateRegistry(BOOL bRegister) {
}

void COleObjectFactory::ConnectTemplate(COleTemplateServer* pTemplate) {
    m_pTemplate = pTemplate;
}

//=============================================================================
// COleTemplateServer
//=============================================================================
IMPLEMENT_DYNAMIC(COleTemplateServer, COleObjectFactory)

COleTemplateServer::COleTemplateServer()
    : m_pDocTemplate(nullptr) {
    memset(_oletemplateserver_padding, 0, sizeof(_oletemplateserver_padding));
}

COleTemplateServer::~COleTemplateServer() {
}

void COleTemplateServer::ConnectTemplate(REFCLSID clsid, CDocTemplate* pDocTemplate, BOOL bMultiInstance) {
    m_clsid = clsid;
    m_pRuntimeClass = nullptr;
    m_bMultiInstance = bMultiInstance;
    m_pDocTemplate = pDocTemplate;
}

void COleTemplateServer::UpdateRegistry(OLE_APPTYPE nAppType,
                                         const wchar_t** rglpszRegister,
                                         const wchar_t** rglpszOverwrite,
                                         BOOL bRegister) {
    (void)nAppType; (void)rglpszRegister; (void)rglpszOverwrite; (void)bRegister;
}

//=============================================================================
// COleControlSite
//=============================================================================
IMPLEMENT_DYNAMIC(COleControlSite, CCmdTarget)

COleControlSite::COleControlSite(COleControlContainer* pCtrlCont)
    : m_pCtrlCont(pCtrlCont), m_pControl(nullptr),
      m_lpObject(nullptr), m_lpInPlaceObject(nullptr), m_lpDispatch(nullptr),
      m_hWnd(nullptr), m_bInPlaceActive(FALSE), m_dwStyle(0) {
    memset(_olecontrolsite_padding, 0, sizeof(_olecontrolsite_padding));
}

COleControlSite::~COleControlSite() {
    DestroyControl();
}

BOOL COleControlSite::CreateControl(CWnd* pWndCtrl, REFCLSID clsid, const wchar_t* lpszWindowName,
                                     DWORD dwStyle, const RECT& rect, UINT nID, CFile* pPersist,
                                     BOOL bStorage, BSTR bstrLicKey) {
    (void)pWndCtrl; (void)clsid; (void)lpszWindowName; (void)dwStyle; (void)rect;
    (void)nID; (void)pPersist; (void)bStorage; (void)bstrLicKey;
    return FALSE;
}

BOOL COleControlSite::CreateControl(CWnd* pWndCtrl, const wchar_t* lpszProgID,
                                     DWORD dwStyle, const RECT& rect, UINT nID) {
    if (!lpszProgID) return FALSE;
    CLSID clsid;
    HRESULT hr = CLSIDFromProgID(lpszProgID, &clsid);
    if (FAILED(hr)) return FALSE;
    return CreateControl(pWndCtrl, clsid, L"", dwStyle, rect, nID);
}

void COleControlSite::DestroyControl() {
    if (m_lpObject) { m_lpObject->Release(); m_lpObject = nullptr; }
    if (m_lpInPlaceObject) { m_lpInPlaceObject->Release(); m_lpInPlaceObject = nullptr; }
    if (m_lpDispatch) { m_lpDispatch->Release(); m_lpDispatch = nullptr; }
}

void COleControlSite::Activate(BOOL bActivate) {
    (void)bActivate;
}

void COleControlSite::Deactivate() {
}

BOOL COleControlSite::IsInPlaceActive() const {
    return m_bInPlaceActive;
}

void COleControlSite::SetProperty(DISPID dwDispID, VARTYPE vtProp, ...) {
    (void)dwDispID; (void)vtProp;
}

void COleControlSite::GetProperty(DISPID dwDispID, VARTYPE vtProp, void* pvProp) {
    (void)dwDispID; (void)vtProp; (void)pvProp;
}

HRESULT COleControlSite::InvokeHelper(DISPID dwDispID, WORD wFlags, VARTYPE vtRet,
                                       void* pvRet, const BYTE* pbParamInfo, ...) {
    va_list args;
    va_start(args, pbParamInfo);
    HRESULT hr = InvokeHelperV(dwDispID, wFlags, vtRet, pvRet, pbParamInfo, args);
    va_end(args);
    return hr;
}

HRESULT COleControlSite::InvokeHelperV(DISPID dwDispID, WORD wFlags, VARTYPE vtRet,
                                        void* pvRet, const BYTE* pbParamInfo, va_list argList) {
    (void)dwDispID; (void)wFlags; (void)vtRet; (void)pvRet; (void)pbParamInfo; (void)argList;
    return E_NOTIMPL;
}

BOOL COleControlSite::GetAmbientProperty(DISPID dwDispid, VARTYPE vtProp, void* pvProp) {
    (void)dwDispid; (void)vtProp; (void)pvProp;
    return FALSE;
}

CWnd* COleControlSite::GetWindow() const {
    return CWnd::FromHandle(m_hWnd);
}

COleControlContainer* COleControlSite::GetContainer() const {
    return m_pCtrlCont;
}

COleControl* COleControlSite::GetControl() const {
    return m_pControl;
}

LPOLEOBJECT COleControlSite::GetOleObject() {
    return m_lpObject;
}

LPOLEINPLACEOBJECT COleControlSite::GetInPlaceObject() {
    return m_lpInPlaceObject;
}

LPDISPATCH COleControlSite::GetDispatch() {
    return m_lpDispatch;
}

//=============================================================================
// COleControlContainer
//=============================================================================
IMPLEMENT_DYNAMIC(COleControlContainer, CCmdTarget)

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
    return nullptr;
}

//=============================================================================
// COleIPFrameWnd
//=============================================================================
IMPLEMENT_DYNAMIC(COleIPFrameWnd, CFrameWnd)

COleIPFrameWnd::COleIPFrameWnd()
    : CFrameWnd(), m_pResizeBar(nullptr) {
    memset(_oleipframewnd_padding, 0, sizeof(_oleipframewnd_padding));
}

COleIPFrameWnd::~COleIPFrameWnd() {
}

BOOL COleIPFrameWnd::OnCreateAggregates() {
    return TRUE;
}

BOOL COleIPFrameWnd::OnCreateControlBars(CFrameWnd* pWndFrame, CFrameWnd* pWndDoc) {
    return TRUE;
}

void COleIPFrameWnd::OnRequestPositionChange(LPCRECT lpRect) {
}

//=============================================================================
// COleConnPtContainer
//=============================================================================
COleConnPtContainer::COleConnPtContainer() {
    memset(_oleconnptcontainer_padding, 0, sizeof(_oleconnptcontainer_padding));
}

COleConnPtContainer::~COleConnPtContainer() {
}

//=============================================================================
// CEnumOleVerb
//=============================================================================
CEnumOleVerb::CEnumOleVerb() {
    memset(_cenumoleverb_padding, 0, sizeof(_cenumoleverb_padding));
}

CEnumOleVerb::~CEnumOleVerb() {
}

//=============================================================================
// COleDialog
//=============================================================================
IMPLEMENT_DYNAMIC(COleDialog, CDialog)

COleDialog::COleDialog(UINT nIDTemplate, CWnd* pParentWnd)
    : CDialog(nIDTemplate, pParentWnd) {
    memset(_olediag_padding, 0, sizeof(_olediag_padding));
}

COleDialog::~COleDialog() {
}

//=============================================================================
// COleBusyDialog
//=============================================================================
IMPLEMENT_DYNAMIC(COleBusyDialog, COleDialog)

COleBusyDialog::COleBusyDialog(HTASK hTaskBusy, BOOL bNotResponding,
                                DWORD dwFlags, CWnd* pParentWnd)
    : COleDialog(0, pParentWnd), m_dwFlags(dwFlags) {
    memset(&m_bz, 0, sizeof(m_bz));
    m_bz.cbStruct = sizeof(OLEUIBUSYW);
    m_bz.hWndOwner = pParentWnd ? pParentWnd->GetSafeHwnd() : nullptr;
    m_bz.dwFlags = dwFlags;
    m_bz.hTask = hTaskBusy;
    memset(_olebusydialog_padding, 0, sizeof(_olebusydialog_padding));
}

COleBusyDialog::~COleBusyDialog() {
}

intptr_t COleBusyDialog::DoModal() {
    UINT result = OleUIBusyW(&m_bz);
    return (result == OLEUI_OK) ? IDOK : IDCANCEL;
}

//=============================================================================
// COleChangeIconDialog
//=============================================================================
IMPLEMENT_DYNAMIC(COleChangeIconDialog, COleDialog)

COleChangeIconDialog::COleChangeIconDialog(COleClientItem* pItem, DWORD dwFlags, CWnd* pParentWnd)
    : COleDialog(0, pParentWnd), m_dwFlags(dwFlags), m_pItem(pItem) {
    memset(&m_ci, 0, sizeof(m_ci));
    m_ci.cbStruct = sizeof(OLEUICHANGEICONW);
    m_ci.hWndOwner = pParentWnd ? pParentWnd->GetSafeHwnd() : nullptr;
    m_ci.dwFlags = dwFlags;
    memset(_olechangeicondialog_padding, 0, sizeof(_olechangeicondialog_padding));
}

COleChangeIconDialog::~COleChangeIconDialog() {
}

intptr_t COleChangeIconDialog::DoModal() {
    UINT result = OleUIChangeIconW(&m_ci);
    return (result == OLEUI_OK) ? IDOK : IDCANCEL;
}

//=============================================================================
// COleChangeSourceDialog
//=============================================================================
IMPLEMENT_DYNAMIC(COleChangeSourceDialog, COleDialog)

COleChangeSourceDialog::COleChangeSourceDialog(COleClientItem* pItem, DWORD dwFlags, CWnd* pParentWnd)
    : COleDialog(0, pParentWnd), m_dwFlags(dwFlags), m_pItem(pItem) {
    memset(&m_cs, 0, sizeof(m_cs));
    m_cs.cbStruct = sizeof(OLEUICHANGESOURCEW);
    m_cs.hWndOwner = pParentWnd ? pParentWnd->GetSafeHwnd() : nullptr;
    m_cs.dwFlags = dwFlags;
    memset(_olechangesourcedialog_padding, 0, sizeof(_olechangesourcedialog_padding));
}

COleChangeSourceDialog::~COleChangeSourceDialog() {
}

intptr_t COleChangeSourceDialog::DoModal() {
    UINT result = OleUIChangeSourceW(&m_cs);
    return (result == OLEUI_OK) ? IDOK : IDCANCEL;
}

CString COleChangeSourceDialog::GetDisplayName() {
    return m_cs.lpszDisplayName ? m_cs.lpszDisplayName : L"";
}

CString COleChangeSourceDialog::GetFrom() {
    return m_cs.lpszFrom ? m_cs.lpszFrom : L"";
}

CString COleChangeSourceDialog::GetTo() {
    return m_cs.lpszTo ? m_cs.lpszTo : L"";
}

//=============================================================================
// COleConvertDialog
//=============================================================================
IMPLEMENT_DYNAMIC(COleConvertDialog, COleDialog)

COleConvertDialog::COleConvertDialog(COleClientItem* pItem, DWORD dwFlags,
                                      CLSID* pClassID, CWnd* pParentWnd)
    : COleDialog(0, pParentWnd), m_dwFlags(dwFlags), m_pItem(pItem) {
    memset(&m_cv, 0, sizeof(m_cv));
    m_cv.cbStruct = sizeof(OLEUICONVERTW);
    m_cv.hWndOwner = pParentWnd ? pParentWnd->GetSafeHwnd() : nullptr;
    m_cv.dwFlags = dwFlags;
    if (pClassID) m_cv.clsid = *pClassID;
    memset(_oleconvertdialog_padding, 0, sizeof(_oleconvertdialog_padding));
}

COleConvertDialog::~COleConvertDialog() {
}

intptr_t COleConvertDialog::DoModal() {
    UINT result = OleUIConvertW(&m_cv);
    return (result == OLEUI_OK) ? IDOK : IDCANCEL;
}

CLSID COleConvertDialog::GetClassID() const {
    CLSID clsid = {};
    return clsid;
}

BOOL COleConvertDialog::IsConvertTo() const {
    return (m_cv.clsidConvertDefault == m_cv.clsidNew);
}

BOOL COleConvertDialog::IsActivateAs() const {
    return FALSE;
}

CString COleConvertDialog::GetDisplayIcon() const {
    return L"";
}

//=============================================================================
// COleInsertDialog
//=============================================================================
IMPLEMENT_DYNAMIC(COleInsertDialog, COleDialog)

COleInsertDialog::COleInsertDialog(DWORD dwFlags, CWnd* pParentWnd)
    : COleDialog(0, pParentWnd), m_dwFlags(dwFlags) {
    memset(&m_io, 0, sizeof(m_io));
    m_io.cbStruct = sizeof(OLEUIINSERTOBJECTW);
    m_io.hWndOwner = pParentWnd ? pParentWnd->GetSafeHwnd() : nullptr;
    m_io.dwFlags = dwFlags;
    memset(_oleinsertdialog_padding, 0, sizeof(_oleinsertdialog_padding));
}

COleInsertDialog::~COleInsertDialog() {
}

intptr_t COleInsertDialog::DoModal() {
    UINT result = OleUIInsertObjectW(&m_io);
    return (result == OLEUI_OK) ? IDOK : IDCANCEL;
}

CLSID COleInsertDialog::GetClassID() const {
    return m_io.clsid;
}

CString COleInsertDialog::GetPathName() const {
    return m_io.lpszFile ? m_io.lpszFile : L"";
}

BOOL COleInsertDialog::IsCreateNew() const {
    return m_io.dwFlags & IOF_SELECTCREATENEW || !(m_io.dwFlags & IOF_SELECTCREATEFROMFILE);
}

BOOL COleInsertDialog::IsDisplayAsIcon() const {
    return m_io.dwFlags & IOF_CHECKDISPLAYASICON;
}

COleClientItem* COleInsertDialog::CreateItem(COleDocument* pDoc) {
    (void)pDoc;
    return nullptr;
}

void COleInsertDialog::GetIconMetafile(HGLOBAL* phMetaPict) {
    if (phMetaPict) *phMetaPict = m_io.hMetaPict;
}

//=============================================================================
// COleLinksDialog
//=============================================================================
IMPLEMENT_DYNAMIC(COleLinksDialog, COleDialog)

COleLinksDialog::COleLinksDialog(COleDocument* pDoc, CView* pView, DWORD dwFlags, CWnd* pParentWnd)
    : COleDialog(0, pParentWnd), m_dwFlags(dwFlags), m_pDoc(pDoc) {
    memset(&m_el, 0, sizeof(m_el));
    m_el.cbStruct = sizeof(OLEUIEDITLINKSW);
    m_el.hWndOwner = pParentWnd ? pParentWnd->GetSafeHwnd() : nullptr;
    m_el.dwFlags = dwFlags;
    memset(_olelinksdialog_padding, 0, sizeof(_olelinksdialog_padding));
}

COleLinksDialog::~COleLinksDialog() {
}

intptr_t COleLinksDialog::DoModal() {
    UINT result = OleUIEditLinksW(&m_el);
    return (result == OLEUI_OK) ? IDOK : IDCANCEL;
}

//=============================================================================
// COlePasteSpecialDialog
//=============================================================================
IMPLEMENT_DYNAMIC(COlePasteSpecialDialog, COleDialog)

COlePasteSpecialDialog::COlePasteSpecialDialog(DWORD dwFlags, COleDataObject* pDataObject, CWnd* pParentWnd)
    : COleDialog(0, pParentWnd), m_dwFlags(dwFlags), m_pDataObject(pDataObject) {
    memset(&m_ps, 0, sizeof(m_ps));
    m_ps.cbStruct = sizeof(OLEUIPASTESPECIALW);
    m_ps.hWndOwner = pParentWnd ? pParentWnd->GetSafeHwnd() : nullptr;
    m_ps.dwFlags = dwFlags;
    memset(_olepastepecialdialog_padding, 0, sizeof(_olepastepecialdialog_padding));
}

COlePasteSpecialDialog::~COlePasteSpecialDialog() {
}

intptr_t COlePasteSpecialDialog::DoModal() {
    UINT result = OleUIPasteSpecialW(&m_ps);
    return (result == OLEUI_OK) ? IDOK : IDCANCEL;
}

CLSID COlePasteSpecialDialog::GetClassID() const {
    CLSID clsid = {};
    return clsid;
}

BOOL COlePasteSpecialDialog::IsPasteLink() const {
    return FALSE;
}

COleClientItem* COlePasteSpecialDialog::CreateItem(COleDocument* pDoc) {
    return nullptr;
}

//=============================================================================
// COlePropertiesDialog
//=============================================================================
IMPLEMENT_DYNAMIC(COlePropertiesDialog, COleDialog)

COlePropertiesDialog::COlePropertiesDialog(COleClientItem* pItem, UINT nScaleMin, UINT nScaleMax, CWnd* pParentWnd)
    : COleDialog(0, pParentWnd), m_pItem(pItem) {
    (void)nScaleMin; (void)nScaleMax;
    memset(_olepropertiesdialog_padding, 0, sizeof(_olepropertiesdialog_padding));
}

COlePropertiesDialog::~COlePropertiesDialog() {
}

intptr_t COlePropertiesDialog::DoModal() {
    return IDOK;  // OLEUIOBJECTPROPSW is not fully defined in MinGW
}

//=============================================================================
// COleUpdateDialog
//=============================================================================
IMPLEMENT_DYNAMIC(COleUpdateDialog, COleDialog)

COleUpdateDialog::COleUpdateDialog(COleDocument* pDoc, BOOL bUpdateLinks,
                                    BOOL bUpdateEmbeddings, CWnd* pParentWnd)
    : COleDialog(0, pParentWnd), m_pDoc(pDoc),
      m_bUpdateLinks(bUpdateLinks), m_bUpdateEmbeddings(bUpdateEmbeddings) {
    memset(_oleupdatedialog_padding, 0, sizeof(_oleupdatedialog_padding));
}

COleUpdateDialog::~COleUpdateDialog() {
}

intptr_t COleUpdateDialog::DoModal() {
    return IDOK;
}

//=============================================================================
// COleControl
//=============================================================================
IMPLEMENT_DYNAMIC(COleControl, CWnd)

COleControl::COleControl()
    : m_pControlSite(nullptr), m_pContainer(nullptr),
      m_bInitialized(FALSE), m_bInPlaceActive(FALSE),
      m_bOptimizedDraw(FALSE) {
    m_cxExtent.cx = 0;
    m_cxExtent.cy = 0;
    m_cyExtent.cx = 0;
    m_cyExtent.cy = 0;
    memset(_olecontrol_padding, 0, sizeof(_olecontrol_padding));
}

COleControl::~COleControl() {
}

BOOL COleControl::CreateControl(REFCLSID clsid, const wchar_t* lpszWindowName,
                                 DWORD dwStyle, const RECT& rect, CWnd* pParentWnd,
                                 UINT nID, CFile* pPersist, BOOL bStorage, BSTR bstrLicKey) {
    (void)clsid; (void)lpszWindowName; (void)dwStyle; (void)rect; (void)pParentWnd;
    (void)nID; (void)pPersist; (void)bStorage; (void)bstrLicKey;
    return FALSE;
}

BOOL COleControl::CreateControl(const wchar_t* lpszProgID, const wchar_t* lpszWindowName,
                                 DWORD dwStyle, const RECT& rect, CWnd* pParentWnd,
                                 UINT nID, CFile* pPersist, BOOL bStorage, BSTR bstrLicKey) {
    if (!lpszProgID) return FALSE;
    CLSID clsid;
    HRESULT hr = CLSIDFromProgID(lpszProgID, &clsid);
    if (FAILED(hr)) return FALSE;
    return CreateControl(clsid, lpszWindowName, dwStyle, rect, pParentWnd, nID, pPersist, bStorage, bstrLicKey);
}

BOOL COleControl::GetLicenseKey(DWORD dwReserved, BSTR* pbstrKey) {
    (void)dwReserved; (void)pbstrKey;
    return FALSE;
}

BSTR COleControl::GetLicenseKey(REFCLSID clsid) {
    (void)clsid;
    return nullptr;
}

void COleControl::Serialize(CArchive& ar) {
    CWnd::Serialize(ar);
}

BOOL COleControl::DoPropExchange(CPropExchange* pPX) {
    (void)pPX;
    return FALSE;
}

BOOL COleControl::GetAmbientProperty(DISPID dwDispid, VARTYPE vtProp, void* pvProp) {
    (void)dwDispid; (void)vtProp; (void)pvProp;
    return FALSE;
}

void COleControl::FireEvent(DISPID dispId, BYTE* pbParams, ...) {
    va_list args;
    va_start(args, pbParams);
    FireEventV(dispId, pbParams, args);
    va_end(args);
}

void COleControl::FireEventV(DISPID dispId, BYTE* pbParams, va_list argList) {
    (void)dispId; (void)pbParams; (void)argList;
}

BOOL COleControl::IsOptimizedDraw() const {
    return m_bOptimizedDraw;
}

void COleControl::SetInitialSize(int cx, int cy) {
    m_cxExtent.cx = cx;
    m_cyExtent.cy = cy;
}

void COleControl::OnDraw(CDC* pDC, const CRect& rcBounds, const CRect& rcInvalid) {
}

//=============================================================================
// OLE Helper Functions (extern "C" exports)
//=============================================================================
extern "C" {

MS_ABI int impl__AfxOleRegisterTypeLib(REFGUID guid, const wchar_t* lpszFileName) {
    if (!lpszFileName) return FALSE;
    ITypeLib* pTypeLib = nullptr;
    HRESULT hr = LoadTypeLib(lpszFileName, &pTypeLib);
    if (FAILED(hr)) return FALSE;
    hr = RegisterTypeLib(pTypeLib, (OLECHAR*)lpszFileName, nullptr);
    pTypeLib->Release();
    return SUCCEEDED(hr) ? TRUE : FALSE;
}

MS_ABI int impl__AfxOleRegisterServerClass(REFCLSID clsid, const wchar_t* lpszClassName,
                                            const wchar_t* lpszShortTypeName,
                                            const wchar_t* lpszLongTypeName,
                                            int nAppType, const wchar_t** rglpszRegister,
                                            const wchar_t** rglpszOverwrite) {
    (void)clsid; (void)lpszClassName; (void)lpszShortTypeName;
    (void)lpszLongTypeName; (void)nAppType; (void)rglpszRegister; (void)rglpszOverwrite;
    return FALSE;
}

MS_ABI int impl__AfxOleRegisterControlClass(REFCLSID clsid, const wchar_t* lpszClassName,
                                             const wchar_t* lpszShortTypeName,
                                             const wchar_t* lpszLongTypeName,
                                             DWORD dwMiscStatus, DWORD dwFlags, DWORD dwVersion) {
    (void)clsid; (void)lpszClassName; (void)lpszShortTypeName;
    (void)lpszLongTypeName; (void)dwMiscStatus; (void)dwFlags; (void)dwVersion;
    return FALSE;
}

MS_ABI int impl__AfxOleUnregisterClass(REFCLSID clsid, const wchar_t* lpszClassName) {
    (void)clsid; (void)lpszClassName;
    return TRUE;
}

MS_ABI int impl__AfxOleSetEditMenu(COleClientItem* pItem, CMenu* pMenu, UINT iMenuItem,
                                    UINT nIDVerbMin, UINT nIDVerbMax, UINT nIDConvert) {
    (void)pItem; (void)pMenu; (void)iMenuItem; (void)nIDVerbMin; (void)nIDVerbMax; (void)nIDConvert;
    return FALSE;
}

MS_ABI int impl__AfxOleSetUserCtrl(BOOL bUserCtrl) {
    (void)bUserCtrl;
    return FALSE;
}

MS_ABI COLORREF impl__AfxOleTranslateColor(OLE_COLOR clrColor, HPALETTE hpal) {
    (void)hpal;
    // OLE_COLOR is a COLORREF for standard colors
    // ARGB -> ABGR conversion for OLE_COLOR type
    if (clrColor & 0x80000000) {
        // Palette entry, extract from hpal
        return clrColor;
    }
    return clrColor;
}

MS_ABI int impl__AfxOleUnregisterTypeLib(REFGUID guid, WORD wVerMajor, WORD wVerMinor, LCID lcid) {
    HRESULT hr = UnRegisterTypeLib(guid, wVerMajor, wVerMinor, lcid, SYS_WIN32);
    return SUCCEEDED(hr) ? TRUE : FALSE;
}

} // extern "C"
