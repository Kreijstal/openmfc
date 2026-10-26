#pragma once
#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxwin.h"
#include "detail/CHandleMapSupport.h"
#include "detail/ModuleThreadStateSupport.h"
namespace openmfc { namespace detail {
CHandleMap* GetWindowHandleMap(bool create);
} }
extern "C" CWnd* AFXAPI impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(HWND);
extern "C" CWnd* AFXAPI impl__FromHandlePermanent_CWnd__SAPEAV1_PEAUHWND_____Z(HWND);
extern "C" BOOL AFXAPI impl__Attach_CWnd__QEAAHPEAUHWND_____Z(CWnd*,HWND);
extern "C" HWND AFXAPI impl__Detach_CWnd__QEAAPEAUHWND____XZ(CWnd*);
extern "C" void AFXAPI impl__DeleteTempMap_CWnd__SAXXZ();
extern "C" void AFXAPI impl__AttachControlSite_CWnd__IEAAXPEAVCHandleMap___Z(CWnd*,CHandleMap*);
extern "C" const CWnd impl__wndBottom_CWnd__2V1_B;
extern "C" const CWnd impl__wndNoTopMost_CWnd__2V1_B;
extern "C" const CWnd impl__wndTop_CWnd__2V1_B;
extern "C" const CWnd impl__wndTopMost_CWnd__2V1_B;
