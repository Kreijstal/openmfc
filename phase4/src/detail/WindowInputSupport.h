#pragma once
#include "detail/WindowMessageSupport.h"
#include <tpcshrd.h>
extern "C" BOOL MS_ABI impl__OnTouchInputs_CWnd__MEAAHIPEAUtagTOUCHINPUT___Z(CWnd*,UINT,TOUCHINPUT*);
extern "C" LRESULT MS_ABI impl__OnTouchMessage_CWnd__IEAA_J_K_J_Z(CWnd*,WPARAM,LPARAM);
extern "C" LRESULT MS_ABI impl__OnGesture_CWnd__IEAA_J_K_J_Z(CWnd*,WPARAM,LPARAM);
extern "C" LRESULT MS_ABI impl__OnTabletQuerySystemGestureStatus_CWnd__IEAA_J_K_J_Z(CWnd*,WPARAM,LPARAM);
