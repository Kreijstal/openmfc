#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxwin.h"
#include "openmfc/afxdb.h"
#include "detail/DatabaseEnvironment.h"
#ifdef __GNUC__
#define MS_ABI __attribute__((ms_abi))
#else
#define MS_ABI
#endif
HENV& OpenMfcDatabaseEnvironment() {
    static HENV environment=SQL_NULL_HENV;
    return environment;
}
HENV AFXAPI AfxGetHENV() {return OpenMfcDatabaseEnvironment();}
LONG& OpenMfcAllocatedDatabaseConnections() {
    static LONG connections=0;
    return connections;
}
// Symbol: ?AfxGetHENV@@YAPEAXXZ
extern "C" void* MS_ABI impl__AfxGetHENV__YAPEAXXZ() {return AfxGetHENV();}
