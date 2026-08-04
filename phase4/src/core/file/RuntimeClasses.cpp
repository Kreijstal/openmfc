// RuntimeClasses — OpenMFC implementation.
// Sources: global_clongbinary_rtti.cpp, global_common_control_rtti.cpp, global_longbinary_rtti.cpp, inet_filefind_rtti.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/ClongbinaryRttiSupport.h"
#include "detail/InetFilefindRttiSupport.h"

#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxmfc.h"
#include "openmfc/afxinet.h"
#include "openmfc/afxdao.h"

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

// Destructors for classes whose repo header definitions faithfully model the
// real MFC ABI (thin CWnd/CDialog control wrappers with no extra owning state,
// plus a few value/handle-owning classes with explicit cleanup logic).
//
// Each impl simply invokes the C++ destructor, which performs exactly the work
// the real MFC destructor does (chaining to the base destructor and freeing any
// owned members).  All are null-guarded.
//
// Placeholder stub classes that are only modelled as `char _pad[N]` (uncertain
// real layout) are intentionally left with their weak stubs.

// ---- Thin Win32 control wrappers (CWnd-derived, defaulted destructors) ----
















// ---- Thin CDialog-derived wrappers (defaulted destructors) ----





// ---- Handle/value-owning classes with explicit cleanup ----



#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxdao.h"

#include <new>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

// CLongBinary is faithfully defined in include/openmfc/afxdao.h as a simple
// CObject-derived data holder (HGLOBAL m_hData; DWORD m_dwDataLength;). Its
// default constructor just initializes the CObject vtable and zeroes the two
// members, so a placement-new is a safe, exact reproduction.

// Implemented in manual_small_stub_implementations.cpp; this unit had a second, unlinked copy.
// Symbol: ?GetThisClass@CLongBinary@@SAPEAUCRuntimeClass@@XZ
// Implemented in manual_small_stub_implementations.cpp; this unit had a second, unlinked copy.
// Symbol: ?GetRuntimeClass@CLongBinary@@UEBAPEAUCRuntimeClass@@XZ
// Symbol: ??1CLongBinary@@UEAA@XZ
extern "C" void MS_ABI impl___1CLongBinary__UEAA_XZ(CLongBinary* pThis) {
    if (pThis) pThis->~CLongBinary();
}
// Symbol: ??0CLongBinary@@QEAA@XZ
extern "C" void* MS_ABI impl___0CLongBinary__QEAA_XZ(void* pThis) {
    if (!pThis) return pThis;
    return new (pThis) CLongBinary();
}
// Implemented in inetcore.cpp; this unit had a second, unlinked copy.
// Symbol: ?GetThisClass@CFileFind@@SAPEAUCRuntimeClass@@XZ
// Implemented in inetcore.cpp; this unit had a second, unlinked copy.
// Symbol: ?GetRuntimeClass@CFileFind@@UEBAPEAUCRuntimeClass@@XZ
// Implemented in inetcore.cpp; this unit had a second, unlinked copy.
// Symbol: ?FindNextFileW@CFileFind@@UEAAHXZ
// Implemented in inetcore.cpp; this unit had a second, unlinked copy.
// Symbol: ?CloseContext@CFileFind@@MEAAXXZ
// Implemented in inetcore.cpp; this unit had a second, unlinked copy.
// Symbol: ?GetFileName@CFileFind@@UEBA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@XZ
// Implemented in inetcore.cpp; this unit had a second, unlinked copy.
// Symbol: ?GetFilePath@CFileFind@@UEBA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@XZ
// Implemented in inetcore.cpp; this unit had a second, unlinked copy.
// Symbol: ?GetFileTitle@CFileFind@@UEBA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@XZ
// Implemented in inetcore.cpp; this unit had a second, unlinked copy.
// Symbol: ?GetFileURL@CFileFind@@UEBA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@XZ
// Implemented in inetcore.cpp; this unit had a second, unlinked copy.
// Symbol: ?GetRoot@CFileFind@@UEBA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@XZ
