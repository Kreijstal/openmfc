// CHtmlEditView — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp
//
// ---------------------------------------------------------------------------
// Retail layout (mfc140u, read from the disassembly cited on each body below):
//   class CHtmlEditView : public CHtmlView, public CHtmlEditCtrlBase<CHtmlEditView>
//   (afxhtml.h:1668).  CHtmlEditCtrlBase is an empty mix-in: CreateObject
//   (RVA 0x27e790) allocates 0x250 bytes == retail sizeof(CHtmlView), and the
//   bodies that call the mix-in's inlined helpers compute
//   `lea 0x250(%rcx); neg; sbb; and %rcx` -- the null-preserving cast of the
//   mix-in subobject back to CHtmlEditView*, which is `this` for any non-null
//   object.  CHtmlEditView adds no data members of its own.
//
//   Retail reads CHtmlView::m_pBrowserApp at +0x248.  OpenMFC's CHtmlView
//   (include/openmfc/afxwin.h) keeps the browser in `IWebBrowser2* m_pBrowser`
//   at +0xf8 instead, and that is the member OpenMFC's CHtmlView::Create fills
//   (core/view/CHtmlView.cpp documents the m_pBrowser == m_pBrowserApp
//   convention).  The bodies here follow that convention, pinned below.
//
//   Retail CHtmlEditView vftable, RVA 0x333568 (mfc140u), 154 slots; the ones
//   dispatched from this file were read out of that table:
//     slot 128 (+0x400) OnNavigateComplete2  -> 0x27e060
//     slot 151 (+0x4b8) GetStartDocument     -> 0x27db70
//     slot 152 (+0x4c0) GetDHtmlCommandMapping -> 0x27e110
//     slot 153 (+0x4c8) ExecHandler          -> 0x71e0
//
// Virtual dispatch.  OpenMFC has no C++ CHtmlEditView class and no MSVC-shaped
// CHtmlEditView vftable.  An object whose vptr points into a CLIENT image was
// built from a class compiled against the real MFC headers (the usual case: an
// AppWizard CMyView : public CHtmlEditView), so its vftable has retail's layout
// and retail's slot numbers are exact -- such objects are dispatched through the
// slot, as retail does.  An object whose vptr points into THIS image was built
// by OpenMFC (our CreateObject, which leaves OpenMFC's Itanium CHtmlView vptr
// in place); for it retail's slot numbers mean something else, so the retail
// CHtmlEditView body of that slot is called directly instead (DEVIATION: no
// override can exist for such an object, so the result is the same).  This is
// the IsOwnObject test core/frame/CFrameImpl.cpp and core/dialog/CDialogImpl.cpp
// use.
// ---------------------------------------------------------------------------

#include "detail/ManualSmallStubImplementationsSupport.h"
#include <mshtml.h>
#include <docobj.h>

// Thunks defined elsewhere in the tree (definitions verified by grep).
extern "C" void* MS_ABI impl___0CHtmlView__IEAA_XZ(CHtmlView* pThis);                               // core/view/CHtmlView.cpp
extern "C" int MS_ABI impl__Create_CHtmlView__UEAAHPEB_W0KAEBUtagRECT__PEAVCWnd__IPEAUCCreateContext___Z(
    CHtmlView* pThis, const wchar_t* lpszClassName, const wchar_t* lpszWindowName,
    unsigned long dwStyle, const RECT& rect, CWnd* pParentWnd, unsigned int nID,
    CCreateContext* pContext);                                                                       // core/view/CHtmlView.cpp
extern "C" void MS_ABI impl__Navigate_CHtmlView__QEAAXPEB_WK00PEAXK_Z(
    CHtmlView* pThis, const wchar_t* lpszURL, unsigned long dwFlags,
    const wchar_t* lpszTargetFrameName, const wchar_t* lpszHeaders,
    void* lpvPostData, unsigned long dwPostDataLen);                                                 // core/view/CHtmlView.cpp
extern "C" int MS_ABI impl__OnCmdMsg_CView__UEAAHIHPEAXPEAUAFX_CMDHANDLERINFO___Z(
    CView* pThis, unsigned int nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);      // core/view/CView.cpp
