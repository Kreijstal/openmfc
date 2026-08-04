// COleConnPtContainer — OpenMFC implementation.
// Sources: olecore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/OlecoreSupport.h"

COleConnPtContainer::COleConnPtContainer() {
    memset(_oleconnptcontainer_padding, 0, sizeof(_oleconnptcontainer_padding));
}
COleConnPtContainer::~COleConnPtContainer() {
}
