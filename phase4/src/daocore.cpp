// OpenMFC DAO Database Classes Implementation
// Stub implementations for ABI compatibility.
// Real DAO functionality requires the DAO 3.6 runtime (dao360.dll).
//
// All exports use MSVC ABI via extern "C" + .def aliasing.

#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxdao.h"
#include <cstring>
#include <mutex>
#include <unordered_map>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

namespace {
struct DaoWorkspaceState {
    BOOL inTransaction = FALSE;
};

struct DaoDatabaseState {
    CString name;
    CString connect;
    BOOL inTransaction = FALSE;
};

struct DaoTableDefState {
    CString name;
    CString connect;
    CString sourceTableName;
    long attributes = 0;
};

struct DaoQueryDefState {
    CString name;
    CString connect;
    long timeout = 60;
    long recordsAffected = 0;
    BOOL returnsRecords = TRUE;
    long type = 0;
};

std::mutex g_daoStateMutex;
std::unordered_map<const CDaoWorkspace*, DaoWorkspaceState> g_workspaceStates;
std::unordered_map<const CDaoDatabase*, DaoDatabaseState> g_databaseStates;
std::unordered_map<const CDaoTableDef*, DaoTableDefState> g_tableDefStates;
std::unordered_map<const CDaoQueryDef*, DaoQueryDefState> g_queryDefStates;
}

//=============================================================================
// CDaoException - IMPLEMENT_DYNAMIC (class declared in header with inline methods)
//=============================================================================
IMPLEMENT_DYNAMIC(CDaoException, CException)

//=============================================================================
// CDaoWorkspace
//=============================================================================
IMPLEMENT_DYNAMIC(CDaoWorkspace, CObject)

CDaoWorkspace::CDaoWorkspace()
    : m_pDAOWorkspace(nullptr), m_bOpen(FALSE) {
    memset(_daoworkspace_padding, 0, sizeof(_daoworkspace_padding));
    std::lock_guard<std::mutex> lock(g_daoStateMutex);
    g_workspaceStates[this] = DaoWorkspaceState{};
}

CDaoWorkspace::CDaoWorkspace(CDaoDatabase* pDatabase)
    : m_pDAOWorkspace(nullptr), m_bOpen(FALSE) {
    (void)pDatabase;
    memset(_daoworkspace_padding, 0, sizeof(_daoworkspace_padding));
    std::lock_guard<std::mutex> lock(g_daoStateMutex);
    g_workspaceStates[this] = DaoWorkspaceState{};
}

CDaoWorkspace::~CDaoWorkspace() {
    Close();
    std::lock_guard<std::mutex> lock(g_daoStateMutex);
    g_workspaceStates.erase(this);
}

CString CDaoWorkspace::GetName() const {
    return CString(L"#Default Workspace#");
}

CString CDaoWorkspace::GetUserName() const {
    return CString(L"Admin");
}

BOOL CDaoWorkspace::IsOpen() const {
    return m_bOpen;
}

CDaoDatabase* CDaoWorkspace::GetDatabase(int nIndex) {
    (void)nIndex;
    return nullptr;
}

int CDaoWorkspace::GetDatabaseCount() const {
    return 0;
}

void CDaoWorkspace::BeginTrans() {
    std::lock_guard<std::mutex> lock(g_daoStateMutex);
    g_workspaceStates[this].inTransaction = TRUE;
}

void CDaoWorkspace::CommitTrans() {
    std::lock_guard<std::mutex> lock(g_daoStateMutex);
    g_workspaceStates[this].inTransaction = FALSE;
}

void CDaoWorkspace::Rollback() {
    std::lock_guard<std::mutex> lock(g_daoStateMutex);
    g_workspaceStates[this].inTransaction = FALSE;
}

BOOL CDaoWorkspace::GetInTransaction() const {
    std::lock_guard<std::mutex> lock(g_daoStateMutex);
    auto it = g_workspaceStates.find(this);
    return (it != g_workspaceStates.end()) ? it->second.inTransaction : FALSE;
}

