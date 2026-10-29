// CMFCColorMenuButton — OpenMFC export implementations and explicit stubs.
// This class is absent from include/openmfc; methods that take `this` therefore
// use opaque pointers unless a shared OpenMFC type is needed by the signature.
// Several behaviors remain stubs where the retail base, popup, drawing, or
// derived vtable/member lifetime has no corresponding model here. Verified
// scalar fields and the two retail-layout color maps are handled locally.

#include "detail/ManualSmallStubImplementationsSupport.h"

#include "openmfc/afxwin.h"
#include "openmfc/afxmfc.h"

#include <windows.h>

#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <new>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

extern "C" void MS_ABI
impl__SetString___CSimpleStringT__W_00_ATL__QEAAXPEB_W_Z(
    void* pThis, const wchar_t* psz);

//-----------------------------------------------------------------------------
// CMFCColorMenuButton::m_ColorsByID
//-----------------------------------------------------------------------------

//=============================================================================
// CMFCColorMenuButton has no public OpenMFC class declaration. Retail's
// CreateObject entry at RVA 0x2a4e0 (mfc140u) requests a 0x1A0-byte allocation.
// This is only a local bounds sentinel for the verified scalar offsets below;
// it is not a modeled C++ class layout or a usable derived vtable.
//=============================================================================
namespace {
struct CMFCColorMenuButtonOpaqueLayout {
    unsigned char bytes[0x1A0];
};
static_assert(sizeof(CMFCColorMenuButtonOpaqueLayout) == 0x1A0,
              "CMFCColorMenuButton allocation bounds");

struct RetailMapLayout {
    void* vftable;
    void* buckets;
    std::uint32_t bucketCount;
    std::uint32_t pad14;
    std::uint64_t count;
    void* freeList;
    void* blocks;
    std::uint32_t blockSize;
    std::uint32_t pad34;
};
static_assert(sizeof(RetailMapLayout) == 56, "retail CMap header size");
static_assert(offsetof(RetailMapLayout, buckets) == 0x08, "retail CMap buckets");
static_assert(offsetof(RetailMapLayout, bucketCount) == 0x10, "retail CMap bucket count");
static_assert(offsetof(RetailMapLayout, count) == 0x18, "retail CMap item count");

struct RetailColorAssoc {
    std::uint32_t key;
    std::uint32_t value;
    RetailColorAssoc* next;
    std::uint32_t hash;
    std::uint32_t pad14;
};
static_assert(sizeof(RetailColorAssoc) == 24, "retail CMap<UINT,UINT> assoc size");
static_assert(offsetof(RetailColorAssoc, next) == 0x08, "retail color assoc next");
static_assert(offsetof(RetailColorAssoc, hash) == 0x10, "retail color assoc hash");

struct RetailStringAssoc {
    std::uint32_t key;
    std::uint32_t pad04;
    alignas(CString) unsigned char valueStorage[sizeof(CString)];
    RetailStringAssoc* next;
    std::uint32_t hash;
    std::uint32_t pad1c;
};
static_assert(sizeof(RetailStringAssoc) == 32, "retail CMap<UINT,CString> assoc size");
static_assert(offsetof(RetailStringAssoc, valueStorage) == 0x08, "retail string assoc value");
static_assert(offsetof(RetailStringAssoc, next) == 0x10, "retail string assoc next");
static_assert(offsetof(RetailStringAssoc, hash) == 0x18, "retail string assoc hash");

// The CMap GetAssocAt path uses ldiv(key, 127773), then remainder*16807 -
// quotient*2836, adding INT_MAX if negative. This is the retail hash path
// observed at entry RVA 0x27ef0 (mfc140); OpenMFC's generic CMap HashKey uses
// key >> 4 and therefore cannot be used for these retail-layout globals.
std::uint32_t RetailColorMapHash(std::uint32_t key) {
    const std::int32_t signedKey = static_cast<std::int32_t>(key);
    const std::int32_t quotient = signedKey / 127773;
    const std::int32_t remainder = signedKey % 127773;
    std::int32_t hash = remainder * 16807 - quotient * 2836;
    if (hash < 0) hash += INT32_MAX;
    return static_cast<std::uint32_t>(hash);
}

bool EnsureRetailBuckets(RetailMapLayout* map) {
    if (!map || map->bucketCount == 0) return false;
    if (!map->buckets)
        map->buckets = std::calloc(map->bucketCount, sizeof(void*));
    return map->buckets != nullptr;
}

RetailColorAssoc* FindRetailColor(RetailMapLayout* map, std::uint32_t key,
                                  std::uint32_t hash) {
    if (!map || map->bucketCount == 0 || !map->buckets) return nullptr;
    auto** buckets = static_cast<RetailColorAssoc**>(map->buckets);
    for (RetailColorAssoc* assoc = buckets[hash % map->bucketCount]; assoc;
         assoc = assoc->next) {
        if (assoc->hash == hash && assoc->key == key) return assoc;
    }
    return nullptr;
}

// These writers keep the retail bucket/association layout but allocate each
// assoc directly instead of using retail's CBlock/free-list pool. On OOM they
// leave the map unchanged rather than raising the retail memory exception.
void SetRetailColor(RetailMapLayout* map, std::uint32_t key, std::uint32_t value) {
    if (!EnsureRetailBuckets(map)) return;
    const std::uint32_t hash = RetailColorMapHash(key);
    if (RetailColorAssoc* assoc = FindRetailColor(map, key, hash)) {
        assoc->value = value;
        return;
    }
    auto* assoc = static_cast<RetailColorAssoc*>(std::calloc(1, sizeof(RetailColorAssoc)));
    if (!assoc) return;
    auto** buckets = static_cast<RetailColorAssoc**>(map->buckets);
    const std::uint32_t bucket = hash % map->bucketCount;
    assoc->key = key;
    assoc->value = value;
    assoc->hash = hash;
    assoc->next = buckets[bucket];
    buckets[bucket] = assoc;
    ++map->count;
}

bool LookupRetailColor(RetailMapLayout* map, std::uint32_t key, std::uint32_t& value) {
    if (!map || map->bucketCount == 0 || !map->buckets) return false;
    const std::uint32_t hash = RetailColorMapHash(key);
    RetailColorAssoc* assoc = FindRetailColor(map, key, hash);
    if (!assoc) return false;
    value = assoc->value;
    return true;
}

// CString values are copied through OpenMFC's string thunk so their buffers
// retain a valid OpenMFC IAtlStringMgr; retail may share its CStringData instead.
void SetRetailString(RetailMapLayout* map, std::uint32_t key, const CString& value) {
    if (!EnsureRetailBuckets(map)) return;
    const std::uint32_t hash = RetailColorMapHash(key);
    auto** buckets = static_cast<RetailStringAssoc**>(map->buckets);
    const std::uint32_t bucket = hash % map->bucketCount;
    for (RetailStringAssoc* assoc = buckets[bucket]; assoc; assoc = assoc->next) {
        if (assoc->hash == hash && assoc->key == key) {
            impl__SetString___CSimpleStringT__W_00_ATL__QEAAXPEB_W_Z(
                assoc->valueStorage, value.GetString());
            return;
        }
    }
    auto* assoc = static_cast<RetailStringAssoc*>(std::calloc(1, sizeof(RetailStringAssoc)));
    if (!assoc) return;
    assoc->key = key;
    assoc->hash = hash;
    assoc->next = buckets[bucket];
    new (assoc->valueStorage) CString();
    impl__SetString___CSimpleStringT__W_00_ATL__QEAAXPEB_W_Z(
        assoc->valueStorage, value.GetString());
    buckets[bucket] = assoc;
    ++map->count;
}

void StoreButtonDword(void* pThis, std::size_t offset, std::uint32_t value) {
    if (pThis && offset <= sizeof(CMFCColorMenuButtonOpaqueLayout) - sizeof(value))
        std::memcpy(static_cast<unsigned char*>(pThis) + offset, &value, sizeof(value));
}
}

