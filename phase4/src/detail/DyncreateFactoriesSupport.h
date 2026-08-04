#pragma once
// Shared internals of the former global_dyncreate_factories.cpp translation unit.
// Definitions live in detail/DyncreateFactoriesSupport.cpp, so file-scope state keeps a
// single instance across the classes this unit was split into.
namespace openmfc { namespace detail { namespace dyncreatefactories {} } }
using namespace openmfc::detail::dyncreatefactories;
// DECLARE_DYNCREATE factories (CreateObject) for classes that exist in mfc140u
// only as RTTI shells here: we have their harvested CRuntimeClass descriptor
// (real m_nObjectSize + real base) but no header definition, so `new CDerived`
// is not expressible.
//
// Strategy: allocate the *harvested* object size, zero it, and construct the
// nearest header-defined base in place. That yields an object which is
//   - correctly sized (safe to delete / store / memcpy as the real type),
//   - non-null, so CRuntimeClass::CreateObject()-driven paths proceed, and
//   - equipped with a working CObject-derived vtable.
// It is an approximation in one respect: GetRuntimeClass() reports the base,
// because no derived vtable exists to install. This is strictly better than the
// previous behaviour (the generated stub returned nullptr, which null-derefs at
// the first use), and every derived method in this library is itself still a
// stub, so no derived behaviour is lost.
//
// Sizes/bases below are copied from the harvested descriptors in
// global_oleresidual_rtti.cpp, global_ribbon_elements_rtti.cpp and
// global_toolbar_buttons_rtti.cpp -- keep them in sync with those tables.

#include "openmfc/afxwin.h"
#include "openmfc/afxmfc.h"

#include <new>
#include <cstring>
#include <cstddef>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif


// CObject's default constructor is protected, so a trivial derived shim is
// needed to construct a bare CObject in place. It adds no members and no
// overrides, so sizeof/layout are identical to CObject.
namespace openmfc { namespace detail { namespace dyncreatefactories {
struct CObjectShim : public CObject { CObjectShim() = default; };
} } }

// Allocate Size bytes (the real MSVC sizeof), zero-fill, then construct Base.
namespace openmfc { namespace detail { namespace dyncreatefactories {
template <class Base, size_t Size>
CObject* CreateSized()
{
    static_assert(sizeof(Base) <= Size, "base class does not fit harvested size");
    void* p = ::operator new(Size, std::nothrow);
    if (!p) return nullptr;
    std::memset(p, 0, Size);
    return static_cast<CObject*>(new (p) Base());
}
} } }


// CSettingsStore::CreateObject()
//
// Unlike the other factories here, this one must produce an object that can
// prove it is a CSettingsStore: CSettingsStoreSP::Create validates the result
// with IsKindOf(RUNTIME_CLASS(CSettingsStore)). Handing back a CObjectShim
// would misreport the type to that check and to every downstream
// DYNAMIC_DOWNCAST, so the shim is not usable here.
//
// A real CSettingsStore runtime descriptor already exists (it backs the
// exported CSettingsStore::GetThisClass), so the object can report its type
// truthfully rather than either lying or refusing to be created. The class
// below is a genuine CObject subclass of the harvested size whose
// GetRuntimeClass returns that descriptor; IsKindOf then passes on the merits.
// Its CSettingsStore methods remain unimplemented -- the same position as any
// partially-implemented class -- but its identity is not a fiction.
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CSettingsStore__SAPEAUCRuntimeClass__XZ();

namespace openmfc { namespace detail { namespace dyncreatefactories {
struct CSettingsStoreObject : public CObject {
    CSettingsStoreObject() = default;
    virtual CRuntimeClass* GetRuntimeClass() const override
    { return impl__GetThisClass_CSettingsStore__SAPEAUCRuntimeClass__XZ(); }
};
} } }









