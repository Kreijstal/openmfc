// CRuntimeClass — OpenMFC implementation.
// Sources: appcore.cpp, cobject_impl.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/CWinAppSupport.h"
#include "detail/CRuntimeClassSupport.h"

// Implemented in cobject_impl.cpp; this unit had a second, unlinked copy.
// Instance factory creation lives in RuntimeClassFactory.cpp.
// Symbol: ?CreateObject@CRuntimeClass@@SAPEAVCObject@@PEBD@Z
// Implemented in cobject_impl.cpp; this unit had a second, unlinked copy.
// Symbol: ?CreateObject@CRuntimeClass@@SAPEAVCObject@@PEB_W@Z
// Implemented in cobject_impl.cpp; this unit had a second, unlinked copy.
// Symbol: ?FromName@CRuntimeClass@@SAPEAU1@PEBD@Z
// Implemented in cobject_impl.cpp; this unit had a second, unlinked copy.
// Symbol: ?FromName@CRuntimeClass@@SAPEAU1@PEB_W@Z
// Implemented in cobject_impl.cpp; this unit had a second, unlinked copy.
// Implemented in cobject_impl.cpp; this unit had a second, unlinked copy.
// Symbol: ?Load@CRuntimeClass@@SAPEAU1@AEAVCArchive@@PEAI@Z
// Implemented in cobject_impl.cpp; this unit had a second, unlinked copy.
// Symbol: ?CreateObject@CRuntimeClass@@SAPEAVCObject@@PEB_W@Z
extern "C" CObject* MS_ABI impl__CreateObject_CRuntimeClass__SAPEAVCObject__PEB_W_Z(
    const wchar_t* lpszClassName  // RCX = class name (wide)
) {
    if (!lpszClassName) {
        return nullptr;
    }

    InitializeClasses();

    char narrowName[256];
    size_t i = 0;
    while (lpszClassName[i] && i < sizeof(narrowName) - 1) {
        wchar_t ch = lpszClassName[i];
        narrowName[i] = (ch >= 0 && ch <= 0x7f) ? static_cast<char>(ch) : '?';
        ++i;
    }
    narrowName[i] = '\0';

    // Search the registered classes
    for (CRuntimeClass* pClass : GetClassRegistry()) {
        if (pClass->m_lpszClassName && strcmp(pClass->m_lpszClassName, narrowName) == 0) {
            if (pClass->m_pfnCreateObject) {
                return pClass->m_pfnCreateObject();
            }
            return nullptr;
        }
    }

    return nullptr;
}
// Symbol: ?CreateObject@CRuntimeClass@@SAPEAVCObject@@PEBD@Z
extern "C" CObject* MS_ABI impl__CreateObject_CRuntimeClass__SAPEAVCObject__PEBD_Z(
    const char* lpszClassName  // RCX = class name (narrow)
) {
    if (!lpszClassName) {
        return nullptr;
    }

    InitializeClasses();

    // Search the registered classes
    for (CRuntimeClass* pClass : GetClassRegistry()) {
        if (pClass->m_lpszClassName && strcmp(pClass->m_lpszClassName, lpszClassName) == 0) {
            if (pClass->m_pfnCreateObject) {
                return pClass->m_pfnCreateObject();
            }
            return nullptr;
        }
    }

    return nullptr;
}
// Symbol: ?FromName@CRuntimeClass@@SAPEAU1@PEB_W@Z
extern "C" CRuntimeClass* MS_ABI impl__FromName_CRuntimeClass__SAPEAU1_PEB_W_Z(
    const wchar_t* lpszClassName  // RCX = class name (wide)
) {
    if (!lpszClassName) {
        return nullptr;
    }

    InitializeClasses();

    char narrowName[256];
    size_t i = 0;
    while (lpszClassName[i] && i < sizeof(narrowName) - 1) {
        wchar_t ch = lpszClassName[i];
        narrowName[i] = (ch >= 0 && ch <= 0x7f) ? static_cast<char>(ch) : '?';
        ++i;
    }
    narrowName[i] = '\0';

    // Search the registered classes
    for (CRuntimeClass* pClass : GetClassRegistry()) {
        if (pClass->m_lpszClassName && strcmp(pClass->m_lpszClassName, narrowName) == 0) {
            return pClass;
        }
    }

    return nullptr;
}
// Symbol: ?FromName@CRuntimeClass@@SAPEAU1@PEBD@Z
extern "C" CRuntimeClass* MS_ABI impl__FromName_CRuntimeClass__SAPEAU1_PEBD_Z(
    const char* lpszClassName  // RCX = class name (narrow)
) {
    if (!lpszClassName) {
        return nullptr;
    }

    InitializeClasses();

    // Search the registered classes
    for (CRuntimeClass* pClass : GetClassRegistry()) {
        if (pClass->m_lpszClassName && strcmp(pClass->m_lpszClassName, lpszClassName) == 0) {
            return pClass;
        }
    }

    return nullptr;
}
// Symbol: ?Load@CRuntimeClass@@SAPEAU1@AEAVCArchive@@PEAI@Z
extern "C" CRuntimeClass* MS_ABI impl__Load_CRuntimeClass__SAPEAU1_AEAVCArchive__PEAI_Z(
    CArchive* ar,              // RCX = archive
    unsigned int* pwSchemaNum  // RDX = schema number output
) {
    if (!ar || !ar->IsLoading()) return nullptr;

    // Read schema number
    unsigned short wSchema = 0;
    *ar >> wSchema;

    if (pwSchemaNum) {
        *pwSchemaNum = wSchema;
    }

    // Read class name length
    unsigned short wNameLen = 0;
    *ar >> wNameLen;

    if (wNameLen == 0 || wNameLen > 64) {
        return nullptr;  // Invalid name length
    }

    // Read class name
    char szClassName[65];
    ar->Read(szClassName, wNameLen);
    szClassName[wNameLen] = '\0';

    // Look up the class
    InitializeClasses();
    CRuntimeClass* pClass = FindRuntimeClass(szClassName);

    return pClass;
}
