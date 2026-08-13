// CPropExchange — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp

#include "detail/ManualSmallStubImplementationsSupport.h"

// Symbol: ?ExchangeVersion@CPropExchange@@UEAAHAEAKKH@Z
extern "C" int MS_ABI impl__ExchangeVersion_CPropExchange__UEAAHAEAKKH_Z(
    void* /*pThis*/, unsigned long* pVersion, unsigned long preferredVersion, int) {
    if (pVersion == nullptr) {
        return 0;
    }

    const unsigned long loadedVersion = *pVersion;
    if (loadedVersion == 0) {
        *pVersion = preferredVersion;
        return 1;
    }

    *pVersion = loadedVersion;
    return (loadedVersion <= preferredVersion) ? 1 : 0;
}

// Symbol: ?PX_Blob@@YAHPEAVCPropExchange@@PEB_WAEAPEAXPEAX@Z
extern "C" int MS_ABI impl__PX_Blob__YAHPEAVCPropExchange__PEB_WAEAPEAXPEAX_Z(void* /*class*/* p0, const wchar_t* p1, void** p2, void* p3) {
    return 0;
}

// Symbol: ?PX_Bool@@YAHPEAVCPropExchange@@PEB_WAEAH@Z
extern "C" int MS_ABI impl__PX_Bool__YAHPEAVCPropExchange__PEB_WAEAH_Z(void* /*class*/* p0, const wchar_t* p1, int* p2) {
    return 0;
}

// Symbol: ?PX_Bool@@YAHPEAVCPropExchange@@PEB_WAEAHH@Z
extern "C" int MS_ABI impl__PX_Bool__YAHPEAVCPropExchange__PEB_WAEAHH_Z(void* /*class*/* p0, const wchar_t* p1, int* p2, int p3) {
    return 0;
}

// Symbol: ?PX_Color@@YAHPEAVCPropExchange@@PEB_WAEAK@Z
extern "C" int MS_ABI impl__PX_Color__YAHPEAVCPropExchange__PEB_WAEAK_Z(void* /*class*/* p0, const wchar_t* p1, unsigned long* p2) {
    return 0;
}

// Symbol: ?PX_Color@@YAHPEAVCPropExchange@@PEB_WAEAKK@Z
extern "C" int MS_ABI impl__PX_Color__YAHPEAVCPropExchange__PEB_WAEAKK_Z(void* /*class*/* p0, const wchar_t* p1, unsigned long* p2, unsigned long p3) {
    return 0;
}

// Symbol: ?PX_Currency@@YAHPEAVCPropExchange@@PEB_WAEATtagCY@@@Z
extern "C" int MS_ABI impl__PX_Currency__YAHPEAVCPropExchange__PEB_WAEATtagCY___Z(void* /*class*/* p0, const wchar_t* p1, void** p2, void* p3, void* p4, void* p5, void* p6, void* p7) {
    return 0;
}

// Symbol: ?PX_Currency@@YAHPEAVCPropExchange@@PEB_WAEATtagCY@@T2@@Z
extern "C" int MS_ABI impl__PX_Currency__YAHPEAVCPropExchange__PEB_WAEATtagCY__T2__Z(void* /*class*/* p0, const wchar_t* p1, void** p2, void* p3, void* p4, void* p5, void* p6, void* p7, void* p8, void** p9) {
    return 0;
}

// Symbol: ?PX_DataPath@@YAHPEAVCPropExchange@@PEB_WAEAVCDataPathProperty@@1@Z
extern "C" int MS_ABI impl__PX_DataPath__YAHPEAVCPropExchange__PEB_WAEAVCDataPathProperty__1_Z(void* /*class*/* p0, const wchar_t* p1, void* /*class*/* p2, const wchar_t* p3) {
    return 0;
}

