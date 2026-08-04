// CMFCBaseAccessibleObject — OpenMFC implementation.
// Sources: global_cmfcbaseaccessibleobject.cpp, manual_small_stub_implementations.cpp

#include "detail/ManualSmallStubImplementationsSupport.h"

// OpenMFC: CMFCBaseAccessibleObject exports.
//
// All 23 exports are implemented here, transcribed from the retail mfc140u.dll
// (14.51.36231, x64) by decoding the export table (ordinal -> RVA -> objdump
// of the retail body).
//
// The class is not declared in the OpenMFC headers (opaque), so `this` is taken
// as void*.  The trivial IAccessible methods (the S_FALSE / E_NOTIMPL stubs and
// the two `return 0` methods) are transcribed faithfully.  The get_acc* family
// validates its parameters exactly like retail and then needs the unmodeled
// state -- the parent window at +0x30, the per-control CString slots
// +0x40..+0x68, the role +0x70 / state +0x74 members and the vslot-0x158 /
// vslot-0x160 / vslot-0x168 virtuals -- so those bodies terminate on the retail
// *failure* terminal for the same argument values, with `// TODO(clean-room)`
// markers.  No member offsets are invented.
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

#include "openmfc/afx.h"
#include "openmfc/afxwin.h"
#include "openmfc/afxmfc.h"

#include <windows.h>
#include <oleacc.h>

#include <cstdint>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

//=============================================================================
// CMFCBaseAccessibleObject - class not declared in the OpenMFC headers
// (opaque). pThis is taken as void*. Retail member offsets the get_acc* bodies
// would read: parent window +0x30, the per-control CString slots +0x40/+0x48/
// +0x50/+0x58/+0x60/+0x68, role +0x70, state +0x74 and the vslot-0x158/
// vslot-0x160/vslot-0x168 virtuals -- all unmodeled.
//=============================================================================

