// Native dispatch-map lookup and standard property storage operations.
#include "detail/CCmdTargetDispatchSupport.h"
#include <cstring>
extern "C" void MS_ABI impl__AfxThrowOleException__YAXJ_Z(long);

// Symbol: ?GetDispEntry@CCmdTarget@@IEAAPEBUAFX_DISPMAP_ENTRY@@J@Z
extern "C" const void* MS_ABI impl__GetDispEntry_CCmdTarget__IEAAPEBUAFX_DISPMAP_ENTRY__J_Z(CCmdTarget* pThis, long memid) {
    if (!pThis) return nullptr;
    const AFX_DISPMAP* pMap = pThis->GetDispatchMap();
    if (!pMap) return nullptr;

    if (memid == 0) {
        const AFX_DISPMAP_ENTRY* pValue = nullptr;
        for (const AFX_DISPMAP* pM = pMap; pM != nullptr && pValue == nullptr;
             pM = pM->pfnGetBaseMap ? pM->pfnGetBaseMap() : nullptr) {
            for (const AFX_DISPMAP_ENTRY* e = pM->lpEntries; e->nPropOffset != (size_t)-1; ++e) {
                if (e->vt == 0xfff) { pValue = e; break; }   // VT_MFCVALUE
            }
        }
        if (pValue != nullptr) {
            memid = pValue->lDispID;
            if (memid == -1) {
                memid = impl__MemberIDFromName_CCmdTarget__KAJPEBUAFX_DISPMAP__PEB_W_Z(pMap, pValue->lpszName);
                if (memid == -1) return nullptr;
            }
        }
    }

    if (memid > 0) {
        const unsigned int nInherit = static_cast<unsigned int>(memid >> 16) & 0xffffu;
        const AFX_DISPMAP* pM = pMap;
        for (unsigned int i = 0; i < nInherit; ++i) {
            if (!pM->pfnGetBaseMap) break;
            pM = pM->pfnGetBaseMap();
            if (!pM) break;
        }
        if (pM != nullptr) {
            const unsigned int nIndex = static_cast<unsigned int>(memid) & 0xffffu;
            // The `nIndex != 0` half of this test is an OpenMFC addition: retail
            // computes lpEntries - 0x40 + nIndex * 0x40 unguarded and would read
            // one record before the table when LOWORD(memid) is 0.
            if (nIndex != 0 && nIndex <= impl__GetEntryCount_CCmdTarget__KAIPEBUAFX_DISPMAP___Z(pM)) {
                const AFX_DISPMAP_ENTRY* e = pM->lpEntries + (nIndex - 1);
                if (e->lDispID == -1 || e->lDispID == memid) return e;
            }
        }
    }

    for (const AFX_DISPMAP* pM = pMap; pM != nullptr;
         pM = pM->pfnGetBaseMap ? pM->pfnGetBaseMap() : nullptr) {
        for (const AFX_DISPMAP_ENTRY* e = pM->lpEntries; e->nPropOffset != (size_t)-1; ++e) {
            if (e->lDispID == memid) return e;
        }
    }
    return nullptr;
}

// Symbol: ?GetEntryCount@CCmdTarget@@KAIPEBUAFX_DISPMAP@@@Z
extern "C" unsigned int MS_ABI impl__GetEntryCount_CCmdTarget__KAIPEBUAFX_DISPMAP___Z(const void* pDispMap) {
    const AFX_DISPMAP* pMap = static_cast<const AFX_DISPMAP*>(pDispMap);
    if (!pMap || !pMap->lpEntryCount || !pMap->lpEntries) return 0;
    unsigned int* pCount = pMap->lpEntryCount;
    if (*pCount == 0xffffffffu) {
        const AFX_DISPMAP_ENTRY* e = pMap->lpEntries;
        while (e->nPropOffset != (size_t)-1) ++e;
        *pCount = static_cast<unsigned int>(e - pMap->lpEntries);
    }
    return *pCount;
}

