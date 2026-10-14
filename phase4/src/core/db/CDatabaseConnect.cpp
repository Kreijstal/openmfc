// Native Unicode connection path, including modal ownership and DPAPI storage.
#define OPENMFC_APPCORE_IMPL
#include "detail/CRecordsetSupport.h"
#include "detail/DatabaseStringBuffer.h"
#include <wincrypt.h>
#include <cwchar>
#include <cstring>
extern "C" void MS_ABI impl__AfxThrowInvalidArgException__YAXXZ();
extern "C" void MS_ABI impl__AllocConnect_CDatabase__IEAAXK_Z(CDatabase*,DWORD);
extern "C" void MS_ABI impl__Free_CDatabase__IEAAXXZ(CDatabase*);
extern "C" void MS_ABI impl__VerifyConnect_CDatabase__IEAAXXZ(CDatabase*);
extern "C" void MS_ABI impl__GetConnectInfo_CDatabase__IEAAXXZ(CDatabase*);
extern "C" HWND MS_ABI impl__GetSafeOwner__CWnd__SAPEAUHWND____PEAU2_PEAPEAU2__Z(HWND,HWND*);
namespace {
void ProtectConnection(CDatabase* database) {
    const wchar_t* text=database->m_strConnect.GetString();
    DATA_BLOB input{DWORD((std::wcslen(text)+1)*sizeof(wchar_t)),reinterpret_cast<BYTE*>(const_cast<wchar_t*>(text))};
    if(::CryptProtectData(&input,nullptr,nullptr,nullptr,nullptr,0,&database->m_blobConnect)) {
        ::SecureZeroMemory(const_cast<wchar_t*>(database->m_strConnect.GetString()),size_t(database->m_strConnect.GetLength())*sizeof(wchar_t));
        openmfc::detail::DatabaseStringBuffer(database->m_strConnect).empty();
    }
}
}
// Symbol: ?Connect@CDatabase@@IEAAHK@Z
extern "C" BOOL MS_ABI impl__Connect_CDatabase__IEAAHK_Z(CDatabase* database,DWORD options) {
    HWND disabled=nullptr;HWND owner=impl__GetSafeOwner__CWnd__SAPEAUHWND____PEAU2_PEAPEAU2__Z(nullptr,&disabled);
    if(!owner)owner=::GetDesktopWindow();
    if(database->m_strConnect.IsEmpty()) {
        DATA_BLOB decoded;
        if(::CryptUnprotectData(&database->m_blobConnect,nullptr,nullptr,nullptr,nullptr,0,&decoded)) {
            openmfc::detail::DatabaseStringBuffer(database->m_strConnect).set(reinterpret_cast<wchar_t*>(decoded.pbData));
            ::LocalFree(database->m_blobConnect.pbData);database->m_blobConnect.pbData=nullptr;
            ::LocalFree(decoded.pbData);
        }
    }
    WORD completion=options&CDatabase::noOdbcDialog?SQL_DRIVER_NOPROMPT:options&CDatabase::forceOdbcDialog?SQL_DRIVER_PROMPT:SQL_DRIVER_COMPLETE;
    wchar_t output[1024];SQLSMALLINT length;
    RETCODE code=::SQLDriverConnectW(database->m_hdbc,owner,reinterpret_cast<SQLWCHAR*>(const_cast<wchar_t*>(database->m_strConnect.GetString())),SQL_NTS,
        reinterpret_cast<SQLWCHAR*>(output),1024,&length,completion);
    if(disabled)::EnableWindow(disabled,TRUE);
    if(code==SQL_NO_DATA) {
        impl__Free_CDatabase__IEAAXXZ(database);::SecureZeroMemory(output,sizeof(output));return FALSE;
    }
    if(!database->Check(code)) {::SecureZeroMemory(output,sizeof(output));database->ThrowDBException(code);}
    openmfc::detail::DatabaseStringBuffer text(database->m_strConnect);text.set(L"ODBC;");text.append(output);ProtectConnection(database);
    ::SecureZeroMemory(output,sizeof(output));return TRUE;
}
BOOL CDatabase::OpenEx(const wchar_t* connection,DWORD options) {
    if((options&0x18)==0x18) {impl__AfxThrowInvalidArgException__YAXXZ();return FALSE;}
    m_bUpdatable=(options&openReadOnly)?FALSE:TRUE;
    openmfc::detail::DatabaseStringBuffer(m_strConnect).set(connection);ProtectConnection(this);
    try {
        impl__AllocConnect_CDatabase__IEAAXK_Z(this,options);
        if(!impl__Connect_CDatabase__IEAAHK_Z(this,options))return FALSE;
        impl__VerifyConnect_CDatabase__IEAAXXZ(this);impl__GetConnectInfo_CDatabase__IEAAXXZ(this);
        return TRUE;
    }catch(CException*) {impl__Free_CDatabase__IEAAXXZ(this);throw;}
}
// Symbol: ?OpenEx@CDatabase@@UEAAHPEB_WK@Z
extern "C" BOOL MS_ABI impl__OpenEx_CDatabase__UEAAHPEB_WK_Z(CDatabase* database,const wchar_t* connection,DWORD options) {
    if(!database) {impl__AfxThrowInvalidArgException__YAXXZ();return FALSE;}
    return database->CDatabase::OpenEx(connection,options);
}
BOOL CDatabase::Open(const wchar_t* dsn,BOOL exclusive,BOOL readOnly,const wchar_t* connection,BOOL cursorLibrary) {
    CString text(connection);
    if(!::_wcsnicmp(text.GetString(),L"ODBC;",5))text=text.Right(text.GetLength()-5);
    if(dsn && *dsn) {text+=L";DSN=";text+=dsn;}
    DWORD options=(exclusive?openExclusive:0)|(readOnly?openReadOnly:0)|(cursorLibrary?useCursorLib:0);
    return OpenEx(text.GetString(),options);
}
// Symbol: ?Open@CDatabase@@UEAAHPEB_WHH0H@Z
extern "C" BOOL MS_ABI impl__Open_CDatabase__UEAAHPEB_WHH0H_Z(CDatabase* database,const wchar_t* dsn,BOOL exclusive,BOOL readOnly,const wchar_t* connection,BOOL cursorLibrary) {
    return database->CDatabase::Open(dsn,exclusive,readOnly,connection,cursorLibrary);
}
