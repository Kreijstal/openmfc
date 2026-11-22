#define OPENMFC_APPCORE_IMPL
#include "detail/DockStateSupport.h"
#include "detail/AppProfile.h"
#include <cstdlib>
#include <cwchar>
#include <new>

extern "C" void* impl___2_YAPEAX_K_Z(size_t);
extern "C" CWinApp* impl__AfxGetApp__YAPEAVCWinApp__XZ();
extern "C" DWORD impl__GetVersion_CDockState__QEAAKXZ(void* state);
extern "C" void impl__ScalePoint_CDockState__QEAAXAEAVCPoint___Z(void* state, POINT* point);
extern "C" void impl__ScaleRectPos_CDockState__QEAAXAEAVCRect___Z(void* state, RECT* rect);

namespace {
void ClipFloatingBar(POINT& point) {
    const int frameX = GetSystemMetrics(SM_CXFRAME);
    const int frameY = GetSystemMetrics(SM_CYFRAME) + GetSystemMetrics(SM_CYSMCAPTION);
    const LONG left = GetSystemMetrics(SM_XVIRTUALSCREEN) + frameX;
    const LONG right = GetSystemMetrics(SM_CXVIRTUALSCREEN) - frameX;
    const LONG top = GetSystemMetrics(SM_YVIRTUALSCREEN) + frameY;
    const LONG bottom = GetSystemMetrics(SM_CYVIRTUALSCREEN) - frameY;
    if (point.x < left) point.x = left;
    if (point.x > right) point.x = right;
    if (point.y < top) point.y = top;
    if (point.y > bottom) point.y = bottom;
    if (!MonitorFromPoint(point, MONITOR_DEFAULTTONULL)) point = {frameX, frameY};
}
}

// Symbol: ??0CControlBarInfo@@QEAA@XZ
extern "C" void* impl___0CControlBarInfo__QEAA_XZ(void* storage) {
    return ::new (storage) S_Ccontrolbarinfo;
}

namespace openmfc::detail::dock {
S_Ccontrolbarinfo* NewBarInfo() {
    void* memory = impl___2_YAPEAX_K_Z(sizeof(S_Ccontrolbarinfo));
    return memory ? ::new (memory) S_Ccontrolbarinfo : nullptr;
}

void DeleteBarInfo(S_Ccontrolbarinfo* info) {
    if (info) {
        info->~S_Ccontrolbarinfo();
        std::free(info);
    }
}
}

// Symbol: ?Serialize@CControlBarInfo@@QEAAXAEAVCArchive@@PEAVCDockState@@@Z
extern "C" void impl__Serialize_CControlBarInfo__QEAAXAEAVCArchive__PEAVCDockState___Z(
    void* self, void* archive, void* state) {
    auto& bar = *static_cast<S_Ccontrolbarinfo*>(self);
    auto& ar = *static_cast<CArchive*>(archive);
    if (ar.IsStoring()) {
        ar << bar.m_nBarID << bar.m_bVisible << bar.m_bFloating << bar.m_bHorz;
        const POINT position = bar.m_pointPos;
        ar.Write(&position, sizeof(position));
        if (impl__GetVersion_CDockState__QEAAKXZ(state) > 1) {
            ar << bar.m_nMRUWidth << bar.m_bDocking;
            if (bar.m_bDocking) {
                ar << bar.m_uMRUDockID;
                ar.Write(&bar.m_rectMRUDockPos, sizeof(RECT));
                ar << bar.m_dwMRUFloatStyle;
                const POINT floatPosition = bar.m_ptMRUFloatPos;
                ar.Write(&floatPosition, sizeof(floatPosition));
            }
        }
        ar << static_cast<WORD>(bar.m_arrBarID.GetSize());
        if (!bar.m_arrBarID.IsEmpty()) {
            if (bar.m_arrBarID.GetSize() < 0) AfxThrowInvalidArgException();
            ar.Write(bar.m_arrBarID.GetData(), static_cast<UINT>(bar.m_arrBarID.GetSize()) * sizeof(UINT));
        }
        return;
    }

    ar >> bar.m_nBarID >> bar.m_bVisible >> bar.m_bFloating >> bar.m_bHorz;
    ar.EnsureRead(&bar.m_pointPos, sizeof(POINT));
    if (impl__GetVersion_CDockState__QEAAKXZ(state) > 1) {
        impl__ScalePoint_CDockState__QEAAXAEAVCPoint___Z(state, &bar.m_pointPos);
        ar >> bar.m_nMRUWidth >> bar.m_bDocking;
        if (bar.m_bDocking) {
            ar >> bar.m_uMRUDockID;
            ar.EnsureRead(&bar.m_rectMRUDockPos, sizeof(RECT));
            impl__ScaleRectPos_CDockState__QEAAXAEAVCRect___Z(state, &bar.m_rectMRUDockPos);
            ar >> bar.m_dwMRUFloatStyle;
            ar.EnsureRead(&bar.m_ptMRUFloatPos, sizeof(POINT));
            impl__ScalePoint_CDockState__QEAAXAEAVCPoint___Z(state, &bar.m_ptMRUFloatPos);
        }
    }
    WORD count;
    ar >> count;
    bar.m_arrBarID.SetSize(count);
    if (count) ar.EnsureRead(bar.m_arrBarID.GetData(), UINT(count) * sizeof(UINT));
}

