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
//=============================================================================
// CMFCRibbonCategory — remaining exports.
//
// Every body below was decoded from the retail mfc140 export (ordinal -> RVA
// -> objdump); the RVA is cited on each one.  Two facts shape the whole block:
//
//  * The retail category carries far more state than the clean-room class —
//    the bodies below reference members out past +0xb00.  The ones that
//    matter here are
//    the panel CArray (m_arPanels at +0x7a0 — its data pointer at +0x7a8 and
//    its size at +0x7b0), the parent ribbon bar (m_pParentRibbonBar at
//    +0x760), the category rect (m_rect at +0xc0), the per-child rect
//    (+0x7c..+0x88), the horizontal scroll offset (m_nScrollOffset at
//    +0x758), the two embedded scroll buttons (m_ScrollLeft at +0x268 and
//    m_ScrollRight at +0x4e0) plus the parent menu bar (m_pParentMenuBar at
//    +0x768), the embedded tab element (m_Tab at +0xd8), the base class's
//    CAccessibilityData block (+0x40), the two CMFCToolBarImages lists
//    (m_SmallImages at +0x7d0 and m_LargeImages at +0x968, each with its
//    image size at +0x68 into it, i.e. +0x838 / +0x9d0), the collapse-order
//    CArray (m_arCollapseOrder at +0x778) and the non-visible element CArray
//    (m_arElements at +0xb00, data +0xb08 / size +0xb10).  OpenMFC's clean-room
//    CMFCRibbonCategory declares only m_strName plus a 64-byte opaque pad, so
//    NONE of those members exists here.
//  * The one thing OpenMFC does model is the panel list, kept in the
//    ribbon_state side table (see detail/RibbonState.h and AddPanel above).
//    Wherever retail only needs m_arPanels, the body is transcribed in full
//    against that side table; everywhere else the function is left a
//    documented no-op rather than a guess.
//
// Where retail has a branch for "no parent ribbon bar" or "no image size",
// that branch is the only reachable state in OpenMFC and is called out as
// such.  Deviations from retail are named explicitly.
//=============================================================================

namespace {

// m_strName is protected and the exported entry points are free functions.
// A derived class may reach a protected base member through a pointer to
// itself; no object of this type is ever created (it exists only to name the
// access path).  Reading the name has a public accessor — CMFCRibbonCategory::
// GetName() — so only the setter needs this.
struct RibbonCategoryNameAccess : public CMFCRibbonCategory {
    static void Set(CMFCRibbonCategory* pCategory, const CString& str) {
        static_cast<RibbonCategoryNameAccess*>(pCategory)->m_strName = str;
    }
};

} // namespace

// CMFCRibbonCategory::accHitTest(long, long, VARIANT*) — retail (RVA 0xf1330)
// rejects a NULL output with E_INVALIDARG; then, if m_pParentRibbonBar (+0x760) is
// NULL or has no window (+0x40), it returns S_FALSE without touching the
// output.  Otherwise it sets the output to VT_I4/0, maps the screen point to
// bar client coordinates, and returns the first panel whose rect (+0x10c)
// contains it as a VT_DISPATCH.  OpenMFC has no m_pParentRibbonBar member at all,
// so the "no ribbon bar" branch is the only reachable one.
// TODO(clean-room): transcribed partially — the parent ribbon bar and the
// panel rects are not modeled.
// Symbol: ?accHitTest@CMFCRibbonCategory@@UEAAJJJPEAUtagVARIANT@@@Z
extern "C" long MS_ABI impl__accHitTest_CMFCRibbonCategory__UEAAJJJPEAUtagVARIANT___Z(
    CMFCRibbonCategory* pThis, long /*xLeft*/, long /*yTop*/, VARIANT* pvarChild)
{
    if (!pThis) return E_INVALIDARG;
    if (!pvarChild) return E_INVALIDARG;
    return S_FALSE;
}

// CMFCRibbonCategory::accLocation(long*, long*, long*, long*, VARIANT) —
// retail (RVA 0xf10f0) returns E_INVALIDARG unless all four output pointers
// are non-NULL.  It then returns S_OK *without writing anything* when
// varChild is not VT_I4, when varChild.lVal < 0, or when varChild.lVal == 0
// and m_pParentRibbonBar (+0x760) is NULL / has no window (+0x40).  Only two paths
// fill the outputs: lVal == 0 uses the category rect (+0xc0) mapped to screen
// coordinates, and lVal > 0 calls the virtual at vtable+0x160 and reads the
// child rect at +0x7c..+0x88.  Neither member is modeled here, so the
// "nothing written, S_OK" path is taken — which is exactly retail's own
// behaviour for lVal == 0 with no ribbon bar, and a deviation for lVal > 0.
// TODO(clean-room): transcribed partially — the category/child rects and the
// parent ribbon bar are not modeled.
// Symbol: ?accLocation@CMFCRibbonCategory@@UEAAJPEAJ000UtagVARIANT@@@Z
extern "C" long MS_ABI impl__accLocation_CMFCRibbonCategory__UEAAJPEAJ000UtagVARIANT___Z(
    CMFCRibbonCategory* pThis, long* pxLeft, long* pyTop, long* pcxWidth,
    long* pcyHeight, VARIANT /*varChild*/)
{
    if (!pThis) return E_INVALIDARG;
    if (!pxLeft || !pyTop || !pcxWidth || !pcyHeight) return E_INVALIDARG;
    return S_OK;
}

