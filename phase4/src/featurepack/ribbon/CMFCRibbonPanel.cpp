// CMFCRibbonPanel — OpenMFC implementation.
// Sources: cbarcore.cpp, global_mfc-feature-23_impl.cpp, mfccore.cpp, ole_olectors_exports.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/CbarcoreSupport.h"
#include "detail/MfcFeature23ImplSupport.h"
#include "detail/MfccoreSupport.h"

#include "openmfc/afxole.h"
#include "openmfc/afxmfc.h"

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

























// Symbol: ??1CMFCRibbonPanel@@UEAA@XZ
extern "C" void MS_ABI impl___1CMFCRibbonPanel__UEAA_XZ(void* pThis) {
    auto* panel = reinterpret_cast<CMFCRibbonPanel*>(pThis);
    if (!panel) return;

    {
        std::lock_guard<std::mutex> lock(g_ribbonMutex);
        auto panelIt = g_ribbonPanels.find(panel);
        if (panelIt != g_ribbonPanels.end()) {
            std::vector<CMFCRibbonBaseElement*> ownedElements(panelIt->second.ownedElements.begin(), panelIt->second.ownedElements.end());
            g_ribbonPanels.erase(panelIt);
            for (CMFCRibbonBaseElement* element : ownedElements) {
                delete element;
            }
        }
        for (auto& [_, categoryState] : g_ribbonCategories) {
            categoryState.ownedPanels.erase(panel);
            auto& panels = categoryState.panels;
            panels.erase(std::remove(panels.begin(), panels.end(), panel), panels.end());
        }
    }

    panel->~CMFCRibbonPanel();
}
// Symbol: ?Add@CMFCRibbonPanel@@UEAAXPEAVCMFCRibbonBaseElement@@@Z
extern "C" void MS_ABI impl__Add_CMFCRibbonPanel__UEAAXPEAVCMFCRibbonBaseElement___Z(
    CMFCRibbonPanel* pThis, CMFCRibbonBaseElement* pElement) {
    if (!pThis || !pElement) return;
    std::lock_guard<std::mutex> lock(g_ribbonMutex);
    auto& state = g_ribbonPanels[pThis];
    state.elements.push_back(pElement);
    state.ownedElements.insert(pElement);
}
// Symbol: ?AddSeparator@CMFCRibbonPanel@@UEAAXXZ
extern "C" void MS_ABI impl__AddSeparator_CMFCRibbonPanel__UEAAXXZ(CMFCRibbonPanel* pThis) {
    if (!pThis) return;
    std::lock_guard<std::mutex> lock(g_ribbonMutex);
    g_ribbonPanels[pThis].elements.push_back(nullptr);
}
// Symbol: ?GetCount@CMFCRibbonPanel@@QEBAHXZ
extern "C" int MS_ABI impl__GetCount_CMFCRibbonPanel__QEBAHXZ(const CMFCRibbonPanel* pThis) {
    if (!pThis) return 0;
    std::lock_guard<std::mutex> lock(g_ribbonMutex);
    auto it = g_ribbonPanels.find(const_cast<CMFCRibbonPanel*>(pThis));
    return (it == g_ribbonPanels.end()) ? 0 : static_cast<int>(it->second.elements.size());
}
// Symbol: ?GetElement@CMFCRibbonPanel@@QEBAPEAVCMFCRibbonBaseElement@@H@Z
extern "C" CMFCRibbonBaseElement* MS_ABI impl__GetElement_CMFCRibbonPanel__QEBAPEAVCMFCRibbonBaseElement__H_Z(
    const CMFCRibbonPanel* pThis, int nIndex) {
    if (!pThis || nIndex < 0) return nullptr;
    std::lock_guard<std::mutex> lock(g_ribbonMutex);
    auto it = g_ribbonPanels.find(const_cast<CMFCRibbonPanel*>(pThis));
    if (it == g_ribbonPanels.end()) return nullptr;
    const auto& elements = it->second.elements;
    if (nIndex >= static_cast<int>(elements.size())) return nullptr;
    return elements[static_cast<size_t>(nIndex)];
}
// Symbol: ?CreateObject@CMFCRibbonPanel@@SAPEAVCObject@@XZ
extern "C" CObject* MS_ABI impl__CreateObject_CMFCRibbonPanel__SAPEAVCObject__XZ() {
    return CMFCRibbonPanel::GetThisClass()->CreateObject();
}
// Symbol: ?GetRuntimeClass@CMFCRibbonPanel@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CMFCRibbonPanel__UEBAPEAUCRuntimeClass__XZ(
    const CMFCRibbonPanel* pThis) {
    return CMFCRibbonPanel::GetThisClass();
}
// Symbol: ?GetThisClass@CMFCRibbonPanel@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMFCRibbonPanel__SAPEAUCRuntimeClass__XZ() {
    return CMFCRibbonPanel::GetThisClass();
}
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
// CMFCRibbonPanel::GetCaptionSize(CDC*) const -- retail (RVA 0x11acf0)
// measures the name CString (+0x140) with GetTextExtentPoint32 and folds in
// the caption height (+0xf4); the +0xc0 flag makes it return CSize(0,0).
// m_strName is protected and the layout members are not modeled, so the
// empty-caption size is the only truthful result.
//
// ABI note (fixed by the reviewer): for an MSVC x64 non-static member
// function returning a large struct, `this` stays in RCX and the hidden
// return buffer is passed in RDX -- retail reads +0xc0/+0x140/+0xf4/+0x288
// through RCX, stores the CSize through RDX and takes pDC in R8
// (`cmpl $0x0,0xc0(%rcx)` / `mov %edx,(%rbx)` with rbx = the entry RDX /
// `mov 0x10(%rax),%rcx` with rax = the entry R8).  The same order is visible
// in ?GetGalleryRect@ (0x11dfe0) and ?GetPaltteMinSize@ (0x11d6a0) below and
// in retail's own call to GetGalleryRect at 0x11df1f.  This thunk previously
// declared the return buffer first, which made an MSVC client write the size
// into the panel object and dereference its return buffer as a panel.
// TODO(clean-room): transcribed partially -- the caption layout state is not
// modeled.
// Symbol: ?GetCaptionSize@CMFCRibbonPanel@@IEBA?AVCSize@@PEAVCDC@@@Z
extern "C" CSize* MS_ABI impl__GetCaptionSize_CMFCRibbonPanel__IEBA_AVCSize__PEAVCDC___Z(
    const CMFCRibbonPanel* /*pThis*/, CSize* pRet, CDC* /*pDC*/)
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
CMFCRibbonPanel::CMFCRibbonPanel(const wchar_t* lpszName, HICON hIcon)
    : m_hIcon(hIcon) {
    if (lpszName) m_strName = lpszName;
    memset(_ribbonpanel_padding, 0, sizeof(_ribbonpanel_padding));
    g_ribbonPanelStates[this];
}
CMFCRibbonPanel::~CMFCRibbonPanel() {
    ClearRibbonPanelState(this);
}
void CMFCRibbonPanel::Add(CMFCRibbonBaseElement* pElem) {
    if (!pElem) return;
    RibbonPanelState& state = g_ribbonPanelStates[this];
    state.elements.push_back(pElem);
    state.ownedElements.insert(pElem);
}
void CMFCRibbonPanel::AddSeparator() {
    g_ribbonPanelStates[this].elements.push_back(nullptr);
}
int CMFCRibbonPanel::GetCount() const {
    auto it = g_ribbonPanelStates.find(this);
    return it == g_ribbonPanelStates.end() ? 0 : static_cast<int>(it->second.elements.size());
}
CMFCRibbonBaseElement* CMFCRibbonPanel::GetElement(int nIndex) const {
    auto it = g_ribbonPanelStates.find(this);
    if (it == g_ribbonPanelStates.end()) return nullptr;
    if (nIndex < 0 || nIndex >= static_cast<int>(it->second.elements.size())) return nullptr;
    return it->second.elements[static_cast<size_t>(nIndex)];
}
CString CMFCRibbonPanel::GetName() const { return m_strName; }
// Symbol: ??0CMFCRibbonPanel@@IEAA@PEAVCMFCRibbonGallery@@@Z
// CMFCRibbonPanel::CMFCRibbonPanel()
extern "C" void* MS_ABI impl___0CMFCRibbonPanel__IEAA_PEAVCMFCRibbonGallery___Z(void* /*p0*/) {
    return new CMFCRibbonPanel();
}
// Symbol: ??0CMFCRibbonPanel@@IEAA@PEB_WPEAUHICON__@@@Z
// CMFCRibbonPanel::CMFCRibbonPanel(const wchar_t* lpszName, HICON hIcon)
extern "C" void* MS_ABI impl___0CMFCRibbonPanel__IEAA_PEB_WPEAUHICON_____Z(
    const wchar_t* pName, void* hIcon) {
    return new CMFCRibbonPanel(pName, static_cast<HICON>(hIcon));
}
//=============================================================================
// CMFCRibbonPanel -- element-collection and list-mutation exports.
//
// The retail panel keeps its elements in a CArray at +0x6d8 (data) / +0x6e0
// (count) and dispatches to the element virtuals for the per-element work.
// OpenMFC's clean-room CMFCRibbonPanel declares none of those members and
// CMFCRibbonBaseElement declares none of those virtuals, so the element list
// lives in the ribbon_state side table (see CMFCRibbonPanel::Add above) and
// the bodies below call the exported base-element implementations directly
// instead of dispatching through a vtable.  A separator is a null entry in
// that list (see CMFCRibbonPanel::AddSeparator above), so null entries are
// skipped by every scan.
//=============================================================================

