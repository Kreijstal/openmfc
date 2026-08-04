#pragma once
// Shared internals of the former appcore.cpp translation unit.
// Definitions live in detail/CWinAppSupport.cpp, so file-scope state keeps a
// single instance across the classes this unit was split into.
namespace openmfc { namespace detail { namespace cwinapp {} } }
using namespace openmfc::detail::cwinapp;
// Types declared by the internals of other units.
struct AFX_MODULE_STATE;
// App Core Implementation (CWinApp, CWinThread)
//
// Implements core application and thread classes.
// Currently stubs to satisfy linker dependencies.

// Define OPENMFC_APPCORE_IMPL to use extern declarations instead of inline stubs
#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxwin.h"
#include "openmfc/afxole.h"
#include <windows.h>
#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <cwctype>
#include <new>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

// MS ABI calling convention
#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

extern "C" void MS_ABI impl__OnHelp_CWinApp__IEAAXXZ(CWinApp* pThis);
extern "C" void MS_ABI impl__UnregisterShellFileTypes_CWinApp__IEAAXXZ(CWinApp* pThis);
extern "C" void MS_ABI impl__WinHelpInternal_CWinApp__UEAAX_KI_Z(CWinApp* pThis, unsigned long long data, unsigned int command);


namespace openmfc { namespace detail { namespace cwinapp {
extern CRuntimeClass* const g_coreRuntimeClasses[];
} } }

namespace openmfc { namespace detail { namespace cwinapp {
bool RuntimeClassNameEquals(const char* lhs, const char* rhs);
} } }

namespace openmfc { namespace detail { namespace cwinapp {
bool RuntimeClassNameEquals(const char* lhs, const wchar_t* rhs);
} } }

namespace openmfc { namespace detail { namespace cwinapp {
CRuntimeClass* FindRuntimeClassByName(const char* name);
} } }

namespace openmfc { namespace detail { namespace cwinapp {
CRuntimeClass* FindRuntimeClassByName(const wchar_t* name);
} } }










// =============================================================================
// Forward declarations from wincore.cpp
// =============================================================================

// Cleanup stale temporary CWnd wrappers (called during idle processing)
void OpenMfcCleanupTempWrappers();

// =============================================================================
// CWinThread Implementation
// =============================================================================

// Core class implementations
// CWnd moved to wincore.cpp
// CWinThread and CWinApp implementations are further down

// Create MSVC symbol aliases for static class members
// These are needed because MSVC client code expects MSVC-mangled names
#ifdef __GNUC__
// CCmdTarget::classCCmdTarget
asm(".globl \"?classCCmdTarget@CCmdTarget@@2UCRuntimeClass@@A\"\n"
    ".set \"?classCCmdTarget@CCmdTarget@@2UCRuntimeClass@@A\", _ZN10CCmdTarget15classCCmdTargetE\n");
// CWinThread::classCWinThread
asm(".globl \"?classCWinThread@CWinThread@@2UCRuntimeClass@@A\"\n"
    ".set \"?classCWinThread@CWinThread@@2UCRuntimeClass@@A\", _ZN10CWinThread15classCWinThreadE\n");
// CWinApp::classCWinApp
asm(".globl \"?classCWinApp@CWinApp@@2UCRuntimeClass@@A\"\n"
    ".set \"?classCWinApp@CWinApp@@2UCRuntimeClass@@A\", _ZN7CWinApp12classCWinAppE\n");
#endif


#ifdef __GNUC__
asm(".globl \"?GetThisClass@CWinThread@@SAPEAUCRuntimeClass@@XZ\"\n"
    ".set \"?GetThisClass@CWinThread@@SAPEAUCRuntimeClass@@XZ\", impl__GetThisClass_CWinThread__SAPEAUCRuntimeClass__XZ\n");
#endif


