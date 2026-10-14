// Native release capability discovery: VerifyConnect 0x1f7a50,
// GetConnectInfo 0x1f7b40 in the verified mfc140u image.
#define OPENMFC_APPCORE_IMPL
#include "detail/CRecordsetSupport.h"
// Symbol: ?VerifyConnect@CDatabase@@IEAAXXZ
extern "C" void MS_ABI impl__VerifyConnect_CDatabase__IEAAXXZ(CDatabase* database) {
    SWORD api,sql;SQLSMALLINT length;
    RETCODE code=::SQLGetInfoW(database->m_hdbc,SQL_ODBC_API_CONFORMANCE,&api,sizeof(api),&length);
    if(!database->Check(code))database->ThrowDBException(code);
    if(api<SQL_OAC_LEVEL1)database->ThrowDBException(1014);
    code=::SQLGetInfoW(database->m_hdbc,SQL_ODBC_SQL_CONFORMANCE,&sql,sizeof(sql),&length);
    if(!database->Check(code))database->ThrowDBException(code);
    if(sql<SQL_OSC_MINIMUM)database->ThrowDBException(1015);
}
// Symbol: ?GetConnectInfo@CDatabase@@IEAAXXZ
extern "C" void MS_ABI impl__GetConnectInfo_CDatabase__IEAAXXZ(CDatabase* database) {
    SQLSMALLINT length;DWORD value;SWORD capability;
    auto info=[&](SQLUSMALLINT kind,void* target,SQLSMALLINT size) {
        return database->Check(::SQLGetInfoW(database->m_hdbc,kind,target,size,&length));
    };
    database->m_dwUpdateOptions=0;
    if(info(SQL_POS_OPERATIONS,&value,sizeof(value)) && (value&0x1c)==0x1c)database->m_dwUpdateOptions|=1;
    if(info(SQL_POSITIONED_STATEMENTS,&value,sizeof(value)) && (value&3)==3)database->m_dwUpdateOptions|=2;
    if(info(SQL_TXN_CAPABLE,&capability,sizeof(capability)) && capability)database->m_bTransactions=TRUE;
    if(!info(SQL_CURSOR_COMMIT_BEHAVIOR,&database->m_nCursorCommitBehavior,sizeof(SWORD)))database->m_nCursorCommitBehavior=-1;
    if(!info(SQL_CURSOR_ROLLBACK_BEHAVIOR,&database->m_nCursorRollbackBehavior,sizeof(SWORD)))database->m_nCursorRollbackBehavior=-1;
    info(SQL_BOOKMARK_PERSISTENCE,&database->m_dwBookmarkAttributes,sizeof(DWORD));
    if(!info(SQL_GETDATA_EXTENSIONS,&value,sizeof(value)))value=0;
    if(value&SQL_GD_BOUND)database->m_dwUpdateOptions|=4;
    if(database->m_bUpdatable) {
        wchar_t readOnly[10];
        database->m_bUpdatable=info(SQL_DATA_SOURCE_READ_ONLY,readOnly,sizeof(readOnly)) && length==sizeof(wchar_t) && readOnly[0]==L'N';
    } else ::SQLSetConnectAttr(database->m_hdbc,SQL_ACCESS_MODE,reinterpret_cast<SQLPOINTER>(UINT_PTR(SQL_MODE_READ_ONLY)),0);
    wchar_t quote[2];
    database->m_chIDQuoteChar=info(SQL_IDENTIFIER_QUOTE_CHAR,quote,sizeof(quote)) && length==sizeof(wchar_t)?quote[0]:L'"';
}
