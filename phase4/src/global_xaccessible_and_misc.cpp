// Accessibility + XML helper shim implementations.
//
// This batch fills currently unimplemented exports for:
// - CXMLParserCollection::CXMLParserCollection(IXMLDOMNodeList*)
// - CPreviewView::PAGE_INFO::PAGE_INFO()
// - XAccessible and XAccessibleServer methods on CWnd
//
// Real logic is not required for the compatibility surface in this project;
// methods return success (0 / S_OK-ish) and provide benign null outputs.

#include "openmfc/afxwin.h"
#include <cstring>

#ifdef __GNUC__
#  define MS_ABI __attribute__((ms_abi))
#else
#  define MS_ABI
#endif

namespace {

struct CXMLParserCollectionShim {
    void* m_pNodeList = nullptr;
};

// Keep a lightweight page-info representation for construction + zero-init.
//
// Exact field layout is not required by the phase4 compatibility layer, and
// no in-repo callers currently read members through this shim.
struct CPreviewViewPageInfoShim {
    long long fields[8] = {};
};

static inline void SetNullString(wchar_t** ppOut) {
    if (ppOut) {
        *ppOut = nullptr;
    }
}

static inline void SetNullDispatch(void** ppOut) {
    if (ppOut) {
        *ppOut = nullptr;
    }
}

static inline void SetNullObject(void** ppOut) {
    if (ppOut) {
        *ppOut = nullptr;
    }
}

} // namespace

// ---------------------------------------------------------------------------
// CXMLParserCollection
// ---------------------------------------------------------------------------

// Symbol: ??0CXMLParserCollection@@IEAA@PEAUIXMLDOMNodeList@@@Z
extern "C" void* MS_ABI
impl___0CXMLParserCollection__IEAA_PEAUIXMLDOMNodeList___Z(
    void* pThis, void* pNodeList) {
    auto* obj = static_cast<CXMLParserCollectionShim*>(pThis);
    if (obj) {
        obj->m_pNodeList = pNodeList;
        return obj;
    }
    return nullptr;
}

// ---------------------------------------------------------------------------
// CPreviewView::PAGE_INFO
// ---------------------------------------------------------------------------

// Symbol: ??0PAGE_INFO@CPreviewView@@QEAA@XZ
extern "C" void* MS_ABI impl___0PAGE_INFO_CPreviewView__QEAA_XZ(void* pThis) {
    auto* obj = static_cast<CPreviewViewPageInfoShim*>(pThis);
    if (obj) {
        *obj = CPreviewViewPageInfoShim{};
        return obj;
    }
    return nullptr;
}

// ---------------------------------------------------------------------------
// XAccessible @ CWnd
// ---------------------------------------------------------------------------

// Symbol: ?GetIDsOfNames@XAccessible@CWnd@@UEAAJAEBU_GUID@@PEAPEA_WIKPEAJ@Z
extern "C" long MS_ABI
impl__GetIDsOfNames_XAccessible_CWnd__UEAAJAEBU_GUID__PEAPEA_WIKPEAJ_Z(
    void* pThis, const void* /*riid*/, wchar_t*** /*rgszNames*/,
    unsigned int cNames, unsigned long /*lcid*/, long* rgDispId) {
    (void)pThis;
    for (unsigned int i = 0; i < cNames; ++i) {
        if (rgDispId) {
            rgDispId[i] = -1;
        }
    }
    return 0;
}

// Symbol: ?GetTypeInfo@XAccessible@CWnd@@UEAAJIKPEAPEAUITypeInfo@@@Z
extern "C" long MS_ABI
impl__GetTypeInfo_XAccessible_CWnd__UEAAJIKPEAPEAUITypeInfo___Z(
    void* pThis, unsigned int /*iTInfo*/, unsigned long /*lcid*/, void** ppTInfo) {
    (void)pThis;
    SetNullObject(ppTInfo);
    return 0;
}

