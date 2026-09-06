// CRuntimeClass — OpenMFC implementation.
// Sources: appcore.cpp, cobject_impl.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/CWinAppSupport.h"
#include "detail/CRuntimeClassSupport.h"

// Implemented in cobject_impl.cpp; this unit had a second, unlinked copy.
// Symbol: ?CreateObject@CRuntimeClass@@QEAAPEAVCObject@@XZ
// Implemented in cobject_impl.cpp; this unit had a second, unlinked copy.
// Symbol: ?CreateObject@CRuntimeClass@@SAPEAVCObject@@PEBD@Z
// Implemented in cobject_impl.cpp; this unit had a second, unlinked copy.
// Symbol: ?CreateObject@CRuntimeClass@@SAPEAVCObject@@PEB_W@Z
// Implemented in cobject_impl.cpp; this unit had a second, unlinked copy.
// Symbol: ?FromName@CRuntimeClass@@SAPEAU1@PEBD@Z
// Implemented in cobject_impl.cpp; this unit had a second, unlinked copy.
// Symbol: ?FromName@CRuntimeClass@@SAPEAU1@PEB_W@Z
// Implemented in cobject_impl.cpp; this unit had a second, unlinked copy.
// Symbol: ?IsDerivedFrom@CRuntimeClass@@QEBAHPEBU1@@Z
// Implemented in cobject_impl.cpp; this unit had a second, unlinked copy.
// Symbol: ?Load@CRuntimeClass@@SAPEAU1@AEAVCArchive@@PEAI@Z
// Implemented in cobject_impl.cpp; this unit had a second, unlinked copy.
// Symbol: ?Store@CRuntimeClass@@QEBAXAEAVCArchive@@@Z
// Symbol: ?CreateObject@CRuntimeClass@@QEAAPEAVCObject@@XZ
extern "C" CObject* MS_ABI impl__CreateObject_CRuntimeClass__QEAAPEAVCObject__XZ(
    CRuntimeClass* pThis  // RCX = this (CRuntimeClass*)
) {
    if (!pThis || !pThis->m_pfnCreateObject) {
        return nullptr;
    }
    return pThis->m_pfnCreateObject();
}
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
// Symbol: ?IsDerivedFrom@CRuntimeClass@@QEBAHPEBU1@@Z
extern "C" int MS_ABI impl__IsDerivedFrom_CRuntimeClass__QEBAHPEBU1__Z(
    const CRuntimeClass* pThis,     // RCX = this (CRuntimeClass*)
    const CRuntimeClass* pBaseClass // RDX = base class to check
) {
    if (!pThis || !pBaseClass) {
        return FALSE;
    }

    // Walk the inheritance chain
    const CRuntimeClass* pClass = pThis;
    while (pClass != nullptr) {
        if (pClass == pBaseClass) {
            return TRUE;
        }
        pClass = pClass->m_pfnGetBaseClass ? pClass->m_pfnGetBaseClass() : nullptr;
    }

    return FALSE;
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
// Symbol: ?Store@CRuntimeClass@@QEBAXAEAVCArchive@@@Z
extern "C" void MS_ABI impl__Store_CRuntimeClass__QEBAXAEAVCArchive___Z(
    const CRuntimeClass* pThis,  // RCX = this
    CArchive* ar                 // RDX = archive
) {
    if (!pThis || !ar || !ar->IsStoring()) return;

    // Write schema number
    unsigned short wSchema = (unsigned short)pThis->m_wSchema;
    *ar << wSchema;

    // Write class name
    if (pThis->m_lpszClassName) {
        size_t nLen = strlen(pThis->m_lpszClassName);
        if (nLen > 64) nLen = 64;

        unsigned short wNameLen = (unsigned short)nLen;
        *ar << wNameLen;

        ar->Write(pThis->m_lpszClassName, (UINT)nLen);
    } else {
        unsigned short wNameLen = 0;
        *ar << wNameLen;
    }
}
