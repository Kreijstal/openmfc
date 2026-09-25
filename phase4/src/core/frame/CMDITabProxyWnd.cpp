// CMDITabProxyWnd — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp
//
// The Windows 7 taskbar tab proxy window (shipping atlmfc/include/
// afxmdichildwndex.h:31, read on this host): `class CMDITabProxyWnd : public
// CWnd` with one data member, CMDIChildWndEx* m_pRelatedMDIChildFrame.  The
// class is not declared in include/openmfc, so every body here takes a void*
// `this` and reaches the member through the S_MDITabProxyWnd shadow below.
//
// Every body is transcribed from the retail disassembly.  disas.py reads the
// ANSI twin mfc140.dll; the bodies are byte-identical in mfc140u.dll, and the
// mfc140u entries quoted below were read from mfc140u.dll itself.  Four of the
// six handlers (OnSendIconicThumbnail, OnSendIconicLivePreviewBitmap,
// OnMouseActivate, OnSysCommand) are not in mfc140u_rva_symbols.json; every
// handler entry below was read from retail's own message map (AFX_MSGMAP
// 0x1802edb98 (mfc140u), whose pfnGetBaseMap is ?GetMessageMap@CWnd@@):
//     WM_DWMSENDICONICTHUMBNAIL         (0x323)  0x87230  OnSendIconicThumbnail
//     WM_DWMSENDICONICLIVEPREVIEWBITMAP (0x326)  0x872b0  OnSendIconicLivePreviewBitmap
//     WM_ACTIVATE                       (0x006)  0x87480  OnActivate
//     WM_MOUSEACTIVATE                  (0x021)  0x874b0  OnMouseActivate
//     WM_SYSCOMMAND                     (0x112)  0x873f0  OnSysCommand
//     WM_CLOSE                          (0x010)  0x874e0  OnClose
// and SetRelatedMDIChildFrame is the 8-byte body at 0x86c40 (mfc140u), which
// mfc140_rva_symbols.json names at 0x871d0 in the ANSI twin.  NOTE: OpenMFC's
// classCMDITabProxyWnd_msgmap (detail/Frame01MsgmapSupport.cpp) currently has
// NO entries, so none of these handlers is reachable through message routing
// in this tree yet; they are exported entry points only (headerRequest filed).
// Filling that map is not sufficient on its own for objects this file builds:
// they carry OpenMFC's CWnd vptr (see "Own vptr" below), so their virtual
// GetMessageMap / GetRuntimeClass answer CWnd's, not CMDITabProxyWnd's.
//
// NULL `this`: every exported body below returns early when `this` is NULL
// (with FALSE / 0 / NULL, TRUE for OnSendIconicLivePreviewBitmap, matching its
// no-related-frame result).  Retail has no such test in any of them; the
// per-function "fully transcribed" notes do not repeat this.
//
// ---------------------------------------------------------------------------
// Retail layout, from ??0CMDITabProxyWnd@@QEAA@XZ (entry RVA 0x86bb0, mfc140u):
//     CWnd::CWnd();                                // 0x28a700 (mfc140u)
//     vfptr = &CMDITabProxyWnd::`vftable';         // 0x1802ed8c0 (mfc140u)
//     m_pRelatedMDIChildFrame = NULL;              // movq $0,0xe8(%rbx)
//   +0x000  CWnd base (0xe8 bytes; m_hWnd at +0x40)
//   +0x0e8  CMDIChildWndEx* m_pRelatedMDIChildFrame
//   sizeof == 0xf0 (CreateObject allocates 0xf0; core/frame/RuntimeClasses.cpp
//   records 240 as the descriptor's m_nObjectSize)
//
// ---------------------------------------------------------------------------
// Virtual dispatch.  Retail calls virtuals on m_pRelatedMDIChildFrame through
// the MSVC CMDIChildWndEx vftable (0x1802ebf68 in mfc140; dumped slot by slot):
//     +0x400 OnGetIconicThumbnail(int, int)            header-inline, `return NULL`
//     +0x408 OnGetIconicLivePreviewBitmap(BOOL, CPoint&) header-inline, `return NULL`
//            (both slots hold the shared `xor eax,eax; ret` at 0x7260 in mfc140)
//     +0x410 OnTaskbarTabThumbnailStretch      +0x418 GetTaskbarPreviewWnd
//     +0x420 OnPressTaskbarThmbnailCloseButton +0x428 OnTaskbarTabThumbnailActivate
//     +0x430 OnTaskbarTabThumbnailMouseActivate +0x450 ActivateTopLevelFrame
// and CWnd slot +0x130 (OnDrawIconicThumbnailOrLivePreview; CWnd vftable
// 0x180334ff8 in mfc140) on the preview window.  OpenMFC's own CMDIChildWndEx
// and CWnd objects carry a mingw vtable on which those offsets mean something
// else, and OpenMFC's headers declare none of these as virtual.  Same convention
// as featurepack/controls/CMFCPreviewCtrlImpl.cpp: when the object's vptr lies
// inside this DLL the call goes straight to the export the retail slot holds.
// Retail's only in-DLL overrides of these slots are CHtmlView's
// OnDrawIconicThumbnailOrLivePreview (RVA 0x27ad30 (mfc140u)) -- CHtmlView is
// DYNCREATE, so an OpenMFC-built instance can be a preview window -- which is
// selected by IsKindOf(RUNTIME_CLASS(CHtmlView)); no shipping MFC class
// derives from CMDIChildWndEx, so its slots have no in-DLL override (checked:
// only afxwin.h and afxhtml.h declare OnDrawIconicThumbnailOrLivePreview, and
// no shipping atlmfc header has `public CMDIChildWndEx`).  Any other vptr
// belongs to a class the client compiled against the real headers, and the
// call is made through that MSVC vftable at the retail offset, so client
// overrides ARE reached.
//
// Own vptr.  ??0CWnd@@QEAA@XZ (core/window/CtorDtorPlacement.cpp) placement-news
// OpenMFC's CWnd; no MSVC-layout CMDITabProxyWnd vftable exists here, so the
// object keeps that vptr (recorded as g_ownVptr) instead of 0x1802ed8c0.  The
// destructor stores g_ownVptr back before running ~CWnd, mirroring retail's own
// "vfptr = &vftable" store.  Consequence: an object built by the constructor
// or CreateObject here reports CWnd's GetRuntimeClass and GetMessageMap.

#include "detail/ManualSmallStubImplementationsSupport.h"

#include <cstddef>
#include <cstring>

