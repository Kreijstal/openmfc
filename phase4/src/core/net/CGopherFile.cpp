// CGopherFile — OpenMFC implementation.
// Sources: global_ctor_quickwins.cpp, inetcore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/InetcoreSupport.h"

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















// CGopherFile::CGopherFile(HINTERNET, CGopherLocator&, CGopherConnection*) —
// protected ctor; the openmfc C++ ctor is defined in inetcore.cpp.
// Symbol: ??0CGopherFile@@IEAA@PEAXAEAVCGopherLocator@@PEAVCGopherConnection@@@Z
extern "C" void* MS_ABI impl___0CGopherFile__IEAA_PEAXAEAVCGopherLocator__PEAVCGopherConnection___Z(
    void* pThis, void* hFile, CGopherLocator* pLocator, CGopherConnection* pConnection)
{
    return new (pThis) CGopherFile(static_cast<HINTERNET>(hFile), *pLocator, pConnection);
}
// Symbol: ?GetRuntimeClass@CGopherFile@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CGopherFile__UEBAPEAUCRuntimeClass__XZ(const CGopherFile* pThis) {
    (void)pThis;
    return &g_classCGopherFile;
}
// Symbol: ?GetThisClass@CGopherFile@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CGopherFile__SAPEAUCRuntimeClass__XZ() {
    return &g_classCGopherFile;
}
// Symbol: ?Write@CGopherFile@@UEAAXPEBXI@Z
extern "C" void MS_ABI impl__Write_CGopherFile__UEAAXPEBXI_Z(CGopherFile* pThis, const void* lpBuf, unsigned int nCount) {
    if (pThis) pThis->CInternetFile::Write(lpBuf, nCount);
}
// Symbol: ?WriteString@CGopherFile@@UEAAXPEB_W@Z
extern "C" void MS_ABI impl__WriteString_CGopherFile__UEAAXPEB_W_Z(CGopherFile* pThis, const wchar_t* pstr) {
    if (pThis) pThis->CInternetFile::WriteString(pstr);
}
CGopherFile::CGopherFile(HINTERNET hFile, CGopherLocator& refLocator,
                         CGopherConnection* pConnection)
    : CInternetFile(hFile, LocatorToString(&refLocator), pConnection, 0) {
}
CGopherFile::~CGopherFile() {}

// === Moved from ManualThunks.cpp ===
// Symbol: ??0CGopherFile@@IEAA@PEAX0PEB_WK_K@Z
extern "C" void* MS_ABI impl___0CGopherFile__IEAA_PEAX0PEB_WK_K_Z(void* pThis, void* p0, void* p1, void* p2, void* p3, void* p4) {
    (void)pThis;
    (void)p0;
    (void)p1;
    (void)p2;
    (void)p3;
    (void)p4;
    return nullptr;
}

