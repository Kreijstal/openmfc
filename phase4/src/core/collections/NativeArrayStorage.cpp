// Native primitive array storage; client inline accessors read these fields.
#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxwin.h"
#include "detail/NativeCollectionArchive.h"
#include <algorithm>
#include <cstring>
#include <limits>
#include <new>
#ifdef __GNUC__
#define MS_ABI __attribute__((ms_abi))
#else
#define MS_ABI
#endif
extern "C" void MS_ABI impl__AfxThrowMemoryException__YAXXZ();
extern "C" void MS_ABI impl__AfxThrowInvalidArgException__YAXXZ();
namespace {
void Require(bool valid) {if(!valid)impl__AfxThrowInvalidArgException__YAXXZ();}
template<class T> T* Allocate(INT_PTR count) {
    if(count<0 || static_cast<size_t>(count)>SIZE_MAX/sizeof(T))impl__AfxThrowMemoryException__YAXXZ();
    auto* bytes=new(std::nothrow) BYTE[static_cast<size_t>(count)*sizeof(T)];
    if(!bytes)impl__AfxThrowMemoryException__YAXXZ();
    return reinterpret_cast<T*>(bytes);
}
template<class Array> void Resize(Array* a,INT_PTR size,INT_PTR grow) {
    Require(size>=0);if(grow>=0)a->m_nGrowBy=grow;
    if(!size) {delete[] reinterpret_cast<BYTE*>(a->m_pData);a->m_pData=nullptr;a->m_nSize=a->m_nMaxSize=0;return;}
    using T=std::remove_pointer_t<decltype(a->m_pData)>;
    if(size>a->m_nMaxSize) {
        INT_PTR capacity=size;
        if(a->m_pData) {
            INT_PTR step=a->m_nGrowBy ? a->m_nGrowBy : std::min<INT_PTR>(1024,std::max<INT_PTR>(4,a->m_nSize/8));
            if(step<=PTRDIFF_MAX-a->m_nMaxSize)capacity=std::max(size,a->m_nMaxSize+step);
        }
        T* replacement=Allocate<T>(capacity);
        if(a->m_nSize)std::memcpy(replacement,a->m_pData,a->m_nSize*sizeof(T));
        delete[] reinterpret_cast<BYTE*>(a->m_pData);a->m_pData=replacement;a->m_nMaxSize=capacity;
    }
    if(size>a->m_nSize)std::memset(a->m_pData+a->m_nSize,0,(size-a->m_nSize)*sizeof(T));
    a->m_nSize=size;
}
template<class Array> void Compact(Array* a) {
    if(a->m_nSize==a->m_nMaxSize)return;
    using T=std::remove_pointer_t<decltype(a->m_pData)>;
    T* replacement=a->m_nSize ? Allocate<T>(a->m_nSize) : nullptr;
    if(a->m_nSize)std::memcpy(replacement,a->m_pData,a->m_nSize*sizeof(T));
    delete[] reinterpret_cast<BYTE*>(a->m_pData);a->m_pData=replacement;a->m_nMaxSize=a->m_nSize;
}

}
IMPLEMENT_SERIAL(CByteArray,CObject,0xffff)
IMPLEMENT_SERIAL(CObArray,CObject,0xffff)
#define ARRAY_METHODS(Class,Type) \
Class::Class():m_pData(nullptr),m_nSize(0),m_nMaxSize(0),m_nGrowBy(0){} \
Class::~Class(){delete[] reinterpret_cast<BYTE*>(m_pData);} \
INT_PTR Class::GetSize()const{return m_nSize;} INT_PTR Class::GetCount()const{return m_nSize;} \
BOOL Class::IsEmpty()const{return !m_nSize;} INT_PTR Class::GetUpperBound()const{return m_nSize-1;} \
void Class::SetSize(INT_PTR size,INT_PTR grow){Resize(this,size,grow);} \
void Class::FreeExtra(){Compact(this);} void Class::RemoveAll(){SetSize(0);} \
Type Class::GetAt(INT_PTR index)const{Require(index>=0 && index<m_nSize);return m_pData[index];} \
void Class::SetAt(INT_PTR index,Type value){Require(index>=0 && index<m_nSize);m_pData[index]=value;} \
Type& Class::ElementAt(INT_PTR index){Require(index>=0 && index<m_nSize);return m_pData[index];} \
const Type& Class::ElementAt(INT_PTR index)const{Require(index>=0 && index<m_nSize);return m_pData[index];} \
Type Class::operator[](INT_PTR index)const{return GetAt(index);} Type& Class::operator[](INT_PTR index){return ElementAt(index);} \
Type* Class::GetData(){return m_pData;} \
void Class::SetAtGrow(INT_PTR index,Type value){Require(index>=0 && index<PTRDIFF_MAX);if(index>=m_nSize)SetSize(index+1);m_pData[index]=value;} \
INT_PTR Class::Add(Type value){INT_PTR index=m_nSize;SetAtGrow(index,value);return index;} \
INT_PTR Class::Append(const Class& src){INT_PTR index=m_nSize,count=src.m_nSize;Require(count<=PTRDIFF_MAX-index);SetSize(index+count);if(count)std::memmove(m_pData+index,src.m_pData,count*sizeof(Type));return index;} \
void Class::Copy(const Class& src){if(this==&src)return;SetSize(src.m_nSize);if(m_nSize)std::memcpy(m_pData,src.m_pData,m_nSize*sizeof(Type));} \
void Class::InsertAt(INT_PTR index,Type value,INT_PTR count){Require(index>=0 && count>0 && count<=PTRDIFF_MAX-std::max(index,m_nSize));INT_PTR old=m_nSize;SetSize(std::max(index,old)+count);if(index<old)std::memmove(m_pData+index+count,m_pData+index,(old-index)*sizeof(Type));std::fill_n(m_pData+index,count,value);} \
void Class::RemoveAt(INT_PTR index,INT_PTR count){Require(index>=0 && count>=0 && index<=m_nSize && count<=m_nSize-index);if(count)std::memmove(m_pData+index,m_pData+index+count,(m_nSize-index-count)*sizeof(Type));m_nSize-=count;} \
void Class::InsertAt(INT_PTR index,Class* src){Require(src && src!=this);if(!src->m_nSize)return;InsertAt(index,Type(),src->m_nSize);std::memcpy(m_pData+index,src->m_pData,src->m_nSize*sizeof(Type));}
ARRAY_METHODS(CByteArray,BYTE)
// A typedef keeps const qualifiers on the pointer itself in the shared macro.
using ObjectPtr=CObject*;
ARRAY_METHODS(CObArray,ObjectPtr)
#undef ARRAY_METHODS
const BYTE* CByteArray::GetData()const{return m_pData;}
const CObject** CObArray::GetData()const{return (const CObject**)m_pData;}
void CByteArray::Serialize(CArchive& ar) {
    if(ar.IsStoring())openmfc::detail::WriteCollectionCount(ar,m_nSize);
    else {UINT64 size=openmfc::detail::ReadCollectionCount(ar);if(size>PTRDIFF_MAX)impl__AfxThrowMemoryException__YAXXZ();SetSize(static_cast<INT_PTR>(size));}
    for(INT_PTR offset=0;offset<m_nSize;) {
        UINT bytes=static_cast<UINT>(std::min<UINT64>(m_nSize-offset,UINT_MAX));
        if(ar.IsStoring())ar.Write(m_pData+offset,bytes);else ar.Read(m_pData+offset,bytes);offset+=bytes;
    }
}
void CObArray::Serialize(CArchive& ar) {
    if(ar.IsStoring()){openmfc::detail::WriteCollectionCount(ar,m_nSize);for(INT_PTR i=0;i<m_nSize;++i)ar<<m_pData[i];}
    else {UINT64 size=openmfc::detail::ReadCollectionCount(ar);if(size>PTRDIFF_MAX)impl__AfxThrowMemoryException__YAXXZ();SetSize(size);for(INT_PTR i=0;i<m_nSize;++i)ar>>m_pData[i];}
}
static_assert(sizeof(CByteArray)==40 && sizeof(CObArray)==40,"native primitive arrays");
