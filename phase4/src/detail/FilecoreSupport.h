#pragma once
// Shared internals of the former filecore.cpp translation unit.
// Definitions live in detail/FilecoreSupport.cpp, so file-scope state keeps a
// single instance across the classes this unit was split into.
namespace openmfc { namespace detail { namespace filecore {} } }
using namespace openmfc::detail::filecore;
// File and Archive Core Implementation
//
// Implements CFile, CMemFile, CStdioFile, and CArchive for serialization.

#define OPENMFC_APPCORE_IMPL
#include "openmfc/afx.h"
#include "detail/AtlStrTCore.h"
#include <windows.h>
#include <algorithm>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cerrno>
#include <cstdint>
#include <io.h>
#include <limits>
#include <memory>
#include <mutex>
#include <type_traits>
#include <unordered_map>
#include <vector>

// MS ABI calling convention
#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

// Internal polymorphic dispatch on CFile-family objects via their MSVC-layout
// vtable (global_file_dispatch.cpp). Internal code must NOT use pFile->Method()
// virtual syntax — mingw indexes the MSVC vtable with Itanium slots and mis-dispatches.
extern "C" {

unsigned int        MS_ABI OpenMFC_File_Read(CFile*, void*, unsigned int);
void                MS_ABI OpenMFC_File_Write(CFile*, const void*, unsigned int);
unsigned long long  MS_ABI OpenMFC_File_Seek(CFile*, long long, unsigned int);
unsigned long long  MS_ABI OpenMFC_File_GetLength(CFile*);
void                MS_ABI OpenMFC_File_Flush(CFile*);
}

extern "C" CRuntimeClass* MS_ABI impl__Load_CRuntimeClass__SAPEAU1_AEAVCArchive__PEAI_Z(
    CArchive* ar,
    unsigned int* pwSchemaNum
);
extern "C" void MS_ABI impl__Store_CRuntimeClass__QEBAXAEAVCArchive___Z(
    const CRuntimeClass* pThis,
    CArchive* ar
);


namespace openmfc { namespace detail { namespace filecore {
extern std::mutex g_collectionStateMutex;
} } }

namespace openmfc { namespace detail { namespace filecore {
template<typename Wrapper, typename State>
using CollectionStateMap = std::unordered_map<const Wrapper*, State>;
} } }

namespace openmfc { namespace detail { namespace filecore {
template<typename Wrapper, typename State>
CollectionStateMap<Wrapper, State>& GetCollectionStates() {
    static CollectionStateMap<Wrapper, State> states;
    return states;
}
} } }

namespace openmfc { namespace detail { namespace filecore {
template<typename Wrapper, typename State, typename... Args>
State& EnsureCollectionState(const Wrapper* self, Args&&... args) {
    auto& states = GetCollectionStates<Wrapper, State>();
    auto it = states.find(self);
    if (it == states.end()) {
        it = states.emplace(self, State(std::forward<Args>(args)...)).first;
    }
    return it->second;
}
} } }

namespace openmfc { namespace detail { namespace filecore {
template<typename Wrapper, typename State>
State* FindCollectionState(const Wrapper* self) {
    auto& states = GetCollectionStates<Wrapper, State>();
    auto it = states.find(self);
    return it == states.end() ? nullptr : &it->second;
}
} } }

namespace openmfc { namespace detail { namespace filecore {
template<typename Wrapper, typename State>
void RemoveCollectionState(const Wrapper* self) {
    GetCollectionStates<Wrapper, State>().erase(self);
}
} } }

namespace openmfc { namespace detail { namespace filecore {
struct CMemFileLockState {
    std::vector<std::pair<unsigned long long, unsigned long long>> ranges;
};
} } }

namespace openmfc { namespace detail { namespace filecore {
unsigned long long EndOffset(unsigned long long pos, unsigned long long count);
} } }

namespace openmfc { namespace detail { namespace filecore {
bool RangesOverlap(const std::pair<unsigned long long, unsigned long long>& lhs,
                         const std::pair<unsigned long long, unsigned long long>& rhs);
} } }

namespace openmfc { namespace detail { namespace filecore {
void AddMemFileRange(CMemFileLockState& state, unsigned long long pos, unsigned long long count);
} } }

namespace openmfc { namespace detail { namespace filecore {
void RemoveMemFileRange(CMemFileLockState& state, unsigned long long pos, unsigned long long count);
} } }

namespace openmfc { namespace detail { namespace filecore {
template<typename TYPE, typename ARG_TYPE>
struct ArrayWrapperState {
    CArray<TYPE, ARG_TYPE> data;
};
} } }

namespace openmfc { namespace detail { namespace filecore {
template<typename TYPE, typename ARG_TYPE>
struct ListWrapperState {
    explicit ListWrapperState(int nBlockSize = 10) : data(nBlockSize) {}
    CList<TYPE, ARG_TYPE> data;
};
} } }

namespace openmfc { namespace detail { namespace filecore {
template<typename KEY, typename ARG_KEY, typename VALUE, typename ARG_VALUE>
struct MapWrapperState {
    explicit MapWrapperState(int nBlockSize = 10) : data(nBlockSize) {}
    CMap<KEY, ARG_KEY, VALUE, ARG_VALUE> data;
};
} } }

namespace openmfc { namespace detail { namespace filecore {
template<typename VALUE, typename ARG_VALUE>
struct CStringKeyMapState {
    explicit CStringKeyMapState(int nBlockSize = 10) : data(nBlockSize) {}
    CMap<CString, const CString&, VALUE, ARG_VALUE> data;
    CString lookupKeyScratch;
};
} } }

