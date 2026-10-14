// Release x64 ExecuteSQL: verified mfc140u entry 0x1f6ee0.
#define OPENMFC_APPCORE_IMPL
#include "detail/CRecordsetSupport.h"
extern "C" void MS_ABI impl__AfxThrowInvalidArgException__YAXXZ();
namespace {
bool Accepted(RETCODE code) {return code==SQL_SUCCESS || code==SQL_SUCCESS_WITH_INFO || code==SQL_NO_DATA;}
void CheckStatement(RETCODE code,CDatabase* database,HSTMT statement) {
    if(!Accepted(code))AfxThrowDBException(code,database,statement);
}
}
// Symbol: ?ExecuteSQL@CDatabase@@QEAAXPEB_W@Z
extern "C" void MS_ABI impl__ExecuteSQL_CDatabase__QEAAXPEB_W_Z(CDatabase* database,const wchar_t* sql) {
    if(!database || !sql) {impl__AfxThrowInvalidArgException__YAXXZ();return;}
    HSTMT statement=nullptr;
    RETCODE code=::SQLAllocStmt(database->m_hdbc,&statement);
    CheckStatement(code,database,statement);
    try {
        database->OnSetOptions(statement);database->BindParameters(statement);
        do {code=::SQLExecDirectW(statement,reinterpret_cast<SQLWCHAR*>(const_cast<wchar_t*>(sql)),SQL_NTS);}while(code==SQL_STILL_EXECUTING);
        CheckStatement(code,database,statement);
        do {
            SQLSMALLINT columns;
            do {code=::SQLNumResultCols(statement,&columns);}while(code==SQL_STILL_EXECUTING);
            CheckStatement(code,database,statement);
            if(columns) {
                do {
                    do {code=::SQLFetch(statement);}while(code==SQL_STILL_EXECUTING);
                    CheckStatement(code,database,statement);
                }while(code!=SQL_NO_DATA);
            }
            do {code=::SQLMoreResults(statement);}while(code==SQL_STILL_EXECUTING);
            CheckStatement(code,database,statement);
        }while(code!=SQL_NO_DATA);
    }catch(CException*) {::SQLCancel(statement);::SQLFreeStmt(statement,SQL_DROP);throw;}
    ::SQLFreeStmt(statement,SQL_DROP);
}
void CDatabase::ExecuteSQL(const wchar_t* sql) {impl__ExecuteSQL_CDatabase__QEAAXPEB_W_Z(this,sql);}
