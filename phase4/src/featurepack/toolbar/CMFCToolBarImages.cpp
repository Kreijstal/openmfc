// CMFCToolBarImages — OpenMFC implementation.
// Sources: gdicore.cpp, global_mfc-feature-9_impl.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/GdicoreSupport.h"
#include "detail/CMFCToolBarImagesSupport.h"

#include <atomic>
#include <climits>
#include <memory>
#include <string>
#include <type_traits>

#define InitImagesMembers() InitMembers()
// Symbol: ??0CMFCToolBarImages@@QEAA@XZ
CMFCToolBarImages::CMFCToolBarImages() {
    InitImagesMembers();
}
// Symbol: ??0CMFCToolBarImages@@QEAA@H@Z
CMFCToolBarImages::CMFCToolBarImages(BOOL bReadOnly) {
    InitImagesMembers();
    m_bReadOnly = bReadOnly;
}
#undef InitImagesMembers
// Symbol: ??1CMFCToolBarImages@@UEAA@XZ
CMFCToolBarImages::~CMFCToolBarImages() {
    ReleaseImageWell();
    g_toolbarResourceOffsets.erase(this);
}
namespace {
extern "C" HINSTANCE MS_ABI impl__AfxFindResourceHandle__YAPEAUHINSTANCE____PEB_W0_Z(const wchar_t*, const wchar_t*);
extern "C" BOOL MS_ABI impl__Load_CPngImage__QEAAHPEB_WPEAUHINSTANCE_____Z(void*, const wchar_t*, HINSTANCE);
extern "C" BOOL MS_ABI impl__PreMultiplyAlpha_CMFCToolBarImages__SAHPEAUHBITMAP____H_Z(HBITMAP, int);

using OwnedBitmap = std::unique_ptr<std::remove_pointer_t<HBITMAP>, decltype(&::DeleteObject)>;
using MemoryDC = std::unique_ptr<std::remove_pointer_t<HDC>, decltype(&::DeleteDC)>;

OwnedBitmap LoadImageResource(const wchar_t* name, HINSTANCE module, BOOL autoCheckAlpha) {
    if (!name) return {nullptr, &::DeleteObject};
    const auto pngModule = module ? module :
        impl__AfxFindResourceHandle__YAPEAUHINSTANCE____PEB_W0_Z(name, L"PNG");
    CBitmap png;
    if (pngModule && impl__Load_CPngImage__QEAAHPEB_WPEAUHINSTANCE_____Z(&png, name, pngModule))
        return {static_cast<HBITMAP>(png.Detach()), &::DeleteObject};

    const auto bitmapModule = module ? module :
        impl__AfxFindResourceHandle__YAPEAUHINSTANCE____PEB_W0_Z(name, RT_BITMAP);
    OwnedBitmap bitmap(static_cast<HBITMAP>(::LoadImageW(bitmapModule, name, IMAGE_BITMAP,
                                                        0, 0, LR_CREATEDIBSECTION)), &::DeleteObject);
    if (bitmap)
        impl__PreMultiplyAlpha_CMFCToolBarImages__SAHPEAUHBITMAP____H_Z(bitmap.get(), autoCheckAlpha);
    return bitmap;
}

OwnedBitmap CreateImageBitmap(int width, int height, WORD bits, void** pixels) {
    BITMAPINFO info{};
    info.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    info.bmiHeader.biWidth = width;
    info.bmiHeader.biHeight = -height;
    info.bmiHeader.biPlanes = 1;
    info.bmiHeader.biBitCount = bits;
    return {::CreateDIBSection(nullptr, &info, DIB_RGB_COLORS, pixels, nullptr, 0), &::DeleteObject};
}

DWORD DibColor(COLORREF color) {
    return (DWORD(GetRValue(color)) << 16) | (DWORD(GetGValue(color)) << 8) | GetBValue(color);
}

void ApplyColorKey(DWORD* pixels, int stride, int left, int width, int height, COLORREF key) {
    ::GdiFlush();
    const DWORD transparent = DibColor(key);
    for (int y = 0; y < height; ++y) {
        auto* row = pixels + static_cast<size_t>(y) * stride + left;
        for (int x = 0; x < width; ++x) {
            const DWORD color = row[x] & 0xffffff;
            row[x] = key != CLR_INVALID && color == transparent ? 0 : color | 0xff000000;
        }
    }
}

OwnedBitmap JoinStrips(HBITMAP first, const BITMAP& firstInfo, HBITMAP last, const BITMAP& lastInfo,
                       COLORREF key) {
    OwnedBitmap result(nullptr, &::DeleteObject);
    if (firstInfo.bmWidth > INT_MAX - lastInfo.bmWidth || firstInfo.bmHeight != lastInfo.bmHeight)
        return result;
    const bool firstHasAlpha = firstInfo.bmBitsPixel == 32;
    const bool lastHasAlpha = lastInfo.bmBitsPixel == 32;
    const bool alpha = firstHasAlpha || lastHasAlpha;
    const int width = firstInfo.bmWidth + lastInfo.bmWidth;
    const int height = firstInfo.bmHeight;
    void* pixels = nullptr;
    result = CreateImageBitmap(width, height, alpha ? 32 : 24, &pixels);
    if (!result) return result;

    MemoryDC src(::CreateCompatibleDC(nullptr), &::DeleteDC);
    MemoryDC dst(::CreateCompatibleDC(nullptr), &::DeleteDC);
    if (!src || !dst) return {nullptr, &::DeleteObject};
    const auto oldDst = ::SelectObject(dst.get(), result.get());
    const bool copied = oldDst && ::SelectObject(src.get(), first) &&
        ::BitBlt(dst.get(), 0, 0, firstInfo.bmWidth, height, src.get(), 0, 0, SRCCOPY) &&
        ::SelectObject(src.get(), last) &&
        ::BitBlt(dst.get(), firstInfo.bmWidth, 0, lastInfo.bmWidth, height, src.get(), 0, 0, SRCCOPY);
    if (oldDst) ::SelectObject(dst.get(), oldDst);
    if (!copied) return {nullptr, &::DeleteObject};

    if (alpha && !firstHasAlpha)
        ApplyColorKey(static_cast<DWORD*>(pixels), width, 0, firstInfo.bmWidth, height, key);
    if (alpha && !lastHasAlpha)
        ApplyColorKey(static_cast<DWORD*>(pixels), width, firstInfo.bmWidth, lastInfo.bmWidth, height, key);
    return result;
}

OwnedBitmap CopyCell(HBITMAP well, CSize size, int index, bool useAlpha, bool applyKey, COLORREF key) {
    void* pixels = nullptr;
    auto image = CreateImageBitmap(size.cx, size.cy, useAlpha ? 32 : 24, &pixels);
    if (!image) return image;

    const MemoryDC src(::CreateCompatibleDC(nullptr), &::DeleteDC);
    const MemoryDC dst(::CreateCompatibleDC(nullptr), &::DeleteDC);
    if (!src || !dst) return {nullptr, &::DeleteObject};

    const auto old = ::SelectObject(dst.get(), image.get());
    const bool copied = old && ::SelectObject(src.get(), well) &&
        ::BitBlt(dst.get(), 0, 0, size.cx, size.cy, src.get(), index * size.cx, 0, SRCCOPY);
    if (old) ::SelectObject(dst.get(), old);
    if (!copied) return {nullptr, &::DeleteObject};
    if (applyKey) ApplyColorKey(static_cast<DWORD*>(pixels), size.cx, 0, size.cx, size.cy, key);
    return image;
}

OwnedBitmap RasterizeIcon(HICON icon, CSize size, bool alpha, COLORREF key) {
    if (size.cx <= 0 || size.cy <= 0 || size.cx > INT_MAX / 2) return {nullptr, &::DeleteObject};
    // The difference between the white and black composites recovers the
    // opacity of both per-pixel alpha and monochrome-mask icons.
    const int scratchWidth = alpha ? size.cx * 2 : size.cx;
    void* pixels = nullptr;
    auto scratch = CreateImageBitmap(scratchWidth, size.cy, alpha ? 32 : 24, &pixels);
    const MemoryDC canvas(::CreateCompatibleDC(nullptr), &::DeleteDC);
    if (!scratch || !canvas) return {nullptr, &::DeleteObject};

    const auto old = ::SelectObject(canvas.get(), scratch.get());
    if (!old) return {nullptr, &::DeleteObject};
    BOOL drawn;
    if (alpha) {
        drawn = ::PatBlt(canvas.get(), 0, 0, size.cx, size.cy, BLACKNESS) &&
            ::PatBlt(canvas.get(), size.cx, 0, size.cx, size.cy, WHITENESS) &&
            ::DrawIconEx(canvas.get(), 0, 0, icon, size.cx, size.cy, 0, nullptr, DI_NORMAL) &&
            ::DrawIconEx(canvas.get(), size.cx, 0, icon, size.cx, size.cy, 0, nullptr, DI_NORMAL);
    } else {
        const RECT bounds{0, 0, size.cx, size.cy};
        const auto brush = ::CreateSolidBrush(key == CLR_INVALID ? ::GetSysColor(COLOR_BTNFACE) : key);
        drawn = brush && ::FillRect(canvas.get(), &bounds, brush) &&
            (!icon || ::DrawIconEx(canvas.get(), 0, 0, icon, size.cx, size.cy, 0, nullptr, DI_NORMAL));
        if (brush) ::DeleteObject(brush);
    }
    ::SelectObject(canvas.get(), old);
    if (!drawn) return {nullptr, &::DeleteObject};
    if (!alpha) return scratch;

    void* output = nullptr;
    auto image = CreateImageBitmap(size.cx, size.cy, 32, &output);
    if (!image) return image;

    ::GdiFlush();
    const auto* input = static_cast<const DWORD*>(pixels);
    auto* result = static_cast<DWORD*>(output);
    for (int y = 0; y < size.cy; ++y) {
        const auto* black = input + static_cast<size_t>(y) * scratchWidth;
        const auto* white = black + size.cx;
        for (int x = 0; x < size.cx; ++x) {
            const int difference = static_cast<int>(white[x] & 0xff) - static_cast<int>(black[x] & 0xff);
            const DWORD opacity = 255 - (std::max)(0, difference);
            *result++ = (black[x] & 0xffffff) | (opacity << 24);
        }
    }
    return image;
}
bool WriteFileBytes(HANDLE file, const void* bytes, DWORD count) {
    DWORD written = 0;
    return ::WriteFile(file, bytes, count, &written, nullptr) && written == count;
}

void UnpremultiplyPixels(BYTE* pixels, size_t bytes) {
    for (size_t i = 0; i < bytes; i += 4) {
        const unsigned int alpha = pixels[i + 3];
        if (alpha == 0) continue;
        for (size_t channel = i; channel < i + 3; ++channel) {
            const unsigned int straight = (pixels[channel] * 255u + alpha - 1) / alpha;
            pixels[channel] = static_cast<BYTE>((std::min)(straight, 255u));
        }
    }
}

BOOL SaveImageBitmap(HBITMAP bitmap, const wchar_t* path) {
    BITMAP image{};
    if (!::GetObjectW(bitmap, sizeof(image), &image) || image.bmWidth <= 0 || image.bmHeight <= 0) return FALSE;
    const WORD depth = image.bmBitsPixel * image.bmPlanes;
    const DWORD colors = depth <= 8 ? 1u << depth : 0;
    constexpr DWORD header = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
    const DWORD pixelsOffset = header + colors * sizeof(RGBQUAD);
    const uint64_t stride = (uint64_t(image.bmWidth) * depth + 31) / 32 * 4;
    const uint64_t size = stride * image.bmHeight;
    if (!stride || size > MAXDWORD - pixelsOffset) return FALSE;

    struct BitmapInfoWithPalette {
        BITMAPINFOHEADER header;
        RGBQUAD palette[256];
    } info{};
    info.header.biSize = sizeof(info.header);
    info.header.biWidth = image.bmWidth;
    info.header.biHeight = image.bmHeight;
    info.header.biPlanes = 1;
    info.header.biBitCount = depth;
    info.header.biSizeImage = static_cast<DWORD>(size);
    std::vector<BYTE> pixels(static_cast<size_t>(size));
    const MemoryDC dc(::CreateCompatibleDC(nullptr), &::DeleteDC);
    if (!dc || ::GetDIBits(dc.get(), bitmap, 0, image.bmHeight, pixels.data(),
                         reinterpret_cast<BITMAPINFO*>(&info), DIB_RGB_COLORS) != image.bmHeight)
        return FALSE;

    // The well holds premultiplied alpha. Load premultiplies file pixels.
    if (depth == 32) UnpremultiplyPixels(pixels.data(), pixels.size());

    BITMAPFILEHEADER fileHeader{};
    fileHeader.bfType = 0x4d42;
    fileHeader.bfSize = pixelsOffset + static_cast<DWORD>(size);
    fileHeader.bfOffBits = pixelsOffset;
    const DWORD pathCapacity = ::GetFullPathNameW(path, 0, nullptr, nullptr);
    if (!pathCapacity) return FALSE;
    std::wstring target(pathCapacity, L'\0');
    const DWORD pathLength = ::GetFullPathNameW(path, pathCapacity, target.data(), nullptr);
    if (!pathLength || pathLength >= pathCapacity) return FALSE;
    target.resize(pathLength);
    const auto separator = target.find_last_of(L"\\/");
    const std::wstring directory = separator == std::wstring::npos ? L"" : target.substr(0, separator + 1);
    static std::atomic<unsigned long> sequence{};
    std::wstring temporary;
    HANDLE file = INVALID_HANDLE_VALUE;
    for (int tries = 0; tries < 16; ++tries) {
        temporary = directory + L"openmfc-" + std::to_wstring(::GetCurrentProcessId()) +
            L"-" + std::to_wstring(++sequence) + L".tmp";
        file = ::CreateFileW(temporary.c_str(), GENERIC_WRITE, 0, nullptr, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, nullptr);
        if (file != INVALID_HANDLE_VALUE) break;
        const DWORD error = ::GetLastError();
        if (error != ERROR_FILE_EXISTS && error != ERROR_ALREADY_EXISTS) return FALSE;
    }
    if (file == INVALID_HANDLE_VALUE) return FALSE;
    const bool complete = WriteFileBytes(file, &fileHeader, sizeof(fileHeader)) &&
        WriteFileBytes(file, &info, sizeof(info.header) + colors * sizeof(RGBQUAD)) &&
        WriteFileBytes(file, pixels.data(), static_cast<DWORD>(size)) &&
        ::FlushFileBuffers(file);
    ::CloseHandle(file);
    if (complete && ::MoveFileExW(temporary.c_str(), target.c_str(), MOVEFILE_REPLACE_EXISTING | MOVEFILE_WRITE_THROUGH))
        return TRUE;
    ::DeleteFileW(temporary.c_str());
    return FALSE;
}
} // namespace

