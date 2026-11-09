// Thunks — OpenMFC implementation.
// Sources: global_string_atl_thunks.cpp

#include "detail/StringAtlThunksSupport.h"






// Symbol: ??0?$CSimpleStringT@_W$00@ATL@@QEAA@AEBV01@@Z
extern "C" void* MS_ABI impl___0__CSimpleStringT__W_00_ATL__QEAA_AEBV01__Z(void* pThis, const void* src) {
    openmfc_str::AtlStrT<wchar_t>* self = reinterpret_cast<openmfc_str::AtlStrT<wchar_t>*>(pThis);
    self->ConstructCopy(*reinterpret_cast<const openmfc_str::AtlStrT<wchar_t>*>(src));
    return pThis;
}
// Symbol: ??0?$CSimpleStringT@_W$00@ATL@@QEAA@AEBV?$CSimpleStringT@_W$0A@@1@@Z
extern "C" void* MS_ABI impl___0__CSimpleStringT__W_00_ATL__QEAA_AEBV__CSimpleStringT__W_0A__1__Z(void* pThis, const void* src) {
    openmfc_str::AtlStrT<wchar_t>* self = reinterpret_cast<openmfc_str::AtlStrT<wchar_t>*>(pThis);
    self->ConstructCopy(*reinterpret_cast<const openmfc_str::AtlStrT<wchar_t>*>(src));
    return pThis;
}
// Symbol: ??0?$CSimpleStringT@_W$00@ATL@@QEAA@PEAUIAtlStringMgr@1@@Z
extern "C" void* MS_ABI impl___0__CSimpleStringT__W_00_ATL__QEAA_PEAUIAtlStringMgr_1__Z(void* pThis, IAtlStringMgr* mgr) {
    openmfc_str::AtlStrT<wchar_t>* self = reinterpret_cast<openmfc_str::AtlStrT<wchar_t>*>(pThis);
    self->ConstructMgr(mgr);
    return pThis;
}
// Symbol: ??0?$CSimpleStringT@_W$00@ATL@@QEAA@PEB_WHPEAUIAtlStringMgr@1@@Z
extern "C" void* MS_ABI impl___0__CSimpleStringT__W_00_ATL__QEAA_PEB_WHPEAUIAtlStringMgr_1__Z(void* pThis, const wchar_t* psz, int n, IAtlStringMgr* mgr) {
    openmfc_str::AtlStrT<wchar_t>* self = reinterpret_cast<openmfc_str::AtlStrT<wchar_t>*>(pThis);
    (void)mgr; self->ConstructPszN(psz, n);
    return pThis;
}
// Symbol: ??0?$CSimpleStringT@_W$00@ATL@@QEAA@PEB_WPEAUIAtlStringMgr@1@@Z
extern "C" void* MS_ABI impl___0__CSimpleStringT__W_00_ATL__QEAA_PEB_WPEAUIAtlStringMgr_1__Z(void* pThis, const wchar_t* psz, IAtlStringMgr* mgr) {
    openmfc_str::AtlStrT<wchar_t>* self = reinterpret_cast<openmfc_str::AtlStrT<wchar_t>*>(pThis);
    (void)mgr; self->ConstructPsz(psz);
    return pThis;
}


