// Sibling thunks, each declared with the signature its mangled name describes;
// the file that defines each one (with a // Symbol: marker) is named.
extern "C" void*    MS_ABI impl___0CWnd__QEAA_XZ(void* pThis);                                        // core/window/CtorDtorPlacement.cpp
extern "C" void     MS_ABI impl___1CWnd__UEAA_XZ(void* pThis);                                        // core/window/CtorDtorPlacement.cpp
extern "C" void*    MS_ABI impl___2_YAPEAX_K_Z(std::size_t size);                                     // detail/MemcoreSupport.cpp
extern "C" __int64  MS_ABI impl__Default_CWnd__IEAA_JXZ(CWnd* pThis);                                 // core/window/Thunks.cpp
extern "C" CFrameWnd* MS_ABI impl__GetTopLevelFrame_CWnd__QEBAPEAVCFrameWnd__XZ(const CWnd* pThis);   // core/window/Thunks.cpp
extern "C" void     MS_ABI impl__ScreenToClient_CWnd__QEBAXPEAUtagRECT___Z(const CWnd* pThis, RECT* p0); // core/window/Thunks.cpp
extern "C" int      MS_ABI impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(
    const CObject* pThis, const CRuntimeClass* pClass);                                              // core/runtime/CObject.cpp
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMDIFrameWndEx__SAPEAUCRuntimeClass__XZ();        // core/frame/CMDIFrameWndEx.cpp
extern "C" CMDIChildWnd* MS_ABI impl__MDIGetActive_CMDIFrameWnd__QEBAPEAVCMDIChildWnd__PEAH_Z(
    const CMDIFrameWnd* pThis, int* p0);                                                             // core/frame/Thunks.cpp
extern "C" CDC*     MS_ABI impl__FromHandle_CDC__SAPEAV1_PEAUHDC_____Z(HDC hDC);                       // core/gdi/CDC.cpp
extern "C" void     MS_ABI impl__OnDrawIconicThumbnailOrLivePreview_CWnd__UEAAXAEAVCDC__VCRect__VCSize__HAEAH_Z(
    CWnd* pThis, CDC* pDC, CRect rc, CSize sz, int bDwmEnabled, int* bUpdateDwm);                   // core/window/CWnd.cpp
extern "C" void     MS_ABI impl__OnDrawIconicThumbnailOrLivePreview_CHtmlView__UEAAXAEAVCDC__VCRect__VCSize__HAEAH_Z(
    void* pThis, CDC* pDC, const RECT* rect, long long szRequiredThumbnailSize,
    int bIsThumbnail, int* bAlphaChannelSet);                                                        // core/view/CHtmlView.cpp (this is a CHtmlView*)
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CHtmlView__SAPEAUCRuntimeClass__XZ();            // core/view/RuntimeClasses.cpp
// CMDIChildWndEx virtuals whose exports the retail vftable slots hold (core/frame/CMDIChildWndEx.cpp)
extern "C" int      MS_ABI impl__OnTaskbarTabThumbnailStretch_CMDIChildWndEx__UEAAHPEAUHBITMAP____AEBVCRect__01_Z(
    CMDIChildWndEx* pThis, HBITMAP hBmpDst, const CRect& rectDst, HBITMAP hBmpSrc, const CRect& rectSrc);
extern "C" CWnd*    MS_ABI impl__GetTaskbarPreviewWnd_CMDIChildWndEx__UEAAPEAVCWnd__XZ(CMDIChildWndEx* pThis);
extern "C" void     MS_ABI impl__OnPressTaskbarThmbnailCloseButton_CMDIChildWndEx__UEAAXXZ(CMDIChildWndEx* pThis);
extern "C" void     MS_ABI impl__OnTaskbarTabThumbnailActivate_CMDIChildWndEx__UEAAXIPEAVCWnd__H_Z(
    CMDIChildWndEx* pThis, unsigned int nState, CWnd* pWndOther, int bMinimized);
extern "C" int      MS_ABI impl__OnTaskbarTabThumbnailMouseActivate_CMDIChildWndEx__UEAAHPEAVCWnd__II_Z(
    CMDIChildWndEx* pThis, CWnd* pDesktopWnd, unsigned int nHitTest, unsigned int message);
extern "C" void     MS_ABI impl__ActivateTopLevelFrame_CMDIChildWndEx__MEAAXXZ(CMDIChildWndEx* pThis);

// This file's own thunks that other bodies here call directly (defined below).
extern "C" void*    MS_ABI impl___0CMDITabProxyWnd__QEAA_XZ(void* pThis);
extern "C" HBITMAP  MS_ABI impl__GetClientBitmap_CMDITabProxyWnd__QEAAPEAUHBITMAP____HHH_Z(
    void* pThis, int nWidth, int nHeight, int bIsThumbnail);
extern "C" int      MS_ABI impl__IsMDIChildActive_CMDITabProxyWnd__IEAAHXZ(void* pThis);

