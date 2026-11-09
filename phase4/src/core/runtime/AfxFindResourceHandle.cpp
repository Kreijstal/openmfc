// Resource modules are probed outside CRIT_DYNLINKLIST, then revalidated
// against the captured module's current extension chain before returning.
#define OPENMFC_APPCORE_IMPL
#include "detail/RegcoreSupport.h"
#include "openmfc/afxdll_.h"
#include "detail/StringResourceSupport.h"
#include <cstdlib>
#include <initializer_list>
extern "C" AFX_MODULE_STATE* AFXAPI impl__AfxGetModuleState__YAPEAVAFX_MODULE_STATE__XZ();
extern "C" void AFXAPI impl__AfxLockGlobals__YAXH_Z(int);
extern "C" void AFXAPI impl__AfxUnlockGlobals__YAXH_Z(int);
namespace {
// Native CSimpleArray grows with the CRT, retaining earlier entries when
// growth fails. A later library can retry that same allocation.
struct ResourceModules {
    HINSTANCE* data=nullptr;
    int size=0,capacity=0;
    ~ResourceModules(){if(data)std::free(data);}
    void Clear(){if(data)std::free(data);data=nullptr;size=capacity=0;}
    bool Add(const HINSTANCE& module) {
        if(size==capacity) {
            unsigned next=capacity ? unsigned(size)*2:1;
            if(next>0x0fffffff)return false;
            auto* grown=static_cast<HINSTANCE*>(_recalloc(data,next,sizeof(HINSTANCE)));
            if(!grown)return false;
            capacity=int(next);data=grown;
        }
        data[size++]=module;
        return true;
    }
};
template<bool String,class Probe>
HINSTANCE Find(Probe probe) {
    ResourceModules modules;
    auto* owner=impl__AfxGetModuleState__YAPEAVAFX_MODULE_STATE__XZ();
    if(!owner->m_bSystem) {
        auto module=impl__AfxGetModuleState__YAPEAVAFX_MODULE_STATE__XZ()->m_hCurrentResourceHandle;
        if(probe(module))return module;
    }
    for(bool system:{false,true}) {
        impl__AfxLockGlobals__YAXH_Z(0);
        for(auto* library=static_cast<CDynLinkLibrary*>(owner->m_libraryList.m_pHead);
            library;library=library->m_pNextDLL) {
            if((library->m_bSystem!=FALSE)==system && library->m_hResource) {
                // The string export captures the handle before growth; the
                // general export reloads the library field after allocation.
                if constexpr(String) {
                    auto module=library->m_hResource;modules.Add(module);
                } else modules.Add(library->m_hResource);
            }
        }
        impl__AfxUnlockGlobals__YAXH_Z(0);
        for(int index=0;index<modules.size;++index) {
            auto module=modules.data[index];
            if(!probe(module))continue;
            impl__AfxLockGlobals__YAXH_Z(0);
            for(auto* library=static_cast<CDynLinkLibrary*>(owner->m_libraryList.m_pHead);
                library;library=library->m_pNextDLL) {
                if((library->m_bSystem!=FALSE)==system && library->m_hResource==module) {
                    modules.Clear();
                    impl__AfxUnlockGlobals__YAXH_Z(0);
                    return module;
                }
            }
            impl__AfxUnlockGlobals__YAXH_Z(0);
        }
        modules.Clear();
        if(!system) {
            auto language=owner->m_appLangDLL;
            if(language && probe(language))return language;
            if(owner->m_bSystem) {
                auto module=impl__AfxGetModuleState__YAPEAVAFX_MODULE_STATE__XZ()->m_hCurrentResourceHandle;
                if(probe(module))return module;
            }
        }
    }
    if constexpr(String)return nullptr;
    else return impl__AfxGetModuleState__YAPEAVAFX_MODULE_STATE__XZ()->m_hCurrentResourceHandle;
}
}
// Symbol: ?AfxFindResourceHandle@@YAPEAUHINSTANCE__@@PEB_W0@Z
// Ordinal: 2187
extern "C" HINSTANCE AFXAPI impl__AfxFindResourceHandle__YAPEAUHINSTANCE____PEB_W0_Z(
    const wchar_t* resource,const wchar_t* type) {
    return Find<false>([=](HINSTANCE module){return ::FindResourceW(module,resource,type)!=nullptr;});
}
// Symbol: ?AfxFindStringResourceHandle@@YAPEAUHINSTANCE__@@I@Z
extern "C" void* AFXAPI impl__AfxFindStringResourceHandle__YAPEAUHINSTANCE____I_Z(UINT resource) {
    return Find<true>([=](HINSTANCE module){return openmfc::detail::StringResourceImage(module,resource)!=nullptr;});
}
