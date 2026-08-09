// CPen — OpenMFC implementation.
// Sources: gdicore.cpp, global_ctor_quickwins.cpp, manual_small_stub_implementations.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/GdicoreSupport.h"
#include "detail/ManualSmallStubImplementationsSupport.h"
#include "detail/CRuntimeClassSupport.h"

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















// CPen constructor with parameters
// Symbol: ??0CPen@@QEAA@HHK@Z
extern "C" CPen* MS_ABI impl___0CPen__QEAA_HHK_Z(CPen* pThis, int nPenStyle, int nWidth, unsigned long crColor) {
    if (!pThis) return nullptr;
    pThis->m_hObject = ::CreatePen(nPenStyle, nWidth, crColor);
    return pThis;
}
// CPen::CPen(int nPenStyle, int nWidth, const LOGBRUSH*, int nStyleCount,
//            const DWORD* lpStyle) — the geometric/cosmetic ExtCreatePen form.
// Mirrors the existing CPen(int,int,COLORREF) thunk in gdicore.cpp, which stores
// the created handle straight into CGdiObject::m_hObject.
// Symbol: ??0CPen@@QEAA@HHPEBUtagLOGBRUSH@@HPEBK@Z
extern "C" CPen* MS_ABI impl___0CPen__QEAA_HHPEBUtagLOGBRUSH__HPEBK_Z(
    CPen* pThis, int nPenStyle, int nWidth, const LOGBRUSH* pLogBrush,
    int nStyleCount, const DWORD* lpStyle)
{
    if (!pThis) return nullptr;
    // Placement-new first: this is a constructor thunk, so the CPen/CGdiObject
    // subobjects and the vtable pointer must be established before any member
    // is written. Storing m_hObject into raw storage (as this did originally)
    // left the vptr uninitialized, so the first virtual call on the pen --
    // including ~CGdiObject -> DeleteObject -- dispatched through garbage.
    CPen* p = new (pThis) CPen();
    p->m_hObject = ::ExtCreatePen(static_cast<DWORD>(nPenStyle),
                                  static_cast<DWORD>(nWidth),
                                  pLogBrush,
                                  static_cast<DWORD>(nStyleCount),
                                  lpStyle);
    return p;
}
// Symbol: ?GetRuntimeClass@CPen@@UEBAPEAUCRuntimeClass@@XZ
extern "C" void* MS_ABI impl__GetRuntimeClass_CPen__UEBAPEAUCRuntimeClass__XZ(
    void* pThis) {
    (void)pThis;
    InitializeClasses();
    return &CPen::classCPen;
}
// Symbol: ?GetThisClass@CPen@@SAPEAUCRuntimeClass@@XZ
extern "C" void* MS_ABI impl__GetThisClass_CPen__SAPEAUCRuntimeClass__XZ() {
    InitializeClasses();
    return &CPen::classCPen;
}
