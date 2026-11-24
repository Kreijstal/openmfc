// CMFCToolBarFontComboBox — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp
//
// EVERY RVA IN THIS FILE IS AN mfc140u.dll RVA (14.51.36231).  The bodies
// marked "Retail (RVA 0x..., mfc140u)" were transcribed from the disassembly
// of that entry point.  Entry points: most come from the export map
// (mfc140u_rva_symbols.json); RebuildFonts (0x1691f0), SetContext (0x169370),
// Serialize (0x169800) and SetFont (0x169cb0) are not in that map and were
// resolved through the mfc140u export table by ordinal (12127, 13069, 12819,
// 13208).  The vftable is at 0x180318248 (mfc140u), stored by every ctor and
// by the dtor; slot 0 is GetRuntimeClass (0x168fe0), slot 2 Serialize
// (0x169800), slot 7 CopyFrom (0x16a6d0), slot 55 CreateCombo (0x169c20),
// slot 62 AddFont (0x1695d0).
//
// Instance layout (atlmfc/include/afxtoolbarfontcombobox.h: the class adds
// int m_nFontType, BYTE m_nCharSet, BYTE m_nPitchAndFamily, CObList*
// m_pLstFontsExternal after the CMFCToolBarComboBoxButton base), confirmed by
// the stores in ??0CMFCToolBarFontComboBox@@IEAA@PEAVCObList@@HEE@Z (0x169150):
//   +0x000  CMFCToolBarComboBoxButton base (sizeof 0x150)
//   +0x150  int      m_nFontType          (r8d)
//   +0x154  BYTE     m_nCharSet           (r9b)
//   +0x155  BYTE     m_nPitchAndFamily    (the stack argument)
//   +0x158  CObList* m_pLstFontsExternal  (rdx)
//   sizeof == 0x160 (CreateObject 0x168fb0: operator new(0x160))
// The base members this file touches (m_iWidth +0x88, m_iSelIndex +0x8c,
// m_nDropDownHeight +0x90, m_dwStyle +0xa4, m_strEdit +0xd8) are the retail
// offsets documented and used by featurepack/toolbar/CMFCToolBarComboBoxButton.cpp.
//
// Class statics (mfc140u .data): ?m_lstFonts@ (a CObList) at 0x3b2100 --
// retail reads its m_pNodeHead at 0x3b2108 and m_nCount at 0x3b2118 -- and
// ?m_nCount@ (int) at 0x3be37c, the latter defined in toolbar/StaticData.cpp.
//
// CMFCFontInfo (afxtoolbarfontcombobox.h; CObject base, then const CString
// m_strName, const CString m_strScript, const BYTE m_nCharSet, const BYTE
// m_nPitchAndFamily, const int m_nType): AddFont allocates it with
// operator new(0x20) and stores m_strName at +0x08, m_strScript at +0x10,
// m_nCharSet at +0x18, m_nPitchAndFamily at +0x19, m_nType at +0x1c; its
// vftable (0x1802e91a8, mfc140u) has five slots: CObject::GetRuntimeClass
// (0x37a0), the scalar deleting dtor (0x60e60) and three folded bare `ret`s
// (0x27d0) for Serialize / AssertValid / Dump.  The class has no export of its
// own (all of it is inline in the header), so this file builds the object
// with a hand-authored vtable of that shape (FontInfo / g_FontInfo_vtbl) --
// the layout featurepack/controls/CMFCFontComboBox.cpp reads back
// (its FontInfoLayout).
//
// Deviations applied throughout:
//  (1) The typed button uses the retail field layout and its own runtime
//      class descriptor. The exported thunks bridge operations outside the
//      OpenMFC virtual interface.
//  (2) Virtual calls on `this` are devirtualised: AddFont (vslot 0x1f0) from
//      the two enumeration callbacks goes to this file's AddFont thunk, and
//      AddItem (vslot 0x1a8, slot 53 -- 0x1628b0 in the dumped vftable; slot
//      54 is AddSortedItem) from SetContext to the base's exported AddItem
//      thunk.  A client override of either is not reached from these bodies.
//  (3) The combo's own item data.  Retail's SetFont walks the base's
//      m_lstItemData (+0x118) directly; OpenMFC keeps the combo items in the
//      cbarcore side table (see the header of CMFCToolBarComboBoxButton.cpp),
//      so SetFont reads them back through the exported GetCount / GetItemData.
//  (4) CObList.  OpenMFC's CObList keeps its elements in a side table keyed by
//      the list object (include/openmfc/afx.h, detail/FilecoreSupport.h
//      OPENMFC_DEFINE_LIST_METHODS), so retail's inlined reads of m_pNodeHead
//      / m_nCount become the exported FindIndex(0) plus a walk of the node
//      chain that POSITION points at (FontNode), the pattern of
//      toolbar/CMFCToolBarButtonsListButton.cpp.  A client that reads
//      m_lstFonts through its own inlined CObList::GetCount() sees 0.
//  (5) Exceptions.  The AtlThrow / AfxThrow* paths retail takes on a NULL
//      argument or a failed DC are not raised from this file; each site says
//      what it does instead.

#define OPENMFC_APPCORE_IMPL

#include "detail/ManualSmallStubImplementationsSupport.h"
#include "detail/CMFCFontCatalog.h"
#include "detail/CbarcoreSupport.h"
#include "detail/ToolBarComboSupport.h"
#include "detail/ToolBarControlArchive.h"
#include "openmfc/afxfontcombobox.h"

#include <memory>

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <cwchar>
#include <new>

// ---------------------------------------------------------------------------
// Cross-file thunks.  Each was grepped to its definition in the file named;
// the parameter lists follow those definitions (and the mangled names).
// ---------------------------------------------------------------------------
//   featurepack/toolbar/Thunks.cpp : base ctors / dtor
//   featurepack/toolbar/CMFCToolBarComboBoxButton.cpp : base behaviour
extern "C" void MS_ABI impl__CopyFrom_CMFCToolBarComboBoxButton__UEAAXAEBVCMFCToolBarButton___Z(
    CMFCToolBarComboBoxButton* pThis, const CMFCToolBarButton* pSrc);
extern "C" INT_PTR MS_ABI impl__AddItem_CMFCToolBarComboBoxButton__UEAA_JPEB_W_K_Z(
    CMFCToolBarComboBoxButton* pThis, const wchar_t* lpszItem, DWORD_PTR dwData);
