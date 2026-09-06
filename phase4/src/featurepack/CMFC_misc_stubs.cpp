// Auto-generated remaining non-class stub implementations
// Kept intentionally grouped to ensure ABI coverage while class-specific
// source placement is unresolved.
#include <cstddef>
#include <cwchar>
#include <cstring>
#include <windows.h>
#include <objbase.h>
#include <ocidl.h>
#include "openmfc/afxwin.h"
#include "detail/CWinAppSupport.h"
#include "detail/DlgcoreSupport.h"
#include "detail/OlecoreSupport.h"
#include "detail/SynccoreSupport.h"
#include "detail/WincoreSupport.h"
#include "detail/InetcoreSupport.h"
#include "openmfc/afxmfc.h"
// Harvested layout of CControlSiteFactoryMgr (size 0x48 = 72, confirmed
// against the retail allocation size in mfc140u!<site factory mgr factory>
// RVA 0x238540: `mov ecx, 0x48; call operator new`).
#include "detail/CControlSiteFactoryMgrSupport.h"
#include <new>

#ifdef __GNUC__
  #define G2_MS_ABI __attribute__((ms_abi))
#else
  #define G2_MS_ABI
#endif

// Defined lower down in this file. The OLE registrar helpers below need the
// per-user registration flag, and this DLL's functions exist only as impl__
// thunks, so go through the thunk rather than through a C++ declaration.
extern "C" int G2_MS_ABI impl__AfxGetPerUserRegistration__YAHXZ();
extern "C" int G2_MS_ABI impl__AfxOleInprocRegisterHelper__YAHPEAUHKEY____0H_Z(HKEY, HKEY, int);
extern "C" int G2_MS_ABI impl__AfxOleRegisterHelper__YAHPEBQEB_W0HHPEAUHKEY_____Z(
    const wchar_t* const*, const wchar_t* const*, int, int, HKEY);
extern "C" int G2_MS_ABI impl__AfxOleUnregisterHelper__YAHPEBQEB_W0HPEAUHKEY_____Z(
    const wchar_t* const*, const wchar_t* const*, int, HKEY);

namespace {

// ---------------------------------------------------------------------------
// Shared support for the AfxOle*Register* family.  Everything here is
// transcribed from the retail mfc140 (RVAs cited per function); the tables
// below are the DLL's own .rdata tables read back through the same pointer
// arrays it indexes with OLE_APPTYPE.
//
// A registrar table is a NULL-terminated array of pointers to *double*
// strings: each entry is  key '\0' value '\0'  -- e.g. _T("CLSID\\%1\0%5").
// The value is taken from the memory immediately after the key, not from the
// next array slot (mfc140 computes it as lpszKey + lstrlen(lpszKey) + 1,
// RVA 0x26326d).  %1..%9 in either half are replaced by rgszSymbols[0..8].
// ---------------------------------------------------------------------------

// HKEY_CLASSES_ROOT, or HKCU\Software\Classes when the process asked for
// per-user registration via AfxSetPerUserRegistration.  mfc140 does this one
// level down, inside AfxRegSetValue/AfxRegQueryValue/AfxRegDeleteKey, which
// rewrite an HKEY_CLASSES_ROOT root into HKEY_CURRENT_USER plus a
// "Software\Classes\" key prefix (RVA 0x1f307c).
HKEY g2_OleRegRoot(HKEY hkeyRoot, bool* pbCloseIt) {
    *pbCloseIt = false;
    if (hkeyRoot == nullptr)
        hkeyRoot = HKEY_CLASSES_ROOT;
    if (hkeyRoot == HKEY_CLASSES_ROOT && impl__AfxGetPerUserRegistration__YAHXZ()) {
        HKEY hKey = nullptr;
        if (::RegCreateKeyExW(HKEY_CURRENT_USER, L"Software\\Classes", 0, nullptr,
                               REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, nullptr,
                               &hKey, nullptr) == ERROR_SUCCESS) {
            *pbCloseIt = true;
            return hKey;
        }
    }
    return hkeyRoot;
}

// Small fixed string builder. Deliberately not std::wstring: this DLL links
// no libstdc++ string out-of-line code.
struct g2_Str {
    enum { kMax = 1024 };
    wchar_t sz[kMax];
    unsigned int len;
    g2_Str() : len(0) { sz[0] = L'\0'; }
    void Empty() { len = 0; sz[0] = L'\0'; }
    void Add(const wchar_t* p) {
        if (p == nullptr) return;
        while (*p != L'\0' && len + 1 < kMax) sz[len++] = *p++;
        sz[len] = L'\0';
    }
    void AddCh(wchar_t c) { if (len + 1 < kMax) { sz[len++] = c; sz[len] = L'\0'; } }
    bool IsEmpty() const { return len == 0; }
};

// AfxFormatStrings (RVA 0x1cbca0), the substituter these tables are run
// through: '%' followed by '1'..'9' selects rgszSymbols[0..8] and '%' followed
// by 'A'..'Z' continues the range at [9..34]; an index at or past nSymbols
// emits '?'; a NULL symbol contributes nothing; anything else (including a
// second '%') is copied verbatim.
void g2_OleSubst(g2_Str& str, const wchar_t* lpsz, const wchar_t* const* rgszSymbols, int nSymbols) {
    str.Empty();
    if (lpsz == nullptr || rgszSymbols == nullptr) return;
    for (const wchar_t* p = lpsz; *p != L'\0'; ++p) {
        int i = -1;
        if (*p == L'%') {
            if (p[1] >= L'1' && p[1] <= L'9')      i = static_cast<int>(p[1] - L'1');
            else if (p[1] >= L'A' && p[1] <= L'Z') i = static_cast<int>(p[1] - L'8');
        }
        if (i < 0) {
            str.AddCh(*p);
            continue;
        }
        ++p;
        if (i >= nSymbols)
            str.AddCh(L'?');
        else
            str.Add(rgszSymbols[i]);
    }
}

// Create lpszKey under hRoot and set its default value -- the ::RegSetValue
// semantics AfxRegSetValue is built on (RVA 0x1f30ed passes the key as
// ::RegSetValue's lpSubKey, so the key is created and its *default* value is
// what gets written).
LONG g2_OleSetKey(HKEY hRoot, const wchar_t* lpszKey, const wchar_t* lpszValue) {
    HKEY hKey = nullptr;
    LONG lRes = ::RegCreateKeyExW(hRoot, lpszKey, 0, nullptr, REG_OPTION_NON_VOLATILE,
                                   KEY_WRITE, nullptr, &hKey, nullptr);
    if (lRes != ERROR_SUCCESS)
        return lRes;
    if (lpszValue != nullptr) {
        lRes = ::RegSetValueExW(hKey, nullptr, 0, REG_SZ,
                                 reinterpret_cast<const BYTE*>(lpszValue),
                                 static_cast<DWORD>((std::wcslen(lpszValue) + 1) * sizeof(wchar_t)));
    }
    ::RegCloseKey(hKey);
    return lRes;
}

// Delete lpszKey, then every parent it leaves empty.  Transcribed from the
// unregister worker at RVA 0x220c04: open the key, enumerate one subkey, and
// stop if there is one -- a key that still has children survives and that is
// not an error -- otherwise delete it and continue with the parent.
void g2_OleDeleteKey(HKEY hRoot, wchar_t* pszKey) {
    while (pszKey[0] != L'\0') {
        HKEY hKey = nullptr;
        if (::RegOpenKeyExW(hRoot, pszKey, 0, KEY_ENUMERATE_SUB_KEYS, &hKey) != ERROR_SUCCESS)
            break;
        wchar_t szSub[MAX_PATH];
        DWORD cchSub = MAX_PATH;
        LONG lEnum = ::RegEnumKeyExW(hKey, 0, szSub, &cchSub, nullptr, nullptr, nullptr, nullptr);
        ::RegCloseKey(hKey);
        if (lEnum == ERROR_SUCCESS)
            break;  // still has subkeys
        if (::RegDeleteKeyW(hRoot, pszKey) != ERROR_SUCCESS)
            break;
        wchar_t* pSlash = nullptr;
        for (wchar_t* p = pszKey; *p != L'\0'; ++p)
            if (*p == L'\\') pSlash = p;
        if (pSlash == nullptr)
            break;
        *pSlash = L'\0';
    }
}

// mfc140 picks between two overwrite tables per OLE_APPTYPE by testing
// AfxGetModuleState()->m_bDLL (RVA 0x262db2): an EXE server is registered with
// LocalServer32 + InprocHandler32, a DLL server with InProcServer32.
// OpenMFC's AFX_MODULE_STATE (detail/RegcoreSupport.h) carries no m_bDLL, so
// ask the module that is being registered: its PE characteristics say whether
// the image is a DLL, which is what m_bDLL records.
bool g2_ModuleIsDll(HINSTANCE hInst) {
    HMODULE hMod = hInst != nullptr ? reinterpret_cast<HMODULE>(hInst) : ::GetModuleHandleW(nullptr);
    if (hMod == nullptr) return false;
    const IMAGE_DOS_HEADER* pDos = reinterpret_cast<const IMAGE_DOS_HEADER*>(hMod);
    if (pDos->e_magic != IMAGE_DOS_SIGNATURE) return false;
    const IMAGE_NT_HEADERS* pNt = reinterpret_cast<const IMAGE_NT_HEADERS*>(
        reinterpret_cast<const BYTE*>(hMod) + pDos->e_lfanew);
    if (pNt->Signature != IMAGE_NT_SIGNATURE) return false;
    return (pNt->FileHeader.Characteristics & IMAGE_FILE_DLL) != 0;
}

// --- default registrar tables, per OLE_APPTYPE -----------------------------
// Read out of retail mfc140's .rdata through the pointer arrays it indexes at
// RVA 0x33bb60 (register + EXE overwrite) and 0x33bbf0 (DLL overwrite),
// entry order preserved.
// %1 CLSID text  %2 ProgID (class name)  %3 quoted module path
// %4 short type name  %5 long type name  %6 app name  %7 icon index
// %8 local filter name  %9 local filter extension

const wchar_t* const g2_rgRegInplaceServer[] = {  // 0x33be60
    L"%2\0%5",
    L"%2\\Insertable\0",
    L"%2\\protocol\\StdFileEditing\\verb\\0\0&Edit",
    L"CLSID\\%1\0%5",
    L"CLSID\\%1\\Verb\\0\0&Edit,0,2",
    L"CLSID\\%1\\Verb\\1\0&Open,0,2",
    L"CLSID\\%1\\Insertable\0",
    L"CLSID\\%1\\AuxUserType\\2\0%4",
    L"CLSID\\%1\\AuxUserType\\3\0%6",
    L"CLSID\\%1\\MiscStatus\0" L"32",
    nullptr
};
const wchar_t* const g2_rgRegServer[] = {  // 0x33be10 -- no second verb
    L"%2\0%5",
    L"%2\\Insertable\0",
    L"%2\\protocol\\StdFileEditing\\verb\\0\0&Edit",
    L"CLSID\\%1\0%5",
    L"CLSID\\%1\\Verb\\0\0&Edit,0,2",
    L"CLSID\\%1\\Insertable\0",
    L"CLSID\\%1\\AuxUserType\\2\0%4",
    L"CLSID\\%1\\AuxUserType\\3\0%6",
    L"CLSID\\%1\\MiscStatus\0" L"32",
    nullptr
};
const wchar_t* const g2_rgRegContainer[] = {  // 0x33bd80
    L"%2\0%5",
    L"CLSID\\%1\0%5",
    nullptr
};
const wchar_t* const g2_rgRegDispatch[] = {  // 0x33bc40
    L"%2\0%5",
    L"CLSID\\%1\0%5",
    nullptr
};
const wchar_t* const g2_rgRegDocObject[] = {  // 0x33bd10
    L"%2\0%5",
    L"%2\\Insertable\0",
    L"%2\\protocol\\StdFileEditing\\verb\\0\0&Edit",
    L"CLSID\\%1\0%5",
    L"CLSID\\%1\\Verb\\0\0&Edit,0,2",
    L"CLSID\\%1\\Verb\\1\0&Open,0,2",
    L"CLSID\\%1\\Insertable\0",
    L"CLSID\\%1\\AuxUserType\\2\0%4",
    L"CLSID\\%1\\AuxUserType\\3\0%6",
    L"CLSID\\%1\\MiscStatus\0" L"32",
    L"CLSID\\%1\\DocObject\0" L"0",
    L"%2\\DocObject\0" L"0",
    L"CLSID\\%1\\Printable\0",
    nullptr
};

const wchar_t* const g2_rgOvrServerExe[] = {  // 0x33bcd0 -- OAT_(INPLACE_)SERVER, EXE
    L"%2\\CLSID\0%1",
    L"%2\\protocol\\StdFileEditing\\server\0%3",
    L"CLSID\\%1\\ProgID\0%2",
    L"CLSID\\%1\\InprocHandler32\0ole32.dll",
    L"CLSID\\%1\\LocalServer32\0%3",
    L"CLSID\\%1\\DefaultIcon\0%3,%7",
    nullptr
};
const wchar_t* const g2_rgOvrServerDll[] = {  // 0x33bd98 -- OAT_(INPLACE_)SERVER, DLL
    L"%2\\CLSID\0%1",
    L"%2\\protocol\\StdFileEditing\\server\0%3",
    L"CLSID\\%1\\ProgID\0%2",
    L"CLSID\\%1\\InProcServer32\0%3",
    L"CLSID\\%1\\DefaultIcon\0%3,%7",
    nullptr
};
const wchar_t* const g2_rgOvrContainer[] = {  // 0x33bc58 -- same for EXE and DLL
    L"%2\\CLSID\0%1",
    L"CLSID\\%1\\ProgID\0%2",
    L"CLSID\\%1\\InprocHandler32\0ole32.dll",
    L"CLSID\\%1\\LocalServer32\0%3",
    L"CLSID\\%1\\DefaultIcon\0%3,%7",
    nullptr
};
const wchar_t* const g2_rgOvrDispatchExe[] = {  // 0x33bca8
    L"%2\\CLSID\0%1",
    L"CLSID\\%1\\ProgID\0%2",
    L"CLSID\\%1\\InprocHandler32\0ole32.dll",
    L"CLSID\\%1\\LocalServer32\0%3",
    nullptr
};
const wchar_t* const g2_rgOvrDispatchDll[] = {  // 0x33bc88
    L"%2\\CLSID\0%1",
    L"CLSID\\%1\\ProgID\0%2",
    L"CLSID\\%1\\InProcServer32\0%3",
    nullptr
};
const wchar_t* const g2_rgOvrDocObject[] = {  // 0x33bbb0 -- same for EXE and DLL
    L"%2\\CLSID\0%1",
    L"%2\\protocol\\StdFileEditing\\server\0%3",
    L"CLSID\\%1\\ProgID\0%2",
    L"CLSID\\%1\\InprocHandler32\0ole32.dll",
    L"CLSID\\%1\\LocalServer32\0%3",
    L"CLSID\\%1\\DefaultIcon\0%3,%7",
    L"CLSID\\%1\\DefaultExtension\0%9, %8",
    nullptr
};

// OAT_UNKNOWN (-1) and OAT_DOC_OBJECT_CONTAINER (5) are outside the retail
// arrays; mfc140's register overload indexes past their end, we take no
// default table instead (its unregister overload rejects them outright).
void g2_OleServerTables(int nAppType, bool bDll, const wchar_t* const** ppReg,
                        const wchar_t* const** ppOvr) {
    *ppReg = nullptr;
    *ppOvr = nullptr;
    switch (nAppType) {
    case 0:  // OAT_INPLACE_SERVER
        *ppReg = g2_rgRegInplaceServer;
        *ppOvr = bDll ? g2_rgOvrServerDll : g2_rgOvrServerExe;
        break;
    case 1:  // OAT_SERVER
        *ppReg = g2_rgRegServer;
        *ppOvr = bDll ? g2_rgOvrServerDll : g2_rgOvrServerExe;
        break;
    case 2:  // OAT_CONTAINER
        *ppReg = g2_rgRegContainer;
        *ppOvr = g2_rgOvrContainer;
        break;
    case 3:  // OAT_DISPATCH_OBJECT
        *ppReg = g2_rgRegDispatch;
        *ppOvr = bDll ? g2_rgOvrDispatchDll : g2_rgOvrDispatchExe;
        break;
    case 4:  // OAT_DOC_OBJECT_SERVER
        *ppReg = g2_rgRegDocObject;
        *ppOvr = g2_rgOvrDocObject;
        break;
    default:
        break;
    }
}

// %7 is the icon index formatted with "%d", but mfc140 validates it first: it
// calls ExtractIcon(hModule, <unquoted module path>, nIconIndex) and falls
// back to 0 when nothing comes back (RVA 0x2629f4).  shell32 is not in this
// DLL's import list, so probe for it the way the other optional-DLL uses in
// this file do.
bool g2_OleHasIcon(HINSTANCE hInst, const wchar_t* pszPath, int nIconIndex) {
    typedef HICON (WINAPI *ExtractIconFn)(HINSTANCE, LPCWSTR, UINT);
    HMODULE hShell = ::GetModuleHandleW(L"shell32.dll");
    if (hShell == nullptr) hShell = ::LoadLibraryW(L"shell32.dll");
    if (hShell == nullptr) return true;  // cannot check: keep the caller's index
    ExtractIconFn pfnExtractIcon = reinterpret_cast<ExtractIconFn>(
        reinterpret_cast<void*>(::GetProcAddress(hShell, "ExtractIconW")));
    if (pfnExtractIcon == nullptr) return true;
    HICON hIcon = pfnExtractIcon(hInst, pszPath, static_cast<UINT>(nIconIndex));
    if (hIcon == nullptr) return false;
    ::DestroyIcon(hIcon);
    return true;
}

// %9: the caller's extension, or -- when it did not supply one -- the
// extension embedded in the local filter name, so that
// "Foo Document (*.foo)" yields ".foo" (mfc140 does this with three
// Find/Mid/Left steps, RVA 0x262aa6).
void g2_OleFilterExt(g2_Str& str, const wchar_t* pszExt, const wchar_t* pszFilterName) {
    str.Empty();
    if (pszExt != nullptr && pszExt[0] != L'\0') {
        str.Add(pszExt);
        return;
    }
    if (pszFilterName == nullptr || pszFilterName[0] == L'\0') return;
    const wchar_t* pOpen = std::wcschr(pszFilterName, L'(');
    if (pOpen == nullptr) return;
    const wchar_t* pDot = std::wcschr(pOpen + 1, L'.');
    if (pDot == nullptr) return;
    const wchar_t* pClose = std::wcschr(pDot, L')');
    if (pClose == nullptr) return;
    for (const wchar_t* p = pDot; p < pClose; ++p)
        str.AddCh(*p);
}

// Builds the %1..%9 symbol table shared by AfxOleRegisterServerClass and
// AfxOleUnregisterServerClass (the retail builder at RVA 0x262880) and runs
// the register table then the overwrite table through the matching helper.
// mfc140 skips the second table when the first one failed and returns the
// last helper's result (RVA 0x262e5e), which is what the caller sees.
int g2_OleServerClass(const GUID* pclsid, const wchar_t* lpszClassName,
                      const wchar_t* lpszShortTypeName, const wchar_t* lpszLongTypeName,
                      int nAppType, const wchar_t** rglpszRegister,
                      const wchar_t** rglpszOverwrite, int nIconIndex,
                      const wchar_t* lpszLocalFilterName,
                      const wchar_t* lpszLocalFilterExt, bool bRegister) {
    // AfxOleUnregisterServerClass rejects an out-of-range OLE_APPTYPE before
    // it does anything else (RVA 0x262ec5).
    if (!bRegister && (nAppType < 0 || nAppType > 4))
        return FALSE;
    if (pclsid == nullptr)
        return FALSE;

    wchar_t szClsid[64] = {};
    if (::StringFromGUID2(*pclsid, szClsid, 64) == 0)
        return FALSE;

    HINSTANCE hInst = AfxGetInstanceHandle();
    wchar_t szModule[MAX_PATH] = {};
    ::GetModuleFileNameW(hInst, szModule, MAX_PATH);
    g2_Str strPath;  // %3 is the module path *in quotes*
    strPath.AddCh(L'"');
    strPath.Add(szModule);
    strPath.AddCh(L'"');

    // %6: AFX_MODULE_STATE::m_lpszCurrentAppName in mfc140; OpenMFC keeps the
    // same string on the application object.
    CWinApp* pApp = AfxGetApp();
    const wchar_t* pszAppName = (pApp != nullptr && pApp->m_pszAppName != nullptr)
                                    ? pApp->m_pszAppName : L"";

    wchar_t szIcon[16] = {};
    int nIcon = nIconIndex;
    if (nIcon != 0 && !g2_OleHasIcon(hInst, szModule, nIcon))
        nIcon = 0;
    std::swprintf(szIcon, 16, L"%d", nIcon);

    g2_Str strExt;
    g2_OleFilterExt(strExt, lpszLocalFilterExt, lpszLocalFilterName);

    // Ten slots, nine filled -- mfc140 allocates 0x50 bytes and passes 10.
    const wchar_t* rgszSymbols[10];
    rgszSymbols[0] = szClsid;
    rgszSymbols[1] = lpszClassName;
    rgszSymbols[2] = strPath.sz;
    rgszSymbols[3] = lpszShortTypeName;
    rgszSymbols[4] = lpszLongTypeName;
    rgszSymbols[5] = pszAppName;
    rgszSymbols[6] = szIcon;
    rgszSymbols[7] = lpszLocalFilterName;
    rgszSymbols[8] = strExt.sz;
    rgszSymbols[9] = nullptr;

    const wchar_t* const* pReg = nullptr;
    const wchar_t* const* pOvr = nullptr;
    g2_OleServerTables(nAppType, g2_ModuleIsDll(hInst), &pReg, &pOvr);
    if (rglpszRegister != nullptr)  pReg = rglpszRegister;
    if (rglpszOverwrite != nullptr) pOvr = rglpszOverwrite;

    int nResult = TRUE;
    if (bRegister) {
        // The "register" table only fills in keys that are not there yet;
        // the "overwrite" table always wins.
        if (pReg != nullptr)
            nResult = impl__AfxOleRegisterHelper__YAHPEBQEB_W0HHPEAUHKEY_____Z(
                pReg, rgszSymbols, 10, FALSE, HKEY_CLASSES_ROOT);
        if (nResult && pOvr != nullptr)
            nResult = impl__AfxOleRegisterHelper__YAHPEBQEB_W0HHPEAUHKEY_____Z(
                pOvr, rgszSymbols, 10, TRUE, HKEY_CLASSES_ROOT);
    } else {
        if (pReg != nullptr)
            nResult = impl__AfxOleUnregisterHelper__YAHPEBQEB_W0HPEAUHKEY_____Z(
                pReg, rgszSymbols, 10, HKEY_CLASSES_ROOT);
        if (nResult && pOvr != nullptr)
            nResult = impl__AfxOleUnregisterHelper__YAHPEBQEB_W0HPEAUHKEY_____Z(
                pOvr, rgszSymbols, 10, HKEY_CLASSES_ROOT);
    }
    return nResult;
}

// The two tables AfxOleRegisterControlClass runs, relative to the ProgID key
// and to the CLSID key it has just created (retail tables at RVA 0x345a90 and
// 0x345a40; both are applied with bReplace=TRUE).  The leading entry of each
// has an empty key, i.e. it writes the root key's own default value.
const wchar_t* const g2_rgCtlProgID[] = {
    L"\0%1",            // <ProgID> = type name
    L"CLSID\0%2",
    nullptr
};
const wchar_t* const g2_rgCtlClsid[] = {
    L"\0%1",            // CLSID\{clsid} = type name
    L"ProgID\0%2",
    L"InprocServer32\0%3",
    L"ToolboxBitmap32\0%3, %4",
    L"MiscStatus\0" L"0",
    L"MiscStatus\\1\0%5",
    L"Control\0",
    L"TypeLib\0%6",
    L"Version\0%7",
    nullptr
};

}  // namespace

// The file's own MS_ABI macro is defined below this region, so the teardown
// hooks' thunk declarations use their own copy of the attribute.
#ifdef __GNUC__
  #define G3_MS_ABI __attribute__((ms_abi))
#else
  #define G3_MS_ABI
#endif

// Thunks used by the module-teardown hooks (AfxWinTerm, AfxReleaseD2DRefs,
// ControlBarCleanUp). This DLL's C++ methods exist only as extern "C" impl__
// thunks, so the teardown paths must go through the thunk, never through the
// class. Definitions:
//   phase4/src/featurepack/docking/Globals.cpp   AfxGetD2DState
//   phase4/src/core/d2d/_AFX_D2D_STATE.cpp       _AFX_D2D_STATE::ReleaseD2DRefs
//   phase4/src/featurepack/toolbar/CMFCToolBarImages.cpp CMFCToolBarImages::CleanUp
//   phase4/src/featurepack/visualmanager/CMFCVisualManager.cpp DestroyInstance
// AfxTermThread / AfxUnregisterWndClasses are NOT declared here on purpose:
// both are defined in this same file above AfxWinTerm (the file is ordered by
// symbol name), so a prologue declaration would only add a second, separately
// maintained copy of their parameter lists to keep in sync.
extern "C" void* G3_MS_ABI impl__AfxGetD2DState__YAPEAV_AFX_D2D_STATE__XZ();
extern "C" void G3_MS_ABI impl__ReleaseD2DRefs__AFX_D2D_STATE__QEAAXXZ(void* pThis);
extern "C" void G3_MS_ABI impl__CleanUp_CMFCToolBarImages__SAXXZ();
extern "C" void G3_MS_ABI impl__DestroyInstance_CMFCVisualManager__SAXH_Z(int bAutoDestroyOnly);


#ifndef MS_ABI
  #ifdef __GNUC__
    #define MS_ABI __attribute__((ms_abi))
  #else
    #define MS_ABI
  #endif
#endif

// Thunks used by this group's implementations (this DLL defines its C++
// methods only as extern "C" impl__ thunks -- see the file header comment).
extern "C" void  MS_ABI impl__AfxDoRegisterMFCControlClass__YAXPEB_W0_Z(const wchar_t*, const wchar_t*);
extern "C" void* MS_ABI impl___0CControlSiteFactoryMgr__QEAA_XZ(void* pThis);
extern "C" BOOL  MS_ABI impl__RegisterSiteFactory_CControlSiteFactoryMgr__QEAAHPEAVIControlSiteFactory___Z(void* pThis, void* pFactory);
extern "C" BOOL  MS_ABI impl__UnregisterSiteFactory_CControlSiteFactoryMgr__QEAAHPEAVIControlSiteFactory___Z(void* pThis, void* pFactory);
extern "C" void* MS_ABI impl___0CCommandManager__IEAA_XZ(void* pThis);

// The OCC manager handed to AfxEnableControlContainer.  Deliberately given
// external C linkage (not file-static) so a future consumer can pick it up
// with a one-line declaration; retail keeps it in AFX_MODULE_STATE+0xA0,
// which OpenMFC's 3-pointer AFX_MODULE_STATE (detail/RegcoreSupport.h) has
// no room for.  Nothing in OpenMFC reads it yet.
extern "C" void* g4_g_pOccManager = nullptr;

