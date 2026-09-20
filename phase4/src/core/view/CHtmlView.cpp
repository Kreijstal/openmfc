// CHtmlView — OpenMFC implementation.
// Sources: viewrich.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/ViewrichSupport.h"
#include <mshtmcid.h>
#include <docobj.h>

// ---------------------------------------------------------------------------
// Retail layout note (mfc140u, read from the disassembly of the bodies below):
//   CHtmlView derives from CFormView (afxhtml.h:87), sizeof(CFormView) == 0x160,
//   then `CWnd m_wndBrowser` at +0x160 (its m_hWnd read at +0x40 inside it) and
//   `CComPtr<IWebBrowser2> m_pBrowserApp` at +0x248; sizeof == 0x250.
// This repo's header (include/openmfc/afxwin.h:3664) derives CHtmlView from
// CView (0xf8) and holds `IWebBrowser2* m_pBrowser` (+0xf8), `CWnd* m_pControlWnd`
// (+0x100), `BOOL m_bCreated` (+0x108) and a padding array up to 0x250.  The
// file's existing convention, followed here, is m_pBrowser == retail
// m_pBrowserApp and m_pControlWnd == &retail m_wndBrowser.  Never re-declare.
// ---------------------------------------------------------------------------

// Thunks defined elsewhere in the tree (definitions verified by grep).
extern "C" __int64 MS_ABI impl__Default_CWnd__IEAA_JXZ(CWnd* pThis);                              // core/window/Thunks.cpp
extern "C" int MS_ABI impl__PreCreateWindow_CView__MEAAHAEAUtagCREATESTRUCTW___Z(CView*, CREATESTRUCTW*); // core/view/CView.cpp
extern "C" unsigned long MS_ABI impl__GetStyle_CWnd__QEBAKXZ(const CWnd* pThis);                   // core/window/Thunks.cpp
extern "C" int MS_ABI impl__SetWindowPos_CWnd__QEAAHPEBV1_HHHHI_Z(
    CWnd* pThis, const CWnd* pWndInsertAfter, int x, int y, int cx, int cy, unsigned int nFlags);   // core/window/CWnd.cpp
extern "C" CFrameWnd* MS_ABI impl__GetParentFrame_CWnd__QEBAPEAVCFrameWnd__XZ(const CWnd* pThis); // core/window/Thunks.cpp
extern "C" int MS_ABI impl__IsFrameWnd_CWnd__UEBAHXZ(const CWnd* pThis);                           // core/window/CWnd.cpp
extern "C" int MS_ABI impl__PreTranslateMessage_CFrameWnd__UEAAHPEAUtagMSG___Z(CFrameWnd*, MSG*);  // core/frame/CFrameWnd.cpp
extern "C" int MS_ABI impl__PreTranslateMessage_CWnd__UEAAHPEAUtagMSG___Z(CWnd*, MSG*);            // core/window/CWnd.cpp
extern "C" void MS_ABI impl__AfxThrowInvalidArgException__YAXXZ();                                 // detail/MfcExceptionsSupport.cpp
extern "C" void MS_ABI impl__AfxThrowUserException__YAXXZ();                                       // detail/MfcExceptionsSupport.cpp
extern "C" void MS_ABI impl__AfxCheckError__YAXJ_Z(long hr);                                       // featurepack/CMFC_misc_stubs.cpp
extern "C" void* MS_ABI impl___0CByteArray__QEAA_XZ(CByteArray* pThis);                            // core/collections/CByteArray.cpp
extern "C" void MS_ABI impl___1CByteArray__UEAA_XZ(CByteArray* pThis);                             // core/collections/CByteArray.cpp
extern "C" void MS_ABI impl__SetSize_CByteArray__QEAAX_J0_Z(CByteArray*, long long, long long);    // core/collections/CByteArray.cpp
extern "C" void MS_ABI impl__SetAtGrow_CByteArray__QEAAX_JE_Z(CByteArray*, long long, unsigned char); // core/collections/CByteArray.cpp
extern "C" void* MS_ABI impl___0CHtmlControlSite__QEAA_PEAVCOleControlContainer___Z(void* pThis, void* pContainer); // core/ole/CHtmlControlSite.cpp
extern "C" const void* MS_ABI impl__GetThisEventSinkMap_CCmdTarget__KAPEBUAFX_EVENTSINKMAP__XZ();  // core/runtime/CCmdTarget.cpp
extern "C" const COleVariant* MS_ABI impl___4COleVariant__QEAAAEBV0_AEBVCByteArray___Z(COleVariant* pThis, const CByteArray* arrSrc); // detail/OlecoreSupport.cpp

// Thunks defined later in this file and referenced by the event sink map.
extern "C" void MS_ABI impl__NavigateComplete2_CHtmlView__MEAAXPEAUIDispatch__PEAUtagVARIANT___Z(CHtmlView*, IDispatch*, VARIANT*);
extern "C" void MS_ABI impl__BeforeNavigate2_CHtmlView__MEAAXPEAUIDispatch__PEAUtagVARIANT__1111PEAF_Z(
    CHtmlView*, IDispatch*, VARIANT*, VARIANT*, VARIANT*, VARIANT*, VARIANT*, short*);
extern "C" void MS_ABI impl__DocumentComplete_CHtmlView__MEAAXPEAUIDispatch__PEAUtagVARIANT___Z(CHtmlView*, IDispatch*, VARIANT*);
extern "C" void MS_ABI impl__NavigateError_CHtmlView__MEAAXPEAUIDispatch__PEAUtagVARIANT__11PEAF_Z(
    CHtmlView*, IDispatch*, VARIANT*, VARIANT*, VARIANT*, short*);
extern "C" long MS_ABI impl__ExecFormsCommand_CHtmlView__QEAAJKPEAUtagVARIANT__0_Z(CHtmlView*, unsigned long, VARIANT*, VARIANT*);
extern "C" long MS_ABI impl__QueryFormsCommand_CHtmlView__QEAAJKPEAH00_Z(CHtmlView*, unsigned long, int*, int*, int*);
extern "C" void MS_ABI impl__OnStatusTextChange_CHtmlView__UEAAXPEB_W_Z(CHtmlView*, const wchar_t*);
extern "C" void MS_ABI impl__OnProgressChange_CHtmlView__UEAAXJJ_Z(CHtmlView*, long, long);
extern "C" void MS_ABI impl__OnCommandStateChange_CHtmlView__UEAAXJH_Z(CHtmlView*, long, int);
extern "C" void MS_ABI impl__OnDownloadBegin_CHtmlView__UEAAXXZ(CHtmlView*);
extern "C" void MS_ABI impl__OnDownloadComplete_CHtmlView__UEAAXXZ(CHtmlView*);
extern "C" void MS_ABI impl__OnTitleChange_CHtmlView__UEAAXPEB_W_Z(CHtmlView*, const wchar_t*);
extern "C" void MS_ABI impl__OnPropertyChange_CHtmlView__UEAAXPEB_W_Z(CHtmlView*, const wchar_t*);
extern "C" void MS_ABI impl__OnNewWindow2_CHtmlView__UEAAXPEAPEAUIDispatch__PEAH_Z(CHtmlView*, IDispatch**, BOOL*);
extern "C" void MS_ABI impl__OnQuit_CHtmlView__UEAAXXZ(CHtmlView*);
extern "C" void MS_ABI impl__OnVisible_CHtmlView__UEAAXH_Z(CHtmlView*, int);
extern "C" void MS_ABI impl__OnToolBar_CHtmlView__UEAAXH_Z(CHtmlView*, int);
extern "C" void MS_ABI impl__OnMenuBar_CHtmlView__UEAAXH_Z(CHtmlView*, int);
extern "C" void MS_ABI impl__OnStatusBar_CHtmlView__UEAAXH_Z(CHtmlView*, int);
extern "C" void MS_ABI impl__OnFullScreen_CHtmlView__UEAAXH_Z(CHtmlView*, int);
extern "C" void MS_ABI impl__OnTheaterMode_CHtmlView__UEAAXH_Z(CHtmlView*, int);

namespace {

// GUIDs the retail bodies reference by address (bytes read from mfc140u .rdata):
//   0x18034f0a0 = {332C4425-26CB-11D0-B483-00C04FD90119}  IID_IHTMLDocument2
//   0x18034f080 = {B722BCCB-4E68-101B-A2BC-00AA00404770}  IID_IOleCommandTarget
//   0x18034f090 = {00000117-0000-0000-C000-000000000046}  IID_IOleInPlaceActiveObject
//   0x1802d9fd0 = {DE4BA900-59CA-11CF-9592-444553540000}  CGID_MSHTML
const GUID kIID_IHTMLDocument2         = { 0x332C4425, 0x26CB, 0x11D0, { 0xB4,0x83,0x00,0xC0,0x4F,0xD9,0x01,0x19 } };
const GUID kIID_IOleCommandTarget      = { 0xB722BCCB, 0x4E68, 0x101B, { 0xA2,0xBC,0x00,0xAA,0x00,0x40,0x47,0x70 } };
const GUID kIID_IOleInPlaceActiveObject= { 0x00000117, 0x0000, 0x0000, { 0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x46 } };
const GUID kCGID_MSHTML                = { 0xDE4BA900, 0x59CA, 0x11CF, { 0x95,0x92,0x44,0x45,0x53,0x54,0x00,0x00 } };

// CString from a BSTR that may be NULL (retail 0x18000dcb0 is the CStringT
// ctor from LPCWSTR, which treats NULL as the empty string).
inline CString StringFromBstr(BSTR b) { return CString(b ? b : L""); }

// Mirror of the private AFX_EVENTSINKMAP structs pinned in
// core/runtime/CCmdTarget.cpp (AFX_DISPMAP_ENTRY 0x40 bytes, entry 0x48).
enum EvDispFlags_ { evDispCustom_ = 0 };
struct EvDispMapEntry_ {
    const wchar_t*  lpszName;     // +0x00
    long            lDispID;      // +0x08
    const char*     lpszParams;   // +0x10
    unsigned short  vt;           // +0x18
    const void*     pfn;          // +0x20
    const void*     pfnSet;       // +0x28
    size_t          nPropOffset;  // +0x30
    EvDispFlags_    flags;        // +0x38
};
struct EvSinkMapEntry_ {
    EvDispMapEntry_ dispEntry;    // +0x00
    unsigned int    nCtrlIDFirst; // +0x40
    unsigned int    nCtrlIDLast;  // +0x44
};
struct EvSinkMap_ {
    const void* (MS_ABI* pfnGetBaseMap)();
    const EvSinkMapEntry_* lpEntries;
    unsigned int* lpEntryCount;
};
static_assert(sizeof(EvDispMapEntry_) == 0x40, "AFX_DISPMAP_ENTRY is 0x40 bytes");
static_assert(sizeof(EvSinkMapEntry_) == 0x48, "AFX_EVENTSINKMAP_ENTRY stride is 0x48");
static_assert(offsetof(EvSinkMapEntry_, nCtrlIDFirst) == 0x40, "nCtrlIDFirst at +0x40");
static_assert(sizeof(EvSinkMap_) == 0x18, "AFX_EVENTSINKMAP is three pointers");

} // namespace

