// CMFCImageEditorDialog — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp, mfccore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/ManualSmallStubImplementationsSupport.h"
#include "detail/MfccoreSupport.h"

#include <cstddef>
#include <cstring>

// ---------------------------------------------------------------------------
// Object layout, pinned from the retail constructor
//   ??0CMFCImageEditorDialog@@QEAA@PEAVCBitmap@@PEAVCWnd@@H@Z
//   RVA 0x6f2e0 (mfc140) / RVA 0x6f260 (mfc140u)
// which constructs, in order: the CDialogEx base (0x3f05 = IDD_AFXBARRES_IMAGE_EDITOR
// passed to it), three CStatic members at 0x170 / 0x258 / 0x340 (CWnd ctor +
// CStatic vfptr each, so sizeof(CStatic) == 0xe8), CMFCImagePaintArea at 0x428
// (its ctor gets the dialog as argument), m_pBitmap = pBitmap at 0x5d0, zeroes
// m_sizeImage (0x5d8) and the two CRects (0x5e4..0x603), CMFCColorBar at 0x608
// and CMFCToolBar-derived CMFCImageEditorPaletteBar at 0x1b10.  It then fills
// m_sizeImage from ::GetObject(m_pBitmap->m_hObject, sizeof(BITMAP)) (bmWidth /
// bmHeight -> 0x5d8 / 0x5dc) and m_nBitsPixel (0x5e0) from bmBitsPixel when the
// nBitsPixel argument is -1.  Member order matches afximageeditordialog.h:47.
//
// NOTE: OpenMFC's constructor thunk (below) is still a stub that constructs none
// of this; the handlers below operate on the retail layout regardless.
// ---------------------------------------------------------------------------
namespace {

struct S_ImageEditorDlg {
    unsigned char _base0[0x40];                        // 0x000 CDialogEx base ...
    HWND          m_hWnd;                              // 0x040   ... CWnd::m_hWnd
    unsigned char _base48[0x170 - 0x48];               // 0x048   ... rest of CDialogEx
    unsigned char m_wndColorPickerLocation[0xe8];      // 0x170 CStatic
    unsigned char m_wndPaletteBarLocation[0xe8];       // 0x258 CStatic
    unsigned char m_wndPreview[0xe8];                  // 0x340 CStatic
    unsigned char m_wndLargeDrawArea[0x1a8];           // 0x428 CMFCImagePaintArea
    CGdiObject*   m_pBitmap;                           // 0x5d0 CBitmap*
    SIZE          m_sizeImage;                         // 0x5d8
    int           m_nBitsPixel;                        // 0x5e0
    RECT          m_rectPreviewFrame;                  // 0x5e4
    RECT          m_rectPreviewImage;                  // 0x5f4
    unsigned char _pad604[4];                          // 0x604 alignment
    unsigned char m_wndColorBar[0x1508];               // 0x608 CMFCColorBar
    unsigned char m_wndPaletteBar[1];                  // 0x1b10 CMFCImageEditorPaletteBar (size not pinned here)
};
static_assert(offsetof(S_ImageEditorDlg, m_hWnd) == 0x40, "CWnd::m_hWnd");
static_assert(offsetof(S_ImageEditorDlg, m_wndColorPickerLocation) == 0x170, "ctor 0x6f2e0 (mfc140), insn at 0x6f315");
static_assert(offsetof(S_ImageEditorDlg, m_wndPaletteBarLocation) == 0x258, "ctor 0x6f2e0 (mfc140), insn at 0x6f32e");
static_assert(offsetof(S_ImageEditorDlg, m_wndPreview) == 0x340, "ctor 0x6f2e0 (mfc140), insn at 0x6f340");
static_assert(offsetof(S_ImageEditorDlg, m_wndLargeDrawArea) == 0x428, "ctor 0x6f2e0 (mfc140), insn at 0x6f352");
static_assert(offsetof(S_ImageEditorDlg, m_pBitmap) == 0x5d0, "ctor 0x6f2e0 (mfc140), insn at 0x6f362");
static_assert(offsetof(S_ImageEditorDlg, m_sizeImage) == 0x5d8, "ctor 0x6f2e0 (mfc140), insn at 0x6f3d7");
static_assert(offsetof(S_ImageEditorDlg, m_nBitsPixel) == 0x5e0, "ctor 0x6f2e0 (mfc140), insn at 0x6f3ee");
static_assert(offsetof(S_ImageEditorDlg, m_rectPreviewFrame) == 0x5e4, "OnPaint 0x6fb20 (mfc140), FillRect rect arg");
static_assert(offsetof(S_ImageEditorDlg, m_rectPreviewImage) == 0x5f4, "OnPaint 0x6fb20 (mfc140), BitBlt x/y args");
static_assert(offsetof(S_ImageEditorDlg, m_wndColorBar) == 0x608, "ctor 0x6f2e0 (mfc140), insn at 0x6f38e");
static_assert(offsetof(S_ImageEditorDlg, m_wndPaletteBar) == 0x1b10, "ctor 0x6f2e0 (mfc140), insn at 0x6f39b");

// Offsets INSIDE the embedded members, as the retail handlers address them.
// CMFCImagePaintArea: CWnd::m_hWnd (+0x40 -> dialog 0x468, InvalidateRect in
// OnToolClear) and m_Mode (+0x1a0 -> dialog 0x5c8, written by the six mode
// handlers OnToolPen/Fill/Line/Rect/Ellipse/Pick and by OnPickColor, read by the
// six matching OnUpdateTool* handlers: SetMode()/GetMode() are inline,
// afximagepaintarea.h:75).  OnToolClear/Copy/Paste do not touch it.
constexpr std::size_t kPaintAreaHwnd = 0x40;
constexpr std::size_t kPaintAreaMode = 0x1a0;
// CMFCColorBar::m_ColorSelected (+0x14d0 -> dialog 0x1ad8), read by OnColors
// through the inline GetColor() (afxcolorbar.h:75).  detail/CMFCColorBarSupport.h
// names the same field `m_nAutoColor` ("current color (SetColor)").
constexpr std::size_t kColorBarSelected = 0x14d0;

// CMFCImagePaintArea::IMAGE_EDIT_MODE (afximagepaintarea.h:40).
enum : int {
    kModePen = 0, kModeFill = 1, kModeLine = 2, kModeRect = 3, kModeEllipse = 4, kModeColor = 5
};

inline S_ImageEditorDlg* D(void* p) { return static_cast<S_ImageEditorDlg*>(p); }

inline void SetPaintAreaMode(void* pThis, int mode) {
    std::memcpy(D(pThis)->m_wndLargeDrawArea + kPaintAreaMode, &mode, sizeof mode);
}
inline int GetPaintAreaMode(void* pThis) {
    int mode;
    std::memcpy(&mode, D(pThis)->m_wndLargeDrawArea + kPaintAreaMode, sizeof mode);
    return mode;
}
inline HWND PaintAreaHwnd(void* pThis) {
    HWND h;
    std::memcpy(&h, D(pThis)->m_wndLargeDrawArea + kPaintAreaHwnd, sizeof h);
    return h;
}
// `pBitmap->GetSafeHandle()` as retail inlines it: NULL pointer -> NULL handle,
// else the m_hObject at +8.
inline HGDIOBJ SafeHandle(const CGdiObject* p) { return p != nullptr ? p->m_hObject : nullptr; }
static_assert(offsetof(CGdiObject, m_hObject) == 8, "retail reads m_hObject at +8 (insn at 0x6fbce inside OnPaint 0x6fb20, mfc140)");

} // namespace