void CMFCToolBarImages::ReleaseImageWell() {
    if (!m_bIsTemporary) {
        if (m_hbmImageWell) ::DeleteObject(m_hbmImageWell);
        if (m_hbmImageLight) ::DeleteObject(m_hbmImageLight);
        if (m_hbmImageShadow) ::DeleteObject(m_hbmImageShadow);
    }
    m_hbmImageWell = m_hbmImageLight = m_hbmImageShadow = nullptr;
    m_iCount = 0;
    m_nBitsPerPixel = 0;
}

int CMFCToolBarImages::StoreImageStrip(HBITMAP bitmap, BOOL append, const CSize* cellSize) {
    BITMAP info{};
    if (m_bIsTemporary || !bitmap || !::GetObjectW(bitmap, sizeof(info), &info) ||
        info.bmWidth <= 0 || info.bmHeight <= 0) return -1;

    // Resource strips are a single row of cells. When no size has been set,
    // keep the default cell width for a strip and accept a custom single icon.
    CSize size = cellSize ? *cellSize : m_sizeImage;
    if (!cellSize && !m_hbmImageWell && size.cx == 16 && size.cy == 15 && info.bmHeight != size.cy) {
        size = CSize(info.bmWidth < size.cx ? info.bmWidth : size.cx, info.bmHeight);
    }
    if (size.cx <= 0 || size.cy <= 0 || info.bmWidth < size.cx || info.bmHeight != size.cy) return -1;

    OwnedBitmap stored(nullptr, &::DeleteObject);
    int first = 0;
    if (append && m_hbmImageWell) {
        BITMAP oldInfo{};
        if (!::GetObjectW(m_hbmImageWell, sizeof(oldInfo), &oldInfo) ||
            oldInfo.bmWidth % size.cx != 0 || info.bmWidth % size.cx != 0) return -1;
        first = oldInfo.bmWidth / size.cx;
        stored = JoinStrips(m_hbmImageWell, oldInfo, bitmap, info, m_clrTransparent);
    } else {
        stored.reset(static_cast<HBITMAP>(::CopyImage(bitmap, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION)));
    }
    BITMAP storedInfo{};
    if (!stored || !::GetObjectW(stored.get(), sizeof(storedInfo), &storedInfo)) return -1;
    ReleaseImageWell();
    m_hbmImageWell = stored.release();
    m_sizeImage = size;
    m_iCount = first + info.bmWidth / size.cx;
    m_nBitsPerPixel = storedInfo.bmBitsPixel;
    return first;
}

// Symbol: ?Load@CMFCToolBarImages@@QEAAHIPEAUHINSTANCE__@@H@Z
BOOL CMFCToolBarImages::Load(UINT nIDResource, HINSTANCE hInstRes, BOOL bAdd) {
    return nIDResource && nIDResource <= 0xffff && LoadStr(MAKEINTRESOURCEW(nIDResource), hInstRes, bAdd);
}

// Symbol: ?Load@CMFCToolBarImages@@QEAAHPEB_WK@Z
BOOL CMFCToolBarImages::Load(const wchar_t* file, DWORD maxFileSize) {
    if (m_bIsTemporary || !file || !*file) return FALSE;
    WIN32_FILE_ATTRIBUTE_DATA attributes{};
    if (!::GetFileAttributesExW(file, GetFileExInfoStandard, &attributes) ||
        (attributes.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) ||
        (maxFileSize && (attributes.nFileSizeHigh || attributes.nFileSizeLow > maxFileSize))) return FALSE;
    const OwnedBitmap bitmap(static_cast<HBITMAP>(::LoadImageW(nullptr, file, IMAGE_BITMAP, 0, 0,
                                                            LR_LOADFROMFILE | LR_CREATEDIBSECTION)), &::DeleteObject);
    if (!bitmap || StoreImageStrip(bitmap.get(), FALSE) < 0) return FALSE;
    impl__PreMultiplyAlpha_CMFCToolBarImages__SAHPEAUHBITMAP____H_Z(m_hbmImageWell, m_bAutoCheckPremlt);
    g_toolbarResourceOffsets.erase(this);
    m_bUserImagesList = TRUE;
    m_bModified = FALSE;
    m_strUDLPath = file;
    return TRUE;
}

// Symbol: ?LoadStr@CMFCToolBarImages@@QEAAHPEB_WPEAUHINSTANCE__@@H@Z
BOOL CMFCToolBarImages::LoadStr(const wchar_t* name, HINSTANCE module, BOOL append) {
    if (m_bIsTemporary || !name) return FALSE;
    const UINT id = IS_INTRESOURCE(name) ? static_cast<UINT>(reinterpret_cast<UINT_PTR>(name)) : 0;
    if (append && id && GetResourceOffset(id) >= 0) return TRUE;

    const auto bitmap = LoadImageResource(name, module, m_bAutoCheckPremlt);
    const int first = StoreImageStrip(bitmap.get(), append);
    if (first < 0) return FALSE;
    if (!append) g_toolbarResourceOffsets.erase(this);
    if (id) g_toolbarResourceOffsets[this][id] = first;
    m_bUserImagesList = FALSE;
    m_bModified = FALSE;
    m_strUDLPath.Empty();
    return TRUE;
}
// Symbol: ?Save@CMFCToolBarImages@@QEAAHPEB_W@Z
BOOL CMFCToolBarImages::Save(const wchar_t* file) {
    if (!m_bUserImagesList || m_bReadOnly || m_bIsTemporary || !m_hbmImageWell) return FALSE;
    const auto* original = static_cast<const wchar_t*>(m_strUDLPath);
    const auto* path = file ? file : original;
    if (!path || !*path) return FALSE;
    if (!m_bModified && original && std::wcscmp(path, original) == 0) return TRUE;
    if (!SaveImageBitmap(m_hbmImageWell, path)) return FALSE;
    m_bModified = FALSE;
    return TRUE;
}
// Symbol: ?AddImage@CMFCToolBarImages@@QEAAHPEAUHBITMAP__@@H@Z
int CMFCToolBarImages::AddImage(HBITMAP hBitmap, BOOL) {
    const int first = StoreImageStrip(hBitmap, TRUE);
    if (first >= 0 && m_bUserImagesList) m_bModified = TRUE;
    return first;
}
// Symbol: ?AddImage@CMFCToolBarImages@@QEAAHAEBV1@H@Z
int CMFCToolBarImages::AddImage(const CMFCToolBarImages& images, int sourceIndex) {
    const CSize size = images.m_sizeImage;
    if (m_bIsTemporary || !images.m_hbmImageWell || sourceIndex < 0 ||
        sourceIndex >= images.m_iCount || size.cx <= 0 || size.cy <= 0) return -1;
    if (m_hbmImageWell && (m_sizeImage.cx != size.cx || m_sizeImage.cy != size.cy)) return -1;

    const bool wasEmpty = !m_hbmImageWell;
    const bool sourceAlpha = images.m_nBitsPerPixel == 32;
    const bool convertKey = !wasEmpty && !sourceAlpha && m_clrTransparent != images.m_clrTransparent;
    const auto cell = CopyCell(images.m_hbmImageWell, size, sourceIndex, sourceAlpha || convertKey,
                               convertKey, images.m_clrTransparent);
    const int index = StoreImageStrip(cell.get(), TRUE, &size);
    if (index < 0) return -1;
    if (wasEmpty) {
        m_clrTransparent = images.m_clrTransparent;
        m_clrImageShadow = images.m_clrImageShadow;
    }
    if (m_bUserImagesList) m_bModified = TRUE;
    return index;
}
// Symbol: ?AddIcon@CMFCToolBarImages@@QEAAHPEAUHICON__@@H@Z
int CMFCToolBarImages::AddIcon(HICON icon, BOOL alphaBlend) {
    if (m_bIsTemporary) return -1;
    const auto bitmap = RasterizeIcon(icon, m_sizeImage, icon && alphaBlend, m_clrTransparent);
    return AddImage(bitmap.get(), TRUE);
}
// Symbol: ?Clear@CMFCToolBarImages@@QEAAXXZ
void CMFCToolBarImages::Clear() {
    ReleaseImageWell();
    m_strUDLPath.Empty();
    InitMembers();
    g_toolbarResourceOffsets.erase(this);
}
// Symbol: ?Initialize@CMFCToolBarImages@@QEAAXXZ
void CMFCToolBarImages::Initialize() {
    CommonInit(TRUE);
}
// Symbol: ?CommonInit@CMFCToolBarImages@@QEAAXH@Z
void CMFCToolBarImages::CommonInit(BOOL) {
    m_bIsRTL = FALSE;                 // statics: reset the shared defaults
    m_nDisabledImageAlpha = 127;
    m_nFadedImageAlpha = 127;
    ReleaseImageWell();
    m_strUDLPath.Empty();
    InitMembers();
    g_toolbarResourceOffsets.erase(this);
}
// Symbol: ?CleanUp@CMFCToolBarImages@@SAXXZ
// (static)
void CMFCToolBarImages__CleanUp() {
    g_toolbarResourceOffsets.clear();
}
// Symbol: ??0CMFCToolBarImages@@QEAA@XZ
extern "C" void* MS_ABI impl___0CMFCToolBarImages__QEAA_XZ(void* pThis) {
    return new(pThis) CMFCToolBarImages();
}
// Symbol: ??0CMFCToolBarImages@@QEAA@H@Z
extern "C" void* MS_ABI impl___0CMFCToolBarImages__QEAA_H_Z(void* pThis, int p0) {
    return new(pThis) CMFCToolBarImages((BOOL)p0);
}
// Symbol: ??1CMFCToolBarImages@@UEAA@XZ
extern "C" void MS_ABI impl___1CMFCToolBarImages__UEAA_XZ(void* pThis) {
    ((CMFCToolBarImages*)pThis)->~CMFCToolBarImages();
}
// Symbol: ?Load@CMFCToolBarImages@@QEAAHIPEAUHINSTANCE__@@H@Z
extern "C" int MS_ABI impl__Load_CMFCToolBarImages__QEAAHIPEAUHINSTANCE____H_Z(CMFCToolBarImages* pThis, unsigned int nID, HINSTANCE hInst, int bAdd) {
    return pThis->Load(nID, hInst, (BOOL)bAdd);
}
// Symbol: ?Load@CMFCToolBarImages@@QEAAHPEB_WK@Z
extern "C" int MS_ABI impl__Load_CMFCToolBarImages__QEAAHPEB_WK_Z(CMFCToolBarImages* pThis, const wchar_t* lpszFile, unsigned long dwMax) {
    return pThis->Load(lpszFile, dwMax);
}
// Symbol: ?LoadStr@CMFCToolBarImages@@QEAAHPEB_WPEAUHINSTANCE__@@H@Z
extern "C" int MS_ABI impl__LoadStr_CMFCToolBarImages__QEAAHPEB_WPEAUHINSTANCE____H_Z(CMFCToolBarImages* pThis, const wchar_t* lpszName, HINSTANCE hInst, int bAdd) {
    return pThis->LoadStr(lpszName, hInst, (BOOL)bAdd);
}
// Symbol: ?Save@CMFCToolBarImages@@QEAAHPEB_W@Z
extern "C" int MS_ABI impl__Save_CMFCToolBarImages__QEAAHPEB_W_Z(CMFCToolBarImages* pThis, const wchar_t* lpszFile) {
    return pThis && pThis->Save(lpszFile);
}
// Symbol: ?AddImage@CMFCToolBarImages@@QEAAHPEAUHBITMAP__@@H@Z
extern "C" int MS_ABI impl__AddImage_CMFCToolBarImages__QEAAHPEAUHBITMAP____H_Z(CMFCToolBarImages* pThis, HBITMAP hBmp, int bSet) {
    return pThis->AddImage(hBmp, (BOOL)bSet);
}
// Symbol: ?AddImage@CMFCToolBarImages@@QEAAHAEBV1@H@Z
extern "C" int MS_ABI impl__AddImage_CMFCToolBarImages__QEAAHAEBV1_H_Z(CMFCToolBarImages* pThis, const CMFCToolBarImages* pImages, int index) {
    return pThis && pImages ? pThis->AddImage(*pImages, index) : -1;
}
// Symbol: ?AddIcon@CMFCToolBarImages@@QEAAHPEAUHICON__@@H@Z
extern "C" int MS_ABI impl__AddIcon_CMFCToolBarImages__QEAAHPEAUHICON____H_Z(CMFCToolBarImages* pThis, HICON hIcon, int alphaBlend) {
    return pThis ? pThis->AddIcon(hIcon, alphaBlend) : -1;
}
// Symbol: ?Clear@CMFCToolBarImages@@QEAAXXZ
extern "C" void MS_ABI impl__Clear_CMFCToolBarImages__QEAAXXZ(CMFCToolBarImages* pThis) {
    pThis->Clear();
}
// Symbol: ?Initialize@CMFCToolBarImages@@QEAAXXZ
extern "C" void MS_ABI impl__Initialize_CMFCToolBarImages__QEAAXXZ(CMFCToolBarImages* pThis) {
    pThis->Initialize();
}
// Symbol: ?CommonInit@CMFCToolBarImages@@QEAAXH@Z
extern "C" void MS_ABI impl__CommonInit_CMFCToolBarImages__QEAAXH_Z(CMFCToolBarImages* pThis, int bFree) {
    pThis->CommonInit((BOOL)bFree);
}
// Symbol: ?CleanUp@CMFCToolBarImages@@SAXXZ
extern "C" void MS_ABI impl__CleanUp_CMFCToolBarImages__SAXXZ() {
    CMFCToolBarImages__CleanUp();
}
BOOL CMFCToolBarImages::m_bDisableTrueColorAlpha = FALSE;
BOOL CMFCToolBarImages::m_bIsDrawOnGlass = FALSE;
BOOL CMFCToolBarImages::m_bMultiThreaded = FALSE;
BOOL CMFCToolBarImages::m_bIsRTL = FALSE;
BYTE CMFCToolBarImages::m_nDisabledImageAlpha = 127;
BYTE CMFCToolBarImages::m_nFadedImageAlpha = 127;
void CMFCToolBarImages::InitMembers() {
    CMFCToolBarImages* const p = this;
    std::memset(reinterpret_cast<char*>(p) + sizeof(CObject), 0,
                sizeof(CMFCToolBarImages) - sizeof(CObject));
    // The memset above spans three members that are real C++ objects, so their
    // representations -- including the vfptrs of the two polymorphic ones --
    // must be re-established. Without this, the first virtual call on m_dcMem
    // or m_bmpMem (destruction included) dispatches through a null vptr.
    new (&p->m_dcMem) CDC();
    new (&p->m_strUDLPath) CString();
    new (&p->m_bmpMem) CBitmap();
    p->m_nBitsPerPixel = 0;
    p->m_nGrayImageLuminancePercentage = 0;
    p->m_nLightPercentage = 0;
    p->m_sizeImage = CSize(16, 15);
    p->m_dblScale = 1.0;
    p->m_clrTransparent = RGB(192, 192, 192);
    p->m_clrTransparentOriginal = p->m_clrTransparent;
    p->m_clrImageShadow = RGB(128, 128, 128);
}
int CMFCToolBarImages::GetCount() const {
    return m_iCount;
}
BOOL CMFCToolBarImages::IsValid() const {
    return m_hbmImageWell != nullptr;
}

