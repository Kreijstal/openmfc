// CSemaphore — OpenMFC implementation.
// Sources: synccore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/SynccoreSupport.h"

// Symbol: ?GetRuntimeClass@CSemaphore@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI
impl__GetRuntimeClass_CSemaphore__UEBAPEAUCRuntimeClass__XZ(
    const CSemaphore* pThis) {
    return &CSemaphore::classCSemaphore;
}
// Symbol: ?GetThisClass@CSemaphore@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI
impl__GetThisClass_CSemaphore__SAPEAUCRuntimeClass__XZ() {
    return &CSemaphore::classCSemaphore;
}
// Symbol: ??0CSemaphore@@QEAA@JJPEB_WPEAU_SECURITY_ATTRIBUTES@@@Z
extern "C" void* MS_ABI
impl___0CSemaphore__QEAA_JJPEB_WPEAU_SECURITY_ATTRIBUTES___Z(
    void* pThis, long p0, long p1, const wchar_t* p2, LPSECURITY_ATTRIBUTES p3) {
    if (!pThis) {
        return nullptr;
    }
    return new(pThis) CSemaphore(p0, p1, p2, p3);
}
CSemaphore::CSemaphore(LONG lInitialCount, LONG lMaxCount,
                       const wchar_t* pstrName,
                       LPSECURITY_ATTRIBUTES lpsaAttributes)
    : CSyncObject(pstrName) {
    m_hObject = ::CreateSemaphoreW(lpsaAttributes, lInitialCount, lMaxCount, pstrName);
}
CSemaphore::~CSemaphore() {
    // Base class destructor closes handle
}
BOOL CSemaphore::Unlock() {
    return Unlock(1, nullptr);
}
BOOL CSemaphore::Unlock(LONG lCount, LPLONG lpPrevCount) {
    return ::ReleaseSemaphore(m_hObject, lCount, lpPrevCount);
}
