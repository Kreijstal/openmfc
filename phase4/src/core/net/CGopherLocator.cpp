// CGopherLocator — OpenMFC implementation.
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















// CGopherLocator::CGopherLocator(const wchar_t* pstrLocator, DWORD dwLen) —
// private ctor that takes ownership of a copy of the locator buffer. dwLen is the
// locator length in characters; the buffer is malloc'd to match ~CGopherLocator,
// which releases it with free().
// Symbol: ??0CGopherLocator@@AEAA@PEB_WK@Z
extern "C" void* MS_ABI impl___0CGopherLocator__AEAA_PEB_WK_Z(
    void* pThis, const wchar_t* pstrLocator, DWORD dwLen)
{
    CGopherLocator* p = new (pThis) CGopherLocator();
    if (pstrLocator) {
        wchar_t* buf = static_cast<wchar_t*>(std::malloc((static_cast<size_t>(dwLen) + 1) * sizeof(wchar_t)));
        if (buf) {
            std::memcpy(buf, pstrLocator, static_cast<size_t>(dwLen) * sizeof(wchar_t));
            buf[dwLen] = L'\0';
            p->m_lpBuffer = buf;
            p->m_dwBufferLength = dwLen;
        }
    }
    return p;
}
