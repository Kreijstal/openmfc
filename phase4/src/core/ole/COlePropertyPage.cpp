// COlePropertyPage — OpenMFC implementation.
// Sources: olecore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/OlecoreSupport.h"
#include <cstddef>
#include <cstdlib>

// ---------------------------------------------------------------------------
// Retail layout (mfc140u.dll, all RVAs in this file are mfc140u RVAs unless
// a note says otherwise).
//
// Read out of the constructor ??0COlePropertyPage@@QEAA@II@Z (RVA 0x1e8ee0),
// which stores or zeroes every one of these slots, and cross-checked against
// each reader named in the per-function notes below:
//
//   0x130  CPtrArray       m_arrayDDP     vfptr 0x321190, four zeroed qwords
//   0x158  BOOL            m_bDirty
//   0x15c  UINT            m_idDialog     (ctor arg 1; its low word is also
//                                          mirrored into 0xf0, CDialog's
//                                          m_lpszTemplateName: `movzwl
//                                          0x15c(%rsi),%eax; mov %rax,0xf0`)
//   0x160  UINT            m_idCaption    (ctor arg 2)
//   0x168  CString         m_strPageName
//   0x170  SIZE            m_sizePage
//   0x178  CString         m_strDocString
//   0x180  CString         m_strHelpFile
//   0x188  DWORD           m_dwHelpContext
//   0x190  IPropertyPageSite* m_pPageSite
//   0x198  LPDISPATCH*     m_ppDisp
//   0x1a0  DWORD*          m_pAdvisors    (connection cookies)
//   0x1a8  BOOL            m_bPropsChanged (deferred-refill flag)
//   0x1ac  ULONG           m_nObjects
//   0x1b0  BOOL            m_bInitializing (ctor: `movq $1,0x1b0`, which also
//                                          zeroes 0x1b4)
//   0x1b4  int             m_nControls
//   0x1b8  {UINT,BOOL}*    m_pStatus      (8-byte entries: id, dirty)
//   0x1c0  CDWordArray     m_IDArray      vfptr 0x321130 (slot 0 is
//                                          ?GetRuntimeClass@CDWordArray@@,
//                                          0x1d4550), m_pData 0x1c8,
//                                          m_nSize 0x1d0, m_nMaxSize 0x1d8,
//                                          m_nGrowBy 0x1e0 (INT_PTRs: the
//                                          ctor zeroes them as qwords and
//                                          IgnoreApply reads m_nSize as one)
//   0x1e8  HGLOBAL         m_hDialog
//   0x1f0  XPropertyPage   vfptr 0x324928 (15 slots, IPropertyPage2)
//   0x1f8  XPropNotifySink vfptr 0x3248f8 (5 slots)
//   sizeof 0x200 (CRuntimeClass descriptor at 0x3248c0, m_nObjectSize 0x200)
//
// The member names are the ones MFC's public afxctl.h gives these slots; the
// offsets and types are what the disassembly shows.
//
// OpenMFC's declaration (include/openmfc/afxole.h) lays the class out
// differently. Measured under this file's own build flags (the static_asserts
// below fail the compile if any of it drifts):
//
//     sizeof(CDialog)          0x130    (agrees with retail)
//     m_pPageSite              0x130    retail 0x190
//     m_bModified              0x138    retail m_bDirty 0x158
//     m_pszPageName            0x140    retail CString m_strPageName 0x168
//     m_pszHelpFile            0x148    retail CString 0x180
//     m_dwHelpContext          0x150    retail 0x188
//     m_pszDocString           0x158    retail CString 0x178
//     _olepropertypage_padding 0x160..0x200
//     sizeof(COlePropertyPage) 0x200    (agrees with retail)
//
// So the six members OpenMFC declares occupy 0x130..0x160 (where retail keeps
// m_arrayDDP, m_bDirty and the two ids) and are used here in place of their
// retail counterparts: m_bModified for m_bDirty, m_pPageSite for the site,
// the three wchar_t* for the three CStrings, and CDialog::m_lpszTemplateName
// (which the CDialog(UINT) ctor sets to MAKEINTRESOURCE(id)) for m_idDialog.
// Everything retail keeps at >= 0x160 has no OpenMFC counterpart and lives
// entirely inside _olepropertypage_padding, so it is modelled below at its
// exact retail offset by S_COlePropertyPageTail (see the static_asserts).
// Two retail slots in that window duplicate members OpenMFC already declares
// (0x168..0x190) and are deliberately left unused. The DDP array at 0x130 is
// NOT modelled: it collides with the declared members, and the DDP_ family is
// not in this file (headerRequest).
//
// The nested COM sub-objects ARE modelled: the constructors install the two
// vtables defined in this file at 0x1f0 and 0x1f8, so the XPropertyPage /
// XPropNotifySink entry points below recover the outer object with the same
// -0x1f0 / -0x1f8 adjustment retail uses. What is still missing is the
// interface map: g_imap_COlePropertyPage (detail/InterfaceMapsSupport.cpp) is
// the bare inherited CCmdTarget map, and CCmdTarget::GetInterface only walks
// CCmdTarget's own map, so QueryInterface(IID_IPropertyPage[2]) on a page
// still fails and nothing outside this file hands out either sub-object.
// Both are headerRequests; the bodies here are complete and waiting on them.
// (Inside this file the sink IS reachable: SetObjects passes &m_xPropNotifySink
// to AfxConnectionAdvise, exactly as retail does.)
//
// Not reproduced anywhere in this file, and stated once: every nested-interface
// body except GetPageInfo, Show, Move, IsPageDirty and OnRequestEdit opens
// with an AFX_MAINTAIN_STATE2 frame (??0AFX_MAINTAIN_STATE2@@ 0x133170 on
// this->m_pModuleState at +0x38); WindowProc has one too. OpenMFC's thunks do
// not push module state at these boundaries.
// ---------------------------------------------------------------------------

// Implementations owned by other translation units. C++ methods of these
// classes exist in this DLL only as extern "C" impl__ thunks, so they must be
// called through the thunk, never through the C++ declaration.
extern "C" int MS_ABI impl__DestroyWindow_CWnd__UEAAHXZ(CWnd* pThis);
extern "C" int MS_ABI impl__OnInitDialog_CDialog__UEAAHXZ(CDialog* pThis);
extern "C" int MS_ABI impl__PreTranslateMessage_CDialog__UEAAHPEAUtagMSG___Z(CDialog* pThis, MSG* pMsg);
extern "C" int MS_ABI impl__SendChildNotifyLastMsg_CWnd__QEAAHPEA_J_Z(CWnd* pThis, LONGLONG* pResult);
extern "C" __int64 MS_ABI impl__Default_CWnd__IEAA_JXZ(CWnd* pThis);
extern "C" const MSG* MS_ABI impl__GetCurrentMessage_CWnd__KAPEBUtagMSG__XZ();
extern "C" CWnd* MS_ABI impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(HWND hWnd);
extern "C" unsigned long MS_ABI impl__GetStyle_CWnd__QEBAKXZ(const CWnd* pThis);
extern "C" CWnd* MS_ABI impl__GetDlgItem_CWnd__QEBAPEAV1_H_Z(const CWnd* pThis, int nID);
extern "C" int MS_ABI impl__IsWindowEnabled_CWnd__QEBAHXZ(const CWnd* pThis);
extern "C" HWND MS_ABI impl__GetSafeOwner__CWnd__SAPEAUHWND____PEAU2_PEAPEAU2__Z(HWND hParent, HWND* pWndTop);
extern "C" LRESULT MS_ABI impl__WindowProc_CWnd__MEAA_JI_K_J_Z(CWnd* pThis, UINT message, WPARAM wParam, LPARAM lParam);
extern "C" int MS_ABI impl__OnCommand_CWnd__MEAAH_K_J_Z(CWnd* pThis, WPARAM wParam, LPARAM lParam);
extern "C" void MS_ABI impl__MoveWindow_CWnd__QEAAXHHHHH_Z(CWnd* pThis, int x, int y, int nWidth, int nHeight, int bRepaint);
extern "C" int MS_ABI impl__ShowWindow_CWnd__QEAAHH_Z(CWnd* pThis, int nCmdShow);
extern "C" void* MS_ABI impl__SetFocus_CWnd__QEAAPEAV1_XZ(CWnd* pThis);
extern "C" int MS_ABI impl__UpdateData_CWnd__QEAAHH_Z(CWnd* pThis, int bSaveAndValidate);
extern "C" int MS_ABI impl__Create_CDialog__UEAAHPEB_WPEAVCWnd___Z(CDialog* pThis, const wchar_t* lpszTemplateName, CWnd* pParentWnd);
extern "C" int MS_ABI impl__CreateIndirect_CDialog__UEAAHPEAXPEAVCWnd___Z(CDialog* pThis, void* lpDialogTemplate, CWnd* pParentWnd);
extern "C" unsigned long MS_ABI impl__ExternalQueryInterface_CCmdTarget__QEAAKPEBXPEAPEAX_Z(CCmdTarget* pThis, const void* iid, void** ppvObj);
extern "C" unsigned long MS_ABI impl__ExternalAddRef_CCmdTarget__QEAAKXZ(CCmdTarget* pThis);
extern "C" unsigned long MS_ABI impl__ExternalRelease_CCmdTarget__QEAAKXZ(CCmdTarget* pThis);
extern "C" int MS_ABI impl__AfxConnectionAdvise__YAHPEAUIUnknown__AEBU_GUID__0HPEAK_Z(IUnknown* pUnkSrc, const GUID* iid, IUnknown* pUnkSink, int bRefCount, unsigned long* pdwCookie);
extern "C" int MS_ABI impl__AfxConnectionUnadvise__YAHPEAUIUnknown__AEBU_GUID__0HK_Z(IUnknown* pUnkSrc, const GUID* iid, IUnknown* pUnkSink, int bRefCount, unsigned long dwCookie);
extern "C" void MS_ABI impl__AfxThrowInvalidArgException__YAXXZ();
// COleDispatchDriver (detail/OlecoreSupport.cpp). GetProperty
// (?GetProperty@COleDispatchDriver@@QEBAXJGPEAX@Z, mfc140u 0x252690) is
// InvokeHelper(dispid, DISPATCH_PROPERTYGET, vt, pv, NULL) -- retail's body
// is exactly that one call onto InvokeHelper (0x252660) -- and SetProperty
// (0x2526c0) is InvokeHelperV(dispid, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL,
// {vt, 0}, ...). The retail GetProp* bodies call InvokeHelper (0x252660)
// directly with those same arguments and the retail SetProp* bodies call
// SetProperty (0x2526c0); calling the two exported thunks here rather than
// the inline InvokeHelper in afxdisp.h keeps this TU's undefined-symbol set
// unchanged.
extern "C" void MS_ABI impl__GetProperty_COleDispatchDriver__QEBAXJGPEAX_Z(const COleDispatchDriver* pThis, long dwDispID, unsigned short vtProp, void* pvProp);
extern "C" void MS_ABI impl__SetProperty_COleDispatchDriver__QEAAXJGZZ(COleDispatchDriver* pThis, long dwDispID, unsigned short vtProp, ...);
// CDialogTemplate (core/dialog/CDialogTemplate.cpp). The two prototypes below
// are derived from the mangled names -- `this` in RCX, the argument in RDX.
// GetSizeInPixels' DEFINITION in that file still carries an auto-generated
// one-parameter placeholder list and an empty body (headerRequest); calling
// it through the correct prototype is harmless today (it reads nothing) and
// becomes right the moment that file is fixed.
extern "C" void MS_ABI impl___0CDialogTemplate__QEAA_PEAX_Z(void* pThis, void* hTemplate);
extern "C" void MS_ABI impl___1CDialogTemplate__QEAA_XZ(void* pThis);
extern "C" void MS_ABI impl__GetSizeInPixels_CDialogTemplate__QEBAXPEAUtagSIZE___Z(const void* pThis, SIZE* pSize);

// Forward declarations of this file's own thunks that the sub-object vtables
// and the C++ members below refer to before their definitions.
extern "C" void MS_ABI impl__CleanupObjectArray_COlePropertyPage__IEAAXXZ(COlePropertyPage* pThis);
extern "C" int MS_ABI impl__EnumChildProc_COlePropertyPage__KAHPEAUHWND_____J_Z(HWND hWnd, __int64 lParam);
extern "C" int MS_ABI impl__EnumControls_COlePropertyPage__KAHPEAUHWND_____J_Z(HWND hWnd, __int64 lParam);
extern "C" int MS_ABI impl__OnHelp_COlePropertyPage__UEAAHPEB_W_Z(COlePropertyPage* pThis, const wchar_t* lpszHelpDir);
extern "C" int MS_ABI impl__PreTranslateMessage_COlePropertyPage__MEAAHPEAUtagMSG___Z(COlePropertyPage* pThis, MSG* pMsg);
// GetPropIndex / SetPropIndex are single `jmp`s onto these two in retail
// (0x1ec3c0 -> 0x1ec240, 0x1ec3b0 -> 0x1ec100, mfc140u).
extern "C" int MS_ABI impl__GetPropRadio_COlePropertyPage__QEAAHPEB_WPEAH_Z(COlePropertyPage* pThis, const wchar_t* pszPropName, int* pValue);
extern "C" int MS_ABI impl__SetPropRadio_COlePropertyPage__QEAAHPEB_WH_Z(COlePropertyPage* pThis, const wchar_t* pszPropName, int nValue);
extern "C" long MS_ABI impl__SetPageSite_XPropertyPage_COlePropertyPage__UEAAJPEAUIPropertyPageSite___Z(void* pThis, void* pPageSite);
extern "C" long MS_ABI impl__Activate_XPropertyPage_COlePropertyPage__UEAAJPEAUHWND____PEBUtagRECT__H_Z(void* pThis, HWND hWndParent, const RECT* prc, int bModal);
extern "C" long MS_ABI impl__Deactivate_XPropertyPage_COlePropertyPage__UEAAJXZ(void* pThis);
extern "C" long MS_ABI impl__GetPageInfo_XPropertyPage_COlePropertyPage__UEAAJPEAUtagPROPPAGEINFO___Z(void* pThis, void* pPageInfo);
extern "C" long MS_ABI impl__SetObjects_XPropertyPage_COlePropertyPage__UEAAJKPEAPEAUIUnknown___Z(void* pThis, unsigned long nObjects, void** ppUnk);
extern "C" long MS_ABI impl__Show_XPropertyPage_COlePropertyPage__UEAAJI_Z(void* pThis, unsigned int nCmdShow);
extern "C" long MS_ABI impl__Move_XPropertyPage_COlePropertyPage__UEAAJPEBUtagRECT___Z(void* pThis, const RECT* prc);
extern "C" long MS_ABI impl__IsPageDirty_XPropertyPage_COlePropertyPage__UEAAJXZ(void* pThis);
extern "C" long MS_ABI impl__Apply_XPropertyPage_COlePropertyPage__UEAAJXZ(void* pThis);
extern "C" long MS_ABI impl__Help_XPropertyPage_COlePropertyPage__UEAAJPEB_W_Z(void* pThis, const wchar_t* pszHelpDir);
extern "C" long MS_ABI impl__TranslateAcceleratorW_XPropertyPage_COlePropertyPage__UEAAJPEAUtagMSG___Z(void* pThis, void* pMsg);
extern "C" long MS_ABI impl__EditProperty_XPropertyPage_COlePropertyPage__UEAAJJ_Z(void* pThis, long dispid);
extern "C" long MS_ABI impl__OnChanged_XPropNotifySink_COlePropertyPage__UEAAJJ_Z(void* pThis, long dispid);
extern "C" long MS_ABI impl__OnRequestEdit_XPropNotifySink_COlePropertyPage__UEAAJJ_Z(void* pThis, long dispid);

