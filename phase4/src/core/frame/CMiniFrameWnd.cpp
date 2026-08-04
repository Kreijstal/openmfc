// CMiniFrameWnd — OpenMFC implementation.
// Sources: mfccore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/MfccoreSupport.h"

CMiniFrameWnd::CMiniFrameWnd() { memset(_pad, 0, sizeof(_pad)); }
CMiniFrameWnd::~CMiniFrameWnd() {}
