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

// === Moved from ManualThunks.cpp ===
// Symbol: ?OnPreparePrinting@COleDocObjectItem@@SAHPEAVCView@@PEAUCPrintInfo@@H@Z
extern "C" int MS_ABI impl__OnPreparePrinting_COleDocObjectItem__SAHPEAVCView__PEAUCPrintInfo__H_Z(void* p0, void* p1, void* p2) {
    (void)p0;
    (void)p1;
    (void)p2;
    return 0;
}


// Symbol: ?OnPrint@COleDocObjectItem@@SAXPEAVCView@@PEAUCPrintInfo@@H@Z
extern "C" void MS_ABI impl__OnPrint_COleDocObjectItem__SAXPEAVCView__PEAUCPrintInfo__H_Z(void* p0, void* p1, void* p2) {
    (void)p0;
    (void)p1;
    (void)p2;
}

// Symbol: ?DoDefaultPrinting@COleDocObjectItem@@SAJPEAVCView@@PEAUCPrintInfo@@@Z
extern "C" long MS_ABI impl__DoDefaultPrinting_COleDocObjectItem__SAJPEAVCView__PEAUCPrintInfo___Z(void* /*class*/* p0, void* /*struct*/* p1) {
    return 0;
}

// Symbol: ?GetCommandMap@COleDocObjectItem@@MEBAPEBUAFX_OLECMDMAP@@XZ
extern "C" void* MS_ABI impl__GetCommandMap_COleDocObjectItem__MEBAPEBUAFX_OLECMDMAP__XZ() {
    return nullptr;
}

// Symbol: ?GetHelpMenu@COleDocObjectItem@@MEAAPEAVCMenu@@AEAI@Z
extern "C" void* MS_ABI impl__GetHelpMenu_COleDocObjectItem__MEAAPEAVCMenu__AEAI_Z(unsigned int* p0) {
    return nullptr;
}

// Symbol: ?GetPageCount@COleDocObjectItem@@QEAAHPEAJ0@Z
extern "C" int MS_ABI impl__GetPageCount_COleDocObjectItem__QEAAHPEAJ0_Z(long* p0, long* p1) {
    return 0;
}

// Symbol: ?GetThisCommandMap@COleDocObjectItem@@KAPEBUAFX_OLECMDMAP@@XZ
extern "C" void* MS_ABI impl__GetThisCommandMap_COleDocObjectItem__KAPEBUAFX_OLECMDMAP__XZ() {
    return nullptr;
}

// Symbol: ?OnCmdMsg@COleDocObjectItem@@MEAAHIHPEAXPEAUAFX_CMDHANDLERINFO@@@Z
extern "C" int MS_ABI impl__OnCmdMsg_COleDocObjectItem__MEAAHIHPEAXPEAUAFX_CMDHANDLERINFO___Z(unsigned int p0, int p1, void* p2, void* /*struct*/* p3) {
    return 0;
}

// Symbol: ?OnGetItemPosition@COleDocObjectItem@@MEAAXAEAVCRect@@@Z
extern "C" void MS_ABI impl__OnGetItemPosition_COleDocObjectItem__MEAAXAEAVCRect___Z(void* /*class*/* p0) {}

// Symbol: ?OnInsertMenus@COleDocObjectItem@@UEAAXPEAVCMenu@@PEAUtagOleMenuGroupWidths@@@Z
extern "C" void MS_ABI impl__OnInsertMenus_COleDocObjectItem__UEAAXPEAVCMenu__PEAUtagOleMenuGroupWidths___Z(void* /*class*/* p0, void* /*struct*/* p1) {}

// Symbol: ?OnRemoveMenus@COleDocObjectItem@@UEAAXPEAVCMenu@@@Z
extern "C" void MS_ABI impl__OnRemoveMenus_COleDocObjectItem__UEAAXPEAVCMenu___Z(void* /*class*/* p0) {}

// Symbol: ?QueryCommand@COleDocObjectItem@@QEAAJKPEAKPEAU_tagOLECMDTEXT@@PEBU_GUID@@@Z
extern "C" long MS_ABI impl__QueryCommand_COleDocObjectItem__QEAAJKPEAKPEAU_tagOLECMDTEXT__PEBU_GUID___Z(unsigned long p0, unsigned long* p1, void* /*struct*/* p2, const void* /*struct*/* p3) {
    return 0;
}

// Symbol: ?Release@COleDocObjectItem@@UEAAXW4tagOLECLOSE@@@Z
extern "C" void MS_ABI impl__Release_COleDocObjectItem__UEAAXW4tagOLECLOSE___Z(int /*enum*/ p0, void* p1, void* p2, void* p3, void* p4, void* p5, unsigned char p6, void* p7, void* p8, void* p9, void* p10, unsigned char p11) {}

// Symbol: ?SupportsIPrint@COleDocObjectItem@@IEAAHXZ
extern "C" int MS_ABI impl__SupportsIPrint_COleDocObjectItem__IEAAHXZ() {
    return 0;
}

// Symbol: ?ActivateMe@XOleDocumentSite@COleDocObjectItem@@UEAAJPEAUIOleDocumentView@@@Z
extern "C" long MS_ABI impl__ActivateMe_XOleDocumentSite_COleDocObjectItem__UEAAJPEAUIOleDocumentView___Z(void* /*struct*/* p0) {
    return 0;
}
