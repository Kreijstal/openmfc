// OpenMFC: hand-written thunks that gen_thunks.py cannot emit.
//
// These route DLL exports through real C++ methods but need shapes the
// generator does not produce: a dynamic_cast down to the concrete tab-ctrl
// type, a 64-bit width argument, or a return-value cast. They previously lived
// inside the generated thunks.cpp, where regenerating that file silently
// dropped them (the generator overwrites its own output and cannot see markers
// in it). Housed here in a hand-owned TU, their `// Symbol:` markers are seen
// by gen_thunks.py, which then correctly skips generating them -- so thunks.cpp
// can be regenerated freely without losing them.

#define OPENMFC_APPCORE_IMPL

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

#include "openmfc/afxwin.h"
#include "openmfc/afxole.h"
#include "openmfc/afxmfc.h"
#include "openmfc/afxdb.h"
#include "openmfc/afx.h"

// Symbol: ?Enable@CCmdUI@@UEAAXH@Z
// CCmdUI::Enable
extern "C" void MS_ABI impl__Enable_CCmdUI__UEAAXH_Z(CCmdUI* pThis, int p0) {
    if (!pThis) return;
    pThis->Enable(p0);
}

// Symbol: ?Read@CArchive@@QEAAIPEAXI@Z
// CArchive::Read
extern "C" unsigned int MS_ABI impl__Read_CArchive__QEAAIPEAXI_Z(CArchive* pThis, void* p0, unsigned int p1) {
    return (unsigned int)pThis->Read(p0, p1);
}

// Symbol: ?Write@CArchive@@QEAAXPEBXI@Z
// CArchive::Write
extern "C" void MS_ABI impl__Write_CArchive__QEAAXPEBXI_Z(CArchive* pThis, const void* p0, unsigned int p1) {
    pThis->Write(p0, p1);
}

// Symbol: ?SetLength@CMemFile@@UEAAX_K@Z
// CMemFile::SetLength
extern "C" void MS_ABI impl__SetLength_CMemFile__UEAAX_K_Z(CMemFile* pThis, unsigned __int64 p0) {
    pThis->SetLength(p0);
}

// Symbol: ?RecalcLayout@CFrameWnd@@UEAAXH@Z
// CFrameWnd::RecalcLayout
extern "C" void MS_ABI impl__RecalcLayout_CFrameWnd__UEAAXH_Z(CFrameWnd* pThis, int p0) {
    pThis->RecalcLayout(p0);
}

// Symbol: ?OnRotateListValue@CMFCPropertyGridProperty@@MEAAHH@Z
// CMFCPropertyGridProperty::OnRotateListValue
extern "C" int MS_ABI impl__OnRotateListValue_CMFCPropertyGridProperty__MEAAHH_Z(CMFCPropertyGridProperty* pThis, int p0) {
    return (int)pThis->OnRotateListValue(p0);
}

// Symbol: ?SetText@CMFCRibbonBaseElement@@UEAAXPEB_W@Z
// CMFCRibbonBaseElement::SetText
extern "C" void MS_ABI impl__SetText_CMFCRibbonBaseElement__UEAAXPEB_W_Z(CMFCRibbonBaseElement* pThis, const wchar_t* p0) {
    pThis->SetText(p0);
}

// Symbol: ?SetTabBorderSize@CMFCBaseTabCtrl@@UEAAXHH@Z
// CMFCBaseTabCtrl::SetTabBorderSize
extern "C" void MS_ABI impl__SetTabBorderSize_CMFCBaseTabCtrl__UEAAXHH_Z(CMFCBaseTabCtrl* pThis, int p0, int p1) {
    (void)p1;
    if (!pThis) {
        return;
    }
    if (auto* pTabCtrl = dynamic_cast<CMFCTabCtrl*>(pThis)) {
        pTabCtrl->SetTabBorderSize(p0);
    }
}

// Symbol: ?SetTabsHeight@CMFCBaseTabCtrl@@UEAAXXZ
// CMFCBaseTabCtrl::SetTabsHeight
extern "C" void MS_ABI impl__SetTabsHeight_CMFCBaseTabCtrl__UEAAXXZ(CMFCBaseTabCtrl* pThis) {
    if (!pThis) {
        return;
    }
    if (auto* pTabCtrl = dynamic_cast<CMFCTabCtrl*>(pThis)) {
        pTabCtrl->SetTabsHeight(0);
    }
}

// Symbol: ?SetTabsHeight@CMFCTabCtrl@@MEAAXXZ
// CMFCTabCtrl::SetTabsHeight
extern "C" void MS_ABI impl__SetTabsHeight_CMFCTabCtrl__MEAAXXZ(CMFCTabCtrl* pThis) {
    if (!pThis) {
        return;
    }
    pThis->SetTabsHeight(0);
}
