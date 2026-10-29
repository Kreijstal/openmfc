// CInvalidArgException — OpenMFC implementation.
// Sources: retail disassembly and CInvalidArgException/CSimpleException layout declarations.

#include "detail/ManualSmallStubImplementationsSupport.h"

extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CSimpleException__SAPEAUCRuntimeClass__XZ();

namespace {
// Retail afx.h (14.51.36231, lines 913-923) declares CInvalidArgException as a
// fieldless CSimpleException-derived class. OpenMFC pins CSimpleException to 288
// bytes in afxwin.h; this shadow keeps that inherited layout explicit here.
struct CInvalidArgExceptionLayout : CSimpleException {};
static_assert(sizeof(CInvalidArgExceptionLayout) == 288,
              "CInvalidArgException inherits the 288-byte CSimpleException layout");
static_assert(offsetof(CRuntimeClass, m_pfnGetBaseClass) == 0x18,
              "CRuntimeClass base-class thunk is at the retail offset");
static_assert(sizeof(CRuntimeClass) == 0x30,
              "CInvalidArgException runtime descriptor uses the retail CRuntimeClass layout");

CRuntimeClass* AFXAPI GetCInvalidArgExceptionBaseClass() {
    return impl__GetThisClass_CSimpleException__SAPEAUCRuntimeClass__XZ();
}

CRuntimeClass g_cInvalidArgExceptionRuntimeClass = {
    "CInvalidArgException",
    static_cast<int>(sizeof(CInvalidArgExceptionLayout)),
    0xFFFF,
    nullptr,
    &GetCInvalidArgExceptionBaseClass,
    nullptr,
    nullptr
};
} // namespace

// Retail returns its class descriptor directly (RVA 0x2275e0, mfc140u).
// pThis is unused by the verified body; the local descriptor models the retail
// fieldless class using its CSimpleException base and the pinned shadow size above.
// Symbol: ?GetRuntimeClass@CInvalidArgException@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CInvalidArgException__UEBAPEAUCRuntimeClass__XZ(
    void* pThis) {
    return &g_cInvalidArgExceptionRuntimeClass;
}
// The Unicode RVA map has no entry for this export. The ANSI twin's entry body
// returns its class descriptor directly (RVA 0x225a40, mfc140.dll); this uses the
// same local descriptor as GetRuntimeClass.
// Symbol: ?GetThisClass@CInvalidArgException@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CInvalidArgException__SAPEAUCRuntimeClass__XZ() {
    return &g_cInvalidArgExceptionRuntimeClass;
}