// Symbol: ??0CHtmlView@@IEAA@XZ
extern "C" void* MS_ABI impl___0CHtmlView__IEAA_XZ(CHtmlView* pThis) {
    new (pThis) CHtmlView();
    return pThis;
}
// Symbol: ??1CHtmlView@@UEAA@XZ
extern "C" void MS_ABI impl___1CHtmlView__UEAA_XZ(CHtmlView* pThis) {
    pThis->~CHtmlView();
}
// Symbol: ?Create@CHtmlView@@UEAAHPEB_W0KAEBUtagRECT@@PEAVCWnd@@IPEAUCCreateContext@@@Z
extern "C" int MS_ABI impl__Create_CHtmlView__UEAAHPEB_W0KAEBUtagRECT__PEAVCWnd__IPEAUCCreateContext___Z(
    CHtmlView* pThis, const wchar_t* lpszClassName, const wchar_t* lpszWindowName,
    unsigned long dwStyle, const RECT& rect, CWnd* pParentWnd, unsigned int nID,
    CCreateContext* pContext) {
    return pThis->CHtmlView::Create(lpszClassName, lpszWindowName, dwStyle, rect,
                                     pParentWnd, nID, pContext);
}
// Symbol: ?OnDraw@CHtmlView@@UEAAXPEAVCDC@@@Z
extern "C" void MS_ABI impl__OnDraw_CHtmlView__UEAAXPEAVCDC___Z(CHtmlView* pThis, CDC* pDC) {
    pThis->CHtmlView::OnDraw(pDC);
}
// Symbol: ?Navigate@CHtmlView@@QEAAXPEB_WK00PEAXK@Z
extern "C" void MS_ABI impl__Navigate_CHtmlView__QEAAXPEB_WK00PEAXK_Z(
    CHtmlView* pThis, const wchar_t* lpszURL, unsigned long dwFlags,
    const wchar_t* lpszTargetFrameName, const wchar_t* lpszHeaders,
    void* lpvPostData, unsigned long dwPostDataLen) {
    pThis->Navigate(lpszURL, dwFlags, lpszTargetFrameName, lpszHeaders,
                    lpvPostData, dwPostDataLen);
}
// Symbol: ?Navigate2@CHtmlView@@QEAAXPEB_WK00PEAXK@Z
extern "C" void MS_ABI impl__Navigate2_CHtmlView__QEAAXPEB_WK00PEAXK_Z(
    CHtmlView* pThis, const wchar_t* lpszURL, unsigned long dwFlags,
    const wchar_t* lpszTargetFrameName, const wchar_t* lpszHeaders,
    void* lpvPostData, unsigned long dwPostDataLen) {
    pThis->Navigate2(lpszURL, dwFlags, lpszTargetFrameName, lpszHeaders,
                     lpvPostData, dwPostDataLen);
}
// Symbol: ?Navigate2@CHtmlView@@QEAAXPEFAU_ITEMIDLIST@@KPEB_W@Z
extern "C" void MS_ABI impl__Navigate2_CHtmlView__QEAAXPEFAU_ITEMIDLIST__KPEB_W_Z(
    CHtmlView* pThis, LPITEMIDLIST pIDL, unsigned long dwFlags,
    const wchar_t* lpszTargetFrameName) {
    pThis->Navigate2(pIDL, dwFlags, lpszTargetFrameName);
}
// Symbol: ?GoBack@CHtmlView@@QEAAXXZ
extern "C" void MS_ABI impl__GoBack_CHtmlView__QEAAXXZ(CHtmlView* pThis) { pThis->GoBack(); }
// Symbol: ?GoForward@CHtmlView@@QEAAXXZ
extern "C" void MS_ABI impl__GoForward_CHtmlView__QEAAXXZ(CHtmlView* pThis) { pThis->GoForward(); }
// Symbol: ?GoHome@CHtmlView@@QEAAXXZ
extern "C" void MS_ABI impl__GoHome_CHtmlView__QEAAXXZ(CHtmlView* pThis) { pThis->GoHome(); }
// Symbol: ?GoSearch@CHtmlView@@QEAAXXZ
extern "C" void MS_ABI impl__GoSearch_CHtmlView__QEAAXXZ(CHtmlView* pThis) { pThis->GoSearch(); }
// Symbol: ?Stop@CHtmlView@@QEAAXXZ
extern "C" void MS_ABI impl__Stop_CHtmlView__QEAAXXZ(CHtmlView* pThis) { pThis->Stop(); }
// Symbol: ?Refresh@CHtmlView@@QEAAXXZ
extern "C" void MS_ABI impl__Refresh_CHtmlView__QEAAXXZ(CHtmlView* pThis) { pThis->Refresh(); }
// Symbol: ?Refresh2@CHtmlView@@QEAAXH@Z
extern "C" void MS_ABI impl__Refresh2_CHtmlView__QEAAXH_Z(CHtmlView* pThis, int nLevel) {
    pThis->Refresh2(nLevel);
}
// Symbol: ?GetBusy@CHtmlView@@QEBAHXZ
extern "C" int MS_ABI impl__GetBusy_CHtmlView__QEBAHXZ(const CHtmlView* pThis) {
    return pThis->GetBusy();
}
// Symbol: ?GetReadyState@CHtmlView@@QEBA?AW4tagREADYSTATE@@XZ
extern "C" READYSTATE MS_ABI impl__GetReadyState_CHtmlView__QEBA_AW4tagREADYSTATE__XZ(
    const CHtmlView* pThis) {
    return pThis->GetReadyState();
}
// CString returned by value: MSVC x64 passes `this` in RCX and the hidden
// return slot in RDX for member functions.  Verified on retail
// CHtmlView::GetLocationName (RVA 0x27b7b0, mfc140u): `mov 0x248(%rcx),%rcx`
// reads m_pBrowserApp through RCX and the result is stored via `mov %rax,(%r15)`
// with r15 = RDX.  These four thunks previously took (pRet, pThis) -- wrong order.
// Symbol: ?GetLocationName@CHtmlView@@QEBA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@XZ
extern "C" void MS_ABI impl__GetLocationName_CHtmlView__QEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__XZ(
    const CHtmlView* pThis, CString* pRet) {
    new (pRet) CString(pThis->GetLocationName());
}
// Symbol: ?GetLocationURL@CHtmlView@@QEBA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@XZ
extern "C" void MS_ABI impl__GetLocationURL_CHtmlView__QEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__XZ(
    const CHtmlView* pThis, CString* pRet) {
    new (pRet) CString(pThis->GetLocationURL());
}
// Symbol: ?GetFullName@CHtmlView@@QEBA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@XZ
extern "C" void MS_ABI impl__GetFullName_CHtmlView__QEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__XZ(
    const CHtmlView* pThis, CString* pRet) {
    new (pRet) CString(pThis->GetFullName());
}
// Symbol: ?GetType@CHtmlView@@QEBA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@XZ
extern "C" void MS_ABI impl__GetType_CHtmlView__QEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__XZ(
    const CHtmlView* pThis, CString* pRet) {
    new (pRet) CString(pThis->GetType());
}
// Symbol: ?GetHtmlDocument@CHtmlView@@QEBAPEAUIDispatch@@XZ
extern "C" IDispatch* MS_ABI impl__GetHtmlDocument_CHtmlView__QEBAPEAUIDispatch__XZ(
    const CHtmlView* pThis) {
    return pThis->GetHtmlDocument();
}
// Symbol: ?GetApplication@CHtmlView@@QEBAPEAUIDispatch@@XZ
extern "C" IDispatch* MS_ABI impl__GetApplication_CHtmlView__QEBAPEAUIDispatch__XZ(
    const CHtmlView* pThis) {
    return pThis->GetApplication();
}
// Symbol: ?GetContainer@CHtmlView@@QEBAPEAUIDispatch@@XZ
extern "C" IDispatch* MS_ABI impl__GetContainer_CHtmlView__QEBAPEAUIDispatch__XZ(
    const CHtmlView* pThis) {
    return pThis->GetContainer();
}
// Symbol: ?GetParentBrowser@CHtmlView@@QEBAPEAUIDispatch@@XZ
extern "C" IDispatch* MS_ABI impl__GetParentBrowser_CHtmlView__QEBAPEAUIDispatch__XZ(
    const CHtmlView* pThis) {
    return pThis->GetParentBrowser();
}
// Symbol: ?GetLeft@CHtmlView@@QEBAJXZ
extern "C" long MS_ABI impl__GetLeft_CHtmlView__QEBAJXZ(const CHtmlView* pThis) {
    return pThis->GetLeft();
}
// Symbol: ?GetTop@CHtmlView@@QEBAJXZ
extern "C" long MS_ABI impl__GetTop_CHtmlView__QEBAJXZ(const CHtmlView* pThis) {
    return pThis->GetTop();
}
// Symbol: ?GetWidth@CHtmlView@@QEBAJXZ
extern "C" long MS_ABI impl__GetWidth_CHtmlView__QEBAJXZ(const CHtmlView* pThis) {
    return pThis->GetWidth();
}
// Symbol: ?GetHeight@CHtmlView@@QEBAJXZ
extern "C" long MS_ABI impl__GetHeight_CHtmlView__QEBAJXZ(const CHtmlView* pThis) {
    return pThis->GetHeight();
}
// Symbol: ?GetVisible@CHtmlView@@QEBAHXZ
extern "C" int MS_ABI impl__GetVisible_CHtmlView__QEBAHXZ(const CHtmlView* pThis) {
    return pThis->GetVisible();
}
// Symbol: ?GetFullScreen@CHtmlView@@QEBAHXZ
extern "C" int MS_ABI impl__GetFullScreen_CHtmlView__QEBAHXZ(const CHtmlView* pThis) {
    return pThis->GetFullScreen();
}
// Symbol: ?GetMenuBar@CHtmlView@@QEBAHXZ
extern "C" int MS_ABI impl__GetMenuBar_CHtmlView__QEBAHXZ(const CHtmlView* pThis) {
    return pThis->GetMenuBar();
}
// Symbol: ?GetAddressBar@CHtmlView@@QEBAHXZ
extern "C" int MS_ABI impl__GetAddressBar_CHtmlView__QEBAHXZ(const CHtmlView* pThis) {
    return pThis->GetAddressBar();
}
// Symbol: ?GetStatusBar@CHtmlView@@QEBAHXZ
extern "C" int MS_ABI impl__GetStatusBar_CHtmlView__QEBAHXZ(const CHtmlView* pThis) {
    return pThis->GetStatusBar();
}
// Symbol: ?GetToolBar@CHtmlView@@QEBAHXZ
extern "C" int MS_ABI impl__GetToolBar_CHtmlView__QEBAHXZ(const CHtmlView* pThis) {
    return pThis->GetToolBar();
}
// Symbol: ?GetOffline@CHtmlView@@QEBAHXZ
extern "C" int MS_ABI impl__GetOffline_CHtmlView__QEBAHXZ(const CHtmlView* pThis) {
    return pThis->GetOffline();
}
// Symbol: ?GetSilent@CHtmlView@@QEBAHXZ
extern "C" int MS_ABI impl__GetSilent_CHtmlView__QEBAHXZ(const CHtmlView* pThis) {
    return pThis->GetSilent();
}
// Symbol: ?GetTheaterMode@CHtmlView@@QEBAHXZ
extern "C" int MS_ABI impl__GetTheaterMode_CHtmlView__QEBAHXZ(const CHtmlView* pThis) {
    return pThis->GetTheaterMode();
}
// Symbol: ?GetRegisterAsBrowser@CHtmlView@@QEBAHXZ
extern "C" int MS_ABI impl__GetRegisterAsBrowser_CHtmlView__QEBAHXZ(const CHtmlView* pThis) {
    return pThis->GetRegisterAsBrowser();
}
// Symbol: ?GetRegisterAsDropTarget@CHtmlView@@QEBAHXZ
extern "C" int MS_ABI impl__GetRegisterAsDropTarget_CHtmlView__QEBAHXZ(const CHtmlView* pThis) {
    return pThis->GetRegisterAsDropTarget();
}
// Symbol: ?GetTopLevelContainer@CHtmlView@@QEBAHXZ
extern "C" int MS_ABI impl__GetTopLevelContainer_CHtmlView__QEBAHXZ(const CHtmlView* pThis) {
    return pThis->GetTopLevelContainer();
}
// Symbol: ?ExecWB@CHtmlView@@QEAAXW4OLECMDID@@W4OLECMDEXECOPT@@PEAUtagVARIANT@@2@Z
extern "C" void MS_ABI impl__ExecWB_CHtmlView__QEAAXW4OLECMDID__W4OLECMDEXECOPT__PEAUtagVARIANT__2_Z(
    CHtmlView* pThis, OLECMDID cmdID, OLECMDEXECOPT cmdexecopt,
    VARIANT* pvaIn, VARIANT* pvaOut) {
    pThis->ExecWB(cmdID, cmdexecopt, pvaIn, pvaOut);
}
// Symbol: ?LoadFromResource@CHtmlView@@QEAAHPEB_W@Z
extern "C" int MS_ABI impl__LoadFromResource_CHtmlView__QEAAHPEB_W_Z(
    CHtmlView* pThis, const wchar_t* lpszResource) {
    return pThis->LoadFromResource(lpszResource);
}
// Symbol: ?LoadFromResource@CHtmlView@@QEAAHI@Z
extern "C" int MS_ABI impl__LoadFromResource_CHtmlView__QEAAHI_Z(
    CHtmlView* pThis, unsigned int nRes) {
    return pThis->LoadFromResource(nRes);
}
// Symbol: ?Print@CHtmlView@@QEAAXXZ (not in mapping but keep for completeness)