extern "C" __int64 MS_ABI impl__Default_CWnd__IEAA_JXZ(CWnd* pThis);                                // core/window/Thunks.cpp
extern "C" void* MS_ABI impl___2_YAPEAX_K_Z(std::size_t size);                                      // detail/MemcoreSupport.cpp
extern "C" void MS_ABI impl__AfxThrowMemoryException__YAXXZ();                                      // detail/MfcExceptionsSupport.cpp
extern "C" void MS_ABI impl__Enable_CCmdUI__UEAAXH_Z(CCmdUI* pThis, int enable);                    // core/cmdui/CCmdUI.cpp
extern "C" void MS_ABI impl__SetCheck_CCmdUI__UEAAXH_Z(CCmdUI* pThis, int nCheck);                  // core/cmdui/CCmdUI.cpp
extern "C" void MS_ABI impl__SetRadio_CCmdUI__UEAAXH_Z(CCmdUI* pThis, int bOn);                     // core/cmdui/CCmdUI.cpp

// Siblings defined below and called before their definitions.
extern "C" int MS_ABI impl__ExecHandler_CHtmlEditView__UEAAHI_Z(void* pThis, unsigned int nCmdID);
extern "C" unsigned int MS_ABI impl__GetDHtmlCommandMapping_CHtmlEditView__UEAAIIAEAHAEAI_Z(
    void* pThis, unsigned int nIDWindowsCommand, int* bHasExecFunc, unsigned int* uiElemType);
extern "C" int MS_ABI impl__GetDHtmlDocument_CHtmlEditView__QEBAHPEAPEAUIHTMLDocument2___Z(
    const void* pThis, IHTMLDocument2** ppDocument);
extern "C" const wchar_t* MS_ABI impl__GetStartDocument_CHtmlEditView__UEAAPEB_WXZ(void* pThis);

// The linker-provided base of this DLL's own image (IsOwnObject).
extern "C" IMAGE_DOS_HEADER __ImageBase;

