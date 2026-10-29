// CMFCToolBarNameDialog — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp

#include "detail/ManualSmallStubImplementationsSupport.h"
#include "detail/ModuleThreadStateSupport.h"

extern "C" int MS_ABI impl__OnInitDialog_CDialog__UEAAHXZ(CDialog* pThis);
extern "C" DWORD MS_ABI impl__GetExStyle_CWnd__QEBAKXZ(const CWnd* pThis);
extern "C" int MS_ABI impl__ModifyStyleEx_CWnd__QEAAHKKI_Z(
    CWnd* pThis, unsigned long dwRemove, unsigned long dwAdd, unsigned int nFlags);
extern "C" BOOL MS_ABI impl__EnableWindow_CWnd__QEAAHH_Z(CWnd* pThis, BOOL bEnable);
extern "C" int MS_ABI impl__UpdateData_CWnd__QEAAHH_Z(CWnd* pThis, int bSaveAndValidate);
extern "C" void MS_ABI impl__DDX_Control__YAXPEAVCDataExchange__HAEAVCWnd___Z(
    void* pDX, int nIDC, void* pv);
extern "C" void MS_ABI impl__DDX_Text__YAXPEAVCDataExchange__HAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
    void* pDX, int nIDC, void* pv);

namespace {
// No CMFCToolBarNameDialog declaration exists in include/openmfc. Retail accesses
// m_btnOk at +0x130 and the CString data pointer at +0x218.
struct CMFCToolBarNameDialogLayout {
    unsigned char base[0x130];
    unsigned char m_btnOk[0xe8];
    const wchar_t* m_strToolbarName;
};
static_assert(offsetof(CMFCToolBarNameDialogLayout, m_btnOk) == 0x130,
              "retail CMFCToolBarNameDialog::m_btnOk offset");
static_assert(offsetof(CMFCToolBarNameDialogLayout, m_strToolbarName) == 0x218,
              "retail CMFCToolBarNameDialog::m_strToolbarName offset");

bool HasToolbarName(const void* pThis) {
    const auto* dialog = static_cast<const CMFCToolBarNameDialogLayout*>(pThis);
    const auto* text = reinterpret_cast<const unsigned char*>(dialog->m_strToolbarName);
    return *reinterpret_cast<const int*>(text - 0x10) != 0;
}
} // namespace

// Symbol: ??0CMFCToolBarNameDialog@@QEAA@PEAVCWnd@@@Z
// Still a stub: retail initializes the CDialog/CButton subobjects and toolbar-name
// CString and installs class vtables; this translation unit has no class/vtable
// model with which to reproduce those ABI-dependent pieces safely.
extern "C" void* MS_ABI impl___0CMFCToolBarNameDialog__QEAA_PEAVCWnd___Z(void* pThis, void* pParent) {
    (void)pParent;
    return pThis;
}
// Symbol: ?DoDataExchange@CMFCToolBarNameDialog@@MEAAXPEAVCDataExchange@@@Z
// Retail mfc140u RVA 0x1765e0 calls DDX_Control(pDX, 1, this+0x130), then tail-jumps
// to DDX_Text(pDX, 0x4084, this+0x218). Both thunk definitions are present in
// core/runtime/DdxExchange.cpp.
extern "C" void MS_ABI impl__DoDataExchange_CMFCToolBarNameDialog__MEAAXPEAVCDataExchange___Z(
    void* pThis, void* pDataExchange) {
    auto* self = static_cast<unsigned char*>(pThis);
    impl__DDX_Control__YAXPEAVCDataExchange__HAEAVCWnd___Z(
        pDataExchange, 1, self + 0x130);
    impl__DDX_Text__YAXPEAVCDataExchange__HAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
        pDataExchange, 0x4084, self + 0x218);
}
// Symbol: ?OnInitDialog@CMFCToolBarNameDialog@@MEAAHXZ
// Retail mfc140.dll (ANSI twin) entry RVA 0x174cd0; --u has no resolved RVA for
// this symbol. It ignores CDialog::OnInitDialog's result, checks the current
// thread's virtual GetMainWnd(), then re-fetches the thread/window and calls
// GetExStyle on that second result (without a second null guard). If the RTL bit
// is set it applies it, then enables m_btnOk iff the CString length is nonzero
// and returns TRUE.
extern "C" int MS_ABI impl__OnInitDialog_CMFCToolBarNameDialog__MEAAHXZ(void* pThis) {
    (void)impl__OnInitDialog_CDialog__UEAAHXZ(reinterpret_cast<CDialog*>(pThis));

    auto* threadState = impl__AfxGetModuleThreadState__YAPEAVAFX_MODULE_THREAD_STATE__XZ();
    CWinThread* thread = threadState->m_pCurrentWinThread;
    if (thread != nullptr && thread->GetMainWnd() != nullptr) {
        threadState = impl__AfxGetModuleThreadState__YAPEAVAFX_MODULE_THREAD_STATE__XZ();
        thread = threadState->m_pCurrentWinThread;
        CWnd* mainWindow = thread != nullptr ? thread->GetMainWnd() : nullptr;
        if ((impl__GetExStyle_CWnd__QEBAKXZ(mainWindow) & WS_EX_LAYOUTRTL) != 0) {
            impl__ModifyStyleEx_CWnd__QEAAHKKI_Z(
                reinterpret_cast<CWnd*>(pThis), 0, WS_EX_LAYOUTRTL, 0);
        }
    }

    auto* dialog = static_cast<CMFCToolBarNameDialogLayout*>(pThis);
    impl__EnableWindow_CWnd__QEAAHH_Z(
        reinterpret_cast<CWnd*>(dialog->m_btnOk), HasToolbarName(pThis) ? TRUE : FALSE);
    return TRUE;
}
// Symbol: ?OnUpdateToolbarName@CMFCToolBarNameDialog@@IEAAXXZ
// Retail mfc140.dll (ANSI twin) entry RVA 0x174c90; --u has no resolved RVA for
// this symbol. It unconditionally calls UpdateData(TRUE), then enables the OK
// button exactly when the CString length is nonzero.
extern "C" void MS_ABI impl__OnUpdateToolbarName_CMFCToolBarNameDialog__IEAAXXZ(void* pThis) {
    impl__UpdateData_CWnd__QEAAHH_Z(reinterpret_cast<CWnd*>(pThis), TRUE);
    auto* dialog = static_cast<CMFCToolBarNameDialogLayout*>(pThis);
    impl__EnableWindow_CWnd__QEAAHH_Z(
        reinterpret_cast<CWnd*>(dialog->m_btnOk), HasToolbarName(pThis) ? TRUE : FALSE);
}
