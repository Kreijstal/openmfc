// OpenMFC implementation of CTestCmdUI.
//
// CTestCmdUI is a CCmdUI-like helper used by test harness code. The behavior here
// mirrors CCmdUI's exported virtuals and keeps layout aligned with the existing
// shim representation used in menucore.cpp.

#include <cstddef>
#include <new>

#ifdef __GNUC__
#define MS_ABI __attribute__((ms_abi))
#else
#define MS_ABI
#endif

#include "openmfc/afxole.h"

namespace {

struct CCmdUIShim {
    void* vfptr;
    unsigned m_nID;
    CCmdUI* m_pOther;
    int m_nIndex;
    CMenu* m_pMenu;
    CMenu* m_pSubMenu;
    char _padding[8];
};

static_assert(sizeof(CCmdUIShim) == 56, "CCmdUIShim size must be 56");
static_assert(offsetof(CCmdUIShim, m_nID) == 8, "m_nID @8");
static_assert(offsetof(CCmdUIShim, m_pOther) == 16, "m_pOther @16");
static_assert(offsetof(CCmdUIShim, m_nIndex) == 24, "m_nIndex @24");
static_assert(offsetof(CCmdUIShim, m_pMenu) == 32, "m_pMenu @32");
static_assert(offsetof(CCmdUIShim, m_pSubMenu) == 40, "m_pSubMenu @40");

// Optional hardening: make sure construction always starts from the same base
// layout that all command UI types in this project assume.
inline void* MS_ABI ConstructAsCCmdUI(void* pThis) {
    return new (pThis) CCmdUI();
}

} // namespace

// Symbol: ??0CTestCmdUI@@QEAA@XZ
extern "C" void* MS_ABI impl___0CTestCmdUI__QEAA_XZ(void* pThis) {
    if (!pThis) {
        return nullptr;
    }
    return ConstructAsCCmdUI(pThis);
}

// Symbol: ?Enable@CTestCmdUI@@UEAAXH@Z
extern "C" void MS_ABI impl__Enable_CTestCmdUI__UEAAXH_Z(CCmdUI* pThis, int bOn) {
    if (!pThis) {
        return;
    }
    auto* ui = reinterpret_cast<CCmdUIShim*>(pThis);
    if (!ui->m_pMenu && !ui->m_pSubMenu) {
        return;
    }
    pThis->Enable(bOn);
}

// Symbol: ?SetCheck@CTestCmdUI@@UEAAXH@Z
extern "C" void MS_ABI impl__SetCheck_CTestCmdUI__UEAAXH_Z(CCmdUI* pThis, int nCheck) {
    if (!pThis) {
        return;
    }
    pThis->SetCheck(nCheck);
}

// Symbol: ?SetRadio@CTestCmdUI@@UEAAXH@Z
extern "C" void MS_ABI impl__SetRadio_CTestCmdUI__UEAAXH_Z(CCmdUI* pThis, int bOn) {
    if (!pThis) {
        return;
    }
    pThis->SetCheck(bOn ? 1 : 0);
}

// Symbol: ?SetText@CTestCmdUI@@UEAAXPEB_W@Z
extern "C" void MS_ABI impl__SetText_CTestCmdUI__UEAAXPEB_W_Z(CCmdUI* pThis, const wchar_t* lpszText) {
    if (!pThis) {
        return;
    }
    pThis->SetText(lpszText);
}