extern "C" int MS_ABI impl__SelectItem_CMFCToolBarComboBoxButton__QEAAHHH_Z(CMFCToolBarComboBoxButton* pThis, int iIndex, int bNotify);
extern "C" int MS_ABI impl__SelectItem_CMFCToolBarComboBoxButton__QEAAH_K_Z(CMFCToolBarComboBoxButton* pThis, DWORD_PTR dwData);
extern "C" INT_PTR MS_ABI impl__GetCount_CMFCToolBarComboBoxButton__QEBA_JXZ(const CMFCToolBarComboBoxButton* pThis);
extern "C" DWORD_PTR MS_ABI impl__GetItemData_CMFCToolBarComboBoxButton__QEBA_KH_Z(const CMFCToolBarComboBoxButton* pThis, int iIndex);
extern "C" void MS_ABI impl__CopyFrom_CMFCToolBarButton__UEAAXAEBV1__Z(void*, const CMFCToolBarButton*);
//   core/collections/CObList.cpp
extern "C" void* MS_ABI impl___0CObList__QEAA__J_Z(CObList* pThis, long long nBlockSize);
extern "C" CObList::POSITION MS_ABI impl__FindIndex_CObList__QEBAPEAU__POSITION____J_Z(const CObList* pThis, long long nIndex);
extern "C" CObList::POSITION MS_ABI impl__AddTail_CObList__QEAAPEAU__POSITION__PEAVCObject___Z(CObList* pThis, CObject* value);
extern "C" CObList::POSITION MS_ABI impl__InsertBefore_CObList__QEAAPEAU__POSITION__PEAU2_PEAVCObject___Z(
    CObList* pThis, CObList::POSITION* pPos, CObject* value);
extern "C" CObject* MS_ABI impl__RemoveHead_CObList__QEAAPEAVCObject__XZ(CObList* pThis);
//   detail/CtrlcoreSupport.cpp : CComboBox::Create
extern "C" int MS_ABI impl__Create_CComboBox__UEAAHKAEBUtagRECT__PEAVCWnd__I_Z(
    CComboBox* pThis, DWORD dwStyle, const RECT* pRect, CWnd* pParentWnd, UINT nID);
//   detail/RegcoreSupport.cpp / core/app/CWinApp.cpp / featurepack/CMFC_misc_stubs.cpp
extern "C" CWinApp* MS_ABI impl__AfxGetApp__YAPEAVCWinApp__XZ();
extern "C" int MS_ABI impl__GetPrinterDeviceDefaults_CWinApp__QEAAHPEAUtagPDW___Z(CWinApp* pThis, PRINTDLGW* pd);
extern "C" void* MS_ABI impl__AfxCreateDC__YAPEAUHDC____PEAX0_Z(void* hDevNames, void* hDevMode);
//   core/runtime/CObject.cpp
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CObject__UEBAPEAUCRuntimeClass__XZ(const CObject* pThis);
//   detail/MemcoreSupport.cpp : ??2@YAPEAX_K@Z / ??3@YAXPEAX@Z
extern "C" void* MS_ABI impl___2_YAPEAX_K_Z(std::size_t size);
extern "C" void  MS_ABI impl___3_YAXPEAX_Z(void* ptr);
//   featurepack/toolbar/StaticData.cpp : ?m_nCount@CMFCToolBarFontComboBox@@1HA (0x3be37c, retail 0)
extern "C" std::int32_t impl__m_nCount_CMFCToolBarFontComboBox__1HA;

// Exports defined later in this file that earlier bodies call.
extern "C" int MS_ABI impl__AddFont_CMFCToolBarFontComboBox__MEAAHPEAUtagENUMLOGFONTW__HPEB_W_Z(
    void* pThis, ENUMLOGFONTW* pelf, int nType, const wchar_t* lpszScript);
extern "C" void MS_ABI impl__ClearFonts_CMFCToolBarFontComboBox__KAXXZ();
extern "C" int MS_ABI impl__GetFontsCount_CMFCToolBarFontComboBox__KAHPEB_W_Z(const wchar_t* lpszName);
extern "C" void MS_ABI impl__RebuildFonts_CMFCToolBarFontComboBox__IEAAXXZ(void* pThis);
extern "C" void MS_ABI impl__SetContext_CMFCToolBarFontComboBox__IEAAXXZ(void* pThis);
extern "C" int MS_ABI impl__EnumFamScreenCallBackEx_CMFCToolBarFontComboBox__KAHPEAUtagENUMLOGFONTEXW__PEAUtagNEWTEXTMETRICEXW__HPEAX_Z(
    ENUMLOGFONTEXW* pelf, NEWTEXTMETRICEXW* lpntm, int FontType, void* pThis);
extern "C" int MS_ABI impl__EnumFamPrinterCallBackEx_CMFCToolBarFontComboBox__KAHPEAUtagENUMLOGFONTEXW__PEAUtagNEWTEXTMETRICEXW__HPEAX_Z(
    ENUMLOGFONTEXW* pelf, NEWTEXTMETRICEXW* lpntm, int FontType, void* pThis);

// ?m_lstFonts@CMFCToolBarFontComboBox@@1VCObList@@A -- storage for the retail
// 56-byte CObList (defined further down, at its marker).
struct alignas(8) FontListStorage { unsigned char bytes[0x38]; };
extern "C" FontListStorage impl__m_lstFonts_CMFCToolBarFontComboBox__1VCObList__A;

