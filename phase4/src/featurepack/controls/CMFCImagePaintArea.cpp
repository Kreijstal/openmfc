// CMFCImagePaintArea — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp
//
// ---------------------------------------------------------------------------
// Object model (read before touching any body)
// ---------------------------------------------------------------------------
// OpenMFC has no C++ declaration of CMFCImagePaintArea.  The retail declaration
// is afximagepaintarea.h:32 (MSVC 14.51):
//     class CMFCImagePaintArea : public CButton
// and the instance layout below was read out of the retail constructor
// ??0CMFCImagePaintArea@@QEAA@PEAVCMFCImageEditorDialog@@@Z (RVA 0x70830,
// mfc140.dll) instruction by instruction, plus the member accesses of the other
// bodies in this file:
//
//   +0x000  CButton subobject (== CWnd, 0xe8 bytes; the ctor calls CWnd::CWnd
//           at 0x288b10 on `this` and then stores the class vftable)
//   +0x0e8  CRect     m_rectParentPreviewArea   (ctor zeroes it, SetRectEmpty)
//   +0x0f8  COLORREF  m_rgbColor                (ctor 0x1800708de; SetColor)
//   +0x100  CDC       m_memDC                   (ctor stores the CDC vftable at
//                                                +0x100, zeroes m_hDC +0x108,
//                                                m_hAttribDC +0x110 and the dword
//                                                m_bPrinting +0x118)
//   +0x120  CSize     m_sizeImage               (cx +0x120, cy +0x124)
//   +0x128  CBitmap*  m_pBitmap
//   +0x130  HCURSOR   m_hcurPen      +0x138 m_hcurFill   +0x140 m_hcurLine
//   +0x148  HCURSOR   m_hcurRect     +0x150 m_hcurEllipse +0x158 m_hcurColor
//                                               (PreSubclassWindow stores all six;
//                                                the ctor does NOT initialise them)
//   +0x160  CSize     m_sizeCell                (cx +0x160, cy +0x164)
//   +0x168  CPen      m_penDraw                 (vfptr +0x168, m_hObject +0x170)
//   +0x178  CPen      m_penStretch              (vfptr +0x178, m_hObject +0x180)
//   +0x188  CRect     m_rectDraw                (left/top/right/bottom
//                                                +0x188/+0x18c/+0x190/+0x194)
//   +0x198  CMFCImageEditorDialog* m_pParentDlg
//   +0x1a0  IMAGE_EDIT_MODE m_Mode
//   sizeof  0x1a8  (m_Mode, a 4-byte enum, is the last member and the object is
//                   8-byte aligned)
// Member NAMES and ORDER are the retail header's; the OFFSETS are the ones the
// disassembly uses.  All of it is pinned by the static_asserts on PaintArea below.
// featurepack/controls/CMFCImageEditorDialog.cpp embeds this object as a
// 0x1a8-byte block at dialog +0x428 and addresses m_hWnd (+0x40) and m_Mode
// (+0x1a0) through it -- the same offsets.
//
// afxGlobalData members read here (offsets pinned by the AfxGlobalData shadow in
// core/runtime/AFX_GLOBAL_DATA.cpp): clrBtnShadow +0x2c, clrBtnHilite +0x30,
// clrBtnText +0x34, clrBtnDkShadow +0x3c, brBtnFace.m_hObject +0xb0.
//
// DEVIATIONS from retail.  The object-model ones are listed here; the per-body
// ones are documented at their functions and summarised at the end of this
// list:
//   * OpenMFC cannot install the retail vftables.  The constructor runs
//     CWnd::CWnd through its thunk (which leaves OpenMFC's CWnd vptr, exactly as
//     toolbar/CMFCToolBarButtonsListButton.cpp does), and the embedded m_memDC /
//     m_penDraw / m_penStretch are raw views whose vfptr slot is left NULL (the
//     same memory image core/runtime/AFX_GLOBAL_DATA.cpp uses for the GDI members
//     of afxGlobalData).  No body in this file dispatches through those vfptrs:
//     every CDC / CGdiObject method retail calls is issued as a direct call to
//     its exported thunk.  The one virtual call retail makes on m_memDC --
//     SelectStockObject, slot 11 (+0x58), confirmed against the CDC vftable --
//     reaches CDC::SelectStockObject either way, because m_memDC is a plain CDC.
//   * Retail's ENSURE macros call AfxThrowInvalidArgException, which does not
//     return; this file calls the same exported thunk at the same place.
//   * The class' AFX_MSGMAP (detail/Mfc04MsgmapSupport.cpp) is currently the
//     shared EMPTY entry table, so none of the WM_ handlers below is reachable
//     from OpenMFC's message pump yet.  Reported as a header request.  (Its base
//     map, CWnd's, is correct: the retail map at 0x1802e9910 (mfc140.dll) links
//     to ?GetThisMessageMap@CWnd@@ -- CButton has no map of its own.)
//   * Per-body choices (not forced by the object model): a NULL GetParent() is
//     skipped where retail faults (InvalidateParentPreview); the destructor calls
//     CDC::DeleteDC before the ??1CDC thunk; SetBitmap zero-fills its BITMAP
//     before GetObject; PreSubclassWindow omits retail's six discarded
//     AfxGetModuleState() calls; the constructor and destructor return early on
//     a NULL `this`, which retail does not test.
//
// Every RVA quoted in this file is an mfc140.dll (ANSI twin) address, named as
// such.  Function bodies are byte-identical between mfc140.dll and mfc140u.dll, so
// the control flow, member offsets and constants transcribed from them are correct
// for mfc140u; the RVAs themselves are NOT mfc140u RVAs.  Import slots were
// resolved with iat.py, never guessed.

#include "detail/ManualSmallStubImplementationsSupport.h"

#include <cstddef>
#include <cstring>

//---------------------------------------------------------------------------
// Thunks for everything this file calls.  A C++ method written directly here
// would compile and fail to link (briefing §1).
//---------------------------------------------------------------------------
extern "C" void*    MS_ABI impl___0CWnd__QEAA_XZ(void* pThis);                            // core/window/CtorDtorPlacement.cpp
extern "C" void     MS_ABI impl___1CButton__UEAA_XZ(void* pThis);                          // core/controls/CtorDtorPlacement.cpp
extern "C" CWnd*    MS_ABI impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(HWND hWnd);       // core/window/CWnd.cpp
extern "C" __int64  MS_ABI impl__Default_CWnd__IEAA_JXZ(CWnd* pThis);                      // core/window/Thunks.cpp
extern "C" void     MS_ABI impl__AfxThrowInvalidArgException__YAXXZ();                     // detail/MfcExceptionsSupport.cpp
extern "C" HINSTANCE MS_ABI impl__AfxFindResourceHandle__YAPEAUHINSTANCE____PEB_W0_Z(
    const wchar_t* lpszResource, const wchar_t* lpszType);                                 // core/runtime/Globals.cpp

extern "C" CDC*     MS_ABI impl__FromHandle_CDC__SAPEAV1_PEAUHDC_____Z(HDC hDC);           // core/gdi/CDC.cpp
extern "C" int      MS_ABI impl__Attach_CDC__QEAAHPEAUHDC_____Z(CDC* pThis, HDC hDC);      // core/gdi/CDC.cpp
extern "C" int      MS_ABI impl__DeleteDC_CDC__QEAAHXZ(CDC* pThis);                        // core/gdi/CDC.cpp
extern "C" void     MS_ABI impl___1CDC__UEAA_XZ(CDC* pThis);                               // core/gdi/CDC.cpp
extern "C" CPen*    MS_ABI impl__SelectObject_CDC__QEAAPEAVCPen__PEAV2__Z(CDC* pThis, CPen* pPen);   // core/gdi/CDC.cpp
extern "C" CBrush*  MS_ABI impl__SelectObject_CDC__QEAAPEAVCBrush__PEAV2__Z(CDC* pThis, CBrush* pBrush); // core/gdi/CDC.cpp
extern "C" int      MS_ABI impl__SelectObject_CDC__QEAAHPEAVCRgn___Z(CDC* pThis, CRgn* pRgn);        // core/gdi/CDC.cpp
extern "C" int      MS_ABI impl__SelectClipRgn_CDC__QEAAHPEAVCRgn___Z(CDC* pThis, CRgn* pRgn);       // core/gdi/CDC.cpp
extern "C" CGdiObject* MS_ABI impl__SelectStockObject_CDC__UEAAPEAVCGdiObject__H_Z(CDC* pThis, int nIndex); // core/gdi/CDC.cpp
extern "C" int      MS_ABI impl__SetROP2_CDC__QEAAHH_Z(CDC* pThis, int nDrawMode);          // core/gdi/CDC.cpp
extern "C" CPoint   MS_ABI impl__MoveTo_CDC__QEAA_AVCPoint__HH_Z(CDC* pThis, int x, int y); // core/gdi/CDC.cpp
extern "C" int      MS_ABI impl__LineTo_CDC__QEAAHHH_Z(CDC* pThis, int x, int y);           // core/gdi/CDC.cpp
extern "C" void     MS_ABI impl__FillSolidRect_CDC__QEAAXPEBUtagRECT__K_Z(CDC* pThis, const RECT* lpRect, unsigned long clr); // core/gdi/CDC.cpp
extern "C" void     MS_ABI impl__Draw3dRect_CDC__QEAAXPEBUtagRECT__KK_Z(
    CDC* pThis, const RECT* lpRect, unsigned long clrTopLeft, unsigned long clrBottomRight);  // core/gdi/CDC.cpp