// afxGlobalData, read the way retail reads it.  Every handler below that reads
// afxGlobalData (OnColors, OnPaint, OnToolClear here; OnInitDialog and OnToolPaste
// in retail) inlines GetGlobalData(): `if (!m_bInitialized) { Initialize(); m_bInitialized = TRUE; }`
// -- the cmpl $0 / call 0x6a5c0 / movl $1 sequence on afxGlobalData+0.
// Field offsets, from AFX_GLOBAL_DATA (afxglobals.h:66) and confirmed by the
// absolute addresses the handlers load (afxGlobalData is 0x1803ba380 in mfc140):
//   +0x28 clrBtnFace, +0x2c clrBtnShadow, +0x30 clrBtnHilite,
//   +0xb0 brBtnFace.m_hObject (CBrush brBtnFace at +0xa8).
extern "C" unsigned char impl__afxGlobalData__3UAFX_GLOBAL_DATA__A[720];              // featurepack/CMFC_misc_stubs.cpp:3652
extern "C" void MS_ABI impl__Initialize_AFX_GLOBAL_DATA__QEAAXXZ(void* pThis);        // core/runtime/AFX_GLOBAL_DATA.cpp:675

namespace {
constexpr std::size_t kGD_clrBtnFace   = 0x28;
constexpr std::size_t kGD_clrBtnShadow = 0x2c;
constexpr std::size_t kGD_clrBtnHilite = 0x30;
constexpr std::size_t kGD_brBtnFace_h  = 0xb0;

inline void EnsureGlobalData() {
    int bInit;
    std::memcpy(&bInit, impl__afxGlobalData__3UAFX_GLOBAL_DATA__A, sizeof bInit);
    if (bInit == 0) {
        impl__Initialize_AFX_GLOBAL_DATA__QEAAXXZ(impl__afxGlobalData__3UAFX_GLOBAL_DATA__A);
        const int one = 1;
        std::memcpy(impl__afxGlobalData__3UAFX_GLOBAL_DATA__A, &one, sizeof one);
    }
}
template <typename T>
inline T GlobalData(std::size_t off) {
    T v;
    std::memcpy(&v, impl__afxGlobalData__3UAFX_GLOBAL_DATA__A + off, sizeof v);
    return v;
}
} // namespace

// ---------------------------------------------------------------------------
// Thunks this file calls.  C++ methods exist in this DLL only as impl__ thunks.
// ---------------------------------------------------------------------------
extern "C" void MS_ABI impl__DDX_Control__YAXPEAVCDataExchange__HAEAVCWnd___Z(void* pDX, int nIDC, void* pv);   // core/runtime/DdxExchange.cpp:255
extern "C" void MS_ABI impl__SetColor_CMFCColorBar__QEAAXK_Z(void* pThis, unsigned long clr);                  // featurepack/controls/CMFCColorBar.cpp:293
extern "C" void MS_ABI impl__OnUpdateCmdUI_CMFCToolBar__UEAAXPEAVCFrameWnd__H_Z(
    CMFCToolBar* pThis, CFrameWnd* pTarget, int bDisableIfNoHandler);                                          // featurepack/toolbar/CMFCToolBar.cpp:2221
