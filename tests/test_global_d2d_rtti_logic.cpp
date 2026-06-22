// Behavioral test for the CD2D* Direct2D RTTI getters (global_d2d_rtti.cpp).
//
// Includes the impl .cpp directly and drives the exported impl_ thunks. Verifies
// each descriptor's name / object size, that GetRuntimeClass returns the same
// static descriptor as GetThisClass (no self-dispatch recursion), and that the
// m_pBaseClass chain matches the harvested CD2D* hierarchy — including that each
// derived class points at the *same* descriptor its base class's GetThisClass
// returns, so IsKindOf walks an unbroken graph up to CObject.

#include "../phase4/src/global_d2d_rtti.cpp"

#include <cstdio>
#include <cstring>

// Base CRuntimeClass statics dragged in by afxwin.h's class graph; defined here
// only so the standalone test TU links (the CD2D* chain itself only references
// CObject::classCObject, which afx.h defines inline).
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
    // GetThisClass of the base class — the descriptor m_pBaseClass must equal.
    CRuntimeClass* (MS_ABI *baseGetThis)();
    // Only the root chains straight to CObject (no in-batch baseGetThis).
    bool           baseIsCObject;
};

int main() {
    Case cases[] = {
        { impl__GetThisClass_CD2DResource__SAPEAUCRuntimeClass__XZ,
          impl__GetRuntimeClass_CD2DResource__UEBAPEAUCRuntimeClass__XZ,
          "CD2DResource", 24, nullptr, true },
        { impl__GetThisClass_CD2DBrush__SAPEAUCRuntimeClass__XZ,
          impl__GetRuntimeClass_CD2DBrush__UEBAPEAUCRuntimeClass__XZ,
          "CD2DBrush", 40, impl__GetThisClass_CD2DResource__SAPEAUCRuntimeClass__XZ, false },
        { impl__GetThisClass_CD2DGeometry__SAPEAUCRuntimeClass__XZ,
          impl__GetRuntimeClass_CD2DGeometry__UEBAPEAUCRuntimeClass__XZ,
          "CD2DGeometry", 32, impl__GetThisClass_CD2DResource__SAPEAUCRuntimeClass__XZ, false },
        { impl__GetThisClass_CD2DBitmap__SAPEAUCRuntimeClass__XZ,
          impl__GetRuntimeClass_CD2DBitmap__UEBAPEAUCRuntimeClass__XZ,
          "CD2DBitmap", 80, impl__GetThisClass_CD2DResource__SAPEAUCRuntimeClass__XZ, false },
        { impl__GetThisClass_CD2DLayer__SAPEAUCRuntimeClass__XZ,
          impl__GetRuntimeClass_CD2DLayer__UEBAPEAUCRuntimeClass__XZ,
          "CD2DLayer", 32, impl__GetThisClass_CD2DResource__SAPEAUCRuntimeClass__XZ, false },
        { impl__GetThisClass_CD2DMesh__SAPEAUCRuntimeClass__XZ,
          impl__GetRuntimeClass_CD2DMesh__UEBAPEAUCRuntimeClass__XZ,
          "CD2DMesh", 32, impl__GetThisClass_CD2DResource__SAPEAUCRuntimeClass__XZ, false },
        { impl__GetThisClass_CD2DTextFormat__SAPEAUCRuntimeClass__XZ,
          impl__GetRuntimeClass_CD2DTextFormat__UEBAPEAUCRuntimeClass__XZ,
          "CD2DTextFormat", 32, impl__GetThisClass_CD2DResource__SAPEAUCRuntimeClass__XZ, false },
        { impl__GetThisClass_CD2DTextLayout__SAPEAUCRuntimeClass__XZ,
          impl__GetRuntimeClass_CD2DTextLayout__UEBAPEAUCRuntimeClass__XZ,
          "CD2DTextLayout", 32, impl__GetThisClass_CD2DResource__SAPEAUCRuntimeClass__XZ, false },
        { impl__GetThisClass_CD2DSolidColorBrush__SAPEAUCRuntimeClass__XZ,
          impl__GetRuntimeClass_CD2DSolidColorBrush__UEBAPEAUCRuntimeClass__XZ,
          "CD2DSolidColorBrush", 64, impl__GetThisClass_CD2DBrush__SAPEAUCRuntimeClass__XZ, false },
        { impl__GetThisClass_CD2DGradientBrush__SAPEAUCRuntimeClass__XZ,
          impl__GetRuntimeClass_CD2DGradientBrush__UEBAPEAUCRuntimeClass__XZ,
          "CD2DGradientBrush", 96, impl__GetThisClass_CD2DBrush__SAPEAUCRuntimeClass__XZ, false },
        { impl__GetThisClass_CD2DBitmapBrush__SAPEAUCRuntimeClass__XZ,
          impl__GetRuntimeClass_CD2DBitmapBrush__UEBAPEAUCRuntimeClass__XZ,
          "CD2DBitmapBrush", 64, impl__GetThisClass_CD2DBrush__SAPEAUCRuntimeClass__XZ, false },
        { impl__GetThisClass_CD2DPathGeometry__SAPEAUCRuntimeClass__XZ,
          impl__GetRuntimeClass_CD2DPathGeometry__UEBAPEAUCRuntimeClass__XZ,
          "CD2DPathGeometry", 40, impl__GetThisClass_CD2DGeometry__SAPEAUCRuntimeClass__XZ, false },
        { impl__GetThisClass_CD2DLinearGradientBrush__SAPEAUCRuntimeClass__XZ,
          impl__GetRuntimeClass_CD2DLinearGradientBrush__UEBAPEAUCRuntimeClass__XZ,
          "CD2DLinearGradientBrush", 120, impl__GetThisClass_CD2DGradientBrush__SAPEAUCRuntimeClass__XZ, false },
        { impl__GetThisClass_CD2DRadialGradientBrush__SAPEAUCRuntimeClass__XZ,
          impl__GetRuntimeClass_CD2DRadialGradientBrush__UEBAPEAUCRuntimeClass__XZ,
          "CD2DRadialGradientBrush", 128, impl__GetThisClass_CD2DGradientBrush__SAPEAUCRuntimeClass__XZ, false },
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
        if (c.baseIsCObject) {
            check(rc->m_pBaseClass == CObject::GetThisClass(), buf);
        } else {
            check(rc->m_pBaseClass == c.baseGetThis(), buf);
        }
    }

    // Spot-check a full IsDerivedFrom walk: CD2DRadialGradientBrush should be
    // kind-of CD2DGradientBrush, CD2DBrush, CD2DResource and CObject, but not
    // CD2DBitmap.
    CRuntimeClass* radial = impl__GetThisClass_CD2DRadialGradientBrush__SAPEAUCRuntimeClass__XZ();
    check(radial->IsDerivedFrom(impl__GetThisClass_CD2DGradientBrush__SAPEAUCRuntimeClass__XZ()),
          "CD2DRadialGradientBrush IsDerivedFrom CD2DGradientBrush");
    check(radial->IsDerivedFrom(impl__GetThisClass_CD2DBrush__SAPEAUCRuntimeClass__XZ()),
          "CD2DRadialGradientBrush IsDerivedFrom CD2DBrush");
    check(radial->IsDerivedFrom(impl__GetThisClass_CD2DResource__SAPEAUCRuntimeClass__XZ()),
          "CD2DRadialGradientBrush IsDerivedFrom CD2DResource");
    check(radial->IsDerivedFrom(CObject::GetThisClass()),
          "CD2DRadialGradientBrush IsDerivedFrom CObject");
    check(!radial->IsDerivedFrom(impl__GetThisClass_CD2DBitmap__SAPEAUCRuntimeClass__XZ()),
          "CD2DRadialGradientBrush is NOT derived from CD2DBitmap");

    std::printf("RESULT: %s (%d failures)\n", g_fail == 0 ? "ALL PASS" : "FAILED", g_fail);
    return g_fail == 0 ? 0 : 1;
}
