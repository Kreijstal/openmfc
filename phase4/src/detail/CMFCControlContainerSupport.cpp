#include "CMFCControlContainerSupport.h"
static_assert(offsetof(CMFCControlContainer,m_pWnd)==8,"native container parent");
static_assert(offsetof(CMFCControlContainer,m_arSubclassedCtrls)==16,"native embedded object array");
