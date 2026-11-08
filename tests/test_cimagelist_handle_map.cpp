// Focused CImageList map/lifetime checks against OpenMFC and the pinned MFC oracle.
#define OPENMFC_APPCORE_IMPL
#include "detail/ModuleThreadStateSupport.h"
#include "detail/CHandleMapSupport.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <thread>

extern "C" {
CImageList* MS_ABI impl___0CImageList__QEAA_XZ(CImageList*);
void MS_ABI impl___1CImageList__UEAA_XZ(CImageList*);
int MS_ABI impl__Attach_CImageList__QEAAHPEAU_IMAGELIST___Z(CImageList*, HIMAGELIST);
HIMAGELIST MS_ABI impl__Detach_CImageList__QEAAPEAU_IMAGELIST__XZ(CImageList*);
int MS_ABI impl__DeleteImageList_CImageList__QEAAHXZ(CImageList*);
CImageList* MS_ABI impl__FromHandle_CImageList__SAPEAV1_PEAU_IMAGELIST___Z(HIMAGELIST);
CImageList* MS_ABI impl__FromHandlePermanent_CImageList__SAPEAV1_PEAU_IMAGELIST___Z(HIMAGELIST);
void MS_ABI impl__DeleteTempMap_CImageList__SAXXZ();
void* AFXAPI impl___2_YAPEAX_K_Z(size_t);
void MS_ABI impl__AfxThrowInvalidArgException__YAXXZ();
void MS_ABI impl__AfxThrowMemoryException__YAXXZ();
int MS_ABI impl__Create_CImageList__QEAAHHHIHH_Z(CImageList*,int,int,UINT,int,int);
int MS_ABI impl__Create_CImageList__QEAAHPEAV1__Z(CImageList*,CImageList*);
int MS_ABI impl__Create_CImageList__QEAAHPEB_WHHK_Z(CImageList*,const wchar_t*,int,int,COLORREF);
}

IMPLEMENT_DYNCREATE(CImageList, CObject)

