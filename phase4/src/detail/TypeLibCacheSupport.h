#pragma once
#include "detail/CCmdTargetVirtualsSupport.h"
#include <oaidl.h>
class CTypeLibCache {
public:
    const GUID* m_pTypeLibID=nullptr;
    LCID m_lcid=static_cast<LCID>(-1);
    ITypeLib* m_ptlib=nullptr;
    GUID m_guidInfo{};
    ITypeInfo* m_ptinfo=nullptr;
    LONG m_cRef=0;
};
static_assert(sizeof(CTypeLibCache)==0x38,"native type-library cache size");
static_assert(offsetof(CTypeLibCache,m_lcid)==8,"native cache locale");
static_assert(offsetof(CTypeLibCache,m_ptlib)==0x10,"native cached library");
static_assert(offsetof(CTypeLibCache,m_guidInfo)==0x18,"native cached GUID");
static_assert(offsetof(CTypeLibCache,m_ptinfo)==0x28,"native cached type information");
static_assert(offsetof(CTypeLibCache,m_cRef)==0x30,"native cache lock count");
extern "C" void MS_ABI impl__Cache_CTypeLibCache__QEAAXKPEAUITypeLib___Z(void*,unsigned long,ITypeLib*);
extern "C" void MS_ABI impl__CacheTypeInfo_CTypeLibCache__QEAAXKAEBU_GUID__PEAUITypeInfo___Z(void*,unsigned long,const GUID*,ITypeInfo*);
extern "C" int MS_ABI impl__Lookup_CTypeLibCache__QEAAHKPEAPEAUITypeLib___Z(void*,unsigned long,ITypeLib**);
extern "C" int MS_ABI impl__LookupTypeInfo_CTypeLibCache__QEAAHKAEBU_GUID__PEAPEAUITypeInfo___Z(void*,unsigned long,const GUID*,ITypeInfo**);
extern "C" void MS_ABI impl__Lock_CTypeLibCache__QEAAXXZ(void*);
extern "C" void MS_ABI impl__Unlock_CTypeLibCache__QEAAXXZ(void*);
extern "C" long MS_ABI impl__GetTypeInfoOfGuid_CCmdTarget__QEAAJKAEBU_GUID__PEAPEAUITypeInfo___Z(CCmdTarget*,unsigned long,const void*,void**);
void OpenMfcClearTypeLibCache(CTypeLibCache*);
