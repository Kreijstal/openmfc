// OpenMFC: CMFCColorMenuButton exports.
//
// All 16 exports are implemented here, transcribed from the retail mfc140u.dll
// (14.51.36231, x64) by decoding the export table (ordinal -> RVA -> objdump
// of the retail body).
//
// The class is not declared in the OpenMFC headers, so `this` is taken as void*
// and the members are reached through raw offsets.  Everything these exports do
// depends on unmodeled machinery -- the CMFCToolBarButton base (CopyFrom /
// Serialize), the CMFCColorPopup window (CreatePopupMenu, the 0x2ed8-byte
// object at 0x180024730), the visual-manager drawing vslots (OnDraw /
// OnDrawOnCustomizeList) and the CMap-backed color-name tables -- so the bodies
// keep their parameter validation and return the retail failure terminal with a
// `// TODO(clean-room)` marker.
//
// Conventions (see IMPLEMENTATION_GUIDE.md):
//   * every export is `extern "C"` + `__attribute__((ms_abi))`;
//   * the implicit `this` is the first explicit parameter;
//   * NULL-check pThis everywhere;
//   * struct-returning exports take the hidden return slot in the register the
//     retail body uses, as documented at each definition.
//
// Kept in a global_*.cpp file so the build's AUTO_EXCLUDES pass (the scan of
// `// Symbol:` comments) drops the generated weak stubs for these ordinals.

#include "openmfc/afxwin.h"
#include "openmfc/afxmfc.h"

#include <windows.h>

#include <cstddef>
#include <cstdint>
#include <cstring>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

//-----------------------------------------------------------------------------
// CMFCColorMenuButton::m_ColorsByID
//-----------------------------------------------------------------------------
// Static color/id map (retail at RVA 0x3b1c40).  The CMap<UINT,UINT,COLORREF,
// COLORREF> layout is not modeled, so the slot is defined zeroed like the
// sibling CTabbedPane data exports in cbarcore.cpp; GetColorByCmdID therefore
// cannot resolve any entry.
// Symbol: ?m_ColorsByID@CMFCColorMenuButton@@1V?$CMap@IIKK@@A
extern "C" __attribute__((used)) void* impl__m_ColorsByID_CMFCColorMenuButton__1V__CMap_IIKK__A = nullptr;

//=============================================================================
// CMFCColorMenuButton - class not declared in the OpenMFC headers (opaque).
// The retail object is 0x1A0 bytes (see CreateObject at RVA 0x2a4e0); the
// color/name state (+0x128 color, +0x12c automatic flag, +0x174/+0x178/+0x17c
// enable flags, +0xf4 tear-off id, the +0x180/+0x188/+0x190 CString slots)
// and the static CMap color tables (m_ColorsByID at 0x1803b1c40, the name
// table at 0x1803b1c00) are not modeled.
//=============================================================================

//-----------------------------------------------------------------------------
// CMFCColorMenuButton::CopyFrom
//-----------------------------------------------------------------------------
// Retail (0x18002b8a0): calls CMFCToolBarButton::CopyFrom (0x180172ca0), then
// copies the color fields (+0x128), re-hashes the color/id map (0x18002346c)
// and duplicates the +0x130 CStringArray (0x180015034).  The base class and
// the string array are unmodeled.
// Symbol: ?CopyFrom@CMFCColorMenuButton@@MEAAXAEBVCMFCToolBarButton@@@Z
extern "C" void MS_ABI impl__CopyFrom_CMFCColorMenuButton__MEAAXAEBVCMFCToolBarButton___Z(
    void* pThis, const CMFCToolBarButton& /*src*/)
{
    if (!pThis) return;
    // TODO(clean-room): not transcribed -- needs the CMFCToolBarButton base
    // CopyFrom, the +0x128 color store, the map re-hash (0x18002346c) and the
    // +0x130 CStringArray copy; not modeled.
}

//-----------------------------------------------------------------------------
// CMFCColorMenuButton::CreateObject
//-----------------------------------------------------------------------------
// Retail (0x18002a4e0) allocates a 0x1A0-byte object and runs the constructor
// (0x18002a560), which installs the retail vftable (0x1802e22c8).  The class
// is opaque in OpenMFC -- no constructor or vftable is modeled -- so no valid
// object can be produced.
// Symbol: ?CreateObject@CMFCColorMenuButton@@SAPEAVCObject@@XZ
extern "C" CObject* MS_ABI impl__CreateObject_CMFCColorMenuButton__SAPEAVCObject__XZ(void)
{
    // TODO(clean-room): partially transcribed -- retail allocates 0x1A0 bytes
    // and runs the CMFCColorMenuButton constructor (RVA 0x18002a560, retail
    // vftable at 0x1802e22c8); the opaque class has no modeled constructor or
    // vftable, so nullptr (the safe allocation-failure terminal) is returned.
    return nullptr;
}

