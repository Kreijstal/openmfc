// OpenMFC DAO Database Classes Implementation
// Stub implementations for ABI compatibility.
// Real DAO functionality requires the DAO 3.6 runtime (dao360.dll).
//
// All exports use MSVC ABI via extern "C" + .def aliasing.

#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxdao.h"
#include <algorithm>
#include <cstring>
#include <mutex>
#include <unordered_map>
#include <vector>

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
    std::vector<const CDaoDatabase*> databases;
};

struct DaoTableDefState {
    CString name;
    CString connect;
    CString sourceTableName;
    long attributes = 0;
    std::vector<CDaoFieldInfo> fields;
    std::vector<CDaoIndexInfo> indexes;
};

struct DaoDatabaseState {
    CString name;
    CString connect;
    BOOL inTransaction = FALSE;
    std::vector<const CDaoTableDef*> tableDefs;
    std::vector<const CDaoQueryDef*> queryDefs;
    std::vector<DaoTableDefState> tableDefInfos;
    std::vector<CDaoRelationInfo> relations;
};

struct DaoQueryDefState {
    CString name;
    CString sql;
    CString connect;
    long timeout = 60;
    long recordsAffected = 0;
    BOOL returnsRecords = TRUE;
    long type = 0;
    std::vector<CDaoFieldInfo> fields;
    std::vector<CDaoParameterInfo> parameters;
};

struct DaoRecordsetState {
    BOOL bBOF = TRUE;
    BOOL bEOF = TRUE;
    long lAbsolutePosition = -1;  // -1 = BOF/EOF/unknown
    double dPercentPosition = 0.0;
    long lRecordCount = 0;
    CString strCurrentIndex;
    CString strSQL;
    std::vector<CDaoFieldInfo> fields;
    std::vector<CDaoIndexInfo> indexes;
    std::vector<COleVariant> currentFieldValues;
};

std::mutex g_daoStateMutex;
std::unordered_map<const CDaoWorkspace*, DaoWorkspaceState> g_workspaceStates;
std::unordered_map<const CDaoDatabase*, DaoDatabaseState> g_databaseStates;
std::unordered_map<const CDaoTableDef*, DaoTableDefState> g_tableDefStates;
std::unordered_map<const CDaoQueryDef*, DaoQueryDefState> g_queryDefStates;
std::unordered_map<const CDaoRecordset*, DaoRecordsetState> g_recordsetStates;

