// CGlobalUtils — OpenMFC implementation.
// Sources: global_cglobalutils_conv.cpp, global_other-22_impl.cpp, mfccore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/CPreviewViewSupport.h"
#include "detail/MfccoreSupport.h"

// OpenMFC: CGlobalUtils — currency/decimal string conversions (oleaut32-backed).
//
// CGlobalUtils is an MFC feature-pack utility class; these two methods are
// stateless numeric parsers that wrap the OLE Automation Variant conversion
// APIs (fully available under Wine). The docking/pane geometry helpers
// (SetNewParent, ForceAdjustLayout, CheckAlignment, GetPaneAndAlignFromPoint,
// CalcExpectedDockedRect) at the end of the file are transcribed from the
// retail disassembly; each carries its own RVA citation and deviation list.
//
// pThis (the CGlobalUtils*) is accepted to match the member-function ABI but is
// unused — both conversions are pure functions of their inputs, and none of the
// docking helpers reads a member of `this` in retail either.

#include <windows.h>
#include <oleauto.h>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

#include <cstddef>
#include <cstring>

// ---------------------------------------------------------------------------
// Thunks and statics used by the docking helpers at the end of this file
// (SetNewParent, ForceAdjustLayout, CheckAlignment, GetPaneAndAlignFromPoint,
// CalcExpectedDockedRect).  BRIEFING S1: inside this DLL the C++ methods do not
// exist as link symbols, only these extern "C" thunks.  Each definition was
// located with grep; the file is named beside it.
// ---------------------------------------------------------------------------
// core/window/Thunks.cpp, core/window/CWnd.cpp, core/runtime/CObject.cpp
extern "C" unsigned long MS_ABI impl__GetStyle_CWnd__QEBAKXZ(const CWnd* pThis);
extern "C" int   MS_ABI impl__ShowWindow_CWnd__QEAAHH_Z(CWnd* pThis, int nCmdShow);
extern "C" CWnd* MS_ABI impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(HWND hWnd);
extern "C" void  MS_ABI impl__ScreenToClient_CWnd__QEBAXPEAUtagRECT___Z(const CWnd* pThis, RECT* pRect);
extern "C" int   MS_ABI impl__SetWindowPos_CWnd__QEAAHPEBV1_HHHHI_Z(
                     CWnd* pThis, const CWnd* pWndInsertAfter, int x, int y, int cx, int cy, unsigned int nFlags);
extern "C" int   MS_ABI impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(const CObject* pThis, const CRuntimeClass* pClass);
// detail/MfcExceptionsSupport.cpp
extern "C" void  MS_ABI impl__AfxThrowInvalidArgException__YAXXZ();
// core/collections/CObList.cpp -- FindIndex(0) yields the head node (see ObNode).
extern "C" CObList::POSITION MS_ABI impl__FindIndex_CObList__QEBAPEAU__POSITION____J_Z(const CObList* pThis, long long nIndex);
// featurepack/docking/RuntimeClasses.cpp (real descriptor getters)
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CPaneDivider__SAPEAUCRuntimeClass__XZ();
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CPaneFrameWnd__SAPEAUCRuntimeClass__XZ();
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMultiPaneFrameWnd__SAPEAUCRuntimeClass__XZ();
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CPane__SAPEAUCRuntimeClass__XZ();
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CBasePane__SAPEAUCRuntimeClass__XZ();
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CDockablePane__SAPEAUCRuntimeClass__XZ();
// featurepack/docking/Thunks.cpp:1208 (forwards to the C++ virtual)
extern "C" void* MS_ABI impl__SetWindowPos_CBasePane__UEAAPEAXPEBVCWnd__HHHHIPEAX_Z(
                     CBasePane* pThis, const CWnd* pWndInsertAfter, int x, int y, int cx, int cy, unsigned int nFlags, void* hdwp);
// featurepack/docking/CBasePane.cpp -- real transcribed bodies
extern "C" unsigned long MS_ABI impl__GetCurrentAlignment_CBasePane__UEBAKXZ(const CBasePane* pThis);
extern "C" void* MS_ABI impl__GetParentMiniFrame_CBasePane__UEBAPEAVCPaneFrameWnd__H_Z(const CBasePane* pThis, int bNoAssert);
// featurepack/docking/CDockablePane.cpp -- real transcribed bodies
extern "C" int MS_ABI impl__GetCaptionHeight_CDockablePane__UEBAHXZ(const CDockablePane* pThis);
extern "C" int MS_ABI impl__IsDocked_CDockablePane__UEBAHXZ(const CDockablePane* pThis);
extern "C" int MS_ABI impl__IsInFloatingMultiPaneFrameWnd_CDockablePane__UEBAHXZ(const CDockablePane* pThis);
// featurepack/docking/CPaneFrameWnd.cpp / CPaneContainerManager.cpp
extern "C" void* MS_ABI impl__GetFirstVisiblePane_CPaneFrameWnd__UEBAPEAVCWnd__XZ(void* pThis);
extern "C" void* MS_ABI impl__GetFirstVisiblePane_CPaneContainerManager__UEBAPEAVCWnd__XZ(void* pThis);
// featurepack/docking/CPaneContainerManager.cpp:1719 -- real body; its definition
// takes the by-value CPoint as `void*` (one 8-byte register), matched here.
extern "C" void* MS_ABI impl__PaneFromPoint_CPaneContainerManager__UEAAPEAVCDockablePane__VCPoint__HHAEAH1_Z(
                     void* pThis, void* point, int nSensitivity, int bExactBar, int* pbIsTabArea, int* pbCaption);
// featurepack/docking/CPaneContainerManager.cpp:892 -- STILL a generated
// placeholder `(void* p0, void** p1) { return 0; }` with no `this`.  Declared
// here with the list the mangled name describes (this, CPoint by value,
// CDockablePane**).  The placeholder reads no argument, so the call is ABI-safe;
// it always answers FALSE (see GetPaneAndAlignFromPoint below).
extern "C" int MS_ABI impl__CheckForMiniFrameAndCaption_CPaneContainerManager__UEAAHVCPoint__PEAPEAVCDockablePane___Z(
                     void* pThis, long long point, void** ppTargetControlBar);
// featurepack/docking/CPaneContainerManager.cpp:729 -- real body.
extern "C" void MS_ABI impl__CalcRects_CPaneContainerManager__QEAAXAEAVCRect__00AEAKKVCSize__2_Z(
                     void* pThis, RECT* pRectOriginal, RECT* pRectInserted, RECT* pRectSlider,
                     unsigned long* pdwSliderStyle, unsigned long dwAlignment,
                     unsigned long long sizeMinOriginal, unsigned long long sizeMinInserted);
// featurepack/docking/StaticData.cpp -- the exported static data members.
extern "C" int impl__m_bHandleMinSize_CPane__2HA;                      // retail .data 0x3b6fd0 (mfc140)
// NB: retail initialises m_nDockSensitivity to 15 (the int stored at .data
// 0x3aaacc, mfc140); StaticData.cpp currently initialises it to 0.
extern "C" int impl__m_nDockSensitivity_CDockingManager__2HA;          // retail .data 0x3aaacc (mfc140)
extern "C" int impl__m_bIgnoreEnabledAlignment_CDockingManager__2HA;   // retail .data 0x3b6f5c (mfc140)
// featurepack/CMFC_misc_stubs.cpp:3665 -- ?afxGlobalUtils@@3VCGlobalUtils@@A
// storage (retail .data 0x3aacf8, mfc140).
extern "C" unsigned char impl__afxGlobalUtils__3VCGlobalUtils__A[];