namespace {

// ---------------------------------------------------------------------------
// Own members (+0x150, see the file header).
// ---------------------------------------------------------------------------
using FontComboOwn = decltype(CMFCToolBarFontComboBox::m_fontSettings);
static_assert(sizeof(CMFCToolBarFontComboBox) == 0x160);
static_assert(offsetof(CMFCToolBarFontComboBox, m_fontSettings) == 0x150);
constexpr std::size_t kOffOwn = 0x150;
static_assert(offsetof(FontComboOwn, m_nFontType) == 0x00, "m_nFontType @+0x150");
static_assert(offsetof(FontComboOwn, m_nCharSet) == 0x04, "m_nCharSet @+0x154");
static_assert(offsetof(FontComboOwn, m_nPitchAndFamily) == 0x05, "m_nPitchAndFamily @+0x155");
static_assert(offsetof(FontComboOwn, m_pLstFontsExternal) == 0x08, "m_pLstFontsExternal @+0x158");
static_assert(kOffOwn + sizeof(FontComboOwn) == 0x160, "retail sizeof(CMFCToolBarFontComboBox) is 0x160 (CreateObject 0x168fb0)");
static_assert(sizeof(CMFCToolBarComboBoxButton) == kOffOwn, "the base ends at +0x150 (retail sizeof 336)");
static_assert(sizeof(CObList) == sizeof(FontListStorage), "retail CObList is 56 bytes");
static_assert(offsetof(CMFCToolBarButton, m_nID) == 0x24, "m_nID @0x24");
static_assert(offsetof(CMFCToolBarButton, m_rect) == 0x68, "m_rect @0x68 (Serialize writes m_rect.right at +0x70)");
static_assert(sizeof(CString) == sizeof(void*), "CString is one pointer (ATL layout)");

// Base members (retail offsets, see CMFCToolBarComboBoxButton.cpp's header).
constexpr std::size_t kOffWidth          = 0x88;
constexpr std::size_t kOffSelIndex       = 0x8c;
constexpr std::size_t kOffDropDownHeight = 0x90;
constexpr std::size_t kOffStyle          = 0xa4;
constexpr std::size_t kOffWndCombo       = 0xd0;
constexpr std::size_t kOffStrEdit        = 0xd8;

template <typename T> T& At(void* p, std::size_t off) { return *reinterpret_cast<T*>(static_cast<unsigned char*>(p) + off); }
template <typename T> const T& At(const void* p, std::size_t off) { return *reinterpret_cast<const T*>(static_cast<const unsigned char*>(p) + off); }

FontComboOwn* Own(void* p) { return &static_cast<CMFCToolBarFontComboBox*>(p)->m_fontSettings; }
const FontComboOwn* Own(const void* p) { return &static_cast<const CMFCToolBarFontComboBox*>(p)->m_fontSettings; }
CMFCToolBarComboBoxButton* Combo(void* p) { return static_cast<CMFCToolBarComboBoxButton*>(p); }

class ToolBarFontWindow final : public CMFCFontComboBox {
public:
    ToolBarFontWindow() { m_bToolBarMode = TRUE; }
};
thread_local const void* g_toolBarFontWindowVptr = nullptr;

// ---------------------------------------------------------------------------
// CMFCFontInfo, built with its retail layout and a retail-shaped vtable.
// ---------------------------------------------------------------------------
struct FontInfo {
    const void* vfptr;          // +0x00
    CString     m_strName;      // +0x08
    CString     m_strScript;    // +0x10
    BYTE        m_nCharSet;     // +0x18
    BYTE        m_nPitchAndFamily; // +0x19
    int         m_nType;        // +0x1c
};
static_assert(offsetof(FontInfo, m_strName) == 0x08, "CMFCFontInfo::m_strName @+0x08");
static_assert(offsetof(FontInfo, m_strScript) == 0x10, "CMFCFontInfo::m_strScript @+0x10");
static_assert(offsetof(FontInfo, m_nCharSet) == 0x18, "CMFCFontInfo::m_nCharSet @+0x18");
static_assert(offsetof(FontInfo, m_nPitchAndFamily) == 0x19, "CMFCFontInfo::m_nPitchAndFamily @+0x19");
static_assert(offsetof(FontInfo, m_nType) == 0x1c, "CMFCFontInfo::m_nType @+0x1c");
static_assert(sizeof(FontInfo) == 0x20, "AddFont allocates 0x20 bytes");

// Retail scalar deleting dtor (RVA 0x60e60, mfc140u), transcribed:
//     m_strScript.~CString();  m_strName.~CString();
//     if (flags & 1) {
//         if (flags & 4) call 0x27d0 (this, 0x20);   // folded bare `ret`: frees nothing
//         else free(this);                          // IAT 0x1802c74e8 = ucrt free
//     }
//     return this;
// Here the object came from ??2@YAPEAX_K@Z (malloc), so it goes back through
// ??3@YAXPEAX@Z (free).
void* MS_ABI FontInfo_ScalarDeletingDtor(void* pThis, unsigned int flags) {
    FontInfo* p = static_cast<FontInfo*>(pThis);
    p->m_strScript.~CString();
    p->m_strName.~CString();
    if ((flags & 1u) != 0 && (flags & 4u) == 0) impl___3_YAXPEAX_Z(pThis);
    return pThis;
}
// Serialize / AssertValid / Dump: the folded bare `ret` (0x27d0) in retail.
void MS_ABI FontInfo_NoOp(void* /*pThis*/, void* /*arg*/) {}

#define VT(fn) reinterpret_cast<const void*>(&fn)
const void* const g_FontInfo_vtbl[5] = {
    VT(impl__GetRuntimeClass_CObject__UEBAPEAUCRuntimeClass__XZ),   // slot 0: CObject::GetRuntimeClass (0x37a0)
    VT(FontInfo_ScalarDeletingDtor),                                 // slot 1 (0x60e60)
    VT(FontInfo_NoOp),                                               // slot 2 Serialize
    VT(FontInfo_NoOp),                                               // slot 3 AssertValid
    VT(FontInfo_NoOp),                                               // slot 4 Dump
};
#undef VT

// new CMFCFontInfo(lpszName, lpszScript, nCharSet, nPitchAndFamily, nType)
FontInfo* NewFontInfo(const wchar_t* lpszName, const wchar_t* lpszScript, BYTE nCharSet, BYTE nPitchAndFamily, int nType) {
    void* pMem = impl___2_YAPEAX_K_Z(sizeof(FontInfo));
    if (pMem == nullptr) return nullptr;
    FontInfo* p = static_cast<FontInfo*>(pMem);
    p->vfptr = g_FontInfo_vtbl;
    new (&p->m_strName) CString(lpszName);
    new (&p->m_strScript) CString(lpszScript);
    p->m_nCharSet = nCharSet;
    p->m_nPitchAndFamily = nPitchAndFamily;
    p->m_nType = nType;
    return p;
}

// CMFCFontInfo::GetFullName() (header inline; retail's outlined copy is at
// 0xf57f8): m_strName, plus " (" + m_strScript + ")" when the script is not empty.
CString FullName(const FontInfo* p) {
    CString str = p->m_strName;
    if (!p->m_strScript.IsEmpty()) {
        str += L" (";
        str += p->m_strScript;
        str += L")";
    }
    return str;
}

// CStringT::MakeLower (0x2da0: _wcslwr_s over the buffer).
void MakeLower(CString& s) {
    const int n = s.GetLength();
    wchar_t* pBuf = s.GetBuffer(n);
    _wcslwr(pBuf);
    s.ReleaseBuffer(n);
}

// ---------------------------------------------------------------------------
// CObList access (deviation (4)).
// ---------------------------------------------------------------------------
struct FontNode {            // CList<CObject*,CObject*>::CNode, what POSITION points at
    FontNode* pNext;
    FontNode* pPrev;
    CObject*  data;
};
static_assert(sizeof(CObList::POSITION) == sizeof(void*), "POSITION is one pointer");

FontNode* NodeFromPos(CObList::POSITION pos) {
    FontNode* p = nullptr;
    std::memcpy(&p, &pos, sizeof p);
    return p;
}
CObList::POSITION PosFromNode(FontNode* p) {
    CObList::POSITION pos(nullptr);
    std::memcpy(&pos, &p, sizeof p);
    return pos;
}
FontNode* ListHead(const CObList* pList) {
    return NodeFromPos(impl__FindIndex_CObList__QEBAPEAU__POSITION____J_Z(pList, 0));
}

// The static m_lstFonts.  Retail's object is a live CObList, constant-
// initialised in .data (mfc140u 0x3b2100 holds the CObList vftable
// 0x18032a6f8, zeros, and m_nBlockSize = 10 at +0x30); its destructor
// (0x2c5820) is registered through atexit (0x2b7770) by the initializer stub
// at 0x2070, stores that vftable back and tail-jumps to the list teardown at
// 0x8350.  Here the storage starts zeroed and is constructed through the
// exported CObList ctor on first use by this file, with the same block size
// 10, and is never destroyed.  A client that reads the static before any
// OpenMFC body has touched it sees zero bytes, not a constructed list.
CObList* StaticFontList() {
    static const bool s_constructed = [] {
        impl___0CObList__QEAA__J_Z(reinterpret_cast<CObList*>(&impl__m_lstFonts_CMFCToolBarFontComboBox__1VCObList__A), 10);
        return true;
    }();
    (void)s_constructed;
    return reinterpret_cast<CObList*>(&impl__m_lstFonts_CMFCToolBarFontComboBox__1VCObList__A);
}

} // namespace