// Symbol: ?MemberIDFromName@CCmdTarget@@KAJPEBUAFX_DISPMAP@@PEB_W@Z
extern "C" long MS_ABI impl__MemberIDFromName_CCmdTarget__KAJPEBUAFX_DISPMAP__PEB_W_Z(const void* pDispMap, const wchar_t* lpszName) {
    const AFX_DISPMAP* pMap = static_cast<const AFX_DISPMAP*>(pDispMap);
    if (!lpszName) return -1;
    for (unsigned int nInherit = 0; pMap != nullptr; ++nInherit) {
        const unsigned int nCount = impl__GetEntryCount_CCmdTarget__KAIPEBUAFX_DISPMAP___Z(pMap);
        for (unsigned int i = 0; i < nCount; ++i) {
            const AFX_DISPMAP_ENTRY* e = pMap->lpEntries + i;
            if (e->vt == 0xfff) continue;              // VT_MFCVALUE
            if (::CompareStringW(0x7f /*LOCALE_INVARIANT*/, NORM_IGNORECASE,
                                 e->lpszName, -1, lpszName, -1) != CSTR_EQUAL)
                continue;
            if (e->lDispID != -1) return e->lDispID;
            return static_cast<long>((static_cast<unsigned long>(nInherit & 0xffffu) << 16) |
                                     static_cast<unsigned short>(i + 1));
        }
        pMap = pMap->pfnGetBaseMap ? pMap->pfnGetBaseMap() : nullptr;
    }
    return -1;  // DISPID_UNKNOWN
}

// Symbol: ?SetStandardProp@CCmdTarget@@IEAAJPEBUAFX_DISPMAP_ENTRY@@PEAUtagDISPPARAMS@@PEAI@Z
extern "C" long MS_ABI impl__SetStandardProp_CCmdTarget__IEAAJPEBUAFX_DISPMAP_ENTRY__PEAUtagDISPPARAMS__PEAI_Z(CCmdTarget* pThis, const void* pEntryV, DISPPARAMS* pDispParams, unsigned int* puArgErr) {
    const AFX_DISPMAP_ENTRY* pEntry = static_cast<const AFX_DISPMAP_ENTRY*>(pEntryV);
    HRESULT hr = S_OK;
    VARIANT va;
    std::memset(&va, 0, sizeof(va));          // AfxVariantInit
    VARIANT* pArg = &pDispParams->rgvarg[0];
    if (pEntry->vt != VT_VARIANT && pArg->vt != pEntry->vt) {
        hr = ::VariantChangeType(&va, pArg, 0, pEntry->vt);
        if (FAILED(hr)) {
            *puArgErr = 0;
            return hr;
        }
        pArg = &va;
    }

    unsigned char* pProp = reinterpret_cast<unsigned char*>(pThis) + pEntry->nPropOffset;
    const unsigned char* pVal = reinterpret_cast<const unsigned char*>(pArg) + 8;
    switch (pEntry->vt) {
    case VT_I1: case VT_UI1:
        std::memcpy(pProp, pVal, 1); break;
    case VT_I2: case VT_UI2:
        std::memcpy(pProp, pVal, 2); break;
    case VT_I4: case VT_R4: case VT_ERROR: case VT_UI4:
        std::memcpy(pProp, pVal, 4); break;
    case VT_R8: case VT_CY: case VT_DATE: case VT_I8: case VT_UI8:
        std::memcpy(pProp, pVal, 8); break;
    case VT_BOOL:
        *reinterpret_cast<BOOL*>(pProp) = (pArg->boolVal != 0) ? TRUE : FALSE; break;
    case VT_BSTR: {
        auto& text=*reinterpret_cast<CString*>(pProp);
        UINT length=SysStringLen(pArg->bstrVal);
        wchar_t* buffer=text.GetBuffer(length);
        if (length) std::memcpy(buffer,pArg->bstrVal,length*sizeof(wchar_t));
        text.ReleaseBuffer(length);
        break;
    }
    case VT_DISPATCH:
    case VT_UNKNOWN: {
        IUnknown* pNew = pArg->punkVal;
        if (pNew != nullptr) pNew->AddRef();
        IUnknown*& rOld = *reinterpret_cast<IUnknown**>(pProp);
        if (rOld != nullptr) { rOld->Release(); rOld = nullptr; }
        rOld = pNew;
        break;
    }
    case VT_VARIANT:
        if (::VariantCopy(reinterpret_cast<VARIANT*>(pProp), pArg) != 0)
            *puArgErr = 0;
        break;
    default:
        hr = DISP_E_BADVARTYPE;
        *puArgErr = 0;
        break;
    }
    ::VariantClear(&va);

    if (SUCCEEDED(hr) && pEntry->pfnSet != nullptr) {
        openmfc::detail::CmdTargetModuleScope module(pThis->m_pModuleState);
        reinterpret_cast<void (MS_ABI*)(void*)>(pEntry->pfnSet)(pThis);
    }
    return hr;
}

