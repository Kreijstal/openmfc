#include "CSharedFileSupport.h"

static_assert(sizeof(CSharedFile) == 72, "CSharedFile must be 72 bytes (repo CMemFile family layout)");
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
BYTE* CSharedFile::Alloc(SIZE_T nBytes)
{
    HGLOBAL hNew = ::GlobalAlloc(m_nAllocFlags, nBytes);
    if (!hNew) {
        return nullptr;
    }
    BYTE* pMsg = reinterpret_cast<BYTE*>(::GlobalLock(hNew));
    if (!pMsg) {
        // Lock failed: don't strand the handle (m_lpBuffer stays null, so the
        // dtor would never Free it) — release it now.
        ::GlobalFree(hNew);
        return nullptr;
    }
    m_hGlobalMemory = hNew;
    return pMsg;
}
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
namespace openmfc { namespace detail { namespace csharedfile {
static CRuntimeClass* AFXAPI _openmfc_gb_g_classCSharedFile() { return &CMemFile::classCMemFile; }
CRuntimeClass g_classCSharedFile = {
    "CSharedFile",
    sizeof(CSharedFile),
    0xFFFF,
    nullptr,
    &_openmfc_gb_g_classCSharedFile,
    nullptr,
    nullptr
};
} } }  // namespace openmfc::detail::csharedfile