// ===========================================================================
// Construction / destruction
// ===========================================================================

IMPLEMENT_SERIAL(CMFCToolBarFontComboBox, CMFCToolBarComboBoxButton, 1)

CMFCToolBarFontComboBox::CMFCToolBarFontComboBox()
    : m_fontSettings{DEVICE_FONTTYPE | RASTER_FONTTYPE | TRUETYPE_FONTTYPE, DEFAULT_CHARSET, DEFAULT_PITCH, nullptr}
{
    ++impl__m_nCount_CMFCToolBarFontComboBox__1HA;
}

CMFCToolBarFontComboBox::CMFCToolBarFontComboBox(CObList* fonts, int types, BYTE charSet, BYTE pitch)
    : m_fontSettings{types, charSet, pitch, fonts}
{
    impl__RebuildFonts_CMFCToolBarFontComboBox__IEAAXXZ(this);
    if (!fonts) ++impl__m_nCount_CMFCToolBarFontComboBox__1HA;
}

CMFCToolBarFontComboBox::CMFCToolBarFontComboBox(UINT id, int image, int types, BYTE charSet,
                                               DWORD style, int width, BYTE pitch)
    : CMFCToolBarComboBoxButton(id, image, style, width), m_fontSettings{types, charSet, pitch, nullptr}
{
    impl__SetContext_CMFCToolBarFontComboBox__IEAAXXZ(this);
    ++impl__m_nCount_CMFCToolBarFontComboBox__1HA;
}

CMFCToolBarFontComboBox::~CMFCToolBarFontComboBox() {
    openmfc::detail::ReleaseToolBarComboWindows(*this);
    if (!m_fontSettings.m_pLstFontsExternal && --impl__m_nCount_CMFCToolBarFontComboBox__1HA == 0)
        impl__ClearFonts_CMFCToolBarFontComboBox__KAXXZ();
}

// Symbol: ??0CMFCToolBarFontComboBox@@IEAA@PEAVCObList@@HEE@Z
extern "C" void* MS_ABI impl___0CMFCToolBarFontComboBox__IEAA_PEAVCObList__HEE_Z(
    void* storage, CObList* fonts, int types, BYTE charSet, BYTE pitch) {
    return storage ? new (storage) CMFCToolBarFontComboBox(fonts, types, charSet, pitch) : nullptr;
}

// Symbol: ??0CMFCToolBarFontComboBox@@IEAA@XZ
extern "C" void* MS_ABI impl___0CMFCToolBarFontComboBox__IEAA_XZ(void* storage) {
    return storage ? new (storage) CMFCToolBarFontComboBox : nullptr;
}

// Symbol: ??0CMFCToolBarFontComboBox@@QEAA@IHHEKHE@Z
extern "C" void* MS_ABI impl___0CMFCToolBarFontComboBox__QEAA_IHHEKHE_Z(
    void* storage, unsigned int id, int image, int types, BYTE charSet,
    unsigned long style, int width, BYTE pitch) {
    return storage ? new (storage) CMFCToolBarFontComboBox(id, image, types, charSet, style, width, pitch) : nullptr;
}

// Symbol: ??1CMFCToolBarFontComboBox@@UEAA@XZ
extern "C" void MS_ABI impl___1CMFCToolBarFontComboBox__UEAA_XZ(void* button) {
    if (button) static_cast<CMFCToolBarFontComboBox*>(button)->CMFCToolBarFontComboBox::~CMFCToolBarFontComboBox();
}

std::vector<CMFCFontInfo> openmfc::detail::CollectFontCatalog(int fontTypes, BYTE charSet, BYTE pitchAndFamily) {
    struct Catalog {
        CObList fonts;
        alignas(void*) unsigned char toolbar[kOffOwn + sizeof(FontComboOwn)]{};
        bool constructed = false;
        ~Catalog() {
            if (constructed) impl___1CMFCToolBarFontComboBox__UEAA_XZ(toolbar);
            for (auto* node = ListHead(&fonts); node; node = ListHead(&fonts)) {
                auto* font = impl__RemoveHead_CObList__QEAAPEAVCObject__XZ(&fonts);
                if (font) FontInfo_ScalarDeletingDtor(font, 1);
            }
        }
    } catalog;
    impl___0CMFCToolBarFontComboBox__IEAA_PEAVCObList__HEE_Z(catalog.toolbar, &catalog.fonts, fontTypes,
                                                          charSet, pitchAndFamily);
    catalog.constructed = true;

    std::vector<CMFCFontInfo> result;
    for (auto* node = ListHead(&catalog.fonts); node; node = node->pNext) {
        const auto* font = reinterpret_cast<const FontInfo*>(node->data);
        result.emplace_back(font->m_strName, font->m_strScript, font->m_nCharSet,
                            font->m_nPitchAndFamily, font->m_nType);
    }
    return result;
}

// Retail (RVA 0x168fb0, mfc140u), fully transcribed:
//     void* p = operator new(0x160);                              // 0x27f0
//     return p ? new(p) CMFCToolBarFontComboBox() : NULL;         // tail jump to 0x169020
// Symbol: ?CreateObject@CMFCToolBarFontComboBox@@SAPEAVCObject@@XZ
extern "C" void* MS_ABI impl__CreateObject_CMFCToolBarFontComboBox__SAPEAVCObject__XZ() {
    return CMFCToolBarFontComboBox::CreateObject();
}

// ===========================================================================
// The font list
// ===========================================================================