namespace {
int failures;
thread_local unsigned moduleIndex;
thread_local void (*nextGetterCallback)();
alignas(16) thread_local BYTE moduleStateBytes[2][sizeof(AFX_MODULE_THREAD_STATE)]{};
AFX_MODULE_THREAD_STATE* sourceState() {
    return reinterpret_cast<AFX_MODULE_THREAD_STATE*>(moduleStateBytes[moduleIndex]);
}
void check(bool ok, const char* message) {
    std::printf("%s: %s\n", ok ? "PASS" : "FAIL", message);
    failures += !ok;
}
struct API {
    CImageList* (MS_ABI* construct)(CImageList*);
    void (MS_ABI* destroy)(CImageList*);
    int (MS_ABI* attach)(CImageList*, HIMAGELIST);
    HIMAGELIST (MS_ABI* detach)(CImageList*);
    int (MS_ABI* remove)(CImageList*);
    CImageList* (MS_ABI* from)(HIMAGELIST);
    CImageList* (MS_ABI* permanent)(HIMAGELIST);
    void (MS_ABI* clear)();
    const char* label;
    decltype(&impl__Create_CImageList__QEAAHHHIHH_Z) create;
    decltype(&impl__Create_CImageList__QEAAHPEAV1__Z) duplicate;
    decltype(&impl__Create_CImageList__QEAAHPEB_WHHK_Z) load;
};
struct Object {
    alignas(16) BYTE bytes[sizeof(CImageList)];
    CImageList* get() { return reinterpret_cast<CImageList*>(bytes); }
};
static void exercise(const API& api, bool testFailureHandle) {
    Object wrapper;
    auto* self = api.construct(wrapper.get());
    check(self == wrapper.get(), "constructor returns caller storage");
    check(api.from(nullptr) == nullptr, "FromHandle(NULL) returns NULL");
    check(api.permanent(nullptr) == nullptr, "FromHandlePermanent(NULL) returns NULL");
    check(api.attach(self, nullptr) == FALSE, "Attach(NULL) fails without changing the wrapper");
    check(api.remove(self) == FALSE, "DeleteImageList on an empty wrapper returns FALSE");

    HIMAGELIST external = ImageList_Create(13, 11, ILC_COLOR32, 1, 1);
    check(external != nullptr, "create external image list");
    if (!external) return;

    CImageList* temporary = api.from(external);
    check(temporary && temporary->m_hImageList == external, "FromHandle makes a wrapper around the external handle");
    check(api.from(external) == temporary, "FromHandle reuses the temporary wrapper");
    check(api.permanent(external) == nullptr, "temporary wrapper is not permanent");
    api.clear();
    check(ImageList_GetImageCount(external) == 0, "DeleteTempMap preserves the external image list");
    CImageList* recreated = api.from(external);
    check(recreated && recreated->m_hImageList == external,
          "FromHandle rematerializes the wrapper after DeleteTempMap");
    api.clear();

    check(api.attach(self, external) != FALSE, "Attach registers a non-NULL handle");
    check(api.permanent(external) == self, "Attach installs a permanent map entry");
    check(api.from(external) == self, "FromHandle prefers a permanent wrapper");
    check(api.detach(self) == external, "Detach returns the attached handle");
    check(api.permanent(external) == nullptr, "Detach removes the permanent map entry");
    check(self->m_hImageList == nullptr && ImageList_GetImageCount(external) == 0,
          "Detach clears wrapper ownership and leaves the external handle alive");

    check(api.attach(self, external) != FALSE, "reattach handle for deletion");
    check(api.remove(self) != FALSE, "DeleteImageList destroys an attached handle");
    check(self->m_hImageList == nullptr && api.permanent(external) == nullptr,
          "DeleteImageList clears the handle and permanent entry");
    int cx = 0, cy = 0;
    check(!ImageList_GetIconSize(external, &cx, &cy), "deleted handle is no longer valid");

    if (testFailureHandle) {
        HIMAGELIST invalid = reinterpret_cast<HIMAGELIST>(static_cast<UINT_PTR>(0x1234));
        check(api.attach(self, invalid) != FALSE, "Attach accepts a non-NULL opaque handle");
        check(api.remove(self) == FALSE, "DeleteImageList returns Win32 failure for an invalid handle");
        check(self->m_hImageList == nullptr && api.permanent(invalid) == nullptr,
              "failed DeleteImageList still detaches and clears the handle");
    }

    // The exported destructor is a body call; it must establish its own root
    // without dispatching through the caller's incoming vptr.
    *reinterpret_cast<void**>(self) = reinterpret_cast<void*>(UINT_PTR(0xdeadbeef));
    api.destroy(self);
}

static CImageList* mutatedWrapper;
static HIMAGELIST mutatedHandle;
static void mutateHandle() { mutatedWrapper->m_hImageList = mutatedHandle; }
static void sourceGetterMutation(const API& api) {
    Object wrapper;
    auto* self = api.construct(wrapper.get());
    auto first = reinterpret_cast<HIMAGELIST>(UINT_PTR(0x1101));
    auto second = reinterpret_cast<HIMAGELIST>(UINT_PTR(0x2202));
    api.attach(self, first);
    api.attach(self, second);
    self->m_hImageList = first;
    mutatedWrapper = self;
    mutatedHandle = second;
    nextGetterCallback = mutateHandle;
    check(api.detach(self) == first && self->m_hImageList == nullptr,
          "Detach returns the original handle and clears the post-getter field");
    auto* map = sourceState()->m_pmapHIMAGELIST;
    check(map->m_permanentMap.GetValueAt(first) == self &&
          map->m_permanentMap.GetValueAt(second) == nullptr,
          "Detach removes the fresh handle key after the module-state getter");
    map->m_permanentMap.RemoveKey(first);
    api.destroy(self);
}

static void creators(const API& api) {
    Object owner,copy,cleanup;
    auto* self=api.construct(owner.get());
    auto* other=api.construct(copy.get());
    auto* stale=api.construct(cleanup.get());
    check(api.create(self,7,9,ILC_COLOR32,1,1)!=FALSE,"Create succeeds and attaches its image list");
    HIMAGELIST first=self->m_hImageList;
    check(api.permanent(first)==self && api.from(first)==self,"Create publishes the permanent wrapper");
    check(api.duplicate(other,self)!=FALSE && other->m_hImageList!=first &&
          api.permanent(other->m_hImageList)==other,"duplicate Create publishes a separate permanent wrapper");
    check(!api.load(self,L"OpenMFC_missing_bitmap_5e88ba31",7,1,CLR_NONE) &&
          self->m_hImageList==first && api.permanent(first)==self,
          "failed bitmap loading preserves the existing handle and permanent registration");
    check(api.create(self,11,13,ILC_COLOR32,1,1)!=FALSE,"Create can overwrite an existing attachment");
    int cx=0,cy=0;
    check(api.permanent(first)==self && api.permanent(self->m_hImageList)==self &&
          ImageList_GetIconSize(first,&cx,&cy) && cx==7 && cy==9,
          "repeated Create preserves the previous handle and old permanent key");
    api.attach(stale,first);api.detach(stale);ImageList_Destroy(first);
    api.destroy(self);api.destroy(other);api.destroy(stale);
}

static void cleanupSourceMaps();
static void sourceIsolation(const API& api) {
    moduleIndex = 0;
    HIMAGELIST h = ImageList_Create(5, 5, ILC_COLOR32, 1, 1);
    CImageList* first = api.from(h);
    moduleIndex = 1;
    CImageList* secondModule = api.from(h);
    check(first && secondModule && first != secondModule,
          "temporary wrappers are isolated between module thread states");
    moduleIndex = 0;
    std::uintptr_t childWrapper = 0;
    std::thread child([&] {
        moduleIndex = 0;
        childWrapper = reinterpret_cast<std::uintptr_t>(api.from(h));
        cleanupSourceMaps();
    });
    child.join();
    check(childWrapper && childWrapper != reinterpret_cast<std::uintptr_t>(first),
          "temporary wrappers are isolated between threads");
    api.clear();
    moduleIndex = 1;
    api.clear();
    ImageList_Destroy(h);
    moduleIndex = 0;
}

static void cleanupSourceMaps() {
    for (unsigned i = 0; i != 2; ++i) {
        moduleIndex = i;
        auto* state = sourceState();
        if (state->m_pmapHIMAGELIST) {
            state->m_pmapHIMAGELIST->~CHandleMap();
            std::free(state->m_pmapHIMAGELIST);
            state->m_pmapHIMAGELIST = nullptr;
        }
    }
    moduleIndex = 0;
}
}

