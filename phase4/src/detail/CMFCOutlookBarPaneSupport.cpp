#include "CMFCOutlookBarPaneSupport.h"

static_assert(sizeof(OutlookBarPane) == 0x2b68, "CMFCOutlookBarPane size mismatch");
static_assert(sizeof(CObList) == 0x38, "CObList size mismatch");
static_assert(sizeof(CMFCToolBarImages) == 0x198, "CMFCToolBarImages size mismatch");