// Retail (RVA 0x1695d0, mfc140u), fully transcribed:
//     CObList& lst = m_pLstFontsExternal != NULL ? *m_pLstFontsExternal : m_lstFonts;
//     if (pelf->elfLogFont.lfCharSet == MAC_CHARSET) return FALSE;          // +0x17 == 0x4d
//     BYTE lfPitch = pelf->elfLogFont.lfPitchAndFamily;                     // +0x1b
//     if ((m_nPitchAndFamily & 0x0F) && (lfPitch & 0x0F) != (m_nPitchAndFamily & 0x0F)) return FALSE;
//     if ((m_nPitchAndFamily & 0xF0) && (lfPitch & 0xF0) != (m_nPitchAndFamily & 0xF0)) return FALSE;
//     for (POSITION pos = lst.GetHeadPosition(); pos != NULL;) {           // duplicate face name
//         CMFCFontInfo* pDesc = (CMFCFontInfo*)lst.GetNext(pos);
//         if (wcscmp(pDesc->m_strName, pelf->elfLogFont.lfFaceName) == 0) return FALSE;
//     }
//     if (::GetSystemMetrics(SM_DBCSENABLED) && lfFaceName[0] == L'@') return FALSE;   // vertical DBCS fonts
//     CMFCFontInfo* pDesc = new CMFCFontInfo(lfFaceName, lpszScript, lfCharSet, lfPitch, nType);   // 0x20 bytes
//     for (POSITION pos = lst.GetHeadPosition(); pos != NULL;) {           // keep the list sorted by full name
//         POSITION posSave = pos;
//         CMFCFontInfo* pDescList = (CMFCFontInfo*)lst.GetNext(pos);
//         if (wcscmp(pDescList->GetFullName(), pDesc->GetFullName()) >= 0) {
//             lst.InsertBefore(posSave, pDesc);                            // 0x232010
//             return TRUE;
//         }
//     }
//     lst.AddTail(pDesc);                                                  // 0x231e70
//     return TRUE;
// The list is walked through FindIndex(0) and the node chain (deviation (4));
// InsertBefore takes the POSITION by address, as the OpenMFC thunk's
// definition (core/collections/CObList.cpp) dereferences it -- the call shape
// of customize/CMFCToolBarsCustomizeDialog.cpp and toolbar/CMFCToolBar.cpp.
// Symbol: ?AddFont@CMFCToolBarFontComboBox@@MEAAHPEAUtagENUMLOGFONTW@@HPEB_W@Z
extern "C" int MS_ABI impl__AddFont_CMFCToolBarFontComboBox__MEAAHPEAUtagENUMLOGFONTW__HPEB_W_Z(
    void* pThis, ENUMLOGFONTW* pelf, int nType, const wchar_t* lpszScript) {
    if (pThis == nullptr || pelf == nullptr) return FALSE;
    const FontComboOwn* o = Own(pThis);
    CObList* pList = o->m_pLstFontsExternal != nullptr ? o->m_pLstFontsExternal : StaticFontList();
    const LOGFONTW& lf = pelf->elfLogFont;

    if (lf.lfCharSet == MAC_CHARSET) return FALSE;
    const BYTE lfPitch = lf.lfPitchAndFamily;
    const BYTE nPitch = o->m_pLstFontsExternal ? o->m_nPitchAndFamily : DEFAULT_PITCH;
    if ((nPitch & 0x0F) != 0 && (lfPitch & 0x0F) != (nPitch & 0x0F)) return FALSE;
    if ((nPitch & 0xF0) != 0 && (lfPitch & 0xF0) != (nPitch & 0xF0)) return FALSE;

    for (FontNode* n = ListHead(pList); n != nullptr; n = n->pNext) {
        const FontInfo* pDesc = reinterpret_cast<const FontInfo*>(n->data);
        if ((o->m_pLstFontsExternal || pDesc->m_nCharSet == lf.lfCharSet) &&
            std::wcscmp(pDesc->m_strName.GetString(), lf.lfFaceName) == 0) return FALSE;
    }

    if (::GetSystemMetrics(SM_DBCSENABLED) != 0 && lf.lfFaceName[0] == L'@') return FALSE;

    FontInfo* pDesc = NewFontInfo(lf.lfFaceName, lpszScript, lf.lfCharSet, lfPitch, nType);
    if (pDesc == nullptr) return FALSE;   // retail's operator new throws instead of returning NULL
    const CString strNew = FullName(pDesc);

    for (FontNode* n = ListHead(pList); n != nullptr; n = n->pNext) {
        const FontInfo* pDescList = reinterpret_cast<const FontInfo*>(n->data);
        const CString strList = FullName(pDescList);
        if (std::wcscmp(static_cast<const wchar_t*>(strList), static_cast<const wchar_t*>(strNew)) >= 0) {
            CObList::POSITION posSave = PosFromNode(n);
            impl__InsertBefore_CObList__QEAAPEAU__POSITION__PEAU2_PEAVCObject___Z(pList, &posSave, reinterpret_cast<CObject*>(pDesc));
            return TRUE;
        }
    }
    impl__AddTail_CObList__QEAAPEAU__POSITION__PEAVCObject___Z(pList, reinterpret_cast<CObject*>(pDesc));
    return TRUE;
}

// Retail (RVA 0x169330, mfc140u), fully transcribed:
//     while (m_lstFonts.GetCount() != 0) {                        // m_nCount at 0x3b2118
//         CObject* p = m_lstFonts.RemoveHead();                   // 0x231f40
//         if (p != NULL) delete p;                                // vslot 0x08 (1), flags 1
//     }
// The emptiness test is FindIndex(0) != NULL (deviation (4)); the delete goes
// through the element's own slot 1 exactly as retail's, so an element a client
// built with its own CMFCFontInfo vtable is destroyed by that vtable.
// Symbol: ?ClearFonts@CMFCToolBarFontComboBox@@KAXXZ
extern "C" void MS_ABI impl__ClearFonts_CMFCToolBarFontComboBox__KAXXZ() {
    CObList* pList = StaticFontList();
    while (ListHead(pList) != nullptr) {
        CObject* p = impl__RemoveHead_CObList__QEAAPEAVCObject__XZ(pList);
        if (p != nullptr) {
            using ScalarDeletingDtorFn = void* (MS_ABI*)(void*, unsigned int);
            reinterpret_cast<ScalarDeletingDtorFn>((*reinterpret_cast<void* const* const*>(p))[1])(p, 1u);
        }
    }
}

// Retail (RVA 0x169bb0, mfc140u), fully transcribed:
//     int nCount = 0;
//     for (POSITION pos = m_lstFonts.GetHeadPosition(); pos != NULL;) {    // m_pNodeHead at 0x3b2108
//         CMFCFontInfo* pDesc = (CMFCFontInfo*)m_lstFonts.GetNext(pos);
//         if (wcscmp(pDesc->m_strName, lpszName) == 0) nCount++;         // lpszName NULL: AtlThrow(E_FAIL)
//     }
//     return nCount;
// Always the static m_lstFonts, never m_pLstFontsExternal (a static method).
// Deviation (5): a NULL lpszName matches nothing (count 0) where retail
// raises, on a non-empty list, through the ATL throw helper (0x333c ->
// AfxThrowOleException(E_FAIL)).
// Symbol: ?GetFontsCount@CMFCToolBarFontComboBox@@KAHPEB_W@Z
extern "C" int MS_ABI impl__GetFontsCount_CMFCToolBarFontComboBox__KAHPEB_W_Z(const wchar_t* lpszName) {
    if (lpszName == nullptr) return 0;
    int nCount = 0;
    for (FontNode* n = ListHead(StaticFontList()); n != nullptr; n = n->pNext) {
        const FontInfo* pDesc = reinterpret_cast<const FontInfo*>(n->data);
        if (std::wcscmp(static_cast<const wchar_t*>(pDesc->m_strName), lpszName) == 0) nCount++;
    }
    return nCount;
}