namespace {

// ---------------------------------------------------------------------------
// The retail members at 0x160..0x200, overlaid on _olepropertypage_padding.
// ---------------------------------------------------------------------------
struct AFX_PPFIELDSTATUS {
    UINT nID;       // +0  (EnumControls: `mov %eax,(%rcx,%r8,8)`)
    BOOL bDirty;    // +4  (EnumControls: `movl $0,0x4(%rax,%rcx,8)`)
};
static_assert(sizeof(AFX_PPFIELDSTATUS) == 8, "retail control-status entries are 8 bytes");

struct S_COlePropertyPageTail {
    UINT               m_idCaption;            // 0x160
    UINT               _pad164;                // 0x164
    void*              _retail_strPageName;    // 0x168  unused: OpenMFC keeps m_pszPageName (0x140)
    SIZE               m_sizePage;             // 0x170
    void*              _retail_strDocString;   // 0x178  unused: m_pszDocString (0x158)
    void*              _retail_strHelpFile;    // 0x180  unused: m_pszHelpFile (0x148)
    DWORD              _retail_dwHelpContext;  // 0x188  unused: m_dwHelpContext (0x150)
    DWORD              _pad18c;                // 0x18c
    void*              _retail_pPageSite;      // 0x190  unused: m_pPageSite (0x130)
    LPDISPATCH*        m_ppDisp;               // 0x198
    DWORD*             m_pAdvisors;            // 0x1a0
    BOOL               m_bPropsChanged;        // 0x1a8
    ULONG              m_nObjects;             // 0x1ac
    BOOL               m_bInitializing;        // 0x1b0
    int                m_nControls;            // 0x1b4
    AFX_PPFIELDSTATUS* m_pStatus;              // 0x1b8
    // Retail's CDWordArray m_IDArray, 0x1c0..0x1e8. OpenMFC's CDWordArray is
    // an 8-byte CObject whose elements live in a side table (detail/
    // FilecoreSupport.h) with no exported GetSize/GetAt, so the retail element
    // storage is kept here directly at the retail offsets instead; the vfptr
    // slot stays NULL because no CDWordArray object is constructed on it.
    const void*        m_IDArray_vfptr;        // 0x1c0  (NULL here; retail 0x321130)
    DWORD*             m_IDArray_pData;        // 0x1c8
    INT_PTR            m_IDArray_nSize;        // 0x1d0
    INT_PTR            m_IDArray_nMaxSize;     // 0x1d8
    INT_PTR            m_IDArray_nGrowBy;      // 0x1e0
    HGLOBAL            m_hDialog;              // 0x1e8
    const void*        m_xPropertyPage_vfptr;  // 0x1f0
    const void*        m_xPropNotifySink_vfptr;// 0x1f8
};
constexpr size_t kTailOffset            = 0x160;
constexpr size_t kXPropertyPageOffset   = 0x1f0;
constexpr size_t kXPropNotifySinkOffset = 0x1f8;

static_assert(sizeof(S_COlePropertyPageTail) == 0xa0, "tail must cover 0x160..0x200 exactly");
static_assert(offsetof(S_COlePropertyPageTail, m_sizePage)      == 0x170 - kTailOffset, "m_sizePage");
static_assert(offsetof(S_COlePropertyPageTail, m_ppDisp)        == 0x198 - kTailOffset, "m_ppDisp");
static_assert(offsetof(S_COlePropertyPageTail, m_pAdvisors)     == 0x1a0 - kTailOffset, "m_pAdvisors");
static_assert(offsetof(S_COlePropertyPageTail, m_bPropsChanged) == 0x1a8 - kTailOffset, "m_bPropsChanged");
static_assert(offsetof(S_COlePropertyPageTail, m_nObjects)      == 0x1ac - kTailOffset, "m_nObjects");
static_assert(offsetof(S_COlePropertyPageTail, m_bInitializing) == 0x1b0 - kTailOffset, "m_bInitializing");
static_assert(offsetof(S_COlePropertyPageTail, m_nControls)     == 0x1b4 - kTailOffset, "m_nControls");
static_assert(offsetof(S_COlePropertyPageTail, m_pStatus)       == 0x1b8 - kTailOffset, "m_pStatus");
static_assert(offsetof(S_COlePropertyPageTail, m_IDArray_vfptr) == 0x1c0 - kTailOffset, "m_IDArray");
static_assert(offsetof(S_COlePropertyPageTail, m_IDArray_pData) == 0x1c8 - kTailOffset, "m_IDArray.m_pData");
static_assert(offsetof(S_COlePropertyPageTail, m_IDArray_nSize) == 0x1d0 - kTailOffset, "m_IDArray.m_nSize");
static_assert(offsetof(S_COlePropertyPageTail, m_hDialog)       == 0x1e8 - kTailOffset, "m_hDialog");
static_assert(offsetof(S_COlePropertyPageTail, m_xPropertyPage_vfptr)   == kXPropertyPageOffset   - kTailOffset, "m_xPropertyPage");
static_assert(offsetof(S_COlePropertyPageTail, m_xPropNotifySink_vfptr) == kXPropNotifySinkOffset - kTailOffset, "m_xPropNotifySink");

// The OpenMFC side of the table in the header comment, pinned. The padding
// member is protected, so a derived probe re-exposes it and the offsets are
// taken once the probe type is complete.
struct COlePropertyPageProbe : COlePropertyPage {
    using COlePropertyPage::_olepropertypage_padding;
};
constexpr size_t kProbePadding     = offsetof(COlePropertyPageProbe, _olepropertypage_padding);
constexpr size_t kProbePaddingSize = sizeof(COlePropertyPageProbe::_olepropertypage_padding);
static_assert(sizeof(CDialog) == 0x130, "CDialog size");
static_assert(sizeof(COlePropertyPage) == 0x200, "COlePropertyPage size (retail 0x200)");
static_assert(offsetof(COlePropertyPage, m_pPageSite)    == 0x130, "m_pPageSite");
static_assert(offsetof(COlePropertyPage, m_bModified)    == 0x138, "m_bModified");
static_assert(offsetof(COlePropertyPage, m_pszPageName)  == 0x140, "m_pszPageName");
static_assert(offsetof(COlePropertyPage, m_pszHelpFile)  == 0x148, "m_pszHelpFile");
static_assert(offsetof(COlePropertyPage, m_dwHelpContext)== 0x150, "m_dwHelpContext");
static_assert(offsetof(COlePropertyPage, m_pszDocString) == 0x158, "m_pszDocString");
static_assert(kProbePadding == kTailOffset, "padding must start at 0x160");
static_assert(kProbePaddingSize == sizeof(S_COlePropertyPageTail), "padding must be 0xa0 bytes");

inline S_COlePropertyPageTail* Tail(COlePropertyPage* p) {
    return reinterpret_cast<S_COlePropertyPageTail*>(reinterpret_cast<unsigned char*>(p) + kTailOffset);
}
inline COlePropertyPage* OuterFromPropPage(void* pSub) {
    return reinterpret_cast<COlePropertyPage*>(reinterpret_cast<unsigned char*>(pSub) - kXPropertyPageOffset);
}
inline COlePropertyPage* OuterFromNotifySink(void* pSub) {
    return reinterpret_cast<COlePropertyPage*>(reinterpret_cast<unsigned char*>(pSub) - kXPropNotifySinkOffset);
}
inline IUnknown* NotifySinkOf(COlePropertyPage* p) {
    return reinterpret_cast<IUnknown*>(&Tail(p)->m_xPropNotifySink_vfptr);
}

// GUIDs, spelled out so this file does not depend on which uuid library the
// final link pulls in. Each was read out of mfc140u .rdata at the address the
// retail bodies load: IID_NULL 0x2d98d8 (all zero), IID_IUnknown 0x2d9a58,
// IID_IDispatch 0x2d9b48, IID_IPropertyNotifySink 0x2d9d28
// ({9BFBBC02-EFF1-101A-84ED-00AA00341D07}).
const GUID kIID_NULL               = { 0x00000000, 0x0000, 0x0000, { 0,0,0,0,0,0,0,0 } };
const GUID kIID_IUnknown           = { 0x00000000, 0x0000, 0x0000, { 0xC0,0,0,0,0,0,0,0x46 } };
const GUID kIID_IDispatch          = { 0x00020400, 0x0000, 0x0000, { 0xC0,0,0,0,0,0,0,0x46 } };
const GUID kIID_IPropertyNotifySink= { 0x9BFBBC02, 0xEFF1, 0x101A, { 0x84,0xED,0x00,0xAA,0x00,0x34,0x1D,0x07 } };

// ---------------------------------------------------------------------------
// The lockout window. Retail CWnd::UpdateData (0x2910d0) parks m_hWnd in
// AFX_THREAD_STATE::m_hLockoutNotifyWindow (+0x198 in mfc140u; +0x138 in the
// MBCS twin) for the duration of DoDataExchange and restores the previous
// value afterwards, and XPropNotifySink::OnChanged (0x1ea5d0) reads it back
// to decide between an immediate refill and a deferred one. OpenMFC's
// CWnd::UpdateData maintains no such field and AFX_THREAD_STATE does not
// model it (headerRequest). This file emulates it for the UpdateData calls
// it makes itself -- which is every one the retail COlePropertyPage bodies
// make -- so an OnChanged fired from inside Apply's own DoDataExchange is
// deferred exactly as in retail. DEVIATION: a derived page's own direct
// UpdateData() calls are not covered until CWnd::UpdateData maintains the
// real field.
thread_local HWND t_hLockoutNotifyWindow = nullptr;

int PageUpdateData(COlePropertyPage* pPage, BOOL bSaveAndValidate) {
    HWND hSave = t_hLockoutNotifyWindow;
    t_hLockoutNotifyWindow = pPage->m_hWnd;
    int bResult = impl__UpdateData_CWnd__QEAAHH_Z(static_cast<CWnd*>(pPage), bSaveAndValidate);
    t_hLockoutNotifyWindow = hSave;
    return bResult;
}

// XPropertyPage::GetPageInfo's string helper (0x1e77b4, called three times):
//     if (!s) return NULL;
//     n = wcslen(s); if (n == (size_t)-1) return NULL;      ; `not; cmp $1; jb`
//     bytes = (n + 1) * 2, with an overflow check;         ; `div; cmp $2; jb`
//     p = CoTaskMemAlloc(bytes); if (p) memcpy(p, s, bytes); return p;
// (IAT 0x2c7748 wcslen, 0x2c7968 ole32!CoTaskMemAlloc, 0x2c7420 memcpy.)
LPOLESTR TaskAllocString(const wchar_t* s) {
    if (s == nullptr) return nullptr;
    size_t n = ::wcslen(s);
    if (n == static_cast<size_t>(-1)) return nullptr;
    size_t bytes = (n + 1) * sizeof(wchar_t);
    if (bytes / sizeof(wchar_t) != n + 1) return nullptr;
    LPOLESTR p = static_cast<LPOLESTR>(::CoTaskMemAlloc(bytes));
    if (p) ::memcpy(p, s, bytes);
    return p;
}

// ---------------------------------------------------------------------------
// The GetProp* / SetProp* shape, shared by all 24 entry points. Every retail
// body opens the same way (checked one by one, RVAs at each thunk below):
//     COleDispatchDriver driver;                    ; m_lpDispatch 0, auto 1
//     BOOL bResult = FALSE;                         ; ESI
//     CString strName(pszPropName);                 ; 0xdcb0, CStringT(LPCWSTR)
//     LPOLESTR rgszNames[1] = { pszPropName ? (LPCWSTR)strName : NULL };
//                                       ; `neg %rbx; sbb %rcx,%rcx; and buf`
//     for (i = 0; i < m_nObjects; ++i) {            ; unsigned compare
//         LPDISPATCH p = m_ppDisp[i];
//         if (FAILED(p->GetIDsOfNames(IID_NULL, rgszNames, 1, 0 /*lcid*/,
//                                     &dispid)))    ; vtable slot 5 (+0x28)
//             continue;                             ; `js` -> next object
//         driver.AttachDispatch(p, FALSE);          ; 0x251e40
//         <get or put, see each thunk>
//         driver.m_lpDispatch = NULL;               ; detach without Release
//         <merge, see each thunk>
//         bResult = TRUE;
//     }
//     return bResult;                               ; strName / driver dtors
// Here the name is passed straight through: retail's CString copy is only ever
// read, and CString(NULL) followed by the NULL mask yields the same pointer
// value the caller supplied. The put side goes through
// COleDispatchDriver::SetProperty (0x2526c0), which is InvokeHelperV with
// wFlags = DISPATCH_PROPERTYPUT (DISPATCH_PROPERTYPUTREF only for
// VT_DISPATCH, never the case here) and a one-byte param-info of the VARTYPE.
// Both sides are called through the exported COleDispatchDriver thunks
// (declared at the top of this file); the get side is the GetProperty thunk
// (0x252690), whose body is exactly the InvokeHelper (0x252660,
// DISPATCH_PROPERTYGET, pbParamInfo NULL) call the retail GetProp* bodies
// make directly. Either one throws COleException on a failed Invoke
// (OpenMFC's InvokeHelperV in afxdisp.h), exactly as retail does, and the
// retail bodies have no handler either.
// DEVIATION (defensive, both families): retail dereferences `this` and the
// out-parameter unconditionally; a NULL either returns FALSE here.
bool LookupDispID(LPDISPATCH pDisp, const wchar_t* pszPropName, DISPID* pDispID) {
    LPOLESTR rgszNames[1] = { const_cast<LPOLESTR>(pszPropName) };
    return SUCCEEDED(pDisp->GetIDsOfNames(kIID_NULL, rgszNames, 1, 0, pDispID));
}

template <typename T>
int GetPropScalar(COlePropertyPage* pThis, const wchar_t* pszPropName, T* pValue,
                  VARTYPE vt, T indeterminate) {
    if (!pThis || !pValue) return FALSE;
    S_COlePropertyPageTail* t = Tail(pThis);
    int bResult = FALSE;
    for (ULONG i = 0; i < t->m_nObjects; ++i) {
        LPDISPATCH pDisp = t->m_ppDisp[i];
        DISPID dispid = 0;
        if (!LookupDispID(pDisp, pszPropName, &dispid)) continue;
        T value = T();
        COleDispatchDriver driver;
        driver.AttachDispatch(pDisp, FALSE);
        impl__GetProperty_COleDispatchDriver__QEBAXJGPEAX_Z(&driver, dispid, vt, &value);
        driver.DetachDispatch();
        if (i == 0) *pValue = value;
        if (*pValue != value) *pValue = indeterminate;
        bResult = TRUE;
    }
    return bResult;
}

template <typename T>
int SetPropScalar(COlePropertyPage* pThis, const wchar_t* pszPropName, VARTYPE vt, T value) {
    if (!pThis) return FALSE;
    S_COlePropertyPageTail* t = Tail(pThis);
    int bResult = FALSE;
    for (ULONG i = 0; i < t->m_nObjects; ++i) {
        LPDISPATCH pDisp = t->m_ppDisp[i];
        DISPID dispid = 0;
        if (!LookupDispID(pDisp, pszPropName, &dispid)) continue;
        COleDispatchDriver driver;
        driver.AttachDispatch(pDisp, FALSE);
        impl__SetProperty_COleDispatchDriver__QEAAXJGZZ(&driver, dispid, vt, value);
        driver.DetachDispatch();
        bResult = TRUE;
    }
    return bResult;
}

// ---------------------------------------------------------------------------
// XPropertyPage (IPropertyPage2) -- vtable 0x324928, 15 slots, dumped out of
// mfc140u .rdata:
//     0 0x1e9ba0 QueryInterface   5 0x1e9e70 Deactivate  10 0x1ea100 IsPageDirty
//     1 0x1e9b60 AddRef           6 0x1e9ec0 GetPageInfo 11 0x1ea110 Apply
//     2 0x1e9b70 Release          7 0x1e9f20 SetObjects  12 0x1ea1d0 Help
//     3 0x1e9bd0 SetPageSite      8 0x1ea080 Show        13 0x1ea270 TranslateAccelerator
//     4 0x1e9c60 Activate         9 0x1ea0c0 Move        14 0x1ea500 EditProperty
// (the qword after slot 14, 0x3249a0, is the next object's RTTI locator).
// Slots 0-2 are not exported; each is `add $-0x1f0,%rcx` followed by the body
// of CCmdTarget::External{QueryInterface,AddRef,Release}:
//     QI      0x1e9ba0: m_pOuterUnknown (+0x10) ? ->QueryInterface : InternalQueryInterface (0x26cfe0)
//     AddRef  0x1e9b60: jmp ExternalAddRef (0x26cd80)
//     Release 0x1e9b70: m_pOuterUnknown ? ->Release : InternalRelease (0x26cdb0)
// which is exactly what the three External* thunks do, so they are called.
// ---------------------------------------------------------------------------
unsigned long MS_ABI XPropertyPage_QueryInterface(void* pSub, const void* iid, void** ppv) {
    return impl__ExternalQueryInterface_CCmdTarget__QEAAKPEBXPEAPEAX_Z(OuterFromPropPage(pSub), iid, ppv);
}
unsigned long MS_ABI XPropertyPage_AddRef(void* pSub) {
    return impl__ExternalAddRef_CCmdTarget__QEAAKXZ(OuterFromPropPage(pSub));
}
unsigned long MS_ABI XPropertyPage_Release(void* pSub) {
    return impl__ExternalRelease_CCmdTarget__QEAAKXZ(OuterFromPropPage(pSub));
}

const void* const g_xPropertyPage_vtbl[15] = {
    reinterpret_cast<const void*>(&XPropertyPage_QueryInterface),
    reinterpret_cast<const void*>(&XPropertyPage_AddRef),
    reinterpret_cast<const void*>(&XPropertyPage_Release),
    reinterpret_cast<const void*>(&impl__SetPageSite_XPropertyPage_COlePropertyPage__UEAAJPEAUIPropertyPageSite___Z),
    reinterpret_cast<const void*>(&impl__Activate_XPropertyPage_COlePropertyPage__UEAAJPEAUHWND____PEBUtagRECT__H_Z),
    reinterpret_cast<const void*>(&impl__Deactivate_XPropertyPage_COlePropertyPage__UEAAJXZ),
    reinterpret_cast<const void*>(&impl__GetPageInfo_XPropertyPage_COlePropertyPage__UEAAJPEAUtagPROPPAGEINFO___Z),
    reinterpret_cast<const void*>(&impl__SetObjects_XPropertyPage_COlePropertyPage__UEAAJKPEAPEAUIUnknown___Z),
    reinterpret_cast<const void*>(&impl__Show_XPropertyPage_COlePropertyPage__UEAAJI_Z),
    reinterpret_cast<const void*>(&impl__Move_XPropertyPage_COlePropertyPage__UEAAJPEBUtagRECT___Z),
    reinterpret_cast<const void*>(&impl__IsPageDirty_XPropertyPage_COlePropertyPage__UEAAJXZ),
    reinterpret_cast<const void*>(&impl__Apply_XPropertyPage_COlePropertyPage__UEAAJXZ),
    reinterpret_cast<const void*>(&impl__Help_XPropertyPage_COlePropertyPage__UEAAJPEB_W_Z),
    reinterpret_cast<const void*>(&impl__TranslateAcceleratorW_XPropertyPage_COlePropertyPage__UEAAJPEAUtagMSG___Z),
    reinterpret_cast<const void*>(&impl__EditProperty_XPropertyPage_COlePropertyPage__UEAAJJ_Z),
};

// ---------------------------------------------------------------------------
// XPropNotifySink (IPropertyNotifySink) -- vtable 0x3248f8, 5 slots:
//     0 0x1ea560 QueryInterface   3 0x1ea5d0 OnChanged
//     1 0x003a60 AddRef           4 0x0071e0 OnRequestEdit
//     2 0x0071e0 Release
// QueryInterface (0x1ea560), in full: two 16-byte memcmp (IAT 0x2c73e0)
// against IID_IPropertyNotifySink (0x2d9d28) and IID_IUnknown (0x2d9a58);
// on either match `*ppv = this; return S_OK` -- with NO AddRef -- otherwise
// `*ppv = NULL; return E_NOINTERFACE (0x80004002)`. AddRef is the shared
// `mov $1,%eax; ret` at 0x3a60 and Release the shared `xor %eax,%eax; ret`
// at 0x71e0: the sink is owned by the page and is deliberately not
// reference-counted, which is why QueryInterface does not AddRef either.
// ---------------------------------------------------------------------------
unsigned long MS_ABI XPropNotifySink_QueryInterface(void* pSub, const void* iid, void** ppv) {
    if (::memcmp(iid, &kIID_IPropertyNotifySink, sizeof(GUID)) == 0 ||
        ::memcmp(iid, &kIID_IUnknown, sizeof(GUID)) == 0) {
        *ppv = pSub;
        return S_OK;
    }
    *ppv = nullptr;
    return 0x80004002UL; // E_NOINTERFACE
}
unsigned long MS_ABI XPropNotifySink_AddRef(void* /*pSub*/)  { return 1; }
unsigned long MS_ABI XPropNotifySink_Release(void* /*pSub*/) { return 0; }

const void* const g_xPropNotifySink_vtbl[5] = {
    reinterpret_cast<const void*>(&XPropNotifySink_QueryInterface),
    reinterpret_cast<const void*>(&XPropNotifySink_AddRef),
    reinterpret_cast<const void*>(&XPropNotifySink_Release),
    reinterpret_cast<const void*>(&impl__OnChanged_XPropNotifySink_COlePropertyPage__UEAAJJ_Z),
    reinterpret_cast<const void*>(&impl__OnRequestEdit_XPropNotifySink_COlePropertyPage__UEAAJJ_Z),
};

// Shared by both constructors. Retail (0x1e8ee0), for the 0x160..0x200
// window: m_idCaption = nIDCaption; every pointer/count zero; m_bInitializing
// = 1 (`movq $1,0x1b0`); the CDWordArray vptr at 0x1c0 and its four zeroed
// fields; the two sub-object vptrs at 0x1f0 / 0x1f8; then
// `lock incl 0x60(AfxGetModuleState())` -- the inlined AfxOleLockApp. The
// memset in the constructors zeroes the whole window first, so only the
// non-zero slots are written here. The CDWordArray vptr is left NULL (see the
// tail struct). AfxOleLockApp is OpenMFC's impl__AfxOleLockApp (detail/
// OlecoreSupport.cpp), whose counting mechanism is its own.
void InitTail(COlePropertyPage* pThis, UINT nIDCaption) {
    S_COlePropertyPageTail* t = Tail(pThis);
    t->m_idCaption = nIDCaption;
    t->m_bInitializing = TRUE;
    t->m_xPropertyPage_vfptr = g_xPropertyPage_vtbl;
    t->m_xPropNotifySink_vfptr = g_xPropNotifySink_vtbl;
    impl__AfxOleLockApp();
}

} // namespace

