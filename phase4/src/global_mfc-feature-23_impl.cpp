// CMFCRibbonBaseElement / CMFCRibbonPanel / CMFCRibbonSeparator exports
// implemented for the wave2_mfc-feature-23 batch (30 symbols: 12 base element
// + 13 panel + 5 separator).
//
// Each body was decoded from the retail mfc140u export (ordinal -> RVA ->
// objdump).  The retail layouts of all three classes are far larger than the
// clean-room declarations in include/openmfc/afxmfc.h model:
//
//   CMFCRibbonBaseElement retail members touched by this batch: the tooltip /
//   key CStrings (+0xa8/+0xb0), the parent pointers (+0xd8/+0xe0/+0xe8/+0xf8),
//   the rect (+0xc8) and its edges (+0xcc/+0xd0/+0xd4), m_nID (+0x118), the
//   text-always-on-right flag (+0x124), the stretch flags (+0x12c/+0x130), the
//   update-tooltip flag (+0x174) and the accessibility-mode flag (+0x17c).
//   OpenMFC declares only m_nID plus m_strText and opaque padding, so
//   everything except the ID is unreachable.  The small ID-based queries were
//   already implemented in global_mfc-feature-22_impl.cpp; the remaining
//   bodies here are type-correct conservative defaults with TODO notes.
//
//   CMFCRibbonSeparator retail members used here are the base element plus a
//   single 4-byte flag at +0x180 (drawn-size mode).  OpenMFC's separator is a
//   32-byte pad on top of the base, so the flag is unreachable.  The one fully
//   transcribed body is GetRegularSize (always CSize(4,4), no state needed);
//   CopyFrom delegates to the base CopyFrom; the draw/list functions are
//   conservative no-ops.
//
//   CMFCRibbonPanel retail members used here are the name CString (+0x140),
//   the layout members (+0xa0..+0x118, +0x140..+0x158, +0x288, +0x328), the
//   pressed element (+0x3e8), the launch button (+0x3f0), the buttons-group
//   (+0x170) and the two CArrays (+0x6a8/+0x6d8).  OpenMFC declares only
//   m_strName (protected, unreachable from a free function) plus m_hIcon and
//   opaque padding; panel membership lives in the ribbon_state side table (see
//   cbarcore.cpp / mfccore.cpp), so the two ID/state queries that can use it
//   (FindByID, GetDroppedDown) iterate the side table and everything else is
//   a conservative default with a TODO note.
//
// Kept in a global_*.cpp file so the build's AUTO_EXCLUDES pass (scan of
// `// Symbol:` comments) drops the generated stubs for these ordinals.

#include "openmfc/afxwin.h"
#include "openmfc/afxmfc.h"
#include "ribbon_state.h"

#include <cstdint>
#include <mutex>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

// Classes referenced by the exported signatures but not declared in the
// clean-room headers.  Only pointers are used, so a forward declaration is
// sufficient.
class CMFCRibbonPanelMenuBar;
class CMFCRibbonCommandsListBox;

// Accessibility return codes from wtypes.h / winerror.h; keep the numeric
// spellings so the file compiles even when <oleauto.h> is not pulled in.
#ifndef S_OK
  #define S_OK 0L
#endif
#ifndef S_FALSE
  #define S_FALSE 1L
#endif
#ifndef E_INVALIDARG
  #define E_INVALIDARG 0x80070057L
#endif

namespace {
using openmfc::ribbon_state::RibbonPanelStates;
using openmfc::ribbon_state::RibbonMutex;
} // namespace

// Defined in global_mfc-feature-22_impl.cpp.
extern "C" CMFCRibbonBaseElement* MS_ABI impl__FindByID_CMFCRibbonBaseElement__UEAAPEAV1_I_Z(
    CMFCRibbonBaseElement* pThis, unsigned int nID);
extern "C" void MS_ABI impl__CopyFrom_CMFCRibbonBaseElement__UEAAXAEBV1__Z(
    CMFCRibbonBaseElement* pThis, const CMFCRibbonBaseElement& src);