namespace {

struct S_MDITabProxyWnd {
    unsigned char   m_cwndHead[0x40];
    HWND            m_hWnd;                    // +0x040  CWnd::m_hWnd
    unsigned char   m_cwndTail[0xe8 - 0x48];
    CMDIChildWndEx* m_pRelatedMDIChildFrame;   // +0x0e8
};
static_assert(offsetof(S_MDITabProxyWnd, m_hWnd) == 0x40, "CWnd::m_hWnd");
static_assert(offsetof(S_MDITabProxyWnd, m_pRelatedMDIChildFrame) == 0xe8,
              "retail reads m_pRelatedMDIChildFrame at 0xe8(%rcx) in every body");
static_assert(sizeof(S_MDITabProxyWnd) == 0xf0, "retail CreateObject allocates 0xf0");
static_assert(sizeof(CWnd) == 0xe8, "CWnd base must end where m_pRelatedMDIChildFrame starts");
static_assert(offsetof(CWnd, m_hWnd) == 0x40, "CWnd::m_hWnd at +0x40");

constexpr std::size_t kSizeofMDITabProxyWnd = 0xf0;   // the retail operator new immediate

inline S_MDITabProxyWnd* L(void* p) { return static_cast<S_MDITabProxyWnd*>(p); }
inline CWnd* W(void* p) { return static_cast<CWnd*>(p); }
inline CMDIChildWndEx* Related(void* p) { return L(p)->m_pRelatedMDIChildFrame; }

// ---------------------------------------------------------------------------
// Virtual dispatch (file header).
// ---------------------------------------------------------------------------
void* g_ownVptr = nullptr;   // the vptr ??0CWnd@@QEAA@XZ installs, recorded by the first ctor to run

HMODULE ModuleOfAddress(const void* p) {
    HMODULE h = nullptr;
    if (!::GetModuleHandleExW(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS |
                                  GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
                              static_cast<LPCWSTR>(p), &h)) {
        return nullptr;
    }
    return h;
}
// Does the object's vptr point into this DLL's image (an object OpenMFC built,
// mingw vtable) rather than into a client module (MSVC vftable)?
bool VptrInThisModule(const void* pObj) {
    HMODULE self = ModuleOfAddress(reinterpret_cast<const void*>(&ModuleOfAddress));
    return self != nullptr && ModuleOfAddress(*static_cast<void* const*>(pObj)) == self;
}
// Entry at vftable byte offset `off` (retail's `mov off(%rax),%rax`).
inline void* SlotAt(const void* p, std::size_t off) {
    return *reinterpret_cast<void* const*>(*static_cast<const unsigned char* const*>(p) + off);
}

// CMDIChildWndEx slots (retail offsets, file header).
using PFN_GetIconicThumbnail   = HBITMAP (MS_ABI*)(void*, int, int);                                // +0x400
using PFN_GetIconicLivePreview = HBITMAP (MS_ABI*)(void*, int, POINT*);                             // +0x408
using PFN_ThumbnailStretch     = int (MS_ABI*)(void*, HBITMAP, const RECT*, HBITMAP, const RECT*);  // +0x410
using PFN_GetPreviewWnd        = CWnd* (MS_ABI*)(void*);                                            // +0x418
using PFN_Void0                = void (MS_ABI*)(void*);                                             // +0x420, +0x450
using PFN_ThumbnailActivate    = void (MS_ABI*)(void*, unsigned int, CWnd*, int);                  // +0x428
using PFN_ThumbnailMouseAct    = int (MS_ABI*)(void*, CWnd*, unsigned int, unsigned int);          // +0x430
// CWnd slot +0x130: OnDrawIconicThumbnailOrLivePreview(CDC&, CRect, CSize, BOOL, BOOL&).
// Under the MSVC x64 ABI the 16-byte CRect goes by pointer to a caller copy and
// the 8-byte CSize in a register, exactly as retail passes them (r8 = &copy,
// r9 = the {nWidth, nHeight} quadword).
using PFN_DrawIconic           = void (MS_ABI*)(void*, CDC*, RECT*, long long, int, int*);         // +0x130

HBITMAP ChildOnGetIconicThumbnail(CMDIChildWndEx* p, int cx, int cy) {
    if (VptrInThisModule(p)) return nullptr;   // the header-inline base body
    return reinterpret_cast<PFN_GetIconicThumbnail>(SlotAt(p, 0x400))(p, cx, cy);
}
HBITMAP ChildOnGetIconicLivePreviewBitmap(CMDIChildWndEx* p, int bIsMDIChildActive, POINT* pt) {
    if (VptrInThisModule(p)) return nullptr;   // the header-inline base body
    return reinterpret_cast<PFN_GetIconicLivePreview>(SlotAt(p, 0x408))(p, bIsMDIChildActive, pt);
}
int ChildOnTaskbarTabThumbnailStretch(CMDIChildWndEx* p, HBITMAP hDst, const CRect& rDst,
                                      HBITMAP hSrc, const CRect& rSrc) {
    if (VptrInThisModule(p)) {
        return impl__OnTaskbarTabThumbnailStretch_CMDIChildWndEx__UEAAHPEAUHBITMAP____AEBVCRect__01_Z(
            p, hDst, rDst, hSrc, rSrc);
    }
    return reinterpret_cast<PFN_ThumbnailStretch>(SlotAt(p, 0x410))(
        p, hDst, reinterpret_cast<const RECT*>(&rDst), hSrc, reinterpret_cast<const RECT*>(&rSrc));
}
CWnd* ChildGetTaskbarPreviewWnd(CMDIChildWndEx* p) {
    if (VptrInThisModule(p)) return impl__GetTaskbarPreviewWnd_CMDIChildWndEx__UEAAPEAVCWnd__XZ(p);
    return reinterpret_cast<PFN_GetPreviewWnd>(SlotAt(p, 0x418))(p);
}
void ChildOnPressTaskbarThmbnailCloseButton(CMDIChildWndEx* p) {
    if (VptrInThisModule(p)) {
        impl__OnPressTaskbarThmbnailCloseButton_CMDIChildWndEx__UEAAXXZ(p);
        return;
    }
    reinterpret_cast<PFN_Void0>(SlotAt(p, 0x420))(p);
}
void ChildOnTaskbarTabThumbnailActivate(CMDIChildWndEx* p, unsigned int nState, CWnd* pWndOther, int bMinimized) {
    if (VptrInThisModule(p)) {
        impl__OnTaskbarTabThumbnailActivate_CMDIChildWndEx__UEAAXIPEAVCWnd__H_Z(p, nState, pWndOther, bMinimized);
        return;
    }
    reinterpret_cast<PFN_ThumbnailActivate>(SlotAt(p, 0x428))(p, nState, pWndOther, bMinimized);
}
int ChildOnTaskbarTabThumbnailMouseActivate(CMDIChildWndEx* p, CWnd* pDesktopWnd,
                                            unsigned int nHitTest, unsigned int message) {
    if (VptrInThisModule(p)) {
        return impl__OnTaskbarTabThumbnailMouseActivate_CMDIChildWndEx__UEAAHPEAVCWnd__II_Z(
            p, pDesktopWnd, nHitTest, message);
    }
    return reinterpret_cast<PFN_ThumbnailMouseAct>(SlotAt(p, 0x430))(p, pDesktopWnd, nHitTest, message);
}
void ChildActivateTopLevelFrame(CMDIChildWndEx* p) {
    if (VptrInThisModule(p)) {
        impl__ActivateTopLevelFrame_CMDIChildWndEx__MEAAXXZ(p);
        return;
    }
    reinterpret_cast<PFN_Void0>(SlotAt(p, 0x450))(p);
}
void WndOnDrawIconicThumbnailOrLivePreview(CWnd* pWnd, CDC* pDC, const CRect& rect,
                                           int cx, int cy, int bIsThumbnail, int* pbAlphaChannelSet) {
    RECT rcCopy = { rect.left, rect.top, rect.right, rect.bottom };
    const long long size = static_cast<long long>(static_cast<unsigned int>(cx)) |
                           (static_cast<long long>(static_cast<unsigned int>(cy)) << 32);
    if (VptrInThisModule(pWnd)) {
        if (impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(
                pWnd, impl__GetThisClass_CHtmlView__SAPEAUCRuntimeClass__XZ())) {
            impl__OnDrawIconicThumbnailOrLivePreview_CHtmlView__UEAAXAEAVCDC__VCRect__VCSize__HAEAH_Z(
                pWnd, pDC, &rcCopy, size, bIsThumbnail, pbAlphaChannelSet);
            return;
        }
        impl__OnDrawIconicThumbnailOrLivePreview_CWnd__UEAAXAEAVCDC__VCRect__VCSize__HAEAH_Z(
            pWnd, pDC, rect, CSize(cx, cy), bIsThumbnail, pbAlphaChannelSet);
        return;
    }
    reinterpret_cast<PFN_DrawIconic>(SlotAt(pWnd, 0x130))(pWnd, pDC, &rcCopy, size, bIsThumbnail,
                                                          pbAlphaChannelSet);
}

// DYNAMIC_DOWNCAST(CMDIFrameWndEx, pChild->GetTopLevelFrame()) -- the inlined
// sequence GetTopLevelFrame (0x28e490, mfc140u) + IsKindOf (0x234cf0, mfc140u)
// against RUNTIME_CLASS(CMDIFrameWndEx) (0x1802ee478, mfc140u) that
// IsMDIChildActive, OnSysCommand and OnSendIconicLivePreviewBitmap all open with.
CFrameWnd* TopLevelMDIFrameEx(CMDIChildWndEx* pChild) {
    if (pChild == nullptr) return nullptr;   // see IsMDIChildActive for why this guard exists
    CFrameWnd* pTop = impl__GetTopLevelFrame_CWnd__QEBAPEAVCFrameWnd__XZ(pChild);
    if (pTop == nullptr) return nullptr;
    if (!impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(
            pTop, impl__GetThisClass_CMDIFrameWndEx__SAPEAUCRuntimeClass__XZ())) {
        return nullptr;
    }
    return pTop;
}

// Retail's non-exported lazily-bound dwmapi wrappers: DwmSetIconicThumbnail
// (0x1cae54 mfc140u / 0x1c9434 mfc140) and DwmSetIconicLivePreviewBitmap
// (0x1caed8 mfc140u / 0x1c94b8 mfc140).  Each loads L"dwmapi.dll" through
// retail's system-directory-only loader (0xd8230 mfc140: LoadLibraryExW(name,
// NULL, LOAD_LIBRARY_SEARCH_SYSTEM32) when kernel32 exports
// SetDefaultDllDirectories, otherwise GetSystemDirectoryW + L"\\" + name and
// LoadLibraryW), GetProcAddress's the named export, caches the EncodePointer'd
// result, forwards all arguments and returns its HRESULT; when the library or
// the export is missing it returns E_FAIL (0x80004005).  The same two-way load
// is done here (the encoded cache is an in-process hardening detail with no
// observable effect); the loader is a private copy of the identical file-local
// helpers in core/frame/CFrameImpl.cpp and featurepack/ribbon/CMFCRibbonBar.cpp.
HMODULE LoadSystemLibrary(const wchar_t* lpszName) {
    HMODULE hKernel = ::GetModuleHandleW(L"kernel32.dll");
    if (hKernel != nullptr && ::GetProcAddress(hKernel, "SetDefaultDllDirectories") != nullptr) {
        return ::LoadLibraryExW(lpszName, nullptr, 0x00000800 /*LOAD_LIBRARY_SEARCH_SYSTEM32*/);
    }
    wchar_t szPath[MAX_PATH + 1] = {};
    const UINT nLen = ::GetSystemDirectoryW(szPath, MAX_PATH + 1);
    if (nLen == 0 || nLen > MAX_PATH) return nullptr;
    UINT nPos = nLen;
    if (szPath[nPos - 1] != L'\\') {
        if (nPos + 1 > MAX_PATH) return nullptr;
        szPath[nPos++] = L'\\';
    }
    const size_t nName = ::wcslen(lpszName);
    if (nPos + nName > MAX_PATH) return nullptr;
    std::memcpy(szPath + nPos, lpszName, (nName + 1) * sizeof(wchar_t));
    return ::LoadLibraryW(szPath);
}
FARPROC DwmProc(FARPROC& cache, const char* name) {
    if (cache == nullptr) {
        HMODULE hDwm = LoadSystemLibrary(L"dwmapi.dll");
        if (hDwm != nullptr) cache = ::GetProcAddress(hDwm, name);
    }
    return cache;
}
typedef HRESULT (WINAPI* PFN_DwmSetIconicThumbnail)(HWND, HBITMAP, DWORD);
typedef HRESULT (WINAPI* PFN_DwmSetIconicLivePreviewBitmap)(HWND, HBITMAP, POINT*, DWORD);
FARPROC g_pfnDwmSetIconicThumbnail = nullptr;
FARPROC g_pfnDwmSetIconicLivePreviewBitmap = nullptr;

HRESULT CallDwmSetIconicThumbnail(HWND hWnd, HBITMAP hBmp, DWORD dwSITFlags) {
    FARPROC p = DwmProc(g_pfnDwmSetIconicThumbnail, "DwmSetIconicThumbnail");
    if (p == nullptr) return E_FAIL;
    return reinterpret_cast<PFN_DwmSetIconicThumbnail>(reinterpret_cast<void*>(p))(hWnd, hBmp, dwSITFlags);
}
HRESULT CallDwmSetIconicLivePreviewBitmap(HWND hWnd, HBITMAP hBmp, POINT* pptClient, DWORD dwSITFlags) {
    FARPROC p = DwmProc(g_pfnDwmSetIconicLivePreviewBitmap, "DwmSetIconicLivePreviewBitmap");
    if (p == nullptr) return E_FAIL;
    return reinterpret_cast<PFN_DwmSetIconicLivePreviewBitmap>(reinterpret_cast<void*>(p))(
        hWnd, hBmp, pptClient, dwSITFlags);
}

// msimg32!AlphaBlend.  Retail reaches it through a delay-load import slot
// (0x1803e9020 in mfc140u); OpenMFC does not link msimg32, so it is resolved
// lazily, as core/gdi/CDrawingManager.cpp does.
typedef BOOL (WINAPI* PFN_AlphaBlend)(HDC, int, int, int, int, HDC, int, int, int, int, BLENDFUNCTION);
PFN_AlphaBlend GetAlphaBlend() {
    static PFN_AlphaBlend s_pfn = nullptr;
    static bool s_bTried = false;
    if (!s_bTried) {
        s_bTried = true;
        HMODULE h = ::LoadLibraryW(L"msimg32.dll");
        if (h != nullptr)
            s_pfn = reinterpret_cast<PFN_AlphaBlend>(reinterpret_cast<void*>(::GetProcAddress(h, "AlphaBlend")));
    }
    return s_pfn;
}

// What retail's ATL CImage::CreateEx(nWidth, nHeight, 32, BI_RGB, NULL,
// createAlphaChannel) (inlined helper at 0x875bc mfc140u / 0x87b6c mfc140)
// produces: a 32bpp BI_RGB DIB section from CreateDIBSection(NULL, &bmi,
// DIB_RGB_COLORS, &bits, NULL, 0) with biPlanes = 1 and biHeight as given
// (positive = bottom-up, negative = top-down).  Only the HBITMAP and the raw
// bits pointer are kept -- the pieces GetClientBitmap reads.
HBITMAP Create32bppDib(int nWidth, int nHeight, void** ppBits) {
    BITMAPINFO bmi;
    std::memset(&bmi, 0, sizeof(bmi));
    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth = nWidth;
    bmi.bmiHeader.biHeight = nHeight;
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 32;
    bmi.bmiHeader.biCompression = BI_RGB;
    void* pBits = nullptr;
    HBITMAP h = ::CreateDIBSection(nullptr, &bmi, DIB_RGB_COLORS, &pBits, nullptr, 0);
    if (ppBits) *ppBits = h ? pBits : nullptr;
    return h;
}

} // namespace