// Symbol: ?GetTypeInfoCount@XAccessible@CWnd@@UEAAJPEAI@Z
extern "C" long MS_ABI
impl__GetTypeInfoCount_XAccessible_CWnd__UEAAJPEAI_Z(
    void* pThis, unsigned int* pctinfo) {
    (void)pThis;
    if (pctinfo) {
        *pctinfo = 0;
    }
    return 0;
}

// Symbol: ?Invoke@XAccessible@CWnd@@UEAAJJAEBU_GUID@@KGPEAUtagDISPPARAMS@@PEAUtagVARIANT@@PEAUtagEXCEPINFO@@PEAI@Z
extern "C" long MS_ABI
impl__Invoke_XAccessible_CWnd__UEAAJJAEBU_GUID__KGPEAUtagDISPPARAMS__PEAUtagVARIANT__PEAUtagEXCEPINFO__PEAI_Z(
    void* pThis, long /*dispIdMember*/, const void* /*riid*/, unsigned short /*wFlags*/,
    void* /*pDispParams*/, void* /*pVarResult*/, void* /*pExcepInfo*/, unsigned int* /*puArgErr*/) {
    (void)pThis;
    return 0;
}

// Symbol: ?accDoDefaultAction@XAccessible@CWnd@@UEAAJUtagVARIANT@@@Z
extern "C" long MS_ABI
impl__accDoDefaultAction_XAccessible_CWnd__UEAAJUtagVARIANT___Z(
    void* pThis, void* /*varChild*/) {
    (void)pThis;
    return 0;
}

// Symbol: ?accHitTest@XAccessible@CWnd@@UEAAJJJPEAUtagVARIANT@@@Z
extern "C" long MS_ABI
impl__accHitTest_XAccessible_CWnd__UEAAJJJPEAUtagVARIANT___Z(
    void* pThis, long /*xLeft*/, long /*yTop*/, void* /*pvarChild*/) {
    (void)pThis;
    return 0;
}

// Symbol: ?accLocation@XAccessible@CWnd@@UEAAJPEAJ000UtagVARIANT@@@Z
extern "C" long MS_ABI
impl__accLocation_XAccessible_CWnd__UEAAJPEAJ000UtagVARIANT___Z(
    void* pThis, long* left, long* top, long* width, long* height, void* /*child*/) {
    (void)pThis;
    if (left) {
        *left = 0;
    }
    if (top) {
        *top = 0;
    }
    if (width) {
        *width = 0;
    }
    if (height) {
        *height = 0;
    }
    return 0;
}

// Symbol: ?accNavigate@XAccessible@CWnd@@UEAAJJUtagVARIANT@@PEAU3@@Z
extern "C" long MS_ABI
impl__accNavigate_XAccessible_CWnd__UEAAJJUtagVARIANT__PEAU3__Z(
    void* pThis, long /*navDir*/, void* /*varStart*/, void* pvarEndUpAt) {
    (void)pThis;
    if (pvarEndUpAt) {
        std::memset(pvarEndUpAt, 0, sizeof(VARIANT));
    }
    return 0;
}

// Symbol: ?accSelect@XAccessible@CWnd@@UEAAJJUtagVARIANT@@@Z
extern "C" long MS_ABI
impl__accSelect_XAccessible_CWnd__UEAAJJUtagVARIANT___Z(
    void* pThis, long /*flagsSelect*/, void* /*varChild*/) {
    (void)pThis;
    return 0;
}

// Symbol: ?get_accChild@XAccessible@CWnd@@UEAAJUtagVARIANT@@PEAPEAUIDispatch@@@Z
extern "C" long MS_ABI
impl__get_accChild_XAccessible_CWnd__UEAAJUtagVARIANT__PEAPEAUIDispatch___Z(
    void* pThis, void* /*varChild*/, void** ppdispChild) {
    (void)pThis;
    SetNullDispatch(ppdispChild);
    return 0;
}