// Base-element implementations the retail panel reaches through the element
// vtable; definitions live in featurepack/ribbon/CMFCRibbonBaseElement.cpp.
extern "C" void MS_ABI impl__GetElementsByID_CMFCRibbonBaseElement__UEAAXIAEAV__CArray_PEAVCMFCRibbonBaseElement__PEAV1____Z(
    CMFCRibbonBaseElement* pThis, unsigned int uiCmd,
    CArray<CMFCRibbonBaseElement*, CMFCRibbonBaseElement*>& arElements);
extern "C" void MS_ABI impl__GetItemIDsList_CMFCRibbonBaseElement__MEBAXAEAV__CList_II___Z(
    const CMFCRibbonBaseElement* pThis, CList<unsigned int, unsigned int>& lstIDs);

namespace {

// The IAccessible entry points below take/return a VARIANT.  On x64 a VARIANT
// is 16 bytes, so MSVC passes it by pointer to a caller-owned copy -- which is
// exactly what the retail bodies do (`cmpw $0x3,(%rdx)`, `mov %eax,0x8(%rdi)`).
// Only the two fields those bodies touch are needed here: the discriminant at
// +0 and the long at +8.
struct RibbonAccVariant {
    unsigned short vt;
    unsigned short wReserved1;
    unsigned short wReserved2;
    unsigned short wReserved3;
    long           lVal;
};
static_assert(sizeof(RibbonAccVariant) >= 12, "VARIANT prefix");

constexpr unsigned short kVarTypeI4 = 3;      // VT_I4
constexpr long kHrOk = 0L;                    // S_OK
constexpr long kHrFalse = 1L;                 // S_FALSE
constexpr long kHrInvalidArg = (long)0x80070057L; // E_INVALIDARG

} // namespace

// CMFCRibbonPanel::accDoDefaultAction(VARIANT) -- retail (RVA 0x11e710),
// transcribed:
//     if (varChild.vt != VT_I4)  return E_INVALIDARG;   // cmpw $0x3,(%rdx)
//     if (varChild.lVal == 0)    return S_FALSE;        // 0x18011e7bc, eax = 1
//     CArray<CMFCRibbonBaseElement*, CMFCRibbonBaseElement*> ar;
//     GetVisibleElements(ar);                           // 0x18011dd10
//     if (varChild.lVal - 1 < 0 || varChild.lVal - 1 >= ar.GetSize())
//         return E_INVALIDARG;                          // 0x18011e7cc
//     ar[varChild.lVal - 1]->OnAccDefaultAction();       // element vtable +0x2c8
//     return S_OK;
// The visible-element set is decided by the element rect at +0xc8 (base
// GetVisibleElements, RVA 0x11b90), which OpenMFC does not model, so the local
// array is always empty here and every varChild.lVal != 0 lands on retail's
// own out-of-range E_INVALIDARG.  That makes the whole return contract exactly
// determined for the modeled state, so it is transcribed rather than stubbed.
// Deviation: the null-pointer guard.  Retail dereferences the VARIANT copy
// unconditionally (MSVC passes the 16-byte VARIANT by pointer); a null
// pointer there is a caller bug, and E_INVALIDARG beats an access violation.
// TODO(clean-room): partially transcribed -- element rects are not modeled,
// so the "perform the child's default action" path is unreachable.
// Symbol: ?accDoDefaultAction@CMFCRibbonPanel@@MEAAJUtagVARIANT@@@Z
extern "C" long MS_ABI impl__accDoDefaultAction_CMFCRibbonPanel__MEAAJUtagVARIANT___Z(
    CMFCRibbonPanel* /*pThis*/, const RibbonAccVariant* pvarChild)
{
    if (pvarChild == nullptr) return kHrInvalidArg;
    if (pvarChild->vt != kVarTypeI4) return kHrInvalidArg;
    if (pvarChild->lVal == 0) return kHrFalse;
    return kHrInvalidArg; // no visible child can be resolved
}

// CMFCRibbonPanel::accHitTest(long, long, VARIANT*) -- retail (RVA 0x11e560),
// transcribed:
//     if (pvarChild == NULL) return E_INVALIDARG;
//     pvarChild->vt = VT_I4; pvarChild->lVal = 0;   // movw $0x3 / movl $0x0
//     if (<pointer at +0x158> == NULL)      return S_FALSE;   // 0x18011e6e3
//     CWnd* pWnd = GetParentWnd();                            // vtable +0x158
//     if (pWnd == NULL || pWnd->m_hWnd == NULL) return S_FALSE;
//     ... map the screen point through the parent window, walk the visible
//     elements and, on a hit, set pvarChild->lVal = i + 1 ...
//     return S_OK;
// The +0x158 parent-category pointer is not declared by the clean-room
// CMFCRibbonPanel, so retail's first bail-out is the only reachable path and
// the result -- VT_I4 / 0 written, S_FALSE returned -- is exact for the
// modeled state.  Returning S_OK while leaving the caller's VARIANT untouched
// (what the generated stub did) would hand an accessibility client an
// uninitialised child id.
// TODO(clean-room): partially transcribed -- the parent link and the panel /
// element rects are not modeled, so no child can be hit.
// Symbol: ?accHitTest@CMFCRibbonPanel@@MEAAJJJPEAUtagVARIANT@@@Z
extern "C" long MS_ABI impl__accHitTest_CMFCRibbonPanel__MEAAJJJPEAUtagVARIANT___Z(
    CMFCRibbonPanel* /*pThis*/, long /*xLeft*/, long /*yTop*/, RibbonAccVariant* pvarChild)
{
    if (pvarChild == nullptr) return kHrInvalidArg;
    pvarChild->vt = kVarTypeI4;
    pvarChild->lVal = 0;
    return kHrFalse; // no parent window is modeled: nothing can be hit
}

// CMFCRibbonPanel::accLocation(long*, long*, long*, long*, VARIANT) -- retail
// (RVA 0x11e280) returns E_INVALIDARG only when one of the four output
// pointers is null; a varChild whose vt is not VT_I4 (and a negative lVal)
// returns S_OK without writing anything.  For lVal == 0 it maps the panel rect
// (+0x10c) through the parent window obtained from the vtable +0x158
// GetParentWnd; for lVal > 0 it calls OnSetAccData (vtable +0x160) and reports
// the accessibility rect cached at +0x7c..+0x88.  Neither the panel rect, the
// parent window, nor the +0x40 accessibility block is modeled.
// TODO(clean-room): transcribed partially -- the panel rect, the parent
// window and the accessibility data block are not modeled.
// Symbol: ?accLocation@CMFCRibbonPanel@@MEAAJPEAJ000UtagVARIANT@@@Z
extern "C" long MS_ABI impl__accLocation_CMFCRibbonPanel__MEAAJPEAJ000UtagVARIANT___Z(long* p0, long* p1, long* p2, long* p3, void* /*struct*/ p4) {
    return 0;
}

// CMFCRibbonPanel::accNavigate(long, VARIANT, VARIANT*) -- retail (RVA
// 0x11e3c0) returns E_INVALIDARG unless the output VARIANT is non-null and
// varStart.vt == VT_I4 -- and note that between those two tests it already
// writes VT_EMPTY into the output VARIANT's vt (`mov %ax,(%r9)` with eax 0),
// which this stub does not do; it then fills a local array through
// CMFCRibbonPanel::GetVisibleElements (0x11dd10) and dispatches on navDir
// (a chain of `sub $1 / je` tests) to pick the requested neighbour in that
// array.  The visible-element set depends on the unmodeled element rects, so
// no navigation target can be produced.
// TODO(clean-room): transcribed partially -- element rects are not modeled.
// Symbol: ?accNavigate@CMFCRibbonPanel@@MEAAJJUtagVARIANT@@PEAU2@@Z
extern "C" long MS_ABI impl__accNavigate_CMFCRibbonPanel__MEAAJJUtagVARIANT__PEAU2__Z(long p0, void* /*struct*/ p1, void* /*struct*/* p2) {
    return 0;
}

