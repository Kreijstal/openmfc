// COleControl — OpenMFC implementation.
// Sources: global_ole-5_impl.cpp, global_ole-6_impl.cpp, global_olecontrol_batch1.cpp, global_olecontrol_batch2.cpp, manual_small_stub_implementations.cpp, olecore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/COleControlSupport.h"
#include "detail/Ole6ImplSupport.h"
#include "detail/OlecontrolBatch2Support.h"
#include "detail/ManualSmallStubImplementationsSupport.h"
#include "detail/OlecoreSupport.h"

// COleControl methods that the corrected retail member layout makes
// implementable for real. Every function here was decoded from the retail
// mfc140u export (ordinal -> RVA -> objdump) rather than inferred, and the
// member offsets the disassembly uses are exactly the ones transcribed into
// include/openmfc/afxole.h (see the static_asserts in olecore.cpp).
//
// The retail bitfield word lives at this+0x160; bit positions referenced below
// are the ones the shipping code actually tests:
//   bit  2 = m_bModified, bit 10 = m_bInPlaceActive.
//
// Kept in a global_*.cpp file so the build's AUTO_EXCLUDES pass drops the
// generated stubs for these ordinals.
//
// Resolving an export that mfc140_rva_symbols.json reports as "NOT FOUND":
// that map is rva -> symbol, so when the linker's /OPT:ICF folded several
// exports onto one address only one of them survives the inversion (14,022
// exports, 12,478 distinct RVAs).  The lost ones are recoverable by joining
// mfc_complete_ordinal_mapping.json (symbol -> ordinal, mfc140u) against the
// export address table of
//   ~/msvc/VC/Redist/MSVC/14.51.36231/x64/Microsoft.VC145.MFC/mfc140u.dll
// (objdump -p, "Ordinal  Address" rows).  Several bodies below were decoded
// that way and cite mfc140u RVAs; the DLL-wide folded bodies worth knowing are
// 0x27d0 `ret` (158 exports fold onto it), 0x71e0 `xor eax,eax ; ret` (134) and
// 0x3a60 `mov eax,1 ; ret` (101) -- counts measured by that same ordinal join
// over mfc140u, not by mfc140_rva_symbols.json, whose own totals (14,022 named
// ordinals resolving to 12,478 distinct RVAs) describe the MBCS mfc140.dll.
// Do NOT mix the two RVA spaces: RVAs cited as
// "mfc140" elsewhere in this file come from the MBCS mfc140.dll and differ.
// The real MFC headers on this host
// (~/msvc/VC/Tools/MSVC/14.51.36231/atlmfc/include/afxctl.h) give the virtual
// declaration order, which is what pins the vtable slot numbers cited below.

#include "openmfc/afxwin.h"
#include "openmfc/afxdisp.h"
#include "openmfc/afxole.h"

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

// ---- sibling impl_ exports called by the bodies in this file ----
// (see the link rule at the top of the campaign briefing: C++ methods of other
// classes exist in this DLL only as impl__ thunks, so they must be called by
// their thunk name.  Every declaration below matches a definition that exists
// today -- CFontHolder.cpp, CDC.cpp, CCmdTarget.cpp, CMFC_misc_stubs.cpp,
// CStringArray.cpp, CDWordArray.cpp, core/window/Thunks.cpp.)
extern "C" void* MS_ABI
impl__GetFontDispatch_CFontHolder__QEAAPEAUIFontDisp__XZ(void* pHolder);
extern "C" void MS_ABI
impl__QueryTextMetrics_CFontHolder__QEAAXPEAUtagTEXTMETRICW___Z(void* pHolder,
                                                               TEXTMETRICW* ptm);
extern "C" int MS_ABI
impl__GetDisplayString_CFontHolder__QEAAHAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
    void* pHolder, CString* pValue);
extern "C" CDC* MS_ABI impl__FromHandle_CDC__SAPEAV1_PEAUHDC_____Z(HDC hDC);
extern "C" __int64 MS_ABI impl__Default_CWnd__IEAA_JXZ(CWnd* pThis);
extern "C" CWinThread* MS_ABI impl__AfxGetThread__YAPEAVCWinThread__XZ();
extern "C" int MS_ABI impl__AfxLoadString__YAHIPEA_WI_Z(unsigned int nID,
                                                        wchar_t* lpszBuf,
                                                        unsigned int nMaxBuf);
extern "C" void MS_ABI impl__EnableTypeLib_CCmdTarget__QEAAXXZ(CCmdTarget* pThis);
extern "C" void MS_ABI
impl__SetAtGrow_CStringArray__QEAAX_JAEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
    CStringArray* pThis, long long nIndex, const CString& value);
extern "C" void MS_ABI impl__SetAtGrow_CDWordArray__QEAAX_JK_Z(
    CDWordArray* pThis, long long nIndex, unsigned long value);
extern "C" void* MS_ABI impl___0CStringArray__QEAA_XZ(CStringArray* pThis);
extern "C" void MS_ABI impl___1CStringArray__UEAA_XZ(CStringArray* pThis);
extern "C" long long MS_ABI impl__Append_CStringArray__QEAA_JAEBV1__Z(
    CStringArray* pThis, const CStringArray* pSrc);
extern "C" void* MS_ABI impl___0CDWordArray__QEAA_XZ(CDWordArray* pThis);
extern "C" void MS_ABI impl___1CDWordArray__UEAA_XZ(CDWordArray* pThis);
extern "C" long long MS_ABI impl__Append_CDWordArray__QEAA_JAEBV1__Z(
    CDWordArray* pThis, const CDWordArray* pSrc);

// ---- forward declarations of exports defined later in this file ----
extern "C" void MS_ABI impl__InitStockEventMask_COleControl__IEAAXXZ(
    COleControl* pThis);
extern "C" void MS_ABI impl__InitStockPropMask_COleControl__IEAAXXZ(
    COleControl* pThis);
extern "C" int MS_ABI impl__OnGetViewExtent_COleControl__UEAAHKJPEAUtagDVTARGETDEVICE__PEAUtagSIZE___Z(
    COleControl* pThis, unsigned long dwAspect, long lindex, void* ptd,
    SIZEL* lpsizel);

// ---- sibling impl_ exports used by the window-message handlers below ----
// (CWnd::OnWndMsg lives in core/window/CWnd.cpp; CReflectorWnd::SetControl in
// core/window/CReflectorWnd.cpp.  Both definitions were checked before being
// declared here, per the campaign's link rule.)
extern "C" int MS_ABI impl__OnWndMsg_CWnd__MEAAHI_K_JPEA_J_Z(
    CWnd* pThis, UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);
extern "C" void MS_ABI impl__SetControl_CReflectorWnd__QEAAXPEAVCOleControl___Z(
    void* pThis, void* pControl);

// ---- further sibling impl_ exports used by the bodies added below ----
// Every one of these was checked to have a definition before being declared
// here, per the campaign's link rule:
//   core/ole/Thunks.cpp                 IsSubclassedControl
//   core/runtime/CCmdTarget.cpp         GetInterface, GetThisConnectionMap
//   core/gdi/CRectTracker.cpp           HitTest, Draw
//   featurepack/CMFC_misc_stubs.cpp     AfxLockTempMaps / AfxUnlockTempMaps
extern "C" int MS_ABI impl__IsSubclassedControl_COleControl__UEAAHXZ(COleControl* pThis);
extern "C" IUnknown* MS_ABI impl__GetInterface_CCmdTarget__QEAAPEAUIUnknown__PEBX_Z(
    CCmdTarget* pThis, const void* piid);
extern "C" const void* MS_ABI
impl__GetThisConnectionMap_CCmdTarget__KAPEBUAFX_CONNECTIONMAP__XZ();
extern "C" int MS_ABI impl__HitTest_CRectTracker__QEBAHVCPoint___Z(
    const void* pThis, unsigned long long point);
extern "C" void MS_ABI impl__Draw_CRectTracker__QEBAXPEAVCDC___Z(
    const void* pThis, void* pDC);
extern "C" void MS_ABI impl__AfxLockTempMaps__YAXXZ();
extern "C" int MS_ABI impl__AfxUnlockTempMaps__YAHH_Z(int bDeleteTemps);
extern "C" int MS_ABI impl__SaveDC_CDC__QEAAHXZ(CDC* pThis);
extern "C" int MS_ABI impl__RestoreDC_CDC__QEAAHH_Z(CDC* pThis, int nSavedDC);

namespace {

// ---- COleControl's two static maps, copied out of retail .rdata -------------
// ?GetConnectionMap@COleControl@@ and ?GetThisConnectionMap@COleControl@@ are
// one ICF-folded body at mfc140u 0x1deff0 (`lea rax,[rip+0x144301] ; ret`) that
// hands back the AFX_CONNECTIONMAP at rva 0x3232f8.  Its 16 bytes read
//     { pfnGetBaseMap = 0x1de8e0, lpEntries = 0x34b5d0 }
// where 0x1de8e0 is CCmdTarget::GetThisConnectionMap (itself ICF-folded with
// CCmdTarget::GetConnectionMap), and the entry table at 0x34b5d0 reads
//     { piid = 0x2d9d28, nOffset = 0x330 }, { NULL, (size_t)-1 }
// with the 16 bytes at 0x2d9d28 being 02 bc fb 9b f1 ef 1a 10 84 ed 00 aa 00 34
// 1d 07 = IID_IPropertyNotifySink.  nOffset 0x330 is afxole.h's m_xPropConnPt.
//
// ?GetEventMap@COleControl@@ and ?GetThisEventMap@COleControl@@ are likewise
// one body at mfc140u 0x1e3560 yielding the AFX_EVENTMAP at rva 0x33e2d0 --
// whose 24 bytes are all zero.  COleControl is the root of the event-map chain
// (pfnGetBaseMap NULL ends the walk) and declares no events, and its
// lpStockEventMask is never dereferenced because InitStockEventMask reads that
// field only from the *first* map in the chain, i.e. from the derived control's.
//
// The two struct shapes below are the real afxctl.h AFX_EVENTMAP /
// AFX_EVENTMAP_ENTRY and afxwin.h AFX_CONNECTIONMAP / AFX_CONNECTIONMAP_ENTRY;
// core/runtime/CCmdTarget.cpp models the same two for CCmdTarget's own maps.
struct EventMapEntry {                          // AFX_EVENTMAP_ENTRY, 0x18 bytes
    unsigned int   flags;                       // +0x00
    long           dispid;                      // +0x04
    const wchar_t* pszName;                     // +0x08
    const char*    lpszParams;                  // +0x10
};
struct EventMap {                               // AFX_EVENTMAP
    const EventMap* (MS_ABI* pfnGetBaseMap)();  // +0x00
    const EventMapEntry* lpEntries;             // +0x08
    unsigned long*       lpStockEventMask;      // +0x10
};
struct ConnectionMapEntry {                     // AFX_CONNECTIONMAP_ENTRY
    const IID* piid;
    size_t     nOffset;
};
struct ConnectionMap {                          // AFX_CONNECTIONMAP
    const void* (MS_ABI* pfnGetBaseMap)();
    const ConnectionMapEntry* pEntry;
};
static_assert(sizeof(EventMapEntry) == 0x18,
              "AFX_EVENTMAP_ENTRY is 0x18 bytes -- retail strides the entry array by 0x18");
static_assert(offsetof(EventMapEntry, dispid) == 0x04, "AFX_EVENTMAP_ENTRY::dispid at +0x04");
static_assert(offsetof(EventMapEntry, pszName) == 0x08, "AFX_EVENTMAP_ENTRY::pszName at +0x08");
static_assert(offsetof(EventMap, lpEntries) == 0x08, "AFX_EVENTMAP::lpEntries at +0x08");
static_assert(offsetof(EventMap, lpStockEventMask) == 0x10,
              "AFX_EVENTMAP::lpStockEventMask at +0x10");

const EventMap g_eventMap_COleControl = { nullptr, nullptr, nullptr };

// {9BFBBC02-EFF1-101A-84ED-00AA00341D07} -- the bytes at mfc140u rva 0x2d9d28.
const IID kIID_IPropertyNotifySink =
    { 0x9BFBBC02, 0xEFF1, 0x101A, { 0x84, 0xED, 0x00, 0xAA, 0x00, 0x34, 0x1D, 0x07 } };
static_assert(offsetof(COleControl, m_xPropConnPt) == 0x330,
              "retail's connection-map entry names nOffset 0x330");
const ConnectionMapEntry g_connEntries_COleControl[] = {
    { &kIID_IPropertyNotifySink, offsetof(COleControl, m_xPropConnPt) },
    { nullptr, (size_t)-1 }
};
const ConnectionMap g_connectionMap_COleControl = {
    &impl__GetThisConnectionMap_CCmdTarget__KAPEBUAFX_CONNECTIONMAP__XZ,
    g_connEntries_COleControl
};

// CRectTracker as OpenMFC lays it out (phase4/src/core/gdi/CRectTracker.cpp,
// harvested with cl.exe /d1reportSingleClassLayoutCRectTracker, size 80).  The
// two fields the COleControl bodies below read are exactly the two the retail
// disassembly reads at +0x08 and +0x24, so the layouts agree.
struct TrackerView {
    const void*  vfptr;          // 0x00
    unsigned int m_nStyle;       // 0x08
    RECT         m_rect;         // 0x0C
    SIZE         m_sizeMin;      // 0x1C
    int          m_nHandleSize;  // 0x24
};
static_assert(offsetof(TrackerView, m_nStyle) == 0x08, "CRectTracker::m_nStyle at +0x08");
static_assert(offsetof(TrackerView, m_rect) == 0x0C, "CRectTracker::m_rect at +0x0C");
static_assert(offsetof(TrackerView, m_nHandleSize) == 0x24, "CRectTracker::m_nHandleSize at +0x24");
// CRectTracker::resizeInside | CRectTracker::resizeOutside
const unsigned int kTrackerResizeMask = 0x18;

// The unexported border helper the OnNcPaint body below calls, transcribed from
// retail mfc140u 0x1e03a0.  It has no export, so it is reproduced here:
//     lea rbx,[rcx+8]                 ; &pDC->m_hDC (CDC::m_hDC is at +8, which
//                                     ;  is also where OpenMFC's CDC keeps it --
//                                     ;  see DcHandle() in core/gdi/CRectTracker.cpp)
//     if (bBorder)  DrawEdge(hdc, lpRect,
//                            bClientEdge ? 0x0C : 0x03,
//                            bClientEdge ? 0x600F : 0xA00F);
//     if (bClientEdge) DrawEdge(hdc, lpRect, 0x0A, 0x200F);
// The two flag words are literally what the `sbb`/`and`/`add` idioms compute:
// 0xA00F = BF_MONO|BF_ADJUST|BF_RECT, 0x600F = BF_FLAT|BF_ADJUST|BF_RECT,
// 0x200F = BF_ADJUST|BF_RECT, 0x0A = EDGE_SUNKEN.  BF_ADJUST means each call
// shrinks *lpRect in place, and the caller depends on that: the scrollbar-corner
// rectangle is computed from the rect AFTER these calls have adjusted it.
void DrawTrackerBorder(HDC hdc, RECT* lpRect, int bBorder, int bClientEdge)
{
    if (bBorder)
        ::DrawEdge(hdc, lpRect, bClientEdge ? 0x0Cu : 0x03u,
                   (unsigned int)(bClientEdge ? 0x600F : 0xA00F));
    if (bClientEdge)
        ::DrawEdge(hdc, lpRect, 0x0Au, 0x200Fu);
}

// Retail's RELEASE helper (mfc140 0x26ba84), used by OnHide below:
//     if (*ppUnk != NULL) { (*ppUnk)->Release(); *ppUnk = NULL; }
// (the disassembly loads vtable slot 2 -- byte offset 0x10 -- i.e. Release.)
void ReleaseIface(void** ppUnk)
{
    if (ppUnk == nullptr || *ppUnk == nullptr) return;
    IUnknown* p = static_cast<IUnknown*>(*ppUnk);
    p->Release();
    *ppUnk = nullptr;
}

// COleControl::ParentToClient(LPCRECT, LPPOINT, BOOL) const, transcribed from
// retail mfc140 0x1e4040:
//     GetClientOffset(&dx, &dy);                    // virtual slot 0x4a8
//     UINT nHit = 0;
//     if (bHitTest && PtInRect(lprcBounds, *pPoint)) {
//         if (dx > 0) {
//             RECT rc = *lprcBounds; InflateRect(&rc, -dx, -dy);
//             nHit = PtInRect(&rc, *pPoint) ? HTCLIENT : HTBORDER;
//         } else {
//             nHit = HTCLIENT;
//         }
//     }
//     pPoint->x -= lprcBounds->left + dx;           // runs on every path
//     pPoint->y -= lprcBounds->top  + dy;
//     return nHit;
// (The HTCLIENT/HTBORDER pair is the retail `neg eax; sbb esi,esi;
// and 0xffffffef,esi; add 0x12,esi` idiom: 1 when the inner PtInRect succeeded,
// 18 when it did not.)
//
// The exported ?ParentToClient@COleControl@@UEBAIPEBUtagRECT@@PEAUtagPOINT@@H@Z
// is still an auto-generated stub further down this same translation unit, and
// its generated prototype omits `this`, so it cannot be called ABI-correctly
// from here.  The transform is therefore inlined; retail reaches it through
// vtable slot 0x4b0, so a derived override would be honoured there and is not
// here.
unsigned int ParentToClientLocal(const COleControl* pThis, const RECT* lprcBounds,
                                 POINT* pPoint, int bHitTest)
{
    long dx = 0, dy = 0;
    impl__GetClientOffset_COleControl__UEBAXPEAJ0_Z(pThis, &dx, &dy);

    unsigned int nHit = 0;
    if (bHitTest && lprcBounds != nullptr && pPoint != nullptr &&
        ::PtInRect(lprcBounds, *pPoint)) {
        if (dx > 0) {
            RECT rc = *lprcBounds;
            ::InflateRect(&rc, -dx, -dy);
            nHit = ::PtInRect(&rc, *pPoint) ? HTCLIENT : HTBORDER;
        } else {
            nHit = HTCLIENT;
        }
    }
    if (lprcBounds != nullptr && pPoint != nullptr) {
        pPoint->x -= lprcBounds->left + dx;
        pPoint->y -= lprcBounds->top + dy;
    }
    return nHit;
}

// MAKELONG for the two 16-bit halves the retail packs into an LPARAM, then
// sign-extended to 64 bits exactly as retail's `movslq` does.
LPARAM PackLParam(long lo, long hi)
{
    const unsigned int v = (static_cast<unsigned int>(lo) & 0xFFFFu) |
                           ((static_cast<unsigned int>(hi) & 0xFFFFu) << 16);
    return static_cast<LPARAM>(static_cast<long>(v));
}

}  // namespace





// COleControl::AmbientScaleUnits() — retail (0x1ee9e0):
//     _AtlInstallStringThunk; *pRet = nil-string
//     GetAmbientProperty(DISPID_AMBIENT_SCALEUNITS, VT_BSTR, pRet)
//     return pRet
// RDX carries the hidden return slot for the by-value CString; the retail
// writes the result straight into it via the ambient lookup. The OpenMFC C++
// member AmbientScaleUnits() performs exactly that lookup.
// Symbol: ?AmbientScaleUnits@COleControl@@QEAA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@XZ
extern "C" void* MS_ABI
impl__AmbientScaleUnits_COleControl__QEAA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__XZ(
    COleControl* pThis, void* pRet)
{
    if (!pThis || !pRet) return nullptr;
    new (pRet) CString(pThis->AmbientScaleUnits());
    return pRet;
}
// COleControl::DoClick() — retail (0x1e1b50) dispatches through the virtual
// OnClick(1) (vtable slot 0x340); MFC's OnClick simply fires the Click event.
// The header does not declare OnClick, so the event fire is done directly.
// Symbol: ?DoClick@COleControl@@QEAAXXZ
extern "C" void MS_ABI impl__DoClick_COleControl__QEAAXXZ(COleControl* pThis)
{
    if (!pThis) return;
    pThis->FireClick();
}
// COleControl::FireError(SCODE, LPCTSTR, UINT) — retail (0x1e4420) fires the
// stock Error event (DISPID_ERROREVENT) after resolving the description /
// help-file strings. Only the four documented Error-event parameters are
// carried by OpenMFC's FireEvent (retail additionally pushes internal help
// strings and a retval slot that the event contract does not expose).
// Symbol: ?FireError@COleControl@@QEAAXJPEB_WI@Z
extern "C" void MS_ABI impl__FireError_COleControl__QEAAXJPEB_WI_Z(
    COleControl* pThis, long scode, const wchar_t* lpszDescription, unsigned int nHelpID)
{
    if (!pThis) return;
    // TODO(clean-room): transcribed partially.
    BYTE params[] = { VT_I4, VT_BSTR, VT_BSTR, VT_I4, 0 };
    pThis->FireEvent(DISPID_ERROREVENT, params,
                     scode,
                     lpszDescription ? lpszDescription : L"",
                     static_cast<const wchar_t*>(nullptr),
                     static_cast<long>(nHelpID));
}
// COleControl::ButtonDown(USHORT, UINT, CPoint) — retail (0x1e3dd0):
//     if (button-message || m_bPendingUIActivation) {
//         if (m_iButtonState == 0) SetCapture();
//         m_iButtonState = nButton;                // bits 4-6 of the 0x160 word
//         if (message & 0x20) FireEvent(MOUSEDOWN, nButton, shift, x, y);
//         m_iDblClkState &= ~nButton;              // bits 7-9 cleared
//     }
// The message-gating (which the retail derives from the current MSG) is not
// reproducible without the message window; firing is what MFC documents.
// Symbol: ?ButtonDown@COleControl@@IEAAXGIVCPoint@@@Z
extern "C" void MS_ABI impl__ButtonDown_COleControl__IEAAXGIVCPoint___Z(
    COleControl* pThis, unsigned short nButton, unsigned int nShiftState,
    unsigned long long pt)
{
    if (!pThis) return;
    if (pThis->m_iButtonState == 0) pThis->SetCapture();
    pThis->m_iButtonState = nButton & 7;
    pThis->m_iDblClkState &= ~(nButton & 7);
    pThis->FireMouseDown((short)nButton, (short)nShiftState, PtX(pt), PtY(pt));
}
// COleControl::ButtonDblClk(USHORT, UINT, CPoint) — retail (0x1e3fd0):
//     if (button-message) {
//         SetCapture();
//         m_iButtonState = nButton;
//         if (message & 0x2) FireEvent(DISPID_DBLCLICK, no params);
//         m_iDblClkState = nButton;                // bits 7-9
//     }
// Symbol: ?ButtonDblClk@COleControl@@IEAAXGIVCPoint@@@Z
extern "C" void MS_ABI impl__ButtonDblClk_COleControl__IEAAXGIVCPoint___Z(
    COleControl* pThis, unsigned short nButton, unsigned int /*nShiftState*/,
    unsigned long long /*pt*/)
{
    if (!pThis) return;
    pThis->SetCapture();
    pThis->m_iButtonState = nButton & 7;
    pThis->FireDblClick();
    pThis->m_iDblClkState = nButton & 7;
}
// COleControl::ButtonUp(USHORT, UINT, CPoint) — retail (0x1e3e90):
//     if (m_iButtonState == 0) return;
//     m_iButtonState &= ~nButton;
//     if (m_iButtonState == 0) ReleaseCapture();
//     if (message & 0x80) FireEvent(MOUSEUP, nButton, shift, x, y);
//     if ((message & 0x1) && !(m_iDblClkState & nButton) && PtInRect(rc, pt))
//         OnClick(nButton);                        // fires DISPID_CLICK
//     m_iDblClkState &= ~nButton;
// The retail's rect-inside test uses a virtual (vtable slot 0x2d8) that the
// headers do not model; the click fire is kept unconditionally.
// Symbol: ?ButtonUp@COleControl@@IEAAXGIVCPoint@@@Z
extern "C" void MS_ABI impl__ButtonUp_COleControl__IEAAXGIVCPoint___Z(
    COleControl* pThis, unsigned short nButton, unsigned int nShiftState,
    unsigned long long pt)
{
    if (!pThis) return;
    if (pThis->m_iButtonState == 0) return;
    pThis->m_iButtonState &= ~(nButton & 7);
    if (pThis->m_iButtonState == 0) pThis->ReleaseCapture();
    pThis->FireMouseUp((short)nButton, (short)nShiftState, PtX(pt), PtY(pt));
    if ((pThis->m_iDblClkState & (nButton & 7)) == 0) {
        pThis->FireClick();
    }
    pThis->m_iDblClkState &= ~(nButton & 7);
}
// COleControl::GetClientOffset(long*, long*) const — retail (0x1e6060):
//     n = (m_sBorderStyle == 1) + (m_sAppearance == 1 ? 2 : 0);
//     if (!n) { *pDx = 0; *pDy = 0; }
//     else { *pDx = GetSystemMetrics(SM_CXBORDER) * n;
//            *pDy = GetSystemMetrics(SM_CYBORDER) * n; }
// Symbol: ?GetClientOffset@COleControl@@UEBAXPEAJ0@Z
extern "C" void MS_ABI impl__GetClientOffset_COleControl__UEBAXPEAJ0_Z(
    const COleControl* pThis, long* pDx, long* pDy)
{
    if (!pThis || !pDx || !pDy) return;
    const long n = (pThis->m_sBorderStyle == 1) + (pThis->m_sAppearance == 1 ? 2 : 0);
    if (n != 0) {
        *pDx = ::GetSystemMetrics(SM_CXBORDER) * n;
        *pDy = ::GetSystemMetrics(SM_CYBORDER) * n;
    } else {
        *pDx = 0;
        *pDy = 0;
    }
}
// COleControl::ClientToParent(LPCRECT, LPPOINT) const — retail (0x1e61c0):
//     GetClientOffset(&offX, &offY);               // virtual slot 0x4a8
//     pt->x += lpRect->left + offX;
//     pt->y += lpRect->top  + offY;
// Symbol: ?ClientToParent@COleControl@@UEBAXPEBUtagRECT@@PEAUtagPOINT@@@Z
extern "C" void MS_ABI impl__ClientToParent_COleControl__UEBAXPEBUtagRECT__PEAUtagPOINT___Z(
    const COleControl* pThis, const RECT* lpRect, POINT* lpPoint)
{
    if (!pThis || !lpRect || !lpPoint) return;
    long offX = 0, offY = 0;
    // Retail dispatches through the virtual GetClientOffset; the base-class
    // implementation (this file) is the OpenMFC one.
    impl__GetClientOffset_COleControl__UEBAXPEAJ0_Z(pThis, &offX, &offY);
    lpPoint->x += lpRect->left + offX;
    lpPoint->y += lpRect->top + offY;
}
// COleControl::GetClientRect(LPRECT) const — retail (0x1e6c00):
//     if (m_bInPlaceActive && m_bInPlaceSiteWndless) {
//         GetClientOffset(&offX, &offY);           // virtual slot 0x4a8
//         *lpRect = m_rcPos;
//         OffsetRect(lpRect, -offX, -offY);
//         OffsetRect(lpRect, -lpRect->left, -lpRect->top);   // origin (0,0)
//     } else if (m_hWnd) GetClientRect(m_hWnd, lpRect);
//     else SetRectEmpty(lpRect);
// The two offsets cancel out of the origin normalization, leaving the control's
// own (0,0,width,height) client rect.
// Symbol: ?GetClientRect@COleControl@@UEBAXPEAUtagRECT@@@Z
extern "C" void MS_ABI impl__GetClientRect_COleControl__UEBAXPEAUtagRECT___Z(
    const COleControl* pThis, RECT* lpRect)
{
    if (!pThis || !lpRect) return;
    if (pThis->m_bInPlaceActive && pThis->m_bInPlaceSiteWndless) {
        long offX = 0, offY = 0;
        impl__GetClientOffset_COleControl__UEBAXPEAJ0_Z(pThis, &offX, &offY);
        RECT rc = { pThis->m_rcPos.left, pThis->m_rcPos.top,
                    pThis->m_rcPos.right, pThis->m_rcPos.bottom };
        ::OffsetRect(&rc, -offX, -offY);
        ::OffsetRect(&rc, -rc.left, -rc.top);
        *lpRect = rc;
    } else if (pThis->m_hWnd) {
        ::GetClientRect(pThis->m_hWnd, lpRect);
    } else {
        ::SetRectEmpty(lpRect);
    }
}
// COleControl::ClipCaretRect(LPRECT) — retail (0x1e6b20):
//     if (!(m_bInPlaceActive && m_bInPlaceSiteWndless)) return FALSE;
//     GetClippingRect(&m_rcPos, &clip);            // virtual slot 0x4b8
//     rect = *lpRect; OffsetRect(rect, -rect.left, -rect.top);
//     hr = m_pInPlaceSite->OnDefWindowMessage(WM_CARET_CLIPRECT, 0, (LPARAM)&rect, 0);
//     if (SUCCEEDED(hr)) { OffsetRect(rect, -rect.left, -rect.top); *lpRect = rect; }
//     return SUCCEEDED(hr);
// Symbol: ?ClipCaretRect@COleControl@@QEAAHPEAUtagRECT@@@Z
extern "C" int MS_ABI impl__ClipCaretRect_COleControl__QEAAHPEAUtagRECT___Z(
    COleControl* pThis, RECT* lpRect)
{
    if (!pThis || !lpRect) return FALSE;
    if (!(pThis->m_bInPlaceActive && pThis->m_bInPlaceSiteWndless)) return FALSE;
    // TODO(clean-room): transcribed partially. The container's windowless site
    // must clip the caret via the private WM_CARET_CLIPRECT path; that site
    // vtable is not modeled in OpenMFC.
    return FALSE;
}
// COleControl::GetCapture() — retail (0x1e6630):
//     if (m_bInPlaceActive && m_bInPlaceSiteWndless)
//         return m_pInPlaceSite->GetCapture() == S_OK ? this : NULL;
//     return CWnd::FromHandle(::GetCapture());
// (IOleInPlaceSiteWindowless::GetCapture takes no arguments and returns S_OK
// when the control holds the capture.)
// Symbol: ?GetCapture@COleControl@@QEAAPEAVCWnd@@XZ
extern "C" CWnd* MS_ABI impl__GetCapture_COleControl__QEAAPEAVCWnd__XZ(COleControl* pThis)
{
    if (!pThis) return nullptr;
    if (pThis->m_bInPlaceActive && pThis->m_bInPlaceSiteWndless) {
        void* pSite = pThis->m_pInPlaceSite;
        if (!pSite) return nullptr;
        // IOleInPlaceSiteWindowless::GetCapture is slot 19 of the site vtable.
        void** vft = *static_cast<void***>(pSite);
        auto* fn = reinterpret_cast<HRESULT (STDMETHODCALLTYPE*)(void*)>(vft[19]);
        return fn(pSite) == S_OK ? pThis : nullptr;
    }
    return impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(::GetCapture());
}
// COleControl::DefWindowProcW — retail (0x1e2130):
//     if (m_hWnd) goto CWnd::DefWindowProcW;       // shared implementation
//     return 0;
// Symbol: ?DefWindowProcW@COleControl@@MEAA_JI_K_J@Z
extern "C" LRESULT MS_ABI impl__DefWindowProcW_COleControl__MEAA_JI_K_J_Z(
    COleControl* pThis, UINT message, WPARAM wParam, LPARAM lParam)
{
    if (!pThis || !pThis->m_hWnd) return 0;
    return impl__DefWindowProcW_CWnd__MEAA_JI_K_J_Z(pThis, message, wParam, lParam);
}
// COleControl::ForwardActivationMsg(MSG*) — retail (0x1e51f0):
//     if (m_bInPlaceSiteWndless && message in 0x200..0x209)
//         translate via the wndless site, then dispatch to a virtual (slot 0x498);
//     else ::SendMessageW(m_hWnd, msg, wParam, lParam);
// Symbol: ?ForwardActivationMsg@COleControl@@IEAAXPEAUtagMSG@@@Z
extern "C" void MS_ABI impl__ForwardActivationMsg_COleControl__IEAAXPEAUtagMSG___Z(
    COleControl* pThis, MSG* pMsg)
{
    if (!pThis || !pMsg) return;
    // TODO(clean-room): transcribed partially. The windowless translation path
    // (site GetWindow + OnDefWindowMessage + virtual dispatch) is not modeled;
    // the windowed path forwards the raw message, which is what a non-windowless
    // control observes in retail.
    if (pThis->m_hWnd) {
        ::SendMessageW(pThis->m_hWnd, pMsg->message, pMsg->wParam, pMsg->lParam);
    }
}

// COleControl::CreateWindowForSubclassedControl() — retail mfc140u 0x1e1cd0:
//     if (!<virtual slot 0x468>()) return;
//     if (m_hWnd) return;                      // cmpq $0,0x40(this)
//     HWND hWndParent = <unexported helper, u 0x1df440>();
//     if (hWndParent == NULL) return;
//     SIZE szPixels;
//     <unexported helper, u 0x26080c>(&{m_cxExtent, m_cyExtent}, &szPixels);
//     RECT rc = { 0, 0, szPixels.cx, szPixels.cy };
//     <virtual slot 0x4c0>(hWndParent, &rc, NULL);
// Slot 0x468 is a no-argument BOOL virtual; InternalGetText (0x1ede80) and
// OnGetDlgCode (0x1e3b10) gate on the very same slot, which is what MFC
// documents as IsSubclassedControl().  (An older revision of this comment named
// it PreCreateWindow(); nothing in the three call sites supports that.)
// The real afxctl.h on this host settles it independently: COleControl declares
// IsSubclassedControl, ReparentControlWindow and GetControlFlags consecutively,
// and the retail slots 0x468 / 0x470 / 0x478 are consecutive too, with 0x478
// pinned to GetControlFlags by GetInterfaceHook (this file).  OnMove (this
// file) gates on 0x468 as well.
// The two helpers, both read out of the retail bodies rather than guessed:
//   u 0x1df440 -- returns an HWND, not a class name.  It caches a hidden
//     module-wide parent window in AfxGetThreadState()->+0x1d0: on first call it
//     allocates a 0x120-byte CWnd (ctor u 0x28a700), calls
//     AfxEndDeferRegisterClass(0x20) and CWnd::CreateEx(u 0x28b500) with
//     dwExStyle 0x84, class name L"AfxOleControl140u" (rdata 0x34b770), style
//     WS_CHILD|WS_VISIBLE (0x50000000) at (-1000,-1000) size 1x1 parented to
//     GetDesktopWindow(); it then returns that CWnd's m_hWnd.
//   u 0x26080c -- HIMETRIC to pixels: MulDiv(<screen dpi at rdata 0x3c32d8 /
//     0x3c32dc>, extent, 2540) for each axis.
// Slot 0x4c0 takes (HWND, const CRect&, LPCRECT), i.e. it is
// COleControl::CreateControlWindow -- the export with that signature is defined
// above in this file.
// Symbol: ?CreateWindowForSubclassedControl@COleControl@@IEAAXXZ
extern "C" void MS_ABI impl__CreateWindowForSubclassedControl_COleControl__IEAAXXZ(
    COleControl* /*pThis*/)
{
    // TODO(clean-room): left a stub. The blocking piece is the hidden
    // "AfxOleControl140u" parent window above: OpenMFC registers no such class
    // and keeps no such per-thread-state slot, and CreateControlWindow (this
    // file) is itself a stub that only reports an already-attached m_hWnd. A
    // no-op is type-correct (void) and leaves m_hWnd untouched.
}
// COleControl::DoSuperclassPaint(CDC*, const CRect&) — retail (0x1dfd70):
//     if (!m_hWnd) CreateWindowForSubclassedControl();
//     if (!m_hWnd) return;
//     ... compare control rect with window rect, then paint through the
//     superclass WNDPROC via CallWindowProcW ...
// Symbol: ?DoSuperclassPaint@COleControl@@QEAAXPEAVCDC@@AEBVCRect@@@Z
extern "C" void MS_ABI impl__DoSuperclassPaint_COleControl__QEAAXPEAVCDC__AEBVCRect___Z(
    COleControl* pThis, CDC* pDC, const CRect& /*rcBounds*/)
{
    if (!pThis) return;
    if (!pThis->m_hWnd) {
        impl__CreateWindowForSubclassedControl_COleControl__IEAAXXZ(pThis);
    }
    // TODO(clean-room): transcribed partially. Painting through the superclass
    // window proc (CallWindowProcW) is not reproducible without a registered
    // superclass WNDPROC.
    (void)pDC;
}
// COleControl::CreateFrameWindow() — retail (0x1e0f80):
//     wnd = new CControlFrameWnd(this);            // 0xf0 bytes, ctor 0x1802b29d0
//     if (wnd && wnd->Create(...)) return wnd;     // virtual slot 0x2d8
//     return NULL;
// The OpenMFC CControlFrameWnd ctor stub installs no vtable, so running the
// virtual Create would read uninitialized memory; report failure instead.
// Symbol: ?CreateFrameWindow@COleControl@@MEAAPEAVCControlFrameWnd@@XZ
extern "C" void* MS_ABI impl__CreateFrameWindow_COleControl__MEAAPEAVCControlFrameWnd__XZ(
    COleControl* pThis)
{
    if (!pThis) return nullptr;
    // TODO(clean-room): transcribed partially.
    return nullptr;
}
// COleControl::CreateControlWindow(HWND, const CRect&, LPCRECT) — retail
// (0x1dff10) computes the window style from m_sBorderStyle/m_sAppearance/
// m_bEnabled, creates a CControlFrameWnd (or reuses m_pReflect), creates the
// HWND and reports m_hWnd != 0.
// Symbol: ?CreateControlWindow@COleControl@@MEAAHPEAUHWND__@@AEBVCRect@@PEBUtagRECT@@@Z
extern "C" int MS_ABI impl__CreateControlWindow_COleControl__MEAAHPEAUHWND____AEBVCRect__PEBUtagRECT___Z(
    COleControl* pThis, HWND /*hwndParent*/, const CRect& /*rcPos*/, const RECT* /*lpRect*/)
{
    if (!pThis) return FALSE;
    // TODO(clean-room): transcribed partially. The frame-window creation path
    // is not implemented (see CreateFrameWindow); a window already attached is
    // the only success case.
    return pThis->m_hWnd ? TRUE : FALSE;
}
// COleControl::DrawContent(CDC*, CRect&) — retail (0x1e0420):
//     if (!m_bOptimizedDraw) nSaveDC = pDC->SaveDC();
//     ... SetBkMode(TRANSPARENT) ...; m_rcBounds = rc;
//     OnDraw(pDC, rc, rc);                         // virtual slot 0x2f0
//     if (!m_bOptimizedDraw && nSaveDC) pDC->RestoreDC(nSaveDC);
// Symbol: ?DrawContent@COleControl@@IEAAXPEAVCDC@@AEAVCRect@@@Z
extern "C" void MS_ABI impl__DrawContent_COleControl__IEAAXPEAVCDC__AEAVCRect___Z(
    COleControl* pThis, CDC* pDC, CRect& rc)
{
    if (!pThis || !pDC) return;
    pThis->m_rcBounds = rc;
    pThis->OnDraw(pDC, rc, rc);
}
// COleControl::DrawMetafile(CDC*, CRect&) — retail (0x1e0550):
//     m_rcBounds = rc;  OnDrawMetafile(pDC, rc);   // virtual slot 0x2f8
// Symbol: ?DrawMetafile@COleControl@@IEAAXPEAVCDC@@AEAVCRect@@@Z
extern "C" void MS_ABI impl__DrawMetafile_COleControl__IEAAXPEAVCDC__AEAVCRect___Z(
    COleControl* pThis, CDC* pDC, CRect& rc)
{
    if (!pThis || !pDC) return;
    pThis->m_rcBounds = rc;
    // TODO(clean-room): transcribed partially. Retail dispatches to the virtual
    // OnDrawMetafile which the header does not model; fall back to OnDraw so the
    // control still renders.
    pThis->OnDraw(pDC, rc, rc);
}
// COleControl::ExchangeExtent(CPropExchange*) — retail (0x1ef880) exchanges
// m_cxExtent/m_cyExtent with the "Extent" properties through the PX_* helpers
// and reports changes to IOleObject::SetExtent(DVASPECT_CONTENT, size).
// Symbol: ?ExchangeExtent@COleControl@@QEAAHPEAVCPropExchange@@@Z
extern "C" int MS_ABI impl__ExchangeExtent_COleControl__QEAAHPEAVCPropExchange___Z(
    COleControl* pThis, CPropExchange* pPX)
{
    if (!pThis || !pPX) return FALSE;
    // TODO(clean-room): transcribed partially. The PX_* exchange helpers are
    // not exported by OpenMFC; report a successful (no-op) exchange.
    return TRUE;
}

