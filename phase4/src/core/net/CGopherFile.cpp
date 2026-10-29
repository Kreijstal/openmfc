// CGopherFile — OpenMFC implementation.
// Sources: global_ctor_quickwins.cpp, inetcore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/InetcoreSupport.h"

// Hand-written ABI thunks for header-defined classes whose exports need
// implementations. Implemented constructor thunks placement-new into
// caller-provided storage to establish the OpenMFC-declared layout; constructors
// whose retail layout is not modeled remain documented stubs. The exact
// `// Symbol:` markers let the build exclude matching weak/typed stubs.

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
// protected retail ctor; the OpenMFC C++ ctor is defined below.
// Symbol: ??0CGopherFile@@IEAA@PEAXAEAVCGopherLocator@@PEAVCGopherConnection@@@Z
extern "C" void* MS_ABI impl___0CGopherFile__IEAA_PEAXAEAVCGopherLocator__PEAVCGopherConnection___Z(
    void* pThis, void* hFile, CGopherLocator* pLocator, CGopherConnection* pConnection)
{
    return new (pThis) CGopherFile(static_cast<HINTERNET>(hFile), *pLocator, pConnection);
}
// GetRuntimeClass/GetThisClass for CGopherFile live in core/net/RuntimeClasses.cpp
// (classCGopherFile, from detail/DeferredRttiSupport.h: size 152, schema 0xFFFF, base
// CInternetFile). That agrees with the retail descriptor at 0x18032a020 in mfc140u.dll
// -- "CGopherFile", m_nObjectSize 152, m_wSchema 0xFFFF, m_pfnGetBaseClass resolving to
// CInternetFile. The copies that used to be here returned g_classCGopherFile, sized
// sizeof(the repo CGopherFile) = 88 with a null base link, so an IsKindOf walk to
// CInternetFile would fail.
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

// STUB: retail initializes the CInternetFile base, installs CGopherFile's
// vtable, then constructs CGopherLocator at this+0x80. OpenMFC's declared class has
// neither this overload nor a locator subobject at the required offset; it cannot
// safely initialize this retail object without the missing layout declaration.
// Retail evidence: ctor entry RVA 0x230ef0 (mfc140u); call at 0x230f22 to
// CInternetFile ctor entry RVA 0x22e480 and call at 0x230f41 to CGopherLocator
// ctor entry RVA 0x231850.
// Symbol: ??0CGopherFile@@IEAA@PEAX0PEB_WK_K@Z
extern "C" void* MS_ABI impl___0CGopherFile__IEAA_PEAX0PEB_WK_K_Z(
    void* pThis, void* hFile, void* hSession, const wchar_t* pstrLocator,
    unsigned long dwLocLen, unsigned __int64 dwContext) {
    (void)pThis;
    (void)hFile;
    (void)hSession;
    (void)pstrLocator;
    (void)dwLocLen;
    (void)dwContext;
    return nullptr;
}