namespace {

static CString CloneCString(const wchar_t* value) {
    return value ? CString(value) : CString();
}

template <typename T>
static T& EnsureState(std::unordered_map<const void*, T>& map, const void* key) {
    return map[key];
}

static void FreeIndexFieldInfos(CDaoIndexInfo& indexInfo) {
    if (indexInfo.m_pFieldInfos) {
        delete[] indexInfo.m_pFieldInfos;
        indexInfo.m_pFieldInfos = nullptr;
    }
    indexInfo.m_nFields = 0;
}

static void EnsureVariantCopy(COleVariant& target, const COleVariant& source) {
    VariantClear(&target);
    // mingw's oleauto.h declares VariantCopy's source as non-const VARIANTARG*;
    // the call is semantically const (it only reads the source), so cast the
    // constness away. COleVariant publicly derives from VARIANT (=VARIANTARG).
    VariantCopy(&target, const_cast<COleVariant*>(&source));
}

static CDaoTableDef* FindTableDefByName(CDaoDatabase* pDatabase, const wchar_t* lpszName) {
    if (!pDatabase || !lpszName) return nullptr;
    auto itDb = g_databaseStates.find(pDatabase);
    if (itDb == g_databaseStates.end()) return nullptr;
    const CString targetName = CloneCString(lpszName);
    for (const CDaoTableDef* pDef : itDb->second.tableDefs) {
        auto itDef = g_tableDefStates.find(pDef);
        if (itDef != g_tableDefStates.end() && !itDef->second.name.IsEmpty() &&
            itDef->second.name == targetName) {
            return const_cast<CDaoTableDef*>(pDef);
        }
    }
    return nullptr;
}

static CDaoQueryDef* FindQueryDefByName(CDaoDatabase* pDatabase, const wchar_t* lpszName) {
    if (!pDatabase || !lpszName) return nullptr;
    auto itDb = g_databaseStates.find(pDatabase);
    if (itDb == g_databaseStates.end()) return nullptr;
    const CString targetName = CloneCString(lpszName);
    for (const CDaoQueryDef* pDef : itDb->second.queryDefs) {
        auto itQuery = g_queryDefStates.find(pDef);
        if (itQuery != g_queryDefStates.end() &&
            itQuery->second.name == targetName) {
            return const_cast<CDaoQueryDef*>(pDef);
        }
    }
    return nullptr;
}

static void RegisterDatabaseWithWorkspace(CDaoDatabase* pDatabase, CDaoWorkspace* pWorkspace) {
    if (!pDatabase || !pWorkspace) return;
    auto wsIt = g_workspaceStates.find(pWorkspace);
    if (wsIt == g_workspaceStates.end()) return;
    auto& dbs = wsIt->second.databases;
    if (std::find(dbs.begin(), dbs.end(), pDatabase) == dbs.end()) {
        dbs.push_back(pDatabase);
    }
}

static void UnregisterDatabaseFromWorkspace(CDaoDatabase* pDatabase, CDaoWorkspace* pWorkspace) {
    if (!pDatabase || !pWorkspace) return;
    auto wsIt = g_workspaceStates.find(pWorkspace);
    if (wsIt == g_workspaceStates.end()) return;
    auto& dbs = wsIt->second.databases;
    dbs.erase(std::remove(dbs.begin(), dbs.end(), pDatabase), dbs.end());
}

static void RegisterTableDef(CDaoTableDef* pTableDef, CDaoDatabase* pDatabase) {
    if (!pTableDef || !pDatabase) return;
    auto dbIt = g_databaseStates.find(pDatabase);
    if (dbIt == g_databaseStates.end()) return;
    auto& tableDefs = dbIt->second.tableDefs;
    if (std::find(tableDefs.begin(), tableDefs.end(), pTableDef) == tableDefs.end()) {
        tableDefs.push_back(pTableDef);
    }
}

static void RegisterQueryDef(CDaoQueryDef* pQueryDef, CDaoDatabase* pDatabase) {
    if (!pQueryDef || !pDatabase) return;
    auto dbIt = g_databaseStates.find(pDatabase);
    if (dbIt == g_databaseStates.end()) return;
    auto& queryDefs = dbIt->second.queryDefs;
    if (std::find(queryDefs.begin(), queryDefs.end(), pQueryDef) == queryDefs.end()) {
        queryDefs.push_back(pQueryDef);
    }
}

static void UnregisterTableDef(CDaoTableDef* pTableDef, CDaoDatabase* pDatabase) {
    if (!pTableDef || !pDatabase) return;
    auto dbIt = g_databaseStates.find(pDatabase);
    if (dbIt == g_databaseStates.end()) return;
    auto& tableDefs = dbIt->second.tableDefs;
    tableDefs.erase(std::remove(tableDefs.begin(), tableDefs.end(), pTableDef), tableDefs.end());
}

static void UnregisterQueryDef(CDaoQueryDef* pQueryDef, CDaoDatabase* pDatabase) {
    if (!pQueryDef || !pDatabase) return;
    auto dbIt = g_databaseStates.find(pDatabase);
    if (dbIt == g_databaseStates.end()) return;
    auto& queryDefs = dbIt->second.queryDefs;
    queryDefs.erase(std::remove(queryDefs.begin(), queryDefs.end(), pQueryDef), queryDefs.end());
}

static void CopyFieldInfo(CDaoFieldInfo& dst, const CDaoFieldInfo& src) {
    dst = src;
}

static void CopyIndexInfo(CDaoIndexInfo& dst, const CDaoIndexInfo& src) {
    if (&dst == &src) return;
    if (dst.m_pFieldInfos) {
        delete[] dst.m_pFieldInfos;
    }
    dst.m_strName = src.m_strName;
    dst.m_nFields = src.m_nFields;
    dst.m_bPrimary = src.m_bPrimary;
    dst.m_bUnique = src.m_bUnique;
    dst.m_bClustered = src.m_bClustered;
    dst.m_bIgnoreNulls = src.m_bIgnoreNulls;
    dst.m_bRequired = src.m_bRequired;
    dst.m_bForeign = src.m_bForeign;
    dst.m_lDistinctCount = src.m_lDistinctCount;
    if (src.m_nFields > 0 && src.m_pFieldInfos) {
        dst.m_pFieldInfos = new CDaoIndexFieldInfo[static_cast<size_t>(src.m_nFields)];
        for (int i = 0; i < src.m_nFields; ++i) {
            dst.m_pFieldInfos[i] = src.m_pFieldInfos[i];
        }
    } else {
        dst.m_pFieldInfos = nullptr;
        dst.m_nFields = 0;
    }
}

static void ApplyTableDefInfoFromState(CDaoTableDefInfo& dst, const DaoTableDefState& state) {
    dst.m_strName = state.name;
    dst.m_lAttributes = state.attributes;
    dst.m_strSourceTable = state.sourceTableName;
    dst.m_strConnect = state.connect;
    dst.m_lRecordCount = 0;
    dst.m_bUpdatable = TRUE;
    dst.m_strValidationRule = CString();
    dst.m_strValidationText = CString();
}

static void ClearTableDefState(DaoTableDefState& state) {
    for (auto& indexInfo : state.indexes) {
        FreeIndexFieldInfos(indexInfo);
    }
    state.indexes.clear();
    state.fields.clear();
}

static void ClearRecordsetState(DaoRecordsetState& state) {
    for (auto& indexInfo : state.indexes) {
        FreeIndexFieldInfos(indexInfo);
    }
    state.indexes.clear();
    state.fields.clear();
    state.currentFieldValues.clear();
}

static std::vector<CDaoIndexInfo> CloneIndexInfoVector(const std::vector<CDaoIndexInfo>& source) {
    std::vector<CDaoIndexInfo> copied;
    copied.reserve(source.size());
    for (const auto& index : source) {
        CDaoIndexInfo clone{};
        CopyIndexInfo(clone, index);
        copied.push_back(clone);
    }
    return copied;
}

} // namespace (dao helper functions)
} // namespace (dao internal state)

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
    if (pDatabase) {
        RegisterDatabaseWithWorkspace(pDatabase, this);
    }
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
    std::lock_guard<std::mutex> lock(g_daoStateMutex);
    auto it = g_workspaceStates.find(this);
    if (it == g_workspaceStates.end()) return nullptr;
    const auto& dbs = it->second.databases;
    if (nIndex < 0 || static_cast<size_t>(nIndex) >= dbs.size()) return nullptr;
    return const_cast<CDaoDatabase*>(dbs[static_cast<size_t>(nIndex)]);
}