// Symbol: ?GetProperty@CHtmlView@@QEAAHPEB_WAEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@@Z
extern "C" int MS_ABI impl__GetProperty_CHtmlView__QEAAHPEB_WAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
    CHtmlView* pThis, const wchar_t* lpszProperty, CString& strValue) {
    return pThis->GetProperty(lpszProperty, strValue);
}
// Symbol: ?PutProperty@CHtmlView@@QEAAXPEB_WAEBUtagVARIANT@@@Z
extern "C" void MS_ABI impl__PutProperty_CHtmlView__QEAAXPEB_WAEBUtagVARIANT___Z(
    CHtmlView* pThis, const wchar_t* lpszProperty, const VARIANT& vtValue) {
    pThis->PutProperty(lpszProperty, vtValue);
}
// Symbol: ?GetSource@CHtmlView@@UEAAHAEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@@Z
extern "C" int MS_ABI impl__GetSource_CHtmlView__UEAAHAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
    CHtmlView* pThis, CString& strRef) {
    return pThis->GetSource(strRef);
}
// Symbol: ?OnGetOptionKeyPath@CHtmlView@@UEAAJPEAPEA_WK@Z
extern "C" long MS_ABI impl__OnGetOptionKeyPath_CHtmlView__UEAAJPEAPEA_WK_Z(
    CHtmlView* pThis, wchar_t** ppszKey, unsigned long dwReserved) {
    return pThis->OnGetOptionKeyPath(ppszKey, dwReserved);
}
// CHtmlView event sink stubs
// Symbol: ?OnBeforeNavigate2@CHtmlView@@UEAAXPEB_WK0AEAVCByteArray@@0PEAH@Z
extern "C" void MS_ABI impl__OnBeforeNavigate2_CHtmlView__UEAAXPEB_WK0AEAVCByteArray__0PEAH_Z(
    CHtmlView* pThis, const wchar_t* lpszURL, unsigned long nFlags,
    const wchar_t* lpszTargetFrameName, CByteArray& baPostData,
    const wchar_t* lpszHeaders, BOOL* pbCancel) {
    pThis->OnBeforeNavigate2(lpszURL, nFlags, lpszTargetFrameName,
                             baPostData, lpszHeaders, pbCancel);
}
// Symbol: ?OnNavigateComplete2@CHtmlView@@UEAAXPEB_W@Z
extern "C" void MS_ABI impl__OnNavigateComplete2_CHtmlView__UEAAXPEB_W_Z(
    CHtmlView* pThis, const wchar_t* lpszURL) {
    pThis->OnNavigateComplete2(lpszURL);
}
// Symbol: ?OnDocumentComplete@CHtmlView@@UEAAXPEB_W@Z
extern "C" void MS_ABI impl__OnDocumentComplete_CHtmlView__UEAAXPEB_W_Z(
    CHtmlView* pThis, const wchar_t* lpszURL) {
    pThis->OnDocumentComplete(lpszURL);
}
// Symbol: ?OnProgressChange@CHtmlView@@UEAAXJJ@Z
extern "C" void MS_ABI impl__OnProgressChange_CHtmlView__UEAAXJJ_Z(
    CHtmlView* pThis, long lProgress, long lProgressMax) {
    pThis->OnProgressChange(lProgress, lProgressMax);
}
// Symbol: ?OnTitleChange@CHtmlView@@UEAAXPEB_W@Z
extern "C" void MS_ABI impl__OnTitleChange_CHtmlView__UEAAXPEB_W_Z(
    CHtmlView* pThis, const wchar_t* lpszText) {
    pThis->OnTitleChange(lpszText);
}
// Symbol: ?OnStatusTextChange@CHtmlView@@UEAAXPEB_W@Z
extern "C" void MS_ABI impl__OnStatusTextChange_CHtmlView__UEAAXPEB_W_Z(
    CHtmlView* pThis, const wchar_t* lpszText) {
    pThis->OnStatusTextChange(lpszText);
}
// Symbol: ?OnCommandStateChange@CHtmlView@@UEAAXJH@Z
extern "C" void MS_ABI impl__OnCommandStateChange_CHtmlView__UEAAXJH_Z(
    CHtmlView* pThis, long lCommand, int bEnable) {
    pThis->OnCommandStateChange(lCommand, bEnable);
}
// Symbol: ?OnDownloadBegin@CHtmlView@@UEAAXXZ
extern "C" void MS_ABI impl__OnDownloadBegin_CHtmlView__UEAAXXZ(CHtmlView* pThis) {
    pThis->OnDownloadBegin();
}
// Symbol: ?OnDownloadComplete@CHtmlView@@UEAAXXZ
extern "C" void MS_ABI impl__OnDownloadComplete_CHtmlView__UEAAXXZ(CHtmlView* pThis) {
    pThis->OnDownloadComplete();
}
// Symbol: ?OnFullScreen@CHtmlView@@UEAAXH@Z
extern "C" void MS_ABI impl__OnFullScreen_CHtmlView__UEAAXH_Z(CHtmlView* pThis, int bFullScreen) {
    pThis->OnFullScreen(bFullScreen);
}
// Symbol: ?OnMenuBar@CHtmlView@@UEAAXH@Z
extern "C" void MS_ABI impl__OnMenuBar_CHtmlView__UEAAXH_Z(CHtmlView* pThis, int bMenuBar) {
    pThis->OnMenuBar(bMenuBar);
}
// Symbol: ?OnNavigateError@CHtmlView@@UEAAXPEB_W0KPEAH@Z
extern "C" void MS_ABI impl__OnNavigateError_CHtmlView__UEAAXPEB_W0KPEAH_Z(
    CHtmlView* pThis, const wchar_t* lpszURL, const wchar_t* lpszFrame,
    unsigned long dwError, BOOL* pbCancel) {
    pThis->OnNavigateError(lpszURL, lpszFrame, dwError, pbCancel);
}
// Symbol: ?OnNewWindow2@CHtmlView@@UEAAXPEAPEAUIDispatch@@PEAH@Z
extern "C" void MS_ABI impl__OnNewWindow2_CHtmlView__UEAAXPEAPEAUIDispatch__PEAH_Z(
    CHtmlView* pThis, IDispatch** ppDisp, BOOL* bCancel) {
    pThis->OnNewWindow2(ppDisp, bCancel);
}
// Symbol: ?OnPropertyChange@CHtmlView@@UEAAXPEB_W@Z
extern "C" void MS_ABI impl__OnPropertyChange_CHtmlView__UEAAXPEB_W_Z(
    CHtmlView* pThis, const wchar_t* lpszProperty) {
    pThis->OnPropertyChange(lpszProperty);
}
// Symbol: ?OnQuit@CHtmlView@@UEAAXXZ
extern "C" void MS_ABI impl__OnQuit_CHtmlView__UEAAXXZ(CHtmlView* pThis) {
    pThis->OnQuit();
}
// Symbol: ?OnStatusBar@CHtmlView@@UEAAXH@Z
extern "C" void MS_ABI impl__OnStatusBar_CHtmlView__UEAAXH_Z(CHtmlView* pThis, int bStatusBar) {
    pThis->OnStatusBar(bStatusBar);
}
// Symbol: ?OnTheaterMode@CHtmlView@@UEAAXH@Z
extern "C" void MS_ABI impl__OnTheaterMode_CHtmlView__UEAAXH_Z(CHtmlView* pThis, int bTheaterMode) {
    pThis->OnTheaterMode(bTheaterMode);
}
// Symbol: ?OnToolBar@CHtmlView@@UEAAXH@Z
extern "C" void MS_ABI impl__OnToolBar_CHtmlView__UEAAXH_Z(CHtmlView* pThis, int bToolBar) {
    pThis->OnToolBar(bToolBar);
}
// Symbol: ?OnVisible@CHtmlView@@UEAAXH@Z
extern "C" void MS_ABI impl__OnVisible_CHtmlView__UEAAXH_Z(CHtmlView* pThis, int bVisible) {
    pThis->OnVisible(bVisible);
}
// Symbol: ?OnTranslateUrl@CHtmlView@@UEAAJKPEA_WPEAPEA_W@Z
extern "C" long MS_ABI impl__OnTranslateUrl_CHtmlView__UEAAJKPEA_WPEAPEA_W_Z(
    CHtmlView* pThis, unsigned long dwTranslate, wchar_t* pchURLIn, wchar_t** ppchURLOut) {
    return pThis->OnTranslateUrl(dwTranslate, pchURLIn, ppchURLOut);
}
// Retail OnUpdateEditCopy/Cut/Paste (mfc140u 0x27d5f0 / 0x27d650 / 0x27d6b0, the
// bodies that follow each OnEdit* thunk):
//     BOOL bEnabled = FALSE;
//     QueryFormsCommand(IDM_xxx, NULL, &bEnabled, NULL);   // call 0x27c0b0
//     pCmdUI->Enable(bEnabled);                             // CCmdUI vslot 0, tail-jumped
// Retail does not null-check pCmdUI; the guard here is a harmless deviation.
// Symbol: ?OnUpdateEditCopy@CHtmlView@@IEAAXPEAVCCmdUI@@@Z
extern "C" void MS_ABI impl__OnUpdateEditCopy_CHtmlView__IEAAXPEAVCCmdUI___Z(CHtmlView* pThis, CCmdUI* ui) {
    int bEnabled = FALSE;
    impl__QueryFormsCommand_CHtmlView__QEAAJKPEAH00_Z(pThis, IDM_COPY, nullptr, &bEnabled, nullptr);
    if (ui) impl__Enable_CCmdUI__UEAAXH_Z(ui, bEnabled);
}
// Symbol: ?OnUpdateEditCut@CHtmlView@@IEAAXPEAVCCmdUI@@@Z
extern "C" void MS_ABI impl__OnUpdateEditCut_CHtmlView__IEAAXPEAVCCmdUI___Z(CHtmlView* pThis, CCmdUI* ui) {
    int bEnabled = FALSE;
    impl__QueryFormsCommand_CHtmlView__QEAAJKPEAH00_Z(pThis, IDM_CUT, nullptr, &bEnabled, nullptr);
    if (ui) impl__Enable_CCmdUI__UEAAXH_Z(ui, bEnabled);
}
// Symbol: ?OnUpdateEditPaste@CHtmlView@@IEAAXPEAVCCmdUI@@@Z
extern "C" void MS_ABI impl__OnUpdateEditPaste_CHtmlView__IEAAXPEAVCCmdUI___Z(CHtmlView* pThis, CCmdUI* ui) {
    int bEnabled = FALSE;
    impl__QueryFormsCommand_CHtmlView__QEAAJKPEAH00_Z(pThis, IDM_PASTE, nullptr, &bEnabled, nullptr);
    if (ui) impl__Enable_CCmdUI__UEAAXH_Z(ui, bEnabled);
}
CHtmlView::CHtmlView()
    : m_pBrowser(nullptr), m_pControlWnd(nullptr), m_bCreated(FALSE) {
    memset(_htmlview_padding, 0, sizeof(_htmlview_padding));
}
CHtmlView::~CHtmlView() {
    if (m_pBrowser) {
        m_pBrowser->Stop();
        m_pBrowser->put_Visible(VARIANT_FALSE);
        m_pBrowser->Release();
        m_pBrowser = nullptr;
    }
}
BOOL CHtmlView::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName,
                       DWORD dwStyle, const RECT& rect, CWnd* pParentWnd,
                       UINT nID, CCreateContext* pContext) {
    if (!CView::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext))
        return FALSE;

    // Create WebBrowser control via COM
    HRESULT hr = CoCreateInstance(CLSID_WebBrowser, nullptr, CLSCTX_INPROC_SERVER,
                                   IID_IWebBrowser2, (void**)&m_pBrowser);
    if (FAILED(hr) || !m_pBrowser) return FALSE;

    // Set up client site for in-place activation
    CHtmlViewClientSite* pSite = new CHtmlViewClientSite(m_hWnd);
    pSite->SetBrowser(m_pBrowser);

    IOleObject* pOleObject = nullptr;
    hr = m_pBrowser->QueryInterface(IID_IOleObject, (void**)&pOleObject);
    if (SUCCEEDED(hr)) {
        pOleObject->SetClientSite(pSite);
        OleSetContainedObject(pOleObject, TRUE);

        RECT rcClient;
        ::GetClientRect(m_hWnd, &rcClient);
        pOleObject->DoVerb(OLEIVERB_INPLACEACTIVATE, nullptr, pSite, 0, m_hWnd, &rcClient);
        pOleObject->Release();
    }

    // Release site (browser holds a ref via AddRef)
    pSite->Release();

    m_pBrowser->put_Visible(VARIANT_TRUE);
    m_bCreated = TRUE;
    return TRUE;
}
void CHtmlView::OnDraw(void* /*pDC*/) {
    // WebBrowser renders itself
}
void CHtmlView::Navigate(const wchar_t* lpszURL, DWORD dwFlags,
                         const wchar_t* lpszTargetFrameName,
                         const wchar_t* lpszHeaders,
                         void* lpvPostData, DWORD dwPostDataLen) {
    if (!m_pBrowser) return;
    VARIANT vFlags, vTarget, vPostData, vHeaders;
    VariantInit(&vFlags); VariantInit(&vTarget); VariantInit(&vPostData); VariantInit(&vHeaders);

    vFlags.vt = VT_I4; vFlags.lVal = dwFlags;

    if (lpszTargetFrameName) { vTarget.vt = VT_BSTR; vTarget.bstrVal = SysAllocString(lpszTargetFrameName); }
    if (lpszHeaders) { vHeaders.vt = VT_BSTR; vHeaders.bstrVal = SysAllocString(lpszHeaders); }
    if (lpvPostData && dwPostDataLen > 0) {
        vPostData.vt = VT_ARRAY | VT_UI1;
        SAFEARRAYBOUND sab = { dwPostDataLen, 0 };
        SAFEARRAY* psa = SafeArrayCreate(VT_UI1, 1, &sab);
        if (psa) {
            void* pvData = nullptr;
            SafeArrayAccessData(psa, &pvData);
            if (pvData) memcpy(pvData, lpvPostData, dwPostDataLen);
            SafeArrayUnaccessData(psa);
            vPostData.parray = psa;
        }
    }

    BSTR bstrUrl = SysAllocString(lpszURL);
    m_pBrowser->Navigate(bstrUrl, &vFlags, &vTarget, &vPostData, &vHeaders);
    SysFreeString(bstrUrl);

    VariantClear(&vFlags); VariantClear(&vTarget); VariantClear(&vPostData); VariantClear(&vHeaders);
}
void CHtmlView::Navigate2(LPITEMIDLIST pIDL, DWORD dwFlags, const wchar_t* lpszTargetFrameName) {
    if (!m_pBrowser) return;
    VARIANT vFlags, vTarget, vEmpty;
    VariantInit(&vFlags); VariantInit(&vTarget); VariantInit(&vEmpty);
    vFlags.vt = VT_I4; vFlags.lVal = dwFlags;
    if (lpszTargetFrameName) { vTarget.vt = VT_BSTR; vTarget.bstrVal = SysAllocString(lpszTargetFrameName); }
    // For PIDL navigation: pass empty URL; PIDL needs CoTaskMemAlloc + ILSaveToStream
    (void)pIDL;
    VARIANT vUrl;
    vUrl.vt = VT_BSTR;
    vUrl.bstrVal = SysAllocString(L"");
    m_pBrowser->Navigate2(&vUrl, &vFlags, &vTarget, &vEmpty, &vEmpty);
    VariantClear(&vUrl);
    VariantClear(&vFlags); VariantClear(&vTarget); VariantClear(&vEmpty);
}
void CHtmlView::Navigate2(const wchar_t* lpszURL, DWORD dwFlags,
                          const wchar_t* lpszTargetFrameName,
                          const wchar_t* lpszHeaders,
                          void* lpvPostData, DWORD dwPostDataLen) {
    if (!m_pBrowser) return;
    VARIANT vFlags, vTarget, vPostData, vHeaders;
    VariantInit(&vFlags); VariantInit(&vTarget); VariantInit(&vPostData); VariantInit(&vHeaders);
    vFlags.vt = VT_I4; vFlags.lVal = dwFlags;
    if (lpszTargetFrameName) { vTarget.vt = VT_BSTR; vTarget.bstrVal = SysAllocString(lpszTargetFrameName); }
    if (lpszHeaders) { vHeaders.vt = VT_BSTR; vHeaders.bstrVal = SysAllocString(lpszHeaders); }
    VARIANT vUrl;
    vUrl.vt = VT_BSTR;
    vUrl.bstrVal = SysAllocString(lpszURL);
    m_pBrowser->Navigate2(&vUrl, &vFlags, &vTarget, &vPostData, &vHeaders);
    VariantClear(&vUrl);
    VariantClear(&vFlags); VariantClear(&vTarget); VariantClear(&vPostData); VariantClear(&vHeaders);
    (void)lpvPostData; (void)dwPostDataLen;
}
void CHtmlView::GoBack() { if (m_pBrowser) m_pBrowser->GoBack(); }
void CHtmlView::GoForward() { if (m_pBrowser) m_pBrowser->GoForward(); }
void CHtmlView::GoHome() { if (m_pBrowser) m_pBrowser->GoHome(); }
void CHtmlView::GoSearch() { if (m_pBrowser) m_pBrowser->GoSearch(); }
void CHtmlView::Stop() { if (m_pBrowser) m_pBrowser->Stop(); }
void CHtmlView::Refresh() { if (m_pBrowser) m_pBrowser->Refresh(); }
void CHtmlView::Refresh2(int nLevel) { if (m_pBrowser) { VARIANT v; v.vt = VT_I4; v.lVal = nLevel; m_pBrowser->Refresh2(&v); } }
BOOL CHtmlView::GetBusy() const {
    if (!m_pBrowser) return FALSE;
    VARIANT_BOOL b = VARIANT_FALSE;
    m_pBrowser->get_Busy(&b);
    return b != VARIANT_FALSE;
}
READYSTATE CHtmlView::GetReadyState() const {
    if (!m_pBrowser) return READYSTATE_UNINITIALIZED;
    READYSTATE rs = READYSTATE_UNINITIALIZED;
    m_pBrowser->get_ReadyState(&rs);
    return rs;
}
CString CHtmlView::GetLocationName() const {
    CString str;
    if (m_pBrowser) {
        BSTR bstr = nullptr;
        if (SUCCEEDED(m_pBrowser->get_LocationName(&bstr)) && bstr) {
            str = bstr;
            SysFreeString(bstr);
        }
    }
    return str;
}
CString CHtmlView::GetLocationURL() const {
    CString str;
    if (m_pBrowser) {
        BSTR bstr = nullptr;
        if (SUCCEEDED(m_pBrowser->get_LocationURL(&bstr)) && bstr) {
            str = bstr;
            SysFreeString(bstr);
        }
    }
    return str;
}
CString CHtmlView::GetFullName() const {
    CString str;
    if (m_pBrowser) {
        BSTR bstr = nullptr;
        if (SUCCEEDED(m_pBrowser->get_FullName(&bstr)) && bstr) {
            str = bstr;
            SysFreeString(bstr);
        }
    }
    return str;
}
CString CHtmlView::GetType() const {
    CString str;
    if (m_pBrowser) {
        BSTR bstr = nullptr;
        if (SUCCEEDED(m_pBrowser->get_Type(&bstr)) && bstr) {
            str = bstr;
            SysFreeString(bstr);
        }
    }
    return str;
}
LPDISPATCH CHtmlView::GetHtmlDocument() const {
    LPDISPATCH pDisp = nullptr;
    if (m_pBrowser) m_pBrowser->get_Document(&pDisp);
    return pDisp;
}
void CHtmlView::ExecWB(OLECMDID cmdID, OLECMDEXECOPT cmdexecopt,
                       VARIANT* pvaIn, VARIANT* pvaOut) {
    if (m_pBrowser) m_pBrowser->ExecWB(cmdID, cmdexecopt, pvaIn, pvaOut);
}
BOOL CHtmlView::LoadFromResource(const wchar_t* lpszResource) {
    if (lpszResource && m_pBrowser) {
        CString strUrl = L"res://";
        // Try to get module path for res:// protocol
        wchar_t buf[MAX_PATH];
        if (::GetModuleFileNameW(AfxGetInstanceHandle(), buf, MAX_PATH)) {
            strUrl += buf;
            strUrl += L"/";
            strUrl += lpszResource;
            Navigate(strUrl);
            return TRUE;
        }
    }
    return FALSE;
}
BOOL CHtmlView::LoadFromResource(UINT nRes) {
    wchar_t buf[32];
    swprintf(buf, 32, L"%u", nRes);
    return LoadFromResource(buf);
}
void CHtmlView::Print() {
    if (m_pBrowser) {
        VARIANT vIn, vOut;
        VariantInit(&vIn); VariantInit(&vOut);
        m_pBrowser->ExecWB(OLECMDID_PRINT, OLECMDEXECOPT_DONTPROMPTUSER, &vIn, &vOut);
    }
}
void CHtmlView::PrintPreview() {
    if (m_pBrowser) {
        VARIANT vIn, vOut;
        VariantInit(&vIn); VariantInit(&vOut);
        m_pBrowser->ExecWB(OLECMDID_PRINTPREVIEW, OLECMDEXECOPT_DONTPROMPTUSER, &vIn, &vOut);
    }
}
// ---------------------------------------------------------------------------
// Overridable notification defaults.  Every one below is a faithful
// transcription of the retail body, resolved through the mfc140u export
// address table (ures.py) and cross-checked against the CHtmlView vftable at
// 0x1803340e8.  Fifteen of them resolve to the ICF-folded `ret` at 0x27d0 (see
// OnDestroy below) -- ordinals: OnNavigateComplete2 10542, OnDocumentComplete
// 9174, OnProgressChange 10853, OnTitleChange 11366, OnCommandStateChange 8953,
// OnDownloadBegin 9191, OnDownloadComplete 9192, OnFullScreen 9973, OnMenuBar
// 10428, OnPropertyChange 10856, OnQuit 10869, OnStatusBar 11292, OnTheaterMode
// 11346, OnToolBar 11367, OnVisible 11583 -- so their bodies are empty.  The
// previous bodies here (SetWindowText on the view, put_FullScreen & co. echoed
// back into the browser, Stop() on OnQuit) were invented and are gone.
// ---------------------------------------------------------------------------
// OnBeforeNavigate2, RVA 0x27d710 (mfc140u, ord 8695, vftable slot 129):
//     mov 0x38(%rsp),%rax ; movl $0,(%rax) ; ret      -> *pbCancel = FALSE
// (pbCancel is the 7th argument, [rsp+0x38]; no NULL check in retail.)
void CHtmlView::OnBeforeNavigate2(const wchar_t*, DWORD, const wchar_t*, CByteArray&,
                                 const wchar_t*, BOOL* pbCancel) {
    if (pbCancel) *pbCancel = FALSE;
}
// OnNavigateComplete2 / OnDocumentComplete: ordinals 10542 / 9174 -> 0x27d0 `ret`.
void CHtmlView::OnNavigateComplete2(const wchar_t*) {}
void CHtmlView::OnDocumentComplete(const wchar_t*) {}
void CHtmlView::OnProgressChange(long, long) {}
void CHtmlView::OnTitleChange(const wchar_t*) {}
// OnStatusTextChange, RVA 0x27d720 (mfc140u, ord 11294, vftable slot 130):
//     CFrameWnd* pFrame = GetParentFrame();                   // call 0x28e200
//     if (pFrame != NULL)
//         ::SendMessage(pFrame->m_hWnd /*+0x40*/, WM_SETMESSAGESTRING /*0x362*/,
//                       0, (LPARAM)lpszText);                 // IAT 0x1802c7120 = USER32!SendMessageW
void CHtmlView::OnStatusTextChange(const wchar_t* lpszText) {
    CFrameWnd* pFrame = impl__GetParentFrame_CWnd__QEBAPEAVCFrameWnd__XZ(this);
    if (pFrame != nullptr) {
        ::SendMessage(pFrame->m_hWnd, 0x0362 /*WM_SETMESSAGESTRING*/, 0,
                      reinterpret_cast<LPARAM>(lpszText));
    }
}
void CHtmlView::OnCommandStateChange(long, BOOL) {}
void CHtmlView::OnDownloadBegin() {}
void CHtmlView::OnDownloadComplete() {}
void CHtmlView::OnFullScreen(BOOL) {}
void CHtmlView::OnMenuBar(BOOL) {}
// OnNavigateError, RVA 0x27d750 (mfc140u, ord 10543, vftable slot 146):
//     mov 0x28(%rsp),%rax ; movl $0,(%rax) ; ret      -> *pbCancel = FALSE
// (pbCancel is the 5th argument, [rsp+0x28]; no NULL check in retail.)
void CHtmlView::OnNavigateError(const wchar_t*, const wchar_t*, DWORD, BOOL* pbCancel) {
    if (pbCancel) *pbCancel = FALSE;
}
// OnNewWindow2, RVA 0x27d700 (mfc140u, ord 10673, vftable slot 137):
//     movl $0,(%r8) ; ret                             -> *pbCancel = FALSE
void CHtmlView::OnNewWindow2(LPDISPATCH*, BOOL* bCancel) {
    if (bCancel) *bCancel = FALSE;
}
void CHtmlView::OnPropertyChange(const wchar_t*) {}
void CHtmlView::OnQuit() {}
void CHtmlView::OnStatusBar(BOOL) {}
void CHtmlView::OnTheaterMode(BOOL) {}
void CHtmlView::OnToolBar(BOOL) {}
void CHtmlView::OnVisible(BOOL) {}
// OnTranslateUrl: ordinal 11423 -> 0x3a60 `mov $1,%eax ; ret` (mfc140u), i.e.
// S_FALSE with *ppchURLOut untouched (IDocHostUIHandler: "not translated").
// The previous body CoTaskMem-duplicated the URL and returned S_OK -- invented.
HRESULT CHtmlView::OnTranslateUrl(DWORD, wchar_t*, wchar_t**) {
    return S_FALSE;
}
BOOL CHtmlView::GetSource(CString& strRef) {
    strRef.Empty();
    if (!m_pBrowser) return FALSE;

    LPDISPATCH pDocDisp = nullptr;
    if (FAILED(m_pBrowser->get_Document(&pDocDisp)) || !pDocDisp) return FALSE;

    IHTMLDocument3* pDoc3 = nullptr;
    HRESULT hr = pDocDisp->QueryInterface(IID_IHTMLDocument3, (void**)&pDoc3);
    pDocDisp->Release();
    if (FAILED(hr) || !pDoc3) return FALSE;

    IHTMLElement* pRoot = nullptr;
    BSTR bstrSource = nullptr;
    hr = pDoc3->get_documentElement(&pRoot);
    pDoc3->Release();
    if (FAILED(hr) || !pRoot) return FALSE;

    hr = pRoot->get_outerHTML(&bstrSource);
    pRoot->Release();
    if (SUCCEEDED(hr) && bstrSource) {
        strRef = bstrSource;
        SysFreeString(bstrSource);
        return TRUE;
    }
    if (bstrSource) {
        SysFreeString(bstrSource);
    }
    return FALSE;
}
// OnGetOptionKeyPath: ordinal 10040 -> 0x3a60 `mov $1,%eax ; ret` (mfc140u),
// i.e. S_FALSE with *ppwszPathKey untouched, so MSHTML keeps its default
// registry key.  The previous body wrote "...\\Main\\FeatureControl" and
// returned S_OK, redirecting MSHTML's user-option lookup -- invented.
HRESULT CHtmlView::OnGetOptionKeyPath(wchar_t**, DWORD) {
    return S_FALSE;
}
LPDISPATCH CHtmlView::GetApplication() const {
    LPDISPATCH p = nullptr;
    if (m_pBrowser) m_pBrowser->get_Application(&p);
    return p;
}
LPDISPATCH CHtmlView::GetContainer() const {
    LPDISPATCH p = nullptr;
    if (m_pBrowser) m_pBrowser->get_Container(&p);
    return p;
}
LPDISPATCH CHtmlView::GetParentBrowser() const {
    LPDISPATCH p = nullptr;
    if (m_pBrowser) m_pBrowser->get_Parent(&p);
    return p;
}
long CHtmlView::GetLeft() const   { long v = 0; if (m_pBrowser) m_pBrowser->get_Left(&v);   return v; }
long CHtmlView::GetTop() const    { long v = 0; if (m_pBrowser) m_pBrowser->get_Top(&v);    return v; }
long CHtmlView::GetWidth() const  { long v = 0; if (m_pBrowser) m_pBrowser->get_Width(&v);  return v; }
long CHtmlView::GetHeight() const { long v = 0; if (m_pBrowser) m_pBrowser->get_Height(&v); return v; }
BOOL CHtmlView::GetVisible() const {
    VARIANT_BOOL b = VARIANT_FALSE;
    if (m_pBrowser) m_pBrowser->get_Visible(&b);
    return b != VARIANT_FALSE;
}
BOOL CHtmlView::GetFullScreen() const {
    VARIANT_BOOL b = VARIANT_FALSE;
    if (m_pBrowser) m_pBrowser->get_FullScreen(&b);
    return b != VARIANT_FALSE;
}
BOOL CHtmlView::GetMenuBar() const {
    VARIANT_BOOL b = VARIANT_FALSE;
    if (m_pBrowser) m_pBrowser->get_MenuBar(&b);
    return b != VARIANT_FALSE;
}
BOOL CHtmlView::GetAddressBar() const {
    VARIANT_BOOL b = VARIANT_FALSE;
    if (m_pBrowser) m_pBrowser->get_AddressBar(&b);
    return b != VARIANT_FALSE;
}
BOOL CHtmlView::GetStatusBar() const {
    VARIANT_BOOL b = VARIANT_FALSE;
    if (m_pBrowser) m_pBrowser->get_StatusBar(&b);
    return b != VARIANT_FALSE;
}
BOOL CHtmlView::GetToolBar() const {
    int v = 0;
    if (m_pBrowser) m_pBrowser->get_ToolBar(&v);
    return v != 0;
}
BOOL CHtmlView::GetOffline() const {
    VARIANT_BOOL b = VARIANT_FALSE;
    if (m_pBrowser) m_pBrowser->get_Offline(&b);
    return b != VARIANT_FALSE;
}
BOOL CHtmlView::GetSilent() const {
    VARIANT_BOOL b = VARIANT_FALSE;
    if (m_pBrowser) m_pBrowser->get_Silent(&b);
    return b != VARIANT_FALSE;
}
BOOL CHtmlView::GetTheaterMode() const {
    VARIANT_BOOL b = VARIANT_FALSE;
    if (m_pBrowser) m_pBrowser->get_TheaterMode(&b);
    return b != VARIANT_FALSE;
}
BOOL CHtmlView::GetRegisterAsBrowser() const {
    VARIANT_BOOL b = VARIANT_FALSE;
    if (m_pBrowser) m_pBrowser->get_RegisterAsBrowser(&b);
    return b != VARIANT_FALSE;
}
BOOL CHtmlView::GetRegisterAsDropTarget() const {
    VARIANT_BOOL b = VARIANT_FALSE;
    if (m_pBrowser) m_pBrowser->get_RegisterAsDropTarget(&b);
    return b != VARIANT_FALSE;
}
BOOL CHtmlView::GetTopLevelContainer() const {
    VARIANT_BOOL b = VARIANT_FALSE;
    if (m_pBrowser) m_pBrowser->get_TopLevelContainer(&b);
    return b != VARIANT_FALSE;
}
BOOL CHtmlView::GetProperty(const wchar_t* lpszProperty, CString& strValue) {
    if (!m_pBrowser || !lpszProperty) return FALSE;
    VARIANT v;
    VariantInit(&v);
    BSTR bstrProp = SysAllocString(lpszProperty);
    HRESULT hr = m_pBrowser->GetProperty(bstrProp, &v);
    SysFreeString(bstrProp);
    if (SUCCEEDED(hr) && v.vt == VT_BSTR && v.bstrVal) {
        strValue = v.bstrVal;
        VariantClear(&v);
        return TRUE;
    }
    VariantClear(&v);
    return FALSE;
}
void CHtmlView::PutProperty(const wchar_t* lpszProperty, const VARIANT& vtValue) {
    if (!m_pBrowser || !lpszProperty) return;
    BSTR bstrProp = SysAllocString(lpszProperty);
    m_pBrowser->PutProperty(bstrProp, const_cast<VARIANT&>(vtValue));
    SysFreeString(bstrProp);
}