COlePropertyPage::COlePropertyPage()
    : CDialog(), m_pPageSite(nullptr), m_bModified(FALSE),
      m_pszPageName(nullptr), m_pszHelpFile(nullptr),
      m_dwHelpContext(0), m_pszDocString(nullptr) {
    memset(_olepropertypage_padding, 0, sizeof(_olepropertypage_padding));
    InitTail(this, 0);
}
COlePropertyPage::COlePropertyPage(UINT nIDTemplate, UINT nIDCaption)
    : CDialog(nIDTemplate), m_pPageSite(nullptr), m_bModified(FALSE),
      m_pszPageName(nullptr), m_pszHelpFile(nullptr),
      m_dwHelpContext(0), m_pszDocString(nullptr) {
    memset(_olepropertypage_padding, 0, sizeof(_olepropertypage_padding));
    InitTail(this, nIDCaption);
}
// Retail ??1COlePropertyPage@@UEAA@XZ (0x1e96b0), in order:
//     if (m_hDialog) ::GlobalFree(m_hDialog);            ; IAT 0x2c66d0
//     if (m_pStatus) { free(m_pStatus); m_pStatus = 0; } ; IAT 0x2c74e8
//     <delete every entry of m_arrayDDP, then SetSize(0)> ; 0x1e8e64 -- the
//                                          ; DDP array is not modelled here
//     _AfxRelease(&m_pPageSite);                          ; 0x26ccc4
//     CleanupObjectArray();                               ; 0x1e95e0
//     if (--AfxGetModuleState()->m_nObjectCount == 0)     ; inlined
//         AfxOleOnReleaseAllObjects();                    ;  AfxOleUnlockApp
//     <CDWordArray dtor: free(m_IDArray.m_pData)>
//     <three CString dtors, CPtrArray dtor>
//     CDialog::~CDialog                                   ; tail jmp 0x207eb0
COlePropertyPage::~COlePropertyPage() {
    S_COlePropertyPageTail* t = Tail(this);
    if (t->m_hDialog != nullptr) {
        ::GlobalFree(t->m_hDialog);
        t->m_hDialog = nullptr;
    }
    if (t->m_pStatus != nullptr) {
        ::free(t->m_pStatus);
        t->m_pStatus = nullptr;
    }
    if (m_pPageSite != nullptr) {
        m_pPageSite->Release();
        m_pPageSite = nullptr;
    }
    impl__CleanupObjectArray_COlePropertyPage__IEAAXXZ(this);
    impl__AfxOleUnlockApp();
    if (t->m_IDArray_pData != nullptr) {
        ::free(t->m_IDArray_pData);
        t->m_IDArray_pData = nullptr;
    }
    t->m_IDArray_nSize = 0;
    t->m_IDArray_nMaxSize = 0;
}
void COlePropertyPage::SetPageName(const wchar_t* pszName) {
    m_pszPageName = (wchar_t*)pszName;
}
void COlePropertyPage::SetHelpInfo(const wchar_t* lpszDocString, const wchar_t* lpszHelpFile, DWORD dwHelpContext) {
    m_pszDocString = (wchar_t*)lpszDocString;
    m_pszHelpFile = (wchar_t*)lpszHelpFile;
    m_dwHelpContext = dwHelpContext;
}
// Retail ?OnSetPageSite@COlePropertyPage@@UEAAXXZ is ordinal 11118 -> RVA
// 0x1e9050 and returns void: it loads the caption string for m_idCaption
// (falling back to string 0xfe01), calls SetPageName on it, zeroes
// m_sizePage and measures the dialog template MAKEINTRESOURCE(m_idDialog)
// through CDialogTemplate::Load / HasFont / GetFont / GetSizeInPixels. Not
// on this file's assignment list and not implemented; the declaration in
// afxole.h returns BOOL, which is itself a mismatch to resolve
// (headerRequest). The thunk in core/ole/Thunks.cpp calls this body.
BOOL COlePropertyPage::OnSetPageSite() {
    return TRUE;
}
// Retail ?OnObjectsChanged@COlePropertyPage@@UEAAXXZ is ordinal 10695 -> RVA
// 0x27d0, a bare `ret`: the base implementation does nothing. (0x27d0 is an
// identical-code-folded COMDAT shared by many void exports; the folding is
// by identical bytes, so "empty" is exact.) This body used to call
// SetModifiedFlag(TRUE), which drove IPropertyPageSite::OnStatusChange at a
// point where retail is silent and, now that XPropertyPage::SetObjects below
// calls this virtual on every SetObjects as retail does, would have marked
// the page dirty merely for being handed its objects. Corrected to the
// retail no-op.
void COlePropertyPage::OnObjectsChanged() {
}
BOOL COlePropertyPage::OnApply() {
    return TRUE;
}
// ?OnEditProperty@COlePropertyPage@@UEAAHJ@Z is ordinal 9712 -> RVA 0x71e0,
// the shared `xor %eax,%eax; ret`: FALSE unconditionally.
BOOL COlePropertyPage::OnEditProperty(DISPID dispid) {
    (void)dispid;
    return FALSE;
}
// COlePropertyPage::SetModifiedFlag(BOOL) -- retail 0x1e9970 (the target of
// the `call 0x1801e9970` in Activate, Apply and SetObjects), in full:
//     if (!bModified) m_bPropsChanged /*0x1a8*/ = 0;
//     if ((m_bDirty /*0x158*/ != 0) == (bModified != 0)) return;  ; no edge
//     m_bDirty = bModified;                                ; stored raw
//     if (m_pPageSite /*0x190*/)
//         m_pPageSite->OnStatusChange(bModified ? PROPPAGESTATUS_DIRTY : 0);
//                                                          ; slot 3, `setne`
// Edge-triggered: the site is told only when the flag actually changes.
// This body used to notify on every call and had no m_bPropsChanged to
// clear; both are now as retail, using m_bModified for m_bDirty.
void COlePropertyPage::SetModifiedFlag(BOOL bModified) {
    S_COlePropertyPageTail* t = Tail(this);
    if (!bModified) {
        t->m_bPropsChanged = FALSE;
    }
    if ((m_bModified != 0) == (bModified != 0)) {
        return;
    }
    m_bModified = bModified;
    if (m_pPageSite != nullptr) {
        m_pPageSite->OnStatusChange(bModified ? PROPPAGESTATUS_DIRTY : 0);
    }
}

// COlePropertyPage::CleanupObjectArray() -- retail 0x1e95e0:
//     if (m_pAdvisors /*0x1a0*/) {
//         for (i = 0; i < m_nObjects /*0x1ac*/; ++i)        ; unsigned
//             AfxConnectionUnadvise(m_ppDisp[i], IID_IPropertyNotifySink,
//                                   &m_xPropNotifySink /*this+0x1f8*/,
//                                   FALSE, m_pAdvisors[i]);  ; 0x1df360
//         free(m_pAdvisors); m_pAdvisors = NULL;              ; IAT 0x2c74e8
//     }
//     if (m_ppDisp /*0x198*/) {
//         for (i = 0; i < m_nObjects; ++i) _AfxRelease(&m_ppDisp[i]); ; 0x26ccc4
//         free(m_ppDisp); m_ppDisp = NULL;
//     }
// m_nObjects is NOT reset; SetObjects overwrites it afterwards.
// Symbol: ?CleanupObjectArray@COlePropertyPage@@IEAAXXZ
extern "C" void MS_ABI impl__CleanupObjectArray_COlePropertyPage__IEAAXXZ(
    COlePropertyPage* pThis) {
    if (!pThis) return;
    S_COlePropertyPageTail* t = Tail(pThis);
    if (t->m_pAdvisors != nullptr) {
        for (ULONG i = 0; i < t->m_nObjects; ++i) {
            impl__AfxConnectionUnadvise__YAHPEAUIUnknown__AEBU_GUID__0HK_Z(
                t->m_ppDisp[i], &kIID_IPropertyNotifySink, NotifySinkOf(pThis),
                FALSE, t->m_pAdvisors[i]);
        }
        ::free(t->m_pAdvisors);
        t->m_pAdvisors = nullptr;
    }
    if (t->m_ppDisp != nullptr) {
        for (ULONG i = 0; i < t->m_nObjects; ++i) {
            if (t->m_ppDisp[i] != nullptr) {
                t->m_ppDisp[i]->Release();
                t->m_ppDisp[i] = nullptr;
            }
        }
        ::free(t->m_ppDisp);
        t->m_ppDisp = nullptr;
    }
}

// COlePropertyPage::EnumChildProc(HWND, LPARAM) -- retail 0x1e9e00, the
// counting pass of XPropertyPage::Activate's two ::EnumChildWindows calls:
//     if (lParam == 0) AfxThrowInvalidArgException();   ; 0x227720
//     ++((COlePropertyPage*)lParam)->m_nControls;       ; 0x1b4
//     return TRUE;
// hWnd is not read.
// Symbol: ?EnumChildProc@COlePropertyPage@@KAHPEAUHWND__@@_J@Z
extern "C" int MS_ABI impl__EnumChildProc_COlePropertyPage__KAHPEAUHWND_____J_Z(HWND /*hWnd*/, __int64 lParam) {
    if (lParam == 0) {
        impl__AfxThrowInvalidArgException__YAXXZ();
    }
    COlePropertyPage* pPage = reinterpret_cast<COlePropertyPage*>(lParam);
    ++Tail(pPage)->m_nControls;
    return TRUE;
}