namespace {

// CBRS_ALIGN_* spelled locally (afxres.h values), as other docking files do.
constexpr unsigned long kAlignLeft   = 0x1000UL;
constexpr unsigned long kAlignTop    = 0x2000UL;
constexpr unsigned long kAlignRight  = 0x4000UL;
constexpr unsigned long kAlignBottom = 0x8000UL;
constexpr unsigned long kAlignAny    = 0xF000UL;

// Retail member offsets these bodies rely on, read at the call sites cited
// below and pinned against OpenMFC's retail-shaped declarations.
static_assert(offsetof(CWnd, m_hWnd) == 0x40, "CWnd::m_hWnd (retail `mov 0x40(%reg),%rcx` before every USER32 call)");
static_assert(offsetof(CBasePane, m_dwEnabledAlignment) == 0x100,
              "CBasePane::m_dwEnabledAlignment -- GetEnabledAlignment() is the inline "
              "`mov 0x100(%rcx),%eax; ret` at RVA 0x87d0 (mfc140), CBasePane vtable slot 104 (+0x340)");
// CMultiPaneFrameWnd embeds its CPaneContainerManager at +0x258: its
// GetFirstVisiblePane (slot +0x368, unexported body at RVA 0x92390 in mfc140) is
// `add $0x258,%rcx; jmp *[vtbl+0x100]`.  Same constant docking/CMultiPaneFrameWnd.cpp pins.
constexpr std::size_t kOffMultiPaneFrameContainerManager = 0x258;

CGlobalUtils* AfxGlobalUtils() {
    return reinterpret_cast<CGlobalUtils*>(impl__afxGlobalUtils__3VCGlobalUtils__A);
}

bool IsKindOf(const void* p, CRuntimeClass* pClass) {
    return impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(static_cast<const CObject*>(p), pClass) != 0;
}

// Read-only view of the node a CObList::POSITION points at ({pNext, pPrev,
// data}, the CList CNode shape and detail/FilecoreSupport.h ListNodeSnapshot).
// GetHeadPosition/GetNext are inline in retail, so the list is walked through
// the exported FindIndex(0) -- the pattern docking/CPaneDivider.cpp uses.
struct ObNode {
    ObNode*  pNext;
    ObNode*  pPrev;
    CObject* data;
};
static_assert(sizeof(CObList::POSITION) == sizeof(void*), "POSITION is one pointer");
ObNode* HeadNode(const CObList* pList) {
    ObNode* p = nullptr;
    CObList::POSITION pos = impl__FindIndex_CObList__QEBAPEAU__POSITION____J_Z(pList, 0);
    std::memcpy(&p, &pos, sizeof p);
    return p;
}

// CDockingManager::GetDockSiteFrameWnd() is inline in retail (no export) and
// reads m_pParentWnd at +0x1b0 of the retail object.  OpenMFC's CDockingManager
// (include/openmfc/afxmfc.h) is NOT retail-shaped -- m_pParentWnd is its first
// member (+0x8) and the object is only 0x90 bytes -- so +0x1b0 would read past
// the end of every OpenMFC-created manager.  The member is read through the
// declaration instead; it is protected, hence the pointer-to-member idiom.
struct DockingManagerParentAccess : CDockingManager {
    static CFrameWnd* Get(const CDockingManager* p) {
        return p->*(&DockingManagerParentAccess::m_pParentWnd);
    }
};

// CPaneFrameWnd::GetFirstVisiblePane() -- vtable slot +0x368 of the frame.
// Retail dispatches virtually; the only override in the shipping headers is
// CMultiPaneFrameWnd's (afxmultipaneframewnd.h:54, which forwards to its
// embedded manager), so the dispatch is reproduced with an IsKindOf test and
// the two exported bodies.
void* FirstVisiblePaneOf(void* pFrame) {
    if (IsKindOf(pFrame, impl__GetThisClass_CMultiPaneFrameWnd__SAPEAUCRuntimeClass__XZ())) {
        return impl__GetFirstVisiblePane_CPaneContainerManager__UEBAPEAVCWnd__XZ(
            static_cast<unsigned char*>(pFrame) + kOffMultiPaneFrameContainerManager);
    }
    return impl__GetFirstVisiblePane_CPaneFrameWnd__UEBAPEAVCWnd__XZ(pFrame);
}

// CDockablePane::CanAcceptPane -- vtable slot +0x318 (slot 99).  The body in
// CDockablePane's vftable is the unexported RVA 0x12d90 (mfc140), the inline
// definition at afxdockablepane.h:107:
//     if (pBar == NULL) return FALSE;
//     return pBar->IsKindOf(RUNTIME_CLASS(CDockablePane))      // 0x3aa178 (mfc140)
//         && (IsDocked() || IsInFloatingMultiPaneFrameWnd());  // slots +0x2e0, +0x360
BOOL DockablePaneCanAcceptPane(const CDockablePane* pThis, const void* pBar) {
    if (pBar == nullptr) return FALSE;
    if (!IsKindOf(pBar, impl__GetThisClass_CDockablePane__SAPEAUCRuntimeClass__XZ())) return FALSE;
    if (impl__IsDocked_CDockablePane__UEBAHXZ(pThis)) return TRUE;
    return impl__IsInFloatingMultiPaneFrameWnd_CDockablePane__UEBAHXZ(pThis) ? TRUE : FALSE;
}

} // namespace


