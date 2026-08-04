// COleCmdUI — OpenMFC implementation.
// Sources: global_ctordtor_param.cpp, manual_small_stub_implementations.cpp, olecore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/ManualSmallStubImplementationsSupport.h"
#include "detail/OlecoreSupport.h"

// Real parameterized-constructor export thunks for header-defined, publicly
// constructible MFC classes. Each viewed-as MS_ABI thunk placement-constructs
// the object with the real ctor body (defined in filecore/olecore/dlgcommon/
// inetcore or inline in headers) -- NOT a stub. Completes classes that were one
// export short of full coverage.
#include "openmfc/afx.h"
#include "openmfc/afxwin.h"
#include "openmfc/afxmfc.h"
#include "openmfc/afxole.h"
#include "openmfc/afxdisp.h"
#include "openmfc/afxdb.h"
#include "openmfc/afxinet.h"
#include <new>
#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif


















// Symbol: ??0COleCmdUI@@QEAA@PEAU_tagOLECMD@@KPEBU_GUID@@@Z
extern "C" void* MS_ABI impl___0COleCmdUI__QEAA_PEAU_tagOLECMD__KPEBU_GUID___Z(
        void* p, OLECMD* rgCmds, unsigned long cCmds, const GUID* pGuid) {
    return new (p) COleCmdUI(rgCmds, cCmds, pGuid);
}
// Implemented in thunks.cpp; this unit had a second, unlinked copy.
// Symbol: ?DoUpdate@COleCmdUI@@UEAAHPEAVCCmdTarget@@H@Z
COleCmdUI::COleCmdUI(OLECMD* rgCmds, ULONG cCmds, const GUID* pGuid)
    : m_pCmd(nullptr), m_cCmds(cCmds), m_pGuid(pGuid), m_rgCmds(rgCmds) {
    memset(_olecmdui_padding, 0, sizeof(_olecmdui_padding));
}
COleCmdUI::~COleCmdUI() {
    m_pCmd = nullptr;
    m_cCmds = 0;
    m_pGuid = nullptr;
    m_rgCmds = nullptr;
}
void COleCmdUI::Enable(BOOL bOn) {
    if (m_rgCmds && m_cCmds > 0) {
        m_rgCmds[0].cmdf = bOn ? OLECMDF_ENABLED : OLECMDF_SUPPORTED;
    }
}
void COleCmdUI::SetCheck(int nCheck) {
    if (m_rgCmds && m_cCmds > 0) {
        if (nCheck == 1)
            m_rgCmds[0].cmdf |= OLECMDF_LATCHED;
        else
            m_rgCmds[0].cmdf &= ~OLECMDF_LATCHED;
    }
}
void COleCmdUI::SetText(const wchar_t* lpszText) {
    (void)lpszText;
}
int COleCmdUI::DoUpdate(CCmdTarget* pTarget, BOOL bDisableIfNoHandler) {
    // Minimal fallback implementation: expose command as supported/enabled
    // without invoking full MFC command-routing.
    (void)pTarget;
    if (!m_rgCmds || m_cCmds == 0) {
        return FALSE;
    }

    m_rgCmds[0].cmdf |= OLECMDF_SUPPORTED;
    if (bDisableIfNoHandler) {
        Enable(FALSE);
    }
    return TRUE;
}