// COlePropertyPage::EnumControls(HWND, LPARAM) -- retail 0x1e9e20, the
// filling pass (no lParam test at all in this one):
//     COlePropertyPage* p = (COlePropertyPage*)lParam;
//     p->m_pStatus[p->m_nControls].nID    = ::GetDlgCtrlID(hWnd);  ; IAT 0x2c72a8
//     p->m_pStatus[p->m_nControls].bDirty = FALSE;
//     ++p->m_nControls;
//     return TRUE;
// Symbol: ?EnumControls@COlePropertyPage@@KAHPEAUHWND__@@_J@Z
extern "C" int MS_ABI impl__EnumControls_COlePropertyPage__KAHPEAUHWND_____J_Z(HWND hWnd, __int64 lParam) {
    COlePropertyPage* pPage = reinterpret_cast<COlePropertyPage*>(lParam);
    S_COlePropertyPageTail* t = Tail(pPage);
    t->m_pStatus[t->m_nControls].nID = static_cast<UINT>(::GetDlgCtrlID(hWnd));
    t->m_pStatus[t->m_nControls].bDirty = FALSE;
    ++t->m_nControls;
    return TRUE;
}

// COlePropertyPage::GetControlStatus(UINT nID) -- retail 0x1ea8e0:
//     for (i = 0; i < m_nControls /*0x1b4*/; ++i)      ; signed compare
//         if (m_pStatus /*0x1b8*/ [i].nID == nID)
//             return m_pStatus[i].bDirty;
//     return TRUE;                                     ; not found => TRUE
// A control the page is not tracking is treated as one whose value must be
// written back.
// Symbol: ?GetControlStatus@COlePropertyPage@@QEAAHI@Z
extern "C" int MS_ABI impl__GetControlStatus_COlePropertyPage__QEAAHI_Z(
    COlePropertyPage* pThis, unsigned int nID) {
    if (!pThis) return TRUE;
    S_COlePropertyPageTail* t = Tail(pThis);
    for (int i = 0; i < t->m_nControls; ++i) {
        if (t->m_pStatus[i].nID == nID) {
            return t->m_pStatus[i].bDirty;
        }
    }
    return TRUE;
}

// COlePropertyPage::GetObjectArray(ULONG* pnObjects) -- retail 0x1e9950,
// six instructions:
//     if (pnObjects) *pnObjects = m_nObjects;   ; 0x1ac, a DWORD store
//     return m_ppDisp;                          ; 0x198
// Symbol: ?GetObjectArray@COlePropertyPage@@QEAAPEAPEAUIDispatch@@PEAK@Z
extern "C" void* MS_ABI impl__GetObjectArray_COlePropertyPage__QEAAPEAPEAUIDispatch__PEAK_Z(
    COlePropertyPage* pThis, unsigned long* pnObjects) {
    if (!pThis) {
        if (pnObjects != nullptr) *pnObjects = 0;
        return nullptr;
    }
    S_COlePropertyPageTail* t = Tail(pThis);
    if (pnObjects != nullptr) {
        *pnObjects = t->m_nObjects;
    }
    return t->m_ppDisp;
}

// COlePropertyPage::GetPageSite() -- retail 0x1e9af0 is two instructions:
//     mov 0x190(%rcx),%rax ; ret
// That member is m_pPageSite in the OpenMFC declaration.
// Symbol: ?GetPageSite@COlePropertyPage@@QEAAPEAUIPropertyPageSite@@XZ
extern "C" void* MS_ABI impl__GetPageSite_COlePropertyPage__QEAAPEAUIPropertyPageSite__XZ(
    COlePropertyPage* pThis) {
    return pThis ? pThis->m_pPageSite : nullptr;
}

// ---------------------------------------------------------------------------
// GetPropCheck / GetPropIndex / GetPropRadio / GetPropText (9 overloads).
// Shape: see GetPropScalar above. Per body, the VARTYPE handed to
// InvokeHelper(dispid, DISPATCH_PROPERTYGET, vt, &value, NULL) (0x252660),
// the merge, and the "objects disagree" value written on a mismatch:
//     GetPropCheck  0x1ebf90  VT_BOOL  b = (value != 0); mismatch -> 2
//                                      (BST_INDETERMINATE; `lea -0x9(%r9)`)
//     GetPropRadio  0x1ec240  VT_I2    *p = (int)(short)v; mismatch -> -1
//     GetPropIndex  0x1ec3c0  a single `jmp 0x1ec240`: an alias of GetPropRadio
//     GetPropText   BYTE&  0x1eaa80 VT_UI1  short& 0x1ead30 VT_I2
//                   int&   0x1eafe0 VT_I4   UINT&  0x1eb150 VT_I4
//                   long&  0x1eb2c0 VT_I4   DWORD& 0x1eb430 VT_I4
//                   float& 0x1eb6e0 VT_R4   double&0x1eb9a0 VT_R8
//                   CString* 0x1ebc60 VT_BSTR
// The GetPropText mismatch value is loaded from a per-type global --
// double 0x3c3f88, float 0x3c3f90, DWORD 0x3c3f94, long 0x3c3f98,
// UINT 0x3c3f9c, int 0x3c3fa0, short 0x3c3fa4, BYTE 0x3c3fa6 -- and every
// one of those sits in .bss and has exactly one reference in the whole image
// (its read in the body above; grepped the full mfc140u disassembly), so
// each is a never-written zero. The CString overload's is a function-static
// CString built once from the empty string at 0x34b94a (guard word 0x3c4148,
// object 0x3c4150): an empty string.
// ---------------------------------------------------------------------------

// Symbol: ?GetPropCheck@COlePropertyPage@@QEAAHPEB_WPEAH@Z
extern "C" int MS_ABI impl__GetPropCheck_COlePropertyPage__QEAAHPEB_WPEAH_Z(
    COlePropertyPage* pThis, const wchar_t* pszPropName, int* pValue) {
    // Retail: `xor %eax,%eax; cmp %eax,value; setne %al` on the BOOL
    // InvokeHelper wrote, then the merge with 2 as the mismatch value.
    // (OpenMFC's InvokeHelper stores VT_BOOL as `boolVal == VARIANT_TRUE`
    // where retail stores `boolVal != 0`; a non-canonical VARIANT_BOOL
    // therefore reads as unchecked here. That is afxdisp.h's, not this
    // body's.)
    if (!pThis || !pValue) return FALSE;
    S_COlePropertyPageTail* t = Tail(pThis);
    int bResult = FALSE;
    for (ULONG i = 0; i < t->m_nObjects; ++i) {
        LPDISPATCH pDisp = t->m_ppDisp[i];
        DISPID dispid = 0;
        if (!LookupDispID(pDisp, pszPropName, &dispid)) continue;
        BOOL bValue = FALSE;
        COleDispatchDriver driver;
        driver.AttachDispatch(pDisp, FALSE);
        impl__GetProperty_COleDispatchDriver__QEBAXJGPEAX_Z(&driver, dispid, VT_BOOL, &bValue);
        driver.DetachDispatch();
        int nCheck = (bValue != 0) ? 1 : 0;
        if (i == 0) *pValue = nCheck;
        if (*pValue != nCheck) *pValue = 2;   // BST_INDETERMINATE
        bResult = TRUE;
    }
    return bResult;
}

// Symbol: ?GetPropIndex@COlePropertyPage@@QEAAHPEB_WPEAH@Z
extern "C" int MS_ABI impl__GetPropIndex_COlePropertyPage__QEAAHPEB_WPEAH_Z(
    COlePropertyPage* pThis, const wchar_t* pszPropName, int* pValue) {
    // Retail 0x1ec3c0 is `jmp 0x1801ec240` -- GetPropRadio's body.
    return impl__GetPropRadio_COlePropertyPage__QEAAHPEB_WPEAH_Z(pThis, pszPropName, pValue);
}

// Symbol: ?GetPropRadio@COlePropertyPage@@QEAAHPEB_WPEAH@Z
extern "C" int MS_ABI impl__GetPropRadio_COlePropertyPage__QEAAHPEB_WPEAH_Z(
    COlePropertyPage* pThis, const wchar_t* pszPropName, int* pValue) {
    // Retail 0x1ec240: VT_I2 into a short, `movswl` into *pValue, -1 on a
    // mismatch (`movl $0xffffffff,(%rdi)`).
    if (!pThis || !pValue) return FALSE;
    S_COlePropertyPageTail* t = Tail(pThis);
    int bResult = FALSE;
    for (ULONG i = 0; i < t->m_nObjects; ++i) {
        LPDISPATCH pDisp = t->m_ppDisp[i];
        DISPID dispid = 0;
        if (!LookupDispID(pDisp, pszPropName, &dispid)) continue;
        short sValue = 0;
        COleDispatchDriver driver;
        driver.AttachDispatch(pDisp, FALSE);
        impl__GetProperty_COleDispatchDriver__QEBAXJGPEAX_Z(&driver, dispid, VT_I2, &sValue);
        driver.DetachDispatch();
        if (i == 0) *pValue = static_cast<int>(sValue);
        if (*pValue != static_cast<int>(sValue)) *pValue = -1;
        bResult = TRUE;
    }
    return bResult;
}

// Symbol: ?GetPropText@COlePropertyPage@@QEAAHPEB_WPEAE@Z
extern "C" int MS_ABI impl__GetPropText_COlePropertyPage__QEAAHPEB_WPEAE_Z(
    COlePropertyPage* pThis, const wchar_t* pszPropName, unsigned char* pValue) {
    return GetPropScalar<unsigned char>(pThis, pszPropName, pValue, VT_UI1, 0);   // 0x1eaa80
}

// Symbol: ?GetPropText@COlePropertyPage@@QEAAHPEB_WPEAF@Z
extern "C" int MS_ABI impl__GetPropText_COlePropertyPage__QEAAHPEB_WPEAF_Z(
    COlePropertyPage* pThis, const wchar_t* pszPropName, short* pValue) {
    return GetPropScalar<short>(pThis, pszPropName, pValue, VT_I2, 0);           // 0x1ead30
}

// Symbol: ?GetPropText@COlePropertyPage@@QEAAHPEB_WPEAH@Z
extern "C" int MS_ABI impl__GetPropText_COlePropertyPage__QEAAHPEB_WPEAH_Z(
    COlePropertyPage* pThis, const wchar_t* pszPropName, int* pValue) {
    return GetPropScalar<int>(pThis, pszPropName, pValue, VT_I4, 0);             // 0x1eafe0
}

// Symbol: ?GetPropText@COlePropertyPage@@QEAAHPEB_WPEAI@Z
extern "C" int MS_ABI impl__GetPropText_COlePropertyPage__QEAAHPEB_WPEAI_Z(
    COlePropertyPage* pThis, const wchar_t* pszPropName, unsigned int* pValue) {
    return GetPropScalar<unsigned int>(pThis, pszPropName, pValue, VT_I4, 0);    // 0x1eb150
}

// Symbol: ?GetPropText@COlePropertyPage@@QEAAHPEB_WPEAJ@Z
extern "C" int MS_ABI impl__GetPropText_COlePropertyPage__QEAAHPEB_WPEAJ_Z(
    COlePropertyPage* pThis, const wchar_t* pszPropName, long* pValue) {
    return GetPropScalar<long>(pThis, pszPropName, pValue, VT_I4, 0);            // 0x1eb2c0
}

// Symbol: ?GetPropText@COlePropertyPage@@QEAAHPEB_WPEAK@Z
extern "C" int MS_ABI impl__GetPropText_COlePropertyPage__QEAAHPEB_WPEAK_Z(
    COlePropertyPage* pThis, const wchar_t* pszPropName, unsigned long* pValue) {
    return GetPropScalar<unsigned long>(pThis, pszPropName, pValue, VT_I4, 0);   // 0x1eb430
}

// Symbol: ?GetPropText@COlePropertyPage@@QEAAHPEB_WPEAM@Z
extern "C" int MS_ABI impl__GetPropText_COlePropertyPage__QEAAHPEB_WPEAM_Z(
    COlePropertyPage* pThis, const wchar_t* pszPropName, float* pValue) {
    return GetPropScalar<float>(pThis, pszPropName, pValue, VT_R4, 0.0f);        // 0x1eb6e0
}

// Symbol: ?GetPropText@COlePropertyPage@@QEAAHPEB_WPEAN@Z
extern "C" int MS_ABI impl__GetPropText_COlePropertyPage__QEAAHPEB_WPEAN_Z(
    COlePropertyPage* pThis, const wchar_t* pszPropName, double* pValue) {
    return GetPropScalar<double>(pThis, pszPropName, pValue, VT_R8, 0.0);        // 0x1eb9a0
}

// Symbol: ?GetPropText@COlePropertyPage@@QEAAHPEB_WPEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@@Z
extern "C" int MS_ABI impl__GetPropText_COlePropertyPage__QEAAHPEB_WPEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
    // PEAV in the mangling: retail's third argument is CString* (a pointer),
    // not CString&.
    COlePropertyPage* pThis, const wchar_t* pszPropName, void* pStrValueRaw) {
    // Retail 0x1ebc60, per object after the dispid lookup:
    //     CString strTemp("");                                  ; 0x1cd550
    //     <once: static CString strIndeterminate("")>           ; 0x3c4150
    //     driver.AttachDispatch(p, FALSE);
    //     InvokeHelper(dispid, DISPATCH_PROPERTYGET, VT_BSTR, &strTemp, NULL);
    //     driver.m_lpDispatch = NULL;
    //     if (i == 0) *pStrValue = strTemp;                      ; 0xde30
    //     if (wcscmp(*pStrValue, strTemp) != 0)                  ; IAT 0x2c7770
    //         *pStrValue = strIndeterminate;
    //     bResult = TRUE;
    // Retail's InvokeHelper writes a VT_BSTR result straight into a CString;
    // OpenMFC's (afxdisp.h) hands back the raw BSTR, which is copied into a
    // CString and freed here -- same resulting string. The `je 0x1ebe3d` on
    // a NULL strTemp buffer is ATL's ATLENSURE in StringCompare (throws
    // E_FAIL through 0x333c); a CString buffer is never NULL, so it is not
    // reproduced.
    CString* pStrValue = static_cast<CString*>(pStrValueRaw);
    if (!pThis || !pStrValue) return FALSE;
    S_COlePropertyPageTail* t = Tail(pThis);
    int bResult = FALSE;
    for (ULONG i = 0; i < t->m_nObjects; ++i) {
        LPDISPATCH pDisp = t->m_ppDisp[i];
        DISPID dispid = 0;
        if (!LookupDispID(pDisp, pszPropName, &dispid)) continue;
        BSTR bstrTemp = nullptr;
        COleDispatchDriver driver;
        driver.AttachDispatch(pDisp, FALSE);
        impl__GetProperty_COleDispatchDriver__QEBAXJGPEAX_Z(&driver, dispid, VT_BSTR, &bstrTemp);
        driver.DetachDispatch();
        CString strTemp(bstrTemp ? bstrTemp : L"");
        if (bstrTemp) ::SysFreeString(bstrTemp);
        if (i == 0) *pStrValue = strTemp;
        if (::wcscmp(pStrValue->GetString(), strTemp.GetString()) != 0) {
            *pStrValue = CString(L"");
        }
        bResult = TRUE;
    }
    return bResult;
}

// COlePropertyPage::IgnoreApply(UINT nID) -- retail 0x1ea8c0 is three
// instructions and a tail jump:
//     add $0x1c0,%rcx ; mov %edx,%r8d ; mov 0x10(%rcx),%rdx ; jmp 0x1d4260
// i.e. CDWordArray::SetAtGrow(&m_IDArray, m_IDArray.m_nSize, nID) -- an
// inlined Add(): append nID to the list of control ids whose changes must
// not mark the page dirty (OnCommand reads m_pData/m_nSize at 0x1c8/0x1d0).
// The element storage is kept at the retail offsets (see the tail struct);
// the growth policy below is this file's own (double, minimum 4), not a
// transcription of CDWordArray::SetAtGrow -- only the resulting contents
// (nID appended at index m_nSize) are what retail observes.
// Symbol: ?IgnoreApply@COlePropertyPage@@QEAAXI@Z
extern "C" void MS_ABI impl__IgnoreApply_COlePropertyPage__QEAAXI_Z(
    COlePropertyPage* pThis, unsigned int nID) {
    if (!pThis) return;
    S_COlePropertyPageTail* t = Tail(pThis);
    if (t->m_IDArray_nSize >= t->m_IDArray_nMaxSize) {
        INT_PTR nNewMax = t->m_IDArray_nMaxSize < 4 ? 4 : t->m_IDArray_nMaxSize * 2;
        DWORD* pNew = static_cast<DWORD*>(::realloc(t->m_IDArray_pData, static_cast<size_t>(nNewMax) * sizeof(DWORD)));
        if (pNew == nullptr) return;
        t->m_IDArray_pData = pNew;
        t->m_IDArray_nMaxSize = nNewMax;
    }
    t->m_IDArray_pData[t->m_IDArray_nSize++] = nID;
}