// Symbol: ?GetStandardProp@CCmdTarget@@IEAAXPEBUAFX_DISPMAP_ENTRY@@PEAUtagVARIANT@@PEAI@Z
extern "C" void MS_ABI impl__GetStandardProp_CCmdTarget__IEAAXPEBUAFX_DISPMAP_ENTRY__PEAUtagVARIANT__PEAI_Z(CCmdTarget* pThis, const void* pEntry, void* pvarResult, unsigned int* puArgErr) {
    HRESULT hr=ReadDispatchProperty(pThis,static_cast<const AFX_DISPMAP_ENTRY*>(pEntry),static_cast<VARIANT*>(pvarResult));
    if (FAILED(hr)) {
        if (puArgErr) *puArgErr=0;
        impl__AfxThrowOleException__YAXJ_Z(hr);
    }
}

// Symbol: ?EnableAutomation@CCmdTarget@@QEAAXXZ
extern "C" void MS_ABI impl__EnableAutomation_CCmdTarget__QEAAXXZ(CCmdTarget* pThis) {
    if (pThis) pThis->m_xDispatch = reinterpret_cast<ULONG_PTR>(CommandTargetDispatchTable());
}

// Symbol: ?FromIDispatch@CCmdTarget@@SAPEAV1@PEAUIDispatch@@@Z
extern "C" void* MS_ABI impl__FromIDispatch_CCmdTarget__SAPEAV1_PEAUIDispatch___Z(void* pDispatch) {
    if (!pDispatch || *static_cast<void***>(pDispatch) != CommandTargetDispatchTable()) return nullptr;
    return static_cast<char*>(pDispatch) - offsetof(CCmdTarget,m_xDispatch);
}

// Symbol: ?GetIDispatch@CCmdTarget@@QEAAPEAUIDispatch@@H@Z
extern "C" void* MS_ABI impl__GetIDispatch_CCmdTarget__QEAAPEAUIDispatch__H_Z(CCmdTarget* pThis, int bAddRef) {
    if (!pThis) return nullptr;
    if (bAddRef) impl__ExternalAddRef_CCmdTarget__QEAAKXZ(pThis);
    return impl__GetInterface_CCmdTarget__QEAAPEAUIUnknown__PEBX_Z(pThis, &IID_IDispatch);
}

HRESULT ReadDispatchProperty(CCmdTarget* target,const AFX_DISPMAP_ENTRY* entry,VARIANT* result) {
    if (!target || !entry || !result) return E_POINTER;
    auto* storage=reinterpret_cast<unsigned char*>(target)+entry->nPropOffset;
    VariantInit(result);
    result->vt=entry->vt;
    switch (entry->vt) {
    case VT_I1: case VT_UI1: std::memcpy(&result->cVal,storage,1); break;
    case VT_I2: case VT_UI2: std::memcpy(&result->iVal,storage,2); break;
    case VT_I4: case VT_UI4: case VT_R4: case VT_ERROR: std::memcpy(&result->lVal,storage,4); break;
    case VT_I8: case VT_UI8: case VT_R8: case VT_CY: case VT_DATE: std::memcpy(&result->llVal,storage,8); break;
    case VT_BOOL: result->boolVal=*reinterpret_cast<BOOL*>(storage) ? VARIANT_TRUE : VARIANT_FALSE; break;
    case VT_BSTR: {
        auto& text=*reinterpret_cast<CString*>(storage);
        result->bstrVal=SysAllocStringLen(text.GetString(),text.GetLength());
        if (!result->bstrVal) return E_OUTOFMEMORY;
        break;
    }
    case VT_UNKNOWN: case VT_DISPATCH:
        result->punkVal=*reinterpret_cast<IUnknown**>(storage);
        if (result->punkVal) result->punkVal->AddRef();
        break;
    case VT_VARIANT:
        result->vt=VT_EMPTY;
        return VariantCopy(result,reinterpret_cast<VARIANT*>(storage));
    default: result->vt=VT_EMPTY; return DISP_E_BADVARTYPE;
    }
    return S_OK;
}