// COleControl::ExchangeStockProps(CPropExchange* pPX) — retail mfc140u 0x1ecfa0
// (disassembled at the identical mfc140 0x1ead40).  Verified from that body:
// it reads the exchange direction (m_bLoading) from pPX+0x8, obtains the
// dispatch map through virtual slot 0x70 and loads the stock-property mask as
// *(DWORD*)pMap->+0x18 -- the very word InitStockPropMask below computes -- then
// runs one PX_* call per set mask bit against the section named "_StockProps".
// The property names it passes are, in the order the code tests them,
// "Caption"/"Text" (mask bits 0x02|0x10, backed by InternalGetText/SetText),
// "ForeColor" (bit 0x08, member 0x168), "BackColor" (bit 0x01, member 0x164),
// then further bits for "Font", "BorderStyle", "Enabled" and "Appearance".
// Those names are one packed run of UTF-16 literals at mfc140u rdata 0x34baa0
// (which starts with the "_StockProps" section name itself); the run continues
// past "Appearance" into the data-path property names, so the list above is the
// start of the block, not proof that it ends there.
// Symbol: ?ExchangeStockProps@COleControl@@QEAAXPEAVCPropExchange@@@Z
extern "C" void MS_ABI impl__ExchangeStockProps_COleControl__QEAAXPEAVCPropExchange___Z(
    COleControl* /*pThis*/, CPropExchange* /*pPX*/)
{
    // TODO(clean-room): transcribed partially. The per-property PX_* helpers and
    // the stock-property mask (see InitStockPropMask below, which is itself a
    // stub) are not modeled; a no-op is type-correct (void).
}
// COleControl::BuildSharedMenu() — retail (0x1e52f0) allocates a 0x28-byte
// OLE menu-holder at m_pUIActiveInfo and shares the in-place frame's menu into
// it. The holder type is not modeled in afxole.h, so only the already-built
// case can be reproduced.
// Symbol: ?BuildSharedMenu@COleControl@@MEAAHXZ
extern "C" int MS_ABI impl__BuildSharedMenu_COleControl__MEAAHXZ(COleControl* pThis)
{
    if (!pThis) return FALSE;
    // TODO(clean-room): transcribed partially.
    return pThis->m_pUIActiveInfo != nullptr ? TRUE : FALSE;
}
// COleControl::DestroySharedMenu() — retail (0x1e5430) releases the shared
// menu held by the m_pUIActiveInfo holder and frees the 0x28-byte object.
// Symbol: ?DestroySharedMenu@COleControl@@MEAAXXZ
extern "C" void MS_ABI impl__DestroySharedMenu_COleControl__MEAAXXZ(COleControl* pThis)
{
    if (!pThis) return;
    // TODO(clean-room): transcribed partially. The holder object was never
    // allocated by the OpenMFC BuildSharedMenu, so dropping the pointer is the
    // symmetric no-op.
    pThis->m_pUIActiveInfo = nullptr;
}
// COleControl::ControlInfoChanged() — retail (0x1e1700):
//     if (m_pControlSite) m_pControlSite->OnControlInfoChanged();
// (OnControlInfoChanged is slot 3 of the IOleControlSite vtable.)
// Symbol: ?ControlInfoChanged@COleControl@@QEAAXXZ
extern "C" void MS_ABI impl__ControlInfoChanged_COleControl__QEAAXXZ(COleControl* pThis)
{
    if (!pThis) return;
    void* pSite = pThis->m_pControlSite;
    if (!pSite) return;
    void** vft = *static_cast<void***>(pSite);
    auto* fn = reinterpret_cast<HRESULT (STDMETHODCALLTYPE*)(void*)>(vft[3]);
    fn(pSite);
}

// COleControl::DisplayError(SCODE, LPCTSTR lpszDescription, LPCTSTR lpszSource,
// LPCTSTR lpszHelpFile, UINT nHelpID) — retail mfc140u 0x1e45b0:
//     if (lpszSource == NULL)
//         lpszSource = AfxGetModuleState()->+0x20;   // = AFX_MODULE_STATE::
//                                                    // m_lpszCurrentAppName;
//                                                    // +0x10/+0x18/+0x20 are
//                                                    // written by CWinApp::
//                                                    // SetCurrentHandles,
//                                                    // u 0x1ce190
//     ::MessageBox(this ? m_hWnd : NULL, lpszDescription, lpszSource, MB_OK);
// (tail jump to the MessageBoxW import; scode, lpszHelpFile and nHelpID are not
// touched by this body at all).
// Symbol: ?DisplayError@COleControl@@UEAAXJPEB_W00I@Z
extern "C" void MS_ABI impl__DisplayError_COleControl__UEAAXJPEB_W00I_Z(
    COleControl* pThis, long /*scode*/, const wchar_t* lpszDescription,
    const wchar_t* lpszSource, const wchar_t* /*lpszHelpFile*/,
    unsigned int /*nHelpID*/)
{
    if (lpszSource == nullptr) {
        // Retail reads AFX_MODULE_STATE::m_lpszCurrentAppName (+0x20).
        // CWinApp::SetCurrentHandles (mfc140u 0x1ce190) is what fills that
        // field, with `mov 0xa0(rbx),rax ; mov rax,0x20(rsi)` -- i.e. it is a
        // copy of CWinApp::m_pszAppName, which OpenMFC does keep. If there is
        // no app object the caption stays NULL and MessageBoxW substitutes its
        // own default title.
        CWinApp* pApp = static_cast<CWinApp*>(
            impl__AfxGetThread__YAPEAVCWinThread__XZ());
        if (pApp != nullptr) lpszSource = pApp->m_pszAppName;
    }
    ::MessageBoxW(pThis ? pThis->m_hWnd : nullptr, lpszDescription, lpszSource,
                  MB_OK);
}
// COleControl::CreateTracker(int, int) — retail (0x1f5510) zeroes the RECT
// pointer and tail-calls the 3-argument form (default rect = m_rcPos).
// Symbol: ?CreateTracker@COleControl@@IEAAXHH@Z
extern "C" void MS_ABI impl__CreateTracker_COleControl__IEAAXHH_Z(
    COleControl* pThis, int nSize, int bEnableResize)
{
    if (!pThis) return;
    impl__CreateTracker_COleControl__IEAAXHHPEBUtagRECT___Z(pThis, nSize, bEnableResize, nullptr);
}
// COleControl::CreateTracker(int, int, LPCRECT) — retail (0x1f5520):
//     style  = (nSize ? 0x10 : 0x0) | (bEnableResize ? 0x4 : 0x0);
//     rect   = m_rcPos (adjusted by an internal helper);
//     m_pRectTracker = new CRectTracker(rect, style);   // 0x60-byte object
//     if (lpRect) CopyRect(&tracker->tail, lpRect);
//     OnSetObjectRects(&m_rcPos, lpRect ? lpRect : &m_rcPos);   // virtual slot 0x330
// Symbol: ?CreateTracker@COleControl@@IEAAXHHPEBUtagRECT@@@Z
extern "C" void MS_ABI impl__CreateTracker_COleControl__IEAAXHHPEBUtagRECT___Z(
    COleControl* pThis, int nSize, int bEnableResize, const RECT* lpRect)
{
    if (!pThis) return;
    const unsigned int style = (nSize != 0 ? 0x10u : 0x0u) |
                               (bEnableResize != 0 ? 0x4u : 0x0u);
    void* pTracker = ::operator new(0x60);
    if (pTracker) {
        RECT rc = { pThis->m_rcPos.left, pThis->m_rcPos.top,
                    pThis->m_rcPos.right, pThis->m_rcPos.bottom };
        impl___0CRectTracker__QEAA_PEBUtagRECT__I_Z(pTracker, &rc, style);
    }
    pThis->m_pRectTracker = pTracker;
    const RECT rcPos = { pThis->m_rcPos.left, pThis->m_rcPos.top,
                         pThis->m_rcPos.right, pThis->m_rcPos.bottom };
    const RECT* notifyRect = lpRect ? lpRect : &rcPos;
    pThis->OnSetObjectRects(&rcPos, notifyRect);
}
// COleControl::DestroyTracker() — retail (0x1f5620):
//     if (m_pRectTracker) { delete m_pRectTracker; m_pRectTracker = NULL; }
//     OnSetObjectRects(&m_rcPos, &tracker-tail-state);   // virtual slot 0x330
// Symbol: ?DestroyTracker@COleControl@@IEAAXXZ
extern "C" void MS_ABI impl__DestroyTracker_COleControl__IEAAXXZ(COleControl* pThis)
{
    if (!pThis) return;
    void* pTracker = pThis->m_pRectTracker;
    if (pTracker) {
        impl___1CRectTracker__UEAA_XZ(pTracker);
        ::operator delete(pTracker);
    }
    pThis->m_pRectTracker = nullptr;
    // TODO(clean-room): retail forwards the tracker's tail rect state here;
    // m_rcPos is the closest named equivalent.
    const RECT rcPos = { pThis->m_rcPos.left, pThis->m_rcPos.top,
                         pThis->m_rcPos.right, pThis->m_rcPos.bottom };
    pThis->OnSetObjectRects(&rcPos, &rcPos);
}

