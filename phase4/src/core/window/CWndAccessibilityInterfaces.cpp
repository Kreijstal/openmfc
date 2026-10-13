#include "detail/CWndAccessibilityProtocol.h"
#include <cstddef>

// ---------------------------------------------------------------------------
// CWnd::XAccessible / CWnd::XAccessibleServer -- the BEGIN_INTERFACE_PART
// sub-objects embedded in CWnd.
//
// The retail CWnd(HWND) constructor (mfc140u RVA 0x28a730) installs the
// XAccessible vftable (RVA 0x337098) at CWnd+0x60 and the XAccessibleServer
// vftable (RVA 0x337180) at CWnd+0x68.  Every method below therefore receives
// `this` == &m_xAccessible (or &m_xAccessibleServer) and starts with
// METHOD_PROLOGUE: `lea -0x60(%rcx)` / `lea -0x68(%rcx)` recovers the owning
// CWnd, and the owner's m_pModuleState (CCmdTarget+0x38) is handed to the
// AFX_MAINTAIN_STATE2 constructor (0x133170), whose inlined destructor restores
// the previous module state before returning.  The module-state juggling has no
// OpenMFC equivalent (CCmdTarget carries no m_pModuleState in this build) and
// is not reproduced; the displacement arithmetic is. The interface pointers
// have named slots at +0x60/+0x68, with their interface vtables installed.
//
// Retail dispatches virtually to the owner. These entry points currently call
// the corresponding default CWnd exports directly; client accessibility
// overrides still need the remaining CWnd virtual layout.
// ---------------------------------------------------------------------------
namespace {

constexpr ptrdiff_t kCWndXAccessibleOffset = 0x60;
constexpr ptrdiff_t kCWndXAccessibleServerOffset = 0x68;

inline CWnd* OuterFromXAccessible(void* pIface) {
    return reinterpret_cast<CWnd*>(static_cast<char*>(pIface) - kCWndXAccessibleOffset);
}
inline CWnd* OuterFromXAccessibleServer(void* pIface) {
    return reinterpret_cast<CWnd*>(static_cast<char*>(pIface) - kCWndXAccessibleServerOffset);
}

}  // namespace

// --- IDispatch part of XAccessible ------------------------------------------

// Retail mfc140u RVA 0x28f860: `if (pctinfo == NULL) return E_POINTER;
// *pctinfo = 1; return S_OK;` -- no METHOD_PROLOGUE.
// Symbol: ?GetTypeInfoCount@XAccessible@CWnd@@UEAAJPEAI@Z
extern "C" long MS_ABI impl__GetTypeInfoCount_XAccessible_CWnd__UEAAJPEAI_Z(void* pThis, unsigned int* pctinfo) {
    (void)pThis;
    if (pctinfo == nullptr) {
        return E_POINTER;   // 0x80004003
    }
    *pctinfo = 1;
    return S_OK;
}

// The mfc140u XAccessible vftable (RVA 0x337098) slot 6 is 0xf4d0, a body
// identical-code-folded with CArchiveStream::Clone: `mov $0x80004001,%eax;
// ret`.  The export itself has no RVA in either symbol map; the vftable is the
// evidence.  Arguments are never read.
// Symbol: ?GetTypeInfo@XAccessible@CWnd@@UEAAJIKPEAPEAUITypeInfo@@@Z
extern "C" long MS_ABI impl__GetTypeInfo_XAccessible_CWnd__UEAAJIKPEAPEAUITypeInfo___Z(void* pThis, unsigned int iTInfo, unsigned long lcid, void** ppTInfo) {
    (void)pThis; (void)iTInfo; (void)lcid; (void)ppTInfo;
    return E_NOTIMPL;   // 0x80004001
}

namespace {

// Name / DISPID tables read out of mfc140u .rdata at RVA 0x34f5d0 (19
// LPCWSTR) and 0x34f670 (19 DISPID) -- the tables GetIDsOfNames walks.
const wchar_t* const kAccessibleNames[19] = {
    L"accParent", L"accChildCount", L"accChild", L"accName", L"accValue",
    L"accDescription", L"accRole", L"accState", L"accHelp", L"accHelpTopic",
    L"accKeyboardShortcut", L"accFocus", L"accSelection", L"accDefaultAction",
    L"accSelect", L"accLocation", L"accNavigate", L"accHitTest", L"accDoDefaultAction",
};
const long kAccessibleDispIds[19] = {
    -5000, -5001, -5002, -5003, -5004, -5005, -5006, -5007, -5008, -5009,
    -5010, -5011, -5012, -5013, -5014, -5015, -5016, -5017, -5018,
};

}  // namespace

// Retail mfc140u RVA 0x28f7d0.  No METHOD_PROLOGUE; riid and lcid are never
// read and no pointer is validated.  For each name i < cNames the whole
// 19-entry table is scanned with lstrcmpW (import slot 0x1802c66a8, case
// sensitive); every match stores its DISPID into rgDispId[i] (the scan does not
// stop at the first hit), and a name with no match returns DISP_E_UNKNOWNNAME
// (0x80020006) at once.  cNames == 0 returns S_OK.
// Symbol: ?GetIDsOfNames@XAccessible@CWnd@@UEAAJAEBU_GUID@@PEAPEA_WIKPEAJ@Z
extern "C" long MS_ABI impl__GetIDsOfNames_XAccessible_CWnd__UEAAJAEBU_GUID__PEAPEA_WIKPEAJ_Z(void* pThis, const GUID* riid, wchar_t** rgszNames, unsigned int cNames, unsigned long lcid, long* rgDispId) {
    (void)pThis; (void)riid; (void)lcid;
    for (unsigned int i = 0; i < cNames; ++i) {
        bool bFound = false;
        for (int j = 0; j < 19; ++j) {
            if (::lstrcmpW(rgszNames[i], kAccessibleNames[j]) == 0) {
                rgDispId[i] = kAccessibleDispIds[j];
                bFound = true;
            }
        }
        if (!bFound) {
            return 0x80020006;   // DISP_E_UNKNOWNNAME
        }
    }
    return S_OK;
}