// Retail ??0CMDITabProxyWnd@@QEAA@XZ (entry RVA 0x86bb0, mfc140u), fully transcribed:
//     CWnd::CWnd();                                // 0x28a700 (mfc140u)
//     vfptr = &CMDITabProxyWnd::`vftable';         // 0x1802ed8c0 (mfc140u)
//     m_pRelatedMDIChildFrame = NULL;              // +0xe8
//     return this;
// DEVIATIONS: no MSVC-layout vftable exists for this class here, so `this`
// keeps the vptr ??0CWnd@@ installs (recorded as g_ownVptr, file header);
// retail has no NULL check.
// Symbol: ??0CMDITabProxyWnd@@QEAA@XZ
extern "C" void* MS_ABI impl___0CMDITabProxyWnd__QEAA_XZ(void* pThis) {
    if (pThis == nullptr) return pThis;
    impl___0CWnd__QEAA_XZ(pThis);
    if (g_ownVptr == nullptr) g_ownVptr = *static_cast<void**>(pThis);
    L(pThis)->m_pRelatedMDIChildFrame = nullptr;
    return pThis;
}
// Retail ??1CMDITabProxyWnd@@UEAA@XZ (entry RVA 0x86c30, mfc140u), fully transcribed:
//     vfptr = &CMDITabProxyWnd::`vftable';         // 0x1802ed8c0 (mfc140u)
//     CWnd::~CWnd();                               // tail jump to 0x28b740 (mfc140u)
// DEVIATION: the own-vftable store becomes the g_ownVptr store (file header),
// so that ??1CWnd@@'s C++ virtual-destructor call dispatches on OpenMFC's
// table and not on a client subclass's MSVC one; retail has no NULL check.
// Symbol: ??1CMDITabProxyWnd@@UEAA@XZ
extern "C" void MS_ABI impl___1CMDITabProxyWnd__UEAA_XZ(void* pThis) {
    if (pThis == nullptr) return;
    if (g_ownVptr != nullptr) *static_cast<void**>(pThis) = g_ownVptr;
    impl___1CWnd__UEAA_XZ(pThis);
}
// Retail ?CreateObject@CMDITabProxyWnd@@ (entry RVA 0x86b60, mfc140u), fully transcribed:
//     void* p = operator new(0xf0);                // 0x27f0 (mfc140u)
//     return p ? new(p) CMDITabProxyWnd : NULL;    // ctor 0x86bb0 (mfc140u)
// The allocation goes through the DLL's exported ::operator new thunk
// (??2@YAPEAX_K@Z), as core/frame/CMiniFrameWnd.cpp's CreateObject does.
// Symbol: ?CreateObject@CMDITabProxyWnd@@SAPEAVCObject@@XZ
extern "C" void* MS_ABI impl__CreateObject_CMDITabProxyWnd__SAPEAVCObject__XZ() {
    void* p = impl___2_YAPEAX_K_Z(kSizeofMDITabProxyWnd);
    if (p == nullptr) return nullptr;
    return impl___0CMDITabProxyWnd__QEAA_XZ(p);
}

