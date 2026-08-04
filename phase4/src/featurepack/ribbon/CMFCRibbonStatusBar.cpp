// CMFCRibbonStatusBar — OpenMFC implementation.
// Sources: mfccore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/MfccoreSupport.h"

CMFCRibbonStatusBar::CMFCRibbonStatusBar() { memset(_pad, 0, sizeof(_pad)); }
CMFCRibbonStatusBar::~CMFCRibbonStatusBar() {}
