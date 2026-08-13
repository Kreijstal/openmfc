// CMFCRibbonCategory — OpenMFC implementation.
// Sources: cbarcore.cpp, global_mfc-feature-17_impl.cpp, mfccore.cpp, ole_olectors_exports.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/CbarcoreSupport.h"
#include "detail/CMFCRibbonCategorySupport.h"
#include "detail/MfccoreSupport.h"

#include "openmfc/afxole.h"
#include "openmfc/afxmfc.h"

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif


























// Implementations this unit calls that are defined with their own class.
extern "C" void MS_ABI impl___1CMFCRibbonPanel__UEAA_XZ(void* pThis);

// Symbol: ??0CMFCRibbonCategory@@IEAA@XZ
extern "C" void* MS_ABI impl___0CMFCRibbonCategory__IEAA_XZ(void* pThis) {
    if (!pThis) return nullptr;
    std::lock_guard<std::mutex> lock(g_ribbonMutex);
    CMFCRibbonCategory* category = new(pThis) CMFCRibbonCategory();
    g_ribbonCategories[category] = RibbonCategoryState{};
    return category;
}
// Symbol: ??1CMFCRibbonCategory@@UEAA@XZ
extern "C" void MS_ABI impl___1CMFCRibbonCategory__UEAA_XZ(void* pThis) {
    auto* category = reinterpret_cast<CMFCRibbonCategory*>(pThis);
    if (!category) return;

    std::vector<CMFCRibbonPanel*> ownedPanels;
    {
        std::lock_guard<std::mutex> lock(g_ribbonMutex);
        if (auto it = g_ribbonCategories.find(category); it != g_ribbonCategories.end()) {
            ownedPanels.assign(it->second.ownedPanels.begin(), it->second.ownedPanels.end());
            g_ribbonCategories.erase(it);
        }
        for (auto& [_, barState] : g_ribbonBars) {
            if (barState.activeCategory == category) {
                barState.activeCategory = nullptr;
            }
            barState.ownedCategories.erase(category);
            auto& categories = barState.categories;
            categories.erase(std::remove(categories.begin(), categories.end(), category), categories.end());
        }
    }

    for (CMFCRibbonPanel* panel : ownedPanels) {
        if (panel) {
            impl___1CMFCRibbonPanel__UEAA_XZ(panel);
            ::operator delete(panel);
        }
    }

    category->~CMFCRibbonCategory();
}
// Symbol: ?AddPanel@CMFCRibbonCategory@@QEAAPEAVCMFCRibbonPanel@@PEB_WPEAUHICON__@@PEAUCRuntimeClass@@@Z
extern "C" CMFCRibbonPanel* MS_ABI impl__AddPanel_CMFCRibbonCategory__QEAAPEAVCMFCRibbonPanel__PEB_WPEAUHICON____PEAUCRuntimeClass___Z(
    CMFCRibbonCategory* pThis, const wchar_t* lpszLabel, HICON__* hIcon, CRuntimeClass*) {
    if (!pThis) return nullptr;

    auto* panel = new CMFCRibbonPanel(lpszLabel, hIcon);
    std::lock_guard<std::mutex> lock(g_ribbonMutex);
    auto& state = g_ribbonCategories[pThis];
    state.panels.push_back(panel);
    state.ownedPanels.insert(panel);
    g_ribbonPanels.try_emplace(panel);
    return panel;
}
// Symbol: ?GetPanel@CMFCRibbonCategory@@QEAAPEAVCMFCRibbonPanel@@H@Z
extern "C" CMFCRibbonPanel* MS_ABI impl__GetPanel_CMFCRibbonCategory__QEAAPEAVCMFCRibbonPanel__H_Z(
    CMFCRibbonCategory* pThis, int nIndex) {
    if (!pThis || nIndex < 0) return nullptr;
    std::lock_guard<std::mutex> lock(g_ribbonMutex);
    auto it = g_ribbonCategories.find(pThis);
    if (it == g_ribbonCategories.end()) return nullptr;
    const auto& panels = it->second.panels;
    if (nIndex >= static_cast<int>(panels.size())) return nullptr;
    return panels[static_cast<size_t>(nIndex)];
}
// Symbol: ?GetPanelCount@CMFCRibbonCategory@@QEBAHXZ
extern "C" int MS_ABI impl__GetPanelCount_CMFCRibbonCategory__QEBAHXZ(const CMFCRibbonCategory* pThis) {
    if (!pThis) return 0;
    std::lock_guard<std::mutex> lock(g_ribbonMutex);
    auto it = g_ribbonCategories.find(const_cast<CMFCRibbonCategory*>(pThis));
    return (it == g_ribbonCategories.end()) ? 0 : static_cast<int>(it->second.panels.size());
}
// Symbol: ?CreateObject@CMFCRibbonCategory@@SAPEAVCObject@@XZ
extern "C" CObject* MS_ABI impl__CreateObject_CMFCRibbonCategory__SAPEAVCObject__XZ() {
    return CMFCRibbonCategory::GetThisClass()->CreateObject();
}
// Symbol: ?GetRuntimeClass@CMFCRibbonCategory@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CMFCRibbonCategory__UEBAPEAUCRuntimeClass__XZ(
    const CMFCRibbonCategory* pThis) {
    return CMFCRibbonCategory::GetThisClass();
}
// Symbol: ?GetThisClass@CMFCRibbonCategory@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMFCRibbonCategory__SAPEAUCRuntimeClass__XZ() {
    return CMFCRibbonCategory::GetThisClass();
}
// CMFCRibbonCategory::AddHidden(CMFCRibbonBaseElement*) -- retail appends the
// element to the hidden-elements CArray at +0xb00/+0xb08/+0xb10. OpenMFC has
// no hidden-element state (the member array is an opaque blob nothing
// populates), so the element is not retained.
// TODO(clean-room): transcribed partially -- hidden elements are not modeled.
// Symbol: ?AddHidden@CMFCRibbonCategory@@QEAAXPEAVCMFCRibbonBaseElement@@@Z
extern "C" void MS_ABI impl__AddHidden_CMFCRibbonCategory__QEAAXPEAVCMFCRibbonBaseElement___Z(
    CMFCRibbonCategory* pThis, CMFCRibbonBaseElement* /*pElem*/)
{
    if (!pThis) return;
}
// CMFCRibbonCategory::CleanUpSizes() -- retail calls CleanUpSizes() on every
// panel and resets the category's cached layout sizes (+0xb0, +0x770) to -1.
// Panel sizes and the cache members are not modeled.
// TODO(clean-room): transcribed partially -- panel/category size state is not
// modeled.
// Symbol: ?CleanUpSizes@CMFCRibbonCategory@@IEAAXXZ
extern "C" void MS_ABI impl__CleanUpSizes_CMFCRibbonCategory__IEAAXXZ(
    CMFCRibbonCategory* pThis)
{
    if (!pThis) return;
}
// CMFCRibbonCategory::CommonInit(CMFCRibbonBar*, const wchar_t*, unsigned int,
// unsigned int, CSize, unsigned int) -- retail zeroes the size cache, writes
// the name CString (+0xd0), stores the parent bar (+0x760) and image resource
// IDs (+0x838/+0x9d0) and the tab color (+0xb28), then builds the scroll
// buttons. OpenMFC's CMFCRibbonCategory ctor already initializes the object
// and m_strName is not reachable from a free function (it is protected), so
// the body keeps the type-correct no-op.
// TODO(clean-room): transcribed partially -- category init state beyond the
// declared members is not modeled.
// Symbol: ?CommonInit@CMFCRibbonCategory@@IEAAXPEAVCMFCRibbonBar@@PEB_WIIVCSize@@2@Z
extern "C" void MS_ABI impl__CommonInit_CMFCRibbonCategory__IEAAXPEAVCMFCRibbonBar__PEB_WIIVCSize__2_Z(
    CMFCRibbonCategory* pThis, CMFCRibbonBar* /*pParentRibbonBar*/,
    const wchar_t* /*lpszName*/, unsigned int /*uiSmallImageResID*/,
    unsigned int /*uiLargeImageResID*/, CSize /*sizeSmallImage*/,
    unsigned int /*uiAnimLargeImageResID*/)
{
    if (!pThis) return;
}
// CMFCRibbonCategory::CopyFrom(CMFCRibbonCategory&) -- retail copies the base
// object, ~20 scalar members, clones every panel via its runtime class
// (CreateObject + panel CopyFrom) and copies the element/group arrays.
// OpenMFC declares only m_strName (protected, not writable from here) plus
// padding, and the panel list lives in the side table with no clone support,
// so nothing can be copied safely.
// TODO(clean-room): transcribed partially -- panel cloning and member copies
// are not modeled.
// Symbol: ?CopyFrom@CMFCRibbonCategory@@UEAAXAEAV1@@Z
extern "C" void MS_ABI impl__CopyFrom_CMFCRibbonCategory__UEAAXAEAV1__Z(
    CMFCRibbonCategory* pThis, CMFCRibbonCategory& /*src*/)
{
    if (!pThis) return;
}
// CMFCRibbonCategory::EnsureVisible(CMFCRibbonButton*) -- retail walks the
// panel hierarchy, hit-tests the button's rect against the visible viewport
// and scrolls the category accordingly (also unwinding focus state). Requires
// the element geometry/scroll model OpenMFC does not have.
// TODO(clean-room): transcribed partially -- element geometry and category
// scrolling are not modeled.
// Symbol: ?EnsureVisible@CMFCRibbonCategory@@IEAAXPEAVCMFCRibbonButton@@@Z
extern "C" void MS_ABI impl__EnsureVisible_CMFCRibbonCategory__IEAAXPEAVCMFCRibbonButton___Z(
    CMFCRibbonCategory* pThis, CMFCRibbonButton* /*pButton*/)
{
    if (!pThis) return;
}
// CMFCRibbonCategory::FindByData(unsigned __int64, int) const -- retail scans
// each panel (calling panel FindByData) and, when nIndexToStart == 0, the
// hidden list first, matching an element's data member (+0x110). OpenMFC's
// CMFCRibbonBaseElement has no data member/accessor, so no element can ever
// match.
// TODO(clean-room): transcribed partially -- element data members are not
// modeled.
// Symbol: ?FindByData@CMFCRibbonCategory@@QEBAPEAVCMFCRibbonBaseElement@@_KH@Z
extern "C" CMFCRibbonBaseElement* MS_ABI impl__FindByData_CMFCRibbonCategory__QEBAPEAVCMFCRibbonBaseElement___KH_Z(
    const CMFCRibbonCategory* pThis, unsigned long long /*dwData*/,
    int /*nIndexToStart*/)
{
    if (!pThis) return nullptr;
    return nullptr;
}
// CMFCRibbonCategory::FindByID(unsigned int, int) const -- retail scans the
// hidden list first (when nIndexToStart == 0) and then each panel's elements
// for the first element whose ID matches. Implemented over the side tables;
// hidden elements are not modeled and never match.
// TODO(clean-room): partially transcribed -- hidden elements are not modeled.
// Symbol: ?FindByID@CMFCRibbonCategory@@QEBAPEAVCMFCRibbonBaseElement@@IH@Z
extern "C" CMFCRibbonBaseElement* MS_ABI impl__FindByID_CMFCRibbonCategory__QEBAPEAVCMFCRibbonBaseElement__IH_Z(
    const CMFCRibbonCategory* pThis, unsigned int nID, int /*nIndexToStart*/)
{
    if (!pThis) return nullptr;

    std::lock_guard<std::mutex> lock(RibbonMutex());
    auto it = RibbonCategoryStates().find(pThis);
    if (it == RibbonCategoryStates().end()) return nullptr;

    for (CMFCRibbonPanel* pPanel : it->second.panels) {
        if (!pPanel) continue;
        auto panelIt = RibbonPanelStates().find(pPanel);
        if (panelIt == RibbonPanelStates().end()) continue;
        for (CMFCRibbonBaseElement* pElem : panelIt->second.elements) {
            if (pElem && pElem->GetID() == nID) {
                return pElem;
            }
        }
    }
    return nullptr;
}
// CMFCRibbonCategory::FindPanelWithElem(const CMFCRibbonBaseElement*) --
// retail calls a panel member test on each panel (0x18011b640) and returns
// the first panel that contains the element. Implemented over the side-table
// element lists, which is where OpenMFC keeps panel membership (see
// CMFCRibbonPanel::Add in cbarcore.cpp).
// Symbol: ?FindPanelWithElem@CMFCRibbonCategory@@QEAAPEAVCMFCRibbonPanel@@PEBVCMFCRibbonBaseElement@@@Z
extern "C" CMFCRibbonPanel* MS_ABI impl__FindPanelWithElem_CMFCRibbonCategory__QEAAPEAVCMFCRibbonPanel__PEBVCMFCRibbonBaseElement___Z(
    CMFCRibbonCategory* pThis, const CMFCRibbonBaseElement* pElement)
{
    if (!pThis || !pElement) return nullptr;

    std::lock_guard<std::mutex> lock(RibbonMutex());
    auto it = RibbonCategoryStates().find(pThis);
    if (it == RibbonCategoryStates().end()) return nullptr;

    for (CMFCRibbonPanel* pPanel : it->second.panels) {
        if (!pPanel) continue;
        auto panelIt = RibbonPanelStates().find(pPanel);
        if (panelIt == RibbonPanelStates().end()) continue;
        const auto& elements = panelIt->second.elements;
        if (std::find(elements.begin(), elements.end(), pElement) != elements.end()) {
            return pPanel;
        }
    }
    return nullptr;
}
// CMFCRibbonCategory::GetDroppedDown() -- retail asks each panel for its
// dropped-down element (0x18011b960) and returns the first hit. Panel
// drop-down state is not modeled.
// TODO(clean-room): transcribed partially -- panel dropped-down state is not
// modeled.
// Symbol: ?GetDroppedDown@CMFCRibbonCategory@@QEAAPEAVCMFCRibbonBaseElement@@XZ
extern "C" CMFCRibbonBaseElement* MS_ABI impl__GetDroppedDown_CMFCRibbonCategory__QEAAPEAVCMFCRibbonBaseElement__XZ(
    CMFCRibbonCategory* pThis)
{
    if (!pThis) return nullptr;
    return nullptr;
}
// CMFCRibbonCategory::GetElements(CArray<CMFCRibbonBaseElement*,
// CMFCRibbonBaseElement*>&) -- retail clears the array then appends every
// panel element followed by every hidden element. Implemented over the side
// tables; hidden elements are not modeled.
// TODO(clean-room): partially transcribed -- hidden elements are not modeled.
// Symbol: ?GetElements@CMFCRibbonCategory@@QEAAXAEAV?$CArray@PEAVCMFCRibbonBaseElement@@PEAV1@@@@Z
extern "C" void MS_ABI impl__GetElements_CMFCRibbonCategory__QEAAXAEAV__CArray_PEAVCMFCRibbonBaseElement__PEAV1____Z(
    CMFCRibbonCategory* pThis,
    CArray<CMFCRibbonBaseElement*, CMFCRibbonBaseElement*>& arElements)
{
    if (!pThis) return;
    arElements.RemoveAll();

    std::lock_guard<std::mutex> lock(RibbonMutex());
    auto it = RibbonCategoryStates().find(pThis);
    if (it == RibbonCategoryStates().end()) return;

    for (CMFCRibbonPanel* pPanel : it->second.panels) {
        if (!pPanel) continue;
        auto panelIt = RibbonPanelStates().find(pPanel);
        if (panelIt == RibbonPanelStates().end()) continue;
        for (CMFCRibbonBaseElement* pElem : panelIt->second.elements) {
            if (pElem) {
                arElements.Add(pElem);
            }
        }
    }
}
// CMFCRibbonCategory::GetElementsByID(unsigned int, CArray<...>&) -- retail
// appends (without clearing, matching the panel helper contract) every panel
// element whose ID matches, then the hidden elements with a matching ID.
// Implemented over the side tables; hidden elements are not modeled.
// TODO(clean-room): partially transcribed -- hidden elements are not modeled.
// Symbol: ?GetElementsByID@CMFCRibbonCategory@@QEAAXIAEAV?$CArray@PEAVCMFCRibbonBaseElement@@PEAV1@@@@Z
extern "C" void MS_ABI impl__GetElementsByID_CMFCRibbonCategory__QEAAXIAEAV__CArray_PEAVCMFCRibbonBaseElement__PEAV1____Z(
    CMFCRibbonCategory* pThis, unsigned int uiCmd,
    CArray<CMFCRibbonBaseElement*, CMFCRibbonBaseElement*>& arElements)
{
    if (!pThis) return;

    std::lock_guard<std::mutex> lock(RibbonMutex());
    auto it = RibbonCategoryStates().find(pThis);
    if (it == RibbonCategoryStates().end()) return;

    for (CMFCRibbonPanel* pPanel : it->second.panels) {
        if (!pPanel) continue;
        auto panelIt = RibbonPanelStates().find(pPanel);
        if (panelIt == RibbonPanelStates().end()) continue;
        for (CMFCRibbonBaseElement* pElem : panelIt->second.elements) {
            if (pElem && pElem->GetID() == uiCmd) {
                arElements.Add(pElem);
            }
        }
    }
}
// CMFCRibbonCategory::GetFirstVisibleElement() const -- retail checks the
// first panel's visibility and returns its first element (or the panel's
// element collection). OpenMFC tracks no visibility flags, so the side-table
// approximation returns the first non-null element of the first panel that
// has any elements.
// TODO(clean-room): transcribed partially -- panel visibility is not modeled.
// Symbol: ?GetFirstVisibleElement@CMFCRibbonCategory@@QEBAPEAVCMFCRibbonBaseElement@@XZ
extern "C" CMFCRibbonBaseElement* MS_ABI impl__GetFirstVisibleElement_CMFCRibbonCategory__QEBAPEAVCMFCRibbonBaseElement__XZ(
    const CMFCRibbonCategory* pThis)
{
    if (!pThis) return nullptr;

    std::lock_guard<std::mutex> lock(RibbonMutex());
    auto it = RibbonCategoryStates().find(pThis);
    if (it == RibbonCategoryStates().end()) return nullptr;

    for (CMFCRibbonPanel* pPanel : it->second.panels) {
        if (!pPanel) continue;
        auto panelIt = RibbonPanelStates().find(pPanel);
        if (panelIt == RibbonPanelStates().end()) continue;
        for (CMFCRibbonBaseElement* pElem : panelIt->second.elements) {
            if (pElem) {
                return pElem;
            }
        }
    }
    return nullptr;
}
// CMFCRibbonCategory::GetFocused() -- retail asks each panel for its focused
// element (0x18011cee0). Panel focus state is not modeled.
// TODO(clean-room): transcribed partially -- panel focus state is not modeled.
// Symbol: ?GetFocused@CMFCRibbonCategory@@QEAAPEAVCMFCRibbonBaseElement@@XZ
extern "C" CMFCRibbonBaseElement* MS_ABI impl__GetFocused_CMFCRibbonCategory__QEAAPEAVCMFCRibbonBaseElement__XZ(
    CMFCRibbonCategory* pThis)
{
    if (!pThis) return nullptr;
    return nullptr;
}
// CMFCRibbonCategory::GetHighlighted() -- retail reads each panel's
// highlighted element (panel +0x3e8). Panel highlight state is not modeled.
// TODO(clean-room): transcribed partially -- panel highlight state is not
// modeled.
// Symbol: ?GetHighlighted@CMFCRibbonCategory@@QEAAPEAVCMFCRibbonBaseElement@@XZ
extern "C" CMFCRibbonBaseElement* MS_ABI impl__GetHighlighted_CMFCRibbonCategory__QEAAPEAVCMFCRibbonBaseElement__XZ(
    CMFCRibbonCategory* pThis)
{
    if (!pThis) return nullptr;
    return nullptr;
}
// CMFCRibbonCategory::GetImageSize(int) const -- retail returns
// m_sizeSmallImage (+0x838) for nImageSizeType == 0 and m_sizeLargeImage
// (+0x9d0) otherwise. Neither member is modeled, so an empty size is
// returned.
// TODO(clean-room): transcribed partially -- category image sizes are not
// modeled.
// Symbol: ?GetImageSize@CMFCRibbonCategory@@QEBA?AVCSize@@H@Z
extern "C" void MS_ABI impl__GetImageSize_CMFCRibbonCategory__QEBA_AVCSize__H_Z(
    void* pRet, const CMFCRibbonCategory* pThis, int /*nImageSizeType*/)
{
    if (!pThis || !pRet) return;
    new(pRet) CSize(0, 0);
}
// CMFCRibbonCategory::GetItemIDsList(CList<UINT, UINT>&, int) const --
// retail, when nCategoryIndex == 0, appends every panel element ID via the
// panel's GetItemIDsList, and always appends the hidden-element IDs. The
// list is not cleared (callers own the fill contract). Implemented over the
// side tables; hidden elements are not modeled.
// TODO(clean-room): partially transcribed -- hidden elements are not modeled.
// Symbol: ?GetItemIDsList@CMFCRibbonCategory@@QEBAXAEAV?$CList@II@@H@Z
extern "C" void MS_ABI impl__GetItemIDsList_CMFCRibbonCategory__QEBAXAEAV__CList_II__H_Z(
    const CMFCRibbonCategory* pThis, CList<unsigned int, unsigned int>& lstIDs,
    int nCategoryIndex)
{
    if (!pThis) return;

    std::lock_guard<std::mutex> lock(RibbonMutex());
    auto it = RibbonCategoryStates().find(pThis);
    if (it == RibbonCategoryStates().end()) return;

    if (nCategoryIndex == 0) {
        for (CMFCRibbonPanel* pPanel : it->second.panels) {
            if (!pPanel) continue;
            auto panelIt = RibbonPanelStates().find(pPanel);
            if (panelIt == RibbonPanelStates().end()) continue;
            for (CMFCRibbonBaseElement* pElem : panelIt->second.elements) {
                if (pElem) {
                    lstIDs.AddTail(pElem->GetID());
                }
            }
        }
    }
}
// CMFCRibbonCategory::GetLastVisibleElement() const -- retail scans the
// panels backwards and returns the last element of the last visible panel.
// OpenMFC tracks no visibility flags, so the side-table approximation returns
// the last non-null element of the last panel that has any elements.
// TODO(clean-room): transcribed partially -- panel visibility is not modeled.
// Symbol: ?GetLastVisibleElement@CMFCRibbonCategory@@QEBAPEAVCMFCRibbonBaseElement@@XZ
extern "C" CMFCRibbonBaseElement* MS_ABI impl__GetLastVisibleElement_CMFCRibbonCategory__QEBAPEAVCMFCRibbonBaseElement__XZ(
    const CMFCRibbonCategory* pThis)
{
    if (!pThis) return nullptr;

    std::lock_guard<std::mutex> lock(RibbonMutex());
    auto it = RibbonCategoryStates().find(pThis);
    if (it == RibbonCategoryStates().end()) return nullptr;

    CMFCRibbonBaseElement* pFound = nullptr;
    for (auto pPanelIt = it->second.panels.rbegin(); pPanelIt != it->second.panels.rend(); ++pPanelIt) {
        CMFCRibbonPanel* pPanel = *pPanelIt;
        if (!pPanel) continue;
        auto panelIt = RibbonPanelStates().find(pPanel);
        if (panelIt == RibbonPanelStates().end()) continue;
        const auto& elements = panelIt->second.elements;
        for (auto elemIt = elements.rbegin(); elemIt != elements.rend(); ++elemIt) {
            if (*elemIt) {
                pFound = *elemIt;
                return pFound;
            }
        }
    }
    return nullptr;
}
// CMFCRibbonCategory::GetMaxHeight(CDC*) -- retail returns the maximum of the
// panels' GetMaxHeight results plus the category rect height (+7). Panel
// heights and the category rect are not modeled.
// TODO(clean-room): transcribed partially -- panel layout heights are not
// modeled.
// Symbol: ?GetMaxHeight@CMFCRibbonCategory@@QEAAHPEAVCDC@@@Z
extern "C" int MS_ABI impl__GetMaxHeight_CMFCRibbonCategory__QEAAHPEAVCDC___Z(
    CMFCRibbonCategory* pThis, CDC* /*pDC*/)
{
    if (!pThis) return 0;
    return 0;
}
// CMFCRibbonCategory::GetMinWidth(CDC*) -- retail starts at 2 and adds each
// panel's GetMinWidth result + 2. Panel min widths are not modeled, so the
// empty-category floor (2) is returned.
// TODO(clean-room): transcribed partially -- panel layout widths are not
// modeled.
// Symbol: ?GetMinWidth@CMFCRibbonCategory@@IEAAHPEAVCDC@@@Z
extern "C" int MS_ABI impl__GetMinWidth_CMFCRibbonCategory__IEAAHPEAVCDC___Z(
    CMFCRibbonCategory* pThis, CDC* /*pDC*/)
{
    if (!pThis) return 0;
    return 2;
}
// CMFCRibbonCategory::GetPanelFromPoint(CPoint) const -- retail hit-tests
// each panel's rect (+0x10c) against pt and returns the panel. OpenMFC keeps
// no panel rects.
// TODO(clean-room): transcribed partially -- panel rects are not modeled.
// Symbol: ?GetPanelFromPoint@CMFCRibbonCategory@@QEBAPEAVCMFCRibbonPanel@@VCPoint@@@Z
extern "C" CMFCRibbonPanel* MS_ABI impl__GetPanelFromPoint_CMFCRibbonCategory__QEBAPEAVCMFCRibbonPanel__VCPoint___Z(
    const CMFCRibbonCategory* pThis, CPoint /*pt*/)
{
    if (!pThis) return nullptr;
    return nullptr;
}
// CMFCRibbonCategory::GetPanelIndex(const CMFCRibbonPanel*) const -- retail
// scans the panel CArray for the pointer and returns its index or -1.
// Implemented over the side-table panel list.
// Symbol: ?GetPanelIndex@CMFCRibbonCategory@@QEBAHPEBVCMFCRibbonPanel@@@Z
extern "C" int MS_ABI impl__GetPanelIndex_CMFCRibbonCategory__QEBAHPEBVCMFCRibbonPanel___Z(
    const CMFCRibbonCategory* pThis, const CMFCRibbonPanel* pPanel)
{
    if (!pThis || !pPanel) return -1;

    std::lock_guard<std::mutex> lock(RibbonMutex());
    auto it = RibbonCategoryStates().find(pThis);
    if (it == RibbonCategoryStates().end()) return -1;

    const auto& panels = it->second.panels;
    for (size_t i = 0; i < panels.size(); i++) {
        if (panels[i] == pPanel) {
            return static_cast<int>(i);
        }
    }
    return -1;
}
// CMFCRibbonCategory::GetParentButton() const -- retail walks the category's
// parent scroll-button chain (+0x768 -> button +0x40 -> parent) and returns
// its parent button. The scroll-button model is not modeled.
// TODO(clean-room): transcribed partially -- the category scroll button
// hierarchy is not modeled.
// Symbol: ?GetParentButton@CMFCRibbonCategory@@QEBAPEAVCMFCRibbonBaseElement@@XZ
extern "C" CMFCRibbonBaseElement* MS_ABI impl__GetParentButton_CMFCRibbonCategory__QEBAPEAVCMFCRibbonBaseElement__XZ(
    const CMFCRibbonCategory* pThis)
{
    if (!pThis) return nullptr;
    return nullptr;
}
CMFCRibbonCategory::CMFCRibbonCategory(const wchar_t* lpszName, UINT, UINT) {
    if (lpszName) m_strName = lpszName;
    memset(_ribboncategory_padding, 0, sizeof(_ribboncategory_padding));
    g_ribbonCategoryStates[this];
}
CMFCRibbonCategory::~CMFCRibbonCategory() {
    ClearRibbonCategoryState(this);
}
CMFCRibbonPanel* CMFCRibbonCategory::AddPanel(const wchar_t* lpszLabel, HICON hIcon, CRuntimeClass*) {
    auto* panel = new (std::nothrow) CMFCRibbonPanel(lpszLabel, hIcon);
    if (!panel) return nullptr;

    RibbonCategoryState& state = g_ribbonCategoryStates[this];
    state.panels.push_back(panel);
    state.ownedPanels.insert(panel);
    return panel;
}
void CMFCRibbonCategory::AddPanel(CMFCRibbonPanel* pPanel) {
    if (!pPanel) return;
    RibbonCategoryState& state = g_ribbonCategoryStates[this];
    if (std::find(state.panels.begin(), state.panels.end(), pPanel) == state.panels.end()) {
        state.panels.push_back(pPanel);
    }
    state.ownedPanels.insert(pPanel);
    g_ribbonPanelStates[pPanel];
}
int CMFCRibbonCategory::GetPanelCount() const {
    auto it = g_ribbonCategoryStates.find(this);
    return it == g_ribbonCategoryStates.end() ? 0 : static_cast<int>(it->second.panels.size());
}
CMFCRibbonPanel* CMFCRibbonCategory::GetPanel(int nIndex) const {
    auto it = g_ribbonCategoryStates.find(this);
    if (it == g_ribbonCategoryStates.end()) return nullptr;
    if (nIndex < 0 || nIndex >= static_cast<int>(it->second.panels.size())) return nullptr;
    return it->second.panels[static_cast<size_t>(nIndex)];
}
CString CMFCRibbonCategory::GetName() const { return m_strName; }
// Symbol: ??0CMFCRibbonCategory@@IEAA@PEAVCMFCRibbonBar@@PEB_WIIVCSize@@2@Z
// CMFCRibbonCategory::CMFCRibbonCategory()
extern "C" void* MS_ABI impl___0CMFCRibbonCategory__IEAA_PEAVCMFCRibbonBar__PEB_WIIVCSize__2_Z(
    void* /*p0*/, const wchar_t* p1, unsigned int p2, unsigned int p3, void* p4, unsigned int p5) {
    (void)p1;
    (void)p2;
    (void)p3;
    (void)p4;
    (void)p5;
    return new CMFCRibbonCategory();
}
// Symbol: ?accHitTest@CMFCRibbonCategory@@UEAAJJJPEAUtagVARIANT@@@Z
extern "C" long MS_ABI impl__accHitTest_CMFCRibbonCategory__UEAAJJJPEAUtagVARIANT___Z(long p0, long p1, void* /*struct*/* p2) {
    return 0;
}

