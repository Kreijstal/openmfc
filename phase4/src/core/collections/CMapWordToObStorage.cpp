// Native 56-byte map with 24-byte associations and pointer-sized POSITION.
#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxwin.h"
#include "detail/CPlexSupport.h"
#include "detail/NativeCollectionArchive.h"
#include <cstring>
#include <new>
extern "C" void MS_ABI impl__AfxThrowMemoryException__YAXXZ();
extern "C" void MS_ABI impl__AfxThrowInvalidArgException__YAXXZ();
IMPLEMENT_SERIAL(CMapWordToOb,CObject,0xffff)
CMapWordToOb::CMapWordToOb(INT_PTR block) : m_pHashTable(nullptr),m_nHashTableSize(17),m_nCount(0),m_pFreeList(nullptr),m_pBlocks(nullptr),m_nBlockSize(block) {
    if(block<=0)impl__AfxThrowInvalidArgException__YAXXZ();
}
CMapWordToOb::~CMapWordToOb(){RemoveAll();}
CMapWordToOb::CAssoc* CMapWordToOb::GetAssocAt(WORD key,UINT& hash,UINT& bucket) const {
    hash=HashKey(key);bucket=hash%m_nHashTableSize;
    if(m_pHashTable)for(CAssoc* a=m_pHashTable[bucket];a;a=a->pNext)if(a->key==key)return a;
    return nullptr;
}
BOOL CMapWordToOb::Lookup(WORD key,CObject*& value)const {
    UINT hash,bucket;CAssoc* a=GetAssocAt(key,hash,bucket);if(!a)return FALSE;value=a->value;return TRUE;
}
void CMapWordToOb::InitHashTable(UINT size,BOOL allocate) {
    if(!size || m_nCount)impl__AfxThrowInvalidArgException__YAXXZ();
    delete[] m_pHashTable;m_pHashTable=nullptr;m_nHashTableSize=size;
    if(allocate) {m_pHashTable=new(std::nothrow) CAssoc*[size]{};if(!m_pHashTable)impl__AfxThrowMemoryException__YAXXZ();}
}
CMapWordToOb::CAssoc* CMapWordToOb::NewAssoc() {
    if(!m_pFreeList) {
        if(static_cast<UINT_PTR>(m_nBlockSize)>(SIZE_MAX-sizeof(CPlex))/sizeof(CAssoc))impl__AfxThrowMemoryException__YAXXZ();
        auto* bytes=new(std::nothrow) BYTE[sizeof(CPlex)+m_nBlockSize*sizeof(CAssoc)];
        if(!bytes)impl__AfxThrowMemoryException__YAXXZ();
        auto* block=reinterpret_cast<CPlex*>(bytes);block->pNext=m_pBlocks;m_pBlocks=block;
        auto* assocs=static_cast<CAssoc*>(block->data());
        for(INT_PTR i=m_nBlockSize;i>0;--i) {CAssoc* a=assocs+i-1;a->pNext=m_pFreeList;m_pFreeList=a;}
    }
    CAssoc* a=m_pFreeList;m_pFreeList=a->pNext;a->key=0;a->value=nullptr;++m_nCount;return a;
}
void CMapWordToOb::FreeAssoc(CAssoc* a) {
    a->pNext=m_pFreeList;m_pFreeList=a;--m_nCount;if(!m_nCount)RemoveAll();
}
CObject*& CMapWordToOb::operator[](WORD key) {
    UINT hash,bucket;CAssoc* a=GetAssocAt(key,hash,bucket);
    if(!a) {
        if(!m_pHashTable)InitHashTable(m_nHashTableSize);
        a=NewAssoc();a->key=key;a->pNext=m_pHashTable[bucket];m_pHashTable[bucket]=a;
    }
    return a->value;
}
CObject* const& CMapWordToOb::operator[](WORD key)const {return const_cast<CMapWordToOb*>(this)->operator[](key);}
BOOL CMapWordToOb::RemoveKey(WORD key) {
    if(!m_pHashTable)return FALSE;
    CAssoc** link=m_pHashTable+HashKey(key)%m_nHashTableSize;
    while(*link) {CAssoc* a=*link;if(a->key==key) {*link=a->pNext;FreeAssoc(a);return TRUE;}link=&a->pNext;}
    return FALSE;
}
void CMapWordToOb::RemoveAll() {
    delete[] m_pHashTable;m_pHashTable=nullptr;m_nCount=0;m_pFreeList=nullptr;
    while(m_pBlocks) {CPlex* next=m_pBlocks->pNext;delete[] reinterpret_cast<BYTE*>(m_pBlocks);m_pBlocks=next;}
}
CMapWordToOb::POSITION CMapWordToOb::GetStartPosition()const {return m_nCount ? reinterpret_cast<POSITION>(static_cast<INT_PTR>(-1)) : nullptr;}
void CMapWordToOb::GetNextAssoc(POSITION& pos,WORD& key,CObject*& value)const {
    CAssoc* a=reinterpret_cast<CAssoc*>(pos);UINT bucket=0;
    if(pos==reinterpret_cast<POSITION>(static_cast<INT_PTR>(-1))) {
        for(;bucket<m_nHashTableSize;++bucket)if(m_pHashTable[bucket]) {a=m_pHashTable[bucket];break;}
    } else bucket=HashKey(a->key)%m_nHashTableSize;
    key=a->key;value=a->value;
    CAssoc* next=a->pNext;
    if(!next)for(++bucket;bucket<m_nHashTableSize;++bucket)if(m_pHashTable[bucket]) {next=m_pHashTable[bucket];break;}
    pos=reinterpret_cast<POSITION>(next);
}
void CMapWordToOb::Serialize(CArchive& ar) {
    if(ar.IsStoring()) {
        openmfc::detail::WriteCollectionCount(ar,m_nCount);POSITION pos=GetStartPosition();
        while(pos) {WORD key;CObject* value;GetNextAssoc(pos,key,value);ar<<key<<value;}
    } else {
        UINT64 count=openmfc::detail::ReadCollectionCount(ar);
        for(UINT64 i=0;i<count;++i) {WORD key;CObject* value;ar>>key>>value;SetAt(key,value);}
    }
}
static_assert(sizeof(CMapWordToOb)==56 && sizeof(CMapWordToOb::CAssoc)==24 && sizeof(CMapWordToOb::POSITION)==8,"native word/object map layout");
static_assert(offsetof(CMapWordToOb::CAssoc,value)==8 && offsetof(CMapWordToOb::CAssoc,key)==16,"native association layout");
