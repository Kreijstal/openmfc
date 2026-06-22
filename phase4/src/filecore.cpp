// File and Archive Core Implementation
//
// Implements CFile, CMemFile, CStdioFile, and CArchive for serialization.

#define OPENMFC_APPCORE_IMPL
#include "openmfc/afx.h"
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

namespace {

std::mutex g_collectionStateMutex;

template<typename Wrapper, typename State>
using CollectionStateMap = std::unordered_map<const Wrapper*, State>;

template<typename Wrapper, typename State>
CollectionStateMap<Wrapper, State>& GetCollectionStates() {
    static CollectionStateMap<Wrapper, State> states;
    return states;
}

template<typename Wrapper, typename State, typename... Args>
State& EnsureCollectionState(const Wrapper* self, Args&&... args) {
    auto& states = GetCollectionStates<Wrapper, State>();
    auto it = states.find(self);
    if (it == states.end()) {
        it = states.emplace(self, State(std::forward<Args>(args)...)).first;
    }
    return it->second;
}

template<typename Wrapper, typename State>
State* FindCollectionState(const Wrapper* self) {
    auto& states = GetCollectionStates<Wrapper, State>();
    auto it = states.find(self);
    return it == states.end() ? nullptr : &it->second;
}

template<typename Wrapper, typename State>
void RemoveCollectionState(const Wrapper* self) {
    GetCollectionStates<Wrapper, State>().erase(self);
}

template<typename TYPE, typename ARG_TYPE>
struct ArrayWrapperState {
    CArray<TYPE, ARG_TYPE> data;
};

template<typename TYPE, typename ARG_TYPE>
struct ListWrapperState {
    explicit ListWrapperState(int nBlockSize = 10) : data(nBlockSize) {}
    CList<TYPE, ARG_TYPE> data;
};

template<typename KEY, typename ARG_KEY, typename VALUE, typename ARG_VALUE>
struct MapWrapperState {
    explicit MapWrapperState(int nBlockSize = 10) : data(nBlockSize) {}
    CMap<KEY, ARG_KEY, VALUE, ARG_VALUE> data;
};

template<typename VALUE, typename ARG_VALUE>
struct CStringKeyMapState {
    explicit CStringKeyMapState(int nBlockSize = 10) : data(nBlockSize) {}
    CMap<CString, const CString&, VALUE, ARG_VALUE> data;
    CString lookupKeyScratch;
};

static int ClampCollectionSize(INT_PTR value) {
    if (value <= 0) return 0;
    if (value > std::numeric_limits<int>::max()) return std::numeric_limits<int>::max();
    return static_cast<int>(value);
}

static int ClampCollectionGrow(INT_PTR value) {
    if (value < -1) return -1;
    if (value > std::numeric_limits<int>::max()) return std::numeric_limits<int>::max();
    return static_cast<int>(value);
}

static int ClampCollectionBlockSize(INT_PTR value) {
    if (value <= 0) return 10;
    if (value > std::numeric_limits<int>::max()) return std::numeric_limits<int>::max();
    return static_cast<int>(value);
}

static CString NormalizeStringKey(const wchar_t* key) {
    return CString(key ? key : L"");
}

template<typename T>
const T& CollectionConstRef(T& value) {
    return value;
}

template<typename T>
const T*& CollectionConstRef(T*& value) {
    static thread_local const T* snapshot = nullptr;
    snapshot = value;
    return snapshot;
}

template<typename T>
const T* CollectionConstData(T* data) {
    return data;
}

template<typename T>
const T** CollectionConstData(T** data) {
    return (const T**)data;
}

static UINT HashCStringKey(const CString& key) {
    UINT hash = 0;
    const wchar_t* p = static_cast<const wchar_t*>(key);
    while (*p) {
        hash = (hash << 5) + hash + static_cast<UINT>(*p++);
    }
    return hash;
}

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

template<typename Wrapper, typename TYPE, typename ARG_TYPE>
static CArray<TYPE, ARG_TYPE>& EnsureArrayStorage(const Wrapper* self) {
    return EnsureCollectionState<Wrapper, ArrayWrapperState<TYPE, ARG_TYPE>>(self).data;
}

template<typename Wrapper, typename TYPE, typename ARG_TYPE>
static const CArray<TYPE, ARG_TYPE>* FindArrayStorage(const Wrapper* self) {
    const auto* state = FindCollectionState<Wrapper, ArrayWrapperState<TYPE, ARG_TYPE>>(self);
    return state ? &state->data : nullptr;
}

template<typename Wrapper, typename TYPE, typename ARG_TYPE>
static CList<TYPE, ARG_TYPE>& EnsureListStorage(const Wrapper* self, int nBlockSize = 10) {
    return EnsureCollectionState<Wrapper, ListWrapperState<TYPE, ARG_TYPE>>(self, ClampCollectionBlockSize(nBlockSize)).data;
}

template<typename Wrapper, typename TYPE, typename ARG_TYPE>
static const CList<TYPE, ARG_TYPE>* FindListStorage(const Wrapper* self) {
    const auto* state = FindCollectionState<Wrapper, ListWrapperState<TYPE, ARG_TYPE>>(self);
    return state ? &state->data : nullptr;
}

template<typename Wrapper, typename KEY, typename ARG_KEY, typename VALUE, typename ARG_VALUE>
static CMap<KEY, ARG_KEY, VALUE, ARG_VALUE>& EnsureMapStorage(const Wrapper* self, int nBlockSize = 10) {
    return EnsureCollectionState<Wrapper, MapWrapperState<KEY, ARG_KEY, VALUE, ARG_VALUE>>(self, ClampCollectionBlockSize(nBlockSize)).data;
}

template<typename Wrapper, typename KEY, typename ARG_KEY, typename VALUE, typename ARG_VALUE>
static const CMap<KEY, ARG_KEY, VALUE, ARG_VALUE>* FindMapStorage(const Wrapper* self) {
    const auto* state = FindCollectionState<Wrapper, MapWrapperState<KEY, ARG_KEY, VALUE, ARG_VALUE>>(self);
    return state ? &state->data : nullptr;
}

template<typename Wrapper, typename VALUE, typename ARG_VALUE>
static CStringKeyMapState<VALUE, ARG_VALUE>& EnsureCStringKeyMapStorage(const Wrapper* self, int nBlockSize = 10) {
    return EnsureCollectionState<Wrapper, CStringKeyMapState<VALUE, ARG_VALUE>>(self, ClampCollectionBlockSize(nBlockSize));
}

template<typename Wrapper, typename VALUE, typename ARG_VALUE>
static const CStringKeyMapState<VALUE, ARG_VALUE>* FindCStringKeyMapStorage(const Wrapper* self) {
    return FindCollectionState<Wrapper, CStringKeyMapState<VALUE, ARG_VALUE>>(self);
}

static CStringMapStringState& EnsureCStringMapStringStorage(const CMapStringToString* self, int nBlockSize = 10) {
    return EnsureCollectionState<CMapStringToString, CStringMapStringState>(self, ClampCollectionBlockSize(nBlockSize));
}

static const CStringMapStringState* FindCStringMapStringStorage(const CMapStringToString* self) {
    return FindCollectionState<CMapStringToString, CStringMapStringState>(self);
}

template<typename KEY, typename VALUE>
struct AssocSnapshot {
    AssocSnapshot* pNext = nullptr;
    UINT nHashValue = 0;
    KEY key{};
    VALUE value{};
};

template<typename VALUE>
struct ListNodeSnapshot {
    ListNodeSnapshot* pNext = nullptr;
    ListNodeSnapshot* pPrev = nullptr;
    VALUE data{};
};

} // namespace

IMPLEMENT_DYNAMIC(CUIntArray, CObject)
IMPLEMENT_SERIAL(CDWordArray, CObject, 0xFFFF)
IMPLEMENT_SERIAL(CObArray, CObject, 0xFFFF)
IMPLEMENT_DYNAMIC(CPtrArray, CObject)
IMPLEMENT_SERIAL(CByteArray, CObject, 0xFFFF)
IMPLEMENT_SERIAL(CStringArray, CObject, 0xFFFF)
IMPLEMENT_SERIAL(CStringList, CObject, 0xFFFF)
IMPLEMENT_SERIAL(CObList, CObject, 0xFFFF)
IMPLEMENT_DYNAMIC(CMapPtrToPtr, CObject)
IMPLEMENT_DYNAMIC(CMapPtrToWord, CObject)
IMPLEMENT_SERIAL(CMapStringToOb, CObject, 0xFFFF)
IMPLEMENT_DYNAMIC(CMapStringToPtr, CObject)
IMPLEMENT_SERIAL(CMapStringToString, CObject, 0xFFFF)
IMPLEMENT_SERIAL(CMapWordToOb, CObject, 0xFFFF)
IMPLEMENT_DYNAMIC(CMapWordToPtr, CObject)

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

OPENMFC_DEFINE_ARRAY_METHODS(CUIntArray, unsigned int, unsigned int)
OPENMFC_DEFINE_ARRAY_METHODS(CDWordArray, DWORD, DWORD)
OPENMFC_DEFINE_ARRAY_METHODS(CObArray, CObject*, CObject*)
OPENMFC_DEFINE_ARRAY_METHODS(CPtrArray, void*, void*)
OPENMFC_DEFINE_ARRAY_METHODS(CByteArray, BYTE, BYTE)

CStringArray::CStringArray() {
    std::lock_guard<std::mutex> lock(g_collectionStateMutex);
    EnsureCollectionState<CStringArray, ArrayWrapperState<CString, const CString&>>(this);
}

CStringArray::~CStringArray() {
    std::lock_guard<std::mutex> lock(g_collectionStateMutex);
    RemoveCollectionState<CStringArray, ArrayWrapperState<CString, const CString&>>(this);
}

INT_PTR CStringArray::GetSize() const { std::lock_guard<std::mutex> lock(g_collectionStateMutex); const auto* data = FindArrayStorage<CStringArray, CString, const CString&>(this); return data ? data->GetSize() : 0; }
INT_PTR CStringArray::GetCount() const { return GetSize(); }
BOOL CStringArray::IsEmpty() const { return GetSize() == 0; }
INT_PTR CStringArray::GetUpperBound() const { return GetSize() - 1; }
void CStringArray::SetSize(INT_PTR nNewSize, INT_PTR nGrowBy) { std::lock_guard<std::mutex> lock(g_collectionStateMutex); EnsureArrayStorage<CStringArray, CString, const CString&>(this).SetSize(ClampCollectionSize(nNewSize), ClampCollectionGrow(nGrowBy)); }
void CStringArray::FreeExtra() { std::lock_guard<std::mutex> lock(g_collectionStateMutex); EnsureArrayStorage<CStringArray, CString, const CString&>(this).FreeExtra(); }
void CStringArray::RemoveAll() { std::lock_guard<std::mutex> lock(g_collectionStateMutex); EnsureArrayStorage<CStringArray, CString, const CString&>(this).RemoveAll(); }
CString CStringArray::GetAt(INT_PTR nIndex) const { std::lock_guard<std::mutex> lock(g_collectionStateMutex); return EnsureArrayStorage<CStringArray, CString, const CString&>(this).GetAt(ClampCollectionSize(nIndex)); }
void CStringArray::SetAt(INT_PTR nIndex, const CString& newElement) { std::lock_guard<std::mutex> lock(g_collectionStateMutex); EnsureArrayStorage<CStringArray, CString, const CString&>(this).SetAt(ClampCollectionSize(nIndex), newElement); }
CString& CStringArray::ElementAt(INT_PTR nIndex) { std::lock_guard<std::mutex> lock(g_collectionStateMutex); return EnsureArrayStorage<CStringArray, CString, const CString&>(this).ElementAt(ClampCollectionSize(nIndex)); }
const CString& CStringArray::ElementAt(INT_PTR nIndex) const { std::lock_guard<std::mutex> lock(g_collectionStateMutex); return EnsureArrayStorage<CStringArray, CString, const CString&>(this).ElementAt(ClampCollectionSize(nIndex)); }
CString CStringArray::operator[](INT_PTR nIndex) const { return GetAt(nIndex); }
CString& CStringArray::operator[](INT_PTR nIndex) { return ElementAt(nIndex); }
CString* CStringArray::GetData() { std::lock_guard<std::mutex> lock(g_collectionStateMutex); return EnsureArrayStorage<CStringArray, CString, const CString&>(this).GetData(); }
const CString* CStringArray::GetData() const { std::lock_guard<std::mutex> lock(g_collectionStateMutex); return EnsureArrayStorage<CStringArray, CString, const CString&>(this).GetData(); }
void CStringArray::SetAtGrow(INT_PTR nIndex, const CString& newElement) { std::lock_guard<std::mutex> lock(g_collectionStateMutex); EnsureArrayStorage<CStringArray, CString, const CString&>(this).SetAtGrow(ClampCollectionSize(nIndex), newElement); }
void CStringArray::SetAtGrow(INT_PTR nIndex, const wchar_t* newElement) { SetAtGrow(nIndex, NormalizeStringKey(newElement)); }
INT_PTR CStringArray::Add(const CString& newElement) { std::lock_guard<std::mutex> lock(g_collectionStateMutex); return EnsureArrayStorage<CStringArray, CString, const CString&>(this).Add(newElement); }
INT_PTR CStringArray::Append(const CStringArray& src) { std::lock_guard<std::mutex> lock(g_collectionStateMutex); auto& data = EnsureArrayStorage<CStringArray, CString, const CString&>(this); const auto& srcData = EnsureArrayStorage<CStringArray, CString, const CString&>(&src); return data.Append(srcData); }
void CStringArray::Copy(const CStringArray& src) { std::lock_guard<std::mutex> lock(g_collectionStateMutex); auto& data = EnsureArrayStorage<CStringArray, CString, const CString&>(this); const auto& srcData = EnsureArrayStorage<CStringArray, CString, const CString&>(&src); data.Copy(srcData); }
void CStringArray::InsertAt(INT_PTR nIndex, const CString& newElement, INT_PTR nCount) { std::lock_guard<std::mutex> lock(g_collectionStateMutex); EnsureArrayStorage<CStringArray, CString, const CString&>(this).InsertAt(ClampCollectionSize(nIndex), newElement, ClampCollectionSize(nCount)); }
void CStringArray::InsertAt(INT_PTR nIndex, const wchar_t* newElement, INT_PTR nCount) { InsertAt(nIndex, NormalizeStringKey(newElement), nCount); }
void CStringArray::RemoveAt(INT_PTR nIndex, INT_PTR nCount) { std::lock_guard<std::mutex> lock(g_collectionStateMutex); EnsureArrayStorage<CStringArray, CString, const CString&>(this).RemoveAt(ClampCollectionSize(nIndex), ClampCollectionSize(nCount)); }
void CStringArray::InsertAt(INT_PTR nStartIndex, CStringArray* pNewArray) { if (!pNewArray) return; std::lock_guard<std::mutex> lock(g_collectionStateMutex); auto& data = EnsureArrayStorage<CStringArray, CString, const CString&>(this); auto& srcData = EnsureArrayStorage<CStringArray, CString, const CString&>(pNewArray); data.InsertAt(ClampCollectionSize(nStartIndex), &srcData); }
void CStringArray::Serialize(CArchive& ar) { std::lock_guard<std::mutex> lock(g_collectionStateMutex); auto& data = EnsureArrayStorage<CStringArray, CString, const CString&>(this); if (ar.IsStoring()) ar << data; else ar >> data; }
void CStringArray::InsertEmpty(INT_PTR nIndex, INT_PTR nCount) { std::lock_guard<std::mutex> lock(g_collectionStateMutex); EnsureArrayStorage<CStringArray, CString, const CString&>(this).InsertAt(ClampCollectionSize(nIndex), CString(), ClampCollectionSize(nCount)); }

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

OPENMFC_DEFINE_LIST_METHODS(CObList, CObject*, CObject*)

CStringList::CStringList(INT_PTR nBlockSize) {
    std::lock_guard<std::mutex> lock(g_collectionStateMutex);
    EnsureCollectionState<CStringList, ListWrapperState<CString, const CString&>>(this, ClampCollectionBlockSize(nBlockSize));
}

CStringList::~CStringList() {
    std::lock_guard<std::mutex> lock(g_collectionStateMutex);
    RemoveCollectionState<CStringList, ListWrapperState<CString, const CString&>>(this);
}

INT_PTR CStringList::GetCount() const { std::lock_guard<std::mutex> lock(g_collectionStateMutex); const auto* data = FindListStorage<CStringList, CString, const CString&>(this); return data ? data->GetCount() : 0; }
BOOL CStringList::IsEmpty() const { return GetCount() == 0; }
CString& CStringList::GetHead() { std::lock_guard<std::mutex> lock(g_collectionStateMutex); return EnsureListStorage<CStringList, CString, const CString&>(this).GetHead(); }
CString CStringList::GetHead() const { std::lock_guard<std::mutex> lock(g_collectionStateMutex); return EnsureListStorage<CStringList, CString, const CString&>(this).GetHead(); }
CString& CStringList::GetTail() { std::lock_guard<std::mutex> lock(g_collectionStateMutex); return EnsureListStorage<CStringList, CString, const CString&>(this).GetTail(); }
CString CStringList::GetTail() const { std::lock_guard<std::mutex> lock(g_collectionStateMutex); return EnsureListStorage<CStringList, CString, const CString&>(this).GetTail(); }
CStringList::POSITION CStringList::GetHeadPosition() const { std::lock_guard<std::mutex> lock(g_collectionStateMutex); const auto* data = FindListStorage<CStringList, CString, const CString&>(this); return data ? data->GetHeadPosition() : POSITION(nullptr); }
CStringList::POSITION CStringList::GetTailPosition() const { std::lock_guard<std::mutex> lock(g_collectionStateMutex); const auto* data = FindListStorage<CStringList, CString, const CString&>(this); return data ? data->GetTailPosition() : POSITION(nullptr); }
CString& CStringList::GetNext(POSITION& rPosition) { std::lock_guard<std::mutex> lock(g_collectionStateMutex); return EnsureListStorage<CStringList, CString, const CString&>(this).GetNext(rPosition); }
CString CStringList::GetNext(POSITION& rPosition) const { std::lock_guard<std::mutex> lock(g_collectionStateMutex); return EnsureListStorage<CStringList, CString, const CString&>(this).GetNext(rPosition); }
CString& CStringList::GetPrev(POSITION& rPosition) { std::lock_guard<std::mutex> lock(g_collectionStateMutex); return EnsureListStorage<CStringList, CString, const CString&>(this).GetPrev(rPosition); }
CString CStringList::GetPrev(POSITION& rPosition) const { std::lock_guard<std::mutex> lock(g_collectionStateMutex); return EnsureListStorage<CStringList, CString, const CString&>(this).GetPrev(rPosition); }
CString CStringList::GetAt(POSITION position) const { std::lock_guard<std::mutex> lock(g_collectionStateMutex); return EnsureListStorage<CStringList, CString, const CString&>(this).GetAt(position); }
void CStringList::SetAt(POSITION pos, const CString& newElement) { std::lock_guard<std::mutex> lock(g_collectionStateMutex); EnsureListStorage<CStringList, CString, const CString&>(this).SetAt(pos, newElement); }
void CStringList::RemoveAt(POSITION position) { std::lock_guard<std::mutex> lock(g_collectionStateMutex); EnsureListStorage<CStringList, CString, const CString&>(this).RemoveAt(position); }
CStringList::POSITION CStringList::FindIndex(INT_PTR nIndex) const { std::lock_guard<std::mutex> lock(g_collectionStateMutex); const auto* data = FindListStorage<CStringList, CString, const CString&>(this); return data ? data->FindIndex(ClampCollectionSize(nIndex)) : POSITION(nullptr); }
CStringList::POSITION CStringList::Find(const CString& searchValue, POSITION startAfter) const { std::lock_guard<std::mutex> lock(g_collectionStateMutex); const auto* data = FindListStorage<CStringList, CString, const CString&>(this); return data ? data->Find(searchValue, startAfter) : POSITION(nullptr); }
CStringList::POSITION CStringList::Find(const wchar_t* searchValue, POSITION startAfter) const { return Find(NormalizeStringKey(searchValue), startAfter); }
CStringList::POSITION CStringList::AddHead(const CString& newElement) { std::lock_guard<std::mutex> lock(g_collectionStateMutex); auto& data = EnsureListStorage<CStringList, CString, const CString&>(this); data.AddHead(newElement); return data.GetHeadPosition(); }
CStringList::POSITION CStringList::AddHead(const wchar_t* newElement) { return AddHead(NormalizeStringKey(newElement)); }
void CStringList::AddHead(CStringList* pNewList) { if (!pNewList || pNewList == this) return; std::lock_guard<std::mutex> lock(g_collectionStateMutex); auto& data = EnsureListStorage<CStringList, CString, const CString&>(this); auto& srcData = EnsureListStorage<CStringList, CString, const CString&>(pNewList); data.AddHead(&srcData); }
CStringList::POSITION CStringList::AddTail(const CString& newElement) { std::lock_guard<std::mutex> lock(g_collectionStateMutex); auto& data = EnsureListStorage<CStringList, CString, const CString&>(this); data.AddTail(newElement); return data.GetTailPosition(); }
CStringList::POSITION CStringList::AddTail(const wchar_t* newElement) { return AddTail(NormalizeStringKey(newElement)); }
void CStringList::AddTail(CStringList* pNewList) { if (!pNewList || pNewList == this) return; std::lock_guard<std::mutex> lock(g_collectionStateMutex); auto& data = EnsureListStorage<CStringList, CString, const CString&>(this); auto& srcData = EnsureListStorage<CStringList, CString, const CString&>(pNewList); data.AddTail(&srcData); }
CString CStringList::RemoveHead() { std::lock_guard<std::mutex> lock(g_collectionStateMutex); return EnsureListStorage<CStringList, CString, const CString&>(this).RemoveHead(); }
CString CStringList::RemoveTail() { std::lock_guard<std::mutex> lock(g_collectionStateMutex); return EnsureListStorage<CStringList, CString, const CString&>(this).RemoveTail(); }
CStringList::POSITION CStringList::InsertBefore(POSITION position, const CString& newElement) { std::lock_guard<std::mutex> lock(g_collectionStateMutex); auto& data = EnsureListStorage<CStringList, CString, const CString&>(this); POSITION oldPos = position; data.InsertBefore(position, newElement); if (oldPos == POSITION(nullptr)) return data.GetTailPosition(); POSITION pos = data.GetHeadPosition(); POSITION prev(nullptr); while (pos != POSITION(nullptr) && pos != oldPos) { prev = pos; data.GetNext(pos); } return prev; }
CStringList::POSITION CStringList::InsertBefore(POSITION position, const wchar_t* newElement) { return InsertBefore(position, NormalizeStringKey(newElement)); }
CStringList::POSITION CStringList::InsertAfter(POSITION position, const CString& newElement) { std::lock_guard<std::mutex> lock(g_collectionStateMutex); auto& data = EnsureListStorage<CStringList, CString, const CString&>(this); POSITION oldPos = position; data.InsertAfter(position, newElement); if (oldPos == POSITION(nullptr)) return data.GetHeadPosition(); POSITION result = oldPos; data.GetNext(result); return result; }
CStringList::POSITION CStringList::InsertAfter(POSITION position, const wchar_t* newElement) { return InsertAfter(position, NormalizeStringKey(newElement)); }
void CStringList::RemoveAll() { std::lock_guard<std::mutex> lock(g_collectionStateMutex); EnsureListStorage<CStringList, CString, const CString&>(this).RemoveAll(); }
void CStringList::Serialize(CArchive& ar) { std::lock_guard<std::mutex> lock(g_collectionStateMutex); auto& data = EnsureListStorage<CStringList, CString, const CString&>(this); if (ar.IsStoring()) ar << data; else ar >> data; }

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