// CMFCRibbonCategory::accNavigate(long, VARIANT, VARIANT*) — transcribed in
// full from retail RVA 0xf1200.  The only category state it reads is the
// panel count (m_arPanels size at +0x7b0), which OpenMFC keeps in the ribbon
// state side table, so the whole control flow carries over:
//
//   pvarEnd->vt = VT_EMPTY;                        (written before any check)
//   if (varStart.vt != VT_I4) return E_INVALIDARG;
//   NAVDIR_LEFT(3) / NAVDIR_PREVIOUS(6):
//       lVal == 0 -> the parent bar's last tab (see below), else
//       --lVal; pvarEnd = VT_I4/lVal; > 0 ? S_OK : (VT_EMPTY, S_FALSE)
//   NAVDIR_RIGHT(4) / NAVDIR_NEXT(5):
//       lVal == 0 -> S_FALSE, else
//       ++lVal; pvarEnd = VT_I4/lVal; > panel count ? (VT_EMPTY, S_FALSE) : S_OK
//   NAVDIR_FIRSTCHILD(7): lVal != 0 ? S_FALSE : (VT_I4/1, S_OK)
//   NAVDIR_LASTCHILD(8):  lVal != 0 ? S_FALSE : (VT_I4/panel count, S_OK)
//   anything else: S_FALSE
//
// The one branch that cannot be reproduced is LEFT/PREVIOUS with lVal == 0:
// retail walks to m_pParentRibbonBar (+0x760), and when it is NULL or has no
// window (+0x40) returns S_FALSE — which is what happens here, since OpenMFC
// declares no ribbon-bar member.  With a bar it would hand back, as a
// VT_DISPATCH, the last element of the CArray inside the bar's m_TabElements
// group (data at +0x620, size at +0x628) — or the bar's m_Tabs group itself
// (+0x1a58) when that array is empty — via ExternalAddRef + GetInterface with
// the IID at 0x1802d7a98, which objdump reads as
// {00020400-0000-0000-C000-000000000046} = IID_IDispatch.
// Deviation: retail writes through pvarEnd with no NULL check; this guards it.
// Symbol: ?accNavigate@CMFCRibbonCategory@@UEAAJJUtagVARIANT@@PEAU2@@Z
extern "C" long MS_ABI impl__accNavigate_CMFCRibbonCategory__UEAAJJUtagVARIANT__PEAU2__Z(
    CMFCRibbonCategory* pThis, long navDir, VARIANT varStart, VARIANT* pvarEnd)
{
    if (!pThis || !pvarEnd) return E_INVALIDARG;

    pvarEnd->vt = 0 /*VT_EMPTY*/;
    if (varStart.vt != 3 /*VT_I4*/) return E_INVALIDARG;

    const long nPanels = static_cast<long>(
        impl__GetPanelCount_CMFCRibbonCategory__QEBAHXZ(pThis));

    switch (navDir) {
    case 3 /*NAVDIR_LEFT*/:
    case 6 /*NAVDIR_PREVIOUS*/: {
        long nChild = varStart.lVal;
        if (nChild == 0) return S_FALSE;   // retail: last tab of m_pParentRibbonBar
        nChild--;
        pvarEnd->vt = 3 /*VT_I4*/;
        pvarEnd->lVal = nChild;
        if (nChild > 0) return S_OK;
        pvarEnd->vt = 0 /*VT_EMPTY*/;
        return S_FALSE;
    }
    case 4 /*NAVDIR_RIGHT*/:
    case 5 /*NAVDIR_NEXT*/: {
        long nChild = varStart.lVal;
        if (nChild == 0) return S_FALSE;
        nChild++;
        pvarEnd->vt = 3 /*VT_I4*/;
        pvarEnd->lVal = nChild;
        if (nChild > nPanels) {
            pvarEnd->vt = 0 /*VT_EMPTY*/;
            return S_FALSE;
        }
        return S_OK;
    }
    case 7 /*NAVDIR_FIRSTCHILD*/:
        if (varStart.lVal != 0) return S_FALSE;
        pvarEnd->vt = 3 /*VT_I4*/;
        pvarEnd->lVal = 1;
        return S_OK;
    case 8 /*NAVDIR_LASTCHILD*/:
        if (varStart.lVal != 0) return S_FALSE;
        pvarEnd->vt = 3 /*VT_I4*/;
        pvarEnd->lVal = nPanels;
        return S_OK;
    default:
        return S_FALSE;
    }
}

// CMFCRibbonCategory::get_accChild(VARIANT, IDispatch**) — retail (RVA
// 0xf1060):
//   if (ppdispChild == NULL) return E_INVALIDARG;
//   if (varChild.vt != VT_I4) return S_FALSE;            (output untouched)
//   i = varChild.lVal - 1;  out of [0, panel count) -> AfxThrowInvalidArgException
//   pPanel = m_arPanels[i];  if (!pPanel) return S_FALSE; (output untouched)
//   pPanel->ExternalAddRef();
//   *ppdispChild = pPanel->GetInterface(&IID_IDispatch);  (written even if NULL;
//                        the IID at 0x1802d7a98 reads {00020400-...-46})
//   return *ppdispChild ? S_OK : S_FALSE;
// The bounds check is transcribed against the side-table panel list.  OpenMFC's
// CMFCRibbonPanel is not a CCmdTarget and exposes no IDispatch, so the final
// step always produces NULL — i.e. retail's own "GetInterface returned NULL"
// path: write NULL, return S_FALSE.
// Deviation: an out-of-range child returns S_FALSE here instead of throwing.
// TODO(clean-room): transcribed partially — panel IDispatch is not modeled.
// Symbol: ?get_accChild@CMFCRibbonCategory@@UEAAJUtagVARIANT@@PEAPEAUIDispatch@@@Z
extern "C" long MS_ABI impl__get_accChild_CMFCRibbonCategory__UEAAJUtagVARIANT__PEAPEAUIDispatch___Z(
    CMFCRibbonCategory* pThis, VARIANT varChild, IDispatch** ppdispChild)
{
    if (!pThis) return E_INVALIDARG;
    if (!ppdispChild) return E_INVALIDARG;
    if (varChild.vt != 3 /*VT_I4*/) return S_FALSE;

    const int nIndex = static_cast<int>(varChild.lVal) - 1;
    if (nIndex < 0 ||
        nIndex >= impl__GetPanelCount_CMFCRibbonCategory__QEBAHXZ(pThis)) {
        return S_FALSE;
    }

    CMFCRibbonPanel* pPanel =
        impl__GetPanel_CMFCRibbonCategory__QEAAPEAVCMFCRibbonPanel__H_Z(pThis, nIndex);
    if (!pPanel) return S_FALSE;

    *ppdispChild = nullptr;
    return S_FALSE;
}

// CMFCRibbonCategory::get_accChildCount(long*) — transcribed in full from
// retail RVA 0xf1040:
//   if (pcountChildren == NULL) return E_INVALIDARG;
//   *pcountChildren = (long)m_arPanels.GetSize();   (32-bit store)
//   return S_OK;
// The panel count is the one piece of category state OpenMFC models.
// Symbol: ?get_accChildCount@CMFCRibbonCategory@@UEAAJPEAJ@Z
extern "C" long MS_ABI impl__get_accChildCount_CMFCRibbonCategory__UEAAJPEAJ_Z(
    CMFCRibbonCategory* pThis, long* pcountChildren)
{
    if (!pThis) return E_INVALIDARG;
    if (!pcountChildren) return E_INVALIDARG;
    *pcountChildren =
        static_cast<long>(impl__GetPanelCount_CMFCRibbonCategory__QEBAHXZ(pThis));
    return S_OK;
}

