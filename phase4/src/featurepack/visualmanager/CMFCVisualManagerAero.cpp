// CMFCVisualManagerAero — OpenMFC implementation.
// Sources: mfccore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/MfccoreSupport.h"

CMFCVisualManagerAero::CMFCVisualManagerAero() {
    memset(_pad, 0, sizeof(_pad));
    ApplyVisualPaletteForClass(GetThisClass());
}
CMFCVisualManagerAero::~CMFCVisualManagerAero() {}