extern "C" CPaintDC*  MS_ABI impl___0CPaintDC__QEAA_PEAVCWnd___Z(CPaintDC* pThis, CWnd* pWnd);                 // core/gdi/CPaintDC.cpp:12
extern "C" void       MS_ABI impl___1CPaintDC__UEAA_XZ(CPaintDC* pThis);                                        // core/gdi/CPaintDC.cpp:30
extern "C" CWindowDC* MS_ABI impl___0CWindowDC__QEAA_PEAVCWnd___Z(CWindowDC* pThis, CWnd* pWnd);               // core/gdi/CWindowDC.cpp:10
extern "C" void       MS_ABI impl___1CWindowDC__UEAA_XZ(CWindowDC* pThis);                                      // core/gdi/CWindowDC.cpp:25
extern "C" void MS_ABI impl__Draw3dRect_CDC__QEAAXPEBUtagRECT__KK_Z(
    CDC* pThis, const RECT* lpRect, unsigned long clrTopLeft, unsigned long clrBottomRight);                  // core/gdi/CDC.cpp:583
extern "C" int  MS_ABI impl__AfxMessageBox__YAHIII_Z(UINT nIDPrompt, UINT nType, UINT nIDHelp);                // core/collections/Globals.cpp:378
// CMFCImagePaintArea::SetColor -- featurepack/controls/CMFCImagePaintArea.cpp is
// still a generated placeholder whose parameter list LOST `this`
// (`void impl__SetColor_...(unsigned long p0) {}`).  The declaration here is the
// one the mangled name ?SetColor@CMFCImagePaintArea@@QEAAXK@Z describes; the
// placeholder ignores its arguments, so the call is a harmless no-op until that
// file is implemented (reported in headerRequests).
extern "C" void MS_ABI impl__SetColor_CMFCImagePaintArea__QEAAXK_Z(void* pThis, unsigned long color);

namespace {
// Retail message-box ids (afxribbonres.h).
constexpr UINT kIdpCantPasteBitmap = 16004;   // IDP_AFXBARRES_CANT_PASTE_BITMAP (0x3e84)
constexpr UINT kIdpCantCopyBitmap  = 16010;   // IDP_AFXBARRES_CANT_COPY_BITMAP  (0x3e8a)
// Dialog control ids (afxribbonres.h).
constexpr int kIdcDrawArea    = 16513;        // IDC_AFXBARRES_DRAW_AREA    (0x4081)
constexpr int kIdcColors      = 16516;        // IDC_AFXBARRES_COLORS       (0x4084)
constexpr int kIdcPreviewArea = 16519;        // IDC_AFXBARRES_PREVIEW_AREA (0x4087)
constexpr int kIdcPalette     = 16533;        // IDC_AFXBARRES_PALETTE      (0x4095)
} // namespace