namespace openmfc { namespace detail { namespace filecore {
int ClampCollectionSize(INT_PTR value);
} } }

namespace openmfc { namespace detail { namespace filecore {
int ClampCollectionGrow(INT_PTR value);
} } }

namespace openmfc { namespace detail { namespace filecore {
int ClampCollectionBlockSize(INT_PTR value);
} } }

namespace openmfc { namespace detail { namespace filecore {
CString NormalizeStringKey(const wchar_t* key);
} } }

namespace openmfc { namespace detail { namespace filecore {
template<typename T>
const T& CollectionConstRef(T& value) {
    return value;
}
} } }

namespace openmfc { namespace detail { namespace filecore {
template<typename T>
const T*& CollectionConstRef(T*& value) {
    static thread_local const T* snapshot = nullptr;
    snapshot = value;
    return snapshot;
}
} } }

namespace openmfc { namespace detail { namespace filecore {
template<typename T>
const T* CollectionConstData(T* data) {
    return data;
}
} } }

namespace openmfc { namespace detail { namespace filecore {
template<typename T>
const T** CollectionConstData(T** data) {
    return (const T**)data;
}
} } }

namespace openmfc { namespace detail { namespace filecore {
UINT HashCStringKey(const CString& key);
} } }

namespace openmfc { namespace detail { namespace filecore {
struct CStringMapStringState {
    explicit CStringMapStringState(int nBlockSize = 10) : data(nBlockSize) {}

    void MarkDirty() { pairsDirty = true; }

    void EnsurePairs() {
        if (!pairsDirty) {
            return;
        }
        pairs.clear();
        CMapStringToString::CPair* previous = nullptr;
        auto pos = data.GetStartPosition();
        while (pos != CMapStringToString::POSITION(nullptr, 0)) {
            CString key;
            CString value;
            data.GetNextAssoc(pos, key, value);
            auto pair = std::make_unique<CMapStringToString::CPair>();
            pair->pNextAssoc = nullptr;
            pair->nHashValue = HashCStringKey(key);
            pair->key = key;
            pair->value = value;
            CMapStringToString::CPair* raw = pair.get();
            if (previous) {
                previous->pNextAssoc = raw;
            }
            pairs.push_back(std::move(pair));
            previous = raw;
        }
        pairsDirty = false;
    }

    CMapStringToString::CPair* PLookup(const wchar_t* key) {
        EnsurePairs();
        CString lookup = NormalizeStringKey(key);
        for (const auto& pair : pairs) {
            if (pair->key == lookup) {
                return pair.get();
            }
        }
        return nullptr;
    }

    const CMapStringToString::CPair* PLookup(const wchar_t* key) const {
        return const_cast<CStringMapStringState*>(this)->PLookup(key);
    }

    CMap<CString, const CString&, CString, const CString&> data;
    CString lookupKeyScratch;
    std::vector<std::unique_ptr<CMapStringToString::CPair>> pairs;
    bool pairsDirty = true;
};
} } }

namespace openmfc { namespace detail { namespace filecore {
template<typename Wrapper, typename TYPE, typename ARG_TYPE>
static CArray<TYPE, ARG_TYPE>& EnsureArrayStorage(const Wrapper* self) {
    return EnsureCollectionState<Wrapper, ArrayWrapperState<TYPE, ARG_TYPE>>(self).data;
}
} } }

namespace openmfc { namespace detail { namespace filecore {
template<typename Wrapper, typename TYPE, typename ARG_TYPE>
static const CArray<TYPE, ARG_TYPE>* FindArrayStorage(const Wrapper* self) {
    const auto* state = FindCollectionState<Wrapper, ArrayWrapperState<TYPE, ARG_TYPE>>(self);
    return state ? &state->data : nullptr;
}
} } }

namespace openmfc { namespace detail { namespace filecore {
template<typename Wrapper, typename TYPE, typename ARG_TYPE>
static CList<TYPE, ARG_TYPE>& EnsureListStorage(const Wrapper* self, int nBlockSize = 10) {
    return EnsureCollectionState<Wrapper, ListWrapperState<TYPE, ARG_TYPE>>(self, ClampCollectionBlockSize(nBlockSize)).data;
}
} } }

namespace openmfc { namespace detail { namespace filecore {
template<typename Wrapper, typename TYPE, typename ARG_TYPE>
static const CList<TYPE, ARG_TYPE>* FindListStorage(const Wrapper* self) {
    const auto* state = FindCollectionState<Wrapper, ListWrapperState<TYPE, ARG_TYPE>>(self);
    return state ? &state->data : nullptr;
}
} } }

namespace openmfc { namespace detail { namespace filecore {
template<typename Wrapper, typename KEY, typename ARG_KEY, typename VALUE, typename ARG_VALUE>
static CMap<KEY, ARG_KEY, VALUE, ARG_VALUE>& EnsureMapStorage(const Wrapper* self, int nBlockSize = 10) {
    return EnsureCollectionState<Wrapper, MapWrapperState<KEY, ARG_KEY, VALUE, ARG_VALUE>>(self, ClampCollectionBlockSize(nBlockSize)).data;
}
} } }

namespace openmfc { namespace detail { namespace filecore {
template<typename Wrapper, typename KEY, typename ARG_KEY, typename VALUE, typename ARG_VALUE>
static const CMap<KEY, ARG_KEY, VALUE, ARG_VALUE>* FindMapStorage(const Wrapper* self) {
    const auto* state = FindCollectionState<Wrapper, MapWrapperState<KEY, ARG_KEY, VALUE, ARG_VALUE>>(self);
    return state ? &state->data : nullptr;
}
} } }

