// CMFCRibbonInfoLoader — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp

#include "detail/ManualSmallStubImplementationsSupport.h"

#include <cstddef>
#include <cstdint>
#include <cwchar>

// CMFCRibbonInfoLoader is not declared in include/openmfc. Its constructor
// (RVA 0x10c6b0, mfc140u) places m_Info at +0x08 and clears m_hInstance at
// +0x10; Load (RVA 0x10c720, mfc140u) stores the selected module there.
namespace {
struct RibbonInfoLoaderLayout {
    void* vfptr;
    void* m_Info;
    HINSTANCE m_hInstance;
};
static_assert(offsetof(RibbonInfoLoaderLayout, m_Info) == 0x08, "loader info offset");
static_assert(offsetof(RibbonInfoLoaderLayout, m_hInstance) == 0x10, "loader instance offset");
static_assert(sizeof(RibbonInfoLoaderLayout) == 0x18, "loader size");

// XID's CString and resource value offsets are pinned by the retail accesses
// in LoadImageW (RVA 0x10c940, mfc140u) and RI_XID in CMFCRibbonInfo.cpp.
struct RibbonInfoLoaderXID {
    CString m_Name;
    unsigned int m_Value;
    unsigned int _pad_0c;
};
static_assert(offsetof(RibbonInfoLoaderXID, m_Value) == 0x08, "XID value offset");
static_assert(sizeof(RibbonInfoLoaderXID) == 0x10, "XID size");

// CMFCToolBarImages offsets follow include/openmfc/afxmfc.h: m_iCount +0x08,
// m_sizeImage +0x68, and m_hbmImageWell +0xa0.
struct RibbonLoaderImagesPrefix {
    std::uint8_t _pad_00[0x08];
    int m_iCount;
    std::uint8_t _pad_0c[0x5c];
    LONG m_sizeImageCx;
    LONG m_sizeImageCy;
    std::uint8_t _pad_70[0x30];
    HBITMAP m_hbmImageWell;
};
static_assert(offsetof(RibbonLoaderImagesPrefix, m_iCount) == 0x08, "image count offset");
static_assert(offsetof(RibbonLoaderImagesPrefix, m_sizeImageCx) == 0x68, "image size offset");
static_assert(offsetof(RibbonLoaderImagesPrefix, m_hbmImageWell) == 0xa0, "image well offset");
static_assert(sizeof(BITMAP) == 0x20, "retail GetObjectW bitmap buffer size");
} // namespace

extern "C" HINSTANCE MS_ABI impl__AfxFindResourceHandle__YAPEAUHINSTANCE____PEB_W0_Z(
    const wchar_t* name, const wchar_t* type);
extern "C" int MS_ABI impl__LoadFromBuffer_CMFCRibbonInfoLoader__UEAAHPEAEI_Z(
    void* pThis, void* buffer, unsigned int size);
extern "C" int MS_ABI impl__Load_CMFCRibbonInfoLoader__QEAAHPEB_W0PEAUHINSTANCE_____Z(
    void* pThis, const wchar_t* name, const wchar_t* type, void* hinst);
extern "C" int MS_ABI impl__LoadStr_CMFCToolBarImages__QEAAHPEB_WPEAUHINSTANCE____H_Z(
    CMFCToolBarImages* pThis, const wchar_t* name, HINSTANCE hInst, int bAdd);

// Symbol: ??0CMFCRibbonInfoLoader@@QEAA@AEAVCMFCRibbonInfo@@@Z
extern "C" void* MS_ABI impl___0CMFCRibbonInfoLoader__QEAA_AEAVCMFCRibbonInfo___Z(
    void* pThis, void* info) {
    // Retail RVA 0x10c6b0 (mfc140u) stores m_Info at +0x08, installs its
    // retail vftable, and clears m_hInstance at +0x10. The retail vftable
    // cannot be installed into an OpenMFC object, but the data fields are safe
    // to initialize using the verified layout.
    RibbonInfoLoaderLayout* loader = static_cast<RibbonInfoLoaderLayout*>(pThis);
    loader->m_Info = info;
    loader->m_hInstance = nullptr;
    return pThis;
}

