// CMFCControlRendererInfo — OpenMFC implementation.
// Sources: collections_strings.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/CollectionsStringsSupport.h"

// Symbol: ??0CMFCControlRendererInfo@@QEAA@XZ
extern "C" void* MS_ABI impl___0CMFCControlRendererInfo__QEAA_XZ(void* self) { return new (self) CMFCControlRendererInfo(); }
// Symbol: ??0CMFCControlRendererInfo@@QEAA@AEBV0@@Z
extern "C" void* MS_ABI impl___0CMFCControlRendererInfo__QEAA_AEBV0__Z(void* self, const CMFCControlRendererInfo* src) { return new (self) CMFCControlRendererInfo(*src); }
// Symbol: ??0CMFCControlRendererInfo@@QEAA@IAEBVCRect@@000@Z
extern "C" void* MS_ABI impl___0CMFCControlRendererInfo__QEAA_IAEBVCRect__000_Z(void* self, unsigned int id, const CRect* image, const CRect* corners, const CRect* sides, const CRect* interior) {
    CRect empty;
    return new (self) CMFCControlRendererInfo(id, image ? *image : empty, corners ? *corners : empty, sides ? *sides : empty, interior ? *interior : empty);
}
// Symbol: ??0CMFCControlRendererInfo@@QEAA@IKAEBVCRect@@000H@Z
extern "C" void* MS_ABI impl___0CMFCControlRendererInfo__QEAA_IKAEBVCRect__000H_Z(void* self, unsigned int id, unsigned long color, const CRect* image, const CRect* corners, const CRect* sides, const CRect* interior, int pre) {
    CRect empty;
    return new (self) CMFCControlRendererInfo(id, color, image ? *image : empty, corners ? *corners : empty, sides ? *sides : empty, interior ? *interior : empty, pre);
}
// Symbol: ??0CMFCControlRendererInfo@@QEAA@PEB_WAEBVCRect@@111@Z
extern "C" void* MS_ABI impl___0CMFCControlRendererInfo__QEAA_PEB_WAEBVCRect__111_Z(void* self, const wchar_t* id, const CRect* image, const CRect* corners, const CRect* sides, const CRect* interior) {
    CRect empty;
    return new (self) CMFCControlRendererInfo(id, image ? *image : empty, corners ? *corners : empty, sides ? *sides : empty, interior ? *interior : empty);
}
// Symbol: ??0CMFCControlRendererInfo@@QEAA@PEB_WKAEBVCRect@@111H@Z
extern "C" void* MS_ABI impl___0CMFCControlRendererInfo__QEAA_PEB_WKAEBVCRect__111H_Z(void* self, const wchar_t* id, unsigned long color, const CRect* image, const CRect* corners, const CRect* sides, const CRect* interior, int pre) {
    CRect empty;
    return new (self) CMFCControlRendererInfo(id, color, image ? *image : empty, corners ? *corners : empty, sides ? *sides : empty, interior ? *interior : empty, pre);
}
// Symbol: ??1CMFCControlRendererInfo@@QEAA@XZ
extern "C" void MS_ABI impl___1CMFCControlRendererInfo__QEAA_XZ(CMFCControlRendererInfo* self) { if (self) self->~CMFCControlRendererInfo(); }
// Symbol: ??4CMFCControlRendererInfo@@QEAAAEAV0@AEBV0@@Z
extern "C" CMFCControlRendererInfo* MS_ABI impl___4CMFCControlRendererInfo__QEAAAEAV0_AEBV0__Z(CMFCControlRendererInfo* self, const CMFCControlRendererInfo* src) { return self && src ? &(*self = *src) : self; }
// Symbol: ?CommonInit@CMFCControlRendererInfo@@QEAAXXZ
extern "C" void MS_ABI impl__CommonInit_CMFCControlRendererInfo__QEAAXXZ(CMFCControlRendererInfo* self) { if (self) self->CommonInit(); }
// Symbol: ?GetResourceID@CMFCControlRendererInfo@@QEBAPEB_WXZ
extern "C" const wchar_t* MS_ABI impl__GetResourceID_CMFCControlRendererInfo__QEBAPEB_WXZ(const CMFCControlRendererInfo* self) { return self ? self->GetResourceID() : L""; }
// Symbol: ?SetResourceID@CMFCControlRendererInfo@@QEAAXPEB_W@Z
extern "C" void MS_ABI impl__SetResourceID_CMFCControlRendererInfo__QEAAXPEB_W_Z(CMFCControlRendererInfo* self, const wchar_t* id) { if (self) self->SetResourceID(id); }
CMFCControlRendererInfo::CMFCControlRendererInfo() {
    memset(_controlrendererinfo_padding, 0, sizeof(_controlrendererinfo_padding));
    CommonInit();
}
CMFCControlRendererInfo::CMFCControlRendererInfo(const CMFCControlRendererInfo& src) {
    memset(_controlrendererinfo_padding, 0, sizeof(_controlrendererinfo_padding));
    g_controlRendererInfoStates[this] = g_controlRendererInfoStates[const_cast<CMFCControlRendererInfo*>(&src)];
}
CMFCControlRendererInfo::CMFCControlRendererInfo(UINT nID, const CRect& rectImage, const CRect& rectCorners,
                                                 const CRect& rectSides, const CRect& rectInter) {
    memset(_controlrendererinfo_padding, 0, sizeof(_controlrendererinfo_padding));
    ControlRendererInfoState& state = g_controlRendererInfoStates[this];
    state.resourceNumericId = nID;
    wchar_t buffer[32] = {};
    wsprintfW(buffer, L"%u", nID);
    state.resourceId = buffer;
    state.image = rectImage;
    state.corners = rectCorners;
    state.sides = rectSides;
    state.interior = rectInter;
    state.hasImage = true;
}
CMFCControlRendererInfo::CMFCControlRendererInfo(UINT nID, DWORD clrTransparent, const CRect& rectImage,
                                                 const CRect& rectCorners, const CRect& rectSides,
                                                 const CRect& rectInter, BOOL bPreMultiplyCheck)
    : CMFCControlRendererInfo(nID, rectImage, rectCorners, rectSides, rectInter) {
    ControlRendererInfoState& state = g_controlRendererInfoStates[this];
    state.transparentColor = clrTransparent;
    state.preMultiplyCheck = bPreMultiplyCheck != FALSE;
}
CMFCControlRendererInfo::CMFCControlRendererInfo(const wchar_t* lpszID, const CRect& rectImage,
                                                 const CRect& rectCorners, const CRect& rectSides,
                                                 const CRect& rectInter) {
    memset(_controlrendererinfo_padding, 0, sizeof(_controlrendererinfo_padding));
    ControlRendererInfoState& state = g_controlRendererInfoStates[this];
    state.resourceId = lpszID ? lpszID : L"";
    unsigned long numeric = 0;
    if (ParseUnsignedDecimalOrHex(state.resourceId.c_str(), &numeric)) state.resourceNumericId = static_cast<UINT>(numeric);
    state.image = rectImage;
    state.corners = rectCorners;
    state.sides = rectSides;
    state.interior = rectInter;
    state.hasImage = true;
}
CMFCControlRendererInfo::CMFCControlRendererInfo(const wchar_t* lpszID, DWORD clrTransparent,
                                                 const CRect& rectImage, const CRect& rectCorners,
                                                 const CRect& rectSides, const CRect& rectInter,
                                                 BOOL bPreMultiplyCheck)
    : CMFCControlRendererInfo(lpszID, rectImage, rectCorners, rectSides, rectInter) {
    ControlRendererInfoState& state = g_controlRendererInfoStates[this];
    state.transparentColor = clrTransparent;
    state.preMultiplyCheck = bPreMultiplyCheck != FALSE;
}
CMFCControlRendererInfo::~CMFCControlRendererInfo() {
    g_controlRendererInfoStates.erase(this);
}
CMFCControlRendererInfo& CMFCControlRendererInfo::operator=(const CMFCControlRendererInfo& src) {
    if (this != &src) g_controlRendererInfoStates[this] = g_controlRendererInfoStates[const_cast<CMFCControlRendererInfo*>(&src)];
    return *this;
}
void CMFCControlRendererInfo::CommonInit() {
    g_controlRendererInfoStates[this] = ControlRendererInfoState();
}
const wchar_t* CMFCControlRendererInfo::GetResourceID() const {
    auto it = g_controlRendererInfoStates.find(const_cast<CMFCControlRendererInfo*>(this));
    return it == g_controlRendererInfoStates.end() ? L"" : it->second.resourceId.c_str();
}
void CMFCControlRendererInfo::SetResourceID(const wchar_t* lpszID) {
    ControlRendererInfoState& state = g_controlRendererInfoStates[this];
    state.resourceId = lpszID ? lpszID : L"";
    unsigned long numeric = 0;
    state.resourceNumericId = ParseUnsignedDecimalOrHex(state.resourceId.c_str(), &numeric) ? static_cast<UINT>(numeric) : 0;
}