// Symbol: ?LoadState@CControlBarInfo@@QEAAHPEB_WHPEAVCDockState@@@Z
extern "C" BOOL impl__LoadState_CControlBarInfo__QEAAHPEB_WHPEAVCDockState___Z(
    void* self, const wchar_t* profile, int index, void* state) {
    auto* app = impl__AfxGetApp__YAPEAVCWinApp__XZ();
    if (!app) return FALSE;
    auto& bar = *static_cast<S_Ccontrolbarinfo*>(self);
    wchar_t section[256];
    MakeBarSection(profile, index, section);
    auto read = [&](const wchar_t* entry, int defaultValue = 0) {
        return impl__GetProfileIntW_CWinApp__UEAAIPEB_W0H_Z(app, section, entry, defaultValue);
    };

    bar.m_nBarID = read(L"BarID");
    bar.m_bVisible = read(L"Visible", TRUE);
    bar.m_bHorz = read(L"Horz", TRUE);
    bar.m_bFloating = read(L"Floating");
    const LONG y = read(L"YPos", -1);
    bar.m_pointPos = {static_cast<LONG>(read(L"XPos", -1)), y};
    impl__ScalePoint_CDockState__QEAAXAEAVCPoint___Z(state, &bar.m_pointPos);
    if (bar.m_bFloating) ClipFloatingBar(bar.m_pointPos);

    bar.m_nMRUWidth = read(L"MRUWidth", kDefaultMRUWidth);
    bar.m_bDocking = read(L"Docking");
    if (bar.m_bDocking) {
        bar.m_uMRUDockID = read(L"MRUDockID");
        const LONG bottom = read(L"MRUDockBottomPos");
        const LONG right = read(L"MRUDockRightPos");
        const LONG top = read(L"MRUDockTopPos");
        bar.m_rectMRUDockPos = {static_cast<LONG>(read(L"MRUDockLeftPos")), top, right, bottom};
        impl__ScaleRectPos_CDockState__QEAAXAEAVCRect___Z(state, &bar.m_rectMRUDockPos);
        bar.m_dwMRUFloatStyle = read(L"MRUFloatStyle");
        const LONG floatY = read(L"MRUFloatYPos");
        bar.m_ptMRUFloatPos = {static_cast<LONG>(read(L"MRUFloatXPos")), floatY};
        impl__ScalePoint_CDockState__QEAAXAEAVCPoint___Z(state, &bar.m_ptMRUFloatPos);
    }
    const int count = read(L"Bars");
    for (int i = 0; i < count; ++i) {
        wchar_t entry[16];
        swprintf_s(entry, L"Bar#%d", i);
        bar.m_arrBarID.Add(read(entry));
    }
    return bar.m_nBarID != 0;
}

// Symbol: ?SaveState@CControlBarInfo@@QEAAHPEB_WH@Z
extern "C" BOOL impl__SaveState_CControlBarInfo__QEAAHPEB_WH_Z(
    void* self, const wchar_t* profile, int index) {
    auto& bar = *static_cast<S_Ccontrolbarinfo*>(self);
    wchar_t section[256];
    MakeBarSection(profile, index, section);
    auto* app = impl__AfxGetApp__YAPEAVCWinApp__XZ();
    if (!app) return FALSE;
    impl__WriteProfileStringW_CWinApp__UEAAHPEB_W00_Z(app, section, nullptr, nullptr);
    if (bar.m_bDockBar && bar.m_bVisible && !bar.m_bFloating &&
        bar.m_pointPos.x == -1 && bar.m_pointPos.y == -1 && bar.m_arrBarID.GetSize() <= 1)
        return FALSE;

    auto write = [&](const wchar_t* entry, int value) {
        impl__WriteProfileInt_CWinApp__UEAAHPEB_W0H_Z(app, section, entry, value);
    };
    write(L"BarID", bar.m_nBarID);
    if (!bar.m_bVisible) write(L"Visible", FALSE);
    if (bar.m_bFloating) {
        write(L"Horz", bar.m_bHorz);
        write(L"Floating", bar.m_bFloating);
    }
    if (bar.m_pointPos.x != -1) write(L"XPos", bar.m_pointPos.x);
    if (bar.m_pointPos.y != -1) write(L"YPos", bar.m_pointPos.y);
    if (bar.m_nMRUWidth != kDefaultMRUWidth) write(L"MRUWidth", bar.m_nMRUWidth);
    if (bar.m_bDocking) {
        write(L"Docking", bar.m_bDocking);
        write(L"MRUDockID", bar.m_uMRUDockID);
        write(L"MRUDockLeftPos", bar.m_rectMRUDockPos.left);
        write(L"MRUDockTopPos", bar.m_rectMRUDockPos.top);
        write(L"MRUDockRightPos", bar.m_rectMRUDockPos.right);
        write(L"MRUDockBottomPos", bar.m_rectMRUDockPos.bottom);
        write(L"MRUFloatStyle", bar.m_dwMRUFloatStyle);
        write(L"MRUFloatXPos", bar.m_ptMRUFloatPos.x);
        write(L"MRUFloatYPos", bar.m_ptMRUFloatPos.y);
    }
    const INT_PTR count = bar.m_arrBarID.GetSize();
    if (count > 1) {
        write(L"Bars", static_cast<int>(count));
        for (INT_PTR i = 0; i < count; ++i) {
            wchar_t entry[16];
            swprintf_s(entry, L"Bar#%d", static_cast<int>(i));
            write(entry, bar.m_arrBarID.GetAt(i));
        }
    }
    return TRUE;
}