// Retail entry RVA 0x86cc0 (mfc140u), transcribed (CImage = ATL CImage, inlined):
//     if (m_pRelatedMDIChildFrame == NULL || nWidth <= 0 || nHeight <= 0) return NULL;
//     CRect rectWnd(0, 0, 0, 0);
//     CWnd* pWnd = m_pRelatedMDIChildFrame->GetTaskbarPreviewWnd();   // slot +0x418
//     ::GetWindowRect(pWnd->m_hWnd, &rectWnd);                        // IAT 0x1802c6c08 (mfc140u)
//     ::OffsetRect(&rectWnd, -rectWnd.left, -rectWnd.top);            // IAT 0x1802c72f0 (mfc140u)
//     if (rectWnd.Width() <= 0 || rectWnd.Height() <= 0) return NULL;
//     CImage bmpSrc; bmpSrc.CreateEx(W, H, 32, BI_RGB, NULL, createAlphaChannel);  // bottom-up
//     CClientDC dc(pWnd);                                             // 0x2a3b20 (mfc140u)
//     CDC dcMem; dcMem.Attach(::CreateCompatibleDC(dc.m_hDC));
//     HBITMAP hOld = bmpSrc ? ::SelectObject(dcMem, bmpSrc) : NULL;
//     BOOL bAlphaChannelSet = FALSE;
//     pWnd->OnDrawIconicThumbnailOrLivePreview(dcMem, rectWnd,        // CWnd slot +0x130
//                                              CSize(nWidth, nHeight), bIsThumbnail, bAlphaChannelSet);
//     if (hOld) ::SelectObject(dcMem, hOld);
//     if (!bAlphaChannelSet)                  // force every source pixel opaque
//         for (i = 0; i < W * H; i++) pixel[i].alpha = 0xff;          // from the lowest DIB address
//     double scale = min((double)nWidth / W, (double)nHeight / H);
//     CRect rectDst(0, 0, (int)(scale * W), (int)(scale * H));       // truncating casts
//     CImage bmpDst; bmpDst.CreateEx(cx, -cy, 32, BI_RGB, NULL, createAlphaChannel);  // top-down
//     if (!bIsThumbnail || !m_pRelatedMDIChildFrame->OnTaskbarTabThumbnailStretch(   // slot +0x410
//                              bmpDst, rectDst, bmpSrc, rectWnd)) {
//         CDC dcDst; dcDst.Attach(::CreateCompatibleDC(dc.m_hDC));
//         HBITMAP hOld2 = bmpDst ? ::SelectObject(dcDst, bmpDst) : NULL;
//         BLENDFUNCTION bf = { AC_SRC_OVER, 0, 255, bmpSrc.m_bHasAlphaChannel ? AC_SRC_ALPHA : 0 };
//         ::AlphaBlend(dcDst, rectDst.left, rectDst.top, rectDst.Width(), rectDst.Height(),
//                      bmpSrc.GetDC(), rectWnd.left, rectWnd.top, rectWnd.Width(), rectWnd.Height(), bf);
//         bmpSrc.ReleaseDC();
//         if (hOld2) ::SelectObject(dcDst, hOld2);
//         // ~dcDst: DeleteDC
//     }
//     return bmpDst.Detach();                 // then ~dcMem DeleteDC; ~dc ReleaseDC; ~bmpSrc deletes the source DIB
// (CImage ctor 0x87a90, CreateEx 0x875bc, GetDC 0x87508, dtor 0x8792c, all
// mfc140u; AlphaBlend is the delay-load slot 0x1803e9020 (mfc140u).)
// Ownership quirk, reproduced as-is: the base OnTaskbarTabThumbnailStretch
// (0x86270 mfc140u) Attach()es hBmpSrc to its own CImage and DESTROYS it on
// return (its ~CImage at 0x8792c calls ::DeleteObject), so on the thumbnail
// path retail's ~bmpSrc here deletes an already-deleted handle, and if the
// stretch returned FALSE the AlphaBlend fallback reads a deleted bitmap.  This
// body does exactly the same.
// DEVIATIONS: (1) CImage's DC comes from ATL's static 4-slot memory-DC cache
// (CreateCompatibleDC(NULL) when the cache is empty); here a fresh
// CreateCompatibleDC(NULL) is made and deleted around the AlphaBlend.
// (2) dcMem is handed to the preview window as CDC::FromHandle's temporary
// wrapper rather than a stack CDC; there is no thunk-reachable way to build a
// vptr-carrying CDC on the stack in this file.  If ::CreateCompatibleDC
// fails, FromHandle(NULL) yields a NULL CDC* where retail passes a CDC whose
// m_hDC is NULL.  (3) CClientDC throws on ::GetDC failure;
// here that returns NULL after freeing the source DIB.  (4) The alpha loop is
// skipped when the source DIB could not be created (retail would write
// through a NULL bits pointer).  (5) A NULL pWnd returns NULL (retail faults).
// Symbol: ?GetClientBitmap@CMDITabProxyWnd@@QEAAPEAUHBITMAP__@@HHH@Z
extern "C" HBITMAP MS_ABI impl__GetClientBitmap_CMDITabProxyWnd__QEAAPEAUHBITMAP____HHH_Z(
    void* pThis, int nWidth, int nHeight, int bIsThumbnail) {
    if (pThis == nullptr) return nullptr;
    CMDIChildWndEx* pChild = Related(pThis);
    if (pChild == nullptr || nWidth <= 0 || nHeight <= 0) return nullptr;

    CRect rectWnd(0, 0, 0, 0);
    CWnd* pWnd = ChildGetTaskbarPreviewWnd(pChild);
    if (pWnd == nullptr) return nullptr;   // deviation (5)
    ::GetWindowRect(pWnd->m_hWnd, reinterpret_cast<RECT*>(&rectWnd));
    ::OffsetRect(reinterpret_cast<RECT*>(&rectWnd), -rectWnd.left, -rectWnd.top);
    const int nSrcW = rectWnd.right - rectWnd.left;
    const int nSrcH = rectWnd.bottom - rectWnd.top;
    if (nSrcW <= 0 || nSrcH <= 0) return nullptr;

    void* pSrcBits = nullptr;
    HBITMAP hBmpSrc = Create32bppDib(nSrcW, nSrcH, &pSrcBits);   // bottom-up
    const bool bSrcHasAlpha = hBmpSrc != nullptr;                // CreateEx sets m_bHasAlphaChannel on success only

    HDC hdcClient = ::GetDC(pWnd->m_hWnd);
    if (hdcClient == nullptr) {                                  // deviation (3)
        if (hBmpSrc) ::DeleteObject(hBmpSrc);
        return nullptr;
    }
    HDC hdcMem = ::CreateCompatibleDC(hdcClient);
    HGDIOBJ hOld = hBmpSrc ? ::SelectObject(hdcMem, hBmpSrc) : nullptr;
    int bAlphaChannelSet = FALSE;
    WndOnDrawIconicThumbnailOrLivePreview(pWnd, impl__FromHandle_CDC__SAPEAV1_PEAUHDC_____Z(hdcMem),
                                          rectWnd, nWidth, nHeight, bIsThumbnail, &bAlphaChannelSet);
    if (hOld) ::SelectObject(hdcMem, hOld);

    if (!bAlphaChannelSet && pSrcBits != nullptr) {              // deviation (4) on the NULL test
        unsigned char* p = static_cast<unsigned char*>(pSrcBits);
        const int nPixels = nSrcW * nSrcH;
        for (int i = 0; i < nPixels; ++i, p += 4) p[3] = 0xff;
    }

    double scaleY = static_cast<double>(nHeight) / nSrcH;
    double scaleX = static_cast<double>(nWidth) / nSrcW;
    const double scale = (scaleY > scaleX) ? scaleX : scaleY;
    const int cx = static_cast<int>(scale * nSrcW);
    const int cy = static_cast<int>(scale * nSrcH);
    CRect rectDst(0, 0, cx, cy);
    HBITMAP hBmpDst = Create32bppDib(cx, -cy, nullptr);          // top-down

    if (!bIsThumbnail ||
        !ChildOnTaskbarTabThumbnailStretch(pChild, hBmpDst, rectDst, hBmpSrc, rectWnd)) {
        HDC hdcDst = ::CreateCompatibleDC(hdcClient);
        HGDIOBJ hOld2 = hBmpDst ? ::SelectObject(hdcDst, hBmpDst) : nullptr;
        BLENDFUNCTION bf;
        bf.BlendOp = AC_SRC_OVER;
        bf.BlendFlags = 0;
        bf.SourceConstantAlpha = 0xff;
        bf.AlphaFormat = bSrcHasAlpha ? AC_SRC_ALPHA : 0;
        HDC hdcSrc = ::CreateCompatibleDC(nullptr);              // deviation (1)
        HGDIOBJ hOldSrc = (hdcSrc && hBmpSrc) ? ::SelectObject(hdcSrc, hBmpSrc) : nullptr;
        if (PFN_AlphaBlend pfn = GetAlphaBlend()) {
            pfn(hdcDst, rectDst.left, rectDst.top, rectDst.right - rectDst.left, rectDst.bottom - rectDst.top,
                hdcSrc, rectWnd.left, rectWnd.top, nSrcW, nSrcH, bf);
        }
        if (hdcSrc) {
            if (hOldSrc) ::SelectObject(hdcSrc, hOldSrc);
            ::DeleteDC(hdcSrc);
        }
        if (hOld2) ::SelectObject(hdcDst, hOld2);
        if (hdcDst) ::DeleteDC(hdcDst);
    }

    // Retail's epilogue order: bmpDst.Detach(), ~dcMem, ~dc, ~bmpSrc.
    if (hdcMem) ::DeleteDC(hdcMem);                              // ~CDC dcMem
    ::ReleaseDC(pWnd->m_hWnd, hdcClient);                        // ~CClientDC
    if (hBmpSrc) ::DeleteObject(hBmpSrc);                        // ~CImage bmpSrc
    return hBmpDst;                                              // bmpDst.Detach()
}