void CDaoWorkspace::Close() {
    m_bOpen = FALSE;
    m_pDAOWorkspace = nullptr;
    std::lock_guard<std::mutex> lock(g_daoStateMutex);
    g_workspaceStates[this].inTransaction = FALSE;
}

CDaoWorkspace* CDaoWorkspace::GetDefaultWorkspace() {
    static CDaoWorkspace* pDefault = nullptr;
    if (!pDefault) pDefault = new CDaoWorkspace();
    return pDefault;
}

CString CDaoWorkspace::GetVersion() {
    return CString(L"3.6");
}

//=============================================================================
// CDaoDatabase
//=============================================================================
IMPLEMENT_DYNAMIC(CDaoDatabase, CObject)

CDaoDatabase::CDaoDatabase(CDaoWorkspace* pWorkspace)
    : m_pDAODatabase(nullptr), m_pWorkspace(pWorkspace), m_bOpen(FALSE),
      m_lRecordsAffected(0) {
    memset(_daodatabase_padding, 0, sizeof(_daodatabase_padding));
    if (!m_pWorkspace) m_pWorkspace = CDaoWorkspace::GetDefaultWorkspace();
    std::lock_guard<std::mutex> lock(g_daoStateMutex);
    g_databaseStates[this] = DaoDatabaseState{};
}

CDaoDatabase::~CDaoDatabase() {
    Close();
    std::lock_guard<std::mutex> lock(g_daoStateMutex);
    g_databaseStates.erase(this);
}

void CDaoDatabase::Open(const wchar_t* lpszName, BOOL bExclusive,
                        BOOL bReadOnly, const wchar_t* lpszConnect) {
    (void)bExclusive; (void)bReadOnly;
    m_bOpen = TRUE;
    std::lock_guard<std::mutex> lock(g_daoStateMutex);
    DaoDatabaseState& state = g_databaseStates[this];
    state.name = lpszName ? CString(lpszName) : CString();
    state.connect = lpszConnect ? CString(lpszConnect) : CString();
    state.inTransaction = FALSE;
}

void CDaoDatabase::Close() {
    m_bOpen = FALSE;
    m_pDAODatabase = nullptr;
    std::lock_guard<std::mutex> lock(g_daoStateMutex);
    g_databaseStates[this].inTransaction = FALSE;
}

BOOL CDaoDatabase::IsOpen() const {
    return m_bOpen;
}

CString CDaoDatabase::GetName() const {
    std::lock_guard<std::mutex> lock(g_daoStateMutex);
    auto it = g_databaseStates.find(this);
    return (it != g_databaseStates.end()) ? it->second.name : CString();
}

CString CDaoDatabase::GetConnect() const {
    std::lock_guard<std::mutex> lock(g_daoStateMutex);
    auto it = g_databaseStates.find(this);
    return (it != g_databaseStates.end()) ? it->second.connect : CString();
}

long CDaoDatabase::GetRecordsAffected() const {
    return m_lRecordsAffected;
}

BOOL CDaoDatabase::CanUpdate() const {
    return TRUE;
}

BOOL CDaoDatabase::CanTransact() const {
    return TRUE;
}

int CDaoDatabase::GetTableDefCount() const {
    return 0;
}

void CDaoDatabase::GetTableDefInfo(int nIndex, CDaoTableDefInfo& tabledefinfo,
                                    DWORD dwInfoOptions) {
    (void)nIndex; (void)tabledefinfo; (void)dwInfoOptions;
}

void CDaoDatabase::CreateTableDef(const wchar_t* lpszName, long lAttributes,
                                   long lOptions, const wchar_t* lpszSource,
                                   const wchar_t* lpszConnect) {
    (void)lpszName; (void)lAttributes; (void)lOptions;
    (void)lpszSource; (void)lpszConnect;
}

void CDaoDatabase::DeleteTableDef(const wchar_t* lpszName) {
    (void)lpszName;
}

