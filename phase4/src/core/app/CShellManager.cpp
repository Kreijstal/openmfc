// CShellManager — OpenMFC implementation.
// Sources: cbarcore.cpp, mfccore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/CbarcoreSupport.h"
#include "detail/MfccoreSupport.h"

// Symbol: ?BrowseCallbackProc@CShellManager@@KAHPEAUHWND__@@I_J1@Z
extern "C" int MS_ABI impl__BrowseCallbackProc_CShellManager__KAHPEAUHWND____I_J1_Z(
    HWND hwnd, UINT msg, intptr_t, intptr_t data) {
    if (msg == BFFM_INITIALIZED && hwnd && data) {
        ::SendMessageW(hwnd, BFFM_SETSELECTIONW, TRUE, static_cast<LPARAM>(data));
    }
    return 0;
}
// Symbol: ?GetItemSize@CShellManager@@QEAAIPEFBU_ITEMIDLIST@@@Z
extern "C" UINT MS_ABI impl__GetItemSize_CShellManager__QEAAIPEFBU_ITEMIDLIST___Z(void*, const ITEMIDLIST* pidl) {
    if (!pidl) return 0;
    UINT size = sizeof(USHORT);
    const ITEMIDLIST* current = pidl;
    while (current->mkid.cb != 0) {
        size += current->mkid.cb;
        current = reinterpret_cast<const ITEMIDLIST*>(
            reinterpret_cast<const BYTE*>(current) + current->mkid.cb);
    }
    return size;
}
// Symbol: ?GetItemCount@CShellManager@@QEAAIPEFBU_ITEMIDLIST@@@Z
extern "C" UINT MS_ABI impl__GetItemCount_CShellManager__QEAAIPEFBU_ITEMIDLIST___Z(void*, const ITEMIDLIST* pidl) {
    UINT count = 0;
    for (const ITEMIDLIST* current = pidl; current && current->mkid.cb != 0;
         current = reinterpret_cast<const ITEMIDLIST*>(reinterpret_cast<const BYTE*>(current) + current->mkid.cb)) {
        ++count;
    }
    return count;
}
// Symbol: ?CreateItem@CShellManager@@QEAAPEFAU_ITEMIDLIST@@I@Z
extern "C" ITEMIDLIST* MS_ABI impl__CreateItem_CShellManager__QEAAPEFAU_ITEMIDLIST__I_Z(void*, UINT cb) {
    const UINT itemBytes = std::max<UINT>(cb, sizeof(USHORT));
    auto* pidl = static_cast<ITEMIDLIST*>(::CoTaskMemAlloc(itemBytes + sizeof(USHORT)));
    if (!pidl) return nullptr;
    memset(pidl, 0, itemBytes + sizeof(USHORT));
    pidl->mkid.cb = static_cast<USHORT>(std::min<UINT>(itemBytes, USHRT_MAX));
    return pidl;
}
// Symbol: ?CopyItem@CShellManager@@QEAAPEFAU_ITEMIDLIST@@PEFBU2@@Z
extern "C" ITEMIDLIST* MS_ABI impl__CopyItem_CShellManager__QEAAPEFAU_ITEMIDLIST__PEFBU2__Z(void* pThis, const ITEMIDLIST* pidl) {
    UINT size = impl__GetItemSize_CShellManager__QEAAIPEFBU_ITEMIDLIST___Z(pThis, pidl);
    if (size == 0) return nullptr;
    auto* copy = static_cast<ITEMIDLIST*>(::CoTaskMemAlloc(size));
    if (copy) memcpy(copy, pidl, size);
    return copy;
}
// Symbol: ?ConcatenateItem@CShellManager@@QEAAPEFAU_ITEMIDLIST@@PEFBU2@0@Z
extern "C" ITEMIDLIST* MS_ABI impl__ConcatenateItem_CShellManager__QEAAPEFAU_ITEMIDLIST__PEFBU2_0_Z(
    void* pThis, const ITEMIDLIST* left, const ITEMIDLIST* right) {
    const UINT leftSize = impl__GetItemSize_CShellManager__QEAAIPEFBU_ITEMIDLIST___Z(pThis, left);
    const UINT rightSize = impl__GetItemSize_CShellManager__QEAAIPEFBU_ITEMIDLIST___Z(pThis, right);
    if (leftSize == 0) return impl__CopyItem_CShellManager__QEAAPEFAU_ITEMIDLIST__PEFBU2__Z(pThis, right);
    if (rightSize == 0) return impl__CopyItem_CShellManager__QEAAPEFAU_ITEMIDLIST__PEFBU2__Z(pThis, left);
    auto* combined = static_cast<ITEMIDLIST*>(::CoTaskMemAlloc(leftSize + rightSize - sizeof(USHORT)));
    if (!combined) return nullptr;
    memcpy(combined, left, leftSize - sizeof(USHORT));
    memcpy(reinterpret_cast<BYTE*>(combined) + leftSize - sizeof(USHORT), right, rightSize);
    return combined;
}
// Symbol: ?FreeItem@CShellManager@@QEAAXPEFAU_ITEMIDLIST@@@Z
extern "C" void MS_ABI impl__FreeItem_CShellManager__QEAAXPEFAU_ITEMIDLIST___Z(void*, ITEMIDLIST* pidl) {
    ::CoTaskMemFree(pidl);
}
// Symbol: ?GetNextItem@CShellManager@@QEAAPEFAU_ITEMIDLIST@@PEFBU2@@Z
extern "C" ITEMIDLIST* MS_ABI impl__GetNextItem_CShellManager__QEAAPEFAU_ITEMIDLIST__PEFBU2__Z(void*, const ITEMIDLIST* pidl) {
    if (!pidl || pidl->mkid.cb == 0) return nullptr;
    auto* next = reinterpret_cast<ITEMIDLIST*>(const_cast<BYTE*>(reinterpret_cast<const BYTE*>(pidl)) + pidl->mkid.cb);
    return next->mkid.cb == 0 ? nullptr : next;
}
// Symbol: ?GetParentItem@CShellManager@@QEAAHPEFBU_ITEMIDLIST@@AEAPEFAU2@@Z
extern "C" int MS_ABI impl__GetParentItem_CShellManager__QEAAHPEFBU_ITEMIDLIST__AEAPEFAU2__Z(
    void*, const ITEMIDLIST* pidl, ITEMIDLIST** parent) {
    if (parent) *parent = nullptr;
    if (!pidl || !parent || pidl->mkid.cb == 0) return FALSE;
    const ITEMIDLIST* current = pidl;
    const ITEMIDLIST* last = nullptr;
    while (current->mkid.cb != 0) {
        last = current;
        current = reinterpret_cast<const ITEMIDLIST*>(reinterpret_cast<const BYTE*>(current) + current->mkid.cb);
    }
    const UINT bytesBeforeLast = static_cast<UINT>(reinterpret_cast<const BYTE*>(last) - reinterpret_cast<const BYTE*>(pidl));
    auto* result = static_cast<ITEMIDLIST*>(::CoTaskMemAlloc(bytesBeforeLast + sizeof(USHORT)));
    if (!result) return FALSE;
    memcpy(result, pidl, bytesBeforeLast);
    memset(reinterpret_cast<BYTE*>(result) + bytesBeforeLast, 0, sizeof(USHORT));
    *parent = result;
    return TRUE;
}
// Symbol: ?ItemFromPath@CShellManager@@QEAAJPEB_WAEAPEFAU_ITEMIDLIST@@@Z
extern "C" HRESULT MS_ABI impl__ItemFromPath_CShellManager__QEAAJPEB_WAEAPEFAU_ITEMIDLIST___Z(
    void*, const wchar_t* path, ITEMIDLIST** pidl) {
    if (pidl) *pidl = nullptr;
    if (!path || !pidl) return E_INVALIDARG;
    SFGAOF attributes = 0;
    return ::SHParseDisplayName(path, nullptr, reinterpret_cast<PIDLIST_ABSOLUTE*>(pidl), 0, &attributes);
}
// Symbol: ?BrowseForFolder@CShellManager@@QEAAHAEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@PEAVCWnd@@PEB_W2IPEAH@Z
extern "C" int MS_ABI impl__BrowseForFolder_CShellManager__QEAAHAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__PEAVCWnd__PEB_W2IPEAH_Z(
    void*, CString* outFolder, CWnd* parent, const wchar_t* title, const wchar_t* initialFolder, UINT flags, int* image) {
    wchar_t displayName[MAX_PATH] = {};
    BROWSEINFOW bi = {};
    bi.hwndOwner = parent ? parent->GetSafeHwnd() : nullptr;
    bi.pszDisplayName = displayName;
    bi.lpszTitle = title;
    bi.ulFlags = flags;
    bi.lpfn = reinterpret_cast<BFFCALLBACK>(impl__BrowseCallbackProc_CShellManager__KAHPEAUHWND____I_J1_Z);
    bi.lParam = reinterpret_cast<LPARAM>(initialFolder);
    bi.iImage = image ? *image : 0;
    PIDLIST_ABSOLUTE pidl = ::SHBrowseForFolderW(&bi);
    if (!pidl) return FALSE;
    wchar_t path[MAX_PATH] = {};
    BOOL ok = ::SHGetPathFromIDListW(pidl, path);
    if (ok && outFolder) *outFolder = path;
    if (image) *image = bi.iImage;
    ::CoTaskMemFree(pidl);
    return ok;
}
// Symbol: ??0CShellManager@@QEAA@XZ
extern "C" void* MS_ABI impl___0CShellManager__QEAA_XZ(CShellManager* pThis) {
    return pThis ? new(pThis) CShellManager() : nullptr;
}
// Symbol: ??1CShellManager@@UEAA@XZ
extern "C" void MS_ABI impl___1CShellManager__UEAA_XZ(CShellManager* pThis) {
    if (pThis) pThis->~CShellManager();
}
// Implemented in cbarcore.cpp; this unit had a second, unlinked copy.
// Symbol: ?BrowseCallbackProc@CShellManager@@KAHPEAUHWND__@@I_J1@Z
// Implemented in cbarcore.cpp; this unit had a second, unlinked copy.
// Symbol: ?BrowseForFolder@CShellManager@@QEAAHAEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@PEAVCWnd@@PEB_W2IPEAH@Z
// Implemented in cbarcore.cpp; this unit had a second, unlinked copy.
// Symbol: ?ConcatenateItem@CShellManager@@QEAAPEFAU_ITEMIDLIST@@PEFBU2@0@Z
// Implemented in cbarcore.cpp; this unit had a second, unlinked copy.
// Symbol: ?CopyItem@CShellManager@@QEAAPEFAU_ITEMIDLIST@@PEFBU2@@Z
// Implemented in cbarcore.cpp; this unit had a second, unlinked copy.
// Symbol: ?CreateItem@CShellManager@@QEAAPEFAU_ITEMIDLIST@@I@Z
// Implemented in cbarcore.cpp; this unit had a second, unlinked copy.
// Symbol: ?FreeItem@CShellManager@@QEAAXPEFAU_ITEMIDLIST@@@Z
// Implemented in cbarcore.cpp; this unit had a second, unlinked copy.
// Symbol: ?GetItemCount@CShellManager@@QEAAIPEFBU_ITEMIDLIST@@@Z
// Implemented in cbarcore.cpp; this unit had a second, unlinked copy.
// Symbol: ?GetItemSize@CShellManager@@QEAAIPEFBU_ITEMIDLIST@@@Z
// Implemented in cbarcore.cpp; this unit had a second, unlinked copy.
// Symbol: ?GetNextItem@CShellManager@@QEAAPEFAU_ITEMIDLIST@@PEFBU2@@Z
// Implemented in cbarcore.cpp; this unit had a second, unlinked copy.
// Symbol: ?GetParentItem@CShellManager@@QEAAHPEFBU_ITEMIDLIST@@AEAPEFAU2@@Z
// Implemented in cbarcore.cpp; this unit had a second, unlinked copy.
// Symbol: ?ItemFromPath@CShellManager@@QEAAJPEB_WAEAPEFAU_ITEMIDLIST@@@Z
int CALLBACK CShellManager::BrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM, LPARAM lpData) {
    if (uMsg == BFFM_INITIALIZED && lpData != 0) {
        ::SendMessageW(hwnd, BFFM_SETSELECTIONW, TRUE, lpData);
    }
    return 0;
}
BOOL CShellManager::BrowseForFolder(CString& strOutFolder, CWnd* pWndParent, const wchar_t* lpszTitle,
                                    const wchar_t* lpszInitialFolder, UINT ulFlags, int* piFolderImage) {
    using SHBrowseForFolderWFn = LPITEMIDLIST (WINAPI*)(LPBROWSEINFOW);
    using SHGetPathFromIDListWFn = BOOL (WINAPI*)(LPCITEMIDLIST, LPWSTR);

    auto browseForFolder = LoadShell32Function<SHBrowseForFolderWFn>("SHBrowseForFolderW");
    auto getPathFromIDList = LoadShell32Function<SHGetPathFromIDListWFn>("SHGetPathFromIDListW");
    if (!browseForFolder || !getPathFromIDList) return FALSE;

    wchar_t displayName[MAX_PATH] = {};
    BROWSEINFOW browseInfo = {};
    browseInfo.hwndOwner = pWndParent ? pWndParent->GetSafeHwnd() : nullptr;
    browseInfo.pszDisplayName = displayName;
    browseInfo.lpszTitle = lpszTitle;
    browseInfo.ulFlags = ulFlags;
    browseInfo.lpfn = &CShellManager::BrowseCallbackProc;
    browseInfo.lParam = reinterpret_cast<LPARAM>(lpszInitialFolder);
    browseInfo.iImage = 0;

    LPITEMIDLIST selected = browseForFolder(&browseInfo);
    if (!selected) return FALSE;

    wchar_t path[MAX_PATH] = {};
    BOOL ok = getPathFromIDList(selected, path);
    if (ok) strOutFolder = path;
    if (piFolderImage) *piFolderImage = browseInfo.iImage;
    FreeItem(selected);
    return ok;
}
LPITEMIDLIST CShellManager::CreateItem(UINT cbSize) {
    if (cbSize < sizeof(USHORT)) cbSize = sizeof(USHORT);
    LPITEMIDLIST pidl = AllocatePidlBytes(cbSize + sizeof(USHORT));
    if (pidl) pidl->mkid.cb = static_cast<USHORT>(cbSize);
    return pidl;
}
void CShellManager::FreeItem(LPITEMIDLIST pidl) {
    ::CoTaskMemFree(pidl);
}
UINT CShellManager::GetItemSize(LPCITEMIDLIST pidl) {
    if (!pidl) return 0;
    UINT total = 0;
    const BYTE* cursor = reinterpret_cast<const BYTE*>(pidl);
    for (;;) {
        const auto* item = reinterpret_cast<LPCITEMIDLIST>(cursor);
        total += sizeof(USHORT);
        if (item->mkid.cb == 0) break;
        total += item->mkid.cb - sizeof(USHORT);
        cursor += item->mkid.cb;
    }
    return total;
}
UINT CShellManager::GetItemCount(LPCITEMIDLIST pidl) {
    if (!pidl) return 0;
    UINT count = 0;
    const BYTE* cursor = reinterpret_cast<const BYTE*>(pidl);
    for (;;) {
        const auto* item = reinterpret_cast<LPCITEMIDLIST>(cursor);
        if (item->mkid.cb == 0) break;
        ++count;
        cursor += item->mkid.cb;
    }
    return count;
}
LPITEMIDLIST CShellManager::GetNextItem(LPCITEMIDLIST pidl) {
    if (!pidl || pidl->mkid.cb == 0) return nullptr;
    return reinterpret_cast<LPITEMIDLIST>(const_cast<BYTE*>(reinterpret_cast<const BYTE*>(pidl)) + pidl->mkid.cb);
}
LPITEMIDLIST CShellManager::CopyItem(LPCITEMIDLIST pidl) {
    UINT bytes = GetItemSize(pidl);
    if (bytes == 0) return nullptr;
    LPITEMIDLIST copy = AllocatePidlBytes(bytes);
    if (copy) std::memcpy(copy, pidl, bytes);
    return copy;
}
LPITEMIDLIST CShellManager::ConcatenateItem(LPCITEMIDLIST pidl1, LPCITEMIDLIST pidl2) {
    if (!pidl1) return CopyItem(pidl2);
    if (!pidl2) return CopyItem(pidl1);

    UINT size1 = GetItemSize(pidl1);
    UINT size2 = GetItemSize(pidl2);
    if (size1 == 0 || size2 == 0) return nullptr;

    UINT payload1 = size1 - sizeof(USHORT);
    LPITEMIDLIST combined = AllocatePidlBytes(payload1 + size2);
    if (!combined) return nullptr;

    std::memcpy(combined, pidl1, payload1);
    std::memcpy(reinterpret_cast<BYTE*>(combined) + payload1, pidl2, size2);
    return combined;
}
BOOL CShellManager::GetParentItem(LPCITEMIDLIST pidl, LPITEMIDLIST& pidlParent) {
    pidlParent = nullptr;
    if (!pidl || pidl->mkid.cb == 0) return FALSE;

    const BYTE* base = reinterpret_cast<const BYTE*>(pidl);
    const BYTE* cursor = base;
    const BYTE* last = base;
    while (reinterpret_cast<LPCITEMIDLIST>(cursor)->mkid.cb != 0) {
        last = cursor;
        cursor += reinterpret_cast<LPCITEMIDLIST>(cursor)->mkid.cb;
    }

    UINT parentBytes = static_cast<UINT>(last - base) + sizeof(USHORT);
    pidlParent = AllocatePidlBytes(parentBytes);
    if (!pidlParent) return FALSE;
    if (parentBytes > sizeof(USHORT)) {
        std::memcpy(pidlParent, pidl, parentBytes - sizeof(USHORT));
    }
    return TRUE;
}
HRESULT CShellManager::ItemFromPath(const wchar_t* lpszPath, LPITEMIDLIST& pidl) {
    pidl = nullptr;
    if (!lpszPath || *lpszPath == L'\0') return E_INVALIDARG;

    using SHParseDisplayNameFn = HRESULT (WINAPI*)(PCWSTR, IBindCtx*, PIDLIST_ABSOLUTE*, SFGAOF, SFGAOF*);
    auto parseDisplayName = LoadShell32Function<SHParseDisplayNameFn>("SHParseDisplayName");
    if (parseDisplayName) {
        PIDLIST_ABSOLUTE absolute = nullptr;
        HRESULT hr = parseDisplayName(lpszPath, nullptr, &absolute, 0, nullptr);
        if (SUCCEEDED(hr)) pidl = absolute;
        return hr;
    }

    PIDLIST_ABSOLUTE absolute = nullptr;
    IShellFolder* desktop = nullptr;
    HRESULT hr = ::SHGetDesktopFolder(&desktop);
    if (FAILED(hr)) return hr;

    SFGAOF attributes = 0;
    ULONG chEaten = 0;
    hr = desktop->ParseDisplayName(nullptr, nullptr, const_cast<LPWSTR>(lpszPath), &chEaten, &absolute, &attributes);
    desktop->Release();
    if (SUCCEEDED(hr)) pidl = absolute;
    return hr;
}
