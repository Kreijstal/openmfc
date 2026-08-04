// CMFCRibbonSpinButtonCtrl — OpenMFC implementation.
// Sources: global_ctor_quickwins.cpp

// Hand-written real implementations for header-defined classes that were one
// export short of complete. Constructors placement-new into the caller-provided
// storage so the openmfc C++ constructor establishes the validated member
// layout. Kept in a global_*.cpp file so the build's AUTO_EXCLUDES pass (scan of
// `// Symbol:` comments) removes the weak/typed stubs for these ordinals
// automatically.

#include "openmfc/afxwin.h"
#include "openmfc/afxinet.h"
#include "openmfc/afxdisp.h"
#include "openmfc/afxole.h"

#include <cstdio>
#include <new>
#include <cstdlib>
#include <cstring>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

// Runtime-class descriptors used by COleException::Process, defined alongside
// the exception RTTI tables (ole_oleexception_rtti.cpp,
// global_simple_exceptions_rtti.cpp).
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_COleException__SAPEAUCRuntimeClass__XZ();
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMemoryException__SAPEAUCRuntimeClass__XZ();
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CNotSupportedException__SAPEAUCRuntimeClass__XZ();
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CInvalidArgException__SAPEAUCRuntimeClass__XZ();

extern "C" void MS_ABI impl__CommonInit_CFile__IEAAXPEB_WIPEAVCAtlTransactionManager_ATL___Z(
    void* pThis, const wchar_t* lpszFileName, unsigned int nOpenFlags, void* pTM);
extern "C" void MS_ABI impl__CommonBaseInit_CStdioFile__IEAAXPEAU_iobuf__PEAVCAtlTransactionManager_ATL___Z(
    void* pThis, FILE* pStream, void* pTM);
extern "C" void MS_ABI impl__CommonInit_CStdioFile__IEAAXPEB_WIPEAVCAtlTransactionManager_ATL___Z(
    void* pThis, const wchar_t* lpszFileName, unsigned int nOpenFlags, void* pTM);















// CMFCRibbonSpinButtonCtrl::OnDeltapos(NMHDR*, LRESULT*) — UDN_DELTAPOS
// reflection handler. Retail (mfc140u, verified at the export's RVA) first
// tests its owning ribbon-edit pointer at this+0x108 and, when that is null,
// takes the early-out `mov qword ptr [pResult],0 / ret`. OpenMFC never
// populates that pointer (the ribbon edit itself is unimplemented), so the
// early-out is the path retail would always take here, and it is reproduced
// exactly rather than dereferencing state we do not model.
//
// Zeroing pResult matters: the generated stub left it untouched, so the common
// control read whatever the caller had on the stack and could reject the spin.
// Symbol: ?OnDeltapos@CMFCRibbonSpinButtonCtrl@@AEAAXPEAUtagNMHDR@@PEA_J@Z
extern "C" void MS_ABI impl__OnDeltapos_CMFCRibbonSpinButtonCtrl__AEAAXPEAUtagNMHDR__PEA_J_Z(
    void* /*pThis*/, void* /*pNMHDR*/, LRESULT* pResult)
{
    if (pResult) *pResult = 0;
}
