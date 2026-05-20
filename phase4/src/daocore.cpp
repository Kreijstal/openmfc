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
    CString name;
    CString userName;
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

struct DaoRecordsetState {
    BOOL bBOF = TRUE;
    BOOL bEOF = TRUE;
    long lAbsolutePosition = -1;  // -1 = BOF/EOF/unknown
    double dPercentPosition = 0.0;
    long lRecordCount = 0;
    CString strCurrentIndex;
    CString strSQL;
};

std::mutex g_daoStateMutex;
std::unordered_map<const CDaoWorkspace*, DaoWorkspaceState> g_workspaceStates;
std::unordered_map<const CDaoDatabase*, DaoDatabaseState> g_databaseStates;
std::unordered_map<const CDaoTableDef*, DaoTableDefState> g_tableDefStates;
std::unordered_map<const CDaoQueryDef*, DaoQueryDefState> g_queryDefStates;
std::unordered_map<const CDaoRecordset*, DaoRecordsetState> g_recordsetStates;
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

void CDaoWorkspace::Open(const wchar_t* lpszName) {
    std::lock_guard<std::mutex> lock(g_daoStateMutex);
    DaoWorkspaceState& state = g_workspaceStates[this];
    if (lpszName && *lpszName)
        state.name = CString(lpszName);
    else
        state.name = CString(L"#Default Workspace#");
    m_strName = state.name;
    m_bOpen = TRUE;
}

CString CDaoWorkspace::GetName() const {
    std::lock_guard<std::mutex> lock(g_daoStateMutex);
    auto it = g_workspaceStates.find(this);
    if (it != g_workspaceStates.end() && !it->second.name.IsEmpty())
        return it->second.name;
    return CString(L"#Default Workspace#");
}

CString CDaoWorkspace::GetUserName() const {
    std::lock_guard<std::mutex> lock(g_daoStateMutex);
    auto it = g_workspaceStates.find(this);
    if (it != g_workspaceStates.end() && !it->second.userName.IsEmpty())
        return it->second.userName;
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
    std::lock_guard<std::mutex> lock(g_daoStateMutex);
    g_recordsetStates[this] = DaoRecordsetState{};
}

CDaoRecordset::~CDaoRecordset() {
    Close();
    std::lock_guard<std::mutex> lock(g_daoStateMutex);
    g_recordsetStates.erase(this);
}

void CDaoRecordset::Open(int nOpenType, const wchar_t* lpszSQL, int nOptions) {
    (void)nOpenType; (void)nOptions;
    std::lock_guard<std::mutex> lock(g_daoStateMutex);
    DaoRecordsetState& state = g_recordsetStates[this];
    if (lpszSQL) {
        m_strSQL = lpszSQL;
        state.strSQL = lpszSQL;
    }
    // Without a real DAO driver, we open to an empty recordset: BOF=EOF=TRUE
    state.bBOF = TRUE;
    state.bEOF = TRUE;
    state.lAbsolutePosition = -1;
    state.dPercentPosition = 0.0;
    state.lRecordCount = 0;
    m_lRecordCount = 0;
    m_bOpen = TRUE;
}

void CDaoRecordset::Open(CDaoTableDef* pTableDef, int nOpenType, int nOptions) {
    (void)pTableDef; (void)nOpenType; (void)nOptions;
    std::lock_guard<std::mutex> lock(g_daoStateMutex);
    DaoRecordsetState& state = g_recordsetStates[this];
    state.bBOF = TRUE;
    state.bEOF = TRUE;
    state.lAbsolutePosition = -1;
    state.dPercentPosition = 0.0;
    state.lRecordCount = 0;
    m_lRecordCount = 0;
    m_bOpen = TRUE;
}

void CDaoRecordset::Open(CDaoQueryDef* pQueryDef, int nOpenType, int nOptions) {
    (void)pQueryDef; (void)nOpenType; (void)nOptions;
    std::lock_guard<std::mutex> lock(g_daoStateMutex);
    DaoRecordsetState& state = g_recordsetStates[this];
    state.bBOF = TRUE;
    state.bEOF = TRUE;
    state.lAbsolutePosition = -1;
    state.dPercentPosition = 0.0;
    state.lRecordCount = 0;
    m_lRecordCount = 0;
    m_bOpen = TRUE;
}