CDaoQueryDef* CDaoDatabase::CreateQueryDef(const wchar_t* lpszName,
                                            const wchar_t* lpszSQL) {
    CDaoQueryDef* pQD = new CDaoQueryDef(this);
    pQD->m_bOpen = TRUE;
    if (lpszName) pQD->SetName(lpszName);
    if (lpszSQL) pQD->SetSQL(lpszSQL);
    return pQD;
}

void CDaoDatabase::DeleteQueryDef(const wchar_t* lpszName) {
    (void)lpszName;
}

void CDaoDatabase::BeginTrans() {
    std::lock_guard<std::mutex> lock(g_daoStateMutex);
    g_databaseStates[this].inTransaction = TRUE;
}

void CDaoDatabase::CommitTrans() {
    std::lock_guard<std::mutex> lock(g_daoStateMutex);
    g_databaseStates[this].inTransaction = FALSE;
}

void CDaoDatabase::Rollback() {
    std::lock_guard<std::mutex> lock(g_daoStateMutex);
    g_databaseStates[this].inTransaction = FALSE;
}

BOOL CDaoDatabase::GetInTransaction() const {
    std::lock_guard<std::mutex> lock(g_daoStateMutex);
    auto it = g_databaseStates.find(this);
    return (it != g_databaseStates.end()) ? it->second.inTransaction : FALSE;
}

void CDaoDatabase::Execute(const wchar_t* lpszSQL, int nOptions) {
    (void)lpszSQL; (void)nOptions;
}

void CDaoDatabase::CreateRelation(const wchar_t* lpszName, const wchar_t* lpszTable,
                                   const wchar_t* lpszForeignTable, long lAttributes,
                                   const wchar_t* lpszField, const wchar_t* lpszForeignField) {
    (void)lpszName; (void)lpszTable; (void)lpszForeignTable;
    (void)lAttributes; (void)lpszField; (void)lpszForeignField;
}

void CDaoDatabase::CreateRelation(CDaoRelationInfo& relinfo) {
    (void)relinfo;
}

void CDaoDatabase::DeleteRelation(const wchar_t* lpszName) {
    (void)lpszName;
}

CDaoWorkspace* CDaoDatabase::GetWorkspace() {
    return m_pWorkspace;
}

//=============================================================================
// CDaoRecordset
//=============================================================================
IMPLEMENT_DYNAMIC(CDaoRecordset, CObject)

CDaoRecordset::CDaoRecordset(CDaoDatabase* pDatabase)
    : m_pDatabase(pDatabase), m_pDAORecordset(nullptr), m_bOpen(FALSE),
      m_nFields(0), m_lRecordCount(0) {
    memset(_daorecordset_padding, 0, sizeof(_daorecordset_padding));
}

CDaoRecordset::~CDaoRecordset() {
    Close();
}

void CDaoRecordset::Open(int nOpenType, const wchar_t* lpszSQL, int nOptions) {
    (void)nOpenType; (void)nOptions;
    if (lpszSQL) m_strSQL = lpszSQL;
    m_bOpen = TRUE;
}

void CDaoRecordset::Open(CDaoTableDef* pTableDef, int nOpenType, int nOptions) {
    (void)pTableDef; (void)nOpenType; (void)nOptions;
    m_bOpen = TRUE;
}

void CDaoRecordset::Open(CDaoQueryDef* pQueryDef, int nOpenType, int nOptions) {
    (void)pQueryDef; (void)nOpenType; (void)nOptions;
    m_bOpen = TRUE;
}

void CDaoRecordset::Close() {
    m_bOpen = FALSE;
    m_pDAORecordset = nullptr;
}

BOOL CDaoRecordset::IsOpen() const { return m_bOpen; }
BOOL CDaoRecordset::IsBOF() const { return FALSE; }
BOOL CDaoRecordset::IsEOF() const { return TRUE; }
BOOL CDaoRecordset::CanUpdate() const { return TRUE; }
BOOL CDaoRecordset::CanAppend() const { return TRUE; }

void CDaoRecordset::DoFieldExchange(CDaoFieldExchange* pFX) {
    (void)pFX;
}

