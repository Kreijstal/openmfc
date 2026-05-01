// Memory core (global new/delete exported by MFC)
//
// MSVC clients may resolve global operator new/delete from mfc140u.dll.
// If we leave these as auto-generated "void" stubs, MSVC code will treat the
// uninitialized RAX value as a pointer and crash immediately.
//
// Also implements: AfxLoadLibrary, AfxFreeLibrary, AfxFindResourceHandle,
//   AfxIsValidAddress, AfxIsValidString, AfxIsValidAtom

#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxwin.h"
#include <windows.h>
#include <cstddef>
#include <cstdlib>
#include <cstring>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

// =============================================================================
// Global new/delete
// =============================================================================

extern "C" void* MS_ABI impl___2_YAPEAX_K_Z(std::size_t size) {
    if (size == 0) {
        size = 1;
    }
    return std::malloc(size);
}

extern "C" void MS_ABI impl___3_YAXPEAX_Z(void* ptr) {
    std::free(ptr);
}

extern "C" void* MS_ABI impl___U_YAPEAX_K_Z(std::size_t size) {
    return impl___2_YAPEAX_K_Z(size);
}

extern "C" void MS_ABI impl___V_YAXPEAX_Z(void* ptr) {
    impl___3_YAXPEAX_Z(ptr);
}

// Array new/delete use a slightly different mangling and stub naming in our generator.
extern "C" void* MS_ABI impl____U_YAPEAX_K_Z(std::size_t size) {
    return impl___2_YAPEAX_K_Z(size);
}

extern "C" void MS_ABI impl____V_YAXPEAX_Z(void* ptr) {
    impl___3_YAXPEAX_Z(ptr);
}

// =============================================================================
// Resource Library Management
// =============================================================================

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

// =============================================================================
// Validation Functions (Debug)
// =============================================================================

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
