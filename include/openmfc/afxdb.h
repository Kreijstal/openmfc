// OpenMFC Database Classes
// ABI-compatible declarations for mfc140u.dll database exports
#pragma once

#ifndef OPENMFC_AFXDB_H
#define OPENMFC_AFXDB_H

#include "afx.h"
#include <sql.h>
#include <sqlext.h>
#include <sqltypes.h>

//=============================================================================
// AFX_DB_USE_DEFAULT_TYPE constant
//=============================================================================
#ifndef AFX_DB_USE_DEFAULT_TYPE
#define AFX_DB_USE_DEFAULT_TYPE (-1)
#endif

//=============================================================================
// Forward declarations
//=============================================================================
class CDatabase;
class CRecordset;
class CDBException;
class CRecordView;
class CFieldExchange;
class CLongBinary;

//=============================================================================
// CDBException - Database Exception
//=============================================================================
class CDBException : public CException {
public:
    CDBException() : CException(1), m_nRetCode(0),
        m_strErrorNativeOrigin(nullptr), m_strError(nullptr),
        m_strStateNativeOrigin(nullptr) {}
    CDBException(RETCODE nRetCode) : CException(1), m_nRetCode(nRetCode),
        m_strErrorNativeOrigin(nullptr), m_strError(nullptr),
        m_strStateNativeOrigin(nullptr) {}
    virtual ~CDBException() {}

    RETCODE m_nRetCode;
    CString m_strError;
    CString m_strStateNativeOrigin;
    CString m_strErrorNativeOrigin;
    char _dbexception_padding[16];
};

//=============================================================================
// CDatabase - ODBC Database Connection
//=============================================================================
class CDatabase : public CObject {
    DECLARE_DYNAMIC(CDatabase)
public:
    CDatabase();
    virtual ~CDatabase();

    // Connection management
    BOOL Open(const wchar_t* lpszDSN, BOOL bExclusive = FALSE,
              BOOL bReadOnly = FALSE, const wchar_t* lpszConnect = L"ODBC;",
              BOOL bUseCursorLib = TRUE);
    BOOL Open(const wchar_t* lpszDSN, int bExclusive, int bReadOnly,
              void* lpszConnect, int bUseCursorLib)
        { return Open(lpszDSN, (BOOL)bExclusive, (BOOL)bReadOnly,
                      (const wchar_t*)lpszConnect, (BOOL)bUseCursorLib); }
    void Close();
    BOOL IsOpen() const { return m_hdbc != SQL_NULL_HDBC; }

    // Operations
    BOOL BeginTrans();
    BOOL CommitTrans();
    BOOL Rollback();
    BOOL ExecuteSQL(const wchar_t* lpszSQL);
    void Cancel();

    // Direct execution
    BOOL CanTransact() const;
    BOOL CanUpdate() const;
    CString GetConnect() const;

    // Connection info
    static int GetLoginTimeout();
    static void SetLoginTimeout(int nSeconds);
    static int GetQueryTimeout();
    static void SetQueryTimeout(int nSeconds);

    // Low-level access
    HDBC GetHDBC() const { return m_hdbc; }
    HENV GetHENV() const { return m_henv; }

public:
    HDBC m_hdbc;
    HENV m_henv;
    BOOL m_bOpen;
    BOOL m_bReadOnly;
    CString m_strConnect;

protected:
    char _cdatabase_padding[48];
};

//=============================================================================
// CFieldExchange - RFX Data Exchange
//=============================================================================
class CFieldExchange {
public:
    enum RFX_Operation {
        BindParam, RebindParam, BindFieldToColumn, BindFieldForUpdate,
        UnbindFieldForUpdate, Fixup, MarkForAddNew, MarkForUpdate,
        Name, NameValue, Value, SetFieldNull, StoreField, LoadField,
        AllocCache, AllocMultiRowBuffer, DeleteMultiRowBuffer
    };
    enum FieldType {
        noFieldType=-1, outputColumn=0, param=SQL_PARAM_INPUT,
        inputParam=param, outputParam=SQL_PARAM_OUTPUT,
        inoutParam=SQL_PARAM_INPUT_OUTPUT
    };
    CFieldExchange(UINT operation, CRecordset* recordset, void* field=nullptr);
    BOOL IsFieldType(UINT* field);
    void SetFieldType(UINT type) {m_nFieldType=type;}
    void Default(const wchar_t* name, void* value, SQLLEN* length,
                 int cType, SQLULEN size, SQLULEN precision);
    SQLLEN GetLongBinarySize(int field);
    void GetLongBinaryData(int field, CLongBinary& binary, SQLLEN* size);
    BYTE* ReallocLongBinary(CLongBinary& binary, SQLLEN required, SQLLEN reallocation);
    void SetNull(const wchar_t* name);

