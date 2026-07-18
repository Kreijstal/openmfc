// Real CPngImage implementation (afxpngimage.h). Self-contained: the DLL is
// mingw-built but exports the MSVC-mangled CPngImage methods as extern "C"
// MS_ABI impl_ thunks that view the object through a layout-faithful struct.
//
// Layout harvested via cl.exe /d1reportSingleClassLayoutCPngImage (size 16):
//   0  {vfptr}            (CObject base vtable pointer)
//   8  m_hObject          (HGDIOBJ from CGdiObject base; the decoded HBITMAP)
// m_pImage is a static ATL::CImage* cache, not part of the instance layout.
//
// vtable slots (harvested order):
//   0 CBitmap::GetRuntimeClass  1 CPngImage::{dtor}(vector-deleting)
//   2 CObject::Serialize        3 CObject::AssertValid   4 CObject::Dump
//
// Load/LoadFromFile/LoadFromBuffer really decode the PNG: a PNG resource/file is
// wrapped in an IStream and handed to GDI+ (GdipCreateBitmapFromStream), and the
// resulting HBITMAP is attached to the CBitmap base (m_hObject), exactly as the
// real MFC CPngImage does through ATL::CImage. GDI+ is linked directly via the
// gdiplus import library (-lgdiplus), matching how ATL::CImage pulls in GDI+.
#include <windows.h>
#include <objbase.h>   // CreateStreamOnHGlobal, IStream
#include <cstring>
#include <new>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

namespace {

struct S {
    const void* vfptr;   // 0
    void*       m_hObject; // 8  (HBITMAP)
};
static_assert(sizeof(S) == 16, "CPngImage layout must match MSVC (size 16)");
static_assert(offsetof(S, m_hObject) == 8, "m_hObject@8");
static_assert(offsetof(S, vfptr) == 0, "vfptr@0");

// ------------------------------------------------------------------ GDI+ glue
// GDI+ flat-API surface, resolved at link time against the gdiplus import lib.
struct GdiplusStartupInput {
    UINT32 GdiplusVersion;
    void*  DebugEventCallback;
    BOOL   SuppressBackgroundThread;
    BOOL   SuppressExternalCodecs;
};

extern "C" {
int  WINAPI GdiplusStartup(ULONG_PTR*, const GdiplusStartupInput*, void*);
void WINAPI GdiplusShutdown(ULONG_PTR);
int  WINAPI GdipCreateBitmapFromStream(IStream*, void**);
int  WINAPI GdipCreateHBITMAPFromBitmap(void*, HBITMAP*, DWORD);
int  WINAPI GdipDisposeImage(void*);
}

// Decode an encoded image (PNG/BMP/...) from an IStream into an HBITMAP via GDI+.
static HBITMAP DecodeStreamToHBITMAP(IStream* pStream) {
    if (pStream == nullptr) return nullptr;

    HBITMAP hbmp = nullptr;
    GdiplusStartupInput input = { 1, nullptr, FALSE, FALSE };
    ULONG_PTR token = 0;
    if (GdiplusStartup(&token, &input, nullptr) == 0 /*Ok*/) {
        void* pGpBitmap = nullptr;
        if (GdipCreateBitmapFromStream(pStream, &pGpBitmap) == 0 /*Ok*/ && pGpBitmap) {
            // 0 background = fully transparent black behind alpha.
            if (GdipCreateHBITMAPFromBitmap(pGpBitmap, &hbmp, 0) != 0 /*!Ok*/) hbmp = nullptr;
            GdipDisposeImage(pGpBitmap);
        }
        GdiplusShutdown(token);
    }
    return hbmp;
}

// Wrap a raw encoded buffer in an IStream and decode it.
static HBITMAP DecodeBufferToHBITMAP(const BYTE* lpBuffer, UINT uiSize) {
    if (lpBuffer == nullptr || uiSize == 0) return nullptr;
    HGLOBAL hMem = ::GlobalAlloc(GMEM_MOVEABLE, uiSize);
    if (hMem == nullptr) return nullptr;
    void* pMem = ::GlobalLock(hMem);
    if (pMem == nullptr) { ::GlobalFree(hMem); return nullptr; }
    std::memcpy(pMem, lpBuffer, uiSize);
    ::GlobalUnlock(hMem);

    IStream* pStream = nullptr;
    HBITMAP hbmp = nullptr;
    if (::CreateStreamOnHGlobal(hMem, TRUE /*fDeleteOnRelease*/, &pStream) == S_OK && pStream) {
        hbmp = DecodeStreamToHBITMAP(pStream);
        pStream->Release();           // frees hMem (fDeleteOnRelease)
    } else {
        ::GlobalFree(hMem);
    }
    return hbmp;
}

// Attach a freshly decoded bitmap to the object, releasing any prior one.
static BOOL AttachDecoded(S* p, HBITMAP hbmp) {
    if (hbmp == nullptr) return FALSE;
    if (p->m_hObject != nullptr) { ::DeleteObject((HGDIOBJ)p->m_hObject); }
    p->m_hObject = (void*)hbmp;
    return TRUE;
}

// ------------------------------------------------------------------ vtable
// Slot 0 (CBitmap::GetRuntimeClass) reuses the DLL's CBitmap RTTI thunk.
extern "C" void* MS_ABI impl__GetRuntimeClass_CBitmap__UEBAPEAUCRuntimeClass__XZ(const void*);

// CObject base virtuals: faithful no-op bodies (CObject::Serialize/AssertValid/
// Dump do nothing in the base class).
void  MS_ABI vt_Serialize(void* /*pThis*/, void* /*pArchive*/) {}
void  MS_ABI vt_AssertValid(const void* /*pThis*/) {}
void  MS_ABI vt_Dump(const void* /*pThis*/, void* /*pDumpContext*/) {}

// Forward declaration so the deleting-dtor slot can run the real destructor body.
} // namespace
extern "C" void MS_ABI impl___1CPngImage__UEAA_XZ(void* pThis);
namespace {

// Slot 1: vector deleting destructor = run ~CPngImage, then free if requested.
void* MS_ABI v_dtor(void* p, unsigned int flags) {
    impl___1CPngImage__UEAA_XZ(p);
    if (flags & 1) ::operator delete(p);
    return p;
}

void* const g_CPngImage_vtbl[5] = {
    (void*)impl__GetRuntimeClass_CBitmap__UEBAPEAUCRuntimeClass__XZ,
    (void*)v_dtor,
    (void*)vt_Serialize,
    (void*)vt_AssertValid,
    (void*)vt_Dump,
};

} // namespace