// Symbol: ?accLocation@CMFCRibbonCategory@@UEAAJPEAJ000UtagVARIANT@@@Z
extern "C" long MS_ABI impl__accLocation_CMFCRibbonCategory__UEAAJPEAJ000UtagVARIANT___Z(long* p0, long* p1, long* p2, long* p3, void* /*struct*/ p4) {
    return 0;
}

// Symbol: ?accNavigate@CMFCRibbonCategory@@UEAAJJUtagVARIANT@@PEAU2@@Z
extern "C" long MS_ABI impl__accNavigate_CMFCRibbonCategory__UEAAJJUtagVARIANT__PEAU2__Z(long p0, void* /*struct*/ p1, void* /*struct*/* p2) {
    return 0;
}

// Symbol: ?get_accChild@CMFCRibbonCategory@@UEAAJUtagVARIANT@@PEAPEAUIDispatch@@@Z
extern "C" long MS_ABI impl__get_accChild_CMFCRibbonCategory__UEAAJUtagVARIANT__PEAPEAUIDispatch___Z(void* /*struct*/ p0, void* /*struct*/** p1) {
    return 0;
}

// Symbol: ?get_accChildCount@CMFCRibbonCategory@@UEAAJPEAJ@Z
extern "C" long MS_ABI impl__get_accChildCount_CMFCRibbonCategory__UEAAJPEAJ_Z(long* p0) {
    return 0;
}