// ?CyFromString@CGlobalUtils@@QEAAHAEATtagCY@@PEB_W@Z
// int CyFromString(CY& cyResult, const wchar_t* lpszValue)
// Parses a currency string into a CY (scaled by 10000). Returns TRUE on success.
// Symbol: ?CyFromString@CGlobalUtils@@QEAAHAEATtagCY@@PEB_W@Z
extern "C" int MS_ABI impl__CyFromString_CGlobalUtils__QEAAHAEATtagCY__PEB_W_Z(
    void* /*pThis*/, CY* pcyResult, const wchar_t* lpszValue)
{
    if (!pcyResult) return 0;
    pcyResult->int64 = 0;
    if (!lpszValue) return 0;
    HRESULT hr = VarCyFromStr(const_cast<wchar_t*>(lpszValue), GetThreadLocale(), 0, pcyResult);
    return SUCCEEDED(hr) ? 1 : 0;
}
// ?DecimalFromString@CGlobalUtils@@QEAAHAEAUtagDEC@@PEB_W@Z
// int DecimalFromString(DECIMAL& decResult, const wchar_t* lpszValue)
// Parses a decimal string into a DECIMAL. Returns TRUE on success.
// Symbol: ?DecimalFromString@CGlobalUtils@@QEAAHAEAUtagDEC@@PEB_W@Z
extern "C" int MS_ABI impl__DecimalFromString_CGlobalUtils__QEAAHAEAUtagDEC__PEB_W_Z(
    void* /*pThis*/, DECIMAL* pdecResult, const wchar_t* lpszValue)
{
    if (!pdecResult) return 0;
    *pdecResult = DECIMAL();
    if (!lpszValue) return 0;
    HRESULT hr = VarDecFromStr(const_cast<wchar_t*>(lpszValue), GetThreadLocale(), 0, pdecResult);
    return SUCCEEDED(hr) ? 1 : 0;
}
// Retail (0x18006dac0): build a VT_CY VARIANT from the CY value, convert it to
// a BSTR with VariantChangeType, assign the BSTR into the CString (helper
// 0x180002e30 = CStringT assignment), VariantClear both VARIANTs, return
// TRUE on success.  VarBstrFromCy covers the same conversion; the BSTR is
// freed by SysFreeString (retail frees it via VariantClear).
// Symbol: ?StringFromCy@CGlobalUtils@@QEAAHAEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@AEATtagCY@@@Z
extern "C" int MS_ABI impl__StringFromCy_CGlobalUtils__QEAAHAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__AEATtagCY___Z(
    void* /*pThis*/, CString* pstr, CY* pcy)
{
    if (!pstr) return 0;
    if (!pcy) return 0;

    BSTR bstr = nullptr;
    HRESULT hr = ::VarBstrFromCy(*pcy, ::GetThreadLocale(), 0, &bstr);
    if (FAILED(hr)) return 0;

    if (bstr) {
        *pstr = static_cast<const wchar_t*>(bstr);
        ::SysFreeString(bstr);
    } else {
        *pstr = L"";
    }
    return 1;
}
// Retail (0x18006dc50): same shape as StringFromCy but with a VT_DECIMAL
// source -- VariantChangeType(&result, &src, 0, VT_BSTR) followed by the
// CString assignment and VariantClear cleanup.
// Symbol: ?StringFromDecimal@CGlobalUtils@@QEAAHAEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@AEAUtagDEC@@@Z
extern "C" int MS_ABI impl__StringFromDecimal_CGlobalUtils__QEAAHAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__AEAUtagDEC___Z(
    void* /*pThis*/, CString* pstr, DECIMAL* pdec)
{
    if (!pstr) return 0;
    if (!pdec) return 0;

    BSTR bstr = nullptr;
    HRESULT hr = ::VarBstrFromDec(pdec, ::GetThreadLocale(), 0, &bstr);
    if (FAILED(hr)) return 0;

    if (bstr) {
        *pstr = static_cast<const wchar_t*>(bstr);
        ::SysFreeString(bstr);
    } else {
        *pstr = L"";
    }
    return 1;
}
// Symbol: ??0CGlobalUtils@@QEAA@XZ
extern "C" void* MS_ABI impl___0CGlobalUtils__QEAA_XZ(void* pThis) { return new (pThis) CGlobalUtils(); }
// Symbol: ??1CGlobalUtils@@UEAA@XZ
extern "C" void MS_ABI impl___1CGlobalUtils__UEAA_XZ(CGlobalUtils* pThis) { if (pThis) pThis->~CGlobalUtils(); }
// Symbol: ?AdjustRectToWorkArea@CGlobalUtils@@QEAAXAEAVCRect@@PEAV2@@Z
extern "C" void MS_ABI impl__AdjustRectToWorkArea_CGlobalUtils__QEAAXAEAVCRect__PEAV2__Z(CGlobalUtils* pThis, CRect* rect, CRect* delta) { if (pThis && rect) pThis->AdjustRectToWorkArea(*rect, delta); }
// Symbol: ?FlipRect@CGlobalUtils@@QEAAXAEAVCRect@@H@Z
extern "C" void MS_ABI impl__FlipRect_CGlobalUtils__QEAAXAEAVCRect__H_Z(CGlobalUtils* pThis, CRect* rect, int horz) { if (pThis && rect) pThis->FlipRect(*rect, horz); }
// Symbol: ?GetOppositeAlignment@CGlobalUtils@@QEAAKK@Z
extern "C" unsigned long MS_ABI impl__GetOppositeAlignment_CGlobalUtils__QEAAKK_Z(CGlobalUtils* pThis, unsigned long align) { return pThis ? pThis->GetOppositeAlignment(align) : align; }
// Symbol: ?GetSystemBorders@CGlobalUtils@@QEAA?AVCSize@@K@Z
extern "C" void MS_ABI impl__GetSystemBorders_CGlobalUtils__QEAA_AVCSize__K_Z(CSize* ret, CGlobalUtils* pThis, unsigned long style) { new (ret) CSize(pThis ? pThis->GetSystemBorders(style) : CSize()); }
// Symbol: ?GetSystemBorders@CGlobalUtils@@QEAA?AVCSize@@PEAVCWnd@@@Z
extern "C" void MS_ABI impl__GetSystemBorders_CGlobalUtils__QEAA_AVCSize__PEAVCWnd___Z(CSize* ret, CGlobalUtils* pThis, CWnd* wnd) { new (ret) CSize(pThis ? pThis->GetSystemBorders(wnd) : CSize()); }
// Symbol: ?GetWndIcon@CGlobalUtils@@QEAAPEAUHICON__@@PEAVCWnd@@@Z
extern "C" HICON MS_ABI impl__GetWndIcon_CGlobalUtils__QEAAPEAUHICON____PEAVCWnd___Z(CGlobalUtils* pThis, CWnd* wnd) { return pThis ? pThis->GetWndIcon(wnd) : nullptr; }
// Symbol: ?CanBeAttached@CGlobalUtils@@QEBAHPEAVCWnd@@@Z
extern "C" int MS_ABI impl__CanBeAttached_CGlobalUtils__QEBAHPEAVCWnd___Z(const CGlobalUtils* pThis, CWnd* wnd) { return pThis ? pThis->CanBeAttached(wnd) : FALSE; }
// Symbol: ?CanPaneBeInFloatingMultiPaneFrameWnd@CGlobalUtils@@QEBAHPEAVCWnd@@@Z
extern "C" int MS_ABI impl__CanPaneBeInFloatingMultiPaneFrameWnd_CGlobalUtils__QEBAHPEAVCWnd___Z(const CGlobalUtils* pThis, CWnd* wnd) { return pThis ? pThis->CanPaneBeInFloatingMultiPaneFrameWnd(wnd) : FALSE; }
// Symbol: ?GetDockingManager@CGlobalUtils@@QEAAPEAVCDockingManager@@PEAVCWnd@@@Z
extern "C" CDockingManager* MS_ABI impl__GetDockingManager_CGlobalUtils__QEAAPEAVCDockingManager__PEAVCWnd___Z(CGlobalUtils* pThis, CWnd* wnd) { return pThis ? pThis->GetDockingManager(wnd) : nullptr; }
CGlobalUtils::CGlobalUtils() { memset(_globalutils_padding, 0, sizeof(_globalutils_padding)); }
CGlobalUtils::~CGlobalUtils() {}
void CGlobalUtils::AdjustRectToWorkArea(CRect& rect, CRect* pRectDelta) {
    CRect old = rect;
    rect.NormalizeRect();
    RECT workArea = {};
    if (!::SystemParametersInfoW(SPI_GETWORKAREA, 0, &workArea, 0)) {
        workArea.left = 0;
        workArea.top = 0;
        workArea.right = ::GetSystemMetrics(SM_CXSCREEN);
        workArea.bottom = ::GetSystemMetrics(SM_CYSCREEN);
    }
    if (rect.right > workArea.right) rect.OffsetRect(workArea.right - rect.right, 0);
    if (rect.bottom > workArea.bottom) rect.OffsetRect(0, workArea.bottom - rect.bottom);
    if (rect.left < workArea.left) rect.OffsetRect(workArea.left - rect.left, 0);
    if (rect.top < workArea.top) rect.OffsetRect(0, workArea.top - rect.top);
    if (pRectDelta) *pRectDelta = CRect(rect.left - old.left, rect.top - old.top, rect.right - old.right, rect.bottom - old.bottom);
}
void CGlobalUtils::FlipRect(CRect& rect, BOOL bHorz) { if (bHorz) std::swap(rect.left, rect.right); else std::swap(rect.top, rect.bottom); rect.NormalizeRect(); }
DWORD CGlobalUtils::GetOppositeAlignment(DWORD dwAlign) {
    if (dwAlign & CBRS_LEFT) return CBRS_RIGHT;
    if (dwAlign & CBRS_RIGHT) return CBRS_LEFT;
    if (dwAlign & CBRS_TOP) return CBRS_BOTTOM;
    if (dwAlign & CBRS_BOTTOM) return CBRS_TOP;
    return dwAlign;
}
CSize CGlobalUtils::GetSystemBorders(DWORD dwStyle) { (void)dwStyle; return CSize(::GetSystemMetrics(SM_CXFRAME), ::GetSystemMetrics(SM_CYFRAME)); }
CSize CGlobalUtils::GetSystemBorders(CWnd* pWnd) { return GetSystemBorders(pWnd ? static_cast<DWORD>(::GetWindowLongPtrW(pWnd->GetSafeHwnd(), GWL_STYLE)) : 0); }
HICON CGlobalUtils::GetWndIcon(CWnd* pWnd) { return pWnd ? reinterpret_cast<HICON>(::SendMessageW(pWnd->GetSafeHwnd(), WM_GETICON, ICON_SMALL, 0)) : nullptr; }
BOOL CGlobalUtils::CanBeAttached(CWnd* pWnd) const { return pWnd != nullptr; }
BOOL CGlobalUtils::CanPaneBeInFloatingMultiPaneFrameWnd(CWnd* pWnd) const { return pWnd != nullptr; }
CDockingManager* CGlobalUtils::GetDockingManager(CWnd* pWnd) { return pWnd && pWnd->IsKindOf(RUNTIME_CLASS(CFrameWndEx)) ? static_cast<CFrameWndEx*>(pWnd)->GetDockingManager() : nullptr; }
// CGlobalUtils::SetNewParent -- transcribed from retail
// ?SetNewParent@CGlobalUtils@@QEAAXAEAVCObList@@PEAVCWnd@@H@Z, entry RVA 0x6d050
// (mfc140u -- absent from the symbol map, resolved through export ordinal 13385;
// its bytes match mfc140's 0x6cef0).  `this` is never read.  Retail (every
// address in this block is mfc140):
//     for (POSITION pos = lstControlBars.GetHeadPosition(); pos != NULL;) {   // +0x8
//         CBasePane* pWnd = (CBasePane*) lstControlBars.GetNext(pos);         // node +0x10 / +0x0
//         if (bCheckVisibility && !(pWnd->GetStyle() & WS_VISIBLE))           // 0x2a75a0, `bt $0x1c`
//             continue;
//         if (!pWnd->IsKindOf(RUNTIME_CLASS(CPaneDivider))) {                 // descriptor 0x1802f4b58
//             pWnd->ShowWindow(SW_HIDE);                                      // 0x2a79e0, edx = 0
//             CWnd::FromHandle(::SetParent(pWnd->m_hWnd,                      // import SetParent,
//                              pNewParent ? pNewParent->m_hWnd : NULL));      // then 0x289180
//             CRect rectWnd(0,0,0,0);
//             ::GetWindowRect(pWnd->m_hWnd, rectWnd);                         // import GetWindowRect
//             pNewParent->ScreenToClient(rectWnd);                            // 0x2a11f0, unguarded
//             pWnd->SetWindowPos(NULL, -rectWnd.Width(), -rectWnd.Height(),   // vtable +0x480
//                                100, 100, SWP_NOSIZE|SWP_NOZORDER|SWP_NOACTIVATE, NULL);  // 0x15
//             pWnd->ShowWindow(SW_SHOW);                                      // edx = 5
//         } else {
//             CWnd::FromHandle(::SetParent(pWnd->m_hWnd,
//                              pNewParent ? pNewParent->m_hWnd : NULL));
//         }
//     }
// (Call targets are mfc140 RVAs and the CPaneDivider descriptor an mfc140 VA;
// the two USER32 names were resolved with iat.py.  Byte 0x480 of the pane vftables is ?SetWindowPos@CBasePane@@, RVA
// 0xb6a0 in mfc140, verified with vslot.py on the CPane and CDockablePane tables.)
// DEVIATIONS: (1) the SetWindowPos virtual is reached through the CBasePane
// thunk, which forwards to OpenMFC's C++ virtual; (2) ScreenToClient is skipped
// when pNewParent is NULL (retail would fault on its m_hWnd); (3) a NULL list
// yields no iterations (FindIndex NULL-checks) where retail would fault.
// Signature corrected: the generated list omitted `this`.
// Symbol: ?SetNewParent@CGlobalUtils@@QEAAXAEAVCObList@@PEAVCWnd@@H@Z
extern "C" void MS_ABI impl__SetNewParent_CGlobalUtils__QEAAXAEAVCObList__PEAVCWnd__H_Z(
    void* pThis, CObList* plstControlBars, CWnd* pNewParent, int bCheckVisibility) {
    (void)pThis;
    CRuntimeClass* pDividerClass = impl__GetThisClass_CPaneDivider__SAPEAUCRuntimeClass__XZ();
    for (ObNode* pos = HeadNode(plstControlBars); pos != nullptr;) {
        CBasePane* pWnd = static_cast<CBasePane*>(pos->data);
        pos = pos->pNext;

        if (bCheckVisibility && !(impl__GetStyle_CWnd__QEBAKXZ(pWnd) & WS_VISIBLE)) continue;

        const HWND hWndNewParent = pNewParent != nullptr ? pNewParent->m_hWnd : nullptr;
        if (!IsKindOf(pWnd, pDividerClass)) {
            impl__ShowWindow_CWnd__QEAAHH_Z(pWnd, SW_HIDE);
            impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(::SetParent(pWnd->m_hWnd, hWndNewParent));

            RECT rectWnd = {0, 0, 0, 0};
            ::GetWindowRect(pWnd->m_hWnd, &rectWnd);
            if (pNewParent != nullptr) {   // deviation (2)
                impl__ScreenToClient_CWnd__QEBAXPEAUtagRECT___Z(pNewParent, &rectWnd);
            }
            impl__SetWindowPos_CBasePane__UEAAPEAXPEBVCWnd__HHHHIPEAX_Z(
                pWnd, nullptr, rectWnd.left - rectWnd.right, rectWnd.top - rectWnd.bottom,
                100, 100, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE, nullptr);
            impl__ShowWindow_CWnd__QEAAHH_Z(pWnd, SW_SHOW);
        } else {
            impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(::SetParent(pWnd->m_hWnd, hWndNewParent));
        }
    }
}