extern "C" CClientDC* MS_ABI impl___0CClientDC__QEAA_PEAVCWnd___Z(CClientDC* pThis, CWnd* pWnd); // core/gdi/CClientDC.cpp
extern "C" void     MS_ABI impl___1CClientDC__UEAA_XZ(CClientDC* pThis);                   // core/gdi/CClientDC.cpp

extern "C" CGdiObject* MS_ABI impl__FromHandle_CGdiObject__SAPEAV1_PEAX_Z(HGDIOBJ hObject); // core/gdi/CGdiObject.cpp
extern "C" int      MS_ABI impl__Attach_CGdiObject__QEAAHPEAX_Z(CGdiObject* pThis, HGDIOBJ hObject); // core/gdi/CGdiObject.cpp
extern "C" int      MS_ABI impl__DeleteObject_CGdiObject__QEAAHXZ(CGdiObject* pThis);      // core/gdi/CGdiObject.cpp
extern "C" CPen*    MS_ABI impl___0CPen__QEAA_HHK_Z(CPen* pThis, int nPenStyle, int nWidth, unsigned long crColor); // core/gdi/CPen.cpp
extern "C" CBrush*  MS_ABI impl___0CBrush__QEAA_K_Z(CBrush* pThis, unsigned long crColor);  // core/gdi/CBrush.cpp

extern "C" unsigned long MS_ABI impl__MapToSysColor_CMFCToolBarImages__SAKKH_Z(unsigned long clr, int bUseRGBQUAD);   // toolbar/CMFCToolBarImages.cpp
extern "C" unsigned long MS_ABI impl__MapFromSysColor_CMFCToolBarImages__SAKKH_Z(unsigned long clr, int bUseRGBQUAD); // toolbar/CMFCToolBarImages.cpp

// ?OnPickColor@CMFCImageEditorDialog@@IEAAHK@Z -- `int OnPickColor(COLORREF)`, a
// non-static member, so (this, color): the list the mangled name describes.  Its
// definition in featurepack/controls/CMFCImageEditorDialog.cpp takes the same
// (void* pThis, unsigned long color) list in the working tree this was reviewed
// against (the committed HEAD version was a `(unsigned long p0)` placeholder with
// no `this`).
extern "C" int      MS_ABI impl__OnPickColor_CMFCImageEditorDialog__IEAAHK_Z(void* pThis, unsigned long color);

extern "C" void     MS_ABI impl__Initialize_AFX_GLOBAL_DATA__QEAAXXZ(void* pThis);         // core/runtime/AFX_GLOBAL_DATA.cpp
extern "C" unsigned char impl__afxGlobalData__3UAFX_GLOBAL_DATA__A[720];                   // featurepack/CMFC_misc_stubs.cpp

//---------------------------------------------------------------------------
// Forward declarations of this file's own exports (they call each other exactly
// as the retail bodies do).  CPoint / POINT passed BY VALUE are 8-byte aggregates
// in one register under the x64 MS ABI, modelled as `long long`.
//---------------------------------------------------------------------------
extern "C" void MS_ABI impl__DrawPixel_CMFCImagePaintArea__IEAAXUtagPOINT___Z(void* pThis, long long ptPacked);
extern "C" void MS_ABI impl__FloodFill_CMFCImagePaintArea__IEAAXAEBVCPoint___Z(void* pThis, const POINT* pPoint);
extern "C" void MS_ABI impl__BitmapToClient_CMFCImagePaintArea__IEAAXAEAVCRect___Z(void* pThis, RECT* pRect);

