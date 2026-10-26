#define OPENMFC_APPCORE_IMPL
// Release-x64 module/thread allocation and handle ownership oracle tests.
#include "detail/ModuleThreadStateSupport.h"
#include "detail/RegcoreSupport.h"
#include "detail/CHandleMapSupport.h"
#include "detail/CThreadLocalObjectSupport.h"
#include <malloc.h>
#include <cstdio>
#include <cstring>
#include <initializer_list>
#include <exception>
#include <cstdlib>
extern "C" {
AFX_MODULE_THREAD_STATE* MS_ABI impl___0AFX_MODULE_THREAD_STATE__QEAA_XZ(AFX_MODULE_THREAD_STATE*);
void MS_ABI impl___1AFX_MODULE_THREAD_STATE__UEAA_XZ(AFX_MODULE_THREAD_STATE*);
void* MS_ABI impl___2_YAPEAX_K_Z(size_t);
int MS_ABI impl__DestroyToolTipCtrl_CToolTipCtrl__QEAAHXZ(CToolTipCtrl*);
void MS_ABI impl___1CThreadLocalObject__QEAA_XZ(S_Cthreadlocalobject*);
}
namespace {
int failures;
void check(bool value, const char* label) {
    printf("%s: %s\n", value ? "PASS" : "FAIL", label);
    if (!value)
        ++failures;
}
alignas(8) BYTE fallbackModule[0x100]{};
alignas(8) BYTE modules[3][0x100]{};
struct API {
    decltype(&impl___0AFX_MODULE_THREAD_STATE__QEAA_XZ) moduleConstruct;
    decltype(&impl___1AFX_MODULE_THREAD_STATE__UEAA_XZ) moduleDestroy;
    decltype(&impl__AfxGetModuleThreadState__YAPEAVAFX_MODULE_THREAD_STATE__XZ) get;
    decltype(&impl__AfxGetNewHandler__YAP6AH_K_ZXZ) getHandler;
    decltype(&impl__AfxSetNewHandler__YAP6AH_K_ZP6AH0_Z_Z) setHandler;
    decltype(&impl___2_YAPEAX_K_Z) allocate;
    decltype(&impl___0CHandleMap__QEAA_PEAUCRuntimeClass__P6AXPEAVCObject___Z2_KH_Z) mapConstruct;
    decltype(&impl__FromHandle_CHandleMap__QEAAPEAVCObject__PEAX_Z) from;
    decltype(&impl__DeleteTemp_CHandleMap__QEAAXXZ) clear;
    void** (*set)(CMapPtrToPtr*, void*);
    __POSITION* (*append)(OpenMFC_CPtrList*, void*);
    _AFX_THREAD_STATE* (*thread)();
    decltype(&impl__DestroyToolTipCtrl_CToolTipCtrl__QEAAHXZ) tooltip;
    decltype(&impl___1CThreadLocalObject__QEAA_XZ) localDestroy;
    AFX_MODULE_THREAD_STATE::NewHandler pressure, defaultHandler;
};
void** setMap(CMapPtrToPtr* map, void* key) { return &(*map)[key]; }
__POSITION* appendList(OpenMFC_CPtrList* list, void* value) { return list->AddTail(value); }
struct Event {
    INT64 v[8];
};
struct Trace {
    Event events[65536];
    size_t count = 0;
} localTrace, nativeTrace;
Trace* trace;
bool capture;
int at, missing, mutation, allocationCalls, handlerCalls, errorKind, failuresRemaining;
bool pointerFailure, structuredFailure;
[[noreturn]] void frameworkError(int);
const API* active;
CHandleMap* state;
AFX_MODULE_THREAD_STATE* ownedModule;
alignas(16) BYTE arena[1 << 20], storage[0x148], mapStorage[184], tooltipStorage[232];
size_t used;
using Malloc = void*(__cdecl*)(size_t);
using Free = void(__cdecl*)(void*);
Malloc realMalloc;
Free realFree;
decltype(&::_msize) realSize;
decltype(&::_expand) realExpand;
INT64 id(const void* p) {
    if (!p)
        return 0;
    auto n = reinterpret_cast<UINT_PTR>(p), a = reinterpret_cast<UINT_PTR>(arena);
    if (n >= a && n < a + sizeof(arena))
        return INT64(n - a) + 0x10000;
    if (p == tooltipStorage)
        return -10;
    if (p == mapStorage)
        return -11;
    return INT64(n);
}
void event(INT64 a, INT64 b = 0, INT64 c = 0, INT64 d = 0, INT64 e = 0, INT64 f = 0, INT64 g = 0,
           INT64 h = 0) {
    if (trace->count == 65536)
        std::abort();
    trace->events[trace->count++] = {{a, b, c, d, e, f, g, h}};
    if (at == a) {
        if (structuredFailure)
            RaiseException(0xe0424d52, EXCEPTION_NONCONTINUABLE, 0, nullptr);
        if (pointerFailure)
            frameworkError(91);
        throw 92;
    }
}
BYTE* bytes(size_t count) {
    if (count > sizeof(arena) - used - 128)
        std::abort();
    auto* p = arena + used;
    used += ((count + 63) & ~size_t(63)) + 64;
    return p;
}
void* __cdecl allocate(size_t size) {
    if (!capture)
        return realMalloc(size);
    event(1, size);
    ++allocationCalls;
    if (failuresRemaining) {
        --failuresRemaining;
        return nullptr;
    }
    if (missing && allocationCalls == missing)
        return nullptr;
    return bytes(size);
}
void __cdecl release(void* p) {
    if (!capture) {
        realFree(p);
        return;
    }
    event(2, id(p));
    if (mutation == 6)
        active->thread()->m_pSafetyPoolBuffer = arena + 0x9000;
}
size_t __cdecl blockSize(void* p) {
    if (!capture)
        return realSize(p);
    event(3, id(p));
    if (mutation == 5)
        active->thread()->m_pSafetyPoolBuffer = arena + 0x9000;
    return 80;
}
void* __cdecl expand(void* p, size_t size) {
    if (!capture)
        return realExpand(p, size);
    event(4, id(p), size);
    if (mutation == 7)
        active->thread()->m_pSafetyPoolBuffer = arena + 0x9000;
    return nullptr;
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
        VirtualProtect(s, 8, PAGE_READWRITE, &protection);
        *s = replacement;
    }
    void restore() {
        *slot = previous;
        DWORD ignored;
        VirtualProtect(slot, 8, protection, &ignored);
    }
};
struct Imports {
    Patch patches[4];
    int count = 0;
    Imports(HMODULE image, bool save) {
#define HOOK(name, callback, original)                                                             \
    {                                                                                              \
        auto** slot = import(image, name);                                                         \
        check(slot != nullptr, "locate module runtime import " name);                              \
        if (!slot)                                                                                 \
            std::abort();                                                                          \
        if (save)                                                                                  \
            original = reinterpret_cast<decltype(original)>(*slot);                                \
        patches[count++].set(slot, reinterpret_cast<void*>(&callback));                            \
    }
        HOOK("malloc", allocate, realMalloc);
        HOOK("free", release, realFree);
        HOOK("_msize", blockSize, realSize);
        HOOK("_expand", expand, realExpand);
#undef HOOK
    }
    ~Imports() {
        while (count)
            patches[--count].restore();
    }
};
CRuntimeClass memoryClass = {"CMemoryException", 16, 0xffff, nullptr, nullptr, nullptr, nullptr};
CRuntimeClass argumentClass = {
    "CInvalidArgException", 16, 0xffff, nullptr, nullptr, nullptr, nullptr};