int CMFCToolBarImages::GetResourceOffset(UINT id) const {
    const auto images = g_toolbarResourceOffsets.find(this);
    if (images == g_toolbarResourceOffsets.end()) return -1;
    const auto resource = images->second.find(id);
    return resource != images->second.end() ? resource->second : -1;
}

BOOL CMFCToolBarImages::CopyTo(CMFCToolBarImages& destination) const {
    if (destination.m_bIsTemporary) return FALSE;
    if (this == &destination) return TRUE;

    OwnedBitmap well(nullptr, &::DeleteObject);
    if (m_hbmImageWell) {
        well.reset(static_cast<HBITMAP>(::CopyImage(m_hbmImageWell, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION)));
        if (!well) return FALSE;
    }
    const auto offsets = g_toolbarResourceOffsets.find(this);
    if (offsets != g_toolbarResourceOffsets.end()) {
        auto copy = offsets->second;
        g_toolbarResourceOffsets[&destination] = std::move(copy);
    } else {
        g_toolbarResourceOffsets.erase(&destination);
    }

    destination.ReleaseImageWell();
    destination.m_hbmImageWell = well.release();
    destination.m_iCount = m_iCount;
    destination.m_nBitsPerPixel = m_nBitsPerPixel;
    destination.m_bUserImagesList = m_bUserImagesList;
    destination.m_bModified = m_bModified;
    destination.m_bReadOnly = m_bReadOnly;
    destination.m_bFadeInactive = m_bFadeInactive;
    destination.m_sizeImage = m_sizeImage;
    destination.m_sizeImageOriginal = m_sizeImageOriginal;
    destination.m_sizeImageDest = m_sizeImageDest;
    destination.m_clrTransparent = m_clrTransparent;
    destination.m_clrTransparentOriginal = m_clrTransparentOriginal;
    destination.m_clrImageShadow = m_clrImageShadow;
    destination.m_dblScale = m_dblScale;
    destination.m_strUDLPath = m_strUDLPath;
    return TRUE;
}

// Retail (0x180170c10) is a single 4-byte store to the m_bIsRTL static.
// Symbol: ?EnableRTL@CMFCToolBarImages@@SAXH@Z
extern "C" void MS_ABI impl__EnableRTL_CMFCToolBarImages__SAXH_Z(int bEnable)
{
    impl__m_bIsRTL_CMFCToolBarImages__1HA = bEnable;
}
// Retail (0x18016c4e0 in the old build; current 0x16c4f0) just returns 1.
// Symbol: ?Is32BitTransparencySupported@CMFCToolBarImages@@SAHXZ
extern "C" int MS_ABI impl__Is32BitTransparencySupported_CMFCToolBarImages__SAHXZ()
{
    return 1;
}
// Retail (0x18016e840) scans a 4-entry table at 0x180342d90:
//   { 0x000000, 0x12 }, { 0x808080, 0x10 }, { 0xC0C0C0, 0x0F }, { 0xFFFFFF, 0x14 }
// (fixed color, sys-color index: COLOR_BTNTEXT / COLOR_BTNSHADOW /
//  COLOR_BTNFACE / COLOR_BTNHIGHLIGHT).  On a match the CMFCColorMaker's
// color for that index is returned; the bHighContrast branch additionally
// channel-swaps the BGR value into RGB order (0x18016e894..).  The maker
// singleton is not modeled, so GetSysColor() approximates it (identical under
// default system colors).
// Symbol: ?MapToSysColor@CMFCToolBarImages@@SAKKH@Z
extern "C" unsigned long MS_ABI impl__MapToSysColor_CMFCToolBarImages__SAKKH_Z(
    unsigned long clr, int bHighContrast)
{
    static const struct { unsigned long clr; int nSys; } kTable[4] = {
        { 0x000000, COLOR_BTNTEXT },
        { 0x808080, COLOR_BTNSHADOW },
        { 0xC0C0C0, COLOR_BTNFACE },
        { 0xFFFFFF, COLOR_BTNHIGHLIGHT },
    };
    for (int i = 0; i < 4; ++i) {
        if (clr == kTable[i].clr) {
            const unsigned long sys = ::GetSysColor(kTable[i].nSys); // BGR
            if (bHighContrast) {
                // Retail swaps the maker's BGR color into RGB order.
                return ((sys & 0xFF) << 16) | (sys & 0xFF00) | ((sys >> 16) & 0xFF);
            }
            return sys;
        }
    }
    return clr;
}
// Retail (0x18016e940): a near-gray color (all pairwise channel deltas <= 10)
// is remapped onto the 3D button-face tone by scaling the face color with
// 1 + (channel - 192)/255 (helpers at 0x18034ff68=1.0, 0x180350090=192.0,
// 0x180350098=255.0 and the scale routine 0x18005b310); everything else is
// returned unchanged.  The global face color at 0x1803c1680 is approximated
// with GetSysColor(COLOR_BTNFACE).
// Symbol: ?MapToSysColorAlpha@CMFCToolBarImages@@SAKK@Z
extern "C" unsigned long MS_ABI impl__MapToSysColorAlpha_CMFCToolBarImages__SAKK_Z(
    unsigned long clr)
{
    const int r = static_cast<int>((clr >> 16) & 0xFF);
    const int g = static_cast<int>((clr >> 8) & 0xFF);
    const int b = static_cast<int>(clr & 0xFF);

    if (std::abs(r - g) > 10 || std::abs(g - b) > 10 || std::abs(r - b) > 10) {
        return clr;   // not gray -> unchanged (retail 0x18016ea0e)
    }

    const unsigned long sys = ::GetSysColor(COLOR_BTNFACE);
    const int sr = static_cast<int>((sys >> 16) & 0xFF);
    const int sg = static_cast<int>((sys >> 8) & 0xFF);
    const int sb = static_cast<int>(sys & 0xFF);

    // Scale channel ch by f, rounding like retail (round-half-up, clamp 255,
    // and a ch==0/f>1 special case from 0x18005b310).
    auto scale = [](int ch, double f) -> int {
        double v = static_cast<double>(ch) * f + 0.5;
        if (ch == 0 && f > 1.0) v = (f - 1.0) * 255.0 + 0.5;
        if (v > 255.0) return 255;
        if (v < 0.0) return 0;
        return static_cast<int>(v);
    };

    return (static_cast<unsigned long>(scale(sr, 1.0 + (r - 192) / 255.0)) << 16)
         | (static_cast<unsigned long>(scale(sg, 1.0 + (g - 192) / 255.0)) << 8)
         |  static_cast<unsigned long>(scale(sb, 1.0 + (b - 192) / 255.0));
}
// Retail (0x18016ea20) is the inverse of MapToSysColor: for each of the four
// table entries it compares clr against the maker's color for that entry
// (channel-swapped when bHighContrast, retail 0x18016ea6c..) and returns the
// fixed table color on a match.  The table colors are all pure gray, so the
// swap is a no-op on the return value.
// Symbol: ?MapFromSysColor@CMFCToolBarImages@@SAKKH@Z
extern "C" unsigned long MS_ABI impl__MapFromSysColor_CMFCToolBarImages__SAKKH_Z(
    unsigned long clr, int bHighContrast)
{
    static const struct { unsigned long clr; int nSys; } kTable[4] = {
        { 0x000000, COLOR_BTNTEXT },
        { 0x808080, COLOR_BTNSHADOW },
        { 0xC0C0C0, COLOR_BTNFACE },
        { 0xFFFFFF, COLOR_BTNHIGHLIGHT },
    };
    for (int i = 0; i < 4; ++i) {
        const unsigned long sys = ::GetSysColor(kTable[i].nSys); // BGR
        if (bHighContrast) {
            const unsigned long sw =
                ((sys & 0xFF) << 16) | (sys & 0xFF00) | ((sys >> 16) & 0xFF);
            if (clr == sw) return kTable[i].clr;
        } else {
            if (clr == sys) return kTable[i].clr;
        }
    }
    return clr;
}
#define TI_OFF_8(p, off)    (*reinterpret_cast<unsigned long long*>(reinterpret_cast<char*>(p) + (off)))
#define TI_OFF_BOOL(p, off) (*reinterpret_cast<BOOL*>(reinterpret_cast<char*>(p) + (off)))
#define TI_OFF_INT(p, off)  (*reinterpret_cast<int*>(reinterpret_cast<char*>(p) + (off)))
#define TI_OFF_PTR(p, off)  (*reinterpret_cast<void**>(reinterpret_cast<char*>(p) + (off)))
// Retail (0x18016fe90): clear/init dst (helper 0x18016f690), then mark it
// temporary + read-only and copy size/well/count/bpp from src.  Transcribed
// 1:1; the helper is the public InitMembers() (via Clear() which also resets
// the side-table state the OpenMFC core keeps per object).
// Symbol: ?CopyTemp@CMFCToolBarImages@@IEAAXAEAV1@@Z
extern "C" void MS_ABI impl__CopyTemp_CMFCToolBarImages__IEAAXAEAV1__Z(
    CMFCToolBarImages* pThis, CMFCToolBarImages* pDst)
{
    if (!pThis || !pDst) return;

    pDst->Clear();                                  // 0x18016f690
    TI_OFF_BOOL(pDst, 0x2C) = TRUE;                 // m_bIsTemporary
    TI_OFF_8(pDst, 0x68) = TI_OFF_8(pThis, 0x68);   // m_sizeImage
    TI_OFF_8(pDst, 0x78) = TI_OFF_8(pThis, 0x78);   // m_sizeImageDest
    TI_OFF_PTR(pDst, 0xA0) = TI_OFF_PTR(pThis, 0xA0); // m_hbmImageWell
    TI_OFF_BOOL(pDst, 0x1C) = TI_OFF_BOOL(pThis, 0x1C); // m_bUserImagesList
    TI_OFF_INT(pDst, 0x08) = TI_OFF_INT(pThis, 0x08);   // m_iCount
    TI_OFF_BOOL(pDst, 0x28) = TRUE;                 // m_bReadOnly
    TI_OFF_INT(pDst, 0x0C) = TI_OFF_INT(pThis, 0x0C);   // m_nBitsPerPixel
}
#undef TI_OFF_8
#undef TI_OFF_BOOL
#undef TI_OFF_INT
#undef TI_OFF_PTR
// Symbol: ?CopyTo@CMFCToolBarImages@@QEAAHAEAV1@@Z
extern "C" int MS_ABI impl__CopyTo_CMFCToolBarImages__QEAAHAEAV1__Z(
    CMFCToolBarImages* source, CMFCToolBarImages* destination)
{
    return source && destination && source->CopyTo(*destination);
}
// Retail (0x180171890): standard compatible-bitmap copy.  Create a compatible
// DC, select the source, GetObject for its size, CreateCompatibleBitmap, then
// BitBlt SRCCOPY through a second compatible DC.  Fully transcribed.
// Symbol: ?Copy@CMFCToolBarImages@@SAPEAUHBITMAP__@@PEAU2@@Z
extern "C" HBITMAP MS_ABI impl__Copy_CMFCToolBarImages__SAPEAUHBITMAP____PEAU2__Z(
    HBITMAP hBitmap)
{
    if (!hBitmap) return nullptr;

    HDC hdcSrc = ::CreateCompatibleDC(nullptr);
    if (!hdcSrc) return nullptr;
    HGDIOBJ hOldSrc = ::SelectObject(hdcSrc, hBitmap);
    if (!hOldSrc) {
        ::DeleteDC(hdcSrc);
        return nullptr;
    }

    BITMAP bmp;
    if (!::GetObjectW(hBitmap, sizeof(BITMAP), &bmp)) {
        ::SelectObject(hdcSrc, hOldSrc);
        ::DeleteDC(hdcSrc);
        return nullptr;
    }

    HBITMAP hNew = ::CreateCompatibleBitmap(hdcSrc, bmp.bmWidth, bmp.bmHeight);
    if (!hNew) {
        ::SelectObject(hdcSrc, hOldSrc);
        ::DeleteDC(hdcSrc);
        return nullptr;
    }

    HDC hdcDst = ::CreateCompatibleDC(nullptr);
    HGDIOBJ hOldDst = hdcDst ? ::SelectObject(hdcDst, hNew) : nullptr;
    if (!hOldDst) {
        ::DeleteObject(hNew);
        if (hdcDst) ::DeleteDC(hdcDst);
        ::SelectObject(hdcSrc, hOldSrc);
        ::DeleteDC(hdcSrc);
        return nullptr;
    }

    ::BitBlt(hdcDst, 0, 0, bmp.bmWidth, bmp.bmHeight, hdcSrc, 0, 0, SRCCOPY);

    ::SelectObject(hdcDst, hOldDst);
    ::SelectObject(hdcSrc, hOldSrc);
    ::DeleteDC(hdcDst);
    ::DeleteDC(hdcSrc);
    return hNew;
}
// Retail (0x18016c6d0, ~9e0 bytes) draws image iImageIndex at (x, y) through
// BeginDrawImage / the mono memory DC / the alpha pipeline and the visual
// manager; not modeled.
// Symbol: ?Draw@CMFCToolBarImages@@QEAAHPEAVCDC@@HHHHHHHHE@Z
extern "C" int MS_ABI impl__Draw_CMFCToolBarImages__QEAAHPEAVCDC__HHHHHHHHE_Z(
    CMFCToolBarImages* pThis, CDC* pDC,
    int x, int y, int iImageIndex, int cx, int cy, int nGap,
    int bDisabled, int bShadow, unsigned char alphaSrc)
{
    (void)pDC; (void)x; (void)y; (void)iImageIndex; (void)cx; (void)cy;
    (void)nGap; (void)bDisabled; (void)bShadow; (void)alphaSrc;
    if (!pThis) return 0;
    // TODO(clean-room): transcribed partially -- the retail draw pipeline
    // (0x18016c6d0) is not modeled.
    return 0;
}
namespace {
void AlignImageAxis(int boundStart, int boundEnd, int& destStart, int& destEnd,
                    int& sourceStart, int& sourceEnd, int align) {
    if (align == 3) return;
    const int extent = sourceEnd - sourceStart;
    const int available = boundEnd - boundStart;
    const int unused = available - extent;
    const int anchor = boundStart + (align == 1 ? unused / 2 : align == 2 ? unused : 0);
    destStart = (std::max)(boundStart, anchor);
    destEnd = (std::min)(boundEnd, anchor + extent);
    sourceStart += destStart - anchor;
    sourceEnd = sourceStart + destEnd - destStart;
}

BOOL PaintImage(HDC dest, const CRect& rect, HDC src, const CRect& source,
                bool perPixelAlpha, COLORREF key, BYTE alpha) {
    static const auto blend = [] {
        using AlphaBlendFunction = decltype(&::AlphaBlend);
        const HMODULE msimg = ::LoadLibraryW(L"msimg32.dll");
        return msimg ? reinterpret_cast<AlphaBlendFunction>(::GetProcAddress(msimg, "AlphaBlend")) : nullptr;
    }();
    if (perPixelAlpha) {
        const BLENDFUNCTION function{AC_SRC_OVER, 0, alpha, AC_SRC_ALPHA};
        return blend && blend(dest, rect.left, rect.top, rect.Width(), rect.Height(), src,
            source.left, source.top, source.Width(), source.Height(), function);
    }
    if (key != CLR_INVALID) {
        static const auto transparent = [] {
            using TransparentBltFunction = decltype(&::TransparentBlt);
            const HMODULE msimg = ::GetModuleHandleW(L"msimg32.dll");
            return msimg ? reinterpret_cast<TransparentBltFunction>(::GetProcAddress(msimg, "TransparentBlt")) : nullptr;
        }();
        return transparent && transparent(dest, rect.left, rect.top, rect.Width(), rect.Height(), src,
            source.left, source.top, source.Width(), source.Height(), key);
    }
    return ::StretchBlt(dest, rect.left, rect.top, rect.Width(), rect.Height(), src,
        source.left, source.top, source.Width(), source.Height(), SRCCOPY);
}
} // namespace

