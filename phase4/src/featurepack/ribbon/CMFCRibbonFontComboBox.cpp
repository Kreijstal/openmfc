// CMFCRibbonFontComboBox — font catalog, selection, and drop-down rendering.

#define OPENMFC_APPCORE_IMPL

#include "detail/CMFCFontCatalog.h"
#include "detail/ManualSmallStubImplementationsSupport.h"

#include <commctrl.h>
#include <algorithm>
#include <cstring>
#include <cwchar>
#include <memory>
#include <new>

extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMFCRibbonFontComboBox__SAPEAUCRuntimeClass__XZ();
extern "C" BOOL impl__m_bDrawUsingFont_CMFCRibbonFontComboBox__2HA;
extern "C" void MS_ABI impl__AfxThrowOleException__YAXJ_Z(long);

static_assert(sizeof(CMFCRibbonFontComboBox) == 0x348);
static_assert(offsetof(CMFCRibbonFontComboBox, m_fontTypes) == 0x330);
static_assert(offsetof(CMFCRibbonFontComboBox, m_charSet) == 0x334);
static_assert(offsetof(CMFCRibbonFontComboBox, m_pitchAndFamily) == 0x335);
static_assert(offsetof(CMFCRibbonFontComboBox, m_fontImages) == 0x338);