// CMFCRibbonPanel::get_accChildCount(long*) -- retail (RVA 0x11e200),
// transcribed:
//     if (pcountChildren == NULL) return E_INVALIDARG;
//     CArray<CMFCRibbonBaseElement*, CMFCRibbonBaseElement*> ar;
//     GetVisibleElements(ar);                       // 0x18011dd10
//     *pcountChildren = ar.GetSize();
//     return S_OK;
// CMFCRibbonPanel::GetVisibleElements dispatches to each element's
// GetVisibleElements (base implementation RVA 0x11b90: append `this` only
// when the element rect at +0xc8 is not empty).  OpenMFC models no element
// rects, so the visible set is always empty and the count is always 0.
// TODO(clean-room): partially transcribed -- element rects are not modeled,
// so the reported child count is always 0.
// Symbol: ?get_accChildCount@CMFCRibbonPanel@@MEAAJPEAJ@Z
extern "C" long MS_ABI impl__get_accChildCount_CMFCRibbonPanel__MEAAJPEAJ_Z(
    CMFCRibbonPanel* /*pThis*/, long* pcountChildren)
{
    if (pcountChildren == nullptr) return kHrInvalidArg;
    *pcountChildren = 0;
    return kHrOk;
}

// CMFCRibbonPanel::get_accParent(IDispatch**) -- retail (RVA 0x11e1a0),
// transcribed:
//     if (ppdispParent == NULL) return E_INVALIDARG;
//     CCmdTarget* pParent = <pointer at +0x158>;        // parent category
//     pParent->ExternalAddRef();                        // 0x18026bb40
//     *ppdispParent = pParent->GetInterface(&IID_IDispatch); // 0x18026bc00
//     return *ppdispParent != NULL ? S_OK : E_INVALIDARG;
// (the trailing `neg/sbb/not/and 0x80070057` sequence is exactly that
// conditional).  Note that retail does NOT test the +0x158 pointer: with no
// parent category it calls ExternalAddRef on NULL and faults.  There is
// therefore no "retail no-parent path" to copy; E_INVALIDARG is retail's
// answer for the neighbouring case -- a parent that exposes no IDispatch --
// and it is the honest one here, since the +0x158 parent pointer is not
// declared by the clean-room CMFCRibbonPanel and no parent IDispatch can be
// produced.  *ppdispParent is cleared first so the caller never reads an
// uninitialised interface pointer.
// TODO(clean-room): partially transcribed -- the +0x158 parent link is not
// modeled, so the parent IDispatch is never available.
// Symbol: ?get_accParent@CMFCRibbonPanel@@MEAAJPEAPEAUIDispatch@@@Z
extern "C" long MS_ABI impl__get_accParent_CMFCRibbonPanel__MEAAJPEAPEAUIDispatch___Z(
    CMFCRibbonPanel* /*pThis*/, void** ppdispParent)
{
    if (ppdispParent == nullptr) return kHrInvalidArg;
    *ppdispParent = nullptr;
    return kHrInvalidArg;
}

// CMFCRibbonPanel::GetElements(CArray<CMFCRibbonBaseElement*,
// CMFCRibbonBaseElement*>&) -- retail (RVA 0x11c450), transcribed:
//     for (i = 0; i < m_arElements.GetSize(); i++)
//         m_arElements[i]->GetElements(arElements);        // vtable +0x3e8
//     if (<group at +0x170>.GetSize() > 0)                 // count at +0x288
//         arElements.Add(&<group at +0x170>);
//     if (!<vtable +0x178 predicate>())
//         arElements.Add(&<default button at +0x3f0>);
// The array is NOT cleared on entry.  The base element's GetElements (the
// vtable +0x3e8 slot, RVA 0xe2a0) is exactly `arElements.Add(this)`, which is
// what the loop below reproduces for the modeled element list.
// The vtable +0x178 predicate is a per-class constant, not state: in
// CMFCRibbonPanel's own vtable (RVA 0x3072e0) that slot is RVA 0x7260
// (`xor eax,eax; ret`), so retail DOES append the embedded default panel
// button here; in CMFCRibbonMainPanel's vtable (RVA 0x305928) it is RVA
// 0x3ae0 (`mov eax,1; ret`), which suppresses it.  Neither embedded object --
// the buttons group at +0x170 nor the default panel button at +0x3f0 -- is
// declared by the clean-room CMFCRibbonPanel, so neither can be appended;
// that is the deviation, not the predicate.
// TODO(clean-room): partially transcribed -- the embedded buttons group and
// default panel button are not modeled.
// Symbol: ?GetElements@CMFCRibbonPanel@@QEAAXAEAV?$CArray@PEAVCMFCRibbonBaseElement@@PEAV1@@@@Z
extern "C" void MS_ABI impl__GetElements_CMFCRibbonPanel__QEAAXAEAV__CArray_PEAVCMFCRibbonBaseElement__PEAV1____Z(
    CMFCRibbonPanel* pThis,
    CArray<CMFCRibbonBaseElement*, CMFCRibbonBaseElement*>& arElements)
{
    if (!pThis) return;

    std::lock_guard<std::mutex> lock(RibbonMutex());
    auto it = RibbonPanelStates().find(pThis);
    if (it == RibbonPanelStates().end()) return;

    for (CMFCRibbonBaseElement* pElem : it->second.elements) {
        if (pElem) {
            arElements.Add(pElem);
        }
    }
}

// CMFCRibbonPanel::GetElementsByID(unsigned int, CArray<CMFCRibbonBaseElement*,
// CMFCRibbonBaseElement*>&) -- retail (RVA 0x11c5c0), transcribed:
//     for (i = 0; i < m_arElements.GetSize(); i++)
//         m_arElements[i]->GetElementsByID(uiCmd, arElements);  // vtable +0x3e0
//     <default button at +0x3f0>->GetElementsByID(uiCmd, arElements);
//     <group at +0x170>->GetElementsByID(uiCmd, arElements);     // tail call
// The array is NOT cleared on entry.  The loop forwards to the exported base
// implementation, which appends `this` when its command ID matches.  The
// embedded default panel button (+0x3f0) and buttons group (+0x170) are not
// declared by the clean-room CMFCRibbonPanel, so they contribute nothing.
// TODO(clean-room): partially transcribed -- the embedded default panel
// button and buttons group are not modeled.
// Symbol: ?GetElementsByID@CMFCRibbonPanel@@QEAAXIAEAV?$CArray@PEAVCMFCRibbonBaseElement@@PEAV1@@@@Z
extern "C" void MS_ABI impl__GetElementsByID_CMFCRibbonPanel__QEAAXIAEAV__CArray_PEAVCMFCRibbonBaseElement__PEAV1____Z(
    CMFCRibbonPanel* pThis, unsigned int uiCmd,
    CArray<CMFCRibbonBaseElement*, CMFCRibbonBaseElement*>& arElements)
{
    if (!pThis) return;

    std::lock_guard<std::mutex> lock(RibbonMutex());
    auto it = RibbonPanelStates().find(pThis);
    if (it == RibbonPanelStates().end()) return;

    for (CMFCRibbonBaseElement* pElem : it->second.elements) {
        if (pElem) {
            impl__GetElementsByID_CMFCRibbonBaseElement__UEAAXIAEAV__CArray_PEAVCMFCRibbonBaseElement__PEAV1____Z(
                pElem, uiCmd, arElements);
        }
    }
}

// CMFCRibbonPanel::GetFirstTabStop() const -- retail (RVA 0x11d270) runs up
// to three forward scans of m_arElements.  When the +0x168 parent pointer is
// non-null it first scans for elements that are of NEITHER of the runtime
// classes at 0x1803048e0 / 0x180307218 and whose +0x16c flag is non-zero,
// then for elements that ARE of the 0x180307218 class; when +0x168 is NULL it
// goes straight to the third, unfiltered scan.  Every scan asks the element
// for its own first tab stop (vtable +0x420, base RVA 0xe300 = "return this
// when the vtable +0x248 predicate holds and the element rect at +0xc8 is not
// empty") and returns the first non-null answer, else NULL.  OpenMFC models
// neither the +0x168 parent pointer nor the element rects, so no element can
// qualify as a tab stop.
// TODO(clean-room): transcribed partially -- element rects and the +0x168
// parent link are not modeled.
// Symbol: ?GetFirstTabStop@CMFCRibbonPanel@@IEBAPEAVCMFCRibbonBaseElement@@XZ
extern "C" void* MS_ABI impl__GetFirstTabStop_CMFCRibbonPanel__IEBAPEAVCMFCRibbonBaseElement__XZ() {
    return nullptr;
}

