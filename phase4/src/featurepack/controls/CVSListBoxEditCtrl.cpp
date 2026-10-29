// CVSListBoxEditCtrl — OpenMFC implementation.
// Sources: dlgcommon.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/DlgcommonSupport.h"

#include <cstddef>
#include <cstdint>

extern "C" void MS_ABI impl__OnNcCalcSize_CMFCEditBrowseCtrl__IEAAXHPEAUtagNCCALCSIZE_PARAMS___Z(
    void* pThis, int bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp);
extern "C" __int64 MS_ABI impl__Default_CWnd__IEAA_JXZ(CWnd* pThis);
extern "C" LRESULT MS_ABI impl__DefWindowProcW_CWnd__MEAA_JI_K_J_Z(
    CWnd* pThis, UINT message, WPARAM wParam, LPARAM lParam);

namespace {

// CVSListBoxEditCtrl's retail base is the 0x150-byte CMFCEditBrowseCtrl.  The
// OpenMFC public declaration omits this base layout and the two members below;
// keep the retail tail offsets local rather than changing a shared header.
struct S_CVSListBoxEditCtrl {
    unsigned char base[0x150];
    int sizeLock;
    std::uint32_t alignment;
    CVSListBoxBase* list;
};
static_assert(offsetof(S_CVSListBoxEditCtrl, sizeLock) == 0x150, "CVSListBoxEditCtrl size lock offset");
static_assert(offsetof(S_CVSListBoxEditCtrl, list) == 0x158, "CVSListBoxEditCtrl list offset");
static_assert(offsetof(WINDOWPOS, flags) == 0x20, "WINDOWPOS flags offset");

} // namespace

