// CMirrorFile — OpenMFC implementation.
// Sources: file_cmirrorfile.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/CMirrorFileSupport.h"

// Symbol: ?Open@CMirrorFile@@UEAAHPEB_WIPEAVCFileException@@@Z
extern "C" int MS_ABI impl__Open_CMirrorFile__UEAAHPEB_WIPEAVCFileException___Z(
    void* pThis, const wchar_t* lpszFileName, unsigned int nOpenFlags, void* /*pException*/) {
    if (!pThis) {
        return 0;
    }
    CMirrorFile* self = static_cast<CMirrorFile*>(pThis);
    CMirrorFileAccessor::InvokeCommonInit(self, lpszFileName, nOpenFlags, nullptr);
    return self->m_hFile != (void*)INVALID_HANDLE_VALUE ? 1 : 0;
}
// Symbol: ?Close@CMirrorFile@@UEAAXXZ
extern "C" void MS_ABI impl__Close_CMirrorFile__UEAAXXZ(void* pThis) {
    if (!pThis) {
        return;
    }
    CMirrorFile* self = static_cast<CMirrorFile*>(pThis);
    if (self->m_hFile != (void*)INVALID_HANDLE_VALUE) {
        CloseHandle((HANDLE)self->m_hFile);
        self->m_hFile = (void*)INVALID_HANDLE_VALUE;
    }
}
// Symbol: ?Abort@CMirrorFile@@UEAAXXZ
extern "C" void MS_ABI impl__Abort_CMirrorFile__UEAAXXZ(void* pThis) {
    if (!pThis) {
        return;
    }
    CMirrorFile* self = static_cast<CMirrorFile*>(pThis);
    if (self->m_hFile != (void*)INVALID_HANDLE_VALUE) {
        CloseHandle((HANDLE)self->m_hFile);
        self->m_hFile = (void*)INVALID_HANDLE_VALUE;
    }
}
