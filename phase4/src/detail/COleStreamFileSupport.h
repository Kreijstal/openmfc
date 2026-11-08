#pragma once
#include "openmfc/afxole.h"
#ifndef MS_ABI
#define MS_ABI
#endif
namespace openmfc { namespace detail { namespace colestreamfile {
inline IStream*& Stream(void* p) { return static_cast<COleStreamFile*>(p)->m_lpStream; }
} } }
using namespace openmfc::detail::colestreamfile;