OPENMFC_DEFINE_MAP_METHODS(CMapPtrToPtr, void*, void*, void*, void*)
OPENMFC_DEFINE_MAP_METHODS(CMapPtrToWord, void*, void*, WORD, WORD)
OPENMFC_DEFINE_MAP_METHODS(CMapWordToOb, WORD, WORD, CObject*, CObject*)
OPENMFC_DEFINE_MAP_METHODS(CMapWordToPtr, WORD, WORD, void*, void*)

CMapStringToOb::CMapStringToOb(INT_PTR nBlockSize) { std::lock_guard<std::mutex> lock(g_collectionStateMutex); EnsureCStringKeyMapStorage<CMapStringToOb, CObject*, CObject*>(this, ClampCollectionBlockSize(nBlockSize)); }
CMapStringToOb::~CMapStringToOb() { std::lock_guard<std::mutex> lock(g_collectionStateMutex); RemoveCollectionState<CMapStringToOb, CStringKeyMapState<CObject*, CObject*>>(this); }
INT_PTR CMapStringToOb::GetCount() const { std::lock_guard<std::mutex> lock(g_collectionStateMutex); const auto* state = FindCStringKeyMapStorage<CMapStringToOb, CObject*, CObject*>(this); return state ? state->data.GetCount() : 0; }
BOOL CMapStringToOb::IsEmpty() const { return GetCount() == 0; }
BOOL CMapStringToOb::Lookup(const CString& key, CObject*& rValue) const { std::lock_guard<std::mutex> lock(g_collectionStateMutex); const auto* state = FindCStringKeyMapStorage<CMapStringToOb, CObject*, CObject*>(this); return (state && state->data.Lookup(key, rValue)) ? TRUE : FALSE; }
BOOL CMapStringToOb::Lookup(const wchar_t* key, CObject*& rValue) const { return Lookup(NormalizeStringKey(key), rValue); }
BOOL CMapStringToOb::LookupKey(const CString& key, const wchar_t*& rKey) const { std::lock_guard<std::mutex> lock(g_collectionStateMutex); auto& state = EnsureCStringKeyMapStorage<CMapStringToOb, CObject*, CObject*>(this); CString actualKey; if (!state.data.LookupKey(key, actualKey)) return FALSE; state.lookupKeyScratch = actualKey; rKey = static_cast<const wchar_t*>(state.lookupKeyScratch); return TRUE; }
BOOL CMapStringToOb::LookupKey(const wchar_t* key, const wchar_t*& rKey) const { return LookupKey(NormalizeStringKey(key), rKey); }
CObject*& CMapStringToOb::operator[](const CString& key) { std::lock_guard<std::mutex> lock(g_collectionStateMutex); return EnsureCStringKeyMapStorage<CMapStringToOb, CObject*, CObject*>(this).data[key]; }
CObject*& CMapStringToOb::operator[](const wchar_t* key) { return (*this)[NormalizeStringKey(key)]; }
const CObject* CMapStringToOb::operator[](const wchar_t* key) const { CObject* value = nullptr; Lookup(key, value); return value; }
void CMapStringToOb::SetAt(const CString& key, CObject* newValue) { std::lock_guard<std::mutex> lock(g_collectionStateMutex); EnsureCStringKeyMapStorage<CMapStringToOb, CObject*, CObject*>(this).data.SetAt(key, newValue); }
void CMapStringToOb::SetAt(const wchar_t* key, CObject* newValue) { SetAt(NormalizeStringKey(key), newValue); }
BOOL CMapStringToOb::RemoveKey(const CString& key) { std::lock_guard<std::mutex> lock(g_collectionStateMutex); auto& state = EnsureCStringKeyMapStorage<CMapStringToOb, CObject*, CObject*>(this); return state.data.RemoveKey(key) ? TRUE : FALSE; }
BOOL CMapStringToOb::RemoveKey(const wchar_t* key) { return RemoveKey(NormalizeStringKey(key)); }
void CMapStringToOb::RemoveAll() { std::lock_guard<std::mutex> lock(g_collectionStateMutex); EnsureCStringKeyMapStorage<CMapStringToOb, CObject*, CObject*>(this).data.RemoveAll(); }
CMapStringToOb::POSITION CMapStringToOb::GetStartPosition() const { std::lock_guard<std::mutex> lock(g_collectionStateMutex); const auto* state = FindCStringKeyMapStorage<CMapStringToOb, CObject*, CObject*>(this); return state ? state->data.GetStartPosition() : POSITION(nullptr, 0); }
void CMapStringToOb::GetNextAssoc(POSITION& rNextPosition, CString& rKey, CObject*& rValue) const { std::lock_guard<std::mutex> lock(g_collectionStateMutex); EnsureCStringKeyMapStorage<CMapStringToOb, CObject*, CObject*>(this).data.GetNextAssoc(rNextPosition, rKey, rValue); }
UINT CMapStringToOb::GetHashTableSize() const { std::lock_guard<std::mutex> lock(g_collectionStateMutex); const auto* state = FindCStringKeyMapStorage<CMapStringToOb, CObject*, CObject*>(this); return state ? state->data.GetHashTableSize() : 17; }
void CMapStringToOb::InitHashTable(UINT hashSize, BOOL bAllocNow) { std::lock_guard<std::mutex> lock(g_collectionStateMutex); EnsureCStringKeyMapStorage<CMapStringToOb, CObject*, CObject*>(this).data.InitHashTable(hashSize, bAllocNow != FALSE); }
void CMapStringToOb::Serialize(CArchive& ar) { std::lock_guard<std::mutex> lock(g_collectionStateMutex); auto& state = EnsureCStringKeyMapStorage<CMapStringToOb, CObject*, CObject*>(this); if (ar.IsStoring()) ar << state.data; else ar >> state.data; }

CMapStringToPtr::CMapStringToPtr(INT_PTR nBlockSize) { std::lock_guard<std::mutex> lock(g_collectionStateMutex); EnsureCStringKeyMapStorage<CMapStringToPtr, void*, void*>(this, ClampCollectionBlockSize(nBlockSize)); }
CMapStringToPtr::~CMapStringToPtr() { std::lock_guard<std::mutex> lock(g_collectionStateMutex); RemoveCollectionState<CMapStringToPtr, CStringKeyMapState<void*, void*>>(this); }
INT_PTR CMapStringToPtr::GetCount() const { std::lock_guard<std::mutex> lock(g_collectionStateMutex); const auto* state = FindCStringKeyMapStorage<CMapStringToPtr, void*, void*>(this); return state ? state->data.GetCount() : 0; }
BOOL CMapStringToPtr::IsEmpty() const { return GetCount() == 0; }
BOOL CMapStringToPtr::Lookup(const CString& key, void*& rValue) const { std::lock_guard<std::mutex> lock(g_collectionStateMutex); const auto* state = FindCStringKeyMapStorage<CMapStringToPtr, void*, void*>(this); return (state && state->data.Lookup(key, rValue)) ? TRUE : FALSE; }
BOOL CMapStringToPtr::Lookup(const wchar_t* key, void*& rValue) const { return Lookup(NormalizeStringKey(key), rValue); }
BOOL CMapStringToPtr::LookupKey(const CString& key, const wchar_t*& rKey) const { std::lock_guard<std::mutex> lock(g_collectionStateMutex); auto& state = EnsureCStringKeyMapStorage<CMapStringToPtr, void*, void*>(this); CString actualKey; if (!state.data.LookupKey(key, actualKey)) return FALSE; state.lookupKeyScratch = actualKey; rKey = static_cast<const wchar_t*>(state.lookupKeyScratch); return TRUE; }
BOOL CMapStringToPtr::LookupKey(const wchar_t* key, const wchar_t*& rKey) const { return LookupKey(NormalizeStringKey(key), rKey); }
void*& CMapStringToPtr::operator[](const CString& key) { std::lock_guard<std::mutex> lock(g_collectionStateMutex); return EnsureCStringKeyMapStorage<CMapStringToPtr, void*, void*>(this).data[key]; }
void*& CMapStringToPtr::operator[](const wchar_t* key) { return (*this)[NormalizeStringKey(key)]; }
const void* CMapStringToPtr::operator[](const wchar_t* key) const { void* value = nullptr; Lookup(key, value); return value; }
void CMapStringToPtr::SetAt(const CString& key, void* newValue) { std::lock_guard<std::mutex> lock(g_collectionStateMutex); EnsureCStringKeyMapStorage<CMapStringToPtr, void*, void*>(this).data.SetAt(key, newValue); }
void CMapStringToPtr::SetAt(const wchar_t* key, void* newValue) { SetAt(NormalizeStringKey(key), newValue); }
BOOL CMapStringToPtr::RemoveKey(const CString& key) { std::lock_guard<std::mutex> lock(g_collectionStateMutex); auto& state = EnsureCStringKeyMapStorage<CMapStringToPtr, void*, void*>(this); return state.data.RemoveKey(key) ? TRUE : FALSE; }
BOOL CMapStringToPtr::RemoveKey(const wchar_t* key) { return RemoveKey(NormalizeStringKey(key)); }
void CMapStringToPtr::RemoveAll() { std::lock_guard<std::mutex> lock(g_collectionStateMutex); EnsureCStringKeyMapStorage<CMapStringToPtr, void*, void*>(this).data.RemoveAll(); }
CMapStringToPtr::POSITION CMapStringToPtr::GetStartPosition() const { std::lock_guard<std::mutex> lock(g_collectionStateMutex); const auto* state = FindCStringKeyMapStorage<CMapStringToPtr, void*, void*>(this); return state ? state->data.GetStartPosition() : POSITION(nullptr, 0); }
void CMapStringToPtr::GetNextAssoc(POSITION& rNextPosition, CString& rKey, void*& rValue) const { std::lock_guard<std::mutex> lock(g_collectionStateMutex); EnsureCStringKeyMapStorage<CMapStringToPtr, void*, void*>(this).data.GetNextAssoc(rNextPosition, rKey, rValue); }
UINT CMapStringToPtr::GetHashTableSize() const { std::lock_guard<std::mutex> lock(g_collectionStateMutex); const auto* state = FindCStringKeyMapStorage<CMapStringToPtr, void*, void*>(this); return state ? state->data.GetHashTableSize() : 17; }
void CMapStringToPtr::InitHashTable(UINT hashSize, BOOL bAllocNow) { std::lock_guard<std::mutex> lock(g_collectionStateMutex); EnsureCStringKeyMapStorage<CMapStringToPtr, void*, void*>(this).data.InitHashTable(hashSize, bAllocNow != FALSE); }
void CMapStringToPtr::Serialize(CArchive& ar) { std::lock_guard<std::mutex> lock(g_collectionStateMutex); auto& state = EnsureCStringKeyMapStorage<CMapStringToPtr, void*, void*>(this); if (ar.IsStoring()) ar << state.data; else ar >> state.data; }

CMapStringToString::CMapStringToString(INT_PTR nBlockSize) { std::lock_guard<std::mutex> lock(g_collectionStateMutex); EnsureCStringMapStringStorage(this, ClampCollectionBlockSize(nBlockSize)); }
CMapStringToString::~CMapStringToString() { std::lock_guard<std::mutex> lock(g_collectionStateMutex); RemoveCollectionState<CMapStringToString, CStringMapStringState>(this); }
INT_PTR CMapStringToString::GetCount() const { std::lock_guard<std::mutex> lock(g_collectionStateMutex); const auto* state = FindCStringMapStringStorage(this); return state ? state->data.GetCount() : 0; }
BOOL CMapStringToString::IsEmpty() const { return GetCount() == 0; }
BOOL CMapStringToString::Lookup(const CString& key, CString& rValue) const { std::lock_guard<std::mutex> lock(g_collectionStateMutex); const auto* state = FindCStringMapStringStorage(this); return (state && state->data.Lookup(key, rValue)) ? TRUE : FALSE; }
BOOL CMapStringToString::Lookup(const wchar_t* key, CString& rValue) const { return Lookup(NormalizeStringKey(key), rValue); }
BOOL CMapStringToString::LookupKey(const CString& key, const wchar_t*& rKey) const { std::lock_guard<std::mutex> lock(g_collectionStateMutex); auto& state = EnsureCStringMapStringStorage(this); CString actualKey; if (!state.data.LookupKey(key, actualKey)) return FALSE; state.lookupKeyScratch = actualKey; rKey = static_cast<const wchar_t*>(state.lookupKeyScratch); return TRUE; }
BOOL CMapStringToString::LookupKey(const wchar_t* key, const wchar_t*& rKey) const { return LookupKey(NormalizeStringKey(key), rKey); }
CString& CMapStringToString::operator[](const CString& key) { std::lock_guard<std::mutex> lock(g_collectionStateMutex); auto& state = EnsureCStringMapStringStorage(this); state.MarkDirty(); return state.data[key]; }
CString& CMapStringToString::operator[](const wchar_t* key) { return (*this)[NormalizeStringKey(key)]; }
const CString& CMapStringToString::operator[](const wchar_t* key) const { return const_cast<CMapStringToString*>(this)->operator[](key); }
void CMapStringToString::SetAt(const CString& key, const CString& newValue) { std::lock_guard<std::mutex> lock(g_collectionStateMutex); auto& state = EnsureCStringMapStringStorage(this); state.data.SetAt(key, newValue); state.MarkDirty(); }
void CMapStringToString::SetAt(const wchar_t* key, const CString& newValue) { SetAt(NormalizeStringKey(key), newValue); }
void CMapStringToString::SetAt(const wchar_t* key, const wchar_t* newValue) { SetAt(NormalizeStringKey(key), NormalizeStringKey(newValue)); }
BOOL CMapStringToString::RemoveKey(const CString& key) { std::lock_guard<std::mutex> lock(g_collectionStateMutex); auto& state = EnsureCStringMapStringStorage(this); BOOL removed = state.data.RemoveKey(key) ? TRUE : FALSE; if (removed) state.MarkDirty(); return removed; }
BOOL CMapStringToString::RemoveKey(const wchar_t* key) { return RemoveKey(NormalizeStringKey(key)); }
void CMapStringToString::RemoveAll() { std::lock_guard<std::mutex> lock(g_collectionStateMutex); auto& state = EnsureCStringMapStringStorage(this); state.data.RemoveAll(); state.MarkDirty(); }
CMapStringToString::POSITION CMapStringToString::GetStartPosition() const { std::lock_guard<std::mutex> lock(g_collectionStateMutex); const auto* state = FindCStringMapStringStorage(this); return state ? state->data.GetStartPosition() : POSITION(nullptr, 0); }
void CMapStringToString::GetNextAssoc(POSITION& rNextPosition, CString& rKey, CString& rValue) const { std::lock_guard<std::mutex> lock(g_collectionStateMutex); EnsureCStringMapStringStorage(this).data.GetNextAssoc(rNextPosition, rKey, rValue); }
UINT CMapStringToString::GetHashTableSize() const { std::lock_guard<std::mutex> lock(g_collectionStateMutex); const auto* state = FindCStringMapStringStorage(this); return state ? state->data.GetHashTableSize() : 17; }
void CMapStringToString::InitHashTable(UINT hashSize, BOOL bAllocNow) { std::lock_guard<std::mutex> lock(g_collectionStateMutex); auto& state = EnsureCStringMapStringStorage(this); state.data.InitHashTable(hashSize, bAllocNow != FALSE); state.MarkDirty(); }
CMapStringToString::CPair* CMapStringToString::PLookup(const wchar_t* key) { std::lock_guard<std::mutex> lock(g_collectionStateMutex); return EnsureCStringMapStringStorage(this).PLookup(key); }
const CMapStringToString::CPair* CMapStringToString::PLookup(const wchar_t* key) const { std::lock_guard<std::mutex> lock(g_collectionStateMutex); return EnsureCStringMapStringStorage(this).PLookup(key); }
CMapStringToString::CPair* CMapStringToString::PGetFirstAssoc() { std::lock_guard<std::mutex> lock(g_collectionStateMutex); auto& state = EnsureCStringMapStringStorage(this); state.EnsurePairs(); return state.pairs.empty() ? nullptr : state.pairs.front().get(); }
const CMapStringToString::CPair* CMapStringToString::PGetFirstAssoc() const { return const_cast<CMapStringToString*>(this)->PGetFirstAssoc(); }
CMapStringToString::CPair* CMapStringToString::PGetNextAssoc(const CPair* pAssocRet) { std::lock_guard<std::mutex> lock(g_collectionStateMutex); auto& state = EnsureCStringMapStringStorage(this); state.EnsurePairs(); if (!pAssocRet) return nullptr; for (size_t i = 0; i < state.pairs.size(); ++i) { if (state.pairs[i].get() == pAssocRet) return (i + 1 < state.pairs.size()) ? state.pairs[i + 1].get() : nullptr; } return nullptr; }
const CMapStringToString::CPair* CMapStringToString::PGetNextAssoc(const CPair* pAssocRet) const { return const_cast<CMapStringToString*>(this)->PGetNextAssoc(pAssocRet); }
void CMapStringToString::Serialize(CArchive& ar) { std::lock_guard<std::mutex> lock(g_collectionStateMutex); auto& state = EnsureCStringMapStringStorage(this); if (ar.IsStoring()) ar << state.data; else { ar >> state.data; state.MarkDirty(); } }

#undef OPENMFC_DEFINE_LIST_METHODS
#undef OPENMFC_DEFINE_MAP_METHODS
#undef OPENMFC_DEFINE_ARRAY_METHODS

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

// Symbol: ??0CUIntArray@@QEAA@XZ
OPENMFC_WRAP_CTOR0(impl___0CUIntArray__QEAA_XZ, CUIntArray)
// Symbol: ??1CUIntArray@@UEAA@XZ
OPENMFC_WRAP_DTOR(impl___1CUIntArray__UEAA_XZ, CUIntArray)
// Symbol: ?Append@CUIntArray@@QEAA_JAEBV1@@Z
extern "C" long long MS_ABI impl__Append_CUIntArray__QEAA_JAEBV1__Z(CUIntArray* pThis, const CUIntArray* pSrc) { return (pThis && pSrc) ? pThis->Append(*pSrc) : 0; }
// Symbol: ?Copy@CUIntArray@@QEAAXAEBV1@@Z
extern "C" void MS_ABI impl__Copy_CUIntArray__QEAAXAEBV1__Z(CUIntArray* pThis, const CUIntArray* pSrc) { if (pThis && pSrc) pThis->Copy(*pSrc); }
// Symbol: ?FreeExtra@CUIntArray@@QEAAXXZ
extern "C" void MS_ABI impl__FreeExtra_CUIntArray__QEAAXXZ(CUIntArray* pThis) { if (pThis) pThis->FreeExtra(); }
// Symbol: ?GetRuntimeClass@CUIntArray@@UEBAPEAUCRuntimeClass@@XZ
OPENMFC_WRAP_GETRUNTIMECLASS(impl__GetRuntimeClass_CUIntArray__UEBAPEAUCRuntimeClass__XZ, CUIntArray)
// Symbol: ?GetThisClass@CUIntArray@@SAPEAUCRuntimeClass@@XZ
OPENMFC_WRAP_GETTHISCLASS(impl__GetThisClass_CUIntArray__SAPEAUCRuntimeClass__XZ, CUIntArray)
// Symbol: ?InsertAt@CUIntArray@@QEAAX_JI0@Z
extern "C" void MS_ABI impl__InsertAt_CUIntArray__QEAAX_JI0_Z(CUIntArray* pThis, long long nIndex, unsigned int value, long long nCount) { if (pThis) pThis->InsertAt(nIndex, value, nCount); }
// Symbol: ?InsertAt@CUIntArray@@QEAAX_JPEAV1@@Z
extern "C" void MS_ABI impl__InsertAt_CUIntArray__QEAAX_JPEAV1__Z(CUIntArray* pThis, long long nIndex, CUIntArray* pNewArray) { if (pThis) pThis->InsertAt(nIndex, pNewArray); }
// Symbol: ?RemoveAt@CUIntArray@@QEAAX_J0@Z
extern "C" void MS_ABI impl__RemoveAt_CUIntArray__QEAAX_J0_Z(CUIntArray* pThis, long long nIndex, long long nCount) { if (pThis) pThis->RemoveAt(nIndex, nCount); }
// Symbol: ?SetAtGrow@CUIntArray@@QEAAX_JI@Z
extern "C" void MS_ABI impl__SetAtGrow_CUIntArray__QEAAX_JI_Z(CUIntArray* pThis, long long nIndex, unsigned int value) { if (pThis) pThis->SetAtGrow(nIndex, value); }
// Symbol: ?SetSize@CUIntArray@@QEAAX_J0@Z
extern "C" void MS_ABI impl__SetSize_CUIntArray__QEAAX_J0_Z(CUIntArray* pThis, long long nNewSize, long long nGrowBy) { if (pThis) pThis->SetSize(nNewSize, nGrowBy); }

// Symbol: ??0CDWordArray@@QEAA@XZ
OPENMFC_WRAP_CTOR0(impl___0CDWordArray__QEAA_XZ, CDWordArray)
// Symbol: ??1CDWordArray@@UEAA@XZ
OPENMFC_WRAP_DTOR(impl___1CDWordArray__UEAA_XZ, CDWordArray)
// Symbol: ??5@YAAEAVCArchive@@AEAV0@AEAPEAVCDWordArray@@@Z
OPENMFC_WRAP_SERIAL_EXTRACT(impl___5_YAAEAVCArchive__AEAV0_AEAPEAVCDWordArray___Z, CDWordArray)
// Symbol: ?Append@CDWordArray@@QEAA_JAEBV1@@Z
extern "C" long long MS_ABI impl__Append_CDWordArray__QEAA_JAEBV1__Z(CDWordArray* pThis, const CDWordArray* pSrc) { return (pThis && pSrc) ? pThis->Append(*pSrc) : 0; }
// Symbol: ?Copy@CDWordArray@@QEAAXAEBV1@@Z
extern "C" void MS_ABI impl__Copy_CDWordArray__QEAAXAEBV1__Z(CDWordArray* pThis, const CDWordArray* pSrc) { if (pThis && pSrc) pThis->Copy(*pSrc); }
// Symbol: ?CreateObject@CDWordArray@@SAPEAVCObject@@XZ
OPENMFC_WRAP_CREATEOBJECT(impl__CreateObject_CDWordArray__SAPEAVCObject__XZ, CDWordArray)
// Symbol: ?FreeExtra@CDWordArray@@QEAAXXZ
extern "C" void MS_ABI impl__FreeExtra_CDWordArray__QEAAXXZ(CDWordArray* pThis) { if (pThis) pThis->FreeExtra(); }
// Symbol: ?GetRuntimeClass@CDWordArray@@UEBAPEAUCRuntimeClass@@XZ
OPENMFC_WRAP_GETRUNTIMECLASS(impl__GetRuntimeClass_CDWordArray__UEBAPEAUCRuntimeClass__XZ, CDWordArray)
// Symbol: ?GetThisClass@CDWordArray@@SAPEAUCRuntimeClass@@XZ
OPENMFC_WRAP_GETTHISCLASS(impl__GetThisClass_CDWordArray__SAPEAUCRuntimeClass__XZ, CDWordArray)
// Symbol: ?InsertAt@CDWordArray@@QEAAX_JK0@Z
extern "C" void MS_ABI impl__InsertAt_CDWordArray__QEAAX_JK0_Z(CDWordArray* pThis, long long nIndex, unsigned long value, long long nCount) { if (pThis) pThis->InsertAt(nIndex, value, nCount); }
// Symbol: ?InsertAt@CDWordArray@@QEAAX_JPEAV1@@Z
extern "C" void MS_ABI impl__InsertAt_CDWordArray__QEAAX_JPEAV1__Z(CDWordArray* pThis, long long nIndex, CDWordArray* pNewArray) { if (pThis) pThis->InsertAt(nIndex, pNewArray); }
// Symbol: ?RemoveAt@CDWordArray@@QEAAX_J0@Z
extern "C" void MS_ABI impl__RemoveAt_CDWordArray__QEAAX_J0_Z(CDWordArray* pThis, long long nIndex, long long nCount) { if (pThis) pThis->RemoveAt(nIndex, nCount); }
// Symbol: ?Serialize@CDWordArray@@UEAAXAEAVCArchive@@@Z
extern "C" void MS_ABI impl__Serialize_CDWordArray__UEAAXAEAVCArchive___Z(CDWordArray* pThis, CArchive& ar) { if (pThis) pThis->Serialize(ar); }
// Symbol: ?SetAtGrow@CDWordArray@@QEAAX_JK@Z
extern "C" void MS_ABI impl__SetAtGrow_CDWordArray__QEAAX_JK_Z(CDWordArray* pThis, long long nIndex, unsigned long value) { if (pThis) pThis->SetAtGrow(nIndex, value); }
// Symbol: ?SetSize@CDWordArray@@QEAAX_J0@Z
extern "C" void MS_ABI impl__SetSize_CDWordArray__QEAAX_J0_Z(CDWordArray* pThis, long long nNewSize, long long nGrowBy) { if (pThis) pThis->SetSize(nNewSize, nGrowBy); }

