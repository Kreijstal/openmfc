#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxdao.h"
#include <new>
#ifdef __GNUC__
#define MS_ABI __attribute__((ms_abi))
#else
#define MS_ABI
#endif
IMPLEMENT_DYNAMIC(CLongBinary,CObject)
// Symbol: ??0CLongBinary@@QEAA@XZ
extern "C" CLongBinary* MS_ABI impl___0CLongBinary__QEAA_XZ(void* storage) {return new(storage) CLongBinary;}
// Symbol: ??1CLongBinary@@UEAA@XZ
extern "C" void MS_ABI impl___1CLongBinary__UEAA_XZ(CLongBinary* binary) {binary->CLongBinary::~CLongBinary();}
// Symbol: ?GetRuntimeClass@CLongBinary@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CLongBinary__UEBAPEAUCRuntimeClass__XZ(const CLongBinary*) {return CLongBinary::GetThisClass();}
// Symbol: ?GetThisClass@CLongBinary@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CLongBinary__SAPEAUCRuntimeClass__XZ() {return CLongBinary::GetThisClass();}