// Symbol: ??0?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@QEAA@AEBV01@@Z
extern "C" void* MS_ABI impl___0__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAA_AEBV01__Z(void* pThis, const void* src) {
    openmfc_str::AtlStrT<wchar_t>* self = reinterpret_cast<openmfc_str::AtlStrT<wchar_t>*>(pThis);
    self->ConstructCopy(*reinterpret_cast<const openmfc_str::AtlStrT<wchar_t>*>(src));
    return pThis;
}
// Symbol: ??0?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@QEAA@DH@Z
extern "C" void* MS_ABI impl___0__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAA_DH_Z(void* pThis, char ch, int n) {
    openmfc_str::AtlStrT<wchar_t>* self = reinterpret_cast<openmfc_str::AtlStrT<wchar_t>*>(pThis);
    openmfc_str::AtlStrT<wchar_t> t; t.ConstructConvertN<char>(&ch, 1); self->ConstructChRep(t.GetLength() ? t.GetString()[0] : (wchar_t)0, n); t.Destruct();
    return pThis;
}
// Symbol: ??0?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@QEAA@PEAD@Z
extern "C" void* MS_ABI impl___0__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAA_PEAD_Z(void* pThis, char* psz) {
    openmfc_str::AtlStrT<wchar_t>* self = reinterpret_cast<openmfc_str::AtlStrT<wchar_t>*>(pThis);
    self->ConstructConvert<char>(psz);
    return pThis;
}
// Symbol: ??0?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@QEAA@PEAE@Z
extern "C" void* MS_ABI impl___0__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAA_PEAE_Z(void* pThis, unsigned char* psz) {
    openmfc_str::AtlStrT<wchar_t>* self = reinterpret_cast<openmfc_str::AtlStrT<wchar_t>*>(pThis);
    self->ConstructConvert<char>(reinterpret_cast<const char*>(psz));
    return pThis;
}
// Symbol: ??0?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@QEAA@PEAUIAtlStringMgr@1@@Z
extern "C" void* MS_ABI impl___0__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAA_PEAUIAtlStringMgr_1__Z(void* pThis, IAtlStringMgr* mgr) {
    openmfc_str::AtlStrT<wchar_t>* self = reinterpret_cast<openmfc_str::AtlStrT<wchar_t>*>(pThis);
    self->ConstructMgr(mgr);
    return pThis;
}
// Symbol: ??0?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@QEAA@PEA_W@Z
extern "C" void* MS_ABI impl___0__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAA_PEA_W_Z(void* pThis, wchar_t* psz) {
    openmfc_str::AtlStrT<wchar_t>* self = reinterpret_cast<openmfc_str::AtlStrT<wchar_t>*>(pThis);
    self->ConstructPsz(psz);
    return pThis;
}
// Symbol: ??0?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@QEAA@PEBD@Z
extern "C" void* MS_ABI impl___0__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAA_PEBD_Z(void* pThis, const char* psz) {
    openmfc_str::AtlStrT<wchar_t>* self = reinterpret_cast<openmfc_str::AtlStrT<wchar_t>*>(pThis);
    self->ConstructConvert<char>(psz);
    return pThis;
}
// Symbol: ??0?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@QEAA@PEBDH@Z
extern "C" void* MS_ABI impl___0__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAA_PEBDH_Z(void* pThis, const char* psz, int n) {
    openmfc_str::AtlStrT<wchar_t>* self = reinterpret_cast<openmfc_str::AtlStrT<wchar_t>*>(pThis);
    self->ConstructConvertN<char>(psz, n);
    return pThis;
}
// Symbol: ??0?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@QEAA@PEBDHPEAUIAtlStringMgr@1@@Z
extern "C" void* MS_ABI impl___0__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAA_PEBDHPEAUIAtlStringMgr_1__Z(void* pThis, const char* psz, int n, IAtlStringMgr* mgr) {
    openmfc_str::AtlStrT<wchar_t>* self = reinterpret_cast<openmfc_str::AtlStrT<wchar_t>*>(pThis);
    (void)mgr; self->ConstructConvertN<char>(psz, n);
    return pThis;
}
// Symbol: ??0?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@QEAA@PEBDPEAUIAtlStringMgr@1@@Z
extern "C" void* MS_ABI impl___0__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAA_PEBDPEAUIAtlStringMgr_1__Z(void* pThis, const char* psz, IAtlStringMgr* mgr) {
    openmfc_str::AtlStrT<wchar_t>* self = reinterpret_cast<openmfc_str::AtlStrT<wchar_t>*>(pThis);
    (void)mgr; self->ConstructConvert<char>(psz);
    return pThis;
}
// Symbol: ??0?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@QEAA@PEBE@Z
extern "C" void* MS_ABI impl___0__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAA_PEBE_Z(void* pThis, const unsigned char* psz) {
    openmfc_str::AtlStrT<wchar_t>* self = reinterpret_cast<openmfc_str::AtlStrT<wchar_t>*>(pThis);
    self->ConstructConvert<char>(reinterpret_cast<const char*>(psz));
    return pThis;
}
// Symbol: ??0?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@QEAA@PEBEPEAUIAtlStringMgr@1@@Z
extern "C" void* MS_ABI impl___0__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAA_PEBEPEAUIAtlStringMgr_1__Z(void* pThis, const unsigned char* psz, IAtlStringMgr* mgr) {
    openmfc_str::AtlStrT<wchar_t>* self = reinterpret_cast<openmfc_str::AtlStrT<wchar_t>*>(pThis);
    (void)mgr; self->ConstructConvert<char>(reinterpret_cast<const char*>(psz));
    return pThis;
}
// Symbol: ??0?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@QEAA@PEB_WH@Z
extern "C" void* MS_ABI impl___0__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAA_PEB_WH_Z(void* pThis, const wchar_t* psz, int n) {
    openmfc_str::AtlStrT<wchar_t>* self = reinterpret_cast<openmfc_str::AtlStrT<wchar_t>*>(pThis);
    self->ConstructPszN(psz, n);
    return pThis;
}
// Symbol: ??0?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@QEAA@PEB_WHPEAUIAtlStringMgr@1@@Z
extern "C" void* MS_ABI impl___0__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAA_PEB_WHPEAUIAtlStringMgr_1__Z(void* pThis, const wchar_t* psz, int n, IAtlStringMgr* mgr) {
    openmfc_str::AtlStrT<wchar_t>* self = reinterpret_cast<openmfc_str::AtlStrT<wchar_t>*>(pThis);
    (void)mgr; self->ConstructPszN(psz, n);
    return pThis;
}
// Symbol: ??0?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@QEAA@PEB_WPEAUIAtlStringMgr@1@@Z
extern "C" void* MS_ABI impl___0__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAA_PEB_WPEAUIAtlStringMgr_1__Z(void* pThis, const wchar_t* psz, IAtlStringMgr* mgr) {
    openmfc_str::AtlStrT<wchar_t>* self = reinterpret_cast<openmfc_str::AtlStrT<wchar_t>*>(pThis);
    (void)mgr; self->ConstructPsz(psz);
    return pThis;
}
// Symbol: ??0?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@QEAA@_WH@Z
extern "C" void* MS_ABI impl___0__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAA__WH_Z(void* pThis, wchar_t ch, int n) {
    openmfc_str::AtlStrT<wchar_t>* self = reinterpret_cast<openmfc_str::AtlStrT<wchar_t>*>(pThis);
    self->ConstructChRep(ch, n);
    return pThis;
}





// Symbol: ??4?$CSimpleStringT@_W$00@ATL@@QEAAAEAV01@AEBV01@@Z
extern "C" void* MS_ABI impl___4__CSimpleStringT__W_00_ATL__QEAAAEAV01_AEBV01__Z(void* pThis, const void* src) {
    openmfc_str::AtlStrT<wchar_t>* self = reinterpret_cast<openmfc_str::AtlStrT<wchar_t>*>(pThis);
    self->AssignCopy(*reinterpret_cast<const openmfc_str::AtlStrT<wchar_t>*>(src));
    return pThis;
}
// Symbol: ??4?$CSimpleStringT@_W$00@ATL@@QEAAAEAV01@AEBV?$CSimpleStringT@_W$0A@@1@@Z
extern "C" void* MS_ABI impl___4__CSimpleStringT__W_00_ATL__QEAAAEAV01_AEBV__CSimpleStringT__W_0A__1__Z(void* pThis, const void* src) {
    openmfc_str::AtlStrT<wchar_t>* self = reinterpret_cast<openmfc_str::AtlStrT<wchar_t>*>(pThis);
    self->AssignCopy(*reinterpret_cast<const openmfc_str::AtlStrT<wchar_t>*>(src));
    return pThis;
}
// Symbol: ??4?$CSimpleStringT@_W$00@ATL@@QEAAAEAV01@PEB_W@Z
extern "C" void* MS_ABI impl___4__CSimpleStringT__W_00_ATL__QEAAAEAV01_PEB_W_Z(void* pThis, const wchar_t* psz) {
    openmfc_str::AtlStrT<wchar_t>* self = reinterpret_cast<openmfc_str::AtlStrT<wchar_t>*>(pThis);
    self->AssignPsz(psz);
    return pThis;
}






