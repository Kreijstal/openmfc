#include <cstdlib>
#include <new>

void* operator new(std::size_t sz) { return std::malloc(sz); }
void operator delete(void* p) noexcept { std::free(p); }
void* operator new[](std::size_t sz) { return std::malloc(sz); }
void operator delete[](void* p) noexcept { std::free(p); }