namespace openmfc { namespace detail { namespace filecore {
template<typename Wrapper, typename VALUE, typename ARG_VALUE>
static CStringKeyMapState<VALUE, ARG_VALUE>& EnsureCStringKeyMapStorage(const Wrapper* self, int nBlockSize = 10) {
    return EnsureCollectionState<Wrapper, CStringKeyMapState<VALUE, ARG_VALUE>>(self, ClampCollectionBlockSize(nBlockSize));
}
} } }

namespace openmfc { namespace detail { namespace filecore {
template<typename Wrapper, typename VALUE, typename ARG_VALUE>
static const CStringKeyMapState<VALUE, ARG_VALUE>* FindCStringKeyMapStorage(const Wrapper* self) {
    return FindCollectionState<Wrapper, CStringKeyMapState<VALUE, ARG_VALUE>>(self);
}
} } }

namespace openmfc { namespace detail { namespace filecore {
CStringMapStringState& EnsureCStringMapStringStorage(const CMapStringToString* self, int nBlockSize = 10);
} } }

namespace openmfc { namespace detail { namespace filecore {
const CStringMapStringState* FindCStringMapStringStorage(const CMapStringToString* self);
} } }

namespace openmfc { namespace detail { namespace filecore {
template<typename KEY, typename VALUE>
struct AssocSnapshot {
    AssocSnapshot* pNext = nullptr;
    UINT nHashValue = 0;
    KEY key{};
    VALUE value{};
};
} } }

namespace openmfc { namespace detail { namespace filecore {
template<typename VALUE>
struct ListNodeSnapshot {
    ListNodeSnapshot* pNext = nullptr;
    ListNodeSnapshot* pPrev = nullptr;
    VALUE data{};
};
} } }



namespace openmfc { namespace detail { namespace filecore {
extern __attribute__((used)) CRuntimeClass g_classCWordArray;
} } }

#define OPENMFC_DEFINE_ARRAY_METHODS(class_name, element_type, arg_type) \
class_name::class_name() { \
    std::lock_guard<std::mutex> lock(g_collectionStateMutex); \
    EnsureCollectionState<class_name, ArrayWrapperState<element_type, arg_type>>(this); \
} \
 \
class_name::~class_name() { \
    std::lock_guard<std::mutex> lock(g_collectionStateMutex); \
    RemoveCollectionState<class_name, ArrayWrapperState<element_type, arg_type>>(this); \
} \
 \
INT_PTR class_name::GetSize() const { \
    std::lock_guard<std::mutex> lock(g_collectionStateMutex); \
    const auto* data = FindArrayStorage<class_name, element_type, arg_type>(this); \
    return data ? data->GetSize() : 0; \
} \
 \
INT_PTR class_name::GetCount() const { \
    return GetSize(); \
} \
 \
BOOL class_name::IsEmpty() const { \
    return GetSize() == 0; \
} \
 \
INT_PTR class_name::GetUpperBound() const { \
    return GetSize() - 1; \
} \
 \
void class_name::SetSize(INT_PTR nNewSize, INT_PTR nGrowBy) { \
    std::lock_guard<std::mutex> lock(g_collectionStateMutex); \
    EnsureArrayStorage<class_name, element_type, arg_type>(this).SetSize(ClampCollectionSize(nNewSize), ClampCollectionGrow(nGrowBy)); \
} \
 \
void class_name::FreeExtra() { \
    std::lock_guard<std::mutex> lock(g_collectionStateMutex); \
    EnsureArrayStorage<class_name, element_type, arg_type>(this).FreeExtra(); \
} \
 \
void class_name::RemoveAll() { \
    std::lock_guard<std::mutex> lock(g_collectionStateMutex); \
    EnsureArrayStorage<class_name, element_type, arg_type>(this).RemoveAll(); \
} \
 \
element_type class_name::GetAt(INT_PTR nIndex) const { \
    std::lock_guard<std::mutex> lock(g_collectionStateMutex); \
    const auto& data = EnsureArrayStorage<class_name, element_type, arg_type>(this); \
    return data.GetAt(ClampCollectionSize(nIndex)); \
} \
 \
void class_name::SetAt(INT_PTR nIndex, arg_type newElement) { \
    std::lock_guard<std::mutex> lock(g_collectionStateMutex); \
    EnsureArrayStorage<class_name, element_type, arg_type>(this).SetAt(ClampCollectionSize(nIndex), newElement); \
} \
 \
element_type& class_name::ElementAt(INT_PTR nIndex) { \
    std::lock_guard<std::mutex> lock(g_collectionStateMutex); \
    return EnsureArrayStorage<class_name, element_type, arg_type>(this).ElementAt(ClampCollectionSize(nIndex)); \
} \
 \
const element_type& class_name::ElementAt(INT_PTR nIndex) const { \
    std::lock_guard<std::mutex> lock(g_collectionStateMutex); \
    return CollectionConstRef(EnsureArrayStorage<class_name, element_type, arg_type>(this).ElementAt(ClampCollectionSize(nIndex))); \
} \
 \
element_type class_name::operator[](INT_PTR nIndex) const { \
    return GetAt(nIndex); \
} \
 \
element_type& class_name::operator[](INT_PTR nIndex) { \
    return ElementAt(nIndex); \
} \
 \
element_type* class_name::GetData() { \
    std::lock_guard<std::mutex> lock(g_collectionStateMutex); \
    return EnsureArrayStorage<class_name, element_type, arg_type>(this).GetData(); \
} \
 \
const element_type* class_name::GetData() const { \
    std::lock_guard<std::mutex> lock(g_collectionStateMutex); \
    return CollectionConstData(EnsureArrayStorage<class_name, element_type, arg_type>(this).GetData()); \
} \
 \
