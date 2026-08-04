#include "CFileFindSupport.h"

static_assert(offsetof(CFileFindView, m_hFindFile) == 8,  "m_hFindFile @8");
static_assert(offsetof(CFileFindView, m_findData)  == 16, "m_findData @16");