extern "C" unsigned char
    impl__m_ColorNames_CMFCColorBar__1V__CMap_KKV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__PEB_W__A[56];
extern "C" int MS_ABI
impl__OnDrawOnCustomizeList_CMFCToolBarMenuButton__UEAAHPEAVCDC__AEBVCRect__H_Z(
    void* pThis, CDC* pDC, const CRect* pRect, int bSelected);
//-----------------------------------------------------------------------------
// CMFCColorMenuButton::CopyFrom
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// CMFCColorMenuButton::CreateObject
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// CMFCColorMenuButton::CreatePopupMenu
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// CMFCColorMenuButton::EnableAutomaticButton
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// CMFCColorMenuButton::EnableDocumentColors
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// CMFCColorMenuButton::EnableOtherButton
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// CMFCColorMenuButton::EnableTearOff
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// CMFCColorMenuButton::GetColorByCmdID
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// CMFCColorMenuButton::Initialize
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// CMFCColorMenuButton::OnChangeParentWnd
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// CMFCColorMenuButton::OnDraw
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// CMFCColorMenuButton::OnDrawOnCustomizeList
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// CMFCColorMenuButton::OpenColorDialog
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// CMFCColorMenuButton::Serialize
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// CMFCColorMenuButton::SetColor
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// CMFCColorMenuButton::SetColorName
//-----------------------------------------------------------------------------
// Retail's CMap<UINT,UINT,COLORREF,COLORREF> object is 56 bytes. Its
// non-virtual fields use the retail order in RetailMapLayout; the vfptr is
// deliberately NULL because OpenMFC has no vtable for this specialization.
// Symbol: ?m_ColorsByID@CMFCColorMenuButton@@1V?$CMap@IIKK@@A
extern "C" {
__attribute__((used)) alignas(8) unsigned char
    impl__m_ColorsByID_CMFCColorMenuButton__1V__CMap_IIKK__A[56] = {
        // vfptr and m_pHashTable are NULL; m_nHashTableSize defaults to 17.
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        17, 0, 0, 0, 0, 0, 0, 0,
        // m_nCount, m_pFreeList, and m_pBlocks are NULL/zero.
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        // Default CMap allocation block size.
        10, 0, 0, 0, 0, 0, 0, 0
    };
}
// Retail entry RVA 0x2b8a0 (mfc140u) calls CMFCToolBarMenuButton::CopyFrom
// at entry RVA 0x172ca0 (mfc140u), updates the color/id map through helper entry RVA
// 0x2346c (mfc140u), copies the +0x130 CStringArray through helper entry RVA
// 0x15034 (mfc140u), and copies the derived scalar/string fields. The base
// operation and CStringArray storage are not safely modeled here.
// Symbol: ?CopyFrom@CMFCColorMenuButton@@MEAAXAEBVCMFCToolBarButton@@@Z
extern "C" void MS_ABI impl__CopyFrom_CMFCColorMenuButton__MEAAXAEBVCMFCToolBarButton___Z(
    void* pThis, const CMFCToolBarButton& /*src*/)
{
    if (!pThis) return;
    // TODO(clean-room): base CopyFrom and the +0x130 CStringArray/string copies
    // are not modeled; the related map helper is retained as a retail reference.
}
// Retail CreateObject entry RVA 0x2a4e0 (mfc140u) allocates 0x1A0 bytes and
// tail-jumps to the default constructor entry RVA 0x2a560 (mfc140u) on success.
// That constructor installs the derived vftable, constructs the base and
// CString/array members, then initializes the scalar fields. This file has no
// compatible derived vftable or retail-layout array construction, so returning
// nullptr is an intentional deviation rather than the retail allocation-failure
// terminal alone.
// Symbol: ?CreateObject@CMFCColorMenuButton@@SAPEAVCObject@@XZ
extern "C" CObject* MS_ABI impl__CreateObject_CMFCColorMenuButton__SAPEAVCObject__XZ(void)
{
    // TODO(clean-room): no compatible derived vftable or member construction is
    // available, so do not expose an allocated object with invalid virtual state.
    return nullptr;
}
// Retail CreatePopupMenu entry RVA 0x2bad0 (mfc140u) conditionally performs a
// parent/map callback path, then allocates a 0x2ED8-byte popup and calls its
// constructor entry RVA 0x24730 (mfc140u). It returns the popup on
// successful allocation and nullptr only on allocation failure. OpenMFC has no
// compatible CMFCColorPopup construction here, so this implementation is a
// documented deviation, not a transcription of retail's return behavior.
// Symbol: ?CreatePopupMenu@CMFCColorMenuButton@@MEAAPEAVCMFCPopupMenu@@XZ
extern "C" void* MS_ABI impl__CreatePopupMenu_CMFCColorMenuButton__MEAAPEAVCMFCPopupMenu__XZ(
    void* pThis)
{
    if (!pThis) return nullptr;
    // TODO(clean-room): the popup class and its constructor are not modeled.
    // Returning nullptr is a deliberate safe deviation from the success path.
    return nullptr;
}
// Retail entry RVA 0x2a980 (mfc140u) stores bIsDefault at +0x174; only when
// bIsDefault is nonzero does it SetString at +0x180 and then store bShow at
// +0x12c. A NULL label takes the retail invalid-argument path before that last
// store. The scalar stores are reproduced; CString copying/exception behavior is not.
// Symbol: ?EnableAutomaticButton@CMFCColorMenuButton@@QEAAXPEB_WKH@Z
extern "C" void MS_ABI impl__EnableAutomaticButton_CMFCColorMenuButton__QEAAXPEB_WKH_Z(
    void* pThis, const wchar_t* lpszLabel, int bShow, int bIsDefault)
{
    if (!pThis) return;
    StoreButtonDword(pThis, 0x174, static_cast<std::uint32_t>(bIsDefault));
    if (bIsDefault && lpszLabel != nullptr)
        StoreButtonDword(pThis, 0x12c, static_cast<std::uint32_t>(bShow));
    // Retail also assigns the label to the CString at +0x180 when enabled.
}
// Retail entry RVA 0x2aa50 (mfc140u) always stores bShow at +0x17c and, when
// bShow is nonzero, assigns the label to the CString at +0x190 (NULL takes the
// invalid-argument path). Only the scalar store is reproduced here.
// Symbol: ?EnableDocumentColors@CMFCColorMenuButton@@QEAAXPEB_WH@Z
extern "C" void MS_ABI impl__EnableDocumentColors_CMFCColorMenuButton__QEAAXPEB_WH_Z(
    void* pThis, const wchar_t* /*lpszLabel*/, int bShow)
{
    if (!pThis) return;
    StoreButtonDword(pThis, 0x17c, static_cast<std::uint32_t>(bShow));
    // Retail also assigns the label to the CString at +0x190 when enabled.
}
// Retail entry RVA 0x2a9e0 (mfc140u) stores bIsDefault at +0x178; only when it
// is nonzero does it assign the label at +0x188 then store !bShow at +0x198.
// A NULL label takes the invalid-argument path. Scalar stores are reproduced;
// CString copying/exception behavior is not.
// Symbol: ?EnableOtherButton@CMFCColorMenuButton@@QEAAXPEB_WHH@Z
extern "C" void MS_ABI impl__EnableOtherButton_CMFCColorMenuButton__QEAAXPEB_WHH_Z(
    void* pThis, const wchar_t* lpszLabel, int bShow, int bIsDefault)
{
    if (!pThis) return;
    StoreButtonDword(pThis, 0x178, static_cast<std::uint32_t>(bIsDefault));
    if (bIsDefault && lpszLabel != nullptr)
        StoreButtonDword(pThis, 0x198, bShow ? 0u : 1u);
    // Retail also assigns the label to the CString at +0x188 when enabled.
}
// Retail EnableTearOff entry RVA 0x2aaa0 (mfc140u) stores bHorz/bVert at
// +0x16c/+0x170. For +0xf4 it preserves nID if the global at VA 0x1803be260
// (mfc140u) is NULL or nID is outside the inclusive bounds at global offsets
// +8/+0xc; if the global exists and nID is within those bounds, it stores zero.
// The two scalar flag stores are reproduced; the manager-dependent ID is not.
// Symbol: ?EnableTearOff@CMFCColorMenuButton@@QEAAXIHH@Z
extern "C" void MS_ABI impl__EnableTearOff_CMFCColorMenuButton__QEAAXIHH_Z(
    void* pThis, unsigned int /*nID*/, int bHorz, int bVert)
{
    if (!pThis) return;
    StoreButtonDword(pThis, 0x16c, static_cast<std::uint32_t>(bHorz));
    StoreButtonDword(pThis, 0x170, static_cast<std::uint32_t>(bVert));
    // TODO(clean-room): +0xf4 depends on the unmodeled range at mfc140u VA
    // 0x1803be260; do not guess the manager's state.
}
// Retail GetColorByCmdID entry RVA 0x2bcd0 (mfc140u) looks up nID through the
// CMap GetAssocAt helper entry RVA 0x236e0 (mfc140u) and returns assoc->value
// or -1 on a miss. The local map has a retail-sized header and the compatible
// integral-key hash/association layout used by that helper.
// Symbol: ?GetColorByCmdID@CMFCColorMenuButton@@SAKI@Z
extern "C" unsigned long MS_ABI impl__GetColorByCmdID_CMFCColorMenuButton__SAKI_Z(
    unsigned int nID)
{
    std::uint32_t color;
    auto* map = reinterpret_cast<RetailMapLayout*>(
        impl__m_ColorsByID_CMFCColorMenuButton__1V__CMap_IIKK__A);
    return LookupRetailColor(map, nID, color) ? color : 0xFFFFFFFFu;
}
// Retail Initialize entry RVA 0x2a880 (mfc140u) stores -1 at +0x128,
// +0x168/+0x16c/+0x170; zero at +0x12c/+0x174/+0x17c/+0x198.
// Symbol: ?Initialize@CMFCColorMenuButton@@IEAAXXZ
extern "C" void MS_ABI impl__Initialize_CMFCColorMenuButton__IEAAXXZ(
    void* pThis)
{
    if (!pThis) return;
    StoreButtonDword(pThis, 0x128, 0xFFFFFFFFu);
    StoreButtonDword(pThis, 0x12c, 0);
    StoreButtonDword(pThis, 0x168, 0xFFFFFFFFu);
    StoreButtonDword(pThis, 0x16c, 0xFFFFFFFFu);
    StoreButtonDword(pThis, 0x170, 0xFFFFFFFFu);
    const std::uint64_t zero = 0;
    std::memcpy(static_cast<unsigned char*>(pThis) + 0x174, &zero, sizeof(zero));
    StoreButtonDword(pThis, 0x17c, 0);
    StoreButtonDword(pThis, 0x198, 0);
}
// Retail OnChangeParentWnd entry RVA 0x2b100 (mfc140u) calls the base entry
// RVA 0x15e570 (mfc140u), checks two parent window classes, updates +0xc/+0xcc,
// stores the parent at +0x80, and sets +0xc8. Those base/window operations are
// not modeled here.
// Symbol: ?OnChangeParentWnd@CMFCColorMenuButton@@UEAAXPEAVCWnd@@@Z
extern "C" void MS_ABI impl__OnChangeParentWnd_CMFCColorMenuButton__UEAAXPEAVCWnd___Z(
    void* pThis, CWnd* /*pWndParent*/)
{
    if (!pThis) return;
    // TODO(clean-room): the base call, parent class-name checks, and updates to
    // +0xc/+0xcc/+0x80/+0xc8 are not modeled.
}
// Retail OnDraw entry RVA 0x2aaf0 (mfc140u) calls the base OnDraw entry RVA
// 0x173010 (mfc140u), then runs visual-manager, image, palette, and color-chip
// drawing paths. Several early exits and stateful helper calls are present;
// this opaque implementation does not reproduce them.
// Symbol: ?OnDraw@CMFCColorMenuButton@@MEAAXPEAVCDC@@AEBVCRect@@PEAVCMFCToolBarImages@@HHHHH@Z
extern "C" void MS_ABI impl__OnDraw_CMFCColorMenuButton__MEAAXPEAVCDC__AEBVCRect__PEAVCMFCToolBarImages__HHHHH_Z(
    void* pThis, CDC* /*pDC*/, const CRect& /*rect*/, void* /*pImages*/,
    int /*bHorz*/, int /*bCustomizeMode*/, int /*bHighlight*/, int /*bDrawBorder*/,
    int /*bGrayDisabledButtons*/)
{
    if (!pThis) return;
    // TODO(clean-room): not transcribed -- needs the visual-manager drawing
    // and the +0x180/+0x188/+0x190 label strings; not modeled.
}
// Retail OnDrawOnCustomizeList entry RVA 0x2af30 (mfc140u) saves the command
// ID at +0x24, clears it, adjusts a local copy of the rectangle through an
// indirect helper, calls CMFCToolBarMenuButton::OnDrawOnCustomizeList at entry
// RVA 0x175d00 (mfc140u), restores the ID, and returns that result. The local
// rectangle adjustment is omitted here.
// Symbol: ?OnDrawOnCustomizeList@CMFCColorMenuButton@@MEAAHPEAVCDC@@AEBVCRect@@H@Z
extern "C" int MS_ABI impl__OnDrawOnCustomizeList_CMFCColorMenuButton__MEAAHPEAVCDC__AEBVCRect__H_Z(
    void* pThis, CDC* pDC, const CRect& rect, int bSelected)
{
    if (!pThis) return FALSE;
    std::uint32_t oldID;
    std::memcpy(&oldID, static_cast<unsigned char*>(pThis) + 0x24, sizeof(oldID));
    const std::uint32_t zero = 0;
    std::memcpy(static_cast<unsigned char*>(pThis) + 0x24, &zero, sizeof(zero));
    const int result = impl__OnDrawOnCustomizeList_CMFCToolBarMenuButton__UEAAHPEAVCDC__AEBVCRect__H_Z(
        pThis, pDC, &rect, bSelected);
    std::memcpy(static_cast<unsigned char*>(pThis) + 0x24, &oldID, sizeof(oldID));
    return result;
}
// Retail OpenColorDialog entry RVA 0x2b9f0 (mfc140u) has two branches: with
// +0x198 nonzero it constructs/runs CColorDialog (constructor entry RVA
// 0x207670 (mfc140u), DoModal entry RVA 0x207760 (mfc140u)); otherwise it
// constructs/runs the alternate color dialog (entries RVA 0x29350, 0x2088b0,
// and 0x294a0 (mfc140u)). Either accepted dialog writes the selected color to
// *pColor and returns TRUE; cancellation returns FALSE. Neither branch is
// modeled here.
// Symbol: ?OpenColorDialog@CMFCColorMenuButton@@UEAAHKAEAK@Z
extern "C" int MS_ABI impl__OpenColorDialog_CMFCColorMenuButton__UEAAHKAEAK_Z(
    void* pThis, unsigned long /*clrDefault*/, unsigned long& /*clrPick*/)
{
    if (!pThis) return FALSE;
    // TODO(clean-room): both dialog branches, their constructors/destructors,
    // and the selected-color output are not modeled.
    return FALSE;
}
// Retail Serialize entry RVA 0x2b190 (mfc140u) calls the base Serialize entry
// RVA 0x172dd0 (mfc140u), then archives color state and the +0x130 string array.
// The base archive implementation and array representation are not modeled.
// Symbol: ?Serialize@CMFCColorMenuButton@@MEAAXAEAVCArchive@@@Z
extern "C" void MS_ABI impl__Serialize_CMFCColorMenuButton__MEAAXAEAVCArchive___Z(
    void* pThis, CArchive& /*ar*/)
{
    if (!pThis) return;
    // TODO(clean-room): not transcribed -- needs the CMFCToolBarButton base
    // Serialize, the color field and the +0x130 CStringArray; not modeled.
}
// Retail SetColor entry RVA 0x2afb0 (mfc140u) stores clr at +0x128, updates
// m_ColorsByID through helper entry RVA 0x2346c (mfc140u) using the command ID
// at +0x24, updates the parent tooltip when present, and conditionally redraws
// the owner. This body reproduces the scalar store and map update only.
// Symbol: ?SetColor@CMFCColorMenuButton@@UEAAXKH@Z
extern "C" void MS_ABI impl__SetColor_CMFCColorMenuButton__UEAAXKH_Z(
    void* pThis, unsigned long clr, int /*bRedraw*/)
{
    if (!pThis) return;
    const std::uint32_t color = static_cast<std::uint32_t>(clr);
    StoreButtonDword(pThis, 0x128, color);
    std::uint32_t commandID;
    std::memcpy(&commandID, static_cast<unsigned char*>(pThis) + 0x24,
                sizeof(commandID));
    auto* map = reinterpret_cast<RetailMapLayout*>(
        impl__m_ColorsByID_CMFCColorMenuButton__1V__CMap_IIKK__A);
    SetRetailColor(map, commandID, color);
    // Retail additionally updates the parent tooltip and optionally redraws.
}
// The Unicode export has no resolved RVA. Its ANSI twin's function entry RVA
// 0x2bd70 (mfc140) loads the CString data and tail-jumps to CMap SetAt helper
// entry RVA 0x27a50 (mfc140). The shared CMFCColorBar::m_ColorNames map is at
// RVA 0x3b1c00 (mfc140u); its 56-byte header is modeled in CMFCColorBar.cpp.
// Retail GetAssocAt entry RVA 0x27ef0 (mfc140) uses the quotient/remainder hash
// reproduced by RetailColorMapHash, rather than OpenMFC's generic key >> 4.
// Symbol: ?SetColorName@CMFCColorMenuButton@@SAXKAEBV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@@Z
extern "C" void MS_ABI impl__SetColorName_CMFCColorMenuButton__SAXKAEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
    unsigned long clr, const CString& name)
{
    auto* map = reinterpret_cast<RetailMapLayout*>(
        impl__m_ColorNames_CMFCColorBar__1V__CMap_KKV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__PEB_W__A);
    SetRetailString(map, static_cast<std::uint32_t>(clr), name);
}
// Retail destructor entry RVA 0x2a8c0 (mfc140u) releases the +0x180/+0x188/
// +0x190 CString references, destroys the +0x158 and +0x130 array members, and
// calls the CMFCToolBarMenuButton base destructor entry RVA 0x172bf0 (mfc140u).
// This no-op remains a stub because those member lifetimes are not represented.
// Symbol: ??1CMFCColorMenuButton@@UEAA@XZ
extern "C" void MS_ABI impl___1CMFCColorMenuButton__UEAA_XZ(void* pThis) {
    (void)pThis;
}
// Retail constructor entry RVA 0x2a6e0 (mfc140u) calls the
// CMFCToolBarMenuButton parameterized constructor entry RVA 0x172870 (mfc140u), initializes the
// derived vftable/array/CString state, calls the palette helper at RVA 0x265f0
// (mfc140u), and seeds the color from GetColorByCmdID. The opaque constructor
// is not transcribed; returning pThis is a documented stub only.
// Symbol: ??0CMFCColorMenuButton@@QEAA@IPEB_WPEAVCPalette@@@Z
extern "C" void* MS_ABI impl___0CMFCColorMenuButton__QEAA_IPEB_WPEAVCPalette___Z(
    void* pThis, int id, const wchar_t* text, void* pPalette) {
    (void)id;
    (void)text;
    (void)pPalette;
    return pThis;
}
// Retail default-constructor entry RVA 0x2a560 (mfc140u) calls the base
// constructor entry RVA 0x172790 (mfc140u), installs a derived vftable, sets
// up the +0x130 array and three CString slots, then initializes scalar state.
// This opaque constructor is not transcribed; returning pThis is only a stub.
// Symbol: ??0CMFCColorMenuButton@@QEAA@XZ
extern "C" void* MS_ABI impl___0CMFCColorMenuButton__QEAA_XZ(void* pThis) {
    return pThis;
}