// CMFCRibbonCategory::get_accParent(IDispatch**) — retail (RVA 0xf0ff0):
//   if (ppdispParent == NULL) return E_INVALIDARG;
//   *ppdispParent = NULL;
//   if (m_pParentRibbonBar == NULL || m_pParentRibbonBar->m_hWnd == NULL) return S_FALSE;
//   IDispatch* p = m_pParentRibbonBar->GetAccessibleDispatch();
//   if (p) *ppdispParent = p;
//   return S_OK;
// OpenMFC declares no m_pParentRibbonBar member, so the S_FALSE branch is the only
// reachable one; the NULL store ahead of it is transcribed as-is.
// TODO(clean-room): transcribed partially — the parent ribbon bar is not
// modeled.
// Symbol: ?get_accParent@CMFCRibbonCategory@@UEAAJPEAPEAUIDispatch@@@Z
extern "C" long MS_ABI impl__get_accParent_CMFCRibbonCategory__UEAAJPEAPEAUIDispatch___Z(
    CMFCRibbonCategory* pThis, IDispatch** ppdispParent)
{
    if (!pThis) return E_INVALIDARG;
    if (!ppdispParent) return E_INVALIDARG;
    *ppdispParent = nullptr;
    return S_FALSE;
}

// CMFCRibbonCategory::GetTextTopLine() const — retail (RVA 0xf03a0) loads as
// one qword the packed CSize that is m_LargeImages(+0x968).m_sizeImage(+0x68)
// = +0x9d0, and returns -1 when both halves are zero, otherwise cy + 5.
// (GetImageSize at RVA 0xefce0 picks the same pair of lists: +0x838 for
// nImageSizeType == 0, +0x9d0 otherwise.)
// OpenMFC models no image sizes — GetImageSize above
// returns CSize(0, 0) unconditionally — so the empty-size branch is the only
// reachable one and -1 is retail's answer for it.
// TODO(clean-room): transcribed partially — category image sizes are not
// modeled.
// Symbol: ?GetTextTopLine@CMFCRibbonCategory@@QEBAHXZ
extern "C" int MS_ABI impl__GetTextTopLine_CMFCRibbonCategory__QEBAHXZ(
    const CMFCRibbonCategory* pThis)
{
    if (!pThis) return -1;
    return -1;
}

// CMFCRibbonCategory::GetVisibleElements(CArray<CMFCRibbonBaseElement*,
// CMFCRibbonBaseElement*>&) — retail (RVA 0xf0bb0) walks m_arPanels and calls
// CMFCRibbonPanel::GetVisibleElements(arElements) (RVA 0x11dd10) on each; it
// never clears the array, so the caller owns the fill contract.  That loop is
// transcribed here over the side-table panel/element lists.  OpenMFC models
// no per-element visibility flag, so every element in a panel counts as
// visible — the same approximation GetElements above makes.
// TODO(clean-room): transcribed partially — element visibility is not modeled.
// Symbol: ?GetVisibleElements@CMFCRibbonCategory@@QEAAXAEAV?$CArray@PEAVCMFCRibbonBaseElement@@PEAV1@@@@Z
extern "C" void MS_ABI impl__GetVisibleElements_CMFCRibbonCategory__QEAAXAEAV__CArray_PEAVCMFCRibbonBaseElement__PEAV1____Z(
    CMFCRibbonCategory* pThis,
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
            if (pElem) {
                arElements.Add(pElem);
            }
        }
    }
}

// CMFCRibbonCategory::HighlightPanel(CMFCRibbonPanel*, CPoint) — retail (RVA
// 0xef280) walks m_arPanels; for each panel whose m_bIsHighlighted (+0xa8) is
// set it calls the panel virtual at vtable+0x238 with FALSE (un-highlight)
// unless the panel is the requested one, remembers the last such panel as the
// result, and calls the same virtual with TRUE for the requested panel; it
// then calls UpdateWindow on the HWND (+0x40) of m_pParentMenuBar (+0x768) if
// that is non-NULL, otherwise of m_pParentRibbonBar (+0x760).  Panel highlight
// state, that flag and the parent window are not modeled.
// TODO(clean-room): transcribed partially — panel highlight state is not
// modeled.
// Symbol: ?HighlightPanel@CMFCRibbonCategory@@QEAAPEAVCMFCRibbonPanel@@PEAV2@VCPoint@@@Z
extern "C" CMFCRibbonPanel* MS_ABI impl__HighlightPanel_CMFCRibbonCategory__QEAAPEAVCMFCRibbonPanel__PEAV2_VCPoint___Z(
    CMFCRibbonCategory* pThis, CMFCRibbonPanel* /*pHighlighted*/, CPoint /*point*/)
{
    if (!pThis) return nullptr;
    return nullptr;
}

// CMFCRibbonCategory::HitTest(CPoint, BOOL) const — retail (RVA 0xef0e0)
// first calls HitTestScrollButtons(point) and, when that returns a button,
// returns that button.  Otherwise it calls GetPanelFromPoint(point) and
// returns NULL when no panel is under the point; with a panel it returns the
// panel's virtual at vtable+0x180 (the panel's own HitTest) called with
// (point, bCheckPanelCaption).  Both the scroll buttons and the panel rects
// are unmodeled — GetPanelFromPoint above already returns NULL for that
// reason — so no element can be hit.
// TODO(clean-room): transcribed partially — panel rects and scroll buttons
// are not modeled.
// Symbol: ?HitTest@CMFCRibbonCategory@@QEBAPEAVCMFCRibbonBaseElement@@VCPoint@@H@Z
extern "C" CMFCRibbonBaseElement* MS_ABI impl__HitTest_CMFCRibbonCategory__QEBAPEAVCMFCRibbonBaseElement__VCPoint__H_Z(
    const CMFCRibbonCategory* pThis, CPoint /*point*/, int /*bCheckPanelCaption*/)
{
    if (!pThis) return nullptr;
    return nullptr;
}

// CMFCRibbonCategory::HitTestEx(CPoint) const — retail (RVA 0xef1d0) calls
// GetPanelFromPoint(point) and returns -1 when it finds no panel, otherwise
// tail-calls the panel virtual at vtable+0x188.  GetPanelFromPoint above
// returns NULL because panel rects are not modeled, so -1 is the reachable
// answer.
// TODO(clean-room): transcribed partially — panel rects are not modeled.
// Symbol: ?HitTestEx@CMFCRibbonCategory@@QEBAHVCPoint@@@Z
extern "C" int MS_ABI impl__HitTestEx_CMFCRibbonCategory__QEBAHVCPoint___Z(
    const CMFCRibbonCategory* pThis, CPoint /*point*/)
{
    if (!pThis) return -1;
    return -1;
}

