// Native GlobalFree helper releases every recorded lock before freeing.
#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxwin.h"
// Symbol: ?AfxGlobalFree@@YAXPEAX@Z
extern "C" void AFXAPI impl__AfxGlobalFree__YAXPEAX_Z(void* memory) {
    if(memory) {
        unsigned locks=static_cast<BYTE>(::GlobalFlags(memory));
        while(locks--)::GlobalUnlock(memory);
        ::GlobalFree(memory);
    }
}
