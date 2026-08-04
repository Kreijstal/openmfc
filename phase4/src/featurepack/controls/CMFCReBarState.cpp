// CMFCReBarState — OpenMFC implementation.
// Sources: global_cmfcrebarstate.cpp

#include "detail/CMFCReBarStateSupport.h"

// Symbol: ?SaveRebarStateProc@CMFCReBarState@@CAHPEAUHWND__@@_J@Z
extern "C" int MS_ABI
impl__SaveRebarStateProc_CMFCReBarState__CAHPEAUHWND_____J_Z(HWND hwnd, LPARAM lParam)
{
    RebarCtx* ctx = reinterpret_cast<RebarCtx*>(lParam);
    if (ctx == nullptr || !IsRebar(hwnd))
        return TRUE; // keep enumerating

    UINT nBands = (UINT)::SendMessageW(hwnd, RB_GETBANDCOUNT, 0, 0);

    // Gather every band's persistent attributes in current visual order.
    BandRec* recs = nullptr;
    if (nBands)
        recs = (BandRec*)::LocalAlloc(LPTR, nBands * sizeof(BandRec));

    UINT nGot = 0;
    for (UINT i = 0; recs && i < nBands; ++i)
    {
        REBARBANDINFOW bi;
        ::ZeroMemory(&bi, sizeof(bi));
        bi.cbSize = sizeof(bi);
        bi.fMask  = RBBIM_STYLE | RBBIM_SIZE | RBBIM_IDEALSIZE |
                    RBBIM_CHILDSIZE | RBBIM_ID;
        if (!::SendMessageW(hwnd, RB_GETBANDINFOW, i, (LPARAM)&bi))
            continue;
        recs[nGot].fStyle     = bi.fStyle;
        recs[nGot].cx         = bi.cx;
        recs[nGot].cxIdeal    = bi.cxIdeal;
        recs[nGot].cxMinChild = bi.cxMinChild;
        recs[nGot].cyMinChild = bi.cyMinChild;
        recs[nGot].wID        = bi.wID;
        ++nGot;
    }

    // Blob: [UINT count][BandRec * count].
    DWORD cbBlob = (DWORD)(sizeof(UINT) + nGot * sizeof(BandRec));
    BYTE* blob = (BYTE*)::LocalAlloc(LPTR, cbBlob);
    if (blob)
    {
        *reinterpret_cast<UINT*>(blob) = nGot;
        if (nGot)
            ::memcpy(blob + sizeof(UINT), recs, nGot * sizeof(BandRec));

        wchar_t keyPath[512];
        BuildKeyPath(ctx->pszKey, keyPath, 512);

        HKEY hKey = nullptr;
        if (::RegCreateKeyExW(HKEY_CURRENT_USER, keyPath, 0, nullptr,
                              REG_OPTION_NON_VOLATILE, KEY_WRITE, nullptr,
                              &hKey, nullptr) == ERROR_SUCCESS)
        {
            wchar_t valName[32];
            wsprintfW(valName, L"ReBar%d", ctx->nIndex);
            ::RegSetValueExW(hKey, valName, 0, REG_BINARY, blob, cbBlob);
            ::RegCloseKey(hKey);
        }
        ::LocalFree(blob);
    }

    if (recs)
        ::LocalFree(recs);

    ctx->nIndex++;
    return TRUE;
}
// Symbol: ?LoadRebarStateProc@CMFCReBarState@@CAHPEAUHWND__@@_J@Z
extern "C" int MS_ABI
impl__LoadRebarStateProc_CMFCReBarState__CAHPEAUHWND_____J_Z(HWND hwnd, LPARAM lParam)
{
    RebarCtx* ctx = reinterpret_cast<RebarCtx*>(lParam);
    if (ctx == nullptr || !IsRebar(hwnd))
        return TRUE;

    wchar_t keyPath[512];
    BuildKeyPath(ctx->pszKey, keyPath, 512);

    HKEY hKey = nullptr;
    if (::RegOpenKeyExW(HKEY_CURRENT_USER, keyPath, 0, KEY_READ, &hKey)
            != ERROR_SUCCESS)
    {
        ctx->nIndex++;
        return TRUE; // nothing stored for this frame
    }

    wchar_t valName[32];
    wsprintfW(valName, L"ReBar%d", ctx->nIndex);

    DWORD cbBlob = 0, type = 0;
    LONG rc = ::RegQueryValueExW(hKey, valName, nullptr, &type, nullptr, &cbBlob);
    if (rc == ERROR_SUCCESS && type == REG_BINARY && cbBlob >= sizeof(UINT))
    {
        BYTE* blob = (BYTE*)::LocalAlloc(LPTR, cbBlob);
        if (blob &&
            ::RegQueryValueExW(hKey, valName, nullptr, &type, blob, &cbBlob)
                == ERROR_SUCCESS)
        {
            UINT nSaved = *reinterpret_cast<UINT*>(blob);
            // Clamp to the actual blob size to avoid overrun.
            UINT nMax = (UINT)((cbBlob - sizeof(UINT)) / sizeof(BandRec));
            if (nSaved > nMax)
                nSaved = nMax;
            const BandRec* recs =
                reinterpret_cast<const BandRec*>(blob + sizeof(UINT));

            for (UINT i = 0; i < nSaved; ++i)
            {
                const BandRec& r = recs[i];
                // Locate the band with this command id, then move it into the
                // saved position and restore its size/style attributes.
                int idx = (int)::SendMessageW(hwnd, RB_IDTOINDEX, r.wID, 0);
                if (idx >= 0 && (UINT)idx != i)
                {
                    ::SendMessageW(hwnd, RB_MOVEBAND, idx, (LPARAM)i);
                    idx = (int)i;
                }
                if (idx < 0)
                    continue;

                REBARBANDINFOW bi;
                ::ZeroMemory(&bi, sizeof(bi));
                bi.cbSize     = sizeof(bi);
                bi.fMask      = RBBIM_STYLE | RBBIM_SIZE | RBBIM_IDEALSIZE |
                                RBBIM_CHILDSIZE;
                bi.fStyle     = r.fStyle;
                bi.cx         = r.cx;
                bi.cxIdeal    = r.cxIdeal;
                bi.cxMinChild = r.cxMinChild;
                bi.cyMinChild = r.cyMinChild;
                ::SendMessageW(hwnd, RB_SETBANDINFOW, idx, (LPARAM)&bi);
            }
        }
        if (blob)
            ::LocalFree(blob);
    }

    ::RegCloseKey(hKey);
    ctx->nIndex++;
    return TRUE;
}
// Symbol: ?SaveState@CMFCReBarState@@SAXAEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@PEAVCFrameWnd@@@Z
extern "C" void MS_ABI
impl__SaveState_CMFCReBarState__SAXAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__PEAVCFrameWnd___Z(
    void* pStrKey, void* pFrame)
{
    if (pFrame == nullptr)
        return;
    HWND hwndFrame =
        *reinterpret_cast<HWND*>(reinterpret_cast<char*>(pFrame) + kCWndHWndOffset);
    if (hwndFrame == nullptr || !::IsWindow(hwndFrame))
        return;

    // CStringT<wchar_t> is a single pointer to its character buffer.
    const wchar_t* pszKey = nullptr;
    if (pStrKey)
        pszKey = *reinterpret_cast<const wchar_t* const*>(pStrKey);

    RebarCtx ctx = { pszKey, 0 };
    ::EnumChildWindows(
        hwndFrame,
        reinterpret_cast<WNDENUMPROC>(
            &impl__SaveRebarStateProc_CMFCReBarState__CAHPEAUHWND_____J_Z),
        reinterpret_cast<LPARAM>(&ctx));
}
// Symbol: ?LoadState@CMFCReBarState@@SAXAEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@PEAVCFrameWnd@@@Z
extern "C" void MS_ABI
impl__LoadState_CMFCReBarState__SAXAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__PEAVCFrameWnd___Z(
    void* pStrKey, void* pFrame)
{
    if (pFrame == nullptr)
        return;
    HWND hwndFrame =
        *reinterpret_cast<HWND*>(reinterpret_cast<char*>(pFrame) + kCWndHWndOffset);
    if (hwndFrame == nullptr || !::IsWindow(hwndFrame))
        return;

    const wchar_t* pszKey = nullptr;
    if (pStrKey)
        pszKey = *reinterpret_cast<const wchar_t* const*>(pStrKey);

    RebarCtx ctx = { pszKey, 0 };
    ::EnumChildWindows(
        hwndFrame,
        reinterpret_cast<WNDENUMPROC>(
            &impl__LoadRebarStateProc_CMFCReBarState__CAHPEAUHWND_____J_Z),
        reinterpret_cast<LPARAM>(&ctx));
}
