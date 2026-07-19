// OpenMFC: COleStreamFile — IStream-lifecycle methods (ole32-backed).
//
// COleStreamFile : public CFile, adding `LPSTREAM m_lpStream`. The repo layout
// (verified at runtime) is: sizeof(CFile)=24, m_lpStream @ offset 24,
// sizeof(COleStreamFile)=64 (24 base + 8 ptr + 32 padding). olecore.cpp already
// owns GetStream/Attach/Detach/Read/Write/Seek/LockRange/UnlockRange and
// thunks.cpp their impl__ thunks; this TU adds the remaining lifecycle methods
// that drive the underlying IStream directly. Each is a distinct mangled symbol.
//
// All methods operate on m_lpStream via a layout-faithful view, so they need no
// header changes and link/run standalone (the test exercises them under Wine
// against a real CreateStreamOnHGlobal / StgCreateDocfile stream).
//
// Deliberately NOT implemented here (left as honest weak stubs):
// GetRuntimeClass/GetThisClass (the repo's CFile family is intentionally
// non-CObject and returns null RTTI).
//
// Duplicate, GetStatus and GetStorageName WERE listed here as unimplemented,
// but the code below implements all three. Their `// Symbol:` markers carried
// 32-bit mangling (PAV/QBA/UBA) where retail exports the 64-bit form, so the
// .def aliased each export to a correctly-mangled impl__ name that only the
// stub generator defined -- the export returned 0 and this code was
// unreachable. Markers and impl__ names corrected; the header now matches.

#include <windows.h>
#include <objidl.h>
#include <cstddef>   // offsetof — llvm-mingw/clang doesn't pull it in via <windows.h>
#include <cstring>
#include "openmfc/afxole.h"

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

namespace {

// Layout-faithful view of COleStreamFile: m_lpStream lives at offset 24.
struct OleStreamFileView {
    char     cfile_base[24];   // CFile subobject (vptr + handle + CString)
    IStream* m_lpStream;       // offset 24
    char     pad[32];          // trailing padding (sizeof == 64)
};
static_assert(sizeof(OleStreamFileView) == 64, "COleStreamFile must be 64 bytes");
static_assert(offsetof(OleStreamFileView, m_lpStream) == 24, "m_lpStream @ 24");

inline IStream*& Stream(void* pThis) {
    return reinterpret_cast<OleStreamFileView*>(pThis)->m_lpStream;
}

} // namespace

// ?Abort@COleStreamFile@@UEAAXXZ
// void Abort() — release the stream without throwing; safe on a closed file.
// Symbol: ?Abort@COleStreamFile@@UEAAXXZ
extern "C" void MS_ABI impl__Abort_COleStreamFile__UEAAXXZ(void* pThis)
{
    if (!pThis) return;
    IStream*& s = Stream(pThis);
    if (s) { s->Release(); s = nullptr; }
}

// ?Close@COleStreamFile@@UEAAXXZ
// void Close() — release the stream and detach.
// Symbol: ?Close@COleStreamFile@@UEAAXXZ
extern "C" void MS_ABI impl__Close_COleStreamFile__UEAAXXZ(void* pThis)
{
    if (!pThis) return;
    IStream*& s = Stream(pThis);
    if (s) { s->Release(); s = nullptr; }
}

// ?Flush@COleStreamFile@@UEAAXXZ
// void Flush() — commit buffered changes to the underlying storage.
// Symbol: ?Flush@COleStreamFile@@UEAAXXZ
extern "C" void MS_ABI impl__Flush_COleStreamFile__UEAAXXZ(void* pThis)
{
    if (!pThis) return;
    IStream* s = Stream(pThis);
    if (s) s->Commit(STGC_DEFAULT);
}

// ?GetLength@COleStreamFile@@UEBA_KXZ
// ULONGLONG GetLength() const — total byte size of the stream.
// Symbol: ?GetLength@COleStreamFile@@UEBA_KXZ
extern "C" unsigned long long MS_ABI impl__GetLength_COleStreamFile__UEBA_KXZ(const void* pThis)
{
    if (!pThis) return 0;
    IStream* s = Stream(const_cast<void*>(pThis));
    if (!s) return 0;
    STATSTG stat;
    if (FAILED(s->Stat(&stat, STATFLAG_NONAME))) return 0;
    return stat.cbSize.QuadPart;
}