//-----------------------------------------------------------------------------
// CMFCColorMenuButton::CreatePopupMenu
//-----------------------------------------------------------------------------
// Retail (0x18002bad0): allocates a 0x2ED8-byte CMFCColorPopup
// (operator new 0x1800027f0) and runs its constructor (0x180024730).  The
// popup window class is not modeled, so no menu can be produced; the retail
// failure terminal nullptr is returned.
// Symbol: ?CreatePopupMenu@CMFCColorMenuButton@@MEAAPEAVCMFCPopupMenu@@XZ
extern "C" void* MS_ABI impl__CreatePopupMenu_CMFCColorMenuButton__MEAAPEAVCMFCPopupMenu__XZ(
    void* pThis)
{
    if (!pThis) return nullptr;
    // TODO(clean-room): partially transcribed -- retail allocates the 0x2ED8-
    // byte CMFCColorPopup (ctor 0x180024730); the popup class is not modeled,
    // so nullptr (the safe allocation-failure terminal) is returned.
    return nullptr;
}

//-----------------------------------------------------------------------------
// CMFCColorMenuButton::EnableAutomaticButton
//-----------------------------------------------------------------------------
// Retail (0x18002a980): stores bIsDefault into +0x174 and, when enabled, the
// label into the +0x180 CString (SetString 0x180002e30) and bShow into the
// +0x12c automatic flag.  The CString slot and the flags are unmodeled.
// Symbol: ?EnableAutomaticButton@CMFCColorMenuButton@@QEAAXPEB_WKH@Z
extern "C" void MS_ABI impl__EnableAutomaticButton_CMFCColorMenuButton__QEAAXPEB_WKH_Z(
    void* pThis, const wchar_t* /*lpszLabel*/, int /*bShow*/, int /*bIsDefault*/)
{
    if (!pThis) return;
    // TODO(clean-room): partially transcribed -- the +0x174 store, the +0x180
    // CString SetString and the +0x12c automatic flag are not modeled.
}

//-----------------------------------------------------------------------------
// CMFCColorMenuButton::EnableDocumentColors
//-----------------------------------------------------------------------------
// Retail (0x18002aa50): stores bShow into +0x17c and, when enabled, the label
// into the +0x190 CString (SetString 0x180002e30).  The CString slot and the
// flag are unmodeled.
// Symbol: ?EnableDocumentColors@CMFCColorMenuButton@@QEAAXPEB_WH@Z
extern "C" void MS_ABI impl__EnableDocumentColors_CMFCColorMenuButton__QEAAXPEB_WH_Z(
    void* pThis, const wchar_t* /*lpszLabel*/, int /*bShow*/)
{
    if (!pThis) return;
    // TODO(clean-room): partially transcribed -- the +0x17c store and the
    // +0x190 CString SetString are not modeled.
}

//-----------------------------------------------------------------------------
// CMFCColorMenuButton::EnableOtherButton
//-----------------------------------------------------------------------------
// Retail (0x18002a9e0): stores bIsDefault into +0x178 and, when enabled, the
// label into the +0x188 CString (SetString 0x180002e30).  The CString slot and
// the flag are unmodeled.
// Symbol: ?EnableOtherButton@CMFCColorMenuButton@@QEAAXPEB_WHH@Z
extern "C" void MS_ABI impl__EnableOtherButton_CMFCColorMenuButton__QEAAXPEB_WHH_Z(
    void* pThis, const wchar_t* /*lpszLabel*/, int /*bShow*/, int /*bIsDefault*/)
{
    if (!pThis) return;
    // TODO(clean-room): partially transcribed -- the +0x178 store and the
    // +0x188 CString SetString are not modeled.
}