// Symbol: ??0CPtrArray@@QEAA@XZ
OPENMFC_WRAP_CTOR0(impl___0CPtrArray__QEAA_XZ, CPtrArray)
// Symbol: ??1CPtrArray@@UEAA@XZ
OPENMFC_WRAP_DTOR(impl___1CPtrArray__UEAA_XZ, CPtrArray)
// Symbol: ?Append@CPtrArray@@QEAA_JAEBV1@@Z
extern "C" long long MS_ABI impl__Append_CPtrArray__QEAA_JAEBV1__Z(CPtrArray* pThis, const CPtrArray* pSrc) { return (pThis && pSrc) ? pThis->Append(*pSrc) : 0; }
// Symbol: ?Copy@CPtrArray@@QEAAXAEBV1@@Z
extern "C" void MS_ABI impl__Copy_CPtrArray__QEAAXAEBV1__Z(CPtrArray* pThis, const CPtrArray* pSrc) { if (pThis && pSrc) pThis->Copy(*pSrc); }
// Symbol: ?FreeExtra@CPtrArray@@QEAAXXZ
extern "C" void MS_ABI impl__FreeExtra_CPtrArray__QEAAXXZ(CPtrArray* pThis) { if (pThis) pThis->FreeExtra(); }
// Symbol: ?GetRuntimeClass@CPtrArray@@UEBAPEAUCRuntimeClass@@XZ
OPENMFC_WRAP_GETRUNTIMECLASS(impl__GetRuntimeClass_CPtrArray__UEBAPEAUCRuntimeClass__XZ, CPtrArray)
// Symbol: ?GetThisClass@CPtrArray@@SAPEAUCRuntimeClass@@XZ
OPENMFC_WRAP_GETTHISCLASS(impl__GetThisClass_CPtrArray__SAPEAUCRuntimeClass__XZ, CPtrArray)
// Symbol: ?InsertAt@CPtrArray@@QEAAX_JPEAV1@@Z
extern "C" void MS_ABI impl__InsertAt_CPtrArray__QEAAX_JPEAV1__Z(CPtrArray* pThis, long long nIndex, CPtrArray* pNewArray) { if (pThis) pThis->InsertAt(nIndex, pNewArray); }
// Symbol: ?InsertAt@CPtrArray@@QEAAX_JPEAX0@Z
extern "C" void MS_ABI impl__InsertAt_CPtrArray__QEAAX_JPEAX0_Z(CPtrArray* pThis, long long nIndex, void* value, long long nCount) { if (pThis) pThis->InsertAt(nIndex, value, nCount); }
// Symbol: ?RemoveAt@CPtrArray@@QEAAX_J0@Z
extern "C" void MS_ABI impl__RemoveAt_CPtrArray__QEAAX_J0_Z(CPtrArray* pThis, long long nIndex, long long nCount) { if (pThis) pThis->RemoveAt(nIndex, nCount); }
// Symbol: ?SetAtGrow@CPtrArray@@QEAAX_JPEAX@Z
extern "C" void MS_ABI impl__SetAtGrow_CPtrArray__QEAAX_JPEAX_Z(CPtrArray* pThis, long long nIndex, void* value) { if (pThis) pThis->SetAtGrow(nIndex, value); }
// Symbol: ?SetSize@CPtrArray@@QEAAX_J0@Z
extern "C" void MS_ABI impl__SetSize_CPtrArray__QEAAX_J0_Z(CPtrArray* pThis, long long nNewSize, long long nGrowBy) { if (pThis) pThis->SetSize(nNewSize, nGrowBy); }

// Symbol: ??0CByteArray@@QEAA@XZ
OPENMFC_WRAP_CTOR0(impl___0CByteArray__QEAA_XZ, CByteArray)
// Symbol: ??1CByteArray@@UEAA@XZ
OPENMFC_WRAP_DTOR(impl___1CByteArray__UEAA_XZ, CByteArray)
// Symbol: ??5@YAAEAVCArchive@@AEAV0@AEAPEAVCByteArray@@@Z
OPENMFC_WRAP_SERIAL_EXTRACT(impl___5_YAAEAVCArchive__AEAV0_AEAPEAVCByteArray___Z, CByteArray)
// Symbol: ?Append@CByteArray@@QEAA_JAEBV1@@Z
extern "C" long long MS_ABI impl__Append_CByteArray__QEAA_JAEBV1__Z(CByteArray* pThis, const CByteArray* pSrc) { return (pThis && pSrc) ? pThis->Append(*pSrc) : 0; }
// Symbol: ?Copy@CByteArray@@QEAAXAEBV1@@Z
extern "C" void MS_ABI impl__Copy_CByteArray__QEAAXAEBV1__Z(CByteArray* pThis, const CByteArray* pSrc) { if (pThis && pSrc) pThis->Copy(*pSrc); }
// Symbol: ?CreateObject@CByteArray@@SAPEAVCObject@@XZ
OPENMFC_WRAP_CREATEOBJECT(impl__CreateObject_CByteArray__SAPEAVCObject__XZ, CByteArray)
// Symbol: ?FreeExtra@CByteArray@@QEAAXXZ
extern "C" void MS_ABI impl__FreeExtra_CByteArray__QEAAXXZ(CByteArray* pThis) { if (pThis) pThis->FreeExtra(); }
// Symbol: ?GetRuntimeClass@CByteArray@@UEBAPEAUCRuntimeClass@@XZ
OPENMFC_WRAP_GETRUNTIMECLASS(impl__GetRuntimeClass_CByteArray__UEBAPEAUCRuntimeClass__XZ, CByteArray)
// Symbol: ?GetThisClass@CByteArray@@SAPEAUCRuntimeClass@@XZ
OPENMFC_WRAP_GETTHISCLASS(impl__GetThisClass_CByteArray__SAPEAUCRuntimeClass__XZ, CByteArray)
// Symbol: ?InsertAt@CByteArray@@QEAAX_JE0@Z
extern "C" void MS_ABI impl__InsertAt_CByteArray__QEAAX_JE0_Z(CByteArray* pThis, long long nIndex, unsigned char value, long long nCount) { if (pThis) pThis->InsertAt(nIndex, value, nCount); }
// Symbol: ?InsertAt@CByteArray@@QEAAX_JPEAV1@@Z
extern "C" void MS_ABI impl__InsertAt_CByteArray__QEAAX_JPEAV1__Z(CByteArray* pThis, long long nIndex, CByteArray* pNewArray) { if (pThis) pThis->InsertAt(nIndex, pNewArray); }
// Symbol: ?RemoveAt@CByteArray@@QEAAX_J0@Z
extern "C" void MS_ABI impl__RemoveAt_CByteArray__QEAAX_J0_Z(CByteArray* pThis, long long nIndex, long long nCount) { if (pThis) pThis->RemoveAt(nIndex, nCount); }
// Symbol: ?Serialize@CByteArray@@UEAAXAEAVCArchive@@@Z
extern "C" void MS_ABI impl__Serialize_CByteArray__UEAAXAEAVCArchive___Z(CByteArray* pThis, CArchive& ar) { if (pThis) pThis->Serialize(ar); }
// Symbol: ?SetAtGrow@CByteArray@@QEAAX_JE@Z
extern "C" void MS_ABI impl__SetAtGrow_CByteArray__QEAAX_JE_Z(CByteArray* pThis, long long nIndex, unsigned char value) { if (pThis) pThis->SetAtGrow(nIndex, value); }
// Symbol: ?SetSize@CByteArray@@QEAAX_J0@Z
extern "C" void MS_ABI impl__SetSize_CByteArray__QEAAX_J0_Z(CByteArray* pThis, long long nNewSize, long long nGrowBy) { if (pThis) pThis->SetSize(nNewSize, nGrowBy); }

// Symbol: ??0CObArray@@QEAA@XZ
OPENMFC_WRAP_CTOR0(impl___0CObArray__QEAA_XZ, CObArray)
// Symbol: ??1CObArray@@UEAA@XZ
OPENMFC_WRAP_DTOR(impl___1CObArray__UEAA_XZ, CObArray)
// Symbol: ??5@YAAEAVCArchive@@AEAV0@AEAPEAVCObArray@@@Z
OPENMFC_WRAP_SERIAL_EXTRACT(impl___5_YAAEAVCArchive__AEAV0_AEAPEAVCObArray___Z, CObArray)
// Symbol: ?Append@CObArray@@QEAA_JAEBV1@@Z
extern "C" long long MS_ABI impl__Append_CObArray__QEAA_JAEBV1__Z(CObArray* pThis, const CObArray* pSrc) { return (pThis && pSrc) ? pThis->Append(*pSrc) : 0; }
// Symbol: ?Copy@CObArray@@QEAAXAEBV1@@Z
extern "C" void MS_ABI impl__Copy_CObArray__QEAAXAEBV1__Z(CObArray* pThis, const CObArray* pSrc) { if (pThis && pSrc) pThis->Copy(*pSrc); }
// Symbol: ?CreateObject@CObArray@@SAPEAVCObject@@XZ
OPENMFC_WRAP_CREATEOBJECT(impl__CreateObject_CObArray__SAPEAVCObject__XZ, CObArray)
// Symbol: ?FreeExtra@CObArray@@QEAAXXZ
extern "C" void MS_ABI impl__FreeExtra_CObArray__QEAAXXZ(CObArray* pThis) { if (pThis) pThis->FreeExtra(); }
// Symbol: ?GetRuntimeClass@CObArray@@UEBAPEAUCRuntimeClass@@XZ
OPENMFC_WRAP_GETRUNTIMECLASS(impl__GetRuntimeClass_CObArray__UEBAPEAUCRuntimeClass__XZ, CObArray)
// Symbol: ?GetThisClass@CObArray@@SAPEAUCRuntimeClass@@XZ
OPENMFC_WRAP_GETTHISCLASS(impl__GetThisClass_CObArray__SAPEAUCRuntimeClass__XZ, CObArray)
// Symbol: ?InsertAt@CObArray@@QEAAX_JPEAV1@@Z
extern "C" void MS_ABI impl__InsertAt_CObArray__QEAAX_JPEAV1__Z(CObArray* pThis, long long nIndex, CObArray* pNewArray) { if (pThis) pThis->InsertAt(nIndex, pNewArray); }
// Symbol: ?InsertAt@CObArray@@QEAAX_JPEAVCObject@@0@Z
extern "C" void MS_ABI impl__InsertAt_CObArray__QEAAX_JPEAVCObject__0_Z(CObArray* pThis, long long nIndex, CObject* value, long long nCount) { if (pThis) pThis->InsertAt(nIndex, value, nCount); }
// Symbol: ?RemoveAt@CObArray@@QEAAX_J0@Z
extern "C" void MS_ABI impl__RemoveAt_CObArray__QEAAX_J0_Z(CObArray* pThis, long long nIndex, long long nCount) { if (pThis) pThis->RemoveAt(nIndex, nCount); }
// Symbol: ?Serialize@CObArray@@UEAAXAEAVCArchive@@@Z
extern "C" void MS_ABI impl__Serialize_CObArray__UEAAXAEAVCArchive___Z(CObArray* pThis, CArchive& ar) { if (pThis) pThis->Serialize(ar); }
// Symbol: ?SetAtGrow@CObArray@@QEAAX_JPEAVCObject@@@Z
extern "C" void MS_ABI impl__SetAtGrow_CObArray__QEAAX_JPEAVCObject___Z(CObArray* pThis, long long nIndex, CObject* value) { if (pThis) pThis->SetAtGrow(nIndex, value); }
// Symbol: ?SetSize@CObArray@@QEAAX_J0@Z
extern "C" void MS_ABI impl__SetSize_CObArray__QEAAX_J0_Z(CObArray* pThis, long long nNewSize, long long nGrowBy) { if (pThis) pThis->SetSize(nNewSize, nGrowBy); }

// Symbol: ??0CStringArray@@QEAA@XZ
OPENMFC_WRAP_CTOR0(impl___0CStringArray__QEAA_XZ, CStringArray)
// Symbol: ??1CStringArray@@UEAA@XZ
OPENMFC_WRAP_DTOR(impl___1CStringArray__UEAA_XZ, CStringArray)
// Symbol: ??5@YAAEAVCArchive@@AEAV0@AEAPEAVCStringArray@@@Z
OPENMFC_WRAP_SERIAL_EXTRACT(impl___5_YAAEAVCArchive__AEAV0_AEAPEAVCStringArray___Z, CStringArray)
// Symbol: ?Append@CStringArray@@QEAA_JAEBV1@@Z
extern "C" long long MS_ABI impl__Append_CStringArray__QEAA_JAEBV1__Z(CStringArray* pThis, const CStringArray* pSrc) { return (pThis && pSrc) ? pThis->Append(*pSrc) : 0; }
// Symbol: ?Copy@CStringArray@@QEAAXAEBV1@@Z
extern "C" void MS_ABI impl__Copy_CStringArray__QEAAXAEBV1__Z(CStringArray* pThis, const CStringArray* pSrc) { if (pThis && pSrc) pThis->Copy(*pSrc); }
// Symbol: ?CreateObject@CStringArray@@SAPEAVCObject@@XZ
OPENMFC_WRAP_CREATEOBJECT(impl__CreateObject_CStringArray__SAPEAVCObject__XZ, CStringArray)
// Symbol: ?FreeExtra@CStringArray@@QEAAXXZ
extern "C" void MS_ABI impl__FreeExtra_CStringArray__QEAAXXZ(CStringArray* pThis) { if (pThis) pThis->FreeExtra(); }
// Symbol: ?GetRuntimeClass@CStringArray@@UEBAPEAUCRuntimeClass@@XZ
OPENMFC_WRAP_GETRUNTIMECLASS(impl__GetRuntimeClass_CStringArray__UEBAPEAUCRuntimeClass__XZ, CStringArray)
// Symbol: ?GetThisClass@CStringArray@@SAPEAUCRuntimeClass@@XZ
OPENMFC_WRAP_GETTHISCLASS(impl__GetThisClass_CStringArray__SAPEAUCRuntimeClass__XZ, CStringArray)
// Symbol: ?InsertAt@CStringArray@@QEAAX_JAEBV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@0@Z
extern "C" void MS_ABI impl__InsertAt_CStringArray__QEAAX_JAEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__0_Z(CStringArray* pThis, long long nIndex, const CString& value, long long nCount) { if (pThis) pThis->InsertAt(nIndex, value, nCount); }
// Symbol: ?InsertAt@CStringArray@@QEAAX_JPEB_W0@Z
extern "C" void MS_ABI impl__InsertAt_CStringArray__QEAAX_JPEB_W0_Z(CStringArray* pThis, long long nIndex, const wchar_t* value, long long nCount) { if (pThis) pThis->InsertAt(nIndex, value, nCount); }
// Symbol: ?InsertAt@CStringArray@@QEAAX_JPEBV1@@Z
extern "C" void MS_ABI impl__InsertAt_CStringArray__QEAAX_JPEBV1__Z(CStringArray* pThis, long long nIndex, CStringArray* pNewArray) { if (pThis) pThis->InsertAt(nIndex, pNewArray); }
// Symbol: ?InsertEmpty@CStringArray@@IEAAX_J0@Z
extern "C" void MS_ABI impl__InsertEmpty_CStringArray__IEAAX_J0_Z(CStringArray* pThis, long long nIndex, long long nCount) { if (pThis) pThis->InsertEmpty(nIndex, nCount); }
// Symbol: ?RemoveAt@CStringArray@@QEAAX_J0@Z
extern "C" void MS_ABI impl__RemoveAt_CStringArray__QEAAX_J0_Z(CStringArray* pThis, long long nIndex, long long nCount) { if (pThis) pThis->RemoveAt(nIndex, nCount); }
// Symbol: ?Serialize@CStringArray@@UEAAXAEAVCArchive@@@Z
extern "C" void MS_ABI impl__Serialize_CStringArray__UEAAXAEAVCArchive___Z(CStringArray* pThis, CArchive& ar) { if (pThis) pThis->Serialize(ar); }
// Symbol: ?SetAtGrow@CStringArray@@QEAAX_JAEBV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@@Z
extern "C" void MS_ABI impl__SetAtGrow_CStringArray__QEAAX_JAEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(CStringArray* pThis, long long nIndex, const CString& value) { if (pThis) pThis->SetAtGrow(nIndex, value); }
// Symbol: ?SetAtGrow@CStringArray@@QEAAX_JPEB_W@Z
extern "C" void MS_ABI impl__SetAtGrow_CStringArray__QEAAX_JPEB_W_Z(CStringArray* pThis, long long nIndex, const wchar_t* value) { if (pThis) pThis->SetAtGrow(nIndex, value); }
// Symbol: ?SetSize@CStringArray@@QEAAX_J0@Z
extern "C" void MS_ABI impl__SetSize_CStringArray__QEAAX_J0_Z(CStringArray* pThis, long long nNewSize, long long nGrowBy) { if (pThis) pThis->SetSize(nNewSize, nGrowBy); }

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

// Symbol: ??0CStringList@@QEAA@_J@Z
OPENMFC_WRAP_CTOR1(impl___0CStringList__QEAA__J_Z, CStringList, long long)
// Symbol: ??1CStringList@@UEAA@XZ
OPENMFC_WRAP_DTOR(impl___1CStringList__UEAA_XZ, CStringList)
// Symbol: ??5@YAAEAVCArchive@@AEAV0@AEAPEAVCStringList@@@Z
OPENMFC_WRAP_SERIAL_EXTRACT(impl___5_YAAEAVCArchive__AEAV0_AEAPEAVCStringList___Z, CStringList)
// Symbol: ?AddHead@CStringList@@QEAAPEAU__POSITION@@AEBV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@@Z
extern "C" CStringList::POSITION MS_ABI impl__AddHead_CStringList__QEAAPEAU__POSITION__AEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(CStringList* pThis, const CString& value) { return pThis ? pThis->AddHead(value) : CStringList::POSITION(nullptr); }
// Symbol: ?AddHead@CStringList@@QEAAPEAU__POSITION@@PEB_W@Z
extern "C" CStringList::POSITION MS_ABI impl__AddHead_CStringList__QEAAPEAU__POSITION__PEB_W_Z(CStringList* pThis, const wchar_t* value) { return pThis ? pThis->AddHead(value) : CStringList::POSITION(nullptr); }
// Symbol: ?AddHead@CStringList@@QEAAXPEAV1@@Z
extern "C" void MS_ABI impl__AddHead_CStringList__QEAAXPEAV1__Z(CStringList* pThis, CStringList* pNewList) { if (pThis) pThis->AddHead(pNewList); }
// Symbol: ?AddTail@CStringList@@QEAAPEAU__POSITION@@AEBV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@@Z
extern "C" CStringList::POSITION MS_ABI impl__AddTail_CStringList__QEAAPEAU__POSITION__AEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(CStringList* pThis, const CString& value) { return pThis ? pThis->AddTail(value) : CStringList::POSITION(nullptr); }
// Symbol: ?AddTail@CStringList@@QEAAPEAU__POSITION@@PEB_W@Z
extern "C" CStringList::POSITION MS_ABI impl__AddTail_CStringList__QEAAPEAU__POSITION__PEB_W_Z(CStringList* pThis, const wchar_t* value) { return pThis ? pThis->AddTail(value) : CStringList::POSITION(nullptr); }
// Symbol: ?AddTail@CStringList@@QEAAXPEAV1@@Z
extern "C" void MS_ABI impl__AddTail_CStringList__QEAAXPEAV1__Z(CStringList* pThis, CStringList* pNewList) { if (pThis) pThis->AddTail(pNewList); }
// Symbol: ?CreateObject@CStringList@@SAPEAVCObject@@XZ
OPENMFC_WRAP_CREATEOBJECT(impl__CreateObject_CStringList__SAPEAVCObject__XZ, CStringList)
// Symbol: ?Find@CStringList@@QEBAPEAU__POSITION@@PEB_WPEAU2@@Z
extern "C" CStringList::POSITION MS_ABI impl__Find_CStringList__QEBAPEAU__POSITION__PEB_WPEAU2__Z(const CStringList* pThis, const wchar_t* value, CStringList::POSITION* pStartAfter) { return pThis ? pThis->Find(value, pStartAfter ? *pStartAfter : CStringList::POSITION(nullptr)) : CStringList::POSITION(nullptr); }
// Symbol: ?FindIndex@CStringList@@QEBAPEAU__POSITION@@_J@Z
extern "C" CStringList::POSITION MS_ABI impl__FindIndex_CStringList__QEBAPEAU__POSITION____J_Z(const CStringList* pThis, long long nIndex) { return pThis ? pThis->FindIndex(nIndex) : CStringList::POSITION(nullptr); }
extern "C" CStringList::POSITION MS_ABI impl__FindIndex_CStringList__QEBAPEAU__POSITION___J_Z(const CStringList* pThis, long long nIndex) { return impl__FindIndex_CStringList__QEBAPEAU__POSITION____J_Z(pThis, nIndex); }
// Symbol: ?FreeNode@CStringList@@IEAAXPEAUCNode@1@@Z
extern "C" void MS_ABI impl__FreeNode_CStringList__IEAAXPEAUCNode_1___Z(CStringList* /*pThis*/, void* pNode) { delete static_cast<ListNodeSnapshot<CString>*>(pNode); }
extern "C" void MS_ABI impl__FreeNode_CStringList__IEAAXPEAUCNode_1__Z(CStringList* pThis, void* pNode) { impl__FreeNode_CStringList__IEAAXPEAUCNode_1___Z(pThis, pNode); }
// Symbol: ?GetRuntimeClass@CStringList@@UEBAPEAUCRuntimeClass@@XZ
OPENMFC_WRAP_GETRUNTIMECLASS(impl__GetRuntimeClass_CStringList__UEBAPEAUCRuntimeClass__XZ, CStringList)
// Symbol: ?GetThisClass@CStringList@@SAPEAUCRuntimeClass@@XZ
OPENMFC_WRAP_GETTHISCLASS(impl__GetThisClass_CStringList__SAPEAUCRuntimeClass__XZ, CStringList)
// Symbol: ?InsertAfter@CStringList@@QEAAPEAU__POSITION@@PEAU2@AEBV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@@Z
extern "C" CStringList::POSITION MS_ABI impl__InsertAfter_CStringList__QEAAPEAU__POSITION__PEAU2_AEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(CStringList* pThis, CStringList::POSITION* pPos, const CString& value) { return pThis ? pThis->InsertAfter(pPos ? *pPos : CStringList::POSITION(nullptr), value) : CStringList::POSITION(nullptr); }
// Symbol: ?InsertAfter@CStringList@@QEAAPEAU__POSITION@@PEAU2@PEB_W@Z
extern "C" CStringList::POSITION MS_ABI impl__InsertAfter_CStringList__QEAAPEAU__POSITION__PEAU2_PEB_W_Z(CStringList* pThis, CStringList::POSITION* pPos, const wchar_t* value) { return pThis ? pThis->InsertAfter(pPos ? *pPos : CStringList::POSITION(nullptr), value) : CStringList::POSITION(nullptr); }
// Symbol: ?InsertBefore@CStringList@@QEAAPEAU__POSITION@@PEAU2@AEBV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@@Z
extern "C" CStringList::POSITION MS_ABI impl__InsertBefore_CStringList__QEAAPEAU__POSITION__PEAU2_AEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(CStringList* pThis, CStringList::POSITION* pPos, const CString& value) { return pThis ? pThis->InsertBefore(pPos ? *pPos : CStringList::POSITION(nullptr), value) : CStringList::POSITION(nullptr); }
// Symbol: ?InsertBefore@CStringList@@QEAAPEAU__POSITION@@PEAU2@PEB_W@Z
extern "C" CStringList::POSITION MS_ABI impl__InsertBefore_CStringList__QEAAPEAU__POSITION__PEAU2_PEB_W_Z(CStringList* pThis, CStringList::POSITION* pPos, const wchar_t* value) { return pThis ? pThis->InsertBefore(pPos ? *pPos : CStringList::POSITION(nullptr), value) : CStringList::POSITION(nullptr); }
// Symbol: ?NewNode@CStringList@@IEAAPEAUCNode@1@PEAU21@0@Z
extern "C" void* MS_ABI impl__NewNode_CStringList__IEAAPEAUCNode_1__PEAU21_0_Z(CStringList* /*pThis*/, void* pPrev, void* pNext) { auto* node = new ListNodeSnapshot<CString>(); node->pPrev = static_cast<ListNodeSnapshot<CString>*>(pPrev); node->pNext = static_cast<ListNodeSnapshot<CString>*>(pNext); return node; }
extern "C" void* MS_ABI impl__NewNode_CStringList__IEAAPEAUCNode_1_PEAU21_0_Z(CStringList* pThis, void* pPrev, void* pNext) { return impl__NewNode_CStringList__IEAAPEAUCNode_1__PEAU21_0_Z(pThis, pPrev, pNext); }
// Symbol: ?RemoveAll@CStringList@@QEAAXXZ
extern "C" void MS_ABI impl__RemoveAll_CStringList__QEAAXXZ(CStringList* pThis) { if (pThis) pThis->RemoveAll(); }
// Symbol: ?RemoveAt@CStringList@@QEAAXPEAU__POSITION@@@Z
extern "C" void MS_ABI impl__RemoveAt_CStringList__QEAAXPEAU__POSITION___Z(CStringList* pThis, CStringList::POSITION* pPos) { if (pThis && pPos) pThis->RemoveAt(*pPos); }
// Symbol: ?RemoveHead@CStringList@@QEAA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@XZ
extern "C" void MS_ABI impl__RemoveHead_CStringList__QEAA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__XZ(CStringList* pThis, void* ret) { new (ret) CString(pThis ? pThis->RemoveHead() : CString()); }
// Symbol: ?RemoveTail@CStringList@@QEAA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@XZ
extern "C" void MS_ABI impl__RemoveTail_CStringList__QEAA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__XZ(CStringList* pThis, void* ret) { new (ret) CString(pThis ? pThis->RemoveTail() : CString()); }
// Symbol: ?Serialize@CStringList@@UEAAXAEAVCArchive@@@Z
extern "C" void MS_ABI impl__Serialize_CStringList__UEAAXAEAVCArchive___Z(CStringList* pThis, CArchive& ar) { if (pThis) pThis->Serialize(ar); }

