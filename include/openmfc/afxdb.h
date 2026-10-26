// OpenMFC Database Classes
// ABI-compatible declarations for mfc140u.dll database exports
#pragma once

#ifndef OPENMFC_AFXDB_H
#define OPENMFC_AFXDB_H

#include "afx.h"
#include <sql.h>
#include <sqlext.h>
#include <sqltypes.h>
#include <wincrypt.h>

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
    DECLARE_DYNAMIC(CDBException)
public:
    explicit CDBException(RETCODE nRetCode=SQL_SUCCESS) : CException(TRUE), m_nRetCode(nRetCode) {}
    virtual ~CDBException() {}
    virtual void BuildErrorString(CDatabase* database,HSTMT statement,BOOL trace=TRUE);
    void Empty();
    BOOL GetErrorMessage(wchar_t* output,UINT capacity,UINT* help=nullptr) const override;

    RETCODE m_nRetCode;
    CString m_strError;
    CString m_strStateNativeOrigin;
};
void AFXAPI AfxThrowDBException(RETCODE code,CDatabase* database,HSTMT statement);
HENV AFXAPI AfxGetHENV();

//=============================================================================
// CDatabase - ODBC Database Connection
//=============================================================================
class CDatabase : public CObject {
    DECLARE_DYNAMIC(CDatabase)
public:
    CDatabase();
    virtual ~CDatabase();
    enum DbOpenOptions {
        openExclusive=0x0001,openReadOnly=0x0002,useCursorLib=0x0004,
        noOdbcDialog=0x0008,forceOdbcDialog=0x0010
    };

    // Connection management
    virtual BOOL Open(const wchar_t* lpszDSN, BOOL bExclusive = FALSE,
              BOOL bReadOnly = FALSE, const wchar_t* lpszConnect = L"ODBC;",
              BOOL bUseCursorLib = TRUE);
    BOOL Open(const wchar_t* lpszDSN, int bExclusive, int bReadOnly,
              void* lpszConnect, int bUseCursorLib)
        { return Open(lpszDSN, (BOOL)bExclusive, (BOOL)bReadOnly,
                      (const wchar_t*)lpszConnect, (BOOL)bUseCursorLib); }
    virtual BOOL OpenEx(const wchar_t* lpszConnectString, DWORD options=0);
    virtual void Close();
    BOOL IsOpen() const { return m_hdbc != SQL_NULL_HDBC; }

    // Operations
    BOOL BeginTrans();
    BOOL CommitTrans();
    BOOL Rollback();
    void ExecuteSQL(const wchar_t* lpszSQL);
    void Cancel();

    // Direct execution
    BOOL CanTransact() const;
    BOOL CanUpdate() const;
    CString GetConnect() const;
    CString GetDatabaseName() const;
    virtual void OnSetOptions(HSTMT statement);
    virtual void BindParameters(HSTMT statement);
    virtual BOOL Check(RETCODE code) const;
    virtual void ThrowDBException(RETCODE code);

    // Connection info
    int GetLoginTimeout() const;
    void SetLoginTimeout(DWORD seconds);
    int GetQueryTimeout() const;
    void SetQueryTimeout(DWORD seconds);
    DWORD GetBookmarkPersistence() const {return m_dwBookmarkAttributes;}
    int GetCursorCommitBehavior() const {return m_nCursorCommitBehavior;}
    int GetCursorRollbackBehavior() const {return m_nCursorRollbackBehavior;}

