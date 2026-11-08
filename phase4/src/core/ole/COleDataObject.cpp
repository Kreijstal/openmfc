// COleDataObject — OpenMFC implementation.
// Sources: olecore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/OlecoreSupport.h"

void COleDataObject::BeginEnumFormats() {
    EnsureClipboardObject();
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
    ULONG fetched = 0;
    return m_lpEnumFmtEtc->Next(1, lpFormatEtc, &fetched) == S_OK && fetched == 1;
}
HGLOBAL COleDataObject::GetGlobalData(CLIPFORMAT cfFormat, FORMATETC* lpFormatEtc) {
    EnsureClipboardObject();
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
    EnsureClipboardObject();
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
    ReleaseStgMedium(&stg);

    if (filePath.IsEmpty()) {
        return nullptr;
    }

    auto* result = new CFile(filePath, CFile::modeRead | CFile::shareDenyNone | CFile::typeBinary);
    if (result == nullptr || result->m_hFile == INVALID_HANDLE_VALUE) {
        if (result) {
            result->Close();
            delete result;
        }
        return nullptr;
    }
    return result;
}
