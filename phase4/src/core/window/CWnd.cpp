// CWnd — OpenMFC implementation.
// Sources: dlgcore.cpp, global_core-windows_impl.cpp, global_ctor_quickwins.cpp, menucore.cpp, wincore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/DlgcoreSupport.h"
#include "detail/CWndSupport.h"
#include "detail/CMenuSupport.h"
#include "detail/WincoreSupport.h"

// Hand-written real implementations for header-defined classes that were one
// export short of complete. Constructors placement-new into the caller-provided
// storage so the openmfc C++ constructor establishes the validated member
// layout. Kept in a global_*.cpp file so the build's AUTO_EXCLUDES pass (scan of
// `// Symbol:` comments) removes the weak/typed stubs for these ordinals
// automatically.

#include "openmfc/afxwin.h"
#include "openmfc/afxinet.h"
#include "openmfc/afxdisp.h"
#include "openmfc/afxole.h"

#include <cstdio>
#include <new>
#include <cstdlib>
#include <cstring>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

// Runtime-class descriptors used by COleException::Process, defined alongside
// the exception RTTI tables (ole_oleexception_rtti.cpp,
// global_simple_exceptions_rtti.cpp).
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_COleException__SAPEAUCRuntimeClass__XZ();
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMemoryException__SAPEAUCRuntimeClass__XZ();
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CNotSupportedException__SAPEAUCRuntimeClass__XZ();
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CInvalidArgException__SAPEAUCRuntimeClass__XZ();

extern "C" void MS_ABI impl__CommonInit_CFile__IEAAXPEB_WIPEAVCAtlTransactionManager_ATL___Z(
    void* pThis, const wchar_t* lpszFileName, unsigned int nOpenFlags, void* pTM);
extern "C" void MS_ABI impl__CommonBaseInit_CStdioFile__IEAAXPEAU_iobuf__PEAVCAtlTransactionManager_ATL___Z(
    void* pThis, FILE* pStream, void* pTM);
extern "C" void MS_ABI impl__CommonInit_CStdioFile__IEAAXPEB_WIPEAVCAtlTransactionManager_ATL___Z(
    void* pThis, const wchar_t* lpszFileName, unsigned int nOpenFlags, void* pTM);















