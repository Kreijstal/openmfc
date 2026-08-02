// CMFCRibbonBar exports implemented for the wave1_mfc-feature-6 batch.
// Each body was decoded from the retail mfc140u export (ordinal -> RVA ->
// objdump) rather than inferred, and member offsets referenced below are the
// ones declared in include/openmfc/afxmfc.h (pinned by static_asserts in
// phase4/src/mfccore.cpp):
//
//   m_bRecalcCategoryHeight        1060 (0x424)
//   m_bRecalcCategoryWidth         1064 (0x428)
//   m_bTracked                     1068 (0x42c)
//   m_bForceRedraw                 1080 (0x438)
//   m_bAutoCommandTimer            1088 (0x440)
//   m_dwHideFlags                  1144 (0x478)
//   m_pMainButton                  1152 (0x480)
//   m_pHighlighted                 1160 (0x488)
//   m_pPressed                     1168 (0x490)
//   m_arContextCaptions            2856 (0xb28)  opaque CArray blob
//   m_arCategories                 2896 (0xb50)  opaque CArray blob
//   m_arKeyElements                2936 (0xb80)  opaque CArray blob
//   m_CaptionButtons               3096 (0xc18)  opaque group blob
//   m_QAToolbar                    4992 (0x1380) opaque group blob
//
// A lot of the retail bodies walk ribbon objects OpenMFC does not model:
// CMFCRibbonButtonsGroup / CMFCRibbonQuickAccessToolBar / the category
// internal panel groups (retail reads element rects at +0xc8, category
// context IDs at +0xac and visibility at +0xa8, key tips, accessibility
// objects). Those functions get type-correct conservative bodies with a
// TODO(clean-room) note, exactly like the earlier ribbon batches. The ones
// that only need the ribbon_state.h side tables (categories/panels/elements)
// are implemented against that state so they return working results instead
// of reading uninitialized retail-shaped blobs.
//
// Kept in a global_*.cpp file so the build's AUTO_EXCLUDES pass (scan of
// `// Symbol:` comments) drops the generated stubs for these ordinals.

#include "openmfc/afxwin.h"
#include "openmfc/afxmfc.h"
#include "ribbon_state.h"

#include <mutex>
#include <vector>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

using openmfc::ribbon_state::RibbonMutex;
using openmfc::ribbon_state::RibbonBarStates;
using openmfc::ribbon_state::RibbonCategoryStates;
using openmfc::ribbon_state::RibbonPanelStates;

// CKeyboardManager::TranslateCharToUpper(unsigned int) -- defined in
// mfccore.cpp; used by CMFCRibbonBar::OnBeforeProcessKey.
extern "C" unsigned int MS_ABI impl__TranslateCharToUpper_CKeyboardManager__SAII_Z(unsigned int ch);

//=============================================================================
// Category lookup
//=============================================================================

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

//=============================================================================
// Element collection
//=============================================================================

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

//=============================================================================
// Context captions
//=============================================================================

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

//=============================================================================
// Layout
//=============================================================================

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

//=============================================================================
// Hit testing / keyboard navigation
//=============================================================================

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

//=============================================================================
// Context categories
//=============================================================================

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

//=============================================================================
// Resource / state loading
//=============================================================================

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

//=============================================================================
// Misc window behaviour
//=============================================================================

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
