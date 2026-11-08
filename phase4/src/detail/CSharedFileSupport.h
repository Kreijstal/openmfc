#pragma once
#define OPENMFC_APPCORE_IMPL
#include "openmfc/afx.h"

#ifndef MS_ABI
#define MS_ABI
#endif

struct CSharedFileAccessor;
class CSharedFile : public CMemFile {
    friend struct CSharedFileAccessor;
public:
    CSharedFile(UINT flags = GMEM_DDESHARE | GMEM_MOVEABLE, UINT grow = 4096);
    ~CSharedFile() override;
    static CRuntimeClass* GetThisClass();
    CRuntimeClass* GetRuntimeClass() const override { return GetThisClass(); }
    HGLOBAL Detach();
    void SetHandle(HGLOBAL, BOOL allowGrow = TRUE);
protected:
    BYTE* Alloc(SIZE_T) override;
    BYTE* Realloc(BYTE*, SIZE_T) override;
    void Free(BYTE*) override;
public:
    UINT m_nAllocFlags;
    HGLOBAL m_hGlobalMemory;
    BOOL m_bAllowGrow;
};
static_assert(sizeof(CSharedFile) == 112 && offsetof(CSharedFile,m_nAllocFlags) == 88 &&
              offsetof(CSharedFile,m_hGlobalMemory) == 96 && offsetof(CSharedFile,m_bAllowGrow) == 104,
              "native shared file model");

namespace openmfc { namespace detail { namespace csharedfile {
extern CRuntimeClass g_classCSharedFile;
} } }
using namespace openmfc::detail::csharedfile;
struct CSharedFileAccessor {
    static BYTE* CallAlloc(CSharedFile* p, SIZE_T n) { return p->CSharedFile::Alloc(n); }
    static BYTE* CallRealloc(CSharedFile* p, BYTE* b, SIZE_T n) { return p->CSharedFile::Realloc(b,n); }
    static void CallFree(CSharedFile* p, BYTE* b) { p->CSharedFile::Free(b); }
};