// Symbol: ??1CMFCRibbonInfoLoader@@UEAA@XZ
extern "C" void MS_ABI impl___1CMFCRibbonInfoLoader__UEAA_XZ(void* pThis) {
    // Retail RVA 0x10c700 (mfc140u) only rewrites +0 with the loader's retail
    // vftable. OpenMFC has no modeled/relocatable vftable for this class, so
    // do not store a retail image address in an OpenMFC object.
    (void)pThis;
}

// Symbol: ?Load@CMFCRibbonInfoLoader@@QEAAHIPEB_WPEAUHINSTANCE__@@@Z
extern "C" int MS_ABI impl__Load_CMFCRibbonInfoLoader__QEAAHIPEB_WPEAUHINSTANCE_____Z(
    void* pThis, unsigned int resource, const wchar_t* type, void* hinst) {
    // Retail RVA 0x10c710 (mfc140u) truncates the resource ID to a WORD,
    // forms MAKEINTRESOURCEW, then tail-calls the string-name overload.
    return impl__Load_CMFCRibbonInfoLoader__QEAAHPEB_W0PEAUHINSTANCE_____Z(
        pThis, MAKEINTRESOURCEW(static_cast<WORD>(resource)), type, hinst);
}

// Symbol: ?Load@CMFCRibbonInfoLoader@@QEAAHPEB_W0PEAUHINSTANCE__@@@Z
extern "C" int MS_ABI impl__Load_CMFCRibbonInfoLoader__QEAAHPEB_W0PEAUHINSTANCE_____Z(
    void* pThis, const wchar_t* name, const wchar_t* type, void* hinst) {
    // Retail RVA 0x10c720 (mfc140u): reject null name/type; resolve a null
    // module with AfxFindResourceHandle; save the module at +0x10; then do
    // FindResourceW -> SizeofResource -> LoadResource -> LockResource and
    // pass the locked bytes and size to this loader's byte-buffer overload.
    if (name == nullptr || type == nullptr) {
        return FALSE;
    }

    HINSTANCE module = static_cast<HINSTANCE>(hinst);
    if (module == nullptr) {
        module = impl__AfxFindResourceHandle__YAPEAUHINSTANCE____PEB_W0_Z(name, type);
        if (module == nullptr) {
            return FALSE;
        }
    }

    static_cast<RibbonInfoLoaderLayout*>(pThis)->m_hInstance = module;
    HRSRC resource = ::FindResourceW(module, name, type);
    if (resource == nullptr) {
        return FALSE;
    }

    DWORD size = ::SizeofResource(module, resource);
    if (size == 0) {
        return FALSE;
    }
    HGLOBAL loaded = ::LoadResource(module, resource);
    if (loaded == nullptr) {
        return FALSE;
    }
    void* bytes = ::LockResource(loaded);
    if (bytes == nullptr) {
        return FALSE;
    }
    return impl__LoadFromBuffer_CMFCRibbonInfoLoader__UEAAHPEAEI_Z(pThis, bytes, size);
}

// Symbol: ?LoadFromBuffer@CMFCRibbonInfoLoader@@UEAAHPEAEI@Z
extern "C" int MS_ABI impl__LoadFromBuffer_CMFCRibbonInfoLoader__UEAAHPEAEI_Z(
    void* pThis, void* buffer, unsigned int size) {
    // Retail RVA 0x10c830 (mfc140u) rejects a null buffer or zero size,
    // dispatches CMFCRibbonInfo::Read(buffer, size) through m_Info at +0x08,
    // then walks the parsed ribbon images. OpenMFC's Read(LPBYTE, UINT) is
    // still a documented stub:
    // its CXMLParserRoot/MSXML front end is absent (CMFCRibbonInfo.cpp), so a
    // successful parse and the subsequent image walk cannot be implemented
    // faithfully here.
    (void)pThis;
    (void)buffer;
    (void)size;
    return FALSE;
}