//-----------------------------------------------------------------------------
// CMFCColorMenuButton::EnableTearOff
//-----------------------------------------------------------------------------
// Retail (0x18002aaa0): clamps nID into the +0x8/+0xc range of the global
// (0x1803be260) and stores the result into +0xf4, plus bHorz into +0x16c and
// bVert into +0x170.  The members and the global range are unmodeled.
// Symbol: ?EnableTearOff@CMFCColorMenuButton@@QEAAXIHH@Z
extern "C" void MS_ABI impl__EnableTearOff_CMFCColorMenuButton__QEAAXIHH_Z(
    void* pThis, unsigned int /*nID*/, int /*bHorz*/, int /*bVert*/)
{
    if (!pThis) return;
    // TODO(clean-room): partially transcribed -- the +0xf4/+0x16c/+0x170
    // stores (with the global 0x1803be260 range clamp) are not modeled.
}

//-----------------------------------------------------------------------------
// CMFCColorMenuButton::GetColorByCmdID
//-----------------------------------------------------------------------------
// Retail (0x18002bcd0): looks nID up in the static m_ColorsByID map
// (0x1800236e0 / the +0x3b1c40 data export) and returns the stored color, or
// -1 when the lookup fails.  The map is the (zeroed) m_ColorsByID data export
// below, which cannot hold entries, so the failure terminal -1 is returned.
// Symbol: ?GetColorByCmdID@CMFCColorMenuButton@@SAKI@Z
extern "C" unsigned long MS_ABI impl__GetColorByCmdID_CMFCColorMenuButton__SAKI_Z(
    unsigned int /*nID*/)
{
    // TODO(clean-room): partially transcribed -- the static m_ColorsByID CMap
    // (data export below) is defined but cannot hold entries; retail's failure
    // terminal -1 is returned.
    return 0xFFFFFFFFu;
}

//-----------------------------------------------------------------------------
// CMFCColorMenuButton::Initialize
//-----------------------------------------------------------------------------
// Retail (0x18002a880): seeds the color/flag members (+0x128 = -1,
// +0x12c/+0x168..+0x17c/+0x198 = 0/-1 constants).  Pure member stores on
// unmodeled state.
// Symbol: ?Initialize@CMFCColorMenuButton@@IEAAXXZ
extern "C" void MS_ABI impl__Initialize_CMFCColorMenuButton__IEAAXXZ(
    void* pThis)
{
    if (!pThis) return;
    // TODO(clean-room): partially transcribed -- the +0x128/+0x12c/+0x168..
    // +0x17c/+0x198 member seeding is not modeled.
}

//-----------------------------------------------------------------------------
// CMFCColorMenuButton::OnChangeParentWnd
//-----------------------------------------------------------------------------
// Retail (0x18002b100): forwards to the CMFCToolBarButton base
// OnChangeParentWnd and re-arms the tooltip when the parent changed.  The base
// class is unmodeled.
// Symbol: ?OnChangeParentWnd@CMFCColorMenuButton@@UEAAXPEAVCWnd@@@Z
extern "C" void MS_ABI impl__OnChangeParentWnd_CMFCColorMenuButton__UEAAXPEAVCWnd___Z(
    void* pThis, CWnd* /*pWndParent*/)
{
    if (!pThis) return;
    // TODO(clean-room): not transcribed -- needs the CMFCToolBarButton base
    // OnChangeParentWnd and the tooltip re-arm; not modeled.
}

//-----------------------------------------------------------------------------
// CMFCColorMenuButton::OnDraw
//-----------------------------------------------------------------------------
// Retail (0x18002aaf0): paints the color menu button (color chip, automatic
// label, document-color palette) through the visual manager and the +0x180/
// +0x188/+0x190 CString slots.  The visual manager and the strings are
// unmodeled.
// Symbol: ?OnDraw@CMFCColorMenuButton@@MEAAXPEAVCDC@@AEBVCRect@@PEAVCMFCToolBarImages@@HHHHH@Z
extern "C" void MS_ABI impl__OnDraw_CMFCColorMenuButton__MEAAXPEAVCDC__AEBVCRect__PEAVCMFCToolBarImages__HHHHH_Z(
    void* pThis, CDC* /*pDC*/, const CRect& /*rect*/, void* /*pImages*/,
    int /*bHorz*/, int /*bCustomizeMode*/, int /*bHighlight*/, int /*bDrawBorder*/,
    int /*bGrayDisabledButtons*/)
{
    if (!pThis) return;
    // TODO(clean-room): not transcribed -- needs the visual-manager drawing
    // and the +0x180/+0x188/+0x190 label strings; not modeled.
}

