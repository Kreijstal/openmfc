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

namespace {

// CObject's default constructor is protected, so a trivial derived shim is
// needed to construct a bare CObject in place. It adds no members and no
// overrides, so sizeof/layout are identical to CObject.
struct CObjectShim : public CObject { CObjectShim() = default; };
static_assert(sizeof(CObjectShim) == sizeof(CObject), "shim must not change CObject layout");

// Allocate Size bytes (the real MSVC sizeof), zero-fill, then construct Base.
template <class Base, size_t Size>
CObject* CreateSized()
{
    static_assert(sizeof(Base) <= Size, "base class does not fit harvested size");
    void* p = ::operator new(Size, std::nothrow);
    if (!p) return nullptr;
    std::memset(p, 0, Size);
    return static_cast<CObject*>(new (p) Base());
}

} // namespace

// Symbol: ?CreateObject@CSettingsStore@@SAPEAVCObject@@XZ
extern "C" CObject* MS_ABI impl__CreateObject_CSettingsStore__SAPEAVCObject__XZ()
{ return CreateSized<CObjectShim, 56>(); }

// Symbol: ?CreateObject@CHelpComboBoxButton@@SAPEAVCObject@@XZ
extern "C" CObject* MS_ABI impl__CreateObject_CHelpComboBoxButton__SAPEAVCObject__XZ()
{ return CreateSized<CMFCToolBarComboBoxButton, 344>(); }

// Symbol: ?CreateObject@CMFCRibbonColorMenuButton@@SAPEAVCObject@@XZ
extern "C" CObject* MS_ABI impl__CreateObject_CMFCRibbonColorMenuButton__SAPEAVCObject__XZ()
{ return CreateSized<CMFCRibbonButton, 640>(); }

// Symbol: ?CreateObject@CMFCRibbonQuickAccessCustomizeButton@@SAPEAVCObject@@XZ
extern "C" CObject* MS_ABI impl__CreateObject_CMFCRibbonQuickAccessCustomizeButton__SAPEAVCObject__XZ()
{ return CreateSized<CMFCRibbonButton, 672>(); }

// Symbol: ?CreateObject@CMFCRibbonStatusBarCustomizeButton@@SAPEAVCObject@@XZ
extern "C" CObject* MS_ABI impl__CreateObject_CMFCRibbonStatusBarCustomizeButton__SAPEAVCObject__XZ()
{ return CreateSized<CMFCRibbonButton, 624>(); }

// Symbol: ?CreateObject@CRibbonUndoLabel@@SAPEAVCObject@@XZ
extern "C" CObject* MS_ABI impl__CreateObject_CRibbonUndoLabel__SAPEAVCObject__XZ()
{ return CreateSized<CMFCRibbonButton, 624>(); }

// Symbol: ?CreateObject@CTasksPaneHistoryButton@@SAPEAVCObject@@XZ
extern "C" CObject* MS_ABI impl__CreateObject_CTasksPaneHistoryButton__SAPEAVCObject__XZ()
{ return CreateSized<CMFCToolBarMenuButton, 360>(); }

// Symbol: ?CreateObject@CTasksPaneMenuButton@@SAPEAVCObject@@XZ
extern "C" CObject* MS_ABI impl__CreateObject_CTasksPaneMenuButton__SAPEAVCObject__XZ()
{ return CreateSized<CMFCToolBarMenuButton, 304>(); }

// Symbol: ?CreateObject@CTasksPaneNavigateButton@@SAPEAVCObject@@XZ
extern "C" CObject* MS_ABI impl__CreateObject_CTasksPaneNavigateButton__SAPEAVCObject__XZ()
{ return CreateSized<CMFCToolBarButton, 136>(); }
