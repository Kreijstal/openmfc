// CBaseKeyFrame — OpenMFC implementation.
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
#include <cstddef>

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















// CBaseKeyFrame is not declared in include/openmfc, so pin the retail layout
// locally. The MSVC retail declaration has these fields after CObject's vfptr
// (afxanimationcontroller.h:298-326); mfc140u's sentinel at RVA 0x3b1ba8
// stores the vftable, storyboard-start keyframe, and two TRUE BOOLs.
namespace {
struct S_CBaseKeyFrameLayout {
    void* vfptr;
    unsigned long long m_keyframe;
    int m_bAdded;
    int m_bIsKeyframeAtOffset;
};
static_assert(offsetof(S_CBaseKeyFrameLayout, m_keyframe) == 0x08, "CBaseKeyFrame::m_keyframe");
static_assert(offsetof(S_CBaseKeyFrameLayout, m_bAdded) == 0x10, "CBaseKeyFrame::m_bAdded");
static_assert(offsetof(S_CBaseKeyFrameLayout, m_bIsKeyframeAtOffset) == 0x14, "CBaseKeyFrame::m_bIsKeyframeAtOffset");
static_assert(sizeof(S_CBaseKeyFrameLayout) == 0x18, "sizeof(CBaseKeyFrame) == 24");
}

// CBaseKeyFrame::AddToStoryboard(IUIAnimationStoryboard*, BOOL). The exact
// export name has no resolved RVA. Retail mfc140u's vftable (RVA 0x2dae80)
// has slot 5 at RVA 0x2daea8 pointing to code RVA 0x3a60, which disassembles
// as `mov $1,%eax; ret`: it ignores this and both arguments and returns TRUE.
// Symbol: ?AddToStoryboard@CBaseKeyFrame@@UEAAHPEAUIUIAnimationStoryboard@@H@Z
extern "C" int MS_ABI impl__AddToStoryboard_CBaseKeyFrame__UEAAHPEAUIUIAnimationStoryboard__H_Z(
    void* /*pThis*/, void* /*pStoryboard*/, int /*bDeepAdd*/)
{
    return TRUE;
}
