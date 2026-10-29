// CMFCRibbonCustomizeDialog — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp

#include "detail/ManualSmallStubImplementationsSupport.h"

namespace {
struct S_CMFCRibbonCustomizeDialog {
    unsigned char m_base[0x7230];
    void* m_pPage;
};
static_assert(offsetof(S_CMFCRibbonCustomizeDialog, m_pPage) == 0x7230,
              "CMFCRibbonCustomizeDialog::m_pPage offset from mfc140u RVA 0xf9840");
static_assert(sizeof(S_CMFCRibbonCustomizeDialog) == 29240,
              "CMFCRibbonCustomizeDialog size from RuntimeClasses.cpp");
}

// Symbol: ??0CMFCRibbonCustomizeDialog@@QEAA@PEAVCWnd@@PEAVCMFCRibbonBar@@@Z
extern "C" void* MS_ABI impl___0CMFCRibbonCustomizeDialog__QEAA_PEAVCWnd__PEAVCMFCRibbonBar___Z(
    void* pThis, void*, void*) {
    return pThis;
}
// Retail destructor entry RVA 0xf9840 (mfc140u) stores the derived vfptr,
// conditionally invokes the page's scalar-deleting-destructor slot at +0x7230,
// then tail-calls CMFCPropertySheet::~CMFCPropertySheet (entry RVA 0xce4a0,
// mfc140u). In OpenMFC, this dialog's pre-existing constructor only returns
// pThis, so neither the property-sheet base nor +0x7230 is initialized; the
// sibling page constructor is also a pThis-only stub and establishes no vtable.
// Reading/deleting the page or running the base destructor would therefore use
// uninitialized state. Keep this destructor stub until those constructors and
// the page's scalar-deleting-destructor path are implemented.
// Symbol: ??1CMFCRibbonCustomizeDialog@@UEAA@XZ
extern "C" void MS_ABI impl___1CMFCRibbonCustomizeDialog__UEAA_XZ(void* pThis) {
    (void)pThis;
}