// Symbol: ??4?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@QEAAAEAV01@AEBV01@@Z
extern "C" void* MS_ABI impl___4__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAAAEAV01_AEBV01__Z(void* pThis, const void* src) {
    openmfc_str::AtlStrT<wchar_t>* self = reinterpret_cast<openmfc_str::AtlStrT<wchar_t>*>(pThis);
    self->AssignCopy(*reinterpret_cast<const openmfc_str::AtlStrT<wchar_t>*>(src));
    return pThis;
}
// Symbol: ??4?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@QEAAAEAV01@D@Z
extern "C" void* MS_ABI impl___4__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAAAEAV01_D_Z(void* pThis, char ch) {
    openmfc_str::AtlStrT<wchar_t>* self = reinterpret_cast<openmfc_str::AtlStrT<wchar_t>*>(pThis);
    openmfc_str::AtlStrT<wchar_t> t; t.ConstructConvertN<char>(&ch, 1); self->AssignCopy(t); t.Destruct();
    return pThis;
}
// Symbol: ??4?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@QEAAAEAV01@PEBD@Z
extern "C" void* MS_ABI impl___4__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAAAEAV01_PEBD_Z(void* pThis, const char* psz) {
    openmfc_str::AtlStrT<wchar_t>* self = reinterpret_cast<openmfc_str::AtlStrT<wchar_t>*>(pThis);
    self->AssignConvert<char>(psz);
    return pThis;
}
// Symbol: ??4?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@QEAAAEAV01@PEBE@Z
extern "C" void* MS_ABI impl___4__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAAAEAV01_PEBE_Z(void* pThis, const unsigned char* psz) {
    openmfc_str::AtlStrT<wchar_t>* self = reinterpret_cast<openmfc_str::AtlStrT<wchar_t>*>(pThis);
    self->AssignConvert<char>(reinterpret_cast<const char*>(psz));
    return pThis;
}
// Symbol: ??4?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@QEAAAEAV01@_W@Z
extern "C" void* MS_ABI impl___4__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAAAEAV01__W_Z(void* pThis, wchar_t ch) {
    openmfc_str::AtlStrT<wchar_t>* self = reinterpret_cast<openmfc_str::AtlStrT<wchar_t>*>(pThis);
    self->AssignChar(ch);
    return pThis;
}






