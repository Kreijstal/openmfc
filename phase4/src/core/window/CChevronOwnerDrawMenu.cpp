// CChevronOwnerDrawMenu — OpenMFC implementation.
// Sources: global_cchevronownerdrawmenu.cpp, manual_small_stub_implementations.cpp

#include "detail/CChevronOwnerDrawMenuSupport.h"
#include "detail/ManualSmallStubImplementationsSupport.h"

// Implemented in manual_small_stub_implementations.cpp; this unit had a second, unlinked copy.
// Symbol: ?DrawItem@CChevronOwnerDrawMenu@@UEAAXPEAUtagDRAWITEMSTRUCT@@@Z
// Symbol: ?MeasureItem@CChevronOwnerDrawMenu@@UEAAXPEAUtagMEASUREITEMSTRUCT@@@Z
// Symbol: ??0CChevronOwnerDrawMenu@@QEAA@XZ
extern "C" void* MS_ABI
impl___0CChevronOwnerDrawMenu__QEAA_XZ(void* pThis) {
    S_Cchevronownerdrawmenu* self = (S_Cchevronownerdrawmenu*)pThis;
    self->vfptr        = (void*)&g_CChevronOwnerDrawMenu_vtbl[0];
    self->m_hMenu      = NULL;              // CMenu default
    self->font_vfptr   = NULL;              // CFont subobject (m_hObject = NULL)
    self->font_hObject = NULL;
    return pThis;
}
// Symbol: ?DrawItem@CChevronOwnerDrawMenu@@UEAAXPEAUtagDRAWITEMSTRUCT@@@Z
// Symbol: ?MeasureItem@CChevronOwnerDrawMenu@@UEAAXPEAUtagMEASUREITEMSTRUCT@@@Z
extern "C" void MS_ABI impl__MeasureItem_CChevronOwnerDrawMenu__UEAAXPEAUtagMEASUREITEMSTRUCT___Z(
    void* pThis, void* pMis) {
    (void)pThis;
    auto* mis = static_cast<MEASUREITEMSTRUCT*>(pMis);
    if (mis == nullptr || mis->CtlType != ODT_MENU) {
        return;
    }

    UINT height = static_cast<UINT>(::GetSystemMetrics(SM_CYMENU));
    UINT checkWidth = static_cast<UINT>(::GetSystemMetrics(SM_CXMENUCHECK));
    LPCWSTR text = reinterpret_cast<LPCWSTR>(mis->itemData);
    UINT width = 0;
    HDC hdc = ::GetDC(nullptr);
    if (hdc) {
        HFONT font = GetChevronOwnerDrawMenuFont();
        HGDIOBJ old = ::SelectObject(hdc, font);
        if (text != nullptr && text[0] != 0) {
            SIZE size {};
            if (::GetTextExtentPoint32W(hdc, text, static_cast<int>(::lstrlenW(text)), &size)) {
                width = static_cast<UINT>(size.cx);
                height = static_cast<UINT>(std::max<int>(static_cast<int>(height), size.cy));
            } else if (size.cx > 0) {
                width = static_cast<UINT>(size.cx);
            }
        }
        TEXTMETRICW tm {};
        if (::GetTextMetricsW(hdc, &tm)) {
            height = static_cast<UINT>(std::max<int>(static_cast<int>(height), tm.tmHeight));
        }
        if (old) {
            ::SelectObject(hdc, old);
        }
        ::ReleaseDC(nullptr, hdc);
    }

    mis->itemWidth = width + checkWidth + 4;
    mis->itemHeight = height;
}

// CChevronOwnerDrawMenu's MSVC-layout vtable: defined here, next to the
// implementations it points at.
namespace openmfc { namespace detail { namespace cchevronownerdrawmenu {
extern void* const g_CChevronOwnerDrawMenu_vtbl[7] = {
    (void*)&slot_GetRuntimeClass_Cchevronownerdrawmenu,   // 0 CMenu::GetRuntimeClass
    (void*)&slot_vecDelDtor_Cchevronownerdrawmenu,        // 1 {dtor}
    (void*)&slot_Serialize_Cchevronownerdrawmenu,         // 2 CObject::Serialize
    (void*)&slot_AssertValid_Cchevronownerdrawmenu,       // 3 CObject::AssertValid
    (void*)&slot_Dump_Cchevronownerdrawmenu,              // 4 CObject::Dump
    (void*)&impl__DrawItem_CChevronOwnerDrawMenu__UEAAXPEAUtagDRAWITEMSTRUCT___Z,   // 5
    (void*)&impl__MeasureItem_CChevronOwnerDrawMenu__UEAAXPEAUtagMEASUREITEMSTRUCT___Z, // 6
};
} } }