namespace {

static_assert(offsetof(CHtmlView, m_pBrowser) == 0xf8,
              "OpenMFC CHtmlView::m_pBrowser (stands in for retail m_pBrowserApp at +0x248)");
static_assert(sizeof(CHtmlView) == 0x250, "retail CreateObject allocates 0x250 == sizeof(CHtmlView)");

constexpr std::size_t kSizeofCHtmlEditView = 0x250;   // `mov $0x250,%ecx` in CreateObject (RVA 0x27e790)

// Retail vftable slots (CHtmlEditView vftable RVA 0x333568, mfc140u).
constexpr int kSlotGetStartDocument       = 0x4b8 / 8;   // 151
constexpr int kSlotGetDHtmlCommandMapping = 0x4c0 / 8;   // 152
constexpr int kSlotExecHandler            = 0x4c8 / 8;   // 153
// Retail CCmdUI vftable (declaration order in the retail atlmfc afxwin.h:
// Enable, SetCheck, SetRadio, SetText; no virtual destructor).
constexpr int kSlotCmdUIEnable   = 0;
constexpr int kSlotCmdUISetCheck = 1;
constexpr int kSlotCmdUISetRadio = 2;

// afxhtml.h macros AFX_UI_ELEMTYPE_CHECBOX (sic) / AFX_UI_ELEMTYPE_RADIO /
// AFX_INVALID_DHTML_CMD_ID (defined in the "CHtmlEditView" section of that header).
constexpr unsigned int kElemTypeCheckBox = 0x1;   // `testb $0x1,0x58(%rbp)` on uiElemType
constexpr unsigned int kElemTypeRadio    = 0x2;   // `test %r14b(=2),0x58(%rbp)` on uiElemType
constexpr unsigned int kInvalidDhtmlCmdId = 0xFFFFFFFFu;

constexpr int kCN_UPDATE_COMMAND_UI = -1;

// GUIDs the retail bodies reference by address (bytes read from mfc140u .rdata):
//   0x18034f0a0 = {332C4425-26CB-11D0-B483-00C04FD90119}  IID_IHTMLDocument2
//   0x18034f080 = {B722BCCB-4E68-101B-A2BC-00AA00404770}  IID_IOleCommandTarget
//   0x1802d9fd0 = {DE4BA900-59CA-11CF-9592-444553540000}  CGID_MSHTML
const GUID kIID_IHTMLDocument2    = { 0x332C4425, 0x26CB, 0x11D0, { 0xB4,0x83,0x00,0xC0,0x4F,0xD9,0x01,0x19 } };
const GUID kIID_IOleCommandTarget = { 0xB722BCCB, 0x4E68, 0x101B, { 0xA2,0xBC,0x00,0xAA,0x00,0x40,0x47,0x70 } };
const GUID kCGID_MSHTML           = { 0xDE4BA900, 0x59CA, 0x11CF, { 0x95,0x92,0x44,0x45,0x53,0x54,0x00,0x00 } };

bool IsOwnObject(const void* pObject) {
    const void* vptr = *reinterpret_cast<const void* const*>(pObject);
    const unsigned char* base = reinterpret_cast<const unsigned char*>(&__ImageBase);
    const IMAGE_NT_HEADERS* nt = reinterpret_cast<const IMAGE_NT_HEADERS*>(
        base + reinterpret_cast<const IMAGE_DOS_HEADER*>(base)->e_lfanew);
    const unsigned char* p = static_cast<const unsigned char*>(vptr);
    return p >= base && p < base + nt->OptionalHeader.SizeOfImage;
}

template <typename Fn>
Fn VSlot(const void* pObject, int nSlot) {
    void* const* vtbl = *reinterpret_cast<void* const* const*>(pObject);
    return reinterpret_cast<Fn>(vtbl[nSlot]);
}

// this->GetStartDocument()  (retail: `call *0x4b8(vtbl)`).
const wchar_t* CallGetStartDocument(void* pThis) {
    if (IsOwnObject(pThis))
        return impl__GetStartDocument_CHtmlEditView__UEAAPEB_WXZ(pThis);
    using Fn = const wchar_t* (MS_ABI*)(void*);
    return VSlot<Fn>(pThis, kSlotGetStartDocument)(pThis);
}

// this->GetDHtmlCommandMapping(nID, bHasExecFunc, uiElemType)  (retail: `call *0x4c0(vtbl)`).
unsigned int CallGetDHtmlCommandMapping(void* pThis, unsigned int nID, int* bHasExecFunc,
                                        unsigned int* uiElemType) {
    if (IsOwnObject(pThis))
        return impl__GetDHtmlCommandMapping_CHtmlEditView__UEAAIIAEAHAEAI_Z(pThis, nID, bHasExecFunc, uiElemType);
    using Fn = unsigned int (MS_ABI*)(void*, unsigned int, int*, unsigned int*);
    return VSlot<Fn>(pThis, kSlotGetDHtmlCommandMapping)(pThis, nID, bHasExecFunc, uiElemType);
}

// this->ExecHandler(nID)  (retail: `call *0x4c8(vtbl)`).
int CallExecHandler(void* pThis, unsigned int nID) {
    if (IsOwnObject(pThis))
        return impl__ExecHandler_CHtmlEditView__UEAAHI_Z(pThis, nID);
    using Fn = int (MS_ABI*)(void*, unsigned int);
    return VSlot<Fn>(pThis, kSlotExecHandler)(pThis, nID);
}

// pCmdUI->Enable / SetCheck / SetRadio (retail: CCmdUI vftable slots 0 / 1 / 2).
// A CCmdUI built by a client has retail's vftable and is dispatched through the
// slot.  One built by OpenMFC goes through the exported CCmdUI thunks, as the
// OnUpdateEdit* handlers in core/view/CHtmlView.cpp do (DEVIATION: OpenMFC's own
// CCmdUI class (include/openmfc/afxole.h) declares a virtual destructor first and has no virtual SetRadio,
// so retail's slot numbers cannot be used on it).  Caveat: the MsCmdUI objects
// core/controlbar/CStatusBar.cpp builds carry an MSVC-shaped vftable that lives
// in THIS image (as do those of featurepack/controls/CMFCStatusBar.cpp), so this
// test also sends them to the thunks.  CStatusBar::OnUpdateCmdUI passes them to
// the frame's OnCmdMsg (MSVC vslot 5) for every pane ID, and in retail's routing
// the frame forwards to the active view (whether OpenMFC's CFrameWnd::OnCmdMsg
// thunk forwards them today was not verified), so if they reach this body and a
// client's DHTMLEDITING_CMDMAP maps a status-bar pane ID, the thunks' Itanium
// virtual calls (Enable = Itanium slot 2, SetCheck = slot 3, after the two
// destructor slots) land on that 4-entry MSVC table's SetRadio and SetText
// entries -- wrong behaviour, and SetCheck would pass an int as a string
// pointer.  KNOWN HAZARD, not fixed here: nothing in this file can tell an
// in-image MSVC-shaped CCmdUI vftable from an Itanium one.
void CmdUICall(CCmdUI* pCmdUI, int nSlot, int nArg) {
    if (!IsOwnObject(pCmdUI)) {
        using Fn = void (MS_ABI*)(CCmdUI*, int);
        VSlot<Fn>(pCmdUI, nSlot)(pCmdUI, nArg);
        return;
    }
    switch (nSlot) {
    case kSlotCmdUIEnable:   impl__Enable_CCmdUI__UEAAXH_Z(pCmdUI, nArg); break;
    case kSlotCmdUISetCheck: impl__SetCheck_CCmdUI__UEAAXH_Z(pCmdUI, nArg); break;
    default:                 impl__SetRadio_CCmdUI__UEAAXH_Z(pCmdUI, nArg); break;
    }
}

// The inlined CComQIPtr<IOleCommandTarget> spCmdTarg = spDoc both OnCmdMsg paths
// use (afxhtml.h CHtmlEditCtrlBase::QueryStatus / ExecCommand): a failed
// QueryInterface (`test %eax,%eax; jns`) leaves the target NULL.
IOleCommandTarget* QueryCommandTarget(IHTMLDocument2* pDoc) {
    IOleCommandTarget* pTarget = nullptr;
    if (FAILED(pDoc->QueryInterface(kIID_IOleCommandTarget, reinterpret_cast<void**>(&pTarget))))
        pTarget = nullptr;
    return pTarget;
}

} // namespace

