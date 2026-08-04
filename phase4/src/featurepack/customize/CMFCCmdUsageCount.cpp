// CMFCCmdUsageCount — OpenMFC implementation.
// Sources: global_cmfccmdusagecount.cpp, manual_small_stub_implementations.cpp

#include "detail/CMFCCmdUsageCountSupport.h"
#include "detail/ManualSmallStubImplementationsSupport.h"

// AddCmd(UINT cmd): records one usage of a command.  Retail (RVA 0x232e0)
// skips the whole body when the module flag at 0x1803be35c is set or when the
// global command hook (0x1803be3b0, +0x40 == cmd) reports the command, then
// bumps the per-command counter in the CMap at +0x8 and increments the total
// count (0x40).  The command-range gate and the total-count bump are
// transcribed; the map (lookup 0x1800236e0 / insert 0x18002346c) is not
// modeled, so the per-command counter is not maintained.
// Symbol: ?AddCmd@CMFCCmdUsageCount@@QEAAXI@Z
extern "C" void MS_ABI impl__AddCmd_CMFCCmdUsageCount__QEAAXI_Z(
    void* pThis, unsigned int cmd)
{
    if (pThis == nullptr) return;
    const unsigned int nCmd = cmd;
    // Retail range gate: [0xE120,0xF0FF] (ID_COMMANDs), [1,0xE10F] (standard
    // command range) or [0xF1F0,0xFFFFFFFE] (the latter is made dead by the
    // 0xFF00 test below but is kept for faithfulness).
    const bool bRange1 = (nCmd >= 0xE120u) && (nCmd - 0xE120u) <= 0xEDFu;
    const bool bRange2 = (nCmd >= 1u) && (nCmd - 1u) <= 0xE10Eu;
    const bool bRange3 = (nCmd >= 0xF1F0u) && (nCmd - 0xF1F0u) <= 0xFFFF0E0Eu;
    if (!(bRange1 || bRange2 || bRange3)) return;
    if (nCmd >= 0xFF00u) return;   // retail: `cmp $0xff00,%ebx; jge`
    if (nCmd >= 0xE210u && nCmd - 0xE210u <= 0xFu) return;  // [0xE210,0xE21F]
    // TODO(clean-room): retail also skips when the global hook at 0x1803be3b0
    // reports the command, and maintains the per-command counter through the
    // CMap at +0x8 (0x1800236e0 / 0x18002346c); neither is modeled.
    CU* s = reinterpret_cast<CU*>(pThis);
    ++s->m_nTotalCount;   // 0x40
}
// GetCount(UINT cmd): returns the stored per-command usage count (or 0).  The
// retail body (RVA 0x233c0) is a single CMap::Lookup on the map at +0x8; the
// map is not modeled, so the not-found terminal 0 is returned.
// Symbol: ?GetCount@CMFCCmdUsageCount@@QEBAII@Z
extern "C" unsigned int MS_ABI impl__GetCount_CMFCCmdUsageCount__QEBAII_Z(
    void* pThis, unsigned int /*cmd*/)
{
    if (pThis == nullptr) return 0;
    // TODO(clean-room): retail runs CMap::Lookup(&this->0x8, cmd, ...) at
    // 0x1800236e0 and returns the stored value (or 0); the CMap is not
    // modeled.
    return 0;
}
// HasEnoughInformation(): returns (m_nTotalCount >= m_nStartCount).  Fully
// transcribed from retail RVA 0x23440.
// Symbol: ?HasEnoughInformation@CMFCCmdUsageCount@@QEBAHXZ
extern "C" int MS_ABI impl__HasEnoughInformation_CMFCCmdUsageCount__QEBAHXZ(
    void* pThis)
{
    if (pThis == nullptr) return 0;
    CU* s = reinterpret_cast<CU*>(pThis);
    return (static_cast<unsigned int>(s->m_nTotalCount) >=
            impl__m_nStartCount_CMFCCmdUsageCount__1IA) ? 1 : 0;
}
// IsFrequentlyUsedCmd(UINT cmd): returns (GetCount(cmd) * 100 / total) >
// m_nMinUsagePercentage (or simply count != 0 when the percentage threshold
// is 0).  Transcribed from retail RVA 0x233f0; GetCount conservatively
// returns 0 so the frequent-use test falls back to the not-found terminal.
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
// Reset(): clears the command map and the total count.  The retail body
// (RVA 0x233a0) runs CMap::RemoveAll on the map at +0x8 (0x18001ba40) and
// zeroes 0x40; the map is not modeled.
// Symbol: ?Reset@CMFCCmdUsageCount@@QEAAXXZ
extern "C" void MS_ABI impl__Reset_CMFCCmdUsageCount__QEAAXXZ(
    void* pThis)
{
    if (pThis == nullptr) return;
    CU* s = reinterpret_cast<CU*>(pThis);
    // TODO(clean-room): retail also runs CMap::RemoveAll(&this->0x8) at
    // 0x18001ba40; the CMap is not modeled.
    s->m_nTotalCount = 0;
}
// Serialize(CArchive& ar): serializes the total count and the command map.
// The retail body (RVA 0x23250) exchanges 0x40 through the archive buffer
// pointers at ar+0x38/0x40 and serializes the map at +0x8 through its vtable
// slot 0x10.  The OpenMFC CArchive layout differs from retail, so no archive
// access is performed (conservative no-op).
// Symbol: ?Serialize@CMFCCmdUsageCount@@UEAAXAEAVCArchive@@@Z
extern "C" void MS_ABI impl__Serialize_CMFCCmdUsageCount__UEAAXAEAVCArchive___Z(
    void* pThis, void* /*pAr*/)
{
    if (pThis == nullptr) return;
    // TODO(clean-room): transcribed partially -- retail serializes the 0x40
    // total through the CArchive buffer pointers (ar+0x38/0x40) and the CMap
    // at +0x8 via its vtable; the OpenMFC CArchive layout differs from retail
    // and the map is unmodeled, so nothing is serialized.
}
// SetOptions(UINT nStartCount, UINT nMinUsagePercentage): class-static
// setter.  Fully transcribed from retail RVA 0x23450: rejects percentages
// >= 100, then stores both statics and returns TRUE.
// Symbol: ?SetOptions@CMFCCmdUsageCount@@SAHII@Z
extern "C" int MS_ABI impl__SetOptions_CMFCCmdUsageCount__SAHII_Z(
    unsigned int nStartCount, unsigned int nMinUsagePercentage)
{
    if (nMinUsagePercentage >= 0x64u) return 0;
    impl__m_nStartCount_CMFCCmdUsageCount__1IA = nStartCount;
    impl__m_nMinUsagePercentage_CMFCCmdUsageCount__1IA = nMinUsagePercentage;
    return 1;
}
// Symbol: ??0CMFCCmdUsageCount@@QEAA@XZ
extern "C" void* MS_ABI impl___0CMFCCmdUsageCount__QEAA_XZ(void* pThis) {
    return pThis;
}
// Symbol: ??1CMFCCmdUsageCount@@UEAA@XZ
extern "C" void MS_ABI impl___1CMFCCmdUsageCount__UEAA_XZ(void* pThis) {
    (void)pThis;
}