// Symbol: ??0CMFCImageEditorDialog@@QEAA@PEAVCBitmap@@PEAVCWnd@@H@Z
extern "C" void* MS_ABI impl___0CMFCImageEditorDialog__QEAA_PEAVCBitmap__PEAVCWnd__H_Z(
    void* pThis, void* pBitmap, void* pWnd, int unusedFlags) {
    (void)pBitmap;
    (void)pWnd;
    (void)unusedFlags;
    return pThis;
}
// The six OnUpdateTool<mode> handlers, retail RVAs 0x70760..0x70800 (mfc140), are
// all the same shape:
//     cmpl $N, 0x5c8(%rcx); sete %dl; pCmdUI->SetCheck(dl)   // CCmdUI vslot 1 (+0x8)
// i.e. pCmdUI->SetCheck(GetMode() == IMAGE_EDIT_MODE_x).  They never call Enable.
// (The bodies these replace called Enable(TRUE) unconditionally, so the palette
// never showed which tool was selected.)  As elsewhere in this tree the CCmdUI
// virtual is reached through its impl__ thunk, which dispatches virtually on the
// OpenMFC side; retail does not null-check pCmdUI, the guard is a harmless deviation.
// Retail RVA 0x70760 (mfc140): pCmdUI->SetCheck(GetMode() == IMAGE_EDIT_MODE_ELLIPSE);
// Symbol: ?OnUpdateToolEllipse@CMFCImageEditorDialog@@IEAAXPEAVCCmdUI@@@Z
extern "C" void MS_ABI impl__OnUpdateToolEllipse_CMFCImageEditorDialog__IEAAXPEAVCCmdUI___Z(void* pThis, CCmdUI* ui) {
    if (ui) impl__SetCheck_CCmdUI__UEAAXH_Z(ui, GetPaintAreaMode(pThis) == kModeEllipse);
}
// Retail RVA 0x70780 (mfc140): pCmdUI->SetCheck(GetMode() == IMAGE_EDIT_MODE_FILL);
// Symbol: ?OnUpdateToolFill@CMFCImageEditorDialog@@IEAAXPEAVCCmdUI@@@Z
extern "C" void MS_ABI impl__OnUpdateToolFill_CMFCImageEditorDialog__IEAAXPEAVCCmdUI___Z(void* pThis, CCmdUI* ui) {
    if (ui) impl__SetCheck_CCmdUI__UEAAXH_Z(ui, GetPaintAreaMode(pThis) == kModeFill);
}
// Retail RVA 0x707a0 (mfc140): pCmdUI->SetCheck(GetMode() == IMAGE_EDIT_MODE_LINE);
// Symbol: ?OnUpdateToolLine@CMFCImageEditorDialog@@IEAAXPEAVCCmdUI@@@Z
extern "C" void MS_ABI impl__OnUpdateToolLine_CMFCImageEditorDialog__IEAAXPEAVCCmdUI___Z(void* pThis, CCmdUI* ui) {
    if (ui) impl__SetCheck_CCmdUI__UEAAXH_Z(ui, GetPaintAreaMode(pThis) == kModeLine);
}
// Retail RVA 0x706c0 (mfc140):
//     pCmdUI->Enable(::IsClipboardFormatAvailable(CF_BITMAP));   // CCmdUI vslot 0
// (IAT slot 0x1802c4dd0 resolved with iat.py.)  The body this replaces called
// Enable(TRUE) unconditionally.
// Symbol: ?OnUpdateToolPaste@CMFCImageEditorDialog@@IEAAXPEAVCCmdUI@@@Z
extern "C" void MS_ABI impl__OnUpdateToolPaste_CMFCImageEditorDialog__IEAAXPEAVCCmdUI___Z(void*, CCmdUI* ui) {
    if (ui) impl__Enable_CCmdUI__UEAAXH_Z(ui, ::IsClipboardFormatAvailable(CF_BITMAP));
}
// Retail RVA 0x707c0 (mfc140): pCmdUI->SetCheck(GetMode() == IMAGE_EDIT_MODE_PEN);
// Symbol: ?OnUpdateToolPen@CMFCImageEditorDialog@@IEAAXPEAVCCmdUI@@@Z
extern "C" void MS_ABI impl__OnUpdateToolPen_CMFCImageEditorDialog__IEAAXPEAVCCmdUI___Z(void* pThis, CCmdUI* ui) {
    if (ui) impl__SetCheck_CCmdUI__UEAAXH_Z(ui, GetPaintAreaMode(pThis) == kModePen);
}
// Retail RVA 0x707e0 (mfc140): pCmdUI->SetCheck(GetMode() == IMAGE_EDIT_MODE_COLOR);
// Symbol: ?OnUpdateToolPick@CMFCImageEditorDialog@@IEAAXPEAVCCmdUI@@@Z
extern "C" void MS_ABI impl__OnUpdateToolPick_CMFCImageEditorDialog__IEAAXPEAVCCmdUI___Z(void* pThis, CCmdUI* ui) {
    if (ui) impl__SetCheck_CCmdUI__UEAAXH_Z(ui, GetPaintAreaMode(pThis) == kModeColor);
}
// Retail RVA 0x70800 (mfc140): pCmdUI->SetCheck(GetMode() == IMAGE_EDIT_MODE_RECT);
// Symbol: ?OnUpdateToolRect@CMFCImageEditorDialog@@IEAAXPEAVCCmdUI@@@Z
extern "C" void MS_ABI impl__OnUpdateToolRect_CMFCImageEditorDialog__IEAAXPEAVCCmdUI___Z(void* pThis, CCmdUI* ui) {
    if (ui) impl__SetCheck_CCmdUI__UEAAXH_Z(ui, GetPaintAreaMode(pThis) == kModeRect);
}

// Retail RVA 0x6f4e0 (mfc140) / RVA 0x6f460 (mfc140u), fully transcribed -- four
// DDX_Control calls (0x291cd0 mfc140), the last one a tail jump.  No call to the
// base-class DoDataExchange is made.
//     DDX_Control(pDX, IDC_AFXBARRES_COLORS,       m_wndColorPickerLocation); // 0x170
//     DDX_Control(pDX, IDC_AFXBARRES_PALETTE,      m_wndPaletteBarLocation);  // 0x258
//     DDX_Control(pDX, IDC_AFXBARRES_PREVIEW_AREA, m_wndPreview);             // 0x340
//     DDX_Control(pDX, IDC_AFXBARRES_DRAW_AREA,    m_wndLargeDrawArea);       // 0x428
// Symbol: ?DoDataExchange@CMFCImageEditorDialog@@MEAAXPEAVCDataExchange@@@Z
extern "C" void MS_ABI impl__DoDataExchange_CMFCImageEditorDialog__MEAAXPEAVCDataExchange___Z(
    void* pThis, void* pDX) {
    S_ImageEditorDlg* d = D(pThis);
    impl__DDX_Control__YAXPEAVCDataExchange__HAEAVCWnd___Z(pDX, kIdcColors, d->m_wndColorPickerLocation);
    impl__DDX_Control__YAXPEAVCDataExchange__HAEAVCWnd___Z(pDX, kIdcPalette, d->m_wndPaletteBarLocation);
    impl__DDX_Control__YAXPEAVCDataExchange__HAEAVCWnd___Z(pDX, kIdcPreviewArea, d->m_wndPreview);
    impl__DDX_Control__YAXPEAVCDataExchange__HAEAVCWnd___Z(pDX, kIdcDrawArea, d->m_wndLargeDrawArea);
}