// COleControl::GetConnectionMap() const — retail mfc140u 0x1deff0 is
//     lea rax,[rip+0x144301] ; ret        -> &<static map at rva 0x3232f8>
// (COleControl::GetThisConnectionMap shares that exact body.)  The map is NOT
// entry-less, contrary to what this comment used to claim: its first field is a
// pfnGetBaseMap at 0x1de8e0, its second points at an entry table at rva
// 0x34b5d0 whose single live entry is { piid = 0x2d9d28, nOffset = 0x330 } —
// the property-notify connection point sub-object — followed by the
// { NULL, -1 } terminator.
// Symbol: ?GetConnectionMap@COleControl@@MEBAPEBUAFX_CONNECTIONMAP@@XZ
extern "C" const void* MS_ABI impl__GetConnectionMap_COleControl__MEBAPEBUAFX_CONNECTIONMAP__XZ(
    const COleControl* /*pThis*/)
{
    // The map itself is now reproduced byte-for-byte from retail .rdata (see
    // g_connectionMap_COleControl at the top of this file): base map getter =
    // CCmdTarget::GetThisConnectionMap, one live entry
    // { &IID_IPropertyNotifySink, offsetof(COleControl, m_xPropConnPt) = 0x330 },
    // then the { NULL, (size_t)-1 } terminator.
    //
    // Remaining deviation, and it is the important one: OpenMFC never
    // constructs the connection-point sub-object that lives at that offset, so
    // a caller that walks this map and treats this+0x330 as a CConnectionPoint
    // will read uninitialised storage.  Handing back the correct *map* is still
    // strictly better than NULL, because a derived control's own connection map
    // chains into this one and a NULL here breaks the walk outright.
    return &g_connectionMap_COleControl;
}
// COleControl::GetConnectionHook(const IID&) — retail (0x1df000):
//     if (!m_piidEvents) return NULL;
//     return memcmp(piid, m_piidEvents, 16) == 0 ? &m_xPropConnPt : NULL;
// Symbol: ?GetConnectionHook@COleControl@@MEAAPEAUIConnectionPoint@@AEBU_GUID@@@Z
extern "C" IConnectionPoint* MS_ABI
impl__GetConnectionHook_COleControl__MEAAPEAUIConnectionPoint__AEBU_GUID___Z(
    COleControl* pThis, const GUID& iid)
{
    if (!pThis || !pThis->m_piidEvents) return nullptr;
    if (::IsEqualGUID(iid, *pThis->m_piidEvents)) {
        // Retail returns &this + 0x328; its XPropConnPt object starts 8 bytes
        // before the header's m_xPropConnPt slot (the header models both
        // connection-point objects as 96-byte arrays). The named member is the
        // closest model.
        return reinterpret_cast<IConnectionPoint*>(&pThis->m_xPropConnPt);
    }
    return nullptr;
}
// COleControl::KeyDown(USHORT*) — retail mfc140u 0x1e3a00 (disassembled at the
// identical mfc140 0x1e1920):
//     const AFX_EVENTMAP* pMap = GetEventMap();          // virtual slot 0x4d0
//     if (*(const BYTE*)pMap->lpStockEventMask /* +0x10 */ & 0x04) {
//         short nShiftState = GetShiftState();           // helper 0x1e1670
//         FireEvent(DISPID_KEYDOWN /* -602 */, params, pnChar, nShiftState);
//         if (*pnChar)
//             OnKeyDownEvent(*pnChar, nShiftState);      // vtable slot 0x348,
//                                                        // both args by VALUE
//     }
// Slot 0x348 is ?OnKeyDownEvent@COleControl@@UEAAXGG@Z (two USHORTs), NOT
// ?OnKeyDown@COleControl@@IEAAXIII@Z -- the latter is the WM_KEYDOWN message
// handler at mfc140 0x1e17b0, which takes three UINTs and is implemented
// further down this file.  (An earlier revision of this comment named it
// "OnKeyDown" and showed it taking the pointer; the binary passes the
// dereferenced 16-bit value in EDX.)
// The stock-event-mask gate cannot be evaluated here (AFX_EVENTMAP is not
// modeled and GetEventMap in this file returns null), so the fire is
// unconditional, matching the DoClick precedent above.
// Symbol: ?KeyDown@COleControl@@IEAAXPEAG@Z
extern "C" void MS_ABI impl__KeyDown_COleControl__IEAAXPEAG_Z(
    COleControl* pThis, unsigned short* pnChar)
{
    if (!pThis || !pnChar) return;
    pThis->FireKeyDown(pnChar, GetShiftState());
    // TODO(clean-room): retail also forwards to the virtual OnKeyDownEvent
    // (vtable slot 0x348) when *pnChar != 0; not modeled in the header.
}
// COleControl::KeyUp(USHORT*) — retail mfc140u 0x1e3970 (disassembled at the
// identical mfc140 0x1e1890): the mirror image of KeyDown, gated on stock-event
// mask bit 0x10, firing DISPID_KEYUP (-604) and then forwarding to virtual slot
// 0x350 = ?OnKeyUpEvent@COleControl@@UEAAXGG@Z with (*pnChar, nShiftState) by
// value.  (Slot 0x350 is OnKeyUpEvent, not ?OnKeyUp@COleControl@@IEAAXIII@Z,
// which is the WM_KEYUP message handler at mfc140 0x1e1820.)
// Symbol: ?KeyUp@COleControl@@IEAAXPEAG@Z
extern "C" void MS_ABI impl__KeyUp_COleControl__IEAAXPEAG_Z(
    COleControl* pThis, unsigned short* pnChar)
{
    if (!pThis || !pnChar) return;
    pThis->FireKeyUp(pnChar, GetShiftState());
    // TODO(clean-room): retail also forwards to the virtual OnKeyUpEvent
    // (vtable slot 0x350) when *pnChar != 0; not modeled in the header.
}
// COleControl::OnChar(UINT, UINT, UINT) — retail mfc140u 0x1e3b70
// (disassembled at the identical mfc140 0x1e1a90):
//     BOOL bLead = IsDBCSLeadByte((BYTE)nChar);
//     USHORT ch  = (USHORT)nChar;
//     if (*(const BYTE*)GetEventMap()->lpStockEventMask & 0x08) { // slot 0x4d0
//         HWND hWnd = m_hWnd;
//         if (bLead && PeekMessage(&msg, m_hWnd, WM_CHAR, WM_CHAR, PM_NOYIELD))
//             ch = (nChar << 8) | LOWORD(msg.wParam);      // merge the pair
//         FireEvent(DISPID_KEYPRESS /* -603 */, params, &ch);
//         if (ch) OnKeyPressEvent(ch);                     // vtable slot 0x358,
//                                                          // the USHORT by VALUE
//         if (m_hWnd != hWnd) return;
//     }
//     if (ch != 0) {
//         ... DBCS re-split / lead-byte bookkeeping ...
//         DefWindowProc(WM_CHAR, ch, MAKELONG(nRepCnt, nFlags)); // slot 0x248
//     }
//     ... trailing lead-byte cleanup when IsDBCSLeadByte was true ...
// Two things this file does NOT reproduce and the comment above used to hide:
// (1) the stock-event-mask gate -- OpenMFC's GetEventMap returns null, so the
// fire below is unconditional, as for KeyDown; (2) everything after the fire,
// i.e. the DefWindowProc(WM_CHAR, ...) forwarding and its DBCS bookkeeping.
// Slot 0x358 is ?OnKeyPressEvent@COleControl@@UEAAXG@Z, not the WM_CHAR handler.
// Symbol: ?OnChar@COleControl@@IEAAXIII@Z
extern "C" void MS_ABI impl__OnChar_COleControl__IEAAXIII_Z(
    COleControl* pThis, unsigned int nChar, unsigned int nRepCnt,
    unsigned int nFlags)
{
    if (!pThis) return;
    (void)nRepCnt;
    (void)nFlags;
    unsigned short ch = static_cast<unsigned short>(nChar);
    if (::IsDBCSLeadByte(static_cast<BYTE>(nChar)) && pThis->m_hWnd) {
        MSG msg;
        if (::PeekMessageW(&msg, pThis->m_hWnd, WM_CHAR, WM_CHAR, PM_NOYIELD)) {
            ch = static_cast<unsigned short>((nChar << 8) |
                                             (msg.wParam & 0xFFFF));
        }
    }
    pThis->FireKeyPress(&ch);
    // TODO(clean-room): retail also forwards to the virtual OnKeyPressEvent
    // (vtable slot 0x358, the USHORT by value) when ch != 0, and then falls
    // through to DefWindowProc(WM_CHAR, ch, MAKELONG(nRepCnt, nFlags)); neither
    // is reproduced here (the slot is not modeled and the WM_CHAR forwarding
    // was never transcribed).
}
// COleControl::OnClick(USHORT) — retail (0x1e43e0) fires DISPID_CLICK
// (event-map gate omitted, as for DoClick).
// Symbol: ?OnClick@COleControl@@UEAAXG@Z
extern "C" void MS_ABI impl__OnClick_COleControl__UEAAXG_Z(
    COleControl* pThis, unsigned short /*nButton*/)
{
    if (!pThis) return;
    pThis->FireClick();
}
// COleControl::OnButtonDown(USHORT, UINT, CPoint) — retail (0x1e4280):
//     if (nButton == MK_LBUTTON) SetFocus();   // windowless-aware
//     ButtonDown(nButton, nShiftState, point);
//     if (m_hWnd unchanged) <message-map notify>
// Symbol: ?OnButtonDown@COleControl@@IEAAXGIVCPoint@@@Z
extern "C" void MS_ABI impl__OnButtonDown_COleControl__IEAAXGIVCPoint___Z(
    COleControl* pThis, unsigned short nButton, unsigned int nShiftState,
    unsigned long long pt)
{
    if (!pThis) return;
    if (nButton == 1 /* MK_LBUTTON */) {
        if (pThis->m_bInPlaceActive && pThis->m_bInPlaceSiteWndless) {
            void* pSite = pThis->m_pInPlaceSite;
            if (pSite) {
                // IOleInPlaceSiteWindowless::SetFocus(TRUE) is slot 22.
                void** vft = *static_cast<void***>(pSite);
                auto* fn = reinterpret_cast<HRESULT (STDMETHODCALLTYPE*)(void*, BOOL)>(vft[22]);
                fn(pSite, TRUE);
            }
        } else if (pThis->m_hWnd) {
            ::SetFocus(pThis->m_hWnd);
        }
    }
    impl__ButtonDown_COleControl__IEAAXGIVCPoint___Z(
        pThis, nButton, nShiftState, pt);
    // TODO(clean-room): retail notifies the message-map machinery when the
    // window is unchanged; not modeled.
}
// COleControl::OnButtonDblClk(USHORT, UINT, CPoint) — retail (0x1e4370):
//     ButtonDblClk(nButton, nShiftState, point);
//     if (m_hWnd unchanged) <message-map notify>
// Symbol: ?OnButtonDblClk@COleControl@@IEAAXGIVCPoint@@@Z
extern "C" void MS_ABI impl__OnButtonDblClk_COleControl__IEAAXGIVCPoint___Z(
    COleControl* pThis, unsigned short nButton, unsigned int nShiftState,
    unsigned long long pt)
{
    if (!pThis) return;
    impl__ButtonDblClk_COleControl__IEAAXGIVCPoint___Z(
        pThis, nButton, nShiftState, pt);
    // TODO(clean-room): retail notifies the message-map machinery when the
    // window is unchanged; not modeled.
}
// COleControl::OnButtonUp(USHORT, UINT, CPoint) — retail (0x1e42d0):
//     ButtonUp(nButton, nShiftState, point);
//     if (m_hWnd unchanged && (flags & 0x1c00) == 0x1400) {
//         // m_bInPlaceActive && !m_bUIActive && m_bPendingUIActivation
//         m_bPendingUIActivation = 0;
//         if (GetFocus() != m_hWnd && !IsChild(m_hWnd, GetFocus()))
//             <virtual SetFocus, vtable slot 0x508>;
//     }
// Symbol: ?OnButtonUp@COleControl@@IEAAXGIVCPoint@@@Z
extern "C" void MS_ABI impl__OnButtonUp_COleControl__IEAAXGIVCPoint___Z(
    COleControl* pThis, unsigned short nButton, unsigned int nShiftState,
    unsigned long long pt)
{
    if (!pThis) return;
    impl__ButtonUp_COleControl__IEAAXGIVCPoint___Z(
        pThis, nButton, nShiftState, pt);
    if (pThis->m_bInPlaceActive && !pThis->m_bUIActive &&
        pThis->m_bPendingUIActivation) {
        pThis->m_bPendingUIActivation = 0;
        HWND hFocus = ::GetFocus();
        if (hFocus != pThis->m_hWnd && !::IsChild(pThis->m_hWnd, hFocus)) {
            // TODO(clean-room): retail dispatches the windowless SetFocus
            // virtual (vtable slot 0x508); not modeled in the header.
        }
    }
}
// COleControl::OnCancelMode() — retail (0x1e43a0):
//     <message-map notify>
//     if (m_iButtonState || m_iDblClkState) {   // bits 4-9 of the 0x160 word
//         ReleaseCapture();                     // windowless-aware
//         m_iButtonState = 0;  m_iDblClkState = 0;
//     }
// Symbol: ?OnCancelMode@COleControl@@IEAAXXZ
extern "C" void MS_ABI impl__OnCancelMode_COleControl__IEAAXXZ(COleControl* pThis)
{
    if (!pThis) return;
    // TODO(clean-room): retail first forwards to the message-map machinery;
    // not modeled.
    if (pThis->m_iButtonState != 0 || pThis->m_iDblClkState != 0) {
        if (pThis->m_bInPlaceActive && pThis->m_bInPlaceSiteWndless) {
            void* pSite = pThis->m_pInPlaceSite;
            if (pSite) {
                // IOleInPlaceSiteWindowless::SetCapture(FALSE) is slot 20.
                void** vft = *static_cast<void***>(pSite);
                auto* fn = reinterpret_cast<HRESULT (STDMETHODCALLTYPE*)(void*, BOOL)>(vft[20]);
                fn(pSite, FALSE);
            }
        } else {
            ::ReleaseCapture();
        }
        pThis->m_iButtonState = 0;
        pThis->m_iDblClkState = 0;
    }
}
// COleControl::OnAppearanceChanged() — retail (0x1edd40) is
// `xor edx,edx; lea r8d,0x1(rdx); jmp <InvalidateControl>`, i.e. exactly
// InvalidateControl(NULL, TRUE).
// Symbol: ?OnAppearanceChanged@COleControl@@UEAAXXZ
extern "C" void MS_ABI impl__OnAppearanceChanged_COleControl__UEAAXXZ(
    COleControl* pThis)
{
    if (!pThis) return;
    pThis->InvalidateControl(nullptr, TRUE);
}
// COleControl::OnBackColorChanged() — retail alias of InvalidateControl
// (see file header).
// Symbol: ?OnBackColorChanged@COleControl@@UEAAXXZ
extern "C" void MS_ABI impl__OnBackColorChanged_COleControl__UEAAXXZ(
    COleControl* pThis)
{
    if (!pThis) return;
    pThis->InvalidateControl(nullptr, TRUE);
}
// COleControl::OnBorderStyleChanged() — retail alias of InvalidateControl.
// Symbol: ?OnBorderStyleChanged@COleControl@@UEAAXXZ
extern "C" void MS_ABI impl__OnBorderStyleChanged_COleControl__UEAAXXZ(
    COleControl* pThis)
{
    if (!pThis) return;
    pThis->InvalidateControl(nullptr, TRUE);
}
// COleControl::OnEnabledChanged() — retail alias of InvalidateControl.
// Symbol: ?OnEnabledChanged@COleControl@@UEAAXXZ
extern "C" void MS_ABI impl__OnEnabledChanged_COleControl__UEAAXXZ(
    COleControl* pThis)
{
    if (!pThis) return;
    pThis->InvalidateControl(nullptr, TRUE);
}
// COleControl::OnForeColorChanged() — retail alias of InvalidateControl.
// Symbol: ?OnForeColorChanged@COleControl@@UEAAXXZ
extern "C" void MS_ABI impl__OnForeColorChanged_COleControl__UEAAXXZ(
    COleControl* pThis)
{
    if (!pThis) return;
    pThis->InvalidateControl(nullptr, TRUE);
}
// COleControl::OnFontChanged() — retail (0x1ee280), when a window is
// attached, pushes the font holder's HFONT to the window with WM_SETFONT and
// remembers it in m_hFontPrev (resetting both when the holder has no font),
// then invalidates. The event-map gate and the PreCreateWindow check are not
// modeled.
// Symbol: ?OnFontChanged@COleControl@@UEAAXXZ
extern "C" void MS_ABI impl__OnFontChanged_COleControl__UEAAXXZ(
    COleControl* pThis)
{
    if (!pThis) return;
    if (pThis->m_hWnd) {
        if (pThis->m_font.m_pFont != nullptr) {
            HFONT hFont = impl__GetFontHandle_CFontHolder__QEAAPEAUHFONT____XZ(
                &pThis->m_font);
            ::SendMessageW(pThis->m_hWnd, WM_SETFONT,
                           reinterpret_cast<WPARAM>(hFont), TRUE);
            pThis->m_hFontPrev = hFont;
        } else {
            ::SendMessageW(pThis->m_hWnd, WM_SETFONT, 0, FALSE);
            pThis->m_hFontPrev = nullptr;
        }
    }
    pThis->InvalidateControl(nullptr, TRUE);
}
// COleControl::OnDoVerb(long, MSG*, HWND, const RECT*) — retail (0x1e7350)
// tail-jumps to the OLE verb-map walker: the base control declares no verb
// map and reports FALSE for every verb.
// Symbol: ?OnDoVerb@COleControl@@UEAAHJPEAUtagMSG@@PEAUHWND__@@PEBUtagRECT@@@Z
extern "C" int MS_ABI impl__OnDoVerb_COleControl__UEAAHJPEAUtagMSG__PEAUHWND____PEBUtagRECT___Z(
    COleControl* pThis, long /*iVerb*/, MSG* /*lpmsg*/, HWND /*hwndParent*/,
    const RECT* /*lpRect*/)
{
    if (!pThis) return FALSE;
    // TODO(clean-room): transcribed partially. The verb-map walk and dispatch
    // are not modeled; the base class returns FALSE (verb not handled).
    return FALSE;
}
// COleControl::OnEnumVerbs(IEnumOLEVERB**) — retail (0x1e73d0) tail-jumps to
// the OLE verb-map enumerator builder. The base control has no verb map, so
// the out-pointer is set to NULL and FALSE is returned (no verbs to report).
// Symbol: ?OnEnumVerbs@COleControl@@UEAAHPEAPEAUIEnumOLEVERB@@@Z
extern "C" int MS_ABI impl__OnEnumVerbs_COleControl__UEAAHPEAPEAUIEnumOLEVERB___Z(
    COleControl* pThis, void** ppEnumOleVerb)
{
    if (!pThis || !ppEnumOleVerb) return FALSE;
    *ppEnumOleVerb = nullptr;
    // TODO(clean-room): transcribed partially. The verb-map enumeration is
    // not modeled; the base class reports no enumerator.
    return FALSE;
}
// COleControl::OnEdit(MSG*, HWND, const RECT*) — retail (0x1e1290):
//     CopyRect(&m_rcPos, lpRect);
//     return SUCCEEDED(<virtual edit activator, vtable slot 0x508>(1, lpmsg));
// The virtual is not modeled; the rect update is kept, the activation step
// reported as unhandled.
// Symbol: ?OnEdit@COleControl@@UEAAHPEAUtagMSG@@PEAUHWND__@@PEBUtagRECT@@@Z
extern "C" int MS_ABI impl__OnEdit_COleControl__UEAAHPEAUtagMSG__PEAUHWND____PEBUtagRECT___Z(
    COleControl* pThis, MSG* /*lpmsg*/, HWND /*hwndParent*/, const RECT* lpRect)
{
    if (!pThis || !lpRect) return FALSE;
    ::CopyRect(pThis->m_rcPos, lpRect);
    // TODO(clean-room): retail dispatches the virtual in-place edit activator
    // (vtable slot 0x508) and returns SUCCEEDED(hr); not modeled.
    return FALSE;
}
// COleControl::OnDrawMetafile(CDC*, const CRect&) — retail (0x1e0600) simply
// forwards to OnDraw(pDC, rcBounds, rcBounds) through the vtable.
// Symbol: ?OnDrawMetafile@COleControl@@UEAAXPEAVCDC@@AEBVCRect@@@Z
extern "C" void MS_ABI impl__OnDrawMetafile_COleControl__UEAAXPEAVCDC__AEBVCRect___Z(
    COleControl* pThis, CDC* pDC, const CRect& rcBounds)
{
    if (!pThis || !pDC) return;
    pThis->OnDraw(pDC, rcBounds, rcBounds);
}
// COleControl::OnEraseBkgnd(CDC*) — retail (0x1e1610):
//     if (PreCreateWindow()) return <message-map machinery>;
//     return 1;
// The base control's contract is "do not erase the background", reported as
// TRUE; the superclass message routing is not modeled.
// Symbol: ?OnEraseBkgnd@COleControl@@IEAAHPEAVCDC@@@Z
extern "C" int MS_ABI impl__OnEraseBkgnd_COleControl__IEAAHPEAVCDC___Z(
    COleControl* pThis, CDC* /*pDC*/)
{
    if (!pThis) return FALSE;
    // TODO(clean-room): transcribed partially.
    return TRUE;
}
// COleControl::OnCreate(CREATESTRUCTW*) — retail (0x1e2140):
//     if (PreCreateWindow()) return <message-map machinery>;
//     return 0;
// The base control's success result is 0; the virtual PreCreateWindow gate
// and the message-map dispatch are not modeled.
// Symbol: ?OnCreate@COleControl@@IEAAHPEAUtagCREATESTRUCTW@@@Z
extern "C" int MS_ABI impl__OnCreate_COleControl__IEAAHPEAUtagCREATESTRUCTW___Z(
    COleControl* pThis, CREATESTRUCTW* /*lpCreateStruct*/)
{
    if (!pThis) return -1;
    // TODO(clean-room): transcribed partially.
    return 0;
}
// COleControl::OnDestroy() — retail (0x1e2800):
//     if (m_hFontPrev) {
//         SendMessageW(m_hWnd, WM_SETFONT, 0, 0);
//         <release the old font through the CFontHolder>;
//         m_hFontPrev = NULL;
//     }
//     <CWnd::OnDestroy>
// The font-holder release and the base-class child destruction are not
// modeled; the WM_SETFONT reset and the handle drop are kept.
// Symbol: ?OnDestroy@COleControl@@IEAAXXZ
extern "C" void MS_ABI impl__OnDestroy_COleControl__IEAAXXZ(COleControl* pThis)
{
    if (!pThis) return;
    if (pThis->m_hFontPrev) {
        if (pThis->m_hWnd) {
            ::SendMessageW(pThis->m_hWnd, WM_SETFONT, 0, 0);
        }
        pThis->m_hFontPrev = nullptr;
    }
    // TODO(clean-room): retail also releases the old font via the CFontHolder
    // and forwards to CWnd::OnDestroy; not modeled.
}
// COleControl::OnFinalRelease() — retail (0x1dfbe0):
//     if (m_bFinalReleaseCalled) return;
//     m_bFinalReleaseCalled = 1;
//     <in-place teardown>
//     if (m_hWnd) DestroyWindow();
//     <delete this>
// The delete is deliberately omitted (OpenMFC objects may be owned by the
// caller); the guard and window destruction are kept.
// Symbol: ?OnFinalRelease@COleControl@@MEAAXXZ
extern "C" void MS_ABI impl__OnFinalRelease_COleControl__MEAAXXZ(COleControl* pThis)
{
    if (!pThis) return;
    if (pThis->m_bFinalReleaseCalled) return;
    pThis->m_bFinalReleaseCalled = 1;
    if (pThis->m_hWnd) {
        pThis->DestroyWindow();
    }
    // TODO(clean-room): retail releases the in-place state and deletes the
    // control; both are not modeled.
}
// COleControl::OnFrameClose() — retail (0x1e1010): hides, reparents and
// destroys the open frame window (m_pReflect, else the control itself), then
// clears m_bOpen/m_pWndOpenFrame, closes the OLE object and notifies the
// advise holder. The notification steps are not modeled.
// Symbol: ?OnFrameClose@COleControl@@MEAAXXZ
extern "C" void MS_ABI impl__OnFrameClose_COleControl__MEAAXXZ(COleControl* pThis)
{
    if (!pThis) return;
    CWnd* pFrame = pThis->m_pReflect;
    if (pFrame == nullptr) {
        pFrame = static_cast<CWnd*>(pThis);
    }
    if (pFrame && pFrame->m_hWnd) {
        // Retail flags: SWP_NOSIZE|SWP_NOMOVE|SWP_NOZORDER|SWP_NOACTIVATE|
        // SWP_HIDEWINDOW (0x97).
        ::SetWindowPos(pFrame->m_hWnd, nullptr, 0, 0, 0, 0, 0x97);
        ::SetParent(pFrame->m_hWnd, nullptr);
        ::DestroyWindow(pFrame->m_hWnd);
    }
    pThis->m_bOpen = 0;   // btrl $0xd on the 0x160 word
    pThis->m_pWndOpenFrame = nullptr;
    if (pThis->m_xOleObject) {
        // IOleObject::Close(OLECLOSE_SAVEIFDIRTY) is slot 6 of the object's
        // vtable.
        void* pObj = pThis->m_xOleObject;
        void** vft = *static_cast<void***>(pObj);
        auto* fn = reinterpret_cast<HRESULT (STDMETHODCALLTYPE*)(void*, DWORD)>(vft[6]);
        fn(pObj, OLECLOSE_SAVEIFDIRTY);
    }
    // TODO(clean-room): retail also notifies the in-place active object and
    // the OLE advise holder; not modeled.
}
// COleControl::OnEnterIdle(UINT, CWnd*) — retail (0x1e2640):
//     if (nIdle != MSGF_DIALOGBOX) return;
//     if (m_nIDTracking == m_nIDLastMessage) return;
//     SendMessageW(m_hWnd, WM_ENTERIDLE, m_nIDTracking, 0);
// Symbol: ?OnEnterIdle@COleControl@@IEAAXIPEAVCWnd@@@Z
extern "C" void MS_ABI impl__OnEnterIdle_COleControl__IEAAXIPEAVCWnd___Z(
    COleControl* pThis, unsigned int nIdle, CWnd* /*pWnd*/)
{
    if (!pThis) return;
    if (nIdle != MSGF_DIALOGBOX) return;
    if (pThis->m_nIDTracking == pThis->m_nIDLastMessage) return;
    if (pThis->m_hWnd) {
        ::SendMessageW(pThis->m_hWnd, WM_ENTERIDLE,
                       static_cast<WPARAM>(pThis->m_nIDTracking), 0);
    }
}
// COleControl::OnActivateInPlace(int, MSG*) — retail (0x1e4c40) is the full
// in-place activation handshake. Only the early paths are reproducible:
// the m_bOpen fast path (activate the open frame window) and the null-site
// E_UNEXPECTED result; the windowless negotiation is not modeled.
// Symbol: ?OnActivateInPlace@COleControl@@MEAAJHPEAUtagMSG@@@Z
extern "C" long MS_ABI impl__OnActivateInPlace_COleControl__MEAAJHPEAUtagMSG___Z(
    COleControl* pThis, int /*lVerb*/, MSG* /*lpmsg*/)
{
    if (!pThis) return 0x8000ffff;   // E_UNEXPECTED
    if (pThis->m_bOpen) {
        CWnd* pFrame = pThis->m_pWndOpenFrame;
        if (pFrame && pFrame->m_hWnd) {
            ::SetActiveWindow(pFrame->m_hWnd);
        }
        // TODO(clean-room): retail also routes the message through the
        // container's TranslateAccelerator; not modeled.
        return 0;   // S_OK
    }
    if (!pThis->m_pInPlaceSite && !pThis->m_pClientSite) {
        return 0x8000ffff;   // E_UNEXPECTED
    }
    // TODO(clean-room): transcribed partially. The site negotiation,
    // windowless activation and SetObjectRects handshake are not modeled;
    // report success without side effects.
    return 0;   // S_OK
}
// COleControl::OnEventAdvise(int) — retail (0x27d0) is a bare `ret $0x0`:
// the base control takes no action when a container connects/disconnects an
// event sink (derived controls use this to delay event work).
// Symbol: ?OnEventAdvise@COleControl@@UEAAXH@Z
extern "C" void MS_ABI impl__OnEventAdvise_COleControl__UEAAXH_Z(
    COleControl* /*pThis*/, int /*fAdvise*/)
{
}
// COleControl::OnFreezeEvents(int) — retail (0x27d0) is a bare `ret $0x0`.
// Symbol: ?OnFreezeEvents@COleControl@@UEAAXH@Z
extern "C" void MS_ABI impl__OnFreezeEvents_COleControl__UEAAXH_Z(
    COleControl* /*pThis*/, int /*bFreeze*/)
{
}
// COleControl::Load(const wchar_t*, CDataPathProperty&) — retail (0x1ef410):
//     prop.SetControl(this);                 ; store this at prop+0x60
//     prop.Open(lpszPath, 0);                ; virtual, vtable slot 0xe0
// CDataPathProperty is not declared in OpenMFC, so the parameter is carried
// as void* and the two retail offsets are used directly.
// Symbol: ?Load@COleControl@@QEAAXPEB_WAEAVCDataPathProperty@@@Z
extern "C" void MS_ABI impl__Load_COleControl__QEAAXPEB_WAEAVCDataPathProperty___Z(
    COleControl* pThis, const wchar_t* lpszPath, void* pProp)
{
    if (!pThis || !pProp) return;
    // TODO(clean-room): CDataPathProperty is not declared in OpenMFC; the
    // 0x60 member slot and vtable slot 0xe0 come from the retail disassembly.
    *reinterpret_cast<void**>(static_cast<char*>(pProp) + 0x60) = pThis;
    void** vft = *static_cast<void***>(pProp);
    auto* fn = reinterpret_cast<void (STDMETHODCALLTYPE*)(void*, const wchar_t*, unsigned int)>(vft[0xe0 / 8]);
    fn(pProp, lpszPath, 0);
}
// COleControl::LoadState(IStream*) — retail (0x1e0b40) wraps the stream in a
// CArchive, invokes the virtual Serialize (vtable slot 0x10) and then clears
// m_bModified while raising m_bInitialized|m_bAutoMenuEnable. The CArchive
// construction is not modeled; report success without side effects.
// Symbol: ?LoadState@COleControl@@IEAAJPEAUIStream@@@Z
extern "C" long MS_ABI impl__LoadState_COleControl__IEAAJPEAUIStream___Z(
    COleControl* pThis, void* /*pStream*/)
{
    if (!pThis) return 0;   // S_OK
    // TODO(clean-room): transcribed partially.
    return 0;   // S_OK
}
// COleControl::LockInPlaceActive(int) — retail (0x1e1730):
//     if (!m_pControlSite) return FALSE;
//     return SUCCEEDED(m_pControlSite->LockInPlaceActive(bLock));
// IOleControlSite::LockInPlaceActive is slot 4 of the site vtable.
// Symbol: ?LockInPlaceActive@COleControl@@QEAAHH@Z
extern "C" int MS_ABI impl__LockInPlaceActive_COleControl__QEAAHH_Z(
    COleControl* pThis, int bLock)
{
    if (!pThis) return FALSE;
    void* pSite = pThis->m_pControlSite;
    if (!pSite) return FALSE;
    void** vft = *static_cast<void***>(pSite);
    auto* fn = reinterpret_cast<HRESULT (STDMETHODCALLTYPE*)(void*, BOOL)>(vft[4]);
    return SUCCEEDED(fn(pSite, bLock ? TRUE : FALSE)) ? TRUE : FALSE;
}
// COleControl::GetClientSite() — retail is a single load:
//     mov rax,[rcx+0x1d8] ; ret          (0x1d8 == 472 == m_pClientSite)
// Note it does NOT AddRef, matching the raw-pointer accessor contract.
// Symbol: ?GetClientSite@COleControl@@QEAAPEAUIOleClientSite@@XZ
extern "C" IOleClientSite* MS_ABI impl__GetClientSite_COleControl__QEAAPEAUIOleClientSite__XZ(
    COleControl* pThis)
{
    if (!pThis) return nullptr;
    return pThis->m_pClientSite;
}
// COleControl::IsModified() — retail:
//     mov eax,[rcx+0x160] ; shr eax,2 ; and eax,1 ; ret
// i.e. it returns the m_bModified bit directly, not a normalized TRUE/FALSE
// from any other state.
// Symbol: ?IsModified@COleControl@@QEAAHXZ
extern "C" int MS_ABI impl__IsModified_COleControl__QEAAHXZ(COleControl* pThis)
{
    if (!pThis) return FALSE;
    return static_cast<int>(pThis->m_bModified);
}
// COleControl::GetActivationPolicy() — retail is literally `xor eax,eax ; ret`.
// The base control declares no activation policy; derived controls override to
// return POINTERINACTIVE_* flags.
// Symbol: ?GetActivationPolicy@COleControl@@UEAAKXZ
extern "C" unsigned long MS_ABI impl__GetActivationPolicy_COleControl__UEAAKXZ(
    COleControl* /*pThis*/)
{
    return 0;
}
// COleControl::GetRectInContainer(RECT*) — retail decodes to:
//     if (m_bInPlaceActive)              // test dword [rcx+0x160],0x400
//         CopyRect(lpRect, &m_rcPos);    // lea rdx,[rcx+0x120]
//     return m_bInPlaceActive;           // shr eax,0xa ; and eax,1
// The rect is left untouched when the control is not in-place active, and the
// return value is the same bit that gated the copy.
// Symbol: ?GetRectInContainer@COleControl@@QEAAHPEAUtagRECT@@@Z
extern "C" int MS_ABI impl__GetRectInContainer_COleControl__QEAAHPEAUtagRECT___Z(
    COleControl* pThis, RECT* lpRect)
{
    if (!pThis) return FALSE;
    const int bInPlaceActive = static_cast<int>(pThis->m_bInPlaceActive);
    if (bInPlaceActive && lpRect) {
        ::CopyRect(lpRect, reinterpret_cast<const RECT*>(&pThis->m_rcPos));
    }
    return bInPlaceActive;
}
// COleControl::GetAmbientDispatchDriver() — retail lazily binds the driver to
// the container's IDispatch the first time it is asked:
//     lea rbx,[rcx+0x100]                 ; &m_ambientDispDriver
//     cmp qword ptr [rbx],0 ; jne done    ; already attached?
//     mov rcx,[rcx+0x1d8]                 ; m_pClientSite
//     ... QueryInterface(IID_IDispatch) and attach ...
// It always returns the address of the embedded driver, attached or not.
// Symbol: ?GetAmbientDispatchDriver@COleControl@@IEAAPEAVCOleDispatchDriver@@XZ
extern "C" COleDispatchDriver* MS_ABI
impl__GetAmbientDispatchDriver_COleControl__IEAAPEAVCOleDispatchDriver__XZ(COleControl* pThis)
{
    if (!pThis) return nullptr;
    COleDispatchDriver* pDriver = &pThis->m_ambientDispDriver;
    // m_lpDispatch is private; GetIDispatch(FALSE) reads the same slot without
    // taking a reference.
    if (pDriver->GetIDispatch(FALSE) == nullptr && pThis->m_pClientSite != nullptr) {
        IDispatch* pDisp = nullptr;
        if (SUCCEEDED(pThis->m_pClientSite->QueryInterface(IID_IDispatch,
                                                           reinterpret_cast<void**>(&pDisp)))
            && pDisp != nullptr) {
            // AttachDispatch takes ownership of the reference QueryInterface
            // handed back, matching retail (no extra AddRef at this site).
            pDriver->AttachDispatch(pDisp, TRUE);
        }
    }
    return pDriver;
}
// COleControl::EnableSimpleFrame() — retail is a single bit set:
//     bts DWORD PTR [rcx+0x160],0x10 ; ret
// It only raises the flag; it does not create the frame or notify the
// container. There is no matching disable entry point in retail either.
// Symbol: ?EnableSimpleFrame@COleControl@@QEAAXXZ
extern "C" void MS_ABI impl__EnableSimpleFrame_COleControl__QEAAXXZ(COleControl* pThis)
{
    if (!pThis) return;
    pThis->m_bSimpleFrame = 1;   // bit 16 of the 0x160 word
}
// COleControl::IsInvokeAllowed(DISPID) — retail:
//     mov eax,[rcx+0x160] ; shr eax,0x12 ; and eax,1 ; ret
// Bit 18 is returned directly and the DISPID argument is ignored entirely --
// the base class makes no per-member decision, it reports whether the control
// is in a state that permits automation at all. Derived classes override.
// Symbol: ?IsInvokeAllowed@COleControl@@MEAAHJ@Z
extern "C" int MS_ABI impl__IsInvokeAllowed_COleControl__MEAAHJ_Z(
    COleControl* pThis, long /*dispid*/)
{
    if (!pThis) return FALSE;
    return pThis->m_bInitialized ? TRUE : FALSE;   // bit 18 of the 0x160 word
}
// COleControl::GetOuterWindow() const — retail:
//     mov rax,[rcx+0x150] ; test rax,rax ; cmovne rcx,rax ; mov rax,rcx ; ret
// i.e. the reflected window if one is installed, otherwise the control itself.
// The cmov makes the fallback branchless but the semantics are a plain
// null-check; note it never returns null for a valid control.
// Symbol: ?GetOuterWindow@COleControl@@IEBAPEAVCWnd@@XZ
extern "C" CWnd* MS_ABI impl__GetOuterWindow_COleControl__IEBAPEAVCWnd__XZ(
    const COleControl* pThis)
{
    if (!pThis) return nullptr;
    CWnd* pReflect = pThis->m_pReflect;
    return pReflect != nullptr ? pReflect
                               : const_cast<CWnd*>(static_cast<const CWnd*>(pThis));
}
// COleControl::InternalGetFont() — retail is `lea rax,[rcx+0x178] ; ret`:
// it hands back a reference to the embedded CFontHolder, not a copy and not a
// COM pointer, so callers mutate the control's own font state through it.
// Symbol: ?InternalGetFont@COleControl@@QEAAAEAVCFontHolder@@XZ
extern "C" CFontHolder* MS_ABI impl__InternalGetFont_COleControl__QEAAAEAVCFontHolder__XZ(
    COleControl* pThis)
{
    if (!pThis) return nullptr;
    return &pThis->m_font;
}
// COleControl::GetWindowlessDropTarget() — retail is `xor eax,eax ; ret`.
// The base control supplies no drop target; a windowless control that wants
// drag-and-drop overrides this to return one.
// Symbol: ?GetWindowlessDropTarget@COleControl@@UEAAPEAUIDropTarget@@XZ
extern "C" void* MS_ABI impl__GetWindowlessDropTarget_COleControl__UEAAPEAUIDropTarget__XZ(
    COleControl* /*pThis*/)
{
    return nullptr;
}
// COleControl::OnCreateAggregates() — retail is `mov eax,0x1 ; ret`.
// The base class aggregates nothing and reports success unconditionally;
// derived controls override to create their aggregates and may fail.
// Symbol: ?OnCreateAggregates@COleControl@@MEAAHXZ
extern "C" int MS_ABI impl__OnCreateAggregates_COleControl__MEAAHXZ(COleControl* /*pThis*/)
{
    return TRUE;
}
// COleControl::GetPropPageIDs(DWORD&) — retail:
//     mov DWORD PTR [rdx],0x0 ; xor eax,eax ; ret
// The base control publishes no property pages: it writes a zero count through
// the out-parameter and returns a null array. Writing the count matters --
// the generated stub left the caller's DWORD untouched, so a container could
// walk a null array using stack garbage as the length.
// Symbol: ?GetPropPageIDs@COleControl@@MEAAPEAU_GUID@@AEAK@Z
extern "C" GUID* MS_ABI impl__GetPropPageIDs_COleControl__MEAAPEAU_GUID__AEAK_Z(
    COleControl* /*pThis*/, DWORD* pcPages)
{
    if (pcPages) *pcPages = 0;
    return nullptr;
}
// Symbol: ??0CControlDataSource@COleControl@@QEAA@PEAV1@@Z
extern "C" void* MS_ABI impl___0CControlDataSource_COleControl__QEAA_PEAV1__Z(void* pThis, void* pContainer) {
    (void)pContainer;
    return pThis;
}
// Symbol: ?GetDataSource@COleControl@@IEAAPEAVCControlDataSource@1@XZ
extern "C" void* MS_ABI impl__GetDataSource_COleControl__IEAAPEAVCControlDataSource_1_XZ(
    void* p0, void* /*p1*/, void* /*p2*/, void* /*p3*/, void* /*p4*/, void* /*p5*/, void* /*p6*/,
    char /*p7*/, void* /*p8*/, void* /*p9*/, void* /*p10*/, void* /*p11*/, void* /*p12*/, void* /*p13*/,
    void* /*p14*/, void* /*p15*/, void* /*p16*/, char /*p17*/) {
    auto* control = static_cast<COleControl*>(p0);
    if (!control) return nullptr;

    if (!control->m_pDataSource) {
        return new COleControl::CControlDataSource(control);
    }

    return control->m_pDataSource;
}
// Symbol: ?OnRenderData@CControlDataSource@COleControl@@MEAAHPEAUtagFORMATETC@@PEAUtagSTGMEDIUM@@@Z
extern "C" int MS_ABI impl__OnRenderData_CControlDataSource_COleControl__MEAAHPEAUtagFORMATETC__PEAUtagSTGMEDIUM___Z(
    void* p0, FORMATETC* p0Format, STGMEDIUM* p1) {
    auto* source = static_cast<COleControl::CControlDataSource*>(p0);
    if (!source || !p0Format || !p1) return FALSE;

    COleControl* control = GetControlFromDataSource(source);
    if (!control) return FALSE;

    FORMATETC request = MakeFormatEtc(0, p0Format);
    OleControlDataSourceCacheEntry* entry = FindControlDataSourceEntry(control, request, false);
    if (!entry || !entry->hasMedium) return FALSE;

    return CopyStorageMedium(entry->medium, p1) ? TRUE : FALSE;
}
// Symbol: ?OnRenderFileData@CControlDataSource@COleControl@@MEAAHPEAUtagFORMATETC@@PEAVCFile@@@Z
extern "C" int MS_ABI impl__OnRenderFileData_CControlDataSource_COleControl__MEAAHPEAUtagFORMATETC__PEAVCFile___Z(
    void* p0, FORMATETC* p0Format, CFile* p1) {
    auto* source = static_cast<COleControl::CControlDataSource*>(p0);
    if (!source || !p0Format || !p1) return FALSE;

    COleControl* control = GetControlFromDataSource(source);
    if (!control) return FALSE;

    OleControlDataSourceCacheEntry* entry = FindControlDataSourceEntry(control, *p0Format, false);
    if (!entry || !entry->hasMedium) return FALSE;

    const STGMEDIUM& medium = entry->medium;
    if (medium.tymed == TYMED_HGLOBAL && medium.hGlobal) {
        SIZE_T size = GlobalSize(medium.hGlobal);
        void* data = GlobalLock(medium.hGlobal);
        if (!data) return FALSE;
        p1->Write(data, static_cast<UINT>(size));
        GlobalUnlock(medium.hGlobal);
        return TRUE;
    }

    if (medium.tymed == TYMED_FILE && medium.lpszFileName) {
        CFile sourceFile(medium.lpszFileName, CFile::modeRead | CFile::shareDenyNone | CFile::typeBinary);
        if (sourceFile.m_hFile == INVALID_HANDLE_VALUE) {
            return FALSE;
        }
        BYTE buffer[4096] = {};
        UINT read = 0;
        while ((read = sourceFile.Read(buffer, sizeof(buffer))) > 0) {
            p1->Write(buffer, read);
        }
        return TRUE;
    }

    return FALSE;
}
// Symbol: ?OnRenderGlobalData@CControlDataSource@COleControl@@MEAAHPEAUtagFORMATETC@@PEAPEAX@Z
extern "C" int MS_ABI impl__OnRenderGlobalData_CControlDataSource_COleControl__MEAAHPEAUtagFORMATETC__PEAPEAX_Z(
    void* p0, FORMATETC* p0Format, void** p0Medium) {
    auto* source = static_cast<COleControl::CControlDataSource*>(p0);
    if (!source || !p0Medium) return FALSE;

    COleControl* control = GetControlFromDataSource(source);
    if (!control) return FALSE;

    if (!p0Format) return FALSE;
    OleControlDataSourceCacheEntry* entry = FindControlDataSourceEntry(control, *p0Format, false);
    if (!entry || !entry->hasMedium || entry->medium.tymed != TYMED_HGLOBAL || !entry->medium.hGlobal) {
        *p0Medium = nullptr;
        return FALSE;
    }

    *p0Medium = DuplicateGlobalMemory(entry->medium.hGlobal);
    return *p0Medium != nullptr;
}
// Symbol: ?OnSetData@CControlDataSource@COleControl@@MEAAHPEAUtagFORMATETC@@PEAUtagSTGMEDIUM@@H@Z
extern "C" int MS_ABI impl__OnSetData_CControlDataSource_COleControl__MEAAHPEAUtagFORMATETC__PEAUtagSTGMEDIUM__H_Z(
    void* p0, FORMATETC* p0Format, STGMEDIUM* p1Medium, int /*p2*/) {
    auto* source = static_cast<COleControl::CControlDataSource*>(p0);
    if (!source || !p0Format || !p1Medium) return FALSE;

    COleControl* control = GetControlFromDataSource(source);
    if (!control) return FALSE;

    OleControlDataSourceCacheEntry* entry = FindControlDataSourceEntry(control, *p0Format, true);
    if (!entry) return FALSE;

    ReleaseDataSourceEntryMedium(*entry);
    entry->format = MakeFormatEtc(0, p0Format);
    entry->hasMedium = CopyStorageMedium(*p1Medium, &entry->medium);
    return entry->hasMedium ? TRUE : FALSE;
}
COleControl::CControlDataSource::CControlDataSource(COleControl* pCtrl) {
    if (pCtrl) pCtrl->m_pDataSource = this;
}
COleControl::COleControl() {
    // Zero the whole retail member block, then set the members whose retail
    // initial values are not zero. Members live at fixed offsets now, so this
    // clears exactly the region a real client would inspect.
    std::memset(reinterpret_cast<char*>(this) + sizeof(CWnd), 0,
                sizeof(COleControl) - sizeof(CWnd));
    m_bAutoMenuEnable = 1;
    m_bAutoClip = 1;
    m_bCountOnAmbients = 1;
    m_sAppearance = 0;
    m_bEnabled = TRUE;
    m_lReadyState = 4;              // READYSTATE_COMPLETE
    m_clrBackColor = 0x80000005;    // COLOR_WINDOW  | 0x80000000
    m_clrForeColor = 0x80000008;    // COLOR_WINDOWTEXT | 0x80000000
    new (&m_strText) CString();
    new (&m_font) CFontHolder();
    new (&m_ambientDispDriver) COleDispatchDriver();
    GetOleControlState(this, true);
}
COleControl::~COleControl() {
    if (m_pDataSource) {
        auto* source = static_cast<COleControl::CControlDataSource*>(m_pDataSource);
        m_pDataSource = nullptr;
        delete source;
    }
    RemoveOleControlState(this);
}
BOOL COleControl::CreateControl(REFCLSID clsid, const wchar_t* lpszWindowName,
                                 DWORD dwStyle, const RECT& rect, CWnd* pParentWnd,
                                 UINT nID, CFile* pPersist, BOOL bStorage, BSTR bstrLicKey) {
    if (!pParentWnd) return FALSE;

    COleControlContainer* container = pParentWnd->GetControlContainer();
    if (!container) {
        if (!pParentWnd->CreateControlContainer(&container) || !container) return FALSE;
    }

    COleControlSite* site = container->CreateSite(container);
    if (!site) return FALSE;
    site->m_pControl = this;

    if (!site->CreateControl(this, clsid, lpszWindowName, dwStyle, rect, nID, pPersist, bStorage, bstrLicKey)) {
        site->m_pControl = nullptr;
        container->DeleteSite(site);
        return FALSE;
    }

    if (OleControlState* state = GetOleControlState(this, true)) {
        state->container = container;
        state->mfcSite = site;
    }
    m_bInitialized = 1;
    return TRUE;
}
BOOL COleControl::CreateControl(const wchar_t* lpszProgID, const wchar_t* lpszWindowName,
                                 DWORD dwStyle, const RECT& rect, CWnd* pParentWnd,
                                 UINT nID, CFile* pPersist, BOOL bStorage, BSTR bstrLicKey) {
    if (!lpszProgID) return FALSE;
    CLSID clsid;
    HRESULT hr = CLSIDFromProgID(lpszProgID, &clsid);
    if (FAILED(hr)) return FALSE;
    return CreateControl(clsid, lpszWindowName, dwStyle, rect, pParentWnd, nID, pPersist, bStorage, bstrLicKey);
}
BOOL COleControl::GetLicenseKey(DWORD dwReserved, BSTR* pbstrKey) {
    (void)dwReserved;
    if (pbstrKey) {
        *pbstrKey = nullptr;
    }

    OleControlState* state = GetOleControlState(this, false);
    if (!state || state->licenseKey.IsEmpty()) {
        return FALSE;
    }

    if (pbstrKey) {
        *pbstrKey = AllocateLicenseBstr(state->licenseKey);
    }
    return TRUE;
}
BSTR COleControl::GetLicenseKey(REFCLSID clsid) {
    return AllocateLicenseBstr(FindFactoryLicense(clsid));
}
void COleControl::Serialize(CArchive& ar) {
    CWnd::Serialize(ar);
}
BOOL COleControl::DoPropExchange(CPropExchange* pPX) {
    return pPX != nullptr;
}
BOOL COleControl::GetAmbientProperty(DISPID dwDispid, VARTYPE vtProp, void* pvProp) {
    if (!pvProp) return FALSE;

    // OpenMFC's own CreateControl path records a COleControlSite in the side
    // table and that site answers ambients directly, so it stays the preferred
    // source. It is not the only way to be hosted, though: an ordinary COM
    // container calls SetClientSite and never touches the side table, and for
    // those controls every ambient lookup used to fail outright. Fall back to
    // the container's IDispatch, which is the path retail always takes.
    if (COleControlSite* pSite = MfcSiteOf(this))
        return pSite->GetAmbientProperty(dwDispid, vtProp, pvProp);

    COleDispatchDriver* pDriver =
        impl__GetAmbientDispatchDriver_COleControl__IEAAPEAVCOleDispatchDriver__XZ(this);
    if (pDriver == nullptr || pDriver->GetIDispatch(FALSE) == nullptr)
        return FALSE;

    // DISPATCH_PROPERTYGET == 2. A container that does not implement the
    // ambient reports failure through the driver rather than by throwing here.
    pDriver->InvokeHelper(dwDispid, 2, vtProp, pvProp, nullptr);
    return TRUE;
}
void COleControl::FireEvent(DISPID dispId, BYTE* pbParams, ...) {
    va_list args;
    va_start(args, pbParams);
    FireEventV(dispId, pbParams, args);
    va_end(args);
}
void COleControl::FireEventV(DISPID dispId, BYTE* pbParams, va_list argList) {
    OleControlState* state = GetOleControlState(this, false);
    if (!state || state->eventSinks.empty()) return;

    // Build DISPPARAMS from the VT-encoded parameter list
    int cParams = CountDispatchParams(pbParams);
    std::vector<VARIANTARG> params(static_cast<size_t>(cParams));
    va_list argCopy;
    // We need a copy of argList per each sink invocation
    // Build params once; they are read-only for each Invoke call
    va_copy(argCopy, argList);
    for (int i = 0; i < cParams; ++i) {
        MakeDispatchVariant(static_cast<VARTYPE>(pbParams[i]), &argCopy, &params[static_cast<size_t>(i)]);
    }
    va_end(argCopy);

    // Reverse for DISPPARAMS (COM convention: last arg first)
    std::vector<VARIANTARG> revParams(static_cast<size_t>(cParams));
    for (int i = 0; i < cParams; ++i) {
        revParams[static_cast<size_t>(i)] = params[static_cast<size_t>(cParams - i - 1)];
    }

    DISPPARAMS dp = {};
    dp.cArgs = static_cast<UINT>(cParams);
    dp.rgvarg = cParams ? revParams.data() : nullptr;

    // Fire to all registered sinks
    for (auto& sink : state->eventSinks) {
        if (!sink.sink) continue;
        IDispatch* pDisp = nullptr;
        if (SUCCEEDED(sink.sink->QueryInterface(IID_IDispatch, reinterpret_cast<void**>(&pDisp))) && pDisp) {
            pDisp->Invoke(dispId, IID_NULL, LOCALE_USER_DEFAULT,
                          DISPATCH_METHOD, &dp, nullptr, nullptr, nullptr);
            pDisp->Release();
        }
    }

    // Clean up variants (don't use VariantClear on the reversed view; clear originals)
    for (int i = 0; i < cParams; ++i) {
        params[static_cast<size_t>(i)].vt = VT_EMPTY;
        VariantClear(&params[static_cast<size_t>(i)]);
    }
}
BOOL COleControl::IsOptimizedDraw() const {
    return m_bOptimizedDraw;
}
void COleControl::SetInitialSize(int cx, int cy) {
    SetControlSize(cx, cy);
}
void COleControl::OnDraw(CDC* pDC, const CRect& rcBounds, const CRect& rcInvalid) {
    (void)rcInvalid;
    if (!pDC || !pDC->GetSafeHdc()) return;

    RECT rc = *rcBounds;
    const COLORREF backColor = m_bEnabled ? AmbientBackColor() : RGB(240, 240, 240);
    HBRUSH hBrush = ::CreateSolidBrush(backColor);
    if (hBrush) {
        ::FillRect(pDC->GetSafeHdc(), &rc, hBrush);
        ::DeleteObject(hBrush);
    } else {
        ::FillRect(pDC->GetSafeHdc(), &rc, (HBRUSH)::GetStockObject(WHITE_BRUSH));
    }
    ::FrameRect(pDC->GetSafeHdc(), &rc, (HBRUSH)::GetStockObject(BLACK_BRUSH));

    const wchar_t* text = m_strText.IsEmpty() ? L"ActiveX Control" : m_strText.GetString();
    int oldMode = ::SetBkMode(pDC->GetSafeHdc(), TRANSPARENT);
    ::SetTextColor(pDC->GetSafeHdc(), AmbientForeColor());
    ::DrawTextW(pDC->GetSafeHdc(), text, -1, &rc,
                DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_NOPREFIX);
    ::SetBkMode(pDC->GetSafeHdc(), oldMode);
}
BOOL COleControl::VerifyUserLicense() { return TRUE; }
BOOL COleControl::VerifyLicenseKey(BSTR bstrKey) {
    OleControlState* state = GetOleControlState(this, false);
    if (!state || state->licenseKey.IsEmpty()) {
        return TRUE;
    }
    CString key = bstrKey ? bstrKey : L"";
    return key == state->licenseKey;
}
BOOL COleControl::SetLicenseKey(const wchar_t* lpszLicenseKey) {
    OleControlState* state = GetOleControlState(this, true);
    if (!state) return FALSE;
    state->licenseKey = lpszLicenseKey ? lpszLicenseKey : L"";
    return TRUE;
}
void COleControl::DoDataExchange(void* pDX) { (void)pDX; }
void COleControl::OnResetState() {
    OleControlState* state = GetOleControlState(this, true);
    if (state) state->text.Empty();
    m_strText.Empty();
    // The properties below now live in the ABI-visible members, so the reset
    // has to clear those rather than the side-table shadow a client cannot
    // see. Values match what the retail constructor establishes.
    m_clrBackColor = 0x80000005;   // COLOR_WINDOW     | 0x80000000
    m_clrForeColor = 0x80000008;   // COLOR_WINDOWTEXT | 0x80000000
    m_bEnabled     = TRUE;
    m_sAppearance  = 0;
    m_sBorderStyle = 0;
    m_lReadyState  = 4;            // READYSTATE_COMPLETE
    // The side table is not the ABI-visible copy: IsModified() reads the
    // m_bModified bit at this+0x160, so the reset has to clear that too or a
    // freshly reset control still reports itself dirty to a real client.
    m_bModified = 0;
    m_cxExtent = 0;
    m_cyExtent = 0;
}
DWORD COleControl::GetControlFlags() {
    return 0x00000002;   // clipPaintDC
}
BOOL COleControl::OnSetExtent(DVASPECT dwDrawAspect, const SIZE& size) {
    if (dwDrawAspect != DVASPECT_CONTENT) return FALSE;
    m_cxExtent = size.cx;
    m_cyExtent = size.cy;
    return TRUE;
}
BOOL COleControl::OnGetExtent(DVASPECT dwDrawAspect, SIZE& size) {
    if (dwDrawAspect != DVASPECT_CONTENT) return FALSE;
    size.cx = m_cxExtent;
    size.cy = m_cyExtent;
    return TRUE;
}
BOOL COleControl::OnMapPropertyToPage(DISPID dispid, CLSID* pclsid, BOOL* pbPageOptional) {
    if (!pclsid) return FALSE;
    *pclsid = CLSID_NULL;
    if (pbPageOptional) *pbPageOptional = FALSE;

    IPerPropertyBrowsing* browsing = QueryOleControlInterface<IPerPropertyBrowsing>(this, IID_IPerPropertyBrowsing);
    if (!browsing) return FALSE;

    CLSID clsid = CLSID_NULL;
    HRESULT hr = browsing->MapPropertyToPage(dispid, &clsid);
    browsing->Release();
    if (FAILED(hr) || IsEqualCLSID(clsid, CLSID_NULL)) return FALSE;

    *pclsid = clsid;
    return TRUE;
}
COLORREF COleControl::AmbientBackColor() { COLORREF cr = RGB(255,255,255); GetAmbientProperty(DISPID_AMBIENT_BACKCOLOR, VT_COLOR, &cr); return cr; }
COLORREF COleControl::AmbientForeColor() { COLORREF cr = RGB(0,0,0); GetAmbientProperty(DISPID_AMBIENT_FORECOLOR, VT_COLOR, &cr); return cr; }
COLORREF COleControl::AmbientAppearance() { short appearance = 0; GetAmbientProperty(DISPID_AMBIENT_APPEARANCE, VT_I2, &appearance); return static_cast<COLORREF>(appearance); }
OLE_COLOR COleControl::AmbientBackColorOle() { return (OLE_COLOR)AmbientBackColor(); }
OLE_COLOR COleControl::AmbientForeColorOle() { return (OLE_COLOR)AmbientForeColor(); }
IFontDisp* COleControl::AmbientFont() { IDispatch* p = nullptr; GetAmbientProperty(DISPID_AMBIENT_FONT, VT_DISPATCH, &p); return (IFontDisp*)p; }
IDispatch* COleControl::AmbientFontDisp() { IDispatch* p = nullptr; GetAmbientProperty(DISPID_AMBIENT_FONT, VT_DISPATCH, &p); return p; }
short COleControl::AmbientTextAlign() {
    short align = 0;
    GetAmbientProperty(DISPID_AMBIENT_TEXTALIGN, VT_I2, &align);
    return align;
}
BOOL COleControl::AmbientUserMode() { BOOL b = TRUE; GetAmbientProperty(DISPID_AMBIENT_USERMODE, VT_BOOL, &b); return b; }
BOOL COleControl::AmbientUIDead() { BOOL b = FALSE; GetAmbientProperty(DISPID_AMBIENT_UIDEAD, VT_BOOL, &b); return b; }
BOOL COleControl::AmbientShowGrabHandles() { BOOL b = TRUE; GetAmbientProperty(DISPID_AMBIENT_SHOWGRABHANDLES, VT_BOOL, &b); return b; }
BOOL COleControl::AmbientShowHatching() { BOOL b = TRUE; GetAmbientProperty(DISPID_AMBIENT_SHOWHATCHING, VT_BOOL, &b); return b; }
CString COleControl::AmbientDisplayName() {
    BSTR bstr = nullptr;
    if (!GetAmbientProperty(DISPID_AMBIENT_DISPLAYNAME, VT_BSTR, &bstr) || !bstr) return CString();
    CString text(bstr);
    SysFreeString(bstr);
    return text;
}
BOOL COleControl::AmbientDisplayAsDefault() { BOOL b = FALSE; GetAmbientProperty(DISPID_AMBIENT_DISPLAYASDEFAULT, VT_BOOL, &b); return b; }
BOOL COleControl::AmbientAutoClip() { return TRUE; }
BOOL COleControl::AmbientSupportsMnemonics() { BOOL b = TRUE; GetAmbientProperty(DISPID_AMBIENT_SUPPORTSMNEMONICS, VT_BOOL, &b); return b; }
CString COleControl::AmbientScaleUnits() {
    BSTR bstr = nullptr;
    if (!GetAmbientProperty(DISPID_AMBIENT_SCALEUNITS, VT_BSTR, &bstr) || !bstr) return CString();
    CString text(bstr);
    SysFreeString(bstr);
    return text;
}
unsigned long COleControl::AmbientLocaleID() { unsigned long locale = static_cast<unsigned long>(::GetUserDefaultLCID()); GetAmbientProperty(DISPID_AMBIENT_LOCALEID, VT_I4, &locale); return locale; }
void COleControl::FireClick() {
    BYTE noParams[1] = { 0 };
    FireEvent(DISPID_CLICK, noParams);
}
void COleControl::FireDblClick() {
    BYTE noParams[1] = { 0 };
    FireEvent(DISPID_DBLCLICK, noParams);
}
void COleControl::FireKeyDown(USHORT* pnChar, short nShiftState) {
    // VT_PI2 (pointer to I2) = VT_BYREF|VT_I2 = 0x4002; MFC uses 0x4002 for USHORT*
    BYTE params[3] = { static_cast<BYTE>(VT_BYREF | VT_I2), VT_I2, 0 };
    FireEvent(DISPID_KEYDOWN, params, pnChar, nShiftState);
}
void COleControl::FireKeyPress(USHORT* pnChar) {
    BYTE params[2] = { static_cast<BYTE>(VT_BYREF | VT_I2), 0 };
    FireEvent(DISPID_KEYPRESS, params, pnChar);
}
void COleControl::FireKeyUp(USHORT* pnChar, short nShiftState) {
    BYTE params[3] = { static_cast<BYTE>(VT_BYREF | VT_I2), VT_I2, 0 };
    FireEvent(DISPID_KEYUP, params, pnChar, nShiftState);
}
void COleControl::FireMouseDown(short nButton, short nShiftState, long x, long y) {
    BYTE params[5] = { VT_I2, VT_I2, VT_I4, VT_I4, 0 };
    FireEvent(DISPID_MOUSEDOWN, params, nButton, nShiftState, x, y);
}
void COleControl::FireMouseMove(short nButton, short nShiftState, long x, long y) {
    BYTE params[5] = { VT_I2, VT_I2, VT_I4, VT_I4, 0 };
    FireEvent(DISPID_MOUSEMOVE, params, nButton, nShiftState, x, y);
}
void COleControl::FireMouseUp(short nButton, short nShiftState, long x, long y) {
    BYTE params[5] = { VT_I2, VT_I2, VT_I4, VT_I4, 0 };
    FireEvent(DISPID_MOUSEUP, params, nButton, nShiftState, x, y);
}
void COleControl::FireReadyStateChange() {
    BYTE noParams[1] = { 0 };
    FireEvent(DISPID_READYSTATECHANGE, noParams);
}
COLORREF COleControl::GetBackColor() const {
    return static_cast<COLORREF>(m_clrBackColor);
}
void COleControl::SetBackColor(COLORREF clr) {
    if (m_clrBackColor == static_cast<OLE_COLOR>(clr)) return;
    m_clrBackColor = static_cast<OLE_COLOR>(clr);
    SetModifiedFlag(TRUE);
    InvalidateControl();
}
COLORREF COleControl::GetForeColor() const {
    return static_cast<COLORREF>(m_clrForeColor);
}
void COleControl::SetForeColor(COLORREF clr) {
    if (m_clrForeColor == static_cast<OLE_COLOR>(clr)) return;
    m_clrForeColor = static_cast<OLE_COLOR>(clr);
    SetModifiedFlag(TRUE);
    InvalidateControl();
}
BOOL COleControl::GetEnabled() const {
    return m_bEnabled;
}
void COleControl::SetEnabled(BOOL bEnabled) {
    if (m_bEnabled == bEnabled) return;
    m_bEnabled = bEnabled;
    if (m_hWnd) ::EnableWindow(m_hWnd, bEnabled);
    SetModifiedFlag(TRUE);
    // A windowless control never sees ::EnableWindow, so this is its only
    // trigger to repaint in the grayed state.
    InvalidateControl();
}
void COleControl::SetFont(LPFONTDISP pFontDisp) {
    impl__SetFont_CFontHolder__QEAAXPEAUIFont___Z(&m_font, pFontDisp);
    SetModifiedFlag(TRUE);
    InvalidateControl();

    if (m_hWnd) {
        ::SendMessageW(m_hWnd, WM_SETFONT,
                       reinterpret_cast<WPARAM>(impl__GetFontHandle_CFontHolder__QEAAPEAUHFONT____XZ(&m_font)),
                       TRUE);
    }
}
void COleControl::SetFont(CFont* pFont) {
    impl__SetFont_CFontHolder__QEAAXPEAUIFont___Z(&m_font,
                                                  pFont ? pFont->GetSafeHandle() : nullptr);
    SetModifiedFlag(TRUE);
    InvalidateControl();

    if (m_hWnd) {
        ::SendMessageW(m_hWnd, WM_SETFONT,
                       reinterpret_cast<WPARAM>(impl__GetFontHandle_CFontHolder__QEAAPEAUHFONT____XZ(&m_font)),
                       TRUE);
    }
}
unsigned int COleControl::GetHwnd() {
    if (!m_bInPlaceActive && !m_bOpen) return 0;
    return (unsigned int)(uintptr_t)m_hWnd;
}
void COleControl::SetHwnd(HWND hWnd) { m_hWnd = hWnd; }
OLE_COLOR COleControl::GetBackColorOle() const { return (OLE_COLOR)GetBackColor(); }
OLE_COLOR COleControl::GetForeColorOle() const { return (OLE_COLOR)GetForeColor(); }
void COleControl::SetBackColorOle(OLE_COLOR clr) { SetBackColor((COLORREF)clr); }
void COleControl::SetForeColorOle(OLE_COLOR clr) { SetForeColor((COLORREF)clr); }
short COleControl::GetAppearance() const {
    return m_sAppearance;
}
void COleControl::SetAppearance(short nAppearance) {
    if (m_sAppearance == nAppearance) return;
    m_sAppearance = nAppearance;
    SetModifiedFlag(TRUE);
    InvalidateControl();
}
short COleControl::GetBorderStyle() const {
    return m_sBorderStyle;
}
void COleControl::SetBorderStyle(short nBorderStyle) {
    if (m_sBorderStyle == nBorderStyle) return;
    m_sBorderStyle = nBorderStyle;
    SetModifiedFlag(TRUE);
    InvalidateControl();
}
wchar_t* COleControl::GetText() {
    return const_cast<wchar_t*>(m_strText.GetString());
}
const CString& COleControl::InternalGetText() { return m_strText; }
void COleControl::SetText(const wchar_t* lpszText) {
    CString newText = lpszText ? lpszText : L"";
    if (m_strText == newText) return;
    m_strText = newText;
    if (OleControlState* state = GetOleControlState(this, true)) {
        state->text = newText;
    }
    SetModifiedFlag(TRUE);
    InvalidateControl();
}
void COleControl::GetText(CString& strText) const { strText = m_strText; }
long COleControl::GetReadyState() const {
    return m_lReadyState;
}
BOOL COleControl::IsSubclassedControl() {
    return m_hWnd != nullptr && MfcSiteOf(this) == nullptr;
}
void COleControl::SetModifiedFlag(BOOL bModified) {
    m_bModified = bModified;
}
BOOL COleControl::GetModifiedFlag() const {
    return m_bModified;
}
ULONG COleControl::InternalAddRef() { return 1; }
ULONG COleControl::InternalRelease() { return 1; }
ULONG COleControl::InternalQueryInterface(REFIID riid, void** ppv) {
    if (!ppv) return E_POINTER;
    *ppv = nullptr;
    if (MfcSiteOf(this) && MfcSiteOf(this)->m_lpObject) {
        return MfcSiteOf(this)->m_lpObject->QueryInterface(riid, ppv);
    }
    return E_NOINTERFACE;
}
void COleControl::GetControlSize(int* pCX, int* pCY) {
    if (pCX) *pCX = m_cxExtent;
    if (pCY) *pCY = m_cyExtent;
}
BOOL COleControl::SetControlSize(int cx, int cy) {
    SIZE size = { cx, cy };
    if (OnSetExtent(DVASPECT_CONTENT, size)) {
        SetModifiedFlag(TRUE);
        return TRUE;
    }
    return FALSE;
}
void COleControl::OnSetClientSite() {
    COleControlSite* site = MfcSiteOf(this);
    m_bInitialized = (site != nullptr) ? 1u : 0u;
    if (OleControlState* state = GetOleControlState(this, true)) {
        state->container = site ? site->GetContainer() : nullptr;
    }
}
void COleControl::OnGetControlInfo(LPCONTROLINFO pControlInfo) {
    if (!pControlInfo) return;
    memset(pControlInfo, 0, sizeof(*pControlInfo));
    pControlInfo->cb = sizeof(*pControlInfo);

    IOleControl* control = QueryOleControlInterface<IOleControl>(this, IID_IOleControl);
    if (control) {
        CONTROLINFO info = {};
        info.cb = sizeof(info);
        if (SUCCEEDED(control->GetControlInfo(&info))) {
            *pControlInfo = info;
        }
        control->Release();
    }
}
BOOL COleControl::OnMnemonic(LPMSG pMsg) {
    if (!pMsg) return FALSE;
    IOleControl* control = QueryOleControlInterface<IOleControl>(this, IID_IOleControl);
    if (!control) return FALSE;
    HRESULT hr = control->OnMnemonic(pMsg);
    control->Release();
    return SUCCEEDED(hr) ? TRUE : FALSE;
}
void COleControl::OnAmbientPropertyChange(DISPID dispid) {
    OleControlState* state = GetOleControlState(this, true);
    if (!state) return;
    if (dispid == DISPID_AMBIENT_BACKCOLOR || dispid == DISPID_UNKNOWN) {
        state->backColor = AmbientBackColor();
        InvalidateControl();
    }
    if (dispid == DISPID_AMBIENT_FORECOLOR || dispid == DISPID_UNKNOWN) {
        state->foreColor = AmbientForeColor();
        InvalidateControl();
    }
}
void COleControl::BoundPropertyChanged(DISPID dispid) {
    OleControlState* state = GetOleControlState(this, false);
    if (state) {
        for (auto* sink : state->propSinks) {
            if (sink) sink->OnChanged(dispid);
        }
    }
    SetModifiedFlag(TRUE);
    InvalidateControl();
}
BOOL COleControl::BoundPropertyRequestEdit(DISPID dispid) {
    OleControlState* state = GetOleControlState(this, false);
    if (!state) return TRUE;
    for (auto* sink : state->propSinks) {
        if (sink && FAILED(sink->OnRequestEdit(dispid))) return FALSE;
    }
    return TRUE;
}
void COleControl::InvalidateControl(LPCRECT lpRect, BOOL bErase) {
    if (m_hWnd) ::InvalidateRect(m_hWnd, lpRect, bErase);
}
int COleControl::OnProperties(MSG* pMsg, HWND hWnd, const RECT* lpRect) {
    (void)pMsg; (void)lpRect;
    if (COleControlSite* site = MfcSiteOf(this)) {
        site->ShowPropertyFrame();
        return TRUE;
    }
    HWND oldWnd = m_hWnd;
    if (!m_hWnd && hWnd) m_hWnd = hWnd;
    int count = GetPropertyPageCount();
    if (count > 0) ShowPropertyPages();
    if (!oldWnd && hWnd) m_hWnd = oldWnd;
    return count > 0 ? TRUE : FALSE;
}
void COleControl::ShowPropertyPages() {
    if (!m_hWnd) return;
    ISpecifyPropertyPages* pSPP = nullptr;
    if (FAILED(InternalQueryInterface(IID_ISpecifyPropertyPages, reinterpret_cast<void**>(&pSPP))) || !pSPP)
        return;
    CAUUID pages = {};
    if (SUCCEEDED(pSPP->GetPages(&pages)) && pages.cElems > 0) {
        IUnknown* pUnk = nullptr;
        InternalQueryInterface(IID_IUnknown, reinterpret_cast<void**>(&pUnk));
        OleCreatePropertyFrame(m_hWnd, 0, 0, nullptr,
                               pUnk ? 1u : 0u, pUnk ? &pUnk : nullptr,
                               pages.cElems, pages.pElems,
                               LOCALE_USER_DEFAULT, 0, nullptr);
        if (pUnk) pUnk->Release();
        CoTaskMemFree(pages.pElems);
    }
    pSPP->Release();
}
int COleControl::GetPropertyPageCount() const {
    IUnknown* unknown = nullptr;
    if (MfcSiteOf(this) && MfcSiteOf(this)->m_lpObject) {
        unknown = MfcSiteOf(this)->m_lpObject;
        unknown->AddRef();
    } else {
        const_cast<COleControl*>(this)->InternalQueryInterface(IID_IUnknown, reinterpret_cast<void**>(&unknown));
    }
    if (!unknown) return 0;

    ISpecifyPropertyPages* pages = nullptr;
    HRESULT hr = unknown->QueryInterface(IID_ISpecifyPropertyPages, reinterpret_cast<void**>(&pages));
    unknown->Release();
    if (FAILED(hr) || !pages) return 0;
    CAUUID cauuid = {};
    int count = 0;
    if (SUCCEEDED(pages->GetPages(&cauuid))) {
        count = static_cast<int>(cauuid.cElems);
        CoTaskMemFree(cauuid.pElems);
    }
    pages->Release();
    return count;
}
BOOL COleControl::IsPropertyPage(LPUNKNOWN lpUnk) {
    if (!lpUnk) return FALSE;
    IPropertyPage* page = nullptr;
    HRESULT hr = lpUnk->QueryInterface(IID_IPropertyPage, reinterpret_cast<void**>(&page));
    if (page) page->Release();
    return SUCCEEDED(hr) ? TRUE : FALSE;
}
BOOL COleControl::CanCreateConnectionPoints() { return TRUE; }
void COleControl::EnableConnectionPoints() {
    OleControlState* state = GetOleControlState(this, true);
    if (!state) return;
    IConnectionPointContainer* container = nullptr;
    IUnknown* unknown = nullptr;
    if (MfcSiteOf(this) && MfcSiteOf(this)->m_lpObject) {
        unknown = MfcSiteOf(this)->m_lpObject;
        unknown->AddRef();
    } else {
        InternalQueryInterface(IID_IUnknown, reinterpret_cast<void**>(&unknown));
    }
    if (!unknown) return;
    HRESULT hr = unknown->QueryInterface(IID_IConnectionPointContainer, reinterpret_cast<void**>(&container));
    unknown->Release();
    if (FAILED(hr) || !container) return;

    IEnumConnectionPoints* enumPoints = nullptr;
    if (SUCCEEDED(container->EnumConnectionPoints(&enumPoints)) && enumPoints) {
        IConnectionPoint* point = nullptr;
        ULONG fetched = 0;
        while (enumPoints->Next(1, &point, &fetched) == S_OK && fetched == 1) {
            IID iid = IID_NULL;
            if (SUCCEEDED(point->GetConnectionInterface(&iid))) {
                bool exists = std::any_of(state->enabledConnectionPoints.begin(), state->enabledConnectionPoints.end(),
                    [&iid](const IID& value) { return IsEqualIID(value, iid); });
                if (!exists) state->enabledConnectionPoints.push_back(iid);
            }
            point->Release();
        }
        enumPoints->Release();
    }
    container->Release();
}
BOOL COleControl::IsConnectionPointEnabled(REFIID riid) {
    OleControlState* state = GetOleControlState(this, false);
    if (!state) return FALSE;
    for (const IID& iid : state->enabledConnectionPoints) {
        if (IsEqualIID(iid, riid)) return TRUE;
    }
    for (auto& sink : state->eventSinks) {
        if (IsEqualIID(sink.iid, riid)) return TRUE;
    }
    return FALSE;
}
void COleControl::FirePropChanged(DISPID dispid) {
    BoundPropertyChanged(dispid);
}
BOOL COleControl::PreTranslateMessage(MSG* pMsg) {
    if (!pMsg) return FALSE;
    IOleInPlaceActiveObject* activeObject = QueryOleControlInterface<IOleInPlaceActiveObject>(this, IID_IOleInPlaceActiveObject);
    if (!activeObject) return FALSE;
    HRESULT hr = activeObject->TranslateAccelerator(pMsg);
    activeObject->Release();
    return hr == S_OK ? TRUE : FALSE;
}
LONG COleControl::OnPosRectChange(LPCRECT lprcPosRect) {
    if (!lprcPosRect) return E_POINTER;
    RECT clipRect = *lprcPosRect;
    OleControlState* state = GetOleControlState(this, true);
    if (state && state->hasObjectRects) {
        clipRect = state->clipRect;
    }
    return OnSetObjectRects(lprcPosRect, &clipRect) ? S_OK : E_FAIL;
}
BOOL COleControl::OnSetObjectRects(LPCRECT lprcPosRect, LPCRECT lprcClipRect) {
    if (!lprcPosRect) return FALSE;
    RECT clipRect = lprcClipRect ? *lprcClipRect : *lprcPosRect;
    OleControlState* state = GetOleControlState(this, true);
    if (state) {
        state->posRect = *lprcPosRect;
        state->clipRect = clipRect;
        state->hasObjectRects = TRUE;
    }
    COleControlSite* ipSite = MfcSiteOf(this);
    if (ipSite && ipSite->m_lpInPlaceObject &&
        SUCCEEDED(ipSite->m_lpInPlaceObject->SetObjectRects(lprcPosRect, &clipRect))) {
        return TRUE;
    }
    IOleInPlaceObject* inPlace = QueryOleControlInterface<IOleInPlaceObject>(this, IID_IOleInPlaceObject);
    if (!inPlace) return FALSE;
    HRESULT hr = inPlace->SetObjectRects(lprcPosRect, &clipRect);
    inPlace->Release();
    return SUCCEEDED(hr) ? TRUE : FALSE;
}
void COleControl::OnClose(DWORD dwSaveOption) {
    m_bOpen = FALSE;
    m_bInPlaceActive = FALSE;
    if (dwSaveOption == OLECLOSE_NOSAVE) {
        SetModifiedFlag(FALSE);
    }
}
CWnd* COleControl::SetCapture() {
    HWND previous = m_hWnd ? ::SetCapture(m_hWnd) : nullptr;
    return previous ? CWnd::FromHandle(previous) : nullptr;
}
BOOL COleControl::ReleaseCapture() { return ::ReleaseCapture(); }
void COleControl::BringWindowToTop() { if(m_hWnd) ::BringWindowToTop(m_hWnd); }
void COleControl::MoveWindow(int X, int Y, int nWidth, int nHeight, BOOL bRepaint) { if(m_hWnd) ::MoveWindow(m_hWnd, X, Y, nWidth, nHeight, bRepaint); }
void COleControl::MoveWindow(LPCRECT lpRect, BOOL bRepaint) { if(m_hWnd && lpRect) ::MoveWindow(m_hWnd, lpRect->left, lpRect->top, lpRect->right-lpRect->left, lpRect->bottom-lpRect->top, bRepaint); }

