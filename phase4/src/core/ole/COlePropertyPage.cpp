// COlePropertyPage — OpenMFC implementation.
// Sources: olecore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/OlecoreSupport.h"

// ---------------------------------------------------------------------------
// Retail decoding notes (mfc140.dll, RVAs quoted per function below).
//
// The retail COlePropertyPage carries a set of members that include/openmfc's
// declaration of the class does not have. Their retail offsets, established by
// disassembly, are:
//
//   0x130  CPtrArray        embedded, 0x28 bytes: vfptr 0x31efd0 (slot 0 of
//                           that table is ?GetThisClass@CPtrArray@@), then
//                           m_pData 0x138, m_nSize 0x140, m_nMaxSize 0x148,
//                           m_nGrowBy 0x150. The ctor (??0COlePropertyPage@
//                           0x1e6e20) stores the vtable and zeroes the four
//                           fields; the dtor (??1 0x1e7450) destroys it at
//                           0x1e7491. WHAT it holds was not established --
//                           nothing else disassembled here reads it.
//   0x158  BOOL             m_bDirty            (?SetModifiedFlag@ 0x1e7710)
//   0x15c  UINT             template id         (ctor arg 1; ?Activate@ reads
//                                                it at 0x1e7a8c)
//   0x160  UINT             caption id          (ctor arg 2, 0x1e6e73)
//   0x168  CString          m_strPageName       (?SetPageName@     0x1e7770)
//   0x170  SIZE             m_sizePage          (?SetDialogResource@0x1e77b0)
//   0x178  CString          doc string          (?GetPageInfo@XPropertyPage@
//                                                            0x1e7c60, -0x78)
//   0x180  CString          help file           (?GetPageInfo@ 0x1e7c60, -0x70)
//   0x188  DWORD            m_dwHelpContext     (?GetPageInfo@ 0x1e7c60, -0x68)
//   0x190  IPropertyPageSite* m_pPageSite       (?GetPageSite@     0x1e7890)
//   0x198  IDispatch**      m_ppDisp            (?GetObjectArray@  0x1e76f0)
//   0x1a0  DWORD*           m_pdwConnections    (?CleanupObjectArray@0x1e7380)
//   0x1a8  int              deferred-refill flg (?OnChanged@XPropNotifySink@
//                                                                   0x1e8370)
//   0x1ac  ULONG            m_nObjects          (?GetObjectArray@  0x1e76f0)
//   0x1b0  int              in-page-update flag (?Activate@XPropertyPage@
//                                                                   0x1e7a00)
//   0x1b4  int              m_nControls         (?GetControlStatus@0x1e8680)
//   0x1b8  {UINT,BOOL}*     m_pControlStatus    (?EnumControls@    0x1e7bc0)
//   0x1c0  CDWordArray      ignore-apply list   (?IgnoreApply@     0x1e8660;
//                           vfptr 0x31ef70, m_pData 0x1c8, m_nSize 0x1d0 --
//                           both read by ?OnCommand@ 0x1e843d/0x1e8469)
//                           CORRECTION (this file previously said CUIntArray):
//                           the embedded array's vptr is CDWordArray's vtable,
//                           not CUIntArray's. The ctor stores mfc140u 0x321130
//                           at 0x1c0; slot 0 of that table is
//                           ?GetRuntimeClass@CDWordArray@@ (0x1d4550), whose
//                           descriptor at 0x3b1838 spells "CDWordArray",
//                           object size 0x28. CUIntArray has its OWN vtable
//                           (0x321250, ??0CUIntArray@ 0x1d5600) and its own
//                           descriptor (0x321218), so the two are not the same
//                           object. The "CUIntArray" name came from the MBCS
//                           symbol map naming the ICF-folded SetAtGrow body --
//                           see the IgnoreApply note below.
//   0x1e8  HGLOBAL          m_hDialogTemplate   (?SetDialogResource@0x1e77b0)
//   0x1f0  XPropertyPage    nested interface    (?Move@XPropertyPage@0x1e7e60)
//   0x1f8  XPropNotifySink  nested interface    (?CleanupObjectArray@0x1e7380)
//
// Every offset in that table was re-checked instruction by instruction against
// the RVA named beside it, and every callee RVA and IAT slot quoted in this
// file was resolved through the symbol map / the import table of the SAME
// binary that was disassembled (mfc140.dll -- note that mfc140u.dll is also on
// this host and has a different IAT layout; do not mix them).
//
// Those rows account for every byte from 0x130 to the 0x200 end of the object
// except two alignment gaps, 0x164 and 0x18c; the CDWordArray at 0x1c0 runs to
// 0x1e8 (m_nMaxSize 0x1d8, m_nGrowBy 0x1e0, both zeroed by the ctor). Nothing
// is claimed about the CDialog part below 0x130.
//
// OpenMFC's COlePropertyPage lays its members out differently. Measured with
// offsetof under this file's own build flags (a temporary
// `template<int> struct Show; Show<offsetof(...)>` probe, since the compiler
// prints the value in the incomplete-type diagnostic):
//
//     member             OpenMFC   retail equivalent
//     sizeof(CDialog)    0x130     0x130   -- the bases agree exactly
//     m_pPageSite        0x130     0x190
//     m_bModified        0x138     0x158  (retail m_bDirty)
//     m_pszPageName      0x140     0x168  (retail CString m_strPageName)
//     m_pszHelpFile      0x148     0x180  (retail CString)
//     m_dwHelpContext    0x150     0x188
//     m_pszDocString     0x158     0x178  (retail CString)
//     padding[160]       0x160..0x200
//     sizeof(class)      0x200     0x200   -- agree
//
// So the six members OpenMFC does declare all sit in the 0x130..0x160 window
// that retail uses for its embedded CPtrArray plus m_bDirty and the template/
// caption ids -- every one of them at the wrong retail offset -- and OpenMFC
// declares none of the object-array, control-status, ignore-apply,
// dialog-template or nested-interface members at all. Those all live at
// >= 0x160, i.e. entirely inside _olepropertypage_padding, so they can be
// added without moving the class's size. Nothing in the tree static_asserts
// any COlePropertyPage offset (checked), so a re-lay is not blocked by an
// existing assertion.
//
// Every entry point below that needs one of the unmodelled members is
// therefore left as a stub with a note saying what the retail body does and
// what is missing; the ones that need only members OpenMFC already declares
// are implemented from the retail disassembly.
//
// ---------------------------------------------------------------------------
// Which retail binary the RVAs above refer to, and the Unicode cross-check.
//
// Unless a note says "mfc140u" outright, every RVA quoted above and in the
// per-function notes below is an offset into mfc140.dll -- the MBCS build,
// which is what the repo's mfc140_rva_symbols.json and the disassembly helper
// resolve against. (The exceptions are the mfc140u table below, the ?OnHelp@
// and ?OnRequestEdit@ notes, the ?IsModified@ note and the CDWordArray
// correction above, each of which names its binary.) OpenMFC actually
// reimplements mfc140u.dll, the Unicode build, and that file is on this host
// beside it. The two are NOT interchangeable: different RVAs, different IAT
// slots.
//
// The Unicode build was cross-checked directly (its ordinal->RVA export table
// joined against mfc_complete_ordinal_mapping.json, which is keyed by mfc140u
// ordinals). Results:
//
//   * Every COlePropertyPage entry point in this file resolves in mfc140u,
//     including the two the MBCS map has no row for. The MBCS map is a
//     one-symbol-per-RVA dictionary, so any export whose body was folded onto
//     an address another export already claimed simply drops out of it; that,
//     not absence from the image, is why ?OnHelp@ and ?OnRequestEdit@ looked
//     unresolvable. Both are resolved below.
//
//   * On COlePropertyPage's OWN members the two builds agree. Re-checked
//     instruction by instruction in mfc140u on the constructor (0x1e8ee0 --
//     which alone pins the whole table above: it stores the two array vptrs,
//     both sub-object vptrs at 0x1f0/0x1f8, and zeroes or seeds every scalar
//     from 0x130 to 0x1e8), GetObjectArray, GetPageSite, SetModifiedFlag,
//     SetHelpInfo, GetControlStatus, SetControlStatus, EnumChildProc,
//     EnumControls, IgnoreApply, SetDialogResource, OnCommand, OnCtlColor,
//     OnFinalRelease, OnInitDialog, PreTranslateMessage, WindowProc,
//     MessageBoxW, GetPropText(int&), SetPropText(BYTE&) and each of the
//     twelve nested-interface methods below, and every one matches the
//     transcription written beside it here: same member offsets
//     (0x158/0x15c/0x160/0x168/0x170/0x178/0x180/0x188/0x190/0x198/0x1a0/
//     0x1a8/0x1ac/0x1b0/0x1b4/0x1b8/0x1c0/0x1e8/0x1f0/0x1f8), same sub-object
//     adjustments (-0x1f0 / -0x1f8), same control flow.
//     sizeof is 0x200 in both -- read out of the retail CRuntimeClass
//     descriptors themselves (mfc140u 0x3248c0 and mfc140 0x322700, each
//     naming "COlePropertyPage" with m_nObjectSize 0x200 and schema 0xffff),
//     not inferred from the last member.
//
//     They do NOT agree on offsets into OTHER structures. The one that matters
//     here: XPropNotifySink::OnChanged compares against a field of
//     AFX_THREAD_STATE that is at +0x138 in mfc140.dll but at +0x198 in
//     mfc140u.dll. Do not carry a non-COlePropertyPage offset across builds
//     without re-reading it -- see the OnChanged note below.
//
//   * mfc140u RVAs for the entry points in this file, if you need to re-check
//     one in the build OpenMFC actually targets:
//       CleanupObjectArray 0x1e95e0   EnumChildProc      0x1e9e00
//       EnumControls       0x1e9e20   GetControlStatus   0x1ea8e0
//       GetObjectArray     0x1e9950   GetPageSite        0x1e9af0
//       IgnoreApply        0x1ea8c0   IsModified         0x1d02f0
//       MessageBoxW        0x1e9b00   OnCommand          0x1ea640
//       OnCtlColor         0x1e98e0   OnFinalRelease     0x1e97f0
//       OnInitDialog       0x1e9460   PreTranslateMessage 0x1e9470
//       SetControlStatus   0x1ea910   SetDialogResource  0x1e9a10
//       WindowProc         0x1e9830   OnHelp             0x0071e0
//       GetPropCheck       0x1ebf90   GetPropRadio       0x1ec240
//       GetPropIndex       0x1ec3c0 (jmp -> GetPropRadio)
//       GetPropText  BYTE& 0x1eaa80  short& 0x1ead30   int&    0x1eafe0
//                    UINT& 0x1eb150   long& 0x1eb2c0   DWORD&  0x1eb430
//                   float& 0x1eb6e0 double& 0x1eb9a0  CString& 0x1ebc60
//       SetPropCheck       0x1ebe50   SetPropRadio       0x1ec100
//       SetPropIndex       0x1ec3b0 (jmp -> SetPropRadio)
//       SetPropText  BYTE& 0x1ea940  short& 0x1eabf0
//                     int& / UINT& / long& / DWORD& all 0x1eaea0 -- one body,
//                     which is the Unicode build stating outright what the
//                     MBCS map could only imply by the three missing rows
//                   float& 0x1eb5a0 double& 0x1eb860  CString& 0x1ebb20
//       XPropertyPage: Activate 0x1e9c60  SetPageSite  0x1e9bd0
//                      Deactivate 0x1e9e70  GetPageInfo 0x1e9ec0
//                      SetObjects 0x1e9f20  Show 0x1ea080  Move 0x1ea0c0
//                      IsPageDirty 0x1ea100  Apply 0x1ea110  Help 0x1ea1d0
//                      TranslateAcceleratorW 0x1ea270  EditProperty 0x1ea500
//       XPropNotifySink: OnChanged 0x1ea5d0   OnRequestEdit 0x0071e0
// ---------------------------------------------------------------------------
//
// Independent re-verification pass (2026-09). Every claim this file makes
// about a symbol on the current assignment list was re-disassembled in
// mfc140u -- NOT re-read from these notes -- and each one held:
//   CleanupObjectArray 0x1e95e0, EnumChildProc 0x1e9e00, EnumControls
//   0x1e9e20, GetControlStatus 0x1ea8e0, SetControlStatus 0x1ea910,
//   IgnoreApply 0x1ea8c0, SetDialogResource 0x1e9a10, OnHelp 0x71e0,
//   GetPropText(int&) 0x1eafe0, GetPropCheck 0x1ebf90,
//   GetPropText(CString&) 0x1ebc60, SetPropText(BYTE&) 0x1ea940,
//   SetPropText(CString&) 0x1ebb20, GetPropIndex 0x1ec3c0 and SetPropIndex
//   0x1ec3b0 (each a single `jmp` to GetPropRadio 0x1ec240 / SetPropRadio
//   0x1ec100), and the nested-interface bodies Activate 0x1e9c60,
//   SetPageSite 0x1e9bd0, Deactivate 0x1e9e70, GetPageInfo 0x1e9ec0,
//   SetObjects 0x1e9f20, Show 0x1ea080, Move 0x1ea0c0, IsPageDirty 0x1ea100,
//   Apply 0x1ea110, Help 0x1ea1d0, EditProperty 0x1ea500,
//   OnChanged 0x1ea5d0. Nothing was found to correct.
// Two details worth pinning while they were in front of me:
//   * The constructor (mfc140u 0x1e8ee0) stores 0x321130 into 0x1c0 -- the
//     CDWordArray correction below is confirmed at its source -- writes
//     0x1b0 as a QWORD `movq $0x1,0x1b0(%rsi)`, which sets the 0x1b0 flag to
//     1 and zeroes m_nControls at 0x1b4 in one store, and installs the two
//     sub-object vptrs 0x324928 (XPropertyPage) at 0x1f0 and 0x3248f8
//     (XPropNotifySink) at 0x1f8. It also mirrors the low word of the
//     template id at 0x15c into 0xf0 (`movzwl 0x15c(%rsi),%eax; mov
//     %rax,0xf0(%rsi)`) -- that is inside the CDialog part, and is the only
//     claim this file makes about it.
//   * The OpenMFC-side offset table further down was not taken on trust
//     either: it was re-measured under this file's own build flags with
//     static_assert(offsetof(...)) on all six declared members plus
//     sizeof(CDialog) == 0x130 and sizeof(COlePropertyPage) == 0x200, and
//     every value in that table compiled clean.
//
// A recurring reduction is used below and is worth stating once. Nothing in
// OpenMFC ever populates the object array: its only writer is
// XPropertyPage::SetObjects (retail 0x1e7cc0), which allocates m_ppDisp/
// m_pdwConnections and sets m_nObjects, and that entry point is a stub here
// because the XPropertyPage sub-object does not exist. So m_ppDisp is
// permanently NULL and m_nObjects permanently 0, and any retail body whose
// whole effect is a `for (i = 0; i < m_nObjects; ++i)` loop reduces to its
// zero-iteration result. Where that is what the generated stub already
// returns, the stub is left alone with a comment saying so; where it is not,
// it is corrected.
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
extern "C" HWND MS_ABI impl__GetSafeOwner__CWnd__SAPEAUHWND____PEAU2_PEAPEAU2__Z(HWND hParent, HWND* pWndTop);
extern "C" LRESULT MS_ABI impl__WindowProc_CWnd__MEAA_JI_K_J_Z(CWnd* pThis, UINT message, WPARAM wParam, LPARAM lParam);
extern "C" int MS_ABI impl__OnCommand_CWnd__MEAAH_K_J_Z(CWnd* pThis, WPARAM wParam, LPARAM lParam);