namespace {

// --- control-site factory registry ---------------------------------------
// Retail AfxRegisterSiteFactory/AfxUnregisterSiteFactory (RVA 0x238450 /
// 0x238490) fetch a process-local CControlSiteFactoryMgr and tail-call
// CControlSiteFactoryMgr::RegisterSiteFactory / ::UnregisterSiteFactory.
// OpenMFC has that class (core/ole/CControlSiteFactoryMgr.cpp) but no
// process-local instance, so this group owns the singleton.
CRITICAL_SECTION* g4_GetSiteFactoryLock() {
    static CRITICAL_SECTION s_cs;
    static bool s_inited = [] { ::InitializeCriticalSection(&s_cs); return true; }();
    (void)s_inited;
    return &s_cs;
}

S_Ccontrolsitefactorymgr* g4_GetSiteFactoryMgr() {
    static S_Ccontrolsitefactorymgr s_mgr;   // zero-initialized static storage
    static bool s_inited = [] {
        impl___0CControlSiteFactoryMgr__QEAA_XZ(&s_mgr);
        return true;
    }();
    (void)s_inited;
    return &s_mgr;
}

// CControlSiteFactoryMgr::RegisterSiteFactory does an unconditional AddTail;
// retail checks for an existing entry first (the CList::Find loop at RVA
// 0x180238352), so the duplicate check lives here.
bool g4_IsSiteFactoryRegistered(const S_Ccontrolsitefactorymgr* p, void* pFactory) {
    for (const CNode* node = p->m_lstFactory.m_pNodeHead; node != nullptr; node = node->pNext) {
        if (node->data == pFactory) return true;
    }
    return false;
}

// --- AFX_SIZEPARENTPARAMS -------------------------------------------------
// Layout taken verbatim from MFC's own afxpriv.h (VS2022 atlmfc):
//     struct AFX_SIZEPARENTPARAMS
//     { HDWP hDWP; RECT rect; SIZE sizeTotal; BOOL bStretch; };
// Only hDWP (offset 0) is touched by AfxRepositionWindow, so the rest is
// carried purely for documentation.
struct g4_AFX_SIZEPARENTPARAMS {
    HDWP hDWP;
    RECT rect;
    SIZE sizeTotal;
    BOOL bStretch;
};

// --- DHtmlEventMapEntry ---------------------------------------------------
// Layout from MFC's afxdhtml.h:
//     struct DHtmlEventMapEntry
//     { DHtmlEventMapEntryType nType; DISPID dispId; LPCTSTR szName;
//       void (__stdcall CDHtmlSinkHandler::*pfnEventFunc)(); };
// CDHtmlSinkHandler deliberately has two base classes, so the member pointer
// is MSVC's multiple-inheritance form {void* pfn; int adj;} padded to 16 --
// 32 bytes per entry, matching the 0x20 stride of retail's base map
// (RVA 0x212020).  DHTMLEVENTMAPENTRY_END == 5 (NAME, CLASS, TAG, ELEMENT,
// CONTROL, END).
struct g4_DHtmlEventMapEntry {
    int            nType;
    long           dispId;
    const wchar_t* szName;
    void*          pfnEventFunc;
    int            nAdjustor;
    int            nPadding;
};
const g4_DHtmlEventMapEntry g4_dhtmlEventMapEmpty[] = {
    { 5 /*DHTMLEVENTMAPENTRY_END*/, 0, nullptr, nullptr, 0, 0 },
};

// --- CCommandManager singleton -------------------------------------------
// sizeof(CCommandManager) == 0x120: retail's vector deleting destructor
// (RVA 0x2ee70) passes 0x120 to operator delete, and retail GetCmdMgr
// (RVA 0x2ed70) is a bare `lea rax, <module-scope instance>; ret`, i.e. the
// manager is a static singleton rather than a lazily new'd object.
alignas(16) unsigned char g4_cmdMgrStorage[0x120];

// Bring that storage into the state retail's constructor (RVA 0x2ed80)
// leaves it in.  Zeroed storage is NOT safe: CCommandManager's public
// accessors are INLINE in afxcommandmanager.h, so a client compiles them
// into its own code and runs them against this object --
// GetMenuUserImage() calls CMap::Lookup(), whose GetAssocAt() computes
// `nHashValue % m_nHashTableSize` BEFORE testing m_pHashTable for NULL
// (afxtempl.h:1538), i.e. a zeroed m_nHashTableSize is an integer
// divide-by-zero in the caller.  The offsets below are exactly the stores
// retail's constructor makes, and they agree independently with the member
// layout in afxcommandmanager.h:
//   0x000 CCommandManager vfptr
//   0x008 m_CommandIndex            CMap  (hash size 0x018, block size 0x038)
//   0x040 m_CommandIndexUser        CMap  (hash size 0x050, block size 0x070)
//   0x078 m_DefaultImages           CMap  (hash size 0x088, block size 0x0a8)
//   0x0b0 m_lstCommandsWithoutImages CList (            block size 0x0e0)
//   0x0e8 m_mapMenuUserImages       CMap  (hash size 0x0f8, block size 0x118)
// Everything else retail writes is zero, which static storage already is.
// The six vfptrs stay NULL -- this DLL has no MSVC-layout vtable for CMap,
// CList or CCommandManager -- which is harmless because none of the inline
// accessors dispatch virtually and retail never destroys the singleton.
void g4_InitCmdMgrStorage(unsigned char* p) {
    static const unsigned kHashSizeOffsets[]  = { 0x18, 0x50, 0x88, 0xf8 };
    static const unsigned kBlockSizeOffsets[] = { 0x38, 0x70, 0xa8, 0xe0, 0x118 };
    for (unsigned off : kHashSizeOffsets) {
        *reinterpret_cast<unsigned int*>(p + off) = 17;   // CMap m_nHashTableSize
    }
    for (unsigned off : kBlockSizeOffsets) {
        *reinterpret_cast<long long*>(p + off) = 10;      // m_nBlockSize
    }
}

}  // namespace

// g5: MS_ABI is only #defined below this region, so the ODBC field-exchange
// helpers declare their own copy of it.
#ifdef __GNUC__
  #define G5_MS_ABI __attribute__((ms_abi))
#else
  #define G5_MS_ABI
#endif
// Field status bits (afxdb.h in real MFC): 0x01 dirty, 0x02 null.
namespace { const unsigned char g5_kFieldFlagNull = 0x02; }
// Defined in phase4/src/core/db/CRecordset.cpp; returns the recordset's current
// per-field status byte for the 0-based field index.
extern "C" unsigned char G5_MS_ABI impl__GetFieldStatus_CRecordset__QEAAEK_Z(void* pRecordset, unsigned long dwField);


// The file-wide MS_ABI macro is defined below this region, so declare a
// private one for the thunk prototypes needed here.
#ifdef __GNUC__
  #define G6_MS_ABI __attribute__((ms_abi))
#else
  #define G6_MS_ABI
#endif

// AfxGetApp has no linkable C++ definition inside this DLL; the only
// definition is the exported thunk in phase4/src/detail/RegcoreSupport.cpp.
extern "C" CWinApp* G6_MS_ABI impl__AfxGetApp__YAPEAVCWinApp__XZ();

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

extern "C" LRESULT MS_ABI impl__WindowProc_CWnd__MEAA_JI_K_J_Z(CWnd* pThis, UINT message, WPARAM wParam, LPARAM lParam);
extern "C" intptr_t MS_ABI impl__ProcessWndProcException_CWinThread__UEAA_JPEAVCException__PEBUtagMSG___Z(
    CWinThread* pThis, CException* exception, const MSG* msg);
extern "C" intptr_t MS_ABI impl__ProcessWndProcException_CWinApp__UEAA_JPEAVCException__PEBUtagMSG___Z(
    CWinApp* pThis, CException* exception, const MSG* msg);

// Internal method thunks. This DLL defines its C++ methods ONLY as extern "C"
// impl__ thunks -- the class declarations in the public headers exist so client
// code compiles, but there is no C++ method symbol to link against from inside
// the DLL. Internal call sites must therefore go through the thunk, not through
// the method. (CWnd::DeleteTempMap and CWnd::Attach are the exception: they have
// real C++ definitions in core/window.)
extern "C" int MS_ABI impl__Attach_CGdiObject__QEAAHPEAX_Z(CGdiObject* pThis, HGDIOBJ hObject);
extern "C" void MS_ABI impl__DeleteTempMap_CDC__SAXXZ();
extern "C" void MS_ABI impl__DeleteTempMap_CGdiObject__SAXXZ();
extern "C" int MS_ABI impl__ReadBool_CTagManager__QEAAHAEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__AEAH_Z(
    void* self, const CString* tag, int* value);
extern "C" unsigned long MS_ABI impl__ApplicationRecoveryCallback_CWinApp__UEAAKPEAX_Z(CWinApp*, void*);

