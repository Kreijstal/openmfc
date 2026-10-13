#pragma once
#include "openmfc/afxctrlcontainer.h"
#include "detail/WindowMessageSupport.h"
#include <cstring>
#include <new>
#ifdef __GNUC__
#define MS_ABI __attribute__((ms_abi))
#else
#define MS_ABI
#endif
extern "C" {
void MS_ABI impl__AfxThrowMemoryException__YAXXZ();
void MS_ABI impl__AfxThrowNotSupportedException__YAXXZ();
int MS_ABI impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(const CObject*,const CRuntimeClass*);
CRuntimeClass* MS_ABI impl__GetThisClass_CMFCShellListCtrl__SAPEAUCRuntimeClass__XZ();
int MS_ABI impl__SubclassWindow_CWnd__QEAAHPEAUHWND_____Z(CWnd*,HWND);
HWND MS_ABI impl__UnsubclassWindow_CWnd__QEAAPEAUHWND____XZ(CWnd*);
CWnd* MS_ABI impl__CreateDlgControl_CMFCControlContainer__IEAAPEAVCWnd__PEB_W_Z(void*,const wchar_t*);
}
