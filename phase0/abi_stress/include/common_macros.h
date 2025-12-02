#pragma once

#ifdef ABI_STRESS_EXPORTS
    #define ABI_STRESS_API __declspec(dllexport)
#else
    #define ABI_STRESS_API __declspec(dllimport)
#endif