// Symbol: ?get_accParent@CMFCRibbonCategory@@UEAAJPEAPEAUIDispatch@@@Z
extern "C" long MS_ABI impl__get_accParent_CMFCRibbonCategory__UEAAJPEAPEAUIDispatch___Z(void* /*struct*/** p0) {
    return 0;
}

// Symbol: ?GetTextTopLine@CMFCRibbonCategory@@QEBAHXZ
extern "C" int MS_ABI impl__GetTextTopLine_CMFCRibbonCategory__QEBAHXZ() {
    return 0;
}

// Symbol: ?GetVisibleElements@CMFCRibbonCategory@@QEAAXAEAV?$CArray@PEAVCMFCRibbonBaseElement@@PEAV1@@@@Z
extern "C" void MS_ABI impl__GetVisibleElements_CMFCRibbonCategory__QEAAXAEAV__CArray_PEAVCMFCRibbonBaseElement__PEAV1____Z(void* /*class*/* p0, void* /*class*/* p1) {}

// Symbol: ?HighlightPanel@CMFCRibbonCategory@@QEAAPEAVCMFCRibbonPanel@@PEAV2@VCPoint@@@Z
extern "C" void* MS_ABI impl__HighlightPanel_CMFCRibbonCategory__QEAAPEAVCMFCRibbonPanel__PEAV2_VCPoint___Z(void* /*class*/* p0) {
    return nullptr;
}

