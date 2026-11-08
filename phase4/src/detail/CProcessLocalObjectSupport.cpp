#include "CProcessLocalObjectSupport.h"
static_assert(sizeof(S_Cprocesslocalobject)==8 && offsetof(S_Cprocesslocalobject,m_pObject)==0,
              "native process-local holder");