// === Moved from ManualThunks.cpp ===
// Symbol: ?InvalidateControl@COleControl@@QEAAXPEBUtagRECT@@H@Z
extern "C" void MS_ABI impl__InvalidateControl_COleControl__QEAAXPEBUtagRECT__H_Z(void* pThis, void* p0, void* p1) {
    (void)pThis;
    (void)p0;
    (void)p1;
}


// Symbol: ?OnSetExtent@COleControl@@UEAAHPEAUtagSIZE@@@Z
extern "C" int MS_ABI impl__OnSetExtent_COleControl__UEAAHPEAUtagSIZE___Z(void* pThis, void* p0) {
    (void)pThis;
    (void)p0;
    return 0;
}


// COleControl::AddFrameLevelUI() — retail mfc140u 0x1e54e0 (disassembled at the
// byte-identical mfc140 0x1e3450):
//     if (m_pUIActiveInfo == NULL) return;              // member 0x1a8
//     m_pInPlaceFrame->SetMenu(m_pUIActiveInfo->+0x18,  // IOleInPlaceFrame
//                              m_pUIActiveInfo->+0x20,  // vtable slot 10
//                              m_hWnd);
//     this-><virtual slot 0x3c0>();
// Symbol: ?AddFrameLevelUI@COleControl@@MEAAXXZ
extern "C" void MS_ABI impl__AddFrameLevelUI_COleControl__MEAAXXZ(COleControl* pThis)
{
    if (!pThis) return;
    if (!pThis->m_pUIActiveInfo) return;   // retail's only guard
    // TODO(clean-room): transcribed partially. The 0x28-byte OLE menu holder
    // that m_pUIActiveInfo points at is not modeled, and BuildSharedMenu in
    // this file never allocates one, so m_pUIActiveInfo is always null under
    // OpenMFC and the guard above is the whole of the reachable behaviour.
    // The SetMenu handshake and the trailing virtual-slot-0x3c0 call are not
    // reproduced.
}


// COleControl::AmbientDisplayName() — retail mfc140u 0x1ee8d0:
//     *pRet = <the shared nil string buffer>;    // empty CString in the retval
//     GetAmbientProperty(DISPID_AMBIENT_DISPLAYNAME /* 0xfffffd42 = -702 */,
//                        VT_BSTR /* 8 */, pRet);
//     return pRet;
// RDX carries the hidden return slot for the by-value CString, exactly as in
// AmbientScaleUnits above.  Retail hands the CString slot straight to the
// ambient lookup as a BSTR out-parameter; the OpenMFC C++ member does the same
// lookup and converts the BSTR to a CString explicitly.
// Symbol: ?AmbientDisplayName@COleControl@@QEAA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@XZ
extern "C" void* MS_ABI impl__AmbientDisplayName_COleControl__QEAA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__XZ(
    COleControl* pThis, void* pRet)
{
    if (!pThis || !pRet) return nullptr;
    new (pRet) CString(pThis->AmbientDisplayName());
    return pRet;
}


// COleControl::GetDC(LPCRECT lprcRect, DWORD dwFlags) — retail mfc140u 0x1e6750:
//     if ((m_bInPlaceActive && m_bInPlaceSiteWndless))     // (0x160 & 0x400400)
//                                                          //   == 0x400400
//     {
//         POINT ptOffset = { 0, 0 };
//         <virtual slot 0x4b8>(&m_rcPos, &ptOffset);
//         if (lprcRect != NULL) {
//             RECT rc = { 0,0,0,0 };
//             ::CopyRect(&rc, lprcRect);
//             ::OffsetRect(&rc, ptOffset.x, ptOffset.y);
//             lprcRect = &rc;
//         }
//         if (FAILED(m_pInPlaceSiteWndless->GetDC(lprcRect, dwFlags, &hDC)))
//             return NULL;                                 // site vtable slot 23
//         return new CWindowlessDC(hDC, ptOffset);         // 0x28 bytes, ctor
//                                                          // at mfc140 0x1e3e80
//     }
//     return CDC::FromHandle(::GetDC(m_hWnd));
// Symbol: ?GetDC@COleControl@@QEAAPEAVCDC@@PEBUtagRECT@@K@Z
extern "C" CDC* MS_ABI impl__GetDC_COleControl__QEAAPEAVCDC__PEBUtagRECT__K_Z(
    COleControl* pThis, const RECT* lprcRect, unsigned long dwFlags)
{
    if (!pThis) return nullptr;
    if (pThis->m_bInPlaceActive && pThis->m_bInPlaceSiteWndless) {
        // TODO(clean-room): transcribed partially. The windowless branch hands
        // back MFC's internal CWindowlessDC wrapper, a class OpenMFC does not
        // model, so it is not reproduced; returning null is the failure result
        // retail produces when the site refuses the DC.
        (void)lprcRect;
        (void)dwFlags;
        return nullptr;
    }
    return impl__FromHandle_CDC__SAPEAV1_PEAUHDC_____Z(::GetDC(pThis->m_hWnd));
}


// COleControl::GetDispatchIID(IID* pIID) — retail mfc140u 0x1dfd10:
//     if (m_piidPrimary) *pIID = *m_piidPrimary;   // 16-byte movups/movdqu
//     return m_piidPrimary != NULL;
// Symbol: ?GetDispatchIID@COleControl@@MEAAHPEAU_GUID@@@Z
extern "C" int MS_ABI impl__GetDispatchIID_COleControl__MEAAHPEAU_GUID___Z(
    COleControl* pThis, GUID* pIID)
{
    if (!pThis) return FALSE;
    // Deviation: retail does not test pIID before writing through it.
    if (pThis->m_piidPrimary && pIID) *pIID = *pThis->m_piidPrimary;
    return pThis->m_piidPrimary != nullptr;
}


// COleControl::GetEventMap() const — retail mfc140u 0x1e3560 is
//     lea rax,[rip+0x15ad69] ; ret     -> &<static AFX_EVENTMAP at rva 0x33e2d0>
// COleControl::GetThisEventMap() shares that exact body (same RVA).  The layout
// the consumers in this file use is { pfnGetBaseMap @0, lpEntries @8,
// lpStockEventMask @0x10 } -- see GetEventMapEntry and InitStockEventMask below.
// Symbol: ?GetEventMap@COleControl@@MEBAPEBUAFX_EVENTMAP@@XZ
extern "C" const void* MS_ABI impl__GetEventMap_COleControl__MEBAPEBUAFX_EVENTMAP__XZ(
    const COleControl* /*pThis*/)
{
    // Reproduced from retail: the 24 bytes at mfc140u rva 0x33e2d0 are all
    // zero, so COleControl's own map is { NULL, NULL, NULL } -- no base map
    // (COleControl is the root of the event-map chain), no entries, and no
    // stock-event-mask word.  Returning it rather than NULL is what lets a
    // derived control's BEGIN_EVENT_MAP chain terminate the way retail's does.
    return &g_eventMap_COleControl;
}


// COleControl::GetEventMapEntry(LPCTSTR lpszName, DISPID* pDispid) const —
// retail mfc140u 0x1e3570 (disassembled at the identical mfc140 0x1e1490):
//     const AFX_EVENTMAP* pMap = GetEventMap();     // virtual slot 0x4d0
//     DWORD dwIndex = 1;                            // low word = entry index,
//                                                   // high word = map depth
//     for (;;) {
//         for (pEntry = pMap->lpEntries; pEntry->pszName != NULL;
//              pEntry = (const BYTE*)pEntry + 0x18, ++dwIndex)
//             if (lstrcmp(pEntry->pszName, lpszName) == 0) {
//                 DISPID id = pEntry->dispid;       // entry + 4
//                 *pDispid = (id == -1) ? dwIndex : id;
//                 return pEntry;
//             }
//         if (pMap->pfnGetBaseMap == NULL) return NULL;
//         pMap = pMap->pfnGetBaseMap();
//         dwIndex = ((((dwIndex >> 16) + 1) & 0xffff) << 16) | 1;
//     }
// The walk itself is transcribed below over the EventMap/EventMapEntry shapes
// declared at the top of this file (afxctl.h's AFX_EVENTMAP*, confirmed against
// the strides and field offsets the disassembly uses: entries advance by 0x18,
// pszName is read at +0x08 and dispid at +0x04).  Retail compares names with
// KERNEL32 lstrcmpW; ::lstrcmpW is used here for the same reason.
//
// DEVIATION, and it is what makes this body a no-op today: retail seeds the
// walk from the *virtual* GetEventMap (slot 0x4d0), so a derived control's map
// is what gets walked.  OpenMFC's COleControl does not declare GetEventMap, so
// there is no virtual to dispatch through and this code can only seed from
// COleControl's own map -- which retail itself defines as all-NULL.  The loop
// therefore always falls out at the first `pMap->lpEntries == NULL` test and
// returns NULL, exactly as the previous stub did.
// Two smaller, deliberate deviations on the found path: retail writes *pDispid
// with no null check (`mov %ecx,(%r14)`) and calls lstrcmpW with whatever
// lpszName it was handed; the body below guards both.
// Symbol: ?GetEventMapEntry@COleControl@@IEBAPEBUAFX_EVENTMAP_ENTRY@@PEB_WPEAJ@Z
extern "C" const void* MS_ABI impl__GetEventMapEntry_COleControl__IEBAPEBUAFX_EVENTMAP_ENTRY__PEB_WPEAJ_Z(
    const COleControl* pThis, const wchar_t* lpszName, long* pDispid)
{
    if (!lpszName) return nullptr;
    const EventMap* pMap = static_cast<const EventMap*>(
        impl__GetEventMap_COleControl__MEBAPEBUAFX_EVENTMAP__XZ(pThis));
    unsigned int dwIndex = 1;   // low word = entry index, high word = map depth
    while (pMap != nullptr) {
        for (const EventMapEntry* pEntry = pMap->lpEntries;
             pEntry != nullptr && pEntry->pszName != nullptr; ++pEntry, ++dwIndex) {
            if (::lstrcmpW(pEntry->pszName, lpszName) == 0) {
                long id = pEntry->dispid;
                if (pDispid) *pDispid = (id == -1) ? (long)dwIndex : id;
                return pEntry;
            }
        }
        if (pMap->pfnGetBaseMap == nullptr) break;
        pMap = pMap->pfnGetBaseMap();
        dwIndex = ((((dwIndex >> 16) + 1) & 0xffffu) << 16) | 1u;
    }
    return nullptr;   // not found: retail leaves *pDispid untouched here too
}


// COleControl::GetExtendedControl() — retail mfc140u 0x1e1760:
//     IDispatch* pDisp = NULL;
//     if (m_pControlSite)                                  // member 0x1e8
//         m_pControlSite->GetExtendedControl(&pDisp);       // vtable slot 5
//     return pDisp;
// (The retail body ignores the HRESULT and returns whatever the out-parameter
// holds, which stays NULL when there is no site.)
// Symbol: ?GetExtendedControl@COleControl@@QEAAPEAUIDispatch@@XZ
extern "C" IDispatch* MS_ABI impl__GetExtendedControl_COleControl__QEAAPEAUIDispatch__XZ(
    COleControl* pThis)
{
    if (!pThis) return nullptr;
    void* pSite = pThis->m_pControlSite;
    if (!pSite) return nullptr;
    IDispatch* pDisp = nullptr;
    void** vft = *static_cast<void***>(pSite);
    auto* fn = reinterpret_cast<HRESULT (STDMETHODCALLTYPE*)(void*, IDispatch**)>(vft[5]);
    fn(pSite, &pDisp);
    return pDisp;
}


// COleControl::GetExtraConnectionPoints(CPtrArray* pConnPoints) — retail
// mfc140u 0x1df050 (disassembled at the identical mfc140 0x1dcf80):
//     pConnPoints->SetAtGrow(pConnPoints->m_nSize /* +0x10 */,
//                            (BYTE*)this + 0x328);
//     return TRUE;
// Note the offset: retail publishes the sub-object at this+0x328, which is not
// a member boundary in afxole.h's model (m_xEventConnPt @0x2D0 spanning 96
// bytes, m_xPropConnPt @0x330) -- +0x328 lands 8 bytes before the end of
// m_xEventConnPt.  The connection *map* (see GetConnectionMap above) names
// +0x330 for the property-notify point, so +0x328 is very likely the event
// connection point.  Two readings fit that evidence equally well and the
// disassembly does not separate them: either afxole.h's 0x2D0 boundary is
// wrong, or the interface retail publishes is a base sub-object sitting at
// +0x58 inside the 96-byte m_xEventConnPt field.  Do not "fix" the header on
// the strength of this comment; the offsets there are asserted in
// phase4/src/detail/Ole6ImplSupport.cpp.
// Symbol: ?GetExtraConnectionPoints@COleControl@@MEAAHPEAVCPtrArray@@@Z
extern "C" int MS_ABI impl__GetExtraConnectionPoints_COleControl__MEAAHPEAVCPtrArray___Z(
    COleControl* /*pThis*/, CPtrArray* /*pConnPoints*/)
{
    // TODO(clean-room): deliberately left a stub. Publishing a pointer into
    // storage OpenMFC never initialises would hand the container an object it
    // will QueryInterface through; returning FALSE ("no extra connection
    // points") is the safe answer. Retail returns TRUE.
    return FALSE;
}


// COleControl::GetFocus() — retail mfc140u 0x1e66f0:
//     if (m_bInPlaceActive && m_bInPlaceSiteWndless)   // (0x160 & 0x400400)
//                                                      //   == 0x400400
//         return m_pInPlaceSiteWndless->GetFocus() == S_OK ? this : NULL;
//                                                      // site vtable slot 21
//     return CWnd::FromHandle(::GetFocus());
// Symbol: ?GetFocus@COleControl@@QEAAPEAVCWnd@@XZ
extern "C" CWnd* MS_ABI impl__GetFocus_COleControl__QEAAPEAVCWnd__XZ(COleControl* pThis)
{
    if (!pThis) return nullptr;
    if (pThis->m_bInPlaceActive && pThis->m_bInPlaceSiteWndless) {
        void* pSite = pThis->m_pInPlaceSiteWndless;
        if (!pSite) return nullptr;
        void** vft = *static_cast<void***>(pSite);
        auto* fn = reinterpret_cast<HRESULT (STDMETHODCALLTYPE*)(void*)>(vft[21]);
        return fn(pSite) == S_OK ? pThis : nullptr;
    }
    return impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(::GetFocus());
}


// COleControl::GetFont() — retail mfc140u 0x1ee220 is a two-instruction tail
// call:  add rcx,0x178 ; jmp CFontHolder::GetFontDispatch
// i.e. return m_font.GetFontDispatch();   (m_font is at 0x178)
// Symbol: ?GetFont@COleControl@@QEAAPEAUIFontDisp@@XZ
extern "C" void* MS_ABI impl__GetFont_COleControl__QEAAPEAUIFontDisp__XZ(
    COleControl* pThis)
{
    if (!pThis) return nullptr;
    return impl__GetFontDispatch_CFontHolder__QEAAPEAUIFontDisp__XZ(&pThis->m_font);
}


// COleControl::GetFontTextMetrics(LPTEXTMETRIC lptm, CFontHolder& fontHolder) —
// retail mfc140u 0x1ee580 is a two-instruction tail call:
//     mov rcx,r8 ; jmp CFontHolder::QueryTextMetrics
// `this` is discarded outright: the metrics come from the caller's holder, not
// from the control's own m_font.
// Symbol: ?GetFontTextMetrics@COleControl@@QEAAXPEAUtagTEXTMETRICW@@AEAVCFontHolder@@@Z
extern "C" void MS_ABI impl__GetFontTextMetrics_COleControl__QEAAXPEAUtagTEXTMETRICW__AEAVCFontHolder___Z(
    COleControl* /*pThis*/, TEXTMETRICW* lptm, CFontHolder* pFontHolder)
{
    if (!lptm || !pFontHolder) return;
    impl__QueryTextMetrics_CFontHolder__QEAAXPEAUtagTEXTMETRICW___Z(pFontHolder, lptm);
}


// COleControl::GetInterfaceHook(const void* pv) — retail mfc140u 0x1dfc30
// (disassembled at the identical mfc140 0x1ddb60).  The whole body, which is
// only two branches long:
//     if (m_piidPrimary != NULL &&
//         memcmp(pv, m_piidPrimary, 16) == 0)          // four 32-bit compares
//         return CCmdTarget::GetInterface(&IID_IDispatch);   // tail jmp to
//                                                            // u 0x26ce40,
//                                                            // rdx = the
//                                                            // IID_IDispatch
//                                                            // GUID at rdata
//                                                            // 0x2d9b48
//     if (memcmp(pv, &IID_IPointerInactive, 16) == 0)   // GUID at rdata
//                                                       // 0x2d9e08 =
//                                                       // {55980BA0-35AA-11CF-
//                                                       //  B671-00AA004CD6D8}
//         return (<virtual slot 0x478>() & 4) ? (IUnknown*)(this + 0x2c8)
//                                             : NULL;   // m_xPointerInactive
//     return NULL;
// The +0x2c8 sub-object is m_xPointerInactive: XPointerInactive::
// GetActivationPolicy (u 0x1e63b0) opens with `add rcx,-0x2c8` to recover the
// control.  That same thunk calls virtual slot 0x480, so slot 0x480 -- not
// 0x478 -- is COleControl::GetActivationPolicy; slot 0x478 is a different
// no-argument DWORD virtual whose bit 2 gates IPointerInactive (MFC documents
// GetControlFlags()'s `pointerInactive` flag as 4, which fits, but that
// identification is inference, not something read out of the binary: COleControl
// is abstract, so it emits no vtable to resolve the slot against).
// Symbol: ?GetInterfaceHook@COleControl@@MEAAPEAUIUnknown@@PEBX@Z
extern "C" IUnknown* MS_ABI impl__GetInterfaceHook_COleControl__MEAAPEAUIUnknown__PEBX_Z(
    COleControl* pThis, const void* pv)
{
    if (!pThis || !pv) return nullptr;
    const IID& iid = *static_cast<const IID*>(pv);

    // Branch 1, transcribed: retail compares the four dwords of *piid against
    // *m_piidPrimary (this+0xE8) and, on a match, tail-jumps to
    // CCmdTarget::GetInterface(this, &IID_IDispatch) -- 0x18026ce40 with rdx
    // pointing at the 16 bytes 00 04 02 00 00 00 00 00 c0 00 00 00 00 00 00 46
    // at rva 0x2d9b48.  CCmdTarget::GetInterface is a real implementation in
    // this DLL (phase4/src/core/runtime/CCmdTarget.cpp).
    if (pThis->m_piidPrimary != nullptr && ::IsEqualGUID(iid, *pThis->m_piidPrimary)) {
        static const IID kIID_IDispatch =
            { 0x00020400, 0x0000, 0x0000, { 0xC0, 0, 0, 0, 0, 0, 0, 0x46 } };
        return impl__GetInterface_CCmdTarget__QEAAPEAUIUnknown__PEBX_Z(
            static_cast<CCmdTarget*>(pThis), &kIID_IDispatch);
    }

    // Branch 2, transcribed but deliberately not completed:
    //     if (*piid == IID_IPointerInactive /* rva 0x2d9e08 */)
    //         return (GetControlFlags() & 4) ? (IUnknown*)(this + 0x2C8) : NULL;
    // where slot 0x478 is GetControlFlags (afxctl.h declares IsSubclassedControl,
    // ReparentControlWindow, GetControlFlags consecutively and the retail slots
    // 0x468/0x470/0x478 line up one-for-one), 4 is COleControl's pointerInactive
    // flag, and this+0x2C8 is afxole.h's m_xPointerInactive.
    // COleControl::GetControlFlags itself is `mov eax,2 ; ret` (mfc140u 0xf3790),
    // so for a base control retail already returns NULL here.  A derived control
    // that sets the pointerInactive bit would get &m_xPointerInactive, which
    // OpenMFC never initialises -- handing that out would give the container a
    // vtable-less object to QueryInterface through, so NULL is returned instead.
    // (Same hazard, same decision as GetExtraConnectionPoints below.)
    return nullptr;
}


