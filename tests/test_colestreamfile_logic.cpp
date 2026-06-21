// Behavioral test for COleStreamFile IStream-lifecycle methods.
//
// Includes the impl .cpp directly and drives the exported impl_ thunks against a
// COleStreamFile-layout buffer (m_lpStream @ offset 24, sizeof 64). Exercises
// real OLE streams under Wine: an HGLOBAL memory stream and a compound-file
// (StgCreateDocfile) storage round-trip.

#include <windows.h>
#include <objbase.h>
#include <objidl.h>
#include <cstdio>
#include <cstring>

#include "../phase4/src/ole_colestreamfile_ext.cpp"

// 64-byte COleStreamFile-shaped storage; m_lpStream view at offset 24.
struct alignas(8) OSF {
    unsigned char bytes[64];
    OSF() { std::memset(bytes, 0, sizeof(bytes)); }
    IStream*& stream() { return *reinterpret_cast<IStream**>(bytes + 24); }
    void* p() { return bytes; }
};

static int g_failures = 0;
static void check(bool cond, const char* what) {
    std::printf("%s: %s\n", cond ? "PASS" : "FAIL", what);
    if (!cond) ++g_failures;
}

int main() {
    CoInitialize(nullptr);

    // ----- 1. CreateMemoryStream + GetLength/Position/SetLength -----
    OSF f;
    int ok = impl__CreateMemoryStream_COleStreamFile__QEAAHPEAVCFileException___Z(f.p(), nullptr);
    check(ok == 1, "CreateMemoryStream returns TRUE");
    check(f.stream() != nullptr, "CreateMemoryStream installs an IStream");

    check(impl__GetLength_COleStreamFile__UEBA_KXZ(f.p()) == 0, "fresh stream length == 0");

    // Write 12 bytes directly through the stream to seed it.
    const char* msg = "hello world!";   // 12 bytes
    ULONG written = 0;
    f.stream()->Write(msg, 12, &written);
    check(written == 12, "seed write of 12 bytes");
    check(impl__GetLength_COleStreamFile__UEBA_KXZ(f.p()) == 12, "GetLength == 12 after write");
    check(impl__GetPosition_COleStreamFile__UEBA_KXZ(f.p()) == 12, "GetPosition == 12 (at end)");

    // Rewind via the stream, GetPosition should follow.
    LARGE_INTEGER zero; zero.QuadPart = 0;
    f.stream()->Seek(zero, STREAM_SEEK_SET, nullptr);
    check(impl__GetPosition_COleStreamFile__UEBA_KXZ(f.p()) == 0, "GetPosition == 0 after rewind");

    // SetLength grows then shrinks.
    impl__SetLength_COleStreamFile__UEAAX_K_Z(f.p(), 100);
    check(impl__GetLength_COleStreamFile__UEBA_KXZ(f.p()) == 100, "SetLength(100) grows stream");
    impl__SetLength_COleStreamFile__UEAAX_K_Z(f.p(), 4);
    check(impl__GetLength_COleStreamFile__UEBA_KXZ(f.p()) == 4, "SetLength(4) shrinks stream");

    // Flush should not fail on a memory stream.
    impl__Flush_COleStreamFile__UEAAXXZ(f.p());
    check(true, "Flush on memory stream does not crash");

    // Close releases and nulls the stream.
    impl__Close_COleStreamFile__UEAAXXZ(f.p());
    check(f.stream() == nullptr, "Close nulls m_lpStream");
    // Abort/Close idempotent on an already-closed file.
    impl__Abort_COleStreamFile__UEAAXXZ(f.p());
    check(f.stream() == nullptr, "Abort safe on closed file");

    // ----- 2. CreateStream / OpenStream round-trip via a compound file -----
    wchar_t tmpdir[MAX_PATH], tmpfile[MAX_PATH];
    GetTempPathW(MAX_PATH, tmpdir);
    GetTempFileNameW(tmpdir, L"osf", 0, tmpfile);
    DeleteFileW(tmpfile);  // StgCreateDocfile wants to create it fresh

    IStorage* stg = nullptr;
    HRESULT hr = StgCreateDocfile(tmpfile,
        STGM_READWRITE | STGM_SHARE_EXCLUSIVE | STGM_CREATE, 0, &stg);
    check(SUCCEEDED(hr) && stg, "StgCreateDocfile created a storage");

    if (stg) {
        OSF cs;
        int c = impl__CreateStream_COleStreamFile__QEAAHPEAUIStorage__PEB_WKPEAVCFileException___Z(
            cs.p(), stg, L"Contents",
            STGM_READWRITE | STGM_SHARE_EXCLUSIVE | STGM_CREATE, nullptr);
        check(c == 1, "CreateStream in storage returns TRUE");
        check(cs.stream() != nullptr, "CreateStream installs an IStream");

        const char* payload = "STREAMDATA";   // 10 bytes
        ULONG w = 0;
        cs.stream()->Write(payload, 10, &w);
        check(impl__GetLength_COleStreamFile__UEBA_KXZ(cs.p()) == 10, "written stream length == 10");
        impl__Close_COleStreamFile__UEAAXXZ(cs.p());

        // Reopen the same stream and read it back.
        OSF os;
        int o = impl__OpenStream_COleStreamFile__QEAAHPEAUIStorage__PEB_WKPEAVCFileException___Z(
            os.p(), stg, L"Contents", STGM_READWRITE | STGM_SHARE_EXCLUSIVE, nullptr);
        check(o == 1, "OpenStream of existing stream returns TRUE");
        check(impl__GetLength_COleStreamFile__UEBA_KXZ(os.p()) == 10, "reopened stream length == 10");

        char readback[16] = {0};
        ULONG r = 0;
        LARGE_INTEGER z; z.QuadPart = 0;
        os.stream()->Seek(z, STREAM_SEEK_SET, nullptr);
        os.stream()->Read(readback, 10, &r);
        check(r == 10 && std::memcmp(readback, payload, 10) == 0,
              "round-trip data matches via OpenStream");

        // OpenStream of a missing stream returns FALSE.
        OSF miss;
        int m = impl__OpenStream_COleStreamFile__QEAAHPEAUIStorage__PEB_WKPEAVCFileException___Z(
            miss.p(), stg, L"DoesNotExist", STGM_READWRITE | STGM_SHARE_EXCLUSIVE, nullptr);
        check(m == 0, "OpenStream of missing stream returns FALSE");
        check(miss.stream() == nullptr, "failed OpenStream leaves m_lpStream null");

        impl__Close_COleStreamFile__UEAAXXZ(os.p());
        stg->Release();
    }
    DeleteFileW(tmpfile);

    CoUninitialize();
    std::printf("RESULT: %s (%d failures)\n", g_failures == 0 ? "ALL PASS" : "FAILED", g_failures);
    return g_failures == 0 ? 0 : 1;
}
