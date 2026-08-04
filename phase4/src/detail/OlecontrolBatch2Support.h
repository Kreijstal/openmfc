#pragma once
// Shared internals of the former global_olecontrol_batch2.cpp translation unit.
// Definitions live in detail/OlecontrolBatch2Support.cpp, so file-scope state keeps a
// single instance across the classes this unit was split into.
namespace openmfc { namespace detail { namespace olecontrolbatch2 {} } }
using namespace openmfc::detail::olecontrolbatch2;
// Second batch of COleControl methods implemented on the retail member layout
// transcribed in a28f781. Each was decoded from the retail mfc140u export
// (ordinal -> RVA -> objdump) rather than inferred.
//
// Retail bitfield word at this+0x160; bit positions used below are the ones
// the shipping code tests or sets:
//   bit 16 (0x10) via bts = m_bSimpleFrame
//   bit 18                = m_bInitialized, read by IsInvokeAllowed
// Both are named bitfields in afxole.h, so the code below spells them out
// instead of masking the raw word.
//
// Other offsets referenced:
//   0x150 = 336 m_pReflect
//   0x178 = 376 m_font
//
// Kept in a global_*.cpp file so the build's AUTO_EXCLUDES pass drops the
// generated stubs for these ordinals.

#include "openmfc/afxwin.h"
#include "openmfc/afxdisp.h"
#include "openmfc/afxole.h"

#include <cstddef>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif








