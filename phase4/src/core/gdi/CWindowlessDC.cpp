// CWindowlessDC — OpenMFC implementation.
// Sources: cbarcore.cpp, global_ctor_quickwins.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/CbarcoreSupport.h"

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















// Symbol: ?Detach@CWindowlessDC@@QEAAPEAUHDC__@@XZ
extern "C" HDC MS_ABI impl__Detach_CWindowlessDC__QEAAPEAUHDC____XZ(void* pThis) {
    std::lock_guard<std::mutex> lock(g_userToolMutex);
    HDC hdc = nullptr;
    auto it = g_windowlessDCs.find(pThis);
    if (it != g_windowlessDCs.end()) {
        hdc = it->second;
        g_windowlessDCs.erase(it);
    }
    return hdc;
}
// Symbol: ?GetRuntimeClass@CWindowlessDC@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CWindowlessDC__UEBAPEAUCRuntimeClass__XZ(const void*) {
    return &g_classCWindowlessDC;
}
// Symbol: ?GetThisClass@CWindowlessDC@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CWindowlessDC__SAPEAUCRuntimeClass__XZ() {
    return &g_classCWindowlessDC;
}
// CWindowlessDC::CWindowlessDC(HDC, CPoint&) — the DC wrapper windowless OLE
// controls use to draw into a host-supplied device context. The harvested
// descriptor (global_taskdialog_dc_rtti.cpp) gives sizeof 40 over base CDC
// (sizeof 32), so the CDC subobject is constructed in place and bound to the
// caller's HDC. The viewport offset carried by the CPoint is deliberately NOT
// applied: mutating the caller's DC origin here would be a destructive guess,
// and binding the handle is the state clients actually observe (previously
// m_hDC was left unset, so any use of the object faulted).
// Symbol: ??0CWindowlessDC@@QEAA@PEAUHDC__@@AEAVCPoint@@@Z
extern "C" void* MS_ABI impl___0CWindowlessDC__QEAA_PEAUHDC____AEAVCPoint___Z(
    void* pThis, HDC hDC, CPoint* /*pPoint*/)
{
    static_assert(sizeof(CDC) <= 40, "CDC must fit the harvested CWindowlessDC size");
    CDC* p = new (pThis) CDC();
    p->m_hDC = hDC;
    p->m_hAttribDC = hDC;
    return p;
}