namespace {

//---------------------------------------------------------------------------
// Instance layout (see the header comment).
//---------------------------------------------------------------------------
// CGdiObject in an x64 MSVC build: vftable pointer, then the HGDIOBJ.
struct GdiObjView {
    void*   vfptr;                           // +0x00  (left NULL here, see header)
    HGDIOBJ m_hObject;                       // +0x08
};
// CDC in an x64 MSVC build.
struct DCView {
    void* vfptr;                             // +0x00  (left NULL here, see header)
    HDC   m_hDC;                             // +0x08
    HDC   m_hAttribDC;                       // +0x10
    int   m_bPrinting;                       // +0x18
};

struct PaintArea {
    unsigned char _base[0xe8];               // 0x000 CButton (== CWnd) subobject
    RECT       m_rectParentPreviewArea;      // 0x0e8
    COLORREF   m_rgbColor;                   // 0x0f8
    DCView     m_memDC;                      // 0x100
    SIZE       m_sizeImage;                  // 0x120
    CBitmap*   m_pBitmap;                    // 0x128
    HCURSOR    m_hcurPen;                    // 0x130
    HCURSOR    m_hcurFill;                   // 0x138
    HCURSOR    m_hcurLine;                   // 0x140
    HCURSOR    m_hcurRect;                   // 0x148
    HCURSOR    m_hcurEllipse;                // 0x150
    HCURSOR    m_hcurColor;                  // 0x158
    SIZE       m_sizeCell;                   // 0x160
    GdiObjView m_penDraw;                    // 0x168
    GdiObjView m_penStretch;                 // 0x178
    RECT       m_rectDraw;                   // 0x188
    void*      m_pParentDlg;                 // 0x198 CMFCImageEditorDialog*
    int        m_Mode;                       // 0x1a0 IMAGE_EDIT_MODE
};

static_assert(sizeof(CWnd) == 0xe8, "OpenMFC's CWnd is the retail 0xe8-byte CWnd");
static_assert(sizeof(CButton) == sizeof(CWnd), "OpenMFC's CButton adds no members to CWnd");
static_assert(offsetof(CWnd, m_hWnd) == 0x40, "CWnd::m_hWnd (every body here reads +0x40)");
// The views stand in for real CDC / CPen objects when handed to the thunks, which
// only touch m_hDC / m_hAttribDC / m_hObject -- so those must line up.
static_assert(sizeof(CDC) == sizeof(DCView), "OpenMFC's CDC is the retail 0x20-byte CDC");
static_assert(offsetof(CDC, m_hDC) == offsetof(DCView, m_hDC), "CDC::m_hDC at +0x08");
static_assert(offsetof(CDC, m_hAttribDC) == offsetof(DCView, m_hAttribDC), "CDC::m_hAttribDC at +0x10");
static_assert(sizeof(CPen) == sizeof(GdiObjView), "OpenMFC's CPen is the retail 0x10-byte CPen");
static_assert(offsetof(CGdiObject, m_hObject) == offsetof(GdiObjView, m_hObject), "CGdiObject::m_hObject at +0x08");

static_assert(offsetof(PaintArea, m_rectParentPreviewArea) == 0x0e8, "ctor, insns at 0x180070857..0x180070864 zero +0xe8 (mfc140)");
static_assert(offsetof(PaintArea, m_rgbColor)    == 0x0f8, "ctor, insn at 0x1800708de; SetColor, insn at 0x1800720c1 (mfc140)");
static_assert(offsetof(PaintArea, m_memDC)       == 0x100, "ctor, insn at 0x180070876 stores the CDC vftable (mfc140)");
static_assert(offsetof(PaintArea, m_memDC) + offsetof(DCView, m_hDC) == 0x108, "DrawPixel, insn at 0x180071a99 (mfc140)");
static_assert(offsetof(PaintArea, m_memDC) + offsetof(DCView, m_bPrinting) == 0x118, "ctor, insn at 0x180070881 (mfc140)");
static_assert(offsetof(PaintArea, m_sizeImage)   == 0x120, "SetBitmap, insn at 0x180071bb6 (mfc140)");
static_assert(offsetof(PaintArea, m_pBitmap)     == 0x128, "SetBitmap, insn at 0x180071b69 (mfc140)");
static_assert(offsetof(PaintArea, m_hcurPen)     == 0x130, "PreSubclassWindow, insn at 0x180071dd5 (mfc140)");
static_assert(offsetof(PaintArea, m_hcurColor)   == 0x158, "PreSubclassWindow, insn at 0x180071e93 (mfc140)");
static_assert(offsetof(PaintArea, m_sizeCell)    == 0x160, "SetBitmap, insn at 0x180071bf9 (mfc140)");
static_assert(offsetof(PaintArea, m_penDraw)     == 0x168, "SetColor, insn at 0x1800720ba (mfc140)");
static_assert(offsetof(PaintArea, m_penStretch)  == 0x178, "SetBitmap, insn at 0x180071be7 (mfc140)");
static_assert(offsetof(PaintArea, m_rectDraw)    == 0x188, "OnLButtonDown, insn at 0x1800713ee (mfc140)");
static_assert(offsetof(PaintArea, m_pParentDlg)  == 0x198, "ctor, insn at 0x1800708c9 (mfc140)");
static_assert(offsetof(PaintArea, m_Mode)        == 0x1a0, "ctor, insn at 0x1800708fe (mfc140)");
static_assert(sizeof(PaintArea) == 0x1a8, "retail CMFCImagePaintArea is 0x1a8 bytes");

// CMFCImagePaintArea::IMAGE_EDIT_MODE (afximagepaintarea.h:40).
enum : int {
    kModePen = 0, kModeFill = 1, kModeLine = 2, kModeRect = 3, kModeEllipse = 4, kModeColor = 5
};

// IDC_AFXBARRES_* cursors (afxribbonres.h:89-94); the values are the immediates
// PreSubclassWindow passes.
constexpr int kIdcRect    = 16140;  // 0x3f0c
constexpr int kIdcEllipse = 16141;  // 0x3f0d
constexpr int kIdcFill    = 16142;  // 0x3f0e
constexpr int kIdcLine    = 16143;  // 0x3f0f
constexpr int kIdcPen     = 16144;  // 0x3f10
constexpr int kIdcColor   = 16145;  // 0x3f11

// afxGlobalData offsets (see header comment).
constexpr int kGlobalDataInitGate      = 0x000;
constexpr int kGlobalDataClrBtnShadow  = 0x02c;
constexpr int kGlobalDataClrBtnHilite  = 0x030;
constexpr int kGlobalDataClrBtnText    = 0x034;
constexpr int kGlobalDataClrBtnDkShadow = 0x03c;
constexpr int kGlobalDataBrBtnFaceHandle = 0x0b0;   // brBtnFace (+0xa8) .m_hObject (+0x08)

// The retail one-time gate, inlined at every afxGlobalData read in this class
// (0x6a5c0 is ?Initialize@AFX_GLOBAL_DATA@@, mfc140.dll):
//     if (afxGlobalData.<+0> == 0) { afxGlobalData.Initialize(); afxGlobalData.<+0> = 1; }
void EnsureGlobalData()
{
    int gate = 0;
    std::memcpy(&gate, impl__afxGlobalData__3UAFX_GLOBAL_DATA__A + kGlobalDataInitGate, sizeof gate);
    if (gate == 0) {
        impl__Initialize_AFX_GLOBAL_DATA__QEAAXXZ(impl__afxGlobalData__3UAFX_GLOBAL_DATA__A);
        const int one = 1;
        std::memcpy(impl__afxGlobalData__3UAFX_GLOBAL_DATA__A + kGlobalDataInitGate, &one, sizeof one);
    }
}

COLORREF GlobalDataColor(int off)
{
    EnsureGlobalData();
    COLORREF clr = 0;
    std::memcpy(&clr, impl__afxGlobalData__3UAFX_GLOBAL_DATA__A + off, sizeof clr);
    return clr;
}

HBRUSH GlobalDataBrush(int off)
{
    EnsureGlobalData();
    HBRUSH h = nullptr;
    std::memcpy(&h, impl__afxGlobalData__3UAFX_GLOBAL_DATA__A + off, sizeof h);
    return h;
}

PaintArea* Self(void* pThis)      { return static_cast<PaintArea*>(pThis); }
CWnd*      AsWnd(void* pThis)     { return reinterpret_cast<CWnd*>(pThis); }
HWND       Hwnd(void* pThis)      { return AsWnd(pThis)->m_hWnd; }
CDC*       MemDC(PaintArea* p)    { return reinterpret_cast<CDC*>(&p->m_memDC); }
CGdiObject* Gdi(GdiObjView* v)    { return reinterpret_cast<CGdiObject*>(v); }
CPen*      Pen(GdiObjView* v)     { return reinterpret_cast<CPen*>(v); }

POINT Unpack(long long packed)
{
    POINT pt;
    std::memcpy(&pt, &packed, sizeof pt);
    return pt;
}
long long Pack(POINT pt)
{
    long long packed = 0;
    std::memcpy(&packed, &pt, sizeof pt);
    return packed;
}

// CGdiObject::GetSafeHandle(), inline in retail: `this == NULL ? NULL : m_hObject`.
HGDIOBJ SafeHandle(const CGdiObject* p) { return p != nullptr ? p->m_hObject : nullptr; }
HGDIOBJ SafeHandle(const CBitmap* p)    { return p != nullptr ? p->m_hObject : nullptr; }

// The inline CDC::SelectObject(CBitmap*) -> CDC::SelectGdiObject(m_hDC, h), which
// retail expands to ::SelectObject + CGdiObject::FromHandle (0x2a1de0, mfc140.dll)
// at every bitmap select/restore in this class.
CGdiObject* SelectGdi(PaintArea* p, HGDIOBJ h)
{
    return impl__FromHandle_CGdiObject__SAPEAV1_PEAX_Z(::SelectObject(p->m_memDC.m_hDC, h));
}

// ScreenToBitmap's arithmetic; retail inlines it verbatim in DrawPixel,
// OnLButtonDown, OnLButtonUp and OnMouseMove.  Unguarded divisions, as in retail.
POINT ToBitmap(const PaintArea* p, POINT pt)
{
    int x = (pt.x - 1) / p->m_sizeCell.cx;
    int y = (pt.y - 1) / p->m_sizeCell.cy;
    if (x > p->m_sizeImage.cx - 1) x = p->m_sizeImage.cx - 1;
    if (x < 0) x = 0;
    if (y > p->m_sizeImage.cy - 1) y = p->m_sizeImage.cy - 1;
    if (y < 0) y = 0;
    POINT r = { x, y };
    return r;
}

// The drawing-area rectangle: client rect with right/bottom replaced by
// left + m_sizeImage * m_sizeCell (inlined in DrawItem, OnLButtonDown,
// OnMouseMove and OnSetCursor).
RECT ImageRect(const PaintArea* p, const RECT& rectClient)
{
    RECT r = rectClient;
    r.right  = rectClient.left + p->m_sizeImage.cx * p->m_sizeCell.cx;
    r.bottom = rectClient.top  + p->m_sizeImage.cy * p->m_sizeCell.cy;
    return r;
}

// GetParent()->InvalidateRect(m_rectParentPreviewArea), as DrawPixel, FloodFill
// and OnLButtonUp inline it: ::GetParent + CWnd::FromHandle, then
// ::InvalidateRect(pParent->m_hWnd, &m_rectParentPreviewArea, TRUE).
// DEVIATION: retail dereferences the FromHandle result unconditionally (a NULL
// parent faults); this skips the call instead.
void InvalidateParentPreview(void* pThis)
{
    CWnd* pParent = impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(::GetParent(Hwnd(pThis)));
    if (pParent == nullptr) return;
    ::InvalidateRect(pParent->m_hWnd, &Self(pThis)->m_rectParentPreviewArea, TRUE);
}

} // namespace

// Symbol: ??0CMFCImagePaintArea@@QEAA@PEAVCMFCImageEditorDialog@@@Z
// Retail (RVA 0x70830, mfc140.dll), fully transcribed:
//     CWnd::CWnd();                                // 0x288b10 (inline CButton())
//     <store class vftable>
//     m_rectParentPreviewArea = {0,0,0,0};
//     m_memDC: <CDC vftable>, m_hDC = m_hAttribDC = NULL, m_bPrinting = 0;
//     m_sizeImage = m_sizeCell = CSize(0,0);
//     m_penDraw / m_penStretch: <CPen vftable>, m_hObject = NULL;
//     m_rectDraw = {0,0,0,0};
//     ENSURE(pParentDlg != NULL);                 // else AfxThrowInvalidArgException
//     m_pParentDlg = pParentDlg;
//     m_sizeImage = CSize(0,0); m_pBitmap = NULL; m_rgbColor = 0;
//     ::SetRectEmpty(&m_rectParentPreviewArea);
//     m_memDC.Attach(::CreateCompatibleDC(NULL));  // 0x2a03c0
//     m_Mode = IMAGE_EDIT_MODE_PEN;
//     ::SetRectEmpty(&m_rectDraw);
//     m_penDraw.Attach(::CreatePen(PS_SOLID, 1, m_rgbColor));   // 0x2a1e10
// The six HCURSOR members are NOT touched (PreSubclassWindow loads them); that is
// retail behaviour and is kept.
// NOTE: this export was not on this file's work list (its previous body just
// returned pThis), but every other body here depends on m_memDC and m_penDraw
// having been set up by it, so it is implemented here.
// DEVIATION: OpenMFC cannot install the retail vftables (see the header comment).
extern "C" void* MS_ABI impl___0CMFCImagePaintArea__QEAA_PEAVCMFCImageEditorDialog___Z(
    void* pThis, void* pDialog) {
    if (!pThis) return pThis;
    PaintArea* p = Self(pThis);
    impl___0CWnd__QEAA_XZ(pThis);
    std::memset(&p->m_rectParentPreviewArea, 0, sizeof p->m_rectParentPreviewArea);
    p->m_memDC.vfptr = nullptr;
    p->m_memDC.m_hDC = nullptr;
    p->m_memDC.m_hAttribDC = nullptr;
    p->m_memDC.m_bPrinting = 0;
    p->m_sizeImage.cx = 0; p->m_sizeImage.cy = 0;
    p->m_sizeCell.cx = 0;  p->m_sizeCell.cy = 0;
    p->m_penDraw.vfptr = nullptr;    p->m_penDraw.m_hObject = nullptr;
    p->m_penStretch.vfptr = nullptr; p->m_penStretch.m_hObject = nullptr;
    std::memset(&p->m_rectDraw, 0, sizeof p->m_rectDraw);
    if (pDialog == nullptr) {
        impl__AfxThrowInvalidArgException__YAXXZ();
        return pThis;   // not reached: the thunk raises
    }
    p->m_pParentDlg = pDialog;
    p->m_sizeImage.cx = 0; p->m_sizeImage.cy = 0;
    p->m_pBitmap = nullptr;
    p->m_rgbColor = 0;
    ::SetRectEmpty(&p->m_rectParentPreviewArea);
    impl__Attach_CDC__QEAAHPEAUHDC_____Z(MemDC(p), ::CreateCompatibleDC(nullptr));
    p->m_Mode = kModePen;
    ::SetRectEmpty(&p->m_rectDraw);
    impl__Attach_CGdiObject__QEAAHPEAX_Z(Gdi(&p->m_penDraw), ::CreatePen(PS_SOLID, 1, p->m_rgbColor));
    return pThis;
}

