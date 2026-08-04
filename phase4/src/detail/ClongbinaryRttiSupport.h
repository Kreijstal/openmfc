#pragma once
// Shared internals of the former global_clongbinary_rtti.cpp translation unit.
// Definitions live in detail/ClongbinaryRttiSupport.cpp, so file-scope state keeps a
// single instance across the classes this unit was split into.
namespace openmfc { namespace detail { namespace clongbinaryrtti {} } }
using namespace openmfc::detail::clongbinaryrtti;
// OpenMFC: CLongBinary RTTI — GetThisClass / GetRuntimeClass.
//
// CLongBinary : public CObject (afxdao.h) is a simple holder for a block of
// binary data (HGLOBAL m_hData + DWORD m_dwDataLength) used by the database
// classes for long-binary columns. It is IMPLEMENT_DYNAMIC (no DYNCREATE
// factory), so only the two RTTI getters are exported; the descriptor itself is
// internal (not an exported symbol). The descriptor chains to CObject.
//
// Layout (verified at runtime): sizeof(CLongBinary) == 24 (vptr + HGLOBAL +
// DWORD, padded). The behavioral test static_asserts this against the real
// afxdao.h type so the literal below cannot silently drift.

#include "openmfc/afx.h"   // CObject, CRuntimeClass, CObject::classCObject

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

namespace openmfc { namespace detail { namespace clongbinaryrtti {
extern CRuntimeClass classCLongBinary;
} } }


