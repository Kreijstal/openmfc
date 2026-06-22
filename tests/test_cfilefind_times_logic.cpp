// Behavioral test for CFileFind CTime-returning time getters.
// Builds a CFileFind-layout buffer from a real temp file's WIN32_FIND_DATAW
// (via FindFirstFileW) and verifies the converted CTime values under Wine.

#include <windows.h>
#include <cstdio>
#include <cstring>

#include "../phase4/src/inet_cfilefind_times.cpp"

static int g_failures = 0;
static void check(bool cond, const char* what) {
    std::printf("%s: %s\n", cond ? "PASS" : "FAIL", what);
    if (!cond) ++g_failures;
}

// Mirror CFileFind's prefix exactly (vptr, m_hFindFile, m_findData).
struct CFFView {
    void*            vptr;
    HANDLE           m_hFindFile;
    WIN32_FIND_DATAW m_findData;
};

// Reference conversion (independent of the impl).
static long long ref_convert(const FILETIME& ft) {
    ULARGE_INTEGER u; u.LowPart = ft.dwLowDateTime; u.HighPart = ft.dwHighDateTime;
    return (long long)(u.QuadPart / 10000000ULL) - 11644473600LL;
}

int main() {
    wchar_t dir[MAX_PATH], path[MAX_PATH];
    GetTempPathW(MAX_PATH, dir);
    GetTempFileNameW(dir, L"cff", 0, path);

    // Write a few bytes so the file has real timestamps.
    HANDLE h = CreateFileW(path, GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS,
                           FILE_ATTRIBUTE_NORMAL, nullptr);
    check(h != INVALID_HANDLE_VALUE, "temp file created");
    DWORD w = 0; WriteFile(h, "data", 4, &w, nullptr); CloseHandle(h);

    WIN32_FIND_DATAW fd;
    HANDLE hf = FindFirstFileW(path, &fd);
    check(hf != INVALID_HANDLE_VALUE, "FindFirstFileW found the temp file");

    CFFView ff;
    std::memset(&ff, 0, sizeof(ff));
    ff.m_hFindFile = hf;          // a valid, active find handle
    ff.m_findData = fd;

    long long t = 0;
    int rc = impl__GetLastWriteTime_CFileFind__UEBAHAEAVCTime_ATL___Z(&ff, &t);
    check(rc == 1, "GetLastWriteTime returns TRUE for an active find");
    check(t == ref_convert(fd.ftLastWriteTime), "write time matches reference conversion");
    check(t > 1000000000LL, "write time is a plausible post-2001 epoch value");

    long long tc = 0;
    impl__GetCreationTime_CFileFind__UEBAHAEAVCTime_ATL___Z(&ff, &tc);
    check(tc == ref_convert(fd.ftCreationTime), "creation time matches reference conversion");

    long long ta = 0;
    impl__GetLastAccessTime_CFileFind__UEBAHAEAVCTime_ATL___Z(&ff, &ta);
    check(ta == ref_convert(fd.ftLastAccessTime), "access time matches reference conversion");

    // Inactive find (null handle) -> FALSE.
    CFFView empty;
    std::memset(&empty, 0, sizeof(empty));
    long long tz = 12345;
    int rz = impl__GetLastWriteTime_CFileFind__UEBAHAEAVCTime_ATL___Z(&empty, &tz);
    check(rz == 0, "inactive find (null handle) returns FALSE");
    check(tz == 12345, "inactive find leaves the out value untouched");

    FindClose(hf);
    DeleteFileW(path);

    std::printf("RESULT: %s (%d failures)\n", g_failures == 0 ? "ALL PASS" : "FAILED", g_failures);
    return g_failures == 0 ? 0 : 1;
}