// --- IAccessible part of XAccessible ----------------------------------------
// Each body is METHOD_PROLOGUE + one virtual call on the owning CWnd; a VARIANT
// argument is re-copied onto the stack and passed by address (MS x64 passes a
// 24-byte VARIANT by reference to a caller-owned copy), which is what the
// `void* p0` parameters of the CWnd exports above receive.

// Retail mfc140u RVA 0x28f880: pThis->get_accParent(ppdispParent), slot 40 (+0x140).
// Symbol: ?get_accParent@XAccessible@CWnd@@UEAAJPEAPEAUIDispatch@@@Z
extern "C" long MS_ABI impl__get_accParent_XAccessible_CWnd__UEAAJPEAPEAUIDispatch___Z(void* pThis, void** ppdispParent) {
    CWnd* pWnd = OuterFromXAccessible(pThis);
    return impl__get_accParent_CWnd__UEAAJPEAPEAUIDispatch___Z(pWnd, ppdispParent);
}

// Retail mfc140u RVA 0x28f8e0: pThis->get_accChildCount(pcountChildren), slot 41 (+0x148).
// Symbol: ?get_accChildCount@XAccessible@CWnd@@UEAAJPEAJ@Z
extern "C" long MS_ABI impl__get_accChildCount_XAccessible_CWnd__UEAAJPEAJ_Z(void* pThis, long* pcountChildren) {
    CWnd* pWnd = OuterFromXAccessible(pThis);
    return impl__get_accChildCount_CWnd__UEAAJPEAJ_Z(pWnd, pcountChildren);
}

// Retail mfc140u RVA 0x28f940: pThis->get_accChild(varChild, ppdispChild), slot 42 (+0x150).
// Symbol: ?get_accChild@XAccessible@CWnd@@UEAAJUtagVARIANT@@PEAPEAUIDispatch@@@Z
extern "C" long MS_ABI impl__get_accChild_XAccessible_CWnd__UEAAJUtagVARIANT__PEAPEAUIDispatch___Z(void* pThis, VARIANT varChild, void** ppdispChild) {
    CWnd* pWnd = OuterFromXAccessible(pThis);
    VARIANT varCopy = varChild;
    return impl__get_accChild_CWnd__UEAAJUtagVARIANT__PEAPEAUIDispatch___Z(pWnd, &varCopy, ppdispChild);
}

// Retail mfc140u RVA 0x28f9c0: pThis->get_accName(varChild, pszName), slot 43 (+0x158).
// Symbol: ?get_accName@XAccessible@CWnd@@UEAAJUtagVARIANT@@PEAPEA_W@Z
extern "C" long MS_ABI impl__get_accName_XAccessible_CWnd__UEAAJUtagVARIANT__PEAPEA_W_Z(void* pThis, VARIANT varChild, wchar_t** pszName) {
    CWnd* pWnd = OuterFromXAccessible(pThis);
    VARIANT varCopy = varChild;
    return impl__get_accName_CWnd__UEAAJUtagVARIANT__PEAPEA_W_Z(pWnd, &varCopy, pszName);
}

// Retail mfc140u RVA 0x28fa40: pThis->get_accValue(varChild, pszValue), slot 44 (+0x160).
// Symbol: ?get_accValue@XAccessible@CWnd@@UEAAJUtagVARIANT@@PEAPEA_W@Z
extern "C" long MS_ABI impl__get_accValue_XAccessible_CWnd__UEAAJUtagVARIANT__PEAPEA_W_Z(void* pThis, VARIANT varChild, wchar_t** pszValue) {
    CWnd* pWnd = OuterFromXAccessible(pThis);
    VARIANT varCopy = varChild;
    return impl__get_accValue_CWnd__UEAAJUtagVARIANT__PEAPEA_W_Z(pWnd, &varCopy, pszValue);
}

// Retail mfc140u RVA 0x28fac0: pThis->get_accDescription(varChild, pszDescription), slot 45 (+0x168).
// Symbol: ?get_accDescription@XAccessible@CWnd@@UEAAJUtagVARIANT@@PEAPEA_W@Z
extern "C" long MS_ABI impl__get_accDescription_XAccessible_CWnd__UEAAJUtagVARIANT__PEAPEA_W_Z(void* pThis, VARIANT varChild, wchar_t** pszDescription) {
    CWnd* pWnd = OuterFromXAccessible(pThis);
    VARIANT varCopy = varChild;
    return impl__get_accDescription_CWnd__UEAAJUtagVARIANT__PEAPEA_W_Z(pWnd, &varCopy, pszDescription);
}

// Retail mfc140u RVA 0x28fb40: pThis->get_accRole(varChild, pvarRole), slot 46 (+0x170).
// Symbol: ?get_accRole@XAccessible@CWnd@@UEAAJUtagVARIANT@@PEAU3@@Z
extern "C" long MS_ABI impl__get_accRole_XAccessible_CWnd__UEAAJUtagVARIANT__PEAU3__Z(void* pThis, VARIANT varChild, VARIANT* pvarRole) {
    CWnd* pWnd = OuterFromXAccessible(pThis);
    VARIANT varCopy = varChild;
    return impl__get_accRole_CWnd__UEAAJUtagVARIANT__PEAU2__Z(pWnd, &varCopy, pvarRole);
}

// Retail mfc140u RVA 0x28fbc0: pThis->get_accState(varChild, pvarState), slot 47 (+0x178).
// Symbol: ?get_accState@XAccessible@CWnd@@UEAAJUtagVARIANT@@PEAU3@@Z
extern "C" long MS_ABI impl__get_accState_XAccessible_CWnd__UEAAJUtagVARIANT__PEAU3__Z(void* pThis, VARIANT varChild, VARIANT* pvarState) {
    CWnd* pWnd = OuterFromXAccessible(pThis);
    VARIANT varCopy = varChild;
    return impl__get_accState_CWnd__UEAAJUtagVARIANT__PEAU2__Z(pWnd, &varCopy, pvarState);
}