// Symbol: ?LoadFromBuffer@CMFCRibbonInfoLoader@@UEAAHPEB_W@Z
extern "C" int MS_ABI impl__LoadFromBuffer_CMFCRibbonInfoLoader__UEAAHPEB_W_Z(
    void* pThis, const wchar_t* buffer) {
    // Retail RVA 0x10c7d0 (mfc140u) returns FALSE for null; otherwise it
    // computes wcslen(buffer) * 2 bytes (excluding the terminator) and forwards
    // to the byte-buffer overload.
    if (buffer == nullptr) {
        return FALSE;
    }
    const unsigned int size = static_cast<unsigned int>(std::wcslen(buffer) * 2);
    return impl__LoadFromBuffer_CMFCRibbonInfoLoader__UEAAHPEAEI_Z(
        pThis, const_cast<wchar_t*>(buffer), size);
}

// Symbol: ?LoadImageW@CMFCRibbonInfoLoader@@MEAAHAEBVXID@CMFCRibbonInfo@@AEAVCMFCToolBarImages@@H@Z
extern "C" int MS_ABI impl__LoadImageW_CMFCRibbonInfoLoader__MEAAHAEBVXID_CMFCRibbonInfo__AEAVCMFCToolBarImages__H_Z(
    void* pThis, const void* id, void* images, int single) {
    // Retail RVA 0x10c940 (mfc140u): empty XID succeeds; otherwise try its
    // numeric resource ID first, fall back to its nonempty string name when
    // no image well was loaded, and for bSingle copy the well bitmap's
    // dimensions into CMFCToolBarImages::m_sizeImage and set m_iCount to 1
    // when GetObjectW succeeds.
    const RibbonInfoLoaderXID* ribbonID = static_cast<const RibbonInfoLoaderXID*>(id);
    RibbonLoaderImagesPrefix* imageState = static_cast<RibbonLoaderImagesPrefix*>(images);
    const bool nameEmpty = ribbonID->m_Name.IsEmpty();
    if (nameEmpty && ribbonID->m_Value == 0) {
        return TRUE;
    }

    HINSTANCE module = static_cast<RibbonInfoLoaderLayout*>(pThis)->m_hInstance;
    if (ribbonID->m_Value != 0) {
        impl__LoadStr_CMFCToolBarImages__QEAAHPEB_WPEAUHINSTANCE____H_Z(
            static_cast<CMFCToolBarImages*>(images),
            MAKEINTRESOURCEW(static_cast<WORD>(ribbonID->m_Value)), module, FALSE);
    }
    if (imageState->m_hbmImageWell == nullptr && !nameEmpty) {
        impl__LoadStr_CMFCToolBarImages__QEAAHPEB_WPEAUHINSTANCE____H_Z(
            static_cast<CMFCToolBarImages*>(images), ribbonID->m_Name.GetString(), module, FALSE);
    }

    if (imageState->m_hbmImageWell != nullptr && single) {
        BITMAP bitmap{};
        if (::GetObjectW(imageState->m_hbmImageWell, sizeof(bitmap), &bitmap) != 0) {
            imageState->m_sizeImageCx = bitmap.bmWidth;
            imageState->m_sizeImageCy = bitmap.bmHeight;
            imageState->m_iCount = 1;
        }
    }
    return imageState->m_hbmImageWell != nullptr;
}

// Symbol: ?LoadImageW@CMFCRibbonInfoLoader@@UEAAHAEAVXImage@CMFCRibbonInfo@@H@Z
extern "C" int MS_ABI impl__LoadImageW_CMFCRibbonInfoLoader__UEAAHAEAVXImage_CMFCRibbonInfo__H_Z(
    void* pThis, void* image, int index) {
    // Retail RVA 0x10c9d0 (mfc140u) clears the embedded toolbar image object,
    // handles the empty primary ID, then selects the HDPI/primary resource and
    // applies DPI-dependent image resizing. The XImage layout and ID-based
    // resource loader are modeled here, but the full global-scale selection
    // and raster-resize path (including helpers at RVAs 0x16d840 and 0x171a30
    // in mfc140u) is not reproduced; leave this pre-existing stub rather than
    // guess at scaling behavior.
    (void)pThis;
    (void)image;
    (void)index;
    return FALSE;
}