// Symbol: ?PX_DataPath@@YAHPEAVCPropExchange@@PEB_WAEAVCDataPathProperty@@AEBV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@@Z
extern "C" int MS_ABI impl__PX_DataPath__YAHPEAVCPropExchange__PEB_WAEAVCDataPathProperty__AEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(void* /*class*/* p0, const wchar_t* p1, void* /*class*/* p2, const void* /*class*/* p3, void** p4, void* p5) {
    return 0;
}

// Symbol: ?PX_Double@@YAHPEAVCPropExchange@@PEB_WAEAN@Z
extern "C" int MS_ABI impl__PX_Double__YAHPEAVCPropExchange__PEB_WAEAN_Z(void* /*class*/* p0, const wchar_t* p1, double* p2) {
    return 0;
}

// Symbol: ?PX_Double@@YAHPEAVCPropExchange@@PEB_WAEANN@Z
extern "C" int MS_ABI impl__PX_Double__YAHPEAVCPropExchange__PEB_WAEANN_Z(void* /*class*/* p0, const wchar_t* p1, double* p2, double p3) {
    return 0;
}

// Symbol: ?PX_Float@@YAHPEAVCPropExchange@@PEB_WAEAM@Z
extern "C" int MS_ABI impl__PX_Float__YAHPEAVCPropExchange__PEB_WAEAM_Z(void* /*class*/* p0, const wchar_t* p1, float* p2) {
    return 0;
}

// Symbol: ?PX_Float@@YAHPEAVCPropExchange@@PEB_WAEAMM@Z
extern "C" int MS_ABI impl__PX_Float__YAHPEAVCPropExchange__PEB_WAEAMM_Z(void* /*class*/* p0, const wchar_t* p1, float* p2, float p3) {
    return 0;
}

// Symbol: ?PX_Font@@YAHPEAVCPropExchange@@PEB_WAEAVCFontHolder@@PEBUtagFONTDESC@@PEAUIFontDisp@@@Z
extern "C" int MS_ABI impl__PX_Font__YAHPEAVCPropExchange__PEB_WAEAVCFontHolder__PEBUtagFONTDESC__PEAUIFontDisp___Z(void* /*class*/* p0, const wchar_t* p1, void* /*class*/* p2, const void* /*struct*/* p3, void* /*struct*/* p4) {
    return 0;
}

// Symbol: ?PX_IUnknown@@YAHPEAVCPropExchange@@PEB_WAEAPEAUIUnknown@@AEBU_GUID@@PEAU2@@Z
extern "C" int MS_ABI impl__PX_IUnknown__YAHPEAVCPropExchange__PEB_WAEAPEAUIUnknown__AEBU_GUID__PEAU2__Z(void* /*class*/* p0, const wchar_t* p1, void* /*struct*/** p2, const void* /*struct*/* p3, void* /*struct*/* p4) {
    return 0;
}

// Symbol: ?PX_Long@@YAHPEAVCPropExchange@@PEB_WAEAJ@Z
extern "C" int MS_ABI impl__PX_Long__YAHPEAVCPropExchange__PEB_WAEAJ_Z(void* /*class*/* p0, const wchar_t* p1, long* p2) {
    return 0;
}

// Symbol: ?PX_Long@@YAHPEAVCPropExchange@@PEB_WAEAJJ@Z
extern "C" int MS_ABI impl__PX_Long__YAHPEAVCPropExchange__PEB_WAEAJJ_Z(void* /*class*/* p0, const wchar_t* p1, long* p2, long p3) {
    return 0;
}

// Symbol: ?PX_Picture@@YAHPEAVCPropExchange@@PEB_WAEAVCPictureHolder@@2@Z
extern "C" int MS_ABI impl__PX_Picture__YAHPEAVCPropExchange__PEB_WAEAVCPictureHolder__2_Z(void* /*class*/* p0, const wchar_t* p1, void* /*class*/* p2, void* /*class*/* p3) {
    return 0;
}

