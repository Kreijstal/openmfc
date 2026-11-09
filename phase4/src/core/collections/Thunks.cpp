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




// Symbol: ?AppendFormat@?$CStringT@DV?$StrTraitMFC_DLL@DV?$ChTraitsCRT@D@ATL@@@@@ATL@@QEAAXIZZ
extern "C" void MS_ABI impl__AppendFormat___CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL__QEAAXIZZ(void* pThis, unsigned int nID, ...) {
    openmfc_str::AtlStrT<char>* self = reinterpret_cast<openmfc_str::AtlStrT<char>*>(pThis);
    openmfc_str::AtlStrT<char> f; f.Construct(); f.LoadStr(GetResHandle(), nID); va_list a; va_start(a, nID); self->AppendFormatV(f.GetString(), a); va_end(a); f.Destruct();
}
// Symbol: ?AppendFormat@?$CStringT@DV?$StrTraitMFC_DLL@DV?$ChTraitsCRT@D@ATL@@@@@ATL@@QEAAXPEBDZZ
extern "C" void MS_ABI impl__AppendFormat___CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL__QEAAXPEBDZZ(void* pThis, const char* fmt, ...) {
    openmfc_str::AtlStrT<char>* self = reinterpret_cast<openmfc_str::AtlStrT<char>*>(pThis);
    va_list a; va_start(a, fmt); self->AppendFormatV(fmt, a); va_end(a);
}
// Symbol: ?AppendFormat@?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@QEAAXIZZ
extern "C" void MS_ABI impl__AppendFormat___CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAAXIZZ(void* pThis, unsigned int nID, ...) {
    openmfc_str::AtlStrT<wchar_t>* self = reinterpret_cast<openmfc_str::AtlStrT<wchar_t>*>(pThis);
    openmfc_str::AtlStrT<wchar_t> f; f.Construct(); f.LoadStr(GetResHandle(), nID); va_list a; va_start(a, nID); self->AppendFormatV(f.GetString(), a); va_end(a); f.Destruct();
}
// Symbol: ?AppendFormat@?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@QEAAXPEB_WZZ
extern "C" void MS_ABI impl__AppendFormat___CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAAXPEB_WZZ(void* pThis, const wchar_t* fmt, ...) {
    openmfc_str::AtlStrT<wchar_t>* self = reinterpret_cast<openmfc_str::AtlStrT<wchar_t>*>(pThis);
    va_list a; va_start(a, fmt); self->AppendFormatV(fmt, a); va_end(a);
}
// Symbol: ?AppendFormatV@?$CStringT@DV?$StrTraitMFC_DLL@DV?$ChTraitsCRT@D@ATL@@@@@ATL@@QEAAXPEBDPEAD@Z
extern "C" void MS_ABI impl__AppendFormatV___CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL__QEAAXPEBDPEAD_Z(void* pThis, const char* fmt, char* args) {
    openmfc_str::AtlStrT<char>* self = reinterpret_cast<openmfc_str::AtlStrT<char>*>(pThis);
    self->AppendFormatV(fmt, (va_list)args);
}
// Symbol: ?AppendFormatV@?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@QEAAXPEB_WPEAD@Z
extern "C" void MS_ABI impl__AppendFormatV___CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAAXPEB_WPEAD_Z(void* pThis, const wchar_t* fmt, char* args) {
    openmfc_str::AtlStrT<wchar_t>* self = reinterpret_cast<openmfc_str::AtlStrT<wchar_t>*>(pThis);
    self->AppendFormatV(fmt, (va_list)args);
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

// Symbol: ?Collate@?$CStringT@DV?$StrTraitMFC_DLL@DV?$ChTraitsCRT@D@ATL@@@@@ATL@@QEBAHPEBD@Z
extern "C" int MS_ABI impl__Collate___CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL__QEBAHPEBD_Z(void* pThis, const char* psz) {
    const openmfc_str::AtlStrT<char>* cself = reinterpret_cast<const openmfc_str::AtlStrT<char>*>(pThis);
    return cself->Collate(psz);
}
// Symbol: ?CollateNoCase@?$CStringT@DV?$StrTraitMFC_DLL@DV?$ChTraitsCRT@D@ATL@@@@@ATL@@QEBAHPEBD@Z
extern "C" int MS_ABI impl__CollateNoCase___CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL__QEBAHPEBD_Z(void* pThis, const char* psz) {
    const openmfc_str::AtlStrT<char>* cself = reinterpret_cast<const openmfc_str::AtlStrT<char>*>(pThis);
    return cself->CollateNoCase(psz);
}
// Symbol: ?Compare@?$CStringT@DV?$StrTraitMFC_DLL@DV?$ChTraitsCRT@D@ATL@@@@@ATL@@QEBAHPEBD@Z
extern "C" int MS_ABI impl__Compare___CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL__QEBAHPEBD_Z(void* pThis, const char* psz) {
    const openmfc_str::AtlStrT<char>* cself = reinterpret_cast<const openmfc_str::AtlStrT<char>*>(pThis);
    return cself->Compare(psz);
}
// Symbol: ?CompareNoCase@?$CStringT@DV?$StrTraitMFC_DLL@DV?$ChTraitsCRT@D@ATL@@@@@ATL@@QEBAHPEBD@Z
extern "C" int MS_ABI impl__CompareNoCase___CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL__QEBAHPEBD_Z(void* pThis, const char* psz) {
    const openmfc_str::AtlStrT<char>* cself = reinterpret_cast<const openmfc_str::AtlStrT<char>*>(pThis);
    return cself->CompareNoCase(psz);
}

// Symbol: ?Construct@?$CStringT@DV?$StrTraitMFC_DLL@DV?$ChTraitsCRT@D@ATL@@@@@ATL@@SAXPEAV12@@Z
extern "C" void MS_ABI impl__Construct___CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL__SAXPEAV12__Z(void* p) {
    openmfc_str::AtlStrT<char>::ConstructStatic(reinterpret_cast<openmfc_str::AtlStrT<char>*>(p));
}
// Symbol: ?Construct@?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@SAXPEAV12@@Z
extern "C" void MS_ABI impl__Construct___CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__SAXPEAV12__Z(void* p) {
    openmfc_str::AtlStrT<wchar_t>::ConstructStatic(reinterpret_cast<openmfc_str::AtlStrT<wchar_t>*>(p));
}


// Symbol: ?Delete@?$CStringT@DV?$StrTraitMFC_DLL@DV?$ChTraitsCRT@D@ATL@@@@@ATL@@QEAAHHH@Z
extern "C" int MS_ABI impl__Delete___CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL__QEAAHHH_Z(void* pThis, int i, int n) {
    openmfc_str::AtlStrT<char>* self = reinterpret_cast<openmfc_str::AtlStrT<char>*>(pThis);
    return self->Delete(i, n);
}

// Symbol: ?Find@?$CStringT@DV?$StrTraitMFC_DLL@DV?$ChTraitsCRT@D@ATL@@@@@ATL@@QEBAHDH@Z
extern "C" int MS_ABI impl__Find___CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL__QEBAHDH_Z(void* pThis, char ch, int start) {
    const openmfc_str::AtlStrT<char>* cself = reinterpret_cast<const openmfc_str::AtlStrT<char>*>(pThis);
    return cself->FindCh(ch, start);
}
// Symbol: ?Find@?$CStringT@DV?$StrTraitMFC_DLL@DV?$ChTraitsCRT@D@ATL@@@@@ATL@@QEBAHPEBDH@Z
extern "C" int MS_ABI impl__Find___CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL__QEBAHPEBDH_Z(void* pThis, const char* psz, int start) {
    const openmfc_str::AtlStrT<char>* cself = reinterpret_cast<const openmfc_str::AtlStrT<char>*>(pThis);
    return cself->FindPsz(psz, start);
}
// Symbol: ?FindOneOf@?$CStringT@DV?$StrTraitMFC_DLL@DV?$ChTraitsCRT@D@ATL@@@@@ATL@@QEBAHPEBD@Z
extern "C" int MS_ABI impl__FindOneOf___CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL__QEBAHPEBD_Z(void* pThis, const char* set) {
    const openmfc_str::AtlStrT<char>* cself = reinterpret_cast<const openmfc_str::AtlStrT<char>*>(pThis);
    return cself->FindOneOf(set);
}

// Symbol: ?Format@?$CStringT@DV?$StrTraitMFC_DLL@DV?$ChTraitsCRT@D@ATL@@@@@ATL@@QEAAXIZZ
extern "C" void MS_ABI impl__Format___CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL__QEAAXIZZ(void* pThis, unsigned int nID, ...) {
    openmfc_str::AtlStrT<char>* self = reinterpret_cast<openmfc_str::AtlStrT<char>*>(pThis);
    openmfc_str::AtlStrT<char> f; f.Construct(); f.LoadStr(GetResHandle(), nID); va_list a; va_start(a, nID); self->FormatV(f.GetString(), a); va_end(a); f.Destruct();
}
// Symbol: ?Format@?$CStringT@DV?$StrTraitMFC_DLL@DV?$ChTraitsCRT@D@ATL@@@@@ATL@@QEAAXPEBDZZ
extern "C" void MS_ABI impl__Format___CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL__QEAAXPEBDZZ(void* pThis, const char* fmt, ...) {
    openmfc_str::AtlStrT<char>* self = reinterpret_cast<openmfc_str::AtlStrT<char>*>(pThis);
    va_list a; va_start(a, fmt); self->FormatV(fmt, a); va_end(a);
}
// Symbol: ?Format@?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@QEAAXIZZ
extern "C" void MS_ABI impl__Format___CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAAXIZZ(void* pThis, unsigned int nID, ...) {
    openmfc_str::AtlStrT<wchar_t>* self = reinterpret_cast<openmfc_str::AtlStrT<wchar_t>*>(pThis);
    openmfc_str::AtlStrT<wchar_t> f; f.Construct(); f.LoadStr(GetResHandle(), nID); va_list a; va_start(a, nID); self->FormatV(f.GetString(), a); va_end(a); f.Destruct();
}
// Symbol: ?FormatMessage@?$CStringT@DV?$StrTraitMFC_DLL@DV?$ChTraitsCRT@D@ATL@@@@@ATL@@QEAAXIZZ
extern "C" void MS_ABI impl__FormatMessage___CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL__QEAAXIZZ(void* pThis, unsigned int nID, ...) {
    openmfc_str::AtlStrT<char>* self = reinterpret_cast<openmfc_str::AtlStrT<char>*>(pThis);
    openmfc_str::AtlStrT<char> f; f.Construct(); f.LoadStr(GetResHandle(), nID); va_list a; va_start(a, nID); self->FormatMessageStr(f.GetString(), &a); va_end(a); f.Destruct();
}
// Symbol: ?FormatMessage@?$CStringT@DV?$StrTraitMFC_DLL@DV?$ChTraitsCRT@D@ATL@@@@@ATL@@QEAAXPEBDZZ
extern "C" void MS_ABI impl__FormatMessage___CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL__QEAAXPEBDZZ(void* pThis, const char* fmt, ...) {
    openmfc_str::AtlStrT<char>* self = reinterpret_cast<openmfc_str::AtlStrT<char>*>(pThis);
    va_list a; va_start(a, fmt); self->FormatMessageStr(fmt, &a); va_end(a);
}
// Symbol: ?FormatMessage@?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@QEAAXIZZ
extern "C" void MS_ABI impl__FormatMessage___CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAAXIZZ(void* pThis, unsigned int nID, ...) {
    openmfc_str::AtlStrT<wchar_t>* self = reinterpret_cast<openmfc_str::AtlStrT<wchar_t>*>(pThis);
    openmfc_str::AtlStrT<wchar_t> f; f.Construct(); f.LoadStr(GetResHandle(), nID); va_list a; va_start(a, nID); self->FormatMessageStr(f.GetString(), &a); va_end(a); f.Destruct();
}
// Symbol: ?FormatMessage@?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@QEAAXPEB_WZZ
extern "C" void MS_ABI impl__FormatMessage___CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAAXPEB_WZZ(void* pThis, const wchar_t* fmt, ...) {
    openmfc_str::AtlStrT<wchar_t>* self = reinterpret_cast<openmfc_str::AtlStrT<wchar_t>*>(pThis);
    va_list a; va_start(a, fmt); self->FormatMessageStr(fmt, &a); va_end(a);
}
// Symbol: ?FormatMessageV@?$CStringT@DV?$StrTraitMFC_DLL@DV?$ChTraitsCRT@D@ATL@@@@@ATL@@QEAAXPEBDPEAPEAD@Z
extern "C" void MS_ABI impl__FormatMessageV___CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL__QEAAXPEBDPEAPEAD_Z(void* pThis, const char* fmt, char** pArgs) {
    openmfc_str::AtlStrT<char>* self = reinterpret_cast<openmfc_str::AtlStrT<char>*>(pThis);
    self->FormatMessageStr(fmt, (va_list*)pArgs);
}
// Symbol: ?FormatMessageV@?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@QEAAXPEB_WPEAPEAD@Z
extern "C" void MS_ABI impl__FormatMessageV___CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAAXPEB_WPEAPEAD_Z(void* pThis, const wchar_t* fmt, char** pArgs) {
    openmfc_str::AtlStrT<wchar_t>* self = reinterpret_cast<openmfc_str::AtlStrT<wchar_t>*>(pThis);
    self->FormatMessageStr(fmt, (va_list*)pArgs);
}
// Symbol: ?FormatMessageW@?$CStringT@DV?$StrTraitMFC_DLL@DV?$ChTraitsCRT@D@ATL@@@@@ATL@@QEAAXIZZ
extern "C" void MS_ABI impl__FormatMessageW___CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL__QEAAXIZZ(void* pThis, unsigned int nID, ...) {
    openmfc_str::AtlStrT<char>* self = reinterpret_cast<openmfc_str::AtlStrT<char>*>(pThis);
    openmfc_str::AtlStrT<char> f; f.Construct(); f.LoadStr(GetResHandle(), nID); va_list a; va_start(a, nID); self->FormatMessageStr(f.GetString(), &a); va_end(a); f.Destruct();
}
// Symbol: ?FormatMessageW@?$CStringT@DV?$StrTraitMFC_DLL@DV?$ChTraitsCRT@D@ATL@@@@@ATL@@QEAAXPEBDZZ
extern "C" void MS_ABI impl__FormatMessageW___CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL__QEAAXPEBDZZ(void* pThis, const char* fmt, ...) {
    openmfc_str::AtlStrT<char>* self = reinterpret_cast<openmfc_str::AtlStrT<char>*>(pThis);
    va_list a; va_start(a, fmt); self->FormatMessageStr(fmt, &a); va_end(a);
}
// Symbol: ?FormatMessageW@?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@QEAAXIZZ
extern "C" void MS_ABI impl__FormatMessageW___CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAAXIZZ(void* pThis, unsigned int nID, ...) {
    openmfc_str::AtlStrT<wchar_t>* self = reinterpret_cast<openmfc_str::AtlStrT<wchar_t>*>(pThis);
    openmfc_str::AtlStrT<wchar_t> f; f.Construct(); f.LoadStr(GetResHandle(), nID); va_list a; va_start(a, nID); self->FormatMessageStr(f.GetString(), &a); va_end(a); f.Destruct();
}
// Symbol: ?FormatMessageW@?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@QEAAXPEB_WZZ
extern "C" void MS_ABI impl__FormatMessageW___CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAAXPEB_WZZ(void* pThis, const wchar_t* fmt, ...) {
    openmfc_str::AtlStrT<wchar_t>* self = reinterpret_cast<openmfc_str::AtlStrT<wchar_t>*>(pThis);
    va_list a; va_start(a, fmt); self->FormatMessageStr(fmt, &a); va_end(a);
}
// Symbol: ?FormatV@?$CStringT@DV?$StrTraitMFC_DLL@DV?$ChTraitsCRT@D@ATL@@@@@ATL@@QEAAXPEBDPEAD@Z
extern "C" void MS_ABI impl__FormatV___CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL__QEAAXPEBDPEAD_Z(void* pThis, const char* fmt, char* args) {
    openmfc_str::AtlStrT<char>* self = reinterpret_cast<openmfc_str::AtlStrT<char>*>(pThis);
    self->FormatV(fmt, (va_list)args);
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



// Symbol: ?Insert@?$CStringT@DV?$StrTraitMFC_DLL@DV?$ChTraitsCRT@D@ATL@@@@@ATL@@QEAAHHD@Z
extern "C" int MS_ABI impl__Insert___CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL__QEAAHHD_Z(void* pThis, int i, char ch) {
    openmfc_str::AtlStrT<char>* self = reinterpret_cast<openmfc_str::AtlStrT<char>*>(pThis);
    return self->Insert(i, ch);
}
// Symbol: ?Insert@?$CStringT@DV?$StrTraitMFC_DLL@DV?$ChTraitsCRT@D@ATL@@@@@ATL@@QEAAHHPEBD@Z
extern "C" int MS_ABI impl__Insert___CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL__QEAAHHPEBD_Z(void* pThis, int i, const char* psz) {
    openmfc_str::AtlStrT<char>* self = reinterpret_cast<openmfc_str::AtlStrT<char>*>(pThis);
    return self->Insert(i, psz);
}
// Symbol: ?Insert@?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@QEAAHHPEB_W@Z
extern "C" int MS_ABI impl__Insert___CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAAHHPEB_W_Z(void* pThis, int i, const wchar_t* psz) {
    openmfc_str::AtlStrT<wchar_t>* self = reinterpret_cast<openmfc_str::AtlStrT<wchar_t>*>(pThis);
    return self->Insert(i, psz);
}
// Symbol: ?Insert@?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@QEAAHH_W@Z
extern "C" int MS_ABI impl__Insert___CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAAHH_W_Z(void* pThis, int i, wchar_t ch) {
    openmfc_str::AtlStrT<wchar_t>* self = reinterpret_cast<openmfc_str::AtlStrT<wchar_t>*>(pThis);
    return self->Insert(i, ch);
}





// Symbol: ?MakeLower@?$CStringT@DV?$StrTraitMFC_DLL@DV?$ChTraitsCRT@D@ATL@@@@@ATL@@QEAAAEAV12@XZ
extern "C" void* MS_ABI impl__MakeLower___CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL__QEAAAEAV12_XZ(void* pThis) {
    openmfc_str::AtlStrT<char>* self = reinterpret_cast<openmfc_str::AtlStrT<char>*>(pThis);
    self->MakeLower();
    return pThis;
}
// Symbol: ?MakeReverse@?$CStringT@DV?$StrTraitMFC_DLL@DV?$ChTraitsCRT@D@ATL@@@@@ATL@@QEAAAEAV12@XZ
extern "C" void* MS_ABI impl__MakeReverse___CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL__QEAAAEAV12_XZ(void* pThis) {
    openmfc_str::AtlStrT<char>* self = reinterpret_cast<openmfc_str::AtlStrT<char>*>(pThis);
    self->MakeReverse();
    return pThis;
}
// Symbol: ?MakeUpper@?$CStringT@DV?$StrTraitMFC_DLL@DV?$ChTraitsCRT@D@ATL@@@@@ATL@@QEAAAEAV12@XZ
extern "C" void* MS_ABI impl__MakeUpper___CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL__QEAAAEAV12_XZ(void* pThis) {
    openmfc_str::AtlStrT<char>* self = reinterpret_cast<openmfc_str::AtlStrT<char>*>(pThis);
    self->MakeUpper();
    return pThis;
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






// Symbol: ?Remove@?$CStringT@DV?$StrTraitMFC_DLL@DV?$ChTraitsCRT@D@ATL@@@@@ATL@@QEAAHD@Z
extern "C" int MS_ABI impl__Remove___CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL__QEAAHD_Z(void* pThis, char ch) {
    openmfc_str::AtlStrT<char>* self = reinterpret_cast<openmfc_str::AtlStrT<char>*>(pThis);
    return self->Remove(ch);
}
// Symbol: ?Remove@?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@QEAAH_W@Z
extern "C" int MS_ABI impl__Remove___CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAAH_W_Z(void* pThis, wchar_t ch) {
    openmfc_str::AtlStrT<wchar_t>* self = reinterpret_cast<openmfc_str::AtlStrT<wchar_t>*>(pThis);
    return self->Remove(ch);
}
// Symbol: ?Replace@?$CStringT@DV?$StrTraitMFC_DLL@DV?$ChTraitsCRT@D@ATL@@@@@ATL@@QEAAHDD@Z
extern "C" int MS_ABI impl__Replace___CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL__QEAAHDD_Z(void* pThis, char a, char b) {
    openmfc_str::AtlStrT<char>* self = reinterpret_cast<openmfc_str::AtlStrT<char>*>(pThis);
    return self->ReplaceCh(a, b);
}
// Symbol: ?Replace@?$CStringT@DV?$StrTraitMFC_DLL@DV?$ChTraitsCRT@D@ATL@@@@@ATL@@QEAAHPEBD0@Z
extern "C" int MS_ABI impl__Replace___CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL__QEAAHPEBD0_Z(void* pThis, const char* a, const char* b) {
    openmfc_str::AtlStrT<char>* self = reinterpret_cast<openmfc_str::AtlStrT<char>*>(pThis);
    return ReplaceStr(self, a, b);
}
// Symbol: ?ReverseFind@?$CStringT@DV?$StrTraitMFC_DLL@DV?$ChTraitsCRT@D@ATL@@@@@ATL@@QEBAHD@Z
extern "C" int MS_ABI impl__ReverseFind___CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL__QEBAHD_Z(void* pThis, char ch) {
    const openmfc_str::AtlStrT<char>* cself = reinterpret_cast<const openmfc_str::AtlStrT<char>*>(pThis);
    return cself->ReverseFind(ch);
}
// Symbol: ?ReverseFind@?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@QEBAH_W@Z
extern "C" int MS_ABI impl__ReverseFind___CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEBAH_W_Z(void* pThis, wchar_t ch) {
    const openmfc_str::AtlStrT<wchar_t>* cself = reinterpret_cast<const openmfc_str::AtlStrT<wchar_t>*>(pThis);
    return cself->ReverseFind(ch);
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



// Symbol: ?Trim@?$CStringT@DV?$StrTraitMFC_DLL@DV?$ChTraitsCRT@D@ATL@@@@@ATL@@QEAAAEAV12@D@Z
extern "C" void* MS_ABI impl__Trim___CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL__QEAAAEAV12_D_Z(void* pThis, char ch) {
    openmfc_str::AtlStrT<char>* self = reinterpret_cast<openmfc_str::AtlStrT<char>*>(pThis);
    Trim_StringAtlThunks<char>(self, ch, (const char*)nullptr, 3);
    return pThis;
}
// Symbol: ?Trim@?$CStringT@DV?$StrTraitMFC_DLL@DV?$ChTraitsCRT@D@ATL@@@@@ATL@@QEAAAEAV12@PEBD@Z
extern "C" void* MS_ABI impl__Trim___CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL__QEAAAEAV12_PEBD_Z(void* pThis, const char* set) {
    openmfc_str::AtlStrT<char>* self = reinterpret_cast<openmfc_str::AtlStrT<char>*>(pThis);
    Trim_StringAtlThunks<char>(self, (char)0, set, 3);
    return pThis;
}
// Symbol: ?Trim@?$CStringT@DV?$StrTraitMFC_DLL@DV?$ChTraitsCRT@D@ATL@@@@@ATL@@QEAAAEAV12@XZ
extern "C" void* MS_ABI impl__Trim___CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL__QEAAAEAV12_XZ(void* pThis) {
    openmfc_str::AtlStrT<char>* self = reinterpret_cast<openmfc_str::AtlStrT<char>*>(pThis);
    Trim_StringAtlThunks<char>(self, (char)0, (const char*)nullptr, 3);
    return pThis;
}
// Symbol: ?TrimLeft@?$CStringT@DV?$StrTraitMFC_DLL@DV?$ChTraitsCRT@D@ATL@@@@@ATL@@QEAAAEAV12@D@Z
extern "C" void* MS_ABI impl__TrimLeft___CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL__QEAAAEAV12_D_Z(void* pThis, char ch) {
    openmfc_str::AtlStrT<char>* self = reinterpret_cast<openmfc_str::AtlStrT<char>*>(pThis);
    Trim_StringAtlThunks<char>(self, ch, (const char*)nullptr, 1);
    return pThis;
}
// Symbol: ?TrimLeft@?$CStringT@DV?$StrTraitMFC_DLL@DV?$ChTraitsCRT@D@ATL@@@@@ATL@@QEAAAEAV12@PEBD@Z
extern "C" void* MS_ABI impl__TrimLeft___CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL__QEAAAEAV12_PEBD_Z(void* pThis, const char* set) {
    openmfc_str::AtlStrT<char>* self = reinterpret_cast<openmfc_str::AtlStrT<char>*>(pThis);
    Trim_StringAtlThunks<char>(self, (char)0, set, 1);
    return pThis;
}
// Symbol: ?TrimLeft@?$CStringT@DV?$StrTraitMFC_DLL@DV?$ChTraitsCRT@D@ATL@@@@@ATL@@QEAAAEAV12@XZ
extern "C" void* MS_ABI impl__TrimLeft___CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL__QEAAAEAV12_XZ(void* pThis) {
    openmfc_str::AtlStrT<char>* self = reinterpret_cast<openmfc_str::AtlStrT<char>*>(pThis);
    Trim_StringAtlThunks<char>(self, (char)0, (const char*)nullptr, 1);
    return pThis;
}
// Symbol: ?TrimRight@?$CStringT@DV?$StrTraitMFC_DLL@DV?$ChTraitsCRT@D@ATL@@@@@ATL@@QEAAAEAV12@D@Z
extern "C" void* MS_ABI impl__TrimRight___CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL__QEAAAEAV12_D_Z(void* pThis, char ch) {
    openmfc_str::AtlStrT<char>* self = reinterpret_cast<openmfc_str::AtlStrT<char>*>(pThis);
    Trim_StringAtlThunks<char>(self, ch, (const char*)nullptr, 2);
    return pThis;
}
// Symbol: ?TrimRight@?$CStringT@DV?$StrTraitMFC_DLL@DV?$ChTraitsCRT@D@ATL@@@@@ATL@@QEAAAEAV12@PEBD@Z
extern "C" void* MS_ABI impl__TrimRight___CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL__QEAAAEAV12_PEBD_Z(void* pThis, const char* set) {
    openmfc_str::AtlStrT<char>* self = reinterpret_cast<openmfc_str::AtlStrT<char>*>(pThis);
    Trim_StringAtlThunks<char>(self, (char)0, set, 2);
    return pThis;
}
// Symbol: ?TrimRight@?$CStringT@DV?$StrTraitMFC_DLL@DV?$ChTraitsCRT@D@ATL@@@@@ATL@@QEAAAEAV12@XZ
extern "C" void* MS_ABI impl__TrimRight___CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL__QEAAAEAV12_XZ(void* pThis) {
    openmfc_str::AtlStrT<char>* self = reinterpret_cast<openmfc_str::AtlStrT<char>*>(pThis);
    Trim_StringAtlThunks<char>(self, (char)0, (const char*)nullptr, 2);
    return pThis;
}
