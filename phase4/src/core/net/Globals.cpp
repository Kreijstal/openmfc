// Globals — OpenMFC implementation.
// Sources: sockcore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/SockcoreSupport.h"

// Symbol: ?AfxSocketTerm@@YAXXZ
extern "C" void MS_ABI impl__AfxSocketTerm__YAXXZ() {
    if (g_bSocketsInitialized) {
        WSACleanup();
        g_bSocketsInitialized = FALSE;
    }
}
// Symbol: ?_AfxSocketInit@@YAHPEAUWSAData@@@Z
extern "C" int MS_ABI impl___AfxSocketInit__YAHPEAUWSAData___Z(WSADATA* lpwsaData) {
    return AfxSocketInit(lpwsaData);
}