// Symbol: ??1CMFCImagePaintArea@@UEAA@XZ
// Retail (RVA 0x709a0, mfc140.dll), fully transcribed:
//     <restore class vftable>
//     ::DestroyCursor(m_hcurPen); ... (m_hcurFill, m_hcurLine, m_hcurRect,
//     m_hcurEllipse, m_hcurColor) -- six unconditional calls (import resolved
//     with iat.py to USER32!DestroyCursor)
//     m_penStretch.~CPen();   m_penDraw.~CPen();   // inline ~CGdiObject: DeleteObject
//     m_memDC.~CDC();                              // 0x2a04a0
//     CButton::~CButton();                         // tail jump to 0x292040
// DEVIATION: retail ~CDC (0x2a04a0) does `if (m_hDC) ::DeleteDC(Detach())`, but
// OpenMFC's ??1CDC thunk (core/gdi/CDC.cpp) only clears the handles and would leak
// the compatible DC.  CDC::DeleteDC is therefore called first to get the retail
// effect; it leaves m_hDC NULL, so a corrected ~CDC cannot double-delete.
extern "C" void MS_ABI impl___1CMFCImagePaintArea__UEAA_XZ(void* pThis) {
    if (!pThis) return;
    PaintArea* p = Self(pThis);
    ::DestroyCursor(p->m_hcurPen);
    ::DestroyCursor(p->m_hcurFill);
    ::DestroyCursor(p->m_hcurLine);
    ::DestroyCursor(p->m_hcurRect);
    ::DestroyCursor(p->m_hcurEllipse);
    ::DestroyCursor(p->m_hcurColor);
    impl__DeleteObject_CGdiObject__QEAAHXZ(Gdi(&p->m_penStretch));
    impl__DeleteObject_CGdiObject__QEAAHXZ(Gdi(&p->m_penDraw));
    impl__DeleteDC_CDC__QEAAHXZ(MemDC(p));
    impl___1CDC__UEAA_XZ(MemDC(p));
    impl___1CButton__UEAA_XZ(pThis);
}

// Symbol: ?BitmapToClient@CMFCImagePaintArea@@IEAAXAEAVCRect@@@Z
// Retail (RVA 0x71f10, mfc140.dll), fully transcribed -- no NULL test, unguarded:
//     rect.left   = rect.left   * m_sizeCell.cx + 1;
//     rect.top    = rect.top    * m_sizeCell.cy + 1;
//     rect.right  = rect.right  * m_sizeCell.cx + 1;
//     rect.bottom = rect.bottom * m_sizeCell.cy + 1;
//     ::OffsetRect(&rect, m_sizeCell.cx / 2, m_sizeCell.cy / 2);   // tail jump
extern "C" void MS_ABI impl__BitmapToClient_CMFCImagePaintArea__IEAAXAEAVCRect___Z(void* pThis, RECT* pRect) {
    PaintArea* p = Self(pThis);
    pRect->left   = pRect->left   * p->m_sizeCell.cx + 1;
    pRect->top    = pRect->top    * p->m_sizeCell.cy + 1;
    pRect->right  = pRect->right  * p->m_sizeCell.cx + 1;
    pRect->bottom = pRect->bottom * p->m_sizeCell.cy + 1;
    ::OffsetRect(pRect, p->m_sizeCell.cx / 2, p->m_sizeCell.cy / 2);
}

// Symbol: ?DrawItem@CMFCImagePaintArea@@MEAAXPEAUtagDRAWITEMSTRUCT@@@Z
// Retail (RVA 0x70a60, mfc140.dll), fully transcribed:
//     CDC* pDC = CDC::FromHandle(lpDIS->hDC);                         // 0x2a0390
//     CRect rectClient = lpDIS->rcItem;                                // ::CopyRect
//     ::FillRect(pDC->m_hDC, &rectClient, afxGlobalData.brBtnFace);   // +0xb0
//     rectClient.InflateRect(-1, -1);
//     CRect rectDraw = rectClient;
//     rectDraw.right  = rectDraw.left + m_sizeImage.cx * m_sizeCell.cx;
//     rectDraw.bottom = rectDraw.top  + m_sizeImage.cy * m_sizeCell.cy;
//     rectClient = rectDraw; rectClient.InflateRect(1, 1);   // result never read
//     pDC->Draw3dRect(&rectDraw, afxGlobalData.clrBtnDkShadow, afxGlobalData.clrBtnHilite);
//     CPen pen(PS_SOLID, 1, afxGlobalData.clrBtnShadow);              // 0x2a1ed0
//     CPen* pOldPen = pDC->SelectObject(&pen);
//     for (x = rectDraw.left + m_sizeCell.cx; x <= rectDraw.right - m_sizeCell.cx; x += m_sizeCell.cx)
//         { pDC->MoveTo(x, rectDraw.top + 1);  pDC->LineTo(x, rectDraw.bottom - 1); }
//     for (y = rectDraw.top + m_sizeCell.cy; y <= rectDraw.bottom - m_sizeCell.cy; y += m_sizeCell.cy)
//         { pDC->MoveTo(rectDraw.left + 1, y); pDC->LineTo(rectDraw.right - 1, y); }
//     pDC->SelectObject(pOldPen);
//     if (m_pBitmap != NULL) {
//         CBitmap* pOldBitmap = m_memDC.SelectObject(m_pBitmap);
//         for (x = 0; x < m_sizeImage.cx; x++) for (y = 0; y < m_sizeImage.cy; y++) {
//             COLORREF color = CMFCToolBarImages::MapFromSysColor(
//                                  ::GetPixel(m_memDC.m_hDC, x, y), FALSE);  // 0x16d040
//             if (color != (COLORREF)-1) {
//                 CRect rectPixel(rectDraw.left + x * m_sizeCell.cx,
//                                 rectDraw.top  + y * m_sizeCell.cy, +m_sizeCell);
//                 rectPixel.InflateRect(-1, -1);
//                 pDC->FillSolidRect(&rectPixel, color);
//             }
//         }
//         m_memDC.SelectObject(pOldBitmap);
//     }
//     ~pen;                                        // inline ~CGdiObject: DeleteObject
// Each afxGlobalData read is preceded by the inline init gate (EnsureGlobalData).
// PRECONDITION inherited from retail: with m_sizeCell still zero (SetBitmap never
// called -- the constructor zeroes it) both grid loops step by 0 and never end.
// Retail avoids that because CMFCImageEditorDialog::OnInitDialog (0x6f560,
// mfc140.dll) calls SetBitmap (the call at 0x18006f609) before any paint; the
// loops are kept unguarded to match retail.
extern "C" void MS_ABI impl__DrawItem_CMFCImagePaintArea__MEAAXPEAUtagDRAWITEMSTRUCT___Z(
    void* pThis, DRAWITEMSTRUCT* lpDIS) {
    PaintArea* p = Self(pThis);
    CDC* pDC = impl__FromHandle_CDC__SAPEAV1_PEAUHDC_____Z(lpDIS->hDC);

    RECT rectClient;
    ::CopyRect(&rectClient, &lpDIS->rcItem);
    ::FillRect(pDC->m_hDC, &rectClient, GlobalDataBrush(kGlobalDataBrBtnFaceHandle));
    ::InflateRect(&rectClient, -1, -1);

    RECT rectDraw = ImageRect(p, rectClient);
    rectClient = rectDraw;
    ::InflateRect(&rectClient, 1, 1);   // retail does this too; the result is never read

    const COLORREF clrHilite = GlobalDataColor(kGlobalDataClrBtnHilite);
    const COLORREF clrDkShadow = GlobalDataColor(kGlobalDataClrBtnDkShadow);
    impl__Draw3dRect_CDC__QEAAXPEBUtagRECT__KK_Z(pDC, &rectDraw, clrDkShadow, clrHilite);

    alignas(void*) unsigned char penStorage[sizeof(CPen)] = {};
    CPen* pPen = reinterpret_cast<CPen*>(penStorage);
    impl___0CPen__QEAA_HHK_Z(pPen, PS_SOLID, 1, GlobalDataColor(kGlobalDataClrBtnShadow));
    CPen* pOldPen = impl__SelectObject_CDC__QEAAPEAVCPen__PEAV2__Z(pDC, pPen);

    for (int x = rectDraw.left + p->m_sizeCell.cx; x <= rectDraw.right - p->m_sizeCell.cx; x += p->m_sizeCell.cx) {
        impl__MoveTo_CDC__QEAA_AVCPoint__HH_Z(pDC, x, rectDraw.top + 1);
        impl__LineTo_CDC__QEAAHHH_Z(pDC, x, rectDraw.bottom - 1);
    }
    for (int y = rectDraw.top + p->m_sizeCell.cy; y <= rectDraw.bottom - p->m_sizeCell.cy; y += p->m_sizeCell.cy) {
        impl__MoveTo_CDC__QEAA_AVCPoint__HH_Z(pDC, rectDraw.left + 1, y);
        impl__LineTo_CDC__QEAAHHH_Z(pDC, rectDraw.right - 1, y);
    }
    impl__SelectObject_CDC__QEAAPEAVCPen__PEAV2__Z(pDC, pOldPen);

    if (p->m_pBitmap != nullptr) {
        CGdiObject* pOldBitmap = SelectGdi(p, p->m_pBitmap->m_hObject);
        for (int x = 0; x < p->m_sizeImage.cx; x++) {
            for (int y = 0; y < p->m_sizeImage.cy; y++) {
                const COLORREF color = impl__MapFromSysColor_CMFCToolBarImages__SAKKH_Z(
                    ::GetPixel(p->m_memDC.m_hDC, x, y), FALSE);
                if (color == static_cast<COLORREF>(-1)) continue;
                RECT rectPixel;
                rectPixel.left   = rectDraw.left + x * p->m_sizeCell.cx;
                rectPixel.top    = rectDraw.top  + y * p->m_sizeCell.cy;
                rectPixel.right  = rectPixel.left + p->m_sizeCell.cx;
                rectPixel.bottom = rectPixel.top  + p->m_sizeCell.cy;
                ::InflateRect(&rectPixel, -1, -1);
                impl__FillSolidRect_CDC__QEAAXPEBUtagRECT__K_Z(pDC, &rectPixel, color);
            }
        }
        SelectGdi(p, SafeHandle(pOldBitmap));
    }
    impl__DeleteObject_CGdiObject__QEAAHXZ(reinterpret_cast<CGdiObject*>(pPen));
}

