#pragma once
#include <algorithm>
#include <array>
extern "C" void* MS_ABI impl___0CMapPtrToPtr__QEAA__J_Z(CMapPtrToPtr*,INT_PTR);
extern "C" void MS_ABI impl___1CMapPtrToPtr__UEAA_XZ(CMapPtrToPtr*);
extern "C" void** MS_ABI impl___ACMapPtrToPtr__QEAAAEAPEAXPEAX_Z(CMapPtrToPtr*,void*);
extern "C" void* MS_ABI impl__GetAssocAt_CMapPtrToPtr__IEBAPEAUCAssoc_1_PEAXAEAI1_Z(const CMapPtrToPtr*,void*,UINT&,UINT&);
extern "C" void MS_ABI impl__GetNextAssoc_CMapPtrToPtr__QEBAXAEAPEAU__POSITION__AEAPEAX1_Z(const CMapPtrToPtr*,CMapPtrToPtr::POSITION&,void*&,void*&);
extern "C" BOOL MS_ABI impl__Lookup_CMapPtrToPtr__QEBAHPEAXAEAPEAX_Z(const CMapPtrToPtr*,void*,void*&);
extern "C" BOOL MS_ABI impl__RemoveKey_CMapPtrToPtr__QEAAHPEAX_Z(CMapPtrToPtr*,void*);
extern "C" void MS_ABI impl__RemoveAll_CMapPtrToPtr__QEAAXXZ(CMapPtrToPtr*);
extern "C" void MS_ABI impl__InitHashTable_CMapPtrToPtr__QEAAXIH_Z(CMapPtrToPtr*,UINT,BOOL);
extern "C" void* MS_ABI impl__NewAssoc_CMapPtrToPtr__IEAAPEAUCAssoc_1_XZ(CMapPtrToPtr*);
extern "C" void MS_ABI impl__FreeAssoc_CMapPtrToPtr__IEAAXPEAUCAssoc_1__Z(CMapPtrToPtr*,void*);
extern "C" void* MS_ABI impl__GetValueAt_CMapPtrToPtr__QEBAPEAXPEAX_Z(const CMapPtrToPtr*,void*);
namespace pointer_map_test {
struct API {
    decltype(&impl___0CMapPtrToPtr__QEAA__J_Z) construct;
    decltype(&impl___1CMapPtrToPtr__UEAA_XZ) destroy;
    decltype(&impl___ACMapPtrToPtr__QEAAAEAPEAXPEAX_Z) value;
    decltype(&impl__GetAssocAt_CMapPtrToPtr__IEBAPEAUCAssoc_1_PEAXAEAI1_Z) assoc;
    decltype(&impl__GetNextAssoc_CMapPtrToPtr__QEBAXAEAPEAU__POSITION__AEAPEAX1_Z) next;
    decltype(&impl__Lookup_CMapPtrToPtr__QEBAHPEAXAEAPEAX_Z) lookup;
    decltype(&impl__RemoveKey_CMapPtrToPtr__QEAAHPEAX_Z) remove;
    decltype(&impl__RemoveAll_CMapPtrToPtr__QEAAXXZ) clear;
    decltype(&impl__InitHashTable_CMapPtrToPtr__QEAAXIH_Z) init;
    decltype(&impl__NewAssoc_CMapPtrToPtr__IEAAPEAUCAssoc_1_XZ) allocate;
    decltype(&impl__FreeAssoc_CMapPtrToPtr__IEAAXPEAUCAssoc_1__Z) release;
    decltype(&impl__GetValueAt_CMapPtrToPtr__QEBAPEAXPEAX_Z) get;
};
static void MS_ABI destroyNative(CMapPtrToPtr* self) {
    using Destructor=void* (MS_ABI*)(CMapPtrToPtr*,UINT);
    auto* slots=*reinterpret_cast<void***>(self);
    reinterpret_cast<Destructor>(slots[1])(self,0);
}
static void* pointer(UINT64 value) {return reinterpret_cast<void*>(static_cast<UINT_PTR>(value));}
std::vector<std::array<UINT64,4>> exercise(const API& api) {
    alignas(8) unsigned char memory[64];memset(memory,0xa5,sizeof(memory));
    auto* map=reinterpret_cast<CMapPtrToPtr*>(memory);
    api.construct(map,-8);
    check(map->m_nBlockSize==10 && map->m_nHashTableSize==17 && !map->m_pHashTable && !map->m_pFreeList && !map->m_pBlocks && !map->m_nCount &&
          *reinterpret_cast<UINT64*>(memory+56)==0xa5a5a5a5a5a5a5a5ULL,
          "pointer map constructor uses the native 56-byte layout and normalizes nonpositive block sizes");
    check(map->GetRuntimeClass()->m_nObjectSize==56 && strcmp(map->GetRuntimeClass()->m_lpszClassName,"CMapPtrToPtr")==0,
          "pointer map constructor installs its own native runtime-class vtable");
    api.destroy(map);api.construct(map,0);
    check(map->m_nBlockSize==10,"zero pointer-map block size uses native default");
    api.destroy(map);api.construct(map,2);
    api.init(map,0,FALSE);
    check(map->m_nHashTableSize==17 && !map->m_pHashTable,"pointer map zero hash-table size selects native default and supports deferred allocation");
    api.init(map,5,TRUE);
    bool emptyBuckets=true;for(UINT i=0;i<5;++i)emptyBuckets &= map->m_pHashTable[i]==nullptr;
    check(emptyBuckets && map->m_nHashTableSize==5,"pointer map allocates a real zeroed bucket array");
    const UINT64 keys[]={0,1,16,127773,0x7fffffff,0x80000000,0xffffffff,0x100000010,0xffffffffffffffffULL};
    std::vector<std::array<UINT64,4>> result;
    bool live=true,lookups=true;
    for(size_t i=0;i<std::size(keys);++i) {
        void* key=pointer(keys[i]);void** ref=api.value(map,key);
        *ref=pointer(i+100);
        UINT bucket=99,hash=99;auto* assoc=static_cast<CMapPtrToPtr::CAssoc*>(api.assoc(map,key,bucket,hash));
        live &= assoc && ref==&assoc->value && assoc->key==key && bucket<5;
        bool foundNode=false;for(auto* a=map->m_pHashTable[bucket];a;a=a->pNext)foundNode |= a==assoc;
        live &= foundNode;
        void* value=pointer(999);lookups &= api.lookup(map,key,value) && value==pointer(i+100) && api.get(map,key)==value;
        result.push_back({keys[i],hash,bucket,reinterpret_cast<UINT_PTR>(value)});
    }
    check(live && map->m_nCount==std::size(keys),"pointer map export references and associations point at live native bucket nodes");
    check(lookups,"pointer map compares all pointer bits while storing colliding signed-low-word keys");
    UINT bucket,hash;auto* first=static_cast<CMapPtrToPtr::CAssoc*>(api.assoc(map,pointer(1),bucket,hash));
    first->value=pointer(700);void* value=nullptr;
    check(api.lookup(map,pointer(1),value) && value==pointer(700),"direct writes to native association values are visible to exported lookup");
    value=pointer(901);auto oldCount=map->m_nCount;
    check(!api.lookup(map,pointer(555),value) && value==pointer(901) && !api.assoc(map,pointer(555),bucket,hash) && map->m_nCount==oldCount,
          "missing pointer-map lookups preserve the output and never allocate association snapshots");
    CMapPtrToPtr::POSITION pos=nullptr;void* key=pointer(902);value=pointer(903);api.next(map,pos,key,value);
    check(!pos && key==pointer(902) && value==pointer(903),"null pointer-map iteration position preserves outputs");
    pos=map->GetStartPosition();size_t visited=0;bool physicalPositions=true;
    while(pos) {
        if(pos!=reinterpret_cast<CMapPtrToPtr::POSITION>(INT_PTR(-1))) {
            bool found=false;for(UINT i=0;i<5;++i)for(auto* a=map->m_pHashTable[i];a;a=a->pNext)found |= reinterpret_cast<void*>(pos)==a;
            physicalPositions &= found;
        }
        api.next(map,pos,key,value);result.push_back({0xeeee,reinterpret_cast<UINT_PTR>(key),reinterpret_cast<UINT_PTR>(value),visited++});
    }
    check(visited==std::size(keys) && physicalPositions,"pointer-map iteration uses node pointers and native bucket/chain order");
    auto* removed=static_cast<CMapPtrToPtr::CAssoc*>(api.assoc(map,pointer(16),bucket,hash));
    check(api.remove(map,pointer(16)) && map->m_pFreeList==removed && map->m_nCount==oldCount-1,
          "pointer-map removal places the detached association on its native free list");
    void** reused=api.value(map,pointer(500));
    check(reused==&removed->value && !*reused && map->m_nCount==oldCount,"pointer map reuses removed nodes and zeroes their value");
    auto* detached=static_cast<CMapPtrToPtr::CAssoc*>(api.allocate(map));
    check(!detached->key && !detached->value && map->m_nCount==oldCount+1,"NewAssoc allocates a zeroed key/value in native block storage");
    api.release(map,detached);
    check(map->m_pFreeList==detached && map->m_nCount==oldCount,"FreeAssoc returns native nodes to the pool without deleting live entries");
    api.clear(map);
    check(!map->m_nCount && !map->m_pHashTable && !map->m_pFreeList && !map->m_pBlocks && map->m_nHashTableSize==5 && map->m_nBlockSize==2,
          "pointer-map cleanup frees buckets and blocks while preserving configuration");
    api.init(map,7,FALSE);*api.value(map,nullptr)=nullptr;
    value=pointer(888);
    check(api.lookup(map,nullptr,value) && !value && api.remove(map,nullptr) && !map->m_nCount && !map->m_pHashTable && !map->m_pBlocks && !map->m_pFreeList,
          "null pointer keys and values work and removing the last association releases all storage");
    auto* standalone=static_cast<CMapPtrToPtr::CAssoc*>(api.allocate(map));
    api.release(map,standalone);
    check(!map->m_nCount && !map->m_pBlocks && !map->m_pFreeList,"freeing a standalone last association releases its native block chain");
    api.destroy(map);
    return result;
}
}
static void pointerMap() {
    using namespace pointer_map_test;
    API production{impl___0CMapPtrToPtr__QEAA__J_Z,impl___1CMapPtrToPtr__UEAA_XZ,impl___ACMapPtrToPtr__QEAAAEAPEAXPEAX_Z,
        impl__GetAssocAt_CMapPtrToPtr__IEBAPEAUCAssoc_1_PEAXAEAI1_Z,impl__GetNextAssoc_CMapPtrToPtr__QEBAXAEAPEAU__POSITION__AEAPEAX1_Z,
        impl__Lookup_CMapPtrToPtr__QEBAHPEAXAEAPEAX_Z,impl__RemoveKey_CMapPtrToPtr__QEAAHPEAX_Z,impl__RemoveAll_CMapPtrToPtr__QEAAXXZ,
        impl__InitHashTable_CMapPtrToPtr__QEAAXIH_Z,impl__NewAssoc_CMapPtrToPtr__IEAAPEAUCAssoc_1_XZ,
        impl__FreeAssoc_CMapPtrToPtr__IEAAXPEAUCAssoc_1__Z,impl__GetValueAt_CMapPtrToPtr__QEBAPEAXPEAX_Z};
    bool threw=false;try {production.get(nullptr,nullptr);}catch(int value) {threw=value==19;}
    check(threw,"GetValueAt rejects a null map through the invalid-argument helper");
    auto expected=exercise(production);
    wchar_t nativePath[1024];
    if(::GetEnvironmentVariableW(L"OPENMFC_NATIVE_COLLECTION_DLL",nativePath,1024)) {
        HMODULE module=::LoadLibraryW(nativePath);check(module!=nullptr,"load native MFC for pointer-map differential checks");
        if(module) {
            // MSVC 14.51.36231 release Unicode DLL, verified by constructor RTTI.
            auto* base=reinterpret_cast<char*>(module);API native=production;
            native.construct=reinterpret_cast<decltype(native.construct)>(base+0x232ba0);
            native.destroy=&destroyNative; // native deleting destructor, with deletion flag zero
            native.value=reinterpret_cast<decltype(native.value)>(base+0x232e40);
            native.assoc=reinterpret_cast<decltype(native.assoc)>(base+0x232d10);
            native.next=reinterpret_cast<decltype(native.next)>(base+0x232f70);
            native.lookup=reinterpret_cast<decltype(native.lookup)>(base+0x232e10);
            native.remove=reinterpret_cast<decltype(native.remove)>(base+0x232ec0);
            native.clear=reinterpret_cast<decltype(native.clear)>(base+0x1ba40);
            native.init=reinterpret_cast<decltype(native.init)>(base+0x232be0);
            native.allocate=reinterpret_cast<decltype(native.allocate)>(base+0x232c80);
            native.release=reinterpret_cast<decltype(native.release)>(base+0x232cf0);
            native.get=reinterpret_cast<decltype(native.get)>(base+0x232d90);
            check(exercise(native)==expected,"pointer-map hashes, buckets, values and iteration match native MFC");
            ::FreeLibrary(module);
        }
    }
}
