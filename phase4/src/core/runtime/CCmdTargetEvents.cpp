// Native control event-map lookup and handler invocation.
#include "detail/CCmdTargetEventsSupport.h"
#include "detail/OleExceptionTranslationSupport.h"
#include <cstring>
#include <vector>
extern "C" void MS_ABI impl__AfxThrowInvalidArgException__YAXXZ();
namespace {
struct S_CmdHandlerInfo { CCmdTarget* pTarget; void* pmf; };
struct EventResult {
    VARIANT value{};
    ~EventResult() { VariantClear(&value); }
};
}

// CCmdTarget::GetEventSinkEntry(UINT idCtrl, AFX_EVENT* pEvent)
// -- retail mfc140u 0x2371c0, transcribed:
//     if (pEvent == NULL) AfxThrowInvalidArgException();
//     pMap = this->vtable[+0x88]()                  ; virtual GetEventSinkMap
//     bProp = (pEvent->m_eventKind != 0) ? 1 : 0    ; setne on [pEvent+0x00]
//     for each map in the chain:
//         for (e = pMap->lpEntries; e->dispEntry.nPropOffset != (size_t)-1; ++e) {
//             if (e->dispEntry.lDispID != pEvent->m_dispid) continue;   ; [pEvent+0x04]
//             if (e->dispEntry.nPropOffset != (size_t)bProp) continue;
//             if (e->nCtrlIDLast == (UINT)-1) {
//                 if (e->nCtrlIDFirst == (UINT)-1 || e->nCtrlIDFirst == idCtrl) return e;
//             } else if (e->nCtrlIDFirst <= idCtrl && idCtrl <= e->nCtrlIDLast) {
//                 return e;
//             }
//         }
//     return NULL;
// Symbol: ?GetEventSinkEntry@CCmdTarget@@IEAAPEBUAFX_EVENTSINKMAP_ENTRY@@IPEAUAFX_EVENT@@@Z
extern "C" const void* MS_ABI impl__GetEventSinkEntry_CCmdTarget__IEAAPEBUAFX_EVENTSINKMAP_ENTRY__IPEAUAFX_EVENT___Z(CCmdTarget* pThis, unsigned int idCtrl, void* pEventV) {
    if (!pThis) return nullptr;
    if (!pEventV) { impl__AfxThrowInvalidArgException__YAXXZ(); return nullptr; }
    const AFX_EVENT* pEvent = static_cast<const AFX_EVENT*>(pEventV);
    const size_t bProp = (pEvent->m_eventKind != 0) ? 1u : 0u;

    for (const AFX_EVENTSINKMAP* pMap = pThis->GetEventSinkMap();
         pMap != nullptr;
         pMap = pMap->pfnGetBaseMap ? pMap->pfnGetBaseMap() : nullptr) {
        for (const AFX_EVENTSINKMAP_ENTRY* e = pMap->lpEntries;
             e->dispEntry.nPropOffset != (size_t)-1; ++e) {
            if (e->dispEntry.lDispID != pEvent->m_dispid) continue;
            if (e->dispEntry.nPropOffset != bProp) continue;
            if (e->nCtrlIDLast == 0xffffffffu) {
                if (e->nCtrlIDFirst == 0xffffffffu || e->nCtrlIDFirst == idCtrl) return e;
            } else if (e->nCtrlIDFirst <= idCtrl && idCtrl <= e->nCtrlIDLast) {
                return e;
            }
        }
    }
    return nullptr;
}

