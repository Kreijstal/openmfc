// Globals — OpenMFC implementation.
// Sources: global_afx_messages.cpp, global_afx_path_helpers.cpp, global_dumpcontext_operators.cpp, global_memory_hash_operators.cpp, global_string_manager.cpp, memcore.cpp, regcore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/AfxMessagesSupport.h"
#include "detail/MemcoreSupport.h"
#include "detail/RegcoreSupport.h"

// Global residuals shard 06/12: global
//
// Real implementations for a small set of global AFX helper functions whose
// behavior is unambiguous from the MFC contract. Conservative: only thin Win32
// forwards / pure path helpers are implemented here.

#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxmfc.h"

#include <windows.h>
#include <cstring>
#include <cwchar>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif



#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxmfc.h"

#include <cstddef>
#include <cstdlib>
#include <new>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif


#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxmfc.h"

#include <cstddef>
#include <cstdlib>
#include <new>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

// Global scalar/array operator new[] and operator delete[].
// MFC re-exports these; forward to the C++ runtime allocator.



// MFC HashKey<> template helpers for string pointer keys.
// Standard MFC hash: nHash = (nHash << 5) + nHash + ch  (i.e. *33).


#include "openmfc/afxstr.h"
#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif
// Symbol: ?AFX_WM_AFTER_TASKBAR_ACTIVATE@@3IA
extern "C" unsigned int impl__AFX_WM_AFTER_TASKBAR_ACTIVATE__3IA =
    RegisterAfxMessage(L"AFX_WM_AFTER_TASKBAR_ACTIVATE");
// Symbol: ?AFX_WM_CHANGE_ACTIVE_TAB@@3IA
extern "C" unsigned int impl__AFX_WM_CHANGE_ACTIVE_TAB__3IA =
    RegisterAfxMessage(L"AFX_WM_CHANGE_ACTIVE_TAB");
// Symbol: ?AFX_WM_CHANGE_CURRENT_FOLDER@@3IA
extern "C" unsigned int impl__AFX_WM_CHANGE_CURRENT_FOLDER__3IA =
    RegisterAfxMessage(L"AFX_WM_CHANGE_CURRENT_FOLDER");
// Symbol: ?AFX_WM_CHANGEVISUALMANAGER@@3IA
extern "C" unsigned int impl__AFX_WM_CHANGEVISUALMANAGER__3IA =
    RegisterAfxMessage(L"AFX_WM_CHANGEVISUALMANAGER");
// Symbol: ?AFX_WM_CHANGING_ACTIVE_TAB@@3IA
extern "C" unsigned int impl__AFX_WM_CHANGING_ACTIVE_TAB__3IA =
    RegisterAfxMessage(L"AFX_WM_CHANGING_ACTIVE_TAB");
// Symbol: ?AFX_WM_CHECKEMPTYMINIFRAME@@3IA
extern "C" unsigned int impl__AFX_WM_CHECKEMPTYMINIFRAME__3IA =
    RegisterAfxMessage(L"AFX_WM_CHECKEMPTYMINIFRAME");
// Symbol: ?AFX_WM_CREATETOOLBAR@@3IA
extern "C" unsigned int impl__AFX_WM_CREATETOOLBAR__3IA =
    RegisterAfxMessage(L"AFX_WM_CREATETOOLBAR");
// Symbol: ?AFX_WM_CUSTOMIZEHELP@@3IA
extern "C" unsigned int impl__AFX_WM_CUSTOMIZEHELP__3IA =
    RegisterAfxMessage(L"AFX_WM_CUSTOMIZEHELP");
// Symbol: ?AFX_WM_CUSTOMIZETOOLBAR@@3IA
extern "C" unsigned int impl__AFX_WM_CUSTOMIZETOOLBAR__3IA =
    RegisterAfxMessage(L"AFX_WM_CUSTOMIZETOOLBAR");
// Symbol: ?AFX_WM_DELETETOOLBAR@@3IA
extern "C" unsigned int impl__AFX_WM_DELETETOOLBAR__3IA =
    RegisterAfxMessage(L"AFX_WM_DELETETOOLBAR");
