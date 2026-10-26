#pragma once
#include <cstdlib>
// Keep MFC operator new and the exception factory as explicit dependency
// boundaries. These collection tests exercise their callers' requested sizes
// and exception propagation, including deliberately wrapping CPlex sizes.
extern "C" void* MS_ABI impl___2_YAPEAX_K_Z(std::size_t size) {return std::malloc(size);}
extern "C" void MS_ABI impl__AfxThrowInvalidArgException__YAXXZ() {throw 19;}
