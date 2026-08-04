// CSyncObject — OpenMFC implementation.
// Sources: synccore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/SynccoreSupport.h"

// Symbol: ?GetRuntimeClass@CSyncObject@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI
impl__GetRuntimeClass_CSyncObject__UEBAPEAUCRuntimeClass__XZ(
    const CSyncObject* pThis) {
    return &CSyncObject::classCSyncObject;
}
// Symbol: ?GetThisClass@CSyncObject@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI
impl__GetThisClass_CSyncObject__SAPEAUCRuntimeClass__XZ() {
    return &CSyncObject::classCSyncObject;
}
// Symbol: ??0CSyncObject@@QEAA@PEB_W@Z
CSyncObject::CSyncObject(const wchar_t* pstrName)
    : m_hObject(nullptr) {
    if (pstrName != nullptr) {
        m_strName = pstrName;
    }
}
CSyncObject::~CSyncObject() {
    if (m_hObject != nullptr) {
        ::CloseHandle(m_hObject);
        m_hObject = nullptr;
    }
}
BOOL CSyncObject::Lock(DWORD dwTimeout) {
    if (m_hObject == nullptr) {
        return FALSE;
    }
    DWORD dwRet = ::WaitForSingleObject(m_hObject, dwTimeout);
    return (dwRet == WAIT_OBJECT_0 || dwRet == WAIT_ABANDONED);
}
BOOL CSyncObject::Unlock() {
    return TRUE;
}
BOOL CSyncObject::Unlock(LONG /* lCount */, LPLONG /* lpPrevCount */) {
    // Default implementation - derived classes override
    return TRUE;
}
