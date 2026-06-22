// Behavioral test for CSharedFile, driven through the exported impl_ thunks.
//
// Build:
//   x86_64-w64-mingw32-g++ -std=c++17 -static -DUNICODE -D_UNICODE -I include \
//       tests/test_csharedfile_logic.cpp -o /tmp/t2_CSharedFile.exe
//   WINEDEBUG=-all wine /tmp/t2_CSharedFile.exe; echo EXIT=$?
//
// We include the implementation directly so we can both (a) call the extern "C"
// MS-ABI exports and (b) inspect the concrete object layout to prove real
// behavior (HGLOBAL handles, locked content, bookkeeping resets).

// Pull in filecore.cpp first to obtain the real out-of-line CFile / CMemFile
// base members (ctor/dtor, Read/Write/Seek/SetLength/GetLength/Flush, and the
// vtable entries) that CSharedFile inherits, then include the CSharedFile impl
// under test directly. (Same approach as test_cmirrorfile_logic.cpp.)
#include "../phase4/src/filecore.cpp"

// filecore.cpp's CArchive serialization code references a handful of symbols
// that live in other translation units. None are reached by the CSharedFile
// code paths exercised here, so provide minimal local definitions to keep the
// test a self-contained, standalone binary.
extern "C" CRuntimeClass* MS_ABI
impl__Load_CRuntimeClass__SAPEAU1_AEAVCArchive__PEAI_Z(CArchive*, unsigned int*) {
    return nullptr;
}
extern "C" void MS_ABI
impl__Store_CRuntimeClass__QEBAXAEAVCArchive___Z(const CRuntimeClass*, CArchive*) {
}
extern "C" void MS_ABI
impl__AfxThrowFileException__YAXHJPEB_W_Z(int, long, const wchar_t*) {
}
extern "C" CRuntimeClass* MS_ABI
impl__GetThisClass_CFileException__SAPEAUCRuntimeClass__XZ() {
    return nullptr;
}

#include "../phase4/src/file_csharedfile.cpp"

#include <cstdio>
#include <cstring>

static int g_failures = 0;

// Test-side accessor to inspect CMemFile's protected bookkeeping members.
struct TestAccessor : CSharedFile {
    static BYTE*& Buffer(CSharedFile* p)  { return static_cast<TestAccessor*>(p)->m_lpBuffer; }
    static UINT&  BufSize(CSharedFile* p) { return static_cast<TestAccessor*>(p)->m_nBufferSize; }
};

static void check(const char* name, bool cond) {
    std::printf("[%s] %s\n", cond ? "PASS" : "FAIL", name);
    if (!cond) ++g_failures;
}

// Declare the exports we drive (they are extern "C" with MS_ABI).
extern "C" void          MS_ABI impl___0CSharedFile__QEAA_II_Z(void*, unsigned int, unsigned int);
extern "C" void          MS_ABI impl___1CSharedFile__UEAA_XZ(void*);
extern "C" unsigned char* MS_ABI impl__Alloc_CSharedFile__MEAAPEAE_K_Z(void*, unsigned long long);
extern "C" void*         MS_ABI impl__Detach_CSharedFile__QEAAPEAXXZ(void*);
extern "C" void          MS_ABI impl__Free_CSharedFile__MEAAXPEAE_Z(void*, unsigned char*);
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CSharedFile__UEBAPEAUCRuntimeClass__XZ(const void*);
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CSharedFile__SAPEAUCRuntimeClass__XZ();
extern "C" unsigned char* MS_ABI impl__Realloc_CSharedFile__MEAAPEAEPEAE_K_Z(void*, unsigned char*, unsigned long long);
extern "C" void          MS_ABI impl__SetHandle_CSharedFile__QEAAXPEAXH_Z(void*, void*, int);

