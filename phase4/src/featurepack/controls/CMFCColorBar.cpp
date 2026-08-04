// CMFCColorBar — OpenMFC implementation.
// Sources: global_cmfccolorbar.cpp, manual_small_stub_implementations.cpp

#include "detail/CMFCColorBarSupport.h"
#include "detail/ManualSmallStubImplementationsSupport.h"

// Symbol: ?m_ColorNames@CMFCColorBar@@1V?$CMap@KKV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@PEB_W@@A
extern "C" CMap_KKCS_56Bytes impl__m_ColorNames_CMFCColorBar__1V__CMap_KKV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__PEB_W__A = {};
// CreateObject(): CRuntimeClass factory.  Retail (RVA 0x244d0) allocates a
// 0x1508-byte CMFCColorBar (operator new at 0x1800027f0) and runs the
// constructor (RVA 0x180024550).  The class is opaque in OpenMFC -- no
// constructor, vftable or member state is modeled -- so no valid object can
// be produced and the factory conservatively fails (same pattern as
// CMFCCaptionBar::CreateObject in global_cmfccaptionbar.cpp).
// Symbol: ?CreateObject@CMFCColorBar@@SAPEAVCObject@@XZ
extern "C" void* MS_ABI impl__CreateObject_CMFCColorBar__SAPEAVCObject__XZ(
    void)
{
    // TODO(clean-room): transcribed partially -- retail allocates 0x1508
    // bytes and runs the CMFCColorBar constructor (RVA 0x180024550, retail
    // vftable at 0x1803b1478); the opaque class has no modeled constructor or
    // vftable, so nullptr (the safe allocation-failure terminal) is returned.
    return nullptr;
}
// Create(CWnd* pWndParent, DWORD dwStyle, UINT nID, CPalette* pPalette, int
// nColumns, int nRows, int nRowsGutter): stores nColumns/nRows/nRowsGutter
// into 0x1420/0x1424/0x1428 when the color count (0x1468) is zero, runs the
// palette helper (0x1800265f0) and tail-calls the CWnd::CreateEx wrapper
// (0x18014d850).  The stores are transcribed from retail RVA 0x26380; the
// helpers are unmodeled, so 0 (failure) is returned.
// Symbol: ?Create@CMFCColorBar@@UEAAHPEAVCWnd@@KIPEAVCPalette@@HHH@Z
extern "C" int MS_ABI impl__Create_CMFCColorBar__UEAAHPEAVCWnd__KIPEAVCPalette__HHH_Z(
    void* pThis, void* /*pWndParent*/, unsigned long /*dwStyle*/,
    unsigned int /*nID*/, void* /*pPalette*/, int nColumns, int nRows,
    int nRowsGutter)
{
    if (pThis == nullptr) return 0;
    CBar* s = reinterpret_cast<CBar*>(pThis);
    if (s->m_nColorCount == 0) {
        s->m_nDefaultColumns = nColumns;   // 0x1420
        s->m_nColumnsOverride = nRows;     // 0x1424
        s->m_nRowsOverride = nRowsGutter;  // 0x1428
        // TODO(clean-room): retail also runs the palette helper (0x1800265f0)
        // before tail-calling CWnd::CreateEx (0x18014d850); neither is
        // modeled.
    }
    return 0;
}
// CreateControl(CWnd* pWndParent, const CRect& rect, UINT nID, int nColumns,
// CPalette* pPalette): clears the 0x10bc flag, derives the column count from
// the rect width / palette handle and creates the control.  The flag store is
// transcribed from retail RVA 0x263f0; the CreateEx tail (0x18014d850) is
// unmodeled, so 0 (failure) is returned.
// Symbol: ?CreateControl@CMFCColorBar@@UEAAHPEAVCWnd@@AEBVCRect@@IHPEAVCPalette@@@Z
extern "C" int MS_ABI impl__CreateControl_CMFCColorBar__UEAAHPEAVCWnd__AEBVCRect__IHPEAVCPalette___Z(
    void* pThis, void* /*pWndParent*/, const void* /*pRect*/,
    unsigned int /*nID*/, int /*nColumns*/, void* /*pPalette*/)
{
    if (pThis == nullptr) return 0;
    CBar* s = reinterpret_cast<CBar*>(pThis);
    s->m_nCaptionShow = 0;   // 0x10bc
    // TODO(clean-room): transcribed partially -- retail (RVA 0x263f0) derives
    // the grid columns from the rect width and the palette handle
    // (GetObjectW) and tail-calls the CWnd::CreateEx wrapper (0x18014d850);
    // not modeled, so 0 (failure) is returned.
    return 0;
}
// CreatePalette(const CArray<DWORD,DWORD>& arColors, CPalette& palette):
// builds a LOGPALETTE from the color array (or the default tables at
// 0x1803c1620/0x1803c18a8) and attaches it to the palette.  The retail body
// (RVA 0x27650) indexes the CArray with the CPtrArray-style layout
// (m_pData@+8, count@+0x10) which does NOT match the OpenMFC CArray layout,
// so no array access is performed (conservative failure).
// Symbol: ?CreatePalette@CMFCColorBar@@KAHAEBV?$CArray@KK@@AEAVCPalette@@@Z
extern "C" int MS_ABI impl__CreatePalette_CMFCColorBar__KAHAEBV__CArray_KK__AEAVCPalette___Z(
    const void* /*pArray*/, void* /*pPalette*/)
{
    // TODO(clean-room): transcribed partially -- retail (RVA 0x27650) reads
    // the color array using a non-OpenMFC layout (m_pData@+8, count@+0x10)
    // and calls CreatePalette/CPalette::Attach (0x1802c6240 / 0x1802a3ed0);
    // neither is modeled, so 0 (failure) is returned.
    return 0;
}
// AdjustLocations(): lays out the color buttons.  The retail body
// (RVA 0x24eb0, ~0x350 bytes) guards on m_hWnd (0x40), the 0x111c flag and
// the visibility check, then iterates the CList at 0x1190 running per-button
// rect math.  The list and the layout virtuals are unmodeled.
// Symbol: ?AdjustLocations@CMFCColorBar@@MEAAXXZ
extern "C" void MS_ABI impl__AdjustLocations_CMFCColorBar__MEAAXXZ(
    void* pThis)
{
    if (pThis == nullptr) return;
    // TODO(clean-room): transcribed partially -- retail (RVA 0x24eb0)
    // iterates the button CList at 0x1190 (elements carry flags at +0x28)
    // and adjusts the button rects; the list and the layout virtuals are not
    // modeled.
}
// CalcSize(int bVert): returns the bar size through the hidden CSize slot.
// Retail (RVA 0x25200) combines GetColorGridSize (RVA 0x25280) with the
// margins (0x1438/0x143c) and GetExtraHeight (RVA 0x25340); with the grid
// size unmodeled the safe CSize(0,0) terminal is returned.
// Symbol: ?CalcSize@CMFCColorBar@@MEAA?AVCSize@@H@Z
extern "C" CSize* MS_ABI impl__CalcSize_CMFCColorBar__MEAA_AVCSize__H_Z(
    void* pThis, CSize* pRet, int /*bVert*/)
{
    if (!pRet) return pRet;
    pRet->cx = 0;
    pRet->cy = 0;
    if (pThis == nullptr) return pRet;
    // TODO(clean-room): transcribed partially -- retail (RVA 0x25200)
    // computes  cx = grid.cx*0x14b8 + 2*0x1438  and
    //            cy = grid.cy*0x14bc + 2*0x143c + GetExtraHeight(...)
    // with grid.cx/cy from GetColorGridSize (not modeled); CSize(0,0) is the
    // conservative terminal.
    return pRet;
}
// ContextToSize(int nWidth, int nHeight): adjusts the row/column counts so
// the bar fits the given client size.  The retail body (RVA 0x25510) guards
// on m_hWnd, runs the vslot-0x558 button-size virtual and updates 0x14bc /
// 0x1434 / 0x14b8.  The virtual is unmodeled.
// Symbol: ?ContextToSize@CMFCColorBar@@QEAAXHH@Z
extern "C" void MS_ABI impl__ContextToSize_CMFCColorBar__QEAAXHH_Z(
    void* pThis, int /*nWidth*/, int /*nHeight*/)
{
    if (pThis == nullptr) return;
    // TODO(clean-room): transcribed partially -- retail (RVA 0x25510) derives
    // the row/column adjustment from the client rect and the vslot-0x558
    // button-size virtual, updating 0x14bc / 0x1434 / 0x14b8; not modeled.
}
// GetColorGridSize(int bVert): returns the color grid size through the hidden
// CSize slot.  Retail (RVA 0x25280) branches on the 0x1448 flag, the
// vslot-0x2f0 button-count virtual, the column/row overrides (0x1424/0x1428/
// 0x1420) and a sqrt path (0x1802c7510) over the color count (0x1468).  The
// virtual paths are unmodeled, so CSize(0,0) is returned.
// Symbol: ?GetColorGridSize@CMFCColorBar@@IEBA?AVCSize@@H@Z
extern "C" CSize* MS_ABI impl__GetColorGridSize_CMFCColorBar__IEBA_AVCSize__H_Z(
    void* pThis, CSize* pRet, int /*bVert*/)
{
    if (!pRet) return pRet;
    pRet->cx = 0;
    pRet->cy = 0;
    if (pThis == nullptr) return pRet;
    // TODO(clean-room): transcribed partially -- retail (RVA 0x25280) derives
    // the grid from the color count (0x1468), the 0x1448 branch flag, the
    // column/row overrides (0x1424/0x1428/0x1420) and the vslot-0x2f0
    // button-count virtual; the virtual is not modeled, so CSize(0,0) is the
    // conservative terminal.
    return pRet;
}
// GetExtraHeight(int nRows): extra height contributed by the automatic /
// other-button labels and the button grid.  Fully transcribed from retail
// RVA 0x25340: label-1 length at 0x14d8 adds 0x1434, label-2 length at 0x14e0
// adds 0x1438 + 0x1434, and when label-3 (0x14e8) is non-empty and the button
// count (0x1498) is non-zero ceil(count/nRows) rows of 0x14bc plus
// 4 + 0x1438 + 0x1434 are added.  The vslot-0x2f0 gate inside the third block
// is unmodeled and treated as enabled.
// Symbol: ?GetExtraHeight@CMFCColorBar@@IEBAHH@Z
extern "C" int MS_ABI impl__GetExtraHeight_CMFCColorBar__IEBAHH_Z(
    void* pThis, int nRows)
{
    if (pThis == nullptr) return 0;
    CBar* s = reinterpret_cast<CBar*>(pThis);
    int nResult = 0;
    if (CStrLen(s->m_strAutoLabel) > 0) {          // 0x14d8
        nResult = s->m_nRowHeight;                 // 0x1434
    } else if (CStrLen(s->m_strOtherLabel) > 0) {  // 0x14e0
        nResult = s->m_nMargin2;                   // 0x1438
    }
    if (CStrLen(s->m_strOtherLabel) > 0) {
        nResult += s->m_nRowHeight;                // 0x1434
    }
    if (CStrLen(s->m_strThirdLabel) > 0 && s->m_nButtonCount != 0) {  // 0x14e8 / 0x1498
        // TODO(clean-room): retail gates this block on the vslot-0x2f0
        // button-count virtual unless the 0x144c flag is set; the virtual is
        // not modeled and the block is transcribed as enabled.
        const int nDiv = (nRows > 0) ? nRows : 1;
        int nRow = s->m_nButtonCount / nDiv;
        if (s->m_nButtonCount % nDiv != 0) ++nRow;   // ceil(count/nRows)
        nResult += nRow * s->m_nGridRows + 4 + s->m_nMargin2 + s->m_nRowHeight;
    }
    return nResult;
}
// EnableAutomaticButton(LPCTSTR lpszLabel, COLORREF clr, BOOL bShowBorder):
// stores clr into 0x14d4 and the label into the auto-label CString (0x14d8,
// empty when bShowBorder is clear or the label is null).  Transcribed from
// retail RVA 0x26e80; the vslot-0x8c8 / vslot-0x428 dispatch tail is not
// modeled.
// Symbol: ?EnableAutomaticButton@CMFCColorBar@@QEAAXPEB_WKH@Z
extern "C" void MS_ABI impl__EnableAutomaticButton_CMFCColorBar__QEAAXPEB_WKH_Z(
    void* pThis, const wchar_t* lpszLabel, unsigned long clr, int bShowBorder)
{
    if (pThis == nullptr) return;
    CBar* s = reinterpret_cast<CBar*>(pThis);
    s->m_clrAuto = clr;   // 0x14d4
    AssignCStr_Cmfccolorbar(&s->m_strAutoLabel,
               (bShowBorder != 0) ? lpszLabel : L"");   // 0x14d8
    // TODO(clean-room): retail (RVA 0x26e80) then runs the vslot-0x8c8
    // virtual and tail-calls the vslot-0x428 base dispatch; not modeled.
}
// EnableOtherButton(LPCTSTR lpszLabel, BOOL bAuto, BOOL bShowBorder): stores
// (bAuto == 0) into 0x1440 and the label into the other-label CString
// (0x14e0, empty when bShowBorder is clear or the label is null).
// Transcribed from retail RVA 0x26f00; the vslot-0x8c8 / vslot-0x428 dispatch
// tail is not modeled.
// Symbol: ?EnableOtherButton@CMFCColorBar@@QEAAXPEB_WHH@Z
extern "C" void MS_ABI impl__EnableOtherButton_CMFCColorBar__QEAAXPEB_WHH_Z(
    void* pThis, const wchar_t* lpszLabel, int bAuto, int bShowBorder)
{
    if (pThis == nullptr) return;
    CBar* s = reinterpret_cast<CBar*>(pThis);
    s->m_bShowOtherButton = (bAuto == 0) ? 1 : 0;   // 0x1440
    AssignCStr_Cmfccolorbar(&s->m_strOtherLabel,
               (bShowBorder != 0) ? lpszLabel : L"");  // 0x14e0
    // TODO(clean-room): retail (RVA 0x26f00) then runs the vslot-0x8c8
    // virtual and tail-calls the vslot-0x428 base dispatch; not modeled.
}
// DoPaint(CDC* pDC): paints the color bar.  Retail (RVA 0x25e70) resolves the
// palette through the helper 0x180027770, runs the base DoPaint
// (0x18014f290) and then the RealizePalette path (0x1802c60e8 /
// 0x1802a3ea0); none of that is modeled.
// Symbol: ?DoPaint@CMFCColorBar@@MEAAXPEAVCDC@@@Z
extern "C" void MS_ABI impl__DoPaint_CMFCColorBar__MEAAXPEAVCDC___Z(
    void* pThis, void* /*pDC*/)
{
    if (pThis == nullptr) return;
    // TODO(clean-room): transcribed partially -- retail (RVA 0x25e70) runs
    // the palette helper (0x180027770), the base DoPaint (0x18014f290) and a
    // RealizePalette path (0x1802c60e8 / 0x1802a3ea0); not modeled.
}
// GetHighlightedColor(): returns the color of the highlighted button, or -1
// (0xffffffff).  Retail (RVA 0x278a0) gates on the highlighted index (0x1140),
// resolves the button through CMFCToolBar::GetButton (0x18014fe00),
// type-checks it as CMFCToolBarColorButton (IsKindOf at 0x180234cf0 against
// the class at 0x1803b10b8) and returns the button's m_clrColor at +0x88.
// The button lookup and the type check are unmodeled, so the not-found
// terminal (-1) is returned.
// Symbol: ?GetHighlightedColor@CMFCColorBar@@QEBAKXZ
extern "C" unsigned long MS_ABI impl__GetHighlightedColor_CMFCColorBar__QEBAKXZ(
    void* pThis)
{
    if (pThis == nullptr) return static_cast<unsigned long>(-1);
    CBar* s = reinterpret_cast<CBar*>(pThis);
    if (s->m_nHighlightedIndex < 0) {
        return static_cast<unsigned long>(-1);   // retail: `js` terminal
    }
    // TODO(clean-room): transcribed partially -- retail resolves the button by
    // index (0x18014fe00), type-checks it (0x180234cf0 / 0x1803b10b8) and
    // returns its +0x88 m_clrColor; the button lookup is not modeled, so the
    // not-found terminal (-1) is returned.
    return static_cast<unsigned long>(-1);
}
// InitColors(CPalette*, CArray<DWORD,DWORD>&): static helper that fills the
// color array from the palette.  Retail (RVA 0x265f0) reads the palette entry
// count (GetObjectW via 0x1802c6290), sizes the CArray (0x180015034) and
// fills it via GetPaletteEntries (0x1802c6248) using the retail CArray layout
// (m_pData@+0x8, count@+0x10).  The palette/array plumbing is not modeled, so
// 0 (the empty-array terminal) is returned.
// Symbol: ?InitColors@CMFCColorBar@@KAHPEAVCPalette@@AEAV?$CArray@KK@@@Z
extern "C" int MS_ABI impl__InitColors_CMFCColorBar__KAHPEAVCPalette__AEAV__CArray_KK___Z(
    void* /*pPalette*/, void* /*arColors*/)
{
    // TODO(clean-room): transcribed partially -- retail sizes the CArray to the
    // palette entry count and copies the palette entries in (GetPaletteEntries
    // at 0x1802c6248); the retail CArray layout (m_pData@+0x8, count@+0x10)
    // differs from OpenMFC's, so nothing is written.
    return 0;
}
// OnChangeHot(int nHot): forwards the hot-button change to the child menu
// bar.  Retail (RVA 0x274a0) gates on the 0x1500 object, resolves the hot
// button (0x18014fe00), type-checks it as CMFCToolBarColorButton
// (0x180234cf0 / 0x1803b10b8), inspects the +0x94/+0x98 flags (falling back
// to -1 when either is set) and dispatches the vslot-0x410 virtual on the
// 0x1500 object through 0x1802c7b30.  The gate is transcribed; the button
// lookup and the dispatch are not modeled.
// Symbol: ?OnChangeHot@CMFCColorBar@@MEAAXH@Z
extern "C" void MS_ABI impl__OnChangeHot_CMFCColorBar__MEAAXH_Z(
    void* pThis, int /*nHot*/)
{
    if (pThis == nullptr) return;
    CBar* s = reinterpret_cast<CBar*>(pThis);
    if (s->m_pMenuBar == nullptr) return;   // 0x1500
    // TODO(clean-room): transcribed partially -- retail resolves the hot
    // button by index (0x18014fe00), type-checks it (0x180234cf0 /
    // 0x1803b10b8), inspects the +0x94/+0x98 flags and dispatches the
    // vslot-0x410 virtual on the 0x1500 object (0x1802c7b30); none of that is
    // modeled.
}
// SetColor(COLORREF clr): stores the current color and drives the button
// states.  The early path (m_nAutoColor store, m_hWnd guard, m_nHotIndex
// reset) is transcribed from retail RVA 0x26f80; the button walk depends on
// unmodeled CMFCToolBarColorButton objects.
// Symbol: ?SetColor@CMFCColorBar@@QEAAXK@Z
extern "C" void MS_ABI impl__SetColor_CMFCColorBar__QEAAXK_Z(
    void* pThis, unsigned long clr)
{
    if (pThis == nullptr) return;
    CBar* s = reinterpret_cast<CBar*>(pThis);
    if (s->m_nAutoColor == static_cast<int>(clr)) return;   // 0x14d0
    s->m_nAutoColor = static_cast<int>(clr);                // 0x14d0
    if (s->m_hWnd == nullptr) return;                       // 0x40
    s->m_nHotIndex = -1;                                    // 0x1138
    // TODO(clean-room): transcribed partially -- retail (RVA 0x26f80) then
    // walks the button CList at 0x1190, type-checks each button as
    // CMFCToolBarColorButton (0x180234cf0 / 0x1803b10b8), toggles the +0x8c
    // pressed flag, updates the button color (+0x88) and invalidates the
    // button rects (+0x68), finally dispatching the vslot-0x7d8 virtual
    // (0x1802c7b30); the button objects and the virtual are not modeled.
}
// SetVertMargin(int nMargin): stores nMargin into the vertical-margin slot
// (0x1438) and tail-calls the vslot-0x428 base dispatch.  Transcribed from
// retail RVA 0x27860; the base dispatch (0x1802c7b30) is not modeled.
// Symbol: ?SetVertMargin@CMFCColorBar@@QEAAXH@Z
extern "C" void MS_ABI impl__SetVertMargin_CMFCColorBar__QEAAXH_Z(
    void* pThis, int nMargin)
{
    if (pThis == nullptr) return;
    CBar* s = reinterpret_cast<CBar*>(pThis);
    s->m_nMargin2 = nMargin;   // 0x1438
    // TODO(clean-room): retail tail-calls the vslot-0x428 base dispatch
    // (0x1802c7b30); not modeled.
}
// SetHorzMargin(int nMargin): stores nMargin into the horizontal-margin slot
// (0x143c) and tail-calls the vslot-0x428 base dispatch.  Transcribed from
// retail RVA 0x27880; the base dispatch (0x1802c7b30) is not modeled.
// Symbol: ?SetHorzMargin@CMFCColorBar@@QEAAXH@Z
extern "C" void MS_ABI impl__SetHorzMargin_CMFCColorBar__QEAAXH_Z(
    void* pThis, int nMargin)
{
    if (pThis == nullptr) return;
    CBar* s = reinterpret_cast<CBar*>(pThis);
    s->m_nMarginX = nMargin;   // 0x143c
    // TODO(clean-room): retail tail-calls the vslot-0x428 base dispatch
    // (0x1802c7b30); not modeled.
}
// SetDocumentColors(LPCTSTR lpszLabel, const CList<DWORD,DWORD>& lstColors,
// BOOL bResetToAutomatic): stores the label into the third-label CString
// (0x14e8, null defaults to the empty string in retail) and the
// reset-to-automatic flag into 0x144c, then dispatches the vslot-0x8c8 /
// vslot-0x428 virtuals.  Transcribed from retail RVA 0x25430; the document-
// colors list sync at 0x1480 (retail CList layout) and the virtuals are not
// modeled.
// Symbol: ?SetDocumentColors@CMFCColorBar@@QEAAXPEB_WAEAV?$CList@KK@@H@Z
extern "C" void MS_ABI impl__SetDocumentColors_CMFCColorBar__QEAAXPEB_WAEAV__CList_KK__H_Z(
    void* pThis, const wchar_t* lpszLabel, void* /*lstColors*/,
    int bResetToAutomatic)
{
    if (pThis == nullptr) return;
    CBar* s = reinterpret_cast<CBar*>(pThis);
    AssignCStr_Cmfccolorbar(&s->m_strThirdLabel, lpszLabel ? lpszLabel : L"");  // 0x14e8
    s->m_bResetAuto = bResetToAutomatic;                           // 0x144c
    // TODO(clean-room): transcribed partially -- retail also copies the
    // caller's CList<DWORD,DWORD> into the member list at 0x1480 (retail CList
    // layout: head@+0x8, count@+0x18) and dispatches the vslot-0x8c8 /
    // vslot-0x428 virtuals (0x1802c7b30); neither is modeled.
}
// OnCreate(CREATESTRUCTW*): the retail body (RVA 0x257c0) runs the base
// CMFCToolBar::OnCreate (0x1800bf180, failure -> -1), then derives the grid
// from a cached size (0x180157400) storing 0x14b8/0x14bc, clears 0x10d4,
// computes 0x1434 and dispatches the vslot-0x8c8 virtual before resolving a
// DC for the 0x14f0/0x14f8 buttons.  None of that is modeled, so the success
// terminal (0) is returned.
// Symbol: ?OnCreate@CMFCColorBar@@IEAAHPEAUtagCREATESTRUCTW@@@Z
extern "C" int MS_ABI impl__OnCreate_CMFCColorBar__IEAAHPEAUtagCREATESTRUCTW___Z(
    void* pThis, CREATESTRUCTW* /*lpCreateStruct*/)
{
    if (pThis == nullptr) return 0;
    // TODO(clean-room): transcribed partially -- retail runs the base
    // CMFCToolBar::OnCreate (0x1800bf180) and the grid/DC setup (0x180157400,
    // GetDC 0x1802c7250, DPI helper 0x18028ad70); none of that is modeled, so
    // the success terminal 0 is returned.
    return 0;
}
// OnDestroy(): destroys the automatic/other buttons and runs the base
// OnDestroy.  The automatic-button qword clear at +0xb98 is transcribed from
// retail RVA 0x27380; the button teardown (0x1802a9b60) and the base
// OnDestroy (0x1800bdd80) are not modeled.
// Symbol: ?OnDestroy@CMFCColorBar@@IEAAXXZ
extern "C" void MS_ABI impl__OnDestroy_CMFCColorBar__IEAAXXZ(
    void* pThis)
{
    if (pThis == nullptr) return;
    CBar* s = reinterpret_cast<CBar*>(pThis);
    if (s->m_pBtnAuto != nullptr) {                     // 0x14f0
        *reinterpret_cast<unsigned long long*>(
            reinterpret_cast<char*>(s->m_pBtnAuto) + 0xb98) = 0;
        // TODO(clean-room): retail destroys the automatic button
        // (0x1802a9b60) and runs the base OnDestroy (0x1800bdd80); not
        // modeled.
    } else if (s->m_pBtnOther != nullptr) {             // 0x14f8
        // TODO(clean-room): retail dispatches the vslot-0x2e0 virtual on the
        // other button (0x1802c7b30), destroys it (0x1802a9b60) and runs the
        // base OnDestroy (0x1800bdd80); not modeled.
    }
}
// OnKey(UINT nChar): forwards VK_RETURN on the hot button to the parent as
// WM_SETMESSAGESTRING and then runs the base OnKey.  The list walk
// (0x1138/0x11a0/0x1190), the parent resolve and the SendMessageW are
// transcribed from retail RVA 0x273e0; the base OnKey tail (0x1800bde70) is
// not modeled.
// Symbol: ?OnKey@CMFCColorBar@@MEAAHI@Z
extern "C" int MS_ABI impl__OnKey_CMFCColorBar__MEAAHI_Z(
    void* pThis, unsigned int nChar)
{
    if (pThis == nullptr) return 0;
    CBar* s = reinterpret_cast<CBar*>(pThis);
    void* pBtn = nullptr;
    const long long nIndex = s->m_nHotIndex;            // 0x1138
    if (nIndex >= 0 && nIndex < s->m_nListButtonCount) {  // 0x11a0
        void* pNode = s->m_pListButtons;                // 0x1190 (head)
        long long i = nIndex;
        while (pNode != nullptr && i > 0) {
            pNode = *reinterpret_cast<void**>(pNode);   // (*node) = next
            --i;
        }
        if (pNode != nullptr) {
            pBtn = *reinterpret_cast<void**>(
                reinterpret_cast<char*>(pNode) + 0x10); // node->data
        }
    }
    if (nChar == VK_RETURN && pBtn != nullptr) {
        HWND hwndParent = s->m_hWndParent;              // 0xa0
        if (hwndParent == nullptr) hwndParent = ::GetParent(s->m_hWnd);
        if (hwndParent != nullptr) {
            // TODO(clean-room): retail resolves hwndParent through the
            // CWnd::FromHandlePermanent helper (0x18028ad70) whose m_hWnd
            // equals hwndParent, then sends WM_SETMESSAGESTRING (0x362) with
            // wParam=0xe001; the helper is not modeled and the message is sent
            // to the parent hwnd directly (equivalent result).
            ::SendMessageW(hwndParent, 0x362, 0xe001, 0);
        }
    }
    // TODO(clean-room): retail tail-calls the base OnKey (0x1800bde70) and
    // returns its result; not modeled, so 0 is returned.
    return 0;
}
// OnLButtonDblClk(UINT, CPoint): the retail body (RVA 0x27250) gates on the
// module flag at 0x1803be35c and this->0x1450, then runs the base
// OnLButtonDblClk (0x1801560c0).  Neither is modeled, so the body is
// conservative.
// Symbol: ?OnLButtonDblClk@CMFCColorBar@@IEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnLButtonDblClk_CMFCColorBar__IEAAXIVCPoint___Z(
    void* pThis, unsigned int /*nFlags*/, CPoint /*point*/)
{
    if (pThis == nullptr) return;
    // TODO(clean-room): transcribed partially -- retail gates on the module
    // flag 0x1803be35c and this->0x1450 and then runs the base
    // OnLButtonDblClk (0x1801560c0); neither is modeled.
}
// OnLButtonDown(UINT, CPoint): the retail body (RVA 0x27180) gates on the
// module flag at 0x1803be35c and this->0x1450, then runs the vslot-0x730
// hit-test virtual (0x1802c7b30) and, when it reports -1, the base
// OnLButtonDown (0x180150410).  None of that is modeled, so the body is
// conservative.
// Symbol: ?OnLButtonDown@CMFCColorBar@@IEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnLButtonDown_CMFCColorBar__IEAAXIVCPoint___Z(
    void* pThis, unsigned int /*nFlags*/, CPoint /*point*/)
{
    if (pThis == nullptr) return;
    // TODO(clean-room): transcribed partially -- retail gates on the module
    // flag 0x1803be35c and this->0x1450, runs the vslot-0x730 hit-test virtual
    // and then the base OnLButtonDown (0x180150410); none of that is modeled.
}
// OnLButtonUp(UINT, CPoint): the retail body (RVA 0x271e0) gates on the
// module flag at 0x1803be35c and this->0x1450, runs the vslot-0x730 hit-test
// virtual, stores a non-negative result into 0x1134 and then runs the base
// OnLButtonUp (0x180151260).  None of that is modeled, so the body is
// conservative.
// Symbol: ?OnLButtonUp@CMFCColorBar@@IEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnLButtonUp_CMFCColorBar__IEAAXIVCPoint___Z(
    void* pThis, unsigned int /*nFlags*/, CPoint /*point*/)
{
    if (pThis == nullptr) return;
    // TODO(clean-room): transcribed partially -- retail gates on the module
    // flag 0x1803be35c and this->0x1450, runs the vslot-0x730 hit-test virtual
    // (storing a non-negative result into 0x1134) and then the base
    // OnLButtonUp (0x180151260); none of that is modeled.
}
// OnMouseLeave(): the retail body (RVA 0x27530) runs the base OnMouseLeave
// (0x1801567a0) when neither the automatic (0x14f0) nor the other (0x14f8)
// button exists; otherwise it clears the hot state (0x10e8/0x12d8/0x12dc/
// 0x1138), dispatches the vslot-0x770 virtual, sends WM_SETMESSAGESTRING and
// dispatches the vslot-0x410 virtual on the 0x1500 object.  None of that
// machinery is modeled, so the body is conservative.
// Symbol: ?OnMouseLeave@CMFCColorBar@@IEAAXXZ
extern "C" void MS_ABI impl__OnMouseLeave_CMFCColorBar__IEAAXXZ(
    void* pThis)
{
    if (pThis == nullptr) return;
    // TODO(clean-room): transcribed partially -- retail gates on the 0x14f0 /
    // 0x14f8 buttons, the module flags 0x1803be360/0x1803be35c and this->0x10ec,
    // then clears the hot state and dispatches the vslot-0x770 / vslot-0x410
    // virtuals (0x1802c7b30); none of that is modeled.
}
// OnMouseMove(UINT, CPoint): the retail body (RVA 0x27160) gates on the
// module flag at 0x1803be35c and this->0x1450, then runs the base OnMouseMove
// (0x180150ce0).  Neither is modeled, so the body is conservative.
// Symbol: ?OnMouseMove@CMFCColorBar@@IEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnMouseMove_CMFCColorBar__IEAAXIVCPoint___Z(
    void* pThis, unsigned int /*nFlags*/, CPoint /*point*/)
{
    if (pThis == nullptr) return;
    // TODO(clean-room): transcribed partially -- retail gates on the module
    // flag 0x1803be35c and this->0x1450 and then runs the base OnMouseMove
    // (0x180150ce0); neither is modeled.
}
// OnNcCalcSize(BOOL, NCCALCSIZE_PARAMS*): when the 0x1448 flag is set, the
// retail body (RVA 0x25770) delegates to the status-bar sub-object at
// &this->0x1120 through 0x180031910.  The gate is transcribed; the
// sub-object is not modeled.
// Symbol: ?OnNcCalcSize@CMFCColorBar@@IEAAXHPEAUtagNCCALCSIZE_PARAMS@@@Z
extern "C" void MS_ABI impl__OnNcCalcSize_CMFCColorBar__IEAAXHPEAUtagNCCALCSIZE_PARAMS___Z(
    void* pThis, int /*bCalcValidRects*/, NCCALCSIZE_PARAMS* /*pParams*/)
{
    if (pThis == nullptr) return;
    CBar* s = reinterpret_cast<CBar*>(pThis);
    if (s->m_bGridSizeFlag == 0) return;                // 0x1448
    // TODO(clean-room): transcribed partially -- retail delegates to the
    // sub-object at &this->0x1120 (0x180031910); not modeled.
}
// OnNcPaint(): when the 0x1448 flag is set, the retail body (RVA 0x257a0)
// delegates to the status-bar sub-object at &this->0x1120 through
// 0x180031620.  The gate is transcribed; the sub-object is not modeled.
// Symbol: ?OnNcPaint@CMFCColorBar@@IEAAXXZ
extern "C" void MS_ABI impl__OnNcPaint_CMFCColorBar__IEAAXXZ(
    void* pThis)
{
    if (pThis == nullptr) return;
    CBar* s = reinterpret_cast<CBar*>(pThis);
    if (s->m_bGridSizeFlag == 0) return;                // 0x1448
    // TODO(clean-room): transcribed partially -- retail delegates to the
    // sub-object at &this->0x1120 (0x180031620); not modeled.
}
// OnPaletteChanged(CWnd* pWndFocus): repaints the bar when the focus window
// differs from this.  The hwnd comparison and the InvalidateRect/UpdateWindow
// pair are transcribed from retail RVA 0x25f00; the palette helper
// (0x18028ac80) is not modeled.
// Symbol: ?OnPaletteChanged@CMFCColorBar@@IEAAXPEAVCWnd@@@Z
extern "C" void MS_ABI impl__OnPaletteChanged_CMFCColorBar__IEAAXPEAVCWnd___Z(
    void* pThis, void* pWndFocus)
{
    if (pThis == nullptr) return;
    CBar* s = reinterpret_cast<CBar*>(pThis);
    // TODO(clean-room): retail first runs the palette-aware helper
    // (0x18028ac80) on this; not modeled.
    HWND hwndFocus = (pWndFocus != nullptr)
        ? *reinterpret_cast<HWND*>(reinterpret_cast<char*>(pWndFocus) + 0x40)  // CWnd::m_hWnd
        : nullptr;
    if (hwndFocus == s->m_hWnd) return;
    if (s->m_hWnd != nullptr) {
        ::InvalidateRect(s->m_hWnd, nullptr, TRUE);
        ::UpdateWindow(s->m_hWnd);
    }
}
// OnQueryNewPalette(): invalidates and repaints the bar and then runs the
// palette helper.  The repaint pair is transcribed from retail RVA 0x25ed0;
// the tail helper (0x18028ac80) is not modeled.
// Symbol: ?OnQueryNewPalette@CMFCColorBar@@IEAAHXZ
extern "C" int MS_ABI impl__OnQueryNewPalette_CMFCColorBar__IEAAHXZ(
    void* pThis)
{
    if (pThis == nullptr) return 0;
    CBar* s = reinterpret_cast<CBar*>(pThis);
    if (s->m_hWnd != nullptr) {
        ::InvalidateRect(s->m_hWnd, nullptr, TRUE);
        ::UpdateWindow(s->m_hWnd);
    }
    // TODO(clean-room): retail tail-calls the palette helper (0x18028ac80) and
    // returns its result; not modeled, so 0 is returned.
    return 0;
}
// OnSendCommand(const CMFCToolBarButton*): complex command-commit path.  The
// retail body (RVA 0x25f60) resolves the parent popup frame (0x18028ad70 /
// +0x228, CMFCPopupMenu check at 0x1803b1448), dispatches the vslot-0x8c0
// color virtual and a global frame hook (0x1803be1b8), then invalidates and
// re-sizes.  The frame/button machinery is unmodeled, so 0 (the base-return
// terminal) is returned.
// Symbol: ?OnSendCommand@CMFCColorBar@@MEAAHPEBVCMFCToolBarButton@@@Z
extern "C" int MS_ABI impl__OnSendCommand_CMFCColorBar__MEAAHPEBVCMFCToolBarButton___Z(
    void* pThis, void* /*pButton*/)
{
    if (pThis == nullptr) return 0;
    // TODO(clean-room): transcribed partially -- retail checks the button flags
    // (+0x94/+0x98), dispatches the vslot-0x8c0 color virtual and the
    // vslot-0x428 base dispatch (0x1802c7b30) and invalidates; not modeled.
    return 0;
}
// OnUpdateCmdUI(CFrameWnd*, BOOL): complex CCmdUI update path.  The retail
// body (RVA 0x25d00) gates on the command id (0x1454), runs CCmdUI::DoUpdate
// through the target vtable, flips the bit-0x12 flag of every
// CMFCToolBarColorButton in the 0x1190 list and finally runs the base
// OnUpdateCmdUI (0x180151c80).  None of that is modeled, so the body is
// conservative.
// Symbol: ?OnUpdateCmdUI@CMFCColorBar@@MEAAXPEAVCFrameWnd@@H@Z
extern "C" void MS_ABI impl__OnUpdateCmdUI_CMFCColorBar__MEAAXPEAVCFrameWnd__H_Z(
    void* pThis, void* /*pTarget*/, int /*bDisableIfNoHandler*/)
{
    if (pThis == nullptr) return;
    // TODO(clean-room): transcribed partially -- retail gates on the command id
    // (0x1454), runs CCmdUI::DoUpdate through the target frame vtable
    // (0x1802c7b30), flips the bit-0x12 button flag and runs the base
    // OnUpdateCmdUI (0x180151c80); none of that is modeled.
}
// OpenColorDialog(COLORREF clrDefault, COLORREF& newColor): opens the
// CColorDialog (or the CMFCColorMenuButton custom-color path) and stores the
// picked color.  The retail body (RVA 0x26d20) builds a CColorDialog and
// dispatches the +0x210 virtual of the CMFCColorMenuButton; the dialog
// machinery is unmodeled, so 0 (the cancel terminal) is returned and the
// out-parameter is left untouched.
// Symbol: ?OpenColorDialog@CMFCColorBar@@MEAAHKAEAK@Z
extern "C" int MS_ABI impl__OpenColorDialog_CMFCColorBar__MEAAHKAEAK_Z(
    void* pThis, unsigned long /*clrDefault*/, unsigned long* /*pNewColor*/)
{
    if (pThis == nullptr) return 0;
    // TODO(clean-room): transcribed partially -- retail opens a CColorDialog
    // (0x180207670/0x180207760) or dispatches the +0x210 virtual of the
    // CMFCColorMenuButton (0x1803b1118 check) and stores the result; the
    // dialog machinery is not modeled, so the cancel terminal 0 is returned.
    return 0;
}
// PreTranslateMessage(MSG*): relays WM_CONTEXTMENU / WM_KEYDOWN /
// WM_SYSKEYDOWN / the three WM_*BUTTONDOWN messages to the parent window and
// otherwise runs the base PreTranslateMessage.  The retail body (RVA 0x27270)
// depends on the 0x14f0/0x14f8/0x1500 objects, the 0x1384 flag, SendMessageW
// and the base dispatch (0x180154940); none of that is modeled, so 0 (the
// not-translated terminal) is returned.
// Symbol: ?PreTranslateMessage@CMFCColorBar@@MEAAHPEAUtagMSG@@@Z
extern "C" int MS_ABI impl__PreTranslateMessage_CMFCColorBar__MEAAHPEAUtagMSG___Z(
    void* pThis, MSG* /*pMsg*/)
{
    if (pThis == nullptr) return 0;
    // TODO(clean-room): transcribed partially -- retail checks the 0x14f0 /
    // 0x14f8 / 0x1500 objects and the 0x1384 flag, then handles
    // WM_CONTEXTMENU (0x7b), WM_KEYDOWN (0x100, ESC), WM_SYSKEYDOWN (0x104)
    // and WM_LBUTTONDOWN/RBUTTONDOWN/MBUTTONDOWN via SendMessageW to the
    // parent; not modeled, so the not-translated terminal 0 is returned.
    return 0;
}
// Rebuild(): rebuilds the automatic/other buttons and the color grid.  The
// retail body (RVA 0x25880) guards on m_hWnd (0x40), runs the vslot-0x6b0
// base virtual, constructs the buttons (0x180023ac8) and adds them through
// the vslot-0x690 virtual.  The guard is transcribed; the rest is not
// modeled.
// Symbol: ?Rebuild@CMFCColorBar@@MEAAXXZ
extern "C" void MS_ABI impl__Rebuild_CMFCColorBar__MEAAXXZ(
    void* pThis)
{
    if (pThis == nullptr) return;
    CBar* s = reinterpret_cast<CBar*>(pThis);
    if (s->m_hWnd == nullptr) return;
    // TODO(clean-room): transcribed partially -- retail runs the vslot-0x6b0
    // base virtual, constructs a CMFCColorButton for the auto label (0x14d8,
    // 0x180023ac8) and one per color in the 0x1460 array, adding each through
    // the vslot-0x690 virtual (0x1802c7b30); none of that is modeled.
}
// SelectPalette(CDC*): resolves and selects the palette into the DC.  The
// retail body (RVA 0x27770) lazily builds the palette tables (0x18006a790 /
// 0x180027650) and calls CDC::SelectPalette (0x1802c60e8).  The palette
// plumbing is unmodeled, so nullptr (the failure terminal) is returned.
// Symbol: ?SelectPalette@CMFCColorBar@@IEAAPEAVCPalette@@PEAVCDC@@@Z
extern "C" void* MS_ABI impl__SelectPalette_CMFCColorBar__IEAAPEAVCPalette__PEAVCDC___Z(
    void* pThis, void* /*pDC*/)
{
    if (pThis == nullptr) return nullptr;
    // TODO(clean-room): transcribed partially -- retail lazily builds the
    // palette tables (0x18006a790 / 0x180027650) and selects the palette
    // through CDC::SelectPalette (0x1802c60e8); the palette plumbing is not
    // modeled, so nullptr is returned.
    return nullptr;
}
// Serialize(CArchive&): serializes the layout, colors and labels through the
// retail CArchive buffer pointers (ar+0x38/0x40) and the base Serialize
// (0x180153070).  The OpenMFC CArchive layout differs from retail, so no
// archive access is performed (conservative no-op).
// Symbol: ?Serialize@CMFCColorBar@@MEAAXAEAVCArchive@@@Z
extern "C" void MS_ABI impl__Serialize_CMFCColorBar__MEAAXAEAVCArchive___Z(
    void* pThis, void* /*pAr*/)
{
    if (pThis == nullptr) return;
    // TODO(clean-room): transcribed partially -- retail reads/writes the
    // 0x1420/0x1424/0x1428/0x14d4 ints and the 0x14d8/0x14e0/0x14e8 CStrings
    // through the CArchive buffer pointers (ar+0x38/0x40) and runs the base
    // Serialize (0x180153070); the OpenMFC CArchive layout differs from retail
    // and the base is unmodeled, so nothing is serialized.
}
// ShowCommandMessageString(UINT nCmdId): relays the command string id to the
// parent frame as WM_SETMESSAGESTRING (0x362).  Transcribed from retail RVA
// 0x26cc0: the parent is the cached 0xa0 hwnd (or GetParent(m_hWnd)) and the
// wParam is the 0x1454 command id (or the 0xe001 default when it is -1); the
// CWnd::FromHandlePermanent helper (0x18028ad70) is not modeled but its
// m_hWnd equals the parent hwnd, so the message is sent directly.
// Symbol: ?ShowCommandMessageString@CMFCColorBar@@MEAAXI@Z
extern "C" void MS_ABI impl__ShowCommandMessageString_CMFCColorBar__MEAAXI_Z(
    void* pThis, unsigned int /*nCmdId*/)
{
    if (pThis == nullptr) return;
    CBar* s = reinterpret_cast<CBar*>(pThis);
    HWND hwndParent = s->m_hWndParent;                  // 0xa0
    if (hwndParent == nullptr) hwndParent = ::GetParent(s->m_hWnd);
    if (hwndParent == nullptr) return;
    WPARAM wID = 0xe001;
    if (s->m_nCommandId != -1) {                        // 0x1454
        wID = static_cast<WPARAM>(static_cast<unsigned int>(s->m_nCommandId));
    }
    // TODO(clean-room): retail resolves hwndParent through CWnd::
    // FromHandlePermanent (0x18028ad70) before the SendMessageW; the helper
    // is not modeled and its m_hWnd equals hwndParent, so the message is sent
    // to the parent hwnd directly (equivalent result).
    ::SendMessageW(hwndParent, 0x362, wID, 0);
}
// Symbol: ??0CMFCColorBar@@IEAA@AEAV0@I@Z
extern "C" void* MS_ABI impl___0CMFCColorBar__IEAA_AEAV0_I_Z(void* pThis, void* pOther, int unusedFlags) {
    (void)pOther;
    (void)unusedFlags;
    return pThis;
}
// Symbol: ??0CMFCColorBar@@IEAA@AEBV?$CArray@KK@@KPEB_W11AEAV?$CList@KK@@HHHKIPEAVCMFCColorButton@@@Z
extern "C" void* MS_ABI impl___0CMFCColorBar__IEAA_AEBV__CArray_KK__KPEB_W11AEAV__CList_KK__HHHKIPEAVCMFCColorButton___Z(
    void* pThis, const void* array, unsigned int count, const wchar_t* name,
    void* list, int row, int col, int unknown, void* colorButton) {
    (void)array;
    (void)count;
    (void)name;
    (void)list;
    (void)row;
    (void)col;
    (void)unknown;
    (void)colorButton;
    return pThis;
}
// Symbol: ??0CMFCColorBar@@IEAA@AEBV?$CArray@KK@@KPEB_W11AEAV?$CList@KK@@HKIPEAVCMFCRibbonColorButton@@@Z
extern "C" void* MS_ABI impl___0CMFCColorBar__IEAA_AEBV__CArray_KK__KPEB_W11AEAV__CList_KK__HKIPEAVCMFCRibbonColorButton___Z(
    void* pThis, const void* array, unsigned int count, const wchar_t* name,
    void* list, int row, int col, void* colorButton) {
    (void)array;
    (void)count;
    (void)name;
    (void)list;
    (void)row;
    (void)col;
    (void)colorButton;
    return pThis;
}
// Symbol: ??0CMFCColorBar@@QEAA@XZ
extern "C" void* MS_ABI impl___0CMFCColorBar__QEAA_XZ(void* pThis) {
    return pThis;
}
// Symbol: ??1CMFCColorBar@@UEAA@XZ
extern "C" void MS_ABI impl___1CMFCColorBar__UEAA_XZ(void* pThis) {
    (void)pThis;
}