// CGlobalUtils::ForceAdjustLayout -- transcribed from retail
// ?ForceAdjustLayout@CGlobalUtils@@QEAAXPEAVCDockingManager@@HH@Z, entry RVA
// 0x6d960 (mfc140u; 0x6d800 in mfc140).  `this` is never read.  Retail:
//     if (pDockManager == NULL) return;                                // first test
//     if (!CPane::m_bHandleMinSize && !bForce) return;                  // .data 0x3b6fd0 (mfc140)
//     CWnd* pDockSite = pDockManager->m_pParentWnd;                     // +0x1b0
//     if (pDockSite == NULL) return;
//     if (!::IsWindowVisible(pDockSite->m_hWnd) && !bForceInvisible) return;
//     ::SendMessage(pDockSite->m_hWnd, WM_SETREDRAW, FALSE, 0);
//     CRect rectWnd(0,0,0,0);
//     ::GetWindowRect(pDockSite->m_hWnd, rectWnd);
//     pDockSite->SetWindowPos(NULL, -1, -1, rectWnd.Width() + 1, rectWnd.Height() + 1,
//                             SWP_NOZORDER | SWP_NOMOVE | SWP_NOACTIVATE);    // 0x16, CWnd 0x2a7970
//     pDockSite->SetWindowPos(NULL, -1, -1, rectWnd.Width(), rectWnd.Height(),
//                             SWP_NOZORDER | SWP_NOMOVE | SWP_NOACTIVATE);
//     ::SendMessage(pDockSite->m_hWnd, WM_SETREDRAW, TRUE, 0);
//     ::RedrawWindow(pDockSite->m_hWnd, NULL, NULL, 0x185);
// (IsWindowVisible, SendMessageA, GetWindowRect and RedrawWindow were resolved
// with iat.py against mfc140; SendMessageA there is SendMessageW in mfc140u, so
// ::SendMessage is written and UNICODE picks.  0x185 = RDW_UPDATENOW |
// RDW_ALLCHILDREN | RDW_ERASE | RDW_INVALIDATE.  0x2a7970 is
// ?SetWindowPos@CWnd@@QEAAHPEBV1@HHHHI@Z in mfc140.)
// DEVIATION: retail reads m_pParentWnd at +0x1b0 of the retail object; this
// body reads the m_pParentWnd member of OpenMFC's own CDockingManager, which is
// not retail-shaped (see DockingManagerParentAccess above).
// Signature corrected: the generated list omitted `this`; the shape now matches
// the declaration docking/CMultiPaneFrameWnd.cpp already calls through.
// Symbol: ?ForceAdjustLayout@CGlobalUtils@@QEAAXPEAVCDockingManager@@HH@Z
extern "C" void MS_ABI impl__ForceAdjustLayout_CGlobalUtils__QEAAXPEAVCDockingManager__HH_Z(
    void* pThis, CDockingManager* pDockManager, int bForce, int bForceInvisible) {
    (void)pThis;
    if (pDockManager == nullptr) return;
    if (impl__m_bHandleMinSize_CPane__2HA == 0 && !bForce) return;

    CFrameWnd* pDockSite = DockingManagerParentAccess::Get(pDockManager);
    if (pDockSite == nullptr) return;
    if (!::IsWindowVisible(pDockSite->m_hWnd) && !bForceInvisible) return;

    ::SendMessage(pDockSite->m_hWnd, WM_SETREDRAW, FALSE, 0);

    RECT rectWnd = {0, 0, 0, 0};
    ::GetWindowRect(pDockSite->m_hWnd, &rectWnd);
    const int cx = rectWnd.right - rectWnd.left;
    const int cy = rectWnd.bottom - rectWnd.top;
    constexpr unsigned int kFlags = SWP_NOZORDER | SWP_NOMOVE | SWP_NOACTIVATE;   // 0x16
    impl__SetWindowPos_CWnd__QEAAHPEBV1_HHHHI_Z(pDockSite, nullptr, -1, -1, cx + 1, cy + 1, kFlags);
    impl__SetWindowPos_CWnd__QEAAHPEBV1_HHHHI_Z(pDockSite, nullptr, -1, -1, cx, cy, kFlags);

    ::SendMessage(pDockSite->m_hWnd, WM_SETREDRAW, TRUE, 0);
    ::RedrawWindow(pDockSite->m_hWnd, nullptr, nullptr,
                   RDW_UPDATENOW | RDW_ALLCHILDREN | RDW_ERASE | RDW_INVALIDATE);   // 0x185
}