void CDaoRecordset::Close() {
    m_bOpen = FALSE;
    m_pDAORecordset = nullptr;
    std::lock_guard<std::mutex> lock(g_daoStateMutex);
    auto it = g_recordsetStates.find(this);
    if (it != g_recordsetStates.end()) {
        it->second.bBOF = TRUE;
        it->second.bEOF = TRUE;
        it->second.lAbsolutePosition = -1;
        it->second.lRecordCount = 0;
    }
}

BOOL CDaoRecordset::IsOpen() const { return m_bOpen; }

BOOL CDaoRecordset::IsBOF() const {
    std::lock_guard<std::mutex> lock(g_daoStateMutex);
    auto it = g_recordsetStates.find(this);
    return (it != g_recordsetStates.end()) ? it->second.bBOF : TRUE;
}

BOOL CDaoRecordset::IsEOF() const {
    std::lock_guard<std::mutex> lock(g_daoStateMutex);
    auto it = g_recordsetStates.find(this);
    return (it != g_recordsetStates.end()) ? it->second.bEOF : TRUE;
}

BOOL CDaoRecordset::CanUpdate() const { return TRUE; }
BOOL CDaoRecordset::CanAppend() const { return TRUE; }

void CDaoRecordset::DoFieldExchange(CDaoFieldExchange* pFX) {
    (void)pFX;
}

void CDaoRecordset::MoveFirst() {
    std::lock_guard<std::mutex> lock(g_daoStateMutex);
    auto it = g_recordsetStates.find(this);
    if (it != g_recordsetStates.end()) {
        if (it->second.lRecordCount > 0) {
            it->second.bBOF = FALSE;
            it->second.bEOF = FALSE;
            it->second.lAbsolutePosition = 0;
            it->second.dPercentPosition = 0.0;
        }
        // else: empty recordset stays BOF=TRUE, EOF=TRUE
    }
}

void CDaoRecordset::MoveLast() {
    std::lock_guard<std::mutex> lock(g_daoStateMutex);
    auto it = g_recordsetStates.find(this);
    if (it != g_recordsetStates.end()) {
        if (it->second.lRecordCount > 0) {
            it->second.bBOF = FALSE;
            it->second.bEOF = FALSE;
            it->second.lAbsolutePosition = it->second.lRecordCount - 1;
            it->second.dPercentPosition = 100.0;
        }
    }
}

void CDaoRecordset::MoveNext() {
    std::lock_guard<std::mutex> lock(g_daoStateMutex);
    auto it = g_recordsetStates.find(this);
    if (it != g_recordsetStates.end()) {
        DaoRecordsetState& s = it->second;
        if (!s.bEOF) {
            if (s.lRecordCount > 0 && s.lAbsolutePosition >= s.lRecordCount - 1) {
                s.bEOF = TRUE;
                s.lAbsolutePosition = s.lRecordCount;
                s.dPercentPosition = 100.0;
            } else if (s.lAbsolutePosition >= 0) {
                s.lAbsolutePosition++;
                s.dPercentPosition = (s.lRecordCount > 0)
                    ? (double)s.lAbsolutePosition / s.lRecordCount * 100.0
                    : 0.0;
            }
        }
    }
}

void CDaoRecordset::MovePrev() {
    std::lock_guard<std::mutex> lock(g_daoStateMutex);
    auto it = g_recordsetStates.find(this);
    if (it != g_recordsetStates.end()) {
        DaoRecordsetState& s = it->second;
        if (!s.bBOF) {
            if (s.lAbsolutePosition <= 0) {
                s.bBOF = TRUE;
                s.lAbsolutePosition = -1;
                s.dPercentPosition = 0.0;
            } else {
                s.lAbsolutePosition--;
                s.dPercentPosition = (s.lRecordCount > 0)
                    ? (double)s.lAbsolutePosition / s.lRecordCount * 100.0
                    : 0.0;
            }
        }
    }
}

void CDaoRecordset::Move(long lRows) {
    if (lRows == 0) return;
    if (lRows > 0) {
        for (long i = 0; i < lRows; i++) MoveNext();
    } else {
        for (long i = 0; i > lRows; i--) MovePrev();
    }
}

long CDaoRecordset::GetRecordCount() const {
    std::lock_guard<std::mutex> lock(g_daoStateMutex);
    auto it = g_recordsetStates.find(this);
    if (it != g_recordsetStates.end())
        return it->second.lRecordCount;
    return m_lRecordCount;
}