// Symbol: ?HitTest@CMFCRibbonCategory@@QEBAPEAVCMFCRibbonBaseElement@@VCPoint@@H@Z
extern "C" void* MS_ABI impl__HitTest_CMFCRibbonCategory__QEBAPEAVCMFCRibbonBaseElement__VCPoint__H_Z(void* /*class*/ p0, int p1) {
    return nullptr;
}

// Symbol: ?HitTestEx@CMFCRibbonCategory@@QEBAHVCPoint@@@Z
extern "C" int MS_ABI impl__HitTestEx_CMFCRibbonCategory__QEBAHVCPoint___Z(void* /*class*/ p0) {
    return 0;
}

// Symbol: ?HitTestScrollButtons@CMFCRibbonCategory@@QEBAPEAVCMFCRibbonBaseElement@@VCPoint@@@Z
extern "C" void* MS_ABI impl__HitTestScrollButtons_CMFCRibbonCategory__QEBAPEAVCMFCRibbonBaseElement__VCPoint___Z(void* /*class*/ p0) {
    return nullptr;
}

// Symbol: ?IsWindows7Look@CMFCRibbonCategory@@QEBAHXZ
extern "C" int MS_ABI impl__IsWindows7Look_CMFCRibbonCategory__QEBAHXZ() {
    return 0;
}