// Retail mfc140u RVA 0x28fc40: pThis->get_accHelp(varChild, pszHelp), slot 48 (+0x180).
// Symbol: ?get_accHelp@XAccessible@CWnd@@UEAAJUtagVARIANT@@PEAPEA_W@Z
extern "C" long MS_ABI impl__get_accHelp_XAccessible_CWnd__UEAAJUtagVARIANT__PEAPEA_W_Z(void* pThis, VARIANT varChild, wchar_t** pszHelp) {
    CWnd* pWnd = OuterFromXAccessible(pThis);
    VARIANT varCopy = varChild;
    return impl__get_accHelp_CWnd__UEAAJUtagVARIANT__PEAPEA_W_Z(pWnd, &varCopy, pszHelp);
}

// Retail mfc140u RVA 0x28fcc0: pThis->get_accHelpTopic(pszHelpFile, varChild, pidTopic), slot 49 (+0x188).
// Symbol: ?get_accHelpTopic@XAccessible@CWnd@@UEAAJPEAPEA_WUtagVARIANT@@PEAJ@Z
extern "C" long MS_ABI impl__get_accHelpTopic_XAccessible_CWnd__UEAAJPEAPEA_WUtagVARIANT__PEAJ_Z(void* pThis, wchar_t** pszHelpFile, VARIANT varChild, long* pidTopic) {
    CWnd* pWnd = OuterFromXAccessible(pThis);
    VARIANT varCopy = varChild;
    return impl__get_accHelpTopic_CWnd__UEAAJPEAPEA_WUtagVARIANT__PEAJ_Z(pWnd, pszHelpFile, &varCopy, pidTopic);
}

// Retail mfc140u RVA 0x28fd40: pThis->get_accKeyboardShortcut(varChild, pszKeyboardShortcut), slot 50 (+0x190).
// Symbol: ?get_accKeyboardShortcut@XAccessible@CWnd@@UEAAJUtagVARIANT@@PEAPEA_W@Z
extern "C" long MS_ABI impl__get_accKeyboardShortcut_XAccessible_CWnd__UEAAJUtagVARIANT__PEAPEA_W_Z(void* pThis, VARIANT varChild, wchar_t** pszKeyboardShortcut) {
    CWnd* pWnd = OuterFromXAccessible(pThis);
    VARIANT varCopy = varChild;
    return impl__get_accKeyboardShortcut_CWnd__UEAAJUtagVARIANT__PEAPEA_W_Z(pWnd, &varCopy, pszKeyboardShortcut);
}

// Retail mfc140u RVA 0x28fdc0: pThis->get_accFocus(pvarChild), slot 51 (+0x198).
// Symbol: ?get_accFocus@XAccessible@CWnd@@UEAAJPEAUtagVARIANT@@@Z
extern "C" long MS_ABI impl__get_accFocus_XAccessible_CWnd__UEAAJPEAUtagVARIANT___Z(void* pThis, VARIANT* pvarChild) {
    CWnd* pWnd = OuterFromXAccessible(pThis);
    return impl__get_accFocus_CWnd__UEAAJPEAUtagVARIANT___Z(pWnd, pvarChild);
}

// Retail mfc140u RVA 0x28fe20: pThis->get_accSelection(pvarChildren), slot 52 (+0x1a0).
// Symbol: ?get_accSelection@XAccessible@CWnd@@UEAAJPEAUtagVARIANT@@@Z
extern "C" long MS_ABI impl__get_accSelection_XAccessible_CWnd__UEAAJPEAUtagVARIANT___Z(void* pThis, VARIANT* pvarChildren) {
    CWnd* pWnd = OuterFromXAccessible(pThis);
    return impl__get_accSelection_CWnd__UEAAJPEAUtagVARIANT___Z(pWnd, pvarChildren);
}

// Retail mfc140u RVA 0x28fe80: pThis->get_accDefaultAction(varChild, pszDefaultAction), slot 53 (+0x1a8).
// Symbol: ?get_accDefaultAction@XAccessible@CWnd@@UEAAJUtagVARIANT@@PEAPEA_W@Z
extern "C" long MS_ABI impl__get_accDefaultAction_XAccessible_CWnd__UEAAJUtagVARIANT__PEAPEA_W_Z(void* pThis, VARIANT varChild, wchar_t** pszDefaultAction) {
    CWnd* pWnd = OuterFromXAccessible(pThis);
    VARIANT varCopy = varChild;
    return impl__get_accDefaultAction_CWnd__UEAAJUtagVARIANT__PEAPEA_W_Z(pWnd, &varCopy, pszDefaultAction);
}

// Retail mfc140u RVA 0x28ff00: pThis->accSelect(flagsSelect, varChild), slot 54 (+0x1b0).
// Symbol: ?accSelect@XAccessible@CWnd@@UEAAJJUtagVARIANT@@@Z
extern "C" long MS_ABI impl__accSelect_XAccessible_CWnd__UEAAJJUtagVARIANT___Z(void* pThis, long flagsSelect, VARIANT varChild) {
    CWnd* pWnd = OuterFromXAccessible(pThis);
    VARIANT varCopy = varChild;
    return impl__accSelect_CWnd__UEAAJJUtagVARIANT___Z(pWnd, flagsSelect, &varCopy);
}

// Retail mfc140u RVA 0x28ff80: pThis->accLocation(pxLeft, pyTop, pcxWidth, pcyHeight, varChild), slot 55 (+0x1b8).
// Symbol: ?accLocation@XAccessible@CWnd@@UEAAJPEAJ000UtagVARIANT@@@Z
extern "C" long MS_ABI impl__accLocation_XAccessible_CWnd__UEAAJPEAJ000UtagVARIANT___Z(void* pThis, long* pxLeft, long* pyTop, long* pcxWidth, long* pcyHeight, VARIANT varChild) {
    CWnd* pWnd = OuterFromXAccessible(pThis);
    VARIANT varCopy = varChild;
    return impl__accLocation_CWnd__UEAAJPEAJ000UtagVARIANT___Z(pWnd, pxLeft, pyTop, pcxWidth, pcyHeight, &varCopy);
}