// Symbol: ??0CObList@@QEAA@_J@Z
OPENMFC_WRAP_CTOR1(impl___0CObList__QEAA__J_Z, CObList, long long)
// Symbol: ??1CObList@@UEAA@XZ
OPENMFC_WRAP_DTOR(impl___1CObList__UEAA_XZ, CObList)
// Symbol: ??5@YAAEAVCArchive@@AEAV0@AEAPEAVCObList@@@Z
OPENMFC_WRAP_SERIAL_EXTRACT(impl___5_YAAEAVCArchive__AEAV0_AEAPEAVCObList___Z, CObList)
// Symbol: ?AddHead@CObList@@QEAAPEAU__POSITION@@PEAVCObject@@@Z
extern "C" CObList::POSITION MS_ABI impl__AddHead_CObList__QEAAPEAU__POSITION__PEAVCObject___Z(CObList* pThis, CObject* value) { return pThis ? pThis->AddHead(value) : CObList::POSITION(nullptr); }
// Symbol: ?AddHead@CObList@@QEAAXPEAV1@@Z
extern "C" void MS_ABI impl__AddHead_CObList__QEAAXPEAV1__Z(CObList* pThis, CObList* pNewList) { if (pThis) pThis->AddHead(pNewList); }
// Symbol: ?AddTail@CObList@@QEAAPEAU__POSITION@@PEAVCObject@@@Z
extern "C" CObList::POSITION MS_ABI impl__AddTail_CObList__QEAAPEAU__POSITION__PEAVCObject___Z(CObList* pThis, CObject* value) { return pThis ? pThis->AddTail(value) : CObList::POSITION(nullptr); }
// Symbol: ?AddTail@CObList@@QEAAXPEAV1@@Z
extern "C" void MS_ABI impl__AddTail_CObList__QEAAXPEAV1__Z(CObList* pThis, CObList* pNewList) { if (pThis) pThis->AddTail(pNewList); }
// Symbol: ?CreateObject@CObList@@SAPEAVCObject@@XZ
OPENMFC_WRAP_CREATEOBJECT(impl__CreateObject_CObList__SAPEAVCObject__XZ, CObList)
// Symbol: ?Find@CObList@@QEBAPEAU__POSITION@@PEAVCObject@@PEAU2@@Z
extern "C" CObList::POSITION MS_ABI impl__Find_CObList__QEBAPEAU__POSITION__PEAVCObject__PEAU2__Z(const CObList* pThis, CObject* value, CObList::POSITION* pStartAfter) { return pThis ? pThis->Find(value, pStartAfter ? *pStartAfter : CObList::POSITION(nullptr)) : CObList::POSITION(nullptr); }
// Symbol: ?FindIndex@CObList@@QEBAPEAU__POSITION@@_J@Z
extern "C" CObList::POSITION MS_ABI impl__FindIndex_CObList__QEBAPEAU__POSITION____J_Z(const CObList* pThis, long long nIndex) { return pThis ? pThis->FindIndex(nIndex) : CObList::POSITION(nullptr); }
extern "C" CObList::POSITION MS_ABI impl__FindIndex_CObList__QEBAPEAU__POSITION___J_Z(const CObList* pThis, long long nIndex) { return impl__FindIndex_CObList__QEBAPEAU__POSITION____J_Z(pThis, nIndex); }
// Symbol: ?FreeNode@CObList@@IEAAXPEAUCNode@1@@Z
extern "C" void MS_ABI impl__FreeNode_CObList__IEAAXPEAUCNode_1___Z(CObList* /*pThis*/, void* pNode) { delete static_cast<ListNodeSnapshot<CObject*>*>(pNode); }
extern "C" void MS_ABI impl__FreeNode_CObList__IEAAXPEAUCNode_1__Z(CObList* pThis, void* pNode) { impl__FreeNode_CObList__IEAAXPEAUCNode_1___Z(pThis, pNode); }
// Symbol: ?GetRuntimeClass@CObList@@UEBAPEAUCRuntimeClass@@XZ
OPENMFC_WRAP_GETRUNTIMECLASS(impl__GetRuntimeClass_CObList__UEBAPEAUCRuntimeClass__XZ, CObList)
// Symbol: ?GetThisClass@CObList@@SAPEAUCRuntimeClass@@XZ
OPENMFC_WRAP_GETTHISCLASS(impl__GetThisClass_CObList__SAPEAUCRuntimeClass__XZ, CObList)
// Symbol: ?InsertAfter@CObList@@QEAAPEAU__POSITION@@PEAU2@PEAVCObject@@@Z
extern "C" CObList::POSITION MS_ABI impl__InsertAfter_CObList__QEAAPEAU__POSITION__PEAU2_PEAVCObject___Z(CObList* pThis, CObList::POSITION* pPos, CObject* value) { return pThis ? pThis->InsertAfter(pPos ? *pPos : CObList::POSITION(nullptr), value) : CObList::POSITION(nullptr); }
// Symbol: ?InsertBefore@CObList@@QEAAPEAU__POSITION@@PEAU2@PEAVCObject@@@Z
extern "C" CObList::POSITION MS_ABI impl__InsertBefore_CObList__QEAAPEAU__POSITION__PEAU2_PEAVCObject___Z(CObList* pThis, CObList::POSITION* pPos, CObject* value) { return pThis ? pThis->InsertBefore(pPos ? *pPos : CObList::POSITION(nullptr), value) : CObList::POSITION(nullptr); }
// Symbol: ?NewNode@CObList@@IEAAPEAUCNode@1@PEAU21@0@Z
extern "C" void* MS_ABI impl__NewNode_CObList__IEAAPEAUCNode_1__PEAU21_0_Z(CObList* /*pThis*/, void* pPrev, void* pNext) { auto* node = new ListNodeSnapshot<CObject*>(); node->pPrev = static_cast<ListNodeSnapshot<CObject*>*>(pPrev); node->pNext = static_cast<ListNodeSnapshot<CObject*>*>(pNext); return node; }
extern "C" void* MS_ABI impl__NewNode_CObList__IEAAPEAUCNode_1_PEAU21_0_Z(CObList* pThis, void* pPrev, void* pNext) { return impl__NewNode_CObList__IEAAPEAUCNode_1__PEAU21_0_Z(pThis, pPrev, pNext); }
// Symbol: ?RemoveAll@CObList@@QEAAXXZ
extern "C" void MS_ABI impl__RemoveAll_CObList__QEAAXXZ(CObList* pThis) { if (pThis) pThis->RemoveAll(); }
// Symbol: ?RemoveAt@CObList@@QEAAXPEAU__POSITION@@@Z
extern "C" void MS_ABI impl__RemoveAt_CObList__QEAAXPEAU__POSITION___Z(CObList* pThis, CObList::POSITION* pPos) { if (pThis && pPos) pThis->RemoveAt(*pPos); }
// Symbol: ?RemoveHead@CObList@@QEAAPEAVCObject@@XZ
extern "C" CObject* MS_ABI impl__RemoveHead_CObList__QEAAPEAVCObject__XZ(CObList* pThis) { return pThis ? pThis->RemoveHead() : nullptr; }
// Symbol: ?RemoveTail@CObList@@QEAAPEAVCObject@@XZ
extern "C" CObject* MS_ABI impl__RemoveTail_CObList__QEAAPEAVCObject__XZ(CObList* pThis) { return pThis ? pThis->RemoveTail() : nullptr; }
// Symbol: ?Serialize@CObList@@UEAAXAEAVCArchive@@@Z
extern "C" void MS_ABI impl__Serialize_CObList__UEAAXAEAVCArchive___Z(CObList* pThis, CArchive& ar) { if (pThis) pThis->Serialize(ar); }

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

// Symbol: ??0CMapPtrToPtr@@QEAA@_J@Z
OPENMFC_WRAP_CTOR1(impl___0CMapPtrToPtr__QEAA__J_Z, CMapPtrToPtr, long long)
// Symbol: ??1CMapPtrToPtr@@UEAA@XZ
OPENMFC_WRAP_DTOR(impl___1CMapPtrToPtr__UEAA_XZ, CMapPtrToPtr)
// Symbol: ??ACMapPtrToPtr@@QEAAAEAPEAXPEAX@Z
extern "C" void** MS_ABI impl___ACMapPtrToPtr__QEAAAEAPEAXPEAX_Z(CMapPtrToPtr* pThis, void* key) { return pThis ? &((*pThis)[key]) : nullptr; }
// Symbol: ?FreeAssoc@CMapPtrToPtr@@IEAAXPEAUCAssoc@1@@Z
extern "C" void MS_ABI impl__FreeAssoc_CMapPtrToPtr__IEAAXPEAUCAssoc_1___Z(CMapPtrToPtr* /*pThis*/, void* pAssoc) { delete static_cast<AssocSnapshot<void*, void*>*>(pAssoc); }
extern "C" void MS_ABI impl__FreeAssoc_CMapPtrToPtr__IEAAXPEAUCAssoc_1__Z(CMapPtrToPtr* pThis, void* pAssoc) { impl__FreeAssoc_CMapPtrToPtr__IEAAXPEAUCAssoc_1___Z(pThis, pAssoc); }
// Symbol: ?GetAssocAt@CMapPtrToPtr@@IEBAPEAUCAssoc@1@PEAXAEAI1@Z
extern "C" void* MS_ABI impl__GetAssocAt_CMapPtrToPtr__IEBAPEAUCAssoc_1_PEAXAEAI1_Z(const CMapPtrToPtr* pThis, void* key, unsigned int& nHash, unsigned int& nHashBucket) { nHash = static_cast<unsigned int>(reinterpret_cast<uintptr_t>(key) >> 4); nHashBucket = pThis ? (nHash % pThis->GetHashTableSize()) : 0; void* value = nullptr; if (!pThis || !pThis->Lookup(key, value)) return nullptr; auto* assoc = new AssocSnapshot<void*, void*>(); assoc->nHashValue = nHash; assoc->key = key; assoc->value = value; return assoc; }
// Symbol: ?GetNextAssoc@CMapPtrToPtr@@QEBAXAEAPEAU__POSITION@@AEAPEAX1@Z
extern "C" void MS_ABI impl__GetNextAssoc_CMapPtrToPtr__QEBAXAEAPEAU__POSITION__AEAPEAX1_Z(const CMapPtrToPtr* pThis, CMapPtrToPtr::POSITION& pos, void*& key, void*& value) { if (pThis) pThis->GetNextAssoc(pos, key, value); else { key = nullptr; value = nullptr; } }
// Symbol: ?GetRuntimeClass@CMapPtrToPtr@@UEBAPEAUCRuntimeClass@@XZ
OPENMFC_WRAP_GETRUNTIMECLASS(impl__GetRuntimeClass_CMapPtrToPtr__UEBAPEAUCRuntimeClass__XZ, CMapPtrToPtr)
// Symbol: ?GetThisClass@CMapPtrToPtr@@SAPEAUCRuntimeClass@@XZ
OPENMFC_WRAP_GETTHISCLASS(impl__GetThisClass_CMapPtrToPtr__SAPEAUCRuntimeClass__XZ, CMapPtrToPtr)
// Symbol: ?GetValueAt@CMapPtrToPtr@@QEBAPEAXPEAX@Z
extern "C" void* MS_ABI impl__GetValueAt_CMapPtrToPtr__QEBAPEAXPEAX_Z(const CMapPtrToPtr* pThis, void* key) { void* value = nullptr; return (pThis && pThis->Lookup(key, value)) ? value : nullptr; }
// Symbol: ?InitHashTable@CMapPtrToPtr@@QEAAXIH@Z
extern "C" void MS_ABI impl__InitHashTable_CMapPtrToPtr__QEAAXIH_Z(CMapPtrToPtr* pThis, unsigned int hashSize, int bAllocNow) { if (pThis) pThis->InitHashTable(hashSize, bAllocNow); }
// Symbol: ?Lookup@CMapPtrToPtr@@QEBAHPEAXAEAPEAX@Z
extern "C" int MS_ABI impl__Lookup_CMapPtrToPtr__QEBAHPEAXAEAPEAX_Z(const CMapPtrToPtr* pThis, void* key, void*& value) { return (pThis && pThis->Lookup(key, value)) ? 1 : 0; }
// Symbol: ?NewAssoc@CMapPtrToPtr@@IEAAPEAUCAssoc@1@XZ
extern "C" void* MS_ABI impl__NewAssoc_CMapPtrToPtr__IEAAPEAUCAssoc_1_XZ(CMapPtrToPtr* /*pThis*/) { return new AssocSnapshot<void*, void*>(); }
// Symbol: ?RemoveAll@CMapPtrToPtr@@QEAAXXZ
extern "C" void MS_ABI impl__RemoveAll_CMapPtrToPtr__QEAAXXZ(CMapPtrToPtr* pThis) { if (pThis) pThis->RemoveAll(); }
// Symbol: ?RemoveKey@CMapPtrToPtr@@QEAAHPEAX@Z
extern "C" int MS_ABI impl__RemoveKey_CMapPtrToPtr__QEAAHPEAX_Z(CMapPtrToPtr* pThis, void* key) { return (pThis && pThis->RemoveKey(key)) ? 1 : 0; }

// Symbol: ??0CMapPtrToWord@@QEAA@_J@Z
OPENMFC_WRAP_CTOR1(impl___0CMapPtrToWord__QEAA__J_Z, CMapPtrToWord, long long)
// Symbol: ??1CMapPtrToWord@@UEAA@XZ
OPENMFC_WRAP_DTOR(impl___1CMapPtrToWord__UEAA_XZ, CMapPtrToWord)
// Symbol: ??ACMapPtrToWord@@QEAAAEAGPEAX@Z
extern "C" unsigned short* MS_ABI impl___ACMapPtrToWord__QEAAAEAGPEAX_Z(CMapPtrToWord* pThis, void* key) { return pThis ? &((*pThis)[key]) : nullptr; }
// Symbol: ?FreeAssoc@CMapPtrToWord@@IEAAXPEAUCAssoc@1@@Z
extern "C" void MS_ABI impl__FreeAssoc_CMapPtrToWord__IEAAXPEAUCAssoc_1___Z(CMapPtrToWord* /*pThis*/, void* pAssoc) { delete static_cast<AssocSnapshot<void*, WORD>*>(pAssoc); }
extern "C" void MS_ABI impl__FreeAssoc_CMapPtrToWord__IEAAXPEAUCAssoc_1__Z(CMapPtrToWord* pThis, void* pAssoc) { impl__FreeAssoc_CMapPtrToWord__IEAAXPEAUCAssoc_1___Z(pThis, pAssoc); }
// Symbol: ?GetAssocAt@CMapPtrToWord@@IEBAPEAUCAssoc@1@PEAXAEAI1@Z
extern "C" void* MS_ABI impl__GetAssocAt_CMapPtrToWord__IEBAPEAUCAssoc_1_PEAXAEAI1_Z(const CMapPtrToWord* pThis, void* key, unsigned int& nHash, unsigned int& nHashBucket) { nHash = static_cast<unsigned int>(reinterpret_cast<uintptr_t>(key) >> 4); nHashBucket = pThis ? (nHash % pThis->GetHashTableSize()) : 0; WORD value = 0; if (!pThis || !pThis->Lookup(key, value)) return nullptr; auto* assoc = new AssocSnapshot<void*, WORD>(); assoc->nHashValue = nHash; assoc->key = key; assoc->value = value; return assoc; }
// Symbol: ?GetNextAssoc@CMapPtrToWord@@QEBAXAEAPEAU__POSITION@@AEAPEAXAEAG@Z
extern "C" void MS_ABI impl__GetNextAssoc_CMapPtrToWord__QEBAXAEAPEAU__POSITION__AEAPEAXAEAG_Z(const CMapPtrToWord* pThis, CMapPtrToWord::POSITION& pos, void*& key, unsigned short& value) { if (pThis) pThis->GetNextAssoc(pos, key, value); else { key = nullptr; value = 0; } }
// Symbol: ?GetRuntimeClass@CMapPtrToWord@@UEBAPEAUCRuntimeClass@@XZ
OPENMFC_WRAP_GETRUNTIMECLASS(impl__GetRuntimeClass_CMapPtrToWord__UEBAPEAUCRuntimeClass__XZ, CMapPtrToWord)
// Symbol: ?GetThisClass@CMapPtrToWord@@SAPEAUCRuntimeClass@@XZ
OPENMFC_WRAP_GETTHISCLASS(impl__GetThisClass_CMapPtrToWord__SAPEAUCRuntimeClass__XZ, CMapPtrToWord)
// Symbol: ?InitHashTable@CMapPtrToWord@@QEAAXIH@Z
extern "C" void MS_ABI impl__InitHashTable_CMapPtrToWord__QEAAXIH_Z(CMapPtrToWord* pThis, unsigned int hashSize, int bAllocNow) { if (pThis) pThis->InitHashTable(hashSize, bAllocNow); }
// Symbol: ?Lookup@CMapPtrToWord@@QEBAHPEAXAEAG@Z
extern "C" int MS_ABI impl__Lookup_CMapPtrToWord__QEBAHPEAXAEAG_Z(const CMapPtrToWord* pThis, void* key, unsigned short& value) { return (pThis && pThis->Lookup(key, value)) ? 1 : 0; }
// Symbol: ?NewAssoc@CMapPtrToWord@@IEAAPEAUCAssoc@1@XZ
extern "C" void* MS_ABI impl__NewAssoc_CMapPtrToWord__IEAAPEAUCAssoc_1_XZ(CMapPtrToWord* /*pThis*/) { return new AssocSnapshot<void*, WORD>(); }
// Symbol: ?RemoveAll@CMapPtrToWord@@QEAAXXZ
extern "C" void MS_ABI impl__RemoveAll_CMapPtrToWord__QEAAXXZ(CMapPtrToWord* pThis) { if (pThis) pThis->RemoveAll(); }
// Symbol: ?RemoveKey@CMapPtrToWord@@QEAAHPEAX@Z
extern "C" int MS_ABI impl__RemoveKey_CMapPtrToWord__QEAAHPEAX_Z(CMapPtrToWord* pThis, void* key) { return (pThis && pThis->RemoveKey(key)) ? 1 : 0; }