// Symbol: ?AFX_WM_DRAW2D@@3IA
extern "C" unsigned int impl__AFX_WM_DRAW2D__3IA =
    RegisterAfxMessage(L"AFX_WM_DRAW2D");
// Symbol: ?AFX_WM_GETDOCUMENTCOLORS@@3IA
extern "C" unsigned int impl__AFX_WM_GETDOCUMENTCOLORS__3IA =
    RegisterAfxMessage(L"AFX_WM_GETDOCUMENTCOLORS");
// Symbol: ?AFX_WM_GETDRAGBOUNDS@@3IA
extern "C" unsigned int impl__AFX_WM_GETDRAGBOUNDS__3IA =
    RegisterAfxMessage(L"AFX_WM_GETDRAGBOUNDS");
// Symbol: ?AFX_WM_ON_AFTER_SHELL_COMMAND@@3IA
extern "C" unsigned int impl__AFX_WM_ON_AFTER_SHELL_COMMAND__3IA =
    RegisterAfxMessage(L"AFX_WM_ON_AFTER_SHELL_COMMAND");
// Symbol: ?AFX_WM_ON_BEFORE_SHOW_RIBBON_ITEM_MENU@@3IA
extern "C" unsigned int impl__AFX_WM_ON_BEFORE_SHOW_RIBBON_ITEM_MENU__3IA =
    RegisterAfxMessage(L"AFX_WM_ON_BEFORE_SHOW_RIBBON_ITEM_MENU");
// Symbol: ?AFX_WM_ON_CANCELTABMOVE@@3IA
extern "C" unsigned int impl__AFX_WM_ON_CANCELTABMOVE__3IA =
    RegisterAfxMessage(L"AFX_WM_ON_CANCELTABMOVE");
// Symbol: ?AFX_WM_ON_CHANGE_RIBBON_CATEGORY@@3IA
extern "C" unsigned int impl__AFX_WM_ON_CHANGE_RIBBON_CATEGORY__3IA =
    RegisterAfxMessage(L"AFX_WM_ON_CHANGE_RIBBON_CATEGORY");
// Symbol: ?AFX_WM_ON_CLOSEPOPUPWINDOW@@3IA
extern "C" unsigned int impl__AFX_WM_ON_CLOSEPOPUPWINDOW__3IA =
    RegisterAfxMessage(L"AFX_WM_ON_CLOSEPOPUPWINDOW");
// Symbol: ?AFX_WM_ON_DRAGCOMPLETE@@3IA
extern "C" unsigned int impl__AFX_WM_ON_DRAGCOMPLETE__3IA =
    RegisterAfxMessage(L"AFX_WM_ON_DRAGCOMPLETE");
// Symbol: ?AFX_WM_ON_GET_TAB_TOOLTIP@@3IA
extern "C" unsigned int impl__AFX_WM_ON_GET_TAB_TOOLTIP__3IA =
    RegisterAfxMessage(L"AFX_WM_ON_GET_TAB_TOOLTIP");
// Symbol: ?AFX_WM_ON_HIGHLIGHT_RIBBON_LIST_ITEM@@3IA
extern "C" unsigned int impl__AFX_WM_ON_HIGHLIGHT_RIBBON_LIST_ITEM__3IA =
    RegisterAfxMessage(L"AFX_WM_ON_HIGHLIGHT_RIBBON_LIST_ITEM");
// Symbol: ?AFX_WM_ON_HSCROLL@@3IA
extern "C" unsigned int impl__AFX_WM_ON_HSCROLL__3IA =
    RegisterAfxMessage(L"AFX_WM_ON_HSCROLL");
// Symbol: ?AFX_WM_ON_MOVE_TAB@@3IA
extern "C" unsigned int impl__AFX_WM_ON_MOVE_TAB__3IA =
    RegisterAfxMessage(L"AFX_WM_ON_MOVE_TAB");
// Symbol: ?AFX_WM_ON_MOVETABCOMPLETE@@3IA
extern "C" unsigned int impl__AFX_WM_ON_MOVETABCOMPLETE__3IA =
    RegisterAfxMessage(L"AFX_WM_ON_MOVETABCOMPLETE");