    UINT m_nOperation;                  // 0
    union {CRecordset* m_prs; CRecordset* m_pRecordset;}; // 8
    UINT m_nFieldType;                  // 16
    UINT m_nFieldFound;                 // 20
    CString* m_pstr;                    // 24
    BOOL m_bField;                      // 32
    void* m_pvField;                    // 40
    const wchar_t* m_lpszSeparator;     // 48
    UINT m_nFields;                     // 56
    UINT m_nParams;                     // 60
    UINT m_nParamFields;                // 64
    HSTMT m_hstmt;                      // 72
    SQLLEN m_lDefaultLBFetchSize;        // 80
    SQLLEN m_lDefaultLBReallocSize;      // 88
};

//=============================================================================
// CRecordset - ODBC Record Set
//=============================================================================
class CRecordset : public CObject {
    DECLARE_DYNAMIC(CRecordset)
public:
    CRecordset(CDatabase* pDatabase = nullptr);
    virtual ~CRecordset();

    // Opening and closing
    virtual BOOL Open(UINT nOpenType = AFX_DB_USE_DEFAULT_TYPE,
                      const wchar_t* lpszSQL = nullptr,
                      DWORD dwOptions = CRecordset::none);
    virtual void Close();
    BOOL IsOpen() const { return m_hstmt != SQL_NULL_HSTMT; }
    BOOL IsBOF() const { return m_bBOF; }
    BOOL IsEOF() const { return m_bEOF; }
    BOOL IsDeleted() const;
    BOOL IsFieldDirty(void* pvField);
    BOOL IsFieldNull(void* pvField);
    BOOL IsFieldNullable(unsigned long dwField);

    // Navigation
    virtual void MoveFirst();
    virtual void MoveLast();
    virtual void MoveNext();
    virtual void MovePrev();
    virtual void Move(long nRows, WORD wFetchType = SQL_FETCH_RELATIVE);
    void SetAbsolutePosition(long nRows);
    long GetAbsolutePosition() const { return m_nAbsolutePosition; }

    // Update operations
    virtual void AddNew();
    virtual void Edit();
    virtual void Delete();
    virtual BOOL Update();
    virtual void CancelUpdate();
    void SetFieldNull(void* pvField, BOOL bNull = TRUE);
    BOOL CanAppend() const;
    BOOL CanRestart() const;
    BOOL CanScroll() const;
    BOOL CanTransact() const;
    BOOL CanUpdate() const;

    // Record count
    long GetRecordCount() const { return m_nRecordCount; }
    void SetRowsetSize(DWORD dwNewRowsetSize);
    DWORD GetRowsetSize() const { return m_dwRowsetSize; }

    // Native CRecordset prefix: statement@8, database@16, filter@24, sort@32.
    HSTMT m_hstmt;
    CDatabase* m_pDatabase;

    // Filtering and sorting
    CString m_strFilter;
    CString m_strSort;

    // Overridables
    virtual CString GetDefaultConnect();
    virtual CString GetDefaultSQL();
    virtual void DoFieldExchange(CFieldExchange* pFX);
    virtual BOOL OnSetOptions(HSTMT hstmt);
    virtual void OnFieldChange(void* pvField, LONG* plLength);

    // SQL access
    HSTMT GetHSTMT() const { return m_hstmt; }
    CDatabase* GetDatabase() const { return m_pDatabase; }
    void SetDirtyFieldStatus(DWORD dwFieldStatus);
    void ClearDirtyFieldStatus(DWORD dwFieldStatus);

    // Field exchange helpers
    enum OpenFlags { none = 0, appendOnly = 1, readOnly = 2, optimizeBulkAdd = 4,
                     useMultiRowFetch = 8, skipDeletedRecords = 16,
                     executeDirect = 32, useExtendedFetch = 64,
                     noDirtyFieldCheck = 128 };
    enum FetchType { dynaset = 0, snapshot = 1, forwardOnly = 2, dynamic = 3 };

    // Standard field exchange
    void SetParamNull(int nIndex, BOOL bNull = TRUE);

public:
    // The remaining recordset fields still require their full native layout.
    BOOL m_bBOF;
    BOOL m_bEOF;
    BOOL m_bDeleted;
    BOOL m_bOpen;
    int m_nFields;
    int m_nParams;
    long m_nRecordCount;
    long m_nAbsolutePosition;
    DWORD m_dwRowsetSize;
    int m_nOpenType;
    DWORD m_dwOptions;
    int m_nEditMode;

protected:
    char _crecordset_padding[128];
};

//=============================================================================
// CRecordView - Record View (form view bound to a recordset)
//=============================================================================
class CRecordView : public CFormView {
    DECLARE_DYNAMIC(CRecordView)
public:
    CRecordView(UINT nIDTemplate);
    CRecordView(const wchar_t* lpszTemplateName);
    virtual ~CRecordView();

    virtual BOOL OnMove(UINT nIDMoveCommand);
    virtual BOOL IsOnFirstRecord();
    virtual BOOL IsOnLastRecord();

    CRecordset* GetRecordset() { return m_pSet; }
    virtual CRecordset* OnGetRecordset();

public:
    CRecordset* m_pSet;
    BOOL m_bOnFirstRecord;
    BOOL m_bOnLastRecord;

protected:
    char _crecordview_padding[32];
};

#endif // OPENMFC_AFXDB_H