// CCmdTarget::OnEvent(UINT idCtrl, AFX_EVENT* pEvent,
//                     AFX_CMDHANDLERINFO* pHandlerInfo)
// -- retail mfc140u 0x236e70 (reached by export ordinal), transcribed:
//   pEntry = GetEventSinkEntry(idCtrl, pEvent);          ; call 0x2371c0
//   if (pEntry == NULL) return FALSE;
//   if (pHandlerInfo != NULL) {                          ; query only
//       pHandlerInfo->pTarget = this;
//       switch (pEvent->m_eventKind) {
//       case event: case propRequest: pHandlerInfo->pmf = pEntry->dispEntry.pfn;    break; ; +0x20
//       case propChanged:             pHandlerInfo->pmf = pEntry->dispEntry.pfnSet; break; ; +0x28
//       }                                                ; other kinds leave pmf alone
//       return pHandlerInfo->pmf != NULL;
//   }
//   bRange = (pEntry->nCtrlIDLast != (UINT)-1);         ; +0x44
//   hResult = S_OK; bHandled = FALSE; uArgError = (UINT)-1;
//   switch (pEvent->m_eventKind) {
//   case event:        VARIANT var = {0};  (AfxVariantInit, memset 0x18)
//                      with bRange, copy *m_pDispParams, grow rgvarg by one
//                      VARIANT (??2, then memcpy of the old ones) and put
//                      VT_I4 idCtrl in the new last slot;
//                      hResult = CallMemberFunc(&pEntry->dispEntry, DISPATCH_METHOD,
//                                               &var, dispparams, &uArgError);  ; 0x2501b0
//                      bHandled = (short)V_BOOL(&var);   ; movswl
//                      with bRange, free the grown rgvarg
//   case propRequest:  bAllow = TRUE; bHandled = pfn([idCtrl,] &bAllow);
//                      hResult = bAllow ? S_OK : S_FALSE;
//   case propChanged:  bHandled = pfnSet([idCtrl]);
//   case propDSCNotify:bAllow = TRUE;
//                      bHandled = pfn([idCtrl,] m_nDSCState, m_nDSCReason, &bAllow); ; +0x28/+0x2c
//                      hResult = bAllow ? S_OK : S_FALSE;
//   }                                                    ; any other kind: FALSE, S_OK
//   if (FAILED(hResult) && pEvent->m_puArgError && uArgError != (UINT)-1)
//       *pEvent->m_puArgError = uArgError;               ; `test %r14d ; jns` skips it
//   pEvent->m_hResult = hResult;                         ; +0x24
//   return bHandled;
// where "[idCtrl,]" is passed only when bRange.  A NULL pEvent never gets
// here: GetEventSinkEntry throws AfxThrowInvalidArgException for it.
// DEVIATIONS:
//   * a NULL `this` returns FALSE.
//   * ranged arguments and the returned VARIANT have scoped storage, including
//     during exception unwinding. The arguments are shallow copies: ownership
//     of incoming BSTRs and interface references remains with the caller.
// Symbol: ?OnEvent@CCmdTarget@@QEAAHIPEAUAFX_EVENT@@PEAUAFX_CMDHANDLERINFO@@@Z
extern "C" int MS_ABI impl__OnEvent_CCmdTarget__QEAAHIPEAUAFX_EVENT__PEAUAFX_CMDHANDLERINFO___Z(CCmdTarget* pThis, unsigned int idCtrl, void* pEventV, void* pHandlerInfoV) {
    if (!pThis) return FALSE;
    const AFX_EVENTSINKMAP_ENTRY* pEntry = static_cast<const AFX_EVENTSINKMAP_ENTRY*>(
        impl__GetEventSinkEntry_CCmdTarget__IEAAPEBUAFX_EVENTSINKMAP_ENTRY__IPEAUAFX_EVENT___Z(pThis, idCtrl, pEventV));
    if (pEntry == nullptr) return FALSE;
    AFX_EVENT* pEvent = static_cast<AFX_EVENT*>(pEventV);

    if (pHandlerInfoV != nullptr) {
        S_CmdHandlerInfo* pInfo = static_cast<S_CmdHandlerInfo*>(pHandlerInfoV);
        pInfo->pTarget = pThis;
        switch (pEvent->m_eventKind) {
        case 0:     // AFX_EVENT::event
        case 1:     // AFX_EVENT::propRequest
            pInfo->pmf = pEntry->dispEntry.pfn;
            break;
        case 2:     // AFX_EVENT::propChanged
            pInfo->pmf = pEntry->dispEntry.pfnSet;
            break;
        default:
            break;
        }
        return pInfo->pmf != nullptr ? TRUE : FALSE;
    }

    const bool bRange = (pEntry->nCtrlIDLast != 0xffffffffu);
    long hResult = 0;                         // S_OK
    int bHandled = FALSE;
    unsigned int uArgError = 0xffffffffu;
    int bAllow = TRUE;
    void* const pfn = pEntry->dispEntry.pfn;

    try {
    switch (pEvent->m_eventKind) {
    case 0: {   // AFX_EVENT::event
        EventResult result;
        VARIANT& var = result.value;
        DISPPARAMS dispparams{};
        std::vector<VARIANT> rangedArguments;
        DISPPARAMS* pDP = static_cast<DISPPARAMS*>(pEvent->m_pDispParams);
        if (bRange) {
            if (pDP == nullptr) { impl__AfxThrowInvalidArgException__YAXXZ(); return FALSE; }
            std::memcpy(&dispparams, pDP, sizeof(DISPPARAMS));
            if (dispparams.cArgs == UINT_MAX) {
                impl__AfxThrowInvalidArgException__YAXXZ(); return FALSE;
            }
            const unsigned int cArgs = ++dispparams.cArgs;
            rangedArguments.resize(cArgs);
            VARIANT* rgvarg = rangedArguments.data();
            dispparams.rgvarg = rgvarg;
            const std::size_t cbOld = static_cast<std::size_t>(cArgs - 1) * sizeof(VARIANT);
            if (cbOld != 0) {
                if (pDP->rgvarg == nullptr) {  // Checked::memcpy_s: EINVAL -> throw
                    impl__AfxThrowInvalidArgException__YAXXZ();
                    return FALSE;
                }
                std::memcpy(rgvarg, pDP->rgvarg, cbOld);
            }
            VARIANT* pvarID = &rgvarg[cArgs - 1];
            pvarID->vt = VT_I4;
            pvarID->lVal = static_cast<LONG>(idCtrl);
            pDP = &dispparams;
        }
        hResult = impl__CallMemberFunc_CCmdTarget__IEAAJPEBUAFX_DISPMAP_ENTRY__GPEAUtagVARIANT__PEAUtagDISPPARAMS__PEAI_Z(
            pThis, &pEntry->dispEntry, DISPATCH_METHOD, &var, pDP, &uArgError);
        bHandled = static_cast<short>(var.boolVal);
        break;
    }
    case 1:     // AFX_EVENT::propRequest
        if (bRange)
            bHandled = reinterpret_cast<int (MS_ABI*)(void*, unsigned int, int*)>(pfn)(pThis, idCtrl, &bAllow);
        else
            bHandled = reinterpret_cast<int (MS_ABI*)(void*, int*)>(pfn)(pThis, &bAllow);
        hResult = bAllow ? 0 : 1;             // S_OK : S_FALSE
        break;
    case 2: {   // AFX_EVENT::propChanged
        void* const pfnSet = pEntry->dispEntry.pfnSet;
        if (bRange)
            bHandled = reinterpret_cast<int (MS_ABI*)(void*, unsigned int)>(pfnSet)(pThis, idCtrl);
        else
            bHandled = reinterpret_cast<int (MS_ABI*)(void*)>(pfnSet)(pThis);
        break;
    }
    case 3:     // AFX_EVENT::propDSCNotify
        if (bRange)
            bHandled = reinterpret_cast<int (MS_ABI*)(void*, unsigned int, int, int, int*)>(pfn)(
                pThis, idCtrl, pEvent->m_nDSCState, pEvent->m_nDSCReason, &bAllow);
        else
            bHandled = reinterpret_cast<int (MS_ABI*)(void*, int, int, int*)>(pfn)(
                pThis, pEvent->m_nDSCState, pEvent->m_nDSCReason, &bAllow);
        hResult = bAllow ? 0 : 1;             // S_OK : S_FALSE
        break;
    default:
        break;
    }
    } catch(CException* error) {
        openmfc::detail::FrameworkExceptionCleanup cleanup{error};
        HRESULT translated=FillOleDispatchExceptionInfo(static_cast<EXCEPINFO*>(pEvent->m_pExcepInfo),error);
        hResult=FAILED(translated) ? translated : DISP_E_EXCEPTION;
    }

    if (hResult < 0 && pEvent->m_puArgError != nullptr && uArgError != 0xffffffffu)
        *pEvent->m_puArgError = uArgError;
    pEvent->m_hResult = hResult;
    return bHandled;
}