int main() {
    // ---- Layout sanity ------------------------------------------------------
    check("sizeof(CSharedFile)==72", sizeof(CSharedFile) == 72);

    // ---- 1. Construct via ctor thunk; placement into a raw buffer -----------
    alignas(CSharedFile) unsigned char storage[sizeof(CSharedFile)];
    void* pThis = storage;
    const unsigned int kFlags = GMEM_MOVEABLE | GMEM_DDESHARE;
    impl___0CSharedFile__QEAA_II_Z(pThis, kFlags, 4096);
    CSharedFile* obj = reinterpret_cast<CSharedFile*>(pThis);
    check("ctor: m_nAllocFlags stored", obj->m_nAllocFlags == kFlags);
    check("ctor: m_hGlobalMemory starts null", obj->m_hGlobalMemory == nullptr);

    // ---- 2. Alloc through thunk -> real GlobalAlloc-backed, locked pointer --
    const SIZE_T n1 = 100;
    unsigned char* p1 = impl__Alloc_CSharedFile__MEAAPEAE_K_Z(pThis, n1);
    check("Alloc: returned non-null", p1 != nullptr);
    check("Alloc: handle recorded", obj->m_hGlobalMemory != nullptr);
    if (p1) {
        // The returned pointer must be the locked address of the recorded handle.
        check("Alloc: ptr == GlobalLock(handle)",
              p1 == reinterpret_cast<unsigned char*>(GlobalLock(obj->m_hGlobalMemory)));
        GlobalUnlock(obj->m_hGlobalMemory);  // balance the extra lock above
        check("Alloc: GlobalSize >= requested", GlobalSize(obj->m_hGlobalMemory) >= n1);
    }

    // Write a recognizable pattern into the allocated block.
    if (p1) {
        for (SIZE_T i = 0; i < n1; ++i) p1[i] = static_cast<unsigned char>((i * 7 + 3) & 0xFF);
    }

    // ---- 3. Realloc through thunk grows the SAME logical block --------------
    const SIZE_T n2 = 5000;  // forces a real grow
    unsigned char* p2 = impl__Realloc_CSharedFile__MEAAPEAEPEAE_K_Z(pThis, p1, n2);
    check("Realloc: returned non-null", p2 != nullptr);
    check("Realloc: handle still recorded", obj->m_hGlobalMemory != nullptr);
    check("Realloc: GlobalSize grew", GlobalSize(obj->m_hGlobalMemory) >= n2);
    bool patternKept = (p2 != nullptr);
    if (p2) {
        for (SIZE_T i = 0; i < n1; ++i) {
            if (p2[i] != static_cast<unsigned char>((i * 7 + 3) & 0xFF)) { patternKept = false; break; }
        }
    }
    check("Realloc: original bytes preserved", patternKept);

    // ---- 4. Detach hands back a usable HGLOBAL and resets bookkeeping -------
    HGLOBAL hDetached = static_cast<HGLOBAL>(impl__Detach_CSharedFile__QEAAPEAXXZ(pThis));
    check("Detach: returned a handle", hDetached != nullptr);
    check("Detach: object handle cleared", obj->m_hGlobalMemory == nullptr);
    check("Detach: m_lpBuffer cleared", TestAccessor::Buffer(obj) == nullptr);
    check("Detach: m_nBufferSize cleared", TestAccessor::BufSize(obj) == 0);

    // The detached handle is independently lockable and still holds the bytes.
    bool detachedContentOk = false;
    if (hDetached) {
        unsigned char* dp = reinterpret_cast<unsigned char*>(GlobalLock(hDetached));
        if (dp) {
            detachedContentOk = true;
            for (SIZE_T i = 0; i < n1; ++i) {
                if (dp[i] != static_cast<unsigned char>((i * 7 + 3) & 0xFF)) { detachedContentOk = false; break; }
            }
            GlobalUnlock(hDetached);
        }
    }
    check("Detach: handle content intact after detach", detachedContentOk);

    // ---- 5. SetHandle installs an external HGLOBAL as the backing store -----
    // Build an external global block with known content.
    const SIZE_T n3 = 64;
    HGLOBAL hExt = GlobalAlloc(GMEM_MOVEABLE, n3);
    check("SetHandle: external GlobalAlloc ok", hExt != nullptr);
    if (hExt) {
        unsigned char* ep = reinterpret_cast<unsigned char*>(GlobalLock(hExt));
        for (SIZE_T i = 0; i < n3; ++i) ep[i] = static_cast<unsigned char>(0xA0 + (i & 0x0F));
        GlobalUnlock(hExt);
    }
    impl__SetHandle_CSharedFile__QEAAXPEAXH_Z(pThis, hExt, /*bAllowGrow=*/1);
    check("SetHandle: object adopted handle", obj->m_hGlobalMemory == hExt);
    check("SetHandle: m_lpBuffer locked non-null", TestAccessor::Buffer(obj) != nullptr);
    check("SetHandle: m_nBufferSize reflects GlobalSize",
          TestAccessor::BufSize(obj) == static_cast<UINT>(GlobalSize(hExt)));
    bool extSeen = (TestAccessor::Buffer(obj) != nullptr);
    if (TestAccessor::Buffer(obj)) {
        for (SIZE_T i = 0; i < n3; ++i) {
            if (TestAccessor::Buffer(obj)[i] != static_cast<unsigned char>(0xA0 + (i & 0x0F))) { extSeen = false; break; }
        }
    }
    check("SetHandle: backing content visible via m_lpBuffer", extSeen);

    // ---- 6. Free thunk releases the currently-owned global block ------------
    impl__Free_CSharedFile__MEAAXPEAE_Z(pThis, TestAccessor::Buffer(obj));
    check("Free: object handle cleared", obj->m_hGlobalMemory == nullptr);

    // ---- 6b. Realloc(..., 0) on a live block frees it (Free branch) ---------
    {
        unsigned char* q1 = impl__Alloc_CSharedFile__MEAAPEAE_K_Z(pThis, 256);
        check("Realloc-zero: pre-alloc ok", q1 != nullptr && obj->m_hGlobalMemory != nullptr);
        unsigned char* q2 = impl__Realloc_CSharedFile__MEAAPEAEPEAE_K_Z(pThis, q1, 0);
        check("Realloc-zero: returns null", q2 == nullptr);
        check("Realloc-zero: handle freed/cleared", obj->m_hGlobalMemory == nullptr);
    }

    // ---- 7. RuntimeClass exports follow the repo file-family convention -----
    check("GetThisClass returns null (file family convention)",
          impl__GetThisClass_CSharedFile__SAPEAUCRuntimeClass__XZ() == nullptr);
    check("GetRuntimeClass returns null (file family convention)",
          impl__GetRuntimeClass_CSharedFile__UEBAPEAUCRuntimeClass__XZ(pThis) == nullptr);

    // ---- 8. Null-guards: thunks must not crash on null this ----------------
    impl___1CSharedFile__UEAA_XZ(nullptr);
    check("Alloc(null this) -> null", impl__Alloc_CSharedFile__MEAAPEAE_K_Z(nullptr, 10) == nullptr);
    check("Detach(null this) -> null", impl__Detach_CSharedFile__QEAAPEAXXZ(nullptr) == nullptr);
    impl__Free_CSharedFile__MEAAXPEAE_Z(nullptr, nullptr);
    impl__SetHandle_CSharedFile__QEAAXPEAXH_Z(nullptr, nullptr, 0);
    check("Realloc(null this) -> null",
          impl__Realloc_CSharedFile__MEAAPEAEPEAE_K_Z(nullptr, nullptr, 10) == nullptr);
    check("null-guards survived (no crash)", true);

    // ---- 9. Clean up the detached handle and destruct the object -----------
    if (hDetached) GlobalFree(hDetached);
    impl___1CSharedFile__UEAA_XZ(pThis);  // destructor thunk; nothing owned now
    check("dtor on empty object ok", true);

    std::printf("\n%d failure(s)\n", g_failures);
    return g_failures ? 1 : 0;
}
