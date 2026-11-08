// COleFrameHook — OpenMFC implementation.
// Sources: ole_olectors_exports.cpp, olecore.cpp
//
// Every retail behaviour described below was decoded from the retail image
// rather than inferred, the way phase4/src/core/ole/COleControl.cpp describes.
// (The exceptions, each flagged where it sits: the typed C++ overrides
// COleFrameHook::OnResizeBorder and ::OnGetClipboardData have no retail
// counterpart at all, and the deviations OpenMFC's narrower class layout
// forces are called out at each site.)  Two facts about the tooling matter
// for reading the citations:
//
//   * `disas.py --u` reads mfc140u.dll and `disas.py` reads mfc140.dll.  The
//     BODIES are byte-identical between the two images, but every export sits
//     at a different RVA.  mfc140u_rva_symbols.json resolves only 15 of this
//     class's 33 functions, while the mfc140 (ANSI) map resolves 32, so most
//     bodies below were read at their ANSI RVA.  Each comment names the image
//     it is quoting; where both are known both are given.
//   * The COleFrameHook class vftable is at mfc140u .rdata 0x32c0f0 (planted
//     by the store at mfc140u 0x247db4 inside the ctor, entry 0x247d90) and
//     at mfc140 .rdata 0x329f30 (store at mfc140 0x246214 inside the ANSI
//     ctor, entry 0x2461f0).  Slots 22..33 are this class's own virtuals; slot 0 is
//     CCmdTarget::GetRuntimeClass and slot 16 is COleFrameHook's own
//     GetInterfaceMap (mfc140u 0x2492f0).  The ANSI vftable names all twelve
//     of the own virtuals outright, because the mfc140 map resolves all
//     twelve bodies.  The mfc140u map resolves only THREE of the twelve —
//     OnActivate (slot 24), OnDocActivate (25) and OnContextHelp (26); it
//     does NOT resolve OnRecalcLayout, so do not expect `disas.py --u` to
//     name it.  The two vftables hold the same twelve functions at the same
//     slot indices, and in RVA order the inter-function byte deltas are
//     identical in both images (0x80, 0xd0, 0x30, 0xb0, 0x40, 0x80, 0xd0,
//     0x50, 0x30, 0x30, 0x40), with the three doubly-named ones landing on
//     the same slots.  That is what names the other nine mfc140u addresses.
//     Slot -> (mfc140u RVA / mfc140 RVA):
//        22 OnRecalcLayout          0x247ef0 / 0x246350
//        23 OnPreTranslateMessage   0x2481e0 / 0x246640
//        24 OnActivate              0x2482b0 / 0x246710
//        25 OnDocActivate           0x247f70 / 0x2463d0
//        26 OnContextHelp           0x248040 / 0x2464a0
//        27 OnEnableModeless        0x248300 / 0x246760
//        28 OnUpdateFrameTitle      0x248330 / 0x246790
//        29 OnPaletteChanged        0x248360 / 0x2467c0
//        30 OnQueryNewPalette       0x2483a0 / 0x246800
//        31 OnInitMenuPopup         0x248160 / 0x2465c0
//        32 OnInitMenu              0x248120 / 0x246580
//        33 OnMenuSelect            0x248070 / 0x2464d0
//     Non-virtual: NotifyAllInPlace 0x2483e0 / 0x246840,
//     DoContextSensitiveHelp 0x248560 / 0x2469c0,
//     DoEnableModeless 0x248580 / 0x2469e0.
//
// RETAIL MEMBER LAYOUT.  Retail's declaration lives in MFC's private
// src/oleimpl2.h, which is not on this host, so the layout was reconstructed
// from the constructor (mfc140u 0x247d90), the destructor (mfc140u 0x247e80)
// and the nested-interface method prologues.  The total size is not guessed:
// COleClientItem::XOleIPSite::GetWindowContext (mfc140 0x248270) allocates the
// hook with `mov $0x90,%ecx ; call operator new` at 0x248392, so
// sizeof(COleFrameHook) == 0x90.  See RetailCOleFrameHook below, whose
// static_asserts pin every offset cited in this file.
//
// COleFrameHook is NOT a DECLARE_DYNAMIC class in retail: vftable slot 0 is
// CCmdTarget::GetRuntimeClass, and a scan of mfc140u for a CRuntimeClass whose
// m_lpszClassName points at a bare "COleFrameHook" string finds none -- there
// is no such string in the image at all.  The name occurs three times, every
// one of them inside a decorated RTTI type descriptor, which a CRuntimeClass
// never points at: ".?AVXOleCommandTarget@COleFrameHook@@" (mfc140u 0x3bc570),
// ".?AVXOleInPlaceFrame@COleFrameHook@@" (0x3bc5a8) and ".?AVCOleFrameHook@@"
// (0x3bc5e0) -- the third is the class's own type descriptor, which an earlier
// revision of this comment wrongly said did not exist.  OpenMFC's afxole.h declares
// DECLARE_DYNAMIC(COleFrameHook) and OlecoreSupport.cpp:175 defines the
// descriptor; that is an OpenMFC addition, harmless but not retail.
//
// WHAT OPENMFC MODELS, AND WHAT IT DOES NOT. include/openmfc/afxole.h keeps
// the existing three members — m_pFrameWnd, m_pActiveItem and
// m_lpActiveUIWindow — followed by its 48-byte tail. This file's size
// assertion is 0x88 bytes, including in the GNU per-file checker; the
// clang-cl/MSVC ABI check also lays CCmdTarget out as 0x40 bytes. The tail now
// names the retail-compatible storage it can hold without moving
// those members. In clang-cl the members are +0x40/+0x48/+0x50. Retail instead
// has m_lpActiveObject at +0x48 and m_pActiveItem at +0x50, so their retail
// values still require side-table-aware access. The named tail holds +0x58
// m_hWndFrame, +0x68 accelerator, +0x70 modeless count, +0x78 CString storage,
// and a raw +0x80 interface slot. No local COM vtable is installed.
//
// THE NESTED INTERFACES.  Retail compiles every XOleInPlaceFrame /
// XOleCommandTarget method with a METHOD_PROLOGUE that backs the incoming
// interface pointer up to the containing COleFrameHook by a fixed byte
// offset.  Both offsets are established by the constructor, which plants the
// two sub-object vftables at exactly those offsets (mfc140u 0x247dde and
// 0x247de5):
//   XOleInPlaceFrame   this - 0x80   vftable mfc140u .rdata 0x32c040, 15 slots
//                                    = IOleInPlaceFrame.  The back-up is also
//                                    visible literally as `lea -0x80(%rcx),%rdi`
//                                    at mfc140 0x24793f in
//                                    XOleInPlaceFrame::ContextSensitiveHelp.
//   XOleCommandTarget  this - 0x88   vftable mfc140u .rdata 0x32c0c0, 5 slots
//                                    = IOleCommandTarget, whose slot 4 is
//                                    XOleCommandTarget::Exec itself.  No `lea`
//                                    appears in these two bodies: Exec reads
//                                    m_pActiveItem as `mov -0x38(%rcx),%rdi`
//                                    (mfc140 0x2477db) and QueryStatus the same
//                                    at 0x24783b, and 0x88 - 0x38 == 0x50
//                                    agrees with the vftable placement.
// OpenMFC has no usable nested interface parts. Under clang-cl +0x80 is a
// named but zeroed storage slot; the constructor does not install a compatible
// XOleInPlaceFrame vtable. The +0x88 XOleCommandTarget part cannot fit without
// changing the current class size, and the interface map at
// detail/InterfaceMapsSupport.cpp forwards directly to CCmdTarget. No valid
// pointer is published for either METHOD_PROLOGUE back-up, so COM callers
// cannot currently obtain these methods. Several simple callback bodies are
// nevertheless implemented below from their recovered behavior and available
// state; methods that need the unpublished interface/callback infrastructure
// remain explicit stubs. Their parameter lists are corrected because the
// generated signatures omitted interface this.

#define OPENMFC_APPCORE_IMPL

#include "detail/OlecoreSupport.h"

#include "openmfc/afxole.h"
#include "openmfc/afxmfc.h"

#include <cstddef>
#include <new>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

// ---- sibling impl__ exports called by the bodies in this file ----
// (C++ methods of other classes exist in this DLL only as impl__ thunks.)
// core/window/CWnd.cpp:786
extern "C" int MS_ABI impl__IsWindowEnabled_CWnd__QEBAHXZ(const CWnd* pThis);
// core/ole/COleClientItem.cpp:654 -- currently a documented stub that always
// returns NULL; calling it is still the faithful transcription of retail.
extern "C" CWnd* MS_ABI impl__GetInPlaceWindow_COleClientItem__QEAAPEAVCWnd__XZ(
    COleClientItem* pThis);
// core/ole/COleClientItem.cpp:2161
extern "C" int MS_ABI impl__OnUpdateFrameTitle_COleClientItem__UEAAHXZ(
    COleClientItem* pThis);
extern "C" BOOL MS_ABI impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(
    const CObject* pThis, const CRuntimeClass* pClass);
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_COleDocObjectItem__SAPEAUCRuntimeClass__XZ();

// ---- forward declarations of exports defined later in this file ----
// NotifyAllInPlace's second parameter is a pointer to member function
// `BOOL (COleFrameHook::*)(BOOL)`.  Retail invokes it with `mov %r15,%rax ;
// call *__guard_dispatch_icall_fptr` after loading rcx with the hook and edx
// with the BOOL (mfc140u 0x2484e8..0x2484f5), i.e. it is a plain 8-byte code
// pointer taking (COleFrameHook*, BOOL) -- the MSVC representation of a
// pointer to a non-virtual member of a single-inheritance class.
typedef int(MS_ABI* PFN_HOOKNOTIFY)(COleFrameHook*, int);

extern "C" int MS_ABI impl__NotifyAllInPlace_COleFrameHook__QEAAHHP81_EAAHH_Z_Z(
    COleFrameHook* pThis, int bParam, PFN_HOOKNOTIFY pfn);
extern "C" void MS_ABI impl__BeginModalState_CFrameWnd__UEAAXXZ(CFrameWnd* pThis);
extern "C" void MS_ABI impl__EndModalState_CFrameWnd__UEAAXXZ(CFrameWnd* pThis);
extern "C" void* MS_ABI impl___0AFX_MAINTAIN_STATE2__QEAA_PEAVAFX_MODULE_STATE___Z(
    void* pThis, AFX_MODULE_STATE* pState);