// Symbol: ?DrawPixel@CMFCImagePaintArea@@IEAAXUtagPOINT@@@Z
// Retail (RVA 0x71980, mfc140.dll), fully transcribed:
//     CRect rectClient; ::GetClientRect(m_hWnd, &rectClient);
//     rectClient.InflateRect(-1, -1);
//     <ScreenToBitmap inlined on a copy of point -> (x, y)>
//     CRect rectPixel(rectClient.left + x * m_sizeCell.cx,
//                     rectClient.top  + y * m_sizeCell.cy, +m_sizeCell);
//     rectPixel.InflateRect(-1, -1);
//     CClientDC dc(this);                                          // 0x2a1a60
//     dc.FillSolidRect(&rectPixel, m_rgbColor);                    // 0x2a39e0
//     CBitmap* pOldBitmap = m_memDC.SelectObject(m_pBitmap);       // m_pBitmap may be NULL
//     ::SetPixel(m_memDC.m_hDC, x, y, CMFCToolBarImages::MapToSysColor(m_rgbColor, TRUE));
//     m_memDC.SelectObject(pOldBitmap);
//     GetParent()->InvalidateRect(m_rectParentPreviewArea);        // bErase TRUE
//     ~dc;                                                         // 0x2a1b20
extern "C" void MS_ABI impl__DrawPixel_CMFCImagePaintArea__IEAAXUtagPOINT___Z(void* pThis, long long ptPacked) {
    PaintArea* p = Self(pThis);
    RECT rectClient = { 0, 0, 0, 0 };
    ::GetClientRect(Hwnd(pThis), &rectClient);
    ::InflateRect(&rectClient, -1, -1);

    const POINT ptBmp = ToBitmap(p, Unpack(ptPacked));
    RECT rectPixel;
    rectPixel.left   = rectClient.left + ptBmp.x * p->m_sizeCell.cx;
    rectPixel.top    = rectClient.top  + ptBmp.y * p->m_sizeCell.cy;
    rectPixel.right  = rectPixel.left + p->m_sizeCell.cx;
    rectPixel.bottom = rectPixel.top  + p->m_sizeCell.cy;
    ::InflateRect(&rectPixel, -1, -1);

    alignas(void*) unsigned char dcStorage[sizeof(CClientDC)] = {};
    CClientDC* pDC = reinterpret_cast<CClientDC*>(dcStorage);
    impl___0CClientDC__QEAA_PEAVCWnd___Z(pDC, AsWnd(pThis));
    impl__FillSolidRect_CDC__QEAAXPEBUtagRECT__K_Z(pDC, &rectPixel, p->m_rgbColor);

    CGdiObject* pOldBitmap = SelectGdi(p, SafeHandle(p->m_pBitmap));
    ::SetPixel(p->m_memDC.m_hDC, ptBmp.x, ptBmp.y,
               impl__MapToSysColor_CMFCToolBarImages__SAKKH_Z(p->m_rgbColor, TRUE));
    SelectGdi(p, SafeHandle(pOldBitmap));

    InvalidateParentPreview(pThis);
    impl___1CClientDC__UEAA_XZ(pDC);
}

// Symbol: ?FloodFill@CMFCImagePaintArea@@IEAAXAEBVCPoint@@@Z
// Retail (RVA 0x71f80, mfc140.dll), fully transcribed:
//     ENSURE(m_pBitmap != NULL);                   // AfxThrowInvalidArgException
//     CBitmap* pOldBitmap = m_memDC.SelectObject(m_pBitmap);
//     CBrush br(m_rgbColor);                       // 0x2a1fa0
//     CBrush* pOldBrush = m_memDC.SelectObject(&br);
//     ::ExtFloodFill(m_memDC.m_hDC, point.x, point.y,
//                    ::GetPixel(m_memDC.m_hDC, point.x, point.y), FLOODFILLSURFACE);
//     m_memDC.SelectObject(pOldBitmap);
//     m_memDC.SelectObject(pOldBrush);
//     ::InvalidateRect(m_hWnd, NULL, TRUE);  ::UpdateWindow(m_hWnd);
//     GetParent()->InvalidateRect(m_rectParentPreviewArea);
//     ~br;                                         // inline ~CGdiObject: DeleteObject
// Retail issues both brush SelectObject calls to 0x2a0670, which the symbol map
// names ?SelectObject@CDC@@UEAAPEAVCFont@@ (CDC vftable slot 12).  The CPen and
// CBrush overloads have no RVA of their own in either map, which is consistent
// with the linker having folded the three identical bodies (an inference, not
// checked against an export table).  Here the calls go to the CBrush overload
// thunk.
extern "C" void MS_ABI impl__FloodFill_CMFCImagePaintArea__IEAAXAEBVCPoint___Z(void* pThis, const POINT* pPoint) {
    PaintArea* p = Self(pThis);
    if (p->m_pBitmap == nullptr) {
        impl__AfxThrowInvalidArgException__YAXXZ();
        return;   // not reached: the thunk raises
    }
    CGdiObject* pOldBitmap = SelectGdi(p, p->m_pBitmap->m_hObject);

    alignas(void*) unsigned char brushStorage[sizeof(CBrush)] = {};
    CBrush* pBrush = reinterpret_cast<CBrush*>(brushStorage);
    impl___0CBrush__QEAA_K_Z(pBrush, p->m_rgbColor);
    CBrush* pOldBrush = impl__SelectObject_CDC__QEAAPEAVCBrush__PEAV2__Z(MemDC(p), pBrush);

    ::ExtFloodFill(p->m_memDC.m_hDC, pPoint->x, pPoint->y,
                   ::GetPixel(p->m_memDC.m_hDC, pPoint->x, pPoint->y), FLOODFILLSURFACE);

    SelectGdi(p, SafeHandle(pOldBitmap));
    impl__SelectObject_CDC__QEAAPEAVCBrush__PEAV2__Z(MemDC(p), pOldBrush);

    ::InvalidateRect(Hwnd(pThis), nullptr, TRUE);
    ::UpdateWindow(Hwnd(pThis));
    InvalidateParentPreview(pThis);
    impl__DeleteObject_CGdiObject__QEAAHXZ(reinterpret_cast<CGdiObject*>(pBrush));
}

// Symbol: ?OnCancelMode@CMFCImagePaintArea@@IEAAXXZ
// Retail (RVA 0x71950, mfc140.dll), fully transcribed -- no base-class call:
//     if (CWnd::FromHandle(::GetCapture()) == this) ::ReleaseCapture();
extern "C" void MS_ABI impl__OnCancelMode_CMFCImagePaintArea__IEAAXXZ(void* pThis) {
    if (impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(::GetCapture()) == AsWnd(pThis))
        ::ReleaseCapture();
}