void class_name::SetAtGrow(INT_PTR nIndex, arg_type newElement) { \
    std::lock_guard<std::mutex> lock(g_collectionStateMutex); \
    EnsureArrayStorage<class_name, element_type, arg_type>(this).SetAtGrow(ClampCollectionSize(nIndex), newElement); \
} \
 \
INT_PTR class_name::Add(arg_type newElement) { \
    std::lock_guard<std::mutex> lock(g_collectionStateMutex); \
    return EnsureArrayStorage<class_name, element_type, arg_type>(this).Add(newElement); \
} \
 \
INT_PTR class_name::Append(const class_name& src) { \
    std::lock_guard<std::mutex> lock(g_collectionStateMutex); \
    auto& data = EnsureArrayStorage<class_name, element_type, arg_type>(this); \
    const auto& srcData = EnsureArrayStorage<class_name, element_type, arg_type>(&src); \
    return data.Append(srcData); \
} \
 \
void class_name::Copy(const class_name& src) { \
    std::lock_guard<std::mutex> lock(g_collectionStateMutex); \
    auto& data = EnsureArrayStorage<class_name, element_type, arg_type>(this); \
    const auto& srcData = EnsureArrayStorage<class_name, element_type, arg_type>(&src); \
    data.Copy(srcData); \
} \
 \
void class_name::InsertAt(INT_PTR nIndex, arg_type newElement, INT_PTR nCount) { \
    std::lock_guard<std::mutex> lock(g_collectionStateMutex); \
    EnsureArrayStorage<class_name, element_type, arg_type>(this).InsertAt(ClampCollectionSize(nIndex), newElement, ClampCollectionSize(nCount)); \
} \
 \
void class_name::RemoveAt(INT_PTR nIndex, INT_PTR nCount) { \
    std::lock_guard<std::mutex> lock(g_collectionStateMutex); \
    EnsureArrayStorage<class_name, element_type, arg_type>(this).RemoveAt(ClampCollectionSize(nIndex), ClampCollectionSize(nCount)); \
} \
 \
void class_name::InsertAt(INT_PTR nStartIndex, class_name* pNewArray) { \
    if (!pNewArray) return; \
    std::lock_guard<std::mutex> lock(g_collectionStateMutex); \
    auto& data = EnsureArrayStorage<class_name, element_type, arg_type>(this); \
    auto& srcData = EnsureArrayStorage<class_name, element_type, arg_type>(pNewArray); \
    data.InsertAt(ClampCollectionSize(nStartIndex), &srcData); \
} \
 \
void class_name::Serialize(CArchive& ar) { \
    std::lock_guard<std::mutex> lock(g_collectionStateMutex); \
    auto& data = EnsureArrayStorage<class_name, element_type, arg_type>(this); \
    if (ar.IsStoring()) ar << data; else ar >> data; \
}





#define OPENMFC_DEFINE_LIST_METHODS(class_name, element_type, arg_type) \
class_name::class_name(INT_PTR nBlockSize) { \
    std::lock_guard<std::mutex> lock(g_collectionStateMutex); \
    EnsureCollectionState<class_name, ListWrapperState<element_type, arg_type>>(this, ClampCollectionBlockSize(nBlockSize)); \
} \
 \
class_name::~class_name() { \
    std::lock_guard<std::mutex> lock(g_collectionStateMutex); \
    RemoveCollectionState<class_name, ListWrapperState<element_type, arg_type>>(this); \
} \
 \
INT_PTR class_name::GetCount() const { \
    std::lock_guard<std::mutex> lock(g_collectionStateMutex); \
    const auto* data = FindListStorage<class_name, element_type, arg_type>(this); \
    return data ? data->GetCount() : 0; \
} \
 \