extern "C" void MS_ABI impl___1AFX_MAINTAIN_STATE2__QEAA_XZ(void* pThis);
extern "C" int MS_ABI impl__PreTranslateMessage_CFrameWnd__UEAAHPEAUtagMSG___Z(
    CFrameWnd* pThis, MSG* pMsg);
extern "C" int MS_ABI impl__OnShowControlBars_COleClientItem__UEAAHPEAVCFrameWnd__H_Z(
    COleClientItem* pThis, CFrameWnd* pFrameWnd, int bShow);
extern "C" void MS_ABI impl__RecalcLayout_CFrameWnd__UEAAXH_Z(CFrameWnd* pThis, int bNotify);
extern "C" CFrameWnd* MS_ABI impl__GetTopLevelFrame_CWnd__QEBAPEAVCFrameWnd__XZ(
    const CWnd* pThis);
extern "C" int MS_ABI impl__CanEnterHelpMode_CFrameWnd__QEAAHXZ(CFrameWnd* pThis);
extern "C" void MS_ABI impl__ExitHelpMode_CFrameWnd__UEAAXXZ(CFrameWnd* pThis);
extern "C" int MS_ABI impl__DoContextSensitiveHelp_COleFrameHook__QEAAHH_Z(
    COleFrameHook* pThis, int bEnter);
extern "C" int MS_ABI impl__DoEnableModeless_COleFrameHook__QEAAHH_Z(
    COleFrameHook* pThis, int bEnable);

