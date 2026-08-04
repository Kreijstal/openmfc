// COleDataSource — OpenMFC implementation.
// Sources: olecore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/OlecoreSupport.h"

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
    if (phGlobal) *phGlobal = nullptr;
    if (!lpFormatEtc || !phGlobal) return FALSE;
    DataCacheState* state = GetDataCacheState(this, false);
    DataCacheEntry* entry = FindCacheEntry(state, *lpFormatEtc);
    if (!entry || !entry->hasMedium || entry->medium.tymed != TYMED_HGLOBAL) return FALSE;
    *phGlobal = DuplicateGlobalMemory(entry->medium.hGlobal);
    return *phGlobal != nullptr;
}
int COleDataSource::OnRenderFileData(FORMATETC* lpFormatEtc, CFile* pFile) {
    if (!lpFormatEtc || !pFile) return FALSE;
    DataCacheState* state = GetDataCacheState(this, false);
    DataCacheEntry* entry = FindCacheEntry(state, *lpFormatEtc);
    if (!entry || !entry->hasMedium) return FALSE;

    if (entry->medium.tymed == TYMED_HGLOBAL && entry->medium.hGlobal) {
        SIZE_T size = GlobalSize(entry->medium.hGlobal);
        void* data = GlobalLock(entry->medium.hGlobal);
        if (!data) return FALSE;
        pFile->Write(data, static_cast<UINT>(size));
        GlobalUnlock(entry->medium.hGlobal);
        return TRUE;
    }
    if (entry->medium.tymed == TYMED_FILE && entry->medium.lpszFileName) {
        CFile source(entry->medium.lpszFileName, CFile::modeRead | CFile::shareDenyNone | CFile::typeBinary);
        if (source.m_hFile == INVALID_HANDLE_VALUE) {
            return FALSE;
        }
        BYTE buffer[4096];
        UINT read = 0;
        while ((read = source.Read(buffer, sizeof(buffer))) > 0) {
            pFile->Write(buffer, read);
        }
        return TRUE;
    }
    return FALSE;
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