// Retail ??0CHtmlEditView (RVA 0x27db90, mfc140u):
//     CHtmlView::CHtmlView(this);  this->vfptr = CHtmlEditView::`vftable' (0x180333568);  return this;
// DEVIATION: OpenMFC has no MSVC-shaped CHtmlEditView vftable, so the vptr the
// CHtmlView constructor stored (OpenMFC's own) is left in place.  A client
// derived class overwrites it with its own vftable right after this returns.
// Symbol: ??0CHtmlEditView@@QEAA@XZ
extern "C" void* MS_ABI impl___0CHtmlEditView__QEAA_XZ(void* pThis) {
    impl___0CHtmlView__IEAA_XZ(static_cast<CHtmlView*>(pThis));
    return pThis;
}
// Retail ??1CHtmlEditView (RVA 0x27dc10, mfc140u):
//     this->vfptr = CHtmlEditView::`vftable';  jmp CHtmlView::~CHtmlView (0x27ab20)
// Left a stub.  The exported chain target, impl___1CHtmlView__UEAA_XZ
// (core/view/CHtmlView.cpp), is `pThis->~CHtmlView()` -- an UNQUALIFIED
// destructor call, which g++ compiles as a virtual call through the object's
// vptr.  The objects that reach this export are client-derived (the client's
// MSVC destructor calls it), so their vptr is an MSVC-shaped vftable and that
// Itanium-indexed call would land on whatever retail-layout slot shares that
// index (not verified which).  A real C++ CHtmlView::~CHtmlView() IS defined
// (core/view/CHtmlView.cpp, next to the CHtmlView constructor), so a qualified
// `static_cast<CHtmlView*>(pThis)->CHtmlView::~CHtmlView()` would link, but it
// would add a C++ symbol reference to this file, which the per-file link audit
// rejects; the fix belongs in that thunk (make its call qualified), after
// which this body can chain to it.  Until then chaining here would risk
// turning a leak into a crash.
// Symbol: ??1CHtmlEditView@@UEAA@XZ
extern "C" void MS_ABI impl___1CHtmlEditView__UEAA_XZ(void* pThis) {
    (void)pThis;
}
// Retail Create (RVA 0x27dc20, mfc140u):
//     if (!CHtmlView::Create(lpszClassName, lpszWindowName, dwStyle, rect,
//                            pParentWnd, nID, pContext))          // direct call 0x27b1a0
//         return FALSE;
//     LPCTSTR lpszDoc = GetStartDocument();                         // vslot 0x4b8/8 = 151
//     if (lpszDoc != NULL)
//         Navigate(lpszDoc, 0, NULL, NULL, NULL, 0);                // direct call 0x27c770
//     return TRUE;
// Symbol: ?Create@CHtmlEditView@@UEAAHPEB_W0KAEBUtagRECT@@PEAVCWnd@@IPEAUCCreateContext@@@Z
extern "C" int MS_ABI impl__Create_CHtmlEditView__UEAAHPEB_W0KAEBUtagRECT__PEAVCWnd__IPEAUCCreateContext___Z(
    void* pThis, const wchar_t* lpszClassName, const wchar_t* lpszWindowName, unsigned long dwStyle,
    const RECT* rect, CWnd* pParentWnd, unsigned int nID, CCreateContext* pContext) {
    CHtmlView* pView = static_cast<CHtmlView*>(pThis);
    if (!impl__Create_CHtmlView__UEAAHPEB_W0KAEBUtagRECT__PEAVCWnd__IPEAUCCreateContext___Z(
            pView, lpszClassName, lpszWindowName, dwStyle, *rect, pParentWnd, nID, pContext))
        return FALSE;
    const wchar_t* lpszDoc = CallGetStartDocument(pThis);
    if (lpszDoc != nullptr)
        impl__Navigate_CHtmlView__QEAAXPEB_WK00PEAXK_Z(pView, lpszDoc, 0, nullptr, nullptr, nullptr, 0);
    return TRUE;
}