int CDaoWorkspace::GetDatabaseCount() const {
    std::lock_guard<std::mutex> lock(g_daoStateMutex);
    auto it = g_workspaceStates.find(this);
    return (it != g_workspaceStates.end()) ? static_cast<int>(it->second.databases.size()) : 0;
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
    RegisterDatabaseWithWorkspace(this, m_pWorkspace);
}

CDaoDatabase::~CDaoDatabase() {
    Close();
    UnregisterDatabaseFromWorkspace(this, m_pWorkspace);
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
    std::lock_guard<std::mutex> lock(g_daoStateMutex);
    auto it = g_databaseStates.find(this);
    if (it == g_databaseStates.end()) return 0;
    return static_cast<int>(it->second.tableDefs.size() + it->second.tableDefInfos.size());
}

void CDaoDatabase::GetTableDefInfo(int nIndex, CDaoTableDefInfo& tabledefinfo,
                                    DWORD dwInfoOptions) {
    (void)dwInfoOptions;
    tabledefinfo = CDaoTableDefInfo{};
    std::lock_guard<std::mutex> lock(g_daoStateMutex);
    auto it = g_databaseStates.find(this);
    if (it == g_databaseStates.end()) return;

    if (nIndex < 0) return;

    int tableDefIndex = 0;
    for (const CDaoTableDef* pDef : it->second.tableDefs) {
        if (nIndex == tableDefIndex) {
            auto itDef = g_tableDefStates.find(pDef);
            if (itDef != g_tableDefStates.end()) {
                ApplyTableDefInfoFromState(tabledefinfo, itDef->second);
            }
            return;
        }
        ++tableDefIndex;
    }

    size_t infoIndex = static_cast<size_t>(nIndex) - it->second.tableDefs.size();
    if (infoIndex >= it->second.tableDefInfos.size()) return;
    const DaoTableDefState& infoState = it->second.tableDefInfos[infoIndex];
    tabledefinfo.m_strName = infoState.name;
    tabledefinfo.m_lAttributes = infoState.attributes;
    tabledefinfo.m_strSourceTable = infoState.sourceTableName;
    tabledefinfo.m_strConnect = infoState.connect;
    tabledefinfo.m_lRecordCount = 0;
    tabledefinfo.m_bUpdatable = TRUE;
    tabledefinfo.m_strValidationRule = CString();
    tabledefinfo.m_strValidationText = CString();
}

void CDaoDatabase::CreateTableDef(const wchar_t* lpszName, long lAttributes,
                                   long lOptions, const wchar_t* lpszSource,
                                   const wchar_t* lpszConnect) {
    (void)lOptions;
    if (!lpszName || !*lpszName) return;
    std::lock_guard<std::mutex> lock(g_daoStateMutex);
    auto& dbState = g_databaseStates[this];
    for (auto& info : dbState.tableDefInfos) {
        if (info.name == lpszName) {
            info.attributes = lAttributes;
            info.sourceTableName = lpszSource ? CString(lpszSource) : CString();
            info.connect = lpszConnect ? CString(lpszConnect) : CString();
            return;
        }
    }
    DaoTableDefState info;
    info.name = CString(lpszName);
    info.attributes = lAttributes;
    info.sourceTableName = lpszSource ? CString(lpszSource) : CString();
    info.connect = lpszConnect ? CString(lpszConnect) : CString();
    dbState.tableDefInfos.push_back(info);
}

void CDaoDatabase::DeleteTableDef(const wchar_t* lpszName) {
    if (!lpszName) return;
    std::lock_guard<std::mutex> lock(g_daoStateMutex);
    auto it = g_databaseStates.find(this);
    if (it == g_databaseStates.end()) return;

    for (auto tableDefIt = it->second.tableDefs.begin();
         tableDefIt != it->second.tableDefs.end(); ++tableDefIt) {
        auto stateIt = g_tableDefStates.find(*tableDefIt);
        if (stateIt != g_tableDefStates.end() && stateIt->second.name == CString(lpszName)) {
            if (const_cast<CDaoTableDef*>(*tableDefIt)) {
                CDaoTableDef* pTableDef = const_cast<CDaoTableDef*>(*tableDefIt);
                pTableDef->Close();
            }
            it->second.tableDefs.erase(tableDefIt);
            break;
        }
    }

    auto& infos = it->second.tableDefInfos;
    infos.erase(std::remove_if(infos.begin(), infos.end(),
                               [&](const DaoTableDefState& info) {
                                   return info.name == CString(lpszName);
                               }),
                infos.end());
}

