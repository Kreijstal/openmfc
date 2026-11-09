// Native wide CSimpleStringT writable-buffer and lock operations.
#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxstr.h"
// Symbol: ?GetBuffer@?$CSimpleStringT@_W$00@ATL@@QEAAPEA_WH@Z
extern "C" wchar_t* impl__GetBuffer___CSimpleStringT__W_00_ATL__QEAAPEA_WH_Z(CString* self,int minimum) {return self->GetBuffer(minimum);}
// Symbol: ?GetBuffer@?$CSimpleStringT@_W$00@ATL@@QEAAPEA_WXZ
extern "C" wchar_t* impl__GetBuffer___CSimpleStringT__W_00_ATL__QEAAPEA_WXZ(CString* self) {return self->GetBuffer();}
// Symbol: ?GetBufferSetLength@?$CSimpleStringT@_W$00@ATL@@QEAAPEA_WH@Z
extern "C" wchar_t* impl__GetBufferSetLength___CSimpleStringT__W_00_ATL__QEAAPEA_WH_Z(CString* self,int length) {return self->GetBufferSetLength(length);}
// Symbol: ?ReleaseBuffer@?$CSimpleStringT@_W$00@ATL@@QEAAXH@Z
extern "C" void impl__ReleaseBuffer___CSimpleStringT__W_00_ATL__QEAAXH_Z(CString* self,int length) {self->ReleaseBuffer(length);}
// Symbol: ?ReleaseBufferSetLength@?$CSimpleStringT@_W$00@ATL@@QEAAXH@Z
extern "C" void impl__ReleaseBufferSetLength___CSimpleStringT__W_00_ATL__QEAAXH_Z(CString* self,int length) {self->ReleaseBufferSetLength(length);}
// Symbol: ?LockBuffer@?$CSimpleStringT@_W$00@ATL@@QEAAPEA_WXZ
extern "C" wchar_t* impl__LockBuffer___CSimpleStringT__W_00_ATL__QEAAPEA_WXZ(CString* self) {return self->LockBuffer();}
// Symbol: ?UnlockBuffer@?$CSimpleStringT@_W$00@ATL@@QEAAXXZ
extern "C" void impl__UnlockBuffer___CSimpleStringT__W_00_ATL__QEAAXXZ(CString* self) {self->UnlockBuffer();}