    // Low-level access
    HDBC GetHDBC() const { return m_hdbc; }
    HENV GetHENV() const;

public:
    HDBC m_hdbc;
    BOOL m_bStripTrailingSpaces;
    BOOL m_bIncRecordCountOnAdd;
    BOOL m_bAddForUpdate;
    wchar_t m_chIDQuoteChar;
    CString m_strConnect;
    using ConnectionBlob=DATA_BLOB;
    ConnectionBlob m_blobConnect;
    // The public CPtrList alias is still a template with a different ABI.
    // Construct this storage with the existing native CPtrList implementation.
    alignas(8) BYTE m_listRecordsets[56];
    int nRefCount;
    BOOL m_bUpdatable;
    BOOL m_bTransactions;
    SWORD m_nTransactionCapable;
    SWORD m_nCursorCommitBehavior;
    SWORD m_nCursorRollbackBehavior;
    DWORD m_dwUpdateOptions;
    DWORD m_dwBookmarkAttributes;
    DWORD m_dwLoginTimeout;
    HSTMT m_hstmt;
    DWORD m_dwQueryTimeout;
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
struct CFieldInfo {
    void* m_pvDataCache;
    SQLLEN m_nLength;
    int m_nDataType;
    BYTE m_bStatus;
};
struct CODBCFieldInfo {
    CString m_strName;
    SWORD m_nSQLType;
    SQLULEN m_nPrecision;
    SWORD m_nScale;
    SWORD m_nNullability;
};
class CRecordset : public CObject {
    DECLARE_DYNAMIC(CRecordset)
public:
    explicit CRecordset(CDatabase* database=nullptr);
    ~CRecordset() override;
    // Native release x64 virtual slots 5..26.
    virtual BOOL Open(UINT type=AFX_DB_USE_DEFAULT_TYPE,const wchar_t* sql=nullptr,DWORD options=0);
    virtual void Close();
    virtual void Move(long rows,WORD fetch=SQL_FETCH_RELATIVE);
    virtual void SetRowsetSize(DWORD);
    virtual void CheckRowsetError(RETCODE);
    virtual void AddNew();
    virtual void Edit();
    virtual BOOL Update();
    virtual void Delete();
    virtual BOOL Requery();
    virtual CString GetDefaultConnect();
    virtual CString GetDefaultSQL();
    virtual void OnSetOptions(HSTMT);
    virtual void OnSetUpdateOptions(HSTMT);
    virtual void DoFieldExchange(CFieldExchange*);
    virtual void DoBulkFieldExchange(CFieldExchange*);
    virtual BOOL Check(RETCODE) const;
    virtual void SetRowsetCurrencyStatus(RETCODE,WORD,long,SQLULEN);
    virtual void PreBindFields();
    virtual void ThrowDBException(RETCODE,HSTMT=nullptr);
    virtual SQLLEN GetLBFetchSize(SQLLEN);
    virtual SQLLEN GetLBReallocSize(SQLLEN);

