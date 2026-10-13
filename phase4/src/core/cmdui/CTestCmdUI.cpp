#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxpriv.h"
#include <new>
#ifdef __GNUC__
#define MS_ABI __attribute__((ms_abi))
#else
#define MS_ABI
#endif
// Symbol: ??0CTestCmdUI@@QEAA@XZ
extern "C" void* MS_ABI impl___0CTestCmdUI__QEAA_XZ(void* storage) { return new(storage) CTestCmdUI; }
// Symbol: ?Enable@CTestCmdUI@@UEAAXH@Z
extern "C" void MS_ABI impl__Enable_CTestCmdUI__UEAAXH_Z(CTestCmdUI* self,int enabled) { self->CTestCmdUI::Enable(enabled); }
// Symbol: ?SetCheck@CTestCmdUI@@UEAAXH@Z
extern "C" void MS_ABI impl__SetCheck_CTestCmdUI__UEAAXH_Z(CTestCmdUI* self,int check) { self->CTestCmdUI::SetCheck(check); }
// Symbol: ?SetRadio@CTestCmdUI@@UEAAXH@Z
extern "C" void MS_ABI impl__SetRadio_CTestCmdUI__UEAAXH_Z(CTestCmdUI* self,int checked) { self->CTestCmdUI::SetRadio(checked); }
// Symbol: ?SetText@CTestCmdUI@@UEAAXPEB_W@Z
extern "C" void MS_ABI impl__SetText_CTestCmdUI__UEAAXPEB_W_Z(CTestCmdUI* self,const wchar_t* text) { self->CTestCmdUI::SetText(text); }