// CMFCRibbonPanel::GetFocused() const -- retail (RVA 0x11dc50) checks, in
// order: the embedded default panel button (+0x3f0) -- returned when its rect
// (+0xc8) is not empty and its IsFocused (vtable +0x1b8) is TRUE; the
// embedded buttons group (+0x170) -- returned on IsFocused alone, with no
// rect test; and finally the first element whose GetFocused (vtable +0x3a8)
// is non-null.  The embedded objects are not declared by the clean-room
// CMFCRibbonPanel and OpenMFC models no element focus state
// (impl__GetFocused_CMFCRibbonBaseElement always returns nullptr), so no
// focused element can be reported -- the same treatment as
// CMFCRibbonCategory::GetFocused.
// TODO(clean-room): transcribed partially -- element focus state is not
// modeled.
// Symbol: ?GetFocused@CMFCRibbonPanel@@QEBAPEAVCMFCRibbonBaseElement@@XZ
extern "C" void* MS_ABI impl__GetFocused_CMFCRibbonPanel__QEBAPEAVCMFCRibbonBaseElement__XZ() {
    return nullptr;
}

// CMFCRibbonPanel::GetGalleryRect() -- retail (RVA 0x11dfe0), transcribed:
//     *pRet = <CRect at +0x10c>;
//     if (!<CRect at +0x11c>.IsRectEmpty()) pRet->top    = <+0x11c>.bottom; // +0x128
//     if (!<CRect at +0x12c>.IsRectEmpty()) pRet->bottom = <+0x12c>.top;    // +0x130
//     return pRet;
// None of those three rects is declared by the clean-room CMFCRibbonPanel, so
// an empty rect is the only truthful result -- the same treatment as
// CMFCRibbonPanel::GetCaptionSize above.
//
// ABI note: for an MSVC x64 member function returning a large struct, `this`
// stays in RCX and the hidden return buffer is passed in RDX (retail loads
// the member rect through RCX and stores the result through RDX), and the
// buffer is returned in RAX -- hence the (pThis, pRet) parameter order here.
// TODO(clean-room): transcribed partially -- the panel layout rects are not
// modeled.
// Symbol: ?GetGalleryRect@CMFCRibbonPanel@@QEAA?AVCRect@@XZ
extern "C" CRect* MS_ABI impl__GetGalleryRect_CMFCRibbonPanel__QEAA_AVCRect__XZ(
    CMFCRibbonPanel* /*pThis*/, CRect* pRet)
{
    if (!pRet) return nullptr;
    pRet->left = 0;
    pRet->top = 0;
    pRet->right = 0;
    pRet->bottom = 0;
    return pRet;
}

// CMFCRibbonPanel::GetHeight(CDC*) const -- retail (RVA 0x1182f0) measures
// the embedded default panel button (+0x3f0) through its vtable +0x310 and
// +0x200 size virtuals, folds in a text height derived from the parent
// category's image size (+0x158 -> +0x838), and then walks m_arElements
// accumulating the row heights.  None of those members is declared by the
// clean-room CMFCRibbonPanel.
// TODO(clean-room): transcribed partially -- panel layout geometry is not
// modeled.
// Symbol: ?GetHeight@CMFCRibbonPanel@@MEBAHPEAVCDC@@@Z
extern "C" int MS_ABI impl__GetHeight_CMFCRibbonPanel__MEBAHPEAVCDC___Z(void* /*class*/* p0) {
    return 0;
}

// CMFCRibbonPanel::GetHighlighted() const -- retail (RVA 0x11c780) is a
// single load: `return m_pHighlighted;` (the pointer at +0x3e8).  That member
// is not declared by the clean-room CMFCRibbonPanel and nothing in OpenMFC
// tracks ribbon highlighting, so nullptr is the truthful result -- the same
// treatment as CMFCRibbonCategory::GetHighlighted.
// TODO(clean-room): transcribed partially -- the +0x3e8 highlighted-element
// pointer is not modeled.
// Symbol: ?GetHighlighted@CMFCRibbonPanel@@QEBAPEAVCMFCRibbonBaseElement@@XZ
extern "C" void* MS_ABI impl__GetHighlighted_CMFCRibbonPanel__QEBAPEAVCMFCRibbonBaseElement__XZ() {
    return nullptr;
}

// CMFCRibbonPanel::GetIndex(CMFCRibbonBaseElement*) const -- retail (RVA
// 0x11b760), transcribed in full:
//     for (i = 0; i < m_arElements.GetSize(); i++)
//         if (m_arElements[i] == pElem) return i;
//     return -1;
// The element list is the ribbon_state side-table vector, so this is exact
// apart from the separator representation (retail stores CMFCRibbonSeparator
// objects where OpenMFC stores null entries; both occupy one index).
// Symbol: ?GetIndex@CMFCRibbonPanel@@UEBAHPEAVCMFCRibbonBaseElement@@@Z
extern "C" int MS_ABI impl__GetIndex_CMFCRibbonPanel__UEBAHPEAVCMFCRibbonBaseElement___Z(
    const CMFCRibbonPanel* pThis, CMFCRibbonBaseElement* pElem)
{
    if (!pThis) return -1;

    std::lock_guard<std::mutex> lock(RibbonMutex());
    auto it = RibbonPanelStates().find(pThis);
    if (it == RibbonPanelStates().end()) return -1;

    const auto& elements = it->second.elements;
    for (size_t i = 0; i < elements.size(); ++i) {
        if (elements[i] == pElem) {
            return static_cast<int>(i);
        }
    }
    return -1;
}

// CMFCRibbonPanel::GetItemIDsList(CList<UINT, UINT>&) const -- retail (RVA
// 0x11c530), transcribed:
//     for (i = 0; i < m_arElements.GetSize(); i++)
//         m_arElements[i]->GetItemIDsList(lstIDs);        // vtable +0x478
//     <default button at +0x3f0>->GetItemIDsList(lstIDs); // tail call
// The list is NOT cleared on entry.  The loop forwards to the exported base
// implementation (append the command ID unless it is 0 or 0xffffffff, and
// only when not already present).  The embedded default panel button (+0x3f0)
// is not declared by the clean-room CMFCRibbonPanel.
// TODO(clean-room): partially transcribed -- the embedded default panel
// button is not modeled.
// Symbol: ?GetItemIDsList@CMFCRibbonPanel@@QEBAXAEAV?$CList@II@@@Z
extern "C" void MS_ABI impl__GetItemIDsList_CMFCRibbonPanel__QEBAXAEAV__CList_II___Z(
    const CMFCRibbonPanel* pThis, CList<unsigned int, unsigned int>& lstIDs)
{
    if (!pThis) return;

    std::lock_guard<std::mutex> lock(RibbonMutex());
    auto it = RibbonPanelStates().find(pThis);
    if (it == RibbonPanelStates().end()) return;

    for (CMFCRibbonBaseElement* pElem : it->second.elements) {
        if (pElem) {
            impl__GetItemIDsList_CMFCRibbonBaseElement__MEBAXAEAV__CList_II___Z(pElem, lstIDs);
        }
    }
}

// CMFCRibbonPanel::GetLastTabStop() const -- retail (RVA 0x11d3e0) is the
// backwards mirror of GetFirstTabStop: up to three reverse scans of
// m_arElements gated the same way on the +0x168 parent pointer and the same
// two runtime classes (this one wants +0x16c == 0 in its first scan), the
// last of which calls the element's GetLastTabStop (vtable +0x428).  Both
// +0x420 and +0x428 fold to the same base body (RVA 0xe300) = "return this
// when the vtable +0x248 predicate holds and the element rect at +0xc8 is not
// empty".  OpenMFC models neither the +0x168 parent pointer nor the element
// rects.
// TODO(clean-room): transcribed partially -- element rects and the +0x168
// parent link are not modeled.
// Symbol: ?GetLastTabStop@CMFCRibbonPanel@@IEBAPEAVCMFCRibbonBaseElement@@XZ
extern "C" void* MS_ABI impl__GetLastTabStop_CMFCRibbonPanel__IEBAPEAVCMFCRibbonBaseElement__XZ() {
    return nullptr;
}

// CMFCRibbonPanel::GetMinWidth(CDC*) const -- retail (RVA 0x11b2a0) calls
// the default panel button's (+0x3f0) vtable +0x310 slot and then its
// vtable +0x200 size getter, returning the cx of the resulting CSize.  The
// embedded default panel button is not declared by the clean-room
// CMFCRibbonPanel.
// TODO(clean-room): transcribed partially -- the embedded default panel
// button is not modeled.
// Symbol: ?GetMinWidth@CMFCRibbonPanel@@IEAAHPEAVCDC@@@Z
extern "C" int MS_ABI impl__GetMinWidth_CMFCRibbonPanel__IEAAHPEAVCDC___Z(void* /*class*/* p0) {
    return 0;
}

