// CFtpFileFind — OpenMFC implementation.
// Sources: global_ctordtor_param.cpp, inetcore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/InetcoreSupport.h"

// Real parameterized-constructor export thunks for header-defined, publicly
// constructible MFC classes. Each viewed-as MS_ABI thunk placement-constructs
// the object with the real ctor body (defined in filecore/olecore/dlgcommon/
// inetcore or inline in headers) -- NOT a stub. Completes classes that were one
// export short of full coverage.
#include "openmfc/afx.h"
#include "openmfc/afxwin.h"
#include "openmfc/afxmfc.h"
#include "openmfc/afxole.h"
#include "openmfc/afxdisp.h"
#include "openmfc/afxdb.h"
#include "openmfc/afxinet.h"
#include <new>
#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif


















// Symbol: ??0CFtpFileFind@@QEAA@PEAVCFtpConnection@@_K@Z
extern "C" void* MS_ABI impl___0CFtpFileFind__QEAA_PEAVCFtpConnection___K_Z(
        void* p, CFtpConnection* pConn, unsigned long long dwContext) {
    return new (p) CFtpFileFind(pConn, dwContext);
}
// Symbol: ?CloseContext@CFtpFileFind@@MEAAXXZ
extern "C" void MS_ABI impl__CloseContext_CFtpFileFind__MEAAXXZ(CFtpFileFind* pThis) {
    if (pThis) pThis->Close();
}
// Symbol: ?FindNextFileW@CFtpFileFind@@UEAAHXZ
extern "C" int MS_ABI impl__FindNextFileW_CFtpFileFind__UEAAHXZ(CFtpFileFind* pThis) {
    return pThis ? pThis->FindNextFile() : FALSE;
}
// Symbol: ?GetFileURL@CFtpFileFind@@UEBA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@XZ
extern "C" void MS_ABI impl__GetFileURL_CFtpFileFind__UEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__XZ(
    CString* pRet, const CFtpFileFind* pThis) {
    openmfcConstructString(pRet, pThis ? pThis->GetFileURL() : CString());
}
// Symbol: ?GetRuntimeClass@CFtpFileFind@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CFtpFileFind__UEBAPEAUCRuntimeClass__XZ(const CFtpFileFind* pThis) {
    return pThis ? pThis->GetRuntimeClass() : CFtpFileFind::GetThisClass();
}
// Symbol: ?GetThisClass@CFtpFileFind@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CFtpFileFind__SAPEAUCRuntimeClass__XZ() {
    return CFtpFileFind::GetThisClass();
}
CFtpFileFind::CFtpFileFind(CFtpConnection* pFtpConnection, DWORD_PTR dwContext)
    : CFileFind(), m_pFtpConnection(pFtpConnection), m_dwContext(dwContext),
      m_hFindHandle(nullptr) {
    memset(&m_findFileData, 0, sizeof(m_findFileData));
    memset(_ftpfifind_padding, 0, sizeof(_ftpfifind_padding));
}
CFtpFileFind::~CFtpFileFind() { Close(); }
int CFtpFileFind::FindFile(const wchar_t* pstrName, DWORD dwFlags) {
    (void)dwFlags;
    Close();
    if (!m_pFtpConnection || !m_pFtpConnection->m_hConnection) return 0;
    m_hFindHandle = ::FtpFindFirstFileW(m_pFtpConnection->m_hConnection,
                                         pstrName, &m_findFileData, 
                                         INTERNET_FLAG_RELOAD, (DWORD_PTR)m_dwContext);
    if (!m_hFindHandle) return 0;
    m_strFileName = m_findFileData.cFileName;
    m_findData = m_findFileData;
    m_bGotFirst = 1;
    return 1;
}
int CFtpFileFind::FindNextFile() {
    if (!m_hFindHandle) return 0;
    if (m_bGotFirst) {
        m_bGotFirst = 0;
        m_strFileName = m_findFileData.cFileName;
        m_findData = m_findFileData;
        return 1;
    }
    if (!::InternetFindNextFileW(m_hFindHandle, &m_findFileData)) return 0;
    m_strFileName = m_findFileData.cFileName;
    m_findData = m_findFileData;
    return 1;
}
void CFtpFileFind::Close() {
    if (m_hFindHandle) {
        ::InternetCloseHandle(m_hFindHandle);
        m_hFindHandle = nullptr;
    }
    CFileFind::Close();
}
CString CFtpFileFind::GetFileURL() const {
    if (m_pFtpConnection)
        return L"ftp://" + m_pFtpConnection->GetServerName() + L"/" + m_strFileName;
    return m_strFileName;
}