// ?GetPosition@COleStreamFile@@UEBA_KXZ
// ULONGLONG GetPosition() const — current seek position.
// Symbol: ?GetPosition@COleStreamFile@@UEBA_KXZ
extern "C" unsigned long long MS_ABI impl__GetPosition_COleStreamFile__UEBA_KXZ(const void* pThis)
{
    if (!pThis) return 0;
    IStream* s = Stream(const_cast<void*>(pThis));
    if (!s) return 0;
    LARGE_INTEGER move; move.QuadPart = 0;
    ULARGE_INTEGER pos; pos.QuadPart = 0;
    if (FAILED(s->Seek(move, STREAM_SEEK_CUR, &pos))) return 0;
    return pos.QuadPart;
}

// ?SetLength@COleStreamFile@@UEAAX_K@Z
// void SetLength(ULONGLONG dwNewLen) — resize the stream.
// Symbol: ?SetLength@COleStreamFile@@UEAAX_K@Z
extern "C" void MS_ABI impl__SetLength_COleStreamFile__UEAAX_K_Z(
    void* pThis, unsigned long long dwNewLen)
{
    if (!pThis) return;
    IStream* s = Stream(pThis);
    if (!s) return;
    ULARGE_INTEGER size; size.QuadPart = dwNewLen;
    s->SetSize(size);
}

// ?CreateMemoryStream@COleStreamFile@@QEAAHPEAVCFileException@@@Z
// BOOL CreateMemoryStream(CFileException* pError) — back the file with a new
// HGLOBAL-based memory stream. Returns TRUE on success.
// Symbol: ?CreateMemoryStream@COleStreamFile@@QEAAHPEAVCFileException@@@Z
extern "C" int MS_ABI impl__CreateMemoryStream_COleStreamFile__QEAAHPEAVCFileException___Z(
    void* pThis, void* /*pError*/)
{
    if (!pThis) return 0;
    IStream*& s = Stream(pThis);
    if (s) { s->Release(); s = nullptr; }
    IStream* pNew = nullptr;
    if (FAILED(CreateStreamOnHGlobal(nullptr, TRUE, &pNew)) || !pNew) return 0;
    s = pNew;
    return 1;
}

// ?CreateStream@COleStreamFile@@QEAAHPEAUIStorage@@PEB_WKPEAVCFileException@@@Z
// BOOL CreateStream(IStorage* lpStorage, const wchar_t* lpszName, DWORD nFlags,
//                   CFileException* pError) — create a named stream in a storage.
// Symbol: ?CreateStream@COleStreamFile@@QEAAHPEAUIStorage@@PEB_WKPEAVCFileException@@@Z
extern "C" int MS_ABI impl__CreateStream_COleStreamFile__QEAAHPEAUIStorage__PEB_WKPEAVCFileException___Z(
    void* pThis, IStorage* lpStorage, const wchar_t* lpszName,
    unsigned long nFlags, void* /*pError*/)
{
    if (!pThis || !lpStorage || !lpszName) return 0;
    IStream*& s = Stream(pThis);
    if (s) { s->Release(); s = nullptr; }
    IStream* pNew = nullptr;
    if (FAILED(lpStorage->CreateStream(lpszName, nFlags, 0, 0, &pNew)) || !pNew) return 0;
    s = pNew;
    return 1;
}

// ?OpenStream@COleStreamFile@@QEAAHPEAUIStorage@@PEB_WKPEAVCFileException@@@Z
// BOOL OpenStream(IStorage* lpStorage, const wchar_t* lpszName, DWORD nFlags,
//                 CFileException* pError) — open an existing named stream.
// Symbol: ?OpenStream@COleStreamFile@@QEAAHPEAUIStorage@@PEB_WKPEAVCFileException@@@Z
extern "C" int MS_ABI impl__OpenStream_COleStreamFile__QEAAHPEAUIStorage__PEB_WKPEAVCFileException___Z(
    void* pThis, IStorage* lpStorage, const wchar_t* lpszName,
    unsigned long nFlags, void* /*pError*/)
{
    if (!pThis || !lpStorage || !lpszName) return 0;
    IStream*& s = Stream(pThis);
    if (s) { s->Release(); s = nullptr; }
    IStream* pNew = nullptr;
    if (FAILED(lpStorage->OpenStream(lpszName, nullptr, nFlags, 0, &pNew)) || !pNew) return 0;
    s = pNew;
    return 1;
}