    BOOL IsOpen() const;
    BOOL IsBOF() const {return m_bBOF;}
    BOOL IsEOF() const {return m_bEOF;}
    BOOL IsDeleted() const;
    BOOL IsFieldDirty(void*);
    BOOL IsFieldNull(void*);
    BOOL IsFieldNullable(unsigned long);
    void MoveFirst();void MoveLast();void MoveNext();void MovePrev();
    void SetAbsolutePosition(long);
    long GetAbsolutePosition() const {return m_lCurrentRecord;}
    void CancelUpdate();
    void StoreFields();void LoadFields();
    void PrepareUpdateHstmt();void BindFieldsForUpdate();void UnbindFieldsForUpdate();
    void Cancel();void SendLongBinaryData(HSTMT);
    void ExecuteSetPosUpdate();void ExecuteUpdateSQL();
    void ResetCursor();void RefreshRowset(WORD,WORD=SQL_LOCK_NO_CHANGE);
    void SetRowsetCursorPosition(WORD,WORD=SQL_LOCK_NO_CHANGE);
    void SetLockingMode(UINT);
    void SetFieldNull(void*,BOOL=TRUE);
    void SetFieldDirty(void*,BOOL=TRUE);
    BOOL CanAppend() const;BOOL CanRestart() const;BOOL CanScroll() const;BOOL CanTransact() const;BOOL CanUpdate() const;
    long GetRecordCount() const {return m_lRecordCount;}
    DWORD GetRowsetSize() const {return m_dwRowsetSize;}
    HSTMT GetHSTMT() const {return m_hstmt;}
    CDatabase* GetDatabase() const {return m_pDatabase;}
    void OnFieldChange(void*,LONG*);
    enum OpenOptions {none=0,readOnly=0x4,appendOnly=0x8,skipDeletedRecords=0x10,noDirtyFieldCheck=0x20,
        useBookmarks=0x100,useMultiRowFetch=0x200,userAllocMultiRowBuffers=0x400,useExtendedFetch=0x800,
        executeDirect=0x2000,optimizeBulkAdd=0x4000,firstBulkAdd=0x8000};
    using OpenFlags=OpenOptions;
    enum OpenType {dynaset=0,snapshot=1,forwardOnly=2,dynamic=3};
    using FetchType=OpenType;
    enum EditMode {noMode=0,edit=1,addnew=2};
    enum LockMode {optimistic,pessimistic};
    void MarkForUpdate();void MarkForAddNew();
    void FreeDataCache();
    void FreeRowset();
    void AllocRowset();
    void AllocAndCacheFieldInfo();
    UINT BindFieldsToColumns();
    void AllocDataCache();
    void InitRecord();
    BOOL AllocHstmt();
    void SetState(UINT,const wchar_t*,DWORD);
    void AllocStatusArrays();
    SQLLEN* GetFieldLengthBuffer(DWORD,int);
    BYTE GetFieldStatus(DWORD);
    void SetFieldStatus(DWORD,BYTE);
    void ClearFieldStatus();
    void SetDirtyFieldStatus(DWORD);void ClearDirtyFieldStatus(DWORD);
    BOOL IsFieldStatusDirty(DWORD) const;
    BOOL IsFieldStatusNull(DWORD) const;
    void SetNullFieldStatus(DWORD);void ClearNullFieldStatus(DWORD);
    BOOL IsParamStatusNull(DWORD) const;
    void SetNullParamStatus(DWORD);void ClearNullParamStatus(DWORD);
    int GetBoundFieldIndex(void*);int GetBoundParamIndex(void*);
    void SetParamNull(int,BOOL=TRUE);

    HSTMT m_hstmt;CDatabase* m_pDatabase;CString m_strFilter;CString m_strSort;
    UINT m_nFields,m_nParams;
    BOOL m_bCheckCacheForDirtyFields,m_bRebindParams,m_bLongBinaryColumns,m_bUseUpdateSQL;
    DWORD m_dwOptions;SWORD m_nResultCols;BOOL m_bUseODBCCursorLib;
    CODBCFieldInfo* m_rgODBCFieldInfos;CFieldInfo* m_rgFieldInfos;
    CMapPtrToPtr m_mapFieldIndex,m_mapParamIndex;
    void** m_pvFieldProxy;void** m_pvParamProxy;UINT m_nProxyFields,m_nProxyParams;
    UINT m_nOpenType,m_nDefaultType;
    union {long m_lOpen;BOOL m_bOpen;};
    UINT m_nEditMode;BOOL m_bEOFSeen;
    union {long m_lRecordCount;long m_nRecordCount;};
    union {long m_lCurrentRecord;long m_nAbsolutePosition;};
    CString m_strCursorName;
    BOOL m_nLockMode;UDWORD m_dwDriverConcurrency,m_dwConcurrency;
    UWORD* m_rgRowStatus;SQLULEN m_dwRowsFetched;HSTMT m_hstmtUpdate;
    BOOL m_bRecordsetDb,m_bBOF,m_bEOF,m_bUpdatable,m_bAppendable;
    CString m_strSQL,m_strUpdateSQL,m_strTableName;
    BOOL m_bScrollable,m_bDeleted;int m_nFieldsBound;
    BYTE* m_pbFieldFlags;BYTE* m_pbParamFlags;SQLLEN* m_plParamLength;
    DWORD m_dwInitialGetDataLen,m_dwRowsetSize,m_dwAllocatedRowsetSize;
    CString m_strRequerySQL,m_strRequeryFilter,m_strRequerySort;
};
static_assert(sizeof(CRecordset)==440 && sizeof(CFieldInfo)==24 && sizeof(CODBCFieldInfo)==32,"native database field storage layouts");

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