namespace {
int g_bPerUserRegistration = 0;
void* g_pfnNewHandler = nullptr;

// Process-wide refcounts for AfxTlsAddRef/Release and
// AfxGlobalsAddRef/Release (see their implementations below for what each
// pair actually gates in this codebase).
LONG g_afxTlsRefCount = 0;
LONG g_afxGlobalsRefCount = 0;

// Backing store for AfxLockGlobals/AfxUnlockGlobals: an array of
// CRITICAL_SECTIONs indexed by nLockType (MFC's CRIT_* constants; the
// highest real one is around 12, so 16 slots leaves headroom). Lazily
// initialized via a function-local static, which C++11 guarantees is
// thread-safely constructed exactly once ("magic statics"), so no separate
// call_once/init-guard object is needed.
constexpr int kAfxLockGlobalsCount = 16;
CRITICAL_SECTION* GetGlobalLock(int nLockType) {
    static CRITICAL_SECTION s_cs[kAfxLockGlobalsCount];
    static bool s_inited = [] {
        for (auto& cs : s_cs) ::InitializeCriticalSection(&cs);
        return true;
    }();
    (void)s_inited;
    if (nLockType < 0 || nLockType >= kAfxLockGlobalsCount) return nullptr;
    return &s_cs[nLockType];
}
}
// Symbol: ?AFXGetParentFrame@@YAPEAVCFrameWnd@@PEBVCWnd@@@Z
extern "C" CFrameWnd* MS_ABI impl__AFXGetParentFrame__YAPEAVCFrameWnd__PEBVCWnd___Z(const CWnd* pWnd) {
    return pWnd ? pWnd->GetParentFrame() : nullptr;
}
// Symbol: ?AFXGetRegPath@@YA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@PEB_W0@Z
// CString AFXGetRegPath(LPCTSTR lpszPostFix, LPCTSTR lpszProfileName)
// Signature rewritten: the generator expanded the by-value CStringT return
// into a (void*, void**, void*) triplet, so both real arguments were being
// read from R9/stack instead of RDX/R8. On x64 MSVC a by-value class return
// is a hidden first parameter in RCX holding caller-allocated storage, and the
// callee returns that same pointer in RAX -- which is what this now does.
// Behaviour: with an explicit non-empty lpszProfileName the profile name IS
// the whole path; otherwise the path is built from the running CWinApp as
// "SOFTWARE\\<m_pszRegistryKey>\\<m_pszProfileName>\\<lpszPostFix>\\".
// Real MFC raises an invalid-argument exception when lpszPostFix is NULL or
// when the app has no registry key / profile name; this returns the best-effort
// path instead, matching how the rest of this DLL handles precondition misuse.
extern "C" CString* MS_ABI impl__AFXGetRegPath__YA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__PEB_W0_Z(
    CString* pRet, const wchar_t* lpszPostFix, const wchar_t* lpszProfileName) {
    if (!pRet) {
        return pRet;
    }
    new (pRet) CString();

    if (lpszProfileName != nullptr && *lpszProfileName != L'\0') {
        *pRet = lpszProfileName;
        return pRet;
    }

    CWinApp* pApp = impl__AfxGetApp__YAPEAVCWinApp__XZ();
    const wchar_t* pszRegistryKey = pApp ? pApp->m_pszRegistryKey : nullptr;
    const wchar_t* pszProfileName = pApp ? pApp->m_pszProfileName : nullptr;

    *pRet = L"SOFTWARE\\";
    if (pszRegistryKey != nullptr && *pszRegistryKey != L'\0') {
        *pRet += pszRegistryKey;
        *pRet += L"\\";
    }
    if (pszProfileName != nullptr) {
        *pRet += pszProfileName;
    }
    *pRet += L"\\";
    if (lpszPostFix != nullptr) {
        *pRet += lpszPostFix;
    }
    *pRet += L"\\";
    return pRet;
}
// Symbol: ?AFXPlaySystemSound@@YAXH@Z
extern "C" void MS_ABI impl__AFXPlaySystemSound__YAXH_Z(int p0) {
    ::MessageBeep(static_cast<UINT>(p0));
}
// Symbol: ?AFXPrintPreview@@YAXPEAVCView@@@Z
// Left stubbed: real MFC's AFXPrintPreview(CView*) runs the print-preview
// nested message pump. The closest existing piece,
// CView::DoPrintPreview(UINT, CView*, CRuntimeClass*, CPrintPreviewState*)
// in phase4/src/core/view/CView.cpp, needs a pre-built preview view /
// runtime class / preview-state that this single-CView* entry point doesn't
// receive, and it doesn't run a message loop -- it's not a drop-in forward
// target, so nothing safe to call.
extern "C" void MS_ABI impl__AFXPrintPreview__YAXPEAVCView___Z(void* /*class*/* p0) {
    (void)p0;
}
// Symbol: ?AFXSoundThreadProc@@YAXPEAX@Z
extern "C" void MS_ABI impl__AFXSoundThreadProc__YAXPEAX_Z(void* p0) {
    // Async worker that plays a system sound; matches the sibling
    // AFXPlaySystemSound(int) implementation above (MessageBeep with the
    // sound id), just invoked off a worker thread with the id passed as the
    // raw pointer value rather than an int argument.
    ::MessageBeep(static_cast<UINT>(reinterpret_cast<uintptr_t>(p0)));
}
// Symbol: ?AfxApplicationRecoveryWrapper@@YAKPEAX@Z
// Real MFC forwards this OS Application-Recovery trampoline straight to
// CWinApp::ApplicationRecoveryCallback (see the forward declaration of
// impl__ApplicationRecoveryCallback_CWinApp__UEAAKPEAX_Z, added near the
// top of this file, whose real definition lives in core/app/CWinApp.cpp).
extern "C" unsigned long MS_ABI impl__AfxApplicationRecoveryWrapper__YAKPEAX_Z(void* p0) {
    CWinApp* pApp = AfxGetApp();
    if (!pApp) return 0;
    return impl__ApplicationRecoveryCallback_CWinApp__UEAAKPEAX_Z(pApp, p0);
}
// Symbol: ?AfxAutosaveTimerProc@@YAXPEAUHWND__@@I_KK@Z
extern "C" void MS_ABI impl__AfxAutosaveTimerProc__YAXPEAUHWND____I_KK_Z(void* /*struct*/* p0, unsigned int p1, unsigned __int64 p2, unsigned long p3) {
    // Real MFC's WM_TIMER callback drives CWinApp's m_pDataRecoveryHandler
    // (CDataRecoveryHandler::AutosaveAllDocumentInfo) on an interval. OpenMFC
    // has no CWinApp field exposing a CDataRecoveryHandler instance to call
    // through (CDataRecoveryHandler::AutosaveAllDocumentInfo is itself a
    // no-op stub), so there is no live subsystem for this proc to drive.
    (void)p0; (void)p1; (void)p2; (void)p3;
}
// Symbol: ?AfxBeginThread@@YAPEAVCWinThread@@P6AIPEAX@Z0HIKPEAU_SECURITY_ATTRIBUTES@@@Z
extern "C" void* MS_ABI impl__AfxBeginThread__YAPEAVCWinThread__P6AIPEAX_Z0HIKPEAU_SECURITY_ATTRIBUTES___Z(void* /*fnptr*/ p0, void* /*fnptr*/ p1, int p2, unsigned int p3, unsigned long p4, void* /*struct*/* p5) {
    return openmfc::detail::synccore::AfxBeginThread(
        reinterpret_cast<AFX_THREADPROC>(p0),
        reinterpret_cast<void*>(p1),
        p2,
        p3,
        p4,
        reinterpret_cast<LPSECURITY_ATTRIBUTES>(p5));
}
// Symbol: ?AfxBeginThread@@YAPEAVCWinThread@@PEAUCRuntimeClass@@HIKPEAU_SECURITY_ATTRIBUTES@@@Z
extern "C" void* MS_ABI impl__AfxBeginThread__YAPEAVCWinThread__PEAUCRuntimeClass__HIKPEAU_SECURITY_ATTRIBUTES___Z(void* /*struct*/* p0, int p1, unsigned int p2, unsigned long p3, void* /*struct*/* p4) {
    return openmfc::detail::synccore::AfxBeginThread(
        reinterpret_cast<CRuntimeClass*>(p0),
        p1,
        p2,
        p3,
        reinterpret_cast<LPSECURITY_ATTRIBUTES>(p4));
}
// Symbol: ?AfxCallWndProc@@YA_JPEAVCWnd@@PEAUHWND__@@I_K_J@Z
extern "C" __int64 MS_ABI impl__AfxCallWndProc__YA_JPEAVCWnd__PEAUHWND____I_K_J_Z(void* /*class*/* p0, void* /*struct*/* p1, unsigned int p2, unsigned __int64 p3, __int64 p4) {
    auto* pWnd = reinterpret_cast<CWnd*>(p0);
    if (pWnd != nullptr) {
        return impl__WindowProc_CWnd__MEAA_JI_K_J_Z(pWnd, p2, static_cast<WPARAM>(p3), static_cast<LPARAM>(p4));
    }
    return static_cast<__int64>(::DefWindowProcW(reinterpret_cast<HWND>(p1), p2, static_cast<WPARAM>(p3), static_cast<LPARAM>(p4)));
}
// Symbol: ?AfxCancelModes@@YAXPEAUHWND__@@@Z
extern "C" void MS_ABI impl__AfxCancelModes__YAXPEAUHWND_____Z(void* /*struct*/* p0) {
    HWND hWndRcvr = reinterpret_cast<HWND>(p0);
    HWND hWndCapture = ::GetCapture();
    if (hWndCapture && hWndCapture != hWndRcvr &&
        !(hWndRcvr && ::IsChild(hWndRcvr, hWndCapture))) {
        ::SendMessageW(hWndCapture, WM_CANCELMODE, 0, 0);
    }
}
// Symbol: ?AfxCheckError@@YAXJ@Z
extern "C" void MS_ABI impl__AfxCheckError__YAXJ_Z(long p0) {
    extern void AFXAPI AfxThrowOleException(LONG sc);
    const HRESULT hr = static_cast<HRESULT>(p0);
    if (FAILED(hr)) {
        if (hr == E_OUTOFMEMORY) {
            AfxThrowMemoryException();
        } else {
            AfxThrowOleException(static_cast<LONG>(hr));
        }
    }
}
// Symbol: ?AfxCompareValueByRef@@YAHPEAX0H@Z
extern "C" int MS_ABI impl__AfxCompareValueByRef__YAHPEAX0H_Z(void* p0, void* p1, int p2) {
    if (p2 <= 0) return 0;
    if (p0 == p1) return 0;
    return std::memcmp(p0, p1, static_cast<size_t>(p2));
}
// Symbol: ?AfxConnectionAdvise@@YAHPEAUIUnknown@@AEBU_GUID@@0HPEAK@Z
extern "C" int MS_ABI impl__AfxConnectionAdvise__YAHPEAUIUnknown__AEBU_GUID__0HPEAK_Z(
    IUnknown* pUnkSrc, const GUID* iid, IUnknown* pUnkSink, int bRefCount, unsigned long* pdwCookie) {
    (void)bRefCount;
    if (!pUnkSrc || !iid || !pUnkSink || !pdwCookie) return FALSE;
    IConnectionPointContainer* pCPC = nullptr;
    if (FAILED(pUnkSrc->QueryInterface(IID_IConnectionPointContainer, (void**)&pCPC))) return FALSE;
    IConnectionPoint* pCP = nullptr;
    HRESULT hr = pCPC->FindConnectionPoint(*iid, &pCP);
    pCPC->Release();
    if (FAILED(hr)) return FALSE;
    hr = pCP->Advise(pUnkSink, pdwCookie);
    pCP->Release();
    return SUCCEEDED(hr);
}
// Symbol: ?AfxConnectionUnadvise@@YAHPEAUIUnknown@@AEBU_GUID@@0HK@Z
extern "C" int MS_ABI impl__AfxConnectionUnadvise__YAHPEAUIUnknown__AEBU_GUID__0HK_Z(
    IUnknown* pUnkSrc, const GUID* iid, IUnknown* pUnkSink, int bRefCount, unsigned long dwCookie) {
    (void)pUnkSink;
    (void)bRefCount;
    if (!pUnkSrc || !iid) return FALSE;
    IConnectionPointContainer* pCPC = nullptr;
    if (FAILED(pUnkSrc->QueryInterface(IID_IConnectionPointContainer, (void**)&pCPC))) return FALSE;
    IConnectionPoint* pCP = nullptr;
    HRESULT hr = pCPC->FindConnectionPoint(*iid, &pCP);
    pCPC->Release();
    if (FAILED(hr)) return FALSE;
    hr = pCP->Unadvise(dwCookie);
    pCP->Release();
    return SUCCEEDED(hr);
}
// Symbol: ?AfxCopyValueByRef@@YAXPEAX0PEA_JH@Z
extern "C" void MS_ABI impl__AfxCopyValueByRef__YAXPEAX0PEA_JH_Z(void* p0, void* p1, __int64* p2, int p3) {
    if (p2) *p2 = 0;
    if (!p0 || !p1 || p3 <= 0) return;
    std::memcpy(p0, p1, static_cast<size_t>(p3));
    if (p2) *p2 = p3;
}
// Symbol: ?AfxCoreInitModule@@YAXXZ
extern "C" void MS_ABI impl__AfxCoreInitModule__YAXXZ() {
    // Stand-alone-DLL hook (afxdll_.h groups it with AfxOleInitModule /
    // AfxNetInitModule / AfxDbInitModule, all of which are empty macros in
    // MFC 14): it chains a CDynLinkLibrary for the shared MFC core DLL onto
    // the calling module's shared-DLL list so that module can reach MFC's
    // resources, runtime classes and object factories.
    // NOT MODELLED HERE: grepped the tree for a shared-DLL list head
    // (m_libraryList / m_pFirstSharedDLL / any owner of
    // CDynLinkLibrary::m_pNextDLL) and for callers of AfxCoreInitModule --
    // there are none. CDynLinkLibrary is only declared, never chained
    // (detail/COleControlModuleSupport.h:78), and AfxInitExtensionModule
    // below likewise leaves pFirstSharedClass/pFirstSharedFactory null.
    // Nothing exists for this hook to register, so it stays empty.
}
// Symbol: ?AfxCreateDC@@YAPEAUHDC__@@PEAX0@Z
extern "C" void* MS_ABI impl__AfxCreateDC__YAPEAUHDC____PEAX0_Z(void* p0, void* p1) {
    HGLOBAL hDevNames = reinterpret_cast<HGLOBAL>(p0);
    HGLOBAL hDevMode = reinterpret_cast<HGLOBAL>(p1);
    if (hDevNames == nullptr) return nullptr;
    DEVNAMES* pDevNames = static_cast<DEVNAMES*>(::GlobalLock(hDevNames));
    if (pDevNames == nullptr) return nullptr;
    const wchar_t* base = reinterpret_cast<const wchar_t*>(pDevNames);
    const wchar_t* lpszDriver = base + pDevNames->wDriverOffset;
    const wchar_t* lpszDevice = base + pDevNames->wDeviceOffset;
    const wchar_t* lpszOutput = base + pDevNames->wOutputOffset;
    DEVMODEW* pDevMode = nullptr;
    if (hDevMode != nullptr) pDevMode = static_cast<DEVMODEW*>(::GlobalLock(hDevMode));
    HDC hdc = ::CreateDCW(lpszDriver, lpszDevice, lpszOutput, pDevMode);
    if (pDevMode != nullptr) ::GlobalUnlock(hDevMode);
    ::GlobalUnlock(hDevNames);
    return hdc;
}
// Symbol: ?AfxCriticalInit@@YAHXZ
extern "C" int MS_ABI impl__AfxCriticalInit__YAHXZ() {
    // Real MFC uses this to initialize the process-wide CRIT_* critical
    // section array consumed by AfxLockGlobals/AfxUnlockGlobals; OpenMFC has
    // no such shared array (see AfxLockGlobals below), so that part is
    // honestly skipped. What we *can* do without extra shared state is
    // install the MFC-style CRT new-handler so that operator-new failures
    // raise a CMemoryException instead of returning null / std::bad_alloc,
    // matching AfxCriticalNewHandler's contract below.
    std::set_new_handler([]() { AfxThrowMemoryException(); });
    return 1;
}
// Symbol: ?AfxCriticalNewHandler@@YAH_K@Z
extern "C" int MS_ABI impl__AfxCriticalNewHandler__YAH_K_Z(unsigned __int64 p0) {
    (void)p0;
    AfxThrowMemoryException();
    return 0;
}
// Symbol: ?AfxCriticalTerm@@YAXXZ
extern "C" void MS_ABI impl__AfxCriticalTerm__YAXXZ() {
    std::set_new_handler(nullptr);
}
// Symbol: ?AfxCustomLogFont@@YAHIPEAUtagLOGFONTW@@@Z
extern "C" int MS_ABI impl__AfxCustomLogFont__YAHIPEAUtagLOGFONTW___Z(unsigned int p0, void* /*struct*/* p1) {
    // Real MFC's AFX_CUSTOMFONT type constants aren't declared anywhere in this
    // repo's public headers, so the exact nFontType -> LOGFONT-field mapping
    // can't be reproduced verbatim.  We approximate it with the same
    // SPI_GETNONCLIENTMETRICS technique real MFC uses (see also
    // GetChevronOwnerDrawMenuFont() in ManualSmallStubImplementationsSupport.cpp),
    // picking the closest NONCLIENTMETRICS field for a handful of plausible
    // small integer type codes and otherwise falling back to the menu font.
    LOGFONTW* pLogFont = reinterpret_cast<LOGFONTW*>(p1);
    if (pLogFont == nullptr) return 0;
    NONCLIENTMETRICSW ncm{};
    ncm.cbSize = sizeof(ncm);
    if (!::SystemParametersInfoW(SPI_GETNONCLIENTMETRICS, sizeof(ncm), &ncm, 0)) return 0;
    switch (p0) {
        case 1: *pLogFont = ncm.lfCaptionFont; break;
        case 2: *pLogFont = ncm.lfSmCaptionFont; break;
        case 3: *pLogFont = ncm.lfStatusFont; break;
        case 4: *pLogFont = ncm.lfMessageFont; break;
        default: *pLogFont = ncm.lfMenuFont; break;
    }
    return 1;
}
// Symbol: ?AfxDelRegTreeHelper@@YAJPEAUHKEY__@@AEBV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@PEAVCAtlTransactionManager@3@@Z
// LONG AFXAPI AfxDelRegTreeHelper(HKEY hParentKey, const CString& strKeyName,
//                                 CAtlTransactionManager* pTM)
// Signature rewritten: the generator expanded the single `const CString&`
// (one pointer at the ABI level) into a (const void*, void**, void*) triplet,
// which pushed the transaction-manager pointer off R8 onto the stack. The
// three real arguments are RCX/RDX/R8.
// Recursively deletes strKeyName and everything under it, the way SHDeleteKey
// does: open with KEY_READ|KEY_WRITE, repeatedly enumerate subkey 0 and recurse
// into it, then RegDeleteKey the now-empty key. HKEY_CLASSES_ROOT is redirected
// to HKEY_CURRENT_USER\\Software\\Classes when per-user registration is on.
// The CAtlTransactionManager argument is accepted for ABI compatibility and
// forwarded down the recursion, but not otherwise honoured: OpenMFC has no
// CAtlTransactionManager (grepped -- only the CFile/CFileFind/CStdioFile
// constructors take one, and they ignore it too), so the plain ADVAPI32
// registry calls are used instead of the transacted wrappers.
extern "C" long MS_ABI impl__AfxDelRegTreeHelper__YAJPEAUHKEY____AEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__PEAVCAtlTransactionManager_3__Z(
    HKEY hParentKey, const CString* pstrKeyName, void* pTM) {
    // Guard mirrors CWinApp::DelRegTree in phase4/src/core/app/CWinApp.cpp;
    // real MFC has no such check and simply faults / fails the open.
    if (hParentKey == nullptr || pstrKeyName == nullptr) {
        return ERROR_INVALID_PARAMETER;
    }

    CString strKeyName(*pstrKeyName);
    if (hParentKey == HKEY_CLASSES_ROOT && impl__AfxGetPerUserRegistration__YAHXZ()) {
        CString strRedirected(L"Software\\Classes\\");
        strRedirected += strKeyName;
        strKeyName = strRedirected;
        hParentKey = HKEY_CURRENT_USER;
    }

    HKEY hKey = nullptr;
    LONG lRes = ::RegOpenKeyExW(hParentKey, strKeyName.GetString(), 0,
                                KEY_READ | KEY_WRITE, &hKey);
    if (lRes != ERROR_SUCCESS) {
        return lRes;
    }

    for (;;) {
        wchar_t szBuffer[MAX_PATH];
        lRes = ::RegEnumKeyW(hKey, 0, szBuffer, MAX_PATH);
        if (lRes != ERROR_SUCCESS) {
            break;
        }
        CString strChild(szBuffer);
        lRes = impl__AfxDelRegTreeHelper__YAJPEAUHKEY____AEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__PEAVCAtlTransactionManager_3__Z(
            hKey, &strChild, pTM);
        if (lRes != ERROR_SUCCESS) {
            break;
        }
    }

    // The subtree is gone once enumeration runs dry (or the key turned out not
    // to be enumerable at all), so the key itself can go.
    if (lRes == ERROR_NO_MORE_ITEMS || lRes == ERROR_BADKEY) {
        lRes = ::RegDeleteKeyW(hParentKey, strKeyName.GetString());
    }
    ::RegCloseKey(hKey);
    return lRes;
}
// Symbol: ?AfxDeleteObject@@YAXPEAPEAX@Z
extern "C" void MS_ABI impl__AfxDeleteObject__YAXPEAPEAX_Z(void** p0) {
    if (p0 && *p0) {
        ::DeleteObject(reinterpret_cast<HGDIOBJ>(*p0));
        *p0 = nullptr;
    }
}
// Symbol: ?AfxDlgProc@@YA_JPEAUHWND__@@I_K_J@Z
extern "C" __int64 MS_ABI impl__AfxDlgProc__YA_JPEAUHWND____I_K_J_Z(void* /*struct*/* p0, unsigned int p1, unsigned __int64 p2, __int64 p3) {
    return static_cast<__int64>(AfxDlgProc(reinterpret_cast<HWND>(p0), p1, static_cast<WPARAM>(p2), static_cast<LPARAM>(p3)));
}
// Symbol: ?AfxDllCanUnloadNow@@YAJXZ
extern "C" long MS_ABI impl__AfxDllCanUnloadNow__YAJXZ() {
    // Approximate real MFC behavior: unloadable when the OLE server lock
    // count (maintained by AfxOleLockApp/AfxOleUnlockApp; see
    // detail/OlecoreSupport.{h,cpp}) has dropped to zero. We don't
    // separately track outstanding object-instance counts, so this is a
    // conservative approximation of the real S_OK/S_FALSE decision.
    return (openmfc::detail::olecore::g_nOleLockCount == 0) ? S_OK : S_FALSE;
}
// Symbol: ?AfxDllGetClassObject@@YAJAEBU_GUID@@0PEAPEAX@Z
extern "C" long MS_ABI impl__AfxDllGetClassObject__YAJAEBU_GUID__0PEAPEAX_Z(const void* /*struct*/* p0, const void* /*struct*/* p1, void** p2) {
    if (!p2) return E_POINTER;
    *p2 = nullptr;
    if (!p0 || !p1) return E_INVALIDARG;
    const CLSID& clsid = *reinterpret_cast<const CLSID*>(p0);
    const IID& riid = *reinterpret_cast<const IID*>(p1);
    // Walk the real registered-factory list: COleObjectFactory instances
    // add themselves to g_oleObjectFactories on construction (see
    // detail/OlecoreSupport.{h,cpp} and core/ole/COleObjectFactory.cpp).
    for (COleObjectFactory* pFactory : openmfc::detail::olecore::g_oleObjectFactories) {
        if (pFactory && IsEqualCLSID(pFactory->GetClassID(), clsid)) {
            return pFactory->m_xClassFactory.QueryInterface(riid, p2);
        }
    }
    return CLASS_E_CLASSNOTAVAILABLE;
}
// Symbol: ?AfxDoRegisterMFCControlClass@@YAXPEB_W0@Z
extern "C" void MS_ABI impl__AfxDoRegisterMFCControlClass__YAXPEB_W0_Z(const wchar_t* p0, const wchar_t* p1) {
    // Superclass an existing (system) window class pszBaseClass under a new
    // name pszClassName, the same technique MFC uses to register its
    // FeaturePack control classes on top of common controls.
    const wchar_t* pszClassName = p0;
    const wchar_t* pszBaseClass = p1;
    if (!pszClassName || !pszClassName[0] || !pszBaseClass || !pszBaseClass[0]) {
        return;
    }
    WNDCLASSEXW wc = {};
    wc.cbSize = sizeof(wc);
    // Already registered under the new name? Nothing to do.
    if (::GetClassInfoExW(nullptr, pszClassName, &wc)) {
        return;
    }
    HINSTANCE hInst = ::GetModuleHandleW(nullptr);
    wc.cbSize = sizeof(wc);
    if (!::GetClassInfoExW(nullptr, pszBaseClass, &wc) &&
        !::GetClassInfoExW(hInst, pszBaseClass, &wc)) {
        return;
    }
    wc.lpszClassName = pszClassName;
    wc.hInstance = hInst;
    ::RegisterClassExW(&wc);
}
// Symbol: ?AfxDrawDitheredBitmap@@YAXPEAVCDC@@HHAEBVCBitmap@@KK@Z
extern "C" void MS_ABI impl__AfxDrawDitheredBitmap__YAXPEAVCDC__HHAEBVCBitmap__KK_Z(void* /*class*/* p0, int p1, int p2, const void* /*class*/* p3, unsigned long p4, unsigned long p5) {
    // Builds the same ordered-dither image as AfxGetDitheredBitmap (see its
    // comment below) and blits it at (x, y). The logic is duplicated rather
    // than calling that sibling function directly because GCC does not
    // accept a block-scope `extern "C"` forward declaration (verified: it is
    // a hard syntax error, not just non-portable), and a file-scope
    // declaration is off-limits for this shard.
    CDC* pDC = reinterpret_cast<CDC*>(p0);
    const CBitmap* pSrc = reinterpret_cast<const CBitmap*>(p3);
    if (pDC == nullptr || pDC->GetSafeHdc() == nullptr || pSrc == nullptr) return;
    HBITMAP hSrc = reinterpret_cast<HBITMAP>(pSrc->GetSafeHandle());
    if (hSrc == nullptr) return;
    BITMAP bm{};
    if (::GetObjectW(hSrc, sizeof(bm), &bm) == 0 || bm.bmWidth <= 0 || bm.bmHeight <= 0) return;

    HDC hSrcDC = ::CreateCompatibleDC(pDC->GetSafeHdc());
    HDC hDstDC = ::CreateCompatibleDC(pDC->GetSafeHdc());
    HBITMAP hDstBmp = ::CreateCompatibleBitmap(pDC->GetSafeHdc(), bm.bmWidth, bm.bmHeight);
    if (hSrcDC != nullptr && hDstDC != nullptr && hDstBmp != nullptr) {
        HGDIOBJ hOldSrc = ::SelectObject(hSrcDC, hSrc);
        HGDIOBJ hOldDst = ::SelectObject(hDstDC, hDstBmp);
        const COLORREF clr1 = static_cast<COLORREF>(p4);
        const COLORREF clr2 = static_cast<COLORREF>(p5);
        for (int y = 0; y < bm.bmHeight; ++y) {
            for (int x = 0; x < bm.bmWidth; ++x) {
                COLORREF c = ::GetPixel(hSrcDC, x, y);
                unsigned lum = (GetRValue(c) * 30u + GetGValue(c) * 59u + GetBValue(c) * 11u) / 100u;
                unsigned threshold = ((x + y) & 1) != 0 ? 170u : 85u;
                ::SetPixel(hDstDC, x, y, (lum < threshold) ? clr1 : clr2);
            }
        }
        ::SelectObject(hSrcDC, hOldSrc);
        ::SelectObject(hDstDC, hOldDst);
        ::BitBlt(pDC->GetSafeHdc(), p1, p2, bm.bmWidth, bm.bmHeight, hDstDC, 0, 0, SRCCOPY);
    }
    if (hDstBmp != nullptr) ::DeleteObject(hDstBmp);
    if (hSrcDC != nullptr) ::DeleteDC(hSrcDC);
    if (hDstDC != nullptr) ::DeleteDC(hDstDC);
}
// Symbol: ?AfxDrawGrayBitmap@@YAXPEAVCDC@@HHAEBVCBitmap@@K@Z
extern "C" void MS_ABI impl__AfxDrawGrayBitmap__YAXPEAVCDC__HHAEBVCBitmap__K_Z(void* /*class*/* p0, int p1, int p2, const void* /*class*/* p3, unsigned long p4) {
    // Builds the same grayscale image as AfxGetGrayBitmap (see its comment
    // below) and blits it at (x, y). Logic duplicated for the same reason
    // documented in AfxDrawDitheredBitmap above.
    CDC* pDC = reinterpret_cast<CDC*>(p0);
    const CBitmap* pSrc = reinterpret_cast<const CBitmap*>(p3);
    if (pDC == nullptr || pDC->GetSafeHdc() == nullptr || pSrc == nullptr) return;
    HBITMAP hSrc = reinterpret_cast<HBITMAP>(pSrc->GetSafeHandle());
    if (hSrc == nullptr) return;
    BITMAP bm{};
    if (::GetObjectW(hSrc, sizeof(bm), &bm) == 0 || bm.bmWidth <= 0 || bm.bmHeight <= 0) return;

    HDC hSrcDC = ::CreateCompatibleDC(pDC->GetSafeHdc());
    HDC hDstDC = ::CreateCompatibleDC(pDC->GetSafeHdc());
    HBITMAP hDstBmp = ::CreateCompatibleBitmap(pDC->GetSafeHdc(), bm.bmWidth, bm.bmHeight);
    if (hSrcDC != nullptr && hDstDC != nullptr && hDstBmp != nullptr) {
        HGDIOBJ hOldSrc = ::SelectObject(hSrcDC, hSrc);
        HGDIOBJ hOldDst = ::SelectObject(hDstDC, hDstBmp);
        const COLORREF crMask = static_cast<COLORREF>(p4);
        for (int y = 0; y < bm.bmHeight; ++y) {
            for (int x = 0; x < bm.bmWidth; ++x) {
                COLORREF c = ::GetPixel(hSrcDC, x, y);
                if (c == crMask) {
                    ::SetPixel(hDstDC, x, y, c);
                    continue;
                }
                BYTE lum = static_cast<BYTE>((GetRValue(c) * 30u + GetGValue(c) * 59u + GetBValue(c) * 11u) / 100u);
                ::SetPixel(hDstDC, x, y, RGB(lum, lum, lum));
            }
        }
        ::SelectObject(hSrcDC, hOldSrc);
        ::SelectObject(hDstDC, hOldDst);
        ::BitBlt(pDC->GetSafeHdc(), p1, p2, bm.bmWidth, bm.bmHeight, hDstDC, 0, 0, SRCCOPY);
    }
    if (hDstBmp != nullptr) ::DeleteObject(hDstBmp);
    if (hSrcDC != nullptr) ::DeleteDC(hSrcDC);
    if (hDstDC != nullptr) ::DeleteDC(hDstDC);
}
// Symbol: ?AfxEditviewTerm@@YAXXZ
extern "C" void MS_ABI impl__AfxEditviewTerm__YAXXZ() {
    // Real MFC frees the process-local _AFX_EDIT_STATE (the shared
    // find/replace strings and flags plus the find/replace dialog pointer).
    // There is no process-wide find/replace state in this codebase: grepped
    // detail/ for a global CFindReplaceDialog pointer -- CFindReplaceDialog
    // (detail/DlgcommonSupport.*) is only ever a caller-owned object.
    // The one piece of module-wide CEditView state that does exist is
    // detail/DocviewSupport.h:111's
    // openmfc::detail::docview::g_editViewExtraStates (per-view buffer,
    // findText/replaceText, printer font, page count), which this hook ought
    // to clear -- but that symbol cannot be referenced from this translation
    // unit: it is a C++ (Itanium-mangled) data symbol and the workflow
    // link-audit rejects any new _Z... undefined here. Clearing it needs
    // either an impl__-style helper exported from
    // detail/DocviewSupport.cpp or that symbol added to the audit baseline;
    // see the report. Left empty rather than half-cleared.
}
// Symbol: ?AfxEnableControlContainer@@YAXPEAVCOccManager@@@Z
// AfxEnableControlContainer(COccManager* pOccManager)
extern "C" void MS_ABI impl__AfxEnableControlContainer__YAXPEAVCOccManager___Z(void* pOccManager) {
    // Retail (RVA 0x237260) stores pOccManager into AFX_MODULE_STATE+0xA0
    // (m_pOccManager), first creating a process-local default COccManager
    // when the argument is NULL (its factory at RVA 0x2384d0 allocates 8
    // bytes -- a lone vfptr -- and stores the COccManager vtable).
    // Neither half is reproducible here: OpenMFC's AFX_MODULE_STATE
    // (detail/RegcoreSupport.h) is a 3-pointer struct with no m_pOccManager
    // slot, and core/ole/COccManager.cpp is thunks only -- there is no
    // COccManager constructor export and no vtable to point an 8-byte
    // default object at, so fabricating one would hand the caller an object
    // that faults on its first virtual call.  The caller's own manager is
    // kept so it is not silently dropped; nothing in OpenMFC consumes it yet
    // (verified: no reference to m_pOccManager anywhere in the tree).
    g4_g_pOccManager = pOccManager;
}
// Symbol: ?AfxEndDeferRegisterClass@@YAHJ@Z
extern "C" int MS_ABI impl__AfxEndDeferRegisterClass__YAHJ_Z(long p0) {
    // OpenMFC only models the single base "AfxWnd"-style window class
    // (RegisterOpenMFCClass/g_atomOpenMFCClass); register it on demand for
    // any requested AFX_WND_REG bit and report success. Real MFC registers a
    // distinct class per bit (AfxWnd/AfxControlBar/AfxMDIFrame/...), which
    // OpenMFC does not yet model separately.
    (void)p0;
    if (!g_atomOpenMFCClass) {
        HINSTANCE hInst = AfxGetInstanceHandle();
        if (!hInst) hInst = ::GetModuleHandleW(nullptr);
        g_atomOpenMFCClass = RegisterOpenMFCClass(hInst);
    }
    return TRUE;
}
// Symbol: ?AfxEndThread@@YAXIH@Z
extern "C" void MS_ABI impl__AfxEndThread__YAXIH_Z(unsigned int p0, int p1) {
    AfxEndThread(p0, p1);
}
// Symbol: ?AfxEnumMetaFileProc@@YAHPEAUHDC__@@PEAUtagHANDLETABLE@@PEAUtagMETARECORD@@H_J@Z
extern "C" int MS_ABI impl__AfxEnumMetaFileProc__YAHPEAUHDC____PEAUtagHANDLETABLE__PEAUtagMETARECORD__H_J_Z(void* /*struct*/* p0, void* /*struct*/* p1, void* /*struct*/* p2, int p3, __int64 p4) {
    (void)p4;
    ::PlayMetaFileRecord(reinterpret_cast<HDC>(p0), reinterpret_cast<LPHANDLETABLE>(p1),
                          reinterpret_cast<LPMETARECORD>(p2), static_cast<UINT>(p3));
    return 1;
}
// Symbol: ?AfxFindMessageEntry@@YAPEBUAFX_MSGMAP_ENTRY@@PEBU1@III@Z
extern "C" void* MS_ABI impl__AfxFindMessageEntry__YAPEBUAFX_MSGMAP_ENTRY__PEBU1_III_Z(const void* /*struct*/* p0, unsigned int p1, unsigned int p2, unsigned int p3) {
    const AFX_MSGMAP_ENTRY* lpEntry = reinterpret_cast<const AFX_MSGMAP_ENTRY*>(p0);
    if (!lpEntry) return nullptr;
    UINT nMsg = p1;
    UINT nCode = p2;
    UINT nID = p3;
    while (lpEntry->nSig != AfxSig_end) {
        if (lpEntry->nMessage == nMsg && lpEntry->nCode == nCode &&
            nID >= lpEntry->nID && nID <= lpEntry->nLastID) {
            return const_cast<AFX_MSGMAP_ENTRY*>(lpEntry);
        }
        ++lpEntry;
    }
    return nullptr;
}
// Symbol: ?AfxFindStringResourceHandle@@YAPEAUHINSTANCE__@@I@Z
extern "C" void* MS_ABI impl__AfxFindStringResourceHandle__YAPEAUHINSTANCE____I_Z(unsigned int p0) {
    // Walk the string-table resource block that would contain nID, using the
    // same module search order as AfxFindResourceHandle (Globals.cpp):
    // resource handle, then instance handle, then the main module. (A
    // local `extern "C"` re-declaration to forward straight to that
    // function is not legal C++ inside a function body -- linkage
    // specifications may only occur at namespace scope -- so the same
    // small search is reproduced here via the ordinary C++-linkage
    // AfxGetResourceHandle()/AfxGetInstanceHandle() accessors already
    // available from CWinAppSupport.h.)
    UINT nID = p0;
    LPCWSTR lpszBlock = MAKEINTRESOURCEW(nID / 16 + 1);
    LPCWSTR lpszType = MAKEINTRESOURCEW(6 /* RT_STRING */);

    HINSTANCE hRes = AfxGetResourceHandle();
    if (hRes && ::FindResourceW(hRes, lpszBlock, lpszType)) {
        return hRes;
    }
    HINSTANCE hInst = AfxGetInstanceHandle();
    if (hInst && ::FindResourceW(hInst, lpszBlock, lpszType)) {
        return hInst;
    }
    HINSTANCE hMain = ::GetModuleHandleW(nullptr);
    if (hMain && hMain != hInst && hMain != hRes && ::FindResourceW(hMain, lpszBlock, lpszType)) {
        return hMain;
    }
    return nullptr;
}
// Symbol: ?AfxGetAfxWndProc@@YAP6A_JPEAUHWND__@@I_K_J@ZXZ
extern "C" void* MS_ABI impl__AfxGetAfxWndProc__YAP6A_JPEAUHWND____I_K_J_ZXZ() {
    return reinterpret_cast<void*>(static_cast<LRESULT(CALLBACK*)(HWND, UINT, WPARAM, LPARAM)>(&AfxWndProc));
}
// Symbol: ?AfxGetClassIDFromString@@YAJPEB_WPEAU_GUID@@@Z
extern "C" long MS_ABI impl__AfxGetClassIDFromString__YAJPEB_WPEAU_GUID___Z(const wchar_t* p0, void* /*struct*/* p1) {
    if (!p0 || !p1) return E_INVALIDARG;
    CLSID* pclsid = reinterpret_cast<CLSID*>(p1);
    HRESULT hr = ::CLSIDFromString(p0, pclsid);
    if (FAILED(hr)) {
        // Not a "{...}" string form -- try it as a ProgID.
        hr = ::CLSIDFromProgID(p0, pclsid);
    }
    return hr;
}
// Symbol: ?AfxGetCurrentMessage@@YAPEAUtagMSG@@XZ
extern "C" void* MS_ABI impl__AfxGetCurrentMessage__YAPEAUtagMSG__XZ() {
    return const_cast<MSG*>(AfxGetThread() ? CWnd::GetCurrentMessage() : nullptr);
}
// Symbol: ?AfxGetDitheredBitmap@@YAXAEBVCBitmap@@PEAV1@KK@Z
extern "C" void MS_ABI impl__AfxGetDitheredBitmap__YAXAEBVCBitmap__PEAV1_KK_Z(const void* /*class*/* p0, void* /*class*/* p1, unsigned long p2, unsigned long p3) {
    // NOTE: this reproduces the *effect* (a same-size ordered-dither copy of
    // the source bitmap blended between the two given colors) rather than
    // MFC's exact undocumented pixel algorithm, which isn't recoverable from
    // this repo. Uses a 2-level checkerboard (Bayer-style) ordered dither on
    // source luminance between crColor1 (p2) and crColor2 (p3).
    const CBitmap* pSrc = reinterpret_cast<const CBitmap*>(p0);
    CBitmap* pDst = reinterpret_cast<CBitmap*>(p1);
    if (pSrc == nullptr || pDst == nullptr) return;
    HBITMAP hSrc = reinterpret_cast<HBITMAP>(pSrc->GetSafeHandle());
    if (hSrc == nullptr) return;
    BITMAP bm{};
    if (::GetObjectW(hSrc, sizeof(bm), &bm) == 0 || bm.bmWidth <= 0 || bm.bmHeight <= 0) return;

    HDC hScreenDC = ::GetDC(nullptr);
    HDC hSrcDC = ::CreateCompatibleDC(hScreenDC);
    HDC hDstDC = ::CreateCompatibleDC(hScreenDC);
    HBITMAP hDstBmp = ::CreateCompatibleBitmap(hScreenDC, bm.bmWidth, bm.bmHeight);
    if (hSrcDC != nullptr && hDstDC != nullptr && hDstBmp != nullptr) {
        HGDIOBJ hOldSrc = ::SelectObject(hSrcDC, hSrc);
        HGDIOBJ hOldDst = ::SelectObject(hDstDC, hDstBmp);
        const COLORREF clr1 = static_cast<COLORREF>(p2);
        const COLORREF clr2 = static_cast<COLORREF>(p3);
        for (int y = 0; y < bm.bmHeight; ++y) {
            for (int x = 0; x < bm.bmWidth; ++x) {
                COLORREF c = ::GetPixel(hSrcDC, x, y);
                unsigned lum = (GetRValue(c) * 30u + GetGValue(c) * 59u + GetBValue(c) * 11u) / 100u;
                unsigned threshold = ((x + y) & 1) != 0 ? 170u : 85u;
                ::SetPixel(hDstDC, x, y, (lum < threshold) ? clr1 : clr2);
            }
        }
        ::SelectObject(hSrcDC, hOldSrc);
        ::SelectObject(hDstDC, hOldDst);
        pDst->DeleteObject();
        impl__Attach_CGdiObject__QEAAHPEAX_Z(pDst, hDstBmp);
    } else if (hDstBmp != nullptr) {
        ::DeleteObject(hDstBmp);
    }
    if (hSrcDC != nullptr) ::DeleteDC(hSrcDC);
    if (hDstDC != nullptr) ::DeleteDC(hDstDC);
    ::ReleaseDC(nullptr, hScreenDC);
}
// Symbol: ?AfxGetDlgItemStartFromHWND@@YAPEAUHWND__@@HPEAU1@@Z
extern "C" void* MS_ABI impl__AfxGetDlgItemStartFromHWND__YAPEAUHWND____HPEAU1__Z(int p0, void* /*struct*/* p1) {
    return reinterpret_cast<void*>(::GetDlgItem(reinterpret_cast<HWND>(p1), p0));
}
// Symbol: ?AfxGetFileTitle@@YAIPEB_WPEA_WI@Z
extern "C" unsigned int MS_ABI impl__AfxGetFileTitle__YAIPEB_WPEA_WI_Z(const wchar_t* p0, wchar_t* p1, unsigned int p2) {
    if (!p0 || !p1 || p2 == 0) return 0;
    const wchar_t* filePart = p0;
    const wchar_t* backslash = std::wcsrchr(p0, L'\\');
    const wchar_t* slash = std::wcsrchr(p0, L'/');
    if (backslash && slash) {
        filePart = (backslash > slash) ? backslash + 1 : slash + 1;
    } else if (backslash) {
        filePart = backslash + 1;
    } else if (slash) {
        filePart = slash + 1;
    }
    size_t len = std::wcslen(filePart);
    if (len >= p2) len = p2 - 1;
    std::memcpy(p1, filePart, len * sizeof(wchar_t));
    p1[len] = L'\0';
    return static_cast<unsigned int>(len);
}
// Symbol: ?AfxGetGrayBitmap@@YAXAEBVCBitmap@@PEAV1@K@Z
extern "C" void MS_ABI impl__AfxGetGrayBitmap__YAXAEBVCBitmap__PEAV1_K_Z(const void* /*class*/* p0, void* /*class*/* p1, unsigned long p2) {
    // Per-pixel luminance grayscale copy of the source bitmap into the
    // destination bitmap, sized identically to the source. crMask (p2) is
    // treated as a transparency/mask color that is copied through unchanged
    // (this mirrors the common MFC pattern of masking a designated color
    // out of gray/disabled toolbar image conversions); it isn't a byte-exact
    // reproduction of MFC's internal disabled-icon algorithm.
    const CBitmap* pSrc = reinterpret_cast<const CBitmap*>(p0);
    CBitmap* pDst = reinterpret_cast<CBitmap*>(p1);
    if (pSrc == nullptr || pDst == nullptr) return;
    HBITMAP hSrc = reinterpret_cast<HBITMAP>(pSrc->GetSafeHandle());
    if (hSrc == nullptr) return;
    BITMAP bm{};
    if (::GetObjectW(hSrc, sizeof(bm), &bm) == 0 || bm.bmWidth <= 0 || bm.bmHeight <= 0) return;

    HDC hScreenDC = ::GetDC(nullptr);
    HDC hSrcDC = ::CreateCompatibleDC(hScreenDC);
    HDC hDstDC = ::CreateCompatibleDC(hScreenDC);
    HBITMAP hDstBmp = ::CreateCompatibleBitmap(hScreenDC, bm.bmWidth, bm.bmHeight);
    if (hSrcDC != nullptr && hDstDC != nullptr && hDstBmp != nullptr) {
        HGDIOBJ hOldSrc = ::SelectObject(hSrcDC, hSrc);
        HGDIOBJ hOldDst = ::SelectObject(hDstDC, hDstBmp);
        const COLORREF crMask = static_cast<COLORREF>(p2);
        for (int y = 0; y < bm.bmHeight; ++y) {
            for (int x = 0; x < bm.bmWidth; ++x) {
                COLORREF c = ::GetPixel(hSrcDC, x, y);
                if (c == crMask) {
                    ::SetPixel(hDstDC, x, y, c);
                    continue;
                }
                BYTE lum = static_cast<BYTE>((GetRValue(c) * 30u + GetGValue(c) * 59u + GetBValue(c) * 11u) / 100u);
                ::SetPixel(hDstDC, x, y, RGB(lum, lum, lum));
            }
        }
        ::SelectObject(hSrcDC, hOldSrc);
        ::SelectObject(hDstDC, hOldDst);
        pDst->DeleteObject();
        impl__Attach_CGdiObject__QEAAHPEAX_Z(pDst, hDstBmp);
    } else if (hDstBmp != nullptr) {
        ::DeleteObject(hDstBmp);
    }
    if (hSrcDC != nullptr) ::DeleteDC(hSrcDC);
    if (hDstDC != nullptr) ::DeleteDC(hDstDC);
    ::ReleaseDC(nullptr, hScreenDC);
}
// Symbol: ?AfxGetHENV@@YAPEAXXZ
extern "C" void* MS_ABI impl__AfxGetHENV__YAPEAXXZ() {
    // Lazily allocate a process-wide ODBC environment handle, mirroring real
    // MFC's cached global HENV. sql.h/sqlext.h aren't part of this file's
    // include set (adding them would require a new file-scope #include), so
    // the handful of ODBC constants/prototypes needed are declared locally
    // and odbc32.dll is resolved dynamically -- this is a real, working ODBC
    // environment handle, just obtained without the SDK headers. Note this
    // is a standalone environment handle: CDatabase in this codebase
    // allocates its own private SQL_HANDLE_ENV per instance rather than
    // calling through this function, so the two aren't currently unified.
    static void* s_henv = nullptr;
    static bool s_attempted = false;
    if (!s_attempted) {
        s_attempted = true;
        HMODULE hOdbc = ::LoadLibraryW(L"odbc32.dll");
        if (hOdbc != nullptr) {
            using PFN_SQLAllocHandle = short(__stdcall*)(short, void*, void**);
            using PFN_SQLSetEnvAttr = short(__stdcall*)(void*, long, void*, long);
            auto pAllocHandle = reinterpret_cast<PFN_SQLAllocHandle>(
                reinterpret_cast<void*>(::GetProcAddress(hOdbc, "SQLAllocHandle")));
            auto pSetEnvAttr = reinterpret_cast<PFN_SQLSetEnvAttr>(
                reinterpret_cast<void*>(::GetProcAddress(hOdbc, "SQLSetEnvAttr")));
            if (pAllocHandle != nullptr) {
                const short SQL_HANDLE_ENV = 1;
                void* henv = nullptr;
                short rc = pAllocHandle(SQL_HANDLE_ENV, nullptr, &henv);
                if (rc >= 0 && henv != nullptr) {
                    if (pSetEnvAttr != nullptr) {
                        const long SQL_ATTR_ODBC_VERSION = 200;
                        const long SQL_OV_ODBC3 = 3;
                        pSetEnvAttr(henv, SQL_ATTR_ODBC_VERSION,
                                    reinterpret_cast<void*>(static_cast<intptr_t>(SQL_OV_ODBC3)), 0);
                    }
                    s_henv = henv;
                }
            }
        }
    }
    return s_henv;
}
// Symbol: ?AfxGetInProcServer@@YAHPEB_WAEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@@Z
extern "C" int MS_ABI impl__AfxGetInProcServer__YAHPEB_WAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(const wchar_t* p0, void* /*class*/* p1, void** p2, void* p3) {
    (void)p2;
    (void)p3;
    // p1 is the raw pointer that lands in the reference argument's ABI
    // slot. CString and CStringT<wchar_t,...> share the single-pointer
    // CSimpleStringT layout (see detail/CStringTSupport.h), so it aliases
    // a CString* directly -- no extra indirection despite the "void**"
    // spelling the auto-generated signature gives it.
    CString* pStrDllPath = reinterpret_cast<CString*>(p1);
    if (!pStrDllPath) return FALSE;
    pStrDllPath->Empty();
    if (!p0 || !*p0) return FALSE;

    CLSID clsid;
    if (FAILED(::CLSIDFromProgID(p0, &clsid)) && FAILED(::CLSIDFromString(p0, &clsid)))
        return FALSE;

    wchar_t clsidText[64] = {};
    if (::StringFromGUID2(clsid, clsidText, 64) == 0) return FALSE;

    CString keyPath = CString(L"CLSID\\") + clsidText + L"\\InProcServer32";
    HKEY hKey = nullptr;
    if (::RegOpenKeyExW(HKEY_CLASSES_ROOT, static_cast<const wchar_t*>(keyPath), 0, KEY_QUERY_VALUE, &hKey) != ERROR_SUCCESS)
        return FALSE;

    wchar_t path[MAX_PATH] = {};
    DWORD cb = sizeof(path);
    DWORD type = 0;
    LONG res = ::RegQueryValueExW(hKey, nullptr, nullptr, &type, reinterpret_cast<BYTE*>(path), &cb);
    ::RegCloseKey(hKey);
    if (res != ERROR_SUCCESS || (type != REG_SZ && type != REG_EXPAND_SZ)) return FALSE;

    *pStrDllPath = path;
    return TRUE;
}
// Symbol: ?AfxGetInternetHandleType@@YAKPEAX@Z
extern "C" unsigned long MS_ABI impl__AfxGetInternetHandleType__YAKPEAX_Z(void* p0) {
    // InternetQueryOptionW(hInternet, INTERNET_OPTION_HANDLE_TYPE, &dwType, &cb)
    // A block-scope `extern "C"` declaration is not legal C++ (linkage-specifications
    // are namespace-scope only), so resolve the wininet.dll entry point dynamically
    // instead of adding a file-scope #include/declaration.
    typedef int (WINAPI *PFN_InternetQueryOptionW)(void*, unsigned long, void*, unsigned long*);
    static PFN_InternetQueryOptionW s_pfnInternetQueryOptionW = nullptr;
    if (!s_pfnInternetQueryOptionW) {
        HMODULE hWinInet = ::LoadLibraryW(L"wininet.dll");
        if (hWinInet) {
            s_pfnInternetQueryOptionW = reinterpret_cast<PFN_InternetQueryOptionW>(
                ::GetProcAddress(hWinInet, "InternetQueryOptionW"));
        }
    }
    if (!p0 || !s_pfnInternetQueryOptionW) return 0;
    unsigned long dwType = 0;
    unsigned long cb = sizeof(dwType);
    const unsigned long kInternetOptionHandleType = 9; // INTERNET_OPTION_HANDLE_TYPE
    if (!s_pfnInternetQueryOptionW(p0, kInternetOptionHandleType, &dwType, &cb)) return 0;
    return dwType;
}
// Symbol: ?AfxGetModuleFileName@@YAXPEAUHINSTANCE__@@AEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@@Z
extern "C" void MS_ABI impl__AfxGetModuleFileName__YAXPEAUHINSTANCE____AEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(void* /*struct*/* p0, void* /*class*/* p1, void** p2, void* p3) {
    // p1 is the real CString& (single pointer, matches CStringT<wchar_t> ABI layout).
    // p2/p3 are demangler artifacts from splitting the CStringT<> template into a
    // bogus (void*, void**, void*) triplet; the CString ref here is the LAST real
    // parameter so nothing else is shifted by the artifact slots. Never read p2/p3.
    (void)p2; (void)p3;
    CString* pStrFileName = reinterpret_cast<CString*>(p1);
    if (!pStrFileName) return;
    HINSTANCE hInst = reinterpret_cast<HINSTANCE>(p0);
    wchar_t buf[32768];
    DWORD len = ::GetModuleFileNameW(hInst, buf, 32768);
    if (len == 0) { pStrFileName->Empty(); return; }
    *pStrFileName = buf;
}
// Symbol: ?AfxGetModuleShortFileName@@YAXPEAUHINSTANCE__@@AEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@@Z
extern "C" void MS_ABI impl__AfxGetModuleShortFileName__YAXPEAUHINSTANCE____AEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(void* /*struct*/* p0, void* /*class*/* p1, void** p2, void* p3) {
    // See AfxGetModuleFileName above: p1 is the real CString&, p2/p3 are artifacts
    // of the CStringT<> template being the last (and only) reference parameter.
    (void)p2; (void)p3;
    CString* pStrShortName = reinterpret_cast<CString*>(p1);
    if (!pStrShortName) return;
    HINSTANCE hInst = reinterpret_cast<HINSTANCE>(p0);
    wchar_t buf[MAX_PATH];
    DWORD len = ::GetModuleFileNameW(hInst, buf, MAX_PATH);
    if (len == 0) { pStrShortName->Empty(); return; }
    wchar_t shortBuf[MAX_PATH];
    DWORD shortLen = ::GetShortPathNameW(buf, shortBuf, MAX_PATH);
    if (shortLen == 0 || shortLen >= MAX_PATH) {
        *pStrShortName = buf; // fall back to the long name
    } else {
        *pStrShortName = shortBuf;
    }
}
// Symbol: ?AfxGetNewHandler@@YAP6AH_K@ZXZ
extern "C" void* MS_ABI impl__AfxGetNewHandler__YAP6AH_K_ZXZ() {
    return g_pfnNewHandler;
}
// Symbol: ?AfxGetParentOwner@@YAPEAUHWND__@@PEAU1@@Z
extern "C" void* MS_ABI impl__AfxGetParentOwner__YAPEAUHWND____PEAU1__Z(void* /*struct*/* p0) {
    auto* pWnd = CWnd::FromHandle(reinterpret_cast<HWND>(p0));
    return pWnd ? pWnd->GetParentOwner() : nullptr;
}
// Symbol: ?AfxGetPerUserRegistration@@YAHXZ
extern "C" int MS_ABI impl__AfxGetPerUserRegistration__YAHXZ() {
    return g_bPerUserRegistration;
}
// Symbol: ?AfxGetPropSheetFont@@YAHAEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@AEAGH@Z
// Left stubbed: the demangled signature is `int AfxGetPropSheetFont(CStringT<...>&, WORD&, int)`
// -- three logical arguments (RCX/RDX/R8 in the real x64 ABI) -- but the
// auto-generated stub below has 5 parameters (p0..p4), which looks like the
// demangler mis-parsed the nested CStringT<wchar_t, StrTraitMFC_DLL<...>>
// template argument into extra params. If real callers only ever pass 3
// register args, the WORD& and int the function actually needs land in
// p1/p2, not the p3/p4 this stub declares -- but that's a guess, and this
// looks like a pre-existing signature-generation issue affecting how this
// symbol's thunk is called, not something safe to paper over inside the
// function body without risking reading the wrong registers as the wrong
// types.
extern "C" int MS_ABI impl__AfxGetPropSheetFont__YAHAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__AEAGH_Z(void* /*class*/* p0, void** p1, void* p2, unsigned short* p3, int p4) {
    (void)p0; (void)p1; (void)p2; (void)p3; (void)p4;
    return 0;
}
// Symbol: ?AfxGetRoot@@YAXPEB_WAEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@@Z
extern "C" void MS_ABI impl__AfxGetRoot__YAXPEB_WAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(const wchar_t* p0, void* /*class*/* p1, void** p2, void* p3) {
    // p1 is the real CString& (single pointer); p2/p3 are CStringT<> demangler
    // artifacts (this is the last real parameter, so nothing is shifted). Never
    // read p2/p3.
    (void)p2; (void)p3;
    CString* pStrRoot = reinterpret_cast<CString*>(p1);
    if (!pStrRoot) return;
    if (!p0) { pStrRoot->Empty(); return; }

    const wchar_t* lpsz = p0;
    if (lpsz[0] == L'\\' && lpsz[1] == L'\\') {
        // UNC name (format: two backslashes, server, backslash, share, backslash)
        const wchar_t* p = lpsz + 2;
        while (*p != L'\0' && *p != L'\\') ++p;
        if (*p == L'\\') {
            ++p;
            while (*p != L'\0' && *p != L'\\') ++p;
            if (*p == L'\\') ++p;
            size_t len = static_cast<size_t>(p - lpsz);
            wchar_t* buf = pStrRoot->GetBuffer(static_cast<int>(len));
            std::memcpy(buf, lpsz, len * sizeof(wchar_t));
            pStrRoot->ReleaseBuffer(static_cast<int>(len));
        } else {
            pStrRoot->Empty(); // malformed UNC path
        }
    } else if (lpsz[0] != L'\0' && lpsz[1] == L':') {
        size_t len = (lpsz[2] == L'\\') ? 3 : 2;
        wchar_t* buf = pStrRoot->GetBuffer(static_cast<int>(len));
        std::memcpy(buf, lpsz, len * sizeof(wchar_t));
        pStrRoot->ReleaseBuffer(static_cast<int>(len));
    } else {
        pStrRoot->Empty();
    }
}
// Symbol: ?AfxGlobalFree@@YAXPEAX@Z
extern "C" void MS_ABI impl__AfxGlobalFree__YAXPEAX_Z(void* p0) {
    ::GlobalFree(p0);
}
// Symbol: ?AfxGlobalsAddRef@@YAXXZ
// Real MFC increments a process-wide refcount shared with AfxGlobalsRelease,
// gating first-time global-data construction on the 0->1 transition.
// OpenMFC's shared global state (CProcessLocalObject, etc.) already
// lazy-constructs itself independently via its own guards, so there is no
// separate one-time construction step for this refcount to gate; it still
// tracks the ref count itself, using g_afxGlobalsRefCount (declared near
// the top of this file, alongside g_afxTlsRefCount).
extern "C" void MS_ABI impl__AfxGlobalsAddRef__YAXXZ() {
    ::InterlockedIncrement(&g_afxGlobalsRefCount);
}
// Symbol: ?AfxGlobalsRelease@@YAXXZ
extern "C" void MS_ABI impl__AfxGlobalsRelease__YAXXZ() {
    ::InterlockedDecrement(&g_afxGlobalsRefCount);
}
// Symbol: ?AfxHelpEnabled@@YAHXZ
extern "C" int MS_ABI impl__AfxHelpEnabled__YAHXZ() {
    CWinApp* pApp = AfxGetApp();
    return (pApp && pApp->m_pszHelpFilePath && pApp->m_pszHelpFilePath[0]) ? TRUE : FALSE;
}
namespace {
// WH_CBT filter installed by AfxHookWindowCreate below. On HCBT_CREATEWND
// (fired just before the new HWND is shown to the app) it attaches the
// CWnd stashed in _AFX_THREAD_STATE::pWndInit by AfxHookWindowCreate to the
// freshly created HWND via CWnd::Attach, matching real MFC's
// AfxHookWindowCreate/PreCreateWindow contract.
LRESULT CALLBACK AfxCbtFilterHook(int nCode, WPARAM wParam, LPARAM lParam) {
    _AFX_THREAD_STATE* pState = AfxGetThreadState();
    if (nCode == HCBT_CREATEWND && pState && pState->pWndInit) {
        CWnd* pWnd = static_cast<CWnd*>(pState->pWndInit);
        pWnd->Attach(reinterpret_cast<HWND>(wParam));
        pState->pWndInit = nullptr;
    }
    HHOOK hHook = pState ? reinterpret_cast<HHOOK>(pState->hHookOldCbtFilter) : nullptr;
    return ::CallNextHookEx(hHook, nCode, wParam, lParam);
}
}
// Symbol: ?AfxHookWindowCreate@@YAXPEAVCWnd@@@Z
extern "C" void MS_ABI impl__AfxHookWindowCreate__YAXPEAVCWnd___Z(void* /*class*/* p0) {
    CWnd* pWnd = reinterpret_cast<CWnd*>(p0);
    _AFX_THREAD_STATE* pState = AfxGetThreadState();
    if (!pState || !pWnd) return;
    if (pState->pWndInit == pWnd) return;
    if (!pState->hHookOldCbtFilter) {
        HHOOK hHook = ::SetWindowsHookExW(WH_CBT, AfxCbtFilterHook, nullptr, ::GetCurrentThreadId());
        pState->hHookOldCbtFilter = reinterpret_cast<void*>(hHook);
    }
    pState->pWndInit = pWnd;
}
// Symbol: ?AfxHtmlHelp@@YAPEAUHWND__@@PEAU1@PEB_WI_K@Z
extern "C" void* MS_ABI impl__AfxHtmlHelp__YAPEAUHWND____PEAU1_PEB_WI_K_Z(void* /*struct*/* p0, const wchar_t* p1, unsigned int p2, unsigned __int64 p3) {
    using HtmlHelpFn = HWND(WINAPI*)(HWND, LPCWSTR, UINT, ULONG_PTR);
    static HtmlHelpFn s_pHtmlHelpW = nullptr;
    static bool s_tried = false;
    if (!s_tried) {
        s_tried = true;
        HMODULE hMod = ::GetModuleHandleW(L"hhctrl.ocx");
        if (!hMod) hMod = ::LoadLibraryW(L"hhctrl.ocx");
        if (hMod) {
            s_pHtmlHelpW = reinterpret_cast<HtmlHelpFn>(::GetProcAddress(hMod, "HtmlHelpW"));
        }
    }
    if (!s_pHtmlHelpW) return nullptr;
    HWND hWnd = reinterpret_cast<HWND>(p0);
    return reinterpret_cast<void*>(s_pHtmlHelpW(hWnd, p1, p2, static_cast<ULONG_PTR>(p3)));
}
// Symbol: ?AfxInitCurrentStateApp@@YAHXZ
extern "C" int MS_ABI impl__AfxInitCurrentStateApp__YAHXZ() {
    // Honest, minimal reading: report whether app-level module state has
    // already come up (an app object exists), rather than fabricating a
    // default CWinApp construction path OpenMFC has no infrastructure for.
    return AfxGetApp() != nullptr;
}
// Symbol: ?AfxInitExtensionModule@@YAHAEAUAFX_EXTENSION_MODULE@@PEAUHINSTANCE__@@@Z
extern "C" int MS_ABI impl__AfxInitExtensionModule__YAHAEAUAFX_EXTENSION_MODULE__PEAUHINSTANCE_____Z(void* /*struct*/* p0, void* /*struct*/* p1) {
    // Mirrors the real afxext.h AFX_EXTENSION_MODULE layout (also mirrored
    // in detail/COleControlModuleSupport.h, not included here): BOOL
    // bInitialized; HMODULE hModule; HMODULE hResourceModule;
    // CRuntimeClass* pFirstSharedClass; COleObjectFactory* pFirstSharedFactory.
    struct LocalExtModule {
        int   bInitialized;
        void* hModule;
        void* hResourceModule;
        void* pFirstSharedClass;
        void* pFirstSharedFactory;
    };
    auto* pState = reinterpret_cast<LocalExtModule*>(p0);
    if (!pState) return 0;
    if (pState->bInitialized) return 0;
    pState->bInitialized = 1;
    pState->hModule = p1;
    pState->hResourceModule = p1;
    // OpenMFC has no per-module shared runtime-class/factory registration
    // list to chain the extension module onto, so these honestly stay null
    // (no shared classes/factories registered) rather than being faked up.
    pState->pFirstSharedClass = nullptr;
    pState->pFirstSharedFactory = nullptr;
    return 1;
}
// Symbol: ?AfxInitLocalData@@YAXPEAUHINSTANCE__@@@Z
extern "C" void MS_ABI impl__AfxInitLocalData__YAXPEAUHINSTANCE_____Z(void* /*struct*/* p0) {
    // Records this module's instance/resource handle into the current
    // AFX_MODULE_STATE (detail/RegcoreSupport.h layout: two HINSTANCEs
    // followed by a CWinApp*), reusing the already-exported
    // impl__AfxGetModuleState accessor rather than adding new state.
    struct LocalModuleState {
        HINSTANCE m_hCurrentInstanceHandle;
        HINSTANCE m_hCurrentResourceHandle;
        void*     m_pCurrentWinApp;
    };
    // AfxGetAppModuleState() is already declared (with plain C++ linkage,
    // AFXAPI calling convention) in openmfc/afxwin.h -- no new declaration
    // needed.
    auto* pState = reinterpret_cast<LocalModuleState*>(AfxGetAppModuleState());
    if (!pState) return;
    HINSTANCE hInstance = reinterpret_cast<HINSTANCE>(p0);
    pState->m_hCurrentInstanceHandle = hInstance;
    pState->m_hCurrentResourceHandle = hInstance;
}
// Symbol: ?AfxInitNetworkAddressControl@@YAHXZ
extern "C" int MS_ABI impl__AfxInitNetworkAddressControl__YAHXZ() {
    // ICC_NETWORK_CLASS (0x00004000) is not defined by the mingw-w64
    // commctrl.h shipped in this toolchain, so define it locally.
    const DWORD kICC_NETWORK_CLASS = 0x00004000;
    INITCOMMONCONTROLSEX icc = {};
    icc.dwSize = sizeof(icc);
    icc.dwICC = kICC_NETWORK_CLASS;
    return ::InitCommonControlsEx(&icc) ? TRUE : FALSE;
}
// Symbol: ?AfxInitRichEdit2@@YAHXZ
extern "C" int MS_ABI impl__AfxInitRichEdit2__YAHXZ() {
    static bool s_tried = false;
    static HMODULE s_hMod = nullptr;
    if (!s_tried) {
        s_tried = true;
        s_hMod = ::LoadLibraryW(L"riched20.dll");
    }
    return s_hMod ? TRUE : FALSE;
}
// Symbol: ?AfxInitRichEdit5@@YAHXZ
extern "C" int MS_ABI impl__AfxInitRichEdit5__YAHXZ() {
    static bool s_tried = false;
    static HMODULE s_hMod = nullptr;
    if (!s_tried) {
        s_tried = true;
        s_hMod = ::LoadLibraryW(L"msftedit.dll");
    }
    return s_hMod ? TRUE : FALSE;
}
// Symbol: ?AfxInitRichEdit@@YAHXZ
extern "C" int MS_ABI impl__AfxInitRichEdit__YAHXZ() {
    static bool s_tried = false;
    static HMODULE s_hMod = nullptr;
    if (!s_tried) {
        s_tried = true;
        s_hMod = ::LoadLibraryW(L"riched32.dll");
    }
    return s_hMod ? TRUE : FALSE;
}
// Symbol: ?AfxInitThread@@YAXXZ
extern "C" void MS_ABI impl__AfxInitThread__YAXXZ() {
    // Resets this thread's AFX bookkeeping to defaults for a freshly
    // attaching thread. _AFX_THREAD_STATE (detail/CWinAppSupport.h) is the
    // canonical thread-local state, already shared process-wide via the
    // publicly declared AfxGetThreadState().
    if (_AFX_THREAD_STATE* pState = AfxGetThreadState()) {
        pState->nTempMapLock = 0;
        pState->nWndCreateLock = 0;
    }
}
// Symbol: ?AfxInternalIsIdleMessage@@YAHPEAUtagMSG@@@Z
extern "C" int MS_ABI impl__AfxInternalIsIdleMessage__YAHPEAUtagMSG___Z(void* /*struct*/* p0) {
    auto* pThread = AfxGetThread();
    return pThread ? pThread->IsIdleMessage(reinterpret_cast<MSG*>(p0)) : FALSE;
}
// Symbol: ?AfxInternalPreTranslateMessage@@YAHPEAUtagMSG@@@Z
extern "C" int MS_ABI impl__AfxInternalPreTranslateMessage__YAHPEAUtagMSG___Z(void* /*struct*/* p0) {
    auto* pThread = AfxGetThread();
    return pThread ? pThread->PreTranslateMessage(reinterpret_cast<MSG*>(p0)) : FALSE;
}
// Symbol: ?AfxInternalProcessWndProcException@@YA_JPEAVCException@@PEBUtagMSG@@@Z
extern "C" __int64 MS_ABI impl__AfxInternalProcessWndProcException__YA_JPEAVCException__PEBUtagMSG___Z(void* /*class*/* p0, const void* /*struct*/* p1) {
    CWinThread* pThread = AfxGetThread();
    if (!pThread) return 0;
    CException* pException = reinterpret_cast<CException*>(p0);
    const MSG* pMsg = reinterpret_cast<const MSG*>(p1);
    if (auto* pApp = dynamic_cast<CWinApp*>(pThread)) {
        return impl__ProcessWndProcException_CWinApp__UEAA_JPEAVCException__PEBUtagMSG___Z(pApp, pException, pMsg);
    }
    return impl__ProcessWndProcException_CWinThread__UEAA_JPEAVCException__PEBUtagMSG___Z(pThread, pException, pMsg);
}
// Symbol: ?AfxInternalPumpMessage@@YAHXZ
extern "C" int MS_ABI impl__AfxInternalPumpMessage__YAHXZ() {
    auto* pThread = AfxGetThread();
    return pThread ? pThread->PumpMessage() : FALSE;
}
// Symbol: ?AfxInternetStatusCallback@@YAXPEAX_KK0K@Z
// Mangled param order (PEAX, _K, K, 0=PEAX, K) matches the real WinInet
// INTERNET_STATUS_CALLBACK signature exactly: (HINTERNET hInternet,
// DWORD_PTR dwContext, DWORD dwInternetStatus, LPVOID lpvStatusInformation,
// DWORD dwStatusInformationLength). Forward straight to the existing
// implementation in InetcoreSupport.h.
extern "C" void MS_ABI impl__AfxInternetStatusCallback__YAXPEAX_KK0K_Z(void* p0, unsigned __int64 p1, unsigned long p2, void* p3, unsigned long p4) {
    openmfc::detail::inetcore::WinInetStatusCallback(
        reinterpret_cast<HINTERNET>(p0),
        static_cast<DWORD_PTR>(p1),
        static_cast<DWORD>(p2),
        p3,
        static_cast<DWORD>(p4));
}
// Symbol: ?AfxIsIdleMessage@@YAHPEAUtagMSG@@@Z
extern "C" int MS_ABI impl__AfxIsIdleMessage__YAHPEAUtagMSG___Z(void* /*struct*/* p0) {
    auto* pThread = AfxGetThread();
    return pThread ? pThread->IsIdleMessage(reinterpret_cast<MSG*>(p0)) : FALSE;
}
// Symbol: ?AfxIsMFCToolBar@@YAHPEAVCWnd@@@Z
extern "C" int MS_ABI impl__AfxIsMFCToolBar__YAHPEAVCWnd___Z(void* /*class*/* p0) {
    // pWnd != nullptr && pWnd->IsKindOf(RUNTIME_CLASS(CMFCToolBar)).
    // afxmfc.h (which declares CMFCToolBar and its "classCMFCToolBar" RTTI
    // descriptor) is not part of this file's include set, and adding it (or
    // a forward extern "C" declaration of the impl__IsKindOf_CObject__.../
    // impl__GetThisClass_CMFCToolBar__... thunks other files use for this
    // same check, e.g. CMFCOutlookBarPaneButton.cpp) would need a new
    // file-scope declaration, which this shard avoids. GCC also rejects a
    // block-scope `extern "C"` forward declaration outright (verified: hard
    // syntax error), so that route is not available either.
    // CObject::IsKindOf()/CRuntimeClass::IsDerivedFrom() are already fully
    // inline in afx.h (already included via afxwin.h), so instead walk the
    // live CRuntimeClass chain from the window's actual GetRuntimeClass()
    // and match by class name -- functionally equivalent to IsKindOf(),
    // without needing any new declaration.
    CWnd* pWnd = reinterpret_cast<CWnd*>(p0);
    if (pWnd == nullptr) return 0;
    for (const CRuntimeClass* pClass = pWnd->GetRuntimeClass(); pClass != nullptr;
         pClass = pClass->m_pfnGetBaseClass ? pClass->m_pfnGetBaseClass() : nullptr) {
        if (pClass->m_lpszClassName != nullptr &&
            std::strcmp(pClass->m_lpszClassName, "CMFCToolBar") == 0) {
            return 1;
        }
    }
    return 0;
}
// Symbol: ?AfxIsModuleDll@@YAHXZ
extern "C" int MS_ABI impl__AfxIsModuleDll__YAHXZ() {
    HMODULE hMod = nullptr;
    if (!::GetModuleHandleExW(
            GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
            reinterpret_cast<LPCWSTR>(&impl__AfxIsModuleDll__YAHXZ),
            &hMod) || hMod == nullptr) {
        return 0;
    }
    auto* base = reinterpret_cast<BYTE*>(hMod);
    auto* dos = reinterpret_cast<IMAGE_DOS_HEADER*>(base);
    if (dos->e_magic != IMAGE_DOS_SIGNATURE) return 0;
    auto* nt = reinterpret_cast<IMAGE_NT_HEADERS*>(base + dos->e_lfanew);
    if (nt->Signature != IMAGE_NT_SIGNATURE) return 0;
    return (nt->FileHeader.Characteristics & IMAGE_FILE_DLL) ? 1 : 0;
}
// Symbol: ?AfxLoadField@@YAXAEAVCRecordset@@IPEAXPEA_J@Z
// void AFXAPI AfxLoadField(CRecordset& rs, UINT nField, void* pvField, long* plLength)
// Restores field nField from the recordset's RFX value cache -- the "double
// buffer" that AfxStoreField fills when Edit() starts and that CancelUpdate()
// replays. The generated parameter list is already right: AEAVCRecordset@@ is
// one pointer slot, so p0/p1/p2/p3 are rs, nField, pvField, plLength.
//
// Decoded from the real mfc140u.dll (x64 redist 14.51, ordinal 2257):
//     i    = nField - 1;                  // status/cache arrays are 0-based
//     info = &rs.m_pFieldInfo[i];         // rs+0x58, 24-byte entries:
//                                         //   +0x00 void*  m_pvDataCache
//                                         //   +0x08 SQLLEN m_nLength
//                                         //   +0x10 int    m_nDataType
//                                         //   +0x14 BYTE   m_bStatus
//     rs.m_pbFieldFlags[i] |= info->m_bStatus;          // rs+0x178
//     if (rs.m_pbFieldFlags[i] & 0x02) { *plLength = -1; return; }  // SQL_NULL_DATA
//     src = (info->m_nDataType is 1..4 or 6)            // small scalars are stored
//               ? (void*)info                           // inline in the cache slot
//               : info->m_pvDataCache;                  // others in a heap buffer
//     AfxCopyValue(src, pvField, plLength, info->m_nDataType);
//
// Only the NULL half is reproducible here. This codebase's CRecordset
// (include/openmfc/afxdb.h) is 216 bytes with m_nFields at +0x38 -- it has no
// m_pFieldInfo cache array and no m_pbFieldFlags byte array, and the real
// offsets 0x58/0x178 lie outside the object. The side table that stands in for
// them (RecordsetState in phase4/src/detail/CRecordsetSupport.h) keeps only
// dirty/null index sets plus SQLLEN lengths: no per-field data type, no saved
// bytes. Nothing would ever populate a cache either, since every RFX_* entry
// point in phase4/src/core/db/CFieldExchange.cpp is an empty stub. So the value
// copy stays unimplemented until CRecordset grows a real per-field cache
// (m_pvDataCache / m_nLength / m_nDataType / m_bStatus).
//
// Without a saved m_bStatus the |= restore is a no-op, so the current field
// status is tested instead: when the field is already NULL the observable
// behaviour (report SQL_NULL_DATA, copy nothing) matches real MFC exactly.
extern "C" void MS_ABI impl__AfxLoadField__YAXAEAVCRecordset__IPEAXPEA_J_Z(void* /*class*/* p0, unsigned int p1, void* p2, __int64* p3) {
    (void)p2;  // no value cache to copy from -- see above
    if (!p0 || p1 < 1) return;
    if (impl__GetFieldStatus_CRecordset__QEAAEK_Z(p0, (unsigned long)(p1 - 1)) & g5_kFieldFlagNull) {
        if (p3) *p3 = -1;  // SQL_NULL_DATA
    }
}
// Symbol: ?AfxLoadLangResourceDLL@@YAPEAUHINSTANCE__@@PEB_W0@Z
extern "C" void* MS_ABI impl__AfxLoadLangResourceDLL__YAPEAUHINSTANCE____PEB_W0_Z(const wchar_t* p0, const wchar_t* p1) {
    (void)p1;
    return p0 ? ::LoadLibraryExW(p0, nullptr, LOAD_LIBRARY_AS_DATAFILE) : nullptr;
}
// Symbol: ?AfxLoadLangResourceDLL@@YAPEAUHINSTANCE__@@PEB_W@Z
extern "C" void* MS_ABI impl__AfxLoadLangResourceDLL__YAPEAUHINSTANCE____PEB_W_Z(const wchar_t* p0) {
    return p0 ? ::LoadLibraryExW(p0, nullptr, LOAD_LIBRARY_AS_DATAFILE) : nullptr;
}
// Symbol: ?AfxLoadString@@YAHIPEADI@Z
extern "C" int MS_ABI impl__AfxLoadString__YAHIPEADI_Z(unsigned int nID, char* lpszBuf, unsigned int nMaxBuf) {
    return ::LoadStringA(AfxGetResourceHandle(), nID, lpszBuf, nMaxBuf);
}
// Symbol: ?AfxLoadString@@YAHIPEA_WI@Z
extern "C" int MS_ABI impl__AfxLoadString__YAHIPEA_WI_Z(unsigned int nID, wchar_t* lpszBuf, unsigned int nMaxBuf) {
    return ::LoadStringW(AfxGetResourceHandle(), nID, lpszBuf, nMaxBuf);
}
// Symbol: ?AfxLoadSysColorBitmap@@YAPEAUHBITMAP__@@PEAUHINSTANCE__@@PEAUHRSRC__@@H@Z
extern "C" void* MS_ABI impl__AfxLoadSysColorBitmap__YAPEAUHBITMAP____PEAUHINSTANCE____PEAUHRSRC____H_Z(void* /*struct*/* p0, void* /*struct*/* p1, int p2) {
    // Loads the DIB resource and remaps the standard MFC toolbar 4-entry
    // color table (black/gray/ltgray/white) to the current system button
    // colors, then realizes it via CreateDIBitmap -- the classic
    // AfxLoadSysColorBitmap algorithm. p2 (bMono) is accepted but not used
    // to alter bit depth; the resource's own bit depth/color table size is
    // trusted as authored.
    (void)p2;
    HINSTANCE hInst = reinterpret_cast<HINSTANCE>(p0);
    HRSRC hRsrc = reinterpret_cast<HRSRC>(p1);
    if (hRsrc == nullptr) return nullptr;
    HGLOBAL hGlobal = ::LoadResource(hInst, hRsrc);
    if (hGlobal == nullptr) return nullptr;
    BITMAPINFOHEADER* pHeader = static_cast<BITMAPINFOHEADER*>(::LockResource(hGlobal));
    if (pHeader == nullptr) return nullptr;

    RGBQUAD* pColorTable = reinterpret_cast<RGBQUAD*>(
        reinterpret_cast<unsigned char*>(pHeader) + pHeader->biSize);
    unsigned int nColors = pHeader->biClrUsed != 0
        ? pHeader->biClrUsed
        : (pHeader->biBitCount < 24 ? (1u << pHeader->biBitCount) : 0u);

    struct Remap { COLORREF from; int sysColorIndex; };
    const Remap table[] = {
        { RGB(0, 0, 0), COLOR_BTNTEXT },
        { RGB(128, 128, 128), COLOR_BTNSHADOW },
        { RGB(192, 192, 192), COLOR_BTNFACE },
        { RGB(255, 255, 255), COLOR_BTNHIGHLIGHT },
    };
    for (unsigned int i = 0; i < nColors; ++i) {
        RGBQUAD& q = pColorTable[i];
        COLORREF cr = RGB(q.rgbRed, q.rgbGreen, q.rgbBlue);
        for (const Remap& r : table) {
            if (cr == r.from) {
                COLORREF sys = ::GetSysColor(r.sysColorIndex);
                q.rgbRed = GetRValue(sys);
                q.rgbGreen = GetGValue(sys);
                q.rgbBlue = GetBValue(sys);
                break;
            }
        }
    }

    unsigned char* lpBits = reinterpret_cast<unsigned char*>(pColorTable) + nColors * sizeof(RGBQUAD);
    HDC hDC = ::GetDC(nullptr);
    HBITMAP hBitmap = ::CreateDIBitmap(hDC, pHeader, CBM_INIT, lpBits,
                                        reinterpret_cast<BITMAPINFO*>(pHeader), DIB_RGB_COLORS);
    ::ReleaseDC(nullptr, hDC);
    return hBitmap;
}
// Symbol: ?AfxLockGlobals@@YAXH@Z
// Enters the CRITICAL_SECTION for nLockType (MFC's CRIT_* constants) from
// the GetGlobalLock array declared near the top of this file. An
// out-of-range nLockType is ignored rather than crashing.
extern "C" void MS_ABI impl__AfxLockGlobals__YAXH_Z(int p0) {
    if (CRITICAL_SECTION* pcs = GetGlobalLock(p0)) {
        ::EnterCriticalSection(pcs);
    }
}
// Symbol: ?AfxLockTempMaps@@YAXXZ
extern "C" void MS_ABI impl__AfxLockTempMaps__YAXXZ() {
    // Shares _AFX_THREAD_STATE (detail/CWinAppSupport.h) with
    // AfxUnlockTempMaps below via the publicly declared AfxGetThreadState().
    if (_AFX_THREAD_STATE* pState = AfxGetThreadState()) {
        pState->nTempMapLock++;
    }
}
// Symbol: ?AfxMergeMenus@@YAPEAUHMENU__@@PEAU1@0PEAJHH@Z
// OLE in-place menu merging: insert hMenuSource's top-level popup items into
// hMenuShared at the group boundary given by lpMenuWidths[nIndex] (the sum
// of the preceding group widths), the standard MFC/OLE technique for
// combining a container's and a server's menu groups. Best-effort
// reconstruction from documented OLE in-place-activation menu group
// semantics; not verified byte-for-byte against real mfc140u.dll.
extern "C" void* MS_ABI impl__AfxMergeMenus__YAPEAUHMENU____PEAU1_0PEAJHH_Z(void* /*struct*/* p0, void* /*struct*/* p1, long* p2, int p3, int p4) {
    HMENU hMenuShared = reinterpret_cast<HMENU>(p0);
    HMENU hMenuSource = reinterpret_cast<HMENU>(p1);
    LONG* lpMenuWidths = p2;
    int nIndex = p3;
    BOOL bAdd = p4;

    if (!hMenuShared || !::IsMenu(hMenuShared)) {
        return nullptr;
    }
    if (!hMenuSource || !::IsMenu(hMenuSource) || !lpMenuWidths || nIndex < 0) {
        return hMenuShared;
    }

    UINT nPosition = 0;
    for (int i = 0; i < nIndex; ++i) {
        nPosition += static_cast<UINT>(lpMenuWidths[i]);
    }

    int nSourceCount = ::GetMenuItemCount(hMenuSource);
    if (nSourceCount < 0) nSourceCount = 0;

    int nInserted = 0;
    for (int i = 0; i < nSourceCount; ++i) {
        wchar_t szText[256] = {};
        MENUITEMINFOW mii = {};
        mii.cbSize = sizeof(mii);
        mii.fMask = MIIM_ID | MIIM_SUBMENU | MIIM_STATE | MIIM_TYPE | MIIM_DATA;
        mii.fType = MFT_STRING;
        mii.dwTypeData = szText;
        mii.cch = 255;
        if (!::GetMenuItemInfoW(hMenuSource, static_cast<UINT>(i), TRUE, &mii)) {
            continue;
        }
        if (!::InsertMenuItemW(hMenuShared, nPosition + static_cast<UINT>(nInserted), TRUE, &mii)) {
            continue;
        }
        ++nInserted;
    }

    if (bAdd) {
        lpMenuWidths[nIndex] += nInserted;
    }

    return hMenuShared;
}
// Symbol: ?AfxOleCanExitApp@@YAHXZ
extern "C" int MS_ABI impl__AfxOleCanExitApp__YAHXZ() {
    return impl__AfxOleCanExitApp();
}
// Symbol: ?AfxOleGetUserCtrl@@YAHXZ
extern "C" int MS_ABI impl__AfxOleGetUserCtrl__YAHXZ() {
    return openmfc::detail::olecore::g_bOleUserCtrl;
}
// Symbol: ?AfxOleInit@@YAHXZ
extern "C" int MS_ABI impl__AfxOleInit__YAHXZ() {
    return SUCCEEDED(::OleInitialize(nullptr));
}
// Symbol: ?AfxOleInprocRegisterHelper@@YAHPEAUHKEY__@@0H@Z
// Decoded: int AfxOleInprocRegisterHelper(HKEY hkeyProgID, HKEY hkeyClassID,
//                                          int nRegFlags)
// PEAUHKEY__@@ = HKEY, 0 = back-reference to it, H = int.
// Transcribed from retail mfc140, RVA 0x2638c0: the Insertable keys are
// created first, the ThreadingModel value second, and a failure in the second
// step removes the keys the first one added.
extern "C" int MS_ABI impl__AfxOleInprocRegisterHelper__YAHPEAUHKEY____0H_Z(
    HKEY hkeyProgID, HKEY hkeyClassID, int nRegFlags) {
    const bool bInsertable = (nRegFlags & 0x0001) != 0;  // afxRegInsertable
    BOOL bResult = TRUE;

    if (bInsertable) {
        // Valueless keys under both the ProgID and the CLSID key.
        HKEY hkeySub = nullptr;
        if (::RegCreateKeyExW(hkeyProgID, L"Insertable", 0, nullptr, REG_OPTION_NON_VOLATILE,
                               KEY_ALL_ACCESS, nullptr, &hkeySub, nullptr) == ERROR_SUCCESS) {
            ::RegCloseKey(hkeySub);
            hkeySub = nullptr;
            if (::RegCreateKeyExW(hkeyClassID, L"Insertable", 0, nullptr, REG_OPTION_NON_VOLATILE,
                                   KEY_ALL_ACCESS, nullptr, &hkeySub, nullptr) == ERROR_SUCCESS)
                ::RegCloseKey(hkeySub);
            else
                bResult = FALSE;
        } else {
            bResult = FALSE;
        }
    }

    if (bResult) {
        // afxRegApartmentThreading / afxRegFreeThreading, on the InprocServer32
        // key the class must already have.
        const wchar_t* pszModel = nullptr;
        if ((nRegFlags & 0x0006) == 0x0006)    pszModel = L"Both";
        else if ((nRegFlags & 0x0004) != 0)    pszModel = L"Free";
        else if ((nRegFlags & 0x0002) != 0)    pszModel = L"Apartment";

        if (pszModel != nullptr) {
            HKEY hkeyServer = nullptr;
            if (::RegOpenKeyExW(hkeyClassID, L"InprocServer32", 0, KEY_WRITE,
                                 &hkeyServer) == ERROR_SUCCESS) {
                LONG lRes = ::RegSetValueExW(hkeyServer, L"ThreadingModel", 0, REG_SZ,
                                              reinterpret_cast<const BYTE*>(pszModel),
                                              static_cast<DWORD>((std::wcslen(pszModel) + 1) * sizeof(wchar_t)));
                ::RegCloseKey(hkeyServer);
                if (lRes != ERROR_SUCCESS) bResult = FALSE;
            } else if (!g2_ModuleIsDll(AfxGetInstanceHandle())) {
                // No inproc server and none expected: mfc140 checks
                // AfxGetModuleState()->m_bDLL here (RVA 0x263a64) and reports
                // success for an EXE module.
                return TRUE;
            } else {
                bResult = FALSE;
            }
        }
    }

    if (!bResult && bInsertable) {
        // Undo the Insertable keys, as the retail code does at RVA 0x263a7f.
        ::RegDeleteKeyW(hkeyClassID, L"Insertable");
        ::RegDeleteKeyW(hkeyProgID, L"Insertable");
    }
    return bResult;
}
// Symbol: ?AfxOleLockApp@@YAXXZ
extern "C" void MS_ABI impl__AfxOleLockApp__YAXXZ() {
    impl__AfxOleLockApp();
}
// Symbol: ?AfxOleLockControl@@YAHAEBU_GUID@@@Z
extern "C" int MS_ABI impl__AfxOleLockControl__YAHAEBU_GUID___Z(const void* /*struct*/* p0) {
    if (!p0) return FALSE;
    const CLSID& clsid = *reinterpret_cast<const CLSID*>(p0);
    IClassFactory* pcf = nullptr;
    HRESULT hr = ::CoGetClassObject(clsid, CLSCTX_INPROC_SERVER, nullptr, IID_IClassFactory,
                                     reinterpret_cast<void**>(&pcf));
    if (FAILED(hr) || !pcf) return FALSE;
    hr = pcf->LockServer(TRUE);
    pcf->Release();
    return SUCCEEDED(hr) ? TRUE : FALSE;
}
// Symbol: ?AfxOleLockControl@@YAHPEB_W@Z
extern "C" int MS_ABI impl__AfxOleLockControl__YAHPEB_W_Z(const wchar_t* p0) {
    if (!p0) return FALSE;
    CLSID clsid;
    if (FAILED(::CLSIDFromProgID(p0, &clsid))) return FALSE;
    return impl__AfxOleLockControl__YAHAEBU_GUID___Z(reinterpret_cast<const void**>(&clsid));
}
// Symbol: ?AfxOleOnReleaseAllObjects@@YAXXZ
extern "C" void MS_ABI impl__AfxOleOnReleaseAllObjects__YAXXZ() {
    // Called when AfxOleUnlockApp drops the OLE object count to zero (see
    // AfxOleLockApp/AfxOleUnlockApp and detail/OlecoreSupport.cpp's
    // g_nOleLockCount): terminate the application unless the user has taken
    // control of it, which is what the AfxOleSetUserCtrl/AfxOleGetUserCtrl
    // flag records.
    if (openmfc::detail::olecore::g_bOleUserCtrl) return;
    CWinThread* pThread = AfxGetThread();
    if (!pThread) return;
    CWnd* pMainWnd = pThread->m_pMainWnd;
    if (pMainWnd == nullptr || pMainWnd->m_hWnd == nullptr) {
        // In-process server with no main window: there is no application to
        // shut down, and the host unloads us through AfxOleCanExitApp
        // instead. Nothing to do.
        return;
    }
    if (::IsWindow(pMainWnd->m_hWnd)) {
        ::PostMessageW(pMainWnd->m_hWnd, WM_CLOSE, 0, 0);
    }
    // Safe to call twice and before any object was ever locked: a second
    // WM_CLOSE on an already-destroyed window is filtered by the IsWindow
    // check above.
}
// Symbol: ?AfxOleRegisterControlClass@@YAHPEAUHINSTANCE__@@AEBU_GUID@@PEB_WIIHK1GG@Z
// Decoded: int AfxOleRegisterControlClass(HINSTANCE hInstance, REFCLSID clsid,
//     LPCTSTR pszProgID, UINT idTypeName, UINT idBitmap, int nRegFlags,
//     DWORD dwMiscStatus, REFGUID tlid, WORD wVerMajor, WORD wVerMinor)
// The generated list had the right arity; the two "const void**" slots are
// AEBU_GUID@@ / its back-reference 1, i.e. one pointer each.
// Retail body at RVA 0x1f22d0: it creates the ProgID and CLSID\{clsid} keys
// and then runs two registrar tables against them with bReplace=TRUE, before
// handing both keys to AfxOleInprocRegisterHelper.
extern "C" int MS_ABI impl__AfxOleRegisterControlClass__YAHPEAUHINSTANCE____AEBU_GUID__PEB_WIIHK1GG_Z(
    HINSTANCE hInstance, const GUID* pclsid, const wchar_t* pszProgID,
    unsigned int idTypeName, unsigned int idBitmap, int nRegFlags,
    unsigned long dwMiscStatus, const GUID* ptlid,
    unsigned short wVerMajor, unsigned short wVerMinor) {
    if (pclsid == nullptr || ptlid == nullptr || pszProgID == nullptr)
        return FALSE;

    wchar_t szClsid[64] = {};
    wchar_t szTlid[64] = {};
    if (::StringFromGUID2(*pclsid, szClsid, 64) == 0) return FALSE;
    if (::StringFromGUID2(*ptlid, szTlid, 64) == 0) return FALSE;

    // The control's display name comes from the string resource; with no
    // usable resource the CLSID text is what ends up in the registry.
    wchar_t szTypeName[256] = {};
    if (idTypeName != 0) {
        ::LoadStringW(hInstance ? hInstance : AfxGetResourceHandle(), idTypeName, szTypeName, 256);
    }
    const wchar_t* pszTypeName = szTypeName[0] ? szTypeName : szClsid;

    // %3 here is the *unquoted* module path, unlike the server-class tables.
    wchar_t szModule[MAX_PATH] = {};
    ::GetModuleFileNameW(hInstance, szModule, MAX_PATH);
    wchar_t szBitmap[16] = {};
    wchar_t szMisc[16] = {};
    wchar_t szVersion[24] = {};
    std::swprintf(szBitmap, 16, L"%u", idBitmap);
    std::swprintf(szMisc, 16, L"%lu", dwMiscStatus);
    std::swprintf(szVersion, 24, L"%u.%u", (unsigned)wVerMajor, (unsigned)wVerMinor);

    bool bClose = false;
    HKEY hRoot = g2_OleRegRoot(HKEY_CLASSES_ROOT, &bClose);

    g2_Str strClsidKey;
    strClsidKey.Add(L"CLSID\\");
    strClsidKey.Add(szClsid);

    BOOL bResult = FALSE;
    HKEY hkeyClsid = nullptr;
    HKEY hkeyProgID = nullptr;
    if (::RegCreateKeyExW(hRoot, strClsidKey.sz, 0, nullptr, REG_OPTION_NON_VOLATILE,
                           KEY_ALL_ACCESS, nullptr, &hkeyClsid, nullptr) == ERROR_SUCCESS &&
        ::RegCreateKeyExW(hRoot, pszProgID, 0, nullptr, REG_OPTION_NON_VOLATILE,
                           KEY_ALL_ACCESS, nullptr, &hkeyProgID, nullptr) == ERROR_SUCCESS) {
        const wchar_t* rgszProgID[2];
        rgszProgID[0] = pszTypeName;
        rgszProgID[1] = szClsid;
        const wchar_t* rgszClsid[7];
        rgszClsid[0] = pszTypeName;
        rgszClsid[1] = pszProgID;
        rgszClsid[2] = szModule;
        rgszClsid[3] = szBitmap;
        rgszClsid[4] = szMisc;
        rgszClsid[5] = szTlid;
        rgszClsid[6] = szVersion;

        if (impl__AfxOleRegisterHelper__YAHPEBQEB_W0HHPEAUHKEY_____Z(
                g2_rgCtlProgID, rgszProgID, 2, TRUE, hkeyProgID) &&
            impl__AfxOleRegisterHelper__YAHPEBQEB_W0HHPEAUHKEY_____Z(
                g2_rgCtlClsid, rgszClsid, 7, TRUE, hkeyClsid)) {
            // Insertable / ThreadingModel are applied by the inproc helper, on
            // the two keys this function has just created.
            bResult = impl__AfxOleInprocRegisterHelper__YAHPEAUHKEY____0H_Z(
                hkeyProgID, hkeyClsid, nRegFlags);
        }
    }
    if (hkeyProgID != nullptr) ::RegCloseKey(hkeyProgID);
    if (hkeyClsid != nullptr) ::RegCloseKey(hkeyClsid);
    if (bClose) ::RegCloseKey(hRoot);
    return bResult;
}
// Symbol: ?AfxOleRegisterHelper@@YAHPEBQEB_W0HHPEAUHKEY__@@@Z
// Decoded: int AfxOleRegisterHelper(LPCTSTR const* rglpszRegister,
//     LPCTSTR const* rglpszSymbols, int nSymbols, BOOL bReplace, HKEY hKeyRoot)
// PEBQEB_W = wchar_t const* const* (one pointer), 0 = back-reference to it,
// H H = two ints, PEAUHKEY__@@ = HKEY. The generated 8-parameter list was
// wrong: it expanded the two string tables into three slots each.
// Loop transcribed from retail mfc140, RVA 0x263150.
extern "C" int MS_ABI impl__AfxOleRegisterHelper__YAHPEBQEB_W0HHPEAUHKEY_____Z(
    const wchar_t* const* rglpszRegister, const wchar_t* const* rglpszSymbols,
    int nSymbols, int bReplace, HKEY hkeyRoot) {
    if (rglpszRegister == nullptr) return TRUE;

    // An empty key means "the root key's own default value"; mfc140 only
    // skips such an entry when the root is HKEY_CLASSES_ROOT (RVA 0x263238),
    // and the control-class tables rely on that.
    const bool bClassesRoot = (hkeyRoot == HKEY_CLASSES_ROOT || hkeyRoot == nullptr);
    bool bClose = false;
    HKEY hRoot = g2_OleRegRoot(hkeyRoot, &bClose);

    BOOL bResult = TRUE;
    for (; *rglpszRegister != nullptr; ++rglpszRegister) {
        const wchar_t* pszKey = *rglpszRegister;
        if (bClassesRoot && pszKey[0] == L'\0') continue;
        // Second half of the entry's double string; "key\0" gives an empty one.
        const wchar_t* pszValue = pszKey + std::wcslen(pszKey) + 1;
        g2_Str strKey, strValue;
        g2_OleSubst(strKey, pszKey, rglpszSymbols, nSymbols);
        g2_OleSubst(strValue, pszValue, rglpszSymbols, nSymbols);
        if (bClassesRoot && strKey.IsEmpty()) continue;
        if (!bReplace) {
            // Register-only entries are skipped whenever the key already
            // exists, whatever it currently holds (the retail test is an
            // AfxRegQueryValue that succeeded).
            HKEY hExisting = nullptr;
            if (::RegOpenKeyExW(hRoot, strKey.sz, 0, KEY_READ, &hExisting) == ERROR_SUCCESS) {
                ::RegCloseKey(hExisting);
                continue;
            }
        }
        LONG lRes = g2_OleSetKey(hRoot, strKey.sz, strValue.sz);
        if (lRes != ERROR_SUCCESS) {
            // The retail loop stops at the first write failure and still
            // reports success when it was only denied access (RVA 0x263311).
            bResult = (lRes == ERROR_ACCESS_DENIED);
            break;
        }
    }
    if (bClose) ::RegCloseKey(hRoot);
    return bResult;
}
// Symbol: ?AfxOleRegisterPropertyPageClass@@YAHPEAUHINSTANCE__@@AEBU_GUID@@I@Z
extern "C" int MS_ABI impl__AfxOleRegisterPropertyPageClass__YAHPEAUHINSTANCE____AEBU_GUID__I_Z(void* /*struct*/* p0, const void* /*struct*/* p1, unsigned int p2) {
    if (!p1) return FALSE;
    HINSTANCE hInst = reinterpret_cast<HINSTANCE>(p0);
    const CLSID& clsid = *reinterpret_cast<const CLSID*>(p1);

    wchar_t clsidText[64] = {};
    if (::StringFromGUID2(clsid, clsidText, 64) == 0) return FALSE;
    CString clsidKey = CString(L"CLSID\\") + clsidText;

    wchar_t name[256] = {};
    if (p2 != 0) {
        ::LoadStringW(hInst ? hInst : AfxGetResourceHandle(), p2, name, 256);
    }

    wchar_t modulePath[MAX_PATH] = {};
    if (hInst) ::GetModuleFileNameW(hInst, modulePath, MAX_PATH);

    // Mirrors the CLSID/InProcServer32 registration pattern already used by
    // COleObjectFactory::UpdateRegistry (core/ole/COleObjectFactory.cpp).
    BOOL ok = TRUE;
    HKEY hKey = nullptr;
    if (::RegCreateKeyExW(HKEY_CLASSES_ROOT, static_cast<const wchar_t*>(clsidKey), 0, nullptr,
                           REG_OPTION_NON_VOLATILE, KEY_SET_VALUE | KEY_CREATE_SUB_KEY, nullptr,
                           &hKey, nullptr) == ERROR_SUCCESS) {
        if (name[0]) {
            ::RegSetValueExW(hKey, nullptr, 0, REG_SZ, reinterpret_cast<const BYTE*>(name),
                              static_cast<DWORD>((std::wcslen(name) + 1) * sizeof(wchar_t)));
        }
        HKEY hInproc = nullptr;
        if (::RegCreateKeyExW(hKey, L"InProcServer32", 0, nullptr, REG_OPTION_NON_VOLATILE,
                               KEY_SET_VALUE, nullptr, &hInproc, nullptr) == ERROR_SUCCESS) {
            ::RegSetValueExW(hInproc, nullptr, 0, REG_SZ, reinterpret_cast<const BYTE*>(modulePath),
                              static_cast<DWORD>((std::wcslen(modulePath) + 1) * sizeof(wchar_t)));
            ::RegSetValueExW(hInproc, L"ThreadingModel", 0, REG_SZ,
                              reinterpret_cast<const BYTE*>(L"Apartment"), sizeof(L"Apartment"));
            ::RegCloseKey(hInproc);
        } else {
            ok = FALSE;
        }
        ::RegCloseKey(hKey);
    } else {
        ok = FALSE;
    }
    return ok;
}
// Symbol: ?AfxOleRegisterPropertyPageClass@@YAHPEAUHINSTANCE__@@AEBU_GUID@@IH@Z
extern "C" int MS_ABI impl__AfxOleRegisterPropertyPageClass__YAHPEAUHINSTANCE____AEBU_GUID__IH_Z(void* /*struct*/* p0, const void* /*struct*/* p1, unsigned int p2, int p3) {
    if (!p1) return FALSE;
    if (p3) {
        // Register: identical to the 3-argument overload above.
        return impl__AfxOleRegisterPropertyPageClass__YAHPEAUHINSTANCE____AEBU_GUID__I_Z(p0, p1, p2);
    }
    // Unregister: delete the CLSID tree (mirrors
    // COleObjectFactory::UpdateRegistry(FALSE)).
    const CLSID& clsid = *reinterpret_cast<const CLSID*>(p1);
    wchar_t clsidText[64] = {};
    if (::StringFromGUID2(clsid, clsidText, 64) == 0) return FALSE;
    CString clsidKey = CString(L"CLSID\\") + clsidText;
    LONG res = ::RegDeleteTreeW(HKEY_CLASSES_ROOT, static_cast<const wchar_t*>(clsidKey));
    return (res == ERROR_SUCCESS || res == ERROR_FILE_NOT_FOUND) ? TRUE : FALSE;
}
// Symbol: ?AfxOleRegisterServerClass@@YAHAEBU_GUID@@PEB_W11W4OLE_APPTYPE@@PEAPEB_W3H11@Z
// Decoded: int AfxOleRegisterServerClass(REFCLSID clsid, LPCTSTR lpszClassName,
//     LPCTSTR lpszShortTypeName, LPCTSTR lpszLongTypeName, OLE_APPTYPE nAppType,
//     LPCTSTR* rglpszRegister, LPCTSTR* rglpszOverwrite, int nIconIndex,
//     LPCTSTR lpszLocalFilterName, LPCTSTR lpszLocalFilterExt)
// AEBU_GUID@@ = one pointer; PEB_W 1 1 = three LPCTSTR; W4OLE_APPTYPE@@ = enum
// (4 bytes); PEAPEB_W 3 = two LPCTSTR*; H = int; 1 1 = two more LPCTSTR.
// Ten parameters -- the generated 17-parameter list was garbage. Retail body
// at RVA 0x262d60.
extern "C" int MS_ABI impl__AfxOleRegisterServerClass__YAHAEBU_GUID__PEB_W11W4OLE_APPTYPE__PEAPEB_W3H11_Z(
    const GUID* pclsid, const wchar_t* lpszClassName, const wchar_t* lpszShortTypeName,
    const wchar_t* lpszLongTypeName, int nAppType, const wchar_t** rglpszRegister,
    const wchar_t** rglpszOverwrite, int nIconIndex,
    const wchar_t* lpszLocalFilterName, const wchar_t* lpszLocalFilterExt) {
    return g2_OleServerClass(pclsid, lpszClassName, lpszShortTypeName, lpszLongTypeName,
                             nAppType, rglpszRegister, rglpszOverwrite, nIconIndex,
                             lpszLocalFilterName, lpszLocalFilterExt, true);
}
// Symbol: ?AfxOleRegisterServerClass@@YAHAEBU_GUID@@PEB_W11W4OLE_APPTYPE@@PEAPEB_W3H1@Z
// Same as the overload above without lpszLocalFilterExt -- nine parameters.
// %9 then comes from the extension inside lpszLocalFilterName.
extern "C" int MS_ABI impl__AfxOleRegisterServerClass__YAHAEBU_GUID__PEB_W11W4OLE_APPTYPE__PEAPEB_W3H1_Z(
    const GUID* pclsid, const wchar_t* lpszClassName, const wchar_t* lpszShortTypeName,
    const wchar_t* lpszLongTypeName, int nAppType, const wchar_t** rglpszRegister,
    const wchar_t** rglpszOverwrite, int nIconIndex,
    const wchar_t* lpszLocalFilterName) {
    return g2_OleServerClass(pclsid, lpszClassName, lpszShortTypeName, lpszLongTypeName,
                             nAppType, rglpszRegister, rglpszOverwrite, nIconIndex,
                             lpszLocalFilterName, nullptr, true);
}
// Symbol: ?AfxOleRegisterTypeLib@@YAHPEAUHINSTANCE__@@AEBU_GUID@@PEB_W2@Z
extern "C" int MS_ABI impl__AfxOleRegisterTypeLib__YAHPEAUHINSTANCE____AEBU_GUID__PEB_W2_Z(void* /*struct*/* p0, const void* /*struct*/* p1, const wchar_t* p2, const wchar_t* p3) {
    (void)p1; // clsid isn't needed to load/register the type library itself
    HINSTANCE hInst = reinterpret_cast<HINSTANCE>(p0);
    wchar_t szPath[MAX_PATH] = {};
    if (!hInst || ::GetModuleFileNameW(hInst, szPath, MAX_PATH) == 0) return FALSE;
    if (p2 && p2[0]) {
        std::wcsncat(szPath, p2, MAX_PATH - std::wcslen(szPath) - 1);
    }
    ITypeLib* pTypeLib = nullptr;
    HRESULT hr = ::LoadTypeLibEx(szPath, REGKIND_NONE, &pTypeLib);
    if (FAILED(hr) || !pTypeLib) return FALSE;
    hr = ::RegisterTypeLib(pTypeLib, szPath, p3);
    pTypeLib->Release();
    return SUCCEEDED(hr) ? TRUE : FALSE;
}
// Symbol: ?AfxOleSetEditMenu@@YAXPEAVCOleClientItem@@PEAVCMenu@@IIII@Z
extern "C" void MS_ABI impl__AfxOleSetEditMenu__YAXPEAVCOleClientItem__PEAVCMenu__IIII_Z(void* /*class*/* p0, void* /*class*/* p1, unsigned int p2, unsigned int p3, unsigned int p4, unsigned int p5) {
    // Forward to the existing internal helper of the same shape
    // (detail/OlecoreSupport.h/.cpp); it is itself currently a stub that
    // does nothing, so this wiring is a no-op today but will pick up real
    // behavior automatically once that helper is implemented.
    impl__AfxOleSetEditMenu(reinterpret_cast<COleClientItem*>(p0), reinterpret_cast<CMenu*>(p1), p2, p3, p4, p5);
}
// Symbol: ?AfxOleSetUserCtrl@@YAXH@Z
extern "C" void MS_ABI impl__AfxOleSetUserCtrl__YAXH_Z(int p0) {
    openmfc::detail::olecore::g_bOleUserCtrl = p0 ? TRUE : FALSE;
}
// Symbol: ?AfxOleTerm@@YAXH@Z
extern "C" void MS_ABI impl__AfxOleTerm__YAXH_Z(int p0) {
    (void)p0;
    ::OleUninitialize();
}
// Symbol: ?AfxOleTermOrFreeLib@@YAXHH@Z
extern "C" void MS_ABI impl__AfxOleTermOrFreeLib__YAXHH_Z(int p0, int p1) {
    (void)p0;
    (void)p1;
    ::OleUninitialize();
}
// Symbol: ?AfxOleUnlockAllControls@@YAXXZ
extern "C" void MS_ABI impl__AfxOleUnlockAllControls__YAXXZ() {
    // Real MFC walks the module's list of controls locked by
    // AfxOleLockControl and calls IClassFactory::LockServer(FALSE) on each.
    // NOT POSSIBLE HERE: both AfxOleLockControl overloads (above in this
    // file) resolve the CLSID, call CoGetClassObject + LockServer(TRUE),
    // Release the factory and record nothing -- there is no per-module
    // locked-control list to walk. Grepped detail/OlecoreSupport.{h,cpp}
    // for one: it tracks only g_nOleLockCount (the AfxOleLockApp object
    // count, a different counter) and g_oleObjectFactories (this module's own
    // COleObjectFactory registrations, not foreign controls we locked).
    // Implementing this needs AfxOleLockControl/AfxOleUnlockControl to keep
    // the lock list; blindly unlocking anything else would drop refs this
    // module never took. Left empty; see the report.
}
// Symbol: ?AfxOleUnlockApp@@YAXXZ
extern "C" void MS_ABI impl__AfxOleUnlockApp__YAXXZ() {
    impl__AfxOleUnlockApp();
}
// Symbol: ?AfxOleUnlockControl@@YAHAEBU_GUID@@@Z
extern "C" int MS_ABI impl__AfxOleUnlockControl__YAHAEBU_GUID___Z(const void* /*struct*/* p0) {
    if (!p0) return FALSE;
    const CLSID& clsid = *reinterpret_cast<const CLSID*>(p0);
    IClassFactory* pcf = nullptr;
    HRESULT hr = ::CoGetClassObject(clsid, CLSCTX_INPROC_SERVER, nullptr, IID_IClassFactory,
                                     reinterpret_cast<void**>(&pcf));
    if (FAILED(hr) || !pcf) return FALSE;
    hr = pcf->LockServer(FALSE);
    pcf->Release();
    return SUCCEEDED(hr) ? TRUE : FALSE;
}
// Symbol: ?AfxOleUnlockControl@@YAHPEB_W@Z
extern "C" int MS_ABI impl__AfxOleUnlockControl__YAHPEB_W_Z(const wchar_t* p0) {
    if (!p0) return FALSE;
    CLSID clsid;
    if (FAILED(::CLSIDFromProgID(p0, &clsid))) return FALSE;
    return impl__AfxOleUnlockControl__YAHAEBU_GUID___Z(reinterpret_cast<const void**>(&clsid));
}
// Symbol: ?AfxOleUnregisterClass@@YAHAEBU_GUID@@PEB_W@Z
extern "C" int MS_ABI impl__AfxOleUnregisterClass__YAHAEBU_GUID__PEB_W_Z(const void* /*struct*/* p0, const wchar_t* p1) {
    if (!p0) return FALSE;
    const CLSID& clsid = *reinterpret_cast<const CLSID*>(p0);
    wchar_t clsidText[64] = {};
    if (::StringFromGUID2(clsid, clsidText, 64) == 0) return FALSE;
    CString clsidKey = CString(L"CLSID\\") + clsidText;
    // Mirrors COleObjectFactory::UpdateRegistry(FALSE) in
    // core/ole/COleObjectFactory.cpp.
    LONG clsidResult = ::RegDeleteTreeW(HKEY_CLASSES_ROOT, static_cast<const wchar_t*>(clsidKey));
    LONG progResult = ERROR_SUCCESS;
    if (p1 && p1[0]) {
        progResult = ::RegDeleteTreeW(HKEY_CLASSES_ROOT, p1);
    }
    bool clsidOk = (clsidResult == ERROR_SUCCESS || clsidResult == ERROR_FILE_NOT_FOUND);
    bool progOk = (progResult == ERROR_SUCCESS || progResult == ERROR_FILE_NOT_FOUND);
    return (clsidOk && progOk) ? TRUE : FALSE;
}
// Symbol: ?AfxOleUnregisterHelper@@YAHPEBQEB_W0HPEAUHKEY__@@@Z
// Decoded: int AfxOleUnregisterHelper(LPCTSTR const* rglpszRegister,
//     LPCTSTR const* rglpszSymbols, int nSymbols, HKEY hKeyRoot)
// Same table as AfxOleRegisterHelper, one int fewer (no bReplace).
// Retail loop at RVA 0x262fe0: only the key half is substituted, each key is
// deleted along with the parents it leaves empty, and the result is always
// TRUE.
extern "C" int MS_ABI impl__AfxOleUnregisterHelper__YAHPEBQEB_W0HPEAUHKEY_____Z(
    const wchar_t* const* rglpszRegister, const wchar_t* const* rglpszSymbols,
    int nSymbols, HKEY hkeyRoot) {
    if (rglpszRegister == nullptr) return TRUE;

    // NOTE: mfc140 ignores its hKeyRoot argument here -- its delete worker
    // hardcodes HKEY_CLASSES_ROOT (RVA 0x220c75). Every real caller passes
    // either nothing or HKEY_CLASSES_ROOT, so honoring the argument is a
    // superset.
    const bool bClassesRoot = (hkeyRoot == HKEY_CLASSES_ROOT || hkeyRoot == nullptr);
    bool bClose = false;
    HKEY hRoot = g2_OleRegRoot(hkeyRoot, &bClose);

    for (; *rglpszRegister != nullptr; ++rglpszRegister) {
        if (bClassesRoot && (*rglpszRegister)[0] == L'\0') continue;
        g2_Str strKey;
        g2_OleSubst(strKey, *rglpszRegister, rglpszSymbols, nSymbols);
        if (!strKey.IsEmpty())
            g2_OleDeleteKey(hRoot, strKey.sz);
    }
    if (bClose) ::RegCloseKey(hRoot);
    // Keys that still hold subkeys of their own are left alone, and that is
    // not reported as a failure.
    return TRUE;
}
// Symbol: ?AfxOleUnregisterServerClass@@YAHAEBU_GUID@@PEB_W11W4OLE_APPTYPE@@PEAPEB_W3@Z
// Decoded: int AfxOleUnregisterServerClass(REFCLSID clsid, LPCTSTR lpszClassName,
//     LPCTSTR lpszShortTypeName, LPCTSTR lpszLongTypeName, OLE_APPTYPE nAppType,
//     LPCTSTR* rglpszRegister, LPCTSTR* rglpszOverwrite)
// Seven parameters -- same prefix as AfxOleRegisterServerClass, stopping after
// the two table pointers. Retail body at RVA 0x262ea0: it builds the same
// symbols with nIconIndex 0 and no filter name or extension, and rejects an
// OLE_APPTYPE outside 0..4.
extern "C" int MS_ABI impl__AfxOleUnregisterServerClass__YAHAEBU_GUID__PEB_W11W4OLE_APPTYPE__PEAPEB_W3_Z(
    const GUID* pclsid, const wchar_t* lpszClassName, const wchar_t* lpszShortTypeName,
    const wchar_t* lpszLongTypeName, int nAppType, const wchar_t** rglpszRegister,
    const wchar_t** rglpszOverwrite) {
    return g2_OleServerClass(pclsid, lpszClassName, lpszShortTypeName, lpszLongTypeName,
                             nAppType, rglpszRegister, rglpszOverwrite, 0,
                             nullptr, nullptr, false);
}
// Symbol: ?AfxOleUnregisterTypeLib@@YAHAEBU_GUID@@GGK@Z
extern "C" int MS_ABI impl__AfxOleUnregisterTypeLib__YAHAEBU_GUID__GGK_Z(const void* /*struct*/* p0, unsigned short p1, unsigned short p2, unsigned long p3) {
    if (!p0) return FALSE;
    const GUID& libid = *reinterpret_cast<const GUID*>(p0);
#ifdef _WIN64
    HRESULT hr = ::UnRegisterTypeLib(libid, p1, p2, static_cast<LCID>(p3), SYS_WIN64);
#else
    HRESULT hr = ::UnRegisterTypeLib(libid, p1, p2, static_cast<LCID>(p3), SYS_WIN32);
#endif
    return SUCCEEDED(hr) ? TRUE : FALSE;
}
// Symbol: ?AfxParseURL@@YAHPEB_WAEAKAEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@2AEAG@Z
// Real signature: int AfxParseURL(LPCWSTR pstrURL, DWORD& dwServiceType,
//                                 CString& strServer, CString& strObject, INTERNET_PORT& nPort)
// -- 5 real (pointer-sized) arguments, so by the x64 calling convention they occupy
// ABI slots RCX, RDX, R8, R9, [stack+0] in that order.
//
// tools/gen_manual_thunks.py (which produced this file) does NOT collapse a
// CStringT<> reference parameter into a single pointer the way tools/gen_thunks.py
// does for class methods; instead the demangler's naive per-param walk expands the
// FIRST CStringT<> reference it meets into a bogus (void*, void**, void*) triplet
// (three declared C params for one real ABI slot) while a *repeated* CStringT<>
// reference reached via an MSVC back-reference ("2") is emitted as a single
// (correctly-typed) pointer param. The net effect: every real argument that comes
// textually after the expanded CStringT<> triplet lands in a thunk parameter whose
// declared name/type no longer matches its real meaning -- p3/p4 below are real,
// mislabeled data; p5/p6 are pure uninitialized-stack artifacts that must never be
// touched (the real call only supplies 5 arguments).
//
//   slot1 p0 (const wchar_t*)      = pstrURL                              [correct label]
//   slot2 p1 (unsigned long*)      = &dwServiceType                       [correct label]
//   slot3 p2 (void* /*class*/*)    = &strServer   (CString*)              [correct label]
//   slot4 p3 (void**)              = &strObject   (CString*, mislabeled)
//   slot5 p4 (void*)               = &nPort       (unsigned short*, mislabeled, 1st stack arg)
//   slot6 p5 (void* /*class*/*)    = GARBAGE -- caller never supplied a 6th argument
//   slot7 p6 (unsigned short*)     = GARBAGE -- caller never supplied a 7th argument
extern "C" int MS_ABI impl__AfxParseURL__YAHPEB_WAEAKAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__2AEAG_Z(const wchar_t* p0, unsigned long* p1, void* /*class*/* p2, void** p3, void* p4, void* /*class*/* p5, unsigned short* p6) {
    (void)p5; (void)p6; // artifacts: never read (uninitialized stack, no backing argument)

    const wchar_t* pstrURL = p0;
    unsigned long* pdwServiceType = p1;
    CString* pStrServer = reinterpret_cast<CString*>(p2);
    CString* pStrObject = reinterpret_cast<CString*>(p3);          // real, despite void** label
    unsigned short* pnPort = reinterpret_cast<unsigned short*>(p4); // real, despite void* label

    if (pdwServiceType) *pdwServiceType = 0;
    if (pStrServer) pStrServer->Empty();
    if (pStrObject) pStrObject->Empty();
    if (pnPort) *pnPort = 0;
    if (!pstrURL || !*pstrURL) return 0;

    // Resolve InternetCrackUrlW dynamically: a block-scope `extern "C"` declaration
    // is not legal C++ (linkage-specifications are namespace-scope only), and adding
    // a file-scope declaration/#include is outside this shard's constraints.
    struct LocalUrlComponentsW {
        unsigned long dwStructSize;
        wchar_t* lpszScheme;
        unsigned long dwSchemeLength;
        int nScheme;
        wchar_t* lpszHostName;
        unsigned long dwHostNameLength;
        unsigned short nPort;
        wchar_t* lpszUserName;
        unsigned long dwUserNameLength;
        wchar_t* lpszPassword;
        unsigned long dwPasswordLength;
        wchar_t* lpszUrlPath;
        unsigned long dwUrlPathLength;
        wchar_t* lpszExtraInfo;
        unsigned long dwExtraInfoLength;
    };
    typedef int (WINAPI *PFN_InternetCrackUrlW)(const wchar_t*, unsigned long, unsigned long, LocalUrlComponentsW*);
    static PFN_InternetCrackUrlW s_pfnInternetCrackUrlW = nullptr;
    if (!s_pfnInternetCrackUrlW) {
        HMODULE hWinInet = ::LoadLibraryW(L"wininet.dll");
        if (hWinInet) {
            s_pfnInternetCrackUrlW = reinterpret_cast<PFN_InternetCrackUrlW>(
                ::GetProcAddress(hWinInet, "InternetCrackUrlW"));
        }
    }
    if (!s_pfnInternetCrackUrlW) return 0;

    wchar_t scheme[32] = {};
    wchar_t host[256] = {};
    wchar_t path[2048] = {};
    LocalUrlComponentsW uc = {};
    uc.dwStructSize = sizeof(uc);
    uc.lpszScheme = scheme; uc.dwSchemeLength = 32;
    uc.lpszHostName = host; uc.dwHostNameLength = 256;
    uc.lpszUrlPath = path; uc.dwUrlPathLength = 2048;

    if (!s_pfnInternetCrackUrlW(pstrURL, 0, 0, &uc)) return 0;

    // INTERNET_SCHEME values (from wininet.h): FTP=1, GOPHER=2, HTTP=3, HTTPS=4, FILE=5.
    // AFX_INET_SERVICE_* values used here follow the long-standing classic-MFC
    // enum (FTP=1, GOPHER=2, HTTP=3); this repo does not yet declare that enum
    // anywhere, and these exact values could not be cross-checked against a
    // harvested real mfc140u.dll in this session.
    unsigned long dwServiceType;
    switch (uc.nScheme) {
        case 1: dwServiceType = 1; break;             // FTP
        case 2: dwServiceType = 2; break;              // GOPHER
        case 3: case 4: dwServiceType = 3; break;      // HTTP / HTTPS
        default: return 0;                             // unsupported scheme (e.g. FILE)
    }

    if (pdwServiceType) *pdwServiceType = dwServiceType;
    if (pStrServer) *pStrServer = host;
    if (pStrObject) *pStrObject = (path[0] != L'\0') ? path : L"/";
    if (pnPort) *pnPort = uc.nPort;
    return 1;
}
// Symbol: ?AfxParseURLEx@@YAHPEB_WAEAKAEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@2AEAG22K@Z
// Real signature: int AfxParseURLEx(LPCWSTR pstrURL, DWORD& dwServiceType,
//                                   CString& strServer, CString& strObject, INTERNET_PORT& nPort,
//                                   CString& strUsername, CString& strPassword, DWORD dwFlags)
// -- 8 real arguments; slots RCX,RDX,R8,R9,[stack0..3]. Same demangler-artifact
// shift as AfxParseURL above (see the comment there for the full explanation), but
// this time TWO subsequent CStringT<> back-references and a trailing DWORD also
// land in mislabeled slots:
//   slot1 p0 = pstrURL                                  [correct label]
//   slot2 p1 = &dwServiceType                            [correct label]
//   slot3 p2 = &strServer   (CString*)                   [correct label]
//   slot4 p3 = &strObject   (CString*, mislabeled void**)
//   slot5 p4 = &nPort       (unsigned short*, mislabeled void*, 1st stack arg)
//   slot6 p5 = &strUsername (CString*, correctly typed by coincidence)
//   slot7 p6 = &strPassword (CString*, mislabeled unsigned short*)
//   slot8 p7 = dwFlags      (unsigned long, mislabeled void* /*class*/*; low 32 bits are valid)
//   slot9 p8, slot10 p9 = GARBAGE -- caller only supplied 8 arguments; never touch.
extern "C" int MS_ABI impl__AfxParseURLEx__YAHPEB_WAEAKAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__2AEAG22K_Z(const wchar_t* p0, unsigned long* p1, void* /*class*/* p2, void** p3, void* p4, void* /*class*/* p5, unsigned short* p6, void* /*class*/* p7, void* /*class*/* p8, unsigned long p9) {
    (void)p8; (void)p9; // artifacts: never read (uninitialized stack, no backing argument)

    const wchar_t* pstrURL = p0;
    unsigned long* pdwServiceType = p1;
    CString* pStrServer = reinterpret_cast<CString*>(p2);
    CString* pStrObject = reinterpret_cast<CString*>(p3);            // real, despite void** label
    unsigned short* pnPort = reinterpret_cast<unsigned short*>(p4);   // real, despite void* label
    CString* pStrUsername = reinterpret_cast<CString*>(p5);
    CString* pStrPassword = reinterpret_cast<CString*>(p6);           // real, despite unsigned short* label
    unsigned long dwFlags = static_cast<unsigned long>(reinterpret_cast<uintptr_t>(p7)); // low 32 bits valid

    if (pdwServiceType) *pdwServiceType = 0;
    if (pStrServer) pStrServer->Empty();
    if (pStrObject) pStrObject->Empty();
    if (pnPort) *pnPort = 0;
    if (pStrUsername) pStrUsername->Empty();
    if (pStrPassword) pStrPassword->Empty();
    if (!pstrURL || !*pstrURL) return 0;

    struct LocalUrlComponentsW {
        unsigned long dwStructSize;
        wchar_t* lpszScheme;
        unsigned long dwSchemeLength;
        int nScheme;
        wchar_t* lpszHostName;
        unsigned long dwHostNameLength;
        unsigned short nPort;
        wchar_t* lpszUserName;
        unsigned long dwUserNameLength;
        wchar_t* lpszPassword;
        unsigned long dwPasswordLength;
        wchar_t* lpszUrlPath;
        unsigned long dwUrlPathLength;
        wchar_t* lpszExtraInfo;
        unsigned long dwExtraInfoLength;
    };
    typedef int (WINAPI *PFN_InternetCrackUrlW)(const wchar_t*, unsigned long, unsigned long, LocalUrlComponentsW*);
    static PFN_InternetCrackUrlW s_pfnInternetCrackUrlW = nullptr;
    if (!s_pfnInternetCrackUrlW) {
        HMODULE hWinInet = ::LoadLibraryW(L"wininet.dll");
        if (hWinInet) {
            s_pfnInternetCrackUrlW = reinterpret_cast<PFN_InternetCrackUrlW>(
                ::GetProcAddress(hWinInet, "InternetCrackUrlW"));
        }
    }
    if (!s_pfnInternetCrackUrlW) return 0;

    wchar_t scheme[32] = {};
    wchar_t host[256] = {};
    wchar_t path[2048] = {};
    wchar_t user[256] = {};
    wchar_t pass[256] = {};
    LocalUrlComponentsW uc = {};
    uc.dwStructSize = sizeof(uc);
    uc.lpszScheme = scheme; uc.dwSchemeLength = 32;
    uc.lpszHostName = host; uc.dwHostNameLength = 256;
    uc.lpszUrlPath = path; uc.dwUrlPathLength = 2048;
    uc.lpszUserName = user; uc.dwUserNameLength = 256;
    uc.lpszPassword = pass; uc.dwPasswordLength = 256;

    if (!s_pfnInternetCrackUrlW(pstrURL, 0, dwFlags, &uc)) return 0;

    // See AfxParseURL above for the AFX_INET_SERVICE_* value caveat.
    unsigned long dwServiceType;
    switch (uc.nScheme) {
        case 1: dwServiceType = 1; break;             // FTP
        case 2: dwServiceType = 2; break;              // GOPHER
        case 3: case 4: dwServiceType = 3; break;      // HTTP / HTTPS
        default: return 0;                             // unsupported scheme (e.g. FILE)
    }

    if (pdwServiceType) *pdwServiceType = dwServiceType;
    if (pStrServer) *pStrServer = host;
    if (pStrObject) *pStrObject = (path[0] != L'\0') ? path : L"/";
    if (pnPort) *pnPort = uc.nPort;
    if (pStrUsername) *pStrUsername = user;
    if (pStrPassword) *pStrPassword = pass;
    return 1;
}
// Symbol: ?AfxPreTranslateMessage@@YAHPEAUtagMSG@@@Z
extern "C" int MS_ABI impl__AfxPreTranslateMessage__YAHPEAUtagMSG___Z(void* /*struct*/* p0) {
    auto* pThread = AfxGetThread();
    return pThread ? pThread->PreTranslateMessage(reinterpret_cast<MSG*>(p0)) : FALSE;
}
// Symbol: ?AfxProcessWndProcException@@YA_JPEAVCException@@PEBUtagMSG@@@Z
extern "C" __int64 MS_ABI impl__AfxProcessWndProcException__YA_JPEAVCException__PEBUtagMSG___Z(void* /*class*/* p0, const void* /*struct*/* p1) {
    CWinThread* pThread = AfxGetThread();
    if (!pThread) return 0;
    CException* pException = reinterpret_cast<CException*>(p0);
    const MSG* pMsg = reinterpret_cast<const MSG*>(p1);
    if (auto* pApp = dynamic_cast<CWinApp*>(pThread)) {
        return impl__ProcessWndProcException_CWinApp__UEAA_JPEAVCException__PEBUtagMSG___Z(pApp, pException, pMsg);
    }
    return impl__ProcessWndProcException_CWinThread__UEAA_JPEAVCException__PEBUtagMSG___Z(pThread, pException, pMsg);
}
// Symbol: ?AfxPropPageCallback@@YAIPEAUHWND__@@IPEAU_PROPSHEETPAGEW@@@Z
extern "C" unsigned int MS_ABI impl__AfxPropPageCallback__YAIPEAUHWND____IPEAU_PROPSHEETPAGEW___Z(void* /*struct*/* p0, unsigned int p1, void* /*struct*/* p2) {
    // This repo has no CPropertyPage/CPropertySheet housekeeping hooked up
    // to intercept via ppsp->lParam (no DlgcommonSupport.h / property-sheet
    // infra found for it), so there is no internal state to update here.
    // What IS safe and correct to honor is the documented Win32 contract for
    // a PROPSHEETPAGE.pfnCallback: returning 0 for PSPCB_CREATE tells the
    // property sheet manager to abort creating the page, so pass it through
    // as TRUE (allow creation); PSPCB_RELEASE/anything else -> 0.
    (void)p0; (void)p2;
    return (p1 == PSPCB_CREATE) ? 1u : 0u;
}
// Symbol: ?AfxPropSheetCallback@@YAHPEAUHWND__@@I_J@Z
extern "C" int MS_ABI impl__AfxPropSheetCallback__YAHPEAUHWND____I_J_Z(void* /*struct*/* p0, unsigned int p1, __int64 p2) {
    // Same situation as AfxPropPageCallback above: no CPropertySheet state
    // (e.g. m_psh.hIcon) is reachable from here to act on PSCB_INITIALIZED
    // etc. The PropSheetProc callback's return value is documented as
    // ignored by Windows for every message it can receive, so 0 is the
    // honest pass-through in the absence of that state.
    (void)p0; (void)p1; (void)p2;
    return 0;
}
// Symbol: ?AfxPumpMessage@@YAHXZ
extern "C" int MS_ABI impl__AfxPumpMessage__YAHXZ() {
    auto* pThread = AfxGetThread();
    return pThread ? pThread->PumpMessage() : FALSE;
}
// Symbol: ?AfxRegCreateKey@@YAJPEAUHKEY__@@PEB_WPEAPEAU1@PEAVCAtlTransactionManager@ATL@@@Z
extern "C" long MS_ABI impl__AfxRegCreateKey__YAJPEAUHKEY____PEB_WPEAPEAU1_PEAVCAtlTransactionManager_ATL___Z(
    HKEY hKey, const wchar_t* lpszSubKey, HKEY* phkResult, void* pTransaction) {
    (void)pTransaction;
    return ::RegCreateKeyExW(hKey, lpszSubKey, 0, nullptr, 0, KEY_ALL_ACCESS, nullptr, phkResult, nullptr);
}
// Symbol: ?AfxRegDeleteKey@@YAJPEAUHKEY__@@PEB_WPEAVCAtlTransactionManager@ATL@@@Z
extern "C" long MS_ABI impl__AfxRegDeleteKey__YAJPEAUHKEY____PEB_WPEAVCAtlTransactionManager_ATL___Z(
    HKEY hKey, const wchar_t* lpszSubKey, void* pTransaction) {
    (void)pTransaction;
    return ::RegDeleteKeyW(hKey, lpszSubKey);
}
// Symbol: ?AfxRegOpenKey@@YAJPEAUHKEY__@@PEB_WPEAPEAU1@PEAVCAtlTransactionManager@ATL@@@Z
extern "C" long MS_ABI impl__AfxRegOpenKey__YAJPEAUHKEY____PEB_WPEAPEAU1_PEAVCAtlTransactionManager_ATL___Z(
    HKEY hKey, const wchar_t* lpszSubKey, HKEY* phkResult, void* pTransaction) {
    (void)pTransaction;
    return ::RegOpenKeyExW(hKey, lpszSubKey, 0, KEY_ALL_ACCESS, phkResult);
}
// Symbol: ?AfxRegOpenKeyEx@@YAJPEAUHKEY__@@PEB_WKKPEAPEAU1@PEAVCAtlTransactionManager@ATL@@@Z
extern "C" long MS_ABI impl__AfxRegOpenKeyEx__YAJPEAUHKEY____PEB_WKKPEAPEAU1_PEAVCAtlTransactionManager_ATL___Z(
    HKEY hKey, const wchar_t* lpszSubKey, unsigned long ulOptions, unsigned long samDesired,
    HKEY* phkResult, void* pTransaction) {
    (void)pTransaction;
    return ::RegOpenKeyExW(hKey, lpszSubKey, (DWORD)ulOptions, (REGSAM)samDesired, phkResult);
}
// Symbol: ?AfxRegQueryValue@@YAJPEAUHKEY__@@PEB_WPEA_WPEAJ@Z
extern "C" long MS_ABI impl__AfxRegQueryValue__YAJPEAUHKEY____PEB_WPEA_WPEAJ_Z(
    HKEY hKey, const wchar_t* lpszSubKey, wchar_t* lpszValue, long* plValueLength) {
    return ::RegQueryValueW(hKey, lpszSubKey, lpszValue, plValueLength);
}
// Symbol: ?AfxRegSetValue@@YAJPEAUHKEY__@@PEB_WK1K@Z
extern "C" long MS_ABI impl__AfxRegSetValue__YAJPEAUHKEY____PEB_WK1K_Z(
    HKEY hKey, const wchar_t* lpszSubKey, unsigned long dwType, const wchar_t* lpszValue, unsigned long cbValue) {
    return ::RegSetValueExW(hKey, lpszSubKey, 0, (DWORD)dwType, (const BYTE*)lpszValue, (DWORD)cbValue);
}
// Symbol: ?AfxRegisterMFCCtrlClasses@@YAXXZ
extern "C" void MS_ABI impl__AfxRegisterMFCCtrlClasses__YAXXZ() {
    // Retail (RVA 0x33fa0) is eleven consecutive calls to
    // AfxDoRegisterMFCControlClass(pszClassName, pszBaseClass); the pairs
    // below are the ones it passes, in its order (string constants at
    // 0x33ed18..0x33ee40, cross-checked against the same eleven names
    // already listed in featurepack/controls/CMFCControlContainer.cpp).
    // Every one of them is a superclass of a system window class, and every
    // corresponding control class exists in this tree
    // (CMFCButton, CMFCColorButton, CMFCEditBrowseCtrl, CMFCFontComboBox,
    // CMFCLinkCtrl, CMFCMaskedEdit, CMFCMenuButton, CMFCPropertyGridCtrl,
    // CMFCShellListCtrl, CMFCShellTreeCtrl, CVSListBox), so nothing here
    // registers a name that OpenMFC does not implement.
    static const wchar_t* const kMFCCtrlClasses[][2] = {
        { L"MFCButton",       L"Button"        },
        { L"MFCColorButton",  L"Button"        },
        { L"MFCEditBrowse",   L"Edit"          },
        { L"MFCFontComboBox", L"ComboBox"      },
        { L"MFCLink",         L"Button"        },
        { L"MFCMaskedEdit",   L"Edit"          },
        { L"MFCMenuButton",   L"Button"        },
        { L"MFCPropertyGrid", L"Static"        },
        { L"MFCShellList",    L"SysListView32" },
        { L"MFCShellTree",    L"SysTreeView32" },
        { L"MFCVSListBox",    L"Static"        },
    };
    for (const auto& entry : kMFCCtrlClasses) {
        impl__AfxDoRegisterMFCControlClass__YAXPEB_W0_Z(entry[0], entry[1]);
    }
}
// Symbol: ?AfxRegisterPreviewHandler@@YAHPEB_W00@Z
// AfxRegisterPreviewHandler(LPCWSTR pszCLSID, LPCWSTR pszName, LPCWSTR pszExt)
extern "C" int MS_ABI impl__AfxRegisterPreviewHandler__YAHPEB_W00_Z(const wchar_t* p0, const wchar_t* p1, const wchar_t* p2) {
    const wchar_t* pszCLSID = p0;
    const wchar_t* pszName = p1;
    const wchar_t* pszExt = p2;
    if (!pszCLSID || !pszName || !pszExt) return 0;

    // HKCR\<ext>\ShellEx\{8895b1c6-b41f-4c1c-a562-0d564250836f} = <CLSID>
    wchar_t keyPath[512];
    std::swprintf(keyPath, 512, L"%ls\\ShellEx\\{8895b1c6-b41f-4c1c-a562-0d564250836f}", pszExt);
    HKEY hKey = nullptr;
    LONG lr = ::RegCreateKeyExW(HKEY_CLASSES_ROOT, keyPath, 0, nullptr, 0, KEY_WRITE, nullptr, &hKey, nullptr);
    if (lr != ERROR_SUCCESS) return 0;
    lr = ::RegSetValueExW(hKey, nullptr, 0, REG_SZ,
                           reinterpret_cast<const BYTE*>(pszCLSID),
                           static_cast<DWORD>((std::wcslen(pszCLSID) + 1) * sizeof(wchar_t)));
    ::RegCloseKey(hKey);
    if (lr != ERROR_SUCCESS) return 0;

    // HKLM\SOFTWARE\Microsoft\Windows\CurrentVersion\PreviewHandlers, <CLSID> = <Name>
    hKey = nullptr;
    lr = ::RegCreateKeyExW(HKEY_LOCAL_MACHINE,
                            L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\PreviewHandlers",
                            0, nullptr, 0, KEY_WRITE, nullptr, &hKey, nullptr);
    if (lr != ERROR_SUCCESS) return 0;
    lr = ::RegSetValueExW(hKey, pszCLSID, 0, REG_SZ,
                           reinterpret_cast<const BYTE*>(pszName),
                           static_cast<DWORD>((std::wcslen(pszName) + 1) * sizeof(wchar_t)));
    ::RegCloseKey(hKey);
    return (lr == ERROR_SUCCESS) ? 1 : 0;
}
// Symbol: ?AfxRegisterSiteFactory@@YAHPEAVIControlSiteFactory@@@Z
// AfxRegisterSiteFactory(IControlSiteFactory* pFactory)
extern "C" int MS_ABI impl__AfxRegisterSiteFactory__YAHPEAVIControlSiteFactory___Z(void* pFactory) {
    // Retail (RVA 0x238450) resolves the process-local CControlSiteFactoryMgr
    // and tail-calls CControlSiteFactoryMgr::RegisterSiteFactory; the same
    // shape is used here against this group's singleton (see prologue).
    // NOTE: both retail paths of that member (RVA 0x238340) end in
    // `xor eax,eax`, i.e. retail answers FALSE even after a successful add.
    // OpenMFC's member returns TRUE, and this forwarder returns whatever the
    // member says so the two never disagree.
    if (pFactory == nullptr) {
        return FALSE;   // retail throws an invalid-argument exception here
    }
    S_Ccontrolsitefactorymgr* pMgr = g4_GetSiteFactoryMgr();
    CRITICAL_SECTION* pLock = g4_GetSiteFactoryLock();
    ::EnterCriticalSection(pLock);
    BOOL bResult = g4_IsSiteFactoryRegistered(pMgr, pFactory)
        ? TRUE      // already registered: retail's Find hit skips the AddTail
        : impl__RegisterSiteFactory_CControlSiteFactoryMgr__QEAAHPEAVIControlSiteFactory___Z(pMgr, pFactory);
    ::LeaveCriticalSection(pLock);
    return bResult;
}
// Symbol: ?AfxRegisterThumbnailHandler@@YAHPEB_W0K@Z
// AfxRegisterThumbnailHandler(LPCWSTR pszCLSID, LPCWSTR pszExt, DWORD dwFlags)
extern "C" int MS_ABI impl__AfxRegisterThumbnailHandler__YAHPEB_W0K_Z(const wchar_t* p0, const wchar_t* p1, unsigned long p2) {
    const wchar_t* pszCLSID = p0;
    const wchar_t* pszExt = p1;
    (void)p2; // dwFlags: no documented additional registry effect implemented here
    if (!pszCLSID || !pszExt) return 0;

    // HKCR\<ext>\ShellEx\{e357fccd-a995-4576-b01f-234630154e96} = <CLSID>
    wchar_t keyPath[512];
    std::swprintf(keyPath, 512, L"%ls\\ShellEx\\{e357fccd-a995-4576-b01f-234630154e96}", pszExt);
    HKEY hKey = nullptr;
    LONG lr = ::RegCreateKeyExW(HKEY_CLASSES_ROOT, keyPath, 0, nullptr, 0, KEY_WRITE, nullptr, &hKey, nullptr);
    if (lr != ERROR_SUCCESS) return 0;
    lr = ::RegSetValueExW(hKey, nullptr, 0, REG_SZ,
                           reinterpret_cast<const BYTE*>(pszCLSID),
                           static_cast<DWORD>((std::wcslen(pszCLSID) + 1) * sizeof(wchar_t)));
    ::RegCloseKey(hKey);
    return (lr == ERROR_SUCCESS) ? 1 : 0;
}
// Symbol: ?AfxReleaseD2DRefs@@YAXXZ
extern "C" void MS_ABI impl__AfxReleaseD2DRefs__YAXXZ() {
    // afxrendertarget.h declares this immediately after AfxGetD2DState: it is
    // simply AfxGetD2DState()->ReleaseD2DRefs(), dropping the D2D/DWrite/WIC
    // factory refs the process-local _AFX_D2D_STATE holds. Both halves exist
    // here (featurepack/docking/Globals.cpp and core/d2d/_AFX_D2D_STATE.cpp),
    // so delegate rather than duplicating the teardown -- that keeps this
    // export and the member function in step.
    if (void* pState = impl__AfxGetD2DState__YAPEAV_AFX_D2D_STATE__XZ()) {
        impl__ReleaseD2DRefs__AFX_D2D_STATE__QEAAXXZ(pState);
    }
    // Idempotent: the member drops this state out of cbarcore's
    // g_d2dInitialized set, and erasing an absent key is a no-op.
}
// Symbol: ?AfxRepositionWindow@@YAXPEAUAFX_SIZEPARENTPARAMS@@PEAUHWND__@@PEBUtagRECT@@@Z
// AfxRepositionWindow(AFX_SIZEPARENTPARAMS* lpLayout, HWND hWnd, LPCRECT lpRect)
extern "C" void MS_ABI impl__AfxRepositionWindow__YAXPEAUAFX_SIZEPARENTPARAMS__PEAUHWND____PEBUtagRECT___Z(
    void* p0, HWND hWnd, const RECT* lpRect) {
    // Accumulates one child window into the parent's deferred-reposition
    // batch.  Only lpLayout->hDWP (offset 0) is read or written; the rect /
    // sizeTotal / bStretch members are trimmed by the control bars
    // themselves, not here.  Layout of AFX_SIZEPARENTPARAMS comes from MFC's
    // afxpriv.h (see g4_AFX_SIZEPARENTPARAMS in the prologue) and the
    // sequence below matches retail RVA 0x28ecc0 step for step, including
    // its SWP flag word 0x14 (SWP_NOACTIVATE|SWP_NOZORDER).
    g4_AFX_SIZEPARENTPARAMS* lpLayout = static_cast<g4_AFX_SIZEPARENTPARAMS*>(p0);
    if (hWnd == nullptr || lpRect == nullptr) {
        return;
    }
    // A batch whose DeferWindowPos chain has already failed is dead: retail
    // bails out before touching the window.
    if (lpLayout != nullptr && lpLayout->hDWP == nullptr) {
        return;
    }

    // Nothing to do when the window is already exactly where it is wanted.
    HWND hParent = ::GetParent(hWnd);
    RECT rectOld = { 0, 0, 0, 0 };
    ::GetWindowRect(hWnd, &rectOld);
    ::ScreenToClient(hParent, reinterpret_cast<LPPOINT>(&rectOld));
    ::ScreenToClient(hParent, reinterpret_cast<LPPOINT>(&rectOld) + 1);
    if (::EqualRect(&rectOld, lpRect)) {
        return;
    }

    const int cx = lpRect->right - lpRect->left;
    const int cy = lpRect->bottom - lpRect->top;
    const UINT nFlags = SWP_NOACTIVATE | SWP_NOZORDER;
    if (lpLayout != nullptr) {
        lpLayout->hDWP = ::DeferWindowPos(lpLayout->hDWP, hWnd, nullptr,
                                          lpRect->left, lpRect->top, cx, cy, nFlags);
    } else {
        // No batch in flight (lpLayout == NULL): move the window directly.
        ::SetWindowPos(hWnd, nullptr, lpRect->left, lpRect->top, cx, cy, nFlags);
    }
}
// Symbol: ?AfxResetMsgCache@@YAXXZ
extern "C" void MS_ABI impl__AfxResetMsgCache__YAXXZ() {
    // Real MFC memsets its message-map lookup cache (the AFX_MSG_CACHE array
    // CWnd::OnWndMsg consults) so that entries pointing into an unloading
    // module's message map are dropped -- which is why afxpriv.h declares it
    // next to AfxHookWindowCreate/AfxUnhookWindowCreate.
    // OpenMFC caches nothing to invalidate: AfxFindMessageEntry (above in
    // this file) walks the AFX_MSGMAP_ENTRY array linearly on every call with
    // no memo, and CWnd::OnWndMsg (core/window/CWnd.cpp and the inline in
    // openmfc/afxwin.h) is a plain switch that never consults a map at all.
    // Grepped include/ and phase4/src for an AFX_MSG_CACHE / msgCache /
    // cached lpEntry: no such state exists. Empty is correct.
}
// Symbol: ?AfxResolveShortcut@@YAHPEAVCWnd@@PEB_WPEA_WH@Z
// Resolves a .lnk shortcut via IShellLinkW + IPersistFile::Load + Resolve.
// This file only includes windows.h/objbase.h/ocidl.h; IShellLinkW and its
// CLSID/IID live in <shlobj.h>, which is NOT included at file scope here
// (per instructions). IPersistFile and IID_IPersistFile already resolve via
// the existing ocidl.h include, so only IShellLinkW is hand-declared below
// (function-local, matching the real vtable layout/order) to keep this file
// self-contained; the merge step should still add <shlobj.h> for the real
// interface eventually. See final report.
extern "C" int MS_ABI impl__AfxResolveShortcut__YAHPEAVCWnd__PEB_WPEA_WH_Z(void* /*class*/* p0, const wchar_t* p1, wchar_t* p2, int p3) {
    if (!p1 || !p2 || p3 <= 0) return FALSE;
    CWnd* pWnd = reinterpret_cast<CWnd*>(p0);
    HWND hWnd = pWnd ? pWnd->m_hWnd : nullptr;

    struct IShellLinkWLocal;
    struct IShellLinkWLocalVtbl {
        HRESULT (STDMETHODCALLTYPE *QueryInterface)(IShellLinkWLocal*, REFIID, void**);
        ULONG   (STDMETHODCALLTYPE *AddRef)(IShellLinkWLocal*);
        ULONG   (STDMETHODCALLTYPE *Release)(IShellLinkWLocal*);
        HRESULT (STDMETHODCALLTYPE *GetPath)(IShellLinkWLocal*, LPWSTR, int, void*, DWORD);
        HRESULT (STDMETHODCALLTYPE *GetIDList)(IShellLinkWLocal*, void**);
        HRESULT (STDMETHODCALLTYPE *SetIDList)(IShellLinkWLocal*, const void*);
        HRESULT (STDMETHODCALLTYPE *GetDescription)(IShellLinkWLocal*, LPWSTR, int);
        HRESULT (STDMETHODCALLTYPE *SetDescription)(IShellLinkWLocal*, LPCWSTR);
        HRESULT (STDMETHODCALLTYPE *GetWorkingDirectory)(IShellLinkWLocal*, LPWSTR, int);
        HRESULT (STDMETHODCALLTYPE *SetWorkingDirectory)(IShellLinkWLocal*, LPCWSTR);
        HRESULT (STDMETHODCALLTYPE *GetArguments)(IShellLinkWLocal*, LPWSTR, int);
        HRESULT (STDMETHODCALLTYPE *SetArguments)(IShellLinkWLocal*, LPCWSTR);
        HRESULT (STDMETHODCALLTYPE *GetHotkey)(IShellLinkWLocal*, WORD*);
        HRESULT (STDMETHODCALLTYPE *SetHotkey)(IShellLinkWLocal*, WORD);
        HRESULT (STDMETHODCALLTYPE *GetShowCmd)(IShellLinkWLocal*, int*);
        HRESULT (STDMETHODCALLTYPE *SetShowCmd)(IShellLinkWLocal*, int);
        HRESULT (STDMETHODCALLTYPE *GetIconLocation)(IShellLinkWLocal*, LPWSTR, int, int*);
        HRESULT (STDMETHODCALLTYPE *SetIconLocation)(IShellLinkWLocal*, LPCWSTR, int);
        HRESULT (STDMETHODCALLTYPE *SetRelativePath)(IShellLinkWLocal*, LPCWSTR, DWORD);
        HRESULT (STDMETHODCALLTYPE *Resolve)(IShellLinkWLocal*, HWND, DWORD);
        HRESULT (STDMETHODCALLTYPE *SetPath)(IShellLinkWLocal*, LPCWSTR);
    };
    struct IShellLinkWLocal { const IShellLinkWLocalVtbl* lpVtbl; };

    static const GUID kCLSID_ShellLink =
        { 0x00021401, 0x0000, 0x0000, { 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 } };
    static const GUID kIID_IShellLinkW =
        { 0x000214F9, 0x0000, 0x0000, { 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 } };
    const DWORD kSLR_NO_UI = 0x0001;
    const DWORD kSLR_ANY_MATCH = 0x0002;

    IShellLinkWLocal* psl = nullptr;
    HRESULT hr = ::CoCreateInstance(kCLSID_ShellLink, nullptr, CLSCTX_INPROC_SERVER,
                                     kIID_IShellLinkW, reinterpret_cast<void**>(&psl));
    if (FAILED(hr) || !psl) return FALSE;

    BOOL bResult = FALSE;
    IPersistFile* ppf = nullptr;
    hr = psl->lpVtbl->QueryInterface(psl, IID_IPersistFile, reinterpret_cast<void**>(&ppf));
    if (SUCCEEDED(hr) && ppf) {
        hr = ppf->Load(p1, STGM_READ);
        if (SUCCEEDED(hr)) {
            hr = psl->lpVtbl->Resolve(psl, hWnd, kSLR_ANY_MATCH | kSLR_NO_UI);
            if (SUCCEEDED(hr)) {
                wchar_t szPath[MAX_PATH] = {};
                hr = psl->lpVtbl->GetPath(psl, szPath, MAX_PATH, nullptr, 0);
                if (SUCCEEDED(hr)) {
                    size_t len = std::wcslen(szPath);
                    if (static_cast<int>(len) >= p3) len = static_cast<size_t>(p3 - 1);
                    std::memcpy(p2, szPath, len * sizeof(wchar_t));
                    p2[len] = L'\0';
                    bResult = TRUE;
                }
            }
        }
        ppf->Release();
    }
    psl->lpVtbl->Release(psl);
    return bResult;
}
// Symbol: ?AfxSafeArrayInit@@YAXPEAVCOleSafeArray@@@Z
extern "C" void MS_ABI impl__AfxSafeArrayInit__YAXPEAVCOleSafeArray___Z(void* /*class*/* p0) {
    // Matches the inline zero-init already used along the COleSafeArray
    // ctor path (core/ole/COleSafeArray.cpp): a COleSafeArray is a
    // tagVARIANT plus two DWORDs, zero-initialized.
    if (p0) std::memset(p0, 0, sizeof(COleSafeArray));
}
// Symbol: ?AfxSetNewHandler@@YAP6AH_K@ZP6AH0@Z@Z
extern "C" void* MS_ABI impl__AfxSetNewHandler__YAP6AH_K_ZP6AH0_Z_Z(void* /*fnptr*/ p0) {
    void* pPrev = g_pfnNewHandler;
    g_pfnNewHandler = p0;
    return pPrev;
}
// Symbol: ?AfxSetPerUserRegistration@@YAXH@Z
extern "C" void MS_ABI impl__AfxSetPerUserRegistration__YAXH_Z(int p0) {
    g_bPerUserRegistration = p0;
}
// Symbol: ?AfxStoreField@@YAXAEAVCRecordset@@IPEAX@Z
// void AFXAPI AfxStoreField(CRecordset& rs, UINT nField, void* pvField)
// The write side of the pair above: snapshot field nField into the recordset's
// RFX value cache. The generated parameter list is already right (the
// CRecordset& is one pointer slot).
//
// Decoded from the real mfc140u.dll (x64 redist 14.51, ordinal 2334):
//     i    = nField - 1;
//     info = &rs.m_pFieldInfo[i];                       // rs+0x58, see AfxLoadField
//     info->m_bStatus = rs.m_pbFieldFlags[i];           // rs+0x178
//     if (rs.m_pbFieldFlags[i] & 0x02) return;          // NULL: nothing to snapshot
//     dst = (info->m_nDataType is 1..4 or 6) ? (void*)info : info->m_pvDataCache;
//     AfxCopyValue(pvField, dst, &ignoredLength, info->m_nDataType);
//
// Left stubbed: both halves of that need storage this codebase does not have.
// CRecordset here (include/openmfc/afxdb.h, 216 bytes) has neither the
// m_pFieldInfo cache array nor the m_pbFieldFlags byte array, and
// RecordsetState (phase4/src/detail/CRecordsetSupport.h) records field status
// only as dirty/null index sets -- it carries no per-field data type and no
// value bytes, so there is no way to know how many bytes at pvField to copy,
// let alone anywhere to put them. Implementing this requires the per-field
// cache described above; a private side map here could not be freed with the
// recordset and would go stale on pointer reuse, which is worse than no-op.
extern "C" void MS_ABI impl__AfxStoreField__YAXAEAVCRecordset__IPEAX_Z(void* /*class*/* p0, unsigned int p1, void* p2) {}
// Symbol: ?AfxStringFromCLSID@@YA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@AEBU_GUID@@@Z
// CString AfxStringFromCLSID(const GUID& clsid)
// Signature rewritten for the same reason as AFXGetRegPath: the by-value
// CStringT return is a hidden RCX return slot (returned again in RAX), so the
// GUID reference is the single real argument and lives in RDX, not R9.
// Formats the registry-style braced form, upper-case hex -- byte-identical
// to StringFromGUID2's output. Real MFC dereferences the reference blind;
// a NULL pointer here yields an empty CString instead of faulting.
extern "C" CString* MS_ABI impl__AfxStringFromCLSID__YA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__AEBU_GUID___Z(
    CString* pRet, const GUID* pclsid) {
    if (!pRet) {
        return pRet;
    }
    wchar_t szCLSID[256];
    szCLSID[0] = L'\0';
    if (pclsid != nullptr) {
        std::swprintf(szCLSID, 256,
                      L"{%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X}",
                      static_cast<unsigned int>(pclsid->Data1),
                      static_cast<unsigned int>(pclsid->Data2),
                      static_cast<unsigned int>(pclsid->Data3),
                      static_cast<unsigned int>(pclsid->Data4[0]),
                      static_cast<unsigned int>(pclsid->Data4[1]),
                      static_cast<unsigned int>(pclsid->Data4[2]),
                      static_cast<unsigned int>(pclsid->Data4[3]),
                      static_cast<unsigned int>(pclsid->Data4[4]),
                      static_cast<unsigned int>(pclsid->Data4[5]),
                      static_cast<unsigned int>(pclsid->Data4[6]),
                      static_cast<unsigned int>(pclsid->Data4[7]));
    }
    new (pRet) CString(szCLSID);
    return pRet;
}
// Symbol: ?AfxTermExtensionModule@@YAXAEAUAFX_EXTENSION_MODULE@@H@Z
extern "C" void MS_ABI impl__AfxTermExtensionModule__YAXAEAUAFX_EXTENSION_MODULE__H_Z(void* /*struct*/* p0, int p1) {
    (void)p1;
    // See AfxInitExtensionModule above for the mirrored AFX_EXTENSION_MODULE
    // layout this unhooks.
    struct LocalExtModule {
        int   bInitialized;
        void* hModule;
        void* hResourceModule;
        void* pFirstSharedClass;
        void* pFirstSharedFactory;
    };
    auto* pState = reinterpret_cast<LocalExtModule*>(p0);
    if (!pState || !pState->bInitialized) return;
    pState->bInitialized = 0;
    pState->hModule = nullptr;
    pState->hResourceModule = nullptr;
    pState->pFirstSharedClass = nullptr;
    pState->pFirstSharedFactory = nullptr;
}
// Symbol: ?AfxTermLocalData@@YAXPEAUHINSTANCE__@@H@Z
extern "C" void MS_ABI impl__AfxTermLocalData__YAXPEAUHINSTANCE____H_Z(void* /*struct*/* p0, int p1) {
    // Mirror of AfxInitLocalData above: clears the current AFX_MODULE_STATE's
    // instance/resource handles, either unconditionally (bAll) or only if
    // they currently belong to the terminating module.
    struct LocalModuleState {
        HINSTANCE m_hCurrentInstanceHandle;
        HINSTANCE m_hCurrentResourceHandle;
        void*     m_pCurrentWinApp;
    };
    // AfxGetAppModuleState() is already declared (with plain C++ linkage,
    // AFXAPI calling convention) in openmfc/afxwin.h -- no new declaration
    // needed.
    auto* pState = reinterpret_cast<LocalModuleState*>(AfxGetAppModuleState());
    if (!pState) return;
    HINSTANCE hInstance = reinterpret_cast<HINSTANCE>(p0);
    const bool bAll = p1 != 0;
    if (bAll || pState->m_hCurrentInstanceHandle == hInstance) {
        pState->m_hCurrentInstanceHandle = nullptr;
        pState->m_hCurrentResourceHandle = nullptr;
    }
}
// Symbol: ?AfxTermThread@@YAXPEAUHINSTANCE__@@@Z
extern "C" void MS_ABI impl__AfxTermThread__YAXPEAUHINSTANCE_____Z(void* /*struct*/* p0) {
    // OpenMFC keeps one global app/thread model rather than tracking state
    // per attaching HINSTANCE, so the incoming module handle is advisory
    // only: unconditionally flush this thread's temp object maps and reset
    // its AFX bookkeeping, mirroring AfxInitThread's defaults.
    (void)p0;
    if (_AFX_THREAD_STATE* pState = AfxGetThreadState()) {
        CWnd::DeleteTempMap();
        impl__DeleteTempMap_CDC__SAXXZ();
        impl__DeleteTempMap_CGdiObject__SAXXZ();
        pState->nTempMapLock = 0;
        pState->nWndCreateLock = 0;
    }
}
// Symbol: ?AfxTimeToFileTime@@YAXAEBVCTime@ATL@@PEAU_FILETIME@@@Z
extern "C" void MS_ABI impl__AfxTimeToFileTime__YAXAEBVCTime_ATL__PEAU_FILETIME___Z(const void* /*class*/* p0, void* /*struct*/* p1) {
    if (!p0 || !p1) return;
    // Both MFC's CTime and ATL::CTime store a single 64-bit time_t (seconds
    // since 1970-01-01) as their sole data member.
    const __int64 t = *reinterpret_cast<const __int64*>(p0);
    const unsigned __int64 ft =
        static_cast<unsigned __int64>(t) * 10000000ULL + 116444736000000000ULL;
    FILETIME* pft = reinterpret_cast<FILETIME*>(p1);
    pft->dwLowDateTime = static_cast<DWORD>(ft & 0xFFFFFFFFULL);
    pft->dwHighDateTime = static_cast<DWORD>(ft >> 32);
}
// Symbol: ?AfxTlsAddRef@@YAXXZ
// Real MFC gates one-time TLS-index allocation on the 0->1 transition of a
// process-wide refcount shared with AfxTlsRelease. OpenMFC's
// CThreadSlotDataSupport.h models per-slot-table instances (each already
// self-managing its own TLS index), not a single module-wide TLS index tied
// to a refcount, so there is no allocation for this transition to gate;
// this still tracks the refcount itself via g_afxTlsRefCount.
extern "C" void MS_ABI impl__AfxTlsAddRef__YAXXZ() {
    ::InterlockedIncrement(&g_afxTlsRefCount);
}
// Symbol: ?AfxTlsRelease@@YAXXZ
extern "C" void MS_ABI impl__AfxTlsRelease__YAXXZ() {
    if (::InterlockedDecrement(&g_afxTlsRefCount) == 0) {
        // Real MFC frees the module's TLS index here on the last release.
        // As noted above, OpenMFC has no shared TLS resource tied to this
        // counter to free, so decrementing to zero is the complete, honest
        // behavior here -- there is nothing to tear down.
    }
}
// Symbol: ?AfxTrackerTerm@@YAXXZ
extern "C" void MS_ABI impl__AfxTrackerTerm__YAXXZ() {
    // Real MFC frees CRectTracker's cached hatch brush and its loaded
    // resize/move cursors here. OpenMFC's CRectTracker
    // (core/gdi/CRectTracker.cpp) is byte-exact but allocates no GDI objects
    // at all: read the whole file -- its only handles are
    // GetStockObject(BLACK_BRUSH) (stock, never deleted) and
    // LoadCursorW(nullptr, IDC_*) (shared system cursors, which must not be
    // destroyed). Grepped it for Create*Brush / CreatePen / DeleteObject /
    // LoadBitmap: no matches. Nothing to release -- empty is correct.
}
// Symbol: ?AfxTryCleanup@@YAXXZ
extern "C" void MS_ABI impl__AfxTryCleanup__YAXXZ() {
    // Called from AFX_EXCEPTION_LINK::~AFX_EXCEPTION_LINK (inline in the
    // client's afx.h): real MFC deletes the exception the link holds and
    // unlinks it from the thread's AFX_EXCEPTION_CONTEXT::m_pLinkTop chain.
    // NOT MODELLED HERE: grepped for AFX_EXCEPTION_CONTEXT, m_pLinkTop and
    // m_pLinkPrev across include/ and phase4/src -- none exist, and the
    // exported AFX_EXCEPTION_LINK constructor
    // (core/exceptions/AFX_EXCEPTION_LINK.cpp) is a no-op that returns pThis
    // without linking or even zeroing the frame. Since nothing is ever
    // pushed, popping here would read uninitialised client stack memory;
    // staying empty is the only consistent behaviour until the link chain is
    // actually built by the constructor.
}
// Symbol: ?AfxUnRegisterPreviewHandler@@YAHPEB_W@Z
// AfxUnRegisterPreviewHandler(LPCWSTR pszCLSID)
extern "C" int MS_ABI impl__AfxUnRegisterPreviewHandler__YAHPEB_W_Z(const wchar_t* p0) {
    const wchar_t* pszCLSID = p0;
    if (!pszCLSID) return 0;
    HKEY hKey = nullptr;
    LONG lr = ::RegOpenKeyExW(HKEY_LOCAL_MACHINE,
                               L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\PreviewHandlers",
                               0, KEY_WRITE, &hKey);
    if (lr != ERROR_SUCCESS) return 0;
    lr = ::RegDeleteValueW(hKey, pszCLSID);
    ::RegCloseKey(hKey);
    return (lr == ERROR_SUCCESS) ? 1 : 0;
}
// Symbol: ?AfxUnhookWindowCreate@@YAHXZ
// Counterpart of AfxHookWindowCreate above: clears the pending-CWnd thread
// state and removes the WH_CBT hook installed there, if one is installed.
extern "C" int MS_ABI impl__AfxUnhookWindowCreate__YAHXZ() {
    _AFX_THREAD_STATE* pState = AfxGetThreadState();
    if (!pState) return FALSE;
    pState->pWndInit = nullptr;
    if (pState->hHookOldCbtFilter) {
        HHOOK hHook = reinterpret_cast<HHOOK>(pState->hHookOldCbtFilter);
        pState->hHookOldCbtFilter = nullptr;
        return ::UnhookWindowsHookEx(hHook) ? TRUE : FALSE;
    }
    return FALSE;
}
// Symbol: ?AfxUnlockGlobals@@YAXH@Z
// Leaves the same per-nLockType critical section AfxLockGlobals (above)
// enters; see that function's comment for the shared array this pair uses.
extern "C" void MS_ABI impl__AfxUnlockGlobals__YAXH_Z(int p0) {
    if (CRITICAL_SECTION* pcs = GetGlobalLock(p0)) {
        ::LeaveCriticalSection(pcs);
    }
}
// Symbol: ?AfxUnlockTempMaps@@YAHH@Z
extern "C" int MS_ABI impl__AfxUnlockTempMaps__YAHH_Z(int p0) {
    // Shares _AFX_THREAD_STATE with AfxLockTempMaps above. bDeleteTemps
    // (p0) forces an immediate flush even if the lock count hasn't reached
    // zero yet, matching real MFC's bAll force-flag behavior; either way,
    // reaching zero always flushes.
    _AFX_THREAD_STATE* pState = AfxGetThreadState();
    if (!pState) return FALSE;
    if (pState->nTempMapLock > 0) {
        pState->nTempMapLock--;
    }
    const BOOL bDeleteTemps = static_cast<BOOL>(p0);
    if (pState->nTempMapLock == 0 || bDeleteTemps) {
        CWnd::DeleteTempMap();
        impl__DeleteTempMap_CDC__SAXXZ();
        impl__DeleteTempMap_CGdiObject__SAXXZ();
        return TRUE;
    }
    return FALSE;
}
// Symbol: ?AfxUnmergeMenus@@YAXPEAUHMENU__@@00@Z
// Undoes AfxMergeMenus above: AfxMergeMenus attaches hMenuSource's existing
// popup HMENUs directly (by handle, not by clone) into hMenuShared, so
// unmerging walks hMenuShared's top-level items and removes any whose
// submenu handle is still found (by identity) in either candidate source
// menu (h1/h2). Best-effort reconstruction, not verified against retail.
extern "C" void MS_ABI impl__AfxUnmergeMenus__YAXPEAUHMENU____00_Z(void* /*struct*/* p0, void* /*struct*/* p1, void* /*struct*/* p2) {
    HMENU hMenuShared = reinterpret_cast<HMENU>(p0);
    HMENU h1 = reinterpret_cast<HMENU>(p1);
    HMENU h2 = reinterpret_cast<HMENU>(p2);
    if (!hMenuShared || !::IsMenu(hMenuShared)) return;

    auto containsSubMenu = [](HMENU hParent, HMENU hSub) -> bool {
        if (!hParent || !hSub || !::IsMenu(hParent)) return false;
        int n = ::GetMenuItemCount(hParent);
        for (int i = 0; i < n; ++i) {
            if (::GetSubMenu(hParent, i) == hSub) return true;
        }
        return false;
    };

    int i = 0;
    int nCount = ::GetMenuItemCount(hMenuShared);
    while (i < nCount) {
        HMENU hSub = ::GetSubMenu(hMenuShared, i);
        if (hSub && (containsSubMenu(h1, hSub) || containsSubMenu(h2, hSub))) {
            ::RemoveMenu(hMenuShared, static_cast<UINT>(i), MF_BYPOSITION);
            nCount = ::GetMenuItemCount(hMenuShared);
        } else {
            ++i;
        }
    }
}
// Symbol: ?AfxUnregisterSiteFactory@@YAHPEAVIControlSiteFactory@@@Z
// AfxUnregisterSiteFactory(IControlSiteFactory* pFactory)
extern "C" int MS_ABI impl__AfxUnregisterSiteFactory__YAHPEAVIControlSiteFactory___Z(void* pFactory) {
    // Mirror of AfxRegisterSiteFactory: retail (RVA 0x238490) tail-calls
    // CControlSiteFactoryMgr::UnregisterSiteFactory, which walks the list and
    // returns TRUE only when the factory was actually found and removed
    // (RVA 0x2383c0) -- the same contract OpenMFC's member implements.
    if (pFactory == nullptr) {
        return FALSE;   // retail throws an invalid-argument exception here
    }
    S_Ccontrolsitefactorymgr* pMgr = g4_GetSiteFactoryMgr();
    CRITICAL_SECTION* pLock = g4_GetSiteFactoryLock();
    ::EnterCriticalSection(pLock);
    BOOL bResult = impl__UnregisterSiteFactory_CControlSiteFactoryMgr__QEAAHPEAVIControlSiteFactory___Z(pMgr, pFactory);
    ::LeaveCriticalSection(pLock);
    return bResult;
}
// Symbol: ?AfxUnregisterWndClasses@@YAXXZ
extern "C" void MS_ABI impl__AfxUnregisterWndClasses__YAXXZ() {
    // OpenMFC only tracks the single base OpenMFC window-class atom; unwind
    // it the same way RegisterOpenMFCClass registers it. Real MFC also
    // unregisters AfxControlBar/AfxMDIFrame/etc., which OpenMFC does not
    // model as separate classes yet.
    if (g_atomOpenMFCClass) {
        HINSTANCE hInst = AfxGetInstanceHandle();
        if (!hInst) hInst = ::GetModuleHandleW(nullptr);
        ::UnregisterClassW(g_szOpenMFCClass, hInst);
        g_atomOpenMFCClass = 0;
    }
}
// Symbol: ?AfxVerifyLicFile@@YAHPEAUHINSTANCE__@@PEB_W1I@Z
extern "C" int MS_ABI impl__AfxVerifyLicFile__YAHPEAUHINSTANCE____PEB_W1I_Z(void* /*struct*/* p0, const wchar_t* p1, const wchar_t* p2, unsigned int p3) {
    if (!p1 || !p2) return FALSE;
    HINSTANCE hInst = reinterpret_cast<HINSTANCE>(p0);

    // Resolve lpszLicFileName relative to the module's own directory.
    wchar_t szPath[MAX_PATH] = {};
    if (hInst) {
        DWORD len = ::GetModuleFileNameW(hInst, szPath, MAX_PATH);
        if (len == 0 || len >= MAX_PATH) szPath[0] = L'\0';
        wchar_t* lastSlash = std::wcsrchr(szPath, L'\\');
        if (lastSlash) *(lastSlash + 1) = L'\0';
        else szPath[0] = L'\0';
    }
    std::wcsncat(szPath, p1, MAX_PATH - std::wcslen(szPath) - 1);

    HANDLE hFile = ::CreateFileW(szPath, GENERIC_READ, FILE_SHARE_READ, nullptr,
                                  OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
    if (hFile == INVALID_HANDLE_VALUE) return FALSE;

    char buf[512];
    DWORD bytesRead = 0;
    BOOL ok = ::ReadFile(hFile, buf, sizeof(buf) - 1, &bytesRead, nullptr);
    ::CloseHandle(hFile);
    if (!ok) return FALSE;
    buf[bytesRead] = '\0';

    wchar_t wbuf[512];
    int n = ::MultiByteToWideChar(CP_ACP, 0, buf, -1, wbuf, 512);
    if (n <= 0) return FALSE;
    for (int i = n - 1; i >= 0 && (wbuf[i] == L'\r' || wbuf[i] == L'\n' || wbuf[i] == L'\0'); --i) {
        wbuf[i] = L'\0';
    }

    size_t compareLen = (p3 != 0) ? p3 : std::wcslen(p2);
    return (std::wcsncmp(wbuf, p2, compareLen) == 0) ? TRUE : FALSE;
}
// Symbol: ?AfxWinTerm@@YAXXZ
extern "C" void MS_ABI impl__AfxWinTerm__YAXXZ() {
    // Counterpart of AfxWinInit (core/app/Globals.cpp), unwound in reverse
    // order. What AfxWinInit brings up in this codebase is (a) the process
    // instance/resource handles, and (b), indirectly, the per-thread AFX
    // state plus the window class CWnd::Create/CFrameWnd register lazily on
    // first use.
    //
    // 1. Per-thread teardown. Real MFC's terminate path flushes the thread's
    //    temp maps; OpenMFC's AfxTermThread does exactly that (CWnd/CDC/
    //    CGdiObject temp maps + the nTempMapLock/nWndCreateLock counters) and
    //    ignores its HINSTANCE argument.
    impl__AfxTermThread__YAXPEAUHINSTANCE_____Z(nullptr);
    // 2. Drop the window class registered lazily on first window creation
    //    (detail/WincoreSupport.cpp's RegisterOpenMFCClass /
    //    g_atomOpenMFCClass), through the existing hook that owns it.
    impl__AfxUnregisterWndClasses__YAXXZ();
    // Both steps are individually idempotent and safe before AfxWinInit ever
    // ran (the temp maps are empty, the class atom is still zero), so calling
    // AfxWinTerm twice is harmless.
    //
    // Deliberately NOT cleared here: detail/CWinAppSupport.h's g_hInstance /
    // g_hResource. Real MFC leaves the module state's instance and resource
    // handles to AfxTermLocalData (exported separately, implemented above),
    // and nulling them from here would break AfxGetResourceHandle for any
    // module that terminates one thread and keeps running.
}
// Symbol: ?AfxWndProc@@YA_JPEAUHWND__@@I_K_J@Z
extern "C" __int64 MS_ABI impl__AfxWndProc__YA_JPEAUHWND____I_K_J_Z(void* /*struct*/* p0, unsigned int p1, unsigned __int64 p2, __int64 p3) {
    return static_cast<__int64>(AfxWndProc(reinterpret_cast<HWND>(p0), p1, static_cast<WPARAM>(p2), static_cast<LPARAM>(p3)));
}
// Symbol: ?AfxWndProcDllOle@@YA_JPEAUHWND__@@I_K_J@Z
extern "C" __int64 MS_ABI impl__AfxWndProcDllOle__YA_JPEAUHWND____I_K_J_Z(void* /*struct*/* p0, unsigned int p1, unsigned __int64 p2, __int64 p3) {
    return static_cast<__int64>(AfxWndProc(reinterpret_cast<HWND>(p0), p1, static_cast<WPARAM>(p2), static_cast<LPARAM>(p3)));
}
// Symbol: ?ChWindowFromPoint@@YAPEAUHWND__@@PEAU1@UtagPOINT@@@Z
// Real signature is (HWND hWnd, POINT pt); the auto-generated thunk had
// dropped the POINT argument. sizeof(POINT)==8, so under the x64 MS ABI it
// arrives by value in RDX -- fixed here by declaring it as a plain by-value
// POINT parameter, which the linker doesn't care about (extern "C" name is
// unaffected by the parameter list). Implements MFC's deepest-child hit
// test: repeatedly ChildWindowFromPoint, remapping pt into each child's
// client coordinates via MapWindowPoints, until no further child contains
// the point.
extern "C" void* MS_ABI impl__ChWindowFromPoint__YAPEAUHWND____PEAU1_UtagPOINT___Z(void* /*struct*/* p0, POINT p1) {
    HWND hWnd = reinterpret_cast<HWND>(p0);
    if (!hWnd) return nullptr;
    POINT pt = p1;
    for (;;) {
        HWND hChild = ::ChildWindowFromPoint(hWnd, pt);
        if (!hChild || hChild == hWnd) break;
        ::MapWindowPoints(hWnd, hChild, &pt, 1);
        hWnd = hChild;
    }
    return hWnd;
}
// Symbol: ?ControlBarCleanUp@@YAXXZ
extern "C" void MS_ABI impl__ControlBarCleanUp__YAXXZ() {
    // afxcontrolbarutil.h declares this next to AfxGlobalsAddRef/Release: it
    // is the control-bar module's teardown hook, run when the last reference
    // to the shared control-bar globals goes away. Release the two pieces of
    // module-wide control-bar state this codebase actually owns.
    //
    // 1. The toolbar image cache: CMFCToolBarImages::CleanUp
    //    (featurepack/toolbar/CMFCToolBarImages.cpp) DestroyIcon()s every
    //    icon the module created and clears g_toolbarImagesState. This is
    //    the only module-wide GDI cache in the control-bar code.
    impl__CleanUp_CMFCToolBarImages__SAXXZ();
    // 2. The process-wide visual-manager singleton (g_pVisualManager), which
    //    owns the control-bar drawing state; DestroyInstance deletes it and
    //    re-syncs the exported instance pointer.
    impl__DestroyInstance_CMFCVisualManager__SAXH_Z(TRUE);
    // Both are idempotent: the image map is emptied, and DestroyInstance
    // deletes a null pointer harmlessly, so a second call (or a call before
    // any control bar was ever created) does nothing.
    // TRUE is real MFC's bAutoDestroyOnly, i.e. "only destroy the instance the
    // framework created itself". OpenMFC's DestroyInstance export ignores that
    // flag and always deletes g_pVisualManager, so an app that constructed its
    // own visual manager (the ctor installs `this` as the singleton) and then
    // deletes it would double-delete. That deviation lives in DestroyInstance,
    // not here; TRUE is passed so this call becomes correct the moment the
    // flag is honoured.
    //
    // Not touched: detail/WincoreSupport.h's g_frameWndRuntimeStates /
    // g_dockStateSnapshots / g_barStateStore are per-frame data keyed by a
    // live CFrameWnd and are released by that frame's own teardown, not by a
    // module-wide hook.
}
// Symbol: ?GetCmdMgr@@YAPEAVCCommandManager@@XZ
extern "C" void* MS_ABI impl__GetCmdMgr__YAPEAVCCommandManager__XZ() {
    // Retail GetCmdMgr (RVA 0x2ed70) hands back the address of a module-scope
    // CCommandManager built by static initialization -- it never allocates
    // and never returns NULL, so callers dereference the result unchecked.
    // The size of that object is known (0x120; see g4_cmdMgrStorage in the
    // prologue), so the singleton is modelled with static storage of exactly
    // that size and run through OpenMFC's own constructor thunk.
    // That thunk is a no-op, which would leave every embedded CMap with
    // m_nHashTableSize == 0 -- and the header-inline CCommandManager
    // accessors that clients compile into their own code divide by exactly
    // that field -- so the constants retail's constructor writes are applied
    // here as well (see g4_InitCmdMgrStorage; the values are idempotent).
    // Caveat: the six vfptr slots retail fills in (0x00, 0x08, 0x40, 0x78,
    // 0xb0, 0xe8) stay null.  That is safe for every exported
    // CCommandManager method -- all of which ignore their `this` -- and for
    // the inline accessors, which dispatch nothing; only a virtual delete of
    // the manager would fault, which retail never does since the singleton
    // is static.
    static bool s_inited = [] {
        impl___0CCommandManager__IEAA_XZ(g4_cmdMgrStorage);
        g4_InitCmdMgrStorage(g4_cmdMgrStorage);
        return true;
    }();
    (void)s_inited;
    return g4_cmdMgrStorage;
}
// Symbol: ?GetCommCtrlVersion@@YAJPEAK0@Z
extern "C" long MS_ABI impl__GetCommCtrlVersion__YAJPEAK0_Z(unsigned long* p0, unsigned long* p1) {
    if (!p0 || !p1) return E_INVALIDARG;
    HMODULE hMod = ::LoadLibraryW(L"comctl32.dll");
    if (!hMod) return E_FAIL;

    // DLLVERSIONINFO / DllGetVersion are declared in <shlwapi.h>, which is
    // not included here; declare the minimal shape locally.
    struct DLLVERSIONINFOLocal {
        DWORD cbSize;
        DWORD dwMajorVersion;
        DWORD dwMinorVersion;
        DWORD dwBuildNumber;
        DWORD dwPlatformID;
    };
    using DllGetVersionProc = HRESULT (CALLBACK*)(DLLVERSIONINFOLocal*);

    HRESULT hr = E_FAIL;
    DllGetVersionProc pDllGetVersion =
        reinterpret_cast<DllGetVersionProc>(::GetProcAddress(hMod, "DllGetVersion"));
    if (pDllGetVersion) {
        DLLVERSIONINFOLocal dvi = {};
        dvi.cbSize = sizeof(dvi);
        hr = pDllGetVersion(&dvi);
        if (SUCCEEDED(hr)) {
            *p0 = dvi.dwMajorVersion;
            *p1 = dvi.dwMinorVersion;
        }
    }
    ::FreeLibrary(hMod);
    return hr;
}
// Symbol: ?GetDHtmlEventMap@@YAPEBUDHtmlEventMapEntry@@XZ
extern "C" void* MS_ABI impl__GetDHtmlEventMap__YAPEBUDHtmlEventMapEntry__XZ() {
    // A DHTML event map is a table walked until its DHTMLEVENTMAPENTRY_END
    // row (END_DHTML_EVENT_MAP in afxdhtml.h emits exactly that terminator),
    // and retail (RVA 0x212020) returns the address of a static table, never
    // NULL -- so the honest empty answer is a terminator-only table, not a
    // null pointer that every walker would dereference.  Retail's own base
    // map carries one entry plus the terminator, but its handler is an
    // internal MFC member function with no counterpart here, so only the
    // terminator is reproduced.
    return const_cast<g4_DHtmlEventMapEntry*>(g4_dhtmlEventMapEmpty);
}
// Symbol: ?IsHelpKey@@YAHPEAUtagMSG@@@Z
extern "C" int MS_ABI impl__IsHelpKey__YAHPEAUtagMSG___Z(void* /*struct*/* p0) {
    const MSG* pMsg = reinterpret_cast<const MSG*>(p0);
    if (!pMsg) return FALSE;
    if (pMsg->message != WM_KEYDOWN && pMsg->message != WM_SYSKEYDOWN) return FALSE;
    if (pMsg->wParam != VK_F1) return FALSE;
    if (::GetKeyState(VK_CONTROL) < 0) return FALSE;
    if (::GetKeyState(VK_MENU) < 0) return FALSE;
    return TRUE;
}
// Symbol: ?ReadBoolProp@@YAHAEAVCTagManager@@PEB_WAEAH@Z
// Mangled params are (CTagManager&, const wchar_t*, int&) -- no CStringT<>
// reference appears in this signature, so the demangler-bug slot-shift that
// affects AfxGetPropSheetFont/AfxParseURL doesn't apply here: the
// auto-generated thunk's 3-parameter list (class*, wchar_t*, int*) already
// matches faithfully. Thin forwarder to CTagManager::ReadBool(const
// CString&, int&) (include/openmfc/afxmfc.h), now that this file includes
// that header.
extern "C" int MS_ABI impl__ReadBoolProp__YAHAEAVCTagManager__PEB_WAEAH_Z(void* /*class*/* p0, const wchar_t* p1, int* p2) {
    if (!p0 || !p2) return FALSE;
    CTagManager* pTagManager = reinterpret_cast<CTagManager*>(p0);
    CString strTag(p1);
    return impl__ReadBool_CTagManager__QEAAHAEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__AEAH_Z(
        pTagManager, &strTag, p2);
}
// Symbol: ?TaskDialogCallback@@YAJPEAUHWND__@@I_K_J2@Z
extern "C" long MS_ABI impl__TaskDialogCallback__YAJPEAUHWND____I_K_J2_Z(void* /*struct*/* p0, unsigned int p1, unsigned __int64 p2, __int64 p3, unsigned __int64 p4) {
    // PFTASKDIALOGCALLBACK. CTaskDialog.cpp (phase4/src/core/dialog) doesn't
    // wire a callback into its TASKDIALOGCONFIG at all, so there's no
    // per-instance state (CTaskDialog* via lpCallbackData) reachable from
    // here to act on notifications like TDN_BUTTON_CLICKED. S_OK is the
    // documented honest default for a callback that lets the dialog proceed
    // without overriding its behavior.
    (void)p0; (void)p1; (void)p2; (void)p3; (void)p4;
    return S_OK;
}
// Symbol: ?_AfxDeleteRegKey@@YAHPEB_W@Z
extern "C" int MS_ABI impl___AfxDeleteRegKey__YAHPEB_W_Z(const wchar_t* p0) {
    if (!p0 || !p0[0]) return FALSE;
    // Same recursive-delete primitive already relied on by
    // COleObjectFactory::UpdateRegistry(FALSE) (core/ole/COleObjectFactory.cpp).
    LONG res = ::RegDeleteTreeW(HKEY_CLASSES_ROOT, p0);
    return (res == ERROR_SUCCESS || res == ERROR_FILE_NOT_FOUND) ? TRUE : FALSE;
}
// Symbol: ?_AfxFindSiteOrWnd@@YAPEAUCOleControlSiteOrWnd@@PEAVCWnd@@0@Z
extern "C" void* MS_ABI impl___AfxFindSiteOrWnd__YAPEAUCOleControlSiteOrWnd__PEAVCWnd__0_Z(void* /*class*/* p0, void* /*class*/* p1) {
    CWnd* pWndDlg = reinterpret_cast<CWnd*>(p0);
    CWnd* pWndCtrl = reinterpret_cast<CWnd*>(p1);
    if (!pWndCtrl) return nullptr;
    if (pWndDlg) {
        // CWnd::FindSiteOrWnd (core/window/CWnd.cpp) already implements
        // the site-lookup-or-identity-fallback semantics this free
        // function wraps.
        return pWndDlg->FindSiteOrWnd(reinterpret_cast<const COleControlSiteOrWnd*>(pWndCtrl));
    }
    return pWndCtrl;
}
// Symbol: ?afxData@@3UAUX_DATA@@A
// Exported data object, not a function. Real MFC `AUX_DATA` is sizeof 120 / align 8
// (LF_STRUCTURE `AUX_DATA`, uafxcw.amd64.pdb, MSVC 14.51 atlmfc -- the struct is not in
// any public header, so the PDB is the only type source). In the harvested mfc140 the
// RVA gap from ?afxData (0x3bc000) to the next exported datum ?wndTop@CWnd@@2V1@B
// (0x3bc0b0) is 176, a hard upper bound on the object; storage is sized to that bound so
// a client writing through the export can never run past it. Zero-initialised: this repo
// has no AUX_DATA definition, and the members the PDB does show (cxVScroll/cyHScroll/
// cxIcon/cyIcon/cxBorder2/cyBorder2/cxPixelsPerInch/cyPixelsPerInch at 0..28, then
// GDI brush/cursor/font/bitmap handles at 32..112) are cached system metrics and GDI
// objects that the real ctor fills in; reproducing that would mean calling GDI/USER32
// from a DLL-load static ctor, which is exactly what MFC documents it avoids.
extern "C" alignas(16) unsigned char MS_ABI impl__afxData__3UAUX_DATA__A[176] = {};
// Symbol: ?afxGlobalData@@3UAFX_GLOBAL_DATA@@A
// Exported data object. sizeof(AFX_GLOBAL_DATA) == 720, align 8 -- agreed by three
// independent sources: afxglobals.h (MSVC 14.51 atlmfc, measured with cl.exe), the
// LF_STRUCTURE record in uafxcw.amd64.pdb, and the mfc140 RVA gap 0x3ba380 -> 0x3ba650
// (?AFX_WM_ON_MOVETOTABGROUP@@3IA) == 720. Zero-initialised; AFX_GLOBAL_DATA's real
// ctor (system metrics, sys colours, GDI objects) has no counterpart in this repo.
extern "C" alignas(16) unsigned char MS_ABI impl__afxGlobalData__3UAFX_GLOBAL_DATA__A[720] = {};
// Symbol: ?afxGlobalUtils@@3VCGlobalUtils@@A
// Exported data object (an instance of CGlobalUtils, not a CRuntimeClass pointer).
// sizeof(CGlobalUtils) == 16, align 8 -- afxglobalutils.h via cl.exe, the PDB
// LF_STRUCTURE, and the mfc140 RVA gap 0x3aacf8 -> 0x3aad08 all give 16
// (vfptr at 0, BOOL m_bDialogApp at 8, BOOL m_bIsDragging at 12).
// Left zero-initialised, and it must stay that way: OpenMFC's own CGlobalUtils
// (include/openmfc/afxmfc.h) is 40 bytes, not 16, so placement-constructing one
// here -- e.g. via impl___0CGlobalUtils__QEAA_XZ, which does exist and is a real
// definition (core/runtime/CGlobalUtils.cpp) -- would overrun this ABI-correct
// storage by 24 bytes, and would publish an Itanium vptr at offset 0 where an
// MSVC client expects an MSVC vftable. Giving this object a live vptr needs a
// hand-authored MSVC vftable (the CFile approach), which is out of scope here.
extern "C" alignas(16) unsigned char MS_ABI impl__afxGlobalUtils__3VCGlobalUtils__A[16] = {};
// Symbol: ?afxKeyboardManager@@3PEAVCKeyboardManager@@EA
// Pointer-typed export (PEAV... = pointer to class): 8 bytes, matching the mfc140
// RVA gap to the next exported datum. Correctly sized already; left as a null CKeyboardManager*.
extern "C" void* MS_ABI impl__afxKeyboardManager__3PEAVCKeyboardManager__EA = {};
// Symbol: ?afxMenuHash@@3VCMenuHash@@A
// Exported data object (an instance of CMenuHash). sizeof(CMenuHash) == 72, align 8 in
// MSVC 14.51 (afxmenuhash.h via cl.exe and the PDB LF_STRUCTURE agree; it has no base
// class there -- vfptr at 0, CMap at 8, BOOL m_bIsActive at 64). The harvested mfc140
// RVA gap 0x3aad10 -> 0x3aad90 (?m_mapFloatingBars@CPaneFrameWnd) is 128, an upper
// bound only; storage is sized to that bound so it cannot be undersized for any
// toolset build of the DLL being cloned.
// Zero-initialised. A constructible CMenuHash *does* exist here
// (core/window/CMenuHash.cpp, also 72 bytes, so it would fit), but it is OpenMFC's own
// layout: CObject-derived with an opaque padding blob, so constructing into this
// storage would install an Itanium vptr at offset 0 where an MSVC client expects an
// MSVC vftable, and would put no member where m_bIsActive is read. Zero is the safer
// value -- a null vptr faults at the call rather than dispatching to the wrong slot.
extern "C" alignas(16) unsigned char MS_ABI impl__afxMenuHash__3VCMenuHash__A[128] = {};
// Symbol: ?afxShellManager@@3PEAVCShellManager@@EA
// Pointer-typed export (PEAV... = pointer to class): 8 bytes, matching the mfc140
// RVA gap to the next exported datum. Correctly sized already; left as a null CShellManager*.
extern "C" void* MS_ABI impl__afxShellManager__3PEAVCShellManager__EA = {};
// Symbol: ?afxUserToolsManager@@3PEAVCUserToolsManager@@EA
// Pointer-typed export (PEAV... = pointer to class): 8 bytes, matching the mfc140
// RVA gap to the next exported datum. Correctly sized already; left as a null CUserToolsManager*.
extern "C" void* MS_ABI impl__afxUserToolsManager__3PEAVCUserToolsManager__EA = {};
// Symbol: ?g_pTopLevelFrame@@3PEAVCFrameWnd@@EA
// Pointer-typed export (PEAV... = pointer to class): 8 bytes, matching the mfc140
// RVA gap to the next exported datum. Correctly sized already; left as a null CFrameWnd*.
extern "C" void* MS_ABI impl__g_pTopLevelFrame__3PEAVCFrameWnd__EA = {};