// Symbol: ??0CMapStringToOb@@QEAA@_J@Z
OPENMFC_WRAP_CTOR1(impl___0CMapStringToOb__QEAA__J_Z, CMapStringToOb, long long)
// Symbol: ??1CMapStringToOb@@UEAA@XZ
OPENMFC_WRAP_DTOR(impl___1CMapStringToOb__UEAA_XZ, CMapStringToOb)
// Symbol: ??5@YAAEAVCArchive@@AEAV0@AEAPEAVCMapStringToOb@@@Z
OPENMFC_WRAP_SERIAL_EXTRACT(impl___5_YAAEAVCArchive__AEAV0_AEAPEAVCMapStringToOb___Z, CMapStringToOb)
// Symbol: ??ACMapStringToOb@@QEAAAEAPEAVCObject@@PEB_W@Z
extern "C" CObject** MS_ABI impl___ACMapStringToOb__QEAAAEAPEAVCObject__PEB_W_Z(CMapStringToOb* pThis, const wchar_t* key) { return pThis ? &((*pThis)[key]) : nullptr; }
// Symbol: ?CreateObject@CMapStringToOb@@SAPEAVCObject@@XZ
OPENMFC_WRAP_CREATEOBJECT(impl__CreateObject_CMapStringToOb__SAPEAVCObject__XZ, CMapStringToOb)
// Symbol: ?FreeAssoc@CMapStringToOb@@IEAAXPEAUCAssoc@1@@Z
extern "C" void MS_ABI impl__FreeAssoc_CMapStringToOb__IEAAXPEAUCAssoc_1___Z(CMapStringToOb* /*pThis*/, void* pAssoc) { delete static_cast<AssocSnapshot<CString, CObject*>*>(pAssoc); }
extern "C" void MS_ABI impl__FreeAssoc_CMapStringToOb__IEAAXPEAUCAssoc_1__Z(CMapStringToOb* pThis, void* pAssoc) { impl__FreeAssoc_CMapStringToOb__IEAAXPEAUCAssoc_1___Z(pThis, pAssoc); }
// Symbol: ?GetAssocAt@CMapStringToOb@@IEBAPEAUCAssoc@1@PEB_WAEAI1@Z
extern "C" void* MS_ABI impl__GetAssocAt_CMapStringToOb__IEBAPEAUCAssoc_1_PEB_WAEAI1_Z(const CMapStringToOb* pThis, const wchar_t* key, unsigned int& nHash, unsigned int& nHashBucket) { CString k = NormalizeStringKey(key); nHash = HashCStringKey(k); nHashBucket = pThis ? (nHash % pThis->GetHashTableSize()) : 0; CObject* value = nullptr; if (!pThis || !pThis->Lookup(k, value)) return nullptr; auto* assoc = new AssocSnapshot<CString, CObject*>(); assoc->nHashValue = nHash; assoc->key = k; assoc->value = value; return assoc; }
// Symbol: ?GetNextAssoc@CMapStringToOb@@QEBAXAEAPEAU__POSITION@@AEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@AEAPEAVCObject@@@Z
extern "C" void MS_ABI impl__GetNextAssoc_CMapStringToOb__QEBAXAEAPEAU__POSITION__AEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__AEAPEAVCObject___Z(const CMapStringToOb* pThis, CMapStringToOb::POSITION& pos, CString& key, CObject*& value) { if (pThis) pThis->GetNextAssoc(pos, key, value); else { key = L""; value = nullptr; } }
// Symbol: ?GetRuntimeClass@CMapStringToOb@@UEBAPEAUCRuntimeClass@@XZ
OPENMFC_WRAP_GETRUNTIMECLASS(impl__GetRuntimeClass_CMapStringToOb__UEBAPEAUCRuntimeClass__XZ, CMapStringToOb)
// Symbol: ?GetThisClass@CMapStringToOb@@SAPEAUCRuntimeClass@@XZ
OPENMFC_WRAP_GETTHISCLASS(impl__GetThisClass_CMapStringToOb__SAPEAUCRuntimeClass__XZ, CMapStringToOb)
// Symbol: ?InitHashTable@CMapStringToOb@@QEAAXIH@Z
extern "C" void MS_ABI impl__InitHashTable_CMapStringToOb__QEAAXIH_Z(CMapStringToOb* pThis, unsigned int hashSize, int bAllocNow) { if (pThis) pThis->InitHashTable(hashSize, bAllocNow); }
// Symbol: ?Lookup@CMapStringToOb@@QEBAHPEB_WAEAPEAVCObject@@@Z
extern "C" int MS_ABI impl__Lookup_CMapStringToOb__QEBAHPEB_WAEAPEAVCObject___Z(const CMapStringToOb* pThis, const wchar_t* key, CObject*& value) { return (pThis && pThis->Lookup(key, value)) ? 1 : 0; }
// Symbol: ?LookupKey@CMapStringToOb@@QEBAHPEB_WAEAPEB_W@Z
extern "C" int MS_ABI impl__LookupKey_CMapStringToOb__QEBAHPEB_WAEAPEB_W_Z(const CMapStringToOb* pThis, const wchar_t* key, const wchar_t*& actualKey) { return (pThis && pThis->LookupKey(key, actualKey)) ? 1 : 0; }
// Symbol: ?NewAssoc@CMapStringToOb@@IEAAPEAUCAssoc@1@XZ
extern "C" void* MS_ABI impl__NewAssoc_CMapStringToOb__IEAAPEAUCAssoc_1_XZ(CMapStringToOb* /*pThis*/) { return new AssocSnapshot<CString, CObject*>(); }
// Symbol: ?RemoveAll@CMapStringToOb@@QEAAXXZ
extern "C" void MS_ABI impl__RemoveAll_CMapStringToOb__QEAAXXZ(CMapStringToOb* pThis) { if (pThis) pThis->RemoveAll(); }
// Symbol: ?RemoveKey@CMapStringToOb@@QEAAHPEB_W@Z
extern "C" int MS_ABI impl__RemoveKey_CMapStringToOb__QEAAHPEB_W_Z(CMapStringToOb* pThis, const wchar_t* key) { return (pThis && pThis->RemoveKey(key)) ? 1 : 0; }
// Symbol: ?Serialize@CMapStringToOb@@UEAAXAEAVCArchive@@@Z
extern "C" void MS_ABI impl__Serialize_CMapStringToOb__UEAAXAEAVCArchive___Z(CMapStringToOb* pThis, CArchive& ar) { if (pThis) pThis->Serialize(ar); }

// Symbol: ??0CMapStringToPtr@@QEAA@_J@Z
OPENMFC_WRAP_CTOR1(impl___0CMapStringToPtr__QEAA__J_Z, CMapStringToPtr, long long)
// Symbol: ??1CMapStringToPtr@@UEAA@XZ
OPENMFC_WRAP_DTOR(impl___1CMapStringToPtr__UEAA_XZ, CMapStringToPtr)
// Symbol: ??ACMapStringToPtr@@QEAAAEAPEAXPEB_W@Z
extern "C" void** MS_ABI impl___ACMapStringToPtr__QEAAAEAPEAXPEB_W_Z(CMapStringToPtr* pThis, const wchar_t* key) { return pThis ? &((*pThis)[key]) : nullptr; }
// Symbol: ?FreeAssoc@CMapStringToPtr@@IEAAXPEAUCAssoc@1@@Z
extern "C" void MS_ABI impl__FreeAssoc_CMapStringToPtr__IEAAXPEAUCAssoc_1___Z(CMapStringToPtr* /*pThis*/, void* pAssoc) { delete static_cast<AssocSnapshot<CString, void*>*>(pAssoc); }
extern "C" void MS_ABI impl__FreeAssoc_CMapStringToPtr__IEAAXPEAUCAssoc_1__Z(CMapStringToPtr* pThis, void* pAssoc) { impl__FreeAssoc_CMapStringToPtr__IEAAXPEAUCAssoc_1___Z(pThis, pAssoc); }
// Symbol: ?GetAssocAt@CMapStringToPtr@@IEBAPEAUCAssoc@1@PEB_WAEAI1@Z
extern "C" void* MS_ABI impl__GetAssocAt_CMapStringToPtr__IEBAPEAUCAssoc_1_PEB_WAEAI1_Z(const CMapStringToPtr* pThis, const wchar_t* key, unsigned int& nHash, unsigned int& nHashBucket) { CString k = NormalizeStringKey(key); nHash = HashCStringKey(k); nHashBucket = pThis ? (nHash % pThis->GetHashTableSize()) : 0; void* value = nullptr; if (!pThis || !pThis->Lookup(k, value)) return nullptr; auto* assoc = new AssocSnapshot<CString, void*>(); assoc->nHashValue = nHash; assoc->key = k; assoc->value = value; return assoc; }
// Symbol: ?GetNextAssoc@CMapStringToPtr@@QEBAXAEAPEAU__POSITION@@AEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@AEAPEAX@Z
extern "C" void MS_ABI impl__GetNextAssoc_CMapStringToPtr__QEBAXAEAPEAU__POSITION__AEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__AEAPEAX_Z(const CMapStringToPtr* pThis, CMapStringToPtr::POSITION& pos, CString& key, void*& value) { if (pThis) pThis->GetNextAssoc(pos, key, value); else { key = L""; value = nullptr; } }
// Symbol: ?GetRuntimeClass@CMapStringToPtr@@UEBAPEAUCRuntimeClass@@XZ
OPENMFC_WRAP_GETRUNTIMECLASS(impl__GetRuntimeClass_CMapStringToPtr__UEBAPEAUCRuntimeClass__XZ, CMapStringToPtr)
// Symbol: ?GetThisClass@CMapStringToPtr@@SAPEAUCRuntimeClass@@XZ
OPENMFC_WRAP_GETTHISCLASS(impl__GetThisClass_CMapStringToPtr__SAPEAUCRuntimeClass__XZ, CMapStringToPtr)
// Symbol: ?InitHashTable@CMapStringToPtr@@QEAAXIH@Z
extern "C" void MS_ABI impl__InitHashTable_CMapStringToPtr__QEAAXIH_Z(CMapStringToPtr* pThis, unsigned int hashSize, int bAllocNow) { if (pThis) pThis->InitHashTable(hashSize, bAllocNow); }
// Symbol: ?Lookup@CMapStringToPtr@@QEBAHPEB_WAEAPEAX@Z
extern "C" int MS_ABI impl__Lookup_CMapStringToPtr__QEBAHPEB_WAEAPEAX_Z(const CMapStringToPtr* pThis, const wchar_t* key, void*& value) { return (pThis && pThis->Lookup(key, value)) ? 1 : 0; }
// Symbol: ?LookupKey@CMapStringToPtr@@QEBAHPEB_WAEAPEB_W@Z
extern "C" int MS_ABI impl__LookupKey_CMapStringToPtr__QEBAHPEB_WAEAPEB_W_Z(const CMapStringToPtr* pThis, const wchar_t* key, const wchar_t*& actualKey) { return (pThis && pThis->LookupKey(key, actualKey)) ? 1 : 0; }
// Symbol: ?NewAssoc@CMapStringToPtr@@IEAAPEAUCAssoc@1@XZ
extern "C" void* MS_ABI impl__NewAssoc_CMapStringToPtr__IEAAPEAUCAssoc_1_XZ(CMapStringToPtr* /*pThis*/) { return new AssocSnapshot<CString, void*>(); }
// Symbol: ?RemoveAll@CMapStringToPtr@@QEAAXXZ
extern "C" void MS_ABI impl__RemoveAll_CMapStringToPtr__QEAAXXZ(CMapStringToPtr* pThis) { if (pThis) pThis->RemoveAll(); }
// Symbol: ?RemoveKey@CMapStringToPtr@@QEAAHPEB_W@Z
extern "C" int MS_ABI impl__RemoveKey_CMapStringToPtr__QEAAHPEB_W_Z(CMapStringToPtr* pThis, const wchar_t* key) { return (pThis && pThis->RemoveKey(key)) ? 1 : 0; }

// Symbol: ??0CMapStringToString@@QEAA@_J@Z
OPENMFC_WRAP_CTOR1(impl___0CMapStringToString__QEAA__J_Z, CMapStringToString, long long)
// Symbol: ??1CMapStringToString@@UEAA@XZ
OPENMFC_WRAP_DTOR(impl___1CMapStringToString__UEAA_XZ, CMapStringToString)
// Symbol: ??5@YAAEAVCArchive@@AEAV0@AEAPEAVCMapStringToString@@@Z
OPENMFC_WRAP_SERIAL_EXTRACT(impl___5_YAAEAVCArchive__AEAV0_AEAPEAVCMapStringToString___Z, CMapStringToString)
// Symbol: ??ACMapStringToString@@QEAAAEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@PEB_W@Z
extern "C" CString* MS_ABI impl___ACMapStringToString__QEAAAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__PEB_W_Z(CMapStringToString* pThis, const wchar_t* key) { return pThis ? &((*pThis)[key]) : nullptr; }
// Symbol: ?CreateObject@CMapStringToString@@SAPEAVCObject@@XZ
OPENMFC_WRAP_CREATEOBJECT(impl__CreateObject_CMapStringToString__SAPEAVCObject__XZ, CMapStringToString)
// Symbol: ?FreeAssoc@CMapStringToString@@IEAAXPEAVCAssoc@1@@Z
extern "C" void MS_ABI impl__FreeAssoc_CMapStringToString__IEAAXPEAVCAssoc_1___Z(CMapStringToString* /*pThis*/, void* pAssoc) { delete static_cast<AssocSnapshot<CString, CString>*>(pAssoc); }
extern "C" void MS_ABI impl__FreeAssoc_CMapStringToString__IEAAXPEAVCAssoc_1__Z(CMapStringToString* pThis, void* pAssoc) { impl__FreeAssoc_CMapStringToString__IEAAXPEAVCAssoc_1___Z(pThis, pAssoc); }
// Symbol: ?GetAssocAt@CMapStringToString@@IEBAPEAVCAssoc@1@PEB_WAEAI1@Z
extern "C" void* MS_ABI impl__GetAssocAt_CMapStringToString__IEBAPEAVCAssoc_1_PEB_WAEAI1_Z(const CMapStringToString* pThis, const wchar_t* key, unsigned int& nHash, unsigned int& nHashBucket) { CString k = NormalizeStringKey(key); nHash = HashCStringKey(k); nHashBucket = pThis ? (nHash % pThis->GetHashTableSize()) : 0; CString value; if (!pThis || !pThis->Lookup(k, value)) return nullptr; auto* assoc = new AssocSnapshot<CString, CString>(); assoc->nHashValue = nHash; assoc->key = k; assoc->value = value; return assoc; }
// Symbol: ?GetNextAssoc@CMapStringToString@@QEBAXAEAPEAU__POSITION@@AEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@1@Z
extern "C" void MS_ABI impl__GetNextAssoc_CMapStringToString__QEBAXAEAPEAU__POSITION__AEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__1_Z(const CMapStringToString* pThis, CMapStringToString::POSITION& pos, CString& key, CString& value) { if (pThis) pThis->GetNextAssoc(pos, key, value); else { key = L""; value = L""; } }
// Symbol: ?GetRuntimeClass@CMapStringToString@@UEBAPEAUCRuntimeClass@@XZ
OPENMFC_WRAP_GETRUNTIMECLASS(impl__GetRuntimeClass_CMapStringToString__UEBAPEAUCRuntimeClass__XZ, CMapStringToString)
// Symbol: ?GetThisClass@CMapStringToString@@SAPEAUCRuntimeClass@@XZ
OPENMFC_WRAP_GETTHISCLASS(impl__GetThisClass_CMapStringToString__SAPEAUCRuntimeClass__XZ, CMapStringToString)
// Symbol: ?InitHashTable@CMapStringToString@@QEAAXIH@Z
extern "C" void MS_ABI impl__InitHashTable_CMapStringToString__QEAAXIH_Z(CMapStringToString* pThis, unsigned int hashSize, int bAllocNow) { if (pThis) pThis->InitHashTable(hashSize, bAllocNow); }
// Symbol: ?Lookup@CMapStringToString@@QEBAHPEB_WAEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@@Z
extern "C" int MS_ABI impl__Lookup_CMapStringToString__QEBAHPEB_WAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(const CMapStringToString* pThis, const wchar_t* key, CString& value) { return (pThis && pThis->Lookup(key, value)) ? 1 : 0; }
// Symbol: ?LookupKey@CMapStringToString@@QEBAHPEB_WAEAPEB_W@Z
extern "C" int MS_ABI impl__LookupKey_CMapStringToString__QEBAHPEB_WAEAPEB_W_Z(const CMapStringToString* pThis, const wchar_t* key, const wchar_t*& actualKey) { return (pThis && pThis->LookupKey(key, actualKey)) ? 1 : 0; }
// Symbol: ?NewAssoc@CMapStringToString@@IEAAPEAVCAssoc@1@PEB_W@Z
extern "C" void* MS_ABI impl__NewAssoc_CMapStringToString__IEAAPEAVCAssoc_1_PEB_W_Z(CMapStringToString* /*pThis*/, const wchar_t* key) { auto* assoc = new AssocSnapshot<CString, CString>(); assoc->key = NormalizeStringKey(key); assoc->nHashValue = HashCStringKey(assoc->key); return assoc; }
// Symbol: ?PGetFirstAssoc@CMapStringToString@@QEAAPEAUCPair@1@XZ
extern "C" CMapStringToString::CPair* MS_ABI impl__PGetFirstAssoc_CMapStringToString__QEAAPEAUCPair_1_XZ(CMapStringToString* pThis) { return pThis ? pThis->PGetFirstAssoc() : nullptr; }
// Symbol: ?PGetFirstAssoc@CMapStringToString@@QEBAPEBUCPair@1@XZ
extern "C" const CMapStringToString::CPair* MS_ABI impl__PGetFirstAssoc_CMapStringToString__QEBAPEBUCPair_1_XZ(const CMapStringToString* pThis) { return pThis ? pThis->PGetFirstAssoc() : nullptr; }
// Symbol: ?PGetNextAssoc@CMapStringToString@@QEAAPEAUCPair@1@PEBU21@@Z
extern "C" CMapStringToString::CPair* MS_ABI impl__PGetNextAssoc_CMapStringToString__QEAAPEAUCPair_1_PEBU21___Z(CMapStringToString* pThis, const CMapStringToString::CPair* pAssoc) { return pThis ? pThis->PGetNextAssoc(pAssoc) : nullptr; }
extern "C" CMapStringToString::CPair* MS_ABI impl__PGetNextAssoc_CMapStringToString__QEAAPEAUCPair_1_PEBU21__Z(CMapStringToString* pThis, const CMapStringToString::CPair* pAssoc) { return impl__PGetNextAssoc_CMapStringToString__QEAAPEAUCPair_1_PEBU21___Z(pThis, pAssoc); }
// Symbol: ?PGetNextAssoc@CMapStringToString@@QEBAPEBUCPair@1@PEBU21@@Z
extern "C" const CMapStringToString::CPair* MS_ABI impl__PGetNextAssoc_CMapStringToString__QEBAPEBUCPair_1_PEBU21___Z(const CMapStringToString* pThis, const CMapStringToString::CPair* pAssoc) { return pThis ? pThis->PGetNextAssoc(pAssoc) : nullptr; }
extern "C" const CMapStringToString::CPair* MS_ABI impl__PGetNextAssoc_CMapStringToString__QEBAPEBUCPair_1_PEBU21__Z(const CMapStringToString* pThis, const CMapStringToString::CPair* pAssoc) { return impl__PGetNextAssoc_CMapStringToString__QEBAPEBUCPair_1_PEBU21___Z(pThis, pAssoc); }
// Symbol: ?PLookup@CMapStringToString@@QEAAPEAUCPair@1@PEB_W@Z
extern "C" CMapStringToString::CPair* MS_ABI impl__PLookup_CMapStringToString__QEAAPEAUCPair_1_PEB_W_Z(CMapStringToString* pThis, const wchar_t* key) { return pThis ? pThis->PLookup(key) : nullptr; }
// Symbol: ?PLookup@CMapStringToString@@QEBAPEBUCPair@1@PEB_W@Z
extern "C" const CMapStringToString::CPair* MS_ABI impl__PLookup_CMapStringToString__QEBAPEBUCPair_1_PEB_W_Z(const CMapStringToString* pThis, const wchar_t* key) { return pThis ? pThis->PLookup(key) : nullptr; }
// Symbol: ?RemoveAll@CMapStringToString@@QEAAXXZ
extern "C" void MS_ABI impl__RemoveAll_CMapStringToString__QEAAXXZ(CMapStringToString* pThis) { if (pThis) pThis->RemoveAll(); }
// Symbol: ?RemoveKey@CMapStringToString@@QEAAHPEB_W@Z
extern "C" int MS_ABI impl__RemoveKey_CMapStringToString__QEAAHPEB_W_Z(CMapStringToString* pThis, const wchar_t* key) { return (pThis && pThis->RemoveKey(key)) ? 1 : 0; }
// Symbol: ?Serialize@CMapStringToString@@UEAAXAEAVCArchive@@@Z
extern "C" void MS_ABI impl__Serialize_CMapStringToString__UEAAXAEAVCArchive___Z(CMapStringToString* pThis, CArchive& ar) { if (pThis) pThis->Serialize(ar); }

// Symbol: ??0CMapWordToOb@@QEAA@_J@Z
OPENMFC_WRAP_CTOR1(impl___0CMapWordToOb__QEAA__J_Z, CMapWordToOb, long long)
// Symbol: ??1CMapWordToOb@@UEAA@XZ
OPENMFC_WRAP_DTOR(impl___1CMapWordToOb__UEAA_XZ, CMapWordToOb)
// Symbol: ??5@YAAEAVCArchive@@AEAV0@AEAPEAVCMapWordToOb@@@Z
OPENMFC_WRAP_SERIAL_EXTRACT(impl___5_YAAEAVCArchive__AEAV0_AEAPEAVCMapWordToOb___Z, CMapWordToOb)
// Symbol: ??ACMapWordToOb@@QEAAAEAPEAVCObject@@G@Z
extern "C" CObject** MS_ABI impl___ACMapWordToOb__QEAAAEAPEAVCObject__G_Z(CMapWordToOb* pThis, unsigned short key) { return pThis ? &((*pThis)[key]) : nullptr; }
// Symbol: ?CreateObject@CMapWordToOb@@SAPEAVCObject@@XZ
OPENMFC_WRAP_CREATEOBJECT(impl__CreateObject_CMapWordToOb__SAPEAVCObject__XZ, CMapWordToOb)
// Symbol: ?FreeAssoc@CMapWordToOb@@IEAAXPEAUCAssoc@1@@Z
extern "C" void MS_ABI impl__FreeAssoc_CMapWordToOb__IEAAXPEAUCAssoc_1___Z(CMapWordToOb* /*pThis*/, void* pAssoc) { delete static_cast<AssocSnapshot<WORD, CObject*>*>(pAssoc); }
extern "C" void MS_ABI impl__FreeAssoc_CMapWordToOb__IEAAXPEAUCAssoc_1__Z(CMapWordToOb* pThis, void* pAssoc) { impl__FreeAssoc_CMapWordToOb__IEAAXPEAUCAssoc_1___Z(pThis, pAssoc); }
// Symbol: ?GetAssocAt@CMapWordToOb@@IEBAPEAUCAssoc@1@GAEAI0@Z
extern "C" void* MS_ABI impl__GetAssocAt_CMapWordToOb__IEBAPEAUCAssoc_1_GAEAI0_Z(const CMapWordToOb* pThis, unsigned short key, unsigned int& nHash, unsigned int& nHashBucket) { nHash = static_cast<unsigned int>(key >> 4); nHashBucket = pThis ? (nHash % pThis->GetHashTableSize()) : 0; CObject* value = nullptr; if (!pThis || !pThis->Lookup(key, value)) return nullptr; auto* assoc = new AssocSnapshot<WORD, CObject*>(); assoc->nHashValue = nHash; assoc->key = key; assoc->value = value; return assoc; }
// Symbol: ?GetNextAssoc@CMapWordToOb@@QEBAXAEAPEAU__POSITION@@AEAGAEAPEAVCObject@@@Z
extern "C" void MS_ABI impl__GetNextAssoc_CMapWordToOb__QEBAXAEAPEAU__POSITION__AEAGAEAPEAVCObject___Z(const CMapWordToOb* pThis, CMapWordToOb::POSITION& pos, unsigned short& key, CObject*& value) { if (pThis) pThis->GetNextAssoc(pos, key, value); else { key = 0; value = nullptr; } }
// Symbol: ?GetRuntimeClass@CMapWordToOb@@UEBAPEAUCRuntimeClass@@XZ
OPENMFC_WRAP_GETRUNTIMECLASS(impl__GetRuntimeClass_CMapWordToOb__UEBAPEAUCRuntimeClass__XZ, CMapWordToOb)
// Symbol: ?GetThisClass@CMapWordToOb@@SAPEAUCRuntimeClass@@XZ
OPENMFC_WRAP_GETTHISCLASS(impl__GetThisClass_CMapWordToOb__SAPEAUCRuntimeClass__XZ, CMapWordToOb)
// Symbol: ?InitHashTable@CMapWordToOb@@QEAAXIH@Z
extern "C" void MS_ABI impl__InitHashTable_CMapWordToOb__QEAAXIH_Z(CMapWordToOb* pThis, unsigned int hashSize, int bAllocNow) { if (pThis) pThis->InitHashTable(hashSize, bAllocNow); }
// Symbol: ?Lookup@CMapWordToOb@@QEBAHGAEAPEAVCObject@@@Z
extern "C" int MS_ABI impl__Lookup_CMapWordToOb__QEBAHGAEAPEAVCObject___Z(const CMapWordToOb* pThis, unsigned short key, CObject*& value) { return (pThis && pThis->Lookup(key, value)) ? 1 : 0; }
// Symbol: ?NewAssoc@CMapWordToOb@@IEAAPEAUCAssoc@1@XZ
extern "C" void* MS_ABI impl__NewAssoc_CMapWordToOb__IEAAPEAUCAssoc_1_XZ(CMapWordToOb* /*pThis*/) { return new AssocSnapshot<WORD, CObject*>(); }
// Symbol: ?RemoveAll@CMapWordToOb@@QEAAXXZ
extern "C" void MS_ABI impl__RemoveAll_CMapWordToOb__QEAAXXZ(CMapWordToOb* pThis) { if (pThis) pThis->RemoveAll(); }
// Symbol: ?RemoveKey@CMapWordToOb@@QEAAHG@Z
extern "C" int MS_ABI impl__RemoveKey_CMapWordToOb__QEAAHG_Z(CMapWordToOb* pThis, unsigned short key) { return (pThis && pThis->RemoveKey(key)) ? 1 : 0; }
// Symbol: ?Serialize@CMapWordToOb@@UEAAXAEAVCArchive@@@Z
extern "C" void MS_ABI impl__Serialize_CMapWordToOb__UEAAXAEAVCArchive___Z(CMapWordToOb* pThis, CArchive& ar) { if (pThis) pThis->Serialize(ar); }

