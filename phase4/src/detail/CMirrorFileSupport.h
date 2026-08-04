#pragma once
// Shared internals of the former file_cmirrorfile.cpp translation unit.
// Definitions live in detail/CMirrorFileSupport.cpp, so file-scope state keeps a
// single instance across the classes this unit was split into.
namespace openmfc { namespace detail { namespace cmirrorfile {} } }
using namespace openmfc::detail::cmirrorfile;
// CMirrorFile implementation
//
// CMirrorFile is an internal MFC helper (CFile-derived) used by MFC's
// registry / recent-file "mirror" writes (filelist.cpp). It behaves like a
// plain CFile for the primary file and carries an additional mirror-name
// member. Only three virtual methods are exported: Open, Close, Abort.
//
// Layout (MSVC x64): CMirrorFile : public CFile.
//   CFile = { vptr@0, void* m_hFile@8, CString m_strFileName@16 }  -> sizeof 24
//   CMirrorFile adds CString m_strMirrorName@24                    -> sizeof 32
//
// Exported (build-phase4/openmfc.def):
//   ?Abort@CMirrorFile@@UEAAXXZ
//   ?Close@CMirrorFile@@UEAAXXZ
//   ?Open@CMirrorFile@@UEAAHPEB_WIPEAVCFileException@@@Z

#define OPENMFC_APPCORE_IMPL
#include "openmfc/afx.h"
#include <windows.h>
#include <cstring>

// MS ABI calling convention
#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

// CMirrorFile : public CFile, adding one protected CString member.
// Derives from the repo's real CFile (declared in openmfc/afx.h) so the vtable
// base and the inherited m_hFile / m_strFileName field offsets are correct.
class CMirrorFile : public CFile {
public:
    CMirrorFile() {}
    virtual ~CMirrorFile() {}

    // Overridden virtual slots (these are the exported symbols).
    virtual int Open(const wchar_t* lpszFileName, UINT nOpenFlags, void* pException);
    virtual void Close() override;
    virtual void Abort();

protected:
    CString m_strMirrorName;
};

// Verify the binary layout matches the MSVC x64 expectation.

// Real CRuntimeClass with the correct object size for CMirrorFile.
// Note: CFile (and thus CMirrorFile) is not actually a CObject-derived class in
// this repo, so this is not an exported symbol; it merely records the correct
// sizeof for tooling/consistency and is referenced by the static_assert below.
namespace openmfc { namespace detail { namespace cmirrorfile {
extern CRuntimeClass g_runtimeClass_CMirrorFile;
} } }

// Accessor to reach CFile::CommonInit (protected) for the primary file, exactly
// like CFileAccessor in filecore.cpp.
struct CMirrorFileAccessor : CFile {
    static void InvokeCommonInit(CFile* pFile, const wchar_t* lpszFileName,
                                 unsigned int nOpenFlags, void* pTM) {
        static_cast<CMirrorFileAccessor*>(pFile)->CommonInit(lpszFileName, nOpenFlags, pTM);
    }
};

// ---- Out-of-line virtual definitions (real behavior) --------------------
// These mirror CFile's single-file behavior. The exported extern "C" thunks
// below delegate to the same logic.




// ---- Exported MS-ABI thunks --------------------------------------------



