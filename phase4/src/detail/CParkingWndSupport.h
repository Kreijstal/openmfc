#pragma once
// Shared internals of the former global_cparkingwnd.cpp translation unit.
// Definitions live in detail/CParkingWndSupport.cpp, so file-scope state keeps a
// single instance across the classes this unit was split into.
namespace openmfc { namespace detail { namespace cparkingwnd {} } }
using namespace openmfc::detail::cparkingwnd;
// CParkingWnd uses the standard CWnd message dispatch path.

#include <cstdint>

#ifdef __GNUC__
#define MS_ABI __attribute__((ms_abi))
#else
#define MS_ABI
#endif

struct CWnd;

extern "C" std::intptr_t MS_ABI impl__WindowProc_CWnd__MEAA_JI_K_J_Z(
    CWnd* window,
    unsigned int message,
    std::uintptr_t wParam,
    std::intptr_t lParam
);

