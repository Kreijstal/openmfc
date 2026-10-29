// Cached metrics and colors used by the classic window/view implementation.
#include "detail/AuxDataSupport.h"
#ifndef MS_ABI
#ifdef __GNUC__
#define MS_ABI __attribute__((ms_abi))
#else
#define MS_ABI
#endif
#endif

// ?afxData is data at RVA 0x3c32c0 (mfc140u). Preserve its existing 176-byte
// storage extent: the shadow has a 120-byte known prefix and an opaque tail.
// Retail's initializer entry RVA 0x2270 (mfc140u) stores 2 at AUX_DATA+0x10
// and +0x14 (instruction RVAs 0x22c8 and 0x22ce).
namespace openmfc { namespace detail {
struct AuxDataExportStorage {
    AuxData data;
    unsigned char trailing[176 - sizeof(AuxData)];
};
static_assert(offsetof(AuxDataExportStorage, data) == 0, "afxData shadow prefix offset");
static_assert(offsetof(AuxDataExportStorage, trailing) == sizeof(AuxData), "afxData opaque tail offset");
static_assert(sizeof(AuxDataExportStorage) == 176, "afxData export storage extent");
static_assert(offsetof(AuxData, cxBorder2) == 0x10, "retail AUX_DATA::cxBorder2 offset");
static_assert(offsetof(AuxData, cyBorder2) == 0x14, "retail AUX_DATA::cyBorder2 offset");
} }

// Symbol: ?afxData@@3UAUX_DATA@@A
extern "C" alignas(16) openmfc::detail::AuxDataExportStorage MS_ABI impl__afxData__3UAUX_DATA__A = {};

namespace openmfc { namespace detail {
AuxData& GetAuxData() { return impl__afxData__3UAUX_DATA__A.data; }
void UpdateAuxDataMetrics() {
    auto& data=GetAuxData();
    data.cxIcon=::GetSystemMetrics(SM_CXICON);
    data.cyIcon=::GetSystemMetrics(SM_CYICON);
    data.cxVScroll=static_cast<int>(static_cast<UINT>(::GetSystemMetrics(SM_CXVSCROLL))+1u);
    data.cyHScroll=static_cast<int>(static_cast<UINT>(::GetSystemMetrics(SM_CYHSCROLL))+1u);
    HDC dc=::GetDC(nullptr);
    data.cxPixelsPerInch=::GetDeviceCaps(dc,LOGPIXELSX);
    data.cyPixelsPerInch=::GetDeviceCaps(dc,LOGPIXELSY);
    ::ReleaseDC(nullptr,dc);
}
void UpdateAuxDataColors() {
    auto& data=GetAuxData();
    data.clrBtnFace=::GetSysColor(COLOR_BTNFACE);
    data.clrBtnShadow=::GetSysColor(COLOR_BTNSHADOW);
    data.clrBtnHilite=::GetSysColor(COLOR_BTNHIGHLIGHT);
    data.clrBtnText=::GetSysColor(COLOR_BTNTEXT);
    data.clrWindowFrame=::GetSysColor(COLOR_WINDOWFRAME);
    data.hbrBtnFace=::GetSysColorBrush(COLOR_BTNFACE);
    data.hbrWindowFrame=::GetSysColorBrush(COLOR_WINDOWFRAME);
}
void InitializeAuxData() {
    UpdateAuxDataMetrics();
    auto& data=GetAuxData();
    data.hbrBtnFace=nullptr;
    UpdateAuxDataColors();
    data.hcurWait=::LoadCursorW(nullptr,IDC_WAIT);
    data.hcurArrow=::LoadCursorW(nullptr,IDC_ARROW);
    data.hbmMenuDot=nullptr;
    data.cxBorder2=2;
    data.cyBorder2=2;
    data.reserved88=nullptr;
}
void CleanupAuxData() noexcept {
    auto& data=GetAuxData();
    if(data.hbmMenuDot) {
        ::DeleteObject(data.hbmMenuDot);
        data.hbmMenuDot=nullptr;
    }
}
} }
namespace {
struct AuxDataLifetime {
    AuxDataLifetime() { openmfc::detail::InitializeAuxData(); }
    ~AuxDataLifetime() { openmfc::detail::CleanupAuxData(); }
} auxDataLifetime;
}