// COlePropertyPage::IsModified() -- mfc140u ordinal 7954 -> RVA 0x1d02f0
// (the symbol is absent from both RVA maps; resolved through the mfc140u
// export table), in full:
//     mov 0x158(%rcx),%eax ; ret
// i.e. the dirty flag RAW, not normalised. OpenMFC's member for that role is
// m_bModified and the inline IsModified in afxole.h returns it raw too.
// Symbol: ?IsModified@COlePropertyPage@@QEAAHXZ
extern "C" int MS_ABI impl__IsModified_COlePropertyPage__QEAAHXZ(COlePropertyPage* pThis) {
    return pThis ? pThis->m_bModified : FALSE;
}

// COlePropertyPage::MessageBox(LPCTSTR, LPCTSTR, UINT) -- retail 0x1e9b00:
//     if (lpszCaption == NULL) lpszCaption = m_strPageName /*0x168*/;
//     hWnd = CWnd::GetSafeOwner_(m_hWnd /*0x40*/, NULL);
//     tail-jmp ::MessageBoxW(hWnd, lpszText, lpszCaption, nType)
// The OpenMFC member SetPageName writes is m_pszPageName. Deviation that
// follows from the substitution: retail's 0x168 is a CString, so an unset
// page name yields "" and the box gets an empty caption, whereas
// m_pszPageName is NULL until SetPageName is called, so ::MessageBoxW then
// supplies its default caption.
// Symbol: ?MessageBoxW@COlePropertyPage@@QEAAHPEB_W0I@Z
extern "C" int MS_ABI impl__MessageBoxW_COlePropertyPage__QEAAHPEB_W0I_Z(
    COlePropertyPage* pThis, const wchar_t* lpszText, const wchar_t* lpszCaption,
    unsigned int nType) {
    if (!pThis) return 0;
    if (lpszCaption == nullptr) {
        lpszCaption = pThis->m_pszPageName;
    }
    HWND hWnd = impl__GetSafeOwner__CWnd__SAPEAUHWND____PEAU2_PEAPEAU2__Z(pThis->m_hWnd, nullptr);
    return ::MessageBoxW(hWnd, lpszText, lpszCaption, nType);
}

// COlePropertyPage::OnCommand(WPARAM, LPARAM) -- retail 0x1ea640 (MBCS
// 0x1e83e0):
//     BOOL bRet = CWnd::OnCommand(wParam, lParam);      ; first
//     if (m_bInitializing /*0x1b0*/) return bRet;
//     if (m_ppDisp /*0x198*/ == NULL) return bRet;
//     if (lParam == 0 /*not a control notification*/) return bRet;
//     ... scan m_IDArray (m_pData 0x1c8, m_nSize 0x1d0) for LOWORD(wParam);
//         if found, return bRet. Otherwise classify the control by class
//         name (::GetClassName into a 0x64-character buffer, CompareString
//         against two 8-entry {class, notification} tables, and "button"
//         with the WS_GROUP / GW_HWNDPREV radio-group walk) and by
//         notification code, then set the dirty flag and call
//         SetControlStatus(nID, ...) and m_pPageSite->OnStatusChange.
//     return bRet;                                      ; always the base's
// Only the base dispatch is implemented here; the dirty-tracking tail is
// not on this file's assignment list and is left for its owner. The members
// it needs (m_bInitializing, m_ppDisp, m_IDArray) are now modelled above.
// Symbol: ?OnCommand@COlePropertyPage@@MEAAH_K_J@Z
extern "C" int MS_ABI impl__OnCommand_COlePropertyPage__MEAAH_K_J_Z(
    COlePropertyPage* pThis, unsigned __int64 wParam, __int64 lParam) {
    if (!pThis) return FALSE;
    return impl__OnCommand_CWnd__MEAAH_K_J_Z(static_cast<CWnd*>(pThis),
                                             static_cast<WPARAM>(wParam),
                                             static_cast<LPARAM>(lParam));
}

// COlePropertyPage::OnCtlColor(CDC*, CWnd*, UINT) -- retail 0x1e98e0:
//     if (pWnd == NULL) AfxThrowInvalidArgException()
//     if (pWnd->SendChildNotifyLastMsg(&lResult)) return (HBRUSH)lResult
//     pMsg = CWnd::GetCurrentMessage()
//     lResult = ::SendMessage(::GetParent(m_hWnd), pMsg->message,
//                             pMsg->wParam, pMsg->lParam)
//     if (lResult == 0) lResult = this->Default()
//     return (HBRUSH)lResult
// The CDC* and the UINT code are not read by the retail body at all: the
// unhandled colour message is reflected to the child and then forwarded
// verbatim to the property frame that owns the page.
// Two deliberate deviations, both defensive: retail THROWS on pWnd == NULL
// where this returns NULL, and retail never null-checks
// CWnd::GetCurrentMessage()'s result where this does.
// Symbol: ?OnCtlColor@COlePropertyPage@@IEAAPEAUHBRUSH__@@PEAVCDC@@PEAVCWnd@@I@Z
extern "C" void* MS_ABI impl__OnCtlColor_COlePropertyPage__IEAAPEAUHBRUSH____PEAVCDC__PEAVCWnd__I_Z(
    COlePropertyPage* pThis, CDC* /*pDC*/, CWnd* pWnd, unsigned int /*nCtlColor*/) {
    if (!pThis || !pWnd) return nullptr;

    LONGLONG lResult = 0;
    if (impl__SendChildNotifyLastMsg_CWnd__QEAAHPEA_J_Z(pWnd, &lResult)) {
        return reinterpret_cast<void*>(static_cast<LONG_PTR>(lResult));
    }

    const MSG* pMsg = impl__GetCurrentMessage_CWnd__KAPEBUtagMSG__XZ();
    LRESULT lr = 0;
    if (pMsg != nullptr) {
        lr = ::SendMessageW(::GetParent(pThis->m_hWnd), pMsg->message,
                            pMsg->wParam, pMsg->lParam);
    }
    if (lr == 0) {
        lr = static_cast<LRESULT>(impl__Default_CWnd__IEAA_JXZ(static_cast<CWnd*>(pThis)));
    }
    return reinterpret_cast<void*>(lr);
}

// COlePropertyPage::OnFinalRelease() -- retail 0x1e97f0:
//     if (m_hWnd /*0x40*/ != 0) this->vtbl[0xd0/8]()   ; CWnd::DestroyWindow
//     tail-jmp this->vtbl[1](this, 1)                  ; deleting destructor
// Deviation: retail dispatches DestroyWindow VIRTUALLY; the thunk called
// here is CWnd::DestroyWindow specifically, so an application override on a
// derived page would be bypassed. That is a property of this DLL's impl__
// thunk model, not of this function.
// Symbol: ?OnFinalRelease@COlePropertyPage@@MEAAXXZ
extern "C" void MS_ABI impl__OnFinalRelease_COlePropertyPage__MEAAXXZ(COlePropertyPage* pThis) {
    if (!pThis) return;
    if (pThis->m_hWnd != nullptr) {
        impl__DestroyWindow_CWnd__UEAAHXZ(static_cast<CWnd*>(pThis));
    }
    delete pThis;
}

// COlePropertyPage::OnHelp(LPCTSTR) -- ?OnHelp@COlePropertyPage@@UEAAHPEB_W@Z
// is mfc140u ordinal 10068 -> RVA 0x71e0, and the body at 0x71e0 is, in full:
//     xor %eax,%eax ; ret
// i.e. `return FALSE;` unconditionally: it never touches `this`, never reads
// the help directory, never opens a help file. The base page declines help
// and leaves it to a derived page to override. (0x71e0 is an identical-code-
// folded COMDAT shared by every trivial `return 0` export in the image; the
// export directory is what ties this ordinal to it.) XPropertyPage::Help
// below maps the FALSE to S_FALSE. m_pszHelpFile / m_dwHelpContext are only
// read by XPropertyPage::GetPageInfo.
// Symbol: ?OnHelp@COlePropertyPage@@UEAAHPEB_W@Z
extern "C" int MS_ABI impl__OnHelp_COlePropertyPage__UEAAHPEB_W_Z(
    COlePropertyPage* /*pThis*/, const wchar_t* /*lpszHelpDir*/) {
    return FALSE;
}

// COlePropertyPage::OnInitDialog() -- retail 0x1e9460 is four instructions:
//     call ?OnInitDialog@CDialog@@UEAAHXZ ; xor %eax,%eax ; ret
// It calls the CDialog base and then returns FALSE (the page must not steal
// the focus from the property frame), discarding the base's return value.
// Symbol: ?OnInitDialog@COlePropertyPage@@UEAAHXZ
extern "C" int MS_ABI impl__OnInitDialog_COlePropertyPage__UEAAHXZ(COlePropertyPage* pThis) {
    if (!pThis) return FALSE;
    (void)impl__OnInitDialog_CDialog__UEAAHXZ(static_cast<CDialog*>(pThis));
    return FALSE;
}

// COlePropertyPage::PreTranslateMessage(MSG*) -- retail 0x1e9470:
//     if (pMsg == NULL) AfxThrowInvalidArgException()
//     if (pMsg->message == 0x100 /*WM_KEYDOWN*/) {
//         if (pMsg->wParam != VK_RETURN && pMsg->wParam != VK_ESCAPE)
//             goto base;
//         pWnd = CWnd::FromHandle(::GetFocus());
//         if (pMsg->wParam == VK_RETURN && pWnd != NULL &&
//             ::IsChild(m_hWnd, pWnd->m_hWnd) &&
//             (pWnd->GetStyle() & 0x1000 /*ES_WANTRETURN*/) &&
//             ::GetClassName(pWnd->m_hWnd, szClass, 10) &&
//             ::CompareString(0x7f /*LOCALE_INVARIANT*/, NORM_IGNORECASE,
//                             szClass, -1, "EDIT", -1) == CSTR_EQUAL) {
//             ::SendMessage(pWnd->m_hWnd, 0x102 /*WM_CHAR*/,
//                           pMsg->wParam, pMsg->lParam);
//             return TRUE;
//         }
//         return FALSE;      ; Return/Escape are never translated by the page
//     }
//     if (pMsg->message == 0x104 /*WM_SYSKEYDOWN*/ &&
//         !::IsChild(m_hWnd, pMsg->hwnd)) {
//         hSave = pMsg->hwnd;
//         pMsg->hwnd = ::GetWindow(m_hWnd, 5 /*GW_CHILD*/);
//         b = CDialog::PreTranslateMessage(pMsg);
//         pMsg->hwnd = hSave;
//         return b;
//     }
//  base:
//     return CDialog::PreTranslateMessage(pMsg)
// Retail calls CWnd::FromHandle(::GetFocus()) twice on the Return/Escape path
// (once before the VK_RETURN retest); only one call is kept here -- the
// second is a pure repeat with no additional effect beyond the handle-map
// lookup the first already performed.
// Deviation: retail throws AfxThrowInvalidArgException on a NULL pMsg; this
// returns FALSE instead.
// Symbol: ?PreTranslateMessage@COlePropertyPage@@MEAAHPEAUtagMSG@@@Z
extern "C" int MS_ABI impl__PreTranslateMessage_COlePropertyPage__MEAAHPEAUtagMSG___Z(
    COlePropertyPage* pThis, MSG* pMsg) {
    if (!pThis || !pMsg) return FALSE;

    if (pMsg->message == WM_KEYDOWN) {
        if (pMsg->wParam != VK_RETURN && pMsg->wParam != VK_ESCAPE) {
            return impl__PreTranslateMessage_CDialog__UEAAHPEAUtagMSG___Z(
                static_cast<CDialog*>(pThis), pMsg);
        }
        CWnd* pWndFocus = impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(::GetFocus());
        if (pMsg->wParam == VK_RETURN && pWndFocus != nullptr &&
            ::IsChild(pThis->m_hWnd, pWndFocus->m_hWnd) &&
            (impl__GetStyle_CWnd__QEBAKXZ(pWndFocus) & ES_WANTRETURN) != 0) {
            wchar_t szClass[10];
            if (::GetClassNameW(pWndFocus->m_hWnd, szClass, 10) &&
                ::CompareStringW(LOCALE_INVARIANT, NORM_IGNORECASE,
                                 szClass, -1, L"EDIT", -1) == CSTR_EQUAL) {
                ::SendMessageW(pWndFocus->m_hWnd, WM_CHAR, pMsg->wParam, pMsg->lParam);
                return TRUE;
            }
        }
        return FALSE;
    }

    if (pMsg->message == WM_SYSKEYDOWN && !::IsChild(pThis->m_hWnd, pMsg->hwnd)) {
        HWND hSave = pMsg->hwnd;
        pMsg->hwnd = ::GetWindow(pThis->m_hWnd, GW_CHILD);
        int bResult = impl__PreTranslateMessage_CDialog__UEAAHPEAUtagMSG___Z(
            static_cast<CDialog*>(pThis), pMsg);
        pMsg->hwnd = hSave;
        return bResult;
    }

    return impl__PreTranslateMessage_CDialog__UEAAHPEAUtagMSG___Z(
        static_cast<CDialog*>(pThis), pMsg);
}

// COlePropertyPage::SetControlStatus(UINT nID, BOOL bDirty) -- retail
// 0x1ea910, the mirror of GetControlStatus:
//     for (i = 0; i < m_nControls /*0x1b4*/; ++i)
//         if (m_pStatus /*0x1b8*/ [i].nID == nID) {
//             m_pStatus[i].bDirty = bDirty;
//             return TRUE;
//         }
//     return FALSE;                                 ; not found => FALSE
// Note the asymmetry with GetControlStatus, whose not-found answer is TRUE.
// Symbol: ?SetControlStatus@COlePropertyPage@@QEAAHIH@Z
extern "C" int MS_ABI impl__SetControlStatus_COlePropertyPage__QEAAHIH_Z(
    COlePropertyPage* pThis, unsigned int nID, int bDirty) {
    if (!pThis) return FALSE;
    S_COlePropertyPageTail* t = Tail(pThis);
    for (int i = 0; i < t->m_nControls; ++i) {
        if (t->m_pStatus[i].nID == nID) {
            t->m_pStatus[i].bDirty = bDirty;
            return TRUE;
        }
    }
    return FALSE;
}