// Retail mfc140u RVA 0x290010: pThis->accNavigate(navDir, varStart, pvarEndUpAt), slot 56 (+0x1c0).
// Symbol: ?accNavigate@XAccessible@CWnd@@UEAAJJUtagVARIANT@@PEAU3@@Z
extern "C" long MS_ABI impl__accNavigate_XAccessible_CWnd__UEAAJJUtagVARIANT__PEAU3__Z(void* pThis, long navDir, VARIANT varStart, VARIANT* pvarEndUpAt) {
    CWnd* pWnd = OuterFromXAccessible(pThis);
    VARIANT varCopy = varStart;
    return impl__accNavigate_CWnd__UEAAJJUtagVARIANT__PEAU2__Z(pWnd, navDir, &varCopy, pvarEndUpAt);
}

// Retail mfc140u RVA 0x290090: pThis->accHitTest(xLeft, yTop, pvarChild), slot 57 (+0x1c8).
// Symbol: ?accHitTest@XAccessible@CWnd@@UEAAJJJPEAUtagVARIANT@@@Z
extern "C" long MS_ABI impl__accHitTest_XAccessible_CWnd__UEAAJJJPEAUtagVARIANT___Z(void* pThis, long xLeft, long yTop, VARIANT* pvarChild) {
    CWnd* pWnd = OuterFromXAccessible(pThis);
    return impl__accHitTest_CWnd__UEAAJJJPEAUtagVARIANT___Z(pWnd, xLeft, yTop, pvarChild);
}

// Retail mfc140u RVA 0x2900f0: pThis->accDoDefaultAction(varChild), slot 58 (+0x1d0).
// Symbol: ?accDoDefaultAction@XAccessible@CWnd@@UEAAJUtagVARIANT@@@Z
extern "C" long MS_ABI impl__accDoDefaultAction_XAccessible_CWnd__UEAAJUtagVARIANT___Z(void* pThis, VARIANT varChild) {
    CWnd* pWnd = OuterFromXAccessible(pThis);
    VARIANT varCopy = varChild;
    return impl__accDoDefaultAction_CWnd__UEAAJUtagVARIANT___Z(pWnd, &varCopy);
}

// Retail mfc140u RVA 0x290160 (XAccessible vftable slot 26; unnamed in the u
// symbol map): pThis->put_accName(varChild, szName), CWnd slot 59 (+0x1d8).
// Symbol: ?put_accName@XAccessible@CWnd@@UEAAJUtagVARIANT@@PEA_W@Z
extern "C" long MS_ABI impl__put_accName_XAccessible_CWnd__UEAAJUtagVARIANT__PEA_W_Z(void* pThis, VARIANT varChild, wchar_t* szName) {
    CWnd* pWnd = OuterFromXAccessible(pThis);
    VARIANT varCopy = varChild;
    return impl__put_accName_CWnd__UEAAJUtagVARIANT__PEA_W_Z(pWnd, &varCopy, szName);
}

// Retail mfc140u RVA 0x2901e0 (XAccessible vftable slot 27; unnamed in the u
// symbol map): pThis->put_accValue(varChild, szValue), CWnd slot 60 (+0x1e0).
// Symbol: ?put_accValue@XAccessible@CWnd@@UEAAJUtagVARIANT@@PEA_W@Z
extern "C" long MS_ABI impl__put_accValue_XAccessible_CWnd__UEAAJUtagVARIANT__PEA_W_Z(void* pThis, VARIANT varChild, wchar_t* szValue) {
    CWnd* pWnd = OuterFromXAccessible(pThis);
    VARIANT varCopy = varChild;
    return impl__put_accValue_CWnd__UEAAJUtagVARIANT__PEA_W_Z(pWnd, &varCopy, szValue);
}

// --- IDispatch::Invoke of XAccessible ---------------------------------------