// Symbol: ??0CMapWordToPtr@@QEAA@_J@Z
OPENMFC_WRAP_CTOR1(impl___0CMapWordToPtr__QEAA__J_Z, CMapWordToPtr, long long)
// Symbol: ??1CMapWordToPtr@@UEAA@XZ
OPENMFC_WRAP_DTOR(impl___1CMapWordToPtr__UEAA_XZ, CMapWordToPtr)
// Symbol: ??ACMapWordToPtr@@QEAAAEAPEAXG@Z
extern "C" void** MS_ABI impl___ACMapWordToPtr__QEAAAEAPEAXG_Z(CMapWordToPtr* pThis, unsigned short key) { return pThis ? &((*pThis)[key]) : nullptr; }
// Symbol: ?FreeAssoc@CMapWordToPtr@@IEAAXPEAUCAssoc@1@@Z
extern "C" void MS_ABI impl__FreeAssoc_CMapWordToPtr__IEAAXPEAUCAssoc_1___Z(CMapWordToPtr* /*pThis*/, void* pAssoc) { delete static_cast<AssocSnapshot<WORD, void*>*>(pAssoc); }
extern "C" void MS_ABI impl__FreeAssoc_CMapWordToPtr__IEAAXPEAUCAssoc_1__Z(CMapWordToPtr* pThis, void* pAssoc) { impl__FreeAssoc_CMapWordToPtr__IEAAXPEAUCAssoc_1___Z(pThis, pAssoc); }
// Symbol: ?GetAssocAt@CMapWordToPtr@@IEBAPEAUCAssoc@1@GAEAI0@Z
extern "C" void* MS_ABI impl__GetAssocAt_CMapWordToPtr__IEBAPEAUCAssoc_1_GAEAI0_Z(const CMapWordToPtr* pThis, unsigned short key, unsigned int& nHash, unsigned int& nHashBucket) { nHash = static_cast<unsigned int>(key >> 4); nHashBucket = pThis ? (nHash % pThis->GetHashTableSize()) : 0; void* value = nullptr; if (!pThis || !pThis->Lookup(key, value)) return nullptr; auto* assoc = new AssocSnapshot<WORD, void*>(); assoc->nHashValue = nHash; assoc->key = key; assoc->value = value; return assoc; }
// Symbol: ?GetNextAssoc@CMapWordToPtr@@QEBAXAEAPEAU__POSITION@@AEAGAEAPEAX@Z
extern "C" void MS_ABI impl__GetNextAssoc_CMapWordToPtr__QEBAXAEAPEAU__POSITION__AEAGAEAPEAX_Z(const CMapWordToPtr* pThis, CMapWordToPtr::POSITION& pos, unsigned short& key, void*& value) { if (pThis) pThis->GetNextAssoc(pos, key, value); else { key = 0; value = nullptr; } }
// Symbol: ?GetRuntimeClass@CMapWordToPtr@@UEBAPEAUCRuntimeClass@@XZ
OPENMFC_WRAP_GETRUNTIMECLASS(impl__GetRuntimeClass_CMapWordToPtr__UEBAPEAUCRuntimeClass__XZ, CMapWordToPtr)
// Symbol: ?GetThisClass@CMapWordToPtr@@SAPEAUCRuntimeClass@@XZ
OPENMFC_WRAP_GETTHISCLASS(impl__GetThisClass_CMapWordToPtr__SAPEAUCRuntimeClass__XZ, CMapWordToPtr)
// Symbol: ?InitHashTable@CMapWordToPtr@@QEAAXIH@Z
extern "C" void MS_ABI impl__InitHashTable_CMapWordToPtr__QEAAXIH_Z(CMapWordToPtr* pThis, unsigned int hashSize, int bAllocNow) { if (pThis) pThis->InitHashTable(hashSize, bAllocNow); }
// Symbol: ?Lookup@CMapWordToPtr@@QEBAHGAEAPEAX@Z
extern "C" int MS_ABI impl__Lookup_CMapWordToPtr__QEBAHGAEAPEAX_Z(const CMapWordToPtr* pThis, unsigned short key, void*& value) { return (pThis && pThis->Lookup(key, value)) ? 1 : 0; }
// Symbol: ?NewAssoc@CMapWordToPtr@@IEAAPEAUCAssoc@1@XZ
extern "C" void* MS_ABI impl__NewAssoc_CMapWordToPtr__IEAAPEAUCAssoc_1_XZ(CMapWordToPtr* /*pThis*/) { return new AssocSnapshot<WORD, void*>(); }
// Symbol: ?RemoveAll@CMapWordToPtr@@QEAAXXZ
extern "C" void MS_ABI impl__RemoveAll_CMapWordToPtr__QEAAXXZ(CMapWordToPtr* pThis) { if (pThis) pThis->RemoveAll(); }
// Symbol: ?RemoveKey@CMapWordToPtr@@QEAAHG@Z
extern "C" int MS_ABI impl__RemoveKey_CMapWordToPtr__QEAAHG_Z(CMapWordToPtr* pThis, unsigned short key) { return (pThis && pThis->RemoveKey(key)) ? 1 : 0; }

#undef OPENMFC_WRAP_CTOR1
#undef OPENMFC_WRAP_DTOR
#undef OPENMFC_WRAP_GETTHISCLASS
#undef OPENMFC_WRAP_GETRUNTIMECLASS
#undef OPENMFC_WRAP_CREATEOBJECT
#undef OPENMFC_WRAP_SERIAL_EXTRACT

// =============================================================================
// CFile Implementation
// =============================================================================

CFile::CFile() : m_hFile(INVALID_HANDLE_VALUE) {
}

CFile::CFile(HANDLE hFile) : m_hFile(hFile) {
}

CFile::CFile(const wchar_t* lpszFileName, UINT nOpenFlags) : m_hFile(INVALID_HANDLE_VALUE) {
    CommonInit(lpszFileName, nOpenFlags, nullptr);
}

CFile::~CFile() {
    if (m_hFile != INVALID_HANDLE_VALUE) {
        Close();
    }
}

void CFile::CommonInit(const wchar_t* lpszFileName, UINT nOpenFlags, void* /*pTM*/) {
    if (!lpszFileName) return;

    m_strFileName = lpszFileName;

    // Map MFC flags to Win32 flags
    DWORD dwAccess = 0;
    DWORD dwShare = 0;
    DWORD dwCreate = OPEN_EXISTING;

    // Access mode
    if ((nOpenFlags & 0x0003) == modeRead) {
        dwAccess = GENERIC_READ;
    } else if ((nOpenFlags & 0x0003) == modeWrite) {
        dwAccess = GENERIC_WRITE;
    } else if ((nOpenFlags & 0x0003) == modeReadWrite) {
        dwAccess = GENERIC_READ | GENERIC_WRITE;
    }

    // Share mode
    if ((nOpenFlags & 0x0070) == shareExclusive) {
        dwShare = 0;
    } else if ((nOpenFlags & 0x0070) == shareDenyWrite) {
        dwShare = FILE_SHARE_READ;
    } else if ((nOpenFlags & 0x0070) == shareDenyRead) {
        dwShare = FILE_SHARE_WRITE;
    } else if ((nOpenFlags & 0x0070) == shareDenyNone) {
        dwShare = FILE_SHARE_READ | FILE_SHARE_WRITE;
    } else {
        dwShare = FILE_SHARE_READ | FILE_SHARE_WRITE;  // shareCompat
    }

    // Creation disposition
    if (nOpenFlags & modeCreate) {
        if (nOpenFlags & modeNoTruncate) {
            dwCreate = OPEN_ALWAYS;
        } else {
            dwCreate = CREATE_ALWAYS;
        }
    } else {
        dwCreate = OPEN_EXISTING;
    }

    m_hFile = CreateFileW(
        lpszFileName,
        dwAccess,
        dwShare,
        nullptr,
        dwCreate,
        FILE_ATTRIBUTE_NORMAL,
        nullptr
    );
}

UINT CFile::Read(void* lpBuf, UINT nCount) {
    if (m_hFile == INVALID_HANDLE_VALUE || !lpBuf || nCount == 0) {
        return 0;
    }

    DWORD dwRead = 0;
    if (!ReadFile(m_hFile, lpBuf, nCount, &dwRead, nullptr)) {
        return 0;
    }
    return dwRead;
}

void CFile::Write(const void* lpBuf, UINT nCount) {
    if (m_hFile == INVALID_HANDLE_VALUE || !lpBuf || nCount == 0) {
        return;
    }

    DWORD dwWritten = 0;
    WriteFile(m_hFile, lpBuf, nCount, &dwWritten, nullptr);
}

ULONGLONG CFile::Seek(LONGLONG lOff, UINT nFrom) {
    if (m_hFile == INVALID_HANDLE_VALUE) {
        return 0;
    }

    DWORD dwMethod;
    switch (nFrom) {
        case begin:   dwMethod = FILE_BEGIN; break;
        case current: dwMethod = FILE_CURRENT; break;
        case end:     dwMethod = FILE_END; break;
        default:      dwMethod = FILE_BEGIN; break;
    }

    LARGE_INTEGER liOff;
    liOff.QuadPart = lOff;
    LARGE_INTEGER liNew;
    if (!SetFilePointerEx(m_hFile, liOff, &liNew, dwMethod)) {
        return 0;
    }
    return liNew.QuadPart;
}

void CFile::SetLength(ULONGLONG dwNewLen) {
    if (m_hFile == INVALID_HANDLE_VALUE) {
        return;
    }

    LARGE_INTEGER liPos;
    liPos.QuadPart = dwNewLen;
    SetFilePointerEx(m_hFile, liPos, nullptr, FILE_BEGIN);
    SetEndOfFile(m_hFile);
}

ULONGLONG CFile::GetLength() const {
    if (m_hFile == INVALID_HANDLE_VALUE) {
        return 0;
    }

    LARGE_INTEGER liSize;
    if (!GetFileSizeEx(m_hFile, &liSize)) {
        return 0;
    }
    return liSize.QuadPart;
}

void CFile::Flush() {
    if (m_hFile != INVALID_HANDLE_VALUE) {
        FlushFileBuffers(m_hFile);
    }
}

void CFile::Close() {
    if (m_hFile != INVALID_HANDLE_VALUE) {
        CloseHandle(m_hFile);
        m_hFile = INVALID_HANDLE_VALUE;
    }
}

void CFile::SetFilePath(const wchar_t* lpszNewName) {
    m_strFileName = lpszNewName ? lpszNewName : L"";
}

CString CFile::GetFileName() const {
    // Extract filename from path
    const wchar_t* pPath = (const wchar_t*)m_strFileName;
    const wchar_t* pName = pPath;
    while (*pPath) {
        if (*pPath == L'\\' || *pPath == L'/') {
            pName = pPath + 1;
        }
        pPath++;
    }
    return CString(pName);
}

CString CFile::GetFileTitle() const {
    CString strName = GetFileName();
    // Remove extension
    int nDot = strName.ReverseFind(L'.');
    if (nDot >= 0) {
        return strName.Left(nDot);
    }
    return strName;
}

CString CFile::GetFilePath() const {
    return m_strFileName;
}

void CFile::Rename(const wchar_t* lpszOldName, const wchar_t* lpszNewName) {
    if (lpszOldName && lpszNewName) {
        MoveFileW(lpszOldName, lpszNewName);
    }
}

void CFile::Remove(const wchar_t* lpszFileName) {
    if (lpszFileName) {
        DeleteFileW(lpszFileName);
    }
}

int CFile::GetStatus(CFileStatus& rStatus) const {
    if (!m_strFileName.IsEmpty()) {
        return GetStatus(m_strFileName.GetString(), rStatus, nullptr);
    }
    if (m_hFile == (void*)INVALID_HANDLE_VALUE) {
        return 0;
    }

    BY_HANDLE_FILE_INFORMATION info;
    if (!GetFileInformationByHandle((HANDLE)m_hFile, &info)) {
        return 0;
    }

    memset(&rStatus, 0, sizeof(rStatus));
    ULARGE_INTEGER ul;

    ul.LowPart = info.ftCreationTime.dwLowDateTime;
    ul.HighPart = info.ftCreationTime.dwHighDateTime;
    rStatus.m_ctime = ul.QuadPart;

    ul.LowPart = info.ftLastWriteTime.dwLowDateTime;
    ul.HighPart = info.ftLastWriteTime.dwHighDateTime;
    rStatus.m_mtime = ul.QuadPart;

    ul.LowPart = info.ftLastAccessTime.dwLowDateTime;
    ul.HighPart = info.ftLastAccessTime.dwHighDateTime;
    rStatus.m_atime = ul.QuadPart;

    ul.LowPart = info.nFileSizeLow;
    ul.HighPart = info.nFileSizeHigh;
    rStatus.m_size = ul.QuadPart;
    rStatus.m_attribute = (BYTE)info.dwFileAttributes;
    rStatus.m_szFullName[0] = L'\0';
    return 1;
}

int CFile::GetStatus(const wchar_t* lpszFileName, CFileStatus& rStatus, void* pTM) { (void)pTM;
    if (!lpszFileName) return 0;

    WIN32_FILE_ATTRIBUTE_DATA data;
    if (!GetFileAttributesExW(lpszFileName, GetFileExInfoStandard, &data)) {
        return 0;
    }

    // Convert FILETIME to ULONGLONG
    ULARGE_INTEGER ul;
    ul.LowPart = data.ftCreationTime.dwLowDateTime;
    ul.HighPart = data.ftCreationTime.dwHighDateTime;
    rStatus.m_ctime = ul.QuadPart;

    ul.LowPart = data.ftLastWriteTime.dwLowDateTime;
    ul.HighPart = data.ftLastWriteTime.dwHighDateTime;
    rStatus.m_mtime = ul.QuadPart;

    ul.LowPart = data.ftLastAccessTime.dwLowDateTime;
    ul.HighPart = data.ftLastAccessTime.dwHighDateTime;
    rStatus.m_atime = ul.QuadPart;

    ul.LowPart = data.nFileSizeLow;
    ul.HighPart = data.nFileSizeHigh;
    rStatus.m_size = ul.QuadPart;

    rStatus.m_attribute = (BYTE)data.dwFileAttributes;

    // Copy full path
    wcsncpy(rStatus.m_szFullName, lpszFileName, _MAX_PATH - 1);
    rStatus.m_szFullName[_MAX_PATH - 1] = L'\0';

    return 1;
}

void CFile::SetStatus(const wchar_t* lpszFileName, const CFileStatus& status) {
    if (!lpszFileName) return;

    // Set file attributes
    SetFileAttributesW(lpszFileName, status.m_attribute);

    // Set file times (requires opening the file)
    HANDLE hFile = CreateFileW(lpszFileName, FILE_WRITE_ATTRIBUTES,
                               FILE_SHARE_READ | FILE_SHARE_WRITE,
                               nullptr, OPEN_EXISTING, 0, nullptr);
    if (hFile != INVALID_HANDLE_VALUE) {
        FILETIME ftCreate, ftAccess, ftWrite;
        ULARGE_INTEGER ul;

        ul.QuadPart = status.m_ctime;
        ftCreate.dwLowDateTime = ul.LowPart;
        ftCreate.dwHighDateTime = ul.HighPart;

        ul.QuadPart = status.m_atime;
        ftAccess.dwLowDateTime = ul.LowPart;
        ftAccess.dwHighDateTime = ul.HighPart;

        ul.QuadPart = status.m_mtime;
        ftWrite.dwLowDateTime = ul.LowPart;
        ftWrite.dwHighDateTime = ul.HighPart;

        SetFileTime(hFile, &ftCreate, &ftAccess, &ftWrite);
        CloseHandle(hFile);
    }
}

// =============================================================================
// CMemFile Implementation
// =============================================================================

CMemFile::CMemFile(UINT nGrowBytes)
    : m_lpBuffer(nullptr), m_nBufferSize(0), m_nFileSize(0),
      m_nGrowBytes(nGrowBytes), m_nPosition(0), m_bAutoDelete(true)
{
    m_hFile = INVALID_HANDLE_VALUE;
}

CMemFile::CMemFile(BYTE* lpBuffer, UINT nBufferSize, UINT nGrowBytes)
    : m_lpBuffer(lpBuffer), m_nBufferSize(nBufferSize), m_nFileSize(nBufferSize),
      m_nGrowBytes(nGrowBytes), m_nPosition(0), m_bAutoDelete(false)
{
    m_hFile = INVALID_HANDLE_VALUE;
}

CMemFile::~CMemFile() {
    if (m_bAutoDelete && m_lpBuffer) {
        free(m_lpBuffer);
    }
}

UINT CMemFile::Read(void* lpBuf, UINT nCount) {
    if (!lpBuf || nCount == 0) return 0;

    UINT nAvail = (m_nPosition < m_nFileSize) ? (m_nFileSize - m_nPosition) : 0;
    UINT nRead = (nCount < nAvail) ? nCount : nAvail;

    if (nRead > 0 && m_lpBuffer) {
        memcpy(lpBuf, m_lpBuffer + m_nPosition, nRead);
        m_nPosition += nRead;
    }

    return nRead;
}

void CMemFile::Write(const void* lpBuf, UINT nCount) {
    if (!lpBuf || nCount == 0) return;

    // Grow buffer if needed
    UINT nNewSize = m_nPosition + nCount;
    if (nNewSize > m_nBufferSize) {
        UINT nGrow = (m_nGrowBytes > 0) ? m_nGrowBytes : 1024;
        UINT nNewBufSize = ((nNewSize + nGrow - 1) / nGrow) * nGrow;

        BYTE* pNewBuf = (BYTE*)realloc(m_lpBuffer, nNewBufSize);
        if (!pNewBuf) return;

        m_lpBuffer = pNewBuf;
        m_nBufferSize = nNewBufSize;
        m_bAutoDelete = true;
    }

    memcpy(m_lpBuffer + m_nPosition, lpBuf, nCount);
    m_nPosition += nCount;

    if (m_nPosition > m_nFileSize) {
        m_nFileSize = m_nPosition;
    }
}

ULONGLONG CMemFile::Seek(LONGLONG lOff, UINT nFrom) {
    LONGLONG lNewPos;
    switch (nFrom) {
        case begin:   lNewPos = lOff; break;
        case current: lNewPos = m_nPosition + lOff; break;
        case end:     lNewPos = m_nFileSize + lOff; break;
        default:      lNewPos = lOff; break;
    }

    if (lNewPos < 0) lNewPos = 0;
    m_nPosition = (UINT)lNewPos;
    return m_nPosition;
}

void CMemFile::SetLength(ULONGLONG dwNewLen) {
    UINT nNewLen = (UINT)dwNewLen;

    if (nNewLen > m_nBufferSize) {
        UINT nGrow = (m_nGrowBytes > 0) ? m_nGrowBytes : 1024;
        UINT nNewBufSize = ((nNewLen + nGrow - 1) / nGrow) * nGrow;

        BYTE* pNewBuf = (BYTE*)realloc(m_lpBuffer, nNewBufSize);
        if (!pNewBuf) return;

        m_lpBuffer = pNewBuf;
        m_nBufferSize = nNewBufSize;
        m_bAutoDelete = true;
    }

    m_nFileSize = nNewLen;
    if (m_nPosition > m_nFileSize) {
        m_nPosition = m_nFileSize;
    }
}

ULONGLONG CMemFile::GetLength() const {
    return m_nFileSize;
}

void CMemFile::Flush() {
    // Nothing to flush for memory file
}

BYTE* CMemFile::Detach() {
    BYTE* lpBuffer = m_lpBuffer;
    m_lpBuffer = nullptr;
    m_nBufferSize = 0;
    m_nFileSize = 0;
    m_nPosition = 0;
    m_bAutoDelete = true;
    return lpBuffer;
}

void CMemFile::Attach(BYTE* lpBuffer, UINT nBufferSize, UINT nGrowBytes) {
    if (m_bAutoDelete && m_lpBuffer) {
        free(m_lpBuffer);
    }
    m_lpBuffer = lpBuffer;
    m_nBufferSize = nBufferSize;
    m_nFileSize = nBufferSize;
    m_nGrowBytes = nGrowBytes;
    m_nPosition = 0;
    m_bAutoDelete = false;
}

// =============================================================================
// CStdioFile Implementation
// =============================================================================

CStdioFile::CStdioFile() : m_pStream(nullptr) {
    m_hFile = INVALID_HANDLE_VALUE;
}

CStdioFile::CStdioFile(const wchar_t* lpszFileName, UINT nOpenFlags) : m_pStream(nullptr) {
    m_hFile = INVALID_HANDLE_VALUE;
    if (!lpszFileName) return;

    m_strFileName = lpszFileName;

    // Build mode string
    wchar_t szMode[4] = { 0 };
    int nModeIdx = 0;

    bool bText = (nOpenFlags & typeText) != 0;

    if ((nOpenFlags & 0x0003) == modeRead) {
        szMode[nModeIdx++] = L'r';
    } else if ((nOpenFlags & 0x0003) == modeWrite) {
        szMode[nModeIdx++] = L'w';
    } else if ((nOpenFlags & 0x0003) == modeReadWrite) {
        if (nOpenFlags & modeCreate) {
            szMode[nModeIdx++] = L'w';
        } else {
            szMode[nModeIdx++] = L'r';
        }
        szMode[nModeIdx++] = L'+';
    }

    if (!bText) {
        szMode[nModeIdx++] = L'b';
    }

    m_pStream = _wfopen(lpszFileName, szMode);
}

CStdioFile::~CStdioFile() {
    if (m_pStream) {
        fclose(m_pStream);
    }
}

UINT CStdioFile::Read(void* lpBuf, UINT nCount) {
    if (!m_pStream || !lpBuf || nCount == 0) return 0;
    return (UINT)fread(lpBuf, 1, nCount, m_pStream);
}

void CStdioFile::Write(const void* lpBuf, UINT nCount) {
    if (!m_pStream || !lpBuf || nCount == 0) return;
    fwrite(lpBuf, 1, nCount, m_pStream);
}

ULONGLONG CStdioFile::Seek(LONGLONG lOff, UINT nFrom) {
    if (!m_pStream) return 0;

    int nOrigin;
    switch (nFrom) {
        case begin:   nOrigin = SEEK_SET; break;
        case current: nOrigin = SEEK_CUR; break;
        case end:     nOrigin = SEEK_END; break;
        default:      nOrigin = SEEK_SET; break;
    }

    if (_fseeki64(m_pStream, lOff, nOrigin) != 0) {
        return 0;
    }
    return _ftelli64(m_pStream);
}

void CStdioFile::Flush() {
    if (m_pStream) {
        fflush(m_pStream);
    }
}

void CStdioFile::Close() {
    if (m_pStream) {
        fclose(m_pStream);
        m_pStream = nullptr;
    }
}

wchar_t* CStdioFile::ReadString(wchar_t* lpsz, UINT nMax) {
    if (!m_pStream || !lpsz || nMax == 0) return nullptr;
    return fgetws(lpsz, nMax, m_pStream);
}