COlePropertyPage::COlePropertyPage()
    : CDialog(), m_pPageSite(nullptr), m_bModified(FALSE),
      m_pszPageName(nullptr), m_pszHelpFile(nullptr),
      m_dwHelpContext(0), m_pszDocString(nullptr) {
    memset(_olepropertypage_padding, 0, sizeof(_olepropertypage_padding));
}
COlePropertyPage::COlePropertyPage(UINT nIDTemplate, UINT nIDCaption)
    : CDialog(nIDTemplate), m_pPageSite(nullptr), m_bModified(FALSE),
      m_pszPageName(nullptr), m_pszHelpFile(nullptr),
      m_dwHelpContext(0), m_pszDocString(nullptr) {
    memset(_olepropertypage_padding, 0, sizeof(_olepropertypage_padding));
}
COlePropertyPage::~COlePropertyPage() {
}
void COlePropertyPage::SetPageName(const wchar_t* pszName) {
    m_pszPageName = (wchar_t*)pszName;
}
void COlePropertyPage::SetHelpInfo(const wchar_t* lpszDocString, const wchar_t* lpszHelpFile, DWORD dwHelpContext) {
    m_pszDocString = (wchar_t*)lpszDocString;
    m_pszHelpFile = (wchar_t*)lpszHelpFile;
    m_dwHelpContext = dwHelpContext;
}
BOOL COlePropertyPage::OnSetPageSite() {
    return TRUE;
}
// DIVERGENCE, recorded not fixed -- OnObjectsChanged is not on this file's
// assignment list, and changing it changes observable behaviour, so it is left
// for whoever owns it. Retail's ?OnObjectsChanged@COlePropertyPage@@UEAAXXZ is
// mfc140u ordinal -> RVA 0x27d0, and the body at 0x27d0 is a bare `ret`: the
// base implementation does NOTHING. (0x27d0 is a folded COMDAT shared by 158
// void exports; the folding is by identical code, so "empty" is exact, but the
// name the symbol map attaches to that address says nothing about this one.)
// The body below instead calls SetModifiedFlag(TRUE), which drives
// IPropertyPageSite::OnStatusChange(PROPPAGESTATUS_DIRTY) at a point where
// retail is silent -- and XPropertyPage::SetObjects (0x1e9f20 / MBCS 0x1e7cc0)
// calls this virtual unconditionally on every SetObjects, so under retail
// semantics a page would be marked dirty merely by being handed its objects.
void COlePropertyPage::OnObjectsChanged() {
    SetModifiedFlag(TRUE);
}
BOOL COlePropertyPage::OnApply() {
    return TRUE;
}
BOOL COlePropertyPage::OnEditProperty(DISPID dispid) {
    (void)dispid;
    return FALSE;
}
// Pre-existing body, left as it is; recording what retail (0x1e7710) actually
// does so nobody has to re-derive it. Retail is EDGE-triggered and also drains
// the deferral flag:
//     if (!bModified) *(int*)(this+0x1a8) = 0;
//     if ((m_bDirty /*0x158*/ != 0) == (bModified != 0)) return;  ; no change
//     m_bDirty = bModified;
//     if (m_pPageSite /*0x190*/)
//         m_pPageSite->OnStatusChange(bModified ? PROPPAGESTATUS_DIRTY : 0);
// The body below notifies on every call rather than only on a transition, and
// has no 0x1a8 to clear. Not on this file's assignment list, so not changed.
void COlePropertyPage::SetModifiedFlag(BOOL bModified) {
    m_bModified = bModified ? TRUE : FALSE;
    if (m_pPageSite) {
        m_pPageSite->OnStatusChange(bModified ? PROPPAGESTATUS_DIRTY : 0);
    }
}
// COlePropertyPage::CleanupObjectArray() -- retail (0x1e7380):
//     if (m_pdwConnections /*0x1a0*/) {
//         for (i = 0; i < m_nObjects /*0x1ac*/; ++i)
//             AfxConnectionUnadvise(m_ppDisp[i], IID_IPropertyNotifySink,
//                                   &this->m_xPropNotifySink /*0x1f8*/,
//                                   FALSE, m_pdwConnections[i]);   ; 0x1dd290
//         free(m_pdwConnections); m_pdwConnections = NULL;
//     }
//     if (m_ppDisp /*0x198*/) {
//         for (i = 0; i < m_nObjects; ++i) release(&m_ppDisp[i]);  ; 0x26ba84
//         free(m_ppDisp); m_ppDisp = NULL;
//     }
// Both pointers are permanently NULL here (see the note at the top of this
// file), so the retail body's whole effect is nothing and the empty stub is
// already the faithful reduction. Left empty deliberately, not by omission.
// The members themselves are a headerRequest.
// Symbol: ?CleanupObjectArray@COlePropertyPage@@IEAAXXZ
extern "C" void MS_ABI impl__CleanupObjectArray_COlePropertyPage__IEAAXXZ(
    COlePropertyPage* /*pThis*/) {}