// Symbol: ?NormalizeFloatingRect@CMFCRibbonCategory@@AEAAXPEAVCMFCRibbonBar@@AEAVCRect@@@Z
extern "C" void MS_ABI impl__NormalizeFloatingRect_CMFCRibbonCategory__AEAAXPEAVCMFCRibbonBar__AEAVCRect___Z(void* /*class*/* p0, void* /*class*/* p1) {}

// Symbol: ?NotifyControlCommand@CMFCRibbonCategory@@UEAAHHH_K_J@Z
extern "C" int MS_ABI impl__NotifyControlCommand_CMFCRibbonCategory__UEAAHHH_K_J_Z(int p0, int p1, unsigned __int64 p2, __int64 p3) {
    return 0;
}

// Symbol: ?OnCancelMode@CMFCRibbonCategory@@UEAAXXZ
extern "C" void MS_ABI impl__OnCancelMode_CMFCRibbonCategory__UEAAXXZ() {}

// Symbol: ?OnDraw@CMFCRibbonCategory@@UEAAXPEAVCDC@@@Z
extern "C" void MS_ABI impl__OnDraw_CMFCRibbonCategory__UEAAXPEAVCDC___Z(void* /*class*/* p0) {}

// Symbol: ?OnDrawImage@CMFCRibbonCategory@@UEAAHPEAVCDC@@VCRect@@PEAVCMFCRibbonBaseElement@@HHH@Z
extern "C" int MS_ABI impl__OnDrawImage_CMFCRibbonCategory__UEAAHPEAVCDC__VCRect__PEAVCMFCRibbonBaseElement__HHH_Z(void* /*class*/* p0, void* /*class*/ p1, void* /*class*/* p2, int p3, int p4, int p5) {
    return 0;
}

