// CEvent — OpenMFC implementation.
// Sources: synccore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/SynccoreSupport.h"

// Symbol: ?GetRuntimeClass@CEvent@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI
impl__GetRuntimeClass_CEvent__UEBAPEAUCRuntimeClass__XZ(
    const CEvent* pThis) {
    return &CEvent::classCEvent;
}
// Symbol: ?GetThisClass@CEvent@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI
impl__GetThisClass_CEvent__SAPEAUCRuntimeClass__XZ() {
    return &CEvent::classCEvent;
}
// Symbol: ??0CEvent@@QEAA@HHPEB_WPEAU_SECURITY_ATTRIBUTES@@@Z
extern "C" void* MS_ABI
impl___0CEvent__QEAA_HHPEB_WPEAU_SECURITY_ATTRIBUTES___Z(
    void* pThis, int p0, int p1, const wchar_t* p2, LPSECURITY_ATTRIBUTES p3) {
    if (!pThis) {
        return nullptr;
    }
    return new(pThis) CEvent(p0, p1, p2, p3);
}
CEvent::CEvent(BOOL bInitiallyOwn, BOOL bManualReset,
               const wchar_t* lpszName,
               LPSECURITY_ATTRIBUTES lpsaAttribute)
    : CSyncObject(lpszName) {
    m_hObject = ::CreateEventW(lpsaAttribute, bManualReset, bInitiallyOwn, lpszName);
}
CEvent::~CEvent() {
    // Base class destructor closes handle
}
BOOL CEvent::SetEvent() {
    return ::SetEvent(m_hObject);
}
BOOL CEvent::PulseEvent() {
    return ::PulseEvent(m_hObject);
}
BOOL CEvent::ResetEvent() {
    return ::ResetEvent(m_hObject);
}
BOOL CEvent::Unlock() {
    return TRUE;  // Events don't need unlock
}
