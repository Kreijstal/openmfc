// Native release x64 fetching, cursor currency and field fixup.
#define OPENMFC_APPCORE_IMPL
#include "detail/CRecordsetSupport.h"
#include <algorithm>
#include <cstdint>
#include <cwchar>
#include <cstring>
extern "C" void MS_ABI impl__AfxThrowInvalidArgException__YAXXZ();
namespace {
long add(long a,long b) { return static_cast<long>(uint32_t(a)+uint32_t(b)); }
void checkCurrency(CRecordset* rs,WORD direction,long rows) {
    if (!rs->m_bScrollable && direction!=SQL_FETCH_NEXT) rs->ThrowDBException(0x3ea,nullptr);
    if (rs->m_bEOF && rs->m_bBOF) rs->ThrowDBException(0x3f8,nullptr);
    if (rs->m_nOpenType!=CRecordset::dynamic &&
        ((rs->m_bEOF && (direction==SQL_FETCH_NEXT || (direction==SQL_FETCH_RELATIVE && rows>0))) ||
         (rs->m_bBOF && (direction==SQL_FETCH_PRIOR || (direction==SQL_FETCH_RELATIVE && rows<0)))))
        rs->ThrowDBException(0x3f8,nullptr);
}
void checkError(CRecordset* rs,RETCODE code) {
    if (code!=SQL_SUCCESS_WITH_INFO) {
        if (!rs->Check(code)) rs->ThrowDBException(code,nullptr);
        return;
    }
    CDBException warning(SQL_SUCCESS_WITH_INFO);
    warning.BuildErrorString(rs->m_pDatabase,rs->m_hstmt,FALSE);
    if (std::wcsstr(warning.m_strStateNativeOrigin.GetString(),L"State:01004")) {
        if (!rs->m_pDatabase) {impl__AfxThrowInvalidArgException__YAXXZ();return;}
        if (!(rs->m_pDatabase->m_dwUpdateOptions&1) || !rs->m_bLongBinaryColumns)
            rs->ThrowDBException(0x3fd,nullptr);
    } else if (std::wcsstr(warning.m_strStateNativeOrigin.GetString(),L"State:01S01"))
        rs->ThrowDBException(0x3fe,nullptr);
}
RETCODE fetch(CRecordset* rs,WORD direction,long rows,SQLULEN* fetched) {
    RETCODE code;
    if (rs->m_nOpenType==CRecordset::forwardOnly && !(rs->m_dwOptions&CRecordset::useExtendedFetch)) {
        do {code=::SQLFetch(rs->m_hstmt);} while(code==SQL_STILL_EXECUTING);
        if (!fetched) {impl__AfxThrowInvalidArgException__YAXXZ();return code;}
        *fetched=1;
        rs->m_bDeleted=FALSE;
    } else {
        do {code=::SQLExtendedFetch(rs->m_hstmt,direction,rows,fetched,rs->m_rgRowStatus);} while(code==SQL_STILL_EXECUTING);
        if (code==SQL_SUCCESS || code==SQL_SUCCESS_WITH_INFO) rs->m_bDeleted=rs->m_rgRowStatus[0]==SQL_ROW_DELETED;
    }
    rs->CheckRowsetError(code);
    return code;
}
void skipDeleted(CRecordset* rs,WORD direction,long rows,SQLULEN* fetched,RETCODE* code) {
    uint32_t remaining=rows<0?0u-uint32_t(rows):uint32_t(rows);
    WORD next=direction;
    if (direction==SQL_FETCH_FIRST) next=SQL_FETCH_NEXT;
    else if (direction==SQL_FETCH_LAST) next=SQL_FETCH_PRIOR;
    else if (direction==SQL_FETCH_RELATIVE) next=rows>0?SQL_FETCH_NEXT:SQL_FETCH_PRIOR;
    *code=fetch(rs,direction==SQL_FETCH_RELATIVE?next:direction,1,fetched);
    for (;;) {
        bool done=!rs->m_bDeleted;
        if (direction==SQL_FETCH_RELATIVE && done) done=--remaining==0;
        if (*code==SQL_NO_DATA_FOUND || done) return;
        *code=fetch(rs,next,1,fetched);
    }
}
void setCurrency(CRecordset* rs,RETCODE code,WORD direction,long rows,SQLULEN) {
    long sign=0;
    const bool noData=code==SQL_NO_DATA_FOUND;
    auto updateCount=[&] {
        if (!rs->m_bEOFSeen && rs->m_lCurrentRecord!=-2)
            rs->m_lRecordCount=std::max(rs->m_lRecordCount,add(rs->m_lCurrentRecord,1));
    };
    switch(direction) {
    case SQL_FETCH_NEXT:
        if (!noData) {
            if (rs->m_lCurrentRecord!=-2) rs->m_lCurrentRecord=add(rs->m_lCurrentRecord,rows);
            updateCount();
        } else if (!rs->m_bEOFSeen && rs->m_lRecordCount==add(rs->m_lCurrentRecord,1)) rs->m_bEOFSeen=TRUE;
        sign=1;break;
    case SQL_FETCH_FIRST:
        if (noData) {rs->m_lRecordCount=0;rs->m_lCurrentRecord=-2;}
        else rs->m_lCurrentRecord=0;
        sign=1;break;
    case SQL_FETCH_LAST:
        if (noData) {rs->m_lRecordCount=0;rs->m_lCurrentRecord=-2;}
        else rs->m_lCurrentRecord=rs->m_bEOFSeen?add(rs->m_lRecordCount,-1):-2;
        sign=-1;break;
    case SQL_FETCH_PRIOR:
        if (!rs->m_bEOF && rs->m_lCurrentRecord!=-2 && !noData) rs->m_lCurrentRecord=add(rs->m_lCurrentRecord,rows);
        sign=-1;break;
    case SQL_FETCH_ABSOLUTE:
        if (noData) rs->m_lCurrentRecord=-2;
        else {
            rs->m_lCurrentRecord=rows>0?add(rows,-1):rs->m_bEOFSeen?add(rs->m_lRecordCount,rows):-2;
            updateCount();
        }
        sign=rows;break;
    case SQL_FETCH_RELATIVE:
        if (!noData) {
            if (rs->m_lCurrentRecord!=-2) rs->m_lCurrentRecord=add(rs->m_lCurrentRecord,rows);
            updateCount();
        }
        sign=rows;break;
    case SQL_FETCH_BOOKMARK:rs->m_lCurrentRecord=-2;break;
    }
    if (!noData) rs->m_bEOF=rs->m_bBOF=FALSE;
    else if (direction==SQL_FETCH_FIRST || direction==SQL_FETCH_LAST || direction==SQL_FETCH_BOOKMARK)
        rs->m_bEOF=rs->m_bBOF=TRUE;
    else {rs->m_bEOF=sign>=0;rs->m_bBOF=sign<0;}
}
void fixups(CRecordset* rs) {
    CFieldExchange exchange(CFieldExchange::Fixup,rs);
    exchange.m_hstmt=rs->m_hstmt;
    rs->DoFieldExchange(&exchange);
}
}
void CRecordset::CheckRowsetError(RETCODE code) {checkError(this,code);}
void CRecordset::SetRowsetCurrencyStatus(RETCODE code,WORD direction,long rows,SQLULEN fetched) {setCurrency(this,code,direction,rows,fetched);}
void CRecordset::ResetCursor() {
    m_lCurrentRecord=-1;
    m_bEOFSeen=FALSE;m_lRecordCount=0;m_bBOF=m_bEOF=FALSE;m_bDeleted=FALSE;
}
void CRecordset::RefreshRowset(WORD row,WORD lock) {
    RETCODE code;
    do {code=::SQLSetPos(m_hstmt,row,SQL_REFRESH,lock);} while(code==SQL_STILL_EXECUTING);
    // Native refresh deliberately ignores the final driver status. Fixup is
    // governed by the recordset's current field, currency and bulk flags.
    if (m_nFields && !m_bEOF && !m_bBOF && !(m_dwOptions&useMultiRowFetch)) fixups(this);
}
void CRecordset::SetRowsetCursorPosition(WORD row,WORD lock) {
    RETCODE code;
    do {code=::SQLSetPos(m_hstmt,row,SQL_POSITION,lock);} while(code==SQL_STILL_EXECUTING);
}
void CRecordset::SetLockingMode(UINT mode) {
    if (mode==1) {
        if (!m_pDatabase) {impl__AfxThrowInvalidArgException__YAXXZ();return;}
        SQLUINTEGER supported;SQLSMALLINT length;
        RETCODE code=::SQLGetInfoW(m_pDatabase->m_hdbc,SQL_LOCK_TYPES,&supported,sizeof(supported),&length);
        if (!Check(code) || !(supported&SQL_LCK_EXCLUSIVE)) ThrowDBException(0x3fc,nullptr);
    }
    m_nLockMode=mode;
}
// Symbol: ?ResetCursor@CRecordset@@QEAAXXZ
extern "C" void MS_ABI impl__ResetCursor_CRecordset__QEAAXXZ(CRecordset* rs) {rs->ResetCursor();}
// Symbol: ?RefreshRowset@CRecordset@@QEAAXGG@Z
extern "C" void MS_ABI impl__RefreshRowset_CRecordset__QEAAXGG_Z(CRecordset* rs,WORD row,WORD lock) {rs->RefreshRowset(row,lock);}
// Symbol: ?SetRowsetCursorPosition@CRecordset@@QEAAXGG@Z
extern "C" void MS_ABI impl__SetRowsetCursorPosition_CRecordset__QEAAXGG_Z(CRecordset* rs,WORD row,WORD lock) {rs->SetRowsetCursorPosition(row,lock);}
// Symbol: ?SetLockingMode@CRecordset@@QEAAXI@Z
extern "C" void MS_ABI impl__SetLockingMode_CRecordset__QEAAXI_Z(CRecordset* rs,UINT mode) {rs->SetLockingMode(mode);}
// Symbol: ?FetchData@CRecordset@@QEAAFGJPEA_K@Z
extern "C" RETCODE MS_ABI impl__FetchData_CRecordset__QEAAFGJPEA_K_Z(CRecordset* rs,WORD direction,long rows,SQLULEN* fetched) {return fetch(rs,direction,rows,fetched);}
// Symbol: ?SkipDeletedRecords@CRecordset@@QEAAXGJPEA_KPEAF@Z
extern "C" void MS_ABI impl__SkipDeletedRecords_CRecordset__QEAAXGJPEA_KPEAF_Z(CRecordset* rs,WORD direction,long rows,SQLULEN* fetched,RETCODE* code) {skipDeleted(rs,direction,rows,fetched,code);}
// Symbol: ?CheckRowsetCurrencyStatus@CRecordset@@QEAAXGJ@Z
extern "C" void MS_ABI impl__CheckRowsetCurrencyStatus_CRecordset__QEAAXGJ_Z(CRecordset* rs,WORD direction,long rows) {checkCurrency(rs,direction,rows);}
// Symbol: ?CheckRowsetError@CRecordset@@UEAAXF@Z
extern "C" void MS_ABI impl__CheckRowsetError_CRecordset__UEAAXF_Z(CRecordset* rs,RETCODE code) {checkError(rs,code);}
// Symbol: ?SetRowsetCurrencyStatus@CRecordset@@UEAAXFGJ_K@Z
extern "C" void MS_ABI impl__SetRowsetCurrencyStatus_CRecordset__UEAAXFGJ_K_Z(CRecordset* rs,RETCODE code,WORD direction,long rows,SQLULEN fetched) {setCurrency(rs,code,direction,rows,fetched);}
// Symbol: ?Fixups@CRecordset@@QEAAXXZ
extern "C" void MS_ABI impl__Fixups_CRecordset__QEAAXXZ(CRecordset* rs) {fixups(rs);}
void CRecordset::Move(long rows,WORD direction) {
    if (!m_nFieldsBound) {
        InitRecord();
        m_bBOF=m_bEOF=FALSE;m_bEOFSeen=FALSE;m_lRecordCount=0;m_bDeleted=FALSE;m_lCurrentRecord=-1;
    }
    if (m_nFieldsBound>0) {std::memset(m_pbFieldFlags,0,m_nFields);m_nEditMode=0;}
    checkCurrency(this,direction,rows);
    RETCODE code;
    if ((m_dwOptions&skipDeletedRecords) && ((direction>=1 && direction<=4) || direction==SQL_FETCH_RELATIVE))
        skipDeleted(this,direction,rows,&m_dwRowsFetched,&code);
    else code=fetch(this,direction,rows,&m_dwRowsFetched);
    SetRowsetCurrencyStatus(code,direction,rows,m_dwRowsFetched);
    if (m_nFields && !m_bEOF && !m_bBOF && !(m_dwOptions&useMultiRowFetch)) fixups(this);
}
// Symbol: ?Move@CRecordset@@UEAAXJG@Z
extern "C" void MS_ABI impl__Move_CRecordset__UEAAXJG_Z(CRecordset* rs,long rows,WORD direction) {rs->CRecordset::Move(rows,direction);}
