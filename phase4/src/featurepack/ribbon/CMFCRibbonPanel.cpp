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
// CMFCRibbonCmdUI is not declared by the clean-room headers; only a pointer
// to it crosses the OnUpdateCmdUI signature below.
class CMFCRibbonCmdUI;
// CMFCRibbonBaseElement::CopyFrom (element vtable +0x2e0); the definition in
// featurepack/ribbon/CMFCRibbonBaseElement.cpp copies the modeled command ID.
extern "C" void MS_ABI impl__CopyFrom_CMFCRibbonBaseElement__UEAAXAEBV1__Z(
    CMFCRibbonBaseElement* pThis, const CMFCRibbonBaseElement& src);
// CMFCRibbonButton::SetMenu overloads -- defined in
// featurepack/ribbon/CMFCRibbonButton.cpp (typed, currently no-op bodies).
extern "C" void MS_ABI impl__SetMenu_CMFCRibbonButton__QEAAXIHH_Z(
    CMFCRibbonButton* pThis, unsigned int uiMenuResID, int bRightAlign, int bTopAlign);
extern "C" void MS_ABI impl__SetMenu_CMFCRibbonButton__QEAAXPEAUHMENU____HH_Z(
    CMFCRibbonButton* pThis, HMENU hMenu, int bRightAlign, int bTopAlign);

namespace {

// Retail links a panel to its category through m_pParent (+0x158, stored by
// CMFCRibbonCategory::AddPanel at 0x1800ee4db in mfc140) and a category to
// its bar through m_pParentRibbonBar (+0x760, stored by
// CMFCRibbonCategory::CommonInit).  The clean-room objects declare neither
// pointer; OpenMFC records the same two relationships in the ribbon_state
// side tables (RibbonCategoryStates()[category].panels and
// RibbonBarStates()[bar].categories), so the links are recovered by a reverse
// lookup.  Both helpers must be called with RibbonMutex() held.
const CMFCRibbonCategory* PanelParentCategory(const CMFCRibbonPanel* pPanel)
{
    for (const auto& entry : openmfc::ribbon_state::RibbonCategoryStates()) {
        const auto& panels = entry.second.panels;
        if (std::find(panels.begin(), panels.end(), pPanel) != panels.end()) {
            return entry.first;
        }
    }
    return nullptr;
}

const CMFCRibbonBar* CategoryParentBar(const CMFCRibbonCategory* pCategory)
{
    if (pCategory == nullptr) return nullptr;
    for (const auto& entry : openmfc::ribbon_state::RibbonBarStates()) {
        const auto& categories = entry.second.categories;
        if (std::find(categories.begin(), categories.end(), pCategory) != categories.end()) {
            return entry.first;
        }
    }
    return nullptr;
}

// OpenMFC currently carries TWO CRuntimeClass descriptors for every ribbon
// class: the file-scope globals in featurepack/ribbon/RuntimeClasses.cpp
// (what the exported GetThisClass thunks return, and therefore what an MSVC
// client's RUNTIME_CLASS() chain reaches) and the static members defined by
// IMPLEMENT_DYNAMIC in detail/MfccoreSupport.cpp (what the vtable of an
// object constructed inside this DLL returns from GetRuntimeClass).  Checked
// with nm on build-phase4/obj: `D classCMFCRibbonButton` in RuntimeClasses.o
// versus `D _ZN16CMFCRibbonButton21classCMFCRibbonButtonE` in
// MfccoreSupport.o.  Retail's IsKindOf / IsDerivedFrom compare descriptor
// pointers; across those two worlds a pointer compare never matches, so the
// class tests in this file walk the m_pfnGetBaseClass chain and compare
// m_lpszClassName instead.  Class names are unique, so the answer is the one
// retail's pointer compare gives.  This is a deviation forced by the duplicate
// descriptors; drop it once a single descriptor per class exists.
bool RuntimeClassDerivesFrom(const CRuntimeClass* pClass, const char* szName)
{
    while (pClass != nullptr) {
        if (pClass->m_lpszClassName != nullptr &&
            std::strcmp(pClass->m_lpszClassName, szName) == 0) {
            return true;
        }
        pClass = pClass->m_pfnGetBaseClass ? pClass->m_pfnGetBaseClass() : nullptr;
    }
    return false;
}

// vtable slot 0 is GetRuntimeClass for MSVC-built and OpenMFC-built objects
// alike (see the header comment of core/runtime/CObject.cpp).
bool ObjectIsKindOf(const CObject* pObject, const char* szName)
{
    if (pObject == nullptr) return false;
    return RuntimeClassDerivesFrom(pObject->GetRuntimeClass(), szName);
}

} // namespace

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

constexpr unsigned short kVarTypeEmpty = 0;   // VT_EMPTY
constexpr unsigned short kVarTypeI4 = 3;      // VT_I4
constexpr long kHrOk = 0L;                    // S_OK
constexpr long kHrFalse = 1L;                 // S_FALSE
constexpr long kHrInvalidArg = (long)0x80070057L; // E_INVALIDARG

// NAVDIR_* constants from oleacc.h, as retail's accNavigate dispatches on
// them (the `sub $3 / sub $1 ...` chain starting at the instruction
// 0x18011d6ab inside accNavigate, entry RVA 0x11d650 in mfc140u).
constexpr long kNavDirLeft = 3;
constexpr long kNavDirRight = 4;
constexpr long kNavDirNext = 5;
constexpr long kNavDirPrevious = 6;
constexpr long kNavDirFirstChild = 7;
constexpr long kNavDirLastChild = 8;

} // namespace

// This file's own exported GetParentWnd (defined further down); retail's
// accessibility bodies reach it through the panel vtable slot +0x158.
extern "C" CWnd* MS_ABI impl__GetParentWnd_CMFCRibbonPanel__MEBAPEAVCWnd__XZ(
    const CMFCRibbonPanel* pThis);
// CWnd::ClientToScreen(LPRECT) -- defined in core/window/Thunks.cpp.
extern "C" void MS_ABI impl__ClientToScreen_CWnd__QEBAXPEAUtagRECT___Z(
    const CWnd* pThis, RECT* pRect);

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
// (RVA 0x11e280; mfc140u RVA 0x11d510), transcribed:
//     if (!pxLeft || !pyTop || !pcxWidth || !pcyHeight) return E_INVALIDARG;
//     if (varChild.vt != VT_I4) return S_OK;             // nothing written
//     if (varChild.lVal == 0) {
//         CWnd* pWnd = GetParentWnd();                    // vtable +0x158
//         if (pWnd == NULL || pWnd->m_hWnd == NULL) return S_FALSE;
//         CRect rect = m_rect;                            // +0x10c
//         pWnd->ClientToScreen(&rect);                    // 0x1802a3310 (mfc140u)
//         *pxLeft = rect.left; *pyTop = rect.top;
//         *pcxWidth = rect.Width(); *pcyHeight = rect.Height();
//         return S_OK;
//     }
//     if (varChild.lVal < 0) return S_OK;                 // nothing written
//     OnSetAccData(varChild.lVal);                        // vtable +0x160
//     <write the +0x7c..+0x88 accessibility rect the same way>; return S_OK;
// The argument validation, the non-VT_I4 / negative early-outs and the
// no-window S_FALSE are reproduced exactly.  The panel rect m_rect is not
// declared by the clean-room CMFCRibbonPanel; the rect mapped for lVal == 0
// is the empty rect retail's CommonInit (mfc140u 0x116260, SetRectEmpty on
// +0x10c) leaves there for a panel that Reposition has never laid out --
// which is every OpenMFC panel, since Reposition below is a stub -- so the
// result (the parent window's client origin, width and height 0) is what
// retail reports for the modeled state.  For lVal > 0 the +0x40
// CAccessibilityData block that OnSetAccData fills is not modeled and the
// modeled visible-child set is empty (see get_accChildCount below), so the
// child rect cannot be produced; S_OK is returned without writing, which is
// retail's answer for a non-VT_I4 / negative child and the only one that
// does not hand the caller invented geometry.
// Signature: `this` in RCX, the four longs in RDX/R8/R9/stack, the VARIANT
// by pointer on the stack (retail reads its vt with `cmpw $0x3,(%rdx)` after
// loading that stack slot).
// TODO(clean-room): partially transcribed -- the panel rect and the
// accessibility data block are not modeled, so the lVal > 0 rect is not
// written.
// Symbol: ?accLocation@CMFCRibbonPanel@@MEAAJPEAJ000UtagVARIANT@@@Z
extern "C" long MS_ABI impl__accLocation_CMFCRibbonPanel__MEAAJPEAJ000UtagVARIANT___Z(
    CMFCRibbonPanel* pThis, long* pxLeft, long* pyTop, long* pcxWidth,
    long* pcyHeight, const RibbonAccVariant* pvarChild)
{
    if (!pxLeft || !pyTop || !pcxWidth || !pcyHeight) return kHrInvalidArg;
    if (pvarChild == nullptr || pvarChild->vt != kVarTypeI4) return kHrOk;
    if (pvarChild->lVal != 0) return kHrOk;

    CWnd* pWnd = impl__GetParentWnd_CMFCRibbonPanel__MEBAPEAVCWnd__XZ(pThis);
    if (pWnd == nullptr || pWnd->m_hWnd == nullptr) return kHrFalse;

    RECT rect = {0, 0, 0, 0};   // m_rect of a never-laid-out panel
    impl__ClientToScreen_CWnd__QEBAXPEAUtagRECT___Z(pWnd, &rect);
    *pxLeft = rect.left;
    *pyTop = rect.top;
    *pcxWidth = rect.right - rect.left;
    *pcyHeight = rect.bottom - rect.top;
    return kHrOk;
}