#ifdef __GNUC__
asm(".globl \"?GetThisClass@CWinApp@@SAPEAUCRuntimeClass@@XZ\"\n"
    ".set \"?GetThisClass@CWinApp@@SAPEAUCRuntimeClass@@XZ\", impl__GetThisClass_CWinApp__SAPEAUCRuntimeClass__XZ\n");
#endif


#ifdef __GNUC__
asm(".globl \"?GetThisClass@CCmdTarget@@SAPEAUCRuntimeClass@@XZ\"\n"
    ".set \"?GetThisClass@CCmdTarget@@SAPEAUCRuntimeClass@@XZ\", impl__GetThisClass_CCmdTarget__SAPEAUCRuntimeClass__XZ\n");
#endif

// =============================================================================
// CCmdTarget Implementation
// =============================================================================

// CCmdTarget destructor

// Exported stub for CCmdTarget destructor
namespace openmfc { namespace detail { namespace cwinapp {
extern "C" void MS_ABI impl___1CCmdTarget__UEAA_XZ(CCmdTarget* pThis);
} } }

// CCmdTarget message map (root)






// Exception implementations

// CFileException::GetErrorMessage implementation

// Define MS_ABI if not defined
#ifdef __GNUC__
  #ifndef MS_ABI
    #define MS_ABI __attribute__((ms_abi))
  #endif
#else
  #define MS_ABI
#endif


#ifdef __GNUC__
asm(".globl \"?GetErrorMessage@CFileException@@UEBAHPEA_WIPEAI@Z\"\n"
    ".set \"?GetErrorMessage@CFileException@@UEBAHPEA_WIPEAI@Z\", impl__GetErrorMessage_CFileException__UEBAHPEA_WIPEAI_Z\n");
#endif


#ifdef __GNUC__
asm(".globl \"?GetErrorMessage@CArchiveException@@UEBAHPEA_WIPEAI@Z\"\n"
    ".set \"?GetErrorMessage@CArchiveException@@UEBAHPEA_WIPEAI@Z\", impl__GetErrorMessage_CArchiveException__UEBAHPEA_WIPEAI_Z\n");
#endif


#ifdef __GNUC__
asm(".globl \"?GetThisClass@CMemoryException@@SAPEAUCRuntimeClass@@XZ\"\n"
    ".set \"?GetThisClass@CMemoryException@@SAPEAUCRuntimeClass@@XZ\", impl__GetThisClass_CMemoryException__SAPEAUCRuntimeClass__XZ\n");
#endif

#ifdef __GNUC__
// CMemoryException::classCMemoryException - MSVC symbol alias
// Note: GCC mangling uses 21 for the member name length (includes the full count)
asm(".globl \"?classCMemoryException@CMemoryException@@2UCRuntimeClass@@A\"\n"
    ".set \"?classCMemoryException@CMemoryException@@2UCRuntimeClass@@A\", _ZN16CMemoryException21classCMemoryExceptionE\n");

// CFileException::classCFileException - MSVC symbol alias
// Note: GCC mangling uses 19 for the member name length
asm(".globl \"?classCFileException@CFileException@@2UCRuntimeClass@@A\"\n"
    ".set \"?classCFileException@CFileException@@2UCRuntimeClass@@A\", _ZN14CFileException19classCFileExceptionE\n");
#endif


#ifdef __GNUC__
asm(".globl \"?GetThisClass@CFileException@@SAPEAUCRuntimeClass@@XZ\"\n"
    ".set \"?GetThisClass@CFileException@@SAPEAUCRuntimeClass@@XZ\", impl__GetThisClass_CFileException__SAPEAUCRuntimeClass__XZ\n");
#endif


#ifdef __GNUC__
// CArchiveException::classCArchiveException - MSVC symbol alias
// Note: GCC mangling uses 22 for the member name length
asm(".globl \"?classCArchiveException@CArchiveException@@2UCRuntimeClass@@A\"\n"
    ".set \"?classCArchiveException@CArchiveException@@2UCRuntimeClass@@A\", _ZN17CArchiveException22classCArchiveExceptionE\n");
#endif