// CMFCRibbonCategory::HitTestScrollButtons(CPoint) const — retail (RVA
// 0xef140) PtInRects the point against the rect at +0x330 and returns
// m_ScrollLeft (+0x268), then against the rect at +0x5a8 returning
// m_ScrollRight (+0x4e0), else NULL (each rect is that button's own
// CMFCRibbonBaseElement::m_rect, which sits 0xc8 into the element).  The
// embedded scroll buttons are not modeled.
// TODO(clean-room): transcribed partially — the category scroll buttons are
// not modeled.
// Symbol: ?HitTestScrollButtons@CMFCRibbonCategory@@QEBAPEAVCMFCRibbonBaseElement@@VCPoint@@@Z
extern "C" CMFCRibbonBaseElement* MS_ABI impl__HitTestScrollButtons_CMFCRibbonCategory__QEBAPEAVCMFCRibbonBaseElement__VCPoint___Z(
    const CMFCRibbonCategory* pThis, CPoint /*point*/)
{
    if (!pThis) return nullptr;
    return nullptr;
}

// CMFCRibbonCategory::IsWindows7Look() const — retail (RVA 0xf0960) returns
// 0 when m_pParentRibbonBar (+0x760) is NULL, otherwise the bar's m_bWindows7Look
// (+0x20c8; the name and offset are confirmed by the CMFCRibbonBar member
// block in include/openmfc/afxmfc.h, where it sits at byte 8392).
// OpenMFC declares no m_pParentRibbonBar member, so the NULL branch — return 0 — is
// the only reachable one.
// TODO(clean-room): transcribed partially — the parent ribbon bar is not
// modeled.
// Symbol: ?IsWindows7Look@CMFCRibbonCategory@@QEBAHXZ
extern "C" int MS_ABI impl__IsWindows7Look_CMFCRibbonCategory__QEBAHXZ(
    const CMFCRibbonCategory* pThis)
{
    if (!pThis) return 0;
    return 0;
}

// CMFCRibbonCategory::NormalizeFloatingRect(CMFCRibbonBar*, CRect&) — retail
// (RVA 0xf08a0) calls GetWindowRect on the bar's window (+0x40), feeds that
// rect's top-left corner to MonitorFromPoint(MONITOR_DEFAULTTONEAREST) and
// takes rcWork from GetMonitorInfo, falling back to SystemParametersInfo
// (SPI_GETWORKAREA, 0x30) when GetMonitorInfo fails.  It then clamps the
// caller's rect against that work area in one direction each:
//   rect.bottom = min(rect.bottom, work.bottom);   (cmp/cmovl at 0x1800f092c)
//   rect.left   = max(rect.left,   work.left);     (cmp/cmovg at 0x1800f093a)
// rect.top and rect.right are left alone.  It needs the parent bar's HWND,
// which OpenMFC's category cannot reach (no m_pParentRibbonBar member); clamping
// against a work area we cannot identify would corrupt the caller's rect.
// TODO(clean-room): not transcribed — the parent ribbon bar window is not
// modeled.
// Symbol: ?NormalizeFloatingRect@CMFCRibbonCategory@@AEAAXPEAVCMFCRibbonBar@@AEAVCRect@@@Z
extern "C" void MS_ABI impl__NormalizeFloatingRect_CMFCRibbonCategory__AEAAXPEAVCMFCRibbonBar__AEAVCRect___Z(
    CMFCRibbonCategory* pThis, CMFCRibbonBar* /*pBar*/, CRect& /*rect*/)
{
    if (!pThis) return;
}

// CMFCRibbonCategory::NotifyControlCommand(BOOL, int, WPARAM, LPARAM) —
// retail (RVA 0xef540) walks m_arPanels calling each panel's virtual at
// vtable+0x208 with the same four arguments, returns TRUE at the first panel
// that handles it, and FALSE when the loop runs out (it touches nothing else).
// OpenMFC's CMFCRibbonPanel declares no such virtual, so there is nothing to
// dispatch to.
// TODO(clean-room): not transcribed — panel command routing is not modeled.
// Symbol: ?NotifyControlCommand@CMFCRibbonCategory@@UEAAHHH_K_J@Z
extern "C" int MS_ABI impl__NotifyControlCommand_CMFCRibbonCategory__UEAAHHH_K_J_Z(
    CMFCRibbonCategory* pThis, int /*bAccelerator*/, int /*notifyCode*/,
    unsigned long long /*wParam*/, long long /*lParam*/)
{
    if (!pThis) return 0;
    return 0;
}

// CMFCRibbonCategory::OnCancelMode() — retail (RVA 0xef360) clears the
// pressed/tracking flag at +0xa0 and then calls CMFCRibbonPanel::CancelMode()
// (RVA 0x11b9b0) on every panel.  The flag is not modeled and the panel entry
// point is not reachable from the clean-room CMFCRibbonPanel.
// TODO(clean-room): not transcribed — the tracking flag and panel cancel-mode
// are not modeled.
// Symbol: ?OnCancelMode@CMFCRibbonCategory@@UEAAXXZ
extern "C" void MS_ABI impl__OnCancelMode_CMFCRibbonCategory__UEAAXXZ(
    CMFCRibbonCategory* pThis)
{
    if (!pThis) return;
}

// CMFCRibbonCategory::OnDraw(CDC*) — retail (RVA 0xee260) returns immediately
// when the category rect (+0xc0) is empty; otherwise it fetches the lazily
// created process-wide visual-manager singleton (RVA 0x97f4) and calls its
// virtual at vtable+0x438 with (pDC, this, rect) to paint the category, then
// goes on to draw the panels.  The rect, the panel geometry and the
// visual-manager hook are not modeled.
// TODO(clean-room): not transcribed — category geometry and visual-manager
// drawing are not modeled.
// Symbol: ?OnDraw@CMFCRibbonCategory@@UEAAXPEAVCDC@@@Z
extern "C" void MS_ABI impl__OnDraw_CMFCRibbonCategory__UEAAXPEAVCDC___Z(
    CMFCRibbonCategory* pThis, CDC* /*pDC*/)
{
    if (!pThis) return;
}

