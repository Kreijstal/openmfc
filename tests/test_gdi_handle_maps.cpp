// CGdiObject handle ownership/lifetime checks with an optional pinned MFC oracle.
#define OPENMFC_APPCORE_IMPL
#include "detail/ModuleThreadStateSupport.h"
#include "detail/CHandleMapSupport.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>

extern "C" {
CGdiObject* MS_ABI impl__FromHandle_CGdiObject__SAPEAV1_PEAX_Z(HGDIOBJ);
int MS_ABI impl__Attach_CGdiObject__QEAAHPEAX_Z(CGdiObject*, HGDIOBJ);
HGDIOBJ MS_ABI impl__Detach_CGdiObject__QEAAPEAXXZ(CGdiObject*);
int MS_ABI impl__DeleteObject_CGdiObject__QEAAHXZ(CGdiObject*);
void MS_ABI impl__DeleteTempMap_CGdiObject__SAXXZ();
AFX_MODULE_THREAD_STATE* MS_ABI impl__AfxGetModuleThreadState__YAPEAVAFX_MODULE_THREAD_STATE__XZ();
void* AFXAPI impl___2_YAPEAX_K_Z(size_t);
void MS_ABI impl__AfxThrowInvalidArgException__YAXXZ();
void MS_ABI impl__AfxThrowMemoryException__YAXXZ();
}

IMPLEMENT_DYNAMIC(CGdiObject, CObject)

// The standalone fixture calls the same C++ destructor used by temporary-map cleanup.
namespace {
bool inspectDestructorVptr;
bool destructorVptrWasValid;
}
int CGdiObject::DeleteObject() {
    if (inspectDestructorVptr)
        destructorVptrWasValid = GetRuntimeClass() == &CGdiObject::classCGdiObject;
    return impl__DeleteObject_CGdiObject__QEAAHXZ(this);
}

