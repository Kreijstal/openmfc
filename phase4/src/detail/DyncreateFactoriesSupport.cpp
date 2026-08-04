#include "DyncreateFactoriesSupport.h"

static_assert(sizeof(CObjectShim) == sizeof(CObject), "shim must not change CObject layout");
