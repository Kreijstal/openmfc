#pragma once
// Shared internals of the former global_cmfccmdusagecount.cpp translation unit.
// Definitions live in detail/CMFCCmdUsageCountSupport.cpp, so file-scope state keeps a
// single instance across the classes this unit was split into.
namespace openmfc { namespace detail { namespace cmfccmdusagecount {} } }
using namespace openmfc::detail::cmfccmdusagecount;
// OpenMFC: CMFCCmdUsageCount exports.
//
// All 7 exports are implemented here, transcribed from the retail mfc140u.dll
// (14.51.36231, x64) by decoding the export table (ordinal -> RVA -> objdump
// of the retail body).
//
// The class is not declared in the OpenMFC headers (opaque), so `this` is taken
// as void*.  The statics plumbing (HasEnoughInformation / SetOptions / Reset /
// the AddCmd range gate) is transcribed faithfully; the CMap at +0x8 that holds
// the per-command counters is unmodeled, so the bodies that would read it keep
// their guards and return the retail terminal value with a
// `// TODO(clean-room)` marker.
//
// Conventions (see IMPLEMENTATION_GUIDE.md):
//   * every export is `extern "C"` + `__attribute__((ms_abi))`;
//   * the implicit `this` is the first explicit parameter;
//   * NULL-check pThis everywhere;
//   * struct-returning exports take the hidden return slot in the register the
//     retail body uses, as documented at each definition.
//
// Kept in a global_*.cpp file so the build's AUTO_EXCLUDES pass (the scan of
// `// Symbol:` comments) drops the generated weak stubs for these ordinals.

#include "openmfc/afxwin.h"
#include "openmfc/afxmfc.h"

#include <windows.h>

#include <cstddef>
#include <cstdint>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

// Class statics (defined in feature_static_data.cpp).
extern "C" std::uint32_t impl__m_nStartCount_CMFCCmdUsageCount__1IA;
extern "C" std::uint32_t impl__m_nMinUsagePercentage_CMFCCmdUsageCount__1IA;
// Forward declaration (defined below).
extern "C" unsigned int MS_ABI impl__GetCount_CMFCCmdUsageCount__QEBAII_Z(
    void* pThis, unsigned int cmd);


namespace openmfc { namespace detail { namespace cmfccmdusagecount {
struct CU {
    char            _pad000[0x8];  // 0x00: CObject vptr
    char            m_map[0x38];   // 0x08: CMap<UINT,UINT,UINT,UINT> (opaque)
    unsigned int    m_nTotalCount; // 0x40: total recorded command count
    char            _pad044[0x48 - 0x44];
};
} } }









