// CDBException — OpenMFC implementation.
// Sources: dbcore.cpp, global_ctordtor_param.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/CRecordsetSupport.h"

// Real parameterized-constructor export thunks for header-defined, publicly
// constructible MFC classes. Each viewed-as MS_ABI thunk placement-constructs
// the object with the real ctor body (defined in filecore/olecore/dlgcommon/
// inetcore or inline in headers) -- NOT a stub. Completes classes that were one
// export short of full coverage.
#include "openmfc/afx.h"
#include "openmfc/afxwin.h"
#include "openmfc/afxmfc.h"
#include "openmfc/afxole.h"
#include "openmfc/afxdisp.h"
#include "openmfc/afxdb.h"
#include "openmfc/afxinet.h"
#include <new>
#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif


















// Symbol: ?GetThisClass@CDBException@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CDBException__SAPEAUCRuntimeClass__XZ() {
    return CException::GetThisClass();
}
// Symbol: ?GetRuntimeClass@CDBException@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CDBException__UEBAPEAUCRuntimeClass__XZ(const CDBException*) {
    return CException::GetThisClass();
}
// Symbol: ?Empty@CDBException@@QEAAXXZ
extern "C" void MS_ABI impl__Empty_CDBException__QEAAXXZ(CDBException* pThis) {
    if (!pThis) return;
    pThis->m_nRetCode = SQL_SUCCESS;
    pThis->m_strError = L"";
    pThis->m_strStateNativeOrigin = L"";
    pThis->m_strErrorNativeOrigin = L"";
}
// Symbol: ?BuildErrorString@CDBException@@UEAAXPEAVCDatabase@@PEAXH@Z
extern "C" void MS_ABI impl__BuildErrorString_CDBException__UEAAXPEAVCDatabase__PEAXH_Z(
    CDBException* pThis, CDatabase* database, void* hstmt, int) {
    if (!pThis) return;
    SQLSMALLINT handleType = hstmt ? SQL_HANDLE_STMT : SQL_HANDLE_DBC;
    SQLHANDLE handle = hstmt ? static_cast<SQLHANDLE>(hstmt) : (database ? static_cast<SQLHANDLE>(database->m_hdbc) : SQL_NULL_HANDLE);
    SQLWCHAR state[16] = {};
    SQLWCHAR message[SQL_MAX_MESSAGE_LENGTH] = {};
    SQLINTEGER native = 0;
    SQLSMALLINT messageLen = 0;
    RETCODE rc = handle ? SQLGetDiagRecW(handleType, handle, 1, state, &native, message,
                                         SQL_MAX_MESSAGE_LENGTH, &messageLen) : SQL_NO_DATA;
    if (SqlSucceeded(rc)) {
        pThis->m_strStateNativeOrigin = reinterpret_cast<const wchar_t*>(state);
        pThis->m_strError = reinterpret_cast<const wchar_t*>(message);
        wchar_t nativeText[64] = {};
        std::swprintf(nativeText, 64, L"%ld", static_cast<long>(native));
        pThis->m_strErrorNativeOrigin = nativeText;
    } else if (pThis->m_strError.IsEmpty()) {
        pThis->m_strError = L"ODBC database operation failed.";
    }
}
// Symbol: ?GetErrorMessage@CDBException@@UEBAHPEA_WIPEAI@Z
extern "C" int MS_ABI impl__GetErrorMessage_CDBException__UEBAHPEA_WIPEAI_Z(
    const CDBException* pThis, wchar_t* error, unsigned int maxError, unsigned int* helpContext) {
    if (helpContext) *helpContext = 0;
    if (!error || maxError == 0) return FALSE;
    const wchar_t* text = (pThis && !pThis->m_strError.IsEmpty())
        ? static_cast<const wchar_t*>(pThis->m_strError)
        : L"ODBC database operation failed.";
    wcsncpy(error, text, maxError - 1);
    error[maxError - 1] = L'\0';
    return TRUE;
}
// Symbol: ??0CDBException@@QEAA@F@Z
extern "C" void* MS_ABI impl___0CDBException__QEAA_F_Z(void* p, short nRetCode) {
    return new (p) CDBException(nRetCode);
}
