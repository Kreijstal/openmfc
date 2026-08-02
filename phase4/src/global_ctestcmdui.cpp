// OpenMFC implementation of CTestCmdUI.
//
// CTestCmdUI is a CCmdUI-like helper used by test harness code. The behavior here
// mirrors CCmdUI's exported virtuals and keeps layout aligned with the existing
// shim representation used in menucore.cpp.

#include <cstddef>
#include <cstring>

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

extern void* const g_CTestCmdUI_vtbl[4];

} // namespace

// Symbol: ??0CTestCmdUI@@QEAA@XZ
extern "C" void* MS_ABI impl___0CTestCmdUI__QEAA_XZ(void* pThis) {
    if (!pThis) {
        return nullptr;
    }
    auto* ui = static_cast<CCmdUIShim*>(pThis);
    std::memset(ui, 0, sizeof(*ui));
    ui->vfptr = g_CTestCmdUI_vtbl;
    return ui;
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
    pThis->CCmdUI::Enable(bOn);
}

// Symbol: ?SetCheck@CTestCmdUI@@UEAAXH@Z
extern "C" void MS_ABI impl__SetCheck_CTestCmdUI__UEAAXH_Z(CCmdUI* pThis, int nCheck) {
    if (!pThis) {
        return;
    }
    pThis->CCmdUI::SetCheck(nCheck);
}

// Symbol: ?SetRadio@CTestCmdUI@@UEAAXH@Z
extern "C" void MS_ABI impl__SetRadio_CTestCmdUI__UEAAXH_Z(CCmdUI* pThis, int bOn) {
    if (!pThis) {
        return;
    }
    pThis->CCmdUI::SetCheck(bOn ? 1 : 0);
}

// Symbol: ?SetText@CTestCmdUI@@UEAAXPEB_W@Z
extern "C" void MS_ABI impl__SetText_CTestCmdUI__UEAAXPEB_W_Z(CCmdUI* pThis, const wchar_t* lpszText) {
    if (!pThis) {
        return;
    }
    pThis->CCmdUI::SetText(lpszText);
}

namespace {

void MS_ABI vt_Enable(void* pThis, int bOn) {
    impl__Enable_CTestCmdUI__UEAAXH_Z(static_cast<CCmdUI*>(pThis), bOn);
}

void MS_ABI vt_SetCheck(void* pThis, int nCheck) {
    impl__SetCheck_CTestCmdUI__UEAAXH_Z(static_cast<CCmdUI*>(pThis), nCheck);
}

void MS_ABI vt_SetRadio(void* pThis, int bOn) {
    impl__SetRadio_CTestCmdUI__UEAAXH_Z(static_cast<CCmdUI*>(pThis), bOn);
}

void MS_ABI vt_SetText(void* pThis, const wchar_t* text) {
    impl__SetText_CTestCmdUI__UEAAXPEB_W_Z(static_cast<CCmdUI*>(pThis), text);
}

void* const g_CTestCmdUI_vtbl[4] = {
    reinterpret_cast<void*>(&vt_Enable),
    reinterpret_cast<void*>(&vt_SetCheck),
    reinterpret_cast<void*>(&vt_SetRadio),
    reinterpret_cast<void*>(&vt_SetText),
};

} // namespace