BOOL CMFCToolBarImages::DrawEx(CDC* dc, CRect bounds, int index, int horizontal, int vertical,
                              CRect source, BYTE alpha) {
    const HDC dest = dc ? dc->GetSafeHdc() : nullptr;
    if (!dest || !m_hbmImageWell || bounds.IsRectEmpty() || index < 0 || index >= m_iCount ||
        horizontal < 0 || horizontal > 3 || vertical < 0 || vertical > 3) return FALSE;

    const CRect cell(0, 0, m_sizeImage.cx, m_sizeImage.cy);
    if (source.IsRectEmpty()) source = cell;
    if (!source.IntersectRect(source, cell)) return FALSE;
    CRect rect = bounds;
    AlignImageAxis(bounds.left, bounds.right, rect.left, rect.right, source.left, source.right, horizontal);
    AlignImageAxis(bounds.top, bounds.bottom, rect.top, rect.bottom, source.top, source.bottom, vertical);
    source.OffsetRect(index * m_sizeImage.cx, 0);
    const MemoryDC src(::CreateCompatibleDC(dest), &::DeleteDC);
    if (!src) return FALSE;
    const auto old = ::SelectObject(src.get(), m_hbmImageWell);
    if (!old) return FALSE;
    const BOOL result = PaintImage(dest, rect, src.get(), source, m_nBitsPerPixel == 32, m_clrTransparent, alpha);
    ::SelectObject(src.get(), old);
    return result;
}

// Symbol: ?DrawEx@CMFCToolBarImages@@QEAAHPEAVCDC@@VCRect@@HW4ImageAlignHorz@1@W4ImageAlignVert@1@1E@Z
extern "C" int MS_ABI impl__DrawEx_CMFCToolBarImages__QEAAHPEAVCDC__VCRect__HW4ImageAlignHorz_1_W4ImageAlignVert_1_1E_Z(
    CMFCToolBarImages* images, CDC* dc, CRect bounds, int index, int horizontal, int vertical,
    CRect source, BYTE alpha)
{
    return images && images->DrawEx(dc, bounds, index, horizontal, vertical, source, alpha);
}
#define TI_OFF_8(p, off)    (*reinterpret_cast<unsigned long long*>(reinterpret_cast<char*>(p) + (off)))
#define TI_OFF_BOOL(p, off) (*reinterpret_cast<BOOL*>(reinterpret_cast<char*>(p) + (off)))
#define TI_OFF_INT(p, off)  (*reinterpret_cast<int*>(reinterpret_cast<char*>(p) + (off)))
#define TI_OFF_PTR(p, off)  (*reinterpret_cast<void**>(reinterpret_cast<char*>(p) + (off)))
#define TI_OFF_ULONG(p, off)(*reinterpret_cast<unsigned long*>(reinterpret_cast<char*>(p) + (off)))
// Retail (0x18016c270).  Observable member effects transcribed:
//   - m_bCreateMonoDC: reselect the saved mono/gray DC handles and delete the
//     saved palette.
//   - m_sizeImageDest and m_rectLastDraw are cleared.
//   - when m_bStretch or (bpp != 32 && clrTransparent != -1): restore the
//     original bitmap, delete m_bmpMem and the memory DC, clear m_pBmpOriginal.
// The global mono/gray DCs and the m_bIsDrawOnGlass cleanup are unmodeled.
// Symbol: ?EndDrawImage@CMFCToolBarImages@@QEAAXAEAUtagAFXDrawState@@@Z
extern "C" void MS_ABI impl__EndDrawImage_CMFCToolBarImages__QEAAXAEAUtagAFXDrawState___Z(
    CMFCToolBarImages* pThis, tagAFXDrawState* pState)
{
    if (!pThis) return;

    if (TI_OFF_BOOL(pThis, 0x44)) {                 // m_bCreateMonoDC
        // TODO(clean-room): retail reselects pState->hOldPattern into the
        // global mono DC (0x1803c39b8) and pState->hOldGrayPattern into the
        // gray DC (0x1803c38a8); not modeled.
        if (pState && pState->hOldPalette) {
            ::DeleteObject(pState->hOldPalette);
            pState->hOldPalette = nullptr;
        }
    }

    TI_OFF_8(pThis, 0x78) = 0;                      // m_sizeImageDest
    std::memset(reinterpret_cast<char*>(pThis) + 0x80, 0, 16); // m_rectLastDraw

    const BOOL bStretch = TI_OFF_BOOL(pThis, 0x24);
    const int nBpp = TI_OFF_INT(pThis, 0x0C);
    // Retail 0x16c2e1 (mfc140u): `cmpl $0,0x24(%rdi); jne <restore>`, then the
    // (bpp == 32 ? -1 : m_clrTransparent) != -1 test -- i.e. m_bStretch OR
    // the transparent-colour case, as the comment above says.
    if (bStretch || (nBpp != 32 &&
                     TI_OFF_ULONG(pThis, 0xD8) != static_cast<unsigned long>(-1))) {
        if (TI_OFF_PTR(pThis, 0xD0)) {              // m_pBmpOriginal
            // TODO(clean-room): retail first reselects the original bitmap
            // into m_dcMem (0x18016c2fb); the mem-DC contents are not modeled.
            // m_bmpMem @0xC0, m_dcMem @0x48 (both protected; reached by offset).
            reinterpret_cast<CBitmap*>(reinterpret_cast<char*>(pThis) + 0xC0)->DeleteObject();
            impl__DeleteDC_CDC__QEAAHXZ(
                reinterpret_cast<CDC*>(reinterpret_cast<char*>(pThis) + 0x48));
            TI_OFF_PTR(pThis, 0xD0) = nullptr;
        }
    }
}
#undef TI_OFF_8
#undef TI_OFF_BOOL
#undef TI_OFF_INT
#undef TI_OFF_PTR
#undef TI_OFF_ULONG
// Retail (0x18016d4b0) fills the rect with the cached CMFCColorMaker dithered
// brush (8x8 face/hilite hatch) via FillRect.  The maker singleton is not
// modeled, so a solid button-face brush is used.
// Symbol: ?FillDitheredRect@CMFCToolBarImages@@SAXPEAVCDC@@AEBVCRect@@@Z
extern "C" void MS_ABI impl__FillDitheredRect_CMFCToolBarImages__SAXPEAVCDC__AEBVCRect___Z(
    CDC* pDC, const CRect* pRect)
{
    if (!pDC || !pRect) return;
    // TODO(clean-room): retail fills with the dithered brush owned by the
    // CMFCColorMaker (0x1803c1620); approximated here with the solid
    // button-face brush.
    HBRUSH hbr = ::GetSysColorBrush(COLOR_BTNFACE);
    if (!hbr) return;
    ::FillRect(pDC->GetSafeHdc(), *pRect, hbr);
}
BOOL CMFCToolBarImages::CopyImageToClipboard(int index) {
    if (!m_hbmImageWell || index < 0 || index >= m_iCount) return FALSE;
    void* pixels = nullptr;
    auto bitmap = CreateImageBitmap(m_sizeImage.cx, m_sizeImage.cy, 24, &pixels);
    const MemoryDC canvas(::CreateCompatibleDC(nullptr), &::DeleteDC);
    if (!bitmap || !canvas) return FALSE;
    const auto old = ::SelectObject(canvas.get(), bitmap.get());
    if (!old) return FALSE;
    const CRect bounds(0, 0, m_sizeImage.cx, m_sizeImage.cy);
    CDC dc;
    dc.m_hDC = canvas.get();
    const bool drawn = ::FillRect(canvas.get(), bounds, ::GetSysColorBrush(COLOR_BTNFACE)) &&
        DrawEx(&dc, bounds, index);
    dc.m_hDC = nullptr;
    ::SelectObject(canvas.get(), old);
    if (!drawn) return FALSE;

    const auto active = ::GetActiveWindow();
    const auto owner = active ? active : ::CreateWindowExW(0, L"STATIC", nullptr, 0,
        0, 0, 0, 0, HWND_MESSAGE, nullptr, nullptr, nullptr);
    BOOL copied = FALSE;
    if (owner && ::OpenClipboard(owner)) {
        if (::EmptyClipboard() && ::SetClipboardData(CF_BITMAP, bitmap.get())) {
            bitmap.release();
            copied = TRUE;
        }
        ::CloseClipboard();
    }
    if (!active && owner) ::DestroyWindow(owner);
    return copied;
}

