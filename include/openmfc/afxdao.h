// OpenMFC DAO Database Classes
// ABI-compatible declarations for MFC DAO exports (CDaoDatabase, CDaoRecordset, etc.)
#pragma once

#include "afxwin.h"
#include "afxdisp.h"  // COleVariant
#include "afxole.h"   // COleCurrency, COleDateTime
#include <objbase.h>
#include <cstring>

//=============================================================================
// DAO info structures (must be before the classes that use them)
//=============================================================================

// AFX_DAO_* constants
#define AFX_DAO_PRIMARY_INFO     0x0
#define AFX_DAO_SECONDARY_INFO   0x1
#define AFX_DAO_ALL_INFO         0x2

struct CDaoFieldInfo {
    CString m_strName;
    short m_nType;
    long m_lSize;
    long m_lAttributes;
    short m_nOrdinalPosition;
    BOOL m_bRequired;
    BOOL m_bAllowZeroLength;
    long m_lCollatingOrder;
    CString m_strForeignName;
    CString m_strSourceField;
    CString m_strSourceTable;
    CString m_strValidationRule;
    CString m_strValidationText;
    CString m_strDefaultValue;
};

struct CDaoIndexFieldInfo {
    CString m_strName;
    BOOL m_bDescending;
};

struct CDaoIndexInfo {
    CString m_strName;
    int m_nFields;
    CDaoIndexFieldInfo* m_pFieldInfos;
    BOOL m_bPrimary;
    BOOL m_bUnique;
    BOOL m_bClustered;
    BOOL m_bIgnoreNulls;
    BOOL m_bRequired;
    BOOL m_bForeign;
    long m_lDistinctCount;
};

struct CDaoTableDefInfo {
    CString m_strName;
    long m_lAttributes;
    CString m_strSourceTable;
    CString m_strConnect;
    CString m_strValidationRule;
    CString m_strValidationText;
    long m_lRecordCount;
    BOOL m_bUpdatable;
};

struct CDaoParameterInfo {
    CString m_strName;
    short m_nType;
    COleVariant m_varValue;
};

struct CDaoRelationInfo {
    CString m_strName;
    CString m_strTable;
    CString m_strForeignTable;
    long m_lAttributes;
    CString m_strField;
    CString m_strForeignField;
};

//=============================================================================
// CLongBinary - wrapper for HGLOBAL binary data
//=============================================================================
class CLongBinary : public CObject {
public:
    CLongBinary() : m_hData(nullptr), m_dwDataLength(0) {}
    ~CLongBinary() { if (m_hData) ::GlobalFree(m_hData); }
    HGLOBAL m_hData;
    DWORD m_dwDataLength;
};

//=============================================================================
// Forward declarations for classes
//=============================================================================
class CDaoDatabase;
class CDaoWorkspace;
class CDaoTableDef;
class CDaoQueryDef;
class CDaoRecordset;
class CDaoFieldExchange;

// DAO-specific error types
typedef long DAO_ERR;

//=============================================================================
// DAO-specific error types
//=============================================================================
typedef long DAO_ERR;

//=============================================================================
// CDaoException - DAO Error Handling
//=============================================================================
class CDaoException : public CException {
    DECLARE_DYNAMIC(CDaoException)
public:
    CDaoException() : m_scode(0), m_nAfxDaoError(0), m_pErrorInfo(0) {}
    virtual ~CDaoException() {}

    BOOL GetErrorMessage(wchar_t* lpszError, UINT nMaxError, UINT* pnHelpContext = nullptr) {
        if (!lpszError || nMaxError == 0) return FALSE;
        if (!m_strError.IsEmpty()) {
            wcsncpy(lpszError, m_strError, nMaxError - 1);
            lpszError[nMaxError - 1] = L'\0';
        } else {
            lpszError[0] = L'\0';
        }
        if (pnHelpContext) *pnHelpContext = 0;
        return TRUE;
    }

    long m_scode;
    long m_nAfxDaoError;
    CString m_strError;
    CString m_strDaoOrigin;
    long m_pErrorInfo;

protected:
    char _daoexception_padding[32];
};

//=============================================================================
// CDaoWorkspace - DAO Workspace
//=============================================================================
class CDaoWorkspace : public CObject {
    DECLARE_DYNAMIC(CDaoWorkspace)
public:
    CDaoWorkspace();
    explicit CDaoWorkspace(CDaoDatabase* pDatabase);
    virtual ~CDaoWorkspace();

    CString GetName() const;
    CString GetUserName() const;
    BOOL IsOpen() const;

    CDaoDatabase* GetDatabase(int nIndex = 0);
    int GetDatabaseCount() const;