// Retail (RVA 0x169490, mfc140u), fully transcribed:
//     ((CMFCToolBarFontComboBox*)pThis)->AddFont((ENUMLOGFONT*)pelf, FontType, CString(pelf->elfScript));   // vslot 0x1f0 (62)
//     return TRUE;
// AddFont is reached through this file's thunk (deviation (2)).
// Symbol: ?EnumFamScreenCallBackEx@CMFCToolBarFontComboBox@@KAHPEAUtagENUMLOGFONTEXW@@PEAUtagNEWTEXTMETRICEXW@@HPEAX@Z
extern "C" int MS_ABI impl__EnumFamScreenCallBackEx_CMFCToolBarFontComboBox__KAHPEAUtagENUMLOGFONTEXW__PEAUtagNEWTEXTMETRICEXW__HPEAX_Z(
    ENUMLOGFONTEXW* pelf, NEWTEXTMETRICEXW* lpntm, int FontType, void* pThis) {
    (void)lpntm;
    if (pelf == nullptr || pThis == nullptr) return TRUE;
    const CString strScript(pelf->elfScript);
    impl__AddFont_CMFCToolBarFontComboBox__MEAAHPEAUtagENUMLOGFONTW__HPEB_W_Z(
        pThis, reinterpret_cast<ENUMLOGFONTW*>(pelf), FontType, static_cast<const wchar_t*>(strScript));
    return TRUE;
}

// Retail (RVA 0x169510, mfc140u), fully transcribed:
//     CString strName = pelf->elfLogFont.lfFaceName;              // built, never read
//     ((CMFCToolBarFontComboBox*)pThis)->AddFont((ENUMLOGFONT*)pelf, FontType, CString(pelf->elfScript));   // vslot 0x1f0 (62)
//     return TRUE;
// The unused face-name copy is dropped; AddFont is reached through this file's
// thunk (deviation (2)).
// Symbol: ?EnumFamPrinterCallBackEx@CMFCToolBarFontComboBox@@KAHPEAUtagENUMLOGFONTEXW@@PEAUtagNEWTEXTMETRICEXW@@HPEAX@Z
extern "C" int MS_ABI impl__EnumFamPrinterCallBackEx_CMFCToolBarFontComboBox__KAHPEAUtagENUMLOGFONTEXW__PEAUtagNEWTEXTMETRICEXW__HPEAX_Z(
    ENUMLOGFONTEXW* pelf, NEWTEXTMETRICEXW* lpntm, int FontType, void* pThis) {
    (void)lpntm;
    if (pelf == nullptr || pThis == nullptr) return TRUE;
    const CString strScript(pelf->elfScript);
    impl__AddFont_CMFCToolBarFontComboBox__MEAAHPEAUtagENUMLOGFONTW__HPEB_W_Z(
        pThis, reinterpret_cast<ENUMLOGFONTW*>(pelf), FontType, static_cast<const wchar_t*>(strScript));
    return TRUE;
}

// Retail (RVA 0x1691f0, mfc140u), fully transcribed:
//     CWindowDC dc(NULL);                                         // 0x2a3c20: ::GetWindowDC(NULL), AfxThrowResourceException on failure
//     LOGFONT lf;  memset(&lf, 0, sizeof(LOGFONT));
//     lf.lfCharSet = m_nCharSet;                                  // +0x154
//     ::EnumFontFamiliesEx(dc.m_hDC, &lf, (FONTENUMPROC)EnumFamScreenCallBackEx, (LPARAM)this, 0);
//     CPrintDialog dlg(FALSE);                                    // 0x218e40, flags 0x14000c (the header default)
//     if (AfxGetApp()->GetPrinterDeviceDefaults(&dlg.m_pd)) {     // AfxGetModuleState (0x133930)->m_pCurrentWinApp (+8); 0x1d0500
//         HDC hDCPrint = dlg.CreatePrinterDC();                   // 0x2190d0: m_pd.hDC = AfxCreateDC(m_pd.hDevNames, m_pd.hDevMode)
//         ENSURE(hDCPrint != NULL);                               // AfxThrowInvalidArgException (0x227720)
//         ::EnumFontFamiliesEx(hDCPrint, &lf, (FONTENUMPROC)EnumFamPrinterCallBackEx, (LPARAM)this, 0);
//         ::DeleteObject(hDCPrint);
//     }
//     // ~CPrintDialog (0x207eb0 after the vftable store), ~CWindowDC (0x2a3ce0: ReleaseDC)
// The list is not cleared first (callers do that through ClearFonts / m_nCount).
// DEVIATIONS: (a) the screen DC is taken and released with ::GetWindowDC /
// ::ReleaseDC directly (what CWindowDC's ctor / dtor do), and a NULL DC skips
// the screen pass instead of throwing; (b) the CPrintDialog serves only as
// the PRINTDLG that GetPrinterDeviceDefaults fills, so a local PRINTDLGW is
// used and the DC is made with the exported AfxCreateDC, which is what
// CPrintDialog::CreatePrinterDC calls; (c) a NULL printer DC (or a NULL
// AfxGetApp()) skips the printer pass instead of throwing -- OpenMFC's
// GetPrinterDeviceDefaults (core/app/CWinApp.cpp) reports TRUE even when no
// printer was ever selected, so retail's ENSURE would fire on every build;
// (d) the printer DC is released with ::DeleteDC, the documented call for a
// DC from CreateDC -- retail passes it to ::DeleteObject (IAT slot
// 0x1802c6278 resolved to GDI32!DeleteObject).  Whether GDI's DeleteObject
// frees a DC handle was not checked here, so no claim is made that retail
// leaks it.
// Symbol: ?RebuildFonts@CMFCToolBarFontComboBox@@IEAAXXZ
extern "C" void MS_ABI impl__RebuildFonts_CMFCToolBarFontComboBox__IEAAXXZ(void* pThis) {
    if (pThis == nullptr) return;
    LOGFONTW lf;
    std::memset(&lf, 0, sizeof lf);
    const auto* settings = Own(pThis);
    lf.lfCharSet = settings->m_pLstFontsExternal ? settings->m_nCharSet : DEFAULT_CHARSET;

    if (HDC hdcScreen = ::GetWindowDC(nullptr)) {
        ::EnumFontFamiliesExW(hdcScreen, &lf,
            reinterpret_cast<FONTENUMPROCW>(&impl__EnumFamScreenCallBackEx_CMFCToolBarFontComboBox__KAHPEAUtagENUMLOGFONTEXW__PEAUtagNEWTEXTMETRICEXW__HPEAX_Z),
            reinterpret_cast<LPARAM>(pThis), 0);
        ::ReleaseDC(nullptr, hdcScreen);
    }

    CWinApp* pApp = impl__AfxGetApp__YAPEAVCWinApp__XZ();
    if (pApp == nullptr) return;
    PRINTDLGW pd;
    std::memset(&pd, 0, sizeof pd);
    pd.lStructSize = sizeof pd;
    if (impl__GetPrinterDeviceDefaults_CWinApp__QEAAHPEAUtagPDW___Z(pApp, &pd)) {
        HDC hDCPrint = static_cast<HDC>(impl__AfxCreateDC__YAPEAUHDC____PEAX0_Z(pd.hDevNames, pd.hDevMode));
        if (hDCPrint == nullptr) return;
        ::EnumFontFamiliesExW(hDCPrint, &lf,
            reinterpret_cast<FONTENUMPROCW>(&impl__EnumFamPrinterCallBackEx_CMFCToolBarFontComboBox__KAHPEAUtagENUMLOGFONTEXW__PEAUtagNEWTEXTMETRICEXW__HPEAX_Z),
            reinterpret_cast<LPARAM>(pThis), 0);
        ::DeleteDC(hDCPrint);
    }
}

