// Native pointer map: 56-byte object, 24-byte associations, CPlex blocks.
#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxwin.h"
#include "detail/CPlexSupport.h"
#include <cstring>
#include <cstdint>
extern "C" void MS_ABI impl__AfxThrowInvalidArgException__YAXXZ();
extern "C" void* MS_ABI impl___2_YAPEAX_K_Z(size_t);
extern "C" CPlex* MS_ABI impl__Create_CPlex__SAPEAU1_AEAPEAU1__K1_Z(CPlex**,unsigned __int64,unsigned __int64);
IMPLEMENT_DYNAMIC(CMapPtrToPtr,CObject)
CMapPtrToPtr::CMapPtrToPtr(INT_PTR block) : m_pHashTable(nullptr),m_nHashTableSize(17),
    m_nCount(0),m_pFreeList(nullptr),m_pBlocks(nullptr),m_nBlockSize(block>0 ? block : 10) {}
CMapPtrToPtr::~CMapPtrToPtr(){RemoveAll();}
UINT CMapPtrToPtr::HashKey(void* key) const {
    // Retail uses ldiv on the signed low 32 bits, followed by Schrage's
    // Park-Miller step. Keys still compare with all 64 pointer bits.
    const auto value=static_cast<int32_t>(reinterpret_cast<UINT_PTR>(key));
    int64_t hash=16807LL*(value%127773)-2836LL*(value/127773);
    if(hash<0)hash+=0x7fffffff;
    return static_cast<UINT>(hash);
}
CMapPtrToPtr::CAssoc* CMapPtrToPtr::GetAssocAt(void* key,UINT& bucket,UINT& hash) const {
    hash=HashKey(key);bucket=hash%m_nHashTableSize;
    if(m_pHashTable)for(CAssoc* a=m_pHashTable[bucket];a;a=a->pNext)if(a->key==key)return a;
    return nullptr;
}
BOOL CMapPtrToPtr::Lookup(void* key,void*& value)const {
    UINT bucket,hash;CAssoc* a=GetAssocAt(key,bucket,hash);
    if(!a)return FALSE;value=a->value;return TRUE;
}
void CMapPtrToPtr::InitHashTable(UINT size,BOOL allocate) {
    if(!size)size=17;
    if(m_pHashTable){std::free(m_pHashTable);m_pHashTable=nullptr;}
    if(allocate) {
        auto* table=static_cast<CAssoc**>(impl___2_YAPEAX_K_Z(size_t(size)*sizeof(CAssoc*)));
        m_pHashTable=table;std::memset(table,0,size_t(size)*sizeof(CAssoc*));
    }
    m_nHashTableSize=size;
}
CMapPtrToPtr::CAssoc* CMapPtrToPtr::NewAssoc() {
    if(!m_pFreeList) {
        auto* block=impl__Create_CPlex__SAPEAU1_AEAPEAU1__K1_Z(&m_pBlocks,m_nBlockSize,sizeof(CAssoc));
        auto* assocs=static_cast<CAssoc*>(block->data());
        for(INT_PTR i=m_nBlockSize;i>0;--i) {CAssoc* a=assocs+i-1;a->pNext=m_pFreeList;m_pFreeList=a;}
    }
    CAssoc* a=m_pFreeList;m_pFreeList=a->pNext;++m_nCount;
    a->key=nullptr;a->value=nullptr;return a;
}
void CMapPtrToPtr::FreeAssoc(CAssoc* a) {
    a->pNext=m_pFreeList;m_pFreeList=a;--m_nCount;if(!m_nCount)RemoveAll();
}
void*& CMapPtrToPtr::operator[](void* key) {
    UINT bucket,hash;CAssoc* a=GetAssocAt(key,bucket,hash);
    if(!a) {
        if(!m_pHashTable)InitHashTable(m_nHashTableSize);
        a=NewAssoc();a->key=key;a->pNext=m_pHashTable[bucket];m_pHashTable[bucket]=a;
    }
    return a->value;
}
BOOL CMapPtrToPtr::RemoveKey(void* key) {
    if(!m_pHashTable)return FALSE;
    CAssoc** link=m_pHashTable+HashKey(key)%m_nHashTableSize;
    while(*link) {CAssoc* a=*link;if(a->key==key) {*link=a->pNext;FreeAssoc(a);return TRUE;}link=&a->pNext;}
    return FALSE;
}
void CMapPtrToPtr::RemoveAll() {
    if(m_pHashTable) {std::free(m_pHashTable);m_pHashTable=nullptr;}
    CPlex* block=m_pBlocks;m_nCount=0;m_pFreeList=nullptr;
    while(block) {CPlex* next=block->pNext;std::free(block);block=next;}
    m_pBlocks=nullptr;
}
void CMapPtrToPtr::GetNextAssoc(POSITION& pos,void*& key,void*& value)const {
    if(!pos)return;
    CAssoc* a=reinterpret_cast<CAssoc*>(pos);UINT bucket=0;
    if(pos==reinterpret_cast<POSITION>(INT_PTR(-1))) {
        for(;bucket<m_nHashTableSize;++bucket)if(m_pHashTable[bucket]) {a=m_pHashTable[bucket];break;}
        if(bucket==m_nHashTableSize) {impl__AfxThrowInvalidArgException__YAXXZ();return;}
    }
    CAssoc* next=a->pNext;
    if(!next)for(bucket=HashKey(a->key)%m_nHashTableSize+1;bucket<m_nHashTableSize;++bucket)
        if(m_pHashTable[bucket]) {next=m_pHashTable[bucket];break;}
    pos=reinterpret_cast<POSITION>(next);key=a->key;value=a->value;
}
static_assert(sizeof(CMapPtrToPtr)==56 && sizeof(CMapPtrToPtr::CAssoc)==24 && sizeof(CMapPtrToPtr::POSITION)==8,"native pointer map ABI");
static_assert(offsetof(CMapPtrToPtr,m_pHashTable)==8 && offsetof(CMapPtrToPtr,m_nHashTableSize)==16 &&
    offsetof(CMapPtrToPtr,m_nCount)==24 && offsetof(CMapPtrToPtr,m_pFreeList)==32 &&
    offsetof(CMapPtrToPtr,m_pBlocks)==40 && offsetof(CMapPtrToPtr,m_nBlockSize)==48,"native map members");
static_assert(offsetof(CMapPtrToPtr::CAssoc,key)==8 && offsetof(CMapPtrToPtr::CAssoc,value)==16,"native pointer association members");
