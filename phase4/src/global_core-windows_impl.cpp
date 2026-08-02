// CWnd core-window exports (wave 1: core-windows)
//
// Implements the remaining CWnd exports in the Windows/MSVC x64 ABI. Every
// function is a plain extern "C" ms_abi function with `this` passed as an
// explicit first parameter. Semantics are transcribed from the retail
// mfc140u.dll disassembly (see /tmp/openmfc_remaining).

#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxwin.h"
#include "openmfc/afxole.h"
#include "openmfc/afxmfc.h"
#include <windows.h>
#include <map>
#include <mutex>
#include <utility>

// MS ABI calling convention
#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

// ---------------------------------------------------------------------------
// Exported stubs used internally (avoid re-implementing)
// ---------------------------------------------------------------------------
extern "C" int MS_ABI impl__ModifyStyle_CWnd__SAHPEAUHWND____KKI_Z(HWND__* p0, unsigned long p1, unsigned long p2, unsigned int p3);
extern "C" int MS_ABI impl__ModifyStyleEx_CWnd__SAHPEAUHWND____KKI_Z(HWND__* p0, unsigned long p1, unsigned long p2, unsigned int p3);

// ---------------------------------------------------------------------------
// File-local per-window state
// ---------------------------------------------------------------------------
// Sibling implementations (wincore.cpp) keep per-window state in
// mutex-guarded std::map containers; follow the same pattern here for the two
// values that have no declared member in the (simplified) CWnd layout:
//   - SetProxy  stores the accessibility proxy (retail offset 0x58)
//   - EndModalLoop stores the modal result (retail offset 0xB8, m_nModalResult)
static std::mutex g_coreWndStateMutex;
static std::map<const CWnd*, void*> g_coreWndProxies;
static std::map<const CWnd*, int> g_coreWndModalResults;

// ===========================================================================
// CWnd
// ===========================================================================

// Symbol: ?AttachControlSite@CWnd@@QEAAXPEAV1@I@Z
extern "C" void MS_ABI impl__AttachControlSite_CWnd__QEAAXPEAV1_I_Z(
    CWnd* pThis, CWnd* pWnd, unsigned int nID)
{
    (void)nID;
    if (!pThis || !pWnd) {
        return;
    }
    // Retail: if pThis->m_pCtrlCont == NULL and pWnd owns a control
    // container, pThis adopts pWnd's container and attaches a control site
    // for nID. OpenMFC does not model OLE control containers (m_pCtrlCont is
    // not a declared member of the simplified CWnd layout), so there is no
    // container to share.
    // TODO(clean-room): transcribed partially
}

// Symbol: ?CalcWindowRect@CWnd@@UEAAXPEAUtagRECT@@I@Z
extern "C" void MS_ABI impl__CalcWindowRect_CWnd__UEAAXPEAUtagRECT__I_Z(
    CWnd* pThis, RECT* lpClientRect, unsigned int nAdjustType)
{
    if (!pThis || !lpClientRect) {
        return;
    }
    // Retail: style = GetStyle(); if (nAdjustType == adjustInside/0)
    // style &= ~0x200; AdjustWindowRectEx(rect, style, FALSE, GetExStyle()).
    DWORD dwStyle = pThis->GetStyle();
    if (nAdjustType == 0) {
        dwStyle &= ~0x200u;
    }
    ::AdjustWindowRectEx(lpClientRect, dwStyle, FALSE, pThis->GetExStyle());
}

// Symbol: ?CheckAutoCenter@CWnd@@UEAAHXZ
extern "C" int MS_ABI impl__CheckAutoCenter_CWnd__UEAAHXZ(CWnd* pThis)
{
    if (!pThis) {
        return FALSE;
    }
    return TRUE;
}