// ?Duplicate@COleStreamFile@@UEBAPEAVCFile@@XZ
// CFile* Duplicate() const — clone the underlying IStream and return a new wrapper.
// Symbol: ?Duplicate@COleStreamFile@@UEBAPEAVCFile@@XZ
extern "C" void* MS_ABI impl__Duplicate_COleStreamFile__UEBAPEAVCFile__XZ(const void* pThis)
{
    if (!pThis) return nullptr;

    COleStreamFile* pDup = new COleStreamFile();
    if (!pDup) return nullptr;

    IStream* source = Stream(const_cast<void*>(pThis));
    if (!source) return pDup;

    IStream* pClone = nullptr;
    if (SUCCEEDED(source->Clone(&pClone)) && pClone) {
        pDup->Attach(pClone);
    }
    return pDup;
}

// ?GetStatus@COleStreamFile@@QEBAHAEAUCFileStatus@@@Z
// int GetStatus(CFileStatus& rStatus) const — fill status from STATSTG.
// Symbol: ?GetStatus@COleStreamFile@@QEBAHAEAUCFileStatus@@@Z
extern "C" int MS_ABI impl__GetStatus_COleStreamFile__QEBAHAEAUCFileStatus___Z(
    const void* pThis, CFileStatus* pStatus)
{
    if (!pThis || !pStatus) return 0;
    std::memset(pStatus, 0, sizeof(*pStatus));

    IStream* stream = Stream(const_cast<void*>(pThis));
    if (!stream) return 0;

    STATSTG stat;
    if (FAILED(stream->Stat(&stat, STATFLAG_DEFAULT))) return 0;

    ULARGE_INTEGER ul;
    ul.LowPart = stat.cbSize.LowPart;
    ul.HighPart = stat.cbSize.HighPart;
    pStatus->m_size = ul.QuadPart;

    ul.LowPart = stat.ctime.dwLowDateTime;
    ul.HighPart = stat.ctime.dwHighDateTime;
    pStatus->m_ctime = ul.QuadPart;

    ul.LowPart = stat.mtime.dwLowDateTime;
    ul.HighPart = stat.mtime.dwHighDateTime;
    pStatus->m_mtime = ul.QuadPart;

    ul.LowPart = stat.atime.dwLowDateTime;
    ul.HighPart = stat.atime.dwHighDateTime;
    pStatus->m_atime = ul.QuadPart;

    if (stat.pwcsName) {
        for (size_t i = 0; i + 1 < _MAX_PATH && stat.pwcsName[i] != L'\0'; ++i) {
            pStatus->m_szFullName[i] = stat.pwcsName[i];
            pStatus->m_szFullName[i + 1] = L'\0';
        }
        CoTaskMemFree(stat.pwcsName);
    }

    return 1;
}

// ?GetStorageName@COleStreamFile@@UEBA?BV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@XZ
// const CStringT<wchar_t,...> GetStorageName() const — return stream storage name.
//
// Returns a class by value, so MSVC x64 passes the hidden result pointer in RCX
// and `this` in RDX: the sret pointer comes FIRST, before pThis. This function
// previously took them the other way round, which went unnoticed because the
// marker was mis-mangled and nothing ever dispatched here. Matches the idiom in
// dlgcommon.cpp / dbcore.cpp / ctrlcore.cpp.
// Symbol: ?GetStorageName@COleStreamFile@@UEBA?BV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@XZ
extern "C" void MS_ABI impl__GetStorageName_COleStreamFile__UEBA_BV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__XZ(
    CString* __ret, const void* pThis)
{
    IStream* stream = pThis ? Stream(const_cast<void*>(pThis)) : nullptr;
    if (!stream) {
        new(__ret) CString();
        return;
    }

    STATSTG stat;
    if (FAILED(stream->Stat(&stat, STATFLAG_DEFAULT))) {
        new(__ret) CString();
        return;
    }

    const wchar_t* storageName = stat.pwcsName ? stat.pwcsName : L"";
    new(__ret) CString(storageName);
    if (stat.pwcsName) {
        CoTaskMemFree(stat.pwcsName);
    }
}