extern "C" AFX_MODULE_THREAD_STATE* MS_ABI impl__AfxGetModuleThreadState__YAPEAVAFX_MODULE_THREAD_STATE__XZ() {
    auto callback = nextGetterCallback;
    nextGetterCallback = nullptr;
    if (callback) callback();
    return sourceState();
}
extern "C" void* AFXAPI impl___2_YAPEAX_K_Z(size_t size) { return std::malloc(size); }
extern "C" void MS_ABI impl__AfxThrowInvalidArgException__YAXXZ() { std::abort(); }
extern "C" void MS_ABI impl__AfxThrowMemoryException__YAXXZ() { std::abort(); }
extern "C" AFX_MODULE_THREAD_STATE::NewHandler MS_ABI
impl__AfxSetNewHandler__YAP6AH_K_ZP6AH0_Z_Z(AFX_MODULE_THREAD_STATE::NewHandler) { return nullptr; }

HINSTANCE AFXAPI AfxGetInstanceHandle() { return nullptr; }
extern "C" HINSTANCE MS_ABI impl__AfxFindResourceHandle__YAPEAUHINSTANCE____PEB_W0_Z(
    const wchar_t*,const wchar_t* type) {
    check(type==RT_BITMAP,"image-list resource loading requests RT_BITMAP");
    return GetModuleHandleW(nullptr);
}
_AFX_THREAD_STATE* AFXAPI AfxGetThreadState() { return nullptr; }
CArchive& CArchive::operator>>(void*&) { return *this; }
CArchive& CArchive::operator>>(int&) { return *this; }
CArchive& CArchive::operator>>(unsigned int&) { return *this; }
CArchive& CArchive::operator>>(unsigned long&) { return *this; }
CArchive& CArchive::operator<<(const void*) { return *this; }
CArchive& CArchive::operator<<(int) { return *this; }
CArchive& CArchive::operator<<(unsigned int) { return *this; }
CArchive& CArchive::operator<<(unsigned long) { return *this; }