// Symbol: ?OnEraseBkgnd@CMFCImagePaintArea@@IEAAHPEAVCDC@@@Z
// This export has no RVA in the symbol map, so its body was located through the
// retail message map instead: ?GetThisMessageMap@CMFCImagePaintArea@@ (RVA
// 0x70a50, mfc140.dll) returns the AFX_MSGMAP at 0x1802e9910 (mfc140.dll), whose
// WM_ERASEBKGND entry (nSig 1 = AfxSig_bD) points at RVA 0x3ae0 (mfc140.dll) --
// a two-instruction `mov $1,%eax; ret` that the linker folded with
// ?OnEraseBkgnd@CPaneTrackingWnd@@.  So: return TRUE.
extern "C" int MS_ABI impl__OnEraseBkgnd_CMFCImagePaintArea__IEAAHPEAVCDC___Z(void* pThis, CDC* pDC) {
    (void)pThis;
    (void)pDC;
    return TRUE;
}

// Symbol: ?OnLButtonDown@CMFCImagePaintArea@@IEAAXIVCPoint@@@Z
// Retail (RVA 0x712d0, mfc140.dll), fully transcribed:
//     if (m_pBitmap == NULL) return;
//     CRect rect; ::GetClientRect(m_hWnd, &rect);
//     rect.right  = rect.left + m_sizeImage.cx * m_sizeCell.cx;
//     rect.bottom = rect.top  + m_sizeImage.cy * m_sizeCell.cy;
//     rect.InflateRect(-1, -1);
//     if (!::PtInRect(&rect, point)) return;
//     switch (m_Mode) {
//     case PEN:                    DrawPixel(point); break;
//     case LINE: case RECT: case ELLIPSE:
//         <ScreenToBitmap inlined on a copy of point -> pt>
//         m_rectDraw = CRect(pt, pt); break;
//     }                            // FILL / COLOR: nothing
//     SetCapture();                // ::SetCapture(m_hWnd) + FromHandle, result unused
extern "C" void MS_ABI impl__OnLButtonDown_CMFCImagePaintArea__IEAAXIVCPoint___Z(
    void* pThis, unsigned int nFlags, long long ptPacked) {
    (void)nFlags;
    PaintArea* p = Self(pThis);
    if (p->m_pBitmap == nullptr) return;

    RECT rectClient = { 0, 0, 0, 0 };
    ::GetClientRect(Hwnd(pThis), &rectClient);
    RECT rect = ImageRect(p, rectClient);
    ::InflateRect(&rect, -1, -1);
    const POINT point = Unpack(ptPacked);
    if (!::PtInRect(&rect, point)) return;

    switch (p->m_Mode) {
    case kModePen:
        impl__DrawPixel_CMFCImagePaintArea__IEAAXUtagPOINT___Z(pThis, ptPacked);
        break;
    case kModeLine:
    case kModeRect:
    case kModeEllipse: {
        const POINT pt = ToBitmap(p, point);
        p->m_rectDraw.left = pt.x;  p->m_rectDraw.top = pt.y;
        p->m_rectDraw.right = pt.x; p->m_rectDraw.bottom = pt.y;
        break;
    }
    default:
        break;
    }
    impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(::SetCapture(Hwnd(pThis)));
}

// Symbol: ?OnLButtonUp@CMFCImagePaintArea@@IEAAXIVCPoint@@@Z
// Retail (RVA 0x71450, mfc140.dll), fully transcribed:
//     if (CWnd::FromHandle(::GetCapture()) == this) ::ReleaseCapture();
//     if (m_Mode == RECT || m_Mode == ELLIPSE) {   // make the rect inclusive
//         if (m_rectDraw.bottom != m_rectDraw.top)
//             { if (m_rectDraw.bottom > m_rectDraw.top) m_rectDraw.bottom++; else m_rectDraw.top++; }
//         if (m_rectDraw.right != m_rectDraw.left)
//             { if (m_rectDraw.right > m_rectDraw.left) m_rectDraw.right++; else m_rectDraw.left++; }
//     }
//     switch (m_Mode) {
//     case PEN:   DrawPixel(point); break;
//     case FILL:  { <ScreenToBitmap inlined -> pt>; FloodFill(pt); } break;
//     case LINE:
//         if (m_rectDraw.bottom == m_rectDraw.top && m_rectDraw.right == m_rectDraw.left)
//             { DrawPixel(point); break; }
//         pOldBitmap = m_memDC.SelectObject(m_pBitmap);
//         pOldPen = m_memDC.SelectObject(&m_penDraw);
//         m_memDC.MoveTo(m_rectDraw.left, m_rectDraw.top);        // 0x2a1000
//         m_memDC.LineTo(m_rectDraw.right, m_rectDraw.bottom);    // 0x2a1060
//         DrawPixel(point);
//         m_memDC.SelectObject(pOldBitmap); m_memDC.SelectObject(pOldPen);
//         goto repaint;
//     case RECT: case ELLIPSE:
//         if (<same single-point test>) { DrawPixel(point); break; }
//         pOldBitmap = m_memDC.SelectObject(m_pBitmap);
//         pOldPen = m_memDC.SelectObject(&m_penDraw);
//         pOldBrush = m_memDC.SelectStockObject(NULL_BRUSH);      // virtual, slot 11
//         ::Rectangle / ::Ellipse(m_memDC.m_hDC, m_rectDraw.left, .top, .right, .bottom);
//         m_memDC.SelectObject(pOldBitmap); m_memDC.SelectObject(pOldPen);
//         m_memDC.SelectObject(pOldBrush);                         // 0x2a0670, see FloodFill
//     repaint:
//         ::InvalidateRect(m_hWnd, NULL, TRUE); ::UpdateWindow(m_hWnd);
//         GetParent()->InvalidateRect(m_rectParentPreviewArea);
//         break;
//     case COLOR: {
//         <ScreenToBitmap inlined -> pt>
//         pOldBitmap = m_memDC.SelectObject(m_pBitmap);
//         COLORREF color = ::GetPixel(m_memDC.m_hDC, pt.x, pt.y);
//         m_memDC.SelectObject(pOldBitmap);
//         m_pParentDlg->OnPickColor(color);                        // 0x6fd90
//         break; }
//     }
//     ::SetRectEmpty(&m_rectDraw);                // tail jump, every path
// Note that the LINE branch draws into the bitmap with m_penDraw and then ALSO
// calls DrawPixel(point); the single-point cases draw only through DrawPixel.
// OnPickColor is called through a correctly-typed declaration (see the
// declaration at the top of this file).
extern "C" void MS_ABI impl__OnLButtonUp_CMFCImagePaintArea__IEAAXIVCPoint___Z(
    void* pThis, unsigned int nFlags, long long ptPacked) {
    (void)nFlags;
    PaintArea* p = Self(pThis);
    if (impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(::GetCapture()) == AsWnd(pThis))
        ::ReleaseCapture();

    RECT& rd = p->m_rectDraw;
    if (p->m_Mode == kModeRect || p->m_Mode == kModeEllipse) {
        if (rd.bottom != rd.top) {
            if (rd.bottom > rd.top) rd.bottom++; else rd.top++;
        }
        if (rd.right != rd.left) {
            if (rd.right > rd.left) rd.right++; else rd.left++;
        }
    }

    const bool bSinglePoint = (rd.bottom == rd.top && rd.right == rd.left);
    bool bRepaint = false;

    switch (p->m_Mode) {
    case kModePen:
        impl__DrawPixel_CMFCImagePaintArea__IEAAXUtagPOINT___Z(pThis, ptPacked);
        break;

    case kModeFill: {
        const POINT pt = ToBitmap(p, Unpack(ptPacked));
        impl__FloodFill_CMFCImagePaintArea__IEAAXAEBVCPoint___Z(pThis, &pt);
        break;
    }

    case kModeLine: {
        if (bSinglePoint) {
            impl__DrawPixel_CMFCImagePaintArea__IEAAXUtagPOINT___Z(pThis, ptPacked);
            break;
        }
        CGdiObject* pOldBitmap = SelectGdi(p, SafeHandle(p->m_pBitmap));
        CPen* pOldPen = impl__SelectObject_CDC__QEAAPEAVCPen__PEAV2__Z(MemDC(p), Pen(&p->m_penDraw));
        impl__MoveTo_CDC__QEAA_AVCPoint__HH_Z(MemDC(p), rd.left, rd.top);
        impl__LineTo_CDC__QEAAHHH_Z(MemDC(p), rd.right, rd.bottom);
        impl__DrawPixel_CMFCImagePaintArea__IEAAXUtagPOINT___Z(pThis, ptPacked);
        SelectGdi(p, SafeHandle(pOldBitmap));
        impl__SelectObject_CDC__QEAAPEAVCPen__PEAV2__Z(MemDC(p), pOldPen);
        bRepaint = true;
        break;
    }

    case kModeRect:
    case kModeEllipse: {
        if (bSinglePoint) {
            impl__DrawPixel_CMFCImagePaintArea__IEAAXUtagPOINT___Z(pThis, ptPacked);
            break;
        }
        CGdiObject* pOldBitmap = SelectGdi(p, SafeHandle(p->m_pBitmap));
        CPen* pOldPen = impl__SelectObject_CDC__QEAAPEAVCPen__PEAV2__Z(MemDC(p), Pen(&p->m_penDraw));
        CGdiObject* pOldBrush = impl__SelectStockObject_CDC__UEAAPEAVCGdiObject__H_Z(MemDC(p), NULL_BRUSH);
        if (p->m_Mode == kModeRect)
            ::Rectangle(p->m_memDC.m_hDC, rd.left, rd.top, rd.right, rd.bottom);
        else
            ::Ellipse(p->m_memDC.m_hDC, rd.left, rd.top, rd.right, rd.bottom);
        SelectGdi(p, SafeHandle(pOldBitmap));
        impl__SelectObject_CDC__QEAAPEAVCPen__PEAV2__Z(MemDC(p), pOldPen);
        impl__SelectObject_CDC__QEAAPEAVCBrush__PEAV2__Z(MemDC(p), reinterpret_cast<CBrush*>(pOldBrush));
        bRepaint = true;
        break;
    }

    case kModeColor: {
        const POINT pt = ToBitmap(p, Unpack(ptPacked));
        CGdiObject* pOldBitmap = SelectGdi(p, SafeHandle(p->m_pBitmap));
        const COLORREF color = ::GetPixel(p->m_memDC.m_hDC, pt.x, pt.y);
        SelectGdi(p, SafeHandle(pOldBitmap));
        impl__OnPickColor_CMFCImageEditorDialog__IEAAHK_Z(p->m_pParentDlg, color);
        break;
    }

    default:
        break;
    }

    if (bRepaint) {
        ::InvalidateRect(Hwnd(pThis), nullptr, TRUE);
        ::UpdateWindow(Hwnd(pThis));
        InvalidateParentPreview(pThis);
    }
    ::SetRectEmpty(&rd);
}