void CDaoRecordset::MoveFirst() {}
void CDaoRecordset::MoveLast() {}
void CDaoRecordset::MoveNext() {}
void CDaoRecordset::MovePrev() {}
void CDaoRecordset::Move(long lRows) { (void)lRows; }

long CDaoRecordset::GetRecordCount() const { return 0; }
long CDaoRecordset::GetAbsolutePosition() const { return -1; }
void CDaoRecordset::SetAbsolutePosition(long lPos) { (void)lPos; }
double CDaoRecordset::GetPercentPosition() const { return 0.0; }
void CDaoRecordset::SetPercentPosition(double dPosition) { (void)dPosition; }

void CDaoRecordset::FindFirst(const wchar_t* lpszCriteria) { (void)lpszCriteria; }
void CDaoRecordset::FindLast(const wchar_t* lpszCriteria) { (void)lpszCriteria; }
void CDaoRecordset::FindNext(const wchar_t* lpszCriteria) { (void)lpszCriteria; }
void CDaoRecordset::FindPrev(const wchar_t* lpszCriteria) { (void)lpszCriteria; }

void CDaoRecordset::AddNew() {}
void CDaoRecordset::Edit() {}
void CDaoRecordset::Update() {}
void CDaoRecordset::Delete() {}
void CDaoRecordset::CancelUpdate() {}

void CDaoRecordset::SetBookmark() {}

COleVariant CDaoRecordset::GetBookmark() {
    COleVariant var;
    var.Clear();
    return var;
}

void CDaoRecordset::GetFieldValue(const wchar_t* lpszName, COleVariant& varValue) {
    (void)lpszName; varValue.Clear();
}

void CDaoRecordset::GetFieldValue(int nIndex, COleVariant& varValue) {
    (void)nIndex; varValue.Clear();
}

void CDaoRecordset::SetFieldValue(const wchar_t* lpszName, const COleVariant& varValue) {
    (void)lpszName; (void)varValue;
}

void CDaoRecordset::SetFieldValue(int nIndex, const COleVariant& varValue) {
    (void)nIndex; (void)varValue;
}

void CDaoRecordset::SetCurrentIndex(const wchar_t* lpszIndex) {
    if (lpszIndex) m_strCurrentIndex = lpszIndex;
}

CString CDaoRecordset::GetCurrentIndex() const {
    return m_strCurrentIndex;
}

CString CDaoRecordset::GetSQL() const {
    return m_strSQL;
}

void CDaoRecordset::SetSQL(const wchar_t* lpszSQL) {
    if (lpszSQL) m_strSQL = lpszSQL;
}

CDaoDatabase* CDaoRecordset::GetDatabase() const {
    return m_pDatabase;
}

int CDaoRecordset::GetFieldCount() const {
    return m_nFields;
}

void CDaoRecordset::GetFieldInfo(int nIndex, CDaoFieldInfo& fieldinfo,
                                  DWORD dwInfoOptions) {
    (void)nIndex; (void)fieldinfo; (void)dwInfoOptions;
}

void CDaoRecordset::GetFieldInfo(const wchar_t* lpszName, CDaoFieldInfo& fieldinfo,
                                  DWORD dwInfoOptions) {
    (void)lpszName; (void)fieldinfo; (void)dwInfoOptions;
}

int CDaoRecordset::GetIndexCount() const { return 0; }

void CDaoRecordset::GetIndexInfo(int nIndex, CDaoIndexInfo& indexinfo,
                                  DWORD dwInfoOptions) {
    (void)nIndex; (void)indexinfo; (void)dwInfoOptions;
}

void CDaoRecordset::GetIndexInfo(const wchar_t* lpszName, CDaoIndexInfo& indexinfo,
                                  DWORD dwInfoOptions) {
    (void)lpszName; (void)indexinfo; (void)dwInfoOptions;
}