// Symbol: ??Y?$CSimpleStringT@_W$00@ATL@@QEAAAEAV01@AEBV01@@Z
extern "C" void* MS_ABI impl___Y__CSimpleStringT__W_00_ATL__QEAAAEAV01_AEBV01__Z(void* pThis, const void* src) {
    openmfc_str::AtlStrT<wchar_t>* self = reinterpret_cast<openmfc_str::AtlStrT<wchar_t>*>(pThis);
    const openmfc_str::AtlStrT<wchar_t>* o = reinterpret_cast<const openmfc_str::AtlStrT<wchar_t>*>(src); self->AppendPszN(o->GetString(), o->GetLength());
    return pThis;
}
// Symbol: ??Y?$CSimpleStringT@_W$00@ATL@@QEAAAEAV01@D@Z
extern "C" void* MS_ABI impl___Y__CSimpleStringT__W_00_ATL__QEAAAEAV01_D_Z(void* pThis, char ch) {
    openmfc_str::AtlStrT<wchar_t>* self = reinterpret_cast<openmfc_str::AtlStrT<wchar_t>*>(pThis);
    openmfc_str::AtlStrT<wchar_t> t; t.ConstructConvertN<char>(&ch, 1); self->AppendPszN(t.GetString(), t.GetLength()); t.Destruct();
    return pThis;
}
// Symbol: ??Y?$CSimpleStringT@_W$00@ATL@@QEAAAEAV01@E@Z
extern "C" void* MS_ABI impl___Y__CSimpleStringT__W_00_ATL__QEAAAEAV01_E_Z(void* pThis, unsigned char ch) {
    openmfc_str::AtlStrT<wchar_t>* self = reinterpret_cast<openmfc_str::AtlStrT<wchar_t>*>(pThis);
    char c = (char)ch; self->AppendConvert<char>(&c);
    return pThis;
}
// Symbol: ??Y?$CSimpleStringT@_W$00@ATL@@QEAAAEAV01@PEB_W@Z
extern "C" void* MS_ABI impl___Y__CSimpleStringT__W_00_ATL__QEAAAEAV01_PEB_W_Z(void* pThis, const wchar_t* psz) {
    openmfc_str::AtlStrT<wchar_t>* self = reinterpret_cast<openmfc_str::AtlStrT<wchar_t>*>(pThis);
    self->AppendPsz(psz);
    return pThis;
}
// Symbol: ??Y?$CSimpleStringT@_W$00@ATL@@QEAAAEAV01@_W@Z
extern "C" void* MS_ABI impl___Y__CSimpleStringT__W_00_ATL__QEAAAEAV01__W_Z(void* pThis, wchar_t ch) {
    openmfc_str::AtlStrT<wchar_t>* self = reinterpret_cast<openmfc_str::AtlStrT<wchar_t>*>(pThis);
    self->AppendChar(ch);
    return pThis;
}
// Symbol: ??Y?$CStringT@DV?$StrTraitMFC_DLL@DV?$ChTraitsCRT@D@ATL@@@@@ATL@@QEAAAEAV01@AEBV?$CSimpleStringT@D$00@1@@Z
extern "C" void* MS_ABI impl___Y__CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL__QEAAAEAV01_AEBV__CSimpleStringT_D_00_1__Z(void* pThis, const void* src) {
    openmfc_str::AtlStrT<char>* self = reinterpret_cast<openmfc_str::AtlStrT<char>*>(pThis);
    const openmfc_str::AtlStrT<char>* o = reinterpret_cast<const openmfc_str::AtlStrT<char>*>(src); self->AppendPszN(o->GetString(), o->GetLength());
    return pThis;
}
// Symbol: ??Y?$CStringT@DV?$StrTraitMFC_DLL@DV?$ChTraitsCRT@D@ATL@@@@@ATL@@QEAAAEAV01@D@Z
extern "C" void* MS_ABI impl___Y__CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL__QEAAAEAV01_D_Z(void* pThis, char ch) {
    openmfc_str::AtlStrT<char>* self = reinterpret_cast<openmfc_str::AtlStrT<char>*>(pThis);
    self->AppendChar(ch);
    return pThis;
}
// Symbol: ??Y?$CStringT@DV?$StrTraitMFC_DLL@DV?$ChTraitsCRT@D@ATL@@@@@ATL@@QEAAAEAV01@E@Z
extern "C" void* MS_ABI impl___Y__CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL__QEAAAEAV01_E_Z(void* pThis, unsigned char ch) {
    openmfc_str::AtlStrT<char>* self = reinterpret_cast<openmfc_str::AtlStrT<char>*>(pThis);
    self->AppendChar((char)ch);
    return pThis;
}
// Symbol: ??Y?$CStringT@DV?$StrTraitMFC_DLL@DV?$ChTraitsCRT@D@ATL@@@@@ATL@@QEAAAEAV01@PEBD@Z
extern "C" void* MS_ABI impl___Y__CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL__QEAAAEAV01_PEBD_Z(void* pThis, const char* psz) {
    openmfc_str::AtlStrT<char>* self = reinterpret_cast<openmfc_str::AtlStrT<char>*>(pThis);
    self->AppendPsz(psz);
    return pThis;
}
// Symbol: ??Y?$CStringT@DV?$StrTraitMFC_DLL@DV?$ChTraitsCRT@D@ATL@@@@@ATL@@QEAAAEAV01@_W@Z
extern "C" void* MS_ABI impl___Y__CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL__QEAAAEAV01__W_Z(void* pThis, wchar_t ch) {
    openmfc_str::AtlStrT<char>* self = reinterpret_cast<openmfc_str::AtlStrT<char>*>(pThis);
    openmfc_str::AtlStrT<char> t; t.ConstructConvertN<wchar_t>(&ch, 1); self->AppendPszN(t.GetString(), t.GetLength()); t.Destruct();
    return pThis;
}
// Symbol: ??Y?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@QEAAAEAV01@AEBV?$CSimpleStringT@_W$00@1@@Z
extern "C" void* MS_ABI impl___Y__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAAAEAV01_AEBV__CSimpleStringT__W_00_1__Z(void* pThis, const void* src) {
    openmfc_str::AtlStrT<wchar_t>* self = reinterpret_cast<openmfc_str::AtlStrT<wchar_t>*>(pThis);
    const openmfc_str::AtlStrT<wchar_t>* o = reinterpret_cast<const openmfc_str::AtlStrT<wchar_t>*>(src); self->AppendPszN(o->GetString(), o->GetLength());
    return pThis;
}
// Symbol: ??Y?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@QEAAAEAV01@D@Z
extern "C" void* MS_ABI impl___Y__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAAAEAV01_D_Z(void* pThis, char ch) {
    openmfc_str::AtlStrT<wchar_t>* self = reinterpret_cast<openmfc_str::AtlStrT<wchar_t>*>(pThis);
    openmfc_str::AtlStrT<wchar_t> t; t.ConstructConvertN<char>(&ch, 1); self->AppendPszN(t.GetString(), t.GetLength()); t.Destruct();
    return pThis;
}
// Symbol: ??Y?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@QEAAAEAV01@E@Z
extern "C" void* MS_ABI impl___Y__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAAAEAV01_E_Z(void* pThis, unsigned char ch) {
    openmfc_str::AtlStrT<wchar_t>* self = reinterpret_cast<openmfc_str::AtlStrT<wchar_t>*>(pThis);
    char c = (char)ch; self->AppendConvert<char>(&c);
    return pThis;
}
// Symbol: ??Y?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@QEAAAEAV01@PEBD@Z
extern "C" void* MS_ABI impl___Y__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAAAEAV01_PEBD_Z(void* pThis, const char* psz) {
    openmfc_str::AtlStrT<wchar_t>* self = reinterpret_cast<openmfc_str::AtlStrT<wchar_t>*>(pThis);
    self->AppendConvert<char>(psz);
    return pThis;
}
// Symbol: ??Y?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@QEAAAEAV01@PEB_W@Z
extern "C" void* MS_ABI impl___Y__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAAAEAV01_PEB_W_Z(void* pThis, const wchar_t* psz) {
    openmfc_str::AtlStrT<wchar_t>* self = reinterpret_cast<openmfc_str::AtlStrT<wchar_t>*>(pThis);
    self->AppendPsz(psz);
    return pThis;
}
// Symbol: ??Y?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@QEAAAEAV01@_W@Z
extern "C" void* MS_ABI impl___Y__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAAAEAV01__W_Z(void* pThis, wchar_t ch) {
    openmfc_str::AtlStrT<wchar_t>* self = reinterpret_cast<openmfc_str::AtlStrT<wchar_t>*>(pThis);
    self->AppendChar(ch);
    return pThis;
}
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