// Symbol: ?OnMouseMove@CMFCImagePaintArea@@IEAAXIVCPoint@@@Z
// Retail (RVA 0x70de0, mfc140.dll), fully transcribed:
//     if ((nFlags & MK_LBUTTON) == 0) return;
//     CRect rectClient; ::GetClientRect(m_hWnd, &rectClient);
//     point.x = min(max(point.x, rectClient.left), rectClient.right);
//     point.y = min(max(point.y, rectClient.top),  rectClient.bottom);
//     switch (m_Mode) {
//     case PEN: DrawPixel(point); break;
//     case LINE: case RECT: case ELLIPSE: {
//         CRect rectDraw = rectClient;
//         rectDraw.right  = rectDraw.left + m_sizeImage.cx * m_sizeCell.cx;
//         rectDraw.bottom = rectDraw.top  + m_sizeImage.cy * m_sizeCell.cy;
//         rectDraw.InflateRect(-1, -1);
//         if (::EqualRect(&m_rectDraw, &rectDraw)) break;   // import: USER32!EqualRect
//         CRgn rgn; rgn.CreateRectRgnIndirect(&rectDraw);     // ::CreateRectRgnIndirect + Attach
//         CClientDC dc(this);
//         dc.SelectObject(&rgn);                              // 0x2a06d0
//         CPen* pOldPen = dc.SelectObject(&m_penStretch);
//         CGdiObject* pOldBrush = dc.SelectStockObject(NULL_BRUSH);   // 0x2a0610, direct
//         int nOldROP = dc.SetROP2(R2_NOT);                   // 0x2a0820
//         // rubber band: erase the old shape (XOR), move the end point, draw the new one
//         for (pass = 0; pass < 2; pass++) {
//             if (pass == 1) {
//                 <ScreenToBitmap inlined on point -> pt>
//                 m_rectDraw.right = pt.x; m_rectDraw.bottom = pt.y;
//             }
//             CRect rect = m_rectDraw; BitmapToClient(rect);
//             if (!(m_rectDraw.right == m_rectDraw.left && m_rectDraw.bottom == m_rectDraw.top))
//                 LINE:    dc.MoveTo(rect.left, rect.top); dc.LineTo(rect.right, rect.bottom);
//                 RECT:    ::Rectangle(dc.m_hDC, rect.left, rect.top, rect.right, rect.bottom);
//                 ELLIPSE: ::Ellipse  (dc.m_hDC, rect.left, rect.top, rect.right, rect.bottom);
//         }
//         dc.SetROP2(nOldROP);
//         dc.SelectObject(pOldBrush); dc.SelectObject(pOldPen);   // both 0x2a0670, see FloodFill
//         dc.SelectClipRgn(NULL);                             // 0x2a0d60
//         ~dc; ~rgn;
//     } }                                                     // FILL / COLOR: nothing
// (Retail unrolls the two passes per mode; the loop above is only notation.)
extern "C" void MS_ABI impl__OnMouseMove_CMFCImagePaintArea__IEAAXIVCPoint___Z(
    void* pThis, unsigned int nFlags, long long ptPacked) {
    if ((nFlags & MK_LBUTTON) == 0) return;
    PaintArea* p = Self(pThis);

    RECT rectClient = { 0, 0, 0, 0 };
    ::GetClientRect(Hwnd(pThis), &rectClient);
    POINT point = Unpack(ptPacked);
    point.x = point.x > rectClient.left ? point.x : rectClient.left;
    if (point.x > rectClient.right) point.x = rectClient.right;
    point.y = point.y > rectClient.top ? point.y : rectClient.top;
    if (point.y > rectClient.bottom) point.y = rectClient.bottom;

    const int mode = p->m_Mode;
    if (mode == kModePen) {
        impl__DrawPixel_CMFCImagePaintArea__IEAAXUtagPOINT___Z(pThis, Pack(point));
        return;
    }
    if (mode != kModeLine && mode != kModeRect && mode != kModeEllipse) return;

    RECT rectDraw = ImageRect(p, rectClient);
    ::InflateRect(&rectDraw, -1, -1);
    if (::EqualRect(&p->m_rectDraw, &rectDraw)) return;

    alignas(void*) unsigned char rgnStorage[sizeof(CRgn)] = {};
    CRgn* pRgn = reinterpret_cast<CRgn*>(rgnStorage);
    impl__Attach_CGdiObject__QEAAHPEAX_Z(reinterpret_cast<CGdiObject*>(pRgn), ::CreateRectRgnIndirect(&rectDraw));

    alignas(void*) unsigned char dcStorage[sizeof(CClientDC)] = {};
    CClientDC* pDC = reinterpret_cast<CClientDC*>(dcStorage);
    impl___0CClientDC__QEAA_PEAVCWnd___Z(pDC, AsWnd(pThis));

    impl__SelectObject_CDC__QEAAHPEAVCRgn___Z(pDC, pRgn);
    CPen* pOldPen = impl__SelectObject_CDC__QEAAPEAVCPen__PEAV2__Z(pDC, Pen(&p->m_penStretch));
    CGdiObject* pOldBrush = impl__SelectStockObject_CDC__UEAAPEAVCGdiObject__H_Z(pDC, NULL_BRUSH);
    const int nOldROP = impl__SetROP2_CDC__QEAAHH_Z(pDC, R2_NOT);

    for (int pass = 0; pass < 2; ++pass) {
        if (pass == 1) {
            const POINT pt = ToBitmap(p, point);
            p->m_rectDraw.right = pt.x;
            p->m_rectDraw.bottom = pt.y;
        }
        RECT rect = p->m_rectDraw;
        impl__BitmapToClient_CMFCImagePaintArea__IEAAXAEAVCRect___Z(pThis, &rect);
        if (p->m_rectDraw.right == p->m_rectDraw.left && p->m_rectDraw.bottom == p->m_rectDraw.top)
            continue;
        if (mode == kModeLine) {
            impl__MoveTo_CDC__QEAA_AVCPoint__HH_Z(pDC, rect.left, rect.top);
            impl__LineTo_CDC__QEAAHHH_Z(pDC, rect.right, rect.bottom);
        } else if (mode == kModeRect) {
            ::Rectangle(pDC->m_hDC, rect.left, rect.top, rect.right, rect.bottom);
        } else {
            ::Ellipse(pDC->m_hDC, rect.left, rect.top, rect.right, rect.bottom);
        }
    }

    impl__SetROP2_CDC__QEAAHH_Z(pDC, nOldROP);
    impl__SelectObject_CDC__QEAAPEAVCBrush__PEAV2__Z(pDC, reinterpret_cast<CBrush*>(pOldBrush));
    impl__SelectObject_CDC__QEAAPEAVCPen__PEAV2__Z(pDC, pOldPen);
    impl__SelectClipRgn_CDC__QEAAHPEAVCRgn___Z(pDC, nullptr);
    impl___1CClientDC__UEAA_XZ(pDC);
    impl__DeleteObject_CGdiObject__QEAAHXZ(reinterpret_cast<CGdiObject*>(pRgn));
}