//=============================================================================
// CMFCRibbonBaseElement
//=============================================================================
// Only m_nID (GetID/SetID) is modeled, so none of the member writes below can
// be transcribed; the bodies are NULL-checked, type-correct conservative
// defaults (no-op / safe return) matching what the sibling
// global_mfc-feature-22_impl.cpp wave established for this class.

// CMFCRibbonBaseElement::SetKeys(const wchar_t*, const wchar_t*) -- retail
// measures both strings (wcslen) and stores them into the key CStrings at
// +0xa8/+0xb0.  Neither string member is modeled.
// TODO(clean-room): transcribed partially -- the key-tip CStrings are not
// modeled.
// Symbol: ?SetKeys@CMFCRibbonBaseElement@@UEAAXPEB_W0@Z
extern "C" void MS_ABI impl__SetKeys_CMFCRibbonBaseElement__UEAAXPEB_W0_Z(
    CMFCRibbonBaseElement* pThis, const wchar_t* /*lpszKeys*/, const wchar_t* /*lpszKeysExt*/)
{
    if (!pThis) return;
}

// CMFCRibbonBaseElement::SetOriginal(CMFCRibbonBaseElement*) -- retail walks
// the m_pOriginal chain (+0xe8) to the last non-null link and stores it into
// +0xe8.  The original-link member is not modeled.
// TODO(clean-room): transcribed partially -- m_pOriginal is not modeled.
// Symbol: ?SetOriginal@CMFCRibbonBaseElement@@UEAAXPEAV1@@Z
extern "C" void MS_ABI impl__SetOriginal_CMFCRibbonBaseElement__UEAAXPEAV1__Z(
    CMFCRibbonBaseElement* pThis, CMFCRibbonBaseElement* /*pOriginal*/)
{
    if (!pThis) return;
}

// CMFCRibbonBaseElement::SetParentCategory(CMFCRibbonCategory*) -- retail
// stores the category pointer at +0xe0.  The parent-category member is not
// modeled.
// TODO(clean-room): transcribed partially -- the parent category is not
// modeled.
// Symbol: ?SetParentCategory@CMFCRibbonBaseElement@@UEAAXPEAVCMFCRibbonCategory@@@Z
extern "C" void MS_ABI impl__SetParentCategory_CMFCRibbonBaseElement__UEAAXPEAVCMFCRibbonCategory___Z(
    CMFCRibbonBaseElement* pThis, CMFCRibbonCategory* /*pCategory*/)
{
    if (!pThis) return;
}

// CMFCRibbonBaseElement::SetParentMenu(CMFCRibbonPanelMenuBar*) -- retail
// stores the panel-menu-bar pointer at +0xf8.  The parent-menu member is not
// modeled.
// TODO(clean-room): transcribed partially -- the parent panel menu bar is not
// modeled.
// Symbol: ?SetParentMenu@CMFCRibbonBaseElement@@UEAAXPEAVCMFCRibbonPanelMenuBar@@@Z
extern "C" void MS_ABI impl__SetParentMenu_CMFCRibbonBaseElement__UEAAXPEAVCMFCRibbonPanelMenuBar___Z(
    CMFCRibbonBaseElement* pThis, CMFCRibbonPanelMenuBar* /*pParentMenu*/)
{
    if (!pThis) return;
}

// CMFCRibbonBaseElement::SetTextAlwaysOnRight(int) -- retail stores the
// argument into the +0x124 flag.  The flag is not modeled.
// TODO(clean-room): transcribed partially -- the +0x124 text-alignment flag
// is not modeled.
// Symbol: ?SetTextAlwaysOnRight@CMFCRibbonBaseElement@@UEAAXH@Z
extern "C" void MS_ABI impl__SetTextAlwaysOnRight_CMFCRibbonBaseElement__UEAAXH_Z(
    CMFCRibbonBaseElement* pThis, int /*bSet*/)
{
    if (!pThis) return;
}

