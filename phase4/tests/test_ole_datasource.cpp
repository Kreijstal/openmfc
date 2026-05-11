// Focused coverage for COleDataSource's IDataObject implementation.
#include "openmfc/afxole.h"

#include <cstdio>
#include <cstring>

static HGLOBAL MakeTextGlobal(const char* text) {
    const SIZE_T size = strlen(text) + 1;
    HGLOBAL h = GlobalAlloc(GMEM_MOVEABLE, size);
    if (!h) return nullptr;
    void* p = GlobalLock(h);
    memcpy(p, text, size);
    GlobalUnlock(h);
    return h;
}

static bool CheckGlobalText(HGLOBAL h, const char* expected) {
    if (!h) return false;
    const char* p = static_cast<const char*>(GlobalLock(h));
    if (!p) return false;
    const bool ok = strcmp(p, expected) == 0;
    GlobalUnlock(h);
    return ok;
}

int main() {
    COleDataSource source;
    HGLOBAL sourceData = MakeTextGlobal("openmfc-data");
    if (!sourceData) {
        std::printf("FAIL: GlobalAlloc failed\n");
        return 1;
    }

    source.CacheGlobalData(CF_TEXT, sourceData);
    GlobalFree(sourceData);

    IDataObject* dataObject = source.GetInterface(TRUE);
    if (!dataObject) {
        std::printf("FAIL: GetInterface returned null\n");
        return 1;
    }

    FORMATETC fmt = {};
    fmt.cfFormat = CF_TEXT;
    fmt.dwAspect = DVASPECT_CONTENT;
    fmt.lindex = -1;
    fmt.tymed = TYMED_HGLOBAL;

    if (dataObject->QueryGetData(&fmt) != S_OK) {
        std::printf("FAIL: QueryGetData did not find cached CF_TEXT\n");
        dataObject->Release();
        return 1;
    }

    STGMEDIUM medium = {};
    if (dataObject->GetData(&fmt, &medium) != S_OK || medium.tymed != TYMED_HGLOBAL) {
        std::printf("FAIL: GetData did not return HGLOBAL data\n");
        dataObject->Release();
        return 1;
    }
    if (!CheckGlobalText(medium.hGlobal, "openmfc-data")) {
        std::printf("FAIL: copied HGLOBAL contents mismatch\n");
        ReleaseStgMedium(&medium);
        dataObject->Release();
        return 1;
    }
    ReleaseStgMedium(&medium);

    IEnumFORMATETC* enumFormats = nullptr;
    if (dataObject->EnumFormatEtc(DATADIR_GET, &enumFormats) != S_OK || !enumFormats) {
        std::printf("FAIL: EnumFormatEtc failed\n");
        dataObject->Release();
        return 1;
    }

    FORMATETC got = {};
    ULONG fetched = 0;
    if (enumFormats->Next(1, &got, &fetched) != S_OK || fetched != 1 || got.cfFormat != CF_TEXT) {
        std::printf("FAIL: first enumerated format mismatch\n");
        enumFormats->Release();
        dataObject->Release();
        return 1;
    }

    IEnumFORMATETC* cloned = nullptr;
    enumFormats->Reset();
    if (enumFormats->Clone(&cloned) != S_OK || !cloned) {
        std::printf("FAIL: Clone failed\n");
        enumFormats->Release();
        dataObject->Release();
        return 1;
    }
    memset(&got, 0, sizeof(got));
    fetched = 0;
    if (cloned->Next(1, &got, &fetched) != S_OK || got.cfFormat != CF_TEXT) {
        std::printf("FAIL: cloned enumerator did not preserve formats\n");
        cloned->Release();
        enumFormats->Release();
        dataObject->Release();
        return 1;
    }

    cloned->Release();
    enumFormats->Release();
    dataObject->Release();

    std::printf("OK: COleDataSource IDataObject tests passed\n");
    return 0;
}