// Retail entry RVA 0x86c50 (mfc140u), fully transcribed:
//     CMDIFrameWndEx* pFrame = DYNAMIC_DOWNCAST(CMDIFrameWndEx,
//                                 m_pRelatedMDIChildFrame->GetTopLevelFrame());
//     if (::IsIconic(pFrame->m_hWnd)) return FALSE;                   // IAT 0x1802c6c28 (mfc140u)
//     return pFrame->MDIGetActive(NULL) == m_pRelatedMDIChildFrame;  // 0x2a6ad0 (mfc140u)
// DEVIATION: retail tests neither pointer -- a NULL m_pRelatedMDIChildFrame is
// handed to GetTopLevelFrame, and a NULL pFrame is dereferenced (+0x40) -- so
// both cases are reported as "not active" (FALSE) here instead of faulting.
// Symbol: ?IsMDIChildActive@CMDITabProxyWnd@@IEAAHXZ
extern "C" int MS_ABI impl__IsMDIChildActive_CMDITabProxyWnd__IEAAHXZ(void* pThis) {
    if (pThis == nullptr) return FALSE;
    CMDIChildWndEx* pChild = Related(pThis);
    CFrameWnd* pFrame = TopLevelMDIFrameEx(pChild);
    if (pFrame == nullptr) return FALSE;
    if (::IsIconic(pFrame->m_hWnd)) return FALSE;
    CMDIChildWnd* pActive = impl__MDIGetActive_CMDIFrameWnd__QEBAPEAVCMDIChildWnd__PEAH_Z(
        static_cast<CMDIFrameWnd*>(pFrame), nullptr);
    return static_cast<CWnd*>(pActive) == static_cast<CWnd*>(pChild) ? TRUE : FALSE;
}

