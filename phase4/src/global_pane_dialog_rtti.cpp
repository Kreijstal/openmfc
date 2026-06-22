#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxmfc.h"

#include <cstring>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

// Symbol: ?GetThisClass@CPrintDialogEx@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CPrintDialogEx__SAPEAUCRuntimeClass__XZ() {
    return CPrintDialogEx::GetThisClass();
}

// Symbol: ?GetRuntimeClass@CPrintDialogEx@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CPrintDialogEx__UEBAPEAUCRuntimeClass__XZ(
    const CPrintDialogEx* pThis) {
    return CPrintDialogEx::GetThisClass();
}