//-----------------------------------------------------------------------------
// CMFCBaseAccessibleObject::OnSetAccData
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// CMFCBaseAccessibleObject::SetACCData
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// CMFCBaseAccessibleObject::accDoDefaultAction
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// CMFCBaseAccessibleObject::accHitTest
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// CMFCBaseAccessibleObject::accLocation
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// CMFCBaseAccessibleObject::accNavigate
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// CMFCBaseAccessibleObject::accSelect
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// CMFCBaseAccessibleObject::get_accChild
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// CMFCBaseAccessibleObject::get_accChildCount
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// CMFCBaseAccessibleObject::get_accDefaultAction
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// CMFCBaseAccessibleObject::get_accDescription
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// CMFCBaseAccessibleObject::get_accFocus
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// CMFCBaseAccessibleObject::get_accHelp
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// CMFCBaseAccessibleObject::get_accHelpTopic
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// CMFCBaseAccessibleObject::get_accKeyboardShortcut
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// CMFCBaseAccessibleObject::get_accName
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// CMFCBaseAccessibleObject::get_accParent
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// CMFCBaseAccessibleObject::get_accRole
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// CMFCBaseAccessibleObject::get_accSelection
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// CMFCBaseAccessibleObject::get_accState
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// CMFCBaseAccessibleObject::get_accValue
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// CMFCBaseAccessibleObject::put_accName
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// CMFCBaseAccessibleObject::put_accValue
//-----------------------------------------------------------------------------
// Retail (0x1800071e0) is `xor %eax,%eax; ret` -- always returns 0.
// Symbol: ?OnSetAccData@CMFCBaseAccessibleObject@@UEAAHJ@Z
extern "C" int MS_ABI impl__OnSetAccData_CMFCBaseAccessibleObject__UEAAHJ_Z(
    void* pThis, long /*lVal*/)
{
    if (!pThis) return FALSE;
    return FALSE;
}
// Retail (0x1800071e0) is `xor %eax,%eax; ret` -- always returns 0.
// Symbol: ?SetACCData@CMFCBaseAccessibleObject@@UEAAHPEAVCWnd@@AEAVCAccessibilityData@@@Z
extern "C" int MS_ABI impl__SetACCData_CMFCBaseAccessibleObject__UEAAHPEAVCWnd__AEAVCAccessibilityData___Z(
    void* pThis, CWnd* /*pParentWnd*/, CAccessibilityData& /*accData*/)
{
    if (!pThis) return FALSE;
    return FALSE;
}
// Retail (0x180003a60): `mov $0x1,%eax; ret` -- always S_FALSE.
// Symbol: ?accDoDefaultAction@CMFCBaseAccessibleObject@@UEAAJUtagVARIANT@@@Z
extern "C" long MS_ABI impl__accDoDefaultAction_CMFCBaseAccessibleObject__UEAAJUtagVARIANT___Z(
    void* pThis, VARIANT /*varID*/)
{
    if (!pThis) return S_FALSE;
    return S_FALSE;
}
// Retail (0x180003a60): `mov $0x1,%eax; ret` -- always S_FALSE.
// Symbol: ?accHitTest@CMFCBaseAccessibleObject@@UEAAJJJPEAUtagVARIANT@@@Z
extern "C" long MS_ABI impl__accHitTest_CMFCBaseAccessibleObject__UEAAJJJPEAUtagVARIANT___Z(
    void* pThis, long /*xLeft*/, long /*yTop*/, VARIANT* /*pvarID*/)
{
    if (!pThis) return S_FALSE;
    return S_FALSE;
}
// Retail (0x180003a60): `mov $0x1,%eax; ret` -- always S_FALSE; the four
// output pointers are left untouched.
// Symbol: ?accLocation@CMFCBaseAccessibleObject@@UEAAJPEAJ000UtagVARIANT@@@Z
extern "C" long MS_ABI impl__accLocation_CMFCBaseAccessibleObject__UEAAJPEAJ000UtagVARIANT___Z(
    void* pThis, long* /*pxLeft*/, long* /*pyTop*/, long* /*pcxWidth*/,
    long* /*pcyHeight*/, VARIANT /*varID*/)
{
    if (!pThis) return S_FALSE;
    return S_FALSE;
}
// Retail (0x180003a60): `mov $0x1,%eax; ret` -- always S_FALSE.
// Symbol: ?accNavigate@CMFCBaseAccessibleObject@@UEAAJJUtagVARIANT@@PEAU2@@Z
extern "C" long MS_ABI impl__accNavigate_CMFCBaseAccessibleObject__UEAAJJUtagVARIANT__PEAU2__Z(
    void* pThis, long /*navDir*/, VARIANT /*varStart*/, VARIANT* /*pvarEnd*/)
{
    if (!pThis) return S_FALSE;
    return S_FALSE;
}
// Retail (0x180003a60): `mov $0x1,%eax; ret` -- always S_FALSE.
// Symbol: ?accSelect@CMFCBaseAccessibleObject@@UEAAJJUtagVARIANT@@@Z
extern "C" long MS_ABI impl__accSelect_CMFCBaseAccessibleObject__UEAAJJUtagVARIANT___Z(
    void* pThis, long /*flagsSelect*/, VARIANT /*varID*/)
{
    if (!pThis) return S_FALSE;
    return S_FALSE;
}
// Retail (0x18000f0e0): NULL-checks ppdispChild (E_INVALIDARG), zeroes
// *ppdispChild, then returns E_INVALIDARG when varChildID.vt != VT_I4 (3) and
// S_FALSE otherwise.
// Symbol: ?get_accChild@CMFCBaseAccessibleObject@@UEAAJUtagVARIANT@@PEAPEAUIDispatch@@@Z
extern "C" long MS_ABI impl__get_accChild_CMFCBaseAccessibleObject__UEAAJUtagVARIANT__PEAPEAUIDispatch___Z(
    void* pThis, VARIANT varChildID, IDispatch** ppdispChild)
{
    if (!pThis) return E_INVALIDARG;
    if (!ppdispChild) return E_INVALIDARG;
    *ppdispChild = nullptr;
    return (varChildID.vt != VT_I4) ? E_INVALIDARG : S_FALSE;
}
// Retail (0x18000cf50): NULL-checks pcountChildren (E_INVALIDARG), writes 0 to
// *pcountChildren and returns S_OK.
// Symbol: ?get_accChildCount@CMFCBaseAccessibleObject@@UEAAJPEAJ@Z
extern "C" long MS_ABI impl__get_accChildCount_CMFCBaseAccessibleObject__UEAAJPEAJ_Z(
    void* pThis, long* pcountChildren)
{
    if (!pThis) return E_INVALIDARG;
    if (!pcountChildren) return E_INVALIDARG;
    *pcountChildren = 0;
    return S_OK;
}
// Retail (0x18000f5a0): with varID.vt != VT_I4 the action is E_INVALIDARG only
// when lVal != 0 (lVal == 0 falls into the generic lookup path, which ends in
// S_FALSE when the unmodeled child state does not resolve); every lookup path
// touches the +0x68 CString slot and the vslot-0x158/0x160/0x168 virtuals, so
// the conservative terminal is S_FALSE.
// Symbol: ?get_accDefaultAction@CMFCBaseAccessibleObject@@UEAAJUtagVARIANT@@PEAPEA_W@Z
extern "C" long MS_ABI impl__get_accDefaultAction_CMFCBaseAccessibleObject__UEAAJUtagVARIANT__PEAPEA_W_Z(
    void* pThis, VARIANT varChildID, wchar_t** /*pszDefaultAction*/)
{
    if (!pThis) return E_INVALIDARG;
    if (varChildID.vt != VT_I4 && varChildID.lVal != 0) return E_INVALIDARG;
    // TODO(clean-room): partially transcribed -- the string comes from the
    // unmodeled +0x68 slot behind the vslot-0x158/0x160/0x168 resolution.
    return S_FALSE;
}
// Retail (0x18000f270): every terminal reachable without the unmodeled +0x50
// slot / vslot-0x158/0x160/0x168 resolution is S_FALSE (including the
// vt != VT_I4 case); the success path writes a BSTR built from the +0x50 slot.
// Symbol: ?get_accDescription@CMFCBaseAccessibleObject@@UEAAJUtagVARIANT@@PEAPEA_W@Z
extern "C" long MS_ABI impl__get_accDescription_CMFCBaseAccessibleObject__UEAAJUtagVARIANT__PEAPEA_W_Z(
    void* pThis, VARIANT /*varChildID*/, wchar_t** /*pszDescription*/)
{
    if (!pThis) return S_FALSE;
    // TODO(clean-room): partially transcribed -- the description string lives
    // in the unmodeled +0x50 slot; without it retail's failure terminal S_FALSE
    // is returned.
    return S_FALSE;
}
// Retail (0x18000f590): returns E_INVALIDARG when pvarID is NULL and E_NOTIMPL
// otherwise.
// Symbol: ?get_accFocus@CMFCBaseAccessibleObject@@UEAAJPEAUtagVARIANT@@@Z
extern "C" long MS_ABI impl__get_accFocus_CMFCBaseAccessibleObject__UEAAJPEAUtagVARIANT___Z(
    void* pThis, VARIANT* pvarID)
{
    if (!pThis) return E_INVALIDARG;
    if (!pvarID) return E_INVALIDARG;
    return E_NOTIMPL;
}
// Retail (0x18000f460): vt != VT_I4 -> E_INVALIDARG; lVal == 0 -> S_FALSE;
// pszHelp == NULL -> E_INVALIDARG; lVal < 0 -> E_INVALIDARG; the help lookup
// (vslot 0x160 + the unmodeled +0x60 slot) terminates in E_INVALIDARG when the
// resolution fails.
// Symbol: ?get_accHelp@CMFCBaseAccessibleObject@@UEAAJUtagVARIANT@@PEAPEA_W@Z
extern "C" long MS_ABI impl__get_accHelp_CMFCBaseAccessibleObject__UEAAJUtagVARIANT__PEAPEA_W_Z(
    void* pThis, VARIANT varChildID, wchar_t** pszHelp)
{
    if (!pThis) return E_INVALIDARG;
    if (varChildID.vt != VT_I4) return E_INVALIDARG;
    if (varChildID.lVal == 0) return S_FALSE;
    if (!pszHelp) return E_INVALIDARG;
    if (varChildID.lVal < 0) return E_INVALIDARG;
    // TODO(clean-room): partially transcribed -- the help string comes from the
    // unmodeled +0x60 slot behind the vslot-0x160 resolution; retail's failure
    // terminal is E_INVALIDARG.
    return E_INVALIDARG;
}
// Retail (0x18000f4d0): `mov $0x80004001,%eax; ret` -- always E_NOTIMPL.
// Symbol: ?get_accHelpTopic@CMFCBaseAccessibleObject@@UEAAJPEAPEA_WUtagVARIANT@@PEAJ@Z
extern "C" long MS_ABI impl__get_accHelpTopic_CMFCBaseAccessibleObject__UEAAJPEAPEA_WUtagVARIANT__PEAJ_Z(
    void* pThis, wchar_t** /*pszHelpFile*/, VARIANT /*varChildID*/, long* /*pidTopic*/)
{
    if (!pThis) return E_NOTIMPL;
    return E_NOTIMPL;
}
// Retail (0x18000f4e0): vt != VT_I4 -> S_FALSE when lVal == 0, E_INVALIDARG
// otherwise; vt == VT_I4 lookup (vslot 0x158/0x160/0x168 + the unmodeled +0x58
// slot) ends in S_FALSE when the resolution fails.
// Symbol: ?get_accKeyboardShortcut@CMFCBaseAccessibleObject@@UEAAJUtagVARIANT@@PEAPEA_W@Z
extern "C" long MS_ABI impl__get_accKeyboardShortcut_CMFCBaseAccessibleObject__UEAAJUtagVARIANT__PEAPEA_W_Z(
    void* pThis, VARIANT varChildID, wchar_t** /*pszKeyboardShortcut*/)
{
    if (!pThis) return E_INVALIDARG;
    if (varChildID.vt != VT_I4) {
        return (varChildID.lVal == 0) ? S_FALSE : E_INVALIDARG;
    }
    // TODO(clean-room): partially transcribed -- the shortcut string lives in
    // the unmodeled +0x58 slot; without it retail's failure terminal S_FALSE is
    // returned.
    return S_FALSE;
}
// Retail (0x18000f110): vt != VT_I4 and lVal <= 0 terminate in S_OK without
// touching *pszName; the lVal > 0 child lookup writes a BSTR built from the
// unmodeled +0x40 slot and otherwise returns S_FALSE.
// Symbol: ?get_accName@CMFCBaseAccessibleObject@@UEAAJUtagVARIANT@@PEAPEA_W@Z
extern "C" long MS_ABI impl__get_accName_CMFCBaseAccessibleObject__UEAAJUtagVARIANT__PEAPEA_W_Z(
    void* pThis, VARIANT varChildID, wchar_t** /*pszName*/)
{
    if (!pThis) return S_OK;
    if (varChildID.vt != VT_I4) return S_OK;
    if (varChildID.lVal > 0) {
        // TODO(clean-room): partially transcribed -- the child name comes from
        // the unmodeled +0x40 slot behind the vslot-0x160 resolution; retail's
        // failure terminal is S_FALSE.
        return S_FALSE;
    }
    return S_OK;
}
// Retail (0x18000f0c0): NULL-checks ppdispParent (E_INVALIDARG), writes NULL to
// *ppdispParent and returns S_OK.
// Symbol: ?get_accParent@CMFCBaseAccessibleObject@@UEAAJPEAPEAUIDispatch@@@Z
extern "C" long MS_ABI impl__get_accParent_CMFCBaseAccessibleObject__UEAAJPEAPEAUIDispatch___Z(
    void* pThis, IDispatch** ppdispParent)
{
    if (!pThis) return E_INVALIDARG;
    if (!ppdispParent) return E_INVALIDARG;
    *ppdispParent = nullptr;
    return S_OK;
}
// Retail (0x18000f320): pvarRole == NULL -> E_INVALIDARG; vt != VT_I4 ->
// S_OK when lVal == 0, E_INVALIDARG otherwise; vt == VT_I4 && lVal <= 0 -> S_OK
// (resolution failure terminal); lVal > 0 writes vt = VT_I4 and the role
// stored at the unmodeled +0x70 slot.
// Symbol: ?get_accRole@CMFCBaseAccessibleObject@@UEAAJUtagVARIANT@@PEAU2@@Z
extern "C" long MS_ABI impl__get_accRole_CMFCBaseAccessibleObject__UEAAJUtagVARIANT__PEAU2__Z(
    void* pThis, VARIANT varChildID, VARIANT* pvarRole)
{
    if (!pThis) return E_INVALIDARG;
    if (!pvarRole) return E_INVALIDARG;
    if (varChildID.vt != VT_I4) {
        return (varChildID.lVal == 0) ? S_OK : E_INVALIDARG;
    }
    if (varChildID.lVal <= 0) return S_OK;
    // TODO(clean-room): partially transcribed -- the role value stored at the
    // unmodeled +0x70 slot defaults to 0.
    pvarRole->vt = VT_I4;
    pvarRole->lVal = 0;
    return S_OK;
}
// Retail (0x18000f590): returns E_INVALIDARG when pvarID is NULL and E_NOTIMPL
// otherwise (same stub as get_accFocus).
// Symbol: ?get_accSelection@CMFCBaseAccessibleObject@@UEAAJPEAUtagVARIANT@@@Z
extern "C" long MS_ABI impl__get_accSelection_CMFCBaseAccessibleObject__UEAAJPEAUtagVARIANT___Z(
    void* pThis, VARIANT* pvarID)
{
    if (!pThis) return E_INVALIDARG;
    if (!pvarID) return E_INVALIDARG;
    return E_NOTIMPL;
}
// Retail (0x18000f3c0): pvarState == NULL, vt != VT_I4 or lVal <= 0 ->
// E_INVALIDARG; the state lookup (vslot 0x158/0x160/0x168) otherwise writes
// vt = VT_I4 and the state stored at the unmodeled +0x74 slot.
// Symbol: ?get_accState@CMFCBaseAccessibleObject@@UEAAJUtagVARIANT@@PEAU2@@Z
extern "C" long MS_ABI impl__get_accState_CMFCBaseAccessibleObject__UEAAJUtagVARIANT__PEAU2__Z(
    void* pThis, VARIANT varChildID, VARIANT* pvarState)
{
    if (!pThis) return E_INVALIDARG;
    if (!pvarState) return E_INVALIDARG;
    if (varChildID.vt != VT_I4) return E_INVALIDARG;
    if (varChildID.lVal <= 0) return E_INVALIDARG;
    // TODO(clean-room): partially transcribed -- the state value stored at the
    // unmodeled +0x74 slot defaults to 0.
    pvarState->vt = VT_I4;
    pvarState->lVal = 0;
    return S_OK;
}
// Retail (0x18000f1c0): every terminal reachable without the unmodeled +0x48
// slot / vslot-0x158/0x160/0x168 resolution is S_FALSE (including the
// vt != VT_I4 case).
// Symbol: ?get_accValue@CMFCBaseAccessibleObject@@UEAAJUtagVARIANT@@PEAPEA_W@Z
extern "C" long MS_ABI impl__get_accValue_CMFCBaseAccessibleObject__UEAAJUtagVARIANT__PEAPEA_W_Z(
    void* pThis, VARIANT /*varChildID*/, wchar_t** /*pszValue*/)
{
    if (!pThis) return S_FALSE;
    // TODO(clean-room): partially transcribed -- the value string lives in the
    // unmodeled +0x48 slot; without it retail's failure terminal S_FALSE is
    // returned.
    return S_FALSE;
}
// Retail (0x180003a60): `mov $0x1,%eax; ret` -- always S_FALSE.
// Symbol: ?put_accName@CMFCBaseAccessibleObject@@UEAAJUtagVARIANT@@PEA_W@Z
extern "C" long MS_ABI impl__put_accName_CMFCBaseAccessibleObject__UEAAJUtagVARIANT__PEA_W_Z(
    void* pThis, VARIANT /*varID*/, wchar_t* /*szName*/)
{
    if (!pThis) return S_FALSE;
    return S_FALSE;
}
// Retail (0x180003a60): `mov $0x1,%eax; ret` -- always S_FALSE.
// Symbol: ?put_accValue@CMFCBaseAccessibleObject@@UEAAJUtagVARIANT@@PEA_W@Z
extern "C" long MS_ABI impl__put_accValue_CMFCBaseAccessibleObject__UEAAJUtagVARIANT__PEA_W_Z(
    void* pThis, VARIANT /*varID*/, wchar_t* /*szValue*/)
{
    if (!pThis) return S_FALSE;
    return S_FALSE;
}
// Symbol: ??0CMFCBaseAccessibleObject@@QEAA@XZ
extern "C" void* MS_ABI impl___0CMFCBaseAccessibleObject__QEAA_XZ(void* pThis) {
    return pThis;
}
// Symbol: ??1CMFCBaseAccessibleObject@@UEAA@XZ
extern "C" void MS_ABI impl___1CMFCBaseAccessibleObject__UEAA_XZ(void* pThis) {
    (void)pThis;
}