// Symbol: ?OnKey@CMFCRibbonCategory@@UEAAHI@Z
extern "C" int MS_ABI impl__OnKey_CMFCRibbonCategory__UEAAHI_Z(unsigned int p0) {
    return 0;
}

// Symbol: ?OnLButtonDown@CMFCRibbonCategory@@UEAAPEAVCMFCRibbonBaseElement@@VCPoint@@@Z
extern "C" void* MS_ABI impl__OnLButtonDown_CMFCRibbonCategory__UEAAPEAVCMFCRibbonBaseElement__VCPoint___Z(void* /*class*/ p0) {
    return nullptr;
}

// Symbol: ?OnLButtonUp@CMFCRibbonCategory@@UEAAXVCPoint@@@Z
extern "C" void MS_ABI impl__OnLButtonUp_CMFCRibbonCategory__UEAAXVCPoint___Z(void* /*class*/ p0) {}

// Symbol: ?OnMouseMove@CMFCRibbonCategory@@UEAAXVCPoint@@@Z
extern "C" void MS_ABI impl__OnMouseMove_CMFCRibbonCategory__UEAAXVCPoint___Z(void* /*class*/ p0) {}

// Symbol: ?OnRTLChanged@CMFCRibbonCategory@@UEAAXH@Z
extern "C" void MS_ABI impl__OnRTLChanged_CMFCRibbonCategory__UEAAXH_Z(int p0) {}