// Symbol: ?ContinueModal@CWnd@@UEAAHXZ
extern "C" int MS_ABI impl__ContinueModal_CWnd__UEAAHXZ(CWnd* pThis)
{
    if (!pThis) {
        return FALSE;
    }
    // Retail: return (m_nFlags & MF_MODALLOOP /* 0x10 */) != 0;
    return (pThis->m_nFlags & 0x10) ? TRUE : FALSE;
}

// Symbol: ?CreateAccessibleProxy@CWnd@@UEAAJ_K_JPEA_J@Z
extern "C" long MS_ABI impl__CreateAccessibleProxy_CWnd__UEAAJ_K_JPEA_J_Z(
    CWnd* pThis, unsigned __int64 dwProxy, __int64 riid, __int64* ppv)
{
    (void)riid;
    if (!pThis) {
        return 0x80004005;  // E_FAIL
    }
    if (ppv) {
        *ppv = 0;
    }
    // Retail requires dwProxy == 0xFFFFFFFC and then fabricates a COM
    // accessibility proxy (CoCreateInstance + QueryInterface). OpenMFC does
    // not host COM proxies, so report failure rather than fake success.
    // TODO(clean-room): transcribed partially
    (void)dwProxy;
    return 0x80004005;  // E_FAIL
}

// Symbol: ?CreateControl@CWnd@@QEAAHAEBVCControlCreationInfo@@KPEBUtagPOINT@@PEBUtagSIZE@@PEAV1@I@Z
extern "C" int MS_ABI impl__CreateControl_CWnd__QEAAHAEBVCControlCreationInfo__KPEBUtagPOINT__PEBUtagSIZE__PEAV1_I_Z(
    CWnd* pThis,
    const CControlCreationInfo* pInfo,  // const CControlCreationInfo&
    unsigned long dwStyle,
    const POINT* pPoint,
    const SIZE* pSize,
    CWnd* pParentWnd,
    unsigned int nID)
{
    if (!pThis) {
        return FALSE;
    }
    // Delegate to the existing C++ member (wincore.cpp), which handles the
    // CControlCreationInfo description and parent placement.
    const CControlCreationInfo* pInfoPtr = pInfo;
    void* parent = pParentWnd;
    return pThis->CreateControl(pInfoPtr, dwStyle, pPoint, pSize, parent, nID);
}

// Symbol: ?CreateControlSite@CWnd@@MEAAHPEAVCOleControlContainer@@PEAPEAVCOleControlSite@@IAEBU_GUID@@@Z
extern "C" int MS_ABI impl__CreateControlSite_CWnd__MEAAHPEAVCOleControlContainer__PEAPEAVCOleControlSite__IAEBU_GUID___Z(
    CWnd* pThis,
    COleControlContainer* pContainer,
    COleControlSite** ppSite,
    unsigned int nID,
    const GUID* pClsid)
{
    (void)pThis;
    (void)pContainer;
    (void)nID;
    (void)pClsid;
    // Retail (default CWnd::CreateControlSite):
    //   ENSURE_ARG(ppSite != NULL); *ppSite = NULL; return TRUE;
    // The ENSURE_ARG throw path is mapped to a conservative FALSE here.
    if (!ppSite) {
        return FALSE;
    }
    *ppSite = nullptr;
    return TRUE;
}

// Symbol: ?EndModalLoop@CWnd@@UEAAXH@Z
extern "C" void MS_ABI impl__EndModalLoop_CWnd__UEAAXH_Z(
    CWnd* pThis, int nResult)
{
    if (!pThis) {
        return;
    }
    // Retail: m_nModalResult = nResult; if (m_nFlags & MF_MODALLOOP) {
    //   m_nFlags &= ~MF_MODALLOOP; AfxCancelModes(m_hWnd); }
    {
        std::lock_guard<std::mutex> lk(g_coreWndStateMutex);
        g_coreWndModalResults[pThis] = nResult;
    }
    if (!(pThis->m_nFlags & 0x10)) {
        return;
    }
    pThis->m_nFlags &= ~0x10;
    if (pThis->m_hWnd) {
        // AfxCancelModes sends WM_CANCELMODE to the window to end modal state.
        ::SendMessageW(pThis->m_hWnd, WM_CANCELMODE, 0, 0);
    }
}

