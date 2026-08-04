#pragma once
// Shared internals of the former file_csharedfile.cpp translation unit.
// Definitions live in detail/CSharedFileSupport.cpp, so file-scope state keeps a
// single instance across the classes this unit was split into.
namespace openmfc { namespace detail { namespace csharedfile {} } }
using namespace openmfc::detail::csharedfile;
#define OPENMFC_APPCORE_IMPL
#include "openmfc/afx.h"
#include <windows.h>
#include <cstring>
#include <cwchar>
#include <new>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

// =============================================================================
// CSharedFile
//
// CSharedFile : public CMemFile. The repo's CMemFile (afx.h) has protected
// data members; CSharedFile derives from it and adds an HGLOBAL handle plus the
// allocation flags. Members are kept public for extern "C" thunk access, mirror-
// ing the convention noted in afx.h (CFile members are public for the same
// reason).
//
// Repo layout target (NOT retail mfc140u.dll):
//   CFile     sizeof = 24  : [vptr@0][void* m_hFile@8][CString m_strFileName@16]
//   CMemFile  sizeof = 56  : + BYTE* m_lpBuffer@24, UINT m_nBufferSize@32,
//                              UINT m_nFileSize@36, UINT m_nGrowBytes@40,
//                              UINT m_nPosition@44, bool m_bAutoDelete@48 (pad)
//   CSharedFile sizeof = 72: + HGLOBAL m_hGlobalMemory@56, UINT m_nAllocFlags@64
//
// All allocation goes through the GlobalAlloc family so that Detach() / SetHandle()
// can hand back a usable HGLOBAL, matching real CSharedFile semantics.
// =============================================================================
class CSharedFile : public CMemFile {
public:
    CSharedFile(UINT nAllocFlags = GMEM_DDESHARE | GMEM_MOVEABLE, UINT nGrowBytes = 4096);
    virtual ~CSharedFile();

    HGLOBAL Detach();
    void SetHandle(HGLOBAL hGlobalMemory, BOOL bAllowGrow = TRUE);

protected:
    // NOTE: the repo's CMemFile header does not declare Alloc/Realloc/Free as
    // member virtuals (only the extern "C" thunks exist in filecore.cpp), so
    // these cannot use `override`. They are declared here as ordinary member
    // functions used by this class's own buffer management; the exported ABI
    // entry points are the extern "C" impl__ thunks below.
    BYTE* Alloc(SIZE_T nBytes);
    BYTE* Realloc(BYTE* lpMem, SIZE_T nBytes);
    void Free(BYTE* lpMem);

public:
    HGLOBAL m_hGlobalMemory;
    UINT m_nAllocFlags;
};

// Confirm the repo-targeted layout at compile time.

namespace openmfc { namespace detail { namespace csharedfile {
extern CRuntimeClass g_classCSharedFile;
} } }

// Accessor to reach CSharedFile's protected virtual-override entry points from
// the extern "C" thunks (mirrors CMemFileAccessor in filecore.cpp).
struct CSharedFileAccessor : CSharedFile {
    static BYTE* CallAlloc(CSharedFile* p, SIZE_T n) {
        return static_cast<CSharedFileAccessor*>(p)->Alloc(n);
    }
    static BYTE* CallRealloc(CSharedFile* p, BYTE* m, SIZE_T n) {
        return static_cast<CSharedFileAccessor*>(p)->Realloc(m, n);
    }
    static void CallFree(CSharedFile* p, BYTE* m) {
        static_cast<CSharedFileAccessor*>(p)->Free(m);
    }
};

// -----------------------------------------------------------------------------
// Helper: reach CMemFile's protected buffer-bookkeeping members from within the
// CSharedFile member functions (they are inherited as protected, so accessible
// here directly). The GlobalAlloc family stores an HGLOBAL handle; the locked
// linear pointer is what CMemFile keeps in m_lpBuffer.
// -----------------------------------------------------------------------------



// Alloc nBytes via GlobalAlloc, remember the handle, return a locked pointer.

// Realloc the global block. lpMem is the currently-locked pointer; unlock,
// realloc the underlying handle, relock, and return the (possibly moved) pointer.

// Free the global block that lpMem points into.

// Detach: hand the HGLOBAL to the caller and reset all buffer bookkeeping.
// The handle is unlocked (matching the number of GlobalLock calls) so the
// caller owns a clean, movable HGLOBAL.

// SetHandle: install an externally-supplied HGLOBAL as the backing store.

// =============================================================================
// Exported MS-ABI thunks (one per research-listed symbol).
// Each takes an explicit pThis first arg (except the static GetThisClass).
// =============================================================================









