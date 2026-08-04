// RuntimeClasses — OpenMFC implementation.
// Sources: global_csimpleexception_rtti.cpp, global_frame_htmlview_rtti.cpp, global_simple_exceptions_rtti.cpp, ole_oleexception_rtti.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/CsimpleexceptionRttiSupport.h"
#include "detail/SimpleExceptionsRttiSupport.h"
#include "detail/OleOleexceptionRttiSupport.h"

#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxmfc.h"

#include <cstring>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif








// ---------------------------------------------------------------------------
// ?GetErrorMessage@CSimpleException@@UEBAHPEA_WIPEAI@Z
//   virtual int GetErrorMessage(wchar_t* lpszError, UINT nMaxError,
//                               UINT* pnHelpContext) const;
// Copies the cached message into lpszError, truncating to nMaxError-1 wide
// chars and NUL-terminating. Sets *pnHelpContext = m_nHelpContext. Returns 1
// if an initialized, non-empty message was produced, else 0.
// ---------------------------------------------------------------------------
// Symbol: ?GetErrorMessage@CSimpleException@@UEBAHPEA_WIPEAI@Z
extern "C" int MS_ABI impl__GetErrorMessage_CSimpleException__UEBAHPEA_WIPEAI_Z(
    const CSimpleException* pThis, wchar_t* lpszError, UINT nMaxError, UINT* pnHelpContext
) {
    if (pnHelpContext) {
        *pnHelpContext = pThis ? pThis->m_nHelpContext : 0;
    }
    if (!lpszError || nMaxError == 0) {
        return 0;
    }
    lpszError[0] = L'\0';
    if (!pThis || !pThis->m_bInitialized || pThis->m_szMessage[0] == L'\0') {
        return 0;
    }
    // Copy at most nMaxError-1 chars, always NUL-terminate.
    UINT i = 0;
    const UINT limit = nMaxError - 1;
    while (i < limit && pThis->m_szMessage[i] != L'\0') {
        lpszError[i] = pThis->m_szMessage[i];
        ++i;
    }
    lpszError[i] = L'\0';
    return 1;
}
// ---------------------------------------------------------------------------
// ?GetRuntimeClass@CSimpleException@@UEBAPEAUCRuntimeClass@@XZ
//   virtual CRuntimeClass* GetRuntimeClass() const;
// ---------------------------------------------------------------------------
// Symbol: ?GetRuntimeClass@CSimpleException@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CSimpleException__UEBAPEAUCRuntimeClass__XZ(
    const CSimpleException* /*pThis*/
) {
    return &classCSimpleException;
}
// ---------------------------------------------------------------------------
// ?GetThisClass@CSimpleException@@SAPEAUCRuntimeClass@@XZ
//   static CRuntimeClass* GetThisClass();
// ---------------------------------------------------------------------------
// Symbol: ?GetThisClass@CSimpleException@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CSimpleException__SAPEAUCRuntimeClass__XZ() {
    return &classCSimpleException;
}
// ---------------------------------------------------------------------------
// ?InitString@CSimpleException@@QEAAXXZ
//   void InitString();
// Marks the cached message as initialized (the message text itself is loaded
// from a resource by derived classes; once that has happened InitString flips
// m_bInitialized so GetErrorMessage will surface the cached text).
// ---------------------------------------------------------------------------
// Symbol: ?InitString@CSimpleException@@QEAAXXZ
extern "C" void MS_ABI impl__InitString_CSimpleException__QEAAXXZ(CSimpleException* pThis) {
    if (!pThis) return;
    pThis->m_bInitialized = TRUE;
}
// Implemented in manual_small_stub_implementations.cpp; this unit had a second, unlinked copy.
// Symbol: ?GetRuntimeClass@CMemoryException@@UEBAPEAUCRuntimeClass@@XZ
// Implemented in manual_small_stub_implementations.cpp; this unit had a second, unlinked copy.
// Symbol: ?GetThisClass@CInvalidArgException@@SAPEAUCRuntimeClass@@XZ
// Implemented in manual_small_stub_implementations.cpp; this unit had a second, unlinked copy.
// Symbol: ?GetRuntimeClass@CInvalidArgException@@UEBAPEAUCRuntimeClass@@XZ
// Implemented in manual_small_stub_implementations.cpp; this unit had a second, unlinked copy.
// Symbol: ?GetThisClass@CNotSupportedException@@SAPEAUCRuntimeClass@@XZ
// Implemented in manual_small_stub_implementations.cpp; this unit had a second, unlinked copy.
// Symbol: ?GetRuntimeClass@CNotSupportedException@@UEBAPEAUCRuntimeClass@@XZ
// ---- CResourceException_SimpleExceptionsRtti ------------------------------------------------------
// Symbol: ?GetThisClass@CResourceException@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI
impl__GetThisClass_CResourceException__SAPEAUCRuntimeClass__XZ() {
    return &class_CResourceException;
}
// Symbol: ?GetRuntimeClass@CResourceException@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI
impl__GetRuntimeClass_CResourceException__UEBAPEAUCRuntimeClass__XZ(const void* /*pThis*/) {
    return &class_CResourceException;
}
// Implemented in cbarcore.cpp; this unit had a second, unlinked copy.
// Symbol: ?GetThisClass@CUserException@@SAPEAUCRuntimeClass@@XZ
// Implemented in cbarcore.cpp; this unit had a second, unlinked copy.
// Symbol: ?GetRuntimeClass@CUserException@@UEBAPEAUCRuntimeClass@@XZ
// Symbol: ?GetThisClass@COleException@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_COleException__SAPEAUCRuntimeClass__XZ() {
    return &classCOleException;
}
// Symbol: ?GetRuntimeClass@COleException@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_COleException__UEBAPEAUCRuntimeClass__XZ(
    const void* /*pThis*/) {
    return &classCOleException;
}
// Symbol: ?GetThisClass@COleDispatchException@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_COleDispatchException__SAPEAUCRuntimeClass__XZ() {
    return &classCOleDispatchException;
}
// Symbol: ?GetRuntimeClass@COleDispatchException@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_COleDispatchException__UEBAPEAUCRuntimeClass__XZ(
    const void* /*pThis*/) {
    return &classCOleDispatchException;
}
