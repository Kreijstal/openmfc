#pragma once
#include "openmfc/afxwin.h"
namespace openmfc { namespace detail {
BOOL DispatchCommandMessage(CCmdTarget* target,UINT id,void* function,void* extra,UINT_PTR signature,AFX_CMDHANDLERINFO* info);
} }
