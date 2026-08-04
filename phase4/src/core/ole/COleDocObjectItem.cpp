// COleDocObjectItem — OpenMFC implementation.
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

























// Symbol: ??0COleDocObjectItem@@QEAA@PEAVCOleDocument@@@Z
// COleDocObjectItem::COleDocObjectItem(COleDocument* pContainerDoc)
extern "C" void* MS_ABI impl___0COleDocObjectItem__QEAA_PEAVCOleDocument___Z(COleDocument* pContainerDoc) {
    return new COleDocObjectItem(pContainerDoc);
}
COleDocObjectItem::COleDocObjectItem(COleDocument* pContainerDoc) : COleClientItem(pContainerDoc) { memset(_coledocobjectitem_padding, 0, sizeof(_coledocobjectitem_padding)); }
COleDocObjectItem::~COleDocObjectItem() {}
BOOL COleDocObjectItem::IsDocObject() const {
    if (!m_lpObject) return FALSE;
    IOleDocument* document = nullptr;
    HRESULT hr = m_lpObject->QueryInterface(IID_IOleDocument, reinterpret_cast<void**>(&document));
    if (document) document->Release();
    return SUCCEEDED(hr);
}
BOOL COleDocObjectItem::IsActive() const { return m_bInPlaceActive && IsOpen(); }
IOleDocumentView* COleDocObjectItem::GetActiveView() const {
    if (!m_lpObject) return nullptr;
    IOleDocumentView* view = nullptr;
    if (m_lpInPlaceObject &&
        SUCCEEDED(m_lpInPlaceObject->QueryInterface(IID_IOleDocumentView, reinterpret_cast<void**>(&view)))) {
        return view;
    }

    IOleDocument* document = nullptr;
    if (SUCCEEDED(m_lpObject->QueryInterface(IID_IOleDocument, reinterpret_cast<void**>(&document))) && document) {
        if (SUCCEEDED(document->CreateView(nullptr, nullptr, 0, &view)) && view) {
            InitializeDocumentView(const_cast<COleDocObjectItem*>(this), view);
        }
        document->Release();
    }
    return view;
}
HRESULT COleDocObjectItem::GetDocument(IUnknown** ppDocument) {
    if (!ppDocument) return E_POINTER;
    *ppDocument = nullptr;
    if (!m_lpObject) return OLE_E_NOTRUNNING;

    IOleDocument* document = nullptr;
    HRESULT hr = m_lpObject->QueryInterface(IID_IOleDocument, reinterpret_cast<void**>(&document));
    if (SUCCEEDED(hr) && document) {
        *ppDocument = static_cast<IUnknown*>(document);
        return S_OK;
    }
    return hr;
}
void COleDocObjectItem::ActivateAndShow() {
    if (!m_lpObject) return;
    Activate(OLEIVERB_SHOW);
    IOleDocumentView* view = GetActiveView();
    if (view) {
        view->Show(TRUE);
        view->Release();
    }
}
BOOL COleDocObjectItem::IsOpen() const { return m_nStatus == OLE_OPEN; }
void COleDocObjectItem::OnActivateView() {
    if (!m_lpObject) return;

    if (!IsOpen()) {
        OnOpen();
        if (m_nStatus == OLE_OPEN) {
            Activate(OLEIVERB_OPEN, nullptr, nullptr, nullptr, nullptr, FALSE);
        }
    }

    IOleDocumentView* view = GetActiveView();
    if (!view) return;
    view->UIActivate(TRUE);
    if (m_lpInPlaceObject && !m_bInPlaceActive) {
        m_bInPlaceActive = TRUE;
    }
    m_nStatus = OLE_OPEN;
    view->Show(TRUE);
    view->Release();
}
BOOL COleDocObjectItem::OnPreparePrinting(void* pInfo) { (void)pInfo; return TRUE; }
void COleDocObjectItem::OnBeginPrinting(CDC* pDC, void* pInfo) { (void)pDC; (void)pInfo; }
void COleDocObjectItem::OnPrint(CDC* pDC, void* pInfo) { (void)pDC; (void)pInfo; }
void COleDocObjectItem::OnEndPrinting(CDC* pDC, void* pInfo) { (void)pDC; (void)pInfo; }
HRESULT COleDocObjectItem::ExecCommand(DWORD nCmdID, DWORD nCmdExecOpt, const GUID* pguidCmdGroup) {
    if (!m_lpObject) return OLE_E_NOTRUNNING;

    IOleCommandTarget* commandTarget = nullptr;
    HRESULT hr = m_lpObject->QueryInterface(IID_IOleCommandTarget, reinterpret_cast<void**>(&commandTarget));
    if (FAILED(hr) || !commandTarget) return hr == E_NOINTERFACE ? OLECMDERR_E_NOTSUPPORTED : hr;

    hr = commandTarget->Exec(pguidCmdGroup, nCmdID, nCmdExecOpt, nullptr, nullptr);
    commandTarget->Release();
    return hr;
}