// Symbol: ?EnsureStdObj@CWnd@@UEAAJXZ
extern "C" long MS_ABI impl__EnsureStdObj_CWnd__UEAAJXZ(CWnd* pThis)
{
    if (!pThis) {
        return 0x80004005;  // E_FAIL
    }
    // Retail: if m_pStdObj != NULL return S_OK; else create the standard OLE
    // object via a helper and return its HRESULT. OpenMFC does not model the
    // standard-object slot (retail offset 0x50 is not a declared member of the
    // simplified CWnd layout), so treat the object as already present.
    // TODO(clean-room): transcribed partially
    return S_OK;  // 0
}

// Symbol: ?HtmlHelpW@CWnd@@UEAAX_KI@Z
extern "C" void MS_ABI impl__HtmlHelpW_CWnd__UEAAX_KI_Z(
    CWnd* pThis, unsigned long long data, unsigned int command)
{
    if (!pThis) {
        return;
    }
    CWinApp* pApp = AfxGetApp();
    if (!pApp || !pApp->m_pszHelpFilePath) {
        return;
    }
    HWND hwnd = pThis->m_hWnd;
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
            htmlHelp(hwnd, pApp->m_pszHelpFilePath, command, data);
        }
        if (owned) {
            ::FreeLibrary(hhModule);
        }
    }
}

// Symbol: ?InvokeHelper@CWnd@@QEAAXJGGPEAXPEBEZZ
extern "C" void MS_ABI impl__InvokeHelper_CWnd__QEAAXJGGPEAXPEBEZZ(
    CWnd* pThis, long dwDispID, unsigned short wFlags, unsigned short vtRet,
    void* pvRet, const unsigned char* pbParamInfo)
{
    (void)dwDispID;
    (void)wFlags;
    (void)vtRet;
    (void)pvRet;
    (void)pbParamInfo;
    if (!pThis) {
        return;
    }
    // Retail forwards the variadic IDispatch::Invoke to the OLE control
    // container (m_pCtrlCont at offset 0xd0). OpenMFC does not model control
    // containers, and the variadic tail cannot be forwarded safely.
    // TODO(clean-room): transcribed partially
}

// Symbol: ?ModifyStyle@CWnd@@QEAAHKKI@Z
extern "C" int MS_ABI impl__ModifyStyle_CWnd__QEAAHKKI_Z(
    CWnd* pThis, unsigned long dwRemove, unsigned long dwAdd, unsigned int nFlags)
{
    if (!pThis) {
        return FALSE;
    }
    // Retail: when there is no OLE control container, delegate to the static
    // CWnd::ModifyStyle(HWND, DWORD, DWORD, UINT); otherwise the container
    // handles style changes. OpenMFC has no control containers, so always use
    // the static helper.
    return impl__ModifyStyle_CWnd__SAHPEAUHWND____KKI_Z(
        pThis->m_hWnd, dwRemove, dwAdd, nFlags);
}

// Symbol: ?ModifyStyleEx@CWnd@@QEAAHKKI@Z
extern "C" int MS_ABI impl__ModifyStyleEx_CWnd__QEAAHKKI_Z(
    CWnd* pThis, unsigned long dwRemove, unsigned long dwAdd, unsigned int nFlags)
{
    if (!pThis) {
        return FALSE;
    }
    return impl__ModifyStyleEx_CWnd__SAHPEAUHWND____KKI_Z(
        pThis->m_hWnd, dwRemove, dwAdd, nFlags);
}

