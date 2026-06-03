// Frame-window and font-dialog exported wrappers for remaining common-control shard symbols.

#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxwin.h"
#include <windows.h>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

extern "C" LRESULT MS_ABI impl__DefWindowProcW_CWnd__MEAA_JI_K_J_Z(
    CWnd* pThis, UINT message, WPARAM wParam, LPARAM lParam);

namespace {
struct CFontDialogAccess : CFontDialog {
    using CFontDialog::m_clrResult;
    using CFontDialog::m_lf;
    using CFontDialog::m_nPointSize;
};

static LOGFONTW* FontLog(CFontDialog* pThis) {
    return pThis ? reinterpret_cast<LOGFONTW*>(static_cast<CFontDialogAccess*>(pThis)->m_lf) : nullptr;
}

static const LOGFONTW* FontLog(const CFontDialog* pThis) {
    return pThis ? reinterpret_cast<const LOGFONTW*>(static_cast<const CFontDialogAccess*>(pThis)->m_lf) : nullptr;
}

static const AFX_MSGMAP* EmptyMessageMap() {
    return CWnd::GetThisMessageMap();
}
}  // namespace

// Symbol: ?CreateObject@CFrameWnd@@SAPEAVCObject@@XZ
extern "C" CObject* MS_ABI impl__CreateObject_CFrameWnd__SAPEAVCObject__XZ() {
    return CFrameWnd::CreateObject();
}

// Symbol: ?GetMessageMap@CFrameWnd@@MEBAPEBUAFX_MSGMAP@@XZ
extern "C" const AFX_MSGMAP* MS_ABI impl__GetMessageMap_CFrameWnd__MEBAPEBUAFX_MSGMAP__XZ(
    const CFrameWnd* pThis) {
    (void)pThis;
    return EmptyMessageMap();
}

// Symbol: ?GetRuntimeClass@CFrameWnd@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CFrameWnd__UEBAPEAUCRuntimeClass__XZ(
    const CFrameWnd* pThis) {
    return pThis ? pThis->GetRuntimeClass() : CFrameWnd::GetThisClass();
}

// Symbol: ?GetThisMessageMap@CFrameWnd@@KAPEBUAFX_MSGMAP@@XZ
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CFrameWnd__KAPEBUAFX_MSGMAP__XZ() {
    return EmptyMessageMap();
}

// Symbol: ?rectDefault@CFrameWnd@@2VCRect@@B
extern "C" const CRect MS_ABI impl__rectDefault_CFrameWnd__2VCRect__B = {};

// Symbol: ?CreateObject@CMDIChildWnd@@SAPEAVCObject@@XZ
extern "C" CObject* MS_ABI impl__CreateObject_CMDIChildWnd__SAPEAVCObject__XZ() {
    return CMDIChildWnd::CreateObject();
}

// Symbol: ?DefWindowProcW@CMDIChildWnd@@MEAA_JI_K_J@Z
extern "C" LRESULT MS_ABI impl__DefWindowProcW_CMDIChildWnd__MEAA_JI_K_J_Z(
    CMDIChildWnd* pThis, UINT message, WPARAM wParam, LPARAM lParam) {
    return impl__DefWindowProcW_CWnd__MEAA_JI_K_J_Z(pThis, message, wParam, lParam);
}

// Symbol: ?FillInLogFont@CFontDialog@@QEAAKAEBU_charformatw@@@Z
extern "C" DWORD MS_ABI impl__FillInLogFont_CFontDialog__QEAAKAEBU_charformatw___Z(
    CFontDialog* pThis, const CHARFORMATW* pCharFormat) {
    LOGFONTW* lf = FontLog(pThis);
    if (!lf || !pCharFormat) return 0;

    lf->lfWeight = (pCharFormat->dwEffects & CFE_BOLD) ? FW_BOLD : FW_NORMAL;
    lf->lfItalic = (BYTE)((pCharFormat->dwEffects & CFE_ITALIC) != 0);
    lf->lfUnderline = (BYTE)((pCharFormat->dwEffects & CFE_UNDERLINE) != 0);
    lf->lfStrikeOut = (BYTE)((pCharFormat->dwEffects & CFE_STRIKEOUT) != 0);
    lf->lfCharSet = pCharFormat->bCharSet;
    lf->lfPitchAndFamily = pCharFormat->bPitchAndFamily;
    if (pCharFormat->yHeight > 0) {
        lf->lfHeight = -MulDiv(pCharFormat->yHeight, 1, 20);
        static_cast<CFontDialogAccess*>(pThis)->m_nPointSize = (int)(pCharFormat->yHeight / 20);
    }
    lstrcpynW(lf->lfFaceName, pCharFormat->szFaceName, LF_FACESIZE);
    static_cast<CFontDialogAccess*>(pThis)->m_clrResult = pCharFormat->crTextColor;
    return pCharFormat->dwMask;
}

// Symbol: ?GetCharFormat@CFontDialog@@QEBAXAEAU_charformatw@@@Z
extern "C" void MS_ABI impl__GetCharFormat_CFontDialog__QEBAXAEAU_charformatw___Z(
    const CFontDialog* pThis, CHARFORMATW* pCharFormat) {
    if (!pCharFormat) return;
    const LOGFONTW* lf = FontLog(pThis);
    ZeroMemory(pCharFormat, sizeof(*pCharFormat));
    pCharFormat->cbSize = sizeof(*pCharFormat);
    if (!lf) return;

    pCharFormat->dwMask = CFM_FACE | CFM_SIZE | CFM_BOLD | CFM_ITALIC |
                          CFM_UNDERLINE | CFM_STRIKEOUT | CFM_COLOR;
    pCharFormat->dwEffects = 0;
    if (lf->lfWeight >= FW_BOLD) pCharFormat->dwEffects |= CFE_BOLD;
    if (lf->lfItalic) pCharFormat->dwEffects |= CFE_ITALIC;
    if (lf->lfUnderline) pCharFormat->dwEffects |= CFE_UNDERLINE;
    if (lf->lfStrikeOut) pCharFormat->dwEffects |= CFE_STRIKEOUT;
    pCharFormat->yHeight = (LONG)(static_cast<const CFontDialogAccess*>(pThis)->m_nPointSize * 20);
    pCharFormat->crTextColor = static_cast<const CFontDialogAccess*>(pThis)->m_clrResult;
    pCharFormat->bCharSet = lf->lfCharSet;
    pCharFormat->bPitchAndFamily = lf->lfPitchAndFamily;
    lstrcpynW(pCharFormat->szFaceName, lf->lfFaceName, LF_FACESIZE);
}

// Symbol: ?GetRuntimeClass@CFontDialog@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CFontDialog__UEBAPEAUCRuntimeClass__XZ(
    const CFontDialog* pThis) {
    return pThis ? pThis->GetRuntimeClass() : CFontDialog::GetThisClass();
}

// Symbol: ?GetThisClass@CFontDialog@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CFontDialog__SAPEAUCRuntimeClass__XZ() {
    return CFontDialog::GetThisClass();
}
