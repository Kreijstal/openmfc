// OLE/COM Implementation
// Provides MFC wrappers around the OLE32/OLEAUT32 COM APIs
//
// All exports use MSVC ABI via extern "C" + .def aliasing.

#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxole.h"
#include <algorithm>
#include <cstring>
#include <cstdio>
#include <vector>

// MinGW compat: Ambient property DISPIDs
#ifndef DISPID_AMBIENT_BACKCOLOR
#define DISPID_AMBIENT_BACKCOLOR    (-701)
#define DISPID_AMBIENT_FORECOLOR    (-705)
#define DISPID_AMBIENT_FONT         (-703)
#define DISPID_AMBIENT_USERMODE     (-709)
#endif
#ifndef VT_COLOR
#define VT_COLOR 0x0000000CL
#endif

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
static COleDataSource* g_pClipboardOwner = nullptr;

namespace {

struct DataCacheEntry {
    FORMATETC format = {};
    STGMEDIUM medium = {};
    bool hasMedium = false;
    bool delayRender = false;
    bool delayRenderFile = false;
};

static FORMATETC MakeFormatEtc(CLIPFORMAT cfFormat, FORMATETC* lpFormatEtc) {
    if (lpFormatEtc) {
        FORMATETC fmt = *lpFormatEtc;
        fmt.cfFormat = cfFormat ? cfFormat : fmt.cfFormat;
        return fmt;
    }

    FORMATETC fmt = {};
    fmt.cfFormat = cfFormat;
    fmt.ptd = nullptr;
    fmt.dwAspect = DVASPECT_CONTENT;
    fmt.lindex = -1;
    fmt.tymed = TYMED_HGLOBAL;
    return fmt;
}

static bool FormatMatches(const FORMATETC& cached, const FORMATETC& requested) {
    if (cached.cfFormat != requested.cfFormat) return false;
    if ((cached.tymed & requested.tymed) == 0) return false;
    if (requested.dwAspect != 0 && cached.dwAspect != requested.dwAspect) return false;
    if (requested.lindex != -1 && cached.lindex != requested.lindex) return false;
    return true;
}

static HGLOBAL DuplicateGlobalMemory(HGLOBAL source) {
    if (!source) return nullptr;

    SIZE_T size = GlobalSize(source);
    HGLOBAL copy = GlobalAlloc(GMEM_MOVEABLE, size);
    if (!copy) return nullptr;

    void* src = GlobalLock(source);
    void* dst = GlobalLock(copy);
    if (!src || !dst) {
        if (src) GlobalUnlock(source);
        if (dst) GlobalUnlock(copy);
        GlobalFree(copy);
        return nullptr;
    }

    memcpy(dst, src, size);
    GlobalUnlock(copy);
    GlobalUnlock(source);
    return copy;
}

static bool CopyStorageMedium(const STGMEDIUM& source, STGMEDIUM* dest) {
    if (!dest) return false;
    memset(dest, 0, sizeof(*dest));
    dest->tymed = source.tymed;
    dest->pUnkForRelease = nullptr;

    switch (source.tymed) {
    case TYMED_HGLOBAL:
        dest->hGlobal = DuplicateGlobalMemory(source.hGlobal);
        return dest->hGlobal != nullptr;
    case TYMED_ISTREAM:
        dest->pstm = source.pstm;
        if (dest->pstm) dest->pstm->AddRef();
        return dest->pstm != nullptr;
    case TYMED_ISTORAGE:
        dest->pstg = source.pstg;
        if (dest->pstg) dest->pstg->AddRef();
        return dest->pstg != nullptr;
    case TYMED_GDI:
        dest->hBitmap = static_cast<HBITMAP>(CopyImage(source.hBitmap, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION));
        return dest->hBitmap != nullptr;
    case TYMED_MFPICT:
        dest->hMetaFilePict = source.hMetaFilePict;
        return dest->hMetaFilePict != nullptr;
    case TYMED_ENHMF:
        dest->hEnhMetaFile = CopyEnhMetaFileW(source.hEnhMetaFile, nullptr);
        return dest->hEnhMetaFile != nullptr;
    case TYMED_FILE:
        dest->lpszFileName = source.lpszFileName ? static_cast<LPOLESTR>(CoTaskMemAlloc((wcslen(source.lpszFileName) + 1) * sizeof(OLECHAR))) : nullptr;
        if (source.lpszFileName && !dest->lpszFileName) return false;
        if (source.lpszFileName) wcscpy(dest->lpszFileName, source.lpszFileName);
        return true;
    default:
        dest->tymed = TYMED_NULL;
        return false;
    }
}

static LPOLESTR CopyOleString(const wchar_t* text) {
    if (!text) return nullptr;
    const size_t bytes = (wcslen(text) + 1) * sizeof(OLECHAR);
    LPOLESTR copy = static_cast<LPOLESTR>(CoTaskMemAlloc(bytes));
    if (copy) memcpy(copy, text, bytes);
    return copy;
}

static bool MakeRenderedFileMedium(COleDataSource* source, FORMATETC* format, STGMEDIUM* medium) {
    if (!source || !medium) return false;

    wchar_t tempPath[MAX_PATH] = {};
    wchar_t tempName[MAX_PATH] = {};
    if (!GetTempPathW(MAX_PATH, tempPath)) return false;
    if (!GetTempFileNameW(tempPath, L"omf", 0, tempName)) return false;

    bool ok = false;
    {
        CFile file(tempName, CFile::modeCreate | CFile::modeWrite | CFile::shareDenyNone | CFile::typeBinary);
        ok = source->OnRenderFileData(format, &file) != 0;
    }

    if (!ok) {
        DeleteFileW(tempName);
        return false;
    }

    memset(medium, 0, sizeof(*medium));
    medium->tymed = TYMED_FILE;
    medium->lpszFileName = CopyOleString(tempName);
    if (!medium->lpszFileName) {
        DeleteFileW(tempName);
        return false;
    }
    return true;
}

static int CountDispatchParams(const BYTE* pbParamInfo) {
    if (!pbParamInfo) return 0;
    int count = 0;
    while (pbParamInfo[count] != 0) ++count;
    return count;
}

static HRESULT MakeDispatchVariant(VARTYPE vt, va_list* args, VARIANTARG* var) {
    if (!var) return E_POINTER;
    VariantInit(var);
    switch (vt) {
    case VT_I2:
        var->vt = VT_I2;
        var->iVal = static_cast<SHORT>(va_arg(*args, int));
        return S_OK;
    case VT_I4:
        var->vt = VT_I4;
        var->lVal = va_arg(*args, long);
        return S_OK;
    case VT_R4:
        var->vt = VT_R4;
        var->fltVal = static_cast<float>(va_arg(*args, double));
        return S_OK;
    case VT_R8:
        var->vt = VT_R8;
        var->dblVal = va_arg(*args, double);
        return S_OK;
    case VT_BOOL:
        var->vt = VT_BOOL;
        var->boolVal = va_arg(*args, int) ? VARIANT_TRUE : VARIANT_FALSE;
        return S_OK;
    case VT_BSTR:
        var->vt = VT_BSTR;
        var->bstrVal = SysAllocString(va_arg(*args, const wchar_t*));
        return var->bstrVal ? S_OK : E_OUTOFMEMORY;
    case VT_DISPATCH:
        var->vt = VT_DISPATCH;
        var->pdispVal = va_arg(*args, LPDISPATCH);
        return S_OK;
    case VT_UNKNOWN:
        var->vt = VT_UNKNOWN;
        var->punkVal = va_arg(*args, LPUNKNOWN);
        return S_OK;
    case VT_VARIANT: {
        VARIANT* src = va_arg(*args, VARIANT*);
        return src ? VariantCopy(var, src) : S_OK;
    }
    case VT_UI1:
        var->vt = VT_UI1;
        var->bVal = static_cast<BYTE>(va_arg(*args, int));
        return S_OK;
    case VT_UI2:
        var->vt = VT_UI2;
        var->uiVal = static_cast<USHORT>(va_arg(*args, int));
        return S_OK;
    case VT_UI4:
        var->vt = VT_UI4;
        var->ulVal = va_arg(*args, unsigned long);
        return S_OK;
    default:
        return DISP_E_TYPEMISMATCH;
    }
}

static HRESULT CopyDispatchResult(VARTYPE vt, void* pvRet, VARIANT* result) {
    if (vt == VT_EMPTY || !pvRet) return S_OK;
    switch (vt) {
    case VT_I2:
        *static_cast<short*>(pvRet) = result->iVal;
        return S_OK;
    case VT_I4:
        *static_cast<long*>(pvRet) = result->lVal;
        return S_OK;
    case VT_R4:
        *static_cast<float*>(pvRet) = result->fltVal;
        return S_OK;
    case VT_R8:
        *static_cast<double*>(pvRet) = result->dblVal;
        return S_OK;
    case VT_BOOL:
        *static_cast<BOOL*>(pvRet) = (result->boolVal == VARIANT_TRUE);
        return S_OK;
    case VT_BSTR:
        *static_cast<BSTR*>(pvRet) = result->bstrVal;
        result->vt = VT_EMPTY;
        return S_OK;
    case VT_DISPATCH:
        *static_cast<LPDISPATCH*>(pvRet) = result->pdispVal;
        result->vt = VT_EMPTY;
        return S_OK;
    case VT_UNKNOWN:
        *static_cast<LPUNKNOWN*>(pvRet) = result->punkVal;
        result->vt = VT_EMPTY;
        return S_OK;
    case VT_VARIANT:
        return VariantCopy(static_cast<VARIANT*>(pvRet), result);
    case VT_UI1:
        *static_cast<BYTE*>(pvRet) = result->bVal;
        return S_OK;
    case VT_UI2:
        *static_cast<USHORT*>(pvRet) = result->uiVal;
        return S_OK;
    case VT_UI4:
        *static_cast<ULONG*>(pvRet) = result->ulVal;
        return S_OK;
    default:
        return DISP_E_TYPEMISMATCH;
    }
}

struct DataCacheState;

class DataSourceDataObject : public IDataObject {
public:
    explicit DataSourceDataObject(DataCacheState* state) : m_refCount(1), m_state(state) {}

    HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** ppvObject) override;
    ULONG STDMETHODCALLTYPE AddRef() override { return InterlockedIncrement(&m_refCount); }
    ULONG STDMETHODCALLTYPE Release() override {
        ULONG ref = InterlockedDecrement(&m_refCount);
        return ref;
    }

    HRESULT STDMETHODCALLTYPE GetData(FORMATETC* pformatetcIn, STGMEDIUM* pmedium) override;
    HRESULT STDMETHODCALLTYPE GetDataHere(FORMATETC*, STGMEDIUM*) override { return DATA_E_FORMATETC; }
    HRESULT STDMETHODCALLTYPE QueryGetData(FORMATETC* pformatetc) override;
    HRESULT STDMETHODCALLTYPE GetCanonicalFormatEtc(FORMATETC*, FORMATETC* pformatetcOut) override {
        if (!pformatetcOut) return E_POINTER;
        pformatetcOut->ptd = nullptr;
        return DATA_S_SAMEFORMATETC;
    }
    HRESULT STDMETHODCALLTYPE SetData(FORMATETC* pformatetc, STGMEDIUM* pmedium, BOOL fRelease) override;
    HRESULT STDMETHODCALLTYPE EnumFormatEtc(DWORD dwDirection, IEnumFORMATETC** ppenumFormatEtc) override;
    HRESULT STDMETHODCALLTYPE DAdvise(FORMATETC*, DWORD, IAdviseSink*, DWORD*) override { return OLE_E_ADVISENOTSUPPORTED; }
    HRESULT STDMETHODCALLTYPE DUnadvise(DWORD) override { return OLE_E_ADVISENOTSUPPORTED; }
    HRESULT STDMETHODCALLTYPE EnumDAdvise(IEnumSTATDATA**) override { return OLE_E_ADVISENOTSUPPORTED; }

private:
    LONG m_refCount;
    DataCacheState* m_state;
};

struct DataCacheState {
    explicit DataCacheState(COleDataSource* source) : owner(source), dataObject(this) {}
    ~DataCacheState() {
        for (DataCacheEntry& entry : entries) {
            if (entry.hasMedium) ReleaseStgMedium(&entry.medium);
        }
    }

    COleDataSource* owner;
    DataSourceDataObject dataObject;
    std::vector<DataCacheEntry> entries;
};

class DropTargetAdapter : public IDropTarget {
public:
    explicit DropTargetAdapter(COleDropTarget* target) : m_refCount(1), m_target(target) {}
    virtual ~DropTargetAdapter() {}

    HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** ppvObject) override {
        if (!ppvObject) return E_POINTER;
        if (riid == IID_IUnknown || riid == IID_IDropTarget) {
            *ppvObject = static_cast<IDropTarget*>(this);
            AddRef();
            return S_OK;
        }
        *ppvObject = nullptr;
        return E_NOINTERFACE;
    }

    ULONG STDMETHODCALLTYPE AddRef() override { return InterlockedIncrement(&m_refCount); }

    ULONG STDMETHODCALLTYPE Release() override {
        ULONG ref = InterlockedDecrement(&m_refCount);
        if (ref == 0) delete this;
        return ref;
    }

    HRESULT STDMETHODCALLTYPE DragEnter(IDataObject* dataObject, DWORD keyState, POINTL point, DWORD* effect) override;
    HRESULT STDMETHODCALLTYPE DragOver(DWORD keyState, POINTL point, DWORD* effect) override;
    HRESULT STDMETHODCALLTYPE DragLeave() override;
    HRESULT STDMETHODCALLTYPE Drop(IDataObject* dataObject, DWORD keyState, POINTL point, DWORD* effect) override;

private:
    CPoint MakePoint(POINTL point) const { return CPoint(static_cast<int>(point.x), static_cast<int>(point.y)); }

    LONG m_refCount;
    COleDropTarget* m_target;
    COleDataObject m_dataObject;
};

struct DropTargetState {
    COleDropTarget* target = nullptr;
    DropTargetAdapter* adapter = nullptr;
};

struct DocumentState {
    COleDocument* document = nullptr;
    std::vector<COleClientItem*> items;
};

struct ClientItemState {
    COleClientItem* item = nullptr;
    LONG activeVerb = OLEIVERB_PRIMARY;
    BOOL modified = FALSE;
};

static std::vector<DropTargetState> g_dropTargetStates;
static std::vector<DocumentState> g_documentStates;
static std::vector<ClientItemState> g_clientItemStates;

static DropTargetState* GetDropTargetState(COleDropTarget* target, bool create) {
    if (!target) return nullptr;
    for (DropTargetState& state : g_dropTargetStates) {
        if (state.target == target) return &state;
    }
    if (!create) return nullptr;
    g_dropTargetStates.push_back(DropTargetState{target, nullptr});
    return &g_dropTargetStates.back();
}

static void RemoveDropTargetState(COleDropTarget* target) {
    g_dropTargetStates.erase(
        std::remove_if(g_dropTargetStates.begin(), g_dropTargetStates.end(),
                       [target](const DropTargetState& state) { return state.target == target; }),
        g_dropTargetStates.end());
}

static DocumentState* GetDocumentState(COleDocument* document, bool create) {
    if (!document) return nullptr;
    for (DocumentState& state : g_documentStates) {
        if (state.document == document) return &state;
    }
    if (!create) return nullptr;
    g_documentStates.push_back(DocumentState());
    g_documentStates.back().document = document;
    return &g_documentStates.back();
}

static void RemoveDocumentState(COleDocument* document) {
    g_documentStates.erase(
        std::remove_if(g_documentStates.begin(), g_documentStates.end(),
                       [document](const DocumentState& state) { return state.document == document; }),
        g_documentStates.end());
}

static ClientItemState* GetClientItemState(COleClientItem* item, bool create) {
    if (!item) return nullptr;
    for (ClientItemState& state : g_clientItemStates) {
        if (state.item == item) return &state;
    }
    if (!create) return nullptr;
    g_clientItemStates.push_back(ClientItemState());
    g_clientItemStates.back().item = item;
    return &g_clientItemStates.back();
}

static void RemoveClientItemState(COleClientItem* item) {
    g_clientItemStates.erase(
        std::remove_if(g_clientItemStates.begin(), g_clientItemStates.end(),
                       [item](const ClientItemState& state) { return state.item == item; }),
        g_clientItemStates.end());
}

static void AddDocumentItem(COleDocument* document, COleClientItem* item) {
    DocumentState* state = GetDocumentState(document, true);
    if (!state || !item) return;
    if (std::find(state->items.begin(), state->items.end(), item) == state->items.end()) {
        state->items.push_back(item);
    }
    item->m_pContainerDoc = document;
    item->m_pDocument = document;
}