// Symbol: ?AFX_WM_ON_MOVETOTABGROUP@@3IA
extern "C" unsigned int impl__AFX_WM_ON_MOVETOTABGROUP__3IA =
    RegisterAfxMessage(L"AFX_WM_ON_MOVETOTABGROUP");
// Symbol: ?AFX_WM_ON_PRESS_CLOSE_BUTTON@@3IA
extern "C" unsigned int impl__AFX_WM_ON_PRESS_CLOSE_BUTTON__3IA =
    RegisterAfxMessage(L"AFX_WM_ON_PRESS_CLOSE_BUTTON");
// Symbol: ?AFX_WM_ON_RENAME_TAB@@3IA
extern "C" unsigned int impl__AFX_WM_ON_RENAME_TAB__3IA =
    RegisterAfxMessage(L"AFX_WM_ON_RENAME_TAB");
// Symbol: ?AFX_WM_ON_RIBBON_CUSTOMIZE@@3IA
extern "C" unsigned int impl__AFX_WM_ON_RIBBON_CUSTOMIZE__3IA =
    RegisterAfxMessage(L"AFX_WM_ON_RIBBON_CUSTOMIZE");
// Symbol: ?AFX_WM_ON_TABGROUPMOUSEMOVE@@3IA
extern "C" unsigned int impl__AFX_WM_ON_TABGROUPMOUSEMOVE__3IA =
    RegisterAfxMessage(L"AFX_WM_ON_TABGROUPMOUSEMOVE");
// Symbol: ?AFX_WM_POSTRECALCLAYOUT@@3IA
extern "C" unsigned int impl__AFX_WM_POSTRECALCLAYOUT__3IA =
    RegisterAfxMessage(L"AFX_WM_POSTRECALCLAYOUT");
// Symbol: ?AFX_WM_POSTSETPREVIEWFRAME@@3IA
extern "C" unsigned int impl__AFX_WM_POSTSETPREVIEWFRAME__3IA =
    RegisterAfxMessage(L"AFX_WM_POSTSETPREVIEWFRAME");
// Symbol: ?AFX_WM_PROPERTY_CHANGED@@3IA
extern "C" unsigned int impl__AFX_WM_PROPERTY_CHANGED__3IA =
    RegisterAfxMessage(L"AFX_WM_PROPERTY_CHANGED");
// Symbol: ?AFX_WM_RECREATED2DRESOURCES@@3IA
extern "C" unsigned int impl__AFX_WM_RECREATED2DRESOURCES__3IA =
    RegisterAfxMessage(L"AFX_WM_RECREATED2DRESOURCES");
// Symbol: ?AFX_WM_RESETCONTEXTMENU@@3IA
extern "C" unsigned int impl__AFX_WM_RESETCONTEXTMENU__3IA =
    RegisterAfxMessage(L"AFX_WM_RESETCONTEXTMENU");
// Symbol: ?AFX_WM_RESETKEYBOARD@@3IA
extern "C" unsigned int impl__AFX_WM_RESETKEYBOARD__3IA =
    RegisterAfxMessage(L"AFX_WM_RESETKEYBOARD");
// Symbol: ?AFX_WM_RESETMENU@@3IA
extern "C" unsigned int impl__AFX_WM_RESETMENU__3IA =
    RegisterAfxMessage(L"AFX_WM_RESETMENU");
// Symbol: ?AFX_WM_RESETRPROMPT@@3IA
extern "C" unsigned int impl__AFX_WM_RESETRPROMPT__3IA =
    RegisterAfxMessage(L"AFX_WM_RESETRPROMPT");
// Symbol: ?AFX_WM_RESETTOOLBAR@@3IA
extern "C" unsigned int impl__AFX_WM_RESETTOOLBAR__3IA =
    RegisterAfxMessage(L"AFX_WM_RESETTOOLBAR");
// Symbol: ?AFX_WM_SHOWREGULARMENU@@3IA
extern "C" unsigned int impl__AFX_WM_SHOWREGULARMENU__3IA =
    RegisterAfxMessage(L"AFX_WM_SHOWREGULARMENU");
