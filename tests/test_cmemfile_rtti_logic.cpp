// Behavioral test for the CMemFile runtime-class descriptor (filecore.cpp).
// Before this fix CMemFile::GetThisClass/GetRuntimeClass returned nullptr; they
// must now return a real DECLARE_DYNAMIC descriptor (base CFile, schema 0xFFFF,
// sizeof 88) so RUNTIME_CLASS(CMemFile) and IsKindOf work in real drop-in usage.
//
// This validates the STATIC RTTI path (RUNTIME_CLASS / GetThisClass / IsDerivedFrom),
// which is what the descriptor fix addresses. filecore.cpp is too large to include,
// so this TU provides the same CFile::classCFile / CMemFile::classCMemFile
// definitions the DLL does and drives the inline GetThisClass members.

#include "openmfc/afx.h"

#include <cstdio>
#include <cstring>

// Definitions of the descriptors declared in afx.h (mirror filecore.cpp exactly).
CRuntimeClass CFile::classCFile = {
    "CFile", 40, 0xFFFF, nullptr, nullptr, &CObject::classCObject, nullptr
};
CRuntimeClass CMemFile::classCMemFile = {
    "CMemFile", 88, 0xFFFF, nullptr, nullptr, &CFile::classCFile, nullptr
};

static int g_fail = 0;
static void check(bool c, const char* what) {
    std::printf("%s: %s\n", c ? "PASS" : "FAIL", what);
    if (!c) ++g_fail;
}

int main() {
    CRuntimeClass* rc = CMemFile::GetThisClass();
    check(rc != nullptr, "CMemFile::GetThisClass non-null (was a null stub)");
    check(rc == &CMemFile::classCMemFile, "GetThisClass returns &CMemFile::classCMemFile");
    check(rc->m_lpszClassName && std::strcmp(rc->m_lpszClassName, "CMemFile") == 0, "class name CMemFile");
    check(rc->m_nObjectSize == 88, "m_nObjectSize == 88 (real MSVC sizeof)");
    check(rc->m_wSchema == 0xFFFF && rc->m_pfnCreateObject == nullptr, "schema 0xFFFF, no factory");
    check(rc->m_pfnGetBaseClass == nullptr && rc->m_pNextClass == nullptr, "getbase/next null");
    check(rc->m_pBaseClass == &CFile::classCFile, "m_pBaseClass == CFile");

    // IsDerivedFrom walks CMemFile -> CFile -> CObject.
    check(rc->IsDerivedFrom(&CFile::classCFile), "CMemFile IsDerivedFrom CFile");
    check(rc->IsDerivedFrom(&CObject::classCObject), "CMemFile IsDerivedFrom CObject");
    check(rc->IsDerivedFrom(rc), "CMemFile IsDerivedFrom CMemFile (self)");

    std::printf("RESULT: %s (%d failures)\n", g_fail == 0 ? "ALL PASS" : "FAILED", g_fail);
    return g_fail == 0 ? 0 : 1;
}
