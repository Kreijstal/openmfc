// RuntimeClasses — OpenMFC implementation.
// Sources: global_common_control_rtti.cpp, global_deferred_rtti.cpp, global_views_rendertarget_rtti.cpp, inet_filefind_rtti.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/DeferredRttiSupport.h"
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



// OpenMFC: RTTI getters (GetThisClass / GetRuntimeClass) for 9 classes.
// File-internal CRuntimeClass descriptors; real m_nObjectSize / m_wSchema harvested
// from mfc140u.dll (tools/harvest). GetRuntimeClass returns the static descriptor
// directly (no self-dispatch). // Symbol: markers auto-exclude the weak stubs.
// Named global_*_rtti.cpp so build_phase4.sh's shard glob compiles it.

#include "openmfc/afx.h"
#include "openmfc/afxwin.h"
#include "openmfc/afxmfc.h"
#include "openmfc/afxinet.h"

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

// m_lpszClassName, m_nObjectSize, m_wSchema, m_pfnCreateObject,
// m_pfnGetBaseClass, m_pNextClass, m_pClassInit.
#define VIEWS_RENDERTARGET_DESC(Cls, Size, Schema, BaseDesc) \
    static CRuntimeClass* AFXAPI _openmfc_gb_##Cls() { return (BaseDesc); } \
    CRuntimeClass class##Cls = { #Cls, (Size), (Schema), nullptr, &_openmfc_gb_##Cls, nullptr, nullptr }
VIEWS_RENDERTARGET_DESC(CSplitterWnd, 384, 0xFFFF, &CWnd::classCWnd);
VIEWS_RENDERTARGET_DESC(CSplitterWndEx, 384, 0xFFFF, &classCSplitterWnd);
VIEWS_RENDERTARGET_DESC(CBaseTransition, 56, 0xFFFF, &CObject::classCObject);
VIEWS_RENDERTARGET_DESC(CBitmapRenderTarget, 88, 0xFFFF, &CRenderTarget::classCRenderTarget);
VIEWS_RENDERTARGET_DESC(CGopherFileFind, 72, 0xFFFF, &CFileFind::classCFileFind);
VIEWS_RENDERTARGET_DESC(CHtmlEditDoc, 384, 0xFFFF, &CDocument::classCDocument);
VIEWS_RENDERTARGET_DESC(CHtmlEditView, 592, 0xFFFF, &CHtmlView::classCHtmlView);
VIEWS_RENDERTARGET_DESC(CHwndRenderTarget, 88, 0xFFFF, &CRenderTarget::classCRenderTarget);
VIEWS_RENDERTARGET_DESC(CMDITabProxyWnd, 240, 0xFFFF, &CWnd::classCWnd);
#undef VIEWS_RENDERTARGET_DESC