// CMFCRibbonBaseElement::StretchToWholeRow(CDC*, int) -- retail calls the
// virtual IsStretchToWholeRow (vtable+0x320), rejects when the flags at
// +0x12c/+0x130 are set, and on success adds the argument to the rect edge at
// +0xcc and stores the result into +0xd4, returning TRUE.  None of that state
// or the virtual slot is modeled.
// TODO(clean-room): transcribed partially -- element geometry and stretch
// state are not modeled.
// Symbol: ?StretchToWholeRow@CMFCRibbonBaseElement@@UEAAHPEAVCDC@@H@Z
extern "C" int MS_ABI impl__StretchToWholeRow_CMFCRibbonBaseElement__UEAAHPEAVCDC__H_Z(
    CMFCRibbonBaseElement* pThis, CDC* /*pDC*/, int /*nHighlighted*/)
{
    if (!pThis) return FALSE;
    return FALSE;
}

// CMFCRibbonBaseElement::UpdateTooltipInfo() -- retail, when the flag at
// +0x174 is set and the ID at +0x118 passes the command range guard, looks
// the tooltip up in the global tooltip-manager and rewrites the description
// CStrings at +0xb8/+0xc0.  The flag, the manager and the strings are not
// modeled.
// TODO(clean-room): transcribed partially -- tooltip state is not modeled.
// Symbol: ?UpdateTooltipInfo@CMFCRibbonBaseElement@@UEAAXXZ
extern "C" void MS_ABI impl__UpdateTooltipInfo_CMFCRibbonBaseElement__UEAAXXZ(
    CMFCRibbonBaseElement* pThis)
{
    if (!pThis) return;
}

//=============================================================================
// CMFCRibbonBaseElement -- IAccessible methods
//=============================================================================
// The MSAA entry points below mostly gate on the accessibility-mode flag at
// +0x17c and on the parent bar/menu state (+0xd8/+0xf0), none of which is
// modeled.  The NULL/error-code contracts are transcribed faithfully; the
// flag-off results (S_FALSE / untouched output variants) are what retail
// returns for a freshly constructed element, which is the only state OpenMFC
// can produce.

// CMFCRibbonBaseElement::accDoDefaultAction(VARIANT) -- retail invokes the
// virtual at vtable+0x2c8 (result discarded) and always returns S_OK.  The
// virtual is not modeled; S_OK is the retail return value on every path.
// Symbol: ?accDoDefaultAction@CMFCRibbonBaseElement@@UEAAJUtagVARIANT@@@Z
extern "C" long MS_ABI impl__accDoDefaultAction_CMFCRibbonBaseElement__UEAAJUtagVARIANT___Z(
    CMFCRibbonBaseElement* pThis, VARIANT /*varChild*/)
{
    if (!pThis) return S_OK;
    return S_OK;
}

// CMFCRibbonBaseElement::accHitTest(long, long, VARIANT*) -- retail rejects a
// NULL output with E_INVALIDARG, then hit-tests through the parent bar at
// +0xd8 and its accessibility object (+0x40); without a parent it leaves the
// output VARIANT untouched and returns S_OK.  The parent state is not
// modeled, so the no-parent path is taken.
// TODO(clean-room): transcribed partially -- the parent-bar accessibility
// object is not modeled.
// Symbol: ?accHitTest@CMFCRibbonBaseElement@@UEAAJJJPEAUtagVARIANT@@@Z
extern "C" long MS_ABI impl__accHitTest_CMFCRibbonBaseElement__UEAAJJJPEAUtagVARIANT___Z(
    CMFCRibbonBaseElement* pThis, long /*x*/, long /*y*/, VARIANT* pVarOut)
{
    if (!pThis) return S_OK;
    if (!pVarOut) return E_INVALIDARG;
    return S_OK;
}

