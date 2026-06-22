// Behavioral test for CDumpContext OpenMFC exports.
//
// Drives CDumpContext THROUGH the exported impl_ thunks, writing to a real
// CFile on disk, then reopens the file and verifies the formatted UTF-16 text.

#include <cstdio>
#include <cstring>
#include <cwchar>
#include <string>
#include <windows.h>

#include "../phase4/src/global_cdumpcontext.cpp"

// ---------------------------------------------------------------------------
// Self-contained CFile virtuals.
//
// The OpenMFC static lib exposes only MSVC-mangled / impl_ thunks, not the
// GCC-ABI CFile members this native test needs. We therefore provide real
// Win32-backed definitions for the CFile methods exercised here, plus trivial
// stubs for the remaining virtuals so CFile's vtable can be emitted. This lets
// CDumpContext's virtual m_pFile->Write()/Flush() calls dispatch correctly.
// ---------------------------------------------------------------------------
CFile::CFile() { m_hFile = INVALID_HANDLE_VALUE; }
CFile::CFile(HANDLE hFile) { m_hFile = hFile; }
CFile::CFile(const wchar_t* lpszFileName, UINT nOpenFlags) {
    DWORD access = (nOpenFlags & modeReadWrite) ? (GENERIC_READ | GENERIC_WRITE)
                 : (nOpenFlags & modeWrite)     ? GENERIC_WRITE
                                                : GENERIC_READ;
    DWORD disp = (nOpenFlags & modeCreate) ? CREATE_ALWAYS : OPEN_EXISTING;
    m_hFile = CreateFileW(lpszFileName, access, FILE_SHARE_READ, nullptr,
                          disp, FILE_ATTRIBUTE_NORMAL, nullptr);
}
CFile::~CFile() { Close(); }
UINT CFile::Read(void* lpBuf, UINT nCount) {
    if (m_hFile == INVALID_HANDLE_VALUE) return 0;
    DWORD got = 0; ReadFile(m_hFile, lpBuf, nCount, &got, nullptr); return got;
}
void CFile::Write(const void* lpBuf, UINT nCount) {
    if (m_hFile == INVALID_HANDLE_VALUE || !lpBuf || !nCount) return;
    DWORD wrote = 0; WriteFile(m_hFile, lpBuf, nCount, &wrote, nullptr);
}
ULONGLONG CFile::Seek(LONGLONG, UINT) { return 0; }
void CFile::SetLength(ULONGLONG) {}
ULONGLONG CFile::GetLength() const { return 0; }
void CFile::Flush() { if (m_hFile != INVALID_HANDLE_VALUE) FlushFileBuffers(m_hFile); }
void CFile::Close() {
    if (m_hFile != INVALID_HANDLE_VALUE) { CloseHandle(m_hFile); m_hFile = INVALID_HANDLE_VALUE; }
}
void CFile::SetFilePath(const wchar_t*) {}
CString CFile::GetFileName() const { return CString(); }
CString CFile::GetFileTitle() const { return CString(); }
CString CFile::GetFilePath() const { return CString(); }

static int g_fail = 0;

static void check(const char* name, bool ok) {
    printf("[%s] %s\n", ok ? "PASS" : "FAIL", name);
    if (!ok) g_fail = 1;
}