// Symbol: ?AppendChar@?$CSimpleStringT@_W$00@ATL@@QEAAX_W@Z
extern "C" void MS_ABI impl__AppendChar___CSimpleStringT__W_00_ATL__QEAAX_W_Z(void* pThis, wchar_t ch) {
    openmfc_str::AtlStrT<wchar_t>* self = reinterpret_cast<openmfc_str::AtlStrT<wchar_t>*>(pThis);
    self->AppendChar(ch);
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

// Symbol: ?Attach@?$CSimpleStringT@_W$00@ATL@@AEAAXPEAUCStringData@2@@Z
extern "C" void MS_ABI impl__Attach___CSimpleStringT__W_00_ATL__AEAAXPEAUCStringData_2__Z(void* pThis, CStringData* pd) {
    openmfc_str::AtlStrT<wchar_t>* self = reinterpret_cast<openmfc_str::AtlStrT<wchar_t>*>(pThis);
    self->Attach(pd);
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

// Symbol: ?CloneData@?$CSimpleStringT@_W$00@ATL@@CAPEAUCStringData@2@PEAU32@@Z
extern "C" CStringData* MS_ABI impl__CloneData___CSimpleStringT__W_00_ATL__CAPEAUCStringData_2_PEAU32__Z(CStringData* pd) {
    return openmfc_str::AtlStrT<wchar_t>::CloneData(pd);
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

// Symbol: ?Concatenate@?$CSimpleStringT@_W$00@ATL@@KAXAEAV12@PEB_WH1H@Z
extern "C" void MS_ABI impl__Concatenate___CSimpleStringT__W_00_ATL__KAXAEAV12_PEB_WH1H_Z(void* dst, const wchar_t* p1, int n1, const wchar_t* p2, int n2) {
    openmfc_str::AtlStrT<wchar_t>::Concatenate(*reinterpret_cast<openmfc_str::AtlStrT<wchar_t>*>(dst), p1, n1, p2, n2);
}
// Symbol: ?Construct@?$CStringT@DV?$StrTraitMFC_DLL@DV?$ChTraitsCRT@D@ATL@@@@@ATL@@SAXPEAV12@@Z
extern "C" void MS_ABI impl__Construct___CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL__SAXPEAV12__Z(void* p) {
    openmfc_str::AtlStrT<char>::ConstructStatic(reinterpret_cast<openmfc_str::AtlStrT<char>*>(p));
}
// Symbol: ?Construct@?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@SAXPEAV12@@Z
extern "C" void MS_ABI impl__Construct___CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__SAXPEAV12__Z(void* p) {
    openmfc_str::AtlStrT<wchar_t>::ConstructStatic(reinterpret_cast<openmfc_str::AtlStrT<wchar_t>*>(p));
}

// Symbol: ?CopyChars@?$CSimpleStringT@_W$00@ATL@@SAXPEA_WPEB_WH@Z
extern "C" void MS_ABI impl__CopyChars___CSimpleStringT__W_00_ATL__SAXPEA_WPEB_WH_Z(wchar_t* d, const wchar_t* s, int n) {
    openmfc_str::AtlStrT<wchar_t>::CopyChars(d, s, n);
}

// Symbol: ?CopyCharsOverlapped@?$CSimpleStringT@_W$00@ATL@@SAXPEA_WPEB_WH@Z
extern "C" void MS_ABI impl__CopyCharsOverlapped___CSimpleStringT__W_00_ATL__SAXPEA_WPEB_WH_Z(wchar_t* d, const wchar_t* s, int n) {
    openmfc_str::AtlStrT<wchar_t>::CopyCharsOverlapped(d, s, n);
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

// Symbol: ?Fork@?$CSimpleStringT@_W$00@ATL@@AEAAXH@Z
extern "C" void MS_ABI impl__Fork___CSimpleStringT__W_00_ATL__AEAAXH_Z(void* pThis, int n) {
    openmfc_str::AtlStrT<wchar_t>* self = reinterpret_cast<openmfc_str::AtlStrT<wchar_t>*>(pThis);
    self->Fork(n);
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

// Symbol: ?FreeExtra@?$CSimpleStringT@_W$00@ATL@@QEAAXXZ
extern "C" void MS_ABI impl__FreeExtra___CSimpleStringT__W_00_ATL__QEAAXXZ(void* pThis) {
    openmfc_str::AtlStrT<wchar_t>* self = reinterpret_cast<openmfc_str::AtlStrT<wchar_t>*>(pThis);
    self->FreeExtra();
}






// Symbol: ?GetData@?$CSimpleStringT@_W$00@ATL@@AEBAPEAUCStringData@2@XZ
extern "C" CStringData* MS_ABI impl__GetData___CSimpleStringT__W_00_ATL__AEBAPEAUCStringData_2_XZ(void* pThis) {
    const openmfc_str::AtlStrT<wchar_t>* cself = reinterpret_cast<const openmfc_str::AtlStrT<wchar_t>*>(pThis);
    return cself->GetData();
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


// Symbol: ?GetManager@?$CSimpleStringT@_W$00@ATL@@QEBAPEAUIAtlStringMgr@2@XZ
extern "C" IAtlStringMgr* MS_ABI impl__GetManager___CSimpleStringT__W_00_ATL__QEBAPEAUIAtlStringMgr_2_XZ(void* pThis) {
    const openmfc_str::AtlStrT<wchar_t>* cself = reinterpret_cast<const openmfc_str::AtlStrT<wchar_t>*>(pThis);
    return cself->GetManager();
}
// Symbol: ?GetManager@?$CStringT@DV?$StrTraitMFC_DLL@DV?$ChTraitsCRT@D@ATL@@@@@ATL@@QEBAPEAUIAtlStringMgr@2@XZ
extern "C" IAtlStringMgr* MS_ABI impl__GetManager___CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL__QEBAPEAUIAtlStringMgr_2_XZ(void* pThis) {
    const openmfc_str::AtlStrT<char>* cself = reinterpret_cast<const openmfc_str::AtlStrT<char>*>(pThis);
    return cself->GetManager();
}
// Symbol: ?GetManager@?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@QEBAPEAUIAtlStringMgr@2@XZ
extern "C" IAtlStringMgr* MS_ABI impl__GetManager___CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEBAPEAUIAtlStringMgr_2_XZ(void* pThis) {
    const openmfc_str::AtlStrT<wchar_t>* cself = reinterpret_cast<const openmfc_str::AtlStrT<wchar_t>*>(pThis);
    return cself->GetManager();
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

// Symbol: ?Left@?$CStringT@DV?$StrTraitMFC_DLL@DV?$ChTraitsCRT@D@ATL@@@@@ATL@@QEBA?AV12@H@Z
extern "C" void* MS_ABI impl__Left___CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL__QEBA_AV12_H_Z(void* pRet, void* pThis, int n) {
    openmfc_str::AtlStrT<char>* r = reinterpret_cast<openmfc_str::AtlStrT<char>*>(pRet);
    const openmfc_str::AtlStrT<char>* cself = reinterpret_cast<const openmfc_str::AtlStrT<char>*>(pThis);
    int L=cself->GetLength(); if(n<0)n=0; if(n>L)n=L; r->ConstructPszN(cself->GetString(), n);
    return pRet;
}
// Symbol: ?Left@?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@QEBA?AV12@H@Z
extern "C" void* MS_ABI impl__Left___CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEBA_AV12_H_Z(void* pRet, void* pThis, int n) {
    openmfc_str::AtlStrT<wchar_t>* r = reinterpret_cast<openmfc_str::AtlStrT<wchar_t>*>(pRet);
    const openmfc_str::AtlStrT<wchar_t>* cself = reinterpret_cast<const openmfc_str::AtlStrT<wchar_t>*>(pThis);
    int L=cself->GetLength(); if(n<0)n=0; if(n>L)n=L; r->ConstructPszN(cself->GetString(), n);
    return pRet;
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
// Symbol: ?Mid@?$CStringT@DV?$StrTraitMFC_DLL@DV?$ChTraitsCRT@D@ATL@@@@@ATL@@QEBA?AV12@H@Z
extern "C" void* MS_ABI impl__Mid___CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL__QEBA_AV12_H_Z(void* pRet, void* pThis, int first) {
    openmfc_str::AtlStrT<char>* r = reinterpret_cast<openmfc_str::AtlStrT<char>*>(pRet);
    const openmfc_str::AtlStrT<char>* cself = reinterpret_cast<const openmfc_str::AtlStrT<char>*>(pThis);
    int L=cself->GetLength(); if(first<0)first=0; if(first>L)first=L; r->ConstructPszN(cself->GetString()+first, L-first);
    return pRet;
}
// Symbol: ?Mid@?$CStringT@DV?$StrTraitMFC_DLL@DV?$ChTraitsCRT@D@ATL@@@@@ATL@@QEBA?AV12@HH@Z
extern "C" void* MS_ABI impl__Mid___CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL__QEBA_AV12_HH_Z(void* pRet, void* pThis, int first, int count) {
    openmfc_str::AtlStrT<char>* r = reinterpret_cast<openmfc_str::AtlStrT<char>*>(pRet);
    const openmfc_str::AtlStrT<char>* cself = reinterpret_cast<const openmfc_str::AtlStrT<char>*>(pThis);
    int L=cself->GetLength(); if(first<0)first=0; if(first>L)first=L; if(count<0)count=0; if(first+count>L)count=L-first; r->ConstructPszN(cself->GetString()+first, count);
    return pRet;
}
// Symbol: ?Mid@?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@QEBA?AV12@H@Z
extern "C" void* MS_ABI impl__Mid___CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEBA_AV12_H_Z(void* pRet, void* pThis, int first) {
    openmfc_str::AtlStrT<wchar_t>* r = reinterpret_cast<openmfc_str::AtlStrT<wchar_t>*>(pRet);
    const openmfc_str::AtlStrT<wchar_t>* cself = reinterpret_cast<const openmfc_str::AtlStrT<wchar_t>*>(pThis);
    int L=cself->GetLength(); if(first<0)first=0; if(first>L)first=L; r->ConstructPszN(cself->GetString()+first, L-first);
    return pRet;
}
// Symbol: ?Mid@?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@QEBA?AV12@HH@Z
extern "C" void* MS_ABI impl__Mid___CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEBA_AV12_HH_Z(void* pRet, void* pThis, int first, int count) {
    openmfc_str::AtlStrT<wchar_t>* r = reinterpret_cast<openmfc_str::AtlStrT<wchar_t>*>(pRet);
    const openmfc_str::AtlStrT<wchar_t>* cself = reinterpret_cast<const openmfc_str::AtlStrT<wchar_t>*>(pThis);
    int L=cself->GetLength(); if(first<0)first=0; if(first>L)first=L; if(count<0)count=0; if(first+count>L)count=L-first; r->ConstructPszN(cself->GetString()+first, count);
    return pRet;
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

// Symbol: ?Preallocate@?$CSimpleStringT@_W$00@ATL@@QEAAXH@Z
extern "C" void MS_ABI impl__Preallocate___CSimpleStringT__W_00_ATL__QEAAXH_Z(void* pThis, int n) {
    openmfc_str::AtlStrT<wchar_t>* self = reinterpret_cast<openmfc_str::AtlStrT<wchar_t>*>(pThis);
    self->Preallocate(n);
}

// Symbol: ?PrepareWrite2@?$CSimpleStringT@_W$00@ATL@@AEAAXH@Z
extern "C" void MS_ABI impl__PrepareWrite2___CSimpleStringT__W_00_ATL__AEAAXH_Z(void* pThis, int n) {
    openmfc_str::AtlStrT<wchar_t>* self = reinterpret_cast<openmfc_str::AtlStrT<wchar_t>*>(pThis);
    self->PrepareWrite2(n);
}

// Symbol: ?PrepareWrite@?$CSimpleStringT@_W$00@ATL@@AEAAPEA_WH@Z
extern "C" wchar_t* MS_ABI impl__PrepareWrite___CSimpleStringT__W_00_ATL__AEAAPEA_WH_Z(void* pThis, int n) {
    openmfc_str::AtlStrT<wchar_t>* self = reinterpret_cast<openmfc_str::AtlStrT<wchar_t>*>(pThis);
    return self->PrepareWrite(n);
}

// Symbol: ?Reallocate@?$CSimpleStringT@_W$00@ATL@@AEAAXH@Z
extern "C" void MS_ABI impl__Reallocate___CSimpleStringT__W_00_ATL__AEAAXH_Z(void* pThis, int n) {
    openmfc_str::AtlStrT<wchar_t>* self = reinterpret_cast<openmfc_str::AtlStrT<wchar_t>*>(pThis);
    self->Reallocate(n);
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
// Symbol: ?Right@?$CStringT@DV?$StrTraitMFC_DLL@DV?$ChTraitsCRT@D@ATL@@@@@ATL@@QEBA?AV12@H@Z
extern "C" void* MS_ABI impl__Right___CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL__QEBA_AV12_H_Z(void* pRet, void* pThis, int n) {
    openmfc_str::AtlStrT<char>* r = reinterpret_cast<openmfc_str::AtlStrT<char>*>(pRet);
    const openmfc_str::AtlStrT<char>* cself = reinterpret_cast<const openmfc_str::AtlStrT<char>*>(pThis);
    int L=cself->GetLength(); if(n<0)n=0; if(n>L)n=L; r->ConstructPszN(cself->GetString()+L-n, n);
    return pRet;
}
// Symbol: ?Right@?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@QEBA?AV12@H@Z
extern "C" void* MS_ABI impl__Right___CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEBA_AV12_H_Z(void* pRet, void* pThis, int n) {
    openmfc_str::AtlStrT<wchar_t>* r = reinterpret_cast<openmfc_str::AtlStrT<wchar_t>*>(pRet);
    const openmfc_str::AtlStrT<wchar_t>* cself = reinterpret_cast<const openmfc_str::AtlStrT<wchar_t>*>(pThis);
    int L=cself->GetLength(); if(n<0)n=0; if(n>L)n=L; r->ConstructPszN(cself->GetString()+L-n, n);
    return pRet;
}

// Symbol: ?SetAt@?$CSimpleStringT@_W$00@ATL@@QEAAXH_W@Z
extern "C" void MS_ABI impl__SetAt___CSimpleStringT__W_00_ATL__QEAAXH_W_Z(void* pThis, int i, wchar_t ch) {
    openmfc_str::AtlStrT<wchar_t>* self = reinterpret_cast<openmfc_str::AtlStrT<wchar_t>*>(pThis);
    self->SetAt(i, ch);
}

// Symbol: ?SetLength@?$CSimpleStringT@_W$00@ATL@@AEAAXH@Z
extern "C" void MS_ABI impl__SetLength___CSimpleStringT__W_00_ATL__AEAAXH_Z(void* pThis, int n) {
    openmfc_str::AtlStrT<wchar_t>* self = reinterpret_cast<openmfc_str::AtlStrT<wchar_t>*>(pThis);
    self->SetLength(n);
}

// Symbol: ?SetManager@?$CSimpleStringT@_W$00@ATL@@QEAAXPEAUIAtlStringMgr@2@@Z
extern "C" void MS_ABI impl__SetManager___CSimpleStringT__W_00_ATL__QEAAXPEAUIAtlStringMgr_2__Z(void* pThis, IAtlStringMgr* mgr) {
    openmfc_str::AtlStrT<wchar_t>* self = reinterpret_cast<openmfc_str::AtlStrT<wchar_t>*>(pThis);
    self->SetManagerEmpty(mgr);
}


// Symbol: ?SetString@?$CSimpleStringT@_W$00@ATL@@QEAAXPEB_W@Z
extern "C" void MS_ABI impl__SetString___CSimpleStringT__W_00_ATL__QEAAXPEB_W_Z(void* pThis, const wchar_t* psz) {
    openmfc_str::AtlStrT<wchar_t>* self = reinterpret_cast<openmfc_str::AtlStrT<wchar_t>*>(pThis);
    self->AssignPsz(psz);
}
// Symbol: ?SetString@?$CSimpleStringT@_W$00@ATL@@QEAAXPEB_WH@Z
extern "C" void MS_ABI impl__SetString___CSimpleStringT__W_00_ATL__QEAAXPEB_WH_Z(void* pThis, const wchar_t* psz, int n) {
    openmfc_str::AtlStrT<wchar_t>* self = reinterpret_cast<openmfc_str::AtlStrT<wchar_t>*>(pThis);
    if (!psz || n <= 0) { self->Empty(); } else { self->Empty(); self->AppendPszN(psz, n); }
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
// Symbol: ?SpanExcluding@?$CStringT@DV?$StrTraitMFC_DLL@DV?$ChTraitsCRT@D@ATL@@@@@ATL@@QEBA?AV12@PEBD@Z
extern "C" void* MS_ABI impl__SpanExcluding___CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL__QEBA_AV12_PEBD_Z(void* pRet, void* pThis, const char* set) {
    openmfc_str::AtlStrT<char>* r = reinterpret_cast<openmfc_str::AtlStrT<char>*>(pRet);
    const openmfc_str::AtlStrT<char>* cself = reinterpret_cast<const openmfc_str::AtlStrT<char>*>(pThis);
    const char* s=cself->GetString(); int i=0; for(; s[i]; ++i){ bool in=false; for(const char* q=set; q&&*q; ++q) if(*q==s[i]){in=true;break;} if(in) break; } r->ConstructPszN(s, i);
    return pRet;
}
// Symbol: ?SpanExcluding@?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@QEBA?AV12@PEB_W@Z
extern "C" void* MS_ABI impl__SpanExcluding___CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEBA_AV12_PEB_W_Z(void* pRet, void* pThis, const wchar_t* set) {
    openmfc_str::AtlStrT<wchar_t>* r = reinterpret_cast<openmfc_str::AtlStrT<wchar_t>*>(pRet);
    const openmfc_str::AtlStrT<wchar_t>* cself = reinterpret_cast<const openmfc_str::AtlStrT<wchar_t>*>(pThis);
    const wchar_t* s=cself->GetString(); int i=0; for(; s[i]; ++i){ bool in=false; for(const wchar_t* q=set; q&&*q; ++q) if(*q==s[i]){in=true;break;} if(in) break; } r->ConstructPszN(s, i);
    return pRet;
}
// Symbol: ?SpanIncluding@?$CStringT@DV?$StrTraitMFC_DLL@DV?$ChTraitsCRT@D@ATL@@@@@ATL@@QEBA?AV12@PEBD@Z
extern "C" void* MS_ABI impl__SpanIncluding___CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL__QEBA_AV12_PEBD_Z(void* pRet, void* pThis, const char* set) {
    openmfc_str::AtlStrT<char>* r = reinterpret_cast<openmfc_str::AtlStrT<char>*>(pRet);
    const openmfc_str::AtlStrT<char>* cself = reinterpret_cast<const openmfc_str::AtlStrT<char>*>(pThis);
    const char* s=cself->GetString(); int i=0; for(; s[i]; ++i){ bool in=false; for(const char* q=set; q&&*q; ++q) if(*q==s[i]){in=true;break;} if(!in) break; } r->ConstructPszN(s, i);
    return pRet;
}
// Symbol: ?SpanIncluding@?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@QEBA?AV12@PEB_W@Z
extern "C" void* MS_ABI impl__SpanIncluding___CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEBA_AV12_PEB_W_Z(void* pRet, void* pThis, const wchar_t* set) {
    openmfc_str::AtlStrT<wchar_t>* r = reinterpret_cast<openmfc_str::AtlStrT<wchar_t>*>(pRet);
    const openmfc_str::AtlStrT<wchar_t>* cself = reinterpret_cast<const openmfc_str::AtlStrT<wchar_t>*>(pThis);
    const wchar_t* s=cself->GetString(); int i=0; for(; s[i]; ++i){ bool in=false; for(const wchar_t* q=set; q&&*q; ++q) if(*q==s[i]){in=true;break;} if(!in) break; } r->ConstructPszN(s, i);
    return pRet;
}


// Symbol: ?StringLength@?$CSimpleStringT@_W$00@ATL@@SAHPEBD@Z
extern "C" int MS_ABI impl__StringLength___CSimpleStringT__W_00_ATL__SAHPEBD_Z(const char* psz) {
    int n = 0; if (psz) while (psz[n]) ++n; return n;
}
// Symbol: ?StringLength@?$CSimpleStringT@_W$00@ATL@@SAHPEB_W@Z
extern "C" int MS_ABI impl__StringLength___CSimpleStringT__W_00_ATL__SAHPEB_W_Z(const wchar_t* psz) {
    return openmfc_str::AtlStrT<wchar_t>::StringLength(psz);
}

// Symbol: ?ThrowMemoryException@?$CSimpleStringT@_W$00@ATL@@KAXXZ
extern "C" void MS_ABI impl__ThrowMemoryException___CSimpleStringT__W_00_ATL__KAXXZ() {
    RaiseException(0xE06D7363u, 0, 0, nullptr);
}
// Symbol: ?Tokenize@?$CStringT@DV?$StrTraitMFC_DLL@DV?$ChTraitsCRT@D@ATL@@@@@ATL@@QEBA?AV12@PEBDAEAH@Z
extern "C" void* MS_ABI impl__Tokenize___CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL__QEBA_AV12_PEBDAEAH_Z(void* pRet, void* pThis, const char* toks, int* piStart) {
    openmfc_str::AtlStrT<char>* r = reinterpret_cast<openmfc_str::AtlStrT<char>*>(pRet);
    const openmfc_str::AtlStrT<char>* cself = reinterpret_cast<const openmfc_str::AtlStrT<char>*>(pThis);
    Tokenize(cself, r, toks, *piStart);
    return pRet;
}
// Symbol: ?Tokenize@?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@QEBA?AV12@PEB_WAEAH@Z
extern "C" void* MS_ABI impl__Tokenize___CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEBA_AV12_PEB_WAEAH_Z(void* pRet, void* pThis, const wchar_t* toks, int* piStart) {
    openmfc_str::AtlStrT<wchar_t>* r = reinterpret_cast<openmfc_str::AtlStrT<wchar_t>*>(pRet);
    const openmfc_str::AtlStrT<wchar_t>* cself = reinterpret_cast<const openmfc_str::AtlStrT<wchar_t>*>(pThis);
    Tokenize(cself, r, toks, *piStart);
    return pRet;
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

// Symbol: ?Truncate@?$CSimpleStringT@_W$00@ATL@@QEAAXH@Z
extern "C" void MS_ABI impl__Truncate___CSimpleStringT__W_00_ATL__QEAAXH_Z(void* pThis, int n) {
    openmfc_str::AtlStrT<wchar_t>* self = reinterpret_cast<openmfc_str::AtlStrT<wchar_t>*>(pThis);
    self->Truncate(n);
}
