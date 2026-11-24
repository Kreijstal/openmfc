#pragma once

#include "openmfc/afxmfc.h"
#include <cstddef>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

namespace openmfc::detail::cmditabinfo {
using S_Cmditabinfo = CMDITabInfo;
}
using openmfc::detail::cmditabinfo::S_Cmditabinfo;