// CMFCRibbonBaseElement::accLocation(long*, long*, long*, long*, VARIANT) --
// retail rejects a NULL output pointer with E_INVALIDARG and returns S_FALSE
// for any child that is not CHILDID_SELF.  For CHILDID_SELF it computes the
// element rect (+0xc8) through the parent accessibility object; the rect and
// the parent are not modeled, so the outputs are zeroed and S_OK returned.
// TODO(clean-room): transcribed partially -- the element rect and the parent
// accessibility object are not modeled.
// Symbol: ?accLocation@CMFCRibbonBaseElement@@UEAAJPEAJ000UtagVARIANT@@@Z
extern "C" long MS_ABI impl__accLocation_CMFCRibbonBaseElement__UEAAJPEAJ000UtagVARIANT___Z(
    CMFCRibbonBaseElement* pThis, long* pxLeft, long* pyTop, long* pcxWidth,
    long* pcyHeight, VARIANT varChild)
{
    if (!pThis) return E_INVALIDARG;
    if (!pxLeft || !pyTop || !pcxWidth || !pcyHeight) return E_INVALIDARG;
    if (varChild.vt != 3 /*VT_I4*/) return S_FALSE;
    if (varChild.lVal != 0 /*CHILDID_SELF*/) return S_FALSE;
    *pxLeft = 0;
    *pyTop = 0;
    *pcxWidth = 0;
    *pcyHeight = 0;
    return S_OK;
}

// CMFCRibbonBaseElement::accNavigate(long, VARIANT, VARIANT*) -- retail
// returns 1 (S_FALSE) when the accessibility-mode flag at +0x17c is clear,
// which is the only state OpenMFC can produce; otherwise it walks the parent
// panel-menu-bar navigation (+0xf0).  The flag and the navigation state are
// not modeled.
// TODO(clean-room): transcribed partially -- accessibility navigation state
// is not modeled.
// Symbol: ?accNavigate@CMFCRibbonBaseElement@@UEAAJJUtagVARIANT@@PEAU2@@Z
extern "C" long MS_ABI impl__accNavigate_CMFCRibbonBaseElement__UEAAJJUtagVARIANT__PEAU2__Z(
    CMFCRibbonBaseElement* pThis, long /*navDir*/, VARIANT /*varStart*/, VARIANT* pvarEnd)
{
    if (!pThis) return S_FALSE;
    if (!pvarEnd) return E_INVALIDARG;
    return S_FALSE;
}

// CMFCRibbonBaseElement::get_accParent(IDispatch**) -- retail rejects a NULL
// output with E_INVALIDARG, clears the output, and returns 1 (S_FALSE) when
// the accessibility-mode flag at +0x17c is clear (the only state OpenMFC can
// produce).  When set it resolves the parent bar's (+0xd8) accessibility
// object; the flag and the parent are not modeled.
// TODO(clean-room): transcribed partially -- the parent-bar accessibility
// object is not modeled.
// Symbol: ?get_accParent@CMFCRibbonBaseElement@@UEAAJPEAPEAUIDispatch@@@Z
extern "C" long MS_ABI impl__get_accParent_CMFCRibbonBaseElement__UEAAJPEAPEAUIDispatch___Z(
    CMFCRibbonBaseElement* pThis, IDispatch** ppdispParent)
{
    if (!pThis) return E_INVALIDARG;
    if (!ppdispParent) return E_INVALIDARG;
    *ppdispParent = nullptr;
    return S_FALSE;
}

//=============================================================================
// CMFCRibbonSeparator
//=============================================================================
// The separator's only retail member beyond the base element is a 4-byte flag
// at +0x180 (drawn-size mode), which the clean-room declaration does not
// model.  GetRegularSize needs no state and is transcribed faithfully;
// everything else is conservative.

// CMFCRibbonSeparator::AddToListBox(CMFCRibbonCommandsListBox*, int) --
// retail loads a "separator" description string, inserts it into the list box
// (through the box's +0x40 toolbar reference) and returns TRUE; the box class
// is not declared and its state is not modeled.
// TODO(clean-room): transcribed partially -- commands list-box state is not
// modeled.
// Symbol: ?AddToListBox@CMFCRibbonSeparator@@UEAAHPEAVCMFCRibbonCommandsListBox@@H@Z
extern "C" int MS_ABI impl__AddToListBox_CMFCRibbonSeparator__UEAAHPEAVCMFCRibbonCommandsListBox__H_Z(
    CMFCRibbonSeparator* pThis, CMFCRibbonCommandsListBox* /*pListBox*/, int /*nIndex*/)
{
    if (!pThis) return FALSE;
    return FALSE;
}

// CMFCRibbonSeparator::CopyFrom(const CMFCRibbonBaseElement&) -- retail
// delegates to CMFCRibbonBaseElement::CopyFrom (0x180010350) and then copies
// the +0x180 flag.  The base copy is implemented over the modeled ID (see
// global_mfc-feature-22_impl.cpp) and is forwarded here; the flag is not
// modeled.
// TODO(clean-room): transcribed partially -- the +0x180 separator flag is not
// modeled.
// Symbol: ?CopyFrom@CMFCRibbonSeparator@@MEAAXAEBVCMFCRibbonBaseElement@@@Z
extern "C" void MS_ABI impl__CopyFrom_CMFCRibbonSeparator__MEAAXAEBVCMFCRibbonBaseElement___Z(
    CMFCRibbonSeparator* pThis, const CMFCRibbonBaseElement& src)
{
    if (!pThis) return;
    impl__CopyFrom_CMFCRibbonBaseElement__UEAAXAEBV1__Z(pThis, src);
}

// CMFCRibbonSeparator::GetRegularSize(CDC*) -- retail writes CSize(4, 4) into
// the hidden return slot and returns it, ignoring the element and the DC.
// Transcribed faithfully (4px x 4px is the retail separator size).
// Symbol: ?GetRegularSize@CMFCRibbonSeparator@@MEAA?AVCSize@@PEAVCDC@@@Z
extern "C" CSize* MS_ABI impl__GetRegularSize_CMFCRibbonSeparator__MEAA_AVCSize__PEAVCDC___Z(
    CSize* pRet, CMFCRibbonSeparator* /*pThis*/, CDC* /*pDC*/)
{
    if (!pRet) return nullptr;
    pRet->cx = 4;
    pRet->cy = 4;
    return pRet;
}

// CMFCRibbonSeparator::OnDraw(CDC*) -- retail (RVA 0xf780) computes the
// separator line rect from the element rect (+0xc8) and the +0x180 flag and
// renders it through the visual-manager virtuals.  The element geometry and
// the visual-manager slots are not modeled, so nothing is drawn.
// TODO(clean-room): transcribed partially -- separator drawing is not modeled.
// Symbol: ?OnDraw@CMFCRibbonSeparator@@MEAAXPEAVCDC@@@Z
extern "C" void MS_ABI impl__OnDraw_CMFCRibbonSeparator__MEAAXPEAVCDC___Z(
    CMFCRibbonSeparator* pThis, CDC* /*pDC*/)
{
    if (!pThis) return;
}

// CMFCRibbonSeparator::OnDrawOnList(CDC*, CString, int, CRect, int, int) --
// retail (RVA 0xfa80) stores the list index into the +0x148 member, adjusts
// the DC text color and renders the separator through the parent bar's
// text-drawing virtual.  None of that state is modeled.
// TODO(clean-room): transcribed partially -- separator list drawing is not
// modeled.
// Symbol: ?OnDrawOnList@CMFCRibbonSeparator@@MEAAXPEAVCDC@@V?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@HVCRect@@HH@Z
extern "C" void MS_ABI impl__OnDrawOnList_CMFCRibbonSeparator__MEAAXPEAVCDC__V__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__HVCRect__HH_Z(
    CMFCRibbonSeparator* pThis, CDC* /*pDC*/, CString /*strText*/, int /*nTextOffset*/,
    CRect /*rect*/, int /*bIsHighlighted*/, int /*bIsDisabled*/)
{
    if (!pThis) return;
}