// Symbol: ?CopyImageToClipboard@CMFCToolBarImages@@QEAAHH@Z
extern "C" int MS_ABI impl__CopyImageToClipboard_CMFCToolBarImages__QEAAHH_Z(
    CMFCToolBarImages* images, int index)
{
    return images && images->CopyImageToClipboard(index);
}

BOOL CMFCToolBarImages::DeleteImage(int index) {
    if (m_bIsTemporary || !m_bUserImagesList || !m_hbmImageWell ||
        index < 0 || index >= m_iCount || m_sizeImage.cx <= 0 || m_sizeImage.cy <= 0) return FALSE;
    const int count = m_iCount - 1;
    if (count == 0) {
        ReleaseImageWell();
        m_bModified = TRUE;
        return TRUE;
    }

    const int cellWidth = m_sizeImage.cx;
    const int height = m_sizeImage.cy;
    const int left = index * cellWidth;
    const int after = (count - index) * cellWidth;
    OwnedBitmap bitmap(nullptr, &::DeleteObject);
    const MemoryDC src(::CreateCompatibleDC(nullptr), &::DeleteDC);
    const MemoryDC dst(::CreateCompatibleDC(nullptr), &::DeleteDC);
    if (!src || !dst) return FALSE;
    const auto oldSrc = ::SelectObject(src.get(), m_hbmImageWell);
    if (!oldSrc) return FALSE;

    if (m_nBitsPerPixel <= 8) {
        bitmap.reset(::CreateCompatibleBitmap(src.get(), count * cellWidth, height));
    } else {
        void* pixels = nullptr;
        bitmap = CreateImageBitmap(count * cellWidth, height, m_nBitsPerPixel == 32 ? 32 : 24, &pixels);
    }
    if (!bitmap) return FALSE;
    const auto oldDst = ::SelectObject(dst.get(), bitmap.get());
    if (!oldDst) return FALSE;

    RGBQUAD palette[256]{};
    const UINT colors = m_nBitsPerPixel <= 8 ? ::GetDIBColorTable(src.get(), 0, 256, palette) : 0;
    const bool copied = (!colors || ::SetDIBColorTable(dst.get(), 0, colors, palette) == colors) &&
        (!left || ::BitBlt(dst.get(), 0, 0, left, height, src.get(), 0, 0, SRCCOPY)) &&
        (!after || ::BitBlt(dst.get(), left, 0, after, height, src.get(), left + cellWidth, 0, SRCCOPY));
    ::SelectObject(src.get(), oldSrc);
    ::SelectObject(dst.get(), oldDst);
    BITMAP stored{};
    if (!copied || !::GetObjectW(bitmap.get(), sizeof(stored), &stored)) return FALSE;

    ReleaseImageWell();
    m_hbmImageWell = bitmap.release();
    m_iCount = count;
    m_nBitsPerPixel = stored.bmBitsPixel;
    m_bModified = TRUE;
    return TRUE;
}

// Symbol: ?DeleteImage@CMFCToolBarImages@@QEAAHH@Z
extern "C" int MS_ABI impl__DeleteImage_CMFCToolBarImages__QEAAHH_Z(
    CMFCToolBarImages* images, int index)
{
    return images && images->DeleteImage(index);
}

HICON CMFCToolBarImages::ExtractIcon(int index) const {
    if (!m_hbmImageWell || index < 0 || index >= m_iCount ||
        m_sizeImage.cx <= 0 || m_sizeImage.cy <= 0) return nullptr;
    const bool alpha = m_nBitsPerPixel == 32;
    const auto bitmap = CopyCell(m_hbmImageWell, m_sizeImage, index, alpha, false, m_clrTransparent);
    if (!bitmap) return nullptr;
    if (alpha) {
        BITMAP info{};
        if (!::GetObjectW(bitmap.get(), sizeof(info), &info) || !info.bmBits) return nullptr;
        const size_t stride = (static_cast<size_t>(m_sizeImage.cx) + 15) / 16 * 2;
        std::vector<BYTE> maskBits(stride * m_sizeImage.cy);
        ::GdiFlush();
        auto* bits = static_cast<BYTE*>(info.bmBits);
        for (int y = 0; y < m_sizeImage.cy; ++y) {
            const auto* row = bits + static_cast<size_t>(y) * info.bmWidthBytes;
            for (int x = 0; x < m_sizeImage.cx; ++x) {
                if (row[static_cast<size_t>(x) * 4 + 3] == 0)
                    maskBits[y * stride + x / 8] |= static_cast<BYTE>(0x80 >> (x % 8));
            }
        }
        const OwnedBitmap mask(::CreateBitmap(m_sizeImage.cx, m_sizeImage.cy, 1, 1, maskBits.data()),
                               &::DeleteObject);
        if (!mask) return nullptr;
        UnpremultiplyPixels(bits, static_cast<size_t>(info.bmWidthBytes) * m_sizeImage.cy);
        ICONINFO iconInfo{TRUE, 0, 0, mask.get(), bitmap.get()};
        return ::CreateIconIndirect(&iconInfo);
    }

    const auto list = ::ImageList_Create(m_sizeImage.cx, m_sizeImage.cy, ILC_COLOR32 | ILC_MASK, 1, 1);
    if (!list) return nullptr;
    const COLORREF key = m_clrTransparent == CLR_INVALID ? ::GetSysColor(COLOR_BTNFACE) : m_clrTransparent;
    const int added = ::ImageList_AddMasked(list, bitmap.get(), key);
    const HICON icon = added >= 0 ? ::ImageList_GetIcon(list, added, ILD_TRANSPARENT) : nullptr;
    ::ImageList_Destroy(list);
    return icon;
}