namespace {

// The unnamed helper at mfc140u RVA 0x289f58 that XAccessible::Invoke tail-
// calls with `this` == the XAccessible sub-object: an IDispatch::Invoke over
// the IAccessible methods.  Retail dispatches every method through the
// sub-object's IAccessible vftable (slot numbers noted per case); for a CWnd
// that vftable (RVA 0x337098) holds exactly the XAccessible exports above, and
// the nested class is never subclassed, so this build calls them directly (the
// sub-object vtable entries are fixed by InitializeAccessibility).  Transcribed control flow:
//
//   if (pAcc == NULL)        return E_INVALIDARG;         // 0x80070057
//   if (pDispParams == NULL) return DISP_E_BADVARTYPE;    // 0x80020008 (sic)
//   cArgs = pDispParams->cArgs;  if (cArgs > 5) return DISP_E_BADPARAMCOUNT;
//   // positional argument table: named args land at their DISPID index,
//   // the rest are taken in reverse rgvarg order.
//   for (k = 0; k < cNamedArgs; k++) {
//       idx = rgdispidNamedArgs[k]; if ((UINT)idx >= cArgs) return DISP_E_BADPARAMCOUNT;
//       args[idx] = &rgvarg[k]; }
//   for (; k < cArgs; k++) args[cArgs - k - 1] = &rgvarg[k];
//   puArgErr   = puArgErr   ? puArgErr   : &local;
//   pVarResult = pVarResult ? pVarResult : &varResult;   // vt pre-set to VT_EMPTY
//   hr = DISP_E_MEMBERNOTFOUND;  VariantInit(&varTemp);  // 0x1802c6b10 = oleaut32 #8
//   switch (dispIdMember) { ... 19 cases via the jump table at 0x28a6b4 ... }
//   VariantClear(&varTemp);                              // 0x1802c69e8 = oleaut32 #9
//   return hr;
// (VariantChangeType is import 0x1802c69e0 = oleaut32 #12.)  Notes on what
// retail actually does, reproduced as-is: the accName/accValue cases fall
// through to the accChild case when wFlags carries neither DISPATCH_PROPERTYGET
// nor DISPATCH_PROPERTYPUT; the PROPERTYPUT branches coerce a non-BSTR value
// argument to VT_I4 (not VT_BSTR) before passing its bstrVal; pExcepInfo, riid
// and lcid are never read; the local varResult used when pVarResult is NULL is
// never cleared.

inline VARIANT* AccArgAsI4(VARIANT* pArg, VARIANT* pTemp, HRESULT* phr) {
    // `if (pArg->vt != VT_I4) { hr = VariantChangeType(&varTemp, pArg, 0, VT_I4);
    //  if (FAILED(hr)) return NULL; pArg = &varTemp; }`
    if (pArg->vt != VT_I4) {
        *phr = ::VariantChangeType(pTemp, pArg, 0, VT_I4);
        if (FAILED(*phr)) {
            return nullptr;
        }
        return pTemp;
    }
    return pArg;
}

long AccessibleInvoke(void* pAcc, long dispIdMember, unsigned short wFlags, DISPPARAMS* pDispParams,
                      VARIANT* pVarResult, unsigned int* puArgErr) {
    VARIANT varResult;
    varResult.vt = VT_EMPTY;
    unsigned int uArgErrLocal = 0;

    if (pAcc == nullptr) {
        return 0x80070057;   // E_INVALIDARG
    }
    if (pDispParams == nullptr) {
        return 0x80020008;   // DISP_E_BADVARTYPE
    }
    const unsigned int cArgs = pDispParams->cArgs;
    if (cArgs > 5) {
        return 0x8002000e;   // DISP_E_BADPARAMCOUNT
    }
    VARIANT* args[5] = {};
    unsigned int k = 0;
    for (; k < pDispParams->cNamedArgs; ++k) {
        const long idx = pDispParams->rgdispidNamedArgs[k];
        if (static_cast<unsigned int>(idx) >= cArgs) {
            return 0x8002000e;   // DISP_E_BADPARAMCOUNT
        }
        args[idx] = &pDispParams->rgvarg[k];
    }
    for (; k < cArgs; ++k) {
        args[cArgs - k - 1] = &pDispParams->rgvarg[k];
    }
    if (puArgErr == nullptr) {
        puArgErr = &uArgErrLocal;
    }
    if (pVarResult == nullptr) {
        pVarResult = &varResult;
    }

    HRESULT hr = 0x80020003;   // DISP_E_MEMBERNOTFOUND
    VARIANT varTemp;
    ::VariantInit(&varTemp);
    VARIANT varChild;

    // Shared tails of the retail switch.
    auto bstrResult = [&](HRESULT h) {   // 0x28a30f: SUCCEEDED -> pVarResult->vt = VT_BSTR
        hr = h;
        if (SUCCEEDED(hr)) {
            pVarResult->vt = VT_BSTR;
        }
    };
    auto dispResult = [&](HRESULT h) {   // 0x28a666: SUCCEEDED -> pVarResult->vt = VT_DISPATCH
        hr = h;
        if (SUCCEEDED(hr)) {
            pVarResult->vt = VT_DISPATCH;
        }
    };
    auto i4Result = [&](HRESULT h) {     // 0x28a425: SUCCEEDED -> pVarResult->vt = VT_I4
        hr = h;
        if (SUCCEEDED(hr)) {
            pVarResult->vt = VT_I4;
        }
    };
    // accChild case body (0x28a600); also the fall-through target of the
    // accName / accValue cases when neither property flag is set.
    auto accChild = [&]() {
        if (cArgs != 1) {
            hr = 0x8002000e;   // DISP_E_BADPARAMCOUNT
            return;
        }
        varChild = *args[0];
        dispResult(impl__get_accChild_XAccessible_CWnd__UEAAJUtagVARIANT__PEAPEAUIDispatch___Z(
            pAcc, varChild, reinterpret_cast<void**>(&pVarResult->pdispVal)));
    };
    // PROPERTYPUT branch shared by accName / accValue (0x28a50a / 0x28a59c).
    auto propertyPut = [&](bool bName) {
        if (cArgs != 2) {
            hr = 0x8002000e;   // DISP_E_BADPARAMCOUNT
            return;
        }
        varChild = *args[0];
        VARIANT* pValue = args[1];
        if (pValue->vt != VT_BSTR) {
            hr = ::VariantChangeType(&varTemp, pValue, 0, VT_I4);   // sic: VT_I4, per retail
            if (FAILED(hr)) {
                *puArgErr = 1;
                return;
            }
            pValue = &varTemp;
        }
        hr = bName ? impl__put_accName_XAccessible_CWnd__UEAAJUtagVARIANT__PEA_W_Z(pAcc, varChild, pValue->bstrVal)
                   : impl__put_accValue_XAccessible_CWnd__UEAAJUtagVARIANT__PEA_W_Z(pAcc, varChild, pValue->bstrVal);
    };

    switch (dispIdMember) {
    case -5018: {   // accDoDefaultAction (0x28a075): IAccessible slot 25
        if (cArgs != 1) { hr = 0x8002000e; break; }
        varChild = *args[0];
        hr = impl__accDoDefaultAction_XAccessible_CWnd__UEAAJUtagVARIANT___Z(pAcc, varChild);
        break;
    }
    case -5017: {   // accHitTest (0x28a0b8): IAccessible slot 24
        if (cArgs != 2) { hr = 0x8002000e; break; }
        VARIANT* pX = AccArgAsI4(args[0], &varTemp, &hr);
        if (pX == nullptr) { *puArgErr = 0; break; }
        const long xLeft = pX->lVal;
        VARIANT* pY = AccArgAsI4(args[1], &varTemp, &hr);
        if (pY == nullptr) { *puArgErr = 1; break; }
        hr = impl__accHitTest_XAccessible_CWnd__UEAAJJJPEAUtagVARIANT___Z(pAcc, xLeft, pY->lVal, pVarResult);
        break;
    }
    case -5016: {   // accNavigate (0x28a154): IAccessible slot 23
        if (cArgs != 2) { hr = 0x8002000e; break; }
        VARIANT* pDir = AccArgAsI4(args[0], &varTemp, &hr);
        if (pDir == nullptr) { *puArgErr = 0; break; }
        varChild = *args[1];
        hr = impl__accNavigate_XAccessible_CWnd__UEAAJJUtagVARIANT__PEAU3__Z(pAcc, pDir->lVal, varChild, pVarResult);
        break;
    }
    case -5015: {   // accLocation (0x28a1c5): IAccessible slot 22
        if (cArgs != 5) { hr = 0x8002000e; break; }
        long* plong[4] = {};
        unsigned int i = 0;
        for (; i < 4; ++i) {
            VARIANT* pv = args[i];
            const VARTYPE vt = pv->vt;
            // retail: requires (vt & 0xf) != 0 and VT_BYREF set, else
            // DISP_E_TYPEMISMATCH with *puArgErr = i
            if ((vt & 0xf) == 0 || (vt & VT_BYREF) == 0) {
                break;
            }
            long* p = static_cast<long*>(pv->byref);
            if (vt & 0xc) {   // retail tests (vt & VT_VARIANT): treat as VARIANT*
                VARIANT* pInner = static_cast<VARIANT*>(pv->byref);
                ::VariantClear(pInner);
                pInner->vt = VT_I4;
                p = &pInner->lVal;
            }
            plong[i] = p;
        }
        if (i < 4) {
            hr = 0x80020005;   // DISP_E_TYPEMISMATCH
            *puArgErr = i;
            break;
        }
        varChild = *args[4];
        hr = impl__accLocation_XAccessible_CWnd__UEAAJPEAJ000UtagVARIANT___Z(pAcc, plong[0], plong[1], plong[2], plong[3], varChild);
        break;
    }
    case -5014: {   // accSelect (0x28a281): IAccessible slot 21
        if (cArgs != 2) { hr = 0x8002000e; break; }
        VARIANT* pFlags = AccArgAsI4(args[0], &varTemp, &hr);
        if (pFlags == nullptr) { *puArgErr = 0; break; }
        varChild = *args[1];
        hr = impl__accSelect_XAccessible_CWnd__UEAAJJUtagVARIANT___Z(pAcc, pFlags->lVal, varChild);
        break;
    }
    case -5013: {   // get_accDefaultAction (0x28a2ef): IAccessible slot 20
        if (cArgs != 1) { hr = 0x8002000e; break; }
        varChild = *args[0];
        bstrResult(impl__get_accDefaultAction_XAccessible_CWnd__UEAAJUtagVARIANT__PEAPEA_W_Z(pAcc, varChild, &pVarResult->bstrVal));
        break;
    }
    case -5012:     // get_accSelection (0x28a33e): IAccessible slot 19; no cArgs check
        hr = impl__get_accSelection_XAccessible_CWnd__UEAAJPEAUtagVARIANT___Z(pAcc, pVarResult);
        break;
    case -5011:     // get_accFocus (0x28a350): IAccessible slot 18; no cArgs check
        hr = impl__get_accFocus_XAccessible_CWnd__UEAAJPEAUtagVARIANT___Z(pAcc, pVarResult);
        break;
    case -5010: {   // get_accKeyboardShortcut (0x28a362): IAccessible slot 17
        if (cArgs != 1) { hr = 0x8002000e; break; }
        varChild = *args[0];
        bstrResult(impl__get_accKeyboardShortcut_XAccessible_CWnd__UEAAJUtagVARIANT__PEAPEA_W_Z(pAcc, varChild, &pVarResult->bstrVal));
        break;
    }
    case -5009: {   // get_accHelpTopic (0x28a384): IAccessible slot 16
        if (cArgs != 2) { hr = 0x8002000e; break; }
        VARIANT* pFile = args[0];
        const VARTYPE vt = pFile->vt;
        if ((vt & (VT_BYREF | VT_VARIANT)) == VT_BYREF) {   // BYREF without any 0xc bit
            hr = 0x80020005;   // DISP_E_TYPEMISMATCH
            *puArgErr = 0;
            break;
        }
        BSTR* pbstrHelpFile;
        if (vt == (VT_BYREF | VT_VARIANT)) {
            VARIANT* pInner = pFile->pvarVal;
            ::VariantClear(pInner);
            pInner->vt = VT_BSTR;
            pbstrHelpFile = &pInner->bstrVal;
        } else if (vt == (VT_BYREF | VT_BSTR)) {
            pbstrHelpFile = pFile->pbstrVal;
        } else {
            ::VariantClear(pFile);
            pFile->vt = VT_BSTR;
            pbstrHelpFile = &pFile->bstrVal;
        }
        varChild = *args[1];
        i4Result(impl__get_accHelpTopic_XAccessible_CWnd__UEAAJPEAPEA_WUtagVARIANT__PEAJ_Z(pAcc, pbstrHelpFile, varChild, &pVarResult->lVal));
        break;
    }
    case -5008: {   // get_accHelp (0x28a43a): IAccessible slot 15
        if (cArgs != 1) { hr = 0x8002000e; break; }
        varChild = *args[0];
        bstrResult(impl__get_accHelp_XAccessible_CWnd__UEAAJUtagVARIANT__PEAPEA_W_Z(pAcc, varChild, &pVarResult->bstrVal));
        break;
    }
    case -5007: {   // get_accState (0x28a45c): IAccessible slot 14
        if (cArgs != 1) { hr = 0x8002000e; break; }
        varChild = *args[0];
        hr = impl__get_accState_XAccessible_CWnd__UEAAJUtagVARIANT__PEAU3__Z(pAcc, varChild, pVarResult);
        break;
    }
    case -5006: {   // get_accRole (0x28a497): IAccessible slot 13
        if (cArgs != 1) { hr = 0x8002000e; break; }
        varChild = *args[0];
        hr = impl__get_accRole_XAccessible_CWnd__UEAAJUtagVARIANT__PEAU3__Z(pAcc, varChild, pVarResult);
        break;
    }
    case -5005: {   // get_accDescription (0x28a4b6): IAccessible slot 12
        if (cArgs != 1) { hr = 0x8002000e; break; }
        varChild = *args[0];
        bstrResult(impl__get_accDescription_XAccessible_CWnd__UEAAJUtagVARIANT__PEAPEA_W_Z(pAcc, varChild, &pVarResult->bstrVal));
        break;
    }
    case -5004: {   // accValue (0x28a4d8): get slot 11 / put slot 27
        if (wFlags & DISPATCH_PROPERTYGET) {
            if (cArgs != 1) { hr = 0x8002000e; break; }
            varChild = *args[0];
            bstrResult(impl__get_accValue_XAccessible_CWnd__UEAAJUtagVARIANT__PEAPEA_W_Z(pAcc, varChild, &pVarResult->bstrVal));
        } else if (wFlags & DISPATCH_PROPERTYPUT) {
            propertyPut(false);
        } else {
            accChild();   // retail falls into the accChild case here
        }
        break;
    }
    case -5003: {   // accName (0x28a56e): get slot 10 / put slot 26
        if (wFlags & DISPATCH_PROPERTYGET) {
            if (cArgs != 1) { hr = 0x8002000e; break; }
            varChild = *args[0];
            bstrResult(impl__get_accName_XAccessible_CWnd__UEAAJUtagVARIANT__PEAPEA_W_Z(pAcc, varChild, &pVarResult->bstrVal));
        } else if (wFlags & DISPATCH_PROPERTYPUT) {
            propertyPut(true);
        } else {
            accChild();   // retail falls into the accChild case here
        }
        break;
    }
    case -5002:     // get_accChild (0x28a600): IAccessible slot 9
        accChild();
        break;
    case -5001:     // get_accChildCount (0x28a639): IAccessible slot 8; no cArgs check
        i4Result(impl__get_accChildCount_XAccessible_CWnd__UEAAJPEAJ_Z(pAcc, &pVarResult->lVal));
        break;
    case -5000:     // get_accParent (0x28a652): IAccessible slot 7; no cArgs check
        dispResult(impl__get_accParent_XAccessible_CWnd__UEAAJPEAPEAUIDispatch___Z(pAcc, reinterpret_cast<void**>(&pVarResult->pdispVal)));
        break;
    default:        // outside the 19-entry jump table: DISP_E_MEMBERNOTFOUND
        break;
    }

    ::VariantClear(&varTemp);
    return hr;
}

}  // namespace