// COlePropertyPage::SetDialogResource(HGLOBAL hDialog) -- retail 0x1e9a10:
//     if (m_hDialog /*0x1e8*/) { ::GlobalFree(m_hDialog); m_hDialog = NULL; }
//     CDialogTemplate dt(hDialog);              ; ??0CDialogTemplate@@QEAA@PEAX@Z 0x219f00
//     dt.GetSizeInPixels(&m_sizePage /*0x170*/); ; 0x21a6c0
//     m_hDialog = dt.m_hTemplate;               ; dt's first qword; no
//                                               ; CDialogTemplate destructor
//                                               ; runs
// What dt.m_hTemplate IS decides the ownership, so the ctor was read too.
// Retail ??0CDialogTemplate@@QEAA@PEAX@Z (0x219f00): a NULL handle zeroes
// m_hTemplate / m_dwTemplateSize and returns; otherwise it GlobalLock's
// hDialog (IAT 0x2c6628), takes GetTemplateSize (0x21a1a0) of the locked
// bytes, calls SetTemplate (0x219f70) -- which GlobalAlloc(GMEM_ZEROINIT,
// size + 0x40)s (IAT 0x2c6620) a FRESH block, memcpy's the template into it
// and AfxThrowMemoryException's (0x2276c0) if the allocation fails -- and
// GlobalUnlock's hDialog (IAT 0x2c6630). So the page ends up owning a
// private GMEM_FIXED copy: the caller's hDialog is only read and remains
// the caller's to free, and ::LockResource(m_hDialog) in Activate works
// because a fixed HGLOBAL is its own pointer.
// OpenMFC's CDialogTemplate (core/dialog/CDialogTemplate.cpp) does none of
// that: its ctor thunk parses the argument as a raw DLGTEMPLATE* into a
// side table keyed by the object address (no GlobalLock, no copy),
// GetTemplateSize is a `return 0` placeholder, GetSizeInPixels an empty
// one-parameter placeholder, and its dtor thunk only drops the side-table
// entry. The copy retail's ctor makes is therefore made here directly, and
// the ctor / GetSizeInPixels / dtor sequence is kept -- on the locked
// bytes, which is also what retail's ctor parses -- so m_sizePage is filled
// the moment GetSizeInPixels is implemented (headerRequest); until then it
// stays 0 and nothing else in this file writes it.
// DEVIATION: the copy is sized by ::GlobalSize(hDialog) instead of
// CDialogTemplate::GetTemplateSize (the placeholder above; headerRequest).
// A real HGLOBAL -- which retail requires as well, since it GlobalLock's
// it -- is at least as large as the template it holds, and nothing reads
// the size back. DEVIATION (defensive): a handle that will not lock or size,
// or a failed allocation, leaves m_hDialog NULL where retail would fault or
// throw. (An earlier body stored hDialog itself and freed it in the
// destructor, i.e. took ownership of the caller's handle; that was wrong.)
// Symbol: ?SetDialogResource@COlePropertyPage@@QEAAXPEAX@Z
extern "C" void MS_ABI impl__SetDialogResource_COlePropertyPage__QEAAXPEAX_Z(
    COlePropertyPage* pThis, void* hDialog) {
    if (!pThis) return;
    S_COlePropertyPageTail* t = Tail(pThis);
    if (t->m_hDialog != nullptr) {
        ::GlobalFree(t->m_hDialog);
        t->m_hDialog = nullptr;
    }
    HGLOBAL hCopy = nullptr;
    const void* pTemplate = nullptr;
    if (hDialog != nullptr) {
        pTemplate = ::GlobalLock(static_cast<HGLOBAL>(hDialog));
        if (pTemplate != nullptr) {
            const SIZE_T cb = ::GlobalSize(static_cast<HGLOBAL>(hDialog));
            if (cb != 0 && cb + 0x40 > cb) {
                hCopy = ::GlobalAlloc(GMEM_ZEROINIT, cb + 0x40);   // GMEM_FIXED
                if (hCopy != nullptr) {
                    ::memcpy(hCopy, pTemplate, cb);
                }
            }
        }
    }
    alignas(8) unsigned char dt[16];
    impl___0CDialogTemplate__QEAA_PEAX_Z(dt, const_cast<void*>(pTemplate));
    impl__GetSizeInPixels_CDialogTemplate__QEBAXPEAUtagSIZE___Z(dt, &t->m_sizePage);
    impl___1CDialogTemplate__QEAA_XZ(dt);
    if (pTemplate != nullptr) {
        ::GlobalUnlock(static_cast<HGLOBAL>(hDialog));
    }
    t->m_hDialog = hCopy;
}

// ---------------------------------------------------------------------------
// SetPropCheck / SetPropIndex / SetPropRadio / SetPropText (9 overloads).
// Shape: see SetPropScalar above. Per body, the VARTYPE handed to
// COleDispatchDriver::SetProperty (0x2526c0) in R8D and how the value is
// loaded into R9:
//     SetPropCheck  0x1ebe50  VT_BOOL  r9d = (nValue == 1)   ; `cmp $1; sete`
//     SetPropRadio  0x1ec100  VT_I2    r9d = (short)nValue   ; `movswl %r15w`
//     SetPropIndex  0x1ec3b0  a single `jmp 0x1ec100`: an alias of SetPropRadio
//     SetPropText   BYTE&  0x1ea940 VT_UI1 `movzbl (%r15)`
//                   short& 0x1eabf0 VT_I2  `movswl (%r15)`
//                   int& / UINT& / long& / DWORD&  0x1eaea0 VT_I4 `mov (%r15),%r9d`
//                        -- ordinals 13505..13508 all export the one body
//                   float& 0x1eb5a0 VT_R4  `movss; cvtps2pd` (promoted double)
//                   double&0x1eb860 VT_R8  `movsd (%r15)`
//                   CString& 0x1ebb20 VT_BSTR `mov (%r15),%r9` (the buffer pointer)
// ---------------------------------------------------------------------------

// Symbol: ?SetPropCheck@COlePropertyPage@@QEAAHPEB_WH@Z
extern "C" int MS_ABI impl__SetPropCheck_COlePropertyPage__QEAAHPEB_WH_Z(
    COlePropertyPage* pThis, const wchar_t* pszPropName, int nValue) {
    return SetPropScalar<int>(pThis, pszPropName, VT_BOOL, (nValue == 1) ? 1 : 0);
}

// Symbol: ?SetPropIndex@COlePropertyPage@@QEAAHPEB_WH@Z
extern "C" int MS_ABI impl__SetPropIndex_COlePropertyPage__QEAAHPEB_WH_Z(
    COlePropertyPage* pThis, const wchar_t* pszPropName, int nValue) {
    // Retail 0x1ec3b0 is `jmp 0x1801ec100` -- SetPropRadio's body.
    return impl__SetPropRadio_COlePropertyPage__QEAAHPEB_WH_Z(pThis, pszPropName, nValue);
}

// Symbol: ?SetPropRadio@COlePropertyPage@@QEAAHPEB_WH@Z
extern "C" int MS_ABI impl__SetPropRadio_COlePropertyPage__QEAAHPEB_WH_Z(
    COlePropertyPage* pThis, const wchar_t* pszPropName, int nValue) {
    return SetPropScalar<int>(pThis, pszPropName, VT_I2, static_cast<int>(static_cast<short>(nValue)));
}

// Symbol: ?SetPropText@COlePropertyPage@@QEAAHPEB_WAEAE@Z
extern "C" int MS_ABI impl__SetPropText_COlePropertyPage__QEAAHPEB_WAEAE_Z(
    COlePropertyPage* pThis, const wchar_t* pszPropName, unsigned char* pValue) {
    if (!pValue) return FALSE;
    return SetPropScalar<int>(pThis, pszPropName, VT_UI1, static_cast<int>(*pValue));
}

// Symbol: ?SetPropText@COlePropertyPage@@QEAAHPEB_WAEAF@Z
extern "C" int MS_ABI impl__SetPropText_COlePropertyPage__QEAAHPEB_WAEAF_Z(
    COlePropertyPage* pThis, const wchar_t* pszPropName, short* pValue) {
    if (!pValue) return FALSE;
    return SetPropScalar<int>(pThis, pszPropName, VT_I2, static_cast<int>(*pValue));
}

// Symbol: ?SetPropText@COlePropertyPage@@QEAAHPEB_WAEAH@Z
extern "C" int MS_ABI impl__SetPropText_COlePropertyPage__QEAAHPEB_WAEAH_Z(
    COlePropertyPage* pThis, const wchar_t* pszPropName, int* pValue) {
    if (!pValue) return FALSE;
    return SetPropScalar<long>(pThis, pszPropName, VT_I4, static_cast<long>(*pValue));
}

// Symbol: ?SetPropText@COlePropertyPage@@QEAAHPEB_WAEAI@Z
extern "C" int MS_ABI impl__SetPropText_COlePropertyPage__QEAAHPEB_WAEAI_Z(
    COlePropertyPage* pThis, const wchar_t* pszPropName, unsigned int* pValue) {
    if (!pValue) return FALSE;
    return SetPropScalar<long>(pThis, pszPropName, VT_I4, static_cast<long>(*pValue));
}

// Symbol: ?SetPropText@COlePropertyPage@@QEAAHPEB_WAEAJ@Z
extern "C" int MS_ABI impl__SetPropText_COlePropertyPage__QEAAHPEB_WAEAJ_Z(
    COlePropertyPage* pThis, const wchar_t* pszPropName, long* pValue) {
    if (!pValue) return FALSE;
    return SetPropScalar<long>(pThis, pszPropName, VT_I4, *pValue);
}

// Symbol: ?SetPropText@COlePropertyPage@@QEAAHPEB_WAEAK@Z
extern "C" int MS_ABI impl__SetPropText_COlePropertyPage__QEAAHPEB_WAEAK_Z(
    COlePropertyPage* pThis, const wchar_t* pszPropName, unsigned long* pValue) {
    if (!pValue) return FALSE;
    return SetPropScalar<long>(pThis, pszPropName, VT_I4, static_cast<long>(*pValue));
}

// Symbol: ?SetPropText@COlePropertyPage@@QEAAHPEB_WAEAM@Z
extern "C" int MS_ABI impl__SetPropText_COlePropertyPage__QEAAHPEB_WAEAM_Z(
    COlePropertyPage* pThis, const wchar_t* pszPropName, float* pValue) {
    if (!pValue) return FALSE;
    return SetPropScalar<double>(pThis, pszPropName, VT_R4, static_cast<double>(*pValue));
}

// Symbol: ?SetPropText@COlePropertyPage@@QEAAHPEB_WAEAN@Z
extern "C" int MS_ABI impl__SetPropText_COlePropertyPage__QEAAHPEB_WAEAN_Z(
    COlePropertyPage* pThis, const wchar_t* pszPropName, double* pValue) {
    if (!pValue) return FALSE;
    return SetPropScalar<double>(pThis, pszPropName, VT_R8, *pValue);
}

// Symbol: ?SetPropText@COlePropertyPage@@QEAAHPEB_WAEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@@Z
extern "C" int MS_ABI impl__SetPropText_COlePropertyPage__QEAAHPEB_WAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
    // AEAV, not AEBV: the reference is NON-const (MFC declares it
    // `CString& strValue`); retail only reads its buffer pointer.
    COlePropertyPage* pThis, const wchar_t* pszPropName, void* pStrValueRaw) {
    CString* pStrValue = static_cast<CString*>(pStrValueRaw);
    if (!pStrValue) return FALSE;
    return SetPropScalar<const wchar_t*>(pThis, pszPropName, VT_BSTR, pStrValue->GetString());
}

// COlePropertyPage::WindowProc(UINT, WPARAM, LPARAM) -- retail 0x1e9830:
//     AFX_MAINTAIN_STATE2 _state(m_pModuleState /*0x38*/)
//     if (message == 0x112 /*WM_SYSCOMMAND*/ &&
//         (wParam & 0xfff0) != 0xf100 /*SC_KEYMENU*/) {
//         pSite = m_pPageSite /*0x190*/;
//         if (pSite != NULL &&
//             pSite->vtbl[6](pSite, CWnd::GetCurrentMessage()) == S_OK)
//             return 0;                                 ; slot 6 =
//     }                                                 ;  IPropertyPageSite::
//     return CWnd::WindowProc(message, wParam, lParam)  ;  TranslateAccelerator
// Symbol: ?WindowProc@COlePropertyPage@@MEAA_JI_K_J@Z
extern "C" __int64 MS_ABI impl__WindowProc_COlePropertyPage__MEAA_JI_K_J_Z(
    COlePropertyPage* pThis, unsigned int message, unsigned __int64 wParam, __int64 lParam) {
    if (!pThis) return 0;

    if (message == WM_SYSCOMMAND && (wParam & 0xfff0) != SC_KEYMENU) {
        IPropertyPageSite* pSite = pThis->m_pPageSite;
        if (pSite != nullptr) {
            const MSG* pMsg = impl__GetCurrentMessage_CWnd__KAPEBUtagMSG__XZ();
            if (pSite->TranslateAccelerator(const_cast<MSG*>(pMsg)) == S_OK) {
                return 0;
            }
        }
    }
    return impl__WindowProc_CWnd__MEAA_JI_K_J_Z(static_cast<CWnd*>(pThis), message,
                                                static_cast<WPARAM>(wParam),
                                                static_cast<LPARAM>(lParam));
}

// ===========================================================================
// XPropertyPage (IPropertyPage2) and XPropNotifySink (IPropertyNotifySink)
//
// Every body below converts the interface pointer it was handed into the
// outer object exactly as retail does -- `lea -0x1f0(%rcx)` for XPropertyPage
// (GetPageInfo addresses the outer's members directly at -0x88..-0x68 and
// IsPageDirty at -0x98, the same origin) and `lea -0x1f8(%rcx)` for
// XPropNotifySink. The vtables the constructors install are defined at the
// top of this file, with the retail slot tables.
//
// Outer vtable slots. Four of these methods call back into the outer through
// four CONSECUTIVE vtable slots:
//     0x328  SetPageSite  -> OnSetPageSite
//     0x330  SetObjects   -> OnObjectsChanged
//     0x338  Help         -> OnHelp
//     0x340  EditProperty -> OnEditProperty
// CDialog's retail vtable is exactly 0x328 bytes long (checked in the MBCS
// image: the qword after its last slot is ?GetThisMessageMap@CWnd@@, the
// first field of the next .rdata object), so these are precisely the first
// four virtuals COlePropertyPage adds, in declaration order, each matched to
// its name by the IPropertyPage method that documents it. OpenMFC's C++
// vtable is unrelated to retail's, so here OnSetPageSite / OnObjectsChanged /
// OnEditProperty are reached by C++ virtual dispatch on the OpenMFC
// declaration (a derived page's override is honoured), while OnHelp -- which
// afxole.h does not declare -- is called through its own thunk above
// (DEVIATION: a derived page's OnHelp override is not reachable until the
// header declares it; headerRequest). Two more outer slots are used:
//     0xd0   -> ?DestroyWindow@CWnd@@UEAAHXZ
//     0x2d8  -> CDialog::Create(UINT, CWnd*), an unexported inline that is
//               `movzwl %dx,%edx; jmp` onto the LPCTSTR form -- i.e.
//               Create(MAKEINTRESOURCE(id), parent)
//     0x2e8  -> ?CreateIndirect@CDialog@@UEAAHPEAXPEAVCWnd@@@Z
// ===========================================================================