BOOL class_name::IsEmpty() const { return GetCount() == 0; } \
element_type& class_name::GetHead() { std::lock_guard<std::mutex> lock(g_collectionStateMutex); return EnsureListStorage<class_name, element_type, arg_type>(this).GetHead(); } \
element_type class_name::GetHead() const { std::lock_guard<std::mutex> lock(g_collectionStateMutex); return EnsureListStorage<class_name, element_type, arg_type>(this).GetHead(); } \
element_type& class_name::GetTail() { std::lock_guard<std::mutex> lock(g_collectionStateMutex); return EnsureListStorage<class_name, element_type, arg_type>(this).GetTail(); } \
element_type class_name::GetTail() const { std::lock_guard<std::mutex> lock(g_collectionStateMutex); return EnsureListStorage<class_name, element_type, arg_type>(this).GetTail(); } \
typename class_name::POSITION class_name::GetHeadPosition() const { std::lock_guard<std::mutex> lock(g_collectionStateMutex); const auto* data = FindListStorage<class_name, element_type, arg_type>(this); return data ? data->GetHeadPosition() : POSITION(nullptr); } \
typename class_name::POSITION class_name::GetTailPosition() const { std::lock_guard<std::mutex> lock(g_collectionStateMutex); const auto* data = FindListStorage<class_name, element_type, arg_type>(this); return data ? data->GetTailPosition() : POSITION(nullptr); } \
element_type& class_name::GetNext(POSITION& rPosition) { std::lock_guard<std::mutex> lock(g_collectionStateMutex); return EnsureListStorage<class_name, element_type, arg_type>(this).GetNext(rPosition); } \
element_type class_name::GetNext(POSITION& rPosition) const { std::lock_guard<std::mutex> lock(g_collectionStateMutex); return EnsureListStorage<class_name, element_type, arg_type>(this).GetNext(rPosition); } \
element_type& class_name::GetPrev(POSITION& rPosition) { std::lock_guard<std::mutex> lock(g_collectionStateMutex); return EnsureListStorage<class_name, element_type, arg_type>(this).GetPrev(rPosition); } \
element_type class_name::GetPrev(POSITION& rPosition) const { std::lock_guard<std::mutex> lock(g_collectionStateMutex); return EnsureListStorage<class_name, element_type, arg_type>(this).GetPrev(rPosition); } \
element_type class_name::GetAt(POSITION position) const { std::lock_guard<std::mutex> lock(g_collectionStateMutex); return EnsureListStorage<class_name, element_type, arg_type>(this).GetAt(position); } \
void class_name::SetAt(POSITION pos, arg_type newElement) { std::lock_guard<std::mutex> lock(g_collectionStateMutex); EnsureListStorage<class_name, element_type, arg_type>(this).SetAt(pos, newElement); } \
void class_name::RemoveAt(POSITION position) { std::lock_guard<std::mutex> lock(g_collectionStateMutex); EnsureListStorage<class_name, element_type, arg_type>(this).RemoveAt(position); } \
typename class_name::POSITION class_name::FindIndex(INT_PTR nIndex) const { std::lock_guard<std::mutex> lock(g_collectionStateMutex); const auto* data = FindListStorage<class_name, element_type, arg_type>(this); return data ? data->FindIndex(ClampCollectionSize(nIndex)) : POSITION(nullptr); } \
typename class_name::POSITION class_name::Find(arg_type searchValue, POSITION startAfter) const { std::lock_guard<std::mutex> lock(g_collectionStateMutex); const auto* data = FindListStorage<class_name, element_type, arg_type>(this); return data ? data->Find(searchValue, startAfter) : POSITION(nullptr); } \
typename class_name::POSITION class_name::AddHead(arg_type newElement) { std::lock_guard<std::mutex> lock(g_collectionStateMutex); auto& data = EnsureListStorage<class_name, element_type, arg_type>(this); data.AddHead(newElement); return data.GetHeadPosition(); } \
typename class_name::POSITION class_name::AddTail(arg_type newElement) { std::lock_guard<std::mutex> lock(g_collectionStateMutex); auto& data = EnsureListStorage<class_name, element_type, arg_type>(this); data.AddTail(newElement); return data.GetTailPosition(); } \
void class_name::AddHead(class_name* pNewList) { if (!pNewList || pNewList == this) return; std::lock_guard<std::mutex> lock(g_collectionStateMutex); auto& data = EnsureListStorage<class_name, element_type, arg_type>(this); auto& srcData = EnsureListStorage<class_name, element_type, arg_type>(pNewList); data.AddHead(&srcData); } \
void class_name::AddTail(class_name* pNewList) { if (!pNewList || pNewList == this) return; std::lock_guard<std::mutex> lock(g_collectionStateMutex); auto& data = EnsureListStorage<class_name, element_type, arg_type>(this); auto& srcData = EnsureListStorage<class_name, element_type, arg_type>(pNewList); data.AddTail(&srcData); } \
element_type class_name::RemoveHead() { std::lock_guard<std::mutex> lock(g_collectionStateMutex); return EnsureListStorage<class_name, element_type, arg_type>(this).RemoveHead(); } \
element_type class_name::RemoveTail() { std::lock_guard<std::mutex> lock(g_collectionStateMutex); return EnsureListStorage<class_name, element_type, arg_type>(this).RemoveTail(); } \
typename class_name::POSITION class_name::InsertBefore(POSITION position, arg_type newElement) { \
    std::lock_guard<std::mutex> lock(g_collectionStateMutex); \
    auto& data = EnsureListStorage<class_name, element_type, arg_type>(this); \
    POSITION oldPos = position; \
    data.InsertBefore(position, newElement); \
    if (oldPos == POSITION(nullptr)) return data.GetTailPosition(); \
    POSITION pos = data.GetHeadPosition(); \
    POSITION prev(nullptr); \
    while (pos != POSITION(nullptr) && pos != oldPos) { prev = pos; data.GetNext(pos); } \
    return prev; \
} \
typename class_name::POSITION class_name::InsertAfter(POSITION position, arg_type newElement) { \
    std::lock_guard<std::mutex> lock(g_collectionStateMutex); \
    auto& data = EnsureListStorage<class_name, element_type, arg_type>(this); \
    POSITION oldPos = position; \
    data.InsertAfter(position, newElement); \
    if (oldPos == POSITION(nullptr)) return data.GetHeadPosition(); \
    POSITION result = oldPos; \
    data.GetNext(result); \
    return result; \
} \
void class_name::RemoveAll() { std::lock_guard<std::mutex> lock(g_collectionStateMutex); EnsureListStorage<class_name, element_type, arg_type>(this).RemoveAll(); } \
void class_name::Serialize(CArchive& ar) { std::lock_guard<std::mutex> lock(g_collectionStateMutex); auto& data = EnsureListStorage<class_name, element_type, arg_type>(this); if (ar.IsStoring()) ar << data; else ar >> data; }





#define OPENMFC_DEFINE_MAP_METHODS(class_name, key_type, arg_key_type, value_type, arg_value_type) \
class_name::class_name(INT_PTR nBlockSize) { \
    std::lock_guard<std::mutex> lock(g_collectionStateMutex); \
    EnsureCollectionState<class_name, MapWrapperState<key_type, arg_key_type, value_type, arg_value_type>>(this, ClampCollectionBlockSize(nBlockSize)); \
} \
 \
