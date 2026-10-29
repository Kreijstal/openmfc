// CMFCCmdUsageCount — OpenMFC implementation.
// Sources: global_cmfccmdusagecount.cpp, manual_small_stub_implementations.cpp

#include "detail/CMFCCmdUsageCountSupport.h"
#include "detail/ManualSmallStubImplementationsSupport.h"

#include <cstdlib>
#include <cstring>
#include <cstdint>

extern "C" void MS_ABI impl__FreeDataChain_CPlex__QEAAXXZ(void* pThis);

namespace {
// CMap fields follow the constructor at RVA 0x23190, CAssoc/hash behavior at
// RVA 0x2346c/0x236e0, and RemoveAll at RVA 0x1ba40 (all mfc140u).
struct CMFCCmdUsageCountAssoc {
    unsigned int key;          // +0x00
    unsigned int value;        // +0x04
    CMFCCmdUsageCountAssoc* pNext; // +0x08
    unsigned int nHashValue;   // +0x10
    unsigned int _pad0014;
};
static_assert(sizeof(CMFCCmdUsageCountAssoc) == 0x18, "CMFCCmdUsageCount CAssoc size");
static_assert(offsetof(CMFCCmdUsageCountAssoc, value) == 0x04, "CAssoc value @+0x04");
static_assert(offsetof(CMFCCmdUsageCountAssoc, pNext) == 0x08, "CAssoc next @+0x08");
static_assert(offsetof(CMFCCmdUsageCountAssoc, nHashValue) == 0x10, "CAssoc hash @+0x10");

struct CMFCCmdUsageCountMapLayout {
    void*           m_pVtable;          // +0x00
    void**          m_pHashTable;       // +0x08
    unsigned int    m_nHashTableSize;   // +0x10
    unsigned int    _pad0014;
    std::intptr_t   m_nCount;           // +0x18
    void*           m_pFreeList;        // +0x20
    void*           m_pBlocks;          // +0x28
    std::intptr_t   m_nBlockSize;       // +0x30
};
static_assert(sizeof(CMFCCmdUsageCountMapLayout) == 0x38, "CMFCCmdUsageCount CMap size");
static_assert(offsetof(CMFCCmdUsageCountMapLayout, m_pHashTable) == 0x08, "CMap buckets @+0x08");
static_assert(offsetof(CMFCCmdUsageCountMapLayout, m_nHashTableSize) == 0x10, "CMap size @+0x10");
static_assert(offsetof(CMFCCmdUsageCountMapLayout, m_nCount) == 0x18, "CMap count @+0x18");
static_assert(offsetof(CMFCCmdUsageCountMapLayout, m_pFreeList) == 0x20, "CMap free list @+0x20");
static_assert(offsetof(CMFCCmdUsageCountMapLayout, m_pBlocks) == 0x28, "CMap blocks @+0x28");
static_assert(offsetof(CMFCCmdUsageCountMapLayout, m_nBlockSize) == 0x30, "CMap block size @+0x30");

std::uint32_t HashCommandKey(std::uint32_t key)
{
    const std::int32_t signedKey = static_cast<std::int32_t>(key);
    const std::int32_t quotient = signedKey / 127773;
    const std::int32_t remainder = signedKey % 127773;
    std::int32_t hash = static_cast<std::int32_t>(
        16807u * static_cast<std::uint32_t>(remainder) -
         2836u * static_cast<std::uint32_t>(quotient));
    if (hash < 0) hash += 0x7fffffff;
    return static_cast<std::uint32_t>(hash);
}

unsigned int LookupCommandCount(const CMFCCmdUsageCountMapLayout* map,
                                std::uint32_t key)
{
    if (map->m_nHashTableSize == 0 || map->m_pHashTable == nullptr) return 0;
    const std::uint32_t hash = HashCommandKey(key);
    const std::uint32_t bucket = hash % map->m_nHashTableSize;
    auto* assoc = static_cast<CMFCCmdUsageCountAssoc*>(map->m_pHashTable[bucket]);
    while (assoc != nullptr) {
        if (assoc->nHashValue == hash && assoc->key == key) return assoc->value;
        assoc = assoc->pNext;
    }
    return 0;
}

void RemoveAllCmdUsageMap(void* pMap)
{
    auto* map = static_cast<CMFCCmdUsageCountMapLayout*>(pMap);
    if (map->m_pHashTable != nullptr) {
        std::free(map->m_pHashTable);
        map->m_pHashTable = nullptr;
    }
    map->m_nCount = 0;
    map->m_pFreeList = nullptr;
    if (map->m_pBlocks != nullptr)
        impl__FreeDataChain_CPlex__QEAAXXZ(map->m_pBlocks);
    map->m_pBlocks = nullptr;
}
} // namespace