// CGlobalUtils::CheckAlignment -- transcribed from retail
// ?CheckAlignment@CGlobalUtils@@QEBAHVCPoint@@PEAVCBasePane@@HPEBVCDockingManager@@HAEAKKPEBUtagRECT@@@Z,
// entry RVA 0x6caf0 (mfc140u; 0x6c990 in mfc140).  `this` is never read.
// Retail, with s = nSensitivity and (L,T,R,B) = rectBounds (callee, descriptor
// and static addresses below are mfc140):
//     BOOL bSmartDocking = FALSE; int nHilitedSide = -1;
//     if (pDockManager == NULL && pBar != NULL)
//         pDockManager = afxGlobalUtils.GetDockingManager(                 // 0x6ccc0, this = 0x3aacf8
//                            CWnd::FromHandle(::GetParent(pBar->m_hWnd)));
//     if (pDockManager != NULL) {
//         CSmartDockingManager* pSD = pDockManager->{+0x308};
//         if (pSD != NULL && pSD->{+0xc} && pSD->{+0x8}) {                // IsStarted()
//             nHilitedSide = pSD->{+0x1b8}; bSmartDocking = TRUE;
//         }
//     }
//     CRect rectBounds(0,0,0,0);
//     int nCaptionHeight = 0, nTabAreaBottomHeight = 0;
//     if (pBar != NULL) {
//         ::GetWindowRect(pBar->m_hWnd, rectBounds);
//         if (pBar->IsKindOf(RUNTIME_CLASS(CDockablePane))) {             // 0x3aa178
//             nCaptionHeight = pBar->GetCaptionHeight();                  // vtable +0x358
//             CRect rcTop, rcBottom; pBar->GetTabArea(rcTop, rcBottom);    // vtable +0x668
//             nTabAreaBottomHeight = rcBottom.Height();
//         }
//     } else if (lpRectBounds != NULL) ::CopyRect(rectBounds, lpRectBounds);
//     else return FALSE;
//     if (bOuterEdge) {
//         if (bSmartDocking) switch (nHilitedSide) {    // 0..3 -> LEFT/RIGHT/TOP/BOTTOM
//             case 0: LEFT; case 1: RIGHT; case 2: TOP; case 3: BOTTOM; default: return FALSE; }
//         {L-s, T-s, R+s, T  } hit && (dwEnabled & TOP)    -> dwAlignment = TOP,    TRUE
//         {L-s, T-s, L,   B+s} hit && (dwEnabled & LEFT)   -> LEFT
//         {L-s, B,   R+s, B+s} hit && (dwEnabled & BOTTOM) -> BOTTOM
//         {R,   T-s, R+s, B+s} hit && (dwEnabled & RIGHT)  -> RIGHT
//         return FALSE;
//     }
//     if (bSmartDocking) switch (nHilitedSide - 4) { same mapping as above }
//     {L-s, T-s,          R+s, T+s+nCaptionHeight} TOP
//     {L-s, T-s,          L+s, B+s             } LEFT
//     {L-s, B-s-nTabBot,  R+s, B+s             } BOTTOM
//     {R-s, T-s,          R+s, B+s             } RIGHT
//     return FALSE;
// "hit" is ::PtInRect(rect, point); every hit that passes the enabled-alignment
// test stores the CBRS_ALIGN_* value into dwAlignment and returns TRUE.  (The
// USER32 names -- GetParent, GetWindowRect, CopyRect, PtInRect -- were resolved
// with iat.py; the vtable slots were read on CDockablePane's mfc140 vftable:
// +0x358 is ?GetCaptionHeight@CDockablePane@@, +0x668 the unexported RVA
// 0x3fc20 that ::SetRectEmpty()s both rects, i.e. afxdockablepane.h:86.)
// DEVIATIONS: (1) the smart-docking probe is dropped -- OpenMFC's
// CDockingManager is not retail-shaped (no +0x308 smart-docking manager) and
// CSmartDockingManager is not modelled -- so bSmartDocking stays FALSE and the
// two switch arms above are never taken; GetDockingManager is still called on
// retail's condition, its result unused, as docking/CDockablePane.cpp's
// GetDockingStatus does. (2) GetTabArea is folded to CDockablePane's default
// (both rects empty), so nTabAreaBottomHeight is 0 and a CTabbedPane's or
// CMFCOutlookBar's tab strip is not honoured. (3) GetCaptionHeight dispatches
// statically to the CDockablePane thunk.
// Signature corrected: the generated list omitted `this` and took the by-value
// CPoint as a pointer; it is one 8-byte register argument (`mov %rdx,%rbx`,
// later handed to ::PtInRect as a POINT).
// Symbol: ?CheckAlignment@CGlobalUtils@@QEBAHVCPoint@@PEAVCBasePane@@HPEBVCDockingManager@@HAEAKKPEBUtagRECT@@@Z
extern "C" int MS_ABI impl__CheckAlignment_CGlobalUtils__QEBAHVCPoint__PEAVCBasePane__HPEBVCDockingManager__HAEAKKPEBUtagRECT___Z(
    const void* pThis, long long point, CBasePane* pBar, int nSensitivity,
    const CDockingManager* pDockManager, int bOuterEdge, unsigned long* pdwAlignment,
    unsigned long dwEnabledDockBars, const RECT* lpRectBounds) {
    (void)pThis;
    POINT pt;
    std::memcpy(&pt, &point, sizeof pt);

    if (pDockManager == nullptr && pBar != nullptr) {
        // Deviation (1): evaluated for parity, result not consumed.
        (void)impl__GetDockingManager_CGlobalUtils__QEAAPEAVCDockingManager__PEAVCWnd___Z(
            AfxGlobalUtils(), impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(::GetParent(pBar->m_hWnd)));
    }

    RECT rectBounds = {0, 0, 0, 0};
    int nCaptionHeight = 0;
    int nTabAreaBottomHeight = 0;
    if (pBar != nullptr) {
        ::GetWindowRect(pBar->m_hWnd, &rectBounds);
        if (IsKindOf(pBar, impl__GetThisClass_CDockablePane__SAPEAUCRuntimeClass__XZ())) {
            nCaptionHeight = impl__GetCaptionHeight_CDockablePane__UEBAHXZ(
                reinterpret_cast<const CDockablePane*>(pBar));
            const RECT rectTabAreaBottom = {0, 0, 0, 0};   // deviation (2)
            nTabAreaBottomHeight = rectTabAreaBottom.bottom - rectTabAreaBottom.top;
        }
    } else if (lpRectBounds != nullptr) {
        ::CopyRect(&rectBounds, lpRectBounds);
    } else {
        return FALSE;
    }

    const LONG s = nSensitivity;
    const LONG L = rectBounds.left, T = rectBounds.top, R = rectBounds.right, B = rectBounds.bottom;
    auto test = [&](LONG left, LONG top, LONG right, LONG bottom, unsigned long dwAlign) -> bool {
        const RECT rect = {left, top, right, bottom};
        if (!::PtInRect(&rect, pt) || !(dwEnabledDockBars & dwAlign)) return false;
        *pdwAlignment = dwAlign;
        return true;
    };

    if (bOuterEdge) {
        if (test(L - s, T - s, R + s, T,     kAlignTop))    return TRUE;
        if (test(L - s, T - s, L,     B + s, kAlignLeft))   return TRUE;
        if (test(L - s, B,     R + s, B + s, kAlignBottom)) return TRUE;
        if (test(R,     T - s, R + s, B + s, kAlignRight))  return TRUE;
        return FALSE;
    }

    if (test(L - s, T - s,                        R + s, T + s + nCaptionHeight, kAlignTop))    return TRUE;
    if (test(L - s, T - s,                        L + s, B + s,                  kAlignLeft))   return TRUE;
    if (test(L - s, B - s - nTabAreaBottomHeight, R + s, B + s,                  kAlignBottom)) return TRUE;
    if (test(R - s, T - s,                        R + s, B + s,                  kAlignRight))  return TRUE;
    return FALSE;
}

