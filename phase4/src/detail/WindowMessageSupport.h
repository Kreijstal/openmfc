#pragma once
#include "openmfc/afxstat_.h"
#ifndef MS_ABI
#ifdef __GNUC__
#define MS_ABI __attribute__((ms_abi))
#else
#define MS_ABI
#endif
#endif
extern "C" LRESULT MS_ABI impl__AfxCallWndProc__YA_JPEAVCWnd__PEAUHWND____I_K_J_Z(CWnd*,HWND,UINT,WPARAM,LPARAM);
extern "C" const MSG* MS_ABI impl__GetCurrentMessage_CWnd__KAPEBUtagMSG__XZ();
extern "C" int MS_ABI impl__OnQueryEndSession_CDialog__IEAAHXZ(CDialog*);
extern "C" void MS_ABI impl__OnEndSession_CDialog__IEAAXH_Z(CDialog*,BOOL);
extern "C" long MS_ABI impl__HandleMessage_CPrintDialogEx__UEAAJPEAUHWND____I_K_JPEA_J_Z(void*,HWND,UINT,WPARAM,LPARAM,LRESULT*);
namespace openmfc { namespace detail {
BOOL DispatchWindowMessageMap(CWnd*,UINT,WPARAM,LPARAM,LRESULT*);
void SetSentMessage(MSG&,HWND,UINT,WPARAM,LPARAM,bool refresh);
void PostInitializeWindow(CWnd*,const RECT&,DWORD);
template<class Fn> Fn WindowSlot(void* object,size_t index) {
    return reinterpret_cast<Fn>((*static_cast<void***>(object))[index]);
}
}}