// CMFCRibbonPanel::GetPaltteMinSize() const -- retail (RVA 0x11d6a0) opens
// with exactly this:
//     if (<pointer at +0x168> == NULL) { pRet->cx = -1; pRet->cy = -1; return pRet; }
// and only then (about 0x290 further bytes) measures the parent menu window
// and the gallery items to compute a real minimum size.  The +0x168 parent
// pointer is not declared by the clean-room CMFCRibbonPanel and nothing in
// OpenMFC ever establishes it, so the early-out is the branch that applies.
//
// ABI note: same hidden-return-buffer convention as GetGalleryRect above --
// retail reads +0x168 through RCX and writes the CSize through RDX.
// TODO(clean-room): transcribed partially -- only the null-parent early-out
// is reachable; the palette measurement path is not modeled.
// Symbol: ?GetPaltteMinSize@CMFCRibbonPanel@@IEBA?AVCSize@@XZ
extern "C" CSize* MS_ABI impl__GetPaltteMinSize_CMFCRibbonPanel__IEBA_AVCSize__XZ(
    const CMFCRibbonPanel* /*pThis*/, CSize* pRet)
{
    if (!pRet) return nullptr;
    pRet->cx = -1;
    pRet->cy = -1;
    return pRet;
}

// CMFCRibbonPanel::GetParentButton() const -- retail (RVA 0x11d660) returns
// NULL when the parent window pointer at +0x160 is NULL; otherwise it reads
// that window's m_hWnd (+0x40), passes it through an imported USER32 call,
// hands the result to the helper at 0x180289180 and returns the pointer that
// object holds at +0x19b8.  Neither the +0x160 parent window nor that owner
// object is modeled.
// TODO(clean-room): transcribed partially -- the +0x160 parent window link is
// not modeled.
// Symbol: ?GetParentButton@CMFCRibbonPanel@@QEBAPEAVCMFCRibbonBaseElement@@XZ
extern "C" void* MS_ABI impl__GetParentButton_CMFCRibbonPanel__QEBAPEAVCMFCRibbonBaseElement__XZ() {
    return nullptr;
}

// CMFCRibbonPanel::GetParentWnd() const -- retail (RVA 0x11c310), in full:
//     if (<pointer at +0x160> != NULL) return <+0x160>;
//     if (<pointer at +0x158> == NULL) return NULL;
//     return <+0x158>-><pointer at +0x760>;
// Neither the +0x160 parent-window pointer nor the +0x158 parent-category
// pointer is declared by the clean-room CMFCRibbonPanel, and the ribbon_state
// side table records no panel -> bar link, so the parent window cannot be
// recovered.
// TODO(clean-room): transcribed partially -- the panel's parent links are not
// modeled.
// Symbol: ?GetParentWnd@CMFCRibbonPanel@@MEBAPEAVCWnd@@XZ
extern "C" void* MS_ABI impl__GetParentWnd_CMFCRibbonPanel__MEBAPEAVCWnd__XZ() {
    return nullptr;
}

// CMFCRibbonPanel::GetPressed() const -- retail (RVA 0x11c670) returns the
// first element whose GetPressed (vtable +0x390) is non-null, else NULL.
// OpenMFC models no element pressed state (impl__GetPressed_CMFCRibbonBaseElement
// always returns nullptr), so no pressed element can be reported.
// TODO(clean-room): transcribed partially -- element pressed state is not
// modeled.
// Symbol: ?GetPressed@CMFCRibbonPanel@@QEBAPEAVCMFCRibbonBaseElement@@XZ
extern "C" void* MS_ABI impl__GetPressed_CMFCRibbonPanel__QEBAPEAVCMFCRibbonBaseElement__XZ() {
    return nullptr;
}

// CMFCRibbonPanel::GetVisibleElements(CArray<CMFCRibbonBaseElement*,
// CMFCRibbonBaseElement*>&) -- retail (RVA 0x11dd10):
//     for (i = 0; i < m_arElements.GetSize(); i++)
//         m_arElements[i]->GetVisibleElements(ar);         // vtable +0x3f0
//     <default button at +0x3f0>->GetVisibleElements(ar);
//     <group at +0x170>->GetVisibleElements(ar);           // tail call
// The base element implementation (RVA 0x11b90) appends `this` only when its
// rect at +0xc8 is not empty, which is the whole point of the call: it is a
// visibility filter, not a copy of the element list.  OpenMFC models no
// element rects, so appending anything here would report elements as visible
// that retail would not; the array is deliberately left untouched.
// TODO(clean-room): transcribed partially -- element rects are not modeled,
// so no element can be classified as visible.
// Symbol: ?GetVisibleElements@CMFCRibbonPanel@@QEAAXAEAV?$CArray@PEAVCMFCRibbonBaseElement@@PEAV1@@@@Z
extern "C" void MS_ABI impl__GetVisibleElements_CMFCRibbonPanel__QEAAXAEAV__CArray_PEAVCMFCRibbonBaseElement__PEAV1____Z(void* /*class*/* p0, void* /*class*/* p1) {}

// CMFCRibbonPanel::HasElement(const CMFCRibbonBaseElement*) const -- retail
// (RVA 0x11c3c0), transcribed:
//     for (i = 0; i < m_arElements.GetSize(); i++)
//         if (m_arElements[i]->Find(pElement) != NULL) return TRUE;   // vtable +0x348
//     return FALSE;
// The base element's Find (RVA 0x102a0) is `return this == pElement ? this
// : NULL`, so for plain elements this is a membership test over the element
// list, which is what the loop below does.  Container elements (e.g.
// CMFCRibbonButton with sub-items) override Find to search their children;
// OpenMFC models no such children, so a nested element is not found.
// TODO(clean-room): partially transcribed -- element sub-item lists are not
// modeled, so only direct members of the panel are found.
// Symbol: ?HasElement@CMFCRibbonPanel@@QEBAHPEBVCMFCRibbonBaseElement@@@Z
extern "C" int MS_ABI impl__HasElement_CMFCRibbonPanel__QEBAHPEBVCMFCRibbonBaseElement___Z(
    const CMFCRibbonPanel* pThis, const CMFCRibbonBaseElement* pElement)
{
    if (!pThis) return 0;

    std::lock_guard<std::mutex> lock(RibbonMutex());
    auto it = RibbonPanelStates().find(pThis);
    if (it == RibbonPanelStates().end()) return 0;

    for (CMFCRibbonBaseElement* pElem : it->second.elements) {
        if (pElem && pElem == pElement) {
            return 1;
        }
    }
    return 0;
}

// CMFCRibbonPanel::Highlight(BOOL, CPoint) -- retail (RVA 0x11b2f0) stores
// the new highlight flag at +0xa8, clears the "pressed" flag at +0xe0 when
// bHighlight is FALSE, runs HitTest (vtable +0x180) to find the element under
// the point, and updates m_pHighlighted (+0x3e8) from the result.  None of
// that state is declared by the clean-room CMFCRibbonPanel.
// TODO(clean-room): transcribed partially -- panel highlight/press state and
// element geometry are not modeled.
// Symbol: ?Highlight@CMFCRibbonPanel@@MEAAXHVCPoint@@@Z
extern "C" void MS_ABI impl__Highlight_CMFCRibbonPanel__MEAAXHVCPoint___Z(int p0, void* /*class*/ p1) {}

// CMFCRibbonPanel::HitTest(CPoint, BOOL) -- retail (RVA 0x11b5b0) tests, in
// order, the default panel button's rect (+0x3f0 +0xc8), the rect at +0x238
// (returning the buttons group at +0x170), and then each element's own rect
// (+0xc8), returning the first element hit.  Every one of those rects lives
// past the end of the clean-room CMFCRibbonPanel.
// TODO(clean-room): transcribed partially -- panel and element rects are not
// modeled.
// Symbol: ?HitTest@CMFCRibbonPanel@@UEAAPEAVCMFCRibbonBaseElement@@VCPoint@@H@Z
extern "C" void* MS_ABI impl__HitTest_CMFCRibbonPanel__UEAAPEAVCMFCRibbonBaseElement__VCPoint__H_Z(void* /*class*/ p0, int p1) {
    return nullptr;
}

// CMFCRibbonPanel::HitTestEx(CPoint) const -- retail (RVA 0x11b6e0):
//     for (i = 0; i < m_arElements.GetSize(); i++)
//         if (!m_arElements[i]->m_rect.IsRectEmpty() &&
//              m_arElements[i]->m_rect.PtInRect(point)) return i;
//     return -1;
// (verified: the fall-through at 0x18011b74a is `or $0xffffffff,%eax`, so the
// "nothing hit" answer really is -1, and the hit answer is the loop index.)
// The element rect at +0xc8 is not modeled, so no element can be hit; -1 is
// what retail returns when every rect is empty, and it is what this returns.
// Returning 0 here would claim that element 0 was hit.
// TODO(clean-room): transcribed partially -- element rects are not modeled.
// Symbol: ?HitTestEx@CMFCRibbonPanel@@UEBAHVCPoint@@@Z
extern "C" int MS_ABI impl__HitTestEx_CMFCRibbonPanel__UEBAHVCPoint___Z(void* /*class*/ p0) {
    return -1;
}