// CGlobalUtils::GetPaneAndAlignFromPoint -- transcribed from retail
// ?GetPaneAndAlignFromPoint@CGlobalUtils@@QEAAHAEAVCPaneContainerManager@@VCPoint@@PEAPEAVCDockablePane@@AEAKAEAH4@Z,
// entry RVA 0x6d6c0 (mfc140u; 0x6d560 in mfc140).  `this` is never read.
// Retail (s = CDockingManager::m_nDockSensitivity, .data 0x3aaacc in mfc140,
// re-read at each use; the callee RVAs below are mfc140 too):
//     ENSURE(ppTargetControlBar != NULL);             // else AfxThrowInvalidArgException (0x225b80)
//     *ppTargetControlBar = NULL;
//     bCaption = mgr.CheckForMiniFrameAndCaption(pt, ppTargetControlBar);    // mgr vtable +0xe8
//     if (bCaption) return TRUE;
//     *ppTargetControlBar = mgr.PaneFromPoint(pt, s, TRUE, bTabArea, bCaption);   // mgr vtable +0xe0
//     if (!bCaption && !bTabArea) {
//         if (*ppTargetControlBar != NULL) {
//             if (!afxGlobalUtils.CheckAlignment(pt, *ppTargetControlBar, s, NULL, FALSE,
//                                                dwAlignment, CBRS_ALIGN_ANY, NULL))   // 0x6c990
//                 *ppTargetControlBar = NULL;
//             return TRUE;
//         }
//     } else if (*ppTargetControlBar != NULL) return TRUE;
//     mgr.PaneFromPoint(pt, s, FALSE, bTabArea, bCaption);   // result discarded, only the
//     return TRUE;                                           // two out-flags are refreshed
// Note CheckAlignment is called on afxGlobalUtils (`lea 0x3aacf8`), not on
// `this`.  Manager slots +0xe0 / +0xe8 of the CPaneContainerManager vftable (RVA
// 0x2f41a8 in mfc140, from its constructor 0xa80b0) are ?PaneFromPoint@ (0xaa3d0)
// and ?CheckForMiniFrameAndCaption@ (0xaab80), verified with vslot.py.
// DEVIATIONS: (1) both manager virtuals are called through their thunks --
// OpenMFC's CPaneContainerManager constructor leaves the vftable NULL, so a
// virtual dispatch is impossible; (2) CheckForMiniFrameAndCaption's thunk is
// still a generated placeholder that returns FALSE, so the mini-frame-caption
// early-out never fires here; (3) the ENSURE throw is followed by `return
// FALSE`, since the OpenMFC throw helper is not declared noreturn.
// Signature corrected: the generated list omitted `this` (its six slots were
// shifted one register left) and spelled the by-value CPoint as `void*`.
// Symbol: ?GetPaneAndAlignFromPoint@CGlobalUtils@@QEAAHAEAVCPaneContainerManager@@VCPoint@@PEAPEAVCDockablePane@@AEAKAEAH4@Z
extern "C" int MS_ABI impl__GetPaneAndAlignFromPoint_CGlobalUtils__QEAAHAEAVCPaneContainerManager__VCPoint__PEAPEAVCDockablePane__AEAKAEAH4_Z(
    void* pThis, void* pBarContainerManager, long long pt, void** ppTargetControlBar,
    unsigned long* pdwAlignment, int* pbTabArea, int* pbCaption) {
    (void)pThis;
    if (ppTargetControlBar == nullptr) {
        impl__AfxThrowInvalidArgException__YAXXZ();
        return FALSE;   // deviation (3)
    }
    *ppTargetControlBar = nullptr;

    void* ptArg = nullptr;   // PaneFromPoint's definition spells the CPoint as void*
    std::memcpy(&ptArg, &pt, sizeof ptArg);

    *pbCaption = impl__CheckForMiniFrameAndCaption_CPaneContainerManager__UEAAHVCPoint__PEAPEAVCDockablePane___Z(
        pBarContainerManager, pt, ppTargetControlBar);
    if (*pbCaption) return TRUE;

    void* pBar = impl__PaneFromPoint_CPaneContainerManager__UEAAPEAVCDockablePane__VCPoint__HHAEAH1_Z(
        pBarContainerManager, ptArg, impl__m_nDockSensitivity_CDockingManager__2HA, TRUE, pbTabArea, pbCaption);
    *ppTargetControlBar = pBar;

    if (*pbCaption == 0 && *pbTabArea == 0) {
        if (pBar != nullptr) {
            if (!impl__CheckAlignment_CGlobalUtils__QEBAHVCPoint__PEAVCBasePane__HPEBVCDockingManager__HAEAKKPEBUtagRECT___Z(
                    AfxGlobalUtils(), pt, static_cast<CBasePane*>(pBar),
                    impl__m_nDockSensitivity_CDockingManager__2HA, nullptr, FALSE,
                    pdwAlignment, kAlignAny, nullptr)) {
                *ppTargetControlBar = nullptr;
            }
            return TRUE;
        }
    } else if (pBar != nullptr) {
        return TRUE;
    }

    impl__PaneFromPoint_CPaneContainerManager__UEAAPEAVCDockablePane__VCPoint__HHAEAH1_Z(
        pBarContainerManager, ptArg, impl__m_nDockSensitivity_CDockingManager__2HA, FALSE, pbTabArea, pbCaption);
    return TRUE;
}

