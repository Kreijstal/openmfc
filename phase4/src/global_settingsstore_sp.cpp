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