// CMFCRibbonCategory::OnDrawImage(CDC*, CRect, CMFCRibbonBaseElement*, BOOL
// bIsLargeImage, BOOL nImageIndex, BOOL bCenter) — retail (RVA 0xefb60)
// selects one of the two embedded CMFCToolBarImages from bIsLargeImage —
// m_SmallImages (+0x7d0) when it is zero, m_LargeImages (+0x968, that base
// plus the 0x198 the prologue folds in) when it is not — returns FALSE when
// nImageIndex is at or past that list's count (+8), and otherwise draws the
// image into rectImage through PrepareDrawImage/DrawEx/EndDrawImage.  The
// third int is the CENTERING flag, not a disabled flag: it is read only at
// 0x1800efbae to gate the "centre the image size (+0x68 of the list) inside
// rectImage" offset computation, and the disabled state comes instead from
// the element's own virtual at vtable+0x1c8.  Neither image list is modeled.
// TODO(clean-room): not transcribed — the category image lists are not
// modeled.
// Symbol: ?OnDrawImage@CMFCRibbonCategory@@UEAAHPEAVCDC@@VCRect@@PEAVCMFCRibbonBaseElement@@HHH@Z
extern "C" int MS_ABI impl__OnDrawImage_CMFCRibbonCategory__UEAAHPEAVCDC__VCRect__PEAVCMFCRibbonBaseElement__HHH_Z(
    CMFCRibbonCategory* pThis, CDC* /*pDC*/, CRect /*rectImage*/,
    CMFCRibbonBaseElement* /*pElement*/, int /*bIsLargeImage*/,
    int /*nImageIndex*/, int /*bCenter*/)
{
    if (!pThis) return 0;
    return 0;
}

// CMFCRibbonCategory::OnKey(UINT) — retail (RVA 0xf0c20) switches on nChar
// through the chained subs at 0x1800f0c4d-0x1800f0c70.  9 (VK_TAB) and
// 37/38/39/40 (VK_LEFT/UP/RIGHT/DOWN) go to 0x1800f0c76, which builds a local
// CArray, fills it with GetVisibleElements(...) (RVA 0xf0bb0) and walks it to
// move the keyboard focus.  13 (VK_RETURN) and 32 (VK_SPACE) go to
// 0x1800f0ccd, which does NOT move focus: it calls GetFocused() (RVA 0xf0b00)
// and, with a focused element, invokes that element's virtual at
// vtable+0x3c8.  Anything else falls straight through to the tail.
// Element focus is not modeled.
// TODO(clean-room): not transcribed — keyboard focus and key tips are not
// modeled.
// Symbol: ?OnKey@CMFCRibbonCategory@@UEAAHI@Z
extern "C" int MS_ABI impl__OnKey_CMFCRibbonCategory__UEAAHI_Z(
    CMFCRibbonCategory* pThis, unsigned int /*nChar*/)
{
    if (!pThis) return 0;
    return 0;
}

// CMFCRibbonCategory::OnLButtonDown(CPoint) — retail (RVA 0xef3d0) first
// hit-tests the scroll buttons; on a hit it calls that button's virtual at
// vtable+0x2b0, re-runs the hit test and returns the button when it is still
// under the cursor (NULL when it is not).  Otherwise it forwards to
// GetPanelFromPoint(point) and,
// with a panel, sets the tracking flag at +0xa0 and calls the panel virtual
// at vtable+0x240.  Scroll buttons, panel rects and that flag are not
// modeled, so no element is ever pressed.
// TODO(clean-room): not transcribed — scroll buttons and panel rects are not
// modeled.
// Symbol: ?OnLButtonDown@CMFCRibbonCategory@@UEAAPEAVCMFCRibbonBaseElement@@VCPoint@@@Z
extern "C" CMFCRibbonBaseElement* MS_ABI impl__OnLButtonDown_CMFCRibbonCategory__UEAAPEAVCMFCRibbonBaseElement__VCPoint___Z(
    CMFCRibbonCategory* pThis, CPoint /*point*/)
{
    if (!pThis) return nullptr;
    return nullptr;
}

// CMFCRibbonCategory::OnLButtonUp(CPoint) — retail (RVA 0xef460) clears the
// two scroll-button pressed flags (+0x3a4, +0x61c), then, when
// GetPanelFromPoint(point) finds a panel, clears the tracking flag at +0xa0
// and calls the panel virtual at vtable+0x248.  Those flags and the panel
// rects are not modeled.
// TODO(clean-room): not transcribed — scroll-button/tracking state and panel
// rects are not modeled.
// Symbol: ?OnLButtonUp@CMFCRibbonCategory@@UEAAXVCPoint@@@Z
extern "C" void MS_ABI impl__OnLButtonUp_CMFCRibbonCategory__UEAAXVCPoint___Z(
    CMFCRibbonCategory* pThis, CPoint /*point*/)
{
    if (!pThis) return;
}

// CMFCRibbonCategory::OnMouseMove(CPoint) — retail (RVA 0xef040) forwards the
// point to both embedded scroll buttons (+0x268 and +0x4e0) through their
// virtual at vtable+0x440, and when neither reports itself highlighted
// (vtable+0x1b0) calls HighlightPanel(GetPanelFromPoint(point), point).  The
// scroll buttons and the panel rects are not modeled, and HighlightPanel
// above is itself a no-op for the same reason.
// TODO(clean-room): not transcribed — scroll buttons and panel rects are not
// modeled.
// Symbol: ?OnMouseMove@CMFCRibbonCategory@@UEAAXVCPoint@@@Z
extern "C" void MS_ABI impl__OnMouseMove_CMFCRibbonCategory__UEAAXVCPoint___Z(
    CMFCRibbonCategory* pThis, CPoint /*point*/)
{
    if (!pThis) return;
}

// CMFCRibbonCategory::OnRTLChanged(BOOL) — retail (RVA 0xf02d0) calls the
// panel virtual at vtable+0x230 for every panel, then the element virtual at
// vtable+0x3f8 for every element of the non-visible m_arElements
// (+0xb08/+0xb10), and finally writes -1 to the cached layout width
// m_nLastCategoryWidth (+0xb0).  OpenMFC's
// CMFCRibbonPanel declares no OnRTLChanged, the hidden list is not modeled and
// neither is the size cache.
// TODO(clean-room): not transcribed — panel RTL handling and the size cache
// are not modeled.
// Symbol: ?OnRTLChanged@CMFCRibbonCategory@@UEAAXH@Z
extern "C" void MS_ABI impl__OnRTLChanged_CMFCRibbonCategory__UEAAXH_Z(
    CMFCRibbonCategory* pThis, int /*bIsRTL*/)
{
    if (!pThis) return;
}

