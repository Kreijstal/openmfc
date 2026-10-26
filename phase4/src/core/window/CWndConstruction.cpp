// Native named-field initialization and qualified destructor entry points.
#include "detail/WindowHandleMapSupport.h"
struct OpenMfcCWndConstruction {
    static void* Construct(void* storage,HWND handle) {return ::new(storage) CWnd(handle);}
    static CWnd Special(HWND handle) {return CWnd(handle);}
};
// Symbol: ??0CWnd@@QEAA@XZ
extern "C" void* AFXAPI impl___0CWnd__QEAA_XZ(void* self) {return ::new(self) CWnd;}
// Symbol: ??0CWnd@@AEAA@PEAUHWND__@@@Z
extern "C" void* AFXAPI impl___0CWnd__AEAA_PEAUHWND_____Z(void* self,HWND handle) {return OpenMfcCWndConstruction::Construct(self,handle);}
// Symbol: ??1CWnd@@UEAA@XZ
extern "C" void AFXAPI impl___1CWnd__UEAA_XZ(CWnd* self) {self->CWnd::~CWnd();}
// These are full constructed windows, including their three interface tables.
// Their identities suppress DestroyWindow during static destruction.
// Symbol: ?wndBottom@CWnd@@2V1@B
extern "C" const CWnd impl__wndBottom_CWnd__2V1_B=OpenMfcCWndConstruction::Special(HWND_BOTTOM);
// Symbol: ?wndNoTopMost@CWnd@@2V1@B
extern "C" const CWnd impl__wndNoTopMost_CWnd__2V1_B=OpenMfcCWndConstruction::Special(HWND_NOTOPMOST);
// Symbol: ?wndTop@CWnd@@2V1@B
extern "C" const CWnd impl__wndTop_CWnd__2V1_B=OpenMfcCWndConstruction::Special(HWND_TOP);
// Symbol: ?wndTopMost@CWnd@@2V1@B
extern "C" const CWnd impl__wndTopMost_CWnd__2V1_B=OpenMfcCWndConstruction::Special(HWND_TOPMOST);