namespace {
int failures;
thread_local unsigned activeModule;
alignas(16) thread_local BYTE moduleStates[2][sizeof(AFX_MODULE_THREAD_STATE)]{};
thread_local void (*nextGetterCallback)();

void check(bool ok, const char* label) {
    std::printf("%s: %s\n", ok ? "PASS" : "FAIL", label);
    failures += !ok;
}

AFX_MODULE_THREAD_STATE* state(unsigned module) {
    return reinterpret_cast<AFX_MODULE_THREAD_STATE*>(moduleStates[module]);
}

void resetFixture() {
    for (auto& bytes : moduleStates)
        std::memset(bytes, 0, sizeof(bytes));
    activeModule = 0;
    nextGetterCallback = nullptr;
}

void cleanupSourceMaps() {
    for (unsigned i = 0; i != 2; ++i) {
        auto* current = state(i);
        if (current->m_pmapHGDIOBJ) {
            current->m_pmapHGDIOBJ->~CHandleMap();
            std::free(current->m_pmapHGDIOBJ);
            current->m_pmapHGDIOBJ = nullptr;
        }
    }
}

struct API {
    CGdiObject* (MS_ABI* from)(HGDIOBJ);
    int (MS_ABI* attach)(CGdiObject*, HGDIOBJ);
    HGDIOBJ (MS_ABI* detach)(CGdiObject*);
    int (MS_ABI* remove)(CGdiObject*);
    void (MS_ABI* clear)();
    bool source;
};

struct ObjectStorage {
    alignas(CGdiObject) BYTE bytes[sizeof(CGdiObject)];
    CGdiObject* get() { return reinterpret_cast<CGdiObject*>(bytes); }
    CGdiObject* construct() { return ::new (bytes) CGdiObject; }
    void destroy() { get()->~CGdiObject(); }
};

int MS_ABI preservedHandler(size_t) { return FALSE; }

bool isGdiObject(CGdiObject* object) {
    if (!object || !object->m_hObject)
        return false;
    CRuntimeClass* type = object->GetRuntimeClass();
    return type && type->m_lpszClassName && !std::strcmp(type->m_lpszClassName, "CGdiObject") &&
           type->m_nObjectSize == sizeof(CGdiObject);
}

void objectVptrLifecycle() {
    ObjectStorage storage;
    auto* object = storage.construct();
    void* constructedVptr = nullptr;
    std::memcpy(&constructedVptr, storage.bytes, sizeof(constructedVptr));
    check(constructedVptr != nullptr && object->GetRuntimeClass() == &CGdiObject::classCGdiObject,
          "CGdiObject constructor installs its runtime-class vptr");
    inspectDestructorVptr = true;
    destructorVptrWasValid = false;
    storage.destroy();
    inspectDestructorVptr = false;
    check(destructorVptrWasValid,
          "CGdiObject destructor body observes its class vptr before DeleteObject");
}

void basicOperations(const API& api, bool testInvalidHandle) {
    ObjectStorage storage;
    auto* wrapper = storage.construct();
    if (api.source)
        state(0)->m_pfnNewHandler = &preservedHandler;
    api.clear();
    check(true, "DeleteTempMap on an uninitialized handle map is safe");
    check(api.from(nullptr) == nullptr, "FromHandle(NULL) returns NULL");
    if (api.source)
        check(state(0)->m_pmapHGDIOBJ &&
                  state(0)->m_pfnNewHandler == &preservedHandler,
              "map creation restores the module's prior allocation handler");
    check(api.attach(wrapper, nullptr) == FALSE, "Attach(NULL) fails");
    check(api.detach(wrapper) == nullptr && wrapper->m_hObject == nullptr,
          "Detach on an empty wrapper returns NULL");
    check(api.remove(wrapper) == FALSE, "DeleteObject on an empty wrapper returns FALSE");
    api.clear();

    HGDIOBJ external = ::CreatePen(PS_SOLID, 2, RGB(12, 34, 56));
    check(external != nullptr, "create an external GDI pen");
    if (!external) {
        storage.destroy();
        return;
    }
    auto* temporary = api.from(external);
    check(temporary && temporary->m_hObject == external && isGdiObject(temporary),
          "FromHandle constructs a correctly typed temporary wrapper");
    check(api.from(external) == temporary, "FromHandle reuses its temporary wrapper");
    api.clear();
    LOGPEN pen{};
    check(::GetObjectW(external, sizeof(pen), &pen) == sizeof(pen),
          "DeleteTempMap does not destroy an external GDI handle");
    auto* recreated = api.from(external);
    check(recreated && recreated->m_hObject == external,
          "FromHandle rematerializes a wrapper after DeleteTempMap");
    api.clear();

    check(api.attach(wrapper, external) != FALSE, "Attach registers an external handle");
    check(api.from(external) == wrapper, "FromHandle prefers a permanent wrapper");
    check(api.detach(wrapper) == external, "Detach returns the attached handle");
    check(wrapper->m_hObject == nullptr, "Detach clears the wrapper handle");
    check(::GetObjectW(external, sizeof(pen), &pen) == sizeof(pen),
          "Detach leaves the GDI handle alive and caller-owned");

    check(api.attach(wrapper, external) != FALSE, "reattach handle for DeleteObject");
    check(api.remove(wrapper) != FALSE, "DeleteObject destroys an owned GDI handle");
    check(wrapper->m_hObject == nullptr && ::GetObjectW(external, sizeof(pen), &pen) == 0,
          "successful DeleteObject detaches the wrapper and destroys the handle");

    if (testInvalidHandle) {
        HGDIOBJ invalid = reinterpret_cast<HGDIOBJ>(static_cast<UINT_PTR>(0x1234));
        check(api.attach(wrapper, invalid) != FALSE, "Attach accepts a non-NULL opaque handle");
        check(api.remove(wrapper) == FALSE, "DeleteObject reports Win32 failure for an invalid handle");
        check(wrapper->m_hObject == nullptr,
              "failed DeleteObject still detaches and clears the wrapper");
    }
    storage.destroy();
}

void reattachment(const API& api) {
    ObjectStorage storage;
    auto* wrapper = storage.construct();
    HGDIOBJ first = ::CreatePen(PS_SOLID, 1, RGB(1, 2, 3));
    HGDIOBJ second = ::CreatePen(PS_SOLID, 1, RGB(4, 5, 6));
    check(first && second, "create handles for reattachment test");
    if (!first || !second) {
        if (first) ::DeleteObject(first);
        if (second) ::DeleteObject(second);
        storage.destroy();
        return;
    }
    check(api.attach(wrapper, first) && api.attach(wrapper, second),
          "reattaching a wrapper to another handle succeeds");
    check(api.from(first) == wrapper && api.from(second) == wrapper,
          "native Attach overwrites the new key without purging the previous key");
    check(api.detach(wrapper) == second, "Detach returns the most recently attached handle");
    check(api.from(first) == wrapper && api.from(second) != wrapper,
          "Detach removes only the current key; stale earlier attachment remains mapped");
    api.clear();
    ::DeleteObject(first);
    ::DeleteObject(second);
    storage.destroy();
}

CGdiObject* callbackObject;
HGDIOBJ callbackHandle;
void mutateObjectHandle() { callbackObject->m_hObject = callbackHandle; }
void switchToModuleOne() { activeModule = 1; }

void getterBoundaries(const API& api) {
    if (!api.source) {
        check(true, "source-only getter callback boundaries are exercised separately");
        return;
    }
    ObjectStorage storage;
    auto* wrapper = storage.construct();
    HGDIOBJ original = reinterpret_cast<HGDIOBJ>(static_cast<UINT_PTR>(0x1101));
    HGDIOBJ changed = reinterpret_cast<HGDIOBJ>(static_cast<UINT_PTR>(0x2202));
    check(api.attach(wrapper, original) != FALSE, "prepare map before Attach getter callback");
    callbackObject = wrapper;
    callbackHandle = changed;
    nextGetterCallback = &mutateObjectHandle;
    check(api.attach(wrapper, reinterpret_cast<HGDIOBJ>(static_cast<UINT_PTR>(0x3303))) != FALSE,
          "Attach succeeds across a getter callback");
    auto* map = state(activeModule)->m_pmapHGDIOBJ;
    check(wrapper->m_hObject == changed && map->m_permanentMap.GetValueAt(changed) == wrapper,
          "Attach reads m_hObject after the module-state getter callback");
    check(map->m_permanentMap.GetValueAt(original) == wrapper,
          "reattach does not implicitly remove the previous permanent key");

    callbackHandle = original;
    nextGetterCallback = &mutateObjectHandle;
    check(api.detach(wrapper) == changed && wrapper->m_hObject == nullptr,
          "Detach returns the pre-callback handle but clears the post-callback field");
    check(map->m_permanentMap.GetValueAt(changed) == wrapper &&
              map->m_permanentMap.GetValueAt(original) == nullptr,
          "Detach removes the post-callback key, not the returned pre-callback handle");
    map->m_permanentMap.RemoveKey(changed);
    map->m_permanentMap.RemoveKey(original);

    ObjectStorage otherStorage;
    auto* other = otherStorage.construct();
    activeModule = 0;
    HGDIOBJ shared = reinterpret_cast<HGDIOBJ>(static_cast<UINT_PTR>(0x4404));
    api.attach(wrapper, shared);
    activeModule = 1;
    api.attach(other, shared);
    activeModule = 0;
    nextGetterCallback = &switchToModuleOne;
    check(api.detach(wrapper) == shared && wrapper->m_hObject == nullptr,
          "Detach fetches module state after its initial handle read");
    check(state(0)->m_pmapHGDIOBJ->m_permanentMap.GetValueAt(shared) == wrapper &&
              state(1)->m_pmapHGDIOBJ->m_permanentMap.GetValueAt(shared) == nullptr,
          "Detach uses the fresh getter module and leaves the original module entry untouched");
    state(0)->m_pmapHGDIOBJ->m_permanentMap.RemoveKey(shared);
    activeModule = 1;
    api.detach(other);
    storage.destroy();
    otherStorage.destroy();
}

struct ThreadResult {
    const API* api;
    HGDIOBJ handle;
    UINT_PTR wrapper;
    bool valid;
};
DWORD WINAPI isolatedWorker(void* raw) {
    auto& result = *static_cast<ThreadResult*>(raw);
    activeModule = 0;
    auto* wrapper = result.api->from(result.handle);
    result.wrapper = reinterpret_cast<UINT_PTR>(wrapper);
    result.valid = wrapper && wrapper->m_hObject == result.handle && isGdiObject(wrapper);
    result.api->clear();
    cleanupSourceMaps();
    return 0;
}

void interoperate(const API& source, const API& native) {
    cleanupSourceMaps();
    resetFixture();
    ObjectStorage sourceStorage, nativeStorage;
    auto* sourceOwner = sourceStorage.construct();
    auto* nativeOwner = nativeStorage.construct();
    HGDIOBJ handle = ::CreatePen(PS_SOLID, 2, RGB(31, 41, 59));
    check(handle != nullptr, "create a shared GDI handle for native interoperation");
    if (!handle) {
        sourceStorage.destroy();
        nativeStorage.destroy();
        return;
    }
    check(source.attach(sourceOwner, handle) != FALSE,
          "OpenMFC permanently attaches a handle consumed by native MFC");
    auto* nativeTemporary = native.from(handle);
    check(nativeTemporary && nativeTemporary != sourceOwner && nativeTemporary->m_hObject == handle,
          "native MFC wraps the same OS handle in its independent temporary map");
    check(source.from(handle) == sourceOwner,
          "OpenMFC still prefers its own permanent mapping");
    check(source.detach(sourceOwner) == handle && native.attach(nativeOwner, handle),
          "ownership can pass from an OpenMFC wrapper to a native permanent wrapper");
    auto* sourceTemporary = source.from(handle);
    check(sourceTemporary && sourceTemporary != nativeOwner && sourceTemporary->m_hObject == handle,
          "OpenMFC temporary lookup does not borrow native MFC's private map entry");
    check(native.detach(nativeOwner) == handle,
          "native MFC detaches the shared OS handle without destroying it");
    source.clear();
    native.clear();
    LOGPEN pen{};
    check(::GetObjectW(handle, sizeof(pen), &pen) == sizeof(pen),
          "both implementations leave the detached native GDI handle caller-owned");
    ::DeleteObject(handle);
    sourceStorage.destroy();
    nativeStorage.destroy();
}

void isolation(const API& api) {
    cleanupSourceMaps();
    resetFixture();
    HGDIOBJ handle = ::CreatePen(PS_SOLID, 1, RGB(9, 8, 7));
    check(handle != nullptr, "create a shared handle for isolation test");
    if (!handle)
        return;
    CGdiObject* moduleZero = nullptr;
    if (api.source) {
        activeModule = 0;
        moduleZero = api.from(handle);
        activeModule = 1;
        auto* moduleOne = api.from(handle);
        check(moduleZero && moduleOne && moduleZero != moduleOne,
              "temporary wrappers are isolated across module thread states");
    } else {
        moduleZero = api.from(handle);
        check(moduleZero && api.from(handle) == moduleZero,
              "native FromHandle reuses its per-thread temporary wrapper");
    }
    activeModule = 0;
    ThreadResult worker{&api, handle, 0, false};
    HANDLE thread = CreateThread(nullptr, 0, isolatedWorker, &worker, 0, nullptr);
    check(thread != nullptr, "start isolated worker thread");
    if (thread) {
        WaitForSingleObject(thread, INFINITE);
        CloseHandle(thread);
    }
    check(worker.valid && worker.wrapper && worker.wrapper != reinterpret_cast<UINT_PTR>(moduleZero),
          "temporary wrappers are isolated across threads");
    if (api.source) {
        activeModule = 1;
        api.clear();
    }
    activeModule = 0;
    api.clear();
    ::DeleteObject(handle);
    if (api.source)
        cleanupSourceMaps();
}

void* __cdecl allocate(size_t size) { return std::malloc(size); }

} // namespace

