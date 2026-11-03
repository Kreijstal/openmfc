// Focused CMenu handle-map and lifetime checks, optionally compared with pinned MFC.
#define OPENMFC_APPCORE_IMPL
#include "detail/CHandleMapSupport.h"
#include "detail/ModuleThreadStateSupport.h"
#include "openmfc/afxwin.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <initializer_list>

namespace openmfc { namespace detail { CHandleMap* GetMenuHandleMap(bool create); } }

extern "C" {
CMenu* MS_ABI impl___0CMenu__QEAA_XZ(CMenu*);
int MS_ABI impl__CreateMenu_CMenu__QEAAHXZ(CMenu*);
int MS_ABI impl__CreatePopupMenu_CMenu__QEAAHXZ(CMenu*);
int MS_ABI impl__LoadMenuW_CMenu__QEAAHI_Z(CMenu*, UINT);
int MS_ABI impl__LoadMenuW_CMenu__QEAAHPEB_W_Z(CMenu*, const wchar_t*);
int MS_ABI impl__DestroyMenu_CMenu__QEAAHXZ(CMenu*);
int MS_ABI impl__Attach_CMenu__QEAAHPEAUHMENU_____Z(CMenu*, HMENU);
HMENU MS_ABI impl__Detach_CMenu__QEAAPEAUHMENU____XZ(CMenu*);
CMenu* MS_ABI impl__FromHandle_CMenu__SAPEAV1_PEAUHMENU_____Z(HMENU);
CMenu* MS_ABI impl__FromHandlePermanent_CMenu__SAPEAV1_PEAUHMENU_____Z(HMENU);
void MS_ABI impl__DeleteTempMap_CMenu__SAXXZ();
void* AFXAPI impl___2_YAPEAX_K_Z(size_t);
void MS_ABI impl__AfxThrowInvalidArgException__YAXXZ();
void MS_ABI impl__AfxThrowMemoryException__YAXXZ();
}