// Retail CreateObject (RVA 0x27e790, mfc140u):
//     void* p = operator new(0x250);            // call 0x27f0 = ??2@YAPEAX_K@Z
//     return p ? new(p) CHtmlEditView : NULL;   // call 0x27db90 = ??0CHtmlEditView
// Symbol: ?CreateObject@CHtmlEditView@@SAPEAVCObject@@XZ
extern "C" void* MS_ABI impl__CreateObject_CHtmlEditView__SAPEAVCObject__XZ() {
    void* p = impl___2_YAPEAX_K_Z(kSizeofCHtmlEditView);
    if (p == nullptr)
        return nullptr;
    return impl___0CHtmlEditView__QEAA_XZ(p);
}

// Retail ExecHandler: export ordinal 4436 resolves to RVA 0x71e0 (mfc140u), an
// identical-code-folded `xor %eax,%eax; ret` -- the base returns FALSE.  (It is
// slot 153 of the CHtmlEditView vftable; a DHTMLEDITING_CMDMAP in a derived
// class overrides it.)
// Symbol: ?ExecHandler@CHtmlEditView@@UEAAHI@Z
extern "C" int MS_ABI impl__ExecHandler_CHtmlEditView__UEAAHI_Z(void* pThis, unsigned int nCmdID) {
    (void)pThis;
    (void)nCmdID;
    return FALSE;
}

// Retail GetDHtmlCommandMapping (RVA 0x27e110, mfc140u):
//     uiElemType = 0;  bHasExecFunc = FALSE;  return AFX_INVALID_DHTML_CMD_ID (-1);
// Symbol: ?GetDHtmlCommandMapping@CHtmlEditView@@UEAAIIAEAHAEAI@Z
extern "C" unsigned int MS_ABI impl__GetDHtmlCommandMapping_CHtmlEditView__UEAAIIAEAHAEAI_Z(
    void* pThis, unsigned int nIDWindowsCommand, int* bHasExecFunc, unsigned int* uiElemType) {
    (void)pThis;
    (void)nIDWindowsCommand;
    *uiElemType = 0;
    *bHasExecFunc = FALSE;
    return kInvalidDhtmlCmdId;
}

