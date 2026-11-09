// COleException — OpenMFC implementation.
// Sources: global_ctor_quickwins.cpp, mfc_exceptions.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/MfcExceptionsSupport.h"

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

extern "C" void MS_ABI impl__CommonInit_CFile__IEAAXPEB_WIPEAVCAtlTransactionManager_ATL___Z(
    void* pThis, const wchar_t* lpszFileName, unsigned int nOpenFlags, void* pTM);
extern "C" void MS_ABI impl__CommonBaseInit_CStdioFile__IEAAXPEAU_iobuf__PEAVCAtlTransactionManager_ATL___Z(
    void* pThis, FILE* pStream, void* pTM);
extern "C" void MS_ABI impl__CommonInit_CStdioFile__IEAAXPEB_WIPEAVCAtlTransactionManager_ATL___Z(
    void* pThis, const wchar_t* lpszFileName, unsigned int nOpenFlags, void* pTM);















// Symbol: ?GetErrorMessage@COleException@@UEBAHPEA_WIPEAI@Z
extern "C" int MS_ABI impl__GetErrorMessage_COleException__UEBAHPEA_WIPEAI_Z(
    const COleException* pThis, wchar_t* lpszError, UINT nMaxError, UINT* pnHelpContext
) {
    if (!lpszError || nMaxError == 0) return 0;
    if (pnHelpContext) *pnHelpContext = 0;
    if (!pThis) {
        lpszError[0] = L'\0';
        return 0;
    }

    const DWORD flags = FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS;
    DWORD len = FormatMessageW(
        flags, nullptr, static_cast<DWORD>(pThis->m_sc), 0, lpszError, nMaxError, nullptr
    );
    if (len != 0) {
        while (len > 0 && (lpszError[len - 1] == L'\r' || lpszError[len - 1] == L'\n')) {
            lpszError[len - 1] = L'\0';
            --len;
        }
        return 1;
    }

    swprintf(lpszError, nMaxError, L"OLE exception 0x%08lX", static_cast<unsigned long>(pThis->m_sc));
    return 1;
}
