// OpenMFC class-specific implementations and fallback stubs for unresolved symbols

#include <cstddef>
#include <cstdint>
#include <cstring>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

extern "C" void MS_ABI impl__Initialize_AFX_GLOBAL_DATA__QEAAXXZ(void* pThis);
extern "C" unsigned char impl__afxGlobalData__3UAFX_GLOBAL_DATA__A[720];

namespace {

constexpr std::size_t kGlobalDataInitGate = 0x000;
constexpr std::size_t kGlobalDataRibbonScale = 0x2b8;
constexpr std::size_t kGlobalDataIsRibbonScale = 0x2c0;

int ReadGlobalDataInt(std::size_t offset) {
    int value;
    std::memcpy(&value, impl__afxGlobalData__3UAFX_GLOBAL_DATA__A + offset, sizeof(value));
    return value;
}

void EnsureGlobalDataInitialized() {
    if (ReadGlobalDataInt(kGlobalDataInitGate) == 0) {
        impl__Initialize_AFX_GLOBAL_DATA__QEAAXXZ(impl__afxGlobalData__3UAFX_GLOBAL_DATA__A);
        const int initialized = 1;
        std::memcpy(impl__afxGlobalData__3UAFX_GLOBAL_DATA__A + kGlobalDataInitGate,
                    &initialized, sizeof(initialized));
    }
}

double RibbonImageScale() {
    if (ReadGlobalDataInt(kGlobalDataIsRibbonScale) == 0) {
        return 1.0;
    }
    double scale;
    std::memcpy(&scale, impl__afxGlobalData__3UAFX_GLOBAL_DATA__A + kGlobalDataRibbonScale,
                sizeof(scale));
    return scale;
}

struct RibbonRecentFilesRect {
    std::int32_t left;
    std::int32_t top;
    std::int32_t right;
    std::int32_t bottom;
};

struct RibbonRecentFilesSize {
    std::int32_t cx;
    std::int32_t cy;
};

// CMFCRibbonRecentFilesList is not declared in include/openmfc. These fields
// are the retail CMFCRibbonBaseElement rect, the opaque context passed to the
// first child callback, and the CArray data/size at the front of the inherited
// CMFCRibbonButtonsGroup button array.
struct RibbonRecentFilesListShadow {
    char beforeRect[0xc8];
    RibbonRecentFilesRect rect;
    char beforeContext[0xf8 - 0xd8];
    void* context;
    char beforeButtons[0x188 - 0x100];
    void** buttons;
    std::int64_t buttonCount;
};

static_assert(sizeof(void*) == 8, "ribbon shadow is for the x64 MSVC ABI");
static_assert(sizeof(RibbonRecentFilesRect) == 0x10, "retail RECT size");
static_assert(sizeof(RibbonRecentFilesSize) == 0x08, "retail CSize size");
static_assert(offsetof(RibbonRecentFilesListShadow, rect) == 0xc8,
              "retail ribbon element rect offset");
static_assert(offsetof(RibbonRecentFilesListShadow, context) == 0xf8,
              "retail recent-files child context offset");
static_assert(offsetof(RibbonRecentFilesListShadow, buttons) == 0x188,
              "retail CMFCRibbonButtonsGroup CArray data offset");
static_assert(offsetof(RibbonRecentFilesListShadow, buttonCount) == 0x190,
              "retail CMFCRibbonButtonsGroup CArray size offset");

} // namespace

// Retail mfc140u RVA 0x10f2e0 allocates 0x670 bytes, calls the
// CMFCRibbonButtonsGroup constructor (RVA 0xe9f30), installs the derived
// vtable, then initializes the label through an internal text routine (RVA
// 0xfe50). OpenMFC has no CMFCRibbonRecentFilesList declaration/vtable, so
// constructing this derived object here would require inventing its vtable.
// Symbol: ?CreateObject@CMFCRibbonRecentFilesList@@SAPEAVCObject@@XZ
extern "C" void* MS_ABI impl__CreateObject_CMFCRibbonRecentFilesList__SAPEAVCObject__XZ() {
    return nullptr;
}

// Retail mfc140u RVA 0x10f5d0 reads the current directory and a global recent-
// file collection, normalizes paths, and constructs/inserts ribbon entries.
// The associated global collection and temporary string/list infrastructure
// are not modeled in this translation unit, so this remains a documented stub.
// Symbol: ?FillList@CMFCRibbonRecentFilesList@@QEAAXXZ
extern "C" void MS_ABI impl__FillList_CMFCRibbonRecentFilesList__QEAAXXZ(void* /*pThis*/) {}

