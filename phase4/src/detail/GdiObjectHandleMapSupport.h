#pragma once
#include "detail/CHandleMapSupport.h"
#include "detail/ModuleThreadStateSupport.h"

namespace openmfc { namespace detail {
CHandleMap* GetGdiObjectHandleMap(bool create);
} }