// COleControl::GetMessageString(UINT nID, CString& rMessage) const — retail
// mfc140u 0x1e2460 (disassembled at the identical mfc140 0x1e0390):
//     LPTSTR psz = rMessage.GetBuffer(255);
//     if (AfxLoadString(nID, psz, 256) != 0) {
//         LPTSTR p = _tcschr(psz, '\n');
//         if (p) *p = '\0';                 // truncate at the first newline
//     }
//     rMessage.ReleaseBuffer();             // relength with strnlen
// Symbol: ?GetMessageString@COleControl@@UEBAXIAEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@@Z
extern "C" void MS_ABI impl__GetMessageString_COleControl__UEBAXIAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
    const COleControl* /*pThis*/, unsigned int nID, CString* pMessage)
{
    if (!pMessage) return;
    wchar_t* psz = pMessage->GetBuffer(255);
    if (!psz) return;
    // Deviation: retail leaves the raw buffer untouched before the load and
    // relies on AfxLoadString to terminate it. The explicit terminator here
    // keeps the ReleaseBuffer() relength safe if the load writes nothing.
    psz[0] = L'\0';
    if (impl__AfxLoadString__YAHIPEA_WI_Z(nID, psz, 256) != 0) {
        wchar_t* p = ::wcschr(psz, L'\n');
        if (p) *p = L'\0';
    }
    pMessage->ReleaseBuffer();
}


// COleControl::GetMetafileData(LPFORMATETC, LPSTGMEDIUM) — retail mfc140u
// 0x1e0620 (disassembled at the identical mfc140 0x1de550).  Verified shape:
//     if (!(lpFormatEtc->tymed & TYMED_MFPICT /*0x20*/)) return FALSE;
//     if (lpStgMedium->u.hGlobal != NULL) return FALSE;
//     CMetaFileDC dc; dc.Create(NULL);          // CreateMetaFile import
//     ... set the map mode from the control extent, run the control's draw
//     path into the metafile DC, close it and package the result as an
//     HGLOBAL METAFILEPICT in lpStgMedium ...
// Symbol: ?GetMetafileData@COleControl@@IEAAHPEAUtagFORMATETC@@PEAUtagSTGMEDIUM@@@Z
extern "C" int MS_ABI impl__GetMetafileData_COleControl__IEAAHPEAUtagFORMATETC__PEAUtagSTGMEDIUM___Z(
    COleControl* /*pThis*/, FORMATETC* /*lpFormatEtc*/, STGMEDIUM* /*lpStgMedium*/)
{
    // TODO(clean-room): transcribed partially. The body drives CMetaFileDC and
    // the control's rendering path, neither of which OpenMFC models here.
    // FALSE ("format not rendered") is the answer retail gives on its own
    // early-out paths and leaves lpStgMedium untouched.
    return FALSE;
}


// COleControl::GetNotSupported() — retail mfc140u 0x1eeca0:
//     ThrowError(CTL_E_GETNOTSUPPORTED /* 0x800a018a */,
//                AFX_IDP_E_GETNOTSUPPORTED /* 0xfec0 */, (UINT)-1);
// It is a plain call followed by int3 padding, i.e. ThrowError never returns.
// Symbol: ?GetNotSupported@COleControl@@QEAAXXZ
extern "C" void MS_ABI impl__GetNotSupported_COleControl__QEAAXXZ(COleControl* /*pThis*/)
{
    // Retail mfc140u 0x1eeca0 is a four-instruction tail call:
    //     ThrowError(0x800A018A, 0xFEC0, (UINT)-1);   // 0x1e29b0, then int3
    // and COleControl::ThrowError(SCODE, UINT nDescriptionID, UINT nHelpID) at
    // 0x1e29b0 does AfxLoadString(nDescriptionID, buf, 256) and forwards to the
    // string-form ThrowError with nHelpID, or with nDescriptionID when nHelpID
    // is (UINT)-1 (`cmp edi,-1 ; cmovne ebx,edi`), which allocates a 0x40-byte
    // exception object and throws it -- the call never returns.
    // TODO(clean-room): still a no-op deliberately. OpenMFC's
    // ?ThrowError@COleControl@@QEAAXJII@Z export (further down this file) is
    // itself an empty stub whose declared signature omits `this`, so calling it
    // would neither throw nor be ABI-correct. Returning normally is wrong
    // relative to retail (which throws) but is the least-damaging stand-in.
}


// COleControl::GetPropsetData(LPFORMATETC, LPSTGMEDIUM, REFGUID fmtid) — retail
// mfc140u 0x1f1430 (disassembled at the identical mfc140 0x1ef190).  Verified
// shape: it rejects null lpFormatEtc / lpStgMedium, requires
// lpFormatEtc->tymed to carry TYMED_ISTORAGE|TYMED_ISTREAM (mask 0xc), reuses a
// caller-supplied medium when one is present and otherwise builds one with
// CreateILockBytesOnHGlobal + StgCreateDocfileOnILockBytes, then writes the
// control's property set into it.
// Symbol: ?GetPropsetData@COleControl@@IEAAHPEAUtagFORMATETC@@PEAUtagSTGMEDIUM@@AEBU_GUID@@@Z
extern "C" int MS_ABI impl__GetPropsetData_COleControl__IEAAHPEAUtagFORMATETC__PEAUtagSTGMEDIUM__AEBU_GUID___Z(
    COleControl* /*pThis*/, FORMATETC* /*lpFormatEtc*/,
    STGMEDIUM* /*lpStgMedium*/, const GUID* /*pFmtid*/)
{
    // TODO(clean-room): transcribed partially. The property-set serialisation
    // it performs depends on the CPropExchange machinery that OpenMFC leaves
    // stubbed (see ExchangeStockProps above). FALSE leaves lpStgMedium alone.
    return FALSE;
}


// COleControl::GetStockTextMetrics(LPTEXTMETRIC lptm) — retail mfc140u 0x1ee570
// is a two-instruction tail call:  add rcx,0x178 ; jmp
// CFontHolder::QueryTextMetrics, i.e. m_font.QueryTextMetrics(lptm).
// Symbol: ?GetStockTextMetrics@COleControl@@QEAAXPEAUtagTEXTMETRICW@@@Z
extern "C" void MS_ABI impl__GetStockTextMetrics_COleControl__QEAAXPEAUtagTEXTMETRICW___Z(
    COleControl* pThis, TEXTMETRICW* lptm)
{
    if (!pThis || !lptm) return;
    impl__QueryTextMetrics_CFontHolder__QEAAXPEAUtagTEXTMETRICW___Z(&pThis->m_font, lptm);
}


// COleControl::GetThisConnectionMap() — retail mfc140u 0x1deff0 is
//     lea rax,[rip+...] ; ret     -> &<static map at rva 0x3232f8>
// It is the same body, at the same RVA, as the virtual GetConnectionMap()
// above; see that comment for the map's contents.
// Symbol: ?GetThisConnectionMap@COleControl@@KAPEBUAFX_CONNECTIONMAP@@XZ
extern "C" const void* MS_ABI impl__GetThisConnectionMap_COleControl__KAPEBUAFX_CONNECTIONMAP__XZ()
{
    // Same static as GetConnectionMap above -- retail ICF-folds the two bodies
    // into the single `lea rax,[rip+0x144301] ; ret` at mfc140u 0x1deff0.
    return &g_connectionMap_COleControl;
}


// COleControl::GetThisEventMap() — retail mfc140u 0x1e3560 is
//     lea rax,[rip+...] ; ret     -> &<static AFX_EVENTMAP at rva 0x33e2d0>
// Same body, same RVA, as the virtual GetEventMap() above.
// Symbol: ?GetThisEventMap@COleControl@@KAPEBUAFX_EVENTMAP@@XZ
extern "C" const void* MS_ABI impl__GetThisEventMap_COleControl__KAPEBUAFX_EVENTMAP__XZ()
{
    // Same static as GetEventMap above -- retail ICF-folds the two bodies into
    // the single `lea rax,[rip+0x15ad69] ; ret` at mfc140u 0x1e3560.
    return &g_eventMap_COleControl;
}


// COleControl::GetUserType(LPTSTR lpszUserType) — retail mfc140u 0x1e7490:
//     *lpszUserType = 0;                             // 2-byte store, U build
//     AfxLoadString(<virtual slot 0x538>(), lpszUserType, 256);
// Slot 0x538 is a no-argument UINT virtual (the user-type-name string id); the
// 256 is a character count, and the buffer is the caller's.
// Symbol: ?GetUserType@COleControl@@IEAAXPEA_W@Z
extern "C" void MS_ABI impl__GetUserType_COleControl__IEAAXPEA_W_Z(
    COleControl* pThis, wchar_t* lpszUserType)
{
    if (!pThis || !lpszUserType) return;
    *lpszUserType = L'\0';
    // TODO(clean-room): transcribed partially. The string id comes from virtual
    // slot 0x538, which OpenMFC's COleControl does not declare, so no string is
    // loaded. Retail also writes the terminator first, so a caller that checks
    // for an empty result behaves the same way as it does when the resource is
    // missing.
}


// COleControl::IgnoreWindowMessage(UINT message, WPARAM wParam, LPARAM lParam,
// LRESULT* plResult) — retail mfc140u 0x1e1ed0 (disassembled at the identical
// mfc140 0x1dfe00):
//     if (!m_bUIDead) return FALSE;              // testl $0x20000,0x160(this)
//     if (message == WM_SETCURSOR) {
//         *plResult = ::SendMessage(::GetParent(m_hWnd), WM_SETCURSOR,
//                                   wParam, lParam);
//         return TRUE;
//     }
//     if (message == WM_NCHITTEST ||             // (message - 0x20) == 0x64
//         (unsigned)(message - WM_KEYFIRST) <= 9) {   // 0x100 .. 0x109
//         *plResult = 0;
//         return TRUE;
//     }
//     return FALSE;
// Symbol: ?IgnoreWindowMessage@COleControl@@IEAAHI_K_JPEA_J@Z
extern "C" int MS_ABI impl__IgnoreWindowMessage_COleControl__IEAAHI_K_JPEA_J_Z(
    COleControl* pThis, unsigned int message, unsigned long long wParam,
    long long lParam, long long* plResult)
{
    if (!pThis) return FALSE;
    if (!pThis->m_bUIDead) return FALSE;
    if (message == WM_SETCURSOR) {
        LRESULT lr = ::SendMessageW(::GetParent(pThis->m_hWnd), WM_SETCURSOR,
                                    static_cast<WPARAM>(wParam),
                                    static_cast<LPARAM>(lParam));
        // Deviation: retail writes through plResult unconditionally.
        if (plResult) *plResult = static_cast<long long>(lr);
        return TRUE;
    }
    if (message == WM_NCHITTEST ||
        (message >= 0x100u && message <= 0x109u)) {
        if (plResult) *plResult = 0;
        return TRUE;
    }
    return FALSE;
}


// COleControl::InitializeIIDs(const IID* piidPrimary, const IID* piidEvents) —
// retail mfc140u 0x1dfd30:
//     m_piidPrimary = piidPrimary;      // member 0x0e8
//     m_piidEvents  = piidEvents;       // member 0x0f0
//     CCmdTarget::EnableTypeLib();      // mfc140 0x26ad30, `this` in rcx
//     InitStockEventMask();             // u 0x1e34a0
//     InitStockPropMask();              // u 0x1ece40, tail call
// Symbol: ?InitializeIIDs@COleControl@@QEAAXPEBU_GUID@@0@Z
extern "C" void MS_ABI impl__InitializeIIDs_COleControl__QEAAXPEBU_GUID__0_Z(
    COleControl* pThis, const GUID* piidPrimary, const GUID* piidEvents)
{
    if (!pThis) return;
    pThis->m_piidPrimary = piidPrimary;
    pThis->m_piidEvents  = piidEvents;
    // OpenMFC's CCmdTarget::EnableTypeLib export (phase4/src/core/runtime/
    // CCmdTarget.cpp) does take `this` and is an empty stub -- the retail body
    // caches a type library on the module state, which OpenMFC does not model.
    // Retail passes the control itself; CCmdTarget is at offset 0 of
    // COleControl, so the conversion is the same address.
    impl__EnableTypeLib_CCmdTarget__QEAAXXZ(pThis);
    impl__InitStockEventMask_COleControl__IEAAXXZ(pThis);
    impl__InitStockPropMask_COleControl__IEAAXXZ(pThis);
}


// COleControl::InitStockEventMask() — retail mfc140u 0x1e34a0 (disassembled at
// the identical mfc140 0x1e13c0):
//     const AFX_EVENTMAP* pMap = GetEventMap();          // virtual slot 0x4d0
//     if (*pMap->lpStockEventMask /* +0x10 */ != (DWORD)-1) return;
//     <enter a global critical section, id 0xe>;
//     if (*pMap->lpStockEventMask != (DWORD)-1) { leave; return; }
//     DWORD dwMask = 0;
//     for (; pMap; pMap = pMap->pfnGetBaseMap())
//         for (pEntry = pMap->lpEntries; pEntry && pEntry->pszName;
//              pEntry = (const BYTE*)pEntry + 0x18)
//             if (pEntry->flags & 1) {                   // stock event entry
//                 int i = -600 /* 0xfffffda8 */ - pEntry->dispid;
//                 if ((unsigned)i < 10)
//                     dwMask |= <table at mfc140u rdata 0x34b830>[i];
//                                        (mfc140 rdata 0x345440)
//             }
//     *pMap->lpStockEventMask = dwMask;
//     <leave the critical section>;
// The 10 DWORDs at mfc140u 0x34b830, read out of .rdata, are simply
//     1, 2, 4, 8, 0x10, 0x20, 0x40, 0x80, 0x100, 0x200
// i.e. one bit per stock event in DISPID order starting at DISPID_CLICK (-600):
// Click, DblClick, KeyDown, KeyPress, KeyUp, MouseDown, MouseMove, MouseUp,
// Error, ReadyStateChange.  Slot 0x4d0 is GetEventMap: afxctl.h declares
// CreateControlWindow, SetInitialDataFormats, GetEventMap consecutively and the
// retail slots 0x4c0 / 0x4c8 / 0x4d0 match one-for-one.  The critical section
// is entered with AfxLockGlobals(0xe) (mfc140u 0x33540) and left by
// LeaveCriticalSection on the object at 0x3c3d20.
// Symbol: ?InitStockEventMask@COleControl@@IEAAXXZ
extern "C" void MS_ABI impl__InitStockEventMask_COleControl__IEAAXXZ(
    COleControl* /*pThis*/)
{
    // TODO(clean-room): AFX_EVENTMAP is not modeled by OpenMFC and GetEventMap
    // above returns null, so there is no mask word to compute or publish.
}


// COleControl::InitStockPropMask() — retail mfc140u 0x1ece40 (disassembled at
// the identical mfc140 0x1eabe0). Same shape as InitStockEventMask, but over
// the dispatch map reached through virtual slot 0x70:
//     if (*pMap->+0x18 != (DWORD)-1) return;             // cached mask word
//     <enter the global critical section, id 0xe>; re-test;
//     DWORD dwMask = 0;
//     for (; pMap; pMap = pMap->+0x0())
//         for (pEntry = pMap->lpEntries /* +8 */;
//              pEntry->+0x30 != -1; pEntry = (const BYTE*)pEntry + 0x40) {
//             int i = -501 /* 0xfffffe0b */ - pEntry->dispid /* +8 */;
//             if ((unsigned)i < 20)
//                 dwMask |= <table at mfc140u rdata 0x34bbb0>[i];
//                                            (mfc140 rdata 0x345750)
//         }
//     *pMap->+0x18 = dwMask;  <leave the critical section>;
// The 20 DWORDs at mfc140u 0x34bbb0, read out of .rdata, are sparse -- only
// eight stock properties have a mask bit, the rest are 0:
//     [0x00]=1  [0x03]=0x20  [0x0b]=4  [0x0c]=8  [0x0d]=0x40  [0x10]=0x10
//     [0x11]=2  [0x13]=0x80
// indexed by (-501 - dispid), so index 0 is DISPID_BACKCOLOR (-501) and index
// 0x13 is DISPID (-520).  Note the loop's terminator test is the *entry's*
// +0x30 field being (size_t)-1, not a NULL name, and the entry stride is 0x40.
// Symbol: ?InitStockPropMask@COleControl@@IEAAXXZ
extern "C" void MS_ABI impl__InitStockPropMask_COleControl__IEAAXXZ(
    COleControl* /*pThis*/)
{
    // TODO(clean-room): the AFX dispatch-map structures this walks are not
    // modeled by OpenMFC, so there is no mask word to compute or publish.
    // ExchangeStockProps above is the only consumer and is itself a stub.
}


// COleControl::InternalGetText() — retail mfc140u 0x1ede80 (disassembled at the
// identical mfc140 0x1ebc20):
//     if (m_hWnd != NULL && <virtual slot 0x468>() /* IsSubclassedControl */) {
//         int n = (int)<virtual slot 0x248>(WM_GETTEXTLENGTH, 0, 0) + 1;
//         if (n < 0) throw E_INVALIDARG;
//         LPTSTR psz = m_strText.GetBuffer(n);      // member 0x170
//         m_strText's length is set to n and psz[n] = 0;
//         <virtual slot 0x248>(WM_GETTEXT, n, psz);
//         m_strText.ReleaseBuffer();                // relength with strnlen
//     }
//     return m_strText;                             // lea rax,[this+0x170]
// Slot 0x248 is the DefWindowProc virtual (a subclassed control's text lives in
// the superclass window proc, which is why retail bypasses SendMessage).
// Symbol: ?InternalGetText@COleControl@@QEAAAEBV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@XZ
extern "C" const CString* MS_ABI impl__InternalGetText_COleControl__QEAAAEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__XZ(
    COleControl* pThis)
{
    if (!pThis) return nullptr;
    // Deviation: retail dispatches both the predicate and DefWindowProc through
    // the control's vtable, so a derived override is honoured. OpenMFC has no
    // vtable slot for either, so the base implementations are called directly.
    if (pThis->m_hWnd && pThis->IsSubclassedControl()) {
        LRESULT lLen = impl__DefWindowProcW_COleControl__MEAA_JI_K_J_Z(
            pThis, WM_GETTEXTLENGTH, 0, 0);
        int nLen = static_cast<int>(lLen) + 1;
        if (nLen > 0) {
            wchar_t* psz = pThis->m_strText.GetBuffer(nLen);
            if (psz) {
                psz[0] = L'\0';
                impl__DefWindowProcW_COleControl__MEAA_JI_K_J_Z(
                    pThis, WM_GETTEXT, static_cast<WPARAM>(nLen),
                    reinterpret_cast<LPARAM>(psz));
                pThis->m_strText.ReleaseBuffer();
            }
        }
    }
    return &pThis->m_strText;
}


// COleControl::InvalidateRgn(CRgn* pRgn, BOOL bErase) — retail mfc140u 0x1e6920
// (disassembled at the identical mfc140 0x1e4880):
//     if (m_bInPlaceActive && m_bInPlaceSiteWndless) {   // (0x160 & 0x400400)
//         CRgn rgnTemp;                                  // m_hObject == NULL
//         if (pRgn != NULL) {
//             POINT ptOffset = { 0, 0 };
//             <virtual slot 0x4b8>(&m_rcPos, &ptOffset);
//             ::CombineRgn(rgnTemp.m_hObject, pRgn->m_hObject, NULL, RGN_COPY);
//             ::OffsetRgn(rgnTemp.m_hObject, ptOffset.x, ptOffset.y);
//         }
//         m_pInPlaceSiteWndless->InvalidateRgn(NULL, bErase);  // site slot 26
//         rgnTemp.~CRgn();
//         return;
//     }
//     ::InvalidateRgn(m_hWnd, pRgn ? pRgn->m_hObject : NULL, bErase);
// Note the windowless branch: because rgnTemp is never given a handle, the
// compiler folded its m_hObject to a literal NULL, so both GDI calls are
// no-ops and the site is always asked to invalidate the *whole* control. That
// is transcribed verbatim below rather than "fixed".
// Symbol: ?InvalidateRgn@COleControl@@QEAAXPEAVCRgn@@H@Z
extern "C" void MS_ABI impl__InvalidateRgn_COleControl__QEAAXPEAVCRgn__H_Z(
    COleControl* pThis, CRgn* pRgn, int bErase)
{
    if (!pThis) return;
    if (pThis->m_bInPlaceActive && pThis->m_bInPlaceSiteWndless) {
        void* pSite = pThis->m_pInPlaceSiteWndless;
        if (!pSite) return;
        void** vft = *static_cast<void***>(pSite);
        auto* fn = reinterpret_cast<HRESULT (STDMETHODCALLTYPE*)(void*, HRGN, BOOL)>(vft[26]);
        fn(pSite, nullptr, bErase);
        return;
    }
    ::InvalidateRgn(pThis->m_hWnd,
                    pRgn ? static_cast<HRGN>(pRgn->GetSafeHandle()) : nullptr,
                    bErase);
}


// COleControl::OnGetColorSet(...) — the retail export shares the DLL-wide
// `xor eax,eax ; ret` body at mfc140u 0x71e0: the base control publishes no
// colour set and leaves the out-parameter untouched. Derived controls override.
// Symbol: ?OnGetColorSet@COleControl@@UEAAHPEAUtagDVTARGETDEVICE@@PEAUHDC__@@PEAPEAUtagLOGPALETTE@@@Z
extern "C" int MS_ABI impl__OnGetColorSet_COleControl__UEAAHPEAUtagDVTARGETDEVICE__PEAUHDC____PEAPEAUtagLOGPALETTE___Z(
    COleControl* /*pThis*/, void* /*ptd*/, HDC /*hicTargetDev*/,
    LOGPALETTE** /*ppColorSet*/)
{
    return FALSE;
}


// COleControl::OnGetDisplayString(DISPID dispid, CString& strValue) — retail
// mfc140u 0x1eee20 (disassembled at the identical mfc140 0x1ecba0):
//     if (dispid == DISPID_FONT /* 0xfffffe00 = -512 */)
//         return m_font.GetDisplayString(strValue);          // m_font @0x178
//     if (dispid == DISPID_BORDERSTYLE /* 0xfffffe08 = -504 */) {
//         UINT nID = 0xfe31 + (m_sBorderStyle != 0);         // m_sBorderStyle
//                                                            //   @0x19a
//         HINSTANCE h = AfxFindStringResourceHandle(nID);
//         if (h == NULL) return FALSE;
//         return strValue.LoadString(h, nID);
//     }
//     return FALSE;
// Symbol: ?OnGetDisplayString@COleControl@@UEAAHJAEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@@Z
extern "C" int MS_ABI impl__OnGetDisplayString_COleControl__UEAAHJAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
    COleControl* pThis, long dispid, CString* pValue)
{
    if (!pThis || !pValue) return FALSE;
    if (dispid == -512) {   // DISPID_FONT
        return impl__GetDisplayString_CFontHolder__QEAAHAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
            &pThis->m_font, pValue);
    }
    if (dispid == -504) {   // DISPID_BORDERSTYLE
        // Deviation: retail resolves the string through
        // AfxFindStringResourceHandle, which walks the module's resource-handle
        // chain; OpenMFC's AfxLoadString consults AfxGetResourceHandle only.
        unsigned int nID = (pThis->m_sBorderStyle != 0) ? 0xfe32u : 0xfe31u;
        wchar_t szText[256];
        szText[0] = L'\0';
        if (impl__AfxLoadString__YAHIPEA_WI_Z(nID, szText, 256) == 0) return FALSE;
        *pValue = szText;
        return TRUE;
    }
    return FALSE;
}


// COleControl::OnGetDlgCode() — retail mfc140u 0x1e3b10 (disassembled at the
// identical mfc140 0x1e1a30):
//     UINT nCode = 0;
//     if (<virtual slot 0x468>() /* IsSubclassedControl */)
//         nCode = (UINT)CWnd::Default();          // mfc140 0x289090
//     const AFX_EVENTMAP* pMap = GetEventMap();   // virtual slot 0x4d0
//     return nCode | ((*pMap->lpStockEventMask /* +0x10 */ & 8) << 4);
// The << 4 turns stock-event-mask bit 3 into DLGC_WANTCHARS (0x80): a control
// that fires KeyPress asks the dialog manager for character messages.
// Symbol: ?OnGetDlgCode@COleControl@@IEAAIXZ
extern "C" unsigned int MS_ABI impl__OnGetDlgCode_COleControl__IEAAIXZ(
    COleControl* pThis)
{
    if (!pThis) return 0;
    unsigned int nCode = 0;
    // Deviation: retail reaches the predicate through vtable slot 0x468, so a
    // derived override is honoured; OpenMFC has no such slot, so the base
    // implementation is called directly.
    if (pThis->IsSubclassedControl()) {
        nCode = static_cast<unsigned int>(
            impl__Default_CWnd__IEAA_JXZ(static_cast<CWnd*>(pThis)));
    }
    // TODO(clean-room): the second half of retail's expression is omitted. It
    // ORs in DLGC_WANTCHARS when the stock event mask has bit 3 set, and that
    // mask lives in AFX_EVENTMAP, which OpenMFC does not model (see
    // InitStockEventMask below). Omitting it matches a control that declares no
    // stock KeyPress event, which is every OpenMFC control today.
    return nCode;
}


// COleControl::OnGetInPlaceMenu() — the retail export shares the DLL-wide
// `xor eax,eax ; ret` body at mfc140u 0x71e0: the base control contributes no
// in-place menu. Derived controls override to return one.
// Symbol: ?OnGetInPlaceMenu@COleControl@@UEAAPEAUHMENU__@@XZ
extern "C" HMENU MS_ABI impl__OnGetInPlaceMenu_COleControl__UEAAPEAUHMENU____XZ(
    COleControl* /*pThis*/)
{
    return nullptr;
}


// COleControl::OnGetNaturalExtent(...) — the retail export shares the DLL-wide
// `xor eax,eax ; ret` body at mfc140u 0x71e0: the base control declines to
// negotiate a natural extent and leaves the out SIZEL untouched.
// Symbol: ?OnGetNaturalExtent@COleControl@@UEAAHKJPEAUtagDVTARGETDEVICE@@PEAUHDC__@@PEAUtagExtentInfo@@PEAUtagSIZE@@@Z
extern "C" int MS_ABI impl__OnGetNaturalExtent_COleControl__UEAAHKJPEAUtagDVTARGETDEVICE__PEAUHDC____PEAUtagExtentInfo__PEAUtagSIZE___Z(
    COleControl* /*pThis*/, unsigned long /*dwAspect*/, long /*lindex*/,
    void* /*ptd*/, HDC /*hicTargetDev*/, void* /*pExtentInfo*/,
    SIZEL* /*psizel*/)
{
    return FALSE;
}



// COleControl::OnGetPredefinedStrings(DISPID dispid, CStringArray* pStringArray,
// CDWordArray* pCookieArray) — retail mfc140u 0x1ef220 (disassembled at the
// identical mfc140 0x1ecf90):
//     if (dispid != DISPID_BORDERSTYLE /* 0xfffffe08 = -504 */) return FALSE;
//     CString str;
//     str.LoadString(AfxFindStringResourceHandle(0xfe31), 0xfe31);
//     pStringArray->SetAtGrow(pStringArray->m_nSize /* +0x10 */, str);
//     pCookieArray->SetAtGrow(pCookieArray->m_nSize /* +0x10 */, 0);
//     str.LoadString(AfxFindStringResourceHandle(0xfe32), 0xfe32);
//     pStringArray->SetAtGrow(pStringArray->m_nSize, str);
//     pCookieArray->SetAtGrow(pCookieArray->m_nSize, 1);
//     return TRUE;
// (A failed resource lookup branches to mfc140 0x180225b80, which does not
// return.)
// Symbol: ?OnGetPredefinedStrings@COleControl@@UEAAHJPEAVCStringArray@@PEAVCDWordArray@@@Z
extern "C" int MS_ABI impl__OnGetPredefinedStrings_COleControl__UEAAHJPEAVCStringArray__PEAVCDWordArray___Z(
    COleControl* pThis, long dispid, CStringArray* pStringArray,
    CDWordArray* pCookieArray)
{
    if (!pThis || !pStringArray || !pCookieArray) return FALSE;
    if (dispid != -504) return FALSE;   // DISPID_BORDERSTYLE

    // Retail appends at the arrays' current size (SetAtGrow(m_nSize, ...)).
    // CStringArray/CDWordArray keep their elements in a side table, not in the
    // object, and OpenMFC publishes no GetSize *thunk* for either -- the plain
    // C++ CStringArray::GetSize() does exist (core/collections/CStringArray.cpp)
    // but calling it from here trips the campaign's link audit, which rejects
    // every newly undefined C++ method. So the two entries are staged in scratch
    // arrays and appended in one go: same resulting order, same indices.
    alignas(16) unsigned char scratchStrings[sizeof(CStringArray)];
    alignas(16) unsigned char scratchCookies[sizeof(CDWordArray)];
    CStringArray* pTmpStrings = reinterpret_cast<CStringArray*>(scratchStrings);
    CDWordArray*  pTmpCookies = reinterpret_cast<CDWordArray*>(scratchCookies);
    impl___0CStringArray__QEAA_XZ(pTmpStrings);
    impl___0CDWordArray__QEAA_XZ(pTmpCookies);

    int bOk = TRUE;
    for (unsigned long dwCookie = 0; dwCookie < 2; ++dwCookie) {
        wchar_t szText[256];
        szText[0] = L'\0';
        // Deviation: retail resolves the string through
        // AfxFindStringResourceHandle and throws when it is missing; OpenMFC's
        // AfxLoadString consults AfxGetResourceHandle only, and a missing
        // resource reports failure here instead of throwing.
        if (impl__AfxLoadString__YAHIPEA_WI_Z(
                static_cast<unsigned int>(0xfe31u + dwCookie), szText, 256) == 0) {
            bOk = FALSE;
            break;
        }
        CString strText(szText);
        impl__SetAtGrow_CStringArray__QEAAX_JAEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
            pTmpStrings, static_cast<long long>(dwCookie), strText);
        impl__SetAtGrow_CDWordArray__QEAAX_JK_Z(
            pTmpCookies, static_cast<long long>(dwCookie), dwCookie);
    }

    if (bOk) {
        impl__Append_CStringArray__QEAA_JAEBV1__Z(pStringArray, pTmpStrings);
        impl__Append_CDWordArray__QEAA_JAEBV1__Z(pCookieArray, pTmpCookies);
    }
    impl___1CDWordArray__UEAA_XZ(pTmpCookies);
    impl___1CStringArray__UEAA_XZ(pTmpStrings);
    return bOk;
}


// COleControl::OnGetPredefinedValue(DISPID dispid, DWORD dwCookie,
// VARIANT* lpvarOut) — retail mfc140u 0x1ef3c0 (disassembled at the identical
// mfc140 0x1ed130):
//     if (dispid == DISPID_BORDERSTYLE /* 0xfffffe08 = -504 */ && dwCookie < 2) {
//         ::VariantClear(lpvarOut);          // oleaut32 ordinal 9
//         lpvarOut->vt = VT_I2;              // 16-bit store of 3
//         *(DWORD*)((BYTE*)lpvarOut + 8) = dwCookie;   // 32-bit store
//         return TRUE;
//     }
//     return FALSE;
// Symbol: ?OnGetPredefinedValue@COleControl@@UEAAHJKPEAUtagVARIANT@@@Z
extern "C" int MS_ABI impl__OnGetPredefinedValue_COleControl__UEAAHJKPEAUtagVARIANT___Z(
    COleControl* /*pThis*/, long dispid, unsigned long dwCookie, VARIANT* lpvarOut)
{
    if (!lpvarOut) return FALSE;
    if (dispid != -504 || dwCookie >= 2) return FALSE;   // DISPID_BORDERSTYLE
    ::VariantClear(lpvarOut);
    lpvarOut->vt = VT_I2;
    // Retail stores the full 32-bit cookie at offset 8 even though vt is VT_I2;
    // lVal aliases iVal there, so this is the same store.
    lpvarOut->lVal = static_cast<long>(dwCookie);
    return TRUE;
}


// COleControl::OnGetViewExtent(DWORD dwAspect, LONG lindex, DVTARGETDEVICE* ptd,
// LPSIZEL lpsizel) — retail mfc140u 0x1f5d20 (disassembled at the identical
// mfc140 0x1f40a0):
//     if (dwAspect == DVASPECT_OPAQUE || dwAspect == DVASPECT_TRANSPARENT)
//         dwAspect = DVASPECT_CONTENT;       // lea eax,[rdx-0x10];
//                                            // test eax,0xffffffef; cmove
//     HRESULT hr = m_xOleObject.GetExtent(dwAspect, lpsizel);  // &this[0x268],
//                                                              // IOleObject
//                                                              // vtable slot 18
//     return SUCCEEDED(hr);                  // not eax; shr eax,31
// lindex and ptd are dropped on the floor by this body.
// Symbol: ?OnGetViewExtent@COleControl@@UEAAHKJPEAUtagDVTARGETDEVICE@@PEAUtagSIZE@@@Z
extern "C" int MS_ABI impl__OnGetViewExtent_COleControl__UEAAHKJPEAUtagDVTARGETDEVICE__PEAUtagSIZE___Z(
    COleControl* pThis, unsigned long dwAspect, long /*lindex*/, void* /*ptd*/,
    SIZEL* lpsizel)
{
    if (!pThis || !lpsizel) return FALSE;
    if (dwAspect == 16u /* DVASPECT_OPAQUE */ ||
        dwAspect == 32u /* DVASPECT_TRANSPARENT */)
        dwAspect = 1u;  // DVASPECT_CONTENT
    // m_xOleObject is a nested BEGIN_INTERFACE_PART sub-object, so the
    // interface pointer is its ADDRESS (retail: `add rcx,0x268`) and the value
    // stored in the member is that sub-object's vtable pointer. afxole.h models
    // the sub-object as the single void* that holds the vptr, so the two must
    // not be confused: `&m_xOleObject` is the `this`, `m_xOleObject` is the
    // vtable.
    void* pVtbl = pThis->m_xOleObject;
    // Deviation: retail does not test the sub-object, which it always
    // constructs; OpenMFC never initialises m_xOleObject, so it is guarded.
    if (!pVtbl) return FALSE;
    void* pObj = &pThis->m_xOleObject;
    void** vft = static_cast<void**>(pVtbl);
    auto* fn = reinterpret_cast<HRESULT (STDMETHODCALLTYPE*)(void*, DWORD, SIZEL*)>(vft[18]);
    return SUCCEEDED(fn(pObj, static_cast<DWORD>(dwAspect), lpsizel)) ? TRUE : FALSE;
}


// COleControl::OnGetViewRect(DWORD dwAspect, RECTL* prcViewRect) — retail
// mfc140u 0x1f5d60 (disassembled at the identical mfc140 0x1f40e0):
//     SIZEL size;
//     <virtual slot 0x3d8>(dwAspect, -1, NULL, &size);   // OnGetViewExtent
//     prcViewRect->left  = 0;                            // one 8-byte zero
//     prcViewRect->top   = 0;                            //   store
//     prcViewRect->right  =  size.cx;
//     prcViewRect->bottom = -size.cy;
//     return TRUE;
// The extent call's result is deliberately ignored; the bottom edge is negated
// because the view rect is expressed in the container's upward HIMETRIC space.
// Symbol: ?OnGetViewRect@COleControl@@UEAAHKPEAU_RECTL@@@Z
extern "C" int MS_ABI impl__OnGetViewRect_COleControl__UEAAHKPEAU_RECTL___Z(
    COleControl* pThis, unsigned long dwAspect, RECTL* prcViewRect)
{
    if (!pThis || !prcViewRect) return FALSE;
    SIZEL size;
    size.cx = 0;
    size.cy = 0;
    // Deviation: retail reaches OnGetViewExtent through vtable slot 0x3d8, so a
    // derived override is honoured; OpenMFC has no such slot, so the base
    // implementation is called directly.
    impl__OnGetViewExtent_COleControl__UEAAHKJPEAUtagDVTARGETDEVICE__PEAUtagSIZE___Z(
        pThis, dwAspect, -1, nullptr, &size);
    prcViewRect->left   = 0;
    prcViewRect->top    = 0;
    prcViewRect->right  = size.cx;
    prcViewRect->bottom = -size.cy;
    return TRUE;
}


