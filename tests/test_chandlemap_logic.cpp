// MSVC ABI behavioral regression for embedded handle maps and class callbacks.
#include "detail/CHandleMapSupport.h"
#include "detail/CThreadLocalObjectSupport.h"
#include <cstdio>
#include <cstddef>
extern "C" void MS_ABI impl___1CThreadLocalObject__QEAA_XZ(S_Cthreadlocalobject*);
namespace {
int failures, constructed, destroyed;
bool cleared = true;
alignas(8) BYTE moduleStorage[0x100]{};
struct Wrapper : CObject {
    UINT_PTR marker = 0x12345678;
    void* handles[2]{};
};
CRuntimeClass wrapperClass{"Wrapper", sizeof(Wrapper), 0xffff, nullptr, nullptr, nullptr, nullptr};
void MS_ABI construct(CObject* object) {
    ::new (object) Wrapper;
    ++constructed;
}
void MS_ABI destruct(CObject* object) {
    auto* wrapper = static_cast<Wrapper*>(object);
    cleared = cleared && !wrapper->handles[0] && !wrapper->handles[1];
    wrapper->~Wrapper();
    ++destroyed;
}
void check(bool value, const char* label) {
    printf("%s: %s\n", value ? "PASS" : "FAIL", label);
    if (!value)
        ++failures;
}
} // namespace
extern "C" AFX_MODULE_STATE* AFXAPI impl__AfxGetModuleState__YAPEAVAFX_MODULE_STATE__XZ() {
    return reinterpret_cast<AFX_MODULE_STATE*>(moduleStorage);
}
int main() {
    CHandleMap map(&wrapperClass, &construct, &destruct, offsetof(Wrapper, handles), 2);
    check(sizeof(map) == 176 && map.m_permanentMap.GetHashTableSize() == 17 &&
              map.m_temporaryMap.GetHashTableSize() == 7,
          "handle map embeds native allocator and permanent/temporary pointer maps");
    Wrapper permanent;
    void* hPermanent = reinterpret_cast<void*>(0x1000);
    permanent.handles[0] = hPermanent;
    map.m_permanentMap[hPermanent] = &permanent;
    check(!impl__FromHandle_CHandleMap__QEAAPEAVCObject__PEAX_Z(nullptr, nullptr),
          "a null handle returns null without touching its map");
    check(map.FromHandle(hPermanent) == &permanent && !permanent.handles[1] && !constructed,
          "permanent lookup returns the caller's wrapper without constructing or rewriting it");
    void *hFirst = reinterpret_cast<void*>(0x2000), *hSecond = reinterpret_cast<void*>(0x3000);
    auto* first = static_cast<Wrapper*>(map.FromHandle(hFirst));
    auto* second = static_cast<Wrapper*>(map.FromHandle(hSecond));
    check(
        first && second && first != second && constructed == 2 &&
            map.m_temporaryMap.GetCount() == 2,
        "unknown handles invoke the supplied constructor and populate the embedded temporary map");
    check(first->marker == 0x12345678 && first->handles[0] == hFirst && first->handles[1] == hFirst,
          "temporary wrappers retain their class fields and receive both configured handles");
    first->handles[0] = nullptr;
    first->handles[1] = nullptr;
    check(map.FromHandle(hFirst) == first && first->handles[0] == hFirst &&
              first->handles[1] == hFirst && constructed == 2,
          "cached temporary lookup restores both handle fields without reconstructing the wrapper");
    impl__DeleteTemp_CHandleMap__QEAAXXZ(nullptr);
    map.DeleteTemp();
    check(destroyed == 2 && cleared && !map.m_temporaryMap.GetCount() && !map.m_pBlocks &&
              !map.m_pNodeFree,
          "temporary deletion clears handles before callbacks and releases the fixed allocation "
          "chain");
    check(map.FromHandle(hPermanent) == &permanent && map.m_permanentMap.GetCount() == 1,
          "temporary deletion preserves application-owned permanent wrappers");
    auto* fresh = static_cast<Wrapper*>(map.FromHandle(hFirst));
    check(fresh && fresh->handles[0] == hFirst && constructed == 3,
          "lookup after cleanup constructs a new class-specific wrapper");
    map.DeleteTemp();
    check(destroyed == 3 && cleared,
          "all temporary wrappers receive exactly one destruction callback");
    impl___1CThreadLocalObject__QEAA_XZ(
        reinterpret_cast<S_Cthreadlocalobject*>(moduleStorage + 0xf8));
    printf("%d failures\n", failures);
    return failures ? 1 : 0;
}