CDaoQueryDef* CDaoDatabase::CreateQueryDef(const wchar_t* lpszName,
                                            const wchar_t* lpszSQL) {
    CDaoQueryDef* pQD = new CDaoQueryDef(this);
    pQD->m_bOpen = TRUE;
    if (lpszName) pQD->SetName(lpszName);
    if (lpszSQL) pQD->SetSQL(lpszSQL);
    {
        std::lock_guard<std::mutex> lock(g_daoStateMutex);
        RegisterQueryDef(pQD, this);
        if (lpszSQL) g_queryDefStates[pQD].sql = CString(lpszSQL);
    }
    return pQD;
}

void CDaoDatabase::DeleteQueryDef(const wchar_t* lpszName) {
    if (!lpszName) return;
    CDaoQueryDef* pFound = FindQueryDefByName(this, lpszName);
    std::lock_guard<std::mutex> lock(g_daoStateMutex);
    if (!pFound) return;
    pFound->Close();
    UnregisterQueryDef(pFound, this);
    g_queryDefStates.erase(pFound);
    delete pFound;
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
    CDaoRelationInfo relinfo;
    relinfo.m_strName = lpszName ? CString(lpszName) : CString();
    relinfo.m_strTable = lpszTable ? CString(lpszTable) : CString();
    relinfo.m_strForeignTable = lpszForeignTable ? CString(lpszForeignTable) : CString();
    relinfo.m_lAttributes = lAttributes;
    relinfo.m_strField = lpszField ? CString(lpszField) : CString();
    relinfo.m_strForeignField = lpszForeignField ? CString(lpszForeignField) : CString();
    CreateRelation(relinfo);
}

void CDaoDatabase::CreateRelation(CDaoRelationInfo& relinfo) {
    if (relinfo.m_strName.IsEmpty()) return;
    std::lock_guard<std::mutex> lock(g_daoStateMutex);
    auto& state = g_databaseStates[this];
    state.relations.push_back(relinfo);
}

void CDaoDatabase::DeleteRelation(const wchar_t* lpszName) {
    if (!lpszName) return;
    std::lock_guard<std::mutex> lock(g_daoStateMutex);
    auto it = g_databaseStates.find(this);
    if (it == g_databaseStates.end()) return;
    auto& rels = it->second.relations;
    rels.erase(std::remove_if(rels.begin(), rels.end(),
                              [&](const CDaoRelationInfo& rel) { return rel.m_strName == lpszName; }),
              rels.end());
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
    auto it = g_recordsetStates.find(this);
    if (it != g_recordsetStates.end()) {
        ClearRecordsetState(it->second);
    }
    g_recordsetStates.erase(this);
}

void CDaoRecordset::Open(int nOpenType, const wchar_t* lpszSQL, int nOptions) {
    (void)nOpenType; (void)nOptions;
    std::lock_guard<std::mutex> lock(g_daoStateMutex);
    DaoRecordsetState& state = g_recordsetStates[this];
    ClearRecordsetState(state);
    if (lpszSQL) {
        m_strSQL = lpszSQL;
        state.strSQL = lpszSQL;
    } else {
        m_strSQL = CString();
        state.strSQL = CString();
    }
    // Without a real DAO driver, we open to an empty recordset: BOF=EOF=TRUE
    state.bBOF = TRUE;
    state.bEOF = TRUE;
    state.lAbsolutePosition = -1;
    state.dPercentPosition = 0.0;
    state.lRecordCount = 0;
    m_lRecordCount = state.lRecordCount;
    m_nFields = 0;
    m_bOpen = TRUE;
}

void CDaoRecordset::Open(CDaoTableDef* pTableDef, int nOpenType, int nOptions) {
    (void)nOpenType; (void)nOptions;
    std::lock_guard<std::mutex> lock(g_daoStateMutex);
    DaoRecordsetState& state = g_recordsetStates[this];
    ClearRecordsetState(state);
    if (pTableDef) {
        auto it = g_tableDefStates.find(pTableDef);
        if (it != g_tableDefStates.end()) {
            state.fields = it->second.fields;
            state.indexes = CloneIndexInfoVector(it->second.indexes);
            state.strSQL = CString(L"SELECT * FROM ") + it->second.name;
            m_strSQL = state.strSQL;
        }
    } else {
        state.strSQL = CString();
        m_strSQL = CString();
    }
    m_nFields = static_cast<int>(state.fields.size());
    m_lRecordCount = 0;
    state.bBOF = TRUE;
    state.bEOF = TRUE;
    state.lAbsolutePosition = -1;
    state.dPercentPosition = 0.0;
    state.lRecordCount = m_lRecordCount;
    m_bOpen = TRUE;
}