static void RemoveDocumentItem(COleDocument* document, COleClientItem* item) {
    DocumentState* state = GetDocumentState(document, false);
    if (!state || !item) return;
    state->items.erase(std::remove(state->items.begin(), state->items.end(), item), state->items.end());
    if (item->m_pContainerDoc == document) item->m_pContainerDoc = nullptr;
    if (item->m_pDocument == document) item->m_pDocument = nullptr;
}

static DataCacheState* GetDataCacheState(COleDataSource* source, bool create) {
    if (!source) return nullptr;
    DataCacheState* state = static_cast<DataCacheState*>(source->m_pDataCache);
    if (!state && create) {
        state = new DataCacheState(source);
        source->m_pDataCache = state;
    }
    return state;
}

static DataCacheEntry* FindCacheEntry(DataCacheState* state, const FORMATETC& format) {
    if (!state) return nullptr;
    for (DataCacheEntry& entry : state->entries) {
        if (FormatMatches(entry.format, format)) return &entry;
    }
    return nullptr;
}

HRESULT DataSourceDataObject::QueryInterface(REFIID riid, void** ppvObject) {
    if (!ppvObject) return E_POINTER;
    if (riid == IID_IUnknown || riid == IID_IDataObject) {
        *ppvObject = static_cast<IDataObject*>(this);
        AddRef();
        return S_OK;
    }
    *ppvObject = nullptr;
    return E_NOINTERFACE;
}

HRESULT DataSourceDataObject::GetData(FORMATETC* pformatetcIn, STGMEDIUM* pmedium) {
    if (!pformatetcIn || !pmedium) return E_POINTER;
    DataCacheEntry* entry = FindCacheEntry(m_state, *pformatetcIn);
    if (!entry) return DATA_E_FORMATETC;

    if (!entry->hasMedium) {
        if (entry->delayRender && m_state->owner->OnRenderGlobalData(&entry->format, reinterpret_cast<void**>(&entry->medium.hGlobal))) {
            entry->medium.tymed = TYMED_HGLOBAL;
            entry->medium.pUnkForRelease = nullptr;
            entry->hasMedium = true;
        } else if (entry->delayRenderFile && MakeRenderedFileMedium(m_state->owner, &entry->format, &entry->medium)) {
            entry->hasMedium = true;
        } else {
            return DATA_E_FORMATETC;
        }
    }

    return CopyStorageMedium(entry->medium, pmedium) ? S_OK : STG_E_MEDIUMFULL;
}

HRESULT DataSourceDataObject::QueryGetData(FORMATETC* pformatetc) {
    if (!pformatetc) return E_POINTER;
    return FindCacheEntry(m_state, *pformatetc) ? S_OK : DATA_E_FORMATETC;
}

HRESULT DataSourceDataObject::SetData(FORMATETC* pformatetc, STGMEDIUM* pmedium, BOOL fRelease) {
    if (!pformatetc || !pmedium) return E_POINTER;

    DataCacheEntry* existing = FindCacheEntry(m_state, *pformatetc);
    if (!existing) {
        m_state->entries.push_back(DataCacheEntry());
        existing = &m_state->entries.back();
    } else if (existing->hasMedium) {
        ReleaseStgMedium(&existing->medium);
        existing->hasMedium = false;
    }

    existing->format = *pformatetc;
    existing->delayRender = false;
    existing->delayRenderFile = false;
    if (fRelease) {
        existing->medium = *pmedium;
        memset(pmedium, 0, sizeof(*pmedium));
        existing->hasMedium = true;
        return S_OK;
    }
    existing->hasMedium = CopyStorageMedium(*pmedium, &existing->medium);
    return existing->hasMedium ? S_OK : STG_E_MEDIUMFULL;
}

HRESULT DataSourceDataObject::EnumFormatEtc(DWORD dwDirection, IEnumFORMATETC** ppenumFormatEtc) {
    if (!ppenumFormatEtc) return E_POINTER;
    *ppenumFormatEtc = nullptr;
    if (dwDirection != DATADIR_GET) return E_NOTIMPL;

    CEnumFormatEtc* enumerator = new CEnumFormatEtc();
    for (const DataCacheEntry& entry : m_state->entries) {
        enumerator->AddFormat(entry.format);
    }
    *ppenumFormatEtc = enumerator;
    return S_OK;
}

class DropSourceAdapter : public IDropSource {
public:
    explicit DropSourceAdapter(COleDropSource* source) : m_refCount(1), m_source(source) {}
    virtual ~DropSourceAdapter() {}
    HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** ppvObject) override {
        if (!ppvObject) return E_POINTER;
        if (riid == IID_IUnknown || riid == IID_IDropSource) {
            *ppvObject = static_cast<IDropSource*>(this);
            AddRef();
            return S_OK;
        }
        *ppvObject = nullptr;
        return E_NOINTERFACE;
    }
    ULONG STDMETHODCALLTYPE AddRef() override { return InterlockedIncrement(&m_refCount); }
    ULONG STDMETHODCALLTYPE Release() override {
        ULONG ref = InterlockedDecrement(&m_refCount);
        if (ref == 0) delete this;
        return ref;
    }
    HRESULT STDMETHODCALLTYPE QueryContinueDrag(BOOL fEscapePressed, DWORD grfKeyState) override {
        return m_source ? m_source->QueryContinueDrag(fEscapePressed, grfKeyState) : DRAGDROP_S_CANCEL;
    }
    HRESULT STDMETHODCALLTYPE GiveFeedback(DWORD dwEffect) override {
        return m_source ? m_source->GiveFeedback(dwEffect) : DRAGDROP_S_USEDEFAULTCURSORS;
    }

private:
    LONG m_refCount;
    COleDropSource* m_source;
};

HRESULT DropTargetAdapter::DragEnter(IDataObject* dataObject, DWORD keyState, POINTL point, DWORD* effect) {
    if (!effect) return E_POINTER;
    *effect = DROPEFFECT_NONE;
    if (!m_target) return S_OK;

    m_dataObject.Release();
    if (dataObject) m_dataObject.Attach(dataObject, FALSE);
    *effect = m_target->OnDragEnter(m_target->m_pWnd, &m_dataObject, keyState, MakePoint(point));
    return S_OK;
}

HRESULT DropTargetAdapter::DragOver(DWORD keyState, POINTL point, DWORD* effect) {
    if (!effect) return E_POINTER;
    *effect = m_target ? m_target->OnDragOver(m_target->m_pWnd, &m_dataObject, keyState, MakePoint(point)) : DROPEFFECT_NONE;
    return S_OK;
}

HRESULT DropTargetAdapter::DragLeave() {
    if (m_target) m_target->OnDragLeave(m_target->m_pWnd);
    m_dataObject.Release();
    return S_OK;
}

HRESULT DropTargetAdapter::Drop(IDataObject* dataObject, DWORD keyState, POINTL point, DWORD* effect) {
    if (!effect) return E_POINTER;
    DROPEFFECT requested = *effect;
    *effect = DROPEFFECT_NONE;
    if (!m_target) return S_OK;

    COleDataObject dropData;
    COleDataObject* data = &m_dataObject;
    if (dataObject) {
        dropData.Attach(dataObject, FALSE);
        data = &dropData;
    }

    if (m_target->OnDrop(m_target->m_pWnd, data, requested, MakePoint(point))) {
        *effect = requested;
    }
    m_dataObject.Release();
    (void)keyState;
    return S_OK;
}

} // namespace

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

BOOL COleDataObject::AttachClipboard() {
    EnsureClipboardObject();
    return m_lpDataObject != nullptr;
}

void COleDataObject::EnsureClipboardObject() {
    if (m_lpDataObject) {
        return;
    }

    LPDATAOBJECT lpDataObject = nullptr;
    if (SUCCEEDED(OleGetClipboard(&lpDataObject)) && lpDataObject) {
        Attach(lpDataObject, TRUE);
    }
}

