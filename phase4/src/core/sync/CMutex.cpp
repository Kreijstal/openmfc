// CMutex — OpenMFC implementation.
// Sources: synccore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/SynccoreSupport.h"

// Symbol: ?GetRuntimeClass@CMutex@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI
impl__GetRuntimeClass_CMutex__UEBAPEAUCRuntimeClass__XZ(
    const CMutex* pThis) {
    return &CMutex::classCMutex;
}
// Symbol: ?GetThisClass@CMutex@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI
impl__GetThisClass_CMutex__SAPEAUCRuntimeClass__XZ() {
    return &CMutex::classCMutex;
}
// Symbol: ??0CMutex@@QEAA@HPEB_WPEAU_SECURITY_ATTRIBUTES@@@Z
extern "C" void* MS_ABI
impl___0CMutex__QEAA_HPEB_WPEAU_SECURITY_ATTRIBUTES___Z(
    void* pThis, int p0, const wchar_t* p1, LPSECURITY_ATTRIBUTES p2) {
    if (!pThis) {
        return nullptr;
    }
    return new(pThis) CMutex(p0, p1, p2);
}
CMutex::CMutex(BOOL bInitiallyOwn, const wchar_t* lpszName,
               LPSECURITY_ATTRIBUTES lpsaAttribute)
    : CSyncObject(lpszName) {
    m_hObject = ::CreateMutexW(lpsaAttribute, bInitiallyOwn, lpszName);
}
CMutex::~CMutex() {
    // Base class destructor closes handle
}
BOOL CMutex::Unlock() {
    return ::ReleaseMutex(m_hObject);
}