// === Bodies transcribed from retail mfc140u (RVAs are function entries) ===

// CHtmlView::GetProperty(LPCTSTR) -> COleVariant, RVA 0x27cf70 (mfc140u).
// Registers: RCX = this, RDX = hidden COleVariant return slot, R8 = lpszProperty;
// the function returns the sret pointer in RAX.  Retail:
//     memset(&result, 0, sizeof(VARIANT));                       // COleVariant()
//     m_wndBrowser.InvokeHelper(0x12f, DISPATCH_METHOD, VT_VARIANT, &result,
//                               VTS_BSTR /*0x1803b29dc = "\x08"*/, lpszProperty);
//     return result;
// DISPID 0x12f (303) is IWebBrowserApp::GetProperty (exdisp.idl `[id(303)]`).
// DEVIATION: this repo's CWnd::InvokeHelper thunk is a stub and m_pControlWnd
// is never populated, so the same DISPID is reached through the IWebBrowser2
// vtable instead (GetProperty, slot 35) -- exactly what retail's own
// GetProperty(LPCTSTR, CString&) (entry 0x27cd00 mfc140u, ordinal 6126; the
// `mov 0x118(%rax)` is at 0x27cd66 inside it) does.  The value is moved into
// the return slot only on success; on failure it stays VT_EMPTY.  Retail's
// InvokeHelper path throws (COleDispatchException / COleException) on a failed
// Invoke; this body returns the VT_EMPTY variant instead -- second DEVIATION.
// Symbol: ?GetProperty@CHtmlView@@QEAA?AVCOleVariant@@PEB_W@Z
extern "C" void* MS_ABI impl__GetProperty_CHtmlView__QEAA_AVCOleVariant__PEB_W_Z(
    CHtmlView* pThis, COleVariant* pRet, const wchar_t* lpszProperty) {
    new (pRet) COleVariant();
    if (pThis && pThis->m_pBrowser && lpszProperty) {
        VARIANT v;
        VariantInit(&v);
        BSTR bstrProp = SysAllocString(lpszProperty);
        HRESULT hr = pThis->m_pBrowser->GetProperty(bstrProp, &v);
        SysFreeString(bstrProp);
        if (SUCCEEDED(hr)) {
            *static_cast<VARIANT*>(pRet) = v;   // ownership moves into the return slot
        } else {
            VariantClear(&v);
        }
    }
    return pRet;
}