// Retail RVA 0x6fd40 (mfc140) / RVA 0x6fcc0 (mfc140u), fully transcribed:
//     COLORREF color = m_wndColorBar.GetColor();          // m_ColorSelected, 0x1ad8
//     if (color == RGB(192, 192, 192))                     // cmp $0xc0c0c0
//         color = GetGlobalData()->clrBtnFace;             // afxGlobalData+0x28
//     m_wndLargeDrawArea.SetColor(color);                  // tail jump to 0x720b0
// Symbol: ?OnColors@CMFCImageEditorDialog@@IEAAXXZ
extern "C" void MS_ABI impl__OnColors_CMFCImageEditorDialog__IEAAXXZ(void* pThis) {
    S_ImageEditorDlg* d = D(pThis);
    COLORREF color;
    std::memcpy(&color, d->m_wndColorBar + kColorBarSelected, sizeof color);
    if (color == RGB(192, 192, 192)) {
        EnsureGlobalData();
        color = GlobalData<COLORREF>(kGD_clrBtnFace);
    }
    impl__SetColor_CMFCImagePaintArea__QEAAXK_Z(d->m_wndLargeDrawArea, color);
}

// OnInitDialog -- left a stub.  Retail RVA 0x6f560 (mfc140) was disassembled: after
// CDialog::OnInitDialog it propagates WS_EX_LAYOUTRTL from the main window, calls
// m_wndLargeDrawArea.SetBitmap(m_pBitmap) (0x71b50 mfc140), then builds the palette
// toolbar at 0x1b10 through virtual calls on its vfptr -- slots read from the
// CMFCImageEditorPaletteBar vftable (0x1802e8d68, mfc140 image): +0x650 =
// CMFCToolBar::Create (0x14bec0), +0x670 = CMFCToolBar::LoadToolBar (0x14c970),
// +0x390/+0x3d8 (non-exported; used as Get/SetPaneStyle: |= 0x30, then &= ~0x400f00),
// +0x558 = CMFCToolBar::CalcSize (0x1535f0), +0x478 = CBasePane::MoveWindow (0xb580),
// +0x480 = CBasePane::SetWindowPos (0xb6a0) -- plus a direct call to the non-exported
// helper at 0x23a74 and to the exported CMFCToolBar::WrapToolBar (0x1538d0 mfc140),
// creates the colour bar through CMFCColorBar vslot 0x8b8 (with a 16-colour CPalette
// built via ::CreatePalette when m_nBitsPixel <= 8), and lays out the preview rectangles.
// None of that is reachable here: OpenMFC's constructor above never constructs the
// CMFCImageEditorPaletteBar / CMFCColorBar members (no vfptrs are written), there is no
// OpenMFC vftable for CMFCImageEditorPaletteBar, and CMFCImagePaintArea::SetBitmap is
// still a placeholder.  Returning 0 is the pre-existing stub value, not retail's (TRUE).
// Symbol: ?OnInitDialog@CMFCImageEditorDialog@@MEAAHXZ
extern "C" int MS_ABI impl__OnInitDialog_CMFCImageEditorDialog__MEAAHXZ(void* pThis) {
    (void)pThis;
    return 0;
}

// Retail RVA 0x6fd10 (mfc140), fully transcribed:
//     m_wndPaletteBar.OnUpdateCmdUI((CFrameWnd*)this, TRUE);   // vslot 0x498 (147)
//     return 0;
// Retail dispatches through the member's vfptr.  The member's static type is
// exactly CMFCImageEditorPaletteBar, whose vftable (0x1802e8d68 in the mfc140
// image, the vfptr the ctor loads at 0x6f3aa and stores at 0x6f3b1) holds ?OnUpdateCmdUI@CMFCToolBar@@
// (0x1502e0 mfc140) at +0x498 -- read from the image, not assumed.  So this calls
// the CMFCToolBar thunk directly, which is the same target without reading a
// vfptr OpenMFC's stub constructor never writes.
// Symbol: ?OnKickIdle@CMFCImageEditorDialog@@IEAA_J_K_J@Z
extern "C" __int64 MS_ABI impl__OnKickIdle_CMFCImageEditorDialog__IEAA_J_K_J_Z(
    void* pThis, unsigned __int64 wParam, __int64 lParam) {
    (void)wParam;
    (void)lParam;
    impl__OnUpdateCmdUI_CMFCToolBar__UEAAXPEAVCFrameWnd__H_Z(
        reinterpret_cast<CMFCToolBar*>(D(pThis)->m_wndPaletteBar),
        static_cast<CFrameWnd*>(pThis), TRUE);
    return 0;
}

