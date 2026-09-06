// CDataRecoveryHandler — OpenMFC implementation.
// Sources: collections_strings.cpp, manual_small_stub_implementations.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/CollectionsStringsSupport.h"
#include "detail/ManualSmallStubImplementationsSupport.h"

// Symbol: ?DeleteAutosavedFile@CDataRecoveryHandler@@UEAAHAEBV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@@Z
extern "C" int MS_ABI impl__DeleteAutosavedFile_CDataRecoveryHandler__UEAAHAEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
    void*, const CString* fileName) {
    const wchar_t* path = CStringText(fileName);
    if (!path || !*path) return FALSE;
    if (DeleteFileW(path)) return TRUE;
    return GetLastError() == ERROR_FILE_NOT_FOUND ? TRUE : FALSE;
}
// Symbol: ?GenerateAutosaveFileName@CDataRecoveryHandler@@UEBA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@AEBV23@@Z
extern "C" void MS_ABI impl__GenerateAutosaveFileName_CDataRecoveryHandler__UEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__AEBV23__Z(
    CString* ret, void*, const CString* documentName) {
    wchar_t tempPath[MAX_PATH] = {};
    if (!GetTempPathW(MAX_PATH, tempPath)) tempPath[0] = L'\0';
    std::wstring fileName = SanitizedFileName(CStringText(documentName));
    new (ret) CString((std::wstring(tempPath) + fileName + L".autosave").c_str());
}
// Symbol: ?GetDocumentListName@CDataRecoveryHandler@@UEBA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@PEAVCDocument@@@Z
extern "C" void MS_ABI impl__GetDocumentListName_CDataRecoveryHandler__UEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__PEAVCDocument___Z(
    CString* ret, void*, CDocument* document) {
    new (ret) CString(DocumentTitleOrPathName(document));
}
// Symbol: ?GetNormalDocumentTitle@CDataRecoveryHandler@@UEBA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@PEAVCDocument@@@Z
extern "C" void MS_ABI impl__GetNormalDocumentTitle_CDataRecoveryHandler__UEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__PEAVCDocument___Z(
    CString* ret, void*, CDocument* document) {
    new (ret) CString(DocumentTitleOrPathName(document));
}
// Symbol: ?GetRecoveredDocumentTitle@CDataRecoveryHandler@@UEBA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@AEBV23@@Z
extern "C" void MS_ABI impl__GetRecoveredDocumentTitle_CDataRecoveryHandler__UEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__AEBV23__Z(
    CString* ret, void*, const CString* documentName) {
    CString title = CStringText(documentName);
    title += L" [Recovered]";
    new (ret) CString(title);
}
// Symbol: ??0CDataRecoveryHandler@@QEAA@KH@Z
extern "C" void* MS_ABI impl___0CDataRecoveryHandler__QEAA_KH_Z(
    void* pThis, unsigned long long unusedFlags, int nInterval) {
    (void)unusedFlags;
    (void)nInterval;
    return pThis;
}
// Symbol: ??1CDataRecoveryHandler@@UEAA@XZ
extern "C" void MS_ABI impl___1CDataRecoveryHandler__UEAA_XZ(void* pThis) {
    (void)pThis;
}
// Symbol: ?AutosaveAllDocumentInfo@CDataRecoveryHandler@@UEAAHXZ
extern "C" int MS_ABI impl__AutosaveAllDocumentInfo_CDataRecoveryHandler__UEAAHXZ() {
    return 0;
}

// Symbol: ?AutosaveDocumentInfo@CDataRecoveryHandler@@UEAAHPEAVCDocument@@H@Z
extern "C" int MS_ABI impl__AutosaveDocumentInfo_CDataRecoveryHandler__UEAAHPEAVCDocument__H_Z(void* /*class*/* p0, int p1) {
    return 0;
}

// Symbol: ?CreateDocumentInfo@CDataRecoveryHandler@@UEAAHPEAVCDocument@@@Z
extern "C" int MS_ABI impl__CreateDocumentInfo_CDataRecoveryHandler__UEAAHPEAVCDocument___Z(void* /*class*/* p0) {
    return 0;
}

// Symbol: ?DeleteAllAutosavedFiles@CDataRecoveryHandler@@UEAAHXZ
extern "C" int MS_ABI impl__DeleteAllAutosavedFiles_CDataRecoveryHandler__UEAAHXZ() {
    return 0;
}

// Symbol: ?Initialize@CDataRecoveryHandler@@UEAAHXZ
extern "C" int MS_ABI impl__Initialize_CDataRecoveryHandler__UEAAHXZ() {
    return 0;
}

// Symbol: ?QueryRestoreAutosavedDocuments@CDataRecoveryHandler@@UEAAXXZ
extern "C" void MS_ABI impl__QueryRestoreAutosavedDocuments_CDataRecoveryHandler__UEAAXXZ() {}

// Symbol: ?ReadOpenDocumentList@CDataRecoveryHandler@@UEAAHXZ
extern "C" int MS_ABI impl__ReadOpenDocumentList_CDataRecoveryHandler__UEAAHXZ() {
    return 0;
}

// Symbol: ?RemoveDocumentInfo@CDataRecoveryHandler@@UEAAHPEAVCDocument@@@Z
extern "C" int MS_ABI impl__RemoveDocumentInfo_CDataRecoveryHandler__UEAAHPEAVCDocument___Z(void* /*class*/* p0) {
    return 0;
}

// Symbol: ?ReopenPreviousDocuments@CDataRecoveryHandler@@UEAAHXZ
extern "C" int MS_ABI impl__ReopenPreviousDocuments_CDataRecoveryHandler__UEAAHXZ() {
    return 0;
}

// Symbol: ?RestoreAutosavedDocuments@CDataRecoveryHandler@@UEAAHXZ
extern "C" int MS_ABI impl__RestoreAutosavedDocuments_CDataRecoveryHandler__UEAAHXZ() {
    return 0;
}

// Symbol: ?SaveOpenDocumentList@CDataRecoveryHandler@@UEAAHXZ
extern "C" int MS_ABI impl__SaveOpenDocumentList_CDataRecoveryHandler__UEAAHXZ() {
    return 0;
}

// Symbol: ?SetAutosaveInterval@CDataRecoveryHandler@@UEAAXH@Z
extern "C" void MS_ABI impl__SetAutosaveInterval_CDataRecoveryHandler__UEAAXH_Z(int p0) {}

// Symbol: ?UpdateDocumentInfo@CDataRecoveryHandler@@UEAAHPEAVCDocument@@@Z
extern "C" int MS_ABI impl__UpdateDocumentInfo_CDataRecoveryHandler__UEAAHPEAVCDocument___Z(void* /*class*/* p0) {
    return 0;
}