// Symbol: ?ExtractIconW@CMFCToolBarImages@@QEAAPEAUHICON__@@H@Z
extern "C" HICON MS_ABI impl__ExtractIconW_CMFCToolBarImages__QEAAPEAUHICON____H_Z(
    CMFCToolBarImages* images, int index)
{
    return images ? images->ExtractIcon(index) : nullptr;
}
#define TI_OFF_INT(p, off) (*reinterpret_cast<int*>(reinterpret_cast<char*>(p) + (off)))
// Retail (0x180170010): validate the list, clear, copy its icon size into
// m_sizeImage and add every icon through the internal AddBitmapToBmp helper.
// The helper is approximated by the public AddIcon() (tracks count in the
// OpenMFC side table).
// Symbol: ?CreateFromImageList@CMFCToolBarImages@@QEAAHAEBVCImageList@@@Z
extern "C" int MS_ABI impl__CreateFromImageList_CMFCToolBarImages__QEAAHAEBVCImageList___Z(
    CMFCToolBarImages* pThis, const CImageList* pList)
{
    if (!pThis || !pList) return 0;
    const HIMAGELIST himl = pList->GetSafeHandle();
    if (!himl) return 0;
    const int nCount = ::ImageList_GetImageCount(himl);
    if (nCount <= 0) return 0;

    pThis->Clear();                                 // 0x18016f690

    int cx = 0;
    int cy = 0;
    ::ImageList_GetIconSize(himl, &cx, &cy);
    TI_OFF_INT(pThis, 0x68) = cx;                   // m_sizeImage.cx
    TI_OFF_INT(pThis, 0x6C) = cy;                   // m_sizeImage.cy

    for (int i = 0; i < nCount; ++i) {
        HICON hIcon = ::ImageList_GetIcon(himl, i, 0);
        if (!hIcon) continue;
        pThis->AddIcon(hIcon, FALSE);               // 0x18016dff0
        ::DestroyIcon(hIcon);                       // 0x1802c7278
    }
    return 1;
}
#undef TI_OFF_INT
// Retail (0x180170510): horizontally flip each nWidth-wide sub-image by
// swapping 32bpp pixels directly in a DIB's bmBits.  Direct path transcribed;
// non-32bpp falls back to a GetBitmapBits/SetBitmapBits path (unmodeled).
// Symbol: ?MirrorBitmap@CMFCToolBarImages@@SAHAEAPEAUHBITMAP__@@H@Z
extern "C" int MS_ABI impl__MirrorBitmap_CMFCToolBarImages__SAHAEAPEAUHBITMAP____H_Z(
    HBITMAP* pHBitmap, int nWidth)
{
    if (!pHBitmap) return 0;
    if (!*pHBitmap) return 1;
    if (nWidth <= 0) return 0;

    BITMAP bmp;
    if (!::GetObjectW(*pHBitmap, sizeof(BITMAP), &bmp)) return 0;

    // Retail direct path requires a 32bpp DIB with bmBits (0x18017058a).
    if (bmp.bmBitsPixel != 32 || !bmp.bmBits) {
        // TODO(clean-room): retail falls back to GetBitmapBits/SetBitmapBits
        // row swapping for non-32bpp (0x180170686); not modeled.
        return 0;
    }

    const int nImages = bmp.bmWidth / nWidth;
    const int half = nWidth / 2;
    DWORD* const bits = static_cast<DWORD*>(bmp.bmBits);
    for (int iy = 0; iy < nImages; ++iy) {
        for (int y = 0; y < bmp.bmHeight; ++y) {
            DWORD* left = bits + static_cast<long>(iy) * nWidth
                               + static_cast<long>(y) * bmp.bmWidth;
            DWORD* right = left + nWidth - 1;
            for (int ic = 0; ic < half; ++ic) {
                const DWORD tmp = left[ic];
                left[ic] = right[-ic];
                right[-ic] = tmp;
            }
        }
    }
    return 1;
}
// Retail (0x180170870): vertically flip each nWidth-row-tall sub-image by
// swapping whole scan lines.  Direct path (bmPlanes == bmBitsPixel, bmBits
// present) transcribed; the GetBitmapBits fallback (0x180170a2c) is not.
// Symbol: ?MirrorBitmapVert@CMFCToolBarImages@@SAHAEAPEAUHBITMAP__@@H@Z
extern "C" int MS_ABI impl__MirrorBitmapVert_CMFCToolBarImages__SAHAEAPEAUHBITMAP____H_Z(
    HBITMAP* pHBitmap, int nWidth)
{
    if (!pHBitmap) return 0;
    if (!*pHBitmap) return 1;
    if (nWidth <= 0) return 0;

    BITMAP bmp;
    if (!::GetObjectW(*pHBitmap, sizeof(BITMAP), &bmp)) return 0;

    // Retail direct path requires bmBitsPixel >= 16, bmPlanes == bmBitsPixel
    // and bmBits (0x1801708e3 / 0x180170908).
    if (bmp.bmBitsPixel < 16 || bmp.bmPlanes != bmp.bmBitsPixel || !bmp.bmBits) {
        // TODO(clean-room): retail falls back to a GetBitmapBits/SetBitmapBits
        // row swap (0x180170a2c); not modeled.
        return 0;
    }

    const int nImages = bmp.bmWidth / nWidth;
    const int half = nWidth / 2;
    // (bmWidth * bpp + 7) / 8 rounded up to a 4-byte boundary (0x180170928..).
    int rowBytes = (bmp.bmWidth * bmp.bmBitsPixel + 7) / 8;
    rowBytes = (rowBytes + 3) & ~3;

    std::vector<unsigned char> tmp(static_cast<size_t>(rowBytes));
    unsigned char* const base = static_cast<unsigned char*>(bmp.bmBits);
    for (int iy = 0; iy < nImages; ++iy) {
        for (int y = 0; y < half; ++y) {
            unsigned char* top = base + static_cast<long>(iy) * nWidth * rowBytes
                                      + static_cast<long>(y) * rowBytes;
            unsigned char* bottom = base + static_cast<long>(iy) * nWidth * rowBytes
                                         + static_cast<long>(nWidth - 1 - y) * rowBytes;
            std::memcpy(tmp.data(), top, rowBytes);
            std::memcpy(top, bottom, rowBytes);
            std::memcpy(bottom, tmp.data(), rowBytes);
        }
    }
    return 1;
}
#define TI_OFF_INT(p, off)  (*reinterpret_cast<int*>(reinterpret_cast<char*>(p) + (off)))
#define TI_OFF_PTR(p, off)  (*reinterpret_cast<void**>(reinterpret_cast<char*>(p) + (off)))
// Retail (0x180170810): MirrorBitmap the well, then the light/shadow bitmaps
// when present; returns the first result.
// Symbol: ?Mirror@CMFCToolBarImages@@QEAAHXZ
extern "C" int MS_ABI impl__Mirror_CMFCToolBarImages__QEAAHXZ(CMFCToolBarImages* pThis)
{
    if (!pThis) return 0;
    const int w = TI_OFF_INT(pThis, 0x68);          // m_sizeImage.cx
    if (!impl__MirrorBitmap_CMFCToolBarImages__SAHAEAPEAUHBITMAP____H_Z(
            reinterpret_cast<HBITMAP*>(reinterpret_cast<char*>(pThis) + 0xA0), w)) {
        return 0;
    }
    if (TI_OFF_PTR(pThis, 0xA8)) {                  // m_hbmImageLight
        impl__MirrorBitmap_CMFCToolBarImages__SAHAEAPEAUHBITMAP____H_Z(
            reinterpret_cast<HBITMAP*>(reinterpret_cast<char*>(pThis) + 0xA8), w);
    }
    if (TI_OFF_PTR(pThis, 0xB0)) {                  // m_hbmImageShadow
        impl__MirrorBitmap_CMFCToolBarImages__SAHAEAPEAUHBITMAP____H_Z(
            reinterpret_cast<HBITMAP*>(reinterpret_cast<char*>(pThis) + 0xB0), w);
    }
    return 1;
}
// Retail (0x180170bb0): same pattern as Mirror with MirrorBitmapVert and the
// image height (m_sizeImage.cy).
// Symbol: ?MirrorVert@CMFCToolBarImages@@QEAAHXZ
extern "C" int MS_ABI impl__MirrorVert_CMFCToolBarImages__QEAAHXZ(CMFCToolBarImages* pThis)
{
    if (!pThis) return 0;
    const int h = TI_OFF_INT(pThis, 0x6C);          // m_sizeImage.cy
    if (!impl__MirrorBitmapVert_CMFCToolBarImages__SAHAEAPEAUHBITMAP____H_Z(
            reinterpret_cast<HBITMAP*>(reinterpret_cast<char*>(pThis) + 0xA0), h)) {
        return 0;
    }
    if (TI_OFF_PTR(pThis, 0xA8)) {                  // m_hbmImageLight
        impl__MirrorBitmapVert_CMFCToolBarImages__SAHAEAPEAUHBITMAP____H_Z(
            reinterpret_cast<HBITMAP*>(reinterpret_cast<char*>(pThis) + 0xA8), h);
    }
    if (TI_OFF_PTR(pThis, 0xB0)) {                  // m_hbmImageShadow
        impl__MirrorBitmapVert_CMFCToolBarImages__SAHAEAPEAUHBITMAP____H_Z(
            reinterpret_cast<HBITMAP*>(reinterpret_cast<char*>(pThis) + 0xB0), h);
    }
    return 1;
}
#undef TI_OFF_INT
#undef TI_OFF_PTR
// Retail (0x18016fe80) is a tail call: MapBmpTo3dColors(&m_hbmImageWell, ...).
// Symbol: ?MapTo3dColors@CMFCToolBarImages@@QEAAHHKK@Z
extern "C" int MS_ABI impl__MapTo3dColors_CMFCToolBarImages__QEAAHHKK_Z(
    CMFCToolBarImages* pThis, int bAutoCheckPremlt,
    unsigned long clrSrc, unsigned long clrDest)
{
    if (!pThis) return 0;
    return impl__MapBmpTo3dColors_CMFCToolBarImages__KAHAEAPEAUHBITMAP____HKK_Z(
        reinterpret_cast<HBITMAP*>(reinterpret_cast<char*>(pThis) + 0xA0),
        bAutoCheckPremlt, clrSrc, clrDest);
}
// Retail (0x18016fc10) walks the bitmap pixels replacing clrSrc with clrDest
// (with a pre-multiply auto-check).  Guards transcribed; pixel loop unmodeled.
// Symbol: ?MapBmpTo3dColors@CMFCToolBarImages@@KAHAEAPEAUHBITMAP__@@HKK@Z
extern "C" int MS_ABI impl__MapBmpTo3dColors_CMFCToolBarImages__KAHAEAPEAUHBITMAP____HKK_Z(
    HBITMAP* pHBitmap, int bAutoCheckPremlt,
    unsigned long clrSrc, unsigned long clrDest)
{
    (void)bAutoCheckPremlt;
    if (!pHBitmap || !*pHBitmap) return 0;
    if (clrDest == static_cast<unsigned long>(-1)) return 0;
    // TODO(clean-room): transcribed partially -- retail (0x18016fc10) creates
    // a copy of the bitmap and swaps clrSrc -> clrDest per pixel; not modeled.
    return 0;
}
#define TI_OFF_BOOL(p, off) (*reinterpret_cast<BOOL*>(reinterpret_cast<char*>(p) + (off)))
// Retail (0x180170000, mfc140u) tail-calls the static PreMultiplyAlpha
// (0x18016fef0, mfc140u; defined below) with (hbmp, m_bAutoCheckPremlt).  The helper is transcribed
// inline: reject non-32bpp/bitmap-less handles, optionally skip already
// premultiplied buffers, then multiply each channel by alpha/255 using the
// 0x80808081 magic constant (equivalent to integer division by 255).
// Symbol: ?PreMultiplyAlpha@CMFCToolBarImages@@IEAAHPEAUHBITMAP__@@@Z
extern "C" int MS_ABI impl__PreMultiplyAlpha_CMFCToolBarImages__IEAAHPEAUHBITMAP_____Z(
    CMFCToolBarImages* pThis, HBITMAP hbmp)
{
    if (!pThis) return 0;
    const BOOL bAutoCheckPremlt = TI_OFF_BOOL(pThis, 0x40);

    if (!hbmp) return 0;
    BITMAP bmp;
    if (!::GetObjectW(hbmp, sizeof(BITMAP), &bmp)) return 0;
    if (bmp.bmBitsPixel != 32 || !bmp.bmBits) return 0;

    const int nPixels = bmp.bmWidth * bmp.bmHeight;
    if (nPixels <= 0) return 1;

    unsigned char* const bits = static_cast<unsigned char*>(bmp.bmBits);
    if (bAutoCheckPremlt) {
        // Skip the conversion when every pixel is already premultiplied
        // (channel <= alpha, retail 0x18016ff50..).
        bool bNeedConvert = false;
        for (int i = 0; i < nPixels; ++i) {
            const unsigned char a = bits[i * 4 + 3];
            if (bits[i * 4 + 0] > a || bits[i * 4 + 1] > a || bits[i * 4 + 2] > a) {
                bNeedConvert = true;
                break;
            }
        }
        if (!bNeedConvert) return 1;
    }

    for (int i = 0; i < nPixels; ++i) {
        const unsigned int a = bits[i * 4 + 3];
        bits[i * 4 + 0] = static_cast<unsigned char>((bits[i * 4 + 0] * a) / 255);
        bits[i * 4 + 1] = static_cast<unsigned char>((bits[i * 4 + 1] * a) / 255);
        bits[i * 4 + 2] = static_cast<unsigned char>((bits[i * 4 + 2] * a) / 255);
    }
    return 1;
}
#define TI_OFF_INT(p, off)  (*reinterpret_cast<int*>(reinterpret_cast<char*>(p) + (off)))
#define TI_OFF_PTR(p, off)  (*reinterpret_cast<void**>(reinterpret_cast<char*>(p) + (off)))
// Retail (0x180170110) flags the images gray and runs a gamma/LUT conversion
// over the well.  The state writes are transcribed; the pixel loop is not.
// Symbol: ?GrayImages@CMFCToolBarImages@@QEAAHH@Z
extern "C" int MS_ABI impl__GrayImages_CMFCToolBarImages__QEAAHH_Z(
    CMFCToolBarImages* pThis, int nLuminancePercentage)
{
    if (!pThis) return 0;
    TI_OFF_BOOL(pThis, 0x34) = TRUE;                // m_bIsGray
    TI_OFF_INT(pThis, 0x10) = nLuminancePercentage; // m_nGrayImageLuminancePercentage
    if (!TI_OFF_PTR(pThis, 0xA0)) return 0;         // no well -> FALSE
    // TODO(clean-room): transcribed partially -- retail (0x180170110) walks
    // the well pixels applying the luminance table; not modeled.
    return 0;
}
#define TI_OFF_8(p, off)    (*reinterpret_cast<unsigned long long*>(reinterpret_cast<char*>(p) + (off)))
#define TI_OFF_DBL(p, off)  (*reinterpret_cast<double*>(reinterpret_cast<char*>(p) + (off)))
#define TI_OFF_ULONG(p, off)(*reinterpret_cast<unsigned long*>(reinterpret_cast<char*>(p) + (off)))
// Retail (0x18016d500, ~f60 bytes).  Observable member effects transcribed in
// order: when m_bIsTemporary the whole body is skipped; a scale != 1.0 resets
// m_dblScale/m_nBitsPerPixel, swaps the transparent colors, copies back
// m_sizeImageOriginal and clears the size/draw rects; then the count is
// recomputed from the well width, the well is mirrored when the m_bIsRTL
// static is set, the light/shadow bitmaps are deleted, gray images are
// re-grayed and m_clrImageShadow is refreshed.
// Symbol: ?OnSysColorChange@CMFCToolBarImages@@QEAAXXZ
extern "C" void MS_ABI impl__OnSysColorChange_CMFCToolBarImages__QEAAXXZ(
    CMFCToolBarImages* pThis)
{
    if (!pThis) return;

    if (TI_OFF_BOOL(pThis, 0x2C)) return;           // m_bIsTemporary

    if (TI_OFF_DBL(pThis, 0xE8) != 1.0) {           // m_dblScale
        TI_OFF_DBL(pThis, 0xE8) = 1.0;
        TI_OFF_INT(pThis, 0x0C) = 0;                // m_nBitsPerPixel
        const unsigned long clrOrig = TI_OFF_ULONG(pThis, 0xDC); // m_clrTransparentOriginal
        if (clrOrig != static_cast<unsigned long>(-1)) {
            TI_OFF_ULONG(pThis, 0xD8) = clrOrig;    // m_clrTransparent
            TI_OFF_ULONG(pThis, 0xDC) = static_cast<unsigned long>(-1);
        }
        TI_OFF_8(pThis, 0x68) = TI_OFF_8(pThis, 0x70); // m_sizeImage = m_sizeImageOriginal
        TI_OFF_8(pThis, 0x70) = 0;                      // m_sizeImageOriginal
        TI_OFF_8(pThis, 0x78) = 0;                      // m_sizeImageDest
        std::memset(reinterpret_cast<char*>(pThis) + 0x80, 0, 16); // m_rectLastDraw
        std::memset(reinterpret_cast<char*>(pThis) + 0x90, 0, 16); // m_rectSubImage
    }

    // TODO(clean-room): retail reloads the color map from m_strUDLPath
    // (0x18016b9e0) when m_bUserImagesList is set; the UDL parser is not
    // modeled.

    // UpdateCount (0x18016d840): m_iCount = wellWidth / m_sizeImage.cx.
    if (TI_OFF_PTR(pThis, 0xA0)) {
        BITMAP bmp;
        const int w = TI_OFF_INT(pThis, 0x68);
        if (w > 0 && ::GetObjectW(TI_OFF_PTR(pThis, 0xA0), sizeof(BITMAP), &bmp)) {
            TI_OFF_INT(pThis, 0x08) = bmp.bmWidth / w;
        }
    }

    if (impl__m_bIsRTL_CMFCToolBarImages__1HA) {
        const int w = TI_OFF_INT(pThis, 0x68);
        impl__MirrorBitmap_CMFCToolBarImages__SAHAEAPEAUHBITMAP____H_Z(
            reinterpret_cast<HBITMAP*>(reinterpret_cast<char*>(pThis) + 0xA0), w);
    }

    void** ppLight = reinterpret_cast<void**>(reinterpret_cast<char*>(pThis) + 0xA8);
    if (*ppLight) {                                 // m_hbmImageLight
        ::DeleteObject(*ppLight);
        *ppLight = nullptr;
    }
    void** ppShadow = reinterpret_cast<void**>(reinterpret_cast<char*>(pThis) + 0xB0);
    if (*ppShadow) {                                // m_hbmImageShadow
        ::DeleteObject(*ppShadow);
        *ppShadow = nullptr;
    }

    if (TI_OFF_BOOL(pThis, 0x34)) {                 // m_bIsGray
        impl__GrayImages_CMFCToolBarImages__QEAAHH_Z(pThis, TI_OFF_INT(pThis, 0x10));
    }

    // m_clrImageShadow = maker's shadow color (0x1803c164c), approximated
    // with the system button shadow color (identical under default colors).
    TI_OFF_ULONG(pThis, 0xE0) = ::GetSysColor(COLOR_BTNSHADOW);
}
#undef TI_OFF_8
#undef TI_OFF_BOOL
#undef TI_OFF_DBL
#undef TI_OFF_INT
#undef TI_OFF_PTR
#undef TI_OFF_ULONG
// Retail (0x18016c370) paints the well into the global mono/gray memory DCs
// with the mask colors 0xFF0062 / 0xEE0086 / 0x8800C6; the DCs are unmodeled.
// Symbol: ?CreateMask@CMFCToolBarImages@@IEAAXHHH@Z
extern "C" void MS_ABI impl__CreateMask_CMFCToolBarImages__IEAAXHHH_Z(
    CMFCToolBarImages* pThis, int nCount, int nBmpFirst, int nBmpLast)
{
    (void)nCount; (void)nBmpFirst; (void)nBmpLast;
    if (!pThis) return;
    // TODO(clean-room): transcribed partially -- retail (0x18016c370) builds
    // the light/shadow masks in the global memory DCs; not modeled.
}
// Retail (0x18016c510) runs BeginDrawImage/CreateMask/EndDrawImage and returns
// the mask bitmap built in the mono DC; conservative.
// Symbol: ?GetMask@CMFCToolBarImages@@QEAAPEAUHBITMAP__@@H@Z
extern "C" HBITMAP MS_ABI impl__GetMask_CMFCToolBarImages__QEAAPEAUHBITMAP____H_Z(
    CMFCToolBarImages* pThis, int iIndex)
{
    (void)iIndex;
    if (!pThis) return nullptr;
    // TODO(clean-room): transcribed partially -- retail (0x18016c510) returns
    // the mask bitmap produced in the global mono DC; not modeled.
    return nullptr;
}
// Retail (0x180171170) walks the bitmap pixels and grows an HRGN out of the
// runs of clrTransparent; conservative.
// Symbol: ?CreateRegionFromImage@CMFCToolBarImages@@SAPEAUHRGN__@@PEAUHBITMAP__@@K@Z
extern "C" HRGN MS_ABI impl__CreateRegionFromImage_CMFCToolBarImages__SAPEAUHRGN____PEAUHBITMAP____K_Z(
    HBITMAP hBitmap, unsigned long clrTransparent)
{
    (void)clrTransparent;
    if (!hBitmap) return nullptr;
    // TODO(clean-room): transcribed partially -- retail (0x180171170) scans
    // the bitmap and accumulates region rectangles; not modeled.
    return nullptr;
}
// Retail (0x180170c20) recolorizes the well from clr1 to clr2 through the
// 3D-color machinery; conservative.
// Symbol: ?AdaptColors@CMFCToolBarImages@@QEAAXKK@Z
extern "C" void MS_ABI impl__AdaptColors_CMFCToolBarImages__QEAAXKK_Z(
    CMFCToolBarImages* pThis, unsigned long clr1, unsigned long clr2)
{
    (void)clr1; (void)clr2;
    if (!pThis) return;
    // TODO(clean-room): transcribed partially -- retail (0x180170c20) recolorizes
    // the well pixels through unmodeled color helpers.
}
// Retail (0x172140) converts the well to 32bpp and rescales it; conservative.
// Symbol: ?ConvertTo32Bits@CMFCToolBarImages@@QEAAHK@Z
extern "C" int MS_ABI impl__ConvertTo32Bits_CMFCToolBarImages__QEAAHK_Z(
    CMFCToolBarImages* pThis, unsigned long clrDest)
{
    (void)clrDest;
    if (!pThis) return 0;
    // TODO(clean-room): transcribed partially -- retail (0x172140) converts
    // the well through the 32bpp helper (0x180056660); not modeled.
    return 0;
}
//=============================================================================
// Wave-2 bodies below.  Every mfc140u RVA quoted here is the export's entry,
// taken from the mfc140u export table by ordinal; the bodies were read in
// mfc140.dll (byte-identical) and spot-checked at the mfc140u entry.
//
// Member offsets used by these bodies, pinned against the harvested retail
// layout in include/openmfc/afxmfc.h through a derived accessor (the members
// are protected, so offsetof must name a derived class).
//=============================================================================
namespace {
struct TbiOffsetsProbe : CMFCToolBarImages {
    static constexpr size_t kCount() { return offsetof(TbiOffsetsProbe, m_iCount); }
    static constexpr size_t kUserImages() { return offsetof(TbiOffsetsProbe, m_bUserImagesList); }
    static constexpr size_t kModified() { return offsetof(TbiOffsetsProbe, m_bModified); }
    static constexpr size_t kIsTemporary() { return offsetof(TbiOffsetsProbe, m_bIsTemporary); }
    static constexpr size_t kSizeImage() { return offsetof(TbiOffsetsProbe, m_sizeImage); }
    static constexpr size_t kImageWell() { return offsetof(TbiOffsetsProbe, m_hbmImageWell); }
    static constexpr size_t kImageLight() { return offsetof(TbiOffsetsProbe, m_hbmImageLight); }
    static constexpr size_t kImageShadow() { return offsetof(TbiOffsetsProbe, m_hbmImageShadow); }
};
struct TbiOffsets {
    static constexpr size_t kCount = TbiOffsetsProbe::kCount();
    static constexpr size_t kUserImages = TbiOffsetsProbe::kUserImages();
    static constexpr size_t kModified = TbiOffsetsProbe::kModified();
    static constexpr size_t kIsTemporary = TbiOffsetsProbe::kIsTemporary();
    static constexpr size_t kSizeImage = TbiOffsetsProbe::kSizeImage();
    static constexpr size_t kImageWell = TbiOffsetsProbe::kImageWell();
    static constexpr size_t kImageLight = TbiOffsetsProbe::kImageLight();
    static constexpr size_t kImageShadow = TbiOffsetsProbe::kImageShadow();
};
static_assert(TbiOffsets::kCount       == 0x08, "m_iCount: mov %eax,0x8(%rbx) in UpdateCount");
static_assert(TbiOffsets::kUserImages  == 0x1C, "m_bUserImagesList: cmpl $0,0x1c(%rcx) in UpdateImage");
static_assert(TbiOffsets::kModified    == 0x20, "m_bModified: mov %esi,0x20(%rbx) in UpdateImage");
static_assert(TbiOffsets::kIsTemporary == 0x2C, "m_bIsTemporary: cmpl $0,0x2c(%rcx) in UpdateImage");
static_assert(TbiOffsets::kSizeImage   == 0x68, "m_sizeImage: 0x68/0x6c(%rbx) in SetSingleImage");
static_assert(TbiOffsets::kImageWell   == 0xA0, "m_hbmImageWell: mov 0xa0(%rcx),%rcx");
static_assert(TbiOffsets::kImageLight  == 0xA8, "m_hbmImageLight: lea 0xa8(%rbx) in UpdateImage");
static_assert(TbiOffsets::kImageShadow == 0xB0, "m_hbmImageShadow: add $0xb0,%rbx in UpdateImage");
static_assert(offsetof(CDC, m_hDC) == 0x08, "CDC::m_hDC: mov 0x8(%rbx),%rcx in TransparentBlt");

template <typename T>
inline T& TbiField(void* pThis, size_t off)
{
    return *reinterpret_cast<T*>(static_cast<char*>(pThis) + off);
}

// msimg32!TransparentBlt.  Retail calls it through the delay-load import slot
// 0x1803e9018 (mfc140u; MSIMG32.dll / TransparentBlt per the delay-import
// directory).  OpenMFC does not link msimg32, so it is resolved lazily -- the
// same pattern CDrawingManager.cpp uses for AlphaBlend.
typedef BOOL (WINAPI* PFN_TransparentBlt)(HDC, int, int, int, int, HDC, int, int, int, int, UINT);
PFN_TransparentBlt TbiGetTransparentBlt()
{
    static PFN_TransparentBlt s_pfn = nullptr;
    static bool s_bTried = false;
    if (!s_bTried) {
        s_bTried = true;
        HMODULE h = ::LoadLibraryW(L"msimg32.dll");
        if (h != nullptr)
            s_pfn = reinterpret_cast<PFN_TransparentBlt>(
                reinterpret_cast<void*>(::GetProcAddress(h, "TransparentBlt")));
    }
    return s_pfn;
}
} // namespace

