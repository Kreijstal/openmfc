// CPictureHolder — OpenMFC implementation.
// Sources: collections_strings.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/CollectionsStringsSupport.h"

// Symbol: ??0CPictureHolder@@QEAA@XZ
extern "C" void* MS_ABI impl___0CPictureHolder__QEAA_XZ(void* self) {
    SetPictureState(self, nullptr, 0);
    return self;
}
// Symbol: ??1CPictureHolder@@QEAA@XZ
extern "C" void MS_ABI impl___1CPictureHolder__QEAA_XZ(void* self) {
    g_pictureHolderStates.erase(self);
}
// Symbol: ?CreateEmpty@CPictureHolder@@QEAAHXZ
extern "C" int MS_ABI impl__CreateEmpty_CPictureHolder__QEAAHXZ(void* self) {
    SetPictureState(self, nullptr, 0);
    return TRUE;
}
// Symbol: ?CreateFromBitmap@CPictureHolder@@QEAAHI@Z
extern "C" int MS_ABI impl__CreateFromBitmap_CPictureHolder__QEAAHI_Z(void* self, unsigned int bitmapId) {
    HBITMAP bitmap = ::LoadBitmapW(nullptr, MAKEINTRESOURCEW(bitmapId));
    SetPictureState(self, bitmap, 1, 0, 0, bitmap != nullptr);
    return bitmap != nullptr;
}
// Symbol: ?CreateFromBitmap@CPictureHolder@@QEAAHPEAUHBITMAP__@@PEAUHPALETTE__@@H@Z
extern "C" int MS_ABI impl__CreateFromBitmap_CPictureHolder__QEAAHPEAUHBITMAP____PEAUHPALETTE____H_Z(void* self, HBITMAP bitmap, HPALETTE, int transferOwnership) {
    SetPictureState(self, bitmap, bitmap ? 1 : 0, 0, 0, transferOwnership != FALSE);
    return bitmap != nullptr;
}
// Symbol: ?CreateFromBitmap@CPictureHolder@@QEAAHPEAVCBitmap@@PEAVCPalette@@H@Z
extern "C" int MS_ABI impl__CreateFromBitmap_CPictureHolder__QEAAHPEAVCBitmap__PEAVCPalette__H_Z(void* self, CBitmap* bitmap, CPalette*, int transferOwnership) {
    return impl__CreateFromBitmap_CPictureHolder__QEAAHPEAUHBITMAP____PEAUHPALETTE____H_Z(
        self, bitmap ? static_cast<HBITMAP>(bitmap->GetSafeHandle()) : nullptr, nullptr, transferOwnership);
}
// Symbol: ?CreateFromIcon@CPictureHolder@@QEAAHI@Z
extern "C" int MS_ABI impl__CreateFromIcon_CPictureHolder__QEAAHI_Z(void* self, unsigned int iconId) {
    HICON icon = static_cast<HICON>(::LoadImageW(nullptr, MAKEINTRESOURCEW(iconId), IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_SHARED));
    SetPictureState(self, icon, icon ? 3 : 0);
    return icon != nullptr;
}
// Symbol: ?CreateFromIcon@CPictureHolder@@QEAAHPEAUHICON__@@H@Z
extern "C" int MS_ABI impl__CreateFromIcon_CPictureHolder__QEAAHPEAUHICON____H_Z(void* self, HICON icon, int transferOwnership) {
    SetPictureState(self, icon, icon ? 3 : 0, 0, 0, transferOwnership != FALSE);
    return icon != nullptr;
}
// Symbol: ?CreateFromMetafile@CPictureHolder@@QEAAHPEAUHMETAFILE__@@HHH@Z
extern "C" int MS_ABI impl__CreateFromMetafile_CPictureHolder__QEAAHPEAUHMETAFILE____HHH_Z(void* self, HMETAFILE metafile, int width, int height, int transferOwnership) {
    SetPictureState(self, metafile, metafile ? 2 : 0, width, height, transferOwnership != FALSE);
    return metafile != nullptr;
}
// Symbol: ?SetPictureDispatch@CPictureHolder@@QEAAXPEAUIPictureDisp@@@Z
extern "C" void MS_ABI impl__SetPictureDispatch_CPictureHolder__QEAAXPEAUIPictureDisp___Z(void* self, void* picture) {
    SetPictureState(self, picture, picture ? 4 : 0);
}
// Symbol: ?GetPictureDispatch@CPictureHolder@@QEAAPEAUIPictureDisp@@XZ
extern "C" void* MS_ABI impl__GetPictureDispatch_CPictureHolder__QEAAPEAUIPictureDisp__XZ(void* self) {
    return g_pictureHolderStates[self].picture;
}
// Symbol: ?GetType@CPictureHolder@@QEAAFXZ
extern "C" short MS_ABI impl__GetType_CPictureHolder__QEAAFXZ(void* self) {
    return g_pictureHolderStates[self].type;
}
// Symbol: ?Render@CPictureHolder@@QEAAXPEAVCDC@@AEBVCRect@@1@Z
extern "C" void MS_ABI impl__Render_CPictureHolder__QEAAXPEAVCDC__AEBVCRect__1_Z(void* self, CDC* dc, const CRect* dest, const CRect* source) {
    if (!dc || !dc->GetSafeHdc() || !dest) return;
    PictureHolderState& state = g_pictureHolderStates[self];
    if (state.type == 3 && state.picture) {
        ::DrawIconEx(dc->GetSafeHdc(), dest->left, dest->top, static_cast<HICON>(state.picture),
                     dest->Width(), dest->Height(), 0, nullptr, DI_NORMAL);
        return;
    }
    if (state.type == 1 && state.picture) {
        HDC memDC = ::CreateCompatibleDC(dc->GetSafeHdc());
        HGDIOBJ oldBitmap = memDC ? ::SelectObject(memDC, static_cast<HBITMAP>(state.picture)) : nullptr;
        int srcWidth = source ? source->Width() : dest->Width();
        int srcHeight = source ? source->Height() : dest->Height();
        int srcLeft = source ? source->left : 0;
        int srcTop = source ? source->top : 0;
        if (memDC) {
            ::StretchBlt(dc->GetSafeHdc(), dest->left, dest->top, dest->Width(), dest->Height(),
                         memDC, srcLeft, srcTop, srcWidth, srcHeight, SRCCOPY);
            if (oldBitmap) ::SelectObject(memDC, oldBitmap);
            ::DeleteDC(memDC);
        }
        return;
    }
    ::Rectangle(dc->GetSafeHdc(), dest->left, dest->top, dest->right, dest->bottom);
}
// Symbol: ?GetDisplayString@CPictureHolder@@QEAAHAEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@@Z
extern "C" int MS_ABI impl__GetDisplayString_CPictureHolder__QEAAHAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(void* self, CString* value) {
    if (!value) return FALSE;
    PictureHolderState& state = g_pictureHolderStates[self];
    switch (state.type) {
    case 1: *value = L"Bitmap"; break;
    case 2: *value = L"Metafile"; break;
    case 3: *value = L"Icon"; break;
    default: *value = state.picture ? L"Picture" : L"Empty"; break;
    }
    return TRUE;
}