//=============================================================================
// CMFCRibbonPanel
//=============================================================================
// Panel membership lives in the ribbon_state side table (see CMFCRibbonPanel
// ctor/Add/AddSeparator in mfccore.cpp / cbarcore.cpp), so the queries that
// are pure element searches (FindByID) are implemented over it.  Everything
// else touches retail members (name CString at +0x140, layout members, the
// launch button at +0x3f0, the buttons-group at +0x170, the element CArray at
// +0x6d8/+0x6e0) that the clean-room declaration does not model, so those
// bodies are type-correct conservative defaults with TODO notes.

// CMFCRibbonPanel::AddToolBar(UINT, UINT, UINT, UINT) -- retail (RVA
// 0x116810, ~0x290 bytes) loads the toolbar resource, builds a
// CMFCRibbonButtonsGroup (small/large image lists + element array) and
// appends it to the buttons-groups array.  CMFCRibbonButtonsGroup is only
// forward-declared and the panel's group storage is not modeled.
// TODO(clean-room): transcribed partially -- toolbar-group construction is
// not modeled.
// Symbol: ?AddToolBar@CMFCRibbonPanel@@QEAAPEAVCMFCRibbonButtonsGroup@@IIII@Z
extern "C" CMFCRibbonButtonsGroup* MS_ABI impl__AddToolBar_CMFCRibbonPanel__QEAAPEAVCMFCRibbonButtonsGroup__IIII_Z(
    CMFCRibbonPanel* pThis, unsigned int /*uiToolbarResID*/, unsigned int /*uiSmallImageResID*/,
    unsigned int /*uiLargeImageResID*/, unsigned int /*nImageCount*/)
{
    if (!pThis) return nullptr;
    return nullptr;
}

// CMFCRibbonPanel::CalcTotalWidth() -- retail scans every non-empty element
// rect (+0xc8/+0xd0) for the maximum right edge and folds it with the panel
// margins (+0xf0/+0x10c) into the total width.  Element geometry and the
// panel layout members are not modeled.
// TODO(clean-room): transcribed partially -- element/panel layout geometry is
// not modeled.
// Symbol: ?CalcTotalWidth@CMFCRibbonPanel@@IEAAHXZ
extern "C" int MS_ABI impl__CalcTotalWidth_CMFCRibbonPanel__IEAAHXZ(
    CMFCRibbonPanel* pThis)
{
    if (!pThis) return 0;
    return 0;
}

// CMFCRibbonPanel::CancelMode() -- retail resets the pressed state (+0x3e8,
// +0xe0), calls the pressed element's CancelMode virtuals (vtable+0x450/
// +0x458), forwards to the parent bar's CancelMode (vtable+0x220) and clears
// the captured flag at +0xa8.  None of that state is modeled.
// TODO(clean-room): transcribed partially -- pressed/capture state is not
// modeled.
// Symbol: ?CancelMode@CMFCRibbonPanel@@IEAAXXZ
extern "C" void MS_ABI impl__CancelMode_CMFCRibbonPanel__IEAAXXZ(
    CMFCRibbonPanel* pThis)
{
    if (!pThis) return;
}

// CMFCRibbonPanel::CenterElementsInColumn(int, int, int) -- retail centers
// the element rects (+0xc8) of [nFirst..nLast] around a computed center using
// the panel margins (+0xd4/+0xf4/+0x118) and the element's bottom edge.  The
// element geometry is not modeled.
// TODO(clean-room): transcribed partially -- element/panel layout geometry is
// not modeled.
// Symbol: ?CenterElementsInColumn@CMFCRibbonPanel@@IEAAXHHH@Z
extern "C" void MS_ABI impl__CenterElementsInColumn_CMFCRibbonPanel__IEAAXHHH_Z(
    CMFCRibbonPanel* pThis, int /*nFirst*/, int /*nLast*/, int /*nCenter*/)
{
    if (!pThis) return;
}