// Symbol: ?AFX_WM_TOOLBARMENU@@3IA
extern "C" unsigned int impl__AFX_WM_TOOLBARMENU__3IA =
    RegisterAfxMessage(L"AFX_WM_TOOLBARMENU");
// Symbol: ?AFX_WM_UPDATETOOLTIPS@@3IA
extern "C" unsigned int impl__AFX_WM_UPDATETOOLTIPS__3IA =
    RegisterAfxMessage(L"AFX_WM_UPDATETOOLTIPS");
// Symbol: ?AFX_WM_WINDOW_HELP@@3IA
extern "C" unsigned int impl__AFX_WM_WINDOW_HELP__3IA =
    RegisterAfxMessage(L"AFX_WM_WINDOW_HELP");
// =============================================================================
// ?AfxIsDescendant@@YAHPEAUHWND__@@0@Z
//   int AfxIsDescendant(HWND hWndParent, HWND hWndChild)
// Returns nonzero if hWndChild is hWndParent itself or a descendant window.
// =============================================================================
// Symbol: ?AfxIsDescendant@@YAHPEAUHWND__@@0@Z
extern "C" int MS_ABI impl__AfxIsDescendant__YAHPEAUHWND____0_Z(HWND hWndParent,
                                                                HWND hWndChild)
{
    if (hWndParent == nullptr || hWndChild == nullptr)
        return 0;
    if (hWndParent == hWndChild)
        return 1;
    return ::IsChild(hWndParent, hWndChild) ? 1 : 0;
}
// =============================================================================
// ?AfxGetFileName@@YAIPEB_WPEA_WI@Z
//   UINT AfxGetFileName(LPCWSTR lpszPathName, LPWSTR lpszTitle, UINT nMax)
// Extracts the file name portion (with extension) of a path. If lpszTitle is
// NULL, returns the buffer length required (including the terminating null).
// Otherwise copies up to nMax characters and returns 0.
// =============================================================================
// Symbol: ?AfxGetFileName@@YAIPEB_WPEA_WI@Z
extern "C" unsigned int MS_ABI impl__AfxGetFileName__YAIPEB_WPEA_WI_Z(
    const wchar_t* lpszPathName, wchar_t* lpszTitle, unsigned int nMax)
{
    if (lpszPathName == nullptr)
        return 0;

    // Find the start of the file name: character after the last '\\', '/' or ':'.
    const wchar_t* lpszName = lpszPathName;
    for (const wchar_t* p = lpszPathName; *p != L'\0'; ++p) {
        if (*p == L'\\' || *p == L'/' || *p == L':')
            lpszName = p + 1;
    }

    size_t nLen = wcslen(lpszName);

    if (lpszTitle == nullptr)
        return static_cast<unsigned int>(nLen) + 1; // include terminating null

    if (nMax == 0)
        return 0;

    // Copy with truncation, always null-terminate (lstrcpyn semantics).
    unsigned int nCopy = static_cast<unsigned int>(nLen);
    if (nCopy > nMax - 1)
        nCopy = nMax - 1;
    if (nCopy > 0)
        wmemcpy(lpszTitle, lpszName, nCopy);
    lpszTitle[nCopy] = L'\0';
    return 0;
}
// =============================================================================
// ?AfxFullPath@@YAHPEA_WPEB_W@Z
//   BOOL AfxFullPath(LPWSTR lpszPathOut, LPCWSTR lpszFileIn)
// Resolves lpszFileIn to a fully-qualified path written to lpszPathOut, which
// must be at least _MAX_PATH characters. Returns nonzero on success.
// =============================================================================
// Symbol: ?AfxFullPath@@YAHPEA_WPEB_W@Z
extern "C" int MS_ABI impl__AfxFullPath__YAHPEA_WPEB_W_Z(wchar_t* lpszPathOut,
                                                         const wchar_t* lpszFileIn)
{
    if (lpszPathOut == nullptr || lpszFileIn == nullptr)
        return 0;

    wchar_t* lpszFilePart = nullptr;
    DWORD dwRet = ::GetFullPathNameW(lpszFileIn, MAX_PATH, lpszPathOut, &lpszFilePart);
    // Success requires the result to fit within the MAX_PATH output buffer.
    return (dwRet != 0 && dwRet < MAX_PATH) ? 1 : 0;
}
// Symbol: ??2CNoTrackObject@@SAPEAX_K@Z
extern "C" void* MS_ABI impl___2CNoTrackObject__SAPEAX_K_Z(std::size_t size) {
    if (size == 0) {
        size = 1;
    }
    // Zero-init to match MFC's LMEM_ZEROINIT allocation (CNoTrackObject-derived
    // thread state assumes cleared members). A throwing operator new must never
    // return null, so raise on exhaustion rather than hand back nullptr.
    void* p = std::calloc(size, 1);
    if (!p) {
        throw std::bad_alloc();
    }
    return p;
}
// Symbol: ??3CNoTrackObject@@SAXPEAX@Z
extern "C" void MS_ABI impl___3CNoTrackObject__SAXPEAX_Z(void* ptr) {
    std::free(ptr);
}
// Implemented in memcore.cpp; this unit had a second, unlinked copy.
// Symbol: ??_U@YAPEAX_K@Z
// Symbol: ??_V@YAXPEAX@Z
// Symbol: ??$HashKey@PEB_W@@YAIPEB_W@Z
extern "C" unsigned int MS_ABI impl____HashKey_PEB_W__YAIPEB_W_Z(const wchar_t* key) {
    unsigned int nHash = 0;
    if (key) {
        while (*key) {
            nHash = (nHash << 5) + nHash + static_cast<unsigned int>(*key++);
        }
    }
    return nHash;
}
// Symbol: ??$HashKey@PEBD@@YAIPEBD@Z
extern "C" unsigned int MS_ABI impl____HashKey_PEBD__YAIPEBD_Z(const char* key) {
    unsigned int nHash = 0;
    if (key) {
        while (*key) {
            nHash = (nHash << 5) + nHash +
                    static_cast<unsigned int>(static_cast<unsigned char>(*key++));
        }
    }
    return nHash;
}
// Symbol: ?AfxGetStringManager@@YAPEAUIAtlStringMgr@ATL@@XZ
extern "C" IAtlStringMgr* MS_ABI impl__AfxGetStringManager__YAPEAUIAtlStringMgr_ATL__XZ() {
    return OpenMFC_GetStringMgr();
}
// AfxLoadLibrary - Load a DLL with MFC-style error handling
// Symbol: ?AfxLoadLibrary@@YAPEAUHINSTANCE__@@PEB_W@Z
// Ordinal: 2260
extern "C" HINSTANCE MS_ABI impl__AfxLoadLibrary__YAPEAUHINSTANCE____PEB_W_Z(const wchar_t* lpszModuleName) {
    if (!lpszModuleName) return nullptr;
    return ::LoadLibraryW(lpszModuleName);
}
// AfxLoadLibraryEx - Load a DLL with extended options
// Symbol: ?AfxLoadLibraryEx@@YAPEAUHINSTANCE__@@PEB_WPEAXK@Z
// Ordinal: 2261
extern "C" HINSTANCE MS_ABI impl__AfxLoadLibraryEx__YAPEAUHINSTANCE____PEB_WPEAXK_Z(
    const wchar_t* lpszModuleName, void* hFile, unsigned long dwFlags) {
    if (!lpszModuleName) return nullptr;
    return ::LoadLibraryExW(lpszModuleName, (HANDLE)hFile, dwFlags);
}
// AfxFreeLibrary - Free a loaded library with MFC-style cleanup
// Symbol: ?AfxFreeLibrary@@YAHPEAUHINSTANCE__@@@Z
// Ordinal: 2193
extern "C" int MS_ABI impl__AfxFreeLibrary__YAHPEAUHINSTANCE_____Z(HINSTANCE hInstLib) {
    if (!hInstLib) return FALSE;
    return ::FreeLibrary(hInstLib) ? TRUE : FALSE;
}
// AfxFindResourceHandle - Find which module contains a resource
// Symbol: ?AfxFindResourceHandle@@YAPEAUHINSTANCE__@@PEB_W0@Z
// Ordinal: 2187
extern "C" HINSTANCE MS_ABI impl__AfxFindResourceHandle__YAPEAUHINSTANCE____PEB_W0_Z(
    const wchar_t* lpszResource, const wchar_t* lpszType) {
    // First, try the resource handle
    HINSTANCE hRes = AfxGetResourceHandle();
    if (hRes && lpszResource && lpszType) {
        if (::FindResourceW(hRes, lpszResource, lpszType)) {
            return hRes;
        }
    }
    // Fall back to instance handle
    HINSTANCE hInst = AfxGetInstanceHandle();
    if (hInst && lpszResource && lpszType) {
        if (::FindResourceW(hInst, lpszResource, lpszType)) {
            return hInst;
        }
    }
    // If still not found, check the main module
    HINSTANCE hMain = ::GetModuleHandleW(nullptr);
    if (hMain && hMain != hInst && hMain != hRes && lpszResource && lpszType) {
        if (::FindResourceW(hMain, lpszResource, lpszType)) {
            return hMain;
        }
    }
    return nullptr;
}
// AfxIsValidAddress - Check if a memory range is readable
// Symbol: ?AfxIsValidAddress@@YAHPEBX_KH@Z
// Ordinal: 2251
extern "C" int MS_ABI impl__AfxIsValidAddress__YAHPEBX_KH_Z(
    const void* lp, unsigned long long nBytes, int bReadWrite) {
    if (!lp || nBytes == 0) return FALSE;

    // Use VirtualQuery to check if the memory is accessible
    MEMORY_BASIC_INFORMATION mbi;
    SIZE_T result = ::VirtualQuery(lp, &mbi, sizeof(mbi));
    if (result == 0) return FALSE;

    // Check that the entire range is within the same allocation
    const char* start = static_cast<const char*>(lp);
    const char* end = start + nBytes - 1;
    const char* regionEnd = static_cast<const char*>(mbi.BaseAddress) + mbi.RegionSize - 1;
    if (end >= regionEnd) {
        // Range spans multiple regions; verify the next region exists
        MEMORY_BASIC_INFORMATION mbi2;
        if (::VirtualQuery(end, &mbi2, sizeof(mbi2)) == 0) return FALSE;
    }

    // Check access rights
    if (bReadWrite) {
        return (mbi.State == MEM_COMMIT &&
                (mbi.Protect & (PAGE_READWRITE | PAGE_EXECUTE_READWRITE))) ? TRUE : FALSE;
    } else {
        return (mbi.State == MEM_COMMIT &&
                (mbi.Protect & (PAGE_READONLY | PAGE_READWRITE |
                                PAGE_EXECUTE_READ | PAGE_EXECUTE_READWRITE))) ? TRUE : FALSE;
    }
}
// AfxIsValidString (wide) - Check if a wide string pointer is valid
// Symbol: ?AfxIsValidString@@YAHPEB_WH@Z
// Ordinal: 2254
extern "C" int MS_ABI impl__AfxIsValidString__YAHPEB_WH_Z(const wchar_t* lpsz, int nLength) {
    if (!lpsz) return FALSE;

    // Use VirtualQuery to check memory accessibility, then try reading
    MEMORY_BASIC_INFORMATION mbi;
    if (::VirtualQuery(lpsz, &mbi, sizeof(mbi)) == 0) return FALSE;
    if (mbi.State != MEM_COMMIT) return FALSE;
    if (mbi.Protect & (PAGE_NOACCESS | PAGE_GUARD)) return FALSE;

    if (nLength == -1) {
        // Check up to null terminator, limited to region size
        const wchar_t* end = static_cast<const wchar_t*>(mbi.BaseAddress) + mbi.RegionSize / sizeof(wchar_t);
        int i = 0;
        while (&lpsz[i] < end && i < 1048576) {  // 1M char limit
            if (lpsz[i] == L'\0') return TRUE;
            i++;
        }
        return FALSE;  // No null terminator found in region
    }

    // Verify nLength characters are within committed region
    const wchar_t* end = static_cast<const wchar_t*>(mbi.BaseAddress) + mbi.RegionSize / sizeof(wchar_t);
    return (&lpsz[nLength - 1] < end) ? TRUE : FALSE;
}
// AfxIsValidString (narrow) - Check if a narrow string pointer is valid
// Symbol: ?AfxIsValidString@@YAHPEBDH@Z
// Ordinal: 2255
extern "C" int MS_ABI impl__AfxIsValidString__YAHPEBDH_Z(const char* lpsz, int nLength) {
    if (!lpsz) return FALSE;

    MEMORY_BASIC_INFORMATION mbi;
    if (::VirtualQuery(lpsz, &mbi, sizeof(mbi)) == 0) return FALSE;
    if (mbi.State != MEM_COMMIT) return FALSE;
    if (mbi.Protect & (PAGE_NOACCESS | PAGE_GUARD)) return FALSE;

    if (nLength == -1) {
        const char* end = static_cast<const char*>(mbi.BaseAddress) + mbi.RegionSize;
        int i = 0;
        while (&lpsz[i] < end && i < 1048576) {
            if (lpsz[i] == '\0') return TRUE;
            i++;
        }
        return FALSE;
    }

    const char* end = static_cast<const char*>(mbi.BaseAddress) + mbi.RegionSize;
    return (&lpsz[nLength - 1] < end) ? TRUE : FALSE;
}
// AfxIsValidAtom (WORD) - Check if an atom is valid
// Symbol: ?AfxIsValidAtom@@YAHG@Z
// Ordinal: 2252
extern "C" int MS_ABI impl__AfxIsValidAtom__YAHG_Z(unsigned short nAtom) {
    if (nAtom == 0) return FALSE;
    // Try to find the atom
    wchar_t buf[256];
    return ::GlobalGetAtomNameW(nAtom, buf, 256) > 0 ? TRUE : FALSE;
}
// AfxIsValidAtom (string) - Check if a string atom name is valid
// Symbol: ?AfxIsValidAtom@@YAHPEB_W@Z
// Ordinal: 2253
extern "C" int MS_ABI impl__AfxIsValidAtom__YAHPEB_W_Z(const wchar_t* lpszAtomName) {
    if (!lpszAtomName) return FALSE;
    ATOM atom = ::GlobalFindAtomW(lpszAtomName);
    return (atom != 0) ? TRUE : FALSE;
}
// AfxRegisterClass - Register a window class with MFC-style defaults
// Symbol: ?AfxRegisterClass@@YAHPEAUtagWNDCLASSW@@@Z
// Ordinal: 2311
extern "C" int MS_ABI impl__AfxRegisterClass__YAHPEAUtagWNDCLASSW___Z(WNDCLASSW* lpWndClass) {
    if (!lpWndClass) return FALSE;

    // Set default instance handle if not specified
    if (!lpWndClass->hInstance) {
        lpWndClass->hInstance = AfxGetInstanceHandle();
    }

    // Register the class
    ATOM atom = ::RegisterClassW(lpWndClass);
    if (atom == 0) {
        // Check if already registered
        DWORD err = ::GetLastError();
        if (err == ERROR_CLASS_ALREADY_EXISTS) {
            return TRUE;  // Already registered, that's ok
        }
        return FALSE;
    }

    // Track the registered class
    if (g_numRegisteredClasses < 256) {
        g_registeredClasses[g_numRegisteredClasses++] = atom;
    }

    return TRUE;
}
// AfxRegisterWndClass - Create and register a window class with given style/cursor/brush/icon
// Symbol: ?AfxRegisterWndClass@@YAPEB_WIPEAUHICON__@@PEAUHBRUSH__@@0@Z
// Ordinal: 2316
// NOTE: The returned pointer is valid for a small number of subsequent calls on the
// same thread (thread-local ring buffer). Callers that cache class names should copy.
extern "C" const wchar_t* MS_ABI impl__AfxRegisterWndClass__YAPEB_WIPEAUHICON____PEAUHBRUSH____0_Z(
    UINT nClassStyle, HCURSOR hCursor, HBRUSH hbrBackground, HICON hIcon)
{
    HINSTANCE hInst = AfxGetInstanceHandle();
    if (!hInst) {
        hInst = ::GetModuleHandle(nullptr);
    }

    // Generate a unique class name based on parameters
    // This mimics MFC's approach of creating class names like "Afx:00400000:b:XXXX:YYYY"
    wchar_t* className = NextWndClassNameBuffer();
    int written = _snwprintf(className, kWndClassNameLen, L"OpenMfc:%p:%x:%p:%p:%p",
             (void*)hInst, nClassStyle, (void*)hCursor, (void*)hbrBackground, (void*)hIcon);
    if (written < 0 || written >= (int)kWndClassNameLen) {
        className[kWndClassNameLen - 1] = L'\0';
    }

    // Check if already registered
    WNDCLASSW existingClass;
    if (::GetClassInfoW(hInst, className, &existingClass)) {
        return className;  // Already registered
    }

    // Fill in the WNDCLASS structure
    WNDCLASSW wndClass = {};
    wndClass.style = nClassStyle;
    wndClass.lpfnWndProc = ::DefWindowProcW;  // Default window proc
    wndClass.cbClsExtra = 0;
    wndClass.cbWndExtra = 0;
    wndClass.hInstance = hInst;
    wndClass.hIcon = hIcon ? hIcon : ::LoadIconW(nullptr, IDI_APPLICATION);
    wndClass.hCursor = hCursor ? hCursor : ::LoadCursorW(nullptr, IDC_ARROW);
    wndClass.hbrBackground = hbrBackground;
    wndClass.lpszMenuName = nullptr;
    wndClass.lpszClassName = className;

    // Register the class
    ATOM atom = ::RegisterClassW(&wndClass);
    if (atom == 0) {
        DWORD err = ::GetLastError();
        if (err == ERROR_CLASS_ALREADY_EXISTS) {
            return className;  // That's fine
        }
        return nullptr;  // Registration failed
    }

    // Track the registered class
    if (g_numRegisteredClasses < 256) {
        g_registeredClasses[g_numRegisteredClasses++] = atom;
    }

    return className;
}
// Symbol: ?AfxGetModuleState@@YAPEAVAFX_MODULE_STATE@@XZ
// Ordinal: 2212
// Return the current thread's module state; keep it synchronized with the
// process-wide app/module handles.
extern "C" AFX_MODULE_STATE* MS_ABI impl__AfxGetModuleState__YAPEAVAFX_MODULE_STATE__XZ() {
    AFX_MODULE_STATE* pState = ResolveCurrentModuleState();
    if (pState == &g_moduleStateStatic) {
        RefreshStaticModuleState();
    }
    return pState;
}
// Symbol: ?AfxSetModuleState@@YAPEAVAFX_MODULE_STATE@@PEAV1@@Z
// Ordinal: 2327
// Returns the previous module state and atomically switches the thread-local
// current module state pointer.
extern "C" AFX_MODULE_STATE* MS_ABI impl__AfxSetModuleState__YAPEAVAFX_MODULE_STATE__PEAV1__Z(AFX_MODULE_STATE* pNewState) {
    AFX_MODULE_STATE* pOldState = ResolveCurrentModuleState();
    g_pCurrentModuleState = pNewState ? pNewState : &g_moduleStateStatic;
    return pOldState;
}
// Symbol: ?AfxGetStaticModuleState@@YAPEAUAFX_MODULE_STATE@@XZ
// Ordinal: 2211
extern "C" AFX_MODULE_STATE* MS_ABI impl__AfxGetStaticModuleState__YAPEAUAFX_MODULE_STATE__XZ() {
    RefreshStaticModuleState();
    return &g_moduleStateStatic;
}
// Symbol: ?AfxGetModuleThreadState@@YAPEAVAFX_MODULE_THREAD_STATE@@XZ
// Ordinal: 2213
// Module thread state is per-thread process bookkeeping in real MFC.
extern "C" AFX_MODULE_THREAD_STATE_Regcore* MS_ABI impl__AfxGetModuleThreadState__YAPEAVAFX_MODULE_THREAD_STATE__XZ() {
    g_moduleThreadState.m_nTempMapLock = 0;
    return &g_moduleThreadState;
}