// Retail RVA 0x6fb20 (mfc140), transcribed:
//     CPaintDC dc(this);                                             // 0x2a1c60
//     ::FillRect(dc.m_hDC, &m_rectPreviewFrame,                      // inline CDC::FillRect
//                GetGlobalData()->brBtnFace);                        // +0xb0
//     CDC dcMem; dcMem.Attach(::CreateCompatibleDC(dc.m_hDC));
//     CBitmap* pOld = dcMem.SelectObject(m_pBitmap);                 // NULL-safe handle
//     ::BitBlt(dc.m_hDC, m_rectPreviewImage.left, m_rectPreviewImage.top,
//              m_sizeImage.cx, m_sizeImage.cy, dcMem.m_hDC, 0, 0, SRCCOPY);
//     dc.Draw3dRect(&m_rectPreviewFrame, GetGlobalData()->clrBtnHilite,   // +0x30
//                   GetGlobalData()->clrBtnShadow);                        // +0x2c
//     dcMem.SelectObject(pOld);
//     dcMem.DeleteDC();                  // inline: if (m_hDC) ::DeleteDC(Detach())
//     // ~CDC dcMem (same check again, now a no-op), ~CPaintDC dc (0x2a1d10)
// DEVIATION (no observable effect): dcMem is a bare HDC rather than a stack CDC, and
// SelectObject results are kept as raw handles instead of CGdiObject::FromHandle
// temporaries -- retail only ever reads m_hObject back out of them.
// Symbol: ?OnPaint@CMFCImageEditorDialog@@IEAAXXZ
extern "C" void MS_ABI impl__OnPaint_CMFCImageEditorDialog__IEAAXXZ(void* pThis) {
    S_ImageEditorDlg* d = D(pThis);
    alignas(16) unsigned char dcStorage[sizeof(CPaintDC)] = {};
    CPaintDC* pDC = reinterpret_cast<CPaintDC*>(dcStorage);
    impl___0CPaintDC__QEAA_PEAVCWnd___Z(pDC, static_cast<CWnd*>(pThis));
    HDC hdc = pDC->m_hDC;

    EnsureGlobalData();
    ::FillRect(hdc, &d->m_rectPreviewFrame, GlobalData<HBRUSH>(kGD_brBtnFace_h));

    HDC hdcMem = ::CreateCompatibleDC(hdc);
    HGDIOBJ hOld = ::SelectObject(hdcMem, SafeHandle(d->m_pBitmap));

    ::BitBlt(hdc, d->m_rectPreviewImage.left, d->m_rectPreviewImage.top,
             d->m_sizeImage.cx, d->m_sizeImage.cy, hdcMem, 0, 0, SRCCOPY);

    EnsureGlobalData();
    impl__Draw3dRect_CDC__QEAAXPEBUtagRECT__KK_Z(pDC, &d->m_rectPreviewFrame,
        GlobalData<COLORREF>(kGD_clrBtnHilite), GlobalData<COLORREF>(kGD_clrBtnShadow));

    ::SelectObject(hdcMem, hOld);
    if (hdcMem != nullptr) ::DeleteDC(hdcMem);

    impl___1CPaintDC__UEAA_XZ(pDC);
}

// Retail RVA 0x6fd90 (mfc140), fully transcribed:
//     m_wndColorBar.SetColor(color);                            // 0x27040
//     m_wndLargeDrawArea.SetColor(color);                       // 0x720b0
//     SetMode(CMFCImagePaintArea::IMAGE_EDIT_MODE_PEN);         // store 0 at 0x5c8
//     return TRUE;
// Symbol: ?OnPickColor@CMFCImageEditorDialog@@IEAAHK@Z
extern "C" int MS_ABI impl__OnPickColor_CMFCImageEditorDialog__IEAAHK_Z(void* pThis, unsigned long color) {
    S_ImageEditorDlg* d = D(pThis);
    impl__SetColor_CMFCColorBar__QEAAXK_Z(d->m_wndColorBar, color);
    impl__SetColor_CMFCImagePaintArea__QEAAXK_Z(d->m_wndLargeDrawArea, color);
    SetPaintAreaMode(pThis, kModePen);
    return TRUE;
}

// Retail RVA 0x6fde0 (mfc140), transcribed:
//     CWindowDC dc(this);                                       // 0x2a1b60
//     CDC memDC; memDC.Attach(::CreateCompatibleDC(dc.m_hDC));
//     CBitmap* pOld = memDC.SelectObject(m_pBitmap);            // NULL-safe handle
//     CRect rect(0, 0, m_sizeImage.cx, m_sizeImage.cy);
//     ::FillRect(memDC.m_hDC, rect, GetGlobalData()->brBtnFace); // +0xb0
//     memDC.SelectObject(pOld);
//     ::InvalidateRect(m_hWnd, &m_rectPreviewImage, TRUE);
//     ::InvalidateRect(m_wndLargeDrawArea.m_hWnd, NULL, TRUE); // 0x468
//     // ~CDC memDC: if (m_hDC) ::DeleteDC(Detach());  ~CWindowDC dc (0x2a1c20)
// There is no m_pBitmap NULL guard (a NULL bitmap selects a NULL handle).
// DEVIATION (no observable effect): memDC / pOld as raw handles, as in OnPaint.
// Symbol: ?OnToolClear@CMFCImageEditorDialog@@IEAAXXZ
extern "C" void MS_ABI impl__OnToolClear_CMFCImageEditorDialog__IEAAXXZ(void* pThis) {
    S_ImageEditorDlg* d = D(pThis);
    alignas(16) unsigned char dcStorage[sizeof(CWindowDC)] = {};
    CWindowDC* pDC = reinterpret_cast<CWindowDC*>(dcStorage);
    impl___0CWindowDC__QEAA_PEAVCWnd___Z(pDC, static_cast<CWnd*>(pThis));

    HDC hdcMem = ::CreateCompatibleDC(pDC->m_hDC);
    HGDIOBJ hOld = ::SelectObject(hdcMem, SafeHandle(d->m_pBitmap));

    RECT rect = { 0, 0, d->m_sizeImage.cx, d->m_sizeImage.cy };
    EnsureGlobalData();
    ::FillRect(hdcMem, &rect, GlobalData<HBRUSH>(kGD_brBtnFace_h));

    ::SelectObject(hdcMem, hOld);

    ::InvalidateRect(d->m_hWnd, &d->m_rectPreviewImage, TRUE);
    ::InvalidateRect(PaintAreaHwnd(pThis), nullptr, TRUE);

    if (hdcMem != nullptr) ::DeleteDC(hdcMem);
    impl___1CWindowDC__UEAA_XZ(pDC);
}

