// CNotSupportedException — OpenMFC implementation.
// Sources: retail RTTI exports and the retail/OpenMFC CSimpleException layouts.

#include "detail/ManualSmallStubImplementationsSupport.h"

extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CSimpleException__SAPEAUCRuntimeClass__XZ();

namespace {
// Retail afx.h (14.51.36231, lines 901-912) declares CNotSupportedException as a
// fieldless CSimpleException-derived class. OpenMFC pins CSimpleException to 288
// bytes in afxwin.h; this shadow keeps the inherited layout explicit here.
struct CNotSupportedExceptionLayout : CSimpleException {};
static_assert(sizeof(CNotSupportedExceptionLayout) == 288,
              "CNotSupportedException inherits the 288-byte CSimpleException layout");
static_assert(offsetof(CRuntimeClass, m_pfnGetBaseClass) == 0x18,
              "CRuntimeClass base-class thunk is at the retail offset");
static_assert(sizeof(CRuntimeClass) == 0x30,
              "CNotSupportedException runtime descriptor uses the retail CRuntimeClass layout");

CRuntimeClass* AFXAPI GetCNotSupportedExceptionBaseClass() {
    return impl__GetThisClass_CSimpleException__SAPEAUCRuntimeClass__XZ();
}

CRuntimeClass g_cNotSupportedExceptionRuntimeClass = {
    "CNotSupportedException",
    static_cast<int>(sizeof(CNotSupportedExceptionLayout)),
    0xFFFF,
    nullptr,
    &GetCNotSupportedExceptionBaseClass,
    nullptr,
    nullptr
};
} // namespace

// Retail returns its class descriptor directly (RVA 0x2275d0, mfc140u). The public
// OpenMFC headers do not declare CNotSupportedException, so return this file's shadow descriptor.
// Symbol: ?GetRuntimeClass@CNotSupportedException@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CNotSupportedException__UEBAPEAUCRuntimeClass__XZ(
    void* pThis) {
    (void)pThis;
    return &g_cNotSupportedExceptionRuntimeClass;
}
// The Unicode map has no entry for this export; its ANSI twin returns the descriptor directly
// (RVA 0x225a30, mfc140.dll). Return the equivalent local descriptor here.
// Symbol: ?GetThisClass@CNotSupportedException@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CNotSupportedException__SAPEAUCRuntimeClass__XZ() {
    return &g_cNotSupportedExceptionRuntimeClass;
}