// Retail GetDHtmlDocument (RVA 0x27dfd0, mfc140u):
//     CComPtr<IDispatch> spDisp;
//     if (ppDocument == NULL) return FALSE;
//     *ppDocument = NULL;
//     BOOL bRet = FALSE;
//     if (SUCCEEDED(m_pBrowserApp->get_Document(&spDisp)) && spDisp != NULL)   // vslot 0x90/8 = 18
//         bRet = spDisp->QueryInterface(IID_IHTMLDocument2, (void**)ppDocument) == S_OK;  // `sete`
//     return bRet;                                 // spDisp released on every path that set it
// DEVIATION: retail does not null-check m_pBrowserApp (it faults before the
// browser exists); this returns FALSE instead.  m_pBrowser is OpenMFC's
// stand-in for m_pBrowserApp (see the file header).
// Symbol: ?GetDHtmlDocument@CHtmlEditView@@QEBAHPEAPEAUIHTMLDocument2@@@Z
extern "C" int MS_ABI impl__GetDHtmlDocument_CHtmlEditView__QEBAHPEAPEAUIHTMLDocument2___Z(
    const void* pThis, IHTMLDocument2** ppDocument) {
    if (ppDocument == nullptr)
        return FALSE;
    *ppDocument = nullptr;
    IWebBrowser2* pBrowser = static_cast<const CHtmlView*>(pThis)->m_pBrowser;
    if (pBrowser == nullptr)
        return FALSE;
    IDispatch* pDisp = nullptr;
    int bRet = FALSE;
    HRESULT hr = pBrowser->get_Document(&pDisp);
    if (SUCCEEDED(hr) && pDisp != nullptr)
        bRet = pDisp->QueryInterface(kIID_IHTMLDocument2, reinterpret_cast<void**>(ppDocument)) == S_OK;
    if (pDisp != nullptr)
        pDisp->Release();
    return bRet;
}

// Retail GetStartDocument: export ordinal 6701 resolves to RVA 0x27db70
// (mfc140u), shared by identical-code folding with CHtmlEditCtrl::GetStartDocument:
//     lea 0x18034f030 -> L"about:blank";  ret
// Symbol: ?GetStartDocument@CHtmlEditView@@UEAAPEB_WXZ
extern "C" const wchar_t* MS_ABI impl__GetStartDocument_CHtmlEditView__UEAAPEB_WXZ(void* pThis) {
    (void)pThis;
    return L"about:blank";
}