// CMFCRibbonCategory::OnScrollHorz(BOOL, int) — retail (RVA 0xf0550) moves
// the horizontal scroll offset at +0x758 by nScrollOffset (default 0x32 when
// zero is passed; it subtracts when bScrollLeft is non-zero and adds when it
// is zero), clamps it to [0, m_nMinWidth(+0x770) - rect.Width()], then
// re-runs ReposPanels on a client DC, refreshes the scroll buttons and
// redraws; it returns TRUE only if the offset actually changed.  The scroll
// offset, the width cache and the category rect are not modeled.
// TODO(clean-room): not transcribed — the horizontal scroll model is not
// modeled.
// Symbol: ?OnScrollHorz@CMFCRibbonCategory@@UEAAHHH@Z
extern "C" int MS_ABI impl__OnScrollHorz_CMFCRibbonCategory__UEAAHHH_Z(
    CMFCRibbonCategory* pThis, int /*bScrollLeft*/, int /*nScrollOffset*/)
{
    if (!pThis) return 0;
    return 0;
}

// CMFCRibbonCategory::OnSetAccData(long) — retail (RVA 0xf0f20) first resets
// the CAccessibilityData block the base class embeds at +0x40 — Empty() on
// its six CStrings (+0x00..+0x28; the +0x00 string is emptied twice, seven
// calls in all) with role/state/hit and the rect/point zeroed — then treats
// lVal - 1 as a panel index:
// out of range returns FALSE, in range it returns the panel's virtual at
// vtable+0x168 called with (m_pParentRibbonBar, that data block).  Neither the
// embedded accessibility block, m_pParentRibbonBar nor the panel virtual is
// modeled, so only the out-of-range answer (FALSE) can be produced.
// TODO(clean-room): not transcribed — the embedded accessibility block and
// panel accessibility are not modeled.
// Symbol: ?OnSetAccData@CMFCRibbonCategory@@UEAAHJ@Z
extern "C" int MS_ABI impl__OnSetAccData_CMFCRibbonCategory__UEAAHJ_Z(
    CMFCRibbonCategory* pThis, long /*lVal*/)
{
    if (!pThis) return 0;
    return 0;
}

// CMFCRibbonCategory::OnUpdateCmdUI(CMFCRibbonCmdUI*, CFrameWnd*, BOOL) —
// retail (RVA 0xef4c0) calls the panel virtual at vtable+0x200 with the same
// three arguments for every panel in m_arPanels, and does nothing else.
// OpenMFC's CMFCRibbonPanel declares no OnUpdateCmdUI and CMFCRibbonCmdUI is
// not declared at all, so there is nothing to dispatch to.
// TODO(clean-room): not transcribed — panel command-UI updating is not
// modeled.
// Symbol: ?OnUpdateCmdUI@CMFCRibbonCategory@@UEAAXPEAVCMFCRibbonCmdUI@@PEAVCFrameWnd@@H@Z
extern "C" void MS_ABI impl__OnUpdateCmdUI_CMFCRibbonCategory__UEAAXPEAVCMFCRibbonCmdUI__PEAVCFrameWnd__H_Z(
    CMFCRibbonCategory* pThis, void* /*pCmdUI*/, CFrameWnd* /*pTarget*/,
    int /*bDisableIfNoHndler*/)
{
    if (!pThis) return;
}

// CMFCRibbonCategory::RecalcLayout(CDC*) — retail (RVA 0xee740) returns at
// once when the category rect (+0xc0) is empty; otherwise it calls
// CleanUpSizes() (RVA 0xeef40) when m_pParentMenuBar (+0x768) is
// non-NULL, then RecalcPanelWidths(pDC) (RVA 0xeee90), then returns
// immediately if there are no panels and otherwise runs the panel layout.
// Every input is an unmodeled member, and CleanUpSizes/RecalcPanelWidths are
// themselves no-ops here.
// TODO(clean-room): not transcribed — category geometry is not modeled.
// Symbol: ?RecalcLayout@CMFCRibbonCategory@@UEAAXPEAVCDC@@@Z
extern "C" void MS_ABI impl__RecalcLayout_CMFCRibbonCategory__UEAAXPEAVCDC___Z(
    CMFCRibbonCategory* pThis, CDC* /*pDC*/)
{
    if (!pThis) return;
}

// CMFCRibbonCategory::RecalcPanelWidths(CDC*) — retail (RVA 0xeee90) walks
// m_arPanels and, for each panel whose m_arWidths is empty (size at +0x6b8),
// calls GetMaxHeight(pDC) (RVA 0xee670) once — the result is memoized in a
// local across the whole loop — then calls the panel's virtual at
// vtable+0x1a0 with (pDC, that height) and writes -1 to the category's
// m_nLastCategoryWidth (+0xb0).  After the loop it writes -1 to m_nMinWidth
// (+0x770) unconditionally.  Panel width caches are not modeled.
// TODO(clean-room): not transcribed — panel width caches are not modeled.
// Symbol: ?RecalcPanelWidths@CMFCRibbonCategory@@IEAAXPEAVCDC@@@Z
extern "C" void MS_ABI impl__RecalcPanelWidths_CMFCRibbonCategory__IEAAXPEAVCDC___Z(
    CMFCRibbonCategory* pThis, CDC* /*pDC*/)
{
    if (!pThis) return;
}

// CMFCRibbonCategory::RemovePanel(int, BOOL) — transcribed in full from
// retail RVA 0xee510:
//   if (nIndex < 0 || nIndex >= m_arPanels.GetSize()) return FALSE;
//   CMFCRibbonPanel* pPanel = m_arPanels[nIndex];
//   m_arPanels.RemoveAt(nIndex);                 (memmove + size--)
//   if (bDelete && pPanel != NULL) delete pPanel; (vtable slot 1, flag 1)
//   return TRUE;
// The panel list lives in the ribbon state side table here, so the same
// sequence runs against it.  Deviation forced by the port: the side table
// also tracks which panels the category owns, and the entry is dropped from
// that set either way — after RemovePanel the category no longer references
// the panel, so leaving it owned would make ~CMFCRibbonCategory free a panel
// this call either already destroyed or handed back to the caller.
// The destroy path mirrors ~CMFCRibbonCategory above: run the panel's
// exported destructor thunk, then release the storage.
// Symbol: ?RemovePanel@CMFCRibbonCategory@@QEAAHHH@Z
extern "C" int MS_ABI impl__RemovePanel_CMFCRibbonCategory__QEAAHHH_Z(
    CMFCRibbonCategory* pThis, int nIndex, int bDelete)
{
    if (!pThis || nIndex < 0) return 0;

    CMFCRibbonPanel* pPanel = nullptr;
    {
        std::lock_guard<std::mutex> lock(RibbonMutex());
        auto it = RibbonCategoryStates().find(pThis);
        if (it == RibbonCategoryStates().end()) return 0;

        auto& panels = it->second.panels;
        if (nIndex >= static_cast<int>(panels.size())) return 0;

        pPanel = panels[static_cast<size_t>(nIndex)];
        panels.erase(panels.begin() + nIndex);
        it->second.ownedPanels.erase(pPanel);
    }

    if (bDelete && pPanel) {
        impl___1CMFCRibbonPanel__UEAA_XZ(pPanel);
        ::operator delete(pPanel);
    }
    return 1;
}

