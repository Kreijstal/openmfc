// COleControlModule — OpenMFC implementation.
// Sources: global_oleresidual_rtti.cpp

#include "detail/COleControlModuleSupport.h"

int  COleControlModule::ExitInstance() { return CWinApp::ExitInstance(); }