// Retail RVA 0x6ff40 (mfc140), transcribed:
//     if (m_pBitmap == NULL) return;
//     CWindowDC dc(this);
//     CDC memDCDst; memDCDst.Attach(::CreateCompatibleDC(NULL));
//     CDC memDCSrc; memDCSrc.Attach(::CreateCompatibleDC(NULL));
//     CBitmap bitmapCopy;
//     if (!bitmapCopy.Attach(::CreateCompatibleBitmap(dc.m_hDC,
//                                m_sizeImage.cx, m_sizeImage.cy))) {    // 0x2a1e10
//         AfxMessageBox(IDP_AFXBARRES_CANT_COPY_BITMAP);  return;       // (id, 0, -1)
//     }
//     CBitmap* pOldDst = memDCDst.SelectObject(&bitmapCopy);
//     CBitmap* pOldSrc = memDCSrc.SelectObject(m_pBitmap);
//     ::BitBlt(memDCDst.m_hDC, 0, 0, m_sizeImage.cx, m_sizeImage.cy,
//              memDCSrc.m_hDC, 0, 0, SRCCOPY);
//     memDCDst.SelectObject(pOldDst);
//     memDCSrc.SelectObject(pOldSrc);
//     if (!::OpenClipboard(m_hWnd)) { AfxMessageBox(...CANT_COPY...); return; }
//     if (!::EmptyClipboard()) { AfxMessageBox(...CANT_COPY...); ::CloseClipboard(); return; }
//     if (::SetClipboardData(CF_BITMAP, bitmapCopy.Detach()) == NULL)
//         AfxMessageBox(IDP_AFXBARRES_CANT_COPY_BITMAP);
//     ::CloseClipboard();
// Every `return` after `CWindowDC dc` is constructed (i.e. all but the first)
// runs ~CBitmap (non-exported 0x1c7a0 mfc140, which calls CGdiObject::DeleteObject
// 0x2a1ea0 -- DeleteObject if still attached), ~CDC (inlined) on
// memDCSrc then memDCDst (DeleteDC if attached) and ~CWindowDC, in that order.
// Note the bitmap is Detach()ed BEFORE SetClipboardData, so a failed
// SetClipboardData leaks it in retail too; that is reproduced.
// DEVIATION (no observable effect): the two memory DCs and bitmapCopy are raw
// handles released in the same order instead of stack CDC / CBitmap objects.
// Symbol: ?OnToolCopy@CMFCImageEditorDialog@@IEAAXXZ
extern "C" void MS_ABI impl__OnToolCopy_CMFCImageEditorDialog__IEAAXXZ(void* pThis) {
    S_ImageEditorDlg* d = D(pThis);
    if (d->m_pBitmap == nullptr) return;

    alignas(16) unsigned char dcStorage[sizeof(CWindowDC)] = {};
    CWindowDC* pDC = reinterpret_cast<CWindowDC*>(dcStorage);
    impl___0CWindowDC__QEAA_PEAVCWnd___Z(pDC, static_cast<CWnd*>(pThis));

    HDC hdcDst = ::CreateCompatibleDC(nullptr);
    HDC hdcSrc = ::CreateCompatibleDC(nullptr);

    HBITMAP hCopy = ::CreateCompatibleBitmap(pDC->m_hDC, d->m_sizeImage.cx, d->m_sizeImage.cy);
    if (hCopy == nullptr) {
        impl__AfxMessageBox__YAHIII_Z(kIdpCantCopyBitmap, 0, static_cast<UINT>(-1));
    } else {
        HGDIOBJ hOldDst = ::SelectObject(hdcDst, hCopy);
        HGDIOBJ hOldSrc = ::SelectObject(hdcSrc, SafeHandle(d->m_pBitmap));
        ::BitBlt(hdcDst, 0, 0, d->m_sizeImage.cx, d->m_sizeImage.cy, hdcSrc, 0, 0, SRCCOPY);
        ::SelectObject(hdcDst, hOldDst);
        ::SelectObject(hdcSrc, hOldSrc);

        if (!::OpenClipboard(d->m_hWnd)) {
            impl__AfxMessageBox__YAHIII_Z(kIdpCantCopyBitmap, 0, static_cast<UINT>(-1));
        } else if (!::EmptyClipboard()) {
            impl__AfxMessageBox__YAHIII_Z(kIdpCantCopyBitmap, 0, static_cast<UINT>(-1));
            ::CloseClipboard();
        } else {
            HBITMAP hDetached = hCopy;   // bitmapCopy.Detach(): ownership leaves the object
            hCopy = nullptr;
            if (::SetClipboardData(CF_BITMAP, hDetached) == nullptr) {
                impl__AfxMessageBox__YAHIII_Z(kIdpCantCopyBitmap, 0, static_cast<UINT>(-1));
            }
            ::CloseClipboard();
        }
    }

    // Stack unwinding order of the retail locals.
    if (hCopy != nullptr) ::DeleteObject(hCopy);   // ~CBitmap bitmapCopy
    if (hdcSrc != nullptr) ::DeleteDC(hdcSrc);     // ~CDC memDCSrc
    if (hdcDst != nullptr) ::DeleteDC(hdcDst);     // ~CDC memDCDst
    impl___1CWindowDC__UEAA_XZ(pDC);               // ~CWindowDC dc
}