    void BeginTrans();
    void CommitTrans();
    void Rollback();
    BOOL GetInTransaction() const;
    void Close();

    static CDaoWorkspace* PASCAL GetDefaultWorkspace();
    static CString PASCAL GetVersion();

public:
    void* m_pDAOWorkspace;
    BOOL m_bOpen;

protected:
    char _daoworkspace_padding[64];
};

//=============================================================================
// CDaoDatabase - DAO Database Connection
//=============================================================================
class CDaoDatabase : public CObject {
    DECLARE_DYNAMIC(CDaoDatabase)
public:
    CDaoDatabase(CDaoWorkspace* pWorkspace = nullptr);
    virtual ~CDaoDatabase();

    virtual void Open(const wchar_t* lpszName, BOOL bExclusive = FALSE,
                      BOOL bReadOnly = FALSE, const wchar_t* lpszConnect = L"");
    virtual void Close();
    BOOL IsOpen() const;

    CString GetName() const;
    CString GetConnect() const;
    long GetRecordsAffected() const;
    BOOL CanUpdate() const;
    BOOL CanTransact() const;
    int GetTableDefCount() const;
    void GetTableDefInfo(int nIndex, CDaoTableDefInfo& tabledefinfo,
                         DWORD dwInfoOptions = AFX_DAO_PRIMARY_INFO);

    void CreateTableDef(const wchar_t* lpszName, long lAttributes = 0,
                        long lOptions = 0, const wchar_t* lpszSource = nullptr,
                        const wchar_t* lpszConnect = nullptr);
    void DeleteTableDef(const wchar_t* lpszName);

    CDaoQueryDef* CreateQueryDef(const wchar_t* lpszName = nullptr,
                                 const wchar_t* lpszSQL = nullptr);
    void DeleteQueryDef(const wchar_t* lpszName);

    void BeginTrans();
    void CommitTrans();
    void Rollback();
    BOOL GetInTransaction() const;
    void Execute(const wchar_t* lpszSQL, int nOptions = 0);

    void CreateRelation(const wchar_t* lpszName, const wchar_t* lpszTable,
                        const wchar_t* lpszForeignTable, long lAttributes,
                        const wchar_t* lpszField, const wchar_t* lpszForeignField);
    void CreateRelation(CDaoRelationInfo& relinfo);
    void DeleteRelation(const wchar_t* lpszName);

    CDaoWorkspace* GetWorkspace();

public:
    void* m_pDAODatabase;
    CDaoWorkspace* m_pWorkspace;
    BOOL m_bOpen;
    long m_lRecordsAffected;

protected:
    char _daodatabase_padding[64];
};

//=============================================================================
// CDaoRecordset - DAO Recordset
//=============================================================================
class CDaoRecordset : public CObject {
    DECLARE_DYNAMIC(CDaoRecordset)
public:
    CDaoRecordset(CDaoDatabase* pDatabase = nullptr);
    virtual ~CDaoRecordset();

    virtual void Open(int nOpenType = 0, const wchar_t* lpszSQL = nullptr,
                      int nOptions = 0);
    virtual void Open(CDaoTableDef* pTableDef, int nOpenType = 0, int nOptions = 0);
    virtual void Open(CDaoQueryDef* pQueryDef, int nOpenType = 0, int nOptions = 0);
    void Close();
    BOOL IsOpen() const;
    BOOL IsBOF() const;
    BOOL IsEOF() const;
    BOOL CanUpdate() const;
    BOOL CanAppend() const;

    virtual void DoFieldExchange(CDaoFieldExchange* pFX);

    void MoveFirst();
    void MoveLast();
    void MoveNext();
    void MovePrev();
    void Move(long lRows);
    long GetRecordCount() const;
    long GetAbsolutePosition() const;
    void SetAbsolutePosition(long lPos);
    double GetPercentPosition() const;
    void SetPercentPosition(double dPosition);
    void FindFirst(const wchar_t* lpszCriteria);
    void FindLast(const wchar_t* lpszCriteria);
    void FindNext(const wchar_t* lpszCriteria);
    void FindPrev(const wchar_t* lpszCriteria);

    virtual void AddNew();
    virtual void Edit();
    virtual void Update();
    virtual void Delete();
    void CancelUpdate();

    void SetBookmark();
    COleVariant GetBookmark();

    void GetFieldValue(const wchar_t* lpszName, COleVariant& varValue);
    void GetFieldValue(int nIndex, COleVariant& varValue);
    void SetFieldValue(const wchar_t* lpszName, const COleVariant& varValue);
    void SetFieldValue(int nIndex, const COleVariant& varValue);

