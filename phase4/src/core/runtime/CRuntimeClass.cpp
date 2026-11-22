// Module-local runtime class registration, lookup and archive metadata loading.
#define OPENMFC_APPCORE_IMPL
#include "detail/RegcoreSupport.h"
#include "detail/FilecoreSupport.h"
#include "openmfc/afxdll_.h"
#include "openmfc/afxstra.h"

extern "C" void AFXAPI impl__AfxLockGlobals__YAXH_Z(int);
extern "C" void AFXAPI impl__AfxUnlockGlobals__YAXH_Z(int);
extern "C" void AFXAPI impl__AfxThrowInvalidArgException__YAXXZ();

namespace {
constexpr int libraryListLock=0;

CRuntimeClass* FindClass(CRuntimeClass* type,const char* name) {
    for(;type;type=type->m_pNextClass) {
        if(::lstrcmpA(name,type->m_lpszClassName)==0)return type;
    }
    return nullptr;
}
}

void AFXAPI AfxClassInit(CRuntimeClass* type) {
    auto* module=impl__AfxGetModuleState__YAPEAVAFX_MODULE_STATE__XZ();
    impl__AfxLockGlobals__YAXH_Z(libraryListLock);
    if(!type)impl__AfxThrowInvalidArgException__YAXXZ();
    module->m_classList.AddHead(type);
    impl__AfxUnlockGlobals__YAXH_Z(libraryListLock);
}

CRuntimeClass* AFXAPI CRuntimeClass::FromName(const char* name) {
    if(!name)impl__AfxThrowInvalidArgException__YAXXZ();
    auto* module=impl__AfxGetModuleState__YAPEAVAFX_MODULE_STATE__XZ();
    impl__AfxLockGlobals__YAXH_Z(libraryListLock);
    auto* type=FindClass(module->m_classList.GetHead(),name);
    impl__AfxUnlockGlobals__YAXH_Z(libraryListLock);
    if(type)return type;

    impl__AfxLockGlobals__YAXH_Z(libraryListLock);
    for(auto* library=module->m_libraryList.GetHead();library;library=library->m_pNextDLL) {
        type=FindClass(library->m_classList.GetHead(),name);
        if(type)break;
    }
    impl__AfxUnlockGlobals__YAXH_Z(libraryListLock);
    return type;
}

CRuntimeClass* AFXAPI CRuntimeClass::FromName(const wchar_t* name) {
    const CStringA converted(name);
    return name ? FromName(converted.GetString()) : nullptr;
}

CObject* AFXAPI CRuntimeClass::CreateObject(const char* name) {
    auto* type=FromName(name);
    return type ? type->CreateObject() : nullptr;
}

CObject* AFXAPI CRuntimeClass::CreateObject(const wchar_t* name) {
    const CStringA converted(name);
    if(!name)impl__AfxThrowInvalidArgException__YAXXZ();
    return CreateObject(converted.GetString());
}

CRuntimeClass* AFXAPI CRuntimeClass::Load(CArchive& archive,UINT* schema) {
    if(!schema)return nullptr;
    *schema=CArchiveAccess::LoadScalar<WORD>(archive);
    const WORD length=CArchiveAccess::LoadScalar<WORD>(archive);
    char name[64];
    if(length>=sizeof(name) || archive.Read(name,length)!=length)return nullptr;
    name[length]='\0';
    return FromName(name);
}

// Symbol: ?AfxClassInit@@YAXPEAUCRuntimeClass@@@Z
extern "C" void AFXAPI impl__AfxClassInit__YAXPEAUCRuntimeClass___Z(CRuntimeClass* type) {AfxClassInit(type);}
// Symbol: ?FromName@CRuntimeClass@@SAPEAU1@PEBD@Z
extern "C" CRuntimeClass* AFXAPI impl__FromName_CRuntimeClass__SAPEAU1_PEBD_Z(const char* name) {return CRuntimeClass::FromName(name);}
// Symbol: ?FromName@CRuntimeClass@@SAPEAU1@PEB_W@Z
extern "C" CRuntimeClass* AFXAPI impl__FromName_CRuntimeClass__SAPEAU1_PEB_W_Z(const wchar_t* name) {return CRuntimeClass::FromName(name);}
// Symbol: ?CreateObject@CRuntimeClass@@SAPEAVCObject@@PEBD@Z
extern "C" CObject* AFXAPI impl__CreateObject_CRuntimeClass__SAPEAVCObject__PEBD_Z(const char* name) {return CRuntimeClass::CreateObject(name);}
// Symbol: ?CreateObject@CRuntimeClass@@SAPEAVCObject@@PEB_W@Z
extern "C" CObject* AFXAPI impl__CreateObject_CRuntimeClass__SAPEAVCObject__PEB_W_Z(const wchar_t* name) {return CRuntimeClass::CreateObject(name);}
// Symbol: ?Load@CRuntimeClass@@SAPEAU1@AEAVCArchive@@PEAI@Z
extern "C" CRuntimeClass* AFXAPI impl__Load_CRuntimeClass__SAPEAU1_AEAVCArchive__PEAI_Z(CArchive* archive,UINT* schema) {
    return schema ? CRuntimeClass::Load(*archive,schema) : nullptr;
}
