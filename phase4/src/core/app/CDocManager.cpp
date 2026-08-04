// CDocManager — OpenMFC implementation.
// Sources: appcore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/CWinAppSupport.h"

// Symbol: ??0CDocManager@@QEAA@XZ
extern "C" void* MS_ABI impl___0CDocManager__QEAA_XZ(void* pThis) {
    if (pThis) g_docManagerStates[reinterpret_cast<CDocManager*>(pThis)] = DocManagerState{};
    return pThis;
}
// Symbol: ??1CDocManager@@UEAA@XZ
extern "C" void MS_ABI impl___1CDocManager__UEAA_XZ(CDocManager* pThis) {
    if (pThis) g_docManagerStates.erase(pThis);
}
// Symbol: ?AddDocTemplate@CDocManager@@UEAAXPEAVCDocTemplate@@@Z
extern "C" void MS_ABI impl__AddDocTemplate_CDocManager__UEAAXPEAVCDocTemplate___Z(CDocManager* pThis, CDocTemplate* pTemplate) {
    if (!pThis || !pTemplate) return;
    auto& templates = g_docManagerStates[pThis].templates;
    if (std::find(templates.begin(), templates.end(), pTemplate) == templates.end()) {
        templates.push_back(pTemplate);
    }
}
// Symbol: ?GetBestTemplate@CDocManager@@UEAAPEAVCDocTemplate@@PEB_W@Z
extern "C" CDocTemplate* MS_ABI impl__GetBestTemplate_CDocManager__UEAAPEAVCDocTemplate__PEB_W_Z(
    CDocManager* pThis, const wchar_t* pathName) {
    (void)pathName;
    if (!pThis) return nullptr;
    auto& templates = g_docManagerStates[pThis].templates;
    return templates.empty() ? nullptr : templates.front();
}
// Symbol: ?GetFirstDocTemplatePosition@CDocManager@@UEBAPEAU__POSITION@@XZ
extern "C" void* MS_ABI impl__GetFirstDocTemplatePosition_CDocManager__UEBAPEAU__POSITION__XZ(const CDocManager* pThis) {
    if (!pThis) return nullptr;
    const auto& templates = g_docManagerStates[pThis].templates;
    return templates.empty() ? nullptr : reinterpret_cast<void*>(static_cast<uintptr_t>(1));
}
// Symbol: ?GetNextDocTemplate@CDocManager@@UEBAPEAVCDocTemplate@@AEAPEAU__POSITION@@@Z
extern "C" CDocTemplate* MS_ABI impl__GetNextDocTemplate_CDocManager__UEBAPEAVCDocTemplate__AEAPEAU__POSITION___Z(
    const CDocManager* pThis, void** pos) {
    if (!pThis || !pos || !*pos) return nullptr;
    const auto& templates = g_docManagerStates[pThis].templates;
    const uintptr_t index = reinterpret_cast<uintptr_t>(*pos) - 1;
    if (index >= templates.size()) {
        *pos = nullptr;
        return nullptr;
    }
    *pos = (index + 1 < templates.size()) ? reinterpret_cast<void*>(index + 2) : nullptr;
    return templates[index];
}
// Symbol: ?GetOpenDocumentCount@CDocManager@@UEAAHXZ
extern "C" int MS_ABI impl__GetOpenDocumentCount_CDocManager__UEAAHXZ(CDocManager* pThis) {
    if (!pThis) return 0;
    int total = 0;
    for (CDocTemplate* tpl : g_docManagerStates[pThis].templates) {
        total += CountTemplateDocuments(tpl);
    }
    return total;
}
// Symbol: ?GetDocumentCount@CDocManager@@IEAAHXZ
extern "C" int MS_ABI impl__GetDocumentCount_CDocManager__IEAAHXZ(CDocManager* pThis) {
    if (!pThis) return 0;
    int total = 0;
    for (CDocTemplate* tpl : g_docManagerStates[pThis].templates) {
        total += CountTemplateDocuments(tpl);
    }
    return total;
}
// Symbol: ?GetRuntimeClass@CDocManager@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CDocManager__UEBAPEAUCRuntimeClass__XZ(const CDocManager* pThis) {
    (void)pThis;
    return &g_classCDocManager;
}
// Symbol: ?GetThisClass@CDocManager@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CDocManager__SAPEAUCRuntimeClass__XZ() {
    return &g_classCDocManager;
}
// Symbol: ?OpenDocumentFile@CDocManager@@UEAAPEAVCDocument@@PEB_W@Z
extern "C" CDocument* MS_ABI impl__OpenDocumentFile_CDocManager__UEAAPEAVCDocument__PEB_W_Z(
    CDocManager* pThis, const wchar_t* pathName) {
    if (!pThis) return nullptr;
    return OpenWithTemplates(g_docManagerStates[pThis].templates, pathName, TRUE);
}
// Symbol: ?OpenDocumentFile@CDocManager@@UEAAPEAVCDocument@@PEB_WH@Z
extern "C" CDocument* MS_ABI impl__OpenDocumentFile_CDocManager__UEAAPEAVCDocument__PEB_WH_Z(
    CDocManager* pThis, const wchar_t* pathName, int bMakeVisible) {
    if (!pThis) return nullptr;
    return OpenWithTemplates(g_docManagerStates[pThis].templates, pathName, bMakeVisible);
}
// Symbol: ?OnFileNew@CDocManager@@UEAAXXZ
extern "C" void MS_ABI impl__OnFileNew_CDocManager__UEAAXXZ(CDocManager* pThis) {
    if (!pThis) return;
    (void)OpenWithTemplates(g_docManagerStates[pThis].templates, nullptr, TRUE);
}
// Symbol: ?OnFileOpen@CDocManager@@UEAAXXZ
extern "C" void MS_ABI impl__OnFileOpen_CDocManager__UEAAXXZ(CDocManager* pThis) {
    if (!pThis) return;
    CFileDialog dlg(
        TRUE, nullptr, nullptr, OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST, nullptr, nullptr);
    if (dlg.DoModal() != IDOK) {
        return;
    }
    (void)OpenWithTemplates(g_docManagerStates[pThis].templates, static_cast<const wchar_t*>(dlg.GetPathName()), TRUE);
}
// Symbol: ?SaveAllModified@CDocManager@@UEAAHXZ
extern "C" int MS_ABI impl__SaveAllModified_CDocManager__UEAAHXZ(CDocManager* pThis) {
    if (!pThis) return TRUE;
    for (CDocTemplate* tpl : g_docManagerStates[pThis].templates) {
        if (!SaveTemplateDocuments(tpl)) return FALSE;
    }
    return TRUE;
}
// Symbol: ?CloseAllDocuments@CDocManager@@UEAAXH@Z
extern "C" void MS_ABI impl__CloseAllDocuments_CDocManager__UEAAXH_Z(CDocManager* pThis, int bEndSession) {
    (void)bEndSession;
    if (!pThis) return;
    for (CDocTemplate* tpl : g_docManagerStates[pThis].templates) {
        CloseTemplateDocuments(tpl);
    }
}
// Symbol: ?DoPromptFileName@CDocManager@@UEAAHAEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@IKHPEAVCDocTemplate@@@Z
extern "C" int MS_ABI impl__DoPromptFileName_CDocManager__UEAAHAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__IKHPEAVCDocTemplate___Z(
    CDocManager* pThis, CString* fileName, unsigned int idTitle, unsigned long flags, int bOpenFileDialog, CDocTemplate* pTemplate) {
    (void)pTemplate;
    (void)idTitle;
    if (!pThis || !fileName) return FALSE;
    CFileDialog dlg(
        bOpenFileDialog, nullptr, static_cast<const wchar_t*>(*fileName), flags, nullptr, nullptr);
    if (dlg.DoModal() != IDOK) return FALSE;
    *fileName = dlg.GetPathName();
    return TRUE;
}
// Symbol: ?OnDDECommand@CDocManager@@UEAAHPEA_W@Z
extern "C" int MS_ABI impl__OnDDECommand_CDocManager__UEAAHPEA_W_Z(CDocManager* pThis, wchar_t* command) {
    if (!pThis || !command || !command[0]) return FALSE;
    const wchar_t* path = command;
    std::vector<std::wstring> tokens = TokenizeCommandLine(command);
    if (!tokens.empty()) {
        if (_wcsicmp(tokens[0].c_str(), L"open") == 0 && tokens.size() >= 2) {
            path = tokens[1].c_str();
        } else {
            path = tokens[0].c_str();
        }
    }
    return OpenWithTemplates(g_docManagerStates[pThis].templates, path, TRUE) ? TRUE : FALSE;
}
// Symbol: ?RegisterShellFileTypes@CDocManager@@UEAAXH@Z
extern "C" void MS_ABI impl__RegisterShellFileTypes_CDocManager__UEAAXH_Z(CDocManager* pThis, int bCompat) {
    if (!pThis) return;
    g_docManagerStates[pThis].shellFileTypesRegistered = true;
    (void)bCompat;
}
// Symbol: ?UnregisterShellFileTypes@CDocManager@@QEAAXXZ
extern "C" void MS_ABI impl__UnregisterShellFileTypes_CDocManager__QEAAXXZ(CDocManager* pThis) {
    if (!pThis) return;
    g_docManagerStates[pThis].shellFileTypesRegistered = false;
}