// CMFCRibbonPanel::accNavigate(long, VARIANT, VARIANT*) -- retail (RVA
// 0x11e3c0; mfc140u RVA 0x11d650), transcribed:
//     if (pvarEndUpAt == NULL) return E_INVALIDARG;
//     pvarEndUpAt->vt = VT_EMPTY;                       // `mov %ax,(%r9)`, eax 0
//     if (varStart.vt != VT_I4) return E_INVALIDARG;
//     CArray<CMFCRibbonBaseElement*, CMFCRibbonBaseElement*> ar;
//     GetVisibleElements(ar);                           // 0x18011cfa0 (mfc140u)
//     switch (navDir) {                                 // `sub $3 / sub $1 ...` chain
//     case NAVDIR_FIRSTCHILD:                           // 7
//         if (varStart.lVal != 0) return S_FALSE;
//         pvarEndUpAt->vt = VT_I4; pvarEndUpAt->lVal = 1; return S_OK;
//     case NAVDIR_LASTCHILD:                            // 8
//         if (varStart.lVal != 0) return S_FALSE;
//         pvarEndUpAt->vt = VT_I4; pvarEndUpAt->lVal = ar.GetSize(); return S_OK;
//     case NAVDIR_RIGHT: case NAVDIR_NEXT:              // 4, 5
//         if (varStart.lVal != 0) {
//             pvarEndUpAt->vt = VT_I4; pvarEndUpAt->lVal = varStart.lVal + 1;
//             if (pvarEndUpAt->lVal <= ar.GetSize()) return S_OK;
//             pvarEndUpAt->vt = VT_EMPTY; return S_FALSE;
//         }
//         <sibling panel: m_pParent (+0x158) -> GetPanelIndex(this) + 1 <
//          m_pParent->panel count (+0x7b0) ? GetPanel(i) : S_FALSE>
//     case NAVDIR_LEFT: case NAVDIR_PREVIOUS:           // 3, 6
//         if (varStart.lVal != 0) {
//             pvarEndUpAt->vt = VT_I4; pvarEndUpAt->lVal = varStart.lVal - 1;
//             if (pvarEndUpAt->lVal > 0) return S_OK;
//             pvarEndUpAt->vt = VT_EMPTY; return S_FALSE;
//         }
//         <sibling panel: GetPanelIndex(this) - 1 >= 0 ? GetPanel(i) : S_FALSE>
//     default: return S_FALSE;                          // UP, DOWN, anything else
//     }
//     // sibling panel found: pvarEndUpAt->vt = VT_DISPATCH (9);
//     // pPanel->ExternalAddRef();                       // 0x18026cd80 (mfc140u)
//     // pvarEndUpAt->pdispVal = pPanel->GetInterface(&IID_IDispatch); // 0x18026ce40,
//     // return S_OK;                                   // IID {00020400-...-46} at 0x1802d9b48
// Note that FIRSTCHILD does not test ar.GetSize(): retail answers child 1
// even for a panel with no visible element, and LASTCHILD answers 0 then.
// The validation, the VT_EMPTY pre-write, the navDir dispatch and the index
// stepping are reproduced exactly against the modeled visible-child count,
// which is 0 for every OpenMFC panel (element rects are not modeled -- the
// same count get_accChildCount below reports).
// Deviation: the lVal == 0 sibling-panel branch returns S_FALSE with
// VT_EMPTY even when a neighbouring panel exists.  Retail hands back that
// panel's IDispatch, which needs a CCmdTarget-derived panel; the clean-room
// CMFCRibbonPanel derives from CObject and has no IDispatch to return.
// Signature: `this` in RCX, navDir in EDX, varStart by pointer in R8,
// pvarEndUpAt in R9.
// TODO(clean-room): partially transcribed -- element rects (the visible
// set) and the panel's IDispatch are not modeled.
// Symbol: ?accNavigate@CMFCRibbonPanel@@MEAAJJUtagVARIANT@@PEAU2@@Z
extern "C" long MS_ABI impl__accNavigate_CMFCRibbonPanel__MEAAJJUtagVARIANT__PEAU2__Z(
    CMFCRibbonPanel* /*pThis*/, long navDir, const RibbonAccVariant* pvarStart,
    RibbonAccVariant* pvarEndUpAt)
{
    if (pvarEndUpAt == nullptr) return kHrInvalidArg;
    pvarEndUpAt->vt = kVarTypeEmpty;
    if (pvarStart == nullptr || pvarStart->vt != kVarTypeI4) return kHrInvalidArg;

    const long nVisible = 0;    // modeled visible-child set (see get_accChildCount)
    const long lStart = pvarStart->lVal;

    switch (navDir) {
    case kNavDirFirstChild:
        if (lStart != 0) return kHrFalse;
        pvarEndUpAt->vt = kVarTypeI4;
        pvarEndUpAt->lVal = 1;
        return kHrOk;
    case kNavDirLastChild:
        if (lStart != 0) return kHrFalse;
        pvarEndUpAt->vt = kVarTypeI4;
        pvarEndUpAt->lVal = nVisible;
        return kHrOk;
    case kNavDirRight:
    case kNavDirNext:
        if (lStart != 0) {
            pvarEndUpAt->vt = kVarTypeI4;
            pvarEndUpAt->lVal = lStart + 1;
            if (pvarEndUpAt->lVal <= nVisible) return kHrOk;
            pvarEndUpAt->vt = kVarTypeEmpty;
            return kHrFalse;
        }
        return kHrFalse;    // sibling panel: no IDispatch to hand back (see above)
    case kNavDirLeft:
    case kNavDirPrevious:
        if (lStart != 0) {
            pvarEndUpAt->vt = kVarTypeI4;
            pvarEndUpAt->lVal = lStart - 1;
            if (pvarEndUpAt->lVal > 0) return kHrOk;
            pvarEndUpAt->vt = kVarTypeEmpty;
            return kHrFalse;
        }
        return kHrFalse;    // sibling panel: no IDispatch to hand back (see above)
    default:
        return kHrFalse;
    }
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
// (mfc140u RVA 0x11c500; the RVA above is the ANSI twin's.)
// TODO(clean-room): transcribed partially -- element rects and the +0x168
// parent link are not modeled.
// Symbol: ?GetFirstTabStop@CMFCRibbonPanel@@IEBAPEAVCMFCRibbonBaseElement@@XZ
extern "C" CMFCRibbonBaseElement* MS_ABI impl__GetFirstTabStop_CMFCRibbonPanel__IEBAPEAVCMFCRibbonBaseElement__XZ(
    const CMFCRibbonPanel* /*pThis*/)
{
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
// (mfc140u RVA 0x11cee0; the RVA above is the ANSI twin's.)
// TODO(clean-room): transcribed partially -- element focus state is not
// modeled.
// Symbol: ?GetFocused@CMFCRibbonPanel@@QEBAPEAVCMFCRibbonBaseElement@@XZ
extern "C" CMFCRibbonBaseElement* MS_ABI impl__GetFocused_CMFCRibbonPanel__QEBAPEAVCMFCRibbonBaseElement__XZ(
    const CMFCRibbonPanel* /*pThis*/)
{
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
// (mfc140u RVA 0x117560; the RVA above is the ANSI twin's.)
// TODO(clean-room): transcribed partially -- panel layout geometry is not
// modeled.
// Symbol: ?GetHeight@CMFCRibbonPanel@@MEBAHPEAVCDC@@@Z
extern "C" int MS_ABI impl__GetHeight_CMFCRibbonPanel__MEBAHPEAVCDC___Z(
    const CMFCRibbonPanel* /*pThis*/, CDC* /*pDC*/)
{
    return 0;
}

// CMFCRibbonPanel::GetHighlighted() const -- retail (RVA 0x11c780) is a
// single load: `return m_pHighlighted;` (the pointer at +0x3e8).  That member
// is not declared by the clean-room CMFCRibbonPanel and nothing in OpenMFC
// tracks ribbon highlighting, so nullptr is the truthful result -- the same
// treatment as CMFCRibbonCategory::GetHighlighted.
// (mfc140u RVA 0x11ba00; the RVA above is the ANSI twin's.)
// TODO(clean-room): transcribed partially -- the +0x3e8 highlighted-element
// pointer is not modeled.
// Symbol: ?GetHighlighted@CMFCRibbonPanel@@QEBAPEAVCMFCRibbonBaseElement@@XZ
extern "C" CMFCRibbonBaseElement* MS_ABI impl__GetHighlighted_CMFCRibbonPanel__QEBAPEAVCMFCRibbonBaseElement__XZ(
    const CMFCRibbonPanel* /*pThis*/)
{
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
// (mfc140u RVA 0x11c670; the RVA above is the ANSI twin's.)
// TODO(clean-room): transcribed partially -- element rects and the +0x168
// parent link are not modeled.
// Symbol: ?GetLastTabStop@CMFCRibbonPanel@@IEBAPEAVCMFCRibbonBaseElement@@XZ
extern "C" CMFCRibbonBaseElement* MS_ABI impl__GetLastTabStop_CMFCRibbonPanel__IEBAPEAVCMFCRibbonBaseElement__XZ(
    const CMFCRibbonPanel* /*pThis*/)
{
    return nullptr;
}

// CMFCRibbonPanel::GetMinWidth(CDC*) const -- retail (RVA 0x11b2a0) calls
// the default panel button's (+0x3f0) vtable +0x310 slot and then its
// vtable +0x200 size getter, returning the cx of the resulting CSize.  The
// embedded default panel button is not declared by the clean-room
// CMFCRibbonPanel.
// (mfc140u RVA 0x11a520; the RVA above is the ANSI twin's.)
// TODO(clean-room): transcribed partially -- the embedded default panel
// button is not modeled.
// Symbol: ?GetMinWidth@CMFCRibbonPanel@@IEAAHPEAVCDC@@@Z
extern "C" int MS_ABI impl__GetMinWidth_CMFCRibbonPanel__IEAAHPEAVCDC___Z(
    CMFCRibbonPanel* /*pThis*/, CDC* /*pDC*/)
{
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
// (mfc140u RVA 0x11c8f0, re-read there: the USER32 import is ::GetParent on
// the +0x160 window's m_hWnd, the helper -- 0x18028ad70 in mfc140u -- is
// CWnd::FromHandle, and the result is the pointer at +0x19b8 of whatever CWnd
// that returns; which class that window is was not verified here.  The RVA
// above is the ANSI twin's.)  The +0x160 pointer is the hosting
// CMFCRibbonPanelMenuBar, a class OpenMFC does not declare, so retail's
// no-menu-bar early-out (NULL) is the only reachable path.
// TODO(clean-room): transcribed partially -- the +0x160 parent menu-bar link
// is not modeled.
// Symbol: ?GetParentButton@CMFCRibbonPanel@@QEBAPEAVCMFCRibbonBaseElement@@XZ
extern "C" CMFCRibbonBaseElement* MS_ABI impl__GetParentButton_CMFCRibbonPanel__QEBAPEAVCMFCRibbonBaseElement__XZ(
    const CMFCRibbonPanel* /*pThis*/)
{
    return nullptr;
}

// CMFCRibbonPanel::GetParentWnd() const -- retail (RVA 0x11c310; mfc140u RVA
// 0x11b590), in full:
//     if (m_pParentMenuBar != NULL) return m_pParentMenuBar;   // +0x160
//     if (m_pParent == NULL) return NULL;                      // +0x158
//     return m_pParent->m_pParentRibbonBar;                    // category +0x760
// The +0x160 menu bar is a CMFCRibbonPanelMenuBar, which OpenMFC does not
// declare and never attaches to a panel, so that branch has no modeled
// counterpart and is skipped.  The category and bar links are recovered from
// the ribbon_state side tables (see PanelParentCategory / CategoryParentBar
// above), which is where OpenMFC keeps the relationships retail stores in the
// +0x158 / +0x760 pointers.  CMFCRibbonBar derives from CWnd through CPane /
// CBasePane by single non-virtual inheritance, so the bar pointer converts to
// CWnd* without adjustment.
// Deviation: only in the storage of the two links (side table instead of
// members); the result for a panel that sits in a category of a bar is the
// bar, and NULL otherwise, exactly as retail.
// Symbol: ?GetParentWnd@CMFCRibbonPanel@@MEBAPEAVCWnd@@XZ
extern "C" CWnd* MS_ABI impl__GetParentWnd_CMFCRibbonPanel__MEBAPEAVCWnd__XZ(
    const CMFCRibbonPanel* pThis)
{
    if (!pThis) return nullptr;

    std::lock_guard<std::mutex> lock(RibbonMutex());
    const CMFCRibbonCategory* pCategory = PanelParentCategory(pThis);
    if (pCategory == nullptr) return nullptr;
    const CMFCRibbonBar* pBar = CategoryParentBar(pCategory);
    return const_cast<CMFCRibbonBar*>(pBar);
}

// CMFCRibbonPanel::GetPressed() const -- retail (RVA 0x11c670) returns the
// first element whose GetPressed (vtable +0x390) is non-null, else NULL.
// OpenMFC models no element pressed state (impl__GetPressed_CMFCRibbonBaseElement
// always returns nullptr), so no pressed element can be reported.
// (mfc140u RVA 0x11b8f0; the RVA above is the ANSI twin's.)  The element
// thunk it would loop over, impl__GetPressed_CMFCRibbonBaseElement in
// featurepack/ribbon/CMFCRibbonBaseElement.cpp, is still a generated stub
// without a `this` parameter, so the loop is not written either.
// TODO(clean-room): transcribed partially -- element pressed state is not
// modeled.
// Symbol: ?GetPressed@CMFCRibbonPanel@@QEBAPEAVCMFCRibbonBaseElement@@XZ
extern "C" CMFCRibbonBaseElement* MS_ABI impl__GetPressed_CMFCRibbonPanel__QEBAPEAVCMFCRibbonBaseElement__XZ(
    const CMFCRibbonPanel* /*pThis*/)
{
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
// (mfc140u RVA 0x11cfa0; the RVA above is the ANSI twin's.)  The element
// thunk the loop would forward to, impl__GetVisibleElements_CMFCRibbonBaseElement
// in featurepack/ribbon/CMFCRibbonBaseElement.cpp, is itself a generated
// no-op with a placeholder signature, so writing the loop here would change
// nothing; the array is left exactly as retail leaves it for all-empty rects.
// TODO(clean-room): transcribed partially -- element rects are not modeled,
// so no element can be classified as visible.
// Symbol: ?GetVisibleElements@CMFCRibbonPanel@@QEAAXAEAV?$CArray@PEAVCMFCRibbonBaseElement@@PEAV1@@@@Z
extern "C" void MS_ABI impl__GetVisibleElements_CMFCRibbonPanel__QEAAXAEAV__CArray_PEAVCMFCRibbonBaseElement__PEAV1____Z(
    CMFCRibbonPanel* /*pThis*/,
    CArray<CMFCRibbonBaseElement*, CMFCRibbonBaseElement*>& /*arElements*/)
{
}

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
// (mfc140u RVA 0x11a570; the RVA above is the ANSI twin's.)
// Signature: `this` in RCX, bHighlight in EDX, the by-value CPoint packed in
// R8 (an 8-byte aggregate travels in a register under the MS x64 ABI, hence
// `long long`).
// TODO(clean-room): transcribed partially -- panel highlight/press state and
// element geometry are not modeled.
// Symbol: ?Highlight@CMFCRibbonPanel@@MEAAXHVCPoint@@@Z
extern "C" void MS_ABI impl__Highlight_CMFCRibbonPanel__MEAAXHVCPoint___Z(
    CMFCRibbonPanel* /*pThis*/, int /*bHighlight*/, long long /*point*/)
{
}

// CMFCRibbonPanel::HitTest(CPoint, BOOL) -- retail (RVA 0x11b5b0; mfc140u
// RVA 0x11a830, re-read there) tests, in order, the default panel button's
// rect (+0x3f0 +0xc8, non-empty and ::PtInRect -> returns the default
// button), the rect at +0x238 (-> returns the buttons group at +0x170), and
// then each element's own rect (+0xc8); for the first element rect hit it
// returns that element's own HitTest (element vtable +0x268), which lets a
// container element resolve a sub-item.  When nothing was hit and the BOOL
// argument is non-zero, a point inside the caption rect (+0xfc) yields the
// default panel button; otherwise NULL.  Every one of those rects lives past
// the end of the clean-room CMFCRibbonPanel.
// Signature: `this` in RCX, the by-value CPoint packed in RDX, the caption
// BOOL in R8D.
// TODO(clean-room): transcribed partially -- panel and element rects are not
// modeled.
// Symbol: ?HitTest@CMFCRibbonPanel@@UEAAPEAVCMFCRibbonBaseElement@@VCPoint@@H@Z
extern "C" CMFCRibbonBaseElement* MS_ABI impl__HitTest_CMFCRibbonPanel__UEAAPEAVCMFCRibbonBaseElement__VCPoint__H_Z(
    CMFCRibbonPanel* /*pThis*/, long long /*point*/, int /*bCheckPanelCaption*/)
{
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
// (mfc140u RVA 0x11a960; the RVA above is the ANSI twin's.)
// Signature: `this` in RCX, the by-value CPoint packed in RDX.
// TODO(clean-room): transcribed partially -- element rects are not modeled.
// Symbol: ?HitTestEx@CMFCRibbonPanel@@UEBAHVCPoint@@@Z
extern "C" int MS_ABI impl__HitTestEx_CMFCRibbonPanel__UEBAHVCPoint___Z(
    const CMFCRibbonPanel* /*pThis*/, long long /*point*/)
{
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
// (mfc140u RVA 0x11b0a0, re-read there: `movups 0x4b8(%rcx)` -> ::IsRectEmpty
// -> `sete %cl`; the RVA above is the ANSI twin's.)
// TODO(clean-room): transcribed partially -- the +0x4b8 collapsed rect is not
// modeled.
// Symbol: ?IsCollapsed@CMFCRibbonPanel@@QEBAHXZ
extern "C" int MS_ABI impl__IsCollapsed_CMFCRibbonPanel__QEBAHXZ(
    const CMFCRibbonPanel* /*pThis*/)
{
    return 0;
}

// CMFCRibbonPanel::IsWindows7Look() const -- retail (RVA 0x11dc20; mfc140u
// RVA 0x11ceb0), in full:
//     if (m_pParent == NULL) return FALSE;                       // +0x158
//     if (m_pParent->m_pParentRibbonBar == NULL) return FALSE;   // category +0x760
//     return m_pParent->m_pParentRibbonBar->m_bWindows7Look;     // bar +0x20c8
// The bar member is declared by the clean-room CMFCRibbonBar (afxmfc.h,
// byte 8392 == 0x20c8, zeroed by its constructor; the only writer,
// CMFCRibbonBar::SetWindows7Look in featurepack/ribbon/CMFCRibbonBar.cpp,
// is still a generated stub, so the flag currently reads 0 for every bar).
// The two parent links are recovered from the ribbon_state side tables
// exactly as in GetParentWnd above; that is the only deviation from retail.
// Symbol: ?IsWindows7Look@CMFCRibbonPanel@@QEBAHXZ
extern "C" int MS_ABI impl__IsWindows7Look_CMFCRibbonPanel__QEBAHXZ(
    const CMFCRibbonPanel* pThis)
{
    if (!pThis) return 0;

    std::lock_guard<std::mutex> lock(RibbonMutex());
    const CMFCRibbonCategory* pCategory = PanelParentCategory(pThis);
    if (pCategory == nullptr) return 0;
    const CMFCRibbonBar* pBar = CategoryParentBar(pCategory);
    if (pBar == nullptr) return 0;
    return pBar->m_bWindows7Look;
}

// CMFCRibbonPanel::JustifyElementsInColumn(int, int) -- retail (RVA
// 0x11ae60) returns immediately when nFirst > nLast or either index is
// negative; otherwise it takes the widest element rect (+0xd0 minus +0xc8)
// over [nFirst..nLast] and stretches every element in that range to it.
// Element rects are not modeled.
// (mfc140u RVA 0x11a0e0; the RVA above is the ANSI twin's.)
// TODO(clean-room): transcribed partially -- element rects are not modeled.
// Symbol: ?JustifyElementsInColumn@CMFCRibbonPanel@@IEAAXHH@Z
extern "C" void MS_ABI impl__JustifyElementsInColumn_CMFCRibbonPanel__IEAAXHH_Z(
    CMFCRibbonPanel* /*pThis*/, int /*nFirst*/, int /*nLast*/)
{
}

// CMFCRibbonPanel::MakeGalleryItemVisible(CMFCRibbonBaseElement*) -- retail
// (RVA 0x11ded0) returns unless the element passes CObject::IsKindOf against
// the runtime class at 0x180307218; then it compares the element's rect edges
// (+0xcc / +0xd4) with the gallery rect from GetGalleryRect (0x11dfe0),
// returns when the item already fits, and otherwise scrolls the palette by
// the difference (0x11d5a0) and hands the result to the parent window
// obtained from the vtable +0x158 GetParentWnd.  Element rects, the gallery
// rect and the palette scroll state are all unmodeled.
// (mfc140u RVA 0x11d160; the runtime class tested there, 0x1803092c8, is
// CMFCRibbonGalleryIcon; the RVA above is the ANSI twin's.)
// TODO(clean-room): transcribed partially -- element rects, the gallery rect
// and the palette scroll state are not modeled.
// Symbol: ?MakeGalleryItemVisible@CMFCRibbonPanel@@QEAAXPEAVCMFCRibbonBaseElement@@@Z
extern "C" void MS_ABI impl__MakeGalleryItemVisible_CMFCRibbonPanel__QEAAXPEAVCMFCRibbonBaseElement___Z(
    CMFCRibbonPanel* /*pThis*/, CMFCRibbonBaseElement* /*pItem*/)
{
}

// CMFCRibbonPanel::MouseButtonDown(CPoint) -- retail (RVA 0x11b7b0) sets the
// "pressed" flag at +0xe0, then works entirely through m_pHighlighted
// (+0x3e8): it queries the element's vtable +0x280 predicate, may mark it
// pressed (+0x144), redraws it through RedrawElement (vtable +0x220) and
// calls its OnLButtonDown (vtable +0x430), returning m_pHighlighted.  The
// highlighted-element pointer and the press state are not modeled.
// (mfc140u RVA 0x11aa30; the RVA above is the ANSI twin's.)
// Signature: `this` in RCX, the by-value CPoint packed in RDX.
// TODO(clean-room): transcribed partially -- the +0x3e8 highlighted element
// and the press state are not modeled.
// Symbol: ?MouseButtonDown@CMFCRibbonPanel@@MEAAPEAVCMFCRibbonBaseElement@@VCPoint@@@Z
extern "C" CMFCRibbonBaseElement* MS_ABI impl__MouseButtonDown_CMFCRibbonPanel__MEAAPEAVCMFCRibbonBaseElement__VCPoint___Z(
    CMFCRibbonPanel* /*pThis*/, long long /*point*/)
{
    return nullptr;
}

// CMFCRibbonPanel::MouseButtonUp(CPoint) -- retail (RVA 0x11b8e0; mfc140u
// RVA 0x11ab60, re-read there) clears the "pressed" flag at +0xe0 and returns
// at once when m_pHighlighted (+0x3e8) is NULL; otherwise it notes the parent
// window's m_hWnd (vtable +0x158 GetParentWnd), calls the highlighted
// element's OnLButtonUp (element vtable +0x438) and then -- only if that HWND
// still passes ::IsWindow and the element's pressed flag (+0x144) is set --
// clears +0x144, redraws it (panel vtable +0x220 RedrawElement) and, when
// m_pHighlighted changed meanwhile, redraws the new one too.  Neither the
// highlighted-element pointer nor the press state is modeled, so the early
// return is the only reachable path.
// Signature: `this` in RCX, the by-value CPoint packed in RDX.
// TODO(clean-room): transcribed partially -- the +0x3e8 highlighted element
// and the press state are not modeled.
// Symbol: ?MouseButtonUp@CMFCRibbonPanel@@MEAAXVCPoint@@@Z
extern "C" void MS_ABI impl__MouseButtonUp_CMFCRibbonPanel__MEAAXVCPoint___Z(
    CMFCRibbonPanel* /*pThis*/, long long /*point*/)
{
}

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
// (mfc140u RVA 0x11ade0; the RVA above is the ANSI twin's.)  The element
// thunk the loop would forward to, impl__NotifyControlCommand_CMFCRibbonBaseElement
// in featurepack/ribbon/CMFCRibbonBaseElement.cpp, is a generated stub with a
// placeholder signature (no `this`), so the loop is not written here.
// TODO(clean-room): transcribed partially -- element control hosting is not
// modeled.
// Symbol: ?NotifyControlCommand@CMFCRibbonPanel@@MEAAHHH_K_J@Z
extern "C" int MS_ABI impl__NotifyControlCommand_CMFCRibbonPanel__MEAAHHH_K_J_Z(
    CMFCRibbonPanel* /*pThis*/, int /*bAccelerator*/, int /*nNotifyCode*/,
    unsigned long long /*wParam*/, long long /*lParam*/)
{
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
// (mfc140u RVA 0x11ae80; the RVA above is the ANSI twin's.)
// TODO(clean-room): transcribed partially -- element rects are not modeled.
// Symbol: ?OnAfterChangeRect@CMFCRibbonPanel@@MEAAXPEAVCDC@@@Z
extern "C" void MS_ABI impl__OnAfterChangeRect_CMFCRibbonPanel__MEAAXPEAVCDC___Z(
    CMFCRibbonPanel* /*pThis*/, CDC* /*pDC*/)
{
}

// CMFCRibbonPanel::OnDrawPaletteMenu(CDC*) -- retail (RVA 0x117df0) starts by
// testing the +0x168 parent pointer with CObject::IsKindOf against the
// runtime class at 0x18030b670, and the ~0x400 bytes that follow draw the
// palette decorations against that parent and the panel's layout rects.
// Neither the +0x168 link nor any panel drawing state is modeled.
// (mfc140u RVA 0x117060; the RVA above is the ANSI twin's.)
// TODO(clean-room): transcribed partially -- panel palette drawing is not
// modeled.
// Symbol: ?OnDrawPaletteMenu@CMFCRibbonPanel@@IEAAXPEAVCDC@@@Z
extern "C" void MS_ABI impl__OnDrawPaletteMenu_CMFCRibbonPanel__IEAAXPEAVCDC___Z(
    CMFCRibbonPanel* /*pThis*/, CDC* /*pDC*/)
{
}

// CMFCRibbonPanel::OnKey(UINT) -- retail (RVA 0x11c790) returns FALSE at once
// when m_arElements is empty; the ~0xaa0 bytes that follow drive keyboard
// navigation starting from m_pHighlighted (+0x3e8) across the element list.
// The highlighted-element pointer and the element geometry the navigation
// depends on are not declared by the clean-room CMFCRibbonPanel, so only the
// empty-panel early-out could be honoured and it says nothing about a
// populated panel.
// (mfc140u RVA 0x11ba10, taken from the mfc140u CMFCRibbonPanel vftable at
// 0x180309390 slot +0x1c8 -- this export is not in the mfc140u symbol map;
// the RVA above is the ANSI twin's.)
// TODO(clean-room): transcribed partially -- panel focus/highlight state and
// element geometry are not modeled.
// Symbol: ?OnKey@CMFCRibbonPanel@@UEAAHI@Z
extern "C" int MS_ABI impl__OnKey_CMFCRibbonPanel__UEAAHI_Z(
    CMFCRibbonPanel* /*pThis*/, unsigned int /*nChar*/)
{
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
// (mfc140u RVA 0x11cc10, from the mfc140u panel vftable slot +0x230; the RVA
// above is the ANSI twin's.)
// TODO(clean-room): transcribed partially -- element image mirroring and the
// embedded objects are not modeled.
// Symbol: ?OnRTLChanged@CMFCRibbonPanel@@MEAAXH@Z
extern "C" void MS_ABI impl__OnRTLChanged_CMFCRibbonPanel__MEAAXH_Z(
    CMFCRibbonPanel* /*pThis*/, int /*bIsRTL*/)
{
}

// CMFCRibbonPanel::OnSetAccData(long) -- retail (RVA 0x11e040) resets the
// CAccessibilityData block embedded at +0x40, fills a local array through
// CMFCRibbonPanel::GetVisibleElements (0x11dd10) and forwards to element
// lVal - 1, or describes the panel itself for lVal == 0.  Both the +0x40
// accessibility block and the element rects that decide visibility are
// unmodeled.
// (mfc140u RVA 0x11d2d0, from the mfc140u panel vftable slot +0x160; the RVA
// above is the ANSI twin's.)
// TODO(clean-room): transcribed partially -- the embedded accessibility data
// block and element rects are not modeled.
// Symbol: ?OnSetAccData@CMFCRibbonPanel@@MEAAHJ@Z
extern "C" int MS_ABI impl__OnSetAccData_CMFCRibbonPanel__MEAAHJ_Z(
    CMFCRibbonPanel* /*pThis*/, long /*lVal*/)
{
    return 0;
}

// CMFCRibbonPanel::OnShow(BOOL) -- retail (RVA 0x11bd50):
//     for (i ...) m_arElements[i]->OnShow(bShow && !m_arElements[i]->m_rect.IsRectEmpty());
// (element vtable +0x260; the flag passed is bShow ANDed with "this element
// has a non-empty rect at +0xc8").  The base element's OnShow (RVA 0x2820) is
// a bare `ret`, and element rects are not modeled, so nothing observable
// happens for the modeled element set.
// (mfc140u RVA 0x11afd0, from the mfc140u panel vftable slot +0x218; the RVA
// above is the ANSI twin's.)
// TODO(clean-room): transcribed partially -- element rects and element show
// state are not modeled.
// Symbol: ?OnShow@CMFCRibbonPanel@@MEAAXH@Z
extern "C" void MS_ABI impl__OnShow_CMFCRibbonPanel__MEAAXH_Z(
    CMFCRibbonPanel* /*pThis*/, int /*bShow*/)
{
}

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
// (mfc140u RVA 0x11ad40, from the mfc140u panel vftable slot +0x200; the RVA
// above is the ANSI twin's.)
// TODO(clean-room): transcribed partially -- element command-UI state is not
// modeled.
// Symbol: ?OnUpdateCmdUI@CMFCRibbonPanel@@MEAAXPEAVCMFCRibbonCmdUI@@PEAVCFrameWnd@@H@Z
extern "C" void MS_ABI impl__OnUpdateCmdUI_CMFCRibbonPanel__MEAAXPEAVCMFCRibbonCmdUI__PEAVCFrameWnd__H_Z(
    CMFCRibbonPanel* /*pThis*/, CMFCRibbonCmdUI* /*pCmdUI*/, CFrameWnd* /*pTarget*/,
    int /*bDisableIfNoHndler*/)
{
}

// CMFCRibbonPanel::RecalcWidths(CDC*, int) -- retail (RVA 0x11af00) drives
// the panel's whole width-fitting pass: it walks m_arElements calling their
// size/layout virtuals and records the resulting per-state widths on the
// panel.  Those cached widths all live past the end of the clean-room
// CMFCRibbonPanel, which declares no layout members at all.
// (mfc140u RVA 0x11a180, from the mfc140u panel vftable slot +0x1a0; the RVA
// above is the ANSI twin's.)
// TODO(clean-room): transcribed partially -- panel layout geometry is not
// modeled.
// Symbol: ?RecalcWidths@CMFCRibbonPanel@@UEAAXPEAVCDC@@H@Z
extern "C" void MS_ABI impl__RecalcWidths_CMFCRibbonPanel__UEAAXPEAVCDC__H_Z(
    CMFCRibbonPanel* /*pThis*/, CDC* /*pDC*/, int /*nHeight*/)
{
}

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
// (mfc140u RVA 0x11b5b0, from the mfc140u panel vftable slot +0x220 and
// re-read there: ::IsRectEmpty on a copy of element +0xc8, then vtable +0x158,
// then ::InvalidateRect(hWnd, &rect, TRUE) and ::UpdateWindow(hWnd); the RVA
// above is the ANSI twin's.)  The parent window is now recoverable through
// GetParentWnd above, but the element rect is not, so the empty-rect early
// return still applies to every element and no repaint is issued.
// TODO(clean-room): transcribed partially -- element rects are not modeled.
// Symbol: ?RedrawElement@CMFCRibbonPanel@@MEAAXPEAVCMFCRibbonBaseElement@@@Z
extern "C" void MS_ABI impl__RedrawElement_CMFCRibbonPanel__MEAAXPEAVCMFCRibbonBaseElement___Z(
    CMFCRibbonPanel* /*pThis*/, CMFCRibbonBaseElement* /*pElem*/)
{
}

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

// CMFCRibbonPanel::RemoveAll() -- retail (mfc140u RVA 0x116c10; ANSI twin
// 0x1179a0, whose export map names it), transcribed:
//     for (i = 0; i < m_arElements.GetSize(); i++)
//         if (m_arElements[i] != NULL) delete m_arElements[i];  // vtable +0x8, flag 1
//     m_arElements.RemoveAll();          // the two-argument CArray SetSize helper at
//                                        // 0x180011f68 (mfc140; 0x180011f48 in mfc140u)
//                                        // called with nNewSize = 0; it takes no
//                                        // grow-by argument (R8 is not read) and uses
//                                        // m_nGrowBy (+0x20) when it has to grow
//     <flag at +0xb0> = 1;               // the cached flag Insert/Remove maintain
//     m_pHighlighted = NULL;             // +0x3e8
// Every non-null element is deleted and the list emptied, which is what the
// side-table body does; the null test is retail's own (`test %rcx,%rcx` at
// 0x180116c42 inside RemoveAll, mfc140u), and a null entry here is OpenMFC's
// separator (see AddSeparator above; retail's AddSeparator stores a
// CMFCRibbonSeparator object instead).  The +0xb0 flag and the +0x3e8
// highlighted pointer are not declared by the clean-room CMFCRibbonPanel.
// The elements are collected under the lock and destroyed after it is
// released, as Remove above does, so a derived-class destructor that touches
// ribbon state cannot deadlock on the non-recursive RibbonMutex().
// TODO(clean-room): partially transcribed -- the +0xb0 flag and the +0x3e8
// highlighted pointer are not modeled.
// Symbol: ?RemoveAll@CMFCRibbonPanel@@QEAAXXZ
extern "C" void MS_ABI impl__RemoveAll_CMFCRibbonPanel__QEAAXXZ(CMFCRibbonPanel* pThis)
{
    if (!pThis) return;

    std::vector<CMFCRibbonBaseElement*> doomed;
    {
        std::lock_guard<std::mutex> lock(RibbonMutex());
        auto it = RibbonPanelStates().find(pThis);
        if (it == RibbonPanelStates().end()) return;

        doomed.swap(it->second.elements);
        it->second.ownedElements.clear();
    }

    for (CMFCRibbonBaseElement* pElem : doomed) {
        if (pElem) {
            delete pElem;
        }
    }
}

// CMFCRibbonPanel::Replace(int, CMFCRibbonBaseElement*) -- retail (mfc140u
// RVA 0x11b320; ANSI twin 0x11c0a0), transcribed:
//     if (nIndex < 0 || nIndex >= m_arElements.GetSize()) return FALSE;
//     CMFCRibbonBaseElement* pOld = m_arElements[nIndex];
//     pElem->CopyFrom(*pOld);                 // element vtable +0x2e0
//     m_arElements[nIndex] = pElem;
//     if (pOld != NULL) delete pOld;          // vtable +0x8, flag 1
//     return TRUE;
// The bounds contract, the copy, the slot store and the delete are
// reproduced against the side-table list.  CopyFrom is reached through the
// exported base implementation rather than the element vtable (OpenMFC's
// elements dispatch nothing), so only the command ID is copied.
// Deviation: retail dereferences pElem unconditionally (`mov (%r8),%rax`),
// so a null pElem faults there; this body returns FALSE instead of storing a
// null -- which the side table would read as a separator -- and faulting.
// Deviation: a null slot (OpenMFC's separator representation; retail's
// array holds a CMFCRibbonSeparator object there) is replaced without the
// CopyFrom step, since there is no source element to copy.
// Ownership: the new element joins the owned set and the old one leaves it
// before being destroyed, as in Remove above.
// Symbol: ?Replace@CMFCRibbonPanel@@QEAAHHPEAVCMFCRibbonBaseElement@@@Z
extern "C" int MS_ABI impl__Replace_CMFCRibbonPanel__QEAAHHPEAVCMFCRibbonBaseElement___Z(
    CMFCRibbonPanel* pThis, int nIndex, CMFCRibbonBaseElement* pElem)
{
    if (!pThis || !pElem) return 0;

    CMFCRibbonBaseElement* pOld = nullptr;
    {
        std::lock_guard<std::mutex> lock(RibbonMutex());
        auto it = RibbonPanelStates().find(pThis);
        if (it == RibbonPanelStates().end()) return 0;

        auto& elements = it->second.elements;
        if (nIndex < 0 || nIndex >= static_cast<int>(elements.size())) return 0;

        pOld = elements[static_cast<size_t>(nIndex)];
        if (pOld) {
            impl__CopyFrom_CMFCRibbonBaseElement__UEAAXAEBV1__Z(pElem, *pOld);
            it->second.ownedElements.erase(pOld);
        }
        elements[static_cast<size_t>(nIndex)] = pElem;
        it->second.ownedElements.insert(pElem);
    }

    if (pOld) {
        delete pOld;
    }
    return 1;
}

// CMFCRibbonPanel::ReplaceByID(UINT, CMFCRibbonBaseElement*) -- retail
// (mfc140u RVA 0x11b3b0; ANSI twin 0x11c130), transcribed:
//     if (uiCmdID == 0 || uiCmdID == (UINT)-1) return FALSE;   // (id-1) > 0xfffffffd
//     for (i = 0; i < m_arElements.GetSize(); i++) {
//         CMFCRibbonBaseElement* pE = m_arElements[i];
//         if (pE->m_nID == uiCmdID) return Replace(i, pElem);   // +0x118, then 0x11c0a0
//         if (pE->ReplaceByID(uiCmdID, pElem)) return TRUE;     // element vtable +0x470
//     }
//     return FALSE;
// (The +0x470 slot is ReplaceByID: CMFCRibbonButtonsGroup's vtable carries
// ?ReplaceByID@CMFCRibbonButtonsGroup@@ there; the base body is
// `xor eax,eax; ret`.)  The ID compare is the exported base
// CMFCRibbonBaseElement::FindByID (`m_nID == id ? this : NULL`).  The
// nested ReplaceByID dispatch is not made: OpenMFC's element thunk for it is
// a generated stub with a placeholder signature, and the base answer is FALSE
// anyway, so only direct members of the panel are matched -- the same
// limitation as HasElement above.
// TODO(clean-room): partially transcribed -- container elements' nested
// ReplaceByID is not modeled.
// Symbol: ?ReplaceByID@CMFCRibbonPanel@@QEAAHIPEAVCMFCRibbonBaseElement@@@Z
extern "C" int MS_ABI impl__ReplaceByID_CMFCRibbonPanel__QEAAHIPEAVCMFCRibbonBaseElement___Z(
    CMFCRibbonPanel* pThis, unsigned int uiCmdID, CMFCRibbonBaseElement* pElem)
{
    if (!pThis) return 0;
    if (uiCmdID - 1u > 0xfffffffdu) return 0;

    int nIndex = -1;
    {
        std::lock_guard<std::mutex> lock(RibbonMutex());
        auto it = RibbonPanelStates().find(pThis);
        if (it == RibbonPanelStates().end()) return 0;

        const auto& elements = it->second.elements;
        for (size_t i = 0; i < elements.size(); ++i) {
            if (elements[i] &&
                impl__FindByID_CMFCRibbonBaseElement__UEAAPEAV1_I_Z(elements[i], uiCmdID) != nullptr) {
                nIndex = static_cast<int>(i);
                break;
            }
        }
    }
    if (nIndex < 0) return 0;

    return impl__Replace_CMFCRibbonPanel__QEAAHHPEAVCMFCRibbonBaseElement___Z(pThis, nIndex, pElem);
}

// CMFCRibbonPanel::Reposition(CDC*, const CRect&) -- retail (mfc140u RVA
// 0x117700, from the mfc140u panel vftable slot +0x1e8; ANSI twin 0x118480,
// ~0x1400 bytes) opens with ::SetRectEmpty on the caption rect (+0xfc) and
// the two menu-area rects (+0x11c / +0x12c), then branches: a non-null
// palette owner at +0x168 calls ReposPalette (vtable +0x1f8) and returns, a
// set +0xb8 flag calls RepositionMenu (vtable +0x1f0) and returns, and
// otherwise it links the embedded default button (+0x3f0) and the embedded
// group (+0x170) to the parent category, stores the incoming rect into m_rect
// (+0x10c), zeroes the group rect at +0x238 and
// runs the column layout over every element rect.  All of that state lives
// past the end of the clean-room CMFCRibbonPanel.
// TODO(clean-room): not transcribed -- panel layout geometry is not modeled.
// Symbol: ?Reposition@CMFCRibbonPanel@@MEAAXPEAVCDC@@AEBVCRect@@@Z
extern "C" void MS_ABI impl__Reposition_CMFCRibbonPanel__MEAAXPEAVCDC__AEBVCRect___Z(
    CMFCRibbonPanel* /*pThis*/, CDC* /*pDC*/, const CRect& /*rect*/)
{
}

// CMFCRibbonPanel::RepositionMenu(CDC*, const CRect&) -- retail (mfc140u RVA
// 0x118dd0, from the mfc140u panel vftable slot +0x1f0; ANSI twin 0x119b50,
// ~0x340 bytes) zeroes +0xd8 and the pair at +0xf0, reads the parent
// category's +0x9d0 when +0x158 is set, and then walks m_arElements laying
// the menu rows out into each element's rect (+0xc8) -- elements below the
// scroll offset at +0xf8 get an empty rect.  Nothing it reads or writes is
// declared by the clean-room CMFCRibbonPanel or CMFCRibbonBaseElement.
// TODO(clean-room): not transcribed -- menu layout geometry is not modeled.
// Symbol: ?RepositionMenu@CMFCRibbonPanel@@MEAAXPEAVCDC@@AEBVCRect@@@Z
extern "C" void MS_ABI impl__RepositionMenu_CMFCRibbonPanel__MEAAXPEAVCDC__AEBVCRect___Z(
    CMFCRibbonPanel* /*pThis*/, CDC* /*pDC*/, const CRect& /*rect*/)
{
}

// CMFCRibbonPanel::ReposPalette(CDC*, const CRect&) -- retail (mfc140u RVA
// 0x119110, from the mfc140u panel vftable slot +0x1f8; ANSI twin 0x119e90,
// ~0xd00 bytes) returns at once when the incoming rect ::EqualRect's an empty
// one, tests the +0x168 palette owner with CObject::IsKindOf against the
// CMFCRibbonUndoButton descriptor, zeroes the scroll offset at +0xf8 and lays
// the gallery icons out into the element rects.  None of that state is
// declared by the clean-room classes.
// TODO(clean-room): not transcribed -- palette layout geometry is not
// modeled.
// Symbol: ?ReposPalette@CMFCRibbonPanel@@MEAAXPEAVCDC@@AEBVCRect@@@Z
extern "C" void MS_ABI impl__ReposPalette_CMFCRibbonPanel__MEAAXPEAVCDC__AEBVCRect___Z(
    CMFCRibbonPanel* /*pThis*/, CDC* /*pDC*/, const CRect& /*rect*/)
{
}

// CMFCRibbonPanel::ScrollPalette(int, BOOL) -- retail (mfc140u RVA 0x11c830;
// ANSI twin 0x11d5a0), transcribed:
//     int nDelta = bIsDelta ? nScrollOffset : m_nScrollOffset - nScrollOffset;  // +0xf8
//     if (nDelta == 0) return;
//     m_nScrollOffset = bIsDelta ? m_nScrollOffset - nDelta : nScrollOffset;
//     for (i = 0; i < m_arElements.GetSize(); i++)
//         if (elem->IsKindOf(RUNTIME_CLASS(CMFCRibbonLabel)) ||
//             elem->IsKindOf(RUNTIME_CLASS(CMFCRibbonGalleryIcon)))
//             ::OffsetRect(&elem->m_rect, 0, nDelta);          // element +0xc8
// The scroll offset at +0xf8 and the element rects are not declared by the
// clean-room classes, so there is nothing to scroll.
// TODO(clean-room): not transcribed -- the palette scroll offset and element
// rects are not modeled.
// Symbol: ?ScrollPalette@CMFCRibbonPanel@@IEAAXHH@Z
extern "C" void MS_ABI impl__ScrollPalette_CMFCRibbonPanel__IEAAXHH_Z(
    CMFCRibbonPanel* /*pThis*/, int /*nScrollOffset*/, int /*bIsDelta*/)
{
}

// CMFCRibbonPanel::SetACCData(CWnd*, CAccessibilityData&) -- retail (mfc140u
// RVA 0x11da80, from the mfc140u panel vftable slot +0x168; ANSI twin
// 0x11e7f0), transcribed (offsets are the CAccessibilityData layout declared
// in afxmfc.h):
//     data.m_strAccDefAction.Empty(); data.m_strAccValue.Empty();
//     data.m_strDescription.Empty();  data.m_strAccKeys.Empty();   // +0x28/+0x8/+0x10/+0x18
//     data.m_strAccName = m_strName;                                // +0x0 <- +0x140
//     data.m_nAccRole = 0x16;                                       // ROLE_SYSTEM_TOOLBAR
//     data.m_strAccValue = L"Group";                                // SetString(p, wcslen(p))
//     data.m_rectAccLocation = m_rect;                              // +0x3c <- +0x10c
//     if (m_pParent && m_pParent->m_pParentRibbonBar && bar->m_hWnd)
//         bar->ClientToScreen(&data.m_rectAccLocation);             // 0x1802a3310
//     data.m_bAccState = 0;                                         // +0x34
//     return TRUE;
// The pParent argument (RDX) is never read.  The four Empty() calls, the
// name (through the public GetName accessor, m_strName being protected),
// the role, the L"Group" value and the state are reproduced exactly -- the
// same shape as CMFCRibbonCategory::SetACCData in
// featurepack/ribbon/CMFCRibbonCategory.cpp.  The panel rect m_rect is not
// declared by the clean-room CMFCRibbonPanel; the rect mapped is the empty
// one retail's CommonInit leaves in +0x10c for a panel Reposition has never
// laid out (every OpenMFC panel, Reposition below being a stub), so the
// location reported is the bar's client origin with width and height 0 --
// what retail reports for the modeled state -- exactly as accLocation above
// does.  The category -> bar walk is this file's exported GetParentWnd,
// whose only modeled branch is that same m_pParent->m_pParentRibbonBar
// lookup (its +0x160 menu-bar branch is unmodeled).
// TODO(clean-room): partially transcribed -- the panel rect is not modeled.
// Symbol: ?SetACCData@CMFCRibbonPanel@@MEAAHPEAVCWnd@@AEAVCAccessibilityData@@@Z
extern "C" int MS_ABI impl__SetACCData_CMFCRibbonPanel__MEAAHPEAVCWnd__AEAVCAccessibilityData___Z(
    CMFCRibbonPanel* pThis, CWnd* /*pParent*/, CAccessibilityData& data)
{
    if (!pThis) return 0;

    data.m_strAccDefAction.Empty();
    data.m_strAccValue.Empty();
    data.m_strDescription.Empty();
    data.m_strAccKeys.Empty();
    data.m_strAccName = pThis->GetName();
    data.m_nAccRole = 0x16;              // ROLE_SYSTEM_TOOLBAR
    data.m_strAccValue = L"Group";

    RECT rect = {0, 0, 0, 0};   // m_rect of a never-laid-out panel
    CWnd* pBar = impl__GetParentWnd_CMFCRibbonPanel__MEBAPEAVCWnd__XZ(pThis);
    if (pBar != nullptr && pBar->m_hWnd != nullptr) {
        impl__ClientToScreen_CWnd__QEBAXPEAUtagRECT___Z(pBar, &rect);
    }
    data.m_rectAccLocation = CRect(rect.left, rect.top, rect.right, rect.bottom);
    data.m_bAccState = 0;
    return 1;
}

// CMFCRibbonPanel::SetElementMenu(UINT, UINT, BOOL, BOOL) -- retail (mfc140u
// RVA 0x11b2c0; ANSI twin 0x11c040), transcribed:
//     CMFCRibbonBaseElement* pElem = FindByID(uiCmdID);         // 0x18011b0f0 (mfc140u)
//     if (pElem == NULL) return FALSE;
//     if (!pElem->IsKindOf(RUNTIME_CLASS(CMFCRibbonButton))) return FALSE;  // 0x180300a80
//     ((CMFCRibbonButton*)pElem)->SetMenu(uiMenuResID, bRightAlign, bTopAlign); // 0x1800e5580
//     return TRUE;
// FindByID is this file's own exported panel implementation, the class test
// is the name-walk ObjectIsKindOf above (see its comment for why a pointer
// compare cannot be used yet), and the menu is handed to the exported
// CMFCRibbonButton::SetMenu(UINT, int, int), which is still a no-op in
// featurepack/ribbon/CMFCRibbonButton.cpp -- so the return contract is exact
// and the menu itself is dropped there, not here.
// Symbol: ?SetElementMenu@CMFCRibbonPanel@@QEAAHIIHH@Z
extern "C" int MS_ABI impl__SetElementMenu_CMFCRibbonPanel__QEAAHIIHH_Z(
    CMFCRibbonPanel* pThis, unsigned int uiCmdID, unsigned int uiMenuResID,
    int bRightAlign, int bTopAlign)
{
    if (!pThis) return 0;

    CMFCRibbonBaseElement* pElem =
        impl__FindByID_CMFCRibbonPanel__QEBAPEAVCMFCRibbonBaseElement__I_Z(pThis, uiCmdID);
    if (pElem == nullptr) return 0;
    if (!ObjectIsKindOf(pElem, "CMFCRibbonButton")) return 0;

    impl__SetMenu_CMFCRibbonButton__QEAAXIHH_Z(
        static_cast<CMFCRibbonButton*>(pElem), uiMenuResID, bRightAlign, bTopAlign);
    return 1;
}

// CMFCRibbonPanel::SetElementMenu(UINT, HMENU, BOOL, BOOL) -- retail (mfc140u
// RVA 0x11b250; ANSI twin 0x11bfd0) is the HMENU twin of the overload above:
// the same FindByID / IsKindOf(CMFCRibbonButton) gate, then
// CMFCRibbonButton::SetMenu(HMENU, int, int) (0x1800e52c0 in mfc140u) and
// TRUE.  Same wiring and the same caveat about the no-op SetMenu body.
// Symbol: ?SetElementMenu@CMFCRibbonPanel@@QEAAHIPEAUHMENU__@@HH@Z
extern "C" int MS_ABI impl__SetElementMenu_CMFCRibbonPanel__QEAAHIPEAUHMENU____HH_Z(
    CMFCRibbonPanel* pThis, unsigned int uiCmdID, HMENU hMenu, int bRightAlign, int bTopAlign)
{
    if (!pThis) return 0;

    CMFCRibbonBaseElement* pElem =
        impl__FindByID_CMFCRibbonPanel__QEBAPEAVCMFCRibbonBaseElement__I_Z(pThis, uiCmdID);
    if (pElem == nullptr) return 0;
    if (!ObjectIsKindOf(pElem, "CMFCRibbonButton")) return 0;

    impl__SetMenu_CMFCRibbonButton__QEAAXPEAUHMENU____HH_Z(
        static_cast<CMFCRibbonButton*>(pElem), hMenu, bRightAlign, bTopAlign);
    return 1;
}

// CMFCRibbonPanel::SetElementRTC(int, CRuntimeClass*) -- retail (mfc140u RVA
// 0x11b450; ANSI twin 0x11c1d0), transcribed:
//     if (pRTC == NULL) AfxThrowInvalidArgException();          // ENSURE
//     if (!pRTC->IsDerivedFrom(RUNTIME_CLASS(CMFCRibbonBaseElement))) return NULL;
//     CMFCRibbonBaseElement* pElem = (CMFCRibbonBaseElement*)pRTC->CreateObject();
//     if (pElem != NULL && !pElem->IsKindOf(RUNTIME_CLASS(CMFCRibbonBaseElement)))
//         pElem = NULL;                                          // DYNAMIC_DOWNCAST
//     if (!Replace(nIndex, pElem)) { if (pElem) delete pElem; }
//     return pElem;
// CreateObject is the inline CRuntimeClass helper in afx.h (the retail call
// lands on the exported one at 0x180234d60 in mfc140u); the IsDerivedFrom
// (0x180234e20) and IsKindOf (0x180234cf0) tests are the name-walk helpers
// above, for the duplicate-descriptor reason given there.  A created object
// that fails the downcast is leaked exactly as retail leaks it (retail never
// deletes the non-element object either).  A descriptor without a creator
// yields NULL from CreateObject; retail then passes that NULL to Replace,
// which faults on it when nIndex is in range and returns FALSE (so NULL
// comes back) when it is not.  Here Replace refuses the null and NULL is
// returned either way.  Caveat: retail's ribbon element descriptors DO carry
// creators (mfc140u 0x180300a80 = CMFCRibbonButton has
// ?CreateObject@CMFCRibbonButton@@ at +0x10), so
// SetElementRTC(i, RUNTIME_CLASS(CMFCRibbonButton)) creates a button there;
// the descriptors OpenMFC hands an MSVC client from
// featurepack/ribbon/RuntimeClasses.cpp leave m_pfnCreateObject NULL even
// though the factories exist in featurepack/ribbon/DynCreateFactories.cpp,
// so the same call currently returns NULL from this body.
// Deviation: after a failed Replace retail deletes pElem and still returns
// the dead pointer (`mov %rbx,%rax` is reached on both arms); this body
// returns NULL on that path, since handing back freed memory can only crash
// the caller.
// Deviation: a null pRTC returns NULL instead of throwing -- OpenMFC's
// AfxThrowInvalidArgException is reached through the C++ exception machinery
// the impl__ thunks do not use.
// Symbol: ?SetElementRTC@CMFCRibbonPanel@@QEAAPEAVCMFCRibbonBaseElement@@HPEAUCRuntimeClass@@@Z
extern "C" CMFCRibbonBaseElement* MS_ABI impl__SetElementRTC_CMFCRibbonPanel__QEAAPEAVCMFCRibbonBaseElement__HPEAUCRuntimeClass___Z(
    CMFCRibbonPanel* pThis, int nIndex, CRuntimeClass* pRTC)
{
    if (!pThis || !pRTC) return nullptr;
    if (!RuntimeClassDerivesFrom(pRTC, "CMFCRibbonBaseElement")) return nullptr;

    CObject* pObject = pRTC->CreateObject();
    CMFCRibbonBaseElement* pElem = nullptr;
    if (ObjectIsKindOf(pObject, "CMFCRibbonBaseElement")) {
        pElem = static_cast<CMFCRibbonBaseElement*>(pObject);
    }

    if (!impl__Replace_CMFCRibbonPanel__QEAAHHPEAVCMFCRibbonBaseElement___Z(pThis, nIndex, pElem)) {
        if (pElem) {
            delete pElem;
        }
        return nullptr;
    }
    return pElem;
}

// CMFCRibbonPanel::SetElementRTCByID(UINT, CRuntimeClass*) -- retail (mfc140u
// RVA 0x11b4f0; ANSI twin 0x11c270) is SetElementRTC with ReplaceByID
// (0x18011b3b0 in mfc140u) in place of Replace: the same ENSURE, the same
// IsDerivedFrom gate, CreateObject + downcast, then
//     if (!ReplaceByID(uiCmdID, pElem)) { if (pElem) delete pElem; }
//     return pElem;
// Same wiring and the same two deviations as SetElementRTC above (NULL
// instead of a freed pointer after a failed replace; NULL instead of a throw
// for a null pRTC).
// Symbol: ?SetElementRTCByID@CMFCRibbonPanel@@QEAAPEAVCMFCRibbonBaseElement@@IPEAUCRuntimeClass@@@Z
extern "C" CMFCRibbonBaseElement* MS_ABI impl__SetElementRTCByID_CMFCRibbonPanel__QEAAPEAVCMFCRibbonBaseElement__IPEAUCRuntimeClass___Z(
    CMFCRibbonPanel* pThis, unsigned int uiCmdID, CRuntimeClass* pRTC)
{
    if (!pThis || !pRTC) return nullptr;
    if (!RuntimeClassDerivesFrom(pRTC, "CMFCRibbonBaseElement")) return nullptr;

    CObject* pObject = pRTC->CreateObject();
    CMFCRibbonBaseElement* pElem = nullptr;
    if (ObjectIsKindOf(pObject, "CMFCRibbonBaseElement")) {
        pElem = static_cast<CMFCRibbonBaseElement*>(pObject);
    }

    if (!impl__ReplaceByID_CMFCRibbonPanel__QEAAHIPEAVCMFCRibbonBaseElement___Z(pThis, uiCmdID, pElem)) {
        if (pElem) {
            delete pElem;
        }
        return nullptr;
    }
    return pElem;
}

// CMFCRibbonPanel::SetFocused(CMFCRibbonBaseElement*) -- retail (mfc140u RVA
// 0x11d050; ANSI twin 0x11ddc0), transcribed:
//     CMFCRibbonBaseElement* pOld = GetFocused();          // 0x18011cee0 (mfc140u)
//     if (pOld == pNewFocus) return;
//     if (pOld != NULL) {
//         pOld->+0x140 = 0;  pOld->vtable+0x458(FALSE);    // focus flag / OnSetFocus
//         if (pOld->+0x13c != 0) {                         // highlight flag
//             pOld->+0x13c = 0;  pOld->vtable+0x450(FALSE); // OnHighlight
//             if (m_pHighlighted == pOld) m_pHighlighted = NULL;   // +0x3e8
//         }
//         pOld->vtable+0x380();                             // Redraw
//     }
//     if (pNewFocus != NULL) {
//         pNewFocus->+0x13c = 1;  pNewFocus->+0x140 = 1;
//         pNewFocus->vtable+0x458(TRUE);  pNewFocus->vtable+0x450(TRUE);
//         pNewFocus->vtable+0x380();
//         m_pHighlighted = pNewFocus;
//     }
// The element flags at +0x13c / +0x140 and the three element virtuals are
// past the clean-room CMFCRibbonBaseElement, and +0x3e8 is past the
// clean-room panel; GetFocused above can never report a focused element, so
// the pOld block is never entered here.  With a NULL pNewFocus retail takes
// the pOld == pNewFocus early return; with a non-NULL one it runs the
// pNewFocus block, which touches nothing but those unmodeled flags,
// virtuals and +0x3e8 -- so neither case has a modeled effect.
// TODO(clean-room): not transcribed -- element focus/highlight state is not
// modeled.
// Symbol: ?SetFocused@CMFCRibbonPanel@@QEAAXPEAVCMFCRibbonBaseElement@@@Z
extern "C" void MS_ABI impl__SetFocused_CMFCRibbonPanel__QEAAXPEAVCMFCRibbonBaseElement___Z(
    CMFCRibbonPanel* /*pThis*/, CMFCRibbonBaseElement* /*pNewFocus*/)
{
}

// CMFCRibbonPanel::SetKeys(LPCTSTR) -- retail (mfc140u RVA 0x11cbf0, located
// as the body 0x20 before OnRTLChanged's vftable slot target; the export is
// absent from the mfc140u symbol map and the ANSI twin
// ?SetKeys@CMFCRibbonPanel@@QEAAXPEBD@Z sits at 0x11d960), in full:
//     m_btnDefault.SetKeys(lpszKeys, NULL);    // +0x3f0, element vtable +0x180, R8 = 0
// The embedded default panel button at +0x3f0 is not declared by the
// clean-room CMFCRibbonPanel, so the keys have nowhere to go.
// TODO(clean-room): not transcribed -- the embedded default panel button is
// not modeled.
// Symbol: ?SetKeys@CMFCRibbonPanel@@QEAAXPEB_W@Z
extern "C" void MS_ABI impl__SetKeys_CMFCRibbonPanel__QEAAXPEB_W_Z(
    CMFCRibbonPanel* /*pThis*/, const wchar_t* /*lpszKeys*/)
{
}

// Symbol: ?ShowDefaultButton@CMFCRibbonPanel@@IEAAXPEAVCDC@@@Z
extern "C" void MS_ABI impl__ShowDefaultButton_CMFCRibbonPanel__IEAAXPEAVCDC___Z(void* /*class*/* p0) {}

// Symbol: ?ShowPopup@CMFCRibbonPanel@@QEAAPEAVCMFCRibbonPanelMenu@@PEAVCMFCRibbonDefaultPanelButton@@@Z
extern "C" void* MS_ABI impl__ShowPopup_CMFCRibbonPanel__QEAAPEAVCMFCRibbonPanelMenu__PEAVCMFCRibbonDefaultPanelButton___Z(void* /*class*/* p0) {
    return nullptr;
}