// COleControl::OnGetViewStatus() — the retail export shares the DLL-wide
// `mov eax,1 ; ret` body at mfc140u 0x3a60, i.e. it returns
// VIEWSTATUS_OPAQUE (1): the base control paints its whole rectangle.
// (The generated stub returned 0, which advertises a transparent control.)
// Symbol: ?OnGetViewStatus@COleControl@@UEAAKXZ
extern "C" unsigned long MS_ABI impl__OnGetViewStatus_COleControl__UEAAKXZ(
    COleControl* /*pThis*/)
{
    return 1;   // VIEWSTATUS_OPAQUE
}

//=============================================================================
// Window / UI message handlers.
//
// Most bodies below were decoded from the retail export (mangled name -> RVA ->
// objdump of mfc140.dll; the U-build is a separate compile of the same sources,
// so the A/W imports it calls differ -- the transcriptions here name the W
// import where the ANSI DLL calls the A one).  The exceptions, each stubbed and
// individually annotated, are OnHideToolBars, OnMove, OnKeyDownEvent,
// OnKeyUpEvent and OnKeyPressEvent (no RVA of their own in
// mfc140_rva_symbols.json, so there is no body to read) and OnInitMenuPopup
// (read but not transcribable).
// In those listings `call *0x...(%rip) # 0x1802c5bd0` is Control Flow Guard's
// __guard_dispatch_icall_fptr, so the real target is whatever the preceding
// instruction left in RAX: `mov (%rcx),%rax ; mov 0xNN(%rax),%rax ; call *cfg`
// is a virtual call at vtable byte offset NN.  Slots used below and how they
// were pinned:
//   0x240 -> CWnd::OnWndMsg        (pinned: a CWnd-derived vtable in .rdata has
//                                   WindowProc/OnWndMsg/DefWindowProc at byte
//                                   offsets 0x238/0x240/0x248)
//   0x248 -> DefWindowProc         (same vtable dump for the slot number; on a
//                                   COleControl that slot holds COleControl's
//                                   own override, ?DefWindowProcA@COleControl@@
//                                   MEAA_JI_K_J@Z at mfc140 0x1e0060, which is
//                                   `if (!m_hWnd) return 0; else tail-jmp
//                                   CWnd::DefWindowProc` -- so the bodies below
//                                   call impl__DefWindowProcW_COleControl__...,
//                                   as InternalGetText above already does)
//   0x4a8 -> COleControl::GetClientOffset  (see ClientToParent above)
//   0x4b0 -> COleControl::ParentToClient   (arity + UINT return match 0x1e4040;
//                                   0x4a8/0x4b8/0x4c0 are GetClientOffset /
//                                   GetClippingRect / CreateControlWindow, and
//                                   ParentToClient is declared between them.
//                                   COleControl is abstract -- OnDraw is pure
//                                   -- so it emits no vtable of its own to
//                                   resolve these against; this identification
//                                   is inference from the call sites, not a
//                                   vtable read.)
//   0x4d0 -> COleControl::GetEventMap      (same use as OnGetDlgCode above)
// `call 0x180289090` is CWnd::Default (the RVA map names it), reached in this
// file through impl__Default_CWnd__IEAA_JXZ.
//=============================================================================

// COleControl::OnHide() — retail mfc140 0x1decc0:
//     CWnd* pWnd = m_bOpen ? m_pWndOpenFrame          // 0x118, bit 13 of 0x160
//                          : (m_pReflect ? m_pReflect : this);   // 0x150
//     if (pWnd != NULL && pWnd->m_hWnd != NULL)
//         ShowWindow(pWnd->m_hWnd, SW_HIDE);
//     RELEASE(&m_pInPlaceFrame);                      // 0x208
//     RELEASE(&m_pInPlaceDoc);                        // 0x230
//     if (m_bOpen) SendAdvise(6);
//     return S_OK;                                    // xor eax,eax
// Symbol: ?OnHide@COleControl@@MEAAJXZ
extern "C" long MS_ABI impl__OnHide_COleControl__MEAAJXZ(COleControl* pThis)
{
    if (!pThis) return 0;

    CWnd* pWnd;
    if (pThis->m_bOpen) {
        pWnd = pThis->m_pWndOpenFrame;
    } else {
        pWnd = pThis->m_pReflect ? pThis->m_pReflect : static_cast<CWnd*>(pThis);
    }
    if (pWnd != nullptr && pWnd->m_hWnd != nullptr) {
        ::ShowWindow(pWnd->m_hWnd, SW_HIDE);
    }
    ReleaseIface(&pThis->m_pInPlaceFrame);
    ReleaseIface(&pThis->m_pInPlaceDoc);
    // TODO(clean-room): retail closes with `if (m_bOpen) SendAdvise(6)`.
    // OpenMFC's ?SendAdvise@COleControl@@IEAAXI@Z is still an auto-generated
    // stub in this same translation unit whose prototype omits `this`, so it
    // can be neither called ABI-correctly nor re-declared here. The advise is
    // therefore not sent; everything above is transcribed.
    return 0;   // S_OK
}

// COleControl::OnHideToolBars() — no entry in mfc140_rva_symbols.json, but the
// ordinal join described in this file's header resolves it: mfc140u RVA 0x27d0,
// the DLL-wide `ret` that 158 exports fold onto. So the no-op below is the
// retail body, not a placeholder. (The sibling
// ?OnShowToolBars@COleControl@@UEAAXXZ is a different, non-empty function --
// mfc140u 0x1e5580, which is the 0x1e34f0 that mfc140_rva_symbols.json reports
// because that map is the MBCS mfc140.dll.  Do not read 0x1e34f0 as a mfc140u
// address; it is not one.)
// Symbol: ?OnHideToolBars@COleControl@@UEAAXXZ
extern "C" void MS_ABI impl__OnHideToolBars_COleControl__UEAAXXZ(
    COleControl* /*pThis*/)
{
}

// COleControl::OnInactiveMouseMove(LPCRECT, long, long, DWORD) — retail
// mfc140 0x1e4220:
//     CPoint pt(x, y);
//     ParentToClient(pRectBounds, &pt, FALSE);        // virtual slot 0x4b0
//     OnWndMsg(WM_MOUSEMOVE, grfKeyState,             // virtual slot 0x240
//              MAKELONG(pt.x, pt.y), NULL);
// (the retail builds `pt` as four 16-bit stores into the argument home area,
// which reassemble to exactly { LONG x; LONG y; }, and passes its address as
// the third argument of slot 0x4b0.)
// Symbol: ?OnInactiveMouseMove@COleControl@@UEAAXPEBUtagRECT@@JJK@Z
extern "C" void MS_ABI impl__OnInactiveMouseMove_COleControl__UEAAXPEBUtagRECT__JJK_Z(
    COleControl* pThis, const RECT* pRectBounds, long x, long y,
    unsigned long grfKeyState)
{
    if (!pThis) return;
    POINT pt = { x, y };
    ParentToClientLocal(pThis, pRectBounds, &pt, FALSE);
    impl__OnWndMsg_CWnd__MEAAHI_K_JPEA_J_Z(
        pThis, WM_MOUSEMOVE, static_cast<WPARAM>(grfKeyState),
        PackLParam(pt.x, pt.y), nullptr);
}

// COleControl::OnInactiveSetCursor(LPCRECT, long, long, DWORD, BOOL) — retail
// mfc140 0x1e4170:
//     BOOL bResult = TRUE;
//     CPoint pt(x, y);
//     UINT nHitTest = ParentToClient(pRectBounds, &pt, TRUE);  // slot 0x4b0
//     LRESULT lResult = 0;
//     OnWndMsg(WM_SETCURSOR, nHitTest, dwMouseMsg, &lResult);  // slot 0x240
//     if (fSetAlways) {
//         if (lResult == 0)
//             SetCursor(LoadCursor(NULL, IDC_ARROW));
//     } else if (lResult == 0) {
//         bResult = FALSE;
//     }
//     return bResult;
// Note the retail passes the hit-test code as WM_SETCURSOR's wParam and
// dwMouseMsg as its lParam -- not the (HWND, MAKELONG(hit, msg)) packing the
// window manager uses. That is what the binary does; it is transcribed as-is.
// Symbol: ?OnInactiveSetCursor@COleControl@@UEAAHPEBUtagRECT@@JJKH@Z
extern "C" int MS_ABI impl__OnInactiveSetCursor_COleControl__UEAAHPEBUtagRECT__JJKH_Z(
    COleControl* pThis, const RECT* pRectBounds, long x, long y,
    unsigned long dwMouseMsg, int fSetAlways)
{
    if (!pThis) return TRUE;

    int bResult = TRUE;
    POINT pt = { x, y };
    const unsigned int nHitTest =
        ParentToClientLocal(pThis, pRectBounds, &pt, TRUE);

    LRESULT lResult = 0;
    impl__OnWndMsg_CWnd__MEAAHI_K_JPEA_J_Z(
        pThis, WM_SETCURSOR, static_cast<WPARAM>(nHitTest),
        static_cast<LPARAM>(static_cast<long>(dwMouseMsg)), &lResult);

    if (fSetAlways) {
        if (lResult == 0) {
            ::SetCursor(::LoadCursorW(nullptr, IDC_ARROW));
        }
    } else if (lResult == 0) {
        bResult = FALSE;
    }
    return bResult;
}

// COleControl::OnInitMenuPopup(CMenu*, UINT, BOOL) — retail mfc140 0x1e00e0.
// Left a stub deliberately. What the disassembly literally shows, at the level
// this pass could establish:
//   AfxCancelModes(m_hWnd);                            // call 0x1802ac4a0
//   if (bSysMenu /* r9d */ != 0) return;
//   if (pMenu == NULL) return;
//   ... zero-initialises a 0x40-byte stack object whose first field is a
//   pointer to rdata 0x1803210e8 and whose +0x10 field is pMenu; compares
//   pMenu->+0x08 (CMenu::m_hMenu) against AfxGetThreadState()->+0xd0
//   (?AfxGetThreadState@@YAPEAV_AFX_THREAD_STATE@@XZ, 0x180133f20).  On a match
//   it stores pMenu at object+0x38; otherwise it calls this->GetTopLevelParent()
//   (0x18028c760), invokes that window's virtual at +0xe8, takes the +0x08
//   field of the object that returns, and walks it with
//   GetMenuItemCount/GetSubMenu looking for pMenu->m_hMenu ...
// The rest of the body was not decoded. Both the _AFX_THREAD_STATE slot at
// +0xd0 and the CWnd virtual at +0xe8 are unmodeled by OpenMFC, and its
// ?AfxCancelModes@@YAXPEAUHWND__@@@Z export is itself an empty generated stub,
// so nothing here can be transcribed faithfully. A no-op is type-correct (void)
// and changes no state.
// Symbol: ?OnInitMenuPopup@COleControl@@IEAAXPEAVCMenu@@IH@Z
extern "C" void MS_ABI impl__OnInitMenuPopup_COleControl__IEAAXPEAVCMenu__IH_Z(
    COleControl* /*pThis*/, void* /*pMenu*/, unsigned int /*nIndex*/,
    int /*bSysMenu*/)
{
}

// COleControl::OnKeyDown(UINT, UINT, UINT) — retail mfc140 0x1e17b0:
//     USHORT nChar16 = (USHORT)nChar;
//     HWND hWnd = m_hWnd;                             // 0x40
//     KeyDown(&nChar16);                              // direct call 0x1e1920
//     if (m_hWnd == hWnd && nChar16 != 0)
//         DefWindowProc(WM_KEYDOWN, nChar16,          // virtual slot 0x248 =
//                       MAKELONG(nRepCnt, nFlags));   // COleControl's override
// Symbol: ?OnKeyDown@COleControl@@IEAAXIII@Z
extern "C" void MS_ABI impl__OnKeyDown_COleControl__IEAAXIII_Z(
    COleControl* pThis, unsigned int nChar, unsigned int nRepCnt,
    unsigned int nFlags)
{
    if (!pThis) return;
    unsigned short nChar16 = static_cast<unsigned short>(nChar);
    HWND hWnd = pThis->m_hWnd;
    impl__KeyDown_COleControl__IEAAXPEAG_Z(pThis, &nChar16);
    if (pThis->m_hWnd == hWnd && nChar16 != 0) {
        impl__DefWindowProcW_COleControl__MEAA_JI_K_J_Z(
            pThis, WM_KEYDOWN, static_cast<WPARAM>(nChar16),
            PackLParam(static_cast<long>(nRepCnt), static_cast<long>(nFlags)));
    }
}

// COleControl::OnKeyDownEvent(USHORT nChar, USHORT nShiftState) — this export
// has no entry in mfc140_rva_symbols.json, so its own body cannot be
// disassembled. What the binary *does* show is who calls it: retail
// COleControl::KeyDown (0x1e1920) fires DISPID_KEYDOWN (-602) and then, when
// *pnChar is still non-zero, calls virtual slot 0x348 with (this, *pnChar,
// nShiftState) -- two 16-bit values, which is exactly this export's signature
// (X G G). So slot 0x348 is OnKeyDownEvent. The ordinal join described in this
// file's header confirms the rest: the export resolves to mfc140u RVA 0x27d0,
// the DLL-wide `ret` that 158 exports fold onto, so the no-op below is
// the retail body. The only prototype change was the missing `this`.
// (The KeyDown/KeyUp/OnChar comments further up this file name slots
// 0x348/0x350/0x358 as OnKeyDownEvent/OnKeyUpEvent/OnKeyPressEvent for the same
// reason; do not confuse them with ?OnKeyDown@COleControl@@IEAAXIII@Z and
// ?OnKeyUp@...@IEAAXIII@Z, the WM_KEYDOWN/WM_KEYUP handlers at 0x1e17b0 and
// 0x1e1820, which take three UINTs.)
// Symbol: ?OnKeyDownEvent@COleControl@@UEAAXGG@Z
extern "C" void MS_ABI impl__OnKeyDownEvent_COleControl__UEAAXGG_Z(
    COleControl* /*pThis*/, unsigned short /*nChar*/,
    unsigned short /*nShiftState*/)
{
}

// COleControl::OnKeyPressEvent(USHORT nChar) — no entry in
// mfc140_rva_symbols.json; same situation as OnKeyDownEvent above. Retail
// COleControl::OnChar (0x1e1a90) fires DISPID_KEYPRESS (-603) and then, when
// the character is non-zero, calls virtual slot 0x358 with (this, ch) -- one
// 16-bit value, matching this export's signature (X G). The ordinal join
// described in this file's header resolves the export to mfc140u RVA 0x27d0,
// the DLL-wide empty `ret`, so the no-op below is the retail body.
// Symbol: ?OnKeyPressEvent@COleControl@@UEAAXG@Z
extern "C" void MS_ABI impl__OnKeyPressEvent_COleControl__UEAAXG_Z(
    COleControl* /*pThis*/, unsigned short /*nChar*/)
{
}

// COleControl::OnKeyUp(UINT, UINT, UINT) — retail mfc140 0x1e1820, the mirror
// image of OnKeyDown above: KeyUp(&nChar16) (direct call 0x1e1890) and then
// DefWindowProc(WM_KEYUP, ...) under the same two guards.
// Symbol: ?OnKeyUp@COleControl@@IEAAXIII@Z
extern "C" void MS_ABI impl__OnKeyUp_COleControl__IEAAXIII_Z(
    COleControl* pThis, unsigned int nChar, unsigned int nRepCnt,
    unsigned int nFlags)
{
    if (!pThis) return;
    unsigned short nChar16 = static_cast<unsigned short>(nChar);
    HWND hWnd = pThis->m_hWnd;
    impl__KeyUp_COleControl__IEAAXPEAG_Z(pThis, &nChar16);
    if (pThis->m_hWnd == hWnd && nChar16 != 0) {
        impl__DefWindowProcW_COleControl__MEAA_JI_K_J_Z(
            pThis, WM_KEYUP, static_cast<WPARAM>(nChar16),
            PackLParam(static_cast<long>(nRepCnt), static_cast<long>(nFlags)));
    }
}

// COleControl::OnKeyUpEvent(USHORT nChar, USHORT nShiftState) — no entry in
// mfc140_rva_symbols.json; the mirror of OnKeyDownEvent above. Retail
// COleControl::KeyUp (0x1e1890) is KeyDown's mirror image and reaches it
// through virtual slot 0x350. The ordinal join described in this file's header
// resolves the export to mfc140u RVA 0x27d0, the DLL-wide empty `ret`, so the
// no-op below is the retail body.
// Symbol: ?OnKeyUpEvent@COleControl@@UEAAXGG@Z
extern "C" void MS_ABI impl__OnKeyUpEvent_COleControl__UEAAXGG_Z(
    COleControl* /*pThis*/, unsigned short /*nChar*/,
    unsigned short /*nShiftState*/)
{
}

// COleControl::OnKillFocus(CWnd*) — retail mfc140 0x1e0780:
//     Default();                                      // 0x180289090
//     if (m_pControlSite != NULL)                     // 0x1e8
//         m_pControlSite->OnFocus(FALSE);             // vtable slot 8 (+0x40)
// Slot 8 of IOleControlSite is OnFocus(BOOL) (QueryInterface/AddRef/Release,
// OnControlInfoChanged, LockInPlaceActive, GetExtendedControl, TransformCoords,
// TranslateAccelerator, OnFocus), and the retail passes 0 in EDX.
// Symbol: ?OnKillFocus@COleControl@@IEAAXPEAVCWnd@@@Z
extern "C" void MS_ABI impl__OnKillFocus_COleControl__IEAAXPEAVCWnd___Z(
    COleControl* pThis, CWnd* /*pNewWnd*/)
{
    if (!pThis) return;
    (void)impl__Default_CWnd__IEAA_JXZ(static_cast<CWnd*>(pThis));
    void* pSite = pThis->m_pControlSite;
    if (pSite != nullptr) {
        void** vft = *static_cast<void***>(pSite);
        auto* fn = reinterpret_cast<HRESULT (STDMETHODCALLTYPE*)(void*, BOOL)>(vft[8]);
        fn(pSite, FALSE);
    }
}

// COleControl::OnLButtonDblClk(UINT, CPoint) — retail mfc140 0x1e2060:
//     HWND hWnd = m_hWnd;
//     ButtonDblClk(1, nFlags, point);                 // direct call 0x1e1ef0
//     if (m_hWnd == hWnd) Default();
// Note it calls ButtonDblClk directly, not the virtual OnButtonDblClk (0x1e2290).
// Symbol: ?OnLButtonDblClk@COleControl@@IEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnLButtonDblClk_COleControl__IEAAXIVCPoint___Z(
    COleControl* pThis, unsigned int nFlags, unsigned long long pt)
{
    if (!pThis) return;
    HWND hWnd = pThis->m_hWnd;
    impl__ButtonDblClk_COleControl__IEAAXGIVCPoint___Z(pThis, 1, nFlags, pt);
    if (pThis->m_hWnd == hWnd) {
        (void)impl__Default_CWnd__IEAA_JXZ(static_cast<CWnd*>(pThis));
    }
}

// COleControl::OnLButtonDown(UINT, CPoint) — retail mfc140 0x1e2020 is a bare
// argument shuffle and tail jump: OnButtonDown(1, nFlags, point) at 0x1e21a0.
// Symbol: ?OnLButtonDown@COleControl@@IEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnLButtonDown_COleControl__IEAAXIVCPoint___Z(
    COleControl* pThis, unsigned int nFlags, unsigned long long pt)
{
    if (!pThis) return;
    impl__OnButtonDown_COleControl__IEAAXGIVCPoint___Z(pThis, 1, nFlags, pt);
}

// COleControl::OnLButtonUp(UINT, CPoint) — retail mfc140 0x1e2040 tail-jumps to
// OnButtonUp(1, nFlags, point) at 0x1e21f0.
// Symbol: ?OnLButtonUp@COleControl@@IEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnLButtonUp_COleControl__IEAAXIVCPoint___Z(
    COleControl* pThis, unsigned int nFlags, unsigned long long pt)
{
    if (!pThis) return;
    impl__OnButtonUp_COleControl__IEAAXGIVCPoint___Z(pThis, 1, nFlags, pt);
}

// COleControl::OnMButtonDblClk(UINT, CPoint) — retail mfc140 0x1e20e0, the same
// body as OnLButtonDblClk with the button code 4 instead of 1.
// Symbol: ?OnMButtonDblClk@COleControl@@IEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnMButtonDblClk_COleControl__IEAAXIVCPoint___Z(
    COleControl* pThis, unsigned int nFlags, unsigned long long pt)
{
    if (!pThis) return;
    HWND hWnd = pThis->m_hWnd;
    impl__ButtonDblClk_COleControl__IEAAXGIVCPoint___Z(pThis, 4, nFlags, pt);
    if (pThis->m_hWnd == hWnd) {
        (void)impl__Default_CWnd__IEAA_JXZ(static_cast<CWnd*>(pThis));
    }
}

// COleControl::OnMButtonDown(UINT, CPoint) — retail mfc140 0x1e20a0 tail-jumps
// to OnButtonDown(4, nFlags, point).
// Symbol: ?OnMButtonDown@COleControl@@IEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnMButtonDown_COleControl__IEAAXIVCPoint___Z(
    COleControl* pThis, unsigned int nFlags, unsigned long long pt)
{
    if (!pThis) return;
    impl__OnButtonDown_COleControl__IEAAXGIVCPoint___Z(pThis, 4, nFlags, pt);
}

// COleControl::OnMButtonUp(UINT, CPoint) — retail mfc140 0x1e20c0 tail-jumps to
// OnButtonUp(4, nFlags, point).
// Symbol: ?OnMButtonUp@COleControl@@IEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnMButtonUp_COleControl__IEAAXIVCPoint___Z(
    COleControl* pThis, unsigned int nFlags, unsigned long long pt)
{
    if (!pThis) return;
    impl__OnButtonUp_COleControl__IEAAXGIVCPoint___Z(pThis, 4, nFlags, pt);
}

// COleControl::OnMenuSelect(UINT, UINT, HMENU) — retail mfc140 0x1e02c0:
//     if (nFlags == 0xFFFF) {                      // menu closed
//         m_nIDTracking = 0xE001;                  // AFX_IDS_IDLEMESSAGE
//         SendMessage(m_hWnd, 0x0362 /*WM_SETMESSAGESTRING*/, 0xE001, 0);
//     } else if (nItemID == 0 || (nFlags & 0x870) != 0) {
//         m_nIDTracking = 0;                       // popup/separator/break
//     } else if ((UINT)(nItemID - 0xF000) <= 0x1EF) {
//         m_nIDTracking = ((nItemID - 0xF000) >> 4) + 0xEF00;   // SC_* command
//     } else if (nItemID >= 0xFF00) {
//         m_nIDTracking = 0xEF1F;
//     } else {
//         m_nIDTracking = nItemID;
//     }
//     if (m_nIDTracking != m_nIDLastMessage) {      // 0x158 vs 0x15c
//         if (CWnd::FromHandle(GetParent(m_hWnd)) != NULL)
//             PostMessage(m_hWnd, 0, 0, 0);         // message id 0 (WM_NULL)
//     }
// The trailing post really does use message 0 in the binary (`xor %edx,%edx`
// immediately before the PostMessage import); it is transcribed unchanged.
// The binary carries only the numbers; `WM_SETMESSAGESTRING` (0x0362) and
// `AFX_IDS_IDLEMESSAGE` (0xE001) are MFC's names for them and are written here
// as annotations, not as anything read out of the DLL.
// Symbol: ?OnMenuSelect@COleControl@@IEAAXIIPEAUHMENU__@@@Z
extern "C" void MS_ABI impl__OnMenuSelect_COleControl__IEAAXIIPEAUHMENU_____Z(
    COleControl* pThis, unsigned int nItemID, unsigned int nFlags,
    HMENU /*hSysMenu*/)
{
    if (!pThis) return;

    if (nFlags == 0xFFFFu) {
        pThis->m_nIDTracking = 0xE001u;   // AFX_IDS_IDLEMESSAGE
        ::SendMessageW(pThis->m_hWnd, 0x0362 /*WM_SETMESSAGESTRING*/,
                       static_cast<WPARAM>(0xE001u), 0);
    } else if (nItemID == 0 || (nFlags & 0x870u) != 0) {
        pThis->m_nIDTracking = 0;
    } else if (static_cast<unsigned int>(nItemID - 0xF000u) <= 0x1EFu) {
        pThis->m_nIDTracking = ((nItemID - 0xF000u) >> 4) + 0xEF00u;
    } else if (nItemID >= 0xFF00u) {
        pThis->m_nIDTracking = 0xEF1Fu;
    } else {
        pThis->m_nIDTracking = nItemID;
    }

    if (pThis->m_nIDTracking != pThis->m_nIDLastMessage) {
        if (impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(
                ::GetParent(pThis->m_hWnd)) != nullptr) {
            ::PostMessageW(pThis->m_hWnd, 0, 0, 0);
        }
    }
}

// COleControl::OnMouseActivate(CWnd*, UINT, UINT) — retail mfc140 0x1dfcc0:
//     DWORD w = <bitfield word at 0x160>;
//     if ((w & 0xC00) == 0x400)            // m_bInPlaceActive && !m_bUIActive
//         <0x160> = w | (1 << 12);         // m_bPendingUIActivation = TRUE
//     return (int)Default();               // tail jmp 0x180289090
// Symbol: ?OnMouseActivate@COleControl@@IEAAHPEAVCWnd@@II@Z
extern "C" int MS_ABI impl__OnMouseActivate_COleControl__IEAAHPEAVCWnd__II_Z(
    COleControl* pThis, CWnd* /*pDesktopWnd*/, unsigned int /*nHitTest*/,
    unsigned int /*message*/)
{
    if (!pThis) return 0;
    if (pThis->m_bInPlaceActive && !pThis->m_bUIActive) {
        pThis->m_bPendingUIActivation = 1;
    }
    return static_cast<int>(impl__Default_CWnd__IEAA_JXZ(static_cast<CWnd*>(pThis)));
}

// COleControl::OnMouseMove(UINT, CPoint) — retail mfc140 0x1e1f90:
//     const AFX_EVENTMAP* pMap = GetEventMap();          // virtual slot 0x4d0
//     if (*pMap->lpStockEventMask /* +0x10 */ & 0x40) {  // stock MouseMove
//         HWND hWnd = m_hWnd;
//         short nShift = GetShiftState();                // helper 0x1e1670
//         FireEvent(DISPID_MOUSEMOVE /* -606 */, params,
//                   m_iButtonState /* bits 4-6 of 0x160 */, nShift,
//                   point.x, point.y);
//         if (m_hWnd != hWnd) return;                    // window died: no
//     }                                                  //   Default()
//     Default();
// The retail helper at 0x1e1670 is GetKeyState(VK_SHIFT/VK_CONTROL/VK_MENU)
// folded into 1|2|4, which is exactly detail::ole6impl::GetShiftState().
// Symbol: ?OnMouseMove@COleControl@@IEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnMouseMove_COleControl__IEAAXIVCPoint___Z(
    COleControl* pThis, unsigned int /*nFlags*/, unsigned long long pt)
{
    if (!pThis) return;

    // Deviation: the stock-event-mask gate cannot be evaluated. AFX_EVENTMAP is
    // not modeled by OpenMFC and GetEventMap (this file) returns null, so there
    // is no mask word to test. The fire is made unconditional, matching the
    // precedent set by KeyDown/OnClick/DoClick above; a control that declares
    // no stock MouseMove event will therefore see FireEvent called where retail
    // would have gone straight to Default().
    HWND hWnd = pThis->m_hWnd;
    pThis->FireMouseMove(static_cast<short>(pThis->m_iButtonState),
                         GetShiftState(), PtX(pt), PtY(pt));
    if (pThis->m_hWnd != hWnd) return;
    (void)impl__Default_CWnd__IEAA_JXZ(static_cast<CWnd*>(pThis));
}

// COleControl::OnMove(int, int) — this export has no entry in
// mfc140_rva_symbols.json, so there is no retail body to disassemble and
// nothing was established about what it does. Left a no-op rather than
// guessing; the only change is the `this` pointer the generated prototype was
// missing.
// COleControl::OnMove(int, int) - retail mfc140u 0x1e2180:
//     if (<virtual slot 0x468>())   // IsSubclassedControl
//         CWnd::Default();          // 0x18028ac80
//     (the x and y arguments are never read)
// Slot 0x468 is IsSubclassedControl: afxctl.h declares IsSubclassedControl,
// ReparentControlWindow and GetControlFlags consecutively, and the retail slots
// 0x468 / 0x470 / 0x478 are consecutive too -- 0x478 being GetControlFlags is
// independently pinned by GetInterfaceHook above.  CreateWindowForSubclassedControl
// (this file) gates on the same slot.
// Deviation: OpenMFC's COleControl does not declare IsSubclassedControl virtual,
// so this calls the base implementation through its export thunk rather than
// dispatching; a derived control that overrides it is not seen.
// Symbol: ?OnMove@COleControl@@IEAAXHH@Z
extern "C" void MS_ABI impl__OnMove_COleControl__IEAAXHH_Z(
    COleControl* pThis, int /*x*/, int /*y*/)
{
    if (!pThis) return;
    if (impl__IsSubclassedControl_COleControl__UEAAHXZ(pThis))
        (void)impl__Default_CWnd__IEAA_JXZ(static_cast<CWnd*>(pThis));
}

// COleControl::OnNcCalcSize(BOOL, NCCALCSIZE_PARAMS*) — retail mfc140 0x1f3be0:
//     Default();
//     if (m_bOpen) return;                           // bit 13 of 0x160
//     CRectTracker* pTracker = m_pRectTracker;       // 0x118
//     if (pTracker == NULL) return;
//     InflateRect(&lpncsp->rgrc[0], 1 - pTracker->+0x24, 1 - pTracker->+0x24);
//     *(RECT*)(pTracker + 0x0c) = m_rcPos;           // 16-byte SSE copy
//     <helper 0x1f37f8>(pTracker + 0x0c, this);      // border OffsetRect
// Symbol: ?OnNcCalcSize@COleControl@@IEAAXHPEAUtagNCCALCSIZE_PARAMS@@@Z
extern "C" void MS_ABI impl__OnNcCalcSize_COleControl__IEAAXHPEAUtagNCCALCSIZE_PARAMS___Z(
    COleControl* pThis, int /*bCalcValidRects*/, void* /*lpncsp*/)
{
    if (!pThis) return;
    (void)impl__Default_CWnd__IEAA_JXZ(static_cast<CWnd*>(pThis));
    // TODO(clean-room): transcribed partially. The remaining branch only runs
    // when !m_bOpen and m_pRectTracker != NULL, and it reaches into the tracker
    // at raw offsets 0x0c (its RECT) and 0x24 (its handle size). OpenMFC keeps
    // m_pRectTracker as a void* allocated through the CRectTracker impl_ thunks
    // and pins no such offsets, so writing through them would be inventing
    // layout. The retail's unconditional Default() call above is transcribed.
}

// COleControl::OnNcCreate(LPCREATESTRUCT) — retail mfc140 0x1dfaa0:
//     if (m_pReflect != NULL)                        // 0x150
//         *(COleControl**)((BYTE*)m_pReflect + 0xE8) = this;
//     return (BOOL)Default();                        // tail jmp 0x180289090
// Offset 0xE8 is the first member past CWnd in the reflector window, i.e.
// CReflectorWnd's back-pointer to the control -- the slot
// ?SetControl@CReflectorWnd@@QEAAXPEAVCOleControl@@@Z writes. OpenMFC keeps
// that back-pointer in a side table (core/window/CReflectorWnd.cpp) instead of
// at +0xE8, so the store is routed through SetControl; a raw store at +0xE8
// would corrupt the OpenMFC object. SetControl additionally clears any stale
// mapping, which the retail store does not do.
// Symbol: ?OnNcCreate@COleControl@@IEAAHPEAUtagCREATESTRUCTW@@@Z
extern "C" int MS_ABI impl__OnNcCreate_COleControl__IEAAHPEAUtagCREATESTRUCTW___Z(
    COleControl* pThis, void* /*lpcs*/)
{
    if (!pThis) return 0;
    if (pThis->m_pReflect != nullptr) {
        impl__SetControl_CReflectorWnd__QEAAXPEAVCOleControl___Z(
            pThis->m_pReflect, pThis);
    }
    return static_cast<int>(impl__Default_CWnd__IEAA_JXZ(static_cast<CWnd*>(pThis)));
}

// COleControl::OnNcHitTest(CPoint) - retail mfc140u 0x1f58e0, transcribed:
//     if (m_bOpen) return Default();                     // bit 13 of 0x160
//     CRectTracker* pT = m_pRectTracker;                 // 0x118
//     if (pT == NULL) return Default();
//     if (!(pT->m_nStyle & 0x18)) return Default();      // resizeInside|Outside
//     LRESULT lr = Default();
//     if ((ULONG_PTR)(lr - 4) <= 3 && lr != 5) return lr;   // the retail test is
//         // `lea rcx,[rax-4] ; test rcx,-4 ; jne continue ; cmp rax,5 ; jne ret`
//     CPoint ptClient = point; ScreenToClient(m_hWnd, &ptClient);
//     RECT rc = {0,0,0,0};
//     GetClientRect(&rc);                                // virtual slot 0x2d8
//     if (PtInRect(&rc, ptClient)) return HTCLIENT;      // 1
//     ::GetWindowRect(m_hWnd, &rc);                      // NOTE: window, not client
//     int d = pT->m_nHandleSize - 1;
//     ::InflateRect(&rc, -d, -d);
//     if (PtInRect(&rc, point)) return HTBORDER;         // 18; the ORIGINAL
//         // screen-coordinate point, which matches rc now being a screen rect
//     return pT->HitTest(ptClient) != -1 ? HTBORDER : HTNOWHERE;   // 18 : 0
//         // (`inc eax ; neg eax ; sbb rax,rax ; and eax,0x12`)
// Slot 0x2d8 is COleControl::GetClientRect(LPRECT) const: afxctl.h declares
// GetClientRect, DoPropExchange, OnResetState, OnDraw, OnDrawMetafile in that
// order and the retail slots 0x2d8/0x2e0/0x2e8/0x2f0/0x2f8 match one-for-one
// (0x2f0 = OnDraw and 0x2f8 = OnDrawMetafile are already relied on by
// DrawContent/DrawMetafile above).  OpenMFC's COleControl does not declare
// GetClientRect virtual, so the base implementation in this file is called
// directly -- a derived override is not seen.
// Symbol: ?OnNcHitTest@COleControl@@IEAA_JVCPoint@@@Z
extern "C" __int64 MS_ABI impl__OnNcHitTest_COleControl__IEAA_JVCPoint___Z(
    COleControl* pThis, unsigned long long point)
{
    if (!pThis) return 0;
    const TrackerView* pT = static_cast<const TrackerView*>(pThis->m_pRectTracker);
    if (pThis->m_bOpen || pT == nullptr || (pT->m_nStyle & kTrackerResizeMask) == 0)
        return impl__Default_CWnd__IEAA_JXZ(static_cast<CWnd*>(pThis));

    __int64 lr = impl__Default_CWnd__IEAA_JXZ(static_cast<CWnd*>(pThis));
    if ((unsigned long long)(lr - 4) <= 3 && lr != 5)
        return lr;

    POINT ptClient = { (LONG)(unsigned int)(point & 0xFFFFFFFFull),
                       (LONG)(unsigned int)(point >> 32) };
    ::ScreenToClient(pThis->m_hWnd, &ptClient);

    RECT rc = { 0, 0, 0, 0 };
    impl__GetClientRect_COleControl__UEBAXPEAUtagRECT___Z(pThis, &rc);
    if (::PtInRect(&rc, ptClient)) return HTCLIENT;

    ::GetWindowRect(pThis->m_hWnd, &rc);
    const int d = pT->m_nHandleSize - 1;
    ::InflateRect(&rc, -d, -d);
    POINT ptScreen = { (LONG)(unsigned int)(point & 0xFFFFFFFFull),
                       (LONG)(unsigned int)(point >> 32) };
    if (::PtInRect(&rc, ptScreen)) return HTBORDER;

    const unsigned long long ptClientPacked =
        ((unsigned long long)(unsigned int)ptClient.y << 32) |
        (unsigned long long)(unsigned int)ptClient.x;
    return impl__HitTest_CRectTracker__QEBAHVCPoint___Z(pT, ptClientPacked) != -1
               ? HTBORDER : HTNOWHERE;
}