// CHtmlView::Navigate2(LPCTSTR, DWORD, CByteArray&, LPCTSTR, LPCTSTR), RVA 0x27cb50 (mfc140u).
// Args: RCX this, RDX lpszURL, R8D dwFlags, R9 baPostedData, [rbp+0x77] lpszTargetFrameName,
// [rbp+0x7f] lpszHeader.  Retail:
//     COleVariant vaPostedData;  vaPostedData = baPostedData;        // ??4COleVariant@@...AEBVCByteArray (0x26e0e0, ord 1527)
//     COleVariant vaURL(lpszURL, VT_BSTR);                            // ??0COleVariant@@QEAA@PEB_WG@Z (0x26d640, ord 873)
//     COleVariant vaHeaders(lpszHeader, VT_BSTR);
//     COleVariant vaTargetFrameName(lpszTargetFrameName, VT_BSTR);
//     COleVariant vaFlags((long)dwFlags, VT_I4);                      // vt=3, lVal=dwFlags built in place
//     m_pBrowserApp->Navigate2(&vaURL, &vaFlags, &vaTargetFrameName, &vaPostedData, &vaHeaders);  // vslot 0x1a0/8 = 52
//     (five VariantClear calls -- OLEAUT32 #9 via IAT 0x1802c69e8 -- for the destructors)
// The retail COleVariant(LPCWSTR, VT_BSTR) ctor (0x26d640: `movw $8,(%rcx);
// movq $0,8(%rcx); test %rdx; je`) leaves bstrVal NULL for a NULL string, so a
// NULL lpszTargetFrameName / lpszHeader reaches the browser as {VT_BSTR, NULL};
// reproduced below.  Retail does NOT null-check m_pBrowserApp.  DEVIATION: the
// null guard, matching every other IWebBrowser2 wrapper in this file.  The
// CByteArray -> VARIANT conversion goes through the same exported
// COleVariant::operator= thunk retail calls (VT_ARRAY|VT_UI1 SAFEARRAY).
// Symbol: ?Navigate2@CHtmlView@@QEAAXPEB_WKAEAVCByteArray@@00@Z
extern "C" void MS_ABI impl__Navigate2_CHtmlView__QEAAXPEB_WKAEAVCByteArray__00_Z(
    CHtmlView* pThis, const wchar_t* lpszURL, unsigned long dwFlags, CByteArray& baPostedData,
    const wchar_t* lpszTargetFrameName, const wchar_t* lpszHeader) {
    if (!pThis || !pThis->m_pBrowser) return;

    VARIANT vaPostedData, vaURL, vaHeaders, vaTargetFrameName, vaFlags;
    VariantInit(&vaPostedData); VariantInit(&vaURL); VariantInit(&vaHeaders);
    VariantInit(&vaTargetFrameName); VariantInit(&vaFlags);

    impl___4COleVariant__QEAAAEBV0_AEBVCByteArray___Z(static_cast<COleVariant*>(&vaPostedData), &baPostedData);
    vaURL.vt = VT_BSTR;             vaURL.bstrVal = lpszURL ? SysAllocString(lpszURL) : nullptr;
    vaHeaders.vt = VT_BSTR;         vaHeaders.bstrVal = lpszHeader ? SysAllocString(lpszHeader) : nullptr;
    vaTargetFrameName.vt = VT_BSTR; vaTargetFrameName.bstrVal = lpszTargetFrameName ? SysAllocString(lpszTargetFrameName) : nullptr;
    vaFlags.vt = VT_I4;             vaFlags.lVal = static_cast<long>(dwFlags);

    pThis->m_pBrowser->Navigate2(&vaURL, &vaFlags, &vaTargetFrameName, &vaPostedData, &vaHeaders);

    VariantClear(&vaFlags); VariantClear(&vaTargetFrameName); VariantClear(&vaHeaders);
    VariantClear(&vaURL); VariantClear(&vaPostedData);
}

// CHtmlView::BeforeNavigate2 (DWebBrowserEvents2 sink), RVA 0x27d270 (mfc140u).
// Args: RCX this, RDX pDisp (unused), R8 URL, R9 Flags, [rbp+0x67] TargetFrameName,
// [rbp+0x6f] PostData, [rbp+0x77] Headers, [rbp+0x7f] Cancel (VARIANT_BOOL*).  Retail:
//     VARIANT* vtPostedData = V_VARIANTREF(PostData);                 // 0x8(PostData)
//     CByteArray array;                                               // vftable 0x180321100
//     if (V_VT(vtPostedData) & VT_ARRAY) {                            // bt $0xd
//         vtPostedData->vt |= VT_UI1;
//         COleSafeArray safe(*vtPostedData);                          // 0x26fcc0
//         DWORD dwSize = safe.GetOneDimSize();                        // 0x26feb0
//         LPVOID pv; safe.AccessData(&pv);                            // SafeArrayAccessData (OLEAUT32 #23) + AfxCheckError
//         array.SetSize(dwSize);                                      // 0x1d3850
//         memcpy_s(array.GetData(), dwSize, pv, dwSize);              // memcpy; NULL/size checks -> _invalid_parameter
//         safe.UnaccessData();                                        // OLEAUT32 #24 + AfxCheckError
//     }
//     CString strTargetFrameName(V_BSTR(TargetFrameName)), strURL(V_BSTR(URL)), strHeaders(V_BSTR(Headers));
//     BOOL bCancel = FALSE;
//     OnBeforeNavigate2(strURL, V_I4(Flags), strTargetFrameName, array, strHeaders, &bCancel);  // vslot 0x408/8 = 129
//     *Cancel = bCancel ? VARIANT_TRUE : VARIANT_FALSE;               // unconditional store
// DEVIATIONS: the SAFEARRAY is read in place (no COleSafeArray copy and no
// `vt |= VT_UI1` write into the caller's VARIANT); the CByteArray is filled
// through its exported SetSize/SetAtGrow thunks because this repo's CByteArray
// keeps its storage in a side table (detail/FilecoreSupport.h) with no GetData
// export; Cancel is null-checked.  Retail reads PostData->pvarVal (+8) and
// every V_BSTR (+8) unconditionally -- here the BYREF indirection is taken only
// when VT_BYREF is set and a BSTR is read only when V_VT == VT_BSTR (otherwise
// the empty string), and NULL VARIANT pointers return early with *Cancel =
// VARIANT_FALSE where retail would fault.  The virtual OnBeforeNavigate2 is
// reached through the C++ vtable, which is the repo's equivalent of retail's
// slot-129 call.
// Symbol: ?BeforeNavigate2@CHtmlView@@MEAAXPEAUIDispatch@@PEAUtagVARIANT@@1111PEAF@Z
extern "C" void MS_ABI impl__BeforeNavigate2_CHtmlView__MEAAXPEAUIDispatch__PEAUtagVARIANT__1111PEAF_Z(
    CHtmlView* pThis, IDispatch* /*pDisp*/, VARIANT* URL, VARIANT* Flags, VARIANT* TargetFrameName,
    VARIANT* PostData, VARIANT* Headers, short* Cancel) {
    if (!pThis || !URL || !Flags || !TargetFrameName || !PostData || !Headers) {
        if (Cancel) *Cancel = VARIANT_FALSE;
        return;
    }
    alignas(alignof(CByteArray)) unsigned char arrayStorage[sizeof(CByteArray)];
    CByteArray* pArray = static_cast<CByteArray*>(
        impl___0CByteArray__QEAA_XZ(reinterpret_cast<CByteArray*>(arrayStorage)));

    VARIANT* vtPostedData = (PostData->vt & VT_BYREF) ? PostData->pvarVal : PostData;
    if (vtPostedData && (vtPostedData->vt & VT_ARRAY)) {
        SAFEARRAY* psa = (vtPostedData->vt & VT_BYREF)
            ? (vtPostedData->pparray ? *vtPostedData->pparray : nullptr)
            : vtPostedData->parray;
        if (psa) {
            long lLBound = 0, lUBound = -1;
            SafeArrayGetLBound(psa, 1, &lLBound);
            SafeArrayGetUBound(psa, 1, &lUBound);
            const long long dwSize = (lUBound >= lLBound) ? (long long)(lUBound - lLBound + 1) : 0;
            void* pv = nullptr;
            HRESULT hr = SafeArrayAccessData(psa, &pv);
            impl__AfxCheckError__YAXJ_Z(hr);
            impl__SetSize_CByteArray__QEAAX_J0_Z(pArray, dwSize, -1);
            if (pv) {
                for (long long i = 0; i < dwSize; ++i)
                    impl__SetAtGrow_CByteArray__QEAAX_JE_Z(pArray, i, static_cast<const BYTE*>(pv)[i]);
            }
            hr = SafeArrayUnaccessData(psa);
            impl__AfxCheckError__YAXJ_Z(hr);
        }
    }

    CString strTargetFrameName = StringFromBstr(V_VT(TargetFrameName) == VT_BSTR ? V_BSTR(TargetFrameName) : nullptr);
    CString strURL             = StringFromBstr(V_VT(URL) == VT_BSTR ? V_BSTR(URL) : nullptr);
    CString strHeaders         = StringFromBstr(V_VT(Headers) == VT_BSTR ? V_BSTR(Headers) : nullptr);

    BOOL bCancel = FALSE;
    pThis->OnBeforeNavigate2(strURL, static_cast<DWORD>(V_I4(Flags)), strTargetFrameName,
                             *pArray, strHeaders, &bCancel);
    if (Cancel) *Cancel = bCancel ? VARIANT_TRUE : VARIANT_FALSE;

    impl___1CByteArray__UEAA_XZ(pArray);
}