int CStdioFile::ReadString(CString& rString) {
    rString.Empty();
    if (!m_pStream) return 0;

    wchar_t buf[256];
    while (fgetws(buf, 256, m_pStream)) {
        rString += buf;
        // Check if we got a complete line
        int len = rString.GetLength();
        if (len > 0 && ((const wchar_t*)rString)[len - 1] == L'\n') {
            break;
        }
    }
    return rString.GetLength() > 0 ? 1 : 0;
}

void CStdioFile::WriteString(const wchar_t* lpsz) {
    if (!m_pStream || !lpsz) return;
    fputws(lpsz, m_pStream);
}

// =============================================================================
// CArchive Implementation
// =============================================================================

CArchive::CArchive(CFile* pFile, UINT nMode, int nBufSize, void* lpBuf)
    : m_pFile(pFile), m_nMode(nMode), m_nBufSize(nBufSize),
      m_nObjectSchema(0), m_bForceFlat(false)
{
    if (lpBuf) {
        m_lpBufStart = static_cast<unsigned char*>(lpBuf);
        m_bUserBuf = true;
    } else {
        m_lpBufStart = new unsigned char[nBufSize];
        m_bUserBuf = false;
    }

    if (IsLoading()) {
        m_lpBufCur = m_lpBufStart;
        m_lpBufMax = m_lpBufStart;  // Empty buffer, will be filled on first read
    } else {
        m_lpBufCur = m_lpBufStart;
        m_lpBufMax = m_lpBufStart + nBufSize;
    }
}

CArchive::~CArchive() {
    if (!(m_nMode & bNoFlushOnDelete)) {
        Flush();
    }
    if (!m_bUserBuf) {
        delete[] m_lpBufStart;
    }
}

void CArchive::FillBuffer(UINT nBytesNeeded) {
    if (!m_pFile || !m_lpBufStart || m_nBufSize <= 0) return;

    // Move remaining data to start of buffer
    UINT nRemaining = (UINT)(m_lpBufMax - m_lpBufCur);
    if (nRemaining > 0 && m_lpBufCur != m_lpBufStart) {
        memmove(m_lpBufStart, m_lpBufCur, nRemaining);
    }
    m_lpBufCur = m_lpBufStart;

    if (nRemaining >= nBytesNeeded) {
        m_lpBufMax = m_lpBufStart + nRemaining;
        return;
    }

    UINT capacity = static_cast<UINT>(m_nBufSize);
    UINT bytesToRead = capacity - nRemaining;

    UINT nRead = OpenMFC_File_Read(m_pFile, m_lpBufStart + nRemaining, bytesToRead);
    m_lpBufMax = m_lpBufStart + nRemaining + nRead;
}

class CArchiveAccess {
public:
    static void InvokeFillBuffer(CArchive* pArchive, unsigned int nBytesNeeded) {
        if (pArchive) {
            pArchive->FillBuffer(nBytesNeeded);
        }
    }
};

// Symbol: ?FillBuffer@CArchive@@QEAAXI@Z
extern "C" void MS_ABI impl__FillBuffer_CArchive__QEAAXI_Z(CArchive* pThis, unsigned int nBytesNeeded) {
    CArchiveAccess::InvokeFillBuffer(pThis, nBytesNeeded);
}

void CArchive::WriteBuffer() {
    if (!m_pFile) return;

    UINT nBytes = (UINT)(m_lpBufCur - m_lpBufStart);
    if (nBytes > 0) {
        OpenMFC_File_Write(m_pFile, m_lpBufStart, nBytes);
        m_lpBufCur = m_lpBufStart;
    }
}

UINT CArchive::Read(void* lpBuf, UINT nMax) {
    if (!IsLoading() || !lpBuf || nMax == 0) return 0;

    UINT nRead = 0;
    unsigned char* pDest = static_cast<unsigned char*>(lpBuf);

    while (nRead < nMax) {
        UINT nAvail = (UINT)(m_lpBufMax - m_lpBufCur);
        if (nAvail == 0) {
            FillBuffer(nMax - nRead);
            nAvail = (UINT)(m_lpBufMax - m_lpBufCur);
            if (nAvail == 0) break;  // EOF
        }

        UINT nCopy = (nMax - nRead < nAvail) ? (nMax - nRead) : nAvail;
        memcpy(pDest + nRead, m_lpBufCur, nCopy);
        m_lpBufCur += nCopy;
        nRead += nCopy;
    }

    return nRead;
}

void CArchive::Write(const void* lpBuf, UINT nMax) {
    if (!IsStoring() || !lpBuf || nMax == 0) return;

    const unsigned char* pSrc = static_cast<const unsigned char*>(lpBuf);
    UINT nWritten = 0;

    while (nWritten < nMax) {
        UINT nAvail = (UINT)(m_lpBufMax - m_lpBufCur);
        if (nAvail == 0) {
            WriteBuffer();
            nAvail = (UINT)(m_lpBufMax - m_lpBufCur);
        }

        UINT nCopy = (nMax - nWritten < nAvail) ? (nMax - nWritten) : nAvail;
        memcpy(m_lpBufCur, pSrc + nWritten, nCopy);
        m_lpBufCur += nCopy;
        nWritten += nCopy;
    }
}

void CArchive::Flush() {
    if (IsStoring()) {
        WriteBuffer();
    }
    if (m_pFile) {
        OpenMFC_File_Flush(m_pFile);
    }
}

void CArchive::Close() {
    Flush();
}

void CArchive::Abort() {
    // Reset without flushing
    m_lpBufCur = m_lpBufStart;
    if (IsLoading()) {
        m_lpBufMax = m_lpBufStart;
    }
}

// Reading operators
CArchive& CArchive::operator>>(bool& b) {
    char ch;
    Read(&ch, sizeof(ch));
    b = (ch != 0);
    return *this;
}

CArchive& CArchive::operator>>(char& ch) {
    Read(&ch, sizeof(ch));
    return *this;
}

CArchive& CArchive::operator>>(unsigned char& ch) {
    Read(&ch, sizeof(ch));
    return *this;
}

CArchive& CArchive::operator>>(short& w) {
    Read(&w, sizeof(w));
    return *this;
}

CArchive& CArchive::operator>>(unsigned short& w) {
    Read(&w, sizeof(w));
    return *this;
}

CArchive& CArchive::operator>>(int& i) {
    Read(&i, sizeof(i));
    return *this;
}

CArchive& CArchive::operator>>(unsigned int& i) {
    Read(&i, sizeof(i));
    return *this;
}

CArchive& CArchive::operator>>(long& l) {
    Read(&l, sizeof(l));
    return *this;
}

CArchive& CArchive::operator>>(unsigned long& l) {
    Read(&l, sizeof(l));
    return *this;
}

CArchive& CArchive::operator>>(float& f) {
    Read(&f, sizeof(f));
    return *this;
}

CArchive& CArchive::operator>>(double& d) {
    Read(&d, sizeof(d));
    return *this;
}

CArchive& CArchive::operator>>(CObject*& pOb) {
    unsigned char hasObject = 0;
    if (Read(&hasObject, sizeof(hasObject)) != sizeof(hasObject) || hasObject == 0) {
        pOb = nullptr;
        return *this;
    }

    unsigned int nSchema = 0xFFFF;
    CRuntimeClass* pClass = impl__Load_CRuntimeClass__SAPEAU1_AEAVCArchive__PEAI_Z(this, &nSchema);
    SetObjectSchema(nSchema);

    if (!pClass || !pClass->m_pfnCreateObject) {
        pOb = nullptr;
        return *this;
    }

    pOb = pClass->CreateObject();
    if (pOb) {
        pOb->Serialize(*this);
    }
    return *this;
}

CArchive& CArchive::operator>>(void*& p) {
    uintptr_t value = 0;
    Read(&value, sizeof(value));
    p = reinterpret_cast<void*>(value);
    return *this;
}

CArchive& CArchive::operator>>(CString& str) {
    // Read string length
    UINT nLen = 0;
    Read(&nLen, sizeof(nLen));

    if (nLen > 0 && nLen < 0x10000000) {
        wchar_t* pBuf = str.GetBuffer(nLen + 1);
        Read(pBuf, nLen * sizeof(wchar_t));
        pBuf[nLen] = L'\0';
        str.ReleaseBuffer(nLen);
    } else {
        str.Empty();
    }
    return *this;
}

// Writing operators
CArchive& CArchive::operator<<(bool b) {
    char ch = b ? 1 : 0;
    Write(&ch, sizeof(ch));
    return *this;
}

CArchive& CArchive::operator<<(char ch) {
    Write(&ch, sizeof(ch));
    return *this;
}

CArchive& CArchive::operator<<(unsigned char ch) {
    Write(&ch, sizeof(ch));
    return *this;
}

CArchive& CArchive::operator<<(short w) {
    Write(&w, sizeof(w));
    return *this;
}

CArchive& CArchive::operator<<(unsigned short w) {
    Write(&w, sizeof(w));
    return *this;
}

CArchive& CArchive::operator<<(int i) {
    Write(&i, sizeof(i));
    return *this;
}

CArchive& CArchive::operator<<(unsigned int i) {
    Write(&i, sizeof(i));
    return *this;
}

CArchive& CArchive::operator<<(long l) {
    Write(&l, sizeof(l));
    return *this;
}

CArchive& CArchive::operator<<(unsigned long l) {
    Write(&l, sizeof(l));
    return *this;
}

CArchive& CArchive::operator<<(float f) {
    Write(&f, sizeof(f));
    return *this;
}

CArchive& CArchive::operator<<(double d) {
    Write(&d, sizeof(d));
    return *this;
}

CArchive& CArchive::operator<<(const CObject* pOb) {
    unsigned char hasObject = (pOb != nullptr) ? 1 : 0;
    Write(&hasObject, sizeof(hasObject));

    if (!hasObject) {
        return *this;
    }

    const CRuntimeClass* pClass = pOb->GetRuntimeClass();
    if (pClass == nullptr) {
        pClass = &CObject::classCObject;
    }
    impl__Store_CRuntimeClass__QEBAXAEAVCArchive___Z(pClass, this);

    // Conservative format: only write payload if load side can create the object.
    if (pClass->m_pfnCreateObject) {
        const_cast<CObject*>(pOb)->Serialize(*this);
    }
    return *this;
}

CArchive& CArchive::operator<<(const void* p) {
    uintptr_t value = reinterpret_cast<uintptr_t>(p);
    Write(&value, sizeof(value));
    return *this;
}

CArchive& CArchive::operator<<(const CString& str) {
    // Write string length and data
    UINT nLen = str.GetLength();
    Write(&nLen, sizeof(nLen));

    if (nLen > 0) {
        Write((const wchar_t*)str, nLen * sizeof(wchar_t));
    }
    return *this;
}

// String operations
int CArchive::ReadString(wchar_t* lpsz, UINT nMax) {
    if (!lpsz || nMax == 0) return 0;

    UINT nRead = 0;
    while (nRead < nMax - 1) {
        wchar_t ch;
        if (Read(&ch, sizeof(ch)) != sizeof(ch)) break;
        if (ch == L'\n') {
            lpsz[nRead++] = ch;
            break;
        }
        if (ch == L'\r') continue;  // Skip CR
        lpsz[nRead++] = ch;
    }
    lpsz[nRead] = L'\0';
    return nRead;
}

int CArchive::ReadString(CString& rString) {
    rString.Empty();
    wchar_t buf[256];
    int nTotal = 0;

    while (true) {
        int nRead = ReadString(buf, 256);
        if (nRead == 0) break;
        rString += buf;
        nTotal += nRead;
        if (buf[nRead - 1] == L'\n') break;
    }
    return nTotal;
}

void CArchive::WriteString(const wchar_t* lpsz) {
    if (!lpsz) return;
    UINT nLen = (UINT)wcslen(lpsz);
    Write(lpsz, nLen * sizeof(wchar_t));
}

// Symbol: ?GetObjectSchema@CArchive@@QEAAIXZ
extern "C" unsigned int MS_ABI impl__GetObjectSchema_CArchive__QEAAIXZ(CArchive* pThis) {
    return pThis ? pThis->GetObjectSchema() : 0;
}

// Symbol: ?WriteCount@CArchive@@QEAAX_K@Z
extern "C" void MS_ABI impl__WriteCount_CArchive__QEAAX_K_Z(CArchive* pThis, unsigned long long count) {
    if (!pThis) return;
    pThis->Write(&count, sizeof(count));
}

// Symbol: ?ReadCount@CArchive@@QEAA_KXZ
extern "C" unsigned long long MS_ABI impl__ReadCount_CArchive__QEAA_KXZ(CArchive* pThis) {
    unsigned long long count = 0;
    if (pThis) pThis->Read(&count, sizeof(count));
    return count;
}

// Symbol: ?CheckCount@CArchive@@QEAAXXZ
extern "C" void MS_ABI impl__CheckCount_CArchive__QEAAXXZ(CArchive* pThis) {
    if (pThis) pThis->Flush();
}

// Symbol: ?WriteClass@CArchive@@QEAAXPEBUCRuntimeClass@@@Z
extern "C" void MS_ABI impl__WriteClass_CArchive__QEAAXPEBUCRuntimeClass___Z(CArchive* pThis, const CRuntimeClass* pClass) {
    if (pThis && pClass) impl__Store_CRuntimeClass__QEBAXAEAVCArchive___Z(pClass, pThis);
}

// Symbol: ?SerializeClass@CArchive@@QEAAXPEBUCRuntimeClass@@@Z
extern "C" void MS_ABI impl__SerializeClass_CArchive__QEAAXPEBUCRuntimeClass___Z(CArchive* pThis, const CRuntimeClass* pClass) {
    if (pThis && pThis->IsStoring()) impl__WriteClass_CArchive__QEAAXPEBUCRuntimeClass___Z(pThis, pClass);
    else if (pThis) {
        unsigned int schema = 0;
        (void)impl__Load_CRuntimeClass__SAPEAU1_AEAVCArchive__PEAI_Z(pThis, &schema);
        pThis->SetObjectSchema(schema);
    }
}

// Symbol: ?ReadClass@CArchive@@QEAAPEAUCRuntimeClass@@PEBU2@PEAIPEAK@Z
extern "C" CRuntimeClass* MS_ABI impl__ReadClass_CArchive__QEAAPEAUCRuntimeClass__PEBU2_PEAIPEAK_Z(
    CArchive* pThis, const CRuntimeClass*, unsigned int* schema, unsigned long*) {
    return pThis ? impl__Load_CRuntimeClass__SAPEAU1_AEAVCArchive__PEAI_Z(pThis, schema) : nullptr;
}

// Symbol: ?WriteObject@CArchive@@QEAAXPEBVCObject@@@Z
extern "C" void MS_ABI impl__WriteObject_CArchive__QEAAXPEBVCObject___Z(CArchive* pThis, const CObject* object) {
    if (pThis) (*pThis) << object;
}

// Symbol: ?ReadObject@CArchive@@QEAAPEAVCObject@@PEBUCRuntimeClass@@@Z
extern "C" CObject* MS_ABI impl__ReadObject_CArchive__QEAAPEAVCObject__PEBUCRuntimeClass___Z(CArchive* pThis, const CRuntimeClass*) {
    CObject* object = nullptr;
    if (pThis) (*pThis) >> object;
    return object;
}

// Symbol: ?ReadString@CArchive@@QEAAHAEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@@Z
extern "C" int MS_ABI impl__ReadString_CArchive__QEAAHAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(CArchive* pThis, CString* stringOut) {
    return (pThis && stringOut) ? pThis->ReadString(*stringOut) : 0;
}

// Symbol: ?MapObject@CArchive@@QEAAXPEBVCObject@@@Z
extern "C" void MS_ABI impl__MapObject_CArchive__QEAAXPEBVCObject___Z(CArchive*, const CObject*) {
}

// Symbol: ?EnsureSchemaMapExists@CArchive@@QEAAXPEAPEAV?$CArray@W4LoadArrayObjType@CArchive@@AEBW412@@@@Z
extern "C" void MS_ABI impl__EnsureSchemaMapExists_CArchive__QEAAXPEAPEAV__CArray_W4LoadArrayObjType_CArchive__AEBW412____Z(CArchive*, void**) {
}

// =============================================================================
// CFile extern "C" MS_ABI Thunks
// These are cross-ABI vtable entry points for CFile virtual methods.
// =============================================================================

// Symbol: ?GetLength@CFile@@UEBA_KXZ
extern "C" unsigned long long MS_ABI impl__GetLength_CFile__UEBA_KXZ(void* pThis) {
    CFile* self = static_cast<CFile*>(pThis);
    if (!self || self->m_hFile == (void*)INVALID_HANDLE_VALUE) {
        return 0;
    }
    LARGE_INTEGER li;
    li.QuadPart = 0;
    if (GetFileSizeEx((HANDLE)self->m_hFile, &li))
        return li.QuadPart;
    return 0;
}

// Symbol: ?Seek@CFile@@UEAA_K_JI@Z
extern "C" unsigned long long MS_ABI impl__Seek_CFile__UEAA_K_JI_Z(void* pThis, long long lOff, unsigned int nFrom) {
    CFile* self = static_cast<CFile*>(pThis);
    if (!self || self->m_hFile == (void*)INVALID_HANDLE_VALUE) {
        return 0;
    }
    LARGE_INTEGER li;
    li.QuadPart = lOff;
    LARGE_INTEGER result;
    if (SetFilePointerEx((HANDLE)self->m_hFile, li, &result, nFrom))
        return result.QuadPart;
    return 0;
}

// Symbol: ?Read@CFile@@UEAAIPEAXI@Z
extern "C" unsigned int MS_ABI impl__Read_CFile__UEAAIPEAXI_Z(void* pThis, void* lpBuf, unsigned int nCount) {
    CFile* self = static_cast<CFile*>(pThis);
    if (!self || self->m_hFile == (void*)INVALID_HANDLE_VALUE || !lpBuf) {
        return 0;
    }
    if (nCount == 0) {
        return 0;
    }
    DWORD dwRead = 0;
    ReadFile((HANDLE)self->m_hFile, lpBuf, nCount, &dwRead, nullptr);
    return dwRead;
}

// Symbol: ?Write@CFile@@UEAAXPEBXI@Z
extern "C" void MS_ABI impl__Write_CFile__UEAAXPEBXI_Z(void* pThis, const void* lpBuf, unsigned int nCount) {
    CFile* self = static_cast<CFile*>(pThis);
    if (!self || self->m_hFile == (void*)INVALID_HANDLE_VALUE || !lpBuf) {
        return;
    }
    if (nCount == 0) {
        return;
    }
    DWORD dwWritten = 0;
    WriteFile((HANDLE)self->m_hFile, lpBuf, nCount, &dwWritten, nullptr);
}

// Symbol: ?GetPosition@CFile@@UEBA_KXZ
extern "C" unsigned long long MS_ABI impl__GetPosition_CFile__UEBA_KXZ(void* pThis) {
    CFile* self = static_cast<CFile*>(pThis);
    if (!self || self->m_hFile == (void*)INVALID_HANDLE_VALUE) {
        return 0;
    }
    LARGE_INTEGER li, result;
    li.QuadPart = 0;
    if (SetFilePointerEx((HANDLE)self->m_hFile, li, &result, FILE_CURRENT))
        return result.QuadPart;
    return 0;
}

// Symbol: ?Flush@CFile@@UEAAXXZ
extern "C" void MS_ABI impl__Flush_CFile__UEAAXXZ(void* pThis) {
    CFile* self = static_cast<CFile*>(pThis);
    if (!self || self->m_hFile == (void*)INVALID_HANDLE_VALUE) {
        return;
    }
    FlushFileBuffers((HANDLE)self->m_hFile);
}

// Symbol: ?Close@CFile@@UEAAXXZ
extern "C" void MS_ABI impl__Close_CFile__UEAAXXZ(void* pThis) {
    CFile* self = static_cast<CFile*>(pThis);
    if (self->m_hFile != (void*)INVALID_HANDLE_VALUE) {
        CloseHandle((HANDLE)self->m_hFile);
        self->m_hFile = (void*)INVALID_HANDLE_VALUE;
    }
}

// Symbol: ?Abort@CFile@@UEAAXXZ
extern "C" void MS_ABI impl__Abort_CFile__UEAAXXZ(void* pThis) {
    CFile* self = static_cast<CFile*>(pThis);
    if (self->m_hFile != (void*)INVALID_HANDLE_VALUE) {
        CloseHandle((HANDLE)self->m_hFile);
        self->m_hFile = (void*)INVALID_HANDLE_VALUE;
    }
}

// Symbol: ?GetFileName@CFile@@UEBA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@XZ
extern "C" void MS_ABI impl__GetFileName_CFile__UEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__XZ(void* pThis, void* ret) {
    CFile* self = static_cast<CFile*>(pThis);
    new(ret) CString(self->GetFileName());
}

// Symbol: ?Duplicate@CFile@@UEBAPEAV1@XZ
extern "C" void* MS_ABI impl__Duplicate_CFile__UEBAPEAV1_XZ(void* pThis) {
    CFile* self = static_cast<CFile*>(pThis);
    CFile* pDup = new CFile();
    HANDLE hDup = INVALID_HANDLE_VALUE;
    if (self->m_hFile != (void*)INVALID_HANDLE_VALUE) {
        HANDLE hProc = GetCurrentProcess();
        DuplicateHandle(hProc, (HANDLE)self->m_hFile, hProc, &hDup, 0, FALSE, DUPLICATE_SAME_ACCESS);
    }
    pDup->m_hFile = hDup;
    return pDup;
}

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

// Symbol: ?GetFilePath@CFile@@UEBA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@XZ
extern "C" void MS_ABI impl__GetFilePath_CFile__UEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__XZ(void* pThis, void* ret) {
    CFile* self = static_cast<CFile*>(pThis);
    new(ret) CString(self->GetFilePath());
}

// Symbol: ?GetFileTitle@CFile@@UEBA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@XZ
extern "C" void MS_ABI impl__GetFileTitle_CFile__UEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__XZ(void* pThis, void* ret) {
    CFile* self = static_cast<CFile*>(pThis);
    new(ret) CString(self->GetFileTitle());
}

// Symbol: ?GetBufferPtr@CFile@@UEAAIIIPEAPEAX0@Z
extern "C" unsigned int MS_ABI impl__GetBufferPtr_CFile__UEAAIIIPEAPEAX0_Z(
    void* /*pThis*/, unsigned int /*nCommand*/, unsigned int /*nCount*/, void** ppBufStart, void** ppBufMax) {
    if (ppBufStart) *ppBufStart = nullptr;
    if (ppBufMax) *ppBufMax = nullptr;
    return 0;
}

// Symbol: ?CommonBaseInit@CFile@@IEAAXPEAXPEAVCAtlTransactionManager@ATL@@@Z
extern "C" void MS_ABI impl__CommonBaseInit_CFile__IEAAXPEAXPEAVCAtlTransactionManager_ATL___Z(
    void* pThis, void* hFile, void* /*pTM*/) {
    CFile* self = static_cast<CFile*>(pThis);
    self->m_hFile = hFile ? hFile : (void*)INVALID_HANDLE_VALUE;
}

// Symbol: ?CommonInit@CFile@@IEAAXPEB_WIPEAVCAtlTransactionManager@ATL@@@Z
extern "C" void MS_ABI impl__CommonInit_CFile__IEAAXPEB_WIPEAVCAtlTransactionManager_ATL___Z(
    void* pThis, const wchar_t* lpszFileName, unsigned int nOpenFlags, void* pTM) {
    if (!pThis) {
        return;
    }
    CFileAccessor::InvokeCommonInit(static_cast<CFile*>(pThis), lpszFileName, nOpenFlags, pTM);
}

// Symbol: ?Rename@CFile@@SAXPEB_W0PEAVCAtlTransactionManager@ATL@@@Z
extern "C" void MS_ABI impl__Rename_CFile__SAXPEB_W0PEAVCAtlTransactionManager_ATL___Z(
    const wchar_t* lpszOldName, const wchar_t* lpszNewName, void* /*pTM*/) {
    CFile::Rename(lpszOldName, lpszNewName);
}

