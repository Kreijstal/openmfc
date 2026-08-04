// CSocketWnd — OpenMFC implementation.
// Sources: sockcore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/SockcoreSupport.h"

// Symbol: ??0CSocketWnd@@QEAA@XZ
extern "C" void* MS_ABI impl___0CSocketWnd__QEAA_XZ(CSocketWnd* pThis) {
    new(pThis) CSocketWnd();
    return pThis;
}
// Symbol: ?GetMessageMap@CSocketWnd@@MEBAPEBUAFX_MSGMAP@@XZ
extern "C" const AFX_MSGMAP* MS_ABI impl__GetMessageMap_CSocketWnd__MEBAPEBUAFX_MSGMAP__XZ(
        const CSocketWnd* pThis) {
    return pThis->_OpenMfcGetMessageMap();
}
// Symbol: ?GetThisMessageMap@CSocketWnd@@KAPEBUAFX_MSGMAP@@XZ
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CSocketWnd__KAPEBUAFX_MSGMAP__XZ() {
    return CSocketWnd::_OpenMfcGetThisMessageMap();
}
// Symbol: ?OnSocketNotify@CSocketWnd@@IEAA_J_K_J@Z
extern "C" LRESULT MS_ABI impl__OnSocketNotify_CSocketWnd__IEAA_J_K_J_Z(
        CSocketWnd* pThis, WPARAM wParam, LPARAM lParam) {
    return pThis->_OpenMfcOnSocketNotify(wParam, lParam);
}
// Symbol: ?OnSocketDead@CSocketWnd@@IEAA_J_K_J@Z
extern "C" LRESULT MS_ABI impl__OnSocketDead_CSocketWnd__IEAA_J_K_J_Z(
        CSocketWnd* pThis, WPARAM wParam, LPARAM lParam) {
    return pThis->_OpenMfcOnSocketDead(wParam, lParam);
}
CSocketWnd::CSocketWnd() : CWnd() {
}
LRESULT CSocketWnd::OnSocketNotify(WPARAM wParam, LPARAM lParam) {
    SOCKET hSocket = (SOCKET)wParam;
    long lEvent = (long)lParam;
    CAsyncSocket* pSocket = CAsyncSocket::LookupHandle(hSocket, FALSE);
    if (pSocket) {
        CAsyncSocket::DoCallBack(hSocket, lEvent);
    }
    return 0;
}
LRESULT CSocketWnd::OnSocketDead(WPARAM wParam, LPARAM lParam) {
    SOCKET hSocket = (SOCKET)wParam;
    (void)lParam;
    CAsyncSocket* pSocket = CAsyncSocket::LookupHandle(hSocket, TRUE);
    if (pSocket) {
        pSocket->Close();
    }
    return 0;
}
