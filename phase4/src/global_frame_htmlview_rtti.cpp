#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxmfc.h"

#include <cstring>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

// Symbol: ?GetThisClass@CHtmlView@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CHtmlView__SAPEAUCRuntimeClass__XZ() {
    return CHtmlView::GetThisClass();
}

// Symbol: ?GetRuntimeClass@CHtmlView@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CHtmlView__UEBAPEAUCRuntimeClass__XZ(
    const CHtmlView* pThis) {
    return CHtmlView::GetThisClass();
}

// Symbol: ?CreateObject@CHtmlView@@SAPEAVCObject@@XZ
extern "C" CObject* MS_ABI impl__CreateObject_CHtmlView__SAPEAVCObject__XZ() {
    return CHtmlView::CreateObject();
}

// Symbol: ?GetRuntimeClass@CMemoryException@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CMemoryException__UEBAPEAUCRuntimeClass__XZ(
    const CMemoryException* pThis) {
    return CMemoryException::GetThisClass();
}

// Symbol: ?GetThisClass@CMetaFileDC@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMetaFileDC__SAPEAUCRuntimeClass__XZ() {
    return CMetaFileDC::GetThisClass();
}

// Symbol: ?GetRuntimeClass@CMetaFileDC@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CMetaFileDC__UEBAPEAUCRuntimeClass__XZ(
    const CMetaFileDC* pThis) {
    return CMetaFileDC::GetThisClass();
}

// Symbol: ?GetThisClass@CMiniFrameWnd@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMiniFrameWnd__SAPEAUCRuntimeClass__XZ() {
    return CMiniFrameWnd::GetThisClass();
}

// Symbol: ?GetRuntimeClass@CMiniFrameWnd@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CMiniFrameWnd__UEBAPEAUCRuntimeClass__XZ(
    const CMiniFrameWnd* pThis) {
    return CMiniFrameWnd::GetThisClass();
}
