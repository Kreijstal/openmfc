// Native release x64 parameter binding and SQL field enumeration.
#define OPENMFC_APPCORE_IMPL
#include "detail/CRecordsetSupport.h"
extern "C" void MS_ABI impl__AfxThrowInvalidArgException__YAXXZ();
// Native AddNew/Edit (RVAs 0x1f8f20/0x1f8fb0) prepare buffers and cache;
// Update performs the eventual write. Reentering either operation restores
// the previous snapshot before storing a fresh one.
void CRecordset::AddNew() {
    if (!m_bAppendable) ThrowDBException(0x3f1,nullptr);
    if (m_dwOptions&useMultiRowFetch) return;
    if (m_bCheckCacheForDirtyFields && m_nFields) {
        if (m_nEditMode) LoadFields();
        StoreFields();
    }
    SetFieldNull(nullptr,TRUE);
    SetFieldDirty(nullptr,FALSE);
    m_nEditMode=addnew;
}
void CRecordset::Edit() {
    if (!m_bUpdatable) ThrowDBException(0x3f1,nullptr);
    if (m_dwOptions&useMultiRowFetch) return;
    if (m_bEOF || m_bBOF || m_bDeleted) ThrowDBException(0x3ef,nullptr);
    if ((m_nOpenType==dynaset || m_nOpenType==dynamic) && m_nLockMode==1) {
        RETCODE code;
        // The native pessimistic Edit path passes SQL_LOCK_UNLOCK (2),
        // despite the locking mode's name. Preserve the driver protocol.
        do {code=::SQLSetPos(m_hstmt,1,SQL_POSITION,SQL_LOCK_UNLOCK);} while(code==SQL_STILL_EXECUTING);
        if (!Check(code)) ThrowDBException(code,nullptr);
    }
    if (m_bCheckCacheForDirtyFields && m_nFields) {
        if (m_nEditMode) LoadFields();
        StoreFields();
    }
    m_nEditMode=edit;
}
// Symbol: ?AddNew@CRecordset@@UEAAXXZ
extern "C" void MS_ABI impl__AddNew_CRecordset__UEAAXXZ(CRecordset* rs) {rs->CRecordset::AddNew();}
// Symbol: ?Edit@CRecordset@@UEAAXXZ
extern "C" void MS_ABI impl__Edit_CRecordset__UEAAXXZ(CRecordset* rs) {rs->CRecordset::Edit();}
void CRecordset::StoreFields() {
    CFieldExchange exchange(CFieldExchange::StoreField,this);
    DoFieldExchange(&exchange);
}
void CRecordset::LoadFields() {
    std::memset(m_pbFieldFlags,0,m_nFields);
    CFieldExchange exchange(CFieldExchange::LoadField,this);
    DoFieldExchange(&exchange);
}
void CRecordset::CancelUpdate() {
    if (!m_nEditMode) return;
    m_nEditMode=noMode;
    if (m_bCheckCacheForDirtyFields && m_nFields) LoadFields();
}
// Symbol: ?StoreFields@CRecordset@@QEAAXXZ
extern "C" void MS_ABI impl__StoreFields_CRecordset__QEAAXXZ(CRecordset* rs) {rs->StoreFields();}
// Symbol: ?LoadFields@CRecordset@@QEAAXXZ
extern "C" void MS_ABI impl__LoadFields_CRecordset__QEAAXXZ(CRecordset* rs) {rs->LoadFields();}
// Symbol: ?CancelUpdate@CRecordset@@QEAAXXZ
extern "C" void MS_ABI impl__CancelUpdate_CRecordset__QEAAXXZ(CRecordset* rs) {rs->CancelUpdate();}
// Symbol: ?BindParams@CRecordset@@QEAAIPEAX@Z
extern "C" UINT MS_ABI impl__BindParams_CRecordset__QEAAIPEAX_Z(CRecordset* rs, void* statement) {
    CFieldExchange exchange(CFieldExchange::BindParam, rs);
    exchange.m_hstmt = static_cast<HSTMT>(statement);
    rs->DoFieldExchange(&exchange);
    return exchange.m_nParams;
}
// Symbol: ?RebindParams@CRecordset@@QEAAXPEAX@Z
extern "C" void MS_ABI impl__RebindParams_CRecordset__QEAAXPEAX_Z(CRecordset* rs, void* statement) {
    if (!rs->m_bRebindParams) return;
    CFieldExchange exchange(CFieldExchange::RebindParam, rs);
    exchange.m_hstmt = static_cast<HSTMT>(statement);
    rs->DoFieldExchange(&exchange);
}
// Symbol: ?AppendNames@CRecordset@@QEAAIPEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@PEB_W@Z
extern "C" UINT MS_ABI impl__AppendNames_CRecordset__QEAAIPEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__PEB_W_Z(CRecordset* rs, CString* sql, const wchar_t* separator) {
    CFieldExchange exchange(CFieldExchange::Name, rs);
    exchange.m_pstr = sql;
    exchange.m_lpszSeparator = separator;
    if (rs->m_dwOptions & CRecordset::useMultiRowFetch) rs->DoBulkFieldExchange(&exchange);
    else rs->DoFieldExchange(&exchange);
    return exchange.m_nFields;
}
// Symbol: ?AppendValues@CRecordset@@QEAAIPEAXPEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@PEB_W@Z
extern "C" UINT MS_ABI impl__AppendValues_CRecordset__QEAAIPEAXPEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__PEB_W_Z(CRecordset* rs, void* statement, CString* sql, const wchar_t* separator) {
    CFieldExchange exchange(CFieldExchange::Value, rs);
    exchange.m_pstr = sql;
    exchange.m_lpszSeparator = separator;
    exchange.m_hstmt = static_cast<HSTMT>(statement);
    rs->DoFieldExchange(&exchange);
    return exchange.m_nFields;
}
// Symbol: ?AppendNamesValues@CRecordset@@QEAAIPEAXPEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@PEB_W@Z
extern "C" UINT MS_ABI impl__AppendNamesValues_CRecordset__QEAAIPEAXPEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__PEB_W_Z(CRecordset* rs, void* statement, CString* sql, const wchar_t* separator) {
    if (!rs || !sql || !separator || !rs->m_hstmt || !statement) {
        impl__AfxThrowInvalidArgException__YAXXZ();
        return 0;
    }
    CFieldExchange exchange(CFieldExchange::NameValue, rs);
    exchange.m_pstr = sql;
    exchange.m_lpszSeparator = separator;
    exchange.m_hstmt = static_cast<HSTMT>(statement);
    rs->DoFieldExchange(&exchange);
    return exchange.m_nFields;
}