// Retail mfc140u RVA 0x10f480 calls child slots +0x310 and +0x1f8, returns a
// height of 4 plus each child height and 8 pixels, and sets width to 300 at
// scale 1.0 or trunc(scale * 300) otherwise. The scale comes from the shared
// AFX_GLOBAL_DATA blob (+0x2c0 flag, +0x2b8 double); this reads the OpenMFC
// blob. Its Initialize implementation does not populate +0x2c0, unlike the
// retail constructor (mfc140u RVA 0x6a590), so the current blob selects 1.0.
// Symbol: ?GetRegularSize@CMFCRibbonRecentFilesList@@MEAA?AVCSize@@PEAVCDC@@@Z
extern "C" void* MS_ABI impl__GetRegularSize_CMFCRibbonRecentFilesList__MEAA_AVCSize__PEAVCDC___Z(
    void* pThis, void* pResult, void* pDC) {
    auto* pList = static_cast<RibbonRecentFilesListShadow*>(pThis);
    auto* result = static_cast<RibbonRecentFilesSize*>(pResult);
    std::uint32_t height = 4;

    for (std::int32_t i = 0; i < pList->buttonCount; ++i) {
        void* pButton = pList->buttons[i];
        void** vtable = *reinterpret_cast<void***>(pButton);
        using ChildCallback = void (MS_ABI *)(void*, void*);
        using ChildSize = RibbonRecentFilesSize* (MS_ABI *)(void*, RibbonRecentFilesSize*, void*);
        auto calcChild = reinterpret_cast<ChildCallback>(vtable[0x310 / sizeof(void*)]);
        auto getSize = reinterpret_cast<ChildSize>(vtable[0x1f8 / sizeof(void*)]);

        calcChild(pButton, pDC);
        RibbonRecentFilesSize childSize{};
        getSize(pButton, &childSize, pDC);
        height += static_cast<std::uint32_t>(childSize.cy) + 8;
    }

    EnsureGlobalDataInitialized();
    const double scale = RibbonImageScale();
    if (scale == 1.0) {
        result->cx = 300;
    } else {
        const double scaledWidth = scale * 300.0;
        // Retail CVTTSD2SI yields INT_MIN for NaN and out-of-range values.
        if (scaledWidth != scaledWidth || scaledWidth < -2147483648.0 ||
            scaledWidth >= 2147483648.0) {
            const std::uint32_t invalidConversion = 0x80000000u;
            std::memcpy(&result->cx, &invalidConversion, sizeof(invalidConversion));
        } else {
            result->cx = static_cast<std::int32_t>(scaledWidth);
        }
    }
    std::memcpy(&result->cy, &height, sizeof(height));
    return pResult;
}

// Retail mfc140u RVA 0x10f340 prepares each child with vtable slot +0x2e8
// and +0x310, obtains its CSize through +0x1f8, assigns a rect at child +0xc8,
// and calls child slot +0x258. The offsets are pinned above and this loop
// transcribes those calls and rectangle updates.
// Symbol: ?OnAfterChangeRect@CMFCRibbonRecentFilesList@@MEAAXPEAVCDC@@@Z
extern "C" void MS_ABI impl__OnAfterChangeRect_CMFCRibbonRecentFilesList__MEAAXPEAVCDC___Z(
    void* pThis, void* pDC) {
    auto* pList = static_cast<RibbonRecentFilesListShadow*>(pThis);
    std::int32_t top = pList->rect.top + 2;

    for (std::int32_t i = 0; i < pList->buttonCount; ++i) {
        void* pButton = pList->buttons[i];
        void** vtable = *reinterpret_cast<void***>(pButton);

        using ChildContext = void (MS_ABI *)(void*, void*);
        using ChildSize = RibbonRecentFilesSize* (MS_ABI *)(void*, RibbonRecentFilesSize*, void*);
        auto setContext = reinterpret_cast<ChildContext>(vtable[0x2e8 / sizeof(void*)]);
        auto calcChild = reinterpret_cast<ChildContext>(vtable[0x310 / sizeof(void*)]);
        auto getSize = reinterpret_cast<ChildSize>(vtable[0x1f8 / sizeof(void*)]);
        auto afterChange = reinterpret_cast<ChildContext>(vtable[0x258 / sizeof(void*)]);

        setContext(pButton, pList->context);
        calcChild(pButton, pDC);

        RibbonRecentFilesSize size{};
        getSize(pButton, &size, pDC);

        RibbonRecentFilesRect rect = pList->rect;
        // Retail calls InflateRect(&rect, -1, 0), shrinking the horizontal
        // bounds by one pixel on each side.
        ++rect.left;
        --rect.right;
        rect.top = top;
        rect.bottom = rect.top + size.cy + 8;
        *reinterpret_cast<RibbonRecentFilesRect*>(
            static_cast<char*>(pButton) + 0xc8) = rect;
        afterChange(pButton, pDC);
        top = rect.bottom;
    }
}

// Retail mfc140u RVA 0x10f8c0 calls USER32!IsRectEmpty on the rect at +0xc8,
// then, only for a nonempty rect, invokes every button's virtual slot +0x308
// with pDC. The Win32 empty-rect check is inlined with RECT semantics to avoid
// introducing a new OS import; the observed loop and early return are retained.
// Symbol: ?OnDraw@CMFCRibbonRecentFilesList@@MEAAXPEAVCDC@@@Z
extern "C" void MS_ABI impl__OnDraw_CMFCRibbonRecentFilesList__MEAAXPEAVCDC___Z(
    void* pThis, void* pDC) {
    auto* pList = static_cast<RibbonRecentFilesListShadow*>(pThis);
    const RibbonRecentFilesRect& rect = pList->rect;
    if (rect.left >= rect.right || rect.top >= rect.bottom) {
        return;
    }

    for (std::int32_t i = 0; i < pList->buttonCount; ++i) {
        void* pButton = pList->buttons[i];
        void** vtable = *reinterpret_cast<void***>(pButton);
        using DrawChild = void (MS_ABI *)(void*, void*);
        auto draw = reinterpret_cast<DrawChild>(vtable[0x308 / sizeof(void*)]);
        draw(pButton, pDC);
    }
}

// The retail export has no resolved RVA in the mfc140u symbol map. Without
// that body, the key handling order/dispatch cannot be transcribed safely.
// Symbol: ?OnMenuKey@CMFCRibbonRecentFilesList@@MEAAHI@Z
extern "C" int MS_ABI impl__OnMenuKey_CMFCRibbonRecentFilesList__MEAAHI_Z(
    void* /*pThis*/, unsigned int /*nUpperChar*/) {
    return 0;
}
