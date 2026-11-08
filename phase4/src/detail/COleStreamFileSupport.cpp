#include "COleStreamFileSupport.h"
static_assert(sizeof(COleStreamFile) == 56 && offsetof(COleStreamFile,m_lpStream) == 40,
              "native stream file model");