CRuntimeClass* MS_ABI exceptionClass(const void*) {
    return errorKind == 19 ? &argumentClass : &memoryClass;
}
void* exceptionTable[] = {reinterpret_cast<void*>(&exceptionClass)};
alignas(8) BYTE exceptionData[24]{};
[[noreturn]] void frameworkError(int kind) {
    errorKind = kind;
    *reinterpret_cast<void***>(exceptionData) = exceptionTable;
    throw reinterpret_cast<CException*>(exceptionData);
}
template <class F> int invoke(F action) {
    try {
        action();
        return 0;
    } catch (int value) {
        return value;
    } catch (long value) {
        return int(value);
    } catch (CException* error) {
        auto* type = error->GetRuntimeClass();
        int result = !strcmp(type->m_lpszClassName, "CInvalidArgException") ? 19 : 91;
        error->Delete();
        return result;
    }
}
CRuntimeClass wrapperClass = {"ModuleTestWrapper", 32, 0xffff, nullptr, nullptr, nullptr, nullptr};
int MS_ABI handler(size_t size) {
    event(5, size);
    ++handlerCalls;
    if (mutation == 8 && handlerCalls == 1)
        active->setHandler(nullptr);
    if (mutation == 9 && handlerCalls == 1) {
        active->thread()->m_pModuleState = reinterpret_cast<AFX_MODULE_STATE*>(modules[1]);
        active->setHandler(nullptr);
    }
    return handlerCalls < 3;
}
int MS_ABI decline(size_t size) {
    event(10, size);
    ++handlerCalls;
    return FALSE;
}
void MS_ABI construct(CObject* object) {
    event(6, id(object));
    auto* words = reinterpret_cast<UINT_PTR*>(object);
    words[0] = 0x1111;
    words[1] = 0x2222;
    words[2] = 0x3333;
    words[3] = 0x4444;
    if (mutation == 1) {
        state->m_nOffset = 8;
        state->m_nHandles = 2;
    }
    if (mutation == 2)
        active->thread()->m_pModuleState = reinterpret_cast<AFX_MODULE_STATE*>(modules[1]);
    if (mutation == 3)
        frameworkError(91);
    if (mutation == 4)
        throw 93;
}
void MS_ABI destruct(CObject* object) {
    auto* words = reinterpret_cast<UINT_PTR*>(object);
    event(7, id(object), words[1], words[2], words[3]);
    if (ownedModule && mutation == 10)
        ownedModule->m_pmapHMENU = nullptr;
}
int MS_ABI destroyWindow(void* object) {
    event(8, id(object));
    return 0;
}
void* MS_ABI deleteTooltip(void* object, UINT flags) {
    event(9, id(object), flags);
    return object;
}
void* tooltipTable[27]{};
void reset(const API& api, Trace& output) {
    capture = false;
    active = &api;
    trace = &output;
    used = 0;
    at = missing = mutation = allocationCalls = handlerCalls = failuresRemaining = 0;
    pointerFailure = structuredFailure = false;
    memset(arena, 0xa5, sizeof(arena));
    memset(storage, 0xcd, sizeof(storage));
    memset(mapStorage, 0xcd, sizeof(mapStorage));
    state = reinterpret_cast<CHandleMap*>(mapStorage);
    ownedModule = nullptr;
    api.thread()->m_pModuleState = reinterpret_cast<AFX_MODULE_STATE*>(modules[0]);
    api.get()->m_pfnNewHandler = &handler;
    api.thread()->m_pModuleState = reinterpret_cast<AFX_MODULE_STATE*>(modules[1]);
    api.get()->m_pfnNewHandler = &handler;
    api.thread()->m_pModuleState = reinterpret_cast<AFX_MODULE_STATE*>(modules[0]);
    api.thread()->m_pSafetyPoolBuffer = nullptr;
    capture = true;
}
void normalizeModule(BYTE* copy) {
    for (size_t offset : {size_t(0), size_t(0x50), size_t(0x60), size_t(0x98), size_t(0xd0)})
        memset(copy + offset, 0, 8);
}
void snapshot() {
    event(20, state->m_nAllocSize, state->m_nBlockSize, id(state->m_pBlocks),
          id(state->m_pNodeFree), state->m_nOffset, state->m_nHandles);
    for (auto* map : {&state->m_permanentMap, &state->m_temporaryMap})
        event(21, id(map->m_pHashTable), map->m_nHashTableSize, map->m_nCount, id(map->m_pFreeList),
              id(map->m_pBlocks), map->m_nBlockSize);
    auto current = active->get()->m_pfnNewHandler;
    event(22, current == &handler, current == active->pressure,
          active->thread()->m_pModuleState == reinterpret_cast<AFX_MODULE_STATE*>(modules[1]));
}
void compare(const Trace& source, const Trace& native, const char* label) {
    bool equal = source.count == native.count;
    size_t i = 0;
    for (; i < source.count && i < native.count; ++i)
        if (memcmp(&source.events[i], &native.events[i], sizeof(Event))) {
            equal = false;
            break;
        }
    if (!equal) {
        printf("Mismatch %zu of %zu/%zu\n", i, source.count, native.count);
        for (size_t j = i > 2 ? i - 2 : 0; j < i + 4; ++j) {
            printf("%zu source:", j);
            if (j < source.count)
                for (auto v : source.events[j].v)
                    printf(" %lld", v);
            printf(" native:");
            if (j < native.count)
                for (auto v : native.events[j].v)
                    printf(" %lld", v);
            puts("");
        }
    }
    check(equal, label);
}
void constructors(const API& api, Trace& out) {
    out.count = 0;
    reset(api, out);
    api.moduleConstruct(reinterpret_cast<AFX_MODULE_THREAD_STATE*>(storage));
    BYTE normalized[sizeof(storage)];
    memcpy(normalized, storage, sizeof(storage));
    normalizeModule(normalized);
    for (size_t i = 0; i < sizeof(normalized); i += 8) {
        INT64 word;
        memcpy(&word, normalized + i, 8);
        event(30, i, word);
    }
    for (unsigned size : {0u, 8u, 32u, UINT_MAX})
        for (int bad : {0, 1, 2}) {
            reset(api, out);
            wrapperClass.m_nObjectSize = size;
            int caught = invoke([&] {
                api.mapConstruct(state, &wrapperClass, bad == 1 ? nullptr : &construct,
                                 bad == 2 ? nullptr : &destruct, 16, 2);
            });
            BYTE copy[184];
            memcpy(copy, mapStorage, sizeof(copy));
            memset(copy + 0x28, 0, 8);
            memset(copy + 0x60, 0, 8);
            for (size_t offset : {size_t(0x18), size_t(0x20), size_t(0x98)}) {
                UINT_PTR value;
                memcpy(&value, copy + offset, 8);
                if (value == reinterpret_cast<UINT_PTR>(&construct))
                    value = 1;
                if (value == reinterpret_cast<UINT_PTR>(&destruct))
                    value = 2;
                if (value == reinterpret_cast<UINT_PTR>(&wrapperClass))
                    value = 3;
                memcpy(copy + offset, &value, 8);
            }
            event(31, caught);
            for (size_t i = 0; i < sizeof(copy); i += 8) {
                INT64 word;
                memcpy(&word, copy + i, 8);
                event(32, i, word);
            }
        }
    wrapperClass.m_nObjectSize = 32;
    capture = false;
}
void maps(const API& api, Trace& out) {
    out.count = 0;
    for (int count : {1, 2, 3})
        for (int mode : {0, 1, 2, 3, 4})
            for (int fail : {0, 1, 2, 3}) {
                reset(api, out);
                api.mapConstruct(state, &wrapperClass, &construct, &destruct, 16, count);
                mutation = mode;
                missing = fail;
                auto caught = invoke([&] {
                    auto* a = api.from(state, reinterpret_cast<void*>(0x1234));
                    event(33, id(a));
                    if (!a)
                        return;
                    event(34, id(api.from(state, reinterpret_cast<void*>(0x1234))));
                    event(35, id(api.from(state, reinterpret_cast<void*>(0x5678))));
                });
                event(36, caught);
                snapshot();
                at = missing = mutation = 0;
                api.clear(state);
                snapshot();
            }
    capture = false;
}
void interruptedMaps(const API& api, Trace& out) {
    out.count = 0;
    for (int handles : {1, 2, 3})
        for (int count : {0, 1, 3})
            for (int failure : {0, 2, 7})
                for (bool framework : {false, true}) {
                    reset(api, out);
                    api.mapConstruct(state, &wrapperClass, &construct, &destruct, 16, handles);
                    auto* permanent = reinterpret_cast<CObject*>(bytes(32));
                    construct(permanent);
                    *api.set(&state->m_permanentMap, reinterpret_cast<void*>(0x4444)) = permanent;
                    event(48, api.from(state, reinterpret_cast<void*>(0x4444)) == permanent);
                    for (int i = 0; i < count; ++i)
                        api.from(state, reinterpret_cast<void*>(UINT_PTR(0x1000 + i)));
                    at = failure;
                    pointerFailure = framework;
                    auto caught = invoke([&] { api.clear(state); });
                    event(49, caught);
                    snapshot();
                }
    capture = false;
}
CObject* structuredCall(const API* api, CHandleMap* map, bool* caught) {
    __try {
        return api->from(map, reinterpret_cast<void*>(0x1234));
    } __except (GetExceptionCode() == 0xe0424d52 ? EXCEPTION_EXECUTE_HANDLER
                                                 : EXCEPTION_CONTINUE_SEARCH) {
        *caught = true;
        return nullptr;
    }
}
void structured(const API& api, Trace& out) {
    out.count = 0;
    reset(api, out);
    api.mapConstruct(state, &wrapperClass, &construct, &destruct, 16, 2);
    at = 6;
    structuredFailure = true;
    bool caught = false;
    auto* value = structuredCall(&api, state, &caught);
    event(50, caught, id(value));
    snapshot();
    at = 0;
    structuredFailure = false;
    api.clear(state);
    capture = false;
}
void globalNew(const API& api, Trace& out) {
    out.count = 0;
    for (size_t size : {size_t(0), size_t(1), size_t(32)})
        for (int mode : {0, 1, 2, 3, 4, 5, 6, 7})
            for (int count : {0, 1, 2, 3}) {
                reset(api, out);
                if (mode == 1)
                    api.setHandler(nullptr);
                if (mode == 2)
                    api.setHandler(api.defaultHandler);
                if (mode == 4)
                    api.setHandler(&decline);
                failuresRemaining = count;
                if (mode == 3 || mode == 7)
                    at = 5;
                if (mode == 5)
                    mutation = 8;
                if (mode == 6)
                    mutation = 9;
                pointerFailure = mode == 7;
                auto caught = invoke([&] { event(37, id(api.allocate(size))); });
                event(38, caught, handlerCalls,
                      active->thread()->m_pModuleState ==
                          reinterpret_cast<AFX_MODULE_STATE*>(modules[1]));
                capture = false;
            }
}
void lazyModule(const API& api, Trace& out) {
    out.count = 0;
    for (size_t size : {size_t(0), size_t(32)})
        for (bool fail : {false, true}) {
            reset(api, out);
            api.thread()->m_pModuleState = reinterpret_cast<AFX_MODULE_STATE*>(modules[2]);
            failuresRemaining = fail ? 1 : 0;
            auto caught = invoke([&] { event(45, id(api.allocate(size))); });
            event(46, caught, *reinterpret_cast<int*>(modules[2] + 0xf8) != 0);
            capture = false;
            api.localDestroy(reinterpret_cast<S_Cthreadlocalobject*>(modules[2] + 0xf8));
        }
}
void pressure(const API& api, Trace& out) {
    out.count = 0;
    for (size_t size : {size_t(0), size_t(1), size_t(16), size_t(76), size_t(77), SIZE_MAX})
        for (bool pool : {false, true})
            for (int mode : {0, 5, 6, 7})
                for (int failure : {0, 2, 3, 4}) {
                    reset(api, out);
                    api.thread()->m_pSafetyPoolBuffer = pool ? bytes(96) : nullptr;
                    at = failure;
                    mutation = mode;
                    int result = 17;
                    auto caught = invoke([&] { result = api.pressure(size); });
                    event(47, result, caught, id(api.thread()->m_pSafetyPoolBuffer));
                    api.thread()->m_pSafetyPoolBuffer = nullptr;
                    capture = false;
                }
}
void tooltips(const API& api, Trace& out) {
    out.count = 0;
    for (bool owner : {false, true})
        for (int fail : {0, 8, 9}) {
            reset(api, out);
            memset(tooltipStorage, 0, sizeof(tooltipStorage));
            tooltipTable[1] = reinterpret_cast<void*>(&deleteTooltip);
            tooltipTable[26] = reinterpret_cast<void*>(&destroyWindow);
            *reinterpret_cast<void***>(tooltipStorage) = tooltipTable;
            *reinterpret_cast<AFX_MODULE_STATE**>(tooltipStorage + 0x38) =
                reinterpret_cast<AFX_MODULE_STATE*>(modules[owner ? 0 : 1]);
            at = fail;
            int result = 17;
            auto caught = invoke(
                [&] { result = api.tooltip(reinterpret_cast<CToolTipCtrl*>(tooltipStorage)); });
            event(39, result, caught);
        }
    capture = false;
}
void setupTeardown(const API& api, int mask, bool tooltip) {
    memset(storage, 0, 0x138);
    ownedModule = api.moduleConstruct(reinterpret_cast<AFX_MODULE_THREAD_STATE*>(storage));
    if (tooltip) {
        memset(tooltipStorage, 0, sizeof(tooltipStorage));
        tooltipTable[1] = reinterpret_cast<void*>(&deleteTooltip);
        tooltipTable[26] = reinterpret_cast<void*>(&destroyWindow);
        *reinterpret_cast<void***>(tooltipStorage) = tooltipTable;
        *reinterpret_cast<AFX_MODULE_STATE**>(tooltipStorage + 0x38) =
            reinterpret_cast<AFX_MODULE_STATE*>(modules[0]);
        ownedModule->m_pToolTip = reinterpret_cast<CToolTipCtrl*>(tooltipStorage);
    }
    ownedModule->m_pLastInfo = allocate(72);
    auto** slots = &ownedModule->m_pmapHWND;
    for (int i = 0; i < 5; ++i)
        if (mask & (1 << i)) {
            auto* map = reinterpret_cast<CHandleMap*>(allocate(sizeof(CHandleMap)));
            api.mapConstruct(map, &wrapperClass, &construct, &destruct, 16, 2);
            slots[i] = map;
            state = map;
            api.from(map, reinterpret_cast<void*>(UINT_PTR(0x1000 + i)));
            *api.set(&map->m_permanentMap, reinterpret_cast<void*>(UINT_PTR(0x2000 + i))) =
                reinterpret_cast<void*>(0x9876);
        }
    state = reinterpret_cast<CHandleMap*>(mapStorage);
    *api.set(&ownedModule->m_mapSocketHandle, reinterpret_cast<void*>(0x5000)) =
        reinterpret_cast<void*>(0x6000);
    *api.set(&ownedModule->m_mapDeadSockets, reinterpret_cast<void*>(0x7000)) =
        reinterpret_cast<void*>(0x8000);
    api.append(&ownedModule->m_listSocketNotifications, allocate(4));
    api.append(&ownedModule->m_listSocketNotifications, allocate(8));
}
void teardown(const API& api, Trace& out) {
    out.count = 0;
    for (int mask : {0, 1, 31})
        for (bool tooltip : {false, true})
            for (int mode : {0, 10}) {
                reset(api, out);
                setupTeardown(api, mask, tooltip);
                mutation = mode;
                api.moduleDestroy(ownedModule);
                event(40, ownedModule->m_listSocketNotifications.m_nCount,
                      ownedModule->m_mapSocketHandle.m_nCount,
                      ownedModule->m_mapDeadSockets.m_nCount);
            }
    capture = false;
}
UINT64 traceHash(const Trace& value) {
    UINT64 hash = 1469598103934665603ULL;
    for (size_t i = 0; i < value.count; ++i)
        for (auto field : value.events[i].v) {
            hash ^= UINT64(field);
            hash *= 1099511628211ULL;
        }
    return hash;
}
void recorded(const Trace& value, UINT64 expected, size_t count, const char* label) {
    bool equal = traceHash(value) == expected && value.count == count;
    if (!equal)
        printf("Recorded native trace mismatch: %llx/%zu expected %llx/%zu\n", traceHash(value),
               value.count, expected, count);
    check(equal, label);
}
void terminateLog() {
    capture = false;
    printf("TERMINATE %llx %zu\n", traceHash(*trace), trace->count);
    ExitProcess(86);
}
[[noreturn]] void terminateChild(const API& api, int eventNumber) {
    localTrace.count = 0;
    reset(api, localTrace);
    setupTeardown(api, 31, true);
    std::set_terminate(&terminateLog);
    if (eventNumber == 11)
        ownedModule->m_listSocketNotifications.m_pNodeHead = nullptr;
    else
        at = eventNumber;
    api.moduleDestroy(ownedModule);
    ExitProcess(87);
}
struct ChildResult {
    DWORD code;
    char line[96];
};
ChildResult runChild(bool native, int eventNumber) {
    wchar_t executable[1024];
    GetModuleFileNameW(nullptr, executable, 1024);
    wchar_t command[1100];
    swprintf_s(command, L"\"%s\"", executable);
    wchar_t number[32];
    swprintf_s(number, L"%d", eventNumber);
    SetEnvironmentVariableW(L"OPENMFC_MODULE_TERMINATE", number);
    SetEnvironmentVariableW(L"OPENMFC_MODULE_TERMINATE_NATIVE", native ? L"1" : L"0");
    SECURITY_ATTRIBUTES security{sizeof(security), nullptr, TRUE};
    HANDLE reader, writer;
    if (!CreatePipe(&reader, &writer, &security, 0))
        std::abort();
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
    DWORD count, total = 0;
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
    SetEnvironmentVariableW(L"OPENMFC_MODULE_TERMINATE", nullptr);
    SetEnvironmentVariableW(L"OPENMFC_MODULE_TERMINATE_NATIVE", nullptr);
    return result;
}
void termination(bool native) {
    bool equal = true;
    for (int at : {2, 7, 8, 9, 11}) {
        auto source = runChild(false, at);
        bool same = source.code == 86 && source.line[0];
        if (native) {
            auto oracle = runChild(true, at);
            same = same && oracle.code == 86 && !strcmp(source.line, oracle.line);
            if (!same)
                printf("Termination case%d source %lu %s native %lu %s\n", at, source.code,
                       source.line, oracle.code, oracle.line);
        }
        equal = equal && same;
    }
    check(equal,
          "module thread destruction preserves native noexcept and interrupted-cleanup traces");
}
struct Worker {
    const API* api;
    int number;
    AFX_MODULE_THREAD_STATE* states[2];
    bool correct;
    HANDLE ready, release;
};
DWORD WINAPI worker(void* value) {
    auto& w = *static_cast<Worker*>(value);
    w.correct = true;
    for (int i = 0; i < 2; ++i) {
        w.api->thread()->m_pModuleState = reinterpret_cast<AFX_MODULE_STATE*>(modules[i]);
        auto* state = w.api->get();
        w.states[i] = state;
        w.correct = w.correct && state->m_pfnNewHandler == w.api->defaultHandler &&
                    state->m_nTempMapLock == 0 && state->m_nSafetyPoolSize == 0x108;
        state->m_nTempMapLock = w.number + i;
        w.api->setHandler(i ? nullptr : &handler);
        w.correct = w.correct && w.api->get() == state && state->m_nTempMapLock == w.number + i &&
                    w.api->getHandler() == (i ? nullptr : &handler);
    }
    SetEvent(w.ready);
    WaitForSingleObject(w.release, INFINITE);
    return 0;
}
void integration(const API& api) {
    capture = false;
    AFX_MODULE_THREAD_STATE* main[2];
    for (int i = 0; i < 2; ++i) {
        api.thread()->m_pModuleState = reinterpret_cast<AFX_MODULE_STATE*>(modules[i]);
        main[i] = api.get();
        main[i]->m_nTempMapLock = 42 + i;
        api.setHandler(i ? nullptr : &handler);
    }
    HANDLE release = CreateEventW(nullptr, TRUE, FALSE, nullptr), threads[4], ready[4];
    Worker workers[4];
    for (int i = 0; i < 4; ++i) {
        ready[i] = CreateEventW(nullptr, TRUE, FALSE, nullptr);
        workers[i] = {&api, 10 + i, {nullptr, nullptr}, false, ready[i], release};
        threads[i] = CreateThread(nullptr, 0, &worker, &workers[i], 0, nullptr);
        if (!threads[i])
            std::abort();
    }
    WaitForMultipleObjects(4, ready, TRUE, INFINITE);
    bool distinct = main[0] != main[1];
    for (int i = 0; i < 4; ++i) {
        distinct = distinct && workers[i].correct && workers[i].states[0] != workers[i].states[1];
        for (int a = 0; a < 2; ++a) {
            for (auto* value : main)
                distinct = distinct && workers[i].states[a] != value;
            for (int j = 0; j < i; ++j)
                for (auto* value : workers[j].states)
                    distinct = distinct && workers[i].states[a] != value;
        }
    }
    for (int i = 0; i < 2; ++i) {
        api.thread()->m_pModuleState = reinterpret_cast<AFX_MODULE_STATE*>(modules[i]);
        distinct = distinct && api.get() == main[i] && main[i]->m_nTempMapLock == 42 + i &&
                   api.getHandler() == (i ? nullptr : &handler);
    }
    check(distinct, "two modules keep independent stable thread state and allocation handlers "
                    "across four workers");
    SetEvent(release);
    WaitForMultipleObjects(4, threads, TRUE, INFINITE);
    for (auto h : threads)
        CloseHandle(h);
    for (auto h : ready)
        CloseHandle(h);
    CloseHandle(release);
}
void moduleLifetime(const API& api) {
    localTrace.count = 0;
    reset(api, localTrace);
    auto* previous = api.thread()->m_pModuleState;
    size_t before;
    void* notify;
    {
        AFX_MODULE_STATE module;
        api.thread()->m_pModuleState = &module;
        auto* state = api.get();
        check(module.m_moduleThreadState.m_nSlot > 0 && api.get() == state,
              "the module-state shadow stores its real cached slot at native offset 0xf8");
        notify = allocate(72);
        state->m_pLastInfo = notify;
        before = localTrace.count;
        api.thread()->m_pModuleState = previous;
    }
    check(localTrace.count == before + 1 && localTrace.events[before].v[0] == 2 &&
              localTrace.events[before].v[1] == id(notify),
          "module-state destruction releases its cached thread state and notification buffer");
    capture = false;
}
void cleanup(const API& api) {
    capture = false;
    api.thread()->m_pModuleState = nullptr;
    for (auto& module : modules) {
        api.localDestroy(reinterpret_cast<S_Cthreadlocalobject*>(module + 0xf8));
        memset(module, 0, sizeof(module));
    }
}
void interoperation(const API& source, const API& oracle) {
    Imports other(GetModuleHandleW(L"mfc140u.dll"), false);
    for (bool nativeConstructs : {false, true}) {
        const API& producer = nativeConstructs ? oracle : source;
        const API& consumer = nativeConstructs ? source : oracle;
        API mixed = consumer;
        mixed.mapConstruct = producer.mapConstruct;
        maps(mixed, localTrace);
        cleanup(mixed);
        maps(consumer, nativeTrace);
        cleanup(consumer);
        compare(localTrace, nativeTrace,
                nativeConstructs ? "source methods consume native-constructed handle maps"
                                 : "native methods consume source-constructed handle maps");
        mixed.moduleConstruct = producer.moduleConstruct;
        teardown(mixed, localTrace);
        cleanup(mixed);
        teardown(consumer, nativeTrace);
        cleanup(consumer);
        compare(localTrace, nativeTrace,
                nativeConstructs
                    ? "source teardown consumes native module/thread and handle-map layouts"
                    : "native teardown consumes source module/thread and handle-map layouts");
    }
}
} // namespace
LRESULT CALLBACK AfxWndProc(HWND, UINT, WPARAM, LPARAM) { std::abort(); }
extern "C" AFX_MODULE_STATE* AFXAPI impl__AfxGetModuleState__YAPEAVAFX_MODULE_STATE__XZ() {
    auto* module = AfxGetThreadState()->m_pModuleState;
    return module ? module : reinterpret_cast<AFX_MODULE_STATE*>(fallbackModule);
}
extern "C" void MS_ABI impl__AfxThrowMemoryException__YAXXZ() { frameworkError(91); }
extern "C" void MS_ABI impl__AfxThrowInvalidArgException__YAXXZ() { frameworkError(19); }
extern "C" void MS_ABI impl__AfxThrowOleException__YAXJ_Z(long value) { throw value; }
int main() {
    setvbuf(stdout, nullptr, _IONBF, 0);
    API source{&impl___0AFX_MODULE_THREAD_STATE__QEAA_XZ,
               &impl___1AFX_MODULE_THREAD_STATE__UEAA_XZ,
               &impl__AfxGetModuleThreadState__YAPEAVAFX_MODULE_THREAD_STATE__XZ,
               &impl__AfxGetNewHandler__YAP6AH_K_ZXZ,
               &impl__AfxSetNewHandler__YAP6AH_K_ZP6AH0_Z_Z,
               &impl___2_YAPEAX_K_Z,
               &impl___0CHandleMap__QEAA_PEAUCRuntimeClass__P6AXPEAVCObject___Z2_KH_Z,
               &impl__FromHandle_CHandleMap__QEAAPEAVCObject__PEAX_Z,
               &impl__DeleteTemp_CHandleMap__QEAAXXZ,
               &setMap,
               &appendList,
               &AfxGetThreadState,
               &impl__DestroyToolTipCtrl_CToolTipCtrl__QEAAHXZ,
               &impl___1CThreadLocalObject__QEAA_XZ,
               &openmfc::detail::modulethreadstate::HandleAllocationFailure,
               &openmfc::detail::modulethreadstate::DefaultAllocationFailure};
    API oracle = source;
    HMODULE native = nullptr;
    wchar_t path[1024];
    if (GetEnvironmentVariableW(L"OPENMFC_NATIVE_MODULE_RUNTIME_DLL", path, 1024)) {
        native = LoadLibraryW(path);
        check(native != nullptr, "load native module runtime oracle");
        if (!native)
            return 1;
        auto* b = reinterpret_cast<BYTE*>(native);
        auto* pe = reinterpret_cast<IMAGE_NT_HEADERS*>(
            b + reinterpret_cast<IMAGE_DOS_HEADER*>(b)->e_lfanew);
        bool pinned = pe->FileHeader.TimeDateStamp == 0x442cba30 &&
                      pe->OptionalHeader.SizeOfImage == 0x55c000;
        check(pinned, "module runtime oracle is the verified MFC build");
        if (!pinned)
            return 1;
#define RVA(field, offset) oracle.field = reinterpret_cast<decltype(oracle.field)>(b + offset)
        RVA(moduleConstruct, 0x133580);
        RVA(moduleDestroy, 0x1336b0);
        RVA(get, 0x133a20);
        RVA(getHandler, 0x8a8c0);
        RVA(setHandler, 0x8a8e0);
        RVA(allocate, 0x27f0);
        RVA(mapConstruct, 0x2a5fb0);
        RVA(from, 0x2a6080);
        RVA(clear, 0x2a6170);
        RVA(set, 0x232e40);
        RVA(append, 0x231e70);
        RVA(thread, 0x1332a0);
        RVA(tooltip, 0x274e20);
        RVA(localDestroy, 0x14d0d0);
        RVA(pressure, 0x2ae6b0);
        RVA(defaultHandler, 0x8a8b0);
#undef RVA
    }
    wchar_t terminationCase[32];
    if (GetEnvironmentVariableW(L"OPENMFC_MODULE_TERMINATE", terminationCase, 32)) {
        wchar_t nativeMode[2];
        GetEnvironmentVariableW(L"OPENMFC_MODULE_TERMINATE_NATIVE", nativeMode, 2);
        Imports imports(GetModuleHandleW(nullptr), true);
        if (nativeMode[0] == L'1') {
            if (!native)
                ExitProcess(88);
            Imports other(native, false);
            terminateChild(oracle, _wtoi(terminationCase));
        }
        terminateChild(source, _wtoi(terminationCase));
    }
    {
        Imports imports(GetModuleHandleW(nullptr), true);
// Recorded from the pinned native image; arena addresses and function/data
// addresses are normalized so Windows CI can also check these traces.
#define RUN(fn, hash, count, label)                                                                \
    fn(source, localTrace);                                                                        \
    recorded(localTrace, hash, count, label);                                                      \
    cleanup(source);                                                                               \
    if (native) {                                                                                  \
        Imports other(native, false);                                                              \
        fn(oracle, nativeTrace);                                                                   \
        cleanup(oracle);                                                                           \
        compare(localTrace, nativeTrace, label);                                                   \
    }
        RUN(constructors, 0x1b2ddd2e1dc290e9ULL, 329,
            "module/thread and handle-map construction match native fields and rejected callbacks");
        RUN(maps, 0xf4a44fb3ce5b4f04ULL, 876,
            "temporary handle allocation, callbacks, caching, errors and cleanup match native MFC");
        RUN(interruptedMaps, 0xa1459ac0cc0605bfULL, 774,
            "temporary cleanup preserves permanent mappings and interrupted callback state");
        RUN(structured, 0x7eb9210d2b5745eaULL, 8,
            "structured exceptions preserve the native temporary allocation handler");
        RUN(globalNew, 0x8cb8dda7aa450063ULL, 387,
            "global allocation sizes, lazy handlers and retries match native MFC");
        RUN(lazyModule, 0x966e6afc2f57132bULL, 10,
            "module thread state is initialized only after a failed global allocation");
        RUN(pressure, 0x974cba5ea93f2e73ULL, 360,
            "safety-pool release, shrink, wrapping sizes and callback mutations match native MFC");
        RUN(tooltips, 0x79a1c3ff39aa5312ULL, 11,
            "tooltip module ownership and virtual destruction match native MFC");
        RUN(teardown, 0xbbd7173b14b9954bULL, 538,
            "module thread resource destruction order matches native MFC");
#undef RUN
        if (native)
            interoperation(source, oracle);
    }
    {
        Imports imports(GetModuleHandleW(nullptr), true);
        moduleLifetime(source);
        cleanup(source);
    }
    integration(source);
    cleanup(source);
    if (native) {
        integration(oracle);
        cleanup(oracle);
    }
    termination(native != nullptr);
    if (native)
        FreeLibrary(native);
    printf("%d failures\n", failures);
    return failures ? 1 : 0;
}