// COlePropertyPage::EnumChildProc(HWND, LPARAM) -- retail (0x1e7ba0) is a
// static EnumChildWindows callback:
//     if (lParam == 0) AfxThrowInvalidArgException();   ; 0x225b80
//     ++((COlePropertyPage*)lParam)->m_nControls;       ; 0x1b4
//     return TRUE;
// It is the counting pass that sizes m_pControlStatus. m_nControls is not
// declared by OpenMFC (headerRequest), so the count is not kept -- but the
// return value is the enumeration-continue flag and retail always returns
// TRUE, so returning FALSE (what the generated stub did) would truncate any
// enumeration this is ever passed to. The TRUE is transcribed; the increment
// is what is missing.
// Deviation, recorded: retail's lParam == 0 arm throws
// AfxThrowInvalidArgException (mfc140u 0x227720) and never returns; the body
// below returns TRUE for a NULL lParam like any other. Re-verified against
// mfc140u 0x1e9e00. EnumControls (0x1e9e20) has no such test at all -- it
// dereferences lParam straight away -- so there is no matching deviation
// there.
// Symbol: ?EnumChildProc@COlePropertyPage@@KAHPEAUHWND__@@_J@Z
extern "C" int MS_ABI impl__EnumChildProc_COlePropertyPage__KAHPEAUHWND_____J_Z(HWND /*hWnd*/, __int64 /*lParam*/) {
    return TRUE;
}

// COlePropertyPage::EnumControls(HWND, LPARAM) -- retail (0x1e7bc0), the
// second EnumChildWindows pass, filling the table EnumChildProc sized:
//     COlePropertyPage* p = (COlePropertyPage*)lParam;
//     p->m_pControlStatus[p->m_nControls].nID    = ::GetDlgCtrlID(hWnd);
//     p->m_pControlStatus[p->m_nControls].bDirty = FALSE;   ; entries are 8
//     ++p->m_nControls;                                     ; bytes: UINT,BOOL
//     return TRUE;
// m_nControls (0x1b4) and m_pControlStatus (0x1b8) are not declared by
// OpenMFC (headerRequest), so the table cannot be filled. As with
// EnumChildProc only the TRUE continue-flag is transcribed.
// Symbol: ?EnumControls@COlePropertyPage@@KAHPEAUHWND__@@_J@Z
extern "C" int MS_ABI impl__EnumControls_COlePropertyPage__KAHPEAUHWND_____J_Z(HWND /*hWnd*/, __int64 /*lParam*/) {
    return TRUE;
}

// COlePropertyPage::GetControlStatus(UINT nID) -- retail (0x1e8680):
//     for (i = 0; i < m_nControls /*0x1b4*/; ++i)
//         if (m_pControlStatus /*0x1b8*/ [i].nID == nID)
//             return m_pControlStatus[i].bDirty;
//     return TRUE;                                  ; not found => TRUE
// The not-found answer is TRUE, not FALSE: MFC treats a control it is not
// tracking as one whose value must be written back. m_pControlStatus is only
// ever built by XPropertyPage::Activate (0x1e7a00) via the two EnumControls
// passes, and Activate is a stub here, so m_nControls is permanently 0 and the
// loop never runs. TRUE is therefore the retail answer for every nID in this
// build; the generated stub's FALSE was the opposite. Tracking the table is a
// headerRequest.
// Symbol: ?GetControlStatus@COlePropertyPage@@QEAAHI@Z
extern "C" int MS_ABI impl__GetControlStatus_COlePropertyPage__QEAAHI_Z(
    COlePropertyPage* /*pThis*/, unsigned int /*nID*/) {
    return TRUE;
}

// COlePropertyPage::GetObjectArray(ULONG* pnObjects) -- retail (0x1e76f0) is
// five instructions:
//     if (pnObjects) *pnObjects = m_nObjects;   ; 0x1ac, a DWORD store
//     return m_ppDisp;                          ; 0x198
// Neither member is declared by OpenMFC (headerRequest) and neither is ever
// populated here, so the pair is permanently (0, NULL). The count store is
// what matters: callers pass the address of an uninitialised ULONG and read it
// back, and the generated stub returned NULL without writing it, leaving that
// variable indeterminate. Writing the 0 is the retail behaviour for this
// object state.
// Symbol: ?GetObjectArray@COlePropertyPage@@QEAAPEAPEAUIDispatch@@PEAK@Z
extern "C" void* MS_ABI impl__GetObjectArray_COlePropertyPage__QEAAPEAPEAUIDispatch__PEAK_Z(
    COlePropertyPage* /*pThis*/, unsigned long* pnObjects) {
    if (pnObjects != nullptr) {
        *pnObjects = 0;
    }
    return nullptr;
}

// COlePropertyPage::GetPageSite() — retail (0x1e7890) is two instructions:
//     mov 0x190(%rcx),%rax ; ret
// i.e. it returns the IPropertyPageSite* member and nothing else. That member
// is m_pPageSite in the OpenMFC declaration.
// Symbol: ?GetPageSite@COlePropertyPage@@QEAAPEAUIPropertyPageSite@@XZ
extern "C" void* MS_ABI impl__GetPageSite_COlePropertyPage__QEAAPEAUIPropertyPageSite__XZ(
    COlePropertyPage* pThis) {
    return pThis ? pThis->m_pPageSite : nullptr;
}

// ---------------------------------------------------------------------------
// GetPropCheck / GetPropIndex / GetPropRadio / GetPropText(9 overloads)
//
// The DLL the disassembly helper resolves against is the MBCS build, so under
// that binary these resolve only under their ...PEBD... names (the Unicode
// RVAs, under the exact ...PEB_W... names this file exports, are in the binary
// note at the top of the file, and the bodies were confirmed identical):
// GetPropCheck 0x1e9d30, GetPropRadio 0x1e9fe0,
// GetPropText 0x1e8820/0x1e8ad0/0x1e8d80/0x1e8ef0/0x1e9060/0x1e91d0/0x1e9480/
// 0x1e9740/0x1e9a00. ?GetPropIndex@ (0x1ea160) is not a separate body at all:
// it is a one-instruction `jmp 0x1801e9fe0`, i.e. an alias of GetPropRadio.
//
// All of them share one shape (transcribed from GetPropText(int&), 0x1e8d80):
//     BOOL bRead = FALSE;
//     for (i = 0; i < m_nObjects /*0x1ac*/; ++i) {
//         IDispatch* p = m_ppDisp /*0x198*/ [i];
//         if (FAILED(p->GetIDsOfNames(...)))    ; IDispatch vtable slot 5
//             continue;                         ; property absent on object i
//         <read the property through the dispatch helper at 0x250e50/0x251720>
//         if (i == 0) *pValue = v;
//         else if (*pValue != v) *pValue = <differing-values sentinel>;
//         bRead = TRUE;
//     }
//     return bRead;                             ; the register is ESI
// -- a multi-object merge that reports "the objects disagree" by writing a
// sentinel into the out-parameter.
//
// m_ppDisp/m_nObjects are not declared by OpenMFC (headerRequest) and are
// permanently NULL/0 here, so the loop never runs: retail writes nothing to
// the out-parameter and returns FALSE. That is exactly what the generated
// stubs below already do, so they are left as they are -- verified equivalent
// for this object state, not skipped.
//
// Signatures: the generated stubs had no leading `COlePropertyPage* pThis`,
// and the CStringT overload's was generated nonsense (four parameters). They
// have been corrected below to the register assignment read out of the retail
// bodies -- `this` in RCX, the property name in RDX, the out-parameter in R8
// (checked on GetPropText(int&) 0x1eafe0, GetPropCheck 0x1ebf90 and
// GetPropText(CString&) 0x1ebc60, all mfc140u). No body reads an argument, so
// the correction changes no behaviour; it exists so the first real body is not
// written against a prototype that is off by one register.
// ---------------------------------------------------------------------------

// Symbol: ?GetPropCheck@COlePropertyPage@@QEAAHPEB_WPEAH@Z
extern "C" int MS_ABI impl__GetPropCheck_COlePropertyPage__QEAAHPEB_WPEAH_Z(
    COlePropertyPage* /*pThis*/, const wchar_t* /*pszPropName*/, int* /*pValue*/) {
    return 0;
}

// Symbol: ?GetPropIndex@COlePropertyPage@@QEAAHPEB_WPEAH@Z
extern "C" int MS_ABI impl__GetPropIndex_COlePropertyPage__QEAAHPEB_WPEAH_Z(
    COlePropertyPage* /*pThis*/, const wchar_t* /*pszPropName*/, int* /*pValue*/) {
    return 0;
}

// Symbol: ?GetPropRadio@COlePropertyPage@@QEAAHPEB_WPEAH@Z
extern "C" int MS_ABI impl__GetPropRadio_COlePropertyPage__QEAAHPEB_WPEAH_Z(
    COlePropertyPage* /*pThis*/, const wchar_t* /*pszPropName*/, int* /*pValue*/) {
    return 0;
}

// Symbol: ?GetPropText@COlePropertyPage@@QEAAHPEB_WPEAE@Z
extern "C" int MS_ABI impl__GetPropText_COlePropertyPage__QEAAHPEB_WPEAE_Z(
    COlePropertyPage* /*pThis*/, const wchar_t* /*pszPropName*/, unsigned char* /*pValue*/) {
    return 0;
}

// Symbol: ?GetPropText@COlePropertyPage@@QEAAHPEB_WPEAF@Z
extern "C" int MS_ABI impl__GetPropText_COlePropertyPage__QEAAHPEB_WPEAF_Z(
    COlePropertyPage* /*pThis*/, const wchar_t* /*pszPropName*/, short* /*pValue*/) {
    return 0;
}

// Symbol: ?GetPropText@COlePropertyPage@@QEAAHPEB_WPEAH@Z
extern "C" int MS_ABI impl__GetPropText_COlePropertyPage__QEAAHPEB_WPEAH_Z(
    COlePropertyPage* /*pThis*/, const wchar_t* /*pszPropName*/, int* /*pValue*/) {
    return 0;
}

// Symbol: ?GetPropText@COlePropertyPage@@QEAAHPEB_WPEAI@Z
extern "C" int MS_ABI impl__GetPropText_COlePropertyPage__QEAAHPEB_WPEAI_Z(
    COlePropertyPage* /*pThis*/, const wchar_t* /*pszPropName*/, unsigned int* /*pValue*/) {
    return 0;
}

