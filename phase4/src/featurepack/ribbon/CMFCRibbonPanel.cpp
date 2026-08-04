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
