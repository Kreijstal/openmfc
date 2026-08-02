// CMFCRibbonCategory / CMFCRibbonGallery exports implemented for the
// wave2_mfc-feature-17 batch (30 symbols).
//
// Each body was decoded from the retail mfc140u export (ordinal -> RVA ->
// objdump). The retail layouts of both classes are much larger than the
// clean-room declarations in include/openmfc/afxmfc.h model:
//
//   CMFCRibbonCategory retail members used here include the panel CArray at
//   +0x7a8/+0x7b0, the hidden-element CArray at +0xb00/+0xb08/+0xb10, image
//   sizes at +0x838/+0x9d0, the scroll-button chain (+0x768) and the size
//   cache members (+0xb0/+0x770). OpenMFC declares only m_strName plus opaque
//   padding, and keeps the panel list in ribbon_state.h instead, so the
//   category functions are implemented against that side table where they
//   can return working results (GetElements / GetElementsByID /
//   GetItemIDsList / FindByID / FindPanelWithElem / GetPanelIndex /
//   first/last-visible-element) and conservatively otherwise.
//
//   CMFCRibbonGallery retail members used in these bodies (m_Items at +0x480,
//   m_arGroups at +0x2a0/+0x2a8, m_Images at +0x2e8, m_nNotifyParentID at
//   +0x4d0, m_bBuilt at +0x4e0 ...) are entirely unmodeled: the clean-room
//   class is a placeholder with only a 32-byte pad, so every gallery mutator
//   gets a type-correct no-op with a TODO(clean-room) note.
//
// Kept in a global_*.cpp file so the build's AUTO_EXCLUDES pass (scan of
// `// Symbol:` comments) drops the generated stubs for these ordinals.

#include "openmfc/afxwin.h"
#include "openmfc/afxmfc.h"
#include "ribbon_state.h"

#include <algorithm>
#include <cstdint>
#include <mutex>
#include <vector>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

using openmfc::ribbon_state::RibbonMutex;
using openmfc::ribbon_state::RibbonCategoryStates;
using openmfc::ribbon_state::RibbonPanelStates;

//=============================================================================
// Static data export
//=============================================================================
// CMFCRibbonGallery::m_mapSelectedItems is a CMap<UINT,UINT,int,int> static.
// The retail object is 56 bytes (same template as CMFCToolBar::m_DefaultImages
// in global_mfc-feature-5_impl.cpp, which documents the harvested size), while
// the clean-room CMap template in afx.h is only 40 bytes, so the export uses
// an opaque 56-byte blob to keep the exported object at retail size.
struct alignas(8) CMap_IIHH_56Bytes {
    unsigned char data[56];
};

// Symbol: ?m_mapSelectedItems@CMFCRibbonGallery@@1V?$CMap@IIHH@@A
extern "C" CMap_IIHH_56Bytes impl__m_mapSelectedItems_CMFCRibbonGallery__1V__CMap_IIHH__A = {};

//=============================================================================
// CMFCRibbonGallery
//=============================================================================
// All eight gallery functions touch member blocks the clean-room class does
// not declare (items at +0x480, groups at +0x2a0, the image list at +0x2e8,
// the notify-parent id at +0x4d0, the "built" flag at +0x4e0 ...).  Nothing
// can be transcribed against declared members, so each body is a type-correct
// no-op that neither reads nor corrupts state.

// CMFCRibbonGallery::RemoveItemToolTips() -- retail walks m_Items (+0x480)
// releasing each item's tool-tip string (and its refcount); OpenMFC has no
// gallery item objects.
// TODO(clean-room): transcribed partially -- gallery items are not modeled.
// Symbol: ?RemoveItemToolTips@CMFCRibbonGallery@@QEAAXXZ
extern "C" void MS_ABI impl__RemoveItemToolTips_CMFCRibbonGallery__QEAAXXZ(
    CMFCRibbonGallery* pThis)
{
    if (!pThis) return;
}

// CMFCRibbonGallery::SetGroupName(int, const wchar_t*) -- retail replaces the
// CString name of the group at m_arGroups[nIndex] (+0x2a0/+0x2a8) and, when a
// parent menu button exists, of the corresponding button in the drop-down
// menu. No group state is modeled.
// TODO(clean-room): transcribed partially -- gallery groups are not modeled.
// Symbol: ?SetGroupName@CMFCRibbonGallery@@QEAAXHPEB_W@Z
extern "C" void MS_ABI impl__SetGroupName_CMFCRibbonGallery__QEAAXHPEB_W_Z(
    CMFCRibbonGallery* pThis, int /*nIndex*/, const wchar_t* /*lpszName*/)
{
    if (!pThis) return;
}