extern "C" AFX_MODULE_THREAD_STATE* MS_ABI
impl__AfxGetModuleThreadState__YAPEAVAFX_MODULE_THREAD_STATE__XZ() {
    auto callback = nextGetterCallback;
    nextGetterCallback = nullptr;
    if (callback)
        callback();
    return state(activeModule);
}
extern "C" void* AFXAPI impl___2_YAPEAX_K_Z(size_t size) { return allocate(size); }
extern "C" void MS_ABI impl__AfxThrowInvalidArgException__YAXXZ() { std::abort(); }
extern "C" void MS_ABI impl__AfxThrowMemoryException__YAXXZ() { std::abort(); }
extern "C" AFX_MODULE_THREAD_STATE::NewHandler MS_ABI
impl__AfxSetNewHandler__YAP6AH_K_ZP6AH0_Z_Z(AFX_MODULE_THREAD_STATE::NewHandler handler) {
    auto* current = impl__AfxGetModuleThreadState__YAPEAVAFX_MODULE_THREAD_STATE__XZ();
    auto previous = current->m_pfnNewHandler;
    current->m_pfnNewHandler = handler;
    return previous;
}
_AFX_THREAD_STATE* AFXAPI AfxGetThreadState() { return nullptr; }

int main() {
    std::setvbuf(stdout, nullptr, _IONBF, 0);
    const API source{&impl__FromHandle_CGdiObject__SAPEAV1_PEAX_Z,
                     &impl__Attach_CGdiObject__QEAAHPEAX_Z,
                     &impl__Detach_CGdiObject__QEAAPEAXXZ,
                     &impl__DeleteObject_CGdiObject__QEAAHXZ,
                     &impl__DeleteTempMap_CGdiObject__SAXXZ, true};
    objectVptrLifecycle();
    basicOperations(source, true);
    reattachment(source);
    getterBoundaries(source);
    isolation(source);
    cleanupSourceMaps();

    if (const wchar_t* path = _wgetenv(L"OPENMFC_NATIVE_GDIOBJECT_DLL")) {
        HMODULE image = LoadLibraryW(path);
        check(image != nullptr, "load pinned native MFC oracle");
        if (image) {
            auto* base = reinterpret_cast<BYTE*>(image);
            auto* pe = reinterpret_cast<IMAGE_NT_HEADERS*>(
                base + reinterpret_cast<IMAGE_DOS_HEADER*>(base)->e_lfanew);
            bool pinned = pe->FileHeader.TimeDateStamp == 0x442cba30 &&
                          pe->OptionalHeader.SizeOfImage == 0x55c000;
            check(pinned, "native oracle timestamp and image size match the pinned DLL");
            if (pinned) {
#define RVA(type, offset) reinterpret_cast<type>(base + (offset))
                const API native{RVA(decltype(source.from), 0x2a3ea0),
                                 RVA(decltype(source.attach), 0x2a3ed0),
                                 RVA(decltype(source.detach), 0x2a3f10),
                                 RVA(decltype(source.remove), 0x2a3f60),
                                 RVA(decltype(source.clear), 0x2a6270), false};
#undef RVA
                basicOperations(native, true);
                reattachment(native);
                interoperate(source, native);
                isolation(native);
            }
            FreeLibrary(image);
        }
    }
    cleanupSourceMaps();
    std::printf("CGdiObject handle maps: %d failures\n", failures);
    return failures ? 1 : 0;
}