namespace {
// Rebuild each button's rows from the shared descriptors. The item-data
// pointers are valid for the lifetime of the font combo buttons.
void BuildFontItems(CMFCToolBarFontComboBox& button) {
    CObList* fonts = button.m_fontSettings.m_pLstFontsExternal;
    if (!fonts) {
        fonts = StaticFontList();
        if (!ListHead(fonts)) impl__RebuildFonts_CMFCToolBarFontComboBox__IEAAXXZ(&button);
    }
    decltype(openmfc::detail::cbarcore::ComboButtonState::items) items;
    const auto& settings = button.m_fontSettings;
    for (auto* node = ListHead(fonts); node; node = node->pNext) {
        const auto* font = reinterpret_cast<const FontInfo*>(node->data);
        if (!(settings.m_nFontType & font->m_nType) ||
            (settings.m_nCharSet != DEFAULT_CHARSET && settings.m_nCharSet != font->m_nCharSet)) continue;
        const BYTE pitch = settings.m_nPitchAndFamily;
        if (((pitch & 0x0f) && (pitch & 0x0f) != (font->m_nPitchAndFamily & 0x0f)) ||
            ((pitch & 0xf0) && (pitch & 0xf0) != (font->m_nPitchAndFamily & 0xf0))) continue;
        const CString text = !settings.m_pLstFontsExternal && settings.m_nCharSet == DEFAULT_CHARSET &&
            impl__GetFontsCount_CMFCToolBarFontComboBox__KAHPEB_W_Z(font->m_strName) > 1 ? FullName(font) : font->m_strName;
        items.emplace_back(text, reinterpret_cast<DWORD_PTR>(font));
    }
    openmfc::detail::cbarcore::RegisterComboButton(&button);
    auto& state = openmfc::detail::cbarcore::EnsureComboState(&button);
    state.items = std::move(items);
    auto& text = At<CString>(&button, kOffStrEdit);
    int& selected = At<int>(&button, kOffSelIndex);
    if (!text.IsEmpty()) {
        const int match = button.FindItem(text);
        selected = match;
    } else if (selected >= 0 && selected < button.GetCount()) {
        text = button.GetItem(selected);
    } else {
        selected = -1;
    }
    openmfc::detail::RefreshToolBarComboItems(button);
}

}  // namespace

// Symbol: ?SetContext@CMFCToolBarFontComboBox@@IEAAXXZ
extern "C" void MS_ABI impl__SetContext_CMFCToolBarFontComboBox__IEAAXXZ(void* button) {
    if (button) BuildFontItems(*static_cast<CMFCToolBarFontComboBox*>(button));
}

// ===========================================================================
// Overrides / operations
// ===========================================================================

// Retail (RVA 0x16a6d0, mfc140u), fully transcribed:
//     CMFCToolBarComboBoxButton::CopyFrom(s);                     // 0x1618b0
//     const CMFCToolBarFontComboBox& src = (const CMFCToolBarFontComboBox&)s;   // no IsKindOf check
//     m_nCharSet = src.m_nCharSet;  m_nFontType = src.m_nFontType;  m_nPitchAndFamily = src.m_nPitchAndFamily;
// (m_pLstFontsExternal is not copied.)
// Symbol: ?CopyFrom@CMFCToolBarFontComboBox@@MEAAXAEBVCMFCToolBarButton@@@Z
extern "C" void MS_ABI impl__CopyFrom_CMFCToolBarFontComboBox__MEAAXAEBVCMFCToolBarButton___Z(void* pThis, const CMFCToolBarButton* pSrc) {
    if (!pThis || !pSrc || pThis == pSrc || !pSrc->IsKindOf(RUNTIME_CLASS(CMFCToolBarFontComboBox))) return;
    impl__CopyFrom_CMFCToolBarComboBoxButton__UEAAXAEBVCMFCToolBarButton___Z(Combo(pThis), pSrc);
    const FontComboOwn* s = Own(static_cast<const void*>(pSrc));
    FontComboOwn* o = Own(pThis);
    o->m_nCharSet = s->m_nCharSet;
    o->m_nFontType = s->m_nFontType;
    o->m_nPitchAndFamily = s->m_nPitchAndFamily;
    BuildFontItems(*static_cast<CMFCToolBarFontComboBox*>(pThis));
}

CComboBox* openmfc::detail::CreateToolBarFontWindow(CWnd* parent, const RECT& bounds, DWORD style, UINT id) {
    if (!parent || !parent->GetSafeHwnd()) return nullptr;
    auto window = std::make_unique<ToolBarFontWindow>();
    g_toolBarFontWindowVptr = *reinterpret_cast<const void* const*>(window.get());
    if (!impl__Create_CComboBox__UEAAHKAEBUtagRECT__PEAVCWnd__I_Z(window.get(), style, &bounds, parent, id))
        return nullptr;
    if (style & CBS_OWNERDRAWFIXED) {
        const int height = (std::max)(16, ::GetSystemMetrics(SM_CYMENUSIZE));
        ::SendMessageW(window->GetSafeHwnd(), CB_SETITEMHEIGHT, 0, height);
    }
    return window.release();
}

bool openmfc::detail::DestroyToolBarFontWindow(CWnd* window) {
    if (!window || !g_toolBarFontWindowVptr ||
        *reinterpret_cast<const void* const*>(window) != g_toolBarFontWindowVptr) return false;
    window->DestroyWindow();
    delete static_cast<ToolBarFontWindow*>(window);
    return true;
}

bool openmfc::detail::DrawToolBarFontComboItem(CMFCToolBarComboBoxButton& button, DRAWITEMSTRUCT& item) {
    auto* combo = At<CComboBox*>(&button, kOffWndCombo);
    if (!combo || combo->GetSafeHwnd() != item.hwndItem ||
        *reinterpret_cast<const void* const*>(combo) != g_toolBarFontWindowVptr) return false;
    if (item.itemData == ULONG_PTR(-1)) item.itemData = 0;
    static_cast<ToolBarFontWindow*>(combo)->DrawItem(&item);
    return true;
}