BOOL CDaoRecordset::Seek(const wchar_t* lpszComparison, COleVariant* pKey1,
                          COleVariant* pKey2, COleVariant* pKey3) {
    (void)lpszComparison; (void)pKey1; (void)pKey2; (void)pKey3;
    return FALSE;
}

CString CDaoRecordset::GetValidationRule() const {
    return CString();
}

CString CDaoRecordset::GetValidationText() const {
    return CString();
}

CDaoDatabase* CDaoRecordset::GetDefaultDB() {
    return m_pDatabase;
}

CString CDaoRecordset::GetDefaultSQL() {
    return m_strSQL;
}

void CDaoRecordset::FillCache(long* pSize, COleVariant* pBookmark) {
    (void)pSize; (void)pBookmark;
}

//=============================================================================
// CDaoTableDef
//=============================================================================
IMPLEMENT_DYNAMIC(CDaoTableDef, CObject)

CDaoTableDef::CDaoTableDef(CDaoDatabase* pDatabase)
    : m_pDatabase(pDatabase), m_pDAOTableDef(nullptr), m_bOpen(FALSE) {
    memset(_daotabledef_padding, 0, sizeof(_daotabledef_padding));
    std::lock_guard<std::mutex> lock(g_daoStateMutex);
    g_tableDefStates[this] = DaoTableDefState{};
}

CDaoTableDef::~CDaoTableDef() {
    Close();
    std::lock_guard<std::mutex> lock(g_daoStateMutex);
    g_tableDefStates.erase(this);
}

void CDaoTableDef::Open(const wchar_t* lpszName) {
    std::lock_guard<std::mutex> lock(g_daoStateMutex);
    g_tableDefStates[this].name = lpszName ? CString(lpszName) : CString();
    m_bOpen = TRUE;
}

void CDaoTableDef::Create(const wchar_t* lpszName, long lAttributes,
                          const wchar_t* lpszSrcTable, const wchar_t* lpszConnect) {
    std::lock_guard<std::mutex> lock(g_daoStateMutex);
    DaoTableDefState& state = g_tableDefStates[this];
    state.name = lpszName ? CString(lpszName) : CString();
    state.attributes = lAttributes;
    state.sourceTableName = lpszSrcTable ? CString(lpszSrcTable) : CString();
    state.connect = lpszConnect ? CString(lpszConnect) : CString();
    m_bOpen = TRUE;
}

void CDaoTableDef::Close() {
    m_bOpen = FALSE;
    m_pDAOTableDef = nullptr;
}

BOOL CDaoTableDef::IsOpen() const { return m_bOpen; }
CString CDaoTableDef::GetName() const {
    std::lock_guard<std::mutex> lock(g_daoStateMutex);
    auto it = g_tableDefStates.find(this);
    return (it != g_tableDefStates.end()) ? it->second.name : CString();
}
void CDaoTableDef::SetName(const wchar_t* lpszName) {
    std::lock_guard<std::mutex> lock(g_daoStateMutex);
    g_tableDefStates[this].name = lpszName ? CString(lpszName) : CString();
}
CString CDaoTableDef::GetConnect() const {
    std::lock_guard<std::mutex> lock(g_daoStateMutex);
    auto it = g_tableDefStates.find(this);
    return (it != g_tableDefStates.end()) ? it->second.connect : CString();
}
CString CDaoTableDef::GetSourceTableName() const {
    std::lock_guard<std::mutex> lock(g_daoStateMutex);
    auto it = g_tableDefStates.find(this);
    return (it != g_tableDefStates.end()) ? it->second.sourceTableName : CString();
}
void CDaoTableDef::SetSourceTableName(const wchar_t* lpszSrcTable) {
    std::lock_guard<std::mutex> lock(g_daoStateMutex);
    g_tableDefStates[this].sourceTableName = lpszSrcTable ? CString(lpszSrcTable) : CString();
}
long CDaoTableDef::GetAttributes() const {
    std::lock_guard<std::mutex> lock(g_daoStateMutex);
    auto it = g_tableDefStates.find(this);
    return (it != g_tableDefStates.end()) ? it->second.attributes : 0;
}
void CDaoTableDef::SetAttributes(long lAttributes) {
    std::lock_guard<std::mutex> lock(g_daoStateMutex);
    g_tableDefStates[this].attributes = lAttributes;
}
BOOL CDaoTableDef::CanUpdate() const { return TRUE; }

