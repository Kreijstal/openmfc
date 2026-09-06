#include "CMirrorFileSupport.h"

static_assert(sizeof(CMirrorFile) == 32, "CMirrorFile must be 32 bytes (0x20)");
static_assert(true, "");
int CMirrorFile::Open(const wchar_t* lpszFileName, UINT nOpenFlags, void* /*pException*/) {
    CMirrorFileAccessor::InvokeCommonInit(this, lpszFileName, nOpenFlags, nullptr);
    return m_hFile != (void*)INVALID_HANDLE_VALUE ? 1 : 0;
}
void CMirrorFile::Close() {
    if (m_hFile != (void*)INVALID_HANDLE_VALUE) {
        CloseHandle((HANDLE)m_hFile);
        m_hFile = (void*)INVALID_HANDLE_VALUE;
    }
}
void CMirrorFile::Abort() {
    if (m_hFile != (void*)INVALID_HANDLE_VALUE) {
        CloseHandle((HANDLE)m_hFile);
        m_hFile = (void*)INVALID_HANDLE_VALUE;
    }
}
namespace openmfc { namespace detail { namespace cmirrorfile {
CRuntimeClass g_runtimeClass_CMirrorFile = {
    "CMirrorFile",                 // m_lpszClassName
    (int)sizeof(CMirrorFile),      // m_nObjectSize == 32
    0xFFFF,                        // m_wSchema (no serialization)
    nullptr,                       // m_pfnCreateObject
    nullptr,                       // m_pfnGetBaseClass
    nullptr,                       // m_pNextClass
    nullptr                        // m_pNextClass
};
} } }  // namespace openmfc::detail::cmirrorfile
