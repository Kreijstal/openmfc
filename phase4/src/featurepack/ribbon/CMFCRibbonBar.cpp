// CMFCRibbonBar — OpenMFC implementation.
// Sources: cbarcore.cpp, global_mfc-feature-6_impl.cpp, global_mfc-feature-7_impl.cpp, global_ribbonbar_batch1.cpp, mfccore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/CbarcoreSupport.h"
#include "detail/CMFCRibbonBarSupport.h"
#include "detail/MfcFeature7ImplSupport.h"
#include "detail/MfccoreSupport.h"

// CMFCRibbonBar methods that the transcribed retail member layout makes
// implementable for real. Each one was decoded from the retail mfc140u export
// (ordinal -> RVA -> objdump) rather than inferred, and the member offsets the
// shipping code uses are exactly the ones in include/openmfc/afxmfc.h (see the
// static_asserts in mfccore.cpp).
//
// Retail offsets referenced below:
//   0x450 = 1104 m_bToolTip
//   0x454 = 1108 m_bToolTipDescr
//   0x458 = 1112 m_bKeyTips
//
// Only two CMFCRibbonBar exports are implemented here. GetCategoryCount is
// deliberately NOT among them: retail reads m_arCategories.m_nSize at 0xb60,
// but OpenMFC's categories live in a side table that AddCategory populates and
// nothing writes that member, so a retail-exact read would report 0 for every
// ribbon this library actually builds. Trading working behaviour for byte
// fidelity is the wrong trade; the existing side-table implementation in
// cbarcore.cpp stays until the CArray members are maintained for real. The
// rest reach through CMFCRibbonCategory / CMFCRibbonBaseElement / the panel
// classes, which OpenMFC does not model; ForceRecalcLayout, for instance, sets
// two flags this file could set and then iterates every category invoking a
// virtual on it. Implementing only the flag half would look like progress
// while silently doing a fraction of the work, so those are left as stubs
// until the element classes exist.
//
// Kept in a global_*.cpp file so the build's AUTO_EXCLUDES pass (scan of
// `// Symbol:` comments) drops the generated stubs for these ordinals.

#include "openmfc/afxwin.h"
#include "openmfc/afxmfc.h"

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif


// Symbol: ??0CMFCRibbonBar@@QEAA@H@Z
extern "C" void* MS_ABI impl___0CMFCRibbonBar__QEAA_H_Z(void* pThis, int bReplaceFrameCaption) {
    if (!pThis) return nullptr;
    std::lock_guard<std::mutex> lock(g_ribbonMutex);
    CMFCRibbonBar* bar = new(pThis) CMFCRibbonBar(bReplaceFrameCaption);
    g_ribbonBars[bar] = RibbonBarState{};
    return bar;
}
// Symbol: ??1CMFCRibbonBar@@UEAA@XZ
extern "C" void MS_ABI impl___1CMFCRibbonBar__UEAA_XZ(void* pThis) {
    auto* bar = reinterpret_cast<CMFCRibbonBar*>(pThis);
    if (!bar) return;

    std::vector<CMFCRibbonCategory*> ownedCategories;
    {
        std::lock_guard<std::mutex> lock(g_ribbonMutex);
        if (auto it = g_ribbonBars.find(bar); it != g_ribbonBars.end()) {
            ownedCategories.assign(it->second.ownedCategories.begin(), it->second.ownedCategories.end());
            g_ribbonBars.erase(it);
        }
    }

    for (CMFCRibbonCategory* category : ownedCategories) {
        if (category) {
            impl___1CMFCRibbonCategory__UEAA_XZ(category);
            ::operator delete(category);
        }
    }

    bar->~CMFCRibbonBar();
}
// Symbol: ?AddCategory@CMFCRibbonBar@@QEAAPEAVCMFCRibbonCategory@@PEB_WIIVCSize@@1HPEAUCRuntimeClass@@@Z
extern "C" CMFCRibbonCategory* MS_ABI impl__AddCategory_CMFCRibbonBar__QEAAPEAVCMFCRibbonCategory__PEB_WIIVCSize__1HPEAUCRuntimeClass___Z(
    CMFCRibbonBar* pThis, const wchar_t* lpszName, unsigned int uiSmallImage, unsigned int uiLargeImage,
    CSize sizeSmall, unsigned int uiAnimLargeImage, int nInsertAt, CRuntimeClass*) {
    if (!pThis) return nullptr;

    auto* category = new CMFCRibbonCategory(lpszName, uiSmallImage, uiLargeImage);
    (void)sizeSmall;
    (void)uiAnimLargeImage;

    std::lock_guard<std::mutex> lock(g_ribbonMutex);
    auto& state = g_ribbonBars[pThis];
    auto insertPos = state.categories.end();
    if (nInsertAt >= 0 && nInsertAt < static_cast<int>(state.categories.size())) {
        insertPos = state.categories.begin() + nInsertAt;
    }
    state.categories.insert(insertPos, category);
    state.ownedCategories.insert(category);
    if (state.activeCategory == nullptr) {
        state.activeCategory = category;
    }
    g_ribbonCategories.try_emplace(category);
    return category;
}
// Symbol: ?GetCategory@CMFCRibbonBar@@QEBAPEAVCMFCRibbonCategory@@H@Z
extern "C" CMFCRibbonCategory* MS_ABI impl__GetCategory_CMFCRibbonBar__QEBAPEAVCMFCRibbonCategory__H_Z(
    const CMFCRibbonBar* pThis, int nIndex) {
    if (!pThis || nIndex < 0) return nullptr;
    std::lock_guard<std::mutex> lock(g_ribbonMutex);
    auto it = g_ribbonBars.find(const_cast<CMFCRibbonBar*>(pThis));
    if (it == g_ribbonBars.end()) return nullptr;
    const auto& categories = it->second.categories;
    if (nIndex >= static_cast<int>(categories.size())) return nullptr;
    return categories[static_cast<size_t>(nIndex)];
}
// Symbol: ?GetCategoryCount@CMFCRibbonBar@@QEBAHXZ
extern "C" int MS_ABI impl__GetCategoryCount_CMFCRibbonBar__QEBAHXZ(const CMFCRibbonBar* pThis) {
    if (!pThis) return 0;
    std::lock_guard<std::mutex> lock(g_ribbonMutex);
    auto it = g_ribbonBars.find(const_cast<CMFCRibbonBar*>(pThis));
    return (it == g_ribbonBars.end()) ? 0 : static_cast<int>(it->second.categories.size());
}
// Symbol: ?SetActiveCategory@CMFCRibbonBar@@UEAAHPEAVCMFCRibbonCategory@@H@Z
extern "C" int MS_ABI impl__SetActiveCategory_CMFCRibbonBar__UEAAHPEAVCMFCRibbonCategory__H_Z(
    CMFCRibbonBar* pThis, CMFCRibbonCategory* pCategory, int) {
    if (!pThis) return FALSE;
    std::lock_guard<std::mutex> lock(g_ribbonMutex);
    auto& state = g_ribbonBars[pThis];
    if (pCategory == nullptr) {
        state.activeCategory = nullptr;
        return TRUE;
    }
    auto it = std::find(state.categories.begin(), state.categories.end(), pCategory);
    if (it == state.categories.end()) return FALSE;
    state.activeCategory = pCategory;
    return TRUE;
}
// Symbol: ?AddToTabs@CMFCRibbonBar@@QEAAXPEAVCMFCRibbonBaseElement@@@Z
extern "C" void MS_ABI impl__AddToTabs_CMFCRibbonBar__QEAAXPEAVCMFCRibbonBaseElement___Z(
    CMFCRibbonBar* pThis, CMFCRibbonBaseElement* pElement) {
    if (!pThis || !pElement) return;
    std::lock_guard<std::mutex> lock(g_ribbonMutex);
    auto& tabs = g_ribbonBars[pThis].tabs;
    if (std::find(tabs.begin(), tabs.end(), pElement) == tabs.end()) {
        tabs.push_back(pElement);
    }
}
// Symbol: ?SetQuickAccessToolbarOnTop@CMFCRibbonBar@@QEAAXH@Z
extern "C" void MS_ABI impl__SetQuickAccessToolbarOnTop_CMFCRibbonBar__QEAAXH_Z(CMFCRibbonBar* pThis, int bOnTop) {
    if (!pThis) return;
    std::lock_guard<std::mutex> lock(g_ribbonMutex);
    g_ribbonBars[pThis].quickAccessToolbarOnTop = (bOnTop != FALSE);
}
// Symbol: ?ToggleMimimizeState@CMFCRibbonBar@@QEAAXXZ
extern "C" void MS_ABI impl__ToggleMimimizeState_CMFCRibbonBar__QEAAXXZ(CMFCRibbonBar* pThis) {
    if (!pThis) return;
    std::lock_guard<std::mutex> lock(g_ribbonMutex);
    auto& state = g_ribbonBars[pThis];
    state.minimized = !state.minimized;
}
// Symbol: ?GetRuntimeClass@CMFCRibbonBar@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CMFCRibbonBar__UEBAPEAUCRuntimeClass__XZ(
    const CMFCRibbonBar* pThis) {
    return CMFCRibbonBar::GetThisClass();
}
// Symbol: ?GetThisClass@CMFCRibbonBar@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMFCRibbonBar__SAPEAUCRuntimeClass__XZ() {
    return CMFCRibbonBar::GetThisClass();
}
// CMFCRibbonBar::FindCategoryIndexByData(unsigned int) -- retail scans
// m_arCategories (m_pData +0xb58 / m_nSize +0xb60) and compares each
// category's data member at +0xb8 with the argument, returning the first
// matching index or -1.
//
// OpenMFC's CMFCRibbonCategory declares only m_strName plus opaque padding;
// there is no data member at +0xb8 and the ribbon_state side tables store no
// per-category data, so no category can ever match.
// TODO(clean-room): partially transcribed -- needs a per-category data
// member that is not modeled.
// Symbol: ?FindCategoryIndexByData@CMFCRibbonBar@@QEBAHK@Z
extern "C" int MS_ABI impl__FindCategoryIndexByData_CMFCRibbonBar__QEBAHK_Z(
    const CMFCRibbonBar* pThis, unsigned int /*nData*/)
{
    if (!pThis) return -1;
    return -1;
}
// CMFCRibbonBar::GetCategoryIndex(CMFCRibbonCategory*) -- retail scans
// m_arCategories and returns the index of the category pointer, or -1.
// Implemented against the ribbon side table, which is where OpenMFC keeps
// the category list (see AddCategory in cbarcore.cpp).
// Symbol: ?GetCategoryIndex@CMFCRibbonBar@@QEBAHPEAVCMFCRibbonCategory@@@Z
extern "C" int MS_ABI impl__GetCategoryIndex_CMFCRibbonBar__QEBAHPEAVCMFCRibbonCategory___Z(
    const CMFCRibbonBar* pThis, CMFCRibbonCategory* pCategory)
{
    if (!pThis || !pCategory) return -1;

    std::lock_guard<std::mutex> lock(RibbonMutex());
    auto it = RibbonBarStates().find(const_cast<CMFCRibbonBar*>(pThis));
    if (it == RibbonBarStates().end()) return -1;

    const auto& categories = it->second.categories;
    for (size_t i = 0; i < categories.size(); i++) {
        if (categories[i] == pCategory) {
            return static_cast<int>(i);
        }
    }
    return -1;
}
// CMFCRibbonBar::GetVisibleCategoryCount() -- retail counts categories whose
// visibility flag at +0xa8 is non-zero. Every category OpenMFC creates is
// visible, so this is just the side-table category count.
// Symbol: ?GetVisibleCategoryCount@CMFCRibbonBar@@QEBAHXZ
extern "C" int MS_ABI impl__GetVisibleCategoryCount_CMFCRibbonBar__QEBAHXZ(
    const CMFCRibbonBar* pThis)
{
    if (!pThis) return 0;

    std::lock_guard<std::mutex> lock(RibbonMutex());
    auto it = RibbonBarStates().find(const_cast<CMFCRibbonBar*>(pThis));
    if (it == RibbonBarStates().end()) return 0;
    return static_cast<int>(it->second.categories.size());
}
// CMFCRibbonBar::GetElementsByID(unsigned int, CArray<CMFCRibbonBaseElement*,
// CMFCRibbonBaseElement*>&) -- retail clears the array (SetSize(0)) then
// appends every element whose ID matches from the main category, each
// regular category and the QAT group. Implemented over the side tables for
// the regular categories.
// TODO(clean-room): partially transcribed -- the main-button group and the
// quick access toolbar are not modeled and contribute no elements.
// Symbol: ?GetElementsByID@CMFCRibbonBar@@QEAAXIAEAV?$CArray@PEAVCMFCRibbonBaseElement@@PEAV1@@@@Z
extern "C" void MS_ABI impl__GetElementsByID_CMFCRibbonBar__QEAAXIAEAV__CArray_PEAVCMFCRibbonBaseElement__PEAV1____Z(
    CMFCRibbonBar* pThis, unsigned int uiCmd,
    CArray<CMFCRibbonBaseElement*, CMFCRibbonBaseElement*>& arElements)
{
    if (!pThis) return;
    arElements.RemoveAll();

    std::lock_guard<std::mutex> lock(RibbonMutex());
    auto it = RibbonBarStates().find(pThis);
    if (it == RibbonBarStates().end()) return;

    for (CMFCRibbonCategory* pCategory : it->second.categories) {
        if (!pCategory) continue;
        auto catIt = RibbonCategoryStates().find(pCategory);
        if (catIt == RibbonCategoryStates().end()) continue;
        for (CMFCRibbonPanel* pPanel : catIt->second.panels) {
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
}
// CMFCRibbonBar::GetItemIDsList(CList<UINT, UINT>&, int) -- retail clears
// the list (CList::RemoveAll) then appends the ID of every element in the
// main category and each regular category; when nCategoryIndex == 0 it also
// appends the QAT commands. Implemented over the side tables for the regular
// categories.
// TODO(clean-room): partially transcribed -- QAT commands are not modeled
// and are never appended.
// Symbol: ?GetItemIDsList@CMFCRibbonBar@@QEBAXAEAV?$CList@II@@H@Z
extern "C" void MS_ABI impl__GetItemIDsList_CMFCRibbonBar__QEBAXAEAV__CList_II__H_Z(
    const CMFCRibbonBar* pThis, CList<UINT, UINT>& lstIDs, int /*nCategoryIndex*/)
{
    if (!pThis) return;
    lstIDs.RemoveAll();

    std::lock_guard<std::mutex> lock(RibbonMutex());
    auto it = RibbonBarStates().find(const_cast<CMFCRibbonBar*>(pThis));
    if (it == RibbonBarStates().end()) return;

    for (CMFCRibbonCategory* pCategory : it->second.categories) {
        if (!pCategory) continue;
        auto catIt = RibbonCategoryStates().find(pCategory);
        if (catIt == RibbonCategoryStates().end()) continue;
        for (CMFCRibbonPanel* pPanel : catIt->second.panels) {
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
// CMFCRibbonBar::GetQuickAccessCommands(CList<UINT, UINT>&) -- retail is a
// tail call into the QAT group's GetCommands (lea +0x1380 / jmp). OpenMFC
// keeps only a pointer to the QAT group in the side table, never its command
// list, so nothing can be appended; the list is cleared to honour the fill
// contract.
// TODO(clean-room): partially transcribed -- the QAT command list is not
// modeled.
// Symbol: ?GetQuickAccessCommands@CMFCRibbonBar@@QEAAXAEAV?$CList@II@@@Z
extern "C" void MS_ABI impl__GetQuickAccessCommands_CMFCRibbonBar__QEAAXAEAV__CList_II___Z(
    CMFCRibbonBar* pThis, CList<UINT, UINT>& lstCommands)
{
    if (!pThis) return;
    lstCommands.RemoveAll();
}
// CMFCRibbonBar::GetVisibleElements(CArray<CMFCRibbonBaseElement*,
// CMFCRibbonBaseElement*>&) -- retail clears the array then collects the
// main-button group, QAT group, the visible category panel groups and the
// active category. Implemented over the side tables for the regular
// categories (all of which are visible in this library).
// TODO(clean-room): partially transcribed -- main-button group, QAT group
// and active-category elements are not modeled and contribute nothing.
// Symbol: ?GetVisibleElements@CMFCRibbonBar@@QEAAXAEAV?$CArray@PEAVCMFCRibbonBaseElement@@PEAV1@@@@Z
extern "C" void MS_ABI impl__GetVisibleElements_CMFCRibbonBar__QEAAXAEAV__CArray_PEAVCMFCRibbonBaseElement__PEAV1____Z(
    CMFCRibbonBar* pThis,
    CArray<CMFCRibbonBaseElement*, CMFCRibbonBaseElement*>& arElements)
{
    if (!pThis) return;
    arElements.RemoveAll();

    std::lock_guard<std::mutex> lock(RibbonMutex());
    auto it = RibbonBarStates().find(pThis);
    if (it == RibbonBarStates().end()) return;

    for (CMFCRibbonCategory* pCategory : it->second.categories) {
        if (!pCategory) continue;
        auto catIt = RibbonCategoryStates().find(pCategory);
        if (catIt == RibbonCategoryStates().end()) continue;
        for (CMFCRibbonPanel* pPanel : catIt->second.panels) {
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
}
// CMFCRibbonBar::FindContextCaption(unsigned int) -- retail scans
// m_arContextCaptions (CArray at +0xb28, m_pData +0xb30 / m_nSize +0xb38)
// and returns the caption whose context ID (+0x274) matches nID.
// OpenMFC has no context-caption state at all: the member array is an opaque
// blob nothing populates and CMFCRibbonContextCaption carries only padding.
// TODO(clean-room): partially transcribed -- context captions are not
// modeled.
// Symbol: ?FindContextCaption@CMFCRibbonBar@@IEBAPEAVCMFCRibbonContextCaption@@I@Z
extern "C" CMFCRibbonContextCaption* MS_ABI impl__FindContextCaption_CMFCRibbonBar__IEBAPEAVCMFCRibbonContextCaption__I_Z(
    const CMFCRibbonBar* pThis, unsigned int /*nID*/)
{
    if (!pThis) return nullptr;
    return nullptr;
}
// CMFCRibbonBar::GetContextName(unsigned int, CString&) -- retail calls
// FindContextCaption(nID); if found, copies the caption's CString name
// (+0xa0) into strName and returns 1, otherwise returns 0. FindContextCaption
// can never match in this library, so the copy path is unreachable.
// TODO(clean-room): partially transcribed -- context captions are not
// modeled.
// Symbol: ?GetContextName@CMFCRibbonBar@@QEBAHIAEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@@Z
extern "C" int MS_ABI impl__GetContextName_CMFCRibbonBar__QEBAHIAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
    const CMFCRibbonBar* pThis, unsigned int nID, CString& /*strName*/)
{
    if (!pThis || nID == 0) return 0;

    CMFCRibbonContextCaption* pCaption =
        impl__FindContextCaption_CMFCRibbonBar__IEBAPEAVCMFCRibbonContextCaption__I_Z(pThis, nID);
    if (pCaption != nullptr) {
        // Retail copies pCaption->m_strText (+0xa0) into strName here.
        return 1;
    }
    return 0;
}
// CMFCRibbonBar::GetVisibleContextCaptionCount() -- retail iterates the
// categories and counts distinct non-zero context IDs (+0xac) among visible
// categories (+0xa8). OpenMFC never assigns context IDs, so the count is 0.
// TODO(clean-room): partially transcribed -- category context IDs are not
// modeled.
// Symbol: ?GetVisibleContextCaptionCount@CMFCRibbonBar@@QEAAHXZ
extern "C" int MS_ABI impl__GetVisibleContextCaptionCount_CMFCRibbonBar__QEAAHXZ(
    CMFCRibbonBar* pThis)
{
    if (!pThis) return 0;
    return 0;
}
// CMFCRibbonBar::GetVisibleContextCaptions(CArray<CMFCRibbonContextCaption*,
// CMFCRibbonContextCaption*>&) -- retail appends the context caption of each
// visible category (no array clear; SetSize grows from the current count).
// No context captions exist here, so nothing is appended.
// TODO(clean-room): partially transcribed -- context captions are not
// modeled.
// Symbol: ?GetVisibleContextCaptions@CMFCRibbonBar@@QEAAXAEAV?$CArray@PEAVCMFCRibbonContextCaption@@PEAV1@@@@Z
extern "C" void MS_ABI impl__GetVisibleContextCaptions_CMFCRibbonBar__QEAAXAEAV__CArray_PEAVCMFCRibbonContextCaption__PEAV1____Z(
    CMFCRibbonBar* pThis,
    CArray<CMFCRibbonContextCaption*, CMFCRibbonContextCaption*>& /*arCaptions*/)
{
    if (!pThis) return;
}
// CMFCRibbonBar::GetVisibleContextCaptions(CArray<int, int>*) -- retail
// appends each visible context ID (+0xac) via SetAtGrow (append-only, no
// clear). No context IDs exist here, so nothing is appended.
// TODO(clean-room): partially transcribed -- category context IDs are not
// modeled.
// Symbol: ?GetVisibleContextCaptions@CMFCRibbonBar@@QEAAXPEAV?$CArray@HH@@@Z
extern "C" void MS_ABI impl__GetVisibleContextCaptions_CMFCRibbonBar__QEAAXPEAV__CArray_HH___Z(
    CMFCRibbonBar* pThis, CArray<int, int>* /*arIDs*/)
{
    if (!pThis) return;
}
// CMFCRibbonBar::ForceRecalcLayout() -- retail sets both recalc flags
// (0x424/0x428), calls CleanUpSizes() on the main category and every
// category, flips a global layout-recalc flag, sets m_bForceRedraw (0x438)
// and repaints the caption area. Only the declared flags can be reproduced;
// CleanUpSizes and the caption repaint are not modeled.
// TODO(clean-room): partially transcribed -- per-category CleanUpSizes()
// calls and the caption repaint are not reproduced.
// Symbol: ?ForceRecalcLayout@CMFCRibbonBar@@QEAAXXZ
extern "C" void MS_ABI impl__ForceRecalcLayout_CMFCRibbonBar__QEAAXXZ(
    CMFCRibbonBar* pThis)
{
    if (!pThis) return;
    pThis->m_bRecalcCategoryHeight = TRUE;
    pThis->m_bRecalcCategoryWidth  = TRUE;
    pThis->m_bForceRedraw          = TRUE;
}
// CMFCRibbonBar::IsCaptionButtons() -- retail returns FALSE if any of the
// three caption-button rects (m_CaptionButtons +0xce0, stride 0x278) is
// empty, else TRUE. The caption-button group is an opaque, never-populated
// blob in OpenMFC (all-zero rects), so IsRectEmpty would be true for each
// and retail would return FALSE.
// TODO(clean-room): partially transcribed -- the caption-button group is not
// modeled.
// Symbol: ?IsCaptionButtons@CMFCRibbonBar@@QEAAHXZ
extern "C" int MS_ABI impl__IsCaptionButtons_CMFCRibbonBar__QEAAHXZ(
    CMFCRibbonBar* pThis)
{
    if (!pThis) return 0;
    return 0;
}
// CMFCRibbonBar::FindNearest(CPoint, const CArray<CMFCRibbonBaseElement*,
// CMFCRibbonBaseElement*>&) [static] -- retail returns the first element
// whose rect (+0xc8) contains the point (PtInRect). The element rect is not
// a declared member of OpenMFC's CMFCRibbonBaseElement, so the containment
// test cannot be reproduced.
// TODO(clean-room): partially transcribed -- needs the element rect member
// at +0xc8.
// Symbol: ?FindNearest@CMFCRibbonBar@@KAPEAVCMFCRibbonBaseElement@@VCPoint@@AEBV?$CArray@PEAVCMFCRibbonBaseElement@@PEAV1@@@@Z
extern "C" CMFCRibbonBaseElement* MS_ABI impl__FindNearest_CMFCRibbonBar__KAPEAVCMFCRibbonBaseElement__VCPoint__AEBV__CArray_PEAVCMFCRibbonBaseElement__PEAV1____Z(
    CPoint /*point*/,
    const CArray<CMFCRibbonBaseElement*, CMFCRibbonBaseElement*>& /*ar*/)
{
    return nullptr;
}
// CMFCRibbonBar::FindNextFocusedElement(int, const CArray<CMFCRibbonBaseElement*,
// CMFCRibbonBaseElement*>&, CRect, CMFCRibbonBaseElement*, int, int, int&)
// [static] -- complex keyboard-navigation helper that scans element rects and
// focus order. Depends on element geometry OpenMFC does not model.
// TODO(clean-room): partially transcribed -- keyboard navigation geometry is
// not modeled.
// Symbol: ?FindNextFocusedElement@CMFCRibbonBar@@KAPEAVCMFCRibbonBaseElement@@HAEBV?$CArray@PEAVCMFCRibbonBaseElement@@PEAV1@@@VCRect@@PEAV2@HHAEAH@Z
extern "C" CMFCRibbonBaseElement* MS_ABI impl__FindNextFocusedElement_CMFCRibbonBar__KAPEAVCMFCRibbonBaseElement__HAEBV__CArray_PEAVCMFCRibbonBaseElement__PEAV1___VCRect__PEAV2_HHAEAH_Z(
    int /*nCmd*/,
    const CArray<CMFCRibbonBaseElement*, CMFCRibbonBaseElement*>& /*ar*/,
    CRect /*rect*/,
    CMFCRibbonBaseElement* /*pFrom*/,
    int /*nDir*/,
    int /*nDir2*/,
    int& nIndex)
{
    nIndex = -1;
    return nullptr;
}
// CMFCRibbonBar::HitTest(CPoint, int, int) -- retail hit-tests the main
// button group, the QAT group, the three caption-button rects and the
// category panels (via FindNearest). The relevant rects and groups are not
// modeled.
// TODO(clean-room): partially transcribed -- group/element geometry is not
// modeled.
// Symbol: ?HitTest@CMFCRibbonBar@@UEAAPEAVCMFCRibbonBaseElement@@VCPoint@@HH@Z
extern "C" CMFCRibbonBaseElement* MS_ABI impl__HitTest_CMFCRibbonBar__UEAAPEAVCMFCRibbonBaseElement__VCPoint__HH_Z(
    CMFCRibbonBar* pThis, CPoint /*pt*/, int /*nRecalcLayout*/, int /*bCheckHighlighted*/)
{
    if (!pThis) return nullptr;
    return nullptr;
}
// CMFCRibbonBar::GetDroppedDown() -- retail queries the main-button group,
// the QAT group, the tab group and the active category's panel group for a
// dropped-down element. None of those groups is modeled.
// TODO(clean-room): partially transcribed -- ribbon groups are not modeled.
// Symbol: ?GetDroppedDown@CMFCRibbonBar@@UEAAPEAVCMFCRibbonBaseElement@@XZ
extern "C" CMFCRibbonBaseElement* MS_ABI impl__GetDroppedDown_CMFCRibbonBar__UEAAPEAVCMFCRibbonBaseElement__XZ(
    CMFCRibbonBar* pThis)
{
    if (!pThis) return nullptr;
    return nullptr;
}
// CMFCRibbonBar::GetFocused() -- retail delegates to GetFocused() on the
// main-button group, the QAT group, the tab group and the active category.
// None of those groups is modeled.
// TODO(clean-room): partially transcribed -- ribbon groups are not modeled.
// Symbol: ?GetFocused@CMFCRibbonBar@@UEAAPEAVCMFCRibbonBaseElement@@XZ
extern "C" CMFCRibbonBaseElement* MS_ABI impl__GetFocused_CMFCRibbonBar__UEAAPEAVCMFCRibbonBaseElement__XZ(
    CMFCRibbonBar* pThis)
{
    if (!pThis) return nullptr;
    return nullptr;
}
// CMFCRibbonBar::NavigateRibbon(int) -- retail implements full keyboard
// navigation (tab/arrow/space, key-tip activation, focus movement across the
// groups). Requires the element geometry and focus model OpenMFC does not
// have; returns "not handled".
// TODO(clean-room): transcribed partially -- keyboard navigation is not
// modeled.
// Symbol: ?NavigateRibbon@CMFCRibbonBar@@IEAAHH@Z
extern "C" int MS_ABI impl__NavigateRibbon_CMFCRibbonBar__IEAAHH_Z(
    CMFCRibbonBar* pThis, int /*nChar*/)
{
    if (!pThis) return 0;
    return 0;
}
// CMFCRibbonBar::OnBeforeProcessKey(int&) -- retail stores
// CKeyboardManager::TranslateCharToUpper(nChar) back into the reference
// (call 0x180074350). pThis is unused in the retail body.
// Symbol: ?OnBeforeProcessKey@CMFCRibbonBar@@MEAAXAEAH@Z
extern "C" void MS_ABI impl__OnBeforeProcessKey_CMFCRibbonBar__MEAAXAEAH_Z(
    CMFCRibbonBar* pThis, int& nChar)
{
    if (!pThis) return;
    nChar = static_cast<int>(
        impl__TranslateCharToUpper_CKeyboardManager__SAII_Z(static_cast<unsigned int>(nChar)));
}
// CMFCRibbonBar::HideKeyTips() -- retail iterates m_arKeyElements
// (CArray at +0xb80) calling CMFCRibbonKeyTip::Hide() on each element.
// OpenMFC has no key-tip state; the array is an opaque blob nothing
// populates.
// TODO(clean-room): partially transcribed -- key tips are not modeled.
// Symbol: ?HideKeyTips@CMFCRibbonBar@@QEAAXXZ
extern "C" void MS_ABI impl__HideKeyTips_CMFCRibbonBar__QEAAXXZ(
    CMFCRibbonBar* pThis)
{
    if (!pThis) return;
}
// CMFCRibbonBar::HideAllContextCategories() -- retail hides every category
// with a non-zero context ID (+0xac), clears the active-category panels'
// highlight state and reselects the active category, returning the number of
// categories hidden. OpenMFC never creates context categories, so there is
// nothing to hide and the count is 0.
// TODO(clean-room): partially transcribed -- context categories are not
// modeled.
// Symbol: ?HideAllContextCategories@CMFCRibbonBar@@QEAAHXZ
extern "C" int MS_ABI impl__HideAllContextCategories_CMFCRibbonBar__QEAAHXZ(
    CMFCRibbonBar* pThis)
{
    if (!pThis) return 0;
    return 0;
}
// CMFCRibbonBar::LoadFromBuffer(const wchar_t*) -- retail parses the XML
// buffer through the ribbon XML loader and builds the whole bar (categories,
// panels, buttons). OpenMFC has no XML ribbon loader.
// TODO(clean-room): transcribed partially -- XML ribbon parsing is not
// modeled.
// Symbol: ?LoadFromBuffer@CMFCRibbonBar@@UEAAHPEB_W@Z
extern "C" int MS_ABI impl__LoadFromBuffer_CMFCRibbonBar__UEAAHPEB_W_Z(
    CMFCRibbonBar* pThis, const wchar_t* /*lpszBuffer*/)
{
    if (!pThis) return 0;
    return 0;
}
// CMFCRibbonBar::LoadFromResource(const wchar_t*, const wchar_t*,
// HINSTANCE) -- retail locates the resource and feeds it to the XML ribbon
// loader. OpenMFC has no XML ribbon loader.
// TODO(clean-room): transcribed partially -- XML ribbon parsing is not
// modeled.
// Symbol: ?LoadFromResource@CMFCRibbonBar@@UEAAHPEB_W0PEAUHINSTANCE__@@@Z
extern "C" int MS_ABI impl__LoadFromResource_CMFCRibbonBar__UEAAHPEB_W0PEAUHINSTANCE_____Z(
    CMFCRibbonBar* pThis, const wchar_t* /*lpszResourceName*/,
    const wchar_t* /*lpszResourceType*/, HINSTANCE /*hResource*/)
{
    if (!pThis) return 0;
    return 0;
}
// CMFCRibbonBar::LoadFromResource(unsigned int, const wchar_t*,
// HINSTANCE) -- retail tail-calls the resource-name overload, truncating the
// numeric ID to a WORD (movzwl %dx,%edx) and passing it as MAKEINTRESOURCEW.
// Symbol: ?LoadFromResource@CMFCRibbonBar@@UEAAHIPEB_WPEAUHINSTANCE__@@@Z
extern "C" int MS_ABI impl__LoadFromResource_CMFCRibbonBar__UEAAHIPEB_WPEAUHINSTANCE_____Z(
    CMFCRibbonBar* pThis, unsigned int nResID, const wchar_t* lpszResourceType,
    HINSTANCE hResource)
{
    if (!pThis) return 0;
    return impl__LoadFromResource_CMFCRibbonBar__UEAAHPEB_W0PEAUHINSTANCE_____Z(
        pThis, MAKEINTRESOURCEW(static_cast<WORD>(nResID)), lpszResourceType, hResource);
}
// CMFCRibbonBar::LoadState(const wchar_t*, int, unsigned int) -- retail
// reads the bar's state (minimized, active category, QAT) from the registry
// via CWinApp/CWinAppEx and reapplies it. Registry-backed ribbon state is
// not modeled.
// TODO(clean-room): transcribed partially -- registry state is not modeled.
// Symbol: ?LoadState@CMFCRibbonBar@@MEAAHPEB_WHI@Z
extern "C" int MS_ABI impl__LoadState_CMFCRibbonBar__MEAAHPEB_WHI_Z(
    CMFCRibbonBar* pThis, const wchar_t* /*lpszProfileName*/, int /*nIndex*/,
    unsigned int /*uiID*/)
{
    if (!pThis) return 0;
    return 0;
}
// CMFCRibbonBar::OnCancelMode() -- retail: CPane::OnCancelMode() ->
// DeactivateKeyboardFocus(FALSE) -> kill the auto-command timer (0xec19) if
// active -> clear m_bTracked (0x42c) -> PopTooltip() -> clear the
// caption-button group -> redraw the caption area (0x105).
//
// CPane::OnCancelMode, DeactivateKeyboardFocus and PopTooltip are not
// exported by OpenMFC, so only the flag/timer/repaint steps are reproduced.
// TODO(clean-room): partially transcribed -- CPane::OnCancelMode,
// DeactivateKeyboardFocus, PopTooltip and the caption-button group reset are
// not modeled.
// Symbol: ?OnCancelMode@CMFCRibbonBar@@IEAAXXZ
extern "C" void MS_ABI impl__OnCancelMode_CMFCRibbonBar__IEAAXXZ(
    CMFCRibbonBar* pThis)
{
    if (!pThis) return;

    if (pThis->m_bAutoCommandTimer && pThis->m_hWnd) {
        ::KillTimer(pThis->m_hWnd, 0xec19);
        pThis->m_bAutoCommandTimer = FALSE;
    }
    pThis->m_bTracked = FALSE;
    // Retail repaints the caption area with RDW_INVALIDATE|RDW_ERASE|
    // RDW_UPDATENOW (0x105); repainting the whole bar is the equivalent
    // partial behaviour.
    pThis->RedrawWindow(nullptr, nullptr, 0x105);
}
// CMFCRibbonBar::GetAccObjectCount() -- retail returns 0 when m_dwHideFlags
// has bit 0x2 set, otherwise sums the accessibility object counts of the
// main-button group, the active category and the visible context captions.
// None of those groups is modeled, so the sum is always 0 here.
// TODO(clean-room): partially transcribed -- group accessibility counts are
// not modeled.
// Symbol: ?GetAccObjectCount@CMFCRibbonBar@@QEAAHXZ
extern "C" int MS_ABI impl__GetAccObjectCount_CMFCRibbonBar__QEAAHXZ(
    CMFCRibbonBar* pThis)
{
    if (!pThis) return 0;
    return 0;
}
// CMFCRibbonBar::GetAccessibleDispatch() -- retail loads the pointer at +0x50
// (the accessibility dispatch in the real CWnd layout) and returns it. OpenMFC's
// CWnd does not declare a pointer at +0x50 (m_nFlags, a DWORD, lives there),
// so no dispatch object can be returned.
// TODO(clean-room): partially transcribed -- the CWnd accessibility dispatch
// member is not modeled.
// Symbol: ?GetAccessibleDispatch@CMFCRibbonBar@@QEAAPEAUIDispatch@@XZ
extern "C" void* MS_ABI impl__GetAccessibleDispatch_CMFCRibbonBar__QEAAPEAUIDispatch__XZ(
    CMFCRibbonBar* pThis)
{
    if (!pThis) return nullptr;
    return nullptr;
}
// CMFCRibbonBar::OnSettingChange(UINT, const wchar_t*) -- retail (RVA 0xdfe50)
// calls the CWnd base handler first, then for nFlags in {0x22 SPI_SETDESKWALLPAPER,
// 0x2a SPI_SETNONCLIENTMETRICS, 0x2f SPI_SETWORKAREA} (bitmask 0x840400000000
// tested with `bt`) calls its own ForceRecalcLayout. The flags match the
// system-wide settings that change font/metrics and force a full ribbon relayout.
// Symbol: ?OnSettingChange@CMFCRibbonBar@@IEAAXIPEB_W@Z
extern "C" void MS_ABI impl__OnSettingChange_CMFCRibbonBar__IEAAXIPEB_W_Z(
    CMFCRibbonBar* pThis, unsigned int nFlags, const wchar_t* lpszNewValue)
{
    if (!pThis) return;

    pThis->CWnd::OnSettingChange(nFlags, lpszNewValue);

    switch (nFlags) {
        case 0x22: // SPI_SETDESKWALLPAPER
        case 0x2a: // SPI_SETNONCLIENTMETRICS
        case 0x2f: // SPI_SETWORKAREA
            impl__ForceRecalcLayout_CMFCRibbonBar__QEAAXXZ(pThis);
            break;
        default:
            break;
    }
}
// CMFCRibbonBar::OnShowRibbonContextMenu(CWnd*, int, int,
// CMFCRibbonBaseElement*) -- retail (RVA 0xdeef0) starts by setting the
// keyboard navigation level, kills the auto-command timer when m_bAutoCommandTimer
// is set, then walks the element/context-menu machinery to build and track the
// popup (GetActiveMenu / frame forwarding). The popup-menu tracking that would
// make the return value meaningful is not modeled.
// TODO(clean-room): partially transcribed -- the popup tracking chain is not
// modeled.
// Symbol: ?OnShowRibbonContextMenu@CMFCRibbonBar@@UEAAHPEAVCWnd@@HHPEAVCMFCRibbonBaseElement@@@Z
extern "C" int MS_ABI impl__OnShowRibbonContextMenu_CMFCRibbonBar__UEAAHPEAVCWnd__HHPEAVCMFCRibbonBaseElement___Z(
    CMFCRibbonBar* pThis, CWnd* /*pWnd*/, int /*x*/, int /*y*/,
    CMFCRibbonBaseElement* /*pElem*/)
{
    if (!pThis) return 0;
    return 0;
}
// CMFCRibbonBar::OnShowRibbonQATMenu(CWnd*, int, int,
// CMFCRibbonBaseElement*) -- retail (RVA 0xdf8e0) saves m_bIsCustomizeMenu,
// sets it TRUE, dispatches the bar's virtual at vtable+0x6d8 (the QAT menu
// builder), then restores the flag. OpenMFC does not model the vtable layout,
// so the dispatch cannot be reproduced; the flag save/restore is kept.
// TODO(clean-room): partially transcribed -- the vtable+0x6d8 dispatch is not
// modeled.
// Symbol: ?OnShowRibbonQATMenu@CMFCRibbonBar@@UEAAHPEAVCWnd@@HHPEAVCMFCRibbonBaseElement@@@Z
extern "C" int MS_ABI impl__OnShowRibbonQATMenu_CMFCRibbonBar__UEAAHPEAVCWnd__HHPEAVCMFCRibbonBaseElement___Z(
    CMFCRibbonBar* pThis, CWnd* /*pWnd*/, int /*x*/, int /*y*/,
    CMFCRibbonBaseElement* /*pElem*/)
{
    if (!pThis) return 0;

    int nSaved = pThis->m_bIsCustomizeMenu;
    pThis->m_bIsCustomizeMenu = TRUE;
    // Retail: call of the vtable+0x6d8 virtual would go here.
    pThis->m_bIsCustomizeMenu = nSaved;
    return 0;
}
// CMFCRibbonBar::OnShowWindow(int, UINT) -- retail (RVA 0xe37b0) calls the
// folded CWnd default, then, when hiding a bar with a transparent caption
// (bShow == 0 && m_bIsTransparentCaption), repaints the parent frame caption
// through GetParent() -> FromHandlePermanent() -> an invalidate helper
// (0x1801cad40). The base behaviour is reproduced with DefWindowProcW; the
// frame-caption repaint is unmodeled.
// TODO(clean-room): partially transcribed -- the transparent-caption parent
// repaint flow is not modeled.
// Symbol: ?OnShowWindow@CMFCRibbonBar@@IEAAXHI@Z
extern "C" void MS_ABI impl__OnShowWindow_CMFCRibbonBar__IEAAXHI_Z(
    CMFCRibbonBar* pThis, int bShow, unsigned int nStatus)
{
    if (!pThis) return;

    if (pThis->m_hWnd) {
        ::DefWindowProcW(pThis->m_hWnd, WM_SHOWWINDOW,
                         static_cast<WPARAM>(bShow), static_cast<LPARAM>(nStatus));
    }
    if (bShow == 0 && pThis->m_bIsTransparentCaption) {
        // Retail repaints the parent's caption area here.
    }
}
// CMFCRibbonBar::OnSize(UINT, int, int) -- retail (RVA 0xda1d0) calls the
// CWnd base handler, updates m_bIsMaximized from IsZoomed(GetParent()), then
// toggles bit 1 of m_dwHideFlags whenever the "currently hidden" state stops
// matching "client width < 300" (hiding the category strip on very narrow
// bars). When no toggle is needed it dispatches the RecalcLayout virtual
// (vtable+0x430) and sets m_bForceRedraw if the maximized state changed; when
// a toggle happens it clears the main button rect (+0xc8, unmodeled). Both
// paths tail-call an internal tooltip-update helper (0x1800e15b0, unmodeled).
// TODO(clean-room): partially transcribed -- main-button rect clear and the
// tooltip-update tail are not modeled.
// Symbol: ?OnSize@CMFCRibbonBar@@IEAAXIHH@Z
extern "C" void MS_ABI impl__OnSize_CMFCRibbonBar__IEAAXIHH_Z(
    CMFCRibbonBar* pThis, unsigned int /*nType*/, int cx, int /*cy*/)
{
    if (!pThis) return;

    BOOL bOldMaximized = pThis->m_bIsMaximized;
    HWND hParent = pThis->m_hWnd ? ::GetParent(pThis->m_hWnd) : nullptr;
    BOOL bParentZoomed = (hParent && ::IsZoomed(hParent)) ? TRUE : FALSE;
    pThis->m_bIsMaximized = bParentZoomed;

    const bool bHideFlagSet = (pThis->m_dwHideFlags & 0x2) != 0;
    const bool bNarrow = (cx < 300);
    if (bHideFlagSet != bNarrow) {
        // Retail: set bit 1 of m_dwHideFlags when the bar becomes narrow,
        // clear it when it becomes wide again.
        if (bNarrow) {
            pThis->m_dwHideFlags |= 0x2;
        } else {
            pThis->m_dwHideFlags &= ~0x2u;
        }
        return;
    }

    if (bParentZoomed != bOldMaximized) {
        pThis->m_bForceRedraw = TRUE;
    }
    // Retail dispatches the RecalcLayout virtual here.
    impl__RecalcLayout_CMFCRibbonBar__UEAAXXZ(pThis);
}
// CMFCRibbonBar::OnSizing(UINT, RECT*) -- retail (RVA 0xdf930) forwards a
// message to a global singleton layout window (0x1803be288) and tail-calls the
// folded CWnd default. Neither the singleton nor its constraint logic is
// modeled; leaving the size unconstrained is the safe equivalent.
// TODO(clean-room): transcribed partially -- the singleton constraint forward
// is not modeled.
// Symbol: ?OnSizing@CMFCRibbonBar@@IEAAXIPEAUtagRECT@@@Z
extern "C" void MS_ABI impl__OnSizing_CMFCRibbonBar__IEAAXIPEAUtagRECT___Z(
    CMFCRibbonBar* pThis, unsigned int /*nSide*/, RECT* /*lpRect*/)
{
    if (!pThis) return;
}
// CMFCRibbonBar::OnSysColorChange() -- retail (RVA 0xe1120) calls the
// CPane/CWnd base handler (InvalidateRect), notifies the application object
// through a vtable dispatch, and repaints the whole bar with
// RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW|RDW_FRAME (0x505). The app-object
// notification is unmodeled; the repaints are reproduced.
// TODO(clean-room): partially transcribed -- the CWinApp virtual notification
// is not modeled.
// Symbol: ?OnSysColorChange@CMFCRibbonBar@@IEAAXXZ
extern "C" void MS_ABI impl__OnSysColorChange_CMFCRibbonBar__IEAAXXZ(
    CMFCRibbonBar* pThis)
{
    if (!pThis) return;

    // Base chain (CPane::OnSysColorChange -> CWnd::OnSysColorChange).
    pThis->CWnd::OnSysColorChange();

    pThis->RedrawWindow(nullptr, nullptr, 0x505);
}
// CMFCRibbonBar::OnSysCommand(UINT, __int64) -- retail (RVA 0xe12f0): when
// m_bIsTransparentCaption is clear it tail-calls the folded CWnd default.
// Otherwise it forwards WM_SYSCOMMAND to the parent window, converting
// SC_MAXIMIZE (0xf030) to SC_RESTORE (0xf120) when the parent is already
// zoomed so the caption button toggles. The FromHandlePermanent round trip
// only serves to obtain the parent HWND and is collapsed into GetParent().
// Symbol: ?OnSysCommand@CMFCRibbonBar@@IEAAXI_J@Z
extern "C" void MS_ABI impl__OnSysCommand_CMFCRibbonBar__IEAAXI_J_Z(
    CMFCRibbonBar* pThis, unsigned int nID, __int64 lParam)
{
    if (!pThis) return;

    if (!pThis->m_bIsTransparentCaption) {
        if (pThis->m_hWnd) {
            ::DefWindowProcW(pThis->m_hWnd, WM_SYSCOMMAND,
                             static_cast<WPARAM>(nID), static_cast<LPARAM>(lParam));
        }
        return;
    }

    unsigned int nCmd = nID;
    if (nCmd == 0xf030) { // SC_MAXIMIZE
        HWND hParent = pThis->m_hWnd ? ::GetParent(pThis->m_hWnd) : nullptr;
        nCmd = (hParent && ::IsZoomed(hParent)) ? 0xf120 /*SC_RESTORE*/ : 0xf030;
    }
    HWND hParent = pThis->m_hWnd ? ::GetParent(pThis->m_hWnd) : nullptr;
    if (hParent) {
        ::SendMessageW(hParent, WM_SYSCOMMAND,
                       static_cast<WPARAM>(nCmd), static_cast<LPARAM>(lParam));
    }
}
// CMFCRibbonBar::OnSysKeyDown(CFrameWnd*, WPARAM, LPARAM) -- retail
// (RVA 0xe3580): VK_SPACE (0x20) computes a point at (client.left+4,
// client.bottom+23), feeds it to an internal mouse helper (0x1800deb10),
// removes all key tips and kills timer 0xec1a, returning 0. When key tips are
// disabled it consumes VK_F10 (0x79) and VK_MENU (0x12). With key tips
// enabled it reads GetKeyState(VK_CONTROL)/GetKeyState(VK_SHIFT) and drives
// the unmodeled key-tip navigation.
// TODO(clean-room): partially transcribed -- key-tip navigation and the
// internal mouse helper are not modeled.
// Symbol: ?OnSysKeyDown@CMFCRibbonBar@@QEAAHPEAVCFrameWnd@@_K_J@Z
extern "C" int MS_ABI impl__OnSysKeyDown_CMFCRibbonBar__QEAAHPEAVCFrameWnd___K_J_Z(
    CMFCRibbonBar* pThis, CFrameWnd* /*pFrame*/, unsigned __int64 nChar,
    __int64 /*lParam*/)
{
    if (!pThis) return 0;

    if (nChar == 0x20) { // VK_SPACE
        // Retail: internal mouse-move helper at (left+4, bottom+23), then
        // RemoveAllKeys and KillTimer(0xec1a).
        pThis->KillTimer(0xec1a);
        return 0;
    }
    if (!pThis->m_bKeyTips) {
        return (nChar == 0x79 || nChar == 0x12) ? 1 : 0;
    }
    return 0;
}
// CMFCRibbonBar::OnSysKeyUp(CFrameWnd*, WPARAM, LPARAM) -- retail
// (RVA 0xe3710): when key tips are disabled it consumes VK_F10 and VK_MENU,
// returning 1. With key tips enabled it kills timer 0xec1a and, on VK_MENU,
// restores keyboard focus (GetFocus/FromHandlePermanent chain, unmodeled) and
// repaints the bar (0x105).
// TODO(clean-room): partially transcribed -- the focus-restore chain is not
// modeled.
// Symbol: ?OnSysKeyUp@CMFCRibbonBar@@QEAAHPEAVCFrameWnd@@_K_J@Z
extern "C" int MS_ABI impl__OnSysKeyUp_CMFCRibbonBar__QEAAHPEAVCFrameWnd___K_J_Z(
    CMFCRibbonBar* pThis, CFrameWnd* /*pFrame*/, unsigned __int64 nChar,
    __int64 /*lParam*/)
{
    if (!pThis) return 0;

    if (!pThis->m_bKeyTips) {
        return (nChar == 0x79 || nChar == 0x12) ? 1 : 0;
    }
    pThis->KillTimer(0xec1a);
    if (nChar == 0x12) { // VK_MENU
        // Retail: restore focus to the bar or frame, then repaint.
        pThis->RedrawWindow(nullptr, nullptr, 0x105);
        return 1;
    }
    return 0;
}
// CMFCRibbonBar::OnTimer(WPARAM) -- retail (RVA 0xe02d0): timer 0xec19 is the
// auto-command timer -- when m_pPressed is set it hit-tests the cursor against
// the pressed element's rect (+0xc8) and an element virtual at vtable+0x2b0,
// killing the timer when the repeat finishes. Timer 0xec1a resets the
// keyboard-navigation level (SetKeyboardNavigationLevel) and kills itself.
// The pressed-element rect/vtable are not modeled, so only the 0xec1a path is
// reproduced; the base CWnd default is DefWindowProcW.
// TODO(clean-room): partially transcribed -- the 0xec19 auto-repeat path
// needs the element rect at +0xc8.
// Symbol: ?OnTimer@CMFCRibbonBar@@IEAAX_K@Z
extern "C" void MS_ABI impl__OnTimer_CMFCRibbonBar__IEAAX_K_Z(
    CMFCRibbonBar* pThis, unsigned __int64 nIDEvent)
{
    if (!pThis) return;

    if (nIDEvent == 0xec19) {
        if (pThis->m_pPressed) {
            // Retail: GetCursorPos + ScreenToClient + PtInRect against the
            // pressed element rect (+0xc8); unmodeled.
        }
    } else if (nIDEvent == 0xec1a) {
        impl__SetKeyboardNavigationLevel_CMFCRibbonBar__QEAAXPEAVCObject__H_Z(
            pThis, nullptr, 0);
        pThis->KillTimer(0xec1a);
    }

    // Base CWnd::OnTimer -> folded default -> DefWindowProcW.
    if (pThis->m_hWnd) {
        ::DefWindowProcW(pThis->m_hWnd, WM_TIMER,
                         static_cast<WPARAM>(nIDEvent), 0);
    }
}
// CMFCRibbonBar::OnUpdateCmdUI(CFrameWnd*, BOOL) -- retail (RVA 0xddc30)
// dispatches the active category's OnUpdateCmdUI virtual, then the QAT group
// (m_QAToolbar, +0x1380) and the tab group (m_TabElements, +0x498) update
// virtuals. All four objects are unmodeled opaque blobs.
// TODO(clean-room): transcribed partially -- the group/category update
// virtuals are not modeled.
// Symbol: ?OnUpdateCmdUI@CMFCRibbonBar@@MEAAXPEAVCFrameWnd@@H@Z
extern "C" void MS_ABI impl__OnUpdateCmdUI_CMFCRibbonBar__MEAAXPEAVCFrameWnd__H_Z(
    CMFCRibbonBar* pThis, CFrameWnd* /*pFrame*/, int /*bDisableIfNoHndler*/)
{
    if (!pThis) return;
}
// CMFCRibbonBar::OnUpdateToolTips(WPARAM, LPARAM) -- retail (RVA 0xde7f0):
// when (wParam & 0x200) it walks the tooltip group behind +0xc00 and sends the
// private message 0x418 to its window, then continues the tooltip layout
// machinery. The +0xc00 tooltip group is not modeled.
// TODO(clean-room): transcribed partially -- the tooltip group is not modeled.
// Symbol: ?OnUpdateToolTips@CMFCRibbonBar@@IEAA_J_K_J@Z
extern "C" __int64 MS_ABI impl__OnUpdateToolTips_CMFCRibbonBar__IEAA_J_K_J_Z(
    CMFCRibbonBar* pThis, unsigned __int64 /*wParam*/, __int64 /*lParam*/)
{
    if (!pThis) return 0;
    return 0;
}
// CMFCRibbonBar::PreTranslateMessage(MSG*) -- retail (RVA 0xde560) forwards
// key/mouse messages (WM_KEYDOWN, WM_SYSKEYDOWN, WM_MOUSEMOVE, WM_LBUTTONDOWN/
// UP, WM_RBUTTONDOWN/UP, WM_MBUTTONDOWN/UP) to a hidden tooltip window behind
// +0xc00 via the private message 0x407, and on WM_LBUTTONDOWN runs a
// focus-management chain. The +0xc00 tooltip state is not modeled, so the
// function reports "not handled", which is also the retail fall-through value.
// TODO(clean-room): partially transcribed -- the +0xc00 tooltip window and the
// focus chain are not modeled.
// Symbol: ?PreTranslateMessage@CMFCRibbonBar@@UEAAHPEAUtagMSG@@@Z
extern "C" int MS_ABI impl__PreTranslateMessage_CMFCRibbonBar__UEAAHPEAUtagMSG___Z(
    CMFCRibbonBar* pThis, MSG* /*pMsg*/)
{
    if (!pThis) return 0;
    return 0;
}
// CMFCRibbonBar::PopTooltip() -- retail (RVA 0xde8b0) reads the tooltip
// object behind +0xc00 and, when present, sends the private message 0x41c
// (WM_POPUPTOOLTIP) to its window (+0x40). The tooltip object is not modeled.
// TODO(clean-room): transcribed partially -- the tooltip object behind +0xc00
// is not modeled.
// Symbol: ?PopTooltip@CMFCRibbonBar@@QEAAXXZ
extern "C" void MS_ABI impl__PopTooltip_CMFCRibbonBar__QEAAXXZ(
    CMFCRibbonBar* pThis)
{
    if (!pThis) return;
}
// CMFCRibbonBar::PreCreateWindow(CREATESTRUCTW&) -- retail (RVA 0xd9fd0)
// clears style bits 0x80..0x800 (the pane alignment/visibility bits, mask
// 0xfffff07f) from m_dwStyle (0x104) and tail-calls CWnd::PreCreateWindow.
// Symbol: ?PreCreateWindow@CMFCRibbonBar@@MEAAHAEAUtagCREATESTRUCTW@@@Z
extern "C" int MS_ABI impl__PreCreateWindow_CMFCRibbonBar__MEAAHAEAUtagCREATESTRUCTW___Z(
    CMFCRibbonBar* pThis, CREATESTRUCTW& cs)
{
    if (!pThis) return FALSE;

    pThis->m_dwStyle &= 0xfffff07f;
    return impl__PreCreateWindow_CWnd__UEAAHAEAUtagCREATESTRUCTW___Z(pThis, cs);
}
// CMFCRibbonBar::ProcessKey(int) -- retail (RVA 0xe2f30) iterates the key-tip
// elements in m_arKeyElements (CArray at +0xb80) and invokes each key tip's
// matching virtual. OpenMFC has no key-tip state, so nothing can ever match.
// TODO(clean-room): partially transcribed -- key tips are not modeled.
// Symbol: ?ProcessKey@CMFCRibbonBar@@IEAAHH@Z
extern "C" int MS_ABI impl__ProcessKey_CMFCRibbonBar__IEAAHH_Z(
    CMFCRibbonBar* pThis, int /*nKey*/)
{
    if (!pThis) return 0;
    return 0;
}
// CMFCRibbonBar::SetKeyboardNavigationLevel(CObject*, int) -- retail
// (RVA 0xe1db0) returns immediately when key tips are disabled or the bar is
// hidden (m_dwHideFlags bit 1); otherwise it removes all key tips, resets the
// navigation cursor (m_nCurrKeyChar, m_pKeyboardNavLevelParent,
// m_pKeyboardNavLevelCurrent, m_nKeyboardNavLevel) and drives the key-tip
// show/hide machinery through GetFocus/IsChild checks and the category groups.
// The declared-member cursor reset is reproduced; the key-tip machinery is
// not.
// TODO(clean-room): partially transcribed -- the key-tip show/hide and focus
// checks are not modeled.
// Symbol: ?SetKeyboardNavigationLevel@CMFCRibbonBar@@QEAAXPEAVCObject@@H@Z
extern "C" void MS_ABI impl__SetKeyboardNavigationLevel_CMFCRibbonBar__QEAAXPEAVCObject__H_Z(
    CMFCRibbonBar* pThis, CObject* pObj, int /*nLevel*/)
{
    if (!pThis) return;
    if (!pThis->m_bKeyTips) return;
    if ((pThis->m_dwHideFlags & 0x2) != 0) return;

    pThis->m_nCurrKeyChar = 0;
    pThis->m_pKeyboardNavLevelParent = nullptr;
    pThis->m_pKeyboardNavLevelCurrent = pObj;
    pThis->m_nKeyboardNavLevel = 0;
}
// CMFCRibbonBar::RecalcLayout() -- retail (RVA 0xdc480) is a large geometry
// pass over the main category, the caption buttons, the QAT and the tab group
// (including a hidden-element sweep via 0x410 and CleanUpSizes). None of that
// geometry is modeled; a no-op is the safe behaviour because nothing has been
// laid out in the first place.
// TODO(clean-room): transcribed partially -- ribbon geometry is not modeled.
// Symbol: ?RecalcLayout@CMFCRibbonBar@@UEAAXXZ
extern "C" void MS_ABI impl__RecalcLayout_CMFCRibbonBar__UEAAXXZ(
    CMFCRibbonBar* pThis)
{
    if (!pThis) return;
}
// CMFCRibbonBar::RemoveAllCategories() -- retail (RVA 0xdb150) clears
// m_pPrintPreviewCategory when it is one of the removed categories, deletes
// every category in m_arCategories (virtual dtor + operator delete) and every
// context caption, then RemoveAll on both arrays. Implemented over the ribbon
// side table, which is where OpenMFC keeps the category list; context captions
// do not exist here.
// Symbol: ?RemoveAllCategories@CMFCRibbonBar@@QEAAXXZ
extern "C" void MS_ABI impl__RemoveAllCategories_CMFCRibbonBar__QEAAXXZ(
    CMFCRibbonBar* pThis)
{
    if (!pThis) return;

    std::vector<CMFCRibbonCategory*> categories;
    {
        std::lock_guard<std::mutex> lock(RibbonMutex());
        auto it = RibbonBarStates().find(pThis);
        if (it == RibbonBarStates().end()) return;
        RibbonBarState& state = it->second;
        categories = state.categories;
        state.categories.clear();
        state.ownedCategories.clear();
        state.activeCategory = nullptr;
    }

    for (CMFCRibbonCategory* pCategory : categories) {
        if (!pCategory) continue;
        impl___1CMFCRibbonCategory__UEAA_XZ(pCategory);
        ::operator delete(pCategory);
    }
}
// CMFCRibbonBar::RemoveCategory(int) -- retail (RVA 0xdaf90) bounds-checks the
// index, deletes the category (virtual dtor + operator delete), shifts the
// array (memmove) and, when the active category was removed, reselects the
// category at min(nIndex, count-1) if it is visible (+0xa8) or the first
// visible category, clearing the active pointer if none is visible. Implemented
// over the ribbon side table; every OpenMFC category is visible (no +0xa8
// member), so the first candidate always wins.
// Symbol: ?RemoveCategory@CMFCRibbonBar@@QEAAHH@Z
extern "C" int MS_ABI impl__RemoveCategory_CMFCRibbonBar__QEAAHH_Z(
    CMFCRibbonBar* pThis, int nIndex)
{
    if (!pThis || nIndex < 0) return 0;

    CMFCRibbonCategory* pRemoved = nullptr;
    {
        std::lock_guard<std::mutex> lock(RibbonMutex());
        auto it = RibbonBarStates().find(pThis);
        if (it == RibbonBarStates().end()) return 0;
        RibbonBarState& state = it->second;
        if (nIndex >= static_cast<int>(state.categories.size())) return 0;

        pRemoved = state.categories[static_cast<size_t>(nIndex)];
        state.categories.erase(state.categories.begin() + nIndex);
        state.ownedCategories.erase(pRemoved);

        if (state.activeCategory == pRemoved) {
            if (state.categories.empty()) {
                state.activeCategory = nullptr;
            } else {
                size_t pick = static_cast<size_t>(nIndex);
                if (pick >= state.categories.size()) {
                    pick = state.categories.size() - 1;
                }
                state.activeCategory = state.categories[pick];
            }
        }
    }

    if (pRemoved) {
        impl___1CMFCRibbonCategory__UEAA_XZ(pRemoved);
        ::operator delete(pRemoved);
    }
    return 1;
}
// CMFCRibbonBar::RemoveAllFromTabs() -- retail (RVA 0xde250) clears the
// m_TabElements group (CArray RemoveAll at +0x498). OpenMFC tracks the tab
// elements in the ribbon side table, so the equivalent is clearing that list.
// Symbol: ?RemoveAllFromTabs@CMFCRibbonBar@@QEAAXXZ
extern "C" void MS_ABI impl__RemoveAllFromTabs_CMFCRibbonBar__QEAAXXZ(
    CMFCRibbonBar* pThis)
{
    if (!pThis) return;

    std::lock_guard<std::mutex> lock(RibbonMutex());
    auto it = RibbonBarStates().find(pThis);
    if (it == RibbonBarStates().end()) return;
    it->second.tabs.clear();
}
// CMFCRibbonBar::SetActiveMDIChild(CWnd*) -- retail (RVA 0xe0250) iterates the
// tab elements (m_TabElements +0x620/+0x628) and writes pWnd's m_hWnd into
// each tab that is kind-of the MDI tab group (element +0x270). The tab group
// and the element slot are not modeled.
// TODO(clean-room): transcribed partially -- the tab element group and its
// +0x270 MDI-child slot are not modeled.
// Symbol: ?SetActiveMDIChild@CMFCRibbonBar@@QEAAXPEAVCWnd@@@Z
extern "C" void MS_ABI impl__SetActiveMDIChild_CMFCRibbonBar__QEAAXPEAVCWnd___Z(
    CMFCRibbonBar* pThis, CWnd* /*pWnd*/)
{
    if (!pThis) return;
}
// CMFCRibbonBar::RemoveAllKeys() -- retail (RVA 0xe3160) removes and destroys
// every key tip in m_arKeyElements (CArray at +0xb80). OpenMFC has no key-tip
// state; the array is an opaque blob nothing populates.
// TODO(clean-room): partially transcribed -- key tips are not modeled.
// Symbol: ?RemoveAllKeys@CMFCRibbonBar@@IEAAXXZ
extern "C" void MS_ABI impl__RemoveAllKeys_CMFCRibbonBar__IEAAXXZ(
    CMFCRibbonBar* pThis)
{
    if (!pThis) return;
}
// CMFCRibbonBar::SetElementKeys(UINT, const wchar_t*, const wchar_t*) --
// retail (RVA 0xddfa0) collects all elements with the given command ID and
// assigns each its key tip (element virtual at vtable+0x180); it returns
// FALSE when no element matched, otherwise TRUE. The key-tip assignment
// virtual is not modeled and no element key tips exist in OpenMFC.
// TODO(clean-room): transcribed partially -- key-tip assignment is not
// modeled.
// Symbol: ?SetElementKeys@CMFCRibbonBar@@QEAAHIPEB_W0@Z
extern "C" int MS_ABI impl__SetElementKeys_CMFCRibbonBar__QEAAHIPEB_W0_Z(
    CMFCRibbonBar* pThis, unsigned int /*uiCmd*/, const wchar_t* /*lpszKeys*/,
    const wchar_t* /*lpszDescr*/)
{
    if (!pThis) return 0;
    return 0;
}
// CMFCRibbonBar::SaveState(const wchar_t*, int, UINT) -- retail (RVA 0xdf970)
// writes the bar's minimized state, active category index and QAT commands to
// the registry through CWinAppEx. Registry-backed ribbon state is not modeled.
// TODO(clean-room): transcribed partially -- registry state is not modeled.
// Symbol: ?SaveState@CMFCRibbonBar@@MEAAHPEB_WHI@Z
extern "C" int MS_ABI impl__SaveState_CMFCRibbonBar__MEAAHPEB_WHI_Z(
    CMFCRibbonBar* pThis, const wchar_t* /*lpszProfileName*/, int /*nIndex*/,
    unsigned int /*uiID*/)
{
    if (!pThis) return 0;
    return 0;
}
// CMFCRibbonBar::SaveToXMLBuffer(unsigned char**) -- retail (RVA 0xe4640)
// serializes the whole ribbon into an XML writer, detaches the buffer into
// *ppBuffer and returns its length. OpenMFC has no ribbon XML writer.
// TODO(clean-room): transcribed partially -- the XML serializer is not
// modeled.
// Symbol: ?SaveToXMLBuffer@CMFCRibbonBar@@QEBAIPEAPEAE@Z
extern "C" unsigned int MS_ABI impl__SaveToXMLBuffer_CMFCRibbonBar__QEBAIPEAPEAE_Z(
    const CMFCRibbonBar* pThis, unsigned char** ppBuffer)
{
    if (!pThis) return 0;
    if (ppBuffer) *ppBuffer = nullptr;
    return 0;
}
// CMFCRibbonBar::SaveToXMLFile(const wchar_t*) -- retail (RVA 0xe45a0)
// calls SaveToXMLBuffer and writes the returned buffer to the file. With no
// XML serializer the save always fails.
// TODO(clean-room): transcribed partially -- the XML serializer is not
// modeled.
// Symbol: ?SaveToXMLFile@CMFCRibbonBar@@QEBAHPEB_W@Z
extern "C" int MS_ABI impl__SaveToXMLFile_CMFCRibbonBar__QEBAHPEB_W_Z(
    const CMFCRibbonBar* pThis, const wchar_t* /*lpszFileName*/)
{
    if (!pThis) return 0;
    return 0;
}
// CMFCRibbonBar::SetApplicationButton(CMFCRibbonApplicationButton*, CSize) --
// retail (RVA 0xda2e0): stores the button into m_pMainButton (+0x480), sets
// the button's back-pointer to the bar (+0xd8, undeclared, skipped) and
// stores the size into m_sizeMainButton (+0xbf8); a null button zeroes the
// size. CMFCRibbonApplicationButton is not declared in OpenMFC, so the
// parameter is taken as void*.
// TODO(clean-room): partially transcribed -- the button back-pointer at +0xd8
// is not modeled.
// Symbol: ?SetApplicationButton@CMFCRibbonBar@@QEAAXPEAVCMFCRibbonApplicationButton@@VCSize@@@Z
extern "C" void MS_ABI impl__SetApplicationButton_CMFCRibbonBar__QEAAXPEAVCMFCRibbonApplicationButton__VCSize___Z(
    CMFCRibbonBar* pThis, void* pButton, CSize size)
{
    if (!pThis) return;

    pThis->m_pMainButton = static_cast<CMFCRibbonButton*>(pButton);
    if (pButton != nullptr) {
        pThis->m_sizeMainButton = size;
    } else {
        pThis->m_sizeMainButton = CSize(0, 0);
    }
}
// CMFCRibbonBar::SetMaximizeMode(BOOL, CWnd*) -- retail (RVA 0xdffa0) returns
// immediately when the mode is unchanged, otherwise it rebuilds the caption
// buttons and re-links the system menu (unmodeled), then stores the mode into
// m_bMaximizeMode, clears m_pHighlighted/m_pPressed, dispatches the
// RecalcLayout virtual and repaints the bar (0x105). The declared-member tail
// is reproduced.
// TODO(clean-room): partially transcribed -- the caption-button rebuild and
// system-menu re-linking are not modeled.
// Symbol: ?SetMaximizeMode@CMFCRibbonBar@@QEAAXHPEAVCWnd@@@Z
extern "C" void MS_ABI impl__SetMaximizeMode_CMFCRibbonBar__QEAAXHPEAVCWnd___Z(
    CMFCRibbonBar* pThis, int bMax, CWnd* /*pWnd*/)
{
    if (!pThis) return;
    if (pThis->m_bMaximizeMode == bMax) return;

    pThis->m_bMaximizeMode = bMax;
    pThis->m_pHighlighted = nullptr;
    pThis->m_pPressed = nullptr;
    impl__RecalcLayout_CMFCRibbonBar__UEAAXXZ(pThis);
    pThis->RedrawWindow(nullptr, nullptr, 0x105);
}
// CMFCRibbonBar::EnableKeyTips(BOOL) — retail is a single store:
//     mov DWORD PTR [rcx+0x458],edx ; ret
// It stores the argument verbatim, with no normalization to TRUE/FALSE and no
// repaint or layout side effect.
// Symbol: ?EnableKeyTips@CMFCRibbonBar@@QEAAXH@Z
extern "C" void MS_ABI impl__EnableKeyTips_CMFCRibbonBar__QEAAXH_Z(
    CMFCRibbonBar* pThis, int bEnable)
{
    if (!pThis) return;
    pThis->m_bKeyTips = bEnable;
}
// CMFCRibbonBar::EnableToolTips(BOOL, BOOL) — retail stores both arguments and
// returns:
//     mov DWORD PTR [rcx+0x450],edx    ; m_bToolTip
//     mov DWORD PTR [rcx+0x454],r8d    ; m_bToolTipDescr
//     ret
// Note the second flag is stored unconditionally -- it is not gated on the
// first -- so a caller can disable tooltips while leaving the description flag
// set, and re-enabling restores the previous description behaviour.
// Symbol: ?EnableToolTips@CMFCRibbonBar@@QEAAXHH@Z
extern "C" void MS_ABI impl__EnableToolTips_CMFCRibbonBar__QEAAXHH_Z(
    CMFCRibbonBar* pThis, int bEnable, int bEnableDescr)
{
    if (!pThis) return;
    pThis->m_bToolTip      = bEnable;
    pThis->m_bToolTipDescr = bEnableDescr;
}
CMFCRibbonBar::CMFCRibbonBar(BOOL bReplaceFrameCaption) {
    // Every member in this span is POD -- the four large embedded ribbon
    // objects are deliberately opaque byte blocks (see afxmfc.h), and CRect /
    // CSize are plain structs -- so unlike the CMFCToolBar and
    // CMFCToolBarImages constructors there is nothing here whose vfptr a
    // blanket memset could destroy. If any of those blocks is ever replaced by
    // a real class, this memset has to be split around it.
    char* const base = reinterpret_cast<char*>(this);
    std::memset(base + sizeof(CPane), 0, sizeof(CMFCRibbonBar) - sizeof(CPane));

    // Non-zero defaults, read off the retail constructor rather than guessed.
    // It zeroes via r15 and writes ebp==1 / eax==-1 to these slots:
    m_nHighlightedTab         = -1;    // 0x404, eax
    m_nKeyboardNavLevel       = -1;    // 0x414, eax
    m_nTooltipWidthRegular    = 210;   // 0x41c, literal 0xd2
    m_nTooltipWidthLargeImage = 318;   // 0x420, literal 0x13e
    m_bRecalcCategoryHeight   = TRUE;  // 0x424, ebp
    m_bRecalcCategoryWidth    = TRUE;  // 0x428, rbp (low half; 0x42c stays 0)
    m_bIsPrintPreview         = TRUE;  // 0x430, ebp
    m_bToolTip                = TRUE;  // 0x450, ebp
    m_bToolTipDescr           = TRUE;  // 0x454, ebp
    m_bKeyTips                = TRUE;  // 0x458, rbp (low half; 0x45c stays 0)
    m_bReplaceFrameCaption    = bReplaceFrameCaption;  // 0x468, the ctor arg

    g_ribbonBarStates[this];
}
CMFCRibbonBar::~CMFCRibbonBar() {
    ClearRibbonBarState(this);
}
BOOL CMFCRibbonBar::Create(CWnd* pParentWnd, DWORD dwStyle, UINT nID) {
    if (!pParentWnd) return FALSE;
    g_ribbonBarStates[this].parent = pParentWnd;
    m_hWnd = ::CreateWindowExW(0, L"STATIC", nullptr,
                                dwStyle, 0, 0, 0, 0,
                                pParentWnd->GetSafeHwnd(), (HMENU)(UINT_PTR)nID,
                                AfxGetInstanceHandle(), nullptr);
    if (!m_hWnd) {
        ClearRibbonBarState(this);
        return FALSE;
    }
    return TRUE;
}
CMFCRibbonCategory* CMFCRibbonBar::AddCategory(const wchar_t* lpszName, unsigned int uiSmallImage, unsigned int uiLargeImage, CSize, unsigned int, int nInsertAt, CRuntimeClass*) {
    auto* category = new (std::nothrow) CMFCRibbonCategory(lpszName, uiSmallImage, uiLargeImage);
    if (!category) return nullptr;

    RibbonBarState& state = g_ribbonBarStates[this];
    auto insertPos = state.categories.end();
    if (nInsertAt >= 0 && nInsertAt < static_cast<int>(state.categories.size())) {
        insertPos = state.categories.begin() + nInsertAt;
    }
    state.categories.insert(insertPos, category);
    state.ownedCategories.insert(category);
    if (!state.activeCategory) {
        state.activeCategory = category;
    }
    return category;
}
BOOL CMFCRibbonBar::AddCategory(CMFCRibbonCategory* pCategory) {
    if (!pCategory) return FALSE;
    RibbonBarState& state = g_ribbonBarStates[this];
    if (std::find(state.categories.begin(), state.categories.end(), pCategory) == state.categories.end()) {
        state.categories.push_back(pCategory);
    }
    state.ownedCategories.insert(pCategory);
    g_ribbonCategoryStates[pCategory];
    if (!state.activeCategory) {
        state.activeCategory = pCategory;
    }
    return TRUE;
}
int CMFCRibbonBar::GetCategoryCount() const {
    auto it = g_ribbonBarStates.find(this);
    return it == g_ribbonBarStates.end() ? 0 : static_cast<int>(it->second.categories.size());
}
CMFCRibbonCategory* CMFCRibbonBar::GetCategory(int nIndex) const {
    auto it = g_ribbonBarStates.find(this);
    if (it == g_ribbonBarStates.end()) return nullptr;
    if (nIndex < 0 || nIndex >= static_cast<int>(it->second.categories.size())) return nullptr;
    return it->second.categories[static_cast<size_t>(nIndex)];
}
BOOL CMFCRibbonBar::SetActiveCategory(CMFCRibbonCategory* pCategory, BOOL) {
    RibbonBarState& state = g_ribbonBarStates[this];
    if (!pCategory) {
        state.activeCategory = nullptr;
        return TRUE;
    }
    if (std::find(state.categories.begin(), state.categories.end(), pCategory) == state.categories.end()) {
        return FALSE;
    }
    state.activeCategory = pCategory;
    return TRUE;
}
void CMFCRibbonBar::AddToTabs(CMFCRibbonBaseElement* pElement) {
    if (!pElement) return;
    auto& tabs = g_ribbonBarStates[this].tabs;
    if (std::find(tabs.begin(), tabs.end(), pElement) == tabs.end()) {
        tabs.push_back(pElement);
    }
}
void CMFCRibbonBar::SetQuickAccessToolbar(CMFCRibbonButtonsGroup* pQAT) {
    g_ribbonBarStates[this].quickAccessToolbar = pQAT;
}
void CMFCRibbonBar::SetQuickAccessToolbarOnTop(BOOL bOnTop) {
    g_ribbonBarStates[this].quickAccessToolbarOnTop = bOnTop != FALSE;
}
void CMFCRibbonBar::ToggleMimimizeState() {
    RibbonBarState& state = g_ribbonBarStates[this];
    state.minimized = !state.minimized;
}
BOOL CMFCRibbonBar::IsMinimized() const {
    auto it = g_ribbonBarStates.find(this);
    return it != g_ribbonBarStates.end() && it->second.minimized ? TRUE : FALSE;
}
// Symbol: ?accDoDefaultAction@CMFCRibbonBar@@UEAAJUtagVARIANT@@@Z
extern "C" long MS_ABI impl__accDoDefaultAction_CMFCRibbonBar__UEAAJUtagVARIANT___Z(void* /*struct*/ p0) {
    return 0;
}

// Symbol: ?AccessibleObjectByIndex@CMFCRibbonBar@@QEAAPEAVCMFCBaseAccessibleObject@@J@Z
extern "C" void* MS_ABI impl__AccessibleObjectByIndex_CMFCRibbonBar__QEAAPEAVCMFCBaseAccessibleObject__J_Z(long p0) {
    return nullptr;
}

// Symbol: ?AccessibleObjectFromPoint@CMFCRibbonBar@@QEAAPEAVCMFCBaseAccessibleObject@@VCPoint@@@Z
extern "C" void* MS_ABI impl__AccessibleObjectFromPoint_CMFCRibbonBar__QEAAPEAVCMFCBaseAccessibleObject__VCPoint___Z(void* /*class*/ p0) {
    return nullptr;
}

// Symbol: ?accHitTest@CMFCRibbonBar@@UEAAJJJPEAUtagVARIANT@@@Z
extern "C" long MS_ABI impl__accHitTest_CMFCRibbonBar__UEAAJJJPEAUtagVARIANT___Z(long p0, long p1, void* /*struct*/* p2) {
    return 0;
}

// Symbol: ?accLocation@CMFCRibbonBar@@UEAAJPEAJ000UtagVARIANT@@@Z
extern "C" long MS_ABI impl__accLocation_CMFCRibbonBar__UEAAJPEAJ000UtagVARIANT___Z(long* p0, long* p1, long* p2, long* p3, void* /*struct*/ p4) {
    return 0;
}

// Symbol: ?accNavigate@CMFCRibbonBar@@UEAAJJUtagVARIANT@@PEAU2@@Z
extern "C" long MS_ABI impl__accNavigate_CMFCRibbonBar__UEAAJJUtagVARIANT__PEAU2__Z(long p0, void* /*struct*/ p1, void* /*struct*/* p2) {
    return 0;
}

// Symbol: ?ActivateContextCategory@CMFCRibbonBar@@QEAAHI@Z
extern "C" int MS_ABI impl__ActivateContextCategory_CMFCRibbonBar__QEAAHI_Z(unsigned int p0) {
    return 0;
}

// Symbol: ?AddContextCategory@CMFCRibbonBar@@QEAAPEAVCMFCRibbonCategory@@PEB_W0IW4AFX_RibbonCategoryColor@@IIVCSize@@2PEAUCRuntimeClass@@@Z
extern "C" void* MS_ABI impl__AddContextCategory_CMFCRibbonBar__QEAAPEAVCMFCRibbonCategory__PEB_W0IW4AFX_RibbonCategoryColor__IIVCSize__2PEAUCRuntimeClass___Z(const wchar_t* p0, const wchar_t* p1, unsigned int p2, int /*enum*/ p3, short* p4, int p5, void* p6, void* p7, void* p8, void* p9, void* p10, void* p11, void* p12, void* p13, void* p14, void* p15, void* p16, void* p17, void* p18, void* p19, void* p20, void* p21, void* p22, void* p23, void* p24, unsigned int p25, unsigned int p26, void* /*class*/ p27, int /*enum*/ p28, void* /*struct*/* p29) {
    return nullptr;
}

// Symbol: ?AddMainCategory@CMFCRibbonBar@@QEAAPEAVCMFCRibbonMainPanel@@PEB_WIIVCSize@@1PEAUCRuntimeClass@@@Z
extern "C" void* MS_ABI impl__AddMainCategory_CMFCRibbonBar__QEAAPEAVCMFCRibbonMainPanel__PEB_WIIVCSize__1PEAUCRuntimeClass___Z(const wchar_t* p0, unsigned int p1, unsigned int p2, void* /*class*/ p3, unsigned int p4, void* /*struct*/* p5) {
    return nullptr;
}

// Symbol: ?AddPrintPreviewCategory@CMFCRibbonBar@@QEAAPEAVCMFCRibbonCategory@@XZ
extern "C" void* MS_ABI impl__AddPrintPreviewCategory_CMFCRibbonBar__QEAAPEAVCMFCRibbonCategory__XZ() {
    return nullptr;
}

// Symbol: ?AddQATOnlyCategory@CMFCRibbonBar@@QEAAPEAVCMFCRibbonCategory@@PEB_WIVCSize@@@Z
extern "C" void* MS_ABI impl__AddQATOnlyCategory_CMFCRibbonBar__QEAAPEAVCMFCRibbonCategory__PEB_WIVCSize___Z(const wchar_t* p0, unsigned int p1, void* /*class*/ p2) {
    return nullptr;
}

// Symbol: ?CalcFixedLayout@CMFCRibbonBar@@MEAA?AVCSize@@HH@Z
extern "C" void* MS_ABI impl__CalcFixedLayout_CMFCRibbonBar__MEAA_AVCSize__HH_Z(void* /*class*/* p0, int p1, int p2) {
    return nullptr;
}

// Symbol: ?CreateEx@CMFCRibbonBar@@QEAAHPEAVCWnd@@KKI@Z
extern "C" int MS_ABI impl__CreateEx_CMFCRibbonBar__QEAAHPEAVCWnd__KKI_Z(void* /*class*/* p0, unsigned long p1, unsigned long p2, unsigned int p3) {
    return 0;
}

// Symbol: ?DeactivateKeyboardFocus@CMFCRibbonBar@@QEAAXH@Z
extern "C" void MS_ABI impl__DeactivateKeyboardFocus_CMFCRibbonBar__QEAAXH_Z(int p0) {}

// Symbol: ?DrawMenuImage@CMFCRibbonBar@@QEAAHPEAVCDC@@PEBVCMFCToolBarMenuButton@@AEBVCRect@@@Z
extern "C" int MS_ABI impl__DrawMenuImage_CMFCRibbonBar__QEAAHPEAVCDC__PEBVCMFCToolBarMenuButton__AEBVCRect___Z(void* /*class*/* p0, const void* /*class*/* p1, const void* /*class*/* p2) {
    return 0;
}

// Symbol: ?DWMCompositionChanged@CMFCRibbonBar@@UEAAXXZ
extern "C" void MS_ABI impl__DWMCompositionChanged_CMFCRibbonBar__UEAAXXZ() {}

// Symbol: ?EnablePrintPreview@CMFCRibbonBar@@QEAAXH@Z
extern "C" void MS_ABI impl__EnablePrintPreview_CMFCRibbonBar__QEAAXH_Z(int p0) {}

// Symbol: ?FindByData@CMFCRibbonBar@@QEBAPEAVCMFCRibbonBaseElement@@_KH@Z
extern "C" void* MS_ABI impl__FindByData_CMFCRibbonBar__QEBAPEAVCMFCRibbonBaseElement___KH_Z(unsigned __int64 p0, int p1) {
    return nullptr;
}

// Symbol: ?FindByID@CMFCRibbonBar@@QEBAPEAVCMFCRibbonBaseElement@@IHH@Z
extern "C" void* MS_ABI impl__FindByID_CMFCRibbonBar__QEBAPEAVCMFCRibbonBaseElement__IHH_Z(unsigned int p0, int p1, int p2) {
    return nullptr;
}

// Symbol: ?get_accChild@CMFCRibbonBar@@UEAAJUtagVARIANT@@PEAPEAUIDispatch@@@Z
extern "C" long MS_ABI impl__get_accChild_CMFCRibbonBar__UEAAJUtagVARIANT__PEAPEAUIDispatch___Z(void* /*struct*/ p0, void* /*struct*/** p1) {
    return 0;
}

// Symbol: ?get_accChildCount@CMFCRibbonBar@@UEAAJPEAJ@Z
extern "C" long MS_ABI impl__get_accChildCount_CMFCRibbonBar__UEAAJPEAJ_Z(long* p0) {
    return 0;
}

// Symbol: ?OnClickButton@CMFCRibbonBar@@UEAAXPEAVCMFCRibbonButton@@VCPoint@@@Z
extern "C" void MS_ABI impl__OnClickButton_CMFCRibbonBar__UEAAXPEAVCMFCRibbonButton__VCPoint___Z(void* /*class*/* p0, void* /*class*/ p1) {}

// Symbol: ?OnCommand@CMFCRibbonBar@@MEAAH_K_J@Z
extern "C" int MS_ABI impl__OnCommand_CMFCRibbonBar__MEAAH_K_J_Z(unsigned __int64 p0, __int64 p1) {
    return 0;
}

// Symbol: ?OnCreate@CMFCRibbonBar@@IEAAHPEAUtagCREATESTRUCTW@@@Z
extern "C" int MS_ABI impl__OnCreate_CMFCRibbonBar__IEAAHPEAUtagCREATESTRUCTW___Z(void* /*struct*/* p0) {
    return 0;
}

// Symbol: ?OnDestroy@CMFCRibbonBar@@IEAAXXZ
extern "C" void MS_ABI impl__OnDestroy_CMFCRibbonBar__IEAAXXZ() {}

// Symbol: ?OnEditContextMenu@CMFCRibbonBar@@UEAAXPEAVCMFCRibbonRichEditCtrl@@VCPoint@@@Z
extern "C" void MS_ABI impl__OnEditContextMenu_CMFCRibbonBar__UEAAXPEAVCMFCRibbonRichEditCtrl__VCPoint___Z(void* /*class*/* p0, void* /*class*/ p1) {}

// Symbol: ?OnEraseBkgnd@CMFCRibbonBar@@IEAAHPEAVCDC@@@Z
extern "C" int MS_ABI impl__OnEraseBkgnd_CMFCRibbonBar__IEAAHPEAVCDC___Z(void* /*class*/* p0) {
    return 0;
}

// Symbol: ?OnFillBackground@CMFCRibbonBar@@MEAAXPEAVCDC@@VCRect@@@Z
extern "C" void MS_ABI impl__OnFillBackground_CMFCRibbonBar__MEAAXPEAVCDC__VCRect___Z(void* /*class*/* p0, void* /*class*/ p1) {}

// Symbol: ?OnGetFont@CMFCRibbonBar@@IEAAPEAUHFONT__@@XZ
extern "C" void* MS_ABI impl__OnGetFont_CMFCRibbonBar__IEAAPEAUHFONT____XZ() {
    return nullptr;
}

// Symbol: ?OnKillFocus@CMFCRibbonBar@@IEAAXPEAVCWnd@@@Z
extern "C" void MS_ABI impl__OnKillFocus_CMFCRibbonBar__IEAAXPEAVCWnd___Z(void* /*class*/* p0) {}

// Symbol: ?OnLButtonDblClk@CMFCRibbonBar@@IEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnLButtonDblClk_CMFCRibbonBar__IEAAXIVCPoint___Z(unsigned int p0, void* /*class*/ p1) {}

// Symbol: ?OnLButtonDown@CMFCRibbonBar@@IEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnLButtonDown_CMFCRibbonBar__IEAAXIVCPoint___Z(unsigned int p0, void* /*class*/ p1) {}

// Symbol: ?OnLButtonUp@CMFCRibbonBar@@IEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnLButtonUp_CMFCRibbonBar__IEAAXIVCPoint___Z(unsigned int p0, void* /*class*/ p1) {}

// Symbol: ?OnMouseLeave@CMFCRibbonBar@@IEAAXXZ
extern "C" void MS_ABI impl__OnMouseLeave_CMFCRibbonBar__IEAAXXZ() {}

// Symbol: ?OnMouseMove@CMFCRibbonBar@@IEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnMouseMove_CMFCRibbonBar__IEAAXIVCPoint___Z(unsigned int p0, void* /*class*/ p1) {}

// Symbol: ?OnMouseWheel@CMFCRibbonBar@@IEAAHIFVCPoint@@@Z
extern "C" int MS_ABI impl__OnMouseWheel_CMFCRibbonBar__IEAAHIFVCPoint___Z(unsigned int p0, short p1, void* /*class*/ p2) {
    return 0;
}

// Symbol: ?OnNeedTipText@CMFCRibbonBar@@IEAAHIPEAUtagNMHDR@@PEA_J@Z
extern "C" int MS_ABI impl__OnNeedTipText_CMFCRibbonBar__IEAAHIPEAUtagNMHDR__PEA_J_Z(unsigned int p0, void* /*struct*/* p1, __int64* p2) {
    return 0;
}

// Symbol: ?OnPaint@CMFCRibbonBar@@IEAAXXZ
extern "C" void MS_ABI impl__OnPaint_CMFCRibbonBar__IEAAXXZ() {}

// Symbol: ?OnPaneContextMenu@CMFCRibbonBar@@MEAAXPEAVCWnd@@VCPoint@@@Z
extern "C" void MS_ABI impl__OnPaneContextMenu_CMFCRibbonBar__MEAAXPEAVCWnd__VCPoint___Z(void* /*class*/* p0, void* /*class*/ p1) {}

// Symbol: ?OnPostRecalcLayout@CMFCRibbonBar@@IEAA_J_K_J@Z
extern "C" __int64 MS_ABI impl__OnPostRecalcLayout_CMFCRibbonBar__IEAA_J_K_J_Z(unsigned __int64 p0, __int64 p1) {
    return 0;
}

// Symbol: ?OnRTLChanged@CMFCRibbonBar@@UEAAXH@Z
extern "C" void MS_ABI impl__OnRTLChanged_CMFCRibbonBar__UEAAXH_Z(int p0) {}

// Symbol: ?OnSetAccData@CMFCRibbonBar@@UEAAHJ@Z
extern "C" int MS_ABI impl__OnSetAccData_CMFCRibbonBar__UEAAHJ_Z(long p0) {
    return 0;
}

// Symbol: ?OnSetCursor@CMFCRibbonBar@@IEAAHPEAVCWnd@@II@Z
extern "C" int MS_ABI impl__OnSetCursor_CMFCRibbonBar__IEAAHPEAVCWnd__II_Z(void* /*class*/* p0, unsigned int p1, unsigned int p2) {
    return 0;
}

// Symbol: ?OnSetFocus@CMFCRibbonBar@@IEAAXPEAVCWnd@@@Z
extern "C" void MS_ABI impl__OnSetFocus_CMFCRibbonBar__IEAAXPEAVCWnd___Z(void* /*class*/* p0) {}

// Symbol: ?OnSetFont@CMFCRibbonBar@@IEAAXPEAVCFont@@H@Z
extern "C" void MS_ABI impl__OnSetFont_CMFCRibbonBar__IEAAXPEAVCFont__H_Z(void* /*class*/* p0, int p1) {}

// Symbol: ?OnSetPrintPreviewKeys@CMFCRibbonBar@@MEAAXPEAVCMFCRibbonPanel@@00@Z
extern "C" void MS_ABI impl__OnSetPrintPreviewKeys_CMFCRibbonBar__MEAAXPEAVCMFCRibbonPanel__00_Z(void* /*class*/* p0, void* /*class*/* p1, void* /*class*/* p2) {}

// Symbol: ?SetPrintPreviewMode@CMFCRibbonBar@@IEAAXH@Z
extern "C" void MS_ABI impl__SetPrintPreviewMode_CMFCRibbonBar__IEAAXH_Z(int p0) {}

// Symbol: ?SetQuickAccessCommands@CMFCRibbonBar@@QEAAXAEBV?$CList@II@@H@Z
extern "C" void MS_ABI impl__SetQuickAccessCommands_CMFCRibbonBar__QEAAXAEBV__CList_II__H_Z(const void* /*class*/* p0, int p1) {}

// Symbol: ?SetQuickAccessDefaultState@CMFCRibbonBar@@QEAAXAEBVCMFCRibbonQuickAccessToolBarDefaultState@@@Z
extern "C" void MS_ABI impl__SetQuickAccessDefaultState_CMFCRibbonBar__QEAAXAEBVCMFCRibbonQuickAccessToolBarDefaultState___Z(const void* /*class*/* p0) {}

// Symbol: ?SetTooltipFixedWidth@CMFCRibbonBar@@QEAAXHH@Z
extern "C" void MS_ABI impl__SetTooltipFixedWidth_CMFCRibbonBar__QEAAXHH_Z(int p0, int p1) {}

// Symbol: ?SetWindows7Look@CMFCRibbonBar@@QEAAXHH@Z
extern "C" void MS_ABI impl__SetWindows7Look_CMFCRibbonBar__QEAAXHH_Z(int p0, int p1) {}

// Symbol: ?ShowCategory@CMFCRibbonBar@@QEAAXHH@Z
extern "C" void MS_ABI impl__ShowCategory_CMFCRibbonBar__QEAAXHH_Z(int p0, int p1) {}

// Symbol: ?ShowContextCategories@CMFCRibbonBar@@QEAAXIH@Z
extern "C" void MS_ABI impl__ShowContextCategories_CMFCRibbonBar__QEAAXIH_Z(unsigned int p0, int p1) {}

// Symbol: ?ShowKeyTips@CMFCRibbonBar@@QEAAXH@Z
extern "C" void MS_ABI impl__ShowKeyTips_CMFCRibbonBar__QEAAXH_Z(int p0) {}

// Symbol: ?ShowSysMenu@CMFCRibbonBar@@IEAAXAEBVCPoint@@@Z
extern "C" void MS_ABI impl__ShowSysMenu_CMFCRibbonBar__IEAAXAEBVCPoint___Z(const void* /*class*/* p0) {}

// Symbol: ?TranslateChar@CMFCRibbonBar@@UEAAHI@Z
extern "C" int MS_ABI impl__TranslateChar_CMFCRibbonBar__UEAAHI_Z(unsigned int p0) {
    return 0;
}

// Symbol: ?UpdateToolTipsRect@CMFCRibbonBar@@IEAAXXZ
extern "C" void MS_ABI impl__UpdateToolTipsRect_CMFCRibbonBar__IEAAXXZ() {}

// Symbol: ?WindowProc@CMFCRibbonBar@@MEAA_JI_K_J@Z
extern "C" __int64 MS_ABI impl__WindowProc_CMFCRibbonBar__MEAA_JI_K_J_Z(unsigned int p0, unsigned __int64 p1, __int64 p2) {
    return 0;
}