// Symbol: ?OnScrollHorz@CMFCRibbonCategory@@UEAAHHH@Z
extern "C" int MS_ABI impl__OnScrollHorz_CMFCRibbonCategory__UEAAHHH_Z(int p0, int p1) {
    return 0;
}

// Symbol: ?OnSetAccData@CMFCRibbonCategory@@UEAAHJ@Z
extern "C" int MS_ABI impl__OnSetAccData_CMFCRibbonCategory__UEAAHJ_Z(long p0) {
    return 0;
}

// Symbol: ?OnUpdateCmdUI@CMFCRibbonCategory@@UEAAXPEAVCMFCRibbonCmdUI@@PEAVCFrameWnd@@H@Z
extern "C" void MS_ABI impl__OnUpdateCmdUI_CMFCRibbonCategory__UEAAXPEAVCMFCRibbonCmdUI__PEAVCFrameWnd__H_Z(void* /*class*/* p0, void* /*class*/* p1, int p2) {}

// Symbol: ?RecalcLayout@CMFCRibbonCategory@@UEAAXPEAVCDC@@@Z
extern "C" void MS_ABI impl__RecalcLayout_CMFCRibbonCategory__UEAAXPEAVCDC___Z(void* /*class*/* p0) {}

// Symbol: ?RecalcPanelWidths@CMFCRibbonCategory@@IEAAXPEAVCDC@@@Z
extern "C" void MS_ABI impl__RecalcPanelWidths_CMFCRibbonCategory__IEAAXPEAVCDC___Z(void* /*class*/* p0) {}