namespace {
int failures;
unsigned resourceLookups;
bool resourceTypesCorrect = true;
thread_local unsigned activeModule;
alignas(16) thread_local BYTE moduleStorage[2][sizeof(AFX_MODULE_THREAD_STATE)]{};
thread_local unsigned getterCalls;
thread_local AFX_MODULE_THREAD_STATE* getterSequence[8]{};
thread_local unsigned getterSequenceCount;
thread_local CMenu* getterMutationMenu;
thread_local HMENU getterMutationHandle;
thread_local unsigned getterMutationAt;

AFX_MODULE_THREAD_STATE* fixtureState() {
    if (getterSequenceCount) {
        const unsigned index = getterCalls < getterSequenceCount ? getterCalls : getterSequenceCount - 1;
        ++getterCalls;
        auto* state = getterSequence[index];
        if (getterMutationMenu && getterCalls == getterMutationAt)
            getterMutationMenu->m_hMenu = getterMutationHandle;
        return state;
    }
    ++getterCalls;
    if (getterMutationMenu && getterCalls == getterMutationAt)
        getterMutationMenu->m_hMenu = getterMutationHandle;
    return reinterpret_cast<AFX_MODULE_THREAD_STATE*>(moduleStorage[activeModule]);
}

struct Object {
    alignas(16) BYTE bytes[sizeof(CMenu)]{};
    CMenu* get() { return reinterpret_cast<CMenu*>(bytes); }
};

using NativeDestructor = CMenu* (MS_ABI*)(CMenu*, UINT);
NativeDestructor nativeDestructor;
void MS_ABI callNativeDestructor(CMenu* menu) {
    nativeDestructor(menu, 0);
}
void MS_ABI callSourceDestructor(CMenu* menu) {
    menu->CMenu::~CMenu();
}

struct API {
    CMenu* (MS_ABI* construct)(CMenu*);
    void (MS_ABI* destroy)(CMenu*);
    int (MS_ABI* attach)(CMenu*, HMENU);
    HMENU (MS_ABI* detach)(CMenu*);
    int (MS_ABI* destroyMenu)(CMenu*);
    CMenu* (MS_ABI* from)(HMENU);
    CMenu* (MS_ABI* permanent)(HMENU);
    void (MS_ABI* deleteTemp)();
    const char* label;
    bool native;
};

void check(bool ok, const char* label) {
    std::printf("%s: %s\n", ok ? "PASS" : "FAIL", label);
    failures += !ok;
}
void resetGetter() {
    getterCalls = 0;
    getterSequenceCount = 0;
    getterMutationMenu = nullptr;
    getterMutationAt = 0;
    getterMutationHandle = nullptr;
}
CHandleMap* sourceMap(unsigned module = 0) {
    return reinterpret_cast<AFX_MODULE_THREAD_STATE*>(moduleStorage[module])->m_pmapHMENU;
}
void cleanupSourceMaps() {
    getterSequenceCount = 0;
    getterMutationMenu = nullptr;
    for (unsigned i = 0; i != 2; ++i) {
        auto* state = reinterpret_cast<AFX_MODULE_THREAD_STATE*>(moduleStorage[i]);
        if (state->m_pmapHMENU) {
            state->m_pmapHMENU->~CHandleMap();
            std::free(state->m_pmapHMENU);
            state->m_pmapHMENU = nullptr;
        }
        std::memset(moduleStorage[i], 0, sizeof(moduleStorage[i]));
    }
    activeModule = 0;
    resetGetter();
}
void constructObject(const API& api, Object& object) {
    api.construct(object.get());
}

void wrapperAndOwnership(const API& api) {
    Object owner;
    constructObject(api, owner);
    check(owner.get()->m_hMenu == nullptr, "constructor initializes an empty HMENU");

    HMENU external = ::CreatePopupMenu();
    check(external && ::IsMenu(external), "create an external HMENU");
    if (!external) return;

    check(api.from(nullptr) == nullptr, "FromHandle(NULL) returns NULL");
    check(api.permanent(nullptr) == nullptr, "FromHandlePermanent(NULL) returns NULL");
    CMenu* temporary = api.from(external);
    check(temporary && temporary->m_hMenu == external, "FromHandle wraps an external HMENU");
    check(api.from(external) == temporary, "temporary lookup preserves wrapper identity");
    check(api.permanent(external) == nullptr, "temporary wrapper is not permanent");
    if (temporary) {
        auto* runtimeClass = temporary->GetRuntimeClass();
        check(runtimeClass && std::strcmp(runtimeClass->m_lpszClassName, "CMenu") == 0,
              "temporary constructor installs a working CMenu root vptr");
    }
    if (!api.native && temporary)
        *reinterpret_cast<void***>(temporary) = reinterpret_cast<void**>(UINT_PTR(1));
    api.deleteTemp();
    check(::IsMenu(external), "DeleteTempMap does not destroy an external HMENU");
    CMenu* recreated = api.from(external);
    check(recreated && recreated->m_hMenu == external,
          "FromHandle recreates a temporary wrapper after map deletion");
    api.deleteTemp();

    check(api.attach(owner.get(), external) != FALSE, "Attach accepts a non-NULL HMENU");
    check(api.permanent(external) == owner.get(), "Attach registers a permanent wrapper");
    check(api.from(external) == owner.get(), "FromHandle prefers a permanent wrapper");
    check(api.attach(owner.get(), nullptr) == FALSE && owner.get()->m_hMenu == external,
          "Attach(NULL) fails without changing the existing handle");

    HMENU replacement = ::CreatePopupMenu();
    check(replacement && api.attach(owner.get(), replacement) != FALSE,
          "reattaching replaces the wrapper's current handle");
    check(api.permanent(external) == owner.get() && api.permanent(replacement) == owner.get(),
          "Attach preserves the old key and adds the fresh member value");
    check(api.detach(owner.get()) == replacement && owner.get()->m_hMenu == nullptr,
          "Detach returns the snapshotted handle and clears the wrapper");
    check(api.permanent(replacement) == nullptr && api.permanent(external) == owner.get(),
          "Detach removes only the fresh member key (without pointer filtering)");
    check(::IsMenu(external) && ::IsMenu(replacement), "Detach leaves both external menus alive");
    // Replacing an attached handle intentionally leaves the old key behind;
    // overwrite/remove it through Attach/Detach before these stack objects leave scope.
    Object staleKeyCleanup;
    constructObject(api, staleKeyCleanup);
    api.attach(staleKeyCleanup.get(), external);
    api.detach(staleKeyCleanup.get());
    ::DestroyMenu(external);
    ::DestroyMenu(replacement);

    HMENU owned = ::CreatePopupMenu();
    Object ownedWrapper;
    constructObject(api, ownedWrapper);
    api.attach(ownedWrapper.get(), owned);
    api.destroy(ownedWrapper.get());
    check(ownedWrapper.get()->m_hMenu == nullptr && !::IsMenu(owned),
          "CMenu destruction owns and destroys an attached HMENU");

    const HMENU invalid = reinterpret_cast<HMENU>(UINT_PTR(0x1234));
    Object failed;
    constructObject(api, failed);
    check(api.attach(failed.get(), invalid) != FALSE, "Attach accepts an opaque non-NULL handle");
    check(api.destroyMenu(failed.get()) == FALSE,
          "DestroyMenu reports the OS failure for an invalid handle");
    check(failed.get()->m_hMenu == nullptr && api.permanent(invalid) == nullptr,
          "failed DestroyMenu still detaches and clears the permanent mapping");
    api.destroy(failed.get());
    api.destroy(owner.get());
}

void sourcePublicMethods() {
    Object object;
    impl___0CMenu__QEAA_XZ(object.get());
    HMENU handle = ::CreatePopupMenu();
    check(object.get()->Attach(handle) != FALSE && CMenu::FromHandlePermanent(handle) == object.get(),
          "public Attach and FromHandlePermanent wrappers forward to the native aliases");
    check(CMenu::FromHandle(handle) == object.get(),
          "public FromHandle wrapper resolves the permanent mapping");
    CMenu::DeleteTempMap();
    check(object.get()->Detach() == handle && object.get()->GetSafeHmenu() == nullptr,
          "public Detach wrapper clears the handle without destroying it");
    check(::IsMenu(handle), "public Detach preserves the external menu");
    ::DestroyMenu(handle);

    HMENU owned = ::CreatePopupMenu();
    object.get()->Attach(owned);
    check(object.get()->DestroyMenu() != FALSE && object.get()->GetSafeHmenu() == nullptr &&
              !::IsMenu(owned),
          "public DestroyMenu wrapper detaches and destroys the native menu");
    object.get()->CMenu::~CMenu();
}

void sourceCreationAndLoading() {
    Object menu;
    impl___0CMenu__QEAA_XZ(menu.get());
    check(menu.get()->CreateMenu() != FALSE, "CMenu::CreateMenu creates a menu");
    HMENU first = menu.get()->m_hMenu;
    check(first && ::IsMenu(first) && impl__FromHandlePermanent_CMenu__SAPEAV1_PEAUHMENU_____Z(first) == menu.get(),
          "CreateMenu registers through Attach");

    check(menu.get()->CreatePopupMenu() != FALSE, "CMenu::CreatePopupMenu creates a popup menu");
    HMENU second = menu.get()->m_hMenu;
    check(second && ::IsMenu(second) && impl__FromHandlePermanent_CMenu__SAPEAV1_PEAUHMENU_____Z(second) == menu.get(),
          "CreatePopupMenu registers through Attach");
    check(impl__FromHandlePermanent_CMenu__SAPEAV1_PEAUHMENU_____Z(first) == menu.get(),
          "creation follows native Attach and leaves an old permanent key untouched");

    check(menu.get()->LoadMenu(UINT(101)) != FALSE, "LoadMenu(UINT) loads the embedded test resource");
    HMENU loadedById = menu.get()->m_hMenu;
    check(loadedById && ::GetMenuItemCount(loadedById) == 1 &&
              impl__FromHandlePermanent_CMenu__SAPEAV1_PEAUHMENU_____Z(loadedById) == menu.get(),
          "LoadMenu(UINT) attaches and registers the loaded menu");
    check(menu.get()->LoadMenu(L"OPENMFC_TEST_MENU") != FALSE,
          "LoadMenu(resource name) loads the embedded test resource");
    HMENU loadedByName = menu.get()->m_hMenu;
    check(loadedByName && ::GetMenuItemCount(loadedByName) == 1 &&
              impl__FromHandlePermanent_CMenu__SAPEAV1_PEAUHMENU_____Z(loadedByName) == menu.get(),
          "LoadMenu(resource name) attaches and registers the loaded menu");

    check(menu.get()->LoadMenu(UINT(65500)) == FALSE && menu.get()->m_hMenu == loadedByName,
          "failed LoadMenu(NULL) preserves the previous handle, as failed Attach does");
    check(resourceLookups == 3 && resourceTypesCorrect,
          "both LoadMenu overloads search the resource chain using RT_MENU");
    check(impl__DestroyMenu_CMenu__QEAAHXZ(menu.get()) != FALSE,
          "DestroyMenu destroys the current loaded menu");
    check(menu.get()->m_hMenu == nullptr &&
              impl__FromHandlePermanent_CMenu__SAPEAV1_PEAUHMENU_____Z(loadedByName) == nullptr,
          "DestroyMenu removes the current key and clears the wrapper");

    auto* map = sourceMap();
    if (map) {
        for (HMENU stale : {first, second, loadedById}) {
            map->m_permanentMap.RemoveKey(stale);
            if (::IsMenu(stale)) ::DestroyMenu(stale);
        }
    }
}

void getterMutationCases() {
    cleanupSourceMaps();
    auto* state0 = reinterpret_cast<AFX_MODULE_THREAD_STATE*>(moduleStorage[0]);
    auto* state1 = reinterpret_cast<AFX_MODULE_THREAD_STATE*>(moduleStorage[1]);
    alignas(16) BYTE state2Bytes[sizeof(AFX_MODULE_THREAD_STATE)]{};
    auto* state2 = reinterpret_cast<AFX_MODULE_THREAD_STATE*>(state2Bytes);
    AFX_MODULE_THREAD_STATE::NewHandler handler1 = state1->m_pfnNewHandler;
    AFX_MODULE_THREAD_STATE::NewHandler handler2 = state2->m_pfnNewHandler;
    getterSequence[0] = state0;
    getterSequence[1] = state1;
    getterSequence[2] = state2;
    getterSequenceCount = 3;
    getterCalls = 0;
    auto* created = openmfc::detail::GetMenuHandleMap(true);
    check(created && state0->m_pmapHMENU == created,
          "cold menu-map factory stores the map in its original module state");
    check(state1->m_pfnNewHandler == &openmfc::detail::modulethreadstate::HandleAllocationFailure &&
              state2->m_pfnNewHandler == handler1,
          "menu-map factory preserves native fresh-getter handler mutations");
    check(getterCalls == 3, "cold menu-map factory makes the three native state lookups");
    getterSequenceCount = 0;
    cleanupSourceMaps();
    state2->m_pfnNewHandler = handler2;

    auto* map = openmfc::detail::GetMenuHandleMap(true);
    Object object;
    impl___0CMenu__QEAA_XZ(object.get());
    HMENU requested = ::CreatePopupMenu();
    HMENU fresh = ::CreatePopupMenu();
    getterMutationMenu = object.get();
    getterMutationHandle = fresh;
    getterMutationAt = getterCalls + 1;
    check(impl__Attach_CMenu__QEAAHPEAUHMENU_____Z(object.get(), requested) != FALSE,
          "Attach succeeds while the state getter mutates m_hMenu");
    check(object.get()->m_hMenu == fresh && map->m_permanentMap.GetValueAt(fresh) == object.get() &&
              map->m_permanentMap.GetValueAt(requested) == nullptr,
          "Attach registers the freshly read member value after the getter callback");
    getterMutationMenu = nullptr;
    map->m_permanentMap.RemoveKey(fresh);

    HMENU nullKeyHandle = ::CreatePopupMenu();
    Object nullKeyObject;
    impl___0CMenu__QEAA_XZ(nullKeyObject.get());
    getterMutationMenu = nullKeyObject.get();
    getterMutationHandle = nullptr;
    getterMutationAt = getterCalls + 1;
    check(impl__Attach_CMenu__QEAAHPEAUHMENU_____Z(nullKeyObject.get(), nullKeyHandle) != FALSE &&
              nullKeyObject.get()->m_hMenu == nullptr &&
              map->m_permanentMap.GetValueAt(nullptr) == nullKeyObject.get(),
          "Attach inserts the fresh NULL self value if the getter mutates it");
    getterMutationMenu = nullptr;
    map->m_permanentMap.RemoveKey(nullptr);
    if (::IsMenu(nullKeyHandle)) ::DestroyMenu(nullKeyHandle);
    object.get()->m_hMenu = requested;
    map->m_permanentMap[requested] = object.get();

    HMENU changed = ::CreatePopupMenu();
    Object other;
    impl___0CMenu__QEAA_XZ(other.get());
    map->m_permanentMap[changed] = other.get();
    object.get()->m_hMenu = requested;
    getterMutationMenu = object.get();
    getterMutationHandle = changed;
    getterMutationAt = getterCalls + 1;
    HMENU returned = impl__Detach_CMenu__QEAAPEAUHMENU____XZ(object.get());
    check(returned == requested && object.get()->m_hMenu == nullptr,
          "Detach returns its pre-callback handle snapshot");
    check(map->m_permanentMap.GetValueAt(requested) == object.get() &&
              map->m_permanentMap.GetValueAt(changed) == nullptr,
          "Detach removes the fresh self member key without a pointer-equality filter");
    getterMutationMenu = nullptr;
    map->m_permanentMap.RemoveKey(requested);

    HMENU nullDetachHandle = ::CreatePopupMenu();
    Object nullDetachObject;
    impl___0CMenu__QEAA_XZ(nullDetachObject.get());
    nullDetachObject.get()->m_hMenu = nullDetachHandle;
    map->m_permanentMap[nullDetachHandle] = nullDetachObject.get();
    map->m_permanentMap[nullptr] = other.get();
    getterMutationMenu = nullDetachObject.get();
    getterMutationHandle = nullptr;
    getterMutationAt = getterCalls + 1;
    returned = impl__Detach_CMenu__QEAAPEAUHMENU____XZ(nullDetachObject.get());
    check(returned == nullDetachHandle && map->m_permanentMap.GetValueAt(nullptr) == nullptr &&
              map->m_permanentMap.GetValueAt(nullDetachHandle) == nullDetachObject.get(),
          "Detach removes the fresh NULL member key without checking its value first");
    getterMutationMenu = nullptr;
    map->m_permanentMap.RemoveKey(nullDetachHandle);
    if (::IsMenu(nullDetachHandle)) ::DestroyMenu(nullDetachHandle);
    if (::IsMenu(requested)) ::DestroyMenu(requested);
    if (::IsMenu(fresh)) ::DestroyMenu(fresh);
    if (::IsMenu(changed)) ::DestroyMenu(changed);

    Object empty;
    impl___0CMenu__QEAA_XZ(empty.get());
    resetGetter();
    check(impl__Detach_CMenu__QEAAPEAUHMENU____XZ(empty.get()) == nullptr && getterCalls == 0,
          "Detach(NULL) skips the native module-state getter");
    resetGetter();
    check(impl__FromHandlePermanent_CMenu__SAPEAV1_PEAUHMENU_____Z(nullptr) == nullptr && getterCalls == 1,
          "FromHandlePermanent(NULL) still performs the native module-state lookup");
    cleanupSourceMaps();
}

void sourceIsolation(const API& api) {
    cleanupSourceMaps();
    HMENU handle = ::CreatePopupMenu();
    activeModule = 0;
    CMenu* first = api.from(handle);
    CMenu* same = api.from(handle);
    activeModule = 1;
    CMenu* secondModule = api.from(handle);
    check(first && same == first && secondModule && secondModule != first,
          "source temporary wrappers are stable and isolated between module states");

    struct ThreadResult { const API* api; HMENU handle; CMenu* wrapper; } result{&api, handle, nullptr};
    HANDLE thread = CreateThread(nullptr, 0, [](void* raw) -> DWORD {
        auto* result = static_cast<ThreadResult*>(raw);
        activeModule = 0;
        result->wrapper = result->api->from(result->handle);
        result->api->deleteTemp();
        cleanupSourceMaps();
        return 0;
    }, &result, 0, nullptr);
    check(thread != nullptr, "create source map isolation worker");
    if (thread) { WaitForSingleObject(thread, INFINITE); CloseHandle(thread); }
    check(result.wrapper && result.wrapper != first,
          "source temporary wrappers are isolated between threads");
    activeModule = 0; api.deleteTemp();
    activeModule = 1; api.deleteTemp();
    cleanupSourceMaps();
    check(::IsMenu(handle), "isolated temporary-map cleanup preserves the shared HMENU");
    ::DestroyMenu(handle);
}

void nativeIsolation(const API& api) {
    HMENU handle = ::CreatePopupMenu();
    CMenu* first = api.from(handle);
    struct ThreadResult { const API* api; HMENU handle; CMenu* wrapper; } result{&api, handle, nullptr};
    HANDLE thread = CreateThread(nullptr, 0, [](void* raw) -> DWORD {
        auto* result = static_cast<ThreadResult*>(raw);
        result->wrapper = result->api->from(result->handle);
        result->api->deleteTemp();
        return 0;
    }, &result, 0, nullptr);
    check(thread != nullptr, "create native map isolation worker");
    if (thread) { WaitForSingleObject(thread, INFINITE); CloseHandle(thread); }
    check(first && result.wrapper && result.wrapper != first && api.from(handle) == first,
          "native temporary wrappers are stable per thread and isolated across threads");
    api.deleteTemp();
    check(::IsMenu(handle), "native temporary-map cleanup preserves the shared HMENU");
    ::DestroyMenu(handle);
}

void sourceNativeInteroperation(const API& source, const API& native) {
    HMENU shared = ::CreatePopupMenu();
    Object sourceOwner, nativeOwner;
    source.construct(sourceOwner.get());
    native.construct(nativeOwner.get());
    check(source.attach(sourceOwner.get(), shared) && source.from(shared) == sourceOwner.get(),
          "source permanent map wraps a shared native HMENU");
    check(native.from(shared) != sourceOwner.get() && native.permanent(shared) == nullptr,
          "native map keeps its own wrapper state for the same HMENU");
    check(native.attach(nativeOwner.get(), shared) && native.from(shared) == nativeOwner.get(),
          "native Attach and lookup interoperate with a source-attached HMENU");
    check(source.from(shared) == sourceOwner.get(),
          "source permanent wrapper remains isolated from native map registration");
    source.detach(sourceOwner.get());
    native.detach(nativeOwner.get());
    source.deleteTemp();
    native.deleteTemp();
    if (::IsMenu(shared)) ::DestroyMenu(shared);
    cleanupSourceMaps();
}
} // namespace