// Retail mfc140u RVA 0x28f730: METHOD_PROLOGUE (the owner's m_pModuleState is
// read at this-0x28 == CWnd+0x38 and handed to AFX_MAINTAIN_STATE2; not
// reproduced, see the block comment above), then the helper at 0x289f58 is
// called with `this` (the sub-object itself, not the CWnd) and the nine
// arguments passed straight through; its HRESULT is returned.
// Symbol: ?Invoke@XAccessible@CWnd@@UEAAJJAEBU_GUID@@KGPEAUtagDISPPARAMS@@PEAUtagVARIANT@@PEAUtagEXCEPINFO@@PEAI@Z
extern "C" long MS_ABI impl__Invoke_XAccessible_CWnd__UEAAJJAEBU_GUID__KGPEAUtagDISPPARAMS__PEAUtagVARIANT__PEAUtagEXCEPINFO__PEAI_Z(void* pThis, long dispIdMember, const GUID* riid, unsigned long lcid, unsigned short wFlags, DISPPARAMS* pDispParams, VARIANT* pVarResult, EXCEPINFO* pExcepInfo, unsigned int* puArgErr) {
    (void)riid; (void)lcid; (void)pExcepInfo;
    return AccessibleInvoke(pThis, dispIdMember, wFlags, pDispParams, pVarResult, puArgErr);
}

