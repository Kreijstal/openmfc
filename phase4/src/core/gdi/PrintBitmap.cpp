// Private 32-bit DIB ownership used by CScrollView's print-client buffer.
#include "detail/PrintBitmapSupport.h"
#include <cstdlib>
#include <cstring>

namespace openmfc { namespace detail {
void* (*printBitmapAllocate)(size_t)=&std::malloc;
void (*printBitmapFree)(void*)=&std::free;
namespace {
struct ImageContext {CRITICAL_SECTION lock;LONG references;HRESULT initialization;};
ImageContext context{};
bool initialized=false;
void FinishContext() {
    ::EnterCriticalSection(&context.lock);
    ::LeaveCriticalSection(&context.lock);
}
ImageContext& Context() {
    if(!initialized) {
        initialized=true;
        if(!::InitializeCriticalSectionAndSpinCount(&context.lock,0)) {
            DWORD error=::GetLastError();
            context.initialization=static_cast<LONG>(error)>0?HRESULT_FROM_WIN32(error):static_cast<HRESULT>(error);
        }
        std::atexit(&FinishContext);
    }
    return context;
}
struct BitmapInfoAllocation {
    void* next;
    size_t reserved;
    BITMAPINFOHEADER header;
    RGBQUAD colors[256];
};
static_assert(sizeof(BitmapInfoAllocation)==0x438,"native print bitmap allocation");
}
void PrintBitmap::Initialize() {
    bitmap_=nullptr;bits_=nullptr;
    auto& state=Context();
    ::EnterCriticalSection(&state.lock);
    ++state.references;
    ::LeaveCriticalSection(&state.lock);
}
// Native ATL image creation has a noexcept boundary, including its Win32
// calls and temporary allocation. A callback exception terminates immediately.
BOOL PrintBitmap::Create(int width,int height) noexcept {
    auto* allocation=static_cast<BitmapInfoAllocation*>(printBitmapAllocate(sizeof(BitmapInfoAllocation)));
    if(!allocation)return FALSE;
    allocation->next=nullptr;
    std::memset(&allocation->header,0,sizeof(allocation->header));
    allocation->header.biSize=sizeof(BITMAPINFOHEADER);
    allocation->header.biWidth=width;
    allocation->header.biHeight=height;
    allocation->header.biPlanes=1;
    allocation->header.biBitCount=32;
    BOOL created=FALSE;
    bitmap_=::CreateDIBSection(nullptr,reinterpret_cast<BITMAPINFO*>(&allocation->header),DIB_RGB_COLORS,&bits_,nullptr,0);
    if(bitmap_) {
        // Match the native image attachment query. The print path uses
        // only the bitmap handle, so it needs no cached pixel metadata.
        DIBSECTION section;
        ::GetObjectW(bitmap_,sizeof(section),&section);
        created=TRUE;
    }
    printBitmapFree(allocation);
    return created;
}
void PrintBitmap::Destroy() {
    HBITMAP previous=bitmap_;
    bitmap_=nullptr;bits_=nullptr;
    if(previous)::DeleteObject(previous);
    auto& state=Context();
    ::EnterCriticalSection(&state.lock);
    if(!--state.references) {
        ::EnterCriticalSection(&state.lock);
        ::LeaveCriticalSection(&state.lock);
    }
    ::LeaveCriticalSection(&state.lock);
}
} }
