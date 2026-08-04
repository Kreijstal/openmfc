// COleDataObject — OpenMFC implementation.
// Sources: olecore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/OlecoreSupport.h"

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
    ULONG fetched = 0;
    return m_lpEnumFmtEtc->Next(1, lpFormatEtc, &fetched) == S_OK && fetched == 1;
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