// COleControl::OnNcLButtonDown(UINT, CPoint) - retail mfc140u 0x1f5a20.  Decoded
// shape (not implemented, see below):
//     if (m_bOpen) return Default();                        // bit 13 of 0x160
//     if (m_pRectTracker == NULL) return Default();
//     if (!(m_pRectTracker->m_nStyle & 0x18)) return Default();
//     if ((UINT)(nHitTest - 6) <= 1) return Default();       // HTHSCROLL/HTVSCROLL
//     CPoint pt = point; ScreenToClient(m_hWnd, &pt);
//     CRect rcSave = m_pRectTracker->m_rect;                 // +0x0c, 16-byte copy
//     CWnd* pClip = CWnd::FromHandle(GetParent((m_pReflect ? m_pReflect : this)->m_hWnd));
//     if (!m_pRectTracker->Track(this, pt, FALSE, pClip))    // 0x180276650
//         return;
//         // NOTE the argument order: rdx is `this` (the tracked CWnd) and the
//         // parent CWnd* goes in the FOURTH slot, [rsp+0x20], i.e. pWndClipTo.
//         // An earlier revision of this comment had it as Track(pParent, pt,
//         // FALSE, NULL); the disassembly says otherwise.
//     if (EqualRect(&m_pRectTracker->m_rect, &rcSave)) return;
//     OffsetRect(&m_pRectTracker->m_rect, -left, -top);      // back to the origin
//     CWnd* pParent = CWnd::FromHandle(GetParent((m_pReflect ? m_pReflect : this)->m_hWnd));
//     RECT rcWnd = {0}, rcParent = {0};
//     ::GetWindowRect((m_pReflect ? m_pReflect : this)->m_hWnd, &rcWnd);
//     ::GetClientRect(pParent->m_hWnd, &rcParent);
//     pParent->ClientToScreen(&rcParent);                    // 0x1802a3310
//     OffsetRect(&rect, (m_nHandleSize-1) - rcParent.left + m_sBorderStyle*GetSystemMetrics(SM_CXBORDER) + rcWnd.left,
//                       (m_nHandleSize-1) - rcParent.top  + m_sBorderStyle*GetSystemMetrics(SM_CYBORDER) + rcWnd.top);
//     SIZE sz = { rect.right-rect.left, rect.bottom-rect.top };
//     <pixels->HIMETRIC, unexported 0x2607b4: MulDiv(px, 2540, LOGPIXELSX)>;
//     if (sz != (m_cxExtent, m_cyExtent)) { m_bModified = TRUE; m_cxExtent = ...; }
//     m_pInPlaceSite->OnPosRectChange(&rect);                // (this+0x1e0) vtable +0x70
// TODO(clean-room): left a stub, but NOT for the reason an earlier revision of
// this comment gave.  CRectTracker::Track *is* available -- OpenMFC implements
// it for real at phase4/src/core/gdi/CRectTracker.cpp:371 (a genuine modal
// capture loop) -- and CreateFrameWindow/CreateControlWindow are not on this
// path at all.  What actually blocks it is the tail: it renegotiates the
// control's extent with the container through IOleInPlaceSite::OnPosRectChange
// (vtable slot 14 of the raw void* m_pInPlaceSite, an interface OpenMFC's
// COleControl does not model as a typed pointer) after a pixel->HIMETRIC
// transform that has no exported helper.  Running the drag loop and then
// silently dropping the negotiated rect would leave the tracker and the
// container disagreeing about the control's extent, which is worse than not
// tracking at all -- so nothing is done.
// Symbol: ?OnNcLButtonDown@COleControl@@IEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnNcLButtonDown_COleControl__IEAAXIVCPoint___Z(
    COleControl* pThis, unsigned int /*nHitTest*/, unsigned long long /*point*/)
{
    if (!pThis) return;
}

// COleControl::OnNcPaint() - retail mfc140u 0x1f56a0, transcribed:
//     if (m_bOpen || m_pRectTracker == NULL) { Default(); return; }
//     DWORD dwStyle = GetStyle(), dwExStyle = GetExStyle();
//     BOOL bScroll = (dwStyle & (WS_HSCROLL|WS_VSCROLL)) != 0;
//     if (bScroll) Default();
//     int d = m_pRectTracker->m_nHandleSize - 1;            // +0x24
//     CWindowDC dc(this);
//     CRect& rcT = m_pRectTracker->m_rect;                  // +0x0c
//     int dx = rcT.left - d, dy = rcT.top - d;
//     rcT.OffsetRect(-dx, -dy);                             // park it at (d,d)
//     CRect rect = rcT;
//     m_pRectTracker->Draw(&dc);                            // 0x180276120
//     rcT.OffsetRect(dx, dy);                               // put it back
//     <border helper 0x1e03a0>(&dc, &rect, dwStyle & WS_BORDER,
//                              dwExStyle & WS_EX_CLIENTEDGE);
//     if ((dwStyle & (WS_HSCROLL|WS_VSCROLL)) == (WS_HSCROLL|WS_VSCROLL)) {
//         int cx = GetSystemMetrics(SM_CXVSCROLL);
//         if (dwExStyle & WS_EX_LAYOUTRTL) rect.right = rect.left + cx;
//         else                             rect.left  = rect.right - cx;
//         rect.top = rect.bottom - GetSystemMetrics(SM_CYHSCROLL);
//         CBrush br(GetSysColor(COLOR_BTNFACE));
//         ::FillRect(dc.m_hDC, &rect, br);                  // the scrollbar corner
//     }
// Implemented.  (An earlier revision left this a stub and named CRectTracker::Draw
// as one of the blockers; that was wrong -- OpenMFC implements Draw for real at
// phase4/src/core/gdi/CRectTracker.cpp:329 and exports the thunk.  The border
// helper at 0x1e03a0 has no export, so it is transcribed as DrawTrackerBorder at
// the top of this file.)
// DEVIATIONS, all of them in how the two stack objects are obtained:
//  * retail builds a stack CWindowDC; OpenMFC's CWindowDC constructor thunk
//    (core/gdi/CWindowDC.cpp) installs no vtable pointer, so ::GetWindowDC +
//    CDC::FromHandle + ::ReleaseDC is used instead.  That is observationally the
//    same DC; it just also parks a temp CDC in the handle map until the next
//    AfxUnlockTempMaps.
//  * retail builds a stack CBrush; a raw ::CreateSolidBrush/::DeleteObject pair
//    is used here for the same reason.
//  * retail does not null-check the DC.  This body returns without painting when
//    ::GetWindowDC fails, rather than passing NULL down to DrawEdge/FillRect.
// Symbol: ?OnNcPaint@COleControl@@IEAAXXZ
extern "C" void MS_ABI impl__OnNcPaint_COleControl__IEAAXXZ(COleControl* pThis)
{
    if (!pThis) return;

    TrackerView* pT = static_cast<TrackerView*>(pThis->m_pRectTracker);
    if (pThis->m_bOpen || pT == nullptr) {
        (void)impl__Default_CWnd__IEAA_JXZ(static_cast<CWnd*>(pThis));
        return;
    }

    const DWORD dwStyle   = (DWORD)::GetWindowLongW(pThis->m_hWnd, GWL_STYLE);
    const DWORD dwExStyle = (DWORD)::GetWindowLongW(pThis->m_hWnd, GWL_EXSTYLE);
    const DWORD dwScroll  = dwStyle & (WS_HSCROLL | WS_VSCROLL);
    if (dwScroll != 0)
        (void)impl__Default_CWnd__IEAA_JXZ(static_cast<CWnd*>(pThis));

    const int d = pT->m_nHandleSize - 1;

    HDC hDC = ::GetWindowDC(pThis->m_hWnd);
    if (hDC == nullptr) return;

    // Park the tracker rect at (d,d), draw it, then put it back exactly where it
    // was -- retail mutates the member in place and restores it the same way.
    const int dx = pT->m_rect.left - d;
    const int dy = pT->m_rect.top  - d;
    ::OffsetRect(&pT->m_rect, -dx, -dy);
    RECT rect = pT->m_rect;
    if (CDC* pDC = impl__FromHandle_CDC__SAPEAV1_PEAUHDC_____Z(hDC))
        impl__Draw_CRectTracker__QEBAXPEAVCDC___Z(pT, pDC);
    ::OffsetRect(&pT->m_rect, dx, dy);

    DrawTrackerBorder(hDC, &rect, (int)(dwStyle & WS_BORDER),
                      (int)(dwExStyle & WS_EX_CLIENTEDGE));

    if (dwScroll == (WS_HSCROLL | WS_VSCROLL)) {
        const int cx = ::GetSystemMetrics(SM_CXVSCROLL);
        if (dwExStyle & WS_EX_LAYOUTRTL) rect.right = rect.left + cx;
        else                             rect.left  = rect.right - cx;
        rect.top = rect.bottom - ::GetSystemMetrics(SM_CYHSCROLL);
        if (HBRUSH hbr = ::CreateSolidBrush(::GetSysColor(COLOR_BTNFACE))) {
            ::FillRect(hDC, &rect, hbr);
            ::DeleteObject(hbr);
        }
    }

    ::ReleaseDC(pThis->m_hWnd, hDC);
}

// COleControl::OnOcmCtlColor*(WPARAM, LPARAM) - seven ICF-distinct one-liners at
// retail mfc140u 0x1e28d0/0x1e28f0/0x1e2910/0x1e2930/0x1e2950/0x1e2970/0x1e2990.
// Each is the same five instructions:
//     mov rcx,[rcx+0x40]       ; m_hWnd
//     mov r9,r8 ; mov r8,rdx   ; shuffle wParam/lParam up one slot
//     mov edx,<WM_CTLCOLOR*>
//     jmp  qword ptr [DefWindowProcW]    ; USER32 import at 0x2c7350
// i.e. `return ::DefWindowProcW(m_hWnd, WM_CTLCOLORxxx, wParam, lParam);`
// The tail-jump target is the USER32 IAT slot 0x2c7350, and objdump -p on
// mfc140u names that slot DefWindowProcW -- NOT SendMessageW, which lives at
// 0x2c7120.  (An earlier revision of this comment and of the seven bodies below
// said SendMessageW; that was a misread of the import table.  It matters: these
// handlers run on the control's OWN window, so ::SendMessageW(m_hWnd, ...) would
// re-enter the control's window procedure instead of reaching the default one.)
// The message numbers are the immediates the seven bodies load, in this order:
//   MSGBOX 0x132, EDIT 0x133, LISTBOX 0x134, BTN 0x135, DLG 0x136,
//   SCROLLBAR 0x137, STATIC 0x138.
// These are the WM_OCM_* reflections: the container sends OCM_CTLCOLORxxx to the
// control, and the control hands the plain WM_CTLCOLORxxx straight to the
// default window procedure of its (usually superclassed) window.
// Symbol: ?OnOcmCtlColorBtn@COleControl@@IEAA_J_K_J@Z
extern "C" __int64 MS_ABI impl__OnOcmCtlColorBtn_COleControl__IEAA_J_K_J_Z(
    COleControl* pThis, unsigned __int64 wParam, __int64 lParam)
{
    if (!pThis) return 0;
    return (__int64)::DefWindowProcW(pThis->m_hWnd, WM_CTLCOLORBTN,
                                     (WPARAM)wParam, (LPARAM)lParam);
}

// Symbol: ?OnOcmCtlColorDlg@COleControl@@IEAA_J_K_J@Z
extern "C" __int64 MS_ABI impl__OnOcmCtlColorDlg_COleControl__IEAA_J_K_J_Z(
    COleControl* pThis, unsigned __int64 wParam, __int64 lParam)
{
    if (!pThis) return 0;
    return (__int64)::DefWindowProcW(pThis->m_hWnd, WM_CTLCOLORDLG,
                                     (WPARAM)wParam, (LPARAM)lParam);
}

// Symbol: ?OnOcmCtlColorEdit@COleControl@@IEAA_J_K_J@Z
extern "C" __int64 MS_ABI impl__OnOcmCtlColorEdit_COleControl__IEAA_J_K_J_Z(
    COleControl* pThis, unsigned __int64 wParam, __int64 lParam)
{
    if (!pThis) return 0;
    return (__int64)::DefWindowProcW(pThis->m_hWnd, WM_CTLCOLOREDIT,
                                     (WPARAM)wParam, (LPARAM)lParam);
}

// Symbol: ?OnOcmCtlColorListBox@COleControl@@IEAA_J_K_J@Z
extern "C" __int64 MS_ABI impl__OnOcmCtlColorListBox_COleControl__IEAA_J_K_J_Z(
    COleControl* pThis, unsigned __int64 wParam, __int64 lParam)
{
    if (!pThis) return 0;
    return (__int64)::DefWindowProcW(pThis->m_hWnd, WM_CTLCOLORLISTBOX,
                                     (WPARAM)wParam, (LPARAM)lParam);
}

// Symbol: ?OnOcmCtlColorMsgBox@COleControl@@IEAA_J_K_J@Z
extern "C" __int64 MS_ABI impl__OnOcmCtlColorMsgBox_COleControl__IEAA_J_K_J_Z(
    COleControl* pThis, unsigned __int64 wParam, __int64 lParam)
{
    if (!pThis) return 0;
    return (__int64)::DefWindowProcW(pThis->m_hWnd, WM_CTLCOLORMSGBOX,
                                     (WPARAM)wParam, (LPARAM)lParam);
}

// Symbol: ?OnOcmCtlColorScrollBar@COleControl@@IEAA_J_K_J@Z
extern "C" __int64 MS_ABI impl__OnOcmCtlColorScrollBar_COleControl__IEAA_J_K_J_Z(
    COleControl* pThis, unsigned __int64 wParam, __int64 lParam)
{
    if (!pThis) return 0;
    return (__int64)::DefWindowProcW(pThis->m_hWnd, WM_CTLCOLORSCROLLBAR,
                                     (WPARAM)wParam, (LPARAM)lParam);
}

// Symbol: ?OnOcmCtlColorStatic@COleControl@@IEAA_J_K_J@Z
extern "C" __int64 MS_ABI impl__OnOcmCtlColorStatic_COleControl__IEAA_J_K_J_Z(
    COleControl* pThis, unsigned __int64 wParam, __int64 lParam)
{
    if (!pThis) return 0;
    return (__int64)::DefWindowProcW(pThis->m_hWnd, WM_CTLCOLORSTATIC,
                                     (WPARAM)wParam, (LPARAM)lParam);
}

// COleControl::OnOpen(BOOL, LPMSG) - retail mfc140u 0x1e0e10.  Decoded shape
// (not implemented, see below):
//     if (!m_bOpen) {                                    // bit 13 of 0x160
//         if (bTryInPlace) {
//             HRESULT hr = OnActivateInPlace(bTryInPlace, pMsg);   // slot 0x508
//             if (SUCCEEDED(hr)) return S_OK;
//         }
//         if (m_bInPlaceActive)                          // bit 10 of 0x160
//             m_xOleInPlaceObject.InPlaceDeactivate();   // (this+0x280) vtable[5]
//         m_bOpen = TRUE;                                // bts 13
//         if (m_pWndOpenFrame == NULL) {                 // 0x118
//             m_pWndOpenFrame = CreateFrameWindow();     // slot 0x4e8
//             if (m_pWndOpenFrame == NULL) return E_FAIL;
//             int cx, cy; GetControlSize(&cx, &cy);
//             ResizeFrameWindow(cx, cy);                 // slot 0x4f0
//             RECT rc = {0,0,0,0};
//             ::GetClientRect(m_pWndOpenFrame->m_hWnd, &rc);
//             if (!CreateControlWindow(m_pWndOpenFrame->m_hWnd, rc, &rc))  // 0x4c0
//                 return E_FAIL;
//         }
//     }
//     m_pWndOpenFrame->ShowWindow(SW_SHOW);              // 0x1802a9ad0
//     CWnd::FromHandle(::SetActiveWindow(m_pWndOpenFrame->m_hWnd));
//         // i.e. the inlined CWnd::SetActiveWindow(); the returned HWND is fed
//         // to CWnd::FromHandle (0x18028ad70) and only the CWnd* is discarded.
//     SendAdvise(5);                                     // 0x1801e0c10
//     return S_OK;
// The slot numbers come from afxctl.h's declaration order: OnOpen, then
// CreateFrameWindow / ResizeFrameWindow / OnFrameClose / OnHide /
// OnActivateInPlace are consecutive, matching 0x4e0/0x4e8/0x4f0/0x4f8/0x500/0x508.
// TODO(clean-room): left a stub.  CreateFrameWindow above is itself a stub that
// returns NULL, so a faithful transcription would take the E_FAIL path on every
// call; and ?SendAdvise@COleControl@@IEAAXI@Z further down this file is a
// generated stub whose declared signature omits `this`, so it cannot be called
// ABI-correctly.  Returning S_OK ("opened") without opening anything would be a
// lie to the container, so 0 is returned unchanged and nothing is claimed.
// Symbol: ?OnOpen@COleControl@@MEAAJHPEAUtagMSG@@@Z
extern "C" long MS_ABI impl__OnOpen_COleControl__MEAAJHPEAUtagMSG___Z(
    COleControl* pThis, int /*bTryInPlace*/, void* /*pMsg*/)
{
    (void)pThis;
    return 0;
}

// COleControl::OnPaint(CDC*) - retail mfc140u 0x1e1470, transcribed:
//     if (m_bNoRedraw) {                                 // bit 23 of 0x160
//         ::ValidateRect(m_hWnd, NULL); m_bNoRedraw = FALSE; return;
//     }
//     AfxLockTempMaps();
//     ::GetWindowRect(m_hWnd, &m_rcBounds);              // the member, in place
//         // NOTE: the WINDOW rect, not the client rect.  The call at 0x1e14d5
//         // goes through the USER32 IAT slot 0x2c6c08, which objdump -p names
//         // GetWindowRect; GetClientRect is a different slot, 0x2c7330 (retail
//         // uses that one three instructions into OnOpen, so both appear in this
//         // file).  It has to be the window rect: the InflateRect below then
//         // shrinks it by the tracker handle size to strip the tracker border.
//     ::OffsetRect(&m_rcBounds, -m_rcBounds.left, -m_rcBounds.top);
//     if (!m_bOpen && m_pRectTracker != NULL) {
//         int d = 1 - m_pRectTracker->m_nHandleSize;     // +0x24
//         ::InflateRect(&m_rcBounds, d, d);
//     }
//     CRect rcClient(0,0,0,0);
//     GetClientRect(&rcClient);                          // virtual slot 0x2d8
//     if (pDC != NULL) {
//         int nSave = pDC->SaveDC();                     // CDC vtable +0x48
//         OnDraw(pDC, rcClient, rcClient);               // virtual slot 0x2f0
//         pDC->RestoreDC(nSave);                         // CDC vtable +0x50
//         // (retail dispatches SaveDC/RestoreDC virtually; the code below calls
//         //  the CDC export thunks non-virtually, so a CDC-derived override is
//         //  not seen.  OnDraw *is* dispatched virtually -- OpenMFC's COleControl
//         //  declares it -- so a derived control's OnDraw does run.)
//     } else {
//         CPaintDC dc(this);
//         CRect rcInvalid; ::CopyRect(&rcInvalid, &dc.m_ps.rcPaint);
//         OnDraw(&dc, rcClient, rcInvalid);
//     }
//     AfxUnlockTempMaps(TRUE);
// DEVIATION in the pDC == NULL branch: retail builds a stack CPaintDC.  OpenMFC's
// CPaintDC constructor thunk (core/gdi/CPaintDC.cpp) does not install a vtable
// pointer, so a CPaintDC built on raw storage here would be handed to a derived
// OnDraw with a null vptr.  BeginPaint/EndPaint are called directly instead and
// the DC is wrapped with CDC::FromHandle, which yields a properly constructed
// CDC from the handle map.  The rectangles passed to OnDraw are the same ones
// retail passes (client rect, and the PAINTSTRUCT's rcPaint as rcInvalid).
// Second, smaller deviation: that branch is skipped entirely when m_hWnd is
// NULL, because BeginPaint on a null window would fail and leave an unbalanced
// EndPaint; retail's CPaintDC constructor asserts on that case instead.
// Symbol: ?OnPaint@COleControl@@IEAAXPEAVCDC@@@Z
extern "C" void MS_ABI impl__OnPaint_COleControl__IEAAXPEAVCDC___Z(
    COleControl* pThis, CDC* pDC)
{
    if (!pThis) return;

    if (pThis->m_bNoRedraw) {
        ::ValidateRect(pThis->m_hWnd, nullptr);
        pThis->m_bNoRedraw = 0;
        return;
    }

    impl__AfxLockTempMaps__YAXXZ();

    ::GetWindowRect(pThis->m_hWnd, pThis->m_rcBounds);
    ::OffsetRect(pThis->m_rcBounds, -pThis->m_rcBounds.left, -pThis->m_rcBounds.top);
    if (!pThis->m_bOpen && pThis->m_pRectTracker != nullptr) {
        const TrackerView* pT = static_cast<const TrackerView*>(pThis->m_pRectTracker);
        const int d = 1 - pT->m_nHandleSize;
        ::InflateRect(pThis->m_rcBounds, d, d);
    }

    CRect rcClient(0, 0, 0, 0);
    impl__GetClientRect_COleControl__UEBAXPEAUtagRECT___Z(pThis, rcClient);

    if (pDC != nullptr) {
        const int nSave = impl__SaveDC_CDC__QEAAHXZ(pDC);
        pThis->OnDraw(pDC, rcClient, rcClient);
        (void)impl__RestoreDC_CDC__QEAAHH_Z(pDC, nSave);
    } else if (pThis->m_hWnd != nullptr) {
        PAINTSTRUCT ps;
        ::memset(&ps, 0, sizeof(ps));
        HDC hDC = ::BeginPaint(pThis->m_hWnd, &ps);
        if (hDC != nullptr) {
            CDC* pPaintDC = impl__FromHandle_CDC__SAPEAV1_PEAUHDC_____Z(hDC);
            if (pPaintDC != nullptr) {
                CRect rcInvalid(ps.rcPaint);
                pThis->OnDraw(pPaintDC, rcClient, rcInvalid);
            }
            ::EndPaint(pThis->m_hWnd, &ps);
        }
    }

    (void)impl__AfxUnlockTempMaps__YAHH_Z(TRUE);
}

// COleControl::OnQueryHitPoint(DWORD, LPCRECT, POINT, LONG, DWORD*) and
// COleControl::OnQueryHitRect(DWORD, LPCRECT, LPCRECT, LONG, DWORD*) are one
// ICF-folded body at retail mfc140u 0x1f5db0 (the two exports resolve to the
// same RVA), which is:
//     mov ecx,1 ; cmp edx,ecx ; jne .no
//     mov rax,[rsp+0x30] ; mov dword ptr [rax],3 ; jmp .ret
//   .no: xor ecx,ecx
//   .ret: mov eax,ecx ; ret
// i.e. `if (dwAspect != DVASPECT_CONTENT) return FALSE;
//       *pHitResult = HITRESULT_HIT; return TRUE;` -- the bounds rect, the
// location and lCloseHint are all ignored, and pHitResult is written without a
// null check.  The two functions fold because they take the same six argument
// slots and neither reads the third one.
// Deviation: the two bodies below DO null-check pHitResult before writing, in
// line with the defensive style of the rest of this file.  Retail would fault.
// Symbol: ?OnQueryHitPoint@COleControl@@UEAAHKPEBUtagRECT@@UtagPOINT@@JPEAK@Z
extern "C" int MS_ABI impl__OnQueryHitPoint_COleControl__UEAAHKPEBUtagRECT__UtagPOINT__JPEAK_Z(
    COleControl* /*pThis*/, unsigned long dwAspect, const RECT* /*pRectBounds*/,
    unsigned long long /*ptlLoc*/, long /*lCloseHint*/, unsigned long* pHitResult)
{
    if (dwAspect != DVASPECT_CONTENT) return FALSE;
    if (pHitResult) *pHitResult = HITRESULT_HIT;
    return TRUE;
}

// Symbol: ?OnQueryHitRect@COleControl@@UEAAHKPEBUtagRECT@@0JPEAK@Z
extern "C" int MS_ABI impl__OnQueryHitRect_COleControl__UEAAHKPEBUtagRECT__0JPEAK_Z(
    COleControl* /*pThis*/, unsigned long dwAspect, const RECT* /*pRectBounds*/,
    const RECT* /*prcLoc*/, long /*lCloseHint*/, unsigned long* pHitResult)
{
    if (dwAspect != DVASPECT_CONTENT) return FALSE;
    if (pHitResult) *pHitResult = HITRESULT_HIT;
    return TRUE;
}

// COleControl::OnRButtonDblClk(UINT, CPoint) - retail mfc140u 0x1e4240, the same
// body as OnLButtonDblClk/OnMButtonDblClk above with the button code 2:
//     HWND hWnd = m_hWnd;
//     ButtonDblClk(2, nFlags, point);          // direct call 0x1e3fd0
//     if (m_hWnd == hWnd) Default();
// Symbol: ?OnRButtonDblClk@COleControl@@IEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnRButtonDblClk_COleControl__IEAAXIVCPoint___Z(
    COleControl* pThis, unsigned int nFlags, unsigned long long pt)
{
    if (!pThis) return;
    HWND hWnd = pThis->m_hWnd;
    impl__ButtonDblClk_COleControl__IEAAXGIVCPoint___Z(pThis, 2, nFlags, pt);
    if (pThis->m_hWnd == hWnd) {
        (void)impl__Default_CWnd__IEAA_JXZ(static_cast<CWnd*>(pThis));
    }
}

// COleControl::OnRButtonDown(UINT, CPoint) - retail mfc140u 0x1e4200 is a bare
// argument shuffle and tail jump to OnButtonDown(2, nFlags, point) at 0x1e4280.
// Note it does NOT carry the m_hWnd/Default() tail that OnRButtonDblClk has.
// Symbol: ?OnRButtonDown@COleControl@@IEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnRButtonDown_COleControl__IEAAXIVCPoint___Z(
    COleControl* pThis, unsigned int nFlags, unsigned long long pt)
{
    if (!pThis) return;
    impl__OnButtonDown_COleControl__IEAAXGIVCPoint___Z(pThis, 2, nFlags, pt);
}

// COleControl::OnRButtonUp(UINT, CPoint) - retail mfc140u 0x1e4220 tail-jumps to
// OnButtonUp(2, nFlags, point) at 0x1e42d0.
// Symbol: ?OnRButtonUp@COleControl@@IEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnRButtonUp_COleControl__IEAAXIVCPoint___Z(
    COleControl* pThis, unsigned int nFlags, unsigned long long pt)
{
    if (!pThis) return;
    impl__OnButtonUp_COleControl__IEAAXGIVCPoint___Z(pThis, 2, nFlags, pt);
}

// Symbol: ?OnReflectorDestroyed@COleControl@@MEAAXXZ
extern "C" void MS_ABI impl__OnReflectorDestroyed_COleControl__MEAAXXZ() {}

// Symbol: ?OnRenderData@COleControl@@UEAAHPEAUtagFORMATETC@@PEAUtagSTGMEDIUM@@@Z
extern "C" int MS_ABI impl__OnRenderData_COleControl__UEAAHPEAUtagFORMATETC__PEAUtagSTGMEDIUM___Z(void* /*struct*/* p0, void* /*struct*/* p1) {
    return 0;
}

// Symbol: ?OnRenderFileData@COleControl@@UEAAHPEAUtagFORMATETC@@PEAVCFile@@@Z
extern "C" int MS_ABI impl__OnRenderFileData_COleControl__UEAAHPEAUtagFORMATETC__PEAVCFile___Z(void* /*struct*/* p0, void* /*class*/* p1) {
    return 0;
}

// Symbol: ?OnRenderGlobalData@COleControl@@UEAAHPEAUtagFORMATETC@@PEAPEAX@Z
extern "C" int MS_ABI impl__OnRenderGlobalData_COleControl__UEAAHPEAUtagFORMATETC__PEAPEAX_Z(void* /*struct*/* p0, void** p1) {
    return 0;
}

// Symbol: ?OnSetCursor@COleControl@@IEAAHPEAVCWnd@@II@Z
extern "C" int MS_ABI impl__OnSetCursor_COleControl__IEAAHPEAVCWnd__II_Z(void* /*class*/* p0, unsigned int p1, unsigned int p2) {
    return 0;
}

// Symbol: ?OnSetData@COleControl@@UEAAHPEAUtagFORMATETC@@PEAUtagSTGMEDIUM@@H@Z
extern "C" int MS_ABI impl__OnSetData_COleControl__UEAAHPEAUtagFORMATETC__PEAUtagSTGMEDIUM__H_Z(void* /*struct*/* p0, void* /*struct*/* p1, int p2) {
    return 0;
}

// Symbol: ?OnSetFocus@COleControl@@IEAAXPEAVCWnd@@@Z
extern "C" void MS_ABI impl__OnSetFocus_COleControl__IEAAXPEAVCWnd___Z(void* /*class*/* p0) {}

// Symbol: ?OnSetMessageString@COleControl@@IEAA_J_K_J@Z
extern "C" __int64 MS_ABI impl__OnSetMessageString_COleControl__IEAA_J_K_J_Z(unsigned __int64 p0, __int64 p1) {
    return 0;
}

// Symbol: ?OnSetText@COleControl@@IEAAHPEB_W@Z
extern "C" int MS_ABI impl__OnSetText_COleControl__IEAAHPEB_W_Z(const wchar_t* p0) {
    return 0;
}

// Symbol: ?OnShowToolBars@COleControl@@UEAAXXZ
extern "C" void MS_ABI impl__OnShowToolBars_COleControl__UEAAXXZ() {}

// Symbol: ?OnShowWindow@COleControl@@IEAAXHI@Z
extern "C" void MS_ABI impl__OnShowWindow_COleControl__IEAAXHI_Z(int p0, unsigned int p1) {}

// Symbol: ?OnSize@COleControl@@IEAAXIHH@Z
extern "C" void MS_ABI impl__OnSize_COleControl__IEAAXIHH_Z(unsigned int p0, int p1, int p2) {}

// Symbol: ?OnSysKeyDown@COleControl@@IEAAXIII@Z
extern "C" void MS_ABI impl__OnSysKeyDown_COleControl__IEAAXIII_Z(unsigned int p0, unsigned int p1, unsigned int p2) {}

// Symbol: ?OnSysKeyUp@COleControl@@IEAAXIII@Z
extern "C" void MS_ABI impl__OnSysKeyUp_COleControl__IEAAXIII_Z(unsigned int p0, unsigned int p1, unsigned int p2) {}

// Symbol: ?OnTextChanged@COleControl@@UEAAXXZ
extern "C" void MS_ABI impl__OnTextChanged_COleControl__UEAAXXZ() {}

// Symbol: ?OnWindowlessMessage@COleControl@@UEAAHI_K_JPEA_J@Z
extern "C" int MS_ABI impl__OnWindowlessMessage_COleControl__UEAAHI_K_JPEA_J_Z(unsigned int p0, unsigned __int64 p1, __int64 p2, __int64* p3) {
    return 0;
}

// Symbol: ?ParentToClient@COleControl@@UEBAIPEBUtagRECT@@PEAUtagPOINT@@H@Z
extern "C" unsigned int MS_ABI impl__ParentToClient_COleControl__UEBAIPEBUtagRECT__PEAUtagPOINT__H_Z(const void* /*struct*/* p0, void* /*struct*/* p1, int p2) {
    return 0;
}

// Symbol: ?PostModalDialog@COleControl@@QEAAXPEAUHWND__@@@Z
extern "C" void MS_ABI impl__PostModalDialog_COleControl__QEAAXPEAUHWND_____Z(void* /*struct*/* p0) {}

// Symbol: ?PreModalDialog@COleControl@@QEAAXPEAUHWND__@@@Z
extern "C" void MS_ABI impl__PreModalDialog_COleControl__QEAAXPEAUHWND_____Z(void* /*struct*/* p0) {}

// Symbol: ?QueryDefHandler@COleControl@@IEAAPEAXAEBU_GUID@@@Z
extern "C" void* MS_ABI impl__QueryDefHandler_COleControl__IEAAPEAXAEBU_GUID___Z(const void* /*struct*/* p0) {
    return nullptr;
}

// Symbol: ?RecreateControlWindow@COleControl@@QEAAXXZ
extern "C" void MS_ABI impl__RecreateControlWindow_COleControl__QEAAXXZ() {}

// Symbol: ?Refresh@COleControl@@QEAAXXZ
extern "C" void MS_ABI impl__Refresh_COleControl__QEAAXXZ() {}

// Symbol: ?ReleaseCaches@COleControl@@IEAAXXZ
extern "C" void MS_ABI impl__ReleaseCaches_COleControl__IEAAXXZ() {}

// Symbol: ?ReleaseDC@COleControl@@QEAAHPEAVCDC@@@Z
extern "C" int MS_ABI impl__ReleaseDC_COleControl__QEAAHPEAVCDC___Z(void* /*class*/* p0) {
    return 0;
}

// Symbol: ?RemoveFrameLevelUI@COleControl@@MEAAXXZ
extern "C" void MS_ABI impl__RemoveFrameLevelUI_COleControl__MEAAXXZ() {}

// Symbol: ?ReparentControlWindow@COleControl@@UEAAXPEAUHWND__@@0@Z
extern "C" void MS_ABI impl__ReparentControlWindow_COleControl__UEAAXPEAUHWND____0_Z(void* /*struct*/* p0, void* /*struct*/* p1) {}

// Symbol: ?ResetStockProps@COleControl@@QEAAXXZ
extern "C" void MS_ABI impl__ResetStockProps_COleControl__QEAAXXZ() {}

// Symbol: ?ResetVersion@COleControl@@QEAAXK@Z
extern "C" void MS_ABI impl__ResetVersion_COleControl__QEAAXK_Z(unsigned long p0) {}

// Symbol: ?ResizeFrameWindow@COleControl@@MEAAXHH@Z
extern "C" void MS_ABI impl__ResizeFrameWindow_COleControl__MEAAXHH_Z(int p0, int p1) {}

// Symbol: ?ResizeOpenControl@COleControl@@IEAAXHH@Z
extern "C" void MS_ABI impl__ResizeOpenControl_COleControl__IEAAXHH_Z(int p0, int p1) {}

// Symbol: ?SaveState@COleControl@@IEAAJPEAUIStream@@@Z
extern "C" long MS_ABI impl__SaveState_COleControl__IEAAJPEAUIStream___Z(void* /*struct*/* p0) {
    return 0;
}

// Symbol: ?ScrollWindow@COleControl@@QEAAXHHPEBUtagRECT@@0@Z
extern "C" void MS_ABI impl__ScrollWindow_COleControl__QEAAXHHPEBUtagRECT__0_Z(int p0, int p1, const void* /*struct*/* p2, int p3) {}

// Symbol: ?SelectFontObject@COleControl@@QEAAPEAVCFont@@PEAVCDC@@AEAVCFontHolder@@@Z
extern "C" void* MS_ABI impl__SelectFontObject_COleControl__QEAAPEAVCFont__PEAVCDC__AEAVCFontHolder___Z(void* /*class*/* p0, void* /*class*/* p1) {
    return nullptr;
}

// Symbol: ?SelectStockFont@COleControl@@QEAAPEAVCFont@@PEAVCDC@@@Z
extern "C" void* MS_ABI impl__SelectStockFont_COleControl__QEAAPEAVCFont__PEAVCDC___Z(void* /*class*/* p0) {
    return nullptr;
}

// Symbol: ?SendAdvise@COleControl@@IEAAXI@Z
extern "C" void MS_ABI impl__SendAdvise_COleControl__IEAAXI_Z(unsigned int p0) {}

// Symbol: ?SerializeExtent@COleControl@@QEAAXAEAVCArchive@@@Z
extern "C" void MS_ABI impl__SerializeExtent_COleControl__QEAAXAEAVCArchive___Z(void* /*class*/* p0) {}

// Symbol: ?SerializeStockProps@COleControl@@QEAAXAEAVCArchive@@@Z
extern "C" void MS_ABI impl__SerializeStockProps_COleControl__QEAAXAEAVCArchive___Z(void* /*class*/* p0) {}

// Symbol: ?SerializeVersion@COleControl@@QEAAKAEAVCArchive@@KH@Z
extern "C" unsigned long MS_ABI impl__SerializeVersion_COleControl__QEAAKAEAVCArchive__KH_Z(void* /*class*/* p0, unsigned long p1, int p2) {
    return 0;
}

// Symbol: ?SetFocus@COleControl@@QEAAPEAVCWnd@@XZ
extern "C" void* MS_ABI impl__SetFocus_COleControl__QEAAPEAVCWnd__XZ() {
    return nullptr;
}

// Symbol: ?SetInitialDataFormats@COleControl@@MEAAXXZ
extern "C" void MS_ABI impl__SetInitialDataFormats_COleControl__MEAAXXZ() {}

// Symbol: ?SetNotPermitted@COleControl@@QEAAXXZ
extern "C" void MS_ABI impl__SetNotPermitted_COleControl__QEAAXXZ() {}

// Symbol: ?SetNotSupported@COleControl@@QEAAXXZ
extern "C" void MS_ABI impl__SetNotSupported_COleControl__QEAAXXZ() {}

