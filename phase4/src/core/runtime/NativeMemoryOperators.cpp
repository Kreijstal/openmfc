// MFC's internal array storage and deleting destructors use its malloc/free
// boundary, including the module new-handler behavior supplied by MemcoreSupport.
#include <cstddef>
#include <new>
extern "C" void* impl___2_YAPEAX_K_Z(std::size_t);
extern "C" void impl___3_YAXPEAX_Z(void*);
// Symbol: ??_U@YAPEAX_K@Z
void* operator new[](std::size_t size) {return impl___2_YAPEAX_K_Z(size);}
// Symbol: ??_V@YAXPEAX@Z
void operator delete[](void* data) noexcept {impl___3_YAXPEAX_Z(data);}
void operator delete(void* data) noexcept {impl___3_YAXPEAX_Z(data);}
