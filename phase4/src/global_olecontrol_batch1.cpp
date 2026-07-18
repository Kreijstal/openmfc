// COleControl methods that the corrected retail member layout makes
// implementable for real. Every function here was decoded from the retail
// mfc140u export (ordinal -> RVA -> objdump) rather than inferred, and the
// member offsets the disassembly uses are exactly the ones transcribed into
// include/openmfc/afxole.h (see the static_asserts in olecore.cpp).
//
// The retail bitfield word lives at this+0x160; bit positions referenced below
// are the ones the shipping code actually tests:
//   bit  2 = m_bModified, bit 10 = m_bInPlaceActive.
//
// Kept in a global_*.cpp file so the build's AUTO_EXCLUDES pass drops the
// generated stubs for these ordinals.

#include "openmfc/afxwin.h"
#include "openmfc/afxdisp.h"
#include "openmfc/afxole.h"

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

// COleControl::GetClientSite() — retail is a single load:
//     mov rax,[rcx+0x1d8] ; ret          (0x1d8 == 472 == m_pClientSite)
// Note it does NOT AddRef, matching the raw-pointer accessor contract.
// Symbol: ?GetClientSite@COleControl@@QEAAPEAUIOleClientSite@@XZ
extern "C" IOleClientSite* MS_ABI impl__GetClientSite_COleControl__QEAAPEAUIOleClientSite__XZ(
    COleControl* pThis)
{
    if (!pThis) return nullptr;
    return pThis->m_pClientSite;
}

// COleControl::IsModified() — retail:
//     mov eax,[rcx+0x160] ; shr eax,2 ; and eax,1 ; ret
// i.e. it returns the m_bModified bit directly, not a normalized TRUE/FALSE
// from any other state.
// Symbol: ?IsModified@COleControl@@QEAAHXZ
extern "C" int MS_ABI impl__IsModified_COleControl__QEAAHXZ(COleControl* pThis)
{
    if (!pThis) return FALSE;
    return static_cast<int>(pThis->m_bModified);
}

// COleControl::GetActivationPolicy() — retail is literally `xor eax,eax ; ret`.
// The base control declares no activation policy; derived controls override to
// return POINTERINACTIVE_* flags.
// Symbol: ?GetActivationPolicy@COleControl@@UEAAKXZ
extern "C" unsigned long MS_ABI impl__GetActivationPolicy_COleControl__UEAAKXZ(
    COleControl* /*pThis*/)
{
    return 0;
}

// COleControl::GetRectInContainer(RECT*) — retail decodes to:
//     if (m_bInPlaceActive)              // test dword [rcx+0x160],0x400
//         CopyRect(lpRect, &m_rcPos);    // lea rdx,[rcx+0x120]
//     return m_bInPlaceActive;           // shr eax,0xa ; and eax,1
// The rect is left untouched when the control is not in-place active, and the
// return value is the same bit that gated the copy.
// Symbol: ?GetRectInContainer@COleControl@@QEAAHPEAUtagRECT@@@Z
extern "C" int MS_ABI impl__GetRectInContainer_COleControl__QEAAHPEAUtagRECT___Z(
    COleControl* pThis, RECT* lpRect)
{
    if (!pThis) return FALSE;
    const int bInPlaceActive = static_cast<int>(pThis->m_bInPlaceActive);
    if (bInPlaceActive && lpRect) {
        ::CopyRect(lpRect, reinterpret_cast<const RECT*>(&pThis->m_rcPos));
    }
    return bInPlaceActive;
}

// COleControl::GetAmbientDispatchDriver() — retail lazily binds the driver to
// the container's IDispatch the first time it is asked:
//     lea rbx,[rcx+0x100]                 ; &m_ambientDispDriver
//     cmp qword ptr [rbx],0 ; jne done    ; already attached?
//     mov rcx,[rcx+0x1d8]                 ; m_pClientSite
//     ... QueryInterface(IID_IDispatch) and attach ...
// It always returns the address of the embedded driver, attached or not.
// Symbol: ?GetAmbientDispatchDriver@COleControl@@IEAAPEAVCOleDispatchDriver@@XZ
extern "C" COleDispatchDriver* MS_ABI
impl__GetAmbientDispatchDriver_COleControl__IEAAPEAVCOleDispatchDriver__XZ(COleControl* pThis)
{
    if (!pThis) return nullptr;
    COleDispatchDriver* pDriver = &pThis->m_ambientDispDriver;
    // m_lpDispatch is private; GetIDispatch(FALSE) reads the same slot without
    // taking a reference.
    if (pDriver->GetIDispatch(FALSE) == nullptr && pThis->m_pClientSite != nullptr) {
        IDispatch* pDisp = nullptr;
        if (SUCCEEDED(pThis->m_pClientSite->QueryInterface(IID_IDispatch,
                                                           reinterpret_cast<void**>(&pDisp)))
            && pDisp != nullptr) {
            // AttachDispatch takes ownership of the reference QueryInterface
            // handed back, matching retail (no extra AddRef at this site).
            pDriver->AttachDispatch(pDisp, TRUE);
        }
    }
    return pDriver;
}