void CDaoTableDef::CreateField(const wchar_t* lpszName, short nType, long lSize,
                                long lAttributes) {
    (void)lpszName; (void)nType; (void)lSize; (void)lAttributes;
}

void CDaoTableDef::CreateField(CDaoFieldInfo& fieldinfo) { (void)fieldinfo; }
void CDaoTableDef::DeleteField(const wchar_t* lpszName) { (void)lpszName; }
void CDaoTableDef::DeleteField(int nIndex) { (void)nIndex; }
int CDaoTableDef::GetFieldCount() const { return 0; }

void CDaoTableDef::GetFieldInfo(int nIndex, CDaoFieldInfo& fieldinfo,
                                 DWORD dwInfoOptions) {
    (void)nIndex; (void)fieldinfo; (void)dwInfoOptions;
}

void CDaoTableDef::GetFieldInfo(const wchar_t* lpszName, CDaoFieldInfo& fieldinfo,
                                 DWORD dwInfoOptions) {
    (void)lpszName; (void)fieldinfo; (void)dwInfoOptions;
}

void CDaoTableDef::CreateIndex(CDaoIndexInfo& indexinfo) { (void)indexinfo; }
void CDaoTableDef::DeleteIndex(const wchar_t* lpszName) { (void)lpszName; }
void CDaoTableDef::DeleteIndex(int nIndex) { (void)nIndex; }
int CDaoTableDef::GetIndexCount() const { return 0; }

void CDaoTableDef::GetIndexInfo(int nIndex, CDaoIndexInfo& indexinfo,
                                 DWORD dwInfoOptions) {
    (void)nIndex; (void)indexinfo; (void)dwInfoOptions;
}

void CDaoTableDef::GetIndexInfo(const wchar_t* lpszName, CDaoIndexInfo& indexinfo,
                                 DWORD dwInfoOptions) {
    (void)lpszName; (void)indexinfo; (void)dwInfoOptions;
}

CString CDaoTableDef::GetValidationRule() const { return CString(); }
CString CDaoTableDef::GetValidationText() const { return CString(); }
void CDaoTableDef::RefreshLink() {}

//=============================================================================
// CDaoQueryDef
//=============================================================================
IMPLEMENT_DYNAMIC(CDaoQueryDef, CObject)

CDaoQueryDef::CDaoQueryDef(CDaoDatabase* pDatabase)
    : m_pDatabase(pDatabase), m_pDAOQueryDef(nullptr), m_bOpen(FALSE) {
    memset(_daoquerydef_padding, 0, sizeof(_daoquerydef_padding));
    std::lock_guard<std::mutex> lock(g_daoStateMutex);
    g_queryDefStates[this] = DaoQueryDefState{};
}

CDaoQueryDef::~CDaoQueryDef() {
    Close();
    std::lock_guard<std::mutex> lock(g_daoStateMutex);
    g_queryDefStates.erase(this);
}

void CDaoQueryDef::Open(const wchar_t* lpszName) {
    std::lock_guard<std::mutex> lock(g_daoStateMutex);
    if (lpszName) g_queryDefStates[this].name = CString(lpszName);
    m_bOpen = TRUE;
}

void CDaoQueryDef::Create(const wchar_t* lpszName, const wchar_t* lpszSQL) {
    if (lpszName) SetName(lpszName);
    if (lpszSQL) m_strSQL = lpszSQL;
    m_bOpen = TRUE;
}

void CDaoQueryDef::Close() {
    m_bOpen = FALSE;
    m_pDAOQueryDef = nullptr;
}