    void SetCurrentIndex(const wchar_t* lpszIndex);
    CString GetCurrentIndex() const;

    CString GetSQL() const;
    void SetSQL(const wchar_t* lpszSQL);
    CDaoDatabase* GetDatabase() const;

    int GetFieldCount() const;
    void GetFieldInfo(int nIndex, CDaoFieldInfo& fieldinfo,
                      DWORD dwInfoOptions = AFX_DAO_PRIMARY_INFO);
    void GetFieldInfo(const wchar_t* lpszName, CDaoFieldInfo& fieldinfo,
                      DWORD dwInfoOptions = AFX_DAO_PRIMARY_INFO);
    int GetIndexCount() const;
    void GetIndexInfo(int nIndex, CDaoIndexInfo& indexinfo,
                      DWORD dwInfoOptions = AFX_DAO_PRIMARY_INFO);
    void GetIndexInfo(const wchar_t* lpszName, CDaoIndexInfo& indexinfo,
                      DWORD dwInfoOptions = AFX_DAO_PRIMARY_INFO);
    BOOL Seek(const wchar_t* lpszComparison, COleVariant* pKey1,
              COleVariant* pKey2 = nullptr, COleVariant* pKey3 = nullptr);

    CString GetValidationRule() const;
    CString GetValidationText() const;

    virtual CDaoDatabase* GetDefaultDB();
    virtual CString GetDefaultSQL();
    void FillCache(long* pSize = nullptr, COleVariant* pBookmark = nullptr);

public:
    CDaoDatabase* m_pDatabase;
    void* m_pDAORecordset;
    BOOL m_bOpen;
    CString m_strSQL;
    CString m_strCurrentIndex;
    int m_nFields;
    long m_lRecordCount;

protected:
    char _daorecordset_padding[128];
};

//=============================================================================
// CDaoTableDef - DAO Table Definition
//=============================================================================
class CDaoTableDef : public CObject {
    DECLARE_DYNAMIC(CDaoTableDef)
public:
    CDaoTableDef(CDaoDatabase* pDatabase = nullptr);
    virtual ~CDaoTableDef();

    void Open(const wchar_t* lpszName);
    void Create(const wchar_t* lpszName, long lAttributes = 0,
                const wchar_t* lpszSrcTable = nullptr,
                const wchar_t* lpszConnect = nullptr);
    void Close();
    BOOL IsOpen() const;

    CString GetName() const;
    void SetName(const wchar_t* lpszName);
    CString GetConnect() const;
    CString GetSourceTableName() const;
    void SetSourceTableName(const wchar_t* lpszSrcTable);
    long GetAttributes() const;
    void SetAttributes(long lAttributes);
    BOOL CanUpdate() const;

    void CreateField(const wchar_t* lpszName, short nType, long lSize,
                     long lAttributes = 0);
    void CreateField(CDaoFieldInfo& fieldinfo);
    void DeleteField(const wchar_t* lpszName);
    void DeleteField(int nIndex);
    int GetFieldCount() const;
    void GetFieldInfo(int nIndex, CDaoFieldInfo& fieldinfo,
                      DWORD dwInfoOptions = AFX_DAO_PRIMARY_INFO);
    void GetFieldInfo(const wchar_t* lpszName, CDaoFieldInfo& fieldinfo,
                      DWORD dwInfoOptions = AFX_DAO_PRIMARY_INFO);
    void CreateIndex(CDaoIndexInfo& indexinfo);
    void DeleteIndex(const wchar_t* lpszName);
    void DeleteIndex(int nIndex);
    int GetIndexCount() const;
    void GetIndexInfo(int nIndex, CDaoIndexInfo& indexinfo,
                      DWORD dwInfoOptions = AFX_DAO_PRIMARY_INFO);
    void GetIndexInfo(const wchar_t* lpszName, CDaoIndexInfo& indexinfo,
                      DWORD dwInfoOptions = AFX_DAO_PRIMARY_INFO);

    CString GetValidationRule() const;
    CString GetValidationText() const;
    void RefreshLink();

public:
    CDaoDatabase* m_pDatabase;
    void* m_pDAOTableDef;
    BOOL m_bOpen;

protected:
    char _daotabledef_padding[64];
};

//=============================================================================
// CDaoQueryDef - DAO Query Definition
//=============================================================================
class CDaoQueryDef : public CObject {
    DECLARE_DYNAMIC(CDaoQueryDef)
public:
    CDaoQueryDef(CDaoDatabase* pDatabase = nullptr);
    virtual ~CDaoQueryDef();

