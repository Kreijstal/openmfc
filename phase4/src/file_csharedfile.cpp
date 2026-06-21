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
static_assert(sizeof(CSharedFile) == 72, "CSharedFile must be 72 bytes (repo CMemFile family layout)");

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

CSharedFile::CSharedFile(UINT nAllocFlags, UINT nGrowBytes)
    : CMemFile(nGrowBytes), m_hGlobalMemory(nullptr), m_nAllocFlags(nAllocFlags)
{
}

CSharedFile::~CSharedFile()
{
    // If the file still owns a global block, release it through Free() so the
    // GlobalAlloc/GlobalFree pairing stays consistent. CMemFile's destructor
    // would otherwise call free() on a GlobalLock pointer, which is wrong, so
    // detach the CMemFile buffer bookkeeping first.
    if (m_bAutoDelete && m_lpBuffer) {
        Free(m_lpBuffer);
    }
    m_lpBuffer = nullptr;
    m_nBufferSize = 0;
    m_nFileSize = 0;
    m_nPosition = 0;
    m_bAutoDelete = false;
    m_hGlobalMemory = nullptr;
}

// Alloc nBytes via GlobalAlloc, remember the handle, return a locked pointer.
BYTE* CSharedFile::Alloc(SIZE_T nBytes)
{
    HGLOBAL hNew = ::GlobalAlloc(m_nAllocFlags, nBytes);
    if (!hNew) {
        return nullptr;
    }
    m_hGlobalMemory = hNew;
    return reinterpret_cast<BYTE*>(::GlobalLock(hNew));
}

// Realloc the global block. lpMem is the currently-locked pointer; unlock,
// realloc the underlying handle, relock, and return the (possibly moved) pointer.
BYTE* CSharedFile::Realloc(BYTE* lpMem, SIZE_T nBytes)
{
    if (!m_hGlobalMemory) {
        // Nothing allocated yet -> fresh allocation.
        return Alloc(nBytes);
    }

    if (nBytes == 0) {
        Free(lpMem);
        return nullptr;
    }

    // Unlock the old locked pointer before reallocating the handle.
    if (lpMem) {
        ::GlobalUnlock(m_hGlobalMemory);
    }

    HGLOBAL hNew = ::GlobalReAlloc(m_hGlobalMemory, nBytes, m_nAllocFlags);
    if (!hNew) {
        // Realloc failed; re-lock the original so the caller's pointer stays valid.
        return reinterpret_cast<BYTE*>(::GlobalLock(m_hGlobalMemory));
    }

    m_hGlobalMemory = hNew;
    return reinterpret_cast<BYTE*>(::GlobalLock(hNew));
}

// Free the global block that lpMem points into.
void CSharedFile::Free(BYTE* lpMem)
{
    if (!m_hGlobalMemory) {
        return;
    }
    if (lpMem) {
        ::GlobalUnlock(m_hGlobalMemory);
    }
    ::GlobalFree(m_hGlobalMemory);
    m_hGlobalMemory = nullptr;
}

// Detach: hand the HGLOBAL to the caller and reset all buffer bookkeeping.
// The handle is unlocked (matching the number of GlobalLock calls) so the
// caller owns a clean, movable HGLOBAL.
HGLOBAL CSharedFile::Detach()
{
    HGLOBAL hMem = m_hGlobalMemory;
    if (hMem && m_lpBuffer) {
        ::GlobalUnlock(hMem);
    }
    m_hGlobalMemory = nullptr;
    m_lpBuffer = nullptr;
    m_nBufferSize = 0;
    m_nFileSize = 0;
    m_nPosition = 0;
    m_bAutoDelete = false;
    return hMem;
}