// ============================== exported thunks ==============================
extern "C" {

// Symbol: ??0CPngImage@@QEAA@XZ
void* MS_ABI impl___0CPngImage__QEAA_XZ(void* pThis) {
    S* p = (S*)pThis;
    p->vfptr = &g_CPngImage_vtbl[0];
    p->m_hObject = nullptr;
    return pThis;
}

// Symbol: ??1CPngImage@@UEAA@XZ
void MS_ABI impl___1CPngImage__UEAA_XZ(void* pThis) {
    S* p = (S*)pThis;
    if (p->m_hObject != nullptr) {
        ::DeleteObject((HGDIOBJ)p->m_hObject);
        p->m_hObject = nullptr;
    }
}

// Symbol: ?Load@CPngImage@@QEAAHPEB_WPEAUHINSTANCE__@@@Z
// BOOL CPngImage::Load(LPCTSTR lpszResourceName, HINSTANCE hinstRes)
int MS_ABI impl__Load_CPngImage__QEAAHPEB_WPEAUHINSTANCE_____Z(
        void* pThis, const wchar_t* lpszResourceName, HINSTANCE hinstRes) {
    S* p = (S*)pThis;
    if (lpszResourceName == nullptr) return FALSE;
    HINSTANCE hInst = hinstRes ? hinstRes : ::GetModuleHandleW(nullptr);

    // PNGs are stored under a custom "PNG" resource type in MFC apps.
    HRSRC hRsrc = ::FindResourceW(hInst, lpszResourceName, L"PNG");
    if (hRsrc == nullptr) return FALSE;
    DWORD dwSize = ::SizeofResource(hInst, hRsrc);
    HGLOBAL hGlobal = ::LoadResource(hInst, hRsrc);
    if (hGlobal == nullptr || dwSize == 0) return FALSE;
    const BYTE* lpData = (const BYTE*)::LockResource(hGlobal);
    if (lpData == nullptr) return FALSE;

    return AttachDecoded(p, DecodeBufferToHBITMAP(lpData, (UINT)dwSize)) ? TRUE : FALSE;
}

// Symbol: ?Load@CPngImage@@QEAAHIPEAUHINSTANCE__@@@Z
// BOOL CPngImage::Load(UINT uiResID, HINSTANCE hinstRes)  -> forwards by name.
int MS_ABI impl__Load_CPngImage__QEAAHIPEAUHINSTANCE_____Z(
        void* pThis, unsigned int uiResID, HINSTANCE hinstRes) {
    return impl__Load_CPngImage__QEAAHPEB_WPEAUHINSTANCE_____Z(
        pThis, MAKEINTRESOURCEW(uiResID), hinstRes);
}

// Symbol: ?LoadFromFile@CPngImage@@QEAAHPEB_W@Z
// BOOL CPngImage::LoadFromFile(LPCTSTR lpszPath)
int MS_ABI impl__LoadFromFile_CPngImage__QEAAHPEB_W_Z(
        void* pThis, const wchar_t* lpszPath) {
    S* p = (S*)pThis;
    if (lpszPath == nullptr) return FALSE;

    HANDLE hFile = ::CreateFileW(lpszPath, GENERIC_READ, FILE_SHARE_READ, nullptr,
                                 OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
    if (hFile == INVALID_HANDLE_VALUE) return FALSE;

    BOOL bResult = FALSE;
    LARGE_INTEGER li;
    if (::GetFileSizeEx(hFile, &li) && li.QuadPart > 0 && li.QuadPart <= 0x7FFFFFFF) {
        DWORD dwSize = (DWORD)li.QuadPart;
        BYTE* pBuf = (BYTE*)::malloc(dwSize);
        if (pBuf != nullptr) {
            DWORD dwRead = 0;
            if (::ReadFile(hFile, pBuf, dwSize, &dwRead, nullptr) && dwRead == dwSize) {
                bResult = AttachDecoded(p, DecodeBufferToHBITMAP(pBuf, dwSize)) ? TRUE : FALSE;
            }
            ::free(pBuf);
        }
    }
    ::CloseHandle(hFile);
    return bResult;
}

// Symbol: ?LoadFromBuffer@CPngImage@@QEAAHPEAEI@Z
// BOOL CPngImage::LoadFromBuffer(LPBYTE lpBuffer, UINT uiSize)
int MS_ABI impl__LoadFromBuffer_CPngImage__QEAAHPEAEI_Z(
        void* pThis, BYTE* lpBuffer, unsigned int uiSize) {
    S* p = (S*)pThis;
    return AttachDecoded(p, DecodeBufferToHBITMAP(lpBuffer, uiSize)) ? TRUE : FALSE;
}

} // extern "C"
