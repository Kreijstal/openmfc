// Native CPtrArray owns its pointer buffer in the public 40-byte member block.
#define OPENMFC_APPCORE_IMPL
#include "openmfc/afx.h"
#include <cstdlib>
#include <cstring>

extern "C" void* impl___2_YAPEAX_K_Z(size_t);
static_assert(sizeof(CPtrArray)==40,"native pointer array representation");
IMPLEMENT_DYNAMIC(CPtrArray,CObject)
namespace {
INT_PTR sum(INT_PTR a,INT_PTR b) {return INT_PTR(UINT_PTR(a)+UINT_PTR(b));}
size_t bytes(INT_PTR count) {return size_t(count)*sizeof(void*);}
void** allocate(INT_PTR count) {return static_cast<void**>(impl___2_YAPEAX_K_Z(bytes(count)));}
void copy(void** to,INT_PTR capacity,void* const* from,INT_PTR count) {
    if(memcpy_s(to,bytes(capacity),from,bytes(count)))AfxThrowInvalidArgException();
}
}
CPtrArray::CPtrArray():m_pData(nullptr),m_nSize(0),m_nMaxSize(0),m_nGrowBy(0) {}
CPtrArray::~CPtrArray() {free(m_pData);}
void CPtrArray::SetSize(INT_PTR size,INT_PTR grow) {
    if(size<0)AfxThrowInvalidArgException();
    if(grow>=0)m_nGrowBy=grow;
    if(!size) {free(m_pData);m_pData=nullptr;m_nSize=m_nMaxSize=0;return;}
    if(!m_pData) {
        m_pData=allocate(size);memset(m_pData,0,bytes(size));m_nMaxSize=m_nSize=size;return;
    }
    if(size<=m_nMaxSize) {
        if(size>m_nSize)memset(m_pData+m_nSize,0,bytes(size-m_nSize));
        m_nSize=size;return;
    }
    INT_PTR extra=m_nGrowBy;
    if(!extra){extra=m_nSize/8;if(extra<4)extra=4;if(extra>1024)extra=1024;}
    INT_PTR capacity=sum(m_nMaxSize,extra);
    if(size>=capacity)capacity=size;
    else if(capacity<m_nMaxSize)AfxThrowInvalidArgException();
    void** data=allocate(capacity);copy(data,capacity,m_pData,m_nSize);
    memset(data+m_nSize,0,bytes(size-m_nSize));
    free(m_pData);m_pData=data;m_nSize=size;m_nMaxSize=capacity;
}
void CPtrArray::FreeExtra() {
    if(m_nSize==m_nMaxSize)return;
    void** data=nullptr;
    if(m_nSize){data=allocate(m_nSize);copy(data,m_nSize,m_pData,m_nSize);}
    free(m_pData);m_nMaxSize=m_nSize;m_pData=data;
}
void CPtrArray::SetAtGrow(INT_PTR index,void* value) {
    if(index<0)AfxThrowInvalidArgException();
    if(index>=m_nSize)SetSize(sum(index,1));
    m_pData[index]=value;
}
INT_PTR CPtrArray::Append(const CPtrArray& source) {
    if(this==&source)AfxThrowInvalidArgException();
    INT_PTR old=m_nSize;SetSize(sum(old,source.m_nSize));
    copy(m_pData+old,source.m_nSize,source.m_pData,source.m_nSize);return old;
}
void CPtrArray::Copy(const CPtrArray& source) {
    if(this==&source)return;
    SetSize(source.m_nSize);copy(m_pData,source.m_nSize,source.m_pData,source.m_nSize);
}
void CPtrArray::InsertAt(INT_PTR index,void* value,INT_PTR count) {
    if(index<0||count<=0)AfxThrowInvalidArgException();
    if(index>=m_nSize)SetSize(sum(index,count));
    else {
        INT_PTR old=m_nSize;SetSize(sum(old,count));
        if(memmove_s(m_pData+sum(index,count),bytes(m_nSize-index-count),m_pData+index,bytes(old-index)))AfxThrowInvalidArgException();
        memset(m_pData+index,0,bytes(count));
    }
    for(INT_PTR at=0;at<count;++at)m_pData[index+at]=value;
}
void CPtrArray::RemoveAt(INT_PTR index,INT_PTR count) {
    INT_PTR end=sum(index,count);
    if(index<0||count<0||end>m_nSize||end<index||end<count)AfxThrowInvalidArgException();
    INT_PTR remaining=m_nSize-end;
    if(remaining&&memmove_s(m_pData+index,bytes(remaining),m_pData+end,bytes(remaining)))AfxThrowInvalidArgException();
    m_nSize-=count;
}
void CPtrArray::InsertAt(INT_PTR index,CPtrArray* source) {
    if(!source||index<0)AfxThrowInvalidArgException();
    if(source->GetSize()>0) {
        InsertAt(index,source->GetAt(0),source->GetSize());
        for(INT_PTR at=0;at<source->GetSize();++at)SetAt(sum(index,at),source->GetAt(at));
    }
}
