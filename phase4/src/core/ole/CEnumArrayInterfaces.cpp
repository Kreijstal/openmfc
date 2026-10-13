// Shared native CEnumArray COM entry points, including the MFC catch contract.
#define OPENMFC_APPCORE_IMPL
#include "detail/CEnumArrayInterfacesSupport.h"
#include "detail/OleExceptionTranslationSupport.h"
namespace {
CCmdTarget* Owner(void* self) { return reinterpret_cast<CCmdTarget*>(static_cast<char*>(self)-0x68); }
template<class T> T& Field(void* owner,size_t offset) { return *reinterpret_cast<T*>(static_cast<char*>(owner)+offset); }
template<class Fn> Fn Slot(void* owner,size_t index) { return reinterpret_cast<Fn>((*static_cast<void***>(owner))[index]); }
}
// Symbol: ?Next@XEnumVOID@CEnumArray@@UEAAJKPEAXPEAK@Z
extern "C" long MS_ABI impl__Next_XEnumVOID_CEnumArray__UEAAJKPEAXPEAK_Z(
    void* self,unsigned long requested,void* out,unsigned long* fetched) {
    if(fetched)*fetched=0;
    if(requested && !out)return E_POINTER;
    auto* owner=Owner(self);
    openmfc::detail::CmdTargetModuleScope module(owner->m_pModuleState);
    using OnNext=BOOL (MS_ABI*)(void*,void*);
    try {
        unsigned long copied=0;
        auto* current=static_cast<unsigned char*>(out);
        while(copied<requested && Slot<OnNext>(owner,22)(owner,current)) {
            current+=Field<size_t>(owner,0x40);++copied;
        }
        if(fetched)*fetched=copied;
        return copied==requested ? S_OK : S_FALSE;
    } catch(CException* error) {
        openmfc::detail::FrameworkExceptionCleanup cleanup{error};
        return E_UNEXPECTED;
    }
}
// Symbol: ?Skip@XEnumVOID@CEnumArray@@UEAAJK@Z
extern "C" long MS_ABI impl__Skip_XEnumVOID_CEnumArray__UEAAJK_Z(void* self,unsigned long requested) {
    auto* owner=Owner(self);
    openmfc::detail::CmdTargetModuleScope module(owner->m_pModuleState);
    using OnSkip=BOOL (MS_ABI*)(void*);
    unsigned long skipped=0;
    try { while(skipped<requested && Slot<OnSkip>(owner,23)(owner))++skipped; }
    catch(CException* error) { openmfc::detail::FrameworkExceptionCleanup cleanup{error}; }
    return skipped==requested ? S_OK : S_FALSE;
}
// Symbol: ?Reset@XEnumVOID@CEnumArray@@UEAAJXZ
extern "C" long MS_ABI impl__Reset_XEnumVOID_CEnumArray__UEAAJXZ(void* self) {
    auto* owner=Owner(self);
    openmfc::detail::CmdTargetModuleScope module(owner->m_pModuleState);
    using OnReset=void (MS_ABI*)(void*);
    Slot<OnReset>(owner,24)(owner);return S_OK;
}
// Symbol: ?Clone@XEnumVOID@CEnumArray@@UEAAJPEAPEAUIEnumVOID@@@Z
extern "C" long MS_ABI impl__Clone_XEnumVOID_CEnumArray__UEAAJPEAPEAUIEnumVOID___Z(void* self,void** out) {
    if(!out)return E_POINTER;
    *out=nullptr;
    auto* owner=Owner(self);
    openmfc::detail::CmdTargetModuleScope module(owner->m_pModuleState);
    using OnClone=void* (MS_ABI*)(void*);
    try {
        void* clone=Slot<OnClone>(owner,25)(owner);
        if(!clone)return E_UNEXPECTED;
        void* original=Field<void*>(owner,0x48);
        if(!original)original=owner;
        Field<void*>(clone,0x48)=original;
        InterlockedIncrement(&static_cast<CCmdTarget*>(original)->m_dwRef);
        *out=static_cast<char*>(clone)+0x68;return S_OK;
    } catch(CException* error) {
        openmfc::detail::FrameworkExceptionCleanup cleanup{error};
        return E_UNEXPECTED;
    }
}