CFile* COleDataObject::GetFileData(CLIPFORMAT cfFormat, FORMATETC* lpFormatEtc) {
    EnsureClipboardObject();
    if (!m_lpDataObject) {
        return nullptr;
    }

    FORMATETC fmt = {};
    if (!lpFormatEtc) {
        fmt.cfFormat = cfFormat;
        fmt.ptd = nullptr;
        fmt.dwAspect = DVASPECT_CONTENT;
        fmt.lindex = -1;
        fmt.tymed = TYMED_FILE;
        lpFormatEtc = &fmt;
    }

    STGMEDIUM stg = {};
    if (m_lpDataObject->GetData(lpFormatEtc, &stg) != S_OK) {
        return nullptr;
    }

    if (stg.tymed != TYMED_FILE || !stg.lpszFileName) {
        ReleaseStgMedium(&stg);
        return nullptr;
    }

    CString filePath = stg.lpszFileName;
    CoTaskMemFree(stg.lpszFileName);
    stg.lpszFileName = nullptr;

    CFileStatus status = {};
    if (!CFile::GetStatus(filePath, status, nullptr)) {
        return nullptr;
    }

    return new CFile(filePath, CFile::modeRead | CFile::shareDenyNone | CFile::typeBinary);
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
    STGMEDIUM medium = {};
    medium.tymed = TYMED_HGLOBAL;
    medium.hGlobal = hGlobal;
    medium.pUnkForRelease = nullptr;
    CacheData(cfFormat, &medium, lpFormatEtc);
}

void COleDataSource::CacheData(CLIPFORMAT cfFormat, STGMEDIUM* lpStorageMedium, FORMATETC* lpFormatEtc) {
    if (!lpStorageMedium) return;

    DataCacheState* state = GetDataCacheState(this, true);
    if (!state) return;

    FORMATETC format = MakeFormatEtc(cfFormat, lpFormatEtc);
    DataCacheEntry* entry = FindCacheEntry(state, format);
    if (!entry) {
        state->entries.push_back(DataCacheEntry());
        entry = &state->entries.back();
    } else if (entry->hasMedium) {
        ReleaseStgMedium(&entry->medium);
        entry->hasMedium = false;
    }

    entry->format = format;
    entry->delayRender = false;
    entry->delayRenderFile = false;
    entry->hasMedium = CopyStorageMedium(*lpStorageMedium, &entry->medium);
}

void COleDataSource::DelayRenderData(CLIPFORMAT cfFormat, FORMATETC* lpFormatEtc) {
    DataCacheState* state = GetDataCacheState(this, true);
    if (!state) return;
    FORMATETC format = MakeFormatEtc(cfFormat, lpFormatEtc);
    DataCacheEntry* entry = FindCacheEntry(state, format);
    if (!entry) {
        state->entries.push_back(DataCacheEntry());
        entry = &state->entries.back();
    } else if (entry->hasMedium) {
        ReleaseStgMedium(&entry->medium);
        entry->hasMedium = false;
    }
    entry->format = format;
    entry->delayRender = true;
    entry->delayRenderFile = false;
}

void COleDataSource::DelayRenderFileData(CLIPFORMAT cfFormat, FORMATETC* lpFormatEtc) {
    DataCacheState* state = GetDataCacheState(this, true);
    if (!state) return;
    FORMATETC format = MakeFormatEtc(cfFormat, lpFormatEtc);
    format.tymed = TYMED_FILE;
    DataCacheEntry* entry = FindCacheEntry(state, format);
    if (!entry) {
        state->entries.push_back(DataCacheEntry());
        entry = &state->entries.back();
    } else if (entry->hasMedium) {
        ReleaseStgMedium(&entry->medium);
        entry->hasMedium = false;
    }
    entry->format = format;
    entry->delayRender = false;
    entry->delayRenderFile = true;
}

void COleDataSource::SetClipboard() {
    IDataObject* dataObject = GetInterface(TRUE);
    if (dataObject && SUCCEEDED(OleSetClipboard(dataObject))) {
        if (g_pClipboardOwner && g_pClipboardOwner != this) {
            g_pClipboardOwner->m_bClipboardOwner = FALSE;
        }
        g_pClipboardOwner = this;
        m_bClipboardOwner = TRUE;
    }
    if (dataObject) dataObject->Release();
}

COleDataSource* COleDataSource::GetClipboardOwner() {
    return g_pClipboardOwner;
}

DROPEFFECT COleDataSource::DoDragDrop(DWORD dwEffects, LPCRECT lpRectStartDrag,
                                       COleDropSource* pDropSource) {
    (void)lpRectStartDrag;
    IDataObject* dataObject = GetInterface(TRUE);
    if (!dataObject) return DROPEFFECT_NONE;

    COleDropSource defaultDropSource;
    COleDropSource* source = pDropSource ? pDropSource : &defaultDropSource;
    DropSourceAdapter* adapter = new DropSourceAdapter(source);
    DWORD effect = DROPEFFECT_NONE;
    HRESULT hr = ::DoDragDrop(dataObject, adapter, dwEffects, &effect);
    adapter->Release();
    dataObject->Release();
    return SUCCEEDED(hr) ? effect : DROPEFFECT_NONE;
}

int COleDataSource::OnRenderGlobalData(FORMATETC* lpFormatEtc, void** phGlobal) {
    (void)lpFormatEtc;
    if (phGlobal) *phGlobal = nullptr;
    return 0;
}

int COleDataSource::OnRenderFileData(FORMATETC* lpFormatEtc, CFile* pFile) {
    (void)lpFormatEtc; (void)pFile;
    return 0;
}

void COleDataSource::Empty() {
    if (g_pClipboardOwner == this) g_pClipboardOwner = nullptr;
    delete static_cast<DataCacheState*>(m_pDataCache);
    m_pDataCache = nullptr;
    m_bClipboardOwner = FALSE;
}

LPDATAOBJECT COleDataSource::GetInterface(BOOL bAddRef) {
    DataCacheState* state = GetDataCacheState(this, true);
    if (!state) return nullptr;
    if (bAddRef) state->dataObject.AddRef();
    return &state->dataObject;
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
    DropTargetState* state = GetDropTargetState(this, true);
    if (!state) return FALSE;
    state->adapter = new DropTargetAdapter(this);
    HRESULT hr = RegisterDragDrop(pWnd->GetSafeHwnd(), state->adapter);
    if (SUCCEEDED(hr)) {
        m_bRegistered = TRUE;
        return TRUE;
    }
    state->adapter->Release();
    state->adapter = nullptr;
    RemoveDropTargetState(this);
    m_pWnd = nullptr;
    return FALSE;
}