// SetHandle: install an externally-supplied HGLOBAL as the backing store.
void CSharedFile::SetHandle(HGLOBAL hGlobalMemory, BOOL bAllowGrow)
{
    // Release any block we currently own.
    if (m_bAutoDelete && m_lpBuffer) {
        Free(m_lpBuffer);
    }

    m_hGlobalMemory = hGlobalMemory;
    SIZE_T nSize = hGlobalMemory ? ::GlobalSize(hGlobalMemory) : 0;
    m_lpBuffer = hGlobalMemory ? reinterpret_cast<BYTE*>(::GlobalLock(hGlobalMemory)) : nullptr;
    m_nBufferSize = static_cast<UINT>(nSize);
    m_nFileSize = static_cast<UINT>(nSize);
    m_nPosition = 0;
    m_bAutoDelete = bAllowGrow ? true : false;
}

// =============================================================================
// Exported MS-ABI thunks (one per research-listed symbol).
// Each takes an explicit pThis first arg (except the static GetThisClass).
// =============================================================================

// Symbol: ??0CSharedFile@@QEAA@II@Z
extern "C" void MS_ABI impl___0CSharedFile__QEAA_II_Z(
    void* pThis, unsigned int nAllocFlags, unsigned int nGrowBytes) {
    if (!pThis) return;
    new (pThis) CSharedFile(nAllocFlags, nGrowBytes);
}

// Symbol: ??1CSharedFile@@UEAA@XZ
extern "C" void MS_ABI impl___1CSharedFile__UEAA_XZ(void* pThis) {
    if (!pThis) return;
    static_cast<CSharedFile*>(pThis)->~CSharedFile();
}

// Symbol: ?Alloc@CSharedFile@@MEAAPEAE_K@Z
extern "C" unsigned char* MS_ABI impl__Alloc_CSharedFile__MEAAPEAE_K_Z(
    void* pThis, unsigned long long nBytes) {
    if (!pThis) return nullptr;
    return CSharedFileAccessor::CallAlloc(static_cast<CSharedFile*>(pThis), static_cast<SIZE_T>(nBytes));
}

// Symbol: ?Detach@CSharedFile@@QEAAPEAXXZ
extern "C" void* MS_ABI impl__Detach_CSharedFile__QEAAPEAXXZ(void* pThis) {
    if (!pThis) return nullptr;
    return static_cast<void*>(static_cast<CSharedFile*>(pThis)->Detach());
}

// Symbol: ?Free@CSharedFile@@MEAAXPEAE@Z
extern "C" void MS_ABI impl__Free_CSharedFile__MEAAXPEAE_Z(
    void* pThis, unsigned char* lpMem) {
    if (!pThis) return;
    CSharedFileAccessor::CallFree(static_cast<CSharedFile*>(pThis), lpMem);
}

// Symbol: ?GetRuntimeClass@CSharedFile@@UEBAPEAUCRuntimeClass@@XZ
// The repo's CFile family is intentionally NOT CObject-derived; CFile and
// CMemFile both return nullptr here (filecore.cpp:2499,2612). Follow suit.
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CSharedFile__UEBAPEAUCRuntimeClass__XZ(
    const void* pThis) {
    (void)pThis;
    return nullptr;
}

// Symbol: ?GetThisClass@CSharedFile@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CSharedFile__SAPEAUCRuntimeClass__XZ() {
    return nullptr;
}

// Symbol: ?Realloc@CSharedFile@@MEAAPEAEPEAE_K@Z
extern "C" unsigned char* MS_ABI impl__Realloc_CSharedFile__MEAAPEAEPEAE_K_Z(
    void* pThis, unsigned char* lpMem, unsigned long long nBytes) {
    if (!pThis) return nullptr;
    return CSharedFileAccessor::CallRealloc(static_cast<CSharedFile*>(pThis), lpMem, static_cast<SIZE_T>(nBytes));
}

// Symbol: ?SetHandle@CSharedFile@@QEAAXPEAXH@Z
extern "C" void MS_ABI impl__SetHandle_CSharedFile__QEAAXPEAXH_Z(
    void* pThis, void* hGlobalMemory, int bAllowGrow) {
    if (!pThis) return;
    static_cast<CSharedFile*>(pThis)->SetHandle(
        static_cast<HGLOBAL>(hGlobalMemory), bAllowGrow);
}
