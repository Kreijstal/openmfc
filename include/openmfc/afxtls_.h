#pragma once
#include "openmfc/afx.h"

// Intrusive lists have no allocation or virtual table. The link's byte offset
// is part of each list, so the same object can participate in different lists.
class CSimpleList {
public:
    CSimpleList(int offset=0) : m_pHead(nullptr),m_nNextOffset(offset) {}
    void Construct(int offset) {m_nNextOffset=offset;}
    BOOL IsEmpty() const {return m_pHead==nullptr;}
    void AddHead(void* object);
    BOOL Remove(void* object);
    void RemoveAll() {m_pHead=nullptr;}
    void* GetHead() const {return m_pHead;}
    void* GetNext(void* object) const {return *GetNextPtr(object);}
    void** GetNextPtr(void* object) const {
        if(!object)AfxThrowInvalidArgException();
        return reinterpret_cast<void**>(static_cast<BYTE*>(object)+m_nNextOffset);
    }
    int GetCount() const {
        int result=0;
        for(auto* object=m_pHead;object;object=GetNext(object))++result;
        return result;
    }
    void* m_pHead;
    size_t m_nNextOffset;
};
static_assert(sizeof(CSimpleList)==16 && offsetof(CSimpleList,m_nNextOffset)==8,"native intrusive list");

template<class Pointer> class CTypedSimpleList : public CSimpleList {
public:
    CTypedSimpleList(int offset=0) : CSimpleList(offset) {}
    void AddHead(Pointer object) {CSimpleList::AddHead(object);}
    BOOL Remove(Pointer object) {return CSimpleList::Remove(object);}
    Pointer GetHead() {return static_cast<Pointer>(CSimpleList::GetHead());}
    Pointer GetNext(Pointer object) {return static_cast<Pointer>(CSimpleList::GetNext(object));}
    operator Pointer() {return GetHead();}
};
