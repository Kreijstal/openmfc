// Verify the actual production OLE file runtime-class providers and ancestry.
#include "openmfc/afxole.h"
#include <cstdio>
#include <cstring>
#ifndef MS_ABI
#define MS_ABI
#endif
extern "C" CRuntimeClass* impl__GetThisClass_COleStreamFile__SAPEAUCRuntimeClass__XZ();
extern "C" CRuntimeClass* impl__GetRuntimeClass_COleStreamFile__UEBAPEAUCRuntimeClass__XZ(const void*);
extern "C" CRuntimeClass* impl__GetThisClass_CMonikerFile__SAPEAUCRuntimeClass__XZ();
extern "C" CRuntimeClass* impl__GetRuntimeClass_CMonikerFile__UEBAPEAUCRuntimeClass__XZ(const void*);
extern "C" CRuntimeClass* impl__GetThisClass_CAsyncMonikerFile__SAPEAUCRuntimeClass__XZ();
extern "C" CRuntimeClass* impl__GetRuntimeClass_CAsyncMonikerFile__UEBAPEAUCRuntimeClass__XZ(const void*);
extern "C" CRuntimeClass* impl__GetThisClass_CDataPathProperty__SAPEAUCRuntimeClass__XZ();
extern "C" CRuntimeClass* impl__GetRuntimeClass_CDataPathProperty__UEBAPEAUCRuntimeClass__XZ(const void*);
extern "C" CRuntimeClass* impl__GetThisClass_CCachedDataPathProperty__SAPEAUCRuntimeClass__XZ();
extern "C" CRuntimeClass* impl__GetRuntimeClass_CCachedDataPathProperty__UEBAPEAUCRuntimeClass__XZ(const void*);
static int g_fail = 0;
static void check(bool c, const char* what) {
    std::printf("%s: %s\n", c ? "PASS" : "FAIL", what);
    if (!c) ++g_fail;
}

struct Case {
    CRuntimeClass* (MS_ABI *getThis)();
    CRuntimeClass* (MS_ABI *getRC)(const void*);
    const char*    name;
    int            size;
    CRuntimeClass* base;
};

int VerifyOleFileRuntimeClasses() {
    Case cases[] = {
        { impl__GetThisClass_COleStreamFile__SAPEAUCRuntimeClass__XZ,
          impl__GetRuntimeClass_COleStreamFile__UEBAPEAUCRuntimeClass__XZ,
          "COleStreamFile", 56, &CFile::classCFile },
        { impl__GetThisClass_CMonikerFile__SAPEAUCRuntimeClass__XZ,
          impl__GetRuntimeClass_CMonikerFile__UEBAPEAUCRuntimeClass__XZ,
          "CMonikerFile", 64, impl__GetThisClass_COleStreamFile__SAPEAUCRuntimeClass__XZ() },
        { impl__GetThisClass_CAsyncMonikerFile__SAPEAUCRuntimeClass__XZ,
          impl__GetRuntimeClass_CAsyncMonikerFile__UEBAPEAUCRuntimeClass__XZ,
          "CAsyncMonikerFile", 96, impl__GetThisClass_CMonikerFile__SAPEAUCRuntimeClass__XZ() },
        { impl__GetThisClass_CDataPathProperty__SAPEAUCRuntimeClass__XZ,
          impl__GetRuntimeClass_CDataPathProperty__UEBAPEAUCRuntimeClass__XZ,
          "CDataPathProperty", 112, impl__GetThisClass_CAsyncMonikerFile__SAPEAUCRuntimeClass__XZ() },
        { impl__GetThisClass_CCachedDataPathProperty__SAPEAUCRuntimeClass__XZ,
          impl__GetRuntimeClass_CCachedDataPathProperty__UEBAPEAUCRuntimeClass__XZ,
          "CCachedDataPathProperty", 200, impl__GetThisClass_CDataPathProperty__SAPEAUCRuntimeClass__XZ() },
    };

    for (const Case& c : cases) {
        CRuntimeClass* rc = c.getThis();
        char buf[112];
        std::snprintf(buf, sizeof(buf), "%s: GetThisClass non-null", c.name);
        check(rc != nullptr, buf);
        if (!rc) continue;
        std::snprintf(buf, sizeof(buf), "%s: class name", c.name);
        check(rc->m_lpszClassName && std::strcmp(rc->m_lpszClassName, c.name) == 0, buf);
        std::snprintf(buf, sizeof(buf), "%s: m_nObjectSize == %d", c.name, c.size);
        check(rc->m_nObjectSize == c.size, buf);
        std::snprintf(buf, sizeof(buf), "%s: schema 0xFFFF, no factory", c.name);
        check(rc->m_wSchema == 0xFFFF && rc->m_pfnCreateObject == nullptr, buf);
        std::snprintf(buf, sizeof(buf), "%s: DLL base getter is present, list/init links are null", c.name);
        check(rc->m_pfnGetBaseClass != nullptr && rc->m_pNextClass == nullptr && rc->m_pClassInit == nullptr, buf);
        std::snprintf(buf, sizeof(buf), "%s: GetRuntimeClass == GetThisClass", c.name);
        check(c.getRC(nullptr) == rc, buf);
        std::snprintf(buf, sizeof(buf), "%s: the base-class link chains correctly", c.name);
        check(rc->BaseClass() == c.base, buf);
    }

    // CCachedDataPathProperty -> CDataPathProperty -> CAsyncMonikerFile ->
    // CMonikerFile -> COleStreamFile -> CFile -> CObject: IsDerivedFrom walks the
    // whole in-file chain onto the external CFile/CObject members.
    CRuntimeClass* cdp = impl__GetThisClass_CCachedDataPathProperty__SAPEAUCRuntimeClass__XZ();
    check(cdp->IsDerivedFrom(impl__GetThisClass_CDataPathProperty__SAPEAUCRuntimeClass__XZ()),
          "CCachedDataPathProperty IsDerivedFrom CDataPathProperty");
    check(cdp->IsDerivedFrom(impl__GetThisClass_COleStreamFile__SAPEAUCRuntimeClass__XZ()),
          "CCachedDataPathProperty IsDerivedFrom COleStreamFile");
    check(cdp->IsDerivedFrom(&CFile::classCFile),
          "CCachedDataPathProperty IsDerivedFrom CFile");
    check(cdp->IsDerivedFrom(&CObject::classCObject),
          "CCachedDataPathProperty IsDerivedFrom CObject");

    std::printf("RESULT: %s (%d failures)\n", g_fail == 0 ? "ALL PASS" : "FAILED", g_fail);
    return g_fail == 0 ? 0 : 1;
}

#ifndef OPENMFC_TEST_OLEFILE_RTTI_HELPER
int main() { return VerifyOleFileRuntimeClasses(); }
#endif