// XPropertyPage::Activate(HWND hWndParent, LPCRECT prc, BOOL bModal) --
// retail 0x1e9c60, under AFX_MAINTAIN_STATE2 (bModal is never read):
//     if (m_hDialog /*0x1e8*/) {
//         LPVOID p = ::LockResource(m_hDialog);         ; IAT 0x2c6550
//         if (p) ok = outer->vtbl[0x2e8/8](p, CWnd::FromHandle(hWndParent));
//                                                       ; CreateIndirect
//         else   ok = outer->vtbl[0x2d8/8](m_idDialog /*0x15c*/,
//                                          CWnd::FromHandle(hWndParent));
//     } else {
//         ok = outer->vtbl[0x2d8/8](m_idDialog, CWnd::FromHandle(hWndParent));
//     }                                                 ; FromHandle 0x28ad70
//     if (!ok) return E_FAIL /*0x80004005*/;
//     CWnd::MoveWindow(prc->left, prc->top, prc->right - prc->left,
//                      prc->bottom - prc->top, TRUE);   ; 0x2a9a10, five ints
//     m_bInitializing /*0x1b0*/ = TRUE;
//     CWnd::UpdateData(FALSE);                          ; 0x2910d0
//     SetModifiedFlag(FALSE);                           ; 0x1e9970
//     m_bInitializing = FALSE;
//     if (m_pStatus /*0x1b8*/) { free(m_pStatus); m_pStatus = NULL; }
//     m_nControls /*0x1b4*/ = 0;
//     ::EnumChildWindows(m_hWnd, EnumChildProc, this);  ; count, IAT 0x2c6e10
//     if (m_nControls > 0)
//         m_pStatus = operator new(m_nControls * 8);    ; 0x27f0, overflow-checked
//     m_nControls = 0;
//     ::EnumChildWindows(m_hWnd, EnumControls, this);   ; fill
//     return S_OK;
// m_idDialog is CDialog::m_lpszTemplateName here (see the layout note), and
// SetModifiedFlag is the C++ member above. The status table is malloc'ed so
// that the free() retail pairs it with in the destructor and here is exact.
// DEVIATION: a NULL prc returns E_POINTER instead of faulting.
// Symbol: ?Activate@XPropertyPage@COlePropertyPage@@UEAAJPEAUHWND__@@PEBUtagRECT@@H@Z
extern "C" long MS_ABI impl__Activate_XPropertyPage_COlePropertyPage__UEAAJPEAUHWND____PEBUtagRECT__H_Z(
    void* pThis, HWND hWndParent, const RECT* prc, int /*bModal*/) {
    if (!pThis) return static_cast<long>(0x80004003L); // E_POINTER
    COlePropertyPage* pPage = OuterFromPropPage(pThis);
    S_COlePropertyPageTail* t = Tail(pPage);

    int bOK;
    LPVOID pTemplate = (t->m_hDialog != nullptr) ? ::LockResource(t->m_hDialog) : nullptr;
    if (pTemplate != nullptr) {
        bOK = impl__CreateIndirect_CDialog__UEAAHPEAXPEAVCWnd___Z(
            static_cast<CDialog*>(pPage), pTemplate,
            impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(hWndParent));
    } else {
        bOK = impl__Create_CDialog__UEAAHPEB_WPEAVCWnd___Z(
            static_cast<CDialog*>(pPage), pPage->m_lpszTemplateName,
            impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(hWndParent));
    }
    if (!bOK) return static_cast<long>(0x80004005L); // E_FAIL
    if (prc == nullptr) return static_cast<long>(0x80004003L); // E_POINTER

    impl__MoveWindow_CWnd__QEAAXHHHHH_Z(static_cast<CWnd*>(pPage), prc->left, prc->top,
                                        prc->right - prc->left, prc->bottom - prc->top, TRUE);
    t->m_bInitializing = TRUE;
    PageUpdateData(pPage, FALSE);
    pPage->SetModifiedFlag(FALSE);
    t->m_bInitializing = FALSE;

    if (t->m_pStatus != nullptr) {
        ::free(t->m_pStatus);
        t->m_pStatus = nullptr;
    }
    t->m_nControls = 0;
    ::EnumChildWindows(pPage->m_hWnd, impl__EnumChildProc_COlePropertyPage__KAHPEAUHWND_____J_Z,
                       reinterpret_cast<LPARAM>(pPage));
    if (t->m_nControls > 0) {
        t->m_pStatus = static_cast<AFX_PPFIELDSTATUS*>(
            ::malloc(static_cast<size_t>(t->m_nControls) * sizeof(AFX_PPFIELDSTATUS)));
        if (t->m_pStatus == nullptr) {
            t->m_nControls = 0;
            return static_cast<long>(0x8007000EL); // E_OUTOFMEMORY (retail: operator new throws)
        }
    }
    t->m_nControls = 0;
    ::EnumChildWindows(pPage->m_hWnd, impl__EnumControls_COlePropertyPage__KAHPEAUHWND_____J_Z,
                       reinterpret_cast<LPARAM>(pPage));
    return S_OK;
}

// XPropertyPage::Apply() -- retail 0x1ea110, under AFX_MAINTAIN_STATE2. Note
// the early exit: when the page is not dirty NOTHING else runs (`cmp
// %edi,0x158(%rbx); je <exit>` with EDI still 0 = S_OK).
//     HRESULT hr = S_OK;
//     if (m_bDirty /*0x158*/ == 0) return S_OK;
//     BOOL bOK = CWnd::UpdateData(TRUE);            ; 0x2910d0
//     if (bOK) m_bDirty = 0; else hr = E_FAIL /*0x80004005*/;
//     if (m_bPropsChanged /*0x1a8*/) {              ; deferred refill pending
//         CWnd::UpdateData(FALSE); m_bPropsChanged = 0;
//     } else if (!bOK) {
//         return hr;                                ; skip the reset
//     }
//     if (m_pStatus /*0x1b8*/)
//         for (i = 0; i < m_nControls /*0x1b4*/; ++i)
//             m_pStatus[i].bDirty = FALSE;
//     return hr;
// m_bDirty is m_bModified here. Note that the flag is cleared by a direct
// store, NOT through SetModifiedFlag, so the site is not told.
// Symbol: ?Apply@XPropertyPage@COlePropertyPage@@UEAAJXZ
extern "C" long MS_ABI impl__Apply_XPropertyPage_COlePropertyPage__UEAAJXZ(void* pThis) {
    if (!pThis) return static_cast<long>(0x80004003L); // E_POINTER
    COlePropertyPage* pPage = OuterFromPropPage(pThis);
    S_COlePropertyPageTail* t = Tail(pPage);

    long hr = S_OK;
    if (pPage->m_bModified == 0) return S_OK;
    int bOK = PageUpdateData(pPage, TRUE);
    if (bOK) {
        pPage->m_bModified = 0;
    } else {
        hr = static_cast<long>(0x80004005L); // E_FAIL
    }
    if (t->m_bPropsChanged) {
        PageUpdateData(pPage, FALSE);
        t->m_bPropsChanged = FALSE;
    } else if (!bOK) {
        return hr;
    }
    if (t->m_pStatus != nullptr) {
        for (int i = 0; i < t->m_nControls; ++i) {
            t->m_pStatus[i].bDirty = FALSE;
        }
    }
    return hr;
}

// XPropertyPage::Deactivate() -- retail 0x1e9e70: under AFX_MAINTAIN_STATE2,
// call the outer through vtable offset 0xd0 (CWnd::DestroyWindow) and
// return S_OK. DEVIATION: dispatched to CWnd::DestroyWindow's thunk, not
// virtually (see OnFinalRelease).
// Symbol: ?Deactivate@XPropertyPage@COlePropertyPage@@UEAAJXZ
extern "C" long MS_ABI impl__Deactivate_XPropertyPage_COlePropertyPage__UEAAJXZ(void* pThis) {
    if (!pThis) return static_cast<long>(0x80004003L); // E_POINTER
    impl__DestroyWindow_CWnd__UEAAHXZ(static_cast<CWnd*>(OuterFromPropPage(pThis)));
    return S_OK;
}

// XPropertyPage::EditProperty(DISPID) -- retail 0x1ea500: under
// AFX_MAINTAIN_STATE2, call the outer's vtable slot 0x340 (OnEditProperty)
// and map the BOOL to an HRESULT:
//     return ret ? S_OK : E_NOTIMPL /*0x80004001*/;   ; `neg; sbb; not; and`
// Symbol: ?EditProperty@XPropertyPage@COlePropertyPage@@UEAAJJ@Z
extern "C" long MS_ABI impl__EditProperty_XPropertyPage_COlePropertyPage__UEAAJJ_Z(
    void* pThis, long dispid) {
    if (!pThis) return static_cast<long>(0x80004003L); // E_POINTER
    return OuterFromPropPage(pThis)->OnEditProperty(dispid) ? S_OK : static_cast<long>(0x80004001L);
}

// XPropertyPage::GetPageInfo(PROPPAGEINFO*) -- retail 0x1e9ec0, no
// module-state frame:
//     if (pPageInfo == NULL) AfxThrowInvalidArgException();      ; 0x227720
//     pPageInfo->pszTitle      = dup(m_strPageName  /*outer+0x168*/); ; 0x1e77b4
//     pPageInfo->size          =     m_sizePage     /*outer+0x170*/;  ; one qword
//     pPageInfo->pszDocString  = dup(m_strDocString /*outer+0x178*/);
//     pPageInfo->pszHelpFile   = dup(m_strHelpFile  /*outer+0x180*/);
//     pPageInfo->dwHelpContext =     m_dwHelpContext/*outer+0x188*/;
//     return S_OK;
// It does not write pPageInfo->cb. dup() is TaskAllocString above. The
// three strings are the OpenMFC wchar_t* members; because retail's are
// CStrings, an unset one reads "" and is still duplicated, so a NULL member
// is duplicated as "" here to give the frame the same non-NULL result.
// Symbol: ?GetPageInfo@XPropertyPage@COlePropertyPage@@UEAAJPEAUtagPROPPAGEINFO@@@Z
extern "C" long MS_ABI impl__GetPageInfo_XPropertyPage_COlePropertyPage__UEAAJPEAUtagPROPPAGEINFO___Z(
    void* pThis, void* pPageInfoRaw) {
    if (!pThis) return static_cast<long>(0x80004003L); // E_POINTER
    if (pPageInfoRaw == nullptr) {
        impl__AfxThrowInvalidArgException__YAXXZ();
    }
    COlePropertyPage* pPage = OuterFromPropPage(pThis);
    PROPPAGEINFO* pPageInfo = static_cast<PROPPAGEINFO*>(pPageInfoRaw);
    pPageInfo->pszTitle      = TaskAllocString(pPage->m_pszPageName  ? pPage->m_pszPageName  : L"");
    pPageInfo->size          = Tail(pPage)->m_sizePage;
    pPageInfo->pszDocString  = TaskAllocString(pPage->m_pszDocString ? pPage->m_pszDocString : L"");
    pPageInfo->pszHelpFile   = TaskAllocString(pPage->m_pszHelpFile  ? pPage->m_pszHelpFile  : L"");
    pPageInfo->dwHelpContext = pPage->m_dwHelpContext;
    return S_OK;
}

// XPropertyPage::Help(LPCOLESTR pszHelpDir) -- retail 0x1ea1d0, under
// AFX_MAINTAIN_STATE2:
//     CString str(pszHelpDir);                      ; 0xdcb0
//     LPCTSTR arg = pszHelpDir ? (LPCTSTR)str : NULL;
//         ; `neg %rbx; sbb %rdx,%rdx; and 0x40(%rsp),%rdx` -- a NULL
//         ; pszHelpDir reaches OnHelp as NULL, not as the nil string
//     BOOL b = outer->vtbl[0x338/8](arg);           ; OnHelp
//     return b ? S_OK : S_FALSE;                    ; `test eax,eax; sete bl`
// The CString copy is only read, so pszHelpDir is passed straight through.
// Symbol: ?Help@XPropertyPage@COlePropertyPage@@UEAAJPEB_W@Z
extern "C" long MS_ABI impl__Help_XPropertyPage_COlePropertyPage__UEAAJPEB_W_Z(
    void* pThis, const wchar_t* pszHelpDir) {
    if (!pThis) return static_cast<long>(0x80004003L); // E_POINTER
    int b = impl__OnHelp_COlePropertyPage__UEAAHPEB_W_Z(OuterFromPropPage(pThis), pszHelpDir);
    return b ? S_OK : S_FALSE;
}

// XPropertyPage::IsPageDirty() -- retail 0x1ea100 is four instructions:
//     xor eax,eax ; cmp eax,[rcx-0x98] ; sete al ; ret
// rcx is outer+0x1f0, so -0x98 is outer+0x158, m_bDirty: 1 (S_FALSE) when
// the page is clean, 0 (S_OK) when it is dirty. m_bDirty is m_bModified.
// Symbol: ?IsPageDirty@XPropertyPage@COlePropertyPage@@UEAAJXZ
extern "C" long MS_ABI impl__IsPageDirty_XPropertyPage_COlePropertyPage__UEAAJXZ(void* pThis) {
    if (!pThis) return static_cast<long>(0x80004003L); // E_POINTER
    return (OuterFromPropPage(pThis)->m_bModified == 0) ? S_FALSE : S_OK;
}

// XPropertyPage::Move(LPCRECT prc) -- retail 0x1ea0c0, no module-state
// frame:
//     CWnd::MoveWindow(outer, prc->left, prc->top,
//                      prc->right - prc->left, prc->bottom - prc->top,
//                      TRUE);                          ; 0x2a9a10
//     return S_OK;
// DEVIATION: a NULL prc returns E_POINTER instead of faulting.
// Symbol: ?Move@XPropertyPage@COlePropertyPage@@UEAAJPEBUtagRECT@@@Z
extern "C" long MS_ABI impl__Move_XPropertyPage_COlePropertyPage__UEAAJPEBUtagRECT___Z(
    void* pThis, const RECT* prc) {
    if (!pThis || !prc) return static_cast<long>(0x80004003L); // E_POINTER
    impl__MoveWindow_CWnd__QEAAXHHHHH_Z(static_cast<CWnd*>(OuterFromPropPage(pThis)),
                                        prc->left, prc->top,
                                        prc->right - prc->left, prc->bottom - prc->top, TRUE);
    return S_OK;
}

// XPropNotifySink::OnChanged(DISPID) -- retail 0x1ea5d0, under
// AFX_MAINTAIN_STATE2 (the dispid is never read):
//     if (outer->m_hWnd /*0x40*/ &&
//         AfxGetThreadState()->m_hLockoutNotifyWindow /*+0x198*/ != m_hWnd)
//         CWnd::UpdateData(outer, FALSE);   ; 0x2910d0 -- refill the controls
//     else
//         m_bPropsChanged /*outer+0x1a8*/ = TRUE;   ; defer; Apply drains it
//     return S_OK;
// The lockout compare suppresses the refill while the page's own
// DoDataExchange is running; see t_hLockoutNotifyWindow at the top of this
// file for how, and how far, that is emulated here.
// Symbol: ?OnChanged@XPropNotifySink@COlePropertyPage@@UEAAJJ@Z
extern "C" long MS_ABI impl__OnChanged_XPropNotifySink_COlePropertyPage__UEAAJJ_Z(
    void* pThis, long /*dispid*/) {
    if (!pThis) return static_cast<long>(0x80004003L); // E_POINTER
    COlePropertyPage* pPage = OuterFromNotifySink(pThis);
    if (pPage->m_hWnd != nullptr && t_hLockoutNotifyWindow != pPage->m_hWnd) {
        PageUpdateData(pPage, FALSE);
    } else {
        Tail(pPage)->m_bPropsChanged = TRUE;
    }
    return S_OK;
}

// XPropNotifySink::OnRequestEdit(DISPID) -- mfc140u ordinal 10919 -> RVA
// 0x71e0, and slot 4 of the retail sink vtable (0x3248f8) holds the same
// address; the body is `xor %eax,%eax ; ret`: S_OK unconditionally. It never
// touches `this` or the dispid -- the page always permits the edit and waits
// for the matching OnChanged.
// Symbol: ?OnRequestEdit@XPropNotifySink@COlePropertyPage@@UEAAJJ@Z
extern "C" long MS_ABI impl__OnRequestEdit_XPropNotifySink_COlePropertyPage__UEAAJJ_Z(
    void* /*pThis*/, long /*dispid*/) {
    return S_OK;
}

