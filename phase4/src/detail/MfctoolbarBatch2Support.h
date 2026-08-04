#pragma once
// Shared internals of the former global_mfctoolbar_batch2.cpp translation unit.
// Definitions live in detail/MfctoolbarBatch2Support.cpp, so file-scope state keeps a
// single instance across the classes this unit was split into.
namespace openmfc { namespace detail { namespace mfctoolbarbatch2 {} } }
using namespace openmfc::detail::mfctoolbarbatch2;
// Second batch of CMFCToolBar methods implemented on the retail member layout
// corrected in 35e9572. Each was decoded from the retail mfc140u export
// (ordinal -> RVA -> objdump); the offsets the shipping code uses were then
// checked against this header's layout with a compile-time offsetof probe, so
// both directions agree before anything here is trusted.
//
// Retail offsets referenced below:
//   0x1308 = 4872 m_uiOriginalResID
//
// Only CanBeRestored is added here. GetCount and EnableDocking already have
// working implementations backed by the toolbar side table (mfccore.cpp), and
// re-expressing them as raw member reads would report 0 buttons for every
// toolbar built through OpenMFC's own AddButton path. EnableDocking is instead
// extended in place so it maintains the retail members as well.
//
// Kept in a global_*.cpp file so the build's AUTO_EXCLUDES pass drops the
// generated stubs for these ordinals.

#include "openmfc/afxwin.h"
#include "openmfc/afxmfc.h"

#include <cstddef>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

// These are the offsets the retail code hard-codes; assert the header agrees
// rather than trusting the transcription.