void CDaoRecordset::Open(CDaoQueryDef* pQueryDef, int nOpenType, int nOptions) {
    (void)nOpenType; (void)nOptions;
    std::lock_guard<std::mutex> lock(g_daoStateMutex);
    DaoRecordsetState& state = g_recordsetStates[this];
    ClearRecordsetState(state);
    if (pQueryDef) {
        auto queryStateIt = g_queryDefStates.find(pQueryDef);
        if (queryStateIt != g_queryDefStates.end()) {
            state.fields = queryStateIt->second.fields;
            state.strSQL = queryStateIt->second.sql;
            if (state.strSQL.IsEmpty()) {
                state.strSQL = pQueryDef->m_strSQL;
            }
            m_strSQL = state.strSQL;
        }
    } else {
        state.strSQL = CString();
        m_strSQL = CString();
    }
    m_nFields = static_cast<int>(state.fields.size());
    m_lRecordCount = 0;
    state.bBOF = TRUE;
    state.bEOF = TRUE;
    state.lAbsolutePosition = -1;
    state.dPercentPosition = 0.0;
    state.lRecordCount = 0;
    m_bOpen = TRUE;
}

void CDaoRecordset::Close() {
    m_bOpen = FALSE;
    m_pDAORecordset = nullptr;
    std::lock_guard<std::mutex> lock(g_daoStateMutex);
    auto it = g_recordsetStates.find(this);
    if (it != g_recordsetStates.end()) {
        ClearRecordsetState(it->second);
        it->second.bBOF = TRUE;
        it->second.bEOF = TRUE;
        it->second.lAbsolutePosition = -1;
        it->second.lRecordCount = 0;
    }
    m_lRecordCount = 0;
    m_nFields = 0;
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
    if (!lpszName) {
        varValue.Clear();
        return;
    }
    std::lock_guard<std::mutex> lock(g_daoStateMutex);
    const auto it = g_recordsetStates.find(this);
    if (it == g_recordsetStates.end()) {
        varValue.Clear();
        return;
    }
    const auto& fields = it->second.fields;
    for (size_t i = 0; i < fields.size(); ++i) {
        if (fields[i].m_strName == lpszName) {
            if (i < it->second.currentFieldValues.size()) {
                EnsureVariantCopy(varValue, it->second.currentFieldValues[i]);
            } else {
                varValue.Clear();
            }
            return;
        }
    }
    varValue.Clear();
}

void CDaoRecordset::GetFieldValue(int nIndex, COleVariant& varValue) {
    if (nIndex < 0) {
        varValue.Clear();
        return;
    }
    std::lock_guard<std::mutex> lock(g_daoStateMutex);
    auto it = g_recordsetStates.find(this);
    if (it == g_recordsetStates.end() || nIndex >= static_cast<int>(it->second.currentFieldValues.size())) {
        varValue.Clear();
        return;
    }
    EnsureVariantCopy(varValue, it->second.currentFieldValues[static_cast<size_t>(nIndex)]);
}

void CDaoRecordset::SetFieldValue(const wchar_t* lpszName, const COleVariant& varValue) {
    if (!lpszName) return;
    std::lock_guard<std::mutex> lock(g_daoStateMutex);
    auto it = g_recordsetStates.find(this);
    if (it == g_recordsetStates.end()) return;
    for (size_t i = 0; i < it->second.fields.size(); ++i) {
        if (it->second.fields[i].m_strName == lpszName) {
            if (it->second.currentFieldValues.size() <= i) {
                it->second.currentFieldValues.resize(it->second.fields.size());
            }
            EnsureVariantCopy(it->second.currentFieldValues[i], varValue);
            return;
        }
    }
}

void CDaoRecordset::SetFieldValue(int nIndex, const COleVariant& varValue) {
    if (nIndex < 0) return;
    std::lock_guard<std::mutex> lock(g_daoStateMutex);
    auto it = g_recordsetStates.find(this);
    if (it == g_recordsetStates.end()) return;
    if (it->second.currentFieldValues.size() <= static_cast<size_t>(nIndex)) {
        it->second.currentFieldValues.resize(static_cast<size_t>(nIndex) + 1);
    }
    EnsureVariantCopy(it->second.currentFieldValues[static_cast<size_t>(nIndex)], varValue);
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
    std::lock_guard<std::mutex> lock(g_daoStateMutex);
    auto it = g_recordsetStates.find(this);
    return (it != g_recordsetStates.end()) ? static_cast<int>(it->second.fields.size()) : m_nFields;
}

void CDaoRecordset::GetFieldInfo(int nIndex, CDaoFieldInfo& fieldinfo,
                                  DWORD dwInfoOptions) {
    (void)dwInfoOptions;
    fieldinfo = CDaoFieldInfo{};
    std::lock_guard<std::mutex> lock(g_daoStateMutex);
    auto it = g_recordsetStates.find(this);
    if (it == g_recordsetStates.end()) return;
    if (nIndex < 0 || nIndex >= static_cast<int>(it->second.fields.size())) return;
    CopyFieldInfo(fieldinfo, it->second.fields[static_cast<size_t>(nIndex)]);
}

void CDaoRecordset::GetFieldInfo(const wchar_t* lpszName, CDaoFieldInfo& fieldinfo,
                                  DWORD dwInfoOptions) {
    (void)dwInfoOptions;
    fieldinfo = CDaoFieldInfo{};
    if (!lpszName) return;
    std::lock_guard<std::mutex> lock(g_daoStateMutex);
    auto it = g_recordsetStates.find(this);
    if (it == g_recordsetStates.end()) return;
    for (const auto& field : it->second.fields) {
        if (field.m_strName == lpszName) {
            fieldinfo = field;
            return;
        }
    }
}

