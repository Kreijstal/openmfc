#include "detail/CThreadLocalObjectSupport.h"
#include <cstdio>
#include <cstring>
#include <initializer_list>
#include <exception>
extern "C" {
CThreadSlotData* AFXAPI impl___0CThreadSlotData__QEAA_XZ(CThreadSlotData*);
void AFXAPI impl___1CThreadSlotData__QEAA_XZ(CThreadSlotData*);
int AFXAPI impl__AllocSlot_CThreadSlotData__QEAAHXZ(CThreadSlotData*);
void AFXAPI impl__FreeSlot_CThreadSlotData__QEAAXH_Z(CThreadSlotData*, int);
void AFXAPI impl__SetValue_CThreadSlotData__QEAAXHPEAX_Z(CThreadSlotData*, int, void*);
void AFXAPI impl__AssignInstance_CThreadSlotData__QEAAXPEAUHINSTANCE_____Z(CThreadSlotData*,
                                                                           HINSTANCE);
void AFXAPI impl__DeleteValues_CThreadSlotData__QEAAXPEAUCThreadData__PEAUHINSTANCE_____Z(
    CThreadSlotData*, CThreadData*, HINSTANCE);
void AFXAPI impl__DeleteValues_CThreadSlotData__QEAAXPEAUHINSTANCE____H_Z(CThreadSlotData*,
                                                                          HINSTANCE, BOOL);
void* AFXAPI impl__GetData_CThreadLocalObject__QEAAPEAVCNoTrackObject__P6APEAV2_XZ_Z(
    S_Cthreadlocalobject*, void*);
void* AFXAPI impl__GetDataNA_CThreadLocalObject__QEAAPEAVCNoTrackObject__XZ(S_Cthreadlocalobject*);
void AFXAPI impl___1CThreadLocalObject__QEAA_XZ(S_Cthreadlocalobject*);
void* AFXAPI impl___0AFX_MAINTAIN_STATE__QEAA_PEAVAFX_MODULE_STATE___Z(void*, AFX_MODULE_STATE*);
void AFXAPI impl___1AFX_MAINTAIN_STATE__QEAA_XZ(void*);
void* AFXAPI impl___0AFX_MAINTAIN_STATE2__QEAA_PEAVAFX_MODULE_STATE___Z(void*, AFX_MODULE_STATE*);
void AFXAPI impl___1AFX_MAINTAIN_STATE2__QEAA_XZ(void*);
void AFXAPI impl__AfxInitLocalData__YAXPEAUHINSTANCE_____Z(HINSTANCE);
void AFXAPI impl__AfxTermLocalData__YAXPEAUHINSTANCE____H_Z(HINSTANCE, BOOL);
void AFXAPI impl__AfxTlsAddRef__YAXXZ();
void AFXAPI impl__AfxTlsRelease__YAXXZ();
}
namespace {
int failures;
void check(bool value, const char* text) {
    printf("%s: %s\n", value ? "PASS" : "FAIL", text);
    if (!value)
        ++failures;
}
struct API {
    decltype(&impl___0CThreadSlotData__QEAA_XZ) construct;
    decltype(&impl___1CThreadSlotData__QEAA_XZ) destroy;
    decltype(&impl__AllocSlot_CThreadSlotData__QEAAHXZ) allocate;
    decltype(&impl__FreeSlot_CThreadSlotData__QEAAXH_Z) release;
    decltype(&impl__SetValue_CThreadSlotData__QEAAXHPEAX_Z) set;
    decltype(&impl__AssignInstance_CThreadSlotData__QEAAXPEAUHINSTANCE_____Z) assign;
    decltype(&impl__DeleteValues_CThreadSlotData__QEAAXPEAUCThreadData__PEAUHINSTANCE_____Z)
        deleteNode;
    decltype(&impl__DeleteValues_CThreadSlotData__QEAAXPEAUHINSTANCE____H_Z) deleteValues;
    decltype(&impl__GetData_CThreadLocalObject__QEAAPEAVCNoTrackObject__P6APEAV2_XZ_Z) get;
    decltype(&impl__GetDataNA_CThreadLocalObject__QEAAPEAVCNoTrackObject__XZ) peek;
    decltype(&impl___1CThreadLocalObject__QEAA_XZ) localDestroy;
    CThreadSlotData** global;
    BYTE* embedded;
    _AFX_THREAD_STATE* (*thread)();
    decltype(&impl___0AFX_MAINTAIN_STATE__QEAA_PEAVAFX_MODULE_STATE___Z) scope;
    decltype(&impl___1AFX_MAINTAIN_STATE__QEAA_XZ) end;
    decltype(&impl___0AFX_MAINTAIN_STATE2__QEAA_PEAVAFX_MODULE_STATE___Z) scope2;
    decltype(&impl___1AFX_MAINTAIN_STATE2__QEAA_XZ) end2;
    decltype(&impl__AfxInitLocalData__YAXPEAUHINSTANCE_____Z) initializeLocal;
    decltype(&impl__AfxTermLocalData__YAXPEAUHINSTANCE____H_Z) terminateLocal;
    decltype(&impl__AfxTlsAddRef__YAXXZ) addReference;
    decltype(&impl__AfxTlsRelease__YAXXZ) releaseReference;
    DWORD* references;
};
struct Event {
    INT64 v[8];
};
struct Trace {
    Event records[65536];
    size_t count = 0;
} localTrace, nativeTrace;
Trace* trace;
bool capture, initializing;
int failure, failedAPI, mutation, currentThread, deleteCalls, factoryCalls;
CThreadSlotData* state;
alignas(16) BYTE arena[1 << 20], object[88];
size_t used;
struct Block {
    BYTE* bytes;
    size_t count;
};
Block globals[32], locals[128];
int globalCount, localCount;
void* tls[4]{};
using FreeValue = void*(AFXAPI*)(void*, UINT);
INT64 id(const void* p) {
    if (!p)
        return 0;
    auto n = reinterpret_cast<uintptr_t>(p), base = reinterpret_cast<uintptr_t>(arena);
    return n >= base && n < base + sizeof(arena) ? INT64(n - base) + 0x10000 : INT64(n);
}
void event(INT64 a, INT64 b = 0, INT64 c = 0, INT64 d = 0, INT64 e = 0, INT64 f = 0, INT64 g = 0,
           INT64 h = 0) {
    if (trace->count >= 65536)
        std::abort();
    trace->records[trace->count++] = {{a, b, c, d, e, f, g, h}};
    if (failure == a)
        throw 92;
}
BYTE* bytes(size_t count) {
    size_t offset = used;
    used += ((count + 63) & ~size_t(63)) + 64;
    if (used > sizeof(arena))
        std::abort();
    return arena + offset;
}
void reset(Trace& out) {
    trace = &out;
    used = 0;
    globalCount = localCount = 0;
    failure = failedAPI = mutation = currentThread = deleteCalls = factoryCalls = 0;
    initializing = false;
    memset(arena, 0xa5, sizeof(arena));
    memset(object, 0xcd, sizeof(object));
    memset(tls, 0, sizeof(tls));
    state = reinterpret_cast<CThreadSlotData*>(object);
    capture = true;
}
void snapshot(INT64 result = 0, INT64 caught = 0) {
    event(30, result, caught, state->m_nAlloc, state->m_nRover, state->m_nMax,
          id(state->m_pSlotData), id(state->m_list.m_pHead));
    event(31, state->m_tlsIndex, state->m_list.m_nNextOffset, deleteCalls, factoryCalls);
    if (state->m_pSlotData && state->m_nAlloc > 0 && state->m_nAlloc < 256)
        for (int i = 0; i < state->m_nMax && i < 8; ++i)
            event(32, i, state->m_pSlotData[i].dwFlags, id(state->m_pSlotData[i].hInst));
    auto* data = static_cast<CThreadData*>(state->m_list.m_pHead);
    for (int n = 0; data && n < 4; ++n) {
        event(33, id(data), id(data->pNext), data->nCount, id(data->pData));
        if (data->pData)
            for (int i = 0; i < data->nCount && i < 8; ++i)
                event(34, i, id(data->pData[i]));
        data = data->pNext;
    }
}
#define REAL(name) decltype(&::name) real##name
REAL(TlsAlloc);
REAL(TlsFree);
REAL(TlsGetValue);
REAL(TlsSetValue);
REAL(InitializeCriticalSection);
REAL(DeleteCriticalSection);
REAL(EnterCriticalSection);
REAL(LeaveCriticalSection);
REAL(GlobalAlloc);
REAL(GlobalReAlloc);
REAL(GlobalHandle);
REAL(GlobalLock);
REAL(GlobalUnlock);
REAL(GlobalFree);
REAL(LocalAlloc);
REAL(LocalReAlloc);
REAL(LocalFree);
#undef REAL
DWORD WINAPI tlsAlloc() {
    if (!capture)
        return realTlsAlloc();
    event(1);
    return failedAPI == 1 ? TLS_OUT_OF_INDEXES : 11;
}
BOOL WINAPI tlsFree(DWORD index) {
    if (!capture)
        return realTlsFree(index);
    event(2, index);
    return FALSE;
}
void* WINAPI tlsGet(DWORD index) {
    if (!capture)
        return realTlsGetValue(index);
    event(3, index, currentThread);
    return tls[currentThread];
}
BOOL WINAPI tlsSet(DWORD index, void* value) {
    if (!capture)
        return realTlsSetValue(index, value);
    event(4, index, id(value), currentThread);
    tls[currentThread] = value;
    return FALSE;
}
void WINAPI initialize(CRITICAL_SECTION* section) {
    if (!capture) {
        realInitializeCriticalSection(section);
        return;
    }
    if (initializing)
        state = reinterpret_cast<CThreadSlotData*>(reinterpret_cast<BYTE*>(section) - 40);
    event(5, reinterpret_cast<BYTE*>(section) - reinterpret_cast<BYTE*>(state));
    memset(section, 0x6a, sizeof(*section));
}
void WINAPI destroy(CRITICAL_SECTION* section) {
    if (!capture) {
        realDeleteCriticalSection(section);
        return;
    }
    event(6, reinterpret_cast<BYTE*>(section) - reinterpret_cast<BYTE*>(state));
}
void WINAPI enter(CRITICAL_SECTION* section) {
    if (!capture) {
        realEnterCriticalSection(section);
        return;
    }
    event(7, reinterpret_cast<BYTE*>(section) - reinterpret_cast<BYTE*>(state));
    if (mutation == 1)
        state->m_nRover = 2;
}
void WINAPI leave(CRITICAL_SECTION* section) {
    if (!capture) {
        realLeaveCriticalSection(section);
        return;
    }
    event(8, reinterpret_cast<BYTE*>(section) - reinterpret_cast<BYTE*>(state));
}
HGLOBAL globalHandle(int index) { return reinterpret_cast<HGLOBAL>(UINT_PTR(0x1000 + index)); }
int index(HGLOBAL h) { return int(reinterpret_cast<UINT_PTR>(h)) - 0x1000; }
HGLOBAL WINAPI globalAlloc(UINT flags, SIZE_T count) {
    if (!capture)
        return realGlobalAlloc(flags, count);
    event(9, flags, count);
    if (failedAPI == 9)
        return nullptr;
    auto& b = globals[globalCount];
    b = {bytes(count + 128), count};
    return globalHandle(globalCount++);
}
HGLOBAL WINAPI globalRealloc(HGLOBAL handle, SIZE_T count, UINT flags) {
    if (!capture)
        return realGlobalReAlloc(handle, count, flags);
    event(10, id(handle), count, flags);
    if (failedAPI == 10)
        return nullptr;
    auto& b = globals[index(handle)];
    BYTE* replacement = bytes(count + 128);
    memcpy(replacement, b.bytes, b.count < count ? b.count : count);
    b = {replacement, count};
    return handle;
}
HGLOBAL WINAPI globalFrom(const void* p) {
    if (!capture)
        return realGlobalHandle(p);
    event(11, id(p));
    if (failedAPI == 11)
        return nullptr;
    for (int i = 0; i < globalCount; ++i)
        if (globals[i].bytes == p)
            return globalHandle(i);
    return nullptr;
}
void* WINAPI globalLock(HGLOBAL h) {
    if (!capture)
        return realGlobalLock(h);
    event(12, id(h));
    return failedAPI == 12 ? nullptr : globals[index(h)].bytes;
}
BOOL WINAPI globalUnlock(HGLOBAL h) {
    if (!capture)
        return realGlobalUnlock(h);
    event(13, id(h));
    return FALSE;
}
HGLOBAL WINAPI globalFree(HGLOBAL h) {
    if (!capture)
        return realGlobalFree(h);
    event(14, id(h));
    return nullptr;
}
HLOCAL WINAPI localAlloc(UINT flags, SIZE_T count) {
    if (!capture)
        return realLocalAlloc(flags, count);
    event(15, flags, count);
    if (failedAPI == 15)
        return nullptr;
    BYTE* p = bytes(count + 128);
    if (flags & LMEM_ZEROINIT)
        memset(p, 0, count);
    locals[localCount++] = {p, count};
    return p;
}
HLOCAL WINAPI localRealloc(HLOCAL old, SIZE_T count, UINT flags) {
    if (!capture)
        return realLocalReAlloc(old, count, flags);
    event(16, id(old), count, flags);
    if (failedAPI == 16)
        return nullptr;
    BYTE* p = bytes(count + 128);
    for (int i = 0; i < localCount; ++i)
        if (locals[i].bytes == old) {
            memcpy(p, old, locals[i].count < count ? locals[i].count : count);
            locals[i] = {p, count};
            break;
        }
    return p;
}
HLOCAL WINAPI localFree(HLOCAL p) {
    if (!capture)
        return realLocalFree(p);
    event(17, id(p));
    return nullptr;
}
void* AFXAPI deleteValue(void* object, UINT flags) {
    event(18, id(object), flags);
    ++deleteCalls;
    if (mutation == 2) {
        auto* node = static_cast<CThreadData*>(tls[currentThread]);
        if (node)
            node->nCount = 2;
    }
    return object;
}
void* valueTable[] = {reinterpret_cast<void*>(&deleteValue)};
void* value() {
    auto* p = bytes(24);
    *reinterpret_cast<void***>(p) = valueTable;
    return p;
}
void* AFXAPI factory() {
    event(19);
    ++factoryCalls;
    return failedAPI == 19 ? nullptr : value();
}
template <class F> INT64 invoke(F action) {
    try {
        action();
        return 0;
    } catch (int n) {
        return n;
    } catch (long n) {
        return n;
    } catch (CException* error) {
        auto* cls = error->GetRuntimeClass();
        const char* name = cls->m_lpszClassName;
        INT64 result = !strcmp(name, "CMemoryException")       ? 91
                       : !strcmp(name, "CInvalidArgException") ? 19
                       : !strcmp(name, "COleException")
                           ? *reinterpret_cast<long*>(reinterpret_cast<BYTE*>(error) + 16)
                           : -999;
        error->Delete();
        return result;
    }
}
void** import(HMODULE image, const char* name) {
    auto* b = reinterpret_cast<BYTE*>(image);
    auto* pe =
        reinterpret_cast<IMAGE_NT_HEADERS*>(b + reinterpret_cast<IMAGE_DOS_HEADER*>(b)->e_lfanew);
    auto* entry = reinterpret_cast<IMAGE_IMPORT_DESCRIPTOR*>(
        b + pe->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress);
    for (; entry->Name; ++entry) {
        auto* names = reinterpret_cast<IMAGE_THUNK_DATA*>(b + entry->OriginalFirstThunk);
        auto* values = reinterpret_cast<IMAGE_THUNK_DATA*>(b + entry->FirstThunk);
        for (; names->u1.AddressOfData; ++names, ++values)
            if (!IMAGE_SNAP_BY_ORDINAL(names->u1.Ordinal)) {
                auto* n = reinterpret_cast<IMAGE_IMPORT_BY_NAME*>(b + names->u1.AddressOfData);
                if (!strcmp(reinterpret_cast<char*>(n->Name), name))
                    return reinterpret_cast<void**>(&values->u1.Function);
            }
    }
    return nullptr;
}
struct Patch {
    void** slot;
    void* previous;
    DWORD protection;
    void set(void** s, void* replacement) {
        slot = s;
        previous = *s;
        VirtualProtect(slot, 8, PAGE_READWRITE, &protection);
        *slot = replacement;
    }
    void restore() {
        *slot = previous;
        DWORD ignored;
        VirtualProtect(slot, 8, protection, &ignored);
    }
};
struct Imports {
    Patch patches[18];
    int count = 0;
    Imports(HMODULE image, bool save) {
#define HOOK(name, callback)                                                                       \
    {                                                                                              \
        auto** s = import(image, #name);                                                           \
        check(s != nullptr, "locate TLS import " #name);                                           \
        if (!s)                                                                                    \
            std::abort();                                                                          \
        if (save)                                                                                  \
            real##name = reinterpret_cast<decltype(real##name)>(*s);                               \
        patches[count++].set(s, reinterpret_cast<void*>(&callback));                               \
    }
        HOOK(TlsAlloc, tlsAlloc);
        HOOK(TlsFree, tlsFree);
        HOOK(TlsGetValue, tlsGet);
        HOOK(TlsSetValue, tlsSet);
        HOOK(InitializeCriticalSection, initialize);
        HOOK(DeleteCriticalSection, destroy);
        HOOK(EnterCriticalSection, enter);
        HOOK(LeaveCriticalSection, leave);
        HOOK(GlobalAlloc, globalAlloc);
        HOOK(GlobalReAlloc, globalRealloc);
        HOOK(GlobalHandle, globalFrom);
        HOOK(GlobalLock, globalLock);
        HOOK(GlobalUnlock, globalUnlock);
        HOOK(GlobalFree, globalFree);
        HOOK(LocalAlloc, localAlloc);
        HOOK(LocalReAlloc, localRealloc);
        HOOK(LocalFree, localFree);
#undef HOOK
    }
    ~Imports() {
        while (count)
            patches[--count].restore();
    }
};
void compare(const Trace& expected, const Trace& actual, const char* text) {
    bool equal = expected.count == actual.count;
    size_t i = 0;
    for (; i < expected.count && i < actual.count; ++i)
        if (memcmp(&expected.records[i], &actual.records[i], sizeof(Event))) {
            equal = false;
            break;
        }
    if (!equal) {
        printf("TLS mismatch at %zu/%zu/%zu\n", i, expected.count, actual.count);
        for (size_t j = i > 2 ? i - 2 : 0; j < i + 4; ++j) {
            printf("%zu local:", j);
            if (j < expected.count)
                for (auto v : expected.records[j].v)
                    printf(" %lld", v);
            printf(" native:");
            if (j < actual.count)
                for (auto v : actual.records[j].v)
                    printf(" %lld", v);
            puts("");
        }
    }
    check(equal, text);
}
UINT64 traceHash(const Trace& value) {
    UINT64 hash = 1469598103934665603ULL;
    for (size_t i = 0; i < value.count; ++i)
        for (auto field : value.records[i].v) {
            hash ^= UINT64(field);
            hash *= 1099511628211ULL;
        }
    return hash;
}
void recordedTrace(const Trace& value, UINT64 expected, size_t count, const char* label) {
    bool equal = traceHash(value) == expected && value.count == count;
    if (!equal)
        printf("TLS trace: actual %llx/%zu, expected %llx/%zu\n", traceHash(value), value.count,
               expected, count);
    check(equal, label);
}
void constructors(const API& api, Trace& out) {
    out.count = 0;
    for (int missing : {0, 1})
        for (int at : {0, 1, 5}) {
            reset(out);
            failedAPI = missing;
            failure = at;
            INT64 result = 17;
            auto caught = invoke([&] { result = api.construct(state) == state; });
            snapshot(result, caught);
        }
    capture = false;
}
void allocation(const API& api, Trace& out) {
    out.count = 0;
    for (int op : {0, 1, 2, 3})
        for (int missing : {0, 9, 10, 11, 12})
            for (int at : {0, 7, 8, 9, 10, 11, 12, 13}) {
                reset(out);
                api.construct(state);
                int initial = op == 0 ? 0 : op == 1 ? 31 : op == 2 ? 32 : 64;
                for (int i = 0; i < initial; ++i)
                    api.allocate(state);
                if (op == 3) {
                    api.release(state, 2);
                    api.release(state, 5);
                }
                failure = at;
                failedAPI = missing;
                int result = 17;
                auto caught = invoke([&] { result = api.allocate(state); });
                snapshot(result, caught);
            }
    for (int allocated : {-33, INT_MIN}) {
        reset(out);
        api.construct(state);
        state->m_nAlloc = allocated;
        int result = 17;
        auto caught = invoke([&] { result = api.allocate(state); });
        snapshot(result, caught);
    }
    capture = false;
}
void values(const API& api, Trace& out) {
    out.count = 0;
    for (int op : {0, 1, 2, 3, 4})
        for (int missing : {0, 15, 16})
            for (int at : {0, 3, 4, 7, 8, 15, 16, 17, 18})
                for (int mode : {0, 2}) {
                    if (op == 4 && at)
                        continue;
                    reset(out);
                    api.construct(state);
                    for (int i = 0; i < 3; ++i)
                        api.allocate(state);
                    void* a = value();
                    api.set(state, 1, a);
                    api.assign(state, reinterpret_cast<HINSTANCE>(UINT_PTR(0x1234)));
                    api.allocate(state);
                    api.assign(state, reinterpret_cast<HINSTANCE>(UINT_PTR(0x5678)));
                    api.set(state, 4, value());
                    currentThread = 1;
                    api.set(state, 1, value());
                    currentThread = 0;
                    failure = at;
                    failedAPI = missing;
                    mutation = mode;
                    INT64 result = 17;
                    auto caught = invoke([&] {
                        switch (op) {
                        case 0:
                            api.allocate(state);
                            api.set(state, 5, value());
                            break;
                        case 1:
                            api.release(state, 1);
                            break;
                        case 2:
                            api.deleteValues(state, reinterpret_cast<HINSTANCE>(UINT_PTR(0x1234)),
                                             69);
                            break;
                        case 3:
                            api.deleteValues(state, nullptr, FALSE);
                            break;
                        case 4:
                            api.destroy(state);
                            break;
                        }
                        result = 0;
                    });
                    snapshot(result, caught);
                }
    capture = false;
}
void globalsTest(const API& api, Trace& out) {
    out.count = 0;
    auto* previous = *api.global;
    BYTE backup[80];
    if (api.embedded)
        memcpy(backup, api.embedded, 80);
    for (bool cold : {false, true})
        for (int mode : {0, 1, 2, 3})
            for (int at : {0, 7, 8, 15, 16, 17, 18, 19}) {
                reset(out);
                api.construct(state);
                *api.global = cold ? nullptr : state;
                initializing = cold;
                S_Cthreadlocalobject local{};
                INT64 first = 17;
                failure = at;
                if (mode == 1)
                    failedAPI = 19;
                auto caught = invoke([&] {
                    first = id(
                        api.get(&local, mode == 2 ? nullptr : reinterpret_cast<void*>(&factory)));
                });
                failure = 0;
                if (*api.global)
                    state = *api.global;
                event(35, first, caught, local.m_nSlot, id(api.peek(&local)));
                if (!caught) {
                    currentThread = 1;
                    event(36, id(api.get(&local, reinterpret_cast<void*>(&factory))));
                    currentThread = 0;
                    event(37, id(api.get(&local, reinterpret_cast<void*>(&factory))));
                }
                if (mode == 3 && local.m_nSlot)
                    api.localDestroy(&local);
                snapshot();
                event(38, local.m_nSlot);
            }
    capture = false;
    *api.global = previous;
    if (api.embedded)
        memcpy(api.embedded, backup, 80);
}
void lifecycle(const API& api, Trace& out) {
    out.count = 0;
    auto* previous = *api.global;
    DWORD references = *api.references;
    for (int op : {0, 1, 2, 3, 4})
        for (bool present : {false, true})
            for (DWORD initial : {DWORD(0), DWORD(1), DWORD(2), DWORD(0xffffffff)})
                for (UINT_PTR module : {UINT_PTR(0), UINT_PTR(0x1234), UINT_PTR(0x5678)})
                    for (int at : {0, 7, 8, 17, 18}) {
                        if (at && (op > 2 || !present || initial != 1 || module != 0x1234))
                            continue;
                        reset(out);
                        api.construct(state);
                        api.allocate(state);
                        api.assign(state, reinterpret_cast<HINSTANCE>(UINT_PTR(0x1234)));
                        api.allocate(state);
                        api.set(state, 1, value());
                        api.set(state, 2, value());
                        currentThread = 1;
                        api.set(state, 2, value());
                        currentThread = 0;
                        *api.global = present ? state : nullptr;
                        *api.references = initial;
                        failure = at;
                        auto caught = invoke([&] {
                            auto instance = reinterpret_cast<HINSTANCE>(module);
                            switch (op) {
                            case 0:
                                api.initializeLocal(instance);
                                break;
                            case 1:
                                api.terminateLocal(instance, FALSE);
                                break;
                            case 2:
                                api.terminateLocal(instance, 69);
                                break;
                            case 3:
                                api.addReference();
                                break;
                            case 4:
                                api.releaseReference();
                                break;
                            }
                        });
                        failure = 0;
                        event(44, *api.references, *api.global == state, *api.global == nullptr);
                        snapshot(0, caught);
                    }
    capture = false;
    *api.global = previous;
    *api.references = references;
}
void terminateLog() {
    capture = false;
    UINT64 hash = 1469598103934665603ULL;
    for (size_t i = 0; i < trace->count; ++i)
        for (auto value : trace->records[i].v) {
            hash ^= UINT64(value);
            hash *= 1099511628211ULL;
        }
    printf("TERMINATE %llx %zu\n", hash, trace->count);
    ExitProcess(86);
}
[[noreturn]] void terminateChild(const API& api, int at) {
    localTrace.count = 0;
    reset(localTrace);
    api.construct(state);
    int slot = api.allocate(state);
    api.set(state, slot, value());
    std::set_terminate(&terminateLog);
    failure = at;
    api.destroy(state);
    ExitProcess(87);
}
struct ChildResult {
    DWORD code;
    char line[96];
};
ChildResult runChild(bool native, int at) {
    wchar_t executable[1024];
    GetModuleFileNameW(nullptr, executable, 1024);
    wchar_t command[1100];
    swprintf_s(command, L"\"%s\"", executable);
    wchar_t mode[32];
    swprintf_s(mode, L"%d", at);
    SetEnvironmentVariableW(L"OPENMFC_TLS_TERMINATE", mode);
    SetEnvironmentVariableW(L"OPENMFC_TLS_TERMINATE_NATIVE", native ? L"1" : L"0");
    SECURITY_ATTRIBUTES security{sizeof(security), nullptr, TRUE};
    HANDLE reader, writer;
    CreatePipe(&reader, &writer, &security, 0);
    SetHandleInformation(reader, HANDLE_FLAG_INHERIT, 0);
    STARTUPINFOW startup{};
    startup.cb = sizeof(startup);
    startup.dwFlags = STARTF_USESTDHANDLES;
    startup.hStdInput = GetStdHandle(STD_INPUT_HANDLE);
    startup.hStdOutput = writer;
    startup.hStdError = writer;
    PROCESS_INFORMATION process{};
    BOOL created = CreateProcessW(nullptr, command, nullptr, nullptr, TRUE, 0, nullptr, nullptr,
                                  &startup, &process);
    CloseHandle(writer);
    if (!created)
        std::abort();
    char output[16384];
    DWORD count = 0, total = 0;
    while (total < sizeof(output) - 1 &&
           ReadFile(reader, output + total, DWORD(sizeof(output) - 1 - total), &count, nullptr) &&
           count)
        total += count;
    output[total] = 0;
    CloseHandle(reader);
    WaitForSingleObject(process.hProcess, 20000);
    ChildResult result{};
    GetExitCodeProcess(process.hProcess, &result.code);
    CloseHandle(process.hThread);
    CloseHandle(process.hProcess);
    if (auto* line = strstr(output, "TERMINATE ")) {
        strncpy_s(result.line, line, _TRUNCATE);
        if (char* end = strchr(result.line, '\r'))
            *end = 0;
        if (char* end = strchr(result.line, '\n'))
            *end = 0;
    }
    SetEnvironmentVariableW(L"OPENMFC_TLS_TERMINATE", nullptr);
    SetEnvironmentVariableW(L"OPENMFC_TLS_TERMINATE_NATIVE", nullptr);
    return result;
}
void terminationTests(bool native) {
    bool result = true;
    for (int at : {4, 6, 7, 8, 11, 13, 14, 17, 18}) {
        auto source = runChild(false, at);
        bool same = source.code == 86 && source.line[0];
        if (native) {
            auto oracle = runChild(true, at);
            same = same && oracle.code == 86 && !strcmp(source.line, oracle.line);
            if (!same)
                printf("termination case%d local %lu %s native %lu %s\n", at, source.code,
                       source.line, oracle.code, oracle.line);
        }
        result = result && same;
    }
    check(result,
          "TLS container destruction preserves the native noexcept boundary and cleanup trace");
}
void expectations(const API& api) {
    localTrace.count = 0;
    reset(localTrace);
    api.construct(state);
    int a = api.allocate(state);
    void* p = value();
    api.set(state, a, p);
    auto* node = static_cast<CThreadData*>(state->m_list.m_pHead);
    check(a == 1 && state->m_nAlloc == 32 && state->m_list.m_nNextOffset == 8 && node &&
              node->nCount == 2 && node->pData[1] == p,
          "TLS storage uses native slot metadata and a polymorphic 32-byte thread node");
    api.release(state, a);
    check(deleteCalls == 1 && !node->pData[1] && !(state->m_pSlotData[a].dwFlags & SLOT_USED),
          "slot release invokes the owned value's deleting destructor and clears its used flag");
    check(api.allocate(state) == 2,
          "slot allocation advances its rover before scanning released slots");
    capture = false;
}
void scopes(const API& api, Trace& out) {
    out.count = 0;
    auto* thread = api.thread();
    auto* saved = thread->m_pModuleState;
    for (int kind : {0, 1})
        for (UINT_PTR old : {UINT_PTR(0), UINT_PTR(0x1122)})
            for (UINT_PTR replacement : {UINT_PTR(0), UINT_PTR(0x3344)}) {
                void* fields[4];
                memset(fields, 0xcd, sizeof(fields));
                thread->m_pModuleState = reinterpret_cast<AFX_MODULE_STATE*>(old);
                void* result =
                    kind ? api.scope2(fields, reinterpret_cast<AFX_MODULE_STATE*>(replacement))
                         : api.scope(fields, reinterpret_cast<AFX_MODULE_STATE*>(replacement));
                trace = &out;
                event(40, result == fields, fields[0] == reinterpret_cast<void*>(old),
                      kind ? fields[1] == thread
                           : fields[1] == reinterpret_cast<void*>(UINT_PTR(0xcdcdcdcdcdcdcdcdULL)),
                      thread->m_pModuleState == reinterpret_cast<AFX_MODULE_STATE*>(replacement));
                if (kind)
                    api.end2(fields);
                else
                    api.end(fields);
                event(41, thread->m_pModuleState == reinterpret_cast<AFX_MODULE_STATE*>(old),
                      fields[kind ? 2 : 1] ==
                          reinterpret_cast<void*>(UINT_PTR(0xcdcdcdcdcdcdcdcdULL)));
            }
    alignas(8) BYTE other[32]{};
    auto* alternate = reinterpret_cast<_AFX_THREAD_STATE*>(other);
    alternate->m_pModuleState = reinterpret_cast<AFX_MODULE_STATE*>(UINT_PTR(0x5566));
    void* fields[] = {reinterpret_cast<void*>(UINT_PTR(0x7788)), alternate};
    thread->m_pModuleState = nullptr;
    api.end2(fields);
    event(42, !thread->m_pModuleState,
          alternate->m_pModuleState == reinterpret_cast<AFX_MODULE_STATE*>(UINT_PTR(0x7788)));
    fields[1] = nullptr;
    api.end2(fields);
    event(43, !thread->m_pModuleState);
    thread->m_pModuleState = saved;
}
struct Owned : CNoTrackObject {
    static LONG destroyed;
    ~Owned() override { InterlockedIncrement(&destroyed); }
};
LONG Owned::destroyed;
void* AFXAPI createOwned() { return new Owned; }
struct Worker {
    const API* api;
    S_Cthreadlocalobject* local;
    void* value;
    HANDLE ready, release;
};
DWORD WINAPI worker(void* p) {
    auto& w = *static_cast<Worker*>(p);
    w.value = w.api->get(w.local, reinterpret_cast<void*>(&createOwned));
    SetEvent(w.ready);
    WaitForSingleObject(w.release, INFINITE);
    return 0;
}
void integration(const API& api) {
    capture = false;
    S_Cthreadlocalobject local{};
    LONG before = Owned::destroyed;
    void* main = api.get(&local, reinterpret_cast<void*>(&createOwned));
    Worker work[4];
    HANDLE threads[4], ready[4];
    HANDLE release = CreateEventW(nullptr, TRUE, FALSE, nullptr);
    for (int i = 0; i < 4; ++i) {
        ready[i] = CreateEventW(nullptr, TRUE, FALSE, nullptr);
        work[i] = {&api, &local, nullptr, ready[i], release};
        threads[i] = CreateThread(nullptr, 0, worker, &work[i], 0, nullptr);
        if (!threads[i])
            std::abort();
    }
    WaitForMultipleObjects(4, ready, TRUE, INFINITE);
    bool distinct = true;
    for (int i = 0; i < 4; ++i) {
        distinct = distinct && work[i].value && work[i].value != main;
        for (int j = 0; j < i; ++j)
            distinct = distinct && work[i].value != work[j].value;
    }
    check(distinct && api.peek(&local) == main &&
              api.get(&local, reinterpret_cast<void*>(&createOwned)) == main,
          "thread-local objects keep independent stable values across four worker threads");
    SetEvent(release);
    WaitForMultipleObjects(4, threads, TRUE, INFINITE);
    for (auto h : threads)
        CloseHandle(h);
    for (auto h : ready)
        CloseHandle(h);
    CloseHandle(release);
    api.localDestroy(&local);
    check(!local.m_nSlot && Owned::destroyed == before + 5,
          "destroying a thread-local slot invokes all five owned destructors");
}
void lifecycleIntegration(const API& api) {
    api.releaseReference();
    check(
        !*api.global && !*api.references,
        "zero-reference release destroys the shared TLS backend and clears its published pointer");
    api.addReference();
    api.addReference();
    S_Cthreadlocalobject local{};
    LONG before = Owned::destroyed;
    void* value = api.get(&local, reinterpret_cast<void*>(&createOwned));
    api.releaseReference();
    check(*api.references == 1 && api.peek(&local) == value,
          "an outstanding TLS reference keeps its backend and owned value alive");
    auto module = reinterpret_cast<HINSTANCE>(UINT_PTR(0x1234));
    api.initializeLocal(module);
    api.terminateLocal(module, TRUE);
    check(Owned::destroyed == before + 1 && !api.peek(&local),
          "terminating module-local data destroys its owned value through the shared backend");
    api.localDestroy(&local);
    api.releaseReference();
    check(!*api.global && !*api.references, "the last TLS release frees the reinitialized backend");
    api.get(&local, reinterpret_cast<void*>(&createOwned));
    check(local.m_nSlot == 1 && api.peek(&local),
          "a later thread-local object can initialize fresh TLS storage after final release");
    api.localDestroy(&local);
    api.releaseReference();
}
} // namespace
int main() {
    setvbuf(stdout, nullptr, _IONBF, 0);
    API source{&impl___0CThreadSlotData__QEAA_XZ,
               &impl___1CThreadSlotData__QEAA_XZ,
               &impl__AllocSlot_CThreadSlotData__QEAAHXZ,
               &impl__FreeSlot_CThreadSlotData__QEAAXH_Z,
               &impl__SetValue_CThreadSlotData__QEAAXHPEAX_Z,
               &impl__AssignInstance_CThreadSlotData__QEAAXPEAUHINSTANCE_____Z,
               &impl__DeleteValues_CThreadSlotData__QEAAXPEAUCThreadData__PEAUHINSTANCE_____Z,
               &impl__DeleteValues_CThreadSlotData__QEAAXPEAUHINSTANCE____H_Z,
               &impl__GetData_CThreadLocalObject__QEAAPEAVCNoTrackObject__P6APEAV2_XZ_Z,
               &impl__GetDataNA_CThreadLocalObject__QEAAPEAVCNoTrackObject__XZ,
               &impl___1CThreadLocalObject__QEAA_XZ,
               &openmfc::detail::cthreadlocalobject::threadSlots,
               nullptr,
               &AfxGetThreadState,
               &impl___0AFX_MAINTAIN_STATE__QEAA_PEAVAFX_MODULE_STATE___Z,
               &impl___1AFX_MAINTAIN_STATE__QEAA_XZ,
               &impl___0AFX_MAINTAIN_STATE2__QEAA_PEAVAFX_MODULE_STATE___Z,
               &impl___1AFX_MAINTAIN_STATE2__QEAA_XZ,
               &impl__AfxInitLocalData__YAXPEAUHINSTANCE_____Z,
               &impl__AfxTermLocalData__YAXPEAUHINSTANCE____H_Z,
               &impl__AfxTlsAddRef__YAXXZ,
               &impl__AfxTlsRelease__YAXXZ,
               &openmfc::detail::cthreadlocalobject::tlsReferences};
    source.thread();
    API oracle = source;
    HMODULE native = nullptr;
    wchar_t path[1024];
    if (GetEnvironmentVariableW(L"OPENMFC_NATIVE_THREAD_STATE_DLL", path, 1024)) {
        native = LoadLibraryW(path);
        check(native != nullptr, "load native MFC TLS oracle");
        if (native) {
            auto* b = reinterpret_cast<BYTE*>(native);
            auto* pe = reinterpret_cast<IMAGE_NT_HEADERS*>(
                b + reinterpret_cast<IMAGE_DOS_HEADER*>(b)->e_lfanew);
            bool pinned = pe->FileHeader.TimeDateStamp == 0x442cba30 &&
                          pe->OptionalHeader.SizeOfImage == 0x55c000;
            check(pinned, "TLS oracle uses the verified MFC image build");
            if (!pinned) {
                FreeLibrary(native);
                native = nullptr;
            } else {
#define RVA(field, offset) oracle.field = reinterpret_cast<decltype(oracle.field)>(b + offset)
                RVA(construct, 0x14c8b0);
                RVA(destroy, 0x14c900);
                RVA(allocate, 0x14c980);
                RVA(release, 0x14cb40);
                RVA(set, 0x14cbd0);
                RVA(assign, 0x14cd70);
                RVA(deleteNode, 0x14cde0);
                RVA(deleteValues, 0x14ced0);
                RVA(get, 0x14cf40);
                RVA(peek, 0x14d040);
                RVA(localDestroy, 0x14d0d0);
                RVA(thread, 0x1332a0);
                RVA(scope, 0x1330f0);
                RVA(end, 0x133110);
                RVA(scope2, 0x133170);
                RVA(end2, 0x1331c0);
                RVA(initializeLocal, 0x14d190);
                RVA(terminateLocal, 0x14d1b0);
                RVA(addReference, 0x14d1e0);
                RVA(releaseReference, 0x14d1f0);
                RVA(references, 0x3c3ea8);
#undef RVA
                oracle.global = reinterpret_cast<CThreadSlotData**>(b + 0x3be330);
                oracle.embedded = b + 0x3be2e0;
                oracle.thread();
            }
        }
    }
    wchar_t terminateCase[32];
    if (GetEnvironmentVariableW(L"OPENMFC_TLS_TERMINATE", terminateCase, 32)) {
        wchar_t useNative[2];
        GetEnvironmentVariableW(L"OPENMFC_TLS_TERMINATE_NATIVE", useNative, 2);
        Imports imports(GetModuleHandleW(nullptr), true);
        if (useNative[0] == L'1') {
            if (!native)
                ExitProcess(88);
            Imports other(native, false);
            terminateChild(oracle, _wtoi(terminateCase));
        }
        terminateChild(source, _wtoi(terminateCase));
    }
    {
        Imports imports(GetModuleHandleW(nullptr), true);
        expectations(source);
// Recorded from the pinned native oracle below. All arena addresses are
// normalized and vptr addresses are omitted, so these traces also run on CI
// without requiring that particular proprietary DLL build.
#define COMPARE(fn, hash, count, text)                                                             \
    fn(source, localTrace);                                                                        \
    recordedTrace(localTrace, hash, count, text);                                                  \
    if (native) {                                                                                  \
        Imports other(native, false);                                                              \
        fn(oracle, nativeTrace);                                                                   \
        compare(localTrace, nativeTrace, text);                                                    \
    }
        COMPARE(constructors, 0x317107ea4034872fULL, 20,
                "TLS construction, resource failures and scalar initialization match native MFC");
        COMPARE(allocation, 0xf0dc19c84b66f061ULL, 13118,
                "slot rover, growth, allocation failures, overflow and synchronization match "
                "native MFC");
        COMPARE(values, 0x5a2b2fc7bb12fa5bULL, 12189,
                "thread values, module filtering, callback mutations and interrupted cleanup match "
                "native MFC");
        COMPARE(globalsTest, 0x2e889b724857fea7ULL, 1680,
                "per-object slot allocation, lazy globals, factories and value destruction match "
                "native MFC");
#undef COMPARE
        lifecycle(source, localTrace);
        recordedTrace(localTrace, 0x89854ef2a71f0ab9ULL, 5360,
                      "TLS module ownership and reference cleanup match the recorded native trace");
        if (native) {
            Imports other(native, false);
            lifecycle(oracle, nativeTrace);
            compare(localTrace, nativeTrace,
                    "TLS module ownership and final-reference cleanup match native MFC");
        }
    }
    scopes(source, localTrace);
    recordedTrace(localTrace, 0xe1b8a959389225a7ULL, 18,
                  "module-scope object fields match the recorded native trace");
    if (native) {
        scopes(oracle, nativeTrace);
        compare(localTrace, nativeTrace,
                "module scopes store and restore their native object fields without temporary-map "
                "side effects");
        integration(oracle);
    }
    integration(source);
    lifecycleIntegration(source);
    terminationTests(native != nullptr);
    if (native)
        FreeLibrary(native);
    printf("%d failures\n", failures);
    return failures ? 1 : 0;
}