// CMFCRibbonPanel::CleanUpSizes() -- retail calls the virtual CleanUpSizes
// (vtable+0x318) on every element and on the launch button (+0x3f0).  Element
// size state is not modeled.
// TODO(clean-room): transcribed partially -- element size state is not
// modeled.
// Symbol: ?CleanUpSizes@CMFCRibbonPanel@@IEAAXXZ
extern "C" void MS_ABI impl__CleanUpSizes_CMFCRibbonPanel__IEAAXXZ(
    CMFCRibbonPanel* pThis)
{
    if (!pThis) return;
}

// CMFCRibbonPanel::CommonInit(const wchar_t*, HICON) -- retail (RVA 0x116260)
// stores the name CString (+0x140), constructs the default-button array
// (+0x3f0) and the element CArray (+0x6d8).  The clean-room CMFCRibbonPanel
// ctor already initializes the object and m_strName is not reachable from a
// free function (it is protected), so the body keeps the type-correct no-op.
// TODO(clean-room): transcribed partially -- panel init state beyond the
// declared members is not modeled.
// Symbol: ?CommonInit@CMFCRibbonPanel@@IEAAXPEB_WPEAUHICON__@@@Z
extern "C" void MS_ABI impl__CommonInit_CMFCRibbonPanel__IEAAXPEB_WPEAUHICON_____Z(
    CMFCRibbonPanel* pThis, const wchar_t* /*lpszName*/, HICON /*hIcon*/)
{
    if (!pThis) return;
}

// CMFCRibbonPanel::CopyFrom(CMFCRibbonPanel&) -- retail copies ~10 scalar
// members, the CString at +0x140, the groups array (+0x6a8), the elements
// array (+0x6d8, cloning each element through CopyFrom) and the launch
// button.  Only m_strName (protected) plus padding are declared and the side
// table has no clone support, so nothing can be copied safely.
// TODO(clean-room): transcribed partially -- panel member/array cloning is
// not modeled.
// Symbol: ?CopyFrom@CMFCRibbonPanel@@MEAAXAEAV1@@Z
extern "C" void MS_ABI impl__CopyFrom_CMFCRibbonPanel__MEAAXAEAV1__Z(
    CMFCRibbonPanel* pThis, CMFCRibbonPanel& /*src*/)
{
    if (!pThis) return;
}

// CMFCRibbonPanel::DoPaint(CDC*) -- retail (RVA 0x116ca0) paints the panel
// background, caption, launch button and every element through the visual
// manager.  None of the panel drawing state is modeled.
// TODO(clean-room): transcribed partially -- panel drawing is not modeled.
// Symbol: ?DoPaint@CMFCRibbonPanel@@MEAAXPEAVCDC@@@Z
extern "C" void MS_ABI impl__DoPaint_CMFCRibbonPanel__MEAAXPEAVCDC___Z(
    CMFCRibbonPanel* pThis, CDC* /*pDC*/)
{
    if (!pThis) return;
}

// CMFCRibbonPanel::EnableLaunchButton(UINT, int, const wchar_t*) -- retail
// stores the command ID at +0x328 and forwards the text/visibility to the
// launch button through the buttons-group at +0x170 (vtable+0x170/+0x180).
// Neither the launch-button state nor the group is modeled.
// TODO(clean-room): transcribed partially -- launch-button state is not
// modeled.
// Symbol: ?EnableLaunchButton@CMFCRibbonPanel@@QEAAXIHPEB_W@Z
extern "C" void MS_ABI impl__EnableLaunchButton_CMFCRibbonPanel__QEAAXIHPEB_W_Z(
    CMFCRibbonPanel* pThis, unsigned int /*uiCmdID*/, int /*bEnable*/, const wchar_t* /*lpszText*/)
{
    if (!pThis) return;
}