    void Open(const wchar_t* lpszName = nullptr);
    void Create(const wchar_t* lpszName = nullptr, const wchar_t* lpszSQL = nullptr);
    void Close();
    BOOL IsOpen() const;

    CString GetName() const;
    void SetName(const wchar_t* lpszName);
    CString GetSQL() const;
    void SetSQL(const wchar_t* lpszSQL);
    BOOL CanUpdate() const;
    long GetType() const;
    CString GetConnect() const;
    void SetConnect(const wchar_t* lpszConnect);
    long GetODBCTimeout() const;
    void SetODBCTimeout(long lODBCTimeout);
    long GetRecordsAffected() const;
    BOOL GetReturnsRecords() const;
    void SetReturnsRecords(BOOL bReturnsRecords);

    int GetFieldCount() const;
    void GetFieldInfo(int nIndex, CDaoFieldInfo& fieldinfo,
                      DWORD dwInfoOptions = AFX_DAO_PRIMARY_INFO);
    int GetParameterCount() const;
    void GetParameterInfo(int nIndex, CDaoParameterInfo& paraminfo,
                          DWORD dwInfoOptions = AFX_DAO_PRIMARY_INFO);
    void SetParamValue(int nIndex, const COleVariant& varValue);
    void SetParamValue(const wchar_t* lpszName, const COleVariant& varValue);

    void Execute(int nOptions = 0);

public:
    CDaoDatabase* m_pDatabase;
    void* m_pDAOQueryDef;
    CString m_strSQL;
    BOOL m_bOpen;

protected:
    char _daoquerydef_padding[64];
};

//=============================================================================
// CDaoFieldExchange - DAO Field Exchange (DFX)
//=============================================================================
class CDaoFieldExchange : public CObject {
public:
    enum {
        outputColumn = 1,
        param = 2
    };

    CDaoFieldExchange(UINT nOperation, CDaoRecordset* pRecordset,
                      void* pvField = nullptr);

    BOOL IsValidOperation();
    UINT m_nOperation;
    CDaoRecordset* m_pRecordset;
    void* m_pvField;
    void* m_pDatabase;
    void* m_pWorkspace;
    void* m_pRecordsetDAO;

protected:
    char _daofieldexchange_padding[32];
};

//=============================================================================
// CDaoRecordView - DAO Record View
//=============================================================================
class CDaoRecordView : public CFormView {
    DECLARE_DYNAMIC(CDaoRecordView)
public:
    CDaoRecordView(UINT nIDTemplate);
    CDaoRecordView(const wchar_t* lpszTemplateName);
    virtual ~CDaoRecordView();

    CDaoRecordset* OnGetRecordset();
    BOOL OnMove(UINT nIDMoveCommand);
    CDaoRecordset* GetRecordset() const { return m_pSet; }

public:
    CDaoRecordset* m_pSet;
    BOOL m_bOnFirstRecord;
    BOOL m_bOnLastRecord;

protected:
    char _daorecordview_padding[48];
};

//=============================================================================
// Global DAO Functions
//=============================================================================
void AFXAPI AfxDaoInit();
void AFXAPI AfxDaoTerm();
void AFXAPI AfxDaoCheck(DAO_ERR scode, long lAfxDaoError,
                        long* pErrorInfo, CDaoException** ppException = nullptr);

// DFX functions
void AFXAPI DFX_Text(CDaoFieldExchange* pFX, const wchar_t* lpszFieldName,
                     CString& value, int nMaxLength = 0, DWORD dwColumnType = 0);
void AFXAPI DFX_Long(CDaoFieldExchange* pFX, const wchar_t* lpszFieldName, long& value);
void AFXAPI DFX_Short(CDaoFieldExchange* pFX, const wchar_t* lpszFieldName, short& value);
void AFXAPI DFX_Double(CDaoFieldExchange* pFX, const wchar_t* lpszFieldName, double& value);
void AFXAPI DFX_Bool(CDaoFieldExchange* pFX, const wchar_t* lpszFieldName, BOOL& value);
void AFXAPI DFX_Currency(CDaoFieldExchange* pFX, const wchar_t* lpszFieldName,
                         COleCurrency& value);
void AFXAPI DFX_DateTime(CDaoFieldExchange* pFX, const wchar_t* lpszFieldName,
                         COleDateTime& value);
void AFXAPI DFX_Binary(CDaoFieldExchange* pFX, const wchar_t* lpszFieldName,
                       CByteArray& value, long nMaxLength = 0);
void AFXAPI DFX_LongBinary(CDaoFieldExchange* pFX, const wchar_t* lpszFieldName,
                           CLongBinary& value, DWORD dwBufSize = 0);
