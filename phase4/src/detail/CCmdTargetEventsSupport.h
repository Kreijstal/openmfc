#pragma once
#include "detail/CCmdTargetDispatchSupport.h"

// AFX_EVENT (afxpriv2.h).  Only the first two members are read by
// GetEventSinkEntry; the rest are carried so the offsets stay honest.
struct AFX_EVENT {
    int   m_eventKind;      // +0x00   event / propRequest / propChanged / propDSCNotify
    long  m_dispid;         // +0x04
    void* m_pDispParams;    // +0x08
    void* m_pExcepInfo;
    unsigned int* m_puArgError;
    int   m_bPropChanged;
    long  m_hResult;
    int   m_nDSCState;
    int   m_nDSCReason;
};
static_assert(offsetof(AFX_EVENT, m_eventKind) == 0x00, "AFX_EVENT::m_eventKind at +0x00");
static_assert(offsetof(AFX_EVENT, m_dispid) == 0x04, "AFX_EVENT::m_dispid at +0x04");

static_assert(offsetof(AFX_EVENT, m_hResult) == 0x24, "native event HRESULT offset");
static_assert(sizeof(AFX_EVENT) == 0x30, "native event storage");
extern "C" const void* MS_ABI impl__GetEventSinkEntry_CCmdTarget__IEAAPEBUAFX_EVENTSINKMAP_ENTRY__IPEAUAFX_EVENT___Z(CCmdTarget*, unsigned int, void*);
extern "C" int MS_ABI impl__OnEvent_CCmdTarget__QEAAHIPEAUAFX_EVENT__PEAUAFX_CMDHANDLERINFO___Z(CCmdTarget*, unsigned int, void*, void*);
