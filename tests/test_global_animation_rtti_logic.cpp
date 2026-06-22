// Behavioral test for the CAnimation* RTTI getters (global_animation_rtti.cpp).
//
// Includes the impl .cpp directly and drives the exported impl_ thunks. Verifies
// each descriptor's name / object size, that GetRuntimeClass returns the same
// static descriptor as GetThisClass (no self-dispatch recursion), and that
// m_pBaseClass equals the descriptor the base class's own GetThisClass returns,
// so IsKindOf walks an unbroken graph up to CObject.

#include "../phase4/src/global_animation_rtti.cpp"

#include <cstdio>
#include <cstring>

// Base CRuntimeClass statics dragged in by afxwin.h's class graph; defined here
// only so the standalone test TU links (the CAnimation* chain itself only
// references CObject::classCObject, which afx.h defines inline).
CRuntimeClass CCmdTarget::classCCmdTarget{};
CRuntimeClass CWinThread::classCWinThread{};
CRuntimeClass CWinApp::classCWinApp{};
CRuntimeClass CWnd::classCWnd{};

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
    CRuntimeClass* (MS_ABI *baseGetThis)();  // null => chains straight to CObject
};

int main() {
    Case cases[] = {
        { impl__GetThisClass_CAnimationBaseObject__SAPEAUCRuntimeClass__XZ,
          impl__GetRuntimeClass_CAnimationBaseObject__UEBAPEAUCRuntimeClass__XZ,
          "CAnimationBaseObject", 40, nullptr },
        { impl__GetThisClass_CAnimationController__SAPEAUCRuntimeClass__XZ,
          impl__GetRuntimeClass_CAnimationController__UEBAPEAUCRuntimeClass__XZ,
          "CAnimationController", 112, nullptr },
        { impl__GetThisClass_CAnimationValue__SAPEAUCRuntimeClass__XZ,
          impl__GetRuntimeClass_CAnimationValue__UEBAPEAUCRuntimeClass__XZ,
          "CAnimationValue", 136, impl__GetThisClass_CAnimationBaseObject__SAPEAUCRuntimeClass__XZ },
        { impl__GetThisClass_CAnimationColor__SAPEAUCRuntimeClass__XZ,
          impl__GetRuntimeClass_CAnimationColor__UEBAPEAUCRuntimeClass__XZ,
          "CAnimationColor", 328, impl__GetThisClass_CAnimationBaseObject__SAPEAUCRuntimeClass__XZ },
        { impl__GetThisClass_CAnimationPoint__SAPEAUCRuntimeClass__XZ,
          impl__GetRuntimeClass_CAnimationPoint__UEBAPEAUCRuntimeClass__XZ,
          "CAnimationPoint", 232, impl__GetThisClass_CAnimationBaseObject__SAPEAUCRuntimeClass__XZ },
        { impl__GetThisClass_CAnimationRect__SAPEAUCRuntimeClass__XZ,
          impl__GetRuntimeClass_CAnimationRect__UEBAPEAUCRuntimeClass__XZ,
          "CAnimationRect", 440, impl__GetThisClass_CAnimationBaseObject__SAPEAUCRuntimeClass__XZ },
        { impl__GetThisClass_CAnimationSize__SAPEAUCRuntimeClass__XZ,
          impl__GetRuntimeClass_CAnimationSize__UEBAPEAUCRuntimeClass__XZ,
          "CAnimationSize", 232, impl__GetThisClass_CAnimationBaseObject__SAPEAUCRuntimeClass__XZ },
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
        std::snprintf(buf, sizeof(buf), "%s: schema 0xFFFF (DYNAMIC, no factory)", c.name);
        check(rc->m_wSchema == 0xFFFF && rc->m_pfnCreateObject == nullptr, buf);
        std::snprintf(buf, sizeof(buf), "%s: GetRuntimeClass == GetThisClass", c.name);
        check(c.getRC(nullptr) == rc, buf);
        std::snprintf(buf, sizeof(buf), "%s: m_pBaseClass chains correctly", c.name);
        check(rc->m_pBaseClass == (c.baseGetThis ? c.baseGetThis() : CObject::GetThisClass()), buf);
    }

    // Full IsDerivedFrom walk: CAnimationColor -> CAnimationBaseObject -> CObject,
    // but not CAnimationController (a sibling under CObject).
    CRuntimeClass* color = impl__GetThisClass_CAnimationColor__SAPEAUCRuntimeClass__XZ();
    check(color->IsDerivedFrom(impl__GetThisClass_CAnimationBaseObject__SAPEAUCRuntimeClass__XZ()),
          "CAnimationColor IsDerivedFrom CAnimationBaseObject");
    check(color->IsDerivedFrom(CObject::GetThisClass()),
          "CAnimationColor IsDerivedFrom CObject");
    check(!color->IsDerivedFrom(impl__GetThisClass_CAnimationController__SAPEAUCRuntimeClass__XZ()),
          "CAnimationColor is NOT derived from CAnimationController");

    std::printf("RESULT: %s (%d failures)\n", g_fail == 0 ? "ALL PASS" : "FAILED", g_fail);
    return g_fail == 0 ? 0 : 1;
}