// CMFCRibbonPanel::FindByData(unsigned __int64) const -- retail scans the
// elements calling their virtual FindByData, then tests the launch button
// (+0x3f0).  OpenMFC's CMFCRibbonBaseElement has no data member/accessor, so
// no element can ever match (same caveat as the CMFCRibbonBaseElement::FindByData
// implementation in global_mfc-feature-22_impl.cpp).
// TODO(clean-room): transcribed partially -- element data members are not
// modeled.
// Symbol: ?FindByData@CMFCRibbonPanel@@QEBAPEAVCMFCRibbonBaseElement@@_K@Z
extern "C" CMFCRibbonBaseElement* MS_ABI impl__FindByData_CMFCRibbonPanel__QEBAPEAVCMFCRibbonBaseElement___K_Z(
    const CMFCRibbonPanel* pThis, unsigned long long /*dwData*/)
{
    if (!pThis) return nullptr;
    return nullptr;
}

// CMFCRibbonPanel::FindByID(unsigned int) const -- retail scans the elements
// calling their virtual FindByID (returning the first hit), then tests the
// buttons-group at +0x170 and the launch button at +0x3f0.  Implemented over
// the ribbon_state side-table element list with the modeled base FindByID;
// the +0x170 group and the +0x3f0 launch button are not modeled.
// TODO(clean-room): partially transcribed -- the panel's buttons group and
// launch button are not modeled.
// Symbol: ?FindByID@CMFCRibbonPanel@@QEBAPEAVCMFCRibbonBaseElement@@I@Z
extern "C" CMFCRibbonBaseElement* MS_ABI impl__FindByID_CMFCRibbonPanel__QEBAPEAVCMFCRibbonBaseElement__I_Z(
    const CMFCRibbonPanel* pThis, unsigned int nID)
{
    if (!pThis) return nullptr;

    std::lock_guard<std::mutex> lock(RibbonMutex());
    auto it = RibbonPanelStates().find(pThis);
    if (it == RibbonPanelStates().end()) return nullptr;

    for (CMFCRibbonBaseElement* pElem : it->second.elements) {
        if (!pElem) continue;
        if (impl__FindByID_CMFCRibbonBaseElement__UEAAPEAV1_I_Z(pElem, nID) != nullptr) {
            return pElem;
        }
    }
    return nullptr;
}

// CMFCRibbonPanel::GetCaptionSize(CDC*) const -- retail measures the name
// CString (+0x140) with GetTextExtentPoint32 and folds in the caption height
// (+0xf4); the +0xc0 flag makes it return CSize(0,0).  m_strName is protected
// and the layout members are not modeled, so the empty-caption size is the
// only truthful result.
// TODO(clean-room): transcribed partially -- the caption layout state is not
// modeled.
// Symbol: ?GetCaptionSize@CMFCRibbonPanel@@IEBA?AVCSize@@PEAVCDC@@@Z
extern "C" CSize* MS_ABI impl__GetCaptionSize_CMFCRibbonPanel__IEBA_AVCSize__PEAVCDC___Z(
    CSize* pRet, const CMFCRibbonPanel* /*pThis*/, CDC* /*pDC*/)
{
    if (!pRet) return nullptr;
    pRet->cx = 0;
    pRet->cy = 0;
    return pRet;
}

// CMFCRibbonPanel::GetDroppedDown() const -- retail asks the launch button
// (+0x3f0) and then each element for its dropped-down element (vtable+0x398).
// Drop-down state is not modeled (the base GetDroppedDown always reports
// none), so nullptr is the truthful result -- same treatment as
// CMFCRibbonCategory::GetDroppedDown in global_mfc-feature-17_impl.cpp.
// TODO(clean-room): transcribed partially -- element drop-down state is not
// modeled.
// Symbol: ?GetDroppedDown@CMFCRibbonPanel@@QEBAPEAVCMFCRibbonBaseElement@@XZ
extern "C" CMFCRibbonBaseElement* MS_ABI impl__GetDroppedDown_CMFCRibbonPanel__QEBAPEAVCMFCRibbonBaseElement__XZ(
    const CMFCRibbonPanel* pThis)
{
    if (!pThis) return nullptr;
    return nullptr;
}