// Retail OnCmdMsg (RVA 0x27dcb0, mfc140u):
//     if (nCode < CN_UPDATE_COMMAND_UI) goto base;
//     BOOL bHasExecFunc = FALSE;  UINT uiElemType = 0;
//     UINT dhtmlCmdID = GetDHtmlCommandMapping(nID, bHasExecFunc, uiElemType);  // vslot 152
//     if (dhtmlCmdID == AFX_INVALID_DHTML_CMD_ID) goto base;
//     // inlined CHtmlEditCtrlBase::QueryStatus(dhtmlCmdID):
//     long nStatus = 0;
//     CComPtr<IHTMLDocument2> spDoc;  GetDHtmlDocument(&spDoc);                // direct call 0x27dfd0
//     if (spDoc) {
//         CComQIPtr<IOleCommandTarget> spCmdTarg = spDoc;                       // NULL on failure
//         if (spCmdTarg) {
//             OLECMD ocmd = { dhtmlCmdID, 0 };
//             if (spCmdTarg->QueryStatus(&CGID_MSHTML, 1, &ocmd, NULL) == S_OK)  // vslot 3
//                 nStatus = ocmd.cmdf;
//         }
//     }
//     if (nCode == CN_UPDATE_COMMAND_UI) {
//         CCmdUI* pCmdUI = (CCmdUI*)pExtra;
//         if (pCmdUI == NULL) return FALSE;
//         BOOL bLatched = nStatus & OLECMDF_LATCHED;                            // `and $0x4`
//         if (bLatched || (nStatus & OLECMDF_ENABLED)) {                        // `test $2`
//             pCmdUI->Enable(TRUE);                                             // CCmdUI vslot 0
//             if (uiElemType & AFX_UI_ELEMTYPE_CHECBOX) pCmdUI->SetCheck(bLatched ? 1 : 0);   // vslot 1
//             else if (uiElemType & AFX_UI_ELEMTYPE_RADIO) pCmdUI->SetRadio(bLatched ? 1 : 0); // vslot 2
//         } else {
//             pCmdUI->Enable(FALSE);
//             if (uiElemType & AFX_UI_ELEMTYPE_CHECBOX) pCmdUI->SetCheck(0);
//             else if (uiElemType & AFX_UI_ELEMTYPE_RADIO) pCmdUI->SetRadio(0);
//         }
//         return TRUE;
//     }
//     if (!(nStatus & (OLECMDF_ENABLED | OLECMDF_LATCHED))) return TRUE;       // `test $6`
//     if (bHasExecFunc) return ExecHandler(nID);                                // vslot 153
//     // inlined CHtmlEditCtrlBase::ExecCommand(dhtmlCmdID, OLECMDEXECOPT_DODEFAULT, NULL, NULL):
//     HRESULT hr = E_FAIL;  GetDHtmlDocument(&spDoc);
//     if (spDoc) {
//         QI(IID_IOleCommandTarget);
//         hr = spCmdTarg ? spCmdTarg->Exec(&CGID_MSHTML, dhtmlCmdID, 0, NULL, NULL)  // vslot 4
//                        : E_NOINTERFACE;
//     }
//     return hr == S_OK;
// base:
//     return CView::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);                 // direct call 0x277780
// The two virtual calls on `this` and the CCmdUI calls go through
// CallGetDHtmlCommandMapping / CallExecHandler / CmdUICall above; see their
// comments for how objects whose vptr lies in this image are handled (a
// DEVIATION from retail's plain slot dispatch, with a documented hazard).
// Symbol: ?OnCmdMsg@CHtmlEditView@@UEAAHIHPEAXPEAUAFX_CMDHANDLERINFO@@@Z
extern "C" int MS_ABI impl__OnCmdMsg_CHtmlEditView__UEAAHIHPEAXPEAUAFX_CMDHANDLERINFO___Z(
    void* pThis, unsigned int nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) {
    if (nCode < kCN_UPDATE_COMMAND_UI)
        return impl__OnCmdMsg_CView__UEAAHIHPEAXPEAUAFX_CMDHANDLERINFO___Z(
            static_cast<CView*>(pThis), nID, nCode, pExtra, pHandlerInfo);

    int bHasExecFunc = FALSE;
    unsigned int uiElemType = 0;
    const unsigned int dhtmlCmdID = CallGetDHtmlCommandMapping(pThis, nID, &bHasExecFunc, &uiElemType);
    if (dhtmlCmdID == kInvalidDhtmlCmdId)
        return impl__OnCmdMsg_CView__UEAAHIHPEAXPEAUAFX_CMDHANDLERINFO___Z(
            static_cast<CView*>(pThis), nID, nCode, pExtra, pHandlerInfo);

    // QueryStatus(dhtmlCmdID)
    DWORD nStatus = 0;
    {
        IHTMLDocument2* pDoc = nullptr;
        impl__GetDHtmlDocument_CHtmlEditView__QEBAHPEAPEAUIHTMLDocument2___Z(pThis, &pDoc);
        if (pDoc != nullptr) {
            if (IOleCommandTarget* pTarget = QueryCommandTarget(pDoc)) {
                OLECMD ocmd = { dhtmlCmdID, 0 };
                if (pTarget->QueryStatus(&kCGID_MSHTML, 1, &ocmd, nullptr) == S_OK)
                    nStatus = ocmd.cmdf;
                pTarget->Release();
            }
            pDoc->Release();
        }
    }

    if (nCode == kCN_UPDATE_COMMAND_UI) {
        CCmdUI* pCmdUI = static_cast<CCmdUI*>(pExtra);
        if (pCmdUI == nullptr)
            return FALSE;
        const bool bLatched = (nStatus & OLECMDF_LATCHED) != 0;
        if (bLatched || (nStatus & OLECMDF_ENABLED) != 0) {
            CmdUICall(pCmdUI, kSlotCmdUIEnable, TRUE);
            if (uiElemType & kElemTypeCheckBox)
                CmdUICall(pCmdUI, kSlotCmdUISetCheck, bLatched ? 1 : 0);
            else if (uiElemType & kElemTypeRadio)
                CmdUICall(pCmdUI, kSlotCmdUISetRadio, bLatched ? 1 : 0);
        } else {
            CmdUICall(pCmdUI, kSlotCmdUIEnable, FALSE);
            if (uiElemType & kElemTypeCheckBox)
                CmdUICall(pCmdUI, kSlotCmdUISetCheck, 0);
            else if (uiElemType & kElemTypeRadio)
                CmdUICall(pCmdUI, kSlotCmdUISetRadio, 0);
        }
        return TRUE;
    }

    if ((nStatus & (OLECMDF_ENABLED | OLECMDF_LATCHED)) == 0)
        return TRUE;
    if (bHasExecFunc)
        return CallExecHandler(pThis, nID);

    // ExecCommand(dhtmlCmdID, OLECMDEXECOPT_DODEFAULT, NULL, NULL)
    HRESULT hr = E_FAIL;
    IHTMLDocument2* pDoc = nullptr;
    impl__GetDHtmlDocument_CHtmlEditView__QEBAHPEAPEAUIHTMLDocument2___Z(pThis, &pDoc);
    if (pDoc != nullptr) {
        if (IOleCommandTarget* pTarget = QueryCommandTarget(pDoc)) {
            hr = pTarget->Exec(&kCGID_MSHTML, dhtmlCmdID, OLECMDEXECOPT_DODEFAULT, nullptr, nullptr);
            pTarget->Release();
        } else {
            hr = E_NOINTERFACE;
        }
        pDoc->Release();
    }
    return hr == S_OK;
}

