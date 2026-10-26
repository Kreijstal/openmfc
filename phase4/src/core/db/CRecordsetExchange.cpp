// Native release x64 parameter binding and SQL field enumeration.
#define OPENMFC_APPCORE_IMPL
#include "detail/CRecordsetSupport.h"
extern "C" void MS_ABI impl__AfxThrowInvalidArgException__YAXXZ();
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
