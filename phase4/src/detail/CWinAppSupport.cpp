#include "CWinAppSupport.h"

IMPLEMENT_DYNAMIC(CCmdTarget, CObject)
IMPLEMENT_DYNAMIC(CMemoryException, CException)
IMPLEMENT_DYNAMIC(CFileException, CException)
IMPLEMENT_DYNAMIC(CArchiveException, CException)
IMPLEMENT_DYNAMIC(CWinThread, CCmdTarget)
IMPLEMENT_DYNAMIC(CWinApp, CWinThread)
CWinApp* g_pApp = nullptr;
__thread CWinThread* g_pCurrentThread = nullptr;
CWinThread* AfxGetThread() {
    // For worker threads, return the thread-local pointer
    if (g_pCurrentThread != nullptr) {
        return g_pCurrentThread;
    }
    // For main thread, return the app
    return static_cast<CWinThread*>(g_pApp);
}
HINSTANCE AFXAPI AfxGetInstanceHandle() {
    return g_hInstance;
}
HINSTANCE AFXAPI AfxGetResourceHandle() {
    return g_hResource ? g_hResource : g_hInstance;
}
void AFXAPI AfxSetResourceHandle(HINSTANCE hInstResource) {
    g_hResource = hInstResource;
}
namespace openmfc { namespace detail { namespace cwinapp {
CRuntimeClass* const g_coreRuntimeClasses[] = {
    &CObject::classCObject,
    &CException::classCException,
    &CMemoryException::classCMemoryException,
    &CFileException::classCFileException,
    &CArchiveException::classCArchiveException,
    &CCmdTarget::classCCmdTarget,
    &CWinThread::classCWinThread,
    &CWinApp::classCWinApp,
};
bool RuntimeClassNameEquals(const char* lhs, const char* rhs) {
    return lhs && rhs && std::strcmp(lhs, rhs) == 0;
}
bool RuntimeClassNameEquals(const char* lhs, const wchar_t* rhs) {
    if (!lhs || !rhs) return false;
    while (*lhs && *rhs) {
        if (static_cast<unsigned char>(*lhs) != static_cast<unsigned int>(*rhs)) return false;
        ++lhs;
        ++rhs;
    }
    return *lhs == '\0' && *rhs == L'\0';
}
CRuntimeClass* FindRuntimeClassByName(const char* name) {
    if (!name || !*name) return nullptr;
    for (CRuntimeClass* seed : g_coreRuntimeClasses) {
        for (CRuntimeClass* pClass = seed; pClass; pClass = pClass->m_pNextClass) {
            if (RuntimeClassNameEquals(pClass->m_lpszClassName, name)) return pClass;
        }
    }
    return nullptr;
}
CRuntimeClass* FindRuntimeClassByName(const wchar_t* name) {
    if (!name || !*name) return nullptr;
    for (CRuntimeClass* seed : g_coreRuntimeClasses) {
        for (CRuntimeClass* pClass = seed; pClass; pClass = pClass->m_pNextClass) {
            if (RuntimeClassNameEquals(pClass->m_lpszClassName, name)) return pClass;
        }
    }
    return nullptr;
}
// Symbol: ??1CCmdTarget@@UEAA@XZ
extern "C" void MS_ABI impl___1CCmdTarget__UEAA_XZ(CCmdTarget* pThis) {
    if (pThis) {
        pThis->~CCmdTarget();
    }
}
int OpenMfcRunExportedMessageLoop(CWinThread* pThis) {
    MSG msg;

    for (;;) {
        while (PeekMessageW(&msg, nullptr, 0, 0, PM_REMOVE)) {
            pThis->m_msgCur = msg;
            if (msg.message == WM_QUIT) {
                return static_cast<int>(msg.wParam);
            }
            TranslateMessage(&msg);
            DispatchMessageW(&msg);
        }

        const BOOL result = GetMessageW(&msg, nullptr, 0, 0);
        pThis->m_msgCur = msg;

        if (result == 0) {
            return static_cast<int>(msg.wParam);
        }
        if (result < 0) {
            return -1;
        }

        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }
}
HINSTANCE g_hInstance = nullptr;
HINSTANCE g_hResource = nullptr;
void* g_appModuleStateStorage[kOpaqueStateWordCount] = {};
__thread _AFX_THREAD_STATE g_threadStateStorage;
extern "C" HINSTANCE MS_ABI impl__AfxGetInstanceHandleHelper__YAPEAUHINSTANCE____XZ() {
    return AfxGetInstanceHandleHelper();
}
extern "C" AFX_MODULE_STATE* MS_ABI impl__AfxGetAppModuleState__YAPEAVAFX_MODULE_STATE__XZ() {
    return AfxGetAppModuleState();
}
extern "C" _AFX_THREAD_STATE* MS_ABI impl__AfxGetThreadState__YAPEAV_AFX_THREAD_STATE__XZ() {
    return AfxGetThreadState();
}
extern "C" int MS_ABI impl__AfxWinInit__YAHPEAUHINSTANCE____0PEA_WH_Z(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    wchar_t* lpCmdLine,
    int nCmdShow)
{
    return AfxWinInit(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
}
extern "C" CWnd* MS_ABI impl__AfxGetMainWnd__YAPEAVCWnd__XZ() {
    return AfxGetMainWnd();
}
std::unordered_map<const CCommandLineInfo*, CommandLineInfoState> g_commandLineInfoStates;
std::unordered_map<const CDocManager*, DocManagerState> g_docManagerStates;
std::unordered_map<const CRecentFileList*, RecentFileListState> g_recentFileListStates;
std::unordered_map<const CWinApp*, AppRuntimeState> g_appRuntimeStates;
std::unordered_map<const CCmdTarget*, int> g_waitCursorDepth;
static CRuntimeClass* AFXAPI _openmfc_gb_g_classCDocManager() { return &CObject::classCObject; }
CRuntimeClass g_classCDocManager = {
    "CDocManager",
    0,
    0xFFFF,
    nullptr,
    &_openmfc_gb_g_classCDocManager,
    nullptr,
    nullptr
};
std::wstring MakeProfileKey(const wchar_t* section, const wchar_t* entry) {
    std::wstring key = section ? section : L"";
    key.push_back(L'\x1f');
    key.append(entry ? entry : L"");
    return key;
}
int AppRecentIndexFromCommand(const AppRuntimeState& state, unsigned int idOrIndex) {
    if (idOrIndex == 0) return -1;

    if (state.maxRecent > 0) {
        const auto maxRecent = static_cast<unsigned int>(state.maxRecent);
        if (idOrIndex >= kDefaultRecentFileMenuFirstId) {
            const unsigned int idx = idOrIndex - kDefaultRecentFileMenuFirstId;
            if (idx < maxRecent) return static_cast<int>(idx);
        }

        if (idOrIndex <= maxRecent) {
            return static_cast<int>(idOrIndex - 1);
        }
    }

    if (idOrIndex <= static_cast<unsigned int>(state.recentEntries.size())) {
        return static_cast<int>(idOrIndex);
    }

    return -1;
}
std::wstring WideValue(const wchar_t* value) {
    return value ? value : L"";
}
std::wstring LowerWide(std::wstring value) {
    std::transform(value.begin(), value.end(), value.begin(),
                   [](wchar_t ch) { return static_cast<wchar_t>(std::towlower(ch)); });
    return value;
}
std::wstring GetAppRegistryRoot(CWinApp* app) {
    AppRuntimeState& state = g_appRuntimeStates[app];
    if (!state.registryRoot.empty()) return state.registryRoot;
    if (app && app->m_pszRegistryKey && *app->m_pszRegistryKey) return app->m_pszRegistryKey;
    if (app && app->m_pszAppName && *app->m_pszAppName) return std::wstring(L"Software\\") + app->m_pszAppName;
    return L"Software\\OpenMFC";
}
std::wstring GetAppName(CWinApp* app) {
    if (app && app->m_pszAppName && *app->m_pszAppName) return app->m_pszAppName;
    if (app && app->m_pszExeName && *app->m_pszExeName) return app->m_pszExeName;
    return L"OpenMFC";
}
bool ReadDwordRegistryValue(HKEY root, const wchar_t* subKey, const wchar_t* valueName, unsigned long* value) {
    if (!subKey || !valueName || !value) return false;
    HKEY key = nullptr;
    if (::RegOpenKeyExW(root, subKey, 0, KEY_READ, &key) != ERROR_SUCCESS) {
        return false;
    }
    DWORD type = 0;
    DWORD bytes = static_cast<DWORD>(sizeof(*value));
    DWORD raw = 0;
    const LONG status = ::RegQueryValueExW(key, valueName, nullptr, &type, reinterpret_cast<LPBYTE>(&raw), &bytes);
    ::RegCloseKey(key);
    if (status != ERROR_SUCCESS || type != REG_DWORD || bytes != sizeof(*value)) return false;
    *value = raw;
    return true;
}
bool ReadPolicyValueFromRegistry(const std::wstring& appName, unsigned long policy, int* value) {
    if (!value) return false;
    *value = 0;
    unsigned long raw = 0;
    std::wstring valueName = std::to_wstring(policy);
    std::wstring policyName = L"Policy" + valueName;
    const wchar_t* keys[][2] = {
        {L"Software\\OpenMFC\\Policies", valueName.c_str()},
        {L"Software\\OpenMFC\\Policies", policyName.c_str()}
    };

    for (auto* const* pair : keys) {
        if (!pair[0]) continue;
        if (ReadDwordRegistryValue(HKEY_CURRENT_USER, pair[0], pair[1], &raw) ||
            ReadDwordRegistryValue(HKEY_LOCAL_MACHINE, pair[0], pair[1], &raw)) {
            *value = static_cast<int>(raw);
            return true;
        }
    }

    if (!appName.empty()) {
        const std::wstring appSpecific = std::wstring(L"Software\\OpenMFC\\Policies\\") + appName;
        if (ReadDwordRegistryValue(HKEY_CURRENT_USER, appSpecific.c_str(), valueName.c_str(), &raw) ||
            ReadDwordRegistryValue(HKEY_LOCAL_MACHINE, appSpecific.c_str(), valueName.c_str(), &raw) ||
            ReadDwordRegistryValue(HKEY_CURRENT_USER, appSpecific.c_str(), policyName.c_str(), &raw) ||
            ReadDwordRegistryValue(HKEY_LOCAL_MACHINE, appSpecific.c_str(), policyName.c_str(), &raw)) {
            *value = static_cast<int>(raw);
            return true;
        }
    }

    return false;
}
HWND GetAppMainHwnd(CWinApp* app) {
    return app && app->m_pMainWnd ? app->m_pMainWnd->GetSafeHwnd() : nullptr;
}
int CountTemplateDocuments(CDocTemplate* tpl) {
    if (!tpl) return 0;
    int count = 0;
    void* pos = tpl->GetFirstDocPosition();
    while (pos) {
        CDocument* doc = tpl->GetNextDoc(pos);
        if (doc) ++count;
    }
    return count;
}
bool SaveTemplateDocuments(CDocTemplate* tpl) {
    if (!tpl) return true;
    void* pos = tpl->GetFirstDocPosition();
    while (pos) {
        CDocument* doc = tpl->GetNextDoc(pos);
        if (doc && !doc->SaveModified()) return false;
    }
    return true;
}
void CloseTemplateDocuments(CDocTemplate* tpl) {
    if (!tpl) return;
    std::vector<CDocument*> docs;
    void* pos = tpl->GetFirstDocPosition();
    while (pos) {
        CDocument* doc = tpl->GetNextDoc(pos);
        if (doc) docs.push_back(doc);
    }
    for (CDocument* doc : docs) {
        doc->OnCloseDocument();
    }
}
const wchar_t* FindFileNamePart(const wchar_t* path) {
    if (path == nullptr) return L"";
    const wchar_t* name = path;
    for (const wchar_t* p = path; *p != L'\0'; ++p) {
        if (*p == L'\\' || *p == L'/') {
            name = p + 1;
        }
    }
    return name;
}
void AddRecentPath(std::vector<std::wstring>& entries, int maxEntries, const wchar_t* path) {
    if (path == nullptr || *path == L'\0') return;
    std::wstring value(path);
    entries.erase(std::remove(entries.begin(), entries.end(), value), entries.end());
    entries.insert(entries.begin(), std::move(value));
    if (maxEntries >= 0 && static_cast<int>(entries.size()) > maxEntries) {
        entries.resize(static_cast<size_t>(maxEntries));
    }
}
std::vector<std::wstring> TokenizeCommandLine(const wchar_t* cmdLine) {
    std::vector<std::wstring> tokens;
    if (cmdLine == nullptr) return tokens;

    const wchar_t* p = cmdLine;
    while (*p != L'\0') {
        while (*p == L' ' || *p == L'\t') ++p;
        if (*p == L'\0') break;

        std::wstring token;
        bool inQuotes = false;
        while (*p != L'\0') {
            if (*p == L'"') {
                inQuotes = !inQuotes;
                ++p;
                continue;
            }
            if (!inQuotes && (*p == L' ' || *p == L'\t')) break;
            token.push_back(*p++);
        }
        tokens.push_back(std::move(token));
        while (*p == L' ' || *p == L'\t') ++p;
    }
    return tokens;
}
void ParseCommandFlag(CommandLineInfoState& state, const wchar_t* flag) {
    if (flag == nullptr || *flag == L'\0') return;
    std::wstring lower(flag);
    std::transform(lower.begin(), lower.end(), lower.begin(),
                   [](wchar_t ch) { return static_cast<wchar_t>(std::towlower(ch)); });

    if (lower == L"n" || lower == L"new") {
        state.shellCommand = kShellCommandFileNew;
    } else if (lower == L"open") {
        state.shellCommand = kShellCommandFileOpen;
    } else if (lower == L"p" || lower == L"print") {
        state.shellCommand = kShellCommandFilePrint;
    } else if (lower == L"pt" || lower == L"printto") {
        state.shellCommand = kShellCommandFilePrintTo;
    } else if (lower == L"dde") {
        state.shellCommand = kShellCommandFileDDE;
    } else if (lower == L"register") {
        state.shellCommand = kShellCommandAppRegister;
    } else if (lower == L"unregister") {
        state.shellCommand = kShellCommandAppUnregister;
    }
}
void ParseNonFlagToken(CommandLineInfoState& state, const wchar_t* token) {
    if (token == nullptr || *token == L'\0') return;
    state.fileName = token;
    state.hasFileName = true;
    if (state.shellCommand == kShellCommandFileNothing || state.shellCommand == kShellCommandFileNew) {
        state.shellCommand = kShellCommandFileOpen;
    }
}
CDocument* OpenWithTemplates(const std::vector<CDocTemplate*>& templates, const wchar_t* path, int makeVisible) {
    for (CDocTemplate* tpl : templates) {
        if (tpl == nullptr) continue;
        CDocument* doc = tpl->OpenDocumentFile(path, makeVisible);
        if (doc != nullptr) return doc;
    }
    return nullptr;
}
HCURSOR GetWaitCursorHandle() {
    static HCURSOR s_wait = ::LoadCursorW(nullptr, MAKEINTRESOURCEW(IDC_WAIT));
    return s_wait;
}
HCURSOR GetArrowCursorHandle() {
    static HCURSOR s_arrow = ::LoadCursorW(nullptr, MAKEINTRESOURCEW(IDC_ARROW));
    return s_arrow;
}
const AFX_MSGMAP_ENTRY g_emptyAppMessageEntries[] = {
    {0, 0, 0, 0, AfxSig_end, (AFX_PMSG)0}
};
const AFX_MSGMAP g_emptyAppMessageMap = {
    nullptr,
    g_emptyAppMessageEntries
};
DWORD WINAPI OpenMfcWinThreadEntry(LPVOID param) {
    CWinThread* thread = reinterpret_cast<CWinThread*>(param);
    if (!thread) return 0;
    g_pCurrentThread = thread;
    UINT result = 0;
    if (thread->InitInstance()) {
        result = static_cast<UINT>(thread->Run());
    }
    result = static_cast<UINT>(thread->ExitInstance());
    if (thread->m_bAutoDelete) {
        delete thread;
    }
    g_pCurrentThread = nullptr;
    return result;
}
} } }  // namespace openmfc::detail::cwinapp