int CDaoRecordset::GetIndexCount() const {
    std::lock_guard<std::mutex> lock(g_daoStateMutex);
    auto it = g_recordsetStates.find(this);
    return (it != g_recordsetStates.end()) ? static_cast<int>(it->second.indexes.size()) : 0;
}

void CDaoRecordset::GetIndexInfo(int nIndex, CDaoIndexInfo& indexinfo,
                                  DWORD dwInfoOptions) {
    (void)dwInfoOptions;
    indexinfo.m_strName = CString();
    indexinfo.m_nFields = 0;
    indexinfo.m_pFieldInfos = nullptr;
    indexinfo.m_bPrimary = FALSE;
    indexinfo.m_bUnique = FALSE;
    indexinfo.m_bClustered = FALSE;
    indexinfo.m_bIgnoreNulls = FALSE;
    indexinfo.m_bRequired = FALSE;
    indexinfo.m_bForeign = FALSE;
    indexinfo.m_lDistinctCount = 0;
    std::lock_guard<std::mutex> lock(g_daoStateMutex);
    auto it = g_recordsetStates.find(this);
    if (it == g_recordsetStates.end() || nIndex < 0 || nIndex >= static_cast<int>(it->second.indexes.size()))
        return;
    CopyIndexInfo(indexinfo, it->second.indexes[static_cast<size_t>(nIndex)]);
}

void CDaoRecordset::GetIndexInfo(const wchar_t* lpszName, CDaoIndexInfo& indexinfo,
                                  DWORD dwInfoOptions) {
    (void)dwInfoOptions;
    indexinfo = CDaoIndexInfo{};
    indexinfo.m_pFieldInfos = nullptr;
    if (!lpszName) return;
    std::lock_guard<std::mutex> lock(g_daoStateMutex);
    auto it = g_recordsetStates.find(this);
    if (it == g_recordsetStates.end()) return;
    for (const auto& index : it->second.indexes) {
        if (index.m_strName == lpszName) {
            CopyIndexInfo(indexinfo, index);
            return;
        }
    }
}

BOOL CDaoRecordset::Seek(const wchar_t* lpszComparison, COleVariant* pKey1,
                          COleVariant* pKey2, COleVariant* pKey3) {
    (void)lpszComparison; (void)pKey1; (void)pKey2; (void)pKey3;
    if (!IsOpen()) return FALSE;
    if (GetRecordCount() == 0) return FALSE;
    MoveFirst();
    return TRUE;
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
    if (pSize) *pSize = GetRecordCount();
    if (pBookmark) pBookmark->Clear();
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
    RegisterTableDef(this, m_pDatabase);
}

CDaoTableDef::~CDaoTableDef() {
    Close();
    UnregisterTableDef(this, m_pDatabase);
    std::lock_guard<std::mutex> lock(g_daoStateMutex);
    auto it = g_tableDefStates.find(this);
    if (it != g_tableDefStates.end()) {
        ClearTableDefState(it->second);
    }
    g_tableDefStates.erase(this);
}

void CDaoTableDef::Open(const wchar_t* lpszName) {
    std::lock_guard<std::mutex> lock(g_daoStateMutex);
    g_tableDefStates[this].name = lpszName ? CString(lpszName) : CString();
    RegisterTableDef(this, m_pDatabase);
    m_bOpen = TRUE;
}

void CDaoTableDef::Create(const wchar_t* lpszName, long lAttributes,
                          const wchar_t* lpszSrcTable, const wchar_t* lpszConnect) {
    std::lock_guard<std::mutex> lock(g_daoStateMutex);
    DaoTableDefState& state = g_tableDefStates[this];
    RegisterTableDef(this, m_pDatabase);
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
    if (!lpszName) return;
    CDaoFieldInfo fieldinfo;
    fieldinfo.m_strName = lpszName;
    fieldinfo.m_nType = nType;
    fieldinfo.m_lSize = lSize;
    fieldinfo.m_lAttributes = lAttributes;
    CreateField(fieldinfo);
}

void CDaoTableDef::CreateField(CDaoFieldInfo& fieldinfo) {
    std::lock_guard<std::mutex> lock(g_daoStateMutex);
    g_tableDefStates[this].fields.push_back(fieldinfo);
}

void CDaoTableDef::DeleteField(const wchar_t* lpszName) {
    if (!lpszName) return;
    std::lock_guard<std::mutex> lock(g_daoStateMutex);
    auto it = g_tableDefStates.find(this);
    if (it == g_tableDefStates.end()) return;
    auto& fields = it->second.fields;
    fields.erase(std::remove_if(fields.begin(), fields.end(),
                               [&](const CDaoFieldInfo& field) {
                                   return field.m_strName == CString(lpszName);
                               }),
                fields.end());
}

