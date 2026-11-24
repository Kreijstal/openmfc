// COleDataObject — OpenMFC implementation.
// Sources: olecore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/OlecoreSupport.h"
#include "detail/CSharedFileSupport.h"

#include <memory>

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
namespace {
struct Medium {
    STGMEDIUM value{};
    ~Medium() { ::ReleaseStgMedium(&value); }
};

class DataFile : public CFile {
public:
    ~DataFile() override { CFile::Abort(); }

    BOOL OpenMedium(STGMEDIUM& medium) {
        if (!CFile::Open(medium.lpszFileName, modeRead | shareDenyNone | typeBinary))
            return FALSE;
        m_medium.value = medium;
        medium = {};
        return TRUE;
    }

private:
    Medium m_medium;
};

class DataStreamFile : public COleStreamFile {
public:
    explicit DataStreamFile(IStream* stream) {
        Attach(stream);
        stream->AddRef();
    }
    ~DataStreamFile() override {
        if (auto* stream = Detach()) stream->Release();
    }
};
}

CFile* COleDataObject::GetFileData(CLIPFORMAT cfFormat, FORMATETC* lpFormatEtc) {
    FORMATETC format = {cfFormat, nullptr, DVASPECT_CONTENT, -1,
                        TYMED_HGLOBAL | TYMED_ISTREAM | TYMED_FILE};
    Medium medium;
    if (!GetData(cfFormat, &medium.value, lpFormatEtc ? lpFormatEtc : &format))
        return nullptr;

    auto& data = medium.value;
    if (data.tymed == TYMED_HGLOBAL && data.hGlobal) {
        // A custom release object can own the original block beyond this call.
        auto file = std::make_unique<CSharedFile>(GMEM_MOVEABLE);
        HGLOBAL block = data.hGlobal;
        if (data.pUnkForRelease) {
            block = ::OleDuplicateData(block, lpFormatEtc ? lpFormatEtc->cfFormat : cfFormat, 0);
        } else {
            data.tymed = TYMED_NULL;
        }
        std::unique_ptr<void, decltype(&::GlobalFree)> owned(block, ::GlobalFree);
        if (!block || !::GlobalLock(block)) return nullptr;
        ::GlobalUnlock(block);
        file->SetHandle(block, FALSE);
        owned.release();
        return file.release();
    }
    if (data.tymed == TYMED_ISTREAM && data.pstm) {
        return new DataStreamFile(data.pstm);
    }
    if (data.tymed == TYMED_FILE && data.lpszFileName) {
        auto file = std::make_unique<DataFile>();
        if (file->OpenMedium(data))
            return file.release();
    }
    return nullptr;
}
