#include <windows.h>
#include <cstring>

namespace {
struct IndexedBitmapInfo {
    BITMAPINFOHEADER header;
    RGBQUAD colors[256];
};

// Classic MFC only substitutes colors in the first sixteen palette entries.
void MapButtonColors(RGBQUAD* colors, unsigned count, BOOL monochrome) {
    constexpr struct {
        COLORREF source;
        int systemColor;
    } mapping[] = {
        {RGB(0, 0, 0), COLOR_BTNTEXT},
        {RGB(128, 128, 128), COLOR_BTNSHADOW},
        {RGB(192, 192, 192), COLOR_BTNFACE},
        {RGB(255, 255, 255), COLOR_BTNHIGHLIGHT},
    };

    for (unsigned i = 0; i < count && i < 16; ++i) {
        auto& entry = colors[i];
        if (entry.rgbReserved) continue;
        const COLORREF color = RGB(entry.rgbRed, entry.rgbGreen, entry.rgbBlue);
        for (const auto& mapped : mapping) {
            if (color != mapped.source) continue;
            if (!monochrome || mapped.systemColor != COLOR_BTNTEXT) {
                const COLORREF replacement = monochrome
                    ? RGB(255, 255, 255) : ::GetSysColor(mapped.systemColor);
                entry = RGBQUAD{GetBValue(replacement), GetGValue(replacement), GetRValue(replacement), 0};
            }
            break;
        }
    }
}

HBITMAP CreateMappedBitmap(const IndexedBitmapInfo& info, const BYTE* bits) {
    const int width = info.header.biWidth;
    const int height = info.header.biHeight;
    HDC screen = ::GetDC(nullptr);
    if (!screen) return nullptr;
    HBITMAP bitmap = ::CreateCompatibleBitmap(screen, width, height);
    HDC dc = bitmap ? ::CreateCompatibleDC(screen) : nullptr;
    bool drawn = false;
    if (dc) {
        HGDIOBJ previous = ::SelectObject(dc, bitmap);
        if (previous && previous != HGDI_ERROR) {
            const int result = ::StretchDIBits(dc, 0, 0, width, height, 0, 0, width, height, bits,
                reinterpret_cast<const BITMAPINFO*>(&info), DIB_RGB_COLORS, SRCCOPY);
            drawn = result != 0 && result != GDI_ERROR;
            ::SelectObject(dc, previous);
        }
        ::DeleteDC(dc);
    }
    ::ReleaseDC(nullptr, screen);
    if (drawn) return bitmap;
    if (bitmap) ::DeleteObject(bitmap);
    return nullptr;
}
}

// Symbol: ?AfxLoadSysColorBitmap@@YAPEAUHBITMAP__@@PEAUHINSTANCE__@@PEAUHRSRC__@@H@Z
extern "C" HBITMAP impl__AfxLoadSysColorBitmap__YAPEAUHBITMAP____PEAUHINSTANCE____PEAUHRSRC____H_Z(
    HINSTANCE module, HRSRC resource, BOOL monochrome) {
    if (!resource) return nullptr;
    const size_t resourceSize = ::SizeofResource(module, resource);
    if (resourceSize < sizeof(BITMAPINFOHEADER)) return nullptr;
    const HGLOBAL data = ::LoadResource(module, resource);
    const auto* bytes = data ? static_cast<const BYTE*>(::LockResource(data)) : nullptr;
    if (!bytes) return nullptr;

    IndexedBitmapInfo info{};
    auto& header = info.header;
    std::memcpy(&header, bytes, sizeof(header));
    const size_t headerSize = header.biSize;
    if (headerSize < sizeof(header) || headerSize > resourceSize || header.biPlanes != 1 ||
        header.biWidth <= 0 || header.biHeight <= 0 ||
        (header.biBitCount != 1 && header.biBitCount != 4 && header.biBitCount != 8)) {
        return nullptr;
    }
    const unsigned fullPalette = 1u << header.biBitCount;
    const unsigned colorCount = header.biClrUsed ? header.biClrUsed : fullPalette;
    if (colorCount > fullPalette || colorCount > (resourceSize - headerSize) / sizeof(RGBQUAD)) {
        return nullptr;
    }
    const size_t paletteSize = colorCount * sizeof(RGBQUAD);
    const size_t bitmapOffset = headerSize + paletteSize;
    const auto remaining = resourceSize - bitmapOffset;
    switch (header.biCompression) {
    case BI_RGB: {
        const auto stride = ((static_cast<ULONGLONG>(header.biWidth) * header.biBitCount + 31) / 32) * 4;
        const auto rows = static_cast<unsigned>(header.biHeight);
        if (stride > remaining / rows) return nullptr;
        header.biSizeImage = static_cast<DWORD>(stride * rows);
        break;
    }
    case BI_RLE4:
    case BI_RLE8:
        if (header.biBitCount != (header.biCompression == BI_RLE4 ? 4 : 8) ||
            !header.biSizeImage || header.biSizeImage > remaining) return nullptr;
        break;
    default:
        return nullptr;
    }

    // Resource pages can be shared and read-only. GDI reads a private palette.
    std::memcpy(info.colors, bytes + headerSize, paletteSize);
    header.biSize = sizeof(header);
    MapButtonColors(info.colors, colorCount, monochrome);
    return CreateMappedBitmap(info, bytes + bitmapOffset);
}
