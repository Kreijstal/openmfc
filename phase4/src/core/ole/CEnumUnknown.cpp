// CEnumUnknown — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp

#include "detail/ManualSmallStubImplementationsSupport.h"
#include "detail/InterfaceMapsSupport.h"

extern "C" int MS_ABI impl__OnNext_CEnumArray__MEAAHPEAX_Z(void* pThis, void* pInterface);
extern "C" void MS_ABI impl___1CEnumArray__UEAA_XZ(void* pThis);

namespace {
// Retail CEnumUnknown::GetInterfaceMap (mfc140u RVA 0x236b80) returns the
// map at RVA 0x32ac00. Its entry array (RVA 0x34d788) contains
// IID_IEnumUnknown at offset 0x68 followed by the {nullptr, SIZE_MAX} marker.
// The map's base callback resolves to the CCmdTarget interface-map accessor
// (mfc140u RVA 0x1de870).
const AFX_INTERFACEMAP* CEnumUnknownInterfaceMap() {
    static const AFX_INTERFACEMAP_ENTRY entries[] = {
        {&IID_IEnumUnknown, 0x68},
        {nullptr, SIZE_MAX},
    };
    static const AFX_INTERFACEMAP map{
        impl__GetThisInterfaceMap_CCmdTarget__KAPEBUAFX_INTERFACEMAP__XZ,
        entries,
    };
    return &map;
}
} // namespace

// Retail CEnumUnknown::OnNext at RVA 0x2351d0 (mfc140.dll, ANSI twin; the
// function body is byte-identical in mfc140u) delegates to CEnumArray::OnNext,
// then AddRefs the returned IUnknown only when an element was copied.
// Symbol: ?OnNext@CEnumUnknown@@MEAAHPEAX@Z
extern "C" int MS_ABI impl__OnNext_CEnumUnknown__MEAAHPEAX_Z(void* pThis, void* pInterface) {
    if (!impl__OnNext_CEnumArray__MEAAHPEAX_Z(pThis, pInterface)) {
        return FALSE;
    }
    (*static_cast<IUnknown**>(pInterface))->AddRef();
    return TRUE;
}

// Retail CEnumUnknown::~CEnumUnknown (mfc140u RVA 0x236b90) first installs its
// vftable, then releases elements at +0x50 when the clone owner at +0x48 is null
// (count at +0x5c), and tail-jumps to CEnumArray::~CEnumArray (mfc140u RVA
// 0x25b720). This TU has no CEnumUnknown-specific vftable to install, so that
// store is not reproduced; the base thunk installs the CEnumArray vftable. Keep
// OpenMFC's defensive null-this guard; retail dereferences this immediately.
// Symbol: ??1CEnumUnknown@@UEAA@XZ
extern "C" void MS_ABI impl___1CEnumUnknown__UEAA_XZ(void* pThis) {
    if (pThis == nullptr) {
        return;
    }

    auto* bytes = static_cast<unsigned char*>(pThis);
    if (*reinterpret_cast<void**>(bytes + 0x48) == nullptr) {
        auto** elements = *reinterpret_cast<IUnknown***>(bytes + 0x50);
        const unsigned int count = *reinterpret_cast<unsigned int*>(bytes + 0x5c);
        for (unsigned int i = 0; i < count; ++i) {
            elements[i]->Release();
        }
    }
    impl___1CEnumArray__UEAA_XZ(pThis);
}
// Symbol: ?GetInterfaceMap@CEnumUnknown@@MEBAPEBUAFX_INTERFACEMAP@@XZ
extern "C" const AFX_INTERFACEMAP* MS_ABI impl__GetInterfaceMap_CEnumUnknown__MEBAPEBUAFX_INTERFACEMAP__XZ(
    const void* pThis) {
    (void)pThis;
    return CEnumUnknownInterfaceMap();
}
// Symbol: ?GetThisInterfaceMap@CEnumUnknown@@KAPEBUAFX_INTERFACEMAP@@XZ
// The mfc140u export table maps ordinal 7202 to the same entry as the virtual
// getter, RVA 0x236b80 (mfc140u); the name-to-RVA map records only one alias.
extern "C" const AFX_INTERFACEMAP* MS_ABI impl__GetThisInterfaceMap_CEnumUnknown__KAPEBUAFX_INTERFACEMAP__XZ() {
    return CEnumUnknownInterfaceMap();
}