namespace {

// AFX_MAINTAIN_STATE2 is two pointers in the retail x64 ABI. Use its exported
// thunks so callback prologues switch to the hook's module state without
// introducing an internal C++ method reference.
class FrameHookModuleStateScope {
public:
    explicit FrameHookModuleStateScope(AFX_MODULE_STATE* state) : storage_{} {
        impl___0AFX_MAINTAIN_STATE2__QEAA_PEAVAFX_MODULE_STATE___Z(storage_, state);
    }
    ~FrameHookModuleStateScope() {
        impl___1AFX_MAINTAIN_STATE2__QEAA_XZ(storage_);
    }
    FrameHookModuleStateScope(const FrameHookModuleStateScope&) = delete;
    FrameHookModuleStateScope& operator=(const FrameHookModuleStateScope&) = delete;

private:
    void* storage_[2];
};

// ---------------------------------------------------------------------------
// Retail mfc140u COleFrameHook layout, as a file-local view.  This struct is
// documentation with proof attached -- it is NEVER used to access an OpenMFC
// COleFrameHook, whose layout is different (see the header comment).  Each
// offset is followed by the instruction that establishes it.
// ---------------------------------------------------------------------------
#pragma pack(push, 8)
struct RetailCOleFrameHook {
    void* vftable;                 // +0x00  mfc140u 0x18032c0f0, planted at 0x247db4
    char  ccmdtarget[0x38];        // +0x08  CCmdTarget tail; +0x38 is m_pModuleState,
                                   //        read as `mov -0x48(%rcx),%rdx` and handed
                                   //        to AFX_MAINTAIN_STATE2 by MOST of the
                                   //        XOleInPlaceFrame methods
                                   //        (mfc140 0x247a42 in GetBorder,
                                   //        0x80-0x48 == 0x38).  Not all: GetWindow
                                   //        (0x247910), SetStatusText (0x247ea0)
                                   //        and both XOleCommandTarget methods
                                   //        (0x2477d0, 0x247830) have no state
                                   //        prologue at all.
    void* m_pFrameWnd;             // +0x40  ctor `mov %rdi,0x40(%rsi)`      0x247dfd
    void* m_lpActiveObject;        // +0x48  ctor `mov %rcx,0x48(%rsi)` = 0  0x247df5
                                   //        stored by XOleInPlaceFrame::SetActiveObject
                                   //        `mov %rbx,-0x38(%rdi)`   mfc140 0x247c83
    void* m_pActiveItem;           // +0x50  ctor `mov %rbx,0x50(%rsi)`      0x247df9
    void* m_hWndFrame;             // +0x58  ctor `mov 0x40(%rdi),%rax ; mov %rax,0x58(%rsi)`
                                   //        0x247e01 -- i.e. pFrameWnd->m_hWnd.
                                   //        Read back by XOleInPlaceFrame::GetWindow
                                   //        as `mov -0x28(%rcx),%rax` (mfc140 0x24791b)
    void* unknown_0x60;            // +0x60  ctor `mov %rcx,0x60(%rsi)` = 0  0x247e09
                                   //        no reader appears in the bodies decoded here
    void* m_hAccelTable;           // +0x68  ctor `mov %rcx,0x68(%rsi)` = 0  0x247e0d;
                                   //        XOleInPlaceFrame::TranslateAccelerator
                                   //        (entry mfc140 0x247fc0; the load of this
                                   //        member is the instruction at 0x247ff5)
                                   //        swaps it into m_pFrameWnd+0xf8 around
                                   //        PreTranslateMessage
    unsigned int m_nModelessCount; // +0x70  ctor `mov %ecx,0x70(%rsi)` = 0  0x247e11;
                                   //        DoEnableModeless is its only user
    unsigned int pad_0x74;
    void* m_strObjName;            // +0x78  a CString: the ctor fills it from the ATL
                                   //        string manager's nil string
                                   //        (`... ; add $0x18,%rax ; mov %rax,0x78(%rsi)`
                                   //        0x247dcf) and the dtor does the CStringData
                                   //        `lock xadd` release on it across
                                   //        0x247eb0..0x247ed3 (the xadd itself is
                                   //        at 0x247ebb).  Its only writer decoded
                                   //        here is XOleInPlaceFrame::SetActiveObject,
                                   //        which Empty()s it and then assigns
                                   //        pszObjName (mfc140 0x247c9c..0x247cb6);
                                   //        SetStatusText does NOT touch it (it builds
                                   //        a temporary CString), so it is named here
                                   //        for the object name it holds.
    void* m_xOleInPlaceFrame;      // +0x80  ctor plants 0x18032c040        0x247dde
    void* m_xOleCommandTarget;     // +0x88  ctor plants 0x18032c0c0        0x247de5
};
#pragma pack(pop)

static_assert(offsetof(RetailCOleFrameHook, m_pFrameWnd) == 0x40, "retail +0x40");
static_assert(offsetof(RetailCOleFrameHook, m_lpActiveObject) == 0x48, "retail +0x48");
static_assert(offsetof(RetailCOleFrameHook, m_pActiveItem) == 0x50, "retail +0x50");
static_assert(offsetof(RetailCOleFrameHook, m_hWndFrame) == 0x58, "retail +0x58");
static_assert(offsetof(RetailCOleFrameHook, m_hAccelTable) == 0x68, "retail +0x68");
static_assert(offsetof(RetailCOleFrameHook, m_nModelessCount) == 0x70, "retail +0x70");
static_assert(offsetof(RetailCOleFrameHook, m_strObjName) == 0x78, "retail +0x78");
static_assert(offsetof(RetailCOleFrameHook, m_xOleInPlaceFrame) == 0x80,
              "retail +0x80 -- XOleInPlaceFrame METHOD_PROLOGUE back-up offset");
static_assert(offsetof(RetailCOleFrameHook, m_xOleCommandTarget) == 0x88,
              "retail +0x88 -- XOleCommandTarget METHOD_PROLOGUE back-up offset");
static_assert(sizeof(RetailCOleFrameHook) == 0x90,
              "sizeof(COleFrameHook) == 0x90 -- the operator new argument at "
              "mfc140 0x248392 inside XOleIPSite::GetWindowContext");

// OpenMFC's own three members must stay in the order this file assumes.
static_assert(offsetof(COleFrameHook, m_pActiveItem) ==
                  offsetof(COleFrameHook, m_pFrameWnd) + sizeof(void*),
              "afxole.h COleFrameHook member order changed");
static_assert(offsetof(COleFrameHook, m_lpActiveUIWindow) ==
                  offsetof(COleFrameHook, m_pActiveItem) + sizeof(void*),
              "afxole.h COleFrameHook member order changed");
static_assert(sizeof(COleFrameHook) == 0x88,
              "preserve the current clang-cl COleFrameHook size");
static_assert(offsetof(COleFrameHook, m_retailTail) +
                  offsetof(COleFrameHook::RetailTailStorage, m_hWndFrame) == 0x58,
              "COleFrameHook retail frame HWND storage at +0x58");
static_assert(offsetof(COleFrameHook, m_retailTail) +
                  offsetof(COleFrameHook::RetailTailStorage, m_unknown_60) == 0x60,
              "COleFrameHook retail +0x60 storage");
static_assert(offsetof(COleFrameHook, m_retailTail) +
                  offsetof(COleFrameHook::RetailTailStorage, m_hAccelTable) == 0x68,
              "COleFrameHook retail accelerator storage at +0x68");
static_assert(offsetof(COleFrameHook, m_retailTail) +
                  offsetof(COleFrameHook::RetailTailStorage, m_nModelessCount) == 0x70,
              "COleFrameHook retail modeless count at +0x70");
static_assert(offsetof(COleFrameHook, m_retailTail) +
                  offsetof(COleFrameHook::RetailTailStorage, m_reserved_74) == 0x74,
              "COleFrameHook retail alignment word at +0x74");
static_assert(offsetof(COleFrameHook, m_retailTail) +
                  offsetof(COleFrameHook::RetailTailStorage, m_strObjNameStorage) == 0x78,
              "COleFrameHook retail CString storage at +0x78");
static_assert(offsetof(COleFrameHook, m_retailTail) +
                  offsetof(COleFrameHook::RetailTailStorage, m_xOleInPlaceFrameVftable) == 0x80,
              "COleFrameHook retail in-place-frame part storage at +0x80");

#define ASSERT_FRAMEWND_FIELD(member, offset) \
    static_assert(offsetof(CFrameWnd, member) == (offset), "CFrameWnd::" #member)
static_assert(sizeof(CFrameWnd) == 0x1d8, "CFrameWnd size is unchanged");
static_assert(sizeof(CFrameWnd::RetailPrefixStorage) == 0x10,
              "CFrameWnd prefix overlay size is unchanged");
static_assert(sizeof(CFrameWnd::RetailMiddleStorage) == 0x64,
              "CFrameWnd middle overlay size is unchanged");
static_assert(sizeof(CFrameWnd::RetailTailStorage) == 0x60,
              "CFrameWnd tail overlay size is unchanged");
ASSERT_FRAMEWND_FIELD(m_retailPrefix, 0xe8);
ASSERT_FRAMEWND_FIELD(m_hAccelTable, 0xf8);
ASSERT_FRAMEWND_FIELD(m_nIDHelp, 0x164);
ASSERT_FRAMEWND_FIELD(m_pViewActive, 0x170);
static_assert(offsetof(CFrameWnd, m_retailPrefix) +
                  offsetof(CFrameWnd::RetailPrefixStorage, m_bAutoMenuEnable) == 0xe8,
              "CFrameWnd retail menu enable flag");
static_assert(offsetof(CFrameWnd, m_retailPrefix) +
                  offsetof(CFrameWnd::RetailPrefixStorage, m_hMenuDefault) == 0xf0,
              "CFrameWnd retail default menu");
ASSERT_FRAMEWND_FIELD(m_retailMiddle, 0x100);
static_assert(offsetof(CFrameWnd, m_retailMiddle) +
                  offsetof(CFrameWnd::RetailMiddleStorage, m_bHelpMode) == 0x104,
              "CFrameWnd retail help-mode flag");
static_assert(offsetof(CFrameWnd, m_retailMiddle) +
                  offsetof(CFrameWnd::RetailMiddleStorage, m_rectBorder) == 0x110,
              "CFrameWnd retail border rectangle");
static_assert(offsetof(CFrameWnd, m_retailMiddle) +
                  offsetof(CFrameWnd::RetailMiddleStorage, m_pNotifyHook) == 0x120,
              "CFrameWnd retail notification-hook pointer");
ASSERT_FRAMEWND_FIELD(m_retailTail, 0x178);
static_assert(offsetof(CFrameWnd, m_retailTail) +
                  offsetof(CFrameWnd::RetailTailStorage, m_dwBorderFlags) == 0x1d0,
              "CFrameWnd retail border flags");
#undef ASSERT_FRAMEWND_FIELD

// Retail's m_lpActiveObject (+0x48) collides with OpenMFC's m_pActiveItem at
// that offset, while retail m_pActiveItem (+0x50) collides with OpenMFC's
// m_lpActiveUIWindow. Keep both retail values in the pThis-keyed state rather
// than interpreting either public member as the other.
inline IOleInPlaceActiveObject* ActiveObject(const COleFrameHook* p) {
    if (!p)
        return nullptr;
    FrameHookState* state = GetFrameHookState(const_cast<COleFrameHook*>(p), false);
    return state ? state->activeObject : nullptr;
}
inline COleClientItem* ActiveItem(const COleFrameHook* p) {
    if (!p)
        return nullptr;
    FrameHookState* state = GetFrameHookState(const_cast<COleFrameHook*>(p), false);
    return state ? state->activeItem : nullptr;
}

// Retail guards these callbacks with a non-null active object and verifies
// item->m_pInPlaceFrame == this at COleClientItem +0xa8. The item hook field
// is overlaid and offset-asserted in afxole.h / COleClientItem.cpp, so this
// helper can perform retail's exact equality test without side-table linkage.
inline bool ActiveForThisFrame(const COleFrameHook* p) {
    COleClientItem* item = ActiveItem(p);
    if (ActiveObject(p) == nullptr || item == nullptr)
        return false;
#if defined(__GNUC__)
    // The non-MSVC declaration intentionally has no retail tail overlay.
    // Per-file GNU compilation is therefore conservative; the shipping MSVC
    // ABI path below performs the retail +0xa8 owner-link comparison.
    return true;
#else
    return item->m_retailTail.m_pInPlaceFrame == p;
#endif
}

// METHOD_PROLOGUE for XOleInPlaceFrame backs its interface pointer up by
// exactly 0x80 bytes, as shown by the retail constructor and method prologues.
inline COleFrameHook* HookFromInPlaceFrameInterface(void* pInterface) {
    return reinterpret_cast<COleFrameHook*>(
        static_cast<unsigned char*>(pInterface) - 0x80);
}

// Retail COleDocObjectItem keeps these members in its derived tail. The
// named RetailTailStorage overlays that same tail and is statically pinned at
// the retail +0xf0 and +0x108 offsets. The XOleDocumentSite vftable storage at
// +0x110 is still not initialized by the OpenMFC constructor.
constexpr size_t kOffDocObjectHelpPopupMenu = 0xf0;
constexpr size_t kOffDocObjectInHelpMenu = 0x108;
static_assert(sizeof(COleDocObjectItem) == 0x118,
              "COleDocObjectItem size must remain retail-sized");
#ifndef __GNUC__
static_assert(offsetof(COleDocObjectItem, m_retailTail) +
                  offsetof(COleDocObjectItem::RetailTailStorage, m_pHelpPopupMenu) ==
                  kOffDocObjectHelpPopupMenu,
              "retail m_pHelpPopupMenu at +0xf0");
static_assert(offsetof(COleDocObjectItem, m_retailTail) +
                  offsetof(COleDocObjectItem::RetailTailStorage, m_pActiveView) == 0xf8,
              "retail m_pActiveView at +0xf8");
static_assert(offsetof(COleDocObjectItem, m_retailTail) +
                  offsetof(COleDocObjectItem::RetailTailStorage, m_pIPrint) == 0x100,
              "retail m_pIPrint at +0x100");
static_assert(offsetof(COleDocObjectItem, m_retailTail) +
                  offsetof(COleDocObjectItem::RetailTailStorage, m_bInHelpMenu) ==
                  kOffDocObjectInHelpMenu,
              "retail m_bInHelpMenu at +0x108");
static_assert(offsetof(COleDocObjectItem, m_retailTail) +
                  offsetof(COleDocObjectItem::RetailTailStorage, m_xOleDocumentSiteVftable) ==
                  0x110,
              "retail XOleDocumentSite storage at +0x110");
#endif

inline COleDocObjectItem* AsDocObjectItem(COleClientItem* pItem) {
    if (pItem == nullptr ||
        !impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(
            static_cast<const CObject*>(pItem),
            impl__GetThisClass_COleDocObjectItem__SAPEAUCRuntimeClass__XZ()))
        return nullptr;
    return static_cast<COleDocObjectItem*>(pItem);
}

inline CMenu* DocObjectHelpPopupMenu(COleDocObjectItem* pItem) {
    return pItem->m_retailTail.m_pHelpPopupMenu;
}

inline BOOL& DocObjectInHelpMenu(COleDocObjectItem* pItem) {
    return pItem->m_retailTail.m_bInHelpMenu;
}

}  // namespace

// Symbol: ??0COleFrameHook@@QEAA@PEAVCFrameWnd@@PEAVCOleClientItem@@@Z
// COleFrameHook::COleFrameHook(CFrameWnd* pFrameWnd, COleClientItem* pItem)
// An MSVC constructor export takes `this` in RCX and returns it; the caller
// has already allocated the storage (retail's own caller does exactly that:
// `mov $0x90,%ecx ; call operator new ; ... ; call 0x1802461f0` at mfc140
// 0x248392..0x2483b1 inside COleClientItem::XOleIPSite::GetWindowContext).
// This thunk used to be declared (CFrameWnd*, COleClientItem*) and to heap-
// allocate with a plain `new`, which read the caller's `this` as pFrameWnd,
// pFrameWnd as pItem, dropped pItem entirely and leaked a second object while
// leaving the caller's storage unconstructed.  Constructing in the caller's
// storage is how the other ctor thunks in this tree work too (placement new at
// e.g. core/ole/Thunks.cpp:1148; core/gdi/CRectTracker.cpp:230 initialises
// pThis by hand instead).
extern "C" void* MS_ABI impl___0COleFrameHook__QEAA_PEAVCFrameWnd__PEAVCOleClientItem___Z(
    void* pThis, CFrameWnd* pFrameWnd, COleClientItem* pItem) {
    if (pThis == nullptr)
        return nullptr;
    return new (pThis) COleFrameHook(pFrameWnd, pItem);
}
// Retail ctor entry RVA 0x247d90 (mfc140u): CCmdTarget::CCmdTarget, plant
// the vftables, then initialize m_lpActiveObject, m_pActiveItem, m_pFrameWnd,
// m_hWndFrame, the accelerator, modeless count and empty CString, and finally
// write the frame's m_pNotifyHook at +0x120. OpenMFC models the frame pointer
// and same-sized tail, initializes the modeled frame back-pointer, and mirrors
// the CString in pThis-keyed FrameHookState; compatible COM interface vtables
// are still absent.
COleFrameHook::COleFrameHook(CFrameWnd* pFrameWnd, COleClientItem* pItem)
    : m_pFrameWnd(pFrameWnd), m_pActiveItem(pItem), m_lpActiveUIWindow(nullptr) {
    memset(_oleframehook_padding, 0, sizeof(_oleframehook_padding));
    m_retailTail.m_hWndFrame = pFrameWnd ? pFrameWnd->m_hWnd : nullptr;
    if (pFrameWnd)
        *reinterpret_cast<void**>(pFrameWnd->m_retailMiddle.m_pNotifyHook) = this;
    if (FrameHookState* state = GetFrameHookState(this, true)) {
        state->activeItem = pItem;
        state->frameWindow = m_retailTail.m_hWndFrame;
    }
}
// Retail dtor entry RVA 0x247e80 (mfc140u): restore the vftable, clear the
// frame back-pointer when it still names this hook, release m_strObjName's
// CStringData, then tail-jump to ~CCmdTarget. OpenMFC clears the now-modeled
// frame back-pointer and removes its side-table association; the CString
// object itself remains represented in the state table.
COleFrameHook::~COleFrameHook() {
    if (m_pFrameWnd &&
        *reinterpret_cast<void**>(m_pFrameWnd->m_retailMiddle.m_pNotifyHook) == this)
        *reinterpret_cast<void**>(m_pFrameWnd->m_retailMiddle.m_pNotifyHook) = nullptr;
    RemoveFrameHookState(this);
}
// STUB. Retail OnDocActivate entry RVA 0x247f70 (mfc140u, vftable slot 25). Its first act is
//     if (m_lpActiveObject == NULL) return TRUE;
// (mfc140u entry RVA 0x247f70: the null check is the first body operation).
// Everything after that remains unreachable until a real SetActiveObject path
// populates the side-table state, so for the record it is:
//     m_lpActiveObject->OnDocWindowActivate(bActivate);      // vtbl 0x38, slot 7
//     COleFrameHook* pHook = m_pActiveItem->[+0xa8];         // 0x247fa1
//     pHook->m_pFrameWnd->[+0x1d0] |= 2;                     // 0x247fac
//     if (bActivate) {
//         pHook->m_pFrameWnd->[+0x120] = pHook;              // 0x248021
//     } else {
//         IOleInPlaceFrame* f = &pHook->m_xOleInPlaceFrame;  // pHook + 0x80
//         f->SetBorderSpace(NULL);                           // vtbl 0x38, slot 7
//         if (m_pActiveItem->[+0xb0] != NULL)                // the doc-frame hook
//             ((IOleInPlaceFrame*)(m_pActiveItem->[+0xb0] + 0x80))->SetBorderSpace(NULL);
//         f->SetMenu(NULL, NULL, NULL);                      // vtbl 0x50, slot 10
//         if (pHook != this) pHook->m_pFrameWnd->[+0x120] = NULL;   // 0x248014
//     }
//     return bActivate;                                      // NOT TRUE  (0x24802c)
// which now has named COleClientItem +0xa8 / +0xb0 storage but still needs a
// compatible XOleInPlaceFrame sub-object. The retail active-object value is
// deliberately kept in FrameHookState (not the colliding m_lpActiveUIWindow
// member). SetActiveObject now updates that state, but its +0x80 interface is
// unpublished, so ordinary COM callers cannot reach the setter. This typed
// method still returns TRUE unconditionally and therefore matches only the
// null-active-object early-out; the later retail path remains unimplemented.
// The exported thunk for this symbol lives in core/ole/Thunks.cpp, not here.
BOOL COleFrameHook::OnDocActivate(BOOL bActivate) {
    (void)bActivate;
    return TRUE;
}
// Retail's override is `BOOL OnContextHelp(BOOL bEnter)`
// (?OnContextHelp@COleFrameHook@@UEAAHH@Z, vftable slot 26).  OpenMFC's
// afxole.h declares it with no parameter, so this typed overload cannot be
// given retail's body -- there is no bEnter to forward.  The real behaviour is
// implemented in the exported thunk below; a headerRequest asks for the
// declaration to take BOOL bEnter.
BOOL COleFrameHook::OnContextHelp() {
    return FALSE;
}
// mfc140u exports no ?OnResizeBorder@COleFrameHook@@ and no
// ?OnGetClipboardData@COleFrameHook@@ (checked against
// mfc_complete_ordinal_mapping.json, which lists 33 COleFrameHook symbols, and
// against both RVA maps), and neither appears in the class vftable at
// mfc140u .rdata 0x32c0f0, whose own slots are exactly the twelve listed in
// the header comment.  These two virtuals are OpenMFC additions with no retail
// counterpart to transcribe, so they keep their existing FALSE bodies.
BOOL COleFrameHook::OnResizeBorder(LPCRECT lpRect, LPOLEINPLACEUIWINDOW lpUIWindow, BOOL bMainWindow) {
    (void)lpRect; (void)lpUIWindow; (void)bMainWindow;
    return FALSE;
}
BOOL COleFrameHook::OnGetClipboardData(COleDataSource* pDataSource) {
    (void)pDataSource;
    return FALSE;
}

// === Moved from ManualThunks.cpp ===
// Symbol: ?OnContextHelp@COleFrameHook@@UEAAHH@Z
// COleFrameHook::OnContextHelp(BOOL bEnter) -- vftable slot 26,
// mfc140u 0x248040 / mfc140 0x2464a0, transcribed whole:
//     if (m_lpActiveObject == NULL)              goto ret_true;   // 0x248040
//     if (m_pActiveItem->m_pInPlaceFrame != this) goto ret_true;  // 0x248047
//     return NotifyAllInPlace(bEnter,
//                             &COleFrameHook::DoContextSensitiveHelp); // jmp 0x2483e0
//   ret_true: return TRUE;                                            // 0x248060
// The r8 the tail-jump passes is 0x180248560, which IS
// COleFrameHook::DoContextSensitiveHelp (its own entry, per the mfc140u map).
// ActiveForThisFrame compares the overlaid COleClientItem +0xa8 hook field.
extern "C" int MS_ABI impl__OnContextHelp_COleFrameHook__UEAAHH_Z(COleFrameHook* pThis,
                                                                  int bEnter) {
    if (!ActiveForThisFrame(pThis))
        return TRUE;
    return impl__NotifyAllInPlace_COleFrameHook__QEAAHHP81_EAAHH_Z_Z(
        pThis, bEnter, &impl__DoContextSensitiveHelp_COleFrameHook__QEAAHH_Z);
}

// Symbol: ?DoContextSensitiveHelp@COleFrameHook@@QEAAHH@Z
// mfc140u 0x248560 / mfc140 0x2469c0, the whole body:
//     mov 0x48(%rcx),%rcx              ; m_lpActiveObject
//     mov (%rcx),%rax ; mov 0x20(%rax),%rax ; call *...
//     not %eax ; shr $0x1f,%eax        ; return SUCCEEDED(hr)
// vtable byte 0x20 is slot 4 of IOleInPlaceActiveObject, i.e.
// IOleWindow::ContextSensitiveHelp, and rdx (bEnter) is passed through
// untouched.  Deviation: retail dereferences m_lpActiveObject without a null
// test because its only callers (OnContextHelp -> NotifyAllInPlace) have
// already established it; the guard below is unreachable on those paths and
// answers TRUE, the value that lets NotifyAllInPlace's walk continue.
extern "C" int MS_ABI impl__DoContextSensitiveHelp_COleFrameHook__QEAAHH_Z(
    COleFrameHook* pThis, int bEnter) {
    IOleInPlaceActiveObject* pActiveObject = pThis ? ActiveObject(pThis) : nullptr;
    if (pActiveObject == nullptr)
        return TRUE;
    return SUCCEEDED(pActiveObject->ContextSensitiveHelp(bEnter)) ? TRUE : FALSE;
}

// Symbol: ?DoEnableModeless@COleFrameHook@@QEAAHH@Z
// Retail entry RVA 0x248580 (mfc140u): maintain the +0x70 nesting count and
// call IOleInPlaceActiveObject::EnableModeless only on the 0->1 / 1->0 edges.
// The counter is the modeled m_retailTail.m_nModelessCount (offset asserted
// above); FrameHookState supplies the active-object pointer. Nested transitions
// that leave the count nonzero do not call the COM method.
extern "C" int MS_ABI impl__DoEnableModeless_COleFrameHook__QEAAHH_Z(COleFrameHook* pThis,
                                                                     int bEnable) {
    const UINT count = pThis->m_retailTail.m_nModelessCount;
    if (!bEnable) {
        pThis->m_retailTail.m_nModelessCount = count + 1;
        if (count == 0)
            ActiveObject(pThis)->EnableModeless(FALSE);
    } else {
        if (count == 0)
            return TRUE;
        pThis->m_retailTail.m_nModelessCount = count - 1;
        if (count == 1)
            ActiveObject(pThis)->EnableModeless(TRUE);
    }
    return TRUE;
}

// Symbol: ?NotifyAllInPlace@COleFrameHook@@QEAAHHP81@EAAHH@Z@Z
// STUB.  mfc140u 0x2483e0 / mfc140 0x246840 broadcasts a hook callback to
// every in-place item hosted inside this frame:
// What the disassembly literally establishes is the offsets, the vtable bytes
// and the three callees the map does name; the MFC class names attached to the
// enumeration steps below come from MFC's documented API shape.
//     HWND hwndFrame = m_hWndFrame;                       // [this+0x58]
//     void* pApp     = AfxGetModuleState()->[+0x08];      // the module's app object
//     void* pMgr     = pApp->[+0xb8];                     // its document manager
//     if (pMgr == NULL) return TRUE;
//     for (POSITION p = pMgr->vf[0x30](); p != NULL; )              // slot 6
//     { void* pT = pMgr->vf[0x38](&p);             // slot 7 -- a doc template
//       for (POSITION q = pT->vf[0xb8](); q != NULL; )              // slot 23
//       { CObject* pDoc = pT->vf[0xc0](&q);                         // slot 24
//         if (!pDoc->IsKindOf(RUNTIME_CLASS(COleDocument))) continue;
//         for (POSITION r = pDoc->vf[0x238]();;)                    // slot 71
//         { COleClientItem* pItem =            // call mfc140u 0x180254120, which the
//               pDoc->GetNextItemOfKind(       // mfc140u map DOES name:
//                   r, RUNTIME_CLASS(COleClientItem));  // ?GetNextItemOfKind@
//                                              //   COleDocument@@IEBAPEAVCDocItem@@
//                                              //   AEAPEAU__POSITION@@PEAUCRuntimeClass@@@Z
//           if (pItem == NULL) break;
//           COleFrameHook* pHook = pItem->[+0xa8];
//           if (pHook == NULL || pHook->m_lpActiveObject == NULL) continue;
//           CView* pView = pItem->[+0x98];
//           if (pView == NULL) continue;
//           if (!AfxIsDescendant(hwndFrame, pView->m_hWnd)) continue;
//           if (!(*pfn)(pHook, bParam)) return FALSE;   // 0x2484f5
//         } } }
//     return TRUE;
// (The two RUNTIME_CLASS operands are mfc140u 0x18032ea80 = COleDocument
// (m_nObjectSize 0x1e8) and 0x18032e788 = COleClientItem (0xf0), read straight
// out of the descriptors; the callees at 0x180133930, 0x180234cf0, 0x18029d3a0
// resolve to AfxGetModuleState, CObject::IsKindOf and AfxIsDescendant, and the
// `call *0x1802c7b30` used for every indirect call is
// __guard_dispatch_icall_fptr, not an import.)
// The item hook (+0xa8) is an asserted COleClientItem tail field; the view
// (+0x98) and this hook's frame HWND (+0x58) are represented by family state.
// The remaining blocker
// is exact retail enumeration through the active module state's app/document
// manager/template vtables, which this file does not model. Substituting the
// process-wide side tables would include documents outside the retail walk and
// would not preserve its ordering. The callback keeps its recovered signature
// for when that traversal is available.
extern "C" int MS_ABI impl__NotifyAllInPlace_COleFrameHook__QEAAHHP81_EAAHH_Z_Z(
    COleFrameHook* pThis, int bParam, PFN_HOOKNOTIFY pfn) {
    (void)pThis;
    (void)bParam;
    (void)pfn;
    return TRUE;
}

// Symbol: ?OnActivate@COleFrameHook@@UEAAXH@Z
// COleFrameHook::OnActivate(BOOL bActive) -- vftable slot 24,
// mfc140u 0x2482b0 / mfc140 0x246710, transcribed whole:
//     if (m_lpActiveObject == NULL) return;
//     if (m_pActiveItem->m_pInPlaceFrame != this) return;
//     if (!m_pFrameWnd->IsWindowEnabled()) return;          // call 0x1802a9b00
//     m_lpActiveObject->OnFrameWindowActivate(bActive);     // vtable 0x30 = slot 6
// 0x1802a9b00 is ?IsWindowEnabled@CWnd@@QEBAHXZ per the mfc140u map.
// ActiveForThisFrame compares the overlaid COleClientItem +0xa8 hook field.
extern "C" void MS_ABI impl__OnActivate_COleFrameHook__UEAAXH_Z(COleFrameHook* pThis,
                                                                int bActive) {
    if (!ActiveForThisFrame(pThis))
        return;
    if (!impl__IsWindowEnabled_CWnd__QEBAHXZ(pThis->m_pFrameWnd))
        return;
    ActiveObject(pThis)->OnFrameWindowActivate(bActive);
}

// Symbol: ?OnEnableModeless@COleFrameHook@@UEAAXH@Z
// COleFrameHook::OnEnableModeless(BOOL bEnable) -- vftable slot 27,
// mfc140u 0x248300 / mfc140 0x246760, transcribed whole:
//     if (m_lpActiveObject == NULL) return;
//     if (m_pActiveItem->m_pInPlaceFrame != this) return;
//     NotifyAllInPlace(bEnable, &COleFrameHook::DoEnableModeless);
//                                  // call at mfc140u 0x24831f (mfc140 0x24677f)
// The r8 the call passes is mfc140u 0x180248580 (mfc140 0x1802469e0), which is
// DoEnableModeless's own entry in each image's map.  The
// BOOL NotifyAllInPlace returns is discarded (this override returns void).
// ActiveForThisFrame checks the overlaid +0xa8 link; the callback itself is
// implemented, but retail document-manager enumeration remains unavailable.
extern "C" void MS_ABI impl__OnEnableModeless_COleFrameHook__UEAAXH_Z(COleFrameHook* pThis,
                                                                      int bEnable) {
    if (!ActiveForThisFrame(pThis))
        return;
    impl__NotifyAllInPlace_COleFrameHook__QEAAHHP81_EAAHH_Z_Z(
        pThis, bEnable, &impl__DoEnableModeless_COleFrameHook__QEAAHH_Z);
}

// Symbol: ?OnInitMenu@COleFrameHook@@UEAAXPEAVCMenu@@@Z
// Retail entry RVA 0x246580 (mfc140, ANSI twin): read m_pActiveItem, and if it
// is a COleDocObjectItem clear m_bInHelpMenu at item+0x108. The installed
// retail afxole.h names that member; pMenu is not read. The Unicode symbol did
// not resolve by name in the RVA map, so this transcription cites the ANSI
// entry (the function bodies are byte-identical between the two images).
// OpenMFC reserves the relevant COleDocObjectItem bytes in padding; the
// file-local size/offset assertions above pin the access. headerRequests asks
// to name these retail members in the public declaration.
extern "C" void MS_ABI impl__OnInitMenu_COleFrameHook__UEAAXPEAVCMenu___Z(
    COleFrameHook* pThis, CMenu* pMenu) {
    (void)pMenu;
    COleDocObjectItem* pItem = AsDocObjectItem(ActiveItem(pThis));
    if (pItem != nullptr)
        DocObjectInHelpMenu(pItem) = FALSE;
}

// Symbol: ?OnInitMenuPopup@COleFrameHook@@UEAAHPEAVCMenu@@HH@Z
// Retail entry RVA 0x2465c0 (mfc140, ANSI twin): return FALSE for a system
// menu, a missing/non-COleDocObjectItem active item, or a clear m_bInHelpMenu
// flag (+0x108). Then call GetInPlaceWindow; if non-null, send
// WM_INITMENUPOPUP to its HWND with pMenu->m_hMenu and LOWORD(nIndex), and
// return TRUE. pMenu is dereferenced without a retail null check. The Unicode
// import slot 0x1802c7120 resolves to USER32!SendMessageW (iatu.py); this uses
// ::SendMessage so UNICODE selects that API. The GetInPlaceWindow thunk is a
// documented null-returning stub today, so the final send is currently
// unreachable in this build, but the control flow mirrors retail.
extern "C" int MS_ABI impl__OnInitMenuPopup_COleFrameHook__UEAAHPEAVCMenu__HH_Z(
    COleFrameHook* pThis, CMenu* pMenu, int nIndex, int bSysMenu) {
    if (bSysMenu)
        return FALSE;
    COleDocObjectItem* pItem = AsDocObjectItem(ActiveItem(pThis));
    if (pItem == nullptr || !DocObjectInHelpMenu(pItem))
        return FALSE;
    CWnd* pWnd = impl__GetInPlaceWindow_COleClientItem__QEAAPEAVCWnd__XZ(pItem);
    if (pWnd == nullptr)
        return FALSE;
    ::SendMessage(pWnd->m_hWnd, WM_INITMENUPOPUP,
                  reinterpret_cast<WPARAM>(pMenu->m_hMenu),
                  static_cast<LPARAM>(static_cast<WORD>(nIndex)));
    return TRUE;
}

// Symbol: ?OnMenuSelect@COleFrameHook@@UEAAHIIPEAUHMENU__@@@Z
// Retail entry RVA 0x2464d0 (mfc140, ANSI twin): reject a missing or
// non-COleDocObjectItem active item, then call GetInPlaceWindow. For MF_POPUP,
// compare hSysMenu with m_pHelpPopupMenu->m_hMenu (+0xf0), set m_bInHelpMenu
// (+0x108) to (nItemID != 0), and reject nItemID == 0. For a non-popup item,
// require m_bInHelpMenu. Both paths then require a non-null in-place window,
// send WM_MENUSELECT with MAKEWPARAM(LOWORD(nItemID), LOWORD(nFlags)) and
// hSysMenu, and return TRUE. The Unicode import slot 0x1802c7120 resolves to
// USER32!SendMessageW (iatu.py); ::SendMessage follows UNICODE. The in-place
// window thunk currently returns NULL, but the menu-selection state update is
// still performed in the same order as retail.
extern "C" int MS_ABI impl__OnMenuSelect_COleFrameHook__UEAAHIIPEAUHMENU_____Z(
    COleFrameHook* pThis, unsigned int nItemID, unsigned int nFlags, HMENU hSysMenu) {
    COleDocObjectItem* pItem = AsDocObjectItem(ActiveItem(pThis));
    if (pItem == nullptr)
        return FALSE;
    CWnd* pWnd = impl__GetInPlaceWindow_COleClientItem__QEAAPEAVCWnd__XZ(pItem);
    if ((nFlags & MF_POPUP) != 0) {
        CMenu* pHelpMenu = DocObjectHelpPopupMenu(pItem);
        HMENU hHelpMenu = pHelpMenu ? pHelpMenu->m_hMenu : nullptr;
        if (hHelpMenu != hSysMenu)
            return FALSE;
        DocObjectInHelpMenu(pItem) = nItemID != 0 ? TRUE : FALSE;
        if (nItemID == 0)
            return FALSE;
    } else if (!DocObjectInHelpMenu(pItem)) {
        return FALSE;
    }
    if (pWnd == nullptr)
        return FALSE;
    ::SendMessage(pWnd->m_hWnd, WM_MENUSELECT,
                  MAKEWPARAM(LOWORD(nItemID), LOWORD(nFlags)),
                  reinterpret_cast<LPARAM>(hSysMenu));
    return TRUE;
}

// Symbol: ?OnPaletteChanged@COleFrameHook@@UEAAXPEAVCWnd@@@Z
// COleFrameHook::OnPaletteChanged(CWnd* pFocusWnd) -- vftable slot 29,
// mfc140u 0x248360 / mfc140 0x2467c0, transcribed whole:
//     CWnd* pWnd = m_pActiveItem->GetInPlaceWindow();  // call mfc140u 0x1802492a0
//     if (pWnd == NULL) return;
//     ::SendMessage(pWnd->m_hWnd, WM_PALETTECHANGED /* 0x311 */,
//                   (WPARAM)(pFocusWnd ? pFocusWnd->m_hWnd : NULL), 0);
// The import slot is mfc140u 0x1802c7120 = USER32!SendMessageW (the same call
// site reads mfc140 0x1802c5378 = SendMessageA in the ANSI twin), so this is
// ::SendMessage with UNICODE picking SendMessageW.
// Deviation: retail loads m_pActiveItem and calls straight through it with no
// null test; the guard below returns instead of faulting.
// COleClientItem::GetInPlaceWindow is a documented stub in this tree today
// (core/ole/COleClientItem.cpp:654 always returns NULL), so this body is a
// no-op until that lands -- but the composition is retail's.
extern "C" void MS_ABI impl__OnPaletteChanged_COleFrameHook__UEAAXPEAVCWnd___Z(
    COleFrameHook* pThis, CWnd* pFocusWnd) {
    COleClientItem* activeItem = ActiveItem(pThis);
    if (!activeItem)
        return;
    CWnd* pWnd = impl__GetInPlaceWindow_COleClientItem__QEAAPEAVCWnd__XZ(activeItem);
    if (pWnd == nullptr)
        return;
    ::SendMessage(pWnd->m_hWnd, WM_PALETTECHANGED,
                  reinterpret_cast<WPARAM>(pFocusWnd ? pFocusWnd->m_hWnd : nullptr), 0);
}

// Symbol: ?OnPreTranslateMessage@COleFrameHook@@UEAAHPEAUtagMSG@@@Z
// Retail entry RVA 0x246640 (mfc140, ANSI twin). With a non-null active
// object and the active item's in-place-frame link equal to this hook, key
// messages WM_KEYFIRST..WM_KEYLAST are passed to TranslateAccelerator and
// return TRUE only for S_OK. WM_COMMAND is handled only for a
// COleDocObjectItem whose m_bInHelpMenu (+0x108) is set: its in-place window,
// if any, receives SendNotifyMessage(WM_COMMAND, wParam, lParam), and the
// result is whether that call returned nonzero. The Unicode IAT slot
// 0x1802c6fb8 resolves to USER32!SendNotifyMessageW (iatu.py); the ANSI twin's
// corresponding call is SendNotifyMessageA. The exact +0xa8 in-place-frame
// exact +0xa8 hook-pointer equality is checked by ActiveForThisFrame. This body
// also retains a defensive null-pMsg check absent in retail. The in-place
// window thunk is currently a null-returning stub.
extern "C" int MS_ABI impl__OnPreTranslateMessage_COleFrameHook__UEAAHPEAUtagMSG___Z(
    COleFrameHook* pThis, MSG* pMsg) {
    if (!ActiveForThisFrame(pThis) || pMsg == nullptr)
        return FALSE;
    if (static_cast<unsigned int>(pMsg->message - WM_KEYFIRST) <= 9u)
        return ActiveObject(pThis)->TranslateAccelerator(pMsg) == S_OK ? TRUE : FALSE;
    if (pMsg->message != WM_COMMAND)
        return FALSE;
    COleDocObjectItem* pItem = AsDocObjectItem(ActiveItem(pThis));
    if (pItem == nullptr || !DocObjectInHelpMenu(pItem))
        return FALSE;
    LRESULT result = 0;
    CWnd* pWnd = impl__GetInPlaceWindow_COleClientItem__QEAAPEAVCWnd__XZ(pItem);
    if (pWnd != nullptr)
        result = ::SendNotifyMessage(pWnd->m_hWnd, WM_COMMAND, pMsg->wParam, pMsg->lParam);
    return result != 0 ? TRUE : FALSE;
}

// Symbol: ?OnQueryNewPalette@COleFrameHook@@UEAAHXZ
// COleFrameHook::OnQueryNewPalette() -- vftable slot 30,
// mfc140u 0x2483a0 / mfc140 0x246800, transcribed whole:
//     CWnd* pWnd = m_pActiveItem->GetInPlaceWindow();  // call mfc140u 0x1802492a0
//     if (pWnd == NULL) return FALSE;          // falls out with eax == 0
//     return ::SendMessage(pWnd->m_hWnd, WM_QUERYNEWPALETTE /* 0x30f */, 0, 0) != 0;
//                                              // slot mfc140u 0x1802c7120
// Deviation: the same missing null test on m_pActiveItem as OnPaletteChanged.
// GetInPlaceWindow is a stub in this tree today, so this returns FALSE until
// that lands.
extern "C" int MS_ABI impl__OnQueryNewPalette_COleFrameHook__UEAAHXZ(COleFrameHook* pThis) {
    COleClientItem* activeItem = ActiveItem(pThis);
    if (!activeItem)
        return FALSE;
    CWnd* pWnd = impl__GetInPlaceWindow_COleClientItem__QEAAPEAVCWnd__XZ(activeItem);
    if (pWnd == nullptr)
        return FALSE;
    return ::SendMessage(pWnd->m_hWnd, WM_QUERYNEWPALETTE, 0, 0) != 0 ? TRUE : FALSE;
}

// Symbol: ?OnRecalcLayout@COleFrameHook@@UEAAXXZ
// STUB.  vftable slot 22, mfc140u 0x247ef0 / mfc140 0x246350, whole body:
//     if (m_lpActiveObject == NULL) return;
//     RECT rc;
//     m_pFrameWnd->NegotiateBorderSpace(1 /* borderGet */, &rc);   // vtbl 0x348
//     m_lpActiveObject->ResizeBorder(&rc, &m_xOleInPlaceFrame,     // vtbl 0x40 = slot 8
//                                    m_pActiveItem->m_pInPlaceFrame == this);
// The second argument is the hook's own XOleInPlaceFrame sub-object (this+0x80).
// OpenMFC names that same-sized storage but has no compatible vtable there,
// and its interface map exposes no IOleInPlaceUIWindow. Passing that zeroed
// slot would not provide the callback target ResizeBorder requires. The
// NegotiateBorderSpace(borderGet) call alone only fills a local RECT, so no
// partial body is useful. See state_ole.md for the remaining interface blocker.
extern "C" void MS_ABI impl__OnRecalcLayout_COleFrameHook__UEAAXXZ(COleFrameHook* pThis) {
    (void)pThis;
}

// Symbol: ?OnUpdateFrameTitle@COleFrameHook@@UEAAHXZ
// COleFrameHook::OnUpdateFrameTitle() -- vftable slot 28,
// mfc140u 0x248330 / mfc140 0x246790, transcribed whole:
//     if (m_lpActiveObject == NULL) return FALSE;
//     if (m_pActiveItem->m_pInPlaceFrame != this) return FALSE;
//     return m_pActiveItem->vf[0x170]();       // tail jmp through the vtable
// COleClientItem vftable byte 0x170 is OnUpdateFrameTitle; that slot is
// identified, and its retail base body (`xor %eax,%eax ; ret`) transcribed, at
// core/ole/COleClientItem.cpp:2155.
// Deviation: ActiveForThisFrame reads the overlaid +0xa8 hook link, and the
// virtual dispatch is devirtualized -- OpenMFC's
// COleClientItem does not declare OnUpdateFrameTitle virtual (it is not in the
// class declaration in afxole.h at all), so the base thunk is called directly
// and a COleDocObjectItem override would not be reached.
extern "C" int MS_ABI impl__OnUpdateFrameTitle_COleFrameHook__UEAAHXZ(COleFrameHook* pThis) {
    if (!ActiveForThisFrame(pThis))
        return FALSE;
    return impl__OnUpdateFrameTitle_COleClientItem__UEAAHXZ(ActiveItem(pThis));
}

// ---------------------------------------------------------------------------
// COleFrameHook::XOleInPlaceFrame (IOleInPlaceFrame) and
// COleFrameHook::XOleCommandTarget (IOleCommandTarget).
//
// The remaining unimplemented methods below are blocked by incomplete
// interface publication or sibling infrastructure: retail backs incoming
// XOleInPlaceFrame pointers up by 0x80 and XOleCommandTarget pointers by 0x88.
// OpenMFC reserves the +0x80 storage but installs no compatible vtable there;
// +0x88 cannot fit in the unchanged clang-cl object. The interface map
// publishes neither part, so COM callers cannot currently obtain these
// pointers. ContextSensitiveHelp, EnableModeless, GetBorder, GetWindow,
// RequestBorderSpace, SetActiveObject, SetBorderSpace, SetStatusText, and
// TranslateAccelerator have bodies using available state/callees; this does
// not publish the missing vtable. SetActiveObject omits CString mutation.
// The bodies needing AFX_MAINTAIN_STATE2 use its thunk-backed scope. The state
// additions name CFrameWnd help mode, border RECT/flags, menu, notify-hook and
// accelerator storage, the
// COleClientItem +0xa8/+0xb0 hook links, and frame-hook tail state. The
// slot-safe NegotiateBorderSpace adapter is in OlecoreSupport.h. The remaining
// E_NOTIMPL returns are honest placeholders for methods that still need the
// missing interface/command-target infrastructure; they are not retail
// HRESULTs. The current object declaration is 0x88 bytes in both the per-file
// GNU checker and clang-cl ABI check; the +0x80 slot is storage only, not a
// published part.
//
// Retail behaviour, so that filling these in later needs no second pass.
// First RVA is mfc140 (ANSI, where every body below was read); the mfc140u RVA
// is given where mfc140u_rva_symbols.json resolves the export.  H = the
// recovered COleFrameHook.  "vtbl 0xNN" on H->m_pFrameWnd is the CFrameWnd
// vftable byte offset, on H->m_pActiveItem the COleClientItem one (mfc140
// .rdata 0x329c48: 0x158 OnInsertMenus, 0x160 OnSetMenu, 0x168 OnRemoveMenus
// and 0x170 OnUpdateFrameTitle are identified at core/ole/COleClientItem.cpp:
// 1535-1538; 0x178 holds mfc140 0x180246d10 =
// ?OnShowControlBars@COleClientItem@@UEAAHPEAVCFrameWnd@@H@Z, read out of
// that vftable for this comment).  Every XOleInPlaceFrame body except
// GetWindow and SetStatusText opens with AFX_MAINTAIN_STATE2 on H's CCmdTarget
// m_pModuleState (call mfc140 0x180133df0) and, where noted, seeds an
// E_UNEXPECTED (0x8000ffff) result that only the catch path returns.
//   0x247910 GetWindow            *phwnd = H->m_hWndFrame (read as -0x28(%rcx));
//                                 return *phwnd ? S_OK : E_FAIL; E_POINTER if !phwnd
//   0x247930 ContextSensitiveHelp (mfc140u 0x2494d0)
//                                 pFrame = H->m_pFrameWnd->GetTopLevelFrame()
//                                 (call mfc140 0x18028c910); if NULL,
//                                 AfxThrowInvalidArgException (0x180225b80).
//                                 !fEnterMode: pFrame->ExitHelpMode() (vtbl 0x378);
//                                 return S_OK.
//                                 fEnterMode: if pFrame->[+0x104] != 0 (retail
//                                 CFrameWnd m_bHelpMode; now named in
//                                 CFrameWnd::RetailMiddleStorage) return S_OK. Otherwise
//                                 return E_UNEXPECTED if
//                                 !pFrame->CanEnterHelpMode() (call 0x18029fbb0),
//                                 or if !H->OnContextHelp(TRUE) (H's own vtbl
//                                 0xd0 = slot 26), or if
//                                 ::PostMessage(pFrame->m_hWnd, WM_COMMAND (0x111),
//                                 ID_CONTEXT_HELP (0xe145), 0) fails; else S_OK.
//                                 WM_COMMAND with a command ID, not
//                                 WM_SYSCOMMAND/SC_CONTEXTHELP; the import slot
//                                 mfc140 0x1802c52d8 resolves to PostMessageA
//                                 (PostMessageW in mfc140u).
//   0x247a20 GetBorder            (mfc140u 0x2495c0)
//                                 BOOL b = H->m_pActiveItem->OnShowControlBars(
//                                          H->m_pFrameWnd, FALSE);   // vtbl 0x178
//                                 RECT saved = m_pFrameWnd->m_rectBorder
//                                 (+0x110, copied AFTER that call);
//                                 then three m_pFrameWnd->NegotiateBorderSpace
//                                 calls (vtbl 0x348): (3, NULL), (1, lprectBorder
//                                 -- passed unchecked), (3, &saved); then
//                                 OnShowControlBars(m_pFrameWnd, TRUE) only if b;
//                                 return S_OK.
//   0x247b30 RequestBorderSpace   return H->m_pFrameWnd->NegotiateBorderSpace(
//                                 2 /*request*/, pborderwidths) ? S_OK
//                                 : OLE_E_INVALIDRECT (0x800401a1)
//                                 (`neg ; sbb ; not ; and $0x800401a1`).
//   0x247ba0 SetBorderSpace       if (NegotiateBorderSpace(3 /*set*/, pborderwidths))
//                                 { m_pFrameWnd->[+0x1d0] &= ~0xc;
//                                   m_pFrameWnd->RecalcLayout(FALSE); } (vtbl 0x300)
//                                 then always
//                                 H->m_pActiveItem->OnShowControlBars(m_pFrameWnd,
//                                 pborderwidths == NULL); return S_OK.
//                                 +0x1d0 is named m_dwBorderFlags.
//   0x247c50 SetActiveObject      (seeds E_UNEXPECTED)  release H->m_lpActiveObject
//                                 through the unexported helper mfc140
//                                 0x18026ba84 (IUnknown slot 2, then NULL it);
//                                 H->m_lpActiveObject = pActiveObject, AddRef'd
//                                 (slot 1) when non-NULL; H->m_strObjName.Empty()
//                                 (+0x78 CString is mirrored in FrameHookState); then ONLY if
//                                 pszObjName && pActiveObject:
//                                 H->m_strObjName = pszObjName and
//                                 H->m_pActiveItem->OnUpdateFrameTitle() (vtbl
//                                 0x170); return S_OK.
//   0x247d00 InsertMenus          (mfc140u 0x2498b0; seeds E_UNEXPECTED)
//                                 H->m_pActiveItem->OnInsertMenus(          // 0x158
//                                   CMenu::FromHandle(hmenuShared), lpMenuWidths);
//                                 return S_OK.
//   0x247d80 SetMenu              (seeds E_UNEXPECTED)
//                                 H->m_pActiveItem->OnSetMenu(              // 0x160
//                                   CMenu::FromHandle(hmenuShared), holemenu,
//                                   hwndActiveObject); return S_OK.
//   0x247e20 RemoveMenus          (seeds E_UNEXPECTED)
//                                 H->m_pActiveItem->OnRemoveMenus(          // 0x168
//                                   CMenu::FromHandle(hmenuShared)); return S_OK.
//                                 (CMenu::FromHandle is the call to mfc140
//                                 0x1802a5fc0 in all three menu methods.)
//   0x247ea0 SetStatusText        no state prologue.  Builds a temporary CString
//                                 from pszStatusText when it is non-NULL (call
//                                 mfc140 0x18003b240, which in the ANSI image is
//                                 CStringA's converting operator=(const wchar_t*);
//                                 mfc140u calls its own CStringW assignment
//                                 instead, at an address not checked here), then
//                                 ::SendMessage(H->m_pFrameWnd->m_hWnd /*CWnd+0x40*/,
//                                 0x362 /* AFX WM_SETMESSAGESTRING */, 0,
//                                 text-or-NULL) -- slot mfc140 0x1802c5378 =
//                                 SendMessageA, i.e. SendMessageW in mfc140u --
//                                 releases the temporary, returns S_OK.
//   0x247f40 EnableModeless       (mfc140u 0x249b00; seeds E_UNEXPECTED)
//                                 H->m_pFrameWnd->BeginModalState() (vtbl 0x218)
//                                 when !fEnable, EndModalState() (0x220) when
//                                 fEnable; return S_OK.
//   (CFrameWnd vtbl names above: read out of the mfc140 CFrameWnd vftable at
//   .rdata 0x337fc8, whose byte 0x348 holds NegotiateBorderSpace, 0x218
//   BeginModalState, 0x220 EndModalState, 0x228 PreTranslateMessage, 0x300
//   RecalcLayout and 0x378 ExitHelpMode.)
//   0x247fc0 TranslateAccelerator (seeds E_UNEXPECTED; mfc140 export name
//                                 TranslateAcceleratorA)  saves
//                                 H->m_pFrameWnd->[+0xf8] and stores
//                                 H->m_hAccelTable there, copies the 0x30-byte MSG
//                                 to the stack, calls m_pFrameWnd->vf[0x228]
//                                 (PreTranslateMessage) on the copy, copies the
//                                 MSG back over *lpmsg, restores [+0xf8], and
//                                 returns S_OK if PreTranslateMessage returned
//                                 nonzero, else S_FALSE (`test ; sete`).  wID is
//                                 never read.
//   0x2477d0 Exec (XOleCommandTarget)        } both: if m_pActiveItem is a
//   0x247830 QueryStatus (XOleCommandTarget) } COleDocObjectItem, forward it to an
//                                 unexported helper (mfc140 0x258128 / 0x257e68)
//                                 and return its result, else return
//                                 OLECMDERR_E_UNKNOWNGROUP (0x80040104)
// ---------------------------------------------------------------------------

// Symbol: ?ContextSensitiveHelp@XOleInPlaceFrame@COleFrameHook@@UEAAJH@Z
// Retail entry RVA 0x247930 (mfc140 ANSI twin): get the top-level frame;
// leave help mode when fEnterMode is false, otherwise return S_OK if help mode
// is already active, or require CanEnterHelpMode, hook OnContextHelp(TRUE),
// and successful PostMessage(WM_COMMAND, 0xe145). Retail raises an invalid-arg
// exception for a missing top-level frame and catches exceptions into
// E_UNEXPECTED; this body maps explicit failures and caught exceptions to that
// HRESULT and uses the thunk-backed AFX module-state scope. The +0x80 interface remains
// unpublished.
extern "C" long MS_ABI impl__ContextSensitiveHelp_XOleInPlaceFrame_COleFrameHook__UEAAJH_Z(
    void* pThis, int fEnterMode) {
    try {
        COleFrameHook* hook = HookFromInPlaceFrameInterface(pThis);
        FrameHookModuleStateScope state(hook->m_pModuleState);
        CFrameWnd* frame = impl__GetTopLevelFrame_CWnd__QEBAPEAVCFrameWnd__XZ(
            hook->m_pFrameWnd);
        if (frame == nullptr)
            return E_UNEXPECTED;
        if (!fEnterMode) {
            impl__ExitHelpMode_CFrameWnd__UEAAXXZ(frame);
            return S_OK;
        }
        if (frame->m_retailMiddle.m_bHelpMode)
            return S_OK;
        if (!impl__CanEnterHelpMode_CFrameWnd__QEAAHXZ(frame) ||
            !impl__OnContextHelp_COleFrameHook__UEAAHH_Z(hook, TRUE) ||
            !::PostMessage(frame->m_hWnd, WM_COMMAND, 0xe145u, 0))
            return E_UNEXPECTED;
        return S_OK;
    } catch (...) {
        return E_UNEXPECTED;
    }
}

// Symbol: ?EnableModeless@XOleInPlaceFrame@COleFrameHook@@UEAAJH@Z
// Retail entry RVA 0x249b00 (mfc140u): call BeginModalState when disabling,
// EndModalState when enabling, then return S_OK. The normal path uses the
// available CFrameWnd thunks and the thunk-backed MFC module-state scope; the
// catch-all maps thrown exceptions to E_UNEXPECTED. The
// constructor still does not publish a compatible +0x80 interface vtable.
extern "C" long MS_ABI impl__EnableModeless_XOleInPlaceFrame_COleFrameHook__UEAAJH_Z(
    void* pThis, int fEnable) {
    COleFrameHook* hook = HookFromInPlaceFrameInterface(pThis);
    try {
        FrameHookModuleStateScope state(hook->m_pModuleState);
        if (fEnable)
            impl__EndModalState_CFrameWnd__UEAAXXZ(hook->m_pFrameWnd);
        else
            impl__BeginModalState_CFrameWnd__UEAAXXZ(hook->m_pFrameWnd);
    } catch (...) {
        return E_UNEXPECTED;
    }
    return S_OK;
}

// Symbol: ?Exec@XOleCommandTarget@COleFrameHook@@UEAAJPEBU_GUID@@KKPEAUtagVARIANT@@1@Z
extern "C" long MS_ABI impl__Exec_XOleCommandTarget_COleFrameHook__UEAAJPEBU_GUID__KKPEAUtagVARIANT__1_Z(
    void* pThis, const GUID* pguidCmdGroup, unsigned long nCmdID,
    unsigned long nCmdExecOpt, VARIANT* pvaIn, VARIANT* pvaOut) {
    (void)pThis;
    (void)pguidCmdGroup;
    (void)nCmdID;
    (void)nCmdExecOpt;
    (void)pvaIn;
    (void)pvaOut;
    return OLECMDERR_E_UNKNOWNGROUP;
}

// Symbol: ?GetBorder@XOleInPlaceFrame@COleFrameHook@@UEAAJPEAUtagRECT@@@Z
// Retail entry RVA 0x247a20 (mfc140 ANSI twin): hide the active item's bars,
// save the frame border RECT, negotiate commands 3/1/3 (the caller's output
// pointer is passed unchecked), then restore the bars only if they were
// initially shown. The needed frame fields and both sibling thunks are
// available; as elsewhere, this body is callable only if a +0x80 interface
// pointer is supplied, because OpenMFC does not publish that interface.
extern "C" long MS_ABI impl__GetBorder_XOleInPlaceFrame_COleFrameHook__UEAAJPEAUtagRECT___Z(
    void* pThis, RECT* lprectBorder) {
    COleFrameHook* hook = HookFromInPlaceFrameInterface(pThis);
    FrameHookModuleStateScope state(hook->m_pModuleState);
    CFrameWnd* frame = hook->m_pFrameWnd;
    COleClientItem* item = ActiveItem(hook);
    const int barsWereShown =
        impl__OnShowControlBars_COleClientItem__UEAAHPEAVCFrameWnd__H_Z(item, frame, FALSE);
    RECT savedBorder = frame->m_retailMiddle.m_rectBorder;
    CallRetailCFrameWndNegotiateBorderSpace(frame, 3, nullptr);
    CallRetailCFrameWndNegotiateBorderSpace(frame, 1, lprectBorder);
    CallRetailCFrameWndNegotiateBorderSpace(frame, 3, &savedBorder);
    if (barsWereShown)
        impl__OnShowControlBars_COleClientItem__UEAAHPEAVCFrameWnd__H_Z(item, frame, TRUE);
    return S_OK;
}

// Symbol: ?GetWindow@XOleInPlaceFrame@COleFrameHook@@UEAAJPEAPEAUHWND__@@@Z
// Retail entry RVA 0x247910 (mfc140 ANSI twin): E_POINTER for a null output,
// otherwise return the stored +0x58 frame HWND and S_OK, or E_FAIL if null.
extern "C" long MS_ABI impl__GetWindow_XOleInPlaceFrame_COleFrameHook__UEAAJPEAPEAUHWND_____Z(
    void* pThis, HWND* phwnd) {
    if (phwnd == nullptr)
        return E_POINTER;
    COleFrameHook* hook = HookFromInPlaceFrameInterface(pThis);
    *phwnd = hook->m_retailTail.m_hWndFrame;
    return *phwnd != nullptr ? S_OK : E_FAIL;
}

// Symbol: ?InsertMenus@XOleInPlaceFrame@COleFrameHook@@UEAAJPEAUHMENU__@@PEAUtagOleMenuGroupWidths@@@Z
// STUB -- the retail +0x80 interface slot is reserved but has no compatible
// vtable and is not published by OpenMFC's interface map.
extern "C" long MS_ABI impl__InsertMenus_XOleInPlaceFrame_COleFrameHook__UEAAJPEAUHMENU____PEAUtagOleMenuGroupWidths___Z(
    void* pThis, HMENU hmenuShared, OLEMENUGROUPWIDTHS* lpMenuWidths) {
    (void)pThis;
    (void)hmenuShared;
    (void)lpMenuWidths;
    return E_NOTIMPL;
}

// Symbol: ?QueryStatus@XOleCommandTarget@COleFrameHook@@UEAAJPEBU_GUID@@KQEAU_tagOLECMD@@PEAU_tagOLECMDTEXT@@@Z
extern "C" long MS_ABI impl__QueryStatus_XOleCommandTarget_COleFrameHook__UEAAJPEBU_GUID__KQEAU_tagOLECMD__PEAU_tagOLECMDTEXT___Z(
    void* pThis, const GUID* pguidCmdGroup, unsigned long cCmds, OLECMD* prgCmds,
    OLECMDTEXT* pCmdText) {
    (void)pThis;
    (void)pguidCmdGroup;
    (void)cCmds;
    (void)prgCmds;
    (void)pCmdText;
    return OLECMDERR_E_UNKNOWNGROUP;
}

// Symbol: ?RemoveMenus@XOleInPlaceFrame@COleFrameHook@@UEAAJPEAUHMENU__@@@Z
// STUB -- the retail +0x80 interface slot is reserved but has no compatible
// vtable and is not published by OpenMFC's interface map.
extern "C" long MS_ABI impl__RemoveMenus_XOleInPlaceFrame_COleFrameHook__UEAAJPEAUHMENU_____Z(
    void* pThis, HMENU hmenuShared) {
    (void)pThis;
    (void)hmenuShared;
    return E_NOTIMPL;
}

// Symbol: ?RequestBorderSpace@XOleInPlaceFrame@COleFrameHook@@UEAAJPEBUtagRECT@@@Z
// Retail entry RVA 0x247b30 (mfc140 ANSI twin): call the frame's
// NegotiateBorderSpace(2, pborderwidths), mapping TRUE to S_OK and FALSE to
// OLE_E_INVALIDRECT. The adapter uses the verified retail vtable byte offset
// +0x348 from OlecoreSupport.h; this body does not publish the missing +0x80
// interface vtable.
extern "C" long MS_ABI impl__RequestBorderSpace_XOleInPlaceFrame_COleFrameHook__UEAAJPEBUtagRECT___Z(
    void* pThis, const RECT* pborderwidths) {
    COleFrameHook* hook = HookFromInPlaceFrameInterface(pThis);
    FrameHookModuleStateScope state(hook->m_pModuleState);
    return CallRetailCFrameWndNegotiateBorderSpace(
               hook->m_pFrameWnd, 2, const_cast<RECT*>(pborderwidths))
        ? S_OK : OLE_E_INVALIDRECT;
}

// Symbol: ?SetActiveObject@XOleInPlaceFrame@COleFrameHook@@UEAAJPEAUIOleInPlaceActiveObject@@PEB_W@Z
// Retail entry RVA 0x247c50 (mfc140 ANSI twin; --u has no resolved export):
// release the old active object, store/AddRef the new one, empty the object
// name, and only for a non-null name AND active object assign the string and
// update the frame title. The active-object ownership and title callback are
// reproduced below. The CString Empty/assignment sequence is intentionally
// omitted because no thunk-safe CString helper is modeled in this file; a
// direct C++ operator call would violate the thunk-only link rule. This body
// uses the thunk-backed module-state scope and catches exceptions as E_UNEXPECTED. The
// +0x80 interface vtable is still not published.
extern "C" long MS_ABI impl__SetActiveObject_XOleInPlaceFrame_COleFrameHook__UEAAJPEAUIOleInPlaceActiveObject__PEB_W_Z(
    void* pThis, IOleInPlaceActiveObject* pActiveObject, const wchar_t* pszObjName) {
    try {
        COleFrameHook* hook = HookFromInPlaceFrameInterface(pThis);
        FrameHookModuleStateScope moduleState(hook->m_pModuleState);
        FrameHookState* state = GetFrameHookState(hook, true);
        if (state == nullptr)
            return E_UNEXPECTED;
        if (state->activeObject != nullptr) {
            state->activeObject->Release();
            state->activeObject = nullptr;
        }
        state->activeObject = pActiveObject;
        if (pActiveObject != nullptr)
            pActiveObject->AddRef();
        if (pszObjName != nullptr && pActiveObject != nullptr)
            impl__OnUpdateFrameTitle_COleClientItem__UEAAHXZ(ActiveItem(hook));
        return S_OK;
    } catch (...) {
        return E_UNEXPECTED;
    }
}

// Symbol: ?SetBorderSpace@XOleInPlaceFrame@COleFrameHook@@UEAAJPEBUtagRECT@@@Z
// Retail entry RVA 0x247ba0 (mfc140 ANSI twin): on successful command-3
// negotiation clear frame border flags 0xc and recalculate layout; regardless
// of that result, tell the active item whether to show its control bars and
// return S_OK. The +0x80 interface part remains unpublished.
extern "C" long MS_ABI impl__SetBorderSpace_XOleInPlaceFrame_COleFrameHook__UEAAJPEBUtagRECT___Z(
    void* pThis, const RECT* pborderwidths) {
    COleFrameHook* hook = HookFromInPlaceFrameInterface(pThis);
    FrameHookModuleStateScope state(hook->m_pModuleState);
    CFrameWnd* frame = hook->m_pFrameWnd;
    COleClientItem* item = ActiveItem(hook);
    if (CallRetailCFrameWndNegotiateBorderSpace(
            frame, 3, const_cast<RECT*>(pborderwidths))) {
        frame->m_retailTail.m_dwBorderFlags &= ~0xcu;
        impl__RecalcLayout_CFrameWnd__UEAAXH_Z(frame, FALSE);
    }
    impl__OnShowControlBars_COleClientItem__UEAAHPEAVCFrameWnd__H_Z(
        item, frame, pborderwidths == nullptr);
    return S_OK;
}

// Symbol: ?SetMenu@XOleInPlaceFrame@COleFrameHook@@UEAAJPEAUHMENU__@@PEAXPEAUHWND__@@@Z
// STUB -- the hook's frame state is modeled, but retail +0x80 has no compatible
// vtable and is not published by the interface map.
extern "C" long MS_ABI impl__SetMenu_XOleInPlaceFrame_COleFrameHook__UEAAJPEAUHMENU____PEAXPEAUHWND_____Z(
    void* pThis, HMENU hmenuShared, void* holemenu, HWND hwndActiveObject) {
    (void)pThis;
    (void)hmenuShared;
    (void)holemenu;
    (void)hwndActiveObject;
    return E_NOTIMPL;
}

// Symbol: ?SetStatusText@XOleInPlaceFrame@COleFrameHook@@UEAAJPEB_W@Z
// Retail entry RVA 0x247ea0 (mfc140 ANSI twin): synchronously send
// WM_SETMESSAGESTRING (0x362), wParam 0, and the temporary CString's text (or
// NULL) to the frame HWND, then return S_OK. The ANSI body's import slot
// 0x1802c5378 resolves via iat.py to USER32!SendMessageA; the Unicode source
// uses ::SendMessage so the build's UNICODE setting selects SendMessageW.
// Passing pszStatusText directly is equivalent here because SendMessage is
// synchronous and the retail CString copy is not otherwise observed.
extern "C" long MS_ABI impl__SetStatusText_XOleInPlaceFrame_COleFrameHook__UEAAJPEB_W_Z(
    void* pThis, const wchar_t* pszStatusText) {
    COleFrameHook* hook = HookFromInPlaceFrameInterface(pThis);
    ::SendMessage(hook->m_pFrameWnd->m_hWnd, 0x0362u, 0,
                  reinterpret_cast<LPARAM>(pszStatusText));
    return S_OK;
}

// Symbol: ?TranslateAcceleratorW@XOleInPlaceFrame@COleFrameHook@@UEAAJPEAUtagMSG@@G@Z
// Retail ANSI entry RVA 0x247fc0 (mfc140); the --u symbol lookup did not
// resolve. Save the frame accelerator, temporarily install the hook's +0x68
// table, call PreTranslateMessage on a 0x30-byte MSG copy, copy it back, restore
// the old table, and map TRUE/FALSE to S_OK/S_FALSE. Retail ignores wID. This
// implementation also restores the table on C++ exceptions and uses the
// thunk-backed AFX module-state scope.
extern "C" long MS_ABI impl__TranslateAcceleratorW_XOleInPlaceFrame_COleFrameHook__UEAAJPEAUtagMSG__G_Z(
    void* pThis, MSG* lpmsg, unsigned short wID) {
    (void)wID;
    try {
        COleFrameHook* hook = HookFromInPlaceFrameInterface(pThis);
        FrameHookModuleStateScope moduleState(hook->m_pModuleState);
        CFrameWnd* frame = hook->m_pFrameWnd;
        struct RestoreAccelerator {
            CFrameWnd* frame;
            HACCEL saved;
            ~RestoreAccelerator() { frame->m_hAccelTable = saved; }
        } restore{frame, frame->m_hAccelTable};
        frame->m_hAccelTable = hook->m_retailTail.m_hAccelTable;
        MSG message = *lpmsg;
        const int translated =
            impl__PreTranslateMessage_CFrameWnd__UEAAHPEAUtagMSG___Z(frame, &message);
        *lpmsg = message;
        return translated ? S_OK : S_FALSE;
    } catch (...) {
        return E_UNEXPECTED;
    }
}