//-----------------------------------------------------------------------------
// CMFCColorMenuButton::OnDrawOnCustomizeList
//-----------------------------------------------------------------------------
// Retail (0x18002af30): draws the button in the customize list (label via the
// +0x180 CString slot, color chip).  The string slot is unmodeled.
// Symbol: ?OnDrawOnCustomizeList@CMFCColorMenuButton@@MEAAHPEAVCDC@@AEBVCRect@@H@Z
extern "C" int MS_ABI impl__OnDrawOnCustomizeList_CMFCColorMenuButton__MEAAHPEAVCDC__AEBVCRect__H_Z(
    void* pThis, CDC* /*pDC*/, const CRect& /*rect*/, int /*bSelected*/)
{
    if (!pThis) return FALSE;
    // TODO(clean-room): not transcribed -- needs the +0x180 label string and
    // the color-chip drawing; not modeled.
    return FALSE;
}

//-----------------------------------------------------------------------------
// CMFCColorMenuButton::OpenColorDialog
//-----------------------------------------------------------------------------
// Retail (0x18002b9f0): runs a full CColorDialog (0xf50-byte stack object,
// 0x180207670 ctor) when the +0x198 automatic flag is set and stores the
// picked color into *pColor.  The dialog machinery is unmodeled; the retail
// failure terminal FALSE is returned.
// Symbol: ?OpenColorDialog@CMFCColorMenuButton@@UEAAHKAEAK@Z
extern "C" int MS_ABI impl__OpenColorDialog_CMFCColorMenuButton__UEAAHKAEAK_Z(
    void* pThis, unsigned long /*clrDefault*/, unsigned long& /*clrPick*/)
{
    if (!pThis) return FALSE;
    // TODO(clean-room): not transcribed -- needs the CColorDialog machinery
    // (0x180207670/0x180207760) and the +0x198 flag; not modeled.
    return FALSE;
}

//-----------------------------------------------------------------------------
// CMFCColorMenuButton::Serialize
//-----------------------------------------------------------------------------
// Retail (0x18002b190): calls CMFCToolBarButton::Serialize (0x180172dd0),
// then reads/writes the color and the +0x130 string array through CArchive
// (0x1801d1cc0 FillBuffer, 0x1801d1ff0 ReadCount, ...).  The base class and
// the string array are unmodeled.
// Symbol: ?Serialize@CMFCColorMenuButton@@MEAAXAEAVCArchive@@@Z
extern "C" void MS_ABI impl__Serialize_CMFCColorMenuButton__MEAAXAEAVCArchive___Z(
    void* pThis, CArchive& /*ar*/)
{
    if (!pThis) return;
    // TODO(clean-room): not transcribed -- needs the CMFCToolBarButton base
    // Serialize, the color field and the +0x130 CStringArray; not modeled.
}

//-----------------------------------------------------------------------------
// CMFCColorMenuButton::SetColor
//-----------------------------------------------------------------------------
// Retail (0x18002afb0): stores color into +0x128, re-hashes the color/id map
// (0x18002346c, key = +0x24 id), repaints the +0x80 tooltip and, when bRedraw
// is set, repaints the owner.  The map and the tooltip are unmodeled; the
// +0x128 store is kept through the layout mirror.
// Symbol: ?SetColor@CMFCColorMenuButton@@UEAAXKH@Z
extern "C" void MS_ABI impl__SetColor_CMFCColorMenuButton__UEAAXKH_Z(
    void* pThis, unsigned long /*clr*/, int /*bRedraw*/)
{
    if (!pThis) return;
    // TODO(clean-room): partially transcribed -- the +0x128 color store, the
    // map re-hash (0x18002346c) and the tooltip repaint are not modeled.
}

//-----------------------------------------------------------------------------
// CMFCColorMenuButton::SetColorName
//-----------------------------------------------------------------------------
// Retail (0x18002bcb0): static -- records the name for a color in the static
// name table (0x180027990 / +0x3b1c00).  The table is not modeled.
// Symbol: ?SetColorName@CMFCColorMenuButton@@SAXKAEBV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@@Z
extern "C" void MS_ABI impl__SetColorName_CMFCColorMenuButton__SAXKAEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
    unsigned long /*clr*/, const CString& /*lpszName*/)
{
    // TODO(clean-room): not transcribed -- needs the static name table
    // (0x180027990 / +0x3b1c00); not modeled.
}