// Symbol: ?get_accChildCount@XAccessible@CWnd@@UEAAJPEAJ@Z
extern "C" long MS_ABI
impl__get_accChildCount_XAccessible_CWnd__UEAAJPEAJ_Z(
    void* pThis, long* pvarChildren) {
    (void)pThis;
    if (pvarChildren) {
        *pvarChildren = 0;
    }
    return 0;
}

// Symbol: ?get_accDefaultAction@XAccessible@CWnd@@UEAAJUtagVARIANT@@PEAPEA_W@Z
extern "C" long MS_ABI
impl__get_accDefaultAction_XAccessible_CWnd__UEAAJUtagVARIANT__PEAPEA_W_Z(
    void* pThis, void* /*varChild*/, wchar_t** ppDefaultAction) {
    (void)pThis;
    SetNullString(ppDefaultAction);
    return 0;
}

// Symbol: ?get_accDescription@XAccessible@CWnd@@UEAAJUtagVARIANT@@PEAPEA_W@Z
extern "C" long MS_ABI
impl__get_accDescription_XAccessible_CWnd__UEAAJUtagVARIANT__PEAPEA_W_Z(
    void* pThis, void* /*varChild*/, wchar_t** pszDescription) {
    (void)pThis;
    SetNullString(pszDescription);
    return 0;
}

// Symbol: ?get_accFocus@XAccessible@CWnd@@UEAAJPEAUtagVARIANT@@@Z
extern "C" long MS_ABI
impl__get_accFocus_XAccessible_CWnd__UEAAJPEAUtagVARIANT___Z(
    void* pThis, void* pvarChild) {
    if (pvarChild) {
        std::memset(pvarChild, 0, sizeof(VARIANT));
    }
    (void)pThis;
    return 0;
}

// Symbol: ?get_accHelp@XAccessible@CWnd@@UEAAJUtagVARIANT@@PEAPEA_W@Z
extern "C" long MS_ABI
impl__get_accHelp_XAccessible_CWnd__UEAAJUtagVARIANT__PEAPEA_W_Z(
    void* pThis, void* /*varChild*/, wchar_t** pszHelp) {
    (void)pThis;
    SetNullString(pszHelp);
    return 0;
}

// Symbol: ?get_accHelpTopic@XAccessible@CWnd@@UEAAJPEAPEA_WUtagVARIANT@@PEAJ@Z
extern "C" long MS_ABI
impl__get_accHelpTopic_XAccessible_CWnd__UEAAJPEAPEA_WUtagVARIANT__PEAJ_Z(
    void* pThis, wchar_t** /*pszHelpFile*/, void* /*varChild*/, long* /*pdwHelpContext*/) {
    (void)pThis;
    return 0;
}

// Symbol: ?get_accKeyboardShortcut@XAccessible@CWnd@@UEAAJUtagVARIANT@@PEAPEA_W@Z
extern "C" long MS_ABI
impl__get_accKeyboardShortcut_XAccessible_CWnd__UEAAJUtagVARIANT__PEAPEA_W_Z(
    void* pThis, void* /*varChild*/, wchar_t** pszKeyboardShortcut) {
    (void)pThis;
    SetNullString(pszKeyboardShortcut);
    return 0;
}

// Symbol: ?get_accName@XAccessible@CWnd@@UEAAJUtagVARIANT@@PEAPEA_W@Z
extern "C" long MS_ABI
impl__get_accName_XAccessible_CWnd__UEAAJUtagVARIANT__PEAPEA_W_Z(
    void* pThis, void* /*varChild*/, wchar_t** pszName) {
    (void)pThis;
    SetNullString(pszName);
    return 0;
}

// Symbol: ?get_accParent@XAccessible@CWnd@@UEAAJPEAPEAUIDispatch@@@Z
extern "C" long MS_ABI
impl__get_accParent_XAccessible_CWnd__UEAAJPEAPEAUIDispatch___Z(
    void* pThis, void** ppdispParent) {
    (void)pThis;
    SetNullDispatch(ppdispParent);
    return 0;
}