extern "C" AFX_MODULE_THREAD_STATE* MS_ABI impl__AfxGetModuleThreadState__YAPEAVAFX_MODULE_THREAD_STATE__XZ() {
    return fixtureState();
}
extern "C" void* AFXAPI impl___2_YAPEAX_K_Z(size_t size) { return std::malloc(size); }
extern "C" AFX_MODULE_THREAD_STATE::NewHandler MS_ABI
impl__AfxSetNewHandler__YAP6AH_K_ZP6AH0_Z_Z(AFX_MODULE_THREAD_STATE::NewHandler handler) {
    auto* state = fixtureState();
    auto previous = state->m_pfnNewHandler;
    state->m_pfnNewHandler = handler;
    return previous;
}
extern "C" void MS_ABI impl__AfxThrowInvalidArgException__YAXXZ() { std::abort(); }
extern "C" void MS_ABI impl__AfxThrowMemoryException__YAXXZ() { std::abort(); }
void AFXAPI AfxThrowMemoryException() { std::abort(); }
_AFX_THREAD_STATE* AFXAPI AfxGetThreadState() { return nullptr; }
HINSTANCE AFXAPI AfxGetInstanceHandle() { return ::GetModuleHandleW(nullptr); }
extern "C" HINSTANCE MS_ABI
impl__AfxFindResourceHandle__YAPEAUHINSTANCE____PEB_W0_Z(const wchar_t*, const wchar_t* type) {
    ++resourceLookups;
    resourceTypesCorrect = resourceTypesCorrect && type == RT_MENU;
    return ::GetModuleHandleW(nullptr);
}