// GetMessageMap/GetThisMessageMap for CVSListBoxEditCtrl live in
// featurepack/controls/MessageMaps.cpp (classCVSListBoxEditCtrl_msgmap, base map
// CMFCEditBrowseCtrl). Retail agrees: ?GetThisMessageMap@CVSListBoxEditCtrl@@KAPEBU
// AFX_MSGMAP@@XZ in mfc140.dll returns the AFX_MSGMAP at 0x18031de78, whose pfnGetBaseMap
// is ?GetThisMessageMap@CMFCEditBrowseCtrl@@KAPEBUAFX_MSGMAP@@XZ. The copies that used to
// be here returned CEdit's map, skipping CMFCEditBrowseCtrl.
// Symbol: ?GetRuntimeClass@CVSListBoxEditCtrl@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CVSListBoxEditCtrl__UEBAPEAUCRuntimeClass__XZ(const CVSListBoxEditCtrl* pThis) {
    return pThis ? pThis->GetRuntimeClass() : CVSListBoxEditCtrl::GetThisClass();
}
// Symbol: ?GetThisClass@CVSListBoxEditCtrl@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CVSListBoxEditCtrl__SAPEAUCRuntimeClass__XZ() {
    return CVSListBoxEditCtrl::GetThisClass();
}
// Retail mfc140.dll (ANSI twin) entry RVA 0x1c3240 stores the second argument
// at this+0x158 and the third at this+0x150; the body is byte-identical in the
// Unicode image.  No NULL-this guard is added, matching retail.
// Symbol: ?LockSize@CVSListBoxEditCtrl@@QEAAXPEAVCVSListBoxBase@@H@Z
extern "C" void MS_ABI impl__LockSize_CVSListBoxEditCtrl__QEAAXPEAVCVSListBoxBase__H_Z(CVSListBoxEditCtrl* pThis, CVSListBoxBase* pList, int nSize) {
    auto* layout = reinterpret_cast<S_CVSListBoxEditCtrl*>(pThis);
    layout->list = pList;
    layout->sizeLock = nSize;
}
// Disassembled retail mfc140.dll (ANSI twin) entry RVA 0x1c3290 (no
// mfc140u symbol-map entry): VK_TAB queries the CVSListBoxBase vtable slot
// +0x2f8 and checks its +0x194 flag. If both tests pass, it forwards an
// Enter key through CWnd::DefWindowProcA (the Unicode thunk is used here),
// preserving the packed key lParam; otherwise it calls CWnd::Default.
// Symbol: ?OnKeyDown@CVSListBoxEditCtrl@@IEAAXIII@Z
extern "C" void MS_ABI impl__OnKeyDown_CVSListBoxEditCtrl__IEAAXIII_Z(
    CVSListBoxEditCtrl* pThis, unsigned int nChar, unsigned int nRepCnt, unsigned int nFlags) {
    auto* layout = reinterpret_cast<S_CVSListBoxEditCtrl*>(pThis);
    if (nChar == VK_TAB) {
        CVSListBoxBase* pList = layout->list;
        using ListSlotFn = int (MS_ABI*)(CVSListBoxBase*);
        void** vtable = *reinterpret_cast<void***>(pList);
        const int slotResult = reinterpret_cast<ListSlotFn>(vtable[0x2f8 / sizeof(void*)])(pList);
        const int newItem = *reinterpret_cast<const int*>(
            reinterpret_cast<const unsigned char*>(pList) + 0x194);
        if (slotResult == 1 && newItem != 0) {
            const auto signExtend32 = [](std::uint32_t value) -> std::int64_t {
                return (value & 0x80000000u) != 0
                    ? static_cast<std::int64_t>(value) - 0x100000000ll
                    : static_cast<std::int64_t>(value);
            };
            const std::uint32_t shiftedFlags = static_cast<std::uint32_t>(nFlags << 16);
            const LPARAM keyLParam = static_cast<LPARAM>(
                signExtend32(shiftedFlags) | signExtend32(static_cast<std::uint32_t>(nRepCnt)));
            (void)impl__DefWindowProcW_CWnd__MEAA_JI_K_J_Z(
                reinterpret_cast<CWnd*>(pThis), WM_KEYDOWN, VK_RETURN, keyLParam);
            return;
        }
    }
    (void)impl__Default_CWnd__IEAA_JXZ(reinterpret_cast<CWnd*>(pThis));
}
// Disassembled retail mfc140.dll (ANSI twin) entry RVA 0x1c3270 checks
// this+0x150 and calls CMFCEditBrowseCtrl::OnNcCalcSize only when it is zero.
// The corresponding mfc140u name has no resolved RVA; this body uses the
// verified byte-identical ANSI implementation.  The base call uses its thunk.
// Symbol: ?OnNcCalcSize@CVSListBoxEditCtrl@@IEAAXHPEAUtagNCCALCSIZE_PARAMS@@@Z
extern "C" void MS_ABI impl__OnNcCalcSize_CVSListBoxEditCtrl__IEAAXHPEAUtagNCCALCSIZE_PARAMS___Z(CVSListBoxEditCtrl* pThis, int bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp) {
    if (reinterpret_cast<S_CVSListBoxEditCtrl*>(pThis)->sizeLock == 0) {
        impl__OnNcCalcSize_CMFCEditBrowseCtrl__IEAAXHPEAUtagNCCALCSIZE_PARAMS___Z(
            pThis, bCalcValidRects, lpncsp);
    }
}
// Disassembled retail mfc140.dll (ANSI twin) entry RVA 0x1c3250 sets
// SWP_NOSIZE when this+0x150 is nonzero, then always tail-jumps to
// CWnd::Default.  The matching mfc140u name has no resolved RVA; the function
// body is byte-identical.  The base call below uses its exported thunk.
// Symbol: ?OnWindowPosChanging@CVSListBoxEditCtrl@@IEAAXPEAUtagWINDOWPOS@@@Z
extern "C" void MS_ABI impl__OnWindowPosChanging_CVSListBoxEditCtrl__IEAAXPEAUtagWINDOWPOS___Z(CVSListBoxEditCtrl* pThis, WINDOWPOS* pWindowPos) {
    if (reinterpret_cast<S_CVSListBoxEditCtrl*>(pThis)->sizeLock != 0) {
        pWindowPos->flags |= SWP_NOSIZE;
    }
    (void)impl__Default_CWnd__IEAA_JXZ(reinterpret_cast<CWnd*>(pThis));
}
