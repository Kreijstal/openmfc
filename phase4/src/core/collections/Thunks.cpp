// Thunks — OpenMFC implementation.
// Sources: global_string_atl_thunks.cpp

#include "detail/StringAtlThunksSupport.h"

// Symbol: ?AllocSysString@?$CStringT@DV?$StrTraitMFC_DLL@DV?$ChTraitsCRT@D@ATL@@@@@ATL@@QEBAPEA_WXZ
extern "C" OLECHAR* MS_ABI impl__AllocSysString___CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL__QEBAPEA_WXZ(void* pThis) {
    const openmfc_str::AtlStrT<char>* cself = reinterpret_cast<const openmfc_str::AtlStrT<char>*>(pThis);
    return cself->AllocSysString();
}
// Symbol: ?AllocSysString@?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@QEBAPEA_WXZ
extern "C" OLECHAR* MS_ABI impl__AllocSysString___CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEBAPEA_WXZ(void* pThis) {
    const openmfc_str::AtlStrT<wchar_t>* cself = reinterpret_cast<const openmfc_str::AtlStrT<wchar_t>*>(pThis);
    return cself->AllocSysString();
}

// Symbol: ?CharToOemA@?$CStringT@DV?$StrTraitMFC_DLL@DV?$ChTraitsCRT@D@ATL@@@@@ATL@@QEAAXXZ
extern "C" void MS_ABI impl__CharToOemA___CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL__QEAAXXZ(void* pThis) {
    openmfc_str::AtlStrT<char>* self = reinterpret_cast<openmfc_str::AtlStrT<char>*>(pThis);
    self->CharToOem();
}
// Symbol: ?CharToOemA@?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@QEAAXXZ
extern "C" void MS_ABI impl__CharToOemA___CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAAXXZ(void* pThis) {
    openmfc_str::AtlStrT<wchar_t>* self = reinterpret_cast<openmfc_str::AtlStrT<wchar_t>*>(pThis);
    self->CharToOem();
}
// Symbol: ?CheckImplicitLoad@?$CStringT@DV?$StrTraitMFC_DLL@DV?$ChTraitsCRT@D@ATL@@@@@ATL@@AEAA_NPEBX@Z
extern "C" bool MS_ABI impl__CheckImplicitLoad___CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL__AEAA_NPEBX_Z(void* pThis, const void* p) {
    openmfc_str::AtlStrT<char>* self = reinterpret_cast<openmfc_str::AtlStrT<char>*>(pThis);
    (void)self; return p != nullptr;
}
// Symbol: ?CheckImplicitLoad@?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@AEAA_NPEBX@Z
extern "C" bool MS_ABI impl__CheckImplicitLoad___CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__AEAA_NPEBX_Z(void* pThis, const void* p) {
    openmfc_str::AtlStrT<wchar_t>* self = reinterpret_cast<openmfc_str::AtlStrT<wchar_t>*>(pThis);
    (void)self; return p != nullptr;
}

// Symbol: ?Construct@?$CStringT@DV?$StrTraitMFC_DLL@DV?$ChTraitsCRT@D@ATL@@@@@ATL@@SAXPEAV12@@Z
extern "C" void MS_ABI impl__Construct___CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL__SAXPEAV12__Z(void* p) {
    openmfc_str::AtlStrT<char>::ConstructStatic(reinterpret_cast<openmfc_str::AtlStrT<char>*>(p));
}
// Symbol: ?Construct@?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@SAXPEAV12@@Z
extern "C" void MS_ABI impl__Construct___CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__SAXPEAV12__Z(void* p) {
    openmfc_str::AtlStrT<wchar_t>::ConstructStatic(reinterpret_cast<openmfc_str::AtlStrT<wchar_t>*>(p));
}

// Symbol: ?GetEnvironmentVariable@?$CStringT@DV?$StrTraitMFC_DLL@DV?$ChTraitsCRT@D@ATL@@@@@ATL@@QEAAHPEBD@Z
extern "C" int MS_ABI impl__GetEnvironmentVariable___CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL__QEAAHPEBD_Z(void* pThis, const char* name) {
    openmfc_str::AtlStrT<char>* self = reinterpret_cast<openmfc_str::AtlStrT<char>*>(pThis);
    return self->LoadEnvVar(name);
}
// Symbol: ?GetEnvironmentVariable@?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@QEAAHPEB_W@Z
extern "C" int MS_ABI impl__GetEnvironmentVariable___CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAAHPEB_W_Z(void* pThis, const wchar_t* name) {
    openmfc_str::AtlStrT<wchar_t>* self = reinterpret_cast<openmfc_str::AtlStrT<wchar_t>*>(pThis);
    return self->LoadEnvVar(name);
}
// Symbol: ?GetEnvironmentVariableW@?$CStringT@DV?$StrTraitMFC_DLL@DV?$ChTraitsCRT@D@ATL@@@@@ATL@@QEAAHPEBD@Z
extern "C" int MS_ABI impl__GetEnvironmentVariableW___CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL__QEAAHPEBD_Z(void* pThis, const char* name) {
    openmfc_str::AtlStrT<char>* self = reinterpret_cast<openmfc_str::AtlStrT<char>*>(pThis);
    return self->LoadEnvVar(name);
}
// Symbol: ?GetEnvironmentVariableW@?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@QEAAHPEB_W@Z
extern "C" int MS_ABI impl__GetEnvironmentVariableW___CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAAHPEB_W_Z(void* pThis, const wchar_t* name) {
    openmfc_str::AtlStrT<wchar_t>* self = reinterpret_cast<openmfc_str::AtlStrT<wchar_t>*>(pThis);
    return self->LoadEnvVar(name);
}

// Symbol: ?OemToCharA@?$CStringT@DV?$StrTraitMFC_DLL@DV?$ChTraitsCRT@D@ATL@@@@@ATL@@QEAAXXZ
extern "C" void MS_ABI impl__OemToCharA___CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL__QEAAXXZ(void* pThis) {
    openmfc_str::AtlStrT<char>* self = reinterpret_cast<openmfc_str::AtlStrT<char>*>(pThis);
    self->OemToChar();
}
// Symbol: ?OemToCharA@?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@QEAAXXZ
extern "C" void MS_ABI impl__OemToCharA___CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAAXXZ(void* pThis) {
    openmfc_str::AtlStrT<wchar_t>* self = reinterpret_cast<openmfc_str::AtlStrT<wchar_t>*>(pThis);
    self->OemToChar();
}

// Symbol: ?SetSysString@?$CStringT@DV?$StrTraitMFC_DLL@DV?$ChTraitsCRT@D@ATL@@@@@ATL@@QEBAPEA_WPEAPEA_W@Z
extern "C" OLECHAR* MS_ABI impl__SetSysString___CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL__QEBAPEA_WPEAPEA_W_Z(void* pThis, OLECHAR** pbstr) {
    const openmfc_str::AtlStrT<char>* cself = reinterpret_cast<const openmfc_str::AtlStrT<char>*>(pThis);
    return cself->SetSysString(pbstr);
}
// Symbol: ?SetSysString@?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@QEBAPEA_WPEAPEA_W@Z
extern "C" OLECHAR* MS_ABI impl__SetSysString___CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEBAPEA_WPEAPEA_W_Z(void* pThis, OLECHAR** pbstr) {
    const openmfc_str::AtlStrT<wchar_t>* cself = reinterpret_cast<const openmfc_str::AtlStrT<wchar_t>*>(pThis);
    return cself->SetSysString(pbstr);
}