// CMFCRibbonCategory::ReposPanels(CDC*) — retail (RVA 0xeecb0) positions the
// panels across the category rect (read as four dwords at +0xc0..+0xcc),
// offset by m_nScrollOffset (+0x758) and bounded by m_nMinWidth (+0x770).
// Every input is an unmodeled member.
// TODO(clean-room): not transcribed — category geometry is not modeled.
// Symbol: ?ReposPanels@CMFCRibbonCategory@@UEAAXPEAVCDC@@@Z
extern "C" void MS_ABI impl__ReposPanels_CMFCRibbonCategory__UEAAXPEAVCDC___Z(
    CMFCRibbonCategory* pThis, CDC* /*pDC*/)
{
    if (!pThis) return;
}

// CMFCRibbonCategory::ResetPanelsLayout() — retail (RVA 0xf04e0) writes 0 to
// each panel's m_nCurrWidthIndex (+0xe4) and m_bTruncateCaption (+0xdc) —
// both offsets pinned against the CMFCRibbonPanel member block, whose first
// member m_bShowCaption sits at +0xa0 — then zeroes the category's
// m_nScrollOffset (+0x758).  All three are unmodeled members.
// TODO(clean-room): not transcribed — panel collapse state and the scroll
// offset are not modeled.
// Symbol: ?ResetPanelsLayout@CMFCRibbonCategory@@IEAAXXZ
extern "C" void MS_ABI impl__ResetPanelsLayout_CMFCRibbonCategory__IEAAXXZ(
    CMFCRibbonCategory* pThis)
{
    if (!pThis) return;
}

// CMFCRibbonCategory::SetACCData(CWnd*, CAccessibilityData&) — retail (RVA
// 0xf1470), in order:
//   data.m_strAccName    = m_strName;      (+0xd0 -> data +0x00)
//   data.m_nAccRole      = 0x16;           (ROLE_SYSTEM_TOOLBAR, data +0x30)
//   data.m_strAccValue   = _T("Group");    (data +0x08)
//   data.m_rectAccLocation = m_rect;       (+0xc0 -> data +0x3c)
//   if (m_pParentRibbonBar && m_pParentRibbonBar->m_hWnd)
//       m_pParentRibbonBar->ClientToScreen(&data.m_rectAccLocation);
//   data.m_bAccState     = 0;              (data +0x34)
//   return TRUE;
// CAccessibilityData is declared in OpenMFC, so everything except the rect is
// transcribed directly.  Deviation: the category rect (+0xc0) has no
// clean-room member, so m_rectAccLocation is set empty instead of to the real
// rect, and the screen mapping is skipped (there is no reachable ribbon bar
// to map through).
// Symbol: ?SetACCData@CMFCRibbonCategory@@UEAAHPEAVCWnd@@AEAVCAccessibilityData@@@Z
extern "C" int MS_ABI impl__SetACCData_CMFCRibbonCategory__UEAAHPEAVCWnd__AEAVCAccessibilityData___Z(
    CMFCRibbonCategory* pThis, CWnd* /*pParent*/, CAccessibilityData& data)
{
    if (!pThis) return 0;

    data.m_strAccName = pThis->GetName();
    data.m_nAccRole = 0x16;              // ROLE_SYSTEM_TOOLBAR
    data.m_strAccValue = L"Group";
    data.m_rectAccLocation = CRect(0, 0, 0, 0);
    data.m_bAccState = 0;
    return 1;
}

// CMFCRibbonCategory::SetActive(BOOL) — retail (RVA 0xef5e0) returns at once
// when m_bIsActive (+0xa4) already matches.  Otherwise, when bit 0
// (AFX_RIBBONBAR_HIDE_ELEMENTS) of the bar's m_dwHideFlags (+0x478 of
// m_pParentRibbonBar) is set it only stores the new flag; else it calls
// ShowElements(TRUE) — skipped when the bar's m_bIsPrintPreview (+0x430) is
// non-zero and afxGlobalData (the AFX_GLOBAL_DATA singleton lazily
// Initialize()d at 0x1803ba380) reports IsDwmCompositionEnabled() — stores
// m_bIsActive, calls each panel's virtual at vtable+0x218 with the new flag,
// and finally writes clock() (or -1 when deactivating) into m_ActiveTime
// (+0x7c8).  The active flag and the parent ribbon bar are not modeled.
// TODO(clean-room): not transcribed — the active flag and the parent ribbon
// bar are not modeled.
// Symbol: ?SetActive@CMFCRibbonCategory@@IEAAXH@Z
extern "C" void MS_ABI impl__SetActive_CMFCRibbonCategory__IEAAXH_Z(
    CMFCRibbonCategory* pThis, int /*bIsActive*/)
{
    if (!pThis) return;
}

// CMFCRibbonCategory::SetCollapseOrder(const CArray<int, int>&) — retail (RVA
// 0xf03d0) does m_arCollapseOrder(+0x778).SetSize(0) and then copies the
// caller's array into it.  That CArray has no clean-room member; the class
// declares only m_strName plus an opaque pad, and the pad must not be
// reinterpreted as a CArray.
// TODO(clean-room): not transcribed — the collapse-order array is not modeled.
// Symbol: ?SetCollapseOrder@CMFCRibbonCategory@@QEAAXAEBV?$CArray@HH@@@Z
extern "C" void MS_ABI impl__SetCollapseOrder_CMFCRibbonCategory__QEAAXAEBV__CArray_HH___Z(
    CMFCRibbonCategory* pThis, const CArray<int, int>& /*arCollapseOrder*/)
{
    if (!pThis) return;
}