// Symbol: ?OnDevModeChange@CWnd@@IEAAXPEA_W@Z
extern "C" void MS_ABI impl__OnDevModeChange_CWnd__IEAAXPEA_W_Z(
    CWnd* pThis, wchar_t* lpszDeviceName)
{
    (void)lpszDeviceName;
    if (!pThis || !pThis->m_hWnd) {
        return;
    }
    // Retail: forwards the change to CWinApp when this is the main window and,
    // for hidden windows, recomputes text metrics and scrolls the client area.
    // Conservatively force a full repaint so the new mode is applied, matching
    // the sibling CWnd::OnDisplayChange behavior.
    // TODO(clean-room): transcribed partially
    ::InvalidateRect(pThis->m_hWnd, nullptr, TRUE);
    ::UpdateWindow(pThis->m_hWnd);
}

// Symbol: ?OnDrawIconicThumbnailOrLivePreview@CWnd@@UEAAXAEAVCDC@@VCRect@@VCSize@@HAEAH@Z
extern "C" void MS_ABI impl__OnDrawIconicThumbnailOrLivePreview_CWnd__UEAAXAEAVCDC__VCRect__VCSize__HAEAH_Z(
    CWnd* pThis, CDC* pDC, CRect rc, CSize sz, int bDwmEnabled, int* bUpdateDwm)
{
    (void)rc;
    (void)sz;
    (void)bDwmEnabled;
    (void)bUpdateDwm;
    if (!pThis || !pThis->m_hWnd) {
        return;
    }
    // Retail: SendMessage(m_hWnd, WM_PRINT, (WPARAM)pDC->m_hDC,
    //                     PRF_ERASEBKGND | PRF_NONCLIENT | PRF_CLIENT | PRF_CHILDREN);
    HDC hdc = pDC ? pDC->GetSafeHdc() : nullptr;
    ::SendMessageW(pThis->m_hWnd, WM_PRINT, reinterpret_cast<WPARAM>(hdc), 0x1E);
}

// Symbol: ?OnGesture@CWnd@@IEAA_J_K_J@Z
extern "C" __int64 MS_ABI impl__OnGesture_CWnd__IEAA_J_K_J_Z(
    CWnd* pThis, unsigned __int64 wParam, __int64 lParam)
{
    (void)wParam;
    (void)lParam;
    if (!pThis) {
        return 0;
    }
    // Retail lazily loads GetGestureInfo/CloseGestureInfoHandle, allocates a
    // per-window gesture buffer (offset 0x90, not declared in this layout),
    // decodes the gesture id and dispatches to the OnGesture* virtuals.
    // Return unhandled (0) like the default DefWindowProc path.
    // TODO(clean-room): transcribed partially
    return 0;
}

// Symbol: ?OnGesturePan@CWnd@@MEAAHVCPoint@@0@Z
extern "C" int MS_ABI impl__OnGesturePan_CWnd__MEAAHVCPoint__0_Z(
    CWnd* pThis, CPoint pt, CPoint pt2)
{
    (void)pt;
    (void)pt2;
    if (!pThis) {
        return FALSE;
    }
    return FALSE;  // default: not handled
}

// Symbol: ?OnGesturePressAndTap@CWnd@@MEAAHVCPoint@@J@Z
extern "C" int MS_ABI impl__OnGesturePressAndTap_CWnd__MEAAHVCPoint__J_Z(
    CWnd* pThis, CPoint pt, long dwExtraInfo)
{
    (void)pt;
    (void)dwExtraInfo;
    if (!pThis) {
        return FALSE;
    }
    return FALSE;  // default: not handled
}

// Symbol: ?OnGestureRotate@CWnd@@MEAAHVCPoint@@N@Z
extern "C" int MS_ABI impl__OnGestureRotate_CWnd__MEAAHVCPoint__N_Z(
    CWnd* pThis, CPoint pt, double dblRotate)
{
    (void)pt;
    (void)dblRotate;
    if (!pThis) {
        return FALSE;
    }
    return FALSE;  // default: not handled
}

// Symbol: ?OnGestureTwoFingerTap@CWnd@@MEAAHVCPoint@@@Z
extern "C" int MS_ABI impl__OnGestureTwoFingerTap_CWnd__MEAAHVCPoint___Z(
    CWnd* pThis, CPoint pt)
{
    (void)pt;
    if (!pThis) {
        return FALSE;
    }
    return FALSE;  // default: not handled
}