int main() {
    std::setvbuf(stdout, nullptr, _IONBF, 0);
    const API source{static_cast<CMenu* (MS_ABI*)(CMenu*)>(&impl___0CMenu__QEAA_XZ),
                     &callSourceDestructor,
                     &impl__Attach_CMenu__QEAAHPEAUHMENU_____Z,
                     &impl__Detach_CMenu__QEAAPEAUHMENU____XZ,
                     &impl__DestroyMenu_CMenu__QEAAHXZ,
                     &impl__FromHandle_CMenu__SAPEAV1_PEAUHMENU_____Z,
                     &impl__FromHandlePermanent_CMenu__SAPEAV1_PEAUHMENU_____Z,
                     &impl__DeleteTempMap_CMenu__SAXXZ, "OpenMFC", false};
    wrapperAndOwnership(source);
    sourcePublicMethods();
    sourceCreationAndLoading();
    getterMutationCases();
    sourceIsolation(source);

    const wchar_t* nativePath = _wgetenv(L"OPENMFC_NATIVE_MENU_DLL");
    HMODULE nativeImage = nativePath ? ::LoadLibraryW(nativePath) : nullptr;
    if (nativePath) check(nativeImage != nullptr, "load pinned native MFC oracle");
    if (nativeImage) {
        auto* image = reinterpret_cast<BYTE*>(nativeImage);
        auto* pe = reinterpret_cast<IMAGE_NT_HEADERS*>(image + reinterpret_cast<IMAGE_DOS_HEADER*>(image)->e_lfanew);
        const bool pinned = pe->FileHeader.TimeDateStamp == 0x442cba30 &&
                            pe->OptionalHeader.SizeOfImage == 0x55c000;
        check(pinned, "native oracle timestamp and image size match the pinned MFC build");
        if (pinned) {
            nativeDestructor = reinterpret_cast<NativeDestructor>(image + 0x2b02e0);
            const API native{
                reinterpret_cast<decltype(source.construct)>(image + 0x2a8cc0),
                &callNativeDestructor,
                reinterpret_cast<decltype(source.attach)>(image + 0x2a8100),
                reinterpret_cast<decltype(source.detach)>(image + 0x2a8140),
                reinterpret_cast<decltype(source.destroyMenu)>(image + 0x2a8190),
                reinterpret_cast<decltype(source.from)>(image + 0x2a80a0),
                reinterpret_cast<decltype(source.permanent)>(image + 0x2a80d0),
                reinterpret_cast<decltype(source.deleteTemp)>(image + 0x2a6290), "native MFC", true};
            wrapperAndOwnership(native);
            nativeIsolation(native);
            sourceNativeInteroperation(source, native);
        }
        ::FreeLibrary(nativeImage);
    }
    cleanupSourceMaps();
    std::printf("CMenu handle map: %d failures\n", failures);
    return failures ? 1 : 0;
}