// Read entire file back as a wide string (assumes UTF-16LE content we wrote).
static std::wstring read_back(const wchar_t* path) {
    HANDLE h = CreateFileW(path, GENERIC_READ, FILE_SHARE_READ, nullptr,
                           OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
    if (h == INVALID_HANDLE_VALUE) return std::wstring();
    DWORD size = GetFileSize(h, nullptr);
    std::wstring out;
    out.resize(size / sizeof(wchar_t));
    DWORD got = 0;
    ReadFile(h, &out[0], size, &got, nullptr);
    CloseHandle(h);
    out.resize(got / sizeof(wchar_t));
    return out;
}

int main() {
    wchar_t tmpDir[MAX_PATH];
    GetTempPathW(MAX_PATH, tmpDir);
    wchar_t path[MAX_PATH];
    swprintf(path, MAX_PATH, L"%socmfc_cdc_%lu.bin", tmpDir, GetCurrentProcessId());

    // --- Build a CFile via its real ctor and attach a CDumpContext ---
    CFile file(path, CFile::modeCreate | CFile::modeWrite);

    // Construct CDumpContext through the exported ctor thunk (this in RCX).
    alignas(8) unsigned char storage[sizeof(CDumpContext)];
    memset(storage, 0xCC, sizeof(storage));
    CDumpContext* dc = reinterpret_cast<CDumpContext*>(storage);
    impl___0CDumpContext__QEAA_PEAVCFile___Z(dc, &file);

    // Layout / ctor checks.
    check("sizeof==16", sizeof(CDumpContext) == 16);
    check("ctor sets m_pFile", dc->m_pFile == &file);
    check("ctor sets m_nDepth=0", dc->m_nDepth == 0);

    // Drive formatting through the thunks. Each returns CDumpContext*.
    CDumpContext* r;
    r = impl___6CDumpContext__QEAAAEAV0_PEB_W_Z(dc, L"S=");
    check("op<< wchar* returns this", r == dc);
    impl___6CDumpContext__QEAAAEAV0_H_Z(dc, -42);          // int
    impl___6CDumpContext__QEAAAEAV0_PEB_W_Z(dc, L";");
    impl___6CDumpContext__QEAAAEAV0_I_Z(dc, 7u);           // unsigned int
    impl___6CDumpContext__QEAAAEAV0_PEB_W_Z(dc, L";");
    impl___6CDumpContext__QEAAAEAV0__J_Z(dc, (__int64)-1234567890123LL);
    impl___6CDumpContext__QEAAAEAV0_PEB_W_Z(dc, L";");
    impl___6CDumpContext__QEAAAEAV0_PEBD_Z(dc, "ansi");    // char* widened
    impl___6CDumpContext__QEAAAEAV0_PEB_W_Z(dc, L";");
    impl__DumpAsHex_CDumpContext__QEAAAEAV1_H_Z(dc, 255);  // 0xFF
    impl___6CDumpContext__QEAAAEAV0_PEB_W_Z(dc, L";");
    impl___6CDumpContext__QEAAAEAV0_E_Z(dc, (unsigned char)65); // 65

    // Flush + close through thunk / member.
    impl__Flush_CDumpContext__QEAAXXZ(dc);
    file.Close();

    std::wstring text = read_back(path);
    wprintf(L"     content=[%ls]\n", text.c_str());

    // Harder assertion: full formatted stream must match exactly.
    check("stream exact content", text == L"S=-42;7;-1234567890123;ansi;0xFF;65");
    check("contains S=", text.find(L"S=") != std::wstring::npos);
    check("int -42 formatted", text.find(L"-42") != std::wstring::npos);
    check("uint 7 formatted", text.find(L";7;") != std::wstring::npos);
    check("int64 formatted", text.find(L"-1234567890123") != std::wstring::npos);
    check("ansi widened", text.find(L"ansi") != std::wstring::npos);
    check("DumpAsHex 0xFF", text.find(L"0xFF") != std::wstring::npos);
    check("uchar 65", text.find(L"65") != std::wstring::npos);

    // --- Second file: HexDump and pointer/NULL behavior ---
    wchar_t path2[MAX_PATH];
    swprintf(path2, MAX_PATH, L"%socmfc_cdc2_%lu.bin", tmpDir, GetCurrentProcessId());
    CFile file2(path2, CFile::modeCreate | CFile::modeWrite);
    alignas(8) unsigned char storage2[sizeof(CDumpContext)];
    CDumpContext* dc2 = reinterpret_cast<CDumpContext*>(storage2);
    impl___0CDumpContext__QEAA_PEAVCFile___Z(dc2, &file2);

    unsigned char bytes[3] = { 0xDE, 0xAD, 0xBE };
    impl__HexDump_CDumpContext__QEAAXPEB_WPEAEHH_Z(dc2, L"H:", bytes, 3, 16);
    impl___6CDumpContext__QEAAAEAV0_PEB_W_Z(dc2, L"|");
    impl___6CDumpContext__QEAAAEAV0_PEB_W_Z(dc2, nullptr); // -> (NULL)
    impl__Flush_CDumpContext__QEAAXXZ(dc2);
    file2.Close();

    std::wstring text2 = read_back(path2);
    wprintf(L"     content2=[%ls]\n", text2.c_str());

    // Harder assertion: exact full content, ordering, separators and casing.
    check("HexDump exact content", text2 == L"H:DE AD BE |(NULL)");
    check("HexDump prefix", text2.find(L"H:") != std::wstring::npos);
    check("HexDump DE", text2.find(L"DE") != std::wstring::npos);
    check("HexDump AD", text2.find(L"AD") != std::wstring::npos);
    check("HexDump BE", text2.find(L"BE") != std::wstring::npos);
    check("NULL wchar handled", text2.find(L"(NULL)") != std::wstring::npos);

    // --- Null-this guards must not crash ---
    CDumpContext* none = nullptr;
    bool guarded = (impl___6CDumpContext__QEAAAEAV0_H_Z(none, 1) == nullptr);
    impl__Flush_CDumpContext__QEAAXXZ(none);
    check("null-this guarded", guarded);

    DeleteFileW(path);
    DeleteFileW(path2);

    printf(g_fail ? "RESULT: FAIL\n" : "RESULT: PASS\n");
    return g_fail;
}