BOOL CDaoQueryDef::IsOpen() const { return m_bOpen; }
CString CDaoQueryDef::GetName() const {
    std::lock_guard<std::mutex> lock(g_daoStateMutex);
    auto it = g_queryDefStates.find(this);
    return (it != g_queryDefStates.end()) ? it->second.name : CString();
}
void CDaoQueryDef::SetName(const wchar_t* lpszName) {
    std::lock_guard<std::mutex> lock(g_daoStateMutex);
    g_queryDefStates[this].name = lpszName ? CString(lpszName) : CString();
}
CString CDaoQueryDef::GetSQL() const { return m_strSQL; }
void CDaoQueryDef::SetSQL(const wchar_t* lpszSQL) { if (lpszSQL) m_strSQL = lpszSQL; }
BOOL CDaoQueryDef::CanUpdate() const { return TRUE; }
long CDaoQueryDef::GetType() const {
    std::lock_guard<std::mutex> lock(g_daoStateMutex);
    auto it = g_queryDefStates.find(this);
    return (it != g_queryDefStates.end()) ? it->second.type : 0;
}
CString CDaoQueryDef::GetConnect() const {
    std::lock_guard<std::mutex> lock(g_daoStateMutex);
    auto it = g_queryDefStates.find(this);
    return (it != g_queryDefStates.end()) ? it->second.connect : CString();
}
void CDaoQueryDef::SetConnect(const wchar_t* lpszConnect) {
    std::lock_guard<std::mutex> lock(g_daoStateMutex);
    g_queryDefStates[this].connect = lpszConnect ? CString(lpszConnect) : CString();
}
long CDaoQueryDef::GetODBCTimeout() const {
    std::lock_guard<std::mutex> lock(g_daoStateMutex);
    auto it = g_queryDefStates.find(this);
    return (it != g_queryDefStates.end()) ? it->second.timeout : 60;
}
void CDaoQueryDef::SetODBCTimeout(long lODBCTimeout) {
    std::lock_guard<std::mutex> lock(g_daoStateMutex);
    g_queryDefStates[this].timeout = lODBCTimeout;
}
long CDaoQueryDef::GetRecordsAffected() const {
    std::lock_guard<std::mutex> lock(g_daoStateMutex);
    auto it = g_queryDefStates.find(this);
    return (it != g_queryDefStates.end()) ? it->second.recordsAffected : 0;
}
BOOL CDaoQueryDef::GetReturnsRecords() const {
    std::lock_guard<std::mutex> lock(g_daoStateMutex);
    auto it = g_queryDefStates.find(this);
    return (it != g_queryDefStates.end()) ? it->second.returnsRecords : TRUE;
}
void CDaoQueryDef::SetReturnsRecords(BOOL bReturnsRecords) {
    std::lock_guard<std::mutex> lock(g_daoStateMutex);
    g_queryDefStates[this].returnsRecords = bReturnsRecords;
}

int CDaoQueryDef::GetFieldCount() const { return 0; }

void CDaoQueryDef::GetFieldInfo(int nIndex, CDaoFieldInfo& fieldinfo,
                                 DWORD dwInfoOptions) {
    (void)nIndex; (void)fieldinfo; (void)dwInfoOptions;
}

int CDaoQueryDef::GetParameterCount() const { return 0; }

void CDaoQueryDef::GetParameterInfo(int nIndex, CDaoParameterInfo& paraminfo,
                                     DWORD dwInfoOptions) {
    (void)nIndex; (void)paraminfo; (void)dwInfoOptions;
}

void CDaoQueryDef::SetParamValue(int nIndex, const COleVariant& varValue) {
    (void)nIndex; (void)varValue;
}

void CDaoQueryDef::SetParamValue(const wchar_t* lpszName, const COleVariant& varValue) {
    (void)lpszName; (void)varValue;
}

void CDaoQueryDef::Execute(int nOptions) { (void)nOptions; }

//=============================================================================
// CDaoFieldExchange
//=============================================================================
CDaoFieldExchange::CDaoFieldExchange(UINT nOperation, CDaoRecordset* pRecordset,
                                     void* pvField)
    : m_nOperation(nOperation), m_pRecordset(pRecordset), m_pvField(pvField),
      m_pDatabase(nullptr), m_pWorkspace(nullptr), m_pRecordsetDAO(nullptr) {
    memset(_daofieldexchange_padding, 0, sizeof(_daofieldexchange_padding));
}

