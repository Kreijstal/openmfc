// Behavioral test for CHandleMap real implementation.
//
// Drives the two exported impl_ functions directly and asserts concrete
// outcomes:
//   - FromHandle(null handle) -> nullptr
//   - FromHandle on a permanent handle returns the exact permanent wrapper
//   - FromHandle on an unknown handle creates a temporary wrapper, and a second
//     call for the same handle returns the SAME temporary wrapper
//   - distinct handles get distinct temporary wrappers
//   - temporary wrappers carry the right handle value
//   - DeleteTemp() destroys temporaries (count -> 0) but keeps permanents
//   - after DeleteTemp, FromHandle(perm) still works and FromHandle(temp)
//     creates a fresh temporary
//
// Build:
//   x86_64-w64-mingw32-g++ -std=c++17 -static -DUNICODE -D_UNICODE -I include \
//       tests/test_chandlemap_logic.cpp -o /tmp/t2_CHandleMap.exe
//   WINEDEBUG=-all wine /tmp/t2_CHandleMap.exe; echo EXIT=$?

#include "../phase4/src/global_chandlemap.cpp"

#include <cstdio>

// Side-table helpers defined in the impl file.
void        CHandleMap_SetPermanent(CHandleMap* self, void* h, CObject* permob);
CObject*    CHandleMap_LookupPermanent(CHandleMap* self, void* h);
std::size_t CHandleMap_TemporaryCount(CHandleMap* self);

static int g_fail = 0;
static void check(const char* name, bool ok) {
    std::printf("[%s] %s\n", ok ? "PASS" : "FAIL", name);
    if (!ok) g_fail = 1;
}

int main() {
    // Construct a real CHandleMap (faithful 136-byte layout). Zero it the way
    // MFC's ctor would, then seed m_pClass / m_nOffset as MFC does.
    CHandleMap map;
    std::memset(&map, 0, sizeof(map));
    map.m_pClass = &CTempHandleObject::classCTempHandleObject;
    map.m_nOffset = 0;

    check("sizeof(CHandleMap)==136", sizeof(CHandleMap) == 136);

    // A fabricated permanent wrapper (as if SetPermanent had been called by an
    // attach path). We use a CTempHandleObject as a stand-in CObject*.
    CTempHandleObject permWrapper;
    permWrapper.m_hHandle = reinterpret_cast<void*>(0x1000);

    void* hPerm = reinterpret_cast<void*>(0x1000);
    void* hTempA = reinterpret_cast<void*>(0x2000);
    void* hTempB = reinterpret_cast<void*>(0x3000);

    CHandleMap_SetPermanent(&map, hPerm, &permWrapper);

    // 1) null handle -> nullptr
    check("FromHandle(null)==nullptr",
          impl__FromHandle_CHandleMap__QEAAPEAVCObject__PEAX_Z(&map, nullptr) == nullptr);

    // 2) null this -> nullptr (ABI null guard)
    check("FromHandle(null this)==nullptr",
          impl__FromHandle_CHandleMap__QEAAPEAVCObject__PEAX_Z(nullptr, hTempA) == nullptr);

    // 3) permanent lookup returns the exact permanent wrapper
    CObject* gotPerm = impl__FromHandle_CHandleMap__QEAAPEAVCObject__PEAX_Z(&map, hPerm);
    check("FromHandle(perm) returns permanent wrapper", gotPerm == &permWrapper);
    check("permanent lookup left temporary map empty",
          CHandleMap_TemporaryCount(&map) == 0);

    // 4) unknown handle creates a temporary wrapper
    CObject* tmpA1 = impl__FromHandle_CHandleMap__QEAAPEAVCObject__PEAX_Z(&map, hTempA);
    check("FromHandle(unknown) returns non-null temp", tmpA1 != nullptr);
    check("temp wrapper is not the permanent wrapper", tmpA1 != &permWrapper);
    check("temporary count is 1 after one temp", CHandleMap_TemporaryCount(&map) == 1);

    // 5) temp wrapper carries the handle value
    auto* tmpA1Typed = static_cast<CTempHandleObject*>(tmpA1);
    check("temp wrapper carries correct handle", tmpA1Typed->m_hHandle == hTempA);

    // 6) second call for same handle returns SAME temporary wrapper (cached)
    CObject* tmpA2 = impl__FromHandle_CHandleMap__QEAAPEAVCObject__PEAX_Z(&map, hTempA);
    check("FromHandle(same temp) returns cached wrapper", tmpA2 == tmpA1);
    check("temporary count still 1 after re-lookup", CHandleMap_TemporaryCount(&map) == 1);

    // 7) distinct handle -> distinct temporary wrapper
    CObject* tmpB = impl__FromHandle_CHandleMap__QEAAPEAVCObject__PEAX_Z(&map, hTempB);
    check("distinct handle -> distinct temp", tmpB != tmpA1 && tmpB != nullptr);
    check("temporary count is 2 after two temps", CHandleMap_TemporaryCount(&map) == 2);

    // 8) DeleteTemp(null this) is a safe no-op (must not crash)
    impl__DeleteTemp_CHandleMap__QEAAXXZ(nullptr);
    check("DeleteTemp(null this) is safe", true);

    // 9) DeleteTemp clears temporaries
    impl__DeleteTemp_CHandleMap__QEAAXXZ(&map);
    check("temporary count 0 after DeleteTemp", CHandleMap_TemporaryCount(&map) == 0);

    // 10) permanent survives DeleteTemp
    check("permanent survives DeleteTemp",
          CHandleMap_LookupPermanent(&map, hPerm) == &permWrapper);
    CObject* gotPerm2 = impl__FromHandle_CHandleMap__QEAAPEAVCObject__PEAX_Z(&map, hPerm);
    check("FromHandle(perm) still works after DeleteTemp", gotPerm2 == &permWrapper);

    // 11) after DeleteTemp, a temp handle creates a FRESH wrapper carrying the
    //     right handle (count went from 0 back to 1). We avoid comparing against
    //     the freed pointer value since the allocator may reuse that address.
    CObject* tmpA3 = impl__FromHandle_CHandleMap__QEAAPEAVCObject__PEAX_Z(&map, hTempA);
    check("FromHandle(temp) creates non-null wrapper after DeleteTemp",
          tmpA3 != nullptr);
    check("fresh wrapper carries correct handle",
          static_cast<CTempHandleObject*>(tmpA3)->m_hHandle == hTempA);
    check("temporary count 1 after fresh temp", CHandleMap_TemporaryCount(&map) == 1);

    // 12) HARDER: a permanent-handle lookup must NOT register a temporary, even
    //     when temporaries already exist. Lookup the perm handle and confirm the
    //     temp count is unchanged (still 1 from tmpA3), and the exact perm
    //     wrapper is returned (not a fresh temp).
    std::size_t beforePermLookup = CHandleMap_TemporaryCount(&map);
    CObject* gotPerm3 = impl__FromHandle_CHandleMap__QEAAPEAVCObject__PEAX_Z(&map, hPerm);
    check("perm lookup returns exact perm wrapper (not temp)",
          gotPerm3 == &permWrapper);
    check("perm lookup did not create a temporary",
          CHandleMap_TemporaryCount(&map) == beforePermLookup);

    // Cleanup
    impl__DeleteTemp_CHandleMap__QEAAXXZ(&map);

    std::printf("%s\n", g_fail ? "RESULT: FAILURE" : "RESULT: ALL CHECKS PASSED");
    return g_fail;
}