void CDaoTableDef::DeleteField(int nIndex) {
    std::lock_guard<std::mutex> lock(g_daoStateMutex);
    auto it = g_tableDefStates.find(this);
    if (it == g_tableDefStates.end()) return;
    auto& fields = it->second.fields;
    if (nIndex < 0 || static_cast<size_t>(nIndex) >= fields.size()) return;
    fields.erase(fields.begin() + nIndex);
}

int CDaoTableDef::GetFieldCount() const {
    std::lock_guard<std::mutex> lock(g_daoStateMutex);
    auto it = g_tableDefStates.find(this);
    return (it != g_tableDefStates.end()) ? static_cast<int>(it->second.fields.size()) : 0;
}

void CDaoTableDef::GetFieldInfo(int nIndex, CDaoFieldInfo& fieldinfo,
                                 DWORD dwInfoOptions) {
    (void)dwInfoOptions;
    fieldinfo = CDaoFieldInfo{};
    std::lock_guard<std::mutex> lock(g_daoStateMutex);
    auto it = g_tableDefStates.find(this);
    if (it == g_tableDefStates.end()) return;
    if (nIndex < 0 || nIndex >= static_cast<int>(it->second.fields.size())) return;
    CopyFieldInfo(fieldinfo, it->second.fields[static_cast<size_t>(nIndex)]);
}

void CDaoTableDef::GetFieldInfo(const wchar_t* lpszName, CDaoFieldInfo& fieldinfo,
                                 DWORD dwInfoOptions) {
    (void)dwInfoOptions;
    fieldinfo = CDaoFieldInfo{};
    if (!lpszName) return;
    std::lock_guard<std::mutex> lock(g_daoStateMutex);
    auto it = g_tableDefStates.find(this);
    if (it == g_tableDefStates.end()) return;
    for (const auto& field : it->second.fields) {
        if (field.m_strName == lpszName) {
            fieldinfo = field;
            return;
        }
    }
}

void CDaoTableDef::CreateIndex(CDaoIndexInfo& indexinfo) {
    std::lock_guard<std::mutex> lock(g_daoStateMutex);
    auto it = g_tableDefStates.find(this);
    if (it == g_tableDefStates.end()) return;
    CDaoIndexInfo copy = indexinfo;
    copy.m_pFieldInfos = nullptr;
    copy.m_nFields = 0;
    if (indexinfo.m_nFields > 0 && indexinfo.m_pFieldInfos) {
        copy.m_nFields = indexinfo.m_nFields;
        copy.m_pFieldInfos = new CDaoIndexFieldInfo[static_cast<size_t>(indexinfo.m_nFields)];
        for (int i = 0; i < indexinfo.m_nFields; ++i) {
            copy.m_pFieldInfos[i] = indexinfo.m_pFieldInfos[i];
        }
    }
    it->second.indexes.push_back(copy);
}

void CDaoTableDef::DeleteIndex(const wchar_t* lpszName) {
    if (!lpszName) return;
    std::lock_guard<std::mutex> lock(g_daoStateMutex);
    auto it = g_tableDefStates.find(this);
    if (it == g_tableDefStates.end()) return;
    for (auto indexIt = it->second.indexes.begin(); indexIt != it->second.indexes.end(); ++indexIt) {
        if (indexIt->m_strName == lpszName) {
            FreeIndexFieldInfos(*indexIt);
            it->second.indexes.erase(indexIt);
            return;
        }
    }
}

void CDaoTableDef::DeleteIndex(int nIndex) {
    std::lock_guard<std::mutex> lock(g_daoStateMutex);
    auto it = g_tableDefStates.find(this);
    if (it == g_tableDefStates.end()) return;
    auto& indexes = it->second.indexes;
    if (nIndex < 0 || static_cast<size_t>(nIndex) >= indexes.size()) return;
    FreeIndexFieldInfos(indexes[nIndex]);
    indexes.erase(indexes.begin() + nIndex);
}

int CDaoTableDef::GetIndexCount() const {
    std::lock_guard<std::mutex> lock(g_daoStateMutex);
    auto it = g_tableDefStates.find(this);
    return (it != g_tableDefStates.end()) ? static_cast<int>(it->second.indexes.size()) : 0;
}

void CDaoTableDef::GetIndexInfo(int nIndex, CDaoIndexInfo& indexinfo,
                                 DWORD dwInfoOptions) {
    (void)dwInfoOptions;
    indexinfo = CDaoIndexInfo{};
    std::lock_guard<std::mutex> lock(g_daoStateMutex);
    auto it = g_tableDefStates.find(this);
    if (it == g_tableDefStates.end()) return;
    if (nIndex < 0 || nIndex >= static_cast<int>(it->second.indexes.size())) return;
    CopyIndexInfo(indexinfo, it->second.indexes[static_cast<size_t>(nIndex)]);
}

void CDaoTableDef::GetIndexInfo(const wchar_t* lpszName, CDaoIndexInfo& indexinfo,
                                 DWORD dwInfoOptions) {
    (void)dwInfoOptions;
    indexinfo = CDaoIndexInfo{};
    indexinfo.m_pFieldInfos = nullptr;
    if (!lpszName) return;
    std::lock_guard<std::mutex> lock(g_daoStateMutex);
    auto it = g_tableDefStates.find(this);
    if (it == g_tableDefStates.end()) return;
    for (const auto& index : it->second.indexes) {
        if (index.m_strName == lpszName) {
            CopyIndexInfo(indexinfo, index);
            return;
        }
    }
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
    if (m_pDatabase) RegisterQueryDef(this, m_pDatabase);
}