int main() {
    const API source{impl___0CImageList__QEAA_XZ, impl___1CImageList__UEAA_XZ,
                     impl__Attach_CImageList__QEAAHPEAU_IMAGELIST___Z,
                     impl__Detach_CImageList__QEAAPEAU_IMAGELIST__XZ,
                     impl__DeleteImageList_CImageList__QEAAHXZ,
                     impl__FromHandle_CImageList__SAPEAV1_PEAU_IMAGELIST___Z,
                     impl__FromHandlePermanent_CImageList__SAPEAV1_PEAU_IMAGELIST___Z,
                     impl__DeleteTempMap_CImageList__SAXXZ, "OpenMFC",
                     impl__Create_CImageList__QEAAHHHIHH_Z,
                     impl__Create_CImageList__QEAAHPEAV1__Z,
                     impl__Create_CImageList__QEAAHPEB_WHHK_Z};
    exercise(source, true);
    creators(source);
    sourceGetterMutation(source);
    sourceIsolation(source);
    cleanupSourceMaps();

    if (const wchar_t* path = _wgetenv(L"OPENMFC_NATIVE_IMAGELIST_DLL")) {
        HMODULE image = LoadLibraryW(path);
        check(image != nullptr, "load pinned native MFC oracle");
        if (image) {
            auto* base = reinterpret_cast<BYTE*>(image);
            auto* pe = reinterpret_cast<IMAGE_NT_HEADERS*>(base + reinterpret_cast<IMAGE_DOS_HEADER*>(base)->e_lfanew);
            check(pe->FileHeader.TimeDateStamp == 0x442cba30 && pe->OptionalHeader.SizeOfImage == 0x55c000,
                  "native oracle timestamp and image size match the pinned DLL");
            if (pe->FileHeader.TimeDateStamp == 0x442cba30 && pe->OptionalHeader.SizeOfImage == 0x55c000) {
#define RVA(type, offset) reinterpret_cast<type>(base + (offset))
                const API native{
                    RVA(decltype(source.construct), 0x296bc0), RVA(decltype(source.destroy), 0x296be0),
                    RVA(decltype(source.attach), 0x296e60), RVA(decltype(source.detach), 0x296c30),
                    RVA(decltype(source.remove), 0x296c80), RVA(decltype(source.from), 0x296cb0),
                    RVA(decltype(source.permanent), 0x296ce0), RVA(decltype(source.clear), 0x2a6230), "native MFC",
                    RVA(decltype(source.create),0x296d10),RVA(decltype(source.duplicate),0x29c910),
                    RVA(decltype(source.load),0x296db0)};
#undef RVA
                // Native AFX module state is initialized by DLL startup. Its own
                // per-thread maps are exercised independently from the source fixture.
                exercise(native, true);
                creators(native);
            }
            FreeLibrary(image);
        }
    }
    cleanupSourceMaps();
    std::printf("CImageList handle map: %d failures\n", failures);
    return failures ? 1 : 0;
}