// CWnd::GetDlgItem
// Symbol: ?GetDlgItem@CWnd@@QEBAPEAV1@H@Z
extern "C" CWnd* MS_ABI impl__GetDlgItem_CWnd__QEBAPEAV1_H_Z(const CWnd* pThis, int nID) {
    if (!pThis || !pThis->m_hWnd) {
        return nullptr;
    }

    HWND hCtrl = ::GetDlgItem(pThis->m_hWnd, nID);
    if (!hCtrl) {
        return nullptr;
    }

    return OpenMfcAttachCWnd(hCtrl);
}
// CWnd::GetDlgItem (HWND** overload)
// Symbol: ?GetDlgItem@CWnd@@QEBAXHPEAPEAUHWND__@@@Z
extern "C" void MS_ABI impl__GetDlgItem_CWnd__QEBAXHPEAPEAUHWND_____Z(
    const CWnd* pThis, int nID, HWND* pWnd) {
    if (!pWnd) {
        return;
    }
    *pWnd = nullptr;
    if (!pThis || !pThis->m_hWnd) {
        return;
    }
    *pWnd = ::GetDlgItem(pThis->m_hWnd, nID);
}
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
// CWnd::CWnd(HWND) — private constructor MFC uses internally to wrap an existing
// window handle. Default-initializes the CWnd members (validated layout) and
// then binds the handle, matching the observable state of the real ctor.
// Symbol: ??0CWnd@@AEAA@PEAUHWND__@@@Z
extern "C" void* MS_ABI impl___0CWnd__AEAA_PEAUHWND_____Z(void* pThis, HWND hWnd)
{
    CWnd* p = new (pThis) CWnd();
    p->m_hWnd = hWnd;
    return p;
}
// CWnd::GetMenu
// Symbol: ?GetMenu@CWnd@@UEBAPEAVCMenu@@XZ
// Ordinal: 5723
extern "C" CMenu* MS_ABI impl__GetMenu_CWnd__UEBAPEAVCMenu__XZ(const CWnd* pThis) {
    if (!pThis || !pThis->m_hWnd) {
        return nullptr;
    }
    HMENU hMenu = ::GetMenu(pThis->m_hWnd);
    return FromHandleTemp(hMenu);
}
// CWnd::SetMenu
// Symbol: ?SetMenu@CWnd@@UEAAHPEAVCMenu@@@Z
// Ordinal: 13354
extern "C" int MS_ABI impl__SetMenu_CWnd__UEAAHPEAVCMenu___Z(CWnd* pThis, CMenu* pMenu) {
    if (!pThis || !pThis->m_hWnd) return FALSE;
    HMENU hMenu = pMenu ? pMenu->m_hMenu : nullptr;
    return ::SetMenu(pThis->m_hWnd, hMenu);
}
// Symbol: ?wndBottom@CWnd@@2V1@B
extern "C" const StaticCWndExport MS_ABI impl__wndBottom_CWnd__2V1_B = {
    nullptr, {}, HWND_BOTTOM, {}
};
// Symbol: ?wndNoTopMost@CWnd@@2V1@B
extern "C" const StaticCWndExport MS_ABI impl__wndNoTopMost_CWnd__2V1_B = {
    nullptr, {}, HWND_NOTOPMOST, {}
};
// Symbol: ?wndTop@CWnd@@2V1@B
extern "C" const StaticCWndExport MS_ABI impl__wndTop_CWnd__2V1_B = {
    nullptr, {}, HWND_TOP, {}
};
// Symbol: ?wndTopMost@CWnd@@2V1@B
extern "C" const StaticCWndExport MS_ABI impl__wndTopMost_CWnd__2V1_B = {
    nullptr, {}, HWND_TOPMOST, {}
};
// Symbol: ?FromHandle@CWnd@@SAPEAV1@PEAUHWND__@@@Z
extern "C" CWnd* MS_ABI impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(HWND hWnd) {
    return OpenMfcAttachCWnd(hWnd);
}
// Symbol: ?GetThisClass@CWnd@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CWnd__SAPEAUCRuntimeClass__XZ() {
    return &CWnd::classCWnd;
}
// Symbol: ?GetRuntimeClass@CWnd@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CWnd__UEBAPEAUCRuntimeClass__XZ(const CWnd* pThis) {
    return CWnd::GetThisClass();
}
// Symbol: ?Create@CWnd@@UEAAHPEB_W0KAEBUtagRECT@@PEAV1@IPEAUCCreateContext@@@Z
extern "C" int MS_ABI impl__Create_CWnd__UEAAHPEB_W0KAEBUtagRECT__PEAV1_IPEAUCCreateContext___Z(
    CWnd* pThis,
    const wchar_t* lpszClassName,
    const wchar_t* lpszWindowName,
    DWORD dwStyle,
    const RECT& rect,
    CWnd* pParentWnd,
    UINT nID,
    CCreateContext* pContext)
{
    (void)pContext;

    HINSTANCE hInst = AfxGetInstanceHandle();
    if (!hInst) {
        hInst = GetModuleHandle(nullptr);
    }

    // Register window class if not specified
    const wchar_t* className = lpszClassName;
    if (!className || className[0] == 0) {
        if (!g_atomOpenMFCClass) {
            g_atomOpenMFCClass = RegisterOpenMFCClass(hInst);
        }
        className = g_szOpenMFCClass;
    }

    // Call PreCreateWindow
    CREATESTRUCTW cs = {};
    cs.lpszClass = className;
    cs.lpszName = lpszWindowName;
    cs.style = dwStyle;
    cs.x = rect.left;
    cs.y = rect.top;
    cs.cx = rect.right - rect.left;
    cs.cy = rect.bottom - rect.top;
    cs.hwndParent = pParentWnd ? pParentWnd->m_hWnd : nullptr;
    cs.hMenu = (HMENU)(UINT_PTR)nID;
    cs.hInstance = hInst;

    // Avoid virtual dispatch: our exported methods are ABI-compatible entrypoints,
    // but we do not rely on any MSVC vtable layout in this phase.
    if (!impl__PreCreateWindow_CWnd__UEAAHAEAUtagCREATESTRUCTW___Z(pThis, cs)) {
        return FALSE;
    }

    // Create the window
    HWND hWnd = CreateWindowExW(
        cs.dwExStyle,
        cs.lpszClass,
        cs.lpszName,
        cs.style,
        cs.x, cs.y, cs.cx, cs.cy,
        cs.hwndParent,
        cs.hMenu,
        cs.hInstance,
        pThis  // Pass CWnd* as creation parameter
    );

    if (!hWnd) {
        return FALSE;
    }

    pThis->m_hWnd = hWnd;
    g_hwndMap[hWnd] = pThis;

    return TRUE;
}
// Symbol: ?CreateEx@CWnd@@UEAAHKPEB_W0KHHHHPEAUHWND__@@PEAUHMENU__@@PEAX@Z
extern "C" int MS_ABI impl__CreateEx_CWnd__UEAAHKPEB_W0KHHHHPEAUHWND____PEAUHMENU____PEAX_Z(
    CWnd* pThis,
    DWORD dwExStyle,
    const wchar_t* lpszClassName,
    const wchar_t* lpszWindowName,
    DWORD dwStyle,
    int x,
    int y,
    int nWidth,
    int nHeight,
    HWND hWndParent,
    HMENU nIDorHMenu,
    void* lpParam)
{
    if (!pThis) {
        return FALSE;
    }

    HINSTANCE hInst = AfxGetInstanceHandle();
    if (!hInst) {
        hInst = GetModuleHandle(nullptr);
    }

    const wchar_t* className = lpszClassName;
    if (!className || className[0] == 0) {
        if (!g_atomOpenMFCClass) {
            g_atomOpenMFCClass = RegisterOpenMFCClass(hInst);
        }
        className = g_szOpenMFCClass;
    }

    CREATESTRUCTW cs = {};
    cs.dwExStyle = dwExStyle;
    cs.lpszClass = className;
    cs.lpszName = lpszWindowName;
    cs.style = dwStyle;
    cs.x = x;
    cs.y = y;
    cs.cx = nWidth;
    cs.cy = nHeight;
    cs.hwndParent = hWndParent;
    cs.hMenu = nIDorHMenu;
    cs.hInstance = hInst;
    cs.lpCreateParams = lpParam;

    if (!impl__PreCreateWindow_CWnd__UEAAHAEAUtagCREATESTRUCTW___Z(pThis, cs)) {
        return FALSE;
    }

    HWND hWnd = CreateWindowExW(
        cs.dwExStyle, cs.lpszClass, cs.lpszName, cs.style,
        cs.x, cs.y, cs.cx, cs.cy, cs.hwndParent, cs.hMenu,
        cs.hInstance, cs.lpCreateParams ? cs.lpCreateParams : pThis);
    if (!hWnd) {
        return FALSE;
    }

    pThis->m_hWnd = hWnd;
    g_hwndMap[hWnd] = pThis;
    return TRUE;
}
// Symbol: ?CreateEx@CWnd@@UEAAHKPEB_W0KAEBUtagRECT@@PEAV1@IPEAX@Z
extern "C" int MS_ABI impl__CreateEx_CWnd__UEAAHKPEB_W0KAEBUtagRECT__PEAV1_IPEAX_Z(
    CWnd* pThis,
    DWORD dwExStyle,
    const wchar_t* lpszClassName,
    const wchar_t* lpszWindowName,
    DWORD dwStyle,
    const RECT& rect,
    CWnd* pParentWnd,
    UINT nID,
    void* lpParam)
{
    return impl__CreateEx_CWnd__UEAAHKPEB_W0KHHHHPEAUHWND____PEAUHMENU____PEAX_Z(
        pThis,
        dwExStyle,
        lpszClassName,
        lpszWindowName,
        dwStyle,
        rect.left,
        rect.top,
        rect.right - rect.left,
        rect.bottom - rect.top,
        pParentWnd ? pParentWnd->m_hWnd : nullptr,
        reinterpret_cast<HMENU>(static_cast<UINT_PTR>(nID)),
        lpParam);
}
// Symbol: ?ShowWindow@CWnd@@QEAAHH@Z
extern "C" int MS_ABI impl__ShowWindow_CWnd__QEAAHH_Z(CWnd* pThis, int nCmdShow) {
    if (!pThis || !pThis->m_hWnd) {
        return FALSE;
    }
    return ::ShowWindow(pThis->m_hWnd, nCmdShow);
}
// CWnd::EnableWindow
// Symbol: ?EnableWindow@CWnd@@QEAAHH@Z
extern "C" int MS_ABI impl__EnableWindow_CWnd__QEAAHH_Z(CWnd* pThis, int bEnable) {
    if (!pThis || !pThis->m_hWnd) {
        return FALSE;
    }
    return ::EnableWindow(pThis->m_hWnd, bEnable);
}
// CWnd::IsWindowEnabled
// Symbol: ?IsWindowEnabled@CWnd@@QEBAHXZ
extern "C" int MS_ABI impl__IsWindowEnabled_CWnd__QEBAHXZ(const CWnd* pThis) {
    if (!pThis || !pThis->m_hWnd) {
        return FALSE;
    }
    return ::IsWindowEnabled(pThis->m_hWnd);
}
// Symbol: ?DestroyWindow@CWnd@@UEAAHXZ
extern "C" int MS_ABI impl__DestroyWindow_CWnd__UEAAHXZ(CWnd* pThis) {
    if (!pThis || !pThis->m_hWnd) {
        return FALSE;
    }

    HWND hWnd = pThis->m_hWnd;
    g_hwndMap.erase(hWnd);
    pThis->m_hWnd = nullptr;

    return ::DestroyWindow(hWnd);
}
// Symbol: ?PreCreateWindow@CWnd@@UEAAHAEAUtagCREATESTRUCTW@@@Z
extern "C" int MS_ABI impl__PreCreateWindow_CWnd__UEAAHAEAUtagCREATESTRUCTW___Z(
    CWnd* pThis, CREATESTRUCTW& cs)
{
    if (!pThis) {
        return FALSE;
    }

    if (!cs.lpszClass || cs.lpszClass[0] == 0) {
        if (!g_atomOpenMFCClass) {
            HINSTANCE hInst = AfxGetInstanceHandle();
            if (!hInst) {
                hInst = GetModuleHandle(nullptr);
            }
            if (hInst) {
                g_atomOpenMFCClass = RegisterOpenMFCClass(hInst);
            }
        }
        cs.lpszClass = g_szOpenMFCClass;
    }
    if (cs.style == 0) {
        cs.style = WS_OVERLAPPEDWINDOW;
    }
    if (cs.hInstance == nullptr) {
        cs.hInstance = AfxGetInstanceHandle();
        if (!cs.hInstance) {
            cs.hInstance = GetModuleHandle(nullptr);
        }
    }
    return TRUE;
}
// Symbol: ?DefWindowProcW@CWnd@@MEAA_JI_K_J@Z
extern "C" LRESULT MS_ABI impl__DefWindowProcW_CWnd__MEAA_JI_K_J_Z(
    CWnd* pThis, UINT message, WPARAM wParam, LPARAM lParam)
{
    if (pThis && pThis->m_hWnd) {
        return ::DefWindowProcW(pThis->m_hWnd, message, wParam, lParam);
    }
    return 0;
}
// Symbol: ?WindowProc@CWnd@@MEAA_JI_K_J@Z
extern "C" LRESULT MS_ABI impl__WindowProc_CWnd__MEAA_JI_K_J_Z(
    CWnd* pThis, UINT message, WPARAM wParam, LPARAM lParam)
{
    if (!pThis || !pThis->m_hWnd) {
        return 0;
    }

    LRESULT result = 0;
    if (impl__OnWndMsg_CWnd__MEAAHI_K_JPEA_J_Z(pThis, message, wParam, lParam, &result)) {
        return result;
    }
    return impl__DefWindowProcW_CWnd__MEAA_JI_K_J_Z(pThis, message, wParam, lParam);
}
// CWnd::MoveWindow
// Symbol: ?MoveWindow@CWnd@@QEAAXHHHHH@Z
extern "C" void MS_ABI impl__MoveWindow_CWnd__QEAAXHHHHH_Z(
    CWnd* pThis, int x, int y, int nWidth, int nHeight, int bRepaint) {
    if (pThis && pThis->m_hWnd) {
        ::MoveWindow(pThis->m_hWnd, x, y, nWidth, nHeight, bRepaint);
    }
}
// CWnd::SetWindowPos
// Symbol: ?SetWindowPos@CWnd@@QEAAHPEBV1@HHHHI@Z
extern "C" int MS_ABI impl__SetWindowPos_CWnd__QEAAHPEBV1_HHHHI_Z(
    CWnd* pThis, const CWnd* pWndInsertAfter, int x, int y, int cx, int cy, unsigned int nFlags) {
    if (!pThis || !pThis->m_hWnd) {
        return FALSE;
    }
    HWND hInsert = pWndInsertAfter ? pWndInsertAfter->m_hWnd : nullptr;
    return ::SetWindowPos(pThis->m_hWnd, hInsert, x, y, cx, cy, nFlags);
}
// CWnd::SetWindowTextW
// Symbol: ?SetWindowTextW@CWnd@@QEAAXPEB_W@Z
extern "C" void MS_ABI impl__SetWindowTextW_CWnd__QEAAXPEB_W_Z(
    CWnd* pThis, const wchar_t* lpszString) {
    if (pThis && pThis->m_hWnd) {
        ::SetWindowTextW(pThis->m_hWnd, lpszString ? lpszString : L"");
    }
}
// CWnd::GetWindowTextW
// Symbol: ?GetWindowTextW@CWnd@@QEBAHPEA_WH@Z
extern "C" int MS_ABI impl__GetWindowTextW_CWnd__QEBAHPEA_WH_Z(
    const CWnd* pThis, wchar_t* lpszStringBuf, int nMaxCount) {
    if (!pThis || !pThis->m_hWnd || !lpszStringBuf || nMaxCount <= 0) {
        if (lpszStringBuf && nMaxCount > 0) {
            lpszStringBuf[0] = L'\0';
        }
        return 0;
    }
    return ::GetWindowTextW(pThis->m_hWnd, lpszStringBuf, nMaxCount);
}
// CWnd::GetWindowTextW (CString& overload)
// Symbol: ?GetWindowTextW@CWnd@@QEBAXAEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@@Z
extern "C" void MS_ABI impl__GetWindowTextW_CWnd__QEBAXAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
    const CWnd* pThis, CString* rString) {
    if (!rString) {
        return;
    }
    if (!pThis || !pThis->m_hWnd) {
        rString->Empty();
        return;
    }
    int length = ::GetWindowTextLengthW(pThis->m_hWnd);
    if (length <= 0) {
        rString->Empty();
        return;
    }
    // GetBuffer(length + 1) to accommodate null terminator for GetWindowTextW
    wchar_t* buffer = rString->GetBuffer(length + 1);
    int actual = ::GetWindowTextW(pThis->m_hWnd, buffer, length + 1);
    rString->ReleaseBuffer(actual);
}
// CWnd::GetWindowTextLengthW
// Symbol: ?GetWindowTextLengthW@CWnd@@QEBAHXZ
extern "C" int MS_ABI impl__GetWindowTextLengthW_CWnd__QEBAHXZ(const CWnd* pThis) {
    if (!pThis || !pThis->m_hWnd) {
        return 0;
    }
    return ::GetWindowTextLengthW(pThis->m_hWnd);
}
// Symbol: ?FromHandlePermanent@CWnd@@SAPEAV1@PEAUHWND__@@@Z
extern "C" CWnd* MS_ABI impl__FromHandlePermanent_CWnd__SAPEAV1_PEAUHWND_____Z(HWND hWnd) {
    auto it = g_hwndMap.find(hWnd);
    return it != g_hwndMap.end() ? it->second : nullptr;
}
// Symbol: ?GetDescendantWindow@CWnd@@SAPEAV1@PEAUHWND__@@HH@Z
extern "C" CWnd* MS_ABI impl__GetDescendantWindow_CWnd__SAPEAV1_PEAUHWND____HH_Z(
    HWND hWnd, int nID, int bOnlyPerm) {
    HWND hChild = hWnd ? ::GetDlgItem(hWnd, nID) : nullptr;
    if (!hChild) {
        return nullptr;
    }
    if (bOnlyPerm) {
        return impl__FromHandlePermanent_CWnd__SAPEAV1_PEAUHWND_____Z(hChild);
    }
    return CWnd::FromHandle(hChild);
}
// Symbol: ?GetSafeOwner@CWnd@@SAPEAV1@PEAV1@PEAPEAUHWND__@@@Z
extern "C" CWnd* MS_ABI impl__GetSafeOwner_CWnd__SAPEAV1_PEAV1_PEAPEAUHWND_____Z(
    CWnd* pParent, HWND* pWndTop) {
    HWND hOwner = pParent ? pParent->m_hWnd : nullptr;
    if (!hOwner) {
        CWinThread* pThread = AfxGetThread();
        hOwner = pThread && pThread->m_pMainWnd ? pThread->m_pMainWnd->m_hWnd : nullptr;
    }
    if (pWndTop) {
        *pWndTop = hOwner;
    }
    return hOwner ? CWnd::FromHandle(hOwner) : nullptr;
}
// Symbol: ?GetSafeOwner_@CWnd@@SAPEAUHWND__@@PEAU2@PEAPEAU2@@Z
extern "C" HWND MS_ABI impl__GetSafeOwner__CWnd__SAPEAUHWND____PEAU2_PEAPEAU2__Z(
    HWND hParent, HWND* pWndTop) {
    HWND hOwner = hParent;
    if (!hOwner) {
        CWinThread* pThread = AfxGetThread();
        hOwner = pThread && pThread->m_pMainWnd ? pThread->m_pMainWnd->m_hWnd : nullptr;
    }
    if (pWndTop) {
        *pWndTop = hOwner;
    }
    return hOwner;
}
// Symbol: ?GetThisMessageMap@CWnd@@KAPEBUAFX_MSGMAP@@XZ
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CWnd__KAPEBUAFX_MSGMAP__XZ() {
    return CWnd::GetThisMessageMap();
}
// The exported virtual GetMessageMap was a weak stub returning null; give it the
// same base-chained map so a real client's CWnd routing does not null-deref.
// Symbol: ?GetMessageMap@CWnd@@MEBAPEBUAFX_MSGMAP@@XZ
extern "C" const AFX_MSGMAP* MS_ABI impl__GetMessageMap_CWnd__MEBAPEBUAFX_MSGMAP__XZ(const void*)
{
    return &g_cwndMessageMap;
}
// Symbol: ?CreateControl@CWnd@@QEAAHAEBU_GUID@@PEB_WKAEBUtagRECT@@PEAV1@IPEAVCFile@@HPEA_W@Z
extern "C" int MS_ABI impl__CreateControl_CWnd__QEAAHAEBU_GUID__PEB_WKAEBUtagRECT__PEAV1_IPEAVCFile__HPEA_W_Z(
    CWnd* pThis,
    const GUID& clsid,
    const WCHAR* pWindowName,
    DWORD dwStyle,
    const RECT& rect,
    CWnd* pParentWnd,
    UINT nID,
    CFile* pPersist,
    int bStorage,
    WCHAR* pLicKey) {
    if (!pThis) {
        return FALSE;
    }
    const GUID* clsidPtr = &clsid;
    RECT const* rectPtr = &rect;
    void* parent = pParentWnd;
    return pThis->CreateControl(clsidPtr, pWindowName, dwStyle, rectPtr, parent, nID, pPersist, bStorage, pLicKey);
}
// Symbol: ?CreateControl@CWnd@@QEAAHAEBU_GUID@@PEB_WKPEBUtagPOINT@@PEBUtagSIZE@@PEAV1@IPEAVCFile@@HPEA_W@Z
extern "C" int MS_ABI impl__CreateControl_CWnd__QEAAHAEBU_GUID__PEB_WKPEBUtagPOINT__PEBUtagSIZE__PEAV1_IPEAVCFile__HPEA_W_Z(
    CWnd* pThis,
    const GUID& clsid,
    const WCHAR* pWindowName,
    DWORD dwStyle,
    const POINT* pPoint,
    const SIZE* pSize,
    CWnd* pParentWnd,
    UINT nID,
    CFile* pPersist,
    int bStorage,
    WCHAR* pLicKey) {
    if (!pThis) {
        return FALSE;
    }
    const GUID* clsidPtr = &clsid;
    const RECT* rectPtr = nullptr;
    if (pPoint || pSize) {
        static RECT fallbackRect{};
        fallbackRect.left = pPoint ? pPoint->x : 0;
        fallbackRect.top = pPoint ? pPoint->y : 0;
        fallbackRect.right = fallbackRect.left + (pSize ? pSize->cx : 0);
        fallbackRect.bottom = fallbackRect.top + (pSize ? pSize->cy : 0);
        rectPtr = &fallbackRect;
    }
    void* parent = pParentWnd;
    return pThis->CreateControl(clsidPtr, pWindowName, dwStyle, rectPtr, parent, nID, pPersist, bStorage, pLicKey);
}
// Symbol: ?CreateControl@CWnd@@QEAAHPEB_W0KAEBUtagRECT@@PEAV1@IPEAVCFile@@HPEA_W@Z
extern "C" int MS_ABI impl__CreateControl_CWnd__QEAAHPEB_W0KAEBUtagRECT__PEAV1_IPEAVCFile__HPEA_W_Z(
    CWnd* pThis,
    const WCHAR* lpszClass,
    const WCHAR* pWindowName,
    DWORD dwStyle,
    const RECT& rect,
    CWnd* pParentWnd,
    UINT nID,
    CFile* pPersist,
    int bStorage,
    WCHAR* pLicKey) {
    if (!pThis) {
        return FALSE;
    }
    const RECT* rectPtr = &rect;
    void* parent = pParentWnd;
    return pThis->CreateControl(lpszClass, pWindowName, dwStyle, rectPtr, parent, nID, pPersist, bStorage, pLicKey);
}
// Symbol: ?CreateDlgIndirect@CWnd@@IEAAHPEBUDLGTEMPLATE@@PEAV1@PEAUHINSTANCE__@@@Z
extern "C" int MS_ABI impl__CreateDlgIndirect_CWnd__IEAAHPEBUDLGTEMPLATE__PEAV1_PEAUHINSTANCE_____Z(
    CWnd* pThis, const DLGTEMPLATE* pTemplate, CWnd* pParentWnd, HINSTANCE hInst) {
    if (!pThis) {
        return FALSE;
    }
    return pThis->CreateDlgIndirect(pTemplate, pParentWnd, hInst);
}
// Symbol: ?CreateRunDlgIndirect@CWnd@@IEAAHPEBUDLGTEMPLATE@@PEAV1@PEAUHINSTANCE__@@@Z
extern "C" int MS_ABI impl__CreateRunDlgIndirect_CWnd__IEAAHPEBUDLGTEMPLATE__PEAV1_PEAUHINSTANCE_____Z(
    CWnd* pThis, const DLGTEMPLATE* pTemplate, CWnd* pParentWnd, HINSTANCE hInst) {
    return impl__CreateDlgIndirect_CWnd__IEAAHPEBUDLGTEMPLATE__PEAV1_PEAUHINSTANCE_____Z(
        pThis, pTemplate, pParentWnd, hInst);
}
// Symbol: ?GetDlgItemTextW@CWnd@@QEBAHHAEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@@Z
extern "C" int MS_ABI impl__GetDlgItemTextW_CWnd__QEBAHHAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
    const CWnd* pThis, int nID, CString* rString) {
    if (!rString) {
        return 0;
    }
    rString->Empty();
    if (!pThis || !pThis->m_hWnd) {
        return 0;
    }

    HWND hItem = ::GetDlgItem(pThis->m_hWnd, nID);
    int length = hItem ? ::GetWindowTextLengthW(hItem) : 0;
    if (length <= 0) {
        return 0;
    }

    wchar_t* buffer = rString->GetBuffer(length + 1);
    int copied = ::GetWindowTextW(hItem, buffer, length + 1);
    rString->ReleaseBuffer(copied > 0 ? copied : 0);
    return copied > 0 ? copied : 0;
}
// Symbol: ?GetScrollPos@CWnd@@QEBAHH@Z
extern "C" int MS_ABI impl__GetScrollPos_CWnd__QEBAHH_Z(const CWnd* pThis, int nBar) {
    if (!pThis || !pThis->m_hWnd) {
        return 0;
    }
    return ::GetScrollPos(pThis->m_hWnd, nBar);
}
// Symbol: ?GetScrollInfo@CWnd@@QEAAHHPEAUtagSCROLLINFO@@I@Z
extern "C" int MS_ABI impl__GetScrollInfo_CWnd__QEAAHHPEAUtagSCROLLINFO__I_Z(
    CWnd* pThis, int nBar, SCROLLINFO* pScrollInfo, UINT nMask) {
    if (!pThis || !pThis->m_hWnd || !pScrollInfo) {
        return FALSE;
    }
    pScrollInfo->fMask = nMask;
    return ::GetScrollInfo(pThis->m_hWnd, nBar, pScrollInfo);
}
// Symbol: ?GetScrollLimit@CWnd@@QEAAHH@Z
extern "C" int MS_ABI impl__GetScrollLimit_CWnd__QEAAHH_Z(CWnd* pThis, int nBar) {
    if (!pThis || !pThis->m_hWnd) {
        return 0;
    }
    SCROLLINFO si = {};
    si.cbSize = sizeof(si);
    si.fMask = SIF_RANGE | SIF_PAGE;
    if (!::GetScrollInfo(pThis->m_hWnd, nBar, &si)) {
        return 0;
    }
    int page = si.nPage > 0 ? static_cast<int>(si.nPage) - 1 : 0;
    return si.nMax - page;
}
// Symbol: ?GetScrollRange@CWnd@@QEBAXHPEAH0@Z
extern "C" void MS_ABI impl__GetScrollRange_CWnd__QEBAXHPEAH0_Z(
    const CWnd* pThis, int nBar, int* pMinPos, int* pMaxPos) {
    if (pMinPos) {
        *pMinPos = 0;
    }
    if (pMaxPos) {
        *pMaxPos = 0;
    }
    if (pThis && pThis->m_hWnd) {
        ::GetScrollRange(pThis->m_hWnd, nBar, pMinPos, pMaxPos);
    }
}
// Symbol: ?IsDialogMessageW@CWnd@@QEAAHPEAUtagMSG@@@Z
extern "C" int MS_ABI impl__IsDialogMessageW_CWnd__QEAAHPEAUtagMSG___Z(CWnd* pThis, MSG* pMsg) {
    if (!pThis || !pThis->m_hWnd || !pMsg) {
        return FALSE;
    }
    return ::IsDialogMessageW(pThis->m_hWnd, pMsg);
}
// Symbol: ?SendChildNotifyLastMsg@CWnd@@QEAAHPEA_J@Z
extern "C" int MS_ABI impl__SendChildNotifyLastMsg_CWnd__QEAAHPEA_J_Z(CWnd* pThis, LONGLONG* pResult) {
    return pThis ? pThis->SendChildNotifyLastMsg(pResult) : FALSE;
}
// Symbol: ?GetScrollBarCtrl@CWnd@@UEBAPEAVCScrollBar@@H@Z
extern "C" CScrollBar* MS_ABI impl__GetScrollBarCtrl_CWnd__UEBAPEAVCScrollBar__H_Z(const CWnd* pThis, int nBar) {
    if (!pThis || !pThis->m_hWnd) {
        return nullptr;
    }
    if (nBar != SB_HORZ && nBar != SB_VERT) {
        return nullptr;
    }

    constexpr UINT AFX_IDW_HSCROLL = 0xE812;
    constexpr UINT AFX_IDW_VSCROLL = 0xE811;
    UINT id = (nBar == SB_HORZ) ? AFX_IDW_HSCROLL : AFX_IDW_VSCROLL;
    HWND hScrollBar = ::GetDlgItem(pThis->m_hWnd, static_cast<int>(id));
    if (!hScrollBar) {
        return nullptr;
    }
    return static_cast<CScrollBar*>(CWnd::FromHandle(hScrollBar));
}
// Symbol: ?IsFrameWnd@CWnd@@UEBAHXZ
extern "C" int MS_ABI impl__IsFrameWnd_CWnd__UEBAHXZ(const CWnd* pThis) {
    return (pThis && dynamic_cast<const CFrameWnd*>(pThis) != nullptr) ? TRUE : FALSE;
}
// Symbol: ?OnAmbientProperty@CWnd@@UEAAHPEAVCOleControlSite@@JPEAUtagVARIANT@@@Z
extern "C" int MS_ABI impl__OnAmbientProperty_CWnd__UEAAHPEAVCOleControlSite__JPEAUtagVARIANT___Z(
    CWnd* pThis, COleControlSite* pSite, long dispid, VARIANT* pVar)
{
    if (!pThis || !pVar) {
        return FALSE;
    }
    (void)pThis;

    const VARTYPE requested = pVar->vt;
    VariantInit(pVar);
    auto set_bool = [&](BOOL value) -> int {
        pVar->vt = VT_BOOL;
        pVar->boolVal = value ? VARIANT_TRUE : VARIANT_FALSE;
        return TRUE;
    };
    auto set_long = [&](long value) -> int {
        pVar->vt = VT_I4;
        pVar->lVal = value;
        return TRUE;
    };
    auto set_bstr = [&](const wchar_t* value) -> int {
        pVar->vt = VT_BSTR;
        pVar->bstrVal = ::SysAllocString(value ? value : L"");
        return TRUE;
    };

    auto site_bool = [&](VARIANT_BOOL* out) -> bool {
        if (!pSite) {
            return false;
        }
        return pSite->GetAmbientProperty(dispid, VT_BOOL, out) == TRUE;
    };
    auto site_long = [&](long* out) -> bool {
        if (!pSite) {
            return false;
        }
        return pSite->GetAmbientProperty(dispid, VT_I4, out) == TRUE;
    };
    auto site_bstr = [&](BSTR* out) -> bool {
        if (!pSite) {
            return false;
        }
        return pSite->GetAmbientProperty(dispid, VT_BSTR, out) == TRUE;
    };

    switch (dispid) {
    case DISPID_AMBIENT_USERMODE:
        if ((requested == VT_BOOL || requested == VT_EMPTY) && pSite) {
            VARIANT_BOOL site = VARIANT_FALSE;
            if (site_bool(&site)) {
                return set_bool(site == VARIANT_TRUE);
            }
            return set_bool(TRUE);
        }
        if (requested == VT_I4 || requested == VT_I2 || requested == VT_UI4 || requested == VT_UI2) {
            long value = 1;
            if (pSite && site_long(&value)) {
                return set_long(value);
            }
            return set_long(1);
        }
        if (requested == VT_EMPTY) {
            return set_bool(TRUE);
        }
        return FALSE;

    case DISPID_AMBIENT_BACKCOLOR:
    case DISPID_AMBIENT_FORECOLOR:
        if (requested == VT_BSTR) {
            return FALSE;
        }
        if ((requested == VT_I4 || requested == VT_I2 || requested == VT_EMPTY) && pSite) {
            long color = 0;
            if (site_long(&color)) {
                return set_long(color);
            }
        }
        if (requested == VT_I4 || requested == VT_I2 || requested == VT_EMPTY) {
            long color = (dispid == DISPID_AMBIENT_BACKCOLOR)
                ? static_cast<long>(::GetSysColor(COLOR_WINDOW))
                : static_cast<long>(::GetSysColor(COLOR_WINDOWTEXT));
            return set_long(color);
        }
        return FALSE;

    case DISPID_AMBIENT_DISPLAYNAME:
    case DISPID_AMBIENT_SCALEUNITS:
        if ((requested == VT_BSTR || requested == VT_EMPTY)) {
            if (pSite) {
                BSTR value = nullptr;
                if (site_bstr(&value)) {
                    return set_bstr(value);
                }
            }
            return set_bstr(L"");
        }
        return FALSE;

    case DISPID_AMBIENT_UIDEAD:
    case DISPID_AMBIENT_SHOWGRABHANDLES:
    case DISPID_AMBIENT_SHOWHATCHING:
    case DISPID_AMBIENT_DISPLAYASDEFAULT:
    case DISPID_AMBIENT_SUPPORTSMNEMONICS:
        if ((requested == VT_BOOL || requested == VT_EMPTY) && pSite) {
            VARIANT_BOOL value = VARIANT_FALSE;
            if (site_bool(&value)) {
                return set_bool(value == VARIANT_TRUE);
            }
        }
        if (requested == VT_BOOL || requested == VT_EMPTY) {
            BOOL value = (dispid == DISPID_AMBIENT_SHOWHATCHING || dispid == DISPID_AMBIENT_UIDEAD) ? FALSE : TRUE;
            return set_bool(value);
        }
        if (requested == VT_I4 || requested == VT_I2 || requested == VT_UI4 || requested == VT_UI2) {
            long value = (dispid == DISPID_AMBIENT_SHOWHATCHING || dispid == DISPID_AMBIENT_UIDEAD) ? 0 : 1;
            if (pSite && site_long(&value)) {
                return set_long(value);
            }
            return set_long(value);
        }
        return FALSE;

    case DISPID_AMBIENT_LOCALEID:
        if (requested == VT_BSTR) {
            return FALSE;
        }
        if ((requested == VT_I4 || requested == VT_I2 || requested == VT_EMPTY) && pSite) {
            long locale = 0;
            if (site_long(&locale)) {
                return set_long(locale);
            }
        }
        if (requested == VT_I4 || requested == VT_I2 || requested == VT_EMPTY) {
            return set_long(static_cast<long>(::GetUserDefaultLCID()));
        }
        return FALSE;

    case DISPID_AMBIENT_APPEARANCE:
        if (requested == VT_BOOL) {
            VARIANT_BOOL value = VARIANT_FALSE;
            if (site_bool(&value)) {
                return set_bool(value == VARIANT_TRUE);
            }
            return set_bool(FALSE);
        }
        if ((requested == VT_I4 || requested == VT_I2 || requested == VT_UI4 || requested == VT_UI2) && pSite) {
            long value = 0;
            if (site_long(&value)) {
                return set_long(value);
            }
        }
        if (requested == VT_I4 || requested == VT_I2 || requested == VT_UI4 || requested == VT_UI2 || requested == VT_EMPTY) {
            return set_long(0);
        }
        return FALSE;

    case DISPID_AMBIENT_FONT:
    case DISPID_AMBIENT_TEXTALIGN:
        return FALSE;

    default:
        return FALSE;
    }
}
// Symbol: ?OnToolHitTest@CWnd@@UEBA_JVCPoint@@PEAUtagTOOLINFOW@@@Z
extern "C" LONGLONG MS_ABI impl__OnToolHitTest_CWnd__UEBA_JVCPoint__PEAUtagTOOLINFOW___Z(
    const CWnd* pThis, CPoint point, TOOLINFOW* pTI) {
    if (!pThis || !pTI) {
        return -1;
    }
    std::memset(pTI, 0, sizeof(*pTI));
    pTI->cbSize = sizeof(*pTI);
    if (!pThis->m_hWnd) {
        return -1;
    }

    RECT clientRect{};
    if (!::GetClientRect(pThis->m_hWnd, &clientRect)) {
        return -1;
    }
    POINT ptScreen{point.x, point.y};
    POINT ptClient{point.x, point.y};
    if (!::ScreenToClient(pThis->m_hWnd, &ptClient) || !::PtInRect(&clientRect, ptClient)) {
        return -1;
    }

    HWND hHit = ::WindowFromPoint(ptScreen);
    if (!hHit || (hHit != pThis->m_hWnd && !::IsChild(pThis->m_hWnd, hHit))) {
        return -1;
    }

    pTI->hwnd = hHit;
    pTI->uId = reinterpret_cast<UINT_PTR>(hHit);
    pTI->lpszText = nullptr;
    RECT targetRect{};
    if (::GetWindowRect(hHit, &targetRect)) {
        POINT origin{targetRect.left, targetRect.top};
        ::ScreenToClient(pThis->m_hWnd, &origin);
        pTI->rect.left = origin.x;
        pTI->rect.top = origin.y;
        pTI->rect.right = origin.x + (targetRect.right - targetRect.left);
        pTI->rect.bottom = origin.y + (targetRect.bottom - targetRect.top);
    }
    return static_cast<LONGLONG>(reinterpret_cast<LONG_PTR>(hHit));
}
// Symbol: ?PreSubclassWindow@CWnd@@UEAAXXZ
extern "C" void MS_ABI impl__PreSubclassWindow_CWnd__UEAAXXZ(CWnd* pThis) {
    CWnd_PreSubclassWindowCompat(pThis);
}
// Symbol: ?PreTranslateMessage@CWnd@@UEAAHPEAUtagMSG@@@Z
extern "C" int MS_ABI impl__PreTranslateMessage_CWnd__UEAAHPEAUtagMSG___Z(CWnd* pThis, MSG* pMsg) {
    return CWnd_PreTranslateMessageCompat(pThis, pMsg);
}
// Symbol: ?OnCommand@CWnd@@MEAAH_K_J@Z
extern "C" int MS_ABI impl__OnCommand_CWnd__MEAAH_K_J_Z(CWnd* pThis, WPARAM wParam, LPARAM lParam) {
    if (!pThis) {
        return FALSE;
    }

    unsigned int nID = static_cast<unsigned int>(LOWORD(static_cast<UINT>(wParam)));
    int nCode = static_cast<int>(HIWORD(static_cast<UINT>(wParam)));
    void* pExtra = reinterpret_cast<void*>(lParam);
    if (pThis->OnCmdMsg(nID, nCode, pExtra, nullptr)) {
        return TRUE;
    }

    if (lParam) {
        CWnd* pSender = CWnd::FromHandle(reinterpret_cast<HWND>(lParam));
        if (pSender && pSender != pThis && pSender->OnCmdMsg(nID, nCode, pExtra, nullptr)) {
            return TRUE;
        }
    }
    return FALSE;
}
// Symbol: ?OnFinalRelease@CWnd@@UEAAXXZ
extern "C" void MS_ABI impl__OnFinalRelease_CWnd__UEAAXXZ(CWnd* pThis) {
    if (!pThis) {
        return;
    }

    if (pThis->m_hWnd && ::IsWindow(pThis->m_hWnd)) {
        // Release window resources as part of final COM/OLE lifetime transition.
        if (!::DestroyWindow(pThis->m_hWnd)) {
            pThis->PostNcDestroy();
        }
        return;
    }

    pThis->PostNcDestroy();
}
// Symbol: ?OnNotify@CWnd@@MEAAH_K_JPEA_J@Z
extern "C" int MS_ABI impl__OnNotify_CWnd__MEAAH_K_JPEA_J_Z(
    CWnd* pThis, WPARAM wParam, LPARAM lParam, LRESULT* pResult) {
    if (pResult) {
        *pResult = 0;
    }
    return pThis ? pThis->OnNotify(static_cast<uintptr_t>(wParam), lParam, reinterpret_cast<intptr_t*>(pResult)) : FALSE;
}
// Symbol: ?OnWndMsg@CWnd@@MEAAHI_K_JPEA_J@Z
extern "C" int MS_ABI impl__OnWndMsg_CWnd__MEAAHI_K_JPEA_J_Z(
    CWnd* pThis, UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult) {
    if (!pThis) {
        return FALSE;
    }
    if (pResult) {
        *pResult = 0;
    }
    // Objects constructed through the MSVC import library do not have a
    // cross-compiler-compatible CWnd vtable yet.  Dispatch the base handlers
    // directly instead of making a virtual call through an invalid vptr.
    switch (message) {
    case WM_COMMAND:
        return impl__OnCommand_CWnd__MEAAH_K_J_Z(pThis, wParam, lParam);
    case WM_NOTIFY:
        return impl__OnNotify_CWnd__MEAAH_K_JPEA_J_Z(pThis, wParam, lParam, pResult);
    default:
        return FALSE;
    }
}
CWnd* CWnd::FromHandle(HWND hWnd) {
    return OpenMfcAttachCWnd(hWnd);
}
void CWnd::CancelToolTips(int p0)
{
    bool enabled = p0 != FALSE;
    std::lock_guard<std::mutex> lock(g_wndStateMutex);
    for (auto& it : g_wndRuntimeStates) {
        it.second.toolTipsEnabled = enabled;
        if (!enabled) {
            it.second.trackingToolTipsEnabled = false;
        }
    }
}
CObject* CWnd::CreateObject()
{
    return new(std::nothrow) CWnd();
}
void CWnd::DeleteTempMap()
{
    OpenMfcCleanupTempWrappers();
}
void* CWnd::FromHandlePermanent(HWND p0)
{
    auto it = g_hwndMap.find(p0);
    CWnd* pWnd = it != g_hwndMap.end() ? it->second : nullptr;
    if (!pWnd || g_tempWrappers.find(pWnd) != g_tempWrappers.end()) {
        return nullptr;
    }
    return pWnd;
}
const MSG* CWnd::GetCurrentMessage()
{
    CWinThread* pThread = AfxGetThread();
    return pThread ? &pThread->m_msgCur : nullptr;
}
void* CWnd::GetDescendantWindow(HWND p0, int p1, int p2)
{
    if (!p0 || p1 == 0) {
        return p2 ? impl__FromHandlePermanent_CWnd__SAPEAV1_PEAUHWND_____Z(p0) : CWnd::FromHandle(p0);
    }

    HWND hDesc = ::GetDlgItem(p0, p1);
    if (!hDesc) {
        return nullptr;
    }
    return p2 ? impl__FromHandlePermanent_CWnd__SAPEAV1_PEAUHWND_____Z(hDesc) : CWnd::FromHandle(hDesc);
}
void* CWnd::GetSafeOwner(void* p0, HWND* p1)
{
    CWnd* pWnd = static_cast<CWnd*>(p0);
    HWND hOwner = pWnd ? pWnd->m_hWnd : nullptr;
    if (!hOwner) {
        CWinThread* pThread = AfxGetThread();
        hOwner = pThread && pThread->m_pMainWnd ? pThread->m_pMainWnd->m_hWnd : nullptr;
    }
    if (p1) {
        *p1 = hOwner;
    }
    return hOwner ? CWnd::FromHandle(hOwner) : nullptr;
}
HWND CWnd::GetSafeOwner_(void* p0, void** p1)
{
    HWND hWnd = p0 ? static_cast<HWND>(p0) : nullptr;
    if (!hWnd) {
        CWinThread* pThread = AfxGetThread();
        hWnd = pThread && pThread->m_pMainWnd ? pThread->m_pMainWnd->m_hWnd : nullptr;
    }
    if (p1) {
        *p1 = hWnd;
    }
    return hWnd;
}
const AFX_INTERFACEMAP* CWnd::GetThisInterfaceMap()
{
    return impl__GetThisInterfaceMap_CCmdTarget__KAPEBUAFX_INTERFACEMAP__XZ();
}
const AFX_MSGMAP* CWnd::GetThisMessageMap()
{
    return &g_cwndMessageMap;
}
int CWnd::GrayCtlColor(HDC p0, HWND p1, UINT p2, HBRUSH p3, DWORD p4)
{
    if (!p0 || !p1) {
        return FALSE;
    }

    // MFC historically uses a light gray brush for controls that request gray
    // disabled color treatment while preserving default text colors.
    COLORREF textColor = ::GetSysColor(COLOR_WINDOWTEXT);
    COLORREF backColor = ::GetSysColor(COLOR_3DFACE);
    if (p4) {
        textColor = ::GetSysColor(COLOR_GRAYTEXT);
        backColor = RGB(240, 240, 240);
    }

    switch (p2) {
        case WM_CTLCOLORMSGBOX:
        case WM_CTLCOLOREDIT:
        case WM_CTLCOLORLISTBOX:
        case WM_CTLCOLORSTATIC:
            break;
        default:
            break;
    }
    ::SetTextColor(p0, textColor);
    ::SetBkColor(p0, backColor);
    (void)p1;
    (void)p3;
    return TRUE;
}
int CWnd::ModifyStyle(HWND p0, DWORD p1, DWORD p2, UINT p3)
{
    if (!p0) {
        return FALSE;
    }

    ::SetLastError(0);
    LONG_PTR style = ::GetWindowLongPtrW(p0, GWL_STYLE);
    if (style == 0 && ::GetLastError() != 0) {
        return FALSE;
    }
    LONG_PTR newStyle = (style & ~static_cast<LONG_PTR>(p1)) | static_cast<LONG_PTR>(p2);
    if (newStyle == style) {
        return TRUE;
    }

    ::SetLastError(0);
    LONG_PTR prevStyle = ::SetWindowLongPtrW(p0, GWL_STYLE, newStyle);
    if (prevStyle == 0 && ::GetLastError() != 0) {
        return FALSE;
    }
    if (p3 != 0) {
        if (!::SetWindowPos(p0, nullptr, 0, 0, 0, 0,
                            SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE | SWP_FRAMECHANGED)) {
            return FALSE;
        }
    }
    return TRUE;
}
int CWnd::ModifyStyleEx(HWND p0, DWORD p1, DWORD p2, UINT p3)
{
    if (!p0) {
        return FALSE;
    }

    ::SetLastError(0);
    LONG_PTR style = ::GetWindowLongPtrW(p0, GWL_EXSTYLE);
    if (style == 0 && ::GetLastError() != 0) {
        return FALSE;
    }
    LONG_PTR newStyle = (style & ~static_cast<LONG_PTR>(p1)) | static_cast<LONG_PTR>(p2);
    if (newStyle == style) {
        return TRUE;
    }

    ::SetLastError(0);
    LONG_PTR prevStyle = ::SetWindowLongPtrW(p0, GWL_EXSTYLE, newStyle);
    if (prevStyle == 0 && ::GetLastError() != 0) {
        return FALSE;
    }
    if (p3 != 0) {
        if (!::SetWindowPos(p0, nullptr, 0, 0, 0, 0,
                            SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE | SWP_FRAMECHANGED)) {
            return FALSE;
        }
    }
    return TRUE;
}
int CWnd::ReflectLastMsg(HWND p0, LONGLONG* p1)
{
    CWnd* pWnd = p0 ? impl__FromHandlePermanent_CWnd__SAPEAV1_PEAUHWND_____Z(p0) : nullptr;
    if (!pWnd) {
        if (p1) {
            *p1 = 0;
        }
        return FALSE;
    }
    return pWnd->SendChildNotifyLastMsg(p1);
}
void CWnd::SendMessageToDescendants(HWND p0, UINT p1, ULONGLONG p2, LONGLONG p3, int p4, int p5)
{
    (void)p5;
    if (!p0) {
        return;
    }
    for (HWND hChild = ::GetWindow(p0, GW_CHILD); hChild; hChild = ::GetWindow(hChild, GW_HWNDNEXT)) {
        if (!::IsWindow(hChild)) {
            continue;
        }
        if (p4 && !::IsWindowVisible(hChild)) {
            continue;
        }
        ::SendMessageW(hChild, p1, static_cast<WPARAM>(p2), static_cast<LPARAM>(p3));
        SendMessageToDescendants(hChild, p1, p2, p3, p4, p5);
    }
}
int CWnd::WalkPreTranslateTree(HWND p0, MSG* p1)
{
    if (!p0 || !p1) {
        return FALSE;
    }

    for (HWND hChild = ::GetWindow(p0, GW_CHILD); hChild; hChild = ::GetWindow(hChild, GW_HWNDNEXT)) {
        if (!::IsWindow(hChild)) {
            continue;
        }

        CWnd* pChild = CWnd::FromHandle(hChild);
        if (pChild && CWnd_PreTranslateMessageCompat(pChild, p1)) {
            return TRUE;
        }
        if (WalkPreTranslateTree(hChild, p1)) {
            return TRUE;
        }
    }
    return FALSE;
}
void CWnd::ActivateTopParent()
{
    if (!m_hWnd) {
        return;
    }
    HWND hTop = static_cast<HWND>(GetTopLevelParent());
    if (!hTop) {
        return;
    }
    ::SetActiveWindow(hTop);
    ::SetForegroundWindow(hTop);
}
int CWnd::Attach(HWND p0)
{
    if (!p0) {
        return FALSE;
    }

    CWnd* existing = OpenMfcLookupCWnd(p0);
    if (existing && existing != this) {
        return FALSE;
    }

    if (m_hWnd == p0) {
        g_hwndMap[p0] = this;
        return TRUE;
    }

    if (m_hWnd) {
        g_hwndMap.erase(m_hWnd);
    }
    g_tempWrappers.erase(this);
    m_hWnd = p0;
    g_hwndMap[p0] = this;
    return TRUE;
}
void CWnd::AttachControlSite(CHandleMap* p0)
{
    GetWindowRuntimeState(this).properties[{0x434D, 0}] = p0;
}
void CWnd::AttachControlSite(COleControlSite* p0, UINT p1)
{
    GetWindowRuntimeState(this).properties[{0x434F, static_cast<WORD>(p1)}] = p0;
}
void CWnd::BindDefaultProperty(long p0, WORD p1, const WCHAR* p2, void* p3)
{
    GetWindowRuntimeState(this).properties[{p0, p1}] = p3;
}
void CWnd::BindProperty(long p0, void* p1)
{
    SetProperty(p0, 0, p1);
}
void CWnd::CenterWindow(void* p0)
{
    if (!m_hWnd) {
        return;
    }

    HWND hParent = nullptr;
    if (p0) {
        if (::IsWindow(reinterpret_cast<HWND>(p0))) {
            hParent = static_cast<HWND>(p0);
        } else if (auto* pWnd = static_cast<CWnd*>(p0)) {
            hParent = pWnd->m_hWnd;
        }
    }

    if (!hParent) {
        hParent = ::GetParent(m_hWnd);
    }
    if (!hParent) {
        hParent = ::GetDesktopWindow();
    }

    RECT rcParent;
    RECT rcWindow;
    if (!::GetWindowRect(m_hWnd, &rcWindow) || !::GetWindowRect(hParent, &rcParent)) {
        return;
    }

    int width = rcWindow.right - rcWindow.left;
    int height = rcWindow.bottom - rcWindow.top;
    int x = rcParent.left + ((rcParent.right - rcParent.left - width) / 2);
    int y = rcParent.top + ((rcParent.bottom - rcParent.top - height) / 2);

    ::SetWindowPos(m_hWnd, nullptr, x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
}
void CWnd::CheckDlgButton(int p0, UINT p1)
{
    if (!m_hWnd) {
        return;
    }
    ::CheckDlgButton(m_hWnd, p0, p1);
}
void CWnd::CheckRadioButton(int p0, int p1, int p2)
{
    if (!m_hWnd) {
        return;
    }
    for (int controlId = p0; controlId <= p1; ++controlId) {
        HWND hCtrl = ::GetDlgItem(m_hWnd, controlId);
        if (!hCtrl) {
            continue;
        }
        ::SendMessageW(hCtrl, BM_SETCHECK, (controlId == p2) ? BST_CHECKED : BST_UNCHECKED, 0);
    }
}
void CWnd::ClientToScreen(RECT* p0) const
{
    if (!m_hWnd || !p0) {
        return;
    }

    POINT points[2] = {
        {p0->left, p0->top},
        {p0->right, p0->bottom}
    };
    if (::MapWindowPoints(m_hWnd, HWND_DESKTOP, points, 2) != 0) {
        p0->left = points[0].x;
        p0->top = points[0].y;
        p0->right = points[1].x;
        p0->bottom = points[1].y;
    }
}
int CWnd::CreateControl(const GUID*& p0, const WCHAR* p1, DWORD p2, const RECT*& p3, void* p4, UINT p5, CFile* p6, int p7, WCHAR* p8)
{
    COleControl* control = dynamic_cast<COleControl*>(this);
    if (!control || !p0) {
        return FALSE;
    }

    CWnd* pParent = p4 ? static_cast<CWnd*>(p4) : this;
    if (!pParent) {
        pParent = this;
    }
    RECT rect{};
    if (p3) {
        rect = *p3;
    } else if (pParent && pParent->m_hWnd) {
        ::GetClientRect(pParent->m_hWnd, &rect);
    }

    return control->CreateControl(*p0, p1, p2, rect, pParent, p5, p6, p7, nullptr) ? TRUE : FALSE;
}
int CWnd::CreateControl(const GUID*& p0, const WCHAR* p1, DWORD p2, const POINT* p3, const SIZE* p4, void* p5, UINT p6, CFile* p7, int p8, WCHAR* p9)
{
    COleControl* control = dynamic_cast<COleControl*>(this);
    if (!control || !p0) {
        return FALSE;
    }

    CWnd* pParent = p5 ? static_cast<CWnd*>(p5) : this;
    if (!pParent) {
        pParent = this;
    }

    RECT rect{};
    if (p3) {
        rect.left = p3->x;
        rect.top = p3->y;
        rect.right = rect.left + (p4 ? p4->cx : 0);
        rect.bottom = rect.top + (p4 ? p4->cy : 0);
    }
    if (rect.right <= rect.left || rect.bottom <= rect.top) {
        if (pParent && pParent->m_hWnd) {
            ::GetClientRect(pParent->m_hWnd, &rect);
        }
    }

    return control->CreateControl(*p0, p1, p2, rect, pParent, p6, p7, p8, nullptr) ? TRUE : FALSE;
}
int CWnd::CreateControl(const CControlCreationInfo*& p0, DWORD p1, const POINT* p2, const SIZE* p3, void* p4, UINT p5)
{
    if (!p0) {
        return FALSE;
    }

    if (COleControl* control = dynamic_cast<COleControl*>(this)) {
        CWnd* pParent = p4 ? static_cast<CWnd*>(p4) : this;
        if (!pParent) {
            pParent = this;
        }
        RECT rect{};
        if (p2) {
            rect.left = p2->x;
            rect.top = p2->y;
            rect.right = rect.left + (p3 ? p3->cx : 0);
            rect.bottom = rect.top + (p3 ? p3->cy : 0);
        } else if (pParent && pParent->m_hWnd) {
            ::GetClientRect(pParent->m_hWnd, &rect);
        }

        return control->CreateControl(L"", L"", p1, rect, pParent, 0, nullptr, FALSE, nullptr) ? TRUE : FALSE;
    }

    return FALSE;
}
int CWnd::CreateControl(const WCHAR* p0, const WCHAR* p1, DWORD p2, const RECT*& p3, void* p4, UINT p5, CFile* p6, int p7, WCHAR* p8)
{
    if (COleControl* control = dynamic_cast<COleControl*>(this)) {
        CWnd* pParent = p4 ? static_cast<CWnd*>(p4) : this;
        if (!pParent) {
            pParent = this;
        }
        RECT rect{};
        if (p3) {
            rect = *p3;
        } else if (pParent && pParent->m_hWnd) {
            ::GetClientRect(pParent->m_hWnd, &rect);
        }
        return control->CreateControl(p0, p1, p2, rect, pParent, p5, p6, p7, p8 ? p8 : nullptr) ? TRUE : FALSE;
    }

    return FALSE;
}
int CWnd::CreateControlContainer(COleControlContainer** p0)
{
    if (!p0) {
        return FALSE;
    }

    auto it = g_controlContainerMap.find(this);
    if (it != g_controlContainerMap.end()) {
        *p0 = it->second;
        return TRUE;
    }

    COleControlContainer* pContainer = new(std::nothrow) COleControlContainer(this);
    if (!pContainer) {
        *p0 = nullptr;
        return FALSE;
    }

    g_controlContainerMap[this] = pContainer;
    *p0 = pContainer;
    return TRUE;
}
int CWnd::CreateControlSite(COleControlContainer* p0, COleControlSite** p1, UINT p2, const GUID*& p3)
{
    (void)p3;
    if (!p1) {
        return FALSE;
    }
    *p1 = nullptr;

    COleControlContainer* pContainer = p0;
    if (!pContainer) {
        if (!CreateControlContainer(&pContainer) || !pContainer) {
            return FALSE;
        }
    }

    COleControlSite* pSite = pContainer->CreateSite(pContainer);
    if (!pSite) {
        return FALSE;
    }

    AttachControlSite(pSite, p2);
    *p1 = pSite;
    if (CWnd* pParent = pContainer->GetWnd()) {
        pSite->m_hWnd = pParent->m_hWnd;
    }
    return TRUE;
}
int CWnd::CreateDlg(const WCHAR* p0, void* p1)
{
    if (!p0) {
        return FALSE;
    }
    CWnd* pParent = p1 ? static_cast<CWnd*>(p1) : nullptr;
    HINSTANCE hInst = AfxGetInstanceHandle();
    if (!hInst) {
        hInst = ::GetModuleHandleW(nullptr);
    }
    HRSRC hTemplate = ::FindResourceW(hInst, p0, MAKEINTRESOURCEW(RT_DIALOG));
    if (!hTemplate) {
        return FALSE;
    }
    HGLOBAL hRes = ::LoadResource(hInst, hTemplate);
    if (!hRes) {
        return FALSE;
    }
    const DLGTEMPLATE* pTemplate = static_cast<const DLGTEMPLATE*>(::LockResource(hRes));
    if (!pTemplate) {
        return FALSE;
    }
    return CreateDlgIndirect(pTemplate, pParent, hInst);
}
int CWnd::CreateDlgIndirect(const DLGTEMPLATE* p0, void* p1, HINSTANCE p2)
{
    if (!p0) {
        return FALSE;
    }
    CWnd* pParent = p1 ? static_cast<CWnd*>(p1) : nullptr;
    HINSTANCE hInst = p2 ? p2 : AfxGetInstanceHandle();
    if (!hInst) {
        hInst = ::GetModuleHandleW(nullptr);
    }
    HWND hWnd = ::CreateDialogIndirectParamW(
        hInst,
        p0,
        pParent ? pParent->m_hWnd : nullptr,
        reinterpret_cast<DLGPROC>(AfxWndProc),
        reinterpret_cast<LPARAM>(this));
    if (!hWnd) {
        return FALSE;
    }
    m_hWnd = hWnd;
    g_hwndMap[hWnd] = this;
    return TRUE;
}
int CWnd::CreateRunDlgIndirect(const DLGTEMPLATE* p0, void* p1, HINSTANCE p2)
{
    return CreateDlgIndirect(p0, p1, p2);
}
LONGLONG CWnd::Default()
{
    if (!m_hWnd) {
        return 0;
    }
    // Re-dispatch the message currently being handled, as real MFC does via the
    // thread's current MSG, rather than default-processing a bogus zero message.
    const MSG* pMsg = GetCurrentMessage();
    if (pMsg && pMsg->message != 0) {
        return impl__DefWindowProcW_CWnd__MEAA_JI_K_J_Z(
            this, pMsg->message, pMsg->wParam, pMsg->lParam);
    }
    return impl__DefWindowProcW_CWnd__MEAA_JI_K_J_Z(this, 0, 0, 0);
}
LONGLONG CWnd::DefWindowProcW(UINT p0, ULONGLONG p1, LONGLONG p2)
{
    if (!m_hWnd) {
        return 0;
    }
    if (m_pfnSuper) {
        return reinterpret_cast<LRESULT>(::CallWindowProcW(m_pfnSuper, m_hWnd, p0, p1, p2));
    }
    return impl__DefWindowProcW_CWnd__MEAA_JI_K_J_Z(this, p0, p1, p2);
}
HWND CWnd::Detach()
{
    if (!m_hWnd) {
        return nullptr;
    }
    HWND hWnd = m_hWnd;
    m_hWnd = nullptr;
    g_hwndMap.erase(hWnd);
    CleanupWindowRuntimeState(this);
    return hWnd;
}
int CWnd::DoD2DPaint()
{
    return m_hWnd ? 1 : 0;
}
void CWnd::EnableScrollBarCtrl(int p0, int p1)
{
    if (!m_hWnd) {
        return;
    }
    UINT flags = ESB_DISABLE_BOTH;
    if (p0) {
        flags = p1 ? ESB_ENABLE_BOTH : ESB_DISABLE_BOTH;
    } else if (p1) {
        flags = ESB_ENABLE_BOTH;
    }
    ::EnableScrollBar(m_hWnd, p0, flags);
}
int CWnd::EnableToolTips(int p0)
{
    if (!m_hWnd) {
        return FALSE;
    }
    GetWindowRuntimeState(this).toolTipsEnabled = p0 != FALSE;
    return TRUE;
}
int CWnd::EnableTrackingToolTips(int p0)
{
    if (!m_hWnd) {
        return FALSE;
    }
    GetWindowRuntimeState(this).trackingToolTipsEnabled = p0 != FALSE;
    return TRUE;
}
int CWnd::ExecuteDlgInit(void* p0)
{
    if (!m_hWnd) {
        return FALSE;
    }
    if (!p0) {
        return FALSE;
    }
    ::ShowWindow(m_hWnd, SW_SHOW);
    ::UpdateWindow(m_hWnd);
    return TRUE;
}
int CWnd::ExecuteDlgInit(const WCHAR* p0)
{
    if (!m_hWnd) {
        return FALSE;
    }
    if (!p0) {
        return FALSE;
    }
    ::SetWindowTextW(m_hWnd, p0);
    ::ShowWindow(m_hWnd, SW_SHOW);
    ::UpdateWindow(m_hWnd);
    return TRUE;
}
void CWnd::FilterToolTipMessage(MSG* p0)
{
    if (!p0 || !m_hWnd) {
        return;
    }
    if (!GetWindowRuntimeState(this).toolTipsEnabled) {
        return;
    }
    if (p0->message == WM_MOUSEMOVE || p0->message == WM_LBUTTONUP || p0->message == WM_RBUTTONUP) {
        ::SendMessageW(m_hWnd, WM_MOUSEMOVE, p0->wParam, p0->lParam);
    }
}
void* CWnd::FindSiteOrWnd(const COleControlSiteOrWnd* p0) const
{
    if (!p0) {
        return nullptr;
    }
    auto it = g_wndRuntimeStates.find(const_cast<CWnd*>(this));
    if (it != g_wndRuntimeStates.end()) {
        for (const auto& entry : it->second.properties) {
            if (entry.second == p0) {
                return entry.second;
            }
        }
    }
    return const_cast<COleControlSiteOrWnd*>(p0);
}
void* CWnd::FindSiteOrWndWithFocus() const
{
    HWND hFocus = ::GetFocus();
    if (!hFocus) {
        return nullptr;
    }
    return FindSiteOrWnd(reinterpret_cast<const COleControlSiteOrWnd*>(CWnd::FromHandle(hFocus)));
}
long CWnd::GetAccessibilityHitTest(long p0, long p1, VARIANT* p2)
{
    if (!m_hWnd || !p2) {
        return 0;
    }
    // Default to returning self unless a child window explicitly owns the point.
    POINT pt = {p0, p1};
    ::ClientToScreen(m_hWnd, &pt);

    VARIANT result = {};
    result.vt = VT_I4;
    result.lVal = 0; // CHILDID_SELF

    HWND hHit = ::WindowFromPoint(pt);
    if (hHit && hHit != m_hWnd) {
        HWND hChild = ::GetWindow(m_hWnd, GW_CHILD);
        while (hChild) {
            RECT rc{};
            if (::GetWindowRect(hChild, &rc) && pt.x >= rc.left && pt.x < rc.right && pt.y >= rc.top && pt.y < rc.bottom) {
                result.lVal = static_cast<long>(::GetDlgCtrlID(hChild));
                break;
            }
            hChild = ::GetWindow(hChild, GW_HWNDNEXT);
        }
    }

    *p2 = result;
    return 0;
}
long CWnd::GetAccessibilityLocation(VARIANT p0, long* p1, long* p2, long* p3, long* p4)
{
    if (!m_hWnd || !p1 || !p2 || !p3 || !p4) {
        return 0;
    }

    HWND hTarget = m_hWnd;
    if (p0.vt == VT_I4 && p0.lVal > 0) {
        hTarget = ::GetDlgItem(m_hWnd, static_cast<int>(p0.lVal));
    }
    if (!hTarget) {
        return 0;
    }

    RECT rc{};
    if (!::GetWindowRect(hTarget, &rc)) {
        return 0;
    }
    *p1 = rc.left;
    *p2 = rc.top;
    *p3 = rc.right;
    *p4 = rc.bottom;
    return 0;
}
long CWnd::GetAccessibleChild(VARIANT p0, IDispatch** p1)
{
    if (!p1) {
        return 0;
    }
    *p1 = nullptr;

    if (!m_hWnd) {
        return 0;
    }

    // Child enumeration for accessibility can return NULL for unsupported children.
    // We expose child HWND/ID data through VARIANT, while there is no concrete
    // IDispatch wrapper in this clean-room port, so return S_FALSE by not
    // producing a dispatch pointer.
    (void)p0;
    return 0;
}
long CWnd::GetAccessibleChildCount()
{
    if (!m_hWnd) {
        return 0;
    }
    long count = 0;
    for (HWND hChild = ::GetWindow(m_hWnd, GW_CHILD); hChild; hChild = ::GetWindow(hChild, GW_HWNDNEXT)) {
        ++count;
    }
    return count;
}
long CWnd::GetAccessibleName(VARIANT p0, WCHAR** p1)
{
    (void)p0;
    if (!p1) {
        return 0;
    }

    *p1 = nullptr;
    if (!m_hWnd) {
        return 0;
    }
    int len = ::GetWindowTextLengthW(m_hWnd);
    if (len <= 0) {
        return 0;
    }
    ++len;
    WCHAR* text = static_cast<WCHAR*>(::CoTaskMemAlloc(static_cast<size_t>(len) * sizeof(WCHAR)));
    if (!text) {
        return 0;
    }
    int copied = ::GetWindowTextW(m_hWnd, text, len);
    if (copied <= 0) {
        ::CoTaskMemFree(text);
        return 0;
    }
    text[copied] = L'\0';
    *p1 = text;
    return 1;
}
int CWnd::GetCheckedRadioButton(int p0, int p1) const
{
    if (!m_hWnd) {
        return 0;
    }
    for (int id = p0; id <= p1; ++id) {
        HWND hBtn = ::GetDlgItem(m_hWnd, id);
        if (!hBtn) {
            continue;
        }
        LRESULT check = ::SendMessageW(hBtn, BM_GETCHECK, 0, 0);
        if (check == BST_CHECKED) {
            return id;
        }
    }
    return 0;
}
COleControlContainer* CWnd::GetControlContainer()
{
    auto it = g_controlContainerMap.find(this);
    if (it != g_controlContainerMap.end()) {
        return it->second;
    }
    return nullptr;
}
IUnknown* CWnd::GetControlUnknown()
{
    if (!m_hWnd) {
        return nullptr;
    }

    auto it = g_controlContainerMap.find(this);
    if (it == g_controlContainerMap.end() || !it->second) {
        return nullptr;
    }

    COleControlContainer* pContainer = it->second;
    CPtrList& sites = pContainer->m_listSites;
    for (CPtrList::POSITION pos = sites.GetHeadPosition(); pos != nullptr;) {
        COleControlSite* pSite = static_cast<COleControlSite*>(sites.GetNext(pos));
        if (!pSite) {
            continue;
        }
        if ((pSite->m_hWnd == m_hWnd || pSite->m_pControl == this) && pSite->m_lpObject) {
            return pSite->m_lpObject;
        }
    }

    return nullptr;
}
int CWnd::GetDlgCtrlID() const
{
    if (!m_hWnd) {
        return 0;
    }
    return static_cast<int>(::GetDlgCtrlID(m_hWnd));
}
UINT CWnd::GetDlgItemInt(int p0, int* p1, int p2) const
{
    if (!m_hWnd) {
        return 0;
    }
    BOOL translated = FALSE;
    UINT value = ::GetDlgItemInt(m_hWnd, p0, &translated, p2 != FALSE);
    if (p1) {
        *p1 = translated ? 1 : 0;
    }
    return value;
}
int CWnd::GetDlgItemTextW(int p0, void*& p1, void** p2, void* p3) const
{
    if (!m_hWnd || !p1) {
        return 0;
    }

    HWND hItem = ::GetDlgItem(m_hWnd, p0);
    if (!hItem) {
        return 0;
    }

    // Backward-compatible "wide string" text extraction path:
    // If p2 is provided as a pointer-to-pointer, return freshly allocated text.
    int len = ::GetWindowTextLengthW(hItem);
    if (len <= 0) {
        *reinterpret_cast<void**>(p2) = nullptr;
        return 0;
    }

    ++len;
    WCHAR* text = static_cast<WCHAR*>(::CoTaskMemAlloc(static_cast<size_t>(len) * sizeof(WCHAR)));
    if (!text) {
        return 0;
    }
    int copied = ::GetWindowTextW(hItem, text, len);
    if (copied <= 0) {
        ::CoTaskMemFree(text);
        text = nullptr;
        copied = 0;
    }

    if (copied > 0) {
        if (p3) {
            // caller-provided destination pointer
            auto* dest = static_cast<WCHAR*>(p3);
            ::lstrcpynW(dest, text, static_cast<int>(copied + 1));
        }
        p1 = text;
        if (p2) {
            *reinterpret_cast<void**>(p2) = text;
        }
    } else {
        if (p2) {
            *reinterpret_cast<void**>(p2) = nullptr;
        }
    }
    return copied;
}
int CWnd::GetDlgItemTextW(int p0, WCHAR* p1, int p2) const
{
    if (!m_hWnd || !p1 || p2 <= 0) {
        return 0;
    }
    return ::GetDlgItemTextW(m_hWnd, p0, p1, p2);
}
IUnknown* CWnd::GetDSCCursor()
{
    return GetControlUnknown();
}
DWORD CWnd::GetExStyle() const
{
    if (!m_hWnd) {
        return 0;
    }
    return static_cast<DWORD>(::GetWindowLongPtrW(m_hWnd, GWL_EXSTYLE));
}
int CWnd::GetGestureConfig(CGestureConfig* p0)
{
    if (!m_hWnd) {
        return 0;
    }
    (void)p0;
    return GetWindowRuntimeState(this).gestureConfigEnabled ? TRUE : FALSE;
}
DWORD CWnd::GetGestureStatus(CPoint p0)
{
    (void)p0;
    return GetWindowRuntimeState(this).d2dSupportEnabled ? 1u : 0u;
}
const AFX_INTERFACEMAP* CWnd::GetInterfaceMap() const
{
    return GetThisInterfaceMap();
}
COleControlSiteOrWnd* CWnd::GetNextDlgGroupItem(void* p0) const
{
    if (!m_hWnd) {
        return nullptr;
    }
    HWND hWndStart = p0 ? static_cast<CWnd*>(p0)->m_hWnd : nullptr;
    HWND hWndNext = ::GetNextDlgGroupItem(m_hWnd, hWndStart, TRUE);
    return hWndNext ? reinterpret_cast<COleControlSiteOrWnd*>(CWnd::FromHandle(hWndNext)) : nullptr;
}
COleControlSiteOrWnd* CWnd::GetNextDlgTabItem(void* p0, int p1) const
{
    if (!m_hWnd) {
        return nullptr;
    }
    HWND hWndStart = p0 ? static_cast<CWnd*>(p0)->m_hWnd : nullptr;
    HWND hWndNext = ::GetNextDlgTabItem(m_hWnd, hWndStart, p1 != FALSE);
    return hWndNext ? reinterpret_cast<COleControlSiteOrWnd*>(CWnd::FromHandle(hWndNext)) : nullptr;
}
_AFX_OCC_DIALOG_INFO* CWnd::GetOccDialogInfo()
{
    auto it = g_wndRuntimeStates.find(this);
    if (it == g_wndRuntimeStates.end()) {
        return nullptr;
    }
    return reinterpret_cast<_AFX_OCC_DIALOG_INFO*>(it->second.properties[{0x4F43, 0}]);
}
COleControlSite* CWnd::GetOleControlSite(UINT p0) const
{
    COleControlContainer* pContainer = const_cast<CWnd*>(this)->GetControlContainer();
    if (!pContainer) {
        return nullptr;
    }
    return pContainer->FindItem(p0);
}
CFrameWnd* CWnd::GetParentFrame() const
{
    CWnd* pParent = GetParent();
    while (pParent) {
        if (auto* pFrame = dynamic_cast<CFrameWnd*>(pParent)) {
            return pFrame;
        }
        pParent = pParent->GetParent();
    }
    return nullptr;
}
void* CWnd::GetParentOwner() const
{
    if (!m_hWnd) {
        return nullptr;
    }
    return static_cast<void*>(GetTopLevelOwner());
}
COleControlSiteOrWnd* CWnd::GetPrevDlgGroupItem(void* p0) const
{
    if (!m_hWnd) {
        return nullptr;
    }
    HWND hWndStart = p0 ? static_cast<CWnd*>(p0)->m_hWnd : nullptr;
    HWND hWndPrev = ::GetNextDlgGroupItem(m_hWnd, hWndStart, FALSE);
    return hWndPrev ? reinterpret_cast<COleControlSiteOrWnd*>(CWnd::FromHandle(hWndPrev)) : nullptr;
}
void CWnd::GetProperty(long p0, WORD p1, void* p2) const
{
    if (!p2) {
        return;
    }
    auto it = g_wndRuntimeStates.find(const_cast<CWnd*>(this));
    if (it == g_wndRuntimeStates.end()) {
        *reinterpret_cast<void**>(p2) = nullptr;
        return;
    }

    auto propIt = it->second.properties.find({p0, p1});
    if (propIt == it->second.properties.end()) {
        *reinterpret_cast<void**>(p2) = nullptr;
        return;
    }
    *reinterpret_cast<void**>(p2) = propIt->second;
}
CHwndRenderTarget* CWnd::GetRenderTarget()
{
    // CHwndRenderTarget is ABI-compatible with the render-target family in this
    // port and shares the same runtime-slot allocation strategy as CDCRenderTarget.
    return reinterpret_cast<CHwndRenderTarget*>(GetDCRenderTarget());
}
DWORD CWnd::GetStyle() const
{
    if (!m_hWnd) {
        return 0;
    }
    return static_cast<DWORD>(::GetWindowLongPtrW(m_hWnd, GWL_STYLE));
}
void* CWnd::GetSuperWndProcAddr()
{
    return reinterpret_cast<void*>(m_pfnSuper);
}
CFrameWnd* CWnd::GetTopLevelFrame() const
{
    CWnd* pWnd = const_cast<CWnd*>(this)->GetTopLevelParent() ? CWnd::FromHandle(
        static_cast<HWND>(const_cast<CWnd*>(this)->GetTopLevelParent())) : nullptr;
    while (pWnd) {
        CFrameWnd* pFrame = dynamic_cast<CFrameWnd*>(pWnd);
        if (pFrame) {
            return pFrame;
        }
        pWnd = pWnd->GetParent();
    }
    return nullptr;
}
void* CWnd::GetTopLevelOwner() const
{
    if (!m_hWnd) {
        return nullptr;
    }
    HWND hOwner = ::GetWindow(m_hWnd, GW_OWNER);
    if (!hOwner) {
        return m_hWnd;
    }
    HWND hPrevOwner = hOwner;
    while ((hPrevOwner = ::GetWindow(hPrevOwner, GW_OWNER)) != nullptr) {
        hOwner = hPrevOwner;
    }
    return hOwner;
}
void* CWnd::GetTopLevelParent() const
{
    if (!m_hWnd) {
        return nullptr;
    }
    CWnd* pParent = GetParent();
    if (!pParent) {
        return m_hWnd;
    }
    CWnd* pTop = pParent;
    while (pTop && pTop->GetParent()) {
        pTop = pTop->GetParent();
    }
    return pTop ? pTop->m_hWnd : m_hWnd;
}
long CWnd::GetWindowLessChildCount()
{
    if (!m_hWnd) {
        return 0;
    }
    long count = 0;
    for (HWND hWnd = ::GetWindow(m_hWnd, GW_CHILD); hWnd; hWnd = ::GetWindow(hWnd, GW_HWNDNEXT)) {
        if ((::GetWindowLongPtrW(hWnd, GWL_STYLE) & WS_CLIPSIBLINGS) == 0) {
            ++count;
        }
    }
    return count;
}
int CWnd::GetWindowPlacement(WINDOWPLACEMENT* p0) const
{
    if (!m_hWnd || !p0) {
        return FALSE;
    }
    p0->length = sizeof(*p0);
    return ::GetWindowPlacement(m_hWnd, p0);
}
long CWnd::GetWindowedChildCount()
{
    if (!m_hWnd) {
        return 0;
    }
    long count = 0;
    for (HWND hWnd = ::GetWindow(m_hWnd, GW_CHILD); hWnd; hWnd = ::GetWindow(hWnd, GW_HWNDNEXT)) {
        ++count;
    }
    return count;
}
int CWnd::HandleFloatingSysCommand(UINT p0, INT_PTR p1)
{
    if (!m_hWnd) {
        return FALSE;
    }
    return static_cast<int>(::SendMessageW(m_hWnd, WM_SYSCOMMAND, p0, p1));
}
int CWnd::InitControlContainer(int p0)
{
    COleControlContainer* pContainer = nullptr;
    int result = CreateControlContainer(&pContainer);
    (void)p0;
    return result;
}
void CWnd::InvokeHelper(long p0, WORD p1, void* p2, VARIANT* p3)
{
    if (p3) {
        // Keep return storage in a known state even when no automation target is
        // attached to this window.
        ::VariantClear(p3);
        ::VariantInit(p3);
        p3->vt = VT_EMPTY;
    }

    if (p0 == 0) {
        return;
    }

    IDispatch* pDispatch = nullptr;
    if (IUnknown* pUnknown = GetControlUnknown()) {
        if (FAILED(pUnknown->QueryInterface(IID_IDispatch, reinterpret_cast<void**>(&pDispatch))) || !pDispatch) {
            return;
        }
    } else {
        // Best-effort fallback: forward well-known automation-style verbs to the
        // window message stream when no control dispatch is present.
        if (m_hWnd && p1 == DISPATCH_METHOD && p3 && p3->vt == VT_EMPTY) {
            p3->vt = VT_I4;
            p3->lVal = static_cast<LONG>(::SendMessageW(m_hWnd, WM_COMMAND, static_cast<WPARAM>(p0), 0));
        }
        return;
    }

    if (!p3) {
        DISPPARAMS noParams = {};
        pDispatch->Invoke(p0, IID_NULL, LOCALE_USER_DEFAULT, p1, &noParams, nullptr, nullptr, nullptr);
        pDispatch->Release();
        return;
    }

    DISPPARAMS noParams = {};
    VARIANT result;
    VariantInit(&result);
    EXCEPINFO excep = {};
    UINT argErr = 0;
    HRESULT hr = pDispatch->Invoke(static_cast<DISPID>(p0), IID_NULL, LOCALE_USER_DEFAULT,
                                   p1, &noParams, &result, &excep, &argErr);
    pDispatch->Release();

    if (FAILED(hr)) {
        if (p3) {
            p3->vt = VT_ERROR;
            p3->scode = static_cast<SCODE>(hr);
        }
        return;
    }

    *p3 = result;
}
UINT CWnd::IsDlgButtonChecked(int p0) const
{
    if (!m_hWnd) {
        return BST_UNCHECKED;
    }
    HWND hControl = ::GetDlgItem(m_hWnd, p0);
    if (!hControl) {
        return BST_UNCHECKED;
    }
    return static_cast<UINT>(::SendMessageW(hControl, BM_GETCHECK, 0, 0));
}
int CWnd::IsTopParentActive() const
{
    if (!m_hWnd) {
        return FALSE;
    }
    HWND hWndTop = static_cast<HWND>(GetTopLevelParent());
    if (!hWndTop) {
        return FALSE;
    }
    return (::GetActiveWindow() == hWndTop || ::GetForegroundWindow() == hWndTop) ? TRUE : FALSE;
}
int CWnd::IsTouchWindow() const
{
    if (!m_hWnd) {
        return FALSE;
    }
    return ::IsTouchWindow(m_hWnd, nullptr) ? TRUE : FALSE;
}
int CWnd::MessageBoxW(const WCHAR* p0, const WCHAR* p1, UINT p2)
{
    return static_cast<int>(::MessageBoxW(m_hWnd, p0, p1, p2));
}
LONGLONG CWnd::OnActivateTopLevel(ULONGLONG p0, LONGLONG p1)
{
    if (!m_hWnd) {
        return 0;
    }

    // Keep the frame visually responsive when activated from shell-driven flows.
    if (p0) {
        ::ShowWindow(m_hWnd, SW_SHOWNA);
        ::BringWindowToTop(m_hWnd);
        if (p1 != 0) {
            ::UpdateWindow(m_hWnd);
        }
    }
    return 0;
}
int CWnd::OnCharToItem(UINT p0, CListBox* p1, UINT p2)
{
    if (!p1 || !p1->m_hWnd) {
        return -1;
    }

    wchar_t ch[2] = { static_cast<wchar_t>(p0), 0 };
    int start = static_cast<int>(p2);
    if (start < -1) {
        start = -1;
    }

    LRESULT result = ::SendMessageW(p1->m_hWnd, LB_SELECTSTRING, static_cast<WPARAM>(start), reinterpret_cast<LPARAM>(ch));
    return static_cast<int>(result);
}
int CWnd::OnChildNotify(UINT p0, ULONGLONG p1, LONGLONG p2, LONGLONG* p3)
{
    if (!m_hWnd || !p3) {
        return 0;
    }

    if (p0 == WM_COMMAND) {
        return CWnd::OnCommand(static_cast<uintptr_t>(p1), p2);
    }
    if (p0 == WM_NOTIFY) {
        intptr_t result = static_cast<intptr_t>(*p3);
        int handled = CWnd::OnNotify(static_cast<uintptr_t>(p1), p2, &result);
        *p3 = result;
        return handled;
    }

    return 0;
}
int CWnd::OnCompareItem(int p0, COMPAREITEMSTRUCT* p1)
{
    (void)p0;
    if (!p1 || !m_hWnd) {
        return 0;
    }

    if (p1->itemData1 == p1->itemData2) {
        return 0;
    }
    if (p1->itemData1 > p1->itemData2) {
        return 1;
    }
    if (p1->itemData1 < p1->itemData2) {
        return -1;
    }
    return 0;
}
HBRUSH CWnd::OnCtlColor(CDC* p0, void* p1, UINT p2)
{
    if (!p0 || !p0->m_hDC) {
        return nullptr;
    }

    auto* pWnd = reinterpret_cast<CWnd*>(p1);
    UINT style = pWnd && pWnd->m_hWnd ? static_cast<UINT>(::GetWindowLongPtrW(pWnd->m_hWnd, GWL_STYLE)) : 0;

    switch (p2) {
        case CTLCOLOR_STATIC:
            if ((style & SS_BLACKFRAME) == 0) {
                ::SetTextColor(p0->m_hDC, GetSysColor(COLOR_WINDOWTEXT));
                ::SetBkColor(p0->m_hDC, GetSysColor(COLOR_BTNFACE));
            }
            return ::GetSysColorBrush(COLOR_BTNFACE);
        case CTLCOLOR_LISTBOX:
            ::SetTextColor(p0->m_hDC, GetSysColor(COLOR_WINDOWTEXT));
            ::SetBkColor(p0->m_hDC, GetSysColor(COLOR_WINDOW));
            return ::GetSysColorBrush(COLOR_WINDOW);
        case CTLCOLOR_EDIT:
            ::SetTextColor(p0->m_hDC, GetSysColor(COLOR_WINDOWTEXT));
            ::SetBkColor(p0->m_hDC, GetSysColor(COLOR_WINDOW));
            return ::GetSysColorBrush(COLOR_WINDOW);
        case CTLCOLOR_BTN:
            ::SetTextColor(p0->m_hDC, GetSysColor(COLOR_BTNTEXT));
            ::SetBkColor(p0->m_hDC, GetSysColor(COLOR_BTNFACE));
            return ::GetSysColorBrush(COLOR_BTNFACE);
        default:
            return ::GetSysColorBrush(COLOR_WINDOW);
    }
}
int CWnd::OnDeleteItem(int p0, DELETEITEMSTRUCT* p1)
{
    (void)p0;
    if (!p1) {
        return 0;
    }
    // No action if the item data is not a heap-allocated pointer. We keep this
    // lightweight by not guessing ownership and only cleaning up if p1 explicitly
    // marks an item as dynamic data.
    if (p1->itemData) {
        return 1;
    }
    return 0;
}
void CWnd::OnDestroy()
{
    if (m_hWnd) {
        CleanupWindowRuntimeState(this);
    }
}
void CWnd::OnDevModeChange(void* p0, const WCHAR* p1)
{
    if (!m_hWnd) {
        return;
    }
    // p1 is typically a form driver description string on Windows.
    (void)p0;
    if (p1) {
        ::SetWindowTextW(m_hWnd, p1);
    }
    ::RedrawWindow(m_hWnd, nullptr, nullptr, RDW_INVALIDATE | RDW_UPDATENOW);
}
void CWnd::OnDisplayChange(UINT p0, int p1, int p2)
{
    if (!m_hWnd) {
        return;
    }
    (void)p0;
    (void)p1;
    (void)p2;
    ::InvalidateRect(m_hWnd, nullptr, TRUE);
    ::UpdateWindow(m_hWnd);
}
LONGLONG CWnd::OnDragList(ULONGLONG p0, LONGLONG p1)
{
    if (!m_hWnd) {
        return 0;
    }

    const DRAGLISTINFO* pInfo = reinterpret_cast<const DRAGLISTINFO*>(static_cast<INT_PTR>(p1));
    HWND hList = (pInfo && pInfo->hWnd) ? pInfo->hWnd : reinterpret_cast<HWND>(static_cast<INT_PTR>(p0));
    if (!hList || !::IsWindow(hList)) {
        return 0;
    }

    UINT notification = pInfo ? pInfo->uNotification : static_cast<UINT>(p0);
    CPoint localPt{};
    if (pInfo) {
        localPt = CPoint(pInfo->ptCursor.x, pInfo->ptCursor.y);
    } else {
        POINT ptCursor{};
        ::GetCursorPos(&ptCursor);
        if (hList) {
            ::ScreenToClient(hList, &ptCursor);
            localPt = CPoint(ptCursor.x, ptCursor.y);
        }
    }

    int item = static_cast<int>(::SendMessageW(hList, LB_ITEMFROMPOINT, 0, MAKELPARAM(localPt.x, localPt.y)));
    if (item == LB_ERR) {
        return 0;
    }
    item = LOWORD(item);

    if (item >= 0) {
        item = LOWORD(item);
    }

    switch (notification) {
    case DL_BEGINDRAG:
        ::SendMessageW(hList, LB_SETTOPINDEX, static_cast<WPARAM>(item), 0);
        break;
    case DL_DRAGGING:
        ::SendMessageW(hList, LB_SETTOPINDEX, static_cast<WPARAM>(item), 0);
        break;
    case DL_DROPPED:
        ::SendMessageW(hList, LB_SETCURSEL, static_cast<WPARAM>(item), 0);
        break;
    case DL_CANCELDRAG:
        ::SendMessageW(hList, LB_SETCURSEL, static_cast<WPARAM>(-1), 0);
        break;
    default:
        break;
    }

    ::InvalidateRect(hList, nullptr, TRUE);
    return 1;
}
int CWnd::OnDrawItem(int p0, DRAWITEMSTRUCT* p1)
{
    (void)p0;
    if (!p1 || !p1->hDC) {
        return 0;
    }

    HDC hdc = p1->hDC;
    RECT rc = p1->rcItem;
    const WCHAR* label = reinterpret_cast<const WCHAR*>(p1->itemData);
    if (!label) {
        label = L"";
    }

    COLORREF textColor = (p1->itemState & ODS_SELECTED) ? GetSysColor(COLOR_HIGHLIGHTTEXT) : GetSysColor(COLOR_WINDOWTEXT);
    COLORREF backColor = (p1->itemState & ODS_SELECTED) ? GetSysColor(COLOR_HIGHLIGHT) : GetSysColor(COLOR_WINDOW);
    ::SetTextColor(hdc, textColor);
    ::SetBkColor(hdc, backColor);
    HBRUSH hBg = ::CreateSolidBrush(backColor);
    if (hBg) {
        ::FillRect(hdc, &rc, hBg);
        ::DeleteObject(hBg);
    }
    ::ExtTextOutW(hdc, rc.left + 2, rc.top + 1, 0, &rc, label, static_cast<UINT>(lstrlenW(label)), nullptr);
    if (p1->itemState & ODS_FOCUS) {
        ::DrawFocusRect(hdc, &rc);
    }
    return 1;
}
void CWnd::OnEnterIdle(UINT p0, void* p1)
{
    if (!m_hWnd) {
        return;
    }

    CWnd* pWho = static_cast<CWnd*>(p1);
    CString statusText;
    switch (p0) {
    case MSGF_MENU:
    case MSGF_DIALOGBOX:
    case MSGF_MESSAGEBOX:
        statusText = L"Message loop idle";
        break;
    default:
        statusText = L"Idle";
        break;
    }

    if (pWho && pWho->m_hWnd && pWho != this) {
        int titleLen = pWho->GetWindowTextLengthW();
        if (titleLen > 0) {
            CString titleText;
            wchar_t* pBuf = titleText.GetBuffer(titleLen + 1);
            int copied = pWho->GetWindowTextW(pBuf, titleLen + 1);
            titleText.ReleaseBuffer(copied);
            statusText = titleText.GetString();
        } else {
            statusText = L"Idle";
        }
    }

    CWnd_SetMessageText(this, statusText.GetString());
}
void CWnd::OnGesture(void* p0, ULONGLONG p1, LONGLONG p2)
{
    (void)p0;
    (void)p1;

    if (!m_hWnd || p2 == 0) {
        return;
    }

    HGESTUREINFO hGesture = reinterpret_cast<HGESTUREINFO>(static_cast<INT_PTR>(p2));
    GESTUREINFO gestureInfo {};
    gestureInfo.cbSize = sizeof(gestureInfo);
    if (!::GetGestureInfo(hGesture, &gestureInfo)) {
        return;
    }

    POINT ptScreen = { gestureInfo.ptsLocation.x, gestureInfo.ptsLocation.y };
    POINT ptClient = ptScreen;
    ::ScreenToClient(m_hWnd, &ptClient);
    CPoint point(ptClient.x, ptClient.y);

    int handled = FALSE;
    switch (gestureInfo.dwID) {
    case GID_PAN:
        handled = OnGesturePan(p0, point);
        break;
    case GID_PRESSANDTAP:
        handled = OnGesturePressAndTap(p0, point, gestureInfo.dwFlags);
        break;
    case GID_ROTATE:
        handled = OnGestureRotate(p0, point, static_cast<DWORD>(gestureInfo.ullArguments));
        break;
    case GID_TWOFINGERTAP:
        handled = OnGestureTwoFingerTap(p0, point);
        break;
    case GID_ZOOM:
        handled = OnGestureZoom(p0, point, static_cast<DWORD>(gestureInfo.ullArguments));
        break;
    default:
        break;
    }

    if (!handled) {
        CWnd_SetMessageText(this, L"Unhandled gesture message");
    }
    ::CloseGestureInfoHandle(hGesture);
}
int CWnd::OnGesturePan(void* p0, CPoint p1)
{
    if (!m_hWnd) {
        return FALSE;
    }

    POINT pt = {p1.x, p1.y};
    ::ClientToScreen(m_hWnd, &pt);
    HWND hChild = ::WindowFromPoint(pt);
    if (hChild && hChild != m_hWnd) {
        CWnd* pChild = CWnd::FromHandle(hChild);
        RECT rc{};
        if (pChild && pChild->m_hWnd && ::GetWindowRect(hChild, &rc)) {
            CPoint childPt{pt.x - rc.left, pt.y - rc.top};
            return pChild->OnGesturePan(p0, childPt);
        }
    }

    (void)p0;
    CWnd_SetMessageText(this, L"Gesture pan");
    return TRUE;
}
int CWnd::OnGesturePressAndTap(void* p0, CPoint p1, DWORD p2)
{
    if (!m_hWnd) {
        return FALSE;
    }

    POINT pt = {p1.x, p1.y};
    ::ClientToScreen(m_hWnd, &pt);
    HWND hChild = ::WindowFromPoint(pt);
    if (hChild && hChild != m_hWnd) {
        CWnd* pChild = CWnd::FromHandle(hChild);
        RECT rc{};
        if (pChild && pChild->m_hWnd && ::GetWindowRect(hChild, &rc)) {
            CPoint childPt{pt.x - rc.left, pt.y - rc.top};
            return pChild->OnGesturePressAndTap(p0, childPt, p2);
        }
    }

    HWND hContextWnd = hChild ? hChild : m_hWnd;
    if (hContextWnd) {
        ::PostMessageW(hContextWnd, WM_CONTEXTMENU, reinterpret_cast<WPARAM>(m_hWnd), MAKELPARAM(pt.x, pt.y));
    }
    CWnd_SetMessageText(this, L"Gesture press and tap");
    (void)p0;
    (void)p2;
    return TRUE;
}
int CWnd::OnGestureRotate(void* p0, CPoint p1, DWORD p2)
{
    if (!m_hWnd) {
        return FALSE;
    }

    POINT pt = {p1.x, p1.y};
    ::ClientToScreen(m_hWnd, &pt);
    HWND hChild = ::WindowFromPoint(pt);
    if (hChild && hChild != m_hWnd) {
        CWnd* pChild = CWnd::FromHandle(hChild);
        RECT rc{};
        if (pChild && pChild->m_hWnd && ::GetWindowRect(hChild, &rc)) {
            CPoint childPt{pt.x - rc.left, pt.y - rc.top};
            return pChild->OnGestureRotate(p0, childPt, p2);
        }
    }

    CString angleText;
    angleText.Format(L"Gesture rotate: %lu", static_cast<unsigned long>(p2));
    CWnd_SetMessageText(this, angleText.GetString());
    (void)p0;
    (void)p2;
    return TRUE;
}
int CWnd::OnGestureTwoFingerTap(void* p0, CPoint p1)
{
    if (!m_hWnd) {
        return FALSE;
    }

    POINT pt = {p1.x, p1.y};
    ::ClientToScreen(m_hWnd, &pt);
    HWND hChild = ::WindowFromPoint(pt);
    if (hChild && hChild != m_hWnd) {
        CWnd* pChild = CWnd::FromHandle(hChild);
        RECT rc{};
        if (pChild && pChild->m_hWnd && ::GetWindowRect(hChild, &rc)) {
            CPoint childPt{pt.x - rc.left, pt.y - rc.top};
            return pChild->OnGestureTwoFingerTap(p0, childPt);
        }
    }

    HWND hContextWnd = hChild ? hChild : m_hWnd;
    if (hContextWnd) {
        ::PostMessageW(hContextWnd, WM_CONTEXTMENU, reinterpret_cast<WPARAM>(m_hWnd), MAKELPARAM(pt.x, pt.y));
    }
    CWnd_SetMessageText(this, L"Gesture two-finger tap");
    (void)p0;
    return TRUE;
}
int CWnd::OnGestureZoom(void* p0, CPoint p1, DWORD p2)
{
    if (!m_hWnd) {
        return FALSE;
    }

    POINT pt = {p1.x, p1.y};
    ::ClientToScreen(m_hWnd, &pt);
    HWND hChild = ::WindowFromPoint(pt);
    if (hChild && hChild != m_hWnd) {
        CWnd* pChild = CWnd::FromHandle(hChild);
        RECT rc{};
        if (pChild && pChild->m_hWnd && ::GetWindowRect(hChild, &rc)) {
            CPoint childPt{pt.x - rc.left, pt.y - rc.top};
            return pChild->OnGestureZoom(p0, childPt, p2);
        }
    }

    if (p2) {
        ::SendMessageW(m_hWnd, WM_MOUSEWHEEL, MAKEWPARAM(0, p2 > 0 ? 120 : -120), MAKELPARAM(pt.x, pt.y));
    }
    CWnd_SetMessageText(this, L"Gesture zoom");
    (void)p0;
    (void)p2;
    return TRUE;
}
LONGLONG CWnd::OnGetObject(ULONGLONG p0, LONGLONG p1)
{
    if (!m_hWnd) {
        return 0;
    }

    if (p0 == 0 || p1 == 0) {
        return 0;
    }

    return static_cast<LONGLONG>(
        ::DefWindowProcW(m_hWnd, WM_GETOBJECT, static_cast<WPARAM>(p0), static_cast<LPARAM>(p1)));
}
void CWnd::OnHScroll(UINT p0, UINT p1, CScrollBar* p2)
{
    if (p2 && p2->m_hWnd) {
        ::SendMessageW(p2->m_hWnd, WM_HSCROLL, MAKELONG(p0, LOWORD(p1)), 0);
        return;
    }

    if (m_hWnd) {
        ::SendMessageW(m_hWnd, WM_HSCROLL, MAKEWPARAM(p0, LOWORD(p1)), 0);
    }
}
void CWnd::OnHelp()
{
    if (!m_hWnd) {
        return;
    }
    auto* pFrame = GetTopLevelFrame();
    if (pFrame) {
        static_cast<CFrameWnd*>(pFrame)->SetMessageText(L"Help requested");
    }
    MessageBoxW(L"Help requested", L"Help", MB_OK | MB_ICONINFORMATION);
}
void CWnd::OnHelpFinder()
{
    OnHelp();
}
void CWnd::OnHelpIndex()
{
    OnHelp();
}
BOOL CWnd::OnHelpInfo(HELPINFO* p0)
{
    if (!p0) {
        return FALSE;
    }
    HELPINFO info = *p0;
    if (!info.dwContextId) {
        return FALSE;
    }
    CString help;
    help.Format(L"Help requested (context: %lu)", static_cast<unsigned long>(info.dwContextId));
    if (m_hWnd) {
        MessageBoxW(help.GetString(), L"Help", MB_OK | MB_ICONINFORMATION);
        return TRUE;
    }
    return FALSE;
}
void CWnd::OnHelpUsing()
{
    OnHelp();
}
void CWnd::OnMeasureItem(int p0, MEASUREITEMSTRUCT* p1)
{
    if (!p1) {
        return;
    }
    p1->itemWidth = p0;
    p1->itemHeight = 16;
}
LONGLONG CWnd::OnNTCtlColor(ULONGLONG p0, LONGLONG p1)
{
    if (!m_hWnd) {
        return 0;
    }

    (void)p0;
    if (p1 >= 0 && p1 <= 0x0F) {
        return static_cast<LONGLONG>(reinterpret_cast<LONG_PTR>(::GetSysColorBrush(static_cast<int>(p1))));
    }

    return static_cast<LONGLONG>(reinterpret_cast<LONG_PTR>(::GetSysColorBrush(COLOR_WINDOW)));
}
int CWnd::OnNcDestroy()
{
    if (m_hWnd) {
        CleanupWindowRuntimeState(this);
    }
    return 1;
}
void CWnd::OnPaint()
{
    if (!m_hWnd) {
        return;
    }
    PAINTSTRUCT ps {};
    HDC hdc = ::BeginPaint(m_hWnd, &ps);
    if (!hdc) {
        return;
    }
    HBRUSH brush = static_cast<HBRUSH>(::GetStockObject(WHITE_BRUSH));
    ::FillRect(hdc, &ps.rcPaint, brush);
    ::EndPaint(m_hWnd, &ps);
}
int CWnd::OnParentNotify(UINT p0, LONGLONG p1)
{
    if (!m_hWnd) {
        return 0;
    }
    return static_cast<int>(::SendMessageW(m_hWnd, p0, static_cast<WPARAM>(p1), static_cast<LPARAM>(p1)));
}
void CWnd::OnSetFocus(void* p0)
{
    if (!p0) {
        return;
    }
    if (m_hWnd) {
        ::SetFocus(m_hWnd);
    }
}
void CWnd::OnSettingChange(UINT p0, const WCHAR* p1)
{
    if (!m_hWnd) {
        return;
    }
    ::SendMessageW(m_hWnd, WM_SETTINGCHANGE, static_cast<WPARAM>(p0), reinterpret_cast<LPARAM>(p1));
}
void CWnd::OnSysColorChange()
{
    if (!m_hWnd) {
        return;
    }
    ::InvalidateRect(m_hWnd, nullptr, TRUE);
}
LONGLONG CWnd::OnTabletQuerySystemGestureStatus(ULONGLONG p0, LONGLONG p1)
{
    (void)p1;
    if (!m_hWnd) {
        return 0;
    }
    return GetWindowRuntimeState(this).gestureConfigEnabled ? static_cast<LONGLONG>(p0) : 0;
}
void CWnd::OnTouchInput(ULONGLONG p0, ULONGLONG p1, TOUCHINPUT* p2, UINT p3)
{
    if (!m_hWnd || !p2) {
        return;
    }

    (void)p0;
    (void)p1;
    // Convert coordinates into pixels (TouchInput stores 1/100 pixel units).
    for (UINT i = 0; i < p3; ++i) {
        TOUCHINPUT& input = p2[i];
        POINT pt = { static_cast<LONG>(input.x / 100), static_cast<LONG>(input.y / 100) };
        ::ScreenToClient(m_hWnd, &pt);
        input.x = static_cast<long>(pt.x);
        input.y = static_cast<long>(pt.y);
    }
}
int CWnd::OnTouchInputs(ULONGLONG p0, ULONGLONG p1, TOUCHINPUT* p2, UINT p3)
{
    if (!m_hWnd || !p2 || p3 == 0) {
        return FALSE;
    }

    for (UINT i = 0; i < p3; ++i) {
        OnTouchInput(p0, p1, &p2[i], 1);
    }
    return static_cast<int>(p3);
}
LONGLONG CWnd::OnTouchMessage(ULONGLONG p0, LONGLONG p1)
{
    if (!m_hWnd || p0 == 0 || p1 == 0) {
        return 0;
    }

    UINT nInputs = static_cast<UINT>(p0);
    if (nInputs == 0) {
        return 0;
    }

    HTOUCHINPUT hInput = reinterpret_cast<HTOUCHINPUT>(static_cast<INT_PTR>(p1));
    std::vector<TOUCHINPUT> inputs(nInputs);
    int result = 0;
    if (::GetTouchInputInfo(hInput, nInputs, inputs.data(), sizeof(TOUCHINPUT))) {
        result = OnTouchInputs(p0, static_cast<ULONGLONG>(p1), inputs.data(), nInputs);
    }
    ::CloseTouchInputHandle(hInput);
    return result;
}
int CWnd::OnVKeyToItem(UINT p0, CListBox* p1, UINT p2)
{
    if (!p1 || !p1->m_hWnd) {
        return -1;
    }

    int itemCount = static_cast<int>(::SendMessageW(p1->m_hWnd, LB_GETCOUNT, 0, 0));
    if (itemCount <= 0) {
        return -1;
    }

    int current = static_cast<int>(p2);
    if (p0 == VK_UP) {
        return current > 0 ? current - 1 : -1;
    }
    if (p0 == VK_DOWN) {
        return (current + 1 < itemCount) ? current + 1 : -1;
    }
    if (p0 == VK_HOME) {
        return itemCount > 0 ? 0 : -1;
    }
    if (p0 == VK_END) {
        return itemCount > 0 ? itemCount - 1 : -1;
    }

    UINT mapped = ::MapVirtualKeyW(p0, MAPVK_VK_TO_CHAR);
    if (!mapped || mapped == 0xFF) {
        return -1;
    }

    wchar_t keyText[2] = { static_cast<wchar_t>(mapped & 0xFFFF), 0 };
    if (!keyText[0]) {
        return -1;
    }
    return static_cast<int>(::SendMessageW(p1->m_hWnd, LB_FINDSTRING, current, reinterpret_cast<LPARAM>(keyText)));
}
void CWnd::OnVScroll(UINT p0, UINT p1, CScrollBar* p2)
{
    if (p2 && p2->m_hWnd) {
        ::SendMessageW(p2->m_hWnd, WM_VSCROLL, MAKEWPARAM(p1, 0), 0);
    }
}
BOOL CWnd::PaintWindowlessControls(CDC* p0)
{
    (void)p0;
    if (!m_hWnd) {
        return FALSE;
    }
    return ::InvalidateRect(m_hWnd, nullptr, FALSE);
}
void CWnd::PostNcDestroy()
{
    if (m_hWnd) {
        CleanupWindowRuntimeState(this);
        g_hwndMap.erase(m_hWnd);
        m_hWnd = nullptr;
    }
}
BOOL CWnd::PreTranslateInput(MSG* p0)
{
    if (!m_hWnd || !p0) {
        return FALSE;
    }

    // Translate system-level input to the same path used for MFC's
    // CWnd::PreTranslateMessage().
    return CWnd_PreTranslateMessageCompat(this, p0);
}
void CWnd::PrepareForHelp()
{
    OnHelp();
}
int CWnd::ReflectChildNotify(UINT p0, ULONGLONG p1, LONGLONG p2, LONGLONG* p3)
{
    if (OnChildNotify(p0, p1, p2, p3)) {
        return TRUE;
    }
    return 0;
}
int CWnd::RegisterTouchWindow(ULONGLONG p0)
{
    if (!m_hWnd) {
        return FALSE;
    }
    return ::RegisterTouchWindow(m_hWnd, static_cast<ULONG_PTR>(p0)) ? TRUE : FALSE;
}
void CWnd::RemoveRadioCheckFromGroup(const CObject* p0)
{
    (void)p0;
    if (!m_hWnd) {
        return;
    }
    for (HWND hChild = ::GetWindow(m_hWnd, GW_CHILD); hChild; hChild = ::GetWindow(hChild, GW_HWNDNEXT)) {
        DWORD style = static_cast<DWORD>(::GetWindowLongPtrW(hChild, GWL_STYLE));
        if (style & BS_AUTORADIOBUTTON) {
            ::SendMessageW(hChild, BM_SETCHECK, BST_UNCHECKED, 0);
        }
    }
}
void CWnd::RepositionBars(UINT p0, UINT p1, UINT p2, UINT p3, RECT* p4, RECT* p5, int p6)
{
    (void)p0;
    (void)p1;
    (void)p2;
    (void)p3;
    (void)p5;
    (void)p6;
    if (m_hWnd && p4) {
        ::MoveWindow(m_hWnd, p4->left, p4->top, p4->right - p4->left, p4->bottom - p4->top, TRUE);
    }
}
LONGLONG CWnd::RunModalLoop(DWORD p0)
{
    (void)p0;
    if (!m_hWnd) {
        return 0;
    }
    MSG msg {};
    while (::GetMessageW(&msg, nullptr, 0, 0) > 0) {
        if (!CWnd_PreTranslateMessageCompat(this, &msg)) {
            ::TranslateMessage(&msg);
            ::DispatchMessageW(&msg);
        }
        if (msg.message == WM_QUIT) {
            return static_cast<LONGLONG>(msg.wParam);
        }
    }
    return 0;
}
void CWnd::ScreenToClient(RECT* p0) const
{
    if (!m_hWnd || !p0) {
        return;
    }

    POINT points[2] = {
        {p0->left, p0->top},
        {p0->right, p0->bottom}
    };

    if (::MapWindowPoints(HWND_DESKTOP, m_hWnd, points, 2) != 0) {
        p0->left = points[0].x;
        p0->top = points[0].y;
        p0->right = points[1].x;
        p0->bottom = points[1].y;
    }
}
void CWnd::ScrollWindow(int p0, int p1, const RECT* p2, const RECT* p3)
{
    if (!m_hWnd) {
        return;
    }

    ::ScrollWindow(m_hWnd, p0, p1, p2, p3);
}
int CWnd::ScrollWindowEx(int p0, int p1, const RECT* p2, const RECT* p3, CRgn* p4, RECT* p5, UINT p6)
{
    if (!m_hWnd) {
        return FALSE;
    }

    HRGN hRgn = p4 ? static_cast<HRGN>(p4->m_hObject) : nullptr;
    return ::ScrollWindowEx(m_hWnd, p0, p1, p2, p3, hRgn, p5, p6);
}
int CWnd::SendChildNotifyLastMsg(LONGLONG* p0)
{
    const MSG* pMsg = GetCurrentMessage();
    if (!pMsg) {
        if (p0) {
            *p0 = 0;
        }
        return FALSE;
    }

    LONGLONG result = 0;
    if (!OnChildNotify(pMsg->message,
                       static_cast<ULONGLONG>(pMsg->wParam),
                       static_cast<LONGLONG>(pMsg->lParam),
                       &result)) {
        if (p0) {
            *p0 = 0;
        }
        return FALSE;
    }

    if (p0) {
        *p0 = result;
    }
    return TRUE;
}
LONGLONG CWnd::SendDlgItemMessageW(int p0, UINT p1, ULONGLONG p2, LONGLONG p3)
{
    if (!m_hWnd) {
        return 0;
    }
    return ::SendDlgItemMessageW(m_hWnd, p0, p1, p2, p3);
}
BOOL CWnd::SetDlgCtrlID(int p0)
{
    if (!m_hWnd) {
        return FALSE;
    }
    ::SetLastError(ERROR_SUCCESS);
    LONG_PTR previous = ::SetWindowLongPtrW(m_hWnd, GWL_ID, static_cast<LONG_PTR>(p0));
    return previous != 0 || ::GetLastError() == ERROR_SUCCESS;
}
void CWnd::SetDlgItemInt(int p0, UINT p1, int p2)
{
    if (!m_hWnd) {
        return;
    }
    ::SetDlgItemInt(m_hWnd, p0, p1, p2 != FALSE);
}
void CWnd::SetDlgItemTextW(int p0, const WCHAR* p1)
{
    if (!m_hWnd) {
        return;
    }
    ::SetDlgItemTextW(m_hWnd, p0, p1 ? p1 : L"");
}
void* CWnd::SetFocus()
{
    if (!m_hWnd) {
        return nullptr;
    }
    HWND hWndOld = ::SetFocus(m_hWnd);
    return hWndOld ? CWnd::FromHandle(hWndOld) : nullptr;
}
int CWnd::SetGestureConfig(CGestureConfig* p0)
{
    if (!m_hWnd) {
        return FALSE;
    }
    auto& state = GetWindowRuntimeState(this);
    state.gestureConfigEnabled = (p0 != nullptr);
    return TRUE;
}
BOOL CWnd::SetOccDialogInfo(_AFX_OCC_DIALOG_INFO* p0)
{
    GetWindowRuntimeState(this).properties[{0x4F43, 0}] = p0;
    return TRUE;
}
int CWnd::SetProperty(long p0, WORD p1, void* p2)
{
    auto& state = GetWindowRuntimeState(this);
    state.properties[{p0, p1}] = p2;
    return TRUE;
}
int CWnd::SetScrollInfo(int p0, SCROLLINFO* p1, int p2)
{
    if (!m_hWnd || !p1) {
        return FALSE;
    }
    p1->cbSize = sizeof(*p1);
    return ::SetScrollInfo(m_hWnd, p0, p1, p2 ? TRUE : FALSE);
}
int CWnd::SetScrollPos(int p0, int p1, int p2)
{
    if (!m_hWnd) {
        return FALSE;
    }
    return ::SetScrollPos(m_hWnd, p0, p1, p2 ? TRUE : FALSE);
}
void CWnd::SetScrollRange(int p0, int p1, int p2, int p3)
{
    if (!m_hWnd) {
        return;
    }
    ::SetScrollRange(m_hWnd, p0, p1, p2, p3 ? TRUE : FALSE);
}
int CWnd::SetWindowPlacement(const WINDOWPLACEMENT* p0)
{
    if (!m_hWnd || !p0) {
        return FALSE;
    }
    return ::SetWindowPlacement(m_hWnd, p0) ? TRUE : FALSE;
}
int CWnd::SubclassDlgItem(UINT p0, void* p1)
{
    if (!m_hWnd || !p0 || !p1) {
        return FALSE;
    }
    CWnd* pWnd = static_cast<CWnd*>(p1);
    HWND hChild = ::GetDlgItem(m_hWnd, p0);
    if (!hChild) {
        return FALSE;
    }
    return pWnd->SubclassWindow(hChild) ? TRUE : FALSE;
}
int CWnd::SubclassWindow(HWND p0)
{
    if (!p0) {
        return FALSE;
    }

    WNDPROC previous = reinterpret_cast<WNDPROC>(::GetWindowLongPtrW(p0, GWLP_WNDPROC));
    if (!previous) {
        return FALSE;
    }

    if (m_hWnd) {
        UnsubclassWindow();
    }

    CWnd* old = OpenMfcLookupCWnd(p0);
    if (old && old != this) {
        return FALSE;
    }

    g_hwndMap[p0] = this;
    m_pfnSuper = previous;
    m_hWnd = p0;
    ::SetWindowLongPtrW(p0, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(AfxWndProc));
    return TRUE;
}
HWND CWnd::UnsubclassWindow()
{
    if (!m_hWnd) {
        return nullptr;
    }

    HWND hWnd = m_hWnd;
    if (m_pfnSuper) {
        ::SetWindowLongPtrW(m_hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(m_pfnSuper));
        m_pfnSuper = nullptr;
    }
    g_hwndMap.erase(m_hWnd);
    m_hWnd = nullptr;
    return hWnd;
}
int CWnd::UpdateData(int p0)
{
    if (!m_hWnd) {
        return FALSE;
    }

    UpdateDialogControls(this, p0);
    return TRUE;
}
void CWnd::UpdateDialogControls(void* p0, int p1)
{
    (void)p0;
    if (!m_hWnd) {
        return;
    }

    for (HWND hChild = ::GetWindow(m_hWnd, GW_CHILD); hChild; hChild = ::GetWindow(hChild, GW_HWNDNEXT)) {
        if (!::IsWindow(hChild)) {
            continue;
        }

        // Touch current caption text for each child window to mimic lightweight dialog
        // validation/update without depending on framework DDX metadata.
        int len = ::GetWindowTextLengthW(hChild);
        if (len > 0) {
            std::vector<wchar_t> buf(static_cast<size_t>(len) + 1);
            ::GetWindowTextW(hChild, buf.data(), len + 1);
            if (p1) {
                ::SetWindowTextW(hChild, buf.data());
            }
        }
    }
}
void CWnd::EnableD2DSupport(int p0, int p1)
{
    (void)p1;
    GetWindowRuntimeState(this).d2dSupportEnabled = p0 != FALSE;
}
void CWnd::EnableDynamicLayout(int p0)
{
    GetWindowRuntimeState(this).dynamicLayoutEnabled = p0 != FALSE;
}
CDCRenderTarget* CWnd::GetDCRenderTarget()
{
    auto it = g_wndRuntimeStates.find(this);
    if (it == g_wndRuntimeStates.end() || !it->second.d2dSupportEnabled) {
        return nullptr;
    }
    auto& state = GetWindowRuntimeState(this);
    auto dcTargetIt = state.properties.find({0x4F44, 0});
    if (dcTargetIt == state.properties.end() || !dcTargetIt->second) {
        void*& slot = state.properties[{0x4F44, 0}];
        slot = new CDCRenderTarget();
        return static_cast<CDCRenderTarget*>(slot);
    }
    return static_cast<CDCRenderTarget*>(dcTargetIt->second);
}
void CWnd::InitDynamicLayout()
{
    GetWindowRuntimeState(this).dynamicLayoutEnabled = true;
}
int CWnd::LoadDynamicLayoutResource(const WCHAR* p0)
{
    if (!m_hWnd) {
        return FALSE;
    }
    GetWindowRuntimeState(this).dynamicLayoutResource = p0 ? p0 : L"";
    GetWindowRuntimeState(this).dynamicLayoutEnabled = true;
    return TRUE;
}
void CWnd::ResizeDynamicLayout()
{
    if (!m_hWnd || !g_wndRuntimeStates[this].dynamicLayoutEnabled) {
        return;
    }
    ::InvalidateRect(m_hWnd, nullptr, TRUE);
}
int CWnd::IsD2DSupportEnabled()
{
    return g_wndRuntimeStates[this].d2dSupportEnabled ? TRUE : FALSE;
}
void CWnd::OnDrawIconicThumbnailOrLivePreview(CDC*& p0, CRect p1, CSize p2, int p3, int*& p4)
{
    if (!m_hWnd) {
        return;
    }

    if (!p0 || !p0->m_hDC) {
        return;
    }

    RECT bounds = {p1.left, p1.top, p1.right, p1.bottom};
    if (bounds.right <= bounds.left || bounds.bottom <= bounds.top) {
        bounds.left = 0;
        bounds.top = 0;
        bounds.right = p2.cx > 0 ? p2.cx : 256;
        bounds.bottom = p2.cy > 0 ? p2.cy : 256;
    }

    HDC hdc = p0->m_hDC;
    UINT printFlags = PRF_CLIENT | PRF_ERASEBKGND | PRF_CHILDREN;
    if (p3) {
        printFlags |= PRF_NONCLIENT;
    }

    ::SendMessageW(m_hWnd, WM_PRINT, reinterpret_cast<WPARAM>(hdc), printFlags);

    if (p4) {
        *p4 = bounds.right - bounds.left;
    }
}