// Symbol: ?OnGestureZoom@CWnd@@MEAAHVCPoint@@J@Z
extern "C" int MS_ABI impl__OnGestureZoom_CWnd__MEAAHVCPoint__J_Z(
    CWnd* pThis, CPoint pt, long dwFactor)
{
    (void)pt;
    (void)dwFactor;
    if (!pThis) {
        return FALSE;
    }
    return FALSE;  // default: not handled
}

// Symbol: ?OnTouchInput@CWnd@@MEAAHVCPoint@@HHPEAUtagTOUCHINPUT@@@Z
extern "C" int MS_ABI impl__OnTouchInput_CWnd__MEAAHVCPoint__HHPEAUtagTOUCHINPUT___Z(
    CWnd* pThis, CPoint pt, int nInputNumber, int nTouchInputs, TOUCHINPUT* pInput)
{
    (void)pt;
    (void)nInputNumber;
    (void)nTouchInputs;
    (void)pInput;
    if (!pThis) {
        return FALSE;
    }
    return FALSE;  // default: not handled
}

// Symbol: ?OnTouchInputs@CWnd@@MEAAHIPEAUtagTOUCHINPUT@@@Z
extern "C" int MS_ABI impl__OnTouchInputs_CWnd__MEAAHIPEAUtagTOUCHINPUT___Z(
    CWnd* pThis, unsigned int nInputs, TOUCHINPUT* pInputs)
{
    if (!pThis) {
        return FALSE;
    }
    if (nInputs == 0 || !pInputs) {
        return TRUE;
    }
    // Retail: for each input, convert 1/100 pixel units to pixels, translate
    // to screen coordinates and dispatch to the virtual
    // OnTouchInput(i, nInputs, &pInputs[i]); abort (return FALSE) if a handler
    // rejects it. The virtual dispatch slot is not part of this layout, so the
    // per-input conversion is done and the call is skipped conservatively.
    // TODO(clean-room): transcribed partially
    for (unsigned int i = 0; i < nInputs; ++i) {
        POINT pt = {
            static_cast<LONG>(pInputs[i].x / 100),
            static_cast<LONG>(pInputs[i].y / 100)
        };
        if (pThis->m_hWnd) {
            ::ClientToScreen(pThis->m_hWnd, &pt);
        }
        (void)pt;
    }
    return TRUE;
}

// Symbol: ?RegisterTouchWindow@CWnd@@QEAAHHK@Z
extern "C" int MS_ABI impl__RegisterTouchWindow_CWnd__QEAAHHK_Z(
    CWnd* pThis, int bRegister, unsigned long dwFlags)
{
    if (!pThis || !pThis->m_hWnd) {
        return FALSE;
    }
    // Retail lazily loads user32!RegisterTouchWindow/UnregisterTouchWindow and
    // calls the matching one; the result is cached. The static imports are
    // equivalent on the supported OS versions.
    if (!bRegister) {
        return ::UnregisterTouchWindow(pThis->m_hWnd) ? TRUE : FALSE;
    }
    return ::RegisterTouchWindow(pThis->m_hWnd, dwFlags) ? TRUE : FALSE;
}

// Symbol: ?RemoveRadioCheckFromGroup@CWnd@@QEBAXPEBUCOleControlSiteOrWnd@@@Z
extern "C" void MS_ABI impl__RemoveRadioCheckFromGroup_CWnd__QEBAXPEBUCOleControlSiteOrWnd___Z(
    CWnd* pThis, const COleControlSiteOrWnd* pSiteOrWnd)
{
    (void)pSiteOrWnd;
    if (!pThis || !pThis->m_hWnd) {
        return;
    }
    // Retail walks the group of radio buttons (starting from the OLE control
    // site or window) and clears their checked state. Without OLE control
    // containers, mirror the sibling CWnd::RemoveRadioCheckFromGroup(const
    // CObject*) behavior: clear every auto-radio child button.
    for (HWND hChild = ::GetWindow(pThis->m_hWnd, GW_CHILD);
         hChild;
         hChild = ::GetWindow(hChild, GW_HWNDNEXT)) {
        DWORD style = static_cast<DWORD>(::GetWindowLongPtrW(hChild, GWL_STYLE));
        if (style & BS_AUTORADIOBUTTON) {
            ::SendMessageW(hChild, BM_SETCHECK, BST_UNCHECKED, 0);
        }
    }
}

