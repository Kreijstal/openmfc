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
// Deliberately NOT implemented here (left as honest weak stubs): Duplicate
// (needs the COleStreamFile ctor), GetRuntimeClass/GetThisClass (the repo's
// CFile family is intentionally non-CObject and returns null RTTI), GetStatus /
// GetStorageName (CFileStatus coupling / CStringT-by-value return).

#include <windows.h>
#include <objidl.h>
#include <cstddef>   // offsetof — llvm-mingw/clang doesn't pull it in via <windows.h>

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