// Symbol: ?CreateCombo@CMFCToolBarFontComboBox@@MEAAPEAVCComboBox@@PEAVCWnd@@AEBVCRect@@@Z
extern "C" CComboBox* MS_ABI impl__CreateCombo_CMFCToolBarFontComboBox__MEAAPEAVCComboBox__PEAVCWnd__AEBVCRect___Z(
    void* button, CWnd* parent, const RECT* bounds) {
    if (!button || !bounds) return nullptr;
    return openmfc::detail::CreateToolBarFontWindow(parent, *bounds,
        At<DWORD>(button, kOffStyle) | CBS_OWNERDRAWFIXED | CBS_HASSTRINGS, Combo(button)->m_nID);
}

void CMFCToolBarFontComboBox::Serialize(CArchive& archive) {
    if (archive.IsStoring()) {
        CMFCToolBarButton::Serialize(archive);
        archive << At<int>(this, kOffWidth) << At<DWORD>(this, kOffStyle) << At<int>(this, kOffSelIndex)
                << At<CString>(this, kOffStrEdit) << At<int>(this, kOffDropDownHeight)
                << m_fontSettings.m_nFontType << m_fontSettings.m_nCharSet;
        return;
    }
    CMFCToolBarButton base;
    impl__CopyFrom_CMFCToolBarButton__UEAAXAEBV1__Z(&base, this);
    base.Serialize(archive);
    int width = 0, selected = -1, height = 0, types = 0;
    DWORD style = 0;
    BYTE charSet = DEFAULT_CHARSET;
    CString text;
    archive >> width >> style >> selected;
    openmfc::detail::ReadToolBarControlText(archive, text);
    archive >> height >> types >> charSet;

    if (m_nID != base.m_nID) openmfc::detail::cbarcore::UnregisterComboButton(this);
    impl__CopyFrom_CMFCToolBarButton__UEAAXAEBV1__Z(this, &base);
    At<int>(this, kOffWidth) = width;
    m_rect.right = m_rect.left + width;
    At<DWORD>(this, kOffStyle) = style;
    At<int>(this, kOffSelIndex) = selected;
    At<CString>(this, kOffStrEdit) = text;
    At<int>(this, kOffDropDownHeight) = height;
    m_fontSettings.m_nFontType = types;
    m_fontSettings.m_nCharSet = charSet;
    BuildFontItems(*this);
}

// Symbol: ?Serialize@CMFCToolBarFontComboBox@@MEAAXAEAVCArchive@@@Z
extern "C" void MS_ABI impl__Serialize_CMFCToolBarFontComboBox__MEAAXAEAVCArchive___Z(void* button, CArchive* archive) {
    if (button && archive) static_cast<CMFCToolBarFontComboBox*>(button)->CMFCToolBarFontComboBox::Serialize(*archive);
}

// Retail (RVA 0x169cb0, mfc140u), fully transcribed:
//     if (lpszName == NULL) AfxThrowInvalidArgException();        // 0x227720
//     CString strName = lpszName;  strName.MakeLower();           // 0x2da0
//     for (POSITION pos = m_lstItemData.GetHeadPosition(); pos != NULL;) {   // +0x120 (m_lstItemData @+0x118)
//         CMFCFontInfo* pDesc = (CMFCFontInfo*)m_lstItemData.GetNext(pos);
//         CString strFullName = pDesc->GetFullName();  strFullName.MakeLower();
//         if (bExact) {
//             if (strFullName == strName                          // wcscmp
//                 || (_wcsicmp(pDesc->m_strName, lpszName) == 0
//                     && (pDesc->m_nCharSet == nCharSet || nCharSet == DEFAULT_CHARSET))) {
//                 SelectItem((DWORD_PTR)pDesc);                   // 0x163d10
//                 return TRUE;
//             }
//         } else if (strFullName.Find(strName) == 0              // wcsstr, offset 0
//                    && (nCharSet == DEFAULT_CHARSET || pDesc->m_nCharSet == nCharSet)) {
//             SelectItem((DWORD_PTR)pDesc);
//             return TRUE;
//         }
//     }
//     return FALSE;
// The item data is read back by index through the base's exported GetCount /
// GetItemData (deviation (3)).  Deviation (5): a NULL lpszName returns FALSE.
// SelectItem's own return value is ignored, as in retail.
// Symbol: ?SetFont@CMFCToolBarFontComboBox@@QEAAHPEB_WEH@Z
extern "C" int MS_ABI impl__SetFont_CMFCToolBarFontComboBox__QEAAHPEB_WEH_Z(
    void* pThis, const wchar_t* lpszName, BYTE nCharSet, int bExact) {
    if (pThis == nullptr || lpszName == nullptr) return FALSE;
    CString strName(lpszName);
    MakeLower(strName);

    const INT_PTR nCount = impl__GetCount_CMFCToolBarComboBoxButton__QEBA_JXZ(Combo(pThis));
    for (INT_PTR i = 0; i < nCount; ++i) {
        const DWORD_PTR dwData = impl__GetItemData_CMFCToolBarComboBoxButton__QEBA_KH_Z(Combo(pThis), static_cast<int>(i));
        const FontInfo* pDesc = reinterpret_cast<const FontInfo*>(dwData);
        if (pDesc == nullptr) continue;   // added guard: retail dereferences the item data unconditionally
        CString strFullName = FullName(pDesc);
        MakeLower(strFullName);

        bool bMatch;
        if (bExact) {
            bMatch = std::wcscmp(static_cast<const wchar_t*>(strFullName), static_cast<const wchar_t*>(strName)) == 0
                || (::_wcsicmp(static_cast<const wchar_t*>(pDesc->m_strName), lpszName) == 0
                    && (pDesc->m_nCharSet == nCharSet || nCharSet == DEFAULT_CHARSET));
        } else {
            bMatch = strFullName.Find(static_cast<const wchar_t*>(strName)) == 0
                && (nCharSet == DEFAULT_CHARSET || pDesc->m_nCharSet == nCharSet);
        }
        if (bMatch) {
            impl__SelectItem_CMFCToolBarComboBoxButton__QEAAH_K_Z(Combo(pThis), dwData);
            return TRUE;
        }
    }
    return FALSE;
}

BOOL CMFCToolBarFontComboBox::SetFont(const wchar_t* name, BYTE charSet, BOOL exact) {
    return impl__SetFont_CMFCToolBarFontComboBox__QEAAHPEB_WEH_Z(this, name, charSet, exact);
}

const CMFCFontInfo* CMFCToolBarFontComboBox::GetFontDesc(int index) const {
    return reinterpret_cast<const CMFCFontInfo*>(GetItemData(index));
}

// Retail .data object at 0x3b2100 (mfc140u): a 56-byte CObList.  Zero storage
// here, constructed through the exported CObList ctor on first use by
// StaticFontList() (see there).  The previous definition was an 8-byte
// `void*`, too small for the object clients address.
// Symbol: ?m_lstFonts@CMFCToolBarFontComboBox@@1VCObList@@A
extern "C" FontListStorage impl__m_lstFonts_CMFCToolBarFontComboBox__1VCObList__A = {};