class_name::~class_name() { \
    std::lock_guard<std::mutex> lock(g_collectionStateMutex); \
    RemoveCollectionState<class_name, MapWrapperState<key_type, arg_key_type, value_type, arg_value_type>>(this); \
} \
 \
INT_PTR class_name::GetCount() const { std::lock_guard<std::mutex> lock(g_collectionStateMutex); const auto* data = FindMapStorage<class_name, key_type, arg_key_type, value_type, arg_value_type>(this); return data ? data->GetCount() : 0; } \
BOOL class_name::IsEmpty() const { return GetCount() == 0; } \
BOOL class_name::Lookup(arg_key_type key, value_type& rValue) const { std::lock_guard<std::mutex> lock(g_collectionStateMutex); const auto* data = FindMapStorage<class_name, key_type, arg_key_type, value_type, arg_value_type>(this); return (data && data->Lookup(key, rValue)) ? TRUE : FALSE; } \
value_type& class_name::operator[](arg_key_type key) { std::lock_guard<std::mutex> lock(g_collectionStateMutex); return EnsureMapStorage<class_name, key_type, arg_key_type, value_type, arg_value_type>(this)[key]; } \
const value_type& class_name::operator[](arg_key_type key) const { \
    return CollectionConstRef(const_cast<class_name*>(this)->operator[](key)); \
} \
void class_name::SetAt(arg_key_type key, arg_value_type newValue) { std::lock_guard<std::mutex> lock(g_collectionStateMutex); EnsureMapStorage<class_name, key_type, arg_key_type, value_type, arg_value_type>(this).SetAt(key, newValue); } \
BOOL class_name::RemoveKey(arg_key_type key) { std::lock_guard<std::mutex> lock(g_collectionStateMutex); auto& data = EnsureMapStorage<class_name, key_type, arg_key_type, value_type, arg_value_type>(this); return data.RemoveKey(key) ? TRUE : FALSE; } \
void class_name::RemoveAll() { std::lock_guard<std::mutex> lock(g_collectionStateMutex); EnsureMapStorage<class_name, key_type, arg_key_type, value_type, arg_value_type>(this).RemoveAll(); } \
typename class_name::POSITION class_name::GetStartPosition() const { std::lock_guard<std::mutex> lock(g_collectionStateMutex); const auto* data = FindMapStorage<class_name, key_type, arg_key_type, value_type, arg_value_type>(this); return data ? data->GetStartPosition() : POSITION(nullptr, 0); } \
void class_name::GetNextAssoc(POSITION& rNextPosition, key_type& rKey, value_type& rValue) const { std::lock_guard<std::mutex> lock(g_collectionStateMutex); EnsureMapStorage<class_name, key_type, arg_key_type, value_type, arg_value_type>(this).GetNextAssoc(rNextPosition, rKey, rValue); } \
UINT class_name::GetHashTableSize() const { std::lock_guard<std::mutex> lock(g_collectionStateMutex); const auto* data = FindMapStorage<class_name, key_type, arg_key_type, value_type, arg_value_type>(this); return data ? data->GetHashTableSize() : 17; } \
void class_name::InitHashTable(UINT hashSize, BOOL bAllocNow) { std::lock_guard<std::mutex> lock(g_collectionStateMutex); EnsureMapStorage<class_name, key_type, arg_key_type, value_type, arg_value_type>(this).InitHashTable(hashSize, bAllocNow != FALSE); } \
void class_name::Serialize(CArchive& ar) { std::lock_guard<std::mutex> lock(g_collectionStateMutex); auto& data = EnsureMapStorage<class_name, key_type, arg_key_type, value_type, arg_value_type>(this); if (ar.IsStoring()) ar << data; else ar >> data; }





// The collection-method macros stay defined: FilecoreSupport.cpp expands them
// and undefines them once it is done.

#define OPENMFC_WRAP_CTOR0(fn_name, class_name) \
extern "C" void* MS_ABI fn_name(class_name* pThis) { \
    return pThis ? new (pThis) class_name() : nullptr; \
}

#define OPENMFC_WRAP_CTOR1(fn_name, class_name, arg_type, cast_expr) \
extern "C" void* MS_ABI fn_name(class_name* pThis, arg_type arg0) { \
    return pThis ? new (pThis) class_name(cast_expr) : nullptr; \
}

#define OPENMFC_WRAP_DTOR(fn_name, class_name) \
extern "C" void MS_ABI fn_name(class_name* pThis) { \
    if (pThis) pThis->~class_name(); \
}

#define OPENMFC_WRAP_GETTHISCLASS(fn_name, class_name) \
extern "C" CRuntimeClass* MS_ABI fn_name() { \
    return class_name::GetThisClass(); \
}

#define OPENMFC_WRAP_GETRUNTIMECLASS(fn_name, class_name) \
extern "C" CRuntimeClass* MS_ABI fn_name(const class_name* pThis) { \
    return class_name::GetThisClass(); \
}

#define OPENMFC_WRAP_CREATEOBJECT(fn_name, class_name) \
extern "C" CObject* MS_ABI fn_name() { \
    return class_name::CreateObject(); \
}

#define OPENMFC_WRAP_SERIAL_EXTRACT(fn_name, class_name) \
extern "C" CArchive* MS_ABI fn_name(CArchive* ar, class_name** pOb) { \
    return (ar && pOb) ? &operator>>(*ar, *pOb) : ar; \
}





extern "C" void MS_ABI impl__Serialize_CWordArray__UEAAXAEAVCArchive___Z(CWordArray* pThis, CArchive* ar);


















