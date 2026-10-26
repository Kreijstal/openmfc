#include "CFixedAllocSupport.h"
#include <cstdint>

static_assert(sizeof(CFixedAllocState)==24 && offsetof(CFixedAllocState,m_pBlocks)==8 &&
              offsetof(CFixedAllocState,m_pNodeFree)==16,"native fixed allocator prefix");
static_assert(sizeof(CFixedAllocSynchronizedState)==64 && offsetof(CFixedAllocSynchronizedState,m_lock)==24,
              "native fixed allocator critical section");
extern "C" CPlex* MS_ABI impl__Create_CPlex__SAPEAU1_AEAPEAU1__K1_Z(CPlex**,unsigned __int64,unsigned __int64);
namespace openmfc { namespace detail { namespace cfixedalloc {
void fa_construct(CFixedAllocState* p,unsigned int size,unsigned int count) {
    p->m_pNodeFree=nullptr;p->m_pBlocks=nullptr;
    p->m_nAllocSize=size<sizeof(void*)?sizeof(void*):size;
    p->m_nBlockSize=count>1?count:64;
}
void fa_free_all(CFixedAllocState* p) {
    auto* block=p->m_pBlocks;
    while(block) {auto* next=block->pNext;std::free(block);block=next;}
    p->m_pBlocks=nullptr;p->m_pNodeFree=nullptr;
}
void* fa_alloc(CFixedAllocState* p) {
    if(!p->m_pNodeFree) {
        auto* block=impl__Create_CPlex__SAPEAU1_AEAPEAU1__K1_Z(&p->m_pBlocks,p->m_nBlockSize,p->m_nAllocSize);
        unsigned int index=p->m_nBlockSize-1;
        // Native indexing multiplies in 32 bits before extending the offset.
        uintptr_t node=reinterpret_cast<uintptr_t>(block)+sizeof(CPlex)+index*p->m_nAllocSize;
        for(;static_cast<int32_t>(index)>=0;--index) {
            auto* freeNode=reinterpret_cast<CFixedAllocState::CNode*>(node);
            freeNode->pNext=p->m_pNodeFree;p->m_pNodeFree=freeNode;
            node-=p->m_nAllocSize;
        }
    }
    auto* node=p->m_pNodeFree;p->m_pNodeFree=node->pNext;return node;
}
void fa_free(CFixedAllocState* p,void* memory) {
    if(!memory)return;
    auto* node=static_cast<CFixedAllocState::CNode*>(memory);
    node->pNext=p->m_pNodeFree;p->m_pNodeFree=node;
}
} } }
