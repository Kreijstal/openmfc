#pragma once
#include "openmfc/afxpriv.h"
#ifdef __GNUC__
#define MS_ABI __attribute__((ms_abi))
#else
#define MS_ABI
#endif
extern "C" void* MS_ABI impl__GetRoutingFrame_CCmdTarget__IEAAPEAVCFrameWnd__XZ(CCmdTarget*);
extern "C" void* MS_ABI impl__GetRoutingFrame__CCmdTarget__KAPEAVCFrameWnd__XZ();
extern "C" void* MS_ABI impl__GetRoutingView_CCmdTarget__IEAAPEAVCView__XZ(CCmdTarget*);
extern "C" void* MS_ABI impl__GetRoutingView__CCmdTarget__KAPEAVCView__XZ();
extern "C" int MS_ABI impl__OnCmdMsg_CFrameWnd__UEAAHIHPEAXPEAUAFX_CMDHANDLERINFO___Z(CFrameWnd*,UINT,int,void*,AFX_CMDHANDLERINFO*);
extern "C" int MS_ABI impl__OnCmdMsg_CView__UEAAHIHPEAXPEAUAFX_CMDHANDLERINFO___Z(CView*,UINT,int,void*,AFX_CMDHANDLERINFO*);