// Retail entry RVA 0x87480 (mfc140u), fully transcribed:
//     if (m_pRelatedMDIChildFrame != NULL)
//         m_pRelatedMDIChildFrame->OnTaskbarTabThumbnailActivate(nState, pWndOther, bMinimized);  // slot +0x428
// Symbol: ?OnActivate@CMDITabProxyWnd@@IEAAXIPEAVCWnd@@H@Z
extern "C" void MS_ABI impl__OnActivate_CMDITabProxyWnd__IEAAXIPEAVCWnd__H_Z(
    void* pThis, unsigned int nState, CWnd* pWndOther, int bMinimized) {
    if (pThis == nullptr) return;
    if (CMDIChildWndEx* pChild = Related(pThis)) {
        ChildOnTaskbarTabThumbnailActivate(pChild, nState, pWndOther, bMinimized);
    }
}

// Retail entry RVA 0x874e0 (mfc140u), fully transcribed (Default() is NOT called):
//     if (m_pRelatedMDIChildFrame != NULL)
//         m_pRelatedMDIChildFrame->OnPressTaskbarThmbnailCloseButton();   // slot +0x420
// Symbol: ?OnClose@CMDITabProxyWnd@@IEAAXXZ
extern "C" void MS_ABI impl__OnClose_CMDITabProxyWnd__IEAAXXZ(void* pThis) {
    if (pThis == nullptr) return;
    if (CMDIChildWndEx* pChild = Related(pThis)) {
        ChildOnPressTaskbarThmbnailCloseButton(pChild);
    }
}

// Retail entry RVA 0x874b0 (mfc140u; the WM_MOUSEACTIVATE entry of the message
// map, see file header), fully transcribed:
//     if (m_pRelatedMDIChildFrame == NULL) return 0;
//     return m_pRelatedMDIChildFrame->OnTaskbarTabThumbnailMouseActivate(     // slot +0x430
//                pDesktopWnd, nHitTest, message);
// Symbol: ?OnMouseActivate@CMDITabProxyWnd@@IEAAHPEAVCWnd@@II@Z
extern "C" int MS_ABI impl__OnMouseActivate_CMDITabProxyWnd__IEAAHPEAVCWnd__II_Z(
    void* pThis, CWnd* pDesktopWnd, unsigned int nHitTest, unsigned int message) {
    if (pThis == nullptr) return 0;
    CMDIChildWndEx* pChild = Related(pThis);
    if (pChild == nullptr) return 0;
    return ChildOnTaskbarTabThumbnailMouseActivate(pChild, pDesktopWnd, nHitTest, message);
}