#ifdef __GNUC__
asm(".globl \"?GetThisClass@CArchiveException@@SAPEAUCRuntimeClass@@XZ\"\n"
    ".set \"?GetThisClass@CArchiveException@@SAPEAUCRuntimeClass@@XZ\", impl__GetThisClass_CArchiveException__SAPEAUCRuntimeClass__XZ\n");
#endif




#ifdef __GNUC__
// CWinThread::classCWinThread - MSVC symbol alias
asm(".globl \"?classCWinThread@CWinThread@@2UCRuntimeClass@@A\"\n"
    ".set \"?classCWinThread@CWinThread@@2UCRuntimeClass@@A\", _ZN10CWinThread15classCWinThreadE\n");
#endif






namespace openmfc { namespace detail { namespace cwinapp {
int OpenMfcRunExportedMessageLoop(CWinThread* pThis);
} } }













// =============================================================================
// CWinApp Implementation
// =============================================================================


#ifdef __GNUC__
// CWinApp::classCWinApp - MSVC symbol alias
asm(".globl \"?classCWinApp@CWinApp@@2UCRuntimeClass@@A\"\n"
    ".set \"?classCWinApp@CWinApp@@2UCRuntimeClass@@A\", _ZN7CWinApp12classCWinAppE\n");
#endif

// CWinApp methods are defined inline in afxwin.h
// Source-backed defaults are provided here for _AFXDLL consumers.
extern CWinApp* g_pApp;







// Global application pointer (exported via openmfc_exports.cpp)
extern CWinApp* g_pApp;

// Thread-local storage for current thread (used by worker threads)
// This is set by AfxBeginThread in synccore.cpp for worker threads
extern __thread CWinThread* g_pCurrentThread;

// AfxGetThread - returns current thread (or app for main thread)
// C++ implementation for internal use
CWinThread* AfxGetThread();


// =============================================================================
// CWinApp Constructor/Destructor
// =============================================================================





// Additional global state needed for AfxGetInstanceHandle, etc.
namespace openmfc { namespace detail { namespace cwinapp {
extern HINSTANCE g_hInstance;
} } }
namespace openmfc { namespace detail { namespace cwinapp {
extern HINSTANCE g_hResource;
} } }
constexpr size_t kOpaqueStateWordCount = 32;
namespace openmfc { namespace detail { namespace cwinapp {
extern void* g_appModuleStateStorage[kOpaqueStateWordCount];
} } }
struct _AFX_THREAD_STATE {
    int nTempMapLock = 0;
    int nWndCreateLock = 0;
    void* pModuleState = nullptr;
};
namespace openmfc { namespace detail { namespace cwinapp {
extern __thread _AFX_THREAD_STATE g_threadStateStorage;
} } }

// AfxGetInstanceHandle implementation
HINSTANCE AFXAPI AfxGetInstanceHandle();

// AfxGetResourceHandle implementation
HINSTANCE AFXAPI AfxGetResourceHandle();


namespace openmfc { namespace detail { namespace cwinapp {
extern "C" HINSTANCE MS_ABI impl__AfxGetInstanceHandleHelper__YAPEAUHINSTANCE____XZ();
} } }

// AfxSetResourceHandle implementation
void AFXAPI AfxSetResourceHandle(HINSTANCE hInstResource);


namespace openmfc { namespace detail { namespace cwinapp {
extern "C" AFX_MODULE_STATE* MS_ABI impl__AfxGetAppModuleState__YAPEAVAFX_MODULE_STATE__XZ();
} } }


namespace openmfc { namespace detail { namespace cwinapp {
extern "C" _AFX_THREAD_STATE* MS_ABI impl__AfxGetThreadState__YAPEAV_AFX_THREAD_STATE__XZ();
} } }




namespace openmfc { namespace detail { namespace cwinapp {
extern "C" int MS_ABI impl__AfxWinInit__YAHPEAUHINSTANCE____0PEA_WH_Z(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    wchar_t* lpCmdLine,
    int nCmdShow);
} } }