// --- XAccessibleServer ------------------------------------------------------

// Retail mfc140u RVA 0x290380 (XAccessibleServer vftable slot 3; unnamed in
// the u symbol map, the ANSI export sits at 0x28e800): METHOD_PROLOGUE with
// `lea -0x68(%rcx)`, then pThis->SetProxy(pProxy), CWnd slot 61 (+0x1e8).
// Symbol: ?SetProxy@XAccessibleServer@CWnd@@UEAAJPEAUIAccessibleProxy@@@Z
extern "C" long MS_ABI impl__SetProxy_XAccessibleServer_CWnd__UEAAJPEAUIAccessibleProxy___Z(void* pThis, void* pProxy) {
    CWnd* pWnd = OuterFromXAccessibleServer(pThis);
    return impl__SetProxy_CWnd__UEAAJPEAUIAccessibleProxy___Z(pWnd, pProxy);
}

// Retail mfc140u RVA 0x2903e0: `if (phWnd == NULL) return E_POINTER;` then
// METHOD_PROLOGUE (m_pModuleState read at this-0x30 == CWnd+0x38, see above)
// and `*phWnd = pThis->m_hWnd` (this-0x28 == CWnd+0x40); return S_OK.
// Symbol: ?GetHWND@XAccessibleServer@CWnd@@UEAAJPEAPEAUHWND__@@@Z
extern "C" long MS_ABI impl__GetHWND_XAccessibleServer_CWnd__UEAAJPEAPEAUHWND_____Z(void* pThis, HWND* phWnd) {
    if (phWnd == nullptr) {
        return E_POINTER;   // 0x80004003
    }
    CWnd* pWnd = OuterFromXAccessibleServer(pThis);
    *phWnd = pWnd->m_hWnd;
    return S_OK;
}

// Retail mfc140u RVA 0x290440: `if (ppEnumVariant == NULL) return E_POINTER;
// *ppEnumVariant = NULL; return E_NOTIMPL;` -- no METHOD_PROLOGUE.
// Symbol: ?GetEnumVariant@XAccessibleServer@CWnd@@UEAAJPEAPEAUIEnumVARIANT@@@Z
extern "C" long MS_ABI impl__GetEnumVariant_XAccessibleServer_CWnd__UEAAJPEAPEAUIEnumVARIANT___Z(void* pThis, void** ppEnumVariant) {
    (void)pThis;
    if (ppEnumVariant == nullptr) {
        return E_POINTER;   // 0x80004003
    }
    *ppEnumVariant = nullptr;
    return E_NOTIMPL;       // 0x80004001
}

