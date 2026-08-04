// CSharedFile — OpenMFC implementation.
// Sources: file_csharedfile.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/CSharedFileSupport.h"

// Symbol: ??0CSharedFile@@QEAA@II@Z
extern "C" void MS_ABI impl___0CSharedFile__QEAA_II_Z(
    void* pThis, unsigned int nAllocFlags, unsigned int nGrowBytes) {
    if (!pThis) return;
    new (pThis) CSharedFile(nAllocFlags, nGrowBytes);
}
// Symbol: ??1CSharedFile@@UEAA@XZ
extern "C" void MS_ABI impl___1CSharedFile__UEAA_XZ(void* pThis) {
    if (!pThis) return;
    static_cast<CSharedFile*>(pThis)->~CSharedFile();
}
// Symbol: ?Alloc@CSharedFile@@MEAAPEAE_K@Z
extern "C" unsigned char* MS_ABI impl__Alloc_CSharedFile__MEAAPEAE_K_Z(
    void* pThis, unsigned long long nBytes) {
    if (!pThis) return nullptr;
    return CSharedFileAccessor::CallAlloc(static_cast<CSharedFile*>(pThis), static_cast<SIZE_T>(nBytes));
}
// Symbol: ?Detach@CSharedFile@@QEAAPEAXXZ
extern "C" void* MS_ABI impl__Detach_CSharedFile__QEAAPEAXXZ(void* pThis) {
    if (!pThis) return nullptr;
    return static_cast<void*>(static_cast<CSharedFile*>(pThis)->Detach());
}
// Symbol: ?Free@CSharedFile@@MEAAXPEAE@Z
extern "C" void MS_ABI impl__Free_CSharedFile__MEAAXPEAE_Z(
    void* pThis, unsigned char* lpMem) {
    if (!pThis) return;
    CSharedFileAccessor::CallFree(static_cast<CSharedFile*>(pThis), lpMem);
}
// Symbol: ?GetRuntimeClass@CSharedFile@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CSharedFile__UEBAPEAUCRuntimeClass__XZ(
    const void* pThis) {
    (void)pThis;
    return &g_classCSharedFile;
}
// Symbol: ?GetThisClass@CSharedFile@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CSharedFile__SAPEAUCRuntimeClass__XZ() {
    return &g_classCSharedFile;
}
// Symbol: ?Realloc@CSharedFile@@MEAAPEAEPEAE_K@Z
extern "C" unsigned char* MS_ABI impl__Realloc_CSharedFile__MEAAPEAEPEAE_K_Z(
    void* pThis, unsigned char* lpMem, unsigned long long nBytes) {
    if (!pThis) return nullptr;
    return CSharedFileAccessor::CallRealloc(static_cast<CSharedFile*>(pThis), lpMem, static_cast<SIZE_T>(nBytes));
}
// Symbol: ?SetHandle@CSharedFile@@QEAAXPEAXH@Z
extern "C" void MS_ABI impl__SetHandle_CSharedFile__QEAAXPEAXH_Z(
    void* pThis, void* hGlobalMemory, int bAllowGrow) {
    if (!pThis) return;
    static_cast<CSharedFile*>(pThis)->SetHandle(
        static_cast<HGLOBAL>(hGlobalMemory), bAllowGrow);
}
