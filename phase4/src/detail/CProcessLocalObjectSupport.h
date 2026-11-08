#pragma once
#include <windows.h>
#include <cstddef>
#ifndef MS_ABI
#ifdef __GNUC__
#define MS_ABI __attribute__((ms_abi))
#else
#define MS_ABI
#endif
#endif
namespace openmfc { namespace detail { namespace cprocesslocalobject {
// The native holder owns a CNoTrackObject through its deleting destructor.
struct S_Cprocesslocalobject {void* m_pObject;};
using DeletingDtor=void* (MS_ABI*)(void*,unsigned);
using CreateFn=void* (MS_ABI*)();
}}}
using namespace openmfc::detail::cprocesslocalobject;