namespace openmfc { namespace detail { namespace cwinapp {
extern "C" CWnd* MS_ABI impl__AfxGetMainWnd__YAPEAVCWnd__XZ();
} } }

// Note: AfxGetThread, AfxGetInstanceHandle, AfxGetResourceHandle,
// AfxSetResourceHandle, AfxGetMainWnd, and AfxWinInit are now
// inline functions defined in afxwin.h

// =============================================================================
// Conservative app/runtime residual helpers
// =============================================================================
class CCommandLineInfo;
class CDocManager;
class CRecentFileList;

namespace openmfc { namespace detail { namespace cwinapp {
struct CommandLineInfoState {
    int shellCommand = 0;
    int openRecentIndex = -1;
    bool hasFileName = false;
    std::wstring fileName;
};
} } }

namespace openmfc { namespace detail { namespace cwinapp {
struct DocManagerState {
    std::vector<CDocTemplate*> templates;
    bool shellFileTypesRegistered = false;
};
} } }

namespace openmfc { namespace detail { namespace cwinapp {
struct RecentFileListState {
    UINT start = 0;
    int maxEntries = 0;
    int maxDisplayLen = 0;
    std::wstring section;
    std::wstring entryFormat;
    std::vector<std::wstring> entries;
};
} } }

namespace openmfc { namespace detail { namespace cwinapp {
struct AppRuntimeState {
    std::vector<CDocTemplate*> templates;
    std::vector<std::wstring> recentEntries;
    unsigned int maxRecent = 4;
    unsigned char dataRecoveryHandler{};
    std::unordered_map<std::wstring, unsigned int> profileInts;
    std::unordered_map<std::wstring, std::wstring> profileStrings;
    std::unordered_map<std::wstring, std::vector<unsigned char>> profileBinary;
    std::wstring registryRoot;
    std::wstring appId;
    bool modelessEnabled = true;
    bool shellOpenEnabled = false;
    bool taskbarEnabled = false;
    bool d2dEnabled = false;
    bool automated = false;
    bool embedded = false;
    HGLOBAL printerDevMode = nullptr;
    HGLOBAL printerDevNames = nullptr;
    int selectedPrinter = 0;
    ITaskbarList* taskbarList = nullptr;
    ITaskbarList3* taskbarList3 = nullptr;
};
} } }

namespace openmfc { namespace detail { namespace cwinapp {
extern std::unordered_map<const CCommandLineInfo*, CommandLineInfoState> g_commandLineInfoStates;
} } }
namespace openmfc { namespace detail { namespace cwinapp {
extern std::unordered_map<const CDocManager*, DocManagerState> g_docManagerStates;
} } }
namespace openmfc { namespace detail { namespace cwinapp {
extern std::unordered_map<const CRecentFileList*, RecentFileListState> g_recentFileListStates;
} } }
namespace openmfc { namespace detail { namespace cwinapp {
extern std::unordered_map<const CWinApp*, AppRuntimeState> g_appRuntimeStates;
} } }
namespace openmfc { namespace detail { namespace cwinapp {
extern std::unordered_map<const CCmdTarget*, int> g_waitCursorDepth;
} } }

namespace openmfc { namespace detail { namespace cwinapp {
extern CRuntimeClass g_classCDocManager;
} } }

namespace openmfc { namespace detail { namespace cwinapp {
constexpr int kShellCommandFileNothing = 0;
} } }
namespace openmfc { namespace detail { namespace cwinapp {
constexpr int kShellCommandFileNew = 1;
} } }
namespace openmfc { namespace detail { namespace cwinapp {
constexpr int kShellCommandFileOpen = 2;
} } }
namespace openmfc { namespace detail { namespace cwinapp {
constexpr int kShellCommandFilePrint = 3;
} } }
namespace openmfc { namespace detail { namespace cwinapp {
constexpr int kShellCommandFilePrintTo = 4;
} } }
namespace openmfc { namespace detail { namespace cwinapp {
constexpr int kShellCommandAppRegister = 5;
} } }
namespace openmfc { namespace detail { namespace cwinapp {
constexpr int kShellCommandAppUnregister = 6;
} } }
namespace openmfc { namespace detail { namespace cwinapp {
constexpr int kShellCommandFileDDE = 7;
} } }

