#pragma once
// Shared internals of the former richedit_ole_ext.cpp translation unit.
// Definitions live in detail/CRichEditDocSupport.cpp, so file-scope state keeps a
// single instance across the classes this unit was split into.
namespace openmfc { namespace detail { namespace cricheditdoc {} } }
using namespace openmfc::detail::cricheditdoc;
#include <cstring>

#include "openmfc/afxole.h"

#ifdef __GNUC__
#define MS_ABI __attribute__((ms_abi))
#else
#define MS_ABI
#endif
