// CMFCCmdUsageCount is opaque in include/openmfc. Retail's destructor accesses
// the object at +0x00 and its CMap member at +0x08 (mfc140u RVA 0x23230);
// other exports access the total at +0x40. Pin the available shadow layout.
static_assert(offsetof(CU, m_map) == 0x08, "CMFCCmdUsageCount CMap @+0x08");
static_assert(offsetof(CU, m_nTotalCount) == 0x40, "CMFCCmdUsageCount total @+0x40");
static_assert(sizeof(CU) == 0x48, "CMFCCmdUsageCount shadow size 0x48");

// AddCmd(UINT cmd), retail entry RVA 0x232e0 (mfc140u): it first tests the
// module flag at RVA 0x3be35c, applies the command-range tests, compares the
// command with the global hook at RVA 0x3be3b0, then looks up/inserts a CMap
// entry through helper entries RVA 0x236e0 / 0x2346c and increments +0x40.
// This pre-existing approximation only applies range checks and increments
// +0x40: it omits the module-flag and hook checks and does not update the map.
// Its unsigned `nCmd >= 0xff00` check also differs from retail's signed JGE,
// which allows high-bit command IDs in the third range. It additionally has
// a defensive null-this return absent from retail; retained with these warnings.
// Symbol: ?AddCmd@CMFCCmdUsageCount@@QEAAXI@Z
extern "C" void MS_ABI impl__AddCmd_CMFCCmdUsageCount__QEAAXI_Z(
    void* pThis, unsigned int cmd)
{
    if (pThis == nullptr) return;
    const unsigned int nCmd = cmd;
    // The range expressions mirror retail's unsigned subtract/compare gates:
    // [0xE120,0xF0FF], [1,0xE10F], or [0xF1F0,0xFFFFFFFE]. Retail's following
    // `cmp $0xff00; jge` is signed, so its third range still admits
    // [0x80000000,0xFFFFFFFE]; this approximation's unsigned check below does not.
    const bool bRange1 = (nCmd >= 0xE120u) && (nCmd - 0xE120u) <= 0xEDFu;
    const bool bRange2 = (nCmd >= 1u) && (nCmd - 1u) <= 0xE10Eu;
    const bool bRange3 = (nCmd >= 0xF1F0u) && (nCmd - 0xF1F0u) <= 0xFFFF0E0Eu;
    if (!(bRange1 || bRange2 || bRange3)) return;
    if (nCmd >= 0xFF00u) return;   // approximation; retail uses signed `jge` here
    if (nCmd >= 0xE210u && nCmd - 0xE210u <= 0xFu) return;  // [0xE210,0xE21F]
    // TODO(clean-room): retail also checks the module flag at RVA 0x3be35c
    // and the hook at RVA 0x3be3b0, then updates the CMap through helper
    // entries RVA 0x236e0 / 0x2346c (all mfc140u); those behaviors are omitted.
    CU* s = reinterpret_cast<CU*>(pThis);
    ++s->m_nTotalCount;   // 0x40
}
// GetCount(UINT cmd), retail entry RVA 0x233c0 (mfc140u), looks up the key
// through helper entry RVA 0x236e0 and returns the association value at +0x04.
// The local lookup mirrors the observed UINT-key hash and association layout;
// it returns 0 for an empty/missing map and guards a zero hash-table size rather
// than reproducing retail's divide fault. The null-this guard is also defensive.
// Symbol: ?GetCount@CMFCCmdUsageCount@@QEBAII@Z
extern "C" unsigned int MS_ABI impl__GetCount_CMFCCmdUsageCount__QEBAII_Z(
    void* pThis, unsigned int cmd)
{
    if (pThis == nullptr) return 0;
    const auto* map = reinterpret_cast<const CMFCCmdUsageCountMapLayout*>(
        reinterpret_cast<const unsigned char*>(pThis) + 0x08);
    return LookupCommandCount(map, cmd);
}
// HasEnoughInformation(): retail entry RVA 0x23440 (mfc140u) compares the
// total at +0x40 with m_nStartCount (RVA 0x3be1ac, mfc140u). The comparison is
// reproduced; the null-this guard is an OpenMFC deviation (retail dereferences).
// Symbol: ?HasEnoughInformation@CMFCCmdUsageCount@@QEBAHXZ
extern "C" int MS_ABI impl__HasEnoughInformation_CMFCCmdUsageCount__QEBAHXZ(
    void* pThis)
{
    if (pThis == nullptr) return 0;
    CU* s = reinterpret_cast<CU*>(pThis);
    return (static_cast<unsigned int>(s->m_nTotalCount) >=
            impl__m_nStartCount_CMFCCmdUsageCount__1IA) ? 1 : 0;
}
// IsFrequentlyUsedCmd(UINT cmd), retail entry RVA 0x233f0 (mfc140u): returns
// (GetCount(cmd) * 100 / total) > m_nMinUsagePercentage, or count != 0 when
// the threshold is zero; total==0 returns FALSE. Its arithmetic is reproduced;
// the null-this guard differs from retail.
// Symbol: ?IsFrequentlyUsedCmd@CMFCCmdUsageCount@@QEBAHI@Z
extern "C" int MS_ABI impl__IsFrequentlyUsedCmd_CMFCCmdUsageCount__QEBAHI_Z(
    void* pThis, unsigned int cmd)
{
    if (pThis == nullptr) return 0;
    CU* s = reinterpret_cast<CU*>(pThis);
    if (s->m_nTotalCount == 0) return 0;
    const unsigned int nCount = impl__GetCount_CMFCCmdUsageCount__QEBAII_Z(pThis, cmd);
    const unsigned int nMin = impl__m_nMinUsagePercentage_CMFCCmdUsageCount__1IA;
    if (nMin == 0) return (nCount != 0) ? 1 : 0;
    return (static_cast<unsigned int>(
                (nCount * 100u) / static_cast<unsigned int>(s->m_nTotalCount)) > nMin)
        ? 1 : 0;
}
// Reset(), retail entry RVA 0x233a0 (mfc140u), calls CMap::RemoveAll at entry
// RVA 0x1ba40 (mfc140u) on the member at +0x08, then zeros +0x40. This retained
// partial body only zeros +0x40; an existing map would remain populated.
// Symbol: ?Reset@CMFCCmdUsageCount@@QEAAXXZ
extern "C" void MS_ABI impl__Reset_CMFCCmdUsageCount__QEAAXXZ(
    void* pThis)
{
    if (pThis == nullptr) return;
    CU* s = reinterpret_cast<CU*>(pThis);
    // TODO(clean-room): retail also calls CMap::RemoveAll on +0x08 (entry
    // RVA 0x1ba40, mfc140u); this partial body does not invoke that adapter.
    s->m_nTotalCount = 0;
}
// Serialize(CArchive& ar), retail entry RVA 0x23250 (mfc140u), exchanges the
// total at +0x40 through archive fields at ar+0x38/+0x40, then dispatches map
// serialization through the map vtable slot at byte offset 0x10. OpenMFC's
// CArchive buffer fields are at different offsets (include/openmfc/afx.h), and
// this map has no compatible vtable path here, so this retained body is a no-op.
// Symbol: ?Serialize@CMFCCmdUsageCount@@UEAAXAEAVCArchive@@@Z
extern "C" void MS_ABI impl__Serialize_CMFCCmdUsageCount__UEAAXAEAVCArchive___Z(
    void* pThis, void* /*pAr*/)
{
    if (pThis == nullptr) return;
    // TODO(clean-room): no archive fields are touched: retail's CArchive
    // offsets do not match OpenMFC's layout and this body has no map-vtable
    // serialization path.
}
// SetOptions(UINT nStartCount, UINT nMinUsagePercentage), retail entry RVA
// 0x23450 (mfc140u): rejects percentages >= 100, then stores the two statics
// at RVAs 0x3be1ac and 0x3b1ab8 (mfc140u) and returns TRUE. Transcribed exactly.
// Symbol: ?SetOptions@CMFCCmdUsageCount@@SAHII@Z
extern "C" int MS_ABI impl__SetOptions_CMFCCmdUsageCount__SAHII_Z(
    unsigned int nStartCount, unsigned int nMinUsagePercentage)
{
    if (nMinUsagePercentage >= 0x64u) return 0;
    impl__m_nStartCount_CMFCCmdUsageCount__1IA = nStartCount;
    impl__m_nMinUsagePercentage_CMFCCmdUsageCount__1IA = nMinUsagePercentage;
    return 1;
}
// Retail constructor entry RVA 0x23190 (mfc140u) initializes both vfptrs,
// the CMap fields (17 buckets, block size 10), and the total at +0x40. This
// implementation initializes the data fields but cannot supply a compatible
// type-specific vfptr; it leaves the object's own vfptr untouched and map vfptr null.
// Symbol: ??0CMFCCmdUsageCount@@QEAA@XZ
extern "C" void* MS_ABI impl___0CMFCCmdUsageCount__QEAA_XZ(void* pThis) {
    if (pThis == nullptr) return nullptr;
    CU* s = reinterpret_cast<CU*>(pThis);
    std::memset(s->m_map, 0, sizeof(s->m_map));
    auto* map = reinterpret_cast<CMFCCmdUsageCountMapLayout*>(s->m_map);
    map->m_nHashTableSize = 0x11;
    map->m_nBlockSize = 10;
    s->m_nTotalCount = 0;
    return pThis;
}
// Retail destructor entry RVA 0x23230 (mfc140u) restores retail vftables and
// tail-jumps to CMap::RemoveAll at RVA 0x1ba40 (mfc140u) for the member at +0x08.
// Retail vftable addresses are not installed into OpenMFC objects; only the
// known map cleanup is reproduced. The CMap fields and CPlex chain layout are pinned above; bucket
// storage and CPlex blocks are both released with the malloc-backed allocator.
// Symbol: ??1CMFCCmdUsageCount@@UEAA@XZ
extern "C" void MS_ABI impl___1CMFCCmdUsageCount__UEAA_XZ(void* pThis) {
    if (pThis == nullptr) return;
    RemoveAllCmdUsageMap(reinterpret_cast<unsigned char*>(pThis) + 0x08);
}