#undef OPENMFC_WRAP_CTOR0
#undef OPENMFC_WRAP_CTOR1
#undef OPENMFC_WRAP_DTOR
#undef OPENMFC_WRAP_GETTHISCLASS
#undef OPENMFC_WRAP_GETRUNTIMECLASS
#undef OPENMFC_WRAP_CREATEOBJECT
#undef OPENMFC_WRAP_SERIAL_EXTRACT

#define OPENMFC_WRAP_CTOR0(fn_name, class_name) extern "C" void* MS_ABI fn_name(class_name* pThis) { return pThis ? new (pThis) class_name() : nullptr; }
#define OPENMFC_WRAP_CTOR1(fn_name, class_name, arg_type) extern "C" void* MS_ABI fn_name(class_name* pThis, arg_type arg0) { return pThis ? new (pThis) class_name(arg0) : nullptr; }
#define OPENMFC_WRAP_DTOR(fn_name, class_name) extern "C" void MS_ABI fn_name(class_name* pThis) { if (pThis) pThis->~class_name(); }
#define OPENMFC_WRAP_GETTHISCLASS(fn_name, class_name) extern "C" CRuntimeClass* MS_ABI fn_name() { return class_name::GetThisClass(); }
#define OPENMFC_WRAP_GETRUNTIMECLASS(fn_name, class_name) extern "C" CRuntimeClass* MS_ABI fn_name(const class_name* pThis) { return class_name::GetThisClass(); }
#define OPENMFC_WRAP_CREATEOBJECT(fn_name, class_name) extern "C" CObject* MS_ABI fn_name() { return class_name::CreateObject(); }
#define OPENMFC_WRAP_SERIAL_EXTRACT(fn_name, class_name) extern "C" CArchive* MS_ABI fn_name(CArchive* ar, class_name** pOb) { return (ar && pOb) ? &operator>>(*ar, *pOb) : ar; }

namespace openmfc { namespace detail { namespace filecore {
extern "C" void MS_ABI impl__FreeNode_CStringList__IEAAXPEAUCNode_1__Z(CStringList* pThis, void* pNode);
} } }
namespace openmfc { namespace detail { namespace filecore {
extern "C" void* MS_ABI impl__NewNode_CStringList__IEAAPEAUCNode_1_PEAU21_0_Z(CStringList* pThis, void* pPrev, void* pNext);
} } }

namespace openmfc { namespace detail { namespace filecore {
extern "C" void MS_ABI impl__FreeNode_CObList__IEAAXPEAUCNode_1__Z(CObList* pThis, void* pNode);
} } }
namespace openmfc { namespace detail { namespace filecore {
extern "C" void* MS_ABI impl__NewNode_CObList__IEAAPEAUCNode_1_PEAU21_0_Z(CObList* pThis, void* pPrev, void* pNext);
} } }

#undef OPENMFC_WRAP_CTOR0
#undef OPENMFC_WRAP_CTOR1
#undef OPENMFC_WRAP_DTOR
#undef OPENMFC_WRAP_GETTHISCLASS
#undef OPENMFC_WRAP_GETRUNTIMECLASS
#undef OPENMFC_WRAP_CREATEOBJECT
#undef OPENMFC_WRAP_SERIAL_EXTRACT

#define OPENMFC_WRAP_CTOR1(fn_name, class_name, arg_type) extern "C" void* MS_ABI fn_name(class_name* pThis, arg_type arg0) { return pThis ? new (pThis) class_name(arg0) : nullptr; }
#define OPENMFC_WRAP_DTOR(fn_name, class_name) extern "C" void MS_ABI fn_name(class_name* pThis) { if (pThis) pThis->~class_name(); }
#define OPENMFC_WRAP_GETTHISCLASS(fn_name, class_name) extern "C" CRuntimeClass* MS_ABI fn_name() { return class_name::GetThisClass(); }
#define OPENMFC_WRAP_GETRUNTIMECLASS(fn_name, class_name) extern "C" CRuntimeClass* MS_ABI fn_name(const class_name* pThis) { return class_name::GetThisClass(); }
#define OPENMFC_WRAP_CREATEOBJECT(fn_name, class_name) extern "C" CObject* MS_ABI fn_name() { return class_name::CreateObject(); }
#define OPENMFC_WRAP_SERIAL_EXTRACT(fn_name, class_name) extern "C" CArchive* MS_ABI fn_name(CArchive* ar, class_name** pOb) { return (ar && pOb) ? &operator>>(*ar, *pOb) : ar; }

namespace openmfc { namespace detail { namespace filecore {
extern "C" void MS_ABI impl__FreeAssoc_CMapPtrToPtr__IEAAXPEAUCAssoc_1__Z(CMapPtrToPtr* pThis, void* pAssoc);
} } }

namespace openmfc { namespace detail { namespace filecore {
extern "C" void MS_ABI impl__FreeAssoc_CMapPtrToWord__IEAAXPEAUCAssoc_1__Z(CMapPtrToWord* pThis, void* pAssoc);
} } }

namespace openmfc { namespace detail { namespace filecore {
extern "C" void MS_ABI impl__FreeAssoc_CMapStringToOb__IEAAXPEAUCAssoc_1__Z(CMapStringToOb* pThis, void* pAssoc);
} } }

namespace openmfc { namespace detail { namespace filecore {
extern "C" void MS_ABI impl__FreeAssoc_CMapStringToPtr__IEAAXPEAUCAssoc_1__Z(CMapStringToPtr* pThis, void* pAssoc);
} } }

namespace openmfc { namespace detail { namespace filecore {
extern "C" void MS_ABI impl__FreeAssoc_CMapStringToString__IEAAXPEAVCAssoc_1__Z(CMapStringToString* pThis, void* pAssoc);
} } }