// Symbol: ?RepositionBars@CWnd@@QEAAXIIIIPEAUtagRECT@@PEBU2@H@Z
extern "C" void MS_ABI impl__RepositionBars_CWnd__QEAAXIIIIPEAUtagRECT__PEBU2_H_Z(
    CWnd* pThis, unsigned int nIDFirst, unsigned int nIDLast,
    unsigned int nIDLeftOver, unsigned int nFlag, RECT* lpRectClient,
    const RECT* lpRectClient2, int bStretch)
{
    if (!pThis) {
        return;
    }
    pThis->RepositionBars(nIDFirst, nIDLast, nIDLeftOver, nFlag,
                          lpRectClient, const_cast<RECT*>(lpRectClient2), bStretch);
}

// Symbol: ?ScrollWindow@CWnd@@QEAAXHHPEBUtagRECT@@0@Z
extern "C" void MS_ABI impl__ScrollWindow_CWnd__QEAAXHHPEBUtagRECT__0_Z(
    CWnd* pThis, int X, int Y, const RECT* lpRectScroll, const RECT* lpRectClip)
{
    if (!pThis) {
        return;
    }
    pThis->ScrollWindow(X, Y, lpRectScroll, lpRectClip);
}

// Symbol: ?ScrollWindowEx@CWnd@@QEAAHHHPEBUtagRECT@@0PEAVCRgn@@PEAU2@I@Z
extern "C" int MS_ABI impl__ScrollWindowEx_CWnd__QEAAHHHPEBUtagRECT__0PEAVCRgn__PEAU2_I_Z(
    CWnd* pThis, int X, int Y, const RECT* lpRectScroll, const RECT* lpRectClip,
    CRgn* prgnUpdate, RECT* lpRectUpdate, unsigned int uiFlags)
{
    if (!pThis) {
        return FALSE;
    }
    return pThis->ScrollWindowEx(X, Y, lpRectScroll, lpRectClip,
                                 prgnUpdate, lpRectUpdate, uiFlags);
}

// Symbol: ?SetProperty@CWnd@@QEAAXJGZZ
extern "C" void MS_ABI impl__SetProperty_CWnd__QEAAXJGZZ(
    CWnd* pThis, long dwDispID, unsigned short vtProp)
{
    (void)dwDispID;
    (void)vtProp;
    if (!pThis) {
        return;
    }
    // Retail forwards the variadic property assignment to the OLE control
    // container (m_pCtrlCont at offset 0xd0). OpenMFC does not model control
    // containers, and the variadic value cannot be forwarded safely.
    // TODO(clean-room): transcribed partially
}

// Symbol: ?SetProxy@CWnd@@UEAAJPEAUIAccessibleProxy@@@Z
extern "C" long MS_ABI impl__SetProxy_CWnd__UEAAJPEAUIAccessibleProxy___Z(
    CWnd* pThis, void* pProxy)
{
    if (!pThis) {
        return 0x80004005;  // E_FAIL
    }
    // Retail: m_pStdObj (offset 0x58) = pProxy; return S_OK. The offset has no
    // declared member in the simplified CWnd layout, so keep the proxy in the
    // per-window runtime state.
    {
        std::lock_guard<std::mutex> lk(g_coreWndStateMutex);
        g_coreWndProxies[pThis] = pProxy;
    }
    return S_OK;  // 0
}