// Symbol: ?Remove@CFile@@SAXPEB_WPEAVCAtlTransactionManager@ATL@@@Z
extern "C" void MS_ABI impl__Remove_CFile__SAXPEB_WPEAVCAtlTransactionManager_ATL___Z(
    const wchar_t* lpszFileName, void* /*pTM*/) {
    CFile::Remove(lpszFileName);
}

// Symbol: ?SetStatus@CFile@@SAXPEB_WAEBUCFileStatus@@PEAVCAtlTransactionManager@ATL@@@Z
extern "C" void MS_ABI impl__SetStatus_CFile__SAXPEB_WAEBUCFileStatus__PEAVCAtlTransactionManager_ATL___Z(
    const wchar_t* lpszFileName, const CFileStatus* pStatus, void* /*pTM*/) {
    if (!pStatus) {
        return;
    }
    CFile::SetStatus(lpszFileName, *pStatus);
}

// Symbol: ?Open@CFile@@UEAAHPEB_WIPEAVCFileException@@@Z
extern "C" int MS_ABI impl__Open_CFile__UEAAHPEB_WIPEAVCFileException___Z(
    void* pThis, const wchar_t* lpszFileName, unsigned int nOpenFlags, void* /*pException*/) {
    CFile* self = static_cast<CFile*>(pThis);
    CFileAccessor::InvokeCommonInit(self, lpszFileName, nOpenFlags, nullptr);
    return self->m_hFile != (void*)INVALID_HANDLE_VALUE ? 1 : 0;
}

// Symbol: ?Open@CFile@@UEAAHPEB_WIPEAVCAtlTransactionManager@ATL@@PEAVCFileException@@@Z
extern "C" int MS_ABI impl__Open_CFile__UEAAHPEB_WIPEAVCAtlTransactionManager_ATL__PEAVCFileException___Z(
    void* pThis, const wchar_t* lpszFileName, unsigned int nOpenFlags, void* pTM, void* pException) {
    (void)pTM;
    return impl__Open_CFile__UEAAHPEB_WIPEAVCFileException___Z(pThis, lpszFileName, nOpenFlags, pException);
}

// Symbol: ?LockRange@CFile@@UEAAX_K0@Z
extern "C" void MS_ABI impl__LockRange_CFile__UEAAX_K0_Z(void* pThis, unsigned long long dwPos, unsigned long long dwCount) {
    CFile* self = static_cast<CFile*>(pThis);
    if (self->m_hFile == (void*)INVALID_HANDLE_VALUE) return;
    OVERLAPPED ov = {};
    ov.Offset = static_cast<DWORD>(dwPos & 0xFFFFFFFFULL);
    ov.OffsetHigh = static_cast<DWORD>(dwPos >> 32);
    LockFileEx((HANDLE)self->m_hFile, LOCKFILE_EXCLUSIVE_LOCK, 0,
               static_cast<DWORD>(dwCount & 0xFFFFFFFFULL), static_cast<DWORD>(dwCount >> 32), &ov);
}

// Symbol: ?UnlockRange@CFile@@UEAAX_K0@Z
extern "C" void MS_ABI impl__UnlockRange_CFile__UEAAX_K0_Z(void* pThis, unsigned long long dwPos, unsigned long long dwCount) {
    CFile* self = static_cast<CFile*>(pThis);
    if (self->m_hFile == (void*)INVALID_HANDLE_VALUE) return;
    OVERLAPPED ov = {};
    ov.Offset = static_cast<DWORD>(dwPos & 0xFFFFFFFFULL);
    ov.OffsetHigh = static_cast<DWORD>(dwPos >> 32);
    UnlockFileEx((HANDLE)self->m_hFile, 0,
                 static_cast<DWORD>(dwCount & 0xFFFFFFFFULL), static_cast<DWORD>(dwCount >> 32), &ov);
}

// Symbol: ?GetStatus@CFile@@SAHPEB_WAEAUCFileStatus@@PEAVCAtlTransactionManager@ATL@@@Z
extern "C" int MS_ABI impl__GetStatus_CFile__SAHPEB_WAEAUCFileStatus__PEAVCAtlTransactionManager_ATL___Z(
    const wchar_t* lpszFileName, CFileStatus& rStatus, void* pTM) {
    return CFile::GetStatus(lpszFileName, rStatus, pTM);
}

// CFile runtime-class descriptor. CFile is DECLARE_DYNAMIC in real MFC: base
// CObject, schema 0xFFFF, no factory. m_nObjectSize is the real MSVC sizeof(CFile)
// (40, harvested from mfc140u.dll), not the host sizeof. Following the repo's
// IMPLEMENT_DYNAMIC convention (m_pfnGetBaseClass null, m_pBaseClass set; the
// exported IsDerivedFrom falls back to m_pBaseClass). This descriptor is an internal
// DLL symbol (not added to the .def) so the export set stays at the 14,109 match,
// but it gives CFile-derived RTTI getters a real base to chain to.
CRuntimeClass CFile::classCFile = {
    "CFile", 40, 0xFFFF, nullptr, nullptr, &CObject::classCObject, nullptr
};

// Symbol: ?GetThisClass@CFile@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CFile__SAPEAUCRuntimeClass__XZ() {
    return &CFile::classCFile;
}

// Symbol: ?GetRuntimeClass@CFile@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CFile__UEBAPEAUCRuntimeClass__XZ(const void* pThis) {
    (void)pThis;
    return &CFile::classCFile;
}

extern "C" void* impl__hFileNull_CFile__2QEAXEA = (void*)INVALID_HANDLE_VALUE;
// Symbol: ?hFileNull@CFile@@2QEAXEA
#ifdef __GNUC__
asm(".globl \"?hFileNull@CFile@@2QEAXEA\"\n"
    ".set \"?hFileNull@CFile@@2QEAXEA\", impl__hFileNull_CFile__2QEAXEA\n");
#endif

// Symbol: ?GetPosition@CMemFile@@UEBA_KXZ
extern "C" unsigned long long MS_ABI impl__GetPosition_CMemFile__UEBA_KXZ(const CMemFile* pThis) {
    return pThis ? CMemFileAccessor::Position(pThis) : 0;
}

// Symbol: ?Abort@CMemFile@@UEAAXXZ
extern "C" void MS_ABI impl__Abort_CMemFile__UEAAXXZ(CMemFile* pThis) {
    if (!pThis) return;
    CMemFileAccessor::Position(pThis) = 0;
}

// Symbol: ?Close@CMemFile@@UEAAXXZ
extern "C" void MS_ABI impl__Close_CMemFile__UEAAXXZ(CMemFile* pThis) {
    if (!pThis) return;
    CMemFileAccessor::Position(pThis) = 0;
}

// Symbol: ?Duplicate@CMemFile@@UEBAPEAVCFile@@XZ
extern "C" CFile* MS_ABI impl__Duplicate_CMemFile__UEBAPEAVCFile__XZ(const CMemFile* pThis) {
    if (!pThis) return nullptr;
    CMemFile* pDup = new CMemFile(CMemFileAccessor::GrowBytes(pThis));
    ULONGLONG len = pThis->GetLength();
    if (len > 0) {
        pDup->SetLength(len);
        memcpy(CMemFileAccessor::Buffer(pDup), CMemFileAccessor::Buffer(pThis), static_cast<size_t>(len));
    }
    CMemFileAccessor::Position(pDup) = CMemFileAccessor::Position(pThis);
    return pDup;
}

// Symbol: ?GetBufferPtr@CMemFile@@UEAAIIIPEAPEAX0@Z
extern "C" unsigned int MS_ABI impl__GetBufferPtr_CMemFile__UEAAIIIPEAPEAX0_Z(
    CMemFile* pThis, unsigned int /*nCommand*/, unsigned int nCount, void** ppBufStart, void** ppBufMax) {
    if (!pThis) return 0;
    if (ppBufStart) {
        BYTE* buf = CMemFileAccessor::Buffer(pThis);
        UINT pos = CMemFileAccessor::Position(pThis);
        UINT size = CMemFileAccessor::BufferSize(pThis);
        *ppBufStart = (buf && pos < size) ? (void*)(buf + pos) : nullptr;
    }
    if (ppBufMax) {
        BYTE* buf = CMemFileAccessor::Buffer(pThis);
        *ppBufMax = buf ? (void*)(buf + CMemFileAccessor::BufferSize(pThis)) : nullptr;
    }
    UINT pos = CMemFileAccessor::Position(pThis);
    UINT size = CMemFileAccessor::BufferSize(pThis);
    UINT nAvail = (pos < size) ? (size - pos) : 0;
    return (nCount < nAvail) ? nCount : nAvail;
}

// Symbol: ?GetStatus@CMemFile@@QEBAHAEAUCFileStatus@@@Z
extern "C" int MS_ABI impl__GetStatus_CMemFile__QEBAHAEAUCFileStatus___Z(const CMemFile* pThis, CFileStatus& rStatus) {
    if (!pThis) return 0;
    memset(&rStatus, 0, sizeof(rStatus));
    rStatus.m_size = pThis->GetLength();
    return 1;
}

// Symbol: ?Alloc@CMemFile@@MEAAPEAE_K@Z
extern "C" unsigned char* MS_ABI impl__Alloc_CMemFile__MEAAPEAE_K_Z(CMemFile* /*pThis*/, unsigned long long nBytes) {
    return static_cast<unsigned char*>(malloc(static_cast<size_t>(nBytes)));
}

// Symbol: ?Realloc@CMemFile@@MEAAPEAEPEAE_K@Z
extern "C" unsigned char* MS_ABI impl__Realloc_CMemFile__MEAAPEAEPEAE_K_Z(
    CMemFile* /*pThis*/, unsigned char* pMem, unsigned long long nBytes) {
    return static_cast<unsigned char*>(realloc(pMem, static_cast<size_t>(nBytes)));
}

// Symbol: ?Free@CMemFile@@MEAAXPEAE@Z
extern "C" void MS_ABI impl__Free_CMemFile__MEAAXPEAE_Z(CMemFile* /*pThis*/, unsigned char* pMem) {
    free(pMem);
}

// Symbol: ?Memcpy@CMemFile@@MEAAPEAEPEAEPEBE_K@Z
extern "C" unsigned char* MS_ABI impl__Memcpy_CMemFile__MEAAPEAEPEAEPEBE_K_Z(
    CMemFile* /*pThis*/, unsigned char* pDest, unsigned char* pOrigDest, const unsigned char* pSrc, unsigned long long nBytes) {
    // pOrigDest is part of the exported ABI signature.
    (void)pOrigDest;
    if (!pDest || !pSrc) return pDest;
    return static_cast<unsigned char*>(memcpy(pDest, pSrc, static_cast<size_t>(nBytes)));
}

// Symbol: ?GrowFile@CMemFile@@MEAAX_K@Z
extern "C" void MS_ABI impl__GrowFile_CMemFile__MEAAX_K_Z(CMemFile* pThis, unsigned long long dwNewLen) {
    if (!pThis) return;
    pThis->SetLength(dwNewLen);
}

// Symbol: ?LockRange@CMemFile@@UEAAX_K0@Z
extern "C" void MS_ABI impl__LockRange_CMemFile__UEAAX_K0_Z(CMemFile* /*pThis*/, unsigned long long /*dwPos*/, unsigned long long /*dwCount*/) {}

// Symbol: ?UnlockRange@CMemFile@@UEAAX_K0@Z
extern "C" void MS_ABI impl__UnlockRange_CMemFile__UEAAX_K0_Z(CMemFile* /*pThis*/, unsigned long long /*dwPos*/, unsigned long long /*dwCount*/) {}

// CMemFile runtime-class descriptor. DECLARE_DYNAMIC in real MFC: base CFile,
// schema 0xFFFF, no factory. m_nObjectSize is the real mfc140u sizeof(CMemFile) (88).
// Internal DLL symbol (not in the .def) so the export set stays the 14,109 match.
CRuntimeClass CMemFile::classCMemFile = {
    "CMemFile", 88, 0xFFFF, nullptr, nullptr, &CFile::classCFile, nullptr
};

// Symbol: ?GetThisClass@CMemFile@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMemFile__SAPEAUCRuntimeClass__XZ() {
    return &CMemFile::classCMemFile;
}

// Symbol: ?GetRuntimeClass@CMemFile@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CMemFile__UEBAPEAUCRuntimeClass__XZ(const CMemFile* pThis) {
    (void)pThis;
    return &CMemFile::classCMemFile;
}

static void OpenStdioFile(CStdioFile* pThis, const wchar_t* lpszFileName, unsigned int nOpenFlags) {
    if (!pThis) return;
    FILE*& stream = CStdioFileAccessor::Stream(pThis);
    if (stream) {
        fclose(stream);
        stream = nullptr;
    }
    if (!lpszFileName) return;
    wchar_t szMode[4] = {0};
    int idx = 0;
    bool bText = (nOpenFlags & CFile::typeText) != 0;
    unsigned int accessMode = nOpenFlags & 0x0003;
    if (accessMode == CFile::modeRead) szMode[idx++] = L'r';
    else if (accessMode == CFile::modeWrite) szMode[idx++] = L'w';
    else { szMode[idx++] = (nOpenFlags & CFile::modeCreate) ? L'w' : L'r'; szMode[idx++] = L'+'; }
    if (!bText) szMode[idx++] = L'b';
    stream = _wfopen(lpszFileName, szMode);
    pThis->m_hFile = (void*)INVALID_HANDLE_VALUE;
    if (stream) {
        int fd = _fileno(stream);
        if (fd >= 0) {
            pThis->m_hFile = (void*)_get_osfhandle(fd);
        }
    }
    pThis->m_strFileName = lpszFileName;
}

// Symbol: ?CommonBaseInit@CStdioFile@@IEAAXPEAU_iobuf@@PEAVCAtlTransactionManager@ATL@@@Z
extern "C" void MS_ABI impl__CommonBaseInit_CStdioFile__IEAAXPEAU_iobuf__PEAVCAtlTransactionManager_ATL___Z(
    CStdioFile* pThis, FILE* pStream, void* /*pTM*/) {
    if (!pThis) return;
    CStdioFileAccessor::Stream(pThis) = pStream;
    pThis->m_hFile = (void*)INVALID_HANDLE_VALUE;
    if (CStdioFileAccessor::Stream(pThis)) {
        int fd = _fileno(CStdioFileAccessor::Stream(pThis));
        if (fd >= 0) pThis->m_hFile = (void*)_get_osfhandle(fd);
    }
}

// Symbol: ?CommonInit@CStdioFile@@IEAAXPEB_WIPEAVCAtlTransactionManager@ATL@@@Z
extern "C" void MS_ABI impl__CommonInit_CStdioFile__IEAAXPEB_WIPEAVCAtlTransactionManager_ATL___Z(
    CStdioFile* pThis, const wchar_t* lpszFileName, unsigned int nOpenFlags, void* /*pTM*/) {
    OpenStdioFile(pThis, lpszFileName, nOpenFlags);
}

// Symbol: ?Open@CStdioFile@@UEAAHPEB_WIPEAVCFileException@@@Z
extern "C" int MS_ABI impl__Open_CStdioFile__UEAAHPEB_WIPEAVCFileException___Z(
    CStdioFile* pThis, const wchar_t* lpszFileName, unsigned int nOpenFlags, void* /*pException*/) {
    OpenStdioFile(pThis, lpszFileName, nOpenFlags);
    return (pThis && CStdioFileAccessor::Stream(pThis)) ? 1 : 0;
}

// Symbol: ?Open@CStdioFile@@UEAAHPEB_WIPEAVCAtlTransactionManager@ATL@@PEAVCFileException@@@Z
extern "C" int MS_ABI impl__Open_CStdioFile__UEAAHPEB_WIPEAVCAtlTransactionManager_ATL__PEAVCFileException___Z(
    CStdioFile* pThis, const wchar_t* lpszFileName, unsigned int nOpenFlags, void* pTM, void* pException) {
    (void)pTM;
    return impl__Open_CStdioFile__UEAAHPEB_WIPEAVCFileException___Z(pThis, lpszFileName, nOpenFlags, pException);
}

// Symbol: ?Abort@CStdioFile@@UEAAXXZ
extern "C" void MS_ABI impl__Abort_CStdioFile__UEAAXXZ(CStdioFile* pThis) {
    if (pThis) pThis->Close();
}

// Symbol: ?Duplicate@CStdioFile@@UEBAPEAVCFile@@XZ
extern "C" CFile* MS_ABI impl__Duplicate_CStdioFile__UEBAPEAVCFile__XZ(const CStdioFile* pThis) {
    if (!pThis || !CStdioFileAccessor::Stream(pThis)) return nullptr;
    int fd = _fileno(CStdioFileAccessor::Stream(pThis));
    if (fd < 0) return nullptr;
    int dupFd = _dup(fd);
    if (dupFd < 0) return nullptr;
    FILE* dupStream = _fdopen(dupFd, "rb+");
    if (!dupStream) return nullptr;
    CStdioFile* pDup = new CStdioFile();
    CStdioFileAccessor::Stream(pDup) = dupStream;
    pDup->m_hFile = (void*)_get_osfhandle(dupFd);
    pDup->m_strFileName = pThis->m_strFileName;
    return pDup;
}

// Symbol: ?GetLength@CStdioFile@@UEBA_KXZ
extern "C" unsigned long long MS_ABI impl__GetLength_CStdioFile__UEBA_KXZ(const CStdioFile* pThis) {
    if (!pThis || !CStdioFileAccessor::Stream(pThis)) return 0;
    FILE* stream = CStdioFileAccessor::Stream(pThis);
    __int64 cur = _ftelli64(stream);
    if (cur < 0) return 0;
    if (_fseeki64(stream, 0, SEEK_END) != 0) return 0;
    __int64 len = _ftelli64(stream);
    _fseeki64(stream, cur, SEEK_SET);
    return len > 0 ? static_cast<unsigned long long>(len) : 0;
}

// Symbol: ?GetPosition@CStdioFile@@UEBA_KXZ
extern "C" unsigned long long MS_ABI impl__GetPosition_CStdioFile__UEBA_KXZ(const CStdioFile* pThis) {
    if (!pThis || !CStdioFileAccessor::Stream(pThis)) return 0;
    __int64 pos = _ftelli64(CStdioFileAccessor::Stream(pThis));
    return pos > 0 ? static_cast<unsigned long long>(pos) : 0;
}

// Symbol: ?ReadString@CStdioFile@@UEAAHAEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@@Z
extern "C" int MS_ABI impl__ReadString_CStdioFile__UEAAHAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
    CStdioFile* pThis, CString& rString) {
    return pThis ? pThis->ReadString(rString) : 0;
}

// Symbol: ?LockRange@CStdioFile@@UEAAX_K0@Z
extern "C" void MS_ABI impl__LockRange_CStdioFile__UEAAX_K0_Z(CStdioFile* pThis, unsigned long long dwPos, unsigned long long dwCount) {
    if (!pThis || !CStdioFileAccessor::Stream(pThis)) return;
    HANDLE h = (HANDLE)_get_osfhandle(_fileno(CStdioFileAccessor::Stream(pThis)));
    if (h == INVALID_HANDLE_VALUE) return;
    OVERLAPPED ov = {};
    ov.Offset = static_cast<DWORD>(dwPos & 0xFFFFFFFFULL);
    ov.OffsetHigh = static_cast<DWORD>(dwPos >> 32);
    LockFileEx(h, LOCKFILE_EXCLUSIVE_LOCK, 0,
               static_cast<DWORD>(dwCount & 0xFFFFFFFFULL), static_cast<DWORD>(dwCount >> 32), &ov);
}

// Symbol: ?UnlockRange@CStdioFile@@UEAAX_K0@Z
extern "C" void MS_ABI impl__UnlockRange_CStdioFile__UEAAX_K0_Z(CStdioFile* pThis, unsigned long long dwPos, unsigned long long dwCount) {
    if (!pThis || !CStdioFileAccessor::Stream(pThis)) return;
    HANDLE h = (HANDLE)_get_osfhandle(_fileno(CStdioFileAccessor::Stream(pThis)));
    if (h == INVALID_HANDLE_VALUE) return;
    OVERLAPPED ov = {};
    ov.Offset = static_cast<DWORD>(dwPos & 0xFFFFFFFFULL);
    ov.OffsetHigh = static_cast<DWORD>(dwPos >> 32);
    UnlockFileEx(h, 0,
                 static_cast<DWORD>(dwCount & 0xFFFFFFFFULL), static_cast<DWORD>(dwCount >> 32), &ov);
}

// Symbol: ?GetThisClass@CStdioFile@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CStdioFile__SAPEAUCRuntimeClass__XZ() {
    return nullptr;
}

// Symbol: ?GetRuntimeClass@CStdioFile@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CStdioFile__UEBAPEAUCRuntimeClass__XZ(const CStdioFile* pThis) {
    (void)pThis;
    return nullptr;
}

extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CFileException__SAPEAUCRuntimeClass__XZ();
extern "C" void MS_ABI impl__AfxThrowFileException__YAXHJPEB_W_Z(int cause, long lOsError, const wchar_t* lpszFileName);

static int FileExceptionFromErrno(int nErrno) {
    enum FileCause {
        kGenericException = 1,
        kFileNotFound = 2,
        kTooManyOpenFiles = 4,
        kAccessDenied = 5,
        kSharingViolation = 10,
        kLockViolation = 11,
        kDiskFull = 13
    };
    switch (nErrno) {
        case ENOENT: return kFileNotFound;
        case EACCES: return kAccessDenied;
        case EMFILE: return kTooManyOpenFiles;
        case ENOSPC: return kDiskFull;
        default: return kGenericException;
    }
}

static int FileExceptionFromOsError(long lOsError) {
    enum FileCause {
        kGenericException = 1,
        kFileNotFound = 2,
        kAccessDenied = 5,
        kSharingViolation = 10,
        kLockViolation = 11,
        kDiskFull = 13
    };
    switch (lOsError) {
        case ERROR_FILE_NOT_FOUND:
        case ERROR_PATH_NOT_FOUND: return kFileNotFound;
        case ERROR_ACCESS_DENIED: return kAccessDenied;
        case ERROR_SHARING_VIOLATION: return kSharingViolation;
        case ERROR_LOCK_VIOLATION: return kLockViolation;
        case ERROR_DISK_FULL: return kDiskFull;
        default: return kGenericException;
    }
}

// Symbol: ?ErrnoToException@CFileException@@SAHH@Z
extern "C" int MS_ABI impl__ErrnoToException_CFileException__SAHH_Z(int nErrno) {
    return FileExceptionFromErrno(nErrno);
}

// Symbol: ?OsErrorToException@CFileException@@SAHJ@Z
extern "C" int MS_ABI impl__OsErrorToException_CFileException__SAHJ_Z(long lOsError) {
    return FileExceptionFromOsError(lOsError);
}

// Symbol: ?ThrowErrno@CFileException@@SAXHPEB_W@Z
extern "C" void MS_ABI impl__ThrowErrno_CFileException__SAXHPEB_W_Z(int nErrno, const wchar_t* lpszFileName) {
    impl__AfxThrowFileException__YAXHJPEB_W_Z(FileExceptionFromErrno(nErrno), static_cast<long>(nErrno), lpszFileName);
}

// Symbol: ?ThrowOsError@CFileException@@SAXJPEB_W@Z
extern "C" void MS_ABI impl__ThrowOsError_CFileException__SAXJPEB_W_Z(long lOsError, const wchar_t* lpszFileName) {
    impl__AfxThrowFileException__YAXHJPEB_W_Z(FileExceptionFromOsError(lOsError), lOsError, lpszFileName);
}

// Symbol: ?GetRuntimeClass@CFileException@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CFileException__UEBAPEAUCRuntimeClass__XZ(const void* pThis) {
    (void)pThis;
    return impl__GetThisClass_CFileException__SAPEAUCRuntimeClass__XZ();
}