// CGlobalUtils::CalcExpectedDockedRect -- transcribed from retail
// ?CalcExpectedDockedRect@CGlobalUtils@@QEAAXAEAVCPaneContainerManager@@PEAVCWnd@@VCPoint@@AEAVCRect@@AEAHPEAPEAVCDockablePane@@@Z,
// entry RVA 0x6d1a0 (mfc140u; 0x6d040 in mfc140).  Retail, in instruction
// order (every callee, descriptor and static address in this block is mfc140;
// USER32 names resolved with iat.py):
//     ENSURE(ppTargetBar != NULL);                     // else AfxThrowInvalidArgException
//     bDrawTab = FALSE; *ppTargetBar = NULL;
//     DWORD dwAlignment = CBRS_ALIGN_LEFT; BOOL bTabArea = FALSE, bCaption = FALSE;
//     ::SetRectEmpty(rectResult);
//     if (::GetKeyState(VK_CONTROL) < 0) return;
//     if (!GetPaneAndAlignFromPoint(mgr, ptMouse, ppTargetBar, dwAlignment,
//                                   bTabArea, bCaption)) return;   // 0x6d560, on `this`
//     if (*ppTargetBar == NULL) return;
//     CPane* pBar = NULL;
//     if (pWndToDock->IsKindOf(RUNTIME_CLASS(CPaneFrameWnd)))               // 0x3aa418
//         pBar = DYNAMIC_DOWNCAST(CPane, DYNAMIC_DOWNCAST(CPaneFrameWnd, pWndToDock)
//                                            ->GetFirstVisiblePane());        // frame vtable +0x368
//     else
//         pBar = DYNAMIC_DOWNCAST(CPane, pWndToDock);                        // 0x2f3868
//     CDockablePane* pT = *ppTargetBar;
//     DWORD dwTargetEnabled = pT->GetEnabledAlignment();                    // +0x340
//     DWORD dwTargetCurrent = pT->GetCurrentAlignment();                    // +0x338
//     CPaneFrameWnd* pTargetFrame = pT->GetParentMiniFrame(FALSE);          // +0x460
//     if (pBar != NULL) {
//         if (pBar->GetEnabledAlignment() != dwTargetEnabled && pTargetFrame != NULL) return;
//         if (!(pBar->GetEnabledAlignment() & dwTargetCurrent) && pTargetFrame == NULL) return;
//     }
//     if (bTabArea || bCaption) {
//         bDrawTab = pT->CanBeAttached() && CanBeAttached(pWndToDock) && pBar != NULL   // +0x328, 0x6d430
//                 && pBar->GetEnabledAlignment() == pT->GetEnabledAlignment();
//         if (!bDrawTab) return;
//     }
//     if (pT->GetParentMiniFrame(FALSE) != NULL &&
//         !CanPaneBeInFloatingMultiPaneFrameWnd(pWndToDock)) { bDrawTab = FALSE; return; }  // 0x6d490
//     if (pWndToDock->IsKindOf(RUNTIME_CLASS(CBasePane)) &&                 // 0x2da490
//         !pT->CanAcceptPane((CBasePane*) pWndToDock)) { bDrawTab = FALSE; return; }         // +0x318
//     CRect rectTargetBar(0,0,0,0); ::GetWindowRect(pT->m_hWnd, rectTargetBar);
//     if (pT == pWndToDock) { bDrawTab = FALSE; return; }
//     if (pWndToDock->IsKindOf(RUNTIME_CLASS(CPaneFrameWnd)) &&
//         pT->GetParentMiniFrame(FALSE) == pWndToDock) { bDrawTab = FALSE; return; }
//     CRect rectFinal(0,0,0,0); ::GetWindowRect(pWndToDock->m_hWnd, rectFinal);
//     if (pBar == NULL) return;
//     if (!(pBar->GetEnabledAlignment() & dwAlignment) &&
//         !CDockingManager::m_bIgnoreEnabledAlignment) return;              // .data 0x3b6f5c
//     CRect rectSlider; DWORD dwSliderStyle;
//     mgr.CalcRects(rectTargetBar, rectFinal, rectSlider, dwSliderStyle, dwAlignment,
//                   CSize(0,0), CSize(0,0));                                 // 0xa95d0
//     rectResult = rectFinal;
// The `pT` NULL tests retail repeats after the early `*ppTargetBar == NULL`
// return can never fail and are not reproduced; nor is the IsKindOf re-test
// inside DYNAMIC_DOWNCAST(CPaneFrameWnd, pWndToDock), which only runs after the
// same test has just succeeded.  The second CPaneFrameWnd test (before the
// GetParentMiniFrame == pWndToDock comparison) reuses the first result, as
// pWndToDock has not changed.  Vtable slots were read on CDockablePane's
// mfc140 vftable (0x2e3068) and matched to afxbasepane.h declaration order:
// +0x318 CanAcceptPane, +0x328 CanBeAttached, +0x338 GetCurrentAlignment
// (0xce30), +0x340 GetEnabledAlignment (0x87d0), +0x460 GetParentMiniFrame
// (0xb460); CPaneFrameWnd's +0x368 is ?GetFirstVisiblePane@ (0xaf910).
// DEVIATIONS: (1) every virtual is dispatched statically: GetEnabledAlignment
// reads m_dwEnabledAlignment (its only definition, afxbasepane.h:101),
// GetCurrentAlignment / GetParentMiniFrame go to the CBasePane thunks (no
// CDockablePane-derived class overrides either in the shipping headers; the
// one GetCurrentAlignment override, afxoutlookbarpane.h:113, is on the
// CMFCToolBar-derived CMFCOutlookBarPane and cannot be the target here),
// IsDocked / IsInFloatingMultiPaneFrameWnd inside CanAcceptPane go to the
// CDockablePane thunks, GetFirstVisiblePane to FirstVisiblePaneOf,
// CanAcceptPane to CDockablePane's inline body (a CMFCOutlookBar override is
// missed), and pT->CanBeAttached() is CDockablePane's inline `return TRUE`
// (afxdockablepane.h:119).  (2) CanBeAttached(pWnd) and
// CanPaneBeInFloatingMultiPaneFrameWnd(pWnd) are this file's exports, whose
// current bodies above are `pWnd != nullptr` approximations, not transcriptions
// of retail 0x6d430 / 0x6d490.  (3) the throw is followed by `return`, a NULL
// rectResult skips the final store, and a NULL pWndToDock is treated as "not a
// frame / not a pane" instead of faulting in IsKindOf.
// Signature corrected: the generated list omitted `this` and took the by-value
// CPoint as a pointer.
// Symbol: ?CalcExpectedDockedRect@CGlobalUtils@@QEAAXAEAVCPaneContainerManager@@PEAVCWnd@@VCPoint@@AEAVCRect@@AEAHPEAPEAVCDockablePane@@@Z
extern "C" void MS_ABI impl__CalcExpectedDockedRect_CGlobalUtils__QEAAXAEAVCPaneContainerManager__PEAVCWnd__VCPoint__AEAVCRect__AEAHPEAPEAVCDockablePane___Z(
    void* pThis, void* pBarContainerManager, CWnd* pWndToDock, long long ptMouse,
    RECT* pRectResult, int* pbDrawTab, void** ppTargetBar) {
    if (ppTargetBar == nullptr) {
        impl__AfxThrowInvalidArgException__YAXXZ();
        return;   // deviation (3)
    }
    *pbDrawTab = FALSE;
    *ppTargetBar = nullptr;
    unsigned long dwAlignment = kAlignLeft;
    int bTabArea = FALSE;
    int bCaption = FALSE;
    ::SetRectEmpty(pRectResult);

    if (::GetKeyState(VK_CONTROL) < 0) return;
    if (!impl__GetPaneAndAlignFromPoint_CGlobalUtils__QEAAHAEAVCPaneContainerManager__VCPoint__PEAPEAVCDockablePane__AEAKAEAH4_Z(
            pThis, pBarContainerManager, ptMouse, ppTargetBar, &dwAlignment, &bTabArea, &bCaption)) {
        return;
    }
    if (*ppTargetBar == nullptr) return;

    CRuntimeClass* pFrameClass = impl__GetThisClass_CPaneFrameWnd__SAPEAUCRuntimeClass__XZ();
    CRuntimeClass* pPaneClass  = impl__GetThisClass_CPane__SAPEAUCRuntimeClass__XZ();
    const bool bDockingFrame = pWndToDock != nullptr && IsKindOf(pWndToDock, pFrameClass);

    CBasePane* pBar = nullptr;   // a CPane in retail; only CBasePane members are read
    if (bDockingFrame) {
        void* pFirst = FirstVisiblePaneOf(pWndToDock);
        if (pFirst != nullptr && IsKindOf(pFirst, pPaneClass)) pBar = static_cast<CBasePane*>(pFirst);
    } else if (pWndToDock != nullptr && IsKindOf(pWndToDock, pPaneClass)) {
        pBar = static_cast<CBasePane*>(pWndToDock);
    }

    CBasePane* pTarget = static_cast<CBasePane*>(*ppTargetBar);
    const unsigned long dwTargetEnabled = pTarget->m_dwEnabledAlignment;
    const unsigned long dwTargetCurrent = impl__GetCurrentAlignment_CBasePane__UEBAKXZ(pTarget);
    void* pTargetMiniFrame = impl__GetParentMiniFrame_CBasePane__UEBAPEAVCPaneFrameWnd__H_Z(pTarget, FALSE);
    if (pBar != nullptr) {
        if (pBar->m_dwEnabledAlignment != dwTargetEnabled && pTargetMiniFrame != nullptr) return;
        if ((pBar->m_dwEnabledAlignment & dwTargetCurrent) == 0 && pTargetMiniFrame == nullptr) return;
    }

    if (bTabArea || bCaption) {
        constexpr BOOL kTargetCanBeAttached = TRUE;   // deviation (1)
        BOOL bDrawTab = FALSE;
        if (kTargetCanBeAttached &&
            impl__CanBeAttached_CGlobalUtils__QEBAHPEAVCWnd___Z(static_cast<const CGlobalUtils*>(pThis), pWndToDock) &&
            pBar != nullptr) {
            bDrawTab = pBar->m_dwEnabledAlignment == pTarget->m_dwEnabledAlignment ? TRUE : FALSE;
        }
        *pbDrawTab = bDrawTab;
        if (!bDrawTab) return;
    }

    if (impl__GetParentMiniFrame_CBasePane__UEBAPEAVCPaneFrameWnd__H_Z(pTarget, FALSE) != nullptr &&
        !impl__CanPaneBeInFloatingMultiPaneFrameWnd_CGlobalUtils__QEBAHPEAVCWnd___Z(
            static_cast<const CGlobalUtils*>(pThis), pWndToDock)) {
        *pbDrawTab = FALSE;
        return;
    }
    if (pWndToDock != nullptr && IsKindOf(pWndToDock, impl__GetThisClass_CBasePane__SAPEAUCRuntimeClass__XZ()) &&
        !DockablePaneCanAcceptPane(reinterpret_cast<const CDockablePane*>(pTarget), pWndToDock)) {
        *pbDrawTab = FALSE;
        return;
    }

    RECT rectTargetBar = {0, 0, 0, 0};
    ::GetWindowRect(pTarget->m_hWnd, &rectTargetBar);
    if (static_cast<void*>(pTarget) == static_cast<void*>(pWndToDock)) {
        *pbDrawTab = FALSE;
        return;
    }
    if (bDockingFrame &&
        impl__GetParentMiniFrame_CBasePane__UEBAPEAVCPaneFrameWnd__H_Z(pTarget, FALSE) == static_cast<void*>(pWndToDock)) {
        *pbDrawTab = FALSE;
        return;
    }

    RECT rectFinal = {0, 0, 0, 0};
    ::GetWindowRect(pWndToDock != nullptr ? pWndToDock->m_hWnd : nullptr, &rectFinal);
    if (pBar == nullptr) return;
    if ((pBar->m_dwEnabledAlignment & dwAlignment) == 0 &&
        impl__m_bIgnoreEnabledAlignment_CDockingManager__2HA == 0) {
        return;
    }

    RECT rectSlider = {0, 0, 0, 0};
    unsigned long dwSliderStyle = 0;
    impl__CalcRects_CPaneContainerManager__QEAAXAEAVCRect__00AEAKKVCSize__2_Z(
        pBarContainerManager, &rectTargetBar, &rectFinal, &rectSlider, &dwSliderStyle,
        dwAlignment, 0ULL, 0ULL);
    if (pRectResult != nullptr) *pRectResult = rectFinal;   // deviation (3)
}