// CMFCRibbonPanel::Insert(CMFCRibbonBaseElement*, int) -- retail (RVA
// 0x1173d0), transcribed:
//     if (nIndex == -1) nIndex = m_arElements.GetSize();
//     if (nIndex < 0 || nIndex > m_arElements.GetSize()) return FALSE;
//     pElem->SetParentCategory(<pointer at +0x158>);        // vtable +0x2d8
//     if (!pElem-><vtable +0x338>() && <flag at +0xb0>)     // cached panel flag
//         for (i ...) if (!m_arElements[i]-><vtable +0x338>()) { <+0xb0> = 0; break; }
//     if (nIndex == m_arElements.GetSize()) m_arElements.Add(pElem);
//     else                                  m_arElements.InsertAt(nIndex, pElem);
//     return TRUE;
// The bounds contract (including nIndex == -1 meaning "append") and the
// insertion are transcribed exactly.  The parent-category link (+0x158) and
// the cached +0xb0 flag are not declared by the clean-room CMFCRibbonPanel,
// so neither is updated.
// Ownership: the element joins the panel's owned set, exactly as
// CMFCRibbonPanel::Add above -- retail's panel destructor likewise deletes
// every element held in m_arElements.
// TODO(clean-room): partially transcribed -- the parent-category link and the
// cached +0xb0 flag are not modeled.
// Symbol: ?Insert@CMFCRibbonPanel@@UEAAHPEAVCMFCRibbonBaseElement@@H@Z
extern "C" int MS_ABI impl__Insert_CMFCRibbonPanel__UEAAHPEAVCMFCRibbonBaseElement__H_Z(
    CMFCRibbonPanel* pThis, CMFCRibbonBaseElement* pElem, int nIndex)
{
    if (!pThis) return 0;

    std::lock_guard<std::mutex> lock(RibbonMutex());
    auto& state = RibbonPanelStates()[pThis];
    const int nCount = static_cast<int>(state.elements.size());

    if (nIndex == -1) nIndex = nCount;
    if (nIndex < 0 || nIndex > nCount) return 0;

    state.elements.insert(state.elements.begin() + nIndex, pElem);
    if (pElem) state.ownedElements.insert(pElem);
    return 1;
}

// CMFCRibbonPanel::InsertSeparator(int) -- retail (RVA 0x1174f0),
// transcribed:
//     if (nIndex < 0 || nIndex > m_arElements.GetSize()) return FALSE;
//     pSep = new CMFCRibbonSeparator(FALSE);           // 0x188 bytes, ctor 0x18000f710
//     pSep-><+0xe0> = <pointer at +0x158>;             // parent category link
//     if (nIndex == m_arElements.GetSize()) m_arElements.Add(pSep);
//     else                                  m_arElements.InsertAt(nIndex, pSep);
//     return TRUE;
// Note that, unlike Insert above, nIndex == -1 is NOT special-cased here: it
// fails the `nIndex < 0` guard and returns FALSE.  That is transcribed as-is.
// Deviation: OpenMFC declares no CMFCRibbonSeparator, and the ribbon_state
// side table represents a separator as a null entry (see
// CMFCRibbonPanel::AddSeparator above), so a null entry is inserted instead
// of a separator object and the parent-category link is not set.
// TODO(clean-room): partially transcribed -- CMFCRibbonSeparator and the
// parent-category link are not modeled.
// Symbol: ?InsertSeparator@CMFCRibbonPanel@@UEAAHH@Z
extern "C" int MS_ABI impl__InsertSeparator_CMFCRibbonPanel__UEAAHH_Z(
    CMFCRibbonPanel* pThis, int nIndex)
{
    if (!pThis) return 0;

    std::lock_guard<std::mutex> lock(RibbonMutex());
    auto& state = RibbonPanelStates()[pThis];
    const int nCount = static_cast<int>(state.elements.size());

    if (nIndex < 0 || nIndex > nCount) return 0;

    state.elements.insert(state.elements.begin() + nIndex, nullptr);
    return 1;
}

// CMFCRibbonPanel::IsCollapsed() const -- retail (RVA 0x11be20) copies the
// CRect at +0x4b8, calls CRect::IsRectEmpty on the copy and returns the
// negation: the panel is collapsed exactly when that rect is NOT empty.  The
// rect is not declared by the clean-room CMFCRibbonPanel; there is no
// collapse state to report.
// TODO(clean-room): transcribed partially -- the +0x4b8 collapsed rect is not
// modeled.
// Symbol: ?IsCollapsed@CMFCRibbonPanel@@QEBAHXZ
extern "C" int MS_ABI impl__IsCollapsed_CMFCRibbonPanel__QEBAHXZ() {
    return 0;
}

// CMFCRibbonPanel::IsWindows7Look() const -- retail (RVA 0x11dc20), in full:
//     if (<pointer at +0x158> == NULL) return FALSE;             // parent category
//     if (<+0x158>-><pointer at +0x760> == NULL) return FALSE;   // parent ribbon bar
//     return <ribbon bar>-><int at +0x20c8>;
// Neither the +0x158 parent-category link nor the ribbon bar's +0x20c8 look
// flag is declared by the clean-room headers, so FALSE (the retail
// no-parent result) is all that can be reported.
// TODO(clean-room): transcribed partially -- the panel -> category -> bar
// links and the bar's look flag are not modeled.
// Symbol: ?IsWindows7Look@CMFCRibbonPanel@@QEBAHXZ
extern "C" int MS_ABI impl__IsWindows7Look_CMFCRibbonPanel__QEBAHXZ() {
    return 0;
}

// CMFCRibbonPanel::JustifyElementsInColumn(int, int) -- retail (RVA
// 0x11ae60) returns immediately when nFirst > nLast or either index is
// negative; otherwise it takes the widest element rect (+0xd0 minus +0xc8)
// over [nFirst..nLast] and stretches every element in that range to it.
// Element rects are not modeled.
// TODO(clean-room): transcribed partially -- element rects are not modeled.
// Symbol: ?JustifyElementsInColumn@CMFCRibbonPanel@@IEAAXHH@Z
extern "C" void MS_ABI impl__JustifyElementsInColumn_CMFCRibbonPanel__IEAAXHH_Z(int p0, int p1) {}

// CMFCRibbonPanel::MakeGalleryItemVisible(CMFCRibbonBaseElement*) -- retail
// (RVA 0x11ded0) returns unless the element passes CObject::IsKindOf against
// the runtime class at 0x180307218; then it compares the element's rect edges
// (+0xcc / +0xd4) with the gallery rect from GetGalleryRect (0x11dfe0),
// returns when the item already fits, and otherwise scrolls the palette by
// the difference (0x11d5a0) and hands the result to the parent window
// obtained from the vtable +0x158 GetParentWnd.  Element rects, the gallery
// rect and the palette scroll state are all unmodeled.
// TODO(clean-room): transcribed partially -- element rects, the gallery rect
// and the palette scroll state are not modeled.
// Symbol: ?MakeGalleryItemVisible@CMFCRibbonPanel@@QEAAXPEAVCMFCRibbonBaseElement@@@Z
extern "C" void MS_ABI impl__MakeGalleryItemVisible_CMFCRibbonPanel__QEAAXPEAVCMFCRibbonBaseElement___Z(void* /*class*/* p0) {}

// CMFCRibbonPanel::MouseButtonDown(CPoint) -- retail (RVA 0x11b7b0) sets the
// "pressed" flag at +0xe0, then works entirely through m_pHighlighted
// (+0x3e8): it queries the element's vtable +0x280 predicate, may mark it
// pressed (+0x144), redraws it through RedrawElement (vtable +0x220) and
// calls its OnLButtonDown (vtable +0x430), returning m_pHighlighted.  The
// highlighted-element pointer and the press state are not modeled.
// TODO(clean-room): transcribed partially -- the +0x3e8 highlighted element
// and the press state are not modeled.
// Symbol: ?MouseButtonDown@CMFCRibbonPanel@@MEAAPEAVCMFCRibbonBaseElement@@VCPoint@@@Z
extern "C" void* MS_ABI impl__MouseButtonDown_CMFCRibbonPanel__MEAAPEAVCMFCRibbonBaseElement__VCPoint___Z(void* /*class*/ p0) {
    return nullptr;
}

// CMFCRibbonPanel::MouseButtonUp(CPoint) -- retail (RVA 0x11b8e0) clears the
// "pressed" flag at +0xe0 and returns at once when m_pHighlighted (+0x3e8) is
// NULL; otherwise it calls that element's OnLButtonUp (vtable +0x438), clears
// its pressed flag (+0x144) and redraws it (vtable +0x220).  Neither the
// highlighted-element pointer nor the press state is modeled, so the early
// return is the only reachable path.
// TODO(clean-room): transcribed partially -- the +0x3e8 highlighted element
// and the press state are not modeled.
// Symbol: ?MouseButtonUp@CMFCRibbonPanel@@MEAAXVCPoint@@@Z
extern "C" void MS_ABI impl__MouseButtonUp_CMFCRibbonPanel__MEAAXVCPoint___Z(void* /*class*/ p0) {}

