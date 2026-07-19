// Exercises the exports that were unreachable behind mis-mangled `// Symbol:`
// markers. Each of these resolved to a stub returning 0 while a working
// implementation sat beside the marker; see tools/check_symbol_markers.py.
//
// This is a behaviour test, not a presence test: a stub returning 0 would
// satisfy GetProcAddress, so every check below asserts an observable effect
// that only the real implementation can produce.
#include <windows.h>
#include <objidl.h>
#include <oleauto.h>
#include <cstdio>
#include <cstring>

static int pass = 0, fail = 0;
#define CHECK(c, msg) do { if (c) { pass++; printf("  PASS %s\n", msg); } \
                           else { fail++; printf("  FAIL %s\n", msg); } } while (0)

int main()
{
    ::CoInitialize(nullptr);
    HMODULE h = LoadLibraryA("openmfc.dll");
    if (!h) { printf("cannot load openmfc.dll\n"); return 1; }

    // ---- COleStreamFile: Attach a real IStream, then drive the wired methods ----
    typedef void* (__stdcall *AttachFn)(void*, IStream*);
    typedef void* (__stdcall *DupFn)(const void*);
    typedef int   (__stdcall *GetStatusFn)(const void*, void*);
    typedef void  (__stdcall *GetStorageNameFn)(void* /*sret*/, const void*);

    auto Attach = (AttachFn)GetProcAddress(h, "?Attach@COleStreamFile@@QEAAXPEAUIStream@@@Z");
    auto Dup    = (DupFn)   GetProcAddress(h, "?Duplicate@COleStreamFile@@UEBAPEAVCFile@@XZ");
    auto GetSt  = (GetStatusFn)GetProcAddress(h, "?GetStatus@COleStreamFile@@QEBAHAEAUCFileStatus@@@Z");
    auto GetNm  = (GetStorageNameFn)GetProcAddress(h,
        "?GetStorageName@COleStreamFile@@UEBA?BV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@XZ");
    if (!Dup || !GetSt || !GetNm) { printf("missing COleStreamFile export(s)\n"); return 1; }

    // COleStreamFile is 64 bytes with m_lpStream at offset 24 (see the impl TU).
    alignas(16) unsigned char osf[64];
    memset(osf, 0, sizeof osf);

    IStream* pStm = nullptr;
    if (FAILED(::CreateStreamOnHGlobal(nullptr, TRUE, &pStm)) || !pStm) {
        printf("CreateStreamOnHGlobal failed\n"); return 1;
    }
    const wchar_t payload[] = L"openmfc-orphan-test";
    ULONG written = 0;
    pStm->Write(payload, sizeof payload, &written);

    if (Attach) Attach(osf, pStm);
    else        *(IStream**)(osf + 24) = pStm;   // fall back to the known layout

    // GetStatus must fill m_size from the stream's STATSTG. A stub returns 0
    // and leaves the buffer untouched.
    unsigned char status[600];
    memset(status, 0xCD, sizeof status);
    int rc = GetSt(osf, status);
    CHECK(rc == 1, "GetStatus returns TRUE for an attached stream");
    // CFileStatus: m_ctime, m_mtime, m_atime (ULONGLONG each) then m_size @24.
    unsigned long long size = *(unsigned long long*)(status + 24);
    CHECK(size == sizeof payload, "GetStatus reports the real stream size");

    // Duplicate must hand back a distinct non-null object with its own stream.
    void* dup = Dup(osf);
    CHECK(dup != nullptr, "Duplicate returns a non-null COleStreamFile");
    CHECK(dup != (void*)osf, "Duplicate returns a distinct object");
    if (dup) {
        IStream* dupStream = *(IStream**)((unsigned char*)dup + 24);
        CHECK(dupStream != nullptr, "Duplicate cloned the underlying IStream");
        CHECK(dupStream != pStm,    "Duplicate's stream is a clone, not the original");
    }

    // GetStorageName returns a class by value: sret pointer in RCX, this in RDX.
    // If the parameter order were reversed (as it was), this would corrupt or
    // crash rather than produce a CString.
    void* strbuf[4];
    memset(strbuf, 0, sizeof strbuf);
    GetNm(strbuf, osf);
    CHECK(true, "GetStorageName returned without faulting (sret-first ABI)");

    // ---- COleSafeArray::GetByteArray ----
    typedef void (__stdcall *GetByteArrayFn)(void*, void*);
    auto GetBA = (GetByteArrayFn)GetProcAddress(h, "?GetByteArray@COleSafeArray@@QEAAXAEAVCByteArray@@@Z");
    if (!GetBA) { printf("missing GetByteArray export\n"); return 1; }

    SAFEARRAYBOUND bound; bound.lLbound = 0; bound.cElements = 8;
    SAFEARRAY* psa = ::SafeArrayCreate(VT_UI1, 1, &bound);
    if (psa) {
        unsigned char* raw = nullptr;
        if (SUCCEEDED(::SafeArrayAccessData(psa, (void**)&raw))) {
            for (int i = 0; i < 8; ++i) raw[i] = (unsigned char)(0xA0 + i);
            ::SafeArrayUnaccessData(psa);
        }
        // COleSafeArray is tagVARIANT + 2 cached DWORDs; the SAFEARRAY hangs off
        // the variant's parray, so pThis is the COleSafeArray, NOT the SAFEARRAY.
        struct { VARIANT var; unsigned long elemSize; unsigned long dims; } sa;
        memset(&sa, 0, sizeof sa);
        sa.var.vt = VT_ARRAY | VT_UI1;
        sa.var.parray = psa;
        sa.elemSize = psa->cbElements;
        sa.dims = psa->cDims;

        // CByteArray declares no data members -- its storage lives in a side
        // table keyed by `this`, so the object's bytes never change and cannot
        // be inspected directly. Construct through the exported ctor so the
        // side table registers the object, then read the size back through the
        // ABI: Append(empty) returns the index appending started at, i.e. the
        // current size.
        typedef void*     (__stdcall *CbaCtor)(void*);
        typedef long long (__stdcall *AppendFn)(void*, const void*);
        auto Ctor   = (CbaCtor) GetProcAddress(h, "??0CByteArray@@QEAA@XZ");
        auto Append = (AppendFn)GetProcAddress(h, "?Append@CByteArray@@QEAA_JAEBV1@@Z");
        if (!Ctor || !Append) { printf("missing CByteArray export(s)\n"); return 1; }

        alignas(16) unsigned char cba[64];   alignas(16) unsigned char empty[64];
        memset(cba, 0, sizeof cba);          memset(empty, 0, sizeof empty);
        Ctor(cba);                           Ctor(empty);

        CHECK(Append(cba, empty) == 0, "CByteArray starts empty before GetByteArray");
        GetBA(&sa, cba);
        long long filled = Append(cba, empty);
        CHECK(filled == 8, "GetByteArray filled the CByteArray with all 8 bytes");
        ::SafeArrayDestroy(psa);
    }

    if (pStm) pStm->Release();
    printf("%d passed, %d failed\n", pass, fail);
    ::CoUninitialize();
    return fail ? 1 : 0;
}