namespace {
ULONG STDMETHODCALLTYPE EmbeddedAddRef(void*) { return 1; }
ULONG STDMETHODCALLTYPE EmbeddedRelease(void*) { return 1; }
HRESULT STDMETHODCALLTYPE EmbeddedAccessibleQuery(void* self, REFIID iid, void** out) {
    if (!out) return E_POINTER;
    *out = nullptr;
    CWnd* window = OuterFromXAccessible(self);
    return window->m_pProxy ? window->m_pProxy->QueryInterface(iid, out) : E_NOINTERFACE;
}
HRESULT STDMETHODCALLTYPE EmbeddedServerQuery(void* self, REFIID iid, void** out) {
    if (!out) return E_POINTER;
    *out = nullptr;
    CWnd* window = OuterFromXAccessibleServer(self);
    return window->m_pProxy ? window->m_pProxy->QueryInterface(iid, out) : E_NOINTERFACE;
}
}
void CWnd::InitializeAccessibility() {
    static void* accessible[] = {
        reinterpret_cast<void*>(EmbeddedAddRef),
        reinterpret_cast<void*>(EmbeddedRelease),
        reinterpret_cast<void*>(EmbeddedAccessibleQuery),
        reinterpret_cast<void*>(impl__Invoke_XAccessible_CWnd__UEAAJJAEBU_GUID__KGPEAUtagDISPPARAMS__PEAUtagVARIANT__PEAUtagEXCEPINFO__PEAI_Z),
        reinterpret_cast<void*>(impl__GetIDsOfNames_XAccessible_CWnd__UEAAJAEBU_GUID__PEAPEA_WIKPEAJ_Z),
        reinterpret_cast<void*>(impl__GetTypeInfoCount_XAccessible_CWnd__UEAAJPEAI_Z),
        reinterpret_cast<void*>(impl__GetTypeInfo_XAccessible_CWnd__UEAAJIKPEAPEAUITypeInfo___Z),
        reinterpret_cast<void*>(impl__get_accParent_XAccessible_CWnd__UEAAJPEAPEAUIDispatch___Z),
        reinterpret_cast<void*>(impl__get_accChildCount_XAccessible_CWnd__UEAAJPEAJ_Z),
        reinterpret_cast<void*>(impl__get_accChild_XAccessible_CWnd__UEAAJUtagVARIANT__PEAPEAUIDispatch___Z),
        reinterpret_cast<void*>(impl__get_accName_XAccessible_CWnd__UEAAJUtagVARIANT__PEAPEA_W_Z),
        reinterpret_cast<void*>(impl__get_accValue_XAccessible_CWnd__UEAAJUtagVARIANT__PEAPEA_W_Z),
        reinterpret_cast<void*>(impl__get_accDescription_XAccessible_CWnd__UEAAJUtagVARIANT__PEAPEA_W_Z),
        reinterpret_cast<void*>(impl__get_accRole_XAccessible_CWnd__UEAAJUtagVARIANT__PEAU3__Z),
        reinterpret_cast<void*>(impl__get_accState_XAccessible_CWnd__UEAAJUtagVARIANT__PEAU3__Z),
        reinterpret_cast<void*>(impl__get_accHelp_XAccessible_CWnd__UEAAJUtagVARIANT__PEAPEA_W_Z),
        reinterpret_cast<void*>(impl__get_accHelpTopic_XAccessible_CWnd__UEAAJPEAPEA_WUtagVARIANT__PEAJ_Z),
        reinterpret_cast<void*>(impl__get_accKeyboardShortcut_XAccessible_CWnd__UEAAJUtagVARIANT__PEAPEA_W_Z),
        reinterpret_cast<void*>(impl__get_accFocus_XAccessible_CWnd__UEAAJPEAUtagVARIANT___Z),
        reinterpret_cast<void*>(impl__get_accSelection_XAccessible_CWnd__UEAAJPEAUtagVARIANT___Z),
        reinterpret_cast<void*>(impl__get_accDefaultAction_XAccessible_CWnd__UEAAJUtagVARIANT__PEAPEA_W_Z),
        reinterpret_cast<void*>(impl__accSelect_XAccessible_CWnd__UEAAJJUtagVARIANT___Z),
        reinterpret_cast<void*>(impl__accLocation_XAccessible_CWnd__UEAAJPEAJ000UtagVARIANT___Z),
        reinterpret_cast<void*>(impl__accNavigate_XAccessible_CWnd__UEAAJJUtagVARIANT__PEAU3__Z),
        reinterpret_cast<void*>(impl__accHitTest_XAccessible_CWnd__UEAAJJJPEAUtagVARIANT___Z),
        reinterpret_cast<void*>(impl__accDoDefaultAction_XAccessible_CWnd__UEAAJUtagVARIANT___Z),
        reinterpret_cast<void*>(impl__put_accName_XAccessible_CWnd__UEAAJUtagVARIANT__PEA_W_Z),
        reinterpret_cast<void*>(impl__put_accValue_XAccessible_CWnd__UEAAJUtagVARIANT__PEA_W_Z),
    };
    static void* server[] = {
        reinterpret_cast<void*>(EmbeddedAddRef),
        reinterpret_cast<void*>(EmbeddedRelease),
        reinterpret_cast<void*>(EmbeddedServerQuery),
        reinterpret_cast<void*>(impl__SetProxy_XAccessibleServer_CWnd__UEAAJPEAUIAccessibleProxy___Z),
        reinterpret_cast<void*>(impl__GetHWND_XAccessibleServer_CWnd__UEAAJPEAPEAUHWND_____Z),
        reinterpret_cast<void*>(impl__GetEnumVariant_XAccessibleServer_CWnd__UEAAJPEAPEAUIEnumVARIANT___Z)
    };
    m_xAccessible = accessible;
    m_xAccessibleServer = server;
}