// CHtmlView::CreateControlSite, RVA 0x27b150 (mfc140u):
//     *ppSite = new CHtmlControlSite(pContainer);   // operator new(0x188) at 0x27f0, ctor 0x27ad80
//     return TRUE;                                   // nID / clsid unused
// Retail skips the ctor and stores NULL when operator new returns NULL.  In
// this repo the CHtmlControlSite ctor thunk placement-constructs a
// COleControlSite, so sizeof(COleControlSite) is the allocation size here.
// Symbol: ?CreateControlSite@CHtmlView@@UEAAHPEAVCOleControlContainer@@PEAPEAVCOleControlSite@@IAEBU_GUID@@@Z
extern "C" int MS_ABI impl__CreateControlSite_CHtmlView__UEAAHPEAVCOleControlContainer__PEAPEAVCOleControlSite__IAEBU_GUID___Z(
    CHtmlView* /*pThis*/, COleControlContainer* pContainer, COleControlSite** ppSite,
    unsigned int /*nID*/, const GUID& /*clsid*/) {
    if (!ppSite) return FALSE;   // DEVIATION: retail stores through ppSite unconditionally
    void* pMem = ::operator new(sizeof(COleControlSite));
    if (pMem) {
        pMem = impl___0CHtmlControlSite__QEAA_PEAVCOleControlContainer___Z(pMem, pContainer);
    }
    *ppSite = static_cast<COleControlSite*>(pMem);
    return TRUE;
}

// CHtmlView::DocumentComplete (sink), RVA 0x27d4a0 (mfc140u):
//     CString str(V_BSTR(URL));            // 0x8(URL) -> CStringT ctor 0x18000dcb0
//     OnDocumentComplete(str);             // vslot 0x450/8 = 138
// DEVIATION (also NavigateComplete2 / NavigateError below): retail reads +8
// unconditionally; here the BSTR is taken only when V_VT == VT_BSTR, and NULL
// pointers return early.
// Symbol: ?DocumentComplete@CHtmlView@@MEAAXPEAUIDispatch@@PEAUtagVARIANT@@@Z
extern "C" void MS_ABI impl__DocumentComplete_CHtmlView__MEAAXPEAUIDispatch__PEAUtagVARIANT___Z(
    CHtmlView* pThis, IDispatch* /*pDisp*/, VARIANT* URL) {
    if (!pThis || !URL) return;
    CString str = StringFromBstr(V_VT(URL) == VT_BSTR ? V_BSTR(URL) : nullptr);
    pThis->OnDocumentComplete(str);
}

// CHtmlView::ExecFormsCommand, RVA 0x27bf50 (mfc140u):
//     HRESULT hr = E_FAIL;
//     ENSURE(m_pBrowserApp != NULL);                                   // NULL -> AfxThrowInvalidArgException (0x227720)
//     CComPtr<IDispatch> spDisp;  m_pBrowserApp->get_Document(&spDisp); // vslot 0x90/8 = 18
//     CComQIPtr<IHTMLDocument2> spDoc(spDisp);                          // QI 0x18034f0a0
//     if (spDoc) {
//         CComQIPtr<IOleCommandTarget> spTarget(spDoc);                 // QI 0x18034f080
//         if (spTarget)
//             hr = spTarget->Exec(&CGID_MSHTML /*0x1802d9fd0*/, dwCommandID,
//                                 OLECMDEXECOPT_DONTPROMPTUSER /*2*/, pVarIn, pVarOut);  // vslot 4
//     }
//     return hr;   (all three interfaces released)
// Symbol: ?ExecFormsCommand@CHtmlView@@QEAAJKPEAUtagVARIANT@@0@Z
extern "C" long MS_ABI impl__ExecFormsCommand_CHtmlView__QEAAJKPEAUtagVARIANT__0_Z(
    CHtmlView* pThis, unsigned long dwCommandID, VARIANT* pVarIn, VARIANT* pVarOut) {
    HRESULT hr = E_FAIL;
    if (!pThis || !pThis->m_pBrowser) {
        impl__AfxThrowInvalidArgException__YAXXZ();
        return hr;
    }
    IDispatch* pDisp = nullptr;
    pThis->m_pBrowser->get_Document(&pDisp);
    if (pDisp) {
        IUnknown* pDoc = nullptr;
        if (FAILED(pDisp->QueryInterface(kIID_IHTMLDocument2, reinterpret_cast<void**>(&pDoc)))) pDoc = nullptr;
        if (pDoc) {
            IOleCommandTarget* pTarget = nullptr;
            if (FAILED(pDoc->QueryInterface(kIID_IOleCommandTarget, reinterpret_cast<void**>(&pTarget)))) pTarget = nullptr;
            if (pTarget) {
                hr = pTarget->Exec(&kCGID_MSHTML, dwCommandID, OLECMDEXECOPT_DONTPROMPTUSER, pVarIn, pVarOut);
                pTarget->Release();
            }
            pDoc->Release();
        }
        pDisp->Release();
    }
    return hr;
}

// CHtmlView::NavigateComplete2 (sink), RVA 0x27d210 (mfc140u):
//     CString str(V_BSTR(URL));  OnNavigateComplete2(str);   // vslot 0x400/8 = 128
// Symbol: ?NavigateComplete2@CHtmlView@@MEAAXPEAUIDispatch@@PEAUtagVARIANT@@@Z
extern "C" void MS_ABI impl__NavigateComplete2_CHtmlView__MEAAXPEAUIDispatch__PEAUtagVARIANT___Z(
    CHtmlView* pThis, IDispatch* /*pDisp*/, VARIANT* URL) {
    if (!pThis || !URL) return;
    CString str = StringFromBstr(V_VT(URL) == VT_BSTR ? V_BSTR(URL) : nullptr);
    pThis->OnNavigateComplete2(str);
}

// CHtmlView::NavigateError (sink), RVA 0x27d500 (mfc140u).
// Args: RCX this, RDX pDisp (unused), R8 pvURL, R9 pvFrame, [rsp+0x60] pvStatusCode,
// [rsp+0x68] pvbCancel.  Retail:
//     CString strURL(V_BSTR(pvURL)), strFrame(V_BSTR(pvFrame));
//     BOOL bCancel = FALSE;
//     OnNavigateError(strURL, strFrame, V_I4(pvStatusCode), &bCancel);   // vslot 0x490/8 = 146
//     if (pvbCancel) *pvbCancel = bCancel ? VARIANT_TRUE : VARIANT_FALSE; // neg/sbb
// Symbol: ?NavigateError@CHtmlView@@MEAAXPEAUIDispatch@@PEAUtagVARIANT@@11PEAF@Z
extern "C" void MS_ABI impl__NavigateError_CHtmlView__MEAAXPEAUIDispatch__PEAUtagVARIANT__11PEAF_Z(
    CHtmlView* pThis, IDispatch* /*pDisp*/, VARIANT* pvURL, VARIANT* pvFrame, VARIANT* pvStatusCode, short* pvbCancel) {
    if (!pThis || !pvURL || !pvFrame || !pvStatusCode) return;
    CString strURL   = StringFromBstr(V_VT(pvURL) == VT_BSTR ? V_BSTR(pvURL) : nullptr);
    CString strFrame = StringFromBstr(V_VT(pvFrame) == VT_BSTR ? V_BSTR(pvFrame) : nullptr);
    BOOL bCancel = FALSE;
    pThis->OnNavigateError(strURL, strFrame, static_cast<DWORD>(V_I4(pvStatusCode)), &bCancel);
    if (pvbCancel) *pvbCancel = bCancel ? VARIANT_TRUE : VARIANT_FALSE;
}

// CHtmlView::OnDestroy: the mfc140u export table resolves ordinal 9116 to
// 0x27d0, the ICF-folded `ret` shared by every empty function in the image
// (OnDocumentComplete, ordinal 9174, and OnNavigateComplete2 resolve there too).
// Corroborated from the other direction: CHtmlView's message map (entries at
// 0x180334b60, reached from GetMessageMap at 0x27aa70) lists WM_DESTROY (0x0002,
// sig 19) with pfn 0x27d0, and the CHtmlView vftable at 0x1803340e8 holds 0x27d0
// in slot 128 (OnNavigateComplete2) and slot 138 (OnDocumentComplete).  The
// retail body is therefore empty -- it does NOT release m_pBrowserApp (that
// happens in the destructor) and does NOT chain to CFormView::OnDestroy.  An
// empty body is the complete transcription; there is nothing else to write.
// Symbol: ?OnDestroy@CHtmlView@@IEAAXXZ
extern "C" void MS_ABI impl__OnDestroy_CHtmlView__IEAAXXZ(CHtmlView* /*pThis*/) {}

// The IDocHostUIHandler defaults.  The export table resolves each to one of two
// ICF-folded stubs in mfc140u: 0x71e0 `xor %eax,%eax ; ret` (S_OK) or
// 0x3a60 `mov $1,%eax ; ret` (S_FALSE).  None reads an argument.
// Ordinals read from the mfc140u export address table (ures.py): S_OK ->
// OnDocWindowActivate 9179, OnEnableModeless 9731, OnFrameWindowActivate 9967,
// OnGetHostInfo 10015, OnHideUI 10095, OnResizeBorder 10933, OnUpdateUI 11570;
// S_FALSE -> OnFilterDataObject 9940, OnGetDropTarget 10003, OnGetExternal
// 10007, OnShowContextMenu 11171, OnShowUI 11213, OnTranslateAccelerator 11422.
// Corroborated by the CHtmlView vftable at 0x1803340e8 (mfc140u): slots 115 and
// 117-122 (seven, matching the seven S_OK exports) hold 0x71e0, and slots
// 113-114, 116 and 123-127 hold 0x3a60.  A bare `return S_OK` / `return S_FALSE`
// IS the complete retail body for each of these; nothing further can be
// transcribed without inventing behaviour.
// Symbol: ?OnDocWindowActivate@CHtmlView@@UEAAJH@Z
extern "C" long MS_ABI impl__OnDocWindowActivate_CHtmlView__UEAAJH_Z(CHtmlView*, int /*fActivate*/) {
    return S_OK;       // 0x71e0
}

// CHtmlView::OnDrawIconicThumbnailOrLivePreview, RVA 0x27ad30 (mfc140u).
// Args: RCX this, RDX CDC& dc, R8 CRect (16-byte struct -> pointer to copy),
// R9 CSize (8-byte struct in register), stack bIsThumbnail / BOOL& -- unused.
//     HDC hdc = (&dc == NULL) ? NULL : dc.m_hDC;      // inlined GetSafeHdc (+8)
//     ::OleDraw(GetHtmlDocument() /*call 0x27bdb0*/, DVASPECT_CONTENT /*1*/, hdc, rect);  // ole32!OleDraw (IAT 0x1802c7ad0)
// GetHtmlDocument returns an AddRef'd IDispatch (get_Document) which retail
// never releases.  DEVIATION: released here after the synchronous OleDraw.
// Symbol: ?OnDrawIconicThumbnailOrLivePreview@CHtmlView@@UEAAXAEAVCDC@@VCRect@@VCSize@@HAEAH@Z
extern "C" void MS_ABI impl__OnDrawIconicThumbnailOrLivePreview_CHtmlView__UEAAXAEAVCDC__VCRect__VCSize__HAEAH_Z(
    CHtmlView* pThis, CDC* pDC, const RECT* rect, long long /*szRequiredThumbnailSize*/,
    int /*bIsThumbnail*/, int* /*bAlphaChannelSet*/) {
    if (!pThis) return;
    HDC hdc = pDC ? static_cast<HDC>(pDC->m_hDC) : nullptr;
    IDispatch* pDoc = pThis->GetHtmlDocument();
    ::OleDraw(pDoc, DVASPECT_CONTENT, hdc, rect);
    if (pDoc) pDoc->Release();
}

// CHtmlView::OnEditCopy / OnEditCut / OnEditPaste, RVAs 0x27d5e0 / 0x27d640 /
// 0x27d6a0 (mfc140u): each tail-jumps to ExecFormsCommand(0x27bf50) with
// EDX = IDM_COPY (0xf) / IDM_CUT (0x10) / IDM_PASTE (0x1a) and R8 = R9 = NULL.
// Symbol: ?OnEditCopy@CHtmlView@@IEAAXXZ
extern "C" void MS_ABI impl__OnEditCopy_CHtmlView__IEAAXXZ(CHtmlView* pThis) {
    impl__ExecFormsCommand_CHtmlView__QEAAJKPEAUtagVARIANT__0_Z(pThis, IDM_COPY, nullptr, nullptr);
}
// Symbol: ?OnEditCut@CHtmlView@@IEAAXXZ
extern "C" void MS_ABI impl__OnEditCut_CHtmlView__IEAAXXZ(CHtmlView* pThis) {
    impl__ExecFormsCommand_CHtmlView__QEAAJKPEAUtagVARIANT__0_Z(pThis, IDM_CUT, nullptr, nullptr);
}
// Symbol: ?OnEditPaste@CHtmlView@@IEAAXXZ
extern "C" void MS_ABI impl__OnEditPaste_CHtmlView__IEAAXXZ(CHtmlView* pThis) {
    impl__ExecFormsCommand_CHtmlView__QEAAJKPEAUtagVARIANT__0_Z(pThis, IDM_PASTE, nullptr, nullptr);
}

// Symbol: ?OnEnableModeless@CHtmlView@@UEAAJH@Z
extern "C" long MS_ABI impl__OnEnableModeless_CHtmlView__UEAAJH_Z(CHtmlView*, int /*fEnable*/) {
    return S_OK;       // 0x71e0
}

