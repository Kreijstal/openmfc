#pragma once

#include "openmfc/afx.h"

#ifndef MS_ABI
#define MS_ABI
#endif

namespace openmfc::detail::ccontrolbarinfo {

constexpr UINT kDefaultMRUWidth = 32767;

struct S_Ccontrolbarinfo {
    UINT m_nBarID = 0;
    BOOL m_bVisible = FALSE;
    BOOL m_bFloating = FALSE;
    BOOL m_bHorz = FALSE;
    BOOL m_bDockBar = FALSE;
    POINT m_pointPos{-1, -1};
    UINT m_nMRUWidth = kDefaultMRUWidth;
    BOOL m_bDocking = FALSE;
    UINT m_uMRUDockID = 0;
    RECT m_rectMRUDockPos{};
    DWORD m_dwMRUFloatStyle = 0;
    POINT m_ptMRUFloatPos{};
    CUIntArray m_arrBarID;
    void* m_pBar = nullptr;
};
static_assert(sizeof(S_Ccontrolbarinfo) == 0x78);
static_assert(offsetof(S_Ccontrolbarinfo, m_pointPos) == 0x14);
static_assert(offsetof(S_Ccontrolbarinfo, m_rectMRUDockPos) == 0x28);
static_assert(offsetof(S_Ccontrolbarinfo, m_arrBarID) == 0x48);
static_assert(offsetof(S_Ccontrolbarinfo, m_pBar) == 0x70);

void MakeBarSection(const wchar_t* profile, int index, wchar_t (&section)[256]);
}

using namespace openmfc::detail::ccontrolbarinfo;
