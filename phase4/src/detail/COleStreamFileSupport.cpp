#include "COleStreamFileSupport.h"

static_assert(sizeof(OleStreamFileView) == 64, "COleStreamFile must be 64 bytes");
static_assert(offsetof(OleStreamFileView, m_lpStream) == 24, "m_lpStream @ 24");