namespace openmfc { namespace detail { namespace filecore {
extern "C" void MS_ABI impl__FreeAssoc_CMapWordToOb__IEAAXPEAUCAssoc_1__Z(CMapWordToOb* pThis, void* pAssoc);
} } }

namespace openmfc { namespace detail { namespace filecore {
extern "C" void MS_ABI impl__FreeAssoc_CMapWordToPtr__IEAAXPEAUCAssoc_1__Z(CMapWordToPtr* pThis, void* pAssoc);
} } }

#undef OPENMFC_WRAP_CTOR1
#undef OPENMFC_WRAP_DTOR
#undef OPENMFC_WRAP_GETTHISCLASS
#undef OPENMFC_WRAP_GETRUNTIMECLASS
#undef OPENMFC_WRAP_CREATEOBJECT
#undef OPENMFC_WRAP_SERIAL_EXTRACT

// =============================================================================
// CFile Implementation
// =============================================================================






















// =============================================================================
// CMemFile Implementation
// =============================================================================












// =============================================================================
// CStdioFile Implementation
// =============================================================================












// =============================================================================
// CArchive Implementation
// =============================================================================




class CArchiveAccess {
public:
    static void InvokeFillBuffer(CArchive* pArchive, unsigned int nBytesNeeded) {
        if (pArchive) {
            pArchive->FillBuffer(nBytesNeeded);
        }
    }
};








// Reading operators














// Writing operators














// String operations















// =============================================================================
// CFile extern "C" MS_ABI Thunks
// These are cross-ABI vtable entry points for CFile virtual methods.
// =============================================================================











struct CFileAccessor : CFile {
    static void InvokeCommonInit(CFile* pFile, const wchar_t* lpszFileName, unsigned int nOpenFlags, void* pTM) {
        static_cast<CFileAccessor*>(pFile)->CommonInit(lpszFileName, nOpenFlags, pTM);
    }
};

struct CMemFileAccessor : CMemFile {
    static UINT& Position(CMemFile* pFile) { return static_cast<CMemFileAccessor*>(pFile)->m_nPosition; }
    static UINT Position(const CMemFile* pFile) { return static_cast<const CMemFileAccessor*>(pFile)->m_nPosition; }
    static UINT GrowBytes(const CMemFile* pFile) { return static_cast<const CMemFileAccessor*>(pFile)->m_nGrowBytes; }
    static BYTE* Buffer(CMemFile* pFile) { return static_cast<CMemFileAccessor*>(pFile)->m_lpBuffer; }
    static const BYTE* Buffer(const CMemFile* pFile) { return static_cast<const CMemFileAccessor*>(pFile)->m_lpBuffer; }
    static UINT BufferSize(const CMemFile* pFile) { return static_cast<const CMemFileAccessor*>(pFile)->m_nBufferSize; }
};

struct CStdioFileAccessor : CStdioFile {
    static FILE*& Stream(CStdioFile* pFile) { return static_cast<CStdioFileAccessor*>(pFile)->m_pStream; }
    static FILE* Stream(const CStdioFile* pFile) { return static_cast<const CStdioFileAccessor*>(pFile)->m_pStream; }
};














// CFile runtime-class descriptor. CFile is DECLARE_DYNAMIC in real MFC: base
// CObject, schema 0xFFFF, no factory. m_nObjectSize is the real MSVC sizeof(CFile)
// (40, harvested from mfc140u.dll), not the host sizeof. Following the repo's
// IMPLEMENT_DYNAMIC convention (the base link is the generated m_pfnGetBaseClass thunk). This descriptor is an internal
// DLL symbol (not added to the .def) so the export set stays at the 14,109 match,
// but it gives CFile-derived RTTI getters a real base to chain to.



extern "C" void* impl__hFileNull_CFile__2QEAXEA = (void*)INVALID_HANDLE_VALUE;













// CMemFile runtime-class descriptor. DECLARE_DYNAMIC in real MFC: base CFile,
// schema 0xFFFF, no factory. m_nObjectSize is the real mfc140u sizeof(CMemFile) (88).
// Internal DLL symbol (not in the .def) so the export set stays the 14,109 match.



namespace openmfc { namespace detail { namespace filecore {
void OpenStdioFile(CStdioFile* pThis, const wchar_t* lpszFileName, unsigned int nOpenFlags);
} } }












// CStdioFile runtime-class descriptor. DECLARE_DYNAMIC in real MFC: base CFile,
// schema 0xFFFF, no factory. Real MSVC sizeof(CStdioFile)=48 (harvested from
// mfc140u.dll). A real _AFXDLL client walks the base chain via m_pfnGetBaseClass,
// so set it to CFile's exported getter (CFile::classCFile is referenceable here, so
// the thunk just returns it). This gives CStdioFile-derived classes (CInternetFile/
// CGopherFile) an unbroken IsKindOf chain to CObject. File-scope internal symbol.
namespace openmfc { namespace detail { namespace filecore {
CRuntimeClass* AFXAPI gb_CStdioFile();
} } }
namespace openmfc { namespace detail { namespace filecore {
extern CRuntimeClass g_classCStdioFile;
} } }



extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CFileException__SAPEAUCRuntimeClass__XZ();
extern "C" void MS_ABI impl__AfxThrowFileException__YAXHJPEB_W_Z(int cause, long lOsError, const wchar_t* lpszFileName);

namespace openmfc { namespace detail { namespace filecore {
int FileExceptionFromErrno(int nErrno);
} } }

namespace openmfc { namespace detail { namespace filecore {
int FileExceptionFromOsError(long lOsError);
} } }








//=== CAsyncMonikerFile export =================================================