namespace {
constexpr int kFontGlyphSize = 16;
constexpr int kFontTextInset = 22;
constexpr UINT kFontGlyphBitmap = 0x4268;

bool MatchesFont(const CMFCFontInfo& font, const wchar_t* name, BYTE charSet, BOOL exact) {
    const CString fullName = font.GetFullName();
    const bool matchesCharSet = charSet == DEFAULT_CHARSET || font.m_nCharSet == charSet;
    if (exact) {
        return fullName.CompareNoCase(name) == 0 ||
               (matchesCharSet && font.m_strName.CompareNoCase(name) == 0);
    }
    return matchesCharSet && _wcsnicmp(fullName, name, std::wcslen(name)) == 0;
}

void DrawFontGlyph(HDC dc, const CRect& row, HIMAGELIST images, int type, COLORREF color) {
    if (!(type & (DEVICE_FONTTYPE | TRUETYPE_FONTTYPE))) return;
    const bool device = (type & DEVICE_FONTTYPE) != 0;
    const int y = row.CenterPoint().y - kFontGlyphSize / 2;
    if (images && ::ImageList_Draw(images, device ? 0 : 1, dc, row.left, y, ILD_NORMAL)) return;
    if (device) {
        for (const CRect& part : {CRect(row.left + 4, y + 1, row.left + 12, y + 4),
                                 CRect(row.left + 2, y + 5, row.left + 14, y + 10),
                                 CRect(row.left + 4, y + 11, row.left + 12, y + 14)}) {
            ::ExtTextOutW(dc, 0, 0, ETO_OPAQUE, part, nullptr, 0, nullptr);
        }
    } else {
        CRect bounds(row.left, y, row.left + kFontGlyphSize, y + kFontGlyphSize);
        ::SetTextColor(dc, color);
        ::DrawTextW(dc, L"TT", 2, bounds, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    }
}

HFONT CreatePreviewFont(HDC dc, const CMFCFontInfo& font) {
    LOGFONTW metrics{};
    const HGDIOBJ selected = ::GetCurrentObject(dc, OBJ_FONT);
    if (!selected || !::GetObjectW(selected, sizeof(metrics), &metrics)) return nullptr;
    wcsncpy_s(metrics.lfFaceName, font.m_strName.GetString(), _TRUNCATE);
    if (font.m_nCharSet != DEFAULT_CHARSET) metrics.lfCharSet = font.m_nCharSet;
    metrics.lfHeight += metrics.lfHeight < 0 ? -4 : 4;
    return ::CreateFontIndirectW(&metrics);
}
}

BOOL& CMFCRibbonFontComboBox::m_bDrawUsingFont = impl__m_bDrawUsingFont_CMFCRibbonFontComboBox__2HA;

CMFCRibbonFontComboBox::CMFCRibbonFontComboBox() : CMFCRibbonComboBox(0, FALSE) {}

CMFCRibbonFontComboBox::CMFCRibbonFontComboBox(UINT id, int types, BYTE charSet, BYTE pitch, int width)
    : CMFCRibbonComboBox(id, TRUE, width) {
    BuildFonts(types, charSet, pitch);
}

CMFCRibbonFontComboBox::~CMFCRibbonFontComboBox() {
    ClearFontData();
}

CRuntimeClass* PASCAL CMFCRibbonFontComboBox::GetThisClass() {
    return impl__GetThisClass_CMFCRibbonFontComboBox__SAPEAUCRuntimeClass__XZ();
}
CRuntimeClass* CMFCRibbonFontComboBox::GetRuntimeClass() const { return GetThisClass(); }

void CMFCRibbonFontComboBox::BuildFonts(int types, BYTE charSet, BYTE pitch) {
    if (GetCount() && GetFontDesc(0) && m_fontTypes == types && m_charSet == charSet && m_pitchAndFamily == pitch) return;
    const int previousTypes = m_fontTypes;
    const BYTE previousCharSet = m_charSet;
    const BYTE previousPitch = m_pitchAndFamily;
    m_fontTypes = types;
    m_charSet = charSet;
    m_pitchAndFamily = pitch;
    try {
        RebuildFonts();
    } catch (...) {
        m_fontTypes = previousTypes;
        m_charSet = previousCharSet;
        m_pitchAndFamily = previousPitch;
        throw;
    }
}

void CMFCRibbonFontComboBox::RebuildFonts() {
    const auto fonts = openmfc::detail::CollectFontCatalog(m_fontTypes, m_charSet, m_pitchAndFamily);
    RemoveAllItems();
    try {
        for (const auto& font : fonts) {
            if (!(font.m_nType & m_fontTypes)) continue;
            // The toolbar catalog is sorted by full name and normally collapses equal face names.
            const auto count = std::count_if(fonts.begin(), fonts.end(), [&](const CMFCFontInfo& other) {
                return other.m_strName == font.m_strName;
            });
            const CString text = count > 1 ? font.GetFullName() : font.m_strName;
            if (FindItem(text) >= 0) continue;
            auto owned = std::make_unique<CMFCFontInfo>(font);
            AddItem(text, reinterpret_cast<DWORD_PTR>(owned.get()));
            owned.release();
        }
    } catch (...) {
        RemoveAllItems();
        throw;
    }
}

CMFCFontInfo* CMFCRibbonFontComboBox::GetFontDesc(int index) const {
    return reinterpret_cast<CMFCFontInfo*>(GetItemData(index));
}

BOOL CMFCRibbonFontComboBox::SetFont(const wchar_t* name, BYTE charSet, BOOL exact) {
    if (!name) return FALSE;
    if (const auto* current = GetFontDesc(); current && MatchesFont(*current, name, charSet, exact)) return TRUE;
    for (int i = 0; i < GetCount(); ++i) {
        const auto* font = GetFontDesc(i);
        if (font && MatchesFont(*font, name, charSet, exact)) return SelectItem(i);
    }
    return FALSE;
}

void CMFCRibbonFontComboBox::ClearFontData() {
    CloseDropDown();
    ClearItemValues();
}

void CMFCRibbonFontComboBox::ReleaseItemValue(DWORD_PTR value) {
    delete reinterpret_cast<CMFCFontInfo*>(value);
}

DWORD_PTR CMFCRibbonFontComboBox::CopyItemValue(const CMFCRibbonComboBox& source, DWORD_PTR value) const {
    if (!value || !source.IsKindOf(GetThisClass())) return 0;
    return reinterpret_cast<DWORD_PTR>(new CMFCFontInfo(*reinterpret_cast<const CMFCFontInfo*>(value)));
}

void CMFCRibbonFontComboBox::CopyFrom(const CMFCRibbonComboBox& source) {
    if (this == &source || !source.IsKindOf(GetThisClass())) return;
    const auto& fontSource = static_cast<const CMFCRibbonFontComboBox&>(source);
    CMFCRibbonComboBox::CopyFrom(source);
    m_fontTypes = fontSource.m_fontTypes;
    m_charSet = fontSource.m_charSet;
    m_pitchAndFamily = fontSource.m_pitchAndFamily;
}

void CMFCRibbonFontComboBox::DropDownList() {
    BuildFonts(m_fontTypes, m_charSet, m_pitchAndFamily);
    CMFCRibbonComboBox::DropDownList();
}

BOOL CMFCRibbonFontComboBox::OnDrawDropListItem(CDC* dc, int index, CMFCToolBarMenuButton* item, BOOL hot) {
    const auto* font = GetFontDesc(index);
    const auto* text = GetItem(index);
    const HDC hdc = dc ? dc->GetSafeHdc() : nullptr;
    if (!hdc || !item || !font || !text || item->m_rect.IsRectEmpty()) return FALSE;
    if (!m_fontImages.GetSafeHandle()) {
        m_fontImages.Create(kFontGlyphBitmap, kFontGlyphSize, 0, RGB(255, 255, 255));
    }
    const int saved = ::SaveDC(hdc);
    if (!saved) return FALSE;
    const COLORREF color = ::GetSysColor(hot ? COLOR_HIGHLIGHTTEXT : COLOR_MENUTEXT);
    ::SetTextColor(hdc, color);
    ::SetBkColor(hdc, color);
    ::SetBkMode(hdc, TRANSPARENT);
    CRect bounds = item->m_rect;
    bounds.left += 3;
    ::IntersectClipRect(hdc, bounds.left, bounds.top, bounds.right, bounds.bottom);
    DrawFontGlyph(hdc, bounds, m_fontImages.GetSafeHandle(), font->m_nType, color);
    bounds.left += kFontTextInset;
    const HFONT preview = m_bDrawUsingFont && font->m_nCharSet != SYMBOL_CHARSET ? CreatePreviewFont(hdc, *font) : nullptr;
    if (preview) ::SelectObject(hdc, preview);
    ::DrawTextW(hdc, text, -1, bounds, DT_SINGLELINE | DT_VCENTER);
    ::RestoreDC(hdc, saved);
    if (preview) ::DeleteObject(preview);
    return TRUE;
}

CSize CMFCRibbonFontComboBox::OnGetDropListItemSize(CDC*, int, CMFCToolBarMenuButton*, CSize sizeDefault) {
    sizeDefault.cx += kFontTextInset;
    return sizeDefault;
}

// Symbol: ??0CMFCRibbonFontComboBox@@IEAA@XZ
extern "C" void* MS_ABI impl___0CMFCRibbonFontComboBox__IEAA_XZ(void* storage) {
    return storage ? new (storage) CMFCRibbonFontComboBox() : nullptr;
}

// Symbol: ??0CMFCRibbonFontComboBox@@QEAA@IHEEH@Z
extern "C" void* MS_ABI impl___0CMFCRibbonFontComboBox__QEAA_IHEEH_Z(
    void* storage, UINT id, int types, BYTE charSet, BYTE pitch, int width) {
    return storage ? new (storage) CMFCRibbonFontComboBox(id, types, charSet, pitch, width) : nullptr;
}

// Symbol: ??1CMFCRibbonFontComboBox@@UEAA@XZ
extern "C" void MS_ABI impl___1CMFCRibbonFontComboBox__UEAA_XZ(CMFCRibbonFontComboBox* combo) {
    if (combo) combo->~CMFCRibbonFontComboBox();
}

// Symbol: ?BuildFonts@CMFCRibbonFontComboBox@@QEAAXHEE@Z
extern "C" void MS_ABI impl__BuildFonts_CMFCRibbonFontComboBox__QEAAXHEE_Z(
    CMFCRibbonFontComboBox* combo, int types, BYTE charSet, BYTE pitch) {
    if (combo) combo->BuildFonts(types, charSet, pitch);
}

// Symbol: ?ClearData@CMFCRibbonFontComboBox@@MEAAXXZ
extern "C" void MS_ABI impl__ClearData_CMFCRibbonFontComboBox__MEAAXXZ(CMFCRibbonFontComboBox* combo) {
    if (combo) combo->ClearFontData();
}

// Symbol: ?CopyFrom@CMFCRibbonFontComboBox@@MEAAXAEBVCMFCRibbonBaseElement@@@Z
extern "C" void MS_ABI impl__CopyFrom_CMFCRibbonFontComboBox__MEAAXAEBVCMFCRibbonBaseElement___Z(
    CMFCRibbonFontComboBox* combo, const CMFCRibbonBaseElement* source) {
    if (combo && source && source->IsKindOf(CMFCRibbonFontComboBox::GetThisClass())) {
        combo->CopyFrom(*static_cast<const CMFCRibbonFontComboBox*>(source));
    }
}

// Symbol: ?CreateObject@CMFCRibbonFontComboBox@@SAPEAVCObject@@XZ
extern "C" CObject* MS_ABI impl__CreateObject_CMFCRibbonFontComboBox__SAPEAVCObject__XZ() {
    return new CMFCRibbonFontComboBox();
}

// Symbol: ?DropDownList@CMFCRibbonFontComboBox@@MEAAXXZ
extern "C" void MS_ABI impl__DropDownList_CMFCRibbonFontComboBox__MEAAXXZ(CMFCRibbonFontComboBox* combo) {
    if (combo) combo->DropDownList();
}

// Symbol: ?GetFontsCount@CMFCRibbonFontComboBox@@IEAAHPEB_WAEBVCObList@@@Z
extern "C" int MS_ABI impl__GetFontsCount_CMFCRibbonFontComboBox__IEAAHPEB_WAEBVCObList___Z(
    void*, const wchar_t* name, const CObList* fonts) {
    if (!fonts) return 0;
    if (!name && !fonts->IsEmpty()) {
        impl__AfxThrowOleException__YAXJ_Z(E_FAIL);
        return 0;
    }
    int count = 0;
    for (auto pos = fonts->GetHeadPosition(); pos != CObList::POSITION(nullptr);) {
        const auto* font = static_cast<const CMFCFontInfo*>(fonts->GetNext(pos));
        if (font && std::wcscmp(font->m_strName, name) == 0) ++count;
    }
    return count;
}

// Symbol: ?OnDrawDropListItem@CMFCRibbonFontComboBox@@MEAAHPEAVCDC@@HPEAVCMFCToolBarMenuButton@@H@Z
extern "C" BOOL MS_ABI impl__OnDrawDropListItem_CMFCRibbonFontComboBox__MEAAHPEAVCDC__HPEAVCMFCToolBarMenuButton__H_Z(
    CMFCRibbonFontComboBox* combo, CDC* dc, int index, CMFCToolBarMenuButton* item, BOOL hot) {
    return combo && combo->OnDrawDropListItem(dc, index, item, hot);
}

// Symbol: ?OnGetDropListItemSize@CMFCRibbonFontComboBox@@MEAA?AVCSize@@PEAVCDC@@HPEAVCMFCToolBarMenuButton@@V2@@Z
extern "C" CSize* MS_ABI impl__OnGetDropListItemSize_CMFCRibbonFontComboBox__MEAA_AVCSize__PEAVCDC__HPEAVCMFCToolBarMenuButton__V2__Z(
    CMFCRibbonFontComboBox* combo, CSize* result, CDC* dc, int index, CMFCToolBarMenuButton* item, long long packed) {
    if (!result) return nullptr;
    CSize fallback;
    static_assert(sizeof(fallback) == sizeof(packed));
    std::memcpy(&fallback, &packed, sizeof(fallback));
    *result = combo ? combo->OnGetDropListItemSize(dc, index, item, fallback) : CSize(fallback.cx + kFontTextInset, fallback.cy);
    return result;
}

// Symbol: ?RebuildFonts@CMFCRibbonFontComboBox@@QEAAXXZ
extern "C" void MS_ABI impl__RebuildFonts_CMFCRibbonFontComboBox__QEAAXXZ(CMFCRibbonFontComboBox* combo) {
    if (combo) combo->RebuildFonts();
}

// Symbol: ?SetFont@CMFCRibbonFontComboBox@@QEAAHPEB_WEH@Z
extern "C" BOOL MS_ABI impl__SetFont_CMFCRibbonFontComboBox__QEAAHPEB_WEH_Z(
    CMFCRibbonFontComboBox* combo, const wchar_t* name, BYTE charSet, BOOL exact) {
    return combo && combo->SetFont(name, charSet, exact);
}