// CMFCRibbonCategory::SetKeys(LPCTSTR) — the retail image on this host is
// the ANSI mfc140.dll, so what was disassembled is this export's char twin,
// ?SetKeys@CMFCRibbonCategory@@QEAAXPEBD@Z at RVA 0xf0190 (the Unicode export
// carries no RVA here).  It returns via
// AfxThrowInvalidArgException when lpszKeys is NULL, and otherwise tail-calls
// the virtual at vtable+0x180 of the tab element embedded at +0xd8, passing
// (lpszKeys, FALSE).  That embedded CMFCRibbonTab is not modeled — OpenMFC's
// category has no tab member — so the keys have nowhere to go.
// TODO(clean-room): not transcribed — the category's embedded tab element is
// not modeled.
// Symbol: ?SetKeys@CMFCRibbonCategory@@QEAAXPEB_W@Z
extern "C" void MS_ABI impl__SetKeys_CMFCRibbonCategory__QEAAXPEB_W_Z(
    CMFCRibbonCategory* pThis, const wchar_t* /*lpszKeys*/)
{
    if (!pThis) return;
}

// CMFCRibbonCategory::SetName(LPCTSTR) — the retail image on this host is
// the ANSI mfc140.dll, so what was disassembled is this export's char twin,
// ?SetName@CMFCRibbonCategory@@QEAAXPEBD@Z at RVA 0xf01c0 (the Unicode export
// carries no RVA here):
//   m_strName = lpszName ? lpszName : _T("");     (+0xd0)
//   CString strKeys;
//   for (int i = 0; i < m_strName.GetLength(); i++)
//       if (m_strName[i] == _T('&')) {
//           m_strName.Delete(i, 1);
//           if (i < m_strName.GetLength()) strKeys += m_strName[i];
//       }
//   m_Tab.SetKeys(strKeys);   // virtual at vtable+0x180 of the tab at +0xd8
// The name half is transcribed exactly, including the in-place '&' stripping.
// Deviation: the embedded tab element at +0xd8 has no clean-room member, so
// the collected key tips are computed and then dropped (SetKeys above is a
// no-op for the same reason).
// Symbol: ?SetName@CMFCRibbonCategory@@QEAAXPEB_W@Z
extern "C" void MS_ABI impl__SetName_CMFCRibbonCategory__QEAAXPEB_W_Z(
    CMFCRibbonCategory* pThis, const wchar_t* lpszName)
{
    if (!pThis) return;

    CString strName(lpszName ? lpszName : L"");
    CString strKeys;

    for (int i = 0; i < strName.GetLength(); i++) {
        if (strName.GetAt(i) == L'&') {
            strName.Delete(i, 1);
            if (i < strName.GetLength()) {
                const wchar_t szKey[2] = { strName.GetAt(i), L'\0' };
                strKeys += szKey;
            }
        }
    }

    (void)strKeys;   // no embedded tab element to hand the key tips to
    RibbonCategoryNameAccess::Set(pThis, strName);
}

// CMFCRibbonCategory::SetPanelsLayout(int) — retail (RVA 0xf0410) walks
// m_arPanels accumulating 2 + panel->m_arWidths(data at +0x6b0)[panel->
// m_nCurrWidthIndex(+0xe4)] + 2 * panel->m_nXMargin(+0xf0) per panel (also
// setting the panel's m_bForceCollpapse (+0xa4) to 1 exactly when the current
// width index is the last of more than one), and returns FALSE as soon as the running
// total exceeds the requested width, TRUE otherwise.  Every panel member it
// reads is unmodeled, so the sum cannot be computed.
// TODO(clean-room): not transcribed — panel width tables are not modeled.
// Symbol: ?SetPanelsLayout@CMFCRibbonCategory@@IEAAHH@Z
extern "C" int MS_ABI impl__SetPanelsLayout_CMFCRibbonCategory__IEAAHH_Z(
    CMFCRibbonCategory* pThis, int /*nWidth*/)
{
    if (!pThis) return 0;
    return 0;
}

// CMFCRibbonCategory::ShowElements(BOOL) — retail (RVA 0xef6e0) writes
// !bShow into the ribbon bar's m_dwHideFlags (+0x478 of m_pParentRibbonBar), i.e.
// sets it to AFX_RIBBONBAR_HIDE_ELEMENTS (1) or clears it outright — the
// store is a plain 32-bit write of 0/1, not a bit operation — then calls the
// panel virtual at vtable+0x218 with bShow for every
// panel, and then redraws through the bar.  There is no reachable
// m_pParentRibbonBar and CMFCRibbonPanel declares no such virtual.
// TODO(clean-room): not transcribed — the parent ribbon bar and panel
// visibility are not modeled.
// Symbol: ?ShowElements@CMFCRibbonCategory@@IEAAXH@Z
extern "C" void MS_ABI impl__ShowElements_CMFCRibbonCategory__IEAAXH_Z(
    CMFCRibbonCategory* pThis, int /*bShow*/)
{
    if (!pThis) return;
}

// CMFCRibbonCategory::ShowFloating(CRect) — retail (RVA 0xefe30) does nothing
// unless bit 0 (AFX_RIBBONBAR_HIDE_ELEMENTS) of the ribbon bar's
// m_dwHideFlags (+0x478 of m_pParentRibbonBar) is set; it then allocates a
// 0x2f60-byte CMFCRibbonPanelMenu (ctor at RVA 0x121eb0) for this category
// and shows it as a floating popup at the given rect.  Neither the parent
// ribbon bar nor CMFCRibbonPanelMenu construction is modeled.
// TODO(clean-room): not transcribed — the floating panel menu is not modeled.
// Symbol: ?ShowFloating@CMFCRibbonCategory@@IEAAXVCRect@@@Z
extern "C" void MS_ABI impl__ShowFloating_CMFCRibbonCategory__IEAAXVCRect___Z(
    CMFCRibbonCategory* pThis, CRect /*rect*/)
{
    if (!pThis) return;
}

// CMFCRibbonCategory::UpdateScrollButtons() — retail (RVA 0xeeb50) copies
// m_pParentMenuBar (+0x768) into each scroll button's own m_pParentMenu
// (+0x360 and +0x5d8, i.e. +0xf8 into m_ScrollLeft/m_ScrollRight), then sizes
// and positions the left/right buttons from the category rect, m_nScrollOffset
// (+0x758) and the afxGlobalData DPI scale, enabling each only when there is
// room to scroll that way.  The buttons, the rect and the offset are
// not modeled.
// TODO(clean-room): not transcribed — the category scroll buttons are not
// modeled.
// Symbol: ?UpdateScrollButtons@CMFCRibbonCategory@@IEAAXXZ
extern "C" void MS_ABI impl__UpdateScrollButtons_CMFCRibbonCategory__IEAAXXZ(
    CMFCRibbonCategory* pThis)
{
    if (!pThis) return;
}
