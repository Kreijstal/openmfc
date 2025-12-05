#include <new>
#include <cstdlib>

// Global new/delete overrides to use UCRT directly
// This ensures that memory allocated by OpenMFC can be freed by the host app and vice-versa.

void* operator new(std::size_t count) {
    void* p = std::malloc(count);
    if (!p) throw std::bad_alloc();
    return p;
}

void operator delete(void* ptr) noexcept {
    std::free(ptr);
}

void* operator new[](std::size_t count) {
    void* p = std::malloc(count);
    if (!p) throw std::bad_alloc();
    return p;
}

void operator delete[](void* ptr) noexcept {
    std::free(ptr);
}