// Symbol: ?OnSetCursor@CMFCImagePaintArea@@IEAAHPEAVCWnd@@II@Z
// Retail (RVA 0x71c90, mfc140.dll), fully transcribed:
//     CPoint ptCursor(0, 0); ::GetCursorPos(&ptCursor); ::ScreenToClient(m_hWnd, &ptCursor);
//     CRect rect; ::GetClientRect(m_hWnd, &rect);
//     rect.right  = rect.left + m_sizeImage.cx * m_sizeCell.cx;
//     rect.bottom = rect.top  + m_sizeImage.cy * m_sizeCell.cy;   // no InflateRect here
//     if (::PtInRect(&rect, ptCursor)) {
//         HCURSOR h per m_Mode: PEN m_hcurPen, FILL m_hcurFill, LINE m_hcurLine,
//                               RECT m_hcurRect, ELLIPSE m_hcurEllipse, COLOR m_hcurColor;
//         if (m_Mode is one of those six) { ::SetCursor(h); return TRUE; }
//     }
//     return (BOOL)Default();      // 0x289090 -- CWnd::Default, not CWnd::OnSetCursor
extern "C" int MS_ABI impl__OnSetCursor_CMFCImagePaintArea__IEAAHPEAVCWnd__II_Z(
    void* pThis, CWnd* pWnd, unsigned int nHitTest, unsigned int message) {
    (void)pWnd; (void)nHitTest; (void)message;
    PaintArea* p = Self(pThis);
    POINT ptCursor = { 0, 0 };
    ::GetCursorPos(&ptCursor);
    ::ScreenToClient(Hwnd(pThis), &ptCursor);
    RECT rectClient = { 0, 0, 0, 0 };
    ::GetClientRect(Hwnd(pThis), &rectClient);
    const RECT rect = ImageRect(p, rectClient);
    if (::PtInRect(&rect, ptCursor)) {
        HCURSOR h = nullptr;
        bool bKnown = true;
        switch (p->m_Mode) {
        case kModePen:     h = p->m_hcurPen;     break;
        case kModeFill:    h = p->m_hcurFill;    break;
        case kModeLine:    h = p->m_hcurLine;    break;
        case kModeRect:    h = p->m_hcurRect;    break;
        case kModeEllipse: h = p->m_hcurEllipse; break;
        case kModeColor:   h = p->m_hcurColor;   break;
        default:           bKnown = false;       break;
        }
        if (bKnown) {
            ::SetCursor(h);
            return TRUE;
        }
    }
    return static_cast<int>(impl__Default_CWnd__IEAA_JXZ(AsWnd(pThis)));
}

// Symbol: ?PreSubclassWindow@CMFCImagePaintArea@@MEAAXXZ
// Retail (RVA 0x71da0, mfc140.dll), fully transcribed -- no base-class call:
// for each of the six cursors, in this order,
//     m_hcurPen     = ::LoadCursorW(AfxFindResourceHandle(MAKEINTRESOURCE(IDC_AFXBARRES_PEN),
//                                                         RT_GROUP_CURSOR),
//                                   MAKEINTRESOURCE(IDC_AFXBARRES_PEN));
//     m_hcurFill (FILL), m_hcurLine (LINE), m_hcurRect (RECT),
//     m_hcurEllipse (ELLIPSE), m_hcurColor (COLOR)
// (AfxFindResourceHandle is 0x2aca40; the import slot resolves to LoadCursorW.)
// Retail also calls AfxGetModuleState (0x1345b0) before each pair and discards the
// result; those six calls have no observable effect and are not reproduced.
extern "C" void MS_ABI impl__PreSubclassWindow_CMFCImagePaintArea__MEAAXXZ(void* pThis) {
    PaintArea* p = Self(pThis);
    auto load = [](int id) -> HCURSOR {
        HINSTANCE h = impl__AfxFindResourceHandle__YAPEAUHINSTANCE____PEB_W0_Z(
            MAKEINTRESOURCEW(id), MAKEINTRESOURCEW(12) /* RT_GROUP_CURSOR */);
        return ::LoadCursorW(h, MAKEINTRESOURCEW(id));
    };
    p->m_hcurPen     = load(kIdcPen);
    p->m_hcurFill    = load(kIdcFill);
    p->m_hcurLine    = load(kIdcLine);
    p->m_hcurRect    = load(kIdcRect);
    p->m_hcurEllipse = load(kIdcEllipse);
    p->m_hcurColor   = load(kIdcColor);
}

// Symbol: ?ScreenToBitmap@CMFCImagePaintArea@@IEAAXAEAVCPoint@@@Z
// Retail (RVA 0x71eb0, mfc140.dll), fully transcribed -- unguarded divisions:
//     point.x = max(0, min((point.x - 1) / m_sizeCell.cx, m_sizeImage.cx - 1));
//     point.y = max(0, min((point.y - 1) / m_sizeCell.cy, m_sizeImage.cy - 1));
extern "C" void MS_ABI impl__ScreenToBitmap_CMFCImagePaintArea__IEAAXAEAVCPoint___Z(void* pThis, POINT* pPoint) {
    *pPoint = ToBitmap(Self(pThis), *pPoint);
}

// Symbol: ?SetBitmap@CMFCImagePaintArea@@QEAAXPEAVCBitmap@@@Z
// Retail (RVA 0x71b50, mfc140.dll), fully transcribed:
//     m_pBitmap = pBitmap;
//     m_sizeCell = CSize(0, 0);
//     if (pBitmap == NULL) { m_sizeImage = CSize(0, 0); return; }
//     BITMAP bmp; ::GetObject(pBitmap->m_hObject, sizeof(BITMAP), &bmp);   // inline GetBitmap
//     m_sizeImage = CSize(bmp.bmWidth, bmp.bmHeight);
//     CRect rectClient; ::GetClientRect(m_hWnd, &rectClient); rectClient.InflateRect(-1, -1);
//     m_sizeCell.cx = rectClient.Width()  / m_sizeImage.cx;               // unguarded
//     m_sizeCell.cy = rectClient.Height() / m_sizeImage.cy;
//     if (m_penStretch.GetSafeHandle() != NULL) m_penStretch.DeleteObject();   // 0x2a1ea0
//     m_penStretch.Attach(::CreatePen(PS_SOLID, min(m_sizeCell.cx, m_sizeCell.cy),
//                                     afxGlobalData.clrBtnText));          // +0x34
// No Invalidate in this body.  The mfc140.dll import is GetObjectA; mfc140u.dll
// imports GetObjectW instead (same BITMAP layout).
extern "C" void MS_ABI impl__SetBitmap_CMFCImagePaintArea__QEAAXPEAVCBitmap___Z(void* pThis, CBitmap* pBitmap) {
    PaintArea* p = Self(pThis);
    p->m_pBitmap = pBitmap;
    p->m_sizeCell.cx = 0; p->m_sizeCell.cy = 0;
    if (pBitmap == nullptr) {
        p->m_sizeImage.cx = 0; p->m_sizeImage.cy = 0;
        return;
    }
    BITMAP bmp;
    std::memset(&bmp, 0, sizeof bmp);   // retail leaves it uninitialised if GetObject fails
    ::GetObjectW(pBitmap->m_hObject, sizeof(BITMAP), &bmp);
    p->m_sizeImage.cx = bmp.bmWidth;
    p->m_sizeImage.cy = bmp.bmHeight;

    RECT rectClient = { 0, 0, 0, 0 };
    ::GetClientRect(Hwnd(pThis), &rectClient);
    ::InflateRect(&rectClient, -1, -1);
    p->m_sizeCell.cx = (rectClient.right - rectClient.left) / p->m_sizeImage.cx;
    p->m_sizeCell.cy = (rectClient.bottom - rectClient.top) / p->m_sizeImage.cy;

    if (p->m_penStretch.m_hObject != nullptr)
        impl__DeleteObject_CGdiObject__QEAAHXZ(Gdi(&p->m_penStretch));
    const COLORREF clrText = GlobalDataColor(kGlobalDataClrBtnText);
    const int nWidth = p->m_sizeCell.cx <= p->m_sizeCell.cy ? p->m_sizeCell.cx : p->m_sizeCell.cy;
    impl__Attach_CGdiObject__QEAAHPEAX_Z(Gdi(&p->m_penStretch), ::CreatePen(PS_SOLID, nWidth, clrText));
}

// Symbol: ?SetColor@CMFCImagePaintArea@@QEAAXK@Z
// Retail (RVA 0x720b0, mfc140.dll), fully transcribed:
//     m_rgbColor = color;
//     if (m_penDraw.GetSafeHandle() != NULL) m_penDraw.DeleteObject();       // 0x2a1ea0
//     m_penDraw.Attach(::CreatePen(PS_SOLID, 1, m_rgbColor));                // tail jump 0x2a1e10
extern "C" void MS_ABI impl__SetColor_CMFCImagePaintArea__QEAAXK_Z(void* pThis, unsigned long color) {
    PaintArea* p = Self(pThis);
    p->m_rgbColor = color;
    if (p->m_penDraw.m_hObject != nullptr)
        impl__DeleteObject_CGdiObject__QEAAHXZ(Gdi(&p->m_penDraw));
    impl__Attach_CGdiObject__QEAAHPEAX_Z(Gdi(&p->m_penDraw), ::CreatePen(PS_SOLID, 1, p->m_rgbColor));
}
