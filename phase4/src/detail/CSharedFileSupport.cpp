#include "CSharedFileSupport.h"

CSharedFile::CSharedFile(UINT flags, UINT grow)
    : CMemFile(grow), m_nAllocFlags(flags), m_hGlobalMemory(nullptr), m_bAllowGrow(TRUE) {}
CSharedFile::~CSharedFile() {
    // Keep this class's Free override installed while closing its global block.
    if (m_lpBuffer) CMemFile::Close();
}
BYTE* CSharedFile::Alloc(SIZE_T bytes) {
    m_hGlobalMemory = GlobalAlloc(m_nAllocFlags, bytes);
    return m_hGlobalMemory ? static_cast<BYTE*>(GlobalLock(m_hGlobalMemory)) : nullptr;
}
BYTE* CSharedFile::Realloc(BYTE*, SIZE_T bytes) {
    if (!m_bAllowGrow) return nullptr;
    GlobalUnlock(m_hGlobalMemory);
    HGLOBAL result = GlobalReAlloc(m_hGlobalMemory, bytes, m_nAllocFlags);
    if (!result) return nullptr;
    m_hGlobalMemory = result;
    return static_cast<BYTE*>(GlobalLock(result));
}
void CSharedFile::Free(BYTE*) {
    GlobalUnlock(m_hGlobalMemory);
    GlobalFree(m_hGlobalMemory);
}
HGLOBAL CSharedFile::Detach() {
    HGLOBAL result = m_hGlobalMemory;
    GlobalUnlock(result);
    m_hGlobalMemory = nullptr;
    m_lpBuffer = nullptr;
    m_nBufferSize = 0;
    return result;
}
void CSharedFile::SetHandle(HGLOBAL handle, BOOL allowGrow) {
    if (!handle) AfxThrowInvalidArgException();
    m_hGlobalMemory = handle;
    m_lpBuffer = static_cast<BYTE*>(GlobalLock(handle));
    m_nFileSize = GlobalSize(m_hGlobalMemory);
    m_nBufferSize = static_cast<SIZE_T>(m_nFileSize);
    m_bAllowGrow = allowGrow;
}
CRuntimeClass* CSharedFile::GetThisClass() { return &g_classCSharedFile; }
namespace openmfc { namespace detail { namespace csharedfile {
static CRuntimeClass* AFXAPI SharedFileBase() { return CMemFile::GetThisClass(); }
CRuntimeClass g_classCSharedFile = {"CSharedFile", sizeof(CSharedFile), 0xffff, nullptr, SharedFileBase, nullptr, nullptr};
} } }