// Symbol: ?GetPropText@COlePropertyPage@@QEAAHPEB_WPEAJ@Z
extern "C" int MS_ABI impl__GetPropText_COlePropertyPage__QEAAHPEB_WPEAJ_Z(
    COlePropertyPage* /*pThis*/, const wchar_t* /*pszPropName*/, long* /*pValue*/) {
    return 0;
}

// Symbol: ?GetPropText@COlePropertyPage@@QEAAHPEB_WPEAK@Z
extern "C" int MS_ABI impl__GetPropText_COlePropertyPage__QEAAHPEB_WPEAK_Z(
    COlePropertyPage* /*pThis*/, const wchar_t* /*pszPropName*/, unsigned long* /*pValue*/) {
    return 0;
}

// Symbol: ?GetPropText@COlePropertyPage@@QEAAHPEB_WPEAM@Z
extern "C" int MS_ABI impl__GetPropText_COlePropertyPage__QEAAHPEB_WPEAM_Z(
    COlePropertyPage* /*pThis*/, const wchar_t* /*pszPropName*/, float* /*pValue*/) {
    return 0;
}

// Symbol: ?GetPropText@COlePropertyPage@@QEAAHPEB_WPEAN@Z
extern "C" int MS_ABI impl__GetPropText_COlePropertyPage__QEAAHPEB_WPEAN_Z(
    COlePropertyPage* /*pThis*/, const wchar_t* /*pszPropName*/, double* /*pValue*/) {
    return 0;
}

// Symbol: ?GetPropText@COlePropertyPage@@QEAAHPEB_WPEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@@Z
extern "C" int MS_ABI impl__GetPropText_COlePropertyPage__QEAAHPEB_WPEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
    COlePropertyPage* /*pThis*/, const wchar_t* /*pszPropName*/, void* /*CString& strValue*/) {
    return 0;
}

// COlePropertyPage::IgnoreApply(UINT nID) -- retail (0x1e8660) is four
// instructions and a tail jump:
//     CDWordArray* a = &this->m_dwaIgnoreApply;  ; 0x1c0
//     CDWordArray::SetAtGrow(a, a->m_nSize /*+0x10*/, nID);  ; a tail jmp --
//         ; i.e. an inlined Add()
// i.e. it appends nID to an array of control ids whose changes must not mark
// the page dirty.
// On the element type -- this file used to call the array a CUIntArray, from
// the name the MBCS map put on the tail-jump target (0x1d21f0,
// ?SetAtGrow@CUIntArray@@QEAAX_JI@Z). That name is NOT evidence: in mfc140u
// the identical body sits at 0x1d4260 and BOTH ?SetAtGrow@CUIntArray@@ and
// ?SetAtGrow@CDWordArray@@QEAAX_JK@Z export to it -- one ICF-folded body for
// two four-byte-element arrays. The vptr settles it instead: the constructor
// (mfc140u 0x1e8ee0) stores 0x321130 at 0x1c0, whose slot 0 is
// ?GetRuntimeClass@CDWordArray@@ returning the descriptor named "CDWordArray";
// CUIntArray's vtable is a different object at 0x321250. So the member is a
// CDWordArray.
// The array is not declared by OpenMFC (headerRequest), so there is nowhere to
// record the id and this stays a no-op. Nothing else in this file reads such a
// list, so the omission is self-consistent: no control is ignored.
// The generated signature had no leading COlePropertyPage* -- its `p0` was
// really `this` in RCX and the nID is in RDX; corrected below. Both are still
// ignored, so the no-op is unaffected.
// Symbol: ?IgnoreApply@COlePropertyPage@@QEAAXI@Z
extern "C" void MS_ABI impl__IgnoreApply_COlePropertyPage__QEAAXI_Z(
    COlePropertyPage* /*pThis*/, unsigned int /*nID*/) {}

// COlePropertyPage::IsModified() -- this file used to say the export "is NOT
// in the retail RVA map, so it was not disassembled". That was wrong: it is
// absent only from the MBCS map (0x1d02f0 there is claimed by
// ?IsWindows7@CWinApp@@, which ICF folded onto the same two instructions).
// In mfc140u it resolves under its own name at RVA 0x1d02f0 and is, in full:
//     mov 0x158(%rcx),%eax ; ret
// i.e. it returns the dirty flag RAW -- not normalised to TRUE/FALSE. That is
// the same 0x158 word retail's SetModifiedFlag (0x1e7710 / mfc140u 0x1e9970)
// writes and hands to IPropertyPageSite::OnStatusChange. OpenMFC's member for
// that role is m_bModified, and the inline COlePropertyPage::IsModified in
// include/openmfc/afxole.h returns it raw as well, so this thunk does too;
// the earlier `? TRUE : FALSE` here disagreed with both retail and that
// inline for any value other than 0/1 (SetModified(2) is reachable, since the
// inline SetModified stores its argument unchanged).
// Symbol: ?IsModified@COlePropertyPage@@QEAAHXZ
extern "C" int MS_ABI impl__IsModified_COlePropertyPage__QEAAHXZ(COlePropertyPage* pThis) {
    return pThis ? pThis->m_bModified : FALSE;
}

// COlePropertyPage::MessageBox(LPCTSTR, LPCTSTR, UINT) — retail (0x1e78a0,
// the MBCS build's ?MessageBoxA@COlePropertyPage@@QEAAHPEBD0I@Z):
//     if (lpszCaption == NULL) lpszCaption = *(this+0x168)   ; m_strPageName
//     hWnd = CWnd::GetSafeOwner_(*(this+0x40) /*m_hWnd*/, NULL)
//     tail-jmp ::MessageBox(hWnd, lpszText, lpszCaption, nType)
// The retail default caption is the page-name string set by SetPageName; the
// OpenMFC member SetPageName writes is m_pszPageName. One deviation follows
// from that substitution and is deliberate: retail's 0x168 is a CString, so
// an unset page name yields a pointer to "" and the box gets an empty caption,
// whereas OpenMFC's m_pszPageName is NULL until SetPageName is called, so an
// unset page name reaches ::MessageBoxW as NULL and the system supplies its
// default caption. Both dereference safely; only the caption text differs.
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

// COlePropertyPage::OnCommand(WPARAM, LPARAM) -- retail (0x1e83e0):
//     BOOL bRet = CWnd::OnCommand(wParam, lParam);      ; 0x28c4c0, first
//     if (*(int*)(this+0x1b0) != 0) return bRet;
//     if (m_ppDisp /*0x198*/ == NULL) return bRet;
//     if (lParam == 0 /*not a control notification*/) return bRet;
//     ... scan m_uaIgnoreApply (m_pData 0x1c8, m_nSize 0x1d0) for
//         LOWORD(wParam); if found, return bRet. Otherwise classify the
//         control by class name -- ::GetClassName into a 0x64-character
//         buffer, then CompareString against two 8-entry, 16-byte-stride
//         {const char* class; WORD notification} tables at 0x3455d0 and
//         0x345550, plus a further CompareString against the class name
//         "button" at 0x345508 (radio-button groups are then resolved with
//         ::GetWindowLong GWL_STYLE / WS_GROUP and ::GetWindow GW_HWNDPREV) --
//         and by notification code, set m_bDirty (0x158) and call
//         SetControlStatus(nID, ...) (0x1e86b0), then notify the page site
//         through m_pPageSite->OnStatusChange (0x190, vtable slot 3).
//     return bRet;                                      ; always the base's
// Every branch past the first two tests needs members OpenMFC does not
// declare, and the second test alone settles it: m_ppDisp is permanently NULL
// here, so retail returns CWnd::OnCommand's result unchanged. (The retail
// constructor at 0x1e6e20 also initialises the dword at 0x1b0 to 1, which
// would short-circuit at the first test, but the m_ppDisp test is the one that
// holds unconditionally for OpenMFC.) The base dispatch is what the generated
// `return 0` was dropping: WM_COMMAND would stop reaching the message map.
// Symbol: ?OnCommand@COlePropertyPage@@MEAAH_K_J@Z
extern "C" int MS_ABI impl__OnCommand_COlePropertyPage__MEAAH_K_J_Z(
    COlePropertyPage* pThis, unsigned __int64 wParam, __int64 lParam) {
    if (!pThis) return FALSE;
    return impl__OnCommand_CWnd__MEAAH_K_J_Z(static_cast<CWnd*>(pThis),
                                             static_cast<WPARAM>(wParam),
                                             static_cast<LPARAM>(lParam));
}

// COlePropertyPage::OnCtlColor(CDC*, CWnd*, UINT) — retail (0x1e7680):
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
// Two deliberate deviations from that transcription, both defensive: retail
// THROWS on pWnd == NULL (AfxThrowInvalidArgException, 0x225b80) where this
// returns NULL, and retail never null-checks CWnd::GetCurrentMessage()'s
// result where this does. Neither changes the behaviour of any well-formed
// call; both turn a retail access violation into a benign return.
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

// COlePropertyPage::OnFinalRelease() — retail (0x1e7590):
//     if (*(this+0x40) /*m_hWnd*/ != 0) this->vtbl[0xd0/8]()   ; CWnd::DestroyWindow
//     tail-jmp this->vtbl[1](this, 1)                          ; deleting destructor
// i.e. destroy the window if one is up, then delete the page. Slot 0xd0 was
// read directly out of a CWnd-derived vtable in the retail image (slot 26 =
// ?DestroyWindow@CWnd@@UEAAHXZ, 0x289f70) and slot 1 is the scalar deleting
// destructor, which `delete pThis` reproduces through the virtual ~CObject.
// Deviation: retail dispatches DestroyWindow VIRTUALLY; the thunk called here
// is CWnd::DestroyWindow specifically, so an application override of
// DestroyWindow on a derived page would be bypassed. That is a property of
// this DLL's impl__ thunk model, not of this function.
// Symbol: ?OnFinalRelease@COlePropertyPage@@MEAAXXZ
extern "C" void MS_ABI impl__OnFinalRelease_COlePropertyPage__MEAAXXZ(COlePropertyPage* pThis) {
    if (!pThis) return;
    if (pThis->m_hWnd != nullptr) {
        impl__DestroyWindow_CWnd__UEAAHXZ(static_cast<CWnd*>(pThis));
    }
    delete pThis;
}