namespace openmfc { namespace detail { namespace cwinapp {
std::wstring MakeProfileKey(const wchar_t* section, const wchar_t* entry);
} } }

namespace openmfc { namespace detail { namespace cwinapp {
constexpr unsigned int kDefaultRecentFileMenuFirstId = 0xE110;
} } }

namespace openmfc { namespace detail { namespace cwinapp {
int AppRecentIndexFromCommand(const AppRuntimeState& state, unsigned int idOrIndex);
} } }

namespace openmfc { namespace detail { namespace cwinapp {
std::wstring WideValue(const wchar_t* value);
} } }

namespace openmfc { namespace detail { namespace cwinapp {
std::wstring LowerWide(std::wstring value);
} } }

namespace openmfc { namespace detail { namespace cwinapp {
std::wstring GetAppRegistryRoot(CWinApp* app);
} } }

namespace openmfc { namespace detail { namespace cwinapp {
std::wstring GetAppName(CWinApp* app);
} } }

namespace openmfc { namespace detail { namespace cwinapp {
bool ReadDwordRegistryValue(HKEY root, const wchar_t* subKey, const wchar_t* valueName, unsigned long* value);
} } }

namespace openmfc { namespace detail { namespace cwinapp {
bool ReadPolicyValueFromRegistry(const std::wstring& appName, unsigned long policy, int* value);
} } }

namespace openmfc { namespace detail { namespace cwinapp {
HWND GetAppMainHwnd(CWinApp* app);
} } }

namespace openmfc { namespace detail { namespace cwinapp {
int CountTemplateDocuments(CDocTemplate* tpl);
} } }

namespace openmfc { namespace detail { namespace cwinapp {
bool SaveTemplateDocuments(CDocTemplate* tpl);
} } }

namespace openmfc { namespace detail { namespace cwinapp {
void CloseTemplateDocuments(CDocTemplate* tpl);
} } }

namespace openmfc { namespace detail { namespace cwinapp {
const wchar_t* FindFileNamePart(const wchar_t* path);
} } }

namespace openmfc { namespace detail { namespace cwinapp {
void AddRecentPath(std::vector<std::wstring>& entries, int maxEntries, const wchar_t* path);
} } }

namespace openmfc { namespace detail { namespace cwinapp {
std::vector<std::wstring> TokenizeCommandLine(const wchar_t* cmdLine);
} } }

namespace openmfc { namespace detail { namespace cwinapp {
void ParseCommandFlag(CommandLineInfoState& state, const wchar_t* flag);
} } }

namespace openmfc { namespace detail { namespace cwinapp {
void ParseNonFlagToken(CommandLineInfoState& state, const wchar_t* token);
} } }

namespace openmfc { namespace detail { namespace cwinapp {
CDocument* OpenWithTemplates(const std::vector<CDocTemplate*>& templates, const wchar_t* path, int makeVisible);
} } }






















namespace openmfc { namespace detail { namespace cwinapp {
HCURSOR GetWaitCursorHandle();
} } }

namespace openmfc { namespace detail { namespace cwinapp {
HCURSOR GetArrowCursorHandle();
} } }














































namespace openmfc { namespace detail { namespace cwinapp {
extern const AFX_MSGMAP_ENTRY g_emptyAppMessageEntries[];
} } }

namespace openmfc { namespace detail { namespace cwinapp {
extern const AFX_MSGMAP g_emptyAppMessageMap;
} } }

namespace openmfc { namespace detail { namespace cwinapp {
DWORD WINAPI OpenMfcWinThreadEntry(LPVOID param);
} } }






































