// CMFCRibbonGallery::SetItemToolTip(int, const wchar_t*) -- retail stores the
// tool-tip string into the item at m_Items[nIndex] (+0x480/+0x488/+0x490).
// No item state is modeled.
// TODO(clean-room): transcribed partially -- gallery items are not modeled.
// Symbol: ?SetItemToolTip@CMFCRibbonGallery@@QEAAXHPEB_W@Z
extern "C" void MS_ABI impl__SetItemToolTip_CMFCRibbonGallery__QEAAXHPEB_W_Z(
    CMFCRibbonGallery* pThis, int /*nIndex*/, const wchar_t* /*lpszToolTip*/)
{
    if (!pThis) return;
}

// CMFCRibbonGallery::SetNotifyParentID(int) -- retail stores nID into
// m_nNotifyParentID (+0x4d0) and propagates it to the parent menu button
// (+0xe8). Neither member is modeled.
// TODO(clean-room): transcribed partially -- gallery parent/notify state is
// not modeled.
// Symbol: ?SetNotifyParentID@CMFCRibbonGallery@@IEAAXH@Z
extern "C" void MS_ABI impl__SetNotifyParentID_CMFCRibbonGallery__IEAAXH_Z(
    CMFCRibbonGallery* pThis, int /*nID*/)
{
    if (!pThis) return;
}

// CMFCRibbonGallery::SetPalette(CMFCToolBarImages&) -- retail (when not yet
// built, m_bBuilt +0x4e0 == 0) copies the images into m_Images (+0x2e8),
// records the image count in m_nIconsInRow (+0x4c8) and rebuilds the gallery.
// The image list and rebuild path are not modeled.
// TODO(clean-room): transcribed partially -- gallery image state is not
// modeled.
// Symbol: ?SetPalette@CMFCRibbonGallery@@QEAAXAEAVCMFCToolBarImages@@@Z
extern "C" void MS_ABI impl__SetPalette_CMFCRibbonGallery__QEAAXAEAVCMFCToolBarImages___Z(
    CMFCRibbonGallery* pThis, CMFCToolBarImages& /*images*/)
{
    if (!pThis) return;
}

// CMFCRibbonGallery::SetPalette(unsigned int, int) -- retail loads the bitmap
// resource uiImageResID into m_Images (+0x2e8), sets icon layout
// (m_nIconsInRow +0x4c8, icon size +0x2e8+0x68), and rebuilds. Not modeled.
// TODO(clean-room): transcribed partially -- gallery image state is not
// modeled.
// Symbol: ?SetPalette@CMFCRibbonGallery@@QEAAXIH@Z
extern "C" void MS_ABI impl__SetPalette_CMFCRibbonGallery__QEAAXIH_Z(
    CMFCRibbonGallery* pThis, unsigned int /*uiImageResID*/, int /*bRTL*/)
{
    if (!pThis) return;
}

// CMFCRibbonGallery::SetParentCategory(CMFCRibbonCategory*) -- retail first
// delegates to CMFCRibbonBaseElement::SetParentCategory (not declared in the
// clean-room header) and then propagates the category to every item in the
// gallery. Neither the base setter nor the items are modeled.
// TODO(clean-room): transcribed partially -- the base element setter and the
// gallery items are not modeled.
// Symbol: ?SetParentCategory@CMFCRibbonGallery@@MEAAXPEAVCMFCRibbonCategory@@@Z
extern "C" void MS_ABI impl__SetParentCategory_CMFCRibbonGallery__MEAAXPEAVCMFCRibbonCategory___Z(
    CMFCRibbonGallery* pThis, CMFCRibbonCategory* /*pCategory*/)
{
    if (!pThis) return;
}

//=============================================================================
// CMFCRibbonCategory
//=============================================================================
// The category keeps its panel list in ribbon_state.h (see AddPanel in
// cbarcore.cpp / mfccore.cpp), so the search/collection methods below are
// implemented against that side table.  The retail hidden-element CArray
// (+0xb00) is not modeled, so hidden elements never participate.

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