extern "C" void MS_ABI impl__AfxThrowResourceException__YAXXZ();   // detail/MfcExceptionsSupport.cpp

// Retail RVA 0x16fef0 (mfc140u).  Transcribed:
//   GetObject(hbmp, sizeof(DIBSECTION) /*0x68*/, &ds) -- failure -> FALSE;
//   dsBm.bmBitsPixel != 32 or dsBm.bmBits == NULL -> FALSE;
//   n = bmWidth * bmHeight;
//   bAutoCheckPremlt: scan pixels, and if every pixel has B,G,R <= A the
//     bitmap is taken as already premultiplied -> TRUE without touching it;
//     the first pixel with a channel > A starts the conversion from pixel 0;
//   conversion: each of B,G,R = channel * A / 255 (retail multiplies by the
//     0x80808081 reciprocal and shifts by 39 -- exact floor division by 255
//     for these ranges); returns TRUE (also when n <= 0).
// Symbol: ?PreMultiplyAlpha@CMFCToolBarImages@@SAHPEAUHBITMAP__@@H@Z
extern "C" int MS_ABI impl__PreMultiplyAlpha_CMFCToolBarImages__SAHPEAUHBITMAP____H_Z(
    HBITMAP hbmp, int bAutoCheckPremlt)
{
    DIBSECTION ds;
    if (!::GetObjectW(hbmp, sizeof(DIBSECTION), &ds)) return FALSE;
    if (ds.dsBm.bmBitsPixel != 32 || ds.dsBm.bmBits == nullptr) return FALSE;

    const int nPixels = ds.dsBm.bmHeight * ds.dsBm.bmWidth;
    unsigned char* const bits = static_cast<unsigned char*>(ds.dsBm.bmBits);

    if (bAutoCheckPremlt) {
        bool bNeedConvert = false;
        for (int i = 0; i < nPixels; ++i) {
            const unsigned char* px = bits + static_cast<size_t>(i) * 4;
            if (px[2] > px[3] || px[1] > px[3] || px[0] > px[3]) {
                bNeedConvert = true;
                break;
            }
        }
        if (!bNeedConvert) return TRUE;
    }

    for (int i = 0; i < nPixels; ++i) {
        unsigned char* px = bits + static_cast<size_t>(i) * 4;
        const unsigned int a = px[3];
        px[2] = static_cast<unsigned char>((px[2] * a) / 255);
        px[1] = static_cast<unsigned char>((px[1] * a) / 255);
        px[0] = static_cast<unsigned char>((px[0] * a) / 255);
    }
    return TRUE;
}

// Retail RVA 0x16c060 (mfc140u).  LEFT A STUB (returns FALSE).  The body
// (read in full): no m_hbmImageWell -> FALSE before any lock.  Otherwise it
// enters the m_bMultiThreaded critical section (left held on success; the
// matching LeaveCriticalSection is in EndDrawImage), clears m_bStretch,
// requires m_dcMem, m_bmpMem and m_pBmpOriginal to be empty (else
// AfxThrowInvalidArgException), selects the well into the global glyph DC
// (mfc140u static 0x1803c38a8) storing the old bitmap in ds.hbmOldGlyphs,
// and when m_bCreateMonoDC creates a (cx+2)x(cy+2) 1bpp ds.hbmMono selected
// into the global mono DC (mfc140u static 0x1803c39b8).  m_sizeImageDest =
// sizeImageDest when both extents are > 0, else m_sizeImage.  When
// m_sizeImageDest != m_sizeImage, or when the image is not 32bpp and
// m_clrTransparent != (COLORREF)-1, it sets m_bStretch = (sizes differ),
// attaches CreateCompatibleDC(NULL) to m_dcMem and a (cx+2)x(cy+2) bitmap
// compatible with the screen DC to m_bmpMem, and stores the bitmap it
// displaces in m_pBmpOriginal.  bFadeInactive is never read.
// OpenMFC does not model those two global DCs (EndDrawImage above leaves the
// matching restore undone for the same reason), so the draw state cannot be
// produced.  Parameters: CSize by value is one 8-byte register (long long).
// Symbol: ?PrepareDrawImage@CMFCToolBarImages@@QEAAHAEAUtagAFXDrawState@@VCSize@@H@Z
extern "C" int MS_ABI impl__PrepareDrawImage_CMFCToolBarImages__QEAAHAEAUtagAFXDrawState__VCSize__H_Z(
    CMFCToolBarImages* pThis, tagAFXDrawState* pDrawState, long long sizeImageDest, int bFadeInactive)
{
    (void)pThis; (void)pDrawState; (void)sizeImageDest; (void)bFadeInactive;
    return FALSE;
}

