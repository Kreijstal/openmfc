#include "openmfc/afxstr.h"
#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif
// Symbol: ?AfxGetStringManager@@YAPEAUIAtlStringMgr@ATL@@XZ
extern "C" IAtlStringMgr* MS_ABI impl__AfxGetStringManager__YAPEAUIAtlStringMgr_ATL__XZ() {
    return OpenMFC_GetStringMgr();
}