// Retail entry RVA 0x872b0 (mfc140u; the WM_DWMSENDICONICLIVEPREVIEWBITMAP
// entry of the message map), fully transcribed:
//     if (m_pRelatedMDIChildFrame == NULL) return TRUE;
//     BOOL bActive = IsMDIChildActive();                              // 0x86c50 (mfc140u)
//     CPoint ptLocation(0, 0);
//     HBITMAP hBmp = m_pRelatedMDIChildFrame->OnGetIconicLivePreviewBitmap(bActive, ptLocation);  // slot +0x408
//     if (hBmp == NULL) {
//         CMDIFrameWndEx* pFrame = DYNAMIC_DOWNCAST(CMDIFrameWndEx,
//                                     m_pRelatedMDIChildFrame->GetTopLevelFrame());
//         CRect rect(0, 0, 0, 0);
//         CWnd* pWnd = m_pRelatedMDIChildFrame->GetTaskbarPreviewWnd();  // slot +0x418
//         ::GetWindowRect(pWnd->m_hWnd, &rect);
//         pFrame->ScreenToClient(&rect);          // CWnd::ScreenToClient(LPRECT): call 0x2a32b0 (mfc140u) = export 0x2a11f0 (mfc140)
//         ptLocation = rect.TopLeft();
//         hBmp = GetClientBitmap(rect.Width(), rect.Height(), FALSE); // 0x86cc0 (mfc140u)
//     }
//     DwmSetIconicLivePreviewBitmap(m_hWnd, hBmp, &ptLocation, 0);   // loader 0x1caed8 (mfc140u)
//     ::DeleteObject(hBmp);                                           // IAT 0x1802c6278 (mfc140u)
//     return FALSE;
// DEVIATIONS: retail dereferences a NULL pWnd and hands a NULL pFrame to
// ScreenToClient (which reads its m_hWnd); here a NULL pWnd leaves the
// rectangle empty (GetClientBitmap then returns NULL for the empty size) and
// a NULL pFrame skips the conversion, leaving the location in screen
// coordinates.  Neither case is reachable while the child sits in a
// CMDIFrameWndEx with a live view.
// Symbol: ?OnSendIconicLivePreviewBitmap@CMDITabProxyWnd@@IEAAHXZ
extern "C" int MS_ABI impl__OnSendIconicLivePreviewBitmap_CMDITabProxyWnd__IEAAHXZ(void* pThis) {
    if (pThis == nullptr) return TRUE;
    CMDIChildWndEx* pChild = Related(pThis);
    if (pChild == nullptr) return TRUE;
    const int bActive = impl__IsMDIChildActive_CMDITabProxyWnd__IEAAHXZ(pThis);
    POINT ptLocation = { 0, 0 };
    HBITMAP hBmp = ChildOnGetIconicLivePreviewBitmap(pChild, bActive, &ptLocation);
    if (hBmp == nullptr) {
        CFrameWnd* pFrame = TopLevelMDIFrameEx(pChild);
        RECT rect = { 0, 0, 0, 0 };
        CWnd* pWnd = ChildGetTaskbarPreviewWnd(pChild);
        if (pWnd != nullptr) ::GetWindowRect(pWnd->m_hWnd, &rect);
        if (pFrame != nullptr) impl__ScreenToClient_CWnd__QEBAXPEAUtagRECT___Z(pFrame, &rect);
        ptLocation.x = rect.left;
        ptLocation.y = rect.top;
        hBmp = impl__GetClientBitmap_CMDITabProxyWnd__QEAAPEAUHBITMAP____HHH_Z(
            pThis, rect.right - rect.left, rect.bottom - rect.top, FALSE);
    }
    CallDwmSetIconicLivePreviewBitmap(L(pThis)->m_hWnd, hBmp, &ptLocation, 0);
    ::DeleteObject(hBmp);
    return FALSE;
}

// Retail entry RVA 0x87230 (mfc140u; the WM_DWMSENDICONICTHUMBNAIL entry of
// the message map), fully transcribed:
//     if (m_pRelatedMDIChildFrame != NULL) {
//         HBITMAP hBmp = m_pRelatedMDIChildFrame->OnGetIconicThumbnail(cx, cy);  // slot +0x400
//         if (hBmp == NULL) hBmp = GetClientBitmap(cx, cy, TRUE);    // 0x86cc0 (mfc140u)
//         DwmSetIconicThumbnail(m_hWnd, hBmp, 0);                    // loader 0x1cae54 (mfc140u)
//         ::DeleteObject(hBmp);                                       // IAT 0x1802c6278 (mfc140u)
//     }
//     return (BOOL)Default();                 // unconditional tail jump to 0x28ac80 (mfc140u)
// Symbol: ?OnSendIconicThumbnail@CMDITabProxyWnd@@IEAAHHH@Z
extern "C" int MS_ABI impl__OnSendIconicThumbnail_CMDITabProxyWnd__IEAAHHH_Z(void* pThis, int cx, int cy) {
    if (pThis == nullptr) return FALSE;
    if (CMDIChildWndEx* pChild = Related(pThis)) {
        HBITMAP hBmp = ChildOnGetIconicThumbnail(pChild, cx, cy);
        if (hBmp == nullptr) {
            hBmp = impl__GetClientBitmap_CMDITabProxyWnd__QEAAPEAUHBITMAP____HHH_Z(pThis, cx, cy, TRUE);
        }
        CallDwmSetIconicThumbnail(L(pThis)->m_hWnd, hBmp, 0);
        ::DeleteObject(hBmp);
    }
    return static_cast<int>(impl__Default_CWnd__IEAA_JXZ(W(pThis)));
}

// Retail entry RVA 0x873f0 (mfc140u; the WM_SYSCOMMAND entry of the message
// map), fully transcribed:
//     if (m_pRelatedMDIChildFrame == NULL) return;
//     CMDIFrameWndEx* pFrame = DYNAMIC_DOWNCAST(CMDIFrameWndEx,
//                                 m_pRelatedMDIChildFrame->GetTopLevelFrame());
//     if (nID == SC_CLOSE /*0xf060*/) { Default(); return; }          // 0x28ac80 (mfc140u)
//     if (nID != SC_MINIMIZE /*0xf020*/)
//         m_pRelatedMDIChildFrame->ActivateTopLevelFrame();           // slot +0x450
//     ::SendMessage(pFrame->m_hWnd, WM_SYSCOMMAND, nID, lParam);      // IAT 0x1802c7120 (mfc140u) = SendMessageW
// (wParam is the zero-extended UINT nID.)  The DYNAMIC_DOWNCAST runs before the
// SC_CLOSE test, as in retail; its result is only used by the SendMessage.
// DEVIATION: retail dereferences a NULL pFrame; here the SendMessage is skipped.
// Symbol: ?OnSysCommand@CMDITabProxyWnd@@IEAAXI_J@Z
extern "C" void MS_ABI impl__OnSysCommand_CMDITabProxyWnd__IEAAXI_J_Z(void* pThis, unsigned int nID, __int64 lParam) {
    if (pThis == nullptr) return;
    CMDIChildWndEx* pChild = Related(pThis);
    if (pChild == nullptr) return;
    CFrameWnd* pFrame = TopLevelMDIFrameEx(pChild);
    if (nID == SC_CLOSE) {
        impl__Default_CWnd__IEAA_JXZ(W(pThis));
        return;
    }
    if (nID != SC_MINIMIZE) {
        ChildActivateTopLevelFrame(pChild);
    }
    if (pFrame != nullptr) {
        ::SendMessage(pFrame->m_hWnd, WM_SYSCOMMAND, static_cast<WPARAM>(nID), static_cast<LPARAM>(lParam));
    }
}

// Retail entry RVA 0x86c40 (mfc140u; named at 0x871d0 in mfc140), whole body:
//     m_pRelatedMDIChildFrame = pRelatedMDIFrame;   // mov %rdx,0xe8(%rcx); ret
// Symbol: ?SetRelatedMDIChildFrame@CMDITabProxyWnd@@QEAAXPEAVCMDIChildWndEx@@@Z
extern "C" void MS_ABI impl__SetRelatedMDIChildFrame_CMDITabProxyWnd__QEAAXPEAVCMDIChildWndEx___Z(
    void* pThis, CMDIChildWndEx* pRelatedMDIFrame) {
    if (pThis == nullptr) return;   // deviation: retail has no NULL check
    L(pThis)->m_pRelatedMDIChildFrame = pRelatedMDIFrame;
}