// COlePropertyPage::OnHelp(LPCTSTR) -- resolved in the Unicode build (see the
// binary note at the top of this file). ?OnHelp@COlePropertyPage@@UEAAHPEB_W@Z
// is mfc140u ordinal 10068 -> RVA 0x71e0, and the body at 0x71e0 is, in full:
//     xor %eax,%eax ; ret
// i.e. `return FALSE;` unconditionally. It never touches `this`, never reads
// the help directory it is handed, and never opens a help file: the base
// COlePropertyPage declines help outright and leaves it to the derived page
// to override. m_pszHelpFile / m_dwHelpContext are NOT consulted here -- the
// only retail reader of the help members is XPropertyPage::GetPageInfo
// (0x1e9ec0 / MBCS 0x1e7c60), which reports them to the property frame.
//
// This also settles what the caller does with it. XPropertyPage::Help
// (0x1ea1d0 / MBCS 0x1e7f70) invokes the outer through vtable offset 0x338 and
// maps the result with `test eax,eax; sete bl`, so a FALSE here becomes
// S_FALSE -- "no help shown" -- which is the correct answer for a page that
// has no help of its own.
//
// 0x71e0 is a folded COMDAT: 134 exports of this DLL share that address
// (re-counted from the mfc140u export table), every one of them a trivial
// `return 0` -- ?OnEditProperty@COlePropertyPage@@ among them, which is the
// value the C++ OnEditProperty above already returns. The folding is by
// identical code, so the behaviour it pins down for this slot is exact; the
// fact that some other export's name also maps to 0x71e0 is not evidence about
// this one and is not relied on. What IS evidence is the export directory: the
// ordinal this DLL must publish for ?OnHelp@ points at 0x71e0, so that is the
// code a caller of this export reaches.
// Implemented rather than stubbed: the retail body needs nothing this build
// lacks. The generated prototype had no leading COlePropertyPage*, so its
// `const wchar_t*` was really `this` in RCX; the signature below has been
// corrected to (this, lpszHelpDir). Neither parameter is read -- which is
// exactly why this one is safe to implement -- so do not start using them.
// Symbol: ?OnHelp@COlePropertyPage@@UEAAHPEB_W@Z
extern "C" int MS_ABI impl__OnHelp_COlePropertyPage__UEAAHPEB_W_Z(
    COlePropertyPage* /*pThis*/, const wchar_t* /*lpszHelpDir*/) {
    return FALSE;
}

// COlePropertyPage::OnInitDialog() — retail (0x1e7200) is four instructions:
//     call ?OnInitDialog@CDialog@@UEAAHXZ ; xor %eax,%eax ; ret
// It calls the CDialog base and then returns FALSE (the page must not steal
// the focus from the property frame), discarding the base's return value.
// Symbol: ?OnInitDialog@COlePropertyPage@@UEAAHXZ
extern "C" int MS_ABI impl__OnInitDialog_COlePropertyPage__UEAAHXZ(COlePropertyPage* pThis) {
    if (!pThis) return FALSE;
    (void)impl__OnInitDialog_CDialog__UEAAHXZ(static_cast<CDialog*>(pThis));
    return FALSE;
}

// COlePropertyPage::PreTranslateMessage(MSG*) — retail (0x1e7210):
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
//                             szClass, -1, "EDIT" /*0x33fbe4*/, -1) == CSTR_EQUAL) {
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
// (once before the VK_RETURN retest); only one call is kept here — the second
// is a pure repeat with no additional effect beyond the handle-map lookup the
// first already performed.
// Deviation: retail throws AfxThrowInvalidArgException (0x225b80) on a NULL
// pMsg; this returns FALSE instead. The MBCS retail body uses ::GetClassNameA
// / ::CompareStringA against the char string "EDIT" at 0x33fbe4 with a
// 10-character buffer; the wide equivalents with the same count are used here.
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
// (0x1e86b0), the mirror of GetControlStatus:
//     for (i = 0; i < m_nControls /*0x1b4*/; ++i)
//         if (m_pControlStatus /*0x1b8*/ [i].nID == nID) {
//             m_pControlStatus[i].bDirty = bDirty;
//             return TRUE;
//         }
//     return FALSE;                                 ; not found => FALSE
// Note the asymmetry with GetControlStatus, whose not-found answer is TRUE.
// The table is never built here (m_nControls is permanently 0), so retail
// falls straight to the FALSE the generated stub already returns; it is left
// alone as the verified reduction. The members are a headerRequest.
// Symbol: ?SetControlStatus@COlePropertyPage@@QEAAHIH@Z
extern "C" int MS_ABI impl__SetControlStatus_COlePropertyPage__QEAAHIH_Z(
    COlePropertyPage* /*pThis*/, unsigned int /*nID*/, int /*bDirty*/) {
    return FALSE;
}

// COlePropertyPage::SetDialogResource(HGLOBAL hDialog) -- retail (0x1e77b0):
//     if (m_hDialogTemplate /*0x1e8*/) {
//         ::GlobalFree(m_hDialogTemplate);          ; import at 0x2c4708
//         m_hDialogTemplate = NULL;
//     }
//     <build a template wrapper on the stack from hDialog>          ; 0x218330
//     <measure it into m_sizePage /*0x170*/>                        ; 0x218bb0
//     m_hDialogTemplate = <the wrapper's handle>;
// Both members it writes are absent from OpenMFC's declaration
// (headerRequest), and m_sizePage is the value XPropertyPage::GetPageInfo
// (0x1e7c60) reports to the property frame, so storing it somewhere this file
// cannot read back would be pointless. Left a no-op: the page keeps whatever
// template CDialog was constructed with.
// Symbol: ?SetDialogResource@COlePropertyPage@@QEAAXPEAX@Z
extern "C" void MS_ABI impl__SetDialogResource_COlePropertyPage__QEAAXPEAX_Z(
    COlePropertyPage* /*pThis*/, void* /*hDialog*/) {}

// ---------------------------------------------------------------------------
// SetPropCheck / SetPropIndex / SetPropRadio / SetPropText(9 overloads)
//
// Resolved under the MBCS build's ...PEBD... names (Unicode RVAs in the binary
// note at the top of the file): SetPropCheck 0x1e9bf0,
// SetPropRadio 0x1e9ea0, and six SetPropText bodies for the nine overloads.
// ?SetPropIndex@ (0x1ea150) is again only a `jmp 0x1801e9ea0`, an alias of
// SetPropRadio. The six SetPropText bodies are distinguished purely by the
// VARTYPE they pass in R8D, read out of each:
//     0x1e86e0  VT_UI1 (0x11)  BYTE&
//     0x1e8990  VT_I2  (2)     short&
//     0x1e8c40  VT_I4  (3)     int& / UINT& / long& / DWORD&  -- one body for
//                              all four, which is why the int&, UINT& and
//                              long& manglings have no RVA of their own
//     0x1e9340  VT_R4  (4)     float&
//     0x1e9600  VT_R8  (5)     double&
//     0x1e98c0  VT_BSTR(8)     CString&
// (SetPropCheck uses VT_BOOL 0xb and SetPropRadio VT_I2 2, likewise read out.)
//
// One shape, transcribed from SetPropText(BYTE&) at 0x1e86e0:
//     BOOL bWrote = FALSE;
//     for (i = 0; i < m_nObjects /*0x1ac*/; ++i) {
//         IDispatch* p = m_ppDisp /*0x198*/ [i];
//         if (FAILED(p->GetIDsOfNames(...)))    ; IDispatch vtable slot 5
//             continue;                         ; 0x1e8772 `js` -- the object
//                                               ; is skipped and bWrote is
//                                               ; left alone for it
//         COleDispatchDriver::AttachDispatch(p, FALSE);      ; 0x250e50
//         COleDispatchDriver::SetProperty(dispid, <VARTYPE>, value); ; 0x251780
//                                               ; the VARTYPE is from the
//                                               ; table above -- R8D = 0x11
//                                               ; (VT_UI1) in this body
//         bWrote = TRUE;
//     }
//     return bWrote;                            ; the register is ESI
//
// As with the GetProp* family the loop bound is permanently 0 here, so retail
// writes to no object and returns FALSE -- which is what the generated stubs
// below already return. Left as-is, verified equivalent for this object
// state. The object array is a headerRequest.
//
// Signatures: as in the GetProp* block, the generated stubs had no leading
// `COlePropertyPage* pThis` and the CStringT overload had four parameters.
// They are corrected below to the retail register assignment (`this` in RCX,
// the property name in RDX, the value in R8), read out of SetPropText(BYTE&)
// 0x1ea940 and SetPropText(CString&) 0x1ebb20 in mfc140u. The bodies still
// ignore every argument, so nothing observable changes.
// ---------------------------------------------------------------------------

// Symbol: ?SetPropCheck@COlePropertyPage@@QEAAHPEB_WH@Z
extern "C" int MS_ABI impl__SetPropCheck_COlePropertyPage__QEAAHPEB_WH_Z(
    COlePropertyPage* /*pThis*/, const wchar_t* /*pszPropName*/, int /*value*/) {
    return 0;
}

// Symbol: ?SetPropIndex@COlePropertyPage@@QEAAHPEB_WH@Z
extern "C" int MS_ABI impl__SetPropIndex_COlePropertyPage__QEAAHPEB_WH_Z(
    COlePropertyPage* /*pThis*/, const wchar_t* /*pszPropName*/, int /*value*/) {
    return 0;
}

// Symbol: ?SetPropRadio@COlePropertyPage@@QEAAHPEB_WH@Z
extern "C" int MS_ABI impl__SetPropRadio_COlePropertyPage__QEAAHPEB_WH_Z(
    COlePropertyPage* /*pThis*/, const wchar_t* /*pszPropName*/, int /*value*/) {
    return 0;
}

// Symbol: ?SetPropText@COlePropertyPage@@QEAAHPEB_WAEAE@Z
extern "C" int MS_ABI impl__SetPropText_COlePropertyPage__QEAAHPEB_WAEAE_Z(
    COlePropertyPage* /*pThis*/, const wchar_t* /*pszPropName*/, unsigned char* /*&value*/) {
    return 0;
}

// Symbol: ?SetPropText@COlePropertyPage@@QEAAHPEB_WAEAF@Z
extern "C" int MS_ABI impl__SetPropText_COlePropertyPage__QEAAHPEB_WAEAF_Z(
    COlePropertyPage* /*pThis*/, const wchar_t* /*pszPropName*/, short* /*&value*/) {
    return 0;
}

