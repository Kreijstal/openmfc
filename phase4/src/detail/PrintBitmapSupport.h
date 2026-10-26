#pragma once
#include <windows.h>
#include <cstddef>

namespace openmfc { namespace detail {
// The print-client image uses the CRT's nullable allocation contract.
extern void* (*printBitmapAllocate)(size_t);
extern void (*printBitmapFree)(void*);

class PrintBitmap {
public:
    void Initialize();
    BOOL Create(int width,int height) noexcept;
    void Destroy();
    HBITMAP Handle() const {return bitmap_;}
private:
    HBITMAP bitmap_=nullptr;
    void* bits_=nullptr;
};
} }