BOOL CDaoFieldExchange::IsValidOperation() {
    return TRUE;
}

//=============================================================================
// CDaoRecordView
//=============================================================================
IMPLEMENT_DYNAMIC(CDaoRecordView, CFormView)

CDaoRecordView::CDaoRecordView(UINT nIDTemplate)
    : CFormView(nIDTemplate), m_pSet(nullptr),
      m_bOnFirstRecord(TRUE), m_bOnLastRecord(TRUE) {
    memset(_daorecordview_padding, 0, sizeof(_daorecordview_padding));
}

CDaoRecordView::CDaoRecordView(const wchar_t* lpszTemplateName)
    : CFormView(lpszTemplateName), m_pSet(nullptr),
      m_bOnFirstRecord(TRUE), m_bOnLastRecord(TRUE) {
    memset(_daorecordview_padding, 0, sizeof(_daorecordview_padding));
}

CDaoRecordView::~CDaoRecordView() {
    if (m_pSet) delete m_pSet;
}

CDaoRecordset* CDaoRecordView::OnGetRecordset() {
    return m_pSet;
}

BOOL CDaoRecordView::OnMove(UINT nIDMoveCommand) {
    (void)nIDMoveCommand;
    return FALSE;
}

//=============================================================================
// Global DAO Functions
//=============================================================================
void AFXAPI AfxDaoInit() {}
void AFXAPI AfxDaoTerm() {}

void AFXAPI AfxDaoCheck(DAO_ERR scode, long lAfxDaoError,
                        long* pErrorInfo, CDaoException** ppException) {
    (void)scode; (void)lAfxDaoError; (void)pErrorInfo;
    if (ppException) *ppException = nullptr;
}

// DFX functions
void AFXAPI DFX_Text(CDaoFieldExchange* pFX, const wchar_t* lpszFieldName,
                     CString& value, int nMaxLength, DWORD dwColumnType) {
    (void)pFX; (void)lpszFieldName; (void)value; (void)nMaxLength; (void)dwColumnType;
}

void AFXAPI DFX_Long(CDaoFieldExchange* pFX, const wchar_t* lpszFieldName, long& value) {
    (void)pFX; (void)lpszFieldName; (void)value;
}

void AFXAPI DFX_Short(CDaoFieldExchange* pFX, const wchar_t* lpszFieldName, short& value) {
    (void)pFX; (void)lpszFieldName; (void)value;
}

void AFXAPI DFX_Double(CDaoFieldExchange* pFX, const wchar_t* lpszFieldName, double& value) {
    (void)pFX; (void)lpszFieldName; (void)value;
}

void AFXAPI DFX_Bool(CDaoFieldExchange* pFX, const wchar_t* lpszFieldName, BOOL& value) {
    (void)pFX; (void)lpszFieldName; (void)value;
}

void AFXAPI DFX_Currency(CDaoFieldExchange* pFX, const wchar_t* lpszFieldName,
                         COleCurrency& value) {
    (void)pFX; (void)lpszFieldName; (void)value;
}

void AFXAPI DFX_DateTime(CDaoFieldExchange* pFX, const wchar_t* lpszFieldName,
                         COleDateTime& value) {
    (void)pFX; (void)lpszFieldName; (void)value;
}

void AFXAPI DFX_Binary(CDaoFieldExchange* pFX, const wchar_t* lpszFieldName,
                       CByteArray& value, long nMaxLength) {
    (void)pFX; (void)lpszFieldName; (void)value; (void)nMaxLength;
}

void AFXAPI DFX_LongBinary(CDaoFieldExchange* pFX, const wchar_t* lpszFieldName,
                           CLongBinary& value, DWORD dwBufSize) {
    (void)pFX; (void)lpszFieldName; (void)value; (void)dwBufSize;
}