// XPropertyPage::SetObjects(ULONG nObjects, IUnknown** ppUnk) -- retail
// 0x1e9f20, under AFX_MAINTAIN_STATE2; the sole writer of the object array
// the GetProp*/SetProp* family reads:
//     CleanupObjectArray();                              ; 0x1e95e0
//     if (nObjects) {
//         m_ppDisp /*0x198*/    = operator new(nObjects * 8);  ; 0x27f0, both
//         m_pAdvisors /*0x1a0*/ = operator new(nObjects * 4);  ; overflow-checked
//         for (i = 0; i < nObjects; ++i) {
//             hr = ppUnk[i]->QueryInterface(IID_IDispatch, &m_ppDisp[i]);
//             if (FAILED(hr)) return hr;   ; straight to the epilogue: m_nObjects
//                 ; is left UNWRITTEN and OnObjectsChanged is never called
//             AfxConnectionAdvise(ppUnk[i], IID_IPropertyNotifySink,
//                                 &m_xPropNotifySink /*this+0x1f8*/, FALSE,
//                                 &m_pAdvisors[i]);          ; 0x1df290
//         }
//     }
//     m_nObjects /*0x1ac*/ = nObjects;
//     BOOL bLocked = FALSE;
//     if (outer->m_hWnd /*0x40*/ && ::IsWindowVisible(m_hWnd)) {  ; IAT 0x2c7328
//         ::LockWindowUpdate(m_hWnd); bLocked = TRUE;             ; IAT 0x2c6c98
//     }
//     outer->vtbl[0x330/8]();          ; OnObjectsChanged -- UNCONDITIONAL
//     if (nObjects && outer->m_hWnd) {
//         CWnd::UpdateData(FALSE);     ; 0x2910d0
//         SetModifiedFlag(FALSE);      ; 0x1e9970
//     }
//     if (bLocked) ::LockWindowUpdate(NULL);
//     return S_OK;
// DEVIATIONS, both on the failure path only: the two arrays are zero-filled
// (retail's operator new leaves them uninitialised, and retail's early
// return leaves the OLD m_nObjects standing over the NEW arrays, which the
// next CleanupObjectArray then walks); here m_nObjects is set to the number
// of entries actually filled before returning the failing HRESULT, so that
// walk stays inside the arrays. The AfxConnectionAdvise result is ignored,
// as in retail.
// Symbol: ?SetObjects@XPropertyPage@COlePropertyPage@@UEAAJKPEAPEAUIUnknown@@@Z
extern "C" long MS_ABI impl__SetObjects_XPropertyPage_COlePropertyPage__UEAAJKPEAPEAUIUnknown___Z(
    void* pThis, unsigned long nObjects, void** ppUnk) {
    if (!pThis) return static_cast<long>(0x80004003L); // E_POINTER
    COlePropertyPage* pPage = OuterFromPropPage(pThis);
    S_COlePropertyPageTail* t = Tail(pPage);

    impl__CleanupObjectArray_COlePropertyPage__IEAAXXZ(pPage);
    if (nObjects != 0) {
        if (ppUnk == nullptr) return static_cast<long>(0x80004003L); // E_POINTER
        t->m_ppDisp    = static_cast<LPDISPATCH*>(::calloc(nObjects, sizeof(LPDISPATCH)));
        t->m_pAdvisors = static_cast<DWORD*>(::calloc(nObjects, sizeof(DWORD)));
        if (t->m_ppDisp == nullptr || t->m_pAdvisors == nullptr) {
            t->m_nObjects = 0;
            impl__CleanupObjectArray_COlePropertyPage__IEAAXXZ(pPage);
            return static_cast<long>(0x8007000EL); // E_OUTOFMEMORY (retail: operator new throws)
        }
        for (ULONG i = 0; i < nObjects; ++i) {
            IUnknown* pUnk = static_cast<IUnknown*>(ppUnk[i]);
            HRESULT hr = pUnk->QueryInterface(kIID_IDispatch, reinterpret_cast<void**>(&t->m_ppDisp[i]));
            if (FAILED(hr)) {
                t->m_nObjects = i;
                return static_cast<long>(hr);
            }
            impl__AfxConnectionAdvise__YAHPEAUIUnknown__AEBU_GUID__0HPEAK_Z(
                pUnk, &kIID_IPropertyNotifySink, NotifySinkOf(pPage), FALSE, &t->m_pAdvisors[i]);
        }
    }
    t->m_nObjects = nObjects;

    BOOL bLocked = FALSE;
    if (pPage->m_hWnd != nullptr && ::IsWindowVisible(pPage->m_hWnd)) {
        ::LockWindowUpdate(pPage->m_hWnd);
        bLocked = TRUE;
    }
    pPage->OnObjectsChanged();
    if (nObjects != 0 && pPage->m_hWnd != nullptr) {
        PageUpdateData(pPage, FALSE);
        pPage->SetModifiedFlag(FALSE);
    }
    if (bLocked) {
        ::LockWindowUpdate(nullptr);
    }
    return S_OK;
}

// XPropertyPage::SetPageSite(IPropertyPageSite*) -- retail 0x1e9bd0, under
// AFX_MAINTAIN_STATE2:
//     _AfxRelease(&outer->m_pPageSite /*0x190*/);   ; 0x26ccc4: Release + NULL
//     outer->m_pPageSite = pPageSite;
//     if (pPageSite) {
//         pPageSite->AddRef();                      ; IUnknown slot 1
//         outer->vtbl[0x328/8]();                   ; OnSetPageSite
//     }
//     return S_OK;
// Retail's OnSetPageSite returns void; OpenMFC's declaration returns BOOL
// and its result is ignored here, which is all retail could do with it.
// Symbol: ?SetPageSite@XPropertyPage@COlePropertyPage@@UEAAJPEAUIPropertyPageSite@@@Z
extern "C" long MS_ABI impl__SetPageSite_XPropertyPage_COlePropertyPage__UEAAJPEAUIPropertyPageSite___Z(
    void* pThis, void* pPageSiteRaw) {
    if (!pThis) return static_cast<long>(0x80004003L); // E_POINTER
    COlePropertyPage* pPage = OuterFromPropPage(pThis);
    IPropertyPageSite* pPageSite = static_cast<IPropertyPageSite*>(pPageSiteRaw);
    if (pPage->m_pPageSite != nullptr) {
        pPage->m_pPageSite->Release();
        pPage->m_pPageSite = nullptr;
    }
    pPage->m_pPageSite = pPageSite;
    if (pPageSite != nullptr) {
        pPageSite->AddRef();
        (void)pPage->OnSetPageSite();
    }
    return S_OK;
}

// XPropertyPage::Show(UINT nCmdShow) -- retail 0x1ea080, no module-state
// frame:
//     CWnd::ShowWindow(outer, nCmdShow);            ; 0x2a9ad0
//     if (nCmdShow == SW_SHOWNORMAL /*1*/) CWnd::SetFocus(outer);  ; 0x2a9b60
//     return S_OK;
// (0x2a9ad0 / 0x2a9b60 are absent from the mfc140u symbol map; they are the
// bodies at the MBCS map's ?ShowWindow@CWnd@@QEAAHH@Z 0x2a79e0 and
// ?SetFocus@CWnd@@QEAAPEAV1@XZ 0x2a7a70.)
// Symbol: ?Show@XPropertyPage@COlePropertyPage@@UEAAJI@Z
extern "C" long MS_ABI impl__Show_XPropertyPage_COlePropertyPage__UEAAJI_Z(
    void* pThis, unsigned int nCmdShow) {
    if (!pThis) return static_cast<long>(0x80004003L); // E_POINTER
    COlePropertyPage* pPage = OuterFromPropPage(pThis);
    impl__ShowWindow_CWnd__QEAAHH_Z(static_cast<CWnd*>(pPage), static_cast<int>(nCmdShow));
    if (nCmdShow == SW_SHOWNORMAL) {
        impl__SetFocus_CWnd__QEAAPEAV1_XZ(static_cast<CWnd*>(pPage));
    }
    return S_OK;
}

// XPropertyPage::TranslateAccelerator(MSG*) -- retail 0x1ea270 (mfc140u,
// ordinal 14015), under AFX_MAINTAIN_STATE2. The page owns the Tab key;
// everything else goes to PreTranslateMessage. In full (IAT slots via
// iatu.py: 0x2c6ca0 GetKeyState, 0x2c71b0 GetFocus, 0x2c71b8 IsChild,
// 0x2c7120 SendMessageW, 0x2c72d8 GetParent, 0x2c6c10 GetWindow, 0x2c6d20
// GetTopWindow; 0x2c7b30 is the CFG __guard_dispatch_icall_fptr, i.e. the
// two `call *0x2c7b30` sites are ordinary virtual calls):
//     HRESULT hr = S_FALSE;                                       ; EBP = 1
//     if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_TAB &&
//         ::GetKeyState(VK_CONTROL) >= 0) {                       ; `test %ax,%ax; js`
//         CWnd* pFocus = CWnd::FromHandle(::GetFocus());          ; 0x28ad70
//         if (::IsChild(m_hWnd, pFocus ? pFocus->m_hWnd : NULL)) {
//             // --- focus is inside the page ---
//             short sShift = ::GetKeyState(VK_SHIFT);             ; ESI
//             if (::SendMessage(m_hWnd /*the PAGE, not the control*/,
//                               WM_GETDLGCODE, 0, 0) & (DLGC_WANTTAB|DLGC_WANTALLKEYS))
//                 goto deflt;                                     ; `test $0x6,%al`
//             pFocus = CWnd::FromHandle(::GetFocus());            ; repeated
//             if (!::IsChild(m_hWnd, pFocus ? pFocus->m_hWnd : NULL)) goto deflt;
//             CWnd* pWnd = pFocus;                                ; climb to the
//             while (CWnd::FromHandle(::GetParent(pWnd->m_hWnd)) != this)  ; page's own
//                 pWnd = CWnd::FromHandle(::GetParent(pWnd->m_hWnd));      ; child
//             UINT nDir = sShift < 0 ? GW_HWNDPREV : GW_HWNDNEXT; ; `sar $0x1f; and $1; +2`
//             for (;;) {
//                 pWnd = CWnd::FromHandle(::GetWindow(pWnd->m_hWnd, nDir));
//                 if (pWnd == NULL) break;                        ; end of the order
//                 if ((pWnd->GetStyle() & (WS_TABSTOP|WS_DISABLED)) == WS_TABSTOP)
//                     goto deflt;   ; a later tab stop exists: the dialog's own
//             }                     ; PreTranslateMessage moves the focus
//             // end of the page's tab order: park the focus on the default
//             // button, then let the frame take the Tab
//             LRESULT lr = ::SendMessage(m_hWnd, DM_GETDEFID /*WM_USER*/, 0, 0);
//             if (HIWORD(lr) == DC_HASDEFID /*0x534b*/) {
//                 CWnd* pDef = GetDlgItem(LOWORD(lr));            ; 0x2a9390
//                 if (pDef && pDef->IsWindowEnabled())            ; 0x2a9b00
//                     ::SendMessage(m_hWnd, WM_NEXTDLGCTL, (WPARAM)pDef->m_hWnd, TRUE);
//             }
//             if (m_pPageSite /*0x190*/ &&
//                 m_pPageSite->TranslateAccelerator(pMsg) == S_OK) ; slot 6 (+0x30)
//                 return S_OK;
//             goto deflt;
//         }
//         // --- focus is outside the page: bring it in ---
//         CWnd* pWnd = CWnd::FromHandle(::GetTopWindow(m_hWnd));
//         if (pWnd == NULL) goto deflt;
//         WORD w = (WORD)::GetKeyState(VK_SHIFT);
//         BOOL bShift = (short)w < 0;                             ; ESI
//         pWnd = CWnd::FromHandle(::GetWindow(pWnd->m_hWnd, w >> 15)); ; GW_HWNDFIRST /
//         while (pWnd != NULL) {                                       ; GW_HWNDLAST
//             if ((pWnd->GetStyle() & (WS_TABSTOP|WS_DISABLED)) == WS_TABSTOP) {
//                 ::SendMessage(m_hWnd, WM_NEXTDLGCTL, (WPARAM)pWnd->m_hWnd, TRUE);
//                 return S_OK;
//             }
//             pWnd = CWnd::FromHandle(::GetWindow(pWnd->m_hWnd,
//                                     bShift ? GW_HWNDPREV : GW_HWNDNEXT));
//         }
//     }
//   deflt:
//     BOOL b = this->vtbl[0x228/8](pMsg);                          ; PreTranslateMessage
//     return b ? S_OK : S_FALSE;                                   ; `je; mov %r15d,%ebp`
// Slot 0x228 is ?PreTranslateMessage@CDialog@@ (0x207c40) in the CDialog
// vftable at 0x325658 (mfc140u); on a page it resolves to
// COlePropertyPage::PreTranslateMessage (0x1e9470), so it is called through
// that thunk above. DEVIATION: retail dispatches it virtually, so a derived
// page's PreTranslateMessage override would be honoured there and is not
// here (afxole.h does not declare the override; the same limitation as
// OnHelp). DEVIATION (defensive): retail dereferences pMsg and, in the
// parent climb, every CWnd::FromHandle result unconditionally; a NULL pMsg
// returns E_POINTER and a NULL step in the climb ends it.
// Symbol: ?TranslateAcceleratorW@XPropertyPage@COlePropertyPage@@UEAAJPEAUtagMSG@@@Z
extern "C" long MS_ABI impl__TranslateAcceleratorW_XPropertyPage_COlePropertyPage__UEAAJPEAUtagMSG___Z(
    void* pThis, void* pMsgRaw) {
    if (!pThis || !pMsgRaw) return static_cast<long>(0x80004003L); // E_POINTER
    COlePropertyPage* pPage = OuterFromPropPage(pThis);
    MSG* pMsg = static_cast<MSG*>(pMsgRaw);
    const HWND hPage = pPage->m_hWnd;

    if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_TAB &&
        ::GetKeyState(VK_CONTROL) >= 0) {
        CWnd* pFocus = impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(::GetFocus());
        if (::IsChild(hPage, pFocus ? pFocus->m_hWnd : nullptr)) {
            // Focus is inside the page.
            const short sShift = ::GetKeyState(VK_SHIFT);
            if ((::SendMessage(hPage, WM_GETDLGCODE, 0, 0) & (DLGC_WANTTAB | DLGC_WANTALLKEYS)) != 0) {
                goto deflt;
            }
            pFocus = impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(::GetFocus());
            if (!::IsChild(hPage, pFocus ? pFocus->m_hWnd : nullptr)) {
                goto deflt;
            }
            CWnd* pWnd = pFocus;
            for (;;) {
                CWnd* pParent = impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(::GetParent(pWnd->m_hWnd));
                if (pParent == static_cast<CWnd*>(pPage)) break;
                if (pParent == nullptr) goto deflt;   // defensive; retail would fault
                pWnd = pParent;
            }
            const UINT nDir = (sShift < 0) ? GW_HWNDPREV : GW_HWNDNEXT;
            for (;;) {
                pWnd = impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(::GetWindow(pWnd->m_hWnd, nDir));
                if (pWnd == nullptr) break;
                if ((impl__GetStyle_CWnd__QEBAKXZ(pWnd) & (WS_TABSTOP | WS_DISABLED)) == WS_TABSTOP) {
                    goto deflt;
                }
            }
            const LRESULT lr = ::SendMessage(hPage, DM_GETDEFID, 0, 0);
            if (HIWORD(lr) == DC_HASDEFID) {
                CWnd* pDef = impl__GetDlgItem_CWnd__QEBAPEAV1_H_Z(pPage, static_cast<int>(LOWORD(lr)));
                if (pDef != nullptr && impl__IsWindowEnabled_CWnd__QEBAHXZ(pDef)) {
                    ::SendMessage(hPage, WM_NEXTDLGCTL, reinterpret_cast<WPARAM>(pDef->m_hWnd), TRUE);
                }
            }
            if (pPage->m_pPageSite != nullptr &&
                pPage->m_pPageSite->TranslateAccelerator(pMsg) == S_OK) {
                return S_OK;
            }
            goto deflt;
        }

        // Focus is outside the page: move it onto the first (last, with
        // Shift) enabled tab stop among the page's children.
        CWnd* pWnd = impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(::GetTopWindow(hPage));
        if (pWnd == nullptr) goto deflt;
        const WORD wShift = static_cast<WORD>(::GetKeyState(VK_SHIFT));
        const BOOL bShift = static_cast<short>(wShift) < 0;
        pWnd = impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(
            ::GetWindow(pWnd->m_hWnd, static_cast<UINT>(wShift >> 15)));   // GW_HWNDFIRST / GW_HWNDLAST
        while (pWnd != nullptr) {
            if ((impl__GetStyle_CWnd__QEBAKXZ(pWnd) & (WS_TABSTOP | WS_DISABLED)) == WS_TABSTOP) {
                ::SendMessage(hPage, WM_NEXTDLGCTL, reinterpret_cast<WPARAM>(pWnd->m_hWnd), TRUE);
                return S_OK;
            }
            pWnd = impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(
                ::GetWindow(pWnd->m_hWnd, bShift ? GW_HWNDPREV : GW_HWNDNEXT));
        }
    }

deflt:
    return impl__PreTranslateMessage_COlePropertyPage__MEAAHPEAUtagMSG___Z(pPage, pMsg) ? S_OK : S_FALSE;
}