// Retail RVA 0x70700 (mfc140): `movl $4, 0x5c8(%rcx); ret` --
// SetMode(CMFCImagePaintArea::IMAGE_EDIT_MODE_ELLIPSE).
// Symbol: ?OnToolEllipse@CMFCImageEditorDialog@@IEAAXXZ
extern "C" void MS_ABI impl__OnToolEllipse_CMFCImageEditorDialog__IEAAXXZ(void* pThis) {
    SetPaintAreaMode(pThis, kModeEllipse);
}

// Retail RVA 0x70710 (mfc140): `movl $1, 0x5c8(%rcx); ret` --
// SetMode(CMFCImagePaintArea::IMAGE_EDIT_MODE_FILL).
// Symbol: ?OnToolFill@CMFCImageEditorDialog@@IEAAXXZ
extern "C" void MS_ABI impl__OnToolFill_CMFCImageEditorDialog__IEAAXXZ(void* pThis) {
    SetPaintAreaMode(pThis, kModeFill);
}

// Retail RVA 0x70720 (mfc140): `movl $2, 0x5c8(%rcx); ret` --
// SetMode(CMFCImagePaintArea::IMAGE_EDIT_MODE_LINE).
// Symbol: ?OnToolLine@CMFCImageEditorDialog@@IEAAXXZ
extern "C" void MS_ABI impl__OnToolLine_CMFCImageEditorDialog__IEAAXXZ(void* pThis) {
    SetPaintAreaMode(pThis, kModeLine);
}

// OnToolPaste -- left a stub.  Retail RVA 0x70340 (mfc140) was disassembled: a stack
// COleDataObject with m_bClipboard = TRUE checks IsDataAvailable(CF_BITMAP), then
// ::OleGetClipboard + Attach(pDataObj, TRUE), IDataObject::GetData(CF_BITMAP,
// DVASPECT_CONTENT, lindex -1, tymed -1), ::GetObject on the returned HBITMAP, fills the
// image with brBtnFace, and copies the clipboard bitmap in centred (clamped to
// m_sizeImage) through CMFCToolBarImages::TransparentBlt(..., RGB(192,192,192), -1, -1)
// (0x16ddc0 mfc140) before invalidating the preview and the paint area; each failure
// shows IDP_AFXBARRES_CANT_PASTE_BITMAP.  It is NOT implemented because TransparentBlt
// (featurepack/toolbar/CMFCToolBarImages.cpp:943) is still an empty placeholder: a
// transcription would erase the user's image to the button face colour and paste
// nothing -- worse than doing nothing.
// Symbol: ?OnToolPaste@CMFCImageEditorDialog@@IEAAXXZ
extern "C" void MS_ABI impl__OnToolPaste_CMFCImageEditorDialog__IEAAXXZ(void* pThis) {
    (void)pThis;
}

// Retail RVA 0x70730 (mfc140): `movl $0, 0x5c8(%rcx); ret` --
// SetMode(CMFCImagePaintArea::IMAGE_EDIT_MODE_PEN).
// Symbol: ?OnToolPen@CMFCImageEditorDialog@@IEAAXXZ
extern "C" void MS_ABI impl__OnToolPen_CMFCImageEditorDialog__IEAAXXZ(void* pThis) {
    SetPaintAreaMode(pThis, kModePen);
}

// Retail RVA 0x70740 (mfc140): `movl $5, 0x5c8(%rcx); ret` --
// SetMode(CMFCImagePaintArea::IMAGE_EDIT_MODE_COLOR).
// Symbol: ?OnToolPick@CMFCImageEditorDialog@@IEAAXXZ
extern "C" void MS_ABI impl__OnToolPick_CMFCImageEditorDialog__IEAAXXZ(void* pThis) {
    SetPaintAreaMode(pThis, kModeColor);
}

// Retail RVA 0x70750 (mfc140): `movl $3, 0x5c8(%rcx); ret` --
// SetMode(CMFCImagePaintArea::IMAGE_EDIT_MODE_RECT).
// Symbol: ?OnToolRect@CMFCImageEditorDialog@@IEAAXXZ
extern "C" void MS_ABI impl__OnToolRect_CMFCImageEditorDialog__IEAAXXZ(void* pThis) {
    SetPaintAreaMode(pThis, kModeRect);
}