// Symbol: ?RemovePanel@CMFCRibbonCategory@@QEAAHHH@Z
extern "C" int MS_ABI impl__RemovePanel_CMFCRibbonCategory__QEAAHHH_Z(int p0, int p1) {
    return 0;
}

// Symbol: ?ReposPanels@CMFCRibbonCategory@@UEAAXPEAVCDC@@@Z
extern "C" void MS_ABI impl__ReposPanels_CMFCRibbonCategory__UEAAXPEAVCDC___Z(void* /*class*/* p0) {}

// Symbol: ?ResetPanelsLayout@CMFCRibbonCategory@@IEAAXXZ
extern "C" void MS_ABI impl__ResetPanelsLayout_CMFCRibbonCategory__IEAAXXZ() {}

// Symbol: ?SetACCData@CMFCRibbonCategory@@UEAAHPEAVCWnd@@AEAVCAccessibilityData@@@Z
extern "C" int MS_ABI impl__SetACCData_CMFCRibbonCategory__UEAAHPEAVCWnd__AEAVCAccessibilityData___Z(void* /*class*/* p0, void* /*class*/* p1) {
    return 0;
}

// Symbol: ?SetActive@CMFCRibbonCategory@@IEAAXH@Z
extern "C" void MS_ABI impl__SetActive_CMFCRibbonCategory__IEAAXH_Z(int p0) {}

// Symbol: ?SetCollapseOrder@CMFCRibbonCategory@@QEAAXAEBV?$CArray@HH@@@Z
extern "C" void MS_ABI impl__SetCollapseOrder_CMFCRibbonCategory__QEAAXAEBV__CArray_HH___Z(const void* /*class*/* p0) {}

// Symbol: ?SetKeys@CMFCRibbonCategory@@QEAAXPEB_W@Z
extern "C" void MS_ABI impl__SetKeys_CMFCRibbonCategory__QEAAXPEB_W_Z(const wchar_t* p0) {}

// Symbol: ?SetName@CMFCRibbonCategory@@QEAAXPEB_W@Z
extern "C" void MS_ABI impl__SetName_CMFCRibbonCategory__QEAAXPEB_W_Z(const wchar_t* p0) {}

// Symbol: ?SetPanelsLayout@CMFCRibbonCategory@@IEAAHH@Z
extern "C" int MS_ABI impl__SetPanelsLayout_CMFCRibbonCategory__IEAAHH_Z(int p0) {
    return 0;
}

// Symbol: ?ShowElements@CMFCRibbonCategory@@IEAAXH@Z
extern "C" void MS_ABI impl__ShowElements_CMFCRibbonCategory__IEAAXH_Z(int p0) {}

// Symbol: ?ShowFloating@CMFCRibbonCategory@@IEAAXVCRect@@@Z
extern "C" void MS_ABI impl__ShowFloating_CMFCRibbonCategory__IEAAXVCRect___Z(void* /*class*/ p0) {}

// Symbol: ?UpdateScrollButtons@CMFCRibbonCategory@@IEAAXXZ
extern "C" void MS_ABI impl__UpdateScrollButtons_CMFCRibbonCategory__IEAAXXZ() {}