// Symbol: ?PX_Picture@@YAHPEAVCPropExchange@@PEB_WAEAVCPictureHolder@@@Z
extern "C" int MS_ABI impl__PX_Picture__YAHPEAVCPropExchange__PEB_WAEAVCPictureHolder___Z(void* /*class*/* p0, const wchar_t* p1, void* /*class*/* p2) {
    return 0;
}

// Symbol: ?PX_Short@@YAHPEAVCPropExchange@@PEB_WAEAF@Z
extern "C" int MS_ABI impl__PX_Short__YAHPEAVCPropExchange__PEB_WAEAF_Z(void* /*class*/* p0, const wchar_t* p1, short* p2) {
    return 0;
}

// Symbol: ?PX_Short@@YAHPEAVCPropExchange@@PEB_WAEAFF@Z
extern "C" int MS_ABI impl__PX_Short__YAHPEAVCPropExchange__PEB_WAEAFF_Z(void* /*class*/* p0, const wchar_t* p1, short* p2, short p3) {
    return 0;
}

// Symbol: ?PX_String@@YAHPEAVCPropExchange@@PEB_WAEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@1@Z
extern "C" int MS_ABI impl__PX_String__YAHPEAVCPropExchange__PEB_WAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__1_Z(void* /*class*/* p0, const wchar_t* p1, void* /*class*/* p2, void** p3, void* p4, const wchar_t* p5) {
    return 0;
}

// Symbol: ?PX_String@@YAHPEAVCPropExchange@@PEB_WAEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@@Z
extern "C" int MS_ABI impl__PX_String__YAHPEAVCPropExchange__PEB_WAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(void* /*class*/* p0, const wchar_t* p1, void* /*class*/* p2, void** p3, void* p4) {
    return 0;
}

// Symbol: ?PX_String@@YAHPEAVCPropExchange@@PEB_WAEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@AEBV23@@Z
extern "C" int MS_ABI impl__PX_String__YAHPEAVCPropExchange__PEB_WAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__AEBV23__Z(void* /*class*/* p0, const wchar_t* p1, void* /*class*/* p2, void** p3, void* p4, const void* /*class*/* p5) {
    return 0;
}

// Symbol: ?PX_ULong@@YAHPEAVCPropExchange@@PEB_WAEAK@Z
extern "C" int MS_ABI impl__PX_ULong__YAHPEAVCPropExchange__PEB_WAEAK_Z(void* /*class*/* p0, const wchar_t* p1, unsigned long* p2) {
    return 0;
}

// Symbol: ?PX_ULong@@YAHPEAVCPropExchange@@PEB_WAEAKK@Z
extern "C" int MS_ABI impl__PX_ULong__YAHPEAVCPropExchange__PEB_WAEAKK_Z(void* /*class*/* p0, const wchar_t* p1, unsigned long* p2, unsigned long p3) {
    return 0;
}

// Symbol: ?PX_UShort@@YAHPEAVCPropExchange@@PEB_WAEAG@Z
extern "C" int MS_ABI impl__PX_UShort__YAHPEAVCPropExchange__PEB_WAEAG_Z(void* /*class*/* p0, const wchar_t* p1, unsigned short* p2) {
    return 0;
}

// Symbol: ?PX_UShort@@YAHPEAVCPropExchange@@PEB_WAEAGG@Z
extern "C" int MS_ABI impl__PX_UShort__YAHPEAVCPropExchange__PEB_WAEAGG_Z(void* /*class*/* p0, const wchar_t* p1, unsigned short* p2, unsigned short p3) {
    return 0;
}

// Symbol: ?PX_VBXFontConvert@@YAHPEAVCPropExchange@@AEAVCFontHolder@@@Z
extern "C" int MS_ABI impl__PX_VBXFontConvert__YAHPEAVCPropExchange__AEAVCFontHolder___Z(void* /*class*/* p0, void* /*class*/* p1) {
    return 0;
}
