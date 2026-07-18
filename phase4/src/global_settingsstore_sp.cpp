// CSettingsStoreSP static runtime-class storage.

#include "openmfc/afxwin.h"

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CSettingsStore__SAPEAUCRuntimeClass__XZ();

// Symbol: ?m_pRTIDefault@CSettingsStoreSP@@1PEAUCRuntimeClass@@EA
extern "C" CRuntimeClass* impl__m_pRTIDefault_CSettingsStoreSP__1PEAUCRuntimeClass__EA =
    impl__GetThisClass_CSettingsStore__SAPEAUCRuntimeClass__XZ();

// Symbol: ?SetRuntimeClass@CSettingsStoreSP@@SAHPEAUCRuntimeClass@@@Z
extern "C" int MS_ABI impl__SetRuntimeClass_CSettingsStoreSP__SAHPEAUCRuntimeClass___Z(
    CRuntimeClass* pRTI) {
    CRuntimeClass* pDefault = impl__GetThisClass_CSettingsStore__SAPEAUCRuntimeClass__XZ();
    if (pRTI != nullptr && !pRTI->IsDerivedFrom(pDefault))
        return 0;

    impl__m_pRTIDefault_CSettingsStoreSP__1PEAUCRuntimeClass__EA =
        pRTI != nullptr ? pRTI : pDefault;
    return 1;
}

extern "C" CObject* MS_ABI impl__CreateObject_CSettingsStore__SAPEAVCObject__XZ();

// CSettingsStoreSP::Create(BOOL bAdmin, BOOL bReadOnly) — lazily builds the
// owned CSettingsStore and returns a reference to it.
//
// Offsets below are read off the retail mfc140 implementation (RVA 0x12b320):
//   CSettingsStoreSP: m_pRegistry@0x00, m_dwUserData@0x08
//   CSettingsStore  : hive key@0x08, m_bReadOnly@0x28, m_bAdmin@0x2C,
//                     m_dwUserData@0x30   (object size 0x38 = 56)
// Retail derives the hive with a sbb/neg sequence that yields
// HKEY_CURRENT_USER for bAdmin==0 and HKEY_LOCAL_MACHINE otherwise.
//
// Retail validates the freshly created object with IsKindOf; we instead check
// IsDerivedFrom on the runtime class *before* creating. The two are equivalent
// for a correctly registered CSettingsStore subclass, but IsKindOf would reject
// the object our CreateObject hands back (it reports its base class, see
// global_dyncreate_factories.cpp).
// Symbol: ?Create@CSettingsStoreSP@@QEAAAEAVCSettingsStore@@HH@Z
extern "C" void* MS_ABI impl__Create_CSettingsStoreSP__QEAAAEAVCSettingsStore__HH_Z(
    void* pThis, int bAdmin, int bReadOnly) {
    if (pThis == nullptr) return nullptr;

    unsigned char* self = static_cast<unsigned char*>(pThis);
    CObject** ppRegistry = reinterpret_cast<CObject**>(self);

    if (*ppRegistry == nullptr) {
        CRuntimeClass* pDefault = impl__GetThisClass_CSettingsStore__SAPEAUCRuntimeClass__XZ();
        CRuntimeClass* pRTI = impl__m_pRTIDefault_CSettingsStoreSP__1PEAUCRuntimeClass__EA;

        CObject* pNew = nullptr;
        if (pRTI != nullptr && pRTI != pDefault && pRTI->IsDerivedFrom(pDefault))
            pNew = pRTI->CreateObject();
        if (pNew == nullptr)
            pNew = impl__CreateObject_CSettingsStore__SAPEAVCObject__XZ();

        *ppRegistry = pNew;

        if (pNew != nullptr) {
            unsigned char* r = reinterpret_cast<unsigned char*>(pNew);
            *reinterpret_cast<int*>(r + 0x28) = bReadOnly;
            *reinterpret_cast<int*>(r + 0x2C) = bAdmin;
            *reinterpret_cast<void**>(r + 0x08) =
                bAdmin ? static_cast<void*>(HKEY_LOCAL_MACHINE) : static_cast<void*>(HKEY_CURRENT_USER);
            *reinterpret_cast<DWORD_PTR*>(r + 0x30) = *reinterpret_cast<DWORD_PTR*>(self + 8);
        }
    }
    return *ppRegistry;
}
