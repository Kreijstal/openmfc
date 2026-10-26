// Native release-x64 plex allocation and chain release (0x271300/0x271340).
#include "detail/CPlexSupport.h"
extern "C" void MS_ABI impl__AfxThrowInvalidArgException__YAXXZ();
extern "C" void* MS_ABI impl___2_YAPEAX_K_Z(std::size_t);
// Symbol: ?Create@CPlex@@SAPEAU1@AEAPEAU1@_K1@Z
extern "C" CPlex* MS_ABI impl__Create_CPlex__SAPEAU1_AEAPEAU1__K1_Z(CPlex** head,unsigned __int64 count,unsigned __int64 size) {
    if(!count || !size)impl__AfxThrowInvalidArgException__YAXXZ();
    // The native API passes the wrapping size directly to MFC operator new.
    auto* block=static_cast<CPlex*>(impl___2_YAPEAX_K_Z(sizeof(CPlex)+count*size));
    block->pNext=*head;*head=block;return block;
}
// Symbol: ?FreeDataChain@CPlex@@QEAAXXZ
extern "C" void MS_ABI impl__FreeDataChain_CPlex__QEAAXXZ(CPlex* block) {
    while(block) {auto* next=block->pNext;std::free(block);block=next;}
}