// Symbol: ?SetPropText@COlePropertyPage@@QEAAHPEB_WAEAH@Z
extern "C" int MS_ABI impl__SetPropText_COlePropertyPage__QEAAHPEB_WAEAH_Z(
    COlePropertyPage* /*pThis*/, const wchar_t* /*pszPropName*/, int* /*&value*/) {
    return 0;
}

// Symbol: ?SetPropText@COlePropertyPage@@QEAAHPEB_WAEAI@Z
extern "C" int MS_ABI impl__SetPropText_COlePropertyPage__QEAAHPEB_WAEAI_Z(
    COlePropertyPage* /*pThis*/, const wchar_t* /*pszPropName*/, unsigned int* /*&value*/) {
    return 0;
}

// Symbol: ?SetPropText@COlePropertyPage@@QEAAHPEB_WAEAJ@Z
extern "C" int MS_ABI impl__SetPropText_COlePropertyPage__QEAAHPEB_WAEAJ_Z(
    COlePropertyPage* /*pThis*/, const wchar_t* /*pszPropName*/, long* /*&value*/) {
    return 0;
}

// Symbol: ?SetPropText@COlePropertyPage@@QEAAHPEB_WAEAK@Z
extern "C" int MS_ABI impl__SetPropText_COlePropertyPage__QEAAHPEB_WAEAK_Z(
    COlePropertyPage* /*pThis*/, const wchar_t* /*pszPropName*/, unsigned long* /*&value*/) {
    return 0;
}

// Symbol: ?SetPropText@COlePropertyPage@@QEAAHPEB_WAEAM@Z
extern "C" int MS_ABI impl__SetPropText_COlePropertyPage__QEAAHPEB_WAEAM_Z(
    COlePropertyPage* /*pThis*/, const wchar_t* /*pszPropName*/, float* /*&value*/) {
    return 0;
}

// Symbol: ?SetPropText@COlePropertyPage@@QEAAHPEB_WAEAN@Z
extern "C" int MS_ABI impl__SetPropText_COlePropertyPage__QEAAHPEB_WAEAN_Z(
    COlePropertyPage* /*pThis*/, const wchar_t* /*pszPropName*/, double* /*&value*/) {
    return 0;
}

// Symbol: ?SetPropText@COlePropertyPage@@QEAAHPEB_WAEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@@Z
extern "C" int MS_ABI impl__SetPropText_COlePropertyPage__QEAAHPEB_WAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
    COlePropertyPage* /*pThis*/, const wchar_t* /*pszPropName*/, void* /*const CString& value*/) {
    return 0;
}

// COlePropertyPage::WindowProc(UINT, WPARAM, LPARAM) — retail (0x1e75d0):
//     AFX_MAINTAIN_STATE2 _state(*(this+0x38))          ; m_pModuleState
//     if (message == 0x112 /*WM_SYSCOMMAND*/ &&
//         (wParam & 0xfff0) != 0xf100 /*SC_KEYMENU*/) {
//         pSite = *(this+0x190);                        ; m_pPageSite
//         if (pSite != NULL &&
//             pSite->vtbl[6](pSite, CWnd::GetCurrentMessage()) == S_OK)
//             return 0;                                 ; slot 6 =
//     }                                                 ;  IPropertyPageSite::
//     return CWnd::WindowProc(message, wParam, lParam)  ;  TranslateAccelerator
// The module-state guard is a retail AFX_MAINTAIN_STATE2 frame; this
// reimplementation does not reproduce it (OpenMFC's thunks do not push module
// state at this boundary), only the accelerator forwarding.
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
// XPropertyPage (IPropertyPage) and XPropNotifySink (IPropertyNotifySink)
//
// These are the two nested interface sub-objects MFC embeds in the retail
// COlePropertyPage. Every one of their bodies opens by converting the
// interface pointer it was handed into the outer object:
//     XPropertyPage   methods: `lea -0x1f0(%rcx),%r*`   (checked on Activate,
//                              Deactivate, Apply, Help, EditProperty, Move,
//                              SetObjects; GetPageInfo instead addresses the
//                              outer's members directly at -0x88..-0x68, and
//                              IsPageDirty at -0x98, which is the same origin)
//     XPropNotifySink methods: `lea -0x1f8(%rcx),%r*`   (checked on OnChanged)
// -- i.e. the sub-objects sit at outer+0x1f0 and outer+0x1f8, which the retail
// constructor (0x1e6e20) fills with the vtables at 0x180322768 and 0x180322738.
//
// OpenMFC's COlePropertyPage declares neither sub-object; the bytes at those
// offsets are inside _olepropertypage_padding and hold no vtable, and
// g_imap_COlePropertyPage (detail/InterfaceMapsSupport.cpp) is the inherited
// CCmdTarget map, so nothing in this build ever hands out a pointer to them.
// Reproducing the retail `this` adjustment would therefore compute an outer
// pointer from an address no caller can legitimately supply. Both sub-objects
// (and the members most of these methods touch) are a headerRequest, and the
// entry points below stay stubs with the decoded behaviour recorded.
//
// The two nested vtables themselves ARE in the retail image and were dumped
// out of .rdata; every slot resolves through the symbol map, which is what
// fixes the identity of each sub-object:
//
//   XPropNotifySink, vtable 0x322738, 5 slots (RTTI locator at 0x322730):
//     0  0x1e8300  QueryInterface        3  0x1e8370  OnChanged
//     1  0x003ae0  AddRef  -> `mov $1,%eax; ret`
//     2  0x007260  Release -> `xor %eax,%eax; ret`
//     4  0x007260  OnRequestEdit  -- SAME address as slot 2; the retail body
//                  is `xor %eax,%eax; ret`, identical-COMDAT-folded with
//                  every other trivial `return 0` in the image. See its entry
//                  point below: the body ignores `this`, so it is
//                  implementable even though the sub-object is unreachable.
//   (AddRef returning a constant 1 and Release a constant 0 is not a decode
//   error: this sink is owned by the page and is deliberately not refcounted.)
//
//   XPropertyPage, vtable 0x322768, 15 slots (RTTI locator at 0x322760):
//     0  0x1e7940  QueryInterface        8  0x1e7e20  Show
//     1  0x1e7900  AddRef                9  0x1e7e60  Move
//     2  0x1e7910  Release              10  0x1e7ea0  IsPageDirty
//     3  0x1e7970  SetPageSite          11  0x1e7eb0  Apply
//     4  0x1e7a00  Activate             12  0x1e7f70  Help
//     5  0x1e7c10  Deactivate           13  0x1e8010  TranslateAccelerator
//     6  0x1e7c60  GetPageInfo          14  0x1e82a0  EditProperty
//     7  0x1e7cc0  SetObjects
//   -- CORRECTION (this file previously said "14 slots ... exactly IUnknown +
//   IPropertyPage"): there are FIFTEEN. The qword at 0x3227d8 is
//   ?EditProperty@XPropertyPage@ (0x1e82a0) and only the one after it,
//   0x3227e0 = 0x18035ca68, is the next .rdata object's RTTI locator. So the
//   sub-object implements IPropertyPage2, not IPropertyPage: EditProperty is
//   slot 14 of THIS vtable, not a second interface part. Whoever adds the
//   BEGIN_INTERFACE_PART must declare IPropertyPage2 and register
//   IID_IPropertyPage2 in the interface map, or the frame's
//   QueryInterface(IID_IPropertyPage2) will fail and property browsing will
//   fall back to the IPropertyPage path.
//   Slots 0-2
//   are `add $-0x1f0,%rcx` thunks that delegate through the outer's
//   CCmdTarget::m_pOuterUnknown at outer+0x10 (falling back to 0x26bda0 /
//   0x26bb40 / 0x26bb70 when it is null), which independently re-confirms the
//   0x1f0 sub-object offset.
//
// Outer vtable slots. Four of these methods call back into the outer object
// through four CONSECUTIVE vtable slots, one each:
//     0x328  SetPageSite  (0x1e7970)   ->  OnSetPageSite
//     0x330  SetObjects   (0x1e7cc0)   ->  OnObjectsChanged
//     0x338  Help         (0x1e7f70)   ->  OnHelp
//     0x340  EditProperty (0x1e82a0)   ->  OnEditProperty
// The four offsets and their call sites were read out of the disassembly and
// are certain. That they are COlePropertyPage's OWN first four added virtuals
// is now structural rather than guessed: CDialog's vtable (0x323498, installed
// by ??0CDialog@ at 0x206015) is exactly 0x328 bytes long -- its last slot is
// 0x3237b8 and the qword at 0x3237c0 is ?GetThisMessageMap@CWnd@@, i.e. the
// first field of the next .rdata object, not a slot -- so 0x328/0x330/0x338/
// 0x340 are precisely the first four slots a CDialog-derived class appends.
// Which of the four each one is remains matched by position and by call site:
// they are the four virtuals MFC's COlePropertyPage adds, in declaration
// order, and each is called from the IPropertyPage method that documents it.
//
// COlePropertyPage's own vtable is NOT in the retail image -- re-checked by
// scanning every section for a qword equal to the VA of OnInitDialog
// 0x1e7200, WindowProc 0x1e75d0, OnFinalRelease 0x1e7590, PreTranslateMessage
// 0x1e7210, OnCommand 0x1e83e0 or OnSetPageSite 0x1e6f90: zero hits, and the
// single hit on OnCtlColor 0x1e7680 is its AFX_MSGMAP_ENTRY at 0x322838.
// ??0COlePropertyPage@ (0x1e6e20) never stores a vptr either; it leaves the
// CDialog vtable that ??0CDialog@ installed. Do not repeat that search.
// Two other outer vtable offsets used below WERE read directly out of the
// CDialog/CWnd vtable at 0x323498:
//     0xd0   -> ?DestroyWindow@CWnd@@UEAAHXZ            (0x289f70)
//     0x2d8  -> ?Create@CDialog@@UEAAHIPEAVCWnd@@@Z     (0x27fc0, a
//               `movzwl %dx,%edx; jmp 0x206220` thunk onto the LPCTSTR form)
//     0x2e8  -> ?CreateIndirect@CDialog@@UEAAHPEAXPEAVCWnd@@@Z (0x2062f0)
// ===========================================================================

