// Cached metrics and colors used by the classic window/view implementation.
#include "detail/AuxDataSupport.h"
#ifndef MS_ABI
#ifdef __GNUC__
#define MS_ABI __attribute__((ms_abi))
#else
#define MS_ABI
#endif
#endif

// Retain the harvested export's storage bound, including its trailing padding.
// Symbol: ?afxData@@3UAUX_DATA@@A
extern "C" alignas(16) unsigned char MS_ABI impl__afxData__3UAUX_DATA__A[176] = {};

namespace openmfc { namespace detail {
AuxData& GetAuxData() { return *reinterpret_cast<AuxData*>(impl__afxData__3UAUX_DATA__A); }
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