// CHtmlView::OnFilePrint, RVA 0x27ab80 (mfc140u):
//     if (m_pBrowserApp == NULL) return;                                // plain test, no throw
//     CComPtr<IDispatch> spDisp;  m_pBrowserApp->get_Document(&spDisp); // vslot 18
//     if (spDisp) {
//         CComQIPtr<IOleCommandTarget> spTarget(spDisp);                // QI 0x18034f080
//         if (spTarget)
//             spTarget->Exec(NULL, OLECMDID_PRINT /*6*/, OLECMDEXECOPT_DODEFAULT /*0*/, NULL, NULL);  // vslot 4
//     }
// Symbol: ?OnFilePrint@CHtmlView@@QEAAXXZ
extern "C" void MS_ABI impl__OnFilePrint_CHtmlView__QEAAXXZ(CHtmlView* pThis) {
    if (!pThis || !pThis->m_pBrowser) return;
    IDispatch* pDisp = nullptr;
    pThis->m_pBrowser->get_Document(&pDisp);
    if (pDisp) {
        IOleCommandTarget* pTarget = nullptr;
        if (FAILED(pDisp->QueryInterface(kIID_IOleCommandTarget, reinterpret_cast<void**>(&pTarget)))) pTarget = nullptr;
        if (pTarget) {
            pTarget->Exec(nullptr, OLECMDID_PRINT, OLECMDEXECOPT_DODEFAULT, nullptr, nullptr);
            pTarget->Release();
        }
        pDisp->Release();
    }
}

// Symbol: ?OnFilterDataObject@CHtmlView@@UEAAJPEAUIDataObject@@PEAPEAU2@@Z
extern "C" long MS_ABI impl__OnFilterDataObject_CHtmlView__UEAAJPEAUIDataObject__PEAPEAU2__Z(
    CHtmlView*, IDataObject* /*pDataObject*/, IDataObject** /*ppDataObject*/) {
    return S_FALSE;    // 0x3a60
}

// Symbol: ?OnFrameWindowActivate@CHtmlView@@UEAAJH@Z
extern "C" long MS_ABI impl__OnFrameWindowActivate_CHtmlView__UEAAJH_Z(CHtmlView*, int /*fActivate*/) {
    return S_OK;       // 0x71e0
}

// Symbol: ?OnGetDropTarget@CHtmlView@@UEAAJPEAUIDropTarget@@PEAPEAU2@@Z
extern "C" long MS_ABI impl__OnGetDropTarget_CHtmlView__UEAAJPEAUIDropTarget__PEAPEAU2__Z(
    CHtmlView*, IDropTarget* /*pDropTarget*/, IDropTarget** /*ppDropTarget*/) {
    return S_FALSE;    // 0x3a60
}

// Symbol: ?OnGetExternal@CHtmlView@@UEAAJPEAPEAUIDispatch@@@Z
extern "C" long MS_ABI impl__OnGetExternal_CHtmlView__UEAAJPEAPEAUIDispatch___Z(CHtmlView*, IDispatch** /*lppDispatch*/) {
    return S_FALSE;    // 0x3a60
}

// Symbol: ?OnGetHostInfo@CHtmlView@@UEAAJPEAU_DOCHOSTUIINFO@@@Z
extern "C" long MS_ABI impl__OnGetHostInfo_CHtmlView__UEAAJPEAU_DOCHOSTUIINFO___Z(CHtmlView*, DOCHOSTUIINFO* /*pInfo*/) {
    return S_OK;       // 0x71e0 -- pInfo is not written
}

// Symbol: ?OnHideUI@CHtmlView@@UEAAJXZ
extern "C" long MS_ABI impl__OnHideUI_CHtmlView__UEAAJXZ(CHtmlView*) {
    return S_OK;       // 0x71e0
}

// CHtmlView::OnPaint: ordinal 10732 resolves to 0xda30 (mfc140u), a one-
// instruction `jmp 0x28ac80` = CWnd::Default().  The view paints nothing itself.
// Symbol: ?OnPaint@CHtmlView@@IEAAXXZ
extern "C" void MS_ABI impl__OnPaint_CHtmlView__IEAAXXZ(CHtmlView* pThis) {
    if (pThis) impl__Default_CWnd__IEAA_JXZ(pThis);
}

// Symbol: ?OnResizeBorder@CHtmlView@@UEAAJPEBUtagRECT@@PEAUIOleInPlaceUIWindow@@H@Z
extern "C" long MS_ABI impl__OnResizeBorder_CHtmlView__UEAAJPEBUtagRECT__PEAUIOleInPlaceUIWindow__H_Z(
    CHtmlView*, const RECT* /*prcBorder*/, IOleInPlaceUIWindow* /*pUIWindow*/, int /*fFrameWindow*/) {
    return S_OK;       // 0x71e0
}

// Symbol: ?OnShowContextMenu@CHtmlView@@UEAAJKPEAUtagPOINT@@PEAUIUnknown@@PEAUIDispatch@@@Z
extern "C" long MS_ABI impl__OnShowContextMenu_CHtmlView__UEAAJKPEAUtagPOINT__PEAUIUnknown__PEAUIDispatch___Z(
    CHtmlView*, unsigned long /*dwID*/, POINT* /*ppt*/, IUnknown* /*pcmdtReserved*/, IDispatch* /*pdispReserved*/) {
    return S_FALSE;    // 0x3a60 -- let MSHTML show its own menu
}

// Symbol: ?OnShowUI@CHtmlView@@UEAAJKPEAUIOleInPlaceActiveObject@@PEAUIOleCommandTarget@@PEAUIOleInPlaceFrame@@PEAUIOleInPlaceUIWindow@@@Z
extern "C" long MS_ABI impl__OnShowUI_CHtmlView__UEAAJKPEAUIOleInPlaceActiveObject__PEAUIOleCommandTarget__PEAUIOleInPlaceFrame__PEAUIOleInPlaceUIWindow___Z(
    CHtmlView*, unsigned long /*dwID*/, IOleInPlaceActiveObject* /*pActiveObject*/,
    IOleCommandTarget* /*pCommandTarget*/, IOleInPlaceFrame* /*pFrame*/, IOleInPlaceUIWindow* /*pDoc*/) {
    return S_FALSE;    // 0x3a60
}

// CHtmlView::OnSize, RVA 0x27ac70 (mfc140u):
//     CScrollView::OnSize(nType, cx, cy);                                  // call 0x2888f0 (CScrollView msgmap WM_SIZE pfn: Default(),
//                                                                          //   then SetScaleToFitSize(m_totalLog) if m_nMapMode == MM_SCALETOFIT
//                                                                          //   (cmpl $-1,0x100) else UpdateBars())
//     if (::IsWindow(m_wndBrowser.m_hWnd)) {                               // USER32!IsWindow, +0x160+0x40
//         CRect rect;  ::GetClientRect(m_hWnd, &rect);                     // USER32!GetClientRect
//         ::AdjustWindowRectEx(&rect, m_wndBrowser.GetStyle() /*0x2a9690*/, FALSE, WS_EX_CLIENTEDGE /*0x200*/);
//         m_wndBrowser.SetWindowPos(NULL, rect.left, rect.top, rect.Width(), rect.Height(),
//                                   SWP_NOACTIVATE | SWP_NOZORDER /*0x14*/);   // call 0x2a9a60 = CWnd::SetWindowPos
//     }
// DEVIATIONS: (1) this header derives CHtmlView from CView, not CFormView, so
// the CScrollView::OnSize thunk must not be applied to it (its scroll members
// would alias m_pBrowser/m_pControlWnd); only the CWnd::OnSize part -- Default()
// -- is kept.  (2) m_pControlWnd stands in for m_wndBrowser; when it is NULL
// (this file's Create never sets it) the browser's in-place object is resized
// through IOleInPlaceObject::SetObjectRects instead, which is what the control
// site path of CWnd::SetWindowPos ends up doing for an in-place control.
// Symbol: ?OnSize@CHtmlView@@IEAAXIHH@Z
extern "C" void MS_ABI impl__OnSize_CHtmlView__IEAAXIHH_Z(CHtmlView* pThis, unsigned int /*nType*/, int /*cx*/, int /*cy*/) {
    if (!pThis) return;
    impl__Default_CWnd__IEAA_JXZ(pThis);

    CWnd* pBrowserWnd = pThis->m_pControlWnd;
    if (pBrowserWnd && ::IsWindow(pBrowserWnd->m_hWnd)) {
        RECT rect = { 0, 0, 0, 0 };
        ::GetClientRect(pThis->m_hWnd, &rect);
        ::AdjustWindowRectEx(&rect, impl__GetStyle_CWnd__QEBAKXZ(pBrowserWnd), FALSE, WS_EX_CLIENTEDGE);
        impl__SetWindowPos_CWnd__QEAAHPEBV1_HHHHI_Z(pBrowserWnd, nullptr, rect.left, rect.top,
                                                    rect.right - rect.left, rect.bottom - rect.top,
                                                    SWP_NOACTIVATE | SWP_NOZORDER);
        return;
    }
    if (pThis->m_pBrowser && pThis->m_hWnd) {
        IOleInPlaceObject* pInPlace = nullptr;
        if (SUCCEEDED(pThis->m_pBrowser->QueryInterface(IID_IOleInPlaceObject, reinterpret_cast<void**>(&pInPlace))) && pInPlace) {
            RECT rect = { 0, 0, 0, 0 };
            ::GetClientRect(pThis->m_hWnd, &rect);
            pInPlace->SetObjectRects(&rect, &rect);
            pInPlace->Release();
        }
    }
}

// Symbol: ?OnTranslateAccelerator@CHtmlView@@UEAAJPEAUtagMSG@@PEBU_GUID@@K@Z
extern "C" long MS_ABI impl__OnTranslateAccelerator_CHtmlView__UEAAJPEAUtagMSG__PEBU_GUID__K_Z(
    CHtmlView*, MSG* /*lpMsg*/, const GUID* /*pguidCmdGroup*/, unsigned long /*nCmdID*/) {
    return S_FALSE;    // 0x3a60 -- not handled here
}

// Symbol: ?OnUpdateUI@CHtmlView@@UEAAJXZ
extern "C" long MS_ABI impl__OnUpdateUI_CHtmlView__UEAAJXZ(CHtmlView*) {
    return S_OK;       // 0x71e0
}

// CHtmlView::PreCreateWindow, RVA 0x27ab70 (mfc140u):
//     cs.style |= WS_CLIPCHILDREN;            // btsl $0x19, 0x30(%rdx)
//     return CView::PreCreateWindow(cs);      // tail jmp 0x277610 (CView::PreCreateWindow, ord 11805)
// (the NULL guard on cs is a deviation; retail dereferences it directly)
// Symbol: ?PreCreateWindow@CHtmlView@@UEAAHAEAUtagCREATESTRUCTW@@@Z
extern "C" int MS_ABI impl__PreCreateWindow_CHtmlView__UEAAHAEAUtagCREATESTRUCTW___Z(CHtmlView* pThis, CREATESTRUCTW* cs) {
    if (!cs) return FALSE;
    cs->style |= WS_CLIPCHILDREN;
    return impl__PreCreateWindow_CView__MEAAHAEAUtagCREATESTRUCTW___Z(pThis, cs);
}

// CHtmlView::PreTranslateMessage, RVA 0x27b310 (mfc140u):
//     if (CWnd::PreTranslateMessage(pMsg)) return TRUE;                // call 0x28bc00 (ord 11902): tooltip-filter hook only, returns 0
//     CFrameWnd* pFrame = GetTopLevelFrame();                          // call 0x28e490
//     if (pFrame != NULL && pFrame->m_bHelpMode) return FALSE;         // +0x104
//     for (pFrame = GetParentFrame(); pFrame; pFrame = pFrame->GetParentFrame())   // 0x28e200
//         if (pFrame->PreTranslateMessage(pMsg)) return TRUE;          // CWnd vslot 0x228/8 = 69
//     BOOL bRet = FALSE;
//     if (m_pBrowserApp != NULL) {
//         CComQIPtr<IOleInPlaceActiveObject> spObj(m_pBrowserApp);     // QI 0x18034f090
//         if (spObj) bRet = (spObj->TranslateAccelerator(pMsg) == S_OK);   // vslot 0x28/8 = 5, `sete`
//     }
//     return bRet;
// DEVIATIONS (same as CControlBar::PreTranslateMessage in this repo): the
// CWnd::PreTranslateMessage call is omitted because retail's is only the
// module-state tooltip filter hook (always 0) while this repo's thunk is an
// IsDialogMessage wrapper that would swallow the browser's keystrokes; the
// Shift+F1 help-mode early-out is omitted because CFrameWnd::m_bHelpMode is not
// modelled.  The frame loop is devirtualised through the CFrameWnd/CWnd thunks.
// Symbol: ?PreTranslateMessage@CHtmlView@@UEAAHPEAUtagMSG@@@Z
extern "C" int MS_ABI impl__PreTranslateMessage_CHtmlView__UEAAHPEAUtagMSG___Z(CHtmlView* pThis, MSG* pMsg) {
    if (!pThis || !pMsg) return FALSE;

    for (CFrameWnd* pFrame = impl__GetParentFrame_CWnd__QEBAPEAVCFrameWnd__XZ(pThis);
         pFrame != nullptr;
         pFrame = impl__GetParentFrame_CWnd__QEBAPEAVCFrameWnd__XZ(pFrame)) {
        int bHandled = impl__IsFrameWnd_CWnd__UEBAHXZ(pFrame)
            ? impl__PreTranslateMessage_CFrameWnd__UEAAHPEAUtagMSG___Z(pFrame, pMsg)
            : impl__PreTranslateMessage_CWnd__UEAAHPEAUtagMSG___Z(pFrame, pMsg);
        if (bHandled) return TRUE;
    }

    BOOL bRet = FALSE;
    if (pThis->m_pBrowser) {
        IOleInPlaceActiveObject* pObj = nullptr;
        if (FAILED(pThis->m_pBrowser->QueryInterface(kIID_IOleInPlaceActiveObject, reinterpret_cast<void**>(&pObj)))) pObj = nullptr;
        if (pObj) {
            bRet = (pObj->TranslateAccelerator(pMsg) == S_OK);
            pObj->Release();
        }
    }
    return bRet;
}

