#pragma once
#include <cstdint>

// Plain C boundary: the runner never instantiates or substitutes an MFC type.
struct ArrayEvent {std::int64_t kind,a,b,c,size,capacity,grow,data;};
struct ArrayReport {unsigned count,overflow;ArrayEvent events[131072];};
using RunArrayProbe=void(__cdecl*)(int kind,int scenario,int failure,ArrayReport*);
