// COleMessageFilter — OpenMFC implementation.
// Sources: olecore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/OlecoreSupport.h"

// Symbol: ?HandleInComingCall@XMessageFilter@COleMessageFilter@@UEAAKKPEAUHTASK__@@KPEAUtagINTERFACEINFO@@@Z
// Ordinal: 7564
extern "C" unsigned long MS_ABI impl__HandleInComingCall_XMessageFilter_COleMessageFilter__UEAAKKPEAUHTASK____KPEAUtagINTERFACEINFO___Z(
    COleMessageFilter::XMessageFilter* pThis, unsigned long dwCallType,
    HTASK__* htaskCaller, unsigned long dwTickCount,
    tagINTERFACEINFO* lpInterfaceInfo) {
    return pThis->HandleInComingCall(dwCallType, htaskCaller, dwTickCount, lpInterfaceInfo);
}
// Symbol: ?IsSignificantMessage@COleMessageFilter@@UEAAHPEAUtagMSG@@@Z
// Ordinal: 8006
extern "C" int MS_ABI impl__IsSignificantMessage_COleMessageFilter__UEAAHPEAUtagMSG___Z(
    COleMessageFilter* pThis, tagMSG* pMsg) {
    return (int)pThis->IsSignificantMessage(pMsg);
}
// Symbol: ?MessagePending@XMessageFilter@COleMessageFilter@@UEAAKPEAUHTASK__@@KK@Z
// Ordinal: 8450
extern "C" unsigned long MS_ABI impl__MessagePending_XMessageFilter_COleMessageFilter__UEAAKPEAUHTASK____KK_Z(
    COleMessageFilter::XMessageFilter* pThis, HTASK__* htaskCallee,
    unsigned long dwTickCount, unsigned long dwType) {
    return pThis->MessagePending(htaskCallee, dwTickCount, dwType);
}
// Symbol: ?OnNotRespondingDialog@COleMessageFilter@@UEAAHPEAUHTASK__@@@Z
// Ordinal: 10692
extern "C" int MS_ABI impl__OnNotRespondingDialog_COleMessageFilter__UEAAHPEAUHTASK_____Z(
    COleMessageFilter* pThis, HTASK__* hTaskBusy) {
    return (int)pThis->OnNotRespondingDialog(hTaskBusy);
}
// Symbol: ?RetryRejectedCall@XMessageFilter@COleMessageFilter@@UEAAKPEAUHTASK__@@KK@Z
// Ordinal: 12562
extern "C" unsigned long MS_ABI impl__RetryRejectedCall_XMessageFilter_COleMessageFilter__UEAAKPEAUHTASK____KK_Z(
    COleMessageFilter::XMessageFilter* pThis, HTASK__* htaskCallee,
    unsigned long dwTickCount, unsigned long dwRejectType) {
    return pThis->RetryRejectedCall(htaskCallee, dwTickCount, dwRejectType);
}
COleMessageFilter::COleMessageFilter()
    : m_nBusyCount(0), m_bEnableBusy(TRUE), m_bEnableNotResponding(TRUE),
      m_nBusyReply(SERVERCALL_RETRYLATER), m_nRetryReply(0), m_nTimeout(5000) {
    memset(_olemessagefilter_padding, 0, sizeof(_olemessagefilter_padding));
    m_xMessageFilter.m_pMessageFilter = this;
}
COleMessageFilter::~COleMessageFilter() {
    if (g_pMessageFilter == this) {
        COleMessageFilter::Revoke();
        g_pMessageFilter = nullptr;
    } else {
        Revoke();
    }
}
int COleMessageFilter::Register() {
    if (g_messageFilterAdapter) {
        CoRegisterMessageFilter(g_previousMessageFilter, nullptr);
        g_messageFilterAdapter->Release();
        g_messageFilterAdapter = nullptr;
        if (g_previousMessageFilter) {
            g_previousMessageFilter->Release();
            g_previousMessageFilter = nullptr;
        }
    }

    g_messageFilterAdapter = new(std::nothrow) OleMessageFilterAdapter(this);
    if (!g_messageFilterAdapter) return FALSE;
    IMessageFilter* oldFilter = nullptr;
    HRESULT hr = CoRegisterMessageFilter(g_messageFilterAdapter, &oldFilter);
    if (FAILED(hr)) {
        g_messageFilterAdapter->Release();
        g_messageFilterAdapter = nullptr;
        return FALSE;
    }
    g_previousMessageFilter = oldFilter;
    return TRUE;
}
void COleMessageFilter::Revoke() {
    IMessageFilter* oldFilter = nullptr;
    CoRegisterMessageFilter(g_previousMessageFilter, &oldFilter);
    if (oldFilter) oldFilter->Release();
    if (g_previousMessageFilter) {
        g_previousMessageFilter->Release();
        g_previousMessageFilter = nullptr;
    }
    if (g_messageFilterAdapter) {
        g_messageFilterAdapter->Release();
        g_messageFilterAdapter = nullptr;
    }
}
BOOL COleMessageFilter::EnableBusyDialog(BOOL bEnableBusy) {
    BOOL bOld = m_bEnableBusy;
    m_bEnableBusy = bEnableBusy;
    return bOld;
}
BOOL COleMessageFilter::EnableNotRespondingDialog(BOOL bEnableNotResponding) {
    BOOL bOld = m_bEnableNotResponding;
    m_bEnableNotResponding = bEnableNotResponding;
    return bOld;
}
void COleMessageFilter::SetBusyReply(SERVERCALL nBusyReply) {
    m_nBusyReply = nBusyReply;
}
void COleMessageFilter::SetRetryReply(DWORD nRetryReply) {
    m_nRetryReply = nRetryReply;
}
void COleMessageFilter::SetMessagePendingDelay(DWORD nTimeout) {
    m_nTimeout = (nTimeout >= 0) ? nTimeout : 5000;
}
void COleMessageFilter::BeginBusyState() {
    m_nBusyCount++;
}
void COleMessageFilter::EndBusyState() {
    if (m_nBusyCount > 0) m_nBusyCount--;
}
BOOL COleMessageFilter::IsBusy() const {
    return m_nBusyCount > 0;
}
BOOL COleMessageFilter::OnMessagePending(const MSG* pMsg) {
    return FALSE;
}
int COleMessageFilter::OnBusyDialog(HTASK hTaskBusy) {
    return -1;
}
BOOL COleMessageFilter::IsSignificantMessage(MSG* pMsg) {
    if (!pMsg) return FALSE;
    return pMsg->message == WM_TIMER || pMsg->message == WM_PAINT ||
           pMsg->message == WM_MOUSEMOVE;
}
int COleMessageFilter::OnNotRespondingDialog(HTASK hTaskBusy) {
    (void)hTaskBusy;
    if (!m_bEnableNotResponding) return 250;
    return m_nRetryReply != 0 ? (int)m_nRetryReply : 250;
}
STDMETHODIMP COleMessageFilter::XMessageFilter::QueryInterface(REFIID riid, void** ppv) {
    if (!ppv) return E_POINTER;
    *ppv = nullptr;
    if (IsEqualIID(riid, IID_IUnknown) || IsEqualIID(riid, IID_IMessageFilter)) {
        *ppv = this;
        AddRef();
        return S_OK;
    }
    return E_NOINTERFACE;
}