// Retail OnNavigateComplete2: export ordinal 10541 resolves to RVA 0x27e060
// (mfc140u), which is also slot 128 of the CHtmlEditView vftable.  It is the
// inlined CHtmlEditCtrlBase::SetDesignMode(TRUE); strURL is not read and the
// CHtmlView base is not called:
//     CComPtr<IHTMLDocument2> spDoc;  GetDHtmlDocument(&spDoc);     // direct call 0x27dfd0
//     if (spDoc) {
//         CComBSTR bstr(L"On");         // SysAllocString (OLEAUT32 #2, IAT 0x1802c69c8)
//                                       // NULL -> AtlThrow(E_OUTOFMEMORY) -> AfxThrowMemoryException
//         spDoc->put_designMode(bstr);  // IHTMLDocument2 vslot 0x98/8 = 19
//     }                                 // SysFreeString (OLEAUT32 #6, IAT 0x1802c69d8)
// Minor difference: the document is released before the out-of-memory throw
// (retail releases it during unwinding).
// Symbol: ?OnNavigateComplete2@CHtmlEditView@@UEAAXPEB_W@Z
extern "C" void MS_ABI impl__OnNavigateComplete2_CHtmlEditView__UEAAXPEB_W_Z(void* pThis, const wchar_t* strURL) {
    (void)strURL;
    IHTMLDocument2* pDoc = nullptr;
    impl__GetDHtmlDocument_CHtmlEditView__QEBAHPEAPEAUIHTMLDocument2___Z(pThis, &pDoc);
    if (pDoc == nullptr)
        return;
    BSTR bstr = ::SysAllocString(L"On");
    if (bstr == nullptr) {
        pDoc->Release();
        impl__AfxThrowMemoryException__YAXXZ();
        return;
    }
    pDoc->put_designMode(bstr);
    ::SysFreeString(bstr);
    pDoc->Release();
}

// Retail OnPaint: export ordinal 10731 resolves to RVA 0xda30 (mfc140u), an
// identical-code-folded `jmp 0x28ac80` = CWnd::Default().
// Symbol: ?OnPaint@CHtmlEditView@@QEAAXXZ
extern "C" void MS_ABI impl__OnPaint_CHtmlEditView__QEAAXXZ(void* pThis) {
    impl__Default_CWnd__IEAA_JXZ(static_cast<CWnd*>(pThis));
}

// Retail OnPreparePrinting: export ordinal 10809 resolves to RVA 0x3a60
// (mfc140u), an identical-code-folded `mov $0x1,%eax; ret` -- it returns TRUE
// without calling DoPreparePrinting.
// Symbol: ?OnPreparePrinting@CHtmlEditView@@UEAAHPEAUCPrintInfo@@@Z
extern "C" int MS_ABI impl__OnPreparePrinting_CHtmlEditView__UEAAHPEAUCPrintInfo___Z(void* pThis, void* /*CPrintInfo**/ pInfo) {
    (void)pThis;
    (void)pInfo;
    return TRUE;
}
