// Globals — OpenMFC implementation.
// Sources: wincore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/WincoreSupport.h"

// Symbol: ?AfxWinMain@@YAHPEAUHINSTANCE__@@0PEA_WH@Z
// Ordinal: 2374
extern "C" int MS_ABI impl__AfxWinMain__YAHPEAUHINSTANCE____0PEA_WH_Z(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    wchar_t* lpCmdLine,
    int nCmdShow)
{
    (void)hPrevInstance;

    // Get the global CWinApp instance
    CWinApp* pApp = AfxGetApp();
    if (!pApp) {
        return -1;  // No application object
    }

    // Store instance handle
    pApp->m_hInstance = hInstance;
    pApp->m_lpCmdLine = lpCmdLine;
    pApp->m_nCmdShow = nCmdShow;

    // Initialize MFC
    if (!AfxWinInit(hInstance, hPrevInstance, lpCmdLine, nCmdShow)) {
        return -1;
    }

    // Call InitInstance (virtual - user overrides this)
    if (!pApp->InitInstance()) {
        return pApp->ExitInstance();
    }

    // Run message loop
    int nReturnCode = pApp->Run();

    return nReturnCode;
}