// XPropertyPage::Activate(HWND hWndParent, LPCRECT prc, BOOL) -- retail
// (0x1e7a00), under AFX_MAINTAIN_STATE2:
//     if (m_hDialogTemplate /*0x1e8*/) {
//         LPVOID p = ::LockResource(m_hDialogTemplate);  ; IAT 0x2c4588,
//                                                        ; = KERNEL32!LockResource
//         if (p)
//             ok = outer->vtbl[0x2e8/8](p, CWnd::FromHandle(hWndParent));
//                                       ; = CDialog::CreateIndirect(void*,CWnd*)
//         else
//             ok = outer->vtbl[0x2d8/8](*(UINT*)(this+0x15c),
//                                       CWnd::FromHandle(hWndParent));
//     } else {
//         ok = outer->vtbl[0x2d8/8](*(UINT*)(this+0x15c),
//                                   CWnd::FromHandle(hWndParent));
//                                       ; = CDialog::Create(UINT,CWnd*)
//     }
//         ; CWnd::FromHandle is 0x289180 and is called separately on each of
//         ; the three arms (0x1e7a46 / 0x1e7a63 / 0x1e7a81), always after the
//         ; LockResource; the LockResource-failed arm then falls into the same
//         ; by-id Create as the no-template arm -- both reach 0x1e7a8c.
//     if (!ok) return E_FAIL /*0x80004005*/;         ; 0x1e7a9d -> 0x1e7b7c
//     CWnd::MoveWindow(prc->left, prc->top, prc->right - prc->left,
//                      prc->bottom - prc->top, TRUE);
//         ; 0x2a7920 = ?MoveWindow@CWnd@@QEAAXHHHHH@Z -- the five-int
//         ; overload with the rect decomposed inline, NOT MoveWindow(LPCRECT,
//         ; BOOL); same call shape as XPropertyPage::Move below
//     *(int*)(this+0x1b0) = 1;
//     CWnd::UpdateData(FALSE);                      ; 0x28f530
//     SetModifiedFlag(FALSE);                       ; 0x1e7710
//     *(int*)(this+0x1b0) = 0;
//     free(m_pControlStatus /*0x1b8*/); m_pControlStatus = NULL;
//     m_nControls /*0x1b4*/ = 0;
//     ::EnumChildWindows(m_hWnd, EnumChildProc, this);   ; count  (0x1e7ba0)
//     if (m_nControls > 0)
//         m_pControlStatus = operator new(m_nControls * 8);
//     m_nControls = 0;
//     ::EnumChildWindows(m_hWnd, EnumControls, this);    ; fill   (0x1e7bc0)
//     return S_OK;
// This is where the control-status table that GetControlStatus/
// SetControlStatus/Apply read is built. It needs the dialog template, the
// control table and the 0x1b0 flag, none of which OpenMFC declares.
// Symbol: ?Activate@XPropertyPage@COlePropertyPage@@UEAAJPEAUHWND__@@PEBUtagRECT@@H@Z
extern "C" long MS_ABI impl__Activate_XPropertyPage_COlePropertyPage__UEAAJPEAUHWND____PEBUtagRECT__H_Z(
    void* /*pThis: the XPropertyPage sub-object*/, HWND /*hWndParent*/,
    const RECT* /*prc*/, int /*bModal*/) {
    return 0;
}

// XPropertyPage::Apply() -- retail (0x1e7eb0). Note the early exit: when the
// page is not dirty NOTHING else runs -- not the deferred-refill drain, not
// the control-status reset. (`cmp %edi,0x158(%rbx); je <exit>` at 0x1e7ed1
// jumps straight to the `mov %edi,%eax` return with EDI still 0.)
//     HRESULT hr = S_OK;
//     if (m_bDirty /*0x158*/ == 0) return S_OK;
//     BOOL bOK = CWnd::UpdateData(TRUE);            ; 0x28f530
//     if (bOK) m_bDirty = 0; else hr = E_FAIL /*0x80004005*/;
//     if (*(int*)(this+0x1a8)) {                    ; deferred refill pending
//         CWnd::UpdateData(FALSE); *(int*)(this+0x1a8) = 0;
//     } else if (!bOK) {
//         return hr;                                ; 0x1e7f1b: skip the reset
//     }
//     if (m_pControlStatus /*0x1b8*/)
//         for (i = 0; i < m_nControls /*0x1b4*/; ++i)
//             m_pControlStatus[i].bDirty = FALSE;
//     return hr;
// Needs the retail dirty flag (0x158, distinct from OpenMFC's m_bModified at
// 0x138), the 0x1a8 flag XPropNotifySink::OnChanged sets, and the control
// table -- plus a reachable sub-object. Stub.
// Symbol: ?Apply@XPropertyPage@COlePropertyPage@@UEAAJXZ
extern "C" long MS_ABI impl__Apply_XPropertyPage_COlePropertyPage__UEAAJXZ(
    void* /*pThis: the XPropertyPage sub-object*/) {
    return 0;
}

// XPropertyPage::Deactivate() -- retail (0x1e7c10): under
// AFX_MAINTAIN_STATE2, call the outer through vtable offset 0xd0 --
// CWnd::DestroyWindow, confirmed by reading offset 0xd0 of a CWnd-derived
// vtable in the retail image -- and return S_OK. The call itself is
// expressible here (impl__DestroyWindow_CWnd__UEAAHXZ is declared above), but
// the outer pointer is not recoverable from an unreachable sub-object. Stub.
// Symbol: ?Deactivate@XPropertyPage@COlePropertyPage@@UEAAJXZ
extern "C" long MS_ABI impl__Deactivate_XPropertyPage_COlePropertyPage__UEAAJXZ(
    void* /*pThis: the XPropertyPage sub-object*/) {
    return 0;
}

// XPropertyPage::EditProperty(DISPID) -- retail (0x1e82a0): under
// AFX_MAINTAIN_STATE2, call the outer's vtable offset 0x340 (see the
// "outer vtable slots" note in this section's header: 0x340 is
// COlePropertyPage::OnEditProperty by position, not by a vtable that was
// read) and map the BOOL it returns to an HRESULT:
//     return ret ? S_OK : E_NOTIMPL /*0x80004001*/;
// (`neg; sbb; not; and $0x80004001` is that select.) OpenMFC does declare
// OnEditProperty as a virtual and defines it above to return FALSE, so only
// the unreachable sub-object blocks this. Stub.
// Symbol: ?EditProperty@XPropertyPage@COlePropertyPage@@UEAAJJ@Z
extern "C" long MS_ABI impl__EditProperty_XPropertyPage_COlePropertyPage__UEAAJJ_Z(
    void* /*pThis: the XPropertyPage sub-object*/, long /*dispid*/) {
    return 0;
}

// XPropertyPage::GetPageInfo(PROPPAGEINFO*) -- retail (0x1e7c60), the one
// method here with no module-state frame:
//     if (pPageInfo == NULL) AfxThrowInvalidArgException();   ; 0x225b80
//     pPageInfo->pszTitle      = dup(m_strPageName  /*outer+0x168*/); ; 0x1e5714
//     pPageInfo->size          =     m_sizePage     /*outer+0x170*/;  ; one qword
//     pPageInfo->pszDocString  = dup(*(outer+0x178));
//     pPageInfo->pszHelpFile   = dup(*(outer+0x180));
//     pPageInfo->dwHelpContext =    *(outer+0x188);
//     return S_OK;
// It notably does not write pPageInfo->cb. Retail keeps the three strings as
// CStrings at 0x168/0x178/0x180 and copies them into task-allocated strings;
// OpenMFC keeps borrowed wchar_t* in m_pszPageName/m_pszDocString/
// m_pszHelpFile at different offsets and has no m_sizePage at all. Stub.
// Symbol: ?GetPageInfo@XPropertyPage@COlePropertyPage@@UEAAJPEAUtagPROPPAGEINFO@@@Z
extern "C" long MS_ABI impl__GetPageInfo_XPropertyPage_COlePropertyPage__UEAAJPEAUtagPROPPAGEINFO___Z(
    void* /*pThis: the XPropertyPage sub-object*/, void* /*PROPPAGEINFO* pPageInfo*/) {
    return 0;
}

// XPropertyPage::Help(LPCOLESTR pszHelpDir) -- retail (0x1e7f70): under
// AFX_MAINTAIN_STATE2,
//     CString str(pszHelpDir);                      ; 0x3b330
//     LPCTSTR arg = pszHelpDir ? (LPCTSTR)str : NULL;
//         ; `neg %rbx; sbb %rdx,%rdx; and 0x40(%rsp),%rdx` -- when pszHelpDir
//         ; is NULL the CString pointer is masked to 0, so OnHelp is called
//         ; with NULL, not with the nil string
//     BOOL b = outer->vtbl[0x338/8](arg);           ; see the section header
//     return b ? S_OK : S_FALSE;                    ; `test eax,eax; sete bl`
// Blocked on ONE thing only: the sub-object is unreachable. (This file used to
// add "and OnHelp itself has no resolvable RVA" -- that was stale even as it
// was written; OnHelp resolves in mfc140u at 0x71e0 and is implemented above.)
// Symbol: ?Help@XPropertyPage@COlePropertyPage@@UEAAJPEB_W@Z
extern "C" long MS_ABI impl__Help_XPropertyPage_COlePropertyPage__UEAAJPEB_W_Z(
    void* /*pThis: the XPropertyPage sub-object*/, const wchar_t* /*pszHelpDir*/) {
    return 0;
}

// XPropertyPage::IsPageDirty() -- retail (0x1e7ea0) is four instructions:
//     xor eax,eax ; cmp eax,[rcx-0x98] ; sete al ; ret
// rcx is outer+0x1f0, so -0x98 is outer+0x158, the retail dirty flag: the
// result is 1 (S_FALSE) when the page is clean and 0 (S_OK) when it is dirty.
// OpenMFC's dirty flag is m_bModified at 0x138, not 0x158, and the sub-object
// is unreachable, so this cannot be wired up from here. Stub.
// Symbol: ?IsPageDirty@XPropertyPage@COlePropertyPage@@UEAAJXZ
extern "C" long MS_ABI impl__IsPageDirty_XPropertyPage_COlePropertyPage__UEAAJXZ(
    void* /*pThis: the XPropertyPage sub-object*/) {
    return 0;
}