// CMFCRibbonPanel::NotifyControlCommand(BOOL, int, WPARAM, LPARAM) -- retail
// (RVA 0x11bb60):
//     for (i = 0; i < m_arElements.GetSize(); i++)
//         if (m_arElements[i]->NotifyControlCommand(...)) return TRUE; // vtable +0x468
//     return FALSE;
// The base element's NotifyControlCommand (vtable +0x468 -> RVA 0x7260,
// `xor eax,eax; ret`) always returns FALSE; only control-hosting elements
// override it, and OpenMFC's clean-room CMFCRibbonBaseElement declares no
// such virtual and no derived element that could override it.  FALSE is
// therefore the retail result for the modeled element set.
// TODO(clean-room): transcribed partially -- element control hosting is not
// modeled.
// Symbol: ?NotifyControlCommand@CMFCRibbonPanel@@MEAAHHH_K_J@Z
extern "C" int MS_ABI impl__NotifyControlCommand_CMFCRibbonPanel__MEAAHHH_K_J_Z(int p0, int p1, unsigned __int64 p2, __int64 p3) {
    return 0;
}

// CMFCRibbonPanel::OnAfterChangeRect(CDC*) -- retail (RVA 0x11bc00) walks
// m_arElements and, for each element, copies its rect (+0xc8), calls the
// element's OnShow (vtable +0x260) with "the rect is not empty" and then its
// OnAfterChangeRect (vtable +0x258) with pDC; it then repeats both for the
// panel rect at +0x4b8 and the default button at +0x3f0.  Element rects are
// not modeled.  Of the two element slots only OnShow is empty in the base
// element vtable (RVA 0x2da510 +0x260 -> RVA 0x2820, a bare `ret`); its
// +0x258 slot is a real body (RVA 0x10ac0, shared with -- and labelled in the
// symbol map as -- CMFCRibbonLabel::OnAfterChangeRect) that reads the
// element's text CString at +0xb8 and calls element vtable +0x368 when that
// string is empty.  Neither the +0xb8 string nor that slot is modeled either.
// TODO(clean-room): transcribed partially -- element rects are not modeled.
// Symbol: ?OnAfterChangeRect@CMFCRibbonPanel@@MEAAXPEAVCDC@@@Z
extern "C" void MS_ABI impl__OnAfterChangeRect_CMFCRibbonPanel__MEAAXPEAVCDC___Z(void* /*class*/* p0) {}

// CMFCRibbonPanel::OnDrawPaletteMenu(CDC*) -- retail (RVA 0x117df0) starts by
// testing the +0x168 parent pointer with CObject::IsKindOf against the
// runtime class at 0x18030b670, and the ~0x400 bytes that follow draw the
// palette decorations against that parent and the panel's layout rects.
// Neither the +0x168 link nor any panel drawing state is modeled.
// TODO(clean-room): transcribed partially -- panel palette drawing is not
// modeled.
// Symbol: ?OnDrawPaletteMenu@CMFCRibbonPanel@@IEAAXPEAVCDC@@@Z
extern "C" void MS_ABI impl__OnDrawPaletteMenu_CMFCRibbonPanel__IEAAXPEAVCDC___Z(void* /*class*/* p0) {}

// CMFCRibbonPanel::OnKey(UINT) -- retail (RVA 0x11c790) returns FALSE at once
// when m_arElements is empty; the ~0xaa0 bytes that follow drive keyboard
// navigation starting from m_pHighlighted (+0x3e8) across the element list.
// The highlighted-element pointer and the element geometry the navigation
// depends on are not declared by the clean-room CMFCRibbonPanel, so only the
// empty-panel early-out could be honoured and it says nothing about a
// populated panel.
// TODO(clean-room): transcribed partially -- panel focus/highlight state and
// element geometry are not modeled.
// Symbol: ?OnKey@CMFCRibbonPanel@@UEAAHI@Z
extern "C" int MS_ABI impl__OnKey_CMFCRibbonPanel__UEAAHI_Z(unsigned int p0) {
    return 0;
}

// CMFCRibbonPanel::OnRTLChanged(BOOL) -- retail (RVA 0x11d980):
//     for (i ...) m_arElements[i]->OnRTLChanged(bIsRTL);   // vtable +0x3f8
//     <default button at +0x3f0>->OnRTLChanged(bIsRTL);
//     <group at +0x170>->OnRTLChanged(bIsRTL);             // tail call
// The base element's OnRTLChanged (vtable +0x3f8 -> RVA 0x2820, a bare `ret`)
// does nothing; only elements with mirrored images override it, and OpenMFC
// declares no such element.  The embedded default button and buttons group
// are not modeled either, so the whole call is a no-op for the modeled set.
// TODO(clean-room): transcribed partially -- element image mirroring and the
// embedded objects are not modeled.
// Symbol: ?OnRTLChanged@CMFCRibbonPanel@@MEAAXH@Z
extern "C" void MS_ABI impl__OnRTLChanged_CMFCRibbonPanel__MEAAXH_Z(int p0) {}

// CMFCRibbonPanel::OnSetAccData(long) -- retail (RVA 0x11e040) resets the
// CAccessibilityData block embedded at +0x40, fills a local array through
// CMFCRibbonPanel::GetVisibleElements (0x11dd10) and forwards to element
// lVal - 1, or describes the panel itself for lVal == 0.  Both the +0x40
// accessibility block and the element rects that decide visibility are
// unmodeled.
// TODO(clean-room): transcribed partially -- the embedded accessibility data
// block and element rects are not modeled.
// Symbol: ?OnSetAccData@CMFCRibbonPanel@@MEAAHJ@Z
extern "C" int MS_ABI impl__OnSetAccData_CMFCRibbonPanel__MEAAHJ_Z(long p0) {
    return 0;
}

// CMFCRibbonPanel::OnShow(BOOL) -- retail (RVA 0x11bd50):
//     for (i ...) m_arElements[i]->OnShow(bShow && !m_arElements[i]->m_rect.IsRectEmpty());
// (element vtable +0x260; the flag passed is bShow ANDed with "this element
// has a non-empty rect at +0xc8").  The base element's OnShow (RVA 0x2820) is
// a bare `ret`, and element rects are not modeled, so nothing observable
// happens for the modeled element set.
// TODO(clean-room): transcribed partially -- element rects and element show
// state are not modeled.
// Symbol: ?OnShow@CMFCRibbonPanel@@MEAAXH@Z
extern "C" void MS_ABI impl__OnShow_CMFCRibbonPanel__MEAAXH_Z(int p0) {}

// CMFCRibbonPanel::OnUpdateCmdUI(CMFCRibbonCmdUI*, CFrameWnd*, BOOL) --
// retail (RVA 0x11bac0):
//     for (i ...) m_arElements[i]->OnUpdateCmdUI(pCmdUI, pTarget, bDisableIfNoHndler);
//     <group at +0x170>->OnUpdateCmdUI(...);          // element vtable +0x460
// The base element's OnUpdateCmdUI (RVA 0x10120) routes the command through
// the target's handler and then applies the result to the element's enabled /
// checked / radio state -- members the clean-room CMFCRibbonBaseElement does
// not declare.  Its OpenMFC counterpart
// (impl__OnUpdateCmdUI_CMFCRibbonBaseElement, in
// featurepack/ribbon/CMFCRibbonBaseElement.cpp) is still a generated no-op
// that does not even take a `this` parameter, so forwarding to it would
// accomplish nothing.
// TODO(clean-room): transcribed partially -- element command-UI state is not
// modeled.
// Symbol: ?OnUpdateCmdUI@CMFCRibbonPanel@@MEAAXPEAVCMFCRibbonCmdUI@@PEAVCFrameWnd@@H@Z
extern "C" void MS_ABI impl__OnUpdateCmdUI_CMFCRibbonPanel__MEAAXPEAVCMFCRibbonCmdUI__PEAVCFrameWnd__H_Z(void* /*class*/* p0, void* /*class*/* p1, int p2) {}

// CMFCRibbonPanel::RecalcWidths(CDC*, int) -- retail (RVA 0x11af00) drives
// the panel's whole width-fitting pass: it walks m_arElements calling their
// size/layout virtuals and records the resulting per-state widths on the
// panel.  Those cached widths all live past the end of the clean-room
// CMFCRibbonPanel, which declares no layout members at all.
// TODO(clean-room): transcribed partially -- panel layout geometry is not
// modeled.
// Symbol: ?RecalcWidths@CMFCRibbonPanel@@UEAAXPEAVCDC@@H@Z
extern "C" void MS_ABI impl__RecalcWidths_CMFCRibbonPanel__UEAAXPEAVCDC__H_Z(void* /*class*/* p0, int p1) {}