// CHtmlView::QueryFormsCommand, RVA 0x27c0b0 (mfc140u).
// Args: RCX this, EDX dwCommandID, R8 pbSupported, R9 pbEnabled, [rbp+0x40] pbChecked.
//     HRESULT hr = E_FAIL;
//     ENSURE(m_pBrowserApp != NULL);                                   // NULL -> AfxThrowInvalidArgException
//     CComPtr<IDispatch> spDisp;  m_pBrowserApp->get_Document(&spDisp); // vslot 18
//     CComQIPtr<IHTMLDocument2> spDoc(spDisp);                          // QI 0x18034f0a0
//     if (spDoc) {
//         CComQIPtr<IOleCommandTarget> spTarget(spDoc);                 // QI 0x18034f080
//         if (spTarget) {
//             OLECMD cmd = { dwCommandID, 0 };
//             hr = spTarget->QueryStatus(&CGID_MSHTML, 1, &cmd, NULL);  // vslot 3
//             if (SUCCEEDED(hr)) {
//                 if (pbSupported) *pbSupported =  cmd.cmdf       & 1;  // OLECMDF_SUPPORTED
//                 if (pbEnabled)   *pbEnabled   = (cmd.cmdf >> 1) & 1;  // OLECMDF_ENABLED
//                 if (pbChecked)   *pbChecked   = (cmd.cmdf >> 2) & 1;  // OLECMDF_LATCHED
//             }
//         }
//     }
//     return hr;
// Symbol: ?QueryFormsCommand@CHtmlView@@QEAAJKPEAH00@Z
extern "C" long MS_ABI impl__QueryFormsCommand_CHtmlView__QEAAJKPEAH00_Z(
    CHtmlView* pThis, unsigned long dwCommandID, int* pbSupported, int* pbEnabled, int* pbChecked) {
    HRESULT hr = E_FAIL;
    if (!pThis || !pThis->m_pBrowser) {
        impl__AfxThrowInvalidArgException__YAXXZ();
        return hr;
    }
    IDispatch* pDisp = nullptr;
    pThis->m_pBrowser->get_Document(&pDisp);
    if (pDisp) {
        IUnknown* pDoc = nullptr;
        if (FAILED(pDisp->QueryInterface(kIID_IHTMLDocument2, reinterpret_cast<void**>(&pDoc)))) pDoc = nullptr;
        if (pDoc) {
            IOleCommandTarget* pTarget = nullptr;
            if (FAILED(pDoc->QueryInterface(kIID_IOleCommandTarget, reinterpret_cast<void**>(&pTarget)))) pTarget = nullptr;
            if (pTarget) {
                OLECMD cmd;
                cmd.cmdID = dwCommandID;
                cmd.cmdf = 0;
                hr = pTarget->QueryStatus(&kCGID_MSHTML, 1, &cmd, nullptr);
                if (SUCCEEDED(hr)) {
                    if (pbSupported) *pbSupported = (cmd.cmdf & OLECMDF_SUPPORTED) ? 1 : 0;
                    if (pbEnabled)   *pbEnabled   = (cmd.cmdf & OLECMDF_ENABLED)   ? 1 : 0;
                    if (pbChecked)   *pbChecked   = (cmd.cmdf & OLECMDF_LATCHED)   ? 1 : 0;
                }
                pTarget->Release();
            }
            pDoc->Release();
        }
        pDisp->Release();
    }
    return hr;
}

// CHtmlView::QueryStatusWB, RVA 0x27bee0 (mfc140u):
//     OLECMDF cmdf;
//     if (FAILED(m_pBrowserApp->QueryStatusWB(cmdID, &cmdf)))   // vslot 0x1a8/8 = 53, no NULL check
//         AfxThrowUserException();                              // call 0x2a4310
//     return cmdf;                                              // EAX
// DEVIATION: a NULL m_pBrowser takes the same throw path instead of faulting.
// Symbol: ?QueryStatusWB@CHtmlView@@QEBA?AW4OLECMDF@@W4OLECMDID@@@Z
extern "C" OLECMDF MS_ABI impl__QueryStatusWB_CHtmlView__QEBA_AW4OLECMDF__W4OLECMDID___Z(const CHtmlView* pThis, OLECMDID cmdID) {
    OLECMDF cmdf = static_cast<OLECMDF>(0);
    HRESULT hr = (pThis && pThis->m_pBrowser) ? pThis->m_pBrowser->QueryStatusWB(cmdID, &cmdf) : E_POINTER;
    if (FAILED(hr)) {
        impl__AfxThrowUserException__YAXXZ();
    }
    return cmdf;
}

// ---------------------------------------------------------------------------
// DWebBrowserEvents2 sink map.  Reproduced from mfc140u .rdata at 0x1803345a0
// (the 0x18 bytes immediately after the CHtmlView vftable):
//   { 0x1801de860 (CCmdTarget::GetThisEventSinkMap, ord 7191, ICF-folded with
//     CCmdTarget::GetEventSinkMap ord 5339 -- CFormView declares no sink map of
//     its own), 0x1803345c0 (entries), 0x1803b29e0 (count, holds 0xffffffff) }
// Nineteen entries in retail order, each { L"" (0x18033d19c, the ON_EVENT
// `_T("")` name -- NOT NULL), dispid, params, VT_BOOL, pfn, NULL, 0,
// afxDispCustom, AFX_IDW_PANE_FIRST (0xe900), (UINT)-1 } followed by the
// END_EVENTSINK_MAP terminator { NULL, DISPID_UNKNOWN, NULL, VT_VOID, NULL,
// NULL, (size_t)-1, afxDispCustom, (UINT)-1, 0 } -- nCtrlIDLast is 0 there,
// not -1 (afxdisp.h END_EVENTSINK_MAP).  Retail's pfn slots hold vcall thunks
// (0x27e7e0..0x27e900, `mov (%rcx),%rax; mov 0xNNN(%rax),%rax; jmp *`) that
// dispatch through the CHtmlView vftable; here they hold this file's impl__
// thunks, which dispatch through the C++ vtable and have the same MS-ABI
// (this, args...) shape.  Nothing in this repo consumes the pfn yet:
// CCmdTarget::CallMemberFunc and CCmdTarget::OnEvent are stubs
// (core/runtime/CCmdTarget.cpp), so the map is currently reachable only as
// data via GetEventSinkMap / GetEventSinkEntry.
// ---------------------------------------------------------------------------
namespace {
const wchar_t kEvName_[] = L"";   // retail 0x18033d19c
const EvSinkMapEntry_ g_evsinkEntries_CHtmlView[] = {
    { { kEvName_, 102 /*DISPID_STATUSTEXTCHANGE*/,  "\x08",                         VT_BOOL, (const void*)&impl__OnStatusTextChange_CHtmlView__UEAAXPEB_W_Z,   nullptr, 0, evDispCustom_ }, 0xe900u, 0xffffffffu },
    { { kEvName_, 108 /*DISPID_PROGRESSCHANGE*/,    "\x03\x03",                     VT_BOOL, (const void*)&impl__OnProgressChange_CHtmlView__UEAAXJJ_Z,        nullptr, 0, evDispCustom_ }, 0xe900u, 0xffffffffu },
    { { kEvName_, 105 /*DISPID_COMMANDSTATECHANGE*/,"\x03\x0b",                     VT_BOOL, (const void*)&impl__OnCommandStateChange_CHtmlView__UEAAXJH_Z,    nullptr, 0, evDispCustom_ }, 0xe900u, 0xffffffffu },
    { { kEvName_, 106 /*DISPID_DOWNLOADBEGIN*/,     nullptr,                        VT_BOOL, (const void*)&impl__OnDownloadBegin_CHtmlView__UEAAXXZ,            nullptr, 0, evDispCustom_ }, 0xe900u, 0xffffffffu },
    { { kEvName_, 104 /*DISPID_DOWNLOADCOMPLETE*/,  nullptr,                        VT_BOOL, (const void*)&impl__OnDownloadComplete_CHtmlView__UEAAXXZ,         nullptr, 0, evDispCustom_ }, 0xe900u, 0xffffffffu },
    { { kEvName_, 113 /*DISPID_TITLECHANGE*/,       "\x08",                         VT_BOOL, (const void*)&impl__OnTitleChange_CHtmlView__UEAAXPEB_W_Z,         nullptr, 0, evDispCustom_ }, 0xe900u, 0xffffffffu },
    { { kEvName_, 252 /*DISPID_NAVIGATECOMPLETE2*/, "\x09\x4c",                     VT_BOOL, (const void*)&impl__NavigateComplete2_CHtmlView__MEAAXPEAUIDispatch__PEAUtagVARIANT___Z, nullptr, 0, evDispCustom_ }, 0xe900u, 0xffffffffu },
    { { kEvName_, 250 /*DISPID_BEFORENAVIGATE2*/,   "\x09\x4c\x4c\x4c\x4c\x4c\x4b", VT_BOOL, (const void*)&impl__BeforeNavigate2_CHtmlView__MEAAXPEAUIDispatch__PEAUtagVARIANT__1111PEAF_Z, nullptr, 0, evDispCustom_ }, 0xe900u, 0xffffffffu },
    { { kEvName_, 112 /*DISPID_PROPERTYCHANGE*/,    "\x08",                         VT_BOOL, (const void*)&impl__OnPropertyChange_CHtmlView__UEAAXPEB_W_Z,      nullptr, 0, evDispCustom_ }, 0xe900u, 0xffffffffu },
    { { kEvName_, 251 /*DISPID_NEWWINDOW2*/,        "\x49\x4b",                     VT_BOOL, (const void*)&impl__OnNewWindow2_CHtmlView__UEAAXPEAPEAUIDispatch__PEAH_Z, nullptr, 0, evDispCustom_ }, 0xe900u, 0xffffffffu },
    { { kEvName_, 259 /*DISPID_DOCUMENTCOMPLETE*/,  "\x09\x4c",                     VT_BOOL, (const void*)&impl__DocumentComplete_CHtmlView__MEAAXPEAUIDispatch__PEAUtagVARIANT___Z, nullptr, 0, evDispCustom_ }, 0xe900u, 0xffffffffu },
    { { kEvName_, 271 /*DISPID_NAVIGATEERROR*/,     "\x09\x4c\x4c\x4c\x4b",         VT_BOOL, (const void*)&impl__NavigateError_CHtmlView__MEAAXPEAUIDispatch__PEAUtagVARIANT__11PEAF_Z, nullptr, 0, evDispCustom_ }, 0xe900u, 0xffffffffu },
    { { kEvName_, 253 /*DISPID_ONQUIT*/,            nullptr,                        VT_BOOL, (const void*)&impl__OnQuit_CHtmlView__UEAAXXZ,                     nullptr, 0, evDispCustom_ }, 0xe900u, 0xffffffffu },
    { { kEvName_, 254 /*DISPID_ONVISIBLE*/,         "\x0b",                         VT_BOOL, (const void*)&impl__OnVisible_CHtmlView__UEAAXH_Z,                 nullptr, 0, evDispCustom_ }, 0xe900u, 0xffffffffu },
    { { kEvName_, 255 /*DISPID_ONTOOLBAR*/,         "\x0b",                         VT_BOOL, (const void*)&impl__OnToolBar_CHtmlView__UEAAXH_Z,                 nullptr, 0, evDispCustom_ }, 0xe900u, 0xffffffffu },
    { { kEvName_, 256 /*DISPID_ONMENUBAR*/,         "\x0b",                         VT_BOOL, (const void*)&impl__OnMenuBar_CHtmlView__UEAAXH_Z,                 nullptr, 0, evDispCustom_ }, 0xe900u, 0xffffffffu },
    { { kEvName_, 257 /*DISPID_ONSTATUSBAR*/,       "\x0b",                         VT_BOOL, (const void*)&impl__OnStatusBar_CHtmlView__UEAAXH_Z,               nullptr, 0, evDispCustom_ }, 0xe900u, 0xffffffffu },
    { { kEvName_, 258 /*DISPID_ONFULLSCREEN*/,      "\x0b",                         VT_BOOL, (const void*)&impl__OnFullScreen_CHtmlView__UEAAXH_Z,              nullptr, 0, evDispCustom_ }, 0xe900u, 0xffffffffu },
    { { kEvName_, 260 /*DISPID_ONTHEATERMODE*/,     "\x0b",                         VT_BOOL, (const void*)&impl__OnTheaterMode_CHtmlView__UEAAXH_Z,             nullptr, 0, evDispCustom_ }, 0xe900u, 0xffffffffu },
    { { nullptr,  -1 /*DISPID_UNKNOWN*/,           nullptr,                        VT_VOID, nullptr, nullptr, (size_t)-1, evDispCustom_ }, 0xffffffffu, 0u },
};
unsigned int g_evsinkEntryCount_CHtmlView = 0xffffffffu;
const EvSinkMap_ g_eventSinkMap_CHtmlView = {
    &impl__GetThisEventSinkMap_CCmdTarget__KAPEBUAFX_EVENTSINKMAP__XZ,
    g_evsinkEntries_CHtmlView,
    &g_evsinkEntryCount_CHtmlView
};
} // namespace

// CHtmlView::GetEventSinkMap, RVA 0x27aa80 (mfc140u): `lea 0x1803345a0,%rax ; ret`.
// Symbol: ?GetEventSinkMap@CHtmlView@@MEBAPEBUAFX_EVENTSINKMAP@@XZ
extern "C" const void* MS_ABI impl__GetEventSinkMap_CHtmlView__MEBAPEBUAFX_EVENTSINKMAP__XZ(const CHtmlView* /*pThis*/) {
    return &g_eventSinkMap_CHtmlView;
}

// CHtmlView::GetThisEventSinkMap: ordinal 7194 resolves to the same 0x27aa80
// (ICF-folded with GetEventSinkMap).
// Symbol: ?GetThisEventSinkMap@CHtmlView@@KAPEBUAFX_EVENTSINKMAP@@XZ
extern "C" const void* MS_ABI impl__GetThisEventSinkMap_CHtmlView__KAPEBUAFX_EVENTSINKMAP__XZ() {
    return &g_eventSinkMap_CHtmlView;
}
