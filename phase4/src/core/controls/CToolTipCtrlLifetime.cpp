// Native module ownership gate and tooltip lifetime (mfc140u 0x274e20).
#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxwin.h"
class CToolTipCtrl;
extern "C" AFX_MODULE_STATE* AFXAPI impl__AfxGetModuleState__YAPEAVAFX_MODULE_STATE__XZ();
// Symbol: ?DestroyToolTipCtrl@CToolTipCtrl@@QEAAHXZ
extern "C" int AFXAPI impl__DestroyToolTipCtrl_CToolTipCtrl__QEAAHXZ(CToolTipCtrl* self) {
    auto* window = reinterpret_cast<CWnd*>(self);
    auto* module = impl__AfxGetModuleState__YAPEAVAFX_MODULE_STATE__XZ();
    if (module != window->m_pModuleState)
        return FALSE;
    window->DestroyWindow();
    using Delete = void*(AFXAPI*)(void*, UINT);
    auto* table = *reinterpret_cast<void***>(self);
    reinterpret_cast<Delete>(table[1])(self, 1);
    return TRUE;
}
