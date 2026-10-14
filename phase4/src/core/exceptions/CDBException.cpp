// Release x64 database exceptions: mfc140u constructor 0x1f6540,
// BuildErrorString 0x1f6680, GetErrorMessage 0x1f6a50, Empty 0x1f6ac0.
#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxwin.h"
#include "openmfc/afxdb.h"
#include <cstddef>
#include <cwchar>
#include <cerrno>
#include <memory>
#include <new>
#include <string>
#ifdef __GNUC__
#define MS_ABI __attribute__((ms_abi))
#else
#define MS_ABI
#endif
extern "C" void MS_ABI impl__AfxThrowInvalidArgException__YAXXZ();
extern "C" void MS_ABI impl__AfxThrowMemoryException__YAXXZ();
extern "C" void* MS_ABI impl__AfxFindStringResourceHandle__YAPEAUHINSTANCE____I_Z(UINT);
IMPLEMENT_DYNAMIC(CDBException,CException)

void CDBException::Empty() {
    m_strError.Empty();m_strStateNativeOrigin.Empty();
}
void CDBException::BuildErrorString(CDatabase* database,HSTMT statement,BOOL) {
    if(!database)return;
    // Native MFC drains SQLError's legacy stream even when both handles are
    // null. The environment belongs to the process database state.
    for(;;) {
        wchar_t state[64]{},message[512]{};
        SQLINTEGER native=0;SQLSMALLINT length=0;
        RETCODE code=::SQLErrorW(AfxGetHENV(),database->m_hdbc,statement,
            reinterpret_cast<SQLWCHAR*>(state),&native,reinterpret_cast<SQLWCHAR*>(message),511,&length);
        if((code!=SQL_SUCCESS && code!=SQL_SUCCESS_WITH_INFO) || !std::wcscmp(state,L"00000"))return;
        std::wstring summary=L"State:";summary+=state;
        wchar_t number[50];std::swprintf(number,50,L",Native:%ld,Origin:",long(native));summary+=number;
        const wchar_t* body=message;
        while(*body==L'[') {
            const wchar_t* end=std::wcschr(body,L']');if(!end)break;
            summary.append(body,size_t(end-body+1));body=end+1;
            if(*body==L' ')++body;
        }
        summary+=L"\n";
        m_strStateNativeOrigin+=summary.c_str();m_strError+=body;m_strError+=L"\n";
    }
}
BOOL CDBException::GetErrorMessage(wchar_t* output,UINT capacity,UINT* help) const {
    if(!output)return FALSE;
    if(help)*help=0;
    errno_t code=::wcsncpy_s(output,capacity,static_cast<const wchar_t*>(m_strError),_TRUNCATE);
    if(code && code!=STRUNCATE) {
        if(code==ENOMEM)impl__AfxThrowMemoryException__YAXXZ();
        else impl__AfxThrowInvalidArgException__YAXXZ();
        return FALSE;
    }
    output[capacity-1]=0;return TRUE;
}
void AFXAPI AfxThrowDBException(RETCODE code,CDatabase* database,HSTMT statement) {
    auto error=std::make_unique<CDBException>(code);
    if(code==SQL_ERROR && database)error->BuildErrorString(database,statement,TRUE);
    else if(WORD(WORD(code)-1001)<=27) {
        const UINT resource=UINT(int(code)+0xee98);
        auto module=static_cast<HINSTANCE>(impl__AfxFindStringResourceHandle__YAPEAUHINSTANCE____I_Z(resource));
        const wchar_t* text=nullptr;
        int length=module?::LoadStringW(module,resource,reinterpret_cast<wchar_t*>(&text),0):0;
        if(length>0) {
            std::wmemcpy(error->m_strError.GetBuffer(length),text,length);
            error->m_strError.ReleaseBuffer(length);
        }
        else error->m_strError=L"MFC internal error: unable to load error string from resource.";
    }
    throw error.release();
}
// Symbol: ?AfxThrowDBException@@YAXFPEAVCDatabase@@PEAX@Z
extern "C" void MS_ABI impl__AfxThrowDBException__YAXFPEAVCDatabase__PEAX_Z(RETCODE code,CDatabase* database,HSTMT statement) {
    AfxThrowDBException(code,database,statement);
}
// Symbol: ?GetThisClass@CDBException@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CDBException__SAPEAUCRuntimeClass__XZ() {return CDBException::GetThisClass();}
// Symbol: ?GetRuntimeClass@CDBException@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CDBException__UEBAPEAUCRuntimeClass__XZ(const CDBException*) {return CDBException::GetThisClass();}
// Symbol: ?Empty@CDBException@@QEAAXXZ
extern "C" void MS_ABI impl__Empty_CDBException__QEAAXXZ(CDBException* self) {self->Empty();}
// Symbol: ?BuildErrorString@CDBException@@UEAAXPEAVCDatabase@@PEAXH@Z
extern "C" void MS_ABI impl__BuildErrorString_CDBException__UEAAXPEAVCDatabase__PEAXH_Z(CDBException* self,CDatabase* database,HSTMT statement,BOOL trace) {self->CDBException::BuildErrorString(database,statement,trace);}
// Symbol: ?GetErrorMessage@CDBException@@UEBAHPEA_WIPEAI@Z
extern "C" BOOL MS_ABI impl__GetErrorMessage_CDBException__UEBAHPEA_WIPEAI_Z(const CDBException* self,wchar_t* output,UINT capacity,UINT* help) {return self->CDBException::GetErrorMessage(output,capacity,help);}
// Symbol: ??0CDBException@@QEAA@F@Z
extern "C" CDBException* MS_ABI impl___0CDBException__QEAA_F_Z(void* storage,RETCODE code) {return new(storage) CDBException(code);}
// Symbol: ??1CDBException@@UEAA@XZ
extern "C" void MS_ABI impl___1CDBException__UEAA_XZ(CDBException* self) {self->CDBException::~CDBException();}
static_assert(sizeof(CDBException)==40 && offsetof(CDBException,m_nRetCode)==16 && offsetof(CDBException,m_strError)==24 && offsetof(CDBException,m_strStateNativeOrigin)==32,"native database exception layout");