// Symbol: ?get_accRole@XAccessible@CWnd@@UEAAJUtagVARIANT@@PEAU3@@Z
extern "C" long MS_ABI
impl__get_accRole_XAccessible_CWnd__UEAAJUtagVARIANT__PEAU3__Z(
    void* pThis, void* /*varChild*/, void* pvarRole) {
    if (pvarRole) {
        std::memset(pvarRole, 0, sizeof(VARIANT));
    }
    (void)pThis;
    return 0;
}

// Symbol: ?get_accSelection@XAccessible@CWnd@@UEAAJPEAUtagVARIANT@@@Z
extern "C" long MS_ABI
impl__get_accSelection_XAccessible_CWnd__UEAAJPEAUtagVARIANT___Z(
    void* pThis, void* pvarChildren) {
    (void)pThis;
    if (pvarChildren) {
        std::memset(pvarChildren, 0, sizeof(VARIANT));
    }
    return 0;
}

// Symbol: ?get_accState@XAccessible@CWnd@@UEAAJUtagVARIANT@@PEAU3@@Z
extern "C" long MS_ABI
impl__get_accState_XAccessible_CWnd__UEAAJUtagVARIANT__PEAU3__Z(
    void* pThis, void* /*varChild*/, void* pvarState) {
    if (pvarState) {
        std::memset(pvarState, 0, sizeof(VARIANT));
    }
    (void)pThis;
    return 0;
}

// Symbol: ?get_accValue@XAccessible@CWnd@@UEAAJUtagVARIANT@@PEAPEA_W@Z
extern "C" long MS_ABI
impl__get_accValue_XAccessible_CWnd__UEAAJUtagVARIANT__PEAPEA_W_Z(
    void* pThis, void* /*varChild*/, wchar_t** pszValue) {
    (void)pThis;
    SetNullString(pszValue);
    return 0;
}

// Symbol: ?put_accName@XAccessible@CWnd@@UEAAJUtagVARIANT@@PEA_W@Z
extern "C" long MS_ABI
impl__put_accName_XAccessible_CWnd__UEAAJUtagVARIANT__PEA_W_Z(
    void* pThis, void* /*varChild*/, wchar_t* /*szName*/) {
    (void)pThis;
    return 0;
}

// Symbol: ?put_accValue@XAccessible@CWnd@@UEAAJUtagVARIANT@@PEA_W@Z
extern "C" long MS_ABI
impl__put_accValue_XAccessible_CWnd__UEAAJUtagVARIANT__PEA_W_Z(
    void* pThis, void* /*varChild*/, wchar_t* /*szValue*/) {
    (void)pThis;
    return 0;
}

// ---------------------------------------------------------------------------
// XAccessibleServer @ CWnd
// ---------------------------------------------------------------------------

// Symbol: ?GetEnumVariant@XAccessibleServer@CWnd@@UEAAJPEAPEAUIEnumVARIANT@@@Z
extern "C" long MS_ABI
impl__GetEnumVariant_XAccessibleServer_CWnd__UEAAJPEAPEAUIEnumVARIANT___Z(
    void* pThis, void** ppenum) {
    (void)pThis;
    SetNullObject(ppenum);
    return 0;
}

// Symbol: ?GetHWND@XAccessibleServer@CWnd@@UEAAJPEAPEAUHWND__@@@Z
extern "C" long MS_ABI
impl__GetHWND_XAccessibleServer_CWnd__UEAAJPEAPEAUHWND_____Z(
    void* pThis, void* phWnd) {
    (void)pThis;
    if (phWnd) {
        *static_cast<HWND*>(phWnd) = nullptr;
    }
    return 0;
}

// Symbol: ?SetProxy@XAccessibleServer@CWnd@@UEAAJPEAUIAccessibleProxy@@@Z
extern "C" long MS_ABI
impl__SetProxy_XAccessibleServer_CWnd__UEAAJPEAUIAccessibleProxy___Z(
    void* pThis, void* /*pAccessibleProxy*/) {
    (void)pThis;
    return 0;
}
