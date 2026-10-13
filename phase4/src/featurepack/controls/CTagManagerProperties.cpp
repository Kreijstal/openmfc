#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxmfc.h"
#ifdef __GNUC__
#define MS_ABI __attribute__((ms_abi))
#else
#define MS_ABI
#endif
extern "C" BOOL MS_ABI impl__ReadBool_CTagManager__QEAAHAEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__AEAH_Z(void*,const CString*,BOOL*);
// Symbol: ?ReadBoolProp@@YAHAEAVCTagManager@@PEB_WAEAH@Z
extern "C" BOOL MS_ABI impl__ReadBoolProp__YAHAEAVCTagManager__PEB_WAEAH_Z(void* manager,const wchar_t* name,BOOL* value) {
    if(!manager || !value)return FALSE;
    CString tag(name);return impl__ReadBool_CTagManager__QEAAHAEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__AEAH_Z(manager,&tag,value);
}