// Retail RVA 0x171320 (mfc140u).  Transcribed: when m_hbmImageWell is set and
// GetObject(well, sizeof(BITMAP)) succeeds, m_sizeImage = (bmWidth, bmHeight)
// and m_iCount = 1; otherwise nothing is changed.
// Symbol: ?SetSingleImage@CMFCToolBarImages@@QEAAXXZ
extern "C" void MS_ABI impl__SetSingleImage_CMFCToolBarImages__QEAAXXZ(CMFCToolBarImages* pThis)
{
    HBITMAP hWell = TbiField<HBITMAP>(pThis, TbiOffsets::kImageWell);
    if (hWell == nullptr) return;
    BITMAP bmp;
    if (!::GetObjectW(hWell, sizeof(BITMAP), &bmp)) return;
    TbiField<int>(pThis, TbiOffsets::kSizeImage)     = bmp.bmWidth;   // m_sizeImage.cx
    TbiField<int>(pThis, TbiOffsets::kSizeImage + 4) = bmp.bmHeight;  // m_sizeImage.cy
    TbiField<int>(pThis, TbiOffsets::kCount)         = 1;             // m_iCount
}

// Retail RVA 0x171a30 (mfc140u).  LEFT A STUB (returns FALSE).  The body
// resamples the well with a filtered (weight-table) scaler.  It returns FALSE
// without a well, with m_nBitsPerPixel < 24, or with a scale of 0.0; it
// returns TRUE WITHOUT doing anything for a scale of 1.0, when the rounded
// new size equals m_sizeImage, when either size is not positive, or when
// m_iCount == 0 -- this stub returns FALSE in those no-op cases too.  The
// real path multiplies m_dblScale, builds source/destination DIBs through
// internal helpers (0x1800564b0 / 0x1800563d0 / 0x180169510 in mfc140) and
// per-axis contribution tables (0x180169170 in mfc140), then accumulates
// each output pixel in double precision.  Those internal helpers are not
// exported and are not reimplemented in OpenMFC, so a transcription is not
// attempted here.
// Symbol: ?SmoothResize@CMFCToolBarImages@@QEAAHN@Z
extern "C" int MS_ABI impl__SmoothResize_CMFCToolBarImages__QEAAHN_Z(
    CMFCToolBarImages* pThis, double dblImageScale)
{
    (void)pThis; (void)dblImageScale;
    return FALSE;
}

// Retail RVA 0x16f7a0 (mfc140u).  Static.  Transcribed:
//   cxDest = (nWidthDest == -1) ? nWidth : nWidthDest;
//   cyDest = (nHeightDest == -1) ? nHeight : nHeightDest;
//   when the m_bIsRTL static is 0: msimg32!TransparentBlt(hdcDest, nXDest,
//     nYDest, cxDest, cyDest, pDcSrc ? pDcSrc->m_hDC : NULL, nXSrc, nYSrc,
//     nWidth, nHeight, colorTransparent); a nonzero result returns at once.
//   Otherwise (RTL, or TransparentBlt failed) the classic mask path:
//     two compatible DCs off hdcDest (mask, image); a cxDest x cyDest
//     compatible bitmap in the image DC; copy the source in -- StretchBlt to
//     (nWidthDest, nHeightDest) when nWidthDest != -1 and the destination size
//     differs from (nWidth, nHeight), else BitBlt nWidth x nHeight -- SRCCOPY;
//     a 1bpp cxDest x cyDest bitmap in the mask DC; image bk = transparent,
//     BitBlt image->mask SRCCOPY; image bk = 0 / text = 0xFFFFFF, BitBlt
//     mask->image SRCAND; dest bk = 0xFFFFFF / text = 0, BitBlt mask->dest
//     SRCAND, then image->dest SRCPAINT.  The old bitmaps are reselected, both
//     bitmaps and both memory DCs deleted.  Retail does NOT restore hdcDest's
//     bk/text colours; neither does this.
// Retail wraps the handles in stack CDC/CBitmap objects (Attach, CDC::
// SetBkColor/SetTextColor on a freshly attached CDC, whose m_hAttribDC equals
// m_hDC); the raw GDI calls below are the same GDI calls without the
// handle-map traffic.  One retail side effect is therefore not reproduced:
// retail also wraps hdcDest itself in a stack CDC (CDC::Attach, then
// CDC::Detach at the end), which overwrites and then removes any permanent
// HDC-map entry an existing CDC (e.g. a CPaintDC) held for hdcDest.
// Deviation: retail's delay-load helper raises an exception when msimg32
// cannot be loaded; here a missing TransparentBlt falls through to the mask
// path instead.
// Symbol: ?TransparentBlt@CMFCToolBarImages@@KAXPEAUHDC__@@HHHHPEAVCDC@@HHKHH@Z
extern "C" void MS_ABI impl__TransparentBlt_CMFCToolBarImages__KAXPEAUHDC____HHHHPEAVCDC__HHKHH_Z(
    HDC hdcDest, int nXDest, int nYDest, int nWidth, int nHeight, CDC* pDcSrc,
    int nXSrc, int nYSrc, unsigned long colorTransparent, int nWidthDest, int nHeightDest)
{
    const int cxDest = (nWidthDest == -1) ? nWidth : nWidthDest;
    const int cyDest = (nHeightDest == -1) ? nHeight : nHeightDest;
    const HDC hdcSrc = (pDcSrc != nullptr) ? pDcSrc->m_hDC : nullptr;

    if (impl__m_bIsRTL_CMFCToolBarImages__1HA == 0) {
        PFN_TransparentBlt pfn = TbiGetTransparentBlt();
        if (pfn != nullptr &&
            pfn(hdcDest, nXDest, nYDest, cxDest, cyDest, hdcSrc,
                nXSrc, nYSrc, nWidth, nHeight, colorTransparent)) {
            return;
        }
    }

    HDC hdcMask = ::CreateCompatibleDC(hdcDest);
    HDC hdcImage = ::CreateCompatibleDC(hdcDest);

    HBITMAP hbmImage = ::CreateCompatibleBitmap(hdcDest, cxDest, cyDest);
    HGDIOBJ hOldImage = ::SelectObject(hdcImage, hbmImage);

    if (nWidthDest != -1 && (nWidthDest != nWidth || nHeightDest != nHeight)) {
        ::StretchBlt(hdcImage, 0, 0, nWidthDest, nHeightDest,
                     hdcSrc, nXSrc, nYSrc, nWidth, nHeight, SRCCOPY);
    } else {
        ::BitBlt(hdcImage, 0, 0, nWidth, nHeight, hdcSrc, nXSrc, nYSrc, SRCCOPY);
    }

    HBITMAP hbmMask = ::CreateBitmap(cxDest, cyDest, 1, 1, nullptr);
    HGDIOBJ hOldMask = ::SelectObject(hdcMask, hbmMask);

    ::SetBkColor(hdcImage, colorTransparent);
    ::BitBlt(hdcMask, 0, 0, cxDest, cyDest, hdcImage, 0, 0, SRCCOPY);

    ::SetBkColor(hdcImage, RGB(0, 0, 0));
    ::SetTextColor(hdcImage, RGB(255, 255, 255));
    ::BitBlt(hdcImage, 0, 0, cxDest, cyDest, hdcMask, 0, 0, SRCAND);

    ::SetBkColor(hdcDest, RGB(255, 255, 255));
    ::SetTextColor(hdcDest, RGB(0, 0, 0));
    ::BitBlt(hdcDest, nXDest, nYDest, cxDest, cyDest, hdcMask, 0, 0, SRCAND);
    ::BitBlt(hdcDest, nXDest, nYDest, cxDest, cyDest, hdcImage, 0, 0, SRCPAINT);

    if (hOldMask != nullptr) ::SelectObject(hdcMask, hOldMask);
    if (hOldImage != nullptr) ::SelectObject(hdcImage, hOldImage);

    if (hbmImage != nullptr) ::DeleteObject(hbmImage);
    if (hbmMask != nullptr) ::DeleteObject(hbmMask);
    if (hdcMask != nullptr) ::DeleteDC(hdcMask);
    if (hdcImage != nullptr) ::DeleteDC(hdcImage);
}

// Retail RVA 0x16d840 (mfc140u).  Transcribed: m_iCount = 0 when there is no
// well or GetObject(well, sizeof(BITMAP)) fails, else bmWidth / m_sizeImage.cx
// (signed division).
// Deviation: retail's idiv faults on m_sizeImage.cx == 0; that is undefined
// behaviour in C++, so the zero-width case stores 0 instead.
// Symbol: ?UpdateCount@CMFCToolBarImages@@IEAAXXZ
extern "C" void MS_ABI impl__UpdateCount_CMFCToolBarImages__IEAAXXZ(CMFCToolBarImages* pThis)
{
    int nCount = 0;
    HBITMAP hWell = TbiField<HBITMAP>(pThis, TbiOffsets::kImageWell);
    BITMAP bmp;
    if (hWell != nullptr && ::GetObjectW(hWell, sizeof(BITMAP), &bmp)) {
        const int cx = TbiField<int>(pThis, TbiOffsets::kSizeImage);   // m_sizeImage.cx
        nCount = (cx != 0) ? bmp.bmWidth / cx : 0;
    }
    TbiField<int>(pThis, TbiOffsets::kCount) = nCount;
}

// Retail RVA 0x16e260 (mfc140u).  Transcribed:
//   m_bIsTemporary or !m_bUserImagesList -> FALSE.
//   CWindowDC(NULL) (GetWindowDC(NULL); a NULL DC throws a resource
//   exception); two memory DCs compatible with it: the well DC and the
//   source DC.  Select m_hbmImageWell (if set) into the well DC and hbmp (if
//   set) into the source DC; BitBlt source (0,0) -> well
//   (m_sizeImage.cx * iImage, 0), m_sizeImage.cx x m_sizeImage.cy, SRCCOPY.
//   iImage is NOT range-checked by retail, and the BitBlt runs even when a
//   handle was NULL.  Reselect the old bitmaps, set m_bModified, delete and
//   clear m_hbmImageLight and m_hbmImageShadow, free the DCs, return TRUE.
// Retail uses stack CDC objects (Attach/Detach + DeleteDC); raw GDI calls are
// the same operations without the temporary handle-map entries.
// Symbol: ?UpdateImage@CMFCToolBarImages@@QEAAHHPEAUHBITMAP__@@@Z
extern "C" int MS_ABI impl__UpdateImage_CMFCToolBarImages__QEAAHHPEAUHBITMAP_____Z(
    CMFCToolBarImages* pThis, int iImage, HBITMAP hbmp)
{
    if (TbiField<BOOL>(pThis, TbiOffsets::kIsTemporary)) return FALSE;
    if (!TbiField<BOOL>(pThis, TbiOffsets::kUserImages)) return FALSE;

    HDC hdcScreen = ::GetWindowDC(nullptr);
    if (hdcScreen == nullptr) {
        impl__AfxThrowResourceException__YAXXZ();
        return FALSE;
    }

    HDC hdcSrc = ::CreateCompatibleDC(hdcScreen);   // created first in retail
    HDC hdcWell = ::CreateCompatibleDC(hdcScreen);

    HBITMAP hWell = TbiField<HBITMAP>(pThis, TbiOffsets::kImageWell);
    HGDIOBJ hOldWell = (hWell != nullptr) ? ::SelectObject(hdcWell, hWell) : nullptr;
    HGDIOBJ hOldSrc = (hbmp != nullptr) ? ::SelectObject(hdcSrc, hbmp) : nullptr;

    const int cx = TbiField<int>(pThis, TbiOffsets::kSizeImage);       // m_sizeImage.cx
    const int cy = TbiField<int>(pThis, TbiOffsets::kSizeImage + 4);   // m_sizeImage.cy
    ::BitBlt(hdcWell, cx * iImage, 0, cx, cy, hdcSrc, 0, 0, SRCCOPY);

    if (hOldWell != nullptr) ::SelectObject(hdcWell, hOldWell);
    if (hOldSrc != nullptr) ::SelectObject(hdcSrc, hOldSrc);

    TbiField<BOOL>(pThis, TbiOffsets::kModified) = TRUE;

    HBITMAP& hLight = TbiField<HBITMAP>(pThis, TbiOffsets::kImageLight);
    if (hLight != nullptr) ::DeleteObject(hLight);
    hLight = nullptr;
    HBITMAP& hShadow = TbiField<HBITMAP>(pThis, TbiOffsets::kImageShadow);
    if (hShadow != nullptr) ::DeleteObject(hShadow);
    hShadow = nullptr;

    if (hdcWell != nullptr) ::DeleteDC(hdcWell);
    if (hdcSrc != nullptr) ::DeleteDC(hdcSrc);
    ::ReleaseDC(nullptr, hdcScreen);
    return TRUE;
}

// Retail: export ordinal 14137 resolves to RVA 0x3a60 (mfc140u), a shared
// identical-code-folded body `mov $0x1,%eax; ret` (the same bytes serve
// CMFCBaseAccessibleObject::accDoDefaultAction).  So UpdateInternalImage
// ignores its argument and returns TRUE.
// Symbol: ?UpdateInternalImage@CMFCToolBarImages@@IEAAHH@Z
extern "C" int MS_ABI impl__UpdateInternalImage_CMFCToolBarImages__IEAAHH_Z(
    CMFCToolBarImages* pThis, int nIndex)
{
    (void)pThis; (void)nIndex;
    return TRUE;
}