CDaoQueryDef::~CDaoQueryDef() {
    Close();
    UnregisterQueryDef(this, m_pDatabase);
    std::lock_guard<std::mutex> lock(g_daoStateMutex);
    auto it = g_queryDefStates.find(this);
    if (it != g_queryDefStates.end()) {
        it->second.fields.clear();
        it->second.parameters.clear();
    }
    g_queryDefStates.erase(this);
}

void CDaoQueryDef::Open(const wchar_t* lpszName) {
    std::lock_guard<std::mutex> lock(g_daoStateMutex);
    if (lpszName) g_queryDefStates[this].name = CString(lpszName);
    RegisterQueryDef(this, m_pDatabase);
    m_bOpen = TRUE;
}

void CDaoQueryDef::Create(const wchar_t* lpszName, const wchar_t* lpszSQL) {
    if (lpszName) SetName(lpszName);
    if (lpszSQL) SetSQL(lpszSQL);
    m_bOpen = TRUE;
    if (m_pDatabase) RegisterQueryDef(this, m_pDatabase);
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
    RegisterQueryDef(this, m_pDatabase);
}
CString CDaoQueryDef::GetSQL() const {
    std::lock_guard<std::mutex> lock(g_daoStateMutex);
    auto it = g_queryDefStates.find(this);
    if (it != g_queryDefStates.end()) return it->second.sql;
    return m_strSQL;
}

void CDaoQueryDef::SetSQL(const wchar_t* lpszSQL) {
    std::lock_guard<std::mutex> lock(g_daoStateMutex);
    if (lpszSQL) {
        m_strSQL = lpszSQL;
        g_queryDefStates[this].sql = lpszSQL;
    }
}
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

int CDaoQueryDef::GetFieldCount() const {
    std::lock_guard<std::mutex> lock(g_daoStateMutex);
    auto it = g_queryDefStates.find(this);
    return (it != g_queryDefStates.end()) ? static_cast<int>(it->second.fields.size()) : 0;
}

void CDaoQueryDef::GetFieldInfo(int nIndex, CDaoFieldInfo& fieldinfo,
                                 DWORD dwInfoOptions) {
    (void)dwInfoOptions;
    fieldinfo = CDaoFieldInfo{};
    std::lock_guard<std::mutex> lock(g_daoStateMutex);
    auto it = g_queryDefStates.find(this);
    if (it == g_queryDefStates.end()) return;
    if (nIndex < 0 || nIndex >= static_cast<int>(it->second.fields.size())) return;
    CopyFieldInfo(fieldinfo, it->second.fields[static_cast<size_t>(nIndex)]);
}

int CDaoQueryDef::GetParameterCount() const {
    std::lock_guard<std::mutex> lock(g_daoStateMutex);
    auto it = g_queryDefStates.find(this);
    return (it != g_queryDefStates.end()) ? static_cast<int>(it->second.parameters.size()) : 0;
}

void CDaoQueryDef::GetParameterInfo(int nIndex, CDaoParameterInfo& paraminfo,
                                     DWORD dwInfoOptions) {
    (void)dwInfoOptions;
    paraminfo = CDaoParameterInfo{};
    std::lock_guard<std::mutex> lock(g_daoStateMutex);
    auto it = g_queryDefStates.find(this);
    if (it == g_queryDefStates.end()) return;
    if (nIndex < 0 || nIndex >= static_cast<int>(it->second.parameters.size())) return;
    paraminfo = it->second.parameters[static_cast<size_t>(nIndex)];
}

void CDaoQueryDef::SetParamValue(int nIndex, const COleVariant& varValue) {
    std::lock_guard<std::mutex> lock(g_daoStateMutex);
    auto it = g_queryDefStates.find(this);
    if (it == g_queryDefStates.end()) return;
    auto& parameters = it->second.parameters;
    if (nIndex < 0 || static_cast<size_t>(nIndex) >= parameters.size()) return;
    EnsureVariantCopy(parameters[static_cast<size_t>(nIndex)].m_varValue, varValue);
}

void CDaoQueryDef::SetParamValue(const wchar_t* lpszName, const COleVariant& varValue) {
    if (!lpszName) return;
    std::lock_guard<std::mutex> lock(g_daoStateMutex);
    auto it = g_queryDefStates.find(this);
    if (it == g_queryDefStates.end()) return;
    for (auto& param : it->second.parameters) {
        if (param.m_strName == lpszName) {
            EnsureVariantCopy(param.m_varValue, varValue);
            return;
        }
    }
    CDaoParameterInfo param;
    param.m_strName = lpszName;
    EnsureVariantCopy(param.m_varValue, varValue);
    it->second.parameters.push_back(param);
}

void CDaoQueryDef::Execute(int nOptions) {
    (void)nOptions;
}

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
    return TRUE;
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
