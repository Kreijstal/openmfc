#pragma once
#include "openmfc/afxole.h"
struct AFX_DATACACHE_ENTRY {
    FORMATETC m_formatEtc;
    STGMEDIUM m_stgMedium;
    DATADIR m_nDataDir;
};
static_assert(sizeof(AFX_DATACACHE_ENTRY)==64 && offsetof(AFX_DATACACHE_ENTRY,m_stgMedium)==32 &&
              offsetof(AFX_DATACACHE_ENTRY,m_nDataDir)==56,"native cache entry layout");
namespace openmfc { namespace detail {
struct OleDataSourceAccess {
    static auto& Cache(COleDataSource* p) { return p->m_pDataCache; }
    static auto& Size(COleDataSource* p) { return p->m_nSize; }
    static auto& Capacity(COleDataSource* p) { return p->m_nMaxSize; }
    static auto& Grow(COleDataSource* p) { return p->m_nGrowBy; }
    static auto Lookup(const COleDataSource* p,FORMATETC* f,DATADIR d) { return p->Lookup(f,d); }
    static auto Entry(COleDataSource* p,FORMATETC* f,DATADIR d) { return p->GetCacheEntry(f,d); }
    static_assert(offsetof(COleDataSource,m_pDataCache)==64 && offsetof(COleDataSource,m_nMaxSize)==72 &&
                  offsetof(COleDataSource,m_nSize)==76 && offsetof(COleDataSource,m_nGrowBy)==80,
                  "native cache member offsets");
};
BOOL CopyDataMedium(CLIPFORMAT,STGMEDIUM*,STGMEDIUM*);
} }
