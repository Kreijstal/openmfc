// CDatabase — OpenMFC implementation.
// Sources: dbcore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/CRecordsetSupport.h"
#include <cstddef>
extern "C" void* MS_ABI impl___0CPtrList__QEAA__J_Z(void*,INT_PTR);
extern "C" void MS_ABI impl___1CPtrList__UEAA_XZ(void*);
extern "C" void MS_ABI impl__AfxThrowDBException__YAXFPEAVCDatabase__PEAX_Z(RETCODE,CDatabase*,HSTMT);
IMPLEMENT_DYNAMIC(CDatabase,CObject)
namespace openmfc { namespace detail { namespace crecordset {
std::unordered_map<const CDatabase*,DbState> g_databaseStates;
} } }

// Symbol: ?GetRuntimeClass@CDatabase@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CDatabase__UEBAPEAUCRuntimeClass__XZ(const CDatabase* pThis) {
    (void)pThis;
    return CDatabase::GetThisClass();
}
// Symbol: ?GetThisClass@CDatabase@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CDatabase__SAPEAUCRuntimeClass__XZ() {
    return CDatabase::GetThisClass();
}

// Symbol: ?OnSetOptions@CDatabase@@UEAAXPEAX@Z
extern "C" void MS_ABI impl__OnSetOptions_CDatabase__UEAAXPEAX_Z(CDatabase* pThis, void* hstmt) {
    pThis->CDatabase::OnSetOptions(static_cast<HSTMT>(hstmt));
}
// Symbol: ?ReplaceBrackets@CDatabase@@QEAAXPEA_W@Z
extern "C" void MS_ABI impl__ReplaceBrackets_CDatabase__QEAAXPEA_W_Z(CDatabase* pThis, wchar_t* lpszSQL) {
    bool quoted=false;
    wchar_t* output=lpszSQL;
    for(wchar_t* input=lpszSQL;*input;++input,++output) {
        wchar_t ch=*input;
        if(ch==L'\'') {
            if(input[1]==ch) {*output++=ch;++input;}
            else quoted=!quoted;
        } else if(!quoted && (ch==L'[' || ch==L']')) {
            if(input[1]==ch)++input;
            else ch=pThis->m_chIDQuoteChar;
        }
        *output=ch;
    }
    *output=0;
}
extern "C" void MS_ABI impl__AllocConnect_CDatabase__IEAAXK_Z(CDatabase*,DWORD);
extern "C" void MS_ABI impl__Free_CDatabase__IEAAXXZ(CDatabase*);

// Symbol: ?BindParameters@CDatabase@@UEAAXPEAX@Z
extern "C" void MS_ABI impl__BindParameters_CDatabase__UEAAXPEAX_Z(CDatabase* pThis, void* pBind) {
    (void)pThis;(void)pBind;
}
// Symbol: ?Check@CDatabase@@UEBAHF@Z
extern "C" int MS_ABI impl__Check_CDatabase__UEBAHF_Z(const CDatabase* pThis, short retCode) {
    (void)pThis;
    return retCode==SQL_SUCCESS || retCode==SQL_SUCCESS_WITH_INFO || retCode==SQL_NO_DATA;
}
// Symbol: ?CheckHstmt@CDatabase@@QEBAHFPEAX@Z
extern "C" int MS_ABI impl__CheckHstmt_CDatabase__QEBAHFPEAX_Z(const CDatabase* pThis, short retCode, void*) {
    return impl__Check_CDatabase__UEBAHF_Z(pThis, retCode);
}


CDatabase::CDatabase() {
    impl___0CPtrList__QEAA__J_Z(m_listRecordsets,10);
    m_hdbc=SQL_NULL_HDBC;m_hstmt=SQL_NULL_HSTMT;
    m_bStripTrailingSpaces=FALSE;m_bIncRecordCountOnAdd=FALSE;m_bAddForUpdate=FALSE;
    m_bUpdatable=FALSE;m_bTransactions=FALSE;
    m_dwLoginTimeout=15;m_dwQueryTimeout=15;m_blobConnect.pbData=nullptr;
    g_databaseStates[this]=DbState{};
}
CDatabase::~CDatabase() {
    impl__Free_CDatabase__IEAAXXZ(this);
    impl___1CPtrList__UEAA_XZ(m_listRecordsets);
    g_databaseStates.erase(this);
}

BOOL CDatabase::ExecuteSQL(const wchar_t* lpszSQL) {
    if (!IsOpen() || !lpszSQL) return FALSE;
    HSTMT hstmt = SQL_NULL_HSTMT;
    if (SQLAllocHandle(SQL_HANDLE_STMT, m_hdbc, &hstmt) != SQL_SUCCESS) {
        return FALSE;
    }
    RETCODE rc = SQLExecDirectW(hstmt, (SQLWCHAR*)lpszSQL, SQL_NTS);
    SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
    return (rc == SQL_SUCCESS || rc == SQL_SUCCESS_WITH_INFO);
}
BOOL CDatabase::CanTransact() const {return m_bTransactions;}
BOOL CDatabase::CanUpdate() const {return m_bUpdatable;}
int CDatabase::GetLoginTimeout() const {return static_cast<int>(m_dwLoginTimeout);}
void CDatabase::SetLoginTimeout(DWORD seconds) {m_dwLoginTimeout=seconds;}
int CDatabase::GetQueryTimeout() const {return static_cast<int>(m_dwQueryTimeout);}
void CDatabase::SetQueryTimeout(DWORD seconds) {m_dwQueryTimeout=seconds;}

HENV CDatabase::GetHENV() const {return AfxGetHENV();}

void CDatabase::OnSetOptions(HSTMT statement) {
    if(m_dwQueryTimeout!=DWORD(-1) && !Check(SQLSetStmtOption(statement,SQL_QUERY_TIMEOUT,m_dwQueryTimeout)))
        m_dwQueryTimeout=DWORD(-1);
}
void CDatabase::BindParameters(HSTMT statement) {impl__BindParameters_CDatabase__UEAAXPEAX_Z(this,statement);}
BOOL CDatabase::Check(RETCODE code) const {return impl__Check_CDatabase__UEBAHF_Z(this,code);}
void CDatabase::ThrowDBException(RETCODE code) {impl__AfxThrowDBException__YAXFPEAVCDatabase__PEAX_Z(code,this,m_hstmt);}
static_assert(sizeof(CDatabase)==160,"native release CDatabase size");
static_assert(offsetof(CDatabase,m_bStripTrailingSpaces)==16 && offsetof(CDatabase,m_strConnect)==32,"native database text-fixup fields");
static_assert(offsetof(CDatabase,m_listRecordsets)==56 && offsetof(CDatabase,m_hstmt)==144 && offsetof(CDatabase,m_dwQueryTimeout)==152,"native database list, statement and timeout fields");

// Symbol: ??1CDatabase@@UEAA@XZ
extern "C" void MS_ABI impl___1CDatabase__UEAA_XZ(CDatabase* database) {database->CDatabase::~CDatabase();}