long CDaoRecordset::GetAbsolutePosition() const {
    std::lock_guard<std::mutex> lock(g_daoStateMutex);
    auto it = g_recordsetStates.find(this);
    return (it != g_recordsetStates.end()) ? it->second.lAbsolutePosition : -1;
}

void CDaoRecordset::SetAbsolutePosition(long lPos) {
    std::lock_guard<std::mutex> lock(g_daoStateMutex);
    auto it = g_recordsetStates.find(this);
    if (it == g_recordsetStates.end()) return;
    DaoRecordsetState& s = it->second;
    if (lPos < 0 || s.lRecordCount == 0) {
        s.lAbsolutePosition = -1;
        s.bBOF = TRUE;
        s.bEOF = (s.lRecordCount == 0);
        return;
    }
    if (lPos >= s.lRecordCount) {
        s.lAbsolutePosition = s.lRecordCount;
        s.bEOF = TRUE;
        s.bBOF = FALSE;
        return;
    }
    s.lAbsolutePosition = lPos;
    s.bBOF = FALSE;
    s.bEOF = FALSE;
    s.dPercentPosition = (s.lRecordCount > 0)
        ? (double)lPos / s.lRecordCount * 100.0
        : 0.0;
}

double CDaoRecordset::GetPercentPosition() const {
    std::lock_guard<std::mutex> lock(g_daoStateMutex);
    auto it = g_recordsetStates.find(this);
    return (it != g_recordsetStates.end()) ? it->second.dPercentPosition : 0.0;
}

void CDaoRecordset::SetPercentPosition(double dPosition) {
    std::lock_guard<std::mutex> lock(g_daoStateMutex);
    auto it = g_recordsetStates.find(this);
    if (it == g_recordsetStates.end()) return;
    DaoRecordsetState& s = it->second;
    if (dPosition < 0.0) dPosition = 0.0;
    if (dPosition > 100.0) dPosition = 100.0;
    s.dPercentPosition = dPosition;
    if (s.lRecordCount > 0) {
        long newPos = (long)(dPosition / 100.0 * s.lRecordCount);
        if (newPos < 0) newPos = 0;
        if (newPos >= s.lRecordCount) newPos = s.lRecordCount - 1;
        s.lAbsolutePosition = newPos;
        s.bBOF = FALSE;
        s.bEOF = FALSE;
    }
}

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
    std::lock_guard<std::mutex> lock(g_daoStateMutex);
    auto it = g_recordsetStates.find(this);
    if (lpszIndex) {
        m_strCurrentIndex = lpszIndex;
        if (it != g_recordsetStates.end())
            it->second.strCurrentIndex = lpszIndex;
    }
}

CString CDaoRecordset::GetCurrentIndex() const {
    std::lock_guard<std::mutex> lock(g_daoStateMutex);
    auto it = g_recordsetStates.find(this);
    if (it != g_recordsetStates.end() && !it->second.strCurrentIndex.IsEmpty())
        return it->second.strCurrentIndex;
    return m_strCurrentIndex;
}

CString CDaoRecordset::GetSQL() const {
    std::lock_guard<std::mutex> lock(g_daoStateMutex);
    auto it = g_recordsetStates.find(this);
    if (it != g_recordsetStates.end() && !it->second.strSQL.IsEmpty())
        return it->second.strSQL;
    return m_strSQL;
}