void COleDropTarget::Revoke() {
    DropTargetState* state = GetDropTargetState(this, false);
    if (m_bRegistered && m_pWnd) {
        RevokeDragDrop(m_pWnd->GetSafeHwnd());
        m_bRegistered = FALSE;
    }
    if (state && state->adapter) {
        state->adapter->Release();
        state->adapter = nullptr;
    }
    RemoveDropTargetState(this);
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

int COleMessageFilter::Register() { return 0; }

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

int COleCmdUI::DoUpdate(CCmdTarget* pTarget, BOOL bDisableIfNoHndler) {
    (void)pTarget;
    if (!m_rgCmds || m_cCmds == 0) {
        return FALSE;
    }

    m_rgCmds[0].cmdf |= OLECMDF_SUPPORTED;
    if (bDisableIfNoHndler) {
        Enable(FALSE);
    }
    return TRUE;
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
    DocumentState* state = GetDocumentState(this, false);
    if (state) {
        for (COleClientItem* item : state->items) {
            if (item && item->m_pContainerDoc == this) item->m_pContainerDoc = nullptr;
            if (item && item->m_pDocument == this) item->m_pDocument = nullptr;
        }
    }
    RemoveDocumentState(this);
}

void COleDocument::AddItem(COleClientItem* pItem) {
    if (pItem) AddDocumentItem(this, pItem);
}

void COleDocument::RemoveItem(COleClientItem* pItem) {
    RemoveDocumentItem(this, pItem);
}

COleClientItem* COleDocument::GetStartPosition() const {
    DocumentState* state = GetDocumentState(const_cast<COleDocument*>(this), false);
    if (!state || state->items.empty()) return nullptr;
    return reinterpret_cast<COleClientItem*>(static_cast<uintptr_t>(1));
}

COleClientItem* COleDocument::GetNextClientItem(POSITION& pos) const {
    DocumentState* state = GetDocumentState(const_cast<COleDocument*>(this), false);
    uintptr_t index = reinterpret_cast<uintptr_t>(pos);
    if (!state || index == 0 || index > state->items.size()) {
        pos = nullptr;
        return nullptr;
    }
    COleClientItem* item = state->items[index - 1];
    pos = (index < state->items.size())
        ? reinterpret_cast<POSITION>(index + 1)
        : nullptr;
    return item;
}

COleClientItem* COleDocument::GetPrimarySelectedItem(CView* pView) const {
    (void)pView;
    POSITION pos = reinterpret_cast<POSITION>(GetStartPosition());
    return GetNextClientItem(pos);
}

int COleDocument::GetItemCount() const {
    DocumentState* state = GetDocumentState(const_cast<COleDocument*>(this), false);
    return state ? static_cast<int>(state->items.size()) : 0;
}

void COleDocument::EnableCompoundFile(BOOL bEnable) {
    m_bCompoundFile = bEnable;
}

BOOL COleDocument::HasBlankItems() const {
    DocumentState* state = GetDocumentState(const_cast<COleDocument*>(this), false);
    if (!state) return FALSE;
    for (COleClientItem* item : state->items) {
        if (item && item->GetItemState() == OLE_EMPTY) return TRUE;
    }
    return FALSE;
}

BOOL COleDocument::IsInPlaceActive() const {
    DocumentState* state = GetDocumentState(const_cast<COleDocument*>(this), false);
    if (!state) return FALSE;
    for (COleClientItem* item : state->items) {
        if (item && item->IsInPlaceActive()) return TRUE;
    }
    return FALSE;
}

void COleDocument::OnShowViews(BOOL bVisible) {
}

COleClientItem* COleDocument::OnGetLinkedItem(const wchar_t* lpszItemName) {
    return nullptr;
}

COleClientItem* COleDocument::OnGetEmbeddedItem() {
    return GetPrimarySelectedItem(nullptr);
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
    m_pDocument = pContainerDoc;
    GetClientItemState(this, true);
    if (pContainerDoc) AddDocumentItem(pContainerDoc, this);
}

COleClientItem::~COleClientItem() {
    if (m_pContainerDoc) RemoveDocumentItem(m_pContainerDoc, this);
    Close(OLECLOSE_NOSAVE);
    RemoveClientItemState(this);
}

BOOL COleClientItem::CreateFromClipboard(OLERENDER render, CLIPFORMAT cfFormat, FORMATETC* lpFormatEtc) {
    IDataObject* dataObject = nullptr;
    HRESULT hr = OleGetClipboard(&dataObject);
    if (FAILED(hr) || !dataObject) return FALSE;
    COleDataObject wrapper;
    wrapper.Attach(dataObject, TRUE);
    return CreateFromData(&wrapper, render, cfFormat, lpFormatEtc);
}

BOOL COleClientItem::CreateNewItem(REFCLSID clsid, OLERENDER render, CLIPFORMAT cfFormat, FORMATETC* lpFormatEtc) {
    (void)render; (void)cfFormat; (void)lpFormatEtc;
    Close(OLECLOSE_NOSAVE);
    HRESULT hr = CoCreateInstance(clsid, nullptr, CLSCTX_ALL, IID_IOleObject,
                                  reinterpret_cast<void**>(&m_lpObject));
    if (SUCCEEDED(hr) && m_lpObject) {
        OleSetContainedObject(m_lpObject, TRUE);
        m_nStatus = OLE_LOADED;
        return TRUE;
    }
    m_lpObject = nullptr;
    return FALSE;
}

BOOL COleClientItem::CreateStaticFromClipboard(OLERENDER render, CLIPFORMAT cfFormat, FORMATETC* lpFormatEtc) {
    return CreateFromClipboard(render, cfFormat, lpFormatEtc);
}

BOOL COleClientItem::CreateFromData(COleDataObject* pDataObject, OLERENDER render, CLIPFORMAT cfFormat, FORMATETC* lpFormatEtc) {
    if (!pDataObject) return FALSE;
    IDataObject* dataObject = pDataObject->GetIDataObject(TRUE);
    if (!dataObject) return FALSE;

    Close(OLECLOSE_NOSAVE);
    FORMATETC format = MakeFormatEtc(cfFormat, lpFormatEtc);
    HRESULT hr = OleCreateFromData(dataObject, IID_IOleObject, render,
                                   cfFormat || lpFormatEtc ? &format : nullptr,
                                   nullptr, nullptr,
                                   reinterpret_cast<void**>(&m_lpObject));
    dataObject->Release();
    if (SUCCEEDED(hr) && m_lpObject) {
        m_nStatus = OLE_LOADED;
        return TRUE;
    }
    m_lpObject = nullptr;
    return FALSE;
}

BOOL COleClientItem::CreateLinkFromClipboard(OLERENDER render, CLIPFORMAT cfFormat, FORMATETC* lpFormatEtc) {
    IDataObject* dataObject = nullptr;
    HRESULT hr = OleGetClipboard(&dataObject);
    if (FAILED(hr) || !dataObject) return FALSE;

    Close(OLECLOSE_NOSAVE);
    FORMATETC format = MakeFormatEtc(cfFormat, lpFormatEtc);
    hr = OleCreateLinkFromData(dataObject, IID_IOleObject, render,
                               cfFormat || lpFormatEtc ? &format : nullptr,
                               nullptr, nullptr,
                               reinterpret_cast<void**>(&m_lpObject));
    dataObject->Release();
    if (SUCCEEDED(hr) && m_lpObject) {
        m_lpObject->QueryInterface(IID_IOleLink, reinterpret_cast<void**>(&m_lpLink));
        m_nStatus = OLE_LOADED;
        return TRUE;
    }
    m_lpObject = nullptr;
    return FALSE;
}

void COleClientItem::Activate(LONG nVerb, CView* pView, HWND hwndParent, LPCRECT lpRect, LPCRECT lpClipRect, BOOL bSplit) {
    (void)lpClipRect; (void)bSplit;
    if (!m_lpObject) return;
    HWND parent = hwndParent;
    if (!parent && pView) parent = pView->GetSafeHwnd();
    RECT rect = {};
    LPCRECT rectToUse = lpRect;
    if (!rectToUse) {
        CRect itemRect;
        OnGetItemPosition(itemRect);
        rect.left = itemRect.left;
        rect.top = itemRect.top;
        rect.right = itemRect.right;
        rect.bottom = itemRect.bottom;
        rectToUse = &rect;
    }
    HRESULT hr = m_lpObject->DoVerb(nVerb, nullptr, nullptr, 0, parent, rectToUse);
    if (SUCCEEDED(hr)) {
        ClientItemState* state = GetClientItemState(this, true);
        if (state) state->activeVerb = nVerb;
        if (m_lpInPlaceObject) {
            m_lpInPlaceObject->Release();
            m_lpInPlaceObject = nullptr;
        }
        if (SUCCEEDED(m_lpObject->QueryInterface(IID_IOleInPlaceObject, reinterpret_cast<void**>(&m_lpInPlaceObject)))) {
            m_bInPlaceActive = TRUE;
        }
        m_nStatus = OLE_RUNNING;
        OnActivate();
    }
}

void COleClientItem::Deactivate() {
    if (m_lpInPlaceObject) {
        m_lpInPlaceObject->InPlaceDeactivate();
    }
    m_bInPlaceActive = FALSE;
}

int COleClientItem::DoVerb(LONG nVerb, CView* pView, MSG* lpMsg) {
    if (!m_lpObject) return -1;
    HWND parent = pView ? pView->GetSafeHwnd() : nullptr;
    CRect rect;
    OnGetItemPosition(rect);
    HRESULT hr = m_lpObject->DoVerb(nVerb, lpMsg, nullptr, 0, parent, rect);
    if (FAILED(hr)) return static_cast<int>(hr);
    ClientItemState* state = GetClientItemState(this, true);
    if (state) state->activeVerb = nVerb;
    if (m_lpInPlaceObject) {
        m_lpInPlaceObject->Release();
        m_lpInPlaceObject = nullptr;
    }
    if (SUCCEEDED(m_lpObject->QueryInterface(IID_IOleInPlaceObject, reinterpret_cast<void**>(&m_lpInPlaceObject)))) {
        m_bInPlaceActive = TRUE;
    }
    m_nStatus = OLE_RUNNING;
    return 0;
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
    m_bInPlaceActive = FALSE;
    m_nStatus = OLE_EMPTY;
}

BOOL COleClientItem::Draw(CDC* pDC, LPCRECT lpBounds, DVASPECT nDrawAspect) {
    if (!m_lpObject || !pDC || !pDC->GetSafeHdc() || !lpBounds) return FALSE;
    return SUCCEEDED(OleDraw(m_lpObject, nDrawAspect, pDC->GetSafeHdc(), lpBounds));
}

BOOL COleClientItem::GetExtent(LPSIZE lpSize, DVASPECT nDrawAspect) {
    if (!m_lpObject || !lpSize) return FALSE;
    SIZEL sizel = {};
    HRESULT hr = m_lpObject->GetExtent(nDrawAspect, &sizel);
    if (FAILED(hr)) return FALSE;
    lpSize->cx = sizel.cx;
    lpSize->cy = sizel.cy;
    return TRUE;
}

BOOL COleClientItem::SetExtent(const CSize& size, DVASPECT nDrawAspect) {
    if (!m_lpObject) return FALSE;
    SIZEL sizel = { size.cx, size.cy };
    return SUCCEEDED(m_lpObject->SetExtent(nDrawAspect, &sizel));
}

void COleClientItem::GetClassID(CLSID* pClassID) {
    if (!pClassID) return;
    *pClassID = CLSID_NULL;
    if (m_lpObject) m_lpObject->GetUserClassID(pClassID);
}

void COleClientItem::CopyToClipboard(BOOL bIncludeLink) {
    (void)bIncludeLink;
    if (!m_lpObject) return;
    IDataObject* dataObject = nullptr;
    if (SUCCEEDED(m_lpObject->QueryInterface(IID_IDataObject, reinterpret_cast<void**>(&dataObject)))) {
        OleSetClipboard(dataObject);
        dataObject->Release();
    }
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
    if (!m_lpInPlaceObject) return FALSE;
    RECT rect = { rectPos.left, rectPos.top, rectPos.right, rectPos.bottom };
    return SUCCEEDED(m_lpInPlaceObject->SetObjectRects(&rect, &rect));
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

int COleObjectFactory::RegisterAll() {
    return 0;
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
    BYTE params[2] = { static_cast<BYTE>(vtProp), 0 };
    va_list args;
    va_start(args, vtProp);
    InvokeHelperV(dwDispID, DISPATCH_PROPERTYPUT, VT_EMPTY, nullptr,
                  params, args);
    va_end(args);
}

void COleControlSite::GetProperty(DISPID dwDispID, VARTYPE vtProp, void* pvProp) {
    InvokeHelper(dwDispID, DISPATCH_PROPERTYGET, vtProp, pvProp, nullptr);
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
    if (!m_lpDispatch) return E_POINTER;

    int cParams = CountDispatchParams(pbParamInfo);
    std::vector<VARIANTARG> params(static_cast<size_t>(cParams));
    for (int i = 0; i < cParams; ++i) {
        HRESULT hr = MakeDispatchVariant(static_cast<VARTYPE>(pbParamInfo[i]), &argList, &params[static_cast<size_t>(i)]);
        if (FAILED(hr)) {
            for (int j = 0; j < i; ++j) VariantClear(&params[static_cast<size_t>(j)]);
            return hr;
        }
    }

    std::vector<VARIANTARG> reversed(static_cast<size_t>(cParams));
    for (int i = 0; i < cParams; ++i) {
        reversed[static_cast<size_t>(i)] = params[static_cast<size_t>(cParams - i - 1)];
    }

    DISPPARAMS dispParams = {};
    dispParams.cArgs = cParams;
    dispParams.rgvarg = cParams ? reversed.data() : nullptr;
    DISPID dispidNamed = DISPID_PROPERTYPUT;
    if (wFlags & (DISPATCH_PROPERTYPUT | DISPATCH_PROPERTYPUTREF)) {
        dispParams.cNamedArgs = 1;
        dispParams.rgdispidNamedArgs = &dispidNamed;
    }

    VARIANT result;
    VariantInit(&result);
    EXCEPINFO excep = {};
    UINT argErr = 0;
    HRESULT hr = m_lpDispatch->Invoke(dwDispID, IID_NULL, LOCALE_USER_DEFAULT, wFlags,
                                      &dispParams, &result, &excep, &argErr);

    for (int i = 0; i < cParams; ++i) {
        reversed[static_cast<size_t>(i)].vt = VT_EMPTY;
        VariantClear(&params[static_cast<size_t>(i)]);
    }

    if (FAILED(hr)) {
        if (excep.bstrDescription) SysFreeString(excep.bstrDescription);
        if (excep.bstrSource) SysFreeString(excep.bstrSource);
        if (excep.bstrHelpFile) SysFreeString(excep.bstrHelpFile);
        VariantClear(&result);
        return hr;
    }

    hr = CopyDispatchResult(vtRet, pvRet, &result);
    VariantClear(&result);
    return hr;
}

BOOL COleControlSite::GetAmbientProperty(DISPID dwDispid, VARTYPE vtProp, void* pvProp) {
    IDispatch* ambient = m_lpDispatch;
    if (!ambient) return FALSE;

    return SUCCEEDED(InvokeHelper(dwDispid, DISPATCH_PROPERTYGET, vtProp, pvProp, nullptr));
}

long COleControlSite::GetWindow(HWND__** phWnd) {
    if (!phWnd) return E_POINTER;
    *phWnd = m_hWnd;
    return m_hWnd ? S_OK : E_FAIL;
}

CWnd* COleControlSite::GetWindow() const {
    return CWnd::FromHandle(m_hWnd);
}

long COleControlSite::GetContainer(IOleContainer** ppContainer) {
    if (!ppContainer) return E_POINTER;
    *ppContainer = nullptr;
    return E_NOINTERFACE;
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

DWORD COleControlSiteOrWnd::GetStyle() const {
    if (!m_pWnd || !m_pWnd->GetSafeHwnd()) {
        return 0;
    }
    return static_cast<DWORD>(GetWindowLongPtrW(m_pWnd->GetSafeHwnd(), GWL_STYLE));
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

int COleDialog::MapResult(UINT nResult) {
    switch (nResult) {
    case OLEUI_OK:
        return IDOK;
    case OLEUI_CANCEL:
        return IDCANCEL;
    default:
        return static_cast<int>(nResult);
    }
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

int COleChangeIconDialog::DoChangeIcon(COleClientItem* pItem) {
    if (pItem) {
        m_pItem = pItem;
    }
    return DoModal() == IDOK;
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

void COleChangeSourceDialog::PreInitDialog() {
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

int COleConvertDialog::DoConvert(COleClientItem* pItem) {
    if (pItem) {
        m_pItem = pItem;
    }
    return DoModal() == IDOK;
}

UINT COleConvertDialog::GetSelectionType() const {
    if (m_cv.dwFlags & CF_SELECTACTIVATEAS) {
        return CF_SELECTACTIVATEAS;
    }
    if (m_cv.dwFlags & CF_SELECTCONVERTTO) {
        return CF_SELECTCONVERTTO;
    }
    return 0;
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

__int64 COleInsertDialog::DoModal() {
    return DoModal(0UL);
}

__int64 COleInsertDialog::DoModal(unsigned long) {
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

int COleInsertDialog::CreateItem(COleClientItem* pItem) {
    (void)pItem;
    return 0;
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

int COlePasteSpecialDialog::CreateItem(COleClientItem* pItem) {
    (void)pItem;
    return 0;
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

COleControl::CControlDataSource::CControlDataSource(COleControl* pCtrl) {
    (void)pCtrl;
}

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

//=============================================================================
// COleClientItem - additional methods
//=============================================================================
void COleClientItem::Serialize(CArchive& ar) { (void)ar; }
void COleClientItem::AssertValid() const {}
void COleClientItem::OnShowItem() {}
void COleClientItem::OnOpen() {}
void COleClientItem::OnClose() {}
BOOL COleClientItem::OnShowControlBars(CFrameWnd* pFrameWnd, BOOL bShow) { (void)pFrameWnd; (void)bShow; return TRUE; }
HGLOBAL COleClientItem::GetIconicMetafile() { return nullptr; }
BOOL COleClientItem::SetIconicMetafile(HGLOBAL hMetaPict) { (void)hMetaPict; return FALSE; }
HGLOBAL COleClientItem::GetMetaFile() { return nullptr; }
void COleClientItem::SetHostNames(const wchar_t* lpszHost, const wchar_t* lpszHostObj) { (void)lpszHost; (void)lpszHostObj; }
BOOL COleClientItem::ConvertTo(REFCLSID clsidNew) {
    if (!m_lpObject) return FALSE;
    CLSID clsidOld = CLSID_NULL;
    if (FAILED(m_lpObject->GetUserClassID(&clsidOld))) return FALSE;
    return SUCCEEDED(CoTreatAsClass(clsidOld, clsidNew));
}
BOOL COleClientItem::ActivateAs(REFCLSID clsidNew, REFCLSID clsidOld) {
    return SUCCEEDED(CoTreatAsClass(clsidOld, clsidNew));
}
BOOL COleClientItem::Reload() {
    IPersistStorage* persist = nullptr;
    if (!m_lpObject || FAILED(m_lpObject->QueryInterface(IID_IPersistStorage, reinterpret_cast<void**>(&persist)))) {
        return FALSE;
    }
    HRESULT hr = persist->HandsOffStorage();
    persist->Release();
    return SUCCEEDED(hr);
}
void COleClientItem::UpdateLink() { if (m_lpLink) m_lpLink->Update(nullptr); }
BOOL COleClientItem::IsLinkUpToDate() const {
    if (!m_lpObject) return TRUE;
    return m_lpObject->IsUpToDate() == S_OK;
}
BOOL COleClientItem::CanActivate() { return m_lpObject != nullptr; }
BOOL COleClientItem::IsOpen() const { return m_nStatus == OLE_OPEN; }
BOOL COleClientItem::IsRunning() const { return m_nStatus == OLE_RUNNING; }
HRESULT COleClientItem::EnumVerbs(IEnumOLEVERB** ppEnumOleVerb) {
    if (!ppEnumOleVerb) return E_POINTER;
    *ppEnumOleVerb = nullptr;
    if (!m_lpObject) return OLE_E_NOTRUNNING;
    CLSID clsid = CLSID_NULL;
    HRESULT hr = m_lpObject->GetUserClassID(&clsid);
    if (FAILED(hr)) return hr;
    return OleRegEnumVerbs(clsid, ppEnumOleVerb);
}
LONG COleClientItem::GetActiveVerb() const {
    ClientItemState* state = GetClientItemState(const_cast<COleClientItem*>(this), false);
    return state ? state->activeVerb : OLEIVERB_PRIMARY;
}

void COleClientItem::SetActiveVerb(LONG nVerb) {
    ClientItemState* state = GetClientItemState(this, true);
    if (state) state->activeVerb = nVerb;
}

BOOL COleClientItem::IsModified() const {
    ClientItemState* state = GetClientItemState(const_cast<COleClientItem*>(this), false);
    return state ? state->modified : FALSE;
}

void COleClientItem::SetModifiedFlag(BOOL bModified) {
    ClientItemState* state = GetClientItemState(this, true);
    if (state) state->modified = bModified;
}
void COleClientItem::AttachDataObject(COleDataObject& dataObject) const { (void)dataObject; }

//=============================================================================
// COleControl - additional methods
//=============================================================================
BOOL COleControl::VerifyUserLicense() { return TRUE; }
BOOL COleControl::VerifyLicenseKey(BSTR bstrKey) { (void)bstrKey; return TRUE; }
BOOL COleControl::SetLicenseKey(const wchar_t* lpszLicenseKey) { (void)lpszLicenseKey; return TRUE; }
void COleControl::DoDataExchange(void* pDX) { (void)pDX; }
void COleControl::OnResetState() {}
DWORD COleControl::GetControlFlags() { return 0; }
BOOL COleControl::OnSetExtent(DVASPECT dwDrawAspect, const SIZE& size) { (void)dwDrawAspect; (void)size; return TRUE; }
BOOL COleControl::OnGetExtent(DVASPECT dwDrawAspect, SIZE& size) { (void)dwDrawAspect; size.cx = 0; size.cy = 0; return TRUE; }
BOOL COleControl::OnMapPropertyToPage(DISPID dispid, CLSID* pclsid, BOOL* pbPageOptional) { (void)dispid; (void)pclsid; (void)pbPageOptional; return FALSE; }

COLORREF COleControl::AmbientBackColor() { COLORREF cr = RGB(255,255,255); GetAmbientProperty(DISPID_AMBIENT_BACKCOLOR, VT_COLOR, &cr); return cr; }
COLORREF COleControl::AmbientForeColor() { COLORREF cr = RGB(0,0,0); GetAmbientProperty(DISPID_AMBIENT_FORECOLOR, VT_COLOR, &cr); return cr; }
COLORREF COleControl::AmbientAppearance() { return RGB(255,255,255); }
OLE_COLOR COleControl::AmbientBackColorOle() { return (OLE_COLOR)AmbientBackColor(); }
OLE_COLOR COleControl::AmbientForeColorOle() { return (OLE_COLOR)AmbientForeColor(); }
IFontDisp* COleControl::AmbientFont() { IDispatch* p = nullptr; GetAmbientProperty(DISPID_AMBIENT_FONT, VT_DISPATCH, &p); return (IFontDisp*)p; }
IDispatch* COleControl::AmbientFontDisp() { IDispatch* p = nullptr; GetAmbientProperty(DISPID_AMBIENT_FONT, VT_DISPATCH, &p); return p; }
short COleControl::AmbientTextAlign() { return 0; }
BOOL COleControl::AmbientUserMode() { BOOL b = TRUE; GetAmbientProperty(DISPID_AMBIENT_USERMODE, VT_BOOL, &b); return b; }
BOOL COleControl::AmbientUIDead() { return FALSE; }
BOOL COleControl::AmbientShowGrabHandles() { return TRUE; }
BOOL COleControl::AmbientShowHatching() { return TRUE; }
CString COleControl::AmbientDisplayName() { return CString(); }
BOOL COleControl::AmbientDisplayAsDefault() { return FALSE; }
BOOL COleControl::AmbientAutoClip() { return TRUE; }
BOOL COleControl::AmbientSupportsMnemonics() { return TRUE; }
CString COleControl::AmbientScaleUnits() { return CString(); }
unsigned long COleControl::AmbientLocaleID() { return 0; }

void COleControl::FireClick() {}
void COleControl::FireDblClick() {}
void COleControl::FireKeyDown(USHORT* pnChar, short nShiftState) { (void)pnChar; (void)nShiftState; }
void COleControl::FireKeyPress(USHORT* pnChar) { (void)pnChar; }
void COleControl::FireKeyUp(USHORT* pnChar, short nShiftState) { (void)pnChar; (void)nShiftState; }
void COleControl::FireMouseDown(short nButton, short nShiftState, long x, long y) { (void)nButton; (void)nShiftState; (void)x; (void)y; }
void COleControl::FireMouseMove(short nButton, short nShiftState, long x, long y) { (void)nButton; (void)nShiftState; (void)x; (void)y; }
void COleControl::FireMouseUp(short nButton, short nShiftState, long x, long y) { (void)nButton; (void)nShiftState; (void)x; (void)y; }
void COleControl::FireReadyStateChange() {}

COLORREF COleControl::GetBackColor() const { return RGB(255,255,255); }
void COleControl::SetBackColor(COLORREF clr) { (void)clr; }
COLORREF COleControl::GetForeColor() const { return RGB(0,0,0); }
void COleControl::SetForeColor(COLORREF clr) { (void)clr; }
BOOL COleControl::GetEnabled() const { return TRUE; }
void COleControl::SetEnabled(BOOL bEnabled) { (void)bEnabled; }
void COleControl::SetFont(LPFONTDISP pFontDisp) { (void)pFontDisp; }
void COleControl::SetFont(CFont* pFont) { (void)pFont; }
unsigned int COleControl::GetHwnd() { return (unsigned int)(uintptr_t)m_hWnd; }
void COleControl::SetHwnd(HWND hWnd) { m_hWnd = hWnd; }
OLE_COLOR COleControl::GetBackColorOle() const { return (OLE_COLOR)GetBackColor(); }
OLE_COLOR COleControl::GetForeColorOle() const { return (OLE_COLOR)GetForeColor(); }
void COleControl::SetBackColorOle(OLE_COLOR clr) { SetBackColor((COLORREF)clr); }
void COleControl::SetForeColorOle(OLE_COLOR clr) { SetForeColor((COLORREF)clr); }
short COleControl::GetAppearance() const { return 0; }
void COleControl::SetAppearance(short nAppearance) { (void)nAppearance; }
short COleControl::GetBorderStyle() const { return 0; }
void COleControl::SetBorderStyle(short nBorderStyle) { (void)nBorderStyle; }
CString COleControl::GetText() const { return CString(); }
void COleControl::SetText(const wchar_t* lpszText) { (void)lpszText; }
void COleControl::GetText(CString& strText) const { strText = CString(); }
long COleControl::GetReadyState() const { return 4; }

BOOL COleControl::IsSubclassedControl() { return FALSE; }
void COleControl::SetModifiedFlag(BOOL bModified) { (void)bModified; }
BOOL COleControl::GetModifiedFlag() const { return FALSE; }
ULONG COleControl::InternalAddRef() { return 1; }
ULONG COleControl::InternalRelease() { return 1; }
ULONG COleControl::InternalQueryInterface(REFIID riid, void** ppv) { (void)riid; *ppv = nullptr; return E_NOINTERFACE; }
void COleControl::GetControlSize(int* pCX, int* pCY) { if(pCX) *pCX = 0; if(pCY) *pCY = 0; }
void COleControl::SetControlSize(int cx, int cy) { (void)cx; (void)cy; }
void COleControl::OnSetClientSite() {}
void COleControl::OnGetControlInfo(LPCONTROLINFO pControlInfo) { (void)pControlInfo; }
BOOL COleControl::OnMnemonic(LPMSG pMsg) { (void)pMsg; return FALSE; }
void COleControl::OnAmbientPropertyChange(DISPID dispid) { (void)dispid; }
void COleControl::BoundPropertyChanged(DISPID dispid) { (void)dispid; }
void COleControl::BoundPropertyRequestEdit(DISPID dispid) { (void)dispid; }
void COleControl::InvalidateControl(LPCRECT lpRect) { (void)lpRect; }
int COleControl::OnProperties(MSG* pMsg, HWND hWnd, const RECT* lpRect) { (void)pMsg; (void)hWnd; (void)lpRect; return 0; }
void COleControl::ShowPropertyPages() {}
int COleControl::GetPropertyPageCount() const { return 0; }
BOOL COleControl::IsPropertyPage(LPUNKNOWN lpUnk) { (void)lpUnk; return FALSE; }
BOOL COleControl::CanCreateConnectionPoints() { return FALSE; }
void COleControl::EnableConnectionPoints() {}
BOOL COleControl::IsConnectionPointEnabled(REFIID riid) { (void)riid; return FALSE; }
void COleControl::FirePropChanged(DISPID dispid) { (void)dispid; }
BOOL COleControl::PreTranslateMessage(MSG* pMsg) { (void)pMsg; return FALSE; }
LONG COleControl::OnPosRectChange(LPCRECT lprcPosRect) { (void)lprcPosRect; return 0; }
BOOL COleControl::OnSetObjectRects(LPCRECT lprcPosRect, LPCRECT lprcClipRect) { (void)lprcPosRect; (void)lprcClipRect; return TRUE; }
void COleControl::OnClose(DWORD dwSaveOption) { (void)dwSaveOption; }
void COleControl::SetCapture() { if(m_hWnd) ::SetCapture(m_hWnd); }
void COleControl::ReleaseCapture() { ::ReleaseCapture(); }
void COleControl::BringWindowToTop() { if(m_hWnd) ::BringWindowToTop(m_hWnd); }
void COleControl::MoveWindow(int X, int Y, int nWidth, int nHeight, BOOL bRepaint) { if(m_hWnd) ::MoveWindow(m_hWnd, X, Y, nWidth, nHeight, bRepaint); }
void COleControl::MoveWindow(LPCRECT lpRect, BOOL bRepaint) { if(m_hWnd && lpRect) ::MoveWindow(m_hWnd, lpRect->left, lpRect->top, lpRect->right-lpRect->left, lpRect->bottom-lpRect->top, bRepaint); }

//=============================================================================
// COleDocObjectItem
//=============================================================================
IMPLEMENT_DYNAMIC(COleDocObjectItem, COleClientItem)

COleDocObjectItem::COleDocObjectItem(COleDocument* pContainerDoc) : COleClientItem(pContainerDoc) { memset(_coledocobjectitem_padding, 0, sizeof(_coledocobjectitem_padding)); }
COleDocObjectItem::~COleDocObjectItem() {}
BOOL COleDocObjectItem::IsDocObject() const { return FALSE; }
BOOL COleDocObjectItem::IsActive() const { return m_bInPlaceActive && IsOpen(); }
IOleDocumentView* COleDocObjectItem::GetActiveView() const { return nullptr; }
HRESULT COleDocObjectItem::GetDocument(IUnknown** ppDocument) { *ppDocument = nullptr; return E_NOTIMPL; }
void COleDocObjectItem::ActivateAndShow() {}
BOOL COleDocObjectItem::IsOpen() const { return m_nStatus == OLE_OPEN; }
void COleDocObjectItem::OnActivateView() {}
BOOL COleDocObjectItem::OnPreparePrinting(void* pInfo) { (void)pInfo; return TRUE; }
void COleDocObjectItem::OnBeginPrinting(CDC* pDC, void* pInfo) { (void)pDC; (void)pInfo; }
void COleDocObjectItem::OnPrint(CDC* pDC, void* pInfo) { (void)pDC; (void)pInfo; }
void COleDocObjectItem::OnEndPrinting(CDC* pDC, void* pInfo) { (void)pDC; (void)pInfo; }
HRESULT COleDocObjectItem::ExecCommand(DWORD nCmdID, DWORD nCmdExecOpt, const GUID* pguidCmdGroup) { (void)nCmdID; (void)nCmdExecOpt; (void)pguidCmdGroup; return E_NOTIMPL; }

//=============================================================================
// CEnumFormatEtc
//=============================================================================
CEnumFormatEtc::CEnumFormatEtc() : m_refCount(1), m_formats(nullptr), m_count(0), m_capacity(0), m_position(0) { memset(_cenumformatetc_padding, 0, sizeof(_cenumformatetc_padding)); }
CEnumFormatEtc::~CEnumFormatEtc() { if (m_formats) free(m_formats); }

STDMETHODIMP CEnumFormatEtc::QueryInterface(REFIID riid, void** ppv) {
    if (riid == IID_IUnknown || riid == IID_IEnumFORMATETC) { *ppv = this; AddRef(); return S_OK; }
    *ppv = nullptr; return E_NOINTERFACE;
}
STDMETHODIMP_(ULONG) CEnumFormatEtc::AddRef() { return ++m_refCount; }
STDMETHODIMP_(ULONG) CEnumFormatEtc::Release() {
    ULONG ref = --m_refCount;
    if (ref == 0) delete this;
    return ref;
}
STDMETHODIMP CEnumFormatEtc::Next(ULONG celt, FORMATETC* rgelt, ULONG* pceltFetched) {
    ULONG fetched = 0;
    while (m_position < m_count && fetched < celt) {
        rgelt[fetched] = m_formats[m_position];
        if (rgelt[fetched].ptd) rgelt[fetched].ptd = nullptr; // Don't copy DVTARGETDEVICE
        m_position++;
        fetched++;
    }
    if (pceltFetched) *pceltFetched = fetched;
    return (fetched == celt) ? S_OK : S_FALSE;
}
STDMETHODIMP CEnumFormatEtc::Skip(ULONG celt) {
    if (m_position + celt > m_count) { m_position = m_count; return S_FALSE; }
    m_position += celt;
    return S_OK;
}
STDMETHODIMP CEnumFormatEtc::Reset() { m_position = 0; return S_OK; }
STDMETHODIMP CEnumFormatEtc::Clone(IEnumFORMATETC** ppEnum) {
    if (!ppEnum) return E_POINTER;
    CEnumFormatEtc* clone = new CEnumFormatEtc();
    for (ULONG i = 0; i < m_count; ++i) {
        clone->AddFormat(m_formats[i]);
    }
    clone->m_position = m_position;
    *ppEnum = clone;
    return S_OK;
}

void CEnumFormatEtc::AddFormat(const FORMATETC& formatEtc) {
    if (m_count >= m_capacity) {
        m_capacity = m_capacity ? m_capacity * 2 : 8;
        m_formats = (FORMATETC*)realloc(m_formats, m_capacity * sizeof(FORMATETC));
    }
    if (m_formats) m_formats[m_count++] = formatEtc;
}