// CMFCRibbonPanel::RedrawElement(CMFCRibbonBaseElement*) -- retail (RVA
// 0x11c330), in full:
//     CRect rect = pElem->m_rect;                 // element +0xc8
//     if (rect.IsRectEmpty()) return;
//     CWnd* pWnd = GetParentWnd();                // panel vtable +0x158
//     if (pWnd == NULL || pWnd->m_hWnd == NULL) return;
//     ::InvalidateRect(pWnd->m_hWnd, &rect, TRUE);
//     ::UpdateWindow(pWnd->m_hWnd);
// The element rect and the panel's parent-window links are both unmodeled, so
// the retail early-out (empty rect / no parent window) is the only reachable
// path and no repaint can be issued.
// TODO(clean-room): transcribed partially -- element rects and the panel's
// parent-window link are not modeled.
// Symbol: ?RedrawElement@CMFCRibbonPanel@@MEAAXPEAVCMFCRibbonBaseElement@@@Z
extern "C" void MS_ABI impl__RedrawElement_CMFCRibbonPanel__MEAAXPEAVCMFCRibbonBaseElement___Z(void* /*class*/* p0) {}

// CMFCRibbonPanel::Remove(int, BOOL) -- retail (RVA 0x117890), transcribed:
//     if (nIndex < 0 || nIndex >= m_arElements.GetSize()) return FALSE;
//     pElem = m_arElements[nIndex];
//     if (pElem == <m_pHighlighted at +0x3e8>) <+0x3e8> = NULL;
//     m_arElements.RemoveAt(nIndex);
//     if (bDelete && pElem != NULL) delete pElem;      // vtable +0x8, flag 1
//     if (<flag at +0xb0> == 0) { ...recount via element vtable +0x338...; }
//     return TRUE;
// The bounds contract, the removal and the conditional delete are transcribed
// exactly.  The highlighted-element pointer (+0x3e8) and the cached +0xb0
// flag are not declared by the clean-room CMFCRibbonPanel.
// Ownership: the element is dropped from the panel's owned set in both cases,
// which matches retail -- after RemoveAt the element is no longer in the
// array the panel destructor deletes, so a bDelete == FALSE removal hands
// ownership back to the caller.
// TODO(clean-room): partially transcribed -- the +0x3e8 highlighted element
// and the cached +0xb0 flag are not modeled.
// Symbol: ?Remove@CMFCRibbonPanel@@QEAAHHH@Z
extern "C" int MS_ABI impl__Remove_CMFCRibbonPanel__QEAAHHH_Z(
    CMFCRibbonPanel* pThis, int nIndex, int bDelete)
{
    if (!pThis) return 0;

    CMFCRibbonBaseElement* pElem = nullptr;
    {
        std::lock_guard<std::mutex> lock(RibbonMutex());
        auto it = RibbonPanelStates().find(pThis);
        if (it == RibbonPanelStates().end()) return 0;

        auto& elements = it->second.elements;
        if (nIndex < 0 || nIndex >= static_cast<int>(elements.size())) return 0;

        pElem = elements[static_cast<size_t>(nIndex)];
        elements.erase(elements.begin() + nIndex);
        if (pElem) it->second.ownedElements.erase(pElem);
    }

    if (bDelete && pElem) {
        delete pElem;
    }
    return 1;
}

// Symbol: ?RemoveAll@CMFCRibbonPanel@@QEAAXXZ
extern "C" void MS_ABI impl__RemoveAll_CMFCRibbonPanel__QEAAXXZ() {}

// Symbol: ?Replace@CMFCRibbonPanel@@QEAAHHPEAVCMFCRibbonBaseElement@@@Z
extern "C" int MS_ABI impl__Replace_CMFCRibbonPanel__QEAAHHPEAVCMFCRibbonBaseElement___Z(int p0, void* /*class*/* p1) {
    return 0;
}

// Symbol: ?ReplaceByID@CMFCRibbonPanel@@QEAAHIPEAVCMFCRibbonBaseElement@@@Z
extern "C" int MS_ABI impl__ReplaceByID_CMFCRibbonPanel__QEAAHIPEAVCMFCRibbonBaseElement___Z(unsigned int p0, void* /*class*/* p1) {
    return 0;
}

// Symbol: ?Reposition@CMFCRibbonPanel@@MEAAXPEAVCDC@@AEBVCRect@@@Z
extern "C" void MS_ABI impl__Reposition_CMFCRibbonPanel__MEAAXPEAVCDC__AEBVCRect___Z(void* /*class*/* p0, const void* /*class*/* p1) {}

// Symbol: ?RepositionMenu@CMFCRibbonPanel@@MEAAXPEAVCDC@@AEBVCRect@@@Z
extern "C" void MS_ABI impl__RepositionMenu_CMFCRibbonPanel__MEAAXPEAVCDC__AEBVCRect___Z(void* /*class*/* p0, const void* /*class*/* p1) {}

// Symbol: ?ReposPalette@CMFCRibbonPanel@@MEAAXPEAVCDC@@AEBVCRect@@@Z
extern "C" void MS_ABI impl__ReposPalette_CMFCRibbonPanel__MEAAXPEAVCDC__AEBVCRect___Z(void* /*class*/* p0, const void* /*class*/* p1) {}

// Symbol: ?ScrollPalette@CMFCRibbonPanel@@IEAAXHH@Z
extern "C" void MS_ABI impl__ScrollPalette_CMFCRibbonPanel__IEAAXHH_Z(int p0, int p1) {}

// Symbol: ?SetACCData@CMFCRibbonPanel@@MEAAHPEAVCWnd@@AEAVCAccessibilityData@@@Z
extern "C" int MS_ABI impl__SetACCData_CMFCRibbonPanel__MEAAHPEAVCWnd__AEAVCAccessibilityData___Z(void* /*class*/* p0, void* /*class*/* p1) {
    return 0;
}

// Symbol: ?SetElementMenu@CMFCRibbonPanel@@QEAAHIIHH@Z
extern "C" int MS_ABI impl__SetElementMenu_CMFCRibbonPanel__QEAAHIIHH_Z(unsigned int p0, unsigned int p1, int p2, int p3) {
    return 0;
}

// Symbol: ?SetElementMenu@CMFCRibbonPanel@@QEAAHIPEAUHMENU__@@HH@Z
extern "C" int MS_ABI impl__SetElementMenu_CMFCRibbonPanel__QEAAHIPEAUHMENU____HH_Z(unsigned int p0, void* /*struct*/* p1, int p2, int p3) {
    return 0;
}

// Symbol: ?SetElementRTC@CMFCRibbonPanel@@QEAAPEAVCMFCRibbonBaseElement@@HPEAUCRuntimeClass@@@Z
extern "C" void* MS_ABI impl__SetElementRTC_CMFCRibbonPanel__QEAAPEAVCMFCRibbonBaseElement__HPEAUCRuntimeClass___Z(int p0, void* /*struct*/* p1) {
    return nullptr;
}

// Symbol: ?SetElementRTCByID@CMFCRibbonPanel@@QEAAPEAVCMFCRibbonBaseElement@@IPEAUCRuntimeClass@@@Z
extern "C" void* MS_ABI impl__SetElementRTCByID_CMFCRibbonPanel__QEAAPEAVCMFCRibbonBaseElement__IPEAUCRuntimeClass___Z(unsigned int p0, void* /*struct*/* p1) {
    return nullptr;
}

// Symbol: ?SetFocused@CMFCRibbonPanel@@QEAAXPEAVCMFCRibbonBaseElement@@@Z
extern "C" void MS_ABI impl__SetFocused_CMFCRibbonPanel__QEAAXPEAVCMFCRibbonBaseElement___Z(void* /*class*/* p0) {}

// Symbol: ?SetKeys@CMFCRibbonPanel@@QEAAXPEB_W@Z
extern "C" void MS_ABI impl__SetKeys_CMFCRibbonPanel__QEAAXPEB_W_Z(const wchar_t* p0) {}

// Symbol: ?ShowDefaultButton@CMFCRibbonPanel@@IEAAXPEAVCDC@@@Z
extern "C" void MS_ABI impl__ShowDefaultButton_CMFCRibbonPanel__IEAAXPEAVCDC___Z(void* /*class*/* p0) {}

// Symbol: ?ShowPopup@CMFCRibbonPanel@@QEAAPEAVCMFCRibbonPanelMenu@@PEAVCMFCRibbonDefaultPanelButton@@@Z
extern "C" void* MS_ABI impl__ShowPopup_CMFCRibbonPanel__QEAAPEAVCMFCRibbonPanelMenu__PEAVCMFCRibbonDefaultPanelButton___Z(void* /*class*/* p0) {
    return nullptr;
}