#define VIEWS_RENDERTARGET_GETTERS(Cls, GetThisSym, GetRtSym) \
    extern "C" CRuntimeClass* MS_ABI GetThisSym() { return &class##Cls; } \
    extern "C" CRuntimeClass* MS_ABI GetRtSym(const void*) { return &class##Cls; }

// Symbol: ??1CGopherLocator@@UEAA@XZ
extern "C" void MS_ABI impl___1CGopherLocator__UEAA_XZ(CGopherLocator* pThis) {
    if (pThis) pThis->~CGopherLocator();
}
#define DEF_GETTERS(Cls, GT, GRC) \
    extern "C" CRuntimeClass* MS_ABI GT() { return &class##Cls; } \
    extern "C" CRuntimeClass* MS_ABI GRC(const void*) { return &class##Cls; }
// Symbol: ?GetThisClass@CGopherFile@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CGopherFile@@UEBAPEAUCRuntimeClass@@XZ
DEF_GETTERS(CGopherFile,
            impl__GetThisClass_CGopherFile__SAPEAUCRuntimeClass__XZ,
            impl__GetRuntimeClass_CGopherFile__UEBAPEAUCRuntimeClass__XZ)
#undef DEF_GETTERS
#define VIEWS_RENDERTARGET_GETTERS(Cls, GetThisSym, GetRtSym) \
    extern "C" CRuntimeClass* MS_ABI GetThisSym() { return &class##Cls; } \
    extern "C" CRuntimeClass* MS_ABI GetRtSym(const void*) { return &class##Cls; }
// Symbol: ?GetThisClass@CGopherFileFind@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CGopherFileFind@@UEBAPEAUCRuntimeClass@@XZ
VIEWS_RENDERTARGET_GETTERS(CGopherFileFind,
           impl__GetThisClass_CGopherFileFind__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CGopherFileFind__UEBAPEAUCRuntimeClass__XZ)
#undef VIEWS_RENDERTARGET_GETTERS
// Implemented in inetcore.cpp; this unit had a second, unlinked copy.
// Symbol: ?GetThisClass@CFtpFileFind@@SAPEAUCRuntimeClass@@XZ
// Implemented in inetcore.cpp; this unit had a second, unlinked copy.
// Symbol: ?GetRuntimeClass@CFtpFileFind@@UEBAPEAUCRuntimeClass@@XZ
// Implemented in inetcore.cpp; this unit had a second, unlinked copy.
// Symbol: ?FindNextFileW@CFtpFileFind@@UEAAHXZ
// Implemented in inetcore.cpp; this unit had a second, unlinked copy.
// Symbol: ?CloseContext@CFtpFileFind@@MEAAXXZ
// Implemented in inetcore.cpp; this unit had a second, unlinked copy.
// Symbol: ?GetFileURL@CFtpFileFind@@UEBA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@XZ
// Implemented in inetcore.cpp; this unit had a second, unlinked copy.
// Symbol: ?GetThisClass@CGopherConnection@@SAPEAUCRuntimeClass@@XZ
// Implemented in inetcore.cpp; this unit had a second, unlinked copy.
// Symbol: ?GetRuntimeClass@CGopherConnection@@UEBAPEAUCRuntimeClass@@XZ
// Implemented in inetcore.cpp; this unit had a second, unlinked copy.
// Symbol: ?GetAttribute@CGopherConnection@@QEAAHAEAVCGopherLocator@@V?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@AEAV34@@Z
// Implemented in inetcore.cpp; this unit had a second, unlinked copy.
// Symbol: ?Write@CGopherFile@@UEAAXPEBXI@Z
// Implemented in inetcore.cpp; this unit had a second, unlinked copy.
// Symbol: ?WriteString@CGopherFile@@UEAAXPEB_W@Z
// Implemented in inetcore.cpp; this unit had a second, unlinked copy.
// Symbol: ?CloseContext@CGopherFileFind@@MEAAXXZ
// Implemented in inetcore.cpp; this unit had a second, unlinked copy.
// Symbol: ?FindFile@CGopherFileFind@@UEAAHAEAVCGopherLocator@@PEB_WK@Z
// Implemented in inetcore.cpp; this unit had a second, unlinked copy.
// Symbol: ?FindFile@CGopherFileFind@@UEAAHPEB_WK@Z
// Implemented in inetcore.cpp; this unit had a second, unlinked copy.
// Symbol: ?FindNextFileW@CGopherFileFind@@UEAAHXZ
// Implemented in inetcore.cpp; this unit had a second, unlinked copy.
// Symbol: ?GetCreationTime@CGopherFileFind@@UEBAHAEAVCTime@ATL@@@Z
// Implemented in inetcore.cpp; this unit had a second, unlinked copy.
// Symbol: ?GetCreationTime@CGopherFileFind@@UEBAHPEAU_FILETIME@@@Z
// Implemented in inetcore.cpp; this unit had a second, unlinked copy.
// Symbol: ?GetFileName@CGopherFileFind@@UEBA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@XZ
// Implemented in inetcore.cpp; this unit had a second, unlinked copy.
// Symbol: ?GetFilePath@CGopherFileFind@@UEBA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@XZ
// Implemented in inetcore.cpp; this unit had a second, unlinked copy.
// Symbol: ?GetFileTitle@CGopherFileFind@@UEBA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@XZ
// Implemented in inetcore.cpp; this unit had a second, unlinked copy.
// Symbol: ?GetFileURL@CGopherFileFind@@UEBA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@XZ
// Implemented in inetcore.cpp; this unit had a second, unlinked copy.
// Symbol: ?GetLastAccessTime@CGopherFileFind@@UEBAHAEAVCTime@ATL@@@Z
// Implemented in inetcore.cpp; this unit had a second, unlinked copy.
// Symbol: ?GetLastAccessTime@CGopherFileFind@@UEBAHPEAU_FILETIME@@@Z
// Implemented in inetcore.cpp; this unit had a second, unlinked copy.
// Symbol: ?GetLastWriteTime@CGopherFileFind@@UEBAHAEAVCTime@ATL@@@Z
// Implemented in inetcore.cpp; this unit had a second, unlinked copy.
// Symbol: ?GetLastWriteTime@CGopherFileFind@@UEBAHPEAU_FILETIME@@@Z
// Implemented in inetcore.cpp; this unit had a second, unlinked copy.
// Symbol: ?GetLength@CGopherFileFind@@UEBA_KXZ
// Implemented in inetcore.cpp; this unit had a second, unlinked copy.
// Symbol: ?GetLocator@CGopherFileFind@@QEBA?AVCGopherLocator@@XZ
// Implemented in inetcore.cpp; this unit had a second, unlinked copy.
// Symbol: ?GetRoot@CGopherFileFind@@UEBA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@XZ
// Implemented in inetcore.cpp; this unit had a second, unlinked copy.
// Symbol: ?GetScreenName@CGopherFileFind@@QEBA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@XZ
// Implemented in inetcore.cpp; this unit had a second, unlinked copy.
// Symbol: ?IsDots@CGopherFileFind@@UEBAHXZ
// Symbol: ??0CGopherFileFind@@QEAA@PEAVCGopherConnection@@_K@Z
extern "C" void* MS_ABI impl___0CGopherFileFind__QEAA_PEAVCGopherConnection___K_Z(
        void* p, CGopherConnection* pConn, unsigned long long dwContext) {
    // GopherFileFind ctor takes CFtpConnection*; reinterpret_cast is the
    // established precedent (inetcore.cpp:1534 does the same cast).
    return new (p) GopherFileFind(reinterpret_cast<CFtpConnection*>(pConn), dwContext);
}
// Symbol: ??1CGopherFileFind@@UEAA@XZ
extern "C" void MS_ABI impl___1CGopherFileFind__UEAA_XZ(void* pThis) {
    ((GopherFileFind*)pThis)->~GopherFileFind();
}