// XPropertyPage::Move(LPCRECT prc) -- retail (0x1e7e60), no module-state
// frame:
//     CWnd::MoveWindow(outer, prc->left, prc->top,
//                      prc->right - prc->left, prc->bottom - prc->top,
//                      TRUE);                          ; 0x2a7920
//     return S_OK;
// The only thing it needs is the outer pointer, which an unreachable
// sub-object cannot yield. Stub.
// Symbol: ?Move@XPropertyPage@COlePropertyPage@@UEAAJPEBUtagRECT@@@Z
extern "C" long MS_ABI impl__Move_XPropertyPage_COlePropertyPage__UEAAJPEBUtagRECT___Z(
    void* /*pThis: the XPropertyPage sub-object*/, const RECT* /*prc*/) {
    return 0;
}

// XPropNotifySink::OnChanged(DISPID) -- retail (0x1e8370): under
// AFX_MAINTAIN_STATE2,
//     if (outer->m_hWnd /*0x40*/ && AfxGetThreadState()->[0x138] != m_hWnd)
//                                 ; 0x138 is the MBCS field offset. In
//                                 ; mfc140u -- the build OpenMFC targets --
//                                 ; the same compare reads +0x198
//                                 ; (0x1801ea5f9: cmp %rcx,0x198(%rax)).
//                                 ; This is the one place in this file where
//                                 ; the two builds differ; the field is in
//                                 ; AFX_THREAD_STATE, not in this class.
//         CWnd::UpdateData(outer, FALSE);   ; 0x28f530 -- refill the controls
//     else
//         *(int*)(outer+0x1a8) = 1;         ; defer it; Apply drains this flag
//     return S_OK;
// The thread-state compare suppresses the refill while the page's own window
// is the one being edited. Needs the 0x1a8 deferral flag and a reachable
// sub-object. Stub.
// Symbol: ?OnChanged@XPropNotifySink@COlePropertyPage@@UEAAJJ@Z
extern "C" long MS_ABI impl__OnChanged_XPropNotifySink_COlePropertyPage__UEAAJJ_Z(
    void* /*pThis: the XPropNotifySink sub-object*/, long /*dispid*/) {
    return 0;
}

// XPropNotifySink::OnRequestEdit(DISPID) -- resolved two independent ways,
// both giving the same body.
//   1. In the Unicode build it is mfc140u ordinal 10919 -> RVA 0x71e0.
//   2. In the MBCS build it is slot 4 of the XPropNotifySink vtable at
//      0x322738 (see this section's header), which holds 0x180007260.
// Both addresses disassemble to the same two instructions:
//     xor %eax,%eax ; ret
// i.e. `return S_OK;` unconditionally. It never touches `this`, never consults
// the dispid, and never vetoes an edit -- MFC's property page always permits
// the change and waits for the matching OnChanged.
//
// Both addresses are folded COMDATs shared with every other trivial
// `return 0` in their image (0x180007260 is also this sub-object's own
// Release), so the NAME the symbol map happens to attach to either address is
// not evidence about this method -- but the folding is by identical code, so
// the behaviour it pins down for this slot is exact. The two builds agreeing
// is what makes it safe to rely on.
//
// Because the retail body ignores its `this`, this is the one entry point in
// this section that does not need the unreachable sub-object: it is
// implemented here rather than stubbed. (The generated stub already returned
// 0; what changes is that the value is now established, not assumed.)
// Signature, as for OnHelp: the generated prototype had no leading `this`,
// so its `long` was really the sub-object pointer in RCX; it is corrected
// below to (this, dispid). Both are ignored -- which is what makes this safe.
// Symbol: ?OnRequestEdit@XPropNotifySink@COlePropertyPage@@UEAAJJ@Z
extern "C" long MS_ABI impl__OnRequestEdit_XPropNotifySink_COlePropertyPage__UEAAJJ_Z(
    void* /*pThis: the XPropNotifySink sub-object*/, long /*dispid*/) {
    return S_OK;
}

// XPropertyPage::SetObjects(ULONG nObjects, IUnknown** ppUnk) -- retail
// (0x1e7cc0), the sole writer of the object array the whole GetProp*/SetProp*
// family reads:
//     CleanupObjectArray();                              ; 0x1e7380
//     if (nObjects) {
//         m_ppDisp /*0x198*/         = operator new(nObjects * 8);
//         m_pdwConnections /*0x1a0*/ = operator new(nObjects * 4);
//         for (i = 0; i < nObjects; ++i) {
//             hr = ppUnk[i]->QueryInterface(IID_IDispatch, &m_ppDisp[i]);
//             if (FAILED(hr)) return hr;   ; NOT a break: 0x1e7d60 `js` goes
//                 ; straight to the epilogue at 0x1e7dff with the failing
//                 ; HRESULT still in EAX, so m_nObjects is left UNWRITTEN and
//                 ; OnObjectsChanged is never called on this path.
//             AfxConnectionAdvise(ppUnk[i], IID_IPropertyNotifySink,
//                                 &this->m_xPropNotifySink /*0x1f8*/, FALSE,
//                                 &m_pdwConnections[i]);  ; 0x1dd1c0
//         }
//     }
//     m_nObjects /*0x1ac*/ = nObjects;
//     BOOL bLocked = FALSE;
//     if (outer->m_hWnd /*0x40*/ && ::IsWindowVisible(m_hWnd)) {  ; 0x2c5350
//         ::LockWindowUpdate(m_hWnd); bLocked = TRUE;             ; 0x2c4d18
//     }
//     outer->vtbl[0x330/8]();          ; OnObjectsChanged -- UNCONDITIONAL,
//                                      ; it is not inside the m_hWnd test
//     if (nObjects && outer->m_hWnd) {
//         CWnd::UpdateData(FALSE);     ; 0x28f530
//         SetModifiedFlag(FALSE);      ; 0x1e7710
//     }
//     if (bLocked) ::LockWindowUpdate(NULL);
//     return S_OK;
// This is the entry point whose absence makes the object array permanently
// empty, which is what every "loop never runs" reduction in this file rests
// on. Implementing it needs m_ppDisp, m_pdwConnections, m_nObjects and the
// XPropNotifySink sub-object -- the headerRequest below. Stub.
// Symbol: ?SetObjects@XPropertyPage@COlePropertyPage@@UEAAJKPEAPEAUIUnknown@@@Z
extern "C" long MS_ABI impl__SetObjects_XPropertyPage_COlePropertyPage__UEAAJKPEAPEAUIUnknown___Z(
    void* /*pThis: the XPropertyPage sub-object*/, unsigned long /*nObjects*/,
    void** /*IUnknown** ppUnk*/) {
    return 0;
}

// XPropertyPage::SetPageSite(IPropertyPageSite*) -- retail (0x1e7970), under
// AFX_MAINTAIN_STATE2:
//     release(&outer->m_pPageSite /*0x190*/);       ; 0x26ba84
//     outer->m_pPageSite = pPageSite;
//     if (pPageSite) {
//         pPageSite->AddRef();                      ; IUnknown slot 1
//         outer->vtbl[0x328/8]();                   ; OnSetPageSite
//     }
//     return S_OK;
// Note that retail's ?OnSetPageSite@COlePropertyPage@@UEAAXXZ (0x1e6f90)
// returns void, while OpenMFC's afxole.h declares it `virtual BOOL
// OnSetPageSite()`. Not on this file's assignment list; recorded here because
// it is a real declaration mismatch someone will have to resolve. Blocked, as
// the rest of this section, on the sub-object being unreachable. Stub.
// Symbol: ?SetPageSite@XPropertyPage@COlePropertyPage@@UEAAJPEAUIPropertyPageSite@@@Z
extern "C" long MS_ABI impl__SetPageSite_XPropertyPage_COlePropertyPage__UEAAJPEAUIPropertyPageSite___Z(
    void* /*pThis: the XPropertyPage sub-object*/, void* /*IPropertyPageSite* pPageSite*/) {
    return 0;
}

// XPropertyPage::Show(UINT nCmdShow) -- retail (0x1e7e20), no module-state
// frame; the whole body past the prologue is:
//     CWnd::ShowWindow(outer, nCmdShow);            ; 0x2a79e0
//     if (nCmdShow == SW_SHOWNORMAL /*1*/) CWnd::SetFocus(outer);  ; 0x2a7a70
//     return S_OK;
// (The two callee RVAs are quoted from the call targets; only the outer
// pointer, which an unreachable sub-object cannot yield, blocks this.) Stub.
// Symbol: ?Show@XPropertyPage@COlePropertyPage@@UEAAJI@Z
extern "C" long MS_ABI impl__Show_XPropertyPage_COlePropertyPage__UEAAJI_Z(
    void* /*pThis: the XPropertyPage sub-object*/, unsigned int /*nCmdShow*/) {
    return 0;
}

// XPropertyPage::TranslateAccelerator(MSG*) -- retail (0x1e8010, the MBCS
// build's ?TranslateAcceleratorA@...). Only partially decoded here: it filters
// on WM_KEYDOWN (0x100) with wParam == VK_TAB (9), consults ::GetKeyState
// (IAT 0x2c4d20) for VK_CONTROL (0x11) and VK_SHIFT (0x10), checks
// ::IsChild(outer->m_hWnd, CWnd::FromHandle(::GetFocus())->m_hWnd) and then
// walks the focus chain with ::GetParent to move the focus between the page's
// controls; the WM_GETDLGCODE (0x87) probe at 0x1e80b0 is used to skip
// controls that want the tab themselves. The tail of the body was not walked,
// so nothing more is asserted. Stub -- unreachable sub-object regardless.
// (Re-checked in mfc140u at 0x1ea270: `lea -0x1f0(%rcx),%rbx`, the WM_KEYDOWN
// /VK_TAB filter, the two ::GetKeyState calls for VK_CONTROL and VK_SHIFT and
// the ::GetFocus/::IsChild pair are all as described. This entry point is not
// on the current assignment list; only its signature was corrected, to the
// same retail register assignment as its siblings -- `this` in RCX, the MSG*
// in RDX.)
// Symbol: ?TranslateAcceleratorW@XPropertyPage@COlePropertyPage@@UEAAJPEAUtagMSG@@@Z
extern "C" long MS_ABI impl__TranslateAcceleratorW_XPropertyPage_COlePropertyPage__UEAAJPEAUtagMSG___Z(
    void* /*pThis: the XPropertyPage sub-object*/, void* /*MSG* pMsg*/) {
    return 0;
}
