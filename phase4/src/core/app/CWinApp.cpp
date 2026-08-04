// CWinApp — OpenMFC implementation.
// Sources: appcore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/CWinAppSupport.h"


// Implementations this unit calls that are defined with their own class.
extern "C" void MS_ABI impl__ParseLast_CCommandLineInfo__IEAAXH_Z(CCommandLineInfo* pThis, int bLast);
extern "C" void MS_ABI impl__ParseParam_CCommandLineInfo__UEAAXPEB_WHH_Z(
    CCommandLineInfo* pThis, const wchar_t* pszParam, int bFlag, int bLast);

// Symbol: ?GetThisClass@CWinApp@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CWinApp__SAPEAUCRuntimeClass__XZ() {
    return &CWinApp::classCWinApp;
}
// CWinApp::Run - exported lifecycle implementation
// Symbol: ?Run@CWinApp@@UEAAHXZ
// Ordinal: 12613
extern "C" int MS_ABI impl__Run_CWinApp__UEAAHXZ(CWinApp* pThis) {
    return OpenMfcRunExportedMessageLoop(pThis);
}
// CWinApp::InitInstance - exported lifecycle implementation
// Symbol: ?InitInstance@CWinApp@@UEAAHXZ
// Ordinal: 7726
extern "C" int MS_ABI impl__InitInstance_CWinApp__UEAAHXZ(CWinApp* pThis) {
    return pThis->InitInstance();
}
// CWinApp::ExitInstance - exported lifecycle implementation
// Symbol: ?ExitInstance@CWinApp@@UEAAHXZ
// Ordinal: 4455
extern "C" int MS_ABI impl__ExitInstance_CWinApp__UEAAHXZ(CWinApp* pThis) {
    return pThis->ExitInstance();
}
// CWinApp constructor
// Symbol: ??0CWinApp@@QEAA@PEB_W@Z
// Ordinal: 983
extern "C" CWinApp* MS_ABI impl___0CWinApp__QEAA_PEB_W_Z(CWinApp* pThis, const wchar_t* lpszAppName) {
    // Initialize base class (CWinThread)
    pThis->m_pMainWnd = nullptr;
    pThis->m_nThreadID = GetCurrentThreadId();
    pThis->m_hThread = GetCurrentThread();
    pThis->m_bAutoDelete = FALSE;  // App object should not auto-delete

    // Initialize CWinApp members
    pThis->m_hInstance = nullptr;
    pThis->m_lpCmdLine = nullptr;
    pThis->m_nCmdShow = SW_SHOW;
    pThis->m_pszAppName = lpszAppName;
    pThis->m_pszExeName = nullptr;
    pThis->m_pszHelpFilePath = nullptr;
    pThis->m_pszProfileName = nullptr;
    pThis->m_pszRegistryKey = nullptr;
    memset(&pThis->m_msgCur, 0, sizeof(pThis->m_msgCur));

    // Register as the global app instance
    g_pApp = pThis;
    return pThis;
}
// CWinApp destructor
// Symbol: ??1CWinApp@@UEAA@XZ
// Ordinal: 1450
extern "C" void MS_ABI impl___1CWinApp__UEAA_XZ(CWinApp* pThis) {
    if (g_pApp == pThis) {
        g_pApp = nullptr;
    }
}
// Symbol: ?ParseCommandLine@CWinApp@@QEAAXAEAVCCommandLineInfo@@@Z
extern "C" void MS_ABI impl__ParseCommandLine_CWinApp__QEAAXAEAVCCommandLineInfo___Z(
    CWinApp* pThis, CCommandLineInfo* pInfo) {
    if (!pThis || !pInfo) return;
    const std::vector<std::wstring> tokens = TokenizeCommandLine(pThis->m_lpCmdLine);
    for (size_t i = 0; i < tokens.size(); ++i) {
        const std::wstring& token = tokens[i];
        const bool isFlag = !token.empty() && (token[0] == L'/' || token[0] == L'-');
        const wchar_t* value = isFlag ? token.c_str() + 1 : token.c_str();
        impl__ParseParam_CCommandLineInfo__UEAAXPEB_WHH_Z(
            pInfo, value, isFlag ? TRUE : FALSE, (i + 1) == tokens.size() ? TRUE : FALSE);
    }
    if (tokens.empty()) {
        impl__ParseLast_CCommandLineInfo__IEAAXH_Z(pInfo, TRUE);
    }
}
// Symbol: ?ProcessShellCommand@CWinApp@@QEAAHAEAVCCommandLineInfo@@@Z
extern "C" int MS_ABI impl__ProcessShellCommand_CWinApp__QEAAHAEAVCCommandLineInfo___Z(
    CWinApp* pThis, CCommandLineInfo* pInfo) {
    if (!pThis || !pInfo) return FALSE;
    CommandLineInfoState& state = g_commandLineInfoStates[pInfo];
    AppRuntimeState& appState = g_appRuntimeStates[pThis];

    switch (state.shellCommand) {
    case kShellCommandFileNothing:
        return TRUE;
    case kShellCommandFileNew:
        return OpenWithTemplates(appState.templates, nullptr, TRUE) != nullptr;
    case kShellCommandFileOpen:
        return state.hasFileName && OpenWithTemplates(appState.templates, state.fileName.c_str(), TRUE) != nullptr;
    case kShellCommandAppRegister:
    case kShellCommandAppUnregister:
        return TRUE;
    default:
        return FALSE;
    }
}
// Symbol: ?LoadStdProfileSettings@CWinApp@@IEAAXI@Z
extern "C" void MS_ABI impl__LoadStdProfileSettings_CWinApp__IEAAXI_Z(CWinApp* pThis, unsigned int nMaxMRU) {
    if (!pThis) return;
    AppRuntimeState& state = g_appRuntimeStates[pThis];
    state.maxRecent = nMaxMRU;
    if (state.recentEntries.size() > nMaxMRU) {
        state.recentEntries.resize(nMaxMRU);
    }
}
// Symbol: ?AddToRecentFileList@CWinApp@@UEAAXPEB_W@Z
extern "C" void MS_ABI impl__AddToRecentFileList_CWinApp__UEAAXPEB_W_Z(CWinApp* pThis, const wchar_t* pathName) {
    if (!pThis) return;
    AppRuntimeState& state = g_appRuntimeStates[pThis];
    AddRecentPath(state.recentEntries, static_cast<int>(state.maxRecent), pathName);
}
// Symbol: ?GetProfileIntW@CWinApp@@UEAAIPEB_W0H@Z
extern "C" unsigned int MS_ABI impl__GetProfileIntW_CWinApp__UEAAIPEB_W0H_Z(
    CWinApp* pThis, const wchar_t* section, const wchar_t* entry, int defaultValue) {
    if (!pThis) return static_cast<unsigned int>(defaultValue);
    const auto& map = g_appRuntimeStates[pThis].profileInts;
    const auto it = map.find(MakeProfileKey(section, entry));
    return it == map.end() ? static_cast<unsigned int>(defaultValue) : it->second;
}
// Symbol: ?GetProfileStringW@CWinApp@@UEAA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@PEB_W00@Z
extern "C" void MS_ABI impl__GetProfileStringW_CWinApp__UEAA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__PEB_W00_Z(
    CString* ret, CWinApp* pThis, const wchar_t* section, const wchar_t* entry, const wchar_t* defaultValue) {
    if (!ret) return;
    if (!pThis) {
        new (ret) CString(defaultValue ? defaultValue : L"");
        return;
    }
    const auto& map = g_appRuntimeStates[pThis].profileStrings;
    const auto it = map.find(MakeProfileKey(section, entry));
    new (ret) CString(it == map.end() ? (defaultValue ? defaultValue : L"") : it->second.c_str());
}
// Symbol: ?GetProfileBinary@CWinApp@@UEAAHPEB_W0PEAPEAEPEAI@Z
extern "C" int MS_ABI impl__GetProfileBinary_CWinApp__UEAAHPEB_W0PEAPEAEPEAI_Z(
    CWinApp* pThis, const wchar_t* section, const wchar_t* entry, unsigned char** ppData, unsigned int* pBytes) {
    if (ppData) *ppData = nullptr;
    if (pBytes) *pBytes = 0;
    if (!pThis || !ppData || !pBytes) return FALSE;

    const auto& map = g_appRuntimeStates[pThis].profileBinary;
    const auto it = map.find(MakeProfileKey(section, entry));
    if (it == map.end()) return FALSE;

    const std::vector<unsigned char>& data = it->second;
    if (data.empty()) return FALSE;

    unsigned char* copy = new (std::nothrow) unsigned char[data.size()];
    if (!copy) return FALSE;
    std::memcpy(copy, data.data(), data.size());
    *ppData = copy;
    *pBytes = static_cast<unsigned int>(data.size());
    return TRUE;
}
// Symbol: ?WriteProfileInt@CWinApp@@UEAAHPEB_W0H@Z
extern "C" int MS_ABI impl__WriteProfileInt_CWinApp__UEAAHPEB_W0H_Z(
    CWinApp* pThis, const wchar_t* section, const wchar_t* entry, int value) {
    if (!pThis || !section || !entry) return FALSE;
    g_appRuntimeStates[pThis].profileInts[MakeProfileKey(section, entry)] = static_cast<unsigned int>(value);
    return TRUE;
}
// Symbol: ?WriteProfileStringW@CWinApp@@UEAAHPEB_W00@Z
extern "C" int MS_ABI impl__WriteProfileStringW_CWinApp__UEAAHPEB_W00_Z(
    CWinApp* pThis, const wchar_t* section, const wchar_t* entry, const wchar_t* value) {
    if (!pThis || !section || !entry) return FALSE;
    g_appRuntimeStates[pThis].profileStrings[MakeProfileKey(section, entry)] = value ? value : L"";
    return TRUE;
}
// Symbol: ?WriteProfileBinary@CWinApp@@UEAAHPEB_W0PEAEI@Z
extern "C" int MS_ABI impl__WriteProfileBinary_CWinApp__UEAAHPEB_W0PEAEI_Z(
    CWinApp* pThis, const wchar_t* section, const wchar_t* entry, unsigned char* data, unsigned int bytes) {
    if (!pThis || !section || !entry) return FALSE;
    std::vector<unsigned char> blob;
    if (data && bytes != 0) blob.assign(data, data + bytes);
    g_appRuntimeStates[pThis].profileBinary[MakeProfileKey(section, entry)] = std::move(blob);
    return TRUE;
}
// Symbol: ?DelRegTree@CWinApp@@QEAAJPEAUHKEY__@@AEBV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@PEAVCAtlTransactionManager@4@@Z
extern "C" long MS_ABI impl__DelRegTree_CWinApp__QEAAJPEAUHKEY____AEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__PEAVCAtlTransactionManager_4__Z(
    CWinApp* pThis, HKEY hParentKey, const CString* keyName, void*) {
    (void)pThis;
    if (!hParentKey || !keyName) return ERROR_INVALID_PARAMETER;
    return ::RegDeleteTreeW(hParentKey, static_cast<const wchar_t*>(*keyName));
}
// Symbol: ?DoPromptFileName@CWinApp@@QEAAHAEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@IKHPEAVCDocTemplate@@@Z
extern "C" int MS_ABI impl__DoPromptFileName_CWinApp__QEAAHAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__IKHPEAVCDocTemplate___Z(
    CWinApp* pThis, CString* fileName, unsigned int, unsigned long flags, int bOpenFileDialog, CDocTemplate*) {
    if (!fileName) return FALSE;
    CFileDialog dlg(bOpenFileDialog, nullptr, static_cast<const wchar_t*>(*fileName), flags, nullptr, pThis ? pThis->m_pMainWnd : nullptr);
    if (dlg.DoModal() != IDOK) return FALSE;
    *fileName = dlg.GetPathName();
    return TRUE;
}
// Symbol: ?RegisterWithRestartManager@CWinApp@@UEAAJHAEBV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@@Z
extern "C" long MS_ABI impl__RegisterWithRestartManager_CWinApp__UEAAJHAEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
    CWinApp*, int bRegisterRecoveryCallback, const CString* restartIdentifier) {
    if (!bRegisterRecoveryCallback) {
        return ::UnregisterApplicationRestart();
    }
    const wchar_t* commandLine = restartIdentifier ? static_cast<const wchar_t*>(*restartIdentifier) : L"";
    return ::RegisterApplicationRestart(commandLine, 0);
}
// Symbol: ?AddDocTemplate@CWinApp@@UEAAXPEAVCDocTemplate@@@Z
extern "C" void MS_ABI impl__AddDocTemplate_CWinApp__UEAAXPEAVCDocTemplate___Z(CWinApp* pThis, CDocTemplate* pTemplate) {
    if (!pThis || !pTemplate) return;
    auto& templates = g_appRuntimeStates[pThis].templates;
    if (std::find(templates.begin(), templates.end(), pTemplate) == templates.end()) {
        templates.push_back(pTemplate);
    }
}
// Symbol: ?GetFirstDocTemplatePosition@CWinApp@@QEBAPEAU__POSITION@@XZ
extern "C" void* MS_ABI impl__GetFirstDocTemplatePosition_CWinApp__QEBAPEAU__POSITION__XZ(const CWinApp* pThis) {
    if (!pThis) return nullptr;
    const auto& templates = g_appRuntimeStates[pThis].templates;
    return templates.empty() ? nullptr : reinterpret_cast<void*>(static_cast<uintptr_t>(1));
}
// Symbol: ?GetNextDocTemplate@CWinApp@@QEBAPEAVCDocTemplate@@AEAPEAU__POSITION@@@Z
extern "C" CDocTemplate* MS_ABI impl__GetNextDocTemplate_CWinApp__QEBAPEAVCDocTemplate__AEAPEAU__POSITION___Z(
    const CWinApp* pThis, void** pos) {
    if (!pThis || !pos || !*pos) return nullptr;
    const auto& templates = g_appRuntimeStates[pThis].templates;
    const uintptr_t index = reinterpret_cast<uintptr_t>(*pos) - 1;
    if (index >= templates.size()) {
        *pos = nullptr;
        return nullptr;
    }
    *pos = (index + 1 < templates.size()) ? reinterpret_cast<void*>(index + 2) : nullptr;
    return templates[index];
}
// Symbol: ?GetOpenDocumentCount@CWinApp@@QEAAHXZ
extern "C" int MS_ABI impl__GetOpenDocumentCount_CWinApp__QEAAHXZ(CWinApp* pThis) {
    if (!pThis) return 0;
    int total = 0;
    for (CDocTemplate* tpl : g_appRuntimeStates[pThis].templates) {
        total += CountTemplateDocuments(tpl);
    }
    return total;
}
// Symbol: ?OpenDocumentFile@CWinApp@@UEAAPEAVCDocument@@PEB_W@Z
extern "C" CDocument* MS_ABI impl__OpenDocumentFile_CWinApp__UEAAPEAVCDocument__PEB_W_Z(CWinApp* pThis, const wchar_t* pathName) {
    if (!pThis) return nullptr;
    return OpenWithTemplates(g_appRuntimeStates[pThis].templates, pathName, TRUE);
}
// Symbol: ?OpenDocumentFile@CWinApp@@UEAAPEAVCDocument@@PEB_WH@Z
extern "C" CDocument* MS_ABI impl__OpenDocumentFile_CWinApp__UEAAPEAVCDocument__PEB_WH_Z(
    CWinApp* pThis, const wchar_t* pathName, int bMakeVisible) {
    if (!pThis) return nullptr;
    return OpenWithTemplates(g_appRuntimeStates[pThis].templates, pathName, bMakeVisible);
}
// Symbol: ?OnFileNew@CWinApp@@IEAAXXZ
extern "C" void MS_ABI impl__OnFileNew_CWinApp__IEAAXXZ(CWinApp* pThis) {
    if (!pThis) return;
    (void)OpenWithTemplates(g_appRuntimeStates[pThis].templates, nullptr, TRUE);
}
// Symbol: ?OnFileOpen@CWinApp@@IEAAXXZ
extern "C" void MS_ABI impl__OnFileOpen_CWinApp__IEAAXXZ(CWinApp* pThis) {
    if (!pThis || !pThis->m_lpCmdLine || pThis->m_lpCmdLine[0] == L'\0') return;
    (void)OpenWithTemplates(g_appRuntimeStates[pThis].templates, pThis->m_lpCmdLine, TRUE);
}
// Symbol: ?OnOpenRecentFile@CWinApp@@IEAAHI@Z
extern "C" int MS_ABI impl__OnOpenRecentFile_CWinApp__IEAAHI_Z(CWinApp* pThis, unsigned int index) {
    if (!pThis) return FALSE;
    const auto& entries = g_appRuntimeStates[pThis].recentEntries;
    const AppRuntimeState& state = g_appRuntimeStates[pThis];
    const int resolved = AppRecentIndexFromCommand(state, index);
    if (resolved < 0 || static_cast<size_t>(resolved) >= entries.size()) return FALSE;
    return OpenWithTemplates(state.templates, entries[static_cast<size_t>(resolved)].c_str(), TRUE) != nullptr;
}
// Symbol: ?OnUpdateRecentFileMenu@CWinApp@@IEAAXPEAVCCmdUI@@@Z
extern "C" void MS_ABI impl__OnUpdateRecentFileMenu_CWinApp__IEAAXPEAVCCmdUI___Z(CWinApp* pThis, CCmdUI* pCmdUI) {
    if (!pThis || !pCmdUI) return;
    const auto& entries = g_appRuntimeStates[pThis].recentEntries;
    const AppRuntimeState& state = g_appRuntimeStates[pThis];
    const int index = AppRecentIndexFromCommand(state, pCmdUI->m_nID);
    if (index < 0 || static_cast<size_t>(index) >= entries.size()) {
        pCmdUI->Enable(FALSE);
        return;
    }
    pCmdUI->SetText(entries[static_cast<size_t>(index)].c_str());
    pCmdUI->Enable(TRUE);
}
// Symbol: ?GetRuntimeClass@CWinApp@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CWinApp__UEBAPEAUCRuntimeClass__XZ(const CWinApp*) {
    return &CWinApp::classCWinApp;
}
// Symbol: ?GetThisMessageMap@CWinApp@@KAPEBUAFX_MSGMAP@@XZ
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CWinApp__KAPEBUAFX_MSGMAP__XZ() {
    return &g_emptyAppMessageMap;
}
// Symbol: ?GetMessageMap@CWinApp@@MEBAPEBUAFX_MSGMAP@@XZ
extern "C" const AFX_MSGMAP* MS_ABI impl__GetMessageMap_CWinApp__MEBAPEBUAFX_MSGMAP__XZ(const CWinApp*) {
    return &g_emptyAppMessageMap;
}
// Symbol: ?AddDocTemplate@CWinApp@@QEAAXPEAVCDocTemplate@@@Z
extern "C" void MS_ABI impl__AddDocTemplate_CWinApp__QEAAXPEAVCDocTemplate___Z(CWinApp* pThis, CDocTemplate* pTemplate) {
    impl__AddDocTemplate_CWinApp__UEAAXPEAVCDocTemplate___Z(pThis, pTemplate);
}
// Symbol: ?CloseAllDocuments@CWinApp@@QEAAXH@Z
extern "C" void MS_ABI impl__CloseAllDocuments_CWinApp__QEAAXH_Z(CWinApp* pThis, int) {
    if (!pThis) return;
    for (CDocTemplate* tpl : g_appRuntimeStates[pThis].templates) CloseTemplateDocuments(tpl);
}
// Symbol: ?SaveAllModified@CWinApp@@UEAAHXZ
extern "C" int MS_ABI impl__SaveAllModified_CWinApp__UEAAHXZ(CWinApp* pThis) {
    if (!pThis) return FALSE;
    for (CDocTemplate* tpl : g_appRuntimeStates[pThis].templates) {
        if (!SaveTemplateDocuments(tpl)) return FALSE;
    }
    return TRUE;
}
// Symbol: ?GetOpenDocumentCount@CWinApp@@QEAAHXZ
extern "C" int MS_ABI impl__GetOpenDocumentCount_CWinApp__QEAAHXZ(CWinApp* pThis);
// Symbol: ?CreatePrinterDC@CWinApp@@QEAAHAEAVCDC@@@Z
extern "C" int MS_ABI impl__CreatePrinterDC_CWinApp__QEAAHAEAVCDC___Z(CWinApp* pThis, CDC* dc) {
    if (!pThis || !dc) return FALSE;
    AppRuntimeState& state = g_appRuntimeStates[pThis];
    HDC hdc = ::CreateDCW(L"WINSPOOL", nullptr, nullptr, nullptr);
    if (!hdc) return FALSE;
    dc->m_hDC = hdc;
    dc->m_hAttribDC = hdc;
    (void)state;
    return TRUE;
}
// Symbol: ?DevModeChange@CWinApp@@QEAAXPEA_W@Z
extern "C" void MS_ABI impl__DevModeChange_CWinApp__QEAAXPEA_W_Z(CWinApp* pThis, wchar_t* lpDeviceName) {
    if (!pThis) return;
    if (!lpDeviceName || *lpDeviceName == L'\0') return;
    auto& state = g_appRuntimeStates[pThis];
    state.selectedPrinter = 0;
    if (state.printerDevMode) {
        ::GlobalFree(state.printerDevMode);
        state.printerDevMode = nullptr;
    }
    if (state.printerDevNames) {
        ::GlobalFree(state.printerDevNames);
        state.printerDevNames = nullptr;
    }
}
// Symbol: ?DoEnableModeless@CWinApp@@SAXH@Z
extern "C" void MS_ABI impl__DoEnableModeless_CWinApp__SAXH_Z(int enable) {
    if (g_pApp) g_appRuntimeStates[g_pApp].modelessEnabled = enable != FALSE;
}
// Symbol: ?ShowAppMessageBox@CWinApp@@SAHPEAV1@PEB_WII@Z
extern "C" int MS_ABI impl__ShowAppMessageBox_CWinApp__SAHPEAV1_PEB_WII_Z(
    CWinApp* app, const wchar_t* prompt, unsigned int type, unsigned int) {
    return ::MessageBoxW(GetAppMainHwnd(app), prompt ? prompt : L"", GetAppName(app).c_str(), type);
}
// Symbol: ?DoMessageBox@CWinApp@@UEAAHPEB_WII@Z
extern "C" int MS_ABI impl__DoMessageBox_CWinApp__UEAAHPEB_WII_Z(
    CWinApp* pThis, const wchar_t* prompt, unsigned int type, unsigned int idPrompt) {
    return impl__ShowAppMessageBox_CWinApp__SAHPEAV1_PEB_WII_Z(pThis, prompt, type, idPrompt);
}
// Symbol: ?DoPrintDialog@CWinApp@@QEAA_JPEAVCPrintDialog@@@Z
extern "C" intptr_t MS_ABI impl__DoPrintDialog_CWinApp__QEAA_JPEAVCPrintDialog___Z(CWinApp*, CPrintDialog* dialog) {
    return dialog ? dialog->DoModal() : IDCANCEL;
}
// Symbol: ?DoWaitCursor@CWinApp@@UEAAXH@Z
extern "C" void MS_ABI impl__DoWaitCursor_CWinApp__UEAAXH_Z(CWinApp*, int code) {
    ::SetCursor(code > 0 ? GetWaitCursorHandle() : GetArrowCursorHandle());
}
// Symbol: ?EnableD2DSupport@CWinApp@@QEAAHW4D2D1_FACTORY_TYPE@@W4DWRITE_FACTORY_TYPE@@@Z
extern "C" int MS_ABI impl__EnableD2DSupport_CWinApp__QEAAHW4D2D1_FACTORY_TYPE__W4DWRITE_FACTORY_TYPE___Z(CWinApp* pThis, int, int) {
    if (!pThis) return FALSE;
    g_appRuntimeStates[pThis].d2dEnabled = true;
    return TRUE;
}
// Symbol: ?EnableModeless@CWinApp@@QEAAXH@Z
extern "C" void MS_ABI impl__EnableModeless_CWinApp__QEAAXH_Z(CWinApp* pThis, int enable) {
    if (pThis) g_appRuntimeStates[pThis].modelessEnabled = enable != FALSE;
}
// Symbol: ?EnableShellOpen@CWinApp@@IEAAXXZ
extern "C" void MS_ABI impl__EnableShellOpen_CWinApp__IEAAXXZ(CWinApp* pThis) {
    if (pThis) g_appRuntimeStates[pThis].shellOpenEnabled = true;
}
// Symbol: ?EnableTaskbarInteraction@CWinApp@@QEAAHH@Z
extern "C" int MS_ABI impl__EnableTaskbarInteraction_CWinApp__QEAAHH_Z(CWinApp* pThis, int enable) {
    if (!pThis) return FALSE;
    g_appRuntimeStates[pThis].taskbarEnabled = enable != FALSE;
    return TRUE;
}
// Symbol: ?GetAppRegistryKey@CWinApp@@QEAAPEAUHKEY__@@PEAVCAtlTransactionManager@ATL@@@Z
extern "C" HKEY MS_ABI impl__GetAppRegistryKey_CWinApp__QEAAPEAUHKEY____PEAVCAtlTransactionManager_ATL___Z(CWinApp* pThis, void*) {
    if (!pThis) return nullptr;
    HKEY key = nullptr;
    return ::RegCreateKeyExW(HKEY_CURRENT_USER, GetAppRegistryRoot(pThis).c_str(), 0, nullptr, 0, KEY_ALL_ACCESS, nullptr, &key, nullptr) == ERROR_SUCCESS ? key : nullptr;
}
// Symbol: ?GetSectionKey@CWinApp@@QEAAPEAUHKEY__@@PEB_WPEAVCAtlTransactionManager@ATL@@@Z
extern "C" HKEY MS_ABI impl__GetSectionKey_CWinApp__QEAAPEAUHKEY____PEB_WPEAVCAtlTransactionManager_ATL___Z(CWinApp* pThis, const wchar_t* section, void*) {
    if (!pThis || !section) return nullptr;
    std::wstring path = GetAppRegistryRoot(pThis);
    if (!path.empty() && path.back() != L'\\') path.push_back(L'\\');
    path.append(section);
    HKEY key = nullptr;
    return ::RegCreateKeyExW(HKEY_CURRENT_USER, path.c_str(), 0, nullptr, 0, KEY_ALL_ACCESS, nullptr, &key, nullptr) == ERROR_SUCCESS ? key : nullptr;
}
// Symbol: ?GetDataRecoveryHandler@CWinApp@@UEAAPEAVCDataRecoveryHandler@@XZ
extern "C" void* MS_ABI impl__GetDataRecoveryHandler_CWinApp__UEAAPEAVCDataRecoveryHandler__XZ(CWinApp* pThis) {
    if (!pThis) return nullptr;
    return &g_appRuntimeStates[pThis].dataRecoveryHandler;
}
// Symbol: ?GetITaskbarList@CWinApp@@QEAAPEAUITaskbarList@@XZ
extern "C" ITaskbarList* MS_ABI impl__GetITaskbarList_CWinApp__QEAAPEAUITaskbarList__XZ(CWinApp* pThis) {
    if (!pThis) return nullptr;
    AppRuntimeState& state = g_appRuntimeStates[pThis];
    if (!state.taskbarEnabled) return nullptr;
    if (!state.taskbarList) {
        ::CoCreateInstance(CLSID_TaskbarList, nullptr, CLSCTX_INPROC_SERVER, IID_ITaskbarList, reinterpret_cast<void**>(&state.taskbarList));
        if (state.taskbarList) state.taskbarList->HrInit();
    }
    return state.taskbarList;
}
// Symbol: ?GetITaskbarList3@CWinApp@@QEAAPEAUITaskbarList3@@XZ
extern "C" ITaskbarList3* MS_ABI impl__GetITaskbarList3_CWinApp__QEAAPEAUITaskbarList3__XZ(CWinApp* pThis) {
    if (!pThis) return nullptr;
    AppRuntimeState& state = g_appRuntimeStates[pThis];
    if (!state.taskbarEnabled) return nullptr;
    if (!state.taskbarList3) {
        ::CoCreateInstance(CLSID_TaskbarList, nullptr, CLSCTX_INPROC_SERVER, IID_ITaskbarList3, reinterpret_cast<void**>(&state.taskbarList3));
        if (state.taskbarList3) state.taskbarList3->HrInit();
    }
    return state.taskbarList3;
}
// Symbol: ?ReleaseTaskBarRefs@CWinApp@@QEAAXXZ
extern "C" void MS_ABI impl__ReleaseTaskBarRefs_CWinApp__QEAAXXZ(CWinApp* pThis) {
    if (!pThis) return;
    AppRuntimeState& state = g_appRuntimeStates[pThis];
    if (state.taskbarList3) state.taskbarList3->Release();
    if (state.taskbarList) state.taskbarList->Release();
    state.taskbarList3 = nullptr;
    state.taskbarList = nullptr;
}
// Symbol: ?GetPrinterDeviceDefaults@CWinApp@@QEAAHPEAUtagPDW@@@Z
extern "C" int MS_ABI impl__GetPrinterDeviceDefaults_CWinApp__QEAAHPEAUtagPDW___Z(CWinApp* pThis, PRINTDLGW* pd) {
    if (!pThis || !pd) return FALSE;
    AppRuntimeState& state = g_appRuntimeStates[pThis];
    pd->hDevMode = state.printerDevMode;
    pd->hDevNames = state.printerDevNames;
    return TRUE;
}
// Symbol: ?GetSysPolicyValue@CWinApp@@QEAAHKPEAH@Z
extern "C" int MS_ABI impl__GetSysPolicyValue_CWinApp__QEAAHKPEAH_Z(CWinApp* pThis, unsigned long uiPolicy, int* value) {
    if (!value) return FALSE;
    const std::wstring appName = GetAppName(pThis);
    return ReadPolicyValueFromRegistry(appName, uiPolicy, value) ? TRUE : FALSE;
}
// Symbol: ?HideApplication@CWinApp@@QEAAXXZ
extern "C" void MS_ABI impl__HideApplication_CWinApp__QEAAXXZ(CWinApp* pThis) {
    HWND hwnd = GetAppMainHwnd(pThis);
    if (hwnd) ::ShowWindow(hwnd, SW_HIDE);
}
// Symbol: ?HtmlHelpW@CWinApp@@UEAAX_KI@Z
extern "C" void MS_ABI impl__HtmlHelpW_CWinApp__UEAAX_KI_Z(
    CWinApp* pThis, unsigned long long data, unsigned int command) {
    if (!pThis || !pThis->m_pszHelpFilePath) return;
    HWND hwnd = GetAppMainHwnd(pThis);
    using HtmlHelpFn = HWND(WINAPI*)(HWND, LPCWSTR, UINT, ULONG_PTR);
    HMODULE hhModule = ::GetModuleHandleW(L"hhctrl.ocx");
    bool owned = false;
    if (!hhModule) {
        hhModule = ::LoadLibraryW(L"hhctrl.ocx");
        owned = hhModule != nullptr;
    }
    if (!hhModule) {
        hhModule = ::LoadLibraryW(L"hhctrl.dll");
        owned = hhModule != nullptr;
    }
    if (hhModule) {
        HtmlHelpFn htmlHelp = reinterpret_cast<HtmlHelpFn>(::GetProcAddress(hhModule, "HtmlHelpW"));
        if (htmlHelp) {
            htmlHelp(hwnd, pThis->m_pszHelpFilePath, command, data);
        } else {
            impl__WinHelpInternal_CWinApp__UEAAX_KI_Z(pThis, data, command);
        }
        if (owned) ::FreeLibrary(hhModule);
        return;
    }
    impl__WinHelpInternal_CWinApp__UEAAX_KI_Z(pThis, data, command);
}
// Symbol: ?InitLibId@CWinApp@@UEAAXXZ
extern "C" void MS_ABI impl__InitLibId_CWinApp__UEAAXXZ(CWinApp* pThis) {
    if (!pThis) return;
    auto& state = g_appRuntimeStates[pThis];
    state.appId = GetAppName(pThis);
}
// Symbol: ?IsTaskbarInteractionEnabled@CWinApp@@UEAAHXZ
extern "C" int MS_ABI impl__IsTaskbarInteractionEnabled_CWinApp__UEAAHXZ(CWinApp* pThis) {
    return pThis && g_appRuntimeStates[pThis].taskbarEnabled ? TRUE : FALSE;
}
// Symbol: ?IsWindows7@CWinApp@@QEAAHXZ
extern "C" int MS_ABI impl__IsWindows7_CWinApp__QEAAHXZ(CWinApp*) {
    OSVERSIONINFOEXW version{};
    version.dwOSVersionInfoSize = sizeof(version);
    if (::GetVersionExW(reinterpret_cast<LPOSVERSIONINFOW>(&version))) {
        return version.dwMajorVersion == 6 && version.dwMinorVersion == 1 ? TRUE : FALSE;
    }
    return FALSE;
}
// Symbol: ?LoadAppLangResourceDLL@CWinApp@@UEAAPEAUHINSTANCE__@@XZ
extern "C" HINSTANCE MS_ABI impl__LoadAppLangResourceDLL_CWinApp__UEAAPEAUHINSTANCE____XZ(CWinApp*) {
    return AfxGetResourceHandle();
}
// Symbol: ?LoadSysPolicies@CWinApp@@UEAAHXZ
extern "C" int MS_ABI impl__LoadSysPolicies_CWinApp__UEAAHXZ(CWinApp* pThis) {
    if (!pThis) return FALSE;
    int policyValue = 0;
    bool hasPolicy = false;
    const std::wstring appName = GetAppName(pThis);
    for (unsigned long policy = 1; policy <= 4; ++policy) {
        if (ReadPolicyValueFromRegistry(appName, policy, &policyValue)) {
            hasPolicy = true;
        }
    }
    return hasPolicy ? TRUE : FALSE;
}
// Symbol: ?OnAppExit@CWinApp@@IEAAXXZ
extern "C" void MS_ABI impl__OnAppExit_CWinApp__IEAAXXZ(CWinApp* pThis) {
    impl__CloseAllDocuments_CWinApp__QEAAXH_Z(pThis, FALSE);
    ::PostQuitMessage(0);
}
// Symbol: ?OnContextHelp@CWinApp@@IEAAXXZ
extern "C" void MS_ABI impl__OnContextHelp_CWinApp__IEAAXXZ(CWinApp* pThis) {
    if (!pThis) return;
    impl__OnHelp_CWinApp__IEAAXXZ(pThis);
}
// Symbol: ?OnDDECommand@CWinApp@@UEAAHPEA_W@Z
extern "C" int MS_ABI impl__OnDDECommand_CWinApp__UEAAHPEA_W_Z(CWinApp* pThis, wchar_t* command) {
    if (!pThis || !command) return FALSE;
    auto& state = g_appRuntimeStates[pThis];
    if (!state.shellOpenEnabled) return FALSE;
    return command[0] != L'\0' ? TRUE : FALSE;
}
// Symbol: ?OnFilePrintSetup@CWinApp@@IEAAXXZ
extern "C" void MS_ABI impl__OnFilePrintSetup_CWinApp__IEAAXXZ(CWinApp* pThis) {
    CPrintDialog dialog(TRUE);
    (void)impl__DoPrintDialog_CWinApp__QEAA_JPEAVCPrintDialog___Z(pThis, &dialog);
}
// Symbol: ?OnHelp@CWinApp@@IEAAXXZ
extern "C" void MS_ABI impl__OnHelp_CWinApp__IEAAXXZ(CWinApp* pThis) {
    if (pThis && pThis->m_pszHelpFilePath) ::ShellExecuteW(GetAppMainHwnd(pThis), L"open", pThis->m_pszHelpFilePath, nullptr, nullptr, SW_SHOWNORMAL);
}
// Symbol: ?OnHelpFinder@CWinApp@@IEAAXXZ
extern "C" void MS_ABI impl__OnHelpFinder_CWinApp__IEAAXXZ(CWinApp* pThis) {
    impl__OnHelp_CWinApp__IEAAXXZ(pThis);
}
// Symbol: ?OnHelpIndex@CWinApp@@IEAAXXZ
extern "C" void MS_ABI impl__OnHelpIndex_CWinApp__IEAAXXZ(CWinApp* pThis) {
    impl__OnHelp_CWinApp__IEAAXXZ(pThis);
}
// Symbol: ?OnHelpUsing@CWinApp@@IEAAXXZ
extern "C" void MS_ABI impl__OnHelpUsing_CWinApp__IEAAXXZ(CWinApp* pThis) {
    impl__OnHelp_CWinApp__IEAAXXZ(pThis);
}
// Symbol: ?OnIdle@CWinApp@@UEAAHJ@Z
extern "C" int MS_ABI impl__OnIdle_CWinApp__UEAAHJ_Z(CWinApp* pThis, long count) {
    return pThis ? pThis->CWinThread::OnIdle(count) : FALSE;
}
// Symbol: ?ProcessWndProcException@CWinApp@@UEAA_JPEAVCException@@PEBUtagMSG@@@Z
extern "C" intptr_t MS_ABI impl__ProcessWndProcException_CWinApp__UEAA_JPEAVCException__PEBUtagMSG___Z(
    CWinApp*, CException* exception, const MSG*) {
    if (exception) {
        wchar_t buffer[512] = {};
        exception->GetErrorMessage(buffer, 512, nullptr);
        if (buffer[0] != L'\0') ::MessageBoxW(nullptr, buffer, L"OpenMFC", MB_OK | MB_ICONERROR);
    }
    return 0;
}
// Symbol: ?Register@CWinApp@@UEAAHXZ
extern "C" int MS_ABI impl__Register_CWinApp__UEAAHXZ(CWinApp* pThis) {
    if (!pThis) return FALSE;
    g_appRuntimeStates[pThis].shellOpenEnabled = true;
    impl__LoadSysPolicies_CWinApp__UEAAHXZ(pThis);
    return TRUE;
}
// Symbol: ?RegisterShellFileTypes@CWinApp@@IEAAXH@Z
extern "C" void MS_ABI impl__RegisterShellFileTypes_CWinApp__IEAAXH_Z(CWinApp* pThis, int) {
    if (pThis) g_appRuntimeStates[pThis].shellOpenEnabled = true;
}
// Symbol: ?RegisterWithRestartManager@CWinApp@@UEAAJPEB_WKP6AKPEAX@Z1KK@Z
extern "C" long MS_ABI impl__RegisterWithRestartManager_CWinApp__UEAAJPEB_WKP6AKPEAX_Z1KK_Z(
    CWinApp*, const wchar_t* restartIdentifier, unsigned long flags, DWORD (WINAPI*)(void*), void*, unsigned long, unsigned long) {
    return ::RegisterApplicationRestart(restartIdentifier ? restartIdentifier : L"", flags);
}
// Symbol: ?RestartInstance@CWinApp@@UEAAHXZ
extern "C" int MS_ABI impl__RestartInstance_CWinApp__UEAAHXZ(CWinApp* pThis) {
    if (!pThis) return FALSE;
    auto& state = g_appRuntimeStates[pThis];
    return state.automated || state.embedded || !state.appId.empty() ? TRUE : FALSE;
}
// Symbol: ?RunAutomated@CWinApp@@QEAAHXZ
extern "C" int MS_ABI impl__RunAutomated_CWinApp__QEAAHXZ(CWinApp* pThis) {
    if (!pThis) return FALSE;
    std::wstring cmd = LowerWide(WideValue(pThis->m_lpCmdLine));
    bool automated = cmd.find(L"/automation") != std::wstring::npos || cmd.find(L"-automation") != std::wstring::npos;
    g_appRuntimeStates[pThis].automated = automated;
    return automated ? TRUE : FALSE;
}
// Symbol: ?RunEmbedded@CWinApp@@QEAAHXZ
extern "C" int MS_ABI impl__RunEmbedded_CWinApp__QEAAHXZ(CWinApp* pThis) {
    if (!pThis) return FALSE;
    std::wstring cmd = LowerWide(WideValue(pThis->m_lpCmdLine));
    bool embedded = cmd.find(L"/embedding") != std::wstring::npos || cmd.find(L"-embedding") != std::wstring::npos;
    g_appRuntimeStates[pThis].embedded = embedded;
    return embedded ? TRUE : FALSE;
}
// Symbol: ?SaveStdProfileSettings@CWinApp@@IEAAXXZ
extern "C" void MS_ABI impl__SaveStdProfileSettings_CWinApp__IEAAXXZ(CWinApp* pThis) {
    if (!pThis) return;
    auto& state = g_appRuntimeStates[pThis];
    if (state.maxRecent == 0) state.maxRecent = 4;
    impl__WriteProfileInt_CWinApp__UEAAHPEB_W0H_Z(pThis, L"Settings", L"MaxRecentDocs", static_cast<int>(state.maxRecent));
    for (unsigned int i = 0; i < state.maxRecent && i < state.recentEntries.size(); ++i) {
        std::wstring key = L"RecentFile" + std::to_wstring(i + 1);
        impl__WriteProfileStringW_CWinApp__UEAAHPEB_W00_Z(pThis, L"Settings", key.c_str(), state.recentEntries[i].c_str());
    }
}
// Symbol: ?SelectPrinter@CWinApp@@QEAAXPEAX0H@Z
extern "C" void MS_ABI impl__SelectPrinter_CWinApp__QEAAXPEAX0H_Z(CWinApp* pThis, void* devNames, void* devMode, int) {
    if (!pThis) return;
    AppRuntimeState& state = g_appRuntimeStates[pThis];
    state.printerDevNames = static_cast<HGLOBAL>(devNames);
    state.printerDevMode = static_cast<HGLOBAL>(devMode);
}
// Symbol: ?SetAppID@CWinApp@@IEAAXPEB_W@Z
extern "C" void MS_ABI impl__SetAppID_CWinApp__IEAAXPEB_W_Z(CWinApp* pThis, const wchar_t* appId) {
    if (pThis) g_appRuntimeStates[pThis].appId = WideValue(appId);
}
// Symbol: ?SetCurrentHandles@CWinApp@@QEAAXXZ
extern "C" void MS_ABI impl__SetCurrentHandles_CWinApp__QEAAXXZ(CWinApp* pThis) {
    if (!pThis) return;
    pThis->m_hInstance = AfxGetInstanceHandle();
    pThis->m_nThreadID = ::GetCurrentThreadId();
    pThis->m_hThread = ::GetCurrentThread();
}
// Symbol: ?SetRegistryKey@CWinApp@@IEAAXPEB_W@Z
extern "C" void MS_ABI impl__SetRegistryKey_CWinApp__IEAAXPEB_W_Z(CWinApp* pThis, const wchar_t* key) {
    if (pThis) g_appRuntimeStates[pThis].registryRoot = key && *key ? std::wstring(L"Software\\") + key : L"Software\\OpenMFC";
}
// Symbol: ?SetRegistryKey@CWinApp@@IEAAXI@Z
extern "C" void MS_ABI impl__SetRegistryKey_CWinApp__IEAAXI_Z(CWinApp* pThis, unsigned int id) {
    if (!pThis) return;
    wchar_t buffer[64] = {};
    std::swprintf(buffer, 64, L"OpenMFC\\%u", id);
    g_appRuntimeStates[pThis].registryRoot = std::wstring(L"Software\\") + buffer;
}
// Symbol: ?Unregister@CWinApp@@UEAAHXZ
extern "C" int MS_ABI impl__Unregister_CWinApp__UEAAHXZ(CWinApp* pThis) {
    if (!pThis) return FALSE;
    auto& state = g_appRuntimeStates[pThis];
    state.shellOpenEnabled = false;
    state.selectedPrinter = 0;
    if (state.printerDevMode) {
        ::GlobalFree(state.printerDevMode);
        state.printerDevMode = nullptr;
    }
    if (state.printerDevNames) {
        ::GlobalFree(state.printerDevNames);
        state.printerDevNames = nullptr;
    }
    impl__UnregisterShellFileTypes_CWinApp__IEAAXXZ(pThis);
    impl__ReleaseTaskBarRefs_CWinApp__QEAAXXZ(pThis);
    return TRUE;
}
// Symbol: ?UnregisterShellFileTypes@CWinApp@@IEAAXXZ
extern "C" void MS_ABI impl__UnregisterShellFileTypes_CWinApp__IEAAXXZ(CWinApp* pThis) {
    if (pThis) g_appRuntimeStates[pThis].shellOpenEnabled = false;
}
// Symbol: ?UpdatePrinterSelection@CWinApp@@IEAAXH@Z
extern "C" void MS_ABI impl__UpdatePrinterSelection_CWinApp__IEAAXH_Z(CWinApp* pThis, int nMode) {
    if (!pThis) return;
    auto& state = g_appRuntimeStates[pThis];
    state.selectedPrinter = nMode;
    if (nMode == 0) {
        if (state.printerDevMode) {
            ::GlobalFree(state.printerDevMode);
            state.printerDevMode = nullptr;
        }
        if (state.printerDevNames) {
            ::GlobalFree(state.printerDevNames);
            state.printerDevNames = nullptr;
        }
    }
}
// Symbol: ?WinHelpInternal@CWinApp@@UEAAX_KI@Z
extern "C" void MS_ABI impl__WinHelpInternal_CWinApp__UEAAX_KI_Z(CWinApp* pThis, unsigned long long data, unsigned int command) {
    if (pThis && pThis->m_pszHelpFilePath) ::WinHelpW(GetAppMainHwnd(pThis), pThis->m_pszHelpFilePath, command, static_cast<ULONG_PTR>(data));
}
// Symbol: ?WinHelpW@CWinApp@@UEAAX_KI@Z
extern "C" void MS_ABI impl__WinHelpW_CWinApp__UEAAX_KI_Z(CWinApp* pThis, unsigned long long data, unsigned int command) {
    impl__WinHelpInternal_CWinApp__UEAAX_KI_Z(pThis, data, command);
}
// Symbol: ?ApplicationRecoveryCallback@CWinApp@@UEAAKPEAX@Z
extern "C" unsigned long MS_ABI impl__ApplicationRecoveryCallback_CWinApp__UEAAKPEAX_Z(CWinApp* pThis, void* pContext) {
    if (!pThis) return 1;
    (void)pContext;
    impl__SaveAllModified_CWinApp__UEAAHXZ(pThis);
    return 0;
}
CWinApp::CWinApp(const wchar_t* lpszAppName) {
    m_pMainWnd = nullptr;
    m_nThreadID = ::GetCurrentThreadId();
    m_hThread = ::GetCurrentThread();
    m_bAutoDelete = FALSE;
    m_hInstance = nullptr;
    m_lpCmdLine = nullptr;
    m_nCmdShow = SW_SHOW;
    m_pszAppName = lpszAppName;
    m_pszExeName = nullptr;
    m_pszHelpFilePath = nullptr;
    m_pszProfileName = nullptr;
    m_pszRegistryKey = nullptr;
    memset(&m_msgCur, 0, sizeof(m_msgCur));
    g_pApp = this;
}
int CWinApp::InitApplication() {
    return TRUE;
}
BOOL CWinApp::InitInstance() {
    return TRUE;
}
int CWinApp::ExitInstance() {
    return CWinThread::ExitInstance();
}
int CWinApp::Run() {
    return CWinThread::Run();
}