// Symbol: ?SetPropsetData@COleControl@@IEAAHPEAUtagFORMATETC@@PEAUtagSTGMEDIUM@@AEBU_GUID@@@Z
extern "C" int MS_ABI impl__SetPropsetData_COleControl__IEAAHPEAUtagFORMATETC__PEAUtagSTGMEDIUM__AEBU_GUID___Z(void* /*struct*/* p0, void* /*struct*/* p1, const void* /*struct*/* p2) {
    return 0;
}

// Symbol: ?SetRectInContainer@COleControl@@QEAAHPEBUtagRECT@@@Z
extern "C" int MS_ABI impl__SetRectInContainer_COleControl__QEAAHPEBUtagRECT___Z(const void* /*struct*/* p0) {
    return 0;
}

// Symbol: ?ThrowError@COleControl@@QEAAXJII@Z
extern "C" void MS_ABI impl__ThrowError_COleControl__QEAAXJII_Z(long p0, unsigned int p1, unsigned int p2) {}

// Symbol: ?ThrowError@COleControl@@QEAAXJPEB_WI@Z
extern "C" void MS_ABI impl__ThrowError_COleControl__QEAAXJPEB_WI_Z(long p0, const wchar_t* p1, unsigned int p2) {}

// Symbol: ?TransformCoords@COleControl@@QEAAXPEAU_POINTL@@PEAUtagPOINTF@@K@Z
extern "C" void MS_ABI impl__TransformCoords_COleControl__QEAAXPEAU_POINTL__PEAUtagPOINTF__K_Z(void* /*struct*/* p0, void* /*struct*/* p1, unsigned long p2) {}

// Symbol: ?TranslateColor@COleControl@@QEAAKKPEAUHPALETTE__@@@Z
extern "C" unsigned long MS_ABI impl__TranslateColor_COleControl__QEAAKKPEAUHPALETTE_____Z(unsigned long p0, void* /*struct*/* p1) {
    return 0;
}

// Symbol: ?WillAmbientsBeValidDuringLoad@COleControl@@QEAAHXZ
extern "C" int MS_ABI impl__WillAmbientsBeValidDuringLoad_COleControl__QEAAHXZ() {
    return 0;
}

// Symbol: ?WindowProc@COleControl@@UEAA_JI_K_J@Z
extern "C" __int64 MS_ABI impl__WindowProc_COleControl__UEAA_JI_K_J_Z(unsigned int p0, unsigned __int64 p1, __int64 p2) {
    return 0;
}

// Symbol: ?Advise@XOleObject@COleControl@@UEAAJPEAUIAdviseSink@@PEAK@Z
extern "C" long MS_ABI impl__Advise_XOleObject_COleControl__UEAAJPEAUIAdviseSink__PEAK_Z(void* /*struct*/* p0, unsigned long* p1) {
    return 0;
}

// Symbol: ?Cache@XOleCache@COleControl@@UEAAJPEAUtagFORMATETC@@KPEAK@Z
extern "C" long MS_ABI impl__Cache_XOleCache_COleControl__UEAAJPEAUtagFORMATETC__KPEAK_Z(void* /*struct*/* p0, unsigned long p1, unsigned long* p2) {
    return 0;
}

// Symbol: ?Close@XOleObject@COleControl@@UEAAJK@Z
extern "C" long MS_ABI impl__Close_XOleObject_COleControl__UEAAJK_Z(unsigned long p0) {
    return 0;
}

// Symbol: ?ContextSensitiveHelp@XOleInPlaceActiveObject@COleControl@@UEAAJH@Z
extern "C" long MS_ABI impl__ContextSensitiveHelp_XOleInPlaceActiveObject_COleControl__UEAAJH_Z(int p0) {
    return 0;
}

// Symbol: ?ContextSensitiveHelp@XOleInPlaceObject@COleControl@@UEAAJH@Z
extern "C" long MS_ABI impl__ContextSensitiveHelp_XOleInPlaceObject_COleControl__UEAAJH_Z(int p0) {
    return 0;
}

// Symbol: ?DAdvise@XDataObject@COleControl@@UEAAJPEAUtagFORMATETC@@KPEAUIAdviseSink@@PEAK@Z
extern "C" long MS_ABI impl__DAdvise_XDataObject_COleControl__UEAAJPEAUtagFORMATETC__KPEAUIAdviseSink__PEAK_Z(void* /*struct*/* p0, unsigned long p1, void* /*struct*/* p2, unsigned long* p3) {
    return 0;
}

// Symbol: ?DUnadvise@XDataObject@COleControl@@UEAAJK@Z
extern "C" long MS_ABI impl__DUnadvise_XDataObject_COleControl__UEAAJK_Z(unsigned long p0) {
    return 0;
}

// Symbol: ?DoVerb@XOleObject@COleControl@@UEAAJJPEAUtagMSG@@PEAUIOleClientSite@@JPEAUHWND__@@PEBUtagRECT@@@Z
extern "C" long MS_ABI impl__DoVerb_XOleObject_COleControl__UEAAJJPEAUtagMSG__PEAUIOleClientSite__JPEAUHWND____PEBUtagRECT___Z(long p0, void* /*struct*/* p1, void* /*struct*/* p2, long p3, void* /*struct*/* p4, const void* /*struct*/* p5) {
    return 0;
}

// Symbol: ?Draw@XViewObject@COleControl@@UEAAJKJPEAXPEAUtagDVTARGETDEVICE@@PEAUHDC__@@2PEBU_RECTL@@3P6AH_K@Z4@Z
extern "C" long MS_ABI impl__Draw_XViewObject_COleControl__UEAAJKJPEAXPEAUtagDVTARGETDEVICE__PEAUHDC____2PEBU_RECTL__3P6AH_K_Z4_Z(unsigned long p0, long p1, void* p2, void* /*struct*/* p3, void* /*struct*/* p4, void* p5, const void* /*struct*/* p6, void* /*struct*/* p7, void* /*fnptr*/ p8, const void* /*struct*/* p9) {
    return 0;
}

// Symbol: ?EnableModeless@XOleInPlaceActiveObject@COleControl@@UEAAJH@Z
extern "C" long MS_ABI impl__EnableModeless_XOleInPlaceActiveObject_COleControl__UEAAJH_Z(int p0) {
    return 0;
}

// Symbol: ?EnumAdvise@XOleObject@COleControl@@UEAAJPEAPEAUIEnumSTATDATA@@@Z
extern "C" long MS_ABI impl__EnumAdvise_XOleObject_COleControl__UEAAJPEAPEAUIEnumSTATDATA___Z(void* /*struct*/** p0) {
    return 0;
}

// Symbol: ?EnumCache@XOleCache@COleControl@@UEAAJPEAPEAUIEnumSTATDATA@@@Z
extern "C" long MS_ABI impl__EnumCache_XOleCache_COleControl__UEAAJPEAPEAUIEnumSTATDATA___Z(void* /*struct*/** p0) {
    return 0;
}

// Symbol: ?EnumDAdvise@XDataObject@COleControl@@UEAAJPEAPEAUIEnumSTATDATA@@@Z
extern "C" long MS_ABI impl__EnumDAdvise_XDataObject_COleControl__UEAAJPEAPEAUIEnumSTATDATA___Z(void* /*struct*/** p0) {
    return 0;
}

// Symbol: ?EnumFormatEtc@XDataObject@COleControl@@UEAAJKPEAPEAUIEnumFORMATETC@@@Z
extern "C" long MS_ABI impl__EnumFormatEtc_XDataObject_COleControl__UEAAJKPEAPEAUIEnumFORMATETC___Z(unsigned long p0, void* /*struct*/** p1) {
    return 0;
}

// Symbol: ?EnumVerbs@XOleObject@COleControl@@UEAAJPEAPEAUIEnumOLEVERB@@@Z
extern "C" long MS_ABI impl__EnumVerbs_XOleObject_COleControl__UEAAJPEAPEAUIEnumOLEVERB___Z(void* /*struct*/** p0) {
    return 0;
}

// Symbol: ?Freeze@XViewObject@COleControl@@UEAAJKJPEAXPEAK@Z
extern "C" long MS_ABI impl__Freeze_XViewObject_COleControl__UEAAJKJPEAXPEAK_Z(unsigned long p0, long p1, void* p2, unsigned long* p3) {
    return 0;
}

// Symbol: ?FreezeEvents@XOleControl@COleControl@@UEAAJH@Z
extern "C" long MS_ABI impl__FreezeEvents_XOleControl_COleControl__UEAAJH_Z(int p0) {
    return 0;
}

// Symbol: ?GetActivationPolicy@XPointerInactive@COleControl@@UEAAJPEAK@Z
extern "C" long MS_ABI impl__GetActivationPolicy_XPointerInactive_COleControl__UEAAJPEAK_Z(unsigned long* p0) {
    return 0;
}

// Symbol: ?GetAdvise@XViewObject@COleControl@@UEAAJPEAK0PEAPEAUIAdviseSink@@@Z
extern "C" long MS_ABI impl__GetAdvise_XViewObject_COleControl__UEAAJPEAK0PEAPEAUIAdviseSink___Z(unsigned long* p0, unsigned long* p1, void* /*struct*/** p2) {
    return 0;
}

// Symbol: ?GetCanonicalFormatEtc@XDataObject@COleControl@@UEAAJPEAUtagFORMATETC@@0@Z
extern "C" long MS_ABI impl__GetCanonicalFormatEtc_XDataObject_COleControl__UEAAJPEAUtagFORMATETC__0_Z(void* /*struct*/* p0, void* /*struct*/* p1) {
    return 0;
}

// Symbol: ?GetClassID@XPersistMemory@COleControl@@UEAAJPEAU_GUID@@@Z
extern "C" long MS_ABI impl__GetClassID_XPersistMemory_COleControl__UEAAJPEAU_GUID___Z(void* /*struct*/* p0) {
    return 0;
}

// Symbol: ?GetClassID@XPersistPropertyBag@COleControl@@UEAAJPEAU_GUID@@@Z
extern "C" long MS_ABI impl__GetClassID_XPersistPropertyBag_COleControl__UEAAJPEAU_GUID___Z(void* /*struct*/* p0) {
    return 0;
}

// Symbol: ?GetClassID@XPersistStorage@COleControl@@UEAAJPEAU_GUID@@@Z
extern "C" long MS_ABI impl__GetClassID_XPersistStorage_COleControl__UEAAJPEAU_GUID___Z(void* /*struct*/* p0) {
    return 0;
}

// Symbol: ?GetClassID@XPersistStreamInit@COleControl@@UEAAJPEAU_GUID@@@Z
extern "C" long MS_ABI impl__GetClassID_XPersistStreamInit_COleControl__UEAAJPEAU_GUID___Z(void* /*struct*/* p0) {
    return 0;
}

// Symbol: ?GetClassInfoW@XProvideClassInfo@COleControl@@UEAAJPEAPEAUITypeInfo@@@Z
extern "C" long MS_ABI impl__GetClassInfoW_XProvideClassInfo_COleControl__UEAAJPEAPEAUITypeInfo___Z(void* /*struct*/** p0) {
    return 0;
}

// Symbol: ?GetClientSite@XOleObject@COleControl@@UEAAJPEAPEAUIOleClientSite@@@Z
extern "C" long MS_ABI impl__GetClientSite_XOleObject_COleControl__UEAAJPEAPEAUIOleClientSite___Z(void* /*struct*/** p0) {
    return 0;
}

// Symbol: ?GetClipboardData@XOleObject@COleControl@@UEAAJKPEAPEAUIDataObject@@@Z
extern "C" long MS_ABI impl__GetClipboardData_XOleObject_COleControl__UEAAJKPEAPEAUIDataObject___Z(unsigned long p0, void* /*struct*/** p1) {
    return 0;
}

// Symbol: ?GetColorSet@XViewObject@COleControl@@UEAAJKJPEAXPEAUtagDVTARGETDEVICE@@PEAUHDC__@@PEAPEAUtagLOGPALETTE@@@Z
extern "C" long MS_ABI impl__GetColorSet_XViewObject_COleControl__UEAAJKJPEAXPEAUtagDVTARGETDEVICE__PEAUHDC____PEAPEAUtagLOGPALETTE___Z(unsigned long p0, long p1, void* p2, void* /*struct*/* p3, void* /*struct*/* p4, void* /*struct*/** p5) {
    return 0;
}

// Symbol: ?GetContentExtent@XQuickActivate@COleControl@@UEAAJPEAUtagSIZE@@@Z
extern "C" long MS_ABI impl__GetContentExtent_XQuickActivate_COleControl__UEAAJPEAUtagSIZE___Z(void* /*struct*/* p0) {
    return 0;
}

// Symbol: ?GetControlInfo@XOleControl@COleControl@@UEAAJPEAUtagCONTROLINFO@@@Z
extern "C" long MS_ABI impl__GetControlInfo_XOleControl_COleControl__UEAAJPEAUtagCONTROLINFO___Z(void* /*struct*/* p0) {
    return 0;
}

// Symbol: ?GetData@XDataObject@COleControl@@UEAAJPEAUtagFORMATETC@@PEAUtagSTGMEDIUM@@@Z
extern "C" long MS_ABI impl__GetData_XDataObject_COleControl__UEAAJPEAUtagFORMATETC__PEAUtagSTGMEDIUM___Z(void* /*struct*/* p0, void* /*struct*/* p1) {
    return 0;
}

// Symbol: ?GetDataHere@XDataObject@COleControl@@UEAAJPEAUtagFORMATETC@@PEAUtagSTGMEDIUM@@@Z
extern "C" long MS_ABI impl__GetDataHere_XDataObject_COleControl__UEAAJPEAUtagFORMATETC__PEAUtagSTGMEDIUM___Z(void* /*struct*/* p0, void* /*struct*/* p1) {
    return 0;
}

// Symbol: ?GetDisplayString@XPerPropertyBrowsing@COleControl@@UEAAJJPEAPEA_W@Z
extern "C" long MS_ABI impl__GetDisplayString_XPerPropertyBrowsing_COleControl__UEAAJJPEAPEA_W_Z(long p0, wchar_t** p1) {
    return 0;
}

// Symbol: ?GetDropTarget@XOleInPlaceObject@COleControl@@UEAAJPEAPEAUIDropTarget@@@Z
extern "C" long MS_ABI impl__GetDropTarget_XOleInPlaceObject_COleControl__UEAAJPEAPEAUIDropTarget___Z(void* /*struct*/** p0) {
    return 0;
}

// Symbol: ?GetExtent@XOleObject@COleControl@@UEAAJKPEAUtagSIZE@@@Z
extern "C" long MS_ABI impl__GetExtent_XOleObject_COleControl__UEAAJKPEAUtagSIZE___Z(unsigned long p0, void* /*struct*/* p1) {
    return 0;
}

// Symbol: ?GetExtent@XViewObject@COleControl@@UEAAJKJPEAUtagDVTARGETDEVICE@@PEAUtagSIZE@@@Z
extern "C" long MS_ABI impl__GetExtent_XViewObject_COleControl__UEAAJKJPEAUtagDVTARGETDEVICE__PEAUtagSIZE___Z(unsigned long p0, long p1, void* /*struct*/* p2, void* /*struct*/* p3) {
    return 0;
}

// Symbol: ?GetGUID@XProvideClassInfo@COleControl@@UEAAJKPEAU_GUID@@@Z
extern "C" long MS_ABI impl__GetGUID_XProvideClassInfo_COleControl__UEAAJKPEAU_GUID___Z(unsigned long p0, void* /*struct*/* p1) {
    return 0;
}

// Symbol: ?GetIID@XEventConnPt@COleControl@@UEAAAEBU_GUID@@XZ
extern "C" void* MS_ABI impl__GetIID_XEventConnPt_COleControl__UEAAAEBU_GUID__XZ(void* p0, void* /*struct*/ p1) {
    return nullptr;
}

// Symbol: ?GetMiscStatus@XOleObject@COleControl@@UEAAJKPEAK@Z
extern "C" long MS_ABI impl__GetMiscStatus_XOleObject_COleControl__UEAAJKPEAK_Z(unsigned long p0, unsigned long* p1) {
    return 0;
}

// Symbol: ?GetMoniker@XOleObject@COleControl@@UEAAJKKPEAPEAUIMoniker@@@Z
extern "C" long MS_ABI impl__GetMoniker_XOleObject_COleControl__UEAAJKKPEAPEAUIMoniker___Z(unsigned long p0, unsigned long p1, void* /*struct*/** p2) {
    return 0;
}

// Symbol: ?GetNaturalExtent@XViewObject@COleControl@@UEAAJKJPEAUtagDVTARGETDEVICE@@PEAUHDC__@@PEAUtagExtentInfo@@PEAUtagSIZE@@@Z
extern "C" long MS_ABI impl__GetNaturalExtent_XViewObject_COleControl__UEAAJKJPEAUtagDVTARGETDEVICE__PEAUHDC____PEAUtagExtentInfo__PEAUtagSIZE___Z(unsigned long p0, long p1, void* /*struct*/* p2, void* /*struct*/* p3, void* /*struct*/* p4, void* /*struct*/* p5) {
    return 0;
}

// Symbol: ?GetPages@XSpecifyPropertyPages@COleControl@@UEAAJPEAUtagCAUUID@@@Z
extern "C" long MS_ABI impl__GetPages_XSpecifyPropertyPages_COleControl__UEAAJPEAUtagCAUUID___Z(void* /*struct*/* p0) {
    return 0;
}

// Symbol: ?GetPredefinedStrings@XPerPropertyBrowsing@COleControl@@UEAAJJPEAUtagCALPOLESTR@@PEAUtagCADWORD@@@Z
extern "C" long MS_ABI impl__GetPredefinedStrings_XPerPropertyBrowsing_COleControl__UEAAJJPEAUtagCALPOLESTR__PEAUtagCADWORD___Z(long p0, void* /*struct*/* p1, void* /*struct*/* p2) {
    return 0;
}

// Symbol: ?GetPredefinedValue@XPerPropertyBrowsing@COleControl@@UEAAJJKPEAUtagVARIANT@@@Z
extern "C" long MS_ABI impl__GetPredefinedValue_XPerPropertyBrowsing_COleControl__UEAAJJKPEAUtagVARIANT___Z(long p0, unsigned long p1, void* /*struct*/* p2) {
    return 0;
}

// Symbol: ?GetRect@XViewObject@COleControl@@UEAAJKPEAU_RECTL@@@Z
extern "C" long MS_ABI impl__GetRect_XViewObject_COleControl__UEAAJKPEAU_RECTL___Z(unsigned long p0, void* /*struct*/* p1) {
    return 0;
}

// Symbol: ?GetSizeMax@XPersistMemory@COleControl@@UEAAJPEAK@Z
extern "C" long MS_ABI impl__GetSizeMax_XPersistMemory_COleControl__UEAAJPEAK_Z(unsigned long* p0) {
    return 0;
}

// Symbol: ?GetSizeMax@XPersistStreamInit@COleControl@@UEAAJPEAT_ULARGE_INTEGER@@@Z
extern "C" long MS_ABI impl__GetSizeMax_XPersistStreamInit_COleControl__UEAAJPEAT_ULARGE_INTEGER___Z(void** p0, int p1, void* /*struct*/ p2) {
    return 0;
}

// Symbol: ?GetUserClassID@XOleObject@COleControl@@UEAAJPEAU_GUID@@@Z
extern "C" long MS_ABI impl__GetUserClassID_XOleObject_COleControl__UEAAJPEAU_GUID___Z(void* /*struct*/* p0) {
    return 0;
}

// Symbol: ?GetUserType@XOleObject@COleControl@@UEAAJKPEAPEA_W@Z
extern "C" long MS_ABI impl__GetUserType_XOleObject_COleControl__UEAAJKPEAPEA_W_Z(unsigned long p0, wchar_t** p1) {
    return 0;
}

// Symbol: ?GetViewStatus@XViewObject@COleControl@@UEAAJPEAK@Z
extern "C" long MS_ABI impl__GetViewStatus_XViewObject_COleControl__UEAAJPEAK_Z(unsigned long* p0) {
    return 0;
}

// Symbol: ?GetWindow@XOleInPlaceActiveObject@COleControl@@UEAAJPEAPEAUHWND__@@@Z
extern "C" long MS_ABI impl__GetWindow_XOleInPlaceActiveObject_COleControl__UEAAJPEAPEAUHWND_____Z(void* /*struct*/** p0) {
    return 0;
}

// Symbol: ?GetWindow@XOleInPlaceObject@COleControl@@UEAAJPEAPEAUHWND__@@@Z
extern "C" long MS_ABI impl__GetWindow_XOleInPlaceObject_COleControl__UEAAJPEAPEAUHWND_____Z(void* /*struct*/** p0) {
    return 0;
}

// Symbol: ?HandsOffStorage@XPersistStorage@COleControl@@UEAAJXZ
extern "C" long MS_ABI impl__HandsOffStorage_XPersistStorage_COleControl__UEAAJXZ() {
    return 0;
}

// Symbol: ?InPlaceDeactivate@XOleInPlaceObject@COleControl@@UEAAJXZ
extern "C" long MS_ABI impl__InPlaceDeactivate_XOleInPlaceObject_COleControl__UEAAJXZ() {
    return 0;
}

// Symbol: ?InitCache@XOleCache@COleControl@@UEAAJPEAUIDataObject@@@Z
extern "C" long MS_ABI impl__InitCache_XOleCache_COleControl__UEAAJPEAUIDataObject___Z(void* /*struct*/* p0) {
    return 0;
}

// Symbol: ?InitFromData@XOleObject@COleControl@@UEAAJPEAUIDataObject@@HK@Z
extern "C" long MS_ABI impl__InitFromData_XOleObject_COleControl__UEAAJPEAUIDataObject__HK_Z(void* /*struct*/* p0, int p1, unsigned long p2) {
    return 0;
}

// Symbol: ?InitNew@XPersistMemory@COleControl@@UEAAJXZ
extern "C" long MS_ABI impl__InitNew_XPersistMemory_COleControl__UEAAJXZ() {
    return 0;
}

// Symbol: ?InitNew@XPersistPropertyBag@COleControl@@UEAAJXZ
extern "C" long MS_ABI impl__InitNew_XPersistPropertyBag_COleControl__UEAAJXZ() {
    return 0;
}

// Symbol: ?InitNew@XPersistStorage@COleControl@@UEAAJPEAUIStorage@@@Z
extern "C" long MS_ABI impl__InitNew_XPersistStorage_COleControl__UEAAJPEAUIStorage___Z(void* /*struct*/* p0) {
    return 0;
}

// Symbol: ?InitNew@XPersistStreamInit@COleControl@@UEAAJXZ
extern "C" long MS_ABI impl__InitNew_XPersistStreamInit_COleControl__UEAAJXZ() {
    return 0;
}

// Symbol: ?IsDirty@XPersistMemory@COleControl@@UEAAJXZ
extern "C" long MS_ABI impl__IsDirty_XPersistMemory_COleControl__UEAAJXZ() {
    return 0;
}

// Symbol: ?IsDirty@XPersistStorage@COleControl@@UEAAJXZ
extern "C" long MS_ABI impl__IsDirty_XPersistStorage_COleControl__UEAAJXZ() {
    return 0;
}

// Symbol: ?IsDirty@XPersistStreamInit@COleControl@@UEAAJXZ
extern "C" long MS_ABI impl__IsDirty_XPersistStreamInit_COleControl__UEAAJXZ() {
    return 0;
}

// Symbol: ?IsUpToDate@XOleObject@COleControl@@UEAAJXZ
extern "C" long MS_ABI impl__IsUpToDate_XOleObject_COleControl__UEAAJXZ() {
    return 0;
}

// Symbol: ?Load@XPersistMemory@COleControl@@UEAAJPEAXK@Z
extern "C" long MS_ABI impl__Load_XPersistMemory_COleControl__UEAAJPEAXK_Z(void* p0, unsigned long p1) {
    return 0;
}

// Symbol: ?Load@XPersistPropertyBag@COleControl@@UEAAJPEAUIPropertyBag@@PEAUIErrorLog@@@Z
extern "C" long MS_ABI impl__Load_XPersistPropertyBag_COleControl__UEAAJPEAUIPropertyBag__PEAUIErrorLog___Z(void* /*struct*/* p0, void* /*struct*/* p1) {
    return 0;
}

// Symbol: ?Load@XPersistStorage@COleControl@@UEAAJPEAUIStorage@@@Z
extern "C" long MS_ABI impl__Load_XPersistStorage_COleControl__UEAAJPEAUIStorage___Z(void* /*struct*/* p0) {
    return 0;
}

// Symbol: ?Load@XPersistStreamInit@COleControl@@UEAAJPEAUIStream@@@Z
extern "C" long MS_ABI impl__Load_XPersistStreamInit_COleControl__UEAAJPEAUIStream___Z(void* /*struct*/* p0) {
    return 0;
}

// Symbol: ?MapPropertyToPage@XPerPropertyBrowsing@COleControl@@UEAAJJPEAU_GUID@@@Z
extern "C" long MS_ABI impl__MapPropertyToPage_XPerPropertyBrowsing_COleControl__UEAAJJPEAU_GUID___Z(long p0, void* /*struct*/* p1) {
    return 0;
}

// Symbol: ?OnAdvise@XEventConnPt@COleControl@@UEAAXH@Z
extern "C" void MS_ABI impl__OnAdvise_XEventConnPt_COleControl__UEAAXH_Z(int p0) {}

// Symbol: ?OnAmbientPropertyChange@XOleControl@COleControl@@UEAAJJ@Z
extern "C" long MS_ABI impl__OnAmbientPropertyChange_XOleControl_COleControl__UEAAJJ_Z(long p0) {
    return 0;
}

// Symbol: ?OnChanged@XFontNotification@COleControl@@UEAAJJ@Z
extern "C" long MS_ABI impl__OnChanged_XFontNotification_COleControl__UEAAJJ_Z(long p0) {
    return 0;
}

// Symbol: ?OnDocWindowActivate@XOleInPlaceActiveObject@COleControl@@UEAAJH@Z
extern "C" long MS_ABI impl__OnDocWindowActivate_XOleInPlaceActiveObject_COleControl__UEAAJH_Z(int p0) {
    return 0;
}

// Symbol: ?OnFrameWindowActivate@XOleInPlaceActiveObject@COleControl@@UEAAJH@Z
extern "C" long MS_ABI impl__OnFrameWindowActivate_XOleInPlaceActiveObject_COleControl__UEAAJH_Z(int p0) {
    return 0;
}

// Symbol: ?OnInactiveMouseMove@XPointerInactive@COleControl@@UEAAJPEBUtagRECT@@JJK@Z
extern "C" long MS_ABI impl__OnInactiveMouseMove_XPointerInactive_COleControl__UEAAJPEBUtagRECT__JJK_Z(const void* /*struct*/* p0, long p1, long p2, unsigned long p3) {
    return 0;
}

// Symbol: ?OnInactiveSetCursor@XPointerInactive@COleControl@@UEAAJPEBUtagRECT@@JJKH@Z
extern "C" long MS_ABI impl__OnInactiveSetCursor_XPointerInactive_COleControl__UEAAJPEBUtagRECT__JJKH_Z(const void* /*struct*/* p0, long p1, long p2, unsigned long p3, int p4) {
    return 0;
}

// Symbol: ?OnMnemonic@XOleControl@COleControl@@UEAAJPEAUtagMSG@@@Z
extern "C" long MS_ABI impl__OnMnemonic_XOleControl_COleControl__UEAAJPEAUtagMSG___Z(void* /*struct*/* p0) {
    return 0;
}

// Symbol: ?OnRequestEdit@XFontNotification@COleControl@@UEAAJJ@Z
extern "C" long MS_ABI impl__OnRequestEdit_XFontNotification_COleControl__UEAAJJ_Z(long p0) {
    return 0;
}

// Symbol: ?OnWindowMessage@XOleInPlaceObject@COleControl@@UEAAJI_K_JPEA_J@Z
extern "C" long MS_ABI impl__OnWindowMessage_XOleInPlaceObject_COleControl__UEAAJI_K_JPEA_J_Z(unsigned int p0, unsigned __int64 p1, __int64 p2, __int64* p3) {
    return 0;
}

// Symbol: ?QueryGetData@XDataObject@COleControl@@UEAAJPEAUtagFORMATETC@@@Z
extern "C" long MS_ABI impl__QueryGetData_XDataObject_COleControl__UEAAJPEAUtagFORMATETC___Z(void* /*struct*/* p0) {
    return 0;
}

// Symbol: ?QueryHitPoint@XViewObject@COleControl@@UEAAJKPEBUtagRECT@@UtagPOINT@@JPEAK@Z
extern "C" long MS_ABI impl__QueryHitPoint_XViewObject_COleControl__UEAAJKPEBUtagRECT__UtagPOINT__JPEAK_Z(unsigned long p0, const void* /*struct*/* p1, void* /*struct*/ p2, long p3, unsigned long* p4) {
    return 0;
}

// Symbol: ?QueryHitRect@XViewObject@COleControl@@UEAAJKPEBUtagRECT@@0JPEAK@Z
extern "C" long MS_ABI impl__QueryHitRect_XViewObject_COleControl__UEAAJKPEBUtagRECT__0JPEAK_Z(unsigned long p0, const void* /*struct*/* p1, unsigned long p2, long p3, unsigned long* p4) {
    return 0;
}

// Symbol: ?QuerySinkInterface@XEventConnPt@COleControl@@UEAAJPEAUIUnknown@@PEAPEAX@Z
extern "C" long MS_ABI impl__QuerySinkInterface_XEventConnPt_COleControl__UEAAJPEAUIUnknown__PEAPEAX_Z(void* /*struct*/* p0, void** p1) {
    return 0;
}

// Symbol: ?QuickActivate@XQuickActivate@COleControl@@UEAAJPEAUtagQACONTAINER@@PEAUtagQACONTROL@@@Z
extern "C" long MS_ABI impl__QuickActivate_XQuickActivate_COleControl__UEAAJPEAUtagQACONTAINER__PEAUtagQACONTROL___Z(void* /*struct*/* p0, void* /*struct*/* p1) {
    return 0;
}

// Symbol: ?ReactivateAndUndo@XOleInPlaceObject@COleControl@@UEAAJXZ
extern "C" long MS_ABI impl__ReactivateAndUndo_XOleInPlaceObject_COleControl__UEAAJXZ() {
    return 0;
}

// Symbol: ?ResizeBorder@XOleInPlaceActiveObject@COleControl@@UEAAJPEBUtagRECT@@PEAUIOleInPlaceUIWindow@@H@Z
extern "C" long MS_ABI impl__ResizeBorder_XOleInPlaceActiveObject_COleControl__UEAAJPEBUtagRECT__PEAUIOleInPlaceUIWindow__H_Z(const void* /*struct*/* p0, void* /*struct*/* p1, int p2) {
    return 0;
}

// Symbol: ?Save@XPersistMemory@COleControl@@UEAAJPEAXHK@Z
extern "C" long MS_ABI impl__Save_XPersistMemory_COleControl__UEAAJPEAXHK_Z(void* p0, int p1, unsigned long p2) {
    return 0;
}

// Symbol: ?Save@XPersistPropertyBag@COleControl@@UEAAJPEAUIPropertyBag@@HH@Z
extern "C" long MS_ABI impl__Save_XPersistPropertyBag_COleControl__UEAAJPEAUIPropertyBag__HH_Z(void* /*struct*/* p0, int p1, int p2) {
    return 0;
}

// Symbol: ?Save@XPersistStorage@COleControl@@UEAAJPEAUIStorage@@H@Z
extern "C" long MS_ABI impl__Save_XPersistStorage_COleControl__UEAAJPEAUIStorage__H_Z(void* /*struct*/* p0, int p1) {
    return 0;
}

// Symbol: ?Save@XPersistStreamInit@COleControl@@UEAAJPEAUIStream@@H@Z
extern "C" long MS_ABI impl__Save_XPersistStreamInit_COleControl__UEAAJPEAUIStream__H_Z(void* /*struct*/* p0, int p1) {
    return 0;
}

// Symbol: ?SaveCompleted@XPersistStorage@COleControl@@UEAAJPEAUIStorage@@@Z
extern "C" long MS_ABI impl__SaveCompleted_XPersistStorage_COleControl__UEAAJPEAUIStorage___Z(void* /*struct*/* p0) {
    return 0;
}

// Symbol: ?SetAdvise@XViewObject@COleControl@@UEAAJKKPEAUIAdviseSink@@@Z
extern "C" long MS_ABI impl__SetAdvise_XViewObject_COleControl__UEAAJKKPEAUIAdviseSink___Z(unsigned long p0, unsigned long p1, void* /*struct*/* p2) {
    return 0;
}

// Symbol: ?SetClientSite@XOleObject@COleControl@@UEAAJPEAUIOleClientSite@@@Z
extern "C" long MS_ABI impl__SetClientSite_XOleObject_COleControl__UEAAJPEAUIOleClientSite___Z(void* /*struct*/* p0) {
    return 0;
}

// Symbol: ?SetColorScheme@XOleObject@COleControl@@UEAAJPEAUtagLOGPALETTE@@@Z
extern "C" long MS_ABI impl__SetColorScheme_XOleObject_COleControl__UEAAJPEAUtagLOGPALETTE___Z(void* /*struct*/* p0) {
    return 0;
}

// Symbol: ?SetContentExtent@XQuickActivate@COleControl@@UEAAJPEAUtagSIZE@@@Z
extern "C" long MS_ABI impl__SetContentExtent_XQuickActivate_COleControl__UEAAJPEAUtagSIZE___Z(void* /*struct*/* p0) {
    return 0;
}

// Symbol: ?SetData@XDataObject@COleControl@@UEAAJPEAUtagFORMATETC@@PEAUtagSTGMEDIUM@@H@Z
extern "C" long MS_ABI impl__SetData_XDataObject_COleControl__UEAAJPEAUtagFORMATETC__PEAUtagSTGMEDIUM__H_Z(void* /*struct*/* p0, void* /*struct*/* p1, int p2) {
    return 0;
}

// Symbol: ?SetData@XOleCache@COleControl@@UEAAJPEAUtagFORMATETC@@PEAUtagSTGMEDIUM@@H@Z
extern "C" long MS_ABI impl__SetData_XOleCache_COleControl__UEAAJPEAUtagFORMATETC__PEAUtagSTGMEDIUM__H_Z(void* /*struct*/* p0, void* /*struct*/* p1, int p2) {
    return 0;
}

// Symbol: ?SetExtent@XOleObject@COleControl@@UEAAJKPEAUtagSIZE@@@Z
extern "C" long MS_ABI impl__SetExtent_XOleObject_COleControl__UEAAJKPEAUtagSIZE___Z(unsigned long p0, void* /*struct*/* p1) {
    return 0;
}

// Symbol: ?SetHostNames@XOleObject@COleControl@@UEAAJPEB_W0@Z
extern "C" long MS_ABI impl__SetHostNames_XOleObject_COleControl__UEAAJPEB_W0_Z(const wchar_t* p0, const wchar_t* p1) {
    return 0;
}

// Symbol: ?SetMoniker@XOleObject@COleControl@@UEAAJKPEAUIMoniker@@@Z
extern "C" long MS_ABI impl__SetMoniker_XOleObject_COleControl__UEAAJKPEAUIMoniker___Z(unsigned long p0, void* /*struct*/* p1) {
    return 0;
}

// Symbol: ?SetObjectRects@XOleInPlaceObject@COleControl@@UEAAJPEBUtagRECT@@0@Z
extern "C" long MS_ABI impl__SetObjectRects_XOleInPlaceObject_COleControl__UEAAJPEBUtagRECT__0_Z(const void* /*struct*/* p0, const void* /*struct*/* p1) {
    return 0;
}

// Symbol: ?TranslateAcceleratorW@XOleInPlaceActiveObject@COleControl@@UEAAJPEAUtagMSG@@@Z
extern "C" long MS_ABI impl__TranslateAcceleratorW_XOleInPlaceActiveObject_COleControl__UEAAJPEAUtagMSG___Z(void* /*struct*/* p0) {
    return 0;
}

// Symbol: ?UIDeactivate@XOleInPlaceObject@COleControl@@UEAAJXZ
extern "C" long MS_ABI impl__UIDeactivate_XOleInPlaceObject_COleControl__UEAAJXZ() {
    return 0;
}

// Symbol: ?Unadvise@XOleObject@COleControl@@UEAAJK@Z
extern "C" long MS_ABI impl__Unadvise_XOleObject_COleControl__UEAAJK_Z(unsigned long p0) {
    return 0;
}

// Symbol: ?Uncache@XOleCache@COleControl@@UEAAJK@Z
extern "C" long MS_ABI impl__Uncache_XOleCache_COleControl__UEAAJK_Z(unsigned long p0) {
    return 0;
}

// Symbol: ?Unfreeze@XViewObject@COleControl@@UEAAJK@Z
extern "C" long MS_ABI impl__Unfreeze_XViewObject_COleControl__UEAAJK_Z(unsigned long p0) {
    return 0;
}

// Symbol: ?Update@XOleObject@COleControl@@UEAAJXZ
extern "C" long MS_ABI impl__Update_XOleObject_COleControl__UEAAJXZ() {
    return 0;
}