void CDaoRecordset::SetSQL(const wchar_t* lpszSQL) {
    if (!lpszSQL) return;
    m_strSQL = lpszSQL;
    std::lock_guard<std::mutex> lock(g_daoStateMutex);
    g_recordsetStates[this].strSQL = lpszSQL;
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

//=============================================================================
// MS_ABI wrappers for DAO classes
// These extern "C" functions use MS ABI so MSVC-compiled code can call them.
// The build_phase4.sh DEF section maps MSVC-mangled names to these wrappers.
//=============================================================================

// --- CDaoWorkspace ---
extern "C" void* MS_ABI dao_CDaoWorkspace_ctor(void* pThis) {
    return new(pThis) CDaoWorkspace();
}
extern "C" void MS_ABI dao_CDaoWorkspace_dtor(CDaoWorkspace* pThis) {
    pThis->~CDaoWorkspace();
}
extern "C" void MS_ABI dao_CDaoWorkspace_Open(CDaoWorkspace* pThis, const wchar_t* lpszName) {
    pThis->Open(lpszName);
}
extern "C" void MS_ABI dao_CDaoWorkspace_Close(CDaoWorkspace* pThis) {
    pThis->Close();
}
extern "C" int MS_ABI dao_CDaoWorkspace_IsOpen(const CDaoWorkspace* pThis) {
    return pThis->IsOpen();
}
extern "C" int MS_ABI dao_CDaoWorkspace_GetInTransaction(const CDaoWorkspace* pThis) {
    return pThis->GetInTransaction();
}
extern "C" void MS_ABI dao_CDaoWorkspace_BeginTrans(CDaoWorkspace* pThis) {
    pThis->BeginTrans();
}
extern "C" void MS_ABI dao_CDaoWorkspace_CommitTrans(CDaoWorkspace* pThis) {
    pThis->CommitTrans();
}
extern "C" void MS_ABI dao_CDaoWorkspace_Rollback(CDaoWorkspace* pThis) {
    pThis->Rollback();
}
extern "C" CRuntimeClass* MS_ABI dao_CDaoWorkspace_GetRuntimeClass(const CDaoWorkspace* pThis) {
    (void)pThis;
    return CDaoWorkspace::GetThisClass();
}
extern "C" CRuntimeClass* MS_ABI dao_CDaoWorkspace_GetThisClass() {
    return CDaoWorkspace::GetThisClass();
}

// --- CDaoDatabase ---
extern "C" void* MS_ABI dao_CDaoDatabase_ctor(void* pThis, CDaoWorkspace* pWS) {
    return new(pThis) CDaoDatabase(pWS);
}
extern "C" void MS_ABI dao_CDaoDatabase_dtor(CDaoDatabase* pThis) {
    pThis->~CDaoDatabase();
}
extern "C" void MS_ABI dao_CDaoDatabase_Open(CDaoDatabase* pThis, const wchar_t* lpszName,
                                              int bExclusive, int bReadOnly, const wchar_t* lpszConnect) {
    pThis->Open(lpszName, bExclusive, bReadOnly, lpszConnect);
}
extern "C" void MS_ABI dao_CDaoDatabase_Close(CDaoDatabase* pThis) {
    pThis->Close();
}
extern "C" int MS_ABI dao_CDaoDatabase_IsOpen(const CDaoDatabase* pThis) {
    return pThis->IsOpen();
}
extern "C" int MS_ABI dao_CDaoDatabase_GetInTransaction(const CDaoDatabase* pThis) {
    return pThis->GetInTransaction();
}
extern "C" void MS_ABI dao_CDaoDatabase_BeginTrans(CDaoDatabase* pThis) {
    pThis->BeginTrans();
}
extern "C" void MS_ABI dao_CDaoDatabase_CommitTrans(CDaoDatabase* pThis) {
    pThis->CommitTrans();
}
extern "C" void MS_ABI dao_CDaoDatabase_Rollback(CDaoDatabase* pThis) {
    pThis->Rollback();
}
extern "C" long MS_ABI dao_CDaoDatabase_GetRecordsAffected(const CDaoDatabase* pThis) {
    return pThis->GetRecordsAffected();
}
extern "C" int MS_ABI dao_CDaoDatabase_CanUpdate(const CDaoDatabase* pThis) {
    return pThis->CanUpdate();
}
extern "C" int MS_ABI dao_CDaoDatabase_CanTransact(const CDaoDatabase* pThis) {
    return pThis->CanTransact();
}
extern "C" CDaoWorkspace* MS_ABI dao_CDaoDatabase_GetWorkspace(CDaoDatabase* pThis) {
    return pThis->GetWorkspace();
}
extern "C" CRuntimeClass* MS_ABI dao_CDaoDatabase_GetRuntimeClass(const CDaoDatabase* pThis) {
    (void)pThis;
    return CDaoDatabase::GetThisClass();
}
extern "C" CRuntimeClass* MS_ABI dao_CDaoDatabase_GetThisClass() {
    return CDaoDatabase::GetThisClass();
}

// --- CDaoRecordset ---
extern "C" void* MS_ABI dao_CDaoRecordset_ctor(void* pThis, CDaoDatabase* pDB) {
    return new(pThis) CDaoRecordset(pDB);
}
extern "C" void MS_ABI dao_CDaoRecordset_dtor(CDaoRecordset* pThis) {
    pThis->~CDaoRecordset();
}
extern "C" void MS_ABI dao_CDaoRecordset_OpenSQL(CDaoRecordset* pThis, int nOpenType,
                                                  const wchar_t* lpszSQL, int nOptions) {
    pThis->Open(nOpenType, lpszSQL, nOptions);
}
extern "C" void MS_ABI dao_CDaoRecordset_Close(CDaoRecordset* pThis) {
    pThis->Close();
}
extern "C" int MS_ABI dao_CDaoRecordset_IsOpen(const CDaoRecordset* pThis) {
    return pThis->IsOpen();
}
extern "C" int MS_ABI dao_CDaoRecordset_IsBOF(const CDaoRecordset* pThis) {
    return pThis->IsBOF();
}
extern "C" int MS_ABI dao_CDaoRecordset_IsEOF(const CDaoRecordset* pThis) {
    return pThis->IsEOF();
}
extern "C" long MS_ABI dao_CDaoRecordset_GetRecordCount(const CDaoRecordset* pThis) {
    return pThis->GetRecordCount();
}
extern "C" long MS_ABI dao_CDaoRecordset_GetAbsolutePosition(const CDaoRecordset* pThis) {
    return pThis->GetAbsolutePosition();
}
extern "C" void MS_ABI dao_CDaoRecordset_SetAbsolutePosition(CDaoRecordset* pThis, long lPos) {
    pThis->SetAbsolutePosition(lPos);
}
extern "C" double MS_ABI dao_CDaoRecordset_GetPercentPosition(const CDaoRecordset* pThis) {
    return pThis->GetPercentPosition();
}
extern "C" void MS_ABI dao_CDaoRecordset_SetPercentPosition(CDaoRecordset* pThis, double d) {
    pThis->SetPercentPosition(d);
}
extern "C" void MS_ABI dao_CDaoRecordset_MoveFirst(CDaoRecordset* pThis) {
    pThis->MoveFirst();
}
extern "C" void MS_ABI dao_CDaoRecordset_MoveLast(CDaoRecordset* pThis) {
    pThis->MoveLast();
}
extern "C" void MS_ABI dao_CDaoRecordset_MoveNext(CDaoRecordset* pThis) {
    pThis->MoveNext();
}
extern "C" void MS_ABI dao_CDaoRecordset_MovePrev(CDaoRecordset* pThis) {
    pThis->MovePrev();
}
extern "C" void MS_ABI dao_CDaoRecordset_Move(CDaoRecordset* pThis, long lRows) {
    pThis->Move(lRows);
}
extern "C" CDaoDatabase* MS_ABI dao_CDaoRecordset_GetDatabase(const CDaoRecordset* pThis) {
    return pThis->GetDatabase();
}
extern "C" int MS_ABI dao_CDaoRecordset_CanUpdate(const CDaoRecordset* pThis) {
    return pThis->CanUpdate();
}
extern "C" int MS_ABI dao_CDaoRecordset_CanAppend(const CDaoRecordset* pThis) {
    return pThis->CanAppend();
}
extern "C" CRuntimeClass* MS_ABI dao_CDaoRecordset_GetRuntimeClass(const CDaoRecordset* pThis) {
    (void)pThis;
    return CDaoRecordset::GetThisClass();
}
extern "C" CRuntimeClass* MS_ABI dao_CDaoRecordset_GetThisClass() {
    return CDaoRecordset::GetThisClass();
}

// --- CDaoTableDef ---
extern "C" void* MS_ABI dao_CDaoTableDef_ctor(void* pThis, CDaoDatabase* pDB) {
    return new(pThis) CDaoTableDef(pDB);
}
extern "C" void MS_ABI dao_CDaoTableDef_dtor(CDaoTableDef* pThis) {
    pThis->~CDaoTableDef();
}
extern "C" void MS_ABI dao_CDaoTableDef_Open(CDaoTableDef* pThis, const wchar_t* lpszName) {
    pThis->Open(lpszName);
}
extern "C" void MS_ABI dao_CDaoTableDef_Close(CDaoTableDef* pThis) {
    pThis->Close();
}
extern "C" int MS_ABI dao_CDaoTableDef_IsOpen(const CDaoTableDef* pThis) {
    return pThis->IsOpen();
}
extern "C" CRuntimeClass* MS_ABI dao_CDaoTableDef_GetRuntimeClass(const CDaoTableDef* pThis) {
    (void)pThis;
    return CDaoTableDef::GetThisClass();
}
extern "C" CRuntimeClass* MS_ABI dao_CDaoTableDef_GetThisClass() {
    return CDaoTableDef::GetThisClass();
}

// --- CDaoQueryDef ---
extern "C" void* MS_ABI dao_CDaoQueryDef_ctor(void* pThis, CDaoDatabase* pDB) {
    return new(pThis) CDaoQueryDef(pDB);
}
extern "C" void MS_ABI dao_CDaoQueryDef_dtor(CDaoQueryDef* pThis) {
    pThis->~CDaoQueryDef();
}
extern "C" void MS_ABI dao_CDaoQueryDef_Open(CDaoQueryDef* pThis, const wchar_t* lpszName) {
    pThis->Open(lpszName);
}
extern "C" void MS_ABI dao_CDaoQueryDef_Close(CDaoQueryDef* pThis) {
    pThis->Close();
}
extern "C" int MS_ABI dao_CDaoQueryDef_IsOpen(const CDaoQueryDef* pThis) {
    return pThis->IsOpen();
}
extern "C" int MS_ABI dao_CDaoQueryDef_CanUpdate(const CDaoQueryDef* pThis) {
    return pThis->CanUpdate();
}
extern "C" long MS_ABI dao_CDaoQueryDef_GetType(const CDaoQueryDef* pThis) {
    return pThis->GetType();
}
extern "C" long MS_ABI dao_CDaoQueryDef_GetODBCTimeout(const CDaoQueryDef* pThis) {
    return pThis->GetODBCTimeout();
}
extern "C" void MS_ABI dao_CDaoQueryDef_SetODBCTimeout(CDaoQueryDef* pThis, long lTimeout) {
    pThis->SetODBCTimeout(lTimeout);
}
extern "C" long MS_ABI dao_CDaoQueryDef_GetRecordsAffected(const CDaoQueryDef* pThis) {
    return pThis->GetRecordsAffected();
}
extern "C" int MS_ABI dao_CDaoQueryDef_GetReturnsRecords(const CDaoQueryDef* pThis) {
    return pThis->GetReturnsRecords();
}
extern "C" void MS_ABI dao_CDaoQueryDef_SetReturnsRecords(CDaoQueryDef* pThis, int b) {
    pThis->SetReturnsRecords(b);
}
extern "C" void MS_ABI dao_CDaoQueryDef_Execute(CDaoQueryDef* pThis, int nOptions) {
    pThis->Execute(nOptions);
}
extern "C" CRuntimeClass* MS_ABI dao_CDaoQueryDef_GetRuntimeClass(const CDaoQueryDef* pThis) {
    (void)pThis;
    return CDaoQueryDef::GetThisClass();
}
extern "C" CRuntimeClass* MS_ABI dao_CDaoQueryDef_GetThisClass() {
    return CDaoQueryDef::GetThisClass();
}

// --- CDaoRecordView ---
extern "C" CRuntimeClass* MS_ABI dao_CDaoRecordView_GetRuntimeClass(const CDaoRecordView* pThis) {
    (void)pThis;
    return CDaoRecordView::GetThisClass();
}
extern "C" CRuntimeClass* MS_ABI dao_CDaoRecordView_GetThisClass() {
    return CDaoRecordView::GetThisClass();
}

// --- CDaoException ---
extern "C" CRuntimeClass* MS_ABI dao_CDaoException_GetRuntimeClass(const CDaoException* pThis) {
    (void)pThis;
    return CDaoException::GetThisClass();
}
extern "C" CRuntimeClass* MS_ABI dao_CDaoException_GetThisClass() {
    return CDaoException::GetThisClass();
}

// --- Global DAO functions wrappers ---
extern "C" void MS_ABI dao_AfxDaoInit() {
    AfxDaoInit();
}
extern "C" void MS_ABI dao_AfxDaoTerm() {
    AfxDaoTerm();
}
extern "C" CDaoWorkspace* MS_ABI dao_CDaoWorkspace_GetDefaultWorkspace() {
    return CDaoWorkspace::GetDefaultWorkspace();
}
