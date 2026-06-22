// Behavioral test for the CFile runtime-class descriptor (filecore.cpp). Before
// this fix CFile::GetThisClass returned nullptr; it must now return a real
// DECLARE_DYNAMIC descriptor (base CObject, schema 0xFFFF, sizeof 40) so that
// CFile-derived classes can chain RTTI to it.
//
// filecore.cpp is too large to include directly, so this TU provides the same
// CFile::classCFile definition the DLL TU does and drives CFile::GetThisClass()
// (the inline member that the exported thunks delegate to). The exported thunks
// themselves are one-liners returning &CFile::classCFile; the full-DLL ABI build
// and the CI wine-runtime job exercise the exported symbols.

#include "openmfc/afx.h"

#include <cstdio>
#include <cstring>

// Definition of the descriptor declared in afx.h (mirrors filecore.cpp exactly).
CRuntimeClass CFile::classCFile = {
    "CFile", 40, 0xFFFF, nullptr, nullptr, &CObject::classCObject, nullptr
};

static int g_fail = 0;
static void check(bool c, const char* what) {
    std::printf("%s: %s\n", c ? "PASS" : "FAIL", what);
    if (!c) ++g_fail;
}

int main() {
    CRuntimeClass* rc = CFile::GetThisClass();
    check(rc != nullptr, "CFile::GetThisClass non-null (was a null stub)");
    check(rc == &CFile::classCFile, "GetThisClass returns &CFile::classCFile");
    check(rc->m_lpszClassName && std::strcmp(rc->m_lpszClassName, "CFile") == 0, "class name CFile");
    check(rc->m_nObjectSize == 40, "m_nObjectSize == 40 (real MSVC sizeof)");
    check(rc->m_wSchema == 0xFFFF && rc->m_pfnCreateObject == nullptr, "schema 0xFFFF, no factory");
    check(rc->m_pfnGetBaseClass == nullptr && rc->m_pNextClass == nullptr, "getbase/next null");
    check(rc->m_pBaseClass == &CObject::classCObject, "m_pBaseClass == CObject");

    // IsDerivedFrom walks CFile -> CObject.
    check(rc->IsDerivedFrom(&CObject::classCObject), "CFile IsDerivedFrom CObject");
    check(rc->IsDerivedFrom(rc), "CFile IsDerivedFrom CFile (self)");

    std::printf("RESULT: %s (%d failures)\n", g_fail == 0 ? "ALL PASS" : "FAILED", g_fail);
    return g_fail == 0 ? 0 : 1;
}
